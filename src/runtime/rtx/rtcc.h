/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef RTCC_H
#define RTCC_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
#define RTCC_GPR_COUNT  9
#define RTCC_XMM_COUNT  8
#define RTCC_GPR_BYTES  (RTCC_GPR_COUNT * 8)
#define RTCC_XMM_BYTES  (RTCC_XMM_COUNT * 16)
#define RTCC_BLOCK_BYTES (RTCC_GPR_BYTES + RTCC_XMM_BYTES)
#define RTCC_SLOT_RAX   0
#define RTCC_SLOT_RCX   1
#define RTCC_SLOT_RDX   2
#define RTCC_SLOT_RSI   3
#define RTCC_SLOT_RDI   4
#define RTCC_SLOT_R8    5
#define RTCC_SLOT_R9    6
#define RTCC_SLOT_R10   7
#define RTCC_SLOT_R11   8
#define RTCC_MBC_QUAD  31
#ifndef RTCC_GLOBAL_R8_ANCHOR
#define RTCC_GLOBAL_R8_ANCHOR   1
#endif
#ifndef RTCC_GLOBAL_R9_GVA
#define RTCC_GLOBAL_R9_GVA   1
#endif
#define RTCC_GVA_REG            "r9"
extern uint64_t rtccb[32];
extern unsigned char g_rtcc_on;
void rtcc_gc_register(void);
void rtcc_coexpr_save(uint64_t *dst_256);
void rtcc_coexpr_restore(const uint64_t *src_256);
void rtcc_load_scratch(void);
void rtcc_load_all(void);
#ifdef __cplusplus
}
#endif
#endif
