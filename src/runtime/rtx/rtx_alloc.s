#include "rtx_abi.inc"
RTX_GATE_DEF(alloc)
#define HBF_TTL 0x0001
#define HB_AGGV 206
#define HB_WSC 205
RTX_FUNC(rt_gcheap_alloc)
    RTX_GATE(alloc, .Lga_c)
    mov     rdx, [rip + g_hp_fr@GOTPCREL]
    mov     eax, dword ptr [rdx + 24]
    test    eax, eax
    je      .Lga_c
.Lga_armed:
    mov     eax, dword ptr [rdx + 40]
    test    eax, eax
    jne     .Lga_c
    lea     rcx, [rsi + 15]
    and     rcx, -16
    add     rcx, 16
    mov     rax, [rdx + 0]
    add     rax, rcx
    jc      .Lga_c
    cmp     rax, [rdx + 48]
    ja      .Lga_c
    mov     rsi, rax
    sub     rsi, rcx
    mov     [rdx + 0], rax
    add     qword ptr [rdx + 56], rcx
#if RT_DIAG
    add     qword ptr [rdx + 16], 1
#endif
    cmp     di, DT_S
    jne     .Lga_counted
    add     qword ptr [rdx + 64], rcx
.Lga_counted:
    mov     qword ptr [rsi + 0], 0
    mov     dword ptr [rsi + 8], ecx
    movzx   eax, di
    or      eax, HBF_TTL << 16
    mov     dword ptr [rsi + 12], eax
    cmp     rsi, [rdx + 32]
    jb      .Lga_reused
    lea     rax, [rsi + rcx]
    mov     [rdx + 32], rax
    lea     rax, [rsi + 16]
    ret
.Lga_reused:
    lea     rax, [rsi + rcx]
    cmp     rax, [rdx + 32]
    jbe     .Lga_vkept
    mov     [rdx + 32], rax
.Lga_vkept:
    cmp     di, DT_S
    je      .Lga_ztail
    cmp     di, HB_WSC
    jne     .Lga_zfull
.Lga_ztail:
    cmp     rcx, 48
    jbe     .Lga_zfull
    lea     rdi, [rsi + rcx - 32]
    mov     qword ptr [rdi + 0], 0
    mov     qword ptr [rdi + 8], 0
    mov     qword ptr [rdi + 16], 0
    mov     qword ptr [rdi + 24], 0
    lea     rax, [rsi + 16]
    ret
.Lga_zfull:
    lea     rdi, [rsi + 16]
    lea     rcx, [rcx - 16]
    shr     rcx, 3
    xor     eax, eax
    cmp     rcx, 32
    ja      .Lga_zrep
.Lga_zloop:
    mov     qword ptr [rdi], rax
    add     rdi, 8
    sub     rcx, 1
    jnz     .Lga_zloop
    lea     rax, [rsi + 16]
    ret
.Lga_zrep:
    rep stosq
    lea     rax, [rsi + 16]
    ret
.Lga_c:
    RTX_SUB_RSP(8)
    call    c_rt_gcheap_alloc
    RTX_ADD_RSP(8)
    RTX_GVA_R9
    ret
RTX_ENDF(rt_gcheap_alloc)
RTX_FUNC(rt_str_alloc)
    RTX_GATE(alloc, .Lsa_c)
    mov     rdx, [rip + g_hp_fr@GOTPCREL]
    mov     eax, dword ptr [rdx + 24]
    test    eax, eax
    je      .Lsa_c
    xor     eax, eax
    test    rdi, rdi
    mov     rsi, rdi
    cmovs   rsi, rax
    inc     rsi
    mov     edi, DT_S
    jmp     .Lga_armed
.Lsa_c:
    RTX_SUB_RSP(8)
    call    c_rt_str_alloc
    RTX_ADD_RSP(8)
    RTX_GVA_R9
    ret
RTX_ENDF(rt_str_alloc)
RTX_FUNC(rt_agg_alloc)
    RTX_GATE(alloc, .Laa_c)
    mov     rdx, [rip + g_hp_fr@GOTPCREL]
    mov     eax, dword ptr [rdx + 24]
    test    eax, eax
    je      .Laa_c
    mov     eax, edi
    xor     ecx, ecx
    test    eax, eax
    cmovs   eax, ecx
    mov     ecx, 2
    cmp     eax, 2
    cmovg   eax, ecx
    add     eax, HB_AGGV
    mov     edi, eax
    mov     rcx, 1
    test    rsi, rsi
    cmove   rsi, rcx
    jmp     .Lga_armed
