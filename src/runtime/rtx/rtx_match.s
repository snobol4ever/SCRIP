#include "rtx_abi.inc"
RTX_GATE_DEF(match)
RTX_FUNC(rt_cap_match_begin)
    mov     eax, dword ptr [rip + g_cap_gen_next]
    add     eax, 1
    je      .Lcmb_wrap
    mov     dword ptr [rip + g_cap_gen_next], eax
    mov     r10, qword ptr [rip + g_cap_gen@GOTPCREL]
    mov     dword ptr [r10], eax
    ret
.Lcmb_wrap:
    mov     eax, 1
    mov     dword ptr [rip + g_cap_gen_next], eax
    mov     r10, qword ptr [rip + g_cap_gen@GOTPCREL]
    mov     dword ptr [r10], eax
    ret
RTX_ENDF(rt_cap_match_begin)
RTX_FUNC(rt_cap_pop)
    mov     r10, qword ptr [rip + g_cap_gen@GOTPCREL]
    mov     eax, dword ptr [r10]
    cmp     eax, dword ptr [rdi + 8]
    jne     .Lcp_ret
    mov     eax, dword ptr [rdi + 12]
    test    eax, eax
    je      .Lcp_ret
    sub     eax, 1
    mov     dword ptr [rdi + 12], eax
.Lcp_ret:
    ret
RTX_ENDF(rt_cap_pop)
RTX_FUNC(rt_cap_top)
    xor     eax, eax
    mov     r10, qword ptr [rip + g_cap_gen@GOTPCREL]
    mov     ecx, dword ptr [r10]
    cmp     ecx, dword ptr [rdi + 8]
    jne     .Lct_ret
    mov     ecx, dword ptr [rdi + 12]
    test    ecx, ecx
    je      .Lct_ret
    mov     rdx, qword ptr [rdi]
    mov     eax, dword ptr [rdx + rcx*4]
.Lct_ret:
    ret
RTX_ENDF(rt_cap_top)
RTX_FUNC(rt_defer_open)
    RTX_GATE(match, c_rt_defer_open)
    test    esi, esi
    jne     c_rt_defer_open
    test    rdi, rdi
    jz      c_rt_defer_open
    mov     al, byte ptr [rdi]
    cmp     al, 0x2A
    je      c_rt_defer_open
    cmp     al, 0x46
    jne     .Ldo_push
    cmp     dword ptr [rdi], 0x4C494146
    jne     .Ldo_push
    cmp     byte ptr [rdi + 4], 0
    je      c_rt_defer_open
.Ldo_push:
    mov     r10, qword ptr [rip + g_dfx]
    test    r10, r10
    jz      c_rt_defer_open
    mov     eax, dword ptr [rip + g_dfx_top]
    cmp     eax, dword ptr [rip + g_dfx_cap]
    jge     c_rt_defer_open
    lea     r11, [rax + rax*2]
    lea     r11, [r10 + r11*8]
    add     eax, 1
    mov     dword ptr [rip + g_dfx_top], eax
    xor     eax, eax
    mov     qword ptr [r11], rax
    lea     rdx, [rip + .Lrtx_dfx_nul]
    mov     qword ptr [r11 + 8], rdx
    mov     dword ptr [r11 + 16], eax
    mov     dword ptr [r11 + 20], eax
    RTX_CALL_ALIGN
    push    r11
    sub     rsp, 8
    call    NV_GET_fn
    add     rsp, 8
    pop     r11
    RTX_CALL_UNALIGN
    cmp     al, DT_X
    je      .Ldo_dtx
    mov     qword ptr [r11], rax
    mov     qword ptr [r11 + 8], rdx
    xor     eax, eax
    ret
.Ldo_dtx:
    mov     dword ptr [r11 + 20], 1
    test    rdx, rdx
    jnz     .Ldo_dtx_arg
    lea     rdx, [rip + .Lrtx_dfx_nul]
.Ldo_dtx_arg:
    RTX_CALL_ALIGN
    push    r11
    sub     rsp, 8
    mov     rdi, rdx
    xor     esi, esi
    call    rt_proc_call_open
    add     rsp, 8
    pop     r11
    RTX_CALL_UNALIGN
    test    rax, rax
    jnz     .Ldo_dtx_ret
    mov     dword ptr [r11 + 16], 1
