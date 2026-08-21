#ifndef RT_LIST_VIEW_H
#define RT_LIST_VIEW_H
#include "core.h"
#include <string.h>
static DATBLK_t *rt_lv_type_cache;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int rt_lv_is_list(DESCR_t o, DESCR_t **elems, int *n) {
    if (o.v != DT_DATA || !o.u) return 0;
    DATBLK_t *t = o.u->type;
    if (t != rt_lv_type_cache) { if (!t || t->nfields < 3 || !t->fields[0] || strcmp(t->fields[0], "frame_elems") != 0) return 0; rt_lv_type_cache = t; }
    DESCR_t gt = o.u->fields[2]; if (gt.v != DT_S || !gt.s || strcmp(gt.s, "list") != 0) return 0;
    DESCR_t ea = o.u->fields[0]; if (elems) *elems = (ea.v == DT_DATA) ? (DESCR_t *)ea.ptr : (DESCR_t *)0; if (n) *n = (int)o.u->fields[1].i;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline void rt_lv_set_size(DESCR_t o, int n) { o.u->fields[1] = INTVAL(n); }
static inline void rt_lv_set_elems(DESCR_t o, DESCR_t ea) { o.u->fields[0] = ea; }
static inline long rt_lv_cap(DESCR_t o) { if (o.u->type->nfields < 4) return -1; DESCR_t c = o.u->fields[3]; return (c.v == DT_I) ? (long)c.i : -1; }
static inline void rt_lv_set_cap(DESCR_t o, long c) { if (o.u->type->nfields >= 4) o.u->fields[3] = INTVAL(c); }
#endif
