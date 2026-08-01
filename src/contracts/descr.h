#ifndef DESCR_H
#define DESCR_H
#include <stdint.h>
#include <stddef.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*
 * DTYPE_t TAG LAYOUT -- ZERO-PRESERVING CLASS BITS (s229, Lon ruling: DT_SNUL STAYS 0).
 *
 *   bit0 = NUMERIC      bit1 = HAS-CHARS      bit2 = REAL      aggregates: stride 8 from 0x08
 *
 * ⭐ DT_SNUL == 0 IS PINNED AND LOad-BEARING. Zeroed memory is a valid null string, which is
 * SNOBOL4 semantics (an unassigned variable IS the null string) and the tree leans on it in
 * bulk: rt.c `rt_gva_island` memsets the WHOLE global variable area, by_name_dispatch.c memsets
 * grown descriptor heap, gc_heap.c zeroes aggregate payload. Keeping 0 keeps all of that free.
 *
 * ⛔ THE ONE THING A ZERO TAG COSTS, STATED SO IT IS NOT REDISCOVERED: 0 is the OR-identity and
 * the AND-annihilator, so it poisons one of the two combined forms. Brute-forced over every
 * assignment of I/R/S with SNUL pinned to 0: SEVEN of the eight predicates below are
 * simultaneously achievable, never eight. The eighth must be surrendered, and the CHEAP one to
 * surrender is the SPITBOL joint-real identity `result tag == (L|R)`, because once the emitter
 * BRANCHES on BOTH-INT to inline the add, the result tag is a compile-time constant in each arm
 * (`mov [slot], DT_I`). The L|R identity only pays for code that does NOT branch -- and inlining
 * requires the branch. So it is surrendered at zero cost. DO NOT "restore" it by making DT_I a
 * subset of DT_R: that re-breaks BOTH INT, which is the hot path.
 *   ⇒ I|R == 0x07 is NOT a valid tag. Never compute an arithmetic result tag with `or`.
 *
 * THE TESTS THAT FALL OUT (verified exhaustively over the full tag set + 64 user datatypes):
 *   is numeric             test eax, DT_NUMERIC_BIT
 *   is string              test eax, DT_NOTSTR_MASK   -> Z
 *   is real (given num)    test eax, DT_REAL_BIT
 *   is NULL vs S           test eax, DT_CHARS_BIT     -> Z
 *   BOTH INT               and eax,ecx ; cmp  eax, DT_I
 *   BOTH REAL              and eax,ecx ; cmp  eax, DT_R
 *   BOTH NUMERIC           and eax,ecx ; test al,  DT_NUMERIC_BIT
 *   BOTH STRING            or  eax,ecx ; test eax, DT_NOTSTR_MASK -> Z
 *
 * ⛔ THE STRING TESTS MUST BE 32-BIT (`test eax,`), NEVER 8-BIT: DT_NOTSTR_MASK has high bits,
 * and user datatypes run past one byte, so an 8-bit form truncates a large DATA tag whose low
 * byte is 0 into a false "is string" hit. The stride-8 rule keeps bit0 clear at every magnitude,
 * so the NUMERIC tests alone are safe in 8-bit form.
 *----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define DT_NUMERIC_BIT 0x01
#define DT_CHARS_BIT   0x02
#define DT_REAL_BIT    0x04
#define DT_NOTSTR_MASK 0xFFFFFFFDu   /* every bit EXCEPT CHARS; (v & mask)==0 <=> v in {SNUL,S} */
#define DT_DATA_STRIDE 8
typedef enum {
    DT_SNUL = 0x00,   /* 0b000  the null string -- ZERO ON PURPOSE, see above */
    DT_S    = 0x02,   /* 0b010  CHARS                                         */
    DT_I    = 0x03,   /* 0b011  NUMERIC | CHARS                               */
    DT_R    = 0x05,   /* 0b101  NUMERIC | REAL                                */
    /* ---- non-scalar: stride 8, NUMERIC bit always clear, never string ---- */
    DT_P    = 0x08,
    DT_A    = 0x10,
    DT_T    = 0x18,   /* A and T adjacent => subscriptable is one range test  */
    DT_C    = 0x20,
    DT_N    = 0x28,
    DT_K    = 0x30,
    DT_E    = 0x38,
    DT_FH   = 0x40,
    DT_PLVAR = 0x48,
    DT_PLREF = 0x50,
    DT_X    = 0x58,
    DT_BLK  = 0x60,
    DT_FAIL = 0x68,   /* carries neither NUMERIC nor string => safe in every numeric guard */
    DT_DATA = 0x70,   /* user datatype k == DT_DATA + 8*k */
} DTYPE_t;
/* Pin every invariant the emitted code leans on, so a later edit breaks the BUILD not the runtime. */
/* DESCR_SASSERT, not _Static_assert: this header is included by the C++ template TUs (-std=c++17), where
 * _Static_assert is not a keyword -- it is C11. The s229 commit used _Static_assert directly and could not
 * build at all; its "11 _Static_asserts compile clean" gate had exercised a C TU only. s230. */
