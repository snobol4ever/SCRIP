/* rtx_misc.s — RTX family MISC. First asm ports (RTX-1 scaffold, s162).
 *
 * READ .github/ARCH-SNOBOL4-RTX.md BEFORE EDITING. Contract macros: rtx_abi.inc.
 *
 * These two functions are deliberately small: together they prove BOTH directions
 * of the descriptor ABI (a DESCR_t argument pair in, a DESCR_t return pair out)
 * plus the gate, the build wiring, and the m3/m4 single-source linkage. They are
 * the plumbing proof, not a speed rung; do not quote a rail number off them.
 */
#include "rtx_abi.inc"

RTX_GATE_DEF(misc)

/*-----------------------------------------------------------------------------
 * DESCR_t rt_faildescr(void)              C of record: src/runtime/rt/rt.c:1469
 *   return FAILDESCR = { .v = DT_FAIL, .i = 0 }; slen implicitly 0.
 *   Return pair: rax = (slen << 32) | v = 99, rdx = value = 0.
 */
RTX_FUNC(rt_faildescr)
    mov     eax, DT_FAIL | (MOD_OP_RT_FAILDESCR << 8)   /* stamped (row descr-stamp-asm-mints): mod_op names this mint, src_node stays 0 (no caller node here) */
    xor     edx, edx                    /* value = 0             */
    ret
RTX_ENDF(rt_faildescr)

/*-----------------------------------------------------------------------------
 * int rt_is_truthy(DESCR_t v)   C of record: src/runtime/by_name_dispatch.c:254
 *
 *   if (IS_FAIL_fn(v))  return 0;              v.v == DT_FAIL
 *   if (IS_INT_fn(v))   return v.i != 0;       v.v == DT_I
 *   if (IS_REAL_fn(v))  return v.r != 0.0;     v.v == DT_R
 *   if (v.v == DT_SNUL) return 0;
 *   const char *s = v.s ? v.s : "";
 *   return s[0] != '\0' && !(s[0] == '0' && s[1] == '\0');
 *
 *   Arg pair: edi = v.v (tag), rsi = value. Result in eax.
 *   Every tag that is not FAIL/I/R/SNUL falls into the string path and
 *   dereferences the value as char* — that is the EXISTING C semantics for
 *   DT_P/DT_A/DT_T/... and this port reproduces it exactly rather than
 *   improving it. Changing it is a separate, gated, measured decision.
 *
 *   TWO IEEE TRAPS the C hides and asm must not (this is why the real
 *   comparison is used instead of a bit test against zero):
 *     -0.0  bit pattern 0x8000000000000000 is NONZERO but -0.0 != 0.0 is FALSE
 *           in C, so -0.0 is FALSY. A test rsi,rsi would wrongly call it true.
 *     NaN   NaN != 0.0 is TRUE in C, so NaN is TRUTHY. ucomisd reports NaN as
 *           unordered (PF=1, ZF=1), so setne alone would wrongly call it false.
 *   Hence: setne (ordered-unequal) OR setp (unordered) — covers both.
 */
RTX_FUNC(rt_is_truthy)
    xor     eax, eax                    /* default result 0                    */
    cmp     dil, DT_FAIL
    je      .Lit_ret                    /* FAIL   -> 0                         */
    cmp     dil, DT_I
    je      .Lit_int
    cmp     dil, DT_R
    je      .Lit_real
    test    edi, edi                    /* DT_SNUL == 0                        */
    je      .Lit_ret                    /* SNUL   -> 0                         */
    test    rsi, rsi
    je      .Lit_ret                    /* NULL char* -> "" -> 0               */
    movzx   ecx, byte ptr [rsi]         /* s[0]                                */
    test    cl, cl
    je      .Lit_ret                    /* ""     -> 0                         */
    cmp     cl, 0x30                    /* '0'                                 */
    jne     .Lit_true
    cmp     byte ptr [rsi + 1], 0
    je      .Lit_ret                    /* exactly "0" -> 0                    */
.Lit_true:
    mov     eax, 1
.Lit_ret:
    ret
.Lit_int:
    test    rsi, rsi
    setne   al                          /* eax upper bits already zeroed       */
    ret
.Lit_real:
    movq    xmm0, rsi
    pxor    xmm1, xmm1
    ucomisd xmm0, xmm1
    setne   al                          /* ordered and unequal                 */
    setp    cl                          /* unordered (NaN) counts as unequal   */
    or      al, cl
    movzx   eax, al
    ret
RTX_ENDF(rt_is_truthy)

/* Non-executable stack marker: without it ld marks the whole .so RWX-stack. */
.section .note.GNU-stack,"",@progbits
