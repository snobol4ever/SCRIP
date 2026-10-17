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
#define HB_WSB 215
#define HB_DTP 221
#define HB_PLDB 216
#define HB_PLDBS 217
#define HB_PLDBR 218
#define HB_PLDBK 219
#define HB_DTPRCP 222
#define DATA_INST_SLEN 0
.macro MK_EXACTQ type, rej
    cmp     r10, 16
    jb      \rej
    test    r10b, 7
    jne     \rej
    MK_BLK_OF r10
    test    rax, rax
    je      \rej
    lea     rdx, [rax + 16]
    cmp     rdx, r10
    jne     \rej
    cmp     word ptr [rax + 12], \type
    jne     \rej
    test    word ptr [rax + 14], HBF_TTL
    je      \rej
.endm
.macro MK_PUSH_CELL p, full
    cmp     r11, qword ptr [rdi + 16]
    jae     \full
    mov     rax, qword ptr [rdi]
    mov     qword ptr [rax + r11*8], \p
    inc     r11
#if RT_DIAG
    cmp     r11, qword ptr [rdi + 120]
    jle     335f
    mov     qword ptr [rdi + 120], r11
335:
#endif
.endm
.macro MK_HLOAD full
    mov     rax, qword ptr [rdi + 288]
    lea     rax, [rax + rax*4]
    add     rax, rax
    mov     rdx, qword ptr [rdi + 280]
    lea     rcx, [rdx*8]
    sub     rcx, rdx
    cmp     rax, rcx
    jae     \full
.endm
.macro MK_HINS
    mov     rax, r10
    shr     rax, 4
    mov     rdx, 0x9E3779B97F4A7C15
    imul    rax, rdx
    mov     rcx, qword ptr [rdi + 280]
    dec     rcx
    and     rax, rcx
    mov     rdx, qword ptr [rdi + 272]
336:
    mov     rbp, qword ptr [rdx + rax*8]
    test    rbp, rbp
    je      337f
    cmp     rbp, r10
    je      338f
    inc     rax
    and     rax, rcx
    jmp     336b
337:
    mov     qword ptr [rdx + rax*8], r10
    inc     qword ptr [rdi + 288]
    mov     eax, 1
    jmp     339f
338:
    xor     eax, eax
339:
.endm
RTX_FUNC(rt_gc_mark_run)
    jmp     .Lmk_enter
RTX_ENTRY(rt_gc_visit_descr_a)
    xor     eax, eax
    test    rdi, rdi
    je      .Lvd_ret
    movzx   ecx, byte ptr [rdi]
    lea     rdx, [rip + .Lmk_class]
    cmp     byte ptr [rdx + rcx], 0
    je      .Lvd_ret
    lea     rdx, [rip + g_gc_mk]
    mov     rcx, qword ptr [rdx + 8]
    cmp     rcx, qword ptr [rdx + 16]
    jae     .Lvd_full
    mov     rsi, qword ptr [rdx]
    mov     qword ptr [rsi + rcx*8], rdi
    inc     rcx
    mov     qword ptr [rdx + 8], rcx
#if RT_DIAG
    cmp     rcx, qword ptr [rdx + 120]
    jle     .Lvd_dr
    mov     qword ptr [rdx + 120], rcx
.Lvd_dr:
#endif
    mov     eax, 3
    cmp     qword ptr [rdx + 184], 0
    jne     .Lvd_ret
    mov     qword ptr [rdx + 184], 1
    mov     rdi, rdx
    jmp     .Lmk_enter
.Lvd_full:
    mov     eax, 4
.Lvd_ret:
    ret
RTX_ENTRY(rt_gc_frame_cells_a)
    lea     rax, [rip + g_gc_mk]
    mov     qword ptr [rax + 192], rdi
    mov     qword ptr [rax + 200], rsi
    mov     qword ptr [rax + 208], rdx
    mov     qword ptr [rax + 216], rcx
    mov     qword ptr [rax + 224], 0
    mov     qword ptr [rax + 248], 0
    mov     qword ptr [rax + 184], 1
    mov     rdi, rax
    jmp     .Lmk_enter
RTX_ENTRY(rt_gc_walk_words_a)
    lea     rax, [rip + g_gc_mk]
    mov     qword ptr [rax + 256], rdi
    mov     qword ptr [rax + 264], rsi
    mov     qword ptr [rax + 184], 1
    mov     rdi, rax
    jmp     .Lmk_enter
.Lmk_enter:
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
    je      .Lmk_n2
    cmp     eax, 1
    je      .Lmk_n1
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
    cmp     edx, HB_AGGV
    je      .Lmk_m_aggv
    cmp     edx, HB_DTP
    je      .Lmk_m_dtp
    cmp     edx, HB_DTPRCP
    je      .Lmk_m_dtprcp
    cmp     edx, HB_PLDB
    je      .Lmk_m_pldb
    cmp     edx, HB_PLDBS
    je      .Lmk_m_pldbs
    cmp     edx, HB_PLDBR
    je      .Lmk_m_pldbr
    cmp     edx, HB_PLDBK
    je      .Lmk_m_pldbk
    or      rax, 2
    jmp     .Lmk_out
.Lmk_m_pldb:
    mov     r10, qword ptr [rsi]
    test    r10, r10
    je      .Lmk_loop
    MK_RAW rsi
    jmp     .Lmk_loop
.Lmk_m_pldbs:
    mov     edx, dword ptr [rax + 8]
    sub     rdx, 16
    xor     ecx, ecx
    mov     rax, rdx
    mov     rdx, 0xCCCCCCCCCCCCCCCD
    mul     rdx
    shr     rdx, 5
    mov     rcx, qword ptr [rdi + 16]
    sub     rcx, r11
    cmp     rcx, rdx
    jl      .Lmk_blk_full_wl
    lea     rdx, [rdx + rdx*4]
    lea     rdx, [rsi + rdx*8]
    mov     qword ptr [rdi + 176], rdx
.Lmk_pldbs_e:
    cmp     rsi, qword ptr [rdi + 176]
    jae     .Lmk_loop
    MK_WLPUSH rsi
    add     rsi, 40
    jmp     .Lmk_pldbs_e
.Lmk_m_pldbr:
    mov     r10, qword ptr [rsi]
    test    r10, r10
    je      .Lmk_m_pldbr1
    MK_RAW rsi
.Lmk_m_pldbr1:
    mov     r10, qword ptr [rsi + 24]
    test    r10, r10
    je      .Lmk_m_pldbr2
    MK_RAW rsi+24
.Lmk_m_pldbr2:
    mov     r10, qword ptr [rsi + 32]
    test    r10, r10
    je      .Lmk_loop
    MK_RAW rsi+32
    jmp     .Lmk_loop
.Lmk_m_pldbk:
    mov     edx, dword ptr [rax + 8]
    sub     rdx, 16
    shr     rdx, 4
    mov     rax, rdx
    mov     rdx, 0xAAAAAAAAAAAAAAAB
    mul     rdx
    shr     rdx, 1
    lea     rcx, [rdx + rdx]
    mov     rax, qword ptr [rdi + 40]
    sub     rax, rbx
    cmp     rax, rcx
    jl      .Lmk_blk_full_slots
    lea     rdx, [rdx + rdx*2]
    shl     rdx, 4
    add     rdx, rsi
    mov     qword ptr [rdi + 176], rdx
