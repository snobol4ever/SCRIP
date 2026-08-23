/* rtx_table.S — RTX-TBL: the hashed TABLE lookup, in assembly.
 *
 * THE CONTRACT IS .github/ARCH-SNOBOL4-RTX.md. READ IT BEFORE EDITING ANY .S FILE.
 * C of record: src/runtime/aggregates.c c_table_find_pair_d, and the eight
 * per-datatype hash algorithms above it. This file is that C, arm for arm.
 *
 * ⭐⭐ WHY THIS FILE EXISTS. Lon, s262: the runtime is moving to register-aware
 * ASM and C is kept only for a few large algorithms, which is also why RT_OPT is
 * -O0 forever. Table lookup is the hottest non-string path in SNOBOL4 -- at fixed
 * work (200 rebuilds x 500 integer keys) the C spends 38.7M of 180.2M Ir inside
 * table_find_pair_d -- and at -O0 most of that is stack traffic gcc emits for
 * locals that live perfectly well in registers. Nothing here is clever; it is the
 * same algorithm with the spills deleted.
 *
 * ⭐ ONE HASH ALGORITHM PER DATATYPE, and each is a separate arm below, not a
 * shared mixer fed by a per-type extractor. Each ends in its own multiply with
 * its own constant and its own >> 8; see aggregates.c for why each constant is
 * what it is. The arms are ordered by tag value so a later port can replace the
 * compare chain with a jump on (v >> 3) without reordering anything.
 *
 * ⛔ THE GUARD IS NARROWER THAN THE C, DELIBERATELY. DT_A, DT_T and DT_DATA all
 * LAZILY ASSIGN a serial id on first use (`if (!k.arr->id) k.arr->id =
 * g_agg_list_ser++`), and those counters are file-static in aggregates.c with no
 * exported cell. Reaching them from here would mean minting a global, which
 * NO-NEW-GLOBALS forbids without an in-chat grant -- so those three tags tail-jump
 * to the C, which already has the identical signature and the args already in
 * place. Every other tag, including every user datatype above DT_DATA, is handled
 * here. This is the standard RTX narrow-guard shape, not a gap.
 *
 * KILLSWITCH: SCRIP_RTX_TABLE=0 (gate byte, tail-jumps to the C body). The typed-
 * hash killswitch SCRIP_TBL_TYPED=0 also clears this gate in rtx_init.c, because
 * that mode hashes the STRINGIFIED key and this file does not implement it.
 */
#include "rtx_abi.inc"

/* ---- TAGS. src/contracts/descr.h:11-29 (DTYPE_t). ⛔ Cited, not remembered:
 * s222 proved an undefined tag compare assembles cleanly as a relocation and
 * admits the wrong datatype silently, so every value here names its source. */
#define DT_SNUL           0x00    /* descr.h:12 */
#define DT_S              0x02    /* descr.h:13 */
#define DT_I              0x03    /* descr.h:14 */
#define DT_R              0x05    /* descr.h:15 */
#define DT_A              0x10    /* descr.h:18 -- serial id, bails to C */
#define DT_T              0x18    /* descr.h:19 -- serial id, bails to C */
#define DT_DATA           0x70    /* descr.h:29 -- instance id, bails to C */

/* ---- LAYOUT. All pinned by _Static_assert in rtx_init.c; a drift breaks the
 * BUILD, not the runtime. TBPAIR_t is {char *key; DESCR_t key_descr; DESCR_t val;
 * unsigned long long hkey;} = 48 bytes, and TBBLK_t opens with buckets[], so the
 * bucket vector is at offset ZERO. ⛔ A BUCKET IS 16 BYTES, NOT 8: it was a bare
 * chain pointer until s262 and is now {TBPAIR_t *ent; unsigned len, cap}. */
