#include "rtx_abi.inc"
RTX_GATE_DEF(icnagg)
#define DATINST_TYPE      0
#define DATINST_FIELDS    8
#define DATBLK_NFIELDS    8
#define DATBLK_FIELDS    16
#define DATA_INST_SLEN    0
#define DATA_ELEMS_SLEN   1
#define FIELD0_V          0
#define FIELD0_SLEN       4
#define FIELD0_P          8
#define FIELD1_P         24
#define FIELD2_V         32
#define FIELD2_P         40
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
    RTX_DECLINE
RTX_ENDF(rt_size_d)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RTX_FUNC(rt_list_bang_at)
    RTX_GATE(icnagg, .Lbang_slow)
    cmp     dil, DT_DATA
    jne     .Lbang_slow
    mov     r8, rdi
    shr     r8, 32
    cmp     r8d, DATA_INST_SLEN
    jne     .Lbang_slow
    test    rsi, rsi
    je      .Lbang_slow
    test    rdx, rdx
    js      .Lbang_slow
    mov     r8, [rsi + DATINST_TYPE]
    test    r8, r8
    je      .Lbang_slow
    cmp     dword ptr [r8 + DATBLK_NFIELDS], 3
    jl      .Lbang_slow
    mov     r10, [r8 + DATBLK_FIELDS]
    test    r10, r10
    je      .Lbang_slow
    mov     r10, [r10]
    test    r10, r10
    je      .Lbang_slow
    mov     r11, 0x6c655f656d617266
    cmp     [r10], r11
    jne     .Lbang_slow
    cmp     dword ptr [r10 + 8], 0x00736d65
    jne     .Lbang_slow
    mov     r8, [rsi + DATINST_FIELDS]
    test    r8, r8
    je      .Lbang_slow
    cmp     dword ptr [r8 + FIELD2_V], DT_S
    jne     .Lbang_slow
    mov     r10, [r8 + FIELD2_P]
    test    r10, r10
    je      .Lbang_slow
    cmp     dword ptr [r10], 0x7473696c
    jne     .Lbang_slow
    cmp     byte ptr [r10 + 4], 0
    jne     .Lbang_slow
    cmp     dword ptr [r8 + FIELD0_V], DT_DATA
    jne     .Lbang_slow
    cmp     dword ptr [r8 + FIELD0_SLEN], DATA_ELEMS_SLEN
    jne     .Lbang_slow
    mov     r10, [r8 + FIELD0_P]
    test    r10, r10
    je      .Lbang_slow
    movsxd  r11, dword ptr [r8 + FIELD1_P]
    cmp     rdx, r11
    jge     .Lbang_done
    shl     rdx, 4
    mov     rax, [r10 + rdx]
    mov     rdx, [r10 + rdx + 8]
    ret
.Lbang_done:
    mov     eax, DT_FAIL
    xor     edx, edx
    ret
.Lbang_slow:
    RTX_SUB_RSP(24)
    mov     rcx, rsp
    call    list_bang_at@PLT
    RTX_GVA_R9
    test    eax, eax
    je      .Lbang_fail
    mov     rax, [rsp]
    mov     rdx, [rsp + 8]
    RTX_ADD_RSP(24)
    ret
.Lbang_fail:
    RTX_CFA(32)
    mov     eax, DT_FAIL
    xor     edx, edx
    RTX_ADD_RSP(24)
    ret
RTX_ENDF(rt_list_bang_at)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RTX_FUNC(dat_field_get)
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
    RTX_GVA_R9
    ret
.Ldfg_c:
    mov     eax, RTX_NOT_HANDLED
    RTX_GVA_R9
    ret
RTX_ENDF(dat_field_get)
.section .note.GNU-stack,"",@progbits
