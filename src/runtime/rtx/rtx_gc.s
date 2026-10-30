#include "rtx_abi.inc"
#define HBF_TTL 0x0001
#define HB_FILL 202
#define HB_AGGV 206
#define HB_DINST 213
#define HB_DVEC 214
#define MK_MOVES 0x807FDE7
#define GC_FRAME_MAP_TBL_HDR 128
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
    cmp     edx, 27
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
    lea     rdx, [rip + g_runtime + G_RT_OFF_GC_MK]
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
    lea     rax, [rip + g_runtime + G_RT_OFF_GC_MK]
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
    lea     rax, [rip + g_runtime + G_RT_OFF_GC_MK]
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
    je      .Lmk_m_pldb1
    MK_RAW rsi
.Lmk_m_pldb1:
    mov     r10, qword ptr [rsi + 64]
    test    r10, r10
    je      .Lmk_loop
    MK_RAW rsi+64
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
    je      .Lmk_m_pldbr3
    MK_RAW rsi+32
.Lmk_m_pldbr3:
    mov     r10, qword ptr [rsi + 56]
    test    r10, r10
    je      .Lmk_loop
    MK_RAW rsi+56
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
.byte MK_SLOW
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
    lea     rdi, [rip + g_runtime + G_RT_OFF_GC_MK]
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
    lea     rdi, [rip + g_runtime + G_RT_OFF_GC_MK]
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
    lea     rdi, [rip + g_runtime + G_RT_OFF_GC_MK]
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
    lea     rsi, [rdx + 128]
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
    lea     rdi, [rip + g_runtime + G_RT_OFF_GC_MK]
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
    sub     rdi, qword ptr [rbx + 72]
.Lsf_es:
    cmp     rax, rdx
    jg      .Lsf_es_miss
    lea     rbp, [rax + rdx]
    sar     rbp, 1
    lea     r11, [rbp + rbp*2]
    shl     r11, 3
    add     r11, r8
    cmp     qword ptr [r11], rdi
    je      .Lsf_hit
    jb      .Lsf_es_lo
    lea     rdx, [rbp - 1]
    jmp     .Lsf_es
.Lsf_es_lo:
    lea     rax, [rbp + 1]
    jmp     .Lsf_es
.Lsf_es_miss:
    add     rdi, qword ptr [rbx + 72]
    jmp     .Lsf_next
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
    lea     rdi, [rip + g_runtime + G_RT_OFF_GC_MK]
    mov     r11, qword ptr [rdi + 8]
    mov     rbx, qword ptr [rdi + 32]
    mov     r12, qword ptr [rdi + 48]
    mov     r14, qword ptr [rdi + 72]
    mov     r15, qword ptr [rdi + 96]
.Lfl_loop:
    mov     rax, qword ptr [r8 + 16]
    cmp     rax, qword ptr [r8 + 8]
    jae     .Lfl_done
    lea     rsi, [rax + rax*2]
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
    lea     rdi, [rip + g_runtime + G_RT_OFF_GC_MK]
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
    mov     qword ptr [r8 + 48], 0
.Lpfr_field:
    mov     rax, qword ptr [r8 + 48]
    cmp     rax, qword ptr [r8 + 32]
    jae     .Lpfr_next
    mov     rcx, qword ptr [r8 + 40]
    mov     r13, qword ptr [rcx + rax*8]
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
    inc     qword ptr [r8 + 48]
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
RTX_FUNC(rt_gc_loc_list_run)
    RTX_PUSHS(rbx)
    RTX_PUSHS(rbp)
    RTX_PUSHS(r12)
    RTX_PUSHS(r13)
    RTX_PUSHS(r14)
    RTX_PUSHS(r15)
    mov     r8, rdi
    lea     rdi, [rip + g_runtime + G_RT_OFF_GC_MK]
    mov     rbx, qword ptr [rdi + 32]
    mov     r12, qword ptr [rdi + 48]
    mov     r14, qword ptr [rdi + 72]
    mov     r15, qword ptr [rdi + 96]
.Lll_loop:
    mov     rax, qword ptr [r8 + 16]
    cmp     rax, qword ptr [r8 + 8]
    jae     .Lll_done
    mov     rsi, qword ptr [r8]
    mov     r13, qword ptr [rsi + rax*8]
    mov     r10, qword ptr [r13]
    test    r10, r10
    je      .Lll_next
    mov     rax, qword ptr [rdi + 40]
    sub     rax, rbx
    jle     .Lll_full
    mov     qword ptr [rdi + 176], r13
    MK_BLK_OF r10
    test    rax, rax
    je      .Lll_next
    MK_MARK_BLK
    MK_SLOTM
.Lll_next:
    inc     qword ptr [r8 + 16]
    jmp     .Lll_loop
.Lll_done:
    xor     eax, eax
    jmp     .Lll_out
.Lll_full:
    mov     eax, 1
.Lll_out:
    mov     qword ptr [rdi + 32], rbx
    mov     qword ptr [rdi + 96], r15
    RTX_POPS(r15)
    RTX_POPS(r14)
    RTX_POPS(r13)
    RTX_POPS(r12)
    RTX_POPS(rbp)
    RTX_POPS(rbx)
    ret
RTX_ENDF(rt_gc_loc_list_run)
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
    lea     rax, [rip + g_runtime + G_RT_OFF_GC_MK]
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
#define SW_LO 0
#define SW_HI 8
#define SW_P 16
#define SW_PREV 24
#define SW_LASTNAME 32
#define SW_ROOTTOP 40
#define SW_N 48
#define SW_RUN 56
#define SW_STOP 64
#define SW_LASTF 88
#define SW_GAPV 120
#define SW_GAPN 128
#define SW_GAPCAP 136
#define LA_BASE 144
#define LA_FBASE 144
#define LA_FMAP 152
#define LA_FLINK 160
#define LA_FRULE 168
#define LA_PREV 192
#define LA_GLO 200
#define LA_GAPV 208
#define LA_GAPN 216
#define LA_GAPCAP 224
#define LA_SBLK 232
#define LA_SBLKN 240
#define LA_POSJ 328
#define LA_POSK 336
#define X_PC 408
#define X_R 416
#define X_RBP 424
#define X_LAST 432
#define X_F 440
#define X_CUR 472
#define X_PHASE 504
#define X_I 512
#define X_SUB 520
#define X_COBODY 528
#define X_GLO 536
#define X_W1LO 544
#define X_W1HI 552
#define X_CLO 560
#define X_CHI 568
#define X_W2LO 576
#define X_W2HI 584
#define X_PNEW 592
#define X_CELLS 600
#define X_E 608
#define X_DPC 616
#define X_DR 624
#define X_DRBP 632
#define X_LAM 640
#define X_PW 648
#define X_GW 656
#define X_TOPG 664
.macro SW_SITE_FIND pc, eo
    mov     rdi, \pc
    mov     rsi, \eo
    mov     rdx, qword ptr [r12 + LA_SBLK]
    mov     rcx, qword ptr [r12 + LA_SBLKN]
    RTX_CALL(rt_gc_site_find_a)
.endm
.macro SW_LINKS_CALL
    mov     rax, qword ptr [r12 + SW_PREV]
    mov     qword ptr [r12 + LA_PREV], rax
    mov     rax, qword ptr [r12 + X_GLO]
    mov     qword ptr [r12 + LA_GLO], rax
    mov     rax, qword ptr [r12 + SW_GAPV]
    mov     qword ptr [r12 + LA_GAPV], rax
    mov     rax, qword ptr [r12 + SW_GAPN]
    mov     qword ptr [r12 + LA_GAPN], rax
    mov     rax, qword ptr [r12 + SW_GAPCAP]
    mov     qword ptr [r12 + LA_GAPCAP], rax
    lea     rdi, [r12 + LA_BASE]
    RTX_CALL(rt_gc_sw_links_a)
    mov     rdx, qword ptr [r12 + LA_GAPN]
    mov     qword ptr [r12 + SW_GAPN], rdx
