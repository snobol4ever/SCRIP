#include <string>
#include <cstdint>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
void rt_gvar_assign_str(const char * name, const char * str);
void rt_gvar_assign_concat_parts(const char * dst, void * parts, int n);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string g_parts_str;
static char gc_b[64];
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_gvar_assign_concat_build_parts() {
    g_parts_str.clear();
    for (int i = 0; i < _.op_parts_n; i++) {
        const char *pl = emit_intern_str(_.op_parts_str[i] ? _.op_parts_str[i] : "");
        if (!pl) { strtab_label(gc_b, sizeof gc_b, _.op_parts_str[i] ? _.op_parts_str[i] : ""); pl = gc_b; }
        g_parts_str += x86("mov", FR(_.op_off + 16 * i),      (long)_.op_parts_tag[i])
                     + x86("lea", "rax", "[rip + __]",         (uint64_t)(uintptr_t)(_.op_parts_str[i] ? _.op_parts_str[i] : ""), pl)
                     + x86("mov", FRQ(_.op_off + 16 * i + 8), "rax");
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_gvar_assign_concat() {
    if (PLATFORM_X86) return _.bb_rs
        ? x86("label",  _.lbl_α)
        + x86("comment","IR_ASSIGN_CONCAT lit_s")
        + x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(_.op_sval           ? _.op_sval           : ""), _.bb_ls)
        + x86("lea",  "rsi", "[rip + __]", (uint64_t)(uintptr_t)(_.op_parts_str[0]   ? _.op_parts_str[0]   : ""), _.bb_rs)
        + x86("call", "rt_gvar_assign_str", (uint64_t)(uintptr_t)(void *)(void (*)(const char *, const char *))rt_gvar_assign_str)
        + x86("jmp",  "γ")
        + x86("def",  "β")
        + x86("jmp",  "ω")
        : _.op_parts_n <= 0 ? x86_bomb("bb_gvar_assign_concat: no parts (not flattenable)")
        : (bb_gvar_assign_concat_build_parts(),
           x86("label",   _.lbl_α)
         + x86("comment", std::string("IR_ASSIGN_CONCAT ") + std::to_string(_.op_parts_n) + " parts")
         + g_parts_str
         + x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)_.bb_ls, _.bb_ls)
         + x86("lea",  "rsi", FR(_.op_off))
         + x86("mov",  "edx", (long)_.op_parts_n)
         + x86("push", "r10")
         + x86("push", "rbx")
         + x86("mov",  "rbx", "rsp")
         + x86("and",  "rsp", -16L)
         + x86("call", "rt_gvar_assign_concat_parts", (uint64_t)(uintptr_t)(void *)(void (*)(const char *, void *, int))rt_gvar_assign_concat_parts)
         + x86("mov",  "rsp", "rbx")
         + x86("pop",  "rbx")
         + x86("pop",  "r10")
         + x86("jmp",  "γ")
         + x86("def",  "β")
         + x86("jmp",  "ω"));
    return std::string();
}
