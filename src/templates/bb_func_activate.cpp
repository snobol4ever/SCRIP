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
void *rt_proc_get_fn(const char *name);   /* RTX-FUNC-0: binary body-jmp target — proc JIT fn pointer, registered by the driver proc loop before bb_ab_emit_nodes fires */
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
static char   g_ab_fn_names[AB_FNCELL_MAX][64];
/* AB-3a: the ONE slot allocator, shared by the block template and the role-2 bind so bind-before-block ordering (main chain emits first, blocks post-chain) is immaterial — first request by fname allocates and initialises to the undef stub. */
static int bb_ab_slot_for(const char * fname) {
    for (int i = 0; i < g_ab_fn_cell_n; i++) if (!strncmp(g_ab_fn_names[i], fname, sizeof g_ab_fn_names[0] - 1)) return i;
    int idx = g_ab_fn_cell_n < AB_FNCELL_MAX ? g_ab_fn_cell_n++ : 0;
    snprintf(g_ab_fn_names[idx], sizeof g_ab_fn_names[idx], "%s", fname);
    g_ab_fn_cells[idx] = (void *)(uintptr_t)rt_ab_undef_fn_stub;
    return idx;
}
static void * bb_ab_cell_addr(const char * fname) { return (void *)&g_ab_fn_cells[bb_ab_slot_for(fname)]; }
void * bb_ab_fn_cell_ptr(const char * fname) { return bb_ab_cell_addr(fname); }   /* AB-3b: non-static accessor for call-site template (bb_call_proc_staged.cpp) — same slot the block and bind use, ONE allocator */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_func_activate() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    long   nsave    = (long)_.op_ival;
    long   nformals = (long)_.op_ab_nformals;   /* AB-3b: formals are save-set slots [1..nformals]; call site installs actuals there before jmp fn_cell; null loop must skip them */
    const char * fname = _.op_sval ? _.op_sval : "?";
    long   K     = ab_frame_k(nsave);
    /* ── fn_cell: allocate/record storage ── */
    void ** fn_cell_ptr = (void **)0;
    std::string fn_cell_lbl = std::string("fn_cell$") + fname;
    if (MEDIUM_BINARY) fn_cell_ptr = (void **)bb_ab_cell_addr(fname);   /* AB-3a: allocation moved into the ONE allocator (bb_ab_slot_for) so the role-2 bind and the block agree on the slot by fname regardless of emission order. */
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
            int gk  = (k < (int)_.op_arg_slot_n) ? _.op_arg_slot[k] : -1;
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
        /* null GVA cells: result cell (k=0) and locals (k>nformals) only.
         * Formals (k=1..nformals) are SKIPPED — the AB-3b call site installs actual arg values there
         * before jumping fn_cell$<FN>.  Pre-existing callers (non-AB path) reach this block through
         * fn_cell which still holds rt_ab_undef_fn_stub until DEFINE runs, so the non-AB path
         * never lands here; the ab_bind path writes INC_act_α and the call site installs args first. */
      + FOR(0, (int)nsave, [&](int k) -> std::string {
            if (k >= 1 && k <= (int)nformals) return std::string();   /* AB-3b: skip formals; call site pre-installs actuals */
            int gk = (k < (int)_.op_arg_slot_n) ? _.op_arg_slot[k] : -1;
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
        /* body-jmp: jmp proc_FN_α  (AB-3b/AB-5 static-direct fold: proc body label always known at emit time).
         * fn_cell$FN holds &FN_act_α (written by DEFINE's AB-3a residual action) and is the CALL-SITE target —
         * using it here would be a self-loop (fn_cell → INC_act_α = this block).  Direct label is both media:
         * TEXT: jmp proc_INC_α (resolved by the assembler); BINARY: movabs rax, rt_proc_get_fn(fname); jmp rax
         * (the proc is already JIT-compiled before bb_ab_emit_nodes runs; the fn pointer is registered by the
         * driver's proc loop; cross-session x86_jmp_lblptr is NOT used — proc_INC_α lives in a different label
         * pool that was reset when the proc's emit_chain session ended). */
      + [&]() -> std::string {
            char blbl[128];
            snprintf(blbl, sizeof blbl, "proc_%s_\xce\xb1", fname);
            if (MEDIUM_BINARY) {
                void *pfn = rt_proc_get_fn(fname);
                if (pfn) return x86_jmpfn(blbl, (uint64_t)(uintptr_t)pfn);
                /* Fallback if proc not yet registered — should not happen when hook fires post-proc-loop */
                return x86_bomb("bb_func_activate: proc fn not registered for binary body-jmp");
            }
            bb_label_t *bp = emit_label_intern(blbl);
            return x86_jmp_lblptr(bp, blbl);
        }()
      + x86_gamma()   /* dead after jmp; present for box structure */
    /* ── β — 3-way dispatch on cl (AB_TYPECODE_REG) ──────────────────────────────────────────── */
      + x86("def", L(1))   /* β: L(1) */
        /* Save type-code into r9 immediately — r9 is dead at β entry (was argreg at call site only).
         * r9 survives all C calls and the LEAVE; we read it after LEAVE for the final dispatch. */
      + x86("movzx", "r9", "cl")    /* r9 = type code: 0=RETURN 1=NRETURN 2=FRETURN */
        /* ADOPT THE FRAME (gdb conviction 2026-08-10): β arrives from the shared floater with the RETURNING STATEMENT's rbp, not this frame's — measured 0x88 below the anchor on the noarg repro.
         * Every access below (result stash, leave_env frame arg, save-set restore, GW/WW/prev loads, LEAVE) is rbp-relative, so without this adopt β works a FOREIGN frame: γ wire loaded 0 → jmp 0
         * (rip=0 crash), prev-anchor loaded dead stack garbage → anchor ← rt_ab_enter_env+107 (both gdb-measured).  The anchor is still linked here (unlink is below) and IS this frame's base. */
      + x86("mov", "rbp", ABSQ(RT_AB_ANCHOR))
        /* FRETURN: result is irrelevant; skip stash; call leave_env(rbp, FAILDESCR, 1) */
      + x86("cmp", "r9d", (long)AB_TC_FRETURN)
      + x86("je",  L(3))
        /* RETURN / NRETURN: the RESULT is the CURRENT value of the fname GVA cell (save-set k=0), read BEFORE the restore loop puts the saved pre-call value back — manual Ch.8: the value of the
         * function is the value of the fname variable AT RETURN TIME.  rax:rdx at β entry are statement residue, NOT the result (gdb/output conviction 2026-08-10: stashing them returned null on
         * every RETURN micro and fed garbage descrs to leave_env on NRETURN → SIGABRT).  Plain ABSQ form deliberately — β's restore loop below is ABSQ-only; the movzx r9 above already conflicts
         * with RTCC_GLOBAL_R9_GVA's r9=GVA-base claim, flagged in the cursor, so no GVARQ here until that coordination is ruled. */
      + [&]() -> std::string { int gk0 = (0 < (int)_.op_arg_slot_n) ? _.op_arg_slot[0] : -1;
            if (gk0 < 0) return x86("xor", "eax", "eax") + x86("xor", "edx", "edx");
            return x86("note", gva_name(gk0))
                 + x86("mov", "rax", ABSQ(RT_GVA_VA + (unsigned long)gk0 * 16))
                 + x86("mov", "rdx", ABSQ(RT_GVA_VA + (unsigned long)gk0 * 16 + 8)); }()
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
            int gk  = (k < (int)_.op_arg_slot_n) ? _.op_arg_slot[k] : -1;
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
        /* LEAVE — restores rsp = rbp, pops rbp; frame is gone after this.  Spelled as the mov/pop PAIR, not x86("leave"): there is NO "leave" arm in
         * x86_asm.h's dispatch, so x86("leave") emitted NOTHING in BOTH media — the callee frame was never torn down.  Neither encoder here touches
         * flags (as leave does not) and both are proven in-tree (x86_srf_floater; bb_glue_framed).  Adding a real "leave" encoder is the 1-byte form
         * but x86_asm.h is NOT-CONCURRENCY-SAFE — Lon routes that seat; this pair is byte-equivalent in effect and stays inside the template. */
      + x86("mov", "rsp", "rbp")
      + x86("pop", "rbp")
        /* dispatch on r9 (type-code saved before any C call, survives LEAVE):
         *   RETURN / NRETURN (r9 != 2) → γ wire in r10
         *   FRETURN          (r9 == 2) → ω wire in r11 */
      + x86("cmp", "r9d", (long)AB_TC_FRETURN)
      + x86("je",  L(6))
      + x86("jmp", "r10")            /* RETURN / NRETURN → γ */
      + x86("def", L(6))
      + x86("jmp", "r11")            /* FRETURN → ω */
      + x86_deflabel(X86P_GAMMA)     /* AB-3a FINDING-FIX (pre-existing, measured this session): x86_gamma() above REFERENCES <FN>_act_γ but nothing DEFINED it — every DEFINE-bearing program failed to LINK in m4 at HEAD (undefined reference, ld exit 1; pristine denominator identical), invisible to the matrix (no matrix probe carries a DEFINE) and to the AB gates as run.  Dead structural landing like ω below; AB-3b's site wires are the real continuations. */
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
std::string bb_ab_bind() {
    /* AB-3a (this session): the DEFINE residual runtime action — ONE store fn_cell$<FN> <- &<FN>_act_α (ladder: "DEFINE residual runtime action = ONE store").  Role-2 IR_FUNC_ACTIVATE minted by the lowerer INSIDE the live chain (anchor -> bind -> sJ), replacing the bare DEFINE skip when SCRIP_AB is armed; SCRIP_AB=0 takes the legacy skip and is byte-identical.  The α label is fname-derived (bb_ab_emit_nodes lbl override: <FN>_act_α), so BOTH MEDIA reference it by NAME — TEXT resolves at assembly time, BINARY through the emit-label table with forward patching (blocks emit post-main-chain, after this store's code).  The CELL splits by medium exactly as the block emission does: TEXT = rip-lea to the .data label; BINARY = the runtime slot via the shared allocator.  K=0 transparent spine box (zd_k AB line); trailer = the bb_goto relay shape (alpha + pair_loop). */
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    const char * fname = _.op_sval ? _.op_sval : "?";
    std::string albl = std::string(fname) + "_act_\xce\xb1";
    std::string clbl = std::string("fn_cell$") + fname;
    /* RTX-FUNC-0 BIND-NEUTRALIZE (BINARY): the lea's [rip + __] 5-arg form routes to x86_load_ro which in
     * BINARY bakes the ptr argument (0 here) as a movabs immediate — there is NO forward-patch for it, so at
     * DEFINE-time this store wrote 0 over the correct address the bb_ab_emit_nodes posthook C-store had
     * already placed (measured: cell probe good, then jmp 0 at first call).  BINARY bind is now a transparent
     * relay — the C-store at block-emit time IS the bind.  TEXT keeps the runtime store: gas resolves
     * <FN>_act_α at assembly time and the fn_cell$<FN> .data slot via GOT, both correct as-is. */
    if (MEDIUM_BINARY) return x86_alpha() + x86_pair_loop();
    return x86_alpha()
         + x86("lea", "rax", std::string("[rip + __]"), (uint64_t)0, albl.c_str())   /* α address by NAME (TEXT): renders [rip + <FN>_act_α], resolved by the assembler.  The bare 3-arg string form is SILENTLY SWALLOWED by the encoder (measured: both leas vanished from the .s, r11 garbage, wild store, segv) — the 5-arg __ form is the sanctioned named-symbol spelling. */
         + x86("mov", "r11", std::string("[rip@got + __]"), (uint64_t)0, clbl.c_str())   /* TEXT cell = &fn_cell$<FN> via the GOT load, the 145-line precedent verbatim */
         + x86("mov", RDQ("r11", 0), "rax")
         + x86_pair_loop();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
        g_emit.op_ab_nformals = nd->seal;   /* RTX-FUNC bug#2 fix: dispatch emit.cpp:1008 is the ONLY other setter and role-1 blocks never reach it ("rides ab_nodes[], never reaches this dispatch in-chain") -- without this copy the null loop reads nformals=0 and wipes formal N (two-probe conviction: lower seal=1, template 0) */
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
        /* RTX-FUNC-0: BINARY port defines/jumps route through the POINTER fields (x86_portlbl), not the name
         * strings — stale saved_emit pointers meant the α define landed on the previous node's label and
         * INC_act_α was interned-but-never-defined (bind-fix probe: def=0 off=-1).  Intern by the same names
         * so D-records and the bind-fix agree on ONE bb_label_t per port. */
        g_emit.lbl_α_p = emit_label_intern(ab_lbl_α);
        g_emit.lbl_β_p = emit_label_intern(ab_lbl_β);
        g_emit.lbl_γ_p = emit_label_intern(ab_lbl_γ);
        g_emit.lbl_ω_p = emit_label_intern(ab_lbl_ω);
        /* 599601e (PT-2 Defect B) heap-backed op_arg_slot (int*+cap/n, emit.h:586) and migrated the in-chain fill
         * sites but missed this one: the fixed-array sizeof idiom silently became 8/4=2 AND the posthook snapshot
         * restore (g_emit = saved_emit) carries op_arg_slot as NULL -> store at k=0 SEGV'd every SCRIP_AB=1 run at
         * EMIT time (gdb: rdx=0 at :324; dark under the default-OFF gate).  Reserve per node AFTER the restore. */
        int _ab_n = (int)nd->n_operands;
        drive_arg_slots_reserve(_ab_n);
        for (int k = 0; k < _ab_n; k++) {
            const char *nm = nd->operands[k] ? IR_LIT(nd->operands[k]).sval : (const char *)0;
            g_emit.op_arg_slot[k] = (nm && gva_active) ? gva_index_of(nm) : -1;
        }
        g_emit.op_arg_slot_n = _ab_n;
        bb_emit_x86(bb_func_activate());
        /* RTX-FUNC-0 BIND-FIX: in BINARY (m3), x86_load_ro baked 0 as the forward-reference placeholder for
         * [rip + INC_act_α] in bb_ab_bind() — movabs is an immediate, not a patchable rel32, so the label
         * address was never resolved.  The label pool persists for the whole emitter session (one bb_emit_begin
         * per emit_chain call), so INC_act_α is now defined (just emitted above).  Look it up and write the
         * absolute JIT address (bb_emit_buf + offset) directly into the fn_cell.  The runtime bind still runs
         * (its MOV stores this same address again) — idempotent and harmless.  TEXT (m4): bind resolves via
         * gas/ld at link time; no C-side store needed.  ONE AUTHORITY: bb_ab_cell_addr is the allocator. */
        if (MEDIUM_BINARY) {
            extern bb_buf_t bb_emit_buf;
            bb_label_t *al = emit_label_intern(ab_lbl_α);
            void **cell = (void **)bb_ab_cell_addr(fn);
            if (al && bb_label_defined(al) && cell)
                *cell = (void *)(bb_emit_buf + al->offset);
        }
    }
    g_emit     = saved_emit;
    g_emit_cfg = saved_cfg;
    g_gva_active = saved_gva;
}
