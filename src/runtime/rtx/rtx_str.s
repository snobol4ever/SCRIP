#include "rtx_abi.inc"
RTX_GATE_DEF(str)
#define HBF_TTL 0x0001
.macro RTX_MEMCPY
    cmp     rcx, 16
    jb      201f
    cmp     rcx, 32
    ja      200f
    movdqu  xmm0, [rsi]
    movdqu  xmm1, [rsi + rcx - 16]
    movdqu  [rdi], xmm0
    movdqu  [rdi + rcx - 16], xmm1
    jmp     209f
200:
    rep movsb
    jmp     209f
201:
    cmp     rcx, 8
    jb      202f
    mov     rax, [rsi]
    mov     r11, [rsi + rcx - 8]
    mov     [rdi], rax
    mov     [rdi + rcx - 8], r11
    jmp     209f
202:
    cmp     rcx, 4
    jb      203f
    mov     eax, [rsi]
    mov     r11d, [rsi + rcx - 4]
    mov     [rdi], eax
    mov     [rdi + rcx - 4], r11d
    jmp     209f
203:
    mov     al, [rsi]
    mov     r11b, [rsi + rcx - 1]
    mov     [rdi], al
    mov     [rdi + rcx - 1], r11b
    cmp     rcx, 3
    jb      209f
    mov     al, [rsi + 1]
    mov     [rdi + 1], al
209:
.endm
RTX_FUNC(str_concat_d)
    RTX_GATE(str, c_str_concat_d)
    cmp     dil, DT_S
    jne     .Lsc_null
    cmp     dl, DT_S
    jne     .Lsc_null
    test    rsi, rsi
    je      .Lsc_slow
    test    rcx, rcx
    je      .Lsc_slow
    mov     r8, rdi
    shr     r8, 32
    je      .Lsc_slow
    cmp     r8d, -1
    je      .Lsc_slow
    mov     r9, rdx
    shr     r9, 32
    je      .Lsc_slow
    cmp     r9d, -1
    je      .Lsc_slow
    mov     rax, [rip + g_gc_pending@GOTPCREL]
    cmp     dword ptr [rax], 0
    jne     .Lsc_slow
    lea     rax, [rip + g_sxt_fr]
    mov     r10d, dword ptr [rax + 20]
    test    r10d, r10d
    js      .Lsc_slow
    jnz     .Lsc_nosxt
    cmp     rsi, [rax + 0]
    je      .Lsc_slow
.Lsc_nosxt:
    push    rsi
    push    rcx
    push    r8
    push    r9
    sub     rsp, 8
    lea     rdi, [r8 + r9]
    call    rt_str_alloc
    add     rsp, 8
    pop     r9
    pop     r8
    pop     rcx
    pop     rsi
    mov     r10, rax
    mov     rdx, rcx
    mov     rdi, rax
    mov     rcx, r8
    RTX_MEMCPY
    lea     rdi, [r10 + r8]
    mov     rsi, rdx
    mov     rcx, r9
    RTX_MEMCPY
    lea     r11, [r8 + r9]
    mov     byte ptr [r10 + r11], 0
    lea     rax, [rip + g_sxt_fr]
    mov     qword ptr [rax + 0], 0
    mov     rdx, [rip + g_hp_fr@GOTPCREL]
    mov     rdx, [rdx + 0]
    lea     rsi, [r10 - 16]
    test    word ptr [rsi + 14], HBF_TTL
    je      .Lsc_ret
    cmp     word ptr [rsi + 12], DT_S
    jne     .Lsc_ret
    mov     ecx, dword ptr [rsi + 8]
    add     rcx, rsi
    cmp     rcx, rdx
    jne     .Lsc_ret
    mov     [rax + 0], r10
    mov     [rax + 8], r11
.Lsc_ret:
    mov     rax, r11
    shl     rax, 32
    or      rax, DT_S | (MOD_OP_RT_STR_CONCAT_D << 8)
    mov     rdx, r10
    ret
.Lsc_null:
    mov     rax, [rip + g_gc_pending@GOTPCREL]
    cmp     dword ptr [rax], 0
    jne     .Lsc_slow
    test    edi, edi
    jne     .Lsc_nb
    cmp     dl, DT_P
    je      .Lsc_slow
    cmp     dl, DT_X
    je      .Lsc_slow
    cmp     dl, DT_FAIL
    je      .Lsc_slow
    mov     rax, rdx
    mov     rdx, rcx
    ret
.Lsc_nb:
    test    edx, edx
    jne     .Lsc_slow
    mov     r8, rdi
    shr     r8, 32
    jne     .Lsc_nb_ok
    cmp     dil, DT_S
    je      .Lsc_slow
.Lsc_nb_ok:
    cmp     dil, DT_P
    je      .Lsc_slow
    cmp     dil, DT_X
    je      .Lsc_slow
    cmp     dil, DT_FAIL
    je      .Lsc_slow
    mov     rax, rdi
    mov     rdx, rsi
    ret
.Lsc_slow:
    jmp     c_str_concat_d
RTX_ENDF(str_concat_d)
RTX_FUNC(VARVAL_fn)
    RTX_GATE(str, c_VARVAL_fn)
    cmp     dil, DT_S
    jne     c_VARVAL_fn
    test    rsi, rsi
    jz      c_VARVAL_fn
    mov     rax, rsi
    ret
RTX_ENDF(VARVAL_fn)
RTX_FUNC(rt_translate_bytes)
    RTX_GATE(str, c_rt_translate_bytes)
    test    rdx, rdx
    je      .Ltrb_done
.Ltrb_loop:
    movzx   r8d, byte ptr [rsi]
    movzx   r8d, byte ptr [rcx + r8]
    mov     [rdi], r8b
    inc     rsi
    inc     rdi
    dec     rdx
    jnz     .Ltrb_loop
.Ltrb_done:
    ret
RTX_ENDF(rt_translate_bytes)
.section .note.GNU-stack,"",@progbits
