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
#define MK_A 7
#define MK_T 8
#define HB_AGGP 207
#define HB_AGGT 208
#define HB_AGGB 211
#define HB_ARR 212
#define HB_PVEC 220
.macro MK_RAW loc
    MK_BLK_OF r10
    test    rax, rax
    je      331f
    MK_MARK_BLK
    MK_SLOT \loc
331:
.endm
.macro MK_SLOTM
    mov     rdx, qword ptr [rdi + 24]
    lea     rcx, [rbx + rbx]
    lea     rdx, [rdx + rcx*8]
    mov     qword ptr [rdx + 8], r10
    mov     rcx, qword ptr [rdi + 176]
    mov     qword ptr [rdx], rcx
    inc     rbx
.endm
.macro MK_WLPUSH p
    movzx   eax, byte ptr [\p]
    lea     rdx, [rip + .Lmk_class]
    cmp     byte ptr [rdx + rax], 0
    je      330f
    mov     rax, qword ptr [rdi]
    mov     qword ptr [rax + r11*8], \p
    inc     r11
#if RT_DIAG
    cmp     r11, qword ptr [rdi + 120]
    jle     330f
    mov     qword ptr [rdi + 120], r11
#endif
330:
.endm
.macro MK_VIS_TEST_SET
    mov     rdx, rax
    sub     rdx, r12
    shr     rdx, 4
    mov     ecx, edx
    and     ecx, 63
    shr     rdx, 6
    mov     r13, qword ptr [rdi + 88]
    xor     ebp, ebp
    bts     qword ptr [r13 + rdx*8], rcx
    jnc     327f
    mov     ebp, 0x10
327:
.endm
.macro MK_VIS_TEST
    mov     rdx, rax
    sub     rdx, r12
    shr     rdx, 4
    mov     ecx, edx
    and     ecx, 63
    shr     rdx, 6
    mov     r13, qword ptr [rdi + 88]
    xor     ebp, ebp
    bt      qword ptr [r13 + rdx*8], rcx
    jnc     328f
    mov     ebp, 0x10
328:
.endm
.macro MK_EXACT type
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
    cmp     word ptr [rax + 12], \type
    jne     .Lmk_loop
    test    word ptr [rax + 14], HBF_TTL
    je      .Lmk_loop
.endm
.macro MK_DVEC_RANGE
    lea     rdx, [rax + 16]
    mov     qword ptr [rdi + 104], rdx
    mov     edx, dword ptr [rax + 8]
    sub     rdx, 16
    shr     rdx, 4
    mov     qword ptr [rdi + 112], rdx
    jmp     .Lmk_range
.endm
RTX_FUNC(rt_gc_mark_run)
    RTX_PUSHS(rbx)
    RTX_PUSHS(rbp)
    RTX_PUSHS(r12)
    RTX_PUSHS(r13)
    RTX_PUSHS(r14)
    RTX_PUSHS(r15)
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
    mov     rax, qword ptr [rdi]
    mov     rsi, qword ptr [rax + r11*8]
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
    cmp     eax, MK_A
    je      .Lmk_a
    cmp     eax, MK_T
    je      .Lmk_t
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
    MK_RAW rsi+8
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
    MK_EXACT HB_DINST
    MK_MARK_VIS
    MK_SLOT rsi+8
    test    ebp, 0x10
    jne     .Lmk_loop
    mov     rsi, qword ptr [rsi + 8]
.Lmk_dinst_u:
    cmp     qword ptr [rsi + 8], 0
    je      .Lmk_loop
    cmp     qword ptr [rsi], 0
    je      .Lmk_loop
    mov     r10, qword ptr [rsi]
    MK_RAW rsi
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
    MK_DVEC_RANGE
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
    mov     rax, qword ptr [rdi]
    mov     qword ptr [rax + r11*8], rsi
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
.Lmk_a:
    MK_EXACT HB_ARR
    MK_MARK_VIS
    MK_SLOT rsi+8
    test    ebp, 0x10
    jne     .Lmk_loop
    mov     rsi, r10
.Lmk_arr:
    mov     r10, qword ptr [rsi + 24]
    test    r10, r10
    je      .Lmk_arr_data
    MK_RAW rsi+24
