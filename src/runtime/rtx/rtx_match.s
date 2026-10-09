#include "rtx_abi.inc"
RTX_GATE_DEF(match)
RTX_FUNC(rt_cap_match_begin)
    mov     eax, dword ptr [rip + g_cap_gen_next]
    add     eax, 1
    je      .Lcmb_wrap
    mov     dword ptr [rip + g_cap_gen_next], eax
    mov     rcx, qword ptr [rip + g_cap_gen@GOTPCREL]
    mov     dword ptr [rcx], eax
    ret
.Lcmb_wrap:
    mov     eax, 1
    mov     dword ptr [rip + g_cap_gen_next], eax
    mov     rcx, qword ptr [rip + g_cap_gen@GOTPCREL]
    mov     dword ptr [rcx], eax
    ret
RTX_ENDF(rt_cap_match_begin)
RTX_FUNC(rt_defer_close)
    RTX_GATE(match, .Ldc_c)
    test    rsi, rsi
    jz      .Ldc_empty
    cmp     dword ptr [rsi + 16], 0
    jne     .Ldc_fail
    mov     ecx, dword ptr [rsi]
    cmp     cl, DT_FAIL
    je      .Ldc_fail
    cmp     cl, DT_S
    je      .Ldc_str
    cmp     cl, DT_SNUL
    jne     .Ldc_c
.Ldc_str:
    mov     ecx, dword ptr [rsi + 4]
    test    ecx, ecx
    jz      .Ldc_c
    test    ecx, 0x80000000
    jnz     .Ldc_c
    mov     rdx, qword ptr [rsi + 8]
    test    rdx, rdx
    jz      .Ldc_c
    mov     rsi, qword ptr [rip + Σlen@GOTPCREL]
    mov     esi, dword ptr [rsi]
    mov     eax, edi
    add     eax, ecx
    cmp     eax, esi
    jg      .Ldc_fail
    mov     rsi, qword ptr [rip + Σ@GOTPCREL]
    mov     rsi, qword ptr [rsi]
    add     rsi, rax
    sub     rsi, rcx
    cmp     ecx, 1
    jne     .Ldc_cmpn
    mov     cl, byte ptr [rsi]
    cmp     cl, byte ptr [rdx]
    jne     .Ldc_fail
    ret
.Ldc_cmpn:
    RTX_PUSH(rdi)
    RTX_PUSH(rax)
    mov     rdi, rdx
    mov     ecx, ecx
    cld
    repe    cmpsb
    RTX_POP(rax)
    RTX_POP(rdi)
    jne     .Ldc_fail
    ret
.Ldc_fail:
.Ldc_empty:
    mov     eax, -1
    ret
.Ldc_c:
    RTX_CTAIL(c_rt_defer_close)
RTX_ENDF(rt_defer_close)
.section .rodata
.align 1
.Lrtx_dfx_nul:
    .byte 0
RTX_FUNC(rt_match_ctx_restore)
    mov     rax, rdi
    neg     rax
    cmp     rax, qword ptr [rdx + 16]
    jne     .Lmr_commit
    mov     rdi, qword ptr [rdx + 8]
.Lmr_commit:
    mov     rax, qword ptr [rip + Σ@GOTPCREL]
    mov     qword ptr [rax], rdi
    mov     rax, qword ptr [rip + Σlen@GOTPCREL]
    mov     dword ptr [rax], esi
    mov     rax, rdi
    ret
RTX_ENDF(rt_match_ctx_restore)
RTX_FUNC(rt_patstk_lazy_init)
    ud2
RTX_ENDF(rt_patstk_lazy_init)
#define RTX_DCAP_TOP_VA 0x70000000
RTX_FUNC(rt_match_enter)
    RTX_GATE(match, .Lme_c)
    cmp     dil, DT_S
    jne     .Lme_c
    test    rsi, rsi
    jz      .Lme_c
    mov     rdx, rdi
    shr     rdx, 32
    jnz     .Lme_mutate
    cmp     byte ptr [rsi], 0
    je      .Lme_c
.Lme_mutate:
    mov     eax, dword ptr [rip + g_cap_gen_next]
    add     eax, 1
    jne     .Lme_gen_ok
    mov     eax, 1
.Lme_gen_ok:
    mov     dword ptr [rip + g_cap_gen_next], eax
    mov     rcx, qword ptr [rip + g_cap_gen@GOTPCREL]
    mov     dword ptr [rcx], eax
    test    r12, r12
    je      .Lme_dcap_cold
