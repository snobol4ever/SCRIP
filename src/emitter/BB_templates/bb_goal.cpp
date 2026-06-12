#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
}
#include "x86_asm.h"
extern "C" {
void  *rt_node_to_term(int kind, long ival, const char *sval, double dval);
void **resolve_bb_env_save_push(int nslots);
void   resolve_bb_bind_arg(int slot, void *caller_term);
void **resolve_bb_env_install(void **env);
void   rt_cp_save_caller_env(void *caller_env);
void   resolve_bb_env_pop(void **saved);
void  *resolve_cp_current(void);
}
extern std::string emit_build_compound_term(const IR_t *nd);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string build_arg(int kind, long ival, const char *sval) {
    if (!kind) return x86("xor", "eax", "eax");
    if (kind == IR_STRUCT) return emit_build_compound_term((const IR_t *)(intptr_t)ival);
    char slbl[64]; slbl[0] = 0;
    if (sval && *sval) strtab_label(slbl, sizeof slbl, sval);
    return x86("mov32", "edi", (int)kind)
         + x86("mov",   "rsi", ival)
         + (slbl[0] ? x86("lea", "rdx", "[rip + __]", (uint64_t)(uintptr_t)sval, slbl)
                    : x86("xor", "edx", "edx"))
         + x86("xorps", "xmm0", "xmm0")
         + x86("call",  "rt_node_to_term", (uint64_t)(uintptr_t)(void *)rt_node_to_term);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_goal() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    const char *callee = _.bb_ls ? _.bb_ls : "";
    int arity  = _.op_sa;
    int n_args = _.op_parts_n;
    char blbl[160]; resolve_call_block_label(blbl, sizeof blbl, callee, arity);
    std::string redo_s = std::string(blbl) + "_redo";
    int nslots = arity + 16;
    std::string out = x86("def",     "α")
                    + x86("comment", "IR_GOAL");
    for (int i = 0; i < n_args; i++)
        out += build_arg(_.op_parts_tag[i], (long)_.op_parts_ival[i], _.op_parts_str[i])
             + x86("push", "rax");
    int need_pad = ((n_args + 1) & 1) ? 1 : 0;
    out += x86("mov32", "edi", nslots)
         + x86("call",  "resolve_bb_env_save_push", (uint64_t)(uintptr_t)(void *)resolve_bb_env_save_push)
         + x86("push",  "rax");
    if (need_pad) out += x86("sub", "rsp", 8);
    for (int i = n_args - 1; i >= 0; i--)
        out += x86("mov",   "rsi", std::string("[rsp + ") + std::to_string(8 * (need_pad + 1 + (n_args - 1 - i))) + "]")
             + x86("mov32", "edi", i)
             + x86("call",  "resolve_bb_bind_arg", (uint64_t)(uintptr_t)(void *)resolve_bb_bind_arg);
    out += x86("call", blbl);
    if (need_pad) out += x86("add", "rsp", 8);
    out += x86("pop",  "rdi");
    if (n_args > 0) out += x86("add", "rsp", 8 * n_args);
    out += x86("test", "eax", "eax")
         + x86("je",   L(0))
         + x86("call", "resolve_bb_env_install", (uint64_t)(uintptr_t)(void *)resolve_bb_env_install)
         + x86("mov",  "rdi", "rax")
         + x86("call", "rt_cp_save_caller_env",  (uint64_t)(uintptr_t)(void *)rt_cp_save_caller_env)
         + x86("jmp",  "γ")
         + x86("def",  L(0))
         + x86("call", "resolve_bb_env_pop", (uint64_t)(uintptr_t)(void *)resolve_bb_env_pop)
         + x86("jmp",  "ω")
         + x86("def",  "β")
         + x86("call", "resolve_cp_current", (uint64_t)(uintptr_t)(void *)resolve_cp_current)
         + x86("test", "rax", "rax")
         + x86("je",   L(1))
         + x86("mov",  "rdi", "[rax + 24]")
         + x86("call", "resolve_bb_env_install", (uint64_t)(uintptr_t)(void *)resolve_bb_env_install)
         + x86("call", redo_s)
         + x86("test", "eax", "eax")
         + x86("je",   L(1))
         + x86("call", "resolve_cp_current", (uint64_t)(uintptr_t)(void *)resolve_cp_current)
         + x86("mov",  "rdi", "[rax + 40]")
         + x86("call", "resolve_bb_env_install", (uint64_t)(uintptr_t)(void *)resolve_bb_env_install)
         + x86("jmp",  "γ")
         + x86("def",  L(1))
         + x86("jmp",  "ω");
    return out;
}