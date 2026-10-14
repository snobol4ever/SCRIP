#include "rtx_abi.inc"
#define DT_SNUL           0x00
#define DT_S              0x02
#define DT_I              0x03
#define DT_R              0x05
#define DT_A              0x10
#define DT_T              0x18
#define DT_DATA           0x70
#define DT_BIG            0x78
#define DT_N              0x28
#define TBPAIR_KEYD_V     0
#define TBPAIR_KEYD_SLEN  4
#define TBPAIR_KEYD_VAL   8
#define TBPAIR_HKEY      32
#define TBPAIR_SIZE      40
#define TBBUCK_LEN        0
#define TBBUCK_ENT        8
#define TBBLK_BUCKETS     0
#define TBBLK_NBUCK       8
#define TBBLK_DFLT       32
#define ARBLK_LO          0
#define ARBLK_HI          4
#define ARBLK_NDIM        8
#define ARBLK_DATA       32
#define TBBLK_SIZE       12
#define TBBLK_INIT       16
#define HB_AGGB         211
#define TBBLK_ORD        56
#define TBBLK_ORD_LEN    64
#define TBBLK_ORD_CAP    68
#define TBBLK_ICN_MASK   72
#define TBBLK_ORD_DEAD  104
#define TBBUCK_CAP        4
#define TBL_LOAD_MAX      4
#define TBL_LINEAR_MAX   12
RTX_GATE_DEF(table)
RTX_FUNC(rt_table_assign_fast)
    RTX_GATE(table, .Lta_c)
    cmp     dil, DT_T
    jne     .Lta_c
    test    rsi, rsi
    je      .Lta_c
    mov     rax, qword ptr [rip + g_gc_pending@GOTPCREL]
    cmp     dword ptr [rax], 0
    jne     .Lta_c
#if RT_DIAG
    mov     rax, qword ptr [rip + g_sno_etrace_n@GOTPCREL]
    cmp     dword ptr [rax], 0
    jne     .Lta_c
    mov     rax, qword ptr [rip + g_trace_budget@GOTPCREL]
    cmp     qword ptr [rax], 0
    jne     .Lta_c
#endif
    RTX_SUB_RSP(16)
    mov     qword ptr [rsp + 8], r8
    mov     qword ptr [rsp], r9
    RTX_PUSH(rdi)
    RTX_PUSH(rsi)
    RTX_PUSH(rdx)
    RTX_PUSH(rcx)
    RTX_SUB_RSP(8)
    mov     rdi, rsi
    mov     rsi, rdx
    mov     rdx, rcx
    RTX_CALL(table_find_pair_d)
    RTX_ADD_RSP(8)
    mov     r10, rdx
    mov     r11, rcx
    RTX_POP(rcx)
    RTX_POP(rdx)
    RTX_POP(rsi)
    RTX_POP(rdi)
    mov     r8, qword ptr [rsp + 8]
    mov     r9, qword ptr [rsp]
    test    rax, rax
    je      .Lta_miss
    cmp     r8b, DT_S
    jne     .Lta_store
    mov     r10, qword ptr [rip + g_sxt_fr@GOTPCREL]
    cmp     qword ptr [r10], r9
    jne     .Lta_store
    mov     qword ptr [r10], 0
.Lta_store:
    mov     qword ptr [rax + 16], r8
    mov     qword ptr [rax + 24], r9
    mov     qword ptr [rax + 0], rdx
    mov     qword ptr [rax + 8], rcx
    mov     rax, r8
    mov     rdx, r9
    RTX_ADD_RSP(16)
    RTX_GVA_R9
    ret
