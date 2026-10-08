#include <string>
#include <cstdint>
#include <cstdlib>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
typedef struct { long fn; long how; } rt_dcap_next_t;
extern "C" rt_dcap_next_t rt_dcap_end_ok_open(const char *mark, const char *top, const char *subj, void *rec);
extern "C" rt_dcap_next_t rt_dcap_land_γ(DESCR_t frame0, void *rec);
extern "C" rt_dcap_next_t rt_dcap_land_ω(void *rec);
#define DCF_RECORD_BYTES 112L
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string flush_pump() {
    return x86("note", "cas_mark")
         + x86_abs_disp32_store64(0x70000000L, "r12")
         + x86_xfer_enter()
         + x86_anchor_enter()
         + x86("sub", "rsp", DCF_RECORD_BYTES)
         + x86("note", "cas_mark")
         + x86("mov", "rdi", "r12")
         + x86("def", L(4))
         + x86("sub", "rdi", 24L)
         + x86("mov", "rax", RDQ("rdi", 0))
         + x86("test", "rax", "rax")
         + x86("jne", L(4))
         + x86("add", "rdi", 24L)
         + x86("mov", "rsi", "r12")
         + x86("mov", "rdx", "r13")
         + x86("mov", "rcx", "rsp")
         + x86("call", "rt_dcap_end_ok_open", (uint64_t)(uintptr_t)(void *)(rt_dcap_next_t (*)(const char *, const char *, const char *, void *))rt_dcap_end_ok_open)
         + x86_rt_gc_poll_rec_sigma_word(1)
         + x86("def", L(1))
         + x86("cmp", "rax", 1L)
         + x86("jbe", L(2))
         + bb_glue_enter_c2bb(20, 8, 9)
         + x86("def", L(8))
         + x86("mov", "rdx", "rsp")
         + x86("call", "rt_dcap_land_γ", (uint64_t)(uintptr_t)(void *)(rt_dcap_next_t (*)(DESCR_t, void *))rt_dcap_land_γ)
         + x86_rt_gc_poll_rec_sigma_word(1)
         + x86("jmp", L(1))
         + x86("def", L(9))
         + x86("mov", "rdi", "rsp")
         + x86("call", "rt_dcap_land_ω", (uint64_t)(uintptr_t)(void *)(rt_dcap_next_t (*)(void *))rt_dcap_land_ω)
         + x86_rt_gc_poll_rec_sigma_word(1)
         + x86("jmp", L(1))
         + x86("def", L(2))
         + x86("add", "rsp", DCF_RECORD_BYTES)
         + x86_anchor_leave()
         + x86_xfer_leave()
         + x86("test", "rax", "rax")
         + x86("je", L(3))
         + x86_omega()
         + x86("def", L(3))
         + x86("note", "cas_mark")
         + x86("def", L(5))
         + x86("sub", "r12", 24L)
         + x86("mov", "rax", RDQ("r12", 0))
         + x86("test", "rax", "rax")
         + x86("jne", L(5))
         + x86("add", "r12", 24L)
         + x86_abs_disp32_store64(0x70000000L, "r12");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_flush() {
    x86_begin();
    return x86("comment",
        "IR_MATCH_FENCE0 as FLUSH (Lon 2026-09-27; the node's literal is 3): alpha runs the conditional assignments "
            "recorded since the match's CAS begin marker [rbp-8] up to the live top r12 -- bb_match_end's pump loop, run "
                "mid-match, its commit-pump record carved on the spine -- drops them (r12 := the marker, the pinned "
                    "top refreshed before and after), then cuts exactly as the bare FENCE box: release, gamma; beta abandons to omega; a refused target abandons to omega")
         + x86_alpha()
         + flush_pump()
         + IF(_.op_fence0_floor > 0, x86("mov", "rsp", "rbp")
                                    + x86("sub", "rsp", (long)_.op_fence0_floor))
         + IF(_.op_fence0_floor <= 0 && _.op_fence0_release > 0, x86("add", "rsp", _.op_fence0_release))
         + x86_gamma()
         + x86_beta()
         + x86_omega();
}
