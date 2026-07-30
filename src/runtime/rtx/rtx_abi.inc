/* rtx_abi.inc — RTX asm runtime ABI contract, in macro form.
 *
 * THE CONTRACT IS .github/ARCH-SNOBOL4-RTX.md. READ IT BEFORE EDITING ANY .S FILE.
 * This header is that document's executable half; if the two disagree, the document wins.
 *
 * SYNTAX: Intel, registers on the left (Lon ruling s162). Project-wide, GNU as via gcc -c.
 * Files are .S (capital) so cpp runs first and these macros expand.
 *
 * ---------------------------------------------------------------------------
 * BLOB PINS — PRESERVE ACROSS EVERY RTX ENTRY. Phase 1 does not touch them.
 *   rbx  arena heap top / DESCR mint pointer
 *   r13  sigma  subject base pointer
 *   r14  delta  subject cursor
 *   r15  Delta  subject length/end
 *   rbp  zeta frame base IN PINNED GRAPHS ONLY   rsp  zeta + C stack (ONE stack)
 * ⛔ r12 IS FREE — NOT A PIN (corrected s219; ARCH section 2 corrected it at s205
 * and this header contradicted the document for FOURTEEN sessions). ZC_FRAME_R12
 * was DELETED outright at ZR-RSPRBP-1 (Lon directive 2026-07-27, SCRIP da8c2347),
 * zero #if consumers; the zeta basis set is CLOSED at RSP and RBP. The old entry
 * here read "r12 conditional-assignment stack pointer (own mmap area)", which is
 * DEAD HISTORY. This is RULES' (a)-class rot in the worst possible place: an asm
 * author reads the header, not the document, and needlessly avoids a free GPR.
 * ⛔ AND rbp IS PER-GRAPH, NOT UNCONDITIONAL (FLATDISP-8, s197): pinned graphs
 * (suspended generators, pattern blobs, deep-arrival) seed mov rbp,rsp; depth-
 * static determinate graphs use rsp and rbp is a free GPR. ONE selector,
 * x86_fb_pinned() == emit_rec_pin(). NEVER hardcode [rbp+off] or [rsp+off] in
 * RTX asm or a template — use FR(off)/FRQ(off), which resolve through x86_fb().
 * The five surviving pins are a SUBSET of the SysV callee-saved GP set, so
 * C-signature RTX routines inherit the guarantee; hand entries that clobber one
 * must save it. r12 being free does NOT exempt it from that rule when a pinned
 * graph is live above you — it is free, not scratch-by-fiat.
 *
 * RTX WORKING SET — free at every entry, zero boundary cost:
 *   rax rcx rdx rsi rdi r8 r9 r10 r11 + xmm0-15
 * Internal roles (SPITBOL lessons, ARCH section 2):
 *   rax value/result (W0) . rdx tag word + mul/div companion . rsi/rdi string
 *   src/dst (movs/lods/stos/scas/cmps native) . rcx length/count, cl char work
 *   r10 idiv divisor scratch, keeping rdx clear of the div blast zone
 * Conditions use EFLAGS + setcc/jcc, never a memory flag byte.
 * cld is a global invariant: any std user restores it before returning.
 *
 * ---------------------------------------------------------------------------
 * DESCR_t — VERIFIED EMPIRICALLY s162 (gcc -O1 probe, src/contracts/descr.h):
 *   16 bytes: v (int32) at 0, slen (uint32) at 4, value union at 8.
 *   Two INTEGER eightbytes, so SysV passes and returns it in a register PAIR:
 *     ARG n=1:  rdi = (slen << 32) | v      rsi = value
 *     RETURN :  rax = (slen << 32) | v      rdx = value
 *   edi/eax alias the type tag directly; shr by 32 gets slen.
 *   Phase-1 ports keep C signatures, so descriptor-in-registers is FREE and
 *   NO template changes are owed. Registerized signatures are RTX-CC (rung 11).
 * DTYPE tags used below (descr.h): SNUL 0 . S 1 . P 3 . A 4 . T 5 . I 6 . R 7
 *                                  C 8 . N 9 . K 10 . E 11 . X 15 . FAIL 99
 */
#ifndef RTX_ABI_INC
#define RTX_ABI_INC

.intel_syntax noprefix

#define DT_SNUL   0
#define DT_S      1
#define DT_P      3
#define DT_A      4
#define DT_T      5
#define DT_I      6
#define DT_R      7
#define DT_N      9
#define DT_X     15
#define DT_FAIL  99

/* CET/IBT is enabled on this toolchain, so every externally reachable entry
 * needs endbr64 (a 4-byte nop where CET is absent). RTX_FUNC plants it. */
#define RTX_FUNC(name)   .text; .globl name; .type name,@function; .align 16; name: ; endbr64
#define RTX_ENDF(name)   .size name, .-name

/* Per-family kill-switch (ARCH section 4). The gate byte is HIDDEN: scrip links
 * dynamically against libscrip_rt.so (Makefile:343, Lon 2026-07-25), so there is
 * exactly ONE runtime copy and hidden visibility makes [rip+sym] direct and
 * interposition-proof. Gate 0 => tail-jump to the untouched C body, which still
 * has the identical signature, so the args are already in place and its return
 * goes straight to our caller. That tail jump IS the byte-identity proof path. */
#define RTX_GATE_DEF(fam)  .data; .align 1; .globl rtx_gate_##fam; .hidden rtx_gate_##fam; \
                           .type rtx_gate_##fam,@object; .size rtx_gate_##fam,1; rtx_gate_##fam: .byte 0; .text
#define RTX_GATE(fam, cfallback)  cmp byte ptr [rip + rtx_gate_##fam], 0; je cfallback

/* libc boundary (ARCH section 3). Entry from a call leaves rsp == 8 (mod 16),
 * so one push realigns. Save live volatiles yourself; the pins are callee-saved
 * and libc preserves them for free. We do NOT need SPITBOL's global reg_block:
 * libc never inspects SCRIP state, unlike SPITBOL's osint functions. */
#define RTX_CALL_ALIGN     push rbp; mov rbp, rsp; and rsp, -16
#define RTX_CALL_UNALIGN   mov rsp, rbp; pop rbp

#endif
