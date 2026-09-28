#include <string>
#include <cstdint>
#include <cstdlib>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
typedef struct { long fn; long how; } rt_dcap_next_t;
extern "C" rt_dcap_next_t rt_dcap_end_ok_open(const char *mark, const char *top, const char *subj);
extern "C" rt_dcap_next_t rt_dcap_land_γ(DESCR_t frame0);
extern "C" rt_dcap_next_t rt_dcap_land_ω(void);
extern "C" void rt_dcap_end_ok_close(void);
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string flush_pump() {
    return x86("comment", "FLUSH (Lon 2026-09-27): run the conditional assignments recorded since the CAS begin marker [rbp-8] up to the live top r12 -- bb_match_end's pump loop, run here -- then drop them (r12 := the marker) so the end box pumps only what follows; a refused target abandons to omega")
         + x86_xfer_enter()
         + x86_anchor_enter()
         + x86("note", "cas_mark")
         + x86("mov",  "rdi", RDQ("rbp", -8))
         + x86("mov",  "rsi", "r12")
         + x86("mov",  "rdx", "r13")
         + x86("call", "rt_dcap_end_ok_open", (uint64_t)(uintptr_t)(void *)(rt_dcap_next_t (*)(const char *, const char *, const char *))rt_dcap_end_ok_open)
         + x86_rt_gc_poll_rec_sigma_word(1)
         + x86("def",  L(1))
         + x86("cmp",  "rax", 1L)
         + x86("jbe",  L(2))
         + bb_glue_enter_c2bb(20, 8, 9)
         + x86("def",  L(8))
         + x86("call", "rt_dcap_land_γ", (uint64_t)(uintptr_t)(void *)(rt_dcap_next_t (*)(DESCR_t))rt_dcap_land_γ)
         + x86_rt_gc_poll_rec_sigma_word(1)
         + x86("jmp",  L(1))
         + x86("def",  L(9))
         + x86("call", "rt_dcap_land_ω", (uint64_t)(uintptr_t)(void *)(rt_dcap_next_t (*)(void))rt_dcap_land_ω)
         + x86_rt_gc_poll_rec_sigma_word(1)
         + x86("jmp",  L(1))
         + x86("def",  L(2))
         + x86("mov",  RDQ("rsp", 0), "rax")
         + x86("call", "rt_dcap_end_ok_close", (uint64_t)(uintptr_t)(void *)(void (*)(void))rt_dcap_end_ok_close)
         + x86("mov",  "rax", RDQ("rsp", 0))
         + x86_anchor_leave()
         + x86_xfer_leave()
         + x86("test", "rax", "rax")
         + x86("je",   L(3))
         + x86_omega()
         + x86("def",  L(3))
         + x86("note", "cas_mark")
         + x86("mov",  "r12", RDQ("rbp", -8));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_fence0() {
    x86_begin();
    return x86("comment", _.op_ival == 3 ? "IR_MATCH_FENCE0 as FLUSH (ival 3): alpha pumps and drops the pending conditional assignments, then the bare FENCE cut"
                        : _.op_fence0_floor > 0 ? "IR_MATCH_FENCE0 (bare FENCE cut box, BLOB DYNAMIC RELEASE: alpha commits and restores rsp to the blob activation floor rbp-K, freeing every left-context backtrack record at once, then gamma; beta abandons to omega)"
                        : _.op_fence0_release > 0
                              ? "IR_MATCH_FENCE0 (bare FENCE cut box: alpha commits, FZ-1 releases the contiguous backtrack-only spine at the frontier, then gamma; beta abandons to omega)"
                                  : "IR_MATCH_FENCE0 (bare FENCE cut box: alpha commits — match null — then gamma; beta abandons to omega; nothing releasable here)")
         + x86_alpha()
         + IF(_.op_ival == 3, flush_pump())
         + IF(_.op_fence0_floor > 0, x86("mov", "rsp", "rbp")
                                    + x86("sub", "rsp", (long)_.op_fence0_floor))
         + IF(_.op_fence0_floor <= 0 && _.op_fence0_release > 0, x86("add", "rsp", _.op_fence0_release))
         + x86_gamma()
         + x86_beta()
         + x86_omega();
}