.Lta_miss:
    RTX_CFA(24)
    test    r10, r10
    je      .Lta_decl
    cmp     dword ptr [rsi + TBBLK_ORD_DEAD], 0
    jne     .Lta_decl
    mov     eax, dword ptr [rsi + TBBLK_ORD_LEN]
    cmp     eax, dword ptr [rsi + TBBLK_ORD_CAP]
    jae     .Lta_decl
    mov     eax, dword ptr [rsi + TBBLK_SIZE]
    inc     eax
    mov     r8, qword ptr [rsi + TBBLK_ICN_MASK]
    inc     r8
    lea     r8, [r8 + r8*4]
    cmp     rax, r8
    ja      .Lta_decl
    mov     r8d, dword ptr [rsi + TBBLK_NBUCK]
    mov     r9d, r8d
    shl     r9d, 2
    cmp     eax, r9d
    ja      .Lta_decl
    dec     r8d
    and     r8d, r11d
    mov     rax, qword ptr [rsi + TBBLK_BUCKETS]
    lea     rax, [rax + r8*8]
    mov     r8, qword ptr [rax]
    test    r8, r8
    je      .Lta_newb
    mov     r9d, dword ptr [r8 + TBBUCK_LEN]
    cmp     r9d, dword ptr [r8 + TBBUCK_CAP]
    jae     .Lta_growb
    mov     rax, r8
    jmp     .Lta_place
.Lta_newb:
    cmp     dword ptr [rsi + TBBLK_INIT], 0
    jl      .Lta_decl
    RTX_PUSH(rsi)
    RTX_PUSH(rdx)
    RTX_PUSH(rcx)
    RTX_PUSH(r10)
    RTX_PUSH(r11)
    mov     r10d, 1
    mov     eax, dword ptr [rsi + TBBLK_INIT]
    test    eax, eax
    jle     .Lta_nb_nc
    xor     edx, edx
    div     dword ptr [rsi + TBBLK_NBUCK]
.Lta_nb_loop:
    cmp     r10d, eax
    jae     .Lta_nb_nc
    cmp     r10d, 64
    jae     .Lta_nb_nc
    shl     r10d, 1
    jmp     .Lta_nb_loop
.Lta_nb_nc:
    RTX_PUSH(r10)
    RTX_PUSH(r8)
    jmp     .Lta_alloc
.Lta_growb:
    RTX_CFA(24)
    RTX_PUSH(rsi)
    RTX_PUSH(rdx)
    RTX_PUSH(rcx)
    RTX_PUSH(r10)
    RTX_PUSH(r11)
    mov     eax, dword ptr [r8 + TBBUCK_CAP]
    lea     r10d, [rax + rax]
    cmp     eax, 128
    jb      .Lta_gb_nc
    lea     r10d, [rax + 128]
.Lta_gb_nc:
    RTX_PUSH(r10)
    RTX_PUSH(r8)
.Lta_alloc:
    mov     eax, dword ptr [rsp + 8]
    imul    eax, eax, TBPAIR_SIZE
    add     eax, TBBUCK_ENT
    mov     esi, eax
    mov     edi, HB_AGGB
    RTX_CALL(rt_gcheap_alloc)
    mov     r8, qword ptr [rsp]
    mov     ecx, dword ptr [rsp + 8]
    mov     dword ptr [rax + TBBUCK_CAP], ecx
    mov     dword ptr [rax + TBBUCK_LEN], 0
    test    r8, r8
    je      .Lta_al_new
    mov     ecx, dword ptr [r8 + TBBUCK_LEN]
    mov     dword ptr [rax + TBBUCK_LEN], ecx
    imul    rcx, rcx, TBPAIR_SIZE
    shr     rcx, 3
    lea     rsi, [r8 + TBBUCK_ENT]
    lea     rdi, [rax + TBBUCK_ENT]
    rep movsq
    mov     r10, qword ptr [rsp + 24]
    sub     r10, r8
    add     r10, rax
    jmp     .Lta_al_done
.Lta_al_new:
    lea     r10, [rax + TBBUCK_ENT]
.Lta_al_done:
    mov     rsi, qword ptr [rsp + 48]
    mov     r11, qword ptr [rsp + 16]
    mov     ecx, dword ptr [rsi + TBBLK_NBUCK]
    dec     ecx
    and     ecx, r11d
    mov     rdx, qword ptr [rsi + TBBLK_BUCKETS]
    mov     qword ptr [rdx + rcx*8], rax
    mov     rcx, qword ptr [rsp + 32]
    mov     rdx, qword ptr [rsp + 40]
    RTX_ADD_RSP(56)
