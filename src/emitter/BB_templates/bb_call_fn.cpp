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
int bb_varslot(const char * name);
}
#include "x86_asm.h"
std::string marshal_call_arg(IR_t * lf, IR_graph_t * sg, int aoff, IR_t * owner, int idx);
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
                  + x86("comment", emit_fmt("BOX IR_CALL %s(...) -> rt_call_arr [operand-marshal, FAIL->ω]", fn));
    for (int i = 0; i < nargs; i++) {
        IR_t * ai = ir_call_arg(pBB, i);
        int dst = argbase + i * 16;
        s += marshal_call_arg(ai, NULL, dst, _.node, i);
    }
uint64_t fptr; { DESCR_t (*fp)(const char *, DESCR_t *, int) = rt_call_arr; fptr = (uint64_t)(uintptr_t)(void*)fp; }
    if (MEDIUM_TEXT) {
        std::string fl = emit_fmt(".Lrkfn%d", g_flat_node_id++);
        s += x86("directive", ".section .rodata")
           + x86("directive", (fl + ": .string \"" + fn + "\"").c_str())
           + x86("directive", ".section .text") + x86("directive", ".intel_syntax noprefix");
        s += x86("directive", (std::string(" lea rdi, [rip + ") + fl + "]").c_str());
    } else {
        s += x86("mov", "rdi", "[rip + __]", (uint64_t)(uintptr_t)fn, "??");
    }
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
