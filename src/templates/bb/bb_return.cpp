#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
}
#include "x86_asm.h"
extern "C" void rt_trace_gen_return_hook(const char *pname, uint64_t lo, uint64_t hi, void *h);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_return() {
    if (_.op_zres && _.op_dval != 2.0) {
        std::string s = x86("comment", "IR_RETURN ZD (ZK-4 cells arm): ZOPQ -> FRQ(result) -> gamma+release")
                      + x86_alpha();
        if (_.op_zread[0] >= 0) {
            s += x86("note", ZOPN(0)) + x86("mov", "rax", ZOPQ(0, 0));
            s += x86("note", ZOPN(0)) + x86("mov", "rdx", ZOPQ(0, 8));
        } else {
            s += x86("mov", "eax", (long)DT_SNUL);
            s += x86("mov", "edx", 0L);
        }
        s += x86("mov", FRQ(0), "rax");
        s += x86("mov", FRQ(8), "rdx");
        if (_.op_zread[0] < 0) { s += x86("mov", "rax", FRQ(0)); s += x86("mov", "rdx", FRQ(8)); }
        s += x86_gamma();
        return s;
    }
    int gen_ret = (_.op_dval != 2.0 && _.flat_gen && _.op_sb >= 0 && _.lbl_t1_p && icn_gen_regime()) ? 1 : 0;
    return x86("comment", "IR_RETURN")
         + x86_alpha()
         + (_.op_dval != 2.0 && _.flat_gen && _.op_sb >= 0 && _.lbl_t1_p ? x86_lea_tgt("rax", X86T_TGT1) + x86("mov", FRQ(_.op_sb), "rax") : std::string())
         + IF(_.op_sa >= 0,
               x86("mov", "rax", FRQ(_.op_sa))
             + x86("mov", "rdx", FRQ(_.op_sa + 8))
             + x86("mov", FRQ(0), "rax")
             + x86("mov", FRQ(8), "rdx"))
         + IF(_.op_sa < 0,
               x86("mov", FRQ(0), (long)DT_SNUL)
             + x86("mov", FRQ(8), 0L)
             + x86("mov", "rax", FRQ(0))
             + x86("mov", "rdx", FRQ(8)))
         + IF(_.op_dval != 2.0 && _.flat_gen,
               x86("push", "rax") + x86("push", "rdx") + x86("push", "rbx") + x86("mov", "rbx", "rsp") + x86("and", "rsp", (long)-16)
             + x86_load_ro_str("rdi", (_.op_activate_proc ? _.op_activate_proc : "main")) + x86("mov", "rsi", FRQ(0)) + x86("mov", "rdx", FRQ(8))
             + x86("comment", "THE FOURTH ARGUMENT IS THIS ACTIVATION'S REGION HEADER H (row icon-a-generator-that-returns-is-re-entered-and-traces-a-failure, cfo, the defect 1493214e4 named as left uncured): a return here parks omega in the resume slot and YIELDS, so the next resume reaches omega and its tap prints a failure where iconx prints nothing at all -- Icon's return removes the generator. rbp is H at this point and again at omega, so recording it is what lets omega's tap suppress exactly the one fail that this return will cause.")
             + x86("mov", "rcx", "rbp")
             + x86("call", "rt_trace_gen_return_hook", (uint64_t)(uintptr_t)(void *)rt_trace_gen_return_hook)
             + x86("mov", "rsp", "rbx") + x86("pop", "rbx") + x86("pop", "rdx") + x86("pop", "rax"))
         + IF(_.op_dval == 2.0, x86_omega())
         + IF(_.op_dval != 2.0, x86_gamma());
}
