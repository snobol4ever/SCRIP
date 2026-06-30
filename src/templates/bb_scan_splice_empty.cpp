#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
void rt_scan_splice_empty(const char *subj_name, int m_start, int m_end);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
/* SPLICE_EMPTY is a pure straight-line action reached by fall-through from the scan's dcap_ok
   block: it splices Σ[m_start..m_end) out of the subject and returns. It has NO ports of its
   own — no α entry label, no β, no ω — so it must NOT emit `def β`/`jmp ω`/`jmp γ` off the
   stale g_emit labels left by flat_drive_match's last FILL (doing so re-defined the match's
   elem_β → assembler "symbol already defined"). flat_drive_scan_native owns the trailing
   `jmp lbl_γ`, mirroring the non-empty-replacement branch. */
std::string bb_scan_splice_empty() {
    if (!PLATFORM_X86) return std::string();
    static char b[24];
    strtab_label(b, sizeof b, _.op_sval ? _.op_sval : "");
    return x86("comment", "SPLICE_EMPTY")
         + x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), b)
         + x86("mov",  "esi", FR(_.op_sa))
         + x86("mov",  "edx", "r14d")
         + x86("push", "rbx")
         + x86("mov",  "rbx", "rsp")
         + x86("and",  "rsp", -16L)
         + x86("call", "rt_scan_splice_empty", (uint64_t)(uintptr_t)(void *)(void(*)(const char*,int,int))rt_scan_splice_empty)
         + x86("mov",  "rsp", "rbx")
         + x86("pop",  "rbx");
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_emit_splice_empty_call(void) { bb_emit_x86(bb_scan_splice_empty()); }
