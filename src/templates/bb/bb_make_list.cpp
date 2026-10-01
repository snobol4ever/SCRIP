#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
DESCR_t rt_make_list(DESCR_t *args, int nargs);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_make_list() {
    x86_begin();
    return IF(_.op_zres,
               x86("comment", "IR_MAKE_LIST zd: elements from ZOPQ -> rt_make_list -> ZRES")
             + x86_alpha()
             + IF(_.op_arg_slot_n > 0,
                  x86("sub", "rsp", (long)(_.op_arg_slot_n * 16))
                + FOR(0, _.op_arg_slot_n, [&](int i) { return x86("mov", "rax", ZOPQ(i, _.op_arg_slot_n * 16 + 0))
                           + x86("mov", x86_zref(i * 16 + 0, 1), "rax")
                           + x86("mov", "rax", ZOPQ(i, _.op_arg_slot_n * 16 + 8))
                           + x86("mov", x86_zref(i * 16 + 8, 1), "rax"); })
                + x86_reg_disp32_lea64("rdi", "rsp", 0))
             + IF(!(_.op_arg_slot_n > 0),
                  x86("xor", "edi", "edi"))
             + x86("mov32", "esi", (long)_.op_arg_slot_n)
             + x86("call", "rt_make_list", (uint64_t)(uintptr_t)(void*)rt_make_list)
             + IF(_.op_arg_slot_n > 0,
                  x86("add", "rsp", (long)(_.op_arg_slot_n * 16)))
             + x86("note", ZRESN())
             + x86("mov", ZRES(0), "rax")
             + x86("note", ZRESN())
             + x86("mov", ZRES(8), "rdx")
             + x86_rt_gc_poll()
             + x86_gamma()
             + x86_beta_trampoline())
         + IF(!_.op_zres,
               _.op_off < 0
             ? x86_alpha() + x86_bomb("bb_make_list: unhandled (needs result slot, descr flat-chain)")
         : ![&]() { for (int i = 0; i < _.op_arg_slot_n; i++) if (_.op_arg_slot[i] < 0) return 0; return 1; }()
             ? x86_alpha() + x86_bomb("bb_make_list: element slot unfilled")
         : x86("comment", "IR_MAKE_LIST")
             + x86_alpha()
             + FOR(0, (int)_.op_arg_slot_n, [&](int i) {
                   return x86("mov", "rax", FRQ(_.op_arg_slot[i]))
                        + x86("mov", FRQ(_.op_off + 16 + i * 16), "rax")
                        + x86("mov", "rax", FRQ(_.op_arg_slot[i] + 8))
                        + x86("mov", FRQ(_.op_off + 16 + i * 16 + 8), "rax"); })
             + x86("lea",   "rdi", FRQ(_.op_off + 16))
             + x86("mov32", "esi", (long)_.op_arg_slot_n)
             + x86("call",  "rt_make_list", (uint64_t)(uintptr_t)(void*)rt_make_list)
             + x86("mov",   FRQ(_.op_off),     "rax")
             + x86("mov",   FRQ(_.op_off + 8), "rdx")
             + x86_rt_gc_poll()
             + x86_gamma()
             + x86_beta_trampoline());
}
