#include "runtime/core/trace.h"
#include <unordered_set>
#include "runtime/core/g_runtime.h"
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include "emitter/emit_medium.h"
#include "emitter/bb_label.h"
#include "ir/ir_graph.h"
#include "emitter/emit_state.h"
#include "emitter/emit_data.h"
#include "emitter/emit_knobs.h"
#include "emitter/emit_regime.h"
#include "emitter/emit_walk.h"
#include "emitter/emit_str.h"
#include "emitter/emit_gc_site.h"
#include "lower/lower_prolog.h"
#include "ir/gc_frame_map.h"
#include "templates/x86/x86_ops.h"
#include "templates/x86/x86_call.h"
#include "templates/x86/x86_frame.h"
#include "templates/x86/x86_gc.h"
#include "templates/x86/x86_jump.h"
extern "C" {
#include "optimizer/gva_collect.h"
}
extern "C++" int zf_display_level(void);
extern "C++" int zf_pas_nest_graph(void);
extern "C++" int zf_display_mem_off(int dl);
extern "C++" int xa_flat_class_zf(void);
extern "C++" int xa_flat_class_c(void);
extern "C++" const char * xa_icn_trace_intern(const char * s);
extern "C++" int xa_flat_graph_holds_a_frame_retry(void);
#include "ir/pin_va.h"
extern "C" {
}
#include "templates/bb/bb_templates.h"
#include "runtime/rt/pl_trail_layout.h"
#include "templates/bb/bb_pl_cell.h"
extern "C" void rt_jmp_frame_lexprep(void *, long);
extern "C" void rt_jmp_frame_lexprep2(void *, long, long);
extern "C" void rt_main_args_fetch(void);
extern int * const rt_k_level_p;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define icn_wire_stack_on() emit_knob_unless_zero("SCRIP_ICN_WIRE_STACK")
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_flat_data_section_str(void) {
    return IF_M4(IF(g_flat_data_any,
               std::string("  .section .data\n")
             + (g_flat_data_len ? std::string((const char *)g_flat_data_buf.p, g_flat_data_len) : std::string())
             + "  .section .text\n"));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void xa_entry_dispatch(void) {
    bb_emit_x86(x86_bomb("xa_entry_dispatch: no lowerer constructs an XA_ENTRY_DISPATCH node (R-2009 under R-2002); a program that gets here names itself"));
}
static void xa_flat_emit_text(const std::string & s) { if (!s.empty()) emit_text_n(s.data(), s.size()); }
extern "C" void xa_flat_data_section(void) { xa_flat_emit_text(xa_flat_data_section_str()); }
extern "C" void rt_pl_dc_prep(void *, long, long, long, long, long);
extern "C" DESCR_t rt_pl_dc_leave_γ(DESCR_t, long, void *);
extern "C" DESCR_t rt_pl_dc_leave_ω(long, void *);
extern "C" void rt_arg_stage(int idx, DESCR_t v);
extern "C" int zls_g_block_args(const IR_graph_t *);
extern "C" { struct rt_sxt_fr_s; extern struct rt_sxt_fr_s * const rt_sxt_fr_p; }
extern "C" struct gv_s g_call_args;
extern "C" void rt_icn_zframe_args_install(void *, int, int);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern int g_flat_dc_np;
static const char * const zf_dc_argreg[4] = { "rsi", "rdx", "rcx", "r8" };
static std::string zf_dc_rets(long n) {
    return x86_deflabel_id(2)
         + x86_gc_site_raw(X86_SITE_RET_ROAD, 7, (int)n)
         + x86("add", "rsp", n)
         + x86("ret")
         + x86_deflabel_id(3)
         + x86_gc_site_raw(X86_SITE_RET_ROAD, 7, (int)n)
         + x86("add", "rsp", n)
         + x86("mov32", "eax", 104L)
         + x86("xor", "edx", "edx")
         + x86("ret");
}
static std::string zf_dc_block_head(int nb) {
    return x86("comment",
            "the block protocol, Raku regime: the direct-call stub builds the callee's argument block on the spine from "
                "the caller's cells and jumps past the staged entry; no rt_arg_stage, no poll (nothing can allocate between here and the callee's first safe point)")
            + x86("pop", "rax")
            + x86("push", "rax")
            + x86("push", "rax")
            + IF(nb > 0, x86("sub", "rsp", (long)(16 * nb)));
}
static std::string zf_dc_block_pass(int i) {
    return x86("mov", "rax", "[" + std::string(zf_dc_argreg[i]) + " + 0]")
         + x86("mov", "rdi", "[" + std::string(zf_dc_argreg[i]) + " + 8]")
         + x86("mov", RDQ("rsp", 16 * i), "rax")
         + x86("mov", RDQ("rsp", 16 * i + 8), "rdi");
}
static std::string zf_dc_block_snul(int i) {
    return x86("mov", RDQ("rsp", 16 * i), (long)DT_SNUL)
         + x86("mov", RDQ("rsp", 16 * i + 8), 0L);
}
static std::string zf_dc_block(int np, int nb) {
    return (np > nb || nb > 12) ? x86_bomb("xa_flat_dc_stub: a block-protocol graph whose direct-call arity exceeds its parameter count, or has more than 12 parameters")
         : emit_seq({zf_dc_block_head(nb), FOR(0, np, zf_dc_block_pass), FOR(np, nb, zf_dc_block_snul),
                     x86_lea_id("rcx", 2) + x86_lea_id("rdx", 3) + x86_jmp_lblptr(g_emit.flat_dc_body_p, g_emit.flat_dc_body_p ? g_emit.flat_dc_body_p->name : "?") + zf_dc_rets(8)});
}
static int zf_dc_pad(int np) {
    return (np > 0 && np % 2 == 1) ? 1 : 0;
}
static int zf_dc_push_size(int np) {
    return np * 8 + (zf_dc_pad(np) ? 8 : 0);
}
static std::string zf_dc_zf_head(int np) {
    return emit_seq({x86("comment", "ICN-FR-3 zframe dc stub: stage args, jmp proc_f_α≡0 with wire shims")
            + x86("pop", "rax")
            + x86("push", "rax")
            + x86("push", "rax"),
        IF(zf_dc_pad(np), x86("push", "rax")),
        FOR(0, np, [&](int j) { return x86("push", zf_dc_argreg[np - 1 - j]); })});
}
static std::string zf_dc_zf_stage(int i) {
            return x86("mov", "rax", "[rsp + " + std::to_string(i * 8) + "]")
                + x86("mov32", "edi", (long)i)
                + x86("mov", "rsi", "[rax + 0]")
                + x86("mov", "rdx", "[rax + 8]")
                + x86("call", "rt_arg_stage", TEMPLATE_FN_ADDR(rt_arg_stage))
         + x86_rt_gc_poll();
}
static std::string zf_dc_zf_wire(void) {
    return x86_lea_id("rcx", 3) + x86("push", "rcx")
         + x86_lea_id("rcx", 2) + x86("push", "rcx")
         + x86_jmp_lblptr(g_emit.flat_dc_body_p, g_emit.flat_lbl_α ? g_emit.flat_lbl_α : "?")
         + zf_dc_rets(24);
}
static std::string zf_dc_zf_plain(void) {
    return x86_lea_id("rcx", 2)
         + x86_lea_id("rdx", 3)
         + x86_jmp_lblptr(g_emit.flat_dc_body_p, g_emit.flat_lbl_α ? g_emit.flat_lbl_α : "?")
         + zf_dc_rets(8);
}
static std::string zf_dc_zf(int np) {
    return emit_seq({zf_dc_zf_head(np), FOR(0, np, zf_dc_zf_stage), IF(zf_dc_push_size(np) > 0, x86("add", "rsp", (long)zf_dc_push_size(np))),
                     (icn_wire_stack_on() && g_emit_cfg && g_emit_cfg->cell_frame && g_emit.flat_lcl_proc) ? zf_dc_zf_wire() : zf_dc_zf_plain()});
}
static std::string zf_dc_pl(int kt, int np) {
    return x86("comment", "PL-DC direct-call entry: retaddr -> kt-32 pad, wires -> local ret-shims, one prep crossing, shared body")
         + x86("pop", "rax")
         + x86("sub", "rsp", (long)(kt + 16))
         + x86_rsp_store64(kt - 8, "rsp")
         + std::string("")
         + x86("mov", FRQ(kt - 32), "rax")
         + x86_lea_id("rax", 2)
         + x86("mov", FRQ(kt - 24), "rax")
         + x86_lea_id("rax", 3)
         + x86("mov", FRQ(kt - 16), "rax")
         + FOR(0, np, [&](int i) { return x86("mov", FRQ(16 + 8 * i), zf_dc_argreg[i]); })
         + x86("mov", "rdi", "rsp")
         + x86("mov32", "esi", (long)(g_emit.flat_seed_off >= 16 ? g_emit.flat_seed_off : 16))
         + x86("mov32", "edx", (long)(kt - 32))
         + x86("mov32", "ecx", (long)np)
         + x86("mov32", "r8d", (long)np)
         + x86("mov32", "r9d", 0L)
         + x86("call", "rt_pl_dc_prep", TEMPLATE_FN_ADDR(rt_pl_dc_prep))
         + x86_jmp_lblptr(g_emit.flat_dc_body_p, g_emit.flat_lbl_α_body ? g_emit.flat_lbl_α_body : "?")
         + x86_deflabel_id(2)
         + x86_rsp_load64("rdx", 0)
         + x86("mov", "rcx", "rsp")
         + x86("add", "rcx", (long)(-kt))
         + x86_rsp_load64("r11", -32)
         + x86_rsp_load64("rsp", -8)
         + x86("add", "rsp", 16L)
         + x86("push", "r11")
         + x86_jmpfn("rt_pl_dc_leave_γ", TEMPLATE_FN_ADDR(rt_pl_dc_leave_γ))
         + x86_deflabel_id(3)
         + x86_rsp_load64("rdi", 0)
         + x86("mov", "rsi", "rsp")
         + x86("add", "rsi", (long)(-kt))
         + x86_rsp_load64("r11", -32)
         + x86_rsp_load64("rsp", -8)
         + x86("add", "rsp", 16L)
         + x86("push", "r11")
         + x86_jmpfn("rt_pl_dc_leave_ω", TEMPLATE_FN_ADDR(rt_pl_dc_leave_ω));
}
static std::string zf_dc(int kt, int np) {
    return (g_emit_cfg && g_emit_cfg->pf.block_args) ? zf_dc_block(np, g_emit_cfg->nparams)
         : (g_emit.zframe_graph || (g_emit_cfg && g_emit_cfg->cell_frame && g_emit.flat_lcl_proc)) ? zf_dc_zf(np)
         : zf_dc_pl(kt, np);
}
static std::string xa_flat_dc_stub_str(void) {
    x86_begin();
    return zf_dc(g_emit.flat_frame_bytes, g_flat_dc_np);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string zf_bse_cell(int i) {
        return x86("cmp", "esi", (long)(i + 1))
           + x86_jcc_id("jb", 2 * i)
           + x86("mov", "rax", "[rdi + " + std::to_string(16 * i) + "]")
           + x86("mov", RDQ("rsp", 16 * i), "rax")
           + x86("mov", "rax", "[rdi + " + std::to_string(16 * i + 8) + "]")
           + x86("mov", RDQ("rsp", 16 * i + 8), "rax")
           + x86_jmp_id(2 * i + 1)
           + x86_deflabel_id(2 * i)
           + x86("mov", RDQ("rsp", 16 * i), (long)DT_SNUL)
           + x86("mov", RDQ("rsp", 16 * i + 8), 0L)
           + x86_deflabel_id(2 * i + 1);
}
static std::string zf_bse(int nb) {
    return nb <= 0 ? std::string() : nb > 12 ? x86_bomb("xa_flat_block_staged_entry: more than 12 parameters")
         : emit_seq({x86("comment",
        "the staged entry of a block-protocol graph (every by-name road, glue and C entry stages g_call_args and jumps "
            "here): build the argument block on the spine from the staged cells, a cell beyond the medium's capacity a "
                "null DESCR, then fall into the block entry; a direct call (the dc stub) jumps past this")
        + x86("mov", "rsi", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_call_args, "g_call_args")
        + x86("mov", "rdi", "[rsi + 0]")
        + x86("mov", "esi", RDD("rsi", 12))
        + x86("sub", "rsp", (long)(16 * nb)),
                     FOR(0, nb, zf_bse_cell)});
}
static std::string xa_flat_block_staged_entry_str(void) {
    x86_begin();
    return zf_bse(g_emit_cfg ? g_emit_cfg->nparams : 0);
}
extern "C" void xa_flat_block_staged_entry(void) { bb_emit_x86(xa_flat_block_staged_entry_str()); }
extern "C" void xa_flat_dc_stub(void) { bb_emit_x86(xa_flat_dc_stub_str()); }
extern "C" void rt_lcl_proc_args_install(void *, int, int);
extern "C" void rt_icn_zframe_args_install(void *, int, int);
extern "C" void rt_arg_stage(int idx, DESCR_t v);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define xa_flat_zanchor_poison() emit_knob_if_one("SCRIP_PL_ZANCHOR_POISON")
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" { int stage2_owner_varslot(const char *, const char *); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * zf_display_reg(int dl) { return dl == 1 ? "r13" : dl == 2 ? "r14" : "r15"; }
static std::string zf_display_restore(int kt) {
    return IF(zf_display_level() >= 1 && zf_display_level() <= 3,
               x86("comment", "PAS-DISPLAY-1: restore caller display[L] from [kt-40]")
             + x86("mov", zf_display_reg(zf_display_level()), x86_hb_on() ? RDQ(x86_hb(), kt - 40) : FRQ(kt - 40)))
         + IF(zf_display_level() > 3 && zf_display_mem_off(zf_display_level()) < 0, x86_bomb("PAS-DISPLAY-N: level>3 ancestor slot unresolved (restore)"))
         + IF(zf_display_level() > 3 && zf_display_mem_off(zf_display_level()) >= 0,
               x86("comment", "PAS-DISPLAY-N: restore [r15+off] from [kt-40] (saved caller value)")
             + x86("mov", "rax", x86_hb_on() ? RDQ(x86_hb(), kt - 40) : FRQ(kt - 40))
             + x86("mov", RDQ("r15", zf_display_mem_off(zf_display_level())), "rax"));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int zls_g_det_block(const IR_graph_t * g);
#define zf_det_block_size() ((g_emit_cfg && g_emit_cfg->pf.det_block) ? 16L * g_emit_cfg->nparams : 0L)
static std::string zf_pin_restore(int kt) { return IF(x86_hb_on(), x86("mov", x86_hb(), RDQ(x86_hb(), kt - 8))); }
#define zf_release_np() ((g_emit_cfg && g_emit_cfg->pf.block_args) ? g_emit_cfg->nparams : 0)
static std::string zf_release(int kt) {
    return x86_hb_on() ? x86("lea", "rsp", RDQ(x86_hb(), kt + 16 * zf_release_np()))
                       : x86("add", "rsp", (long)(kt + 16 * zf_release_np()));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void rt_pl_quad_seed(void *);
static std::string pl_standing_cells_zero(int kt, int n) {
    return n > 16 ? x86("lea", "rdi", RDQ("rsp", kt - 64 - 24 - 8 * (n - 1)))
                  + x86("xor", "eax", "eax")
                  + x86("mov32", "ecx", (long)n)
                  + x86("rep_stosq")
                  : FOR(0, n, [&](int k) { return x86("mov", RDQ("rsp", kt - 64 - 24 - 8 * k), 0L); });
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define xa_flat_pkt_slot_addr(idx64, out, tmp) ( \
      x86("mov", (out), (idx64)) + x86_shift_imm("shl", 4, (out), 5) \
    + x86("mov", (tmp), (idx64)) + x86_shift_imm("shl", 4, (tmp), 3) \
    + x86("add", (out), (tmp)) \
    + x86("add", (out), "r9") \
)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int rt_pl_db_frame_cells(void);
extern "C" int rt_pl_db_slot_ensure(void * root, void * db, int i);
static std::string xa_flat_pkt_cell_rax(int k) {
    return k < rt_pl_db_frame_cells() ? x86("mov", "rax", RDQ("r14", -24 - 8 * k))
         : x86("note",
        "a root cell past the frame's PL_DB_FRAME_CELLS lives in the registry's overflow vector: cell 0 holds the "
            "registry, [registry + 24] the vector, re-read here because the collector may move both")
         + x86("mov", "rax", RDQ("r14", -24))
         + x86("mov", "rax", RDQ("rax", 24))
         + x86("mov", "rax", RDQ("rax", 8 * (k - rt_pl_db_frame_cells())));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string xa_flat_pkt_tail_pop(const char * fb, int kt, int lsk) {
    return IF(g_emit.flat_pkt_cell >= 0 && g_emit.flat_pkt_slot >= 0,
               x86("note", "packet fragment last call: when this frame's own clause choice is the youngest (r13 == its header) and no later slot of the chain is visible at the generation the call "
                       "began in, this clause is the last alternative, so the choice is popped to F.B0 as the packet gamma's det exit pops it, and the frame-reuse test that follows passes")
         + x86("lea", "rax", RDQ(fb, kt - 64))
         + x86("cmp", "r13", "rax")
         + x86("jne", L(lsk + 97))
         + x86("mov", "r8", RDQ(fb, kt - 80)) + x86_shift_imm("shr", 5, "r8", 8)
         + xa_flat_pkt_cell_rax(g_emit.flat_pkt_cell) + x86("mov", "r9", RDQ("rax", 0))
         + x86("mov", "r10d", RDD("r9", 40 * g_emit.flat_pkt_slot + 32))
         + x86("movsxd", "r10", "r10d")
         + x86("def", L(lsk + 95))
         + x86("cmp", "r10d", 0L)
         + x86("jl", L(lsk + 96))
         + xa_flat_pkt_slot_addr("r10", "rcx", "rax")
         + x86("mov", "eax", RDD("rcx", 20))
         + x86("cmp", "eax", "r8d")
         + x86("jge", L(lsk + 94))
         + x86("mov", "eax", RDD("rcx", 16))
         + x86("test", "eax", "eax")
         + x86("je", L(lsk + 99))
         + x86("cmp", "eax", "r8d")
         + x86("jge", L(lsk + 99))
         + x86("def", L(lsk + 94))
         + x86("mov", "r10d", RDD("rcx", 32))
         + x86("movsxd", "r10", "r10d")
         + x86("jmp", L(lsk + 95))
         + x86("def", L(lsk + 96))
         + x86("mov", "r13", RDQ(fb, kt - 40))
         + x86("def", L(lsk + 97)));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define PKT_FRESH_OK() (g_emit.flat_pkt_cell >= 0 && g_emit.flat_pkt_slot >= 0 && g_emit.flat_pkt_walk_p)
static std::string xa_flat_pkt_fresh_entry_str(void) {
    return IF(!PKT_FRESH_OK(), x86_bomb("xa_flat_pkt_fresh_entry: a packet fragment without its cell, its slot or its walk label"))
         + IF(PKT_FRESH_OK(), x86("comment",
        "PACKET FRAGMENT fresh entry (ARCH-PROLOG-C-OUT-OF-THE-BOX 5.2 A): the registry road -- the packet from root cell k, G = next_ref, this slot visible at G, else the walk from the head")
         + xa_flat_pkt_cell_rax(g_emit.flat_pkt_cell)
         + x86("mov", "r8d", RDD("rax", 20))
         + x86("mov", "r9", RDQ("rax", 0))
         + x86("mov", "r11d", RDD("r9", 40 * g_emit.flat_pkt_slot + 20))
         + x86("cmp", "r11d", "r8d")
         + x86("jge", L(210))
         + x86("mov", "r11d", RDD("r9", 40 * g_emit.flat_pkt_slot + 16))
         + x86("test", "r11d", "r11d")
         + x86("je", L(211))
         + x86("cmp", "r11d", "r8d")
         + x86("jl", L(210))
         + x86("def", L(211))
         + x86("jmp", L(212))
         + x86("def", L(210))
         + x86("mov", "esi", RDD("rax", 24))
         + x86("movsxd", "rsi", "esi")
         + x86("jmp", "extlbl", (uint64_t)(uintptr_t)g_emit.flat_pkt_walk_p)
         + x86("def", L(212)));
}
extern "C" void xa_flat_pkt_fresh_entry(void) { bb_emit_x86(xa_flat_pkt_fresh_entry_str()); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string zf_pro_head(int kt) {
    return x86("comment", "ICN-FR-2 zframe prologue: sub rsp,kt + wire header [kt-24]=γ [kt-16]=ω [kt-8]=caller____ + pin ___=rsp")
         + x86("sub", "rsp", (long)kt)
         + x86("mov", "[rsp + " + std::to_string(kt - 24) + "]", "rcx")
         + x86("mov", "[rsp + " + std::to_string(kt - 16) + "]", "rdx")
         + (x86_resumable_frame()
            ? ( (x86_hb_on() ? x86("mov", "[rsp + " + std::to_string(kt - 8) + "]", "rbp")
                             + x86("mov", x86_hb(), "rsp")
                             : (emit_jmp_pin_legacy() ? x86("mov", "[rsp + " + std::to_string(kt - 8) + "]", "rsp") : std::string()))
             + IF(g_emit_cfg && g_emit_cfg->root_graph, x86("lea", "rdi", RDQ("rsp", kt - 64))
                                                      + x86("call_bare", "rt_pl_quad_seed", TEMPLATE_FN_ADDR(rt_pl_quad_seed)))
             + IF(g_emit_cfg && g_emit_cfg->root_graph && g_emit_cfg->standing_cells > 0,
                   x86("comment", "RUNG 10b ζ-STANDING CELLS (ARCH sec B.15, sec C ruling): one named root cell per dynamic predicate, carved INSIDE this root frame just under "
                                  "the header block (emit.cpp added 8*n to the region) and reached from any depth as [r14 - 24 - 8k] -- never a global, never one table of all of "
                                  "them in one slot. ⛔ ZEROED HERE BECAUSE A CARVE IS NOT AN INITIALISATION: the bytes are whatever the C stack left, and the runtime reads a "
                                  "NULL cell as \"this predicate has no store yet\" -- an unzeroed cell is a garbage pointer the first assertz would follow.")
                 + pl_standing_cells_zero(kt, g_emit_cfg->standing_cells))
         + x86("lea", "rax", RDQ("rsp", kt)) + x86_raw_pack("rax")
             + x86("mov", RDQ("rsp", kt - 32), "rax")
             + x86("mov", RDQ("rsp", kt - 40), "r13")
             + x86("mov", RDQ("rsp", kt - 48), 0L)
             + x86("mov", RDQ("rsp", kt - 56), 0L)
             + (g_emit_cfg && g_emit_cfg->n_alts > 1 && g_emit.flat_alt1_p ? x86_pl_choice_mark(RDQ("rsp", kt - 64), 245) : x86("mov", RDQ("rsp", kt - 64), "r12"))
             + IF(g_emit_cfg && g_emit_cfg->n_alts > 1 && g_emit.flat_alt1_p,
                    x86("lea", "rax", "extlbl", (uint64_t)(uintptr_t)g_emit.flat_alt1_p)
                 + x86("mov", RDQ("rsp", kt - 56), "rax")
                 + x86("note", "pl_choice_open inline (ARCH-PROLOG-C-OUT-OF-THE-BOX 2.2): B := this frame's header H = rsp+kt-64")
                 + x86("lea", "r13", RDQ("rsp", kt - 64))))
            : x86_hb_on() ? x86("mov", "[rsp + " + std::to_string(kt - 8) + "]", "rbp")
                          + x86("mov", x86_hb(), "rsp")
            : (emit_jmp_pin_legacy() ? (xa_flat_zanchor_poison() ? std::string() : x86("mov", "[rsp + " + std::to_string(kt - 8) + "]", "rsp"))
                                + std::string("")
                               : std::string()));
}
static std::string zf_pro_zero(int zb) {
    return zb <= 128 ? FOR(0, zb / 8, [&](int q) { return x86("mov", RDQ("rsp", 8 * q), 0L); })
                     : x86("mov", "rdi", "rsp")
                     + x86("xor", "eax", "eax")
                     + x86("mov32", "ecx", (long)(zb / 8))
                     + x86("rep_stosq");
}
static std::string zf_pro_resumable(int kt) {
    return x86("comment",
            "the block protocol (ARCH-PROLOG-C-OUT-OF-THE-BOX 1.2): the arguments are the caller's block at [rbp+kt+16i], "
                "never copied; the value region [0, kt-64) is zeroed inline so every mapped slot is a null DESCR at the first poll")
           + zf_pro_zero(kt - 64)
           + IF(g_emit_cfg && g_emit_cfg->root_graph && g_emit_cfg->standing_cells > 0,
                x86("comment",
                    "the creation serial: standing cell 255 is the next unbound cell's tag word, seeded to serial 1 after the value region is zeroed; every unbound-cell creation reads it and adds 51"
                    "2 (PL_SERIAL_CELL)")
              + x86("mov", RDQ("rsp", kt - 64 - PL_SERIAL_OFF), (long)((long)DT_PLVAR | PL_SERIAL_STEP)));
}
static std::string zf_pro_block_args(int kt) {
    return x86("comment",
            "the block protocol, Raku regime (ARCH-PROLOG-C-OUT-OF-THE-BOX 1.2 as landed for Prolog at e95282e39): the arguments are the caller's block at [rsp+kt+16i], "
                "never copied; the value region [0, kt-32) is zeroed inline; the string-extension table is invalidated inline, as rt_icn_zframe_args_install did (c8701b17e)")
           + zf_pro_zero(kt - 32)
           + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&rt_sxt_fr_p, "rt_sxt_fr_p")
           + x86("mov", "rax", RDQ("rax", 0))
           + x86("mov", RDD("rax", 20), 1L)
           + x86("mov", RDQ("rax", 0), 0L);
}
static std::string zf_pro_stosb(int kt) {
    return IF(kt > 48, x86("mov", "rdi", "rsp")
                     + x86("xor", "eax", "eax")
                     + x86("mov32", "ecx", (long)(kt - 32))
                     + x86("rep_stosb"));
}
static std::string zf_pro_args(int np, int nl, const char * call) {
    return x86("mov", "rdi", "rsp")
         + x86("mov32", "esi", (long)np)
         + x86("mov32", "edx", (long)nl)
         + x86(call, "rt_icn_zframe_args_install", TEMPLATE_FN_ADDR(rt_icn_zframe_args_install));
}
static std::string zf_pro_lexprep(int kt) {
    return x86("mov", "rdi", "rsp")
         + x86("mov32", "esi", (long)(g_emit.flat_seed_off ? g_emit.flat_seed_off : 16))
         + x86("mov32", "edx", (long)(kt - 32))
         + x86("call_bare", "rt_jmp_frame_lexprep2", TEMPLATE_FN_ADDR(rt_jmp_frame_lexprep2));
}
extern int g_flat_dc_np;
static std::string zf_pro_lex(int kt, int np, int nl) {
    return g_flat_dc_np >= 0 ? emit_seq({zf_pro_stosb(kt), zf_pro_args(np, nl, "call")})
                             : emit_seq({zf_pro_lexprep(kt), IF(np > 0, zf_pro_args(np, 0, "call_bare"))});
}
static std::string zf_pro_plain(int kt, int np, int nl) {
    return emit_seq({zf_pro_stosb(kt), IF(np > 0 || nl > 0, zf_pro_args(np, nl, "call"))});
}
static std::string zf_pro_body(int kt, int np, int nl) {
    return x86_resumable_frame() ? zf_pro_resumable(kt)
         : (g_emit_cfg && g_emit_cfg->pf.block_args) ? zf_pro_block_args(kt)
         : g_emit.flat_lex ? zf_pro_lex(kt, np, nl)
         : zf_pro_plain(kt, np, nl);
}
static std::string zf_pro_display_reg(int kt, const char * dr) {
    return x86("comment", "PAS-DISPLAY-1: save caller display[L] into [kt-40]; display[L] = this frame")
         + x86("mov", FRQ(kt - 40), dr)
         + x86("mov", dr, "rsp");
}
static std::string zf_pro_display_mem(int kt, int off) {
    return off < 0 ? x86_bomb("PAS-DISPLAY-N: level>3 ancestor slot unresolved (save)")
         : x86("comment", "PAS-DISPLAY-N: save [r15+off] into [kt-40]; [r15+off] = this frame")
         + x86("mov", "rax", RDQ("r15", off))
         + x86("mov", FRQ(kt - 40), "rax")
         + x86("mov", "rax", "rsp")
         + x86("mov", RDQ("r15", off), "rax");
}
static std::string zf_pro_display(int kt, int dl) {
    return (dl && dl <= 3) ? zf_pro_display_reg(kt, dl == 1 ? "r13" : dl == 2 ? "r14" : "r15")
         : dl > 3 ? zf_pro_display_mem(kt, zf_display_mem_off(dl))
         : std::string();
}
static std::string zf_pro_map(int kt) {
    return (g_emit_cfg && g_emit_cfg->root_graph) ? emit_gc_map_register(kt - FLAT_FRAME_ALLOWANCE_ROOT, kt, FLAT_FRAME_ALLOWANCE_ROOT - 16, GC_FRAME_MAP_ROOT, 0, 0u)
         : emit_gc_map_register(g_emit_cfg ? g_emit_cfg->jcon_value_region : 0, kt, kt - (g_emit_cfg ? g_emit_cfg->jcon_value_region : 0) - 16,
        (!g_runtime.eval.rt_fragment_emit && g_emit.flat_fam && (!strcmp(g_emit.flat_fam, "main") || !strcmp(g_emit.flat_fam, "pat_flat"))) ? GC_FRAME_MAP_ROOT : 0u, 0,
        (zf_pas_nest_graph() ? GC_LINK_Q(kt + (int)zf_det_block_size(), kt - 8, kt + (int)zf_det_block_size() + 16, x86_hb_on() ? 0u : GC_LINK_RBP_KEPT) :
        GC_LINK_Q(kt - 24, kt - 8, kt + 16 * ((g_emit_cfg && g_emit_cfg->pf.block_args) ? g_emit_cfg->nparams : 0), x86_hb_on() ? 0u : GC_LINK_RBP_KEPT)));
}
static std::string zf_pro_packet(int kt) {
    return x86("comment", "PACKET FRAGMENT: G (the generation this call took) as a packed RAW word in the pinned allowance's spare cell [kt-80], read by the chain-omega and the gamma scan")
           + x86("mov", "rax", "r8") + x86_shift_imm("shl", 4, "rax", 8) + x86_or_imm("rax", (long)DT_RAW)
           + x86("mov", FRQ(kt - 80), "rax")
           + x86("mov", FRQ(kt - 72), 0L)
           + x86("note",
               "PACKET FRAGMENT: this frame is the youngest choice from its entry (B = its header, the floor word at its "
                   "base), as a multi-clause chain's alternation is, so every binding of a caller cell is trailed and the chain-omega's unwind undoes it before the next clause")
           + x86_pl_frame_choice_open(x86_hb(), kt, 236, 237, g_emit_cfg->n_alts > 1 && g_emit.flat_alt1_p ? -1 : 245);
}
static std::string zf_pro_depth(int kt);
static std::string zf_epo_pl(int kt, int l_ok);
static std::string zf_pro_finish(const std::string & s, int kt) {
    return (g_emit.flat_pkt && (!g_emit_cfg || kt - g_emit_cfg->jcon_value_region != FLAT_FRAME_ALLOWANCE_PINNED))
         ? x86_bomb("packet fragment: the pinned allowance is not 96, the G word at [kt-80] has no home")
         : emit_seq({s, IF(g_emit.flat_pkt, zf_pro_packet(kt)), zf_pro_depth(kt), zf_pro_map(kt)});
}
static int zf_pro_kt(int kt) {
    if (kt < 48 || (kt & 15)) { fprintf(stderr, "FATAL xa_flat_zframe_prologue: kt=%d (must be 16-mult >= 48)\n", kt); abort(); }
    return kt;
}
static int zf_pro_np(void) {
    return g_emit_cfg ? g_emit_cfg->nparams : 0;
}
static int zf_pro_nl(void) {
    return g_emit_cfg ? g_emit_cfg->nlocals : 0;
}
static int zf_pro_depth_on(void) {
    return g_emit_cfg && g_emit_cfg->depth_counted && x86_resumable_frame() && !g_emit_cfg->root_graph;
}
static std::string zf_pro_depth_latent(int kt) {
    return x86("comment",
        "DEPTH-LATENT (ARCH-PROLOG-DATABASE section 8, the control-construct frame): swipl's temp clause holds a level and counts nothing; "
                          "LEVEL is the caller's plus 1 minus the caller's latent tag, the tag being bit 0 of the LIMITER word "
                          "(set here, so a latent frame under a latent frame holds no second level); REACHED and LIMIT are not touched")
         + x86("mov", "rax", RDQ("rsp", kt - 8))
         + x86("mov", "rcx", RDQ("rax", 0))
         + x86("add", "rcx", 1L)
         + x86("mov", "rdi", RDQ("rax", 8))
         + x86("mov", "rdx", "rdi")
         + x86("and", "edx", 1L)
         + x86("sub", "rcx", "rdx")
         + x86("mov", RDQ("rsp", 0), "rcx")
         + x86("or", "rdi", 1L)
         + x86("mov", RDQ("rsp", 8), "rdi");
}
static std::string zf_pro_depth(int kt) {
    return IF(zf_pro_depth_on() && g_emit_cfg->dl_latent, zf_pro_depth_latent(kt))
         + IF(zf_pro_depth_on() && !g_emit_cfg->dl_latent,
               x86("comment",
        "DEPTH-COUNTED (ARCH-PROLOG-DATABASE section 8): LEVEL is the caller's plus 1 and LIMITER the caller's with the latent tag (bit 0) cleared, "
                          "copied down through the link at [kt-8] from the caller's base +0 and +8; "
                          "an active limiter's REACHED (pair +8) rises to this level, and a level over its LIMIT (pair +0) is this clause's omega -- the predicate fails, no ball")
         + x86("mov", "rax", RDQ("rsp", kt - 8))
         + x86("mov", "rcx", RDQ("rax", 0))
         + x86("add", "rcx", 1L)
         + x86("mov", RDQ("rsp", 0), "rcx")
         + x86("mov", "rdi", RDQ("rax", 8))
         + x86("and", "rdi", -2L)
         + x86("mov", RDQ("rsp", 8), "rdi")
         + x86("test", "rdi", "rdi")
         + x86("je", L(961))
         + x86("cmp", "rcx", RDQ("rdi", 8))
         + x86("jle", L(960))
         + x86("mov", RDQ("rdi", 8), "rcx")
         + x86("def", L(960))
         + x86("cmp", "rcx", RDQ("rdi", 0))
         + x86("jle", L(961))
         + (g_emit.flat_pkt && g_emit.flat_pkt_chainω_p ? x86("jmp", "extlbl", (uint64_t)(uintptr_t)g_emit.flat_pkt_chainω_p) : zf_epo_pl(kt, 962))
         + x86("def", L(961)));
}
static std::string zf_pro(int kt) {
    return zf_pro_finish(emit_seq({zf_pro_head(zf_pro_kt(kt)), zf_pro_body(kt, zf_pro_np(), zf_pro_nl()), zf_pro_display(kt, zf_display_level())}), kt);
}
static std::string xa_flat_zframe_prologue_str(void) {
    return g_emit.zframe_graph ? zf_pro(g_emit.flat_frame_bytes) : std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define xa_flat_sig_gq(gk, w) (g_runtime.abi.rtcc_on ? std::string(GVARQ((gk), (w))) : std::string(ABSQ(RT_GVA_VA + (unsigned long)(gk) * 16 + (unsigned long)(w))))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define xa_flat_wn_park(fname) (!g_runtime.eval.rt_fragment_emit && (fname) && (fname)[0] && !strchr((fname), '$') && strncmp((fname), "LBL__", 5) != 0)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_flat_wn_park_str(int kt, const char * fname) {
    extern int rt_g_want_name; extern int rt_g_ret_by_name;
    return IF(xa_flat_wn_park(fname), x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&rt_g_ret_by_name, "rt_g_ret_by_name")
         + x86("mov", RDD("rax", 0), (long)0)
         + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&rt_g_want_name, "rt_g_want_name")
         + x86("mov", "edx", RDD("rax", 0))
         + x86("mov", RDQ("rsp", kt - 16), "rdx")
         + x86("mov", RDD("rax", 0), (long)0));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_flat_wn_restore_str(int kt, const char * fname) {
    extern int rt_g_want_name;
    return IF(xa_flat_wn_park(fname), x86("comment", "name request: put the parked request back so the call site's by-name consult reads THIS call's")
         + x86("push", "rax")
         + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&rt_g_want_name, "rt_g_want_name")
         + x86("mov", "rcx", RDQ("rsp", kt - 16 + 8))
         + x86("mov", RDD("rax", 0), "ecx")
         + x86("pop", "rax"));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define zf_kt() (g_emit.flat_frame_bytes)
static std::string xa_flat_chain_diag(void) {
    extern int bb_emit_pos;
    fprintf(stderr, "[CHAINFRAME] pos=%d kt=%d text=%d jmp=%d pat=%d\n", bb_emit_pos, g_emit.flat_frame_bytes, g_is_text ? 1 : 0, g_emit.flat_jmp_entry, g_emit.flat_pat);
    return std::string();
}
static std::string xa_flat_chain_kt_fatal(void) { fprintf(stderr, "FATAL xa_flat_chain_prologue: kt=%d (must be a 16-multiple >= 48)\n", zf_kt()); abort(); return std::string(); }
static int xa_flat_argkt_set(int * argkt_out, int v) { if (argkt_out) *argkt_out = v; return 1; }
static std::string xa_flat_chain_sig_arg(const int * gk, int nf, int i) {
    return x86("note", gva_note(gk[i]))
         + x86("mov", "r10", xa_flat_sig_gq(gk[i], 0))
         + x86("mov", "[rsp + " + std::to_string(16 * i) + "]", "r10")
         + x86("mov", "r10", xa_flat_sig_gq(gk[i], 8))
         + x86("mov", "[rsp + " + std::to_string(16 * i + 8) + "]", "r10")
         + IF(i >= nf, x86("mov", xa_flat_sig_gq(gk[i], 0), (long)DT_SNUL)
                     + x86("mov", xa_flat_sig_gq(gk[i], 8), (long)0))
         + IF(i < nf, x86("cmp", "rdx", (long)i)
                    + x86_jcc_id("jbe", 1 + i)
                    + x86("mov", "r10", "[rcx + " + std::to_string(24 + 8 * i) + "]")
                    + x86("add", "r10", "r8")
                    + x86("mov", "r11", "[r10 + 0]")
                    + x86("mov", xa_flat_sig_gq(gk[i], 0), "r11")
                    + x86("mov", "r11", "[r10 + 8]")
                    + x86("mov", xa_flat_sig_gq(gk[i], 8), "r11")
                    + x86_jmp_id(40 + i)
                    + x86_deflabel_id(1 + i)
                    + x86("mov", xa_flat_sig_gq(gk[i], 0), (long)DT_SNUL)
                    + x86("mov", xa_flat_sig_gq(gk[i], 8), (long)0)
                    + x86_deflabel_id(40 + i));
}
static std::string xa_flat_chain_sig_block(const char * fname, int * argkt_out) {
    return IF(_.op_csig_ok && xa_flat_argkt_set(argkt_out, 16 * _.op_csig_ns),
               x86("sub", "rsp", (long)(16 * _.op_csig_ns))
             + x86("mov", "rdx", "[rcx + 0]")
             + x86("lea", "r8", "[rsp + " + std::to_string(zf_kt() + 16 * _.op_csig_ns) + "]")
             + FOR(0, _.op_csig_ns, [&](int i) { return xa_flat_chain_sig_arg(_.op_csig_gk, _.op_csig_nf, i); }));
}
static std::string xa_flat_chain_prologue_str(const char * fname, int * argkt_out) {
    return IF(xa_flat_class_c(),
               IF(emit_knob_if_one("SCRIP_CHAIN_DIAG"), xa_flat_chain_diag())
             + IF(zf_kt() & 15, xa_flat_chain_kt_fatal())
             + x86("comment", "CLASS-C chain prologue (s114): carve kt + park {γ,ω} at [kt-24]/[kt-16] + save caller ___ at [kt-8]; ___ NOT pinned")
             + x86("sub", "rsp", (long)zf_kt())
             + x86("mov", "[rsp + " + std::to_string(zf_kt() - 24) + "]", "rcx")
             + x86("mov", "[rsp + " + std::to_string(zf_kt() - 16) + "]", "rdx")
             + x86("mov", "[rsp + " + std::to_string(zf_kt() - 8) + "]", "rbp")
             + xa_flat_wn_park_str(zf_kt(), fname)
             + xa_flat_chain_sig_block(fname, argkt_out));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_flat_chain_epilogue_str(void) {
    return IF(xa_flat_class_c(),
               x86("comment", "CLASS-C chain epilogue (s114): release the α carve; no whack exists on this exit")
             + x86("add", "rsp", (long)g_emit.flat_frame_bytes));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_flat_epi_sig_diag(int is_gamma, const char * fname) {
    fprintf(stderr, "[SIGEPI] fname=%s is_gamma=%d kt=%d sig_names_have=%d nf=%d nsave=%d\n", fname ? fname : "(null)", is_gamma, zf_kt(), _.op_csig_ok, _.op_csig_nf, _.op_csig_ns);
    return std::string();
}
static std::string xa_flat_epi_sig_block(int is_gamma, const char * fname) {
    return IF(_.op_csig_ok,
               IF(is_gamma && _.op_csig_rg >= 0, x86("note", gva_note(_.op_csig_rg))
                                               + x86("mov", "rax", xa_flat_sig_gq(_.op_csig_rg, 0))
                                               + x86("mov", "rdx", xa_flat_sig_gq(_.op_csig_rg, 8)))
             + FOR(0, _.op_csig_ns, [&](int i) {
                   return x86("note", gva_note(_.op_csig_gk[i]))
                        + x86("mov", "r10", "[rsp + " + std::to_string(16 * i) + "]")
                        + x86("mov", xa_flat_sig_gq(_.op_csig_gk[i], 0), "r10")
                        + x86("mov", "r10", "[rsp + " + std::to_string(16 * i + 8) + "]")
                        + x86("mov", xa_flat_sig_gq(_.op_csig_gk[i], 8), "r10"); })
             + x86("add", "rsp", (long)(16 * _.op_csig_ns)));
}
static std::string xa_flat_chain_epilogue_sig_str(int is_gamma, const char * fname) {
    return IF(xa_flat_class_c(),
               IF(emit_knob_nonzero("SCRIP_SIGEPI_DIAG"), xa_flat_epi_sig_diag(is_gamma, fname))
             + xa_flat_epi_sig_block(is_gamma, fname)
             + x86("comment", is_gamma
                   ? "CLASS-C chain epilogue-γ, det-arm signature form (s272 snocone-returns-codegen): the α carve parked the caller's det-arm signature pointer at [kt-24] "
                     "(bcps_det_arm's .Lsig blob: nargs, γ-cont at +8, ω-cont at +16, arg offsets) but this exit used to just release the frame and fall through to the bare/wire "
                     "epilogue, which pops garbage -- reload the pointer, follow it to the γ continuation, THEN release, THEN jmp. rax:rdx carry the typed result -- reloaded from "
                     "res_gk just above (nreturn-after-indirect-assign-wrong-value fix) whenever this graph's signature info is known, otherwise still whatever the last node to "
                     "reach this exit staged. Must NOT clobber rax:rdx -- the call site's own landing tells success from failure by reading al, and only DT_FAIL (0x68) reads as "
                     "failure, so a live result's low byte must survive untouched. ⛔ ONLY valid when this chain was actually entered via bcps_det_arm's jmp-with-signature "
                     "convention (guarded by !g_runtime.eval.rt_fragment_emit at the call site) -- EVAL's runtime-compiled fragments reach this SAME class-C prologue but are invoked by a "
                     "normal C call/ret (rt_proc_call_open_det* calling a real function pointer), so for them the plain xa_flat_chain_epilogue (release-only, fall through to ret) "
                     "is the correct and only exit; reading [rsp+kt-24] as a signature pointer for an eval fragment reads whatever garbage sat in rcx at entry and segfaults "
                     "(measured: corpus/crosscheck/rung10/1019_eval_string.sno SIGSEGV before this guard was added)."
                   : "CLASS-C chain epilogue-ω, det-arm signature form (s272 snocone-returns-codegen): same signature reload as epilogue-γ, but the ω continuation lives at "
                     "sig+16, not sig+8 (confirmed against a working DEFINE'd proc's own epilogue-ω: genuinely different offsets, not a symmetric pair) -- and unlike γ this exit "
                     "MUST overwrite rax:rdx with FAILDESCR, because the call site's landing (shared with γ when the two continuations coincide, per bcps_det_arm) tells the two "
                     "apart only by `cmp al, DT_FAIL`. Same eval-fragment caveat as epilogue-γ applies -- see its comment.")
         + xa_flat_wn_restore_str(zf_kt(), fname)
         + x86("mov", "rcx", RDQ("rsp", zf_kt() - 24))
         + x86("mov", "rcx", RDQ("rcx", is_gamma ? 8 : 16))
         + IF(!is_gamma, x86("mov32", "eax", (long)DT_FAIL)
                       + x86("xor", "edx", "edx"))
         + x86("add", "rsp", (long)zf_kt())
         + x86_jmp_reg("rcx"));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string trace_tap_skip(int kind) { return kind <= 4 ? "L24" + std::to_string(6 + kind) : "L" + std::to_string(235 + kind); }
static std::string trace_tap_lbl(int id) { return ".Licn_trace_nm" + std::to_string(id); }
static std::string trace_tap_body(const char * pname, int kind, int np, int r11d, int id) {
    extern void rt_trace_call_hook_f(const char *, int, void *); extern void rt_trace_return_hook(const char *, DESCR_t); extern void rt_trace_fail_hook(const char *);
    extern void rt_trace_gen_fail_hook(const char *, void *);
    return x86("push", "rax")
         + x86("push", "rdx")
         + x86_align_call_enter()
         + IF(kind != 1, x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_runtime, "g_runtime")
                       + x86("mov", "rax", RDQ("rax", GRT_AT(trace)))
                       + x86("cmp", "rax", (long)0)
                       + x86("je", trace_tap_skip(kind)))
         + x86("directive", ".section .rodata")
         + x86("directive", (trace_tap_lbl(id) + ": .string \"" + pname + "\"").c_str())
         + x86("directive", ".section .text")
         + x86("directive", ".intel_syntax noprefix")
         + x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)pname, trace_tap_lbl(id).c_str())
         + IF(kind == 1, x86("mov32", "esi", (long)np)
                       + x86("lea", "rdx", RDQ("r11", r11d)))
         + IF(kind == 2, x86("mov", "rsi", RDQ("r11", 8))
                       + x86("mov", "rdx", RDQ("r11", 0)))
         + IF(kind == 5, x86("mov", "rsi", "rbp"))
         + IF(kind == 1, x86("call", "rt_trace_call_hook_f", (uint64_t)(uintptr_t)(void *)rt_trace_call_hook_f) + x86_rt_gc_poll())
         + IF(kind == 2, x86("call", "rt_trace_return_hook", (uint64_t)(uintptr_t)(void *)rt_trace_return_hook) + x86_rt_gc_poll())
         + IF(kind == 5, x86("call", "rt_trace_gen_fail_hook", (uint64_t)(uintptr_t)(void *)rt_trace_gen_fail_hook) + x86_rt_gc_poll())
         + IF(kind != 1 && kind != 2 && kind != 5, x86("call", "rt_trace_fail_hook", (uint64_t)(uintptr_t)(void *)rt_trace_fail_hook) + x86_rt_gc_poll())
         + x86("def", trace_tap_skip(kind))
         + x86_align_call_leave()
         + x86("pop", "rdx")
         + x86("pop", "rax");
}
std::string xa_icn_trace_tap(const char * pname, int kind, int np, int r11d) {
    extern int g_flat_node_id;
    return IF(x86_trace_taps_on() && pname, trace_tap_body(xa_icn_trace_intern(pname), kind, np, r11d, g_flat_node_id++));
}
const char * xa_icn_trace_pname(void) {
    return (g_emit_cfg && g_emit_cfg->root_graph) ? "main"
         : !g_emit.flat_fam ? (const char *)0
         : (strncmp(g_emit.flat_fam, "proc_", 5) == 0) ? g_emit.flat_fam + 5
         : g_emit.flat_fam;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zf_epi_kt(void) {
    return g_emit.flat_frame_bytes + ((g_emit_cfg && g_emit_cfg->cell_frame && g_emit.flat_lcl_proc) ? (g_emit_cfg->nparams + g_emit_cfg->nlocals) * 16 : 0);
}
extern int xa_icn_block_size(void);
static std::string zf_epg_gen(int kt) {
    return x86("comment", "Z-3 γ-RETAIN cells-arm generator: marshal rax:rdx→rdi:rsi; load γ wire; NO unwind (frame survives for β); gen____→rax; jmp γ wire")
             + x86("mov", "rdi", "rax")
             + x86("mov", "rsi", "rdx")
             + x86("mov", "rcx", "qword ptr [rsp# + " + std::to_string(kt - 24) + "]")
             + x86("mov", "rax", "rsp")
             + x86("jmp", "rcx");
}
static std::string zf_epg_wire(int kt) {
    return xa_icn_trace_tap(xa_icn_trace_pname(), 2, 0, 16)
             + x86("mov", "rdi", "rax")
             + x86("mov", "rsi", "rdx")
         + x86("push", "rax")
             + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&rt_k_level_p, "rt_k_level_p")
             + x86("mov", "rax", RDQ("rax", 0))
             + x86("mov", "ecx", RDD("rax", 0))
             + x86("movsxd", "rcx", "ecx")
             + x86("sub", "rcx", (long)1)
             + x86("mov", RDD("rax", 0), "ecx")
             + x86("sub", "rcx", (long)1)
             + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_runtime, "g_runtime")
             + x86("mov", RDQ("rax", GRT_AT(kw_fnclevel)), "rcx")
             + x86("pop", "rax")
             + (x86_hb_on() ? x86("lea", "rsp", RDQ(x86_hb(), kt + xa_icn_block_size()))
                            + x86("mov", x86_hb(), RDQ(x86_hb(), kt - 8))
                            : x86("add", "rsp", (long)(kt + xa_icn_block_size())))
             + bb_glue_wire_γ();
}
static std::string zf_epg_nest(int kt) {
    return x86("mov", "rdi", "rax")
             + x86("mov", "rsi", "rdx")
             + zf_display_restore(kt)
             + IF(x86_hb_on(), x86("mov", x86_hb(), RDQ("rsp", kt - 8)))
             + x86("add", "rsp", (long)kt + zf_det_block_size())
             + x86("pop", "rcx")
             + x86("add", "rsp", 8L)
             + x86("jmp", "rcx");
}
static std::string zf_epg_pkt(int kt) {
    return x86("comment",
            "PACKET FRAGMENT gamma (ARCH-PROLOG-C-OUT-OF-THE-BOX 5.2 A): B == this frame's header -> no choice inside the "
                "clause: a later slot visible at G makes the token this frame with beta = the chain-omega, none pops the "
                    "choice and exits det; B younger -> the clause's own beta; B older -> a cut ran, det")
             + IF(xa_flat_graph_holds_a_frame_retry(), x86("comment",
                 "PACKET FRAGMENT gamma: this clause holds a disjunction or a builtin generator, whose choice re-aims B at THIS frame's header -- "
                     "B == header cannot say no choice is left inside, so it takes the clause's own beta, whose exhaustion reaches the chain-omega's walk"))
             + x86("mov32", "edi", (long)DT_I)
             + x86("mov32", "esi", 1L)
             + x86("mov", "rcx", RDQ(x86_hb(), kt - 24))
             + x86("lea", "rax", RDQ(x86_hb(), kt - 64))
             + x86("cmp", "r13", "rax")
             + x86(xa_flat_graph_holds_a_frame_retry() ? "jbe" : "jb", L(238))
             + x86("ja", L(233))
             + x86("mov", "r8", FRQ(kt - 80)) + x86_shift_imm("shr", 5, "r8", 8)
             + xa_flat_pkt_cell_rax(g_emit.flat_pkt_cell) + x86("mov", "r9", RDQ("rax", 0))
             + x86("mov", "r10d", RDD("r9", 40 * g_emit.flat_pkt_slot + 32))
             + x86("movsxd", "r10", "r10d")
             + x86("def", L(230))
             + x86("cmp", "r10d", 0L)
             + x86("jl", L(233))
             + xa_flat_pkt_slot_addr("r10", "r11", "rax")
             + x86("mov", "eax", RDD("r11", 20))
             + x86("cmp", "eax", "r8d")
             + x86("jge", L(232))
             + x86("mov", "eax", RDD("r11", 16))
             + x86("test", "eax", "eax")
             + x86("je", L(231))
             + x86("cmp", "eax", "r8d")
             + x86("jl", L(232))
             + x86("def", L(231))
             + x86("lea", "rdx", "extlbl", (uint64_t)(uintptr_t)g_emit.flat_pkt_chainω_p)
             + x86("mov", "rax", x86_hb()) + zf_pin_restore(kt)
             + x86("jmp", "rcx")
             + x86("def", L(232))
             + x86("mov", "r10d", RDD("r11", 32))
             + x86("movsxd", "r10", "r10d")
             + x86("jmp", L(230))
             + x86("def", L(238))
             + x86("lea", "rdx", "extlbl", (uint64_t)(uintptr_t)g_emit.flat_β_p)
             + x86("mov", "rax", x86_hb()) + zf_pin_restore(kt)
             + x86("jmp", "rcx")
             + x86("def", L(233))
             + x86("mov", "r13", RDQ(x86_hb(), kt - 40))
             + x86("xor", "eax", "eax") + zf_release(kt) + zf_pin_restore(kt)
             + x86("jmp", "rcx");
}
static std::string zf_epg_pl(int kt) {
    return x86("comment", "PL γ: a predicate has no value -- hand the caller the definite success DESCR {DT_I, 1} the return trampolines already hand it, never the last box's leftover rax:rdx")
             + x86("comment",
                 "(a bound integer payload 104 = DT_FAIL read as failure at the caller's al test; a {small int, cell address} pair in the caller's result slot is no DESCR to the collector)")
             + x86("mov32", "edi", (long)DT_I)
             + x86("mov32", "esi", 1L)
             + x86("mov", "rcx", RDQ(x86_hb(), kt - 24))
             + IF(!(g_emit_cfg && g_emit_cfg->root_graph),
                   x86("mov", "rax", RDQ(x86_hb(), kt - 40))
                 + x86("cmp", "r13", "rax")
                 + x86("je", "extlbl", (uint64_t)(uintptr_t)g_emit.flat_altdet_p)
                 + x86("lea", "rdx", "extlbl", (uint64_t)(uintptr_t)g_emit.flat_β_p)
                 + x86("mov", "rax", x86_hb())
                 + zf_pin_restore(kt)
                 + x86("jmp", "rcx")
                 + x86_def_ext(g_emit.flat_altdet_p))
             + x86("xor", "eax", "eax")
             + zf_release(kt)
             + zf_pin_restore(kt)
             + x86("jmp", "rcx");
}
static std::string zf_epg_plain(int kt) {
    return x86("mov", "rdi", "rax")
         + x86("mov", "rsi", "rdx")
         + x86("mov", "rcx", RDQ(x86_hb(), kt - 24))
         + zf_display_restore(kt)
         + zf_release(kt)
         + zf_pin_restore(kt)
         + std::string("")
         + x86("jmp", "rcx");
}
static std::string zf_epg(int kt) {
    return (g_emit.flat_gen && g_emit_cfg && g_emit_cfg->cell_frame && g_emit.flat_lcl_proc) ? zf_epg_gen(kt)
         : (icn_wire_stack_on() && g_emit_cfg && g_emit_cfg->cell_frame && g_emit.flat_lcl_proc) ? zf_epg_wire(kt)
         : zf_pas_nest_graph() ? zf_epg_nest(kt)
         : (x86_resumable_frame() && g_emit.flat_pkt && g_emit.flat_β_p && g_emit.flat_altdet_p && !(g_emit_cfg && g_emit_cfg->root_graph)) ? zf_epg_pkt(kt)
         : (x86_resumable_frame() && g_emit.flat_β_p && g_emit.flat_altdet_p) ? zf_epg_pl(kt)
         : zf_epg_plain(kt);
}
static std::string xa_flat_zframe_epilogue_γ_str(void) {
    return xa_flat_class_zf() ? zf_epg(zf_epi_kt()) : std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string zf_epo_wire(int kt) {
    return xa_icn_trace_tap(xa_icn_trace_pname(), 3, 0, 16)
             + x86("push", "rax")
             + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&rt_k_level_p, "rt_k_level_p")
             + x86("mov", "rax", RDQ("rax", 0))
             + x86("mov", "ecx", RDD("rax", 0))
             + x86("movsxd", "rcx", "ecx")
             + x86("sub", "rcx", (long)1)
             + x86("mov", RDD("rax", 0), "ecx")
             + x86("sub", "rcx", (long)1)
             + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_runtime, "g_runtime")
             + x86("mov", RDQ("rax", GRT_AT(kw_fnclevel)), "rcx")
             + x86("pop", "rax")
             + (x86_hb_on() ? x86("lea", "rsp", RDQ(x86_hb(), kt + xa_icn_block_size()))
                            + x86("mov", x86_hb(), RDQ(x86_hb(), kt - 8))
                            : x86("add", "rsp", (long)(kt + xa_icn_block_size())))
             + bb_glue_wire_ω();
}
static std::string zf_epo_nest(int kt) {
    return x86("comment", "PAS-NEST epilogue-ω: consume the caller-pushed wire pair (discard γ-landing, jmp ω-landing) — twin of PAS-NEST epilogue-γ")
             + zf_display_restore(kt)
             + IF(x86_hb_on(), x86("mov", x86_hb(), RDQ("rsp", kt - 8)))
             + x86("add", "rsp", (long)kt + zf_det_block_size())
             + x86("add", "rsp", 8L)
             + x86("pop", "rcx")
             + x86("jmp", "rcx");
}
static std::string zf_epo_pkt_walk(int kt, int k, int i, int np) {
    return x86_def_ext(g_emit.flat_pkt_chainω_p)
             + IF(emit_pl_scc_armed(), x86_pl_scc_release_check(x86_hb(), kt, 250))
             + x86("comment",
                 "PACKET FRAGMENT chain-omega (ARCH-PROLOG-C-OUT-OF-THE-BOX 5.2 A): undo this clause's bindings, restore B and "
                     "the caller's landings, release the frame but not the block, then walk next_idx to the next slot visible at G "
                         "and enter its fragment's chain entry; none left -> the predicate's omega")
             + x86("note",
                 "a cut in this clause left B at an OLDER choice than this frame's header H (B > H, the gamma's own rule): the "
                     "predicate is committed and concedes -- no later clause runs; a younger B (a retained inner choice, a local cut's barrier) walks on; r11 = H - B carries it past the unwind")
             + x86("lea", "r11", FRQ(kt - 64))
             + x86("sub", "r11", "r13")
             + x86_pl_tr_unwind_at(FRQ(kt - 64), 220, 221)
             + x86("mov", "rcx", FRQ(kt - 24))
             + x86("mov", "rdx", FRQ(kt - 16))
             + x86("mov", "r13", FRQ(kt - 40))
             + x86("mov", "r8", FRQ(kt - 80)) + x86_shift_imm("shr", 5, "r8", 8)
             + xa_flat_pkt_cell_rax(k) + x86("mov", "r9", RDQ("rax", 0))
             + x86("mov", "esi", RDD("r9", 40 * i + 32))
             + x86("movsxd", "rsi", "esi")
             + x86("lea", "rsp", RDQ(x86_hb(), kt))
             + x86("mov", x86_hb(), RDQ(x86_hb(), kt - 8))
             + x86("note", "a ball in flight (r15 armed) propagates past every remaining clause, never retries one -- the static chain's step does the same (its _step_ball arm)")
             + x86("test", "r15", "r15")
             + x86("jne", L(225))
             + x86("cmp", "r11", 0L)
             + x86("jl", L(225))
             + x86_def_ext(g_emit.flat_pkt_walk_p)
             + x86("cmp", "esi", 0L)
             + x86("jl", L(225))
             + xa_flat_pkt_slot_addr("rsi", "r10", "r11")
             + x86("mov", "r11d", RDD("r10", 20))
             + x86("cmp", "r11d", "r8d")
             + x86("jge", L(224))
             + x86("mov", "r11d", RDD("r10", 16))
             + x86("test", "r11d", "r11d")
             + x86("je", L(223))
             + x86("cmp", "r11d", "r8d")
             + x86("jl", L(224))
             + x86("def", L(223))
             + x86("mov", "r11d", RDD("r10", 24))
             + x86("test", "r11d", "r11d")
             + x86("jns", L(226))
             +
                 x86("note",
                 "a slot no call has reached has no fragment yet (ARCH-PROLOG-DATABASE section 5 narrowed to the flip): compile it now, from the slot index and the packet's root cell, and walk on")
             + x86("push", "rcx")
             + x86("push", "rdx")
             + x86("push", "rsi")
             + x86("push", "r8")
             + xa_flat_pkt_cell_rax(k)
             + x86("mov", "rdx", "rsi")
             + x86("mov", "rsi", "rax")
             + x86("mov", "rdi", "r14")
             + x86("call", "rt_pl_db_slot_ensure", (uint64_t)(uintptr_t)(void *)rt_pl_db_slot_ensure)
             + x86("pop", "r8")
             + x86("pop", "rsi")
             + x86("pop", "rdx")
             + x86("pop", "rcx")
             + xa_flat_pkt_cell_rax(k) + x86("mov", "r9", RDQ("rax", 0))
             + xa_flat_pkt_slot_addr("rsi", "r10", "r11")
             + x86("def", L(226))
             + x86("mov", "r11d", RDD("r10", 24))
             + x86("mov", "rdi", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_runtime, "g_runtime")
             + x86("mov", "rdi", RDQ("rdi", GRT_AT(rt_gen_procs)))
             + x86_shift_imm("shl", 4, "r11", 7) + x86("add", "rdi", "r11")
             + x86("mov", "rdi", RDQ("rdi", 8))
             + x86("mov", "r11d", RDD("r10", 28))
             + x86("movsxd", "r11", "r11d")
             + x86("add", "rdi", "r11")
             + x86_jmp_reg("rdi")
             + x86("def", L(224))
             + x86("mov", "esi", RDD("r10", 32))
             + x86("movsxd", "rsi", "esi")
             + x86("jmp", "extlbl", (uint64_t)(uintptr_t)g_emit.flat_pkt_walk_p)
             + x86("def", L(225)) + IF(np > 0,
             x86("add", "rsp", (long)(16 * np))) + x86_jmp_reg("rdx");
}
static std::string zf_epo_pkt(int kt, int k, int i, int np) {
    return (!g_emit.flat_pkt_chainω_p || !g_emit.flat_pkt_walk_p) ? x86_bomb("packet fragment chain-omega without its labels") : zf_epo_pkt_walk(kt, k, i, np);
}
static std::string zf_epo_pl(int kt, int l_ok) {
    return IF(emit_pl_scc_armed(), x86_pl_scc_release_check(x86_hb(), kt, l_ok))
             + x86("mov", "rcx", RDQ(x86_hb(), kt - 16))
             + x86("mov", "r13", RDQ(x86_hb(), kt - 40))
             + zf_release(kt)
             + zf_pin_restore(kt)
             + x86("jmp", "rcx");
}
static std::string zf_epo_plain(int kt) {
    return x86("comment", "ICN-FR-2 zframe epilogue-ω: load ω wire from [kt-16]; unwind to flat base; jmp. NOTE: no caller-base restore happens here — the [kt-8] slot is "
                          "WRITE-ONLY on every arm that fills it (s247)")
         + x86("mov", "rcx", "qword ptr [rsp# + " + std::to_string(kt - 16) + "]")
         + zf_display_restore(kt)
         + zf_release(kt)
         + zf_pin_restore(kt)
         + std::string("")
         + x86("jmp", "rcx");
}
static std::string zf_epo(int kt) {
    return (icn_wire_stack_on() && g_emit_cfg && g_emit_cfg->cell_frame && g_emit.flat_lcl_proc) ? zf_epo_wire(kt)
         : zf_pas_nest_graph() ? zf_epo_nest(kt)
         : (x86_resumable_frame() && g_emit.flat_pkt) ? zf_epo_pkt(kt, g_emit.flat_pkt_cell, g_emit.flat_pkt_slot, zf_pro_np())
         : x86_resumable_frame() ? zf_epo_pl(kt, 250)
         : zf_epo_plain(kt);
}
static std::string xa_flat_zframe_epilogue_ω_str(void) {
    return xa_flat_class_zf() ? zf_epo(zf_epi_kt()) : std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pl_switch_ext(const char * op, bb_label_t * l) { return x86(op, "extlbl", (uint64_t)(uintptr_t)l); }
static int pl_switch_has(const pl_ix_arm_t * arms, int n, int tag) { return n > 0 && (arms[n - 1].tag == tag || pl_switch_has(arms, n - 1, tag)); }
static std::string pl_switch_arm_ref(const pl_ix_arm_t * a) { return x86("cmp", "ecx", (long)a->val) + pl_switch_ext("je", a->to); }
static std::string pl_switch_arm_val(const pl_ix_arm_t * a) { return x86_movabs_r64("rax", (uint64_t)a->val) + x86("cmp", "rsi", "rax") + pl_switch_ext("je", a->to); }
static std::vector<const pl_ix_arm_t *> pl_sw_sorted(const pl_ix_arm_t * arms, int n, int tag) {
    std::vector<const pl_ix_arm_t *> v;
    for (int i = 0; i < n; i++) if (arms[i].tag == tag) v.push_back(&arms[i]);
    std::stable_sort(v.begin(), v.end(), [tag](const pl_ix_arm_t * x, const pl_ix_arm_t * y) { return tag == DT_PLREF ? (int32_t)x->val < (int32_t)y->val : x->val < y->val; });
    return v;
}
static int pl_sw_nodes(int n) { return n <= 8 ? 0 : 1 + pl_sw_nodes(n / 2) + pl_sw_nodes(n - n / 2 - 1); }
static int pl_sw_cnt(const pl_ix_arm_t * arms, int n, int tag) { return (int)pl_sw_sorted(arms, n, tag).size(); }
static std::string pl_sw_leaf(const std::vector<const pl_ix_arm_t *> & v, int lo, int hi, int tag, bb_label_t * fb) {
    return lo >= hi ? pl_switch_ext("jmp", fb) : (tag == DT_PLREF ? pl_switch_arm_ref(v[lo]) : pl_switch_arm_val(v[lo])) + pl_sw_leaf(v, lo + 1, hi, tag, fb);
}
static std::string pl_sw_cmp(const pl_ix_arm_t * a, int tag) {
    return tag == DT_PLREF ? x86("cmp", "ecx", (long)a->val) + pl_switch_ext("je", a->to) : x86_movabs_r64("rax", (uint64_t)a->val) + x86("cmp", "rsi", "rax") + pl_switch_ext("je", a->to);
}
static std::string pl_sw_tree(const std::vector<const pl_ix_arm_t *> & v, int lo, int hi, int tag, bb_label_t * fb, bb_label_t ** nl, int base) {
    return hi - lo <= 8 ? pl_sw_leaf(v, lo, hi, tag, fb)
         : pl_sw_cmp(v[lo + (hi - lo) / 2], tag) + pl_switch_ext("jl", nl[base]) + pl_sw_tree(v, lo + (hi - lo) / 2 + 1, hi, tag, fb, nl, base + 1)
         + x86_def_ext(nl[base]) + pl_sw_tree(v, lo, lo + (hi - lo) / 2, tag, fb, nl, base + 1 + pl_sw_nodes(hi - lo - (hi - lo) / 2 - 1));
}
static std::string pl_sw_class(const pl_ix_arm_t * arms, int n, int tag, bb_label_t * fb, bb_label_t ** nl, int base) {
    std::vector<const pl_ix_arm_t *> v = pl_sw_sorted(arms, n, tag);
    return pl_sw_tree(v, 0, (int)v.size(), tag, fb, nl, base);
}
#define PLSW_REF() (pl_switch_has(arms, narms, DT_PLREF) || fb_ref != chain)
#define PLSW_ATOM() (pl_switch_has(arms, narms, DT_PLATOM) || fb_atom != chain)
#define PLSW_INT() (pl_switch_has(arms, narms, DT_I) || fb_int != chain)
#define PLSW_BR 0
#define PLSW_BA (PLSW_BR + pl_sw_nodes(pl_sw_cnt(arms, narms, DT_PLREF)))
#define PLSW_BI (PLSW_BA + pl_sw_nodes(pl_sw_cnt(arms, narms, DT_PLATOM)))
static std::string xa_pl_switch_str(int a0_off, const pl_ix_arm_t * arms, int narms, bb_label_t * fb_ref, bb_label_t * fb_atom, bb_label_t * fb_int, bb_label_t * chain, bb_label_t ** nl) {
    x86_begin();
    return x86("comment", "PL SWITCH (ARCH-PROLOG-C-OUT-OF-THE-BOX 11; the design of record's section 7.2, V1): the first argument's key -- a functor id, an atom id or a small integer -- "
                                   "selects the clauses whose first head argument can match it; ONE candidate is entered in last-alternative form (no next alternative, B back to the outer choice), "
                                   "NONE concedes through the step, two or more take the full chain; an unbound, float, bignum or text first argument takes the chain")
         + x86("lea", "rdi", FRQ(a0_off))
         + PL_DEREF(10, 11, 12, 13)
         + IF(PLSW_REF(), x86("cmp", "al", (long)DT_PLREF)
                        + x86("je", L(20)))
         + IF(PLSW_ATOM(), x86("cmp", "al", (long)DT_PLATOM)
                         + x86("je", L(30)))
         + IF(PLSW_INT(), x86("cmp", "al", (long)DT_I)
                        + x86("je", L(40)))
         + pl_switch_ext("jmp", chain)
         + IF(PLSW_REF(), x86("def", L(20))
                        + x86("mov", "ecx", RDD("rdi", 4))
                        + pl_sw_class(arms, narms, DT_PLREF, fb_ref, nl, PLSW_BR))
         + IF(PLSW_ATOM(), x86("def", L(30))
                         + x86("mov", "rsi", RDQ("rdi", 8))
                         + pl_sw_class(arms, narms, DT_PLATOM, fb_atom, nl, PLSW_BA))
         + IF(PLSW_INT(), x86("def", L(40))
                        + x86("mov", "ecx", RDD("rdi", 4))
                        + x86("test", "ecx", "ecx")
                        + pl_switch_ext("jne", chain)
                        + x86("mov", "rsi", RDQ("rdi", 8))
                        + pl_sw_class(arms, narms, DT_I, fb_int, nl, PLSW_BI));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int xa_pl_switch_nodes(const pl_ix_arm_t * arms, int narms) { return pl_sw_nodes(pl_sw_cnt(arms, narms, DT_PLREF)) + pl_sw_nodes(pl_sw_cnt(arms, narms, DT_PLATOM)) + pl_sw_nodes(pl_sw_cnt(arms, narms, DT_I)); }
extern "C" void xa_pl_switch(int a0_off, const pl_ix_arm_t * arms, int narms, bb_label_t * fb_ref, bb_label_t * fb_atom, bb_label_t * fb_int, bb_label_t * chain, bb_label_t ** nl) {
    bb_emit_x86(xa_pl_switch_str(a0_off, arms, narms, fb_ref, fb_atom, fb_int, chain, nl)); }
extern "C" void xa_flat_zframe_prologue(void) { bb_emit_x86(xa_flat_zframe_prologue_str()); }
extern "C" void xa_flat_chain_prologue(const char * fname, int * argkt_out) { bb_emit_x86(xa_flat_chain_prologue_str(fname, argkt_out)); }
extern "C" void xa_flat_chain_epilogue(void) { bb_emit_x86(xa_flat_chain_epilogue_str()); }
extern "C" void xa_flat_chain_epilogue_sig(int is_gamma, const char * fname) { bb_emit_x86(xa_flat_chain_epilogue_sig_str(is_gamma, fname)); }
extern "C" void xa_flat_zframe_epilogue_γ(void) { bb_emit_x86(xa_flat_zframe_epilogue_γ_str()); }
extern "C" void xa_flat_zframe_epilogue_ω(void) { bb_emit_x86(xa_flat_zframe_epilogue_ω_str()); }
