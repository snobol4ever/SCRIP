#ifndef RUNTIME_LIB_BN_29_PL_H
#define RUNTIME_LIB_BN_29_PL_H
#include "runtime/lib/bn_head.h"
DESCR_t dop_write(DESCR_t *args, int nargs);
DESCR_t dop_pl_writeln(DESCR_t *args, int nargs);
DESCR_t dop_pl_tab(DESCR_t *args, int nargs);
DESCR_t dop_pl_put_char(DESCR_t *args, int nargs);
DESCR_t dop_pl_put_code(DESCR_t *args, int nargs);
DESCR_t rt_pl_dop_format_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx);
__attribute__((visibility("hidden"))) int pl_stream_idx(DESCR_t s, int out);
__attribute__((visibility("hidden"))) int rt_pl_unifiable_cell(DESCR_t *ax, DESCR_t *ay, DESCR_t *au, pl_tr_ctx_t *cx);
__attribute__((visibility("hidden"))) int pl_write_opts_check(DESCR_t optlist, DESCR_t *culprit);
DESCR_t rt_pl_dop_write_term_c(DESCR_t *, int, pl_tr_ctx_t *);
__attribute__((visibility("hidden"))) int pl_gnu_position(DESCR_t s, long *ch, long *ln, long *lp, pl_tr_ctx_t *cx, const char *who);
#endif