#ifdef __cplusplus
#define DESCR_SASSERT(c, m) static_assert(c, m)
#else
#define DESCR_SASSERT(c, m) _Static_assert(c, m)
#endif
DESCR_SASSERT(DT_SNUL == 0, "DT_SNUL must stay 0: bulk memset init mints null strings for free");
DESCR_SASSERT((DT_I & DT_NUMERIC_BIT) && (DT_R & DT_NUMERIC_BIT), "NUMERIC is bit0");
DESCR_SASSERT(!(DT_S & DT_NUMERIC_BIT) && !(DT_SNUL & DT_NUMERIC_BIT), "strings never read numeric");
DESCR_SASSERT(!(DT_I & DT_REAL_BIT) && (DT_R & DT_REAL_BIT), "REAL is bit2");
DESCR_SASSERT(((DT_I & DT_R) != DT_I) && ((DT_I & DT_R) & DT_NUMERIC_BIT),
               "DT_I must NOT be a subset of DT_R (or BOTH INT dies) yet must share NUMERIC (or BOTH NUMERIC dies)");
DESCR_SASSERT(((DT_SNUL | DT_S) & DT_NOTSTR_MASK) == 0, "SNUL|S must vanish under the string mask");
DESCR_SASSERT((DT_I & DT_NOTSTR_MASK) && (DT_R & DT_NOTSTR_MASK), "numerics must NOT read as string");
DESCR_SASSERT(!(DT_FAIL & DT_NUMERIC_BIT) && (DT_FAIL & DT_NOTSTR_MASK),
               "DT_FAIL must read as neither numeric nor string");
DESCR_SASSERT(!(DT_DATA & DT_NUMERIC_BIT) && !(DT_DATA_STRIDE & DT_NUMERIC_BIT),
               "DATA base and stride must leave NUMERIC clear so no user datatype enters the arith fast path");
DESCR_SASSERT(DT_FAIL < DT_DATA, "the v >= DT_DATA range tests require every fixed tag below DT_DATA");
DESCR_SASSERT(DT_T - DT_A == 8, "rtx_icnsub.S array+table share one subscript range guard");
struct _ARBLK_t;
struct _TBBLK_t;
struct _DATINST_t;
typedef struct DESCR_t {
    DTYPE_t  v;
    uint32_t slen;
    union {
        char              *s;
        int64_t            i;
        double             r;
        void              *p;
        struct _ARBLK_t   *arr;
        struct _TBBLK_t   *tbl;
        struct _DATINST_t *u;
        void              *ptr;
    };
} DESCR_t;
typedef struct _VCELL_t { DESCR_t *cellp; struct _TBBLK_t *tbl; const char *key; DESCR_t key_d; DESCR_t sv; long pos; long len; } VCELL_t;
#define FAILDESCR    ((DESCR_t){ .v = DT_FAIL, .i = 0 })
#define NAMETRAP(vc_) ((DESCR_t){ .v = DT_N, .slen = 2, .p = (void *)(vc_) })
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int IS_FAIL_fn(DESCR_t v) { return v.v == DT_FAIL; }
static inline int IS_NAMETRAP_fn(DESCR_t v) { return v.v == DT_N && v.slen == 2; }
static inline int IS_VARREF_fn(DESCR_t v) { return v.v == DT_N && (v.slen == 2 || (v.slen == 1 && v.ptr) || (v.slen == 0 && v.s && *v.s)); }
#define FHVAL(idx_) ((DESCR_t){ .v = DT_FH, .i = (int64_t)(idx_) })
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int IS_FH_fn(DESCR_t v) { return v.v == DT_FH; }
#endif
