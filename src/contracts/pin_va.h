#ifndef PIN_VA_H
#define PIN_VA_H
#include <sys/mman.h>
#include <stdint.h>
/* REG-0 PIN-VA (s73 six-register ruling, GOAL-SNOBOL4-BB RUNG REG-MAP): build-constant low VAs so BOTH mediums bake absolute-disp32 (identical bytes, MODE34 law); page mapped MAP_FIXED_NOREPLACE by rt_pin_init (rt.c) -- abort on conflict is the tripwire; fixed low VA => mode-4 links non-PIE or VA sits outside loader range. */
#define RT_PIN_BASE   0x30000000UL
#define RT_PIN_BYTES  4096UL
#define RT_CAS_TOP    (RT_PIN_BASE + 0)
#define RT_WS_LIMIT   (RT_PIN_BASE + 8)
#define RT_GVA_VA     0x30001000UL
#ifndef MAP_FIXED_NOREPLACE
#define MAP_FIXED_NOREPLACE 0x100000
#endif
#endif