.Lmk_pldbk_e:
    cmp     rsi, qword ptr [rdi + 176]
    jae     .Lmk_loop
    mov     r10, qword ptr [rsi + 16]
    test    r10, r10
    je      .Lmk_pldbk_k
    MK_RAW rsi+16
.Lmk_pldbk_k:
    mov     r10, qword ptr [rsi]
    test    r10, r10
    je      .Lmk_pldbk_n
    MK_RAW rsi
.Lmk_pldbk_n:
    add     rsi, 48
    jmp     .Lmk_pldbk_e
.Lmk_m_dtp:
    mov     r10, qword ptr [rsi + 8]
    test    r10, r10
    je      .Lmk_m_dtp1
    MK_RAW rsi+8
.Lmk_m_dtp1:
    mov     r10, qword ptr [rsi + 32]
    test    r10, r10
    je      .Lmk_loop
    MK_RAW rsi+32
    jmp     .Lmk_loop
.Lmk_m_dtprcp:
    mov     r10, qword ptr [rsi + 8]
    test    r10, r10
    je      .Lmk_m_rcp1
    MK_RAW rsi+8
.Lmk_m_rcp1:
    mov     r10, qword ptr [rsi + 32]
    test    r10, r10
    je      .Lmk_m_rcp2
    MK_RAW rsi+32
.Lmk_m_rcp2:
    mov     r10, qword ptr [rsi + 40]
    test    r10, r10
    je      .Lmk_loop
    MK_RAW rsi+40
    jmp     .Lmk_loop
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
.Lmk_n1:
    mov     r10, qword ptr [rsi + 8]
    MK_BLK_OF r10
    test    rax, rax
    je      .Lmk_loop
    lea     rdx, [rax + 16]
    cmp     r10, rdx
    jb      .Lmk_loop
    mov     edx, dword ptr [rax + 8]
    add     rdx, rax
    lea     rcx, [r10 + 16]
    cmp     rcx, rdx
    ja      .Lmk_loop
    mov     qword ptr [rdi + 296], rax
    MK_HLOAD .Lmk_hgrow_d
    mov     rax, qword ptr [rdi + 16]
    sub     rax, r11
    cmp     rax, 1
    jl      .Lmk_wlgrow_d
    mov     rax, qword ptr [rdi + 296]
    MK_SLOT rsi+8
    MK_MARK_BLK
    or      r10, 2
    MK_HINS
    test    eax, eax
    je      .Lmk_loop
    and     r10, -4
    MK_WLPUSH r10
    jmp     .Lmk_loop
.Lmk_n2:
    mov     r10, qword ptr [rsi + 8]
    MK_EXACTQ HB_AGGV, .Lmk_loop
    mov     qword ptr [rdi + 296], rax
    mov     r10, qword ptr [r10 + 8]
    test    r10, r10
    je      .Lmk_n2_ok
    MK_EXACTQ HB_AGGT, .Lmk_slow
.Lmk_n2_ok:
    MK_HLOAD .Lmk_hgrow_d
    mov     rax, qword ptr [rdi + 16]
    sub     rax, r11
    cmp     rax, 3
    jl      .Lmk_wlgrow_d
    mov     rax, qword ptr [rdi + 40]
    sub     rax, rbx
    cmp     rax, 4
    jl      .Lmk_slgrow_d
    mov     r10, qword ptr [rsi + 8]
    MK_SLOT rsi+8
    mov     rax, qword ptr [rdi + 296]
    MK_VIS_TEST_SET
    test    ebp, ebp
    jne     .Lmk_loop
    mov     rsi, r10
    mov     rax, qword ptr [rdi + 296]
.Lmk_vcell:
    MK_MARK_BLK
    mov     r10, qword ptr [rsi + 16]
    test    r10, r10
    je      .Lmk_vc_tbl
    MK_RAW rsi+16
.Lmk_vc_tbl:
    mov     r10, qword ptr [rsi + 8]
    test    r10, r10
    je      .Lmk_vc_cellp
    MK_BLK_OF r10
    test    rax, rax
    je      .Lmk_vc_cellp
    MK_SLOT rsi+8
.Lmk_vc_cellp:
    mov     r10, qword ptr [rsi]
    test    r10, r10
    je      .Lmk_vc_descrs
    MK_RAW rsi
.Lmk_vc_descrs:
    lea     r10, [rsi + 24]
    MK_WLPUSH r10
    lea     r10, [rsi + 40]
    MK_WLPUSH r10
    mov     r10, qword ptr [rsi]
    test    r10, r10
    je      .Lmk_vc_table
    MK_WLPUSH r10
.Lmk_vc_table:
    mov     r10, qword ptr [rsi + 8]
    test    r10, r10
    je      .Lmk_loop
    MK_HINS
    test    eax, eax
    je      .Lmk_loop
    mov     rsi, r10
    jmp     .Lmk_tbl
.Lmk_m_aggv:
    mov     qword ptr [rdi + 296], rax
    mov     r10, qword ptr [rsi + 8]
    test    r10, r10
    je      .Lmk_m_aggv_ok
    MK_EXACTQ HB_AGGT, .Lmk_m_slow
.Lmk_m_aggv_ok:
    MK_HLOAD .Lmk_hgrow_b
    mov     rax, qword ptr [rdi + 16]
    sub     rax, r11
    cmp     rax, 3
    jl      .Lmk_wlgrow_b
    mov     rax, qword ptr [rdi + 296]
    MK_VIS_TEST_SET
    test    ebp, ebp
    jne     .Lmk_loop
    mov     rax, qword ptr [rdi + 296]
    jmp     .Lmk_vcell
.Lmk_m_slow:
    mov     rax, qword ptr [rdi + 296]
    or      rax, 2
    jmp     .Lmk_out
.Lmk_hgrow_d:
    mov     rax, qword ptr [rdi]
    mov     qword ptr [rax + r11*8], rsi
    inc     r11
    mov     eax, 6
    jmp     .Lmk_out
.Lmk_wlgrow_d:
    mov     rax, qword ptr [rdi]
    mov     qword ptr [rax + r11*8], rsi
    inc     r11
    mov     eax, 2
    jmp     .Lmk_out
.Lmk_slgrow_d:
    mov     rax, qword ptr [rdi]
    mov     qword ptr [rax + r11*8], rsi
    inc     r11
    mov     eax, 1
    jmp     .Lmk_out
.Lmk_hgrow_b:
    mov     rax, qword ptr [rdi + 296]
    mov     qword ptr [rax], r15
    mov     r15, rax
    mov     eax, 6
    jmp     .Lmk_out
.Lmk_wlgrow_b:
    mov     rax, qword ptr [rdi + 296]
    mov     qword ptr [rax], r15
    mov     r15, rax
    mov     eax, 2
    jmp     .Lmk_out
.Lmk_slow:
    mov     rax, rsi
    jmp     .Lmk_out
