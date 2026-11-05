#include "rtx_abi.inc"
RTX_GATE_DEF(icnvar)
#define VCELL_TBL        8
#define VCELL_KEY_D      24
.macro RTX_AV_BODY sfx, dec
.Lav_go_\sfx:
    mov     rax, [rip + g_gc_pending@GOTPCREL]
    cmp     dword ptr [rax], 0
    jne     \dec
    cmp     dl, DT_S
    je      .Lav_sxt_\sfx
.Lav_sxt_done_\sfx:
    cmp     dil, DT_N
    jne     \dec
    mov     rax, rdi
    shr     rax, 32
    cmp     eax, 1
    je      .Lav_cell_\sfx
    test    eax, eax
    je      .Lav_named_\sfx
    cmp     eax, 2
    je      .Lav_nametrap_\sfx
    jmp     \dec
.Lav_cell_\sfx:
    test    rsi, rsi
    je      \dec
    mov     [rsi], rdx
    mov     [rsi + 8], rcx
    mov     rax, rdx
    mov     rdx, rcx
    ret
.Lav_nametrap_\sfx:
    test    rsi, rsi
    je      \dec
#if RT_DIAG
    mov     rax, qword ptr [rip + g_runtime@GOTPCREL]
    cmp     dword ptr [rax + G_RT_OFF_SNO_ETRACE_N], 0
    jne     \dec
#endif
    mov     rax, [rsi]
    test    rax, rax
    jne     .Lav_cellp_store_\sfx
    mov     rax, [rsi + VCELL_TBL]
    test    rax, rax
    je      \dec
    jmp     .Lav_table_store_\sfx
.Lav_cellp_store_\sfx:
    mov     [rax], rdx
    mov     [rax + 8], rcx
    mov     rax, rdx
    mov     rdx, rcx
    ret
.Lav_table_store_\sfx:
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
.Lav_named_\sfx:
    test    rsi, rsi
    je      \dec
    cmp     byte ptr [rsi], 0
    je      \dec
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
.Lav_sxt_\sfx:
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
    jmp     .Lav_sxt_done_\sfx
.endm
RTX_FUNC(rt_assign_var)
    RTX_GATE(icnvar, .Lav_c)
    RTX_AV_BODY a, .Lav_c
.Lav_c:
    RTX_DECLINE
RTX_ENDF(rt_assign_var)
RTX_FUNC(rt_assign_var_strict)
    RTX_GATE(icnvar, .Lavs_c)
    RTX_AV_BODY s, .Lavs_c
.Lavs_c:
    RTX_SLOW(c_rt_assign_var_strict)
RTX_ENDF(rt_assign_var_strict)
.section .note.GNU-stack,"",@progbits
