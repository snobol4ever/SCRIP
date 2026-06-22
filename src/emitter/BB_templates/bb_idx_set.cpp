#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
DESCR_t NV_GET_fn(const char * name);
int subscript_set(DESCR_t arr, DESCR_t idx, DESCR_t val);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_idx_set() {
    if (!PLATFORM_X86 || !(_.op_name1 && _.op_parts_lbl[0] && _.op_sa >= 0 && _.op_sb >= 0 && _.op_sc >= 0))
        return x86_bomb("bb_idx_set: unhandled (needs base name + base/key/value scratch slots)");
    if (!(_.bb_lk == (int)IR_LIT_I || (_.bb_lk == (int)IR_VAR && _.op_name2 && _.op_parts_lbl[1])))
        return x86_bomb("bb_idx_set: unhandled key kind (LIT_I immediate or VAR by-name only)");
    if (!(_.bb_rk == (int)IR_LIT_I || (_.bb_rk == (int)IR_VAR && _.op_parts_str[2] && _.op_parts_lbl[2])))
        return x86_bomb("bb_idx_set: unhandled value kind (LIT_I immediate or VAR by-name only)");
    std::string s = x86("label", _.lbl_α)
                  + x86("comment", "IR_IDX_SET (subscript_set, by-name)")
                  + x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t) _.op_name1, _.op_parts_lbl[0])
                  + x86("call", "NV_GET_fn", (uint64_t)(uintptr_t)(void *) NV_GET_fn)
                  + x86("mov",  FRQ(_.op_sa),     "rax")
                  + x86("mov",  FRQ(_.op_sa + 8), "rdx");
    if (_.bb_lk == (int)IR_LIT_I)
        s += x86("movabs", "rax", (uint64_t)DT_I)     + x86("mov", FRQ(_.op_sb),     "rax")
           + x86("movabs", "rax", (uint64_t)_.bb_li)  + x86("mov", FRQ(_.op_sb + 8), "rax");
    else
        s += x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t) _.op_name2, _.op_parts_lbl[1])
           + x86("call", "NV_GET_fn", (uint64_t)(uintptr_t)(void *) NV_GET_fn)
           + x86("mov",  FRQ(_.op_sb),     "rax")
           + x86("mov",  FRQ(_.op_sb + 8), "rdx");
    if (_.bb_rk == (int)IR_LIT_I)
        s += x86("movabs", "rax", (uint64_t)DT_I)     + x86("mov", FRQ(_.op_sc),     "rax")
           + x86("movabs", "rax", (uint64_t)_.bb_ri)  + x86("mov", FRQ(_.op_sc + 8), "rax");
    else
        s += x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t) _.op_parts_str[2], _.op_parts_lbl[2])
           + x86("call", "NV_GET_fn", (uint64_t)(uintptr_t)(void *) NV_GET_fn)
           + x86("mov",  FRQ(_.op_sc),     "rax")
           + x86("mov",  FRQ(_.op_sc + 8), "rdx");
    return s + x86("mov",  "rdi", FRQ(_.op_sa)) + x86("mov", "rsi", FRQ(_.op_sa + 8))
             + x86("mov",  "rdx", FRQ(_.op_sb)) + x86("mov", "rcx", FRQ(_.op_sb + 8))
             + x86("mov",  "r8",  FRQ(_.op_sc)) + x86("mov", "r9",  FRQ(_.op_sc + 8))
             + x86("call", "subscript_set", (uint64_t)(uintptr_t)(void *) subscript_set)
             + x86("cmp",  "eax", (long)0)
             + x86("je",   "ω")
             + x86("jmp",  "γ")
             + x86("def",  "β")
             + x86("jmp",  "ω");
}
