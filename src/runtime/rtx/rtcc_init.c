#include "rtcc.h"
#include "gc_heap.h"
#include "../../contracts/pin_va.h"
#include <stdlib.h>
#include <string.h>
__attribute__((aligned(64))) uint64_t rtccb[32];
unsigned char g_rtcc_on = 1;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__attribute__((constructor)) static void rtcc_init(void) { rtcc_gc_register(); if (RTCC_GLOBAL_R9_GVA) rtccb[RTCC_SLOT_R9] = (uint64_t)(uintptr_t)(void *)RT_GVA_VA; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rtcc_gc_register(void)
{
    const char *e = getenv("SCRIP_GC_UNROOT");
    rt_gc_root_pin_add_seamsafe((const char *)&rtccb[0]);
    if (e && strstr(e, "rtcc")) return;
    rt_gc_root_range_add_seamsafe((const char *)&rtccb[0], (const char *)&rtccb[32]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rtcc_load_scratch(void) { if (!g_rtcc_on) return; __asm__ __volatile__ ("movq %0, %%r10\n\tmovq %1, %%r11\n\tmovq %2, %%r8\n\tmovq %3, %%r9\n" : : "m"(rtccb[RTCC_SLOT_R10]), "m"(rtccb[RTCC_SLOT_R11]), "m"(rtccb[RTCC_SLOT_R8]), "m"(rtccb[RTCC_SLOT_R9]) : "r8", "r9", "r10", "r11"); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rtcc_load_all(void) {
    if (!g_rtcc_on) return;
    const uint64_t * blk = rtccb;
    __asm__ __volatile__ (
        "movq  0(%0), %%rax\n\t"
        "movq  8(%0), %%rcx\n\t"
        "movq 16(%0), %%rdx\n\t"
        "movq 24(%0), %%rsi\n\t"
        "movq 32(%0), %%rdi\n\t"
        "movq 40(%0), %%r8\n\t"
        "movq 48(%0), %%r9\n\t"
        "movq 56(%0), %%r10\n\t"
        "movq 64(%0), %%r11\n\t"
        : : "r"(blk)
        : "rax", "rcx", "rdx", "rsi", "rdi", "r8", "r9", "r10", "r11"
    );
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rtcc_coexpr_save(uint64_t *dst_256) { if (!g_rtcc_on) return; memcpy(dst_256, rtccb, RTCC_BLOCK_BYTES); }
void rtcc_coexpr_restore(const uint64_t *src_256) { if (!g_rtcc_on) return; memcpy(rtccb, src_256, RTCC_BLOCK_BYTES); }