.Lmk_arr_data:
    mov     r10, qword ptr [rsi + 32]
    test    r10, r10
    je      .Lmk_loop
    MK_BLK_OF r10
    test    rax, rax
    je      .Lmk_arr_elems
    cmp     word ptr [rax + 12], HB_DVEC
    jne     .Lmk_arr_flat
    MK_MARK_VIS
    MK_SLOT rsi+32
    test    ebp, 0x10
    jne     .Lmk_loop
    MK_DVEC_RANGE
.Lmk_arr_flat:
    MK_MARK_BLK
    MK_SLOT rsi+32
.Lmk_arr_elems:
    mov     eax, dword ptr [rsi + 4]
    sub     eax, dword ptr [rsi]
    inc     eax
    movsxd  rax, eax
    cmp     dword ptr [rsi + 8], 2
    jne     .Lmk_arr_n
    mov     edx, dword ptr [rsi + 16]
    sub     edx, dword ptr [rsi + 12]
    inc     edx
    movsxd  rdx, edx
    imul    rax, rdx
.Lmk_arr_n:
    test    rax, rax
    jle     .Lmk_loop
    mov     qword ptr [rdi + 112], rax
    mov     rdx, qword ptr [rsi + 32]
    mov     qword ptr [rdi + 104], rdx
    jmp     .Lmk_range
.Lmk_t:
    MK_EXACT HB_AGGT
    MK_SLOT rsi+8
    mov     rsi, r10
.Lmk_tbl:
    lea     rax, [rsi - 16]
    MK_VIS_TEST
    test    ebp, ebp
    jne     .Lmk_loop
    mov     r10d, dword ptr [rsi + 8]
    mov     rax, qword ptr [rdi + 40]
    sub     rax, rbx
    lea     rdx, [r10 + 3]
    cmp     rax, rdx
    jl      .Lmk_blk_full_slots
    xor     edx, edx
    mov     rcx, qword ptr [rsi]
    test    rcx, rcx
    je      .Lmk_tbl_cnt_done
    xor     eax, eax
.Lmk_tbl_cnt:
    cmp     rax, r10
    jae     .Lmk_tbl_cnt_done
    mov     r13, qword ptr [rcx + rax*8]
    test    r13, r13
    je      .Lmk_tbl_cnt_next
    mov     r13d, dword ptr [r13]
    add     rdx, r13
.Lmk_tbl_cnt_next:
    inc     rax
    jmp     .Lmk_tbl_cnt
.Lmk_tbl_cnt_done:
    mov     eax, dword ptr [rsi + 64]
    lea     rdx, [rax + rdx*2 + 1]
    mov     rax, qword ptr [rdi + 16]
    sub     rax, r11
    cmp     rax, rdx
    jl      .Lmk_blk_full_wl
    lea     rax, [rsi - 16]
    MK_VIS_TEST_SET
    MK_MARK_BLK
    lea     r10, [rsi + 32]
    MK_WLPUSH r10
    mov     r10, qword ptr [rsi + 56]
    test    r10, r10
    je      .Lmk_tbl_icn
    MK_RAW rsi+56
    mov     r8, qword ptr [rsi + 56]
    mov     r10d, dword ptr [rsi + 64]
    shl     r10, 4
    add     r10, r8
.Lmk_tbl_ord:
    cmp     r8, r10
    jae     .Lmk_tbl_icn
    MK_WLPUSH r8
    add     r8, 16
    jmp     .Lmk_tbl_ord
.Lmk_tbl_icn:
    mov     r10, qword ptr [rsi + 112]
    test    r10, r10
    je      .Lmk_tbl_bk
    MK_RAW rsi+112
.Lmk_tbl_bk:
    mov     r10, qword ptr [rsi]
    test    r10, r10
    je      .Lmk_loop
    MK_RAW rsi
    xor     r8d, r8d
.Lmk_tbl_b:
    cmp     r8d, dword ptr [rsi + 8]
    jae     .Lmk_loop
    mov     rax, qword ptr [rsi]
    lea     rax, [rax + r8*8]
    mov     r10, qword ptr [rax]
    test    r10, r10
    je      .Lmk_tbl_bnext
    mov     qword ptr [rdi + 176], rax
    MK_BLK_OF r10
    test    rax, rax
    je      .Lmk_tbl_pairs
    MK_MARK_BLK
    MK_SLOTM
