#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "../../runtime/rt/rt.h"
int bb_slot_get(IR_t *nd);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_rk_bool_str(IR_t * pBB) {
    if (!PLATFORM_X86) return std::string();
    int off = _.op_a_slot;
    if (off < 0) return x86_bomb("bb_call_rk_bool: arg slot not allocated");
    if (MEDIUM_BINARY)
        return x86("label", _.lbl_α)
             + x86_frame_load64("rdi", off)
             + x86_frame_load64("rsi", off + 8)
             + x86("call", "rt_rk_is_truthy", (uint64_t)(uintptr_t)(void *)rt_rk_is_truthy)
             + x86("test", "eax", "eax")
             + x86("je", "ω")
             + x86("jmp", "γ")
             + x86("def", "β")
             + x86("jmp", "ω");
    if (MEDIUM_TEXT)
        return x86("label", _.lbl_α)
             + x86("comment", "BOX __rk_bool [descr flat-chain: slot truthiness test]")
             + x86("ins2", "mov", "rdi, [r12+" + std::to_string(off) + "]")
             + x86("ins2", "mov", "rsi, [r12+" + std::to_string(off + 8) + "]")
             + x86("ins2", "call", "rt_rk_is_truthy@PLT")
             + x86("ins2", "test", "eax, eax")
             + x86("ins2", "je",   std::string(_.lbl_ω))
             + x86("ins2", "jmp",  std::string(_.lbl_γ))
             + x86("Lins1", std::string(_.lbl_β) + ":", "")
             + x86("ins2", "jmp",  std::string(_.lbl_ω));
    return std::string();
}
