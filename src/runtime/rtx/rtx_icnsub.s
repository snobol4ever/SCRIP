#include "rtx_abi.inc"
RTX_GATE_DEF(icnsub)
#define DATINST_TYPE      0
#define DATINST_FIELDS    8
#define DATBLK_NFIELDS    8
#define DATBLK_FIELDS    16
#define VCELL_CELLP       0
#define VCELL_TBL         8
#define VCELL_KEY        16
#define VCELL_KEY_D      24
#define VCELL_SV         40
#define VCELL_POS        56
#define VCELL_LEN        64
#define VCELL_SIZE       72
#define DESCR_SIZE       16
#define FIELD0_V          0
#define FIELD0_P          8
#define FIELD1_P         24
#define FIELD2_V         32
#define FIELD2_P         40
#define TBPAIR_KEY        0
#define TBPAIR_VAL       24
#define TBPAIR_NEXT      40
#define TBBLK_BUCKETS     0
#define TBL_HASH_SEED  5381
#define ARBLK_LO          0
#define ARBLK_HI          4
#define ARBLK_NDIM        8
#define ARBLK_DATA       32
RTX_FUNC(rt_subscript_var)
    RTX_GATE(icnsub, c_rt_subscript_var)
    cmp     dl, DT_I
    je      .Lsub_tag_ok
    cmp     dl, DT_S
    jne     c_rt_subscript_var
.Lsub_tag_ok:
    cmp     dil, DT_A
    jne     .Lsub_not_array
    cmp     dl, DT_I
    jne     c_rt_subscript_var
    jmp     .Lsub_array
.Lsub_not_array:
    cmp     dil, DT_T
    jne     .Lsub_not_table_direct
    cmp     dl, DT_I
    jne     c_rt_subscript_var
    jmp     .Lsub_table_direct
.Lsub_not_table_direct:
    cmp     dil, DT_N
    jne     c_rt_subscript_var
    mov     rax, rdi
    shr     rax, 32
    cmp     eax, 1
    jne     c_rt_subscript_var
    test    rsi, rsi
    je      c_rt_subscript_var
    sub     rsp, 88
    mov     [rsp + 0], rdi
    mov     [rsp + 8], rsi
    mov     [rsp + 16], rdx
    mov     [rsp + 24], rcx
    call    rt_deref
    cmp     dword ptr [rsp + 16], DT_S
    je      .Lsub_table
    cmp     al, DT_S
    je      .Lsub_string
    cmp     al, DT_T
    je      .Lsub_table_int
    cmp     al, DT_DATA
    jne     .Lsub_bail
    mov     rsi, rdx
    test    rsi, rsi
    je      .Lsub_bail
    mov     r8, [rsi + DATINST_TYPE]
    test    r8, r8
    je      .Lsub_bail
    cmp     dword ptr [r8 + DATBLK_NFIELDS], 3
    jl      .Lsub_bail
    mov     r9, [r8 + DATBLK_FIELDS]
    test    r9, r9
    je      .Lsub_bail
    mov     r10, [r9]
    test    r10, r10
    je      .Lsub_bail
    mov     rax, 0x6c655f656d617266
    cmp     [r10], rax
    jne     .Lsub_bail
    cmp     dword ptr [r10 + 8], 0x00736d65
    jne     .Lsub_bail
    mov     r9, [rsi + DATINST_FIELDS]
    test    r9, r9
    je      .Lsub_bail
    cmp     dword ptr [r9 + FIELD2_V], DT_S
    jne     .Lsub_bail
    mov     r10, [r9 + FIELD2_P]
    test    r10, r10
    je      .Lsub_bail
    cmp     dword ptr [r10], 0x7473696c
    jne     .Lsub_bail
    cmp     byte ptr [r10 + 4], 0
    jne     .Lsub_bail
    cmp     dword ptr [r9 + FIELD0_V], DT_DATA
    jne     .Lsub_bail
    mov     r8, [r9 + FIELD0_P]
    test    r8, r8
    je      .Lsub_bail
    mov     [rsp + 32], r8
    mov     r11d, dword ptr [r9 + FIELD1_P]
    mov     eax, dword ptr [rsp + 24]
    test    eax, eax
    jns     .Lsub_nowrap
    lea     eax, [rax + r11 + 1]
