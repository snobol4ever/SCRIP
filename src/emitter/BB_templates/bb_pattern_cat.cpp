#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit_bb.h"
#include "emit.h"
#include "dtp.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_pattern_cat() {
    if (!PLATFORM_X86) return std::string();
    uint64_t fn; { void (*fp)(DTP_FRAG_t*,const DTP_FRAG_t*,const DTP_FRAG_t*) = rt_pattern_stitch_cat; fn=(uint64_t)(uintptr_t)(void*)fp; }
    return x86("comment", "IR_PATTERN_CAT")
         + x86("label",   _.lbl_α)
         + x86("lea",     "rdi", FRQ(_.op_off))
         + x86("lea",     "rsi", FRQ(_.op_sa))
         + x86("lea",     "rdx", FRQ(_.op_sb))
         + x86("push",    "rbx")
         + x86("mov",     "rbx", "rsp")
         + x86("and",     "rsp", -16L)
         + x86("call",    "rt_pattern_stitch_cat", fn)
         + x86("mov",     "rsp", "rbx")
         + x86("pop",     "rbx")
         + x86("jmp",     "γ")
         + x86("def",     "β")
         + x86("jmp",     "ω");
}
