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
extern "C" void rt_main_args_fetch(void);   /* ICNBENCH-ARGS-RSP: returns the staged main(args) DESCR in rax:rdx (true return type is DESCR_t; declared void here — the template only takes its ADDRESS, the emitted call site honors the real ABI) */
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
 * \x49\xC7\x84\x24 forms, and ZC_FRAME_RBP gets 0x55 / 48 89 FD / 49 89 ED-family / 0x5D / 48 C7 85 (shorter:
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
/* ALIGN-INV-2 / RBP-FREE (Lon directive 2026-07-13: "I'd rather waste 8 bytes to gain a register"; rbp -> the
 * dcap cursor).  The graph-level anchor no longer lives in a REGISTER: it lives in the ZETA FRAME, at the pad
 * qword the graph-scope zeta_mark slot has always reserved and never used (zeta_storage.c: "zeta_mark.pad
 * (unused)").  Legal because the frame base (r12 = fb) is loaded per ACTIVATION from rdi in this very
 * prologue, so [fb + anchor] is per-activation too => recursion-safe and depth-independent, which is exactly
 * what the old push-rbp/mov-rbp,rsp pair bought and the only reason a register was used at all.  Frame growth
 * ZERO (the pad already existed).  Stack-parity IDENTICAL: the old pair moved rsp by -16 (push 8 + sub 8), the
 * new store moves it by 0, and 16 == 0 (mod 16), so every downstream alignment claim survives untouched.
 * Restore point is unchanged (rsp as of end-of-prologue-pushes), so the epilogue's existing dpop/pop_frame run
 * at exactly the positions they already expected.  NO FALLBACK, and that is a MEASURED claim not a hopeful
 * one: an instrumented build counted zeta_mark_slot<0 on frame-active graphs across 627 programs of all five
 * frontends (307 snobol4 crosscheck + icon/prolog/raku/pascal) and got ZERO, which is what zls_build's
 * unconditional assignment already implied -- so the rbp arms were deleted outright and a slotless
 * frame-active graph is now a LOUD FATAL (a compiler bug, not a codegen mode).  x86_align_save() died with
 * them: emitted code's only remaining rbp uses are the four TRANSIENT push/pop anchor windows, which is what
 * makes rbp available to dcap. */
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
static int xaf_deep(void) { return g_emit.flat_deep_arrival || (!xaf_shed_np() && g_emit.flat_outer_nparams >= 1); }   /* FLATDISP-5b/5c (s193): THE one outer-frame rbp condition.  SHED-1 (OMEGA s27 session): nparams conjunct retired under default SCRIP_SHED_NP=1.  Every graph with nparams>=1 is a DEFINE proc entered via jmp-wire (flat_jmp_entry=1 implies flat_deep_arrival=1); the conjunct is therefore redundant.  Measured: 318/318 byte-identical SCRIP_SHED_NP=1 vs =0.  ICNBENCH-ARGS note: the [rbp+16]/[rbp+24] param stores in xa_flat prologue are only reached when flat_jmp_entry=1 (proc wire entry), which implies deep_arrival=1 already — so the seed is established by that path regardless.  Killswitch =0 if a depth-static nparams>=1 graph ever appears (not in current corpus). */
static std::string xaf_anchor_leave_bin(void) {
    if (!x86_port_cstack()) return std::string();
    if (x86_zc_frame() == ZC_FRAME_RSP && !xaf_deep()) return std::string();   /* FLATDISP-5c (s193): BINARY twin of the TEXT gate below — the bulk unwind reads the rbp seed, so a depth-static graph (which never seeded) must not emit it; LIFO balance has rsp at base already.  This line is WHY m3 held at 185 while m4 broke in s192: the TEXT arm was gated without this twin (BOTH-MEDIUM violation, recorded not hidden). */
    if (x86_zc_frame() == ZC_FRAME_RSP) return bytes(3, "\x48\x89\xEC");   /* mov rsp, rbp — ALIGN-INV-3 twin of the TEXT arm (as-form 48 89 EC); the s90 rsp-relative-read lesson lives in the TEXT arm comment */
    return xaf_frame_rsp_rm((uint32_t)xaf_anchor_off(), 0x8B);
}
static std::string xaf_anchor_leave_text(void) {
    if (!x86_port_cstack()) return std::string();
    if (x86_zc_frame() == ZC_FRAME_RSP && !xaf_deep()) return std::string();   /* FLATDISP-5 GATE (s193: condition unified into xaf_deep, see its comment): this reset is the BULK unwind that returns rsp to the activation base from whatever depth the arrival happened to land at.  When no arrival can be deep, the LIFO balance has already returned rsp to base and the reset is a provable no-op — so it is DELETED rather than re-spelled, and with it the last reader of rbp in a determinate graph.  FALSIFIABLE BY CONSTRUCTION: if any depth-static graph does arrive off-base this drops rsp reconstruction entirely and the crosscheck fails loudly at once, which is exactly the intended tripwire. */
    if (x86_zc_frame() == ZC_FRAME_RSP) return std::string("mov rsp, rbp\n");   /* ALIGN-INV-3: under RSP fb IS the snapshot — U1/U2/s79 seed rbp=base at every frame-active activation, so the s90 frame-base READ collapses to the register (the s90 hazard was rsp-RELATIVE reads at depth; reading rbp keeps its cure).  Enter store retained: s142 NOFILL dead-store-cut assumes it overwrites; reader sweep owed before retiring it. */
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
 * prologue builds (rbp save at kt-8, rbp seed, zeta-mark anchor), points the γ/ω wires at the two stub-local ret-shims below, stashes the arg pointers in the param slots (rt_pl_dc_prep reads them
 * back before overwriting — self-clobber-safe by read-all-then-write order in the leaf), and makes the ONE slim prep crossing (bind + NULVCL pad + slot0/suffix seed + vtmark push) before jmping the
 * shared α_body.  The shared γ/ω exits then `lea rsp,[rbp+kt]; jmp [wire]` exactly as for a wire activation; the shims read the retaddr from the red zone (rsp-32 — kernel-honored, no call between),
 * re-push it, and tail-jmp the leave leaves, whose `ret` lands the site with the DESCR in rax:rdx (v==99 = FAIL, the site's existing join).  Alignment: site call at rsp≡0 → entry ≡8 → pop → ≡0 =
 * IDENTICAL to the wire jmp's parity, so every body call keeps today's alignment; the shim's push makes the leaf entry ≡8, the SysV post-call shape. */
extern "C" void rt_pl_dc_prep(void *, long, long, long, long, long);
extern "C" DESCR_t rt_pl_dc_leave_γ(DESCR_t, long, void *);
extern "C" DESCR_t rt_pl_dc_leave_ω(long, void *);
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
    return x86("comment", "PL-DC direct-call entry: retaddr -> kt-32 pad, wires -> local ret-shims, one prep crossing, shared body")
         + x86("pop", "r11")
         + x86("sub", "rsp", (long)(kt + 16))
         + x86_rsp_store64(kt - 8, "rbp")
         + x86("mov", "rbp", "rsp")
         + x86("mov", FRQ(kt - 32), "r11")
         + x86_lea_id("rax", 2)
         + x86("mov", FRQ(kt - 24), "rax")
         + x86_lea_id("rax", 3)
         + x86("mov", FRQ(kt - 16), "rax")
         + IF(anchor >= 0, x86("mov", FRQ(anchor), "rbp"))
         + FOR(0, np, [&](int i) { return x86("mov", FRQ(16 + 8 * i), argreg[i]); })
         + x86("mov", "rdi", "rbp")
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
         + x86_rsp_load64("rbp", -8)
         + x86("add", "rsp", 16L)
         + x86("push", "r11")
         + x86_jmpfn("rt_pl_dc_leave_γ", lvg_fp)
         + x86_deflabel_id(3)
         + x86_rsp_load64("rdi", 0)
         + x86("mov", "rsi", "rsp")
         + x86("add", "rsi", (long)(-kt))
         + x86_rsp_load64("r11", -32)
         + x86_rsp_load64("rbp", -8)
         + x86("add", "rsp", 16L)
         + x86("push", "r11")
         + x86_jmpfn("rt_pl_dc_leave_ω", lvw_fp);
}
extern "C" void xa_flat_dc_stub(void) { bb_emit_x86(xa_flat_dc_stub_str()); }
