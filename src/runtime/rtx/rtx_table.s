#include "rtx_abi.inc"
#define DT_SNUL           0x00
#define DT_S              0x02
#define DT_I              0x03
#define DT_R              0x05
#define DT_A              0x10
#define DT_T              0x18
#define DT_DATA           0x70
#define DT_BIG            0x78
#define TBPAIR_KEYD_V     8
#define TBPAIR_KEYD_SLEN 12
#define TBPAIR_KEYD_VAL  16
#define TBPAIR_HKEY      40
#define TBPAIR_SIZE      48
#define TBBUCK_LEN        0
#define TBBUCK_ENT        8
#define TBBLK_BUCKETS     0
#define TBBLK_NBUCK       8
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
    mov     rax, qword ptr [rip + g_sno_etrace_n@GOTPCREL]
    cmp     dword ptr [rax], 0
    jne     .Lta_c
    mov     rax, qword ptr [rip + g_trace_budget@GOTPCREL]
    cmp     qword ptr [rax], 0
    jne     .Lta_c
    RTX_SAVE
    push    rdi
    push    rsi
    push    rdx
    push    rcx
    sub     rsp, 8
    mov     rdi, rsi
    mov     rsi, rdx
    mov     rdx, rcx
    RTX_CALL(table_find_pair_d)
    add     rsp, 8
    pop     rcx
    pop     rdx
    pop     rsi
    pop     rdi
    test    rax, rax
    je      .Lta_miss
    cmp     r8b, DT_S
    jne     .Lta_store
    mov     r10, qword ptr [rip + g_sxt_fr@GOTPCREL]
    cmp     qword ptr [r10], r9
    jne     .Lta_store
    mov     qword ptr [r10], 0
.Lta_store:
    mov     qword ptr [rax + 24], r8
    mov     qword ptr [rax + 32], r9
    mov     qword ptr [rax + 8], rdx
    mov     qword ptr [rax + 16], rcx
    mov     rax, r8
    mov     rdx, r9
    RTX_RET_GVA
.Lta_miss:
    RTX_CTAIL_SAVED_GVA(c_rt_table_assign_fast)
.Lta_c:
    RTX_CTAIL_GVA(c_rt_table_assign_fast)
RTX_ENDF(rt_table_assign_fast)
RTX_FUNC(table_find_pair_d)
    RTX_SAVE
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
    je      .Ltf_str_n0
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
.Ltf_str_n0:
    mov     r11, 5381
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
    je      .Ltf_null
    lea     r8, [rcx + TBBUCK_ENT]
    mov     ecx, [rcx + TBBUCK_LEN]
    test    ecx, ecx
    je      .Ltf_null
    mov     rdi, rdx
    mov     rax, rcx
    imul    rax, rax, TBPAIR_SIZE
    lea     r9, [r8 + rax]
    cmp     ecx, TBL_LINEAR_MAX
    ja      .Ltf_bin
    mov     r10, r8
.Ltf_lin:
    cmp     r10, r9
    jae     .Ltf_null
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
    jae     .Ltf_null
    mov     rax, [r10 + TBPAIR_HKEY]
    cmp     rax, r11
    jne     .Ltf_null
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
    RTX_RET
.Ltf_null:
    xor     eax, eax
    RTX_RET
.Ltf_c:
    RTX_CTAIL_SAVED(c_table_find_pair_d)
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
    push    rsi
    mov     rdi, rsi
    mov     rsi, rdx
    mov     rdx, rcx
    RTX_CALL(table_find_pair_d)
    pop     rdi
    test    rax, rax
    je      .Lsvco_miss
    mov     rdx, [rax + 32]
    mov     rax, [rax + 24]
    ret
.Lsvco_sv:
    RTX_JMP(rt_subscript_var)
.Lsvco_miss:
    RTX_CTAIL(c_rt_svco_miss_d)
.Lsvco_c:
    RTX_CTAIL(c_rt_subscript_var_container_only)
RTX_ENDF(rt_subscript_var_container_only)
.section .note.GNU-stack,"",@progbits
