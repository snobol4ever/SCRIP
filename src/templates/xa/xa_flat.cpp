#include <unordered_set>
#include <string>
#include <cstdlib>
#include <cstdio>
#include "emit.h"
#include "gc_frame_map.h"
#include "x86_asm.h"
extern "C++" int zf_display_level(void);
extern "C++" int zf_pas_nest_graph(void);
extern "C++" int zf_display_mem_off(int dl);
extern "C++" int xa_flat_class_zf(void);
extern "C++" int xa_flat_class_c(void);
extern "C++" int xa_flat_sig_names(const char * fname, int * nf_out, int * nsave_out, int * gk_out, int * res_gk_out = 0);
extern "C++" const char * xa_icn_trace_intern(const char * s);
extern "C++" int xa_flat_graph_holds_a_frame_retry(void);
#include "pin_va.h"
extern "C" {
#include "xa_template_common.h"
}
#include "../bb/bb_templates.h"
#include "../bb/bb_pl_cell.h"
extern "C" void rt_jmp_frame_lexprep(void *, long);
extern "C" void rt_jmp_frame_lexprep2(void *, long, long);
extern "C" void rt_main_args_fetch(void);
extern "C" int rt_proc_nformals(const char *name);
extern "C" int bb_scc_probe(const char *fname, int nargs, int *np_out, int *nsave_out, int *gk_out, int *res_gk_out);
extern int g_rt_fragment_emit;
extern int * const rt_k_level_p;
extern int64_t kw_fnclevel;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define icn_wire_stack_on() emit_knob_unless_zero("SCRIP_ICN_WIRE_STACK")
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_entry_dispatch_str(void) {
    if (MEDIUM_MACRO_DEF) return x86("comment", "# no macro form — XA_ENTRY_DISPATCH");
    if (MEDIUM_BINARY) return std::string();
    if (MEDIUM_TEXT) {
        if (!g_is_text) return std::string();
        return std::string("  cmp esi, 0\n")
             + "  je " + (g_emit.flat_lbl_α_body ? g_emit.flat_lbl_α_body : "?") + "\n"
             + "  jmp " + (g_emit.flat_lbl_β ? g_emit.flat_lbl_β : "?") + "\n";
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_flat_data_section_str(void) {
    if (MEDIUM_MACRO_DEF) return x86("comment", "# no macro form — XA_FLAT_DATA_SECTION");
    if (MEDIUM_BINARY) return std::string();
    if (MEDIUM_TEXT) {
        if (!g_flat_data_any) return std::string();
        return std::string("  .section .data\n")
             + (g_flat_data_len ? std::string((const char *)g_flat_data_buf.p, g_flat_data_len) : std::string())
             + "  .section .text\n";
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void xa_entry_dispatch(void) {
    auto s = xa_entry_dispatch_str();
    if (!s.empty()) emit_text_n(s.data(), s.size());
}
extern "C" void xa_flat_data_section(void) {
    auto s = xa_flat_data_section_str();
    if (!s.empty()) emit_text_n(s.data(), s.size());
}
extern "C" void rt_pl_dc_prep(void *, long, long, long, long, long);
extern "C" DESCR_t rt_pl_dc_leave_γ(DESCR_t, long, void *);
extern "C" DESCR_t rt_pl_dc_leave_ω(long, void *);
extern "C" void rt_arg_stage(int idx, DESCR_t v);
extern "C" int zls_g_block_args(const IR_graph_t *);
extern "C" { struct rt_sxt_fr_s; extern struct rt_sxt_fr_s * const rt_sxt_fr_p; }
extern "C" struct gv_s g_call_args;
extern "C" void rt_icn_zframe_args_install(void *, int, int);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_flat_dc_stub_str(void) {
    x86_begin();
    int kt = g_emit.flat_frame_bytes;
    int anchor = -1;
    int suffix = (g_emit.flat_seed_off >= 16) ? g_emit.flat_seed_off : 16;
    extern int g_flat_dc_np;
    int np = g_flat_dc_np;
    static const char * const argreg[4] = { "rsi", "rdx", "rcx", "r8" };
    if (!x86_fb_pinned() && g_emit_cfg && zls_g_block_args(g_emit_cfg)) {
        int nb = g_emit_cfg->nparams;
        if (np > nb || nb > 12) return x86_bomb("xa_flat_dc_stub: a block-protocol graph whose direct-call arity exceeds its parameter count, or has more than 12 parameters");
        std::string zs = x86("comment",
            "the block protocol, Raku regime: the direct-call stub builds the callee's argument block on the spine from "
                "the caller's cells and jumps past the staged entry; no rt_arg_stage, no poll (nothing can allocate between here and the callee's first safe point)")
            + x86("pop", "rax")
            + x86("push", "rax")
            + x86("push", "rax")
            + IF(nb > 0, x86("sub", "rsp", (long)(16 * nb)));
        for (int i = 0; i < np; i++)
            zs += x86("mov", "rax", "[" + std::string(argreg[i]) + " + 0]")
                + x86("mov", "rdi", "[" + std::string(argreg[i]) + " + 8]")
                + x86("mov", RDQ("rsp", 16 * i), "rax")
                + x86("mov", RDQ("rsp", 16 * i + 8), "rdi");
        for (int i = np; i < nb; i++)
            zs += x86("mov", RDQ("rsp", 16 * i), (long)DT_SNUL)
                + x86("mov", RDQ("rsp", 16 * i + 8), 0L);
        zs += x86_lea_id("rcx", 2)
            + x86_lea_id("rdx", 3)
            + x86_jmp_lblptr(g_emit.flat_dc_body_p, g_emit.flat_dc_body_p ? g_emit.flat_dc_body_p->name : "?")
            + x86_deflabel_id(2)
            + x86_gc_site_raw(X86_SITE_RET_ROAD, 7, 8)
            + x86("add", "rsp", 8L)
            + x86("ret")
            + x86_deflabel_id(3)
            + x86_gc_site_raw(X86_SITE_RET_ROAD, 7, 8)
            + x86("add", "rsp", 8L)
            + x86("mov32", "eax", 104L)
            + x86("xor", "edx", "edx")
            + x86("ret");
        return zs;
    }
    if (g_emit.zframe_graph || (g_emit_cfg && g_emit_cfg->icn_cells_graph && g_emit.flat_lcl_proc)) {
        static const char * const dcarg4[4] = { "rsi", "rdx", "rcx", "r8" };
        bool need_align_pad = (np > 0) && (np % 2 == 1);
        std::string zs = x86("comment", "ICN-FR-3 zframe dc stub: stage args, jmp proc_f_α≡0 with wire shims")
            + x86("pop", "rax")
            + x86("push", "rax")
            + x86("push", "rax");
        if (need_align_pad) zs += x86("push", "rax");
        for (int i = np - 1; i >= 0; i--) zs += x86("push", dcarg4[i]);
        int push_bytes = np * 8 + (need_align_pad ? 8 : 0);
        for (int i = 0; i < np; i++) {
            zs += x86("mov", "rax", "[rsp + " + std::to_string(i * 8) + "]")
                + x86("mov32", "edi", (long)i)
                + x86("mov", "rsi", "[rax + 0]")
                + x86("mov", "rdx", "[rax + 8]")
                + x86("call", "rt_arg_stage", TEMPLATE_FN_ADDR(rt_arg_stage))
           + x86_rt_gc_poll();
        }
        if (push_bytes > 0) zs += x86("add", "rsp", (long)push_bytes);
        if (icn_wire_stack_on() && g_emit_cfg && g_emit_cfg->icn_cells_graph && g_emit.flat_lcl_proc) {
            zs += x86_lea_id("rcx", 3) + x86("push", "rcx")
                + x86_lea_id("rcx", 2) + x86("push", "rcx")
                + x86_jmp_lblptr(g_emit.flat_dc_body_p, g_emit.flat_lbl_α ? g_emit.flat_lbl_α : "?")
                + x86_deflabel_id(2)
                + x86_gc_site_raw(X86_SITE_RET_ROAD, 7, 24)
                + x86("add", "rsp", 24L)
                + x86("ret")
                + x86_deflabel_id(3)
                + x86_gc_site_raw(X86_SITE_RET_ROAD, 7, 24)
                + x86("add", "rsp", 24L)
                + x86("mov32", "eax", 104L)
                + x86("xor", "edx", "edx")
                + x86("ret");
            return zs;
        }
        zs += x86_lea_id("rcx", 2)
            + x86_lea_id("rdx", 3)
            + x86_jmp_lblptr(g_emit.flat_dc_body_p, g_emit.flat_lbl_α ? g_emit.flat_lbl_α : "?")
            + x86_deflabel_id(2)
            + x86_gc_site_raw(X86_SITE_RET_ROAD, 7, 8)
            + x86("add", "rsp", 8L)
            + x86("ret")
            + x86_deflabel_id(3)
            + x86_gc_site_raw(X86_SITE_RET_ROAD, 7, 8)
            + x86("add", "rsp", 8L)
            + x86("mov32", "eax", 104L)
            + x86("xor", "edx", "edx")
            + x86("ret");
        return zs;
    }
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
         + IF(anchor >= 0, x86("mov", FRQ(anchor), "rsp"))
         + FOR(0, np, [&](int i) { return x86("mov", FRQ(16 + 8 * i), argreg[i]); })
         + x86("mov", "rdi", "rsp")
         + x86("mov32", "esi", (long)suffix)
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_flat_block_staged_entry_str(void) {
    x86_begin();
    int nb = g_emit_cfg ? g_emit_cfg->nparams : 0;
    if (nb <= 0) return std::string();
    if (nb > 12) return x86_bomb("xa_flat_block_staged_entry: more than 12 parameters");
    std::string s = x86("comment",
        "the staged entry of a block-protocol graph (every by-name road, glue and C entry stages g_call_args and jumps "
            "here): build the argument block on the spine from the staged cells, a cell beyond the medium's capacity a "
                "null DESCR, then fall into the block entry; a direct call (the dc stub) jumps past this")
        + x86("mov", "rsi", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_call_args, "g_call_args")
        + x86("mov", "rdi", "[rsi + 0]")
        + x86("mov", "esi", RDD("rsi", 12))
        + x86("sub", "rsp", (long)(16 * nb));
    for (int i = 0; i < nb; i++) {
        s += x86("cmp", "esi", (long)(i + 1))
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
    return s;
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
static std::string zf_display_restore(int kt) {
    int dl = zf_display_level();
    if (!dl) return std::string();
    if (dl <= 3) {
    const char * dr = dl == 1 ? "r13" : dl == 2 ? "r14" : "r15";
        return x86("comment", "PAS-DISPLAY-1: restore caller display[L] from [kt-40]")
             + x86("mov", dr, FRQ(kt - 40)); }
    int off = zf_display_mem_off(dl);
    if (off < 0) return x86_bomb("PAS-DISPLAY-N: level>3 ancestor slot unresolved (restore)");
    return x86("comment", "PAS-DISPLAY-N: restore [r15+off] from [kt-40] (saved caller value)")
         + x86("mov", "rax", FRQ(kt - 40))
         + x86("mov", RDQ("r15", off), "rax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string zf_pin_restore(int kt) {
    if (!x86_fb_pinned()) return std::string();
    return x86("comment", "PZ-4 (e): caller base <- [kt-8], read through the pin")
         + x86("mov", x86_fb(), RDQ(x86_fb(), kt - 8));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int zls_g_det_block(const IR_graph_t * g);
#define zf_det_block_size() ((g_emit_cfg && zls_g_det_block(g_emit_cfg)) ? 16L * g_emit_cfg->nparams : 0L)
static std::string zf_release(int kt) {
    if (!x86_fb_pinned()) {
    int np = (g_emit_cfg && zls_g_block_args(g_emit_cfg)) ? g_emit_cfg->nparams : 0;
    return x86("add", "rsp", (long)(kt + 16 * np));
}
    { int np = (g_emit_cfg && zls_g_block_args(g_emit_cfg)) ? g_emit_cfg->nparams : 0;
      return x86("comment", "PZ-4 PL-ZA-2: exact release off the pin, not off wherever rsp happens to be; the block protocol releases the caller's argument block with the frame")
           + x86("lea", "rsp", RDQ(x86_fb(), kt + 16 * np)); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void rt_pl_quad_seed(void *);
static std::string pl_standing_cells_zero(int kt, int n) {
    std::string s;
    if (n > 16) return x86("lea", "rdi", RDQ("rsp", kt - 64 - 24 - 8 * (n - 1)))
                     + x86("xor", "eax", "eax")
                     + x86("mov32", "ecx", (long)n)
                     + x86("rep_stosq");
    for (int k = 0; k < n; k++) s += x86("mov", RDQ("rsp", kt - 64 - 24 - 8 * k), 0L);
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void *g_rt_gen_procs;
#define xa_flat_pkt_slot_addr(idx64, out, tmp) ( \
      x86("mov", (out), (idx64)) + x86_shift_imm("shl", 4, (out), 5) \
    + x86("mov", (tmp), (idx64)) + x86_shift_imm("shl", 4, (tmp), 3) \
    + x86("add", (out), (tmp)) \
    + x86("add", (out), "r9") \
)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int rt_pl_db_frame_cells(void);
static std::string xa_flat_pkt_cell_rax(int k) {
    int nf = rt_pl_db_frame_cells();
    if (k < nf) return x86("mov", "rax", RDQ("r14", -24 - 8 * k));
    return x86("note",
        "a root cell past the frame's PL_DB_FRAME_CELLS lives in the registry's overflow vector: cell 0 holds the "
            "registry, [registry + 24] the vector, re-read here because the collector may move both")
         + x86("mov", "rax", RDQ("r14", -24))
         + x86("mov", "rax", RDQ("rax", 24))
         + x86("mov", "rax", RDQ("rax", 8 * (k - nf)));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_flat_pkt_fresh_entry_str(void) {
    int k = g_emit.flat_pkt_cell, i = g_emit.flat_pkt_slot;
    if (k < 0 || i < 0 || !g_emit.flat_pkt_walk_p) return x86_bomb("xa_flat_pkt_fresh_entry: a packet fragment without its cell, its slot or its walk label");
    return x86("comment",
        "PACKET FRAGMENT fresh entry (ARCH-PROLOG-C-OUT-OF-THE-BOX 5.2 A): the registry road -- the packet from root cell k, G = next_ref, this slot visible at G, else the walk from the head")
         + xa_flat_pkt_cell_rax(k)
         + x86("mov", "r8d", RDD("rax", 20))
         + x86("mov", "r9", RDQ("rax", 0))
         + x86("mov", "r11d", RDD("r9", 40 * i + 20))
         + x86("cmp", "r11d", "r8d")
         + x86("jge", L(210))
         + x86("mov", "r11d", RDD("r9", 40 * i + 16))
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
         + x86("def", L(212));
}
extern "C" void xa_flat_pkt_fresh_entry(void) { bb_emit_x86(xa_flat_pkt_fresh_entry_str()); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_flat_zframe_prologue_str(void) {
    if (!g_emit.zframe_graph) return std::string();
    int kt = g_emit.flat_frame_bytes;
    if (kt < 48 || (kt & 15)) { fprintf(stderr, "FATAL xa_flat_zframe_prologue: kt=%d (must be 16-mult >= 48)\n", kt); abort(); }
    int np = g_emit_cfg ? g_emit_cfg->nparams : 0;
    int nl = g_emit_cfg ? g_emit_cfg->nlocals : 0;
    uint64_t _seed_fp; { void (*_f)(void *) = rt_pl_quad_seed; _seed_fp = (uint64_t)(uintptr_t)(void *)_f; }
    std::string s = x86("comment", "ICN-FR-2 zframe prologue: sub rsp,kt + wire header [kt-24]=γ [kt-16]=ω [kt-8]=caller____ + pin ___=rsp")
         + x86("sub", "rsp", (long)kt)
         + x86("mov", "[rsp + " + std::to_string(kt - 24) + "]", "rcx")
         + x86("mov", "[rsp + " + std::to_string(kt - 16) + "]", "rdx")
         + (x86_fb_pinned()
            ? ( x86("mov", "[rsp + " + std::to_string(kt - 8) + "]", "rbp")
             + x86("mov", x86_fb(), "rsp")
             + IF(g_emit_cfg && g_emit_cfg->root_graph, x86("lea", "rdi", RDQ("rsp", kt - 64))
                                                      + x86("call_bare", "rt_pl_quad_seed", _seed_fp))
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
             + x86("mov", RDQ("rsp", kt - 64), "r12")
             + IF(g_emit_cfg && g_emit_cfg->n_alts > 1 && g_emit.flat_alt1_p,
                    x86("lea", "rax", "extlbl", (uint64_t)(uintptr_t)g_emit.flat_alt1_p)
                 + x86("mov", RDQ("rsp", kt - 56), "rax")
                 + x86("note", "pl_choice_open inline (ARCH-PROLOG-C-OUT-OF-THE-BOX 2.2): B := this frame's header H = rsp+kt-64")
                 + x86("lea", "r13", RDQ("rsp", kt - 64))))
            : (emit_jmp_pin_legacy() ? (xa_flat_zanchor_poison() ? std::string() : x86("mov", "[rsp + " + std::to_string(kt - 8) + "]", "rsp"))
                                + std::string("")
                               : std::string()));
    if (x86_fb_pinned()) {
        int zb = kt - 64;
        s += x86("comment",
            "the block protocol (ARCH-PROLOG-C-OUT-OF-THE-BOX 1.2): the arguments are the caller's block at [rbp+kt+16i], "
                "never copied; the value region [0, kt-64) is zeroed inline so every mapped slot is a null DESCR at the first poll")
           + (zb <= 128 ? FOR(0, zb / 8, [&](int q) { return x86("mov", RDQ("rsp", 8 * q), 0L); })
                        : x86("mov", "rdi", "rsp")
                        + x86("xor", "eax", "eax")
                        + x86("mov32", "ecx", (long)(zb / 8))
                        + x86("rep_stosq"));
    } else if (zls_g_block_args(g_emit_cfg)) {
        int zb = kt - 32;
        s += x86("comment",
            "the block protocol, Raku regime (ARCH-PROLOG-C-OUT-OF-THE-BOX 1.2 as landed for Prolog at e95282e39): the arguments are the caller's block at [rsp+kt+16i], "
                "never copied; the value region [0, kt-32) is zeroed inline; the string-extension table is invalidated inline, as rt_icn_zframe_args_install did (c8701b17e)")
           + (zb <= 128 ? FOR(0, zb / 8, [&](int q) { return x86("mov", RDQ("rsp", 8 * q), 0L); })
                        : x86("mov", "rdi", "rsp")
                        + x86("xor", "eax", "eax")
                        + x86("mov32", "ecx", (long)(zb / 8))
                        + x86("rep_stosq"))
           + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&rt_sxt_fr_p, "rt_sxt_fr_p")
           + x86("mov", "rax", RDQ("rax", 0))
           + x86("mov", RDD("rax", 20), 1L)
           + x86("mov", RDQ("rax", 0), 0L);
    } else if (g_emit.flat_lex) {
        extern int g_flat_dc_np;
        if (g_flat_dc_np >= 0) {
            if (kt > 48) {
                int data_bytes = kt - 32;
                s += x86("mov", "rdi", "rsp")
                   + x86("xor", "eax", "eax")
                   + x86("mov32", "ecx", (long)data_bytes)
                   + x86("rep_stosb");
            }
            uint64_t _args_fp; { void (*_f)(void *, int, int) = rt_icn_zframe_args_install; _args_fp = (uint64_t)(uintptr_t)(void *)_f; }
            s += x86("mov", "rdi", "rsp")
               + x86("mov32", "esi", (long)np)
               + x86("mov32", "edx", (long)nl)
               + x86("call", "rt_icn_zframe_args_install", _args_fp);
        } else {
            uint64_t _lex_fp; { void (*_f)(void *, long, long) = rt_jmp_frame_lexprep2; _lex_fp = (uint64_t)(uintptr_t)(void *)_f; }
            int seed_off = g_emit.flat_seed_off ? g_emit.flat_seed_off : 16;
            s += x86("mov", "rdi", "rsp")
               + x86("mov32", "esi", (long)seed_off)
               + x86("mov32", "edx", (long)(kt - (x86_fb_pinned() ? 64 : 32)))
               + x86("call_bare", "rt_jmp_frame_lexprep2", _lex_fp);
            if (np > 0) {
                uint64_t _args_fp; { void (*_f)(void *, int, int) = rt_icn_zframe_args_install; _args_fp = (uint64_t)(uintptr_t)(void *)_f; }
                s += x86("mov", "rdi", "rsp")
                   + x86("mov32", "esi", (long)np)
                   + x86("mov32", "edx", 0L)
                   + x86("call_bare", "rt_icn_zframe_args_install", _args_fp);
            }
        }
    } else {
        if (kt > 48) {
            int data_bytes = kt - (x86_fb_pinned() ? 64 : 32);
            s += x86("mov", "rdi", "rsp")
               + x86("xor", "eax", "eax")
               + x86("mov32", "ecx", (long)data_bytes)
               + x86("rep_stosb");
        }
        if (np > 0 || nl > 0) {
            uint64_t _args_fp; { void (*_f)(void *, int, int) = rt_icn_zframe_args_install; _args_fp = (uint64_t)(uintptr_t)(void *)_f; }
            s += x86("mov", "rdi", "rsp")
               + x86("mov32", "esi", (long)np)
               + x86("mov32", "edx", (long)nl)
               + x86("call", "rt_icn_zframe_args_install", _args_fp);
        }
    }
    { int _dl = zf_display_level();
      if (_dl && _dl <= 3) {
    const char * _dr = _dl == 1 ? "r13" : _dl == 2 ? "r14" : "r15";
          s += x86("comment", "PAS-DISPLAY-1: save caller display[L] into [kt-40]; display[L] = this frame")
             + x86("mov", FRQ(kt - 40), _dr)
             + x86("mov", _dr, "rsp"); }
      else if (_dl > 3) {
    int _off = zf_display_mem_off(_dl);
          if (_off < 0) { s += x86_bomb("PAS-DISPLAY-N: level>3 ancestor slot unresolved (save)"); }
          else { s += x86("comment", "PAS-DISPLAY-N: save [r15+off] into [kt-40]; [r15+off] = this frame")
                    + x86("mov", "rax", RDQ("r15", _off))
                    + x86("mov", FRQ(kt - 40), "rax")
                    + x86("mov", "rax", "rsp")
                    + x86("mov", RDQ("r15", _off), "rax"); } } }
    if (g_emit.flat_pkt) { if (!g_emit_cfg
        || kt - g_emit_cfg->jcon_value_region != FLAT_FRAME_ALLOWANCE_PINNED) return x86_bomb("packet fragment: the pinned allowance is not 96, the G word at [kt-80] has no home");
        s += x86("comment", "PACKET FRAGMENT: G (the generation this call took) as a packed RAW word in the pinned allowance's spare cell [kt-80], read by the chain-omega and the gamma scan")
           + x86("mov", "rax", "r8") + x86_shift_imm("shl", 4, "rax", 8) + x86_or_imm("rax", (long)DT_RAW)
           + x86("mov", FRQ(kt - 80), "rax")
           + x86("mov", FRQ(kt - 72), 0L)
           + x86("note",
               "PACKET FRAGMENT: this frame is the youngest choice from its entry (B = its header, the floor word at its "
                   "base), as a multi-clause chain's alternation is, so every binding of a caller cell is trailed and the chain-omega's unwind undoes it before the next clause")
           + x86_pl_disj_open(x86_fb(), kt, 236, 237); }
    if (g_emit_cfg && g_emit_cfg->root_graph) s += emit_gc_map_cell(kt - FLAT_FRAME_ALLOWANCE_ROOT, kt, FLAT_FRAME_ALLOWANCE_ROOT - 16, GC_FRAME_MAP_ROOT, 0, 0u);
    else s += emit_gc_map_cell(g_emit_cfg ? g_emit_cfg->jcon_value_region : 0, kt, kt - (g_emit_cfg ? g_emit_cfg->jcon_value_region : 0) - 16,
        (!g_rt_fragment_emit && g_emit.flat_fam && (!strcmp(g_emit.flat_fam, "main") || !strcmp(g_emit.flat_fam, "pat_flat"))) ? GC_FRAME_MAP_ROOT : 0u, 0,
        (zf_pas_nest_graph() ? GC_LINK_Q(kt + (int)zf_det_block_size(), kt - 8, kt + (int)zf_det_block_size() + 16, x86_fb_pinned() ? 0u : GC_LINK_RBP_KEPT) :
        GC_LINK_Q(kt - 24, kt - 8, kt + 16 * ((g_emit_cfg && zls_g_block_args(g_emit_cfg)) ? g_emit_cfg->nparams : 0), x86_fb_pinned() ? 0u : GC_LINK_RBP_KEPT)));
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define xa_flat_sig_gq(gk, w) (g_rtcc_on ? std::string(GVARQ((gk), (w))) : std::string(ABSQ(RT_GVA_VA + (unsigned long)(gk) * 16 + (unsigned long)(w))))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define xa_flat_wn_park(fname) (!g_rt_fragment_emit && (fname) && (fname)[0] && !strchr((fname), '$') && strncmp((fname), "LBL__", 5) != 0)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_flat_wn_park_str(int kt, const char * fname) {
    extern int rt_g_want_name; extern int rt_g_ret_by_name;
    if (!xa_flat_wn_park(fname)) return std::string();
    return x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&rt_g_ret_by_name, "rt_g_ret_by_name")
         + x86("mov", RDD("rax", 0), (long)0)
         + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&rt_g_want_name, "rt_g_want_name")
         + x86("mov", "edx", RDD("rax", 0))
         + x86("mov", RDQ("rsp", kt - 16), "rdx")
         + x86("mov", RDD("rax", 0), (long)0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_flat_wn_restore_str(int kt, const char * fname) {
    extern int rt_g_want_name;
    if (!xa_flat_wn_park(fname)) return std::string();
    return x86("comment", "name request: put the parked request back so the call site's by-name consult reads THIS call's")
         + x86("push", "rax")
         + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&rt_g_want_name, "rt_g_want_name")
         + x86("mov", "rcx", RDQ("rsp", kt - 16 + 8))
         + x86("mov", RDD("rax", 0), "ecx")
         + x86("pop", "rax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_flat_chain_prologue_str(const char * fname) {
    if (!xa_flat_class_c()) return std::string();
    {
    static int _d = -1; if (_d < 0) {
    const char * e = getenv("SCRIP_CHAIN_DIAG");
    _d = (e && *e == '1') ? 1 : 0;
}
      if (_d) { extern int bb_emit_pos; fprintf(stderr, "[CHAINFRAME] pos=%d kt=%d text=%d jmp=%d pat=%d\n",
                                                bb_emit_pos, g_emit.flat_frame_bytes, g_is_text ? 1 : 0, g_emit.flat_jmp_entry, g_emit.flat_pat); } }
    int kt = g_emit.flat_frame_bytes;
    if (kt & 15) { fprintf(stderr, "FATAL xa_flat_chain_prologue: kt=%d (must be a 16-multiple >= 48)\n", kt); abort(); }
    std::string s = x86("comment", "CLASS-C chain prologue (s114): carve kt + park {γ,ω} at [kt-24]/[kt-16] + save caller ___ at [kt-8]; ___ NOT pinned")
         + x86("sub", "rsp", (long)kt)
         + x86("mov", "[rsp + " + std::to_string(kt - 24) + "]", "rcx")
         + x86("mov", "[rsp + " + std::to_string(kt - 16) + "]", "rdx")
         + x86("mov", "[rsp + " + std::to_string(kt - 8) + "]", "rbp")
         + xa_flat_wn_park_str(kt, fname);
    int nf = 0, nsave = 0; int gk[BB_SCC_NP_MAX + 1];
    if (xa_flat_sig_names(fname, &nf, &nsave, gk)) {
        int argkt = 16 * nsave;
        s += x86("sub", "rsp", (long)argkt)
             + x86("mov", "rdx", "[rcx + 0]")
             + x86("lea", "r8", "[rsp + " + std::to_string(kt + argkt) + "]")
             + FOR(0, nsave, [&](int i) {
                   std::string sv = x86("note", gva_note(gk[i]))
                        + x86("mov", "r10", xa_flat_sig_gq(gk[i], 0))
                        + x86("mov", "[rsp + " + std::to_string(16 * i) + "]", "r10")
                        + x86("mov", "r10", xa_flat_sig_gq(gk[i], 8))
                        + x86("mov", "[rsp + " + std::to_string(16 * i + 8) + "]", "r10");
                   if (i >= nf) return sv + x86("mov", xa_flat_sig_gq(gk[i], 0), (long)DT_SNUL)
                                          + x86("mov", xa_flat_sig_gq(gk[i], 8), (long)0);
                   return sv + x86("cmp", "rdx", (long)i) + x86_jcc_id("jbe", 1 + i)
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
                        + x86_deflabel_id(40 + i); });
    }
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_flat_chain_epilogue_str(void) {
    if (!xa_flat_class_c()) return std::string();
    return x86("comment", "CLASS-C chain epilogue (s114): release the α carve; no whack exists on this exit")
         + x86("add", "rsp", (long)g_emit.flat_frame_bytes);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_flat_chain_epilogue_sig_str(int is_gamma, const char * fname) {
    if (!xa_flat_class_c()) return std::string();
    int kt = g_emit.flat_frame_bytes;
    std::string pre;
    { int nf = 0, nsave = 0, res_gk = -1; int gk[BB_SCC_NP_MAX + 1];
      int have = xa_flat_sig_names(fname, &nf, &nsave, gk, &res_gk);
      if (getenv("SCRIP_SIGEPI_DIAG")) fprintf(stderr, "[SIGEPI] fname=%s is_gamma=%d kt=%d sig_names_have=%d nf=%d nsave=%d\n", fname?fname:"(null)", is_gamma, kt, have, nf, nsave);
      if (have) {
          int argkt = 16 * nsave;
          std::string reload = (is_gamma && res_gk >= 0)
              ? ( x86("note", gva_note(res_gk))
                + x86("mov", "rax", xa_flat_sig_gq(res_gk, 0))
                + x86("mov", "rdx", xa_flat_sig_gq(res_gk, 8)))
              : std::string();
          pre = reload
         + FOR(0, nsave, [&](int i) {
                    return x86("note", gva_note(gk[i]))
                         + x86("mov", "r10", "[rsp + " + std::to_string(16 * i) + "]")
                         + x86("mov", xa_flat_sig_gq(gk[i], 0), "r10")
                         + x86("mov", "r10", "[rsp + " + std::to_string(16 * i + 8) + "]")
                         + x86("mov", xa_flat_sig_gq(gk[i], 8), "r10"); })
              + x86("add", "rsp", (long)argkt);
          (void)nf;
      }
    }
    return pre + x86("comment", is_gamma
                   ? "CLASS-C chain epilogue-γ, det-arm signature form (s272 snocone-returns-codegen): the α carve parked the caller's det-arm signature pointer at [kt-24] "
                     "(bcps_det_arm's .Lsig blob: nargs, γ-cont at +8, ω-cont at +16, arg offsets) but this exit used to just release the frame and fall through to the bare/wire "
                     "epilogue, which pops garbage -- reload the pointer, follow it to the γ continuation, THEN release, THEN jmp. rax:rdx carry the typed result -- reloaded from "
                     "res_gk just above (nreturn-after-indirect-assign-wrong-value fix) whenever this graph's signature info is known, otherwise still whatever the last node to "
                     "reach this exit staged. Must NOT clobber rax:rdx -- the call site's own landing tells success from failure by reading al, and only DT_FAIL (0x68) reads as "
                     "failure, so a live result's low byte must survive untouched. ⛔ ONLY valid when this chain was actually entered via bcps_det_arm's jmp-with-signature "
                     "convention (guarded by !g_rt_fragment_emit at the call site) -- EVAL's runtime-compiled fragments reach this SAME class-C prologue but are invoked by a "
                     "normal C call/ret (rt_proc_call_open_det* calling a real function pointer), so for them the plain xa_flat_chain_epilogue (release-only, fall through to ret) "
                     "is the correct and only exit; reading [rsp+kt-24] as a signature pointer for an eval fragment reads whatever garbage sat in rcx at entry and segfaults "
                     "(measured: corpus/crosscheck/rung10/1019_eval_string.sno SIGSEGV before this guard was added)."
                   : "CLASS-C chain epilogue-ω, det-arm signature form (s272 snocone-returns-codegen): same signature reload as epilogue-γ, but the ω continuation lives at "
                     "sig+16, not sig+8 (confirmed against a working DEFINE'd proc's own epilogue-ω: genuinely different offsets, not a symmetric pair) -- and unlike γ this exit "
                     "MUST overwrite rax:rdx with FAILDESCR, because the call site's landing (shared with γ when the two continuations coincide, per bcps_det_arm) tells the two "
                     "apart only by `cmp al, DT_FAIL`. Same eval-fragment caveat as epilogue-γ applies -- see its comment.")
         + xa_flat_wn_restore_str(kt, fname)
         + x86("mov", "rcx", RDQ("rsp", kt - 24))
         + x86("mov", "rcx", RDQ("rcx", is_gamma ? 8 : 16))
         + (is_gamma ? std::string() : (x86("mov32", "eax", (long)DT_FAIL)
                                      + x86("xor", "edx", "edx")))
         + x86("add", "rsp", (long)kt)
         + x86_jmp_reg("rcx");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string xa_icn_trace_tap(const char * pname, int kind, int np, int r11d) {
    extern long g_trace; extern void rt_trace_call_hook_f(const char *, int, void *); extern void rt_trace_return_hook(const char *, DESCR_t); extern void rt_trace_fail_hook(const char *);
    extern void rt_trace_gen_fail_hook(const char *, void *);
    extern int g_flat_node_id;
    if (!x86_trace_hooks_on()) return std::string();
    if (!pname) return std::string();
    pname = xa_icn_trace_intern(pname);
    std::string id = std::to_string(g_flat_node_id++);
    std::string sk = (kind <= 4) ? "L24" + std::to_string(6 + kind) : "L" + std::to_string(235 + kind); std::string fl = ".Licn_trace_nm" + id;
    std::string s = x86("push", "rax")
                  + x86("push", "rdx") + x86_align_call_enter()
        + IF(kind != 1, x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_trace, "g_trace")
        + x86("mov", "rax", RDQ("rax", 0))
        + x86("cmp", "rax", (long)0)
        + x86("je", sk))
        + x86("directive", ".section .rodata")
        + x86("directive", (fl + ": .string \"" + pname + "\"").c_str())
        + x86("directive", ".section .text")
        + x86("directive", ".intel_syntax noprefix")
        + x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)pname, fl.c_str());
    if (kind == 1) s += x86("mov32", "esi", (long)np)
                      + x86("lea", "rdx", RDQ("r11", r11d));
    else if (kind == 2) s += x86("mov", "rsi", RDQ("r11", 8))
                           + x86("mov", "rdx", RDQ("r11", 0));
    else if (kind == 5) s += x86("mov", "rsi", "rbp");
    if (kind == 1) s += x86("call", "rt_trace_call_hook_f", (uint64_t)(uintptr_t)(void *)rt_trace_call_hook_f);
    else if (kind == 2) s += x86("call", "rt_trace_return_hook", (uint64_t)(uintptr_t)(void *)rt_trace_return_hook);
    else if (kind == 5) s += x86("call", "rt_trace_gen_fail_hook", (uint64_t)(uintptr_t)(void *)rt_trace_gen_fail_hook);
    else s += x86("call", "rt_trace_fail_hook", (uint64_t)(uintptr_t)(void *)rt_trace_fail_hook);
    s += x86_rt_gc_poll();
    s += x86("def", sk) + x86_align_call_leave()
       + x86("pop", "rdx")
       + x86("pop", "rax");
    return s;
}
const char * xa_icn_trace_pname(void) {
    return (g_emit_cfg && g_emit_cfg->root_graph) ? "main"
         : !g_emit.flat_fam ? (const char *)0
         : (strncmp(g_emit.flat_fam, "proc_", 5) == 0) ? g_emit.flat_fam + 5
         : g_emit.flat_fam;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_flat_zframe_epilogue_γ_str(void) {
    if (!xa_flat_class_zf()) return std::string();
    int kt = g_emit.flat_frame_bytes; if (g_emit_cfg && g_emit_cfg->icn_cells_graph && g_emit.flat_lcl_proc) kt += (g_emit_cfg->nparams + g_emit_cfg->nlocals) * 16;
    if (g_emit.flat_gen && g_emit_cfg && g_emit_cfg->icn_cells_graph && g_emit.flat_lcl_proc) {
        return x86("comment", "Z-3 γ-RETAIN cells-arm generator: marshal rax:rdx→rdi:rsi; load γ wire; NO unwind (frame survives for β); gen____→rax; jmp γ wire")
             + x86("mov", "rdi", "rax")
             + x86("mov", "rsi", "rdx")
             + x86("mov", "rcx", "qword ptr [rsp# + " + std::to_string(kt - 24) + "]")
             + x86("mov", "rax", "rsp")
             + x86("jmp", "rcx");
    }
    extern int xa_icn_block_size(void);
    if (icn_wire_stack_on() && g_emit_cfg && g_emit_cfg->icn_cells_graph && g_emit.flat_lcl_proc)
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
             + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&kw_fnclevel, "kw_fnclevel")
             + x86("mov", RDQ("rax", 0), "rcx")
             + x86("pop", "rax")
             + (icn_host_pinned() ? x86("lea", "rsp", RDQ("rbp", kt + xa_icn_block_size()))
                                  + x86("mov", "rbp", RDQ("rbp", kt - 8))
                                  : x86("add", "rsp", (long)(kt + xa_icn_block_size())))
             + bb_glue_wire_γ();
    if (zf_pas_nest_graph())
        return x86("mov", "rdi", "rax")
             + x86("mov", "rsi", "rdx")
             + zf_display_restore(kt)
             + x86("add", "rsp", (long)kt + zf_det_block_size())
             + x86("pop", "rcx")
             + x86("add", "rsp", 8L)
             + x86("jmp", "rcx");
    if (x86_fb_pinned() && g_emit.flat_pkt && g_emit.flat_β_p && g_emit.flat_altdet_p && !(g_emit_cfg && g_emit_cfg->root_graph)) {
        return x86("comment",
            "PACKET FRAGMENT gamma (ARCH-PROLOG-C-OUT-OF-THE-BOX 5.2 A): B == this frame's header -> no choice inside the "
                "clause: a later slot visible at G makes the token this frame with beta = the chain-omega, none pops the "
                    "choice and exits det; B younger -> the clause's own beta; B older -> a cut ran, det")
             + IF(xa_flat_graph_holds_a_frame_retry(), x86("comment",
                 "PACKET FRAGMENT gamma: this clause holds a disjunction or a builtin generator, whose choice re-aims B at THIS frame's header -- "
                     "B == header cannot say no choice is left inside, so it takes the clause's own beta, whose exhaustion reaches the chain-omega's walk"))
             + x86("mov32", "edi", (long)DT_I)
             + x86("mov32", "esi", 1L)
             + x86("mov", "rcx", RDQ(x86_fb(), kt - 24))
             + x86("lea", "rax", RDQ(x86_fb(), kt - 64))
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
             + x86("mov", "rax", x86_fb()) + zf_pin_restore(kt)
             + x86("jmp", "rcx")
             + x86("def", L(232))
             + x86("mov", "r10d", RDD("r11", 32))
             + x86("movsxd", "r10", "r10d")
             + x86("jmp", L(230))
             + x86("def", L(238))
             + x86("lea", "rdx", "extlbl", (uint64_t)(uintptr_t)g_emit.flat_β_p)
             + x86("mov", "rax", x86_fb()) + zf_pin_restore(kt)
             + x86("jmp", "rcx")
             + x86("def", L(233))
             + x86("mov", "r13", RDQ(x86_fb(), kt - 40))
             + x86("xor", "eax", "eax") + zf_release(kt) + zf_pin_restore(kt)
             + x86("jmp", "rcx"); }
    if (x86_fb_pinned() && g_emit.flat_β_p && g_emit.flat_altdet_p) {
        int _plretain = !(g_emit_cfg && g_emit_cfg->root_graph);
        return x86("comment", "PL γ: a predicate has no value -- hand the caller the definite success DESCR {DT_I, 1} the return trampolines already hand it, never the last box's leftover rax:rdx")
             + x86("comment",
                 "(a bound integer payload 104 = DT_FAIL read as failure at the caller's al test; a {small int, cell address} pair in the caller's result slot is no DESCR to the collector)")
             + x86("mov32", "edi", (long)DT_I)
             + x86("mov32", "esi", 1L)
             + x86("mov", "rcx", RDQ(x86_fb(), kt - 24))
             + IF(_plretain,
                   x86("mov", "rax", RDQ(x86_fb(), kt - 40))
                 + x86("cmp", "r13", "rax")
                 + x86("je", "extlbl", (uint64_t)(uintptr_t)g_emit.flat_altdet_p)
                 + x86("lea", "rdx", "extlbl", (uint64_t)(uintptr_t)g_emit.flat_β_p)
                 + x86("mov", "rax", x86_fb())
                 + zf_pin_restore(kt)
                 + x86("jmp", "rcx")
                 + x86_def_ext(g_emit.flat_altdet_p))
             + x86("xor", "eax", "eax")
             + zf_release(kt)
             + zf_pin_restore(kt)
             + x86("jmp", "rcx"); }
    return x86("mov", "rdi", "rax")
         + x86("mov", "rsi", "rdx")
         + x86("mov", "rcx", RDQ(x86_fb(), kt - 24))
         + zf_display_restore(kt)
         + zf_release(kt)
         + zf_pin_restore(kt)
         + std::string("")
         + x86("jmp", "rcx");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_flat_zframe_epilogue_ω_str(void) {
    if (!xa_flat_class_zf()) return std::string();
    int kt = g_emit.flat_frame_bytes; if (g_emit_cfg && g_emit_cfg->icn_cells_graph && g_emit.flat_lcl_proc) kt += (g_emit_cfg->nparams + g_emit_cfg->nlocals) * 16;
    extern int xa_icn_block_size(void);
    if (icn_wire_stack_on() && g_emit_cfg && g_emit_cfg->icn_cells_graph && g_emit.flat_lcl_proc)
        return xa_icn_trace_tap(xa_icn_trace_pname(), 3, 0, 16)
             + x86("push", "rax")
             + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&rt_k_level_p, "rt_k_level_p")
             + x86("mov", "rax", RDQ("rax", 0))
             + x86("mov", "ecx", RDD("rax", 0))
             + x86("movsxd", "rcx", "ecx")
             + x86("sub", "rcx", (long)1)
             + x86("mov", RDD("rax", 0), "ecx")
             + x86("sub", "rcx", (long)1)
             + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&kw_fnclevel, "kw_fnclevel")
             + x86("mov", RDQ("rax", 0), "rcx")
             + x86("pop", "rax")
             + (icn_host_pinned() ? x86("lea", "rsp", RDQ("rbp", kt + xa_icn_block_size()))
                                  + x86("mov", "rbp", RDQ("rbp", kt - 8))
                                  : x86("add", "rsp", (long)(kt + xa_icn_block_size())))
             + bb_glue_wire_ω();
    if (zf_pas_nest_graph())
        return x86("comment", "PAS-NEST epilogue-ω: consume the caller-pushed wire pair (discard γ-landing, jmp ω-landing) — twin of PAS-NEST epilogue-γ")
             + zf_display_restore(kt)
             + x86("add", "rsp", (long)kt + zf_det_block_size())
             + x86("add", "rsp", 8L)
             + x86("pop", "rcx")
             + x86("jmp", "rcx");
    if (x86_fb_pinned() && g_emit.flat_pkt) {
        int k = g_emit.flat_pkt_cell, i = g_emit.flat_pkt_slot; int np = g_emit_cfg ? g_emit_cfg->nparams : 0;
        if (!g_emit.flat_pkt_chainω_p || !g_emit.flat_pkt_walk_p) return x86_bomb("packet fragment chain-omega without its labels");
        return x86_def_ext(g_emit.flat_pkt_chainω_p)
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
             + x86("lea", "rsp", RDQ(x86_fb(), kt))
             + x86("mov", x86_fb(), RDQ(x86_fb(), kt - 8))
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
             + x86("mov", "rdi", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_rt_gen_procs, "g_rt_gen_procs")
             + x86("mov", "rdi", RDQ("rdi", 0))
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
    if (x86_fb_pinned())
        return x86("mov", "rcx", RDQ(x86_fb(), kt - 16))
             + x86("mov", "r13", RDQ(x86_fb(), kt - 40))
             + zf_release(kt)
             + zf_pin_restore(kt)
             + x86("jmp", "rcx");
    return x86("comment", "ICN-FR-2 zframe epilogue-ω: load ω wire from [kt-16]; unwind to flat base; jmp. NOTE: no caller-base restore happens here — the [kt-8] slot is "
                          "WRITE-ONLY on every arm that fills it (s247)")
         + x86("mov", "rcx", "qword ptr [rsp# + " + std::to_string(kt - 16) + "]")
         + zf_display_restore(kt)
         + zf_release(kt)
         + zf_pin_restore(kt)
         + std::string("")
         + x86("jmp", "rcx");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_pl_switch_str(int a0_off, const pl_ix_arm_t * arms, int narms, bb_label_t * fb_ref, bb_label_t * fb_atom, bb_label_t * fb_int, bb_label_t * chain) {
    x86_begin();
    auto ext = [](const char * op, bb_label_t * l) { return x86(op, "extlbl", (uint64_t)(uintptr_t)l); };
    auto has = [&](int tag) { for (int i = 0; i < narms; i++) if (arms[i].tag == tag) return 1; return 0; };
    int dref = has(DT_PLREF) || fb_ref != chain, datom = has(DT_PLATOM) || fb_atom != chain, dint = has(DT_I) || fb_int != chain;
    std::string s = x86("comment", "PL SWITCH (ARCH-PROLOG-C-OUT-OF-THE-BOX 11; the design of record's section 7.2, V1): the first argument's key -- a functor id, an atom id or a small integer -- "
                                   "selects the clauses whose first head argument can match it; ONE candidate is entered in last-alternative form (no next alternative, B back to the outer choice), "
                                   "NONE concedes through the step, two or more take the full chain; an unbound, float, bignum or text first argument takes the chain")
                  + x86("lea", "rdi", FRQ(a0_off))
                  + PL_DEREF(10, 11, 12, 13);
    if (dref) s += x86("cmp", "al", (long)DT_PLREF)
                  + x86("je", L(20));
    if (datom) s += x86("cmp", "al", (long)DT_PLATOM)
                  + x86("je", L(30));
    if (dint) s += x86("cmp", "al", (long)DT_I)
                  + x86("je", L(40));
    s += ext("jmp", chain);
    if (dref) {
        s += x86("def", L(20))
           + x86("mov", "ecx", RDD("rdi", 4));
        for (int i = 0; i < narms; i++) if (arms[i].tag == DT_PLREF) s += x86("cmp", "ecx", (long)arms[i].val) + ext("je", arms[i].to);
        s += ext("jmp", fb_ref);
    }
    if (datom) {
        s += x86("def", L(30))
           + x86("mov", "rsi", RDQ("rdi", 8));
        for (int i = 0; i < narms; i++) if (arms[i].tag == DT_PLATOM) s += x86_movabs_r64("rax", (uint64_t)arms[i].val) + x86("cmp", "rsi", "rax") + ext("je", arms[i].to);
        s += ext("jmp", fb_atom);
    }
    if (dint) {
        s += x86("def", L(40))
           + x86("mov", "ecx", RDD("rdi", 4))
           + x86("test", "ecx", "ecx") + ext("jne", chain)
           + x86("mov", "rsi", RDQ("rdi", 8));
        for (int i = 0; i < narms; i++) if (arms[i].tag == DT_I) s += x86_movabs_r64("rax", (uint64_t)arms[i].val) + x86("cmp", "rsi", "rax") + ext("je", arms[i].to);
        s += ext("jmp", fb_int);
    }
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void xa_pl_switch(int a0_off, const pl_ix_arm_t * arms, int narms, bb_label_t * fb_ref, bb_label_t * fb_atom, bb_label_t * fb_int, bb_label_t * chain) {
    bb_emit_x86(xa_pl_switch_str(a0_off, arms, narms, fb_ref, fb_atom, fb_int, chain)); }
extern "C" void xa_flat_zframe_prologue(void) { bb_emit_x86(xa_flat_zframe_prologue_str()); }
extern "C" void xa_flat_chain_prologue(const char * fname) { bb_emit_x86(xa_flat_chain_prologue_str(fname)); }
extern "C" void xa_flat_chain_epilogue(void) { bb_emit_x86(xa_flat_chain_epilogue_str()); }
extern "C" void xa_flat_chain_epilogue_sig(int is_gamma, const char * fname) { bb_emit_x86(xa_flat_chain_epilogue_sig_str(is_gamma, fname)); }
extern "C" void xa_flat_zframe_epilogue_γ(void) { bb_emit_x86(xa_flat_zframe_epilogue_γ_str()); }
extern "C" void xa_flat_zframe_epilogue_ω(void) { bb_emit_x86(xa_flat_zframe_epilogue_ω_str()); }
