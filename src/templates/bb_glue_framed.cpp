#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_framed_enter() {
    if (!PLATFORM_X86) return std::string();
    return std::string("")
         + std::string("")
         + x86("sub", "rsp", ((_.op_fc_bytes + 15) & ~15L));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_framed_leave() {
    if (!PLATFORM_X86) return std::string();
    return x86("add", "rsp", ((_.op_fc_bytes + 15) & ~15L));
    return std::string("")
         + std::string("");
}