.Lmk_prod_wl_full:
    mov     eax, 2
    jmp     .Lmk_out
.Lmk_prod_slots_full:
    mov     eax, 1
    jmp     .Lmk_out
.Lmk_none:
    cmp     qword ptr [rdi + 200], 0
    jne     .Lfc_resume
    cmp     qword ptr [rdi + 264], 0
    jne     .Lsw_cells
    xor     eax, eax
    jmp     .Lmk_out
.Lfc_resume:
    mov     rax, qword ptr [rdi + 248]
    cmp     rax, 1
    je      .Lfc_descr
    cmp     rax, 2
    je      .Lfc_ptr
.Lfc_entry:
    mov     rsi, qword ptr [rdi + 200]
    mov     rax, qword ptr [rdi + 224]
    cmp     rax, qword ptr [rsi + 32]
    jae     .Lfc_done
    mov     rdx, qword ptr [rsi + 40 + rax*8]
    inc     rax
    mov     qword ptr [rdi + 224], rax
    movsxd  rax, edx
    add     rax, qword ptr [rdi + 192]
    mov     rcx, rdx
    shr     rcx, 40
    and     ecx, 0xFFFF
    lea     r8, [rax + rcx]
    shr     rdx, 32
    and     edx, 0xFF
    cmp     rax, qword ptr [rdi + 208]
    jae     .Lfc_w_ok
    mov     rcx, qword ptr [rdi + 208]
    test    edx, edx
    jne     .Lfc_w_lo
    sub     rcx, rax
    add     rcx, 15
    and     rcx, -16
    add     rax, rcx
    jmp     .Lfc_w_ok
.Lfc_w_lo:
    mov     rax, rcx
.Lfc_w_ok:
    cmp     r8, qword ptr [rdi + 216]
    jbe     .Lfc_e_ok
    mov     r8, qword ptr [rdi + 216]
.Lfc_e_ok:
    cmp     rax, r8
    jae     .Lfc_entry
    mov     qword ptr [rdi + 232], rax
    mov     qword ptr [rdi + 240], r8
    test    edx, edx
    je      .Lfc_set_descr
    cmp     edx, 2
    jne     .Lfc_entry
    mov     qword ptr [rdi + 248], 2
    jmp     .Lfc_ptr
.Lfc_set_descr:
    mov     qword ptr [rdi + 248], 1
.Lfc_descr:
    mov     rsi, qword ptr [rdi + 232]
    lea     rax, [rsi + 16]
    cmp     rax, qword ptr [rdi + 240]
    ja      .Lfc_entry_end
    movzx   eax, byte ptr [rsi]
    lea     rdx, [rip + .Lmk_known]
    cmp     byte ptr [rdx + rax], 0
    je      .Lfc_descr_next
    lea     rdx, [rip + .Lmk_class]
    cmp     byte ptr [rdx + rax], 0
    je      .Lfc_descr_next
    MK_PUSH_CELL rsi, .Lmk_prod_wl_full
    add     qword ptr [rdi + 232], 16
    jmp     .Lmk_loop
.Lfc_descr_next:
    add     qword ptr [rdi + 232], 16
    jmp     .Lfc_descr
.Lfc_entry_end:
    mov     qword ptr [rdi + 248], 0
    jmp     .Lfc_entry
.Lfc_ptr:
    mov     rsi, qword ptr [rdi + 232]
    lea     rax, [rsi + 8]
    cmp     rax, qword ptr [rdi + 240]
    ja      .Lfc_entry_end
    mov     rax, qword ptr [rdi + 40]
    sub     rax, rbx
    cmp     rax, 1
    jl      .Lmk_prod_slots_full
    add     qword ptr [rdi + 232], 8
    mov     r10, qword ptr [rsi]
    MK_RAW rsi
    jmp     .Lfc_ptr
.Lfc_done:
    mov     qword ptr [rdi + 200], 0
    mov     qword ptr [rdi + 248], 0
    jmp     .Lmk_none
.Lsw_cells:
    mov     rsi, qword ptr [rdi + 256]
    lea     rax, [rsi + 16]
    cmp     rax, qword ptr [rdi + 264]
    ja      .Lsw_done
    movzx   eax, byte ptr [rsi]
    mov     r10, qword ptr [rsi + 8]
    cmp     eax, DT_S
    je      .Lsw_str
    cmp     eax, DT_X
    je      .Lsw_str
    cmp     eax, DT_N
    je      .Lsw_n
    cmp     eax, DT_T
    je      .Lsw_t
    cmp     eax, DT_A
    je      .Lsw_a
    cmp     eax, DT_E
    je      .Lsw_e
    cmp     eax, DT_DATA
    je      .Lsw_data
    cmp     eax, DT_P
    je      .Lsw_anyblk
    cmp     eax, DT_PLVAR
    je      .Lsw_anyblk
    cmp     eax, DT_PLREF
    je      .Lsw_anyblk
    cmp     eax, DT_BIG
    je      .Lsw_wsb
    cmp     eax, DT_CPLX
    je      .Lsw_wsb
    cmp     eax, DT_EXTL
    je      .Lsw_wsb
.Lsw_reject:
    add     qword ptr [rdi + 256], 8
    jmp     .Lsw_cells
.Lsw_str:
    MK_BLK_OF r10
    test    rax, rax
    je      .Lsw_reject
    lea     rdx, [rax + 16]
    cmp     r10, rdx
    jb      .Lsw_reject
    jmp     .Lsw_accept
.Lsw_n:
    mov     edx, dword ptr [rsi + 4]
    test    edx, edx
    je      .Lsw_str
    cmp     edx, 2
    je      .Lsw_aggv
    cmp     edx, 1
    jne     .Lsw_anyblk
    MK_BLK_OF r10
    test    rax, rax
    je      .Lsw_reject
    lea     rdx, [rax + 16]
    cmp     r10, rdx
    jb      .Lsw_reject
    mov     edx, dword ptr [rax + 8]
    add     rdx, rax
    lea     rcx, [r10 + 16]
    cmp     rcx, rdx
    ja      .Lsw_reject
    jmp     .Lsw_accept
.Lsw_aggv:
    MK_BLK_OF r10
    test    rax, rax
    je      .Lsw_reject
    cmp     word ptr [rax + 12], HB_AGGV
    jne     .Lsw_reject
    lea     rdx, [rax + 16]
    cmp     r10, rdx
    jne     .Lsw_reject
    jmp     .Lsw_accept
.Lsw_t:
    MK_BLK_OF r10
    test    rax, rax
    je      .Lsw_reject
    cmp     word ptr [rax + 12], HB_AGGT
    jne     .Lsw_reject
    lea     rdx, [rax + 16]
    cmp     r10, rdx
    jne     .Lsw_reject
    jmp     .Lsw_accept
.Lsw_anyblk:
    MK_BLK_OF r10
    test    rax, rax
    je      .Lsw_reject
    jmp     .Lsw_accept
.Lsw_a:
    MK_EXACTQ HB_ARR, .Lsw_reject
    jmp     .Lsw_accept
