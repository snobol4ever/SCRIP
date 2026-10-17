#include "rtx_abi.inc"
RTX_GATE_DEF(misc)
RTX_FUNC(rt_call_bid_sn4)
    RTX_GATE(misc, .Lcb_c)
    mov     rax, qword ptr [rip + g_error@GOTPCREL]
    mov     rax, qword ptr [rax]
    add     rax, rax
    jnz     .Lcb_c
    mov     rax, qword ptr [rip + kw_errlimit@GOTPCREL]
    cmp     qword ptr [rax], 0
    jne     .Lcb_c
    test    ecx, ecx
    js      .Lcb_c
    test    ecx, 0x4000
    jz      .Lcb_c
    cmp     edx, 1
    jne     .Lcb_go
    cmp     byte ptr [rsi], DT_DATA
    je      .Lcb_c
.Lcb_go:
    mov     eax, ecx
    and     eax, 0x3FFF
    shl     rax, 4
    lea     r10, [rip + g_bn_direct]
    add     r10, rax
    mov     r11, qword ptr [r10]
    test    r11, r11
    jz      .Lcb_miss
    RTX_PUSH(rdi)
    RTX_PUSH(rsi)
    RTX_PUSH(rdx)
    RTX_PUSH(rcx)
    RTX_SUB_RSP(40)
    mov     ecx, dword ptr [r10 + 8]
    mov     rdi, rsi
    mov     esi, edx
    lea     rdx, [rsp]
    call    r11
    cmp     eax, 1
    jne     .Lcb_nohit
    mov     rax, qword ptr [rsp]
    mov     rdx, qword ptr [rsp + 8]
    RTX_ADD_RSP(72)
    RTX_GVA_R9
    ret
.Lcb_nohit:
    RTX_CFA(80)
    RTX_ADD_RSP(40)
    RTX_POP(rcx)
    RTX_POP(rdx)
    RTX_POP(rsi)
    RTX_POP(rdi)
.Lcb_miss:
    mov     eax, RTX_NOT_HANDLED
    RTX_GVA_R9
    ret
.Lcb_c:
    RTX_CFA(8)
    RTX_DECLINE
RTX_ENDF(rt_call_bid_sn4)
RTX_FUNC(rt_kw_set_rtntype_role)
    cmp     edi, 2
    je      .Lkr_f
    mov     rax, qword ptr [rip + rt_g_ret_by_name@GOTPCREL]
    cmp     dword ptr [rax], 0
    jne     .Lkr_n
    mov     rax, 0x00004E5255544552
    jmp     .Lkr_store
.Lkr_f:
    mov     rax, 0x004E525554455246
    jmp     .Lkr_store
.Lkr_n:
    mov     rax, 0x004E52555445524E
.Lkr_store:
    mov     rdx, qword ptr [rip + kw_rtntype@GOTPCREL]
    mov     qword ptr [rdx], rax
    ret
RTX_ENDF(rt_kw_set_rtntype_role)
RTX_FUNC(descr_identical)
    RTX_GATE(misc, .Ldi_c)
    cmp     dil, DT_FAIL
    je      .Ldi_zero
    cmp     dl, DT_FAIL
    je      .Ldi_zero
    xor     r8d, r8d
    cmp     dil, DT_SNUL
    je      .Ldi_an1
    cmp     dil, DT_S
    jne     .Ldi_an_done
    test    rsi, rsi
    jz      .Ldi_an1
    mov     rax, rdi
    shr     rax, 32
    cmp     eax, -1
    jne     .Ldi_an_len
    cmp     byte ptr [rsi], 0
    jne     .Ldi_an_done
    jmp     .Ldi_an1
.Ldi_an_len:
    test    eax, eax
    jnz     .Ldi_an_done
.Ldi_an1:
    mov     r8d, 1
.Ldi_an_done:
    xor     r9d, r9d
    cmp     dl, DT_SNUL
    je      .Ldi_bn1
    cmp     dl, DT_S
    jne     .Ldi_bn_done
    test    rcx, rcx
    jz      .Ldi_bn1
    mov     rax, rdx
    shr     rax, 32
    cmp     eax, -1
    jne     .Ldi_bn_len
    cmp     byte ptr [rcx], 0
    jne     .Ldi_bn_done
    jmp     .Ldi_bn1