.Lta_place:
    mov     r8d, dword ptr [rax + TBBUCK_LEN]
    imul    r8, r8, TBPAIR_SIZE
    lea     r8, [rax + r8 + TBBUCK_ENT]
.Lta_shift:
    cmp     r8, r10
    jbe     .Lta_shifted
    sub     r8, TBPAIR_SIZE
    mov     r9, qword ptr [r8]
    mov     qword ptr [r8 + 40], r9
    mov     r9, qword ptr [r8 + 8]
    mov     qword ptr [r8 + 48], r9
    mov     r9, qword ptr [r8 + 16]
    mov     qword ptr [r8 + 56], r9
    mov     r9, qword ptr [r8 + 24]
    mov     qword ptr [r8 + 64], r9
    mov     r9, qword ptr [r8 + 32]
    mov     qword ptr [r8 + 72], r9
    jmp     .Lta_shift
.Lta_shifted:
    mov     r8, qword ptr [rsp + 8]
    mov     r9, qword ptr [rsp]
    mov     qword ptr [r10], rdx
    mov     qword ptr [r10 + 8], rcx
    mov     qword ptr [r10 + 16], r8
    mov     qword ptr [r10 + 24], r9
    mov     qword ptr [r10 + 32], r11
    inc     dword ptr [rax + TBBUCK_LEN]
    inc     dword ptr [rsi + TBBLK_SIZE]
    mov     eax, dword ptr [rsi + TBBLK_ORD_LEN]
    mov     r10, qword ptr [rsi + TBBLK_ORD]
    shl     rax, 4
    mov     qword ptr [r10 + rax], rdx
    mov     qword ptr [r10 + rax + 8], rcx
    inc     dword ptr [rsi + TBBLK_ORD_LEN]
    cmp     r8b, DT_S
    jne     .Lta_ins_done
    mov     r10, qword ptr [rip + g_sxt_fr@GOTPCREL]
    cmp     qword ptr [r10], r9
    jne     .Lta_ins_done
    mov     qword ptr [r10], 0
.Lta_ins_done:
    mov     rax, r8
    mov     rdx, r9
    RTX_ADD_RSP(16)
    RTX_GVA_R9
    ret
.Lta_decl:
    RTX_CFA(24)
    mov     r8, qword ptr [rsp + 8]
    mov     r9, qword ptr [rsp]
    RTX_ADD_RSP(16)
    RTX_DECLINE
.Lta_c:
    RTX_CFA(8)
    RTX_DECLINE
RTX_ENDF(rt_table_assign_fast)
RTX_FUNC(table_find_pair_d)
    RTX_GATE(table, .Ltf_c)
    test    rdi, rdi
    je      .Ltf_null
    movzx   eax, sil
    cmp     al, DT_I
    je      .Ltf_h_int
    cmp     al, DT_S
    je      .Ltf_h_str
    cmp     al, DT_SNUL
    je      .Ltf_h_snul
    cmp     al, DT_R
    je      .Ltf_h_real
    cmp     al, DT_A
    je      .Ltf_c
    cmp     al, DT_T
    je      .Ltf_c
    cmp     al, DT_DATA
    je      .Ltf_c
    cmp     al, DT_BIG
    je      .Ltf_c
    cmp     al, DT_N
    je      .Ltf_c
.Ltf_h_ptr:
    mov     r11, rdx
    shr     r11, 4
    mov     rcx, 0xC2B2AE3D27D4EB4F
    imul    r11, rcx
    shr     r11, 8
    jmp     .Ltf_key
.Ltf_h_int:
    mov     r11, rdx
    mov     rcx, 0x9E3779B97F4A7C15
    imul    r11, rcx
    shr     r11, 8
    jmp     .Ltf_key
.Ltf_h_snul:
    mov     r11, 0x2F1B3D5C7E9A11
    jmp     .Ltf_key
.Ltf_h_real:
    mov     r11, rdx
    mov     rcx, r11
    shr     rcx, 32
    xor     r11, rcx
    mov     rcx, 0xBF58476D1CE4E5B9
    imul    r11, rcx
    shr     r11, 8
    jmp     .Ltf_key
