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
#define TBPAIR_KEYD_SLEN 12       /* key_descr.slen (uint32)        */
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
/* ⛔ THE BUCKET COUNT IS NO LONGER A COMPILE-TIME CONSTANT.  It is sized per table
 * from the program's own TABLE(n) estimate (SPITBOL: Arg1 is the estimated entry
 * count, not a limit and not a bucket count), so the mask is LOADED from the
 * table, never baked here.  A literal 255 was correct only while every table had
 * 256 buckets, and it would silently select the wrong bucket for every other
 * width -- the same class of bug as the old `movzx eax,al` fold it replaced. */
#define TBBLK_BUCKETS     0       /* TBBLK_t.buckets (TBBUCK_t **)  */
#define TBBLK_NBUCK       8       /* TBBLK_t.nbuck   (unsigned, pow2) */
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

/* --- DT_S: WORD-AT-A-TIME, EIGHT BYTES PER MULTIPLY. This arm used to be djb2,
 * one byte per iteration, and it compiled to ELEVEN instructions per byte
 * (test/je/movzx/mov/shl/add/xor/mov/inc/dec/jmp) -- so a 4-character tag cost
 * ~50 Ir before a single bucket was touched. MEASURED on CLAWS5 (hq_P s264):
 * table_find_pair_d was 13.02% of the whole run, 92 Ir/call over 109,360 calls
 * per parse, and this loop was the majority of it.
 * ⛔ THE TAIL IS OVERLAPPING, AND THAT IS THE ENTIRE SAFETY ARGUMENT. A 1..7-byte
 * remainder is read as a 4/2/1-byte load at the FRONT plus a second load ENDING
 * exactly at p+n, so every byte touched lies inside the string. The tempting
 * alternative -- one 8-byte load then mask -- can step into an unmapped page when
 * a string ends at a page boundary. Do not "simplify" it into that.
 * ⛔ COUNTED, NEVER NUL-TERMINATED (Lon s263) IS UNCHANGED: every load is driven by
 * the length and nothing tests a byte for zero, so 'a' CHAR(0) 'b' and
 * 'a' CHAR(0) 'c' still hash apart. The LENGTH is mixed in first, so two keys
 * sharing a prefix cannot collide on length alone.
 * ⛔⛔ THIS ARM AND aggregates.c:_tbl_h_str ARE ONE ALGORITHM IN TWO SPELLINGS.
 * SCRIP_RTX_TABLE=0 routes lookups to the C twin, so a table INSERTED through one
 * and READ through the other must agree bit for bit or entries simply vanish.
 * Edit them together; A/B any table-heavy program both ways and diff. */
.Ltf_h_str:
    test    rdx, rdx
    je      .Ltf_str_n0                     /* s == NULL reads as "", n forced to 0  */
    mov     rcx, rsi
    shr     rcx, 32                         /* ecx = k.slen, the counted length      */
    test    ecx, ecx
    jne     .Ltf_str_have
    mov     r9, rdx                         /* slen unstamped: strlen fallback, so    */
.Ltf_str_scan:                              /*   ASM and C lose the SAME distinction  */
    cmp     byte ptr [r9], 0
    je      .Ltf_str_scand
    inc     r9
    jmp     .Ltf_str_scan
.Ltf_str_scand:
    mov     rcx, r9
    sub     rcx, rdx
.Ltf_str_have:
    mov     r8, rdx                         /* p -- rdx must survive, .Ltf_key reads it */
    mov     eax, ecx                        /* n, zero-extended                       */
    mov     r10, 0x9E3779B97F4A7C15
    imul    r10, rax                        /* n * K0                                 */
    mov     r11, 5381
    xor     r11, r10                        /* h = 5381 ^ (n * K0)                    */
    mov     r10, 0xFF51AFD7ED558CCD         /* K1, held live across the loop          */
    cmp     ecx, 8
    jb      .Ltf_str_tail
.Ltf_str_w8:
    xor     r11, [r8]                       /* unaligned 8-byte load is fine on x86-64 */
    imul    r11, r10
    add     r8, 8
    sub     ecx, 8
    cmp     ecx, 8
    jae     .Ltf_str_w8
