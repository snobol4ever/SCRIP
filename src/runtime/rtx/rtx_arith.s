/* rtx_arith.s — RTX family ARITH (RTX-6, s203).
 *
 * READ .github/ARCH-SNOBOL4-RTX.md BEFORE EDITING. Contract macros: rtx_abi.inc.
 *
 * TARGET SELECTED BY MEASUREMENT, NOT BY THE RUNG TEXT (ARCH section 7 step 0(d)).
 * The RTX-6 rung names rt_num_arith as its primary target. An LD_PRELOAD counting
 * interposer (signature-agnostic asm trampolines, chained via dlsym(RTLD_NEXT))
 * measured it at ZERO calls in arith_loop at BOTH N=100000 and N=200000, while
 * rt_cmp_d scaled exactly 100000 -> 200000 in the same runs. Integer arithmetic is
 * inlined by the emitter and never reaches the C runtime at all; rt_num_arith is
 * reached only by REAL/MIXED arithmetic (a probe with X = X + 1.5 measured 50000).
 * Porting rt_num_arith and grading it on arith_loop would move the board by zero
 * BY CONSTRUCTION -- the rt_call_arr flat-8 vacuity of s188, exactly.
 *
 * rt_cmp_d is instead the #1 hottest C runtime call in every stable benchmark:
 *   var_access 10,000,001 . func_call 10,000,001 . string_manip 5,000,001
 *   table_access 5,016,003 . fibonacci 2,692,537 . op_dispatch 2,081,407
 * and it was entirely unported and ungated -- zero references in src/runtime/rtx/.
 *
 * C of record: src/runtime/rt/rt.c:296, renamed c_rt_cmp_d in this same commit.
 *   int rt_cmp_d(const DESCR_t *a, const DESCR_t *b)
 * ARGS ARE POINTERS, NOT DESCRIPTOR PAIRS: rdi = a, rsi = b. Result int in eax.
 * Field offsets (descr.h, asserted in rtx_init.c): v at 0, slen at 4, value at 8.
 *
 * BRANCH ORDER IS DELIBERATELY NOT THE C's. The C tests the string pair first and
 * the integer pair second; this port tests the integer pair FIRST because that is
 * the measured hot shape (LT(N,limit) on two DT_I). The reorder is semantics-
 * preserving because the two guards are MUTUALLY EXCLUSIVE: the string arm requires
 * both tags in {DT_SNUL=0, DT_S=1} and the integer arm requires both == DT_I=6, so
 * no descriptor pair can satisfy both. Reordering a pair of overlapping guards
 * would be a semantic change; this pair cannot overlap.
 *
 * THE IEEE TRAP THE C HIDES AND THE ASM MUST NOT (same class as rt_is_truthy's):
 * the fallthrough arm is (x < y) ? -1 : (x > y) ? 1 : 0. With a NaN operand BOTH
 * comparisons are false in C, so the result is 0. A single comisd + setb would be
 * WRONG: comisd sets CF=1 for unordered as well as for below, so NaN would report
 * -1. This port issues comisd in BOTH directions and uses seta twice -- seta is
 * (CF=0 and ZF=0), which is false for unordered in both directions, so NaN yields
 * 1-1... no: yields 0-0 = 0, matching C exactly.
 */
#include "rtx_abi.inc"

RTX_GATE_DEF(arith)

.section .rodata
.align 1
.Lcd_empty:
    .byte 0
.text

/*-----------------------------------------------------------------------------
 * int rt_cmp_d(const DESCR_t *a, const DESCR_t *b)
 */
RTX_FUNC(rt_cmp_d)
    mov     eax, dword ptr [rdi]        /* a->v                                */
    mov     ecx, dword ptr [rsi]        /* b->v                                */
    /* ---- HOT ARM: both DT_I. Measured shape of LT/GT/LE/GE on counters. ---- */
    cmp     al, DT_I
    jne     .Lcd_notint
    cmp     cl, DT_I
    jne     .Lcd_notint
    mov     rdx, qword ptr [rdi + 8]    /* a->i                                */
    cmp     rdx, qword ptr [rsi + 8]    /* signed compare against b->i         */
    setg    r11b
    setl    r10b
    sub     r11b, r10b                  /* 1 / -1 / 0, branchless              */
    movsx   eax, r11b
    ret