.Ltf_h_str:
    test    rdx, rdx
    je      .Ltf_c
    cmp     byte ptr [rdx], 0
    je      .Ltf_c
    mov     rcx, rsi
    shr     rcx, 32
    test    ecx, ecx
    je      .Ltf_str_unstamped
    cmp     ecx, -1
    jne     .Ltf_str_have
.Ltf_str_unstamped:
    mov     r9, rdx
.Ltf_str_scan:
    cmp     byte ptr [r9], 0
    je      .Ltf_str_scand
    inc     r9
    jmp     .Ltf_str_scan
.Ltf_str_scand:
    mov     rcx, r9
    sub     rcx, rdx
.Ltf_str_have:
    mov     r8, rdx
    mov     eax, ecx
    mov     r10, 0x9E3779B97F4A7C15
    imul    r10, rax
    mov     r11, 5381
    xor     r11, r10
    mov     r10, 0xFF51AFD7ED558CCD
    cmp     ecx, 8
    jb      .Ltf_str_tail
.Ltf_str_w8:
    xor     r11, [r8]
    imul    r11, r10
    add     r8, 8
    sub     ecx, 8
    cmp     ecx, 8
    jae     .Ltf_str_w8
.Ltf_str_tail:
    test    ecx, ecx
    je      .Ltf_str_fin
    cmp     ecx, 4
    jb      .Ltf_str_t2
    mov     eax, [r8]
    mov     r9d, ecx
    sub     r9d, 4
    mov     r9d, [r8 + r9]
    shl     r9, 32
    or      rax, r9
    jmp     .Ltf_str_mix
.Ltf_str_t2:
    cmp     ecx, 2
    jb      .Ltf_str_t1
    movzx   eax, word ptr [r8]
    mov     r9d, ecx
    sub     r9d, 2
    movzx   r9d, word ptr [r8 + r9]
    shl     r9, 32
    or      rax, r9
    jmp     .Ltf_str_mix
.Ltf_str_t1:
    movzx   eax, byte ptr [r8]
.Ltf_str_mix:
    xor     r11, rax
    imul    r11, r10
    jmp     .Ltf_str_fin
.Ltf_str_fin:
    mov     rax, r11
    shr     rax, 31
    xor     r11, rax
    mov     rcx, 0xC4CEB9FE1A85EC53
    imul    r11, rcx
    shr     r11, 8
.Ltf_key:
    movzx   rax, sil
    shl     rax, 56
    mov     rcx, 0x00FFFFFFFFFFFFFF
    and     r11, rcx
    or      r11, rax
    mov     eax, [rdi + TBBLK_NBUCK]
    dec     eax
    and     eax, r11d
    mov     rcx, [rdi + TBBLK_BUCKETS]
    mov     rcx, [rcx + rax*8]
    test    rcx, rcx
    je      .Ltf_null_nobucket
    lea     r8, [rcx + TBBUCK_ENT]
    mov     ecx, [rcx + TBBUCK_LEN]
    test    ecx, ecx
    je      .Ltf_null_empty
    mov     rdi, rdx
    mov     rax, rcx
    imul    rax, rax, TBPAIR_SIZE
    lea     r9, [r8 + rax]
    cmp     ecx, TBL_LINEAR_MAX
    ja      .Ltf_bin
    mov     r10, r8
.Ltf_lin:
    cmp     r10, r9
    jae     .Ltf_null_slot
    cmp     [r10 + TBPAIR_HKEY], r11
    jae     .Ltf_scan
    add     r10, TBPAIR_SIZE
    jmp     .Ltf_lin
.Ltf_bin:
    xor     eax, eax
.Ltf_bin_loop:
    test    ecx, ecx
    je      .Ltf_bin_done
    mov     edx, ecx
    shr     edx, 1
    mov     r10d, eax
    add     r10d, edx
    imul    r10, r10, TBPAIR_SIZE
    add     r10, r8
    cmp     [r10 + TBPAIR_HKEY], r11
    jb      .Ltf_bin_lo
    mov     ecx, edx
    jmp     .Ltf_bin_loop
