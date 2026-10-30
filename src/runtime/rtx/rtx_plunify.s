#include "rtx_abi.inc"
RTX_GATE_DEF(plunify)
#define CTX_TR            0
#define CTX_B             8
#define CTX_BALL         16
#define CTX_RBX          24
#define CTX_R14          32
#define CTX_R15          40
#define CTX_FRAME        56
#define CTX_SAVE mov qword ptr [rsp + CTX_TR], r12; mov qword ptr [rsp + CTX_B], r13; mov qword ptr [rsp + CTX_RBX], rbx; mov qword ptr [rsp + CTX_R14], r14; mov qword ptr [rsp + CTX_R15], r15
#define PL_TR_ARENA_MASK  -536870912
#define PL_TR_ENTRY_BYTES 32
#define PL_TR_BALL_SLOT   8
#define PL_TR_OCHECK_SLOT 24
#define PL_VAR_ATTV_MASK  0x1FF
#define PL_VAR_ATTV_WORD  0x148
#define PL_SERIAL_SLOT    -2064
#define PL_SERIAL_STEP    512
#define PL_BALL_ARM(r, t)  mov r15, r; mov t, r12; and t, PL_TR_ARENA_MASK; mov qword ptr [t + PL_TR_BALL_SLOT], r
#define PL_BALL_DROP(t)    xor r15d, r15d; mov t, r12; and t, PL_TR_ARENA_MASK; mov qword ptr [t + PL_TR_BALL_SLOT], 0
#define PL_BALL_GET(d)  mov d, r12; and d, PL_TR_ARENA_MASK; mov d, qword ptr [d + PL_TR_BALL_SLOT]
RTX_FUNC(rt_pl_quad_seed)
    RTX_SUB_RSP(8)
    mov     r14, rdi
    xor     r13d, r13d
    call rt_pl_tr_init; RTX_GVA_R9
    mov     r12, rax
    PL_BALL_DROP(rcx)
    RTX_ADD_RSP(8)
    ret
RTX_ENDF(rt_pl_quad_seed)
RTX_FUNC(rt_pl_throw_raise)
    RTX_SUB_RSP(8)
    call rt_pl_ball_make; RTX_GVA_R9
    PL_BALL_ARM(rax, rcx)
    RTX_ADD_RSP(8)
    mov     eax, DT_FAIL
    xor     edx, edx
    ret
RTX_ENDF(rt_pl_throw_raise)
RTX_FUNC(rt_pl_exist_raise)
    RTX_SUB_RSP(8)
    call rt_pl_ball_existence; RTX_GVA_R9
    PL_BALL_ARM(rax, rcx)
    RTX_ADD_RSP(8)
    mov     eax, DT_FAIL
    xor     edx, edx
    ret
RTX_ENDF(rt_pl_exist_raise)
RTX_FUNC(rt_pl_cap_raise)
    cmp     dword ptr [rip + g_runtime + G_RT_OFF_GC_HP_CAP_DUE], 1
    je      .Lcr_go
    cmp     dword ptr [rip + g_runtime + G_RT_OFF_GC_HP_CAP_DUE], 3
    jne     .Lcr_ret
.Lcr_go:
    RTX_PUSHS(rcx)
    RTX_PUSHS(rsi)
    RTX_PUSHS(rdi)
    RTX_PUSHS(r8)
    RTX_PUSHS(r10)
    RTX_PUSHS(r11)
    RTX_SUB_RSP(8)
    call rt_pl_cap_ball; RTX_GVA_R9
    PL_BALL_ARM(rax, rcx)
    RTX_ADD_RSP(8)
    RTX_POPS(r11)
    RTX_POPS(r10)
    RTX_POPS(r8)
    RTX_POPS(rdi)
    RTX_POPS(rsi)
    RTX_POPS(rcx)
    mov     eax, DT_FAIL
    xor     edx, edx
.Lcr_ret:
    ret
RTX_ENDF(rt_pl_cap_raise)
RTX_FUNC(rt_pl_goal_resolve)
    RTX_SUB_RSP(24)
    mov     qword ptr [rsp + 8], 0
    lea     rcx, [rsp + 8]
    mov     r8, r14
    call    rt_pl_goal_resolve_c
    mov     rcx, qword ptr [rsp + 8]
    RTX_ADD_RSP(24)
    test    rcx, rcx
    jz      .Lgr_ret
    PL_BALL_ARM(rcx, rax)
    xor     eax, eax
    xor     edx, edx
.Lgr_ret:
    RTX_GVA_R9
    ret
RTX_ENDF(rt_pl_goal_resolve)
RTX_FUNC(rt_pl_catch_handle)
    test    r15, r15
    jz      .Lch_fail
    RTX_SUB_RSP(CTX_FRAME)
    CTX_SAVE
    PL_BALL_GET(rdx)
    mov     rcx, rsp
    call rt_pl_catch_handle_c; RTX_GVA_R9
    mov     r12, qword ptr [rsp + CTX_TR]
    RTX_ADD_RSP(CTX_FRAME)
    test    eax, eax
    jz      .Lch_fail
    PL_BALL_DROP(rdx)
    mov     eax, DT_I
    mov     edx, 1
    ret
.Lch_fail:
    mov     eax, DT_FAIL
    xor     edx, edx
    ret
RTX_ENDF(rt_pl_catch_handle)
RTX_FUNC(rt_pl_ball_take)
    PL_BALL_GET(rax)
    PL_BALL_DROP(rcx)
    ret
RTX_ENDF(rt_pl_ball_take)
RTX_FUNC(rt_pl_dop_ball_pending)
    PL_BALL_GET(rdi)
    RTX_SUB_RSP(8)
    call    rt_pl_dop_ball_pending_c
    RTX_ADD_RSP(8)
    RTX_GVA_R9
    ret
RTX_ENDF(rt_pl_dop_ball_pending)
RTX_FUNC(rt_pl_dop_db_data_unify)
    RTX_SUB_RSP(CTX_FRAME)
    CTX_SAVE
    mov     qword ptr [rsp + CTX_BALL], 0
    mov     rdx, rsp
    mov     rcx, r14
    call rt_pl_dop_db_data_unify_c; RTX_GVA_R9
    mov     r12, qword ptr [rsp + CTX_TR]
    mov     rcx, qword ptr [rsp + CTX_BALL]
    RTX_ADD_RSP(CTX_FRAME)
    cmp     al, DT_FAIL
    jne     .Lpdu_ret
    test    rcx, rcx
    jz      .Lpdu_fail
    PL_BALL_ARM(rcx, rdx)
.Lpdu_fail:
    mov     eax, DT_FAIL
    xor     edx, edx
.Lpdu_ret:
    ret
RTX_ENDF(rt_pl_dop_db_data_unify)
RTX_FUNC(rt_pl_dop_dl_push)
    RTX_SUB_RSP(CTX_FRAME)
    CTX_SAVE
    mov     qword ptr [rsp + CTX_BALL], 0
    mov     rdx, rsp
    mov     rcx, rbp
    call rt_pl_dop_dl_push_c; RTX_GVA_R9
    mov     r12, qword ptr [rsp + CTX_TR]
    mov     rcx, qword ptr [rsp + CTX_BALL]
    RTX_ADD_RSP(CTX_FRAME)
    cmp     al, DT_FAIL
    jne     .Lpdl_dl_push_ret
    test    rcx, rcx
    jz      .Lpdl_dl_push_fail
    PL_BALL_ARM(rcx, rdx)
.Lpdl_dl_push_fail:
    mov     eax, DT_FAIL
    xor     edx, edx
.Lpdl_dl_push_ret:
    ret
RTX_ENDF(rt_pl_dop_dl_push)
RTX_FUNC(rt_pl_dop_dl_reached)
    RTX_SUB_RSP(CTX_FRAME)
    CTX_SAVE
    mov     qword ptr [rsp + CTX_BALL], 0
    mov     rdx, rsp
    mov     rcx, rbp
    call rt_pl_dop_dl_reached_c; RTX_GVA_R9
    mov     r12, qword ptr [rsp + CTX_TR]
    mov     rcx, qword ptr [rsp + CTX_BALL]
    RTX_ADD_RSP(CTX_FRAME)
    cmp     al, DT_FAIL
    jne     .Lpdl_dl_reached_ret
    test    rcx, rcx
    jz      .Lpdl_dl_reached_fail
    PL_BALL_ARM(rcx, rdx)
