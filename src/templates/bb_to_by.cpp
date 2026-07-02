#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_to_by() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    if (!(_.op_off >= 0 && _.op_sa >= 0 && _.op_sb >= 0 && _.op_sc >= 0))
        return x86_bomb("bb_to_by: unhandled (needs from/to/by operand slots, descr flat-chain)");
    return x86("comment", "IR_TO_BY")
         + x86("label",   _.lbl_α)
         + x86("mov",     "rax", FRQ(_.op_sa + 8))
         + x86("mov",     FRQ(_.op_off + 16), "rax")
         + x86("def",     L(0))
         + x86("mov",     "rax", FRQ(_.op_off + 16))
         + x86("mov",     "rcx", FRQ(_.op_sb + 8))
         + x86("mov",     "rdx", FRQ(_.op_sc + 8))
         + x86("cmp",     "rdx", (long)0)
         + x86("jl",      L(1))
         + x86("cmp",     "rax", "rcx")
         + x86("jg",      "ω")
         + x86("jmp",     L(2))
         + x86("def",     L(1))
         + x86("cmp",     "rax", "rcx")
         + x86("jl",      "ω")
         + x86("def",     L(2))
         + x86("mov",     FRQ(_.op_off),     (long)DT_I)
         + x86("mov",     FRQ(_.op_off + 8), "rax")
         + x86("jmp",     "γ")
         + x86("def",     "β")
         + x86("mov",     "rdx", FRQ(_.op_sc + 8))
         + x86("mov",     "rax", FRQ(_.op_off + 16))
         + x86("add",     "rax", "rdx")
         + x86("mov",     FRQ(_.op_off + 16), "rax")
         + x86("jmp",     L(0));
}