.Lsw_e:
    mov     edx, dword ptr [rsi + 4]
    cmp     edx, PROCVAL_SLEN
    je      .Lsw_str
    cmp     edx, PROCVAL_BUILTIN_SLEN
    je      .Lsw_str
    cmp     edx, PROCVAL_EXTERNAL_SLEN
    je      .Lsw_str
    jmp     .Lsw_reject
.Lsw_data:
    mov     edx, dword ptr [rsi + 4]
    cmp     edx, DATA_ELEMS_SLEN
    jne     .Lsw_data_inst
    test    r10, r10
    je      .Lsw_reject
    MK_BLK_OF r10
    test    rax, rax
    je      .Lsw_reject
    cmp     word ptr [rax + 12], HB_DVEC
    jne     .Lsw_reject
    mov     rdx, r10
    sub     rdx, rax
    sub     rdx, 16
    test    dl, 15
    jne     .Lsw_reject
    jmp     .Lsw_accept
.Lsw_data_inst:
    cmp     edx, DATA_INST_SLEN
    jne     .Lsw_reject
    MK_EXACTQ HB_DINST, .Lsw_reject
    jmp     .Lsw_accept
.Lsw_wsb:
    MK_BLK_OF r10
    test    rax, rax
    je      .Lsw_reject
    cmp     word ptr [rax + 12], HB_WSB
    jne     .Lsw_reject
    lea     rdx, [rax + 16]
    cmp     r10, rdx
    jne     .Lsw_reject
.Lsw_accept:
    MK_PUSH_CELL rsi, .Lmk_prod_wl_full
    add     qword ptr [rdi + 256], 16
    jmp     .Lmk_loop
.Lsw_done:
    mov     qword ptr [rdi + 264], 0
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
.p2align 6
.Lmk_known:
.org .Lmk_known + 0
.byte 1
.org .Lmk_known + 2
.byte 1
.org .Lmk_known + 3
.byte 1
.org .Lmk_known + 5
.byte 1
.org .Lmk_known + 8
.byte 1
.org .Lmk_known + 16
.byte 1
.org .Lmk_known + 24
.byte 1
.org .Lmk_known + 32
.byte 1
.org .Lmk_known + 40
.byte 1
.org .Lmk_known + 48
.byte 1
.org .Lmk_known + 56
.byte 1
.org .Lmk_known + 64
.byte 1
.org .Lmk_known + 72
.byte 1
.org .Lmk_known + 80
.byte 1
.org .Lmk_known + 88
.byte 1
.org .Lmk_known + 96
.byte 1
.org .Lmk_known + 104
.byte 1
.org .Lmk_known + 112
.byte 1
.org .Lmk_known + 120
.byte 1
.org .Lmk_known + 128
.byte 1
.org .Lmk_known + 136
.byte 1
.org .Lmk_known + 144
.byte 1
.org .Lmk_known + 152
.byte 1
.org .Lmk_known + 176
.byte 1
.org .Lmk_known + 256
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
RTX_FUNC(rt_gc_visit_raw_a)
    mov     rsi, rdi
    lea     rdi, [rip + g_gc_mk]
    mov     rax, qword ptr [rdi + 40]
    sub     rax, qword ptr [rdi + 32]
    jle     .Lvr_no
    RTX_PUSHS(rbx)
    RTX_PUSHS(rbp)
    RTX_PUSHS(r12)
    RTX_PUSHS(r13)
    RTX_PUSHS(r14)
    RTX_PUSHS(r15)
    mov     rbx, qword ptr [rdi + 32]
    mov     r12, qword ptr [rdi + 48]
    mov     r14, qword ptr [rdi + 72]
    mov     r15, qword ptr [rdi + 96]
    mov     r10, qword ptr [rsi]
    MK_RAW rsi
    mov     qword ptr [rdi + 32], rbx
    mov     qword ptr [rdi + 96], r15
    RTX_POPS(r15)
    RTX_POPS(r14)
    RTX_POPS(r13)
    RTX_POPS(r12)
    RTX_POPS(rbp)
    RTX_POPS(rbx)
    mov     eax, 1
    ret
.Lvr_no:
    xor     eax, eax
    ret
RTX_ENDF(rt_gc_visit_raw_a)
.macro MK_PTRS
332:
    mov     rax, qword ptr [r8 + 72]
    cmp     rax, qword ptr [r8 + 64]
    jae     334f
    mov     r10, qword ptr [rax]
    test    r10, r10
    je      333f
    mov     qword ptr [rdi + 176], rax
    MK_BLK_OF r10
    test    rax, rax
    je      333f
    MK_MARK_BLK
    MK_SLOTM
333:
    add     qword ptr [r8 + 72], 8
    jmp     332b
334:
.endm
RTX_FUNC(rt_gc_core_roots_run)
    RTX_PUSHS(rbx)
    RTX_PUSHS(rbp)
    RTX_PUSHS(r12)
    RTX_PUSHS(r13)
    RTX_PUSHS(r14)
    RTX_PUSHS(r15)
    mov     r8, rdi
    lea     rdi, [rip + g_gc_mk]
    mov     r11, qword ptr [rdi + 8]
    mov     rbx, qword ptr [rdi + 32]
    mov     r12, qword ptr [rdi + 48]
    mov     r14, qword ptr [rdi + 72]
    mov     r15, qword ptr [rdi + 96]
    mov     rax, qword ptr [r8 + 40]
    cmp     rax, 0
    je      .Lcr_nv
    cmp     rax, 1
    je      .Lcr_fn
    cmp     rax, 2
    je      .Lcr_dt
    xor     eax, eax
    jmp     .Lcr_out
.Lcr_nv:
    mov     rax, qword ptr [r8 + 56]
    cmp     rax, qword ptr [r8 + 8]
    jae     .Lcr_nv_done
    mov     rsi, qword ptr [r8 + 48]
    test    rsi, rsi
    jne     .Lcr_nv_e
    mov     rdx, qword ptr [r8]
    test    rdx, rdx
    je      .Lcr_nv_done
    mov     rsi, qword ptr [rdx + rax*8]
.Lcr_nv_e:
    test    rsi, rsi
    je      .Lcr_nv_nb
    mov     qword ptr [r8 + 48], rsi
    mov     rax, qword ptr [rdi + 40]
    sub     rax, rbx
    cmp     rax, 3
    jl      .Lcr_full_slots
    mov     rax, qword ptr [rdi + 16]
    sub     rax, r11
    cmp     rax, 1
    jl      .Lcr_full_wl
    mov     r10, qword ptr [rsi]
    test    r10, r10
    je      .Lcr_nv1
    MK_RAW rsi
.Lcr_nv1:
    mov     r10, qword ptr [rsi + 72]
    test    r10, r10
    je      .Lcr_nv2
    MK_RAW rsi+72
.Lcr_nv2:
    lea     r10, [rsi + 32]
    MK_WLPUSH r10
    mov     r10, qword ptr [rsi + 48]
    test    r10, r10
    je      .Lcr_nv3
    MK_RAW rsi+48
.Lcr_nv3:
    mov     rsi, qword ptr [rsi + 72]
    jmp     .Lcr_nv_e
