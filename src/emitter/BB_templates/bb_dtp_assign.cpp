#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit_bb.h"
#include "emit.h"
#include "dtp.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static inline const char * da_name()  { return _.op_sval ? _.op_sval : ""; }
static inline const char * da_label() { const char * l = emit_intern_str(da_name()); if (l) return l;
                                        static char b[24]; strtab_label(b, sizeof b, da_name()); return b; }
static inline uint64_t     da_addr()  { return (uint64_t)(uintptr_t)(const void *)da_name(); }
static inline std::string  da_sa()    { return std::to_string((long)_.op_sa); }
static inline uint64_t     da_fn()    { void (*fp)(DTP_FRAG_t*,const char*) = rt_dtp_head_build;
                                        return (uint64_t)(uintptr_t)(void *)fp; }
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_dtp_assign_str() {
    if (PLATFORM_X86)
        return x86("label", _.lbl_α)
             + x86("comment", std::string("BOX DTP_ASSIGN('") + da_name() + "')  [HEAD ζ=r12 frag@" + da_sa() + "]")
             + x86("lea",  "rdi", FRQ(_.op_sa))
             + x86("lea",  "rsi", "[rip + __]", da_addr(), da_label())
             + x86("push", "rbx")
             + x86("mov",  "rbx", "rsp")
             + x86("and",  "rsp", -16L)
             + x86("call", "rt_dtp_head_build", da_fn())
             + x86("mov",  "rsp", "rbx")
             + x86("pop",  "rbx")
             + x86("jmp",  "γ")
             + x86("def",  "β")
             + x86("jmp",  "ω");
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_dtp_assign(void) { bb_emit_x86(bb_dtp_assign_str()); }
