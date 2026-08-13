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
 * FRAME LAYOUT (___-relative, 80 bytes, keeps rsp 16-aligned at every call)
 *   [___-80] .. [___-65]   frame0, the incoming DESCR_t, parked across the tidy call
 *   [___-64] .. [___-1]    c, the private copy of the popped rt_pcall_t
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
 *   __builtin_frame_address(0) is exactly ___ under the standard prologue, so the
 *   dead-window upper bound is lea rdx,[___+16] — frame-size independent, it names
 *   the first byte above this activation either way.
 */
RTX_FUNC(rt_proc_call_epilogue_γ)
    RTX_GATE(call, c_rt_proc_call_epilogue_γ)
    sub     rsp, 80
    mov     qword ptr [rsp - 1], rdi         /* park frame0 across the tidy call  */
    mov     qword ptr [rsp - 1], rsi
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
    movdqu  xmmword ptr [rsp - 1], xmm0
    movdqu  xmmword ptr [rsp - 1], xmm1
    movdqu  xmmword ptr [rsp - 1], xmm2
    movdqu  xmmword ptr [rsp - 1], xmm3
    mov     rax, qword ptr [rsp - 1]    /* c.p                               */
    test    rax, rax
    je      .Lg_body
    cmp     dword ptr [rax + PROC_ISGEN], 0
    jne     .Lg_body
    mov     edi, dword ptr [rsp - 1]
    mov     rsi, qword ptr [rsp - 1]
    lea     rdx, [rsp - 1]
    call    rt_value_trail_tidy_dead_window
.Lg_body:
    lea     rdi, [rsp - 1]                   /* &c — must outlive the call, so    */
    xor     esi, esi                        /* this is a call, never a tail jmp  */
    mov     rdx, qword ptr [rsp - 1]         /* frame0 pair as arg 3              */
    mov     rcx, qword ptr [rsp - 1]
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
    movdqu  xmmword ptr [rsp - 1], xmm0
    movdqu  xmmword ptr [rsp - 1], xmm1
    movdqu  xmmword ptr [rsp - 1], xmm2
    movdqu  xmmword ptr [rsp - 1], xmm3
    mov     rax, qword ptr [rsp - 1]
    test    rax, rax
    je      .Lw_body
    cmp     dword ptr [rax + PROC_ISGEN], 0
    jne     .Lw_body
    mov     edi, dword ptr [rsp - 1]
    mov     rsi, qword ptr [rsp - 1]
    lea     rdx, [rsp - 1]
    call    rt_value_trail_tidy_dead_window
.Lw_body:
    lea     rdi, [rsp - 1]
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
 *   (push ___; mov ___,rsp) gcc's __builtin_frame_address(0) IS ___, and
 *   [___+0]=saved ___, [___+8]=return address, so ___+16 is the caller's rsp
 *   immediately before its `call` — the first byte above THIS activation,
 *   whatever `sub rsp,N` follows.  lea rdx,[___+16] is therefore byte-exact for
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
 *   [___-80],[___-72]  result, parked across tidy / nret_fix / monitor
 *   [___-64] .. [___-1] c, the private copy of the popped record
 */
RTX_FUNC(rt_proc_call_epilogue_slim_γ)
    RTX_GATE(call, c_rt_proc_call_epilogue_slim_γ)
    sub     rsp, 80
    mov     qword ptr [rsp - 1], rdi
    mov     qword ptr [rsp - 1], rsi
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
    movdqu  xmmword ptr [rsp - 1], xmm0
    movdqu  xmmword ptr [rsp - 1], xmm1
    movdqu  xmmword ptr [rsp - 1], xmm2
    movdqu  xmmword ptr [rsp - 1], xmm3
    mov     rax, qword ptr [rsp - 1]
    test    rax, rax
    je      .Lsg_sigma
    cmp     dword ptr [rax + PROC_ISGEN], 0
    jne     .Lsg_sigma
    mov     edi, dword ptr [rsp - 1]
    mov     rsi, qword ptr [rsp - 1]
    lea     rdx, [rsp - 1]                    /* see the bound discussion above    */
    call    rt_value_trail_tidy_dead_window
