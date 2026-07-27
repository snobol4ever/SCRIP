/* rtx_call.S — RTX family CALL. RTX-4 slice 1: the two classic epilogue landings.
 *
 * READ .github/ARCH-SNOBOL4-RTX.md BEFORE EDITING. Contract macros: rtx_abi.inc.
 *
 * WHAT THIS PORTS AND WHY IT IS THE RIGHT FIRST SLICE
 *   rt_proc_call_epilogue_γ / _ω are the RETURN and FRETURN landings of every
 *   SNOBOL4 procedure call (manual Ch.8: RETURN yields a value, FRETURN signals
 *   failure and yields none; NRETURN is a γ citizen too, its by-name flag riding
 *   in rt_g_ret_by_name).  They are the #2 static call-boundary symbol pair in
 *   the whole runtime — 24 references each, second only to rt_call_arr.
 *
 *   The -O0 C of record was MEASURED (objdump, s165) to spend ~25 instructions
 *   on bookkeeping before any semantic work happens:
 *     - a 64-byte struct copy of the popped pcall record, emitted as 8 load/store
 *       PAIRS (16 memory ops), because rt_pcall_t is 64 bytes (stride shl 6);
 *     - g_pcall_top loaded THREE times to perform ONE decrement;
 *     - rt_k_level reached through a GOT indirection loaded TWICE for ONE
 *       decrement, because the symbol was visible and therefore interposable.
 *   This port does the same work in ~12 instructions.  Nothing semantic changes.
 *
 * THE ONE SUBTLETY THAT MUST NOT BE OPTIMIZED AWAY
 *   The private 64-byte copy of the pcall record is NOT redundant.  The record is
 *   popped (g_pcall_top decremented) BEFORE the body runs, so anything the body
 *   re-enters that pushes a new call would land on exactly the slot we are still
 *   reading.  The copy is what makes that safe, and it is PRESERVED here — merely
 *   done as 4 SSE 16-byte moves instead of 16 scalar ones.
 *   What WAS redundant is the SECOND copy: the C passed that already-private
 *   local to rt_proc_epilogue_body BY VALUE, copying 64 bytes a second time.  The
 *   body now takes a const pointer (same commit), which is provably safe because
 *   the body is static with exactly these two callers and each already owns a
 *   private copy.  That C change is deliberately separable from this asm so the
 *   two effects can be measured apart (gate off = by-pointer C, gate on = asm).
 *
 * FRAME LAYOUT (rbp-relative, 80 bytes, keeps rsp 16-aligned at every call)
 *   [rbp-80] .. [rbp-65]   frame0, the incoming DESCR_t, parked across the tidy call
 *   [rbp-64] .. [rbp-1]    c, the private copy of the popped rt_pcall_t
 *
 * rt_pcall_t OFFSETS — CONFIRMED FROM THE EMITTED CODE, NOT COMPUTED BY HAND
 *   (objdump of the -O0 build, s165; the 64-byte stride is visible as shl $0x6)
 *     p 0 . rname 8 . save_Σ 16 . save_Σlen 24 . save_base 28 . wn 32
 *     lex 36 . nargs 40 . fb 48 . vtmark 56 . sizeof 64
 *   rt_proc_t.is_generator is at 0x4c (visible as mov 0x4c(%rax),%eax).
 *   If any of these move, this file breaks LOUDLY in the differential battery —
 *   which is why that battery exists and runs before the gate is ever defaulted on.
 */
#include "rtx_abi.inc"

RTX_GATE_DEF(call)

#define PC_P        0
#define PC_FB       48
#define PC_VTMARK   56
#define PC_SIZE     64
#define PROC_ISGEN  0x4c

/* NULVCL = { .v = DT_SNUL, .slen = 0, .s = "" }. The value word is a POINTER to an
 * empty string, not zero, so the ω landing hands the body the identical descriptor
 * the C did. (The body provably never reads it when failed=1 — frame0 appears only
 * in the c->lex arm's `failed ? FAILDESCR : ...` ternary — but reproducing it
 * exactly costs two instructions and removes the question entirely.) */
.section .rodata
.align 1
.Lrtx_nulstr:
    .byte 0
.text

