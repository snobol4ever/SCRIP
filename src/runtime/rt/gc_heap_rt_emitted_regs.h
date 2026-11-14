#ifndef GC_HEAP_RT_EMITTED_REGS_H
#define GC_HEAP_RT_EMITTED_REGS_H
#include <stdint.h>
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct { uint64_t ra, sp, rbx, rbp, r12, r13, r14, r15, rax, rdx; } rt_emitted_regs_t;
#ifdef __cplusplus
}
#endif
#endif
