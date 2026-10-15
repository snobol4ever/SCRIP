#include "rtx_abi.inc"
#define HBF_TTL 0x0001
#define HB_FILL 202
#define HB_AGGV 206
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
    cmp     \p, qword ptr [rdi + 56]
    jae     309f
    mov     rdx, \p
    sub     rdx, r12
    shr     rdx, 4
    mov     ecx, edx
    and     ecx, 63
    shr     rdx, 6
    mov     rbp, -2
    shl     rbp, cl
    not     rbp
    and     rbp, qword ptr [r14 + rdx*8]
    jne     303f
    test    rdx, rdx
    je      309f
    dec     rdx
    mov     ecx, edx
    and     ecx, 63
    mov     rax, rdx
    shr     rax, 6
    mov     rbp, -2
    shl     rbp, cl
    not     rbp
    mov     r13, qword ptr [rdi + 64]
    and     rbp, qword ptr [r13 + rax*8]
    jne     302f
301:
    test    rax, rax
    je      308f
    dec     rax
    mov     rbp, qword ptr [r13 + rax*8]
    test    rbp, rbp
    je      301b
302:
    bsr     rbp, rbp
    shl     rax, 6
    add     rax, rbp
    mov     rdx, rax
    mov     rbp, qword ptr [r14 + rdx*8]
303:
    bsr     rbp, rbp
    shl     rdx, 6
    add     rdx, rbp
    shl     rdx, 4
    lea     rax, [r12 + rdx]
    mov     edx, dword ptr [rax + 8]
    add     rdx, rax
    cmp     \p, rdx
    jb      309f
308:
    xor     eax, eax
309:
.endm
.macro MK_LIVE_SET
    mov     edx, dword ptr [rax + 8]
    shr     edx, 4
    mov     ebp, 64
    sub     ebp, ecx
    cmp     edx, ebp
    ja      312f
    cmp     edx, 64
    je      311f
    mov     rbp, 1
    xchg    ecx, edx
    shl     rbp, cl
    dec     rbp
    xchg    ecx, edx
    shl     rbp, cl
    or      qword ptr [r13], rbp
    jmp     315f
311:
    or      qword ptr [r13], -1
    jmp     315f
312:
    mov     rbp, -1
    shl     rbp, cl
    or      qword ptr [r13], rbp
    neg     ecx
    add     ecx, 64
    sub     edx, ecx
313:
    cmp     edx, 64
    jb      314f
    add     r13, 8
    mov     qword ptr [r13], -1
    sub     edx, 64
    jmp     313b
314:
    test    edx, edx
    je      315f
    add     r13, 8
    mov     ecx, edx
    mov     rbp, 1
    shl     rbp, cl
    dec     rbp
    or      qword ptr [r13], rbp
315:
.endm
.macro MK_MARK_BLK
    movzx   edx, word ptr [rax + 12]
    cmp     edx, HB_FILL
    je      319f
    mov     rdx, rax
    sub     rdx, r12
    shr     rdx, 4
    mov     ecx, edx
    and     ecx, 63
    shr     rdx, 6
    mov     r13, qword ptr [rdi + 80]
    lea     r13, [r13 + rdx*8]
    bt      qword ptr [r13], rcx
    jc      319f
    MK_LIVE_SET
    inc     qword ptr [rdi + 152]
    mov     edx, dword ptr [rax + 8]
    cmp     rdx, qword ptr [rdi + 160]
    jle     318f
    mov     qword ptr [rdi + 160], rdx
318:
    movzx   edx, word ptr [rax + 12]
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
.macro MK_MARK_VIS
    mov     rdx, rax
    sub     rdx, r12
    shr     rdx, 4
    mov     ecx, edx
    and     ecx, 63
    shr     rdx, 6
    mov     r13, qword ptr [rdi + 80]
    lea     r13, [r13 + rdx*8]
    bt      qword ptr [r13], rcx
    jnc     324f
    mov     ebp, 2
    jmp     325f
324:
    MK_LIVE_SET
    inc     qword ptr [rdi + 152]
    mov     edx, dword ptr [rax + 8]
    cmp     rdx, qword ptr [rdi + 160]
    jle     323f
    mov     qword ptr [rdi + 160], rdx
323:
    xor     ebp, ebp
325:
    mov     rdx, rax
    sub     rdx, r12
    shr     rdx, 4
    mov     ecx, edx
    and     ecx, 63
    shr     rdx, 6
    mov     r13, qword ptr [rdi + 88]
    bt      qword ptr [r13 + rdx*8], rcx
    jnc     326f
    or      ebp, 0x10
326:
    bts     qword ptr [r13 + rdx*8], rcx