.Laa_c:
    RTX_SUB_RSP(8)
    call    c_rt_agg_alloc
    RTX_ADD_RSP(8)
    RTX_GVA_R9
    ret
RTX_ENDF(rt_agg_alloc)
RTX_FUNC(rt_gc_fix_slots)
    test    rsi, rsi
    jle     .Lfx_done
.Lfx_loop:
    mov     rax, qword ptr [rdi + 8]
    cmp     rax, rdx
    jb      .Lfx_next
    mov     rcx, qword ptr [rax]
    test    rcx, rcx
    je      .Lfx_next
    cmp     rcx, rax
    je      .Lfx_next
    add     rcx, qword ptr [rdi + 16]
    mov     rax, qword ptr [rdi]
    add     rcx, 16
    mov     qword ptr [rax], rcx
.Lfx_next:
    add     rdi, 24
    dec     rsi
    jne     .Lfx_loop
.Lfx_done:
    ret
RTX_ENDF(rt_gc_fix_slots)
RTX_FUNC(rt_gc_index_run)
    RTX_PUSHS(rbx)
    RTX_PUSHS(r12)
    RTX_PUSHS(r13)
    RTX_PUSHS(r14)
    RTX_PUSHS(r15)
    mov     rsi, qword ptr [rdi + 0]
    mov     r12, qword ptr [rdi + 8]
    mov     r13, qword ptr [rdi + 16]
    mov     r14, qword ptr [rdi + 24]
    mov     rcx, qword ptr [rdi + 32]
    mov     r8, qword ptr [rdi + 40]
    mov     r15, qword ptr [rdi + 48]
    mov     rbx, qword ptr [rdi + 56]
.Lix_loop:
    cmp     rsi, r12
    jae     .Lix_done
    cmp     rcx, r8
    jge     .Lix_done
    and     word ptr [rsi + 14], 0xFFED
    mov     qword ptr [rsi], 0
    mov     qword ptr [r14 + rcx*8], rsi
    inc     rcx
    mov     edx, dword ptr [rsi + 8]
    add     rdx, rsi
    mov     rax, rsi
    sub     rax, r13
    lea     r9, [rax + 63]
    shr     r9, 6
    shr     rax, 3
    mov     r10, rdx
    sub     r10, r13
    add     r10, 63
    shr     r10, 6
    cmp     r9, r10
    jae     .Lix_next
    mov     r11, r10
    sub     r11, r9
    add     rbx, r11
.Lix_fill:
    mov     dword ptr [r15 + r9*4], eax
    inc     r9
    cmp     r9, r10
    jb      .Lix_fill
.Lix_next:
    mov     rsi, rdx
    jmp     .Lix_loop
.Lix_done:
    mov     qword ptr [rdi + 0], rsi
    mov     qword ptr [rdi + 32], rcx
    mov     qword ptr [rdi + 56], rbx
    RTX_POPS(r15)
    RTX_POPS(r14)
    RTX_POPS(r13)
    RTX_POPS(r12)
    RTX_POPS(rbx)
    RTX_GVA_R9
    ret
RTX_ENDF(rt_gc_index_run)
RTX_FUNC(rt_gc_forward_run)
    RTX_PUSHS(rbx)
    RTX_PUSHS(r12)
    RTX_PUSHS(r13)
    RTX_PUSHS(r14)
    mov     r12, qword ptr [rdx]
    xor     eax, eax
    xor     r13d, r13d
    test    rsi, rsi
    jle     .Lfw_done
.Lfw_loop:
    mov     rbx, qword ptr [rdi + r13*8]
    test    word ptr [rbx + 14], 2
    je      .Lfw_dead
    cmp     r12, rbx
    je      .Lfw_live
    cmp     qword ptr [rdx + 8], 0
    jne     .Lfw_live
    mov     qword ptr [rdx + 8], rbx
.Lfw_live:
    mov     qword ptr [rbx], r12
    mov     qword ptr [rcx + rax*8], rbx
    mov     qword ptr [r8 + rax*8], r12
    inc     rax
    mov     r14d, dword ptr [rbx + 8]
    add     r12, r14
    jmp     .Lfw_next
