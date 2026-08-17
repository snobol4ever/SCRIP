/* rtx_zdp.S — LON'S ANCHOR CHECK (s135), RUNTIME HALF.  READ .github/ARCH-SNOBOL4-RTX.md BEFORE EDITING.  Contract macros: rtx_abi.inc.
 *
 * THE MODEL (Lon in-chat s135): an anchor is a point where this graph's stack is TOTALLY EMPTY, so every anchor must observe ONE AND THE SAME rsp.
 * IR_STATEMENT_BEGIN and IR_MATCH_BEGIN are the anchors / re-basing points.  A disagreement is a box that carved and never released — at a
 * guaranteed meet point the frontier is fixed by the UNWIND LAW, so the disagreement is a BUG REPORT, never an imprecision to be normalised away.
 *
 * ⛔ WHY THIS IS ASM AND NOT C (Lon s135 "use asm not C for this").  A C callee is entitled to destroy the whole SysV caller-saved set — rax rcx rdx
 * rsi rdi r8 r9 r10 r11.  Three of those are CLAIMED by this project: r10/r11 are the γ/ω WIRES (RULES.md: "zero r10/r11 scratch (incl. RTX hand
 * asm)") and r8/r9 carry the RTCC/GVA tier.  Instrumenting every anchor with a C call would therefore CHANGE THE PROGRAM BEING MEASURED, which is
 * the s132 monitor lesson verbatim ("a monitor verdict is a verdict on a DIFFERENT program").  Here the AGREEING path — the overwhelming majority —
 * touches rax and rflags ONLY, both restored before return, so a sound anchor is perturbation-free by construction.
 *
 * ENTRY:  rdi = the TRUE anchor rsp (computed by the call site BEFORE its own pushes)  ·  rsi = IR op  ·  rdx = node id.
 * EXIT:   every register and rflags preserved.  No return value.
 * The COLD path (disagreement only) calls rt_zdp_report and saves the full caller-saved set around it, so even a firing anchor cannot corrupt the run.
 */
#include "rtx_abi.inc"

RTX_FUNC(rt_zdp_anchor)
    push    rax
    pushfq                                      /* the compare below writes rflags; an anchor sits between arbitrary boxes, so preserve it */
    mov     rax, qword ptr [rip + g_zdp_anchor_rsp]
    test    rax, rax
    jz      .Lzdp_first
    cmp     rax, rdi
    jne     .Lzdp_report
.Lzdp_done:
    popfq
    pop     rax
    ret
.Lzdp_first:
    mov     qword ptr [rip + g_zdp_anchor_rsp], rdi   /* FIRST anchor establishes the datum.  It cannot itself disagree, and it is never re-entered: the cell is RUN-lifetime, not per-activation, so the s134 bootstrapping caveat is structurally absent under Lon's anchor model. */
    jmp     .Lzdp_done
.Lzdp_report:
    push    rcx
    push    rdx
    push    rsi
    push    rdi
    push    r8
    push    r9
    push    r10
    push    r11                                 /* ⛔ THE WIRES.  Saved and restored across the C sink so the reporting path is as perturbation-free as the agreeing one. */
    push    rbp
    mov     rbp, rsp
    and     rsp, -16                            /* SysV requires rsp%16==0 at the call; the anchor's own alignment is unknown here, so force it and restore from rbp */
    mov     rcx, rax                            /* arg4 = expected (the datum) */
    call    rt_zdp_report                       /* rdi = actual rsp, rsi = op, rdx = node — all still live, none of them clobbered above */
    mov     rsp, rbp
    pop     rbp
    pop     r11
    pop     r10
    pop     r9
    pop     r8
    pop     rdi
    pop     rsi
    pop     rdx
    pop     rcx
    jmp     .Lzdp_done
