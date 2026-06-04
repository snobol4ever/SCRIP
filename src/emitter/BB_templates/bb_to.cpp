#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "descr.h"
extern int g_descr_flat_chain;
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_to_str(IR_t * pBB) {
    if (!PLATFORM_X86) return std::string();
    if (!(g_descr_flat_chain && _.op_off >= 0 && _.op_sa >= 0 && _.op_sb >= 0)) return std::string();
    int lo = _.op_sa, hi = _.op_sb, off = _.op_off, cur = _.op_off + 16;
    int is_by = (pBB && pBB->t == IR_TO_BY);
    int64_t by = (is_by && pBB->ival) ? pBB->ival : 1;
    if (by <= 0) return std::string();
    std::string step = (by == 1) ? x86("inc", FRQ(cur)) : (x86("mov", "rax", FRQ(cur)) + x86("add", "rax", (long)by) + x86("mov", FRQ(cur), "rax"));
    return IF(MEDIUM_TEXT, s_1asm(std::string(_.lbl_α) + ":")
                          + s_comment(emit_fmt("# BOX IR_TO [x86() stackless int range pump by=%lld; cursor [r12+%d], lo slot %d, hi slot %d -> result %d]", (long long)by, cur, lo, hi, off)))
         + x86("mov", "rax", FRQ(lo + 8))
         + x86("mov", FRQ(cur), "rax")
         + x86("def", L(0))
         + x86("mov", "rax", FRQ(cur))
         + x86("mov", "rcx", FRQ(hi + 8))
         + x86("cmp", "rax", "rcx")
         + x86("jg",  PORT_OMEGA)
         + x86("mov", FRQ(off),     (long)DT_I)
         + x86("mov", FRQ(off + 8), "rax")
         + x86("jmp", PORT_GAMMA)
         + x86("def", PORT_BETA)
         + step
         + x86("jmp", L(0));
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_to(IR_t * pBB) {
    x86_begin();
    std::string s = bb_to_str(pBB);
    if (s.empty()) { bb_emit_x86(x86_bomb("bb_to: unhandled (needs static int operands, positive by, descr flat-chain)")); return; }
    bb_emit_x86(s);
}
