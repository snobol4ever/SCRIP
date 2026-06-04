#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
#include "IR.h"
#include "IR_interp_state.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string cc_load_args(const pl_gz_call_state_t *st) {
    std::string s;
    static const char * areg[2] = { "rdi", "rsi" };
    for (int i = 0; i < st->nargs && i < 2; i++) {
        const IR_t *a = st->args[i];
        if (!a || a->t != IR_LOGICVAR) return std::string();
        s += x86("mov", areg[i], FRQ(GZ_CELL_OFF((int)a->ival)));
    }
    return s;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_cell_call_str() {
    if (!PLATFORM_X86) return std::string();
    x86_begin();
    const IR_t *nd = (const IR_t *)_.bb_zn;
    const pl_gz_call_state_t *st = nd ? (const pl_gz_call_state_t *)(intptr_t)nd->ival : (const pl_gz_call_state_t *)0;
    if (!st || !st->callee || st->nargs < 0 || st->nargs > 2)
        return x86_bomb("bb_cell_call: unadmitted call shape reached the emitter");
    std::string args = cc_load_args(st);
    if (st->nargs > 0 && args.empty()) return x86_bomb("bb_cell_call: non-slot arg reached the emitter");
    return IF(MEDIUM_TEXT, s_1asm(std::string(_.lbl_α) + ":")
             + s_comment("# BOX CELL_CALL  [PL-GZ-5a seed transcription: args are cell POINTERS in rdi/rsi; call callee α (δ port); verdict-in-rax λ-test; β reloads args and re-enters callee β (ε port)]"))
         + args
         + x86("call", PORT_DELTA)
         + x86("def", L(0))
         + x86("test", "eax", "eax")
         + x86("jne", PORT_GAMMA)
         + x86("jmp", PORT_OMEGA)
         + x86("def", PORT_BETA)
         + args
         + x86("call", PORT_EPSILON)
         + x86("jmp", L(0));
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_cell_call(void) { bb_emit_x86(bb_cell_call_str()); }
