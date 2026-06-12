#include <string>
#include <cstring>
#include <cstdint>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void rt_subject_load_nv(const char *name, void *slot);
static inline const char * subj_chars() { return _.op_a_sval; }
static inline const char * subj_label() { return emit_intern_str(subj_chars() ? subj_chars() : ""); }
static inline uint64_t     subj_addr()  { return (uint64_t)(uintptr_t)(const void *)subj_chars(); }
static inline const char * subj_name()  { return _.op_sval ? _.op_sval : ""; }
static inline const char * subj_nlbl()  { const char * l = emit_intern_str(subj_name()); if (l) return l;
                                          static char b[24]; strtab_label(b, sizeof b, subj_name()); return b; }
static inline uint64_t     subj_naddr() { return (uint64_t)(uintptr_t)(const void *)subj_name(); }
static inline uint64_t     subj_nv_fn() { void (*fp)(const char *, void *) = rt_subject_load_nv;
                                          return (uint64_t)(uintptr_t)(void *)fp; }
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_subject() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    if (_.op_sa < 0) return x86_bomb("IR_SUBJECT: subject slot not promoted (flat_drive_subject)");
    if (!subj_chars() && subj_name()[0]) {
        return IF(MEDIUM_TEXT,
                   x86("label", _.lbl_α)
                 + x86("comment", std::string("BOX SUBJECT VAR(") + subj_name() + ")  [NV -> sigma ptr + Delta len -> zeta-slot; sets runtime Sigma]"))
             + x86("lea",  "rdi", "[rip + __]", subj_naddr(), subj_nlbl())
             + x86("lea",  "rsi", FR(_.op_sa))
             + x86("push", "r10")
             + x86("push", "rbx")
             + x86("mov",  "rbx", "rsp")
             + x86("and",  "rsp", -16L)
             + x86("call", "rt_subject_load_nv", subj_nv_fn())
             + x86("mov",  "rsp", "rbx")
             + x86("pop",  "rbx")
             + x86("pop",  "r10")
             + x86("jmp",  "γ")
             + x86("def",  "β")
             + x86("jmp",  "ω");
    }
    if (!subj_chars()) return x86_bomb("IR_SUBJECT: non-literal subject (PB-RB ladder)");
    return IF(MEDIUM_TEXT,
               x86("label", _.lbl_α)
             + x86("comment", "BOX SUBJECT  [REG-0 sigma ptr + Delta len -> zeta-slot, x86() self-encoding]"))
         + x86("mov", "rax", "[rip + __]", subj_addr(), subj_label())
         + x86("mov", FRQ(_.op_sa), "rax")
         + x86("mov", FR(_.op_sa + 8), (long)strlen(subj_chars()))
         + x86("jmp", "γ")
         + x86("def", "β")
         + x86("jmp", "ω");
}