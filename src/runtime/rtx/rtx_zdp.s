#include "rtx_abi.inc"
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
    push    r8
    push    r9
    push    r10
    push    r11
    push    rbp
    mov     rbp, rsp
    and     rsp, -16
    mov     rcx, rax
    call    rt_zdp_report
    mov     rsp, rbp
    pop     rbp
    pop     r11
    pop     r10
    pop     r9
    pop     r8
    pop     rdi
    pop     rsi
    pop     rdx
    pop     rcx
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
    push    r8
    push    r9
    push    r10
    push    r11
    push    rdi
    push    rsi
    push    rdx
    push    rcx
    push    rbp
    mov     rbp, rsp
    and     rsp, -16
    call    rt_zdp_sm_event
    mov     rsp, rbp
    pop     rbp
    pop     rcx
    pop     rdx
    pop     rsi
    pop     rdi
    pop     r11
    pop     r10
    pop     r9
    pop     r8
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
    push    r8
    push    r9
    push    r10
    push    r11
    push    rbp
    mov     rbp, rsp
    and     rsp, -16
    push    r8
    push    rax
    mov     r9, rax
    call    rt_zdp_probe_report
    pop     rax
    pop     r8
    mov     rsp, rbp
    pop     rbp
    pop     r11
    pop     r10
    pop     r9
    pop     r8
    pop     rdi
    pop     rsi
    pop     rdx
    pop     rcx
    jmp     .Lzdpp_done
