/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef PIN_VA_H
#define PIN_VA_H
#include <sys/mman.h>
#include <stdint.h>
#define RT_PIN_BASE   0x70000000UL
#define RT_PIN_BYTES  4096UL
#define RT_DCAP_TOP    (RT_PIN_BASE + 0)
#define RT_WS_LIMIT   (RT_PIN_BASE + 8)
#define RT_WS_TOP     (RT_PIN_BASE + 16)
#define RT_AB_ANCHOR  (RT_PIN_BASE + 24)
#define RT_AB_NRET    (RT_PIN_BASE + 32)
#define RT_GVA_VA     (RT_PIN_BASE + RT_PIN_BYTES)
#ifndef MAP_FIXED_NOREPLACE
#define MAP_FIXED_NOREPLACE 0x100000
#endif
#endif
