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
    add     qword ptr [rdx + 16], 1
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
.section .note.GNU-stack,"",@progbits
