#ifndef RUNTIME_UNIFICATION_UN_05_PL_H
#define RUNTIME_UNIFICATION_UN_05_PL_H
#include "runtime/unification/un_head.h"
__attribute__((visibility("hidden"))) int plc_op_info_id(int nid, int arity, int *prec, int *lmax, int *rmax);
__attribute__((visibility("hidden"))) int plc_op_info(const char *name, int arity, int *prec, int *lmax, int *rmax);
__attribute__((visibility("hidden"))) int plc_op_is_postfix(const char *name);
__attribute__((visibility("hidden"))) int plc_is_nil(pl_cell_t *d);
__attribute__((visibility("hidden"))) int plc_atom_needs_quoting(const char *name);
__attribute__((visibility("hidden"))) void plc_wt_num(FILE *fp, pl_cell_t *d);
__attribute__((visibility("hidden"))) int plc_term_prio(pl_cell_t *c, int ignore_ops);
__attribute__((visibility("hidden"))) int plc_first_char_d(pl_cell_t *c, int quoted, int ignore_ops, int numbervars, long dl);
__attribute__((visibility("hidden"))) int plc_last_char_d(pl_cell_t *c, int quoted, int ignore_ops, int numbervars, long dl);
__attribute__((visibility("hidden"))) long plc_ws_mark(const pl_tr_ctx_t *cx);
__attribute__((visibility("hidden"))) void plc_ws_end(const pl_tr_ctx_t *cx, long mark);
__attribute__((visibility("hidden"))) void plc_wsref_set(const pl_tr_ctx_t *cx, plc_wsref *r, pl_cell_t *c);
__attribute__((visibility("hidden"))) pl_cell_t *plc_wsref_get(const pl_tr_ctx_t *cx, plc_wsref *r);
__attribute__((visibility("hidden"))) void plc_fb_u8(plc_fb *f, unsigned cp);
void *rt_pl_format_run(const char *fmt, void *list_cell, pl_tr_ctx_t *cx);
#endif
