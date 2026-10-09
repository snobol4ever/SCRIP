#include "rtx_abi.inc"
RTX_GATE_DEF(arith)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define RTX_REAL_FINITE_OR(slow) \
    movq    rax, xmm0 ; \
    add     rax, rax ; \
    shr     rax, 53 ; \
    cmp     eax, 0x7FF ; \
    jae     slow
#define RTX_ARITH_INTSTR(p, opi) \
.L##p##_try: \
    movq xmm2, rdi; movq xmm3, rsi; movq xmm4, rdx; movq xmm5, rcx; \
    RTX_SUB_RSP(8); \
    call qword ptr [rip + rtx_int_str_operand@GOTPCREL]; test ecx, ecx; jz .L##p##_fail; \
    movq xmm6, rax; movq rdi, xmm4; movq rsi, xmm5; \
    call qword ptr [rip + rtx_int_str_operand@GOTPCREL]; test ecx, ecx; jz .L##p##_fail; \
    RTX_ADD_RSP(8); \
    mov rcx, rax; movq rax, xmm6; opi rax, rcx; jo .L##p##_rst; mov rdx, rax; mov eax, DT_I; ret; \
.L##p##_fail: \
    .cfi_adjust_cfa_offset 8; RTX_ADD_RSP(8); \
.L##p##_rst: \
    movq rdi, xmm2; movq rsi, xmm3; movq rdx, xmm4; movq rcx, xmm5;
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
    setg    al
    setl    cl
    sub     al, cl
    movsx   eax, al
    ret
.Lcd_notint:
    test    al, (DT_NOTSTR_MASK & 0xFF)
    jnz     .Lcd_real
    test    cl, (DT_NOTSTR_MASK & 0xFF)
    jnz     .Lcd_real
    xor     edx, edx
    cmp     al, DT_S
    jne     .Lcd_a_nots
    mov     rdx, qword ptr [rdi + 8]
    mov     edi, dword ptr [rdi + 4]
    jmp     .Lcd_a_fix
.Lcd_a_nots:
    xor     edi, edi
.Lcd_a_fix:
    test    rdx, rdx
    jnz     .Lcd_a_len
    lea     rdx, [rip + .Lcd_empty]
    xor     edi, edi
.Lcd_a_len:
    test    edi, edi
    jz      .Lcd_a_scan
    cmp     edi, -1
    jne     .Lcd_a_known
    xor     edi, edi
.Lcd_a_scan:
    cmp     byte ptr [rdx + rdi], 0
    je      .Lcd_a_known
    inc     rdi
    jmp     .Lcd_a_scan
.Lcd_a_known:
    xor     eax, eax
    cmp     cl, DT_S
    jne     .Lcd_b_nots
    mov     rax, qword ptr [rsi + 8]
    mov     esi, dword ptr [rsi + 4]
    jmp     .Lcd_b_fix
.Lcd_b_nots:
    xor     esi, esi
.Lcd_b_fix:
    test    rax, rax
    jnz     .Lcd_b_len
    lea     rax, [rip + .Lcd_empty]
    xor     esi, esi
.Lcd_b_len:
    test    esi, esi
    jz      .Lcd_b_scan
    cmp     esi, -1
    jne     .Lcd_b_known
    xor     esi, esi
.Lcd_b_scan:
    cmp     byte ptr [rax + rsi], 0
    je      .Lcd_b_known
    inc     rsi
    jmp     .Lcd_b_scan
.Lcd_b_known:
    mov     rcx, rsi
    cmp     rcx, rdi
    cmova   rcx, rdi
    cmp     rdi, rsi
    seta    dil
    setb    sil
    sub     dil, sil
    movsx   edi, dil
    mov     rsi, rax
.Lcd_strloop:
    test    rcx, rcx
    jz      .Lcd_strtie
    movzx   eax, byte ptr [rdx]
    cmp     al, byte ptr [rsi]
    jne     .Lcd_strdiff
    inc     rdx
    inc     rsi
    dec     rcx
    jmp     .Lcd_strloop
.Lcd_strtie:
    mov     eax, edi
    ret
