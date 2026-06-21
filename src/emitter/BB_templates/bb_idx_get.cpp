#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
DESCR_t NV_GET_fn(const char * name);
DESCR_t subscript_get(DESCR_t arr, DESCR_t idx);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_idx_get() {
    if (!PLATFORM_X86 || !(_.op_off >= 0 && _.op_name1 && _.op_parts_lbl[0]))
        return x86_bomb("bb_idx_get: unhandled (needs base name + result slot)");
    if (!(_.op_node_kind == (int)IR_LIT_I || (_.op_node_kind == (int)IR_VAR && _.op_name2 && _.op_parts_lbl[1] && _.op_sa >= 0)))
        return x86_bomb("bb_idx_get: unhandled key kind (LIT_I immediate or VAR by-name only)");
    std::string s = x86("label", _.lbl_α)
                  + x86("comment", "IR_IDX (read via subscript_get)")
                  + x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t) _.op_name1, _.op_parts_lbl[0])
                  + x86("call", "NV_GET_fn", (uint64_t)(uintptr_t)(void *) NV_GET_fn);
    if (_.op_node_kind == (int)IR_LIT_I)
        s += x86("mov",    "rdi", "rax")
           + x86("mov",    "rsi", "rdx")
           + x86("movabs", "rdx", (uint64_t)DT_I)
           + x86("movabs", "rcx", (uint64_t)_.op_ival)
           + x86("call",   "subscript_get", (uint64_t)(uintptr_t)(void *) subscript_get);
    else
        s += x86("mov",  FRQ(_.op_sa),     "rax")
           + x86("mov",  FRQ(_.op_sa + 8), "rdx")
           + x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t) _.op_name2, _.op_parts_lbl[1])
           + x86("call", "NV_GET_fn", (uint64_t)(uintptr_t)(void *) NV_GET_fn)
           + x86("mov",  "rcx", "rdx")
           + x86("mov",  "rdx", "rax")
           + x86("mov",  "rdi", FRQ(_.op_sa))
           + x86("mov",  "rsi", FRQ(_.op_sa + 8))
           + x86("call", "subscript_get", (uint64_t)(uintptr_t)(void *) subscript_get);
    return s + x86("mov", FRQ(_.op_off),     "rax")
             + x86("mov", FRQ(_.op_off + 8), "rdx")
             + x86("jmp", "γ")
             + x86("def", "β")
             + x86("jmp", "ω");
}