.Lme_dcap_done:
    test    edx, edx
    jnz     .Lme_store
    mov     rdi, rsi
    RTX_PUSH(rsi)
    RTX_CCALL(strlen@PLT)
    RTX_POP(rsi)
    mov     rdx, rax
.Lme_store:
    mov     rcx, qword ptr [rip + Σ@GOTPCREL]
    mov     qword ptr [rcx], rsi
    mov     rcx, qword ptr [rip + Σlen@GOTPCREL]
    mov     dword ptr [rcx], edx
    mov     rax, rsi
    ret
.Lme_dcap_cold:
    RTX_PUSH(rsi)
    RTX_PUSH(rdx)
    RTX_SUB_RSP(8)
    RTX_CCALL(rt_dcap_lazy_init)
    RTX_ADD_RSP(8)
    RTX_POP(rdx)
    RTX_POP(rsi)
    jmp     .Lme_dcap_done
.Lme_c:
    RTX_CTAIL(c_rt_match_enter)
RTX_ENDF(rt_match_enter)
RTX_FUNC(rt_dcap_fast_run)
    RTX_GATE(match, .Ldfr_off)
    RTX_PUSH(rbx)
    RTX_PUSH(r12)
    RTX_PUSH(r13)
    RTX_PUSH(r14)
    RTX_PUSH(r15)
    mov     rax, rdi
    test    rdx, rdx
    jz      .Ldfr_ret
.Ldfr_loop:
    cmp     rax, rsi
    jae     .Ldfr_ret
    mov     rcx, qword ptr [rax]
    test    rcx, rcx
    jz      .Ldfr_marker
    cmp     byte ptr [rcx], 42
    je      .Ldfr_ret
    mov     rdi, qword ptr [rax + 16]
    test    rdi, rdi
    jz      .Ldfr_ret
    mov     r12, qword ptr [rax + 8]
    mov     r13, qword ptr [rip + Σlen@GOTPCREL]
    movsxd  r13, dword ptr [r13]
    movsxd  r14, edi
    cmp     r14, r13
    jg      .Ldfr_ret
    lea     r14, [r12 + rdi]
    cmp     r14, r13
    jg      .Ldfr_ret
    movabs  r14, 0x9E3779B97F4A7C15
    imul    r14, rcx
    shr     r14, 32
    and     r14d, 15
    lea     r13, [rip + g_dcap_nv_key]
    cmp     qword ptr [r13 + r14*8], rcx
    jne     .Ldfr_ret
    mov     rbx, qword ptr [rip + g_nv_memo_gen]
    lea     r13, [rip + g_dcap_nv_seen]
    cmp     qword ptr [r13 + r14*8], rbx
    jne     .Ldfr_ret
    lea     r13, [rip + g_dcap_nv_cell]
    mov     r13, qword ptr [r13 + r14*8]
    lea     r15, [rip + g_sxt_fr]
    mov     r14, qword ptr [r15]
    cmp     r14, rdx
    jne     .Ldfr_own1
    mov     qword ptr [r15], 0
    xor     r14d, r14d
.Ldfr_own1:
    lea     r12, [rdx + r12]
    cmp     r14, r12
    jne     .Ldfr_own2
    mov     qword ptr [r15], 0
.Ldfr_own2:
    shl     rdi, 32
    or      rdi, DT_S
    mov     qword ptr [r13], rdi
    mov     qword ptr [r13 + 8], r12
    add     rax, 24
    jmp     .Ldfr_loop
.Ldfr_marker:
    add     rax, 24
    jmp     .Ldfr_loop
.Ldfr_ret:
    RTX_POP(r15)
    RTX_POP(r14)
    RTX_POP(r13)
    RTX_POP(r12)
    RTX_POP(rbx)
    ret
.Ldfr_off:
    mov     rax, rdi
    ret
RTX_ENDF(rt_dcap_fast_run)
RTX_FUNC(rt_dcap_end_ok_open)
    RTX_GATE(match, .Ldeoo_c)
#if RT_DIAG
    cmp     dword ptr [rip + g_dcap_trace], 0
    jne     .Ldeoo_c
