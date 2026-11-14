#ifndef RUNTIME_LIB_BN_27_PL_H
#define RUNTIME_LIB_BN_27_PL_H
#include "runtime/lib/bn_head.h"
#include "runtime/lib/bn_head_pl_flag.h"
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
#include "runtime/rt/rt_pl_trail_pl_tr_ctx_s.h"
const char * pl_real_iso_str(double fv, char * fb, int bufsz);
__attribute__((visibility("hidden"))) int pl_ax_eval(DESCR_t t, DESCR_t *out, void **ball);
__attribute__((visibility("hidden"))) int pl_ax_eval_d(DESCR_t t, DESCR_t *out, void **ball, int depth);
__attribute__((visibility("hidden"))) void *pl_ax_int_ball(DESCR_t a, DESCR_t b, int ai);
__attribute__((visibility("hidden"))) int pl_float_flag(const char *nm, const char *val);
__attribute__((visibility("hidden"))) int pl_swi_dialect(void);
__attribute__((visibility("hidden"))) int pl_gnu_dialect(void);
__attribute__((visibility("hidden"))) int pl_xsb_dialect(void);
__attribute__((visibility("hidden"))) int pl_ax_undef(const char *fn, int arity, DESCR_t *out, void **ball);
__attribute__((visibility("hidden"))) int pl_ax_zdiv(double num, double den, int fdiv, const char *fn, int arity, DESCR_t *out, void **ball);
__attribute__((visibility("hidden"))) int pl_ax_float_result(double r, double x, double y, const char *fn, int arity, DESCR_t *out, void **ball);
__attribute__((visibility("hidden"))) double pl_big_as_real(DESCR_t d);
__attribute__((visibility("hidden"))) int pl_big_unop(const char *op, DESCR_t a, DESCR_t *out);
__attribute__((visibility("hidden"))) int pl_big_binop(const char *op, DESCR_t a, DESCR_t b, DESCR_t *out, void **ball);
__attribute__((visibility("hidden"))) int pl_ax_shift(const char *op, DESCR_t a, DESCR_t b, DESCR_t *out, void **ball);
__attribute__((visibility("hidden"))) int pl_ax_powm(DESCR_t *args, DESCR_t *out, void **ball);
__attribute__((visibility("hidden"))) int pl_rat_done(int qe, const char *op, int arity, DESCR_t culprit, DESCR_t *out, void **ball);
__attribute__((visibility("hidden"))) int pl_prefer_rationals(void);
__attribute__((visibility("hidden"))) int pl_q_odd(DESCR_t q);
__attribute__((visibility("hidden"))) int dop_ax(const char *op, DESCR_t *args, int nargs, DESCR_t *out, void **ball);
__attribute__((visibility("hidden"))) const char *pl_sp_mode_name(int i);
__attribute__((visibility("hidden"))) const char *pl_flag_encoding(void);
__attribute__((visibility("hidden"))) int pl_swi_string_zero_quote(const char *t);
__attribute__((visibility("hidden"))) int pl_tok_is_graphic(int c);
__attribute__((visibility("hidden"))) int pl_read_syntax_mode(DESCR_t opts);
__attribute__((visibility("hidden"))) int pl_read_term_from_text(DESCR_t *args, pl_tr_ctx_t *cx, int atom_text);
__attribute__((visibility("hidden"))) pl_flag_t * pl_flag_find(const char *nm);
int rt_pl_dialect_swi(void);
int rt_pl_dialect_xsb(void);
int rt_pl_iso_mode(void);
int rt_pl_double_quotes_mode(void);
__attribute__((visibility("hidden"))) char *pl_gnu_cwd(void);
__attribute__((visibility("hidden"))) char *pl_gnu_abs_path(const char *src);
__attribute__((visibility("hidden"))) int pl_seed_src(void *db, DESCR_t *fl, int ok);
#endif