.Ltf_str_tail:
    test    ecx, ecx
    je      .Ltf_str_fin
    cmp     ecx, 4
    jb      .Ltf_str_t2
    mov     eax, [r8]                       /* front 4                                */
    mov     r9d, ecx
    sub     r9d, 4
    mov     r9d, [r8 + r9]                  /* last 4, ending exactly at p+m          */
    shl     r9, 32
    or      rax, r9
    jmp     .Ltf_str_mix
.Ltf_str_t2:
    cmp     ecx, 2
    jb      .Ltf_str_t1
    movzx   eax, word ptr [r8]
    mov     r9d, ecx
    sub     r9d, 2
    movzx   r9d, word ptr [r8 + r9]         /* last 2, ending exactly at p+m          */
    shl     r9, 32
    or      rax, r9
    jmp     .Ltf_str_mix
.Ltf_str_t1:
    movzx   eax, byte ptr [r8]
.Ltf_str_mix:
    xor     r11, rax
    imul    r11, r10
    jmp     .Ltf_str_fin
.Ltf_str_n0:
    mov     r11, 5381                       /* n == 0: h = 5381 ^ (0 * K0)            */
.Ltf_str_fin:
    mov     rax, r11
    shr     rax, 31
    xor     r11, rax
    mov     rcx, 0xC4CEB9FE1A85EC53
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
    mov     eax, [rdi + TBBLK_NBUCK]        /* nbuck, always a power of two          */
    dec     eax                             /* mask                                   */
    and     eax, r11d                       /* bucket = hkey & (nbuck-1)              */
    mov     rcx, [rdi + TBBLK_BUCKETS]      /* the vector, its own block              */
    mov     rcx, [rcx + rax*8]              /* b = buckets[bucket]                    */
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
/* --- DT_S equality: LENGTH FIRST, then bytes. Never a NUL-terminated compare.
 * A length mismatch rejects without touching a byte, and an equal length then
 * compares exactly that many bytes -- so an embedded CHAR(0) is just data. */
.Ltf_eq_str:
    mov     rax, [r10 + TBPAIR_KEYD_VAL]    /* e->key_descr.s                        */
    test    rax, rax
    je      .Ltf_next
    test    rdi, rdi
    je      .Ltf_next
    cmp     rax, rdi
    je      .Ltf_hit                        /* same bytes, trivially equal           */
    mov     edx, [r10 + TBPAIR_KEYD_SLEN]   /* e length                              */
    test    edx, edx
    jne     .Ltf_eq_klen
    mov     r8, rax                         /* unstamped: strlen fallback, as in C   */
.Ltf_eq_escan:
    cmp     byte ptr [r8], 0
    je      .Ltf_eq_escand
    inc     r8
    jmp     .Ltf_eq_escan
.Ltf_eq_escand:
    mov     rdx, r8
    sub     rdx, rax
.Ltf_eq_klen:
    mov     rcx, rsi
    shr     rcx, 32                         /* k length                              */
    test    ecx, ecx
    jne     .Ltf_eq_cmplen
    mov     r8, rdi
.Ltf_eq_kscan:
    cmp     byte ptr [r8], 0
    je      .Ltf_eq_kscand
    inc     r8
    jmp     .Ltf_eq_kscan
.Ltf_eq_kscand:
    mov     rcx, r8
    sub     rcx, rdi
.Ltf_eq_cmplen:
    cmp     edx, ecx
    jne     .Ltf_next                       /* different lengths => different keys    */
    test    ecx, ecx
    je      .Ltf_hit                        /* both empty                             */
    mov     r8, rdi
.Ltf_scmp:
    movzx   edx, byte ptr [rax]
    movzx   r9d, byte ptr [r8]
    cmp     dl, r9b
    jne     .Ltf_next
    inc     rax
    inc     r8
    dec     ecx
    jne     .Ltf_scmp
    jmp     .Ltf_hit

.Ltf_hit:
    mov     rax, r10
    ret
.Ltf_null:
    xor     eax, eax
    ret
RTX_ENDF(table_find_pair_d)