.endm
.macro SW_DRAIN_CLEAR
    lea     rax, [rip + g_runtime + G_RT_OFF_GC_MK]
    mov     qword ptr [rax + 184], 0
.endm
.macro SW_COPY_CHF dst, src
    mov     rax, qword ptr [\src]
    mov     qword ptr [\dst], rax
    mov     rax, qword ptr [\src + 8]
    mov     qword ptr [\dst + 8], rax
    mov     rax, qword ptr [\src + 16]
    mov     qword ptr [\dst + 16], rax
    mov     rax, qword ptr [\src + 24]
    mov     qword ptr [\dst + 24], rax
.endm
.macro SW_SWAP32 p, q
    mov     r8, qword ptr [\p + 0]
    mov     r10, qword ptr [\q + 0]
    mov     qword ptr [\p + 0], r10
    mov     qword ptr [\q + 0], r8
    mov     r8, qword ptr [\p + 8]
    mov     r10, qword ptr [\q + 8]
    mov     qword ptr [\p + 8], r10
    mov     qword ptr [\q + 8], r8
    mov     r8, qword ptr [\p + 16]
    mov     r10, qword ptr [\q + 16]
    mov     qword ptr [\p + 16], r10
    mov     qword ptr [\q + 16], r8
    mov     r8, qword ptr [\p + 24]
    mov     r10, qword ptr [\q + 24]
    mov     qword ptr [\p + 24], r10
    mov     qword ptr [\q + 24], r8
.endm
RTX_FUNC(rt_gc_sw_step_a)
    RTX_PUSHS(rbx)
    RTX_PUSHS(rbp)
    RTX_PUSHS(r12)
    RTX_PUSHS(r13)
    RTX_PUSHS(r14)
    RTX_PUSHS(r15)
    RTX_SUB_RSP(8)
    mov     r12, rdi
    mov     rax, qword ptr [r12 + X_PHASE]
    cmp     rax, 1
    je      .Lsw_p1
    cmp     rax, 2
    je      .Lsw_p2
    cmp     rax, 4
    je      .Lsw_p4
    cmp     rax, 5
    je      .Lsw_p5
    mov     r14, qword ptr [r12 + X_R]
    cmp     r14, qword ptr [r12 + SW_HI]
    ja      .Lsw_stop7
    mov     rax, qword ptr [r12 + X_LAST]
    test    rax, rax
    je      .Lsw_find
    cmp     r14, rax
    jb      .Lsw_stop7
.Lsw_find:
    mov     rbx, qword ptr [r12 + X_PC]
    mov     r15, qword ptr [r12 + X_RBP]
    test    r14, r14
    je      .Lsw_stop1
    mov     qword ptr [r12 + X_LAM], 0
    mov     qword ptr [r12 + X_PW], 1
    mov     qword ptr [r12 + X_DPC], 0
    mov     qword ptr [r12 + X_DR], 0
    mov     qword ptr [r12 + X_DRBP], 0
.Lsw_again:
    cmp     rbx, qword ptr [r12 + X_DPC]
    jne     .Lsw_nocyc
    cmp     r14, qword ptr [r12 + X_DR]
    jne     .Lsw_nocyc
    cmp     r15, qword ptr [r12 + X_DRBP]
    je      .Lsw_stop4
.Lsw_nocyc:
    mov     rax, qword ptr [r12 + X_LAM]
    inc     rax
    mov     qword ptr [r12 + X_LAM], rax
    cmp     rax, qword ptr [r12 + X_PW]
    jne     .Lsw_brent_done
    shl     qword ptr [r12 + X_PW], 1
    mov     qword ptr [r12 + X_LAM], 0
    mov     qword ptr [r12 + X_DPC], rbx
    mov     qword ptr [r12 + X_DR], r14
    mov     qword ptr [r12 + X_DRBP], r15
.Lsw_brent_done:
    mov     rax, qword ptr [rip + rt_tge_gamma_ret@GOTPCREL]
    cmp     rbx, rax
    je      .Lsw_gamma
    mov     rax, qword ptr [rip + rt_tge_omega_ret@GOTPCREL]
    cmp     rbx, rax
    je      .Lsw_omega
    mov     rax, qword ptr [rip + rt_genp_n2_conts@GOTPCREL]
    cmp     rbx, qword ptr [rax]
    je      .Lsw_stop8
    cmp     rbx, qword ptr [rax + 8]
    je      .Lsw_stop8
    mov     rax, qword ptr [rip + rt_genp_conts@GOTPCREL]
    cmp     rbx, qword ptr [rax]
    je      .Lsw_stop8
    cmp     rbx, qword ptr [rax + 8]
    je      .Lsw_stop8
    cmp     rbx, qword ptr [rax + 16]
    je      .Lsw_stop8
    lea     rsi, [r12 + X_E]
    SW_SITE_FIND rbx, rsi
    mov     r13, rax
    test    r13, r13
    je      .Lsw_noent
    mov     rbp, qword ptr [r12 + X_E]
    test    rbp, rbp
    je      .Lsw_noent
    movzx   eax, word ptr [rbp + 8]
    and     eax, 255
    cmp     eax, 7
    jne     .Lsw_have_site
    movsxd  rax, dword ptr [rbp + 12]
    mov     rbx, qword ptr [r14 + rax]
    lea     r14, [r14 + rax + 8]
    jmp     .Lsw_again
.Lsw_gamma:
    mov     rcx, qword ptr [r14]
    lea     rax, [rcx*8 + 39]
    and     rax, -16
    shl     rcx, 4
    add     rax, rcx
    add     r14, rax
    mov     rbx, qword ptr [r14 + 8]
    jmp     .Lsw_again
.Lsw_omega:
    mov     rcx, qword ptr [r14]
    lea     rax, [rcx*8 + 39]
    and     rax, -16
    shl     rcx, 4
    add     rax, rcx
    add     r14, rax
    mov     rbx, qword ptr [r14 + 16]
    jmp     .Lsw_again
.Lsw_noent:
    mov     r13, r15
.Lsw_scan_l:
    test    r13, r13
    je      .Lsw_stop1
    cmp     r13, r14
    jbe     .Lsw_stop1
    lea     rax, [r13 + 16]
    cmp     rax, qword ptr [r12 + SW_HI]
    ja      .Lsw_stop1
    mov     rbp, qword ptr [r13 + 8]
    mov     rax, qword ptr [rip + rt_genp_n2_deliver_ret@GOTPCREL]
    cmp     rbp, rax
    je      .Lsw_decline
    lea     rsi, [r12 + X_E]
    SW_SITE_FIND rbp, rsi
    test    rax, rax
    je      .Lsw_scan_next
    cmp     qword ptr [r12 + X_E], 0
    je      .Lsw_scan_next
    mov     rbx, rbp
    lea     r14, [r13 + 16]
    mov     r15, qword ptr [r13]
    mov     r13, rax
    mov     rbp, qword ptr [r12 + X_E]
    jmp     .Lsw_have_site
.Lsw_scan_next:
    mov     rax, qword ptr [r13]
    cmp     rax, r13
    jbe     .Lsw_stop1
    mov     r13, rax
    jmp     .Lsw_scan_l
.Lsw_have_site:
    movzx   eax, word ptr [rbp + 10]
    and     eax, 255
    cmp     eax, 9
    jne     .Lsw_not9
    movzx   ecx, word ptr [rbp + 8]
    shr     ecx, 8
    and     ecx, 255
    lea     rcx, [r14 + rcx*8]
    mov     edx, dword ptr [rbp + 12]
    mov     eax, edx
    and     eax, 0xffff
    shr     edx, 16
    and     edx, 0xffff
    mov     rax, qword ptr [rcx + rax]
    mov     rbx, qword ptr [rax + 8]
    lea     r14, [rcx + rdx + 16]
    jmp     .Lsw_again
