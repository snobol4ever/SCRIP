/* rtx_zdp.s — LON'S ANCHOR CHECK (s135), RUNTIME HALF.  READ .github/ARCH-SNOBOL4-RTX.md BEFORE EDITING.  Contract macros: rtx_abi.inc.
 *
 * THE MODEL (Lon in-chat s135): an anchor is a point where this graph's stack is TOTALLY EMPTY, so every anchor must observe ONE AND THE SAME rsp.
 * IR_STATEMENT_BEGIN and IR_MATCH_BEGIN are the anchors / re-basing points.  A disagreement is a box that carved and never released — at a
 * guaranteed meet point the frontier is fixed by the UNWIND LAW, so the disagreement is a BUG REPORT, never an imprecision to be normalised away.
 *
 * ⛔ WHY THIS IS ASM AND NOT C (Lon s135 "use asm not C for this").  A C callee is entitled to destroy the whole SysV caller-saved set — rax rcx rdx
 * rsi rdi r8 r9 r10 r11 — all nine are saved/restored below, not a claimed subset.  r8/r9 carry the RTCC/GVA tier.  r10/r11 are NOT the product-wide
 * γ/ω WIRES anymore — that permanent claim (LADDER WREG) was retired by the s195 PUSH-PUSH-at-RBP migration and the 2026-08-22 free-r10/free-r11
 * eradication (ARCH-SNOBOL4-RTX.md §2) — but they still carry a LIVE γ/ω pair in two surviving mechanisms documented there (the frameless-blob
 * suspend cache, `emit.cpp`; the DEFINE-activation shim, `bb_define.cpp` role 4), so saving them here is not purely generic completism: an anchor
 * can legitimately fire while one of those pairs is in flight.  Instrumenting every anchor with a C call would therefore CHANGE THE PROGRAM BEING MEASURED, which is
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
    push    r11                                 /* not a product-wide wire anymore (§2) but may hold a live γ/ω pair from a surviving mechanism -- saved/restored across the C sink like every other register so the reporting path is as perturbation-free as the agreeing one */
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
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* rt_zdp_origin — LON'S EVERY-PORT PROBE (s136), THE DATUM.  "Instrument each BB graph's FIRST BB to save RSP (RSP0)."
 * ENTRY: rdi = the graph's TRUE entry rsp (call site added back its own pushes).  rsi = node id (unused on the hot path, kept for the report).
 * EXIT:  every register and rflags preserved.  Unconditional store: each graph RE-BASES, so a nested graph legitimately overwrites the datum.
 * ⛔ THE KNOWN LIMIT, NAMED NOT DISCOVERED: the cell is ONE cell, so a RECURSIVE callee's origin overwrites its caller's and the caller's
 * remaining ports then measure against the callee's datum.  Lon's ruling s136 in-chat: "I know RSP changes into a function but just ignore
 * all that.  Each statement of the function will do a good job."  The per-statement anchors re-establish agreement inside the callee, so the
 * recursion noise is bounded to the caller's post-call ports and is READ AS NOISE, never as a finding, until the census says otherwise. */
RTX_FUNC(rt_zdp_origin)
    mov     qword ptr [rip + g_zdp_anchor_rsp], rdi
    ret
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* rt_zdp_probe — LON'S EVERY-PORT PROBE (s136), THE CHECK AT EVERY α AND EVERY β.
 * ENTRY: rdi = TRUE rsp at this port  ·  rsi = op  ·  rdx = node  ·  rcx = EXPECTED delta in bytes (-1 = the lattice refuses, RECORD ONLY)  ·  r8 = port | rbp-flag (1=α 2=β, +4 = check rbp).
 * EXIT:  every register and rflags preserved.  The AGREEING path touches rax and rflags only -- both restored -- so a sound port is
 *        perturbation-free BY CONSTRUCTION.  This is why the hot path is hand asm: a C callee may destroy r10/r11 (no longer the product-wide γ/ω
 *        WIRES, but still live in the surviving mechanisms named in ARCH-SNOBOL4-RTX.md §2) and r8/r9
 *        (the RTCC/GVA tier), which would make the instrument measure a DIFFERENT PROGRAM -- the s132 monitor lesson, verbatim. */
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* rt_zdp_ev — ZETA SM SHIM (Lon in-chat, this session).  ONE entry point for all three ports; the state
 * machine itself is C (rt_zdp_sm_event in runtime_init.c) because the bookkeeping is real logic, and this
 * shim is what makes that safe.
 * ⛔ HAND ASM BECAUSE A C CALLEE OWNS THE WHOLE SysV CALLER-SAVED SET (Lon in-chat, same law as rt_zdp_probe
 * above): rax rcx rdx rsi rdi r8 r9 r10 r11 -- and THREE of those carry a role here, r8/r9 the RTCC/GVA tier
 * and r10/r11 possibly a live γ/ω pair from one of the surviving mechanisms in ARCH-SNOBOL4-RTX.md §2 (the
 * product-wide WREG wire claim itself is retired, s195 + 2026-08-22 free-r10/free-r11).  Letting C clobber them would make the instrument measure a DIFFERENT
 * program, the s132 monitor lesson verbatim.  So EVERY caller-saved register and rflags is saved and restored
 * around the sink; the measured program cannot observe that this ran.  Unlike the earlier probes there is no
 * "agreeing fast path" here -- the SM must see every port to keep its state, and correctness of the state is
 * worth more than the cycles.
 * ENTRY: rdi = node id · rsi = rbp at the port · rdx = TRUE rsp at the port (call site added back its own
 *        pushes) · rcx = kind (1=α 2=β 3=ω).   EXIT: every register and rflags preserved. */