.Lsg_sigma:
    mov     rax, qword ptr [rip + Σ@GOTPCREL]     /* exported => GOT, not lea     */
    mov     rcx, qword ptr [rsp - 1]
    mov     qword ptr [rax], rcx
    mov     rax, qword ptr [rip + Σlen@GOTPCREL]
    mov     ecx, dword ptr [rsp - 1]
    mov     dword ptr [rax], ecx                  /* Σlen is int, 4-byte store    */
    mov     rdi, qword ptr [rsp - 1]
    mov     rsi, qword ptr [rsp - 1]
    mov     edx, dword ptr [rsp - 1]
    call    rt_nret_fix                           /* de-static'd + hidden, s187   */
    mov     qword ptr [rsp - 1], rax
    mov     qword ptr [rsp - 1], rdx
    mov     rcx, qword ptr [rip + g_monitor_bin@GOTPCREL]
    cmp     dword ptr [rcx], 0
    je      .Lsg_ret
    mov     rcx, qword ptr [rsp - 1]
    mov     rdi, qword ptr [rcx + PROC_NAME]
    mov     rsi, qword ptr [rsp - 1]
    mov     rdx, qword ptr [rsp - 1]
    call    mon_emit_return_bin
.Lsg_ret:
    mov     rax, qword ptr [rsp - 1]
    mov     rdx, qword ptr [rsp - 1]
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
    movdqu  xmmword ptr [rsp - 1], xmm0
    movdqu  xmmword ptr [rsp - 1], xmm1
    movdqu  xmmword ptr [rsp - 1], xmm2
    movdqu  xmmword ptr [rsp - 1], xmm3
    mov     rax, qword ptr [rsp - 1]
    test    rax, rax
    je      .Lsw_sigma
    cmp     dword ptr [rax + PROC_ISGEN], 0
    jne     .Lsw_sigma
    mov     edi, dword ptr [rsp - 1]
    mov     rsi, qword ptr [rsp - 1]
    lea     rdx, [rsp - 1]
    call    rt_value_trail_tidy_dead_window
.Lsw_sigma:
    mov     rax, qword ptr [rip + Σ@GOTPCREL]
    mov     rcx, qword ptr [rsp - 1]
    mov     qword ptr [rax], rcx
    mov     rax, qword ptr [rip + Σlen@GOTPCREL]
    mov     ecx, dword ptr [rsp - 1]
    mov     dword ptr [rax], ecx
    mov     edi, DT_FAIL                          /* FAILDESCR word0 = (0<<32)|99 */
    xor     esi, esi                              /* FAILDESCR word1 = 0          */
    mov     edx, dword ptr [rsp - 1]
    call    rt_nret_fix
    mov     qword ptr [rsp - 1], rax
    mov     qword ptr [rsp - 1], rdx
    mov     rcx, qword ptr [rip + g_monitor_bin@GOTPCREL]
    cmp     dword ptr [rcx], 0
    je      .Lsw_ret
    mov     rcx, qword ptr [rsp - 1]
    mov     rdi, qword ptr [rcx + PROC_NAME]
    mov     rsi, qword ptr [rsp - 1]
    mov     rdx, qword ptr [rsp - 1]
    call    mon_emit_return_bin
.Lsw_ret:
    mov     rax, qword ptr [rsp - 1]
    mov     rdx, qword ptr [rsp - 1]
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

