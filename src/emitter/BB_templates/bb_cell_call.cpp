#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
}
#include "x86_asm.h"
extern "C" void * rt_enter(void **slot, int nslots);
/*--------------------------------------------------------------------------------------------------------------------*/
static const char *bcc_areg(int i) { static const char * t[3] = { "rsi", "rdx", "rcx" }; return t[i]; }
static bool bcc_sh() { return _.op_parts_n > 0 && _.op_parts_ival[0] >= 0 && _.op_parts_ival[1] >= 0 && _.op_parts_ival[1] <= 3 && _.op_parts_ival[2] >= 0; }
static bool bcc_ar() { return _.op_parts_ival[3] != -2 && _.op_parts_ival[4] != -2 && _.op_parts_ival[5] != -2; }
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_cell_call() {
    x86_begin();
    if (PLATFORM_X86) return IF(!bcc_sh(), x86_bomb("bb_cell_call: unadmitted call shape reached the emitter"))
                           + IF(bcc_sh() && !bcc_ar(), x86_bomb("bb_cell_call: non-slot arg reached the emitter"))
                           + IF(bcc_sh() && bcc_ar(),
                             x86("label", _.lbl_α)
                           + x86("comment", "IR_CELL_CALL")
                           + x86("lea", "rdi", FR(GZ_CELL_OFF((int)_.op_parts_ival[0])))
                           + x86("mov32", "esi", (long)_.op_parts_ival[2])
                           + x86("call", "rt_enter", (uint64_t)(uintptr_t)(void *)rt_enter)
                           + x86("mov", "rdi", "rax")
                           + FOR(0, ((int)_.op_parts_ival[1] < 3 ? (int)_.op_parts_ival[1] : 3), [&](int i) { return x86("mov", bcc_areg(i), FRQ(GZ_CELL_OFF((int)_.op_parts_ival[3 + i]))); })
                           + x86_call_tgt(X86T_TGT0)
                           + x86("def", L(0))
                           + x86("test", "eax", "eax")
                           + x86("jne", "γ")
                           + x86("jmp", "ω")
                           + x86("def", "β")
                           + x86("mov", "rdi", FRQ(GZ_CELL_OFF((int)_.op_parts_ival[0])))
                           + x86_call_tgt(X86T_TGT1)
                           + x86("jmp", L(0)));
    return std::string();
}