#define TBPAIR_KEYD_V     8       /* key_descr.v    (uint8)         */
#define TBPAIR_KEYD_VAL  16       /* key_descr value union          */
#define TBPAIR_HKEY      40       /* hkey           (u64, sort key) */
#define TBPAIR_SIZE      48
/* ⛔ A BUCKET IS A POINTER, AND THE COUNTS LIVE IN THE BLOCK IT NAMES. An
 * intermediate s262 cut made the bucket a 16-byte {ent,len,cap} struct inline in
 * the vector; that doubled TBBLK_t to 4,160 bytes, which every TABLE() memsets,
 * and CLAWS5 -- thousands of tiny tables -- lost 7.4% in CYCLES while its
 * INSTRUCTION count fell. len/cap moved into the entry block: the vector is bare
 * pointers again, an empty bucket is NULL and allocates nothing, and len now
 * shares a cache line with ent[0] instead of sitting in a separate 4 KB vector. */
#define TBBUCK_LEN        0       /* TBBUCK_t.len   (unsigned)      */
#define TBBUCK_CAP        4       /* TBBUCK_t.cap   (unsigned)      */
#define TBBUCK_ENT        8       /* TBBUCK_t.ent[] (TBPAIR_t [])   */
#define TBL_BUCKET_MASK 255       /* TABLE_BUCKETS-1, core.h; the assert pins the power of two */
#define TBL_LINEAR_MAX   12       /* aggregates.c _tbl_lower crossover */

RTX_GATE_DEF(table)

/*-----------------------------------------------------------------------------
 * TBPAIR_t *table_find_pair_d(TBBLK_t *tbl, DESCR_t k)
 *   rdi = tbl . rsi = k tag word (v in sil) . rdx = k value . -> rax = entry or 0
 *
 * REGISTER PLAN (no frame, no spills, no call -- this routine never touches the
 * stack, which is the entire point of porting it):
 *   r11 hash then hkey . r8 ent . r9 end . r10 p (cursor) . rdi key value, after
 *   the bucket address is computed and tbl is dead . rsi tag word, live to the end
 *   because the equality arm compares against sil . rax/rcx/rdx scratch.
 * ⛔ rbx/r13/r14/r15 are BLOB PINS and are never written here.
 */
RTX_FUNC(table_find_pair_d)
    RTX_GATE(table, c_table_find_pair_d)
    test    rdi, rdi
    je      .Ltf_null
    movzx   eax, sil                        /* v -- 8-bit, never a 32-bit read of  */
                                            /*   the tag word: bits 8-31 carry the */
                                            /*   DESCR provenance stamp            */
    cmp     al, DT_I
    je      .Ltf_h_int
    cmp     al, DT_S
    je      .Ltf_h_str
    cmp     al, DT_SNUL
    je      .Ltf_h_snul
    cmp     al, DT_R
    je      .Ltf_h_real
    cmp     al, DT_A
    je      c_table_find_pair_d             /* lazily assigns a file-static serial  */
    cmp     al, DT_T
    je      c_table_find_pair_d             /*   id; see the ⛔ block in the header  */
    cmp     al, DT_DATA
    je      c_table_find_pair_d

/* --- EVERY REMAINING TAG: identity IS the address, and an address is not an int.
 * Allocator output is 16-byte aligned, so the low FOUR bits are always zero;
 * shift them off before the multiply or a quarter of the bucket space is dead. */
.Ltf_h_ptr:
    mov     r11, rdx
    shr     r11, 4
    mov     rcx, 0xC2B2AE3D27D4EB4F
    imul    r11, rcx
    shr     r11, 8
    jmp     .Ltf_key

/* --- DT_I: Fibonacci multiplicative, ONE imul, no avalanche chain. Integer keys
 * arrive CONSECUTIVE (loop counters, ids, indices) and for consecutive inputs the
 * golden-ratio multiplier is optimal -- successive products are maximally spread.
 * ⛔ Do not "improve" this into a full mixer; that is two more multiplies to buy
 * nothing on the distribution that actually arrives. */
