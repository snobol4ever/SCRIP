#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
}
#include "x86_asm.h"
extern "C" void rt_trace_suspend_hook(const char *pname, uint64_t lo, uint64_t hi, long line);
extern "C" void rt_trace_resume_hook(const char *pname);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_suspend() {
    x86_begin();
    if (_.op_sa < 0 && !_.op_zres) return x86_alpha() + x86_bomb("bb_suspend: no expr-value slot (needs descr flat-chain producer)");
    return x86("comment", _.op_zres ? "IR_SUSPEND yield+resume [PL-ZK-2 ZD: ZOPQ->ZRES]" : "IR_SUSPEND yield+resume")
         + x86_alpha()
         + (_.op_sb >= 0 && _.lbl_t1_p ? x86_lea_tgt("rax", X86T_TGT1) + x86("mov", FRQ(_.op_sb), "rax") : std::string())
         + (_.op_zres ?
                x86("note", ZRESN())
              + x86("mov", "rax", ZOPQ(0, 0))
              + x86("mov", ZRES(0), "rax")
              + x86("note", ZRESN())
              + x86("mov", "rax", ZOPQ(0, 8))
              + x86("mov", ZRES(8), "rax")
            :
                x86("mov", "rax", FRQ(_.op_sa))
              + x86("mov", FRQ(0), "rax")
              + x86("mov", "rax", FRQ(_.op_sa + 8))
              + x86("mov", FRQ(8), "rax"))
         + x86_scan_sync_out()
         + x86("push", "rax") + x86("push", "rdx") + x86("push", "rbx") + x86("mov", "rbx", "rsp") + x86("and", "rsp", (long)-16)
         + x86_load_ro_str("rdi", (_.op_activate_proc ? _.op_activate_proc : "main")) + x86("mov", "rsi", FRQ(0)) + x86("mov", "rdx", FRQ(8))
         + x86("comment", "ARG 4 = THIS SUSPEND'S OWN SOURCE LINE, a compile-time constant. The hook used to print g_line, which tracks EXECUTION: in a re-suspension chain every outer level re-yields a value produced deep inside and nothing moves g_line on the way out, so all of them printed the innermost line (cxtrace: 16/16/16 where iconx prints 33/29/25). rcx is free here -- rax/rdx/rbx are already pushed around this call and rdi/rsi/rdx carry args 1-3.")
         + x86("mov", "rcx", (long)_.op_line)
         + x86("call", "rt_trace_suspend_hook", (uint64_t)(uintptr_t)(void *)rt_trace_suspend_hook)
         + x86("mov", "rsp", "rbx") + x86("pop", "rbx") + x86("pop", "rdx") + x86("pop", "rax")
         + x86_gamma()
         + x86_beta()
         + x86("push", "rax") + x86("push", "rdx") + x86("push", "rbx") + x86("mov", "rbx", "rsp") + x86("and", "rsp", (long)-16)
         + x86_load_ro_str("rdi", (_.op_activate_proc ? _.op_activate_proc : "main"))
         + x86("call", "rt_trace_resume_hook", (uint64_t)(uintptr_t)(void *)rt_trace_resume_hook)
         + x86("mov", "rsp", "rbx") + x86("pop", "rbx") + x86("pop", "rdx") + x86("pop", "rax")
         + x86_scan_sync_in_rr()
         + (_.lbl_t0 && _.op_suspend_stmt_uclaim > 0 ? x86_sub("rsp", _.op_suspend_stmt_uclaim) : std::string())
         + (_.lbl_t0 ? x86_jmp_tgt(X86T_TGT0) : x86_omega());
}