.Lsw_not9:
    cmp     qword ptr [r13 + 8], 0
    je      .Lsw_decline
    mov     rdx, qword ptr [r13 + 8]
    test    dword ptr [rdx + 12], GC_FRAME_MAP_TBL_HDR
    jne     .Lsw_decline
    mov     qword ptr [r12 + X_CLO], 0
    cmp     eax, 6
    jne     .Lsw_r_not6
    movzx   ecx, word ptr [rbp + 8]
    mov     edx, ecx
    shr     ecx, 8
    and     ecx, 15
    shr     edx, 12
    and     edx, 15
    mov     rsi, r15
    mov     edi, 1
.Lsw_r6_l:
    cmp     edi, ecx
    jge     .Lsw_r6_d
    mov     rsi, qword ptr [rsi]
    inc     edi
    jmp     .Lsw_r6_l
.Lsw_r6_d:
    cmp     edx, 1
    je      .Lsw_r6_ind
    cmp     edx, 3
    je      .Lsw_r6_ind
    movsxd  rax, dword ptr [rbp + 12]
    lea     rax, [rsi + rax + 8]
    jmp     .Lsw_r6_b
.Lsw_r6_ind:
    mov     rax, qword ptr [rsi]
.Lsw_r6_b:
    test    edx, edx
    jne     .Lsw_base_done
    mov     rcx, qword ptr [rsi]
    mov     qword ptr [r12 + X_CLO], rcx
    test    rcx, rcx
    je      .Lsw_base_done
    jmp     .Lsw_chk0
.Lsw_r_not6:
    test    eax, eax
    jne     .Lsw_r1
    movsxd  rax, dword ptr [rbp + 12]
    add     rax, r14
    jmp     .Lsw_chk0
.Lsw_r1:
    cmp     eax, 1
    je      .Lsw_r_rbp
    cmp     eax, 3
    je      .Lsw_r_rbp
    cmp     eax, 2
    jne     .Lsw_r4
    movsxd  rax, dword ptr [rbp + 12]
    mov     rcx, r15
    sub     rcx, rax
    mov     rax, rcx
    jmp     .Lsw_base_done
.Lsw_r_rbp:
    mov     rax, r15
    jmp     .Lsw_base_done
.Lsw_r4:
    cmp     eax, 4
    jne     .Lsw_r_none
    movsxd  rax, dword ptr [rbp + 12]
    add     rax, qword ptr [r14]
    jmp     .Lsw_base_done
.Lsw_r_none:
    xor     eax, eax
    jmp     .Lsw_base_done
.Lsw_chk0:
    mov     qword ptr [r12 + X_CHI], rax
    mov     rdi, qword ptr [rax]
    mov     qword ptr [r12 + X_W1LO], rdi
    lea     rsi, [r12 + X_W1HI]
    SW_SITE_FIND rdi, rsi
    test    rax, rax
    je      .Lsw_chk_none
    mov     rcx, qword ptr [r12 + X_W1HI]
    test    rcx, rcx
    je      .Lsw_chk_none
    movzx   edx, word ptr [rcx + 8]
    and     edx, 255
    cmp     edx, 6
    je      .Lsw_chk_fn
    cmp     edx, 2
    jne     .Lsw_chk_none
    mov     rbx, qword ptr [r12 + X_W1LO]
    mov     r14, qword ptr [r12 + X_CHI]
    jmp     .Lsw_chk_rbm
.Lsw_chk_fn:
    mov     edx, dword ptr [rcx + 12]
    mov     eax, edx
    and     eax, 0xffff
    shr     edx, 16
    and     edx, 0xffff
    mov     rsi, qword ptr [r12 + X_CHI]
    mov     rax, qword ptr [rsi + rax]
    mov     rbx, qword ptr [rax + 8]
    movzx   edi, word ptr [rcx + 10]
    and     edi, 255
    cmp     edi, 8
    jne     .Lsw_chk_fn_r
    mov     r14, rax
    jmp     .Lsw_chk_rbm
.Lsw_chk_fn_r:
    lea     r14, [rsi + rdx + 16]
.Lsw_chk_rbm:
    mov     rax, qword ptr [r12 + X_CLO]
    test    rax, rax
    je      .Lsw_again
    mov     r15, rax
    jmp     .Lsw_again
.Lsw_chk_none:
    mov     rax, qword ptr [r12 + X_CHI]
.Lsw_base_done:
    movzx   ecx, word ptr [rbp + 10]
    and     ecx, 255
    cmp     ecx, 5
    je      .Lsw_stop3
    mov     rdx, qword ptr [r12 + X_LAST]
    test    rdx, rdx
    je      .Lsw_f_ok
    cmp     rax, rdx
    jbe     .Lsw_stop6
.Lsw_f_ok:
    mov     qword ptr [r12 + X_F], rax
    mov     rdx, qword ptr [r13 + 8]
    mov     qword ptr [r12 + X_F + 8], rdx
    mov     rdx, qword ptr [r13 + 56]
    mov     qword ptr [r12 + X_F + 16], rdx
    movzx   edx, word ptr [rbp + 10]
    mov     ecx, edx
    and     ecx, 255
    mov     dword ptr [r12 + X_F + 24], ecx
    shr     edx, 8
    mov     dword ptr [r12 + X_F + 28], edx
    mov     qword ptr [r12 + SW_GAPN], 0
    mov     rax, qword ptr [r12 + SW_P]
    mov     qword ptr [r12 + X_GLO], rax
    mov     qword ptr [r12 + X_SUB], 0
    mov     qword ptr [r12 + X_PHASE], 1
.Lsw_p1:
    mov     rax, qword ptr [r12 + X_F]
    mov     qword ptr [r12 + LA_FBASE], rax
    mov     rax, qword ptr [r12 + X_F + 8]
    mov     qword ptr [r12 + LA_FMAP], rax
    mov     rax, qword ptr [r12 + X_F + 16]
    mov     qword ptr [r12 + LA_FLINK], rax
    movsxd  rax, dword ptr [r12 + X_F + 24]
    mov     qword ptr [r12 + LA_FRULE], rax
    cmp     qword ptr [r12 + X_SUB], 0
    jne     .Lsw_p1_call
    mov     qword ptr [r12 + LA_POSJ], 0
    mov     qword ptr [r12 + LA_POSK], 0
    mov     qword ptr [r12 + X_SUB], 1
.Lsw_p1_call:
    SW_LINKS_CALL
    cmp     rax, 1
    je      .Lsw_p1_ok
    cmp     rax, 4
    je      .Lsw_gapgrow
    jmp     .Lsw_decline
.Lsw_p1_ok:
    mov     qword ptr [r12 + X_I], 0
    mov     qword ptr [r12 + X_SUB], 0
    mov     qword ptr [r12 + X_PHASE], 2
.Lsw_p2:
    mov     rax, qword ptr [r12 + X_I]
    cmp     rax, qword ptr [r12 + SW_GAPN]
    jge     .Lsw_p3
    shl     rax, 5
    add     rax, qword ptr [r12 + SW_GAPV]
    mov     rdx, qword ptr [rax]
    mov     qword ptr [r12 + LA_FBASE], rdx
    mov     rdx, qword ptr [rax + 8]
    mov     qword ptr [r12 + LA_FMAP], rdx
    mov     rdx, qword ptr [rax + 16]
    mov     qword ptr [r12 + LA_FLINK], rdx
    movsxd  rdx, dword ptr [rax + 24]
    mov     qword ptr [r12 + LA_FRULE], rdx
    cmp     qword ptr [r12 + X_SUB], 0
    jne     .Lsw_p2_call
    mov     qword ptr [r12 + LA_POSJ], 0
    mov     qword ptr [r12 + LA_POSK], 0
    mov     qword ptr [r12 + X_SUB], 1
