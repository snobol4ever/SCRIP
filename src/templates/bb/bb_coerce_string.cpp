#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
int rt_coerce_str_d(const DESCR_t *in, DESCR_t *out, long codes);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string coerce_string_fast(void) {
    return x86("mov",  "rax", RDQ("rdi", 0))
         + x86("cmp",  "al", (long)DT_S)
         + x86("jne",  L(21))
         + x86("mov",  "rdx", RDQ("rdi", 8))
         + x86("test", "rdx", "rdx")
         + x86("je",   L(21))
         + x86("mov",  "ecx", RDD("rdi", 4))
         + x86("test", "ecx", "ecx")
         + x86("je",   L(21))
         + x86("cmp",  "ecx", (long)-1)
         + x86("je",   L(21))
         + x86("mov",  RDQ("rsi", 0), "rax")
         + x86("mov",  RDQ("rsi", 8), "rdx")
         + x86("jmp",  L(22))
         + x86("def",  L(21));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string coerce_string_raised(void) {
    return IF(_.op_ival != 0,
               x86("test", "eax", "eax")
             + x86("jz",   L(24))
             + x86_rt_gc_poll()
             + x86_omega()
             + x86("def",  L(24)));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_coerce_string() {
    x86_begin();
    return IF(_.op_zres,
               x86("comment", "IR_COERCE_STRING zd")
             + x86_alpha()
             + x86("note", ZOPN(0))
             + x86("lea",  "rdi", ZOPQ(0, 0))
             + x86("note", ZRESN())
             + x86("lea",  "rsi", ZRES(0))
             + coerce_string_fast()
             + x86("mov",  "rdx", (long)_.op_ival)
             + x86("call", "rt_coerce_str_d", (uint64_t)(uintptr_t)(void *)rt_coerce_str_d)
             + coerce_string_raised()
             + x86_rt_gc_poll()
             + x86("def",  L(22))
             + x86_gamma()
             + x86_beta_trampoline())
         + IF(!_.op_zres,
               IF(_.op_sa < 0 || _.op_off < 0, x86_bomb("bb_coerce_string: needs operand slot (op_sa) + own value slot (op_off)"))
             + IF(!(_.op_sa < 0 || _.op_off < 0),
             x86("comment", "IR_COERCE_STRING")
           + x86_alpha()
           + x86("lea",  "rdi", FRQ(_.op_sa))
           + x86("lea",  "rsi", FRQ(_.op_off))
           + coerce_string_fast()
           + x86("mov",  "rdx", (long)_.op_ival)
           + x86("call", "rt_coerce_str_d", (uint64_t)(uintptr_t)(void *)rt_coerce_str_d)
           + coerce_string_raised()
           + x86_rt_gc_poll()
           + x86("def",  L(22))
           + x86_gamma()
           + x86_beta_trampoline()));
}
