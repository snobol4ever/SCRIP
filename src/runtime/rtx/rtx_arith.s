#include "rtx_abi.inc"
RTX_GATE_DEF(arith)
.section .rodata
.align 1
.Lcd_empty:
    .byte 0
.text
RTX_FUNC(rt_cmp_d)
    mov     eax, dword ptr [rdi]
    mov     ecx, dword ptr [rsi]
    cmp     al, DT_I
    jne     .Lcd_notint
    cmp     cl, DT_I
    jne     .Lcd_notint
    mov     rdx, qword ptr [rdi + 8]
    cmp     rdx, qword ptr [rsi + 8]
    setg    r11b
    setl    r10b
    sub     r11b, r10b
    movsx   eax, r11b
    ret
.Lcd_notint:
    mov     r10d, eax
    or      r10d, ecx
    test    r10b, (DT_NOTSTR_MASK & 0xFF)
    jnz     .Lcd_real
    xor     edx, edx
    xor     r11d, r11d
    cmp     al, DT_S
    jne     .Lcd_a_fix
    mov     rdx, qword ptr [rdi + 8]
    mov     r11d, dword ptr [rdi + 4]
.Lcd_a_fix:
    test    rdx, rdx
    jnz     .Lcd_a_len
    lea     rdx, [rip + .Lcd_empty]
    xor     r11d, r11d
.Lcd_a_len:
    test    r11d, r11d
    jz      .Lcd_a_scan
    cmp     r11d, -1
    jne     .Lcd_a_known
    xor     r11d, r11d
.Lcd_a_scan:
    cmp     byte ptr [rdx + r11], 0
    je      .Lcd_a_known
    inc     r11
    jmp     .Lcd_a_scan
.Lcd_a_known:
    mov     rdi, r11
    xor     r10d, r10d
    xor     r11d, r11d
    cmp     cl, DT_S
    jne     .Lcd_b_fix
    mov     r10, qword ptr [rsi + 8]
    mov     r11d, dword ptr [rsi + 4]
.Lcd_b_fix:
    test    r10, r10
    jnz     .Lcd_b_len
    lea     r10, [rip + .Lcd_empty]
    xor     r11d, r11d
.Lcd_b_len:
    test    r11d, r11d
    jz      .Lcd_b_scan
    cmp     r11d, -1
    jne     .Lcd_b_known
    xor     r11d, r11d
.Lcd_b_scan:
    cmp     byte ptr [r10 + r11], 0
    je      .Lcd_b_known
    inc     r11
    jmp     .Lcd_b_scan
.Lcd_b_known:
    mov     rsi, r11
    cmp     r11, rdi
    cmova   r11, rdi
.Lcd_strloop:
    test    r11, r11
    jz      .Lcd_strtie
    movzx   eax, byte ptr [rdx]
    movzx   ecx, byte ptr [r10]
    cmp     al, cl
    jne     .Lcd_strdiff
    inc     rdx
    inc     r10
    dec     r11
    jmp     .Lcd_strloop
.Lcd_strtie:
    cmp     rdi, rsi
.Lcd_strdiff:
    seta    r11b
    setb    r10b
    sub     r11b, r10b
    movsx   eax, r11b
    ret
.Lcd_real:
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
    seta    r11b
    comisd  xmm1, xmm0
    seta    r10b
    sub     r11b, r10b
    movsx   eax, r11b
    ret
RTX_ENDF(rt_cmp_d)
RTX_FUNC(rt_add)
    RTX_GATE(arith, c_rt_add)
    cmp     dil, DT_I
    jne     .Ladd_notii
    cmp     dl, DT_I
    jne     .Ladd_notii
    mov     rdx, rsi
    add     rdx, rcx
    jo      .Ladd_slow
    mov     eax, DT_I | (MOD_OP_RT_ADD << 8)
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
    mov     eax, DT_R | (MOD_OP_RT_ADD << 8)
    ret
.Ladd_slow:
    jmp     c_rt_add
RTX_ENDF(rt_add)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RTX_FUNC(rt_sub)
    RTX_GATE(arith, c_rt_sub)
    cmp     dil, DT_I
    jne     .Lsub_notii
    cmp     dl, DT_I
    jne     .Lsub_notii
    mov     rdx, rsi
    sub     rdx, rcx
    jo      .Lsub_slow
    mov     eax, DT_I | (MOD_OP_RT_SUB << 8)
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
    mov     eax, DT_R | (MOD_OP_RT_SUB << 8)
    ret
.Lsub_slow:
    jmp     c_rt_sub
RTX_ENDF(rt_sub)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RTX_FUNC(rt_mul)
    RTX_GATE(arith, c_rt_mul)
    cmp     dil, DT_I
    jne     .Lmul_notii
    cmp     dl, DT_I
    jne     .Lmul_notii
    mov     rdx, rsi
    imul    rdx, rcx
    jo      .Lmul_slow
    mov     eax, DT_I | (MOD_OP_RT_MUL << 8)
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
    mov     eax, DT_R | (MOD_OP_RT_MUL << 8)
    ret
.Lmul_slow:
    jmp     c_rt_mul
RTX_ENDF(rt_mul)
.section .note.GNU-stack,"",@progbits
