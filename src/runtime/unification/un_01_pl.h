#ifndef RUNTIME_UNIFICATION_UN_01_PL_H
#define RUNTIME_UNIFICATION_UN_01_PL_H
#include "runtime/unification/un_head.h"
#include "runtime/rt/rt_pl_trail_pl_tr_ctx_s.h"
__attribute__((visibility("hidden"))) const char *plc_atom_text(pl_cell_t *d);
__attribute__((visibility("hidden"))) const char *plc_vname(plc_vmap *m, pl_cell_t *d);
__attribute__((visibility("hidden"))) int plc_vindex(plc_vmap *m, pl_cell_t *d);
__attribute__((visibility("hidden"))) size_t plc_atom_len(pl_cell_t *d);
int rt_pl_u8_get(const char *s, int *adv);
int rt_pl_u8_get(const char *s, int *adv);
__attribute__((visibility("hidden"))) int plc_opt_is_true(pl_cell_t *o);
__attribute__((visibility("hidden"))) const char *plc_atom_op_text(pl_cell_t *t, char *buf, size_t bufsz);
__attribute__((visibility("hidden"))) pl_cell_t plc_make_atom_cell(const char *name);
__attribute__((visibility("hidden"))) int plc_text_out(void *cell, const char *s, int as_str, pl_tr_ctx_t *cx);
__attribute__((visibility("hidden"))) size_t pl_u8_count_n(const char *s, size_t len);
int rt_pl_u8_put(char *o, int cp);
__attribute__((visibility("hidden"))) int plc_fmt_is_atom(pl_cell_t *d);
#endif
