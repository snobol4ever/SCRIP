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
        if (_.op_off == -1) return x86_alpha() + x86_bomb("bb_var_ref: needs own slot");   /* PL-FR-2: was < 0; negative is a valid ___-relative zframe offset; -1 is the absent sentinel */
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

        if (_.op_zres && _.op_sa >= 0 && !(g_emit_cfg && g_emit_cfg->pl_cells_graph))   /* PL-ZK-5B: exclude pl_cells_graph from the ICN NAMETRAP arm -- Prolog falls to the legacy FRQ arm below. Prolog variable passing requires plw_entry dereference chains, not Icon DEREF chains; the legacy arm's FRQ(op_off) NAMETRAP is correct for Prolog's runtime, and stage_arg_inline reads it via ___RAWQ(op_off). */
            return x86("comment", "IR_VAR_REF icn cells zd: NAMETRAP{DT_N,slen=1,&____slot} -> ZRES")
                 + x86_alpha()
                 + x86("mov", "rax", (long)((long)1 << 32 | (long)DT_N))   /* ZK-2 ICN-CELLS ZD ARM: {slen=1,v=DT_N} NAMETRAP pointing at the pinned frame-local cell.  x86_fb_pinned() guarantees ___ is locked at activation entry (flat_lcl_proc prologue: push ___;mov ___,rsp;sub rsp,frame_sz); FRQ(op_sa)=[___+op_sa] is depth-immune across subsequent RSP carves.  Consumed by IR_DEREF ZD arm (bb_deref.cpp op_zres: call rt_deref({DT_N,slen=1,ptr}) -> *ptr).  Killswitch SCRIP_ZD_ICN_VR=0 at the zd_wl_kind admission line disables both. ONE AUTHORITY: only this arm writes the NAMETRAP to ZRES for the icn_cells pinned class. */
                 + x86("note", ZRESN()) + x86("mov", ZRES(0), "rax")
                 + x86("lea", "rax", FRQ(_.op_sa))
                 + x86("note", ZRESN()) + x86("mov", ZRES(8), "rax")
                 + x86_gamma()
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