/*-----------------------------------------------------------------------------
 * DESCR_t rt_proc_call_epilogue_γ(DESCR_t frame0)   C of record: rt/rt.c:1043
 *
 *   rt_k_level--;
 *   if (g_pcall_top <= 0) return FAILDESCR;
 *   rt_pcall_t c = g_pcall[--g_pcall_top];
 *   if (c.p && !c.p->is_generator)
 *       rt_value_trail_tidy_dead_window(c.vtmark, c.fb, frame_address(0) + 16);
 *   return rt_proc_epilogue_body(&c, 0, frame0);
 *
 *   Arg pair: rdi = (slen<<32)|v, rsi = value.  Return pair: rax, rdx.
 *   __builtin_frame_address(0) is exactly rbp under the standard prologue, so the
 *   dead-window upper bound is lea rdx,[rbp+16] — frame-size independent, it names
 *   the first byte above this activation either way.
 */
RTX_FUNC(rt_proc_call_epilogue_γ)
    RTX_GATE(call, c_rt_proc_call_epilogue_γ)
    push    rbp
    mov     rbp, rsp
    sub     rsp, 80
    mov     qword ptr [rbp-80], rdi         /* park frame0 across the tidy call  */
    mov     qword ptr [rbp-72], rsi
    dec     dword ptr [rip + rt_k_level]    /* hidden => rip-direct, one op      */
    mov     ecx, dword ptr [rip + g_pcall_top]
    test    ecx, ecx
    jle     .Lg_fail
    dec     ecx                             /* --g_pcall_top, written back once  */
    mov     dword ptr [rip + g_pcall_top], ecx
    mov     r8, qword ptr [rip + g_pcall]
    movsxd  r9, ecx
    shl     r9, 6                           /* * sizeof(rt_pcall_t) == 64        */
    add     r8, r9                          /* r8 = &g_pcall[top]                */
    movdqu  xmm0, xmmword ptr [r8]          /* the private copy: 4 moves, not 16 */
    movdqu  xmm1, xmmword ptr [r8+16]
    movdqu  xmm2, xmmword ptr [r8+32]
    movdqu  xmm3, xmmword ptr [r8+48]
    movdqu  xmmword ptr [rbp-64], xmm0
    movdqu  xmmword ptr [rbp-48], xmm1
    movdqu  xmmword ptr [rbp-32], xmm2
    movdqu  xmmword ptr [rbp-16], xmm3
    mov     rax, qword ptr [rbp-64+PC_P]    /* c.p                               */
    test    rax, rax
    je      .Lg_body
    cmp     dword ptr [rax + PROC_ISGEN], 0
    jne     .Lg_body
    mov     edi, dword ptr [rbp-64+PC_VTMARK]
    mov     rsi, qword ptr [rbp-64+PC_FB]
    lea     rdx, [rbp+16]
    call    rt_value_trail_tidy_dead_window
.Lg_body:
    lea     rdi, [rbp-64]                   /* &c — must outlive the call, so    */
    xor     esi, esi                        /* this is a call, never a tail jmp  */
    mov     rdx, qword ptr [rbp-80]         /* frame0 pair as arg 3              */
    mov     rcx, qword ptr [rbp-72]
    call    rt_proc_epilogue_body
    leave
    ret
.Lg_fail:
    mov     eax, DT_FAIL
    xor     edx, edx
    leave
    ret
RTX_ENDF(rt_proc_call_epilogue_γ)

/*-----------------------------------------------------------------------------
 * DESCR_t rt_proc_call_epilogue_ω(void)             C of record: rt/rt.c:1055
 *
 *   Manual Ch.8, verbatim: "Transferring to the special label FRETURN returns
 *   from a function signaling failure to the caller.  No value is returned as the
 *   function result."  Arriving here IS the failure signal; no frame value is
 *   read.  Identical shape to γ minus the incoming descriptor, with failed = 1.
 */
RTX_FUNC(rt_proc_call_epilogue_ω)
    RTX_GATE(call, c_rt_proc_call_epilogue_ω)
    push    rbp
    mov     rbp, rsp
    sub     rsp, 64
    dec     dword ptr [rip + rt_k_level]
    mov     ecx, dword ptr [rip + g_pcall_top]
    test    ecx, ecx
    jle     .Lw_fail
    dec     ecx
    mov     dword ptr [rip + g_pcall_top], ecx
    mov     r8, qword ptr [rip + g_pcall]
    movsxd  r9, ecx
    shl     r9, 6
    add     r8, r9
    movdqu  xmm0, xmmword ptr [r8]
    movdqu  xmm1, xmmword ptr [r8+16]
    movdqu  xmm2, xmmword ptr [r8+32]
    movdqu  xmm3, xmmword ptr [r8+48]
    movdqu  xmmword ptr [rbp-64], xmm0
    movdqu  xmmword ptr [rbp-48], xmm1
    movdqu  xmmword ptr [rbp-32], xmm2
    movdqu  xmmword ptr [rbp-16], xmm3
    mov     rax, qword ptr [rbp-64+PC_P]
    test    rax, rax
    je      .Lw_body
    cmp     dword ptr [rax + PROC_ISGEN], 0
    jne     .Lw_body
    mov     edi, dword ptr [rbp-64+PC_VTMARK]
    mov     rsi, qword ptr [rbp-64+PC_FB]
    lea     rdx, [rbp+16]
    call    rt_value_trail_tidy_dead_window
