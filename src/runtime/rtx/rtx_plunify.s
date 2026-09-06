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
RTX_FUNC(rt_pl_cut_barrier)
    mov     r13, qword ptr [rdi + 24]
    ret
RTX_ENDF(rt_pl_cut_barrier)
RTX_FUNC(rt_pl_throw_raise)
    sub     rsp, 8
    call    rt_pl_ball_make
    mov     r15, rax
    add     rsp, 8
    mov     eax, DT_FAIL | (MOD_OP_RT_PL_THROW_RAISE << 8)
    xor     edx, edx
    ret
RTX_ENDF(rt_pl_throw_raise)
RTX_FUNC(rt_pl_exist_raise)
    sub     rsp, 8
    call    rt_pl_ball_existence
    mov     r15, rax
    add     rsp, 8
    mov     eax, DT_FAIL | (MOD_OP_RT_PL_EXIST_RAISE << 8)
    xor     edx, edx
    ret
RTX_ENDF(rt_pl_exist_raise)
RTX_FUNC(rt_pl_catch_handle)
    test    r15, r15
    jz      .Lch_fail
    sub     rsp, CTX_FRAME
    mov     qword ptr [rsp + CTX_TR], r12
    mov     qword ptr [rsp + CTX_B], r13
    mov     rdx, r15
    mov     rcx, rsp
    call    rt_pl_catch_handle_c
    mov     r12, qword ptr [rsp + CTX_TR]
    add     rsp, CTX_FRAME
    test    eax, eax
    jz      .Lch_fail
    xor     r15d, r15d
    mov     eax, DT_I
    mov     edx, 1
    ret
.Lch_fail:
    mov     eax, DT_FAIL | (MOD_OP_RT_PL_CATCH_HANDLE << 8)
    xor     edx, edx
    ret
RTX_ENDF(rt_pl_catch_handle)
RTX_FUNC(rt_pl_ball_take)
    mov     rax, r15
    xor     r15d, r15d
    ret
RTX_ENDF(rt_pl_ball_take)
RTX_FUNC(rt_pl_disj_open)
    mov     qword ptr [rdi + 32], rsi
    test    r13, r13
    jz      .Ldo_take
    cmp     r13, rdi
    jb      .Ldo_done
.Ldo_take:
    mov     r13, rdi
.Ldo_done:
    ret
RTX_ENDF(rt_pl_disj_open)
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
#define PL_CTX_LEAF(nm) RTX_FUNC(rt_pl_dop_##nm); sub rsp, CTX_FRAME; mov qword ptr [rsp + CTX_TR], r12; mov qword ptr [rsp + CTX_B], r13; mov rdx, rsp; \
    call rt_pl_dop_##nm##_c; mov r12, qword ptr [rsp + CTX_TR]; add rsp, CTX_FRAME; ret; RTX_ENDF(rt_pl_dop_##nm)