.Ldo_dtx_ret:
    ret
RTX_ENDF(rt_defer_open)
RTX_FUNC(rt_defer_close)
    RTX_GATE(match, c_rt_defer_close)
    mov     eax, dword ptr [rip + g_dfx_top]
    test    eax, eax
    jle     .Ldc_empty
    mov     r10, qword ptr [rip + g_dfx]
    test    r10, r10
    jz      c_rt_defer_close
    lea     r11d, [rax - 1]
    lea     r11, [r11 + r11*2]
    lea     r11, [r10 + r11*8]
    cmp     dword ptr [r11 + 16], 0
    jne     .Ldc_pop_fail
    mov     ecx, dword ptr [r11]
    cmp     cl, DT_FAIL
    je      .Ldc_pop_fail
    cmp     cl, DT_S
    je      .Ldc_str
    cmp     cl, DT_SNUL
    jne     c_rt_defer_close
.Ldc_str:
    mov     ecx, dword ptr [r11 + 4]
    test    ecx, ecx
    jz      c_rt_defer_close
    test    ecx, 0x80000000
    jnz     c_rt_defer_close
    mov     rdx, qword ptr [r11 + 8]
    test    rdx, rdx
    jz      c_rt_defer_close
    sub     dword ptr [rip + g_dfx_top], 1
    mov     r10, qword ptr [rip + Σlen@GOTPCREL]
    mov     r10d, dword ptr [r10]
    mov     eax, edi
    add     eax, ecx
    cmp     eax, r10d
    jg      .Ldc_fail
    mov     r10, qword ptr [rip + Σ@GOTPCREL]
    mov     r10, qword ptr [r10]
    mov     esi, edi
    add     r10, rsi
    cmp     ecx, 1
    jne     .Ldc_cmpn
    mov     sil, byte ptr [r10]
    cmp     sil, byte ptr [rdx]
    jne     .Ldc_fail
    ret
.Ldc_cmpn:
    mov     r8, rdi
    mov     rsi, r10
    mov     rdi, rdx
    push    rax
    mov     ecx, ecx
    cld
    repe    cmpsb
    pop     rax
    mov     rdi, r8
    jne     .Ldc_fail
    ret
.Ldc_pop_fail:
    sub     dword ptr [rip + g_dfx_top], 1
.Ldc_fail:
.Ldc_empty:
    mov     eax, -1
    ret
RTX_ENDF(rt_defer_close)
.section .rodata
.align 1
.Lrtx_dfx_nul:
    .byte 0
RTX_FUNC(rt_match_ctx_restore)
    mov     r10, qword ptr [rip + Σ@GOTPCREL]
    mov     qword ptr [r10], rdi
    mov     r10, qword ptr [rip + Σlen@GOTPCREL]
    mov     dword ptr [r10], esi
    ret
RTX_ENDF(rt_match_ctx_restore)
RTX_FUNC(rt_patstk_lazy_init)
    ud2
RTX_ENDF(rt_patstk_lazy_init)
#define RTX_DCAP_TOP_VA 0x70000000
RTX_FUNC(rt_match_enter)
    RTX_GATE(match, c_rt_match_enter)
    cmp     dil, DT_S
    jne     c_rt_match_enter
    test    rsi, rsi
    jz      c_rt_match_enter
    mov     r8, rdi
    shr     r8, 32
    jnz     .Lme_mutate
    cmp     byte ptr [rsi], 0
    je      c_rt_match_enter
.Lme_mutate:
    mov     eax, dword ptr [rip + g_cap_gen_next]
    add     eax, 1
    jne     .Lme_gen_ok
    mov     eax, 1
.Lme_gen_ok:
    mov     dword ptr [rip + g_cap_gen_next], eax
    mov     r10, qword ptr [rip + g_cap_gen@GOTPCREL]
    mov     dword ptr [r10], eax
    test    r12, r12
    je      .Lme_dcap_cold
