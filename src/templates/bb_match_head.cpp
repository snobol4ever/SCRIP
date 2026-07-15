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
extern const char *g_dcap_top;
extern long g_anchor;
}
#include "x86_asm.h"
/* ZB-FC-3d (partition ruling, s49): under the statement grant (op_fc_wbytes window, hook dormant) HEAD self-pushes a 32-byte rsp cell after alpha holding the three MATCH-SPAN-lifetime fields -- anchor
 * @+0, zls mark @+8, rsp mark @+16 -- which the window rebases with zero arithmetic below (every FR/FRQ in [op_off, op_off+24) becomes [rsp+k]; +32/+40 fall outside and stay FLAT, exactly right: the
 * dcap mark and saved rbp have POST-UNWIND consumers).  The rsp mark = the PRE-PUSH rsp (mov rax,rsp; add rax,32 -- lea [rsp+K] deliberately avoided: x86_reg_disp32_lea64 emits no SIB so an rsp base
 * mis-encodes in BINARY), so the existing zls2 unwind releases this cell and every suspended pattern cell in one mov, at BOTH statement exits, unchanged.  The flat +0 write disappears under grant --
 * RELEASE re-homes the match START there pre-unwind for REPLACE (the splice-survivor partition).  L(1)'s zls-mark read hoists BEFORE align_enter (a window read inside the align dance is wrong -- the
 * 3c lesson); rdi rides the pushes.  Ungranted: every line byte-verbatim today's path (degrade never die). */
static inline int hfc() { return x86_port_mode() == ZC_PORT_FORTH && _.op_fc_wbytes > 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_head() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    return (_.op_sa < 0 || _.op_off < 0)
         ? x86_bomb("IR_MATCH_HEAD: subject/start slot not promoted (emit_drive)")
         : x86("comment", "IR_MATCH_HEAD")
         + x86_alpha()
         + IF(hfc(), x86("sub", "rsp", (long)32))
         + IF(hfc() && ZC_FRAME != ZC_FRAME_RSP, x86("call", "rt_zls_mark", (uint64_t)(uintptr_t)(void *)rt_zls_mark) + x86("mov", FRQ(_.op_off + 8), "rax"))
         /* R12-ERAD: under RSP the FORTH cell base (rsp after sub) IS the ζ mark — rsp+16 already holds it; slot+8 (arena mark) unused */
         + (hfc() ? x86("mov", "rax", "rsp") + x86("add", "rax", (long)32) + x86("mov", FRQ(_.op_off + 16), "rax")
                  : x86_zls2_mark_save(_.op_off + 16))
         + x86("mov", "rdi", FRQ(_.op_sa))
         + x86("mov", "rsi", FRQ(_.op_sa + 8))
         + x86("call", "rt_match_enter", (uint64_t)(uintptr_t)(void *)rt_match_enter)
         + x86("mov", "r13", "rax")
         + x86("mov", "r15", "rdx")
         /* rbp-dcap α: save the C caller's rbp (callee-saved contract — graphs stay rbp-free, match spans own
          * the register), load the live pend cursor from the mirror (rt_match_enter just lazy-init'd the
          * island), and save it as this match's MARK.  Nested heads see the outer's live top because every
          * mid-match transfer window in a match-family box mirrors rbp out first. */
         + x86("mov", FRQ(_.op_off + 40), "rbp")
         + x86("mov", "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&g_dcap_top, "g_dcap_top")
         + x86("mov", "rbp", RDQ("rcx", 0))
         + x86("mov", FRQ(_.op_off + 32), "rbp")
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
         /* R12-ERAD (ZC_FRAME_RSP + hfc): cell's rsp-mark lives at [rsp+16] (32B FORTH cell field +16).
          * No align dance open — bare mov rsp,[rsp+16] pops the 32B cell and restores pre-HEAD rsp. */
         + (hfc() && ZC_FRAME == ZC_FRAME_RSP
             ? x86("mov", "rsp", "qword ptr [rsp + 16]")
             : ( IF(hfc(), x86("mov", "rdi", FRQ(_.op_off + 8)))
               + x86_align_enter()
               + IF(!hfc(), x86("mov",  "rdi", FRQ(_.op_off + 8)))
               + x86("call", "rt_zls_release_to", (uint64_t)(uintptr_t)(void *)rt_zls_release_to)
               + x86_zls2_release_to_call(_.op_off + 16)
               + x86_align_leave()))
         /* rbp-dcap ω (all anchors exhausted): truncate = store mirror ← MARK, then restore the C caller's
          * rbp.  This IS the old rt_dcap_end_fail, inline — the depth array died with the ring. */
         + x86("mov", "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&g_dcap_top, "g_dcap_top")
         + x86("mov", "rax", FRQ(_.op_off + 32))
         + x86("mov", RDQ("rcx", 0), "rax")
         + x86("mov", "rbp", FRQ(_.op_off + 40))
         + x86_omega();
}