.Ldi_bn_len:
    test    eax, eax
    jnz     .Ldi_bn_done
.Ldi_bn1:
    mov     r9d, 1
.Ldi_bn_done:
    mov     eax, r8d
    and     eax, r9d
    jnz     .Ldi_one
    cmp     r8d, r9d
    jne     .Ldi_zero_r
    cmp     dil, DT_I
    jne     .Ldi_str
    cmp     dl, DT_I
    jne     .Ldi_cs
    xor     eax, eax
    cmp     rsi, rcx
    sete    al
    RTX_GVA_R9
    ret
.Ldi_str:
    cmp     dil, DT_S
    jne     .Ldi_cs
    cmp     dl, DT_S
    jne     .Ldi_cs
    mov     rax, rdi
    shr     rax, 32
    cmp     eax, -1
    je      .Ldi_cs
    mov     r10, rdx
    shr     r10, 32
    cmp     r10d, -1
    je      .Ldi_cs
    cmp     eax, r10d
    jne     .Ldi_zero_r
    xor     r10d, r10d
.Ldi_cmp:
    cmp     r10d, eax
    jae     .Ldi_one
    movzx   r11d, byte ptr [rsi + r10]
    cmp     r11b, byte ptr [rcx + r10]
    jne     .Ldi_zero_r
    inc     r10d
    jmp     .Ldi_cmp
.Ldi_one:
    mov     eax, 1
    RTX_GVA_R9
    ret
.Ldi_zero_r:
    xor     eax, eax
    RTX_GVA_R9
    ret
.Ldi_cs:
    mov     eax, RTX_NOT_HANDLED
    RTX_GVA_R9
    ret
.Ldi_zero:
    xor     eax, eax
    ret
.Ldi_c:
    RTX_DECLINE
RTX_ENDF(descr_identical)
RTX_FUNC(rt_faildescr)
    mov     eax, DT_FAIL
    xor     edx, edx
    ret
RTX_ENDF(rt_faildescr)
RTX_FUNC(rt_is_truthy)
    xor     eax, eax
    cmp     dil, DT_FAIL
    je      .Lit_ret
    cmp     dil, DT_I
    je      .Lit_int
    cmp     dil, DT_BOOL
    je      .Lit_int
    cmp     dil, DT_R
    je      .Lit_real
    test    edi, edi
    je      .Lit_ret
    test    rsi, rsi
    je      .Lit_ret
    movzx   ecx, byte ptr [rsi]
    test    cl, cl
    je      .Lit_ret
    cmp     cl, 0x30
    jne     .Lit_true
    cmp     byte ptr [rsi + 1], 0
    je      .Lit_ret
.Lit_true:
    mov     eax, 1
.Lit_ret:
    ret
.Lit_int:
    test    rsi, rsi
    setne   al
    ret
.Lit_real:
    movq    xmm0, rsi
    pxor    xmm1, xmm1
    ucomisd xmm0, xmm1
    setne   al
    setp    cl
    or      al, cl
    movzx   eax, al
    ret
RTX_ENDF(rt_is_truthy)
#define NV_NAME   0
#define NV_NLEN   8
#define NV_HASNUL 12
#define NV_VAL    32
#define NV_CELL   48
#define NV_ISGVA  56
#define NV_ISIO   68
#define NV_NEXT   72
RTX_FUNC(rt_sno_name_dl)
    RTX_GATE(misc, .Lsn_c)
    cmp     esi, 1
    jne     .Lsn_c
    mov     rax, qword ptr [rip + g_error@GOTPCREL]
    cmp     qword ptr [rax], 0
    jne     .Lsn_c
    mov     rax, qword ptr [rip + kw_errlimit@GOTPCREL]
    cmp     qword ptr [rax], 0
    jne     .Lsn_c