PL_CTX_LEAF(sub_atom_at)
PL_CTX_LEAF(findall_result)
PL_CTX_LEAF(bagof_result)
PL_CTX_LEAF(setof_result)
PL_CTX_LEAF(compare)
PL_CTX_LEAF(functor)
PL_CTX_LEAF(arg)
PL_CTX_LEAF(univ)
PL_CTX_LEAF(copy_term)
PL_CTX_LEAF(numbervars3)
PL_CTX_LEAF(numbervars1)
PL_CTX_LEAF(succ)
PL_CTX_LEAF(plus)
PL_CTX_LEAF(sort)
PL_CTX_LEAF(msort)
PL_CTX_LEAF(char_type)
PL_CTX_LEAF(term_string)
PL_CTX_LEAF(atom_length)
PL_CTX_LEAF(atom_concat)
PL_CTX_LEAF(atom_chars)
PL_CTX_LEAF(atom_codes)
PL_CTX_LEAF(atom_number)
PL_CTX_LEAF(atom_string)
PL_CTX_LEAF(upcase_atom)
PL_CTX_LEAF(downcase_atom)
PL_CTX_LEAF(string_concat)
PL_CTX_LEAF(string_length)
PL_CTX_LEAF(string_lower)
PL_CTX_LEAF(string_upper)
PL_CTX_LEAF(string_to_atom)
PL_CTX_LEAF(number_string)
PL_CTX_LEAF(atomic_list_concat)
PL_CTX_LEAF(concat_atom)
PL_CTX_LEAF(char_code)
PL_CTX_LEAF(number_codes)
PL_CTX_LEAF(number_chars)
PL_CTX_LEAF(name)
PL_CTX_LEAF(get_char)
PL_CTX_LEAF(peek_char)
PL_CTX_LEAF(read)
PL_CTX_LEAF(atom_to_term)
PL_CTX_LEAF(read_term_from_atom)
PL_CTX_LEAF(read_term_from_chars)
PL_CTX_LEAF(read_term_from_codes)
PL_CTX_LEAF(read_s)
PL_CTX_LEAF(get_char_s)
PL_CTX_LEAF(peek_char_s)
PL_CTX_LEAF(current_output)
PL_CTX_LEAF(current_input)
PL_CTX_LEAF(open)
PL_CTX_LEAF(open4)
PL_CTX_LEAF(keysort)
PL_CTX_LEAF(format3)
#define PL_ROOT_LEAF(nm) RTX_FUNC(rt_pl_dop_##nm); mov rdx, r14; jmp rt_pl_dop_##nm##_c; RTX_ENDF(rt_pl_dop_##nm)
PL_ROOT_LEAF(db_assertz)
PL_ROOT_LEAF(db_asserta)
PL_ROOT_LEAF(db_erase)
PL_ROOT_LEAF(db_abolish)
PL_ROOT_LEAF(db_retractall)
PL_ROOT_LEAF(nb_setval)
#define PL_ROOTCTX_LEAF(nm) RTX_FUNC(rt_pl_dop_##nm); sub rsp, CTX_FRAME; mov qword ptr [rsp + CTX_TR], r12; mov qword ptr [rsp + CTX_B], r13; mov rdx, rsp; mov rcx, r14; \
    call rt_pl_dop_##nm##_c; mov r12, qword ptr [rsp + CTX_TR]; add rsp, CTX_FRAME; ret; RTX_ENDF(rt_pl_dop_##nm)
PL_ROOTCTX_LEAF(nb_getval)
RTX_FUNC(rt_pl_dop_ax_zguard)
    sub     rsp, 8
    call    rt_pl_dop_ax_zguard_c
    add     rsp, 8
    test    rax, rax
    jz      .Lzg_ok
    mov     r15, rax
    mov     eax, DT_FAIL | (MOD_OP_RT_PL_AX_ZGUARD << 8)
    xor     edx, edx
    ret
.Lzg_ok:
    mov     eax, DT_I
    mov     edx, 1
    ret
RTX_ENDF(rt_pl_dop_ax_zguard)
RTX_FUNC(rt_pl_dop_db_alive)
    sub     rsp, 8
    mov     rdx, r14
    call    rt_pl_dop_db_alive_c
    add     rsp, 8
    test    rax, rax
    jz      .Lda_ok
    mov     r15, rax
    mov     eax, DT_FAIL | (MOD_OP_RT_PL_DB_ALIVE << 8)
    xor     edx, edx
    ret
.Lda_ok:
    mov     eax, DT_I
    mov     edx, 1
    ret
RTX_ENDF(rt_pl_dop_db_alive)
RTX_FUNC(rt_pl_dop_char_guard)
    sub     rsp, 8
    call    rt_pl_dop_char_guard_c
    add     rsp, 8
    test    rax, rax
    jz      .Lcg_ok
    mov     r15, rax
    mov     eax, DT_FAIL | (MOD_OP_RT_PL_CHAR_GUARD << 8)
    xor     edx, edx
    ret
.Lcg_ok:
    mov     eax, DT_I
    mov     edx, 1
    ret
