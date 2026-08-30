#ifndef SIL_MACROS_H
#define SIL_MACROS_H
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "core.h"
#define IS_INT(d)    ((d).v == DT_I)
#define IS_REAL(d)   ((d).v == DT_R)
#define IS_STR(d)    ((d).v == DT_S || (d).v == DT_SNUL)
#define IS_PAT(d)    ((d).v == DT_P)
#define IS_NAME(d)   ((d).v == DT_N)
#define IS_KW(d)     ((d).v == DT_K)
#define IS_ARR(d)    ((d).v == DT_A)
#define IS_TBL(d)    ((d).v == DT_T)
#define IS_DATA(d)   ((d).v == DT_DATA)
#define IS_FAIL(d)   ((d).v == DT_FAIL)
#define IS_NULL(d)   ((d).v == DT_SNUL)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int ACOMP(DESCR_t a, DESCR_t b) {
    return (a.i > b.i) - (a.i < b.i);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int RCOMP(DESCR_t a, DESCR_t b) {
    return (a.r > b.r) - (a.r < b.r);
}
DESCR_t INTRL_fn(DESCR_t d);
DESCR_t RLINT_fn(DESCR_t d);
DESCR_t NEG_I_fn(DESCR_t d);
DESCR_t NEG_R_fn(DESCR_t d);
DESCR_t EXP_R_fn(DESCR_t base, DESCR_t exp);
DESCR_t TRIM_fn(DESCR_t d);
int SPCINT_fn(DESCR_t *out, const char *s, size_t len);
int SPREAL_fn(DESCR_t *out, const char *s, size_t len);
DESCR_t REALST_fn(DESCR_t d);
DESCR_t INTSP_fn(DESCR_t d);
int LCOMP_fn(const char *s1, size_t l1, const char *s2, size_t l2);
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
#endif
