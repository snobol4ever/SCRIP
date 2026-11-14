#ifndef CORE_BN_39_SH_API_H
#define CORE_BN_39_SH_API_H
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef __cplusplus
extern "C" {
#endif
void c_rt_translate_bytes(char *dst, const char *src, size_t n, const char *map);
#ifdef __cplusplus
}
#endif
#endif