RTX_ENDF(rt_pl_dop_char_guard)
RTX_FUNC(rt_pl_dop_nb_getval_guard)
    sub     rsp, 8
    mov     rdx, r14
    call    rt_pl_dop_nb_getval_guard_c
    add     rsp, 8
    test    rax, rax
    jz      .Lnbgg_ok
    mov     r15, rax
    mov     eax, DT_FAIL | (MOD_OP_RT_PL_NB_GETVAL_GUARD << 8)
    xor     edx, edx
    ret
.Lnbgg_ok:
    mov     eax, DT_I
    mov     edx, 1
    ret
RTX_ENDF(rt_pl_dop_nb_getval_guard)
RTX_FUNC(rt_pl_dop_anum_guard2)
    sub     rsp, 8
    call    rt_pl_dop_anum_guard2_c
    add     rsp, 8
    test    rax, rax
    jz      .Lag2_ok
    mov     r15, rax
    mov     eax, DT_FAIL | (MOD_OP_RT_PL_ANUM_GUARD2 << 8)
    xor     edx, edx
    ret
.Lag2_ok:
    mov     eax, DT_I
    mov     edx, 1
    ret
RTX_ENDF(rt_pl_dop_anum_guard2)
RTX_FUNC(rt_pl_dop_anum_guard3)
    sub     rsp, 8
    call    rt_pl_dop_anum_guard3_c
    add     rsp, 8
    test    rax, rax
    jz      .Lag3_ok
    mov     r15, rax
    mov     eax, DT_FAIL | (MOD_OP_RT_PL_ANUM_GUARD3 << 8)
    xor     edx, edx
    ret
.Lag3_ok:
    mov     eax, DT_I
    mov     edx, 1
    ret
RTX_ENDF(rt_pl_dop_anum_guard3)
RTX_FUNC(rt_pl_dop_anum_guard5)
    sub     rsp, 8
    call    rt_pl_dop_anum_guard5_c
    add     rsp, 8
    test    rax, rax
    jz      .Lag5_ok
    mov     r15, rax
    mov     eax, DT_FAIL | (MOD_OP_RT_PL_ANUM_GUARD5 << 8)
    xor     edx, edx
    ret
.Lag5_ok:
    mov     eax, DT_I
    mov     edx, 1
    ret
RTX_ENDF(rt_pl_dop_anum_guard5)
RTX_FUNC(rt_pl_dop_between_guard)
    sub     rsp, 8
    call    rt_pl_dop_between_guard_c
    add     rsp, 8
    test    rax, rax
    jz      .Lbg_ok
    mov     r15, rax
    mov     eax, DT_FAIL | (MOD_OP_RT_PL_BETWEEN_GUARD << 8)
    xor     edx, edx
    ret
.Lbg_ok:
    mov     eax, DT_I
    mov     edx, 1
    ret
RTX_ENDF(rt_pl_dop_between_guard)
RTX_FUNC(rt_pl_dop_stream_guard)
    sub     rsp, 8
    call    rt_pl_dop_stream_guard_c
    add     rsp, 8
    test    rax, rax
    jz      .Lsg_ok
    mov     r15, rax
    mov     eax, DT_FAIL | (MOD_OP_RT_PL_STREAM_GUARD << 8)
    xor     edx, edx
    ret
.Lsg_ok:
    mov     eax, DT_I
    mov     edx, 1
    ret
RTX_ENDF(rt_pl_dop_stream_guard)
RTX_FUNC(rt_pl_dop_curstream_guard)
    sub     rsp, 8
    call    rt_pl_dop_curstream_guard_c
    add     rsp, 8
    test    rax, rax
    jz      .Lcsg_ok
    mov     r15, rax
    mov     eax, DT_FAIL | (MOD_OP_RT_PL_CURSTREAM_GUARD << 8)
    xor     edx, edx
    ret
.Lcsg_ok:
    mov     eax, DT_I
    mov     edx, 1
    ret
RTX_ENDF(rt_pl_dop_curstream_guard)
.section .note.GNU-stack,"",@progbits
