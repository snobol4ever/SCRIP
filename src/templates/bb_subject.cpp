#include <string>
#include <cstring>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
#include "x86_asm.h"
extern "C" void rt_subject_load_nv(const char *name, void *slot);
extern "C" void rt_subject_load_lit(const char *s, void *slot);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_subject() {
    x86_begin();
    if (PLATFORM_X86) {
        static char nb[24];
        static char lb[24];
        return _.op_sa < 0 ? x86_alpha() + x86_bomb("IR_SUBJECT: subject slot not promoted (flat_drive_subject)") :
               (!_.op_a_sval && _.op_sval && _.op_sval[0]) ?
               ( strtab_label(nb, sizeof nb, _.op_sval ? _.op_sval : ""),
                 x86("comment", "IR_SUBJECT")
               + x86_alpha()
               + x86("lea",     "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), nb)
               + x86("lea",     "rsi", FR(_.op_sa))
               + x86("sub",     "rsp", (long)8)
               + x86_align_enter()
               + x86("call",    "rt_subject_load_nv", (uint64_t)(uintptr_t)(void *)(void (*)(const char *, void *))rt_subject_load_nv)
               + x86_align_leave()
               + x86("add",     "rsp", (long)8)
               + x86_gamma()
               + x86_beta_trampoline() ) :
               !_.op_a_sval ? x86_alpha() + x86_bomb("IR_SUBJECT: non-literal subject (PB-RB ladder)") :
               ( strtab_label(lb, sizeof lb, _.op_a_sval ? _.op_a_sval : ""),
                 x86("comment", "IR_SUBJECT")
               + x86_alpha()
               + x86("lea",     "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_a_sval ? _.op_a_sval : ""), lb)
               + x86("lea",     "rsi", FR(_.op_sa))
               + x86("sub",     "rsp", (long)8)
               + x86_align_enter()
               + x86("call",    "rt_subject_load_lit", (uint64_t)(uintptr_t)(void *)(void (*)(const char *, void *))rt_subject_load_lit)
               + x86_align_leave()
               + x86("add",     "rsp", (long)8)
               + x86_gamma()
               + x86_beta_trampoline() );
    }
    return std::string();
}
