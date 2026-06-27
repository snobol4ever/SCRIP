/*--------------------------------------------------------------------------------------------------------------------*/
#ifndef ARITH_FOLD_H
#define ARITH_FOLD_H
#include "IR.h"
int gz_arith_const_eval(const IR_t *nd, long *out);
int sno_arith_lit_coerce(const IR_t *nd, long *out);
int gz_arith_float_eval(const IR_t *nd, double *out);
int gz_arith_var_plus_const(const IR_t *nd, int *var_slot, const char **op_out, long *c_out);
int gz_arith_var_bivar(const IR_t *nd, int *slot1, int *slot2, const char **op_out);
#endif