.Lme_dcap_done:
    test    r8d, r8d
    jnz     .Lme_store
    mov     rdi, rsi
    push    rsi
    call    strlen@PLT
    pop     rsi
    mov     r8, rax
.Lme_store:
    mov     r10, qword ptr [rip + Σ@GOTPCREL]
    mov     qword ptr [r10], rsi
    mov     r10, qword ptr [rip + Σlen@GOTPCREL]
    mov     dword ptr [r10], r8d
    mov     rax, rsi
    mov     rdx, r8
    ret
.Lme_dcap_cold:
    push    rsi
    push    r8
    sub     rsp, 8
    call    rt_dcap_lazy_init
    add     rsp, 8
    pop     r8
    pop     rsi
    jmp     .Lme_dcap_done
RTX_ENDF(rt_match_enter)
RTX_FUNC(rt_dcap_end_ok_open)
    RTX_GATE(match, c_rt_dcap_end_ok_open)
    cmp     dword ptr [rip + g_dcap_trace], 0
    jne     c_rt_dcap_end_ok_open
    mov     rax, qword ptr [rip + g_dcf]
    test    rax, rax
    jz      c_rt_dcap_end_ok_open
    mov     ecx, dword ptr [rip + g_dcf_top]
    cmp     ecx, dword ptr [rip + g_dcf_cap]
    jge     c_rt_dcap_end_ok_open
.Ldeoo_mutate:
    lea     rcx, [rcx + rcx*4]
    shl     rcx, 3
    add     rax, rcx
    inc     dword ptr [rip + g_dcf_top]
    mov     qword ptr [rax + 0], rdi
    mov     qword ptr [rax + 8], rsi
    mov     qword ptr [rax + 16], rdx
    mov     qword ptr [rax + 24], 0
    lea     rcx, [rip + .Lrtx_dfx_nul]
    mov     qword ptr [rax + 32], rcx
    jmp     rt_dcap_pump
RTX_ENDF(rt_dcap_end_ok_open)
RTX_FUNC(rt_dcap_end_ok_close)
    cmp     dword ptr [rip + g_dcf_top], 0
    jle     .Lrtx_deoc_ret
    dec     dword ptr [rip + g_dcf_top]
.Lrtx_deoc_ret:
    ret
RTX_ENDF(rt_dcap_end_ok_close)
RTX_FUNC(rt_match_end_all)
    RTX_GATE(match, c_rt_match_end_all)
    cmp     dword ptr [rip + g_dcap_trace], 0
    jne     c_rt_match_end_all
    mov     r8, qword ptr [rcx + 0]
    mov     r9, qword ptr [rcx + 8]
    push    r9
    push    r8
    sub     rsp, 8
    call    rt_dcap_end_ok_open
    add     rsp, 8
    cmp     dword ptr [rip + g_dcf_top], 0
    jle     .Lmea_skip_pop
    dec     dword ptr [rip + g_dcf_top]
.Lmea_skip_pop:
    mov     rdi, qword ptr [rsp + 0]
    mov     rsi, qword ptr [rsp + 8]
    mov     r10, qword ptr [rip + Σ@GOTPCREL]
    mov     qword ptr [r10], rdi
    mov     r10, qword ptr [rip + Σlen@GOTPCREL]
    mov     dword ptr [r10], esi
    add     rsp, 16
    ret
RTX_ENDF(rt_match_end_all)
RTX_FUNC(rt_match_replace)
    RTX_GATE(match, c_rt_match_replace)
    cmp     dword ptr [rip + g_repl_trace], 0
    jne     c_rt_match_replace
    test    rdi, rdi
    jz      c_rt_match_replace
    cmp     byte ptr [rdi], 0
    je      c_rt_match_replace
    cmp     sil, DT_S
    jne     c_rt_match_replace
    test    rdx, rdx
    jz      c_rt_match_replace
    mov     r11, rsi
    shr     r11, 32
    jnz     .Lmr_subj_ok
    cmp     byte ptr [rdx], 0
    je      c_rt_match_replace
