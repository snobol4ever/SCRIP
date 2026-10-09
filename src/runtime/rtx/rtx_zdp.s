#include "rtx_abi.inc"
#if RT_DIAG
RTX_FUNC(rt_zdp_anchor)
    RTX_PUSH(rax)
    pushfq  ; .cfi_adjust_cfa_offset 8
    mov     rax, qword ptr [rip + g_zdp_anchor_rsp]
    test    rax, rax
    jz      .Lzdp_first
    cmp     rax, rdi
    jne     .Lzdp_report
.Lzdp_done:
    popfq   ; .cfi_adjust_cfa_offset -8
    RTX_POP(rax)
    ret
.Lzdp_first:
    RTX_CFA(24)
    mov     qword ptr [rip + g_zdp_anchor_rsp], rdi
    jmp     .Lzdp_done
.Lzdp_report:
    RTX_CFA(24)
    RTX_PUSH(rcx)
    RTX_PUSH(rdx)
    RTX_PUSH(rsi)
    RTX_PUSH(rdi)
    RTX_CALL_ALIGN(56)
    mov     rcx, rax
    call    rt_zdp_report
    RTX_CALL_UNALIGN(56)
    RTX_POP(rdi)
    RTX_POP(rsi)
    RTX_POP(rdx)
    RTX_POP(rcx)
    RTX_GVA_R9
    jmp     .Lzdp_done
RTX_ENDF(rt_zdp_anchor)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RTX_FUNC(rt_zdp_origin)
    mov     qword ptr [rip + g_zdp_anchor_rsp], rdi
    ret
RTX_ENDF(rt_zdp_origin)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RTX_FUNC(rt_zdp_ev)
    RTX_PUSH(rax)
    pushfq  ; .cfi_adjust_cfa_offset 8
    RTX_PUSH(rdi)
    RTX_PUSH(rsi)
    RTX_PUSH(rdx)
    RTX_PUSH(rcx)
    RTX_CALL_ALIGN(56)
    call    rt_zdp_sm_event
    RTX_CALL_UNALIGN(56)
    RTX_POP(rcx)
    RTX_POP(rdx)
    RTX_POP(rsi)
    RTX_POP(rdi)
    RTX_GVA_R9
    popfq   ; .cfi_adjust_cfa_offset -8
    RTX_POP(rax)
    ret
RTX_ENDF(rt_zdp_ev)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RTX_FUNC(rt_zdp_probe)
    RTX_PUSH(rax)
    pushfq  ; .cfi_adjust_cfa_offset 8
    mov     rax, qword ptr [rip + g_zdp_anchor_rsp]
    test    rax, rax
    jz      .Lzdpp_done
    sub     rax, rdi
    cmp     rcx, -1
    je      .Lzdpp_record
    cmp     rax, rcx
    jne     .Lzdpp_report
    test    r8, 4
    jnz     .Lzdpp_rbp
    test    r8, 8
    jnz     .Lzdpp_rbp_save
.Lzdpp_done:
    popfq   ; .cfi_adjust_cfa_offset -8
    RTX_POP(rax)
    ret
.Lzdpp_rbp_save:
    RTX_CFA(24)
    mov     qword ptr [rip + g_zdp_anchor_rbp], rbp
    jmp     .Lzdpp_done
.Lzdpp_rbp:
    RTX_CFA(24)
    cmp     rbp, qword ptr [rip + g_zdp_anchor_rbp]
    jne     .Lzdpp_report
    jmp     .Lzdpp_done
.Lzdpp_record:
.Lzdpp_report:
    RTX_CFA(24)
    RTX_PUSH(rcx)
    RTX_PUSH(rdx)
    RTX_PUSH(rsi)
    RTX_PUSH(rdi)
    RTX_CALL_ALIGN(56)
    mov     r9, rax
    call    rt_zdp_probe_report
    RTX_CALL_UNALIGN(56)
    RTX_POP(rdi)
    RTX_POP(rsi)
    RTX_POP(rdx)
    RTX_POP(rcx)
    RTX_GVA_R9
    jmp     .Lzdpp_done
RTX_ENDF(rt_zdp_probe)
#else
RTX_FUNC(rt_zdp_anchor)
    ret
RTX_ENDF(rt_zdp_anchor)
RTX_FUNC(rt_zdp_origin)
    ret
RTX_ENDF(rt_zdp_origin)
RTX_FUNC(rt_zdp_ev)
    ret
RTX_ENDF(rt_zdp_ev)
RTX_FUNC(rt_zdp_probe)
    ret
RTX_ENDF(rt_zdp_probe)
#endif
.section .note.GNU-stack,"",@progbits