.endm
.macro MK_SLOT loc
    mov     rdx, qword ptr [rdi + 24]
    lea     rcx, [rbx + rbx]
    lea     rdx, [rdx + rcx*8]
    mov     qword ptr [rdx + 8], r10
    lea     rcx, [\loc]
    mov     qword ptr [rdx], rcx
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
    mov     r11, qword ptr [rdi + 8]
    mov     rbx, qword ptr [rdi + 32]
    mov     r12, qword ptr [rdi + 48]
    mov     r14, qword ptr [rdi + 72]
    mov     r15, qword ptr [rdi + 96]
    cmp     qword ptr [rdi + 112], 0
    jne     .Lmk_range
.Lmk_loop:
    mov     rax, qword ptr [rdi + 40]
    sub     rax, rbx
    cmp     rax, 3
    jl      .Lmk_grow
    test    r11, r11
    je      .Lmk_empty
    dec     r11
    mov     rsi, qword ptr [r8 + r11*8]
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
    MK_MARK_VIS
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
    MK_MARK_VIS
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
    cmp     r11, qword ptr [rdi + 16]
    je      .Lmk_range_full
    mov     qword ptr [r8 + r11*8], rsi
    inc     r11
#if RT_DIAG
    cmp     r11, qword ptr [rdi + 120]
    jle     .Lmk_range_next
    mov     qword ptr [rdi + 120], r11
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
    mov     qword ptr [rdi + 8], r11
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
.macro GC_POPCNT dst, src
    popcnt  \dst, \src
.endm
.macro GC_PEXT dst, src, mask
    pext    \dst, \src, \mask
.endm
RTX_FUNC(rt_gc_first_dead_run)
    xor     ecx, ecx
    lea     r8, [rsi + 63]
    shr     r8, 6
.Lfd_loop:
    cmp     rcx, r8
    jae     .Lfd_top
    mov     rax, qword ptr [rdi + rcx*8]
    not     rax
    test    rax, rax
    jne     .Lfd_hit
    inc     rcx
    jmp     .Lfd_loop
.Lfd_hit:
    bsf     rax, rax
    shl     rcx, 6
    add     rax, rcx
    cmp     rax, rsi
    jb      .Lfd_out
.Lfd_top:
    mov     rax, rsi
.Lfd_out:
    ret
RTX_ENDF(rt_gc_first_dead_run)
RTX_FUNC(rt_gc_prefix_run)
    xor     eax, eax
    xor     ecx, ecx
.Lpf_loop:
    cmp     rcx, rdx
    jae     .Lpf_out
    mov     qword ptr [rdi + rcx*8], rax
    mov     r8, qword ptr [rsi + rcx*8]
    GC_POPCNT r8, r8
    shl     r8, 4
    add     rax, r8
    inc     rcx
    jmp     .Lpf_loop
.Lpf_out:
    ret
RTX_ENDF(rt_gc_prefix_run)
RTX_FUNC(rt_gc_fix_run)
    RTX_PUSHS(rbx)
    RTX_PUSHS(rbp)
    RTX_PUSHS(r12)
    mov     r12, rdx
    mov     rbp, rcx
    mov     r10, qword ptr [r8 + 72]
    mov     r11, qword ptr [r8 + 112]
    shl     rsi, 4
    add     rsi, rdi
.Lfx_loop:
    cmp     rdi, rsi
    jae     .Lfx_out
    mov     rax, qword ptr [rdi + 8]
    sub     rax, r9
    mov     r8, rax
    shr     r8, 4
    cmp     r8, rbp
    jb      .Lfx_next
    mov     ecx, r8d
    and     ecx, 63
    shr     r8, 6
    mov     rbx, qword ptr [r10 + r8*8]
    bt      rbx, rcx
    jnc     .Lfx_next
    mov     rdx, -1
    shl     rdx, cl
    not     rdx
    and     rdx, rbx
    GC_POPCNT rdx, rdx
    shl     rdx, 4
    add     rdx, qword ptr [r11 + r8*8]
    and     eax, 15
    add     rdx, rax
    add     rdx, r12
    mov     rbx, qword ptr [rdi]
    mov     qword ptr [rbx], rdx
.Lfx_next:
    add     rdi, 16
    jmp     .Lfx_loop
.Lfx_out:
    RTX_POPS(r12)
    RTX_POPS(rbp)
    RTX_POPS(rbx)
    RTX_GVA_R9
    ret
RTX_ENDF(rt_gc_fix_run)
RTX_FUNC(rt_gc_slide_run)
    RTX_PUSHS(rbx)
    RTX_PUSHS(rbp)
    RTX_PUSHS(r12)
    RTX_PUSHS(r13)
    RTX_PUSHS(r14)
    RTX_PUSHS(r15)
    mov     r12, rcx
    mov     r13, qword ptr [rdx + 72]
    mov     r14, r8
    sub     r14, r12
    shr     r14, 4
    lea     r15, [r14 + 63]
    shr     r15, 6
    xor     ebx, ebx
    cmp     rdi, r12
    jne     .Lsl_g0
    mov     rbx, rsi
