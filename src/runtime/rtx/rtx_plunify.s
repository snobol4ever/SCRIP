#include "rtx_abi.inc"
RTX_GATE_DEF(plunify)
#define CTX_TR            0
#define CTX_B             8
#define CTX_FRAME        24
RTX_FUNC(rt_pl_quad_seed)
    sub     rsp, 8
    mov     r14, rdi
    xor     r13d, r13d
    xor     r15d, r15d
    call    rt_pl_tr_init
    mov     r12, rax
    add     rsp, 8
    ret
RTX_ENDF(rt_pl_quad_seed)
RTX_FUNC(rt_pl_tr_unwind)
    sub     rsp, 8
    mov     rsi, rdi
    mov     rdi, r12
    call    rt_pl_tr_unwind_sync
    mov     r12, rax
    add     rsp, 8
    ret
RTX_ENDF(rt_pl_tr_unwind)
RTX_FUNC(rt_pl_choice_open)
    mov     r13, rdi
    ret
RTX_ENDF(rt_pl_choice_open)
RTX_FUNC(rt_pl_dop_unify)
    sub     rsp, CTX_FRAME
    mov     qword ptr [rsp + CTX_TR], r12
    mov     qword ptr [rsp + CTX_B], r13
    mov     rdx, rsp
    call    rt_pl_dop_unify_c
    mov     r12, qword ptr [rsp + CTX_TR]
    add     rsp, CTX_FRAME
    cmp     al, DT_FAIL
    jne     .Lpu_ret
    mov     eax, DT_FAIL | (MOD_OP_RT_PL_DOP_UNIFY << 8)
    xor     edx, edx
.Lpu_ret:
    ret
RTX_ENDF(rt_pl_dop_unify)
RTX_FUNC(rt_pl_dop_unify_ci)
    sub     rsp, CTX_FRAME
    mov     qword ptr [rsp + CTX_TR], r12
    mov     qword ptr [rsp + CTX_B], r13
    mov     rdx, rsp
    call    rt_pl_dop_unify_ci_c
    mov     r12, qword ptr [rsp + CTX_TR]
    add     rsp, CTX_FRAME
    ret
RTX_ENDF(rt_pl_dop_unify_ci)
RTX_FUNC(rt_pl_dop_unify_cs)
    sub     rsp, CTX_FRAME
    mov     qword ptr [rsp + CTX_TR], r12
    mov     qword ptr [rsp + CTX_B], r13
    mov     rdx, rsp
    call    rt_pl_dop_unify_cs_c
    mov     r12, qword ptr [rsp + CTX_TR]
    add     rsp, CTX_FRAME
    ret
RTX_ENDF(rt_pl_dop_unify_cs)
RTX_FUNC(rt_pl_dop_mkc)
    sub     rsp, CTX_FRAME
    mov     qword ptr [rsp + CTX_TR], r12
    mov     qword ptr [rsp + CTX_B], r13
    mov     rdx, rsp
    call    rt_pl_dop_mkc_c
    mov     r12, qword ptr [rsp + CTX_TR]
    add     rsp, CTX_FRAME
    ret
RTX_ENDF(rt_pl_dop_mkc)
RTX_FUNC(rt_pl_dop_is_v)
    sub     rsp, CTX_FRAME
    mov     qword ptr [rsp + CTX_TR], r12
    mov     qword ptr [rsp + CTX_B], r13
    mov     rdx, rsp
    call    rt_pl_dop_is_v_c
    mov     r12, qword ptr [rsp + CTX_TR]
    add     rsp, CTX_FRAME
    ret
RTX_ENDF(rt_pl_dop_is_v)
.section .note.GNU-stack,"",@progbits