.Lsub_nowrap:
    cmp     eax, 1
    jl      .Lsub_fail
    cmp     eax, r11d
    jg      .Lsub_fail
    dec     eax
    mov     [rsp + 40], eax
    xor     edi, edi
    mov     esi, VCELL_SIZE
    call    rt_agg_alloc
    movsxd  rcx, dword ptr [rsp + 40]
    shl     rcx, 4
    add     rcx, [rsp + 32]
    mov     [rax + VCELL_CELLP], rcx
    mov     qword ptr [rax + VCELL_TBL], 0
    mov     qword ptr [rax + VCELL_KEY], 0
    mov     r8, [rsp + 16]
    mov     r9, [rsp + 24]
    mov     [rax + VCELL_KEY_D], r8
    mov     [rax + VCELL_KEY_D + 8], r9
    mov     qword ptr [rax + VCELL_SV], DT_FAIL | (MOD_OP_RT_SUBSCRIPT_VAR << 8)
    mov     qword ptr [rax + VCELL_SV + 8], 0
    mov     qword ptr [rax + VCELL_POS], 0
    mov     qword ptr [rax + VCELL_LEN], 0
    add     rsp, 88
    mov     rdx, rax
    mov     rax, DT_NAMETRAP_LO | (MOD_OP_RT_SUBSCRIPT_VAR << 8)
    ret
.Lsub_table:
    jmp     .Lsub_bail
    cmp     al, DT_T
    jne     .Lsub_bail
    mov     rsi, rdx
    test    rsi, rsi
    je      .Lsub_bail
    mov     rdi, [rsp + 24]
    test    rdi, rdi
    je      .Lsub_bail
.Lsub_hash_init:
    mov     eax, TBL_HASH_SEED
    mov     r8, rdi
.Lsub_hash:
    movzx   ecx, byte ptr [r8]
    test    cl, cl
    je      .Lsub_hash_done
    mov     r9d, eax
    shl     r9d, 5
    add     r9d, eax
    xor     r9d, ecx
    mov     eax, r9d
    inc     r8
    jmp     .Lsub_hash
.Lsub_hash_done:
    movzx   eax, al
    mov     r10, [rsi + rax*8 + TBBLK_BUCKETS]
.Lsub_chain:
    test    r10, r10
    je      .Lsub_tbl_miss
    mov     r11, [r10 + TBPAIR_KEY]
    test    r11, r11
    je      .Lsub_bail
    mov     r8, rdi
.Lsub_cmp:
    movzx   ecx, byte ptr [r11]
    movzx   edx, byte ptr [r8]
    cmp     cl, dl
    jne     .Lsub_chain_next
    test    cl, cl
    je      .Lsub_hit
    inc     r11
    inc     r8
    jmp     .Lsub_cmp
.Lsub_chain_next:
    mov     r10, [r10 + TBPAIR_NEXT]
    jmp     .Lsub_chain
.Lsub_hit:
    lea     rcx, [r10 + TBPAIR_VAL]
    mov     [rsp + 32], rsi
    mov     [rsp + 40], rcx
    xor     edi, edi
    mov     esi, VCELL_SIZE
    call    rt_agg_alloc
    mov     rcx, [rsp + 40]
    mov     [rax + VCELL_CELLP], rcx
    mov     rcx, [rsp + 32]
    mov     [rax + VCELL_TBL], rcx
    mov     qword ptr [rax + VCELL_KEY], 0
    mov     r8, [rsp + 16]
    mov     r9, [rsp + 24]
    mov     [rax + VCELL_KEY_D], r8
    mov     [rax + VCELL_KEY_D + 8], r9
    mov     qword ptr [rax + VCELL_SV], DT_FAIL | (MOD_OP_RT_SUBSCRIPT_VAR << 8)
    mov     qword ptr [rax + VCELL_SV + 8], 0
    mov     qword ptr [rax + VCELL_POS], 0
    mov     qword ptr [rax + VCELL_LEN], 0
    add     rsp, 88
    mov     rdx, rax
    mov     rax, DT_NAMETRAP_LO | (MOD_OP_RT_SUBSCRIPT_VAR << 8)
    ret