.Lcr_nv_nb:
    inc     qword ptr [r8 + 56]
    mov     qword ptr [r8 + 48], 0
    jmp     .Lcr_nv
.Lcr_nv_done:
    mov     qword ptr [r8 + 40], 1
    mov     qword ptr [r8 + 56], 0
    mov     qword ptr [r8 + 48], 0
.Lcr_fn:
    mov     rax, qword ptr [r8 + 56]
    cmp     rax, qword ptr [r8 + 24]
    jae     .Lcr_fn_done
    mov     rsi, qword ptr [r8 + 48]
    test    rsi, rsi
    jne     .Lcr_fn_e
    mov     rax, qword ptr [rdi + 40]
    sub     rax, rbx
    cmp     rax, 1
    jl      .Lcr_full_slots
    mov     rdx, qword ptr [r8 + 16]
    mov     rax, qword ptr [r8 + 56]
    lea     rdx, [rdx + rax*8]
    mov     r10, qword ptr [rdx]
    test    r10, r10
    je      .Lcr_fn_nb
    mov     qword ptr [rdi + 176], rdx
    MK_BLK_OF r10
    test    rax, rax
    je      .Lcr_fn_h
    MK_MARK_BLK
    MK_SLOTM
.Lcr_fn_h:
    mov     rsi, qword ptr [rdi + 176]
    mov     rsi, qword ptr [rsi]
.Lcr_fn_e:
    test    rsi, rsi
    je      .Lcr_fn_nb
    mov     qword ptr [r8 + 48], rsi
    movsxd  rax, dword ptr [rsi + 32]
    movsxd  rdx, dword ptr [rsi + 48]
    add     rax, rdx
    add     rax, 6
    mov     rdx, qword ptr [rdi + 40]
    sub     rdx, rbx
    cmp     rdx, rax
    jl      .Lcr_full_slots
    mov     r10, qword ptr [rsi]
    test    r10, r10
    je      .Lcr_fn1
    MK_RAW rsi
.Lcr_fn1:
    mov     r10, qword ptr [rsi + 8]
    test    r10, r10
    je      .Lcr_fn2
    MK_RAW rsi+8
.Lcr_fn2:
    mov     r10, qword ptr [rsi + 16]
    test    r10, r10
    je      .Lcr_fn3
    MK_RAW rsi+16
.Lcr_fn3:
    mov     r10, qword ptr [rsi + 40]
    test    r10, r10
    je      .Lcr_fn4
    MK_RAW rsi+40
    mov     rdx, qword ptr [rsi + 40]
    mov     qword ptr [r8 + 72], rdx
    movsxd  rax, dword ptr [rsi + 32]
    lea     rax, [rdx + rax*8]
    mov     qword ptr [r8 + 64], rax
    MK_PTRS
.Lcr_fn4:
    mov     r10, qword ptr [rsi + 56]
    test    r10, r10
    je      .Lcr_fn5
    MK_RAW rsi+56
    mov     rdx, qword ptr [rsi + 56]
    mov     qword ptr [r8 + 72], rdx
    movsxd  rax, dword ptr [rsi + 48]
    lea     rax, [rdx + rax*8]
    mov     qword ptr [r8 + 64], rax
    MK_PTRS
.Lcr_fn5:
    mov     r10, qword ptr [rsi + 80]
    test    r10, r10
    je      .Lcr_fn6
    MK_RAW rsi+80
.Lcr_fn6:
    mov     rsi, qword ptr [rsi + 80]
    jmp     .Lcr_fn_e
.Lcr_fn_nb:
    inc     qword ptr [r8 + 56]
    mov     qword ptr [r8 + 48], 0
    jmp     .Lcr_fn
.Lcr_fn_done:
    mov     qword ptr [r8 + 40], 2
    mov     qword ptr [r8 + 56], 0
    mov     qword ptr [r8 + 48], 0
.Lcr_dt:
    mov     rsi, qword ptr [r8 + 48]
    test    rsi, rsi
    jne     .Lcr_dt_e
    cmp     qword ptr [r8 + 56], 0
    jne     .Lcr_dt_done
    mov     qword ptr [r8 + 56], 1
    mov     rsi, qword ptr [r8 + 32]
.Lcr_dt_e:
    test    rsi, rsi
    je      .Lcr_dt_done
    mov     qword ptr [r8 + 48], rsi
    movsxd  rax, dword ptr [rsi + 8]
    add     rax, 3
    mov     rdx, qword ptr [rdi + 40]
    sub     rdx, rbx
    cmp     rdx, rax
    jl      .Lcr_full_slots
    mov     r10, qword ptr [rsi]
    test    r10, r10
    je      .Lcr_dt1
    MK_RAW rsi
.Lcr_dt1:
    mov     r10, qword ptr [rsi + 16]
    test    r10, r10
    je      .Lcr_dt2
    MK_RAW rsi+16
    mov     rdx, qword ptr [rsi + 16]
    mov     qword ptr [r8 + 72], rdx
    movsxd  rax, dword ptr [rsi + 8]
    lea     rax, [rdx + rax*8]
    mov     qword ptr [r8 + 64], rax
    MK_PTRS
.Lcr_dt2:
    mov     r10, qword ptr [rsi + 24]
    test    r10, r10
    je      .Lcr_dt3
    MK_RAW rsi+24
.Lcr_dt3:
    mov     rsi, qword ptr [rsi + 24]
    jmp     .Lcr_dt_e
.Lcr_dt_done:
    mov     qword ptr [r8 + 40], 3
    xor     eax, eax
    jmp     .Lcr_out
.Lcr_full_slots:
    mov     eax, 1
    jmp     .Lcr_out
.Lcr_full_wl:
    mov     eax, 2
.Lcr_out:
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
RTX_ENDF(rt_gc_core_roots_run)
RTX_FUNC(rt_gc_trail_roots_run)
    RTX_PUSHS(rbx)
    RTX_PUSHS(rbp)
    RTX_PUSHS(r12)
    RTX_PUSHS(r13)
    RTX_PUSHS(r14)
    RTX_PUSHS(r15)
    mov     r8, rdi
    lea     rdi, [rip + g_gc_mk]
    mov     r11, qword ptr [rdi + 8]
    mov     rbx, qword ptr [rdi + 32]
    mov     r12, qword ptr [rdi + 48]
    mov     r14, qword ptr [rdi + 72]
    mov     r15, qword ptr [rdi + 96]
.Ltr2_range:
    mov     rax, qword ptr [r8 + 16]
    cmp     rax, qword ptr [r8 + 8]
    jae     .Ltr2_done
    shl     rax, 4
    add     rax, qword ptr [r8]
    cmp     qword ptr [rax + 8], 0
    jne     .Ltr2_next
    mov     rdx, qword ptr [rax]
    mov     rcx, qword ptr [rdx]
    mov     qword ptr [r8 + 40], rcx
    mov     rsi, qword ptr [r8 + 24]
    test    rsi, rsi
    jne     .Ltr2_entry
    lea     rsi, [rdx + 64]
