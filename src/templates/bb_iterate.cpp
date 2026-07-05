#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
DESCR_t rt_list_bang_at(DESCR_t obj, int64_t idx);
DESCR_t rt_list_bang_var_at(DESCR_t obj, int64_t idx);
DESCR_t rt_list_bang_key_at(DESCR_t obj, int64_t idx);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_iterate(IR_t * pBB) {
    x86_begin();
    (void)pBB;
    if (!PLATFORM_X86) return std::string();
    if (MEDIUM_MACRO_DEF) return x86("comment", "no macro form — IR_LIST_BANG");
    if (_.op_sa < 0 || _.op_sb < 0 || _.op_off < 0) return x86_bomb("bb_iterate: IR_LIST_BANG — operand/idx/out slot missing");
    const bool lv  = (_.op_sval && std::string(_.op_sval) == "lv");
    const bool key = (_.op_sval && std::string(_.op_sval) == "key");
    return x86("comment", key ? "IR_ITERATE(key)" : "IR_LIST_BANG")
         + x86("label",   _.lbl_α)
         + x86("mov",  FRQ(_.op_sb), 0L)
         + x86("def",  L(0))
         + x86("mov",  "rdi", FRQ(_.op_sa))
         + x86("mov",  "rsi", FRQ(_.op_sa + 8))
         + x86("mov",  "rdx", FRQ(_.op_sb))
         + (key ? x86("call", "rt_list_bang_key_at", (uint64_t)(uintptr_t)(void *)rt_list_bang_key_at)
            : lv ? x86("call", "rt_list_bang_var_at", (uint64_t)(uintptr_t)(void *)rt_list_bang_var_at)
                 : x86("call", "rt_list_bang_at",     (uint64_t)(uintptr_t)(void *)rt_list_bang_at))
         + x86("mov",  FRQ(_.op_off),     "rax")
         + x86("mov",  FRQ(_.op_off + 8), "rdx")
         + x86("cmp64", "rax", 99L)
         + x86("je",   "ω")
         + x86("jmp",  "γ")
         + x86("def",  "β")
         + x86("inc",  FRQ(_.op_sb))
         + x86("jmp",  L(0));
}
