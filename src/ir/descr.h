#ifndef DESCR_H
#define DESCR_H
#include <stdint.h>
#include <stddef.h>
#define DT_NUMERIC_BIT 0x01
#define DT_CHARS_BIT   0x02
#define DT_REAL_BIT    0x04
#define DT_NOTSTR_MASK 0xFFFFFFFDu
#define DT_DATA_STRIDE 8
typedef enum {
    DT_SNUL = 0x00,
    DT_S    = 0x02,
    DT_I    = 0x03,
    DT_R    = 0x05,
    DT_P    = 0x08,
    DT_A    = 0x10,
    DT_T    = 0x18,
    DT_C    = 0x20,
    DT_N    = 0x28,
    DT_K    = 0x30,
    DT_E    = 0x38,
    DT_FH   = 0x40,
    DT_PLVAR = 0x48,
    DT_PLREF = 0x50,
    DT_X    = 0x58,
    DT_BLK  = 0x60,
    DT_FAIL = 0x68,
    DT_DATA = 0x70,
} DTYPE_t;
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
DESCR_SASSERT(((DT_SNUL | DT_S) & (DT_NOTSTR_MASK & 0xFF)) == 0, "SNUL|S must vanish under the 8-bit string mask -- every .S string-family test reads only the tag byte, never the 32-bit word (that word's bits 8-31 carry the DESCR provenance stamp, see ARCH-SNOBOL4-RTX.md sec9)");
DESCR_SASSERT((DT_I & (DT_NOTSTR_MASK & 0xFF)) && (DT_R & (DT_NOTSTR_MASK & 0xFF)), "numerics must NOT read as string under the 8-bit mask");
DESCR_SASSERT(!(DT_FAIL & DT_NUMERIC_BIT) && (DT_FAIL & (DT_NOTSTR_MASK & 0xFF)),
               "DT_FAIL must read as neither numeric nor string under the 8-bit mask");
DESCR_SASSERT(!(DT_DATA & DT_NUMERIC_BIT) && !(DT_DATA_STRIDE & DT_NUMERIC_BIT),
               "DATA base and stride must leave NUMERIC clear so no user datatype enters the arith fast path");
DESCR_SASSERT(DT_FAIL < DT_DATA, "the v >= DT_DATA range tests require every fixed tag below DT_DATA");
DESCR_SASSERT(DT_T - DT_A == 8, "rtx_icnsub.s array+table share one subscript range guard");
struct _ARBLK_t;
struct _TBBLK_t;
struct _DATINST_t;
typedef struct DESCR_t {
    uint8_t  v;
    uint8_t  mod_op;
    uint16_t src_node;
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
DESCR_SASSERT(sizeof(DESCR_t) == 16, "DESCR_t is a SysV register-pair (rax:rdx) INTEGER-class return; 17+ bytes flips it to MEMORY class across 4,009 lines of asm");
#define DESCR_SRC_NODE_UNSTAMPED 0u
#define DESCR_SRC_NODE_OVERFLOW  0xFFFFu
#define DESCR_MOD_OP_UNSTAMPED   0u
typedef struct _VCELL_t { DESCR_t *cellp; struct _TBBLK_t *tbl; const char *key; DESCR_t key_d; DESCR_t sv; long pos; long len; } VCELL_t;
#define FAILDESCR    ((DESCR_t){ .v = DT_FAIL, .i = 0 })
#define NAMETRAP(vc_) ((DESCR_t){ .v = DT_N, .slen = 2, .p = (void *)(vc_) })
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline __attribute__((always_inline)) int IS_FAIL_fn(DESCR_t v) { return v.v == DT_FAIL; }
static inline __attribute__((always_inline)) int IS_NAMETRAP_fn(DESCR_t v) { return v.v == DT_N && v.slen == 2; }
static inline __attribute__((always_inline)) int IS_VARREF_fn(DESCR_t v) { return v.v == DT_N && (v.slen == 2 || (v.slen == 1 && v.ptr) || (v.slen == 0 && v.s && *v.s)); }
#define FHVAL(idx_) ((DESCR_t){ .v = DT_FH, .i = (int64_t)(idx_) })
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline __attribute__((always_inline)) int IS_FH_fn(DESCR_t v) { return v.v == DT_FH; }
#endif
