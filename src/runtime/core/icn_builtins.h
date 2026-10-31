#ifndef ICN_BUILTINS_H
#define ICN_BUILTINS_H
#include <stdint.h>
#include <stdio.h>
#include "ir/descr.h"
#ifdef __cplusplus
extern "C" {
#endif
int64_t core_icn_to_int_d(DESCR_t d);
int core_icn_int_operand_ok_d(DESCR_t d);
int core_icn_num_operand_ok_d(DESCR_t d, int code);
typedef struct icn_bi_rec { const char *name; DESCR_t *args; int nargs; int level; struct icn_bi_rec *prev; DESCR_t callee; } icn_bi_rec_t;
void core_icn_op_ctx(const char *sym, int arity, DESCR_t a, DESCR_t b);
void core_icn_toby_ctx(const DESCR_t *from, const DESCR_t *to, const DESCR_t *by);
int core_icn_by_zero_at(const DESCR_t *from, const DESCR_t *to, const DESCR_t *by);
void core_icn_op_ctx_clear(void);
const char *core_icn_binop_sym(int bcode);
void core_icn_bi_push(icn_bi_rec_t *r, const char *name, DESCR_t *args, int nargs);
void core_icn_bi_pop(icn_bi_rec_t *r);
void *core_icn_bi_mark(void);
void core_icn_bi_reset(void *mark);
void core_icn_traceback(void);
void core_icn_display_image(FILE *fp, DESCR_t v);
int core_icn_act_np(int lv);
DESCR_t *core_icn_act_arg(int lv, int k);
int core_icn_builtin_argcheck(const char *fn, DESCR_t *args, int nargs, int strict);
#ifdef __cplusplus
}
#endif
#endif
