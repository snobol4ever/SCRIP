#include <string>
#include "emit.h"
#include "x86_asm.h"
extern "C" {
#include "xa_template_common.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_cap_fixup_text(void) {
    return IF(g_emit.xa_cap_dlbl && g_emit.xa_cap_dlbl[0] && g_emit.xa_cap_child_lbl && g_emit.xa_cap_child_lbl[0],
              std::string("lea rdi, [rip + ") + g_emit.xa_cap_dlbl + "]\n"
            + "lea rsi, [rip + " + g_emit.xa_cap_child_lbl + "]\n"
            + IF(g_emit.xa_cap_is_arbno, std::string("call rt_init_arbno@PLT\n"))
            + IF(!g_emit.xa_cap_is_arbno,
                   IF(g_emit.xa_cap_varname_lbl && g_emit.xa_cap_varname_lbl[0], std::string("lea rdx, [rip + ") + (g_emit.xa_cap_varname_lbl ? g_emit.xa_cap_varname_lbl : "") + "]\n")
                 + IF(!(g_emit.xa_cap_varname_lbl && g_emit.xa_cap_varname_lbl[0]), std::string("xor edx, edx\n"))
                 + IF(g_emit.xa_cap_is_callcap, std::string("call rt_init_cap_call@PLT\n"))
                 + IF(!g_emit.xa_cap_is_callcap, std::string("mov ecx, ") + std::to_string(g_emit.xa_cap_immediate) + "\n" + "call rt_init_cap@PLT\n")));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void xa_cap_fixup(void) {
    bb_emit_x86(xa_cap_fixup_text());
}