.Ltr2_entry:
    lea     rax, [rsi + 32]
    cmp     rax, qword ptr [r8 + 40]
    ja      .Ltr2_next
    mov     qword ptr [r8 + 24], rsi
    mov     rax, qword ptr [rdi + 40]
    sub     rax, rbx
    cmp     rax, 1
    jl      .Ltr2_full_slots
    mov     rax, qword ptr [rdi + 16]
    sub     rax, r11
    cmp     rax, 1
    jl      .Ltr2_full_wl
    mov     r10, qword ptr [rsi]
    test    r10, r10
    je      .Ltr2_pad
    MK_RAW rsi
    jmp     .Ltr2_old
.Ltr2_pad:
    cmp     qword ptr [rsi + 8], 0
    je      .Ltr2_old
    inc     qword ptr [r8 + 32]
.Ltr2_old:
    lea     r10, [rsi + 16]
    MK_WLPUSH r10
    add     rsi, 32
    jmp     .Ltr2_entry
.Ltr2_next:
    inc     qword ptr [r8 + 16]
    mov     qword ptr [r8 + 24], 0
    jmp     .Ltr2_range
.Ltr2_done:
    xor     eax, eax
    jmp     .Ltr2_out
.Ltr2_full_slots:
    mov     eax, 1
    jmp     .Ltr2_out
.Ltr2_full_wl:
    mov     eax, 2
.Ltr2_out:
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
RTX_ENDF(rt_gc_trail_roots_run)
RTX_FUNC(rt_gc_proc_roots_run)
    RTX_PUSHS(rbx)
    RTX_PUSHS(rbp)
    RTX_PUSHS(r12)
    RTX_PUSHS(r13)
    RTX_PUSHS(r14)
    RTX_PUSHS(r15)
    mov     r8, rdi
    lea     rdi, [rip + g_gc_mk]
    mov     r11, qword ptr [rdi + 8]
    mov     rbx, qword ptr [rdi + 32]
    mov     r12, qword ptr [rdi + 48]
    mov     r14, qword ptr [rdi + 72]
    mov     r15, qword ptr [rdi + 96]
.Lpr_loop:
    mov     rax, qword ptr [r8 + 16]
    cmp     rax, qword ptr [r8 + 8]
    jae     .Lpr_done
    shl     rax, 7
    add     rax, qword ptr [r8]
    mov     rsi, rax
    movsxd  rax, dword ptr [rsi + 24]
    lea     rax, [rax + rax + 5]
    mov     rdx, qword ptr [rdi + 40]
    sub     rdx, rbx
    cmp     rdx, rax
    jl      .Lpr_full
    mov     r10, qword ptr [rsi]
    test    r10, r10
    je      .Lpr1
    MK_RAW rsi
.Lpr1:
    mov     r10, qword ptr [rsi + 16]
    test    r10, r10
    je      .Lpr2
    MK_RAW rsi+16
    mov     rdx, qword ptr [rsi + 16]
    mov     qword ptr [r8 + 72], rdx
    movsxd  rax, dword ptr [rsi + 24]
    lea     rax, [rdx + rax*8]
    mov     qword ptr [r8 + 64], rax
    MK_PTRS
.Lpr2:
    mov     r10, qword ptr [rsi + 56]
    test    r10, r10
    je      .Lpr3
    MK_RAW rsi+56
    mov     rdx, qword ptr [rsi + 56]
    mov     qword ptr [r8 + 72], rdx
    movsxd  rax, dword ptr [rsi + 24]
    lea     rax, [rdx + rax*8]
    mov     qword ptr [r8 + 64], rax
    MK_PTRS
.Lpr3:
    mov     r10, qword ptr [rsi + 64]
    test    r10, r10
    je      .Lpr4
    MK_RAW rsi+64
.Lpr4:
    mov     r10, qword ptr [rsi + 88]
    test    r10, r10
    je      .Lpr5
    MK_RAW rsi+88
.Lpr5:
    inc     qword ptr [r8 + 16]
    jmp     .Lpr_loop
.Lpr_done:
    xor     eax, eax
    jmp     .Lpr_out
.Lpr_full:
    mov     eax, 1
.Lpr_out:
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
RTX_ENDF(rt_gc_proc_roots_run)
RTX_FUNC(rt_gc_site_find_a)
    RTX_PUSHS(rbx)
    RTX_PUSHS(rbp)
    mov     r10, rdx
    xor     eax, eax
    lea     rdx, [rcx - 1]
    mov     rcx, -1
.Lsf_bs:
    cmp     rax, rdx
    jg      .Lsf_scan
    lea     r8, [rax + rdx]
    sar     r8, 1
    mov     rbx, r8
    shl     rbx, 4
    lea     rbx, [rbx + rbx*4]
    add     rbx, r10
    cmp     qword ptr [rbx + 16], rdi
    ja      .Lsf_bs_hi
    mov     rcx, r8
    lea     rax, [r8 + 1]
    jmp     .Lsf_bs
.Lsf_bs_hi:
    lea     rdx, [r8 - 1]
    jmp     .Lsf_bs
.Lsf_scan:
    test    rcx, rcx
    js      .Lsf_none
    mov     rbx, rcx
    shl     rbx, 4
    lea     rbx, [rbx + rbx*4]
    add     rbx, r10
    cmp     qword ptr [rbx + 32], rdi
    jbe     .Lsf_none
    cmp     rdi, qword ptr [rbx + 16]
    jb      .Lsf_next
    cmp     rdi, qword ptr [rbx + 24]
    jae     .Lsf_next
    xor     eax, eax
    movsxd  rdx, dword ptr [rbx + 40]
    dec     rdx
    mov     r8, qword ptr [rbx + 64]
.Lsf_es:
    cmp     rax, rdx
    jg      .Lsf_next
    lea     rbp, [rax + rdx]
    sar     rbp, 1
    mov     r11, rbp
    shl     r11, 4
    add     r11, r8
    cmp     qword ptr [r11], rdi
    je      .Lsf_hit
    jb      .Lsf_es_lo
    lea     rdx, [rbp - 1]
    jmp     .Lsf_es
.Lsf_es_lo:
    lea     rax, [rbp + 1]
    jmp     .Lsf_es
.Lsf_hit:
    test    rsi, rsi
    je      .Lsf_hit_ret
    mov     qword ptr [rsi], r11
.Lsf_hit_ret:
    mov     rax, rbx
    jmp     .Lsf_out
.Lsf_next:
    dec     rcx
    jmp     .Lsf_scan
.Lsf_none:
    test    rsi, rsi
    je      .Lsf_none_ret
    mov     qword ptr [rsi], 0
.Lsf_none_ret:
    xor     eax, eax
.Lsf_out:
    RTX_POPS(rbp)
    RTX_POPS(rbx)
    ret
RTX_ENDF(rt_gc_site_find_a)
RTX_FUNC(rt_gc_flags_roots_run)
    RTX_PUSHS(rbx)
    RTX_PUSHS(rbp)
    RTX_PUSHS(r12)
    RTX_PUSHS(r13)
    RTX_PUSHS(r14)
    RTX_PUSHS(r15)
    mov     r8, rdi
    lea     rdi, [rip + g_gc_mk]
    mov     r11, qword ptr [rdi + 8]
    mov     rbx, qword ptr [rdi + 32]
    mov     r12, qword ptr [rdi + 48]
    mov     r14, qword ptr [rdi + 72]
    mov     r15, qword ptr [rdi + 96]
