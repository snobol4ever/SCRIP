#include <string>
#include <stdint.h>
#include <string.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern DESCR_t rt_var_ref_cell(DESCR_t *cellp);
extern DESCR_t rt_pl_fresh_var_ref(void);   /* PL-FR-2: allocate a fresh unbound PLJ PLVAR cell for Prolog anonymous vars (G0/G1 etc from _); returns {DT_PLVAR,0,heap_cell} in rax:rdx */
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_var_ref() {
    x86_begin();
    if (PLATFORM_X86) {
        if (_.op_off == -1) return x86_alpha() + x86_bomb("bb_var_ref: needs own slot");   /* PL-FR-2: was < 0; negative is a valid rbp-relative zframe offset; -1 is the absent sentinel */
        if (_.op_gva_k < 0 && _.op_sa == -1) {   /* PL-FR-2 ANON-VAR ARM: Prolog anonymous var (G0/G1 etc from _); no frame vslot (op_sa==-1); allocate a fresh unbound PLJ cell via rt_pl_fresh_var_ref().  Result {DT_PLVAR,0,heap_cell} in rax:rdx is stored directly into the result slot at op_off/op_off+8.  BOTH MEDIA: x86() encoders handle binary and text.  ONE AUTHORITY: only this arm calls rt_pl_fresh_var_ref; no second call site.  SNOBOL4/Icon watermark: their lowerers never set zframe_graph for anonymous-var-producing code paths; this arm is structurally invisible to them.  SEMANTICS: a Prolog anonymous _ is a fresh unbound logical variable — it should be a heap cell so that unification and trailing work correctly.  It is NOT a frame slot (frame slots are for named params A0..AN that persist across clause body); each use of _ gets its own fresh cell. */
            uint64_t _fvr_fp; { DESCR_t (*_f)(void) = rt_pl_fresh_var_ref; _fvr_fp = (uint64_t)(uintptr_t)(void *)_f; }
            return x86("comment", "IR_VAR_REF anon: rt_pl_fresh_var_ref -> PLJ PLVAR cell")
                 + x86_alpha()
                 + x86("call", "rt_pl_fresh_var_ref", (long)_fvr_fp)
                 + x86("mov", FRQ(_.op_off),     "rax")
                 + x86("mov", FRQ(_.op_off + 8), "rdx")
                 + x86_gamma()
                 + x86_beta_trampoline();
        }
        if (_.op_zres && x86_fb_pinned())   /* ⭐ ZK-2: Icon cells arm, pinned-graph only (flat_gen/flat_deep_arrival/pat_proc).  Produces {DT_N, &frame_slot} in ZRES(0/8) -- a reference-DESCR pointing at the variable's rbp-relative cell.  Downstream IR_DEREF's ZD arm reads this via ZOPQ(0,0/8) and calls rt_deref to extract the value.  PINNED GUARD: on unpinned (rsp-based) graphs `lea rdx,[rbp+op_sa]` would drift as subsequent sub rsp,K carves accumulate -- the variable slot's absolute address changes with depth, so a stored pointer is stale by the time DEREF reads it.  Pinned rbp is depth-immune (established by xa_flat_wire_hdr_base / gen-adopt; x86_fb_pinned() is ONE PREDICATE).  Globals excluded: op_gva_k >= 0 routes GVA; no frame cell to point at.  SNOBOL4/Prolog watermark: icn_cells_graph=0 for non-Icon graphs; zd_wl_kind's icn_cells_graph gate prevents op_zres being set here.  ONE AUTHORITY: zd_wl_kind's SCRIP_ZD_ICN_VR killswitch. */
            return x86("comment", "IR_VAR_REF zd pinned")\
                 + x86_alpha()\
                 + x86("mov", "rax", (long)((long)1 << 32 | (long)DT_N))\
                 + x86("lea", "rdx", FRQ(_.op_sa))\
                 + x86("note", ZRESN()) + x86("mov", ZRES(0), "rax")\
                 + x86("note", ZRESN()) + x86("mov", ZRES(8), "rdx")\
                 + x86_gamma()\
                 + x86_beta_trampoline();
        return x86("comment", "IR_VAR_REF")
             + x86_alpha()
             + x86("mov", "rax", (long)((long)1 << 32 | (long)DT_N))   /* PL-FR-2 TAG-FIX: DT_N was renumbered 9→0x28 (40) in TAG-3 commit 03cecd87; emit {v=DT_N, slen=1} so plw_entry's `v==DT_N && slen==1` guard dereferences the frame-cell pointer correctly. */
             + (_.op_gva_k >= 0
                 ? x86("note", gva_name(_.op_gva_k)) + x86("mov", "rdx", (long)(RT_GVA_VA + _.op_gva_k * 16))
                 : x86("lea", "rdx", FRQ(_.op_sa)))
             + x86("mov",     FRQ(_.op_off),     "rax")
             + x86("mov",     FRQ(_.op_off + 8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline();
    }
    return std::string();
}
