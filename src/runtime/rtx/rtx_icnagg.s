#include "rtx_abi.inc"
RTX_GATE_DEF(icnagg)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RTX_FUNC(rt_size_d)
    RTX_GATE(icnagg, c_rt_size_d)
    cmp     dil, DT_SNUL
    je      .Lsz_zero
    cmp     dil, DT_S
    jne     c_rt_size_d
    mov     rax, rdi
    shr     rax, 32
    test    eax, eax
    je      c_rt_size_d
    cmp     eax, -1
    je      c_rt_size_d
    mov     rdx, rax
    mov     eax, DT_I | (MOD_OP_RT_SIZE_D << 8)
    ret
.Lsz_zero:
    xor     edx, edx
    mov     eax, DT_I | (MOD_OP_RT_SIZE_D << 8)
    ret
RTX_ENDF(rt_size_d)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RTX_FUNC(rt_list_bang_at)
    sub     rsp, 24
    mov     rcx, rsp
    call    list_bang_at@PLT
    test    eax, eax
    je      .Lbang_fail
    mov     rax, [rsp]
    mov     rdx, [rsp + 8]
    add     rsp, 24
    ret
.Lbang_fail:
    mov     eax, DT_FAIL | (MOD_OP_RT_LIST_BANG_AT << 8)
    xor     edx, edx
    add     rsp, 24
    ret
RTX_ENDF(rt_list_bang_at)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RTX_FUNC(dat_field_get)
    RTX_GATE(icnagg, c_dat_field_get)
    cmp     sil, DT_DATA
    jl      c_dat_field_get
    test    rdx, rdx
    je      c_dat_field_get
    mov     r8, [rdx]
    test    r8, r8
    je      c_dat_field_get
    mov     ecx, [r8 + 8]
    test    ecx, ecx
    jle     c_dat_field_get
    mov     r9, [r8 + 16]
    test    r9, r9
    je      c_dat_field_get
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
    jmp     c_dat_field_get
.Ldfg_hit:
    mov     r8, [rdx + 8]
    test    r8, r8
    je      c_dat_field_get
    shl     r10, 4
    add     r8, r10
    mov     rax, [r8]
    mov     rdx, [r8 + 8]
    ret
RTX_ENDF(dat_field_get)
.section .note.GNU-stack,"",@progbits
