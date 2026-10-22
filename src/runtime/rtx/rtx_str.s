#include "rtx_abi.inc"
RTX_GATE_DEF(str)
#define HBF_TTL 0x0001
#define RTX_GVA_VA 0x70001000
.macro RTX_MEMCPY
    cmp     rcx, 16
    jb      201f
    cmp     rcx, 32
    ja      200f
    movdqu  xmm0, [rsi]
    movdqu  xmm1, [rsi + rcx - 16]
    movdqu  [rdi], xmm0
    movdqu  [rdi + rcx - 16], xmm1
    jmp     209f
200:
    rep movsb
    jmp     209f
201:
    cmp     rcx, 8
    jb      202f
    mov     rax, [rsi]
    mov     r11, [rsi + rcx - 8]
    mov     [rdi], rax
    mov     [rdi + rcx - 8], r11
    jmp     209f
202:
    cmp     rcx, 4
    jb      203f
    mov     eax, [rsi]
    mov     r11d, [rsi + rcx - 4]
    mov     [rdi], eax
    mov     [rdi + rcx - 4], r11d
    jmp     209f
203:
    mov     al, [rsi]
    mov     r11b, [rsi + rcx - 1]
    mov     [rdi], al
    mov     [rdi + rcx - 1], r11b
    cmp     rcx, 3
    jb      209f
    mov     al, [rsi + 1]
    mov     [rdi + 1], al
209:
.endm
RTX_FUNC(str_concat_d)
RTX_ENTRY(sno_concat_d)
    RTX_GATE(str, .Lsc_slow)
    cmp     dil, DT_S
    jne     .Lsc_null
    cmp     dl, DT_S
    jne     .Lsc_null
    test    rsi, rsi
    je      .Lsc_slow
    test    rcx, rcx
    je      .Lsc_slow
    mov     r8, rdi
    shr     r8, 32
    je      .Lsc_slow
    cmp     r8d, -1
    je      .Lsc_slow
    mov     r9, rdx
    shr     r9, 32
    je      .Lsc_slow
    cmp     r9d, -1
    je      .Lsc_slow
    mov     rax, [rip + g_gc_pending@GOTPCREL]
    cmp     dword ptr [rax], 0
    jne     .Lsc_slow
    lea     rax, [rip + g_runtime + G_RT_OFF_SXT_FR]
    mov     r10d, dword ptr [rax + 20]
    test    r10d, r10d
    js      .Lsc_slow
    jnz     .Lsc_nosxt
    cmp     rsi, [rax + 0]
    je      .Lsc_ext
.Lsc_nosxt:
    RTX_SUB_RSP(40)
    mov     qword ptr [rsp + 32], rsi
    mov     qword ptr [rsp + 24], rcx
    mov     qword ptr [rsp + 16], r8
    mov     qword ptr [rsp + 8], r9
    lea     rdi, [r8 + r9]
    call    rt_str_alloc
    mov     r9, qword ptr [rsp + 8]
    mov     r8, qword ptr [rsp + 16]
    mov     rcx, qword ptr [rsp + 24]
    mov     rsi, qword ptr [rsp + 32]
    RTX_ADD_RSP(40)
    mov     r10, rax
    mov     rdx, rcx
    mov     rdi, rax
    mov     rcx, r8
    RTX_MEMCPY
    lea     rdi, [r10 + r8]
    mov     rsi, rdx
    mov     rcx, r9
    RTX_MEMCPY
    lea     r11, [r8 + r9]
    mov     byte ptr [r10 + r11], 0
    lea     rax, [rip + g_runtime + G_RT_OFF_SXT_FR]
    mov     qword ptr [rax + 0], 0
    mov     rdx, [rip + g_runtime@GOTPCREL]
    mov     rdx, [rdx + 0]
    lea     rsi, [r10 - 16]
    test    word ptr [rsi + 14], HBF_TTL
    je      .Lsc_ret
    cmp     word ptr [rsi + 12], DT_S
    jne     .Lsc_ret
    mov     ecx, dword ptr [rsi + 8]
    add     rcx, rsi
    cmp     rcx, rdx
    jne     .Lsc_ret
    mov     [rax + 0], r10
    mov     [rax + 8], r11
