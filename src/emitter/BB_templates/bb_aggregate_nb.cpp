#include "bb_common.h"
extern std::string emit_build_compound_term(const IR_t *nd);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_aggregate_nb() {
    if (PLATFORM_X86)
        return IF(_.op_sval && !strcmp(_.op_sval, "aggregate_all") && _.op_parts_n == 3,
                  x86("label",   _.lbl_α)
                + x86("comment", "IR_BUILTIN aggregate_all")
                + emit_build_compound_term((const IR_t *)(intptr_t)_.op_parts_ival[8])
                + x86("push", "rax")
                + emit_build_compound_term((const IR_t *)(intptr_t)_.op_parts_ival[9])
                + x86("push", "rax")
                + emit_build_compound_term((const IR_t *)(intptr_t)_.op_parts_ival[10])
                + x86("push", "rax")
                + x86("sub",  "rsp", 8L)
                + x86("mov",  "rdx", RSP(8))
                + x86("mov",  "rsi", RSP(16))
                + x86("mov",  "rdi", RSP(24))
                + x86("call", "rt_aggregate_all_meta", (uint64_t)(uintptr_t)(void *)rt_aggregate_all_meta)
                + x86("add",  "rsp", 32L)
                + x86("test", "eax", "eax")
                + x86("je",   "ω")
                + x86("jmp",  "γ")
                + x86("def",  "β")
                + x86("jmp",  "ω"))
             + IF(_.op_sval && !strcmp(_.op_sval, "nb_setval") && _.op_parts_n >= 2,
                  x86("label",   _.lbl_α)
                + x86("comment", "IR_BUILTIN nb_setval")
                + emit_build_compound_term((const IR_t *)(intptr_t)_.op_parts_ival[8])
                + x86("push", "rax")
                + x86("sub",  "rsp", 8L)
                + emit_build_compound_term((const IR_t *)(intptr_t)_.op_parts_ival[9])
                + x86("mov",  "rsi", "rax")
                + x86("mov",  "rdi", RSP(8))
                + x86("call", "rt_nb_setval_term", (uint64_t)(uintptr_t)(void *)rt_nb_setval_term)
                + x86("add",  "rsp", 16L)
                + x86("test", "eax", "eax")
                + x86("je",   "ω")
                + x86("jmp",  "γ")
                + x86("def",  "β")
                + x86("jmp",  "ω"))
             + IF(_.op_sval && !strcmp(_.op_sval, "nb_getval") && _.op_parts_n >= 2,
                  x86("label",   _.lbl_α)
                + x86("comment", "IR_BUILTIN nb_getval")
                + emit_build_compound_term((const IR_t *)(intptr_t)_.op_parts_ival[8])
                + x86("mov",    "rdi", "rax")
                + x86("mov32",  "esi", (long)_.op_parts_tag[1])
                + x86("movabs", "rdx", (uint64_t)(int64_t)_.op_parts_ival[1])
                + IF(_.op_parts_tag[1] == (int)IR_ATOM && _.op_parts_str[1] && *_.op_parts_str[1],
                     x86("lea", "rcx", "[rip + __]", (uint64_t)(uintptr_t)_.op_parts_str[1], _.bb_rs))
                + IF(!(_.op_parts_tag[1] == (int)IR_ATOM && _.op_parts_str[1] && *_.op_parts_str[1]),
                     x86("xor", "ecx", "ecx"))
                + x86("call", "rt_nb_getval_term", (uint64_t)(uintptr_t)(void *)rt_nb_getval_term)
                + x86("test", "eax", "eax")
                + x86("je",   "ω")
                + x86("jmp",  "γ")
                + x86("def",  "β")
                + x86("jmp",  "ω"));
    return std::string();
}
