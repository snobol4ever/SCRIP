#include "rtx_abi.inc"
RTX_GATE_DEF(arith)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define RTX_REAL_FINITE_OR(slow) \
    movq    rax, xmm0 ; \
    add     rax, rax ; \
    shr     rax, 53 ; \
    cmp     eax, 0x7FF ; \
    jae     slow
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
RTX_FUNC(rt_add)
    RTX_GATE(arith, .Ladd_slow)
    cmp     dil, DT_I
    jne     .Ladd_notii
    cmp     dl, DT_I
    jne     .Ladd_notii
    mov     rdx, rsi
    add     rdx, rcx
    jo      .Ladd_slow
    mov     eax, DT_I
    ret
.Ladd_notii:
    cmp     dil, DT_R
    jne     .Ladd_slow
    cmp     dl, DT_R
    jne     .Ladd_slow
    movq    xmm0, rsi
    movq    xmm1, rcx
    addsd   xmm0, xmm1
    RTX_REAL_FINITE_OR(.Ladd_slow)
    movq    rdx, xmm0
    mov     eax, DT_R
    ret
.Ladd_slow:
    RTX_CTAIL(c_rt_add)
RTX_ENDF(rt_add)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RTX_FUNC(rt_sub)
    RTX_GATE(arith, .Lsub_slow)
    cmp     dil, DT_I
    jne     .Lsub_notii
    cmp     dl, DT_I
    jne     .Lsub_notii
    mov     rdx, rsi
    sub     rdx, rcx
    jo      .Lsub_slow
    mov     eax, DT_I
    ret
.Lsub_notii:
    cmp     dil, DT_R
    jne     .Lsub_slow
    cmp     dl, DT_R
    jne     .Lsub_slow
    movq    xmm0, rsi
    movq    xmm1, rcx
    subsd   xmm0, xmm1
    RTX_REAL_FINITE_OR(.Lsub_slow)
    movq    rdx, xmm0
    mov     eax, DT_R
    ret
.Lsub_slow:
    RTX_CTAIL(c_rt_sub)
RTX_ENDF(rt_sub)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RTX_FUNC(rt_mul)
    RTX_GATE(arith, .Lmul_slow)
    cmp     dil, DT_I
    jne     .Lmul_notii
    cmp     dl, DT_I
    jne     .Lmul_notii
    mov     rdx, rsi
    imul    rdx, rcx
    jo      .Lmul_slow
    mov     eax, DT_I
    ret
.Lmul_notii:
    cmp     dil, DT_R
    jne     .Lmul_slow
    cmp     dl, DT_R
    jne     .Lmul_slow
    movq    xmm0, rsi
    movq    xmm1, rcx
    mulsd   xmm0, xmm1
    RTX_REAL_FINITE_OR(.Lmul_slow)
    movq    rdx, xmm0
    mov     eax, DT_R
    ret
.Lmul_slow:
    RTX_CTAIL(c_rt_mul)
RTX_ENDF(rt_mul)
.section .note.GNU-stack,"",@progbits