.Lw_body:
    lea     rdi, [rbp-64]
    mov     esi, 1                          /* failed = 1                        */
    xor     edx, edx                        /* NULVCL: v = DT_SNUL, slen = 0     */
    lea     rcx, [rip + .Lrtx_nulstr]       /* NULVCL: .s = ""                   */
    call    rt_proc_epilogue_body
    leave
    ret
.Lw_fail:
    mov     eax, DT_FAIL
    xor     edx, edx
    leave
    ret
RTX_ENDF(rt_proc_call_epilogue_ω)

/*=============================================================================
 * RTX-4 SLICE 2 — THE SLIM LANDINGS (BP-7 static-save-set call convention).
 *
 * WHY THESE AND NOT THE CLASSIC PAIR AGAIN
 *   Slice 1's classic landing is reached by exactly 4 of 315 corpus programs,
 *   all EVAL/defer — MEASURED, not estimated, by the slice-1 falsification probe
 *   (s187 re-ran it: gate ON + broken asm moves m3 314/1 -> 310/5, and the four
 *   movers ARE expr_eval / 140_pat_eval_double_fn_trick /
 *   141_pat_eval_double_fn_arbno / 161_pat_defer_fn_nested_match).  Ordinary
 *   DEFINE calls land HERE instead.  This is the hot pair.
 *
 * THEY ARE A BETTER TARGET THAN SLICE 1 WAS: SELF-CONTAINED
 *   No rt_proc_epilogue_body call — the whole epilogue is in scope, so the asm
 *   SUBSUMES it rather than tail-calling into C.  No c.lex arm, no rcell /
 *   NV_GET_fn result fetch (the result ARRIVES as the argument), no
 *   rt_name_restore (open_slim parks save_base = -1; the emitted static arm did
 *   the saves inline).  What remains is exactly: k_level, the pop, the tidy, the
 *   Sigma restore, rt_nret_fix, the monitor event.
 *
 * THE DEAD-WINDOW UPPER BOUND — THE RUNG'S FLAGGED PRINCIPAL RISK, RESOLVED
 *   The C passes (char *)__builtin_frame_address(0) + 16.  No grep can surface
 *   this (it is an intrinsic, not a name), and the fear of record was that the
 *   asm's frame differs from the -O0 C's, so a transliterated bound would be
 *   wrong in a direction the crosscheck is WORST at catching (too high tidies
 *   LIVE trail entries = silent corruption).
 *   IT IS NOT FRAME-SIZE DEPENDENT.  Under the standard prologue
 *   (push rbp; mov rbp,rsp) gcc's __builtin_frame_address(0) IS rbp, and
 *   [rbp+0]=saved rbp, [rbp+8]=return address, so rbp+16 is the caller's rsp
 *   immediately before its `call` — the first byte above THIS activation,
 *   whatever `sub rsp,N` follows.  lea rdx,[rbp+16] is therefore byte-exact for
 *   any frame size, and slice 1 has shipped precisely this since s165.
 *   MEASURED, NOT ASSUMED: the s187 probe proved slice 1's asm — including this
 *   very lea and the tidy call it feeds — actually EXECUTES on all four covering
 *   programs, which still match their oracles.  Before that probe ON==OFF was
 *   ambiguous; it no longer is.  The bound is live, correct, and portable here.
 *
 * LINKAGE SPLIT — NEW HAZARD AT THIS SLICE, ABSENT FROM SLICE 1
 *   Slice 1 never touched Sigma / Sigmalen / g_monitor_bin: they live inside
 *   rt_proc_epilogue_body, which it CALLS.  A self-contained port must reach
 *   them directly, and they are EXPORTED (nm: capital B) hence PREEMPTIBLE, so
 *   they need @GOTPCREL — the exact link failure RTX-2 hit on g_hp_fr.  By
 *   contrast g_pcall / g_pcall_top / rt_k_level are visibility-hidden, which the
 *   linker localises in the .so, so those stay rip-direct as in slice 1.
 *   Getting this backwards is a link error, not a silent bug — but only because
 *   the split was checked with nm rather than assumed from the C.
 *
 * FAITHFUL TRANSLITERATION NOTE (deliberate, not an oversight)
 *   The monitor arm dereferences c.p->name with NO null guard, exactly as the C
 *   does, even though the tidy arm two lines above DOES guard c.p.  If c.p can
 *   ever be null with the monitor armed, the C segfaults too; reproducing the
 *   asymmetry keeps the port a port.  The monitor is dark today (g_monitor_bin
 *   is 0), so this branch is untaken in every battery — recorded so the next
 *   session does not read its greenness as evidence about it.
 *
 * rt_pcall_t OFFSETS ARE LOCKED IN THE BUILD, NOT TRUSTED FROM THIS COMMENT:
 *   rt.c carries _Static_asserts on sizeof==64 and on p/save_S/save_Slen/wn/fb/
 *   vtmark, plus rt_proc_t.name==0 and is_generator==0x4c.  Layout drift now
 *   fails the COMPILE instead of shearing this file silently.
 */
