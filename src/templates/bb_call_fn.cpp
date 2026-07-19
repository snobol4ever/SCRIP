#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
DESCR_t rt_call_arr(const char * fn, DESCR_t * args, int nargs);
int bb_slot_get(IR_t * nd);
void bb_slot_register(IR_t * nd, int off);
}
#include "x86_asm.h"
std::string marshal_call_arg(IR_t * lf, IR_graph_t * sg, int aoff, IR_t * owner, int idx);
void * dop_direct_fp(const char * fn, int64_t narg, const char ** sym);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bcfn_result_slot(IR_t * nd) {
    { int _s = nd ? zls_off(nd) : -1; if (_s >= 0) { if (bb_slot_get(nd) < 0) bb_slot_register(nd, _s); return _s; } }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_fn_str(IR_t * pBB) {
    if (!PLATFORM_X86) return std::string();
    const char * fn = _.op_sval ? _.op_sval : "";
    int nargs = (int) _.op_ival;
    int resoff = bcfn_result_slot(pBB);
    if (resoff < 0) return x86_alpha() + x86_bomb("bb_call_fn: no LOWER slot grant (TMP-ERADICATE)");
    if (_.node && nargs > _.node->n_operands) return x86_alpha() + x86_bomb("bb_call_fn: arg count exceeds LOWER grant (TMP-ERADICATE)");
    int argbase = resoff + 16;
    IR_graph_t ** subs = (IR_graph_t **)(intptr_t) _.op_counter;
    std::string s = x86_alpha()
                  + x86("comment", std::string("BOX IR_CALL ") + fn + "(...) -> rt_call_arr [operand-marshal, FAIL->ω]");
    for (int i = 0; i < nargs; i++)
        s += marshal_call_arg((subs && subs[i]) ? subs[i]->entry : ir_call_arg(pBB, i), (subs && subs[i]) ? subs[i] : NULL, argbase + i * 16, _.node, i);
    const char * dsym = 0; void * dfp = dop_direct_fp(fn, (int64_t)nargs, &dsym);
    if (dfp) {
        s += x86("comment", (std::string("PL-REGAIN-2 direct det leaf: ") + dsym + " (no by-name dispatch)").c_str());
        s += x86("lea", "rdi", FRQ(argbase));
        s += x86("mov32", "esi", (long)nargs);
        s += x86("call", dsym, (uint64_t)(uintptr_t)dfp);
    } else {
        std::string fl = std::string(".Lrkfn") + std::to_string(g_flat_node_id++);
        s += x86("directive", ".section .rodata");
        s += x86("directive", (fl + ": .string \"" + fn + "\"").c_str());
        s += x86("directive", ".section .text");
        s += x86("directive", ".intel_syntax noprefix");
        s += x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)fn, fl.c_str());
        s += x86("lea", "rsi", FRQ(argbase));
        s += x86("mov32", "edx", (long)nargs);
        s += x86("call", "rt_call_arr", (uint64_t)(uintptr_t)(void *)rt_call_arr);
    }
    s += x86("mov", FRQ(resoff), "rax");
    s += x86("mov", FRQ(resoff + 8), "rdx");
    s += x86("cmp", "eax", (long)99);
    s += x86_omega("je");
    s += x86_gamma();
    s += x86_beta();
    s += x86_omega();
    return s;
}
