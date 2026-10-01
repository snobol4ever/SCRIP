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
    RTX_CTAIL(c_rt_gcheap_alloc)
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
    RTX_CTAIL(c_rt_str_alloc)
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
    RTX_CTAIL(c_rt_agg_alloc)
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
    RTX_SAVE
    push    rbx
    push    r12
    push    r13
    push    r14
    push    r15
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
    pop     r15
    pop     r14
    pop     r13
    pop     r12
    pop     rbx
    RTX_RET
RTX_ENDF(rt_gc_index_run)
RTX_FUNC(rt_gc_forward_run)
    push    rbx
    push    r12
    push    r13
    push    r14
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
    pop     r14
    pop     r13
    pop     r12
    pop     rbx
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
.section .note.GNU-stack,"",@progbits
