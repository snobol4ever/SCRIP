#ifndef DESCR_H
#define DESCR_H
#include <stdint.h>
#include <stddef.h>
typedef enum {
    DT_SNUL =  0,
    DT_S    =  1,
    DT_P    =  3,
    DT_A    =  4,
    DT_T    =  5,
    DT_I    =  6,
    DT_R    =  7,
    DT_C    =  8,
    DT_N    =  9,
    DT_K    = 10,
    DT_E    = 11,
    DT_FH   = 12,
    DT_FAIL = 99,
    DT_DATA = 100,
} DTYPE_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
struct _PATND_t;
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
        struct _PATND_t   *p;
        struct _ARBLK_t   *arr;
        struct _TBBLK_t   *tbl;
        struct _DATINST_t *u;
        void              *ptr;
    };
} DESCR_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* DESCR LAYOUT MODE.  A single global drives x86 emission of the inline   */
/* descriptor-store quartet (mov [reg],v / [reg+SLEN],slen / [reg+PAY],    */
/* payload / add reg,STRIDE) and pointer-payload basing.  The C struct is  */
/* the source of truth for offsets via offsetof/sizeof, so the symbolic    */
/* constants below stay correct automatically when the struct changes.     */
/* The emitter reads g_descr_layout to choose stride/offsets and whether   */
/* to base pointer payloads off RBP.  Default DESCR_LAYOUT_16 == today.     */
typedef enum {
    DESCR_LAYOUT_16 = 0,   /* current: 4(v) + 4(slen) + 8(payload) = 16, raw 64-bit pointers */
    DESCR_LAYOUT_8  = 1,    /* packed:  1(v) + 3(slen) + 4(off) = 8, RBP-based 32-bit payload */
} descr_layout_t;
/* Symbolic field offsets / stride for the CURRENT (16-byte) C struct.     */
/* These come straight from the struct so the C side can never drift.      */
#include <stddef.h>
#define DESCR_OFF_V        ((int)offsetof(DESCR_t, v))      /* = 0  */
#define DESCR_OFF_SLEN     ((int)offsetof(DESCR_t, slen))   /* = 4  */
#define DESCR_OFF_PAYLOAD  ((int)offsetof(DESCR_t, s))      /* = 8  */
#define DESCR_STRIDE       ((int)sizeof(DESCR_t))           /* = 16 */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* codebase routes through these GET_ and SET_ accessors plus the sentinel */
/* and constructor macros below.  In the current 16-byte layout each       */
/* expands to the identical field access it replaces, so the build stays   */
/* byte-for-byte behavior-identical.  When the 8-byte layout lands, only    */
/* these definitions change; no call site moves.  Lives in descr.h so it   */
/* is in scope wherever DESCR_t is (descr.h is pulled in via core.h).       */
#define GET_V(d)        ((d).v)
#define SET_V(d, val)   ((d).v = (DTYPE_t)(val))
#define GET_SLEN(d)     ((d).slen)
#define SET_SLEN(d, n)  ((d).slen = (uint32_t)(n))
#define GET_I(d)        ((d).i)
#define SET_I(d, val)   ((d).i = (int64_t)(val))
#define GET_R(d)        ((d).r)
#define SET_R(d, val)   ((d).r = (double)(val))
#define GET_S(d)        ((d).s)
#define SET_S(d, val)   ((d).s = (char *)(val))
#define GET_PTR(d)      ((d).ptr)
#define SET_PTR(d, val) ((d).ptr = (void *)(val))
#define GET_P(d)        ((d).p)
#define SET_P(d, val)   ((d).p = (struct _PATND_t *)(val))
#define GET_ARR(d)      ((d).arr)
#define SET_ARR(d, val) ((d).arr = (struct _ARBLK_t *)(val))
#define GET_TBL(d)      ((d).tbl)
#define SET_TBL(d, val) ((d).tbl = (struct _TBBLK_t *)(val))
#define GET_U(d)        ((d).u)
#define SET_U(d, val)   ((d).u = (struct _DATINST_t *)(val))
/* CSET sentinel — currently slen == 0xFFFFFFFF on a DT_S string.          */
#define CSET_SENTINEL   (0xFFFFFFFFu)
#define IS_CSET(d)      ((d).v == DT_S && (d).slen == CSET_SENTINEL)
#define MK_CSET_SLEN(d) ((d).slen = CSET_SENTINEL)
/* Constructors for the field-by-field DT_DATA / DT_T / DT_A builds.       */
#define MK_DATA(ptr_)   ((DESCR_t){ .v = DT_DATA, .slen = 0, .ptr = (void *)(ptr_) })
#define MK_TBL(tbl_)    ((DESCR_t){ .v = DT_T,    .slen = 0, .tbl = (tbl_) })
#define MK_ARR(arr_)    ((DESCR_t){ .v = DT_A,    .slen = 0, .arr = (arr_) })
/* Name discriminators — a DT_N descriptor is a name-by-PTR (slen == 1,    */
/* payload is a DESCR_t*) or a name-by-VAL (slen == 0, payload is the      */
/* variable-name string).  Mirrored here from sil_macros.h so they are in  */
/* scope across the whole runtime (descr.h is pulled in via core.h), same  */
/* rationale as the GET_/SET_ accessors above.  When the 8-byte layout     */
/* lands only these expansions change.                                     */
#ifndef IS_NAMEPTR
#define IS_NAMEPTR(d)  ((d).v == DT_N && (d).slen == 1 && (d).ptr)
#endif
#ifndef IS_NAMEVAL
#define IS_NAMEVAL(d)  ((d).v == DT_N && (d).slen == 0 && (d).s)
#endif
#ifndef NAME_DEREF_PTR
#define NAME_DEREF_PTR(d)  (*(DESCR_t *)(d).ptr)
#endif
#define MK_NAMEPTR(dp_)  ((DESCR_t){ .v = DT_N, .slen = 1, .ptr = (void *)(dp_) })
#define MK_NAMEVAL(s_)   ((DESCR_t){ .v = DT_N, .slen = 0, .s = (char *)(s_) })
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define FAILDESCR    ((DESCR_t){ .v = DT_FAIL, .i = 0 })
static inline int IS_FAIL_fn(DESCR_t v) { return v.v == DT_FAIL; }
#define FHVAL(idx_) ((DESCR_t){ .v = DT_FH, .i = (int64_t)(idx_) })
static inline int IS_FH_fn(DESCR_t v) { return v.v == DT_FH; }
#endif
