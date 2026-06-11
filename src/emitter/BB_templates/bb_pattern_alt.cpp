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
static inline std::string  pa_off()  { return std::to_string((long)_.op_off); }
static inline std::string  pa_sa()   { return std::to_string((long)_.op_sa); }
static inline std::string  pa_sb()   { return std::to_string((long)_.op_sb); }
static inline uint64_t     pa_fn()   { void (*fp)(DTP_FRAG_t*,const DTP_FRAG_t*,const DTP_FRAG_t*) = rt_pattern_stitch_alt;
                                       return (uint64_t)(uintptr_t)(void *)fp; }
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_pattern_alt_str() {
    if (PLATFORM_X86)
        return x86("label", _.lbl_α)
             + x86("comment", std::string("BOX PATTERN_ALT  [STITCH ζ=r12 frag@") + pa_off() + " <- @" + pa_sa() + " | @" + pa_sb() + "]")
             + x86("lea",  "rdi", FRQ(_.op_off))
             + x86("lea",  "rsi", FRQ(_.op_sa))
             + x86("lea",  "rdx", FRQ(_.op_sb))
             + x86("push", "rbx")
             + x86("mov",  "rbx", "rsp")
             + x86("and",  "rsp", -16L)
             + x86("call", "rt_pattern_stitch_alt", pa_fn())
             + x86("mov",  "rsp", "rbx")
             + x86("pop",  "rbx")
             + x86("jmp",  "γ")
             + x86("def",  "β")
             + x86("jmp",  "ω");
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pattern_alt(void) { bb_emit_x86(bb_pattern_alt_str()); }
