#include "runtime/core/core.h"
#include <string>
#include "runtime/core/g_runtime.h"
#include <vector>
#include <algorithm>
#include <stdint.h>
#include "emitter/emit_knobs.h"
#include "ir/ir_node.h"
#include "ir/ir_graph.h"
#include "emitter/emit_regime.h"
#include "emitter/emit_str.h"
#include "emitter/emit_gc_site.h"
#include "planner/frame_layout.h"
extern "C" int emit_zrow_actoff(const IR_t * nd);
extern "C" int emit_zrow_sprec(const IR_t * nd);
extern "C" {
#include "bb_templates.h"
#include "emitter/bb_label.h"
#include "emitter/emit_state.h"
int zls_g_resumable_frame(const IR_graph_t * g);
#include "runtime/rt/pl_trail_layout.h"
#include "runtime/rt/rt_pl_trail.h"
#include "bb_pl_cell.h"
#include "runtime/rt/gc_heap.h"
long rt_proc_call_open(const char *name, int nargs);
void *rt_proc_fn(const char *name); const char *bb_ab_sym_name(const char *nm); const char *bb_ab_fn_sym(const char *nm);
void *rt_proc_call_open_det(long idx, int nargs);
void *rt_proc_call_open_det0(long idx);
void *rt_proc_call_open_det1(long idx, DESCR_t *a0);
void *rt_proc_call_open_det2(long idx, DESCR_t *a0, DESCR_t *a1);
void *rt_proc_call_open_det3(long idx, DESCR_t *a0, DESCR_t *a1, DESCR_t *a2);
void *rt_proc_call_open_det4(long idx, DESCR_t *a0, DESCR_t *a1, DESCR_t *a2, DESCR_t *a3);
int rt_proc_index_of(const char *name);
void *rt_proc_open_fn(void);
void *rt_frame_prep(void *fb, long fbytes);
DESCR_t rt_proc_call_epilogue_γ(DESCR_t frame0, long touched);
DESCR_t rt_proc_call_epilogue_ω(long touched);
DESCR_t rt_proc_call_epilogue_named_γ(const char *name, long wn);
DESCR_t rt_proc_call_epilogue_named_ω(const char *name, long wn);
DESCR_t rt_faildescr(void);
void rt_ab_undef_fn_stub(void); void rt_ab_undef_sig_stub(void);
void rt_ab_undef_fn_fail(void);
void rt_pl_iso_throw_existence_key(const char *key);
DESCR_t rt_proc_call_gen_h(const char *name, int nargs, void **act_slot, const uint64_t *regs);
DESCR_t rt_proc_resume_frame_h(void **hslot);
DESCR_t rt_gen_spine_pass_γ(DESCR_t v);
DESCR_t rt_gen_spine_pass_ω(void);
void rt_gen_spine_resume_enter(void);
int zls_g_resume_by_name(const char *name);
int zls_g_block_args(const IR_graph_t * g);
int zls_g_resumable_frame(const IR_graph_t * g);
int rt_proc_is_generator(const char *name);
int rt_define_returns_by_frame(const char *);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define bcps_wire_pair_consumed(fname) (((fname) && rt_define_returns_by_frame((fname))) ? 0 : 1)
int rt_proc_dyn_scope(const char *name);
void rt_arg_stage(int idx, DESCR_t v);
extern "C" struct gv_s g_call_args;
extern "C" int g_gc_pending;
extern "C" DESCR_t rt_nret_name(DESCR_t r);
int rt_proc_is_registered(const char *name); int rt_proc_is_redefined(const char *name); int rt_proc_is_variadic(const char *name);
int rt_proc_nformals(const char *name);
int rt_pl_dc_ok(const char *name, int nargs);
void **rt_pl_dc_slot(long idx);
DESCR_t rt_nret_fix(DESCR_t r, int wn);
int rt_proc_nparams(const char *name);
const char *rt_proc_pname(const char *name, int k);
const char *rt_proc_result_name_get(const char *name);
int scc_program_ok(void);
int gva_index_of(const char *name); int rt_gva_add_var(const char *name); int gva_trace_demoted(void);
extern int g_gva_active;
int bb_slot_get(IR_t * nd);
void bb_slot_register(IR_t * nd, int off);
int bb_proc_target_zframe_graph(const char *fname);
int bb_proc_target_pinned_graph(const char *fname); int bb_proc_target_det_block(const char *fname);
int bb_proc_target_block_graph(const char *fname); int bb_sig_site_ok(const char *fname, int nargs);
int rt_proc_pinned(const char *name);
void *bb_ab_fn_cell_ptr(const char *fname);
int bb_proc_target_icn_block(const char *fname, int *np, int *vari);
DESCR_t rt_make_list(DESCR_t *args, int nargs);
void rt_trace_call_hook_f(const char *fname, int np, void *base);
int zls_g_block_args(const IR_graph_t * g);
}
#include "templates/x86/x86_ops.h"
#include "templates/x86/x86_call.h"
#include "templates/x86/x86_frame.h"
#include "templates/x86/x86_gc.h"
#include "templates/x86/x86_icn.h"
#include "templates/x86/x86_jump.h"
#include "templates/x86/x86_rsp_tracker.h"
#include "templates/x86/x86_rtcc.h"
#include "templates/x86/x86_rodata.h"
#include "templates/x86/x86_zeta.h"
#include "templates/x86/x86_zverify.h"
extern "C" {
#include "optimizer/gva_collect.h"
}
extern "C++" long bcps_parse_rsp(const char * t);
extern "C++" int bcps_arg_slot(IR_t * call, int i);
extern "C++" int bcps_result_slot();
extern "C++" int bcps_beta_pair_idx();
extern "C++" std::string bcps_alpha_cellp_label(const char * fname);
extern "C++" std::string bcps_alpha_cellp_data(const char * fname);
extern "C++" void bcps_sig_tally(const char * arm, const char * fn, long n, int ok, const char * why, const char * opnd);
#define RO_SEAL_STR(n, s) \
    (x86(".quad_ro", L(n), LS(n), (s)) \
   + x86("label", LS(n)) \
   + x86(".string", (s)))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_beta_leave(int bidx) { return bidx < 0 ? x86_omega() : x86_port_hook(X86H_JMP, X86P_OMEGA) + x86_zdp_rbp_omega_at(X86P_OMEGA) + x86_pair_jmp(bidx); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define icn_wire_stack_on() emit_knob_unless_zero("SCRIP_ICN_WIRE_STACK")
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define icn_wire_stack_for(fname) (icn_wire_stack_on() && !bb_proc_target_zframe_graph((fname)))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define bcps_wire_cross(gid, wid, fname) (icn_wire_stack_for((fname)) ? bb_glue_pass_wires_blob((gid), (wid)) : bb_glue_pass_wires((gid), (wid)))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_wire_cross_gen(int gid, int wid) {
    if (!icn_wire_stack_on()) return bb_glue_pass_wires(gid, wid);
    return x86_lea_id("rcx", wid) + x86("push", "rcx")
         + x86_lea_id("rcx", gid) + x86("push", "rcx")
         + x86_lea_id("rdx", wid)
         + x86_jmp_reg("rax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define bcps_wire_land_site(fname) (x86_rsp_land((icn_wire_stack_for((fname)) && !bcps_wire_pair_consumed((fname))) ? 16L : 0L) + x86_gc_site(X86_SITE_LANDING))
#define bcps_wire_land(fname) (icn_wire_stack_for((fname)) ? IF(!bcps_wire_pair_consumed((fname)), x86("add", "rsp", 16L)) : std::string())
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define bcps_retfix() emit_knob_unless_zero("SCRIP_RET_FIX")
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int bcps_pl();
#define bcps_open_word_keep() (x86("sub", "rsp", 16L) + x86_rsp_store64(0, "rax"))
static std::string bcps_epi_named(int is_omega, uint64_t bare_fp)
{
    const char * fr = is_omega ? "rdi" : "rdx", * fr32 = is_omega ? "edi" : "edx";
    if (!bcps_retfix()) return (bcps_pl() ? x86("mov32", fr32, 0L) : x86_rsp_load64(fr, 0)
                                          + x86("shr", fr, 61L)
                                          + x86("and", fr32, 1L)
                                          + x86("add", "rsp", 16L))
         + x86("call", is_omega ? "rt_proc_call_epilogue_ω" : "rt_proc_call_epilogue_γ", bare_fp);
    return (bcps_pl() ? x86("mov32", "esi", 0L) : x86_rsp_load64("rsi", 0)
                      + x86("shr", "rsi", 62L)
                      + x86("add", "rsp", 16L))
         + x86("mov", "rdi", ROQ(0))
         + x86("call", is_omega ? "rt_proc_call_epilogue_named_ω" : "rt_proc_call_epilogue_named_γ",
        TEMPLATE_FN_ADDR(is_omega ? rt_proc_call_epilogue_named_ω : rt_proc_call_epilogue_named_γ))
         + x86_rt_gc_poll_res();
}
#define bcps_epi_det(is_omega, is_gen) (x86("mov32", (is_omega) ? "edi" : "edx", (long)((is_gen) ? 1 : 0)))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define SAI_L0 200
static std::string stage_arg_inline(int i, int slot, uint64_t stage_fp) {
    std::string slow = x86("mov32", "edi", (long)i)
                     + x86("mov", "rsi", FRQ(slot))
                     + x86("mov", "rdx", FRQ(slot + 8))
                     + x86("call", "rt_arg_stage", stage_fp)
                        + x86_rt_gc_poll();
    if (i < 0 || i >= 8 || getenv("SCRIP_NO_SINK")) return slow;
    return x86("lea", "r8", "[rip + __]", (uint64_t)(uintptr_t)&g_gc_pending, "g_gc_pending")
         + x86("mov", "eax", "dword ptr [r8 + 0]")
         + x86("test", "eax", "eax")
         + x86("jne", L(SAI_L0 + i * 2))
         + x86("mov", "rax", FRQ(slot))
         + x86("mov", "rdx", FRQ(slot + 8))
         + x86("mov", "r8", "[rip@got + __]", (uint64_t)(uintptr_t)&g_call_args, "g_call_args")
         + x86("mov", "r8", "qword ptr [r8 + 0]")
         + x86("test", "r8", "r8")
         + x86("je", L(SAI_L0 + i * 2))
         + x86("mov", (std::string("[r8 + ") + std::to_string(i * 16) + "]").c_str(), "rax")
         + x86("mov", (std::string("[r8 + ") + std::to_string(i * 16 + 8) + "]").c_str(), "rdx")
         + x86("xor", "r8d", "r8d")
         + x86("jmp", L(SAI_L0 + 1 + i * 2))
         + x86("def", L(SAI_L0 + i * 2))
         + slow
         + x86("def", L(SAI_L0 + 1 + i * 2));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int bcps_pl() { return x86_resumable_frame(); }
#define bcps_fnsig() emit_knob_unless_zero("SCRIP_FN_SIG")
#define bcps_no_tiny() emit_knob_if_one("SCRIP_NO_TINY")
#define bcps_b1c_parity() emit_knob_unless_zero("SCRIP_B1C_PARITY")
#define bcps_slim_pair() emit_knob_unless_zero("SCRIP_SLIM_PAIR")
#define bcps_no_genidx() emit_knob_if_one("SCRIP_NO_GENIDX")
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define bcps_nret_got() std::string("[rip@got + __]")
#define bcps_cap_name_strict() emit_knob_unless_zero("SCRIP_CAP_NAME_STRICT")
static std::string bcps_nret_consult(const std::string & r0, const std::string & r8) {
    extern int rt_g_ret_by_name;
    extern int rt_g_want_name;
    extern DESCR_t rt_deref(DESCR_t d);
    return x86("note", std::string("NRETURN by-name consult (live wn, consumed): the fix-up is emitted here, rt_nret_fix_tiny is gone from the graph"))
         + x86("mov", "rcx", bcps_nret_got(), (uint64_t)(uintptr_t)(void *)&rt_g_ret_by_name, "rt_g_ret_by_name")
         + x86("mov", "ecx", RDD("rcx", 0))
         + x86("cmp", "ecx", (long)0)
         + x86("je", L(29))
         + x86("cmp", "eax", (long)DT_N)
         + x86("je", L(25))
         + x86("mov", "rdi", "rax")
         + x86("mov", "rsi", "rdx")
         + x86_rtcc_call_descr_ops("rt_nret_name", TEMPLATE_FN_ADDR(rt_nret_name), r0, r8)
         + x86("mov", "rax", r0.c_str())
         + x86("mov", "rdx", r8.c_str())
         + x86("cmp", "eax", (long)DT_N)
         + x86("je", L(25))
         + x86("mov", "rcx", bcps_nret_got(), (uint64_t)(uintptr_t)(void *)&rt_g_ret_by_name, "rt_g_ret_by_name")
         + x86("mov", RDD("rcx", 0), (long)0)
         + x86("jmp", L(23))
         + x86("def", L(25))
         + x86("mov", "rcx", bcps_nret_got(), (uint64_t)(uintptr_t)(void *)&rt_g_want_name, "rt_g_want_name")
         + x86("mov", "ecx", RDD("rcx", 0))
         + x86("cmp", "ecx", (long)0)
         + x86("jne", L(24))
         + x86("mov", "rcx", bcps_nret_got(), (uint64_t)(uintptr_t)(void *)&rt_g_ret_by_name, "rt_g_ret_by_name")
         + x86("mov", RDD("rcx", 0), (long)0)
         + x86("cmp", "eax", (long)DT_N)
         + x86("jne", L(23))
         + x86("mov", "rdi", "rax")
         + x86("mov", "rsi", "rdx")
         + x86_rtcc_call_descr_ops("rt_deref", TEMPLATE_FN_ADDR(rt_deref), r0, r8)
         + x86("mov", "rax", r0.c_str())
         + x86("mov", "rdx", r8.c_str())
         + x86("jmp", L(23))
         + x86("def", L(24))
         + IF(!bcps_cap_name_strict(),
               x86("mov", "rcx", bcps_nret_got(), (uint64_t)(uintptr_t)(void *)&rt_g_ret_by_name, "rt_g_ret_by_name")
             + x86("mov", RDD("rcx", 0), (long)0))
         + x86("def", L(23))
         + x86("mov", "rcx", bcps_nret_got(), (uint64_t)(uintptr_t)(void *)&rt_g_want_name, "rt_g_want_name")
         + x86("mov", RDD("rcx", 0), (long)0)
         + x86("def", L(29));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define bcps_zref_disp(zoff) (bcps_parse_rsp(x86_zref((zoff), 1)))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_undef_fallback(uint64_t) {
    return x86("call", "rt_ab_undef_fn_fail", (uint64_t)(uintptr_t)(void *)rt_ab_undef_fn_fail)
         + x86_rt_gc_poll() + x86_omega(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_block_build(int n);
static std::string bcps_icn_block_arm(int is_gen, int off, int act, long idx, int np, int vari, int n2_ftc);
static std::string bcps_block_build(int n); static std::string bcps_det_block_arm(int zres, int off, int bidx, long idx, int nb, int wires);
static std::string bcps_pinned_byname_road(const std::string & blk, int fncell, int bump, int lγ, int lω, int lskip);
static std::string bcps_det_arm() {
    x86_begin();
    int off = bcps_result_slot(); if (off < 0) return x86_bomb("bb_call_proc_staged: no LOWER slot grant (TMP-ERADICATE)");
    { int ibnp = 0, ibva = 0; long ibidx = (_.op_sval && !rt_proc_dyn_scope(_.op_sval)) ? (long)rt_proc_index_of(_.op_sval) : -1L;
      if (ibidx >= 0 && bb_proc_target_icn_block(_.op_sval, &ibnp, &ibva)) return bcps_icn_block_arm(0, off, 0, ibidx, ibnp, ibva, 0); }
    if (_.op_zres) {
        int bidx_z = bcps_beta_pair_idx();
        int is_dyn_z = _.op_sval && rt_proc_dyn_scope(_.op_sval);
        long det_idx_z = (!is_dyn_z && _.op_sval) ? (long)rt_proc_index_of(_.op_sval) : -1L;
        int det_nA_z = (int)_.op_ival; { int _bnp = (det_idx_z >= 0 && _.op_sval) ? bb_proc_target_det_block(_.op_sval) : -1;
            if (_bnp >= 0) return _bnp != det_nA_z ? x86_alpha() + x86_bomb("bb_call_proc_staged: a block-protocol callee is called with the wrong argument count")
                : bcps_det_block_arm(1, off, bidx_z, det_idx_z, det_nA_z, 1); }
        int det_fuse_z = (det_idx_z >= 0 && det_nA_z >= 0 && det_nA_z <= 4);
        int dc_z = 0; uint64_t dc_slot_z = 0; char dc_name_z[_.op_sval ? fmt_len("%s_dc\xce\xb1", bb_ab_sym_name(_.op_sval)) : 1]; dc_name_z[0] = 0;
        if (det_fuse_z && _.op_sval && rt_pl_dc_ok(_.op_sval, det_nA_z)) {
            void **sl = rt_pl_dc_slot(det_idx_z); if (sl) { dc_z = 1; dc_slot_z = (uint64_t)(uintptr_t)sl;
                snprintf(dc_name_z, sizeof dc_name_z, "%s_dc\xce\xb1", bb_ab_sym_name(_.op_sval)); } }
        static const char * const detN_argreg_z[4] = { "rsi", "rdx", "rcx", "r8" };
        uint64_t detN_fp_z[5];
        { void *(*f0)(long) = rt_proc_call_open_det0; detN_fp_z[0] = (uint64_t)(uintptr_t)(void*)f0; }
        { void *(*f1)(long, DESCR_t*) = rt_proc_call_open_det1; detN_fp_z[1] = (uint64_t)(uintptr_t)(void*)f1; }
        { void *(*f2)(long, DESCR_t*, DESCR_t*) = rt_proc_call_open_det2; detN_fp_z[2] = (uint64_t)(uintptr_t)(void*)f2; }
        { void *(*f3)(long, DESCR_t*, DESCR_t*, DESCR_t*) = rt_proc_call_open_det3; detN_fp_z[3] = (uint64_t)(uintptr_t)(void*)f3; }
        { void *(*f4)(long, DESCR_t*, DESCR_t*, DESCR_t*, DESCR_t*) = rt_proc_call_open_det4; detN_fp_z[4] = (uint64_t)(uintptr_t)(void*)f4; }
        static const char * const detN_nm_z[5] = { "rt_proc_call_open_det0","rt_proc_call_open_det1","rt_proc_call_open_det2","rt_proc_call_open_det3","rt_proc_call_open_det4" };
        uint64_t dc_slot_fp_z = dc_slot_z;
        return x86_alpha()
             + x86_anchor_enter()
             + ((_.op_sig_ok || (_.op_sval && bb_sig_site_ok(_.op_sval, (int)_.op_ival)))
                ? [&]() -> std::string {
                    if (!bcps_no_tiny() && ({ !g_runtime.eval.rt_fragment_emit || bcps_b1c_parity(); }) && _.op_sval && bb_sig_site_ok(_.op_sval, (int)_.op_ival)) {
                        std::string laz = bcps_alpha_cellp_label(_.op_sval); std::string lazd = bcps_alpha_cellp_data(_.op_sval);
                        if (bcps_fnsig()) {
                            std::vector<long> soffz((size_t)_.op_ival); const char * whyz = "eligible"; std::string badz; int sigokz = 1;
                            for (int i = 0; sigokz && i < (int)_.op_ival; i++) {
                                int zs = _.op_zread[i];
                                if (x86_fc_hit(zs) || x86_fc_hit(zs + 8)) { sigokz = 0; whyz = "fc-hit"; break; }
                                long dlo = bcps_zref_disp(zs), dhi = bcps_zref_disp(zs + 8);
                                if (dlo < 0) { sigokz = 0; whyz = "unparsed-operand"; badz = x86_zref(zs, 1); break; }
                                if (dhi != dlo + 8) { sigokz = 0; whyz = "hi-lo-not-adjacent"; badz = x86_zref(zs + 8, 1); break; }
                                soffz[i] = dlo; }
                            bcps_sig_tally("zref", _.op_sval, (long)_.op_ival, sigokz, whyz, badz.c_str());
                            if (sigokz) {
                                std::string snmz = std::string(".L") + x86_boxkind() + "_sig" + std::to_string((long)_.x86_uid) + "z";
                                const struct bb_label_t * sigl_z = emit_label_intern(snmz.c_str());
                                std::string sz = lazd + x86_rsp_mark_save()
                                     + x86("lea", "rcx", "extlbl", (uint64_t)(uintptr_t)sigl_z)
                                     + x86("jmp_cellp", "[rip@cell + __]", (uint64_t)(uintptr_t)bb_ab_fn_cell_ptr((std::string("alpha$") + _.op_sval).c_str()), laz.c_str())
                                     + x86_ro_rec_head(sigl_z, (uint64_t)_.op_ival)
                                     + x86(".quad", L(2))
                                     + (_.op_ival > 0 ? x86(".quad", L(2)) : x86_ro_rec_tail_ilbl(2));
                                for (int i = 0; i < (int)_.op_ival; i++) sz += (i + 1 < (int)_.op_ival) ? x86(".quad", (uint64_t)soffz[i]) : x86_ro_rec_tail_q((uint64_t)soffz[i]);
                                return sz + x86_rsp_land(0L);
                            }
                            return x86_bomb("bcps signature arm DECLINED and there is no safe fallthrough: the callee's role-4 SIG shim (bb_define.cpp, "
                                "fnsig()) reads its formals from [rcx + 24 + 8i], but a site that cannot compute the staged offsets falls "
                                    "through to the SCC/open_slim convention, which puts the gamma continuation in rcx instead -- one entry, two "
                                        "calling conventions, chosen independently by caller and callee, and the callee dereferences whatever it is "
                                            "handed. MEASURED hq_S 2026-09-06, incremental make, RT_OPT=-O0, m3 and m4 agreeing: 2507 shipped programs "
                                                "compiled, 228 of them carry a signature site, 3639 live sites in total, DECLINES = 0. The 42 frame-arm "
                                                    "declines that existed before bcps_parse_rsp learned x86_fr64_prefix's '$' spelling are all SIG now (ADDON/1 "
                                                        "x18, CUT/1 x9, PR/1 x6, PR/2 x6, RET/1 x3, in aisnobol, gimpel and snoflake_suite). So this refusal has a "
                                                            "measured hit rate of ZERO, and reaching it means a NEW operand spelling or frame shape got here: run "
                                                                "SCRIP_SIG_DIAG=1, which prints arm/fn/nargs/why/opnd for every site and names the operand that would not "
                                                                    "parse. One theoretical false positive, unmeasured because it has zero instances today: the callee tests "
                                                                        "bb_tiny_shim_ok(fn, 0) while this site tests it at the real nargs, so a proc tiny-ok at N but not at 0 would "
                                                                            "bomb here where slim-to-slim was in fact correct. The standing cure for that shape is two entry points at the "
                                                                                "callee, not a silent decline here. [arm=zref]");
                        } else {
                        long Kbz = 16L * (long)_.op_ival + 32;
                        auto ZOPQT = [&](int i, int w) { return x86_zref(_.op_zread[i] + w + (int)Kbz, 1); };
                        return lazd + x86("sub", "rsp", Kbz)
                             + FOR(0, (int)_.op_ival, [&](int i) {
                                   return x86("note", ZOPN(i))
                                        + x86("mov", "rax", ZOPQT(i, 0)) + x86_rsp_store64(32 + 16 * i, "rax")
                                        + x86("note", ZOPN(i))
                                        + x86("mov", "rax", ZOPQT(i, 8)) + x86_rsp_store64(32 + 16 * i + 8, "rax"); })
                             + x86("mov32", "eax", (long)_.op_ival) + x86_rsp_store64(0, "rax")
                             + x86("lea", "rax", L(2)) + x86_rsp_store64(16, "rax") + x86_rsp_store64(24, "rax")
                             + x86("jmp_cellp", "[rip@cell + __]", (uint64_t)(uintptr_t)bb_ab_fn_cell_ptr((std::string("alpha$") + _.op_sval).c_str()), laz.c_str());
                        }
                    }
                    if (!_.op_sig_ok) return std::string();
                    return x86_bomb("bb_call_proc_staged: the CALL2BB slice-2 slim road (zref arm) retired 2026-09-27 on a plant proof over the 4659 corpus sources, "
                                    "the 5027 rungs entries compiled and the SNOBOL4, Snocone and Rebus rungs entries run in mode 3 (0 reached it once the role-4 shim "
                                    "took 30 to 60 formals, scripts/gc_rungs_entry_plant_receipt.tsv): the SCC probe held but no tiny or signature arm took the call");
                }()
                : std::string(""))
             + (!_.op_sig_ok && dc_z
                ? FOR(0, det_nA_z, [&](int i) { return x86("note", ZOPN(i))
                + x86("lea", detN_argreg_z[i], ZOPQ(i, 0)); })
                + x86_call_dc(dc_name_z, dc_slot_fp_z)
                + x86("jmp", L(2))
                : std::string(""))
             + (!_.op_sig_ok && !dc_z
                ? ((det_fuse_z
                    ? x86("mov32", "edi", det_idx_z)
                    + FOR(0, det_nA_z, [&](int i) { return x86("note", ZOPN(i))
                    + x86("lea", detN_argreg_z[i], ZOPQ(i, 0)); })
                    + x86("call", detN_nm_z[det_nA_z], detN_fp_z[det_nA_z])
                    : ((det_idx_z >= 0
                        ? FOR(0, (int)_.op_ival, [&](int i) { return x86("mov32", "edi", (long)i)
                        + x86("note", ZOPN(i))
                        + x86("mov", "rsi", ZOPQ(i, 0))
                        + x86("note", ZOPN(i))
                        + x86("mov", "rdx", ZOPQ(i, 8))
                        + x86("call", "rt_arg_stage", TEMPLATE_FN_ADDR(rt_arg_stage))
                        + x86_rt_gc_poll(); })
                        + x86("mov32", "edi", (long)det_idx_z)
                        + x86("mov32", "esi", (long)_.op_ival)
                        + x86("call", "rt_proc_call_open_det", (uint64_t)TEMPLATE_FN_ADDR(rt_proc_call_open_det))
                        : FOR(0, (int)_.op_ival, [&](int i) {return x86("mov32", "edi", (long)i)
                        + x86("note", ZOPN(i))
                        + x86("mov", "rsi", ZOPQ(i, 0))
                        + x86("note", ZOPN(i))
                        + x86("mov", "rdx", ZOPQ(i, 8))
                        + x86("call", "rt_arg_stage", TEMPLATE_FN_ADDR(rt_arg_stage))
                        + x86_rt_gc_poll(); })
                        + x86("mov", "rdi", ROQ(0))
                        + x86("mov32", "esi", (long)_.op_ival)
                        + x86("call", "rt_proc_call_open", TEMPLATE_FN_ADDR(rt_proc_call_open)))))
                   + x86_rt_gc_poll()
                   + x86("test", "rax", "rax")
                   + x86("je", L(1))
                   + IF(det_idx_z < 0 && !bcps_pl(), bcps_open_word_keep())
                   + (det_idx_z >= 0 && det_fuse_z ? std::string("") : x86("mov", "rdi", ROQ(0))
                   + x86("call", "rt_proc_fn", TEMPLATE_FN_ADDR(rt_proc_fn)))
                   + IF(det_idx_z < 0 && bcps_pl(), bcps_pinned_byname_road(x86("sub", "rsp", (long)(16 * det_nA_z)) + FOR(0, det_nA_z, [&](int i) { return
                   x86("note", ZOPN(i))
                   + x86("mov", "rax", ZOPQS(i, 16 * det_nA_z, 0)) + x86_rsp_store64(16 * i, "rax")
                   + x86("mov", "rax", ZOPQS(i, 16 * det_nA_z, 8)) + x86_rsp_store64(16 * i + 8, "rax"); }), off, 16 * det_nA_z, 61, 62, 60))
                   + IF(!icn_wire_stack_for(_.op_sval) && bcps_slim_pair() && bcps_wire_pair_consumed(_.op_sval), x86("note",
                           "s111 floater pair (ZD twin NON-SLIM fallback): THE arm GVA-off actually reaches — MONITOR_BIN forces "
                               "n_gva_m3=0, the slim tail at ~:403 that s110 patched refuses, and the site falls through to rt_proc_call_open "
                                   "here with flat rcx/rdx wires and NO pair.  Push omega then gamma = [rsp+0]=gamma [rsp+8]=omega; the fnrbp2 "
                                       "floater consumes 16 so L(3)/L(4) arrive at today's depth.  SCRIP_ICN_WIRE_STACK=0 restores prior bytes.")
                                                                                                             + x86("lea", "rcx", L(4))
                                                                                                             + x86("push", "rcx")
                                                                                                             + x86("lea", "rcx", L(3))
                                                                                                             + x86("push", "rcx"))
                   + x86_rsp_mark_save()
                   + bcps_wire_cross(3, 4, _.op_sval)
                   + x86("def", L(3))
                   + bcps_wire_land_site(_.op_sval)
                   + bcps_wire_land(_.op_sval)
                   + (det_idx_z >= 0 ? bcps_epi_det(0, rt_proc_is_generator(_.op_sval)) + x86("call", "rt_proc_call_epilogue_γ", TEMPLATE_FN_ADDR(rt_proc_call_epilogue_γ))
                                      : bcps_epi_named(0, TEMPLATE_FN_ADDR(rt_proc_call_epilogue_γ)))
                   + x86("jmp", L(2))
                   + x86("def", L(4))
                   + bcps_wire_land_site(_.op_sval)
                   + bcps_wire_land(_.op_sval)
                   + (det_idx_z >= 0 ? bcps_epi_det(1, rt_proc_is_generator(_.op_sval)) + x86("call", "rt_proc_call_epilogue_ω", TEMPLATE_FN_ADDR(rt_proc_call_epilogue_ω))
                                      : bcps_epi_named(1, TEMPLATE_FN_ADDR(rt_proc_call_epilogue_ω)))
                   + x86("jmp", L(2))
                   + x86("def", L(1))
                   + bcps_undef_fallback(TEMPLATE_FN_ADDR(rt_ab_undef_fn_stub)))
                : std::string(""))
             + x86("def", L(2))
             + x86_gc_site(X86_SITE_LANDING)
             + x86_anchor_leave()
             + IF(!bb_proc_target_block_graph(_.op_sval), bcps_nret_consult(std::string(ZRES(0)), std::string(ZRES(8))))
             + x86("note", ZRESN())
             + x86("mov", ZRES(0), "rax")
             + x86("note", ZRESN())
             + x86("mov", ZRES(8), "rdx")
             + x86("cmp", "al", (long)DT_FAIL)
             + x86_omega("je")
             + x86_gamma()
             + x86_beta()
             + bcps_beta_leave(bidx_z)
             + RO_SEAL_STR(0, _.op_sval ? _.op_sval : "");
    }
    return x86_alpha()
         + x86_bomb("bb_call_proc_staged: bcps_det_arm flat twin (op_zres 0) retired by R-2002, no witness reached it; a program that gets here names itself");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_jmp_proc_fn(long idx) {
    return x86("note",
        "the registry jump: the record's fn word (PROC_FN at +8, the 128-byte stride both asserted in rt.c), so a fn "
            "re-sealed at run time -- a dynamic predicate's enumerator, a redefinition -- is followed in both media")
         + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_runtime, "g_runtime")
         + x86("mov", "rax", RDQ("rax", GRT_AT(rt_gen_procs)))
         + x86("mov", "rax", RDQ("rax", (int)(8 + 128 * idx)))
         + x86_jmp_reg("rax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_jmp_callee(long idx) {
    if (_.node && (_.node->strict & (1 << 30)) && _.op_sval) return x86("note",
        "a static callee (ARCH-PROLOG-C-OUT-OF-THE-BOX 9.3): a direct jump to its entry label in the text, through its "
            "alpha cell in the slab (x86_jmp_via_cell, both media; the cell is sealed by rt_proc_seal_alpha when the "
                "record's fn is set and pinned, under no knob); the registry record is for a fn re-sealed at run time, which a static predicate never is (assertz on it raises permission_error)")
                                                               + x86("jmp", "[rip@cell + __]", (uint64_t)(uintptr_t)bb_ab_fn_cell_ptr((std::string("alpha$") + _.op_sval).c_str()),
                                                                   bb_ab_fn_sym(_.op_sval));
    return bcps_jmp_proc_fn(idx);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_lvl_add(long d);
static const char * bcps_fncell_past_block(int fncell, int bump) {
    const char * a = FRQ(fncell);
    if (!strncmp(a, "qword ptr [rsp# + ", 18)) return x86_zref(atoi(a + 18) + bump, 1);
    if (!strncmp(a, "qword ptr [rsp + ", 17)) return x86_zref(atoi(a + 17) + bump, 1);
    return a;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_pinned_byname_road(const std::string & blk, int fncell, int bump, int lγ, int lω, int lskip) {
    extern int rt_proc_pinned(const char *);
    return x86("note",
        "the by-name road into a callee whose record reads PINNED at run time (a dynamic predicate's enumerator, a runtime-compiled graph): the block protocol, decided by the record, never by a name")
         + x86("mov", FRQ(fncell), "rax")
         + x86("mov", "rdi", ROQ(0))
         + x86("call", "rt_proc_pinned", TEMPLATE_FN_ADDR(rt_proc_pinned))
         + x86("test", "eax", "eax")
         + x86("je", L(lskip))
         + blk
         + x86_rsp_mark_save()
         + x86_lea_id("rcx", lγ) + x86_lea_id("rdx", lω)
         + x86("mov", "rax", bcps_fncell_past_block(fncell, bump))
         + x86_jmp_reg("rax")
         + x86("def", L(lγ))
         + x86_rsp_land(0L)
         + x86_gc_site(X86_SITE_LANDING)
         + bcps_lvl_add(-1L) + x86("mov", "rax", "rdi")
         + x86("mov", "rdx", "rsi")
         + x86("jmp", L(2))
         + x86("def", L(lω))
         + x86_rsp_land(0L)
         + x86_gc_site(X86_SITE_LANDING)
         + bcps_lvl_add(-1L) + x86("mov32", "eax", (long)DT_FAIL)
         + x86("xor", "edx", "edx")
         + x86("jmp", L(2))
         + x86("def", L(lskip))
         + x86("mov", "rax", FRQ(fncell));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_lvl_add(long d) {
    extern int * const rt_k_level_p;
    return x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&rt_k_level_p, "rt_k_level_p")
         + x86("mov", "rax", RDQ("rax", 0))
         + x86("add", RDD("rax", 0), d);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_block_zop(int i, int w, int n) {
    int in = i >= 0 && i < _.op_zcap;
    return (in && _.op_zread_xf[i] != -1) ? std::string(RDQ("rbp", _.op_zread_xf[i] + w)) : std::string(x86_zref((in ? _.op_zread[i] : 0) + w + 16 * n, 1));
}
static std::string bcps_block_build(int n) {
    return x86("sub", "rsp", (long)(16 * n))
         + IF(_.op_zres, FOR(0, n, [&](int i) { return x86("note", ZOPN(i))
                                                     + x86("mov", "rax", bcps_block_zop(i, 0, n)) + x86_rsp_store64(16 * i, "rax")
                                                     + x86("note", ZOPN(i))
                                                     + x86("mov", "rax", bcps_block_zop(i, 8, n)) + x86_rsp_store64(16 * i + 8, "rax"); }))
         + IF(!_.op_zres, FOR(0, n, [&](int i) { int slot = bcps_arg_slot(_.node, i);
               return x86("note", std::string("block A") + std::to_string(i))
                    + x86("mov", "rax", FRQB(slot, 16 * n)) + x86_rsp_store64(16 * i, "rax")
                    + x86("mov", "rax", FRQB(slot + 8, 16 * n)) + x86_rsp_store64(16 * i + 8, "rax"); }));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_icn_lvl(long d) {
    extern int * const rt_k_level_p;
    return x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&rt_k_level_p, "rt_k_level_p")
         + x86("mov", "rax", RDQ("rax", 0))
         + x86("add", RDD("rax", 0), d)
         + x86("mov", "ecx", RDD("rax", 0))
         + x86("movsxd", "rcx", "ecx")
         + x86("sub", "rcx", 1L)
         + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_runtime, "g_runtime")
         + x86("mov", RDQ("rax", GRT_AT(kw_fnclevel)), "rcx");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_icn_opnd(int i, int w, long d) {
    std::string u = _.op_zres ? x86_zref((i < _.op_zcap ? _.op_zread[i] : 0) + w, 1) : FRQ(bcps_arg_slot(_.node, i) + w);
    return (u.find("rbp") != std::string::npos || (u.find("rsp$") != std::string::npos && x86_fb_num() == 5)) ? u
         : _.op_zres ? std::string(x86_zref((i < _.op_zcap ? _.op_zread[i] : 0) + w + (int)d, 1))
         : std::string(FRQB(bcps_arg_slot(_.node, i) + w, (int)d));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define bcps_icn_src(i, w, d) ( \
      (_.op_zres && (i) < _.op_zcap && _.op_zread_xf[(i)] != -1) ? x86("mov", "rcx", RDQ("rbp", _.op_zread_xf[(i)] + (w))) \
    : (_.op_zres && (i) < _.op_zcap && _.op_zread_fr[(i)] >= 0) ? x86("mov", "rcx", x86_zop(_.op_zread_fr[(i)] + (w), 1, (int)(d))) \
    : (!_.op_zres && x86_fc_hit(bcps_arg_slot(_.node, (i)) + (w))) \
    ? x86_rsp_load64("rcx", bcps_arg_slot(_.node, (i)) + (w) - _.op_fc_base + (int)(d)) \
    : x86("mov", "rcx", bcps_icn_opnd((i), (w), (d)).c_str()) \
)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_icn_block_arm(int is_gen, int off, int act, long idx, int np, int vari, int n2_ftc) {
    extern std::string xa_icn_trace_tap(const char * pname, int kind, int np, int r11d); extern std::string icn_landing_line_restore(void);
    int nargs = (int)_.op_ival, nfix = vari ? np - 1 : np, ncp = nargs < nfix ? nargs : nfix, rest = (vari && nargs > nfix) ? nargs - nfix : 0;
    long K = (is_gen ? 40L : 16L) + 16L * np, pad = is_gen ? 8L : 0L;
    return x86_alpha()
         + x86("note", "the block protocol (ARCH-PROLOG-C-OUT-OF-THE-BOX 1.2, Icon): the wires and entry words first, then the argument block at the callee's entry rsp, "
                       "then a jump through the registry -- no staged medium, no C open, no C epilogue, no NRETURN consult")
         + x86_anchor_enter()
         + IF(is_gen, x86("mov", FRQ(act), (long)DT_RAW)
                    + bcps_icn_lvl(1L)
                    + x86("sub", "rsp", 24L)
                    + x86_rsp_store64_imm(16, 0L)
                    + x86_rsp_store64_imm(8, 0L)
                    + x86_rsp_store64_imm(0, 0L))
         + x86_rsp_mark_save()
         + x86_lea_id("rcx", 4)
         + x86("push", "rcx")
         + x86_lea_id("rcx", 3)
         + x86("push", "rcx")
         + x86("sub", "rsp", 16L * np)
         + FOR(0, ncp, [&](int i) { return bcps_icn_src(i, 0, K)
                                         + x86_rsp_store64(16 * i, "rcx")
                                         + bcps_icn_src(i, 8, K)
                                         + x86_rsp_store64(16 * i + 8, "rcx"); })
         + FOR(ncp, np, [&](int i) { return x86_rsp_store64_imm(16 * i, 0L)
                                          + x86_rsp_store64_imm(16 * i + 8, 0L); })
         + IF(vari, x86("sub", "rsp", 16L * rest + pad)
                  + FOR(0, rest, [&](int k) { return bcps_icn_src(nfix + k, 0, K + 16 * rest + pad)
                                                   + x86_rsp_store64(16 * k, "rcx")
                                                   + bcps_icn_src(nfix + k, 8, K + 16 * rest + pad)
                                                   + x86_rsp_store64(16 * k + 8, "rcx"); })
                  + x86("mov", "rdi", "rsp")
                  + x86("mov32", "esi", (long)rest)
                  + x86("call", "rt_make_list", (uint64_t)(uintptr_t)(void *)rt_make_list)
                  + x86_rt_gc_poll_res()
                  + x86("add", "rsp", 16L * rest + pad)
                  + x86_rsp_store64(16 * (np - 1), "rax")
                  + x86_rsp_store64(16 * (np - 1) + 8, "rdx"))
         + IF(is_gen, xa_icn_trace_tap(_.op_sval, 1, np, 0))
         + IF(is_gen, x86_lea_id("rdx", 4))
         + bcps_jmp_proc_fn(idx)
         + x86("def", L(3))
         + x86_rsp_land(16)
         + x86_gc_site(X86_SITE_LANDING)
         + IF(is_gen && emit_gc_site_rsp_based(), x86_bomb("bcps_icn_block_arm gamma landing, generator arm: a rule-0 caller took this road, its landing delta is 24 bytes wrong"))
         + icn_landing_line_restore()
         + (is_gen ? x86("cmp", "al", (long)DT_FAIL)
                   + x86("je", L(8))
                   + x86("mov", "rdi", RDQ("rdx", 0 - n2_ftc))
                   + x86("mov", "rsi", RDQ("rdx", 8 - n2_ftc))
                   + x86("mov", FRQ(act + 8), "rdx")
                   + x86("jmp", L(9))
                   + x86("def", L(8))
                   + x86("mov32", "edi", (long)DT_FAIL)
                   + x86("mov32", "esi", 0L)
                   + x86("mov", FRQ(act + 8), 0L)
                   + x86("def", L(9))
                   + x86("mov", "rax", FRQ(act))
                   + x86_raw_unpack("rax")
                   + x86("test", "rax", "rax")
                   + x86("jne", L(5))
                   + x86("mov", FRQ(act), (long)(0x100 | DT_RAW))
                   + bcps_icn_lvl(-1L)
                   + x86("mov", "rax", "rdi")
                   + x86("mov", "rdx", "rsi")
                   + x86("jmp", L(2))
                   + x86("def", L(5))
                   + x86("call", "rt_gen_spine_pass_γ", (uint64_t)(uintptr_t)(void *)rt_gen_spine_pass_γ)
                   + x86("jmp", L(2))
                 : bcps_wire_land(_.op_sval)
                   + x86("mov", "rax", "rdi")
                   + x86("mov", "rdx", "rsi")
                   + x86("jmp", L(2)))
         + x86("def", L(4))
         + x86_rsp_land(16)
         + x86_gc_site(X86_SITE_LANDING)
         + icn_landing_line_restore()
         + bcps_wire_land(_.op_sval)
         + (is_gen ? x86("add", "rsp", 8L)
                   + x86("mov", FRQ(act + 8), 0L)
                   + x86("mov", "rax", FRQ(act))
                   + x86_raw_unpack("rax")
                   + x86("test", "rax", "rax")
                   + x86("jne", L(6))
                   + x86("mov", FRQ(act), (long)(0x100 | DT_RAW))
                   + bcps_icn_lvl(-1L)
                   + x86("mov32", "eax", (long)DT_FAIL)
                   + x86("xor", "edx", "edx")
                   + x86("jmp", L(2))
                   + x86("def", L(6))
                   + x86("call", "rt_gen_spine_pass_ω", (uint64_t)(uintptr_t)(void *)rt_gen_spine_pass_ω)
                   + x86("jmp", L(2))
                 : x86("mov32", "eax", (long)DT_FAIL)
                   + x86("xor", "edx", "edx"))
         + x86("def", L(2))
         + x86_anchor_leave()
         + x86("mov", _.op_zres ? ZRES(0) : FRQ(off), "rax")
         + x86("mov", _.op_zres ? ZRES(8) : FRQ(off + 8), "rdx")
         + x86("cmp", "al", (long)DT_FAIL)
         + x86_omega("je")
         + x86_gamma()
         + x86_beta()
         + icn_landing_line_restore()
         + (is_gen ? x86("call", "rt_gen_spine_resume_enter", (uint64_t)(uintptr_t)(void *)rt_gen_spine_resume_enter)
                   + x86("mov", "rax", FRQ(act + 8))
                   + x86_jmp_mem("rax", 32)
                 : bcps_beta_leave(bcps_beta_pair_idx()))
         + RO_SEAL_STR(0, _.op_sval ? _.op_sval : "");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_spine_gen_arm() {
    x86_begin();
    int off = bcps_result_slot(); if (off < 0) return x86_bomb("bb_call_proc_staged: no LOWER slot grant (TMP-ERADICATE)");
    int act = emit_zrow_actoff(_.node); if (act < 0) act = off + 16 * (1 + (int)_.op_ival);
    int gi_dyn = _.op_sval && rt_proc_dyn_scope(_.op_sval);
    long gi_idx = (!bcps_no_genidx() && !gi_dyn && _.op_sval) ? (long)rt_proc_index_of(_.op_sval) : -1L;
    int sprec = emit_zrow_sprec(_.node);
    if (sprec) return x86_alpha() + x86_bomb("bb_call_proc_staged: the plan puts this call's resume record on the spine, but the call does not take the block protocol");
    int n2_fb = -1, cb_np = 0, cb_va = 0, callee_blk = (gi_idx >= 0 && _.op_sval) ? bb_proc_target_icn_block(_.op_sval, &cb_np, &cb_va) : 0;
    if ((icn_gen_regime() || callee_blk) && _.op_sval) n2_fb = _.op_cft;
    if ((icn_gen_regime() || callee_blk) && n2_fb <= 0) return x86_alpha()
        + x86_bomb("N-3: generator call site cannot size the callee's result slot (the callee's frame bytes are not registered: forward reference) -- refusing loudly") + x86_beta()
            + x86_bomb("N-3: beta re-entry into a refused generator call site");
    int n2_ftc = (n2_fb > 0) ? ((n2_fb + 15) & ~15) : 0;
    { int ibnp = 0, ibva = 0; if (gi_idx >= 0 && n2_ftc > 0 && bb_proc_target_icn_block(_.op_sval, &ibnp, &ibva)) return bcps_icn_block_arm(1, off, act, gi_idx, ibnp, ibva, n2_ftc); }
    return x86_alpha()
         + x86_anchor_enter()
         + x86("mov", FRQ(act), 0L)
         + FOR(0, (int)_.op_ival, [&](int i) {
        int slot = bcps_arg_slot(_.node, i);
        return stage_arg_inline(i, slot, TEMPLATE_FN_ADDR(rt_arg_stage));
    })
         + (gi_idx >= 0
            ? x86("mov32", "edi", (long)gi_idx)
            + x86("mov32", "esi", (long)_.op_ival)
            + x86("call", "rt_proc_call_open_det", (uint64_t)TEMPLATE_FN_ADDR(rt_proc_call_open_det))
            : x86("mov", "rdi", ROQ(0))
            + x86("mov32", "esi", (long)_.op_ival)
            + x86("call", "rt_proc_call_open", TEMPLATE_FN_ADDR(rt_proc_call_open)))
         + x86_rt_gc_poll()
         + IF(gi_idx < 0, x86("mov", FRQ(act + 8), "rax"))
         + IF(icn_gen_regime(), x86("sub", "rsp", 8L) + x86_rsp_store64_imm(0, 0)
         + x86("note",
             "N-2 ABI WORD (row icon-generator-call-path-enters-every-runtime-helper-8-bytes-off-the-sysv-abi, hq_I "
                 "root-caused, hq_B authored): the REGION HAND-OFF push below is a LONE 8B word and therefore PARITY-FLIPPING, "
                     "so the armed call site pushed 40 bytes (pad+L7+region+wire pair) where the unarmed one pushes 32. The callee "
                         "body then ran at rsp0-40 = 8 mod 16 and EVERY call it made entered a helper at 0 mod 16 -- latent until some "
                             "callee reached an aligned SSE store, which is why it read as a record bug (suspend a list, nothing; suspend a "
                                 "RECORD and dat_construct -> rt_fire_buildplan_tweak -> snprintf -> movaps -> dead). This word is pushed "
                                     "FIRST, above the pad, ON PURPOSE: every documented entry offset ([rsp+0]=gamma [rsp+8]=omega [rsp+16]="
                                         "REGION [rsp+24]=L7 [rsp+32]=pad) is UNCHANGED, and only the caller pre-pad rsp0 moves from [rsp+40] to [rsp+48] -- "
                                             "one constant in the alpha's ANCHOR lea and one in the beta re-creation. Placing it between L7 and the region "
                                                 "instead would keep the region at +16 and silently move the pad, which is the slot the selfrec depth is read from at [entry rsp+32]."))
         + (x86("note",
             "CFO-36 (cfo 2026-09-09): the landing words (N-2 word, landing cell) are pushed AFTER the prologue call, not "
                 "before it -- in the generator regime they are an ODD count, so rt_proc_call_open_det ran at rsp 8-mod-16 and "
                     "everything it reached did too (rt_trace_event_args -> image -> vsnprintf movaps: SIGSEGV on every traced "
                         "generator call with an argument; Arizona coexpr and errors, rungs rung03). The entry layout the callee sees "
                             "is byte-identical; only the prologue call moved above the words,") + IF(!icn_gen_regime(), x86_sub("rsp", 16)
         + x86("note",
             "LANDING UNIT (cto, ARCH-GC section 13.2): the 16 bytes under the wire pair are a raw carve nothing reads; "
                 "nothing ever jumped to the L(7) they used to hold (plant over 1,800 programs, both modes).")) + IF(icn_gen_regime(),
         x86("note",
             "R-483 (hq_U): NO PAD IN THE GENERATOR REGIME. The pad above is caller-side transient bookkeeping that had "
                 "drifted into the callee ENTRY FRAME as a sixth word, and hq_U FINDING-2026-09-09 measured that NOTHING READS "
                     "IT -- an injected 0x5EEDFACE store into [entry rsp+32] left parse byte-identical while the same store into "
                         "[entry rsp+0] SIGSEGVd, so the experiment had a positive control and the slot is padding. The comment that "
                             "used to sit here named a `selfrec depth` reader at [entry rsp+32]; `selfrec` occurred exactly once in the "
                                 "whole tree -- in that sentence. The 8 bytes are NOT deleted, they MOVE ACROSS THE CALL into the callee`s own "
                                     "carve (emit.cpp: carve gains 8, ANCHOR lea rsp+48 -> rsp+40), so the callee body still lands 0 mod 16. "
                                         "Dropping the pad WITHOUT that move was measured on 2026-09-10 and SIGSEGVs patchu -- the crash is parity, "
                                             "never a lost datum. Entry frame in the generator regime is now FIVE words: [rsp+0]=gamma [rsp+8]="
                                                 "omega [rsp+16]=REGION [rsp+24]=0, the landing word nobody reads [rsp+32]=N-2 ABI word, ANCHOR=[rsp+40]. "
                                                     "rt_genp_spine_enter_n2 (rt.c) is the hand-written twin of this block and was shrunk by the same word in the same landing."))
                                                         + IF(icn_gen_regime(),
         x86("sub", "rsp", 8L) + x86_rsp_store64_imm(0, 0L)))
         + x86("test", "rax", "rax")
         + x86("je", L(1))
         + (gi_idx >= 0 ? std::string("") : x86("mov", "rdi", ROQ(0))
         + x86("call", "rt_proc_fn", TEMPLATE_FN_ADDR(rt_proc_fn)))
         + IF(gi_idx < 0 && bcps_pl(), bcps_pinned_byname_road(bcps_block_build((int)_.op_ival), act + 8, 16 * (int)_.op_ival, 61, 62, 60))
         + IF(icn_gen_regime(), x86("sub", "rsp", 8L) + x86_rsp_store64_imm(0, 0))
         + bcps_wire_cross_gen(3, 4)
         + x86("def", L(3))
         + (bcps_pl()
            ? x86("mov", FRQ(act), "rax")
              + x86("mov", FRQ(act + 8), "rdx")
              + x86("test", "rax", "rax")
              + x86("jne", L(21))
              + x86("add", "rsp", 32L)
              + x86("def", L(21))
              + x86("call", "rt_gen_spine_pass_γ", TEMPLATE_FN_ADDR(rt_gen_spine_pass_γ))
              + x86("jmp", L(2))
            : icn_gen_regime()
            ?
               x86("cmp", "al", (long)DT_FAIL)
              + x86("je", L(8))
              + x86("mov", "rdi", RDQ("rdx", 0 - n2_ftc))
              + x86("mov", "rsi", RDQ("rdx", 8 - n2_ftc))
              + x86("mov", FRQ(act + 8), "rdx")
              + x86("jmp", L(9))
              + x86("def", L(8))
              + x86("mov32", "edi", (long)DT_FAIL)
              + x86("mov32", "esi", 0L)
              + x86("mov", FRQ(act + 8), 0L)
              + x86("def", L(9))
            : bcps_wire_land(_.op_sval)
              + (gi_idx >= 0 ? bcps_epi_det(0, rt_proc_is_generator(_.op_sval)) : x86("mov", "rdx", FRQ(act + 8))
              + x86("shr", "rdx", 61L)
              + x86("and", "edx", 1L))
              + (x86("mov", FRQ(act + 8), "rsp")
                 + x86("add", "rsp", 16L)))
         + IF(!bcps_pl(),
           x86("mov", "rax", FRQ(act))
         + x86("test", "rax", "rax")
         + x86("jne", L(5))
         + x86("mov", FRQ(act), 1L)
         + x86("call", "rt_proc_call_epilogue_γ", TEMPLATE_FN_ADDR(rt_proc_call_epilogue_γ))
         + x86("jmp", L(2))
         + x86("def", L(5))
         + x86("call", "rt_gen_spine_pass_γ", TEMPLATE_FN_ADDR(rt_gen_spine_pass_γ))
         + x86("jmp", L(2)))
         + x86("def", L(4))
         + (bcps_pl()
            ? x86("add", "rsp", 32L)
              + x86("mov", FRQ(act), 0L)
              + x86("call", "rt_gen_spine_pass_ω", TEMPLATE_FN_ADDR(rt_gen_spine_pass_ω))
              + x86("jmp", L(2))
            : bcps_wire_land(_.op_sval)
         + (gi_idx >= 0 ? bcps_epi_det(1, rt_proc_is_generator(_.op_sval)) : x86("mov", "rdi", FRQ(act + 8))
         + x86("shr", "rdi", 61L)
         + x86("and", "edi", 1L))
         + x86("add", "rsp", icn_gen_regime() ? 8L : 16L)
         + IF(icn_gen_regime(), x86("mov", FRQ(act + 8), 0L))
         + x86("mov", "rax", FRQ(act))
         + x86("test", "rax", "rax")
         + x86("jne", L(6))
         + x86("mov", FRQ(act), 1L)
         + x86("call", "rt_proc_call_epilogue_ω", TEMPLATE_FN_ADDR(rt_proc_call_epilogue_ω))
         + x86("jmp", L(2))
         + x86("def", L(6))
         + x86("call", "rt_gen_spine_pass_ω", TEMPLATE_FN_ADDR(rt_gen_spine_pass_ω))
         + x86("jmp", L(2)))
         + x86("def", L(1))
         + bcps_undef_fallback(TEMPLATE_FN_ADDR(rt_ab_undef_fn_stub))
         + x86("def", L(2))
         + x86_anchor_leave()
         + bcps_nret_consult(std::string(FRQ(off)), std::string(FRQ(off + 8)))
         + x86("mov", FRQ(off), "rax")
         + x86("mov", FRQ(off + 8), "rdx")
         + x86("cmp", "al", (long)DT_FAIL)
         + x86_omega("je")
         + x86_gamma()
         + x86_beta()
         + IF(!bcps_pl(), x86("call", "rt_gen_spine_resume_enter", TEMPLATE_FN_ADDR(rt_gen_spine_resume_enter)))
         + ((bcps_pl()
               ? x86_pl_ball_rule(30, 22)
                 + x86("mov", "rax", FRQ(act))
                 + x86("test", "rax", "rax")
                 + x86("je", L(22))
                 + x86("mov", "rcx", FRQ(act + 8))
                 + x86("mov", x86_hb(), "rax")
                 + x86("call", "rt_gen_spine_resume_enter", TEMPLATE_FN_ADDR(rt_gen_spine_resume_enter))
                 + x86_jmp_reg("rcx")
                 + x86("def", L(22))
                 + x86_omega()
               : icn_gen_regime()
               ? x86("mov", "rax", FRQ(act + 8))
                 + x86_jmp_mem("rax", 32)
               : x86("mov", "rax", FRQ(act + 8))
                 + x86_jmp_mem("rax", 0)))
         + RO_SEAL_STR(0, _.op_sval ? _.op_sval : "");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_proc_staged() {
    int is_gen = _.op_sval && rt_proc_is_generator(_.op_sval);
    if (is_gen && _.op_node_kind != (int)IR_PROC_GEN && _.op_node_kind != (int)IR_CALL_PROC_STAGED) return x86_alpha()
        + x86_bomb("bb_call_proc_staged: generator call on an op kind without a callgen.act RSP-carve handle grant (zeta_storage.c widens only IR_PROC_GEN / IR_CALL_PROC_STAGED)");
    if (is_gen) return bcps_spine_gen_arm();
    return bcps_det_arm();
}
static std::string bcps_det_block_arm(int zres, int off, int bidx, long idx, int nb, int wires) {
    int n = (int)_.op_ival; long bias = (wires ? 16L : 0L) + 16L * nb;
    auto zop = [&](int i, int w) -> std::string { int in = i >= 0 && i < _.op_zcap; if (in && _.op_zread_xf[i] != -1) return RDQ("rbp", _.op_zread_xf[i]
        + w); return x86_zref((in ? _.op_zread[i] : 0) + w + (int)bias, 1); };
    std::string s = x86_alpha() + x86_anchor_enter() + x86_rsp_mark_save()
         + x86("note",
             "the block protocol for a deterministic Pascal callee (R-1491): the two wire cells, then the argument block "
                 "on this spine, then a jump; no staged medium, no registry call, no C prologue or epilogue")
         + IF(wires, x86_lea_id("rcx", 4) + x86("push", "rcx"))
         + IF(wires, x86_lea_id("rcx", 3) + x86("push", "rcx"))
         + x86("sub", "rsp", (long)(16 * nb))
         + FOR(n, nb, [&](int i) { return x86_rsp_store64_imm(16 * i, (long)DT_SNUL) + x86_rsp_store64_imm(16 * i + 8, 0L); })
         + FOR(0, n, [&](int i) {
               if (zres) return x86("note", ZOPN(i))
                              + x86("mov", "rax", zop(i, 0)) + x86_rsp_store64(16 * i, "rax")
                              + x86("note", ZOPN(i))
                              + x86("mov", "rax", zop(i, 8)) + x86_rsp_store64(16 * i + 8, "rax");
               int slot = bcps_arg_slot(_.node, i);
               return x86("note", std::string("block A") + std::to_string(i))
                    + x86("mov", "rax", FRQB(slot, (int)bias)) + x86_rsp_store64(16 * i, "rax")
                    + x86("mov", "rax", FRQB(slot + 8, (int)bias)) + x86_rsp_store64(16 * i + 8, "rax"); })
         + x86_lea_id("rcx", 3) + x86_lea_id("rdx", 4)
         + bcps_jmp_proc_fn(idx)
         + x86("def", L(3))
         + x86_rsp_land(0)
         + x86_gc_site(X86_SITE_LANDING)
         + x86("mov", "rax", "rdi")
         + x86("mov", "rdx", "rsi")
         + x86("jmp", L(2))
         + x86("def", L(4))
         + x86_rsp_land(0)
         + x86_gc_site(X86_SITE_LANDING)
         + x86("mov32", "eax", (long)DT_FAIL)
         + x86("xor", "edx", "edx")
         + x86("def", L(2))
         + x86_anchor_leave()
         + (zres ? x86("note", ZRESN())
         + x86("mov", ZRES(0), "rax")
         + x86("note", ZRESN())
         + x86("mov", ZRES(8), "rdx")
                 : x86("mov", FRQ(off), "rax")
                 + x86("mov", FRQ(off + 8), "rdx"))
         + x86("cmp", "al", (long)DT_FAIL)
         + x86_omega("je")
         + x86_gamma()
         + x86_beta()
         + bcps_beta_leave(bidx)
         + RO_SEAL_STR(0, _.op_sval ? _.op_sval : "");
    return s;
}
