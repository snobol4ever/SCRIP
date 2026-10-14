#ifndef PL_RAT_H
#define PL_RAT_H
#include <stdio.h>
#include "descr.h"
enum { PLQ_ZDIV = 1, PLQ_UNDEF = 2, PLQ_TYPE_INT = 3, PLQ_TYPE_RAT = 4 };
int pl_rat_is(DESCR_t a);
int pl_rat_exact(DESCR_t a);
DESCR_t pl_rat_make(DESCR_t n, DESCR_t d);
double pl_rat_real(DESCR_t a);
int pl_rat_cmp(DESCR_t a, DESCR_t b);
int pl_rat_eq(DESCR_t a, DESCR_t b);
DESCR_t pl_rat_num(DESCR_t a);
DESCR_t pl_rat_den(DESCR_t a);
DESCR_t pl_rat_parse(const char *s);
void pl_rat_fput(DESCR_t a, FILE *fp);
size_t pl_rat_text(DESCR_t a, char *out, size_t cap);
int pl_rat_from_double(double x, DESCR_t *out);
int pl_rat_rationalize(double x, DESCR_t *out);
int pl_rat_pow(DESCR_t b, DESCR_t e, DESCR_t *out, int *err);
int pl_rat_unop(const char *op, DESCR_t a, DESCR_t *out, int *err);
int pl_rat_binop(const char *op, DESCR_t a, DESCR_t b, DESCR_t *out, int *err, DESCR_t *culprit);
#endif
