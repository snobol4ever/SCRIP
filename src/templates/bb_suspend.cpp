#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
}
#include "x86_asm.h"
extern "C" void rt_gen_save_cont(void *gen_fb5, void *cont);   /* ICN-FR-5: save continuation ptr keyed by gen____ (pcall scan by fb) */
extern "C" void rt_pl_cp_push3(long tm_lo, long tm_hi, void *cont);   /* PL-FR-4 ZFRAME: save {trail_mark_lo, trail_mark_hi, cont_addr} triple at each Prolog zframe yield */
extern "C" void *g_pl_zf_pending_cursor;   /* PL-FR-4 ZFRAME: set by bcps β arm to signal pending resume */
extern "C" int   g_pl_zf_target_pcall_top;   /* PL-FR-4 BUG-FIX s14: pcall_top snapshot at resume_set time; intercept fires only when g_pcall_top == this+1 */
extern "C" int   g_pcall_top;              /* PL-FR-4 BUG-FIX s14: current pcall depth; hidden but same .so so &g_pcall_top is valid */
extern "C" void rt_pl_zf_resume_clear(void);   /* PL-FR-4 ZFRAME: clear g_pl_zf_pending_cursor */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_suspend() {
    x86_begin();
    if (PLATFORM_X86) {
        /* PL-ZK-2 ZD ARM: when op_zres=1 (pl_cells_graph arm), read yield value from ZOPQ(0,0/8) — the predecessor's RSP-FORTH cell — and write to ZRES(0/8). Legacy FRQ(op_sa) path when op_zres=0 (byte-identical). STORAGE FLAVOR, NOT A DISPATCH ROUTE: the suspend/resume protocol (alpha/gamma/beta/omega wiring + lbl_t0/lbl_t1 + op_sb resume-slot) is unchanged. nops=1 (ONE AUTHORITY in zd_nops) stages ZOPQ(0) = depth-difference to the value producer; ZRES writes to [rsp+0/8] (own 16B K=16 cell). */
        if (_.op_sa < 0 && !_.op_zres) return x86_alpha() + x86_bomb("bb_suspend: no expr-value slot (needs descr flat-chain producer)");
        /* ICN-FR-4 LAYER 3: for zframe generators (g_emit.flat_gen), the continuation pointer written to
         * FRQ(op_sb) is also saved to pcall.save_Σ via rt_gen_save_cont.  The caller's C-call stack can
         * overwrite any [___+N] slot after the first yield, so the in-frame cont slot is unreliable for
         * β-resumes after the first yield.  pcall record is heap-allocated and immune.
         * This call happens BEFORE the yield value copy (rax/rdx unset at alpha) and uses rdi for the arg,
         * which is ABI-legal at alpha (no live callee values yet).  Byte-identical for non-flat_gen graphs. */
        std::string cont_save;
        if (g_emit.flat_gen && g_emit_cfg && g_emit_cfg->icn_zframe_gen && _.op_sb >= 0 && _.lbl_t1_p) {   /* ICN-FR-5: GATED icn_zframe_gen; pass gen____=___ as rdi, cont label as rsi. */
            uint64_t _sc_fp; { void (*_f)(void *, void *) = rt_gen_save_cont; _sc_fp = (uint64_t)(uintptr_t)(void *)_f; }
            cont_save = x86("mov", "rdi", "rsp")           /* gen____ = ___ at suspend α */
                      + x86_lea_tgt("rsi", X86T_TGT1)     /* cont = suspend_β label (shifted from rdi→rsi) */
                      + x86("call", "rt_gen_save_cont", _sc_fp);
        }
        /* PL-FR-4 ZFRAME TRIPLE PUSH: for Prolog zframe generators (zframe_graph=1, NOT icn_zframe_gen), save {trail_mark_lo, trail_mark_hi, suspend_β_addr} into g_pl_cp_stack before each yield.
         * The trail mark lives at FRQ(op_sa)/FRQ(op_sa+8) — the yield-value slot; op_sa is the descr-pair holding the trail mark descriptor (set by n0_call_builtin_prolog for $trail_mark).
         * The continuation label (lbl_t1 = suspend_β) is a rip-relative code address — stable across frame instances, so the β-resume path in bcps_spine_gen_arm can jump there via a fresh callee frame.
         * Byte-identical for ICN (icn_zframe_gen gate above handles that path) and SN4/non-zframe (zframe_graph=0 → this block absent).
         * Register protocol: rdi=tm_lo, rsi=tm_hi, rdx=cont; all three are ABI args before any yield-value copy clobbers rax/rdx.  Fires at α BEFORE the yield-value copy so rax is free.
         * PL-FR-4 RESUME GATE (PL-FR-4 BUG-FIX s14 — per-frame sentinel):
         * rt_jmp_frame_lexprep2 writes 1 to [fb+0] (the yield-value lo word, normally 0 until suspend fires).
         * α_body NEVER writes [___+0] — only the yield path does.  So [___+0]==1 iff this is a β-resume re-entry.
         * Per-frame: handles any recursion depth with concurrent β-resumes correctly.
         * Intercept: check [___+0]==1 (sentinel set by lexprep2); clear it; jmp to cursor in frame slot. */
        std::string pl_zf_push;
        if (g_emit.flat_gen && g_emit_cfg && g_emit_cfg->zframe_graph && !g_emit_cfg->icn_zframe_gen && _.op_sb >= 0 && _.lbl_t1_p && _.op_sa >= 0
            && _.op_sb == g_emit_cfg->resume_slot) {
            uint64_t _clear_fp; { void (*_f)(void) = rt_pl_zf_resume_clear; _clear_fp = (uint64_t)(uintptr_t)(void *)_f; }
            uint64_t _push3_fp; { void (*_f)(long, long, void *) = rt_pl_cp_push3; _push3_fp = (uint64_t)(uintptr_t)(void *)_f; }
            /* PL-FR-4 BUG-FIX (s14): check per-frame sentinel at [___+0] (== FRQ(0)).
             * 0 = fresh call → normal push3.  1 = β-resume re-entry → intercept. */
            pl_zf_push = x86("mov", "rax", FRQ(0))                  /* rax = [___+0] sentinel (1=re-entry, 0=fresh) */
                       + x86("test", "rax", "rax")
                       + x86("je", L(61))                            /* 0 = fresh call → push3 */
                       /* β-resume re-entry: clear sentinel, load cursor, jump */
                       + x86("mov", FRQ(0), 0L)                      /* clear sentinel at [___+0] */
                       + x86("call", "rt_pl_zf_resume_clear", _clear_fp)   /* clear g_pl_zf_pending_cursor (belt+suspenders; rax is clobbered) */
                       + x86("mov", "rax", FRQ(_.op_sb))             /* reload cursor from frame slot (rax was clobbered by clear call) */
                       + x86("jmp", "rax")                           /* jmp to clause-2 entry */
                       + x86("def", L(61))
                       + x86("mov", "rdi", FRQ(_.op_sa))             /* trail_mark_lo */
                       + x86("mov", "rsi", FRQ(_.op_sa + 8))         /* trail_mark_hi */
                       + x86_lea_tgt("rdx", X86T_TGT1)               /* cont = suspend_β label */
                       + x86("call", "rt_pl_cp_push3", _push3_fp);
        }
        return x86("comment", _.op_zres ? "IR_SUSPEND yield+resume [PL-ZK-2 ZD: ZOPQ->ZRES]" : "IR_SUSPEND yield+resume")
             + x86_alpha()
             + cont_save   /* ICN-FR-4: save cont to pcall before the yield-value copy clobbers registers */
             + pl_zf_push  /* PL-FR-4: save {trail_mark_lo, trail_mark_hi, suspend_β} triple to g_pl_cp_stack before yield */
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
