#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
typedef struct { uint64_t ptr; uint64_t len; } ScanSubjRegs;
ScanSubjRegs rt_match_enter(uint64_t lo, uint64_t hi);
void * rt_zls_mark(void);
void   rt_zls_release_to(void *mark);
extern long g_anchor;
}
#include "x86_asm.h"
extern "C" uint64_t g_patstk_sp;
#define hfc() (x86_port_mode() == ZC_PORT_FORTH && _.op_fc_wbytes > 0)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_head() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    return (_.op_sa < 0 || _.op_off < 0)
         ? x86_bomb("IR_MATCH_HEAD: subject/start slot not promoted (emit_drive)")
         : x86("comment", "IR_MATCH_HEAD")
         + x86_alpha()
         + IF(_.flat_deep_arrival, x86("mov", FRQ(_.op_off + 40), "rbp"))   /* BRACKET-GATE (s193): the +40 save exists to bracket the ARBNO zv() borrow (and any deep repoint); a depth-static graph has no repointer, so save AND both restores gate together on the same predicate the outer quartet reads — drift-proof by shared condition. */
         + IF(x86_zc_frame() != ZC_FRAME_RSP, IF(hfc(), x86("sub", "rsp", (long)32))
             + IF(hfc(), x86("call", "rt_zls_mark", (uint64_t)(uintptr_t)(void *)rt_zls_mark)
                       + x86("mov", FRQ(_.op_off + 8), "rax"))
             + (hfc() ? x86("mov", "rax", "rsp")
                      + x86("add", "rax", (long)32)
                      + x86("mov", FRQ(_.op_off + 16), "rax")
                      : x86_zls2_mark_save(_.op_off + 16)))
         + x86("mov", "rdi", FRQ(_.op_sa))
         + x86("mov", "rsi", FRQ(_.op_sa + 8))
         + x86("call", "rt_match_enter", (uint64_t)(uintptr_t)(void *)rt_match_enter)
         + x86("mov", "r13", "rax")
         + x86("mov", "r15", "rdx")
         + x86("mov", "r10", ABSQ(RT_CAS_TOP)) + x86("mov", RDQ("r10", 0), (long)0) + x86("add", "r10", (long)24) + x86("mov", ABSQ(RT_CAS_TOP), "r10")   /* CAS-MARKER (Lon s8 directive: "instantiate a bottom marker at each start of a new pattern match"): the pend stack carries its OWN bracket -- a tag-0 sentinel entry (varname pointers are never 0) pushed at match start.  Replaces the flat +32 slot save: no frame-addressed read, no compile-time depth, config-blind.  RELEASE and the fail exit below scan down to this marker instead of reloading a slot -- the first of the two variable-depth reaches deleted on the road to pure FORTH cells. */
         + IF(x86_zc_frame() == ZC_FRAME_RSP, (hfc() ? x86("mov", "rax", "rsp")
                                                + x86("sub", "rsp", (long)32)
                                                + x86("mov", FRQ(_.op_off + 16), "rax")
                                                : x86_zls2_mark_save(_.op_off + 16))
             + x86("lea", "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&g_patstk_sp, "g_patstk_sp")
             + x86("mov", "rax", RDQ("rcx", 0))
             + x86("mov", FRQ(_.op_off + 8), "rax"))
         + x86("mov", FR(_.op_off), (long)0)
         + x86("def", L(0))
         + x86("mov", "r14d", FR(_.op_off))
         + x86_gamma()
         + x86_beta()
         + x86("add", FR(_.op_off), (long)1)
         + x86("mov", "eax", FR(_.op_off))
         + x86("cmp", "eax", "r15d")
         + x86("jg",  L(1))
         + x86("mov", "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&g_anchor, "g_anchor")
         + x86("mov", "rax", "[rcx]")
         + x86("cmp64", "rax", (long)0)
         + x86("jne", L(1))
         + x86("jmp", L(0))
         + x86("def", L(1))
         + IF(x86_zc_frame() == ZC_FRAME_RSP, (hfc() ? x86("mov", "rax", "qword ptr [rsp + 8]")
                                                : x86("mov", "rax", FRQ(_.op_off + 8)))
             + x86("lea", "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&g_patstk_sp, "g_patstk_sp")
             + x86("mov", RDQ("rcx", 0), "rax"))
         + (hfc() && x86_zc_frame() == ZC_FRAME_RSP
             ? x86("mov", "rsp", "qword ptr [rsp + 16]")
             : x86_zc_frame() == ZC_FRAME_RSP
             ? x86_zls2_release_to_call(_.op_off + 16)
             : ( IF(hfc(), x86("mov", "rdi", FRQ(_.op_off + 8)))
               + x86_align_enter()
               + IF(!hfc(), x86("mov",  "rdi", FRQ(_.op_off + 8)))
               + x86("call", "rt_zls_release_to", (uint64_t)(uintptr_t)(void *)rt_zls_release_to)
               + x86_zls2_release_to_call(_.op_off + 16)
               + x86_align_leave()))
         + x86("mov", "r10", ABSQ(RT_CAS_TOP)) + x86("def", L(2)) + x86("sub", "r10", (long)24) + x86("mov", "rax", RDQ("r10", 0)) + x86("test", "rax", "rax") + x86("jne", L(2)) + x86("mov", ABSQ(RT_CAS_TOP), "r10")   /* CAS-MARKER: fail-exit pops pend entries AND the marker by scanning to tag 0 -- depth-free, replaces the flat +32 reload */
         + IF(_.flat_deep_arrival, x86("mov", "rbp", FRQ(_.op_off + 40)))   /* BRACKET-GATE (s193): restore only if the save above ran */
         + x86_omega();
}