.Lsl_g0:
    mov     rbp, rbx
    shl     rbp, 4
    add     rbp, rdi
.Lsl_loop:
    cmp     rbx, r14
    jae     .Lsl_done
    mov     r8, rbx
    shr     r8, 6
    mov     ecx, ebx
    and     ecx, 63
    mov     rdx, -1
    shl     rdx, cl
    and     rdx, qword ptr [r13 + r8*8]
    jne     .Lsl_a
.Lsl_skipz:
    inc     r8
    cmp     r8, r15
    jae     .Lsl_done
    mov     rdx, qword ptr [r13 + r8*8]
    test    rdx, rdx
    je      .Lsl_skipz
.Lsl_a:
    bsf     rdx, rdx
    mov     r11, r8
    shl     r11, 6
    add     r11, rdx
    mov     ecx, r11d
    and     ecx, 63
    mov     rdx, -1
    shl     rdx, cl
    mov     rax, qword ptr [r13 + r8*8]
    not     rax
    and     rdx, rax
    jne     .Lsl_b
.Lsl_skipo:
    inc     r8
    cmp     r8, r15
    jae     .Lsl_btop
    mov     rdx, qword ptr [r13 + r8*8]
    not     rdx
    test    rdx, rdx
    je      .Lsl_skipo
.Lsl_b:
    bsf     rdx, rdx
    mov     r10, r8
    shl     r10, 6
    add     r10, rdx
    cmp     r10, r14
    jbe     .Lsl_copy
.Lsl_btop:
    mov     r10, r14
.Lsl_copy:
    mov     rcx, r10
    sub     rcx, r11
    shl     rcx, 1
    mov     rsi, r11
    shl     rsi, 4
    add     rsi, r12
    mov     rdi, rbp
    cmp     rdi, rsi
    je      .Lsl_adv
    rep movsq
.Lsl_adv:
    mov     rax, r10
    sub     rax, r11
    shl     rax, 4
    add     rbp, rax
    mov     rbx, r10
    jmp     .Lsl_loop
.Lsl_done:
    mov     rax, rbp
    RTX_POPS(r15)
    RTX_POPS(r14)
    RTX_POPS(r13)
    RTX_POPS(r12)
    RTX_POPS(rbp)
    RTX_POPS(rbx)
    ret
RTX_ENDF(rt_gc_slide_run)
RTX_FUNC(rt_gc_start_compact_run)
    RTX_PUSHS(rbx)
    RTX_PUSHS(rbp)
    RTX_PUSHS(r12)
    RTX_PUSHS(r13)
    RTX_PUSHS(r14)
    RTX_PUSHS(r15)
    mov     r8, qword ptr [rdi + 72]
    mov     r10, qword ptr [rdi + 96]
    sub     rcx, rsi
    shr     rcx, 4
    lea     r11, [rcx + 63]
    shr     r11, 6
    sub     rdx, rsi
    shr     rdx, 4
    mov     r12, rdx
    shr     r12, 6
    mov     rdi, r12
    and     edx, 63
    mov     r13, rdx
    xor     ebx, ebx
    xor     ebp, ebp
.Lsc_loop:
    cmp     rbp, r11
    jae     .Lsc_flush
    mov     r14, qword ptr [r8 + rbp*8]
    GC_POPCNT r15, r14
    test    r15, r15
    je      .Lsc_next
    mov     rax, qword ptr [r10 + rbp*8]
    GC_PEXT r14, rax, r14
    mov     rcx, r13
    mov     rax, r14
    shl     rax, cl
    or      rbx, rax
    lea     rax, [r13 + r15]
    cmp     rax, 64
    jb      .Lsc_ob
    mov     qword ptr [r10 + rdi*8], rbx
    inc     rdi
    xor     ebx, ebx
    cmp     rax, 64
    je      .Lsc_ob
    test    r13, r13
    je      .Lsc_ob
    mov     ecx, 64
    sub     ecx, r13d
    mov     rbx, r14
    shr     rbx, cl
.Lsc_ob:
    lea     rax, [r13 + r15]
    and     eax, 63
    mov     r13, rax
.Lsc_next:
    inc     rbp
    jmp     .Lsc_loop
.Lsc_flush:
    test    r13, r13
    je      .Lsc_zero
    mov     qword ptr [r10 + rdi*8], rbx
    inc     rdi
.Lsc_zero:
    cmp     rdi, r11
    jae     .Lsc_low
    mov     qword ptr [r10 + rdi*8], 0
    inc     rdi
    jmp     .Lsc_zero
