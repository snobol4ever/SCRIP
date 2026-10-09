#include "rtx_abi.inc"
RTX_GATE_DEF(icnvar)
#define VCELL_TBL        8
#define VCELL_KEY_D      24
RTX_FUNC(rt_assign_var)
    RTX_GATE(icnvar, .Lav_c)
    mov     rax, [rip + g_gc_pending@GOTPCREL]
    cmp     dword ptr [rax], 0
    jne     .Lav_c
    cmp     dl, DT_S
    je      .Lav_sxt
.Lav_sxt_done:
    cmp     dil, DT_N
    jne     .Lav_c
    mov     rax, rdi
    shr     rax, 32
    cmp     eax, 1
    je      .Lav_cell
    test    eax, eax
    je      .Lav_named
    cmp     eax, 2
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
#if RT_DIAG
    mov     rax, [rip + g_sno_etrace_n@GOTPCREL]
    cmp     dword ptr [rax], 0
    jne     .Lav_c
#endif
    mov     rax, [rsi]
    test    rax, rax
    jne     .Lav_cellp_store
    mov     rax, [rsi + VCELL_TBL]
    test    rax, rax
    je      .Lav_c
    jmp     .Lav_table_store
.Lav_cellp_store:
    mov     [rax], rdx
    mov     [rax + 8], rcx
    mov     rax, rdx
    mov     rdx, rcx
    ret
.Lav_table_store:
    RTX_CALL_ALIGN(8)
    RTX_PUSH_DYN(rdx, 16, 8)
    RTX_PUSH_DYN(rcx, 24, 8)
    mov     rdi, rax
    mov     rdx, [rsi + VCELL_KEY_D + 8]
    mov     rsi, [rsi + VCELL_KEY_D]
    mov     rcx, [rsp + 8]
    mov     r8,  [rsp]
    call    table_set_descr_d@PLT
    RTX_POP_DYN(rcx, 16, 8)
    RTX_POP_DYN(rdx, 8, 8)
    RTX_CALL_UNALIGN(8)
    RTX_GVA_R9
    mov     rax, rdx
    mov     rdx, rcx
    ret
.Lav_named:
    test    rsi, rsi
    je      .Lav_c
    cmp     byte ptr [rsi], 0
    je      .Lav_c
    RTX_CALL_ALIGN(8)
    RTX_PUSH_DYN(rdx, 16, 8)
    RTX_PUSH_DYN(rcx, 24, 8)
    mov     rdi, rsi
    mov     rsi, rdx
    mov     rdx, rcx
    call    NV_SET_fn@PLT
    RTX_POP_DYN(rdx, 16, 8)
    RTX_POP_DYN(rax, 8, 8)
    RTX_CALL_UNALIGN(8)
    RTX_GVA_R9
    ret
.Lav_sxt:
    RTX_CALL_ALIGN(8)
    RTX_PUSH_DYN(rdi, 16, 8)
    RTX_PUSH_DYN(rsi, 24, 8)
    RTX_PUSH_DYN(rdx, 32, 8)
    RTX_PUSH_DYN(rcx, 40, 8)
    mov     rdi, rcx
    call    rt_sxt_break@PLT
    RTX_POP_DYN(rcx, 32, 8)
    RTX_POP_DYN(rdx, 24, 8)
    RTX_POP_DYN(rsi, 16, 8)
    RTX_POP_DYN(rdi, 8, 8)
    RTX_CALL_UNALIGN(8)
    RTX_GVA_R9
    jmp     .Lav_sxt_done
.Lav_c:
    RTX_CTAIL(c_rt_assign_var)
RTX_ENDF(rt_assign_var)
.section .note.GNU-stack,"",@progbits