.Ltf_h_int:
    mov     r11, rdx
    mov     rcx, 0x9E3779B97F4A7C15
    imul    r11, rcx
    shr     r11, 8
    jmp     .Ltf_key

/* --- DT_SNUL: the null string is a SINGLETON. One possible key, so there is no
 * distribution to spread -- a constant is the whole algorithm. Distinguished (not
 * zero) so it does not share bucket 0 with every null-valued pointer key. */
.Ltf_h_snul:
    mov     r11, 0x2F1B3D5C7E9A11
    jmp     .Ltf_key

/* --- DT_R: FOLD FIRST, because a double's entropy is all in the top half. Program
 * reals have long runs of zero low mantissa bits, so hashing the raw pattern feeds
 * the multiplier dead bits; high^low moves sign+exponent+high mantissa into the
 * bits the multiply then spreads. Bitwise throughout, so -0.0 and 0.0 stay
 * DIFFERENT keys and a NaN key stays findable -- matching _tbl_eq_d exactly. */
.Ltf_h_real:
    mov     r11, rdx
    mov     rcx, r11
    shr     rcx, 32
    xor     r11, rcx
    mov     rcx, 0xBF58476D1CE4E5B9
    imul    r11, rcx
    shr     r11, 8
    jmp     .Ltf_key

/* --- DT_S: streaming, because the key is N bytes and not a word. djb2 (h*33 ^ b):
 * shift-add-xor per byte, no multiply in the loop, which is what makes it the right
 * shape to inline here. ⛔ djb2's own low bits are weak for SHORT keys -- 'a','b','c'
 * differ only at the bottom -- and short keys are exactly what SNOBOL4 uses, so the
 * accumulator gets ONE finishing multiply to drive that difference up top. */
.Ltf_h_str:
    mov     r11, 5381
    test    rdx, rdx
    je      .Ltf_str_fin                    /* NULL reads as "" -- C substitutes it */
    mov     r8, rdx
.Ltf_str_loop:
    movzx   ecx, byte ptr [r8]
    test    cl, cl
    je      .Ltf_str_fin
    mov     r9, r11
    shl     r9, 5
    add     r9, r11                         /* h * 33 == (h << 5) + h              */
    xor     r9, rcx
    mov     r11, r9
    inc     r8
    jmp     .Ltf_str_loop
.Ltf_str_fin:
    mov     rcx, 0x9E3779B97F4A7C15
    imul    r11, rcx
    shr     r11, 8

/* --- hkey = (v << 56) | (h & 0x00FF_FFFF_FFFF_FFFF). The DATATYPE IS THE HIGH
 * BYTE, so the sort order is datatype-major and two keys of different type can
 * never compare equal whatever their values hash to. The bucket is read from the
 * LOW bits, which are value-hash bits, so types spread across all buckets. */
.Ltf_key:
    movzx   rax, sil
    shl     rax, 56
    mov     rcx, 0x00FFFFFFFFFFFFFF
    and     r11, rcx
    or      r11, rax                        /* r11 = hkey                          */
    mov     ecx, r11d
    and     ecx, TBL_BUCKET_MASK
    mov     rcx, [rdi + rcx*8]              /* b = tbl->buckets[i]; buckets at off 0 */
    test    rcx, rcx
    je      .Ltf_null                       /* NULL bucket == empty, no block at all  */
    lea     r8, [rcx + TBBUCK_ENT]          /* ent                                    */
    mov     ecx, [rcx + TBBUCK_LEN]         /* len -- same cache line as ent[0]       */
    test    ecx, ecx
    je      .Ltf_null
    mov     rdi, rdx                        /* tbl is dead; rdi carries the key value */
    mov     rax, rcx
    imul    rax, rax, TBPAIR_SIZE
    lea     r9, [r8 + rax]                  /* end = ent + len*48                   */
    cmp     ecx, TBL_LINEAR_MAX
    ja      .Ltf_bin

