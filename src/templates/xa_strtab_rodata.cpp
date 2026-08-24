#include <string>
#include "emit.h"
#include "x86_asm.h"
extern "C" {
#include "xa_template_common.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_strtab_rodata_str(void) {
    if (MEDIUM_MACRO_DEF) return x86("comment", "# no macro form — XA_STRTAB_RODATA");
    if (MEDIUM_BINARY)    return std::string();
    if (MEDIUM_TEXT) {
        if (g_emit.xa_strtab_n <= 0) return std::string();
        return std::string(".section .rodata\n")
            + emit_for(0, g_emit.xa_strtab_n, [](int i) {
                return std::string(g_emit.xa_strtab_labels[i])
                     + " .string "
                     + g_emit.xa_strtab_escaped[i]
                     + "\n";
              })
            + ".text\n";
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void xa_strtab_rodata(void) {
    std::string s = xa_strtab_rodata_str();
    if (!s.empty()) emit_text_n(s.data(), s.size());
}
