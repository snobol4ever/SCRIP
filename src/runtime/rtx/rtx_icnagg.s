#include "rtx_abi.inc"
RTX_GATE_DEF(icnagg)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RTX_FUNC(rt_size_d)
    RTX_GATE(icnagg, .Lsz_c)
    cmp     dil, DT_S
    jne     .Lsz_c
    mov     rax, rdi
    shr     rax, 32
    test    eax, eax
    je      .Lsz_c
    cmp     eax, -1
    je      .Lsz_c
    mov     rdx, rax
    mov     eax, DT_I
    ret
.Lsz_c:
    RTX_CTAIL(c_rt_size_d)
RTX_ENDF(rt_size_d)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RTX_FUNC(rt_list_bang_at)
    sub     rsp, 24
    mov     rcx, rsp
    RTX_CCALL(list_bang_at@PLT)
    test    eax, eax
    je      .Lbang_fail
    mov     rax, [rsp]
    mov     rdx, [rsp + 8]
    add     rsp, 24
    ret
.Lbang_fail:
    mov     eax, DT_FAIL
    xor     edx, edx
    add     rsp, 24
    ret
RTX_ENDF(rt_list_bang_at)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RTX_FUNC(dat_field_get)
    RTX_SAVE
    RTX_GATE(icnagg, .Ldfg_c)
    cmp     sil, DT_DATA
    jl      .Ldfg_c
    mov     r8, rsi
    shr     r8, 32
    test    r8d, r8d
    jne     .Ldfg_c
    test    rdx, rdx
    je      .Ldfg_c
    mov     r8, [rdx]
    test    r8, r8
    je      .Ldfg_c
    mov     ecx, [r8 + 8]
    test    ecx, ecx
    jle     .Ldfg_c
    mov     r9, [r8 + 16]
    test    r9, r9
    je      .Ldfg_c
    xor     r10d, r10d
.Ldfg_field:
    mov     r11, [r9 + r10*8]
    test    r11, r11
    je      .Ldfg_next
    xor     eax, eax
.Ldfg_byte:
    movzx   r8d, byte ptr [r11 + rax]
    cmp     r8b, byte ptr [rdi + rax]
    jne     .Ldfg_next
    test    r8b, r8b
    je      .Ldfg_hit
    inc     rax
    jmp     .Ldfg_byte
.Ldfg_next:
    inc     r10d
    cmp     r10d, ecx
    jl      .Ldfg_field
    jmp     .Ldfg_c
.Ldfg_hit:
    mov     r8, [rdx + 8]
    test    r8, r8
    je      .Ldfg_c
    shl     r10, 4
    add     r8, r10
    mov     rax, [r8]
    mov     rdx, [r8 + 8]
    RTX_RET
.Ldfg_c:
    RTX_CTAIL_SAVED(c_dat_field_get)
RTX_ENDF(dat_field_get)
.section .note.GNU-stack,"",@progbits