#if RT_DIAG
    mov     rax, qword ptr [rip + g_comm_dbg@GOTPCREL]
    cmp     dword ptr [rax], 0
    jne     .Lsn_c
    mov     rax, qword ptr [rip + trace_set_n@GOTPCREL]
    cmp     dword ptr [rax], 0
    jne     .Lsn_c
    mov     rax, qword ptr [rip + monitor_fd@GOTPCREL]
    cmp     dword ptr [rax], 0
    jge     .Lsn_c
#endif
    mov     rax, qword ptr [rdi]
    cmp     al, DT_S
    jne     .Lsn_c
    mov     r8, qword ptr [rdi + 8]
    test    r8, r8
    jz      .Lsn_c
    mov     r10, rax
    shr     r10, 32
    jz      .Lsn_c
    cmp     r10d, 0x7FFFFFFF
    jae     .Lsn_c
    movzx   ecx, byte ptr [r8]
    cmp     cl, 1
    je      .Lsn_c
    cmp     cl, 38
    je      .Lsn_c
    lea     eax, [rcx - 65]
    cmp     eax, 18
    ja      .Lsn_lead_ok
    mov     edx, 0x60023
    bt      edx, eax
    jc      .Lsn_c
.Lsn_lead_ok:
    cmp     cl, 73
    jne     .Lsn_not_i
    cmp     r10d, 5
    je      .Lsn_c
.Lsn_not_i:
    cmp     cl, 79
    jne     .Lsn_not_o
    cmp     r10d, 6
    je      .Lsn_c
.Lsn_not_o:
    cmp     cl, 84
    jne     .Lsn_not_t
    cmp     r10d, 8
    je      .Lsn_c
.Lsn_not_t:
    mov     rax, qword ptr [rip + _var_init_done@GOTPCREL]
    cmp     dword ptr [rax], 0
    je      .Lsn_c
    mov     rax, qword ptr [rip + g_var_buckets@GOTPCREL]
    mov     rdx, qword ptr [rax]
    test    rdx, rdx
    jz      .Lsn_c
    mov     rax, qword ptr [rip + _var_nbuckets@GOTPCREL]
    mov     r9d, dword ptr [rax]
    test    r9d, r9d
    jz      .Lsn_c
    dec     r9d
    mov     r11, 0x9E3779B97F4A7C15
    xor     r11, r10
    xor     ecx, ecx
.Lsn_w8:
    lea     rax, [rcx + 8]
    cmp     rax, r10
    ja      .Lsn_tail
    mov     rax, qword ptr [r8 + rcx]
    xor     r11, rax
    mov     rax, 0xFF51AFD7ED558CCD
    imul    r11, rax
    add     rcx, 8
    jmp     .Lsn_w8
.Lsn_tail:
    cmp     rcx, r10
    jae     .Lsn_fin
    cmp     r10, 8
    jb      .Lsn_lt8
    mov     rax, qword ptr [r8 + r10 - 8]
    jmp     .Lsn_mix
.Lsn_lt8:
    cmp     r10, 4
    jb      .Lsn_lt4
    mov     eax, dword ptr [r8]
    mov     ecx, dword ptr [r8 + r10 - 4]
    shl     rcx, 32
    or      rax, rcx
    jmp     .Lsn_mix
.Lsn_lt4:
    movzx   eax, byte ptr [r8]
    mov     rcx, r10
    shr     rcx, 1
    movzx   ecx, byte ptr [r8 + rcx]
    shl     ecx, 8
    or      eax, ecx
    movzx   ecx, byte ptr [r8 + r10 - 1]
    shl     ecx, 16
    or      eax, ecx
.Lsn_mix:
    xor     r11, rax
    mov     rax, 0xC4CEB9FE1A85EC53
    imul    r11, rax
.Lsn_fin:
    mov     rax, r11
    shr     rax, 33
    xor     r11, rax
    and     r11, r9
    mov     rax, qword ptr [rdx + r11*8]
.Lsn_chain:
    test    rax, rax
    jz      .Lsn_c
    mov     ecx, dword ptr [rax + NV_NLEN]
    cmp     rcx, r10
    jne     .Lsn_next
    mov     rdx, qword ptr [rax + NV_NAME]
    cmp     r10, 8
    jb      .Lsn_cmp_lt8
    xor     ecx, ecx