.Lsw_p2_call:
    SW_LINKS_CALL
    cmp     rax, 1
    je      .Lsw_p2_ok
    cmp     rax, 4
    je      .Lsw_gapgrow
    jmp     .Lsw_decline
.Lsw_p2_ok:
    inc     qword ptr [r12 + X_I]
    mov     qword ptr [r12 + X_SUB], 0
    jmp     .Lsw_p2
.Lsw_p3:
    mov     rcx, qword ptr [r12 + SW_GAPN]
    cmp     rcx, 1
    jle     .Lsw_p3_done
    mov     rsi, qword ptr [r12 + SW_GAPV]
    cmp     rcx, 16
    jg      .Lsw_heap
    mov     edx, 1
.Lsw_sort_i:
    cmp     rdx, rcx
    jge     .Lsw_p3_done
    mov     rax, rdx
.Lsw_sort_j:
    test    rax, rax
    je      .Lsw_sort_next
    mov     rdi, rax
    shl     rdi, 5
    add     rdi, rsi
    mov     r8, qword ptr [rdi]
    cmp     r8, qword ptr [rdi - 32]
    jae     .Lsw_sort_next
    mov     r10, qword ptr [rdi - 32]
    mov     qword ptr [rdi], r10
    mov     qword ptr [rdi - 32], r8
    mov     r8, qword ptr [rdi + 8]
    mov     r10, qword ptr [rdi - 24]
    mov     qword ptr [rdi + 8], r10
    mov     qword ptr [rdi - 24], r8
    mov     r8, qword ptr [rdi + 16]
    mov     r10, qword ptr [rdi - 16]
    mov     qword ptr [rdi + 16], r10
    mov     qword ptr [rdi - 16], r8
    mov     r8, qword ptr [rdi + 24]
    mov     r10, qword ptr [rdi - 8]
    mov     qword ptr [rdi + 24], r10
    mov     qword ptr [rdi - 8], r8
    dec     rax
    jmp     .Lsw_sort_j
.Lsw_sort_next:
    inc     rdx
    jmp     .Lsw_sort_i
.Lsw_heap:
    mov     rdx, rcx
    shr     rdx, 1
    xor     r11d, r11d
.Lsw_hb_next:
    test    rdx, rdx
    je      .Lsw_hx_init
    dec     rdx
    mov     rax, rdx
    jmp     .Lsw_sift
.Lsw_hx_init:
    mov     r11d, 1
.Lsw_hx_next:
    dec     rcx
    je      .Lsw_p3_done
    mov     r13, rcx
    shl     r13, 5
    add     r13, rsi
    SW_SWAP32 rsi, r13
    xor     eax, eax
.Lsw_sift:
    lea     rdi, [rax + rax + 1]
    cmp     rdi, rcx
    jae     .Lsw_sift_done
    mov     r13, rdi
    shl     r13, 5
    add     r13, rsi
    lea     r8, [rdi + 1]
    cmp     r8, rcx
    jae     .Lsw_sift_cmp
    mov     r10, qword ptr [r13 + 32]
    cmp     qword ptr [r13], r10
    jae     .Lsw_sift_cmp
    inc     rdi
    add     r13, 32
.Lsw_sift_cmp:
    mov     rbx, rax
    shl     rbx, 5
    add     rbx, rsi
    mov     r10, qword ptr [r13]
    cmp     qword ptr [rbx], r10
    jae     .Lsw_sift_done
    SW_SWAP32 rbx, r13
    mov     rax, rdi
    jmp     .Lsw_sift
.Lsw_sift_done:
    test    r11, r11
    je      .Lsw_hb_next
    jmp     .Lsw_hx_next
.Lsw_p3_done:
    mov     rcx, qword ptr [r12 + SW_GAPN]
    cmp     rcx, 1
    jle     .Lsw_dd_done
    mov     rsi, qword ptr [r12 + SW_GAPV]
    mov     edx, 1
    mov     eax, 1
.Lsw_dd:
    cmp     rdx, rcx
    jge     .Lsw_dd_set
    mov     rdi, rdx
    shl     rdi, 5
    add     rdi, rsi
    mov     r10, rax
    shl     r10, 5
    add     r10, rsi
    mov     r8, qword ptr [rdi]
    cmp     r8, qword ptr [r10 - 32]
    je      .Lsw_dd_next
    cmp     rdi, r10
    je      .Lsw_dd_keep
    mov     qword ptr [r10], r8
    mov     r11, qword ptr [rdi + 8]
    mov     qword ptr [r10 + 8], r11
    mov     r11, qword ptr [rdi + 16]
    mov     qword ptr [r10 + 16], r11
    mov     r11, qword ptr [rdi + 24]
    mov     qword ptr [r10 + 24], r11
.Lsw_dd_keep:
    inc     rax
.Lsw_dd_next:
    inc     rdx
    jmp     .Lsw_dd
.Lsw_dd_set:
    mov     qword ptr [r12 + SW_GAPN], rax
.Lsw_dd_done:
    mov     qword ptr [r12 + X_I], 0
    mov     qword ptr [r12 + X_SUB], 0
    mov     qword ptr [r12 + X_PHASE], 4
.Lsw_p4:
    mov     rax, qword ptr [r12 + X_I]
    cmp     rax, qword ptr [r12 + SW_GAPN]
    jge     .Lsw_p4_done
    cmp     qword ptr [r12 + X_SUB], 0
    jne     .Lsw_visit
    shl     rax, 5
    add     rax, qword ptr [r12 + SW_GAPV]
    mov     rsi, rax
    SW_COPY_CHF r12 + X_CUR, rsi
    jmp     .Lsw_visit
.Lsw_p4_done:
    mov     qword ptr [r12 + X_SUB], 0
    mov     qword ptr [r12 + X_PHASE], 5
.Lsw_p5:
    cmp     qword ptr [r12 + X_SUB], 0
    jne     .Lsw_visit
    lea     rsi, [r12 + X_F]
    SW_COPY_CHF r12 + X_CUR, rsi
.Lsw_visit:
    mov     rbx, qword ptr [r12 + X_CUR]
    mov     r14, qword ptr [r12 + X_CUR + 8]
    mov     r15, qword ptr [r12 + SW_HI]
    mov     rax, qword ptr [r12 + X_SUB]
    cmp     rax, 2
    je      .Lsw_v2
    cmp     rax, 3
    je      .Lsw_v3
    cmp     rax, 4
    je      .Lsw_v4
    lea     rsi, [r12 + X_CUR]
    SW_COPY_CHF r12 + SW_LASTF, rsi
#if RT_DIAG
    mov     rax, qword ptr [r12 + X_GW]
    inc     qword ptr [rax]
    cmp     qword ptr [r12 + SW_N], 0
    jne     .Lsw_v0_tg
    mov     rax, qword ptr [r12 + X_TOPG]
    cmp     qword ptr [rax], 0
    jne     .Lsw_v0_tg
    mov     rcx, qword ptr [r14 + 16]
    test    rcx, rcx
    jne     .Lsw_v0_tgs
    lea     rcx, [rip + .Lsw_qmark]
.Lsw_v0_tgs:
    mov     qword ptr [rax], rcx
.Lsw_v0_tg:
#endif
    test    dword ptr [r14 + 12], 4
    je      .Lsw_v0_plain
    mov     eax, dword ptr [r14 + 4]
    mov     rcx, rbx
    sub     rcx, rax
    mov     eax, dword ptr [r14 + 8]
    lea     rdx, [rbx + rax]
    cmp     rdx, r15
    jbe     .Lsw_v0_bt
    mov     rdx, r15