#endif
    test    rcx, rcx
    jz      .Ldeoo_c
    mov     qword ptr [rcx + 0], 0
    lea     rax, [rip + .Lrtx_dfx_nul]
    mov     qword ptr [rcx + 8], rax
    mov     qword ptr [rcx + 16], DT_S
    mov     qword ptr [rcx + 24], rdx
    mov     qword ptr [rcx + 32], DT_S
    mov     qword ptr [rcx + 40], 0
    mov     qword ptr [rcx + 48], DT_I
    mov     qword ptr [rcx + 56], rdi
    mov     qword ptr [rcx + 64], DT_I
    mov     qword ptr [rcx + 72], rsi
    mov     qword ptr [rcx + 80], DT_I
    mov     qword ptr [rcx + 88], 0
    mov     qword ptr [rcx + 96], DT_I
    mov     qword ptr [rcx + 104], 0
    mov     rdi, rcx
    RTX_CTAIL(rt_dcap_pump)
.Ldeoo_c:
    RTX_CTAIL(c_rt_dcap_end_ok_open)
RTX_ENDF(rt_dcap_end_ok_open)
RTX_FUNC(rt_match_replace)
    RTX_GATE(match, .Lmr_c)
#if RT_DIAG
    cmp     dword ptr [rip + g_repl_trace], 0
    jne     .Lmr_c
#endif
    test    rdi, rdi
    jz      .Lmr_c
    cmp     byte ptr [rdi], 0
    je      .Lmr_c
    cmp     sil, DT_S
    jne     .Lmr_c
    test    rdx, rdx
    jz      .Lmr_c
    mov     r11, rsi
    shr     r11, 32
    jnz     .Lmr_subj_ok
    cmp     byte ptr [rdx], 0
    je      .Lmr_c
.Lmr_subj_ok:
    xor     r10d, r10d
    test    r9, r9
    jz      .Lmr_repl_done
    mov     eax, dword ptr [r9 + 0]
    cmp     al, DT_SNUL
    je      .Lmr_repl_done
    cmp     al, DT_S
    jne     .Lmr_c
    mov     r10, qword ptr [r9 + 8]
    test    r10, r10
    jz      .Lmr_c
    cmp     dword ptr [r9 + 4], 0
    jnz     .Lmr_repl_done
    cmp     byte ptr [r10], 0
    jne     .Lmr_repl_done
    xor     r10d, r10d
.Lmr_repl_done:
.Lmr_call:
    RTX_SUB_RSP(88)
    xor     eax, eax
    test    r9, r9
    jz      .Lmr_replslen_done
    mov     eax, dword ptr [r9 + 4]
.Lmr_replslen_done:
    mov     qword ptr [rsp + 80], rax
    mov     qword ptr [rsp + 0], rdi
    mov     qword ptr [rsp + 8], rdx
    mov     qword ptr [rsp + 16], rcx
    mov     qword ptr [rsp + 24], r8
    mov     qword ptr [rsp + 32], r10
    mov     qword ptr [rsp + 72], r11
    mov     rdi, rdx
    call    strlen@PLT
    mov     rcx, qword ptr [rsp + 72]
    test    rcx, rcx
    cmovnz  rax, rcx
    mov     qword ptr [rsp + 40], rax
    xor     eax, eax
    mov     rdi, qword ptr [rsp + 32]
    test    rdi, rdi
    jz      .Lmr_rlen_zero
    call    strlen@PLT
    mov     rcx, qword ptr [rsp + 80]
    test    rcx, rcx
    cmovnz  rax, rcx
.Lmr_rlen_zero:
    mov     qword ptr [rsp + 48], rax
    mov     rcx, qword ptr [rsp + 16]
    mov     r8,  qword ptr [rsp + 24]
    mov     rsi, qword ptr [rsp + 40]
    xor     r11d, r11d
    test    rcx, rcx
    cmovs   rcx, r11
    cmp     rcx, rsi
    cmovg   rcx, rsi
    cmp     r8, rcx
    cmovl   r8, rcx
    cmp     r8, rsi
    cmovg   r8, rsi
    mov     qword ptr [rsp + 16], rcx
    mov     qword ptr [rsp + 24], r8
    mov     rax, rsi
    sub     rax, r8
    mov     qword ptr [rsp + 56], rax
    add     rax, rcx
    add     rax, qword ptr [rsp + 48]
    mov     qword ptr [rsp + 72], rax
    mov     rdi, rax
    call    rt_str_alloc
    mov     qword ptr [rsp + 64], rax
    test    rax, rax
    jz      .Lmr_nobuf
    mov     rdx, qword ptr [rsp + 16]
    test    rdx, rdx
    jz      .Lmr_mid
    mov     rdi, rax
    mov     rsi, qword ptr [rsp + 8]
    call    memcpy@PLT
