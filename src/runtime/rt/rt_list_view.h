#ifndef RT_LIST_VIEW_H
#define RT_LIST_VIEW_H
#include "core.h"
#include <string.h>
int pm_lf_type_is(DATBLK_t *t);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline DESCR_t *rt_lv_nil_elems(void) { static DESCR_t nil[1]; return nil; }
int rt_lv_is_list(DESCR_t o, DESCR_t **elems, int *n);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline void rt_lv_set_size(DESCR_t o, int n) { o.u->fields[1] = INTVAL(n); }
static inline void rt_lv_set_elems(DESCR_t o, DESCR_t ea) { o.u->fields[0] = ea; }
static inline long rt_lv_cap(DESCR_t o) { if (o.u->type->nfields < 4) return -1; DESCR_t c = o.u->fields[3]; return (c.v == DT_I) ? (long)c.i : -1; }
static inline void rt_lv_set_cap(DESCR_t o, long c) { if (o.u->type->nfields >= 4) o.u->fields[3] = INTVAL(c); }
static inline __attribute__((always_inline)) DESCR_t *rt_lv_blocks(DESCR_t o) {
    if (o.u->type->nfields < 5 || !IS_DATA_ELEMS_fn(o.u->fields[4])) return (DESCR_t *)0;
    return (DESCR_t *)o.u->fields[4].ptr;
}
void rt_lvb_push(DESCR_t o, int s);
void rt_lvb_put(DESCR_t o, int s);
void rt_lvb_get(DESCR_t o, int s);
void rt_lvb_pull(DESCR_t o, int s);
long rt_lvb_cursor(DESCR_t o, int64_t *cur, int n);
#endif
