#ifndef POSIX_LEAVES_H
#define POSIX_LEAVES_H
#include "ir/descr.h"
void px_begin(void);
DESCR_t px_fail(void);
const char *px_str(DESCR_t d);
DESCR_t px_rec(const char *tn, DESCR_t *f, int n);
DESCR_t px_list(DESCR_t *f, int n);
DESCR_t px_fmt(const char *fmt, ...);
int px_signal_number(DESCR_t d);
const char *px_signal_name(int sig);
#include <stdio.h>
FILE *px_sock_open(const char *addr, const char *mode);
#endif
