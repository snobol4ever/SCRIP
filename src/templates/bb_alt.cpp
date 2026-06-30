#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern int g_descr_flat_chain;
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_alt() {
    x86_begin();
    if (PLATFORM_X86) return IF(!(g_descr_flat_chain && _.op_off >= 0 && _.op_parts_n > 0),
                                x86_bomb("bb_alt: unhandled (needs <=5 literal arms, descr flat-chain)"))
                           + IF(g_descr_flat_chain && _.op_off >= 0 && _.op_parts_n > 0,
                             x86("label", _.lbl_α)
                           + x86("comment", "IR_ALT")
                           + x86("mov", FRQ(_.op_off + 16), 0L)
                           + x86("def", L(_.op_parts_n))
                           + FOR(0, _.op_parts_n, [&](int i) { return x86("mov", "rax", FRQ(_.op_off + 16))
                                                                    + x86("cmp", "rax", (long)i)
                                                                    + x86("je", L(_.op_parts_n + 1 + i)); })
                           + x86("jmp", "ω")
                           + FOR(0, _.op_parts_n, [&](int i) { return x86("def", L(_.op_parts_n + 1 + i))
                                                                    + x86("mov", FRQ(_.op_off), (long)_.op_parts_tag[i])
                                                                    + x86("mov", "rax", ROQ(i))
                                                                    + x86("mov", FRQ(_.op_off + 8), "rax")
                                                                    + x86("mov", "rax", FRQ(_.op_off + 16))
                                                                    + x86("add", "rax", 1L)
                                                                    + x86("mov", FRQ(_.op_off + 16), "rax")
                                                                    + x86("jmp", "γ"); })
                           + x86("def", "β")
                           + x86("jmp", L(_.op_parts_n))
                           + FOR(0, _.op_parts_n, [&](int i) { return (_.op_parts_tag[i] == (int)DT_I || _.op_parts_tag[i] == (int)DT_R)
                                                                    ? x86("def", L(i))
                                                                    + x86(".quad", (long)_.op_parts_ival[i])
                                                                    : x86("def", L(i))
                                                                    + x86(".quad", LS(i), _.op_parts_str[i])
                                                                    + x86("label", LS(i))
                                                                    + x86(".string", _.op_parts_str[i]); }));
    return std::string();
}
