#include <string>
#include <cstdint>
#include "emit.h"
#include "x86_asm.h"
extern "C" {
#include "xa_template_common.h"
}
extern "C" int zls_g_block_args(const IR_graph_t *);
extern long g_line;
extern const char * g_file;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string icn_landing_line_restore(void) {
    if (g_emit.icn_line_cur <= 0) return std::string();
    return x86("mov", "rcx", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_line, "g_line")
         + x86("mov", RDQ("rcx", 0), (long)g_emit.icn_line_cur)
         + IF(g_emit.icn_file_cur && *g_emit.icn_file_cur,
               x86_load_ro_str("r11", g_emit.icn_file_cur)
             + x86("mov", "rcx", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_file, "g_file")
             + x86("mov", RDQ("rcx", 0), "r11"));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int xa_icn_block_size(void) {
    return (g_emit_cfg && g_emit_cfg->icn_cells_graph && zls_g_block_args(g_emit_cfg)) ? 16 * g_emit_cfg->nparams : 0;
}