#define PC_SAVE_S     16
#define PC_SAVE_SLEN  24
#define PC_WN         32
#define PROC_NAME     0

/*-----------------------------------------------------------------------------
 * DESCR_t rt_proc_call_epilogue_slim_γ(DESCR_t result)   C: rt/rt.c c_ fallback
 *   Arg pair rdi/rsi = the returned descriptor; return pair rax/rdx.
 * FRAME (80 B, rsp stays 16-aligned at every call site)
 *   [rbp-80],[rbp-72]  result, parked across tidy / nret_fix / monitor
 *   [rbp-64] .. [rbp-1] c, the private copy of the popped record
 */
RTX_FUNC(rt_proc_call_epilogue_slim_γ)
    RTX_GATE(call, c_rt_proc_call_epilogue_slim_γ)
    push    rbp
    mov     rbp, rsp
    sub     rsp, 80
    mov     qword ptr [rbp-80], rdi
    mov     qword ptr [rbp-72], rsi
    dec     dword ptr [rip + rt_k_level]     /* hidden => rip-direct              */
    mov     ecx, dword ptr [rip + g_pcall_top]
    test    ecx, ecx
    jle     .Lsg_fail
    dec     ecx
    mov     dword ptr [rip + g_pcall_top], ecx
    mov     r8, qword ptr [rip + g_pcall]
    movsxd  r9, ecx
    shl     r9, 6                            /* * sizeof(rt_pcall_t) == 64        */
    add     r8, r9
    movdqu  xmm0, xmmword ptr [r8]           /* private copy: 4 moves, not 16     */
    movdqu  xmm1, xmmword ptr [r8+16]
    movdqu  xmm2, xmmword ptr [r8+32]
    movdqu  xmm3, xmmword ptr [r8+48]
    movdqu  xmmword ptr [rbp-64], xmm0
    movdqu  xmmword ptr [rbp-48], xmm1
    movdqu  xmmword ptr [rbp-32], xmm2
    movdqu  xmmword ptr [rbp-16], xmm3
    mov     rax, qword ptr [rbp-64+PC_P]
    test    rax, rax
    je      .Lsg_sigma
    cmp     dword ptr [rax + PROC_ISGEN], 0
    jne     .Lsg_sigma
    mov     edi, dword ptr [rbp-64+PC_VTMARK]
    mov     rsi, qword ptr [rbp-64+PC_FB]
    lea     rdx, [rbp+16]                    /* see the bound discussion above    */
    call    rt_value_trail_tidy_dead_window
.Lsg_sigma:
    mov     rax, qword ptr [rip + Σ@GOTPCREL]     /* exported => GOT, not lea     */
    mov     rcx, qword ptr [rbp-64+PC_SAVE_S]
    mov     qword ptr [rax], rcx
    mov     rax, qword ptr [rip + Σlen@GOTPCREL]
    mov     ecx, dword ptr [rbp-64+PC_SAVE_SLEN]
    mov     dword ptr [rax], ecx                  /* Σlen is int, 4-byte store    */
    mov     rdi, qword ptr [rbp-80]
    mov     rsi, qword ptr [rbp-72]
    mov     edx, dword ptr [rbp-64+PC_WN]
    call    rt_nret_fix                           /* de-static'd + hidden, s187   */
    mov     qword ptr [rbp-80], rax
    mov     qword ptr [rbp-72], rdx
    mov     rcx, qword ptr [rip + g_monitor_bin@GOTPCREL]
    cmp     dword ptr [rcx], 0
    je      .Lsg_ret
    mov     rcx, qword ptr [rbp-64+PC_P]
    mov     rdi, qword ptr [rcx + PROC_NAME]
    mov     rsi, qword ptr [rbp-80]
    mov     rdx, qword ptr [rbp-72]
    call    mon_emit_return_bin