RTX_FUNC(rt_zdp_ev)
    push    rax
    pushfq
    push    r8
    push    r9
    push    r10
    push    r11                             /* possible live γ/ω pair, see §2 -- not the product-wide claim, that's retired */
    push    rdi
    push    rsi
    push    rdx
    push    rcx
    push    rbp
    mov     rbp, rsp
    and     rsp, -16                        /* SysV wants rsp%16==0 at the call; a port's own alignment is unknown */
    call    rt_zdp_sm_event                 /* args already live in rdi/rsi/rdx/rcx */
    mov     rsp, rbp
    pop     rbp
    pop     rcx
    pop     rdx
    pop     rsi
    pop     rdi
    pop     r11
    pop     r10
    pop     r9
    pop     r8
    popfq
    pop     rax
    ret
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RTX_FUNC(rt_zdp_probe)
    push    rax
    pushfq
    mov     rax, qword ptr [rip + g_zdp_anchor_rsp]
    test    rax, rax
    jz      .Lzdpp_done                         /* no datum yet (a graph whose first BB was never instrumented) -- refuse to judge rather than report against zero */
    sub     rax, rdi                            /* rax = MEASURED delta = RSP0 - rsp_here, i.e. bytes carved and still standing at this port */
    cmp     rcx, -1
    je      .Lzdpp_record                       /* ⊤: the lattice REFUSES a prediction.  Record the observed delta anyway -- constant across runs = PRECISION bug (the lattice is refusing the spine for nothing), varying = the ⊤ was EARNED.  Rows 3 and 4 of Lon's mutual-validation table; nothing static can separate them. */
    cmp     rax, rcx
    jne     .Lzdpp_report                       /* MEASURED != CALCULATED.  Either the transfer function mis-calculated or the box leaked / over-freed.  The witness is the program itself. */
    test    r8, 4
    jnz     .Lzdpp_rbp                          /* the port agreed on rsp; if this box owns an rbp activation its β must ALSO still see the rbp its α left */
    test    r8, 8
    jnz     .Lzdpp_rbp_save                     /* α of an rbp-creating box: the probe sits BEFORE the template's own `push rbp`, so what we bank here is the ENCLOSING rbp -- exactly the value the ω/β whack (`mov rsp,rbp; pop rbp`) is obliged to put back */
.Lzdpp_done:
    popfq
    pop     rax
    ret
.Lzdpp_rbp_save:
    mov     qword ptr [rip + g_zdp_anchor_rbp], rbp
    jmp     .Lzdpp_done
.Lzdpp_rbp:
    cmp     rbp, qword ptr [rip + g_zdp_anchor_rbp]
    jne     .Lzdpp_report
    jmp     .Lzdpp_done
.Lzdpp_record:
.Lzdpp_report:
    push    rcx
    push    rdx
    push    rsi
    push    rdi
    push    r8
    push    r9
    push    r10
    push    r11                                 /* not a product-wide wire anymore (§2) but may hold a live γ/ω pair from a surviving mechanism -- saved/restored across the C sink so even a FIRING port is as perturbation-free as an agreeing one */
    push    rbp
    mov     rbp, rsp
    and     rsp, -16                            /* SysV wants rsp%16==0 at the call; a port's own alignment is unknown here, so force it and restore from rbp */
    push    r8
    push    rax
    mov     r9, rax                             /* arg6 = MEASURED delta */
    call    rt_zdp_probe_report                 /* rdi=rsp rsi=op rdx=node rcx=expected r8=portflags r9=measured */
    pop     rax
    pop     r8
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
    jmp     .Lzdpp_done