.Lsn_cmp_w:
    lea     r9, [rcx + 8]
    cmp     r9, r10
    ja      .Lsn_cmp_last
    mov     r11, qword ptr [rdx + rcx]
    cmp     r11, qword ptr [r8 + rcx]
    jne     .Lsn_next
    mov     rcx, r9
    jmp     .Lsn_cmp_w
.Lsn_cmp_last:
    mov     r11, qword ptr [rdx + r10 - 8]
    cmp     r11, qword ptr [r8 + r10 - 8]
    jne     .Lsn_next
    jmp     .Lsn_hit
.Lsn_cmp_lt8:
    cmp     r10, 4
    jb      .Lsn_cmp_lt4
    mov     r11d, dword ptr [rdx]
    cmp     r11d, dword ptr [r8]
    jne     .Lsn_next
    mov     r11d, dword ptr [rdx + r10 - 4]
    cmp     r11d, dword ptr [r8 + r10 - 4]
    jne     .Lsn_next
    jmp     .Lsn_hit
.Lsn_cmp_lt4:
    xor     ecx, ecx
.Lsn_cmp_b:
    movzx   r11d, byte ptr [rdx + rcx]
    cmp     r11b, byte ptr [r8 + rcx]
    jne     .Lsn_next
    inc     rcx
    cmp     rcx, r10
    jb      .Lsn_cmp_b
    jmp     .Lsn_hit
.Lsn_next:
    mov     rax, qword ptr [rax + NV_NEXT]
    jmp     .Lsn_chain
.Lsn_hit:
    cmp     dword ptr [rax + NV_HASNUL], 0
    jne     .Lsn_c
    cmp     dword ptr [rax + NV_ISIO], 0
    jne     .Lsn_c
    cmp     dword ptr [rax + NV_ISGVA], 0
    je      .Lsn_val
    mov     rdx, qword ptr [rax + NV_CELL]
    test    rdx, rdx
    jz      .Lsn_c
    jmp     .Lsn_ret
.Lsn_val:
    lea     rdx, [rax + NV_VAL]
.Lsn_ret:
    mov     rax, 0x100000028
    RTX_GVA_R9
    ret
.Lsn_c:
    mov     eax, RTX_NOT_HANDLED
    RTX_GVA_R9
    ret
RTX_ENDF(rt_sno_name_dl)
#define PAS_REAL_ARG(l) \
    movzx eax, byte ptr [rdi]; cmp al, DT_R; jne .l##_ni; movsd xmm0, qword ptr [rdi + 8]; jmp .l##_ok; \
