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
    if (!PLATFORM_X86) return std::string();
    if (_.op_zres) {
        /* ZK-2 ZD ARM: element DESCRs from ZOPQ predecessor cells -> rt_make_list -> ZRES(0/8).
         * No DT_FAIL omega: rt_make_list always succeeds (returns a list DESCR).
         * x86_reg_disp32_lea64("rdi","rsp",0): bypass x86() RSP-offset adder (same law as bb_call_fn ZD-7).
         * ONE AUTHORITY: only this arm writes the list result to ZRES for icn_cells_graph. */
        int na = _.op_arg_slot_n;
        std::string s = x86("comment", "IR_MAKE_LIST zd: elements from ZOPQ -> rt_make_list -> ZRES")
                      + x86_alpha();
        if (na > 0) {
            s += x86("sub", "rsp", (long)(na * 16));
            for (int i = 0; i < na; i++) {
                s += x86("mov", "r8", (std::string("[rsp + ") + std::to_string(_.op_zread[i] + na * 16 + 0) + "]").c_str());
                s += x86("mov", (std::string("[rsp + ") + std::to_string(i * 16 + 0) + "]").c_str(), "r8");
                s += x86("mov", "r8", (std::string("[rsp + ") + std::to_string(_.op_zread[i] + na * 16 + 8) + "]").c_str());
                s += x86("mov", (std::string("[rsp + ") + std::to_string(i * 16 + 8) + "]").c_str(), "r8");
            }
            s += x86_reg_disp32_lea64("rdi", "rsp", 0);
        } else {
            s += x86("xor", "edi", "edi");
        }
        s += x86("mov32", "esi", (long)na);
        s += x86("call", "rt_make_list", (uint64_t)(uintptr_t)(void*)rt_make_list);
        if (na > 0) s += x86("add", "rsp", (long)(na * 16));
        s += x86("note", ZRESN()) + x86("mov", ZRES(0), "rax");
        s += x86("note", ZRESN()) + x86("mov", ZRES(8), "rdx");
        s += x86_gamma();
        s += x86_beta_trampoline();
        return s;
    }
    return !PLATFORM_X86
             ? std::string()
         : _.op_off < 0
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
             + x86_gamma()
             + x86_beta_trampoline();
}