.Lsw_v0_bt:
    mov     rax, qword ptr [r12 + SW_P]
    mov     qword ptr [r12 + X_W1LO], rax
    mov     qword ptr [r12 + X_W1HI], rcx
    mov     rax, qword ptr [r12 + SW_LO]
    mov     qword ptr [r12 + X_CLO], rax
    mov     qword ptr [r12 + X_CHI], r15
    mov     qword ptr [r12 + X_CELLS], 1
    mov     qword ptr [r12 + X_W2LO], 0
    mov     qword ptr [r12 + X_W2HI], 0
    mov     qword ptr [r12 + X_PNEW], rdx
    jmp     .Lsw_v1
.Lsw_v0_plain:
    mov     rax, qword ptr [r12 + SW_P]
    mov     qword ptr [r12 + X_W1LO], rax
    mov     rcx, rbx
    cmp     rcx, rax
    jae     .Lsw_v0_slo
    mov     rcx, rax
.Lsw_v0_slo:
    mov     qword ptr [r12 + X_W1HI], rcx
    mov     rax, qword ptr [r14 + 24]
    lea     rcx, [rbx + rax + 16]
    mov     qword ptr [r12 + X_W2LO], rcx
    mov     eax, dword ptr [r14 + 8]
    add     rcx, rax
    cmp     rcx, r15
    jbe     .Lsw_v0_hhi
    mov     rcx, r15
.Lsw_v0_hhi:
    mov     qword ptr [r12 + X_W2HI], rcx
    mov     qword ptr [r12 + X_PNEW], rcx
    test    dword ptr [r14 + 12], 8
    je      .Lsw_v0_notab
    mov     qword ptr [r12 + X_CELLS], 1
    mov     rax, qword ptr [r12 + SW_LO]
    mov     qword ptr [r12 + X_CLO], rax
    mov     rcx, r15
    movsxd  rax, dword ptr [r12 + X_CUR + 28]
    test    rax, rax
    jle     .Lsw_v0_chi
    shl     rax, 4
    add     rax, rbx
    cmp     rax, r15
    jae     .Lsw_v0_chi
    mov     rcx, rax
.Lsw_v0_chi:
    mov     qword ptr [r12 + X_CHI], rcx
    jmp     .Lsw_v1
.Lsw_v0_notab:
    mov     qword ptr [r12 + X_CELLS], 0
#if RT_DIAG
    mov     rax, qword ptr [r12 + X_GW]
    inc     qword ptr [rax + 24]
#endif
.Lsw_v1:
    mov     qword ptr [r12 + X_SUB], 2
    mov     rdi, qword ptr [r12 + X_W1LO]
    mov     rsi, qword ptr [r12 + X_W1HI]
    lea     rax, [rdi + 16]
    cmp     rax, rsi
    ja      .Lsw_v2
    RTX_CALL(rt_gc_walk_words_a)
    test    rax, rax
    jne     .Lsw_out
    SW_DRAIN_CLEAR
.Lsw_v2:
    mov     qword ptr [r12 + X_SUB], 3
    cmp     qword ptr [r12 + X_CELLS], 0
    je      .Lsw_v3
    mov     rdi, rbx
    mov     rsi, r14
    mov     rdx, qword ptr [r12 + X_CLO]
    mov     rcx, qword ptr [r12 + X_CHI]
    RTX_CALL(rt_gc_frame_cells_a)
    test    rax, rax
    jne     .Lsw_out
    SW_DRAIN_CLEAR
.Lsw_v3:
    mov     qword ptr [r12 + X_SUB], 4
    mov     rdi, qword ptr [r12 + X_W2LO]
    mov     rsi, qword ptr [r12 + X_W2HI]
    lea     rax, [rdi + 16]
    cmp     rax, rsi
    ja      .Lsw_v4
    RTX_CALL(rt_gc_walk_words_a)
    test    rax, rax
    jne     .Lsw_out
    SW_DRAIN_CLEAR
.Lsw_v4:
    mov     rax, qword ptr [r12 + X_PNEW]
    mov     qword ptr [r12 + SW_P], rax
#if RT_DIAG
    test    dword ptr [r14 + 12], 1
    je      .Lsw_v4_nr
    mov     rax, qword ptr [r12 + X_GW]
    inc     qword ptr [rax + 8]
.Lsw_v4_nr:
#endif
    inc     qword ptr [r12 + SW_N]
    mov     rax, qword ptr [r14 + 16]
    mov     qword ptr [r12 + SW_LASTNAME], rax
    mov     qword ptr [r12 + X_SUB], 0
    cmp     qword ptr [r12 + X_PHASE], 5
    je      .Lsw_post
    inc     qword ptr [r12 + X_I]
    jmp     .Lsw_p4
.Lsw_post:
    mov     rbx, qword ptr [r12 + X_F]
    mov     r14, qword ptr [r12 + X_F + 8]
    mov     qword ptr [r12 + SW_PREV], rbx
    mov     qword ptr [r12 + X_LAST], rbx
    test    dword ptr [r14 + 12], 1
    je      .Lsw_post_nr
    mov     rax, qword ptr [r14 + 24]
    lea     rax, [rbx + rax + 16]
    mov     ecx, dword ptr [r14 + 8]
    add     rax, rcx
    cmp     dword ptr [r12 + SW_RUN], 0
    je      .Lsw_post_rt
    cmp     rax, qword ptr [r12 + SW_HI]
    ja      .Lsw_stop11
.Lsw_post_rt:
    mov     qword ptr [r12 + SW_ROOTTOP], rax
.Lsw_post_nr:
    mov     rax, qword ptr [r12 + X_COBODY]
    test    rax, rax
    je      .Lsw_post_link
    cmp     rax, rbx
    je      .Lsw_stop8
.Lsw_post_link:
    mov     rcx, qword ptr [r12 + X_F + 16]
    bt      rcx, 63
    jnc     .Lsw_post_nolink
    mov     rax, rcx
    and     eax, 0x0FFFFFFF
    mov     rdx, qword ptr [rbx + rax]
    mov     qword ptr [r12 + X_PC], rdx
    mov     rdx, rcx
    shr     rdx, 60
    and     edx, 7
    test    edx, 1
    jne     .Lsw_post_r
    mov     rsi, rcx
    shr     rsi, 28
    movsx   rsi, si
    add     rsi, rax
    mov     rsi, qword ptr [rbx + rsi]
    mov     qword ptr [r12 + X_RBP], rsi
.Lsw_post_r:
    mov     rsi, rcx
    shr     rsi, 44
    movsx   rsi, si
    add     rsi, rax
    add     rsi, rbx
    test    edx, 2
    je      .Lsw_post_rs
    mov     rsi, qword ptr [rsi]
.Lsw_post_rs:
    mov     qword ptr [r12 + X_R], rsi
    test    dword ptr [r14 + 12], 1
    je      .Lsw_cont
    mov     rdi, qword ptr [r12 + X_PC]
    xor     esi, esi
    SW_SITE_FIND rdi, rsi
    test    rax, rax
    je      .Lsw_stop0
.Lsw_cont:
    mov     qword ptr [r12 + X_PHASE], 0
    xor     eax, eax
    jmp     .Lsw_out
.Lsw_post_nolink:
    test    dword ptr [r14 + 12], 1
    je      .Lsw_stop5
.Lsw_stop0:
    mov     dword ptr [r12 + SW_STOP], 0
    jmp     .Lsw_stopped
.Lsw_stop1:
    mov     dword ptr [r12 + SW_STOP], 1
    jmp     .Lsw_stopped
.Lsw_stop3:
    mov     dword ptr [r12 + SW_STOP], 3
    jmp     .Lsw_stopped
.Lsw_stop4:
    mov     dword ptr [r12 + SW_STOP], 4
    jmp     .Lsw_stopped