.Lsc_ret:
    mov     rax, r11
    shl     rax, 32
    or      rax, DT_S
    mov     rdx, r10
    RTX_GVA_R9
    ret
.Lsc_null:
    mov     rax, [rip + g_gc_pending@GOTPCREL]
    cmp     dword ptr [rax], 0
    jne     .Lsc_slow
    test    edi, edi
    jne     .Lsc_nb
    cmp     dl, DT_P
    je      .Lsc_slow
    cmp     dl, DT_X
    je      .Lsc_slow
    cmp     dl, DT_FAIL
    je      .Lsc_slow
    mov     rax, rdx
    mov     rdx, rcx
    RTX_GVA_R9
    ret
.Lsc_nb:
    test    edx, edx
    jne     .Lsc_chk_int
    mov     r8, rdi
    shr     r8, 32
    jne     .Lsc_nb_ok
    cmp     dil, DT_S
    je      .Lsc_slow
.Lsc_nb_ok:
    cmp     dil, DT_P
    je      .Lsc_slow
    cmp     dil, DT_X
    je      .Lsc_slow
    cmp     dil, DT_FAIL
    je      .Lsc_slow
    mov     rax, rdi
    mov     rdx, rsi
    RTX_GVA_R9
    ret
.Lsc_chk_int:
    cmp     dl, DT_I
    je      .Lsc_int_r
    cmp     dil, DT_I
    je      .Lsc_int_l
    jmp     .Lsc_slow
.Lsc_int_r:
    cmp     dil, DT_S
    jne     .Lsc_slow
    test    rsi, rsi
    je      .Lsc_slow
    mov     r8, rdi
    shr     r8, 32
    je      .Lsc_slow
    cmp     r8d, -1
    je      .Lsc_slow
    mov     rax, rcx
    mov     r8d, 1
    jmp     .Lsc_int_conv
.Lsc_int_l:
    cmp     dl, DT_S
    jne     .Lsc_slow
    test    rcx, rcx
    je      .Lsc_slow
    mov     r8, rdx
    shr     r8, 32
    je      .Lsc_slow
    cmp     r8d, -1
    je      .Lsc_slow
    mov     rax, rsi
    xor     r8d, r8d
.Lsc_int_conv:
    RTX_SUB_RSP(56)
    mov     qword ptr [rsp + 24], rdi
    mov     qword ptr [rsp + 32], rsi
    mov     qword ptr [rsp + 40], rdx
    mov     qword ptr [rsp + 48], rcx
    mov     r10, rax
    sar     r10, 63
    xor     rax, r10
    sub     rax, r10
    lea     r11, [rsp + 24]
    mov     ecx, 10
.Lsc_ic_dig:
    xor     edx, edx
    div     rcx
    add     dl, 48
    dec     r11
    mov     byte ptr [r11], dl
    test    rax, rax
    jnz     .Lsc_ic_dig
    test    r10, r10
    jz      .Lsc_ic_nosign
    dec     r11
    mov     byte ptr [r11], 45
.Lsc_ic_nosign:
    lea     r9, [rsp + 24]
    sub     r9, r11
    shl     r9, 32
    or      r9, DT_S
    test    r8d, r8d
    jz      .Lsc_ic_left
    mov     rdx, r9
    mov     rcx, r11
    mov     rdi, qword ptr [rsp + 24]
    mov     rsi, qword ptr [rsp + 32]
    jmp     .Lsc_ic_call
.Lsc_ic_left:
    mov     rdi, r9
    mov     rsi, r11
    mov     rdx, qword ptr [rsp + 40]
    mov     rcx, qword ptr [rsp + 48]
.Lsc_ic_call:
    RTX_CALL(str_concat_d)
    cmp     al, RTX_NOT_HANDLED
    jne     .Lsc_ic_ret
    mov     rdi, qword ptr [rsp + 24]
    mov     rsi, qword ptr [rsp + 32]
    mov     rdx, qword ptr [rsp + 40]
    mov     rcx, qword ptr [rsp + 48]
.Lsc_ic_ret:
    RTX_ADD_RSP(56)
    ret
