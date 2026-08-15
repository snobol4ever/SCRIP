#include <string>
#include <cstdlib>
#include "emit.h"
#include "x86_asm.h"
extern "C" {
#include "xa_template_common.h"
#include "../emitter/emit.h"
}
#define ADDR_SIGMA   ((uint64_t)(uintptr_t)&Σ)
extern "C" void rt_jmp_frame_lexprep(void *, long);   /* NCB-1d: det-lexical jmp-entry prologue tail — NULVCL fill + rt_frame_bind_args on the self-allocated frame (rt.c) */
extern "C" void rt_jmp_frame_lexprep2(void *, long, long);   /* PL-REGAIN-4: lazy-seed tail — NULVCL slot0 + [suffix,region) only, box grants first-write-wins, SCRIP_ZLS_POISON=1 poison lane inside (rt.c) */
extern "C" void rt_pl_zf_resume_clear(void);   /* PL-FR-4: clear g_pl_zf_pending_cursor after epilogue intercept */
extern "C" void *g_pl_zf_pending_cursor;       /* PL-FR-4: pending resume cursor (0 = none) */
extern "C" void rt_main_args_fetch(void);   /* ICNBENCH-ARGS-RSP: returns the staged main(args) DESCR in rax:rdx (true return type is DESCR_t; declared void here — the template only takes its ADDRESS, the emitted call site honors the real ABI) */
extern "C" void rt_gen_save_wires(void *gen_fb, void *gw, void *ww);      /* ICN-FR-5: save γ/ω wires keyed by gen____ */
extern "C" void *rt_gen_get_gamma_wire(void *gen_fb);                     /* ICN-FR-5: retrieve saved γ-wire for gen____ */
extern "C" void *rt_gen_get_omega_wire(void *gen_fb);                     /* ICN-FR-5: retrieve saved ω-wire for gen____ */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_entry_dispatch_str(void) {
    if (PLATFORM_X86) {
        if (MEDIUM_MACRO_DEF) return x86("comment", "# no macro form — XA_ENTRY_DISPATCH");
        if (MEDIUM_BINARY)    return std::string();
        if (MEDIUM_TEXT) {
            if (!g_is_text) return std::string();
            return std::string("  cmp esi, 0\n")
                 + "  je "  + (g_emit.flat_lbl_α_body ? g_emit.flat_lbl_α_body : "?") + "\n"
                 + "  jmp " + (g_emit.flat_lbl_β      ? g_emit.flat_lbl_β      : "?") + "\n";
        }
    }
    return std::string();
}
#define BB_BANNER_RULE_LEN 119
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xaf_seprule(char ch) { static int _sep = -1; if (_sep < 0) { const char *e = getenv("SCRIP_ASM_SEP"); _sep = (e && *e == '0') ? 0 : 1; } return _sep ? std::string("#") + std::string(BB_BANNER_RULE_LEN, ch) + "\n" : std::string(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xaf_ω_label(void) { return (g_emit.flat_fail_p && g_emit.flat_fail_p->name) ? xaf_seprule('-') + g_emit.flat_fail_p->name + ":\n" : std::string(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_wired_base(void) {
    std::string s(g_emit.flat_lbl_α ? g_emit.flat_lbl_α : "");
    return s.size() > 3 ? s.substr(0, s.size() - 3) : s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void xa_emit_one(const std::string & out, int site, bb_label_t * lbl, bool is_def) {
    if (site < 0) { bb_emit_x86(out); return; }   /* XA-FLAT-CONVERT sentinel: site<0 means the arm already returned an in-band record stream (L/J/D/E/F/X tags) instead of raw spliced bytes, so it routes to the ordinary bb_emit_x86 tag walker every other template uses.  External-label patching that this file used to do by hand-counted offset is carried by the 'X' record (x86_jmp_ext/x86_jcc_ext).  Lets each arm convert independently; when the last one flips, this branch and the raw splicer below both die. */
    if (!MEDIUM_BINARY) { if (!out.empty()) emit_text_n(out.data(), out.size()); return; }
    int pos = 0;
    if (lbl) {
        for (; pos < site; pos++) bb_emit_byte((uint8_t)(unsigned char)out[pos]);
        if (is_def) { bb_label_define(lbl); }
        else        { bb_emit_patch_rel32(lbl); pos += 4; }
    }
    for (; pos < (int)out.size(); pos++) bb_emit_byte((uint8_t)(unsigned char)out[pos]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Frame-register byte fragments (2026-07-08 session 2, ZC_FRAME switch).  These five helpers are the ONLY
 * frame-dependent machine bytes in this file; everything routes through x86_zr_num() so the r12 default is
 * byte-identical to the former hard-coded \x41\x54 / \x49\x89\xFC / \x4D\x89\xE5-family / \x41\x5C /
 * \x49\xC7\x84\x24 forms, and ZC_FRAME____ gets 0x55 / 48 89 FD / 49 89 ED-family / 0x5D / 48 C7 85 (shorter:
 * no REX.B on push/pop, no SIB on the store).  Patch-site offsets below are COMPUTED from the built string's
 * actual length, never hand-counted — hand counts break the moment fragment sizes become frame-dependent
 * (and hand-counting bytes is this repo's named bb_bin_t-era debt anyway). */
static std::string xaf_push_frame(void)  { int z = x86_zr_num(); std::string c; if (z >= 8) c += (char)0x41; c += (char)(0x50 | (z & 7)); return c; }
static std::string xaf_pop_frame(void)   { int z = x86_zr_num(); std::string c; if (z >= 8) c += (char)0x41; c += (char)(0x58 | (z & 7)); return c; }
/* ZS-2 jmp-entry fragments (Lon s58, FINDING-2026-07-14 §5-CORRECTED).  A jmp-entered blob is a NEW ACTIVATION
 * of a BB: it self-allocates on rsp (sub rsp,K_total), and its first 32 bytes are the WIRE HEADER at fixed,
 * universally-known offsets — [rsp+0] β-resume wire, [rsp+8] outside-γ wire, [rsp+16] outside-ω wire,
 * [rsp+24] saved frame reg — so the OUTSIDE backtrack edge is exactly `jmp qword [rsp+0]` with zero per-blob
 * knowledge (all ζ on rsp is pre-determined length; the offset is a simple calculation).  The zls value region
 * sits above the header at [rsp+32, rsp+K_total), and the frame register points AT the region (zr = rsp+32) so
 * every FR()/FRQ() offset in every box is byte-identical to the call-regime layout.  γ SUSPENDS (frame + wires
 * retained, jmp through the γ wire); ω UNWINDS (add rsp,K_total, jmp through the ω wire); the β landing stub
 * (flat_res_p, defined by codegen_flat_chain_body just before lbl_β) re-pins zr = rsp+32 and falls into the
 * chain's existing resume dispatch.  All fragments route through x86_zr_num() — no literal frame-register
 * bytes, same ZC_FRAME discipline as the five helpers above. */
static std::string xaf_ld64_from_zr8(int dst, int disp8) {   /* mov dst, [zr+disp8] (disp8 may be negative — the wire header lives below the region base) */
    int z = x86_zr_num(); std::string c; uint8_t rex = 0x48; if (dst >= 8) rex |= 0x04; if (z >= 8) rex |= 0x01; c += (char)rex; c += (char)0x8B;
    int lo = z & 7; c += (char)(0x40 | ((dst & 7) << 3) | (lo == 4 ? 4 : lo)); if (lo == 4) c += (char)0x24; c += (char)(disp8 & 0xFF); return c;
}
static std::string xaf_lea_rsp_zr32(int disp32) {   /* lea rsp, [zr+disp32] — the ω-half's ABSOLUTE unwind: correct from ANY rsp depth (fence/ABORT seal cuts reach lbl_ω without the popping cascade) */
    int z = x86_zr_num(); std::string c; uint8_t rex = 0x48; if (z >= 8) rex |= 0x01; c += (char)rex; c += (char)0x8D;
    int lo = z & 7; c += (char)(0x80 | (4 << 3) | (lo == 4 ? 4 : lo)); if (lo == 4) c += (char)0x24; c += u32le((uint32_t)disp32); return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xaf_frame_store_imm32(uint32_t disp, uint32_t imm) {
    int z = x86_zr_num(), lo = z & 7; std::string c;
    c += (char)(0x48 | (z >= 8 ? 0x01 : 0x00)); c += (char)0xC7;
    c += (char)(0x80 | (lo == 4 ? 4 : lo)); if (lo == 4) c += (char)0x24;
    c += u32le(disp); c += u32le(imm); return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* C-STACK ANCHOR fragments (ZC_PORT_CSTACK rung, Lon 2026-07-09).  Under CSTACK the ζ cursor IS rsp, and a
 * flat graph's γ/ω landing can arrive with rsp anywhere at-or-below chain start (a blob's granted construct
 * that γ-succeeds hands its live frame down and RETURNS — the 075 crash: pop/ret at a bumped rsp, r12=0,
 * pc=0x10000).  Cross-unit reclamation is impossible on the C stack, so every C-callable emitted unit must
 * restore its entry rsp at its own ret: the prologue pushes the align-save register, snapshots rsp into it,
 * and drops 8 more (net 16 — call-site alignment parity preserved); the epilogue restores rsp from it FIRST,
 * then the existing pops run at their expected positions.  INNERMOST wrapper: placed after every existing
 * prologue push, restored before every existing epilogue pop.  The per-dance push/pop of the same register
 * preserves the anchor value across every alignment window, including the CSTACK release arm's
 * leave/re-enter (x86_asm.h).  Frame-active arms only: grants and the statement backstop are FRQ-keyed, so
 * rsp never moves in a frame-less graph.  Mode read inside the fragment helper follows this file's own
 * ZC_FRAME precedent (the five xaf_ helpers above). */
/* ALIGN-INV-2 / ___-FREE (Lon directive 2026-07-13: "I'd rather waste 8 bytes to gain a register"; ___ -> the
 * dcap cursor).  The graph-level anchor no longer lives in a REGISTER: it lives in the ZETA FRAME, at the pad
 * qword the graph-scope zeta_mark slot has always reserved and never used (zeta_storage.c: "zeta_mark.pad
 * (unused)").  Legal because the frame base (r12 = fb) is loaded per ACTIVATION from rdi in this very
 * prologue, so [fb + anchor] is per-activation too => recursion-safe and depth-independent, which is exactly
 * what the old push-___/mov-___,rsp pair bought and the only reason a register was used at all.  Frame growth
 * ZERO (the pad already existed).  Stack-parity IDENTICAL: the old pair moved rsp by -16 (push 8 + sub 8), the
 * new store moves it by 0, and 16 == 0 (mod 16), so every downstream alignment claim survives untouched.
 * Restore point is unchanged (rsp as of end-of-prologue-pushes), so the epilogue's existing dpop/pop_frame run
 * at exactly the positions they already expected.  NO FALLBACK, and that is a MEASURED claim not a hopeful
 * one: an instrumented build counted zeta_mark_slot<0 on frame-active graphs across 627 programs of all five
 * frontends (307 snobol4 crosscheck + icon/prolog/raku/pascal) and got ZERO, which is what zls_build's
 * unconditional assignment already implied -- so the ___ arms were deleted outright and a slotless
 * frame-active graph is now a LOUD FATAL (a compiler bug, not a codegen mode).  x86_align_save() died with
 * them: emitted code's only remaining ___ uses are the four TRANSIENT push/pop anchor windows, which is what
 * makes ___ available to dcap. */
static int xaf_anchor_off(void) {
    if (!g_emit_cfg || g_emit_cfg->zeta_mark_slot < 0) { fprintf(stderr, "FATAL xa_flat: frame-active graph with no zeta_mark slot -- the graph-scope anchor has no home (zls_build did not run?)\n"); abort(); }
    return g_emit_cfg->zeta_mark_slot + 8;
}
static int xaf_outer_frame_k(void) {
    if (g_emit.flat_all_zd && !g_emit.flat_jmp_entry && !g_emit.flat_pat && !g_emit.flat_gen && x86_zc_frame() == ZC_FRAME_RSP) return 8;   /* LP-1 LAYOUT PASS (s22c NEXT(1)): every chain node self-allocates; the whole-graph region is unused -- return the alignment-only 8B pad, dropping the 240+ byte legacy carve.  Guard: jmp-entry blobs, pat/gen blobs keep legacy (their protocols read the frame slots); non-RSP builds are not on this ladder.  THE ORACLE: arithmetic.sno was `sub rsp, 248` with max offset 40; with this flag set it becomes `sub rsp, 8` -- a correct layout authority that licenses a 0-byte region (the carve-erad prerequisite the LIVE CURSOR named as NEXT(1)). */
    int rg = g_emit_cfg ? g_emit_cfg->jcon_value_region : 0;
    return ((rg + 15) & ~15) + 8;
}
static std::string xaf_frame_rsp_rm(uint32_t disp, unsigned char op) {
    int z = x86_zr_num(), lo = z & 7; std::string c;
    c += (char)(0x48 | (z >= 8 ? 0x01 : 0x00)); c += (char)op;
    c += (char)(0x80 | (4 << 3) | (lo == 4 ? 4 : lo)); if (lo == 4) c += (char)0x24;
    c += u32le(disp); return c;
}
static std::string xaf_ld64_rsp(int reg, int disp) {   /* FLATDISP-7 (s194): mov r64, [rsp+disp] — as-matching mod selection (no-disp / disp8 / disp32), SIB base=rsp.  Serves the depth-static determinate epilogue's wire/result reads, where rsp == base by LIFO balance. */
    std::string c; c += (char)(0x48 | (reg >= 8 ? 0x04 : 0x00)); c += (char)0x8B; int lo = reg & 7;
    if (disp == 0)                        { c += (char)(0x00 | (lo << 3) | 4); c += (char)0x24; }
    else if (disp >= -128 && disp <= 127) { c += (char)(0x40 | (lo << 3) | 4); c += (char)0x24; c += (char)(unsigned char)disp; }
    else                                  { c += (char)(0x80 | (lo << 3) | 4); c += (char)0x24; c += u32le((uint32_t)disp); }
    return c;
}
static std::string xaf_addq_rsp(int imm) {   /* FLATDISP-7 (s194): add rsp, imm — as-matching imm8/imm32 short form (the xaf_add_rdi_imm_bin discipline applied to rsp).  The depth-static determinate epilogue's ENTIRE frame teardown. */
    if (imm >= -128 && imm <= 127) { std::string c = bytes(3, "\x48\x83\xC4"); c += (char)(unsigned char)imm; return c; }
    return bytes(3, "\x48\x81\xC4") + u32le((uint32_t)imm);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int xaf_shed_np(void) { static int on = -1; if (on < 0) { const char * e = getenv("SCRIP_SHED_NP"); on = (e && *e == '0') ? 0 : 1; } return on; }   /* SHED-1 killswitch: =0 restores pre-SHED-1 nparams conjunct byte-for-byte. */
static int xaf_deep(void) { return g_emit.flat_deep_arrival || (!xaf_shed_np() && g_emit.flat_outer_nparams >= 1); }   /* FLATDISP-5b/5c (s193): THE one outer-frame ___ condition.  SHED-1 (OMEGA s27 session): nparams conjunct retired under default SCRIP_SHED_NP=1.  Every graph with nparams>=1 is a DEFINE proc entered via jmp-wire (flat_jmp_entry=1 implies flat_deep_arrival=1); the conjunct is therefore redundant.  Measured: 318/318 byte-identical SCRIP_SHED_NP=1 vs =0.  ICNBENCH-ARGS note: the [___+16]/[___+24] param stores in xa_flat prologue are only reached when flat_jmp_entry=1 (proc wire entry), which implies deep_arrival=1 already — so the seed is established by that path regardless.  Killswitch =0 if a depth-static nparams>=1 graph ever appears (not in current corpus). */
static std::string xaf_anchor_leave_bin(void) {
    if (!x86_port_cstack()) return std::string();
    if (x86_zc_frame() == ZC_FRAME_RSP && !xaf_deep()) return std::string();   /* FLATDISP-5c (s193): BINARY twin of the TEXT gate below — the bulk unwind reads the ___ seed, so a depth-static graph (which never seeded) must not emit it; LIFO balance has rsp at base already.  This line is WHY m3 held at 185 while m4 broke in s192: the TEXT arm was gated without this twin (BOTH-MEDIUM violation, recorded not hidden). */
    if (x86_zc_frame() == ZC_FRAME_RSP) return bytes(3, "\x48\x89\xEC");   /* mov rsp, ___ — ALIGN-INV-3 twin of the TEXT arm (as-form 48 89 EC); the s90 rsp-relative-read lesson lives in the TEXT arm comment */
    return xaf_frame_rsp_rm((uint32_t)xaf_anchor_off(), 0x8B);
}
static std::string xaf_anchor_leave_text(void) {
    if (!x86_port_cstack()) return std::string();
    if (x86_zc_frame() == ZC_FRAME_RSP && !xaf_deep()) return std::string();   /* FLATDISP-5 GATE (s193: condition unified into xaf_deep, see its comment): this reset is the BULK unwind that returns rsp to the activation base from whatever depth the arrival happened to land at.  When no arrival can be deep, the LIFO balance has already returned rsp to base and the reset is a provable no-op — so it is DELETED rather than re-spelled, and with it the last reader of ___ in a determinate graph.  FALSIFIABLE BY CONSTRUCTION: if any depth-static graph does arrive off-base this drops rsp reconstruction entirely and the crosscheck fails loudly at once, which is exactly the intended tripwire. */
    if (x86_zc_frame() == ZC_FRAME_RSP) return std::string();   /* ALIGN-INV-3: under RSP fb IS the snapshot — U1/U2/s79 seed ___=base at every frame-active activation, so the s90 frame-base READ collapses to the register (the s90 hazard was rsp-RELATIVE reads at depth; reading ___ keeps its cure).  Enter store retained: s142 NOFILL dead-store-cut assumes it overwrites; reader sweep owed before retiring it. */
    char b[160]; snprintf(b, sizeof b, "mov rsp, qword ptr [%s + %d]\n", x86_zr(), xaf_anchor_off()); return std::string(b);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xaf_add_rdi_imm_bin(int n) { return (n >= -128 && n <= 127) ? bytes(3, "\x48\x83\xC7") + std::string(1, (char)(unsigned char)n) : bytes(3, "\x48\x81\xC7") + u32le((uint32_t)n); }   /* add rdi, imm — as-matching short form (imm8 when it fits), byte twin of the TEXT "add rdi, N" */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_flat_data_section_str(void) {
    if (PLATFORM_X86) {
        if (MEDIUM_MACRO_DEF) return x86("comment", "# no macro form — XA_FLAT_DATA_SECTION");
        if (MEDIUM_BINARY)    return std::string();
        if (MEDIUM_TEXT) {
            if (!g_flat_data_any) return std::string();
            return std::string("  .section .data\n")
                 + std::string(g_flat_data_buf, g_flat_data_len)
                 + "  .section .text\n";
        }
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void xa_entry_dispatch(void)    { auto s = xa_entry_dispatch_str();    if (!s.empty()) emit_text_n(s.data(), s.size()); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void xa_flat_data_section(void) { auto s = xa_flat_data_section_str(); if (!s.empty()) emit_text_n(s.data(), s.size()); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* PL-DC STUB (REGAIN-1 SLICE C, 2026-07-20 s108) — the per-proc DIRECT-CALL entry for det-lexical jmp-entry graphs.  The site `call`s here with the arg CELL POINTERS in rsi/rdx/rcx/r8 (the exact
 * registers the fused open_detN family took); the stub parks the retaddr in the FREE kt-32 header pad (restoring the wire-identical rsp parity for the shared body), builds the SAME frame the wire
 * prologue builds (___ save at kt-8, ___ seed, zeta-mark anchor), points the γ/ω wires at the two stub-local ret-shims below, stashes the arg pointers in the param slots (rt_pl_dc_prep reads them
 * back before overwriting — self-clobber-safe by read-all-then-write order in the leaf), and makes the ONE slim prep crossing (bind + NULVCL pad + slot0/suffix seed + vtmark push) before jmping the
 * shared α_body.  The shared γ/ω exits then `lea rsp,[___+kt]; jmp [wire]` exactly as for a wire activation; the shims read the retaddr from the red zone (rsp-32 — kernel-honored, no call between),
 * re-push it, and tail-jmp the leave leaves, whose `ret` lands the site with the DESCR in rax:rdx (v==99 = FAIL, the site's existing join).  Alignment: site call at rsp≡0 → entry ≡8 → pop → ≡0 =
 * IDENTICAL to the wire jmp's parity, so every body call keeps today's alignment; the shim's push makes the leaf entry ≡8, the SysV post-call shape. */
extern "C" void rt_pl_dc_prep(void *, long, long, long, long, long);
extern "C" DESCR_t rt_pl_dc_leave_γ(DESCR_t, long, void *);
extern "C" DESCR_t rt_pl_dc_leave_ω(long, void *);
extern "C" void rt_arg_stage(int idx, DESCR_t v);            /* ICN-FR-2 zframe dc stub: stage one arg into g_call_args[idx] */
extern "C" void rt_icn_zframe_args_install(void *, int, int); /* ICN-FR-2 zframe prologue: install g_call_args[0..np-1] into frame slots, no pcall lookup */
static std::string xa_flat_dc_stub_str(void) {
    if (!PLATFORM_X86) return std::string();
    x86_begin();
    int kt = g_emit.flat_frame_bytes;
    int anchor = -1;   /* ZW-0 stage 2: island arm deleted; under ZC_FRAME_RSP anchor store is always absent (ALIGN-INV-3c) */
    int suffix = (g_emit.flat_seed_off >= 16) ? g_emit.flat_seed_off : 16;
    extern int g_flat_dc_np;
    int np = g_flat_dc_np;
    uint64_t prep_fp;  { void (*fp)(void *, long, long, long, long, long) = rt_pl_dc_prep; prep_fp = (uint64_t)(uintptr_t)(void *)fp; }
    uint64_t lvg_fp;   { DESCR_t (*fp)(DESCR_t, long, void *) = rt_pl_dc_leave_γ; lvg_fp = (uint64_t)(uintptr_t)(void *)fp; }
    uint64_t lvw_fp;   { DESCR_t (*fp)(long, void *) = rt_pl_dc_leave_ω; lvw_fp = (uint64_t)(uintptr_t)(void *)fp; }
    static const char *argreg[4] = { "rsi", "rdx", "rcx", "r8" };
    if (g_emit.zframe_graph || (g_emit_cfg && g_emit_cfg->icn_cells_graph && g_emit.flat_lcl_proc)) {   /* ICN-FR-2 ZFRAME DC STUB / ZK-4 SLICE 2 CELLS-ARM CLASS ZF: stage args into g_call_args via rt_arg_stage, then jmp proc_f_α (not body) so the prologue runs.  ZK-4 SLICE 2 ROOT CAUSE: the old non-zframe DC stub (sub rsp,kt+16) allocated only flat_frame_bytes+16 bytes, but the CLASS ZF epilogue (xa_flat_wire_hdr_base) uses flat_frame_bytes+(np+nl)*16 as kt_adjusted.  For f1 (np=1,nl=1) kt_adjusted=224 but DC stub sub'd 208+16=224 but with ___ pinned at DC-stub's rsp the epilogue lea rsp,[___+224] lands ABOVE the DC stub frame top, 32 bytes off — SEGV on exit after correct output.  The arg-staging path (rt_arg_stage→g_call_args, jmp lbl_α so flat_lcl_proc prologue runs sub rsp,frame_total+installs) is identical to the zframe path: rt_lcl_proc_args_install reads g_call_args[], so staging there feeds it correctly.  CLASS ZF predicate here mirrors xa_flat_class_zf() ONE AUTHORITY.
     * The site `call`s here with arg CELL POINTERS in rsi/rdx/rcx/r8. This arm stages each into g_call_args[i] then sets wire shims and jumps proc_f_α.
     * proc_f_α: sub rsp,kt; wire-header [kt-24]=rcx [kt-16]=rdx [kt-8]=caller____; pin ___=rsp; rt_icn_zframe_args_install(___,np,nl) reads g_call_args[0..np-1].
     * γ exit: lea rsp,[___+kt] (= pushed-r11 rsp level); restore caller ___; jmp shim_γ via [___+kt-24].
     * shim_γ: pop r11; push r11 (SysV alignment ≡0); mov rdi,rax; mov rsi,rdx; jmp rt_proc_call_epilogue_γ.
     * No pcall record: dc path skips open_detN; rt_proc_call_epilogue_γ reads g_pcall[--g_pcall_top] — BUT g_pcall_top=0 → it returns FAILDESCR. WORKAROUND: use the plain leave-ω path (push r11 + jmp r11) after marshaling result — since no pcall the result is already in rax:rdx and the site's join just reads rax.
     * CLEANEST: skip epilogue entirely; the site's dc arm does `jmp L(2)` after `call dc_stub` — at L(2) rax:rdx holds the result (from the callee's γ exit). So the shim just restores rsp and returns via r11 with rax:rdx intact. */
        static const char *dcarg4[4] = { "rsi", "rdx", "rcx", "r8" };
        uint64_t stg_fp; { void (*fp)(int, DESCR_t) = rt_arg_stage; stg_fp = (uint64_t)(uintptr_t)(void *)fp; }
        /* ICN-FR-3 BUG-FIX (dc-stub caller-save): rt_arg_stage is void(int,DESCR_t) but the SysV calling convention
         * lets it clobber rax/rcx/rdx/rsi/rdi/r8/r9/r10/r11 — every one of the four dc-arg registers.  The old loop
         * read dcarg4[i] directly into r10 each iteration, but after iteration 0 the call had already clobbered rdx,
         * so iteration 1's "mov r10, rdx" loaded a garbage value and SEGVd.  Fix: push all np cell ptrs onto the stack
         * before the loop (after pop/push of r11 so retaddr is already saved); reload via [rsp+i*8] in each iteration.
         * After the loop, clean the pushes with "add rsp, push_bytes" restoring retaddr to [rsp+0] before the jmp.
         * Alignment: at dc stub entry rsp≡8 (call pushed retaddr); pop r11 → ≡0; push r11 → ≡8; then np cell-ptr
         * pushes.  Before each "call rt_arg_stage" rsp must ≡0 mod 16.  After push r11 + np pushes, rsp≡8+8*np.
         * rsp≡0 iff np is odd.  For even np, insert a dummy push at start and a corresponding add rsp,8 at end.
         * ICN-FR-3 ALIGNMENT FIX: the old single push r11 left rsp≡8 at proc_f_α entry, which propagated as a
         * systematic 8-byte parity error through every C call inside the proc (str_concat_d → gc_heap_init →
         * dl_iterate_phdr's movaps [___-0x60] → SIGSEGV).  Fix: push r11 TWICE so rsp≡0 at jmp proc_f_α.
         * Epilogue lea rsp,[___+kt] lands at the push-2 slot; shim_γ/ω pop TWICE to skip the push-2 word
         * and retrieve the original retaddr from push-1.  Stack layout from bottom (after add rsp, push_bytes):
         *   [rsp+0]  = retaddr  (push-2: alignment slot, popped-and-discarded by shim)
         *   [rsp+8]  = retaddr  (push-1: original retaddr, jumped to by shim)
         * Alignment recalculation: after 2 r11 pushes rsp≡0; np cell-ptr pushes follow.  Before rt_arg_stage
         * calls, rsp must ≡0.  rsp≡0 + np pushes: odd np → rsp≡8 → BAD → need pad; even np → rsp≡0 → OK.
         * So need_align_pad condition FLIPS relative to the old single-push baseline. */
        bool need_align_pad = (np > 0) && (np % 2 == 1);   /* odd np needs pad (rsp≡0 baseline after 2 r11 pushes) */
        std::string zs = x86("comment", "ICN-FR-3 zframe dc stub: stage args, jmp proc_f_α≡0 with wire shims")
            + x86("pop", "r11")
            + x86("push", "r11")    /* push-1: retaddr; rsp≡8 */
            + x86("push", "r11");   /* push-2: alignment slot → rsp≡0 at proc_f_α entry (all C calls inside proc are correctly aligned) */
        if (need_align_pad) zs += x86("push", "r11");   /* alignment pad: odd np → rsp≡8 after cell-ptr pushes → need one more to reach ≡0 before calls */
        /* push all arg cell ptrs in reverse order so pop (implicit via indexed load) is i=0..np-1 */
        for (int i = np - 1; i >= 0; i--) zs += x86("push", dcarg4[i]);
        /* rsp≡0 now; cell ptr[0] at [rsp+0], cell ptr[1] at [rsp+8], ...; above them: pad? push-2 push-1 (retaddrx2) */
        int push_bytes = np * 8 + (need_align_pad ? 8 : 0);   /* cell-ptr pushes + pad to clean after loop; the 2 r11 pushes stay until epilogue */
        for (int i = 0; i < np; i++) {   /* for each arg: load cell ptr from stack (caller-save-safe), read DESCR, call */
            zs += x86("mov", "r10", "[rsp + " + std::to_string(i * 8) + "]")   /* cell ptr[i] — r10+0/r10+8 not clobbered by call */
                + x86("mov32", "edi", (long)i)
                + x86("mov", "rsi", "[r10 + 0]")                  /* DESCR.v — use [r10+0] not [r10] to avoid XK_R10MIR parse */
                + x86("mov", "rdx", "[r10 + 8]")                  /* DESCR.i */
                + x86("call", "rt_arg_stage", stg_fp);
        }
        /* clean cell-ptr pushes + pad; after this: [rsp+0]=retaddr(push-2 alignment slot) [rsp+8]=retaddr(push-1 real) */
        if (push_bytes > 0) zs += x86("add", "rsp", (long)push_bytes);
        zs += x86_lea_id("rcx", 2)   /* γ shim */
            + x86_lea_id("rdx", 3)   /* ω shim */
            + x86_jmp_lblptr(g_emit.flat_dc_body_p, g_emit.flat_lbl_α ? g_emit.flat_lbl_α : "?")   /* jmp proc_f_α with rsp≡0; shim_γ/ω pop twice: skip push-2 slot, get real retaddr from push-1 */
            + x86_deflabel_id(2)   /* shim_γ: epilogue left rsp at push-2 slot; double-pop to get real retaddr */
            + x86("pop", "r11")    /* pop push-2 alignment slot (retaddr copy, discarded) */
            + x86("pop", "r11")    /* pop push-1 real retaddr */
            + x86("jmp", "r11")    /* tail-return to call site's L(2) join; rax:rdx hold result from γ epilogue */
            + x86_deflabel_id(3)   /* shim_ω: same double-pop protocol */
            + x86("pop", "r11")    /* pop push-2 alignment slot */
            + x86("pop", "r11")    /* pop push-1 real retaddr */
            + x86("mov32", "eax", 104L)   /* DT_FAIL — matches what the site checks: cmp eax, DT_FAIL; je proc_ω */
            + x86("xor", "edx", "edx")
            + x86("jmp", "r11");
        return zs;
    }
    return x86("comment", "PL-DC direct-call entry: retaddr -> kt-32 pad, wires -> local ret-shims, one prep crossing, shared body")
         + x86("pop", "r11")
         + x86("sub", "rsp", (long)(kt + 16))
         + x86_rsp_store64(kt - 8, "rsp")
         + std::string("")
         + x86("mov", FRQ(kt - 32), "r11")
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
         + x86_rsp_load64("r11", -32)
         + x86_rsp_load64("rsp", -8)
         + x86("add", "rsp", 16L)
         + x86("push", "r11")
         + x86_jmpfn("rt_pl_dc_leave_γ", lvg_fp)
         + x86_deflabel_id(3)
         + x86_rsp_load64("rdi", 0)
         + x86("mov", "rsi", "rsp")
         + x86("add", "rsi", (long)(-kt))
         + x86_rsp_load64("r11", -32)
         + x86_rsp_load64("rsp", -8)
         + x86("add", "rsp", 16L)
         + x86("push", "r11")
         + x86_jmpfn("rt_pl_dc_leave_ω", lvw_fp);
}
extern "C" void xa_flat_dc_stub(void) { bb_emit_x86(xa_flat_dc_stub_str()); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ICN-FR-2: ζ-FRAME PROLOGUE — re-expressed from anchor xa_flat.cpp:164–170 using x86() calls, BOTH MEDIUM.
 * R5 parameterless: reads g_emit (flat_frame_bytes, lbl_α_body name for comment) and emit_jmp_pin____() — no locals leak into the arm's concat.
 * R10: byte-identity with the anchor BINARY is NOT required; the encoder picks imm8/disp8 short forms where they fit — behavioral gates only.
 * LAYOUT CONTRACT: kt is sized by the existing layout pass (flat_frame_bytes includes the wire header floor of 48).  The pin is gate-driven by
 * emit_jmp_pin____(), which now includes g_emit.zframe_graph, so the save+seed pair fires unconditionally for every ζ-frame graph — every such
 * graph is either deep-arriving (jmp-wire procs) or suspending (gen), both of which require the pinned ___ for the REG-7 U5 resume/exit record.
 * Wire header layout per THE ANCHOR (re-stated for FR-2's reader, never re-derived): [rsp+kt-24]=rcx (outside-γ wire), [rsp+kt-16]=rdx (outside-ω
 * wire), [rsp+kt-8]=___ (caller ___ saved for REG-7 U2 restore on exit), then ___ seeded to this activation's flat base (mov ___,rsp).
 * Callers: emit.cpp alpha dispatch, gated on g_emit.zframe_graph after all other jmp-entry arms. */
extern "C" void rt_lcl_proc_args_install(void *, int, int);   /* ICN-PROC-FRAME (s211): used by flat_lcl_proc arm in emit.cpp */
extern "C" void rt_icn_zframe_args_install(void *, int, int);   /* ICN-FR-2: zframe variant — reads g_call_args directly, no pcall lookup; correct for both dc-stub and jmp-entry C paths */
extern "C" void rt_arg_stage(int idx, DESCR_t v);   /* ICN-FR-2: stage one arg into g_call_args[idx]; used by zframe dc stub */
static std::string xa_flat_zframe_prologue_str(void) {
    if (!PLATFORM_X86 || !g_emit.zframe_graph) return std::string();
    int kt = g_emit.flat_frame_bytes;
    if (kt < 48 || (kt & 15)) { fprintf(stderr, "FATAL xa_flat_zframe_prologue: kt=%d (must be 16-mult >= 48)\n", kt); abort(); }
    int np = g_emit_cfg ? g_emit_cfg->nparams : 0;
    int nl = g_emit_cfg ? g_emit_cfg->nlocals : 0;
    std::string s = x86("comment", "ICN-FR-2 zframe prologue: sub rsp,kt + wire header [kt-24]=γ [kt-16]=ω [kt-8]=caller____ + pin ___=rsp")
         + x86("sub", "rsp", (long)kt)
         + x86("mov", "[rsp + " + std::to_string(kt - 24) + "]", "rcx")
         + x86("mov", "[rsp + " + std::to_string(kt - 16) + "]", "rdx")
         + (emit_jmp_pin_legacy() ? x86("mov", "[rsp + " + std::to_string(kt - 8) + "]", "rsp")
                                + std::string("")
                               : std::string());
    if (g_emit.flat_lex) {   /* PL-FR-2 LEX SEED ARM: lexical graphs (Prolog + det-lex Icon procs) carry a pcall record with a lex seed; call rt_jmp_frame_lexprep2(fb=___, suffix_off, region_bytes=kt-32) which runs: pcall registration (c->fb=fb) + slot0 NULVCL + suffix NULVCL splat + rt_frame_bind_args (g_call_args→frame slots, NULVCL pad).  Behavioral discriminator: flat_lex==1 iff the activation is lexical-scope and non-LBL__ — exactly the graphs whose open_detN/open_genN pushed a pcall record that lexprep2 can read.  seed_off=0 case falls back to full fill. */
        extern int g_flat_dc_np;   /* PL-DC: >=0 when dc-stub is active; dc-stub stages args via rt_arg_stage into g_call_args but does NOT push a pcall record (it jumps directly to proc_f_α). For the dc path use rt_icn_zframe_args_install which reads g_call_args directly; for the normal open_det path use rt_jmp_frame_lexprep2 which reads the pcall record. Both are called with the same g_call_args content. NULVCL seeding: dc path uses the rep-stosb FRAME_RSP arm below for the whole frame before jumping here — Prolog dc procs must zero first; normal open_det path: rt_jmp_frame_lexprep2 seeds slot0 + suffix. */
        if (g_flat_dc_np >= 0) {   /* DC-STUB PATH: no pcall record; zero frame first (named vars start as NULVCL = unbound), then rt_icn_zframe_args_install reads g_call_args[0..np-1] directly */
            if (kt > 48) {   /* zero the data region [___+0..___+kt-32) before installing args */
                int data_bytes = kt - 32;
                s += x86("mov", "rdi", "rsp")   /* frame base = rsp post-grant; [rsp-1] (a50037ae) is the dynamic-depth sentinel — no lea arm, silently emitted nothing (ZB-FC-1) */
                   + x86("xor", "eax", "eax")
                   + x86("mov32", "ecx", (long)data_bytes)
                   + x86("rep_stosb");
            }
            uint64_t _args_fp; { void (*_f)(void *, int, int) = rt_icn_zframe_args_install; _args_fp = (uint64_t)(uintptr_t)(void *)_f; }
            s += x86("mov", "rdi", "rsp")
               + x86("mov32", "esi", (long)np)
               + x86("mov32", "edx", (long)nl)
               + x86("call", "rt_icn_zframe_args_install", _args_fp);
        } else {   /* NORMAL JMP-ENTRY PATH: pcall record was pushed by rt_proc_call_open_det; rt_jmp_frame_lexprep2 reads it */
            uint64_t _lex_fp; { void (*_f)(void *, long, long) = rt_jmp_frame_lexprep2; _lex_fp = (uint64_t)(uintptr_t)(void *)_f; }   /* rt_jmp_frame_lexprep2 declared extern "C" at file scope (line 11) */
            int seed_off = g_emit.flat_seed_off ? g_emit.flat_seed_off : 16;
            s += x86("mov", "rdi", "rsp")
               + x86("mov32", "esi", (long)seed_off)
               + x86("mov32", "edx", (long)(kt - 32))
               + x86("call", "rt_jmp_frame_lexprep2", _lex_fp);
            if (g_emit.flat_gen && g_emit_cfg && g_emit_cfg->icn_zframe_gen) {
                /* ICN-FR-5 ONE-SLOT FIX: save_wires and save_caller____ both run AFTER lexprep2.
                 * lexprep2 writes pcall.fb = ___ (ONE AUTHORITY: rt.c rt_proc_call_prologue_lex).
                 * icn_gen_find_slot scans g_pcall for .fb == gen____; must run after fb is populated.
                 * Wire values at [___+kt-24] and [___+kt-16] are written at prologue top (before lexprep2)
                 * and survive — lexprep2 seeds only [___+0..___+kt-32), leaving the header triple intact. */
                uint64_t _sw_fp;  { void (*_f)(void *, void *, void *) = rt_gen_save_wires;      _sw_fp  = (uint64_t)(uintptr_t)(void *)_f; }
                                s += x86("mov", "rdi", "rsp")               /* gen____ */
                   + x86("mov", "rsi", "qword ptr [rsp# + " + std::to_string(kt - 24) + "]")   /* γ-wire */
                   + x86("mov", "rdx", "qword ptr [rsp# + " + std::to_string(kt - 16) + "]")   /* ω-wire */
                   + x86("call", "rt_gen_save_wires", _sw_fp);
            }
        }
    } else {   /* PL-FR-2 FRAME_RSP ARM: non-lex graphs (Prolog main / outer graphs) have no pcall record; zero-fill the data region with rep stosb so named variables (G0, G1...) start as NULVCL (= DT_SNUL = unbound). Mirrors anchor FRAME_RSP arm (rep stosb + 65544 floor). Harmless for Icon (Icon never stamps non-lex zframe graphs except via dc-stub which has its own path in the stub code above). */
        if (kt > 48) {   /* data region = [___+0..___+kt-32); floor at 48 has 16B data region which is just slot0 — skip rep stosb for the minimum frame */
            int data_bytes = kt - 32;   /* bytes below the 32B wire-header+pad: γ(8B)+ω(8B)+caller____(8B)+8B_align = 32B; data occupies [___+0..___+kt-32) */
            s += x86("mov", "rdi", "rsp")   /* frame base = rsp post-grant; [rsp-1] (a50037ae) is the dynamic-depth sentinel — no lea arm, silently emitted nothing (ZB-FC-1) */
               + x86("xor", "eax", "eax")          /* value = 0 (NULVCL = DT_SNUL = 0) */
               + x86("mov32", "ecx", (long)data_bytes)
               + x86("rep_stosb");   /* rep stosb: byte-zero [rdi..rdi+rcx) */
        }
        if (np > 0 || nl > 0) {   /* ICN-FR-2(f) RESOLVED: rt_icn_zframe_args_install reads g_call_args[0..np-1] directly (no pcall-nargs clamp) — correct for BOTH the dc-stub path (args staged into g_call_args by rt_arg_stage calls in the stub) and the jmp-entry C path (open_detN copies args into g_call_args then jmps proc_f_α). */
            uint64_t _args_fp; { void (*_f)(void *, int, int) = rt_icn_zframe_args_install; _args_fp = (uint64_t)(uintptr_t)(void *)_f; }
            s += x86("mov", "rdi", "rsp")
               + x86("mov32", "esi", (long)np)
               + x86("mov32", "edx", (long)nl)
               + x86("call", "rt_icn_zframe_args_install", _args_fp);
        }
    }
    return s;
}
/* ICN-FR-2: ζ-FRAME EPILOGUE γ (success / suspend port).
 * Re-expressed from anchor epilogue (git 1ba33ea6~1 xa_flat.cpp:172-174 TEXT / :185-199 BINARY) for ζ-frame graphs.
 * γ UNWINDS ABSOLUTELY: lea rsp,[___+kt] rejoins pre-entry rsp from ANY arrival depth.
 * Then loads the γ wire from [___+kt-24], restores caller ___ from [___+kt-8], and jmps the wire.
 * This is the FR-3 exit protocol expressed in FR-2 so simple procs (f0/f1/fib) exit correctly.
 * Depth-immune because it reads through the PINNED ___ (depth-independent by construction). */
/* ⭐ ONE AUTHORITY for the CLASS-ZF wire-header base (s215, ZK-4 SLICE 1 — the s22k law: this constant is spelled HERE and nowhere else).
 * The header triple always sits at [base-24]=γ [base-16]=ω [base-8]=caller-___, but WHICH base depends on which prologue carved the frame:
 *   ZFRAME regime (xa_flat_zframe_prologue): carves exactly flat_frame_bytes, param slots INSIDE it  → base = flat_frame_bytes.
 *   CELLS regime  (the flat_lcl_proc prologue, emit.cpp ICN-PROC-FRAME): carves flat_frame_bytes + (np+nl)*16, param/local slots ADDED ON → base = that total.
 * Reading the wrong base under-reads by (np+nl)*16 (16 bytes on f1: header at 136 read as 120) and jmps a garbage wire.
 * MUST STAY IN LOCKSTEP with emit.cpp's `frame_total` in the flat_lcl_proc prologue arm — same expression, two consumers. */
static int xa_flat_wire_hdr_base(void) {
    int kt = g_emit.flat_frame_bytes;
    if (g_emit_cfg && g_emit_cfg->icn_cells_graph && g_emit.flat_lcl_proc) kt += (g_emit_cfg->nparams + g_emit_cfg->nlocals) * 16;
    return kt;
}
/* CLASS-ZF admission predicate — ONE AUTHORITY (s215).  ZFRAME graphs by their own flag; CELLS-arm lexical procs because the flat_lcl_proc
 * prologue writes the IDENTICAL header triple (verified f1: sub rsp,160 / [rsp+136]=rcx / [rsp+144]=rdx / [rsp+152]=___ / mov ___,rsp).
 * WHY the cells arm needs this: CLASS P (_wire_stub → bb_glue_wire_γ/ω → rt_flat_ret_snap → g_pcall_wires[]) requires a pcall record, but the
 * cells-arm call site enters via the DC stub (rt_pl_dc_prep) which pushes NONE — g_pcall_top==0 → core error 18 "Return from level zero" on
 * every proc call.  The header read is self-contained and needs no runtime record, which is exactly why ICN-FR-3 chose it for ZFRAME. */
static int xa_flat_class_zf(void) {
    if (g_emit.zframe_graph) return 1;
    if (g_emit_cfg && g_emit_cfg->icn_cells_graph && g_emit.flat_lcl_proc) return 1;
    return 0;
}
static std::string xa_flat_zframe_epilogue_γ_str(void) {
    if (!PLATFORM_X86 || !xa_flat_class_zf()) return std::string();
    int kt = xa_flat_wire_hdr_base();
    if (g_emit.flat_gen && g_emit_cfg && g_emit_cfg->icn_zframe_gen) {
        /* ICN-FR-4 LAYER 2+1+3 + PL-ZD-WINDOW2-FIX: GATED icn_zframe_gen (Icon zframe generators only).
         * Prolog zframe predicates with flat_gen=1 fall through to the direct ___RAWQ frame-read path below
         * (same as the non-gen path); their frame-resident wire and caller____ values are not clobbered
         * between γ-exit and the prologue of the NEXT activation (Prolog uses WAM-style choice points,
         * not the single-active-generator model that makes globals safe for Icon). */
        uint64_t _ggw_fp; { void *(*_f)(void *) = rt_gen_get_gamma_wire; _ggw_fp = (uint64_t)(uintptr_t)(void *)_f; }
        /* ICN-FR-5 ROOT-CAUSE FIX — NO RSP UNWIND IN GENERATOR γ-EPILOGUE.
         * ROOT CAUSE: the `lea rsp,[___+kt]` (entry_rsp unwind) was the origin of ALL frame-clobber bugs.
         * With rsp at entry_rsp, any C call from the L(3) landing (rt_proc_call_epilogue_γ, rt_call_arr,
         * etc.) pushes frames into [entry_rsp-N .. entry_rsp-8] = [gen____+kt-N .. gen____+kt-8].
         * That region IS the generator frame: params at [gen____+16..], locals at [gen____+48..], etc.
         * ANCHOR MECHANIC §5 "γ-RETAIN": at γ exit, rsp is already at gen____ (n6_suspend_α did
         * `add rsp, claim` before `jmp proc_upto_γ`). Leave rsp THERE. The L(3) landing is entirely
         * ___-relative and doesn't need a specific rsp. Subsequent C calls (epilogue_γ, write, ...) push
         * BELOW gen____ — safely outside the generator's frame [gen____ .. gen____+kt).
         * The PLT calls must still precede the ___-overwrite (order below): callee-saved r14=gen____
         * survives them; callee-saved r15 holds the γ-wire; ___ is only overwritten AFTER all reads.
         * BYTE-IDENTICAL for non-generator zframe graphs (they fall through to the ICN-FR-2 path below
         * which DOES unwind — non-generators never have a do-body and their L(3) callers are not
         * affected since no live generator frame sits between the activation frame and the caller). */
        return x86("comment", "ICN-FR-5 no-unwind epilogue-γ: r14=gen____; get γ-wire(r14)→r15; get caller____(r14)→___ (rsp stays at gen____); rdi:rsi=[r14+0/8]; rax=gen____; jmp r15")
             + x86("mov", "r14", "rsp")                /* save generator____ in callee-saved r14 */
             + x86("mov", "rdi", "r14")                /* ICN-FR-5: gen____ as first arg for keyed lookup */
             + x86("call", "rt_gen_get_gamma_wire", _ggw_fp)  /* → rax = γ-wire */
             + x86("mov", "r15", "rax")                /* γ-wire in callee-saved r15 */
             /* NO lea rsp,[___+kt] — rsp stays at gen____ (the deep frontier); L(3) is ___-relative */
             + x86("mov", "rdi", "[r14 + 0]")          /* yield v-field from generator frame via r14 */
             + x86("mov", "rsi", "[r14 + 8]")          /* yield i-field from generator frame via r14 */
             + x86("mov", "rax", "r14")                /* generator____ → rax for L(3) landing */
             + x86("jmp", "r15");
    }
    /* PL-FR-4 PENDING RESUME INTERCEPT (Prolog zframe gen, non-icn): when bcps_spine_gen_arm's β arm set
     * g_pl_zf_pending_cursor (via rt_pl_zf_resume_set before re-calling rt_proc_call_open_det), the caller
     * wants to SKIP clause 1's result and resume at the cursor (n15_suspend_β = clause 2 entry).  The intercept
     * fires HERE — while ___ still points to the callee's live frame — before the lea rsp,[___+kt] unwind.
     * Protocol: check g_pl_zf_pending_cursor; if set, write it into [___+resume_slot] (OVERRIDING the α_body
     * initial cursor write) and jmp there directly with the frame still live.  The trail mark at [___+tm_off]
     * was already written by rt_jmp_frame_lexprep2 (pending override) and re-set by n0_call_builtin_prolog_α
     * ($trail_mark) to the current trail top — which is correct for the new clause attempt.
     * Guard: only for Prolog zframe generators with a valid resume_slot (flat_gen=1, !icn_zframe_gen). */
    if (g_emit.flat_gen && g_emit_cfg && g_emit_cfg->zframe_graph && !g_emit_cfg->icn_zframe_gen && g_emit_cfg->resume_slot > 0) {
        extern void *g_pl_zf_pending_cursor;
        extern void rt_pl_zf_resume_clear(void);
        uint64_t _clear_fp; { void (*_f)(void) = rt_pl_zf_resume_clear; _clear_fp = (uint64_t)(uintptr_t)(void *)_f; }
        int rs = g_emit_cfg->resume_slot;
        return x86("comment", "PL-FR-4 zframe epilogue-γ (Prolog gen): check pending resume cursor before unwind")
             + x86("lea", "r11", "[rip + __]", (uint64_t)(uintptr_t)&g_pl_zf_pending_cursor, "g_pl_zf_pending_cursor")
             + x86("mov", "r11", "[r11]")           /* r11 = g_pl_zf_pending_cursor (0 = normal path) */
             + x86("test", "r11", "r11")
             + x86("je", L(50))                     /* 0 = no pending resume, normal γ-exit */
             /* pending resume: write cursor into [___+resume_slot] (overrides α_body's clause-1 cursor) */
             + x86("mov", "qword ptr [rsp# + " + std::to_string(rs) + "]", "r11")   /* cursor = pending cursor */
             + x86("call", "rt_pl_zf_resume_clear", _clear_fp)            /* clear the pending flag */
             + x86("mov", "rax", "qword ptr [rsp# + " + std::to_string(rs) + "]") + x86("jmp", "rax")           /* jmp cursor (= n15_suspend_β) */
             + x86("def", L(50))
             /* normal γ-exit path */
             + x86("mov", "rdi", "rax")
             + x86("mov", "rsi", "rdx")
             + x86("mov", "rcx", "qword ptr [rsp# + " + std::to_string(kt - 24) + "]")
             + x86("add", "rsp", (long)kt)
             + std::string("")
             + x86("jmp", "rcx");
    }
    return x86("comment", "ICN-FR-2 zframe epilogue-γ: marshal result rax:rdx→rdi:rsi; unwind; restore caller ___ from header; jmp γ wire")
         + x86("mov", "rdi", "rax")
         + x86("mov", "rsi", "rdx")
         + x86("mov", "rcx", "qword ptr [rsp# + " + std::to_string(kt - 24) + "]")
         + x86("add", "rsp", (long)kt)
         + std::string("")
         + x86("jmp", "rcx");
}
/* ICN-FR-2: ζ-FRAME EPILOGUE ω (failure port).
 * Same absolute-unwind shape as γ but reads the ω wire from [___+kt-16].
 * ICN-FR-4 LAYER 3: for flat_gen, retrieve caller____ from pcall.rname (heap-safe). ω is terminal — no L1.
 * CRITICAL: load ω-wire into r15 BEFORE lea rsp (see γ epilogue comment for the clobber analysis). */
static std::string xa_flat_zframe_epilogue_ω_str(void) {
    if (!PLATFORM_X86 || !xa_flat_class_zf()) return std::string();
    int kt = xa_flat_wire_hdr_base();
    if (g_emit.flat_gen && g_emit_cfg && g_emit_cfg->icn_zframe_gen) {   /* ICN-FR-5: GATED icn_zframe_gen. */
        uint64_t _gow_fp; { void *(*_f)(void *) = rt_gen_get_omega_wire; _gow_fp = (uint64_t)(uintptr_t)(void *)_f; }
        uint64_t _sw_fp;  { void  (*_f)(void *, void *, void *) = rt_gen_save_wires; _sw_fp = (uint64_t)(uintptr_t)(void *)_f; }
        /* ICN-FR-5 ROOT-CAUSE FIX — NO RSP UNWIND IN GENERATOR ω-EPILOGUE (same as γ).
         * At ω entry, rsp is already at gen____ (add rsp,claim ran before jmp proc_upto_ω).
         * Don't unwind to entry_rsp; ω's fail-path callers are also ___-relative and harmless. */
        return x86("comment", "ICN-FR-5 no-unwind epilogue-ω: r14=gen____; get ω-wire→r15; get caller____→rbx; pop gen state stack; ___=rbx; jmp r15")
             + x86("mov", "r14", "rsp")                        /* save gen____ before any call clobbers ___ */
             + x86("mov", "rdi", "r14")                        /* gen____ for ω-wire lookup */
             + x86("call", "rt_gen_get_omega_wire", _gow_fp)   /* rax = ω-wire keyed by gen____ */
             + x86("mov", "r15", "rax")                        /* ω-wire in callee-saved r15 */
             + x86("mov", "rdi", "r14")                        /* gen____ for pop sentinel */
             + x86("xor", "esi", "esi")                        /* gw = NULL → pop sentinel */
             + x86("xor", "edx", "edx")                        /* ww = NULL */
             + x86("call", "rt_gen_save_wires", _sw_fp)        /* pop this generator's stack entry */
             /* NO lea rsp,[___+kt] — rsp stays at gen____; caller manages its own rsp */
             + x86("jmp", "r15");
    }
    return x86("comment", "ICN-FR-2 zframe epilogue-ω: unwind to flat base; load ω wire; restore caller ___; jmp")
         + x86("mov", "rcx", "qword ptr [rsp# + " + std::to_string(kt - 16) + "]")
         + x86("add", "rsp", (long)kt)
         + std::string("")
         + x86("jmp", "rcx");
}
extern "C" void xa_flat_zframe_prologue(void) { bb_emit_x86(xa_flat_zframe_prologue_str()); }
extern "C" void xa_flat_zframe_epilogue_γ(void) { bb_emit_x86(xa_flat_zframe_epilogue_γ_str()); }
extern "C" void xa_flat_zframe_epilogue_ω(void) { bb_emit_x86(xa_flat_zframe_epilogue_ω_str()); }