.Lsw_stop5:
    mov     dword ptr [r12 + SW_STOP], 5
    jmp     .Lsw_stopped
.Lsw_stop6:
    mov     dword ptr [r12 + SW_STOP], 6
    jmp     .Lsw_stopped
.Lsw_stop7:
    mov     dword ptr [r12 + SW_STOP], 7
    jmp     .Lsw_stopped
.Lsw_stop8:
    mov     dword ptr [r12 + SW_STOP], 8
    jmp     .Lsw_stopped
.Lsw_stop11:
    mov     dword ptr [r12 + SW_STOP], 11
.Lsw_stopped:
    mov     rax, -1
    jmp     .Lsw_out
.Lsw_decline:
    mov     rax, -2
    jmp     .Lsw_out
.Lsw_gapgrow:
    mov     rax, -3
.Lsw_out:
    RTX_ADD_RSP(8)
    RTX_POPS(r15)
    RTX_POPS(r14)
    RTX_POPS(r13)
    RTX_POPS(r12)
    RTX_POPS(rbp)
    RTX_POPS(rbx)
    ret
RTX_ENDF(rt_gc_sw_step_a)
#if RT_DIAG
.section .rodata
.Lsw_qmark:
    .asciz "?"
.text
#endif
#define GRT(off) qword ptr [rip + g_runtime + off]
#define GRTD(off) dword ptr [rip + g_runtime + off]
#define GXMK(f) qword ptr [rip + g_runtime + G_RT_OFF_GC_MK + f]
#define GXHP(f) qword ptr [rip + g_runtime + G_RT_OFF_HP_FR + f]
#define GX_WLN 8
#define GX_SLOTS 24
#define GX_NSLOT 32
#define GX_ARENA 48
#define GX_TOP 56
#define GX_SSUM 64
#define GX_START 72
#define GX_LIVE 80
#define GX_VIS 88
#define GX_MHEAD 96
#define GX_RNGP 104
#define GX_RNGN 112
#define GX_INTERIOR 128
#define GX_DVEC_E 136
#define GX_DVEC_N 144
#define GX_NMK 152
#define GX_MXS 160
#define GX_WALKED 168
#define GX_DRAIN 184
#define GX_HS 272
#define GX_HCAP 280
#define GX_HN 288
#define HPF_END 8
#define HPF_BLOCKS 16
#define HPF_LINE 48
#define HPF_LIVE 72
#define HPF_VIS 88
#define HPF_START 96
#define HPF_SSUM 104
#define HPF_PREFIX 112
.macro GX_DISPATCH
    cmp     rax, 1
    jne     911f
    cmp     GXMK(112), 0
    je      912f
    RTX_CALL(gc_wl_grow)
    jmp     919f
912:
    RTX_CALL(gc_slot_grow)
    jmp     919f
911:
    cmp     rax, 2
    jne     913f
    RTX_CALL(gc_wl_grow)
    jmp     919f
913:
    cmp     rax, 6
    jne     914f
    RTX_CALL(gc_hins_grow)
    jmp     919f
914:
    test    al, 2
    je      915f
    and     rax, -3
    mov     rdi, rax
    RTX_CALL(gc_visit_mblk)
    jmp     919f
915:
    mov     rdi, rax
    RTX_CALL(gc_visit_one)
919:
.endm
RTX_FUNC(gc_wl_drain)
    RTX_SUB_RSP(8)
.Lwd_loop:
    lea     rdi, [rip + g_runtime + G_RT_OFF_GC_MK]
    RTX_CALL(rt_gc_mark_run)
    test    rax, rax
    je      .Lwd_done
    GX_DISPATCH
    jmp     .Lwd_loop
.Lwd_done:
    RTX_ADD_RSP(8)
    RTX_GVA_R9
    ret
RTX_ENDF(gc_wl_drain)
RTX_FUNC(gc_run_verdicts)
    RTX_SUB_RSP(8)
    mov     rax, rdi
.Lrv_loop:
    test    rax, rax
    je      .Lrv_done
    GX_DISPATCH
    lea     rdi, [rip + g_runtime + G_RT_OFF_GC_MK]
    RTX_CALL(rt_gc_mark_run)
    jmp     .Lrv_loop
.Lrv_done:
    mov     GXMK(GX_DRAIN), 0
    RTX_ADD_RSP(8)
    RTX_GVA_R9
    ret
RTX_ENDF(gc_run_verdicts)
RTX_FUNC(rt_gc_visit_descr)
    RTX_PUSHS(rbx)
    RTX_SUB_RSP(16)
    mov     rbx, rdi
    mov     rax, GXMK(GX_DRAIN)
    mov     qword ptr [rsp], rax
.Lvd_again:
    mov     rdi, rbx
    RTX_CALL(rt_gc_visit_descr_a)
    test    rax, rax
    jne     .Lvd_nz
    mov     rax, qword ptr [rsp]
    mov     GXMK(GX_DRAIN), rax
    jmp     .Lvd_out
.Lvd_nz:
    cmp     rax, 3
    je      .Lvd_out
    cmp     rax, 4
    jne     .Lvd_run
    RTX_CALL(gc_wl_grow)
    jmp     .Lvd_again
.Lvd_run:
    mov     rdi, rax
    RTX_CALL(gc_run_verdicts)
.Lvd_out:
    RTX_ADD_RSP(16)
    RTX_POPS(rbx)
    RTX_GVA_R9
    ret
RTX_ENDF(rt_gc_visit_descr)
RTX_FUNC(rt_gcheap_line_reset)
    mov     rax, GXHP(HPF_END)
    sub     rax, GXHP(0)
    mov     rdx, rax
    sar     rdx, 4
    mov     rcx, rax
    sar     rcx, 1
    cmp     rdi, rcx
    jle     1f
    mov     rdi, rcx
1:
    cmp     rdi, rdx
    jle     2f
    mov     rdx, rdi
2:
    sub     rax, rdx
    mov     rcx, GRT(G_RT_OFF_GC_GC_LINE_MBV)
    test    rcx, rcx
    je      3f
    shl     rcx, 20
    cmp     rcx, rax
    jge     3f
    mov     rax, rcx
3:
    add     rax, GXHP(0)
    mov     GRT(G_RT_OFF_HP_GCLINE), rax
    mov     rdx, GRT(G_RT_OFF_GC_HP_QLO)
    test    rdx, rdx
    je      4f
    cmp     rdx, rax
    jae     4f
    mov     rax, rdx
4:
    mov     GXHP(HPF_LINE), rax
    ret
RTX_ENDF(rt_gcheap_line_reset)
RTX_FUNC(gc_collect_ex)
    cmp     GRT(G_RT_OFF_GC_FAST), 0
    jne     .Lgx_fast
    RTX_SUB_RSP(8)
    RTX_CALL(gc_collect_ex_c)
    RTX_ADD_RSP(8)
    RTX_GVA_R9
    ret
