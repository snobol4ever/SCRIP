#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
void rt_gen_save_cont(void *);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_return() {
    if (!PLATFORM_X86) return x86_alpha() + x86_bomb("bb_return: unhandled (needs descr flat-chain)");
    if (_.op_zres && _.op_dval != 2.0) {
        std::string s = x86("comment", "IR_RETURN ZD (ZK-4 cells arm): ZOPQ -> FRQ(result) -> gamma+release")
                      + x86_alpha();
        if (_.op_zread[0] >= 0) {
            s += x86("note", ZOPN(0)) + x86("mov", "rax", ZOPQ(0, 0));
            s += x86("note", ZOPN(0)) + x86("mov", "rdx", ZOPQ(0, 8));
        } else {
            s += x86("mov", "eax", (long)DT_SNUL);
            s += x86("mov", "edx", 0L);
        }
        s += x86("mov", FRQ(0), "rax");
        s += x86("mov", FRQ(8), "rdx");
        s += x86_gamma();
        return s;
    }
    std::string ret_cont_save;
    if (_.op_dval != 2.0 && _.flat_gen && g_emit_cfg && g_emit_cfg->icn_zframe_gen && _.op_sb >= 0 && _.lbl_t1_p) {
        uint64_t _sc_fp; { void (*_f)(void *) = rt_gen_save_cont; _sc_fp = (uint64_t)(uintptr_t)(void *)_f; }
        ret_cont_save = x86_lea_tgt("rdi", X86T_TGT1) + x86("call", "rt_gen_save_cont", _sc_fp);
    }
    return x86("comment", "IR_RETURN")
         + x86_alpha()
         + (_.op_dval != 2.0 && _.flat_gen && _.op_sb >= 0 && _.lbl_t1_p ? x86_lea_tgt("rax", X86T_TGT1) + x86("mov", FRQ(_.op_sb), "rax") : std::string())
         + ret_cont_save
         + IF(_.op_sa >= 0,
               x86("mov", "rax", FRQ(_.op_sa))
             + x86("mov", "rdx", FRQ(_.op_sa + 8))
             + x86("mov", FRQ(0), "rax")
             + x86("mov", FRQ(8), "rdx"))
         + IF(_.op_sa < 0,
               x86("mov", FRQ(0), (long)DT_SNUL)
             + x86("mov", FRQ(8), 0L))
         + IF(_.op_dval == 2.0, x86_omega())
         + IF(_.op_dval != 2.0, x86_gamma());
}
