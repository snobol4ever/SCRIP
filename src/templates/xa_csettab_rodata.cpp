#include <string>
#include "emit.h"
#include "x86_asm.h"
extern "C" {
#include "xa_template_common.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_csettab_rodata_str(void) {
    if (PLATFORM_X86) {
        if (MEDIUM_MACRO_DEF) return x86("comment", "# no macro form — XA_CSETTAB_RODATA");
        if (MEDIUM_BINARY)    return std::string();
        if (MEDIUM_TEXT) {
            if (g_emit.xa_csettab_n <= 0) return std::string();
            return std::string(".section .rodata\n")
                + emit_for(0, g_emit.xa_csettab_n, [](int i) {
                    return std::string(g_emit.xa_csettab_labels[i])
                         + "\n"
                         + g_emit.xa_csettab_rows[i];
                  })
                + ".text\n";
        }
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void xa_csettab_rodata(void) {
    std::string s = xa_csettab_rodata_str();
    if (!s.empty()) emit_text_n(s.data(), s.size());
}
