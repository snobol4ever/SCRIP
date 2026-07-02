#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern DESCR_t rt_call_value(DESCR_t callee, DESCR_t *argv, int n);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_value() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    if (_.op_off < 0 || _.op_sa < 0) return x86_bomb("bb_call_value: needs own slot + callee operand slot");
    int n = _.op_arg_slot_n;
    for (int i = 0; i < n; i++)
        if (_.op_arg_slot[i] < 0)
            return x86_bomb("bb_call_value: argument slot unfilled");
    std::string s = x86("comment", "IR_CALL_VALUE invoke through a runtime callee value (proc/string/int)")
                  + x86("label",   _.lbl_α);
    for (int i = 0; i < n; i++)
        s += x86("mov", "rax", FRQ(_.op_arg_slot[i]))
           + x86("mov", FRQ(_.op_off + 16 + i * 16), "rax")
           + x86("mov", "rax", FRQ(_.op_arg_slot[i] + 8))
           + x86("mov", FRQ(_.op_off + 16 + i * 16 + 8), "rax");
    s += x86("mov",   "rdi", FRQ(_.op_sa))
       + x86("mov",   "rsi", FRQ(_.op_sa + 8))
       + x86("lea",   "rdx", FRQ(_.op_off + 16))
       + x86("mov32", "ecx", (long)n)
       + x86("call",  "rt_call_value", (uint64_t)(uintptr_t)(void *)rt_call_value)
       + x86("cmp",   "eax", (long)DT_FAIL)
       + x86("je",  "ω")
       + x86("mov",   FRQ(_.op_off),     "rax")
       + x86("mov",   FRQ(_.op_off + 8), "rdx")
       + x86("jmp", "γ")
       + x86("def", "β")
       + x86("jmp", "ω");
    return s;
}
