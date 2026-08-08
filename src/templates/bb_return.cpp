#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
void rt_gen_save_cont(void *);   /* ICN-FR-5 BUG3: same as bb_suspend — saves continuation ptr to heap-global g_gen_pending_cont; for generator return, the continuation is proc_g_ω (lbl_t1_p = flat_fail_p). */
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_return() {
    if (!PLATFORM_X86) return x86_alpha() + x86_bomb("bb_return: unhandled (needs descr flat-chain)");
    /* ZK-4 ZD ARM (cells arm, det return only): return value is in ZOPQ(0,0/8) (predecessor's RSP cell, staged depth-diff).
     * Write to FRQ(0)/FRQ(8) = result slot at [rbp+frame_off(0)] (depth-immune under flat_lcl_proc's pinned rbp).
     * x86_gamma() fires the X86H_JMP/GAMMA hook which emits add rsp,op_zgpop = K_total release before jmp.
     * Generator return (op_dval==2.0 -> ω) excluded: generator suspension bookkeeping (resume-slot poison,
     * lbl_t1) is not yet on the cells arm; falls through to the legacy FRQ path which is correct under pinned rbp.
     * GATED icn_cells_graph (via op_zres set by zd_plan): SN4/Prolog byte-identical by construction.
     * ONE AUTHORITY: zd_wl_kind + zd_k + zd_nops + this site per s22k law. */
    if (_.op_zres && _.op_dval != 2.0) {
        std::string s = x86("comment", "IR_RETURN ZD (ZK-4 cells arm): ZOPQ -> FRQ(result) -> gamma+release")
                      + x86_alpha();
        if (_.op_zread[0] >= 0) {
            s += x86("note", ZOPN(0)) + x86("mov", "rax", ZOPQ(0, 0));
            s += x86("note", ZOPN(0)) + x86("mov", "rdx", ZOPQ(0, 8));
        } else {
            /* no predecessor in run (return with no value, n_operands=0) -- install null */
            s += x86("mov", "eax", (long)DT_SNUL);
            s += x86("mov", "edx", 0L);
        }
        s += x86("mov", FRQ(0), "rax");
        s += x86("mov", FRQ(8), "rdx");
        s += x86_gamma();
        return s;
    }
    /* ICN-FR-5 BUG3: generator return must update g_gen_pending_cont to proc_g_ω (lbl_t1 = flat_fail_p).
     * bb_suspend sets g_gen_pending_cont at each suspend so the caller's β-resume calls the right continuation.
     * bb_return did NOT update it — after `return e` the last saved cont was the prior suspend's β-label,
     * so the caller's β fired AGAIN producing a duplicate yield before looping.
     * Fix: rt_gen_save_cont(lbl_t1 = proc_g_ω), same pattern as bb_suspend.
     * GATED icn_zframe_gen: Prolog flat_gen=1 graphs never set icn_zframe_gen → byte-identical. */
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