/* --- LOWER BOUND, LINEAR ARM. Below the crossover a forward walk over SORTED
 * hkeys beats halving: no imul per probe, and it keeps the early-out that an
 * unsorted chain could never have -- the first hkey past the target IS the miss. */
    mov     r10, r8
.Ltf_lin:
    cmp     r10, r9
    jae     .Ltf_null
    cmp     [r10 + TBPAIR_HKEY], r11
    jae     .Ltf_scan
    add     r10, TBPAIR_SIZE
    jmp     .Ltf_lin

/* --- LOWER BOUND, BINARY ARM. eax = lo, ecx = n, rdx free (the key value moved
 * to rdi above, which is exactly why it moved). */
.Ltf_bin:
    xor     eax, eax
.Ltf_bin_loop:
    test    ecx, ecx
    je      .Ltf_bin_done
    mov     edx, ecx
    shr     edx, 1                          /* half                                 */
    mov     r10d, eax
    add     r10d, edx                       /* mid = lo + half                      */
    imul    r10, r10, TBPAIR_SIZE
    add     r10, r8
    cmp     [r10 + TBPAIR_HKEY], r11
    jb      .Ltf_bin_lo
    mov     ecx, edx                        /* n = half                             */
    jmp     .Ltf_bin_loop
.Ltf_bin_lo:
    add     eax, edx
    add     eax, 1                          /* lo = mid + 1                         */
    sub     ecx, edx
    sub     ecx, 1                          /* n -= half + 1                        */
    jmp     .Ltf_bin_loop
.Ltf_bin_done:
    imul    rax, rax, TBPAIR_SIZE
    lea     r10, [r8 + rax]                 /* p = ent + lo*48                      */

/* --- THE EQUAL-HKEY RUN. Over a 56-bit value hash this run is essentially always
 * length 1, so a hit costs one dereference; the loop exists for the genuine
 * collision. ⛔ The FIRST hkey mismatch ends the search -- the bucket is sorted,
 * so there is nothing equal further on. r8 (ent) is dead from here and the string
 * arm reuses it. */
.Ltf_scan:
    cmp     r10, r9
    jae     .Ltf_null
    mov     rax, [r10 + TBPAIR_HKEY]
    cmp     rax, r11
    jne     .Ltf_null
    movzx   eax, byte ptr [r10 + TBPAIR_KEYD_V]
    cmp     al, sil                         /* datatype first, always               */
    jne     .Ltf_next
    cmp     al, DT_S
    je      .Ltf_eq_str
    cmp     al, DT_SNUL
    je      .Ltf_hit                        /* singleton: the tag IS the equality    */
    mov     rax, [r10 + TBPAIR_KEYD_VAL]    /* DT_I / DT_R / every pointer tag: one  */
    cmp     rax, rdi                        /*   64-bit bitwise compare covers all   */
    je      .Ltf_hit
.Ltf_next:
    add     r10, TBPAIR_SIZE
    jmp     .Ltf_scan

/* --- DT_S equality: strcmp, and the null guards are the C's, kept exactly
 * (`e->key_descr.s && k.s && strcmp(...) == 0`). */
.Ltf_eq_str:
    mov     rax, [r10 + TBPAIR_KEYD_VAL]
    test    rax, rax
    je      .Ltf_next
    test    rdi, rdi
    je      .Ltf_next
    mov     rcx, rdi
.Ltf_scmp:
    movzx   edx, byte ptr [rax]
    movzx   r8d, byte ptr [rcx]
    cmp     dl, r8b
    jne     .Ltf_next
    test    dl, dl
    je      .Ltf_hit                        /* both hit NUL together == strcmp 0     */
    inc     rax
    inc     rcx
    jmp     .Ltf_scmp

.Ltf_hit:
    mov     rax, r10
    ret
.Ltf_null:
    xor     eax, eax
    ret
RTX_ENDF(table_find_pair_d)
