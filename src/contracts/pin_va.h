#ifndef PIN_VA_H
#define PIN_VA_H
#include <sys/mman.h>
#include <stdint.h>
/* REG-0 PIN-VA (s73 six-register ruling, GOAL-SNOBOL4-BB RUNG REG-MAP): build-constant low VAs so BOTH mediums bake absolute-disp32 (identical bytes, MODE34 law); page mapped MAP_FIXED_NOREPLACE by rt_pin_init (rt.c) -- abort on conflict is the tripwire; fixed low VA => mode-4 links non-PIE or VA sits outside loader range. */
/* s75 RELOCATION 0x30000000 -> 0x70000000: non-PIE brk-base randomization (heap = image_end + rand(0..32MB)) intermittently landed inside the old 16MB island window (EEXIST -> tripwire, ~1% of execs). Pinned VAs must sit ABOVE brk reach and BELOW 0x7FFFFFFF (absolute-disp32 ceiling). */
#define RT_PIN_BASE   0x70000000UL
#define RT_PIN_BYTES  4096UL
#define RT_DCAP_TOP    (RT_PIN_BASE + 0)
#define RT_WS_LIMIT   (RT_PIN_BASE + 8)
#define RT_WS_TOP     (RT_PIN_BASE + 16)
#define RT_GVA_VA     (RT_PIN_BASE + RT_PIN_BYTES)
#ifndef MAP_FIXED_NOREPLACE
#define MAP_FIXED_NOREPLACE 0x100000
#endif
#endif
