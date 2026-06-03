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
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_alt_emit_arm(const IR_t * arm, int dslot, int off) {
    if (!arm) return std::string();
    if (arm->t == (IR_e)IR_LIT_I)
        return x86_frame_mov_imm64(off, (long)DT_I) + x86_ro_load_q("rax", dslot) + x86_frame_store64(off + 8, "rax");
    if (arm->t == (IR_e)IR_LIT_S)
        return x86_frame_mov_imm64(off, (long)DT_S) + x86_ro_load_q("rax", dslot) + x86_frame_store64(off + 8, "rax");
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_alt_seal_arm(const IR_t * arm, int dslot) {
    if (!arm) return std::string();
    if (arm->t == (IR_e)IR_LIT_I) return x86_ro_seal_q(dslot, (uint64_t) arm->ival);
    if (arm->t == (IR_e)IR_LIT_S) return x86_ro_seal_str(dslot, arm->sval ? arm->sval : "");
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_alt_str(IR_t * pBB) {
    if (!PLATFORM_X86) return std::string();
    if (!(g_descr_flat_chain && _.op_off >= 0)) return std::string();
    int n = 0;
    IR_t * const * arms = (pBB && g_emit_cfg) ? bb_operand_aux_get(g_emit_cfg, pBB, &n) : NULL;
    if (!arms || n <= 0 || n > 5) return std::string();
    for (int i = 0; i < n; i++)
        if (!arms[i] || (arms[i]->t != (IR_e)IR_LIT_I && arms[i]->t != (IR_e)IR_LIT_S)) return std::string();
    int off = _.op_off, ctr = _.op_off + 16;
    int Lbase = n;
    std::string body =
          IF(MEDIUM_TEXT, s_1asm(std::string(_.lbl_α) + ":")
                         + s_comment(emit_fmt("# BOX IR_ALT [x86() stackless %d-arm generator; result [r12+%d], counter [r12+%d]]", n, off, ctr)))
        + x86_frame_mov_imm64(ctr, 0)
        + x86("def", L(Lbase));
    for (int i = 0; i < n; i++) {
        body += x86("mov", "rax", FRQ(ctr))
              + x86("cmp", "rax", (long)i)
              + x86("je",  L(Lbase + 1 + i));
    }
    body += x86("jmp", PORT_OMEGA);
    for (int i = 0; i < n; i++) {
        body += x86("def", L(Lbase + 1 + i))
              + bb_alt_emit_arm(arms[i], i, off)
              + x86("mov", "rax", FRQ(ctr))
              + x86("add", "rax", 1L)
              + x86("mov", FRQ(ctr), "rax")
              + x86("jmp", PORT_GAMMA);
    }
    body += x86("def", PORT_BETA)
          + x86("jmp", L(Lbase));
    for (int i = 0; i < n; i++) body += bb_alt_seal_arm(arms[i], i);
    return body;
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_alt(IR_t * pBB) {
    x86_begin();
    std::string s = bb_alt_str(pBB);
    if (s.empty()) { bb_emit_x86(x86_bomb("bb_alt: unhandled (needs <=5 literal arms, descr flat-chain)")); return; }
    bb_emit_x86(s);
}
