#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
void rt_coerce_num2_d(const DESCR_t *self, const DESCR_t *other, DESCR_t *out, long codes);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_coerce_numeric() {
    x86_begin();
    if (_.op_zres)
        return x86("comment", "IR_COERCE_NUMERIC zd")
             + x86_alpha()
             + x86("note", ZOPN(0))
             + x86("mov", "eax", ZOPD(0, 0))
             + x86("cmp", "al", (long)DT_R)
             + x86("je", L(1))
             + x86("cmp", "al", (long)DT_I)
             + x86("jne", L(0))
             + x86("note", ZOPN(1))
             + x86("mov", "eax", ZOPD(1, 0))
             + x86("cmp", "al", (long)DT_I)
             + x86("jne", L(0))
             + x86("def", L(1))
             + x86("note", ZOPN(0))
             + x86("mov", "rax", ZOPQ(0, 0))
             + x86("note", ZRESN())
             + x86("mov", ZRES(0), "rax")
             + x86("note", ZOPN(0))
             + x86("mov", "rax", ZOPQ(0, 8))
             + x86("note", ZRESN())
             + x86("mov", ZRES(8), "rax")
             + x86_gamma()
             + x86("def", L(0))
             + x86("note", ZOPN(0))
             + x86("lea",  "rdi", ZOPQ(0, 0))
             + x86("note", ZOPN(1))
             + x86("lea",  "rsi", ZOPQ(1, 0))
             + x86("note", ZRESN())
             + x86("lea",  "rdx", ZRES(0))
             + x86("mov",  "rcx", (long)_.op_ival)
             + x86("call", "rt_coerce_num2_d", (uint64_t)(uintptr_t)(void *)rt_coerce_num2_d)
             + x86_gamma()
             + x86_beta_trampoline();
    return IF(_.op_sa < 0 || _.op_sb < 0 || _.op_off < 0, x86_bomb("bb_coerce_numeric: needs self slot (op_sa) + other slot (op_sb) + own value slot (op_off)"))
         + IF(!(_.op_sa < 0 || _.op_sb < 0 || _.op_off < 0),
             x86("comment", "IR_COERCE_NUMERIC")
           + x86_alpha()
           + x86("mov", "eax", FR(_.op_sa))
           + x86("cmp", "al", (long)DT_R)
           + x86("je", L(1))
           + x86("cmp", "al", (long)DT_I)
           + x86("jne", L(0))
           + x86("mov", "eax", FR(_.op_sb))
           + x86("cmp", "al", (long)DT_I)
           + x86("jne", L(0))
           + x86("def", L(1))
           + x86("mov", "rax", FRQ(_.op_sa))
           + x86("mov", FRQ(_.op_off), "rax")
           + x86("mov", "rax", FRQ(_.op_sa + 8))
           + x86("mov", FRQ(_.op_off + 8), "rax")
           + x86_gamma()
           + x86("def", L(0))
           + x86("lea",  "rdi", FRQ(_.op_sa))
           + x86("lea",  "rsi", FRQ(_.op_sb))
           + x86("lea",  "rdx", FRQ(_.op_off))
           + x86("mov",  "rcx", (long)_.op_ival)
           + x86("call", "rt_coerce_num2_d", (uint64_t)(uintptr_t)(void *)rt_coerce_num2_d)
           + x86_gamma()
           + x86_beta_trampoline());
}
