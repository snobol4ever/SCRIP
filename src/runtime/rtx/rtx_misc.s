#include "rtx_abi.inc"
RTX_GATE_DEF(misc)
RTX_FUNC(rt_kw_set_rtntype_role)
    cmp     edi, 2
    je      .Lkr_f
    mov     rax, qword ptr [rip + rt_g_ret_by_name@GOTPCREL]
    cmp     dword ptr [rax], 0
    jne     .Lkr_n
    mov     rax, 0x00004E5255544552
    jmp     .Lkr_store
.Lkr_f:
    mov     rax, 0x004E525554455246
    jmp     .Lkr_store
.Lkr_n:
    mov     rax, 0x004E52555445524E
.Lkr_store:
    mov     rdx, qword ptr [rip + kw_rtntype@GOTPCREL]
    mov     qword ptr [rdx], rax
    ret
RTX_ENDF(rt_kw_set_rtntype_role)
RTX_FUNC(descr_identical)
    RTX_GATE(misc, .Ldi_c)
    cmp     dil, DT_FAIL
    je      .Ldi_zero
    cmp     dl, DT_FAIL
    je      .Ldi_zero
    RTX_SAVE
    xor     r8d, r8d
    cmp     dil, DT_SNUL
    je      .Ldi_an1
    cmp     dil, DT_S
    jne     .Ldi_an_done
    test    rsi, rsi
    jz      .Ldi_an1
    mov     rax, rdi
    shr     rax, 32
    cmp     eax, -1
    jne     .Ldi_an_len
    cmp     byte ptr [rsi], 0
    jne     .Ldi_an_done
    jmp     .Ldi_an1
.Ldi_an_len:
    test    eax, eax
    jnz     .Ldi_an_done
.Ldi_an1:
    mov     r8d, 1
.Ldi_an_done:
    xor     r9d, r9d
    cmp     dl, DT_SNUL
    je      .Ldi_bn1
    cmp     dl, DT_S
    jne     .Ldi_bn_done
    test    rcx, rcx
    jz      .Ldi_bn1
    mov     rax, rdx
    shr     rax, 32
    cmp     eax, -1
    jne     .Ldi_bn_len
    cmp     byte ptr [rcx], 0
    jne     .Ldi_bn_done
    jmp     .Ldi_bn1
.Ldi_bn_len:
    test    eax, eax
    jnz     .Ldi_bn_done
.Ldi_bn1:
    mov     r9d, 1
.Ldi_bn_done:
    mov     eax, r8d
    and     eax, r9d
    jnz     .Ldi_one
    cmp     r8d, r9d
    jne     .Ldi_zero_r
    cmp     dil, DT_I
    jne     .Ldi_str
    cmp     dl, DT_I
    jne     .Ldi_cs
    xor     eax, eax
    cmp     rsi, rcx
    sete    al
    RTX_RET
.Ldi_str:
    cmp     dil, DT_S
    jne     .Ldi_cs
    cmp     dl, DT_S
    jne     .Ldi_cs
    mov     rax, rdi
    shr     rax, 32
    cmp     eax, -1
    je      .Ldi_cs
    mov     r10, rdx
    shr     r10, 32
    cmp     r10d, -1
    je      .Ldi_cs
    cmp     eax, r10d
    jne     .Ldi_zero_r
    xor     r10d, r10d
.Ldi_cmp:
    cmp     r10d, eax
    jae     .Ldi_one
    movzx   r11d, byte ptr [rsi + r10]
    cmp     r11b, byte ptr [rcx + r10]
    jne     .Ldi_zero_r
    inc     r10d
    jmp     .Ldi_cmp
.Ldi_one:
    mov     eax, 1
    RTX_RET
.Ldi_zero_r:
    xor     eax, eax
    RTX_RET
.Ldi_cs:
    RTX_CTAIL_SAVED(c_descr_identical)
.Ldi_zero:
    xor     eax, eax
    ret
.Ldi_c:
    RTX_CTAIL(c_descr_identical)
RTX_ENDF(descr_identical)
RTX_FUNC(rt_faildescr)
    mov     eax, DT_FAIL
    xor     edx, edx
    ret
RTX_ENDF(rt_faildescr)
RTX_FUNC(rt_is_truthy)
    xor     eax, eax
    cmp     dil, DT_FAIL
    je      .Lit_ret
    cmp     dil, DT_I
    je      .Lit_int
    cmp     dil, DT_BOOL
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
