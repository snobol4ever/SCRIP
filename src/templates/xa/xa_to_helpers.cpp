#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
extern DESCR_t rt_num_arith_strict(DESCR_t, DESCR_t, int);
#include "bb_template_common.h"
#include "descr.h"
#include "../runtime/builtins/gen.h"
DESCR_t rt_num_arith(DESCR_t a, DESCR_t b, int op);
int     rt_jct_relop(DESCR_t lhs, DESCR_t rhs, int op);
int64_t to_int(DESCR_t v);
int64_t core_icn_to_int_check(uint64_t lo, uint64_t hi);
int     core_icn_int_operand_ok(uint64_t lo, uint64_t hi);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string xa_to_trail_mark() {
    return IF(x86_fb_pinned(),
               x86("mov", FRQ(_.op_off + 24), "r12")
             + x86_pl_disj_open(x86_fb(), g_emit.flat_frame_bytes, 226, 227));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string xa_to_trail_unwind() {
    return IF(!x86_fb_pinned(),
               std::string())
         + IF(!(!x86_fb_pinned()),
               x86_pl_tr_unwind_at(FRQ(_.op_off + 24), 200, 201)
               + x86("test", "r15", "r15")
               + x86_omega("jne"));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string xa_to_int_operand_guard(int slot) {
    return IF(_.op_range_int_operands,
               x86("mov",  "rdi", FRQ(slot))
             + x86("mov",  "rsi", FRQ(slot + 8))
             + x86("call", "core_icn_int_operand_ok", (uint64_t)(uintptr_t)(void *)(int (*)(uint64_t, uint64_t))core_icn_int_operand_ok)
             + x86("test", "eax", "eax")
             + x86_omega("jz")
             + x86_rt_gc_poll());
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string xa_to_db_load_store() {
    return x86("mov", "rax", FRQ(_.op_sa + 8))
           + x86_shift_imm("shl", 4, "rax", 3)
           + x86("mov", "rcx", "r14")
           + x86("sub", "rcx", "rax")
           + x86("mov", "rax", RDQ("rcx", -24))
           + x86("mov", "r9", RDQ("rax", 0));
}
std::string xa_to_db_slot_addr() {
    return x86("mov", "r10", "rsi")
           + x86_shift_imm("shl", 4, "r10", 5)
           + x86("mov", "r11", "rsi")
           + x86_shift_imm("shl", 4, "r11", 3)
           + x86("add", "r10", "r11")
           + x86("add", "r10", "r9");
}