.Lpdl_dl_reached_fail:
    mov     eax, DT_FAIL
    xor     edx, edx
.Lpdl_dl_reached_ret:
    ret
RTX_ENDF(rt_pl_dop_dl_reached)
RTX_FUNC(rt_pl_dop_dl_false)
    RTX_SUB_RSP(CTX_FRAME)
    CTX_SAVE
    mov     qword ptr [rsp + CTX_BALL], 0
    mov     rdx, rsp
    mov     rcx, rbp
    call rt_pl_dop_dl_false_c; RTX_GVA_R9
    mov     r12, qword ptr [rsp + CTX_TR]
    mov     rcx, qword ptr [rsp + CTX_BALL]
    RTX_ADD_RSP(CTX_FRAME)
    cmp     al, DT_FAIL
    jne     .Lpdl_dl_false_ret
    test    rcx, rcx
    jz      .Lpdl_dl_false_fail
    PL_BALL_ARM(rcx, rdx)
.Lpdl_dl_false_fail:
    mov     eax, DT_FAIL
    xor     edx, edx
.Lpdl_dl_false_ret:
    ret
RTX_ENDF(rt_pl_dop_dl_false)
RTX_FUNC(rt_pl_dop_dl_reset)
    RTX_SUB_RSP(CTX_FRAME)
    CTX_SAVE
    mov     qword ptr [rsp + CTX_BALL], 0
    mov     rdx, rsp
    mov     rcx, rbp
    call rt_pl_dop_dl_reset_c; RTX_GVA_R9
    mov     r12, qword ptr [rsp + CTX_TR]
    mov     rcx, qword ptr [rsp + CTX_BALL]
    RTX_ADD_RSP(CTX_FRAME)
    cmp     al, DT_FAIL
    jne     .Lpdl_dl_reset_ret
    test    rcx, rcx
    jz      .Lpdl_dl_reset_fail
    PL_BALL_ARM(rcx, rdx)
.Lpdl_dl_reset_fail:
    mov     eax, DT_FAIL
    xor     edx, edx
.Lpdl_dl_reset_ret:
    ret
RTX_ENDF(rt_pl_dop_dl_reset)
RTX_FUNC(rt_pl_dop_dl_tick)
    RTX_SUB_RSP(CTX_FRAME)
    CTX_SAVE
    mov     qword ptr [rsp + CTX_BALL], 0
    mov     rdx, rsp
    mov     rcx, rbp
    call rt_pl_dop_dl_tick_c; RTX_GVA_R9
    mov     r12, qword ptr [rsp + CTX_TR]
    mov     rcx, qword ptr [rsp + CTX_BALL]
    RTX_ADD_RSP(CTX_FRAME)
    cmp     al, DT_FAIL
    jne     .Lpdl_dl_tick_ret
    test    rcx, rcx
    jz      .Lpdl_dl_tick_fail
    PL_BALL_ARM(rcx, rdx)
.Lpdl_dl_tick_fail:
    mov     eax, DT_FAIL
    xor     edx, edx
.Lpdl_dl_tick_ret:
    ret
RTX_ENDF(rt_pl_dop_dl_tick)
RTX_FUNC(rt_pl_dop_clause_unify)
    RTX_SUB_RSP(CTX_FRAME)
    CTX_SAVE
    mov     qword ptr [rsp + CTX_BALL], 0
    mov     rdx, rsp
    call rt_pl_dop_clause_unify_c; RTX_GVA_R9
    mov     r12, qword ptr [rsp + CTX_TR]
    mov     rcx, qword ptr [rsp + CTX_BALL]
    RTX_ADD_RSP(CTX_FRAME)
    cmp     al, DT_FAIL
    jne     .Lpcu_ret
    test    rcx, rcx
    jz      .Lpcu_fail
    PL_BALL_ARM(rcx, rdx)
.Lpcu_fail:
    mov     eax, DT_FAIL
    xor     edx, edx
.Lpcu_ret:
    ret
RTX_ENDF(rt_pl_dop_clause_unify)
#define PL_U_DEREF(l) .l##0: mov eax, dword ptr [rdi]; cmp al, DT_PLVAR; je .l##3; cmp al, DT_N; jne .l##9; mov rcx, qword ptr [rdi + 8]; test rcx, rcx; jz .l##9; mov edx, dword ptr [rdi + 4]; cmp edx, 1; jne .l##2; cmp rcx, rdi; je .l##9; mov rdi, rcx; jmp .l##0; \
    .l##2: cmp edx, 2; jne .l##9; mov rcx, qword ptr [rcx]; test rcx, rcx; jz .l##9; mov rdi, rcx; jmp .l##0; \
    .l##3: mov rcx, qword ptr [rdi + 8]; test rcx, rcx; jz .l##9; cmp rcx, rdi; je .l##9; mov rdi, rcx; jmp .l##0; \
    .l##9:
#define PL_U_UNB(c, lyes, lno) cmp al, 0; je lyes; cmp al, DT_FAIL; je lyes; cmp al, DT_PLVAR; je lyes; cmp al, DT_N; jne lno; cmp qword ptr [c + 8], c; jne lno; jmp lyes
#define PL_U_TRAIL(c, l) test r13, r13; jz .l##9; cmp c, rsp; ja .l##4; mov rax, qword ptr [r13]; mov rcx, qword ptr [rax - 32]; test rcx, rcx; jne .l##5; \
    mov rcx, qword ptr [rax - 24]; cmp c, rcx; jb .l##5; mov rax, qword ptr [rip + g_runtime@GOTPCREL]; mov rax, qword ptr [rax]; cmp c, rax; jb .l##9; jmp .l##5; \
    .l##4: mov rax, qword ptr [r13 + 32]; shr rax, 8; cmp c, rax; jb .l##9; \
    .l##5: mov rax, r12; and rax, (-PL_TR_ARENA_MASK - 1); cmp rax, (-PL_TR_ARENA_MASK - PL_TR_ENTRY_BYTES); jae .Lun_refuse; mov rax, qword ptr [c]; mov rcx, qword ptr [c + 8]; mov qword ptr [r12], c; mov qword ptr [r12 + 8], 0; mov qword ptr [r12 + 16], rax; mov qword ptr [r12 + 24], rcx; add r12, 32; \
    .l##9:
#define PL_U_WORK_BYTES 696
#define PL_U_FRAME_BYTES 728
#define PL_U_SPILL mov qword ptr [rsp + PL_U_WORK_BYTES], r8; mov qword ptr [rsp + PL_U_WORK_BYTES + 8], r9; mov qword ptr [rsp + PL_U_WORK_BYTES + 16], r10; mov qword ptr [rsp + PL_U_WORK_BYTES + 24], r11
#define PL_U_FILL mov r8, qword ptr [rsp + PL_U_WORK_BYTES]; mov r9, qword ptr [rsp + PL_U_WORK_BYTES + 8]; mov r10, qword ptr [rsp + PL_U_WORK_BYTES + 16]; mov r11, qword ptr [rsp + PL_U_WORK_BYTES + 24]
#define PL_U_CALL(fn) PL_U_SPILL; call fn; PL_U_FILL
#define PL_U_AG_BASE 64
RTX_FUNC(rt_pl_unify_deep)
    RTX_SUB_RSP(CTX_FRAME)
    CTX_SAVE
    mov     rdx, rsp
    call rt_pl_unify_deep_c; RTX_GVA_R9
    mov     r12, qword ptr [rsp + CTX_TR]
    RTX_ADD_RSP(CTX_FRAME)
    ret
RTX_ENDF(rt_pl_unify_deep)
RTX_FUNC(rtx_pl_unify)
    RTX_SUB_RSP(PL_U_FRAME_BYTES)
    mov     r8, r12
    mov     qword ptr [rsp], 0
    mov     qword ptr [rsp + 8], 0
    mov     qword ptr [rsp + 16], 1
    mov     qword ptr [rsp + 24], 0
    mov     qword ptr [rsp + 32], 0
    mov     qword ptr [rsp + 48], rdi
    mov     qword ptr [rsp + 56], rsi
    lea     r9, [rsp + PL_U_AG_BASE]
