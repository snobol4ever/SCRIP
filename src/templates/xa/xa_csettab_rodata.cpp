#include <string>
#include "emit.h"
#include "x86_asm.h"
extern "C" {
#include "xa_template_common.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_csettab_rodata_str(void) {
    return IF(MEDIUM_MACRO_DEF, x86("comment", "# no macro form — XA_CSETTAB_RODATA"))
         + IF(!MEDIUM_MACRO_DEF && !MEDIUM_BINARY && MEDIUM_TEXT && g_emit.xa_csettab_n > 0,
               std::string(".section .rodata\n")
             + emit_for(0, g_emit.xa_csettab_n, [](int i) {
                return std::string(g_emit.xa_csettab_labels[i])
                     + "\n"
                     + g_emit.xa_csettab_rows[i];
              })
             + ".text\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void xa_csettab_rodata(void) { emit_text_s(xa_csettab_rodata_str()); }