.Lsc_ext:
    cmp     r8, qword ptr [rax + 8]
    jne     .Lsc_nosxt
    lea     r10, [rsi - 16]
    test    word ptr [r10 + 14], HBF_TTL
    je      .Lsc_ext_bad
    cmp     word ptr [r10 + 12], DT_S
    jne     .Lsc_ext_bad
    mov     r11d, dword ptr [r10 + 8]
    add     r11, r10
    mov     rdx, qword ptr [rip + g_runtime@GOTPCREL]
    cmp     r11, qword ptr [rdx + 0]
    jne     .Lsc_ext_bad
    mov     edi, dword ptr [rax + 16]
    mov     r11d, RTX_GVA_VA
    xor     r10d, r10d
    cmp     edi, 8
    jl      .Lsc_ext_scan
.Lsc_ext_blk:
    cmp     qword ptr [r11 + 8], rsi
    je      .Lsc_ext_hit8
    cmp     qword ptr [r11 + 24], rsi
    je      .Lsc_ext_hit8
    cmp     qword ptr [r11 + 40], rsi
    je      .Lsc_ext_hit8
    cmp     qword ptr [r11 + 56], rsi
    je      .Lsc_ext_hit8
    cmp     qword ptr [r11 + 72], rsi
    je      .Lsc_ext_hit8
    cmp     qword ptr [r11 + 88], rsi
    je      .Lsc_ext_hit8
    cmp     qword ptr [r11 + 104], rsi
    je      .Lsc_ext_hit8
    cmp     qword ptr [r11 + 120], rsi
    je      .Lsc_ext_hit8
    add     r11, 128
    sub     edi, 8
    cmp     edi, 8
    jge     .Lsc_ext_blk
    jmp     .Lsc_ext_scan
.Lsc_ext_hit8:
    mov     eax, 8
.Lsc_ext_h8:
    cmp     qword ptr [r11 + 8], rsi
    jne     .Lsc_ext_h8n
    cmp     byte ptr [r11], DT_S
    jne     .Lsc_ext_h8n
    inc     r10d
    cmp     r10d, 1
    ja      .Lsc_ext_bad
.Lsc_ext_h8n:
    add     r11, 16
    dec     eax
    jnz     .Lsc_ext_h8
    sub     edi, 8
    cmp     edi, 8
    jge     .Lsc_ext_blk
.Lsc_ext_scan:
    test    edi, edi
    jle     .Lsc_ext_have
    cmp     qword ptr [r11 + 8], rsi
    jne     .Lsc_ext_next
    cmp     byte ptr [r11], DT_S
    jne     .Lsc_ext_next
    inc     r10d
    cmp     r10d, 1
    ja      .Lsc_ext_bad
.Lsc_ext_next:
    add     r11, 16
    dec     edi
    jmp     .Lsc_ext_scan
.Lsc_ext_have:
    lea     r10, [rsi - 16]
    lea     rdi, [r8 + r9 + 16]
    and     rdi, -16
    add     rdi, 16
    mov     r11d, dword ptr [r10 + 8]
    cmp     rdi, r11
    jbe     .Lsc_ext_pad
    sub     r11, rdi
    neg     r11
    mov     rax, qword ptr [rdx + 0]
    add     rax, r11
    cmp     rax, qword ptr [rdx + 48]
    ja      .Lsc_ext_bad
    mov     qword ptr [rdx + 0], rax
    add     qword ptr [rdx + 56], r11
    add     qword ptr [rdx + 64], r11
    mov     dword ptr [r10 + 8], edi
.Lsc_ext_pad:
    mov     r10, rsi
    mov     rdx, rdi
    lea     rdi, [rsi + r8]
    mov     rsi, rcx
    mov     rcx, r9
    RTX_MEMCPY
    lea     rdi, [r10 + r8]
    add     rdi, r9
    mov     byte ptr [rdi], 0
    inc     rdi
    lea     rsi, [r10 + rdx - 16]
    sub     rsi, rdi
    jz      .Lsc_ext_done
    cmp     rsi, 8
    jb      .Lsc_ext_z4
    mov     qword ptr [rdi], 0
    mov     qword ptr [rdi + rsi - 8], 0
    jmp     .Lsc_ext_done
