#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
}
#include "x86_asm.h"
extern "C" void rt_gen_save_cont(void *);   /* ICN-FR-4 L3: save continuation ptr to pcall.save_Σ (heap-safe; lex epilogue never reads save_Σ) */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_suspend() {
    x86_begin();
    if (PLATFORM_X86) {
        /* PL-ZK-2 ZD ARM: when op_zres=1 (pl_cells_graph arm), read yield value from ZOPQ(0,0/8) — the predecessor's RSP-FORTH cell — and write to ZRES(0/8). Legacy FRQ(op_sa) path when op_zres=0 (byte-identical). STORAGE FLAVOR, NOT A DISPATCH ROUTE: the suspend/resume protocol (alpha/gamma/beta/omega wiring + lbl_t0/lbl_t1 + op_sb resume-slot) is unchanged. nops=1 (ONE AUTHORITY in zd_nops) stages ZOPQ(0) = depth-difference to the value producer; ZRES writes to [rsp+0/8] (own 16B K=16 cell). */
        if (_.op_sa < 0 && !_.op_zres) return x86_alpha() + x86_bomb("bb_suspend: no expr-value slot (needs descr flat-chain producer)");
        /* ICN-FR-4 LAYER 3: for zframe generators (g_emit.flat_gen), the continuation pointer written to
         * FRQ(op_sb) is also saved to pcall.save_Σ via rt_gen_save_cont.  The caller's C-call stack can
         * overwrite any [rbp+N] slot after the first yield, so the in-frame cont slot is unreliable for
         * β-resumes after the first yield.  pcall record is heap-allocated and immune.
         * This call happens BEFORE the yield value copy (rax/rdx unset at alpha) and uses rdi for the arg,
         * which is ABI-legal at alpha (no live callee values yet).  Byte-identical for non-flat_gen graphs. */
        std::string cont_save;
        if (g_emit.flat_gen && g_emit_cfg && g_emit_cfg->icn_zframe_gen && _.op_sb >= 0 && _.lbl_t1_p) {   /* ICN-FR-4 + PL-ZD-WINDOW2-FIX: GATED icn_zframe_gen — Prolog flat_gen graphs skip this save. */
            uint64_t _sc_fp; { void (*_f)(void *) = rt_gen_save_cont; _sc_fp = (uint64_t)(uintptr_t)(void *)_f; }
            cont_save = x86_lea_tgt("rdi", X86T_TGT1) + x86("call", "rt_gen_save_cont", _sc_fp);
        }
        return x86("comment", _.op_zres ? "IR_SUSPEND yield+resume [PL-ZK-2 ZD: ZOPQ->ZRES]" : "IR_SUSPEND yield+resume")
             + x86_alpha()
             + cont_save   /* ICN-FR-4: save cont to pcall before the yield-value copy clobbers registers */
             + (_.op_sb >= 0 && _.lbl_t1_p ? x86_lea_tgt("rax", X86T_TGT1) + x86("mov", FRQ(_.op_sb), "rax") : std::string())
             + (_.op_zres ?
                    /* ZD arm: copy predecessor cell from ZOPQ(0) to ZRES */
                    x86("note", ZRESN()) + x86("mov", "rax", ZOPQ(0, 0)) + x86("mov", ZRES(0), "rax")
                  + x86("note", ZRESN()) + x86("mov", "rax", ZOPQ(0, 8)) + x86("mov", ZRES(8), "rax")
                :
                    /* legacy arm: copy from flat slot */
                    x86("mov", "rax", FRQ(_.op_sa)) + x86("mov", FRQ(0), "rax")
                  + x86("mov", "rax", FRQ(_.op_sa + 8)) + x86("mov", FRQ(8), "rax"))
             + x86_scan_sync_out()
             + x86_gamma()
             + x86_beta()
             + x86_scan_sync_in_rr()
             /* ICN-FR-5 SUSPEND-DO RECARVE: when lbl_t0 (the do-body) is present, the γ-exit above freed
              * the statement's uclaim (add rsp, K via the X86H_JMP/X86P_GAMMA port hook).  The β re-entry
              * lands AFTER that free; the do-body runs inside the same statement, and the loop-back to the
              * while head re-enters this suspend's α which does the matching add rsp K again.
              * Without re-carving here, each iteration drifts rsp upward by the claim size, eventually
              * clobbering the zframe wire header and causing an infinite loop or SEGV (upto() witness).
              * op_suspend_stmt_uclaim = zd_uk[head_i] staged by the drive loop; 0 for non-zframe or
              * suspend-without-body, so this sub rsp emits only when needed (byte-identical otherwise).
              * The zero-init from the original α carve is NOT repeated — frame slots retain their live
              * values across the yield; the re-carve only re-establishes rsp parity. */
             + (_.lbl_t0 && _.op_suspend_stmt_uclaim > 0 ? x86_sub("rsp", _.op_suspend_stmt_uclaim) : std::string())
             + (_.lbl_t0 ? x86_jmp_tgt(X86T_TGT0) : x86_omega());
    }
    return std::string();
}