.Lun_pair:
    PL_U_DEREF(Lun_da)
    mov     r10, rdi
    mov     rdi, rsi
    PL_U_DEREF(Lun_db)
    mov     r11, rdi
    cmp     r10, r11
    je      .Lun_next
    mov     eax, dword ptr [r10]
    PL_U_UNB(r10, .Lun_a_unb, .Lun_a_bnd)
.Lun_a_unb:
    and     eax, PL_VAR_ATTV_MASK
    cmp     eax, PL_VAR_ATTV_WORD
    je      .Lun_restart
    mov     eax, dword ptr [r11]
    PL_U_UNB(r11, .Lun_both_unb, .Lun_a_unb_b_bnd)
.Lun_a_unb_b_bnd:
    cmp     al, DT_PLREF
    jne     .Lun_oc1
    mov     rax, r12
    and     rax, PL_TR_ARENA_MASK
    cmp     qword ptr [rax + PL_TR_OCHECK_SLOT], 0
    jne     .Lun_restart
.Lun_oc1:
    PL_U_TRAIL(r10, Lun_t1)
    mov     rax, qword ptr [r11]
    mov     rdx, qword ptr [r11 + 8]
    mov     qword ptr [r10], rax
    mov     qword ptr [r10 + 8], rdx
    jmp     .Lun_next
.Lun_both_unb:
    and     eax, PL_VAR_ATTV_MASK
    cmp     eax, PL_VAR_ATTV_WORD
    je      .Lun_restart
    cmp     r10, rsp
    jbe     .Lun_a_heap
    cmp     r11, rsp
    jbe     .Lun_ref_a_to_b
    cmp     r10, r11
    jb      .Lun_ref_a_to_b
    jmp     .Lun_ref_b_to_a
.Lun_a_heap:
    cmp     r11, rsp
    ja      .Lun_ref_b_to_a
    cmp     r10, r11
    ja      .Lun_ref_a_to_b
.Lun_ref_b_to_a:
    xchg    r10, r11
.Lun_ref_a_to_b:
    cmp     byte ptr [r11], DT_PLVAR
    jne     .Lun_norm
    cmp     qword ptr [r11 + 8], r11
    je      .Lun_refstore
.Lun_norm:
    PL_U_TRAIL(r11, Lun_t2)
    mov     rax, qword ptr [r14 + PL_SERIAL_SLOT]
    add     qword ptr [r14 + PL_SERIAL_SLOT], PL_SERIAL_STEP
    mov     qword ptr [r11], rax
    mov     qword ptr [r11 + 8], r11
.Lun_refstore:
    PL_U_TRAIL(r10, Lun_t3)
    mov     qword ptr [r10], DT_PLVAR
    mov     qword ptr [r10 + 8], r11
    jmp     .Lun_next
.Lun_a_bnd:
    mov     eax, dword ptr [r11]
    PL_U_UNB(r11, .Lun_b_unb_a_bnd, .Lun_both_bnd)
.Lun_b_unb_a_bnd:
    and     eax, PL_VAR_ATTV_MASK
    cmp     eax, PL_VAR_ATTV_WORD
    je      .Lun_restart
    cmp     byte ptr [r10], DT_PLREF
    jne     .Lun_oc4
    mov     rax, r12
    and     rax, PL_TR_ARENA_MASK
    cmp     qword ptr [rax + PL_TR_OCHECK_SLOT], 0
    jne     .Lun_restart
.Lun_oc4:
    PL_U_TRAIL(r11, Lun_t4)
    mov     rax, qword ptr [r10]
    mov     rdx, qword ptr [r10 + 8]
    mov     qword ptr [r11], rax
    mov     qword ptr [r11 + 8], rdx
    jmp     .Lun_next
.Lun_both_bnd:
    mov     al, byte ptr [r10]
    cmp     al, DT_PLREF
    jne     .Lun_atomic
    cmp     byte ptr [r11], DT_PLREF
    jne     .Lun_fail
    mov     eax, dword ptr [r10 + 4]
    cmp     eax, dword ptr [r11 + 4]
    jne     .Lun_fail
    cmp     r10, qword ptr [rsp + 24]
    jne     .Lun_brent_step
    cmp     r11, qword ptr [rsp + 32]
    je      .Lun_restart
.Lun_brent_step:
    mov     rax, qword ptr [rsp + 8]
    inc     rax
    mov     qword ptr [rsp + 8], rax
    cmp     rax, qword ptr [rsp + 16]
    jb      .Lun_brent_kept
    mov     qword ptr [rsp + 24], r10
    mov     qword ptr [rsp + 32], r11
    mov     qword ptr [rsp + 8], 0
    shl     qword ptr [rsp + 16], 1
.Lun_brent_kept:
    mov     rcx, qword ptr [rsp]
    test    rcx, rcx
    jnz     .Lun_tab
    PL_U_CALL(rt_pl_functor_entries)
    mov     qword ptr [rsp], rax
    mov     rcx, rax
.Lun_tab:
    mov     eax, dword ptr [r10 + 4]
    mov     eax, dword ptr [rcx + rax*8 + 4]
    test    eax, eax
    jz      .Lun_next
    mov     rdi, qword ptr [r10 + 8]
    mov     rsi, qword ptr [r11 + 8]
    dec     eax
    jz      .Lun_pair
    lea     rcx, [rsp + PL_U_WORK_BYTES - 24]
    cmp     r9, rcx
    ja      .Lun_deep
    lea     rcx, [rdi + 16]
    mov     qword ptr [r9], rcx
    lea     rcx, [rsi + 16]
    mov     qword ptr [r9 + 8], rcx
    mov     qword ptr [r9 + 16], rax
    add     r9, 24
    jmp     .Lun_pair
.Lun_atomic:
    cmp     byte ptr [r11], DT_PLREF
    je      .Lun_fail
    cmp     al, DT_I
    jne     .Lun_atom
    cmp     byte ptr [r11], DT_I
    jne     .Lun_cold
    cmp     dword ptr [r10 + 4], 0
    jne     .Lun_cold
    cmp     dword ptr [r11 + 4], 0
    jne     .Lun_cold
    mov     rax, qword ptr [r10 + 8]
    cmp     rax, qword ptr [r11 + 8]
    jne     .Lun_fail
    jmp     .Lun_next
.Lun_atom:
    cmp     al, DT_PLATOM
    jne     .Lun_cold
    cmp     byte ptr [r11], DT_PLATOM
    jne     .Lun_cold
    mov     rax, qword ptr [r10 + 8]
    cmp     rax, qword ptr [r11 + 8]
    jne     .Lun_fail
    jmp     .Lun_next
.Lun_cold:
    mov     rdi, r10
    mov     rsi, r11
    PL_U_CALL(rt_pl_unify_atomic_cold)
    test    eax, eax
    jz      .Lun_fail
.Lun_next:
    lea     rcx, [rsp + PL_U_AG_BASE]
    cmp     r9, rcx
    jbe     .Lun_ok
    sub     r9, 24
    mov     rdi, qword ptr [r9]
    mov     rsi, qword ptr [r9 + 8]
    mov     rdx, qword ptr [r9 + 16]
    dec     rdx
    jz      .Lun_pair
    lea     rcx, [rdi + 16]
    mov     qword ptr [r9], rcx
    lea     rcx, [rsi + 16]
    mov     qword ptr [r9 + 8], rcx
    mov     qword ptr [r9 + 16], rdx
    add     r9, 24
    jmp     .Lun_pair
.Lun_ok:
    mov     rax, r12
    and     rax, PL_TR_ARENA_MASK
    mov     qword ptr [rax], r12
    RTX_ADD_RSP(PL_U_FRAME_BYTES)
    mov     eax, 1
    RTX_GVA_R9
    ret
