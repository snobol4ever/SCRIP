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
#define PL_TR_ARENA_MASK  -134217728
#define PL_TR_BALL_SLOT   8
#define PL_BALL_ARM(r, t)  mov r15, r; mov t, r12; and t, PL_TR_ARENA_MASK; mov qword ptr [t + PL_TR_BALL_SLOT], r
#define PL_BALL_DROP(t)    xor r15d, r15d; mov t, r12; and t, PL_TR_ARENA_MASK; mov qword ptr [t + PL_TR_BALL_SLOT], 0
#define PL_BALL_GET(d)  mov d, r12; and d, PL_TR_ARENA_MASK; mov d, qword ptr [d + PL_TR_BALL_SLOT]
RTX_FUNC(rt_pl_quad_seed)
    sub     rsp, 8
    mov     r14, rdi
    xor     r13d, r13d
    RTX_CCALL(rt_pl_tr_init)
    mov     r12, rax
    PL_BALL_DROP(rcx)
    add     rsp, 8
    ret
RTX_ENDF(rt_pl_quad_seed)
RTX_FUNC(rt_pl_throw_raise)
    sub     rsp, 8
    RTX_CCALL(rt_pl_ball_make)
    PL_BALL_ARM(rax, rcx)
    add     rsp, 8
    mov     eax, DT_FAIL
    xor     edx, edx
    ret
RTX_ENDF(rt_pl_throw_raise)
RTX_FUNC(rt_pl_exist_raise)
    sub     rsp, 8
    RTX_CCALL(rt_pl_ball_existence)
    PL_BALL_ARM(rax, rcx)
    add     rsp, 8
    mov     eax, DT_FAIL
    xor     edx, edx
    ret
RTX_ENDF(rt_pl_exist_raise)
RTX_FUNC(rt_pl_goal_resolve)
    RTX_SAVE
    sub     rsp, 24
    mov     qword ptr [rsp + 8], 0
    lea     rcx, [rsp + 8]
    call    rt_pl_goal_resolve_c
    mov     rcx, qword ptr [rsp + 8]
    add     rsp, 24
    test    rcx, rcx
    jz      .Lgr_ret
    PL_BALL_ARM(rcx, rax)
    xor     eax, eax
    xor     edx, edx
.Lgr_ret:
    RTX_RET
RTX_ENDF(rt_pl_goal_resolve)
RTX_FUNC(rt_pl_catch_handle)
    test    r15, r15
    jz      .Lch_fail
    sub     rsp, CTX_FRAME
    CTX_SAVE
    PL_BALL_GET(rdx)
    mov     rcx, rsp
    RTX_CCALL(rt_pl_catch_handle_c)
    mov     r12, qword ptr [rsp + CTX_TR]
    add     rsp, CTX_FRAME
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
    RTX_CTAIL(rt_pl_dop_ball_pending_c)
RTX_ENDF(rt_pl_dop_ball_pending)
RTX_FUNC(rt_pl_dop_clause_unify)
    sub     rsp, CTX_FRAME
    CTX_SAVE
    mov     rdx, rsp
    RTX_CCALL(rt_pl_dop_clause_unify_c)
    mov     r12, qword ptr [rsp + CTX_TR]
    add     rsp, CTX_FRAME
    cmp     al, DT_FAIL
    jne     .Lpcu_ret
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
#define PL_U_TRAIL(c, l) test r13, r13; jz .l##9; cmp c, rsp; jbe .l##5; mov rax, qword ptr [r13 + 32]; shr rax, 8; cmp c, rax; jb .l##9; \
    .l##5: mov rax, r12; and rax, 134217727; cmp rax, 134217696; jae .Lun_refuse; mov rax, qword ptr [c]; mov rcx, qword ptr [c + 8]; mov qword ptr [r12], c; mov qword ptr [r12 + 8], 0; mov qword ptr [r12 + 16], rax; mov qword ptr [r12 + 24], rcx; add r12, 32; \
    .l##9:
