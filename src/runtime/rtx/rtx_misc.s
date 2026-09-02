#include "rtx_abi.inc"
RTX_GATE_DEF(misc)
RTX_FUNC(rt_faildescr)
    mov     eax, DT_FAIL | (MOD_OP_RT_FAILDESCR << 8)
    xor     edx, edx
    ret
RTX_ENDF(rt_faildescr)
RTX_FUNC(rt_is_truthy)
    xor     eax, eax
    cmp     dil, DT_FAIL
    je      .Lit_ret
    cmp     dil, DT_I
    je      .Lit_int
    cmp     dil, DT_R
    je      .Lit_real
    test    edi, edi
    je      .Lit_ret
    test    rsi, rsi
    je      .Lit_ret
    movzx   ecx, byte ptr [rsi]
    test    cl, cl
    je      .Lit_ret
    cmp     cl, 0x30
    jne     .Lit_true
    cmp     byte ptr [rsi + 1], 0
    je      .Lit_ret
.Lit_true:
    mov     eax, 1
.Lit_ret:
    ret
.Lit_int:
    test    rsi, rsi
    setne   al
    ret
.Lit_real:
    movq    xmm0, rsi
    pxor    xmm1, xmm1
    ucomisd xmm0, xmm1
    setne   al
    setp    cl
    or      al, cl
    movzx   eax, al
    ret
RTX_ENDF(rt_is_truthy)
.section .note.GNU-stack,"",@progbits