/*=========================================================================================================
 * RTX-4 SLICE 3 — THE ENTRY-SIDE LEAVES: rt_flat_ret_snap AND rt_proc_open_fn
 *
 * WHY THESE TWO, AND WHY TOGETHER
 *   Slice 1 ported the two classic epilogue LANDINGS (the gamma/omega exits).  These are the other end of
 *   the same boundary: the RETURN floater's wire fetch and the call site's fn fetch.  Both are strict
 *   leaves, both take NO arguments and return a single void*, and both are called EXACTLY ONCE PER
 *   SNOBOL4 PROCEDURE CALL -- measured 1:1 with the loop count at two scales (1,000,000 and 2,000,000
 *   calls in func_call.sno), which is what step 0(d) demands and what makes this slice worth doing.
 *   Manual Ch.8: every DEFINE'd function reaches RETURN (or FRETURN/NRETURN) exactly once per activation,
 *   so "once per call" is a property of the LANGUAGE here, not an accident of the benchmark.
 *
 * WHAT THE -O0 C SPENT, MEASURED BY objdump AT HEAD (not estimated)
 *   rt_flat_ret_snap  ~48 instructions: a full ___ frame plus a pushed rbx (a PIN, saved only because the
 *                     40-byte copy was emitted as 5 scalar load/store PAIRS = 10 memory ops); g_pcall_top
 *                     loaded TWICE; g_pcall_wires loaded TWICE; the w pointer spilled to stack and
 *                     reloaded THREE times; a jmp to rejoin the ternary's two arms.
 *   rt_proc_open_fn   16 instructions: an ___ frame for a function that touches no stack at all, and
 *                     g_pcall_top loaded TWICE to perform ONE decrement.
 *
 * WHAT THIS PORT DOES INSTEAD
 *   Each global is loaded ONCE.  Neither entry builds a frame (both are strict leaves; nothing on the hot
 *   path calls out, so there is nothing to align for and no return address to protect).  The 40-byte
 *   wire copy becomes 2 SSE 16-byte moves + 1 qword = 6 memory ops instead of 10, and because no pin is
 *   used as copy scratch, the rbx save/restore disappears entirely.
 *
 * THE ERROR PATHS ARE DELIBERATELY NOT PORTED
 *   Every failure edge -- level-zero RETURN (manual Ch.8's "Return from level zero", core error 18, which
 *   must route through the core machinery so &ERROR/SETEXIT trapping still applies) and the
 *   no-return-wires diagnostic -- TAIL-JUMPS to the C body.  The asm implements the SUCCESS path only.
 *   Consequence worth stating plainly: this port CANNOT regress error behaviour, because it does not
 *   contain any.  The diagnostics stay byte-identical by construction, not by testing.
 *
 * REGISTER SAFETY
 *   Uses rax rcx rdx r8 xmm0 xmm1 -- all inside the ARCH section 2 working set.  No pin is touched.
 *   rdx IS clobbered, and that is provably safe rather than merely assumed: the -O0 C body already
 *   clobbers rdx (objdump: movslq %eax,%rdx), so no emitted caller can legally depend on it surviving.
 *   The rt.c comment about "rax:rdx riding untouched through the floater's tail" describes the floater's
 *   own tail AFTER this call returns, not a guarantee across it -- verified against the emitted site in
 *   func_call.s, which consumes only rax and then reloads rcx/___/rsp from the returned buffer.
 *
 * OFFSETS -- ALL ANCHORED BY _Static_assert IN rt.c, SO A STRUCT MOVE BREAKS THE BUILD, NOT THE RUNTIME
 *   rt_pcall_t      sizeof 64 (stride shl 6), p at 0                    (rt.c assert, pre-existing)
 *   rt_proc_t       fn at 8                                             (rt.c assert, added this slice)
 *   rt_flat_wires_t sizeof 40, gw 0, ww 8, rsp 16, ___ 24, r12 32       (rt.c assert, Z4-7 slice 2)
 *   The 40-byte stride is NOT a power of two: index*40 == ((index*5)<<3), done as lea+shl.
 *=======================================================================================================*/

#define PROC_FN     8
#define FW_GW       0
#define FW_WW       8
#define FW_SIZE     40

/* void *rt_proc_open_fn(void) -- the emitted call site's callee-entry fetch.
 * C of record: if (g_pcall_top <= 0) return 0; return g_pcall[g_pcall_top-1].p->fn; */
RTX_FUNC(rt_proc_open_fn)
    RTX_GATE(call, c_rt_proc_open_fn)
    mov     eax, dword ptr [rip + g_pcall_top]      /* the ONE load of top */
    test    eax, eax
    jle     .Lopenfn_zero                           /* no open activation: return NULL, exactly as C */
    dec     eax                                     /* top - 1 */
    movsxd  rax, eax
    shl     rax, 6                                  /* * sizeof(rt_pcall_t) == 64 */
    add     rax, qword ptr [rip + g_pcall]
    mov     rax, qword ptr [rax + PC_P]             /* c->p   (no NULL check: the C has none either) */
    mov     rax, qword ptr [rax + PROC_FN]          /* p->fn */
    ret