.Lcd_notint:
    /* ---- STRING ARM (s229 tag renumber): DT_S and DT_SNUL are the only tags
     * carrying DT_STRING_BIT, so "both operands are string family" is the
     * SINGLE combined test (a & b) & DT_STRING_BIT -- no per-operand branch.
     * This REPLACES the old unsigned `tag <= DT_S` range trick, which was only
     * valid while DT_SNUL==0 && DT_S==1 and would silently admit the wrong
     * datatypes under the new numbering.                                   ---- */
    mov     r10d, eax
    or      r10d, ecx
    test    r10b, (DT_NOTSTR_MASK & 0xFF)                 /* 8-BIT: r10d bits 8-31 carry the DESCR stamp (s256)        */
    jnz     .Lcd_real
    /* ptr_a = (a->v == DT_S && a->s) ? a->s : ""   -- DT_SNUL always yields ""
     * even when the union happens to hold a non-null pointer.                 */
    xor     edx, edx
    cmp     al, DT_S
    jne     .Lcd_a_fix
    mov     rdx, qword ptr [rdi + 8]
.Lcd_a_fix:
    test    rdx, rdx
    jnz     .Lcd_b
    lea     rdx, [rip + .Lcd_empty]
.Lcd_b:
    xor     r10d, r10d
    cmp     cl, DT_S
    jne     .Lcd_b_fix
    mov     r10, qword ptr [rsi + 8]
.Lcd_b_fix:
    test    r10, r10
    jnz     .Lcd_strloop
    lea     r10, [rip + .Lcd_empty]
    /* Inline unsigned byte compare rather than a libc strcmp call. This arm is
     * COLD in every measured benchmark (the hot shape is DT_I/DT_I), so the
     * libc-boundary realignment ceremony would buy nothing and cost a call.
     * strcmp compares as UNSIGNED char, hence movzx and the seta/setb pair.
     * ptr_b rides r10, not r8 -- r10's only other use in this function (the
     * DT_NOTSTR_MASK combine above) is dead by here, and keeping r8/r9 untouched
     * for the whole function is what licenses the RTCC veneer omission at every
     * call site (rtcc-veneer-strip-pure-asm; mask re-derived by
     * scripts/test_gate_rtcc_callee_class.sh, table in x86_asm.h).            */
.Lcd_strloop:
    movzx   eax, byte ptr [rdx]
    movzx   ecx, byte ptr [r10]
    cmp     al, cl
    jne     .Lcd_strdiff
    test    al, al
    je      .Lcd_streq
    inc     rdx
    inc     r10
    jmp     .Lcd_strloop
.Lcd_strdiff:
    seta    r11b                        /* flags still those of cmp al, cl     */
    setb    r10b                        /* r10's ptr_b role is dead by here    */
    sub     r11b, r10b
    movsx   eax, r11b
    ret
.Lcd_streq:
    xor     eax, eax
    ret
.Lcd_real:
    /* x = (a->v == DT_R) ? a->r : (double)a->i, and likewise y. The cvtsi2sd
     * arm reinterprets the union's 64 bits as a signed integer for EVERY tag
     * that is not DT_R -- including pointer-bearing tags. That is the existing
     * C semantics and this port reproduces it rather than improving it.      */
    cmp     al, DT_R
    je      .Lcd_a_real
    cvtsi2sd xmm0, qword ptr [rdi + 8]
    jmp     .Lcd_bval
.Lcd_a_real:
    movsd   xmm0, qword ptr [rdi + 8]
.Lcd_bval:
    cmp     cl, DT_R
    je      .Lcd_b_real
    cvtsi2sd xmm1, qword ptr [rsi + 8]
    jmp     .Lcd_cmp
.Lcd_b_real:
    movsd   xmm1, qword ptr [rsi + 8]
.Lcd_cmp:
    comisd  xmm0, xmm1
    seta    r11b                        /* x > y  (false when unordered)       */
    comisd  xmm1, xmm0
    seta    r10b                        /* y > x, i.e. x < y (false unordered) */
    sub     r11b, r10b                  /* NaN => 0 - 0 = 0, matching the C    */
    movsx   eax, r11b
    ret
RTX_ENDF(rt_cmp_d)

