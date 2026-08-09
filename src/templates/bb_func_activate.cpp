#include <string>
#include <cstdint>
#include <cstdio>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "ab_abi.h"
#include "rt.h"
extern int64_t kw_fnclevel;
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* IR_FUNC_ACTIVATE — LADDER AB (2026-08-09 AB-1): per-DEFINE ACTIVATION BLOCK.                                                                                                                      */
/* SPITBOL manual Ch.8 pp.102-106: DEFINE'd function saves fname/formals/locals on a pushdown stack                                                                                                   */
/* at entry (here: an RBP frame) and restores them on RETURN/FRETURN/NRETURN.  Ch.16: &FNCLEVEL++                                                                                                     */
/* at call, -- at return.  ABI frozen in contracts/ab_abi.h (ONE AUTHORITY for offsets/argreg/verdict).                                                                                              */
/*                                                                                                                                                                                                    */
/* AB-1 DELIVERS:                                                                                                                                                                                     */
/*   α  = push rbp; mov rbp,rsp; sub rsp,K; save save-set from GVA cells; null GVA cells; FNCLEVEL++;  */
/*        BOMB stub for body-jmp (AB-2 installs fn_cell + body entry wire).                              */
/*   β  = BOMB stub (AB-2 installs 3-way dispatch + restore + LEAVE + wire jmp).                        */
/*   fn_cell$<FN> = .data quad initialised to &rt_ab_undef_fn_stub (AB-3 sites jmp through it).         */
/*   DEFINE residual: ONE store fn_cell$<FN> ← &<FN>_act_α (also AB-3; site sees live α label).         */
/*   Killswitch SCRIP_AB=0 → byte-identical legacy (lowerer gate; this template is simply not reached).  */
/*                                                                                                        */
/* op_sval = fname (compile-time constant string)                                                         */
/* op_ival = nsave = 1 + nformals + nlocals                                                               */
/* op_arg_slot[k] = GVA index of save-set member k {fname, formal0..np-1, local0..nl-1}; -1 = no GVA    */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* fn_cell storage: one static void* per activation block, indexed by g_flat_node_id uid.                */
/* Mode-4 TEXT: fn_cell$<FN> .data label; initialised to rt_ab_undef_fn_stub symbol.                     */
/* Mode-3 BINARY: static void* in g_ab_fn_cells[]; absolute address baked at emit time.                  */
#define AB_FNCELL_MAX 256
static void * g_ab_fn_cells[AB_FNCELL_MAX];   /* mode-3: one slot per IR_FUNC_ACTIVATE node this program */
static int    g_ab_fn_cell_n = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_func_activate() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    long   nsave = (long)_.op_ival;
    const char * fname = _.op_sval ? _.op_sval : "?";
    long   K     = ab_frame_k(nsave);
    /* ── fn_cell: allocate storage and record address ── */
    void ** fn_cell_ptr = (void **)0;
    std::string fn_cell_lbl = std::string("fn_cell$") + fname;   /* TEXT label */
    if (MEDIUM_BINARY) {
        int idx = g_ab_fn_cell_n < AB_FNCELL_MAX ? g_ab_fn_cell_n++ : 0;
        g_ab_fn_cells[idx] = (void *)(uintptr_t)rt_ab_undef_fn_stub;
        fn_cell_ptr = &g_ab_fn_cells[idx];
    }
    /* ── α — entry: frame + save-set save + null GVA cells + FNCLEVEL++ + BOMB body-jmp ── */
    std::string s =
        /* fn_cell .data emission (TEXT only; binary: no-op directive) */
        x86("directive", std::string(".section .data"))
      + x86("directive", std::string(".align 8"))
      + x86("directive", fn_cell_lbl + std::string(":"))
      + x86("directive", std::string(".quad rt_ab_undef_fn_stub"))
      + x86("directive", std::string(".section .text"))
      + x86("directive", std::string(".intel_syntax noprefix"))
        /* α label and frame prologue */
      + x86_alpha()
      + x86("push", "rbp")
      + x86("mov",  "rbp", "rsp")
      + x86("sub",  "rsp", K)
        /* save-set: spill each GVA cell into RBP-relative frame slot (depth-immune) */
      + FOR(0, (int)nsave, [&](int k) -> std::string {
            int gk  = (k < (int)(sizeof _.op_arg_slot / sizeof *_.op_arg_slot)) ? _.op_arg_slot[k] : -1;
            int ot  = ab_save_off(nsave, k);       /* type word rbp offset */
            int ov  = ab_save_off(nsave, k) + 8;   /* value word rbp offset */
            if (gk >= 0) {
                return x86("note", gva_name(gk))
                     + x86("mov", "rax", ABSQ(RT_GVA_VA + (unsigned long)gk * 16))
                     + x86("mov", RDQ("rbp", ot), "rax")
                     + x86("mov", "rax", ABSQ(RT_GVA_VA + (unsigned long)gk * 16 + 8))
                     + x86("mov", RDQ("rbp", ov), "rax");
            }
            return x86("mov", RDQ("rbp", ot), (long)0)
                 + x86("mov", RDQ("rbp", ov), (long)0);
        })
        /* null GVA cells (fname + all save-set members); actuals staged by call site (AB-3) install after */
      + (FOR(0, (int)nsave, [&](int k) -> std::string {
            int gk = (k < (int)(sizeof _.op_arg_slot / sizeof *_.op_arg_slot)) ? _.op_arg_slot[k] : -1;
            if (gk >= 0) {
                return x86("note", gva_name(gk))
                     + x86("xor",  "eax", "eax")
                     + x86("mov", ABSQ(RT_GVA_VA + (unsigned long)gk * 16),     "rax")
                     + x86("mov", ABSQ(RT_GVA_VA + (unsigned long)gk * 16 + 8), "rax");
            }
            return std::string();
        }))
        /* &FNCLEVEL++ — movabs for address that may exceed 2GB (SIB no-base ABSQ only safe < 0x7FFFFFFF)
         * KW-COORD: after KW-1 this becomes KWQ(FNCLEVEL_IDX); until then use movabs indirect. */
      + x86("movabs", "rcx", (unsigned long)(uintptr_t)&kw_fnclevel)
      + x86("mov",    "rax", RDQ("rcx", 0))
      + x86("add",    "rax", (long)1)
      + x86("mov",    RDQ("rcx", 0), "rax")
        /* AB-1 BODY-JMP STUB: premature entry surfaces here; AB-2 installs fn_cell + static jmp body-entry */
      + x86_bomb((std::string("IR_FUNC_ACTIVATE α: body-jmp not wired (AB-2 installs fn_cell + body entry wire; AB-3 flips call sites) — fname=") + fname).c_str())
      + x86_gamma()   /* γ dead after bomb; present for correct box structure */
        /* ── β — return landing stub (AB-2 replaces with 3-way dispatch + restore + LEAVE + wire jmp) ── */
      + x86_beta()
      + x86_bomb((std::string("IR_FUNC_ACTIVATE β: return landing not wired (AB-2 installs restore + LEAVE + 3-way dispatch) — fname=") + fname).c_str())
      + x86_omega()   /* ω dead after bomb */
        /* RO fname string (AB-2 monitor tap: lea rdi,[rip+<internal_0_s>]; call mon_emit_call_bin) */
      + x86_ro_seal_str(0, fname);
    /* fn_cell DEFINE residual store (mode-3 only; mode-4 stores via the α body at AB-3 — the TEXT label
     * fn_cell$<FN> is visible then).  AB-1: store rt_ab_undef_fn_stub address, confirming the cell is live.
     * This is a no-op in mode-3 when SCRIP_AB=0 (lowerer never emits IR_FUNC_ACTIVATE in that path). */
    if (MEDIUM_BINARY && fn_cell_ptr) {
        *fn_cell_ptr = (void *)(uintptr_t)rt_ab_undef_fn_stub;   /* AB-1: already set above; AB-3 sets &act_α */
    }
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* bb_ab_emit_nodes — called by the driver after the main chain to emit all IR_FUNC_ACTIVATE blocks
 * registered in g->ab_nodes[].  Jump-target-only dead code at AB-1; AB-3 wires sites.             */
