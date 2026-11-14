#ifndef RUNTIME_UNIFICATION_UN_04_PL_H
#define RUNTIME_UNIFICATION_UN_04_PL_H
#include "runtime/unification/un_head.h"
#include "runtime/rt/rt_pl_trail_pl_tr_ctx_s.h"
__attribute__((visibility("hidden"))) FILE *plc_out(void);
__attribute__((visibility("hidden"))) int plc_is_op_atom(pl_cell_t *d);
__attribute__((visibility("hidden"))) void plc_wt(pl_cell_t *c, int quoted, int ignore_ops, int numbervars, long max_depth, long depth, int maxp, plc_vmap *m);
__attribute__((visibility("hidden"))) pl_cell_t plc_factorize_x(pl_cell_t *c, int named, int dag);
__attribute__((visibility("hidden"))) void plc_writeq(pl_cell_t *c, plc_vmap *m);
__attribute__((visibility("hidden"))) void plc_atoms_ready(void);
void rt_pl_write_cell_fp(void *cell, FILE *fp);
__attribute__((visibility("hidden"))) pl_cell_t *plc_fmt_next_arg(pl_cell_t **args);
__attribute__((visibility("hidden"))) void plc_fb_raw(plc_fb *f, const char *s, size_t n);
__attribute__((visibility("hidden"))) void plc_fb_add(plc_fb *f, const char *s, size_t n);
__attribute__((visibility("hidden"))) size_t plc_fb_colat(plc_fb *f, size_t at);
__attribute__((visibility("hidden"))) void plc_fb_stop(plc_fb *f, long target);
__attribute__((visibility("hidden"))) void *plc_fb_term(plc_fb *f, pl_cell_t *t, int kind, int quoted, int ignore_ops);
__attribute__((visibility("hidden"))) pl_cell_t plc_copy(pl_cell_t *c);
__attribute__((visibility("hidden"))) pl_cell_t plc_copy_cx(pl_cell_t *c, int atts, pl_tr_ctx_t *cx);
__attribute__((visibility("hidden"))) pl_cell_t plc_copy_x(pl_cell_t *c, int atts);
pl_cell_t rt_pl_cell_snapshot(void *cell);
#endif
