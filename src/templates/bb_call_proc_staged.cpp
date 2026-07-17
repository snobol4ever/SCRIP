#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
long    rt_proc_call_open(const char *name, int nargs);
void   *rt_proc_open_fn(void);
void   *rt_frame_prep(void *fb, long fbytes);
DESCR_t rt_proc_call_epilogue_γ(DESCR_t frame0);
DESCR_t rt_proc_call_epilogue_ω(void);
DESCR_t rt_proc_call_epilogue_ret(DESCR_t fret);
DESCR_t rt_faildescr(void);
DESCR_t rt_proc_call_gen_h(const char *name, int nargs, void **act_slot);
DESCR_t rt_proc_resume_frame(void *act);
DESCR_t rt_proc_resume_frame_h(void **hslot);
int  rt_proc_is_generator(const char *name);
int  rt_proc_dyn_scope(const char *name);
void rt_arg_stage(int idx, DESCR_t v);
int  rt_proc_is_registered(const char *name);
int  bb_slot_get(IR_t * nd);
void bb_slot_register(IR_t * nd, int off);
}
#include "x86_asm.h"
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
 *  · F2 — rbp is the align-save register TODAY (x86_align_save() = rbp while the ζ frame is r12), so the anchor
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
    int bidx = bcps_beta_pair_idx(); IR_graph_t ** argblks = (IR_graph_t **)(intptr_t)_.op_counter;
    uint64_t stage_fp; { void (*fp)(int, DESCR_t) = rt_arg_stage; stage_fp = (uint64_t)(uintptr_t)(void*)fp; }
    uint64_t open_fp;  { long (*fp)(const char *, int) = rt_proc_call_open; open_fp = (uint64_t)(uintptr_t)(void*)fp; }
    uint64_t openfn_fp; { void * (*fp)(void) = rt_proc_open_fn; openfn_fp = (uint64_t)(uintptr_t)(void*)fp; }
    uint64_t prep_fp;  { void * (*fp)(void *, long) = rt_frame_prep; prep_fp = (uint64_t)(uintptr_t)(void*)fp; }
    uint64_t epig_fp;  { DESCR_t (*fp)(DESCR_t) = rt_proc_call_epilogue_γ; epig_fp = (uint64_t)(uintptr_t)(void*)fp; }
    uint64_t epiw_fp;  { DESCR_t (*fp)(void) = rt_proc_call_epilogue_ω; epiw_fp = (uint64_t)(uintptr_t)(void*)fp; }
    uint64_t epir_fp;  { DESCR_t (*fp)(DESCR_t) = rt_proc_call_epilogue_ret; epir_fp = (uint64_t)(uintptr_t)(void*)fp; }
    uint64_t fail_fp;  { DESCR_t (*fp)(void) = rt_faildescr; fail_fp = (uint64_t)(uintptr_t)(void*)fp; }
    /* PROC-CONV regime selector — NCB-1d (Lon "RSP/RBP FORTH ζ for ALL, sharing the C stack", s90): under ZC_FRAME_RSP EVERY det proc call takes the jmp-entry wire — dyn (SNOBOL4) unchanged, det LEXICAL
     * (Icon/Raku) newly joined: the callee blob self-allocates and its lexprep prologue tail binds the staged g_call_args, so the caller-made-frame window below is LEGACY-CONFIG ONLY now (both its arms).
     * The rt table is populated before any emission in both media, so this is emit-time-static.  Congruence with the callee side: emit_jmp_entry_for_proc admits exactly the !is_generator procs, and this
     * det arm is dispatched exactly on !rt_proc_is_generator — one truth source (scrip.c rt_proc_set_generator from proc_table). */
    int is_dyn = _.op_sval && rt_proc_dyn_scope(_.op_sval);
    return x86_alpha()
         + x86_scan_sync_out()
         + x86_anchor_enter()
         + FOR(0, (int)_.op_ival, [&](int i) {
        int slot = bcps_arg_slot(_.node, argblks, i);
        return x86("mov32", "edi", (long)i) + x86("mov", "rsi", FRQ(slot)) + x86("mov", "rdx", FRQ(slot + 8)) + x86("call", "rt_arg_stage", stage_fp);
    })
         + x86_ro_load_q("rdi", 0)
         + x86("mov32", "esi", (long)_.op_ival)
         + x86("call", "rt_proc_call_open", open_fp)
         + x86("test", "rax", "rax")
         + x86("je", L(1))
         + (ZC_FRAME == ZC_FRAME_RSP
            /* R12-ERAD s65: the r12 anchor is DEAD — the callee's LIFO exits fully unwind frame+header before jmping the wire, so rsp at either landing = rsp at the jmp below; result arrives in rdi:rsi.
             * REG-7 s80 GUARD WIDENED (was && !_.flat_pat): proc callees are ALWAYS the determinate full-unwind class under ZC_FRAME_RSP — the suspending zr-exit class is PAT$ fragments, which a proc call
             * can never land in — so a flat_pat CALLER takes this anchor-free wire too, retiring the REG-6 hazard (r12 = pend top rides untouched through the call).  Unexercised intersection (census
             * 0/308): soundness is by the exit-class argument above, non-regression by the gates. */
            ? x86("call", "rt_proc_open_fn", openfn_fp)
            + x86_lea_id("rcx", 3)
            + x86_lea_id("rdx", 4)
            + x86_jmp_reg("rax")
            + x86("def", L(3))
            + x86("call", "rt_proc_call_epilogue_γ", epig_fp)
            + x86("jmp", L(2))
            + x86("def", L(4))
            + x86("call", "rt_proc_call_epilogue_ω", epiw_fp)
            + x86("jmp", L(2))
            : is_dyn
            /* LEGACY-CONFIG ONLY (REG-7 s80 audit resolved; was the ⛔ REG-6 hazard flag): reachable solely
             * when ZC_FRAME != ZC_FRAME_RSP after the guard widening above — configs where r12 IS the ζ frame
             * (pre-REG-MAP tenancy), the pend top is NOT register-resident, and this rsp-anchor bracket is the
             * correct suspending-exit protocol.  Under the RSP default this arm is unreachable, so the REG-6
             * unsoundness (a callee capture bumping the anchor) cannot fire.  Delete with the legacy configs. */
            ? x86("call", "rt_proc_open_fn", openfn_fp)
            + x86("push", "r12")
            + x86("sub", "rsp", 8L)
            + x86_lea_id("rcx", 3)
            + x86_lea_id("rdx", 4)
            + x86("mov", "r12", "rsp")
            + x86_jmp_reg("rax")
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
         + x86("def", L(1))
         + x86("call", "rt_faildescr", fail_fp)
         + x86("def", L(2))
         + x86_anchor_leave()
         + x86_scan_sync_in_rr()
         + x86("mov", FRQ(off), "rax")
         + x86("mov", FRQ(off + 8), "rdx")
         + x86("cmp", "eax", (long)99)
         + x86_omega("je")
         + x86_gamma()
         + x86_beta()
         + (bidx < 0 ? x86_omega() : x86_pair_jmp(bidx))
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
        return x86("mov32", "edi", (long)i) + x86_frame_load64("rsi", slot) + x86_frame_load64("rdx", slot + 8) + x86("call", "rt_arg_stage", stage_fp);
    })
         + x86("mov", "rdi", (uint64_t)(uintptr_t)(_.op_sval ? _.op_sval : ""))
         + x86("mov32", "esi", (long)_.op_ival)
         + x86_frame_lea("rdx", act)
         + x86("call", "rt_proc_call_gen_h", callg_fp)
         + x86_scan_sync_in_rr()
         + x86_frame_store64(off, "rax")
         + x86_frame_store64(off + 8, "rdx")
         + x86("cmp", "eax", (long)99)
         + x86_omega("je")
         + x86_gamma()
         + x86_beta()
         + x86_scan_sync_out()
         + x86_frame_lea("rdi", act)
         + x86("call", "rt_proc_resume_frame_h", resumeg_fp)
         + x86_scan_sync_in_rr()
         + x86_frame_store64(off, "rax")
         + x86_frame_store64(off + 8, "rdx")
         + x86("cmp", "eax", (long)99)
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
         + x86("call", "rt_proc_call_gen_h@PLT")
         + x86_scan_sync_in_rr()
         + x86("mov", FRQ(off), "rax")
         + x86("mov", FRQ(off + 8), "rdx")
         + x86("cmp", "eax", "99")
         + x86_omega("je")
         + x86_gamma()
         + x86("label", _.lbl_β)
         + x86_scan_sync_out()
         + x86_frame_lea("rdi", act)
         + x86("call", "rt_proc_resume_frame_h@PLT")
         + x86_scan_sync_in_rr()
         + x86("mov", FRQ(off), "rax")
         + x86("mov", FRQ(off + 8), "rdx")
         + x86("cmp", "eax", "99")
         + x86_omega("je")
         + x86_gamma();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_proc_staged_str(IR_t * pBB) {
    if (!PLATFORM_X86) return std::string();
    int is_gen = _.op_sval && rt_proc_is_generator(_.op_sval);
    if (is_gen && _.op_node_kind != (int)IR_PROC_GEN && _.op_node_kind != (int)IR_CALL_PROC_STAGED) return x86_alpha() + x86_bomb("bb_call_proc_staged: generator call on an op kind without a callgen.act ZLS2 handle grant (zeta_storage.c widens only IR_PROC_GEN / IR_CALL_PROC_STAGED)");
    if (is_gen) {
        if (MEDIUM_BINARY) return bcps_bin_gen_arm();
        if (MEDIUM_TEXT)   return bcps_txt_gen_arm();
        return std::string();
    }
    if (MEDIUM_BINARY || MEDIUM_TEXT) return bcps_det_arm();
    return std::string();
}
