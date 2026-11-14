#include <string>
#include "emitter/emit_medium.h"
#include "emitter/emit_state_em_03_sh_api.h"
#include "emitter/emit_str.h"
extern "C" {
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_strtab_rodata_str(void) {
    return IF_M4(IF(g_emit.xa_strtab_n > 0,
               std::string(".section .rodata\n")
             + emit_for(0, g_emit.xa_strtab_n, [](int i) {
                return std::string(g_emit.xa_strtab_labels[i])
                     + " .string "
                     + g_emit.xa_strtab_escaped[i]
                     + "\n";
              })
             + ".text\n"));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void xa_strtab_rodata(void) { emit_text_s(xa_strtab_rodata_str()); }
