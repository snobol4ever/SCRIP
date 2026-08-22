#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
DESCR_t str_concat_d(DESCR_t a, DESCR_t b);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_scan_sequence() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    return _.op_off < 0
             ? x86_alpha() + x86_bomb("IR_SCAN_SEQUENCE: value/state slot not granted (zls)")
             : x86("comment", "IR_SCAN_SEQ_NARY")
             + x86_alpha()
             + x86("mov", FR(_.op_off + 16), "r14d")
             + x86("mov", FR(_.op_off + 20), 0)
             + x86("jmp", PAIR(0))
             + x86("def", PAIR((int)(2 * _.op_ival)))
             + x86("mov", "eax", FR(_.op_off + 20))
             + x86("add", "eax", 1)
             + x86("mov", FR(_.op_off + 20), "eax")
             + FOR(1, (int)_.op_ival, [&](int i){ return x86("cmp", "eax", i)
                  + x86("je", PAIR(i)); })
             + x86("mov", "rax", FRQ((int)_.op_parts_ival[0]))
             + x86("mov", "rdx", FRQ((int)_.op_parts_ival[0] + 8))
             + FOR(1, (int)_.op_ival, [&](int j){ return x86("mov", "rdi", "rax")
                  + x86("mov", "rsi", "rdx")
                  + x86("mov", "rdx", FRQ((int)_.op_parts_ival[j]))
                  + x86("mov", "rcx", FRQ((int)_.op_parts_ival[j] + 8))
                  + x86("push", "r11")
                  + x86("push", "r11")
                  + x86("call", "str_concat_d", (uint64_t)(uintptr_t)(void*)str_concat_d)
                  + x86("pop", "r11")
                  + x86("pop", "r11"); })
             + x86("mov", FRQ(_.op_off), "rax")
             + x86("mov", FRQ(_.op_off + 8), "rdx")
             + x86_gamma()
             + x86_beta()
             + x86("mov", FR(_.op_off + 20), (int)_.op_ival)
             + x86("def", PAIR((int)(2 * _.op_ival + 1)))
             + x86("mov", "eax", FR(_.op_off + 20))
             + x86("sub", "eax", 1)
             + x86("mov", FR(_.op_off + 20), "eax")
             + FOR(0, (int)_.op_ival, [&](int i){ return x86("cmp", "eax", i)
                  + x86("je", PAIR((int)_.op_ival + i)); })
             + x86_omega();
}