.Lfw_dead:
    mov     qword ptr [rbx], 0
.Lfw_next:
    inc     r13
    cmp     r13, rsi
    jl      .Lfw_loop
.Lfw_done:
    mov     qword ptr [rdx], r12
    RTX_POPS(r14)
    RTX_POPS(r13)
    RTX_POPS(r12)
    RTX_POPS(rbx)
    ret
RTX_ENDF(rt_gc_forward_run)
RTX_FUNC(rt_gc_reset_run)
    test    rsi, rsi
    jle     .Lrs_done
.Lrs_loop:
    mov     rax, qword ptr [rdi]
    mov     qword ptr [rax], 0
    movzx   edx, word ptr [rax + 14]
    or      edx, 1
    and     edx, 0xFFED
    mov     word ptr [rax + 14], dx
    add     rdi, 8
    dec     rsi
    jne     .Lrs_loop
.Lrs_done:
    ret
RTX_ENDF(rt_gc_reset_run)
#define HB_FILL 202
#define HB_DINST 213
#define HB_DVEC 214
#define MK_MOVES 0x7FDE7
#define DATA_ELEMS_SLEN 1
#define PROCVAL_SLEN 0xFFFFFFFE
#define PROCVAL_BUILTIN_SLEN 0xFFFFFFFC
#define PROCVAL_EXTERNAL_SLEN 0xFFFFFFFB
#define MK_PLAIN_I 1
#define MK_DATA 2
#define MK_PLAIN 3
#define MK_E 4
#define MK_N 5
#define MK_SLOW 6
.macro MK_BLK_OF p
    xor     eax, eax
    cmp     \p, r12
    jb      309f
    cmp     \p, r13
    jae     309f
    cmp     \p, qword ptr [rdi + 64]
    jae     305f
    mov     rdx, \p
    sub     rdx, r12
    shr     rdx, 6
    mov     eax, dword ptr [r14 + rdx*4]
    lea     rax, [r12 + rax*8]
301:
    mov     edx, dword ptr [rax + 8]
    test    edx, edx
    je      305f
    add     rdx, rax
    cmp     rdx, \p
    ja      309f
    mov     rax, rdx
    jmp     301b
305:
    mov     qword ptr [rdi + 152], 0
    mov     rax, qword ptr [rdi + 88]
    dec     rax
    mov     qword ptr [rdi + 160], rax
306:
    mov     rax, qword ptr [rdi + 152]
    cmp     rax, qword ptr [rdi + 160]
    jg      308f
    add     rax, qword ptr [rdi + 160]
    sar     rax, 1
    mov     rdx, qword ptr [rdi + 80]
    mov     rbp, qword ptr [rdx + rax*8]
    cmp     \p, rbp
    jb      307f
    mov     edx, dword ptr [rbp + 8]
    add     rdx, rbp
    cmp     \p, rdx
    jb      304f
    inc     rax
    mov     qword ptr [rdi + 152], rax
    jmp     306b
307:
    dec     rax
    mov     qword ptr [rdi + 160], rax
    jmp     306b
304:
    mov     rax, rbp
    jmp     309f
308:
    xor     eax, eax
309:
.endm
.macro MK_MARK_BLK
    movzx   edx, word ptr [rax + 12]
    cmp     edx, HB_FILL
    je      319f
    test    word ptr [rax + 14], 2
    jne     319f
    or      word ptr [rax + 14], 2
    sub     edx, HB_AGGV
    cmp     edx, 18
    ja      319f
    mov     ebp, MK_MOVES
    bt      ebp, edx
    jnc     319f
    mov     qword ptr [rax], r15
    mov     r15, rax
319:
.endm
.macro MK_SLOT loc
    lea     rdx, [rbx + rbx*2]
    lea     rdx, [r11 + rdx*8]
    mov     qword ptr [rdx + 8], rax
    sub     r10, rax
    sub     r10, 16
    mov     qword ptr [rdx + 16], r10
    lea     r10, [\loc]
    mov     qword ptr [rdx], r10
    inc     rbx
.endm
.macro MK_INTERIOR
#if RT_DIAG
    lea     rdx, [rax + 16]
    cmp     rdx, r10
    je      329f
    inc     qword ptr [rdi + 128]
