#ifndef DESCR_H
#define DESCR_H
#include <stdint.h>
#include <stddef.h>
#include "ir/descr_tags_dt_platom.h"
#ifdef __cplusplus
extern "C" {
#endif
struct _ARBLK_t;
struct _TBBLK_t;
struct _DATINST_t;
typedef struct DESCR_t {
    uint8_t v;
    uint8_t src_node0, src_node1, src_node2;
    uint32_t slen;
    union { char *s; int64_t i; double r; void *p; struct _ARBLK_t *arr; struct _TBBLK_t *tbl; struct _DATINST_t *u; void *ptr; };
} DESCR_t;
#define DESCR_SRC_NODE_UNSTAMPED 0u
#define DESCR_SRC_NODE_OVERFLOW 0xFFFFFFu
static inline __attribute__((always_inline)) uint32_t descr_src_node(DESCR_t d) { return (uint32_t)d.src_node0 | ((uint32_t)d.src_node1 << 8) | ((uint32_t)d.src_node2 << 16); }
static inline __attribute__((always_inline)) void descr_set_src_node(DESCR_t *d, uint32_t id) {
    if (id > DESCR_SRC_NODE_OVERFLOW) id = DESCR_SRC_NODE_OVERFLOW;
    d->src_node0 = (uint8_t)id;
    d->src_node1 = (uint8_t)(id >> 8);
    d->src_node2 = (uint8_t)(id >> 16);
}
#define FAILDESCR ((DESCR_t){ .v = DT_FAIL, .i = 0 })
#define NAMETRAP(vc_) ((DESCR_t){ .v = DT_N, .slen = 2, .p = (void *)(vc_) })
#define ICN_FIELD_PLAIN 0x10000
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline __attribute__((always_inline)) int IS_FAIL_fn(DESCR_t v) { return v.v == DT_FAIL; }
static inline __attribute__((always_inline)) int IS_DATA_TAG_fn(uint8_t v) { return v >= DT_DATA && v < DT_RAW; }
static inline __attribute__((always_inline)) int IS_NAMETRAP_fn(DESCR_t v) { return v.v == DT_N && v.slen == 2; }
static inline __attribute__((always_inline)) int IS_VARREF_fn(DESCR_t v) { return v.v == DT_N && (v.slen == 2 || (v.slen == 1 && v.ptr) || (v.slen == 0 && v.s && *v.s)); }
static inline __attribute__((always_inline)) int IS_KWNAME_fn(DESCR_t v) { return v.v == DT_N && v.slen == 0 && v.s && v.s[0] == '&' && v.s[1] == '&'; }
#define DATA_INST_SLEN 0u
#define DATA_ELEMS_SLEN 1u
#define DATA_RVEC_I_SLEN 2u
#define DATA_RVEC_R_SLEN 3u
static inline __attribute__((always_inline)) int IS_DATA_INST_fn(DESCR_t v) { return v.v == DT_DATA && v.slen == DATA_INST_SLEN; }
static inline __attribute__((always_inline)) int IS_DATA_ELEMS_fn(DESCR_t v) { return v.v == DT_DATA && v.slen == DATA_ELEMS_SLEN; }
#define DATA_ELEMS(p_) ((DESCR_t){ .v = DT_DATA, .slen = DATA_ELEMS_SLEN, .ptr = (void *)(p_) })
#define FHVAL(idx_) ((DESCR_t){ .v = DT_FH, .i = (int64_t)(idx_) })
#define COERCE_ERR_FAILURE_CONVERTIBLE 0x1000000L
#define COERCE_KEEP_INT 0x2000000L
#define COERCE_OP_SHIFT 32
#define COERCE_OP_TAG(bcode) (((long)((bcode) + 1)) << COERCE_OP_SHIFT)
#define COERCE_OP_OF(codes) ((int)(((codes) >> COERCE_OP_SHIFT) & 0xffff) - 1)
#define COERCE_OP_SELF_RIGHT (1L << 48)
#define COERCE_OP_UNARY_NEG (1L << 49)
#define COERCE_OP_UNARY_POS (1L << 50)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline __attribute__((always_inline)) int IS_FH_fn(DESCR_t v) { return v.v == DT_FH; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define PROCVAL_SLEN 0xFFFFFFFEu
#define PROCVAL_BUILTIN_SLEN 0xFFFFFFFCu
#define PROCVAL_EXTERNAL_SLEN 0xFFFFFFFBu
typedef struct _EXTFN_t { void *fn; char name[]; } EXTFN_t;
typedef struct _CVSPINE_t { long fn; long act0; } CVSPINE_t;
static inline __attribute__((always_inline)) int IS_PROCVAL_EXTERNAL_fn(DESCR_t v) { return v.v == DT_E && v.slen == PROCVAL_EXTERNAL_SLEN; }
#define PROCVAL_EXTERNAL(blk_) ((DESCR_t){ .v = DT_E, .slen = PROCVAL_EXTERNAL_SLEN, .s = (blk_)->name })
#define PROCVAL_EXT_FN(v_) (((EXTFN_t *)(void *)((char *)(v_).s - offsetof(EXTFN_t, name)))->fn)
static inline __attribute__((always_inline)) int IS_PROCVAL_fn(DESCR_t v) { return v.v == DT_E && (v.slen == PROCVAL_SLEN || v.slen == PROCVAL_BUILTIN_SLEN || v.slen == PROCVAL_EXTERNAL_SLEN); }
static inline __attribute__((always_inline)) int IS_PROCVAL_BUILTIN_fn(DESCR_t v) { return v.v == DT_E && v.slen == PROCVAL_BUILTIN_SLEN; }
#define PROCVAL_BUILTIN(name_) ((DESCR_t){ .v = DT_E, .slen = PROCVAL_BUILTIN_SLEN, .s = (char *)(name_) })
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline __attribute__((always_inline)) uint32_t descr_cstrlen(const char *s_) { return s_ ? (uint32_t)__builtin_strlen(s_) : 0u; }
const char *kw_cset_empty(void);
#define CSET_BLK_HDR 40
#define CSET_BLK_LEN_OFF (-40)
#define CSET_BLK_MAGIC_OFF (-36)
#define CSET_BLK_BITS_OFF (-32)
#define CSET_BLK_MAGIC 0xC5E7B175u
static inline __attribute__((always_inline)) int cset_blk_len(const char *p) { int32_t n; __builtin_memcpy(&n, p + CSET_BLK_LEN_OFF, 4); return (int)n; }
static inline __attribute__((always_inline)) const unsigned char *cset_blk_bits(const char *p) { return (const unsigned char *)(p + CSET_BLK_BITS_OFF); }
static inline __attribute__((always_inline)) size_t descr_slen(DESCR_t d) {
    if (d.v == DT_S) { if (d.slen == 0xFFFFFFFFu) return d.s ? (size_t)cset_blk_len(d.s) : 0; return (size_t)d.slen; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rt_cstr_materialize(DESCR_t d);
static inline __attribute__((always_inline)) const char *rt_cstr_d(DESCR_t d) {
    if (d.v != DT_S || !d.s || !d.slen || d.slen == 0xFFFFFFFFu) return d.s ? d.s : "";
    return d.s[d.slen] ? rt_cstr_materialize(d) : d.s;
}
#define NULVCL ((DESCR_t){ .v = DT_SNUL, .slen = 0, .s = "" })
#define STRVAL(s_) __extension__({ char *_sv_ = (char *)(s_); (DESCR_t){ .v = DT_S, .slen = descr_cstrlen(_sv_), .s = _sv_ }; })
#define BSTRVAL(s_, len_) ((DESCR_t){ .v = DT_S, .slen = (uint32_t)(len_), .s = (s_) })
#define INTVAL(i_) ((DESCR_t){ .v = DT_I, .i = (i_) })
#define REALVAL(r_)((DESCR_t){ .v = DT_R, .r = (r_) })
#define CSETVAL(s_) ((DESCR_t){ .v = DT_S, .slen = 0xFFFFFFFFu, .s = (s_) })
#define NAMEPTR(dp_) ((DESCR_t){ .v = DT_N, .slen = 1, .ptr = (void*)(dp_) })
static inline __attribute__((always_inline)) DESCR_t NAME_AS_STR_fn(DESCR_t d) { return (d.v == DT_N && d.slen == 0 && d.s && !IS_KWNAME_fn(d)) ? STRVAL(d.s) : d; }
#define NAMEVAL(s_) ((DESCR_t){ .v = DT_N, .slen = 0, .s = (char *)(s_) })
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline __attribute__((always_inline)) int IS_NULL_fn(DESCR_t v) { return v.v == DT_SNUL || (v.v == DT_S && v.slen == 0 && (!v.s || !*v.s)); }
static inline __attribute__((always_inline)) int IS_STR_fn(DESCR_t v) { return v.v == DT_S || v.v == DT_SNUL; }
static inline __attribute__((always_inline)) int IS_INT_fn(DESCR_t v) { return v.v == DT_I; }
static inline __attribute__((always_inline)) int IS_REAL_fn(DESCR_t v) { return v.v == DT_R; }
static inline __attribute__((always_inline)) int IS_CSET_fn(DESCR_t v) { return v.v == DT_S && v.slen == 0xFFFFFFFFu; }
static inline __attribute__((always_inline)) int IS_DATA_fn(DESCR_t v) { return v.v == DT_DATA; }
#ifdef __cplusplus
}
#endif
#endif
