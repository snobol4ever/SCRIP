#include <string>
#include "emit.h"
#include "x86_asm.h"
extern "C" {
#include "xa_template_common.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_cap_fixup_str(void) {
    if (PLATFORM_X86) {
        if (MEDIUM_MACRO_DEF) return x86("comment", "# no macro form — XA_CAP_FIXUP");
        if (MEDIUM_BINARY)    return std::string();
        if (MEDIUM_TEXT) {
            if (!g_emit.xa_cap_dlbl || !g_emit.xa_cap_dlbl[0]) return std::string();
            if (!g_emit.xa_cap_child_lbl || !g_emit.xa_cap_child_lbl[0]) return std::string();
            return emit_fmt("lea rdi, [rip + %s]\n", g_emit.xa_cap_dlbl)
                 + emit_fmt("lea rsi, [rip + %s]\n", g_emit.xa_cap_child_lbl)
                 + (g_emit.xa_cap_is_arbno
                     ? std::string("call rt_init_arbno@PLT\n")
                     : (g_emit.xa_cap_is_callcap
                         ? (g_emit.xa_cap_varname_lbl && g_emit.xa_cap_varname_lbl[0]
                             ? emit_fmt("lea rdx, [rip + %s]\n", g_emit.xa_cap_varname_lbl)
                             : std::string("xor edx, edx\n"))
                           + "call rt_init_cap_call@PLT\n"
                         : (g_emit.xa_cap_varname_lbl && g_emit.xa_cap_varname_lbl[0]
                             ? emit_fmt("lea rdx, [rip + %s]\n", g_emit.xa_cap_varname_lbl)
                             : std::string("xor edx, edx\n"))
                           + emit_fmt("mov ecx, %d\n", g_emit.xa_cap_immediate)
                           + "call rt_init_cap@PLT\n"));
        }
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void xa_cap_fixup(void) {
    std::string s = xa_cap_fixup_str();
    if (!s.empty()) emit_text_n(s.data(), s.size());
}
