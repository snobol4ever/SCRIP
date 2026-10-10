#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
#include "x86_asm.h"
extern "C" long rk_box_step(const char *subj, long delta, long end, const char *blob);
extern "C" long rk_box_back(const char *subj, long delta, const char *blob, long floor_delta);
#define RUN_MIN() ((long)(_.op_ival & 0xFFFF))
#define RUN_MAX() ((long)((_.op_ival >> 16) & 0xFFFF))
#define RUN_FRUGAL() ((_.op_ival >> 32) & 1)
#define RUN_RATCHET() ((_.op_ival >> 32) & 2)
#define RUN_START() RDD("rbp", _.op_off)
#define RUN_COUNT() RDD("rbp", _.op_off + 4)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string rkrun_step_call() {
    return x86("mov", "rdi", "r13")
         + x86("movsxd", "rsi", "r14d")
         + x86("movsxd", "rdx", "r15d")
         + x86("lea", "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), x86_strtab_lbl((_.op_sval ? _.op_sval : "")).c_str())
         + x86("call", "rk_box_step", (uint64_t)(uintptr_t)(void *)rk_box_step);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string rkrun_back_call() {
    return x86("mov", "rdi", "r13")
         + x86("movsxd", "rsi", "r14d")
         + x86("lea", "rdx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), x86_strtab_lbl((_.op_sval ? _.op_sval : "")).c_str())
         + x86("mov", "ecx", RUN_START())
         + x86("movsxd", "rcx", "ecx")
         + x86("call", "rk_box_back", (uint64_t)(uintptr_t)(void *)rk_box_back);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string rkrun_greedy() {
    return x86("comment", "IR_MATCH_RKRUN greedy: match as many atoms as the bounds allow, give one back on beta")
         + x86_alpha()
         + x86("mov", RUN_START(), "r14d")
         + x86("mov", RUN_COUNT(), (long)0)
         + x86("def", L(0))
         + IF(RUN_MAX() != 0xFFFF, x86("mov", "eax", RUN_COUNT()) + x86("cmp", "eax", RUN_MAX()) + x86("jge", L(2)))
         + rkrun_step_call()
         + x86("test", "eax", "eax")
         + x86("js", L(2))
         + x86("cmp", "eax", "r14d")
         + x86("je", L(2))
         + x86("mov", "r14d", "eax")
         + x86("mov", "eax", RUN_COUNT())
         + x86("add", "eax", (long)1)
         + x86("mov", RUN_COUNT(), "eax")
         + x86("jmp", L(0))
         + x86("def", L(2))
         + x86("mov", "eax", RUN_COUNT())
         + x86("cmp", "eax", RUN_MIN())
         + x86("jl", L(3))
         + x86_gamma()
         + x86_beta()
         + IF(RUN_RATCHET(), x86("jmp", L(3)))
         + IF(!RUN_RATCHET(),
              x86("mov", "eax", RUN_COUNT())
            + x86("cmp", "eax", RUN_MIN())
            + x86("jle", L(3))
            + rkrun_back_call()
            + x86("mov", "r14d", "eax")
            + x86("mov", "eax", RUN_COUNT())
            + x86("sub", "eax", (long)1)
            + x86("mov", RUN_COUNT(), "eax")
            + x86_gamma())
         + x86("def", L(3))
         + x86("mov", "r14d", RUN_START())
         + x86_omega();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string rkrun_frugal() {
    return x86("comment", "IR_MATCH_RKRUN frugal: match the minimum, take one more atom on each beta")
         + x86_alpha()
         + x86("mov", RUN_START(), "r14d")
         + x86("mov", RUN_COUNT(), (long)0)
         + x86("def", L(0))
         + x86("mov", "eax", RUN_COUNT())
         + x86("cmp", "eax", RUN_MIN())
         + x86("jge", L(1))
         + rkrun_step_call()
         + x86("test", "eax", "eax")
         + x86("js", L(3))
         + x86("mov", "r14d", "eax")
         + x86("mov", "eax", RUN_COUNT())
         + x86("add", "eax", (long)1)
         + x86("mov", RUN_COUNT(), "eax")
         + x86("jmp", L(0))
         + x86("def", L(1))
         + x86_gamma()
         + x86_beta()
         + IF(RUN_MAX() != 0xFFFF, x86("mov", "eax", RUN_COUNT()) + x86("cmp", "eax", RUN_MAX()) + x86("jge", L(3)))
         + rkrun_step_call()
         + x86("test", "eax", "eax")
         + x86("js", L(3))
         + x86("cmp", "eax", "r14d")
         + x86("je", L(3))
         + x86("mov", "r14d", "eax")
         + x86("mov", "eax", RUN_COUNT())
         + x86("add", "eax", (long)1)
         + x86("mov", RUN_COUNT(), "eax")
         + x86_gamma()
         + x86("def", L(3))
         + x86("mov", "r14d", RUN_START())
         + x86_omega();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_rkrun() {
    x86_begin();
    return _.op_off > -16
         ? x86_alpha() + x86_bomb("IR_MATCH_RKRUN: frame cell not granted") + x86_beta() + x86_omega()
         : RUN_FRUGAL() ? rkrun_frugal() : rkrun_greedy();
}