.Lmr_mid:
    mov     rdx, qword ptr [rsp + 48]
    test    rdx, rdx
    jz      .Lmr_tail
    mov     rdi, qword ptr [rsp + 64]
    add     rdi, qword ptr [rsp + 16]
    mov     rsi, qword ptr [rsp + 32]
    call    memcpy@PLT
.Lmr_tail:
    mov     rdx, qword ptr [rsp + 56]
    test    rdx, rdx
    jz      .Lmr_nul
    mov     rdi, qword ptr [rsp + 64]
    add     rdi, qword ptr [rsp + 16]
    add     rdi, qword ptr [rsp + 48]
    mov     rsi, qword ptr [rsp + 8]
    add     rsi, qword ptr [rsp + 24]
    call    memcpy@PLT
.Lmr_nul:
    mov     rax, qword ptr [rsp + 64]
    mov     rcx, qword ptr [rsp + 72]
    mov     byte ptr [rax + rcx], 0
    mov     rdx, rax
    jmp     .Lmr_setnv
.Lmr_nobuf:
    lea     rdx, [rip + .Lrtx_dfx_nul]
.Lmr_setnv:
    mov     rsi, qword ptr [rsp + 72]
    shl     rsi, 32
    or      rsi, DT_S
    mov     rdi, qword ptr [rsp + 0]
    call    NV_SET_fn
    RTX_ADD_RSP(88)
    RTX_GVA_R9
    ret
.Lmr_c:
    RTX_SUB_RSP(8)
    call    c_rt_match_replace
    RTX_ADD_RSP(8)
    RTX_GVA_R9
    ret
RTX_ENDF(rt_match_replace)
RTX_FUNC(rt_cap_open)
    test    rdi, rdi
    jz      .Lco_c
    cmp     byte ptr [rdi], 0
    je      .Lco_c
    cmp     byte ptr [rdi], 42
    je      .Lco_c
RTX_ENTRY(rt_cap_open_plain)
    mov     eax, edx
    sub     eax, esi
    test    eax, eax
    jns     .Lcap_len_ok
    xor     eax, eax
.Lcap_len_ok:
    mov     r11, rdi
    mov     ecx, eax
    test    ecx, ecx
    jz      .Lcap_nv_null
    mov     r10, qword ptr [rip + Σ@GOTPCREL]
    mov     r10, qword ptr [r10]
    mov     r9, qword ptr [rip + g_sxt_fr@GOTPCREL]
    cmp     qword ptr [r9], r10
    jne     .Lcap_sxt_subj_ok
    mov     qword ptr [r9], 0
.Lcap_sxt_subj_ok:
    movsxd  rsi, esi
    lea     r8, [r10 + rsi]
    jmp     .Lcap_nv
.Lcap_nv_null:
    lea     r8, [rip + .Lcap_empty]
    mov     ecx, 0
.Lcap_nv:
    mov     r10, qword ptr [rip + g_protected_pat_vars_armed@GOTPCREL]
    cmp     dword ptr [r10], 0
    je      .Lcap_fast
    movzx   eax, byte ptr [r11]
    cmp     al, 65
    je      .Lcap_lead_hit
    cmp     al, 66
    je      .Lcap_lead_hit
    cmp     al, 70
    je      .Lcap_lead_hit
    cmp     al, 82
    je      .Lcap_lead_hit
    cmp     al, 83
    jne     .Lcap_fast
.Lcap_lead_hit:
    RTX_CALL_ALIGN(8)
    RTX_SUB_RSP_DYN(32, 40, 8)
    mov     qword ptr [rsp + 24], r11
    mov     qword ptr [rsp + 16], r8
    mov     qword ptr [rsp + 8], rcx
    mov     rdi, r11
    call    is_protected_pat_name@PLT
    mov     rcx, qword ptr [rsp + 8]
    mov     r8, qword ptr [rsp + 16]
    mov     r11, qword ptr [rsp + 24]
    RTX_ADD_RSP_DYN(32, 8, 8)
    RTX_CALL_UNALIGN(8)
    test    eax, eax
    jnz     .Lcap_slow
