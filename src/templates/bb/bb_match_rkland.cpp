#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
#include "x86_asm.h"
extern "C" void rk_box_match_build(const char *subj, long span, const long *cells, const char *names);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_rkland() {
    x86_begin();
    return _.op_off > -16
         ? x86_alpha() + x86_bomb("IR_MATCH_RKLAND: the capture block was not granted") + x86_beta() + x86_omega()
         : x86("comment", "IR_MATCH_RKLAND: the pattern succeeded at [start_delta, r14d); fill the current match from the capture block")
         + x86_alpha()
         + x86("mov", "eax", "r15d")
         + x86("shl", "rax", (long)32)
         + x86("or", "rax", (long)_.op_ival)
         + x86("mov", RDQ("rbp", _.op_off), "rax")
         + x86("mov", "rdi", "r13")
         + x86("mov", "esi", RDD("rbp", -40))
         + x86("shl", "rsi", (long)32)
         + x86("mov", "eax", "r14d")
         + x86("or", "rsi", "rax")
         + x86("lea", "rdx", RDQ("rbp", _.op_off))
         + x86("lea", "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), x86_strtab_lbl((_.op_sval ? _.op_sval : "")).c_str())
         + x86_rtcc_call("rk_box_match_build", (uint64_t)(uintptr_t)(void *)rk_box_match_build)
         + x86_gamma()
         + x86_beta()
         + x86_omega();
}
