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
static const char * bb_subj_nlbl() {
    static char b[24];
    if (emit_intern_str(_.op_sval ? _.op_sval : "")) return emit_intern_str(_.op_sval ? _.op_sval : "");
    strtab_label(b, sizeof b, _.op_sval ? _.op_sval : ""); return b;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_subject_nv_arm() {
    return x86("comment", "IR_SUBJECT")
         + x86_alpha()
         + x86("lea",     "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), bb_subj_nlbl())
         + x86("lea",     "rsi", FR(_.op_sa))
         + x86("push",    "r10")
         + x86_align_enter()
         + x86("call",    "rt_subject_load_nv", (uint64_t)(uintptr_t)(void *)(void (*)(const char *, void *))rt_subject_load_nv)
         + x86_align_leave()
         + x86("pop",     "r10")
         + x86_gamma()
         + x86_beta()
         + x86_omega();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * bb_subj_litlbl() {
    static char b[24];
    const char * s = _.op_a_sval ? _.op_a_sval : "";
    const char * l = emit_intern_str(s);
    if (l) return l;
    strtab_label(b, sizeof b, s); return b;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_subject_lit_arm() {
    return x86("comment", "IR_SUBJECT")
         + x86_alpha()
         + x86("lea",     "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_a_sval ? _.op_a_sval : ""), bb_subj_litlbl())
         + x86("lea",     "rsi", FR(_.op_sa))
         + x86("push",    "r10")
         + x86_align_enter()
         + x86("call",    "rt_subject_load_lit", (uint64_t)(uintptr_t)(void *)(void (*)(const char *, void *))rt_subject_load_lit)
         + x86_align_leave()
         + x86("pop",     "r10")
         + x86_gamma()
         + x86_beta()
         + x86_omega();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_subject() {
    x86_begin();
    if (PLATFORM_X86)
        return _.op_sa < 0 ? x86_alpha() + x86_bomb("IR_SUBJECT: subject slot not promoted (flat_drive_subject)") :
               (!_.op_a_sval && _.op_sval && _.op_sval[0]) ? bb_subject_nv_arm() :
               !_.op_a_sval ? x86_alpha() + x86_bomb("IR_SUBJECT: non-literal subject (PB-RB ladder)") :
               bb_subject_lit_arm();
    return std::string();
}
