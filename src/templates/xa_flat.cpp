#include <string>
#include "emit.h"
#include "x86_asm.h"
extern "C" {
#include "xa_template_common.h"
#include "../emitter/emit.h"
}
#define ADDR_SIGMA   ((uint64_t)(uintptr_t)&Σ)
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
static std::string xa_wired_base(void) {
    std::string s(g_emit.flat_lbl_α ? g_emit.flat_lbl_α : "");
    return s.size() > 3 ? s.substr(0, s.size() - 3) : s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void xa_emit_one(const std::string & out, int site, bb_label_t * lbl, bool is_def) {
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
static std::string xaf_frame_rsp_rm(uint32_t disp, unsigned char op) {
    int z = x86_zr_num(), lo = z & 7; std::string c;
    c += (char)(0x48 | (z >= 8 ? 0x01 : 0x00)); c += (char)op;
    c += (char)(0x80 | (4 << 3) | (lo == 4 ? 4 : lo)); if (lo == 4) c += (char)0x24;
    c += u32le(disp); return c;
}
static std::string xaf_anchor_enter_bin(void) {
    if (!x86_port_cstack()) return std::string();
    return xaf_frame_rsp_rm((uint32_t)xaf_anchor_off(), 0x89);
}
static std::string xaf_anchor_leave_bin(void) {
    if (!x86_port_cstack()) return std::string();
    return xaf_frame_rsp_rm((uint32_t)xaf_anchor_off(), 0x8B);
}
static std::string xaf_anchor_enter_text(void) {
    if (!x86_port_cstack()) return std::string();
    char b[160]; snprintf(b, sizeof b, "  mov qword ptr [%s + %d], rsp\n", x86_zr(), xaf_anchor_off()); return std::string(b);
}
static std::string xaf_anchor_leave_text(void) {
    if (!x86_port_cstack()) return std::string();
    char b[160]; snprintf(b, sizeof b, "mov rsp, qword ptr [%s + %d]\n", x86_zr(), xaf_anchor_off()); return std::string(b);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* GVA-BASE RELOAD fragments (ZC_PORT_CSTACK rung, same session).  The rbx=GVA-slab contract previously
 * survived C trampolines by ACCIDENT of -O0 codegen (C functions rarely allocated rbx); the CSTACK alloca in
 * rt_call_named_proc made GCC's stack-clash probe loop use rbx (div rbx) — callee-saved correctly at the C
 * boundary, but p->fn runs INSIDE the window, and the proc body read [rbx+k*16] = garbage (083 segfault,
 * fault pc in the BB arena on mov rax,[rbx+0x10]).  Durable contract: under CSTACK every frame-active graph
 * self-loads rbx from the exported g_gva_base cell (set by gva_register — the one seam both m3 driver and m4
 * main registration flow through).  Medium split is the sanctioned R10 RO-load exception, the g_zls2_cur
 * precedent: TEXT rip-relative lea resolved by the -no-pie link, BINARY movabs of the in-process cell. */
extern "C" void *g_gva_base;
static std::string xaf_gva_reload_bin(void) {
    if (!x86_port_cstack()) return std::string();
    std::string c; c += (char)0x48; c += (char)0xB8; c += u64le((uint64_t)(uintptr_t)&g_gva_base);
    c += (char)0x48; c += (char)0x8B; c += (char)0x18; return c;
}
static std::string xaf_gva_reload_text(void) {
    if (!x86_port_cstack()) return std::string();
    return std::string("  lea rax, [rip + g_gva_base]\n  mov rbx, qword ptr [rax]\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
                std::string r = bytes(3, "\x48\x81\xEC") + u32le((uint32_t)kt)                         /* sub rsp, K_total — the activation self-allocates */
                              + bytes(4, "\x48\x89\x4C\x24") + std::string(1, (char)0x08)               /* mov [rsp+8],  rcx — outside-γ wire */
                              + bytes(4, "\x48\x89\x54\x24") + std::string(1, (char)0x10)               /* mov [rsp+16], rdx — outside-ω wire */
                              + xaf_store_zr_rsp8(24)                                                    /* mov [rsp+24], zr — caller's frame reg ([rsp+0] = pad; β resumes ride the γ-pushed frontier record, not the header) */
                              + xaf_lea_zr_rsp8(32)                                                      /* zr = rsp+32 — region base; every FR() offset unchanged */
                              + xaf_mov_rdi_zr()
                   + bytes(1, "\xB9") + u32le((uint32_t)(kt - 32))                                       /* mov ecx, K_region */
                   + bytes(2, "\x31\xC0")                                                                /* xor eax, eax */
                   + bytes(2, "\xF3\xAA")                                                                /* rep stosb — zls_alloc zeroed (ZC_INIT_ZERO); the activation must too */
                   + xaf_gva_reload_bin()
                   + xaf_anchor_enter_bin();
                out_site = 0; out_lbl = nullptr; out_def = false;
                return r;
            }
            if (g_frame_active) {
                extern int g_emit_frame_caller_dl;
                std::string disp;
                if      (g_emit_frame_caller_dl == 1) disp = bytes(2, "\x41\x55") + xaf_mov_dreg_frame(13) + bytes(4, "\x48\x83\xEC\x08");
                else if (g_emit_frame_caller_dl == 2) disp = bytes(2, "\x41\x56") + xaf_mov_dreg_frame(14) + bytes(4, "\x48\x83\xEC\x08");
                else if (g_emit_frame_caller_dl == 3) disp = bytes(2, "\x41\x57") + xaf_mov_dreg_frame(15) + bytes(4, "\x48\x83\xEC\x08");
                std::string r = xaf_push_frame()
                              + xaf_mov_frame_rdi()
                              + xaf_gva_reload_bin()
                              + disp
                              + xaf_anchor_enter_bin()
                              + bytes(3, "\x83\xFE\x00")
                              + bytes(2, "\x0F\x85") + u32le(0);
                out_site = (int)r.size() - 4; out_lbl = g_emit.flat_β_p; out_def = false;
                return r;
            }
            out_site = 9; out_lbl = g_emit.flat_β_p; out_def = false;
            return bytes(4, "\x48\x83\xEC\x08")
                 + bytes(3, "\x83\xFE\x00")
                 + bytes(2, "\x0F\x85") + u32le(0);
        }
        if (MEDIUM_TEXT) {
            if (!g_is_text) return std::string();
            std::string banner;
            if (g_emit.flat_text_externalise) {
                banner = std::string("#") + std::string(BB_BANNER_RULE_LEN, '=') + "\n"
                       + "    .global " + (g_emit.flat_lbl_α    ? g_emit.flat_lbl_α    : "") + "\n"
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
                char b[640];
                snprintf(b, sizeof b,
                    "  sub rsp, %d\n  mov [rsp+8], rcx\n  mov [rsp+16], rdx\n  mov [rsp+24], %s\n  lea %s, [rsp+32]\n  mov rdi, %s\n  mov ecx, %d\n  xor eax, eax\n  rep stosb\n",
                    kt, x86_zr(), x86_zr(), x86_zr(), kt - 32);
                return banner + b + xaf_gva_reload_text() + xaf_anchor_enter_text();
            }
            if (g_frame_active) {
                extern int g_emit_frame_caller_dl;
                const char *dreg = (g_emit_frame_caller_dl == 1) ? "r13" : (g_emit_frame_caller_dl == 2) ? "r14" : (g_emit_frame_caller_dl == 3) ? "r15" : (const char *)0;
                std::string disp = dreg ? (std::string("  push ") + dreg + "\n  mov " + dreg + ", " + x86_zr() + "\n  sub rsp, 8\n") : std::string();
                std::string pro = banner + "push " + std::string(x86_zr()) + "\n  mov " + x86_zr() + ", rdi\n" + xaf_gva_reload_text() + disp + xaf_anchor_enter_text();
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
                     + (g_emit.flat_fail_p && g_emit.flat_fail_p->name ? std::string(g_emit.flat_fail_p->name) + ":\n" : std::string())
                     + " jmp " + xa_wired_base() + "_wω\n";
            }
            if (g_emit.flat_jmp_entry) {
                int kt = g_emit.flat_frame_bytes;
                char sa[224], sb2[224], fb[320];
                snprintf(sa, sizeof sa, "push %s\nlea rax, [rip + %s]\n", x86_zr(), (g_emit.flat_res_p && g_emit.flat_res_p->name) ? g_emit.flat_res_p->name : "?");
                snprintf(sb2, sizeof sb2, "push rax\nmov rax, [%s-24]\nmov %s, [%s-8]\njmp rax\n", x86_zr(), x86_zr(), x86_zr());
                snprintf(fb, sizeof fb, "%s%smov rax, [%s-16]\nlea rsp, [%s + %d]\nmov %s, [%s-8]\njmp rax\n",
                    (g_emit.flat_fail_p && g_emit.flat_fail_p->name) ? g_emit.flat_fail_p->name : "", (g_emit.flat_fail_p && g_emit.flat_fail_p->name) ? ":\n" : "", x86_zr(), x86_zr(), kt - 32, x86_zr(), x86_zr());
                if (out_succ) *out_succ = std::string(sa);
                if (out_fail) *out_fail = std::string(fb);
                return std::string(sb2);
            }
            if (g_frame_active) {
                extern int g_emit_frame_caller_dl;
                const char *dreg = (g_emit_frame_caller_dl == 1) ? "r13" : (g_emit_frame_caller_dl == 2) ? "r14" : (g_emit_frame_caller_dl == 3) ? "r15" : (const char *)0;
                std::string dpop = xaf_anchor_leave_text() + (dreg ? (std::string("add rsp, 8\npop ") + dreg + "\n") : std::string());
                std::string succ_half = std::string("mov eax, 1\n")
                     + "xor edx, edx\n"
                     + dpop
                     + "pop " + x86_zr() + "\n"
                     + "ret\n";
                std::string fail_half = (g_emit.flat_fail_p && g_emit.flat_fail_p->name ? std::string(g_emit.flat_fail_p->name) + ":\n" : std::string())
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
             + (g_emit.flat_fail_p && g_emit.flat_fail_p->name ? std::string(g_emit.flat_fail_p->name) + ":\n" : std::string())
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
    bool have_halves = g_frame_active && g_emit_cfg && g_emit_cfg->zeta_mark_slot >= 0 && (!succ.empty() || !fail.empty());
    if (!have_halves) { xa_emit_one(s, st, lb, df); return; }
    int off = g_emit_cfg->zeta_mark_slot;
    xa_emit_one(succ, 0, nullptr, false);
    bb_emit_x86(x86_zeta_release_to_call(off));
    xa_emit_one(fail, 0, lb, df);
}
extern "C" void xa_flat_data_section(void) { auto s = xa_flat_data_section_str(); if (!s.empty()) emit_text_n(s.data(), s.size()); }
