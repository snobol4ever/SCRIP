#include "rtx_abi.inc"
RTX_GATE_DEF(icnvar)
#define VCELL_TBL        8
#define VCELL_KEY_D      24
RTX_FUNC(rt_assign_var)
    RTX_GATE(icnvar, c_rt_assign_var)
    mov     r10, [rip + g_gc_pending@GOTPCREL]
    cmp     dword ptr [r10], 0
    jne     .Lav_c
    cmp     dl, DT_S
    je      .Lav_sxt
.Lav_sxt_done:
    cmp     dil, DT_N
    jne     .Lav_c
    mov     r11, rdi
    shr     r11, 32
    cmp     r11d, 1
    je      .Lav_cell
    test    r11d, r11d
    je      .Lav_named
    cmp     r11d, 2
    je      .Lav_nametrap
    jmp     .Lav_c
.Lav_cell:
    test    rsi, rsi
    je      .Lav_c
    mov     [rsi], rdx
    mov     [rsi + 8], rcx
    mov     rax, rdx
    mov     rdx, rcx
    ret
.Lav_nametrap:
    test    rsi, rsi
    je      .Lav_c
    mov     r10, [rsi]
    test    r10, r10
    jne     .Lav_cellp_store
    mov     r11, [rsi + VCELL_TBL]
    test    r11, r11
    je      .Lav_c
    jmp     .Lav_table_store
.Lav_cellp_store:
    mov     [r10], rdx
    mov     [r10 + 8], rcx
    mov     rax, rdx
    mov     rdx, rcx
    ret
.Lav_table_store:
    mov     r9,  [rsi + VCELL_KEY_D]
    mov     r10, [rsi + VCELL_KEY_D + 8]
    RTX_CALL_ALIGN
    push    rdx
    push    rcx
    mov     rdi, r11
    mov     rsi, r9
    mov     rdx, r10
    mov     rcx, [rsp + 8]
    mov     r8,  [rsp]
    call    table_set_descr_d@PLT
    pop     rcx
    pop     rdx
    RTX_CALL_UNALIGN
    mov     rax, rdx
    mov     rdx, rcx
    ret
.Lav_named:
    test    rsi, rsi
    je      .Lav_c
    cmp     byte ptr [rsi], 0
    je      .Lav_c
    RTX_CALL_ALIGN
    push    rdx
    push    rcx
    mov     rdi, rsi
    mov     rsi, rdx
    mov     rdx, rcx
    call    NV_SET_fn@PLT
    pop     rdx
    pop     rax
    RTX_CALL_UNALIGN
    ret
.Lav_sxt:
    RTX_CALL_ALIGN
    push    rdi
    push    rsi
    push    rdx
    push    rcx
    mov     rdi, rcx
    call    rt_sxt_break@PLT
    pop     rcx
    pop     rdx
    pop     rsi
    pop     rdi
    RTX_CALL_UNALIGN
    jmp     .Lav_sxt_done
.Lav_c:
    jmp     c_rt_assign_var
RTX_ENDF(rt_assign_var)
.section .note.GNU-stack,"",@progbits