extern "C" void bb_ab_emit_nodes(IR_graph_t *g, int gva_active) {
    if (!g || g->ab_n <= 0) return;
    extern int g_gva_active;
    extern int gva_index_of(const char *);
    int saved_gva = g_gva_active;
    g_gva_active = gva_active;
    /* Save/restore g_emit fields we touch so we don't corrupt the caller's state */
    const char * saved_sval    = g_emit.op_sval;
    long         saved_ival    = g_emit.op_ival;
    int *        saved_slot    = g_emit.op_arg_slot;
    int          saved_cap     = g_emit.op_arg_slot_cap;
    int          saved_beta    = g_emit.op_beta_dead;
    int          saved_zdepth  = g_emit.op_zdepth;
    int          saved_zres    = g_emit.op_zres;
    /* Use a local slot array so the template's _.op_arg_slot[k] is always valid */
    int local_slots[64];
    g_emit.op_arg_slot     = local_slots;
    g_emit.op_arg_slot_cap = (int)(sizeof local_slots / sizeof *local_slots);
    g_emit.op_beta_dead    = 1;   /* β bomb stub has no inbound edges at AB-1 */
    g_emit.op_zdepth       = 0;
    g_emit.op_zres         = 0;
    /* AB label name storage: one set per block, kept live until bb_emit_x86 consumes them */
    char ab_lbl_α[128], ab_lbl_β[128], ab_lbl_γ[128], ab_lbl_ω[128];
    const char * saved_lbl_α = g_emit.lbl_α;
    const char * saved_lbl_β = g_emit.lbl_β;
    const char * saved_lbl_γ = g_emit.lbl_γ;
    const char * saved_lbl_ω = g_emit.lbl_ω;
    for (int i = 0; i < g->ab_n; i++) {
        IR_t *nd = g->ab_nodes[i];
        if (!nd) continue;
        g_emit.op_sval = IR_LIT(nd).sval;
        g_emit.op_ival = (long)nd->n_operands;
        /* Set unique per-block label names (design name: <FN>_act_α/β/γ/ω) */
        const char *fn = g_emit.op_sval ? g_emit.op_sval : "unknown";
        snprintf(ab_lbl_α, sizeof ab_lbl_α, "%s_act_\xce\xb1", fn);
        snprintf(ab_lbl_β, sizeof ab_lbl_β, "%s_act_\xce\xb2", fn);
        snprintf(ab_lbl_γ, sizeof ab_lbl_γ, "%s_act_\xce\xb3", fn);
        snprintf(ab_lbl_ω, sizeof ab_lbl_ω, "%s_act_\xcf\x89", fn);
        g_emit.lbl_α = ab_lbl_α;
        g_emit.lbl_β = ab_lbl_β;
        g_emit.lbl_γ = ab_lbl_γ;
        g_emit.lbl_ω = ab_lbl_ω;
        int _ab_n = (int)nd->n_operands < g_emit.op_arg_slot_cap
                  ? (int)nd->n_operands : g_emit.op_arg_slot_cap;
        for (int _k = 0; _k < _ab_n; _k++) {
            const char *_nm = nd->operands[_k] ? IR_LIT(nd->operands[_k]).sval : (const char *)0;
            g_emit.op_arg_slot[_k] = (_nm && gva_active) ? gva_index_of(_nm) : -1;
        }
        bb_emit_x86(bb_func_activate());
    }
    g_emit.op_sval         = saved_sval;
    g_emit.op_ival         = saved_ival;
    g_emit.op_arg_slot     = saved_slot;
    g_emit.op_arg_slot_cap = saved_cap;
    g_emit.op_beta_dead    = saved_beta;
    g_emit.op_zdepth       = saved_zdepth;
    g_emit.op_zres         = saved_zres;
    g_emit.lbl_α           = saved_lbl_α;
    g_emit.lbl_β           = saved_lbl_β;
    g_emit.lbl_γ           = saved_lbl_γ;
    g_emit.lbl_ω           = saved_lbl_ω;
    g_gva_active = saved_gva;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
