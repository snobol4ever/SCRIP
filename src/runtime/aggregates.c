#include "rt/rt_arena.h"
#include "rt/gc_heap.h"
#include "core.h"
#include "sil_macros.h"
#include <string.h>
/* Forward declarations: table_new sits above the hash/bucket machinery in this file and needs its sizing helpers. */
static unsigned  _tbl_nbuck_for(int init);
static struct _TBBUCK_t **_tbl_vec_new(unsigned nb);
static long g_agg_list_ser = 1;
static long g_agg_table_ser = 1;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long rt_agg_serial_list(void) { return g_agg_list_ser++; }
long rt_agg_serial_table(void) { return g_agg_table_ser++; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
ARBLK_t *array_new(int lo, int hi) {
    ARBLK_t *a = rt_ws_alloc(sizeof(ARBLK_t));
    a->lo   = lo;
    a->hi   = hi;
    a->ndim = 1;
    a->id   = g_agg_list_ser++;
    a->proto = (const char *)0;
    int sz  = hi - lo + 1;
    if (sz < 1) sz = 1;
    a->data = rt_ws_alloc(sz * sizeof(DESCR_t));
    for (int i = 0; i < sz; i++) a->data[i] = NULVCL;
    return a;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
ARBLK_t *array_new2d(int lo1, int hi1, int lo2, int hi2) {
    ARBLK_t *a = rt_ws_alloc(sizeof(ARBLK_t));
    a->lo   = lo1;
    a->hi   = hi1;
    a->lo2  = lo2;
    a->hi2  = hi2;
    a->ndim = 2;
    a->id   = g_agg_list_ser++;
    a->proto = (const char *)0;
    int rows = hi1 - lo1 + 1;
    int cols = hi2 - lo2 + 1;
    if (rows < 1) rows = 1;
    if (cols < 1) cols = 1;
    a->data = rt_ws_alloc(rows * cols * sizeof(DESCR_t));
    for (int i = 0; i < rows * cols; i++) a->data[i] = NULVCL;
    return a;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t array_get(ARBLK_t *a, int i) {
    if (!a) return FAILDESCR;
    int idx = i - a->lo;
    if (idx < 0 || idx >= (a->hi - a->lo + 1)) return FAILDESCR;
    return a->data[idx];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void array_set(ARBLK_t *a, int i, DESCR_t v) {
    if (!a) return;
    int idx = i - a->lo;
    if (idx < 0 || idx >= (a->hi - a->lo + 1)) return;
    a->data[idx] = v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t array_get2(ARBLK_t *a, int i, int j) {
    if (!a) return FAILDESCR;
    int cols = a->hi2 - a->lo2 + 1;
    int row  = i - a->lo;
    int col  = j - a->lo2;
    int idx  = row * cols + col;
    int total = (a->hi - a->lo + 1) * cols;
    if (row < 0 || row >= (a->hi - a->lo + 1) || col < 0 || col >= cols || idx < 0 || idx >= total)
        return FAILDESCR;
    return a->data[idx];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void array_set2(ARBLK_t *a, int i, int j, DESCR_t v) {
    if (!a) return;
    int cols = a->hi2 - a->lo2 + 1;
    int row  = i - a->lo;
    int col  = j - a->lo2;
    int idx  = row * cols + col;
    a->data[idx] = v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static TBBUCK_t **_tbl_vec_new(unsigned nb) {
    TBBUCK_t **v = rt_gcheap_alloc(HB_AGGB, (unsigned long long)nb * sizeof(TBBUCK_t *));
    memset(v, 0, (size_t)nb * sizeof(TBBUCK_t *));
    return v;
}
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
TBBLK_t *table_new(void) {
    TBBLK_t *t = rt_agg_alloc(2, sizeof(TBBLK_t));   /*⭐ s262: a zeroed TBBUCK_t IS the empty bucket -- {slot=0,len=0,cap=0} -- so the bulk clear survives the chain-to-array change unchanged */
    t->id   = g_agg_table_ser++;
    t->size = 0;
    t->init = 11;
    t->inc  = 10;
    t->is_set = 0;
    t->nbuck = _tbl_nbuck_for(t->init);
    t->buckets = _tbl_vec_new(t->nbuck);
    return t;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*⛔ inc IS ACCEPTED AND IGNORED, DELIBERATELY AND ON THE ORACLE'S AUTHORITY: SPITBOL manual sec 4214 -- "Arg2 is just there for
  compatibility with other versions of SNOBOL; it is ignored by SPITBOL".  It is stored so PROTOTYPE can report it, never consulted for sizing. */
TBBLK_t *table_new_args(int init, int inc) {
    TBBLK_t *t = table_new();
    if (inc  > 0) t->inc  = inc;
    if (init > 0) {
        t->init  = init;
        unsigned nb = _tbl_nbuck_for(init);
        if (nb != t->nbuck) { t->nbuck = nb; t->buckets = _tbl_vec_new(nb); }   /* empty table: nothing to move */
    }
    return t;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t agg_prototype(DESCR_t v) {
    if (IS_TBL(v) && v.tbl) return INTVAL(v.tbl->init);
    if (!(IS_ARR(v) && v.arr)) { core_runtime_error(164, "prototype argument is not valid object"); return FAILDESCR; }
    ARBLK_t *a = v.arr;
    char pb[64];
    const char *p = a->proto;
    if (!p) {
        if (a->ndim > 1) snprintf(pb, sizeof pb, "%d,%d", a->hi - a->lo + 1, a->hi2 - a->lo2 + 1);
        else if (a->lo == 1) snprintf(pb, sizeof pb, "%d", a->hi);
        else snprintf(pb, sizeof pb, "%d:%d", a->lo, a->hi);
        p = pb;
    }
    int alldig = (p[0] != 0);
    for (const char *q = p; *q; q++) if (*q < '0' || *q > '9') { alldig = 0; break; }
    if (!alldig) return STRVAL(rt_ws_strdup_c(p));
    long long iv = 0;
    for (const char *q = p; *q; q++) iv = iv * 10 + (*q - '0');
    return INTVAL(iv);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *tbl_key_str(DESCR_t kd, char *buf, size_t bufn) {
    switch (kd.v) {
        case DT_SNUL: return "\001n";
        case DT_S:    return kd.s ? kd.s : "";
        case DT_I:    { char *p = buf; *p++ = '\001'; *p++ = 'i'; long long v = (long long)kd.i; unsigned long long u; if (v < 0) { *p++ = '-'; u = (unsigned long long)(-(v + 1)) + 1ull; } else u = (unsigned long long)v;
                        char t[24]; int n = 0; do { t[n++] = (char)('0' + (int)(u % 10ull)); u /= 10ull; } while (u); while (n) *p++ = t[--n]; *p = 0; (void)bufn; return buf; }
        case DT_R:    snprintf(buf, bufn, "\001r%.17g", kd.r); return buf;
        case DT_DATA: { if (!kd.u) return "\001d0"; snprintf(buf, bufn, "\001d%s#%ld", kd.u->type ? kd.u->type->name : "?", kd.u->id); return buf; }
        case DT_A:    { if (!kd.arr) return "\001l0"; if (!kd.arr->id) kd.arr->id = g_agg_list_ser++; snprintf(buf, bufn, "\001l%ld", kd.arr->id); return buf; }
        case DT_T:    { if (!kd.tbl) return "\001t0"; if (!kd.tbl->id) kd.tbl->id = g_agg_table_ser++; snprintf(buf, bufn, "\001%c%ld", kd.tbl->is_set ? 'S' : 't', kd.tbl->id); return buf; }
        default:      snprintf(buf, bufn, "\001p%p", kd.ptr); return buf;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*⭐⭐⭐ THE TABLE, REWRITTEN (Lon, 2026-08-23 s262): HASH BY DATATYPE THEN BY VALUE . CONTIGUOUS BUCKET . 2x/FIXED GROWTH . BINARY SEARCH ON LOOKUP.
  ⛔ WHAT THE OLD SHAPE WAS AND WHY IT HAD TO GO.  A bucket was a singly-linked chain of separately-allocated 48-byte entries, so a lookup that missed on the first
  entry paid a dependent load per link -- pointer-chasing with no prefetch and one cache line touched per candidate.  Worse, the s262 typed hash had been landed on
  only SIX datatypes; every other tag still fell through to `tbl_key_str` + djb2, which means an `snprintf("%p")` PER LOOKUP for patterns, code, files and Prolog refs.
  ⭐ WHAT REPLACES IT.  (1) hkey = (datatype << 56) | (mix64(value) >> 8) -- the DATATYPE IS THE HIGH BYTE, so the ordering itself is datatype-major and two keys of
  different type can never compare equal no matter what their values hash to.  (2) A bucket is a CONTIGUOUS, hkey-SORTED array of 16-byte {hkey, entry*} slots: four
  per cache line, binary-searched.  (3) It grows 2x while small and by a FIXED +128 once large, because doubling a hot bucket past 128 slots wastes arena the GC then
  has to sweep.  (4) A hit is a binary search plus ONE dereference: over a 56-bit value hash the equal-hkey run is essentially always length 1, so the linear scan
  after the search is a formality that exists only for the genuine-collision case.
  ⭐ THE ENTRIES ARE INLINE IN THE BUCKET (Lon s262: "we should never have in our code a place that depends on that pointer not moving") -- see the ⭐⭐ block in core.h for what that replaced and why the first cut had it backwards.
  ⭐ EVERY DATATYPE IS HANDLED BY VALUE.  Not six: all of them.  DT_S is bytes, DT_I is the integer, DT_R is the bit pattern, DT_A/DT_T/DT_DATA are their SERIAL IDs
  (deterministic run to run -- ⛔ never the pointer, or CONVERT's iteration order would move under ASLR), and every remaining tag is its payload word, which is the
  object identity the old "%p" encoding was reaching for anyway.  NOTHING calls snprintf on a lookup any more.
  ⛔ THE MIXED-HASH HAZARD IS GONE BY CONSTRUCTION, NOT BY DISCIPLINE.  The old file carried a warning that string-keyed and descriptor-keyed callers must not share a
  table.  A warning is not a mechanism: the string-keyed lookups are DELETED in this change and their two callers (runtime_shim.h) converted, so there is no longer a
  second way to hash a key.  tbl_key_str survives ONLY to populate e->key for iteration/CONVERT/sorting, built ONCE on insert.
  KILLSWITCH SCRIP_TBL_TYPED=0 forces every key through the tbl_key_str encoding before hashing -- same buckets, same binary search, string-derived hkey -- so the
  typed hash can be A/B'd on one binary without reviving the chain. */
static inline __attribute__((always_inline)) int tbl_typed_off(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_TBL_TYPED"); v = (e && *e == '0') ? 1 : 0; } return v; }
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*⭐⭐⭐ ONE HASH ALGORITHM PER DATATYPE -- NOT ONE EXTRACTOR PER DATATYPE FEEDING ONE SHARED MIXER.
  ⛔ THE INTERMEDIATE VERSION OF THIS BLOCK FAILED THAT TEST AND IT IS WORTH RECORDING WHY, BECAUSE IT LOOKED DONE.  It had eight named per-datatype functions --
  but seven of them were the SAME algorithm wearing different names: pull the 64-bit value word out of the descriptor and hand it to one shared _tbl_mix64.  Only
  DT_S had an algorithm of its own.  "One function per datatype" is a naming convention; "one hash algorithm per datatype" is a design, and they are not the same
  claim.  What follows is the design: each arm ends in its OWN multiply with its OWN constant, chosen for how THAT datatype's values are actually distributed.
  ⭐ EVERY ARM RETURNS A FINISHED 56-BIT HASH, high bits down.  Multiplicative hashing concentrates entropy in the HIGH half of the product, so each algorithm ends
  in `>> 8` -- which both trims to the 56 bits _tbl_hkey has room for AND puts the well-mixed middle of the product into the LOW bits, where TBL_BUCKET_OF reads.
  A shared post-mix would undo the point of having per-datatype algorithms at all, so there is none: after this, _tbl_hkey only prepends the tag byte.
  ⛔ THE CONSTANTS ARE DELIBERATELY DIFFERENT PER TYPE.  The tag byte already keeps types in separate ORDER ranges, but every type shares one 256-bucket space; a
  common multiplier would give an integer key and a table's serial id correlated bucket sequences, so a program keying a table by small ints AND by small aggregates
  would pile both into the same buckets.  Distinct odd constants decorrelate them. */
static inline __attribute__((always_inline)) unsigned long long _tbl_rotl(unsigned long long u, int n) { return (u << n) | (u >> (64 - n)); }
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* DT_SNUL -- THE NULL STRING IS A SINGLETON.  There is exactly one such key, so there is nothing to hash and no distribution to spread: a constant is the whole
   algorithm, and it is a distinguished one (not 0) so the empty key does not land in bucket 0 alongside every zero-valued pointer type. */
static inline __attribute__((always_inline)) unsigned long long _tbl_h_snul(const DESCR_t *k) { (void)k; return 0x2F1B3D5C7E9A11ull; }
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* DT_S -- STREAMING, BECAUSE THE KEY IS N BYTES AND NOT A WORD.  djb2 (h*33 ^ byte) over the bytes: shift-add-xor per byte, no multiply in the loop, which is what
   makes it the right shape for the inlined ASM arm.  ⛔ djb2's own low bits are weak for SHORT keys -- 'a', 'b', 'c' differ only in the bottom bits -- and short
   keys are exactly what SNOBOL4 programs use, so the accumulator gets ONE finishing multiply to drive that difference up into the high half before the >> 8. */
/*⛔⛔ COUNTED, NEVER NUL-TERMINATED (Lon, 2026-08-23 s263): *"Using any C function to manipulate strings is INVALID since the NUL character problem."*
  A SNOBOL4 string may CONTAIN CHAR(0) -- it is a counted string, and its length is the only thing that says where it ends.  This arm used to walk until *p == 0,
  which meant 'a' CHAR(0) 'b' and 'a' CHAR(0) 'c' hashed IDENTICALLY, and the equality arm below used strcmp, which called them EQUAL.  Two distinct keys silently
  collapsing into one table entry is a wrong ANSWER, not a slow one.  Both arms are length-driven now and they agree by construction.
  ⛔ THE slen == 0 FALLBACK IS THE CODEBASE'S BUG, NOT THIS FILE'S, AND IT IS DELIBERATELY LEFT VISIBLE.  `slen ? slen : strlen(s)` is the idiom everywhere in the
  runtime (rt_runtime.c:77, pattern_match.c:54, string_builtins.c:50, ...), so a descriptor that never got its length stamped still truncates here.  Hashing and
  equality use the SAME length, so the table stays self-consistent either way -- it can lose a distinction the descriptor already lost, never invent one.  The
  real cure is that every DT_S descriptor carries slen; that is a runtime-wide sweep and it is routed to hq_C. */
static inline __attribute__((always_inline)) unsigned _tbl_slen(const DESCR_t *k) { return k->slen ? k->slen : (k->s ? (unsigned)strlen(k->s) : 0u); }
static inline __attribute__((always_inline)) unsigned long long _tbl_h_str(const DESCR_t *k) {
    unsigned long long h = 5381ull; const unsigned char *p = (const unsigned char *)(k->s ? k->s : ""); unsigned n = _tbl_slen(k);
    while (n--) h = h * 33ull ^ (unsigned long long)*p++;
    return (h * 0x9E3779B97F4A7C15ull) >> 8;
}
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* DT_I -- FIBONACCI (KNUTH) MULTIPLICATIVE, ONE IMUL, NO AVALANCHE CHAIN.  Integer keys in real programs are CONSECUTIVE (1..n loop counters, ids, indices), and
   for consecutive inputs the golden-ratio multiplier is not merely adequate, it is optimal: successive products are maximally spread across the word, so 1..n fills
   the buckets perfectly evenly with no xor-shift rounds at all.  ⛔ Do NOT "improve" this into a full avalanche mixer -- that is two more multiplies and two shifts
   to buy nothing on the input distribution that actually arrives, and integer subscripting is the hottest table path in the language. */
static inline __attribute__((always_inline)) unsigned long long _tbl_h_int(const DESCR_t *k) { return ((unsigned long long)(long long)k->i * 0x9E3779B97F4A7C15ull) >> 8; }
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* DT_R -- FOLD FIRST, BECAUSE A DOUBLE'S ENTROPY IS ALL IN THE TOP HALF.  Program-written reals (0.5, 2.5, 1.0/3.0 aside) have long runs of ZERO low mantissa
   bits, so hashing the raw 64-bit pattern feeds a multiplier 20-odd dead bits.  Folding high^low first moves sign+exponent+high mantissa -- the part that actually
   differs between 1.5 and 2.5 -- into the bits the multiply then spreads.  ⛔ BITWISE, matching the "%.17g" text this replaced: -0.0 and 0.0 stay DIFFERENT keys,
   and a NaN key stays findable because its bits equal themselves.  _tbl_eq_d compares the same bits, so hash and equality agree by construction. */
static inline __attribute__((always_inline)) unsigned long long _tbl_h_real(const DESCR_t *k) {
    union { double d; unsigned long long u; } cv; cv.d = k->r;
    return ((cv.u ^ (cv.u >> 32)) * 0xBF58476D1CE4E5B9ull) >> 8;
}
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* DT_A / DT_T -- SERIAL IDS ARE SMALL AND DENSE, LIKE INTEGERS, SO THEY NEED THE OPPOSITE TREATMENT: a DIFFERENT multiplier, or `tab[3]` and the third array ever
   minted walk the same bucket sequence.  A rotate before the multiply also breaks the shared low-bit structure of "1, 2, 3, ..." between the two id counters, which
   are independent and both start at 1 -- so array #4 and table #4 do not chase each other bucket for bucket either.  ⛔ IDS, NEVER ADDRESSES: hashing the pointer
   would make bucket layout ASLR-dependent and CONVERT's iteration order would differ from run to run. */
static inline __attribute__((always_inline)) unsigned long long _tbl_h_arr(const DESCR_t *k) {
    if (!k->arr) return 0x51ED270B2C1A33ull;
    if (!k->arr->id) k->arr->id = g_agg_list_ser++;
    return (_tbl_rotl((unsigned long long)k->arr->id, 21) * 0xD6E8FEB86659FD93ull) >> 8;
}
static inline __attribute__((always_inline)) unsigned long long _tbl_h_tbl(const DESCR_t *k) {
    if (!k->tbl) return 0x1C69B3F74AC4Aull;
    if (!k->tbl->id) k->tbl->id = g_agg_table_ser++;
    return (_tbl_rotl((unsigned long long)k->tbl->id, 43) * 0xD6E8FEB86659FD93ull) >> 8;
}
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* DT_DATA -- TWO-PART IDENTITY, SO TWO PARTS GO IN.  A record key is (which type, which instance); instance ids are dense per program, not per type, so hashing the
   id alone spreads fine but loses the type entirely -- and the tag byte cannot recover it, because every user datatype above DT_DATA shares the DT_DATA range only
   by stride, not by tag.  Mixing the type descriptor in keeps two instances of DIFFERENT record types apart even when their ids are close. */
static inline __attribute__((always_inline)) unsigned long long _tbl_h_data(const DESCR_t *k) {
    if (!k->u) return 0x3A5C17E9B24D6Full;
    return (((unsigned long long)k->u->id ^ _tbl_rotl((unsigned long long)(uintptr_t)k->u->type, 32)) * 0xFF51AFD7ED558CCDull) >> 8;
}
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* EVERY REMAINING TAG (DT_P/C/N/K/E/FH/PLVAR/PLREF/X/BLK/FAIL) -- IDENTITY IS THE ADDRESS, AND AN ADDRESS IS NOT AN INTEGER.  Allocator output is 16-byte aligned,
   so the low FOUR bits are always zero and hashing a pointer as if it were an int feeds the multiplier four dead bits and quarters the effective bucket count.
   Shift them off first.  This is what the old "\001p%p" encoding was reaching for, minus the snprintf. */
static inline __attribute__((always_inline)) unsigned long long _tbl_h_ptr(const DESCR_t *k) { return (((unsigned long long)(uintptr_t)k->ptr >> 4) * 0xC2B2AE3D27D4EB4Full) >> 8; }
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* THE DISPATCH.  A switch, not a function-pointer table: the tags are dense (stride 8) so this compiles to a jump, and a table of pointers would be a new file-scope
   array -- NO-NEW-GLOBALS applies and this needs no grant.  ⭐ The ASM port replaces this with a jump on (v >> 3), which is why the arms are ordered by tag value. */
static inline __attribute__((always_inline)) unsigned long long _tbl_hval(const DESCR_t *k) {
    switch (k->v) {
        case DT_SNUL: return _tbl_h_snul(k);
        case DT_S:    return _tbl_h_str (k);
        case DT_I:    return _tbl_h_int (k);
        case DT_R:    return _tbl_h_real(k);
        case DT_A:    return _tbl_h_arr (k);
        case DT_T:    return _tbl_h_tbl (k);
        case DT_DATA: return _tbl_h_data(k);
        default:      return _tbl_h_ptr (k);
    }
}
/*⭐ DATATYPE FIRST, THEN VALUE -- and the datatype is the HIGH BYTE so it dominates the sort order too.  Bucket = the low bits of hkey, which are value-hash bits
  (mix64 avalanches, and >>8 keeps the well-mixed middle), so distinct types spread across all 256 buckets instead of clustering by tag. */
static inline __attribute__((always_inline)) unsigned long long _tbl_hkey(DESCR_t k) {
    if (tbl_typed_off()) { char kb[64]; DESCR_t sk = k; sk.v = DT_S; sk.s = (char *)tbl_key_str(k, kb, sizeof kb); return ((unsigned long long)DT_S << 56) | _tbl_h_str(&sk); }
    return ((unsigned long long)k.v << 56) | (_tbl_hval(&k) & 0x00FFFFFFFFFFFFFFull);   /*⭐ NO SHARED POST-MIX: each per-datatype algorithm already returned a finished 56-bit hash.  All this adds is the tag byte on top. */
}
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*⭐⭐⭐ THE BUCKET COUNT COMES FROM THE PROGRAM'S OWN TABLE(n) (hq_P s263, on Lon's prompt to go look at what that argument actually means).
  ⛔ WHAT IT MEANS, CHECKED AGAINST BOTH ORACLES RATHER THAN ASSUMED.  SPITBOL manual v3.7 sec 4208-4219 and 4536-4538: TABLE(Arg1,Arg2,Arg3) -- "Arg1 is the
  estimated size... The number you specify is NOT A LIMIT on the size of the table -- it just sets aside an initial amount of memory", "Arg2 is just there for
  compatibility with other versions of SNOBOL; IT IS IGNORED BY SPITBOL", Arg3 is the default entry value.  CSNOBOL4's snobol4func(1) agrees: "optional initial
  size n".  So Arg1 is an ESTIMATED ENTRY COUNT.  It is NOT a bucket count and NOT a ceiling -- TABLE(10) must still hold ten thousand entries correctly.
  ⛔ SCRIP WAS IGNORING IT FOR SIZING AND GAVE EVERY TABLE 256 BUCKETS.  That is a 2 KB pointer vector cleared per TABLE(), free for one big table and ruinous for a
  program that builds thousands of small ones -- CLAWS5 is exactly that program (a TABLE per number, and a TABLE per word inside it), and it was paying +17%
  cache-misses against the chained original for vectors that were 99% empty.
  ⭐ SO: nbuck is a power of two derived from init, aimed at ~2 entries per bucket, floored at 4 and capped so a wild estimate cannot allocate the world.  And
  because Arg1 is an ESTIMATE rather than a limit, the table REHASHES when it outgrows it (_tbl_rehash).  A wrong estimate costs a rehash, never correctness. */
static unsigned _tbl_nbuck_for(int init) {
    unsigned want = (init > 0) ? (unsigned)init / 2u : 4u, nb = 4u;
    while (nb < want && nb < 65536u) nb <<= 1;
    return nb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define TBL_BUCKET_OF(t_, h_) ((unsigned)(h_) & ((t_)->nbuck - 1u))
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Datatype first, then value.  Reached only for candidates whose FULL 64-bit hkey already matched, so it is the genuine-collision arm, not the hot path. */
static inline __attribute__((always_inline)) int _tbl_eq_d(const TBPAIR_t *e, DESCR_t k) {
    if (e->key_descr.v != k.v) return 0;
    switch (k.v) {
        case DT_SNUL: return 1;
        /*⭐ LENGTH FIRST, THEN memcmp -- NEVER strcmp.  A DESCR_t already carries slen, so a length compare rejects a
           mismatch without touching a single byte, and a match then costs a LENGTH-BOUNDED memcmp instead of a scan
           that has to hunt for the NUL.  __strcmp_avx2 was 5.4% of CLAWS5, a string-keyed program, entirely from
           this arm.  ⛔ slen == 0 is ambiguous in SCRIP (it can mean "empty" or "not stamped, use strlen"), so a
           zero on EITHER side falls back to strcmp rather than guessing -- the fast path is only taken when both
           lengths are known.  Identical pointers short-circuit both. */
        case DT_S:    { const char *a = e->key_descr.s, *b = k.s; unsigned la, lb;
                        if (a == b) return 1;
                        if (!a || !b) return 0;
                        la = _tbl_slen(&e->key_descr); lb = _tbl_slen(&k);
                        return la == lb && memcmp(a, b, (size_t)la) == 0; }   /* length first: rejects without touching a byte, and memcmp is NUL-clean where strcmp was not */
        case DT_I:    return e->key_descr.i == k.i;
        case DT_R:    { union { double d; unsigned long long u; } a, b; a.d = e->key_descr.r; b.d = k.r; return a.u == b.u; }
        case DT_A:    return e->key_descr.arr == k.arr;
        case DT_T:    return e->key_descr.tbl == k.tbl;
        case DT_DATA: return e->key_descr.u == k.u;
        default:      return e->key_descr.ptr == k.ptr;
    }
}
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*⭐⭐ LOWER BOUND over the bucket's sorted hkeys -- BINARY ABOVE THE CROSSOVER, LINEAR BELOW IT, AND THE CROSSOVER WAS MEASURED, NOT GUESSED.
  ⛔ THE FIRST CUT BINARY-SEARCHED UNCONDITIONALLY AND THAT COST 26% Ir ON table_access.  The reason is arithmetic, not doctrine: TABLE_BUCKETS is 256, so a
  500-key table averages TWO entries per bucket, and a binary search over two elements is all setup and no saving.  Worse, TBPAIR_t is 48 bytes -- not a power of
  two -- so every probe `en[mid]` is an imul, while a chain walk was one load per link.  Halving beats scanning only once the bucket is deep enough to amortise
  that; below the crossover a forward pointer walk over SORTED hkeys wins, and it keeps the early-out `hkey > h => miss` that an unsorted chain could never have.
  ⭐ THE SORTED INVARIANT PAYS FOR BOTH ARMS: the linear arm stops at the first hkey past the target instead of walking the whole bucket, so even the small-bucket
  case reads about half the entries a chain did on a miss.  MEASURED at fixed work (200 rebuilds x 500 int keys, callgrind Ir, RT_OPT=-O0). */
#define TBL_LINEAR_MAX 12u
#define TBL_LOAD_MAX    4u   /* average entries per bucket before the vector doubles; 4 keeps every bucket inside the linear-scan window */
static inline __attribute__((always_inline)) unsigned _tbl_lower(const TBPAIR_t *en, unsigned n, unsigned long long h) {
    if (n <= TBL_LINEAR_MAX) { const TBPAIR_t *p = en, *e = en + n; while (p < e && p->hkey < h) p++; return (unsigned)(p - en); }
    { unsigned lo = 0;
      while (n) { unsigned half = n >> 1; unsigned mid = lo + half; if (en[mid].hkey < h) { lo = mid + 1; n -= half + 1; } else n = half; }
      return lo; }
}
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*⭐ GROW BY 2x WHILE SMALL, BY A FIXED STEP ONCE LARGE.  First allocation is seeded from the program's own TABLE(init) prototype -- init/256 rounded up to a power of
  two -- ⛔ AND THE FLOOR IS 1, NOT 4.  A floor of 4 allocates 200 bytes for a bucket holding ONE entry, where the chain it replaced allocated one 48-byte node; on CLAWS5 (thousands of tiny tables) that showed up as 499K cache misses against the chain's 354K.  Floor 1 brings it to 375K and costs 1.6% on the integer kernel (153.3M -> 155.8M Ir, both far under the chain's 185.8M).  MEASURED with perf cache-misses; callgrind could not see it.  ⛔ Fixed +128 past 128: doubling a hot bucket to 4096 slots
  costs 64 KB of arena the GC then sweeps every cycle, and the binary search does not care whether the array grew geometrically. */
static TBBUCK_t *_tbl_grow(TBBLK_t *tbl, TBBUCK_t *b) {
    unsigned nc;
    if (!b)                 { int hint = tbl->init / (int)tbl->nbuck; nc = 1u; while (nc < (unsigned)hint && nc < 64u) nc <<= 1; }
    else if (b->cap < 128u) nc = b->cap * 2u;
    else                    nc = b->cap + 128u;
    TBBUCK_t *nb = rt_gcheap_alloc(HB_AGGB, (unsigned long long)(sizeof(TBBUCK_t) + (size_t)nc * sizeof(TBPAIR_t)));   /*⛔ HB_AGGB, NOT rt_agg_alloc.  Every rt_agg_alloc kind is a type the GC sweep CASTS -- kind 0 is read back as a VCELL_t, kind 1 as a SINGLE TBPAIR_t, kind 2 as a TBBLK_t -- and a bucket is an ARRAY of entries, which none of those three describe. */
    if (b && b->len) { memcpy(nb->ent, b->ent, (size_t)b->len * sizeof(TBPAIR_t)); nb->len = b->len; }
    nb->cap = nc;
    return nb;
}
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*⭐ THE ONE LOOKUP.  Binary search to the first slot with this hkey, then walk the equal-hkey run -- length 1 in every non-adversarial program -- confirming by value. */
const char *tbl_pair_key(TBPAIR_t *e) {
    if (!e) return "";
    if (!e->key) { char kb[64]; e->key = rt_ws_strdup_c(tbl_key_str(e->key_descr, kb, sizeof kb)); }
    return e->key ? e->key : "";
}
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
TBPAIR_t *c_table_find_pair_d(TBBLK_t *tbl, DESCR_t k) {   /*⭐ THE C OF RECORD.  rtx_table.S owns the exported symbol table_find_pair_d and tail-jumps here when its gate is clear or the key's datatype lazily assigns a serial id (DT_A/DT_T/DT_DATA -- file-static counters, and NO-NEW-GLOBALS forbids exporting them without a grant). */
    if (!tbl) return (TBPAIR_t *)0;
    unsigned long long h = _tbl_hkey(k);
    TBBUCK_t *b = tbl->buckets[TBL_BUCKET_OF(tbl, h)];
    if (!b) return (TBPAIR_t *)0;
    { const TBPAIR_t *p = b->ent + _tbl_lower(b->ent, b->len, h), *e = b->ent + b->len;   /* pointer walk: the equal-hkey run is ~1 long, and a pointer bump beats recomputing i*48 per step */
      for (; p < e && p->hkey == h; p++) if (_tbl_eq_d(p, k)) return (TBPAIR_t *)p; }
    return (TBPAIR_t *)0;
}
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t table_get_d(TBBLK_t *tbl, DESCR_t k) { TBPAIR_t *e = table_find_pair_d(tbl, k); return e ? e->val : NULVCL; }
DESCR_t table_get_found_d(TBBLK_t *tbl, DESCR_t k, int *found) { TBPAIR_t *e = table_find_pair_d(tbl, k); *found = e ? 1 : 0; return e ? e->val : NULVCL; }
int     table_has_d(TBBLK_t *tbl, DESCR_t k) { return table_find_pair_d(tbl, k) != (TBPAIR_t *)0; }
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Delete COMPACTS the slot array so slot[0..len-1] stays dense and sorted; the entry block itself is left for the GC, since a VCELL may still name its val cell. */
int table_delete_d(TBBLK_t *tbl, DESCR_t k) {
    if (!tbl) return 0;
    unsigned long long h = _tbl_hkey(k);
    TBBUCK_t *b = tbl->buckets[TBL_BUCKET_OF(tbl, h)];
    if (!b) return 0;
    for (unsigned i = _tbl_lower(b->ent, b->len, h); i < b->len && b->ent[i].hkey == h; i++)
        if (_tbl_eq_d(&b->ent[i], k)) { memmove(&b->ent[i], &b->ent[i + 1], (size_t)(b->len - i - 1) * sizeof(TBPAIR_t)); b->len--; tbl->size--; return 1; }
    return 0;
}
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*⭐⭐ REHASH -- WHAT MAKES TABLE(n) AN ESTIMATE RATHER THAN A LIMIT.  SPITBOL is explicit that n "is not a limit on the size of the table", so a table handed a bad
  estimate must degrade in SPEED, never in correctness.  When the average bucket passes TBL_LOAD_MAX entries the vector doubles and every entry is redistributed.
  ⭐ AND IT NEVER RE-HASHES A KEY.  hkey is stored in the entry, so redistribution is a mask against the new width -- no per-key hash, no strcmp, no datatype
  dispatch, and no touching of the key at all.  That is the second time storing the hash pays for its 8 bytes (the first is the sorted binary search).
  ⛔ Entries move here, which is exactly why nothing may hold an address into a bucket -- see the ⭐⭐ block in core.h. */
static void _tbl_rehash(TBBLK_t *tbl) {
    unsigned old_n = tbl->nbuck, nb = old_n << 1;
    if (!nb || nb > 1u << 22) return;                       /* refuse to grow past sanity; the table still works, just deeper buckets */
    TBBUCK_t **ov = tbl->buckets, **nv = _tbl_vec_new(nb);
    tbl->buckets = nv; tbl->nbuck = nb;
    for (unsigned b = 0; b < old_n; b++) {
        TBBUCK_t *ob = ov[b];
        if (!ob) continue;
        for (unsigned i = 0; i < ob->len; i++) {
            TBPAIR_t *e = &ob->ent[i];
            unsigned  nbi = (unsigned)e->hkey & (nb - 1u);
            TBBUCK_t *nbk = nv[nbi];
            if (!nbk || nbk->len == nbk->cap) { nbk = _tbl_grow(tbl, nbk); nv[nbi] = nbk; }
            unsigned j = _tbl_lower(nbk->ent, nbk->len, e->hkey);   /* the old bucket is sorted, but two old buckets interleave into one new one */
            if (j < nbk->len) memmove(&nbk->ent[j + 1], &nbk->ent[j], (size_t)(nbk->len - j) * sizeof(TBPAIR_t));
            nbk->ent[j] = *e; nbk->len++;
        }
    }
}
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*⭐ INSERT KEEPS THE BUCKET SORTED.  Overwrite in place on a hit (no growth, no move); otherwise open a hole at the end of the equal-hkey run and drop the slot in.
  ⭐⭐ AND IT NO LONGER BUILDS e->key AT ALL.  s262 had already cut the stringify from once-per-LOOKUP to once-per-INSERT; measuring the result showed that was still
  12% of table_access -- tbl_key_str 16.1M Ir + rt_ws_strdup_c 2.8M + rt_ws_alloc_c 1.9M + strlen, on a program that never prints a key.  Since _tbl_eq_d compares
  key_descr and the hash is typed, NOTHING on the hot path needs the text: it is minted on first demand by tbl_pair_key() and cached.  A table that is only ever
  subscripted never builds a single key string. */
void table_set_descr_d(TBBLK_t *tbl, DESCR_t k, DESCR_t val) {
    if (!tbl) return;
    { extern void rt_sxt_break(const char *); if (val.v == DT_S) rt_sxt_break(val.s); }
    unsigned long long h = _tbl_hkey(k);
    unsigned bi = TBL_BUCKET_OF(tbl, h);
    TBBUCK_t *b = tbl->buckets[bi];
    unsigned i = b ? _tbl_lower(b->ent, b->len, h) : 0u;
    if (b) for (; i < b->len && b->ent[i].hkey == h; i++)
        if (_tbl_eq_d(&b->ent[i], k)) { b->ent[i].val = val; b->ent[i].key_descr = k; return; }
    if (!b || b->len == b->cap) { b = _tbl_grow(tbl, b); tbl->buckets[bi] = b; }
    if (i < b->len) memmove(&b->ent[i + 1], &b->ent[i], (size_t)(b->len - i) * sizeof(TBPAIR_t));   /* i == len is the common case (a fresh hkey past every sibling) and costs no move at all */
    { TBPAIR_t *n = &b->ent[i]; n->key = (char *)0; n->key_descr = k; n->val = val; n->hkey = h; }   /*⭐ key stays NULL -- tbl_pair_key() mints it if anyone ever asks */
    b->len++; tbl->size++;
    if ((unsigned)tbl->size > tbl->nbuck * TBL_LOAD_MAX) _tbl_rehash(tbl);
}
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*⭐ THE STRING-KEYED LOOKUPS ARE DELETED (s262).  table_get / table_get_found / table_has / table_delete / table_find_pair / table_set_descr / table_set_descr_keyown
  all hashed the ENCODED key as text, which is a second, incompatible way to place an entry in a table -- the exact hazard the old file could only WARN about.  Their
  callers were two inlines in runtime_shim.h, both of which already held the key DESCRIPTOR and merely stringified it on the way in; they now call the _d forms.
  ⛔ Do not reintroduce them.  If a caller has only text, it has a DT_S key: mint the descriptor and use table_get_d. */
static void set_copy_all(TBBLK_t *dst, TBBLK_t *src) {
    if (!dst || !src) return;
    TBPAIR_t *e; TBL_FOREACH(src, e) table_set_descr_d(dst, e->key_descr, e->key_descr);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
TBBLK_t *set_union(TBBLK_t *x, TBBLK_t *y) {
    TBBLK_t *r = table_new(); r->is_set = 1;
    set_copy_all(r, x); set_copy_all(r, y);
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
TBBLK_t *set_diff(TBBLK_t *x, TBBLK_t *y) {
    TBBLK_t *r = table_new(); r->is_set = 1;
    TBPAIR_t *e; if (x) TBL_FOREACH(x, e) if (!table_has_d(y, e->key_descr)) table_set_descr_d(r, e->key_descr, e->key_descr);
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
TBBLK_t *set_inter(TBBLK_t *x, TBBLK_t *y) {
    TBBLK_t *r = table_new(); r->is_set = 1;
    TBPAIR_t *e; if (x) TBL_FOREACH(x, e) if (table_has_d(y, e->key_descr)) table_set_descr_d(r, e->key_descr, e->key_descr);
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_table_idx_get(DESCR_t base, DESCR_t key) {
    if (base.v != DT_T || !base.tbl) return NULVCL;
    return table_get_d(base.tbl, key);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_table_idx_set(DESCR_t base, DESCR_t key, DESCR_t val) {
    if (base.v != DT_T || !base.tbl) return;
    table_set_descr_d(base.tbl, key, val);
}