.Lfl_loop:
    mov     rax, qword ptr [r8 + 16]
    cmp     rax, qword ptr [r8 + 8]
    jae     .Lfl_done
    lea     rsi, [rax + rax*4]
    lea     rsi, [rax + rsi*4]
    shl     rsi, 3
    add     rsi, qword ptr [r8]
    mov     rax, qword ptr [rdi + 40]
    sub     rax, rbx
    cmp     rax, 2
    jl      .Lfl_full_slots
    mov     rax, qword ptr [rdi + 16]
    sub     rax, r11
    cmp     rax, 1
    jl      .Lfl_full_wl
    mov     r10, qword ptr [rsi]
    test    r10, r10
    je      .Lfl1
    MK_RAW rsi
.Lfl1:
    mov     r10, qword ptr [rsi + 8]
    test    r10, r10
    je      .Lfl2
    MK_RAW rsi+8
.Lfl2:
    cmp     dword ptr [rsi + 72], 0
    je      .Lfl3
    lea     r10, [rsi + 80]
    MK_WLPUSH r10
.Lfl3:
    inc     qword ptr [r8 + 16]
    jmp     .Lfl_loop
.Lfl_done:
    xor     eax, eax
    jmp     .Lfl_out
.Lfl_full_slots:
    mov     eax, 1
    jmp     .Lfl_out
.Lfl_full_wl:
    mov     eax, 2
.Lfl_out:
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
RTX_ENDF(rt_gc_flags_roots_run)
RTX_FUNC(rt_gc_ptr_fields_run)
    RTX_PUSHS(rbx)
    RTX_PUSHS(rbp)
    RTX_PUSHS(r12)
    RTX_PUSHS(r13)
    RTX_PUSHS(r14)
    RTX_PUSHS(r15)
    mov     r8, rdi
    lea     rdi, [rip + g_gc_mk]
    mov     rbx, qword ptr [rdi + 32]
    mov     r12, qword ptr [rdi + 48]
    mov     r14, qword ptr [rdi + 72]
    mov     r15, qword ptr [rdi + 96]
.Lpfr_loop:
    mov     rax, qword ptr [r8 + 24]
    cmp     rax, qword ptr [r8 + 8]
    jae     .Lpfr_done
    imul    rax, qword ptr [r8 + 16]
    add     rax, qword ptr [r8]
    mov     rsi, rax
    mov     rax, qword ptr [rdi + 40]
    sub     rax, rbx
    cmp     rax, qword ptr [r8 + 32]
    jl      .Lpfr_full
    mov     qword ptr [r8 + 80], 0
.Lpfr_field:
    mov     rax, qword ptr [r8 + 80]
    cmp     rax, qword ptr [r8 + 32]
    jae     .Lpfr_next
    mov     r13, qword ptr [r8 + rax*8 + 40]
    add     r13, rsi
    mov     r10, qword ptr [r13]
    test    r10, r10
    je      .Lpfr_skip
    mov     qword ptr [rdi + 176], r13
    MK_BLK_OF r10
    test    rax, rax
    je      .Lpfr_skip
    MK_MARK_BLK
    MK_SLOTM
.Lpfr_skip:
    inc     qword ptr [r8 + 80]
    jmp     .Lpfr_field
.Lpfr_next:
    inc     qword ptr [r8 + 24]
    jmp     .Lpfr_loop
.Lpfr_done:
    xor     eax, eax
    jmp     .Lpfr_out
.Lpfr_full:
    mov     eax, 1
.Lpfr_out:
    mov     qword ptr [rdi + 32], rbx
    mov     qword ptr [rdi + 96], r15
    RTX_POPS(r15)
    RTX_POPS(r14)
    RTX_POPS(r13)
    RTX_POPS(r12)
    RTX_POPS(rbp)
    RTX_POPS(rbx)
    ret
RTX_ENDF(rt_gc_ptr_fields_run)
RTX_FUNC(rt_gc_sw_links_a)
    RTX_PUSHS(rbx)
    RTX_PUSHS(rbp)
    RTX_PUSHS(r12)
    RTX_PUSHS(r13)
    RTX_PUSHS(r14)
    RTX_PUSHS(r15)
    mov     r12, rdi
    mov     rbx, qword ptr [r12 + 8]
    test    rbx, rbx
    je      .Lln_ok
    test    dword ptr [rbx + 12], 8
    je      .Lln_ok
    lea     r13, [rbx + 32]
    mov     r14, qword ptr [r12]
    mov     r15, qword ptr [r12 + 32]
.Lln_j:
    mov     rax, qword ptr [r12 + 184]
    cmp     rax, qword ptr [r13]
    jae     .Lln_ok
    mov     rdx, qword ptr [r13 + 8 + rax*8]
    mov     rcx, rdx
    shr     rcx, 32
    and     ecx, 0xFF
    cmp     ecx, 4
    je      .Lln_kind
    cmp     ecx, 5
    jne     .Lln_next_j
.Lln_kind:
    mov     qword ptr [r12 + 200], rcx
    mov     rcx, rdx
    shr     rcx, 40
    and     ecx, 0xFFFF
    mov     qword ptr [r12 + 208], rcx
    movsxd  rdx, edx
    add     rdx, r14
    mov     qword ptr [r12 + 216], rdx
.Lln_k:
    mov     rax, qword ptr [r12 + 192]
    cmp     rax, qword ptr [r12 + 208]
    jae     .Lln_next_j
    mov     rbp, qword ptr [r12 + 216]
    add     rbp, rax
    mov     rsi, qword ptr [rbp]
    test    rsi, rsi
    je      .Lln_next_k
    cmp     qword ptr [r12 + 24], -1
    jne     .Lln_rule_ok
    cmp     qword ptr [r12 + 16], 0
    jne     .Lln_rule_ok
    cmp     rsi, r15
    jb      .Lln_next_k
    cmp     rsi, qword ptr [r12 + 40]
    jae     .Lln_next_k
.Lln_rule_ok:
    mov     qword ptr [r12 + 224], 0
    cmp     qword ptr [r12 + 200], 5
    jne     .Lln_beta_c
    mov     qword ptr [r12 + 224], rsi
    cmp     rsi, r15
    jb      .Lln_refuse
    lea     rax, [rsi + 48]
    cmp     rax, qword ptr [r12 + 40]
    ja      .Lln_refuse
    cmp     rsi, r14
    jae     .Lln_refuse
    mov     rdi, qword ptr [rsi + 32]
    jmp     .Lln_beta_ok
.Lln_beta_c:
    mov     rdi, qword ptr [rbp + 8]
.Lln_beta_ok:
    mov     qword ptr [r12 + 232], rdi
    mov     qword ptr [r12 + 240], rsi
    cmp     qword ptr [r12 + 104], rdi
    jne     .Lln_m1
    mov     rax, qword ptr [r12 + 136]
    test    rax, rax
    jne     .Lln_have_b