.Ltf_bin_lo:
    add     eax, edx
    add     eax, 1
    sub     ecx, edx
    sub     ecx, 1
    jmp     .Ltf_bin_loop
.Ltf_bin_done:
    imul    rax, rax, TBPAIR_SIZE
    lea     r10, [r8 + rax]
.Ltf_scan:
    cmp     r10, r9
    jae     .Ltf_null_slot
    mov     rax, [r10 + TBPAIR_HKEY]
    cmp     rax, r11
    jne     .Ltf_null_slot
    movzx   eax, byte ptr [r10 + TBPAIR_KEYD_V]
    cmp     al, sil
    jne     .Ltf_next
    cmp     al, DT_S
    je      .Ltf_eq_str
    cmp     al, DT_SNUL
    je      .Ltf_hit
    mov     rax, [r10 + TBPAIR_KEYD_VAL]
    cmp     rax, rdi
    je      .Ltf_hit
.Ltf_next:
    add     r10, TBPAIR_SIZE
    jmp     .Ltf_scan
.Ltf_eq_str:
    mov     rax, [r10 + TBPAIR_KEYD_VAL]
    test    rax, rax
    je      .Ltf_next
    test    rdi, rdi
    je      .Ltf_next
    cmp     rax, rdi
    je      .Ltf_hit
    mov     edx, [r10 + TBPAIR_KEYD_SLEN]
    test    edx, edx
    je      .Ltf_eq_eunstamped
    cmp     edx, -1
    jne     .Ltf_eq_klen
.Ltf_eq_eunstamped:
    mov     r8, rax
.Ltf_eq_escan:
    cmp     byte ptr [r8], 0
    je      .Ltf_eq_escand
    inc     r8
    jmp     .Ltf_eq_escan
.Ltf_eq_escand:
    mov     rdx, r8
    sub     rdx, rax
.Ltf_eq_klen:
    mov     rcx, rsi
    shr     rcx, 32
    test    ecx, ecx
    je      .Ltf_eq_kunstamped
    cmp     ecx, -1
    jne     .Ltf_eq_cmplen
.Ltf_eq_kunstamped:
    mov     r8, rdi
.Ltf_eq_kscan:
    cmp     byte ptr [r8], 0
    je      .Ltf_eq_kscand
    inc     r8
    jmp     .Ltf_eq_kscan
.Ltf_eq_kscand:
    mov     rcx, r8
    sub     rcx, rdi
.Ltf_eq_cmplen:
    cmp     edx, ecx
    jne     .Ltf_next
    test    ecx, ecx
    je      .Ltf_hit
    mov     r8, rdi
.Ltf_scmp:
    movzx   edx, byte ptr [rax]
    movzx   r9d, byte ptr [r8]
    cmp     dl, r9b
    jne     .Ltf_next
    inc     rax
    inc     r8
    dec     ecx
    jne     .Ltf_scmp
    jmp     .Ltf_hit
.Ltf_hit:
    mov     rax, r10
    RTX_GVA_R9
    ret
.Ltf_null_empty:
    mov     r10, r8
.Ltf_null_slot:
    mov     rdx, r10
    mov     rcx, r11
    xor     eax, eax
    RTX_GVA_R9
    ret
.Ltf_null_nobucket:
    mov     edx, 1
    mov     rcx, r11
    xor     eax, eax
    RTX_GVA_R9
    ret
.Ltf_null:
    xor     eax, eax
    xor     edx, edx
    RTX_GVA_R9
    ret
.Ltf_c:
    RTX_SUB_RSP(8)
    call    c_table_find_pair_d
    RTX_ADD_RSP(8)
    xor     edx, edx
    RTX_GVA_R9
    ret
RTX_ENDF(table_find_pair_d)
RTX_FUNC(rt_subscript_var_container_only)
    RTX_GATE(table, .Lsvco_c)
    cmp     dil, DT_T
    je      .Lsvco_tbl
    cmp     dil, DT_A
    je      .Lsvco_sv
    jmp     .Lsvco_c
