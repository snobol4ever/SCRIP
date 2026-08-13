#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
long    rt_proc_call_open(const char *name, int nargs);
void   *rt_proc_call_open_det(long idx, int nargs);
void   *rt_proc_call_open_det0(long idx);
void   *rt_proc_call_open_det1(long idx, DESCR_t *a0);
void   *rt_proc_call_open_det2(long idx, DESCR_t *a0, DESCR_t *a1);
void   *rt_proc_call_open_det3(long idx, DESCR_t *a0, DESCR_t *a1, DESCR_t *a2);
void   *rt_proc_call_open_det4(long idx, DESCR_t *a0, DESCR_t *a1, DESCR_t *a2, DESCR_t *a3);
int     rt_proc_index_of(const char *name);
void   *rt_proc_open_fn(void);
void   *rt_frame_prep(void *fb, long fbytes);
DESCR_t rt_proc_call_epilogue_γ(DESCR_t frame0);
DESCR_t rt_proc_call_epilogue_ω(void);
DESCR_t rt_proc_call_epilogue_ret(DESCR_t fret);
DESCR_t rt_faildescr(void);
DESCR_t rt_proc_call_gen_h(const char *name, int nargs, void **act_slot);
DESCR_t rt_proc_resume_frame(void *act);
DESCR_t rt_proc_resume_frame_h(void **hslot);
DESCR_t rt_gen_spine_pass_γ(DESCR_t v);
DESCR_t rt_gen_spine_pass_ω(void);
void rt_gen_spine_resume_enter(void);
void   *rt_gen_get_fb(void);   /* ICN-FR-4: returns generator frame base (pcall.fb) for zframe β-resume dispatch */
void   *rt_gen_get_cont(void *gen____); /* ICN-FR-5: returns continuation ptr keyed by gen____ (pcall scan by fb) */
int     zls_g_resume_by_name(const char *name);   /* ICN-FR-4: emit-time callee resume-slot lookup by name (zeta_storage.c; scans zg[] once per call-site; result baked as immediate) */
int     zls_g_icn_zframe_gen_by_name(const char *name);   /* ICN-FR-5 BUG1: callee's icn_zframe_gen flag by name — 1 = Icon zframe generator; 0 = Prolog or non-generator (zeta_storage.c) */
int     zls_g_pl_zf_trail_mark_off_by_name(const char *name);   /* PL-FR-4 s12: callee's pl_trail_mark_off by name — >0 = frame slot of $trail_mark result; 0 = absent (zeta_storage.c) */
int     zls_g_pl_zf_trail_mark_off_by_name(const char *name);   /* PL-FR-4: callee trail-mark frame slot offset by name; 0 = not a Prolog zframe pred or no $trail_mark; mirrors icn_zframe_gen_by_name pattern (zeta_storage.c) */
int  rt_proc_is_generator(const char *name);
int rt_define_tiny_ok(const char *, int);
void *rt_pl_cp_pop3(long *tm_lo, long *tm_hi);   /* PL-FR-4 ZFRAME: pop {trail_mark_lo, trail_mark_hi, cont_addr} triple; 0 = exhausted = omega. */
void rt_pl_zf_resume_set(void *cursor, long tm_lo, long tm_hi, int tm_off, int cursor_off);   /* PL-FR-4 ZFRAME RESUME: set pending-resume globals. */
int  rt_proc_dyn_scope(const char *name);
void rt_arg_stage(int idx, DESCR_t v);
extern "C" DESCR_t g_call_args[];
extern "C" int g_gc_pending;
int  rt_proc_is_registered(const char *name);
long rt_proc_call_open_slim(const char *name, int np, int nargs);
int  rt_proc_nformals(const char *name);
void rt_c2b_arm_trap(void);   /* CALL2BB 3b: loud abort for the slim runtime-decline landing on an fc-armed call -- the flat fallback does not exist as storage on an armed statement (correct-or-loud, FLATDISP-6 conservatism) */
int  rt_pl_dc_ok(const char *name, int nargs);
void **rt_pl_dc_slot(long idx);
DESCR_t rt_proc_call_epilogue_slim_γ(DESCR_t result);
DESCR_t rt_proc_call_epilogue_slim_ω(void);
int  rt_proc_nparams(const char *name);
const char *rt_proc_pname(const char *name, int k);
const char *rt_proc_result_name_get(const char *name);
int  scc_program_ok(void);
int  gva_index_of(const char *name);
extern int g_gva_active;
int  bb_slot_get(IR_t * nd);
void bb_slot_register(IR_t * nd, int off);
}
#include "x86_asm.h"
/* PL-STAGE-1 (2026-07-25) - INLINE ARG INSTALL, the REGAIN-1 "slice B" parked since s100.  rt_arg_stage(idx,v) is `rt_gc_point(&v,0); g_call_args[idx] = v;` and rt_gc_point_arr's FIRST act is
 * `if (!g_gc_pending) return;` - so on every call where no collection is pending (the overwhelming majority) the runtime spends THREE nested -O0 call frames to perform ONE 16-byte store.  nrev stages
 * ~25M args and the sampled leaf profile put the proc-call spine at ~36% of non-GC time.  This emits the store inline and calls the C leaf ONLY when g_gc_pending is set (there the collector may adjust v
 * under the shield, so the leaf must own it).  Bit-identical by construction: the tested predicate is the leaf's own first branch, the bounds test 0 <= idx < CALL_ARGS_MAX is decided at EMIT time
 * (out-of-range keeps the plain call), and the fast arm reproduces the leaf's only other effect.  THE s100 BLOCKER IS STALE: that note parked this on "g_call_args residency (.so data, movabs-forbidden)",
 * which is precisely what SINK-1 solved - the dual-medium RIPSEAL load x86("lea", r, "[rip + __]", &sym, "sym") emits a rip-relative symbol in TEXT and the live address in BINARY.  Two internal labels per
 * staged arg based at 20 (this box uses L(1)..L(7)); capped at 8 args so the pair range stays 20..35.  Kill switch: SCRIP_NO_SINK=1 at emit time. */
