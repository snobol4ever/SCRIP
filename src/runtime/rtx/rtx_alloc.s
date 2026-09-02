#include "rtx_abi.inc"
RTX_GATE_DEF(alloc)
#define HBF_TTL 0x0001
#define HB_AGGV 206
#define HB_WS   203
RTX_FUNC(rt_gcheap_alloc)
    RTX_GATE(alloc, c_rt_gcheap_alloc)
    mov     r10, [rip + g_hp_fr@GOTPCREL]
    mov     eax, dword ptr [r10 + 24]
    test    eax, eax
    je      .Lga_slow
.Lga_armed:
    mov     eax, dword ptr [r10 + 40]
    test    eax, eax
    jne     .Lga_slow
    lea     rcx, [rsi + 15]
    and     rcx, -16
    add     rcx, 16
    mov     r11, [r10 + 0]
    mov     rax, r11
    add     rax, rcx
    jc      .Lga_slow
    cmp     rax, [r10 + 8]
    ja      .Lga_slow
    cmp     r11, [r10 + 32]
    jb      .Lga_slow
    mov     qword ptr [r11 + 0], 0
    mov     dword ptr [r11 + 8], ecx
    movzx   ecx, di
    or      ecx, HBF_TTL << 16
    mov     dword ptr [r11 + 12], ecx
    mov     [r10 + 32], rax
    mov     [r10 + 0], rax
    add     qword ptr [r10 + 16], 1
    lea     rax, [r11 + 16]
    ret
.Lga_slow:
    jmp     c_rt_gcheap_alloc
RTX_ENDF(rt_gcheap_alloc)
RTX_FUNC(rt_str_alloc)
    RTX_GATE(alloc, c_rt_str_alloc)
    mov     r10, [rip + g_hp_fr@GOTPCREL]
    mov     eax, dword ptr [r10 + 24]
    test    eax, eax
    je      c_rt_str_alloc
    xor     eax, eax
    test    rdi, rdi
    mov     rsi, rdi
    cmovs   rsi, rax
    inc     rsi
    mov     edi, DT_S
    jmp     .Lga_armed
RTX_ENDF(rt_str_alloc)
RTX_FUNC(rt_agg_alloc)
    RTX_GATE(alloc, c_rt_agg_alloc)
    mov     r10, [rip + g_hp_fr@GOTPCREL]
    mov     eax, dword ptr [r10 + 24]
    test    eax, eax
    je      c_rt_agg_alloc
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
RTX_ENDF(rt_agg_alloc)
RTX_FUNC(rt_ws_alloc)
    RTX_GATE(alloc, c_rt_ws_alloc)
    cmp     dword ptr [rip + g_ah_on], 0
    jne     c_rt_ws_alloc
    cmp     qword ptr [rip + g_wsi_base], 0
    je      c_rt_ws_alloc
    mov     r10, [rip + g_hp_fr@GOTPCREL]
    cmp     dword ptr [r10 + 40], 0
    jne     c_rt_ws_alloc
    mov     rcx, 1
    test    rdi, rdi
    cmovnz  rcx, rdi
    add     rcx, 15
    and     rcx, -16
    add     rcx, 16
    mov     r9, [rip + g_wsi_ws]
    mov     rdx, [rip + g_wsi_wss]
    sub     rdx, r9
    cmp     rdx, rcx
    jb      c_rt_ws_alloc
    mov     qword ptr [r9 + 0], 0
    mov     dword ptr [r9 + 8], ecx
    mov     dword ptr [r9 + 12], (HB_WS | (HBF_TTL << 16))
    lea     r10, [r9 + rcx]
    mov     [rip + g_wsi_ws], r10
    add     qword ptr [rip + g_wsi_blocks], 1
    lea     rax, [r9 + 16]
    ret
RTX_ENDF(rt_ws_alloc)
.section .note.GNU-stack,"",@progbits