329:
#endif
.endm
RTX_FUNC(rt_gc_mark_run)
    RTX_PUSHS(rbx)
    RTX_PUSHS(rbp)
    RTX_PUSHS(r12)
    RTX_PUSHS(r13)
    RTX_PUSHS(r14)
    RTX_PUSHS(r15)
    mov     r8, qword ptr [rdi + 0]
    mov     rcx, qword ptr [rdi + 8]
    mov     r11, qword ptr [rdi + 24]
    mov     rbx, qword ptr [rdi + 32]
    mov     r12, qword ptr [rdi + 48]
    mov     r13, qword ptr [rdi + 56]
    mov     r14, qword ptr [rdi + 72]
    mov     r15, qword ptr [rdi + 96]
    cmp     qword ptr [rdi + 112], 0
    jne     .Lmk_range
.Lmk_loop:
    mov     rax, qword ptr [rdi + 40]
    sub     rax, rbx
    cmp     rax, 3
    jl      .Lmk_grow
    test    rcx, rcx
    je      .Lmk_empty
    dec     rcx
    mov     rsi, qword ptr [r8 + rcx*8]
    movzx   eax, byte ptr [rsi]
    lea     rdx, [rip + .Lmk_class]
    movzx   eax, byte ptr [rdx + rax]
    cmp     eax, MK_PLAIN_I
    je      .Lmk_plain_i
    cmp     eax, MK_DATA
    je      .Lmk_data
    cmp     eax, MK_PLAIN
    je      .Lmk_plain
    cmp     eax, MK_E
    je      .Lmk_e
    cmp     eax, MK_N
    je      .Lmk_n
    cmp     eax, MK_SLOW
    je      .Lmk_slow
    jmp     .Lmk_loop
.Lmk_e:
    mov     eax, dword ptr [rsi + 4]
    cmp     eax, PROCVAL_SLEN
    je      .Lmk_plain_i
    cmp     eax, PROCVAL_BUILTIN_SLEN
    je      .Lmk_plain_i
    cmp     eax, PROCVAL_EXTERNAL_SLEN
    je      .Lmk_plain_i
    jmp     .Lmk_loop
.Lmk_n:
    mov     eax, dword ptr [rsi + 4]
    cmp     eax, 2
    je      .Lmk_slow
    cmp     eax, 1
    je      .Lmk_slow
.Lmk_plain:
    mov     r10, qword ptr [rsi + 8]
    MK_BLK_OF r10
    test    rax, rax
    je      .Lmk_loop
    MK_MARK_BLK
    MK_SLOT rsi+8
    jmp     .Lmk_loop
.Lmk_plain_i:
    mov     r10, qword ptr [rsi + 8]
    MK_BLK_OF r10
    test    rax, rax
    je      .Lmk_loop
    MK_INTERIOR
    MK_MARK_BLK
    MK_SLOT rsi+8
    jmp     .Lmk_loop
.Lmk_data:
    cmp     dword ptr [rsi + 4], DATA_ELEMS_SLEN
    jne     .Lmk_dinst
    mov     r10, qword ptr [rsi + 8]
    MK_BLK_OF r10
    test    rax, rax
    je      .Lmk_loop
#if RT_DIAG
    inc     qword ptr [rdi + 136]
    cmp     word ptr [rax + 12], HB_DVEC
    je      .Lmk_elems_dvec
    inc     qword ptr [rdi + 144]
.Lmk_elems_dvec:
#endif
    MK_MARK_BLK
    MK_SLOT rsi+8
    jmp     .Lmk_loop
.Lmk_dinst:
    mov     r10, qword ptr [rsi + 8]
    cmp     r10, 16
    jb      .Lmk_loop
    test    r10b, 7
    jne     .Lmk_loop
    MK_BLK_OF r10
    test    rax, rax
    je      .Lmk_loop
    lea     rdx, [rax + 16]
    cmp     rdx, r10
    jne     .Lmk_loop
    cmp     word ptr [rax + 12], HB_DINST
    jne     .Lmk_loop
    test    word ptr [rax + 14], HBF_TTL
    je      .Lmk_loop
    movzx   ebp, word ptr [rax + 14]
    or      word ptr [rax + 14], 0x12
    MK_SLOT rsi+8
    test    ebp, 0x10
    jne     .Lmk_loop
    mov     rsi, qword ptr [rsi + 8]
    cmp     qword ptr [rsi + 8], 0
    je      .Lmk_loop
    cmp     qword ptr [rsi], 0
    je      .Lmk_loop
    mov     r10, qword ptr [rsi]
    MK_BLK_OF r10
    test    rax, rax
    je      .Lmk_dtype_done
    MK_MARK_BLK
    MK_SLOT rsi