.Lcap_fast:
    RTX_CALL_ALIGN(8)
    RTX_SUB_RSP_DYN(32, 40, 8)
    mov     qword ptr [rsp + 24], r11
    mov     qword ptr [rsp + 16], r8
    mov     qword ptr [rsp + 8], rcx
    mov     rdi, r11
    call    NV_CELL_IF_FASTSET_fn@PLT
    mov     rcx, qword ptr [rsp + 8]
    mov     r8, qword ptr [rsp + 16]
    mov     r11, qword ptr [rsp + 24]
    RTX_ADD_RSP_DYN(32, 8, 8)
    RTX_CALL_UNALIGN(8)
    test    rax, rax
    jz      .Lcap_slow
    mov     r9, rax
    mov     eax, 2
    mov     rdx, rcx
    shl     rdx, 32
    or      rax, rdx
    mov     qword ptr [r9], rax
    mov     qword ptr [r9 + 8], r8
#if RT_DIAG
    RTX_CALL_ALIGN(8)
    RTX_SUB_RSP_DYN(32, 40, 8)
    mov     qword ptr [rsp + 24], r11
    mov     qword ptr [rsp + 16], r8
    mov     qword ptr [rsp + 8], rcx
    call    comm_var_active@PLT
    mov     rcx, qword ptr [rsp + 8]
    mov     r8, qword ptr [rsp + 16]
    mov     r11, qword ptr [rsp + 24]
    RTX_ADD_RSP_DYN(32, 8, 8)
    RTX_CALL_UNALIGN(8)
    test    eax, eax
    jz      .Lcap_fastret
    RTX_CALL_ALIGN(8)
    mov     rdi, r11
    mov     esi, 2
    shl     rcx, 32
    or      rsi, rcx
    mov     rdx, r8
    xor     ecx, ecx
    xor     r8d, r8d
    xor     r9d, r9d
    call    comm_var@PLT
    RTX_CALL_UNALIGN(8)
#endif
.Lcap_fastret:
    xor     eax, eax
    RTX_GVA_R9
    ret
.Lcap_slow:
    RTX_CALL_ALIGN(8)
    mov     rdi, r11
    mov     esi, 2
    shl     rcx, 32
    or      rsi, rcx
    mov     rdx, r8
    call    NV_SET_fn@PLT
    RTX_CALL_UNALIGN(8)
    xor     eax, eax
    RTX_GVA_R9
    ret
.Lco_c:
    RTX_CTAIL(c_rt_cap_open)
RTX_ENDF(rt_cap_open)
    .size rt_cap_open_plain, .-rt_cap_open_plain
RTX_FUNC(rt_cap_open_gva)
    mov     rax, qword ptr [rip + g_call_fastpath_off@GOTPCREL]
    cmp     dword ptr [rax], 0
    jne     .Lcg_plain
#if RT_DIAG
    mov     rax, qword ptr [rip + monitor_fd@GOTPCREL]
    cmp     dword ptr [rax], 0
    jge     .Lcg_plain
    cmp     dword ptr [rip + trace_set_n], 0
    jne     .Lcg_plain
    cmp     dword ptr [rip + g_comm_dbg], 0
    jne     .Lcg_plain
#endif
    mov     r11, rdi
    mov     eax, edx
    sub     eax, esi
    jns     .Lcg_len_ok
    xor     eax, eax
.Lcg_len_ok:
    mov     ecx, eax
    test    ecx, ecx
    jz      .Lcg_nul
    mov     r10, qword ptr [rip + Σ@GOTPCREL]
    mov     r10, qword ptr [r10]
    mov     r9, qword ptr [rip + g_sxt_fr@GOTPCREL]
    cmp     qword ptr [r9], r10
    jne     .Lcg_sxt_skip
    mov     qword ptr [r9], 0
.Lcg_sxt_skip:
    movsxd  rsi, esi
    lea     r8, [r10 + rsi]
.Lcg_store:
    mov     eax, DT_S
    mov     rdx, rcx
    shl     rdx, 32
    or      rax, rdx
    mov     qword ptr [r11], rax
    mov     qword ptr [r11 + 8], r8
    xor     eax, eax
    RTX_GVA_R9
    ret
.Lcg_nul:
    lea     r8, [rip + .Lcap_empty]
    xor     ecx, ecx
    jmp     .Lcg_store
.Lcg_plain:
    mov     rdi, rcx
    mov     ecx, 1
    RTX_JMP(rt_cap_open_plain)
RTX_ENDF(rt_cap_open_gva)
.section .rodata
.Ldfpf_empty: .byte 0
.Lcap_empty:  .byte 0
.text
.section .note.GNU-stack,"",@progbits