.Lmr_subj_ok:
    xor     r10d, r10d
    test    r9, r9
    jz      .Lmr_repl_done
    mov     eax, dword ptr [r9 + 0]
    cmp     al, DT_SNUL
    je      .Lmr_repl_done
    cmp     al, DT_S
    jne     c_rt_match_replace
    mov     r10, qword ptr [r9 + 8]
    test    r10, r10
    jz      c_rt_match_replace
    cmp     dword ptr [r9 + 4], 0
    jnz     .Lmr_repl_done
    cmp     byte ptr [r10], 0
    jne     .Lmr_repl_done
    xor     r10d, r10d
.Lmr_repl_done:
    xor     r13d, r13d
    test    r9, r9
    jz      .Lmr_replslen_done
    mov     r13d, dword ptr [r9 + 4]
.Lmr_replslen_done:
.Lmr_call:
    push    r12
    push    r13
    sub     rsp, 80
    mov     qword ptr [rsp + 0], rdi
    mov     qword ptr [rsp + 8], rdx
    mov     qword ptr [rsp + 16], rcx
    mov     qword ptr [rsp + 24], r8
    mov     qword ptr [rsp + 32], r10
    mov     r12, r11
    mov     rdi, rdx
    call    strlen@PLT
    test    r12, r12
    cmovnz  rax, r12
    mov     qword ptr [rsp + 40], rax
    xor     eax, eax
    mov     rdi, qword ptr [rsp + 32]
    test    rdi, rdi
    jz      .Lmr_rlen_zero
    call    strlen@PLT
    test    r13, r13
    cmovnz  rax, r13
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
    mov     r12, rax
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
    mov     byte ptr [rax + r12], 0
    mov     rdx, rax
    jmp     .Lmr_setnv
.Lmr_nobuf:
    lea     rdx, [rip + .Lrtx_dfx_nul]
.Lmr_setnv:
    mov     rsi, r12
    shl     rsi, 32
    or      rsi, DT_S | (MOD_OP_RT_MATCH_REPLACE << 8)
    mov     rdi, qword ptr [rsp + 0]
    call    NV_SET_fn
    add     rsp, 80
    pop     r13
    pop     r12
    ret
RTX_ENDF(rt_match_replace)
RTX_FUNC(rt_dcap_step)
    RTX_GATE(match, c_rt_dcap_step)
    mov     r8d, dword ptr [rip + g_dcf_top]
    test    r8d, r8d
    jle     .Lrtx_dcs_ret0
    cmp     dil, DT_FAIL
    je      c_rt_dcap_step
    test    dil, (DT_NOTSTR_MASK & 0xFF)
    jz      c_rt_dcap_step
.Lrtx_dcs_mutate:
    dec     r8d
    lea     r8, [r8 + r8*4]
    shl     r8, 3
    add     r8, qword ptr [rip + g_dcf]
    mov     r10, qword ptr [rip + rt_g_want_name@GOTPCREL]
    mov     dword ptr [r10], 0
    mov     rdx, qword ptr [r8 + 24]
    mov     rcx, qword ptr [r8 + 32]
    sub     rsp, 8
    call    rt_assign_var
    add     rsp, 8
    jmp     rt_dcap_pump
.Lrtx_dcs_ret0:
    xor     eax, eax
    ret
RTX_ENDF(rt_dcap_step)
RTX_FUNC(rt_defer_get_pat_fn)
    test    rdi, rdi
    jz      .Ldfpf_nv
    cmp     byte ptr [rdi], 42
    je      c_rt_defer_get_pat_fn
.Ldfpf_nv:
.Ldfpf_mutate:
    test    rdi, rdi
    jnz     .Ldfpf_call
    lea     rdi, [rip + .Ldfpf_empty]
.Ldfpf_call:
    sub     rsp, 8
    mov     r10, qword ptr [rip + NV_GET_fn@GOTPCREL]
    call    r10
    add     rsp, 8
    cmp     eax, 8
    jne     .Ldfpf_null
    test    rdx, rdx
    jz      .Ldfpf_null
    mov     rdi, rdx
    mov     r10, qword ptr [rip + dtp_fn_of@GOTPCREL]
    jmp     r10
.Ldfpf_null:
    xor     eax, eax
    ret