#define PL_U_WORK_BYTES 696
#define PL_U_AG_BASE 64
RTX_FUNC(rt_pl_unify_deep)
    sub     rsp, CTX_FRAME
    CTX_SAVE
    mov     rdx, rsp
    RTX_CCALL(rt_pl_unify_deep_c)
    mov     r12, qword ptr [rsp + CTX_TR]
    add     rsp, CTX_FRAME
    ret
RTX_ENDF(rt_pl_unify_deep)
RTX_FUNC(rtx_pl_unify)
    RTX_SAVE
    sub     rsp, PL_U_WORK_BYTES
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
    mov     eax, dword ptr [r11]
    PL_U_UNB(r11, .Lun_both_unb, .Lun_a_unb_b_bnd)
.Lun_a_unb_b_bnd:
    PL_U_TRAIL(r10, Lun_t1)
    mov     rax, qword ptr [r11]
    mov     rdx, qword ptr [r11 + 8]
    mov     qword ptr [r10], rax
    mov     qword ptr [r10 + 8], rdx
    jmp     .Lun_next
.Lun_both_unb:
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
    mov     qword ptr [r11], DT_PLVAR
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
    RTX_CCALL(rt_pl_functor_entries)
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
    RTX_CCALL(rt_pl_unify_atomic_cold)
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
    add     rsp, PL_U_WORK_BYTES
    mov     eax, 1
    RTX_RET
.Lun_fail:
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
    mov     rax, r12
    and     rax, PL_TR_ARENA_MASK
    mov     qword ptr [rax], r12
    add     rsp, PL_U_WORK_BYTES
    xor     eax, eax
    RTX_RET
.Lun_refuse:
    mov     rdi, r12
    RTX_CCALL(rt_pl_tr_refuse)
    add     rsp, PL_U_WORK_BYTES
    xor     eax, eax
    RTX_RET
.Lun_restart:
    mov     rdi, qword ptr [rsp + 48]
    mov     rsi, qword ptr [rsp + 56]
    RTX_CCALL(rt_pl_unify_deep)
    test    eax, eax
    jz      .Lun_fail
    jmp     .Lun_ok
.Lun_deep:
    mov     rdi, r10
    mov     rsi, r11
    RTX_CCALL(rt_pl_unify_deep)
    test    eax, eax
    jz      .Lun_fail
    jmp     .Lun_next