.Lcd_strdiff:
    seta    al
    setb    cl
    sub     al, cl
    movsx   eax, al
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
    seta    al
    comisd  xmm1, xmm0
    seta    cl
    sub     al, cl
    movsx   eax, al
    ret
RTX_ENDF(rt_cmp_d)
RTX_FUNC(rtx_int_str_operand)
    cmp     dil, DT_I
    jne     .Lis_ni
    mov     rax, rsi
    mov     ecx, 1
    ret
.Lis_ni:
    test    dil, dil
    jz      .Lis_zero
    cmp     dil, DT_S
    jne     .Lis_fail
    test    rsi, rsi
    jz      .Lis_zero
    shr     rdi, 32
    cmp     edi, -1
    je      .Lis_fail
    add     rdi, rsi
.Lis_ws1:
    cmp     rsi, rdi
    je      .Lis_zero
    movzx   ecx, byte ptr [rsi]
    cmp     cl, 0x20
    je      .Lis_ws1n
    cmp     cl, 0x09
    jne     .Lis_sign
.Lis_ws1n:
    inc     rsi
    jmp     .Lis_ws1
.Lis_sign:
    xor     edx, edx
    cmp     cl, 0x2B
    je      .Lis_sg
    cmp     cl, 0x2D
    jne     .Lis_dig0
    inc     edx
.Lis_sg:
    inc     rsi
    cmp     rsi, rdi
    je      .Lis_fail
    movzx   ecx, byte ptr [rsi]
.Lis_dig0:
    sub     ecx, 0x30
    cmp     ecx, 9
    ja      .Lis_fail
    xor     eax, eax
.Lis_dig:
    imul    rax, rax, 10
    jo      .Lis_fail
    sub     rax, rcx
    jo      .Lis_fail
    inc     rsi
    cmp     rsi, rdi
    je      .Lis_end
    movzx   ecx, byte ptr [rsi]
    sub     ecx, 0x30
    cmp     ecx, 9
    jbe     .Lis_dig
    add     ecx, 0x30
.Lis_ws2:
    cmp     cl, 0x20
    je      .Lis_ws2n
    cmp     cl, 0x09
    jne     .Lis_fail
.Lis_ws2n:
    inc     rsi
    cmp     rsi, rdi
    je      .Lis_end
    movzx   ecx, byte ptr [rsi]
    jmp     .Lis_ws2
.Lis_end:
    test    edx, edx
    jnz     .Lis_ok
    neg     rax
    jo      .Lis_fail
.Lis_ok:
    mov     ecx, 1
    ret
.Lis_zero:
    xor     eax, eax
    mov     ecx, 1
    ret
.Lis_fail:
    xor     ecx, ecx
    ret
RTX_ENDF(rtx_int_str_operand)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RTX_FUNC(rt_add)
    RTX_GATE(arith, .Ladd_slow)
    cmp     dil, DT_I
    jne     .Ladd_notii
    cmp     dl, DT_I
    jne     .Ladd_notii
    mov     rax, rsi
    add     rax, rcx
    jo      .Ladd_slow
    mov     rdx, rax
    mov     eax, DT_I
    ret
.Ladd_notii:
    cmp     dil, DT_R
    jne     .Ladd_try
    cmp     dl, DT_R
    jne     .Ladd_try
    movq    xmm0, rsi
    movq    xmm1, rcx
    addsd   xmm0, xmm1
    RTX_REAL_FINITE_OR(.Ladd_slow)
    movq    rdx, xmm0
    mov     eax, DT_R
    ret
    RTX_ARITH_INTSTR(add, add)
.Ladd_slow:
    RTX_DECLINE
RTX_ENDF(rt_add)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RTX_FUNC(rt_sub)
    RTX_GATE(arith, .Lsub_slow)
    cmp     dil, DT_I
    jne     .Lsub_notii
    cmp     dl, DT_I
    jne     .Lsub_notii
    mov     rax, rsi
    sub     rax, rcx
    jo      .Lsub_slow
    mov     rdx, rax
    mov     eax, DT_I
    ret
.Lsub_notii:
    cmp     dil, DT_R
    jne     .Lsub_try
    cmp     dl, DT_R
    jne     .Lsub_try
    movq    xmm0, rsi
    movq    xmm1, rcx
    subsd   xmm0, xmm1
    RTX_REAL_FINITE_OR(.Lsub_slow)
    movq    rdx, xmm0
    mov     eax, DT_R
    ret
    RTX_ARITH_INTSTR(sub, sub)