.Lun_fail:
    RTX_CFA(PL_U_FRAME_BYTES + 8)
    cmp     r12, r8
    jbe     .Lun_unw_done
    sub     r12, 32
    mov     rdi, qword ptr [r12]
    mov     rax, qword ptr [r12 + 16]
    mov     rdx, qword ptr [r12 + 24]
    mov     qword ptr [rdi], rax
    mov     qword ptr [rdi + 8], rdx
    jmp     .Lun_fail
.Lun_unw_done:
    RTX_CFA(PL_U_FRAME_BYTES + 8)
    mov     rax, r12
    and     rax, PL_TR_ARENA_MASK
    mov     qword ptr [rax], r12
    RTX_ADD_RSP(PL_U_FRAME_BYTES)
    xor     eax, eax
    RTX_GVA_R9
    ret
.Lun_refuse:
    RTX_CFA(PL_U_FRAME_BYTES + 8)
    mov     rdi, r12
    call rt_pl_tr_refuse; RTX_GVA_R9
    RTX_ADD_RSP(PL_U_FRAME_BYTES)
    xor     eax, eax
    RTX_GVA_R9
    ret
.Lun_restart:
    RTX_CFA(PL_U_FRAME_BYTES + 8)
    mov     rdi, qword ptr [rsp + 48]
    mov     rsi, qword ptr [rsp + 56]
    PL_U_CALL(rt_pl_unify_deep)
    test    eax, eax
    jz      .Lun_fail
    jmp     .Lun_ok
.Lun_deep:
    RTX_CFA(PL_U_FRAME_BYTES + 8)
    mov     rdi, r10
    mov     rsi, r11
    PL_U_CALL(rt_pl_unify_deep)
    test    eax, eax
    jz      .Lun_fail
    jmp     .Lun_next
RTX_ENDF(rtx_pl_unify)
#define PL_COLD_BALL_OP(nm) RTX_FUNC(rt_pl_##nm##_cold); RTX_SUB_RSP(24); mov qword ptr [rsp + 8], 0; lea rcx, [rsp + 8]; \
    call rt_pl_##nm##_cold_c; RTX_GVA_R9; mov rcx, qword ptr [rsp + 8]; RTX_ADD_RSP(24); test rcx, rcx; jz 9f; PL_BALL_ARM(rcx, rax); mov eax, DT_FAIL; xor edx, edx; 9: ret; RTX_ENDF(rt_pl_##nm##_cold)
PL_COLD_BALL_OP(ax)
PL_COLD_BALL_OP(cmp)
#define PL_COLD_BALL_V(nm) RTX_FUNC(rt_pl_##nm##_cold); RTX_SUB_RSP(24); mov qword ptr [rsp + 8], 0; lea rdx, [rsp + 8]; \
    call rt_pl_##nm##_cold_c; RTX_GVA_R9; mov rcx, qword ptr [rsp + 8]; RTX_ADD_RSP(24); test rcx, rcx; jz 9f; PL_BALL_ARM(rcx, rax); mov eax, DT_FAIL; xor edx, edx; 9: ret; RTX_ENDF(rt_pl_##nm##_cold)
