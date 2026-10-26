#include <string>
#include "g_runtime.h"
#include <cstdint>
#include "emit.h"
#include "x86_asm.h"
extern "C" {
#include "xa_template_common.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string icn_landing_line_restore(void) {
    return IF(g_emit.icn_line_cur > 0,
               x86("mov", "rcx", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_runtime, "g_runtime")
             + x86("mov", RDQ("rcx", GRT_AT(line)), (long)g_emit.icn_line_cur)
             + IF(g_emit.icn_file_cur && *g_emit.icn_file_cur,
                   x86_load_ro_str("r11", g_emit.icn_file_cur)
                 + x86("mov", "rcx", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_runtime, "g_runtime")
                 + x86("mov", RDQ("rcx", GRT_AT(file)), "r11")));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int xa_icn_block_size(void) {
    return (g_emit_cfg && g_emit_cfg->cell_frame && g_emit_cfg->pf.block_args) ? 16 * g_emit_cfg->nparams : 0;
}
