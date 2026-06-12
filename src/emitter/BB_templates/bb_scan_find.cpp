#include <string>
#include <cstring>
#include <cstdint>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern int g_descr_flat_chain;
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_scan_find() {
    x86_begin();
    if (!PLATFORM_X86 || !(g_descr_flat_chain && _.op_off >= 0 && _.op_name1 && _.op_name1[0] && strlen(_.op_name1) <= 32)) return x86_bomb("bb_scan_find: unhandled (needs nonempty literal needle <=32 + descr flat-chain slot)");
    return x86("comment", "IR_SCAN_FIND")
         + x86("label",   _.lbl_α)
         + x86("mov",     FRQ(_.op_off + 16), "r14")
         + x86("def",     L(0))
         + x86("mov",     "rax", FRQ(_.op_off + 16))
         + x86("mov",     "rcx", "r15")
         + x86("sub",     "rcx", (long)strlen(_.op_name1))
         + x86("cmp",     "rax", "rcx")
         + x86("jg",      "ω")
         + FOR(0, (int)strlen(_.op_name1), [&](int i) -> std::string {
               return x86("mov",   "rcx", "rax")
                    + IF(i != 0, x86("add", "rcx", (long)i))
                    + x86("movzx", "esi", "[r13+rcx]")
                    + x86("cmp64", "rsi", (long)(unsigned char)_.op_name1[i])
                    + x86("jne",   L(1)); })
         + x86("mov",     FRQ(_.op_off), (long)DT_I)
         + x86("add",     "rax", (long)1)
         + x86("mov",     FRQ(_.op_off + 8), "rax")
         + x86("jmp",     "γ")
         + x86("def",     L(1))
         + x86("inc",     FRQ(_.op_off + 16))
         + x86("jmp",     L(0))
         + x86("def",     "β")
         + x86("inc",     FRQ(_.op_off + 16))
         + x86("jmp",     L(0));
}
