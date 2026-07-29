/* rtx_arith.S — RTX family ARITH (RTX-6, s203).
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
    RTX_GATE(arith, c_rt_cmp_d)
    mov     eax, dword ptr [rdi]        /* a->v                                */
    mov     ecx, dword ptr [rsi]        /* b->v                                */
    /* ---- HOT ARM: both DT_I. Measured shape of LT/GT/LE/GE on counters. ---- */
    cmp     eax, DT_I
    jne     .Lcd_notint
    cmp     ecx, DT_I
    jne     .Lcd_notint
    mov     rdx, qword ptr [rdi + 8]    /* a->i                                */
    cmp     rdx, qword ptr [rsi + 8]    /* signed compare against b->i         */
    setg    r9b
    setl    r10b
    sub     r9b, r10b                   /* 1 / -1 / 0, branchless              */
    movsx   eax, r9b
    ret
.Lcd_notint:
    /* ---- STRING ARM: both tags in {DT_SNUL=0, DT_S=1}. DT_SNUL is 0 and
     * DT_S is 1, so "tag == DT_S || tag == DT_SNUL" is exactly the UNSIGNED
     * test tag <= 1 -- one compare per operand instead of two. Every DTYPE_t
     * enumerator is non-negative, so the unsigned form is safe.            ---- */
    cmp     eax, DT_S
    ja      .Lcd_real
    cmp     ecx, DT_S
    ja      .Lcd_real
    /* ptr_a = (a->v == DT_S && a->s) ? a->s : ""   -- DT_SNUL always yields ""
     * even when the union happens to hold a non-null pointer.                 */
    xor     edx, edx
    cmp     eax, DT_S
    jne     .Lcd_a_fix
    mov     rdx, qword ptr [rdi + 8]
.Lcd_a_fix:
    test    rdx, rdx
    jnz     .Lcd_b
    lea     rdx, [rip + .Lcd_empty]
.Lcd_b:
    xor     r8d, r8d
    cmp     ecx, DT_S
    jne     .Lcd_b_fix
    mov     r8, qword ptr [rsi + 8]
.Lcd_b_fix:
    test    r8, r8
    jnz     .Lcd_strloop
    lea     r8, [rip + .Lcd_empty]
    /* Inline unsigned byte compare rather than a libc strcmp call. This arm is
     * COLD in every measured benchmark (the hot shape is DT_I/DT_I), so the
     * libc-boundary realignment ceremony would buy nothing and cost a call.
     * strcmp compares as UNSIGNED char, hence movzx and the seta/setb pair.  */
.Lcd_strloop:
    movzx   eax, byte ptr [rdx]
    movzx   ecx, byte ptr [r8]
    cmp     al, cl
    jne     .Lcd_strdiff
    test    al, al
    je      .Lcd_streq
    inc     rdx
    inc     r8
    jmp     .Lcd_strloop
.Lcd_strdiff:
    seta    r9b                         /* flags still those of cmp al, cl     */
    setb    r10b
    sub     r9b, r10b
    movsx   eax, r9b
    ret
.Lcd_streq:
    xor     eax, eax
    ret
.Lcd_real:
    /* x = (a->v == DT_R) ? a->r : (double)a->i, and likewise y. The cvtsi2sd
     * arm reinterprets the union's 64 bits as a signed integer for EVERY tag
     * that is not DT_R -- including pointer-bearing tags. That is the existing
     * C semantics and this port reproduces it rather than improving it.      */
    cmp     eax, DT_R
    je      .Lcd_a_real
    cvtsi2sd xmm0, qword ptr [rdi + 8]
    jmp     .Lcd_bval
.Lcd_a_real:
    movsd   xmm0, qword ptr [rdi + 8]
.Lcd_bval:
    cmp     ecx, DT_R
    je      .Lcd_b_real
    cvtsi2sd xmm1, qword ptr [rsi + 8]
    jmp     .Lcd_cmp
.Lcd_b_real:
    movsd   xmm1, qword ptr [rsi + 8]
.Lcd_cmp:
    comisd  xmm0, xmm1
    seta    r9b                         /* x > y  (false when unordered)       */
    comisd  xmm1, xmm0
    seta    r10b                        /* y > x, i.e. x < y (false unordered) */
    sub     r9b, r10b                   /* NaN => 0 - 0 = 0, matching the C    */
    movsx   eax, r9b
    ret
RTX_ENDF(rt_cmp_d)

/* Non-executable stack marker: without it ld marks the whole .so RWX-stack. */
.section .note.GNU-stack,"",@progbits
