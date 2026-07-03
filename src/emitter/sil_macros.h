#ifndef SIL_MACROS_H
#define SIL_MACROS_H
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "core.h"
#define MOVD(dst, src)         ((dst) = (src))
#define MOVV(dst, src)         ((dst).v = (src).v)
#define MOVA(dst, src)         ((dst).ptr = (src).ptr)
#define SETAC(d, val)          ((d).ptr = (void *)(intptr_t)(val))
#define SETAV(d, src)          ((d).ptr = (void *)(intptr_t)(src).v)
#define GETDC(d, base, off) \
    ((d) = *((DESCR_t *)((char *)(base) + (off))))
#define PUTDC(base, off, d) \
    (*((DESCR_t *)((char *)(base) + (off))) = (d))
#define MOVBLK(dst, src, sz)   (memmove((void *)(dst), (void *)(src), (size_t)(sz)))
#define IS_FNC(d)              (0)
#define VEQLC(d, T)            ((d).v == (T))
#define VEQL(a, b)             ((a).v == (b).v)
#define DEQL(a, b)             ((a).v == (b).v && (a).ptr == (b).ptr)
#define AEQLC(d, val)          ((intptr_t)(d).ptr == (intptr_t)(val))
#define AEQL(a, b)             ((a).ptr == (b).ptr)
#define IS_INT(d)    ((d).v == DT_I)
#define IS_REAL(d)   ((d).v == DT_R)
#define IS_STR(d)    ((d).v == DT_S || (d).v == DT_SNUL)
#define IS_PAT(d)    ((d).v == DT_P)
#define IS_NAME(d)   ((d).v == DT_N)
#define IS_KW(d)     ((d).v == DT_K)
#define IS_EXPR(d)   ((d).v == DT_E)
#define IS_CODE(d)   ((d).v == DT_C)
#define IS_ARR(d)    ((d).v == DT_A)
#define IS_TBL(d)    ((d).v == DT_T)
#define IS_DATA(d)   ((d).v == DT_DATA)
#define IS_FAIL(d)   ((d).v == DT_FAIL)
#define IS_NULL(d)   ((d).v == DT_SNUL)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int ACOMP(DESCR_t a, DESCR_t b) {
    return (a.i > b.i) - (a.i < b.i);
}
#define ACOMPC(d, val) \
    (((d).i > (int64_t)(val)) - ((d).i < (int64_t)(val)))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int RCOMP(DESCR_t a, DESCR_t b) {
    return (a.r > b.r) - (a.r < b.r);
}
#define PCOMP(d, val) \
    (((uintptr_t)(d).ptr > (uintptr_t)(val)) - \
     ((uintptr_t)(d).ptr < (uintptr_t)(val)))
#define INCRA(d, n)   ((d).i += (int64_t)(n))
#define DECRA(d, n)   ((d).i -= (int64_t)(n))
#define ADDLG(d, s, slen)   ((d).i += (int64_t)(slen))
DESCR_t INTRL_fn(DESCR_t d);
DESCR_t RLINT_fn(DESCR_t d);
DESCR_t NEG_I_fn(DESCR_t d);
DESCR_t NEG_R_fn(DESCR_t d);
DESCR_t EXP_R_fn(DESCR_t base, DESCR_t exp);
#define LOCSP(ptr_out, len_out, d) \
    do { \
        (ptr_out) = (d).s; \
        (len_out) = descr_slen(d); \
    } while (0)
#define GETLG(d)   ((int64_t)descr_slen(d))
#define LEQLC(d, n)   (descr_slen(d) == (size_t)(n))
DESCR_t TRIM_fn(DESCR_t d);
int SPCINT_fn(DESCR_t *out, const char *s, size_t len);
int SPREAL_fn(DESCR_t *out, const char *s, size_t len);
DESCR_t REALST_fn(DESCR_t d);
DESCR_t INTSP_fn(DESCR_t d);
int LCOMP_fn(const char *s1, size_t l1, const char *s2, size_t l2);
#define LEXEQ(s1, l1, s2, l2) \
    ((l1) == (l2) && ((l1) == 0 || memcmp((s1), (s2), (l1)) == 0))
void state_push(void);
void state_pop(void);
DESCR_t NAME_fn(const char *varname);
int     ASGNIC_fn(const char *kw, DESCR_t v);
DESCR_t EXPVAL_fn(DESCR_t expr_d);
DESCR_t EXPEVL_fn(DESCR_t expr_d);
DESCR_t CONVE_fn(DESCR_t str_d);
#define IS_NAMEPTR(d)  ((d).v == DT_N && (d).slen == 1 && (d).ptr)
#define IS_NAMEVAL(d)  ((d).v == DT_N && (d).slen == 0 && (d).s)
#define NAME_DEREF_PTR(d)  (*(DESCR_t *)(d).ptr)
#define NAME_DEREF_VAL(d, nv_get)  ((nv_get)((d).s))
#endif
