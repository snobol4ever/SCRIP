#ifndef CORE_IO_CHAN_H
#define CORE_IO_CHAN_H
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct { FILE *fp; char *varname; int is_output; int is_popen; char *buf; size_t cap; long rlen; int unit; char *prebind; } io_chan_t;
#ifdef __cplusplus
}
#endif
#endif