.Lsub_slow:
    RTX_DECLINE
RTX_ENDF(rt_sub)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RTX_FUNC(rt_mul)
    RTX_GATE(arith, .Lmul_slow)
    cmp     dil, DT_I
    jne     .Lmul_notii
    cmp     dl, DT_I
    jne     .Lmul_notii
    mov     rax, rsi
    imul    rax, rcx
    jo      .Lmul_slow
    mov     rdx, rax
    mov     eax, DT_I
    ret
.Lmul_notii:
    cmp     dil, DT_R
    jne     .Lmul_try
    cmp     dl, DT_R
    jne     .Lmul_try
    movq    xmm0, rsi
    movq    xmm1, rcx
    mulsd   xmm0, xmm1
    RTX_REAL_FINITE_OR(.Lmul_slow)
    movq    rdx, xmm0
    mov     eax, DT_R
    ret
    RTX_ARITH_INTSTR(mul, imul)
.Lmul_slow:
    RTX_DECLINE
RTX_ENDF(rt_mul)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RTX_FUNC(rt_add_sno)
    RTX_GATE(arith, .Laddsno_slow)
    cmp     dil, DT_I
    jne     .Laddsno_notii
    cmp     dl, DT_I
    jne     .Laddsno_notii
    mov     rax, rsi
    add     rax, rcx
    jo      .Laddsno_slow
    mov     rdx, rax
    mov     eax, DT_I
    ret
.Laddsno_notii:
    cmp     dil, DT_R
    jne     .Laddsno_try
    cmp     dl, DT_R
    jne     .Laddsno_try
    movq    xmm0, rsi
    movq    xmm1, rcx
    addsd   xmm0, xmm1
    RTX_REAL_FINITE_OR(.Laddsno_slow)
    movq    rdx, xmm0
    mov     eax, DT_R
    ret
    RTX_ARITH_INTSTR(addsno, add)
.Laddsno_slow:
    RTX_DECLINE
RTX_ENDF(rt_add_sno)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RTX_FUNC(rt_sub_sno)
    RTX_GATE(arith, .Lsubsno_slow)
    cmp     dil, DT_I
    jne     .Lsubsno_notii
    cmp     dl, DT_I
    jne     .Lsubsno_notii
    mov     rax, rsi
    sub     rax, rcx
    jo      .Lsubsno_slow
    mov     rdx, rax
    mov     eax, DT_I
    ret
.Lsubsno_notii:
    cmp     dil, DT_R
    jne     .Lsubsno_try
    cmp     dl, DT_R
    jne     .Lsubsno_try
    movq    xmm0, rsi
    movq    xmm1, rcx
    subsd   xmm0, xmm1
    RTX_REAL_FINITE_OR(.Lsubsno_slow)
    movq    rdx, xmm0
    mov     eax, DT_R
    ret
    RTX_ARITH_INTSTR(subsno, sub)
.Lsubsno_slow:
    RTX_DECLINE
RTX_ENDF(rt_sub_sno)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RTX_FUNC(rt_mul_sno)
    RTX_GATE(arith, .Lmulsno_slow)
    cmp     dil, DT_I
    jne     .Lmulsno_notii
    cmp     dl, DT_I
    jne     .Lmulsno_notii
    mov     rax, rsi
    imul    rax, rcx
    jo      .Lmulsno_slow
    mov     rdx, rax
    mov     eax, DT_I
    ret
.Lmulsno_notii:
    cmp     dil, DT_R
    jne     .Lmulsno_try
    cmp     dl, DT_R
    jne     .Lmulsno_try
    movq    xmm0, rsi
    movq    xmm1, rcx
    mulsd   xmm0, xmm1
    RTX_REAL_FINITE_OR(.Lmulsno_slow)
    movq    rdx, xmm0
    mov     eax, DT_R
    ret
    RTX_ARITH_INTSTR(mulsno, imul)
.Lmulsno_slow:
    RTX_DECLINE
RTX_ENDF(rt_mul_sno)
.section .note.GNU-stack,"",@progbits
