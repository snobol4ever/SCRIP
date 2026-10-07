#include "rtx_abi.inc"
#if RT_DIAG
RTX_FUNC(rt_zdp_anchor)
    push    rax
    pushfq
    mov     rax, qword ptr [rip + g_zdp_anchor_rsp]
    test    rax, rax
    jz      .Lzdp_first
    cmp     rax, rdi
    jne     .Lzdp_report
.Lzdp_done:
    popfq
    pop     rax
    ret
.Lzdp_first:
    mov     qword ptr [rip + g_zdp_anchor_rsp], rdi
    jmp     .Lzdp_done
.Lzdp_report:
    push    rcx
    push    rdx
    push    rsi
    push    rdi
    RTX_CALL_ALIGN
    mov     rcx, rax
    call    rt_zdp_report
    RTX_CALL_UNALIGN
    pop     rdi
    pop     rsi
    pop     rdx
    pop     rcx
    RTX_GVA_R9
    jmp     .Lzdp_done
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RTX_FUNC(rt_zdp_origin)
    mov     qword ptr [rip + g_zdp_anchor_rsp], rdi
    ret
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RTX_FUNC(rt_zdp_ev)
    push    rax
    pushfq
    push    rdi
    push    rsi
    push    rdx
    push    rcx
    RTX_CALL_ALIGN
    call    rt_zdp_sm_event
    RTX_CALL_UNALIGN
    pop     rcx
    pop     rdx
    pop     rsi
    pop     rdi
    RTX_GVA_R9
    popfq
    pop     rax
    ret
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RTX_FUNC(rt_zdp_probe)
    push    rax
    pushfq
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
    popfq
    pop     rax
    ret
.Lzdpp_rbp_save:
    mov     qword ptr [rip + g_zdp_anchor_rbp], rbp
    jmp     .Lzdpp_done
.Lzdpp_rbp:
    cmp     rbp, qword ptr [rip + g_zdp_anchor_rbp]
    jne     .Lzdpp_report
    jmp     .Lzdpp_done
.Lzdpp_record:
.Lzdpp_report:
    push    rcx
    push    rdx
    push    rsi
    push    rdi
    RTX_CALL_ALIGN
    mov     r9, rax
    call    rt_zdp_probe_report
    RTX_CALL_UNALIGN
    pop     rdi
    pop     rsi
    pop     rdx
    pop     rcx
    RTX_GVA_R9
    jmp     .Lzdpp_done
#else
RTX_FUNC(rt_zdp_anchor)
    ret
RTX_FUNC(rt_zdp_origin)
    ret
RTX_FUNC(rt_zdp_ev)
    ret
RTX_FUNC(rt_zdp_probe)
    ret
#endif
.section .note.GNU-stack,"",@progbits
