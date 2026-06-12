#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "dtp.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_dtp_assign_str() {
    if (PLATFORM_X86)
        return x86("label", _.lbl_α)
             + x86("comment", "IR_DTP_ASSIGN")
             + x86("lea",  "rdi", FRQ(_.op_sa))
             + x86("lea",  "rsi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), _.bb_ls)
             + x86("push", "rbx")
             + x86("mov",  "rbx", "rsp")
             + x86("and",  "rsp", -16L)
             + x86("call", "rt_dtp_head_build", (uint64_t)(uintptr_t)(void *)rt_dtp_head_build)
             + x86("mov",  "rsp", "rbx")
             + x86("pop",  "rbx")
             + x86("jmp",  "γ")
             + x86("def",  "β")
             + x86("jmp",  "ω");
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_dtp_assign(void) { bb_emit_x86(bb_dtp_assign_str()); }
