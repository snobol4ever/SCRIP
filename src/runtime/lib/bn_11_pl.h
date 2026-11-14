#ifndef RUNTIME_LIB_BN_11_PL_H
#define RUNTIME_LIB_BN_11_PL_H
#include "runtime/lib/bn_head.h"
#include <sys/un.h>
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
#include "runtime/rt/rt_pl_trail_pl_tr_ctx_s.h"
int junction_mirror_op(int op);
__attribute__((visibility("hidden"))) void pas_heap_release(long m);
__attribute__((visibility("hidden"))) int pl_alias_idx(const char *nm);
__attribute__((visibility("hidden"))) DESCR_t pl_mk_stream(int idx);
__attribute__((visibility("hidden"))) int pl_stream_resolve(DESCR_t s, int out, int textop, void **ball);
__attribute__((visibility("hidden"))) int pl_in_is_text(void);
DESCR_t rt_pl_dop_get_char_c(DESCR_t *, int, pl_tr_ctx_t *);
DESCR_t rt_pl_dop_peek_char_c(DESCR_t *, int, pl_tr_ctx_t *);
DESCR_t rt_pl_dop_get_code_c(DESCR_t *, int, pl_tr_ctx_t *);
DESCR_t rt_pl_dop_peek_code_c(DESCR_t *, int, pl_tr_ctx_t *);
__attribute__((visibility("hidden"))) void *pl_byte_cur_ball(int out);
__attribute__((visibility("hidden"))) void *pl_text_cur_ball(void);
DESCR_t rt_pl_dop_get_byte_c(DESCR_t *, int, pl_tr_ctx_t *);
DESCR_t rt_pl_dop_peek_byte_c(DESCR_t *, int, pl_tr_ctx_t *);
DESCR_t rt_pl_dop_unget_char_c(DESCR_t *, int, pl_tr_ctx_t *);
DESCR_t rt_pl_dop_unget_code_c(DESCR_t *, int, pl_tr_ctx_t *);
DESCR_t rt_pl_dop_unget_byte_c(DESCR_t *, int, pl_tr_ctx_t *);
__attribute__((visibility("hidden"))) int pl_read_esc_body(FILE *in, char **bp, size_t *np, size_t *kp);
__attribute__((visibility("hidden"))) DESCR_t pl_gs_arg(DESCR_t t, int i, int n);
__attribute__((visibility("hidden"))) DESCR_t pl_gs_fdstream(int fd, const char *m);
__attribute__((visibility("hidden"))) int pl_gs_unix_addr(struct sockaddr_un *u, const char *p);
__attribute__((visibility("hidden"))) int pl_gs_sock(const char *op, DESCR_t a, DESCR_t res, pl_tr_ctx_t *cx);
__attribute__((visibility("hidden"))) int pl_alarm_take(pl_tr_ctx_t *cx);
__attribute__((visibility("hidden"))) int bn_array(DESCR_t *args, int nargs, DESCR_t *out, int op);
int rt_dat_field_of_any_live(const char *name);
const char *rt_builtin_synonym(const char *name);
DESCR_t sno_array_d(DESCR_t a, DESCR_t init);
#endif
