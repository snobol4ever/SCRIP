#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "IR.h"
}
#include "x86_asm.h"
extern std::string gzu_build(const IR_t *nd);
extern "C" int  rt_trail_mark(void);
extern "C" void rt_trail_unwind(int mark);
extern "C" int  rt_pl_throw_pending(void);
extern "C" int  rt_pl_throw_match(void *catcher_cell);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bcc_mark_off() { return GZ_CELL_OFF((int)_.op_parts_ival[0]); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_cell_catch() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    if (_.op_parts_ival[0] < 0) return x86_bomb("bb_cell_catch: unadmitted catch shape reached the emitter");
    const IR_t *catcher = (const IR_t *)(intptr_t)_.op_parts_ival[1];
    if (_.op_sa == 0)
        return x86("def",     "α")
             + x86("comment", "IR_CELL_CATCH alpha: mark trail, run goal")
             + x86("call", "rt_trail_mark", (uint64_t)(uintptr_t)(void *)rt_trail_mark)
             + x86("mov", FR(bcc_mark_off()), "eax")
             + x86_jmp_tgt(X86T_TGT0);
    if (_.op_sa == 1)
        return x86("comment", "IR_CELL_CATCH ball-check (goal failed)")
             + x86("call", "rt_pl_throw_pending", (uint64_t)(uintptr_t)(void *)rt_pl_throw_pending)
             + x86("test", "eax", "eax")
             + x86("je", "ω")
             + x86("mov", "edi", FR(bcc_mark_off()))
             + x86("call", "rt_trail_unwind", (uint64_t)(uintptr_t)(void *)rt_trail_unwind)
             + gzu_build(catcher)
             + x86("mov", "rdi", "rax")
             + x86("call", "rt_pl_throw_match", (uint64_t)(uintptr_t)(void *)rt_pl_throw_match)
             + x86("test", "eax", "eax")
             + x86_jcc_tgt("jne", X86T_TGT0)
             + x86("jmp", "ω");
    if (_.op_sa == 2)
        return x86("def", "β") + x86("jmp", "ω");
    return x86_bomb("bb_cell_catch: unknown aspect");
}
