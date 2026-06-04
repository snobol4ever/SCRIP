#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
extern "C" int  rt_trail_mark(void);
extern "C" void rt_trail_unwind(int mark);
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_query_frame_str() {
    if (PLATFORM_X86) {
        if (_.op_sa == 0) {
            return IF(MEDIUM_TEXT,
                       s_1asm(std::string(_.lbl_α) + ":")
                     + s_comment("# BOX QUERY_FRAME α  [PL-GZ seed ABI: ζ=r12 activation, trail-mark in frame row [ζ+0], x86() self-encoding]"))
                 + x86("push", "r12")
                 + x86("mov", "r12", "rdi")
                 + x86("call", "rt_trail_mark", (uint64_t)(uintptr_t)(void *)rt_trail_mark)
                 + x86("mov", FR(0), "eax")
                 + x86("jmp", PORT_GAMMA);
        }
        return IF(MEDIUM_TEXT,
                   s_comment("# BOX QUERY_FRAME γ/ω landings  [verdict-in-rax: γ=1, ω=trail-unwind+0; ret to the query driver]"))
             + x86("def", PORT_GAMMA)
             + x86("mov32", "eax", 1L)
             + x86("pop", "r12")
             + x86("ret")
             + x86("def", PORT_OMEGA)
             + x86("mov", "edi", FR(0))
             + x86("call", "rt_trail_unwind", (uint64_t)(uintptr_t)(void *)rt_trail_unwind)
             + x86("mov32", "eax", 0L)
             + x86("pop", "r12")
             + x86("ret");
    }
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_query_frame(void) { bb_emit_x86(bb_query_frame_str()); }