.Lsvco_tbl:
    test    rsi, rsi
    je      .Lsvco_c
    RTX_PUSH(rsi)
    mov     rdi, rsi
    mov     rsi, rdx
    mov     rdx, rcx
    RTX_CALL(table_find_pair_d)
    RTX_POP(rdi)
    test    rax, rax
    je      .Lsvco_miss
    mov     rdx, [rax + 24]
    mov     rax, [rax + 16]
    ret
.Lsvco_sv:
    RTX_JMP(rt_subscript_var)
.Lsvco_miss:
    movzx   eax, byte ptr [rdi + TBBLK_DFLT]
    cmp     al, DT_FAIL
    je      .Lsvco_null
    test    al, al
    je      .Lsvco_null
    mov     rdx, [rdi + TBBLK_DFLT + 8]
    mov     rax, [rdi + TBBLK_DFLT]
    ret
.Lsvco_null:
    xor     eax, eax
    lea     rdx, [rip + .Lsvco_empty]
    ret
.Lsvco_c:
    RTX_DECLINE
RTX_ENDF(rt_subscript_var_container_only)
RTX_FUNC(rt_subscript_val)
    RTX_GATE(table, .Lsvv_c)
    cmp     dil, DT_T
    je      .Lsvv_tbl
    cmp     dil, DT_A
    jne     .Lsvv_c
    cmp     dl, DT_I
    jne     .Lsvv_c
    test    rsi, rsi
    je      .Lsvv_c
    cmp     dword ptr [rsi + ARBLK_NDIM], 1
    jne     .Lsvv_c
    mov     r8, qword ptr [rsi + ARBLK_DATA]
    test    r8, r8
    je      .Lsvv_c
    movsxd  rax, dword ptr [rsi + ARBLK_LO]
    mov     r10, rcx
    sub     r10, rax
    movsxd  r11, dword ptr [rsi + ARBLK_HI]
    sub     r11, rax
    js      .Lsvv_c
    cmp     r10, r11
    ja      .Lsvv_c
    shl     r10, 4
    mov     rax, qword ptr [r8 + r10]
    mov     rdx, qword ptr [r8 + r10 + 8]
    RTX_GVA_R9
    ret
.Lsvv_tbl:
    test    rsi, rsi
    je      .Lsvv_c
    RTX_PUSH(rdi)
    RTX_PUSH(rsi)
    RTX_PUSH(rdx)
    RTX_PUSH(rcx)
    RTX_SUB_RSP(8)
    mov     rdi, rsi
    mov     rsi, rdx
    mov     rdx, rcx
    RTX_CALL(table_find_pair_d)
    RTX_ADD_RSP(8)
    test    rax, rax
    je      .Lsvv_miss
    mov     rdx, qword ptr [rax + 24]
    mov     rax, qword ptr [rax + 16]
    jmp     .Lsvv_have
.Lsvv_miss:
    mov     rdi, qword ptr [rsp + 16]
    movzx   eax, byte ptr [rdi + TBBLK_DFLT]
    cmp     al, DT_FAIL
    je      .Lsvv_null
    test    al, al
    je      .Lsvv_null
    mov     rdx, qword ptr [rdi + TBBLK_DFLT + 8]
    mov     rax, qword ptr [rdi + TBBLK_DFLT]
    jmp     .Lsvv_have
.Lsvv_null:
    xor     eax, eax
    lea     rdx, [rip + .Lsvco_empty]
.Lsvv_have:
    cmp     al, DT_N
    jne     .Lsvv_ok
    mov     rcx, rax
    shr     rcx, 32
    jnz     .Lsvv_decl
.Lsvv_ok:
    RTX_ADD_RSP(32)
    RTX_GVA_R9
    ret
.Lsvv_decl:
    RTX_CFA(40)
    RTX_POP(rcx)
    RTX_POP(rdx)
    RTX_POP(rsi)
    RTX_POP(rdi)
    RTX_DECLINE
.Lsvv_c:
    RTX_DECLINE
RTX_ENDF(rt_subscript_val)
.section .rodata
.Lsvco_empty:
    .byte 0
.section .note.GNU-stack,"",@progbits
