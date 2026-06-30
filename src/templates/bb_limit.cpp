#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
/* IR_LIMIT — the Icon `\` limit operator.  Generator is ON-SPINE (a chain node whose γ flows into LIMIT_α); the
   count is a compile-time literal carried in _.op_ival; the generator's resume entry is carried in _.lbl_t0.  The
   per-result counter lives at [r12 + op_off + 16] and is pre-initialised to 0 once (bb_limit_init) before the
   generator first runs.  Result DESCR of the limited value is copied into the LIMIT slot at [r12 + op_off] so the
   consumer reads it.  check-BEFORE-yield (c starts at 0): on each generator success, if c >= t fail, else c++,
   copy, yield; on resume, pump the generator (jmp gen-β).  This naturally yields exactly t values and 0 for `\0`. */
std::string bb_limit() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    if (!(_.op_off >= 0 && _.op_sa >= 0 && _.lbl_t0))
        return x86_bomb("bb_limit: unhandled (needs descr flat-chain, static slots, literal count, gen-β)");
    long t = (long)_.op_ival;
    return x86("comment", "IR_LIMIT")
         + x86("label", _.lbl_α)
         + x86("mov",   "rax", FRQ(_.op_off + 16))
         + x86("cmp",   "rax", t)
         + x86("jge",   "ω")
         + x86("inc",   FRQ(_.op_off + 16))
         + x86("mov",   "rax", FRQ(_.op_sa))
         + x86("mov",   FRQ(_.op_off),     "rax")
         + x86("mov",   "rax", FRQ(_.op_sa + 8))
         + x86("mov",   FRQ(_.op_off + 8), "rax")
         + x86("jmp",   "γ")
         + x86("def",   "β")
         + x86_jmp_tgt(X86T_TGT0);
}
/*--------------------------------------------------------------------------------------------------------------------*/
/* One-shot counter initialiser, emitted once in the chain pre-pass before the spine runs (the frame is not zeroed). */
std::string bb_limit_init() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    return x86("mov", FRQ(_.op_off + 16), (long)0);
}