.Lsub_tbl_miss:
    mov     [rsp + 32], rsi
    mov     [rsp + 48], rdi
    xor     edi, edi
    mov     esi, VCELL_SIZE
    call    rt_agg_alloc
    mov     [rsp + 40], rax
    mov     qword ptr [rax + VCELL_CELLP], 0
    mov     rcx, [rsp + 32]
    mov     [rax + VCELL_TBL], rcx
    mov     rdi, [rsp + 48]
    call    rt_ws_strdup_c
    mov     rcx, [rsp + 40]
    mov     [rcx + VCELL_KEY], rax
    mov     r8, [rsp + 16]
    mov     r9, [rsp + 24]
    mov     [rcx + VCELL_KEY_D], r8
    mov     [rcx + VCELL_KEY_D + 8], r9
    mov     qword ptr [rcx + VCELL_SV], DT_FAIL | (MOD_OP_RT_SUBSCRIPT_VAR << 8)
    mov     qword ptr [rcx + VCELL_SV + 8], 0
    mov     qword ptr [rcx + VCELL_POS], 0
    mov     qword ptr [rcx + VCELL_LEN], 0
    add     rsp, 88
    mov     rdx, rcx
    mov     rax, DT_NAMETRAP_LO | (MOD_OP_RT_SUBSCRIPT_VAR << 8)
    ret
.Lsub_string:
    mov     r8, rax
    shr     r8, 32
    test    r8d, r8d
    je      .Lsub_bail
    cmp     r8d, 0xFFFFFFFF
    je      .Lsub_bail
    test    rdx, rdx
    je      .Lsub_bail
    mov     rax, [rsp + 24]
    test    rax, rax
    jg      .Lsub_str_nowrap
    lea     rax, [rax + r8 + 1]
.Lsub_str_nowrap:
    cmp     rax, 1
    jl      .Lsub_fail
    cmp     rax, r8
    jg      .Lsub_fail
    mov     [rsp + 40], rax
    xor     edi, edi
    mov     esi, VCELL_SIZE
    call    rt_agg_alloc
    mov     qword ptr [rax + VCELL_CELLP], 0
    mov     qword ptr [rax + VCELL_TBL], 0
    mov     qword ptr [rax + VCELL_KEY], 0
    mov     r8, [rsp + 16]
    mov     r9, [rsp + 24]
    mov     [rax + VCELL_KEY_D], r8
    mov     [rax + VCELL_KEY_D + 8], r9
    mov     r8, [rsp + 0]
    mov     r9, [rsp + 8]
    mov     [rax + VCELL_SV], r8
    mov     [rax + VCELL_SV + 8], r9
    mov     rcx, [rsp + 40]
    mov     [rax + VCELL_POS], rcx
    mov     qword ptr [rax + VCELL_LEN], 1
    add     rsp, 88
    mov     rdx, rax
    mov     rax, DT_NAMETRAP_LO | (MOD_OP_RT_SUBSCRIPT_VAR << 8)
    ret
