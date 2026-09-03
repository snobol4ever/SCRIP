#include <string>
#include <cstdlib>
#include <cstdio>
#include "emit.h"
#include "x86_asm.h"
#include "pin_va.h"
extern "C" {
#include "xa_template_common.h"
#include "../emitter/emit.h"
}
extern "C" void rt_jmp_frame_lexprep(void *, long);
extern "C" void rt_jmp_frame_lexprep2(void *, long, long);
extern "C" void rt_main_args_fetch(void);
extern "C" int rt_proc_nformals(const char *name);
extern "C" int bb_scc_probe(const char *fname, int nargs, int *np_out, int *nsave_out, int *gk_out, int *res_gk_out);
extern int g_rt_fragment_emit;
extern int * const rt_k_level_p;
extern int64_t kw_fnclevel;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_wire_stack_on(void) { static int _v = -1; if (_v < 0) { const char *e = getenv("SCRIP_ICN_WIRE_STACK"); _v = (e && *e == (char)48) ? 0 : 1; } return _v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_entry_dispatch_str(void) {
    if (MEDIUM_MACRO_DEF) return x86("comment", "# no macro form — XA_ENTRY_DISPATCH");
    if (MEDIUM_BINARY)    return std::string();
    if (MEDIUM_TEXT) {
        if (!g_is_text) return std::string();
        return std::string("  cmp esi, 0\n")
             + "  je "  + (g_emit.flat_lbl_α_body ? g_emit.flat_lbl_α_body : "?") + "\n"
             + "  jmp " + (g_emit.flat_lbl_β      ? g_emit.flat_lbl_β      : "?") + "\n";
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_flat_data_section_str(void) {
    if (MEDIUM_MACRO_DEF) return x86("comment", "# no macro form — XA_FLAT_DATA_SECTION");
    if (MEDIUM_BINARY)    return std::string();
    if (MEDIUM_TEXT) {
        if (!g_flat_data_any) return std::string();
        return std::string("  .section .data\n")
             + std::string(g_flat_data_buf, g_flat_data_len)
             + "  .section .text\n";
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void xa_entry_dispatch(void)    { auto s = xa_entry_dispatch_str();    if (!s.empty()) emit_text_n(s.data(), s.size()); }
extern "C" void xa_flat_data_section(void) { auto s = xa_flat_data_section_str(); if (!s.empty()) emit_text_n(s.data(), s.size()); }
extern "C" void rt_pl_dc_prep(void *, long, long, long, long, long);
extern "C" DESCR_t rt_pl_dc_leave_γ(DESCR_t, long, void *);
extern "C" DESCR_t rt_pl_dc_leave_ω(long, void *);
extern "C" void rt_arg_stage(int idx, DESCR_t v);
extern "C" void rt_icn_zframe_args_install(void *, int, int);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_flat_dc_stub_str(void) {
    x86_begin();
    int kt = g_emit.flat_frame_bytes;
    int anchor = -1;
    int suffix = (g_emit.flat_seed_off >= 16) ? g_emit.flat_seed_off : 16;
    extern int g_flat_dc_np;
    int np = g_flat_dc_np;
    uint64_t prep_fp;  { void (*fp)(void *, long, long, long, long, long) = rt_pl_dc_prep; prep_fp = (uint64_t)(uintptr_t)(void *)fp; }
    uint64_t lvg_fp;   { DESCR_t (*fp)(DESCR_t, long, void *) = rt_pl_dc_leave_γ; lvg_fp = (uint64_t)(uintptr_t)(void *)fp; }
    uint64_t lvw_fp;   { DESCR_t (*fp)(long, void *) = rt_pl_dc_leave_ω; lvw_fp = (uint64_t)(uintptr_t)(void *)fp; }
    static const char *argreg[4] = { "rsi", "rdx", "rcx", "r8" };
    if (g_emit.zframe_graph || (g_emit_cfg && g_emit_cfg->icn_cells_graph && g_emit.flat_lcl_proc)) {
        static const char *dcarg4[4] = { "rsi", "rdx", "rcx", "r8" };
        uint64_t stg_fp; { void (*fp)(int, DESCR_t) = rt_arg_stage; stg_fp = (uint64_t)(uintptr_t)(void *)fp; }
        bool need_align_pad = (np > 0) && (np % 2 == 1);
        std::string zs = x86("comment", "ICN-FR-3 zframe dc stub: stage args, jmp proc_f_α≡0 with wire shims")
            + x86("pop", "r12")
            + x86("push", "r12")
            + x86("push", "r12");
        if (need_align_pad) zs += x86("push", "r12");
        for (int i = np - 1; i >= 0; i--) zs += x86("push", dcarg4[i]);
        int push_bytes = np * 8 + (need_align_pad ? 8 : 0);
        for (int i = 0; i < np; i++) {
            zs += x86("mov", "rax", "[rsp + " + std::to_string(i * 8) + "]")
                + x86("mov32", "edi", (long)i)
                + x86("mov", "rsi", "[rax + 0]")
                + x86("mov", "rdx", "[rax + 8]")
                + x86("call", "rt_arg_stage", stg_fp);
        }
        if (push_bytes > 0) zs += x86("add", "rsp", (long)push_bytes);
        if (icn_wire_stack_on() && g_emit_cfg && g_emit_cfg->icn_cells_graph && g_emit.flat_lcl_proc) {
            zs += x86("comment", "N-1(b/c): push {gamma=id2,omega=id3} instead of rcx/rdx, ON TOP of the unconditional double r12 stash above (kept unconditional, not just for OLD, so this arm's stack parity vs the caller of this stub stays a multiple of 16 -- three qwords under the pair would misalign every nested call the callee makes: measured as a real SIGSEGV deep in glibc's snprintf via a nested image() call, s268). Release order at landing: pair(16) + one r12 duplicate(8) = 24, then pop the other (identical) duplicate as the real jump target.")
                + x86_lea_id("rcx", 3) + x86("push", "rcx")
                + x86_lea_id("rcx", 2) + x86("push", "rcx")
                + x86_jmp_lblptr(g_emit.flat_dc_body_p, g_emit.flat_lbl_α ? g_emit.flat_lbl_α : "?")
                + x86_deflabel_id(2)
                + x86("add", "rsp", 24L)
                + x86("pop", "r12")
                + x86("jmp", "r12")
                + x86_deflabel_id(3)
                + x86("add", "rsp", 24L)
                + x86("pop", "r12")
                + x86("mov32", "eax", 104L)
                + x86("xor", "edx", "edx")
                + x86("jmp", "r12");
            return zs;
        }
        zs += x86_lea_id("rcx", 2)
            + x86_lea_id("rdx", 3)
            + x86_jmp_lblptr(g_emit.flat_dc_body_p, g_emit.flat_lbl_α ? g_emit.flat_lbl_α : "?")
            + x86_deflabel_id(2)
            + x86("pop", "r12")
            + x86("pop", "r12")
            + x86("jmp", "r12")
            + x86_deflabel_id(3)
            + x86("pop", "r12")
            + x86("pop", "r12")
            + x86("mov32", "eax", 104L)
            + x86("xor", "edx", "edx")
            + x86("jmp", "r12");
        return zs;
    }
    return x86("comment", "PL-DC direct-call entry: retaddr -> kt-32 pad, wires -> local ret-shims, one prep crossing, shared body")
         + x86("pop", "r12")
         + x86("sub", "rsp", (long)(kt + 16))
         + x86_rsp_store64(kt - 8, "rsp")
         + std::string("")
         + x86("mov", FRQ(kt - 32), "r12")
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
         + x86("call", "rt_pl_dc_prep", prep_fp)
         + x86_jmp_lblptr(g_emit.flat_dc_body_p, g_emit.flat_lbl_α_body ? g_emit.flat_lbl_α_body : "?")
         + x86_deflabel_id(2)
         + x86_rsp_load64("rdx", 0)
         + x86("mov", "rcx", "rsp")
         + x86("add", "rcx", (long)(-kt))
         + x86_rsp_load64("r12", -32)
         + x86_rsp_load64("rsp", -8)
         + x86("add", "rsp", 16L)
         + x86("push", "r12")
         + x86_jmpfn("rt_pl_dc_leave_γ", lvg_fp)
         + x86_deflabel_id(3)
         + x86_rsp_load64("rdi", 0)
         + x86("mov", "rsi", "rsp")
         + x86("add", "rsi", (long)(-kt))
         + x86_rsp_load64("r12", -32)
         + x86_rsp_load64("rsp", -8)
         + x86("add", "rsp", 16L)
         + x86("push", "r12")
         + x86_jmpfn("rt_pl_dc_leave_ω", lvw_fp);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void xa_flat_dc_stub(void) { bb_emit_x86(xa_flat_dc_stub_str()); }
extern "C" void rt_lcl_proc_args_install(void *, int, int);
extern "C" void rt_icn_zframe_args_install(void *, int, int);
extern "C" void rt_arg_stage(int idx, DESCR_t v);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int xa_flat_zanchor_poison(void) { static int on = -1; if (on < 0) { const char * e = getenv("SCRIP_PL_ZANCHOR_POISON"); on = (e && *e == '1') ? 1 : 0; } return on; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zf_display_level(void) {
    if (!g_emit_cfg || g_emit_cfg->icn_cells_graph) return 0;
    int dl = g_emit_cfg->decl_level;
    return (dl >= 1) ? dl : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zf_pas_nest_graph(void) {
    if (!g_emit_cfg || g_emit_cfg->icn_cells_graph) return 0;
    return g_emit_cfg->decl_level >= 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" { int stage2_owner_varslot(const char *, const char *); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zf_display_mem_off(int dl) {
    if (!g_emit_cfg || !g_emit_cfg->l3_ancestor_name) return -1;
    char nm[32]; snprintf(nm, sizeof nm, "__pas_display_%d", dl);
    return stage2_owner_varslot(g_emit_cfg->l3_ancestor_name, nm);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string zf_display_restore(int kt) {
    int dl = zf_display_level();
    if (!dl) return std::string();
    if (dl <= 3) { const char * dr = dl == 1 ? "r13" : dl == 2 ? "r14" : "r15";
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
static std::string zf_release(int kt) {
    if (!x86_fb_pinned()) return x86("add", "rsp", (long)kt);
    return x86("comment", "PZ-4 PL-ZA-2: exact release off the pin, not off wherever rsp happens to be")
         + x86("lea", "rsp", RDQ(x86_fb(), kt));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void rt_pl_quad_seed(void *);
extern "C" void rt_pl_choice_open(void *);
static std::string xa_flat_zframe_prologue_str(void) {
    if (!g_emit.zframe_graph) return std::string();
    int kt = g_emit.flat_frame_bytes;
    if (kt < 48 || (kt & 15)) { fprintf(stderr, "FATAL xa_flat_zframe_prologue: kt=%d (must be 16-mult >= 48)\n", kt); abort(); }
    int np = g_emit_cfg ? g_emit_cfg->nparams : 0;
    int nl = g_emit_cfg ? g_emit_cfg->nlocals : 0;
    uint64_t _seed_fp; { void (*_f)(void *) = rt_pl_quad_seed; _seed_fp = (uint64_t)(uintptr_t)(void *)_f; }
    uint64_t _open_fp; { void (*_f)(void *) = rt_pl_choice_open; _open_fp = (uint64_t)(uintptr_t)(void *)_f; }
    std::string s = x86("comment", "ICN-FR-2 zframe prologue: sub rsp,kt + wire header [kt-24]=γ [kt-16]=ω [kt-8]=caller____ + pin ___=rsp")
         + x86("sub", "rsp", (long)kt)
         + x86("mov", "[rsp + " + std::to_string(kt - 24) + "]", "rcx")
         + x86("mov", "[rsp + " + std::to_string(kt - 16) + "]", "rdx")
         + (x86_fb_pinned()
            ? (x86("comment", "PZ-4 clause (a) PL-ZA-1: [kt-8] STOPS BEING WRITE-ONLY. It held this frame's own base -- a self-anchor nothing ever read (s247, and both epilogue comments below said so). It now holds the CALLER'S base, and the next instruction pins ours. ⛔ THE PIN IS TAKEN AFTER THE CARVE, so the pinned base equals the α-time rsp and every re-homed ζ reference is `[pin + off]` with the SAME off the spine arm used -- a pure rebase, no parity change and NO PUSH. A `push rbp` here would shift every body byte by 8 and turn the two PL-CALL-ALIGN pads from cure into defect.")
             + x86("mov", "[rsp + " + std::to_string(kt - 8) + "]", "rbp")
             + x86("mov", x86_fb(), "rsp")
             + IF(g_emit_cfg && g_emit_cfg->root_graph, x86("lea", "rdi", RDQ("rsp", kt - 64)) + x86("call_bare", "rt_pl_quad_seed", _seed_fp))
             + x86("comment", "F.HI at [H+32] (rung 3, ARCH sec A.1): the LOG THRESHOLD -- a binder holding only B logs a cell at or above [B+32] and skips one below it. Seeded here to this frame's TOP, which is byte-for-byte the rule rung 1 computed as B+64; it is a WORD rather than a constant offset because a choice opened INSIDE an activation (a disjunction, sec B.5) protects less than the whole frame and lowers it.")
             + x86("lea", "rax", RDQ("rsp", kt))
             + x86("mov", RDQ("rsp", kt - 32), "rax")
             + x86("mov", RDQ("rsp", kt - 40), "r13")
             + x86("mov", RDQ("rsp", kt - 48), 0L)
             + x86("mov", RDQ("rsp", kt - 56), 0L)
             + x86("mov", RDQ("rsp", kt - 64), "r12")
             + IF(g_emit_cfg && g_emit_cfg->n_alts > 1 && g_emit.flat_alt1_p,
                   x86("comment", "PL CHOICE OPEN (rung 2, ARCH sec B.3): more than one candidate clause, so this activation IS the youngest choice. F.CUR at [H+8] is seeded with the address of alternative 1 -- Jcon ir_MoveLabel with the temporary promoted into the frame, so the clause step needs no table and no index compare -- and B is set to H through a NAMED rtx helper, never an emitted write of r13.")
                 + x86("lea", "rax", "extlbl", (uint64_t)(uintptr_t)g_emit.flat_alt1_p)
                 + x86("mov", RDQ("rsp", kt - 56), "rax")
                 + x86("lea", "rdi", RDQ("rsp", kt - 64))
                 + x86("call_bare", "rt_pl_choice_open", _open_fp)))
            : (emit_jmp_pin_legacy() ? (xa_flat_zanchor_poison() ? std::string() : x86("mov", "[rsp + " + std::to_string(kt - 8) + "]", "rsp"))
                                + std::string("")
                               : std::string()));
    if (g_emit.flat_lex) {
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
      if (_dl && _dl <= 3) { const char * _dr = _dl == 1 ? "r13" : _dl == 2 ? "r14" : "r15";
          s += x86("comment", "PAS-DISPLAY-1: save caller display[L] into [kt-40]; display[L] = this frame")
             + x86("mov", FRQ(kt - 40), _dr)
             + x86("mov", _dr, "rsp"); }
      else if (_dl > 3) { int _off = zf_display_mem_off(_dl);
          if (_off < 0) { s += x86_bomb("PAS-DISPLAY-N: level>3 ancestor slot unresolved (save)"); }
          else { s += x86("comment", "PAS-DISPLAY-N: save [r15+off] into [kt-40]; [r15+off] = this frame")
                    + x86("mov", "rax", RDQ("r15", _off))
                    + x86("mov", FRQ(kt - 40), "rax")
                    + x86("mov", "rax", "rsp")
                    + x86("mov", RDQ("r15", _off), "rax"); } } }
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int xa_flat_class_zf(void) {
    if (g_emit.zframe_graph) return 1;
    if (g_emit_cfg && g_emit_cfg->icn_cells_graph && g_emit.flat_lcl_proc) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int xa_flat_class_c(void) {
    if (!g_emit.flat_jmp_entry) { if (getenv("SCRIP_FLOOR_DIAG")) fprintf(stderr, "[CLASS-C] nid=%d bail=no_jmp_entry\n", g_emit.nid); return 0; }
    if (g_emit.flat_pat || g_emit.flat_gen || g_emit.flat_lcl_proc || g_emit.zframe_graph || g_emit.flat_stmt_frame) { if (getenv("SCRIP_FLOOR_DIAG")) fprintf(stderr, "[CLASS-C] nid=%d bail=pat/gen/lcl/zframe/stmt pat=%d gen=%d lcl=%d zframe=%d stmt=%d\n", g_emit.nid, g_emit.flat_pat, g_emit.flat_gen, g_emit.flat_lcl_proc, g_emit.zframe_graph, g_emit.flat_stmt_frame); return 0; }
    { extern int g_flat_frame_floor; if (g_flat_frame_floor > 0) { if (getenv("SCRIP_FLOOR_DIAG")) fprintf(stderr, "[CLASS-C] nid=%d bail=floor floor=%d\n", g_emit.nid, g_flat_frame_floor); return 0; } }
    { int _r = (g_emit.flat_frame_bytes >= 48) ? 1 : 0; if (getenv("SCRIP_FLOOR_DIAG")) fprintf(stderr, "[CLASS-C] nid=%d PASS frame_bytes=%d -> %d\n", g_emit.nid, g_emit.flat_frame_bytes, _r); return _r; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_flat_sig_gq(int gk, int w) {
    return g_rtcc_on ? std::string(GVARQ(gk, w)) : std::string(ABSQ(RT_GVA_VA + (unsigned long)gk * 16 + (unsigned long)w));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int xa_flat_sig_names(const char * fname, int * nf_out, int * nsave_out, int * gk_out, int * res_gk_out = 0) {
    if (!fname || !fname[0] || g_rt_fragment_emit) return 0;
    int nf = rt_proc_nformals(fname); if (nf < 0) nf = 0;
    int np = 0, nsave = 0, res_gk = -1;
    int have = bb_scc_probe(fname, nf, &np, &nsave, gk_out, &res_gk);
    if (!have || nsave <= 0 || nsave > 29) return 0;
    *nf_out = nf; *nsave_out = nsave;
    if (res_gk_out) *res_gk_out = res_gk;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_flat_chain_prologue_str(const char * fname) {
    if (!xa_flat_class_c()) return std::string();
    static int _cf = -1; if (_cf < 0) { const char * e = getenv("SCRIP_CHAIN_FRAME"); _cf = (e && *e == '0') ? 0 : 1; }
    if (!_cf) return std::string();
    { static int _d = -1; if (_d < 0) { const char * e = getenv("SCRIP_CHAIN_DIAG"); _d = (e && *e == '1') ? 1 : 0; } if (_d) { extern int bb_emit_pos; fprintf(stderr, "[CHAINFRAME] pos=%d kt=%d text=%d jmp=%d pat=%d\n", bb_emit_pos, g_emit.flat_frame_bytes, g_is_text ? 1 : 0, g_emit.flat_jmp_entry, g_emit.flat_pat); } }
    int kt = g_emit.flat_frame_bytes;
    if (kt & 15) { fprintf(stderr, "FATAL xa_flat_chain_prologue: kt=%d (must be a 16-multiple >= 48)\n", kt); abort(); }
    std::string s = x86("comment", "CLASS-C chain prologue (s114): carve kt + park {γ,ω} at [kt-24]/[kt-16] + save caller ___ at [kt-8]; ___ NOT pinned")
         + x86("sub", "rsp", (long)kt)
         + x86("mov", "[rsp + " + std::to_string(kt - 24) + "]", "rcx")
         + x86("mov", "[rsp + " + std::to_string(kt - 16) + "]", "rdx")
         + x86("mov", "[rsp + " + std::to_string(kt - 8) + "]", "rbp");
    int nf = 0, nsave = 0; int gk[29];
    if (xa_flat_sig_names(fname, &nf, &nsave, gk)) {
        int argkt = 16 * nsave;
        s += x86("comment", "CLASS-C sig-arg marshal-in (s272 snocone-returns-codegen): the caller's .Lsig blob (nargs at +0, per-arg caller-frame offsets from +24 -- rcx above only COPIED the blob pointer, never clobbered it, so it is still the original here) is the only channel carrying argument VALUES; nothing previously read it, so a callee's formal params/locals/own-result-cell (all GVA slots, same addressing classic DEFINE's procs already use correctly) kept whatever a prior activation or the program's static default left there. Own a SEPARATE argkt-byte carve ABOVE the kt carve -- never reinterpret kt's own internal layout, which jcon_value_region may already be using -- purely to stash each touched GVA slot's PRE-CALL value for restore at exit. One-way copy only: read the caller's argument, never write back through it -- unlike bb_define.cpp's SIG shim, CLASS-C's caller-side offsets point into the caller's own still-live zeta storage, not a call-scoped throwaway, so writing through them is not safe to assume. Formal params (index < nf) get the caller's value when the call actually supplied one (nargs>i), else DT_SNUL/blank, matching SNOBOL4's default-to-null-string convention; any name at index >= nf (declared locals, or the function's own result cell when distinct from every param) always starts blank once per activation, mirroring bb_define.cpp's xt-loop. Mirrored at exit by the epilogue-sig's restore, which must run, and fully release argkt, before its own kt-relative sig-pointer reload.")
             + x86("sub", "rsp", (long)argkt)
             + x86("mov", "rdx", "[rcx + 0]")
             + x86("lea", "r8", "[rsp + " + std::to_string(kt + argkt) + "]")
             + FOR(0, nsave, [&](int i) {
                   std::string sv = x86("note", gva_name(gk[i]))
                        + x86("mov", "r10", xa_flat_sig_gq(gk[i], 0)) + x86("mov", "[rsp + " + std::to_string(16 * i) + "]", "r10")
                        + x86("mov", "r10", xa_flat_sig_gq(gk[i], 8)) + x86("mov", "[rsp + " + std::to_string(16 * i + 8) + "]", "r10");
                   if (i >= nf) return sv + x86("mov", xa_flat_sig_gq(gk[i], 0), (long)DT_SNUL) + x86("mov", xa_flat_sig_gq(gk[i], 8), (long)0);
                   return sv + x86("cmp", "rdx", (long)i) + x86_jcc_id("jbe", 1 + i)
                        + x86("mov", "r10", "[rcx + " + std::to_string(24 + 8 * i) + "]") + x86("add", "r10", "r8")
                        + x86("mov", "r11", "[r10 + 0]") + x86("mov", xa_flat_sig_gq(gk[i], 0), "r11")
                        + x86("mov", "r11", "[r10 + 8]") + x86("mov", xa_flat_sig_gq(gk[i], 8), "r11")
                        + x86_jmp_id(40 + i)
                        + x86_deflabel_id(1 + i)
                        + x86("mov", xa_flat_sig_gq(gk[i], 0), (long)DT_SNUL) + x86("mov", xa_flat_sig_gq(gk[i], 8), (long)0)
                        + x86_deflabel_id(40 + i); });
    }
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_flat_chain_epilogue_str(void) {
    if (!xa_flat_class_c()) return std::string();
    static int _cf = -1; if (_cf < 0) { const char * e = getenv("SCRIP_CHAIN_FRAME"); _cf = (e && *e == '0') ? 0 : 1; }
    if (!_cf) return std::string();
    return x86("comment", "CLASS-C chain epilogue (s114): release the α carve; no whack exists on this exit")
         + x86("add", "rsp", (long)g_emit.flat_frame_bytes);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_flat_chain_epilogue_sig_str(int is_gamma, const char * fname) {
    if (!xa_flat_class_c()) return std::string();
    static int _cf = -1; if (_cf < 0) { const char * e = getenv("SCRIP_CHAIN_FRAME"); _cf = (e && *e == '0') ? 0 : 1; }
    if (!_cf) return std::string();
    int kt = g_emit.flat_frame_bytes;
    std::string pre;
    { int nf = 0, nsave = 0, res_gk = -1; int gk[29];
      if (xa_flat_sig_names(fname, &nf, &nsave, gk, &res_gk)) {
          int argkt = 16 * nsave;
          std::string reload = (is_gamma && res_gk >= 0)
              ? (x86("comment", "nreturn-after-indirect-assign-wrong-value fix (2026-08-27): capture the live result BEFORE the marshal-out restore below overwrites res_gk's own GVA cell with its pre-call value. Cannot rely on rax:rdx already holding it -- that was only ever true by accident, when the return-slot assignment happens to be the last value-producing statement the body ran. Any later statement breaks it: e.g. beauty/match.inc's own `assign(name, expression)` idiom runs an indirect `$name = expression` AFTER `Fnc = .dummy` and BEFORE nreturn, leaving ITS value in rax:rdx instead, which the caller then received unchanged. Reading res_gk fresh here is correct on every gamma exit alike -- explicit `return`, `nreturn`, or an implicit fall-off-the-end success all mean the same thing, 'hand over whatever is in my own result cell' -- so no NRETURN-specific case is needed. Omega is untouched: freturn overrides any pending value unconditionally, so that arm keeps setting FAILDESCR outright.")
                 + x86("note", gva_name(res_gk)) + x86("mov", "rax", xa_flat_sig_gq(res_gk, 0)) + x86("mov", "rdx", xa_flat_sig_gq(res_gk, 8)))
              : std::string();
          pre = reload
              + x86("comment", "CLASS-C sig-arg marshal-out (s272 snocone-returns-codegen): mirror image of the prologue's marshal-in -- restore every touched GVA slot's pre-call value from this activation's own argkt save area BEFORE releasing it, so an outer activation (recursion, or the same name used as an ordinary variable elsewhere) sees its own value again, not this call's leftovers. Must run, and fully unwind (add rsp,argkt), before the kt-relative sig-pointer reload directly below -- that reload's [kt-24] offset is only correct once this carve is gone. rcx is about to be reloaded fresh from [kt-24]; rax:rdx (the live result, staged above on gamma) are never touched here -- this loop only ever writes gk[i]'s backing GVA memory, never those registers.")
              + FOR(0, nsave, [&](int i) {
                    return x86("note", gva_name(gk[i]))
                         + x86("mov", "r10", "[rsp + " + std::to_string(16 * i) + "]") + x86("mov", xa_flat_sig_gq(gk[i], 0), "r10")
                         + x86("mov", "r10", "[rsp + " + std::to_string(16 * i + 8) + "]") + x86("mov", xa_flat_sig_gq(gk[i], 8), "r10"); })
              + x86("add", "rsp", (long)argkt);
          (void)nf;
      }
    }
    return pre + x86("comment", is_gamma
                   ? "CLASS-C chain epilogue-γ, det-arm signature form (s272 snocone-returns-codegen): the α carve parked the caller's det-arm signature pointer at [kt-24] (bcps_det_arm's .Lsig blob: nargs, γ-cont at +8, ω-cont at +16, arg offsets) but this exit used to just release the frame and fall through to the bare/wire epilogue, which pops garbage -- reload the pointer, follow it to the γ continuation, THEN release, THEN jmp. rax:rdx carry the typed result -- reloaded from res_gk just above (nreturn-after-indirect-assign-wrong-value fix) whenever this graph's signature info is known, otherwise still whatever the last node to reach this exit staged. Must NOT clobber rax:rdx -- the call site's own landing tells success from failure by reading al, and only DT_FAIL (0x68) reads as failure, so a live result's low byte must survive untouched. ⛔ ONLY valid when this chain was actually entered via bcps_det_arm's jmp-with-signature convention (guarded by !g_rt_fragment_emit at the call site) -- EVAL's runtime-compiled fragments reach this SAME class-C prologue but are invoked by a normal C call/ret (rt_proc_call_open_det* calling a real function pointer), so for them the plain xa_flat_chain_epilogue (release-only, fall through to ret) is the correct and only exit; reading [rsp+kt-24] as a signature pointer for an eval fragment reads whatever garbage sat in rcx at entry and segfaults (measured: corpus/crosscheck/rung10/1019_eval_string.sno SIGSEGV before this guard was added)."
                   : "CLASS-C chain epilogue-ω, det-arm signature form (s272 snocone-returns-codegen): same signature reload as epilogue-γ, but the ω continuation lives at sig+16, not sig+8 (confirmed against a working DEFINE'd proc's own epilogue-ω: genuinely different offsets, not a symmetric pair) -- and unlike γ this exit MUST overwrite rax:rdx with FAILDESCR, because the call site's landing (shared with γ when the two continuations coincide, per bcps_det_arm) tells the two apart only by `cmp al, DT_FAIL`. Same eval-fragment caveat as epilogue-γ applies -- see its comment.")
         + x86("mov", "rcx", RDQ("rsp", kt - 24))
         + x86("mov", "rcx", RDQ("rcx", is_gamma ? 8 : 16))
         + (is_gamma ? std::string() : (x86("mov32", "eax", (long)DT_FAIL) + x86("xor", "edx", "edx")))
         + x86("add", "rsp", (long)kt)
         + x86_jmp_reg("rcx");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_flat_zframe_epilogue_γ_str(void) {
    if (!xa_flat_class_zf()) return std::string();
    int kt = g_emit.flat_frame_bytes; if (g_emit_cfg && g_emit_cfg->icn_cells_graph && g_emit.flat_lcl_proc) kt += (g_emit_cfg->nparams + g_emit_cfg->nlocals) * 16; { kt += icn_gen_host_reserve((const char *)0); }
    if (g_emit.flat_gen && g_emit_cfg && g_emit_cfg->icn_cells_graph && g_emit.flat_lcl_proc) {
        return x86("comment", "Z-3 γ-RETAIN cells-arm generator: marshal rax:rdx→rdi:rsi; load γ wire; NO unwind (frame survives for β); gen____→rax; jmp γ wire")
             + x86("mov", "rdi", "rax")
             + x86("mov", "rsi", "rdx")
             + x86("mov", "rcx", "qword ptr [rsp# + " + std::to_string(kt - 24) + "]")
             + x86("mov", "rax", "rsp")
             + x86("jmp", "rcx");
    }
    if (icn_wire_stack_on() && g_emit_cfg && g_emit_cfg->icn_cells_graph && g_emit.flat_lcl_proc)
        return x86("comment", "N-1(b/c) ICN-FR-2 epilogue-γ: marshal result rax:rdx→rdi:rsi; unwind; NON-CONSUMING jmp through the caller-pushed gamma wire at [rsp+0] -- the caller's own landing releases the pair (bcps_wire_land), never this exit. Guarded to the Icon (icn_cells_graph) case only -- xa_flat_class_zf() also admits pure zframe_graph (Prolog/Raku/Pascal), whose callee side this rung never touched. SCRIP_ICN_WIRE_STACK=0 restores the [kt-24] header byte-exactly.")
             + x86("mov", "rdi", "rax")
             + x86("mov", "rsi", "rdx")
             + x86("comment", "&level HALF-CURE (seat01, row icon-rung-ladder-absorption): decrement rt_k_level/kw_fnclevel on this class's own exit, mirroring bb_define_activate's leave_env pair verbatim (bb_define.cpp:185-193). rax is SAVED/RESTORED around this block -- confirmed via asm-diff + direct trace (fact_dcα's own success landing does `jmp r12` with NO fresh success tag, so whatever this exit leaves in AL is exactly what the caller's `cmp al,104` check reads; clobbering it with an unrelated GOT address broke every non-trivial call site, caught by the Icon rung board before landing, never assume a register is free just because no comment claims it). Entry-side increment still NOT YET LANDED -- see FINDING-2026-08-30-seat01-icon-level-exact-fix-sites-located-implementation-ready.md.")
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
             + x86("add", "rsp", (long)kt)
             + bb_glue_wire_γ();
    if (zf_pas_nest_graph())
        return x86("comment", "PAS-NEST epilogue-γ: bcps callers PUSH the wire pair and assume the callee consumes it (bcps_wire_pair_consumed=1); the [kt-24] arm left the pair seated and skewed every frame-relative caller access by 16 — the whole nest* rc=139 class. Consume: pop γ-landing, discard ω, jmp")
             + x86("mov", "rdi", "rax")
             + x86("mov", "rsi", "rdx")
             + zf_display_restore(kt)
             + x86("add", "rsp", (long)kt)
             + x86("pop", "rcx")
             + x86("add", "rsp", 8L)
             + x86("jmp", "rcx");
    if (x86_fb_pinned() && g_emit.flat_β_p && g_emit.flat_altdet_p) {
        int _plretain = !(g_emit_cfg && g_emit_cfg->root_graph);
        return x86("comment", "PL epilogue-γ (rung 2, ARCH sec A.1 + sec B.3 + PZ-4 clause (f)): γ hands r13 through untouched. If r13 still equals F.B0 at [H+24] this activation left no live choice -- release exactly off the pin and hand rax = 0. Otherwise a choice younger than this frame is LIVE and carved BELOW us, so releasing would let the caller carve straight over a live choice point: KEEP the frame, hand rax = our own base as the caller re-entry token and rdx = our graph β as its resume address, and restore only the caller pin; the caller landing reads rax to decide whether to pop its wire pair. THE ROOT GRAPH NEVER RETAINS -- it is entered from the driver, whose wires call exit, so no landing exists to consume the token. ⛔ Every read here is through the PIN, never through rsp: the ICN-FR-2 arm below spells its own wire [rsp# + kt-24], which equals the pin only while nothing is retained -- under γ-retain rsp sits below a live callee frame and that spelling loads garbage into the wire register.")
             + x86("mov", "rdi", "rax")
             + x86("mov", "rsi", "rdx")
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
    return x86("comment", "ICN-FR-2 zframe epilogue-γ: marshal result rax:rdx→rdi:rsi; load γ wire from [kt-24] THROUGH THE PIN (never bare rsp -- x86_fb() is rbp for a zframe_graph=1 pinned caller, rsp otherwise, matching the PL arm's identical read at :429 and the class-wide ONE-SPELLING rule); unwind; jmp. NOTE: no caller-base restore happens here — the [kt-8] slot is WRITE-ONLY on every arm that fills it (s247)")
         + x86("mov", "rdi", "rax")
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
    int kt = g_emit.flat_frame_bytes; if (g_emit_cfg && g_emit_cfg->icn_cells_graph && g_emit.flat_lcl_proc) kt += (g_emit_cfg->nparams + g_emit_cfg->nlocals) * 16; { kt += icn_gen_host_reserve((const char *)0); }
    if (icn_wire_stack_on() && g_emit_cfg && g_emit_cfg->icn_cells_graph && g_emit.flat_lcl_proc)
        return x86("comment", "N-1(b/c) ICN-FR-2 epilogue-ω: unwind; NON-CONSUMING jmp through the caller-pushed omega wire at [rsp+8] -- the caller's own landing releases the pair. Guarded to the Icon (icn_cells_graph) case only, same reasoning as epilogue-γ. SCRIP_ICN_WIRE_STACK=0 restores the [kt-16] header byte-exactly.")
             + x86("comment", "&level HALF-CURE (seat01, row icon-rung-ladder-absorption): same decrement as epilogue-γ, twin arm -- see that comment for the full rationale, the confirmed register-preservation bug this rax save/restore fixes, and what is still owed (the entry-side increment).")
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
             + x86("add", "rsp", (long)kt)
             + bb_glue_wire_ω();
    if (zf_pas_nest_graph())
        return x86("comment", "PAS-NEST epilogue-ω: consume the caller-pushed wire pair (discard γ-landing, jmp ω-landing) — twin of PAS-NEST epilogue-γ")
             + zf_display_restore(kt)
             + x86("add", "rsp", (long)kt)
             + x86("add", "rsp", 8L)
             + x86("pop", "rcx")
             + x86("jmp", "rcx");
    if (x86_fb_pinned())
        return x86("comment", "PL epilogue-ω (rung 2, ARCH sec A.1): conceding kills every choice this activation opened, so B reverts to F.B0 at [H+24] on the wire and the frame is released exactly off the pin. This is the one r13 write in emitted code and it is the enrolled B-restore shape, not a scratch use.")
             + x86("mov", "rcx", RDQ(x86_fb(), kt - 16))
             + x86("mov", "r13", RDQ(x86_fb(), kt - 40))
             + zf_release(kt)
             + zf_pin_restore(kt)
             + x86("jmp", "rcx");
    return x86("comment", "ICN-FR-2 zframe epilogue-ω: load ω wire from [kt-16]; unwind to flat base; jmp. NOTE: no caller-base restore happens here — the [kt-8] slot is WRITE-ONLY on every arm that fills it (s247)")
         + x86("mov", "rcx", "qword ptr [rsp# + " + std::to_string(kt - 16) + "]")
         + zf_display_restore(kt)
         + zf_release(kt)
         + zf_pin_restore(kt)
         + std::string("")
         + x86("jmp", "rcx");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void xa_flat_zframe_prologue(void) { bb_emit_x86(xa_flat_zframe_prologue_str()); }
extern "C" void xa_flat_chain_prologue(const char * fname) { bb_emit_x86(xa_flat_chain_prologue_str(fname)); }
extern "C" int xa_flat_class_c_pred(void) { return xa_flat_class_c(); }
extern "C" void xa_flat_chain_epilogue(void) { bb_emit_x86(xa_flat_chain_epilogue_str()); }
extern "C" void xa_flat_chain_epilogue_sig(int is_gamma, const char * fname) { bb_emit_x86(xa_flat_chain_epilogue_sig_str(is_gamma, fname)); }
extern "C" void xa_flat_zframe_epilogue_γ(void) { bb_emit_x86(xa_flat_zframe_epilogue_γ_str()); }
extern "C" void xa_flat_zframe_epilogue_ω(void) { bb_emit_x86(xa_flat_zframe_epilogue_ω_str()); }