.Lsg_ret:
    mov     rax, qword ptr [rbp-80]
    mov     rdx, qword ptr [rbp-72]
    leave
    ret
.Lsg_fail:
    mov     eax, DT_FAIL
    xor     edx, edx
    leave
    ret
RTX_ENDF(rt_proc_call_epilogue_slim_γ)

/*-----------------------------------------------------------------------------
 * DESCR_t rt_proc_call_epilogue_slim_ω(void)
 *   Manual Ch.8: FRETURN signals failure and yields no value.  Identical to the
 *   slim γ except the descriptor handed to rt_nret_fix is FAILDESCR (99, 0)
 *   rather than an incoming one — the C writes rt_nret_fix(FAILDESCR, c.wn), so
 *   the wn side effect still runs on the failure path and must not be skipped.
 */
RTX_FUNC(rt_proc_call_epilogue_slim_ω)
    RTX_GATE(call, c_rt_proc_call_epilogue_slim_ω)
    push    rbp
    mov     rbp, rsp
    sub     rsp, 80
    dec     dword ptr [rip + rt_k_level]
    mov     ecx, dword ptr [rip + g_pcall_top]
    test    ecx, ecx
    jle     .Lsw_fail
    dec     ecx
    mov     dword ptr [rip + g_pcall_top], ecx
    mov     r8, qword ptr [rip + g_pcall]
    movsxd  r9, ecx
    shl     r9, 6
    add     r8, r9
    movdqu  xmm0, xmmword ptr [r8]
    movdqu  xmm1, xmmword ptr [r8+16]
    movdqu  xmm2, xmmword ptr [r8+32]
    movdqu  xmm3, xmmword ptr [r8+48]
    movdqu  xmmword ptr [rbp-64], xmm0
    movdqu  xmmword ptr [rbp-48], xmm1
    movdqu  xmmword ptr [rbp-32], xmm2
    movdqu  xmmword ptr [rbp-16], xmm3
    mov     rax, qword ptr [rbp-64+PC_P]
    test    rax, rax
    je      .Lsw_sigma
    cmp     dword ptr [rax + PROC_ISGEN], 0
    jne     .Lsw_sigma
    mov     edi, dword ptr [rbp-64+PC_VTMARK]
    mov     rsi, qword ptr [rbp-64+PC_FB]
    lea     rdx, [rbp+16]
    call    rt_value_trail_tidy_dead_window
.Lsw_sigma:
    mov     rax, qword ptr [rip + Σ@GOTPCREL]
    mov     rcx, qword ptr [rbp-64+PC_SAVE_S]
    mov     qword ptr [rax], rcx
    mov     rax, qword ptr [rip + Σlen@GOTPCREL]
    mov     ecx, dword ptr [rbp-64+PC_SAVE_SLEN]
    mov     dword ptr [rax], ecx
    mov     edi, DT_FAIL                          /* FAILDESCR word0 = (0<<32)|99 */
    xor     esi, esi                              /* FAILDESCR word1 = 0          */
    mov     edx, dword ptr [rbp-64+PC_WN]
    call    rt_nret_fix
    mov     qword ptr [rbp-80], rax
    mov     qword ptr [rbp-72], rdx
    mov     rcx, qword ptr [rip + g_monitor_bin@GOTPCREL]
    cmp     dword ptr [rcx], 0
    je      .Lsw_ret
    mov     rcx, qword ptr [rbp-64+PC_P]
    mov     rdi, qword ptr [rcx + PROC_NAME]
    mov     rsi, qword ptr [rbp-80]
    mov     rdx, qword ptr [rbp-72]
    call    mon_emit_return_bin
.Lsw_ret:
    mov     rax, qword ptr [rbp-80]
    mov     rdx, qword ptr [rbp-72]
    leave
    ret
.Lsw_fail:
    mov     eax, DT_FAIL
    xor     edx, edx
    leave
    ret
RTX_ENDF(rt_proc_call_epilogue_slim_ω)

/* Non-executable stack marker: without it ld marks the whole .so RWX-stack. */
.section .note.GNU-stack,"",@progbits