static std::string stage_arg_inline(int i, int slot, uint64_t stage_fp) {
    bool plc = g_emit_cfg && g_emit_cfg->pl_cells_graph;   /* PL-ZK-5B: pl_cells_graph needs ___RAWQ(slot) not FRQ(slot) -- see dual-write fix. */
    std::string slow = x86("mov32", "edi", (long)i) + x86("mov", "rsi", FRQ(slot)) + x86("mov", "rdx", FRQ(slot + 8)) + x86("call", "rt_arg_stage", stage_fp);
    if (i < 0 || i >= 8 || getenv("SCRIP_NO_SINK")) return slow;
    return x86("lea", "r8", "[rip + __]", (uint64_t)(uintptr_t)&g_gc_pending, "g_gc_pending")
         + x86("mov", "eax", "dword ptr [r8 + 0]")
         + x86("test", "eax", "eax")
         + x86("jne", L(20 + i * 2))
         + x86("mov", "rax", FRQ(slot))
         + x86("mov", "rdx", FRQ(slot + 8))
         + x86("lea", "r8", "[rip + __]", (uint64_t)(uintptr_t)g_call_args, "g_call_args")
         + x86("mov", (std::string("[r8 + ") + std::to_string(i * 16) + "]").c_str(), "rax")
         + x86("mov", (std::string("[r8 + ") + std::to_string(i * 16 + 8) + "]").c_str(), "rdx")
         + x86("jmp", L(21 + i * 2))
         + x86("def", L(20 + i * 2))
         + slow
         + x86("def", L(21 + i * 2));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * bb_chain_terminal_staged(IR_t * entry) { IR_t * n = entry; int guard = 0;
    while (n && n->γ.node && n->γ.node->op != IR_SUCCEED && n->γ.node->op != IR_FAIL && guard++ < 4096) n = n->γ.node;
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static bb_label_t * bb_call_staged_beta_target() {
    for (int i = 0; i < g_emit.xa_bb_emit_pair_n; i++)
        if (g_emit.xa_bb_emit_pair_define[i] == _.lbl_β_p && g_emit.xa_bb_emit_pair_jmp[i])
            return g_emit.xa_bb_emit_pair_jmp[i];
    return _.lbl_ω_p;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* The same search, yielding the pair INDEX (-1 = none, i.e. the β edge is ω).  x86_pair_jmp() is keyed by index
 * and is medium-invisible, so the unified arm needs the index, not the label.  NCB-1b note: the old BIN arm
 * hardcoded x86_pair_jmp(0) while the TEXT arm jumped to the FOUND target — an asymmetry that was latent only
 * while the matching pair happened to be index 0.  Threading the real index closes it in both media at once. */
static int bcps_beta_pair_idx() {
    for (int i = 0; i < g_emit.xa_bb_emit_pair_n; i++)
        if (g_emit.xa_bb_emit_pair_define[i] == _.lbl_β_p && g_emit.xa_bb_emit_pair_jmp[i]) return i;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bcps_arg_slot(IR_t * call, IR_graph_t ** argblks, int i) {
    IR_t * a = ir_call_arg(call, i);
    if (a) { int s = bb_slot_get(a); if (s < 0) s = zls_off(a); if (s >= 0) return s; }
    IR_t * prod = bb_chain_terminal_staged(argblks && argblks[i] ? argblks[i]->entry : NULL); int s = prod ? bb_slot_get(prod) : -1; return s < 0 ? 0 : s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bcps_result_slot() {
    IR_t * nd = _.node;
    { int _s = nd ? zls_off(nd) : -1; if (_s >= 0) { if (bb_slot_get(nd) < 0) bb_slot_register(nd, _s); return _s; } }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int c2farm() { return x86_port_mode() == ZC_PORT_FORTH && _.op_fc_wbytes > 0; }   /* CALL2BB 3b: fc-registered value-spine call (the dispatch preamble armed the RESULT window base=own quad) -- the one arg rides the TOP cell at alpha ([rsp + scc_sb] above the save block), the result replaces it IN PLACE at L(2) (net-zero rsp), and the L(2) FRQ stores self-rebase through the window */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* CALL2BB slice 2 (Lon s21x-c: "DEFINE, when CONSTANT FOLDED, emits exactly TWO BBs: an IR_SAVE_RESTORE and an IR_CALL") — the SCC eligibility PROBE + the role-0 producer→consumer HANDOFF.
 * bb_scc_probe is the BP-7 emit-time predicate factored to ONE body so the role-0 IR_SAVE_RESTORE template (bb_save_restore.cpp) and this consumer compute the SAME answer from the SAME inputs —
 * structural agreement, no drift (a disagreement bombs loudly below).  !is_generator is explicit here: the det arm arrives pre-filtered by its dispatch but role-0 has no such gate, and open_slim's
 * runtime guard declines generators anyway, so the conjunct is redundant-true for this file and load-bearing for role-0.  The handoff is a template-file static (the fc_pair_extent side-table idiom —
 * PEERS RULE: not an IR_t field; not g_emit: DRIVE_FILL owns that lifecycle): role-0 sets it as its string is built, and THIS box consumes-and-clears on its very next build (sx_call_named chains
 * sr0.γ → call adjacently, emission is chain-ordered and single-threaded, so the window is exactly one box).  Every shape surprise is an in-band x86_bomb per RULES — loud over silent. */
extern "C" int bb_scc_probe(const char *fname, int nargs, int *np_out, int *nsave_out, int *gk_out, int *res_gk_out) {
    int np = 0, nsave = 0, res_gk = -1, scc = 0;
    if (x86_zc_frame() == ZC_FRAME_RSP && fname && rt_proc_dyn_scope(fname) && !rt_proc_is_generator(fname) && !getenv("SCRIP_SCC_OFF") && g_gva_active && scc_program_ok() && rt_proc_is_registered(fname)) {
        np = rt_proc_nparams(fname);
        if (np >= 0 && np <= 60 && nargs <= rt_proc_nformals(fname)) {   /* NPSPLIT (s22w): admission is against FORMALS (excess-arg calls fall to the classic arm, which clamps per the manual); the k<np save-set walk below stays FULL-set — np keeps the full-name meaning */
            const char *rn = rt_proc_result_name_get(fname); int ok = rn ? 1 : 0, sh = 0;
            for (int k = 0; ok && k < np; k++) { const char *nm = rt_proc_pname(fname, k); int gk = nm ? gva_index_of(nm) : -1; if (gk < 0) ok = 0; else { gk_out[nsave++] = gk; if (!strcmp(nm, rn)) sh = 1; } }
            if (ok) { res_gk = gva_index_of(rn); if (res_gk < 0) ok = 0; else if (!sh) gk_out[nsave++] = res_gk; }
            if (ok) scc = 1;
        }
    }
    if (np_out) *np_out = np; if (nsave_out) *nsave_out = nsave; if (res_gk_out) *res_gk_out = res_gk; return scc;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Node-exact handoff: BFS emission ORDER interleaves sr0 boxes and their calls freely (measured: two role-0 boxes emitted back-to-back on the operand-position witness), but CONTROL follows the γ edge —
 * sr0 jmps directly to ITS call's α with rax + the live block riding, so runtime adjacency holds regardless of text order.  The consumer key is therefore the CALL NODE POINTER, deposited by the sr0
 * DRIVE arm (drivers own nodes; templates never see them), promoted to ARMED only when the role-0 template actually emits the prefix, and consumed in the call-family drive arm which marshals the clean
 * scalar op_c2 for the template.  Table capacity 64 outstanding pairs; overflow declines the new sr0 to pass-through (safe).  Reset per graph at emit_chain (stale-entry hygiene). */
#define BB_C2H_MAX 64
static struct { const void *call; char name[256]; } g_c2h_tab[BB_C2H_MAX]; static int g_c2h_n;
static struct { const void *call; const char *name; int live; } g_c2h_pend;
extern "C" void bb_scc_handoff_pending_set(const void *call_nd, const char *fname) { g_c2h_pend.call = call_nd; g_c2h_pend.name = fname; g_c2h_pend.live = call_nd && fname ? 1 : 0; }
extern "C" void bb_scc_handoff_pending_clear(void) { g_c2h_pend.live = 0; }
extern "C" int  bb_scc_handoff_arm(void) { if (!g_c2h_pend.live || g_c2h_n >= BB_C2H_MAX) return 0; g_c2h_tab[g_c2h_n].call = g_c2h_pend.call; snprintf(g_c2h_tab[g_c2h_n].name, sizeof g_c2h_tab[g_c2h_n].name, "%s", g_c2h_pend.name); g_c2h_n++; g_c2h_pend.live = 0; return 1; }
extern "C" int  bb_scc_handoff_room(void) { return g_c2h_pend.live && g_c2h_n < BB_C2H_MAX; }
extern "C" int  bb_scc_handoff_consume(const void *call_nd, const char *fname) { for (int i = 0; i < g_c2h_n; i++) if (g_c2h_tab[i].call == call_nd) { int ok = fname && !strcmp(g_c2h_tab[i].name, fname) ? 1 : 0; g_c2h_tab[i] = g_c2h_tab[--g_c2h_n]; return ok ? 1 : -1; } return 0; }
extern "C" void bb_scc_handoff_reset(void) { g_c2h_n = 0; g_c2h_pend.live = 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* NCB-1b — THE DETERMINISTIC CALL SITE, EMITTED.  One R2 concatenation, one medium, invisible: this replaced a
 * hand-written bcps_bin_arm/bcps_txt_arm PAIR (the named FORBIDDEN SHAPE — one instruction stream written
 * twice).  The two arms only ever diverged on the two sanctioned R10 medium-specific encodings — the RO name
 * load and the helper call — and BOTH already have medium-invisible encoders (x86_ro_seal_str/x86_ro_load_q,
 * the bb_lit_scalar pattern; x86_call_ro), so the pair had no reason to exist.  ZERO new encoders were needed:
 * `call rax` (XK_REG, byte-verified vs as: FF D0) already existed, so s24's F3 is satisfied as it stands.
 *
 * THE CONVENTION (Lon s22): C → BB → BB, with C helpers as STRICT LEAVES.  The C trampoline rt_call_proc_descr
 * is GONE from this path; the site now runs the leaves itself and performs the BB→BB transfer directly:
 *
 *     align_enter  →  stage args  →  open leaf  →  sub rsp (THE FRAME)  →  frame_prep leaf  →  call rax  ⭐
 *                  →  epilogue leaf  →  align_leave (THE FRAME RELEASE, for free)
 *
 * WHY THE SHAPE IS SAFE, verified rather than assumed:
 *  · F2 — ⭐ CORRECTED s202: this line read "___ is the align-save register TODAY (x86_align_save() = ___ while
 *    the ζ frame is r12)".  x86_align_save() has ZERO definitions (grep: comments only) and r12 is not a ζ basis
 *    (ZC_FRAME_R12 deleted, ZR-RSP___-1 s201).  x86_align_enter/leave is a PUSH-based dance touching no callee-
 *    saved register, and is a no-op entirely under the RSP default.  The invariant the line was defending still
 *    holds and is what matters here: the anchor
 *    is taken with x86_align_enter/leave, never by hand.  align_leave restores rsp FROM the save reg, which is
 *    why the frame release is free and needs no fbytes at the bottom.  EVERY exit passes through it (hence the
 *    two internal labels): skipping it would leave the save reg holding OUR rsp, and the enclosing graph's
 *    epilogue would then restore the wrong rsp.
 *  · F4 — the callee ABI already admits this call: xa_flat's prologue takes (rdi = frame base, esi = entry),
 *    pushes the ζ frame reg and anchors its own rsp; GVA is register-free since REG-1 (templates read the pinned
 *    island absolute at RT_GVA_VA, so no register carries GVA state across the C window — the durable form of the
 *    083 fix; the interim reload-cell self-load is deleted), and it already dispatches on esi.
 *  · The callee is handed fb = rsp AFTER the bump, so its own pushes land BELOW the frame it is given.
 *  · Register order respects x86_call_ro clobbering rax (movabs rax; call rax): fbytes is copied out of rax
 *    into rsi BEFORE the frame_prep call, and the returned DESCR is moved out of rax:rdx into rdi:rsi BEFORE
 *    the epilogue call — the same marshalling gcc itself emits for this exact sequence (objdump-confirmed).
 *  · DESCR_t is a 16-byte INTEGER-class pair: returned in rax:rdx, passed in rdi:rsi. */
static std::string bcps_det_arm() {
    x86_begin();
    int off = bcps_result_slot(); if (off < 0) return x86_bomb("bb_call_proc_staged: no LOWER slot grant (TMP-ERADICATE)");
    /* ZD-7c USER-PROC ARM (s23r): when the statement is ZD-armed (op_zres=1) each arg's DESCR lives at its own
     * cell [rsp+op_zread[k]] and the result goes to this box's own cell [rsp+0..15].  Three arg-delivery shapes
     * all use ZOPQ instead of FRQ; result writes use ZRES.  Sibling idiom: bb_deref.cpp:13-22.  The arm covers
     * only the ZC_PORT_FORTH / ZC_FRAME_RSP path (the only live basis post ZC_FRAME_R12 deletion); a generator
     * call never reaches here (is_gen pre-filters to bcps_spine_gen_arm before bcps_det_arm is called).
     * KILLSWITCH SCRIP_ZD_PROC=0 prevents admission in zd_wl_kind -- this arm fires iff that gate is open. */
    if (_.op_zres) {
        int bidx_z = bcps_beta_pair_idx(); IR_graph_t ** argblks_z = (IR_graph_t **)(intptr_t)_.op_counter;
        int is_dyn_z = _.op_sval && rt_proc_dyn_scope(_.op_sval);
        long det_idx_z = (!is_dyn_z && _.op_sval) ? (long)rt_proc_index_of(_.op_sval) : -1L;
        int det_nA_z = (int)_.op_ival;
        int det_fuse_z = (det_idx_z >= 0 && x86_zc_frame() == ZC_FRAME_RSP && det_nA_z >= 0 && det_nA_z <= 4);
        int dc_z = 0; uint64_t dc_slot_z = 0; char dc_name_z[280]; dc_name_z[0] = 0;
        if (det_fuse_z && _.op_sval && rt_pl_dc_ok(_.op_sval, det_nA_z)) {
            void **sl = rt_pl_dc_slot(det_idx_z); if (sl) { dc_z = 1; dc_slot_z = (uint64_t)(uintptr_t)sl;
                { char mang[256]; int mi = 0; const char *nm = _.op_sval; for (; *nm && mi < 250; nm++) { unsigned char u = (unsigned char)*nm; if ((u>='A'&&u<='Z')||(u>='a'&&u<='z')||(u>='0'&&u<='9')||u=='_'||u=='$'||u=='.') mang[mi++]=(char)u; else mi+=snprintf(mang+mi,(size_t)(256-mi),"$%02X",u); } mang[mi]=0; snprintf(dc_name_z,sizeof dc_name_z,"proc_%s_dc\xce\xb1",mang); } } }
        static const char *detN_argreg_z[4] = { "rsi", "rdx", "rcx", "r8" };
        uint64_t detN_fp_z[5];
        { void *(*f0)(long) = rt_proc_call_open_det0; detN_fp_z[0] = (uint64_t)(uintptr_t)(void*)f0; }
        { void *(*f1)(long, DESCR_t*) = rt_proc_call_open_det1; detN_fp_z[1] = (uint64_t)(uintptr_t)(void*)f1; }
        { void *(*f2)(long, DESCR_t*, DESCR_t*) = rt_proc_call_open_det2; detN_fp_z[2] = (uint64_t)(uintptr_t)(void*)f2; }
        { void *(*f3)(long, DESCR_t*, DESCR_t*, DESCR_t*) = rt_proc_call_open_det3; detN_fp_z[3] = (uint64_t)(uintptr_t)(void*)f3; }
        { void *(*f4)(long, DESCR_t*, DESCR_t*, DESCR_t*, DESCR_t*) = rt_proc_call_open_det4; detN_fp_z[4] = (uint64_t)(uintptr_t)(void*)f4; }
        static const char *detN_nm_z[5] = { "rt_proc_call_open_det0","rt_proc_call_open_det1","rt_proc_call_open_det2","rt_proc_call_open_det3","rt_proc_call_open_det4" };
        uint64_t open_fp_z;  { long (*fp)(const char *, int) = rt_proc_call_open; open_fp_z = (uint64_t)(uintptr_t)(void*)fp; }
        uint64_t openfn_fp_z; { void * (*fp)(void) = rt_proc_open_fn; openfn_fp_z = (uint64_t)(uintptr_t)(void*)fp; }
        uint64_t epig_fp_z;  { DESCR_t (*fp)(DESCR_t) = rt_proc_call_epilogue_γ; epig_fp_z = (uint64_t)(uintptr_t)(void*)fp; }
        uint64_t epiw_fp_z;  { DESCR_t (*fp)(void) = rt_proc_call_epilogue_ω; epiw_fp_z = (uint64_t)(uintptr_t)(void*)fp; }
        uint64_t fail_fp_z;  { DESCR_t (*fp)(void) = rt_faildescr; fail_fp_z = (uint64_t)(uintptr_t)(void*)fp; }
        uint64_t det_fp_z; { void * (*fp)(long, int) = rt_proc_call_open_det; det_fp_z = (uint64_t)(uintptr_t)(void*)fp; }
        int scc_z = 0, scc_np_z = 0, scc_nsave_z = 0, scc_res_gk_z = -1; int scc_gk_z[64];
        scc_z = bb_scc_probe(_.op_sval, (int)_.op_ival, &scc_np_z, &scc_nsave_z, scc_gk_z, &scc_res_gk_z);
        long scc_sb_z = 16L * (long)scc_nsave_z;
        uint64_t trap_fp_z; { void (*fp)(void) = rt_c2b_arm_trap; trap_fp_z = (uint64_t)(uintptr_t)(void*)fp; }
        long scc_fp_oz; { long (*fp)(const char *, int, int) = rt_proc_call_open_slim; scc_fp_oz = (long)(uint64_t)(uintptr_t)(void*)fp; }
        long scc_fp_gz; { DESCR_t (*fp)(DESCR_t) = rt_proc_call_epilogue_slim_γ; scc_fp_gz = (long)(uint64_t)(uintptr_t)(void*)fp; }
        long scc_fp_wz; { DESCR_t (*fp)(void) = rt_proc_call_epilogue_slim_ω; scc_fp_wz = (long)(uint64_t)(uintptr_t)(void*)fp; }
        uint64_t dc_slot_fp_z = dc_slot_z;
        uint64_t stage_fp_z; { void (*fp)(int, DESCR_t) = rt_arg_stage; stage_fp_z = (uint64_t)(uintptr_t)(void*)fp; }
        return x86_alpha()
             + x86_scan_sync_out()
             + x86_anchor_enter()
             + (scc_z
                ? [&]() -> std::string {
                    /* TINY-SITE s57 ZD twin (Lon in-chat: one-shot glue to the DEFINE shim; ASM staging, no veneers, no open_slim/epilogue crossings).  Same gate, arm and residue notes as the non-ZD twin below; args read from their ZD cells (ZOPQ). */
                    static int _ntz = -1; if (_ntz < 0) { const char * _e = getenv("SCRIP_NO_TINY"); _ntz = (_e && *_e == '1') ? 1 : 0; }
                    if (!_ntz && MEDIUM_TEXT && _.op_sval && rt_define_tiny_ok(_.op_sval, (int)_.op_ival)) {   /* TINY-SITE s57: TEXT-only this seat (m3 cross-chain rel32 owed) */
                        std::string shim = std::string(_.op_sval) + "_shim";
                       /* TINY-SITE s57 m3 LIMITATION (owed): X-record rel32 cannot patch across per-chain buffers, so BINARY takes tiny only when the shim is already defined in THIS emission session (stub-before-main ordering); otherwise the byte-identical slim arm below. TEXT: gas resolves the name at assembly. */
                        int rgz = scc_res_gk_z < 0 ? 0 : scc_res_gk_z;
                        auto ZOPQT = [&](int i, int w) { return x86_zref(_.op_zread[i] + w, 1); };
                        return FOR(0, (int)_.op_ival, [&](int i) {
                                   return x86("mov", "rax", ZOPQT(i, 0)) + x86("mov", "rdx", ZOPQT(i, 8))
                                        + x86("lea", "r8", std::string("[rip + __]"), (uint64_t)(uintptr_t)g_call_args, "g_call_args")
                                        + x86("mov", (std::string("[r8 + ") + std::to_string(i * 16) + "]").c_str(), "rax")
                                        + x86("mov", (std::string("[r8 + ") + std::to_string(i * 16 + 8) + "]").c_str(), "rdx"); })
                             + x86("lea", "r10", L(6))
                             + x86("lea", "r11", L(7))
                             + x86_jmp_ext(emit_label_intern(shim.c_str()))
                             + x86("def", L(6))
                             + x86("note", gva_name(rgz)) + x86("mov", "rax", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(rgz, 0) : ABSQ(RT_GVA_VA + (unsigned long)rgz * 16))
                             + x86("note", gva_name(rgz)) + x86("mov", "rdx", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(rgz, 8) : ABSQ(RT_GVA_VA + (unsigned long)rgz * 16 + 8))
                             + x86("jmp", L(2))
                             + x86("def", L(7))
                             + x86("mov32", "eax", (long)DT_FAIL) + x86("xor", "edx", "edx")
                             + x86("jmp", L(2));
                    }
                    _tiny_declined_z: ;
                    /* RTX-FUNC-0 (ZD AB-3b): when this program has DEFINE activation blocks (ab_n>0), replace
                     * rt_arg_stage×n + open_slim + open_fn + epilogue with: save-set spill (same as non-ZD),
                     * install actuals from their ZD-frame cells (ZOPQ) directly into formal GVA slots, pass
                     * γ/ω wire addresses in rcx/rdx, jmp [fn_cell$FN] → FN_act_α.  The activation block
                     * handles Σ/vtmark/k_level/null-result+locals/body dispatch (rt_ab_enter_env).
                     * L(10)=γ landing, L(11)=ω landing: result already in rax:rdx from rt_ab_leave_env.
                     * Arg i installs into scc_gk_z[i] (probe layout: params first in prototype order, then
                     * result — no offset needed; this differs from the non-ZD arm's i+1 which is a bug there).
                     * Falls back to the classic open_slim body when ab_n==0 (SCRIP_AB=0 hatch). */
                    bool ab3b_z = (g_emit_cfg && g_emit_cfg->ab_n > 0 && _.op_sval);
                    if (ab3b_z) {
                        std::string fn_cell_lbl_z = std::string("fn_cell$") + _.op_sval;
                        void * fn_cell_bin_z = bb_ab_fn_cell_ptr(_.op_sval);
                        /* RTX-FUNC-0 FIX: install actuals from ZOPQ BEFORE sub rsp so the ZD-frame
                         * displacements are still valid (ZOPQ is relative to rsp at box entry; sub rsp
                         * would shift every ZOPQ address by scc_sb_z, corrupting the reads).  Then spill
                         * the save-set, which no longer needs the ZOPQ cells and uses rsp-relative stores. */
                        /* RTX-FUNC recursion fix (2026-08-10, convicted by r_keepn: caller reads its formal after the inner call and gets the INNER ACTUAL — AB=0 z1 vs AB=1 z0): the pushdown record must hold the CALLER'S LIVE values, so the save-set spill now runs BEFORE the actual install, not after.  Manual Ch.8: formals/locals are saved on the pushdown stack at entry, and only THEN do the formals receive the actuals — the old order saved the actuals over the caller's live environment, which is invisible for sequential calls (twice/nested/r_plain all green) and fatal the moment two frames of one proc are live. */
                        /* ZOPQ is rsp-relative to box entry, so reading it after `sub rsp` needs the displacement shifted by scc_sb_z — exactly the compensation the classic ab3b arm already applies with FRQB(slot, scc_sb).  Built on the SAME x86_zref inline ZOPQ itself uses, so x86_asm.h stays untouched (NOT-CONCURRENCY-SAFE this seat). */
                        auto ZOPQC = [&](int i, int w) { return x86_zref(_.op_zread[i] + w + (int)scc_sb_z, 1); };
                        /* Landing-side formals restore: beta restores the whole save-set from the CALLEE frame, whose formal slots were spilled post-install and therefore hold that call's own actuals.  The caller block (spilled pre-install, just above) is the correct snapshot, so the landings put the formals back after beta has run.  Caller-side restore at the landings is the established BP-7 SCC convention, not a new one.  ABSQ ONLY and rcx scratch: rax:rdx carry the result here, and beta clobbers r9 (movzx r9,cl), so GVARQ's r9=GVA-base claim is dead at this point — the same reasoning that kept beta's restore ABSQ-only. */
                        auto ab_formals_restore = [&]() { return FOR(0, scc_np_z, [&](int i) {
                              return x86("note", gva_name(scc_gk_z[i]))
                                   + x86_rsp_load64("rcx", 16 * i)     + x86("mov", ABSQ(RT_GVA_VA + (unsigned long)scc_gk_z[i] * 16),     "rcx")
                                   + x86_rsp_load64("rcx", 16 * i + 8) + x86("mov", ABSQ(RT_GVA_VA + (unsigned long)scc_gk_z[i] * 16 + 8), "rcx"); }); };
                        return x86("sub", "rsp", scc_sb_z)
                            + FOR(0, scc_nsave_z, [&](int k) {
                                  return x86("note", gva_name(scc_gk_z[k])) + x86("mov", "rax", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk_z[k], 0) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk_z[k] * 16)) + x86_rsp_store64(16 * k, "rax")
                                       + x86("note", gva_name(scc_gk_z[k])) + x86("mov", "rax", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk_z[k], 8) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk_z[k] * 16 + 8)) + x86_rsp_store64(16 * k + 8, "rax"); })
                            + FOR(0, (int)_.op_ival, [&](int i) {
                                  int gk_iz = scc_gk_z[i];
                                  return x86("note", ZOPN(i)) + x86("mov", "rax", ZOPQC(i, 0))
                                       + x86("note", gva_name(gk_iz)) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(gk_iz, 0) : ABSQ(RT_GVA_VA + (unsigned long)gk_iz * 16), "rax")
                                       + x86("note", ZOPN(i)) + x86("mov", "rax", ZOPQC(i, 8))
                                       + x86("note", gva_name(gk_iz)) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(gk_iz, 8) : ABSQ(RT_GVA_VA + (unsigned long)gk_iz * 16 + 8), "rax"); })
                            + x86("lea", "rcx", L(10))
                            + x86("lea", "rdx", L(11))
                            + (MEDIUM_BINARY && fn_cell_bin_z
                                ? x86("movabs", "rax", (uint64_t)(uintptr_t)fn_cell_bin_z) + x86("mov", "rax", RDQ("rax", 0)) + x86("jmp", "rax")
                                : x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)0, fn_cell_lbl_z.c_str()) + x86("mov", "rax", RDQ("rax", 0)) + x86("jmp", "rax"))
                            + x86("def", L(10)) + ab_formals_restore() + x86("add", "rsp", scc_sb_z) + x86("jmp", L(2))
                            + x86("def", L(11)) + ab_formals_restore() + x86("add", "rsp", scc_sb_z) + x86("jmp", L(2));
                    }
                    /* Classic ZD SCC path (SCRIP_AB=0 or no DEFINE in this program) */
                    return FOR(0, (int)_.op_ival, [&](int i) {
                               return x86("mov32", "edi", (long)i) + x86("note", ZOPN(i)) + x86("mov", "rsi", ZOPQ(i, 0)) + x86("note", ZOPN(i)) + x86("mov", "rdx", ZOPQ(i, 8)) + x86("call", "rt_arg_stage", stage_fp_z); })
                        + x86("sub", "rsp", scc_sb_z)
                        + FOR(0, scc_nsave_z, [&](int k) {
                              return x86("note", gva_name(scc_gk_z[k])) + x86("mov", "rax", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk_z[k], 0) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk_z[k] * 16)) + x86_rsp_store64(16 * k, "rax")
                                   + x86("note", gva_name(scc_gk_z[k])) + x86("mov", "rax", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk_z[k], 8) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk_z[k] * 16 + 8)) + x86_rsp_store64(16 * k + 8, "rax"); })
                        + x86_ro_load_q("rdi", 0)
                        + x86("mov32", "esi", (long)scc_np_z)
                        + x86("mov32", "edx", (long)_.op_ival)
                        + x86("call", "rt_proc_call_open_slim", (uint64_t)scc_fp_oz)
                        + x86("test", "rax", "rax")
                        + x86("je", L(5))
                        + x86("mov", "r10", "rax")   /* GLOBALS-GONE s55: open_slim's return IS the transfer target (rax channel, record eradicated); parked in r10 across the install loop (rax/r8 clobbered there) — r10 is dead until the callee's role-3 adopt overwrites it from rcx */
                        + FOR(0, (int)_.op_ival, [&](int i) {
                              return x86("lea", "r8", "[rip + __]", (uint64_t)(uintptr_t)g_call_args, "g_call_args")
                                   + x86("mov", "rax", (std::string("[r8 + ") + std::to_string(i * 16) + "]").c_str())
                                   + x86("note", gva_name(scc_gk_z[i])) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk_z[i], 0) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk_z[i] * 16), "rax")
                                   + x86("mov", "rax", (std::string("[r8 + ") + std::to_string(i * 16 + 8) + "]").c_str())
                                   + x86("note", gva_name(scc_gk_z[i])) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk_z[i], 8) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk_z[i] * 16 + 8), "rax"); })
                        + x86("mov", "rax", "r10")   /* GLOBALS-GONE s55: rt_proc_open_fn crossing DELETED — the record it read is gone; fn rode the open return */
                        + bb_glue_pass_wires_blob(6, 7)   /* ⭐ FUNCTION LINKAGE s55 (Lon in-chat): "R10 and R11 for success and fail return address ... just like any BB BLOB interface" — site-set wires, adopt hop DELETED (role-3 emits nothing) */
                        + x86("def", L(6))
                        + x86("note", gva_name((scc_res_gk_z < 0 ? 0 : scc_res_gk_z))) + x86("mov", "rdi", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ((scc_res_gk_z < 0 ? 0 : scc_res_gk_z), 0) : ABSQ(RT_GVA_VA + (unsigned long)(scc_res_gk_z < 0 ? 0 : scc_res_gk_z) * 16))
                        + x86("note", gva_name((scc_res_gk_z < 0 ? 0 : scc_res_gk_z))) + x86("mov", "rsi", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ((scc_res_gk_z < 0 ? 0 : scc_res_gk_z), 8) : ABSQ(RT_GVA_VA + (unsigned long)(scc_res_gk_z < 0 ? 0 : scc_res_gk_z) * 16 + 8))
                        + FOR(0, scc_nsave_z, [&](int j) { int k = scc_nsave_z - 1 - j;
                              return x86_rsp_load64("rax", 16 * k) + x86("note", gva_name(scc_gk_z[k])) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk_z[k], 0) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk_z[k] * 16), "rax")
                                   + x86_rsp_load64("rax", 16 * k + 8) + x86("note", gva_name(scc_gk_z[k])) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk_z[k], 8) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk_z[k] * 16 + 8), "rax"); })
                        + x86("add", "rsp", scc_sb_z)
                        + x86("call", "rt_proc_call_epilogue_slim_γ", (uint64_t)scc_fp_gz)
                        + x86("jmp", L(2))
                        + x86("def", L(7))
                        + FOR(0, scc_nsave_z, [&](int j) { int k = scc_nsave_z - 1 - j;
                              return x86_rsp_load64("rax", 16 * k) + x86("note", gva_name(scc_gk_z[k])) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk_z[k], 0) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk_z[k] * 16), "rax")
                                   + x86_rsp_load64("rax", 16 * k + 8) + x86("note", gva_name(scc_gk_z[k])) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk_z[k], 8) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk_z[k] * 16 + 8), "rax"); })
                        + x86("add", "rsp", scc_sb_z)
                        + x86("call", "rt_proc_call_epilogue_slim_ω", (uint64_t)scc_fp_wz)
                        + x86("jmp", L(2))
                        + x86("def", L(5))
                        + x86("add", "rsp", scc_sb_z);
                }()
                : std::string(""))
             + (!scc_z && dc_z
                ? FOR(0, det_nA_z, [&](int i) { return x86("note", ZOPN(i)) + x86("lea", detN_argreg_z[i], ZOPQ(i, 0)); })
                + x86_call_dc(dc_name_z, dc_slot_fp_z)
                + x86("jmp", L(2))
                : std::string(""))
             + (!scc_z && !dc_z
                ? ((det_fuse_z
                    ? x86("mov32", "edi", det_idx_z)
                    + FOR(0, det_nA_z, [&](int i) { return x86("note", ZOPN(i)) + x86("lea", detN_argreg_z[i], ZOPQ(i, 0)); })
                    + x86("call", detN_nm_z[det_nA_z], detN_fp_z[det_nA_z])
                    : ((det_idx_z >= 0
                        ? x86("mov32", "edi", (long)det_idx_z)
                        + x86("mov32", "esi", (long)_.op_ival)
                        + x86("call", "rt_proc_call_open_det", (uint64_t)det_fp_z)
                        : FOR(0, (int)_.op_ival, [&](int i) { uint64_t stage_fp_z; { void (*fp)(int, DESCR_t) = rt_arg_stage; stage_fp_z = (uint64_t)(uintptr_t)(void*)fp; } return x86("mov32", "edi", (long)i) + x86("note", ZOPN(i)) + x86("mov", "rsi", ZOPQ(i, 0)) + x86("note", ZOPN(i)) + x86("mov", "rdx", ZOPQ(i, 8)) + x86("call", "rt_arg_stage", stage_fp_z); })
                        + x86_ro_load_q("rdi", 0)
                        + x86("mov32", "esi", (long)_.op_ival)
                        + x86("call", "rt_proc_call_open", open_fp_z))))
                   + x86("test", "rax", "rax")
                   + x86("je", L(1))
                   + (det_idx_z >= 0 && det_fuse_z ? std::string("") : x86("call", "rt_proc_open_fn", openfn_fp_z))
                   + bb_glue_pass_wires(3, 4)
                   + x86("def", L(3))
                   + x86("call", "rt_proc_call_epilogue_γ", epig_fp_z)
                   + x86("jmp", L(2))
                   + x86("def", L(4))
                   + x86("call", "rt_proc_call_epilogue_ω", epiw_fp_z)
                   + x86("jmp", L(2))
                   + x86("def", L(1))
                   + x86("call", "rt_faildescr", fail_fp_z))
                : std::string(""))
             + x86("def", L(2))
             + x86_anchor_leave()
             + x86_scan_sync_in_rr()
             + x86("note", ZRESN()) + x86("mov", ZRES(0), "rax")
             + x86("note", ZRESN()) + x86("mov", ZRES(8), "rdx")
             + x86("cmp", "eax", (long)DT_FAIL)
             + x86_omega("je")
             + x86_gamma()
             + x86_beta()
             + (bidx_z < 0 ? x86_omega() : x86_pair_jmp(bidx_z))
             + x86_ro_seal_str(0, _.op_sval ? _.op_sval : "");   /* ZD-7c: result in rax:rdx from epilogue → own cell via ZRES; args read at ZOPQ(k,0/8); LEAs for fused-open/DC arms use lea reg, ZOPQ(k,0) (cell address, same x86_parse XK_RSP route as bb_cmp_test/bb_coerce_numeric). Generators excluded: is_gen pre-filters to bcps_spine_gen_arm before this fn; only det procs with a ZD arm land here. */
    }
    int bidx = bcps_beta_pair_idx(); IR_graph_t ** argblks = (IR_graph_t **)(intptr_t)_.op_counter;
    uint64_t stage_fp; { void (*fp)(int, DESCR_t) = rt_arg_stage; stage_fp = (uint64_t)(uintptr_t)(void*)fp; }
    uint64_t open_fp;  { long (*fp)(const char *, int) = rt_proc_call_open; open_fp = (uint64_t)(uintptr_t)(void*)fp; }
    uint64_t openfn_fp; { void * (*fp)(void) = rt_proc_open_fn; openfn_fp = (uint64_t)(uintptr_t)(void*)fp; }
    uint64_t prep_fp;  { void * (*fp)(void *, long) = rt_frame_prep; prep_fp = (uint64_t)(uintptr_t)(void*)fp; }
    uint64_t epig_fp;  { DESCR_t (*fp)(DESCR_t) = rt_proc_call_epilogue_γ; epig_fp = (uint64_t)(uintptr_t)(void*)fp; }
    uint64_t epiw_fp;  { DESCR_t (*fp)(void) = rt_proc_call_epilogue_ω; epiw_fp = (uint64_t)(uintptr_t)(void*)fp; }
    uint64_t epir_fp;  { DESCR_t (*fp)(DESCR_t) = rt_proc_call_epilogue_ret; epir_fp = (uint64_t)(uintptr_t)(void*)fp; }
    uint64_t fail_fp;  { DESCR_t (*fp)(void) = rt_faildescr; fail_fp = (uint64_t)(uintptr_t)(void*)fp; }
    /* PROC-CONV regime selector — NCB-1d (Lon "RSP/___ FORTH ζ for ALL, sharing the C stack", s90): under ZC_FRAME_RSP EVERY det proc call takes the jmp-entry wire — dyn (SNOBOL4) unchanged, det LEXICAL
     * (Icon/Raku) newly joined: the callee blob self-allocates and its lexprep prologue tail binds the staged g_call_args, so the caller-made-frame window below is LEGACY-CONFIG ONLY now (both its arms).
     * The rt table is populated before any emission in both media, so this is emit-time-static.  Congruence with the callee side: emit_jmp_entry_for_proc admits exactly the !is_generator procs, and this
     * det arm is dispatched exactly on !rt_proc_is_generator — one truth source (scrip.c rt_proc_set_generator from proc_table). */
    int is_dyn = _.op_sval && rt_proc_dyn_scope(_.op_sval);
    uint64_t det_fp; { void * (*fp)(long, int) = rt_proc_call_open_det; det_fp = (uint64_t)(uintptr_t)(void*)fp; }
    long det_idx = (!is_dyn && _.op_sval) ? (long)rt_proc_index_of(_.op_sval) : -1L;
    /* PL-REGAIN-4 (2026-07-19): FUSED OPEN — for a det-eligible site with ≤4 args the stage×nargs+open_det pair collapses to ONE crossing: the site leas each arg's CALLER-FRAME CELL ADDRESS into the SysV
     * slots (rsi rdx rcx r8; rdi = idx) and rt_proc_call_open_detN copies through the pointers into g_call_args itself.  Arg PLACEMENT is unchanged (slice-B residency decision stays with Lon); only the
     * crossing count moves: det caller side is now open_detN + epilogue = 2.  nargs > 4 (none on the hot corpus) falls through to the classic stage chain + open_det verbatim. */
    uint64_t detN_fp[5]; static const char *detN_nm[5] = { "rt_proc_call_open_det0", "rt_proc_call_open_det1", "rt_proc_call_open_det2", "rt_proc_call_open_det3", "rt_proc_call_open_det4" };
    { void *(*f0)(long) = rt_proc_call_open_det0; detN_fp[0] = (uint64_t)(uintptr_t)(void*)f0; }
    { void *(*f1)(long, DESCR_t*) = rt_proc_call_open_det1; detN_fp[1] = (uint64_t)(uintptr_t)(void*)f1; }
    { void *(*f2)(long, DESCR_t*, DESCR_t*) = rt_proc_call_open_det2; detN_fp[2] = (uint64_t)(uintptr_t)(void*)f2; }
    { void *(*f3)(long, DESCR_t*, DESCR_t*, DESCR_t*) = rt_proc_call_open_det3; detN_fp[3] = (uint64_t)(uintptr_t)(void*)f3; }
    { void *(*f4)(long, DESCR_t*, DESCR_t*, DESCR_t*, DESCR_t*) = rt_proc_call_open_det4; detN_fp[4] = (uint64_t)(uintptr_t)(void*)f4; }
    static const char *detN_argreg[4] = { "rsi", "rdx", "rcx", "r8" };
    int det_nA = (int)_.op_ival; int det_fuse = (det_idx >= 0 && x86_zc_frame() == ZC_FRAME_RSP && det_nA >= 0 && det_nA <= 4);
    /* PL-DC (REGAIN-1 SLICE C, 2026-07-20 s108) — the DIRECT det call: `call proc_X_dcα` (m4 named / m3 through the fixed dc slot), args as CELL POINTERS in the SAME rsi/rdx/rcx/r8 the fused open
     * took, result lands rax:rdx by the callee's ret-shims — no open crossing, no wire leas, no landing pair, no epilogue calls; the callee's stub+prep+leave carry the whole per-call residue.
     * Eligibility = the callee-side table predicate verbatim (rt_pl_dc_ok: registered !dyn !gen nparams==nargs<=4 jmp_entry, hatch SCRIP_NO_DC) so site and stub agree by construction; any decline
     * falls through to the fused arm unchanged. */
    int dc = (det_fuse && _.op_sval && rt_pl_dc_ok(_.op_sval, det_nA));
    uint64_t dc_slot = 0; char dc_name[280]; dc_name[0] = 0;
    if (dc) { void **sl = rt_pl_dc_slot(det_idx); if (!sl) dc = 0; else { dc_slot = (uint64_t)(uintptr_t)sl;
        { char mang[256]; int mi = 0; const char *nm = _.op_sval; for (; *nm && mi < 250; nm++) { unsigned char u = (unsigned char) *nm; if ((u >= 'A' && u <= 'Z') || (u >= 'a' && u <= 'z') || (u >= '0' && u <= '9') || u == '_' || u == '$' || u == '.') mang[mi++] = (char) u; else mi += snprintf(mang + mi, (size_t) (256 - mi), "$%02X", u); } mang[mi] = 0; snprintf(dc_name, sizeof dc_name, "proc_%s_dc\xce\xb1", mang); } } }
    /* BP-7 SCC — STATIC SAVE-SET CALL CONVENTION (GOAL-SNOBOL4-BB BP-7).  Emit-time eligibility: literal target, registered dyn-scope table proc, every save-set name (formals+locals per the DEFINE
     * prototype, plus the result name unless shadowed by a formal) GVA-resident, nargs within the prototype, program free of OPSYN/UNLOAD (scc_program_ok), hatch SCRIP_SCC_OFF unset.  The arm saves
     * the old cell values inline (GVA absolute -> an rsp block below the anchor), calls the open_slim leaf (guards re-checked with ZERO side effects before commit -- a decline falls through L(5) into
     * the classic sequence verbatim), installs the staged-slot args inline (rsp-load compensated by the block size while the bump is live), and jmps the wire; the L(6)/L(7) landings read the result
     * from the result cell BEFORE restoring, restore the save-set in reverse, and close through the slim epilogue leaves.  Runtime residue (Sigma, pcall, wn/NRETURN, monitor, k_level, vtmark) stays
     * in the slim leaves; rt_name_save_push/restore and rt_arg_stage vanish from this path.  ZC_FRAME_RSP only: the callee's LIFO exits land at the jmp's rsp, so the save block is live at both wires. */
    long scc_fp_o; { long (*fp)(const char *, int, int) = rt_proc_call_open_slim; scc_fp_o = (long)(uint64_t)(uintptr_t)(void*)fp; }
    uint64_t trap_fp; { void (*fp)(void) = rt_c2b_arm_trap; trap_fp = (uint64_t)(uintptr_t)(void*)fp; }
    long scc_fp_g; { DESCR_t (*fp)(DESCR_t) = rt_proc_call_epilogue_slim_γ; scc_fp_g = (long)(uint64_t)(uintptr_t)(void*)fp; }
    long scc_fp_w; { DESCR_t (*fp)(void) = rt_proc_call_epilogue_slim_ω; scc_fp_w = (long)(uint64_t)(uintptr_t)(void*)fp; }
    int scc = 0, scc_np = 0, scc_nsave = 0, scc_res_gk = -1; int scc_gk[64];
    scc = bb_scc_probe(_.op_sval, (int)_.op_ival, &scc_np, &scc_nsave, scc_gk, &scc_res_gk);   /* CALL2BB slice 2: the inline BP-7 predicate factored to the shared probe above (identical answer by construction; is_dyn now inside) */
    int c2 = _.op_c2 == 1 ? 1 : 0;
    { static int _td=-1; if(_td<0)_td=getenv("SCRIP_TINY_DIAG")?1:0; if(_td) fprintf(stderr,"[TINYX] fn=%s nargs=%ld scc=%d c2=%d\n", _.op_sval?_.op_sval:"?",(long)_.op_ival,scc,c2); }
    if (_.op_c2 < 0) return x86_alpha() + x86_bomb("bb_call_proc_staged: CALL2BB handoff callee-name mismatch for this exact call node (producer/consumer drift)");
    if (c2 && !scc)  return x86_alpha() + x86_bomb("bb_call_proc_staged: CALL2BB consumer probe disagrees with the role-0 producer that armed for this node (structural drift — bb_scc_probe is supposed to make this impossible)");
    if (c2farm() && (!scc || (int)_.op_ival != 1)) return x86_alpha() + x86_bomb("bb_call_proc_staged: fc-armed call without SCC 1-arg shape (CALL2BB 3b v1) — the flat fallback does not exist as storage on an armed statement; registration and the probe disagreed");
    long scc_sb = 16L * (long)scc_nsave;
    return x86_alpha()
         + (c2 ? IF(g_scan_regs_live, x86("push", "rax") + x86("push", "rax")) + x86_scan_sync_out() + IF(g_scan_regs_live, x86("pop", "rax") + x86("pop", "rax")) : x86_scan_sync_out())   /* c2: rax carries sr0's open_slim outcome across the box edge; sync_out's C call clobbers it, so the pair (two pushes = alignment held) shields it — emits nothing when the site is not scan-live */
         + x86_anchor_enter()
         + (c2
            /* CALL2BB slice 2 — STAGED-BOX SKIP: the role-0 IR_SAVE_RESTORE box just ahead of this one carved the save block (still LIVE at rsp on the committed path), spilled the save-set, ran
             * open_slim, and installed the staged args into the NV globals.  rax==1 = slim record OPEN → transfer here (landings restore + release sr0's block through the slim epilogues, exactly the
             * merged shape); rax==0 = runtime decline (redefined / fastpath-off / prototype drift) → sr0 already released its block → fall to L(5) = the classic sequence verbatim, at base depth. */
            ? x86("test", "rax", "rax")
            + x86("je", L(5))
            + bb_glue_pass_wires_blob(6, 7)   /* FUNCTION LINKAGE s55: open_slim's return IS the target (rax channel; rt_proc_open_fn crossing DELETED — read an eradicated record); site-set r10/r11 wires */
            + x86("def", L(6))
            + x86("note", gva_name((scc_res_gk < 0 ? 0 : scc_res_gk))) + x86("mov", "rdi", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ((scc_res_gk < 0 ? 0 : scc_res_gk), 0) : ABSQ(RT_GVA_VA + (unsigned long)(scc_res_gk < 0 ? 0 : scc_res_gk) * 16))
            + x86("note", gva_name((scc_res_gk < 0 ? 0 : scc_res_gk))) + x86("mov", "rsi", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ((scc_res_gk < 0 ? 0 : scc_res_gk), 8) : ABSQ(RT_GVA_VA + (unsigned long)(scc_res_gk < 0 ? 0 : scc_res_gk) * 16 + 8))
            + FOR(0, scc_nsave, [&](int j) { int k = scc_nsave - 1 - j;
                  return x86_rsp_load64("rax", 16 * k) + x86("note", gva_name(scc_gk[k])) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk[k], 0) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk[k] * 16), "rax")
                       + x86_rsp_load64("rax", 16 * k + 8) + x86("note", gva_name(scc_gk[k])) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk[k], 8) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk[k] * 16 + 8), "rax"); })
            + x86("add", "rsp", scc_sb)
            + x86("call", "rt_proc_call_epilogue_slim_γ", (uint64_t)scc_fp_g)
            + x86("jmp", L(2))
            + x86("def", L(7))
            + FOR(0, scc_nsave, [&](int j) { int k = scc_nsave - 1 - j;
                  return x86_rsp_load64("rax", 16 * k) + x86("note", gva_name(scc_gk[k])) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk[k], 0) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk[k] * 16), "rax")
                       + x86_rsp_load64("rax", 16 * k + 8) + x86("note", gva_name(scc_gk[k])) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk[k], 8) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk[k] * 16 + 8), "rax"); })
            + x86("add", "rsp", scc_sb)
            + x86("call", "rt_proc_call_epilogue_slim_ω", (uint64_t)scc_fp_w)
            + x86("jmp", L(2))
            + x86("def", L(5))
            + IF(c2farm(), x86("call", "rt_c2b_arm_trap", trap_fp))
            : std::string(""))
         + (scc && !c2
            ? [&]() -> std::string {
                /* ⭐⭐⭐ TINY-SITE s57 (Lon in-chat, roman.s n201: "That needs all be DELETED and REPLACED with a simple ONE-SHOT GLUE to the SHIM that is created by DEFINE. The CALL sites are TINY.
                 * You have it backwards."  And: "Is rt_arg_stage a C function? If so then NOT. Make it ASM and DO NOT have RTCC VENEERS there").  The folded full-arity class collapses to: inline
                 * g_call_args stores (raw ASM — rt_arg_stage crossing and its RTCC veneers DELETED from this arm) + site-set r10/r11 wires + ONE direct jmp-ext to the DEFINE-created <FN>_shim
                 * (bb_save_restore role 3), which installs formals and falls into the fold transfer.  γ landing reads the result GVA cell inline; ω loads FAILDESCR inline — open_slim and both
                 * epilogue_slim crossings DELETED here.  Eligibility EMIT-TIME (rt_define_tiny_ok: registered dyn_scope !gen !variadic !redefined, nargs==nformals>0); everything else keeps the slim
                 * arm below (its NULVCL under-arity pad still matters there).  Residue owed to the return-side rung: k_level, result/locals NULVCL, GC-pending shield on the stores.  SCRIP_NO_TINY=1
                 * restores the slim arm byte-identically. */
                static int _ntiny = -1; if (_ntiny < 0) { const char * _e = getenv("SCRIP_NO_TINY"); _ntiny = (_e && *_e == '1') ? 1 : 0; }
                { static int _td=-1; if(_td<0)_td=getenv("SCRIP_TINY_DIAG")?1:0; if(_td) fprintf(stderr,"[TINY] fn=%s nargs=%ld ok=%d scc=%d c2=%d\n", _.op_sval?_.op_sval:"?",(long)_.op_ival,_.op_sval?rt_define_tiny_ok(_.op_sval,(int)_.op_ival):-1,scc,c2); }
                if (!_ntiny && MEDIUM_TEXT && _.op_sval && rt_define_tiny_ok(_.op_sval, (int)_.op_ival)) {   /* TINY-SITE s57: TEXT-only this seat */
                    std::string shim = std::string(_.op_sval) + "_shim";
                       /* TINY-SITE s57 m3 LIMITATION (owed): X-record rel32 cannot patch across per-chain buffers, so BINARY takes tiny only when the shim is already defined in THIS emission session (stub-before-main ordering); otherwise the byte-identical slim arm below. TEXT: gas resolves the name at assembly. */
                    int rg = scc_res_gk < 0 ? 0 : scc_res_gk;
                    return FOR(0, (int)_.op_ival, [&](int i) { int slot = bcps_arg_slot(_.node, argblks, i);
                               return x86("mov", "rax", FRQ(slot)) + x86("mov", "rdx", FRQ(slot + 8))
                                    + x86("lea", "r8", std::string("[rip + __]"), (uint64_t)(uintptr_t)g_call_args, "g_call_args")
                                    + x86("mov", (std::string("[r8 + ") + std::to_string(i * 16) + "]").c_str(), "rax")
                                    + x86("mov", (std::string("[r8 + ") + std::to_string(i * 16 + 8) + "]").c_str(), "rdx"); })
                         + x86("lea", "r10", L(6))
                         + x86("lea", "r11", L(7))
                         + x86_jmp_ext(emit_label_intern(shim.c_str()))
                         + x86("def", L(6))
                         + x86("note", gva_name(rg)) + x86("mov", "rax", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(rg, 0) : ABSQ(RT_GVA_VA + (unsigned long)rg * 16))
                         + x86("note", gva_name(rg)) + x86("mov", "rdx", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(rg, 8) : ABSQ(RT_GVA_VA + (unsigned long)rg * 16 + 8))
                         + x86("jmp", L(2))
                         + x86("def", L(7))
                         + x86("mov32", "eax", (long)DT_FAIL) + x86("xor", "edx", "edx")
                         + x86("jmp", L(2));
                }
                /* AB-3b path: when this program has DEFINE activation blocks (SCRIP_AB on, ab_n>0),
                 * replace open_slim+open_fn+arg-install with: save-set spill, install actuals into
                 * formal GVA cells, pass wires rcx/rdx, jmp [fn_cell$FN] → FN_act_α.
                 * The activation block handles Σ/vtmark/k_level/null-result+locals/body dispatch.
                 * L(8)=γ wire, L(9)=ω wire: result already in rax:rdx from rt_ab_leave_env.
                 * Falls back to classic open_slim path when ab_n==0 (SCRIP_AB=0). */
                bool ab3b = (g_emit_cfg && g_emit_cfg->ab_n > 0 && _.op_sval);
                if (ab3b) {
                    std::string fn_cell_lbl = std::string("fn_cell$") + _.op_sval;
                    void * fn_cell_bin = bb_ab_fn_cell_ptr(_.op_sval);
                    return x86("sub", "rsp", scc_sb)
                        + FOR(0, scc_nsave, [&](int k) {
                              return x86("note", gva_name(scc_gk[k])) + x86("mov", "rax", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk[k], 0) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk[k] * 16)) + x86_rsp_store64(16 * k, "rax")
                                   + x86("note", gva_name(scc_gk[k])) + x86("mov", "rax", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk[k], 8) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk[k] * 16 + 8)) + x86_rsp_store64(16 * k + 8, "rax"); })
                        + FOR(0, (int)_.op_ival, [&](int i) {
                              int slot = bcps_arg_slot(_.node, argblks, i);
                              int gk_i = scc_gk[i < scc_np ? i + 1 : i];
                              return (x86_fc_hit(slot) ? x86_rsp_load64("rax", slot - _.op_fc_base + (int)scc_sb) : x86("mov", "rax", FRQB(slot, (int)scc_sb)))
                                   + x86("note", gva_name(gk_i)) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(gk_i, 0) : ABSQ(RT_GVA_VA + (unsigned long)gk_i * 16), "rax")
                                   + (x86_fc_hit(slot + 8) ? x86_rsp_load64("rax", slot + 8 - _.op_fc_base + (int)scc_sb) : x86("mov", "rax", FRQB(slot + 8, (int)scc_sb)))
                                   + x86("note", gva_name(gk_i)) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(gk_i, 8) : ABSQ(RT_GVA_VA + (unsigned long)gk_i * 16 + 8), "rax"); })
                        + x86("lea", "rcx", L(8))
                        + x86("lea", "rdx", L(9))
                        + (MEDIUM_BINARY && fn_cell_bin
                            ? x86("movabs", "rax", (uint64_t)(uintptr_t)fn_cell_bin) + x86("mov", "rax", RDQ("rax", 0)) + x86("jmp", "rax")
                            : x86("mov",  "rax", std::string("[rip@got + __]"), (uint64_t)0, fn_cell_lbl.c_str()) + x86("mov", "rax", RDQ("rax", 0)) + x86("jmp", "rax"))
                        + x86("def", L(8)) + x86("add", "rsp", scc_sb) + x86("jmp", L(2))
                        + x86("def", L(9)) + x86("add", "rsp", scc_sb) + x86("jmp", L(2));
                }
                /* Classic SCC path (SCRIP_AB=0 or no DEFINE in this program) */
                return x86("sub", "rsp", scc_sb)
                    + FOR(0, scc_nsave, [&](int k) {
                          return x86("note", gva_name(scc_gk[k])) + x86("mov", "rax", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk[k], 0) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk[k] * 16)) + x86_rsp_store64(16 * k, "rax")
                               + x86("note", gva_name(scc_gk[k])) + x86("mov", "rax", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk[k], 8) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk[k] * 16 + 8)) + x86_rsp_store64(16 * k + 8, "rax"); })
                    + x86_ro_load_q("rdi", 0)
                    + x86("mov32", "esi", (long)scc_np)
                    + x86("mov32", "edx", (long)_.op_ival)
                    + x86("call", "rt_proc_call_open_slim", (uint64_t)scc_fp_o)
                    + x86("test", "rax", "rax")
                    + x86("je", L(5))
                    + x86("mov", "r10", "rax")   /* FUNCTION LINKAGE s55: fn rides the open return; parked across the install loop (rax clobbered), r10 dead until the site's own lea below */
                    + FOR(0, (int)_.op_ival, [&](int i) {
                          int slot = bcps_arg_slot(_.node, argblks, i);
                          return (c2farm() ? x86_rsp_load64("rax", (int)scc_sb) : x86_fc_hit(slot) ? x86_rsp_load64("rax", slot - _.op_fc_base + (int)scc_sb) : x86("mov", "rax", FRQB(slot, (int)scc_sb)))
                               + x86("note", gva_name(scc_gk[i])) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk[i], 0) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk[i] * 16), "rax")
                               + (c2farm() ? x86_rsp_load64("rax", (int)scc_sb + 8) : x86_fc_hit(slot + 8) ? x86_rsp_load64("rax", slot + 8 - _.op_fc_base + (int)scc_sb) : x86("mov", "rax", FRQB(slot + 8, (int)scc_sb)))
                               + x86("note", gva_name(scc_gk[i])) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk[i], 8) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk[i] * 16 + 8), "rax"); })
                    + x86("mov", "rax", "r10")   /* FUNCTION LINKAGE s55: rt_proc_open_fn crossing DELETED */
                    + bb_glue_pass_wires_blob(6, 7)
                    + x86("def", L(6))
                    + x86("note", gva_name((scc_res_gk < 0 ? 0 : scc_res_gk))) + x86("mov", "rdi", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ((scc_res_gk < 0 ? 0 : scc_res_gk), 0) : ABSQ(RT_GVA_VA + (unsigned long)(scc_res_gk < 0 ? 0 : scc_res_gk) * 16))
                    + x86("note", gva_name((scc_res_gk < 0 ? 0 : scc_res_gk))) + x86("mov", "rsi", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ((scc_res_gk < 0 ? 0 : scc_res_gk), 8) : ABSQ(RT_GVA_VA + (unsigned long)(scc_res_gk < 0 ? 0 : scc_res_gk) * 16 + 8))
                    + FOR(0, scc_nsave, [&](int j) { int k = scc_nsave - 1 - j;
                          return x86_rsp_load64("rax", 16 * k) + x86("note", gva_name(scc_gk[k])) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk[k], 0) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk[k] * 16), "rax")
                               + x86_rsp_load64("rax", 16 * k + 8) + x86("note", gva_name(scc_gk[k])) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk[k], 8) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk[k] * 16 + 8), "rax"); })
                    + x86("add", "rsp", scc_sb)
                    + x86("call", "rt_proc_call_epilogue_slim_γ", (uint64_t)scc_fp_g)
                    + x86("jmp", L(2))
                    + x86("def", L(7))
                    + FOR(0, scc_nsave, [&](int j) { int k = scc_nsave - 1 - j;
                          return x86_rsp_load64("rax", 16 * k) + x86("note", gva_name(scc_gk[k])) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk[k], 0) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk[k] * 16), "rax")
                               + x86_rsp_load64("rax", 16 * k + 8) + x86("note", gva_name(scc_gk[k])) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk[k], 8) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk[k] * 16 + 8), "rax"); })
                    + x86("add", "rsp", scc_sb)
                    + x86("call", "rt_proc_call_epilogue_slim_ω", (uint64_t)scc_fp_w)
                    + x86("jmp", L(2))
                    + x86("def", L(5))
                    + x86("add", "rsp", scc_sb)
                    + IF(c2farm(), x86("call", "rt_c2b_arm_trap", trap_fp));

              }()
            : std::string(""))
         + (dc
            ? FOR(0, det_nA, [&](int i) { int slot = bcps_arg_slot(_.node, argblks, i); return x86("lea", detN_argreg[i], FRQ(slot)); })   /* PL-ZK-5B DC-ARG-FIX (Bug 5): on pl_cells_graph, dual-write placed arg values at [___+slot] (FRQ(slot) under pinned ___). FRQ(slot) routes through FB-STMT refinement (x86_fb_data) which can select rsp-relative addressing when op_fb____=0 — causing all args to LEA [rsp+0] identically. ___RAWQ(slot) bypasses the refinement and directly names [___+slot], which is always correct for Prolog ZLS frame slots under the zframe prologue's ___ pin. SN4/Icon: pl_cells_graph=0 → FRQ path unchanged — byte-identical. ONE AUTHORITY. */
            + x86_call_dc(dc_name, dc_slot)
            + x86("jmp", L(2))
            : std::string(""))
         + (det_fuse || dc ? std::string("") : FOR(0, (int)_.op_ival, [&](int i) {
        int slot = bcps_arg_slot(_.node, argblks, i);
        return stage_arg_inline(i, slot, stage_fp);
    }))
         + (dc ? std::string("")
            : det_fuse
            ? x86("mov32", "edi", det_idx)
            + FOR(0, det_nA, [&](int i) { int slot = bcps_arg_slot(_.node, argblks, i); return x86("lea", detN_argreg[i], FRQ(slot)); })
            + x86("call", detN_nm[det_nA], detN_fp[det_nA])
            : det_idx >= 0 && x86_zc_frame() == ZC_FRAME_RSP
            /* PL-REGAIN-1 slice A (2026-07-19 s100): emit-time-resolved det callee — the index into the dense registry replaces the name (no hash, no strcmp) and the fused leaf returns the fn pointer,
             * eliding the rt_proc_open_fn crossing; rax carries the fn straight to the jmp below.  Eligibility is emit-time-static (!is_dyn, literal target, registered = index >= 0); a runtime guard
             * mismatch returns 0 into the SAME je L(1) FAIL arm as a bodyless proc, side-effect-free.  Inline arg install (kills rt_arg_stage) and the direct cross-box jmp are slices B/C — B blocked
             * on g_call_args residency (.so data, movabs-forbidden; needs slab home or register-arg ABI, Lon design call), C needs the driver-minted proc-entry label record. */
            ? x86("mov32", "edi", (long)det_idx)
            + x86("mov32", "esi", (long)_.op_ival)
            + x86("call", "rt_proc_call_open_det", (uint64_t)det_fp)
            : x86_ro_load_q("rdi", 0)
            + x86("mov32", "esi", (long)_.op_ival)
            + x86("call", "rt_proc_call_open", open_fp))
         + IF(!dc, x86("test", "rax", "rax")
         + x86("je", L(1)))
         + (dc ? std::string("")
            : (x86_zc_frame() == ZC_FRAME_RSP)
            /* ZW-0 stage 2: ISLE conjunct deleted -- ZC_FRAME_RSP is the only live basis.
             * R12-ERAD s65: the r12 anchor is DEAD — the callee's LIFO exits fully unwind frame+header before jmping the wire, so rsp at either landing = rsp at the jmp below; result arrives in rdi:rsi.
             * REG-7 s80 GUARD WIDENED (was && !_.flat_pat): proc callees are ALWAYS the determinate full-unwind class under ZC_FRAME_RSP — the suspending zr-exit class is PAT$ fragments, which a proc call
             * can never land in — so a flat_pat CALLER takes this anchor-free wire too, retiring the REG-6 hazard (r12 = pend top rides untouched through the call). */
            ? (det_idx >= 0 ? std::string("") : x86("call", "rt_proc_open_fn", openfn_fp))
            + bb_glue_pass_wires(3, 4)   /* GLUE-SYM (s22x) */
            + x86("def", L(3))
            + x86("call", "rt_proc_call_epilogue_γ", epig_fp)
            + x86("jmp", L(2))
            + x86("def", L(4))
            + x86("call", "rt_proc_call_epilogue_ω", epiw_fp)
            + x86("jmp", L(2))
            : is_dyn
            /* LEGACY-CONFIG ONLY (REG-7 s80 audit resolved; was the ⛔ REG-6 hazard flag): reachable solely
             * when x86_zc_frame() != ZC_FRAME_RSP after the guard widening above — configs where r12 IS the ζ frame
             * (⭐ s202: that basis NO LONGER EXISTS.  ZC_FRAME_R12 was deleted at ZR-RSP___-1 s201, so `!= RSP`
             * now means ___ — a basis this arm was never written for, and which is #error-guarded as non-running
             * in zeta_choices.h after a matched-pair A/B measured 9 NET NEW crashes (s202).  This arm is therefore DEAD CODE awaiting the
             * delete-or-re-establish call; see FINDING-2026-07-28b.)
             * (pre-REG-MAP tenancy), the pend top is NOT register-resident, and this rsp-anchor bracket is the
             * correct suspending-exit protocol.  Under the RSP default this arm is unreachable, so the REG-6
             * unsoundness (a callee capture bumping the anchor) cannot fire.  Delete with the legacy configs. */
            ? x86("call", "rt_proc_open_fn", openfn_fp)
            + x86("push", "r12")
            + x86("sub", "rsp", 8L)
            + x86("mov", "r12", "rsp")   /* GLUE-SYM (s22x): hoisted above the glue -- lea rcx/rdx touch neither rsp nor r12, so the anchor records the identical value; legacy non-RSP arm only */
            + bb_glue_pass_wires(3, 4)
            + x86("def", L(3))
            + x86("mov", "rax", "rsp")
            + x86("mov", "rax", RDQ("rax", 8))
            + x86("mov", "rdi", RDQ("rax", 0))
            + x86("mov", "rsi", RDQ("rax", 8))
            + x86("mov", "rsp", "r12")
            + x86("add", "rsp", 8L)
            + x86("pop", "r12")
            + x86("call", "rt_proc_call_epilogue_γ", epig_fp)
            + x86("jmp", L(2))
            + x86("def", L(4))
            + x86("mov", "rsp", "r12")
            + x86("add", "rsp", 8L)
            + x86("pop", "r12")
            + x86("call", "rt_proc_call_epilogue_ω", epiw_fp)
            + x86("jmp", L(2))
            : x86_frame_sink()
            + x86_frame_base("rdi")
            + x86("mov", "rsi", "rax")
            + x86("call", "rt_frame_prep", prep_fp)
            + x86_frame_base("rdi")
            + x86("xor", "esi", "esi")
            + x86("call", "rax")
            + x86("mov", "rdi", "rax")
            + x86("mov", "rsi", "rdx")
            + x86_frame_unsink()
            + x86("call", "rt_proc_call_epilogue_ret", epir_fp)
            + x86("jmp", L(2)))
         + IF(!dc, x86("def", L(1))
         + x86("call", "rt_faildescr", fail_fp))
         + x86("def", L(2))
         + x86_anchor_leave()
         + x86_scan_sync_in_rr()
         + x86("mov", FRQ(off), "rax")
         + x86("mov", FRQ(off + 8), "rdx")
         + x86("cmp", "eax", (long)DT_FAIL)
         + x86_omega("je")
         + x86_gamma()
         + x86_beta()
         + (bidx < 0 ? x86_omega() : x86_pair_jmp(bidx))
         + x86_ro_seal_str(0, _.op_sval ? _.op_sval : "");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* GENP-SPINE (Lon directive 2026-07-17 s92: "get generator procedures on the main spine; co-expressions are the only construct requiring a separate stack/pthread") — THE GENERATOR CALL SITE, SPINE-RESIDENT.
 * Supersedes the s91 per-instance-stack arms below (bcps_bin/txt_gen_arm, now LEGACY-CONFIG ONLY: non-RSP frames): the callee is the SAME jmp-entry blob the det arm enters — flat_lex lexprep binds the
 * staged args, the body runs on the ONE RSP/___ ζ stack, and suspend routes through xa_flat's RETAINING γ epilogue (result preloaded rdi:rsi, 16B resume record {res-landing, callee ___} left at the
 * deep frontier, γ wire jumped, NO unwind).  The caller continues DEEP below the retained activation — legal since NCB-1d made consumers depth-immune — and the β resume edge is the ZS-2 outside law
 * verbatim: jmp qword [rsp] (the record's landing word sits AT the frontier by LIFO balance; res-landing drops it, re-pins ___, dispatches jmp [___+resume_slot] to the recorded suspend's β).
 * Exhaustion and post-`return` resumption (the bb_return slot-poison) unwind ABSOLUTELY lea rsp,[___+kt] and land the ω wire.  ABANDONMENT IS FREE: any enclosing epilogue's ___-absolute unwind reclaims
 * every retained frame below it — no thread, no join, the s91 rc=124 exit-hang / abandoned-instance-leak class dies structurally.  ONE-POP LAW: rt_proc_call_open pushed ONE pcall record and
 * epilogue_γ/ω POP one, so only the FIRST delivery may run an epilogue leaf; the callgen.act ZLS2 slot (the pthread model's activation handle, repurposed) is the once-flag — α zeroes it, the landings
 * test-and-set, resumed deliveries pass rdi:rsi through to rax:rdx (γ) or synthesize FAILDESCR (ω) with no pop.  Wires are per-activation constants, so EVERY yield lands L(3) and EVERY failure L(4):
 * first-vs-resumed is the flag's job alone.  β never re-stages/re-opens — args were bound into the retained frame at prologue. */
static std::string bcps_spine_gen_arm() {
    x86_begin();
    int off = bcps_result_slot(); if (off < 0) return x86_bomb("bb_call_proc_staged: no LOWER slot grant (TMP-ERADICATE)");
    int act = off + 16 * (1 + (int)_.op_ival);
    IR_graph_t ** argblks = (IR_graph_t **)(intptr_t)_.op_counter;
    uint64_t stage_fp; { void (*fp)(int, DESCR_t) = rt_arg_stage; stage_fp = (uint64_t)(uintptr_t)(void*)fp; }
    uint64_t open_fp;  { long (*fp)(const char *, int) = rt_proc_call_open; open_fp = (uint64_t)(uintptr_t)(void*)fp; }
    uint64_t openfn_fp; { void * (*fp)(void) = rt_proc_open_fn; openfn_fp = (uint64_t)(uintptr_t)(void*)fp; }
    uint64_t epig_fp;  { DESCR_t (*fp)(DESCR_t) = rt_proc_call_epilogue_γ; epig_fp = (uint64_t)(uintptr_t)(void*)fp; }
    uint64_t epiw_fp;  { DESCR_t (*fp)(void) = rt_proc_call_epilogue_ω; epiw_fp = (uint64_t)(uintptr_t)(void*)fp; }
    uint64_t fail_fp;  { DESCR_t (*fp)(void) = rt_faildescr; fail_fp = (uint64_t)(uintptr_t)(void*)fp; }
    uint64_t pasg_fp;  { DESCR_t (*fp)(DESCR_t) = rt_gen_spine_pass_γ; pasg_fp = (uint64_t)(uintptr_t)(void*)fp; }
    uint64_t pasw_fp;  { DESCR_t (*fp)(void) = rt_gen_spine_pass_ω; pasw_fp = (uint64_t)(uintptr_t)(void*)fp; }
    uint64_t rsen_fp;  { void (*fp)(void) = rt_gen_spine_resume_enter; rsen_fp = (uint64_t)(uintptr_t)(void*)fp; }
    uint64_t getfb_fp; { void *(*fp)(void) = rt_gen_get_fb; getfb_fp = (uint64_t)(uintptr_t)(void*)fp; }
    /* PL-FR-4 ZFRAME β RESUME — emitted for Prolog zframe generators (zframe_graph=1, !icn_zframe_gen, resume_slot>0).
     * The non-zframe β path (mov rsp,FRQ(act+8); jmp[rsp]) jumps into a DEAD C frame after the callee's γ-epilogue
     * unwound it — causing hangs, wrong output, or SEGV (FR-4 root cause, fully diagnosed in GOAL-PL-ZFRAME-RESTORE).
     * The fix: pop {tm_lo, tm_hi, cursor_cont} triple from g_pl_cp_stack (pushed by bb_suspend at each yield),
     * call rt_pl_zf_resume_set to register the cursor+trail for the new activation, re-stage args, re-call
     * rt_proc_call_open_det to open a fresh pcall, and jmp into the callee α AGAIN.  The callee's α runs:
     * rt_jmp_frame_lexprep2 (which applies the pending cursor+trail override into the fresh frame),
     * then α_body (which re-writes the cursor — same value as our override), then n0 ($trail_mark = CURRENT
     * trail top), then the body runs clause 1 again.  At n15_suspend_α (clause 1 γ-exit), before the
     * xa_flat_zframe_epilogue_γ unwinds the frame, the epilogue checks g_pl_zf_pending_cursor:
     *   - if set (our resume_set was still pending), write cursor to [___+resume_slot] and jmp there directly
     *     → SKIPS clause 1's result, jumps straight to n15_suspend_β (clause 2 entry)
     *   - if cleared (normal path), γ-exit proceeds to L(3) with clause 1's result
     * Wait: rt_jmp_frame_lexprep2 calls rt_pl_zf_resume_clear() — so by n15_suspend_α time, the pending flag
     * is already cleared.  The epilogue check fires BEFORE lexprep2 in the α prologue... no, lexprep2 is called
     * IN α before the body.  The epilogue fires AFTER the full clause-1 execution.  The intercept must detect
     * that this was a β-resume re-entry by a DIFFERENT mechanism.
     * REVISED: use a call-count sentinel: save g_pl_cp_top value at β-resume time into FRQ(act+32). At L(7)
     * landing, check if g_pl_cp_top > FRQ(act+32) (a new triple was pushed by n15_suspend_α during this re-entry).
     * If so: the new top is clause 2's triple; pop it and re-resume (jmp cursor = n15_suspend_β, WITHIN the
     * still-live callee frame... but the frame is DEAD at L(7)).
     * FINAL CORRECT APPROACH: set g_pl_zf_pending_cursor BEFORE re-calling open_det. rt_jmp_frame_lexprep2
     * writes cursor+trail. α_body overwrites cursor with n15_suspend_β (SAME VALUE). n0 sets fresh trail top.
     * At n15_suspend_α (yields clause 1), bb_suspend emits rt_pl_cp_push3 BEFORE the yield.  The push3 call
     * checks g_pl_zf_pending_cursor: if still set, SKIP the push3 (don't push clause 1 again; caller already
     * has what it needs). Instead, jmp cursor = n15_suspend_β directly WITHIN the live frame.
     * THIS IS THE FIX: bb_suspend gates the push3 on !g_pl_zf_pending_cursor, and if pending: jmp cursor. */
    /* ICN-FR-4 ZFRAME GENERATOR RESUME — emit-time callee resume-slot lookup.
     * Under the zframe model (g_emit.zframe_graph=1), the generator's γ epilogue absolutely unwinds the
     * deep stack (lea rsp,[___+kt]).  The non-zframe resume record ({res-landing, callee____} at the FORTH
     * frontier) is not retained.  Instead we jump to the generator's stored β continuation via:
     *   call rt_gen_get_fb       → rax = generator____ (stored in pcall.fb by rt_jmp_frame_lexprep2)
     *   jmp  [rax + zf_cont_off] → reaches n1_suspend_β / n3_suspend_β / ... (the next body segment)
     * zf_cont_off = zls_g_resume_by_name(callee) is the byte offset in the generator's frame where bb_suspend
     * stores the next continuation address via `lea rax,[rip+n_suspend_β]; mov [___+cont_off], rax`.
     * zls_g_resume_by_name is emit-time-only (scans zg[] by name) and bakes the offset as an immediate.
     * zframe_graph=0 for all SN4/Prolog/Raku/Pascal graphs by law R-ICN-D — non-zframe generators keep the
     * original push/jmp[rsp] protocol unchanged (byte-identical). */
    int  zf_cont_off = (g_emit.zframe_graph && _.op_sval) ? ([]() { extern int zls_g_resume_by_name(const char *); return zls_g_resume_by_name(_.op_sval); })() : -1;
    bool zf_resume   = g_emit.zframe_graph && (zf_cont_off >= 0) && zls_g_icn_zframe_gen_by_name(_.op_sval);   /* ICN-FR-5 BUG1 FIX: was g_emit_cfg->icn_zframe_gen (CALLER graph's flag), which is 0 for main() and every non-generator caller — so zf_resume was always false and the non-zframe push/jmp[rsp] path was taken even for Icon zframe generator calls, looping back to L(7) instead of advancing the generator.  The correct discriminator is the CALLEE's icn_zframe_gen, looked up by name: lower_icon.c stamps icn_zframe_gen=1 on the generator proc's own graph (ONE AUTHORITY, line 1424); Prolog graphs also reach bcps_spine_gen_arm (lower_prolog emits IR_SUSPEND, giving them a resume_off >= 0 via zls_g_resume_by_name) but lower_prolog NEVER sets icn_zframe_gen, so zls_g_icn_zframe_gen_by_name returns 0 for them → correct non-zframe Prolog path preserved.  PL-ZD-WINDOW2-FIX intent (gate Prolog flat_gen=1 out of the icn-zframe rt_gen_get_cont path) is still honored by the callee-lookup. */
    /* PL-FR-4 FIX (s12): was g_emit_cfg->resume_slot (CALLER graph — always 0 for non-suspending main/0, making pl_zf_resume=false and falling through to the dead-frame jmp[rsp] path).
     * Correct discriminator: the CALLEE's resume_slot, already resolved as zf_cont_off = zls_g_resume_by_name(_.op_sval) above (>=0 means callee is a multi-clause Prolog generator).
     * Parallel with zf_resume's ICN-FR-5 BUG1 fix which also corrected caller→callee lookup. */
    bool pl_zf_resume = g_emit.zframe_graph && !zf_resume && (zf_cont_off >= 0) && !zls_g_icn_zframe_gen_by_name(_.op_sval);   /* Callee is a Prolog zframe multi-clause generator (resume slot known, not an Icon gen) */
    int  pl_tm_off = pl_zf_resume ? zls_g_pl_zf_trail_mark_off_by_name(_.op_sval) : 0;   /* PL-FR-4: callee's trail-mark frame slot offset via callee name lookup (file-scope forward decl above; was caller g_emit_cfg->pl_zf_trail_mark_off — wrong for same reason as zf_resume's ICN-FR-5 BUG1 fix). */
    /* PL-GENIDX-1 (2026-07-25) — EMIT-TIME-RESOLVED CALLEE FOR THE *GENERATOR* SITE.  PL-REGAIN-1 slice A (s100) gave the DET arm an index-based open (no FNV hash, no strcmp, and the fused leaf returns
     * the fn pointer so the separate rt_proc_open_fn crossing dies).  THE GENERATOR ARM NEVER GOT IT — and every nondet Prolog predicate (`app/3`, `nrev/2`, every multi-clause pred) is dispatched HERE,
     * so the whole Prolog hot corpus was re-hashing its callee's NAME STRING on every one of ~10M calls.  Measured: 6/6 nrev call sites emitted the name path, 0 the index path; rt_proc_fnv +
     * rt_proc_hash_lookup + rt_proc_find held ~15-20% of leaf samples in the mode-4 profile.
     * Eligibility is IDENTICAL to the det arm's and emit-time-static (!is_dyn, literal target, registered => index >= 0); the rt table is fully populated before any emission in BOTH media and registration
     * order is identical in-process and in the mode-4 startup bake, so the index is stable in both.  SEMANTIC EQUIVALENCE, checked not assumed: (a) ONE-POP LAW — rt_proc_call_open and
     * rt_proc_call_open_det BOTH reach rt_proc_call_prologue_lex exactly once, which is the sole pusher of the pcall record (g_pcall_top++), so the epilogue_γ/ω single pop still balances; (b) the SUCCESS
     * TEST is preserved — the name leaf returns fbytes (>= PROC_FRAME_QWORDS*8, never 0) and the index leaf returns p->fn (non-null by its own guard), so `test rax,rax; je L(1)` means "opened" on both;
     * (c) a runtime dyn_scope mismatch makes the index leaf return 0 into that SAME side-effect-free FAIL arm, exactly as the det arm already accepts.  Hatch: SCRIP_NO_GENIDX=1 restores the name path
     * byte-for-byte for A/B. */
    int   gi_off; { static int c = -1; if (c < 0) { const char *e = getenv("SCRIP_NO_GENIDX"); c = (e && *e == '1') ? 1 : 0; } gi_off = c; }
    int   gi_dyn = _.op_sval && rt_proc_dyn_scope(_.op_sval);
    long  gi_idx = (!gi_off && !gi_dyn && _.op_sval) ? (long)rt_proc_index_of(_.op_sval) : -1L;
    uint64_t gidet_fp; { void * (*fp)(long, int) = rt_proc_call_open_det; gidet_fp = (uint64_t)(uintptr_t)(void*)fp; }
    return x86_alpha()
         + x86_scan_sync_out()
         + x86_anchor_enter()
         + x86("mov", FRQ(act), 0L)
         + FOR(0, (int)_.op_ival, [&](int i) {
        int slot = bcps_arg_slot(_.node, argblks, i);
        return stage_arg_inline(i, slot, stage_fp);
    })
         /* ICN-FR-4: zframe path needs NO stack guard before open. old____ is now stored at [___+kt-32]
          * (inside the generator's allocated frame, not at [entry_rsp-8] where C callers push return addresses).
          * Header relocation in xa_flat_zframe_prologue/epilogue is the structural fix. No padding needed here.
          * Non-zframe: push rax (L(7) landing word) provides the FORTH resume record at [rsp]; unchanged. */
         + (zf_resume ? std::string("") : x86_lea_id("rax", 7) + x86("push", "rax"))
         + (gi_idx >= 0
            ? x86("mov32", "edi", (long)gi_idx)
            + x86("mov32", "esi", (long)_.op_ival)
            + x86("call", "rt_proc_call_open_det", (uint64_t)gidet_fp)
            : x86_ro_load_q("rdi", 0)
            + x86("mov32", "esi", (long)_.op_ival)
            + x86("call", "rt_proc_call_open", open_fp))
         + x86("test", "rax", "rax")
         + x86("je", L(1))
         + (gi_idx >= 0 ? std::string("") : x86("call", "rt_proc_open_fn", openfn_fp))
         + bb_glue_pass_wires(3, 4)   /* GLUE-SYM (s22x) */
         + x86("def", L(3))
         /* ICN-FR-4 zframe: rax = generator____ (set by xa_flat_zframe_epilogue_γ: mov rax,___ before ___ restore).
          * Save to FRQ(act+8) WITHOUT a call — any call at rsp=generator_entry_rsp would push the return address
          * to [generator_entry_rsp-8] = generator's old____ header slot, permanently corrupting it.
          * Non-zframe: rax is the epilogue's return value (unrelated); save rsp for FORTH [rsp] resume record. */
         + (zf_resume
            ? x86("mov", FRQ(act + 8), "rax")   /* save generator____ (from epilogue) in caller's frame */
            : x86("mov", FRQ(act + 8), "rsp")   /* non-zframe: save rsp with landing word at [rsp] */
              + x86("add", "rsp", 8L))           /* pop landing word after save (non-zframe) */
         + x86("mov", "rax", FRQ(act))
         + x86("test", "rax", "rax")
         + x86("jne", L(5))
         + x86("mov", FRQ(act), 1L)
         + x86("call", "rt_proc_call_epilogue_γ", epig_fp)
         + x86("jmp", L(2))
         + x86("def", L(5))
         + x86("call", "rt_gen_spine_pass_γ", pasg_fp)
         + x86("jmp", L(2))
         + x86("def", L(4))
         + x86("mov", "rax", FRQ(act))
         + x86("test", "rax", "rax")
         + x86("jne", L(6))
         + x86("mov", FRQ(act), 1L)
         + x86("call", "rt_proc_call_epilogue_ω", epiw_fp)
         + x86("jmp", L(2))
         + x86("def", L(6))
         + x86("call", "rt_gen_spine_pass_ω", pasw_fp)
         + x86("jmp", L(2))
         + x86("def", L(1))
         + x86("call", "rt_faildescr", fail_fp)
         + x86("def", L(2))
         + x86_anchor_leave()
         + x86_scan_sync_in_rr()
         + x86("mov", FRQ(off), "rax")
         + x86("mov", FRQ(off + 8), "rdx")
         + x86("cmp", "eax", (long)DT_FAIL)
         + x86_omega("je")
         + x86_gamma()
         + x86_beta()
         + x86_scan_sync_out()
         + x86("call", "rt_gen_spine_resume_enter", rsen_fp)
         /* ICN-FR-4 β RESUME — TWO PATHS:
          * ZFRAME (zf_resume=true): FRQ(act+8) = generator____ (saved at L(3) from epilogue's rax=r11).
          *   1. call rt_gen_get_cont → rax = continuation ptr (saved in pcall.save_Σ by bb_suspend).
          *      The in-frame slot [generator____+zf_cont_off] is CLOBBERED by the caller's C-calls between
          *      yields (the caller's call stack expands downward into the generator's frame). pcall.save_Σ
          *      is heap-allocated and immune. rsen_fp was already called above; rax is clobbered here.
          *   2. Save cont to r11 (ABI scratch); load generator____ from FRQ(act+8) into rax.
          *   3. mov ___=rsp=rax (generator____); jmp r11 (continuation).
          * NON-ZFRAME: mov rsp,FRQ(act+8) restores frontier where [rsp]=landing word; jmp[rsp]→L(7). */
         + (zf_resume
            ? ( [&]() -> std::string {
                uint64_t _gc_fp; { void *(*_f)(void *) = rt_gen_get_cont; _gc_fp = (uint64_t)(uintptr_t)(void *)_f; }
                return x86("mov", "rax", FRQ(act + 8))          /* rax = generator____ (FRQ slot saved at L(3)) */
                     + x86("mov", "rdi", "rax")                 /* ICN-FR-5: gen____ as first arg for keyed cont lookup */
                     + x86("call", "rt_gen_get_cont", _gc_fp)   /* rax = continuation ptr keyed by gen____ */
                     + x86("mov", "r8", "rax")                 /* save cont in r11 (ABI scratch) */
                     + x86("mov", "rax", FRQ(act + 8))          /* rax = generator____ again */
                     + std::string("")                 /* pin generator frame base */
                     + x86("mov", "rsp", "rax")                 /* set FORTH base to generator____ */
                     + x86("jmp", "r8");                       /* jmp to stored continuation */
              })()
            : pl_zf_resume
            ? ( [&]() -> std::string {
                /* PL-FR-4 ZFRAME β RESUME:
                 * 1. Pop triple {tm_lo, tm_hi, cursor_cont} from g_pl_cp_stack.
                 * 2. If exhausted (cursor_cont=0), jmp omega.
                 * 3. Call rt_pl_zf_resume_set(cursor_cont, tm_lo, tm_hi, pl_tm_off) to register pending resume.
                 * 4. Re-stage args (still live in caller's FRQ arg slots from the original call).
                 * 5. Re-call rt_proc_call_open_det → rax=fn; push L(7); jmp fn.
                 * Inside the callee α: rt_jmp_frame_lexprep2 finds g_pl_zf_pending_cursor set and writes
                 * cursor+trail into the fresh frame.  α_body re-writes cursor (same value).  n0 ($trail_mark)
                 * sets a fresh trail checkpoint.  n15_suspend_α: bb_suspend checks g_pl_zf_pending_cursor —
                 * if still set (it was cleared by rt_pl_zf_resume_clear in rt_jmp_frame_lexprep2... wait,
                 * rt_jmp_frame_lexprep2 calls rt_pl_zf_resume_clear.  So by n15_suspend_α time it's cleared.
                 * The epilogue-γ check fires BEFORE lexprep2 clears it — no, lexprep2 is IN the PROLOGUE
                 * which runs BEFORE the body and before n15_suspend_α.
                 * CORRECT FLOW: rt_jmp_frame_lexprep2 writes cursor+trail and CLEARS pending.  n0 overwrites
                 * trail.  n15_suspend_α fires — pending is clear — bb_suspend's gate says NOT pending, so
                 * push3 fires normally (pushes clause-1's triple).  Callee γ-exits to L(3).  L(3) saves rsp.
                 * At L(7): we have clause 1's result.  BUT FRQ(act+8) holds the rsp pointing at L(7) in the
                 * caller's frame — exactly the non-zframe state.  So the NEXT β fires the legacy path...
                 * which is still broken.
                 * THE PENDING CURSOR MUST NOT BE CLEARED BY LEXPREP2 — it must survive until γ-exit so the
                 * epilogue intercept can fire.  Move the clear OUT of rt_jmp_frame_lexprep2 and into the
                 * epilogue intercept (which already calls rt_pl_zf_resume_clear).  The α_body's cursor
                 * re-write (lea rax,[rip+n15_suspend_β]; mov [___+1120],rax) happens AFTER lexprep2 and
                 * sets [___+1120] = n15_suspend_β — same as our pending cursor, no conflict.
                 * n15_suspend_α at γ-time: g_pl_zf_pending_cursor is still set (not cleared by lexprep2).
                 * bb_suspend checks it — pending=set → jmp r11 (cursor = n15_suspend_β, in live frame).
                 * n15_suspend_β → n16_call_builtin_prolog_α → trail unwind → clause 2 body → ...
                 * THIS IS CORRECT if we DON'T clear in lexprep2. */
                uint64_t _pop3_fp; { void *(*_f)(long *, long *) = rt_pl_cp_pop3; _pop3_fp = (uint64_t)(uintptr_t)(void *)_f; }
                uint64_t _set_fp; { void (*_f)(void *, long, long, int, int) = rt_pl_zf_resume_set; _set_fp = (uint64_t)(uintptr_t)(void *)_f; }
                /* pop3: returns cont in rax; we pass stack addrs for tm_lo/tm_hi using scratch slots */
                return x86("comment", "PL-FR-4 zframe β: pop triple, set pending resume, re-enter callee α")
                     /* reset FRQ(act) to 0 so L(3) calls rt_proc_call_epilogue_γ (not rt_gen_spine_pass_γ) — the fresh open_det pushes a new pcall that must be popped at γ */
                     + x86("mov", FRQ(act), 0L)
                     /* save two scratch slots for tm_lo/tm_hi output from pop3 */
                     + x86("lea", "rdi", FRQ(act + 16))    /* &tm_lo → rdi (scratch FRQ slot) */
                     + x86("lea", "rsi", FRQ(act + 24))    /* &tm_hi → rsi */
                     + x86("call", "rt_pl_cp_pop3", _pop3_fp)   /* rax = cursor_cont (0 = exhausted) */
                     + x86("test", "rax", "rax")
                     + x86_omega("je")                      /* exhausted = fail */
                     /* set pending resume: rdi=cursor, rsi=tm_lo, rdx=tm_hi, ecx=tm_off, r8d=cursor_off */
                     + x86("mov", "rdi", "rax")             /* cursor_cont */
                     + x86("mov", "rsi", FRQ(act + 16))    /* tm_lo */
                     + x86("mov", "rdx", FRQ(act + 24))    /* tm_hi */
                     + x86("mov32", "ecx", (long)pl_tm_off) /* tm_off (baked immediate) */
                     + x86("mov32", "r8d", (long)zf_cont_off) /* cursor_off = resume_slot */
                     + x86("call", "rt_pl_zf_resume_set", _set_fp)
                     /* re-stage args for the fresh α call — use plain rt_arg_stage (not inline) to avoid L(20+) collision with α's stage calls */
                     + FOR(0, (int)_.op_ival, [&](int i) {
                         int slot = bcps_arg_slot(_.node, argblks, i);
                         return x86("mov32", "edi", (long)i)
                              + x86("mov", "rsi", FRQ(slot))
                              + x86("mov", "rdx", FRQ(slot + 8))
                              + x86("call", "rt_arg_stage", stage_fp);
                     })
                     /* re-open callee (fresh pcall + fn ptr); set wires and enter — must push L(7) landing word BEFORE jmp so L(3) landing finds it on stack */
                     + (gi_idx >= 0
                        ? x86("mov32", "edi", (long)gi_idx)
                        + x86("mov32", "esi", (long)_.op_ival)
                        + x86("call", "rt_proc_call_open_det", (uint64_t)gidet_fp)
                        : x86_ro_load_q("rdi", 0)
                        + x86("mov32", "esi", (long)_.op_ival)
                        + x86("call", "rt_proc_call_open", open_fp))
                     + x86("test", "rax", "rax")
                     + x86_omega("je")
                     + (gi_idx >= 0 ? std::string("") : x86("call", "rt_proc_open_fn", openfn_fp))
                     /* push L(7) landing word FIRST (matches what α-path does before open_det; L(3) landing does add rsp,8 to pop it) */
                     + x86_lea_id("r11", 7)
                     + x86("push", "r11")
                     /* set γ/ω wires and enter callee — equivalent to bb_glue_pass_wires(3,4) but with L(7) already on stack */
                     + x86_lea_id("rcx", 3)
                     + x86_lea_id("rdx", 4)
                     + x86("jmp", "rax")
                     /* L(7): bb_suspend's pending-cursor check intercepts BEFORE γ-exit and jmps to clause 2.
                      * We should not reach L(7) in the pl_zf_resume path — if we do, clause 2 was also exhausted
                      * (the callee γ-exited normally) and we have a fresh result to pass through. */
                     + x86("def", L(7))
                     + x86("add", "rsp", 8L)
                     + x86_anchor_leave()
                     + x86_scan_sync_in_rr()
                     + x86("mov", FRQ(off), "rax")
                     + x86("mov", FRQ(off + 8), "rdx")
                     + x86("cmp", "eax", (long)DT_FAIL)
                     + x86_omega("je")
                     + x86_gamma();
              })()
            : x86("mov", "rsp", FRQ(act + 8))
            + x86_jmp_mem("rsp", 0)
            + x86("def", L(7))
            + x86("add", "rsp", 8L)
            + x86_anchor_leave()
            + x86_scan_sync_in_rr()
            + x86("mov", FRQ(off), "rax")
            + x86("mov", FRQ(off + 8), "rdx")
            + x86("cmp", "eax", (long)DT_FAIL)
            + x86_omega("je")
            + x86_gamma())
         + x86_ro_seal_str(0, _.op_sval ? _.op_sval : "");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_bin_gen_arm() {
    int off = bcps_result_slot(); if (off < 0) return x86_bomb("bb_call_proc_staged: no LOWER slot grant (TMP-ERADICATE)");
    int act = off + 16 * (1 + (int)_.op_ival);
    IR_graph_t ** argblks = (IR_graph_t **)(intptr_t)_.op_counter;
    uint64_t stage_fp; { void (*fp)(int, DESCR_t) = rt_arg_stage; stage_fp = (uint64_t)(uintptr_t)(void*)fp; }
    uint64_t callg_fp; { DESCR_t (*fp)(const char *, int, void **) = rt_proc_call_gen_h; callg_fp = (uint64_t)(uintptr_t)(void*)fp; }
    uint64_t resumeg_fp; { DESCR_t (*fp)(void **) = rt_proc_resume_frame_h; resumeg_fp = (uint64_t)(uintptr_t)(void*)fp; }
    return x86_alpha()
         + x86_scan_sync_out()
         + FOR(0, (int)_.op_ival, [&](int i) {
        int slot = bcps_arg_slot(_.node, argblks, i);
        return stage_arg_inline(i, slot, stage_fp);
    })
         + x86("mov", "rdi", (uint64_t)(uintptr_t)(_.op_sval ? _.op_sval : ""))
         + x86("mov32", "esi", (long)_.op_ival)
         + x86_frame_lea("rdx", act)
         + x86("call", "rt_proc_call_gen_h", callg_fp)
         + x86_scan_sync_in_rr()
         + x86_frame_store64(off, "rax")
         + x86_frame_store64(off + 8, "rdx")
         + x86("cmp", "eax", (long)DT_FAIL)
         + x86_omega("je")
         + x86_gamma()
         + x86_beta()
         + x86_scan_sync_out()
         + x86_frame_lea("rdi", act)
         + x86("call", "rt_proc_resume_frame_h", resumeg_fp)
         + x86_scan_sync_in_rr()
         + x86_frame_store64(off, "rax")
         + x86_frame_store64(off + 8, "rdx")
         + x86("cmp", "eax", (long)DT_FAIL)
         + x86_omega("je")
         + x86_gamma();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_txt_gen_arm() {
    int off = bcps_result_slot(); if (off < 0) return x86_bomb("bb_call_proc_staged: no LOWER slot grant (TMP-ERADICATE)");
    int act = off + 16 * (1 + (int)_.op_ival);
    IR_graph_t ** argblks = (IR_graph_t **)(intptr_t)_.op_counter;
    return x86_alpha()
         + x86_scan_sync_out()
         + x86("directive", ".section .rodata")
         + x86("directive", std::string(".Lcall") + std::to_string(_.nid) + "_pname: .string \"" + std::string(_.op_sval ? _.op_sval : "") + "\"")
         + x86("directive", ".section .text")
         + x86("directive", ".intel_syntax noprefix")
         + FOR(0, (int)_.op_ival, [&](int i) {
             int slot = bcps_arg_slot(_.node, argblks, i);
             return x86("mov", "edi", std::to_string(i)) + x86("mov", "rsi", FRQ(slot)) + x86("mov", "rdx", FRQ(slot + 8)) + x86("call", "rt_arg_stage@PLT");
         })
         + x86("directive", (std::string(" lea rdi, [rip + .Lcall") + std::to_string(_.nid) + "_pname]").c_str())
         + x86("mov", "esi", std::to_string((int)_.op_ival))
         + x86_frame_lea("rdx", act)
         + x86("rtcc_wb")
         + x86("call_bare", "rt_proc_call_gen_h@PLT")
         + x86_scan_sync_in_rr()
         + x86("mov", FRQ(off), "rax")
         + x86("mov", FRQ(off + 8), "rdx")
         + x86("cmp", "eax", std::to_string((long)DT_FAIL))
         + x86_omega("je")
         + x86("rtcc_rl")
         + x86_gamma()
         + x86("label", _.lbl_β)
         + x86_scan_sync_out()
         + x86_frame_lea("rdi", act)
         + x86("rtcc_wb")
         + x86("call_bare", "rt_proc_resume_frame_h@PLT")
         + x86_scan_sync_in_rr()
         + x86("mov", FRQ(off), "rax")
         + x86("mov", FRQ(off + 8), "rdx")
         + x86("cmp", "eax", std::to_string((long)DT_FAIL))
         + x86_omega("je")
         + x86("rtcc_rl")
         + x86_gamma();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_proc_staged_str(IR_t * pBB) {
    if (!PLATFORM_X86) return std::string();
    int is_gen = _.op_sval && rt_proc_is_generator(_.op_sval);
    if (is_gen && _.op_node_kind != (int)IR_PROC_GEN && _.op_node_kind != (int)IR_CALL_PROC_STAGED) return x86_alpha() + x86_bomb("bb_call_proc_staged: generator call on an op kind without a callgen.act ZLS2 handle grant (zeta_storage.c widens only IR_PROC_GEN / IR_CALL_PROC_STAGED)");
    if (is_gen) {
        if (x86_zc_frame() == ZC_FRAME_RSP) { if (MEDIUM_BINARY || MEDIUM_TEXT) return bcps_spine_gen_arm(); return std::string(); }   /* GENP-SPINE s92: spine-resident generators under the RSP default; the pthread arms below serve legacy non-RSP frames only */
        if (MEDIUM_BINARY) return bcps_bin_gen_arm();
        if (MEDIUM_TEXT)   return bcps_txt_gen_arm();
        return std::string();
    }
    if (MEDIUM_BINARY || MEDIUM_TEXT) return bcps_det_arm();
    return std::string();
}