PL_COLD_BALL_V(is)
#define PL_COLD_GUARD(nm) RTX_FUNC(rt_pl_##nm##_cold); RTX_SUB_RSP(8); call rt_pl_##nm##_cold_c; RTX_GVA_R9; RTX_ADD_RSP(8); \
    test rax, rax; jz 9f; PL_BALL_ARM(rax, rcx); mov eax, DT_FAIL; xor edx, edx; ret; 9: mov eax, DT_I; mov edx, 1; ret; RTX_ENDF(rt_pl_##nm##_cold)
PL_COLD_GUARD(zguard)
PL_COLD_GUARD(anum)
#define PL_COLD_PLAIN(nm) RTX_FUNC(rt_pl_##nm##_cold); RTX_SUB_RSP(8); call rt_pl_##nm##_cold_c; RTX_GVA_R9; RTX_ADD_RSP(8); ret; RTX_ENDF(rt_pl_##nm##_cold)
PL_COLD_PLAIN(type)
PL_COLD_PLAIN(atop)
#define PL_CTX_LEAF(nm) RTX_FUNC(rt_pl_dop_##nm); RTX_SUB_RSP(CTX_FRAME); CTX_SAVE; mov rdx, rsp; \
    call rt_pl_dop_##nm##_c; RTX_GVA_R9; mov r12, qword ptr [rsp + CTX_TR]; RTX_ADD_RSP(CTX_FRAME); ret; RTX_ENDF(rt_pl_dop_##nm)
#define PL_CTX_LEAF_BALL(nm) RTX_FUNC(rt_pl_dop_##nm); RTX_SUB_RSP(CTX_FRAME); CTX_SAVE; \
    mov qword ptr [rsp + CTX_BALL], 0; mov rdx, rsp; call rt_pl_dop_##nm##_c; RTX_GVA_R9; mov r12, qword ptr [rsp + CTX_TR]; mov rcx, qword ptr [rsp + CTX_BALL]; RTX_ADD_RSP(CTX_FRAME); \
    test rcx, rcx; jz 99f; PL_BALL_ARM(rcx, rax); mov eax, DT_FAIL ; xor edx, edx; 99: ret; RTX_ENDF(rt_pl_dop_##nm)
#define PL_CTX_SWITCH_LEAF(nm) RTX_FUNC(rt_pl_dop_##nm); RTX_PUSHS(rbp); mov rbp, rsp; .cfi_def_cfa_register rbp; sub rsp, 48; CTX_SAVE; mov qword ptr [rsp + CTX_BALL], 0; \
    mov rdx, rsp; call rt_pl_dop_##nm##_c; RTX_GVA_R9; mov r12, qword ptr [rsp + CTX_TR]; mov rcx, qword ptr [rsp + CTX_BALL]; mov rsp, rbp; pop rbp; .cfi_def_cfa rsp, 8; .cfi_restore rbp; \
    test rcx, rcx; jz 99f; PL_BALL_ARM(rcx, rax); mov eax, DT_FAIL ; xor edx, edx; 99: ret; RTX_ENDF(rt_pl_dop_##nm)
PL_CTX_SWITCH_LEAF(co_start)
PL_CTX_SWITCH_LEAF(co_resume)
PL_CTX_SWITCH_LEAF(co_yield)
PL_CTX_SWITCH_LEAF(co_unwound)
PL_CTX_SWITCH_LEAF(co_finish)
PL_CTX_SWITCH_LEAF(engine_new)
PL_CTX_SWITCH_LEAF(engine_next)
PL_CTX_SWITCH_LEAF(engine_yield)
PL_CTX_LEAF(engine_destroy)
PL_CTX_LEAF_BALL(engine_self)
PL_CTX_LEAF_BALL(engine_id)
PL_CTX_LEAF_BALL(engine_list)
PL_CTX_LEAF_BALL(engine_reaped)
PL_CTX_LEAF_BALL(engine_stats)
PL_CTX_LEAF(gc_now)
PL_CTX_LEAF(co_mark)
PL_CTX_LEAF(co_b0)
PL_CTX_LEAF(co_inside)
PL_CTX_LEAF_BALL(sub_atom_at)
PL_CTX_LEAF_BALL(atom_concat_at)
PL_CTX_LEAF_BALL(bagof_group_at)
PL_CTX_LEAF_BALL(setof_group_at)
PL_CTX_LEAF_BALL(findall_result)
PL_CTX_LEAF_BALL(findall_result4)
PL_CTX_LEAF_BALL(bagof_result)
PL_CTX_LEAF_BALL(setof_result)
PL_CTX_LEAF_BALL(compare)
PL_CTX_LEAF_BALL(unify_raise)
PL_CTX_LEAF_BALL(functor)
PL_CTX_LEAF_BALL(arg)
PL_CTX_LEAF_BALL(skip_list)
PL_CTX_LEAF_BALL(argv)
PL_CTX_LEAF_BALL(univ)
PL_CTX_LEAF_BALL(copy_term)
PL_CTX_LEAF_BALL(copy_term_nat)
PL_CTX_LEAF_BALL(copy_keep)
PL_CTX_LEAF_BALL(pl_backtrace)
PL_CTX_LEAF_BALL(factorize_term)
PL_CTX_LEAF_BALL(term_variables)
PL_CTX_LEAF_BALL(numbervars3)
PL_CTX_LEAF(numbervars1)
PL_CTX_LEAF_BALL(scc_bnew)
PL_CTX_LEAF_BALL(scc_take)
PL_CTX_LEAF_BALL(attv_set)
PL_CTX_LEAF_BALL(attv_atts)
PL_CTX_LEAF_BALL(attv_take)
PL_CTX_LEAF_BALL(crv_enter)
PL_CTX_LEAF_BALL(crv_adj)
PL_CTX_LEAF_BALL(crv_collect)
PL_CTX_LEAF_BALL(setarg)
PL_CTX_LEAF_BALL(nb_setarg)
PL_CTX_LEAF_BALL(nb_linkarg)
PL_CTX_LEAF_BALL(stamp_date_time)
PL_CTX_LEAF_BALL(date_time_stamp)
PL_CTX_LEAF_BALL(format_time_text)
PL_CTX_LEAF_BALL(create_prolog_flag)
PL_CTX_LEAF_BALL(flag_meta)
PL_CTX_LEAF_BALL(unifiable)
PL_CTX_LEAF_BALL(succ)
PL_CTX_LEAF_BALL(plus)
PL_CTX_LEAF_BALL(wall_us)
PL_CTX_LEAF_BALL(wall_ms)
PL_CTX_LEAF_BALL(cpu_ms)
PL_CTX_LEAF_BALL(sort)
PL_CTX_LEAF_BALL(msort)
PL_CTX_LEAF_BALL(char_type)
PL_CTX_LEAF_BALL(term_string)
PL_CTX_LEAF_BALL(term_to_atom)
PL_CTX_LEAF_BALL(atom_length)
PL_CTX_LEAF_BALL(atom_concat)
PL_CTX_LEAF_BALL(atomic_concat)
PL_CTX_LEAF_BALL(atom_chars)
PL_CTX_LEAF_BALL(atom_codes)
PL_CTX_LEAF_BALL(atom_number)
PL_CTX_LEAF_BALL(atom_string)
PL_CTX_LEAF_BALL(upcase_atom)
PL_CTX_LEAF_BALL(downcase_atom)
PL_CTX_LEAF_BALL(string_concat)
PL_CTX_LEAF_BALL(string_length)
PL_CTX_LEAF_BALL(string_lower)
PL_CTX_LEAF_BALL(string_upper)
PL_CTX_LEAF_BALL(string_to_atom)
PL_CTX_LEAF_BALL(string_codes)
PL_CTX_LEAF_BALL(string_chars)
PL_CTX_LEAF_BALL(number_string)
PL_CTX_LEAF_BALL(atomic_list_concat)
PL_CTX_LEAF_BALL(concat_atom)
PL_CTX_LEAF_BALL(char_code)
PL_CTX_LEAF_BALL(number_codes)
PL_CTX_LEAF_BALL(number_chars)
PL_CTX_LEAF_BALL(name)
PL_CTX_LEAF_BALL(get_char)
PL_CTX_LEAF_BALL(peek_char)
PL_CTX_LEAF_BALL(get_code)
PL_CTX_LEAF_BALL(peek_code)
PL_CTX_LEAF_BALL(get_byte)
PL_CTX_LEAF_BALL(peek_byte)
PL_CTX_LEAF_BALL(unget_char)
PL_CTX_LEAF_BALL(unget_code)
PL_CTX_LEAF_BALL(unget_byte)
PL_CTX_LEAF_BALL(get_edin)
PL_CTX_LEAF_BALL(put_edin)
PL_CTX_LEAF_BALL(telling)
PL_CTX_LEAF_BALL(seeing)
PL_CTX_LEAF(skip)
PL_CTX_LEAF_BALL(read)
PL_CTX_LEAF_BALL(atom_to_term)
PL_CTX_LEAF_BALL(display)
PL_CTX_LEAF_BALL(display_s)
PL_CTX_LEAF(unify_oc)
PL_CTX_LEAF_BALL(aggregate_reduce)
PL_CTX_LEAF_BALL(read_term_opts)
PL_CTX_LEAF_BALL(read_term_opts_s)
PL_CTX_LEAF_BALL(wot_open)
PL_CTX_LEAF_BALL(wot_capture)
PL_CTX_LEAF(wot_discard)
PL_CTX_LEAF(set_prolog_flag_declare)
PL_CTX_LEAF_BALL(read_term_from_atom)
PL_CTX_LEAF_BALL(read_term_from_chars)
PL_CTX_LEAF_BALL(read_term_from_codes)
PL_CTX_LEAF_BALL(read_s)
PL_CTX_LEAF_BALL(get_char_s)
PL_CTX_LEAF_BALL(get_code_s)
PL_CTX_LEAF_BALL(peek_code_s)
PL_CTX_LEAF_BALL(get_byte_s)
PL_CTX_LEAF_BALL(peek_byte_s)
PL_CTX_LEAF_BALL(unget_char_s)
PL_CTX_LEAF_BALL(unget_code_s)
PL_CTX_LEAF_BALL(unget_byte_s)
PL_CTX_LEAF_BALL(put_byte)
PL_CTX_LEAF_BALL(put_byte_s)
PL_CTX_LEAF_BALL(at_end_of_stream_s)
PL_CTX_LEAF_BALL(close)
PL_CTX_LEAF_BALL(put_char_c)
PL_CTX_LEAF_BALL(put_char_c_s)
PL_CTX_LEAF_BALL(tell)
PL_CTX_LEAF_BALL(append1)
PL_CTX_LEAF_BALL(see)
PL_CTX_LEAF_BALL(current_prolog_flag)
PL_CTX_LEAF_BALL(set_prolog_flag)
PL_CTX_LEAF_BALL(peek_char_s)
PL_CTX_LEAF_BALL(current_output)
PL_CTX_LEAF_BALL(current_input)
PL_CTX_LEAF_BALL(open)
PL_CTX_LEAF_BALL(open4)
PL_CTX_LEAF_BALL(keysort)
PL_CTX_LEAF_BALL(gnu_sort1)
PL_CTX_LEAF_BALL(gnu_msort1)
PL_CTX_LEAF_BALL(gnu_keysort1)
PL_CTX_LEAF_BALL(gnu_line_count)
PL_CTX_LEAF_BALL(gnu_line_position)
PL_CTX_LEAF_BALL(gnu_character_count)
PL_CTX_LEAF_BALL(put_token)
PL_CTX_LEAF_BALL(sha_hash)
PL_CTX_LEAF_BALL(gnu_stream_line_column)
PL_CTX_LEAF_BALL(gnu_last_read_start)
PL_CTX_LEAF_BALL(gnu_absolute_file_name)
PL_CTX_LEAF_BALL(gnu_prolog_file_name)
PL_CTX_LEAF_BALL(gnu_working_directory)
PL_CTX_LEAF_BALL(gnu_change_directory)
PL_CTX_LEAF_BALL(gnu_make_directory)
PL_CTX_LEAF_BALL(gnu_os)
PL_CTX_LEAF_BALL(compound0)
PL_CTX_LEAF_BALL(set_stream)
PL_CTX_LEAF_BALL(gnu_delete_file)
PL_CTX_LEAF_BALL(gnu_file_exists)
PL_CTX_LEAF_BALL(gnu_directory_files)
PL_CTX_LEAF_BALL(gnu_environ_list)
PL_CTX_LEAF_BALL(gnu_file_props)
PL_CTX_LEAF_BALL(gnu_term_hash)
PL_CTX_LEAF_BALL(gnu_prolog_pid)
PL_CTX_LEAF_BALL(pipe)
PL_CTX_LEAF_BALL(open_null_stream)
PL_CTX_LEAF(gnu_builtin)
PL_CTX_LEAF_BALL(set_stream_position)
PL_CTX_LEAF_BALL(format)
PL_CTX_LEAF_BALL(format3)
PL_CTX_LEAF_BALL(write_term)
PL_CTX_LEAF_BALL(write_term_s)
PL_CTX_LEAF_BALL(print)
PL_CTX_LEAF_BALL(print_s)
PL_CTX_LEAF_BALL(write_sb)
PL_CTX_LEAF_BALL(writeq_sb)
PL_CTX_LEAF_BALL(write_canonical_sb)
PL_CTX_LEAF_BALL(writeln_sb)
PL_CTX_LEAF_BALL(nl_sb)
PL_CTX_LEAF_BALL(tab_sb)
PL_CTX_LEAF_BALL(put_char_sb)
PL_CTX_LEAF_BALL(put_code_sb)
PL_CTX_LEAF_BALL(flush_output_sb)
PL_CTX_LEAF_BALL(pl_op_count)
PL_CTX_LEAF_BALL(cutcall)
PL_CTX_LEAF_BALL(shc)
PL_CTX_LEAF_BALL(op)
PL_CTX_LEAF_BALL(pl_op_check)
PL_CTX_LEAF_BALL(heap_guard)
PL_CTX_LEAF_BALL(pl_sp_check)
PL_CTX_LEAF_BALL(pl_ioarg)
PL_CTX_LEAF_BALL(pl_op_nth)
PL_CTX_LEAF_BALL(pl_sp_count)
PL_CTX_LEAF_BALL(pl_sp_nth)
PL_CTX_LEAF_BALL(pl_cs_count)
PL_CTX_LEAF_BALL(pl_cs_nth)
PL_CTX_LEAF_BALL(pl_cp_guard)
PL_CTX_LEAF_BALL(pl_pp_guard)
PL_CTX_LEAF_BALL(halt)
#define PL_ROOT_LEAF(nm) RTX_FUNC(rt_pl_dop_##nm); mov rdx, r14; RTX_SUB_RSP(8); call rt_pl_dop_##nm##_c; RTX_ADD_RSP(8); RTX_GVA_R9; ret; RTX_ENDF(rt_pl_dop_##nm)
PL_ROOT_LEAF(db_assertz)
PL_ROOT_LEAF(db_asserta)
PL_ROOT_LEAF(db_erase)
PL_ROOT_LEAF(db_abolish)
PL_ROOT_LEAF(db_retractall)
PL_ROOT_LEAF(db_nonempty)
PL_ROOT_LEAF(db_seed_once)
PL_ROOT_LEAF(nb_setval)
PL_ROOT_LEAF(nb_delete)
PL_ROOT_LEAF(db_assertz_t)
PL_ROOT_LEAF(db_asserta_t)
PL_ROOT_LEAF(db_consult_t)
PL_ROOT_LEAF(db_consult_done)
PL_ROOT_LEAF(db_erase_t)
PL_ROOT_LEAF(db_erase_ref)
PL_ROOT_LEAF(db_n_r)
PL_ROOT_LEAF(db_at_r)
PL_ROOT_LEAF(db_ref_r)
PL_ROOT_LEAF(db_abolish_t)
PL_ROOT_LEAF(db_retractall_t)
PL_ROOT_LEAF(db_store_k)
PL_ROOT_LEAF(db_copy)
#define PL_ROOTCTX_LEAF(nm) RTX_FUNC(rt_pl_dop_##nm); RTX_SUB_RSP(CTX_FRAME); CTX_SAVE; mov rdx, rsp; mov rcx, r14; \
    call rt_pl_dop_##nm##_c; RTX_GVA_R9; mov r12, qword ptr [rsp + CTX_TR]; RTX_ADD_RSP(CTX_FRAME); ret; RTX_ENDF(rt_pl_dop_##nm)
PL_ROOTCTX_LEAF(nb_getval)
PL_ROOTCTX_LEAF(b_setval)
PL_ROOTCTX_LEAF(pl_cp_count)
PL_ROOTCTX_LEAF(mod_get)
PL_ROOTCTX_LEAF(mod_set)
PL_ROOTCTX_LEAF(pl_cp_nth)
PL_ROOTCTX_LEAF(pl_pp_count)
PL_ROOTCTX_LEAF(pl_pp_nth)
PL_ROOTCTX_LEAF(db_assertz_r)
PL_ROOTCTX_LEAF(db_asserta_r)
PL_ROOTCTX_LEAF(trie_new)
PL_ROOTCTX_LEAF(trie_alive)
PL_ROOTCTX_LEAF(trie_destroy)
PL_ROOTCTX_LEAF(trie_insert)
PL_ROOTCTX_LEAF(trie_lookup)
PL_ROOTCTX_LEAF(trie_delete)
PL_ROOTCTX_LEAF(trie_entries)
PL_ROOTCTX_LEAF(trie_entry_key)
PL_ROOTCTX_LEAF(trie_slice)
PL_ROOTCTX_LEAF(trie_count)
PL_ROOTCTX_LEAF(trie_meta_set)
PL_ROOTCTX_LEAF(trie_meta_get)
PL_ROOTCTX_LEAF(tbl_vt)
PL_ROOTCTX_LEAF(tbl_open)
PL_ROOTCTX_LEAF(tbh_close)
PL_ROOTCTX_LEAF(tbh_leading)
PL_ROOTCTX_LEAF(tbh_get)
PL_ROOTCTX_LEAF(tbh_own)
PL_ROOTCTX_LEAF(tbh_set)
PL_ROOTCTX_LEAF(tbh_bset)
PL_ROOTCTX_LEAF(tbl_dlget)
PL_ROOTCTX_LEAF(tbl_dlset)
PL_ROOTCTX_LEAF(tx_db)
PL_ROOTCTX_LEAF(mod_ext)
RTX_FUNC(rt_pl_dop_db_alive)
    RTX_SUB_RSP(8)
    mov     rdx, r14
    call rt_pl_dop_db_alive_c; RTX_GVA_R9
    RTX_ADD_RSP(8)
    test    rax, rax
    jz      .Lda_ok
    PL_BALL_ARM(rax, rcx)
    mov     eax, DT_FAIL
    xor     edx, edx
    ret
.Lda_ok:
    mov     eax, DT_I
    mov     edx, 1
    ret
RTX_ENDF(rt_pl_dop_db_alive)
RTX_FUNC(rt_pl_dop_db_t_guard)
    RTX_SUB_RSP(8)
    mov     rdx, r14
    call rt_pl_dop_db_t_guard_c; RTX_GVA_R9
    RTX_ADD_RSP(8)
    test    rax, rax
    jz      .Ldtg_ok
    PL_BALL_ARM(rax, rcx)
    mov     eax, DT_FAIL
    xor     edx, edx
    ret
.Ldtg_ok:
    mov     eax, DT_I
    mov     edx, 1
    ret
RTX_ENDF(rt_pl_dop_db_t_guard)
RTX_FUNC(rt_pl_dop_goal_guard)
    RTX_SUB_RSP(8)
    call rt_pl_dop_goal_guard_c; RTX_GVA_R9
    RTX_ADD_RSP(8)
    test    rax, rax
    jz      .Lgg_ok
    PL_BALL_ARM(rax, rcx)
    mov     eax, DT_FAIL
    xor     edx, edx
    ret
.Lgg_ok:
    mov     eax, DT_I
    mov     edx, 1
    ret
RTX_ENDF(rt_pl_dop_goal_guard)
RTX_FUNC(rt_pl_dop_list_guard)
    RTX_SUB_RSP(8)
    call rt_pl_dop_list_guard_c; RTX_GVA_R9
    RTX_ADD_RSP(8)
    test    rax, rax
    jz      .Llg_ok
    PL_BALL_ARM(rax, rcx)
    mov     eax, DT_FAIL
    xor     edx, edx
    ret
.Llg_ok:
    mov     eax, DT_I
    mov     edx, 1
    ret
RTX_ENDF(rt_pl_dop_list_guard)
PL_ROOT_LEAF(pl_declared)
PL_ROOT_LEAF(pl_dynamic)
RTX_FUNC(rt_pl_dop_char_guard)
    RTX_SUB_RSP(8)
    call rt_pl_dop_char_guard_c; RTX_GVA_R9
    RTX_ADD_RSP(8)
    test    rax, rax
    jz      .Lcg_ok
    PL_BALL_ARM(rax, rcx)
    mov     eax, DT_FAIL
    xor     edx, edx
    ret
.Lcg_ok:
    mov     eax, DT_I
    mov     edx, 1
    ret
RTX_ENDF(rt_pl_dop_char_guard)
RTX_FUNC(rt_pl_dop_nb_getval_guard)
    RTX_SUB_RSP(8)
    mov     rdx, r14
    call rt_pl_dop_nb_getval_guard_c; RTX_GVA_R9
    RTX_ADD_RSP(8)
    test    rax, rax
    jz      .Lnbgg_ok
    PL_BALL_ARM(rax, rcx)
    mov     eax, DT_FAIL
    xor     edx, edx
    ret
.Lnbgg_ok:
    mov     eax, DT_I
    mov     edx, 1
    ret
RTX_ENDF(rt_pl_dop_nb_getval_guard)
RTX_FUNC(rt_pl_dop_ax_eguard)
    RTX_SUB_RSP(8)
    call rt_pl_dop_ax_eguard_c; RTX_GVA_R9
    RTX_ADD_RSP(8)
    test    rax, rax
    jz      .Leg_ok
    PL_BALL_ARM(rax, rcx)
    mov     eax, DT_FAIL
    xor     edx, edx
    ret
.Leg_ok:
    mov     eax, DT_I
    mov     edx, 1
    ret
RTX_ENDF(rt_pl_dop_ax_eguard)
RTX_FUNC(rt_pl_dop_between_guard)
    RTX_SUB_RSP(8)
    call rt_pl_dop_between_guard_c; RTX_GVA_R9
    RTX_ADD_RSP(8)
    test    rax, rax
    jz      .Lbg_ok
    PL_BALL_ARM(rax, rcx)
    mov     eax, DT_FAIL
    xor     edx, edx
    ret
.Lbg_ok:
    mov     eax, DT_I
    mov     edx, 1
    ret
RTX_ENDF(rt_pl_dop_between_guard)
RTX_FUNC(rt_pl_dop_stream_guard)
    RTX_SUB_RSP(8)
    call rt_pl_dop_stream_guard_c; RTX_GVA_R9
    RTX_ADD_RSP(8)
    test    rax, rax
    jz      .Lsg_ok
    PL_BALL_ARM(rax, rcx)
    mov     eax, DT_FAIL
    xor     edx, edx
    ret
.Lsg_ok:
    mov     eax, DT_I
    mov     edx, 1
    ret
RTX_ENDF(rt_pl_dop_stream_guard)
RTX_FUNC(rt_pl_dop_curstream_guard)
    RTX_SUB_RSP(8)
    call rt_pl_dop_curstream_guard_c; RTX_GVA_R9
    RTX_ADD_RSP(8)
    test    rax, rax
    jz      .Lcsg_ok
    PL_BALL_ARM(rax, rcx)
    mov     eax, DT_FAIL
    xor     edx, edx
    ret
.Lcsg_ok:
    mov     eax, DT_I
    mov     edx, 1
    ret
RTX_ENDF(rt_pl_dop_curstream_guard)
RTX_FUNC(rt_pl_db_reclaimed_entry)
    RTX_PUSHS(rbp)
    mov     rbp, rsp
    .cfi_def_cfa_register rbp
    and     rsp, -16
    lea     rdi, [rip + .Lpl_db_reclaimed_msg]
    call    rt_bomb@PLT
    ud2
RTX_ENDF(rt_pl_db_reclaimed_entry)
.section .rodata
.Lpl_db_reclaimed_msg:
    .string "a reclaimed run-time Prolog fragment was entered -- its clause was erased and the assertz reclaim found no live activation that could still reach it (unification.c pl_db_reclaim)"
#define PL_TV_LINK (DT_PLREF + 2)
#define PL_TV_SEEN (DT_PLREF + 3)
#define PL_TV_VCOPY (DT_PLREF + 5)
#define PL_TV_AG_BYTES 2048
#define PL_TV_DEREF(l, var) \
.l##_cell: movzx eax, byte ptr [rbx]; cmp al, DT_PLVAR; jne .l##_dn; mov rcx, qword ptr [rbx + 8]; cmp rcx, rbx; je var; mov rbx, rcx; jmp .l##_cell; \
.l##_dn: cmp al, DT_N; jne .l##_link; mov ecx, dword ptr [rbx + 4]; mov rdx, qword ptr [rbx + 8]; test rdx, rdx; jz .l##_have; cmp ecx, 1; je .l##_go; cmp ecx, 2; jne .l##_have; \
    mov rdx, qword ptr [rdx]; test rdx, rdx; jz .l##_have; \
.l##_go: mov rbx, rdx; jmp .l##_cell; \
.l##_link: cmp al, PL_TV_LINK; jne .l##_have; \
.l##_lgo: mov rbx, qword ptr [rbx + 8]; movzx eax, byte ptr [rbx]; cmp al, PL_TV_LINK; je .l##_lgo; \
.l##_have:
#define PL_TV_NEXT(l, done) \
.l##_next: cmp r12, r15; je done; mov rbx, qword ptr [r12 - 16]; lea rdx, [rbx + 16]; mov qword ptr [r12 - 16], rdx; dec qword ptr [r12 - 8]; jnz .l##_cell; sub r12, 16; jmp .l##_cell;
RTX_FUNC(rt_pl_tv1_walk_a)
    RTX_PUSHS(rbx)
    RTX_PUSHS(rbp)
    RTX_PUSHS(r12)
    RTX_PUSHS(r13)
    RTX_PUSHS(r14)
    RTX_PUSHS(r15)
    RTX_SUB_RSP(PL_TV_AG_BYTES + 16)
    mov     qword ptr [rsp + PL_TV_AG_BYTES], rdi
    mov     rbp, rdx
    mov     r14, rsi
    mov     r15, rsp
    mov     r12, rsp
    xor     r13d, r13d
    mov     rbx, rdi
    PL_TV_DEREF(Ltv, .Ltv_var)
    cmp     al, PL_TV_SEEN
    je      .Ltv_next
    cmp     al, PL_TV_VCOPY
    je      .Ltv_next
    cmp     al, DT_PLVAR
    je      .Ltv_var
    cmp     al, DT_PLREF
    je      .Ltv_cmp
    cmp     al, DT_SNUL
    je      .Ltv_other
    cmp     al, DT_FAIL
    je      .Ltv_other
    jmp     .Ltv_next
.Ltv_cmp:
    mov     eax, dword ptr [rbx + 4]
    cmp     rax, rbp
    jae     .Ltv_next
    mov     ecx, dword ptr [r14 + rax*8 + 4]
    test    ecx, ecx
    jle     .Ltv_next
    lea     rax, [r15 + PL_TV_AG_BYTES]
    cmp     r12, rax
    jae     .Ltv_full
    mov     rdx, qword ptr [rbx + 8]
    mov     qword ptr [r12], rdx
    mov     qword ptr [r12 + 8], rcx
    add     r12, 16
    mov     byte ptr [rbx], PL_TV_SEEN
    jmp     .Ltv_next
.Ltv_var:
    mov     byte ptr [rbx], PL_TV_VCOPY
    mov     qword ptr [rbx + 8], r13
    mov     r13, rbx
    PL_TV_NEXT(Ltv, .Ltv_done)
.Ltv_done:
    xor     edi, edi
    jmp     .Ltu_start
.Ltv_other:
    mov     edi, 1
    jmp     .Ltu_start
.Ltv_full:
    mov     edi, 2
.Ltu_start:
    mov     qword ptr [rsp + PL_TV_AG_BYTES + 8], rdi
    mov     r12, r15
    mov     rbx, qword ptr [rsp + PL_TV_AG_BYTES]
    PL_TV_DEREF(Ltu, .Ltu_next)
    cmp     al, PL_TV_SEEN
    jne     .Ltu_next
    mov     byte ptr [rbx], DT_PLREF
    mov     eax, dword ptr [rbx + 4]
    mov     ecx, dword ptr [r14 + rax*8 + 4]
    lea     rax, [r15 + PL_TV_AG_BYTES]
    cmp     r12, rax
    jae     .Ltu_lost
    mov     rdx, qword ptr [rbx + 8]
    mov     qword ptr [r12], rdx
    mov     qword ptr [r12 + 8], rcx
    add     r12, 16
    PL_TV_NEXT(Ltu, .Ltu_done)
.Ltu_lost:
    ud2
.Ltu_done:
    mov     rax, r13
    mov     rdx, qword ptr [rsp + PL_TV_AG_BYTES + 8]
    RTX_ADD_RSP(PL_TV_AG_BYTES + 16)
    RTX_POPS(r15)
    RTX_POPS(r14)
    RTX_POPS(r13)
    RTX_POPS(r12)
    RTX_POPS(rbp)
    RTX_POPS(rbx)
    ret
RTX_ENDF(rt_pl_tv1_walk_a)
#define PL_OC_DEREF(l, ncell, hopx) \
.l##_cell: xor esi, esi; \
.l##_hop: movzx eax, byte ptr [rbx]; cmp al, DT_N; je ncell; cmp al, DT_PLVAR; jne .l##_link; mov rcx, qword ptr [rbx + 8]; test rcx, rcx; jz .l##_have; cmp rcx, rbx; je .l##_have; \
    inc rsi; bt rsi, 28; jc hopx; mov rbx, rcx; jmp .l##_hop; \
.l##_link: cmp al, PL_TV_LINK; jne .l##_have; \
.l##_lgo: mov rbx, qword ptr [rbx + 8]; movzx eax, byte ptr [rbx]; cmp al, PL_TV_LINK; je .l##_lgo; \
.l##_have:
RTX_FUNC(rt_pl_occurs_a)
    RTX_PUSHS(rbx)
    RTX_PUSHS(rbp)
    RTX_PUSHS(r12)
    RTX_PUSHS(r13)
    RTX_PUSHS(r14)
    RTX_PUSHS(r15)
    RTX_SUB_RSP(PL_TV_AG_BYTES + 16)
    mov     qword ptr [rsp + PL_TV_AG_BYTES], rsi
    mov     r13, rdi
    mov     r14, rdx
    mov     rbp, rcx
    mov     r15, rsp
    mov     r12, rsp
    mov     rbx, rsi
    PL_OC_DEREF(Loc, .Loc_full, .Loc_full)
    cmp     rbx, r13
    je      .Loc_found
    cmp     al, PL_TV_SEEN
    je      .Loc_next
    cmp     al, DT_PLREF
    jne     .Loc_next
    mov     eax, dword ptr [rbx + 4]
    cmp     rax, rbp
    jae     .Loc_next
    mov     ecx, dword ptr [r14 + rax*8 + 4]
    test    ecx, ecx
    jle     .Loc_next
    lea     rax, [r15 + PL_TV_AG_BYTES]
    cmp     r12, rax
    jae     .Loc_full
    mov     rdx, qword ptr [rbx + 8]
    mov     qword ptr [r12], rdx
    mov     qword ptr [r12 + 8], rcx
    add     r12, 16
    mov     byte ptr [rbx], PL_TV_SEEN
    PL_TV_NEXT(Loc, .Loc_none)
.Loc_none:
    xor     edi, edi
    jmp     .Lou_start
.Loc_found:
    mov     edi, 1
    jmp     .Lou_start
.Loc_full:
    mov     edi, 2
.Lou_start:
    mov     qword ptr [rsp + PL_TV_AG_BYTES + 8], rdi
    mov     r12, r15
    mov     rbx, qword ptr [rsp + PL_TV_AG_BYTES]
    PL_OC_DEREF(Lou, .Lou_next, .Lou_next)
    cmp     al, PL_TV_SEEN
    jne     .Lou_next
    mov     byte ptr [rbx], DT_PLREF
    mov     eax, dword ptr [rbx + 4]
    mov     ecx, dword ptr [r14 + rax*8 + 4]
    lea     rax, [r15 + PL_TV_AG_BYTES]
    cmp     r12, rax
    jae     .Lou_lost
    mov     rdx, qword ptr [rbx + 8]
    mov     qword ptr [r12], rdx
    mov     qword ptr [r12 + 8], rcx
    add     r12, 16
    PL_TV_NEXT(Lou, .Lou_done)
.Lou_lost:
    ud2
.Lou_done:
    mov     rax, qword ptr [rsp + PL_TV_AG_BYTES + 8]
    RTX_ADD_RSP(PL_TV_AG_BYTES + 16)
    RTX_POPS(r15)
    RTX_POPS(r14)
    RTX_POPS(r13)
    RTX_POPS(r12)
    RTX_POPS(rbp)
    RTX_POPS(rbx)
    ret
RTX_ENDF(rt_pl_occurs_a)
RTX_FUNC(rt_pl_db_reg_find_na)
    RTX_PUSHS(rbp)
    RTX_PUSHS(rbx)
    RTX_SUB_RSP(16)
    mov     eax, edx
    mov     r8d, edx
    test    eax, eax
    jns     .Ldbf_pos
    neg     eax
.Ldbf_pos:
    lea     rbx, [rsp + 12]
    mov     rbp, rbx
    mov     r10d, 10
.Ldbf_dig:
    xor     edx, edx
    div     r10d
    add     dl, 48
    dec     rbp
    mov     byte ptr [rbp], dl
    test    eax, eax
    jnz     .Ldbf_dig
    test    r8d, r8d
    jns     .Ldbf_hash
    dec     rbp
    mov     byte ptr [rbp], 45
.Ldbf_hash:
    movabs  rax, 0xCBF29CE484222325
    movabs  r11, 0x100000001B3
    mov     r10, rsi
.Ldbf_nm:
    movzx   ecx, byte ptr [r10]
    test    ecx, ecx
    jz      .Ldbf_slash
    xor     rax, rcx
    imul    rax, r11
    inc     r10
    jmp     .Ldbf_nm
.Ldbf_slash:
    xor     rax, 47
    imul    rax, r11
    mov     r10, rbp
.Ldbf_ad:
    cmp     r10, rbx
    jae     .Ldbf_probe
    movzx   ecx, byte ptr [r10]
    xor     rax, rcx
    imul    rax, r11
    inc     r10
    jmp     .Ldbf_ad
.Ldbf_probe:
    mov     r8d, dword ptr [rdi + 64]
    dec     r8d
    and     eax, r8d
    mov     rdx, qword ptr [rdi + 56]
.Ldbf_slot:
    mov     ecx, dword ptr [rdx + rax*4]
    test    ecx, ecx
    jz      .Ldbf_none
    dec     ecx
    lea     rcx, [rcx + rcx*2]
    shl     rcx, 4
    add     rcx, qword ptr [rdi]
    mov     r11, qword ptr [rcx]
    mov     r10, rsi
.Ldbf_cn:
    movzx   r8d, byte ptr [r10]
    test    r8d, r8d
    jz      .Ldbf_cs
    cmp     r8b, byte ptr [r11]
    jne     .Ldbf_next
    inc     r10
    inc     r11
    jmp     .Ldbf_cn
.Ldbf_cs:
    cmp     byte ptr [r11], 47
    jne     .Ldbf_next
    inc     r11
    mov     r10, rbp
.Ldbf_cd:
    cmp     r10, rbx
    jae     .Ldbf_ce
    movzx   r8d, byte ptr [r10]
    cmp     r8b, byte ptr [r11]
    jne     .Ldbf_next
    inc     r10
    inc     r11
    jmp     .Ldbf_cd
.Ldbf_ce:
    cmp     byte ptr [r11], 0
    jne     .Ldbf_next
    mov     rax, rcx
    jmp     .Ldbf_ret
.Ldbf_next:
    mov     r8d, dword ptr [rdi + 64]
    dec     r8d
    inc     eax
    and     eax, r8d
    jmp     .Ldbf_slot
.Ldbf_none:
    xor     eax, eax
.Ldbf_ret:
    RTX_ADD_RSP(16)
    RTX_POPS(rbx)
    RTX_POPS(rbp)
    ret
RTX_ENDF(rt_pl_db_reg_find_na)
.section .note.GNU-stack,"",@progbits
