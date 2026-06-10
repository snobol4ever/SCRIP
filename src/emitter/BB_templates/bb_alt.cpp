#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "descr.h"
#include "IR.h"
extern int g_descr_flat_chain;
extern IR_graph_t * g_emit_cfg;
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bb_alt_n(IR_t * pBB) {
    int n = 0;
    IR_t * const * arms = (pBB && g_emit_cfg) ? bb_operand_aux_get(g_emit_cfg, pBB, &n) : NULL;
    if (!arms || n <= 0 || n > 5) return 0;
    for (int i = 0; i < n; i++)
        if (!arms[i] || (arms[i]->op != (IR_e)IR_LIT_I && arms[i]->op != (IR_e)IR_LIT_S)) return 0;
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * bb_alt_arm(IR_t * pBB, int i) {
    int n = 0;
    IR_t * const * arms = (pBB && g_emit_cfg) ? bb_operand_aux_get(g_emit_cfg, pBB, &n) : NULL;
    return (arms && i >= 0 && i < n) ? arms[i] : NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_alt_str(IR_t * pBB) {
    return IF(!(PLATFORM_X86 && g_descr_flat_chain && _.op_off >= 0 && bb_alt_n(pBB) > 0),
              x86_bomb("bb_alt: unhandled (needs <=5 literal arms, descr flat-chain)"))
         + IF(PLATFORM_X86 && g_descr_flat_chain && _.op_off >= 0 && bb_alt_n(pBB) > 0,
           x86("label", _.lbl_α)
         + x86("comment", "IR_ALT")
         + x86("mov", FRQ(_.op_off + 16), 0L)
         + x86("def", L(bb_alt_n(pBB)))
         + FOR(0, bb_alt_n(pBB), [&](int i) { return x86("mov", "rax", FRQ(_.op_off + 16))
                                                   + x86("cmp", "rax", (long)i)
                                                   + x86("je", L(bb_alt_n(pBB) + 1 + i)); })
         + x86("jmp", "ω")
         + FOR(0, bb_alt_n(pBB), [&](int i) { return x86("def", L(bb_alt_n(pBB) + 1 + i))
                                                   + x86("mov", FRQ(_.op_off), (long)(bb_alt_arm(pBB, i)->op == (IR_e)IR_LIT_I ? DT_I : DT_S))
                                                   + x86("ro_load_q", "rax", (long)i)
                                                   + x86("mov", FRQ(_.op_off + 8), "rax")
                                                   + x86("mov", "rax", FRQ(_.op_off + 16))
                                                   + x86("add", "rax", 1L)
                                                   + x86("mov", FRQ(_.op_off + 16), "rax")
                                                   + x86("jmp", "γ"); })
         + x86("def", "β")
         + x86("jmp", L(bb_alt_n(pBB)))
         + FOR(0, bb_alt_n(pBB), [&](int i) { return bb_alt_arm(pBB, i)->op == (IR_e)IR_LIT_I
                                                   ? x86("ro_seal_q", (long)i, (long)IR_LIT(bb_alt_arm(pBB, i)).ival)
                                                   : x86("ro_seal_str", (long)i, IR_LIT(bb_alt_arm(pBB, i)).sval ? IR_LIT(bb_alt_arm(pBB, i)).sval : ""); }));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_alt(IR_t * pBB) {
    x86_begin();
    bb_emit_x86(bb_alt_str(pBB));
}
