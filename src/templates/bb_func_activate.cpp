#include <string>
#include <cstdint>
#include <cstdio>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "ab_abi.h"
#include "pin_va.h"
#include "rt.h"
extern int64_t kw_fnclevel;
extern int g_monitor_bin;
void mon_emit_call_bin(const char *fname);
void mon_emit_return_bin(const char *fname, DESCR_t retval);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* IR_FUNC_ACTIVATE — LADDER AB (2026-08-09 AB-2): per-DEFINE ACTIVATION BLOCK.                                                                                                                      */
/* SPITBOL manual Ch.8 pp.102-106: DEFINE'd function saves fname/formals/locals on a pushdown stack                                                                                                   */
/* at entry (here: an RBP frame) and restores them on RETURN/FRETURN/NRETURN.  Ch.16: &FNCLEVEL++                                                                                                     */
/* at call, -- at return.  ABI frozen in contracts/ab_abi.h (ONE AUTHORITY).                                                                                                                          */
/*                                                                                                                                                                                                    */
/* AB-2 DELIVERS:                                                                                                                                                                                     */
/*   α  = push rbp; mov rbp,rsp; sub rsp,K;                                                                                                                                                           */
/*        fill meta frame: wires (rcx/rdx from call site), entry-rsp, ANCHOR link/update;                                                                                                             */
/*        call rt_ab_enter_env(rbp) → Σ/wn/vtmark snapshot + k_level++;                                                                                                                              */
/*        save-set spill (GVA→frame); null GVA cells;                                                                                                                                                 */
/*        monitor call tap (g_monitor_bin guard + RO fname + mon_emit_call_bin);                                                                                                                       */
/*        jmp [fn_cell$<FN>] → lands at <FN>_act_α which IS here, after DEFINE store at AB-3.                                                                                                        */
/*        (body-jmp is the fn_cell indirect so AB-3 can flip the cell; static-direct fold at AB-5.)                                                                                                   */
/*   β  = 3-way on cl (AB_TC_RETURN/NRETURN/FRETURN):                                                                                                                                                */
/*        RETURN/NRETURN: result rax:rdx pre-restore (NRETURN: deref CALLER-SIDE post-restore);                                                                                                       */
/*        stash result → call rt_ab_leave_env(rbp, result, is_fail) → restore rax:rdx;                                                                                                               */
/*        restore save-set from frame; write &RTNTYPE;                                                                                                                                                */
/*        monitor return tap (g_monitor_bin guard + RO fname + mon_emit_return_bin(fname, result));                                                                                                   */
/*        unlink ANCHOR; LEAVE; jmp γ-wire (RETURN/NRETURN) or ω-wire (FRETURN).                                                                                                                     */
/*   fn_cell$<FN> = .data quad init'd to &rt_ab_undef_fn_stub (AB-3 stores &<FN>_act_α).                                                                                                            */
/*   Killswitch SCRIP_AB=0 → byte-identical legacy (lowerer gate; template unreached).                                                                                                               */
/*                                                                                                                                                                                                    */
/* DUAL ARM (anchor guard): if [RT_AB_ANCHOR]==0 on RETURN/FRETURN/NRETURN floater arrival,                                                                                                           */
/* fall through to legacy rt_flat_ret_snap path — so AB-2 and legacy calls coexist until AB-3.                                                                                                       */
/*                                                                                                                                                                                                    */
/* op_sval = fname (compile-time constant string)                                                                                                                                                     */
/* op_ival = nsave = 1 + nformals + nlocals                                                                                                                                                           */
/* op_arg_slot[k] = GVA index of save-set member k {fname, formal0..np-1, local0..nl-1}; -1 = no GVA */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define AB_FNCELL_MAX 256
static void * g_ab_fn_cells[AB_FNCELL_MAX];
static int    g_ab_fn_cell_n = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_func_activate() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    long   nsave = (long)_.op_ival;
    const char * fname = _.op_sval ? _.op_sval : "?";
    long   K     = ab_frame_k(nsave);
    /* ── fn_cell: allocate/record storage ── */
    void ** fn_cell_ptr = (void **)0;
    std::string fn_cell_lbl = std::string("fn_cell$") + fname;
    if (MEDIUM_BINARY) {
        int idx = g_ab_fn_cell_n < AB_FNCELL_MAX ? g_ab_fn_cell_n++ : 0;
        g_ab_fn_cells[idx] = (void *)(uintptr_t)rt_ab_undef_fn_stub;
        fn_cell_ptr = &g_ab_fn_cells[idx];
    }
    /* ── α ────────────────────────────────────────────────────────────────────────────────────── */
    std::string s =
        /* fn_cell .data (TEXT only; binary: runtime static array) */
        x86("directive", std::string(".section .data"))
      + x86("directive", std::string(".align 8"))
      + x86("directive", fn_cell_lbl + std::string(":"))
      + x86("directive", std::string(".quad rt_ab_undef_fn_stub"))
      + x86("directive", std::string(".section .text"))
      + x86("directive", std::string(".intel_syntax noprefix"))
        /* α label + RBP frame prologue */
      + x86_alpha()
      + x86("push", "rbp")
      + x86("mov",  "rbp", "rsp")
      + x86("sub",  "rsp", K)
        /* store γ/ω wires (arrive in rcx/rdx from call site — lea contract) */
      + x86("mov", RDQ("rbp", AB_OFF_GW), "rcx")
      + x86("mov", RDQ("rbp", AB_OFF_WW), "rdx")
        /* entry rsp = rbp + 8 (BEFORE push rbp: rsp was rbp+8 at caller's call point; +8 undoes the push) */
      + x86("lea", "rax", RDQ("rbp", 8))
      + x86("mov", RDQ("rbp", AB_OFF_ERSP), "rax")
        /* link ACT-ANCHOR: prev = [RT_AB_ANCHOR]; [RT_AB_ANCHOR] = rbp; store prev in frame */
      + x86("mov", "rax", ABSQ(RT_AB_ANCHOR))
      + x86("mov", RDQ("rbp", AB_OFF_ANCHOR), "rax")
      + x86("mov", ABSQ(RT_AB_ANCHOR), "rbp")
        /* store β address in frame so the shared floater can reach it via anchor chain */
      + x86("lea", "rax", L(1))               /* LEA rip-relative to β label (L(1) string → XK_ILBL → x86_lea_rip_id) */
      + x86("mov", RDQ("rbp", AB_OFF_BADDR), "rax")
        /* call rt_ab_enter_env(rbp) → Σ/wn/vtmark snapshot + k_level++; clobbers rax/rcx/rdx/rsi/rdi */
      + x86_align_enter()
      + x86("mov", "rdi", "rbp")
      + x86("call", "rt_ab_enter_env", (uint64_t)(uintptr_t)(void *)rt_ab_enter_env)
      + x86_align_leave()
        /* save-set: spill each GVA cell into RBP-relative frame slot */
      + FOR(0, (int)nsave, [&](int k) -> std::string {
            int gk  = (k < (int)(sizeof _.op_arg_slot / sizeof *_.op_arg_slot)) ? _.op_arg_slot[k] : -1;
            int ot  = ab_save_off(nsave, k);
            int ov  = ab_save_off(nsave, k) + 8;
            if (gk >= 0) {
                return x86("note", gva_name(gk))
                     + x86("mov", "rax", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(gk, 0) : ABSQ(RT_GVA_VA + (unsigned long)gk * 16))
                     + x86("mov", RDQ("rbp", ot), "rax")
                     + x86("mov", "rax", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(gk, 8) : ABSQ(RT_GVA_VA + (unsigned long)gk * 16 + 8))
                     + x86("mov", RDQ("rbp", ov), "rax");
            }
            return x86("mov", RDQ("rbp", ot), (long)0)
                 + x86("mov", RDQ("rbp", ov), (long)0);
        })
        /* null GVA cells (actuals installed by call site at AB-3) */
      + FOR(0, (int)nsave, [&](int k) -> std::string {
            int gk = (k < (int)(sizeof _.op_arg_slot / sizeof *_.op_arg_slot)) ? _.op_arg_slot[k] : -1;
            if (gk >= 0) {
                return x86("note", gva_name(gk))
                     + x86("xor",  "eax", "eax")
                     + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(gk, 0) : ABSQ(RT_GVA_VA + (unsigned long)gk * 16),     "rax")
                     + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(gk, 8) : ABSQ(RT_GVA_VA + (unsigned long)gk * 16 + 8), "rax");
            }
            return std::string();
        })
        /* monitor CALL tap — load g_monitor_bin value via movabs+mov; fname from RO slot 0 */
      + x86("movabs", "rax", (uint64_t)(uintptr_t)(void *)&g_monitor_bin)
      + x86("mov",    "rax", RDQ("rax", 0))    /* load int value; rax != 0 iff monitor active */
      + x86("test",   "rax", "rax")
      + x86("je",  L(2))             /* skip monitor tap */
      + x86_align_enter()
      + x86_ro_load_q("rdi", 0)     /* rdi = address of sealed fname string (RO slot 0) */
      + x86("call", "mon_emit_call_bin", (uint64_t)(uintptr_t)(void *)mon_emit_call_bin)
      + x86_align_leave()
      + x86("def", L(2))
        /* body-jmp: jmp [fn_cell$<FN>]  (TEXT: jmp qword ptr [rip + fn_cell$<FN>@GOTPCREL])
         *           (BINARY: movabs rax, &fn_cell_ptr; jmp [rax]) */
      + (MEDIUM_BINARY && fn_cell_ptr
          ? x86("movabs", "rax", (uint64_t)(uintptr_t)(void *)fn_cell_ptr)
            + x86("mov",  "rax", RDQ("rax", 0))
            + x86("jmp",  "rax")
          : x86("mov",  "rax", std::string("[rip@got + __]"), (uint64_t)0, fn_cell_lbl.c_str())
            + x86("jmp", "rax"))
      + x86_gamma()   /* dead after jmp; present for box structure */
    /* ── β — 3-way dispatch on cl (AB_TYPECODE_REG) ──────────────────────────────────────────── */
      + x86("def", L(1))   /* β: L(1) */
        /* Save type-code into r9 immediately — r9 is dead at β entry (was argreg at call site only).
         * r9 survives all C calls and the LEAVE; we read it after LEAVE for the final dispatch. */
      + x86("movzx", "r9", "cl")    /* r9 = type code: 0=RETURN 1=NRETURN 2=FRETURN */
        /* FRETURN: result is irrelevant; skip stash; call leave_env(rbp, FAILDESCR, 1) */
      + x86("cmp", "r9d", (long)AB_TC_FRETURN)
      + x86("je",  L(3))
        /* RETURN / NRETURN: stash result (rax:rdx) in frame before C call clobbers them */
      + x86("mov", RDQ("rbp", AB_OFF_RES0), "rax")
      + x86("mov", RDQ("rbp", AB_OFF_RES1), "rdx")
      + x86_align_enter()
      + x86("mov", "rdi", "rbp")
      + x86("mov", "rsi", RDQ("rbp", AB_OFF_RES0))   /* result.v */
      + x86("mov", "rdx", RDQ("rbp", AB_OFF_RES1))   /* result.i */
      + x86("xor", "ecx", "ecx")                     /* is_fail=0 */
      + x86("call", "rt_ab_leave_env", (uint64_t)(uintptr_t)(void *)rt_ab_leave_env)
      + x86_align_leave()
        /* rax:rdx = nret-fixed result from leave_env */
      + x86("mov", RDQ("rbp", AB_OFF_RES0), "rax")   /* re-stash: monitor tap reads from frame */
      + x86("mov", RDQ("rbp", AB_OFF_RES1), "rdx")
      + x86("jmp", L(4))
      + x86("def", L(3))             /* FRETURN path */
      + x86_align_enter()
      + x86("mov", "rdi", "rbp")
      + x86("xor", "esi", "esi")     /* result.v = 0 */
      + x86("xor", "edx", "edx")     /* result.i = 0 */
      + x86("mov", "ecx", (long)1)   /* is_fail=1 */
      + x86("call", "rt_ab_leave_env", (uint64_t)(uintptr_t)(void *)rt_ab_leave_env)
      + x86_align_leave()
      + x86("mov", RDQ("rbp", AB_OFF_RES0), "rax")   /* stash FAILDESCR for monitor */
      + x86("mov", RDQ("rbp", AB_OFF_RES1), "rdx")
      + x86("def", L(4))             /* common β tail */
        /* restore save-set: GVA cells ← frame (rcx is scratch; rax:rdx stashed in frame) */
      + FOR(0, (int)nsave, [&](int k) -> std::string {
            int gk  = (k < (int)(sizeof _.op_arg_slot / sizeof *_.op_arg_slot)) ? _.op_arg_slot[k] : -1;
            int ot  = ab_save_off(nsave, k);
            int ov  = ab_save_off(nsave, k) + 8;
            if (gk >= 0) {
                return x86("note", gva_name(gk))
                     + x86("mov", "rcx", RDQ("rbp", ot))
                     + x86("mov", ABSQ(RT_GVA_VA + (unsigned long)gk * 16),     "rcx")
                     + x86("mov", "rcx", RDQ("rbp", ov))
                     + x86("mov", ABSQ(RT_GVA_VA + (unsigned long)gk * 16 + 8), "rcx");
            }
            return std::string();
        })
        /* load γ/ω wires and prev ACT-ANCHOR from frame BEFORE LEAVE tears it down */
      + x86("mov", "r10", RDQ("rbp", AB_OFF_GW))     /* γ wire */
      + x86("mov", "r11", RDQ("rbp", AB_OFF_WW))     /* ω wire */
      + x86("mov", "rcx", RDQ("rbp", AB_OFF_ANCHOR)) /* prev ACT-ANCHOR */
        /* monitor RETURN tap — frame still live; result is in frame RES0/RES1 */
      + x86("movabs", "rax", (uint64_t)(uintptr_t)(void *)&g_monitor_bin)
      + x86("mov",    "rax", RDQ("rax", 0))
      + x86("test",   "rax", "rax")
      + x86("je",  L(5))
      + x86_align_enter()
      + x86_ro_load_q("rdi", 0)                       /* rdi = sealed fname ptr */
      + x86("mov", "rsi", RDQ("rbp", AB_OFF_RES0))   /* result.v */
      + x86("mov", "rdx", RDQ("rbp", AB_OFF_RES1))   /* result.i */
      + x86("call", "mon_emit_return_bin", (uint64_t)(uintptr_t)(void *)mon_emit_return_bin)
      + x86_align_leave()
      + x86("def", L(5))
        /* restore result to rax:rdx */
      + x86("mov", "rax", RDQ("rbp", AB_OFF_RES0))
      + x86("mov", "rdx", RDQ("rbp", AB_OFF_RES1))
        /* unlink ACT-ANCHOR: [RT_AB_ANCHOR] ← rcx (prev, loaded above) */
      + x86("mov", ABSQ(RT_AB_ANCHOR), "rcx")
        /* LEAVE — restores rsp = rbp, pops rbp; frame is gone after this */
      + x86("leave")
        /* dispatch on r9 (type-code saved before any C call, survives LEAVE):
         *   RETURN / NRETURN (r9 != 2) → γ wire in r10
         *   FRETURN          (r9 == 2) → ω wire in r11 */
      + x86("cmp", "r9d", (long)AB_TC_FRETURN)
      + x86("je",  L(6))
      + x86("jmp", "r10")            /* RETURN / NRETURN → γ */
      + x86("def", L(6))
      + x86("jmp", "r11")            /* FRETURN → ω */
      + x86_deflabel(X86P_OMEGA)     /* ω port label define only — never reached; dispatch above is exhaustive */
                /* RO fname string slot 0 — used by both monitor taps */
      + x86_ro_seal_str(0, fname);
    /* mode-3 fn_cell residual store */
    if (MEDIUM_BINARY && fn_cell_ptr) {
        *fn_cell_ptr = (void *)(uintptr_t)rt_ab_undef_fn_stub;
    }
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* bb_ab_emit_nodes — post-main-chain sweep: emit one IR_FUNC_ACTIVATE block per DEFINE fold.                                                                                                         */
/* Called from scrip.c after both m3 and m4 main-chain emission.  Saves/restores g_emit and related                                                                                                   */
/* globals so the sweep is side-effect-free from the caller's perspective.                                                                                                                             */
extern "C" void bb_ab_emit_nodes(IR_graph_t *g, int gva_active)
{
    if (!g || g->ab_n <= 0) return;
    extern int g_gva_active;
    extern IR_graph_t *g_emit_cfg;
    extern int gva_index_of(const char *);
    sm_emit_t saved_emit   = g_emit;
    IR_graph_t *saved_cfg  = g_emit_cfg;
    int         saved_gva  = g_gva_active;
    g_gva_active = gva_active;
    /* Local storage for per-function label name strings — kept alive until bb_emit_x86 consumes them */
    char ab_lbl_α[128], ab_lbl_β[128], ab_lbl_γ[128], ab_lbl_ω[128];
    for (int i = 0; i < g->ab_n; i++) {
        IR_t *nd = g->ab_nodes[i];
        if (!nd || nd->op != IR_FUNC_ACTIVATE) continue;
        g_emit         = saved_emit;
        g_emit_cfg     = g;
        g_emit.op_sval = IR_LIT(nd).sval;
        g_emit.op_ival = (long)nd->n_operands;
        g_emit.op_beta_dead = 0;   /* β has inbound edges from the floaters */
        /* Set unique per-function label names (ROMAN_act_α / _β / _γ / _ω) */
        const char *fn = g_emit.op_sval ? g_emit.op_sval : "unknown";
        snprintf(ab_lbl_α, sizeof ab_lbl_α, "%s_act_\xce\xb1", fn);
        snprintf(ab_lbl_β, sizeof ab_lbl_β, "%s_act_\xce\xb2", fn);
        snprintf(ab_lbl_γ, sizeof ab_lbl_γ, "%s_act_\xce\xb3", fn);
        snprintf(ab_lbl_ω, sizeof ab_lbl_ω, "%s_act_\xcf\x89", fn);
        g_emit.lbl_α = ab_lbl_α;
        g_emit.lbl_β = ab_lbl_β;
        g_emit.lbl_γ = ab_lbl_γ;
        g_emit.lbl_ω = ab_lbl_ω;
        int _ab_n = (int)nd->n_operands < (int)(sizeof g_emit.op_arg_slot / sizeof *g_emit.op_arg_slot)
                  ? (int)nd->n_operands
                  : (int)(sizeof g_emit.op_arg_slot / sizeof *g_emit.op_arg_slot);
        for (int k = 0; k < _ab_n; k++) {
            const char *nm = nd->operands[k] ? IR_LIT(nd->operands[k]).sval : (const char *)0;
            g_emit.op_arg_slot[k] = (nm && gva_active) ? gva_index_of(nm) : -1;
        }
        bb_emit_x86(bb_func_activate());
    }
    g_emit     = saved_emit;
    g_emit_cfg = saved_cfg;
    g_gva_active = saved_gva;
}