.l##_ni: xorpd xmm0, xmm0; cmp al, DT_I; jne .l##_ok; cvtsi2sd xmm0, qword ptr [rdi + 8]; \
.l##_ok:
#define PAS_LIBM(l, fn) \
RTX_FUNC(rt_pas_##l); \
    PAS_REAL_ARG(Lpas_##l); \
    RTX_CALL_ALIGN(8); \
    RTX_SUB_RSP_DYN(32, 40, 8); \
    mov qword ptr [rsp + 24], r11; mov qword ptr [rsp + 16], r8; mov qword ptr [rsp + 8], r10; \
    call fn@PLT; \
    mov r10, qword ptr [rsp + 8]; mov r8, qword ptr [rsp + 16]; mov r11, qword ptr [rsp + 24]; \
    RTX_ADD_RSP_DYN(32, 8, 8); \
    RTX_CALL_UNALIGN(8); \
    RTX_GVA_R9; \
    movq rdx, xmm0; mov eax, DT_R; ret; \
RTX_ENDF(rt_pas_##l)
RTX_FUNC(rt_pas_resundef)
    xor     edx, edx
    cmp     byte ptr [rdi], DT_SNUL
    sete    dl
    mov     eax, DT_I
    ret
RTX_ENDF(rt_pas_resundef)
PAS_LIBM(sin, sin)
PAS_LIBM(cos, cos)
PAS_LIBM(exp, exp)
PAS_LIBM(arctan, atan)
RTX_FUNC(rt_pas_ln)
    RTX_GATE(misc, .Lpln_c)
    PAS_REAL_ARG(Lpln)
    xorpd   xmm1, xmm1
    ucomisd xmm0, xmm1
    jbe     .Lpln_c
    RTX_CALL_ALIGN(8)
    RTX_SUB_RSP_DYN(32, 40, 8)
    mov     qword ptr [rsp + 24], r11
    mov     qword ptr [rsp + 16], r8
    mov     qword ptr [rsp + 8], r10
    call    log@PLT
    mov     r10, qword ptr [rsp + 8]
    mov     r8, qword ptr [rsp + 16]
    mov     r11, qword ptr [rsp + 24]
    RTX_ADD_RSP_DYN(32, 8, 8)
    RTX_CALL_UNALIGN(8)
    RTX_GVA_R9
    movq    rdx, xmm0
    mov     eax, DT_R
    ret
.Lpln_c:
    RTX_DECLINE
RTX_ENDF(rt_pas_ln)
RTX_FUNC(rt_pas_sqrt)
    RTX_GATE(misc, .Lpsq_c)
    PAS_REAL_ARG(Lpsq)
    xorpd   xmm1, xmm1
    ucomisd xmm0, xmm1
    jp      .Lpsq_go
    jb      .Lpsq_c
.Lpsq_go:
    sqrtsd  xmm0, xmm0
    movq    rdx, xmm0
    mov     eax, DT_R
    ret
.Lpsq_c:
    RTX_DECLINE
RTX_ENDF(rt_pas_sqrt)
RTX_FUNC(rt_pas_sqr)
    RTX_GATE(misc, .Lpsr_c)
    movzx   eax, byte ptr [rdi]
    cmp     al, DT_R
    jne     .Lpsr_i
    movsd   xmm0, qword ptr [rdi + 8]
    mulsd   xmm0, xmm0
    movq    rdx, xmm0
    mov     eax, DT_R
    ret
.Lpsr_i:
    xor     edx, edx
    cmp     al, DT_I
    jne     .Lpsr_v
    mov     rdx, qword ptr [rdi + 8]
.Lpsr_v:
    cmp     rdx, 46340
    jg      .Lpsr_c
    cmp     rdx, -46340
    jl      .Lpsr_c
    imul    rdx, rdx
    mov     eax, DT_I
    ret
.Lpsr_c:
    RTX_DECLINE
RTX_ENDF(rt_pas_sqr)
.section .note.GNU-stack,"",@progbits
RTX_FUNC(rt_rk_arr_at)
    RTX_GATE(misc, .Lrka_c)
    cmp     byte ptr [rdi], DT_A
    jne     .Lrka_c
    mov     rax, qword ptr [rdi + 8]
    test    rax, rax
    je      .Lrka_c
    cmp     qword ptr [rax + 24], 0
    jne     .Lrka_c
    cmp     byte ptr [rdi + 16], DT_I
    jne     .Lrka_c
    mov     rcx, qword ptr [rdi + 24]
    movsxd  r8, dword ptr [rax]
    sub     rcx, r8
    js      .Lrka_c
    movsxd  rdx, dword ptr [rax + 4]
    sub     rdx, r8
    cmp     rcx, rdx
    jg      .Lrka_c
    mov     r10, qword ptr [rax + 32]
    shl     rcx, 4
    add     r10, rcx
    mov     r11, qword ptr [r10]
    mov     rdx, qword ptr [r10 + 8]
    test    dword ptr [rax + 20], 16
    je      .Lrka_ret
    cmp     r11b, DT_N
    jne     .Lrka_ret
    mov     rcx, r11
    shr     rcx, 32
    cmp     ecx, 1
    jne     .Lrka_ret
    test    rdx, rdx
    je      .Lrka_ret
    mov     r11, qword ptr [rdx]
    mov     rdx, qword ptr [rdx + 8]
.Lrka_ret:
    mov     rax, r11
    ret
.Lrka_c:
    RTX_DECLINE
RTX_ENDF(rt_rk_arr_at)
RTX_FUNC(rt_rk_arr_set)
    RTX_GATE(misc, .Lrks_c)
    cmp     byte ptr [rdi], DT_A
    jne     .Lrks_c
    mov     rax, qword ptr [rdi + 8]
    test    rax, rax
    je      .Lrks_c
    cmp     qword ptr [rax + 24], 0
    jne     .Lrks_c
    cmp     dword ptr [rax + 8], 1
    jne     .Lrks_c
    cmp     dword ptr [rax], 0
    jne     .Lrks_c
    cmp     byte ptr [rdi + 16], DT_I
    jne     .Lrks_c
    mov     rcx, qword ptr [rdi + 24]
    test    rcx, rcx
    js      .Lrks_c
    movsxd  rdx, dword ptr [rax + 4]
    inc     rdx
    cmp     rcx, rdx
    jge     .Lrks_c
    mov     r10, qword ptr [rax + 32]
    shl     rcx, 4
    add     r10, rcx
    mov     r8, qword ptr [rdi + 32]
    mov     r11, qword ptr [rdi + 40]
    test    dword ptr [rax + 20], 16
    je      .Lrks_store
    mov     rcx, qword ptr [r10]
    cmp     cl, DT_N
    jne     .Lrks_store
    shr     rcx, 32
    cmp     ecx, 1
    jne     .Lrks_store
    mov     rcx, qword ptr [r10 + 8]
    test    rcx, rcx
    je      .Lrks_store
    mov     r10, rcx
.Lrks_store:
    mov     qword ptr [r10], r8
    mov     qword ptr [r10 + 8], r11
    mov     rax, qword ptr [rdi]
    mov     rdx, qword ptr [rdi + 8]
    ret
.Lrks_c:
    RTX_DECLINE
RTX_ENDF(rt_rk_arr_set)
RTX_FUNC(rt_nv_set_fast)
    RTX_GATE(misc, .Lnvs_c)
    test    rdi, rdi
    je      .Lnvs_c
    xor     ecx, ecx
.Lnvs_len:
    cmp     byte ptr [rdi + rcx], 0
    je      .Lnvs_lend
    inc     rcx
    jmp     .Lnvs_len
.Lnvs_lend:
    test    rcx, rcx
    je      .Lnvs_c
    RTX_PUSHS(rbx)
    RTX_PUSHS(r12)
    RTX_PUSHS(r13)
    RTX_SUB_RSP(32)
    mov     rbx, rsi
    mov     r12, rdx
    mov     r13, rdi
    mov     rax, rcx
    shl     rax, 32
    or      rax, DT_S
    mov     qword ptr [rsp], rax
    mov     qword ptr [rsp + 8], rdi
    mov     rdi, rsp
    mov     esi, 1
    RTX_CALL(rt_sno_name_dl)
    cmp     al, RTX_NOT_HANDLED
    je      .Lnvs_undo
    cmp     bl, DT_S
    jne     .Lnvs_store
    mov     rcx, qword ptr [rip + g_sxt_fr@GOTPCREL]
    cmp     qword ptr [rcx], r12
    jne     .Lnvs_store
    mov     qword ptr [rcx], 0
.Lnvs_store:
    mov     qword ptr [rdx], rbx
    mov     qword ptr [rdx + 8], r12
    mov     rax, rbx
    mov     rdx, r12
    RTX_ADD_RSP(32)
    RTX_POPS(r13)
    RTX_POPS(r12)
    RTX_POPS(rbx)
    ret
.Lnvs_undo:
    RTX_CFA(64)
    .cfi_offset rbx, -16
    .cfi_offset r12, -24
    .cfi_offset r13, -32
    mov     rdi, r13
    mov     rsi, rbx
    mov     rdx, r12
    RTX_ADD_RSP(32)
    RTX_POPS(r13)
    RTX_POPS(r12)
    RTX_POPS(rbx)
.Lnvs_c:
    RTX_CFA(8)
    RTX_DECLINE
RTX_ENDF(rt_nv_set_fast)
