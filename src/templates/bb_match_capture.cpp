#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
extern "C" long rt_cap_open(const char *varname, int saved_delta, int cur_delta, int is_imm);
extern "C" void *rt_frame_prep(void *fb, long fbytes);
extern "C" void rt_cap_finish(DESCR_t fret);
extern "C" void rt_cap_push(void *slot, int delta);
extern "C" void rt_cap_pop(void *slot);
extern "C" int rt_cap_top(void *slot);
extern "C" void rt_dcap_pop(void);
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_capture() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    static char b[24];
    return (_.op_off < 0)
         ? ( x86_alpha()
           + x86_bomb("IR_MATCH_ASSIGN: capture stack slot not promoted (flat_drive_capture)") )
         : !(_.op_sval ? _.op_sval : "")[0]
         ? ( x86_alpha()
           + x86_bomb("IR_MATCH_ASSIGN: empty capture variable name") )
         : (int)_.op_phase == 0
         ? ( x86("comment", "IR_MATCH_CAPTURE_SAVE push")
           + x86_alpha()
           + x86("lea",  "rdi", FR(_.op_off))
           + x86("mov",  "esi", "r14d")
           + x86_align_enter()
           + x86("call", "rt_cap_push", (uint64_t)(uintptr_t)(void *)(void (*)(void *, int))rt_cap_push)
           + x86_align_leave()
           + x86_gamma()
           + x86_beta()
           + x86("lea",  "rdi", FR(_.op_off))
           + x86_align_enter()
           + x86("call", "rt_cap_pop", (uint64_t)(uintptr_t)(void *)(void (*)(void *))rt_cap_pop)
           + x86_align_leave()
           + x86_omega() )
         : ( x86("comment", (int)_.op_phase == 2 ? "IR_MATCH_CAPTURE_IMM" : "IR_MATCH_CAPTURE_COND")
           + x86_alpha()
           + x86_align_enter()
           + x86("lea",  "rdi", FR(_.op_off))
           + x86("call", "rt_cap_top", (uint64_t)(uintptr_t)(void *)(int (*)(void *))rt_cap_top)
           + x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), (strtab_label(b, sizeof b, (_.op_sval ? _.op_sval : "")), b))
           + x86("mov",  "esi", "eax")
           + x86("mov",  "edx", "r14d")
           + x86("mov",  "ecx", (long)((int)_.op_phase == 2 ? 1 : 0))
           + x86("call", "rt_cap_open", (uint64_t)(uintptr_t)(void *)(long (*)(const char *, int, int, int))rt_cap_open)
           + x86("test", "rax", "rax")
           + x86("je",   L(1))
           + x86("sub",  "rsp", "rax")
           + x86("mov",  "rdi", "rsp")
           + x86("mov",  "rsi", "rax")
           + x86("call", "rt_frame_prep", (uint64_t)(uintptr_t)(void *)(void *(*)(void *, long))rt_frame_prep)
           + x86("mov",  "rdi", "rsp")
           + x86("xor",  "esi", "esi")
           + x86("call", "rax")
           + x86("mov",  "rdi", "rax")
           + x86("mov",  "rsi", "rdx")
           + x86("call", "rt_cap_finish", (uint64_t)(uintptr_t)(void *)(void (*)(DESCR_t))rt_cap_finish)
           + x86("def",  L(1))
           + x86_align_leave()
           + x86_gamma()
           + x86_beta()
           + IF((int)_.op_phase == 1,
                 x86_align_enter()
               + x86("call", "rt_dcap_pop", (uint64_t)(uintptr_t)(void *)(void (*)(void))rt_dcap_pop)
               + x86_align_leave())
           + x86_omega() );
}