.Lsub_array:
    test    rsi, rsi
    je      c_rt_subscript_var
    cmp     dword ptr [rsi + ARBLK_NDIM], 1
    jne     c_rt_subscript_var
    mov     eax, ecx
    mov     r10d, dword ptr [rsi + ARBLK_LO]
    sub     eax, r10d
    js      .Lsub_arr_fail
    mov     r8d, dword ptr [rsi + ARBLK_HI]
    sub     r8d, r10d
    inc     r8d
    cmp     eax, r8d
    jge     .Lsub_arr_fail
    mov     r9, [rsi + ARBLK_DATA]
    test    r9, r9
    je      c_rt_subscript_var
    movsxd  rax, eax
    shl     rax, 4
    add     rax, r9
    sub     rsp, 88
    mov     [rsp + 16], rdx
    mov     [rsp + 24], rcx
    mov     [rsp + 32], rax
    xor     edi, edi
    mov     esi, VCELL_SIZE
    call    rt_agg_alloc
    mov     rcx, [rsp + 32]
    mov     [rax + VCELL_CELLP], rcx
    mov     qword ptr [rax + VCELL_TBL], 0
    mov     qword ptr [rax + VCELL_KEY], 0
    mov     r8, [rsp + 16]
    mov     r9, [rsp + 24]
    mov     [rax + VCELL_KEY_D], r8
    mov     [rax + VCELL_KEY_D + 8], r9
    mov     qword ptr [rax + VCELL_SV], DT_FAIL | (MOD_OP_RT_SUBSCRIPT_VAR << 8)
    mov     qword ptr [rax + VCELL_SV + 8], 0
    mov     qword ptr [rax + VCELL_POS], 0
    mov     qword ptr [rax + VCELL_LEN], 0
    add     rsp, 88
    mov     rdx, rax
    mov     rax, DT_NAMETRAP_LO | (MOD_OP_RT_SUBSCRIPT_VAR << 8)
    ret
.Lsub_arr_fail:
    mov     eax, DT_FAIL | (MOD_OP_RT_SUBSCRIPT_VAR << 8)
    xor     edx, edx
    ret
.Lsub_table_direct:
    test    rsi, rsi
    je      c_rt_subscript_var
    sub     rsp, 24
    mov     [rsp + 0], rsi
    mov     [rsp + 8], rdx
    mov     [rsp + 16], rcx
    xor     edi, edi
    mov     esi, VCELL_SIZE
    call    rt_agg_alloc
    mov     qword ptr [rax + VCELL_CELLP], 0
    mov     rcx, [rsp + 0]
    mov     [rax + VCELL_TBL], rcx
    mov     qword ptr [rax + VCELL_KEY], 0
    mov     r8, [rsp + 8]
    mov     r9, [rsp + 16]
    mov     [rax + VCELL_KEY_D], r8
    mov     [rax + VCELL_KEY_D + 8], r9
    mov     qword ptr [rax + VCELL_SV], DT_FAIL | (MOD_OP_RT_SUBSCRIPT_VAR << 8)
    mov     qword ptr [rax + VCELL_SV + 8], 0
    mov     qword ptr [rax + VCELL_POS], 0
    mov     qword ptr [rax + VCELL_LEN], 0
    add     rsp, 24
    mov     rdx, rax
    mov     rax, DT_NAMETRAP_LO | (MOD_OP_RT_SUBSCRIPT_VAR << 8)
    ret
.Lsub_table_int:
    jmp     .Lsub_bail
    mov     rsi, rdx
    test    rsi, rsi
    je      .Lsub_bail
    lea     r8, [rsp + 87]
    mov     byte ptr [r8], 0
    mov     rax, [rsp + 24]
    xor     r11d, r11d
    test    rax, rax
    jns     .Lsub_ti_digits
    mov     r11d, 1
    neg     rax
.Lsub_ti_digits:
    mov     r9d, 10
.Lsub_ti_loop:
    xor     edx, edx
    div     r9
    add     dl, '0'
    dec     r8
    mov     [r8], dl
    test    rax, rax
    jnz     .Lsub_ti_loop
    test    r11d, r11d
    je      .Lsub_ti_tag
    dec     r8
    mov     byte ptr [r8], '-'
.Lsub_ti_tag:
    dec     r8
    mov     byte ptr [r8], 'i'
    dec     r8
    mov     byte ptr [r8], 1
    mov     rdi, r8
    jmp     .Lsub_hash_init
.Lsub_fail:
    add     rsp, 88
    mov     eax, DT_FAIL | (MOD_OP_RT_SUBSCRIPT_VAR << 8)
    xor     edx, edx
    ret
.Lsub_bail:
    mov     rdi, [rsp + 0]
    mov     rsi, [rsp + 8]
    mov     rdx, [rsp + 16]
    mov     rcx, [rsp + 24]
    add     rsp, 88
    jmp     c_rt_subscript_var
RTX_ENDF(rt_subscript_var)
.section .note.GNU-stack,"",@progbits
