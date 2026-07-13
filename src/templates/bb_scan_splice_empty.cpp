#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
void rt_scan_splice_empty(const char *subj_name, int m_start, int m_end);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_scan_splice_empty() {
    if (!PLATFORM_X86) return std::string();
    static char b[24];
    strtab_label(b, sizeof b, _.op_sval ? _.op_sval : "");
    return x86("comment", "SPLICE_EMPTY")
         + x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), b)
         + x86("mov",  "esi", FR(_.op_sa))
         + x86("mov",  "edx", "r14d")
         + x86_align_enter()
         + x86("call", "rt_scan_splice_empty", (uint64_t)(uintptr_t)(void *)(void(*)(const char*,int,int))rt_scan_splice_empty)
         + x86_align_leave();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_emit_splice_empty_call(void) { bb_emit_x86(bb_scan_splice_empty()); }