.Lln_m1:
    cmp     qword ptr [r12 + 112], rdi
    jne     .Lln_m2
    mov     rax, qword ptr [r12 + 144]
    test    rax, rax
    jne     .Lln_have_b
.Lln_m2:
    cmp     qword ptr [r12 + 120], rdi
    jne     .Lln_m3
    mov     rax, qword ptr [r12 + 152]
    test    rax, rax
    jne     .Lln_have_b
.Lln_m3:
    cmp     qword ptr [r12 + 128], rdi
    jne     .Lln_cb
    mov     rax, qword ptr [r12 + 160]
    test    rax, rax
    jne     .Lln_have_b
.Lln_cb:
    lea     rax, [rip + g_gc_mk]
    cmp     qword ptr [rax + 304], 0
    jne     .Lln_decline
    mov     r10, qword ptr [r12 + 88]
    xor     eax, eax
    mov     rdx, qword ptr [r12 + 96]
    dec     rdx
    mov     rcx, -1
.Lln_bs:
    cmp     rax, rdx
    jg      .Lln_bs_done
    lea     r11, [rax + rdx]
    sar     r11, 1
    mov     rsi, r11
    shl     rsi, 4
    lea     rsi, [rsi + rsi*4]
    add     rsi, r10
    cmp     qword ptr [rsi + 16], rdi
    ja      .Lln_bs_hi
    mov     rcx, r11
    lea     rax, [r11 + 1]
    jmp     .Lln_bs
.Lln_bs_hi:
    lea     rdx, [r11 - 1]
    jmp     .Lln_bs
.Lln_bs_done:
    test    rcx, rcx
    js      .Lln_cb_none
    mov     rsi, rcx
    shl     rsi, 4
    lea     rsi, [rsi + rsi*4]
    add     rsi, r10
    cmp     qword ptr [rsi + 8], 0
    je      .Lln_sb_prev
    cmp     qword ptr [rsi + 72], 0
    jne     .Lln_sb_hit
.Lln_sb_prev:
    dec     rcx
    jmp     .Lln_bs_done
.Lln_sb_hit:
    mov     rax, rsi
    mov     rdx, qword ptr [rsi + 72]
    mov     r11, rcx
.Lln_nb:
    dec     r11
    js      .Lln_cb_store
    mov     rsi, r11
    shl     rsi, 4
    lea     rsi, [rsi + rsi*4]
    add     rsi, r10
    cmp     qword ptr [rsi + 16], rdx
    jne     .Lln_cb_store
    cmp     qword ptr [rsi + 8], 0
    je      .Lln_nb
    cmp     qword ptr [rsi + 72], rdx
    jne     .Lln_nb
    mov     rsi, qword ptr [rsi + 8]
    cmp     rsi, qword ptr [rax + 8]
    je      .Lln_nb
.Lln_cb_none:
    xor     eax, eax
.Lln_cb_store:
    mov     rcx, qword ptr [r12 + 168]
    and     ecx, 3
    mov     qword ptr [r12 + 104 + rcx*8], rdi
    mov     qword ptr [r12 + 136 + rcx*8], rax
    inc     qword ptr [r12 + 168]
.Lln_have_b:
    test    rax, rax
    je      .Lln_refuse
    mov     rcx, rax
    mov     rdx, qword ptr [rcx + 8]
    mov     rsi, qword ptr [r12 + 240]
    cmp     qword ptr [r12 + 224], 0
    je      .Lln_nohdr
    test    dword ptr [rdx + 12], 2
    je      .Lln_refuse
    mov     rsi, qword ptr [r12 + 224]
    mov     eax, dword ptr [rdx + 4]
    sub     rsi, rax
    cmp     rsi, r15
    jb      .Lln_refuse
    mov     qword ptr [r12 + 240], rsi
.Lln_nohdr:
    cmp     rsi, r15
    jb      .Lln_refuse
    mov     rax, qword ptr [rdx + 24]
    add     rax, rsi
    add     rax, 16
    mov     edi, dword ptr [rdx + 8]
    add     rax, rdi
    cmp     rax, qword ptr [r12 + 40]
    ja      .Lln_refuse
    cmp     rsi, r14
    jae     .Lln_refuse
    mov     rax, qword ptr [rcx + 56]
    bt      rax, 63
    jnc     .Lln_refuse
    mov     qword ptr [r12 + 248], rcx
    and     rax, 0x0FFFFFFF
    mov     rdi, qword ptr [rsi + rax]
    lea     rsi, [r12 + 256]
    mov     rdx, qword ptr [r12 + 88]
    mov     rcx, qword ptr [r12 + 96]
    RTX_SUB_RSP(8)
    RTX_CALL(rt_gc_site_find_a)
    RTX_ADD_RSP(8)
    test    rax, rax
    je      .Lln_refuse
    cmp     qword ptr [r12 + 256], 0
    je      .Lln_refuse
    cmp     qword ptr [rax + 8], rbx
    jne     .Lln_refuse
    mov     rsi, qword ptr [r12 + 240]
    cmp     rsi, qword ptr [r12 + 48]
    je      .Lln_next_k
    cmp     rsi, qword ptr [r12 + 56]
    jb      .Lln_refuse
    mov     rcx, qword ptr [r12 + 64]
    mov     rdx, qword ptr [r12 + 72]
    xor     eax, eax
.Lln_dd:
    cmp     rax, rdx
    jae     .Lln_push
    mov     rdi, rax
    shl     rdi, 5
    cmp     qword ptr [rcx + rdi], rsi
    je      .Lln_next_k
    inc     rax
    jmp     .Lln_dd
.Lln_push:
    cmp     rdx, qword ptr [r12 + 80]
    jae     .Lln_grow
    mov     rdi, rdx
    shl     rdi, 5
    add     rdi, rcx
    mov     qword ptr [rdi], rsi
    mov     rax, qword ptr [r12 + 248]
    mov     rcx, qword ptr [rax + 8]
    mov     qword ptr [rdi + 8], rcx
    mov     rcx, qword ptr [rax + 56]
    mov     qword ptr [rdi + 16], rcx
    mov     dword ptr [rdi + 24], -1
    mov     dword ptr [rdi + 28], 0
    inc     qword ptr [r12 + 72]
    mov     rax, qword ptr [r12 + 176]
    inc     qword ptr [rax]
.Lln_next_k:
    add     qword ptr [r12 + 192], 8
    jmp     .Lln_k
.Lln_next_j:
    inc     qword ptr [r12 + 184]
    mov     qword ptr [r12 + 192], 0
    jmp     .Lln_j
.Lln_ok:
    mov     eax, 1
    jmp     .Lln_out
.Lln_refuse:
    xor     eax, eax
    jmp     .Lln_out
.Lln_grow:
    mov     eax, 4
    jmp     .Lln_out
.Lln_decline:
    mov     eax, 5
.Lln_out:
    RTX_POPS(r15)
    RTX_POPS(r14)
    RTX_POPS(r13)
    RTX_POPS(r12)
    RTX_POPS(rbp)
    RTX_POPS(rbx)
    ret
RTX_ENDF(rt_gc_sw_links_a)
.section .note.GNU-stack,"",@progbits
