#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
extern "C" int   rt_defer_close            (int cur_delta);
extern "C" void *rt_match_value_get_pat_fn (DESCR_t *pval);
extern "C" void *rt_match_value_get_pat_dtp(DESCR_t *pval);
extern "C" long  rt_match_value_open       (DESCR_t *pval);
extern "C" void *dtp_fn_of(void *headv);
extern uint64_t g_scan_hit_start;
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_value() {
    return x86("comment", "IR_MATCH_VALUE (operand[0] pattern value, no manufactured name)")
         + x86_alpha()
         + IF(_.op_zres,  x86("lea",  "rdi", ZOPQ(0, 0)))
         + IF(!_.op_zres, x86("lea",  "rdi", FRQ(_.op_a_slot)))
         + x86_align_enter()
         + x86("call", "rt_match_value_get_pat_dtp", (uint64_t)(uintptr_t)(void *)(void *(*)(DESCR_t *))rt_match_value_get_pat_dtp)
         + x86_align_leave()
         + x86("mov",  "rdx", "rax")
         + x86("test", "rax", "rax")
         + x86("jz",   "L0")
         + x86("mov",  "rax", RDQ("rdx", 0))
         + x86("mov",  "r8d", (long)(_.op_scan ? 1 : 0))
         + bb_glue_pass_wires_blob(4, 5)
         + x86("def",  L(4))
         + bb_glue_wire_land()
         + IF(_.op_scan && _.op_scan_head_off >= 0 && !emit_match_owns_startd(),
               x86("lea",  "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&g_scan_hit_start, "g_scan_hit_start")
             + x86("mov",  "rax", "[rcx]")
             + x86("mov",  FR(_.op_scan_head_off), "eax"))
         + x86_gamma()
         + x86("def",  L(5))
         + bb_glue_wire_land()
         + x86_omega()
         + x86("def",  "L0")
         + IF(_.op_zres,  x86("lea",  "rdi", ZOPQ(0, 0)))
         + IF(!_.op_zres, x86("lea",  "rdi", FRQ(_.op_a_slot)))
         + x86_align_enter()
         + x86("call", "rt_match_value_open", (uint64_t)(uintptr_t)(void *)(long (*)(DESCR_t *))rt_match_value_open)
         + x86_align_leave()
         + x86("mov",  "edi", "r14d")
         + x86_align_enter()
         + x86("call", "rt_defer_close", (uint64_t)(uintptr_t)(void *)(int (*)(int))rt_defer_close)
         + x86_align_leave()
         + x86("test", "eax", "eax")
         + x86_omega("js")
         + x86("mov",  "r14d", "eax")
         + x86_lea_id("rax", 6)
         + x86_sub("rsp", 8)
         + x86("push", "rax")
         + x86_gamma()
         + x86("def",  L(6))
         + x86_add("rsp", 16)
         + x86_omega()
         + x86_beta()
         + x86_jmp_mem("rsp", 0);
}
