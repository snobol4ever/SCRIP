#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
#include "emit_bb.h"
DESCR_t rt_call_arr(const char * fn, DESCR_t * args, int nargs);
int bb_slot_alloc16(IR_t * nd);
int bb_slot_get(IR_t * nd);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_fn_str(IR_t * pBB) {
    if (!PLATFORM_X86) return std::string();
    const char * fn = _.op_sval ? _.op_sval : "";
    int nargs = (int) _.op_ival;
    int resoff = bb_slot_alloc16(pBB);
    int argbase = resoff + 16;
    for (int i = 0; i < nargs; i++) {
        IR_t * ai = ir_call_arg(pBB, i);
        bb_slot_alloc16(ai ? ai : pBB);
    }
    std::string s = x86("label", _.lbl_α)
                  + x86("comment", emit_fmt("BOX IR_CALL %s(...) -> rt_call_arr [slot-based, FAIL->ω]", fn));
    for (int i = 0; i < nargs; i++) {
        IR_t * ai = ir_call_arg(pBB, i);
        int src = ai ? bb_slot_get(ai) : -1;
        int dst = argbase + i * 16;
        if (src >= 0) {
            s += x86("mov", "rax", FRQ(src));
            s += x86("mov", FRQ(dst), "rax");
            s += x86("mov", "rax", FRQ(src + 8));
            s += x86("mov", FRQ(dst + 8), "rax");
        } else {
            s += x86("mov", FRQ(dst), (long)6);
            s += x86("mov", FRQ(dst + 8), (long)0);
        }
    }
uint64_t fptr; { DESCR_t (*fp)(const char *, DESCR_t *, int) = rt_call_arr; fptr = (uint64_t)(uintptr_t)(void*)fp; }
        s += x86("mov", "rdi", "[rip + __]", (uint64_t)(uintptr_t)fn, "??");
        s += x86("lea", "rsi", FRQ(argbase));
        s += x86("mov32", "edx", (long)nargs);
        s += x86("call", "rt_call_arr", fptr);
        s += x86("mov", FRQ(resoff), "rax");
        s += x86("mov", FRQ(resoff + 8), "rdx");
        s += x86("cmp", "eax", (long)99);
        s += x86("je", PORT_OMEGA);
        s += x86("jmp", PORT_GAMMA);
        s += x86("def", PORT_BETA);
        s += x86("jmp", PORT_OMEGA);
    return s;
}