.Lgx_fast:
    RTX_PUSHS(rbx)
    RTX_PUSHS(r12)
    RTX_PUSHS(r13)
    RTX_PUSHS(r14)
    RTX_PUSHS(r15)
    RTX_SUB_RSP(64)
    mov     rbx, rdi
    mov     qword ptr [rip + g_runtime + G_RT_OFF_SXT_FR], 0
    cmp     GRTD(G_RT_OFF_GC_IN), 0
    jne     .Lgx_zero
    mov     rax, GRT(G_RT_OFF_GC_HP_ARENA)
    test    rax, rax
    je      .Lgx_zero
    mov     GRTD(G_RT_OFF_GC_IN), 1
    mov     rcx, GXHP(0)
    mov     r12, rcx
    sub     r12, rax
    mov     GRT(G_RT_OFF_GC_HP_WIN), 0
    mov     GRT(G_RT_OFF_GC_HP_WEND), 0
    mov     GRT(G_RT_OFF_GC_GC_NBLK), 0
    mov     GXMK(GX_NMK), 0
    mov     GXMK(GX_MXS), 0
    mov     GXMK(GX_ARENA), rax
    mov     GXMK(GX_TOP), rcx
    mov     rdx, GXHP(HPF_SSUM)
    mov     GXMK(GX_SSUM), rdx
    mov     rdx, GXHP(HPF_START)
    mov     GXMK(GX_START), rdx
    mov     rdx, GXHP(HPF_LIVE)
    mov     GXMK(GX_LIVE), rdx
    mov     rdx, GXHP(HPF_VIS)
    mov     GXMK(GX_VIS), rdx
    mov     GXMK(GX_RNGP), 0
    mov     GXMK(GX_RNGN), 0
    mov     GXMK(GX_INTERIOR), 0
    mov     GXMK(GX_DVEC_E), 0
    mov     GXMK(GX_DVEC_N), 0
    mov     GXMK(GX_WALKED), 0
    mov     GRT(G_RT_OFF_GC_GC_IDX), 0
    lea     rdi, [rip + g_runtime + G_RT_OFF_HP_FR]
    mov     rsi, r12
    shr     rsi, 4
    add     rsi, 63
    shr     rsi, 6
    RTX_CALL(rt_gc_ssum_run)
    mov     GXMK(GX_MHEAD), 0
    mov     GXMK(GX_HN), 0
    mov     rdi, GXMK(GX_HS)
    test    rdi, rdi
    je      1f
    mov     rcx, GXMK(GX_HCAP)
    xor     eax, eax
    rep stosq
1:
    mov     GXMK(GX_NSLOT), 0
    mov     GRTD(G_RT_OFF_GC_GC_SPINE_RECN), 0
    xor     r13d, r13d
.Lgx_rb:
    cmp     r13, GRT(G_RT_OFF_GC_GC_RRNG_N)
    jge     .Lgx_rbd
    mov     rax, GRT(G_RT_OFF_GC_GC_RRNG)
    mov     rcx, r13
    shl     rcx, 4
    add     rax, rcx
    cmp     qword ptr [rax + 8], 0
    jne     .Lgx_rbn
    mov     rdi, qword ptr [rax]
    RTX_CALL(pl_tr_gc_root_ball)
.Lgx_rbn:
    inc     r13
    jmp     .Lgx_rb
.Lgx_rbd:
    mov     GXMK(GX_DRAIN), 1
    mov     rax, GRT(G_RT_OFF_GC_GC_RRNG)
    mov     qword ptr [rsp], rax
    mov     rax, GRT(G_RT_OFF_GC_GC_RRNG_N)
    mov     qword ptr [rsp + 8], rax
    mov     qword ptr [rsp + 16], 0
    mov     qword ptr [rsp + 24], 0
    mov     qword ptr [rsp + 32], 0
    mov     qword ptr [rsp + 40], 0
.Lgx_tr:
    mov     rdi, rsp
    RTX_CALL(rt_gc_trail_roots_run)
    test    rax, rax
    je      .Lgx_trd
    cmp     rax, 1
    jne     .Lgx_trw
    RTX_CALL(gc_slot_grow)
    jmp     .Lgx_tr
.Lgx_trw:
    RTX_CALL(gc_wl_grow)
    jmp     .Lgx_tr
.Lgx_trd:
    mov     r13, qword ptr [rsp + 32]
    mov     GXMK(GX_DRAIN), 0
    RTX_CALL(gc_wl_drain)
    xor     edi, edi
    xor     esi, esi
    RTX_CALL(scrip_co_gc_visit_records)
    mov     rdi, GRT(G_RT_OFF_GC_GC_SEAM_SP)
    test    rdi, rdi
    jne     1f
    mov     rdi, rsp
1:
    mov     rsi, rbx
    RTX_CALL(gc_stack_segments)
    mov     GXMK(GX_DRAIN), 0
    RTX_CALL(gc_wl_drain)
    RTX_CALL(rt_cas_gc_roots)
    mov     eax, 0x70001000
    mov     GXMK(GX_RNGP), rax
    movsxd  rax, dword ptr [rip + g_runtime + G_RT_OFF_SXT_FR + 16]
    mov     GXMK(GX_RNGN), rax
    mov     GXMK(GX_DRAIN), 1
    RTX_CALL(gc_wl_drain)
    mov     GXMK(GX_DRAIN), 0
    RTX_CALL(kw_cset_gc_roots)
    RTX_CALL(core_gc_roots)
    RTX_CALL(dat_gc_roots)
    RTX_CALL(pas_gc_roots)
    RTX_CALL(pl_gc_roots)
    RTX_CALL(rt_gc_root_args)
    RTX_CALL(eval_gc_roots)
    RTX_CALL(lower_gc_roots)
    RTX_CALL(bnd_gc_roots)
    RTX_CALL(drv_gc_roots)
    mov     rax, GRT(G_RT_OFF_GC_GC_SHIELD_R)
    test    rax, rax
    je      .Lgx_sh0
    mov     rdx, qword ptr [rax]
    test    rdx, rdx
    je      .Lgx_sh0
    mov     rcx, qword ptr [rip + Σ@GOTPCREL]
    cmp     rdx, qword ptr [rcx]
    je      .Lgx_shv
    cmp     GRTD(G_RT_OFF_GC_GC_SHIELD_R_IS_PROBE), 0
    jne     .Lgx_sh0
    mov     rdi, rdx
    RTX_CALL(gc_shield_refuse)
.Lgx_shv:
    mov     rdi, rax
    RTX_CALL(rt_gc_visit_raw)
.Lgx_sh0:
    mov     rdi, qword ptr [rip + Σ@GOTPCREL]
    RTX_CALL(rt_gc_visit_raw)
    xor     r14d, r14d
.Lgx_shl:
    movsxd  rax, dword ptr [rip + g_runtime + G_RT_OFF_GC_GC_SHIELD_N]
    cmp     r14, rax
    jge     .Lgx_shd
    mov     rdi, r14
    shl     rdi, 4
    add     rdi, GRT(G_RT_OFF_GC_GC_SHIELD_ARR)
    RTX_CALL(rt_gc_visit_descr)
    inc     r14
    jmp     .Lgx_shl
.Lgx_shd:
.Lgx_rd:
    cmp     GXMK(GX_MHEAD), 0
    jne     .Lgx_rdg
    cmp     GXMK(GX_WLN), 0
    je      .Lgx_rdd
.Lgx_rdg:
    mov     GXMK(GX_DRAIN), 1
    RTX_CALL(gc_wl_drain)
    mov     GXMK(GX_DRAIN), 0
    jmp     .Lgx_rd
