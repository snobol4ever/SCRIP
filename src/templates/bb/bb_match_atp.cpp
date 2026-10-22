#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
extern "C" long rt_at_cursor(const char *varname, int cur_delta, void *pend);
extern "C" long rt_at_cursor_patv(void *hv, long i, const char *fb, int cur_delta);
typedef struct { long fn; long how; } rt_dcap_next_t;
extern "C" rt_dcap_next_t rt_at_cursor_open(const char *varname, int cur_delta, void *rec);
extern "C" long rt_at_cursor_land_γ(DESCR_t frame0, long word, void *rec);
extern "C" long rt_at_cursor_land_ω(long word, void *rec);
#define ATCO_RECORD_BYTES 144L
#include "x86_asm.h"
#include "bb_templates.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_atp() {
    return !(_.op_sval ? _.op_sval : "")[0] ? std::string()
         : bb_patv_slot_of(_.op_sval) >= 0
         ? ( x86("comment", "IR_MATCH_ATP patv-slot: the NAME the pattern's constructor snapped for an element target, read from the pattern's own header")
            + x86_alpha()
            + bb_glue_patv_args()
            + x86("mov", "ecx", "r14d")
            + x86("call", "rt_at_cursor_patv", (uint64_t)(uintptr_t)(void *)(long (*)(void *, long, const char *, int))rt_at_cursor_patv)
            + x86_rt_gc_poll()
            + x86_gamma()
            + x86_beta_trampoline())
         : (_.op_sval[0] == '*' && _.op_sval[1])
         ? ( x86("comment", "IR_MATCH_ATP computed target: the box opens the target's procedure and lands the name (CEO-1633)")
            + x86_alpha()
            + x86_anchor_enter()
            + x86("sub", "rsp", ATCO_RECORD_BYTES)
            + x86("mov", RDQ("rsp", 32), 0L)
            + x86("mov", RDQ("rsp", 40), 0L)
            + bb_glue_name_or_rec_lea(_.op_sval)
            + x86("mov", "esi", "r14d")
            + x86("mov", "rdx", "rsp")
            + x86("call", "rt_at_cursor_open", (uint64_t)(uintptr_t)(void *)(rt_dcap_next_t (*)(const char *, int, void *))rt_at_cursor_open)
            + x86_rt_gc_poll()
            + x86("cmp", "rax", (long)2)
            + x86("je", L(5))
            + x86("test", "rax", "rax")
            + x86("je", L(1))
            + bb_glue_enter_c2bb(10, 2, 3)
            + x86("def", L(2))
            + x86("mov", "rcx", "rsp")
            + x86("call", "rt_at_cursor_land_γ", (uint64_t)(uintptr_t)(void *)(long (*)(DESCR_t, long, void *))rt_at_cursor_land_γ)
            + x86_rt_gc_poll()
            + x86("cmp", "rax", (long)2)
            + x86("je", L(5))
            + x86("jmp", L(1))
            + x86("def", L(3))
            + x86("mov", "rsi", "rsp")
            + x86("call", "rt_at_cursor_land_ω", (uint64_t)(uintptr_t)(void *)(long (*)(long, void *))rt_at_cursor_land_ω)
            + x86_rt_gc_poll()
            + x86("jmp", L(1))
            + x86("def", L(5))
            + bb_glue_trace_pend_run(30, std::string(), 0, 32)
            + x86("def", L(1))
            + x86("add", "rsp", ATCO_RECORD_BYTES)
            + x86_anchor_leave()
            + x86_gamma()
            + x86_beta_trampoline())
         : ( x86("comment", "IR_MATCH_ATP: the leaf assigns the cursor and fills a pending VALUE trace; the box runs the callback (CEO-1636)")
            + x86_alpha()
            + x86("sub", "rsp", 112L)
            + x86("mov", RDQ("rsp", 0), 0L)
            + x86("mov", RDQ("rsp", 8), 0L)
            + x86("mov", "esi", "r14d")
            + bb_glue_name_or_rec_lea(_.op_sval ? _.op_sval : "")
            + x86("mov", "rdx", "rsp")
            + x86("call", "rt_at_cursor", (uint64_t)(uintptr_t)(void *)(long (*)(const char *, int, void *))rt_at_cursor)
            + x86_rt_gc_poll()
            + bb_glue_trace_pend_run(30, std::string(), 0)
            + x86("add", "rsp", 112L)
            + x86_gamma()
            + x86_beta_trampoline());
}
