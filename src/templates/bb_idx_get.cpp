#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
DESCR_t NV_GET_fn(const char * name);
DESCR_t subscript_get(DESCR_t arr, DESCR_t idx);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_idx_get() {
    if (!(PLATFORM_X86 && _.op_off >= 0 && _.op_name1 && _.op_parts_lbl[0] && _.op_sa >= 0)
     || !(_.bb_lk == (int)IR_LIT_INTEGER || _.bb_lk == (int)IR_LIT_STRING || (_.bb_lk == (int)IR_VAR && _.op_name2 && _.op_parts_lbl[1])))
        return x86_alpha()
             + x86_bomb(!(PLATFORM_X86 && _.op_off >= 0 && _.op_name1 && _.op_parts_lbl[0] && _.op_sa >= 0)
                        ? "bb_idx_get: unhandled (needs base name + result/scratch slots)"
                        : "bb_idx_get: unhandled key kind (LIT_I immediate, LIT_S literal, or VAR by-name only)");
    x86_begin();
    return IF(_.bb_lk == (int)IR_LIT_STRING,
              x86_alpha()
            + x86("comment", "IR_IDX: string-literal key — table-only, subscript_get")
            + x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t) _.op_name1, _.op_parts_lbl[0])
            + x86("call", "NV_GET_fn", (uint64_t)(uintptr_t)(void *) NV_GET_fn)
            + x86("mov",  "rdi", "rax")
            + x86("mov",  "rsi", "rdx")
            + x86("movabs", "rdx", (uint64_t)DT_S)
            + x86("lea",  "rcx", "[rip + __]", (uint64_t)(uintptr_t) _.op_name2, _.op_parts_lbl[1])
            + x86("call", "subscript_get", (uint64_t)(uintptr_t)(void *) subscript_get)
            + x86("mov",  FRQ(_.op_off),     "rax")
            + x86("mov",  FRQ(_.op_off + 8), "rdx")
            + x86_gamma()
            + x86_beta_trampoline())
         + IF(_.bb_lk != (int)IR_LIT_STRING,
              x86_alpha()
            + x86("comment", "IR_IDX: AXS inline DT_A+int fast path, else subscript_get")
            + IF(_.bb_lk == (int)IR_VAR,
                 x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t) _.op_name2, _.op_parts_lbl[1])
               + x86("call", "NV_GET_fn", (uint64_t)(uintptr_t)(void *) NV_GET_fn)
               + x86("mov",  FRQ(_.op_sa),     "rax")
               + x86("mov",  FRQ(_.op_sa + 8), "rdx"))
            + x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t) _.op_name1, _.op_parts_lbl[0])
            + x86("call", "NV_GET_fn", (uint64_t)(uintptr_t)(void *) NV_GET_fn)
            + x86("cmp",  "al", (long)DT_A)
            + x86("jne",  L(0))
            + IF(_.bb_lk == (int)IR_VAR,
                 x86("mov", "r8", FRQ(_.op_sa))
               + x86("cmp", "r8b", (long)DT_I)
               + x86("jne", L(0))
               + x86("mov", "rcx", FRQ(_.op_sa + 8)))
            + IF(!(_.bb_lk == (int)IR_VAR),
                 x86("movabs", "rcx", (uint64_t)(int64_t) _.bb_li))
            + x86("mov", "rsi", "rdx")
            + x86("mov", "r8",  "[rsi]")
            + x86("sub", "ecx", "r8d")
            + x86("js",  L(0))
            + x86("mov", "r9",  "[rsi + 4]")
            + x86("sub", "r9d", "r8d")
            + x86("cmp", "ecx", "r9d")
            + x86("jg",  L(0))
            + x86("mov", "rdi", "[rsi + 24]")
            + x86("movsxd", "rcx", "ecx")
            + x86("add", "rcx", "rcx")
            + x86("mov", "rax", "[rdi + rcx*8]")
            + x86("add", "rdi", (long)8)
            + x86("mov", "rdx", "[rdi + rcx*8]")
            + x86("mov", FRQ(_.op_off),     "rax")
            + x86("mov", FRQ(_.op_off + 8), "rdx")
            + x86_gamma()
            + x86("def", L(0))
            + x86("mov", "rdi", "rax")
            + x86("mov", "rsi", "rdx")
            + IF(_.bb_lk == (int)IR_VAR,
                 x86("mov", "rdx", FRQ(_.op_sa))
               + x86("mov", "rcx", FRQ(_.op_sa + 8)))
            + IF(!(_.bb_lk == (int)IR_VAR),
                 x86("movabs", "rdx", (uint64_t)DT_I)
               + x86("movabs", "rcx", (uint64_t)(int64_t) _.bb_li))
            + x86("call", "subscript_get", (uint64_t)(uintptr_t)(void *) subscript_get)
            + x86("cmp", "al", (long)DT_FAIL)
            + x86_omega("je")
            + x86("mov", FRQ(_.op_off),     "rax")
            + x86("mov", FRQ(_.op_off + 8), "rdx")
            + x86_gamma()
            + x86_beta_trampoline());
}