/*-----------------------------------------------------------------------------
 * DESCR_t rt_add/rt_sub/rt_mul(DESCR_t a, DESCR_t b)
 *
 * ARG PAIRS (ARCH section 2):  a = rdi:(slen<<32)|v , rsi:value
 *                              b = rdx:(slen<<32)|v , rcx:value
 * RETURN PAIR:                     rax:(slen<<32)|v , rdx:value
 * edi/edx alias the tag, so one 32-bit cmp per operand tests it.
 * INTVAL/REALVAL (core.h:21-22) leave slen ZERO, so "mov eax,DT_x" -- which
 * zero-extends into rax -- reproduces the C descriptor exactly.
 *
 * TWO FAST ARMS, BOTH MEASURED HOT s210 (LD_PRELOAD census, arith_mixed):
 *   rt_add is called 80,000,001 times, split 50/50 int-int and real-real,
 *   and the split is STABLE across a 2x loop scaling (160,000,001 calls).
 *   Everything else tail-jumps to the untouched C body with args in place.
 *
 * WHY BYPASSING setjmp IS SOUND ON THE REAL ARM -- the part that must be
 * argued, not assumed.  The C entry (arithmetic.c RT_BINOP_ENTRY) arms a
 * setjmp before calling rt_num_arith_impl so a core_runtime_error deep inside
 * can longjmp out as FAILDESCR.  For two DT_R operands that path is provably
 * unreachable for ADD/SUB/MUL: to_real(core.c:2010) returns v.r directly for
 * DT_R without calling anything, csop is 0, both IS_REAL_fn are 1 so anyf is
 * 1, and the switch arm is a bare REALVAL(ld op rd) -- no zero test, no fmod,
 * no pow, no error call.  DIV and MOD are NOT ported for exactly this reason
 * (both can raise on a zero divisor), and neither is POW (calls pow()).
 * The errjmp_n bookkeeping is likewise skippable: the C restores it to its
 * entry value before returning, so skipping the increment/decrement pair is
 * not observable.
 *
 * IEEE: addsd/subsd/mulsd ARE the operations gcc emits for double + - * on
 * SysV x86-64, so NaN, +-Inf, -0.0 and rounding all propagate bit-identically.
 */
RTX_FUNC(rt_add)
    RTX_GATE(arith, c_rt_add)
    cmp     dil, DT_I
    jne     .Ladd_notii
    cmp     dl, DT_I
    jne     .Ladd_notii
    lea     rdx, [rsi + rcx]            /* a.i + b.i, one instruction          */
    mov     eax, DT_I | (MOD_OP_RT_ADD << 8)   /* stamped (row descr-stamp-asm-mints), zero extra cost: same imm width */
    ret
.Ladd_notii:
    cmp     dil, DT_R
    jne     .Ladd_slow
    cmp     dl, DT_R
    jne     .Ladd_slow
    movq    xmm0, rsi
    movq    xmm1, rcx
    addsd   xmm0, xmm1
    movq    rdx, xmm0
    mov     eax, DT_R | (MOD_OP_RT_ADD << 8)   /* stamped (row descr-stamp-asm-mints) */
    ret
.Ladd_slow:
    jmp     c_rt_add                    /* rdi/rsi/rdx/rcx untouched above     */
RTX_ENDF(rt_add)

/*---------------------------------------------------------------------------*/
RTX_FUNC(rt_sub)
    RTX_GATE(arith, c_rt_sub)
    cmp     dil, DT_I
    jne     .Lsub_notii
    cmp     dl, DT_I
    jne     .Lsub_notii
    mov     rdx, rsi
    sub     rdx, rcx                    /* a.i - b.i                           */
    mov     eax, DT_I | (MOD_OP_RT_SUB << 8)   /* stamped (row descr-stamp-asm-mints), zero extra cost: same imm width */
    ret
.Lsub_notii:
    cmp     dil, DT_R
    jne     .Lsub_slow
    cmp     dl, DT_R
    jne     .Lsub_slow
    movq    xmm0, rsi
    movq    xmm1, rcx
    subsd   xmm0, xmm1
    movq    rdx, xmm0
    mov     eax, DT_R | (MOD_OP_RT_SUB << 8)   /* stamped (row descr-stamp-asm-mints) */
    ret
.Lsub_slow:
    jmp     c_rt_sub
RTX_ENDF(rt_sub)

/*---------------------------------------------------------------------------*/
RTX_FUNC(rt_mul)
    RTX_GATE(arith, c_rt_mul)
    cmp     dil, DT_I
    jne     .Lmul_notii
    cmp     dl, DT_I
    jne     .Lmul_notii
    mov     rdx, rsi
    imul    rdx, rcx                    /* a.i * b.i, low 64 bits, wraps       */
    mov     eax, DT_I | (MOD_OP_RT_MUL << 8)   /* stamped (row descr-stamp-asm-mints), zero extra cost: same imm width */
    ret
.Lmul_notii:
    cmp     dil, DT_R
    jne     .Lmul_slow
    cmp     dl, DT_R
    jne     .Lmul_slow
    movq    xmm0, rsi
    movq    xmm1, rcx
    mulsd   xmm0, xmm1
    movq    rdx, xmm0
    mov     eax, DT_R | (MOD_OP_RT_MUL << 8)   /* stamped (row descr-stamp-asm-mints) */
    ret
.Lmul_slow:
    jmp     c_rt_mul
RTX_ENDF(rt_mul)

/* Non-executable stack marker: without it ld marks the whole .so RWX-stack. */
.section .note.GNU-stack,"",@progbits