RTX_ENDF(rt_defer_get_pat_fn)
RTX_FUNC(rt_cap_open)
    test    rdi, rdi
    jz      c_rt_cap_open
    cmp     byte ptr [rdi], 0
    je      c_rt_cap_open
    cmp     byte ptr [rdi], 42
    je      c_rt_cap_open
    .globl  rt_cap_open_plain
    .type   rt_cap_open_plain,@function
rt_cap_open_plain:
    endbr64
    mov     eax, edx
    sub     eax, esi
    test    eax, eax
    jns     .Lcap_len_ok
    xor     eax, eax
.Lcap_len_ok:
    RTX_CALL_ALIGN
    push    rdi
    push    rsi
    push    rax
    push    rax
    movsxd  rdi, eax
    call    rt_str_alloc@PLT
    pop     r8
    pop     rcx
    pop     rsi
    pop     r11
    RTX_CALL_UNALIGN
    test    rax, rax
    jz      .Lcap_nul
    test    ecx, ecx
    jz      .Lcap_nul
    mov     r10, qword ptr [rip + Σ@GOTPCREL]
    mov     r10, qword ptr [r10]
    movsxd  rsi, esi
    add     r10, rsi
    mov     rdi, rax
    mov     r8, rax
    push    rcx
    mov     rsi, r10
    rep movsb
    pop     rcx
    mov     byte ptr [r8 + rcx], 0
    jmp     .Lcap_nv
.Lcap_nul:
    test    rax, rax
    jz      .Lcap_nv_null
    mov     byte ptr [rax], 0
    mov     ecx, 0
    mov     r8, rax
    jmp     .Lcap_nv
.Lcap_nv_null:
    lea     r8, [rip + .Lcap_empty]
    mov     ecx, 0
.Lcap_nv:
    mov     r9, qword ptr [rip + g_sxt_fr@GOTPCREL]
    cmp     qword ptr [r9], r8
    jne     .Lcap_sxt_skip
    mov     qword ptr [r9], 0
.Lcap_sxt_skip:
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
    RTX_CALL_ALIGN
    push    r11
    push    r8
    push    rcx
    push    rcx
    mov     rdi, r11
    call    is_protected_pat_name@PLT
    pop     rcx
    pop     rcx
    pop     r8
    pop     r11
    RTX_CALL_UNALIGN
    test    eax, eax
    jnz     .Lcap_slow
.Lcap_fast:
    RTX_CALL_ALIGN
    push    r11
    push    r8
    push    rcx
    push    rcx
    mov     rdi, r11
    call    NV_CELL_IF_FASTSET_fn@PLT
    pop     rcx
    pop     rcx
    pop     r8
    pop     r11
    RTX_CALL_UNALIGN
    test    rax, rax
    jz      .Lcap_slow
    mov     r9, rax
    mov     eax, 2
    mov     rdx, rcx
    shl     rdx, 32
    or      rax, rdx
    mov     qword ptr [r9], rax
    mov     qword ptr [r9 + 8], r8
    RTX_CALL_ALIGN
    push    r11
    push    r8
    push    rcx
    push    rcx
    call    comm_var_active@PLT
    pop     rcx
    pop     rcx
    pop     r8
    pop     r11
    RTX_CALL_UNALIGN
    test    eax, eax
    jz      .Lcap_fastret
    RTX_CALL_ALIGN
    mov     rdi, r11
    mov     esi, 2
    shl     rcx, 32
    or      rsi, rcx
    mov     rdx, r8
    call    comm_var@PLT
    RTX_CALL_UNALIGN
.Lcap_fastret:
    xor     eax, eax
    ret
.Lcap_slow:
    RTX_CALL_ALIGN
    mov     rdi, r11
    mov     esi, 2
    shl     rcx, 32
    or      rsi, rcx
    mov     rdx, r8
    call    NV_SET_fn@PLT
    RTX_CALL_UNALIGN
    xor     eax, eax
    ret
RTX_ENDF(rt_cap_open)
    .size rt_cap_open_plain, .-rt_cap_open_plain
.section .rodata
.Ldfpf_empty: .byte 0
.Lcap_empty:  .byte 0
.text
.section .note.GNU-stack,"",@progbits