.Lopenfn_zero:
    xor     eax, eax
    ret
RTX_ENDF(rt_proc_open_fn)

/* void *rt_flat_ret_snap(void) -- the RETURN/FRETURN floater's wire fetch.  PEEKS (never pops) the top
 * activation's wire quad into a private static buffer and returns its address.  The copy is what makes
 * the peek safe against a re-entrant push landing on the same slot, so it is preserved exactly. */
RTX_FUNC(rt_flat_ret_snap)
    RTX_GATE(call, c_rt_flat_ret_snap)
    mov     eax, dword ptr [rip + g_pcall_top]      /* the ONE load of top */
    test    eax, eax
    jle     .Lsnap_c                                /* level-zero RETURN -> C raises core error 18 */
    mov     rcx, qword ptr [rip + g_pcall_wires]    /* the ONE load of the wire array */
    test    rcx, rcx
    jz      .Lsnap_c                                /* no wire array -> C diagnostic */
    dec     eax
    movsxd  rax, eax
    lea     rax, [rax + rax*4]                      /* index * 5 */
    shl     rax, 3                                  /* * 8  => * 40 == sizeof(rt_flat_wires_t) */
    add     rcx, rax                                /* rcx = w = &g_pcall_wires[top-1] */
    mov     rax, qword ptr [rcx + FW_GW]
    test    rax, rax
    jz      .Lsnap_c                                /* !w->gw -> C prints the un-adopted diagnostic */
    mov     rdx, qword ptr [rcx + FW_WW]
    test    rdx, rdx
    jz      .Lsnap_c                                /* !w->ww -> same */
    movdqu  xmm0, xmmword ptr [rcx +  0]            /* gw, ww  */
    movdqu  xmm1, xmmword ptr [rcx + 16]            /* rsp, ___ */
    mov     r8,  qword ptr [rcx + 32]               /* r12 (Z4-7 slice 2 island caller-base) */
    lea     rax, [rip + g_flat_ret_snapbuf]
    movdqu  xmmword ptr [rax +  0], xmm0
    movdqu  xmmword ptr [rax + 16], xmm1
    mov     qword ptr [rax + 32], r8
    ret                                             /* rax = &g_flat_ret_snapbuf */
.Lsnap_c:
    jmp     c_rt_flat_ret_snap                      /* every failure edge is C's, unchanged */
RTX_ENDF(rt_flat_ret_snap)

