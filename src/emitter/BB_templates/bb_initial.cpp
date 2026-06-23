#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
extern int g_descr_flat_chain;
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_initial() {
    if (!PLATFORM_X86) return std::string();
    x86_begin();
    return x86("comment", "IR_INITIAL done-flag + branch")
         + x86("def",   L(0))
         + x86(".quad", 0ULL)
         + x86("label", _.lbl_α)
         + x86("mov",   "rax", ROQ(0))
         + x86("test",  "rax", "rax")
         + x86("jne",   "γ")
         + x86_pair_jmp(1)
         + x86("def",   L(2))
         + x86("mov",   ROQ(0), 1L)
         + x86_pair_jmp(2)
         + x86("def",   "β")
         + x86_pair_jmp(3)
         + x86_ro_seal_q(0, 0ULL);
}
