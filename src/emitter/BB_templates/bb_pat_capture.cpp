#include <string>
#include <cstdint>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
}
extern "C" void rt_cap_assign_cursor(const char *varname, int saved_delta, int cur_delta, int is_imm);
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static inline int          sub_kind()  { return (int)_.op_ival; }
static inline int          stoff()     { return _.op_off; }
static inline const char * vname()     { return _.op_sval ? _.op_sval : ""; }
static inline const char * vlabel()    { const char * l = emit_intern_str(vname()); if (l) return l;
                                         static char b[24]; strtab_label(b, sizeof b, vname()); return b; }
static inline uint64_t     vaddr()     { return (uint64_t)(uintptr_t)(const void *)vname(); }
static inline uint64_t     cap_fn()    { void (*fp)(const char *, int, int, int) = rt_cap_assign_cursor;
                                         return (uint64_t)(uintptr_t)(void *)fp; }
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_pat_capture_str() {
    if (!PLATFORM_X86) return std::string();
    if (stoff() < 0) return x86_bomb("IR_PAT_ASSIGN: start slot not promoted (flat_drive_capture)");
    if (!vname()[0]) return x86_bomb("IR_PAT_ASSIGN: empty capture variable name");
    if (sub_kind() == 0) {
        return IF(MEDIUM_TEXT,
                   s_1asm(std::string(_.lbl_α) + ":")
                 + s_comment(emit_fmt("# BOX CAPTURE SAVE(%s)  [delta=r14 -> zeta-slot; falls through into the inline child]", vname())))
             + x86("mov", FR(stoff()), "r14d");
    }
    return IF(MEDIUM_TEXT,
               s_comment(emit_fmt("# BOX CAPTURE %s(%s)  [SPITBOL ch.13: matched substring Sigma[saved..delta) -> variable]",
                                   sub_kind() == 2 ? "IMM" : "COND", vname())))
         + x86("lea",  "rdi", "[rip + __]", vaddr(), vlabel())
         + x86("mov",  "esi", FR(stoff()))
         + x86("mov",  "edx", "r14d")
         + x86("mov",  "ecx", (long)(sub_kind() == 2 ? 1 : 0))
         + x86("push", "r10")
         + x86("push", "rbx")
         + x86("mov",  "rbx", "rsp")
         + x86("and",  "rsp", -16L)
         + x86("call", "rt_cap_assign_cursor", cap_fn())
         + x86("mov",  "rsp", "rbx")
         + x86("pop",  "rbx")
         + x86("pop",  "r10")
         + x86("jmp",  PORT_GAMMA);
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pat_capture(void) {
    x86_begin();
    bb_emit_x86(bb_pat_capture_str());
}