/*=======================================================================================================
 * RTX-4 SLICE 3A — rt_flat_wire_adopt, THE WRITE SIDE OF THE RECORD SLICE 3 ALREADY READS
 *
 * WHY THIS ONE, AND WHY IT WAS INVISIBLE UNTIL NOW
 *   The benchmark artifacts carry only SIX static call sites for this symbol, which ranks it 16th of the
 *   46 symbols the SNOBOL4 benchmarks call -- below rt_call_arr (99 sites), NV_SET_fn (43) and every
 *   rt_proc_set_* (16 each).  Measured with the s217 interposer across all 21 benchmarks it takes
 *   23,743,053 ENTRIES, fourth-highest in the whole still-C set, and on func_call/func_call_overhead it
 *   is EXACTLY 10,000,000 == the loop count, i.e. 1:1 with SNOBOL4 procedure calls at two scales.
 *   Static site rank is a property of the EMITTER'S REACH; this is the s188/s225 lesson arriving a third
 *   time, and the only instrument that sees it is step 0(d).
 *
 * IT IS THE EXACT COUNTERPART OF rt_flat_ret_snap ABOVE
 *   snap PEEKS the top activation's wire quad on the RETURN floater; adopt WRITES that same quad on the
 *   stub blob's WIRE-ADOPT box right after the jmp-entry prologue.  Same two globals, same struct, same
 *   stride, same family gate.  Every 0(c) fact this port needs was therefore already proven by its
 *   sibling and RE-VERIFIED on the OBJECT FILE this session (never the .so -- ARCH section 7 step 0(c)):
 *   g_pcall_top and g_pcall_wires are GLOBAL HIDDEN, absent from the .so dynamic table, so [rip+sym] is
 *   direct and @GOTPCREL is NOT owed.
 *
 * WHAT THE -O0 C SPENDS (objdump at HEAD, measured not estimated)
 *   A full ___ frame for a leaf that touches no stack, all FOUR pointer arguments spilled to [___-N] and
 *   reloaded, g_pcall_top loaded TWICE, g_pcall_wires loaded TWICE, and the w pointer spilled and
 *   reloaded across the five stores.  The port loads each global ONCE, keeps all four arguments in their
 *   incoming SysV registers, builds no frame, and stores five fields straight through r8.
 *
 * THE GUARD IS A SILENT NO-OP AND THAT IS LOAD-BEARING, NOT AN OVERSIGHT
 *   rt.c's own comment records why: a blob entered with no open activation (LBL__/CODE chain-enter) has
 *   nothing to adopt into, and its RETURN legitimately belongs to the ENCLOSING open call, whose adopt
 *   already filled the top record.  So the C RETURNS on the guard rather than erroring, and this port
 *   returns on the same two conditions in the same order.  There is no error path here to get wrong --
 *   unlike snap above, which tail-jumps to C on all four of its failure edges.
 *
 * REGISTER SAFETY
 *   Uses rax and r8 only, both inside the ARCH section 2 working set; rdi/rsi/rdx/rcx are read in place
 *   and never written.  No pin is touched, so nothing needs saving.  Return value is void, so rax is
 *   free scratch by the SysV contract.
 *
 * OFFSETS -- anchored by the _Static_assert at rt.c:1026 (sizeof 40, gw 0, ww 8, rsp 16, ___ 24, r12 32),
 *   so a struct move breaks the BUILD rather than the runtime.  40 is not a power of two: index*40 is
 *   done as lea(index*5) + shl 3, identical to snap's arithmetic above.
 *=======================================================================================================*/

#define FW_RSP     16
#define FW_FB5     24
#define FW_R12     32

/* void rt_flat_wire_adopt(void *gw, void *ww, void *rsp, void *___)
 * C of record (rt.c:1051):
 *   if (g_pcall_top <= 0 || !g_pcall_wires) return;
 *   w = &g_pcall_wires[g_pcall_top - 1]; w->gw=gw; w->ww=ww; w->rsp=rsp; w->___=___; w->r12=0; */
RTX_FUNC(rt_flat_wire_adopt)
    RTX_GATE(call, c_rt_flat_wire_adopt)
    mov     eax, dword ptr [rip + g_pcall_top]      /* the ONE load of top */
    test    eax, eax
    jle     .Ladopt_done                            /* no open activation: silent no-op, exactly as C */
    mov     r8, qword ptr [rip + g_pcall_wires]     /* the ONE load of the wire array */
    test    r8, r8
    jz      .Ladopt_done                            /* no wire array: silent no-op, same order as C */
    dec     eax                                     /* top - 1 */
    movsxd  rax, eax
    lea     rax, [rax + rax*4]                      /* index * 5 */
    shl     rax, 3                                  /* * 8  => * 40 == sizeof(rt_flat_wires_t) */
    add     r8, rax                                 /* r8 = w = &g_pcall_wires[top-1] */
    mov     qword ptr [r8 + FW_GW],  rdi            /* w->gw  = gw   (arg1, still in place) */
    mov     qword ptr [r8 + FW_WW],  rsi            /* w->ww  = ww   (arg2) */
    mov     qword ptr [r8 + FW_RSP], rdx            /* w->rsp = rsp  (arg3) */
    mov     qword ptr [r8 + FW_FB5], rcx            /* w->___ = ___  (arg4) */
    mov     qword ptr [r8 + FW_R12], 0              /* w->r12 = 0    (the _isle variant is a SEPARATE symbol and is untouched) */
.Ladopt_done:
    ret
RTX_ENDF(rt_flat_wire_adopt)
