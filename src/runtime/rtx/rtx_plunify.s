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
PL_CTX_LEAF(read_s)
PL_CTX_LEAF(get_char_s)
PL_CTX_LEAF(peek_char_s)
PL_CTX_LEAF(current_output)
PL_CTX_LEAF(current_input)
PL_CTX_LEAF(open)
PL_CTX_LEAF(open4)
PL_CTX_LEAF(keysort)
PL_CTX_LEAF(format3)
.section .note.GNU-stack,"",@progbits
