#include <string>
#include <cstddef>
#include <cstdint>
#include "emit.h"
#include "x86_asm.h"
#include "icn_act.h"
extern "C" {
#include "xa_template_common.h"
}
extern int * const rt_k_level_p;
extern long g_line;
extern const char * g_file;
extern "C" int zls_g_block_args(const IR_graph_t *);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string xa_icn_act_restore_call_line(int lbl) {
    return x86("comment", "the caller's line and file come back at the callee's return or failure, traced or not: g_icn_act's record of this level holds what the call site's statement set")
         + x86("push", "rax")
         + x86("push", "rdx")
         + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&rt_k_level_p, "rt_k_level_p")
         + x86("mov", "rax", RDQ("rax", 0))
         + x86("mov", "ecx", RDD("rax", 0))
         + x86("mov", "rdx", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_icn_act, "g_icn_act")
         + x86("mov", "eax", RDD("rdx", (int)offsetof(cv_t, cap)))
         + x86("cmp", "rcx", "rax")
         + x86("jae", L(lbl))
         + x86("mov", "rax", (long)sizeof(icn_act_rec_t))
         + x86("imul", "rcx", "rax")
         + x86("mov", "rdx", RDQ("rdx", (int)offsetof(cv_t, p)))
         + x86("add", "rdx", "rcx")
         + x86("mov", "rcx", RDQ("rdx", (int)offsetof(icn_act_rec_t, line)))
         + x86("cmp", "rcx", (long)0)
         + x86("jle", L(lbl))
         + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_line, "g_line")
         + x86("mov", RDQ("rax", 0), "rcx")
         + x86("mov", "rcx", RDQ("rdx", (int)offsetof(icn_act_rec_t, file)))
         + x86("test", "rcx", "rcx")
         + x86("je", L(lbl))
         + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_file, "g_file")
         + x86("mov", RDQ("rax", 0), "rcx")
         + x86("def", L(lbl))
         + x86("pop", "rdx")
         + x86("pop", "rax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int xa_icn_block_size(void) {
    return (g_emit_cfg && g_emit_cfg->icn_cells_graph && zls_g_block_args(g_emit_cfg)) ? 16 * g_emit_cfg->nparams : 0;
}
