#include <string>
#include "emitter/emit_medium.h"
#include "emitter/emit_state_em_03_sh_api.h"
#include "emitter/emit_str.h"
extern "C" {
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_csettab_rodata_str(void) {
    return IF_M4(IF(g_emit.xa_csettab_n > 0,
               std::string(".section .rodata\n")
             + emit_for(0, g_emit.xa_csettab_n, [](int i) {
                return std::string(g_emit.xa_csettab_labels[i])
                     + "\n"
                     + g_emit.xa_csettab_rows[i];
              })
             + ".text\n"));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void xa_csettab_rodata(void) { emit_text_s(xa_csettab_rodata_str()); }