.Lmk_tbl_pairs:
    mov     rax, qword ptr [rdi + 176]
    mov     r10, qword ptr [rax]
    mov     ecx, dword ptr [r10]
    lea     rcx, [rcx + rcx*4]
    shl     rcx, 3
    add     r10, 8
    add     rcx, r10
.Lmk_tbl_p:
    cmp     r10, rcx
    jae     .Lmk_tbl_bnext
    MK_WLPUSH r10
    add     r10, 16
    MK_WLPUSH r10
    add     r10, 24
    jmp     .Lmk_tbl_p
.Lmk_tbl_bnext:
    inc     r8
    jmp     .Lmk_tbl_b
.Lmk_empty:
    test    r15, r15
    je      .Lmk_none
    mov     rax, r15
    mov     r15, qword ptr [rax]
    mov     qword ptr [rax], 0
    inc     qword ptr [rdi + 168]
    lea     rsi, [rax + 16]
    movzx   edx, word ptr [rax + 12]
    cmp     edx, HB_DVEC
    je      .Lmk_m_dvec
    cmp     edx, HB_ARR
    je      .Lmk_m_arr
    cmp     edx, HB_DINST
    je      .Lmk_m_dinst
    cmp     edx, HB_AGGP
    je      .Lmk_m_aggp
    cmp     edx, HB_PVEC
    je      .Lmk_m_pvec
    cmp     edx, HB_AGGT
    je      .Lmk_tbl
    cmp     edx, HB_AGGB
    je      .Lmk_loop
    or      rax, 2
    jmp     .Lmk_out
.Lmk_m_dvec:
    MK_VIS_TEST_SET
    test    ebp, ebp
    jne     .Lmk_loop
    MK_DVEC_RANGE
.Lmk_m_arr:
    MK_VIS_TEST_SET
    test    ebp, ebp
    jne     .Lmk_loop
    jmp     .Lmk_arr
.Lmk_m_dinst:
    MK_VIS_TEST_SET
    test    ebp, ebp
    jne     .Lmk_loop
    jmp     .Lmk_dinst_u
.Lmk_m_aggp:
    mov     qword ptr [rdi + 104], rsi
    mov     qword ptr [rdi + 112], 2
    jmp     .Lmk_range
.Lmk_m_pvec:
    mov     edx, dword ptr [rax + 8]
    sub     rdx, 16
    shr     rdx, 3
    mov     rcx, qword ptr [rdi + 40]
    sub     rcx, rbx
    cmp     rcx, rdx
    jl      .Lmk_blk_full_slots
    lea     rdx, [rsi + rdx*8]
    mov     qword ptr [rdi + 176], rdx
.Lmk_pv:
    cmp     rsi, qword ptr [rdi + 176]
    jae     .Lmk_loop
    mov     r10, qword ptr [rsi]
    test    r10, r10
    je      .Lmk_pv_next
    MK_RAW rsi
.Lmk_pv_next:
    add     rsi, 8
    jmp     .Lmk_pv
.Lmk_blk_full_slots:
    lea     rax, [rsi - 16]
    mov     qword ptr [rax], r15
    mov     r15, rax
    mov     eax, 1
    jmp     .Lmk_out
.Lmk_blk_full_wl:
    lea     rax, [rsi - 16]
    mov     qword ptr [rax], r15
    mov     r15, rax
    mov     eax, 2
    jmp     .Lmk_out
.Lmk_slow:
    mov     rax, rsi
    jmp     .Lmk_out
.Lmk_none:
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
.byte MK_A
.org .Lmk_class + DT_T
.byte MK_T
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
    test    r14, r14
    je      .Lsc_next
    mov     rax, qword ptr [r10 + rbp*8]
    mov     rsi, r14
    xor     ecx, ecx
    xor     edx, edx
.Lsc_bits:
    test    rsi, rsi
    je      .Lsc_bits_done
    bsf     r15, rsi
    bt      rax, r15
    jnc     .Lsc_bits_next
    bts     rdx, rcx
.Lsc_bits_next:
    inc     ecx
    lea     r15, [rsi - 1]
    and     rsi, r15
    jmp     .Lsc_bits
.Lsc_bits_done:
    mov     r15, rcx
    mov     r14, rdx
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
