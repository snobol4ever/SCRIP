#include "bb_common.h"
extern "C" {
void rt_write_int(long v);
void rt_write_float(double v);
void rt_pl_write_cell(void *cell_term);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bio_write_arg(void) {
    if (_.op_parts_tag[0] == (int)IR_ATOM)
        return (_.bb_ls ? x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(_.op_parts_str[0] ? _.op_parts_str[0] : ""), _.bb_ls)
                        : x86("xor",  "edi", "edi"))
             + x86("call", "rt_write_atom", (uint64_t)(uintptr_t)(void *)(void (*)(const char *))rt_write_atom);
    if (_.op_parts_tag[0] == (int)IR_LOGICVAR)
        return x86("mov",  "rdi", FRQ(GZ_CELL_OFF((int)_.op_parts_ival[0])))
             + x86("call", "rt_pl_write_cell", (uint64_t)(uintptr_t)(void *)rt_pl_write_cell);
    if (_.op_parts_tag[0] == (int)IR_LIT_I)
        return x86("mov",  "rdi", (long)_.op_parts_ival[0])
             + x86("call", "rt_write_int", (uint64_t)(uintptr_t)(void *)(void (*)(long))rt_write_int);
    if (_.op_parts_tag[0] == (int)IR_LIT_F)
        return x86("movabs", "rax",  (uint64_t)_.op_parts_ival[0])
             + x86("movq",   "xmm0", "rax")
             + x86("call",   "rt_write_float", (uint64_t)(uintptr_t)(void *)(void (*)(double))rt_write_float);
    return IF(_.op_parts_ival[8] != 0,
              x86("sub",  "rsp", (long)8)
            + emit_build_compound_term((const IR_t *)(intptr_t)_.op_parts_ival[8])
            + x86("mov",  "rdi", "rax")
            + x86("call", "rt_write_term_ptr", (uint64_t)(uintptr_t)(void *)(void (*)(void *))rt_write_term_ptr)
            + x86("add",  "rsp", (long)8));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_io(void) {
    if (!_.op_sval || !_.op_sval[0]) return std::string();
    if (!strcmp(_.op_sval, "nl"))
        return x86("label", _.lbl_α)
             + x86("mov32", "edi",  (long)10)
             + x86("call",  "putchar", (uint64_t)(uintptr_t)(void *)(int (*)(int))putchar)
             + x86("jmp",   "γ") + x86("def", "β") + x86("jmp", "γ");
    if (!strcmp(_.op_sval, "halt"))
        return x86("label", _.lbl_α)
             + x86("xor",  "edi",  "edi")
             + x86("call", "exit",  (uint64_t)(uintptr_t)(void *)(void (*)(int))exit)
             + x86("def",  "β") + x86("jmp", "γ");
    if (!strcmp(_.op_sval, "write") || !strcmp(_.op_sval, "writeln") || !strcmp(_.op_sval, "print"))
        return x86("label", _.lbl_α)
             + IF(_.op_parts_n >= 1, bio_write_arg())
             + IF(!strcmp(_.op_sval, "writeln"), x86("mov32", "edi", (long)10) + x86("call", "putchar", (uint64_t)(uintptr_t)(void *)(int (*)(int))putchar))
             + x86("jmp",   "γ") + x86("def", "β") + x86("jmp", "γ");
    if (!strcmp(_.op_sval, "writeq") && _.op_parts_n >= 1)
        return x86("label", _.lbl_α)
             + x86("sub",  "rsp", (long)8)
             + emit_build_compound_term((const IR_t *)(intptr_t)_.op_parts_ival[8])
             + x86("mov",  "rdi", "rax")
             + x86("call", "rt_writeq_term_ptr", (uint64_t)(uintptr_t)(void *)(void (*)(void *))rt_writeq_term_ptr)
             + x86("add",  "rsp", (long)8)
             + x86("jmp",  "γ") + x86("def", "β") + x86("jmp", "γ");
    if (!strcmp(_.op_sval, "write_canonical") && _.op_parts_n >= 1)
        return x86("label", _.lbl_α)
             + x86("sub",  "rsp", (long)8)
             + emit_build_compound_term((const IR_t *)(intptr_t)_.op_parts_ival[8])
             + x86("mov",  "rdi", "rax")
             + x86("call", "rt_write_canonical_term_ptr", (uint64_t)(uintptr_t)(void *)(void (*)(void *))rt_write_canonical_term_ptr)
             + x86("add",  "rsp", (long)8)
             + x86("jmp",  "γ") + x86("def", "β") + x86("jmp", "γ");
    return std::string();
}
