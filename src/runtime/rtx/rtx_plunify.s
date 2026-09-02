#include "rtx_abi.inc"
RTX_GATE_DEF(plunify)
#define D_LO               0
#define D_HI               8
#define D_SIZE            16
#define FR_ARGS           16
#define FR_FLOOR           8
RTX_FUNC(rt_pl_dop_unify)
    cmp     esi, 2
    jne     .Lpu_fail_bare
    sub     rsp, 24
    mov     qword ptr [rsp + FR_ARGS], rdi
    mov     rax, qword ptr [rip + g_plw_unwind_floor@GOTPCREL]
    mov     rcx, qword ptr [rax]
    mov     qword ptr [rsp + FR_FLOOR], rcx
    lea     rcx, [rsp + 16]
    mov     qword ptr [rax], rcx
    mov     rax, qword ptr [rip + g_gc_pending@GOTPCREL]
    cmp     dword ptr [rax], 0
    jne     .Lpu_gc
.Lpu_gc_done:
    mov     rax, qword ptr [rsp + FR_ARGS]
    mov     rdi, qword ptr [rax + D_LO]
    mov     rsi, qword ptr [rax + D_HI]
    mov     rdx, qword ptr [rax + D_SIZE + D_LO]
    mov     rcx, qword ptr [rax + D_SIZE + D_HI]
    call    plw_unify_vals
    test    eax, eax
    je      .Lpu_fail
    mov     rax, qword ptr [rsp + FR_ARGS]
    mov     rdi, qword ptr [rax + D_LO]
    mov     rsi, qword ptr [rax + D_HI]
    call    rt_pl_deref_val
    jmp     .Lpu_restore
.Lpu_fail:
    mov     eax, DT_FAIL | (MOD_OP_RT_PL_DOP_UNIFY << 8)
    xor     edx, edx
.Lpu_restore:
    mov     rcx, qword ptr [rip + g_plw_unwind_floor@GOTPCREL]
    mov     r8,  qword ptr [rsp + FR_FLOOR]
    mov     qword ptr [rcx], r8
    add     rsp, 24
    ret
.Lpu_gc:
    mov     rdi, qword ptr [rsp + FR_ARGS]
    mov     esi, 2
    xor     edx, edx
    call    rt_gc_point_arr
    jmp     .Lpu_gc_done
.Lpu_fail_bare:
    mov     eax, DT_FAIL | (MOD_OP_RT_PL_DOP_UNIFY << 8)
    xor     edx, edx
    ret
RTX_ENDF(rt_pl_dop_unify)
.section .note.GNU-stack,"",@progbits