.Lmk_dtype_done:
    mov     r10, qword ptr [rsi + 8]
    MK_BLK_OF r10
    test    rax, rax
    je      .Lmk_dfields
    cmp     word ptr [rax + 12], HB_DVEC
    jne     .Lmk_dflat
    movzx   ebp, word ptr [rax + 14]
    or      word ptr [rax + 14], 0x12
    MK_SLOT rsi+8
    test    ebp, 0x10
    jne     .Lmk_loop
    lea     rdx, [rax + 16]
    mov     qword ptr [rdi + 104], rdx
    mov     edx, dword ptr [rax + 8]
    sub     rdx, 16
    shr     rdx, 4
    mov     qword ptr [rdi + 112], rdx
    jmp     .Lmk_range
.Lmk_dflat:
    MK_MARK_BLK
    MK_SLOT rsi+8
.Lmk_dfields:
    mov     rdx, qword ptr [rsi + 8]
    mov     qword ptr [rdi + 104], rdx
    mov     rdx, qword ptr [rsi]
    movsxd  rdx, dword ptr [rdx + 8]
    test    rdx, rdx
    jle     .Lmk_loop
    mov     qword ptr [rdi + 112], rdx
.Lmk_range:
    mov     rsi, qword ptr [rdi + 104]
    mov     rdx, qword ptr [rdi + 112]
    lea     rbp, [rip + .Lmk_class]
.Lmk_range_loop:
    test    rdx, rdx
    je      .Lmk_range_done
    movzx   eax, byte ptr [rsi]
    cmp     byte ptr [rbp + rax], 0
    je      .Lmk_range_next
    cmp     rcx, qword ptr [rdi + 16]
    je      .Lmk_range_full
    mov     qword ptr [r8 + rcx*8], rsi
    inc     rcx
#if RT_DIAG
    cmp     rcx, qword ptr [rdi + 120]
    jle     .Lmk_range_next
    mov     qword ptr [rdi + 120], rcx
#endif
.Lmk_range_next:
    add     rsi, 16
    dec     rdx
    jmp     .Lmk_range_loop
.Lmk_range_done:
    mov     qword ptr [rdi + 112], 0
    jmp     .Lmk_loop
.Lmk_range_full:
    mov     qword ptr [rdi + 104], rsi
    mov     qword ptr [rdi + 112], rdx
.Lmk_grow:
    mov     eax, 1
    jmp     .Lmk_out
.Lmk_slow:
    mov     rax, rsi
    jmp     .Lmk_out
.Lmk_empty:
    xor     eax, eax
.Lmk_out:
    mov     qword ptr [rdi + 8], rcx
    mov     qword ptr [rdi + 32], rbx
    mov     qword ptr [rdi + 96], r15
    RTX_POPS(r15)
    RTX_POPS(r14)
    RTX_POPS(r13)
    RTX_POPS(r12)
    RTX_POPS(rbp)
    RTX_POPS(rbx)
    ret
RTX_ENDF(rt_gc_mark_run)
.section .rodata
.p2align 6
.Lmk_class:
.org .Lmk_class + DT_SNUL
.byte MK_PLAIN_I
.org .Lmk_class + DT_S
.byte MK_PLAIN_I
.org .Lmk_class + DT_P
.byte MK_PLAIN
.org .Lmk_class + DT_A
.byte MK_SLOW
.org .Lmk_class + DT_T
.byte MK_SLOW
.org .Lmk_class + DT_N
.byte MK_N
.org .Lmk_class + DT_E
.byte MK_E
.org .Lmk_class + DT_PLVAR
.byte MK_PLAIN_I
.org .Lmk_class + DT_PLREF
.byte MK_PLAIN_I
.org .Lmk_class + DT_X
.byte MK_PLAIN_I
.org .Lmk_class + DT_DATA
.byte MK_DATA
.org .Lmk_class + DT_BIG
.byte MK_PLAIN
.org .Lmk_class + DT_CPLX
.byte MK_PLAIN
.org .Lmk_class + DT_EXTL
.byte MK_PLAIN
.org .Lmk_class + 256
.text
.section .note.GNU-stack,"",@progbits