.Lsc_low:
    xor     ebp, ebp
.Lsc_low_loop:
    cmp     rbp, r12
    jae     .Lsc_out
    mov     qword ptr [r10 + rbp*8], 0
    inc     rbp
    jmp     .Lsc_low_loop
.Lsc_out:
    RTX_POPS(r15)
    RTX_POPS(r14)
    RTX_POPS(r13)
    RTX_POPS(r12)
    RTX_POPS(rbp)
    RTX_POPS(rbx)
    ret
RTX_ENDF(rt_gc_start_compact_run)
RTX_FUNC(rt_gc_ssum_run)
    mov     r10, qword ptr [rdi + 96]
    mov     r11, qword ptr [rdi + 104]
    lea     rcx, [rsi + 63]
    shr     rcx, 6
    xor     eax, eax
    xor     edx, edx
.Lss_zero:
    cmp     rdx, rcx
    jae     .Lss_set
    mov     qword ptr [r11 + rdx*8], rax
    inc     rdx
    jmp     .Lss_zero
.Lss_set:
    xor     edx, edx
.Lss_loop:
    cmp     rdx, rsi
    jae     .Lss_out
    cmp     qword ptr [r10 + rdx*8], 0
    je      .Lss_next
    bts     qword ptr [r11], rdx
.Lss_next:
    inc     rdx
    jmp     .Lss_loop
.Lss_out:
    ret
RTX_ENDF(rt_gc_ssum_run)
RTX_FUNC(rt_gc_trail_run)
    RTX_PUSHS(rbx)
    RTX_PUSHS(rbp)
    RTX_PUSHS(r12)
    RTX_PUSHS(r13)
    RTX_PUSHS(r14)
    RTX_PUSHS(r15)
    mov     rsi, qword ptr [rdi + 0]
    mov     rax, qword ptr [rdi + 8]
    shl     rax, 4
    mov     r12, qword ptr [rdi + 16]
    mov     r13, qword ptr [rdi + 24]
    mov     r14, qword ptr [rdi + 32]
    mov     rdx, qword ptr [rdi + 40]
    mov     r10, qword ptr [rdx + 72]
    mov     r11, qword ptr [rdx + 112]
    mov     r15, qword ptr [rdi + 48]
    mov     rbp, qword ptr [rdi + 56]
    lea     rdi, [rsi + rax]
.Ltr_range:
    cmp     rsi, rdi
    jae     .Ltr_ok
    cmp     qword ptr [rsi + 8], 0
    jne     .Ltr_rnext
    mov     rbx, qword ptr [rsi]
    add     rbx, 64
.Ltr_entry:
    mov     r8, qword ptr [rsi]
    mov     r8, qword ptr [r8]
    lea     rax, [rbx + 32]
    cmp     rax, r8
    ja      .Ltr_rnext
    cmp     qword ptr [rbx], 0
    jne     .Ltr_enext
    mov     rax, qword ptr [rbx + 8]
    test    rax, rax
    je      .Ltr_enext
    cmp     rax, r15
    jb      .Ltr_bad
    cmp     rax, rbp
    ja      .Ltr_bad
    cmp     rax, r13
    jb      .Ltr_rank
    mov     qword ptr [rbx + 8], r14
    jmp     .Ltr_enext
.Ltr_rank:
    sub     rax, r15
    mov     rdx, rax
    shr     rdx, 4
    mov     ecx, edx
    and     ecx, 63
    shr     rdx, 6
    mov     r8, -1
    shl     r8, cl
    not     r8
    and     r8, qword ptr [r10 + rdx*8]
    GC_POPCNT r8, r8
    shl     r8, 4
    add     r8, qword ptr [r11 + rdx*8]
    and     eax, 15
    add     rax, r8
    add     rax, r12
    mov     qword ptr [rbx + 8], rax
.Ltr_enext:
    add     rbx, 32
    jmp     .Ltr_entry
.Ltr_rnext:
    add     rsi, 16
    jmp     .Ltr_range
.Ltr_ok:
    xor     eax, eax
.Ltr_bad:
    RTX_POPS(r15)
    RTX_POPS(r14)
    RTX_POPS(r13)
    RTX_POPS(r12)
    RTX_POPS(rbp)
    RTX_POPS(rbx)
    ret
RTX_ENDF(rt_gc_trail_run)
RTX_FUNC(rt_gc_clear_run)
    mov     r8, rdi
    mov     rdx, rsi
    xor     eax, eax
    mov     rdi, qword ptr [r8 + 72]
    mov     rcx, rdx
    rep stosq
    mov     rdi, qword ptr [r8 + 88]
    mov     rcx, rdx
    rep stosq
    ret
RTX_ENDF(rt_gc_clear_run)
.section .note.GNU-stack,"",@progbits
