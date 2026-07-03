#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
DESCR_t rt_make_list(DESCR_t *args, int nargs);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_make_list() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    if (_.op_off < 0)
        return x86_bomb("bb_make_list: unhandled (needs result slot, descr flat-chain)");
    int n = _.op_arg_slot_n;
    for (int i = 0; i < n; i++)
        if (_.op_arg_slot[i] < 0)
            return x86_bomb("bb_make_list: element slot unfilled");
    std::string s = x86("comment", "IR_MAKE_LIST")
                  + x86("label",   _.lbl_α);
    for (int i = 0; i < n; i++)
        s += x86("mov", "rax", FRQ(_.op_arg_slot[i]))
           + x86("mov", FRQ(_.op_off + 16 + i * 16), "rax")
           + x86("mov", "rax", FRQ(_.op_arg_slot[i] + 8))
           + x86("mov", FRQ(_.op_off + 16 + i * 16 + 8), "rax");
    s += x86("lea",   "rdi", FRQ(_.op_off + 16))
       + x86("mov32", "esi", (long)n)
       + x86("call",  "rt_make_list", (uint64_t)(uintptr_t)(void*)rt_make_list)
       + x86("mov",   FRQ(_.op_off),     "rax")
       + x86("mov",   FRQ(_.op_off + 8), "rdx")
       + x86("jmp",   "γ")
       + x86("def",   "β")
       + x86("jmp",   "ω");
    return s;
}
