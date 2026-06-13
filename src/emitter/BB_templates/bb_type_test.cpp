#include "bb_common.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_type_test() {
    if (!(_.op_ival == 1 && _.op_parts_n >= 1 && _.op_sval &&
          (strcmp(_.op_sval, "var") == 0      || strcmp(_.op_sval, "nonvar") == 0   || strcmp(_.op_sval, "atom") == 0
        || strcmp(_.op_sval, "atomic") == 0   || strcmp(_.op_sval, "number") == 0   || strcmp(_.op_sval, "integer") == 0
        || strcmp(_.op_sval, "float") == 0    || strcmp(_.op_sval, "compound") == 0 || strcmp(_.op_sval, "callable") == 0
        || strcmp(_.op_sval, "is_list") == 0  || strcmp(_.op_sval, "ground") == 0)))
        return std::string();
    return (_.op_parts_tag[0] == (int)IR_STRUCT || _.op_parts_tag[0] == (int)IR_ARITH)
         ? x86("label", _.lbl_α)
             + x86("sub", "rsp", (long)16)
             + emit_build_compound_term((IR_t *)(intptr_t)_.op_parts_ival[8])
             + x86("mov", "rsi", "rax")
             + x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)_.op_sval, _.op_sval_lbl)
             + x86("call", "rt_type_test_term", (uint64_t)(uintptr_t)(void*)rt_type_test_term)
             + x86("add", "rsp", (long)16)
             + x86("test", "eax", "eax")
             + x86("je", "ω")
             + x86("jmp", "γ")
             + x86("def", "β")
             + x86("jmp", "ω")
         : x86("label", _.lbl_α)
             + x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)_.op_sval, _.op_sval_lbl)
             + x86("mov", "esi", (long)_.op_parts_tag[0])
             + x86("mov", "rdx", (long)_.op_parts_ival[0])
             + (_.op_parts_lbl[0] ? x86("lea", "rcx", "[rip + __]", (uint64_t)(uintptr_t)_.op_parts_str[0], _.op_parts_lbl[0])
                                  : x86("xor", "ecx", "ecx"))
             + x86("call", "rt_type_test", (uint64_t)(uintptr_t)(void*)rt_type_test)
             + x86("test", "eax", "eax")
             + x86("je", "ω")
             + x86("jmp", "γ")
             + x86("def", "β")
             + x86("jmp", "ω");
}