.Lsc_ext_z4:
    cmp     rsi, 4
    jb      .Lsc_ext_z1
    mov     dword ptr [rdi], 0
    mov     dword ptr [rdi + rsi - 4], 0
    jmp     .Lsc_ext_done
.Lsc_ext_z1:
    mov     byte ptr [rdi], 0
    mov     byte ptr [rdi + rsi - 1], 0
    cmp     rsi, 3
    jb      .Lsc_ext_done
    mov     byte ptr [rdi + 1], 0
.Lsc_ext_done:
    lea     rax, [r8 + r9]
    lea     rcx, [rip + g_runtime + G_RT_OFF_SXT_FR]
    mov     qword ptr [rcx + 8], rax
    mov     rdx, r10
    shl     rax, 32
    or      rax, DT_S
    RTX_GVA_R9
    ret
.Lsc_ext_bad:
    lea     rax, [rip + g_runtime + G_RT_OFF_SXT_FR]
    mov     qword ptr [rax + 0], 0
    jmp     .Lsc_nosxt
.Lsc_slow:
    mov     eax, RTX_NOT_HANDLED
    RTX_GVA_R9
    ret
RTX_ENDF(str_concat_d)
RTX_FUNC(str_concat_fracdigit_d)
    cmp     dil, DT_S
    je      .Lscf_a
    cmp     dil, DT_I
    jne     .Lscf_slow
.Lscf_a:
    cmp     dl, DT_S
    je      .Lscf_b
    cmp     dl, DT_I
    jne     .Lscf_slow
.Lscf_b:
    RTX_JMP(str_concat_d)
.Lscf_slow:
    RTX_DECLINE
RTX_ENDF(str_concat_fracdigit_d)
RTX_FUNC(VARVAL_fn)
    RTX_GATE(str, .Lvv_c)
    cmp     dil, DT_S
    jne     .Lvv_c
    test    rsi, rsi
    jz      .Lvv_c
    mov     rax, rdi
    shr     rax, 32
    je      .Lvv_raw
    cmp     eax, -1
    je      .Lvv_raw
    cmp     byte ptr [rsi + rax], 0
    jne     .Lvv_c
.Lvv_raw:
    mov     rax, rsi
    ret
.Lvv_c:
    RTX_SUB_RSP(8)
    call    c_VARVAL_fn
    RTX_ADD_RSP(8)
    RTX_GVA_R9
    ret
RTX_ENDF(VARVAL_fn)
RTX_FUNC(rt_translate_bytes)
    RTX_GATE(str, .Ltrb_c)
    test    rdx, rdx
    je      .Ltrb_done
.Ltrb_loop:
    movzx   eax, byte ptr [rsi]
    movzx   eax, byte ptr [rcx + rax]
    mov     [rdi], al
    inc     rsi
    inc     rdi
    dec     rdx
    jnz     .Ltrb_loop
.Ltrb_done:
    ret
.Ltrb_c:
    RTX_SUB_RSP(8)
    call    c_rt_translate_bytes
    RTX_ADD_RSP(8)
    RTX_GVA_R9
    ret
RTX_ENDF(rt_translate_bytes)
.section .note.GNU-stack,"",@progbits
RTX_FUNC(rt_substr)
    RTX_PUSHS(rbx)
    RTX_PUSHS(r12)
    RTX_SUB_RSP(8)
    mov     rbx, rdi
    test    rbx, rbx
    jne     .Lss_sig
    lea     rbx, [rip + .Lss_empty]
.Lss_sig:
    mov     rax, rsi
    mov     rcx, rdx
    cmp     rax, rcx
    jle     .Lss_ord
    xchg    rax, rcx
.Lss_ord:
    add     rbx, rax
    sub     rcx, rax
    mov     r12, rcx
    mov     rdi, rcx
    RTX_CALL(rt_str_alloc)
    mov     rdi, rax
    mov     rdx, rax
    mov     rsi, rbx
    mov     rcx, r12
    rep movsb
    mov     byte ptr [rdi], 0
    mov     rax, r12
    shl     rax, 32
    or      rax, DT_S
    RTX_ADD_RSP(8)
    RTX_POPS(r12)
    RTX_POPS(rbx)
    ret
RTX_ENDF(rt_substr)
.section .rodata
.Lss_empty:
    .asciz ""
.text
