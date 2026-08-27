#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
}
#include "x86_asm.h"
extern "C" void rt_gen_save_cont(void *gen_fb, void *cont);
extern "C" void rt_pl_cp_push3(long tm_lo, long tm_hi, void *cont);
extern "C" void *g_pl_zf_pending_cursor;
extern "C" int   g_pl_zf_target_pcall_top;
extern "C" int   g_pcall_top;
extern "C" void rt_pl_zf_resume_clear(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_suspend() {
    x86_begin();
    if (_.op_sa < 0 && !_.op_zres) return x86_alpha() + x86_bomb("bb_suspend: no expr-value slot (needs descr flat-chain producer)");
    std::string cont_save;
    if (g_emit.flat_gen && g_emit_cfg && g_emit_cfg->icn_zframe_gen && _.op_sb >= 0 && _.lbl_t1_p) {
        uint64_t _sc_fp; { void (*_f)(void *, void *) = rt_gen_save_cont; _sc_fp = (uint64_t)(uintptr_t)(void *)_f; }
        cont_save = x86("mov", "rdi", "rsp")
                  + x86_lea_tgt("rsi", X86T_TGT1)
                  + x86("call", "rt_gen_save_cont", _sc_fp);
    }
    std::string pl_zf_push;
    if (g_emit.flat_gen && g_emit_cfg && g_emit_cfg->zframe_graph && !g_emit_cfg->icn_zframe_gen && _.op_sb >= 0 && _.lbl_t1_p && _.op_sa >= 0
        && _.op_sb == g_emit_cfg->resume_slot) {
        uint64_t _clear_fp; { void (*_f)(void) = rt_pl_zf_resume_clear; _clear_fp = (uint64_t)(uintptr_t)(void *)_f; }
        uint64_t _push3_fp; { void (*_f)(long, long, void *) = rt_pl_cp_push3; _push3_fp = (uint64_t)(uintptr_t)(void *)_f; }
        pl_zf_push = x86("mov", "rax", FRQ(0))
                   + x86("test", "rax", "rax")
                   + x86("je", L(61))
                   + x86("mov", FRQ(0), 0L)
                   + x86("call", "rt_pl_zf_resume_clear", _clear_fp)
                   + x86("mov", "rax", FRQ(_.op_sb))
                   + x86("jmp", "rax")
                   + x86("def", L(61))
                   + x86("mov", "rdi", FRQ(_.op_sa))
                   + x86("mov", "rsi", FRQ(_.op_sa + 8))
                   + x86_lea_tgt("rdx", X86T_TGT1)
                   + x86("call", "rt_pl_cp_push3", _push3_fp);
    }
    return x86("comment", _.op_zres ? "IR_SUSPEND yield+resume [PL-ZK-2 ZD: ZOPQ->ZRES]" : "IR_SUSPEND yield+resume")
         + x86_alpha()
         + cont_save
         + pl_zf_push
         + (_.op_sb >= 0 && _.lbl_t1_p ? x86_lea_tgt("rax", X86T_TGT1) + x86("mov", FRQ(_.op_sb), "rax") : std::string())
         + (_.op_zres ?
                x86("note", ZRESN()) + x86("mov", "rax", ZOPQ(0, 0)) + x86("mov", ZRES(0), "rax")
              + x86("note", ZRESN()) + x86("mov", "rax", ZOPQ(0, 8)) + x86("mov", ZRES(8), "rax")
            :
                x86("mov", "rax", FRQ(_.op_sa)) + x86("mov", FRQ(0), "rax")
              + x86("mov", "rax", FRQ(_.op_sa + 8)) + x86("mov", FRQ(8), "rax"))
         + x86_scan_sync_out()
         + x86_gamma()
         + x86_beta()
         + x86_scan_sync_in_rr()
         + (_.lbl_t0 && _.op_suspend_stmt_uclaim > 0 ? x86_sub("rsp", _.op_suspend_stmt_uclaim) : std::string())
         + (_.lbl_t0 ? x86_jmp_tgt(X86T_TGT0) : x86_omega());
    return std::string();
}