.Lgx_rdd:
    RTX_CALL(kw_cset_gc_weak)
    RTX_CALL(rt_pl_eng_gc_weak)
    RTX_CALL(gc_cells_resolve)
    mov     rax, GXHP(0)
    sub     rax, GRT(G_RT_OFF_GC_HP_ARENA)
    shr     rax, 4
    mov     r15, rax
    mov     rdi, GXHP(HPF_LIVE)
    mov     rsi, r15
    RTX_CALL(rt_gc_first_dead_run)
    mov     r14, rax
    mov     rdi, GXHP(HPF_PREFIX)
    mov     rsi, GXHP(HPF_LIVE)
    lea     rdx, [r15 + 63]
    shr     rdx, 6
    RTX_CALL(rt_gc_prefix_run)
    mov     rdi, GXMK(GX_SLOTS)
    mov     rsi, GXMK(GX_NSLOT)
    mov     rdx, GRT(G_RT_OFF_GC_HP_ARENA)
    mov     rcx, r14
    lea     r8, [rip + g_runtime + G_RT_OFF_HP_FR]
    mov     r9, rdx
    RTX_CALL(rt_gc_fix_run)
    mov     r15, GXHP(0)
    mov     rdi, GRT(G_RT_OFF_GC_HP_ARENA)
    mov     rsi, r14
    lea     rdx, [rip + g_runtime + G_RT_OFF_HP_FR]
    mov     rcx, rdi
    mov     r8, r15
    RTX_CALL(rt_gc_slide_run)
    mov     r14, rax
    lea     rdi, [rip + g_runtime + G_RT_OFF_HP_FR]
    mov     rsi, GRT(G_RT_OFF_GC_HP_ARENA)
    mov     rdx, rsi
    mov     rcx, r15
    RTX_CALL(rt_gc_start_compact_run)
    mov     rsi, r14
    cmp     rsi, r15
    cmovbe  rsi, r15
    sub     rsi, GRT(G_RT_OFF_GC_HP_ARENA)
    shr     rsi, 4
    add     rsi, 63
    shr     rsi, 6
    lea     rdi, [rip + g_runtime + G_RT_OFF_HP_FR]
    RTX_CALL(rt_gc_ssum_run)
    mov     r15d, dword ptr [rip + g_runtime + G_RT_OFF_GC_MK + GX_MXS]
    mov     GXHP(0), r14
    mov     rax, GXMK(GX_NMK)
    mov     GXHP(HPF_BLOCKS), rax
    mov     rax, r14
    sub     rax, GRT(G_RT_OFF_GC_HP_ARENA)
    mov     GRT(G_RT_OFF_GC_HP_LIVE), rax
    mov     rdi, r15
    RTX_CALL(rt_gcheap_line_reset)
    test    r13, r13
    je      .Lgx_notr
    mov     rax, GRT(G_RT_OFF_GC_GC_RRNG)
    mov     qword ptr [rsp], rax
    mov     rax, GRT(G_RT_OFF_GC_GC_RRNG_N)
    mov     qword ptr [rsp + 8], rax
    mov     rax, GRT(G_RT_OFF_GC_HP_ARENA)
    mov     qword ptr [rsp + 16], rax
    add     rax, r12
    mov     qword ptr [rsp + 24], rax
    mov     rax, GXHP(0)
    mov     qword ptr [rsp + 32], rax
    lea     rax, [rip + g_runtime + G_RT_OFF_HP_FR]
    mov     qword ptr [rsp + 40], rax
    mov     rax, GRT(G_RT_OFF_GC_HP_ARENA)
    mov     qword ptr [rsp + 48], rax
    mov     rax, GRT(G_RT_OFF_GC_HP_CAP_END)
    mov     qword ptr [rsp + 56], rax
    mov     rdi, rsp
    RTX_CALL(rt_gc_trail_run)
    test    rax, rax
    je      .Lgx_notr
    mov     rdi, rax
    RTX_CALL(gc_trail_refuse)
.Lgx_notr:
    lea     rdi, [rip + g_runtime + G_RT_OFF_HP_FR]
    mov     rsi, r12
    shr     rsi, 4
    add     rsi, 63
    shr     rsi, 6
    RTX_CALL(rt_gc_clear_run)
    mov     r13, GXHP(0)
    sub     r13, GRT(G_RT_OFF_GC_HP_ARENA)
    RTX_CALL(rt_nv_memo_invalidate)
    RTX_CALL(pm_lf_type_invalidate)
    inc     GRT(G_RT_OFF_GC_GC_RUNS)
    mov     rax, GRT(G_RT_OFF_GC_HP_CAP_END)
    cmp     rax, GXHP(HPF_END)
    jbe     .Lgx_ng
    mov     rcx, GXHP(HPF_END)
    sub     rcx, GRT(G_RT_OFF_GC_HP_ARENA)
    mov     rdx, rcx
    sub     rdx, GRT(G_RT_OFF_GC_HP_LIVE)
    imul    rdx, rdx, 100
    imul    rcx, rcx, 15
    cmp     rdx, rcx
    jge     .Lgx_ng
    xor     edi, edi
    RTX_CALL(rt_gcheap_grow)
.Lgx_ng:
    mov     rdi, r15
    RTX_CALL(rt_gcheap_line_reset)
    mov     GRT(G_RT_OFF_GC_GC_IDX), 0
    mov     GRTD(G_RT_OFF_GC_IN), 0
    mov     rax, r12
    sub     rax, r13
    jmp     .Lgx_out
.Lgx_zero:
    xor     eax, eax
.Lgx_out:
    RTX_ADD_RSP(64)
    RTX_POPS(r15)
    RTX_POPS(r14)
    RTX_POPS(r13)
    RTX_POPS(r12)
    RTX_POPS(rbx)
    RTX_GVA_R9
    ret
RTX_ENDF(gc_collect_ex)
.macro GX_GROW_LOOP run, slotonly
    RTX_PUSHS(rbx)
    mov     rbx, rdi
1:
    mov     rdi, rbx
    RTX_CALL(\run)
    test    rax, rax
    je      3f
.if \slotonly
    RTX_CALL(gc_slot_grow)
    jmp     1b
.else
    cmp     rax, 1
    jne     2f
    RTX_CALL(gc_slot_grow)
    jmp     1b
2:
    RTX_CALL(gc_wl_grow)
    jmp     1b
.endif
3:
    RTX_POPS(rbx)
    RTX_GVA_R9
    ret
.endm
RTX_FUNC(rt_gc_run_roots)
    GX_GROW_LOOP rt_gc_core_roots_run, 0
RTX_ENDF(rt_gc_run_roots)
RTX_FUNC(rt_gc_run_flags_roots)
    GX_GROW_LOOP rt_gc_flags_roots_run, 0
RTX_ENDF(rt_gc_run_flags_roots)
RTX_FUNC(rt_gc_run_proc_roots)
    GX_GROW_LOOP rt_gc_proc_roots_run, 1
RTX_ENDF(rt_gc_run_proc_roots)
RTX_FUNC(rt_gc_run_ptr_fields)
    lea     rax, [rcx - 1]
    cmp     rax, 4
    ja      .Lpf_bad
    RTX_SUB_RSP(56)
    mov     qword ptr [rsp], rdi
    mov     qword ptr [rsp + 8], rsi
    mov     qword ptr [rsp + 16], rdx
    mov     qword ptr [rsp + 24], 0
    mov     qword ptr [rsp + 32], rcx
    mov     qword ptr [rsp + 40], r8
    mov     qword ptr [rsp + 48], 0
.Lpf_again:
    mov     rdi, rsp
    RTX_CALL(rt_gc_ptr_fields_run)
    test    rax, rax
    je      .Lpf_done
    RTX_CALL(gc_slot_grow)
    jmp     .Lpf_again
.Lpf_done:
    RTX_ADD_RSP(56)
    RTX_GVA_R9
    ret
.Lpf_bad:
    RTX_SUB_RSP(8)
    RTX_CALL(rt_gc_ptr_fields_bad)
    RTX_ADD_RSP(8)
    RTX_GVA_R9
    ret
RTX_ENDF(rt_gc_run_ptr_fields)
RTX_FUNC(rt_gc_run_loc_list)
    RTX_SUB_RSP(24)
    mov     qword ptr [rsp], rdi
    mov     qword ptr [rsp + 8], rsi
    mov     qword ptr [rsp + 16], 0
.Lrll_again:
    mov     rdi, rsp
    RTX_CALL(rt_gc_loc_list_run)
    test    rax, rax
    je      .Lrll_done
    RTX_CALL(gc_slot_grow)
    jmp     .Lrll_again
.Lrll_done:
    RTX_ADD_RSP(24)
    RTX_GVA_R9
    ret
RTX_ENDF(rt_gc_run_loc_list)
