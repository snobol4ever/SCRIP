#ifndef CORE_RT_PLAIN_INT_STR_H
#define CORE_RT_PLAIN_INT_STR_H
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef __cplusplus
extern "C" {
#endif
static inline __attribute__((always_inline)) int rt_plain_int_str(const char *s) {
    if (!s) return 0;
    while (*s == ' ' || *s == '\t') s++;
    if (*s == '+' || *s == '-') s++;
    if (*s < '0' || *s > '9') return 0;
    while (*s >= '0' && *s <= '9') s++;
    while (*s == ' ' || *s == '\t') s++;
    return *s == '\0';
}
#ifdef __cplusplus
}
#endif
#endif
