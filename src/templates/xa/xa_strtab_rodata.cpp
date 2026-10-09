#include <string>
#include "emit.h"
#include "x86_asm.h"
extern "C" {
#include "xa_template_common.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_strtab_rodata_str(void) {
    return IF(MEDIUM_MACRO_DEF, x86("comment", "# no macro form — XA_STRTAB_RODATA"))
         + IF(!MEDIUM_MACRO_DEF && !MEDIUM_BINARY && MEDIUM_TEXT && g_emit.xa_strtab_n > 0,
               std::string(".section .rodata\n")
             + emit_for(0, g_emit.xa_strtab_n, [](int i) {
                return std::string(g_emit.xa_strtab_labels[i])
                     + " .string "
                     + g_emit.xa_strtab_escaped[i]
                     + "\n";
              })
             + ".text\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void xa_strtab_rodata(void) { emit_text_s(xa_strtab_rodata_str()); }