RTX_ENDF(rtx_pl_unify)
#define PL_COLD_BALL_OP(nm) RTX_FUNC(rt_pl_##nm##_cold); sub rsp, 24; mov qword ptr [rsp + 8], 0; lea rcx, [rsp + 8]; RTX_CCALL(rt_pl_##nm##_cold_c); mov rcx, qword ptr [rsp + 8]; add rsp, 24; \
    test rcx, rcx; jz 9f; PL_BALL_ARM(rcx, rax); mov eax, DT_FAIL; xor edx, edx; 9: ret; RTX_ENDF(rt_pl_##nm##_cold)
PL_COLD_BALL_OP(ax)
PL_COLD_BALL_OP(cmp)
#define PL_COLD_BALL_V(nm) RTX_FUNC(rt_pl_##nm##_cold); sub rsp, 24; mov qword ptr [rsp + 8], 0; lea rdx, [rsp + 8]; RTX_CCALL(rt_pl_##nm##_cold_c); mov rcx, qword ptr [rsp + 8]; add rsp, 24; \
    test rcx, rcx; jz 9f; PL_BALL_ARM(rcx, rax); mov eax, DT_FAIL; xor edx, edx; 9: ret; RTX_ENDF(rt_pl_##nm##_cold)
PL_COLD_BALL_V(is)
#define PL_COLD_GUARD(nm) RTX_FUNC(rt_pl_##nm##_cold); sub rsp, 8; RTX_CCALL(rt_pl_##nm##_cold_c); add rsp, 8; test rax, rax; jz 9f; PL_BALL_ARM(rax, rcx); mov eax, DT_FAIL; xor edx, edx; ret; \
    9: mov eax, DT_I; mov edx, 1; ret; RTX_ENDF(rt_pl_##nm##_cold)
PL_COLD_GUARD(zguard)
PL_COLD_GUARD(anum)
#define PL_COLD_PLAIN(nm) RTX_FUNC(rt_pl_##nm##_cold); sub rsp, 8; RTX_CCALL(rt_pl_##nm##_cold_c); add rsp, 8; ret; RTX_ENDF(rt_pl_##nm##_cold)
PL_COLD_PLAIN(type)
PL_COLD_PLAIN(atop)
#define PL_CTX_LEAF(nm) RTX_FUNC(rt_pl_dop_##nm); sub rsp, CTX_FRAME; CTX_SAVE; mov rdx, rsp; \
    RTX_CCALL(rt_pl_dop_##nm##_c); mov r12, qword ptr [rsp + CTX_TR]; add rsp, CTX_FRAME; ret; RTX_ENDF(rt_pl_dop_##nm)
#define PL_CTX_LEAF_BALL(nm) RTX_FUNC(rt_pl_dop_##nm); sub rsp, CTX_FRAME; CTX_SAVE; \
    mov qword ptr [rsp + CTX_BALL], 0; mov rdx, rsp; RTX_CCALL(rt_pl_dop_##nm##_c); mov r12, qword ptr [rsp + CTX_TR]; mov rcx, qword ptr [rsp + CTX_BALL]; add rsp, CTX_FRAME; \
    test rcx, rcx; jz 99f; PL_BALL_ARM(rcx, rax); mov eax, DT_FAIL ; xor edx, edx; 99: ret; RTX_ENDF(rt_pl_dop_##nm)
PL_CTX_LEAF(sub_atom_at)
PL_CTX_LEAF(atom_concat_at)
PL_CTX_LEAF(bagof_group_at)
PL_CTX_LEAF(setof_group_at)
PL_CTX_LEAF(findall_result)
PL_CTX_LEAF(findall_result4)
PL_CTX_LEAF(bagof_result)
PL_CTX_LEAF(setof_result)
PL_CTX_LEAF_BALL(compare)
PL_CTX_LEAF(functor)
PL_CTX_LEAF(arg)
PL_CTX_LEAF(skip_list)
PL_CTX_LEAF_BALL(univ)
PL_CTX_LEAF(copy_term)
PL_CTX_LEAF_BALL(term_variables)
PL_CTX_LEAF_BALL(numbervars3)
PL_CTX_LEAF(numbervars1)
PL_CTX_LEAF_BALL(succ)
PL_CTX_LEAF_BALL(plus)
PL_CTX_LEAF(wall_us)
PL_CTX_LEAF(wall_ms)
PL_CTX_LEAF_BALL(sort)
PL_CTX_LEAF_BALL(msort)
PL_CTX_LEAF(char_type)
PL_CTX_LEAF_BALL(term_string)
PL_CTX_LEAF(atom_length)
PL_CTX_LEAF(atom_concat)
PL_CTX_LEAF(atomic_concat)
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
PL_CTX_LEAF(string_codes)
PL_CTX_LEAF(string_chars)
PL_CTX_LEAF(number_string)
PL_CTX_LEAF(atomic_list_concat)
PL_CTX_LEAF(concat_atom)
PL_CTX_LEAF(char_code)
PL_CTX_LEAF(number_codes)
PL_CTX_LEAF(number_chars)
PL_CTX_LEAF(name)
PL_CTX_LEAF(get_char)
PL_CTX_LEAF(peek_char)
PL_CTX_LEAF(get_code)
PL_CTX_LEAF(peek_code)
PL_CTX_LEAF_BALL(get_byte)
PL_CTX_LEAF_BALL(peek_byte)
PL_CTX_LEAF(unget_char)
PL_CTX_LEAF(unget_code)
PL_CTX_LEAF(unget_byte)
PL_CTX_LEAF(get_edin)
PL_CTX_LEAF(telling)
PL_CTX_LEAF(seeing)
PL_CTX_LEAF(skip)
PL_CTX_LEAF_BALL(read)
PL_CTX_LEAF_BALL(atom_to_term)
PL_CTX_LEAF_BALL(display)
PL_CTX_LEAF_BALL(display_s)
PL_CTX_LEAF(unify_oc)
PL_CTX_LEAF(aggregate_reduce)
PL_CTX_LEAF_BALL(read_term_opts)
PL_CTX_LEAF_BALL(read_term_opts_s)
PL_CTX_LEAF(wot_open)
PL_CTX_LEAF(wot_capture)
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
PL_CTX_LEAF(current_output)
PL_CTX_LEAF(current_input)
PL_CTX_LEAF_BALL(open)
PL_CTX_LEAF_BALL(open4)
PL_CTX_LEAF_BALL(keysort)
PL_CTX_LEAF_BALL(gnu_sort1)
PL_CTX_LEAF_BALL(gnu_msort1)
PL_CTX_LEAF_BALL(gnu_keysort1)
PL_CTX_LEAF_BALL(gnu_line_count)
PL_CTX_LEAF_BALL(gnu_line_position)
PL_CTX_LEAF_BALL(gnu_character_count)
PL_CTX_LEAF_BALL(gnu_stream_line_column)
PL_CTX_LEAF_BALL(gnu_last_read_start)
PL_CTX_LEAF_BALL(gnu_absolute_file_name)
PL_CTX_LEAF_BALL(gnu_prolog_file_name)
PL_CTX_LEAF_BALL(gnu_working_directory)
PL_CTX_LEAF_BALL(gnu_change_directory)
PL_CTX_LEAF_BALL(gnu_make_directory)
PL_CTX_LEAF_BALL(gnu_delete_file)
PL_CTX_LEAF_BALL(gnu_file_exists)
PL_CTX_LEAF_BALL(gnu_directory_files)
PL_CTX_LEAF_BALL(gnu_environ_list)
PL_CTX_LEAF_BALL(gnu_file_props)
PL_CTX_LEAF_BALL(gnu_term_hash)
PL_CTX_LEAF_BALL(gnu_prolog_pid)
PL_CTX_LEAF(gnu_builtin)
PL_CTX_LEAF_BALL(set_stream_position)
PL_CTX_LEAF_BALL(format)
PL_CTX_LEAF_BALL(format3)
PL_CTX_LEAF_BALL(write_term)
PL_CTX_LEAF_BALL(write_term_s)
PL_CTX_LEAF_BALL(write_sb)
PL_CTX_LEAF_BALL(writeq_sb)
PL_CTX_LEAF_BALL(write_canonical_sb)
PL_CTX_LEAF_BALL(writeln_sb)
PL_CTX_LEAF_BALL(nl_sb)
PL_CTX_LEAF_BALL(tab_sb)
PL_CTX_LEAF_BALL(put_char_sb)
PL_CTX_LEAF_BALL(put_code_sb)
PL_CTX_LEAF_BALL(flush_output_sb)
PL_CTX_LEAF(pl_op_count)
PL_CTX_LEAF(cutcall)
PL_CTX_LEAF_BALL(op)
PL_CTX_LEAF_BALL(pl_op_check)
PL_CTX_LEAF_BALL(pl_sp_check)
PL_CTX_LEAF_BALL(pl_ioarg)
PL_CTX_LEAF(pl_op_nth)
PL_CTX_LEAF(pl_sp_count)
PL_CTX_LEAF_BALL(pl_sp_nth)
PL_CTX_LEAF(pl_cs_count)
PL_CTX_LEAF(pl_cs_nth)
PL_CTX_LEAF_BALL(pl_cp_guard)
PL_CTX_LEAF_BALL(pl_pp_guard)
PL_CTX_LEAF_BALL(halt)
#define PL_ROOT_LEAF(nm) RTX_FUNC(rt_pl_dop_##nm); mov rdx, r14; RTX_CTAIL(rt_pl_dop_##nm##_c); RTX_ENDF(rt_pl_dop_##nm)
PL_ROOT_LEAF(db_assertz)
PL_ROOT_LEAF(db_asserta)
PL_ROOT_LEAF(db_erase)
PL_ROOT_LEAF(db_abolish)
PL_ROOT_LEAF(db_retractall)
PL_ROOT_LEAF(db_nonempty)
PL_ROOT_LEAF(db_seed_once)
PL_ROOT_LEAF(nb_setval)
PL_ROOT_LEAF(db_assertz_t)
PL_ROOT_LEAF(db_asserta_t)
PL_ROOT_LEAF(db_erase_t)
PL_ROOT_LEAF(db_erase_ref)
PL_ROOT_LEAF(db_n_r)
PL_ROOT_LEAF(db_at_r)
PL_ROOT_LEAF(db_ref_r)
PL_ROOT_LEAF(db_abolish_t)
PL_ROOT_LEAF(db_retractall_t)
PL_ROOT_LEAF(db_store_k)
PL_ROOT_LEAF(db_copy)
#define PL_ROOTCTX_LEAF(nm) RTX_FUNC(rt_pl_dop_##nm); sub rsp, CTX_FRAME; CTX_SAVE; mov rdx, rsp; mov rcx, r14; \
    RTX_CCALL(rt_pl_dop_##nm##_c); mov r12, qword ptr [rsp + CTX_TR]; add rsp, CTX_FRAME; ret; RTX_ENDF(rt_pl_dop_##nm)
PL_ROOTCTX_LEAF(nb_getval)
PL_ROOTCTX_LEAF(b_setval)
PL_ROOTCTX_LEAF(pl_cp_count)
PL_ROOTCTX_LEAF(pl_cp_nth)
PL_ROOTCTX_LEAF(pl_pp_count)
PL_ROOTCTX_LEAF(pl_pp_nth)
PL_ROOTCTX_LEAF(db_assertz_r)
PL_ROOTCTX_LEAF(db_asserta_r)
RTX_FUNC(rt_pl_dop_db_alive)
    sub     rsp, 8
    mov     rdx, r14
    RTX_CCALL(rt_pl_dop_db_alive_c)
    add     rsp, 8
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
    sub     rsp, 8
    mov     rdx, r14
    RTX_CCALL(rt_pl_dop_db_t_guard_c)
    add     rsp, 8
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
    sub     rsp, 8
    RTX_CCALL(rt_pl_dop_goal_guard_c)
    add     rsp, 8
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
    sub     rsp, 8
    RTX_CCALL(rt_pl_dop_list_guard_c)
    add     rsp, 8
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
    sub     rsp, 8
    RTX_CCALL(rt_pl_dop_char_guard_c)
    add     rsp, 8
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
    sub     rsp, 8
    mov     rdx, r14
    RTX_CCALL(rt_pl_dop_nb_getval_guard_c)
    add     rsp, 8
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
    sub     rsp, 8
    RTX_CCALL(rt_pl_dop_ax_eguard_c)
    add     rsp, 8
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
    sub     rsp, 8
    RTX_CCALL(rt_pl_dop_between_guard_c)
    add     rsp, 8
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
    sub     rsp, 8
    RTX_CCALL(rt_pl_dop_stream_guard_c)
    add     rsp, 8
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
    sub     rsp, 8
    RTX_CCALL(rt_pl_dop_curstream_guard_c)
    add     rsp, 8
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
.section .note.GNU-stack,"",@progbits
