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
static const char * bg_slbl(const char * s) { static char b[64]; b[0] = 0; if (s && *s) strtab_label(b, sizeof b, s); return b; }
static const char * bg_blbl(const char * callee, int arity) { static char b[160]; resolve_call_block_label(b, sizeof b, callee, arity); return b; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string build_arg(int kind, long ival, const char *sval) {
    return !kind ? x86("xor", "eax", "eax") : kind == (int)IR_STRUCT ? emit_build_compound_term((const IR_t *)(intptr_t)ival) :
           x86("mov32", "edi", kind)
         + x86("mov",   "rsi", ival)
         + (bg_slbl(sval)[0] ? x86("lea", "rdx", "[rip + __]", (uint64_t)(uintptr_t)sval, bg_slbl(sval))
                              : x86("xor", "edx", "edx"))
         + x86("xorps", "xmm0", "xmm0")
         + x86("call",  "rt_node_to_term", (uint64_t)(uintptr_t)(void *)rt_node_to_term);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_goal() {
    if (!PLATFORM_X86) return std::string();
    return x86("def",     "α")
         + x86("comment", "IR_GOAL")
         + FOR(0, _.op_parts_n, [&](int i) {
               return build_arg(_.op_parts_tag[i], (long)_.op_parts_ival[i], _.op_parts_str[i])
                    + x86("push", "rax"); })
         + x86("mov32", "edi", _.op_sa + 16)
         + x86("call",  "resolve_bb_env_save_push", (uint64_t)(uintptr_t)(void *)resolve_bb_env_save_push)
         + x86("push",  "rax")
         + IF(((_.op_parts_n + 1) & 1), x86("sub", "rsp", 8))
         + FOR(0, _.op_parts_n, [&](int i) {
               return x86("mov",   "rsi", std::string("[rsp + ") + std::to_string(8 * (((_.op_parts_n + 1) & 1) + 1 + (_.op_parts_n - 1 - i))) + "]")
                    + x86("mov32", "edi", i)
                    + x86("call",  "resolve_bb_bind_arg", (uint64_t)(uintptr_t)(void *)resolve_bb_bind_arg); })
         + x86("call", bg_blbl(_.bb_ls ? _.bb_ls : "", _.op_sa))
         + IF(((_.op_parts_n + 1) & 1), x86("add", "rsp", 8))
         + x86("pop",  "rdi")
         + IF(_.op_parts_n > 0, x86("add", "rsp", 8 * _.op_parts_n))
         + x86("test", "eax", "eax")
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
         + x86("call", std::string(bg_blbl(_.bb_ls ? _.bb_ls : "", _.op_sa)) + "_redo")
         + x86("test", "eax", "eax")
         + x86("je",   L(1))
         + x86("call", "resolve_cp_current", (uint64_t)(uintptr_t)(void *)resolve_cp_current)
         + x86("mov",  "rdi", "[rax + 40]")
         + x86("call", "resolve_bb_env_install", (uint64_t)(uintptr_t)(void *)resolve_bb_env_install)
         + x86("jmp",  "γ")
         + x86("def",  L(1))
         + x86("jmp",  "ω");
}
