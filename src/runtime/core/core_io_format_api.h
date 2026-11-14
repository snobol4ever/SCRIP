#ifndef CORE_IO_FORMAT_API_H
#define CORE_IO_FORMAT_API_H
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ir/descr.h"
#ifdef __cplusplus
extern "C" {
#endif
void output_val(DESCR_t v);
void output_str(const char *s);
#ifdef __cplusplus
}
#endif
#endif
