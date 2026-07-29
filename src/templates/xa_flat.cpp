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
static std::string xaf_mov_frame_rdi(void) { int z = x86_zr_num(); std::string c; c += (char)(0x48 | (z >= 8 ? 0x01 : 0x00)); c += (char)0x89; c += (char)(0xC0 | (7 << 3) | (z & 7)); return c; }
static std::string xaf_mov_dreg_frame(int d) { int z = x86_zr_num(); std::string c; c += (char)(0x48 | (z >= 8 ? 0x04 : 0x00) | (d >= 8 ? 0x01 : 0x00)); c += (char)0x89; c += (char)(0xC0 | ((z & 7) << 3) | (d & 7)); return c; }
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
static std::string xaf_store_zr_rsp8(int disp8) {   /* mov [rsp+disp8], zr */
    int z = x86_zr_num(); std::string c; c += (char)(0x48 | (z >= 8 ? 0x04 : 0x00)); c += (char)0x89; c += (char)(0x40 | ((z & 7) << 3) | 4); c += (char)0x24; c += (char)(disp8 & 0xFF); return c;
}
static std::string xaf_lea_zr_rsp8(int disp8) {     /* lea zr, [rsp+disp8] */
    int z = x86_zr_num(); std::string c; c += (char)(0x48 | (z >= 8 ? 0x04 : 0x00)); c += (char)0x8D; c += (char)(0x40 | ((z & 7) << 3) | 4); c += (char)0x24; c += (char)(disp8 & 0xFF); return c;
}
static std::string xaf_mov_rdi_zr(void) {           /* mov rdi, zr */
    int z = x86_zr_num(); std::string c; c += (char)(0x48 | (z >= 8 ? 0x04 : 0x00)); c += (char)0x89; c += (char)(0xC0 | ((z & 7) << 3) | 7); return c;
}
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
static std::string xaf_anchor_enter_bin(void) {
    if (!x86_port_cstack()) return std::string();
    if (x86_zc_frame() == ZC_FRAME_RSP) return std::string();   /* ALIGN-INV-3b: RSP leaves read rbp directly; no slot reader remains — store removed, regression is the falsifier (s142 NOFILL skip at the short-suffix unroll is now a stale micro-opt if this holds) */
    return xaf_frame_rsp_rm((uint32_t)xaf_anchor_off(), 0x89);
}
static std::string xaf_anchor_enter_x86(void) {
    if (!x86_port_cstack()) return std::string();
    if (x86_zc_frame() == ZC_FRAME_RSP) return std::string();   /* ALIGN-INV-3b: RSP leaves read rbp directly; no slot reader remains — store removed, regression is the falsifier (s142 NOFILL skip at the short-suffix unroll is now a stale micro-opt if this holds) */
    return x86("mov", (std::string("qword ptr [") + x86_zr() + " + " + std::to_string(xaf_anchor_off()) + "]").c_str(), "rsp");
}   /* XA-FLAT-CONVERT: x86() twin of xaf_anchor_enter_bin — same store (anchor slot <- rsp), frame-register-agnostic via x86_zr() so ZC_FRAME_RSP and ZC_FRAME_RBP both encode correctly, and medium-invisible because the encoder switches internally. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void xaf_mark(const char * arm) { static int on = -1; if (on < 0) { const char * e = getenv("SCRIP_XAF_MARK"); on = e ? (atoi(e) != 0) : 0; } if (on) fprintf(stderr, "XAFMARK %s\n", arm); }   /* XA-FLAT-CONVERT reachability instrument (s150).  KEEP: this is not debug residue, it is the measurement that decides convert-vs-delete per arm, and s150 proved it load-bearing — s149 declared four arms dead from a Prolog+Icon sweep alone and NOFILL turned out to fire 2354 times in SNOBOL4.  Env-gated, resolved once, inert by default; emit-time only, never on a runtime path.  Method: SCRIP_XAF_MARK=1 <sweep a corpus> 2>&1 | grep -o "XAFMARK [A-Z0-9_]*" | sort | uniq -c. */
static std::string xaf_jmp_hdr_x86(void) {
    int kt = g_emit.flat_frame_bytes;
    return x86("sub", "rsp", (long)kt)
         + x86("mov", (std::string("qword ptr [rsp + ") + std::to_string(kt - 24) + "]").c_str(), "rcx")
         + x86("mov", (std::string("qword ptr [rsp + ") + std::to_string(kt - 16) + "]").c_str(), "rdx")
         + (emit_jmp_pin_rbp() ? x86("mov", (std::string("qword ptr [rsp + ") + std::to_string(kt - 8)  + "]").c_str(), "rbp")
                               + x86("mov", "rbp", "rsp") : std::string());
}   /* XA-FLAT-CONVERT slice B: x86() twin of the jmp-entry raw-byte `hdr` — HEADER ABOVE THE FRAME (outside-γ wire at kt-24, outside-ω wire at kt-16, caller rbp at kt-8), then rbp seeded to this activation's flat base.  FLATDISP-7 GATE (s194): the save+seed pair rides emit_jmp_pin_rbp — see the raw-byte hdr twin and emit.h.  Parameterless per R5 (reads g_emit, no locals leak into the arm's concat).  ⚠ THE BYTES ARE NOT THE LEGACY BYTES AND THAT IS CORRECT (R10): the legacy arm hand-encoded `sub rsp,imm32` + three disp32 stores unconditionally, while the encoder picks the as-matching imm8/disp8 short form whenever kt fits, so a small-frame blob converts SHORTER.  Byte-identity vs the legacy BINARY is therefore NOT the acceptance test here — behavioral gates are (s149 §5). */
static int xaf_deep(void) { extern int g_flat_outer_nparams; return g_emit.flat_deep_arrival || g_flat_outer_nparams >= 1; }   /* FLATDISP-5b/5c (s193): THE one outer-frame rbp condition — every gate site (anchor-leave, prologue save+seed, epilogue reload, BOTH media) reads THIS, never the raw field, so save/seed/read/restore cannot drift apart.  The nparams conjunct: the ICNBENCH-ARGS param-0 bind stores through [rbp+16]/[rbp+24], so an outer graph taking params REQUIRES the seed even when its own kinds are depth-static — before this, the TEXT arm gated the seed on the raw field but stored through rbp unconditionally (latent, unhit only because Icon mains carry scan/gen kinds). */
static std::string xaf_anchor_leave_bin(void) {
    if (!x86_port_cstack()) return std::string();
    if (x86_zc_frame() == ZC_FRAME_RSP && !xaf_deep()) return std::string();   /* FLATDISP-5c (s193): BINARY twin of the TEXT gate below — the bulk unwind reads the rbp seed, so a depth-static graph (which never seeded) must not emit it; LIFO balance has rsp at base already.  This line is WHY m3 held at 185 while m4 broke in s192: the TEXT arm was gated without this twin (BOTH-MEDIUM violation, recorded not hidden). */
    if (x86_zc_frame() == ZC_FRAME_RSP) return bytes(3, "\x48\x89\xEC");   /* mov rsp, rbp — ALIGN-INV-3 twin of the TEXT arm (as-form 48 89 EC); the s90 rsp-relative-read lesson lives in the TEXT arm comment */
    return xaf_frame_rsp_rm((uint32_t)xaf_anchor_off(), 0x8B);
}
static std::string xaf_anchor_enter_text(void) {
    if (!x86_port_cstack()) return std::string();
    if (x86_zc_frame() == ZC_FRAME_RSP) return std::string();   /* ALIGN-INV-3b: RSP leaves read rbp directly; no slot reader remains — store removed, regression is the falsifier (s142 NOFILL skip at the short-suffix unroll is now a stale micro-opt if this holds) */
    char b[160]; snprintf(b, sizeof b, "  mov qword ptr [%s + %d], rsp\n", x86_zr(), xaf_anchor_off()); return std::string(b);
}
static std::string xaf_anchor_leave_text(void) {
    if (!x86_port_cstack()) return std::string();
    if (x86_zc_frame() == ZC_FRAME_RSP && !xaf_deep()) return std::string();   /* FLATDISP-5 GATE (s193: condition unified into xaf_deep, see its comment): this reset is the BULK unwind that returns rsp to the activation base from whatever depth the arrival happened to land at.  When no arrival can be deep, the LIFO balance has already returned rsp to base and the reset is a provable no-op — so it is DELETED rather than re-spelled, and with it the last reader of rbp in a determinate graph.  FALSIFIABLE BY CONSTRUCTION: if any depth-static graph does arrive off-base this drops rsp reconstruction entirely and the crosscheck fails loudly at once, which is exactly the intended tripwire. */
    if (x86_zc_frame() == ZC_FRAME_RSP) return std::string("mov rsp, rbp\n");   /* ALIGN-INV-3: under RSP fb IS the snapshot — U1/U2/s79 seed rbp=base at every frame-active activation, so the s90 frame-base READ collapses to the register (the s90 hazard was rsp-RELATIVE reads at depth; reading rbp keeps its cure).  Enter store retained: s142 NOFILL dead-store-cut assumes it overwrites; reader sweep owed before retiring it. */
    char b[160]; snprintf(b, sizeof b, "mov rsp, qword ptr [%s + %d]\n", x86_zr(), xaf_anchor_off()); return std::string(b);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int xaf_nofill(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_PAT_NOFILL"); v = e ? (atoi(e) != 0) : 1; } return v; }   /* SPD-NOFILL (s139): default ON; SCRIP_PAT_NOFILL=0 restores the eager stosb (kill-switch, SCRIP_SLOT_ELIDE precedent) */
static int xaf_poison(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_ZLS_POISON"); v = e ? (atoi(e) != 0) : 0; } return v; }   /* SPD-NOFILL verification lane (emit-time twin of rt.c's lexprep2 lane): fill the SKIPPED grant span 0xA5 so any read-before-write surfaces loudly; m4 must be COMPILED with the env set for the lane to bake */
static int xaf_slot0_elide(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_SLOT0_ELIDE"); v = e ? (atoi(e) != 0) : 0; } return v; }   /* DB-2a (R1 alpha-diet): OMIT the slot0 16B seed entirely for flat_lex=0 jmp-entry citizens.  slot0 sits at frame offset 0 = the COLDEST end of a downward carve, so for every blob with kt >= 160 it is the ONLY reason cache line 0 is touched at all (census s156: all 31 PAT$ blobs across the 5 demos touch line 0 for exactly these two stores, 2-5 lines away from the suffix/continuation cluster at the top).  Default OFF until the poison lane below proves deadness per corpus. */
static int xaf_slot0_poison(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_SLOT0_POISON"); v = e ? (atoi(e) != 0) : 0; } return v; }   /* DB-2a LIVENESS LANE: seed slot0 with 0xA5A5A5A5A5A5A5A5 instead of 0 so any read-before-write of the NULVCL quad surfaces loudly instead of silently consuming an implicit zero.  Strictly more aggressive than the elision it licenses (elide leaves whatever the stack held, which can COINCIDENTALLY be zero and hide a reader; poison never can), so a green poison lane is the admissible proof for flipping SCRIP_SLOT0_ELIDE on. */
static int xaf_slot0_pad(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_SLOT0_PAD"); v = e ? (atoi(e) != 0) : 0; } return v; }   /* DB-2a CONTROL LANE: when eliding, replace the 17 removed bytes with 17 bytes of multi-byte nop so EVERY downstream byte keeps its address.  The elision removes two stores from a prologue that cannot plausibly cost 10% of wall on its own, so the raw ELIDE A/B measures (memory-touch saving + whole-blob code-layout shift) FUSED.  Padding holds layout constant, isolating the touch saving; ELIDE-vs-ELIDE+PAD then isolates the alignment artifact.  Without this control an alignment swing is indistinguishable from a real win — which is exactly how a 10% claws5 "regression" and a 6.5% treebank "gain" showed up in the same measurement. */
static std::string xaf_slot0_seed_bin(void) { if (xaf_slot0_elide()) return xaf_slot0_pad() ? bytes(8, "\x0F\x1F\x84\x00\x00\x00\x00\x00") + bytes(8, "\x0F\x1F\x84\x00\x00\x00\x00\x00") + bytes(1, "\x90") : std::string(); uint32_t w = xaf_slot0_poison() ? 0xA5A5A5A5u : 0u; return bytes(4, "\x48\xC7\x04\x24") + u32le(w) + bytes(5, "\x48\xC7\x44\x24\x08") + u32le(w); }   /* slot0 lo+hi seed, BINARY (8+9 = 17 bytes; the pad lane replaces them with 8+8+1 nop bytes).  imm32 sign-extends to 64 bits, so the poison word lands as 0xFFFFFFFFA5A5A5A5 — the TEXT twin spells that same imm as the signed decimal -1515870811 so `as` reproduces these exact bytes (R10 medium agreement). */
static std::string xaf_slot0_seed_text(void) { if (xaf_slot0_elide()) return xaf_slot0_pad() ? std::string("  .byte 0x0f,0x1f,0x84,0x00,0x00,0x00,0x00,0x00\n  .byte 0x0f,0x1f,0x84,0x00,0x00,0x00,0x00,0x00\n  .byte 0x90\n") : std::string(); char s[128]; snprintf(s, sizeof s, "  mov qword ptr [rsp], %s\n  mov qword ptr [rsp + 8], %s\n", xaf_slot0_poison() ? "-1515870811" : "0", xaf_slot0_poison() ? "-1515870811" : "0"); return std::string(s); }   /* slot0 lo+hi seed, TEXT twin of xaf_slot0_seed_bin — -1515870811 == (int32_t)0xA5A5A5A5, chosen so the assembler emits byte-for-byte what the BINARY arm hand-encodes; the pad lane spells the same 17 nop bytes the BINARY arm emits */
static std::string xaf_add_rdi_imm_bin(int n) { return (n >= -128 && n <= 127) ? bytes(3, "\x48\x83\xC7") + std::string(1, (char)(unsigned char)n) : bytes(3, "\x48\x81\xC7") + u32le((uint32_t)n); }   /* add rdi, imm — as-matching short form (imm8 when it fits), byte twin of the TEXT "add rdi, N" */
static int xaf_nofill_from(int sd, int rgn) { static int v = -2; if (v == -2) { const char *e = getenv("SCRIP_NOFILL_FROM"); v = e ? atoi(e) : -1; } if (v >= 0) { int n = v < 16 ? 16 : v; return n > rgn ? rgn : n; } return (sd >= 16 && sd < rgn) ? sd : rgn; }   /* SPD-NOFILL bisect knob: zero [FROM, region); default = seed suffix (rgn = zero nothing beyond slot0).  Diagnostic only — binary-search the offset that must stay zeroed to localize an implicit-zero consumer. */
static std::string xaf_zero_q_rsp_bin(int off) { return (off >= 0 && off <= 127) ? bytes(4, "\x48\xC7\x44\x24") + std::string(1, (char)(unsigned char)off) + u32le(0) : bytes(4, "\x48\xC7\x84\x24") + u32le((uint32_t)off) + u32le(0); }   /* mov qword ptr [rsp + off], 0 — as-matching disp8/disp32, byte twin of the TEXT spelling */
static std::string xa_flat_prologue_str(int & out_site, bb_label_t * & out_lbl, bool & out_def) {
    out_site = 0; out_lbl = nullptr; out_def = false;
    if (PLATFORM_X86) {
        if (MEDIUM_MACRO_DEF) return x86("comment", "# no macro form — XA_FLAT_PROLOGUE");
        if (MEDIUM_BINARY) {
            extern int g_frame_active;
            if (g_emit.flat_jmp_entry) {
                extern int g_emit_frame_caller_dl;
                if (!g_frame_active || g_emit_frame_caller_dl > 0) { fprintf(stderr, "FATAL xa_flat: jmp-entry blob must be frame-active with no display reg (frame_active=%d caller_dl=%d)\n", g_frame_active, g_emit_frame_caller_dl); abort(); }
                int kt = g_emit.flat_frame_bytes;
                if (kt < 48 || (kt & 15)) { fprintf(stderr, "FATAL xa_flat: jmp-entry K_total=%d (must be 16-mult >= 48: 32B wire header + region)\n", kt); abort(); }
                if (x86_zc_frame() == ZC_FRAME_RSP) {   /* R12-ERAD s65 BINARY twin: header above the frame.  REG-7 U5 (Lon FORTH ruling): pat blobs now take THIS arm too — rbp pinned at base, cells ride the one rsp stream; the legacy zr arm below serves non-RSP builds only */
                    std::string hdr = bytes(3, "\x48\x81\xEC") + u32le((uint32_t)kt)                   /* sub rsp, K_total */
                                    + bytes(4, "\x48\x89\x8C\x24") + u32le((uint32_t)(kt - 24))        /* mov [rsp + kt-24], rcx — outside-γ wire */
                                    + bytes(4, "\x48\x89\x94\x24") + u32le((uint32_t)(kt - 16))        /* mov [rsp + kt-16], rdx — outside-ω wire */
                                    + (emit_jmp_pin_rbp() ? bytes(4, "\x48\x89\xAC\x24") + u32le((uint32_t)(kt - 8))         /* FLATDISP-7 GATE (s194): mov [rsp + kt-8], rbp — REG-7 U2: caller rbp saved in the TOP header pad, ONLY when the seed below is about to clobber it (kt-8 picked over kt-32 to keep the pad adjacent to the region free; both were unused, FINDING s81 measurement 5).  Depth-static determinate graphs never clobber rbp — nothing to save, slot unwritten and unread. */
                                                            + bytes(3, "\x48\x89\xE5") : std::string());  /* mov rbp, rsp — REG-7 U2 seed: rbp = this activation's flat base (mirror of the s79 pat-blob save/seed at [rsp+24]/zr; restored on BOTH exit edges in the epilogue arm).  Seeded only when an arrival can be deep or the γ RETAINS (suspend record protocol) — emit_jmp_pin_rbp, the s193 xaf_deep discipline. */
                    if (g_emit.flat_lex && g_emit.flat_seed_off >= 16) {   /* PL-REGAIN-4 lazy arm: NO rep stosb — rt_jmp_frame_lexprep2 seeds slot0 + [suffix,region) only; box grants first-write-wins (SCRIP_ZLS_POISON=1 lane lives inside the leaf) */
                        xaf_mark("LEXPREP2");
                        std::string r = xaf_jmp_hdr_x86()
                                      + xaf_anchor_enter_x86()
                                      + x86("mov", "rdi", "rsp")                                          /* fb = the just-allocated region base (== rbp seed) */
                                      + x86("mov32", "esi", (long)g_emit.flat_seed_off)                   /* mov esi, suffix_off — NULVCL seed starts here (resume slot / zeta-mark / locals).  ⚠ "mov32" NOT "mov": the plain mov-imm dispatch is x86_movimm, which ALWAYS stamps REX.W + a full imm64 (`movabs rsi`), while the TEXT twin `mov esi, N` assembles to the 5-byte B8+r imm32 — that pair is a MEDIUM DIVERGENCE (contract §5 width trap).  x86_movimm32 is the as-matching form and reproduces the legacy BE+u32 exactly. */
                                      + x86("mov32", "edx", (long)(kt - 32))                              /* mov edx, K_region — seed/bind stops below the header; same mov32 reasoning as esi above */
                                      + x86("call", "rt_jmp_frame_lexprep2", (uint64_t)(uintptr_t)(void *)&rt_jmp_frame_lexprep2);   /* x86_call_ro IS the legacy pair verbatim — BINARY `48 B8 <ptr> FF D0` (movabs rax + call rax), TEXT ` call sym@PLT` — so the hand-encoded movabs/call and the separate TEXT snprintf collapse into ONE medium-invisible call (R2/R10).  rsp = base ≡ 0 mod 16 here (caller jmps at ≡0, kt is a 16-mult), the SysV pre-call parity; clobbers only caller-saved regs, the wires+rbp are already in the header. */
                        out_site = -1; out_lbl = nullptr; out_def = false;   /* XA-FLAT-CONVERT slice B: record stream — this arm carries NO external patch (it never had one; the legacy site was 0/nullptr), so the sentinel here buys the tag walker, not an 'X' record. */
                        return r;
                    }
                    if (!g_emit.flat_lex && !g_emit.flat_layout_unknown && xaf_nofill()) {   /* SPD-NOFILL (s139): flat_lex=0 jmp-entry citizens (PAT$ patprocs / LBL__ / EVAL-CODE chains / DYN procs) run NO lexprep and carry NO in-frame named locals — the region is box grants (first-write-wins by the four-port contract, rt.c:1402) + the [seed,region) suffix (resume slot / zeta-mark).  Zero ONLY slot0 + that suffix and SKIP the grant span: the blanket rep stosb was 63% of json-match's TOTAL Ir (255.7M/404M, FINDING s139).  Layout-unknown fallback graphs keep the eager arm below; SCRIP_PAT_NOFILL=0 kill-switch; SCRIP_ZLS_POISON=1 poisons the skipped span. */
                        int rgn = kt - 32, fz = xaf_nofill_from(g_emit.flat_seed_off, rgn);
                        std::string r = hdr + xaf_slot0_seed_bin();                                      /* DB-2a: slot0 lo+hi seed, now elidable/poisonable — see xaf_slot0_seed_bin */
                        if (xaf_poison() && fz > 16) r = r + bytes(3, "\x48\x89\xE7")                     /* mov rdi, rsp */
                                      + xaf_add_rdi_imm_bin(16)
                                      + bytes(1, "\xB9") + u32le((uint32_t)(fz - 16))                     /* ecx = skipped grant span */
                                      + bytes(2, "\xB0\xA5")                                              /* mov al, 0xA5 */
                                      + bytes(2, "\xF3\xAA");                                             /* rep stosb — poison lane */
                        for (int ci = 0; ci < g_emit.flat_cap_n; ci++) { int o = g_emit.flat_cap_off[ci]; if (o >= 16 && o + 16 <= rgn) r = r + xaf_zero_q_rsp_bin(o) + xaf_zero_q_rsp_bin(o + 8); }   /* CAP-NOFILL (s143): zero each 16B rt_cap grant (+0 buf, +8 gen|sp) individually — AFTER the poison fill so the lane still proves no other implicit-zero reader; see emit.cpp emit_jmp_entry_arm_region */
                        if (fz < rgn && rgn - fz <= 64) { for (int o = fz; o < rgn; o += 8) { if (x86_zc_frame() != ZC_FRAME_RSP && o == xaf_anchor_off()) continue; r = r + xaf_zero_q_rsp_bin(o); } }   /* SPD-NOFILL-B (s139): short suffix (the universal case — resume+mark quads, 32B) unrolls to plain quad zeros, eliminating the ~30-40 cycle REP startup per activation.  s142 DEAD-STORE CUT (the s141-proven free store): the anchor quad is unconditionally overwritten by xaf_anchor_enter appended immediately below (mov [rsp+anchor_off], rsp) with no intervening reader — skip its zero in BOTH twins; the guard is equality-based so an anchor outside [fz,rgn) leaves the unroll untouched */
                        else if (fz < rgn) r = r + bytes(3, "\x48\x89\xE7")                                /* mov rdi, rsp */
                                      + xaf_add_rdi_imm_bin(fz)
                                      + bytes(1, "\xB9") + u32le((uint32_t)(rgn - fz))                    /* ecx = suffix span */
                                      + bytes(2, "\x31\xC0")                                              /* xor eax, eax */
                                      + bytes(2, "\xF3\xAA");                                             /* rep stosb — resume/zeta-mark suffix stays zeroed */
                        r = r + xaf_anchor_enter_bin();
                        xaf_mark("NOFILL");
                        out_site = 0; out_lbl = nullptr; out_def = false;
                        return r;
                    }
                    std::string r = hdr
                                  + bytes(3, "\x48\x89\xE7")                                            /* mov rdi, rsp */
                                  + bytes(1, "\xB9") + u32le((uint32_t)(kt - 32))                         /* mov ecx, K_region — stosb stops below the header */
                                  + bytes(2, "\x31\xC0")
                                  + bytes(2, "\xF3\xAA")
                                  + xaf_anchor_enter_bin()
                                  + (g_emit.flat_lex ? bytes(3, "\x48\x89\xE7")                            /* mov rdi, rsp — NCB-1d lexprep: fb = the just-allocated region base (== rbp seed) */
                                                     + bytes(1, "\xBE") + u32le((uint32_t)(kt - 32))       /* mov esi, K_region — fill/bind stops below the header */
                                                     + bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void *)&rt_jmp_frame_lexprep)   /* movabs rax, rt_jmp_frame_lexprep — raw bytes, NOT x86_movabs_r64: this arm is the legacy raw-byte family, an L-record would corrupt the stream */
                                                     + bytes(2, "\xFF\xD0")                                /* call rax — rsp = base ≡ 0 mod 16 here (caller jmps at ≡0, kt is a 16-mult), the SysV pre-call parity; clobbers only caller-saved regs, the wires+rbp are already in the header */
                                                     : std::string());
                    xaf_mark("JMP_EAGER");
                    out_site = 0; out_lbl = nullptr; out_def = false;
                    return r;
                }
                std::string r = bytes(3, "\x48\x81\xEC") + u32le((uint32_t)kt)                         /* sub rsp, K_total — the activation self-allocates */
                              + bytes(4, "\x48\x89\x4C\x24") + std::string(1, (char)0x08)               /* mov [rsp+8],  rcx — outside-γ wire */
                              + bytes(4, "\x48\x89\x54\x24") + std::string(1, (char)0x10)               /* mov [rsp+16], rdx — outside-ω wire */
                              + xaf_store_zr_rsp8(24)                                                    /* mov [rsp+24], zr — caller's frame reg ([rsp+0] = pad; β resumes ride the γ-pushed frontier record, not the header) */
                              + xaf_lea_zr_rsp8(32)                                                      /* zr = rsp+32 — region base; every FR() offset unchanged */
                              + xaf_mov_rdi_zr()
                   + bytes(1, "\xB9") + u32le((uint32_t)(kt - 32))                                       /* mov ecx, K_region */
                   + bytes(2, "\x31\xC0")                                                                /* xor eax, eax */
                   + bytes(2, "\xF3\xAA")                                                                /* rep stosb — zls_alloc zeroed (ZC_INIT_ZERO); the activation must too */
                   + xaf_anchor_enter_bin();
                xaf_mark("JMP_NONRSP");
                out_site = 0; out_lbl = nullptr; out_def = false;
                return r;
            }
            if (g_frame_active) {
                extern int g_emit_frame_caller_dl;
                if (x86_zc_frame() == ZC_FRAME_RSP) { /* R12-ERAD: FORTH self-alloc, byte twin of the TEXT arm — sub rsp,K / mov rdi,rsp / mov ecx,K / xor eax,eax / rep stosb */
                    extern int g_gen_proc_active; extern int g_resumable_callable_active; extern int g_flat_outer_nparams;
                    if (g_gen_proc_active || g_resumable_callable_active) {   /* PL-GEN-RSP fix: a generator/resumable proc's frame MUST survive beta-resume, so it lives on the heap (rt_proc_call_gen_h -> rt_zls_alloc) and arrives in rdi; the box ADOPTS it as its rbp frame base (args already bound at [rbp+16]+, region pre-zeroed) instead of carving a throwaway rsp frame that discards the fb.  Restores the pre-s65 non-RSP `mov zr,rdi` the RSP conversion dropped when it applied the outer-graph self-alloc prologue to gen procs.  rsp stays free; both alpha and beta re-enter through this prefix. */
                        std::string rg = bytes(1, "\x55") + bytes(3, "\x48\x89\xFD")
                                       + bytes(3, "\x83\xFE\x00") + bytes(2, "\x0F\x85") + u32le(0);
                        xaf_mark("GEN_RESUMABLE");
                        out_site = (int)rg.size() - 4; out_lbl = g_emit.flat_β_p; out_def = false;
                        return rg;
                    }
                    int K = xaf_outer_frame_k();
                    std::string r = x86("sub", "rsp", (long)K)
                                  + x86("mov", "rdi", "rsp")
                                  + x86("mov32", "ecx", (long)K)   /* XA-FLAT-CONVERT slice B fix: was x86("mov",...) — that dispatches to x86_movimm, which unconditionally stamps REX.W + imm64 (`movabs rcx`, 10 bytes) while this arm's TEXT twin `mov ecx, K` assembles to the 5-byte B8+r imm32.  Verified with as: `mov esi,-1` -> be ff ff ff ff (rsi = 4294967295) vs `movabs rsi,-1` -> 48 be ff.. (rsi = -1), so the pair is a BYTE divergence at every site and a SEMANTIC one for any negative immediate (R10 "same REX.W width").  Harmless HERE only because K = xaf_outer_frame_k() >= 65544 is positive; x86_movimm32 is the as-matching form. */
                                  + x86("xor", "eax", "eax")
                                  + x86("rep_stosb")
                                  + xaf_anchor_enter_x86()
                                  + (x86_port_mode() == ZC_PORT_HEAP ? x86("mov", "rbx", (std::string("qword ptr [") + std::to_string((unsigned long)RT_WS_TOP) + "]").c_str()) : std::string()) /* REG-4b OUTER SEED: outermost graphs only — jmp-entry blobs inherit rbx live */
                                  + (xaf_deep() ? x86("mov", (std::string("qword ptr [rsp + ") + std::to_string(K - 8) + "]").c_str(), "rbp") : std::string()) /* FLATDISP-5c (s193) GATE, twin of the TEXT arm: CALLER-RBP SAVE (ABI fix, mode-3): store caller rbp at [rsp+K-8] header slot before the seed clobbers rbp; restored before every ret.  Depth-static graphs never clobber rbp — nothing to save. */
                                  + (xaf_deep() ? x86("mov", "rbp", "rsp") : std::string()) /* FLATDISP-5c (s193) GATE, twin of the TEXT arm: REG-7 U1 OUTER SEED: rbp = the activation FLAT BASE (REG-MAP law 1) — rsp == flat base here by LIFO balance.  Seeded ONLY when an arrival can land deep (or the param-0 bind below stores through it — the xaf_deep nparams conjunct). */
                                  + (g_flat_outer_nparams >= 1 ? x86("push", "rsi") + x86("sub", "rsp", (long)8) /* ICNBENCH-ARGS-RSP: push rsi (entry selector survives the call) + sub rsp,8 (base = 0 mod 16 -> call-parity 0 kept) */
                                                              + x86("movabs", "rax", (unsigned long)(uintptr_t)(void *)&rt_main_args_fetch)
                                                              + x86("call", "rax") /* staged args DESCR returns in rax:rdx */
                                                              + x86("add", "rsp", (long)8) + x86("pop", "rsi")
                                                              + x86("mov", "qword ptr [rbp + 16]", "rax") + x86("mov", "qword ptr [rbp + 24]", "rdx") /* param-0 slot; runs on beta re-entry too (rep stosb rezeroed the frame) */
                                                              : std::string())
                                  + x86("cmp", "esi", (long)0)
                                  + x86("jne", "extlbl", (unsigned long)(uintptr_t)g_emit.flat_β_p);
                    xaf_mark("FRAME_RSP");
                    out_site = -1; out_lbl = nullptr; out_def = false;   /* XA-FLAT-CONVERT slice A: record stream, patch site discovered by bb_emit_x86 via the 'X' record — the hand-counted r.size()-4 is retired here. */
                    return r;
                }
                std::string disp;
                if      (g_emit_frame_caller_dl == 1) disp = bytes(2, "\x41\x55") + xaf_mov_dreg_frame(13) + bytes(4, "\x48\x83\xEC\x08");
                else if (g_emit_frame_caller_dl == 2) disp = bytes(2, "\x41\x56") + xaf_mov_dreg_frame(14) + bytes(4, "\x48\x83\xEC\x08");
                else if (g_emit_frame_caller_dl == 3) disp = bytes(2, "\x41\x57") + xaf_mov_dreg_frame(15) + bytes(4, "\x48\x83\xEC\x08");
                std::string r = xaf_push_frame()
                              + xaf_mov_frame_rdi()
                              + disp
                              + xaf_anchor_enter_bin()
                              + bytes(3, "\x83\xFE\x00")
                              + bytes(2, "\x0F\x85") + u32le(0);
                xaf_mark("FRAME_NONRSP");
                out_site = (int)r.size() - 4; out_lbl = g_emit.flat_β_p; out_def = false;
                return r;
            }
            xaf_mark("BARE");
            out_site = -1; out_lbl = nullptr; out_def = false;   /* XA-FLAT-CONVERT slice A: first arm off the raw-byte family.  The hand-counted out_site=9 (4+3+2 bytes before the rel32) is gone — the 'X' record carries the flat_β_p pointer and bb_emit_x86 discovers the patch site as it walks, so the offset cannot drift when an encoder picks a different short form. */
            return x86("sub", "rsp", (long)8)
                 + x86("cmp", "esi", (long)0)
                 + x86("jne", "extlbl", (unsigned long)(uintptr_t)g_emit.flat_β_p);
        }
        if (MEDIUM_TEXT) {
            if (!g_is_text) return std::string();
            std::string banner;
            if (g_emit.flat_text_externalise) {
                banner = std::string("    .global ") + (g_emit.flat_lbl_α    ? g_emit.flat_lbl_α    : "") + "\n"
                       + "    .global " + (g_emit.flat_lbl_β    ? g_emit.flat_lbl_β    : "") + "\n"
                       + "    .global " + (g_emit.flat_lbl_γ ? g_emit.flat_lbl_γ : "") + "\n"
                       + "    .global " + (g_emit.flat_lbl_ω ? g_emit.flat_lbl_ω : "") + "\n";
            }
            extern int g_frame_active;
            extern int g_gen_proc_active;
            extern int g_resumable_callable_active;
            if (g_emit.flat_wired) {
                return banner;
            }
            if (g_emit.flat_jmp_entry) {
                int kt = g_emit.flat_frame_bytes;
                if (x86_zc_frame() == ZC_FRAME_RSP) {   /* R12-ERAD s65 (+ REG-7 U5: pat blobs unified onto this arm — Lon FORTH ruling): HEADER ABOVE THE FRAME — [E-32, E) holds the wires, frame = [base, E-32), rsp = base.  The old header-below layout put the wires
                     * directly under rsp, where the FIRST push (a HEAD cell, a callee's sub) clobbered them — recursive procs and any pattern inside a proc died exactly there.  Wires at
                     * [rsp + kt-24/-16]; the rep stosb count kt-32 stops below the header; the LIFO exits unwind the full kt.  Pat blobs fall through to the zr arms (zr = r12 for them). */
                    if (g_emit.flat_lex && g_emit.flat_seed_off >= 16) {   /* PL-REGAIN-4 lazy arm (TEXT twin): NO rep stosb — rt_jmp_frame_lexprep2 seeds slot0 + [suffix,region) only; box grants first-write-wins (SCRIP_ZLS_POISON=1 lane lives inside the leaf) */
                        char b2[640];
                        snprintf(b2, sizeof b2,
                            "  sub rsp, %d\n  mov [rsp + %d], rcx\n  mov [rsp + %d], rdx\n",
                            kt, kt - 24, kt - 16);
                        if (emit_jmp_pin_rbp()) { char rp2[96]; snprintf(rp2, sizeof rp2, "  mov [rsp + %d], rbp\n  mov rbp, rsp\n", kt - 8); strncat(b2, rp2, sizeof b2 - strlen(b2) - 1); }   /* FLATDISP-7 GATE (s194): caller-rbp save + seed only when pinned — TEXT twin of the BINARY hdr gate, same predicate (emit_jmp_pin_rbp) */
                        char lx2[192];
                        snprintf(lx2, sizeof lx2, "  mov rdi, rsp\n  mov esi, %d\n  mov edx, %d\n  call rt_jmp_frame_lexprep2@PLT\n", g_emit.flat_seed_off, kt - 32);   /* seed suffix + region; rsp = base ≡ 0 mod 16, the SysV pre-call parity */
                        return banner + b2 + xaf_anchor_enter_text() + lx2;
                    }
                    if (!g_emit.flat_lex && !g_emit.flat_layout_unknown && xaf_nofill()) {   /* SPD-NOFILL TEXT twin (s139) — see the BINARY arm: slot0 + suffix zero only, grant span skipped (first-write-wins), poison lane under SCRIP_ZLS_POISON=1 */
                        int rgn = kt - 32, fz = xaf_nofill_from(g_emit.flat_seed_off, rgn);
                        char b3[640];
                        snprintf(b3, sizeof b3,
                            "  sub rsp, %d\n  mov [rsp + %d], rcx\n  mov [rsp + %d], rdx\n",
                            kt, kt - 24, kt - 16);
                        if (emit_jmp_pin_rbp()) { char rp3[96]; snprintf(rp3, sizeof rp3, "  mov [rsp + %d], rbp\n  mov rbp, rsp\n", kt - 8); strncat(b3, rp3, sizeof b3 - strlen(b3) - 1); }   /* FLATDISP-7 GATE (s194): caller-rbp save + seed only when pinned — TEXT twin of the BINARY hdr gate, same predicate (emit_jmp_pin_rbp) */
                        std::string r3 = std::string(b3) + xaf_slot0_seed_text();                        /* DB-2a TEXT twin — see the BINARY arm */
                        if (xaf_poison() && fz > 16) { char p3[200]; snprintf(p3, sizeof p3, "  mov rdi, rsp\n  add rdi, 16\n  mov ecx, %d\n  mov al, 0xA5\n  rep stosb\n", fz - 16); r3 += p3; }
                        for (int ci = 0; ci < g_emit.flat_cap_n; ci++) { int o = g_emit.flat_cap_off[ci]; if (o >= 16 && o + 16 <= rgn) { char c3[96]; snprintf(c3, sizeof c3, "  mov qword ptr [rsp + %d], 0\n  mov qword ptr [rsp + %d], 0\n", o, o + 8); r3 += c3; } }   /* CAP-NOFILL (s143) TEXT twin — see the BINARY arm */
                        if (fz < rgn && rgn - fz <= 64) { for (int o = fz; o < rgn; o += 8) { if (x86_zc_frame() != ZC_FRAME_RSP && o == xaf_anchor_off()) continue; char q3[64]; snprintf(q3, sizeof q3, "  mov qword ptr [rsp + %d], 0\n", o); r3 += q3; } }   /* SPD-NOFILL-B TEXT twin — see the BINARY arm (s142 dead-store cut mirrored: the anchor quad is re-stored by anchor_enter immediately below) */
                        else if (fz < rgn) { char s3[200]; snprintf(s3, sizeof s3, "  mov rdi, rsp\n  add rdi, %d\n  mov ecx, %d\n  xor eax, eax\n  rep stosb\n", fz, rgn - fz); r3 += s3; }
                        return banner + r3 + xaf_anchor_enter_text();
                    }
                    char b[640];
                    snprintf(b, sizeof b,
                        "  sub rsp, %d\n  mov [rsp + %d], rcx\n  mov [rsp + %d], rdx\n",
                        kt, kt - 24, kt - 16);
                    if (emit_jmp_pin_rbp()) { char rpe[96]; snprintf(rpe, sizeof rpe, "  mov [rsp + %d], rbp\n  mov rbp, rsp\n", kt - 8); strncat(b, rpe, sizeof b - strlen(b) - 1); }   /* FLATDISP-7 GATE (s194): caller-rbp save + seed only when pinned — TEXT twin of the BINARY hdr gate, same predicate (emit_jmp_pin_rbp) */
                    { char zf[128]; snprintf(zf, sizeof zf, "  mov rdi, rsp\n  mov ecx, %d\n  xor eax, eax\n  rep stosb\n", kt - 32); strncat(b, zf, sizeof b - strlen(b) - 1); }
                    char lx[160]; lx[0] = 0;
                    if (g_emit.flat_lex) snprintf(lx, sizeof lx, "  mov rdi, rsp\n  mov esi, %d\n  call rt_jmp_frame_lexprep@PLT\n", kt - 32);   /* NCB-1d TEXT twin of the BINARY lexprep tail: NULVCL fill + arg-bind on the self-allocated frame; rsp = base ≡ 0 mod 16, the SysV pre-call parity */
                    return banner + b + xaf_anchor_enter_text() + lx;
                }
                char b[640];
                snprintf(b, sizeof b,
                    "  sub rsp, %d\n  mov [rsp+8], rcx\n  mov [rsp+16], rdx\n  mov [rsp+24], %s\n  lea %s, [rsp+32]\n  mov rdi, %s\n  mov ecx, %d\n  xor eax, eax\n  rep stosb\n",
                    kt, x86_zr(), x86_zr(), x86_zr(), kt - 32);
                return banner + b + xaf_anchor_enter_text();
            }
            if (g_frame_active) {
                extern int g_emit_frame_caller_dl;
                if (x86_zc_frame() == ZC_FRAME_RSP) { /* R12-ERAD: FORTH — blob self-allocates on the C stack; ret-addr sits at [rsp+K], above every FR offset; C calls clobber only below rsp */
                    if (g_gen_proc_active || g_resumable_callable_active)   /* PL-GEN-RSP fix (TEXT twin): adopt the heap fb (rdi) as the rbp frame base; frame survives beta-resume on the heap, rsp stays free */
                        return banner + "  push rbp\n  mov rbp, rdi\n  cmp esi, 0\n  jne " + (g_emit.flat_lbl_β ? g_emit.flat_lbl_β : "?") + "\n";
                    int K = xaf_outer_frame_k();
                    char fb[224]; snprintf(fb, sizeof fb, "  sub rsp, %d\n  mov rdi, rsp\n  mov ecx, %d\n  xor eax, eax\n  rep stosb\n", K, K); /* K = region+8 (floored 65544); 8 phase pad: entry rsp is 8 mod 16, base must land 0 mod 16 = the R12-mode body parity every bare template call assumes */
                    std::string pro = banner + fb + xaf_anchor_enter_text();
                    if (x86_port_mode() == ZC_PORT_HEAP) pro += std::string("  mov rbx, ") + ABSQ(RT_WS_TOP) + "\n"; /* REG-4b OUTER SEED (twin of the BINARY arm's 48 8B 1C 25): outermost graphs only — jmp-entry blobs inherit rbx live through C's callee-saved contract */
                    if (xaf_deep()) { char rbps[64]; snprintf(rbps, sizeof rbps, "  mov [rsp + %d], rbp\n", K - 8); pro += std::string(rbps); } /* FLATDISP-5 GATE: the caller-rbp header save exists ONLY to protect the register the seed below is about to clobber — no seed, nothing to protect, so the depth-static arm drops BOTH and leaves rbp untouched (a free GPR for the whole activation).  CALLER-RBP SAVE (ABI fix, twin of BINARY 48 89 AC 24): store caller rbp at [rsp+K-8] header slot before the seed clobbers it; restored before every ret */
                    if (xaf_deep()) pro += std::string("  mov rbp, rsp\n"); /* FLATDISP-5 GATE: seed the depth-immune base ONLY when an arrival can be deep; a depth-static graph recovers the base by arithmetic on rsp alone.  REG-7 U1 OUTER SEED (twin of the BINARY arm's 48 89 E5): rbp = activation flat base per REG-MAP law 1 — dead until U3's consumer flip; blobs save/seed/restore their own (s79 pat, U2 non-pat).  Bare clobber safe: the dance restores callee-saved regs, same as rbx/r12 above. */
                    { extern int g_flat_outer_nparams; if (g_flat_outer_nparams >= 1) pro += std::string("  push rsi\n  sub rsp, 8\n  call rt_main_args_fetch@PLT\n  add rsp, 8\n  pop rsi\n  mov [rbp + 16], rax\n  mov [rbp + 24], rdx\n"); } /* ICNBENCH-ARGS-RSP TEXT twin: bind staged main(args) into the param-0 slot; esi preserved, rsp call-parity 0 kept */
                    if (g_gen_proc_active || g_resumable_callable_active)
                        pro += std::string("  cmp esi, 0\n") + "  jne " + (g_emit.flat_lbl_β ? g_emit.flat_lbl_β : "?") + "\n";
                    return pro;
                }
                const char *dreg = (g_emit_frame_caller_dl == 1) ? "r13" : (g_emit_frame_caller_dl == 2) ? "r14" : (g_emit_frame_caller_dl == 3) ? "r15" : (const char *)0;
                std::string disp = dreg ? (std::string("  push ") + dreg + "\n  mov " + dreg + ", " + x86_zr() + "\n  sub rsp, 8\n") : std::string();
                std::string pro = banner + "push " + std::string(x86_zr()) + "\n  mov " + x86_zr() + ", rdi\n" + disp + xaf_anchor_enter_text();
                if (g_gen_proc_active || g_resumable_callable_active)
                    pro += std::string("  cmp esi, 0\n")
                         + "  jne " + (g_emit.flat_lbl_β ? g_emit.flat_lbl_β : "?") + "\n";
                return pro;
            }
            return banner
                 + "  cmp esi, 0\n"
                 + "  je "  + (g_emit.flat_lbl_α_body ? g_emit.flat_lbl_α_body : "?") + "\n"
                 + "  jmp " + (g_emit.flat_lbl_β      ? g_emit.flat_lbl_β      : "?") + "\n";
        }
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_flat_epilogue_str(int & out_site, bb_label_t * & out_lbl, bool & out_def, std::string * out_succ, std::string * out_fail) {
    out_site = 0; out_lbl = nullptr; out_def = false;
    if (out_succ) out_succ->clear();
    if (out_fail) out_fail->clear();
    if (PLATFORM_X86) {
        if (MEDIUM_MACRO_DEF) return x86("comment", "# no macro form — XA_FLAT_EPILOGUE");
        if (MEDIUM_BINARY) {
            extern int g_frame_active;
            extern int g_emit_frame_caller_dl;
            if (g_emit.flat_jmp_entry) {
                /* ZS-2 (Lon s58/s59): NO ret, NO eax code — the landing label IS the signal.  γ-half: the chain
                 * succeeded with rsp at the DEEP frontier (interior suspended cells live below), so first leave
                 * the 16B RESUME RECORD at the frontier — push the frame reg, push the β-landing address — then
                 * read the wire header through the PINNED frame reg (zr = region base; header at zr-32): the
                 * whole activation is RETAINED and control jmps through the outside-γ wire with the caller's
                 * frame reg restored.  The outside backtrack edge `jmp qword [rsp+0]` reads the record's
                 * landing word.  ω-half: reached with rsp back at this activation's own header base (the
                 * every-ω-pops law), so add rsp,K_total kills header+region and control jmps through the
                 * outside-ω wire.  disp8s from zr: β wire -32, γ wire -24, ω wire -16, saved zr -8.
                 * CONVENTION (wrapper contract): out_succ = γ piece A ending at the lea rel32 patch site
                 * (out_site/out_lbl/out_def describe THAT patch); the RETURN VALUE is γ piece B; out_fail =
                 * the ω-half. */
                int kt = g_emit.flat_frame_bytes;
                if (x86_zc_frame() == ZC_FRAME_RSP && (g_emit.flat_pat || g_emit.flat_gen)) {   /* REG-7 U5 (Lon FORTH ruling) BINARY twin: pat-blob SUSPEND exits on the rbp/header-above protocol; GENP-SPINE (Lon "generator procedures on the main spine", 2026-07-17 s92) widens the arm to flat_gen — generator procs suspend on the ONE ζ stack, LIFO law, no per-instance coexpr stack.  γ retains the activation — 16B resume record {landing, rbp} pushed at the DEEP frontier, wires read through the PINNED rbp ([rbp+kt-24/-16], depth-immune), caller rbp restored self-referentially [rbp+kt-8] (U2b discipline); flat_gen ADDITIONALLY preloads the result DESCR into rdi:rsi pre-record (frame slot 0, bb_suspend/bb_return's write — the det delivery protocol the caller's γ landing expects).  ω unwinds ABSOLUTELY: lea rsp,[rbp+kt] rejoins pre-entry rsp at ANY arrival depth (seal cuts), reads-before-motion (rax and the lea both read rbp before its restore). */
                    std::string succA = (g_emit.flat_gen ? bytes(4, "\x48\x8B\x7D\x00")      /* mov rdi, [rbp] — result word0 via pinned base (flat_gen only) */
                                                         + bytes(4, "\x48\x8B\x75\x08")      /* mov rsi, [rbp + 8] — result word1 */
                                                         : std::string())
                                      + bytes(1, "\x55")                                     /* push rbp — record payload: flat base */
                                      + bytes(3, "\x48\x8D\x05");                            /* lea rax, [rip + res-landing] */
                    out_site = (int)succA.size(); out_lbl = g_emit.flat_res_p; out_def = false;
                    succA += u32le(0);
                    std::string succB = bytes(1, "\x50")                                     /* push rax — record top: the landing word */
                                      + bytes(3, "\x48\x8B\x85") + u32le((uint32_t)(kt - 24))   /* mov rax, [rbp + kt-24] — outside-γ wire via pinned base */
                                      + bytes(3, "\x48\x8B\xAD") + u32le((uint32_t)(kt - 8))    /* mov rbp, [rbp + kt-8] — caller restore, self-referential (U2b) */
                                      + bytes(2, "\xFF\xE0");                                /* jmp rax */
                    std::string fo = bytes(3, "\x48\x8B\x85") + u32le((uint32_t)(kt - 16))      /* mov rax, [rbp + kt-16] — outside-ω wire */
                                   + bytes(3, "\x48\x8D\xA5") + u32le((uint32_t)kt)              /* lea rsp, [rbp + kt] — ABSOLUTE unwind to pre-entry rsp */
                                   + bytes(3, "\x48\x8B\xAD") + u32le((uint32_t)(kt - 8))        /* mov rbp, [rbp + kt-8] — caller restore */
                                   + bytes(2, "\xFF\xE0");
                    if (out_succ) *out_succ = succA;
                    if (out_fail) *out_fail = fo;
                    return succB;
                }
                if (x86_zc_frame() == ZC_FRAME_RSP && !emit_jmp_pin_rbp()) {   /* FLATDISP-7 (s194): DEPTH-STATIC determinate exits — the classifier proved every arrival lands with rsp == base (LIFO balance), which is EXACTLY the every-ω-pops guarded assumption the s90 comment below records as "true for SNOBOL4's determinate procs"; the rbp-absolute form was adopted because Icon procs falsified it, and the per-graph classifier now separates the two language-blind.  All reads rsp-relative, teardown = one add, NO rbp touch anywhere (never saved, never seeded, never restored — a free GPR for the whole activation).  FALSIFIABLE BY CONSTRUCTION: an off-base arrival here reads garbage wires and the crosscheck fails loudly at once — the s193 tripwire discipline. */
                    std::string sg = xaf_ld64_rsp(7, 0)                                    /* mov rdi, [rsp] — result DESCR word0 (frame slot 0, IR_RETURN's write); rsp == base */
                                   + xaf_ld64_rsp(6, 8)                                    /* mov rsi, [rsp + 8] — result word1 */
                                   + xaf_ld64_rsp(0, kt - 24)                              /* mov rax, [rsp + kt-24] — γ wire at the header, base-relative */
                                   + xaf_addq_rsp(kt)                                      /* add rsp, kt — the ENTIRE teardown (as-matching imm8/imm32) */
                                   + bytes(2, "\xFF\xE0");                                 /* jmp rax */
                    std::string fo = xaf_ld64_rsp(0, kt - 16)                              /* mov rax, [rsp + kt-16] — ω wire */
                                   + xaf_addq_rsp(kt)
                                   + bytes(2, "\xFF\xE0");
                    out_site = 0; out_lbl = (bb_label_t *)0; out_def = false;
                    if (out_succ) *out_succ = sg;
                    if (out_fail) *out_fail = fo;
                    return std::string();
                }
                if (x86_zc_frame() == ZC_FRAME_RSP) {   /* R12-ERAD s65 → NCB-1d (s90): DETERMINATE exits, DEPTH-IMMUNE rbp-absolute form (the pat-arm ω discipline applied to BOTH edges).  The old rsp-relative reads ([rsp+0/8], [rsp+kt-24/-16], lea rsp,[rsp+kt]) rode the every-ω-pops
                     * GUARDED ASSUMPTION (line-429 comment) that arrivals land with rsp == base — true for SNOBOL4's determinate procs, FALSE for Icon: `return expr` from inside nested generator/scan depth arrives DEEP.  All reads now go through the PINNED rbp (== base, the U2 seed),
                     * the unwind is ABSOLUTE lea rsp,[rbp+kt], reads-before-motion (γ wire + result + lea all read rbp before its self-referential restore).  Byte-different, semantics-identical on at-base arrivals — the SN4 crosscheck watermark is the tripwire.  FLATDISP-7 (s194): this arm now serves PINNED graphs only (deep arrivals or suspend protocols) — the depth-static arm above restores the cheap form for exactly the graphs where the assumption was always sound. */
                    std::string sg = bytes(4, "\x48\x8B\x7D\x00")                          /* mov rdi, [rbp] — result DESCR word0 via the pinned base (frame slot 0, IR_RETURN's write) */
                                   + bytes(4, "\x48\x8B\x75\x08")                          /* mov rsi, [rbp+8] — result word1 */
                                   + bytes(3, "\x48\x8B\x85") + u32le((uint32_t)(kt - 24))   /* mov rax, [rbp + kt-24] — γ wire via base */
                                   + bytes(3, "\x48\x8D\xA5") + u32le((uint32_t)kt)          /* lea rsp, [rbp + kt] — ABSOLUTE unwind to pre-entry rsp at ANY arrival depth */
                                   + bytes(3, "\x48\x8B\xAD") + u32le((uint32_t)(kt - 8))    /* mov rbp, [rbp + kt-8] — caller restore, self-referential (U2b), LAST rbp read */
                                   + bytes(2, "\xFF\xE0");                                 /* jmp rax */
                    std::string fo = bytes(3, "\x48\x8B\x85") + u32le((uint32_t)(kt - 16))   /* mov rax, [rbp + kt-16] — ω wire via base */
                                   + bytes(3, "\x48\x8D\xA5") + u32le((uint32_t)kt)          /* lea rsp, [rbp + kt] — ABSOLUTE unwind */
                                   + bytes(3, "\x48\x8B\xAD") + u32le((uint32_t)(kt - 8))    /* mov rbp, [rbp + kt-8] — caller restore */
                                   + bytes(2, "\xFF\xE0");
                    out_site = 0; out_lbl = (bb_label_t *)0; out_def = false;
                    if (out_succ) *out_succ = sg;
                    if (out_fail) *out_fail = fo;
                    return std::string();
                }
                std::string succA = xaf_push_frame()                                       /* push zr — record payload: region base */
                                  + bytes(3, "\x48\x8D\x05");                              /* lea rax, [rip + res-landing] */
                out_site = (int)succA.size(); out_lbl = g_emit.flat_res_p; out_def = false;
                succA += u32le(0);
                std::string succB = bytes(1, "\x50")                                       /* push rax — record top: the landing word */
                                  + xaf_ld64_from_zr8(0, -24)                              /* mov rax, [zr-24] — outside-γ wire */
                                  + xaf_ld64_from_zr8(x86_zr_num(), -8)                    /* mov zr, [zr-8] — caller's frame reg */
                                  + bytes(2, "\xFF\xE0");                                  /* jmp rax */
                std::string fail_half = xaf_ld64_from_zr8(0, -16)                          /* mov rax, [zr-16] — outside-ω wire */
                                      + xaf_lea_rsp_zr32(kt - 32)                            /* lea rsp, [zr + K_total-32] — ABSOLUTE unwind (pre-entry rsp = header base + K_total); fence/ABORT seal cuts arrive here at ANY depth, so never add-relative */
                                      + xaf_ld64_from_zr8(x86_zr_num(), -8)
                                      + bytes(2, "\xFF\xE0");
                if (out_succ) *out_succ = succA;
                if (out_fail) *out_fail = fail_half;
                return succB;
            }
            if (g_frame_active && x86_zc_frame() == ZC_FRAME_RSP) { /* R12-ERAD: BINARY twin of the TEXT RSP epilogue — eax code, anchor-leave normalize, FAILDESCR at [rsp+0], add rsp,K, ret */
                extern int g_gen_proc_active; extern int g_resumable_callable_active;
                if (g_gen_proc_active || g_resumable_callable_active) {   /* PL-GEN-RSP fix: gen/resumable frame is the heap fb in rbp — result (IR_RETURN) and FAILDESCR land at [rbp+0]; pop caller rbp and ret, NO add rsp (the gen prologue carved no rsp frame) */
                    std::string sg = bytes(1, "\x5D") + bytes(1, "\xC3");
                    std::string fg = bytes(3, "\xC7\x45\x00") + u32le(99) + bytes(3, "\xC7\x45\x04") + u32le(0) + bytes(4, "\x48\xC7\x45\x08") + u32le(0) + bytes(1, "\x5D") + bytes(1, "\xC3");
                    out_site = (int)sg.size(); out_lbl = g_emit.flat_fail_p; out_def = true;
                    if (out_succ) *out_succ = sg;
                    if (out_fail) *out_fail = fg;
                    return sg + fg;
                }
                int Ke = xaf_outer_frame_k();
                std::string succ_half = bytes(1, "\xB8") + u32le(1)
                                      + bytes(2, "\x31\xD2")
                                      + xaf_anchor_leave_bin()
                                      + (xaf_deep() ? bytes(4, "\x48\x8B\xAC\x24") + u32le((uint32_t)(Ke - 8)) : std::string()) /* FLATDISP-5c (s193) GATE, twin of the TEXT rbpr: CALLER-RBP RESTORE (ABI fix): rsp==base after anchor-leave, reload caller rbp from [rsp+K-8] header slot before rejoining the caller.  Depth-static graphs never saved — slot unwritten, reload deleted. */
                                      + bytes(3, "\x48\x81\xC4") + u32le((uint32_t)Ke)
                                      + bytes(1, "\xC3");
                std::string fail_half = xaf_anchor_leave_bin()
                                      + bytes(1, "\xB8") + u32le(99)
                                      + bytes(2, "\x31\xD2")
                                      + (xaf_deep() ? bytes(4, "\x48\x8B\xAC\x24") + u32le((uint32_t)(Ke - 8)) : std::string()) /* FLATDISP-5c (s193) GATE, twin of the TEXT rbpr: CALLER-RBP RESTORE (ABI fix): reload caller rbp from [rsp+K-8] before rejoining the caller — deleted for depth-static graphs, matching the prologue */
                                      + bytes(3, "\x48\x81\xC4") + u32le((uint32_t)Ke)
                                      + bytes(1, "\xC3");
                out_site = (int)succ_half.size(); out_lbl = g_emit.flat_fail_p; out_def = true;
                if (out_succ) *out_succ = succ_half;
                if (out_fail) *out_fail = fail_half;
                return succ_half + fail_half;
            }
            std::string dpop;
            if      (g_frame_active && g_emit_frame_caller_dl == 1) dpop = bytes(4, "\x48\x83\xC4\x08") + bytes(2, "\x41\x5D");
            else if (g_frame_active && g_emit_frame_caller_dl == 2) dpop = bytes(4, "\x48\x83\xC4\x08") + bytes(2, "\x41\x5E");
            else if (g_frame_active && g_emit_frame_caller_dl == 3) dpop = bytes(4, "\x48\x83\xC4\x08") + bytes(2, "\x41\x5F");
            std::string unwind = g_frame_active ? (xaf_anchor_leave_bin() + dpop + xaf_pop_frame()) : bytes(4, "\x48\x83\xC4\x08");
            std::string succ_half = g_frame_active
                 ? ( bytes(1, "\xB8") + u32le(1)
                   + bytes(2, "\x31\xD2")
                   + unwind
                   + bytes(1, "\xC3") )
                 : ( bytes(2, "\x48\xB9") + u64le(ADDR_SIGMA)
                   + bytes(3, "\x48\x8B\x01")
                   + bytes(3, "\x49\x63\xCE")
                   + bytes(4, "\x48\x8D\x04\x08")
                   + bytes(3, "\x48\x89\xC2")
                   + bytes(1, "\xB8") + u32le(1)
                   + unwind
                   + bytes(1, "\xC3") );
            std::string fail_half = g_frame_active
                 ? ( xaf_frame_store_imm32(0u, 99u)
                   + xaf_frame_store_imm32(8u, 0u)
                   + bytes(1, "\xB8") + u32le(99)
                   + bytes(2, "\x31\xD2")
                   + unwind
                   + bytes(1, "\xC3") )
                 : ( bytes(1, "\xB8") + u32le(99)
                   + bytes(2, "\x31\xD2")
                   + unwind
                   + bytes(1, "\xC3") );
            out_site = (int)succ_half.size(); out_lbl = g_emit.flat_fail_p; out_def = true;
            if (out_succ) *out_succ = succ_half;
            if (out_fail) *out_fail = fail_half;
            return succ_half + fail_half;
        }
        {
            extern int g_frame_active;
            if (g_emit.flat_wired) {
                return std::string(" jmp ") + xa_wired_base() + "_wγ\n"
                     + xaf_ω_label()
                     + " jmp " + xa_wired_base() + "_wω\n";
            }
            if (g_emit.flat_jmp_entry) {
                int kt = g_emit.flat_frame_bytes;
                if (x86_zc_frame() == ZC_FRAME_RSP && (g_emit.flat_pat || g_emit.flat_gen)) {   /* REG-7 U5 (Lon FORTH ruling) TEXT twin: pat-blob SUSPEND exits, rbp/header-above; GENP-SPINE (s92) widens to flat_gen — generator procs suspend on the main ζ spine, flat_gen preloading the result DESCR into rdi:rsi pre-record (det delivery protocol).  γ retains the activation (16B record {landing, rbp} at the deep frontier, wires via pinned rbp, self-referential caller restore); ω unwinds ABSOLUTELY lea rsp,[rbp+kt] (seal cuts arrive at any depth), reads-before-motion. */
                    char sa[288], sb2[224], fb2[320];
                    snprintf(sa, sizeof sa, "%spush rbp\nlea rax, [rip + %s]\n", g_emit.flat_gen ? "mov rdi, [rbp]\nmov rsi, [rbp + 8]\n" : "", (g_emit.flat_res_p && g_emit.flat_res_p->name) ? g_emit.flat_res_p->name : "?");
                    snprintf(sb2, sizeof sb2, "push rax\nmov rax, [rbp + %d]\nmov rbp, [rbp + %d]\njmp rax\n", kt - 24, kt - 8);
                    snprintf(fb2, sizeof fb2, "mov rax, [rbp + %d]\nlea rsp, [rbp + %d]\nmov rbp, [rbp + %d]\njmp rax\n", kt - 16, kt, kt - 8);
                    if (out_succ) *out_succ = std::string(sa);
                    if (out_fail) *out_fail = xaf_ω_label() + fb2;
                    return std::string(sb2);
                }
                if (x86_zc_frame() == ZC_FRAME_RSP && !emit_jmp_pin_rbp()) {   /* FLATDISP-7 (s194) TEXT twin of the BINARY depth-static arm: every arrival at these exits is at base (the classifier's proof), so reads are rsp-relative, teardown is one add, rbp untouched end to end.  Same falsifiable tripwire: off-base arrival = garbage wires = loud crosscheck failure. */
                    char sg[288], fo[352];
                    snprintf(sg, sizeof sg, "mov rdi, [rsp]\nmov rsi, [rsp + 8]\nmov rax, [rsp + %d]\nadd rsp, %d\njmp rax\n", kt - 24, kt);
                    snprintf(fo, sizeof fo, "mov rax, [rsp + %d]\nadd rsp, %d\njmp rax\n", kt - 16, kt);
                    if (out_succ) *out_succ = std::string(sg);
                    if (out_fail) *out_fail = xaf_ω_label() + fo;
                    return std::string();
                }
                if (x86_zc_frame() == ZC_FRAME_RSP) {   /* R12-ERAD s65 → NCB-1d (s90): DETERMINATE exits, DEPTH-IMMUNE rbp-absolute (TEXT twin of the BINARY arm above).  γ loads the result DESCR (frame slot 0, IR_RETURN's write) into rdi:rsi via the PINNED rbp PRE-unwind (the landing's
                     * epilogue_γ takes it BY VALUE — the frame is dead memory after the lea), reads the outside-γ wire at [rbp+kt−24], unwinds ABSOLUTELY lea rsp,[rbp+kt] (Icon `return`/`fail` from nested generator/scan depth arrives with rsp DEEP — the old rsp-relative form's
                     * every-ω-pops assumption holds only for SNOBOL4's determinate procs), restores caller rbp self-referentially LAST (reads-before-motion), and jmps the wire — no resume record (det: β fires only on a post-return re-entry, UB).  ω mirrors with the [rbp+kt−16] wire.  FLATDISP-7 (s194): pinned graphs only — see the depth-static arm above. */
                    char sg[288], fo[352];
                    snprintf(sg, sizeof sg, "mov rdi, [rbp]\nmov rsi, [rbp + 8]\nmov rax, [rbp + %d]\nlea rsp, [rbp + %d]\nmov rbp, [rbp + %d]\njmp rax\n", kt - 24, kt, kt - 8);
                    snprintf(fo, sizeof fo, "mov rax, [rbp + %d]\nlea rsp, [rbp + %d]\nmov rbp, [rbp + %d]\njmp rax\n", kt - 16, kt, kt - 8);
                    if (out_succ) *out_succ = std::string(sg);
                    if (out_fail) *out_fail = xaf_ω_label() + fo;
                    return std::string();
                }
                char sa[224], sb2[224], fb[320];
                snprintf(sa, sizeof sa, "push %s\nlea rax, [rip + %s]\n", x86_zr(), (g_emit.flat_res_p && g_emit.flat_res_p->name) ? g_emit.flat_res_p->name : "?");
                snprintf(sb2, sizeof sb2, "push rax\nmov rax, [%s-24]\nmov %s, [%s-8]\njmp rax\n", x86_zr(), x86_zr(), x86_zr());
                snprintf(fb, sizeof fb, "mov rax, [%s-16]\nlea rsp, [%s + %d]\nmov %s, [%s-8]\njmp rax\n", x86_zr(), x86_zr(), kt - 32, x86_zr(), x86_zr());
                if (out_succ) *out_succ = std::string(sa);
                if (out_fail) *out_fail = xaf_ω_label() + fb;
                return std::string(sb2);
            }
            if (g_frame_active) {
                extern int g_emit_frame_caller_dl;
                if (x86_zc_frame() == ZC_FRAME_RSP) { /* R12-ERAD: anchor-leave normalizes rsp to frame base, FAILDESCR lands at [rsp+0], add rsp,K rejoins the caller's ret-addr */
                    extern int g_gen_proc_active; extern int g_resumable_callable_active;
                    if (g_gen_proc_active || g_resumable_callable_active) {   /* PL-GEN-RSP fix (TEXT twin): result/FAILDESCR at [rbp+0] (heap frame); pop rbp; ret; no add rsp */
                        std::string sgt = std::string("pop rbp\nret\n");
                        std::string fgt = xaf_ω_label()
                             + "mov dword ptr [rbp+0], 99\nmov dword ptr [rbp+4], 0\nmov qword ptr [rbp+8], 0\npop rbp\nret\n";
                        if (out_succ) *out_succ = sgt;
                        if (out_fail) *out_fail = fgt;
                        return sgt + fgt;
                    }
                    int Kt = xaf_outer_frame_k();
                    char rbpr[64]; if (xaf_deep()) snprintf(rbpr, sizeof rbpr, "mov rbp, [rsp + %d]\n", Kt - 8); else rbpr[0] = 0;   /* FLATDISP-5 GATE: reload the caller's rbp only if this activation's prologue actually saved it.  Depth-static graphs never clobbered rbp, so there is nothing to restore and the header slot at Kt-8 goes unwritten and unread — the `add rsp, Kt` release below is the ENTIRE frame teardown. */
                    char frel[64]; snprintf(frel, sizeof frel, "add rsp, %d\n", Kt);
                    std::string succ_half = std::string("mov eax, 1\n") + "xor edx, edx\n" + xaf_anchor_leave_text() + rbpr + frel + "ret\n";
                    std::string fail_half = xaf_ω_label()
                         + xaf_anchor_leave_text()
                         + "mov eax, 99\n" + "xor edx, edx\n" + rbpr + frel + "ret\n";
                    if (out_succ) *out_succ = succ_half;
                    if (out_fail) *out_fail = fail_half;
                    return succ_half + fail_half;
                }
                const char *dreg = (g_emit_frame_caller_dl == 1) ? "r13" : (g_emit_frame_caller_dl == 2) ? "r14" : (g_emit_frame_caller_dl == 3) ? "r15" : (const char *)0;
                std::string dpop = xaf_anchor_leave_text() + (dreg ? (std::string("add rsp, 8\npop ") + dreg + "\n") : std::string());
                std::string succ_half = std::string("mov eax, 1\n")
                     + "xor edx, edx\n"
                     + dpop
                     + "pop " + x86_zr() + "\n"
                     + "ret\n";
                std::string fail_half = xaf_ω_label()
                     + "# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure\n"
                     + "mov dword ptr [" + x86_zr() + "+0], 99\n"
                     + "mov dword ptr [" + x86_zr() + "+4], 0\n"
                     + "mov qword ptr [" + x86_zr() + "+8], 0\n"
                     + "mov eax, 99\n"
                     + "xor edx, edx\n"
                     + dpop
                     + "pop " + x86_zr() + "\n"
                     + "ret\n";
                if (out_succ) *out_succ = succ_half;
                if (out_fail) *out_fail = fail_half;
                return succ_half + fail_half;
            }
        }
        return std::string("lea rcx, [rip + Σ]\n")
             + "mov rax, [rcx]\n"
             + "movsxd rcx, r14d\n"
             + "lea rax, [rax+rcx]\n"
             + "mov rdx, rax\n"
             + "mov eax, 1\n"
             + "ret\n"
             + xaf_ω_label()
             + "mov eax, 99\n"
             + "xor edx, edx\n"
             + "ret\n";
    }
    return std::string();
}
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
/* xa_flat_prologue/epilogue: the mark/release_to calls are emitted via bb_emit_x86 (the ordinary template
 * dispatch consumer, tag-decodes x86()'s L/J/D/E/F/X record stream) as SEPARATE statements around the
 * existing xa_emit_one call (which still only ever sees the original, unmodified raw prologue/epilogue
 * bytes it always has) — never spliced into the string xa_emit_one patches. This keeps xa_emit_one's own
 * patch-site byte-offset arithmetic completely untouched by this rung, and keeps zero manual MEDIUM_* checks
 * in this file: bb_emit_x86 and xa_emit_one each already dispatch on medium internally, exactly once, inside
 * their own bodies — this wrapper just sequences calls, it never inspects MEDIUM_* itself. */
extern "C" void xa_flat_prologue(void) {
    int st; bb_label_t * lb; bool df;
    auto s = xa_flat_prologue_str(st, lb, df);
    xa_emit_one(s, st, lb, df);
    extern int g_frame_active;
    if (g_frame_active && g_emit_cfg && g_emit_cfg->zeta_mark_slot >= 0) bb_emit_x86(x86_zeta_mark_call(g_emit_cfg->zeta_mark_slot));
}
extern "C" void xa_flat_epilogue(void) {
    int st; bb_label_t * lb; bool df; std::string succ, fail;
    auto s = xa_flat_epilogue_str(st, lb, df, &succ, &fail);
    extern int g_frame_active;
    if (g_emit.flat_jmp_entry && (!succ.empty() || !fail.empty())) {
        /* ZS-2: γ SUSPENDS the activation (frame + interior state stay live for β resumes) so it must NOT
         * release the zls mark; the release belongs to ω alone, placed AFTER the ω define so a jump to lbl_ω
         * runs it.  Suspended-then-succeeded activations are reclaimed wholesale by the statement bracket
         * (head.zeta_mark / S10e), the same law that reclaims their rsp cells.  Piece convention (see the
         * _str jmp-entry arm): succ = γ piece A (ends at the res-landing lea patch, described by st/lb/df),
         * s (the return) = γ piece B, fail = the ω-half. */
        xa_emit_one(succ, st, lb, df);
        xa_emit_one(s, 0, nullptr, false);
        xa_emit_one(std::string(), 0, g_emit.flat_fail_p, true);
        if (g_frame_active && g_emit_cfg && g_emit_cfg->zeta_mark_slot >= 0) bb_emit_x86(x86_zeta_release_to_call(g_emit_cfg->zeta_mark_slot));
        xa_emit_one(fail, 0, nullptr, false);
        return;
    }
    bool have_halves = g_frame_active && (!succ.empty() || !fail.empty());
    if (!have_halves) { xa_emit_one(s, st, lb, df); return; }
    int off = g_emit_cfg ? g_emit_cfg->zeta_mark_slot : -1;
    xa_emit_one(succ, 0, nullptr, false);
    if (off >= 0) bb_emit_x86(x86_zeta_release_to_call(off));
    xa_emit_one(fail, 0, lb, df);
}
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
    int anchor = (x86_zc_frame() != ZC_FRAME_RSP) ? xaf_anchor_off() : -1;   /* ALIGN-INV-3c: DC stub mirrors the jmp-entry prologue -- under RSP the anchor store is dead there too */
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
