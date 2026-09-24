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
 *   ___  zeta frame base IN PINNED GRAPHS ONLY   rsp  zeta + C stack (ONE stack)
 * ⛔ r12 IS FREE — NOT A PIN (corrected s219; ARCH section 2 corrected it at s205
 * and this header contradicted the document for FOURTEEN sessions). the r12 frame constant
 * was DELETED outright at ZR-RSP___-1 (Lon directive 2026-07-27, SCRIP da8c2347),
 * zero #if consumers; the zeta basis set is CLOSED at RSP and ___. The old entry
 * here read "r12 conditional-assignment stack pointer (own mmap area)", which is
 * DEAD HISTORY. This is RULES' (a)-class rot in the worst possible place: an asm
 * author reads the header, not the document, and needlessly avoids a free GPR.
 * ⛔ AND ___ IS PER-GRAPH, NOT UNCONDITIONAL (FLATDISP-8, s197): pinned graphs
 * (suspended generators, pattern blobs, deep-arrival) seed mov ___,rsp; depth-
 * static determinate graphs use rsp and ___ is a free GPR. ONE selector,
 * x86_fb_pinned(). ⛔ IT IS NOT emit_rec_pin(), AND THIS LINE CLAIMED IT WAS FOR
 * MANY SESSIONS WHILE x86_fb_pinned() WAS A `return 0` STUB — so the stated
 * equality was false in BOTH directions and nothing observable disagreed with it
 * (the recurring shape: a stub and a doc that agree only because neither is
 * exercised). emit_rec_pin() is true for Icon generators, lcl_proc graphs AND
 * every zframe graph — Raku and Pascal included — so keying the pin on it would
 * re-home three languages' frames to grant one its activation zeta, the exact
 * language-blind widening that cost 47 Icon programs at s272. x86_fb_pinned()
 * answers the graph's own STORAGE REGIME (emit_zframe_pinned) and today only the
 * Prolog activation regime sets it, per PZ-4 clause (a). NEVER hardcode
 * [___+off] or [rsp+off] in RTX asm or a template — use FR(off)/FRQ(off), which
 * resolve through x86_fb().
 * The five surviving pins are a SUBSET of the SysV callee-saved GP set, so
 * C-signature RTX routines inherit the guarantee; hand entries that clobber one
 * must save it. r12 being free does NOT exempt it from that rule when a pinned
 * graph is live above you — it is free, not scratch-by-fiat.
 *
 * RTX WORKING SET — free at every entry, zero boundary cost:
 *   rax rcx rdx rsi rdi + xmm0-15
 * THE RTCC FOUR SURVIVE EVERY RTX ENTRY (Lon 2026-09-23, "get rid of veneer for all RT
 * ASM instances"; CEO-1224; ARCH-SNOBOL4-RTX.md section 2). Emitted code calls an RTX
 * entry BARE -- no rtccb write-back before, no reload after -- so the entry itself owes
 * what the veneer used to buy:
 *   r8 r10 r11  returned exactly as received (r10 = statement number and r11 = box id
 *               are the crash telemetry; r8 is whatever the caller left there, the
 *               fifth argument included)
 *   r9          returned as the GVA base, rtccb slot 6 -- the caller may have loaned it
 *               out as a sixth argument (rt_match_replace), and the veneer's reload was
 *               what handed it back
 * A body keeps the promise one of three ways, and the static gate
 * test_gate_rtx_entries_keep_the_rtcc_four.sh proves which by walking every entry:
 *   (1) it never writes them;
 *   (2) it opens with RTX_SAVE and leaves by RTX_RET (RTX_RET_GVA when r9 came in as
 *       an argument), so every write in between is its own business;
 *   (3) it saves them around the one place it needs them (its own push/pop pairs).
 * EVERY EXIT INTO C IS WRAPPED, because gcc-compiled C clobbers all four: a call in an
 * unsaved body is RTX_CCALL(fn); a tail exit lands on a local stub that is RTX_CTAIL(fn)
 * (or RTX_CTAIL_SAVED(fn) inside a saved body, which reloads the fifth and sixth
 * arguments from the save area first); RTX_GATE's gate-off arm lands on such a stub.
 * AN RTX ENTRY IS NEVER REACHED THROUGH A PLT: a lazily bound PLT slot resolves through
 * _dl_runtime_resolve, which jumps to the target through r11 and runs _dl_fixup, so the
 * FIRST call of every symbol would hand the callee a clobbered r10/r11 to "preserve".
 * Nested calls and tail jumps between RTX entries use RTX_CALL / RTX_JMP (through the
 * GOT, which the loader fills eagerly); mode 4 emits call qword ptr [rip + sym@GOTPCREL]
 * for the same reason; mode 3 already calls by address.
 * THE ENTRY LIST IS DERIVED, NEVER WRITTEN: RTX_FUNC and RTX_ENTRY append the name to
 * section rtx_entry_names, and rtx_entry_is() (rtx_init.c) is what the emitter asks.
 * Internal roles (SPITBOL lessons, ARCH section 2):
 *   rax value/result (W0) . rdx tag word + mul/div companion . rsi/rdi string
 *   src/dst (movs/lods/stos/scas/cmps native) . rcx length/count, cl char work
 * Conditions use EFLAGS + setcc/jcc, never a memory flag byte.
 * cld is a global invariant: any std user restores it before returning.
 *
 * ---------------------------------------------------------------------------
 * DESCR_t — 4-FIELD LAYOUT SINCE descr-stamp-fields (this HEAD; supersedes the s162 2-field probe
 * below, kept for the history): v (uint8) at 0, src_node (uint8[3], 24-bit) at 1, slen
 * (uint32) at 4, value union at 8. Two INTEGER eightbytes still, so SysV still passes and returns it
 * in a register PAIR -- the split narrowed v within the SAME low eightbyte, it did not add one:
 *     ARG n=1:  rdi = (slen << 32) | src_node << 8 | v                 rsi = value
 *     RETURN :  rax = (slen << 32) | src_node << 8 | v                 rdx = value
 *   al/eax still alias the tag byte directly (v is the LOW BYTE now, not the low 32 bits -- a 32-bit
 *   `cmp eax,DT_x` or a 32-bit zero-test against the whole low eightbyte reads src_node/slen
 *   too and is WRONG post-split; every live tag test in this family compares al/cl/dl/dil/sil, per
 *   the 171-site conversion). shr by 32 still gets slen; src_node is
 *   (eax >> 8) & 0xFFFFFF. A HAND-WRITTEN MINT HERE LEAVES src_node 0, UNSTAMPED, AND THAT IS DELIBERATE: the
 *   123 per-routine ids this family used to fold into the same immediate were a SECOND DIMENSION (which rt
 *   function a BB called, never which BB minted) and they are gone -- see descr_tags.inc's mint_op block for
 *   what was lost, why Lon accepted the loss, and why its home must NOT be this byte.
 *   Phase-1 ports keep C signatures, so descriptor-in-registers is FREE and
 *   NO template changes are owed. Registerized signatures are RTX-CC (rung 11).
 *
 *   [SUPERSEDED, kept for provenance] the original s162 probe read this as a 2-field struct (v as a
 *   FULL int32 at 0, slen as uint32 at 4) -- true before descr-stamp-fields split v out of its low
 *   32 bits; a bare `(slen<<32)|v` immediate with v small still assembles to the identical bit
 *   pattern either way, which is exactly why the stale reading went unnoticed here for a full row.
 * DTYPE tags (descr.h is authoritative -- never hardcode in .S without a comment citing descr.h line):
 *   SNUL 0x00  S 0x02  I 0x03  R 0x05  N 0x09  E 0x38  FAIL 0x68  DATA 0x70+8k
 * ⛔ THE OLD LIST (SNUL 0 S 1 I 6 E 11 FAIL 99 DATA 100) IS PRE-s229 AND WRONG -- any .S #define must cite descr.h line
 */
#ifndef RTX_ABI_INC
#define RTX_ABI_INC

.intel_syntax noprefix

/* ---- TAG LAYOUT: must stay byte-identical to src/contracts/descr.h (s229).
 * DT_SNUL is 0 ON PURPOSE (bulk memset mints null strings free). rtx_init.c carries a
 * _Static_assert per value; a drift here breaks the BUILD, not the runtime.
 * ⛔ STRING tests are 8-BIT ONLY (test <r8-class>, DT_NOTSTR_MASK & 0xFF) -- CORRECTED s256:
 * a 32-bit test reads the DESCR provenance stamp's 24-bit src_node (bits 8-31, ARCH-
 * SNOBOL4-RTX.md sec9) once SCRIP_DESCR_STAMP=1, and misclassifies a stamped string as not-a-
 * string (HQ FINDING s256). The OLD 32-bit rule this comment stated was the bug. See descr.h. ---- */
#include "descr_tags.inc"   /* s230: THE tag numbers live there and ONLY there -- syntax-neutral so the AT&T files can share them. */

/* CET/IBT is enabled on this toolchain, so every externally reachable entry
 * needs endbr64 (a 4-byte nop where CET is absent). RTX_FUNC plants it. */
#define RTX_NAME(name)   .pushsection rtx_entry_names,"a",@progbits; .asciz #name; .popsection
#define RTX_FUNC(name)   .text; .globl name; .type name,@function; .align 16; name: ; endbr64; RTX_NAME(name)
#define RTX_ENTRY(name)  .globl name; .type name,@function; name: ; endbr64; RTX_NAME(name)
#define RTX_ENDF(name)   .size name, .-name

#define RTX_SAVE              push r8; push r9; push r10; push r11
#define RTX_RESTORE           pop r11; pop r10; pop r9; pop r8
#define RTX_RET               RTX_RESTORE; ret
#define RTX_GVA_R9            mov r9, qword ptr [rip + rtccb@GOTPCREL]; mov r9, qword ptr [r9 + 48]
#define RTX_RET_GVA           RTX_RESTORE; RTX_GVA_R9; ret
#define RTX_CCALL(fn)         push r8; push r9; push r10; push r11; call fn; pop r11; pop r10; pop r9; pop r8
#define RTX_CTAIL(fn)         push r8; push r9; push r10; push r11; sub rsp, 8; call fn; add rsp, 8; pop r11; pop r10; pop r9; pop r8; ret
#define RTX_CTAIL_GVA(fn)     push r8; push r9; push r10; push r11; sub rsp, 8; call fn; add rsp, 8; pop r11; pop r10; pop r9; pop r8; RTX_GVA_R9; ret
#define RTX_CTAIL_SAVED(fn)   mov r8, qword ptr [rsp + 24]; mov r9, qword ptr [rsp + 16]; sub rsp, 8; call fn; add rsp, 8; RTX_RET
#define RTX_CTAIL_SAVED_GVA(fn) mov r8, qword ptr [rsp + 24]; mov r9, qword ptr [rsp + 16]; sub rsp, 8; call fn; add rsp, 8; RTX_RET_GVA
#define RTX_CALL(fn)          call qword ptr [rip + fn@GOTPCREL]
#define RTX_JMP(fn)           jmp qword ptr [rip + fn@GOTPCREL]

/* Per-family kill-switch (ARCH section 4). The gate byte is HIDDEN: scrip links
 * dynamically against libscrip_rt.so (Makefile:343, Lon 2026-07-25), so there is
 * exactly ONE runtime copy and hidden visibility makes [rip+sym] direct and
 * interposition-proof. Gate 0 => jump to a LOCAL STUB (RTX_CTAIL of the untouched
 * C body), which still has the identical signature, so the args are already in
 * place; the stub saves the RTCC four around the call, because the C body clobbers
 * them and our caller no longer reloads them. The second argument names that stub. */
#define RTX_GATE_DEF(fam)  .data; .align 1; .globl rtx_gate_##fam; .hidden rtx_gate_##fam; \
                           .type rtx_gate_##fam,@object; .size rtx_gate_##fam,1; rtx_gate_##fam: .byte 0; .text
#define RTX_GATE(fam, stub)  cmp byte ptr [rip + rtx_gate_##fam], 0; je stub

/* libc boundary (ARCH section 3). Entry from a call leaves rsp == 8 (mod 16),
 * so one push realigns. Save live volatiles yourself; the pins are callee-saved
 * and libc preserves them for free, but r8-r11 are the caller's (see above), so
 * a libc call inside an unsaved body is RTX_CCALL like any other C. We do NOT
 * need SPITBOL's global reg_block: libc never inspects SCRIP state. */
#define RTX_CALL_ALIGN     push rsp; push qword ptr [rsp]; and rsp, -16
#define RTX_CALL_UNALIGN   mov rsp, [rsp + 8]

#endif
