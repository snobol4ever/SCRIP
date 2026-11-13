#include "emitter/em/em_head.h"
#include "emitter/emit_state.h"
#include "emitter/emit_regime.h"
#include "ir/bb_box.h"
#include "emitter/bb_label.h"
#include "ir/gc_frame_map.h"
#include "ir/dtp.h"
#include "driver/scrip_sm.h"
#include "ir/stage2.h"
#include "templates/xa/xa_templates.h"
#include "planner/g_planner.h"
#include "ir/ir_fwd.h"
#include "ir/ct_vec.h"
#include "ir/descr.h"
#include "ir/ast.h"
#include "runtime/core/g_runtime_entry.h"
#include "runtime/core/g_runtime_vec.h"
#include "runtime/core/g_runtime_abi.h"
#include "emitter/emit_walk.h"
#include "emitter/emit_str.h"
#include "lower/lower_prolog.h"
#include "ir/ir_node_index.h"
#include "templates/bb/bb_templates.h"
extern "C" {
#include "templates/x86/x86_frame.h"
}
extern "C" {
#include "templates/x86/x86_gc.h"
}
extern "C" {
#include "templates/x86/x86_icn.h"
}
extern "C" {
#include "templates/x86/x86_zverify.h"
}
extern "C" {
#include "planner/zeta_plan.h"
}
extern "C" {
#include "lower/lower_common.h"
}
#include "emitter/em/em_01_sh.h"
#include "emitter/em/em_02_sh.h"
#include "emitter/em/em_03_sh.h"
#include "emitter/em/em_04_sh.h"
#include "emitter/em/em_05_sh.h"
#include "emitter/em/em_06_sh.h"
#include "emitter/em/em_07_sh.h"
extern "C" { extern int is_global(const char *name); }
extern "C" {
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string icn_main_args_inline(int np, int blk) {
    if (blk || np <= 0) return std::string();
    if (np > 10) return x86_bomb("emit: an Icon root graph with more than ten parameters has no inline argument load");
    std::string s =
        x86("comment", "the root graph keeps its parameters in its own frame: g_call_args[i] -> [rsp+16+16i] inline while i < cap (the frame was zeroed, so a short medium leaves a null DESCR)")
                  + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_call_args, "g_call_args")
                  + x86("mov", "ecx", RDD("rax", (int)offsetof(gv_t, cap)))
                  + x86("mov", "rax", RDQ("rax", (int)offsetof(gv_t, p)));
    for (int i = 0; i < np; i++)
        s += x86("cmp", "ecx", (long)i) + x86("jbe", L(220 + i))
           + x86("mov", "rdx", RDQ("rax", 16 * i)) + x86("mov", RDQ("rsp", 16 + 16 * i), "rdx")
           + x86("mov", "rdx", RDQ("rax", 16 * i + 8)) + x86("mov", RDQ("rsp", 24 + 16 * i), "rdx")
           + x86("def", L(220 + i));
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int codegen_flat_chain_body(zeta_plan_t * _pl, const char *prefix) {
    bb_label_t lbl_α, lbl_α_body, lbl_γ, lbl_ω, lbl_β, lbl_res;
    bb_label_t * lbl_α_orig_p = (bb_label_t *)0;
    int _top_hoist = 0;
    int _flt_hoisted[4] = {0, 0, 0, 0};
    int _flt_uid_burn[4] = {0, 0, 0, 0};
    const char *fam = (strncmp(prefix, "proc_", 5) == 0) ? prefix + 5 : prefix;
    g_emit.flat_fam = fam;
    g_emitter.g_gc_sites_v.len = 0; g_emitter.g_gc_sites_armed = 1; g_emitter.g_gc_sites_last_off = -1; g_emitter.g_gc_sites_offs_v.len = 0; g_emitter.g_gc_match_n = 0; emit_gc_rsp_reset();
    emit_label_initf(&g_emit.gc_code_lbl, ".Lgccode_%s_%d", fam ? fam : "chain", g_emit.gc_code_seq++);
    if (g_is_text) { std::string _cl = std::string(g_emit.gc_code_lbl.name) + ":\n"; emit_text_n(_cl.data(), _cl.size()); } else emit_label_define_bb(&g_emit.gc_code_lbl);
    g_emit.op_zdepth = 0; g_emit.op_zslid = 0; g_emit.op_fc_bytes = 0;
    if (gc_sites_report_on()) fprintf(stderr, "[GC-GRAPH] fam=%s prefix=%s buf=%p text=%d\n", fam ? fam : "?", prefix ? prefix : "?", (const void *)bb_emit_buf, g_is_text);
    g_emitter.g_flt_fam = (strncmp(prefix, "proc_", 5) == 0) ? fam : (const char *)0; g_emitter.g_flt_lbl[1] = g_emitter.g_flt_lbl[2] = g_emitter.g_flt_lbl[3] = (bb_label_t *)0;
    g_emit.x86_uid_kind = (const char *)0;
    emit_label_initf(&lbl_α, "%s_α", fam);
    emit_label_initf(&lbl_α_body, "%s_α_body", fam);
    emit_label_initf(&lbl_γ, "%s_γ", fam);
    emit_label_initf(&lbl_ω, "%s_ω", fam);
    emit_label_initf(&lbl_β, "%s_β", fam);
    emit_label_initf(&lbl_res, "%s_res", fam);
       bb_label_t * alt_step_lbl = (bb_label_t *)0; int n_alt = 0;
    struct IxP { bb_label_t * sw; int slot, n, nset, fbr, fba, fbi; int * tag; int * code; int64_t * val; int * sets; int * set_beg; int * set_len; };
    bb_label_t * ix_switch = (bb_label_t *)0; IxP * ixp = (IxP *)0; int ix_np = 0;
    g_emit.flat_alt1_p = (bb_label_t *)0; g_emit.flat_altdet_p = (bb_label_t *)0;
    if (g_emit_cfg && g_emit_cfg->n_alts > 0) {
        n_alt = g_emit_cfg->n_alts;
        if (g_emitter.codegen_flat_chain_body_alt_tr_cap < n_alt) { g_emitter.codegen_flat_chain_body_alt_tr =
            (bb_label_t **)ct_grow(g_emitter.codegen_flat_chain_body_alt_tr, (size_t)n_alt * sizeof(bb_label_t *));
                                  g_emitter.codegen_flat_chain_body_ret_tr = (bb_label_t **)ct_grow(g_emitter.codegen_flat_chain_body_ret_tr, (size_t)n_alt * sizeof(bb_label_t *));
                                      g_emitter.codegen_flat_chain_body_alt_tr_cap = n_alt; }
        for (int _ak = 0; _ak < n_alt; _ak++) g_emitter.codegen_flat_chain_body_alt_tr[_ak] = emit_label_alloc("%s_alt%d", fam, _ak);
        for (int _ak = 0; _ak < n_alt; _ak++) g_emitter.codegen_flat_chain_body_ret_tr[_ak] = emit_label_alloc("%s_ret%d", fam, _ak);
        alt_step_lbl = emit_label_alloc("%s_step", fam);
        g_emit.flat_altdet_p = emit_label_alloc("%s_altdet", fam);
        if (n_alt > 1) g_emit.flat_alt1_p = g_emitter.codegen_flat_chain_body_alt_tr[1];
    }
    int bare = g_emit.flat_bare_chain;
    if (strncmp(prefix, "proc_LBL__", 10) == 0) emit_label_initf(&lbl_α, "%s", prefix + 5);
    else if (strncmp(prefix, "proc_", 5) == 0 && !bare) {
        { extern int bb_ab_sym_is_thunk_stem(const char *); if (bb_ab_sym_is_thunk_stem(prefix + 5)) emit_label_initf(&lbl_α, "%s", prefix + 5); else emit_label_initf(&lbl_α, "FN__%s", prefix + 5);
            }
        lbl_α_orig_p = emit_label_alloc("%s_α", prefix + 5);
    }
    int fn_face_dead = bare && strncmp(prefix, "proc_", 5) == 0 && strncmp(prefix, "proc_LBL__", 10) != 0;
    if (bare) { emit_label_initf(&lbl_γ, "main_\xcf\x89"); emit_label_initf(&lbl_ω, "main_\xcf\x89"); emit_label_initf(&lbl_β, "main_\xcf\x89"); emit_label_initf(&lbl_res, "main_\xcf\x89"); }
    int text_externalise = g_is_text ? 1 : 0;
    if (text_externalise) data_buf_reset();
    g_emit.flat_lbl_α = lbl_α.name;
    g_emit.flat_lbl_α_body = lbl_α_body.name;
    g_emit.flat_lbl_γ = lbl_γ.name;
    g_emit.flat_lbl_ω = lbl_ω.name;
    g_emit.flat_lbl_β = lbl_β.name;
    g_emit.flat_β_p = &lbl_β;
    g_emit.flat_res_p = &lbl_res;
    g_emit.flat_succ_p = &lbl_γ;
    g_emit.flat_fail_p = &lbl_ω;
    g_emit.flat_text_externalise = text_externalise;
    g_resumable_callable_active = (g_emit_cfg && g_emit_cfg->resumable_callable) ? 1 : 0;
    nidx_reset();
    IR_t * entry = _pl->entry; int n = _pl->n; int entry_is_own_graph_root = _pl->own_root; int flat_empty_body_fail = _pl->empty_fail,
        flat_empty_body_succ = _pl->empty_succ;
    unsigned char * zd_on = _pl->on; int * zd_out = _pl->out; int * zd_gp = _pl->gp; int * zd_wp = _pl->wp; int * zd_arm = _pl->arm; int * zd_cps = _pl->cps; int * zd_cpd = _pl->cpd;
        int * zd_chn = _pl->chn;
    int * zd_claim = _pl->claim; int * zd_vl = _pl->vl; int * zd_xb = _pl->xb; int * zd_cut = _pl->cut; (void)entry_is_own_graph_root; (void)flat_empty_body_succ; (void)zd_vl; (void)zd_chn;
        int _sph = 0, _spgd = 0, _spf = 0, _wpn = 0;
        bb_label_t ** _wpl = (bb_label_t **)alloca(sizeof(bb_label_t *) * (size_t)(n + 1)); int * _wpd = (int *)alloca(sizeof(int) * (size_t)(n + 1));
        bb_label_t ** _tpl = (bb_label_t **)alloca(sizeof(bb_label_t *) * (size_t)(n + 1)); bb_label_t ** _tpt = (bb_label_t **)alloca(sizeof(bb_label_t *) * (size_t)(n + 1));
        int * _tpd = (int *)alloca(sizeof(int) * (size_t)(n + 1)); int _tpn = 0;
    for (int i = 0; i < n; i++) if (_pl->zn[i].loff >= 0) bb_slot_register(g_planner.codegen_flat_chain_body_nodes[i], _pl->zn[i].loff);
    zd_check(g_planner.codegen_flat_chain_body_nodes, n, zd_on, zd_out, zd_gp, zd_wp, zd_claim, zd_xb, zd_chn, zd_cut, zd_arm, prefix);
    {
    if (g_emitter.codegen_flat_chain_body__zk0_on < 0) { const char * _e = getenv("SCRIP_ZD_CENSUS"); g_emitter.codegen_flat_chain_body__zk0_on = (_e && *_e == '1') ? 1 : 0; }
    if (g_emitter.codegen_flat_chain_body__zk0_on && g_emit_cfg && g_emit_cfg->spine_cells) {
        long _k_total = 0; int _armed = 0; int _unpriced_runs = 0;
        int _blocker_op[IR_OP_COUNT]; int _blocker_cnt[IR_OP_COUNT]; int _n_blockers = 0;
        int _globals_on_stack = 0;
        { extern int is_global(const char *); for (int _i = 0; _i < n; _i++) { if (zd_on[_i]) { _k_total += _pl->zn[_i].k; _armed++;
            int _op5 = (int)g_planner.codegen_flat_chain_body_nodes[_i]->op;
            if ((_op5 == IR_VAR || _op5 == IR_ASSIGN) && IR_LIT(g_planner.codegen_flat_chain_body_nodes[_i]).sval && is_global(IR_LIT(g_planner.codegen_flat_chain_body_nodes[_i]).sval) &&
                !graph_has_local(g_emit_cfg, IR_LIT(g_planner.codegen_flat_chain_body_nodes[_i]).sval)) _globals_on_stack++; } } }
        for (int _i = 0; _i < n; _i++) {
            if (zd_on[_i]) continue;
            {
                int _op = (int)g_planner.codegen_flat_chain_body_nodes[_i]->op; int _found = 0;
                for (int _b = 0; _b < _n_blockers; _b++) if (_blocker_op[_b] == _op) { _blocker_cnt[_b]++; _found = 1; break; }
                if (!_found && _n_blockers < IR_OP_COUNT) { _blocker_op[_n_blockers] = _op; _blocker_cnt[_n_blockers++] = 1; }
                _unpriced_runs++;
            }
        }
        int _bss_bytes = g_emit_cfg->jcon_value_region;
        fprintf(stderr, "[ZK-CENSUS] graph=%s icn_cells=1 n=%d armed=%d K_total=%ld refused_nodes=%d bss_arena=%d globals_on_stack=%d\n",
                prefix ? prefix : "?", n, _armed, _k_total, _unpriced_runs, _bss_bytes, _globals_on_stack);
        for (int _b = 0; _b < _n_blockers && _b < 8; _b++) {
            const char * _nm = bb_op_name((IR_e)_blocker_op[_b]);
            fprintf(stderr, "[ZK-CENSUS-BLOCKER]   %-30s count=%d\n", _nm ? _nm : "<unknown>", _blocker_cnt[_b]);
        }
    } }
    {
    if (g_emitter.codegen_flat_chain_body__plzk0_on < 0) { const char * _e = getenv("SCRIP_ZD_CENSUS"); g_emitter.codegen_flat_chain_body__plzk0_on = (_e && *_e == '1') ? 1 : 0; }
    if (g_emitter.codegen_flat_chain_body__plzk0_on) {
        long _k_total = 0; int _armed = 0; int _refused_nodes = 0;
        int _blocker_op[IR_OP_COUNT]; int _blocker_cnt[IR_OP_COUNT]; int _n_blockers = 0;
        for (int _i = 0; _i < n; _i++) {
            if (zd_on[_i]) { _k_total += _pl->zn[_i].k; _armed++; }
        }
        for (int _i = 0; _i < n; _i++) {
            if (zd_on[_i]) continue;
            {
                int _op = (int)g_planner.codegen_flat_chain_body_nodes[_i]->op; int _found = 0;
                for (int _b = 0; _b < _n_blockers; _b++) if (_blocker_op[_b] == _op) { _blocker_cnt[_b]++; _found = 1; break; }
                if (!_found && _n_blockers < IR_OP_COUNT) { _blocker_op[_n_blockers] = _op; _blocker_cnt[_n_blockers++] = 1; }
                _refused_nodes++;
            }
        }
        int _bss_bytes = g_emit_cfg->jcon_value_region;
        fprintf(stderr, "[PL-ZK-CENSUS] graph=%s pl_cells=1 n=%d armed=%d K_total=%ld refused_nodes=%d bss_arena=%d\n",
                prefix ? prefix : "?", n, _armed, _k_total, _refused_nodes, _bss_bytes);
        for (int _b = 0; _b < _n_blockers && _b < 8; _b++) {
            const char * _nm = bb_op_name((IR_e)_blocker_op[_b]);
            fprintf(stderr, "[PL-ZK-CENSUS-BLOCKER]   %-30s count=%d\n", _nm ? _nm : "<unknown>", _blocker_cnt[_b]);
        }
    } }
    struct zw5_stmt_s { int idx, cnt, base, stno; }; cv_t zw5_dv = { 0, 0, 0, 0 }; cv_t zw5_sv = { 0, 0, 0, 0 };
    bb_label_t * zw5_pool = (bb_label_t *)0; int zw5_base = 0;
    int * zw5_depth = (int *)0; struct zw5_stmt_s * zw5_stmt = (struct zw5_stmt_s *)0; int zw5_pool_stmts = 0;
     if (g_emitter.codegen_flat_chain_body__zw5_on < 0) { const char *_e = getenv("SCRIP_ZW5"); g_emitter.codegen_flat_chain_body__zw5_on = (_e && *_e == '1') ? 1 : 0; }
    if (g_emitter.codegen_flat_chain_body__zw5_on) { for (int _i = 0; _i < n; _i++) { if (g_planner.codegen_flat_chain_body_nodes[_i]->op != IR_STATEMENT || !zd_on[_i]) continue;
        int _stno = (int)IR_LIT(g_planner.codegen_flat_chain_body_nodes[_i]).ival; int _ndepths = 0;
        for (int _j = _i - 1; _j >= 0; _j--) { if (zd_on[_j] && zd_wp[_j] > 0) { int _w = zd_wp[_j]; int _dup = 0;
        for (int _d = 0; _d < _ndepths; _d++) if (CV_AT(zw5_dv, int, zw5_base + _d) == _w) { _dup = 1; break;
        } if (!_dup) { cv_reserve(&zw5_dv, (uint32_t)sizeof(int), (uint64_t)zw5_base + _ndepths + 1, "zw5_depth"); CV_AT(zw5_dv, int, zw5_base + _ndepths) = _w; _ndepths++;
        } } if (g_planner.codegen_flat_chain_body_nodes[_j]->op == IR_STATEMENT) break; }
        if (_ndepths == 0) continue;
        cv_reserve(&zw5_sv, (uint32_t)sizeof(struct zw5_stmt_s), (uint64_t)zw5_pool_stmts + 1, "zw5_stmt"); CV_AT(zw5_sv, struct zw5_stmt_s, zw5_pool_stmts) = zw5_stmt_s{ _i, _ndepths, zw5_base,
            _stno }; zw5_pool_stmts++;
        zw5_base += _ndepths; }
      zw5_depth = (int *)zw5_dv.p; zw5_stmt = (struct zw5_stmt_s *)zw5_sv.p; zw5_pool = (bb_label_t *)ct_zalloc((size_t)(zw5_base > 0 ? zw5_base : 1), sizeof(bb_label_t));
          for (int _s = 0; _s < zw5_pool_stmts; _s++) for (int _d = 0; _d < zw5_stmt[_s].cnt; _d++)
          emit_label_initf(&zw5_pool[zw5_stmt[_s].base + _d], "%s_zw5s%d_ω_d%d", fam, zw5_stmt[_s].stno, zw5_depth[zw5_stmt[_s].base + _d]); }
    { g_emit.flat_all_zd = 0; { int _gen_ok = !g_emit.flat_gen; int _jmp_ok = !g_emit.flat_jmp_entry; if (!g_emit.flat_pat && _gen_ok && _jmp_ok && n > 0) { int _azd = 1;
        for (int _i = 0; _i < n; _i++) if (!zd_on[_i]) { _azd = 0; break; } g_emit.flat_all_zd = _azd;
            } } { if (g_emitter.codegen_flat_chain_body__lpd < 0) { const char * _e = getenv("SCRIP_LP_DIAG");
        g_emitter.codegen_flat_chain_body__lpd = (_e && *_e == '1') ? 1 : 0; } if (g_emitter.codegen_flat_chain_body__lpd) { int _arm = 0; for (int _i = 0; _i < n; _i++) if (zd_on[_i]) _arm++;
        fprintf(stderr, "[LP] prefix=%s n=%d armed=%d all_zd=%d region=%d jmp=%d pat=%d gen=%d\n", prefix, n, _arm, g_emit.flat_all_zd, (g_emit_cfg ? g_emit_cfg->jcon_value_region : -1),
        g_emit.flat_jmp_entry, g_emit.flat_pat, g_emit.flat_gen); } } } { extern void emit_fb_divergence_check(void); emit_fb_divergence_check(); }
    { if (g_emitter.codegen_flat_chain_body__tp < 0) { const char * e = getenv("SCRIP_TOP_PORTS"); g_emitter.codegen_flat_chain_body__tp = (e && *e == '1') ? 1 : 0; }
      int _cls_o =
          (!bare && !g_emit.flat_jmp_entry && !g_emit.zframe_graph && !g_emit.flat_lcl_proc && !g_emit.flat_pat && !g_emit.flat_gen && !g_gen_proc_active &&
          !(g_emit_cfg && g_emit_cfg->cell_frame) && !(g_emit_cfg && g_emit_cfg->body_root) && g_emit.flat_outer_nparams == 0) ? 1 : 0;
      if (_cls_o) for (int _i = 0; _i < n; _i++) if (g_planner.codegen_flat_chain_body_nodes[_i]->op == IR_SUSPEND) { _cls_o = 0; break; }
      _top_hoist = (g_emitter.codegen_flat_chain_body__tp && _cls_o) ? 1 : 0;
      if (_top_hoist) {
          g_emit.flat_main_body_p = &lbl_α_body; g_emit.flat_main_ω_p = &lbl_ω;
          bb_emit_x86(bb_main_entry_bridge());
          emit_sep_rule('-'); emit_label_define_bb(&lbl_β); bb_emit_x86(bb_main_β());
          emit_sep_rule('-'); emit_label_define_bb(&lbl_γ); bb_emit_x86(bb_glue_outer_γ());
          emit_sep_rule('-'); emit_label_define_bb(&lbl_ω); bb_emit_x86(bb_glue_outer_ω());
          for (int _k = 1; _k <= 3; _k++) { int _has = 0; for (int _i = 0; _i < n; _i++) if (ir_floater_kind(g_planner.codegen_flat_chain_body_nodes[_i]) == _k) { _has = 1; break;
              } if (!_has) continue; _flt_hoisted[_k] = 1;
              bb_label_t * _fa = emit_floater_label(_k); bb_label_t * _fg = emit_floater_label(1);
              g_emit.lbl_α_p = _fa; g_emit.lbl_α = _fa->name; g_emit.lbl_γ_p = _fg; g_emit.lbl_γ = _fg->name; g_emit.lbl_ω_p = &lbl_ω; g_emit.lbl_ω = lbl_ω.name; g_emit.lbl_β_p = _fa;
                  g_emit.lbl_β = _fa->name;
              g_emit.op_ival = _k; g_emit.op_define_role = (_k == 3) ? 0 : _k;
              sig_prepare_floater(); int _uid0 = g_flat_node_id; emit_sep_rule('-'); bb_emit_x86(bb_main_floater(_k)); _flt_uid_burn[_k] = g_flat_node_id - _uid0; g_flat_node_id = _uid0; }
          g_emit.flat_main_body_p = (bb_label_t *)0; g_emit.flat_main_ω_p = (bb_label_t *)0;
      } }
    if (text_externalise && g_is_text && !fn_face_dead) emit_label_define_bb(&lbl_α);
    else if (g_emit.zframe_graph || (g_emit_cfg && g_emit_cfg->cell_frame && g_emit.flat_lcl_proc)) emit_label_define_bb(&lbl_α);
    if (g_is_text) g_emit_pos += 7;
    g_emit.flat_carve_total = 0; g_emit.flat_gen_rbp = 0; g_emit.flat_gen_above = 0; g_emit.op_zxb = 0; g_emit.op_wk_off = -1;
    bb_label_t lbl_αblk; emit_label_initf(&lbl_αblk, "%s_αblk", fam);
    int _blk_graph = (g_emit.zframe_graph && !emit_resumable_frame() && g_emit_cfg && g_emit_cfg->pf.block_args && !g_emit_cfg->pf.det_block) ? 1 : 0;
    if (_blk_graph) { extern void xa_flat_block_staged_entry(void); xa_flat_block_staged_entry(); emit_label_define_bb(&lbl_αblk); }
    if (prefix && !strcmp(gc_map_entry_fam(prefix), "main") && !g_runtime.eval.rt_fragment_emit && g_emit_cfg && !g_emit.zframe_graph && !(icn_gen_regime() && g_emit.flat_gen) &&
        !g_emit.flat_lcl_proc &&
        !g_emit.flat_pat) { int _rg = g_emit_cfg->jcon_value_region;
        if (_rg >= 0 && !(_rg & 15)) { bb_emit_x86(emit_gc_map_register(_rg, _rg + 16, 0, GC_FRAME_MAP_ROOT | (g_emit_cfg->root_graph ? GC_FRAME_MAP_ROOT : 0u), 1, 0u)); } }
    g_emit.flat_pkt = (g_emit.zframe_graph && g_emit_cfg && g_emit_cfg->pkt_fragment) ? 1 : 0; g_emit.flat_pkt_cell = g_emit.flat_pkt ? g_emit_cfg->pkt_cell : -1;
        g_emit.flat_pkt_slot = g_emit.flat_pkt ? g_emit_cfg->pkt_slot : -1; g_emit.flat_pkt_chain_off = -1;
    g_emit.flat_pkt_walk_p = (bb_label_t *)0; g_emit.flat_pkt_chainω_p = (bb_label_t *)0;
    if (g_emit.zframe_graph) {
        if (g_emit.flat_pkt) {
            g_emit.flat_pkt_walk_p = emit_label_alloc("%s_pkwalk", fam); g_emit.flat_pkt_chainω_p = emit_label_alloc("%s_pkchω", fam);
            { extern void xa_flat_pkt_fresh_entry(void); xa_flat_pkt_fresh_entry(); }
            { bb_label_t lbl_pc; emit_label_initf(&lbl_pc, "%s_pkα", fam); emit_sep_rule('-'); emit_label_define_bb(&lbl_pc); if (!g_is_text) g_emit.flat_pkt_chain_off = (long)lbl_pc.offset; } }
        { extern void xa_flat_zframe_prologue(void); xa_flat_zframe_prologue(); }
    } else if (icn_gen_regime() && g_emit.flat_gen) {
        int kt2 = g_emit.flat_frame_bytes;
        int np = g_emit_cfg ? g_emit_cfg->nparams : 0;
        int nl = g_emit_cfg ? g_emit_cfg->nlocals : 0;
        int frame_total = g_emit_cfg ? g_emit_cfg->pf.flat_gen_rbp : kt2 + (np + nl) * 16;
        g_emit.flat_carve_total = frame_total; g_emit.flat_gen_rbp = frame_total;
        extern void rt_lcl_proc_args_install(void *, int, int);
        extern void rt_icn_zframe_args_install(void *, int, int);
        int _use_zframe_install = (g_emit_cfg && g_emit_cfg->cell_frame) ? 1 : 0;
        int carve = ((frame_total + 15) & ~15) + 48 + 8 + 16;
        std::string _gseed;
        {
           if (g_emitter.codegen_flat_chain_body__en < 0) { const char * _e = getenv("SCRIP_LCL_SEED"); g_emitter.codegen_flat_chain_body__en = (_e && *_e == '0') ? 0 : 1; }
          int _lo = (g_emitter.codegen_flat_chain_body__en && g_emit_cfg) ? g_emit_cfg->pf.locals : -1;
          int _rg = (g_emitter.codegen_flat_chain_body__en && g_emit_cfg) ? g_emit_cfg->pf.value_region : -1;
          if (_lo >= 0 && _rg > 0) _lo = 0;
          if (_lo >= 0 && _rg > _lo && _rg <= frame_total)
              _gseed = flat_zero_fill(_lo, _rg - _lo);
          if (_use_zframe_install && _gseed.empty() && _rg > 0 && _rg <= frame_total)
              _gseed = x86("comment", "the block protocol: no C install runs, so the value region is zeroed here -- every local reads a null DESCR")
                     + flat_zero_fill(0, _rg);
          }
        extern int zls_g_block_args(const IR_graph_t *); extern int zls_g_block_base(const IR_graph_t *);
        int _gblk = (_use_zframe_install && g_emit_cfg->pf.block_args) ? 1 : 0, _gw = _gblk ? 16 * np : 0;
        g_emit.flat_gen_above = carve - frame_total + _gw + 40;
        if (_gblk && g_emit_cfg->pf.block_base != carve)
            { fprintf(stderr, "FATAL emit: generator %s's argument block lies at +%d by the layout and +%d by the carve -- the two must agree to the byte\n", prefix ? prefix : "?",
            g_emit_cfg->pf.block_base, carve); abort(); }
        bb_emit_x86( IF(_gblk,
            x86("comment",
            "the block protocol: the caller's argument block is at this entry rsp, A[i] = [entry+16i]; the five entry words lie above it, so gamma, omega and the anchor are read 16n higher"))
                  + x86("lea", "rax", RDQ("rsp", 0 - carve))
                  + x86("mov", RDQ("rax", frame_total + 0), "rbp")
                  + x86("mov", "rcx", RDQ("rsp", _gw + 0)) + x86("mov", RDQ("rax", frame_total + 8), "rcx")
                  + x86("mov", "rcx", RDQ("rsp", _gw + 8)) + x86("mov", RDQ("rax", frame_total + 16), "rcx")
                  + x86("lea", "rcx", RDQ("rsp", _gw + 40)) + x86("mov", RDQ("rax", frame_total + 24), "rcx")
                  + IF(x86_trace_taps_on(), x86("mov", RDQ("rax", frame_total + 64), 0L))
                  + x86("lea", "rbp", RDQ("rax", frame_total))
                  + x86("mov", "rsp", "rax")
                  +
                      emit_gc_map_register(g_emit_cfg ? g_emit_cfg->jcon_value_region : 0, frame_total, frame_total - (g_emit_cfg ? g_emit_cfg->jcon_value_region : 0) - 16 + 32,
                      GC_FRAME_MAP_GEN_ANCHOR | GC_FRAME_MAP_ICN_PROC | (_gblk ? GC_FRAME_MAP_ICN_BLOCK : 0u) | (g_emit_cfg && g_emit_cfg->root_graph ? GC_FRAME_MAP_ROOT : 0u), 0,
                      GC_LINK_Q(frame_total + 8, frame_total, frame_total + 24, GC_LINK_R_IND))
                  + _gseed
                  + IF(_gblk, icn_entry_gva())
                  + IF(!_gblk, x86("mov", "rdi", "rsp") + x86("mov32", "esi", (long)np) + x86("mov32", "edx", (long)nl)
                  + x86("call", _use_zframe_install ? "rt_icn_zframe_args_install" : "rt_lcl_proc_args_install",
                        (uint64_t)(uintptr_t)(void *)(_use_zframe_install ? rt_icn_zframe_args_install : rt_lcl_proc_args_install))));
        if (_use_zframe_install) {
            const char * _gn = (prefix && strncmp(prefix, "proc_", 5) == 0) ? prefix + 5 : prefix;
            icn_register_local_offsets(_gn);
        }
    } else if (g_emit.flat_lcl_proc) {
        int kt2 = g_emit.flat_frame_bytes;
        int np = g_emit_cfg ? g_emit_cfg->nparams : 0;
        int nl = g_emit_cfg ? g_emit_cfg->nlocals : 0;
        int frame_total = kt2 + (np + nl) * 16;
        int host_frame_base = frame_total;
        g_emit.flat_carve_total = frame_total;
        (void)host_frame_base;
        std::string _pin;
        if (x86_hb_on()) _pin = x86("mov", RDQ("rsp", frame_total - 8), "rbp") + x86("mov", "rbp", "rsp");
        extern void rt_lcl_proc_args_install(void *, int, int);
        extern void rt_icn_zframe_args_install(void *, int, int);
        int _use_zframe_install = (g_emit_cfg && g_emit_cfg->cell_frame) ? 1 : 0;
        std::string _lseed;
         if (g_emitter.codegen_flat_chain_body__iws < 0) { const char * _e = getenv("SCRIP_ICN_WIRE_STACK"); g_emitter.codegen_flat_chain_body__iws = (_e && *_e == (char)48) ? 0 : 1; }
        {
           if (g_emitter.codegen_flat_chain_body__en_0 < 0) { const char * _e = getenv("SCRIP_LCL_SEED"); g_emitter.codegen_flat_chain_body__en_0 = (_e && *_e == '0') ? 0 : 1; }
          int _lo = (g_emitter.codegen_flat_chain_body__en_0 && g_emit_cfg) ? g_emit_cfg->pf.locals : -1;
          int _rg = (g_emitter.codegen_flat_chain_body__en_0 && g_emit_cfg) ? g_emit_cfg->pf.value_region : -1, _hz = frame_total - (g_emitter.codegen_flat_chain_body__iws ? 8 : 24);
          if (_lo >= 0 && _rg > 0) _lo = 0;
          if (_lo >= 0 && _rg > _lo && _rg <= frame_total - 32)
              _lseed =
                  x86("comment",
                  "LCL-SEED: NULVCL the value region AND the header up to the saved rbp [___+0, ___+hz), so no dead activation's pointer survives inside this frame, the header's unwritten words "
                      "are DT_SNUL cells (ARCH-GC section 13), and lexical locals read unbound")
                     + flat_zero_fill(_lo, _hz - _lo);
          if (_use_zframe_install && _lseed.empty() && _hz > 0)
              _lseed = x86("comment", "the block protocol: no C install runs, so the frame [0, hz) is zeroed here -- every local reads a null DESCR")
                     + flat_zero_fill(0, _hz);
          }
        extern int zls_g_block_args(const IR_graph_t *); extern int zls_g_block_base(const IR_graph_t *);
        int _blk = (_use_zframe_install && g_emit_cfg->pf.block_args) ? 1 : 0;
        int _aoff = _blk ? frame_total : 16;
        if (_blk && g_emit_cfg->pf.block_base != frame_total)
            { fprintf(stderr, "FATAL emit: %s's argument block lies at +%d by the layout and +%d by the prologue -- the two must agree to the byte\n", prefix ? prefix : "?",
            g_emit_cfg->pf.block_base, frame_total); abort(); }
        std::string _inst = _use_zframe_install ? icn_entry_gva() + icn_main_args_inline(np, _blk) : std::string();
        if (g_is_text) {
            char _lp[512]; int _lz = 0;
            _lz += g_emitter.codegen_flat_chain_body__iws ? snprintf(_lp + _lz, (int)sizeof(_lp) - _lz, "sub rsp, %d\n", frame_total)
                        : snprintf(_lp + _lz, (int)sizeof(_lp) - _lz, "sub rsp, %d\nmov qword ptr [rsp + %d], rcx\nmov qword ptr [rsp + %d], rdx\n", frame_total, frame_total - 24, frame_total - 16);
            emit_text_n(_lp, strlen(_lp));
            if (!_lseed.empty()) bb_emit_x86(_lseed);
                bb_emit_x86(emit_gc_map_register(g_emit_cfg ? g_emit_cfg->jcon_value_region : 0, frame_total, frame_total - (g_emit_cfg ? g_emit_cfg->jcon_value_region : 0) - 16,
                ((g_emit_cfg && g_emit_cfg->root_graph) ? GC_FRAME_MAP_ROOT : 0u) |
                (_use_zframe_install ? (GC_FRAME_MAP_ICN_PROC | (_blk ? GC_FRAME_MAP_ICN_BLOCK : 0u) | (x86_hb_on() ? GC_FRAME_MAP_ICN_PINNED : 0u)) : 0u), 0,
                GC_LINK_Q(g_emitter.codegen_flat_chain_body__iws ? frame_total + (_blk ? 16 * np : 0) : frame_total - 24, frame_total - 8,
                    g_emitter.codegen_flat_chain_body__iws ? frame_total + (_blk ? 16 * np : 0) : frame_total, _pin.empty() ? GC_LINK_RBP_KEPT : 0u))
                ); if (!_pin.empty()) bb_emit_x86(_pin);
            if (_use_zframe_install) bb_emit_x86(_inst);
            else { _lz = 0; _lz += snprintf(_lp + _lz, (int)sizeof(_lp) - _lz, "mov rdi, rsp\nmov esi, %d\nmov edx, %d\ncall %s@PLT\n", np, nl, "rt_lcl_proc_args_install");
                   emit_text_n(_lp, strlen(_lp)); }
        } else {
            if (frame_total <= 127) { ef_b3(0x48, 0x83, 0xEC); ef_b1((uint8_t)frame_total); } else { ef_b3(0x48, 0x81, 0xEC); bb_emit_u32((uint32_t)frame_total); }
            if (!g_emitter.codegen_flat_chain_body__iws) {
            { int _d = frame_total - 24; ef_b2(0x48, 0x89); if (_d >= -128 && _d <= 127) { ef_b3(0x4C, 0x24, (uint8_t)(int8_t)_d); } else { ef_b2(0x8C, 0x24); bb_emit_u32((uint32_t)_d); } }
            { int _d = frame_total - 16; ef_b2(0x48, 0x89); if (_d >= -128 && _d <= 127) { ef_b3(0x54, 0x24, (uint8_t)(int8_t)_d); } else { ef_b2(0x94, 0x24); bb_emit_u32((uint32_t)_d); } }
            { if (g_emitter.codegen_flat_chain_body__hd < 0) { const char * _e = getenv("SCRIP_ICN_HDR_DEAD"); g_emitter.codegen_flat_chain_body__hd = (_e && *_e == '1') ? 1 : 0;
                } if (g_emitter.codegen_flat_chain_body__hd) { int _d = frame_total - 8; ef_b2(0x48, 0x89);
                if (_d >= -128 && _d <= 127) { ef_b3(0x6C, 0x24, (uint8_t)(int8_t)_d); } else { ef_b2(0xAC, 0x24); bb_emit_u32((uint32_t)_d); } } }
            }
            if (!_lseed.empty()) bb_emit_x86(_lseed);
                bb_emit_x86(emit_gc_map_register(g_emit_cfg ? g_emit_cfg->jcon_value_region : 0, frame_total, frame_total - (g_emit_cfg ? g_emit_cfg->jcon_value_region : 0) - 16,
                ((g_emit_cfg && g_emit_cfg->root_graph) ? GC_FRAME_MAP_ROOT : 0u) |
                (_use_zframe_install ? (GC_FRAME_MAP_ICN_PROC | (_blk ? GC_FRAME_MAP_ICN_BLOCK : 0u) | (x86_hb_on() ? GC_FRAME_MAP_ICN_PINNED : 0u)) : 0u), 0,
                GC_LINK_Q(g_emitter.codegen_flat_chain_body__iws ? frame_total + (_blk ? 16 * np : 0) : frame_total - 24, frame_total - 8,
                    g_emitter.codegen_flat_chain_body__iws ? frame_total + (_blk ? 16 * np : 0) : frame_total, _pin.empty() ? GC_LINK_RBP_KEPT : 0u))
                ); if (!_pin.empty()) bb_emit_x86(_pin);
            if (_use_zframe_install) bb_emit_x86(_inst);
            else {
            ef_b3(0x48, 0x89, 0xE7);
            ef_b1(0xBE); bb_emit_u32((uint32_t)np);
            ef_b1(0xBA); bb_emit_u32((uint32_t)nl);
            { uint64_t _fn = (uint64_t)(uintptr_t)(void *)rt_lcl_proc_args_install; ef_b2(0x48, 0xB8); bb_emit_u64(_fn); ef_b2(0xFF, 0xD0); } }
        }
        if (g_emitter.codegen_flat_chain_body__iws && _use_zframe_install) {
            const char * _apn = (g_emit_cfg && g_emit_cfg->root_graph) ? "main" : (strncmp(prefix, "proc_", 5) == 0) ? prefix + 5 : prefix;
            extern int * const rt_k_level_p;
            bb_emit_x86( x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&rt_k_level_p, "rt_k_level_p")
                     + x86("mov", "rax", RDQ("rax", 0))
                     + x86("add", RDD("rax", 0), (long)1)
                     + x86("mov", "ecx", RDD("rax", 0))
                     + x86("movsxd", "rcx", "ecx")
                     + x86("sub", "rcx", (long)1)
                     + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_runtime, "g_runtime")
                     + x86("mov", RDQ("rax", GRT_AT(kw_fnclevel)), "rcx"));
            bb_emit_x86(icn_trace_tap(_apn, 1, np, _aoff));
            icn_register_local_offsets(_apn);
        }
    }
    if (!bare) emit_label_define_bb(&lbl_α_body);
    if (lbl_α_orig_p && xa_flat_class_c_pred() && !g_runtime.eval.rt_fragment_emit) emit_label_define_bb(lbl_α_orig_p);
    { extern std::string bb_zdp_origin(long); if (_pl->zdp_on) bb_emit_x86(bb_zdp_origin((long)0)); } { if (x86_zdp_rbp_on()) bb_emit_x86(x86_zsm_ev(0)); }
    int _sigk = 0;
    if (xa_flat_class_c_pred()) { sig_prepare_chain(fam); xa_flat_chain_prologue(fam, &_sigk); }
    if (!bare && alt_step_lbl && g_emit_cfg && g_emit_cfg->poll_alpha) {
        bb_label_t * pa_ok = emit_label_alloc("%s_pa", fam);
        bb_emit_x86(x86_pl_alarm_probe());
        emit_jmp_label(pa_ok, JMP_JE);
        bb_emit_x86(x86_alarm_raise_call());
        emit_jmp_label(alt_step_lbl, JMP_JE);
        emit_label_define_bb(pa_ok);
    }
    if (!bare && blob_frame_scope()) blob_first_guard();
    { int _bfb = blob_frame_bytes(); if (_bfb > 0) { blob_layout_build();
        bb_emit_x86( x86("push", "rbp") + x86("mov", "rbp", "rsp") + x86("sub", "rsp", (long)(blob_carve_bytes() + blob_carve_pad())) +
        emit_gc_map_register(_bfb + 16, _bfb + 16, 24, GC_FRAME_MAP_BLOB, 2, GC_LINK_Q(8, 0, 8, 0u)) + blob_zero_fill(_bfb) +
        IF(blob_cont_copy(), x86("mov", "rcx", RDQ("rbp", 8)) + x86("mov", RDQ("rbp", -8), "rcx") + x86("mov", "rcx", RDQ("rbp", 16)) + x86("mov", RDQ("rbp", -16), "rcx")) +
        x86("mov", RDQ("rbp", -32), (long)DT_P) + x86("mov", RDQ("rbp", -24), "rdx") + IF(sn4_blob_casmark(), x86("mov", RDQ("rbp", -40), "r12"))); } else if (_pl->spf) { _spf = 1; _sph = _pl->sph;
            _spgd = _pl->spgd;
        if (_sph)
        bb_emit_x86(x86("sub", "rsp", (long)_sph) + x86("mov", RDQ("rsp", _sph - 16), (long)DT_P) + x86("mov", RDQ("rsp", _sph - 8), "rdx") +
        IF(_sph == 32, x86("mov", RDQ("rsp", 0), (long)DT_I) + x86("mov", RDQ("rsp", 8), "r12"))); }
    }
    { extern int g_flat_outer_nparams; int _legacy = (!g_emit.flat_jmp_entry && !g_emit.flat_pat && !g_emit.flat_gen && !g_gen_proc_active && !g_emit.zframe_graph && !g_emit.flat_lcl_proc);
        extern int g_glue_entered; g_glue_entered = (g_emit.flat_outer_nparams == 0 && _legacy) ? 1 : 0; if (g_glue_entered) bb_emit_x86(x86_main_prologue()); {
        if (g_emitter.codegen_flat_chain_body__gluo < 0) { const char * e = getenv("SCRIP_GLUEO"); g_emitter.codegen_flat_chain_body__gluo = (e && *e == '0') ? 0 : 1;
            } if (g_emitter.codegen_flat_chain_body__gluod < 0) { const char * e = getenv("SCRIP_GLUEO_DIAG");
        g_emitter.codegen_flat_chain_body__gluod = (e && *e == '1') ? 1 : 0; } extern int g_glue_o_sup;
            g_glue_o_sup = (g_emitter.codegen_flat_chain_body__gluo && g_glue_entered && !emit_rec_pin()) ? 1 : 0; if (g_glue_o_sup) g_glue_entered = 0;
        if (g_emitter.codegen_flat_chain_body__gluod)
        fprintf(stderr, "[GLUEO] graph=%s entered=%d rec_pin=%d deep=%d pat=%d gen=%d -> closed_loop_suppressed=%d\n", g_emit.flat_lbl_α ? g_emit.flat_lbl_α : "<anon>", g_glue_entered,
        emit_rec_pin() ? 1 : 0, g_emit.flat_deep_arrival, g_emit.flat_pat, g_emit.flat_gen, g_glue_o_sup); } }
    nidx_build(g_planner.codegen_flat_chain_body_nodes, n);
    bb_label_t **lbls = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t **betas = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t **ra_y = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t **ra_t = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t **na_s = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t ***fc_sig = (bb_label_t ***)alloca(sizeof(bb_label_t **) * n);
    bb_label_t **na_f = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t **na_fo = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t **bxs = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    g_emitter.g_suspend_resume_slot = -1;
    if (g_gen_proc_active && g_emit_cfg && g_emit_cfg->resume_slot >= 0)
        for (int _si = 0; _si < n; _si++) if (g_planner.codegen_flat_chain_body_nodes[_si]->op == IR_SUSPEND) { g_emitter.g_suspend_resume_slot = g_emit_cfg->resume_slot; break; }
    if (g_emitter.g_suspend_resume_slot < 0 && g_resumable_callable_active && g_emit_cfg && g_emit_cfg->resume_slot >= 0)
        g_emitter.g_suspend_resume_slot = g_emit_cfg->resume_slot;
     int id = g_emitter.codegen_flat_chain_body__diag_chain_seq++;
    int own_mark = -1;
    {
        for (int _oi = 0; _oi < n; _oi++) if (g_planner.codegen_flat_chain_body_nodes[_oi]->op == IR_MATCH_BEGIN) { int _ho = _pl->zn[_oi].loff;
        if (_ho >= 0) own_mark = _ho + 16; break; } }
    for (int i = 0; i < n; i++) {
        int _uid = g_flat_node_id++; const char * _kn = flat_label_kind(g_planner.codegen_flat_chain_body_nodes[i]->op);
        lbls[i] = emit_label_alloc("n%d_%s_α", _uid, _kn);
        { int _flk = ir_floater_kind(g_planner.codegen_flat_chain_body_nodes[i]); if (_flk) lbls[i] = emit_floater_label(_flk); }
        if (g_emit_cfg && g_emit_cfg->n_balias > 0) { for (int _bi = 0; _bi < g_emit_cfg->n_balias; _bi++) if (g_emit_cfg->balias_node[_bi] == g_planner.codegen_flat_chain_body_nodes[i]) { lbls[i] =
            emit_label_alloc("%s", g_emit_cfg->balias_name[_bi]); break; } }
        if (g_emit_cfg && g_emit_cfg->n_dentry > 0) { for (int _dq = 0; _dq < g_emit_cfg->n_dentry; _dq++) if (g_emit_cfg->dentry_entry[_dq] == g_planner.codegen_flat_chain_body_nodes[i])
            g_emit_cfg->dentry_name[_dq] = lbls[i]->name;
            }
        betas[i] = emit_label_alloc("n%d_%s_β", _uid, _kn);
        ra_y[i] = (g_planner.codegen_flat_chain_body_nodes[i]->op == IR_REPALT) ? emit_label_alloc(".L%s_γ_%d_ry", _kn, _uid) : NULL;
        ra_t[i] = (g_planner.codegen_flat_chain_body_nodes[i]->op == IR_REPALT) ? emit_label_alloc(".L%s_ω_%d_rt", _kn, _uid) : NULL;
        na_s[i] = ir_glue_box(g_planner.codegen_flat_chain_body_nodes[i], 0) ? emit_label_alloc(".L%s_γ_%d_as", _kn, _uid) : NULL;
        { int _naf_on = ir_glue_box(g_planner.codegen_flat_chain_body_nodes[i], 1);
          na_f[i] = _naf_on ? emit_label_alloc(".L%s_γ_%d_af", _kn, _uid) : NULL;
          na_fo[i] = _naf_on ? emit_label_alloc(".L%s_ω_%d_af", _kn, _uid) : NULL; }
        bxs[i] = emit_label_alloc("n%d_%s_bx", _uid, _kn);
        fc_sig[i] = NULL;
        if (g_planner.codegen_flat_chain_body_nodes[i]->op == IR_MATCH_ALTERNATE && g_planner.codegen_flat_chain_body_nodes[i]->n_operands > 0) {
            int _N = (int)(g_planner.codegen_flat_chain_body_nodes[i]->n_operands / 2);
            fc_sig[i] = (bb_label_t **)alloca(sizeof(bb_label_t *) * _N);
            for (int _j = 0; _j < _N; _j++) fc_sig[i][_j] = emit_label_alloc(".L%s_γ_%d_s%d", _kn, _uid, _j);
        }
    }
    bb_label_t *resume_init_lbl = NULL;
    if (g_emitter.g_suspend_resume_slot >= 0 && g_gen_proc_active) {
        for (int _si = 0; _si < n; _si++) if (g_planner.codegen_flat_chain_body_nodes[_si]->op == IR_SUSPEND) { resume_init_lbl = betas[_si]; break; }
    }
    if (resume_init_lbl) {
        if (g_is_text) {
            char _init[fmt_len("lea rax, [rip + %s]\n", resume_init_lbl->name)];
            snprintf(_init, sizeof _init, "lea rax, [rip + %s]\n", resume_init_lbl->name);
            emit_text_n(_init, strlen(_init));
        } else {
            ef_b3(0x48, 0x8D, 0x05); bb_emit_patch_rel32(resume_init_lbl);
        }
        bb_emit_x86(x86("mov", FRQ(g_emitter.g_suspend_resume_slot), "rax"));
    }
    for (int _li = 0; _li < n && !g_emit_cfg->pf.home_frame; _li++) if (g_planner.codegen_flat_chain_body_nodes[_li]->op == IR_LIMIT) {
        g_emit.op_off = drive_value_slot(g_planner.codegen_flat_chain_body_nodes[_li]);
        bb_emit_x86(bb_limit_init());
    }
    if (g_emit_cfg && n_alt > 1 && n_alt <= 4096 && emit_resumable_frame() && !g_emit_cfg->root_graph && !g_emit.flat_pkt && g_emit_cfg->nparams >= 1 && g_emit_cfg->pnames && g_emit_cfg->pnames[0] &&
        g_emit_cfg->alt_entry) {
        ixp = (IxP *)alloca(sizeof(IxP) * (size_t)g_emit_cfg->nparams);
        for (int _pi = 0; _pi < g_emit_cfg->nparams; _pi++) {
            if (!g_emit_cfg->pnames[_pi]) continue;
            IxP & P = ixp[ix_np]; P.n = 0; P.nset = 0; P.fbr = -2; P.fba = -2; P.fbi = -2; P.sw = (bb_label_t *)0; P.slot = _pi < (int)_pl->v_pso.size() ? _pl->v_pso[_pi] : -1;
        const char * _p0 = g_emit_cfg->pnames[_pi];
        int * _ktag = (int *)alloca(sizeof(int) * (size_t)n_alt); int64_t * _kval = (int64_t *)alloca(sizeof(int64_t) * (size_t)n_alt); int _nvar = 0, _nvc = -1;
        for (int _ak = 0; _ak < n_alt; _ak++) { IR_t * _e = g_emit_cfg->alt_entry[_ak]; _ktag[_ak] = 0; _kval[_ak] = 0;
            { int _sg = 0; while (_e && _e->op == IR_SUCCEED && _e->γ.node && _sg++ < 4096) _e = _e->γ.node; }
            { int _hw = 0; while (_e && (_e->op == IR_UNIFY_CONST || _e->op == IR_UNIFY_STRUCT) && _e->n_operands >= 3 && _e->operands[0] && _e->operands[0]->op == IR_VAR_REF && IR_LIT(_e->operands[0]).sval && strcmp(IR_LIT(_e->operands[0]).sval, _p0) && _e->γ.node && _hw++ < 4096) _e = _e->γ.node; }
            if (_e && (_e->op == IR_UNIFY_CONST || _e->op == IR_UNIFY_STRUCT) && _e->n_operands >= 3 && _e->operands[0] && _e->operands[0]->op == IR_VAR_REF && IR_LIT(_e->operands[0]).sval &&
                !strcmp(IR_LIT(_e->operands[0]).sval, _p0) && _e->operands[2]) {
                IR_t * _k = _e->operands[2];
                if (_e->op == IR_UNIFY_CONST && _k->op == IR_LIT_ATOM) { _ktag[_ak] = DT_PLATOM; _kval[_ak] = (int64_t)(unsigned)prolog_atom_intern_lit(IR_LIT(_k).sval ? IR_LIT(_k).sval : ""); }
                else if (_e->op == IR_UNIFY_CONST && _k->op == IR_LIT_INTEGER) { _ktag[_ak] = DT_I; _kval[_ak] = (int64_t)IR_LIT(_k).ival; }
                else if (_e->op == IR_UNIFY_STRUCT && _k->op == IR_LIT_INTEGER && prolog_functor_arity((int)IR_LIT(_k).ival) >= 1) { _ktag[_ak] = DT_PLREF; _kval[_ak] = (int64_t)IR_LIT(_k).ival; } }
            if (!_ktag[_ak]) { _nvar++; _nvc = _ak; } }
        if (P.slot >= 0 && _nvar < n_alt) {
            P.tag = (int *)alloca(sizeof(int) * (size_t)n_alt); P.val = (int64_t *)alloca(sizeof(int64_t) * (size_t)n_alt); P.code = (int *)alloca(sizeof(int) * (size_t)n_alt);
            const int _scap = n_alt * 8 + 64; int _nsi = 0, _vset = -9;
            P.sets = (int *)alloca(sizeof(int) * (size_t)_scap); P.set_beg = (int *)alloca(sizeof(int) * (size_t)(n_alt + 2)); P.set_len = (int *)alloca(sizeof(int) * (size_t)(n_alt + 2));
            auto _setcode = [&](int ct, int64_t kv, int keyed) -> int {
                int _m = 0, _who = -1, _b0 = _nsi;
                for (int _b = 0; _b < n_alt; _b++) if (!_ktag[_b] || (keyed && _ktag[_b] == ct && _kval[_b] == kv)) { _m++; _who = _b; }
                if (_m == 0) return -1;
                if (_m == 1) return _who;
                if (_m == n_alt || _nsi + _m > _scap || P.nset >= n_alt + 2) return -2;
                for (int _b = 0; _b < n_alt; _b++) if (!_ktag[_b] || (keyed && _ktag[_b] == ct && _kval[_b] == kv)) P.sets[_nsi++] = _b;
                P.set_beg[P.nset] = _b0; P.set_len[P.nset] = _m; return -3 - P.nset++; };
            for (int _c = 0; _c < 3; _c++) { int _nk = 0, _n0 = P.n, _ns0 = P.nset, _si0 = _nsi; const int _ct = _c == 0 ? (int)DT_PLREF : _c == 1 ? (int)DT_PLATOM : (int)DT_I;
                for (int _ak = 0; _ak < n_alt; _ak++) { if (_ktag[_ak] != _ct) continue; int _dup = 0;
                    for (int _q = _n0; _q < P.n && !_dup; _q++) if (P.val[_q] == _kval[_ak]) _dup = 1;
                    if (_dup) continue;
                    P.tag[P.n] = _ct; P.val[P.n] = _kval[_ak]; P.code[P.n] = _setcode(_ct, _kval[_ak], 1); P.n++; _nk++; }
                { int _fb; if (_vset == -9) _vset = _setcode(0, 0, 0); _fb = _vset;
                  if (_c == 0) P.fbr = _fb; else if (_c == 1) P.fba = _fb; else P.fbi = _fb; } }
            P.sw = emit_label_alloc("%s_switch%d", fam, _pi); ix_np++; } } }
    if (ix_np) ix_switch = ixp[0].sw;
    if (ix_switch) emit_jmp_label(ix_switch, JMP_JMP);
    else if (flat_empty_body_fail && n > 0) emit_jmp_label(alt_step_lbl && n_alt > 1 ? alt_step_lbl : &lbl_ω, JMP_JMP);
    int _bx_open = -1;
    int *repalt_of = (int *)alloca(sizeof(int) * (n > 0 ? n : 1)); for (int _q = 0; _q < n; _q++) repalt_of[_q] = -1;
    for (int _r = 0; _r < n; _r++) if (g_planner.codegen_flat_chain_body_nodes[_r]->op == IR_REPALT && g_planner.codegen_flat_chain_body_nodes[_r]->n_operands > 0) { int _k =
        nidx(g_planner.codegen_flat_chain_body_nodes, n, g_planner.codegen_flat_chain_body_nodes[_r]->operands[0]); if (_k >= 0 && repalt_of[_k] < 0) repalt_of[_k] = _r;
        }
    std::unordered_map<const IR_t *, int> repalt_by_fail, repalt_by_op0;
    for (int _r = 0; _r < n; _r++) {
        IR_t * _rn = g_planner.codegen_flat_chain_body_nodes[_r];
        if (_rn->op != IR_REPALT) continue;
        if (_rn->n_operands > 2) repalt_by_fail.emplace(_rn->operands[2], _r);
        if (_rn->n_operands > 0) repalt_by_op0.emplace(_rn->operands[0], _r);
    }
    std::unordered_map<const bb_label_t *, int> beta_of; beta_of.reserve((size_t)n * 2 + 1); for (int _q = 0; _q < n; _q++) if (betas[_q]) beta_of.emplace(betas[_q], _q);
    int _hk_c = -1, _hk_upto = 0;
    if (g_emit_cfg && n_alt > 1 && g_emit_cfg->alt_entry && !g_emit_cfg->root_graph) {
        IR_t * _e0 = g_emit_cfg->alt_entry[0];
        { int _sg = 0; while (_e0 && _e0->op == IR_SUCCEED && _e0->γ.node && _sg++ < 4096) _e0 = _e0->γ.node; }
        if (_e0 && _e0->op == IR_SUCCEED && nidx(g_planner.codegen_flat_chain_body_nodes, n, _e0) < 0) {
            emit_jmp_label(g_emit_cfg->alt_redo[0] ? g_emitter.codegen_flat_chain_body_ret_tr[0] : &lbl_γ, JMP_JMP);
        }
    }
    for (int i = 0; i < n; i++) {
        { int _fk = ir_floater_kind(g_planner.codegen_flat_chain_body_nodes[i]); if (_fk && _flt_hoisted[_fk]) { g_flat_node_id += _flt_uid_burn[_fk]; continue; } }
        { if (g_emitter.codegen_flat_chain_body__sz < 0) { const char *_e = getenv("SCRIP_ASM_SYMSIZE"); g_emitter.codegen_flat_chain_body__sz = (_e && _e[0] == '0') ? 0 : 1; }
          if (g_emitter.codegen_flat_chain_body__sz && g_is_text) {
              if (_bx_open >= 0) emit_textf(".size %s, .-%s\n", bxs[_bx_open]->name, bxs[_bx_open]->name);
              emit_textf(".type %s, @function\n%s:\n", bxs[i]->name, bxs[i]->name); _bx_open = i; } }
        { if (g_emitter.codegen_flat_chain_body__beo < 0) { const char *_e = getenv("SCRIP_BETA_ELIDE_OFF"); g_emitter.codegen_flat_chain_body__beo = (_e && _e[0] == '1') ? 1 : 0;
            } g_emit.op_beta_dead = (g_emitter.codegen_flat_chain_body__beo || _pl->zn[i].bu) ? 0 : 1; }
        g_emit.op_wpop = 0;
        { g_emit.op_own_mark = own_mark; g_emit.op_own_ci = _pl->zn[i].owni; }
        bb_label_t *node_γ = &lbl_γ;
        bb_label_t *node_ω = &lbl_ω;
        IR_t *gtgt = g_planner.codegen_flat_chain_body_nodes[i]->γ.node;
        IR_t *otgt = g_planner.codegen_flat_chain_body_nodes[i]->ω.node;
        int gamma_is_beta = port_sz_beta(g_planner.codegen_flat_chain_body_nodes[i]->γ.sz);
        int omega_is_beta = port_sz_beta(g_planner.codegen_flat_chain_body_nodes[i]->ω.sz);
        int gamma_is_sig = (g_planner.codegen_flat_chain_body_nodes[i]->γ.sz[0] == (char)0xcf && (unsigned char)g_planner.codegen_flat_chain_body_nodes[i]->γ.sz[1] == 0x83);
        int gamma_is_phi = (g_planner.codegen_flat_chain_body_nodes[i]->γ.sz[0] == (char)0xcf && (unsigned char)g_planner.codegen_flat_chain_body_nodes[i]->γ.sz[1] == 0x86);
        int omega_is_phi = (g_planner.codegen_flat_chain_body_nodes[i]->ω.sz[0] == (char)0xcf && (unsigned char)g_planner.codegen_flat_chain_body_nodes[i]->ω.sz[1] == 0x86);
        { int _gg = 0; IR_t * _g = gtgt; while (_g && _g->op == IR_GOTO && _gg++ < 128) { if (!gamma_is_beta) gamma_is_beta = port_sz_beta(_g->γ.sz);
            if (!gamma_is_sig) gamma_is_sig = (_g->γ.sz[0] == (char)0xcf && (unsigned char)_g->γ.sz[1] == 0x83);
            if (!gamma_is_phi) gamma_is_phi = (_g->γ.sz[0] == (char)0xcf && (unsigned char)_g->γ.sz[1] == 0x86); gtgt = _g->γ.node; _g = gtgt; } }
        { int _gg = 0; IR_t * _o = otgt; while (_o && _o->op == IR_GOTO && _gg++ < 128) { if (!omega_is_beta) omega_is_beta = port_sz_beta(_o->γ.sz);
            if (!omega_is_phi) omega_is_phi = (_o->γ.sz[0] == (char)0xcf && (unsigned char)_o->γ.sz[1] == 0x86); otgt = _o->γ.node; _o = otgt; } }
        { int _op0 = (int)g_planner.codegen_flat_chain_body_nodes[i]->op; int _always_exits = (_op0 == IR_RETURN || _op0 == IR_SUSPEND || _op0 == IR_CORET || _op0 == IR_COFAIL);
          if (!_always_exits) { int _sg = 0; while (gtgt && gtgt->op == IR_SUCCEED && gtgt->γ.node && _sg++ < 4096) gtgt = gtgt->γ.node; }
          if (!_always_exits) { int _sg = 0; while (otgt && otgt->op == IR_SUCCEED && otgt->γ.node && _sg++ < 4096) otgt = otgt->γ.node; } }
        int omega_is_retry = omega_is_beta && !beta_is_stmt_land(otgt);
        { int k = nidx(g_planner.codegen_flat_chain_body_nodes, n, gtgt); if (k >= 0) {
            node_γ = (gamma_is_phi && na_f[k]) ? na_f[k] : (gamma_is_sig && na_s[k]) ? na_s[k] : gamma_is_beta ? betas[k] : lbls[k];
            if (getenv("SCRIP_NAF_DIAG") && gamma_is_phi && na_f[k])
                fprintf(stderr, "[NAF-DIAG] GAMMA i=%d(%s) -> na_f[k=%d](%s) label=%s\n", i, bb_op_name(g_planner.codegen_flat_chain_body_nodes[i]->op), k,
                    bb_op_name(g_planner.codegen_flat_chain_body_nodes[k]->op), na_f[k]->name);
            if (gamma_is_sig && fc_sig[k]) {
                int _N = (int)(g_planner.codegen_flat_chain_body_nodes[k]->n_operands / 2), _arm = 0, _src = -1, _best = -1;
                { int _p = nidx(g_planner.codegen_flat_chain_body_nodes, n, g_planner.codegen_flat_chain_body_nodes[i]); if (_p >= 0) { _src = _p; } }
                for (int _j = 0; _j < _N; _j++) { IR_t *_e = g_planner.codegen_flat_chain_body_nodes[k]->operands[2 * _j]; { int _p = nidx(g_planner.codegen_flat_chain_body_nodes, n, _e);
                    if (_p >= 0) { if (_p <= _src && _p > _best) { _best = _p; _arm = _j; } } } }
                node_γ = fc_sig[k][_arm];
            }
            if (gamma_is_sig) { node_γ = node_γ; }
            if (gamma_is_phi) { node_γ = node_γ; }
        } }
        { int _flk = ir_floater_kind(gtgt); if (_flk) node_γ = emit_floater_label(_flk); }
        { if (gtgt && ir_norm_call_kind(gtgt->op) == IR_CALL && ir_floater_member(gtgt) && node_γ == &lbl_γ) node_γ = emit_floater_label(1); }
        if (gtgt == NULL || gtgt->op == IR_SUCCEED) node_γ = &lbl_γ;
        if (gtgt && gtgt->op == IR_FAIL) node_γ = &lbl_ω;
        int omega_resolved = 0;
        { int k = nidx(g_planner.codegen_flat_chain_body_nodes, n, otgt); if (k >= 0) { node_ω = (omega_is_phi && na_fo[k]) ? na_fo[k] : omega_is_beta ? betas[k] : lbls[k]; omega_resolved = 1;
            if (getenv("SCRIP_NAF_DIAG") && omega_is_phi && na_fo[k])
            fprintf(stderr, "[NAF-DIAG] OMEGA i=%d(%s) -> na_fo[k=%d](%s) label=%s\n", i, bb_op_name(g_planner.codegen_flat_chain_body_nodes[i]->op), k,
                bb_op_name(g_planner.codegen_flat_chain_body_nodes[k]->op), na_fo[k]->name); } }
        { int _flk = ir_floater_kind(otgt); if (_flk) { node_ω = emit_floater_label(_flk); omega_resolved = 1; } }
        if (!omega_resolved) node_ω = (otgt && otgt->op == IR_SUCCEED) ? &lbl_γ : &lbl_ω;
        if (zd_chn[2 * i] >= 0) node_γ = betas[zd_chn[2 * i]];
        if (zd_chn[2 * i + 1] >= 0) node_ω = betas[zd_chn[2 * i + 1]];
        g_emit.op_omega_is_death = (!omega_resolved && !(otgt && otgt->op == IR_SUCCEED)) ? 1 : 0;
        if (g_emit_cfg && n_alt > 0) {
              if (otgt && otgt == g_emit_cfg->alt_fail && alt_step_lbl) node_ω = alt_step_lbl;
              if (gtgt && gtgt == g_emit_cfg->alt_fail && alt_step_lbl) node_γ = alt_step_lbl;
              if (!g_emit_cfg->root_graph) for (int _ak = 0; _ak < n_alt; _ak++) if (gtgt && gtgt == g_emit_cfg->alt_ret[_ak] && g_emit_cfg->alt_redo[_ak]) { node_γ =
                  g_emitter.codegen_flat_chain_body_ret_tr[_ak]; break; } }
        if (getenv("SCRIP_OMEGA_DIAG"))
            fprintf(stderr, "[OMEGA-DIAG] i=%d node_op=%s omega_resolved=%d otgt_op=%s op_omega_is_death=%d\n", i, bb_op_name(g_planner.codegen_flat_chain_body_nodes[i]->op), omega_resolved,
                otgt ? bb_op_name(otgt->op) : "NULL",
            g_emit.op_omega_is_death);
        { int r = repalt_of[i]; if (r >= 0) { node_γ = ra_y[r]; if (g_planner.codegen_flat_chain_body_nodes[r]->n_operands < 3) node_ω = ra_t[r]; } }
        if (g_planner.codegen_flat_chain_body_nodes[i]->ω.node && g_planner.codegen_flat_chain_body_nodes[i]->ω.node->op == IR_FAIL) {
            auto _rf = repalt_by_fail.find(g_planner.codegen_flat_chain_body_nodes[i]->ω.node);
            if (_rf != repalt_by_fail.end()) node_ω = ra_t[_rf->second];
        }
        g_emitter.g_limit_gen_beta = NULL;
        g_emitter.g_scan_body_beta = NULL;
        g_emitter.g_scan_t0_pop = 0;
        g_create_body_entry = NULL;
        g_emitter.g_suspend_dobody_beta = NULL;
        if (_pl->zn[i].sdb == -1) g_emitter.g_suspend_dobody_beta = &lbl_ω;
        else if (_pl->zn[i].sdb >= 0) g_emitter.g_suspend_dobody_beta = _pl->zn[i].sdbb ? betas[_pl->zn[i].sdb] : lbls[_pl->zn[i].sdb];
        if (g_planner.codegen_flat_chain_body_nodes[i]->op == IR_MATCH_DEFER || g_planner.codegen_flat_chain_body_nodes[i]->op == IR_MATCH_TRAP) { g_emit.lbl_t1_p = NULL; g_emit.lbl_t1 = NULL;
            { const znode_t * _t = &_pl->zn[i];
                if (_t->dtk >= 0) { bb_label_t * _sl = (_t->dtph && na_fo[_t->dtk]) ? na_fo[_t->dtk] : betas[_t->dtk]; g_emit.lbl_t1_p = _sl;
                    if (_t->dtd < 0) g_emit.lbl_t1_p = (bb_label_t *)0;
                    else if (_t->dtd > 0) { _tpl[_tpn] = emit_label_alloc("%s_trap%d", prefix, _tpn); _tpd[_tpn] = _t->dtd; _tpt[_tpn] = _sl; g_emit.lbl_t1_p = _tpl[_tpn++]; }
                    g_emit.lbl_t1 = g_emit.lbl_t1_p ? g_emit.lbl_t1_p->name : NULL; } } }
        if (g_planner.codegen_flat_chain_body_nodes[i]->op == IR_CREATE && g_planner.codegen_flat_chain_body_nodes[i]->n_operands > 0 && g_planner.codegen_flat_chain_body_nodes[i]->operands[0]) {
            IR_t *body_entry = g_planner.codegen_flat_chain_body_nodes[i]->operands[0];
            g_emit.op_sval_lbl = NULL;
            g_create_body_entry = NULL;
            { int k = nidx(g_planner.codegen_flat_chain_body_nodes, n, body_entry); if (k >= 0) { g_emit.op_sval_lbl = lbls[k]->name; g_create_body_entry = lbls[k]; } }
        }
        if (g_planner.codegen_flat_chain_body_nodes[i]->op == IR_GATE_ARM && g_planner.codegen_flat_chain_body_nodes[i]->n_operands > 1 && g_planner.codegen_flat_chain_body_nodes[i]->operands[1] &&
            !g_planner.codegen_flat_chain_body_nodes[i]->γ.node) {
            IR_t *igp = g_planner.codegen_flat_chain_body_nodes[i]->operands[1];
            IR_t *sg = igp->γ.node;
            int sg_is_beta = port_sz_beta(igp->γ.sz);
            int sg_is_sig = (igp->γ.sz[0] == (char)0xcf && (unsigned char)igp->γ.sz[1] == 0x83);
            int sg_is_phi = (igp->γ.sz[0] == (char)0xcf && (unsigned char)igp->γ.sz[1] == 0x86);
            if (sg == NULL || sg->op == IR_SUCCEED) node_γ = &lbl_γ;
            else if (sg->op == IR_FAIL) node_γ = &lbl_ω;
            else { int k = nidx(g_planner.codegen_flat_chain_body_nodes, n, sg);
                if (k >= 0) { node_γ = (sg_is_phi && na_f[k]) ? na_f[k] : (sg_is_sig && na_s[k]) ? na_s[k] : sg_is_beta ? betas[k] : lbls[k]; } }
            if (g_emit_cfg && n_alt > 0 && !g_emit_cfg->root_graph)
                for (int _ak = 0; _ak < n_alt; _ak++) if (sg && sg == g_emit_cfg->alt_ret[_ak] && g_emit_cfg->alt_redo[_ak]) { node_γ = g_emitter.codegen_flat_chain_body_ret_tr[_ak]; break; }
        }
        if (g_planner.codegen_flat_chain_body_nodes[i]->op == IR_REPALT) {
            { if (g_emitter.codegen_flat_chain_body__dd < 0) { const char *_e = getenv("SCRIP_DRIVE_DIAG"); g_emitter.codegen_flat_chain_body__dd = (_e && _e[0] == '1') ? 1 : 0; }
              long _p0 = g_emitter.codegen_flat_chain_body__dd ? emit_text_count() : 0;
              flat_drive_repalt(g_planner.codegen_flat_chain_body_nodes, n, i, lbls, betas, ra_y, ra_t, node_γ, node_ω);
              if (g_emitter.codegen_flat_chain_body__dd && emit_text_count() == _p0)
                  fprintf(stderr, "[DRIVE-DIAG] ZERO-EMIT chain=%d i=%d op=%s n_operands=%d (repalt path)\n", id, i, bb_op_name(g_planner.codegen_flat_chain_body_nodes[i]->op),
                  g_planner.codegen_flat_chain_body_nodes[i]->n_operands); }
            continue;
        }
        if (g_planner.codegen_flat_chain_body_nodes[i]->op == IR_GATE && g_planner.codegen_flat_chain_body_nodes[i]->n_operands > 0) {
            if (zd_xb && (zd_gp[i] != 0 || zd_wp[i] != 0)) { g_emitter.g_zd_stage = 1; g_emitter.g_zd_arm = 0; g_emitter.g_zd_gpop = zd_gp[i]; g_emitter.g_zd_wpop = zd_wp[i];
                g_emitter.g_zd_wsteal = 0; g_emitter.g_zd_cpread = 0; g_emitter.g_zd_rdread = 0; }
            flat_drive_gate(g_planner.codegen_flat_chain_body_nodes, n, i, lbls, betas, node_γ, node_ω, zd_on, zd_out, zd_xb);
            continue;
        }
        if (g_planner.codegen_flat_chain_body_nodes[i]->op == IR_CLEANUP) {
            flat_drive_cleanup(g_planner.codegen_flat_chain_body_nodes, n, i, lbls, betas, node_γ, node_ω, _pl->zn[i].clr);
            continue;
        }
        if ((g_planner.codegen_flat_chain_body_nodes[i]->op == IR_MATCH_ALTERNATE || g_planner.codegen_flat_chain_body_nodes[i]->op == IR_MATCH_ARBNO ||
            g_planner.codegen_flat_chain_body_nodes[i]->op == IR_MATCH_FENCE1 ||
            g_planner.codegen_flat_chain_body_nodes[i]->op == IR_SCAN_ALTERNATE ||
            g_planner.codegen_flat_chain_body_nodes[i]->op == IR_DISJUNCTION) && g_planner.codegen_flat_chain_body_nodes[i]->n_operands > 0) {
            { if (g_emitter.codegen_flat_chain_body__dd_0 < 0) { const char *_e = getenv("SCRIP_DRIVE_DIAG"); g_emitter.codegen_flat_chain_body__dd_0 = (_e && _e[0] == '1') ? 1 : 0; }
              long _p0 = g_emitter.codegen_flat_chain_body__dd_0 ? emit_text_count() : 0;
              if (g_planner.codegen_flat_chain_body_nodes[i]->op == IR_DISJUNCTION && zd_on[i] && _pl->zn[i].djadm && _pl->zn[i].djret) {
                  g_emitter.g_zd_stage = 1;
                  g_emitter.g_zd_arm = 0;
                  g_emitter.g_zd_gpop = zd_gp[i];
                  g_emitter.g_zd_wpop = zd_wp[i];
                  g_emitter.g_zd_wsteal = 0;
                  g_emitter.g_zd_cpread = (zd_cpd && zd_cpd[i] <= -2) ? 1 : 0; g_emitter.g_zd_rdread = 0;
              }
              else if (g_planner.codegen_flat_chain_body_nodes[i]->op == IR_DISJUNCTION && zd_on[i]) {
                  g_emitter.g_zd_stage = 1;
                  g_emitter.g_zd_arm = 1;
                  g_emitter.g_zd_djok = _pl->zn[i].djadm;
                  g_emitter.g_zd_gpop = zd_gp[i];
                  g_emitter.g_zd_wpop = zd_wp[i];
                  g_emitter.g_zd_wsteal = 0;
                  g_emitter.g_zd_k = _pl->zn[i].k;
                  g_emitter.g_zd_abs = zd_out[i] + ((zd_xb && zd_xb[i] != INT_MIN) ? zd_xb[i] : 0); g_planner.g_zd_cur = i; g_emitter.g_zd_def = (zd_xb && zd_xb[i] == INT_MIN) ? 0 : 1;
                      g_emitter.g_zd_cutd = zd_cut ? zd_cut[i] : -1; g_emitter.g_zd_cpread = 1; g_emitter.g_zd_rdread = 0;
              }
              { g_emitter.g_zd_on_cur = zd_on; g_emitter.g_zd_n_cur = n;
                int _awp = g_emit.op_wpop;
                if (g_planner.codegen_flat_chain_body_nodes[i]->op == IR_MATCH_ALTERNATE || g_planner.codegen_flat_chain_body_nodes[i]->op == IR_MATCH_FENCE1)
                    g_emit.op_wpop = (zd_on[i] && zd_wp[i] > 0) ? zd_wp[i] : 0;
                flat_drive_match_alt(g_planner.codegen_flat_chain_body_nodes, n, i, lbls, betas, na_s, na_f, na_fo, fc_sig, node_γ, node_ω, &lbl_ω); g_emitter.g_zd_on_cur = NULL;
                g_emit.op_wpop = _awp; }
              if (g_emitter.codegen_flat_chain_body__dd_0 && emit_text_count() == _p0)
                  fprintf(stderr, "[DRIVE-DIAG] ZERO-EMIT chain=%d i=%d op=%s n_operands=%d (match_alt path)\n", id, i, bb_op_name(g_planner.codegen_flat_chain_body_nodes[i]->op),
                  g_planner.codegen_flat_chain_body_nodes[i]->n_operands)
                  ; }
            continue;
        }
        if (g_planner.codegen_flat_chain_body_nodes[i]->op == IR_LIMIT) {
            g_emitter.g_limit_gen_beta = _pl->zn[i].t0x >= 0 ? betas[_pl->zn[i].t0x] : NULL;
            g_emitter.g_limit_t0_pop = _pl->zn[i].t0p;
            g_emit.lbl_t1_p = (bb_label_t *)0; g_emit.lbl_t1 = (const char *)0;
            if (_pl->zn[i].lgx >= 0) { g_emit.lbl_t1_p = betas[_pl->zn[i].lgx]; g_emit.lbl_t1 = betas[_pl->zn[i].lgx]->name; }
        }
        if (g_planner.codegen_flat_chain_body_nodes[i]->op == IR_SCAN) { const znode_t * _s = &_pl->zn[i];
            g_emitter.g_scan_body_beta = _s->sbk < 0 ? NULL : _s->sbb ? betas[_s->sbk] : lbls[_s->sbk]; g_emitter.g_scan_t0_pop = _s->sbp;
        }
        { if (g_emitter.codegen_flat_chain_body__zpf < 0) { const char *_e = getenv("SCRIP_ZPOP_FOLD_OFF"); g_emitter.codegen_flat_chain_body__zpf = (_e && _e[0] == '1') ? 1 : 0; }
          if (!g_emitter.codegen_flat_chain_body__zpf) {
              int _fk = -1; { auto _bit = beta_of.find(node_ω); if (_bit != beta_of.end()) _fk = _bit->second; }
              if (_fk >= 0) { const znode_t * _w = &_pl->zn[_fk];
                  if (_w->wek == ZW_NAFO) node_ω = na_fo[_w->wex]; else if (_w->wek == ZW_BETA) node_ω = betas[_w->wex]; else if (_w->wek == ZW_ALPHA) node_ω = lbls[_w->wex];
                  else if (_w->wek == ZW_GAMMA) node_ω = &lbl_γ; else if (_w->wek == ZW_OMEGA) node_ω = &lbl_ω; else if (_w->wek == ZW_REPALT) node_ω = ra_t[_w->wex];
                  g_emit.op_wpop = _w->ws; } else g_emit.op_wpop = 0;
          } }
        { if (g_emitter.codegen_flat_chain_body__dd_1 < 0) { const char *_e = getenv("SCRIP_DRIVE_DIAG"); g_emitter.codegen_flat_chain_body__dd_1 = (_e && _e[0] == '1') ? 1 : 0; }
          long _p0 = g_emitter.codegen_flat_chain_body__dd_1 ? emit_text_count() : 0;
          g_emit.op_scan = 0; g_emit.op_scan_head_off = -1;
          if (g_planner.codegen_flat_chain_body_nodes[i]->op == IR_MATCH_DEFER) {
              for (; _hk_c < 0 && _hk_upto < n; _hk_upto++) if (g_planner.codegen_flat_chain_body_nodes[_hk_upto]->op == IR_MATCH_BEGIN) _hk_c = _hk_upto;
              int _hk = _hk_c;
              if (_hk >= 0) { IR_t *_gt = g_planner.codegen_flat_chain_body_nodes[_hk]->γ.node; { int _gg = 0; while (_gt && _gt->op == IR_GOTO && _gg++ < 128) _gt = _gt->γ.node; }
                  if (_gt == g_planner.codegen_flat_chain_body_nodes[i] && node_ω == betas[_hk]) { g_emit.op_scan = 1;
                      g_emit.op_scan_head_off = (int)drive_value_slot(g_planner.codegen_flat_chain_body_nodes[_hk]); } }
          }
          int _uw_stolen = 0; long _uw_pop = 0;
          { if (g_emitter.codegen_flat_chain_body__uw < 0) { const char *_e = getenv("SCRIP_UNWIND"); g_emitter.codegen_flat_chain_body__uw = (_e && *_e == '0') ? 0 : 1; }
            int _h3 = i;
            if (g_emitter.codegen_flat_chain_body__uw && g_emit.op_wpop == 0 && !omega_is_retry && !omega_is_phi && _pl->zn[i].uwj >= 0 && _pl->zn[i].uwd >= 0) {
                node_ω = betas[_pl->zn[i].uwj]; _uw_stolen = 1; _uw_pop = _pl->zn[i].uwd; } }
          int _endj_stolen = 0;
          {
            int _h2 = i;
            if (!_uw_stolen && g_emit.op_wpop == 0 && !omega_is_retry && !omega_is_phi && _pl->zn[i].endj >= 0 && node_ω == lbls[_pl->zn[i].otx]) { node_ω = lbls[_pl->zn[i].endj]; _endj_stolen = 1;
                g_emit.op_wpop = 0; } }
          bb_label_t *_zw5_saved_omega = NULL; int _zw5_wpop_stolen = 0;
          { int _h = i;
          if (g_emitter.codegen_flat_chain_body__zw5_on && !_endj_stolen && !_uw_stolen && g_planner.codegen_flat_chain_body_nodes[i]->op != IR_STATEMENT &&
              g_planner.codegen_flat_chain_body_nodes[i]->op != IR_STATEMENT_END && zd_on[i] && zd_wp[i] > 0 && !omega_is_retry && !omega_is_phi) { int _lo =
              0, _hi = zw5_pool_stmts; while (_lo < _hi) { int _m = (_lo + _hi) / 2; if (zw5_stmt[_m].idx > i) _hi = _m; else _lo = _m + 1; } if (_lo < zw5_pool_stmts) { int _sb = zw5_stmt[_lo].base;
              for (int _d = 0; _d < zw5_stmt[_lo].cnt; _d++) { if (zw5_depth[_sb + _d] == zd_wp[i]) { _zw5_saved_omega = node_ω; node_ω = &zw5_pool[_sb + _d]; _zw5_wpop_stolen = zd_wp[i]; break;
              } } } } }
          g_emit.op_trap_drop = (long)_pl->zn[i].trapd;
          g_emit.op_zrun = _pl->zn[i].zrunb + _sigk + _sph + (g_emit.flat_gen ? _pl->zn[i].xbp : 0);
          g_emit.op_zxb = g_emit.flat_gen ? 0 : _pl->zn[i].xbp;
          g_emit.op_vdj_live = zd_vl[i];
          g_emit.op_wk_off = _pl->zn[i].wkoff; g_emit.op_wk_n = _pl->zn[i].wkn; g_emit.op_wk_e = _pl->zn[i].wke; g_emit.op_wk_pre = _sigk + _sph; g_emit.op_wk_pool = _pl->v_wk.data();
          g_emitter.g_zd_fence_exit =
              (zd_arm && zd_arm[i] >= 0 && g_planner.codegen_flat_chain_body_nodes[zd_arm[i]]->op == IR_MATCH_FENCE1 &&
              _pl->zn[i].gt == g_planner.codegen_flat_chain_body_nodes[zd_arm[i]]) ? 1 : 0;
          if (zd_on[i] || zd_gp[i] > 0 || zd_wp[i] > 0) { g_emitter.g_zd_stage = 1; g_emitter.g_zd_arm = zd_on[i] ? 1 : 0; g_emitter.g_zd_gpop = zd_gp[i];
              g_emitter.g_zd_bgrow = zd_on[i] && zd_arm[i] >= 0 &&
                  (g_planner.codegen_flat_chain_body_nodes[zd_arm[i]]->op == IR_MATCH_ALTERNATE || _pl->zn[i].armrbp) &&
                  _pl->zn[i].gt == g_planner.codegen_flat_chain_body_nodes[zd_arm[i]];
              g_emitter.g_zd_wpop = (_uw_stolen ? (int)_uw_pop : ((_zw5_wpop_stolen || _endj_stolen) ? 0 : zd_wp[i])); g_emitter.g_zd_wsteal = _endj_stolen || _uw_stolen;
              g_emitter.g_zd_k = zd_on[i] ? _pl->zn[i].k : 0;
              if (zd_on[i] && _pl->zn[i].isusp) g_emitter.g_zd_suspend_uclaim = _pl->zn[i].suc;
              { g_emitter.g_zd_abs = _pl->zn[i].zabs; g_planner.g_zd_cur = i;
                  g_emitter.g_zd_def = _pl->zn[i].zdef; g_emitter.g_zd_cutd = _pl->zn[i].zcut; g_emitter.g_zd_cpread = 1; g_emitter.g_zd_rdread = 1; } }
          if (g_emit_cfg && n_alt > 0) {
              if (otgt && otgt == g_emit_cfg->alt_fail && alt_step_lbl) node_ω = alt_step_lbl;
              if (gtgt && gtgt == g_emit_cfg->alt_fail && alt_step_lbl) node_γ = alt_step_lbl;
              if (!g_emit_cfg->root_graph) for (int _ak = 0; _ak < n_alt; _ak++) if (gtgt && gtgt == g_emit_cfg->alt_ret[_ak] && g_emit_cfg->alt_redo[_ak]) { node_γ =
                  g_emitter.codegen_flat_chain_body_ret_tr[_ak]; break; } }
          if (g_planner.codegen_flat_chain_body_nodes[i]->op == IR_MATCH_BEGIN) { g_emit.lbl_t0 = na_f[i] ? na_f[i]->name : NULL; g_emit.lbl_t0_p = na_f[i]; g_emit.lbl_t0o_p = na_fo[i]; }
          if (_spf && node_ω == &lbl_ω && zd_on[i]) { int _wd = zd_out[i] - _pl->zn[i].k - g_emit.op_wpop - (g_emitter.g_zd_stage ? g_emitter.g_zd_wpop : 0);
              if (_wd > 0) { int _k = 0;
              while (_k < _wpn && _wpd[_k] != _wd) _k++; if (_k == _wpn) { _wpd[_wpn] = _wd; _wpl[_wpn++] = emit_label_alloc("%s_\xcf\x89%d", prefix, _wd); } node_ω = _wpl[_k];
              } }
          if (_spf && node_ω == &lbl_ω && !zd_on[i]) { int _wd = _pl->zn[i].arr;
              if (_wd > 0) { int _k = 0; while (_k < _wpn && _wpd[_k] != _wd) _k++; if (_k == _wpn) { _wpd[_wpn] = _wd; _wpl[_wpn++] = emit_label_alloc("%s_\xcf\x89%d", prefix, _wd);
                  } node_ω = _wpl[_k]; } }
          g_emit.op_head_spine = _sph > 0; g_emit.op_head_rsp = _pl->zn[i].hrsp;
                  { IR_t * _cb = g_planner.codegen_flat_chain_body_nodes[i]; g_emit.op_catch_resume_p = (bb_label_t *)0;
                    if (_cb->op == IR_BOUND && (IR_LIT(_cb).ival & 64)) for (int _q = 0; _q < n && !g_emit.op_catch_resume_p; _q++) { IR_t * _u = g_planner.codegen_flat_chain_body_nodes[_q];
                        if (_u->op == IR_UNMARK && IR_LIT(_u).ival == 4 && _u->n_operands > 0 && _u->operands[0] == _cb) g_emit.op_catch_resume_p = lbls[_q]; } }
                  emit_drive(g_planner.codegen_flat_chain_body_nodes[i], lbls[i], node_γ, node_ω, betas[i]);
              g_emit.op_head_spine = 0;
          if (_zw5_saved_omega) node_ω = _zw5_saved_omega;
          if (g_emitter.codegen_flat_chain_body__zw5_on && g_planner.codegen_flat_chain_body_nodes[i]->op == IR_STATEMENT) { int _lo = 0, _hi = zw5_pool_stmts;
              while (_lo < _hi) { int _m = (_lo + _hi) / 2; if (zw5_stmt[_m].idx >= i) _hi = _m; else _lo = _m + 1;
              } if (_lo < zw5_pool_stmts && zw5_stmt[_lo].idx == i) { int _sb = zw5_stmt[_lo].base; for (int _d = 0; _d < zw5_stmt[_lo].cnt; _d++) { emit_label_define_bb(&zw5_pool[_sb + _d]);
              bb_emit_x86(x86("add", "rsp", (long)zw5_depth[_sb + _d]) + x86("jmp", "extlbl", (uint64_t)(uintptr_t)node_ω)); } } }
          if (g_emitter.codegen_flat_chain_body__dd_1 && emit_text_count() == _p0)
              fprintf(stderr, "[DRIVE-DIAG] ZERO-EMIT chain=%d i=%d op=%s n_operands=%d\n", id, i, bb_op_name(g_planner.codegen_flat_chain_body_nodes[i]->op),
              g_planner.codegen_flat_chain_body_nodes[i]->n_operands); }
    }
    { if (g_emitter.codegen_flat_chain_body__sz_0 < 0) { const char *_e = getenv("SCRIP_ASM_SYMSIZE"); g_emitter.codegen_flat_chain_body__sz_0 = (_e && _e[0] == '0') ? 0 : 1; }
      if (g_emitter.codegen_flat_chain_body__sz_0 && g_is_text && _bx_open >= 0) emit_textf(".size %s, .-%s\n", bxs[_bx_open]->name, bxs[_bx_open]->name); }
    g_emit.x86_uid_kind = (const char *)0;
    g_emit.op_beta_dead = 0;
    g_emit.op_wpop = 0;
    g_emit.op_vdj_live = 0; g_planner.g_zd_plan_on = (const unsigned char *)0; g_planner.g_zd_plan_nodes = (IR_t **)0; g_planner.g_zd_plan_n = 0;
    if (n == 0) emit_jmp_label(flat_empty_body_fail ? &lbl_ω : &lbl_γ, JMP_JMP);
    auto pl_alt_target = [&](int ak) -> bb_label_t * {
        IR_t * _e = g_emit_cfg->alt_entry[ak];
        { int _sg = 0; while (_e && _e->op == IR_SUCCEED && _e->γ.node && _sg++ < 4096) _e = _e->γ.node; }
        if (_e) { int _k = nidx(g_planner.codegen_flat_chain_body_nodes, n, _e); if (_k >= 0) return lbls[_k]; }
        if (_e && _e->op == IR_SUCCEED) return g_emit_cfg->alt_redo[ak] ? g_emitter.codegen_flat_chain_body_ret_tr[ak] : &lbl_γ;
        return &lbl_ω; };
    if (g_emit_cfg && n_alt > 0) {
        int _kt0 = g_emit.flat_frame_bytes; int _np0 = g_emit_cfg->nparams; int _nl0 = g_emit_cfg->nlocals;
        for (int _ak = 0; _ak < n_alt; _ak++) {
            IR_t * _rd = g_emit_cfg->alt_redo[_ak]; bb_label_t * _rb = (bb_label_t *)0;
            if (!_rd || g_emit_cfg->root_graph) continue;
            { int _k = nidx(g_planner.codegen_flat_chain_body_nodes, n, _rd); if (_k >= 0) { _rb = betas[_k]; } }
            if (!_rb) continue;
            emit_sep_rule('-'); emit_label_define_bb(g_emitter.codegen_flat_chain_body_ret_tr[_ak]);
            bb_emit_x86( x86("lea", "rcx", "extlbl", (uint64_t)(uintptr_t)_rb)
                      + x86("mov", RDQ(emit_pl_fb(), _kt0 - 48), "rcx")
                      + x86("mov32", "eax", (long)DT_I)
                      + x86("mov32", "edx", 1L));
            emit_jmp_label(&lbl_γ, JMP_JMP);
        }
        emit_sep_rule('-'); emit_label_define_bb(alt_step_lbl);
        if (emit_pl_scc_armed()) {
            bb_label_t * sccd_ok = emit_label_alloc("%s_sccd", fam);
            bb_emit_x86(x86("mov", "rax", "r12") + x86("and", "rax", X86_PL_TR_ARENA_MASK) + x86("mov", "rcx", RDQ("rax", 32)) + x86("test", "rcx", "rcx"));
            emit_jmp_label(sccd_ok, JMP_JE);
            bb_emit_x86(x86("lea", "rdx", RDQ(emit_pl_fb(), _kt0)) + x86("cmp", "rcx", "rdx"));
            emit_jmp_label(sccd_ok, JMP_JGE);
            bb_emit_x86(x86_bomb("setup_call_cleanup: a clause retry reuses a frame that still holds a linked cleanup record, or one younger"));
            emit_label_define_bb(sccd_ok);
        }
        bb_label_t * pl_step_ball_lbl = emit_label_alloc("%s_step_ball", fam);
        bb_emit_x86(x86("test", "r15", "r15"));
        emit_jmp_label(pl_step_ball_lbl, JMP_JNE);
        bb_label_t * pl_uw_top = emit_label_alloc("%s_uw", fam), * pl_uw_done = emit_label_alloc("%s_uwd", fam);
        bb_emit_x86(x86_pl_tr_undo_prestep(237) + x86("mov", "rsi", RDQ(emit_pl_fb(), _kt0 - 64)));
        emit_label_define_bb(pl_uw_top);
        bb_emit_x86(x86("cmp", "rsi", "r12"));
        emit_jmp_label(pl_uw_done, JMP_JGE);
        bb_emit_x86(x86_pl_tr_pop_head());
        emit_jmp_label(pl_uw_top, JMP_JE);
        bb_emit_x86(x86_pl_tr_pop_restore());
        emit_jmp_label(pl_uw_top, JMP_JMP);
        emit_label_define_bb(pl_uw_done);
        bb_emit_x86( x86_pl_tr_top_sync()
                  + x86("mov", RDQ(emit_pl_fb(), _kt0 - 48), 0L)
                  + pl_step_reseed_locals()
                  + x86("mov", "rax", RDQ(emit_pl_fb(), _kt0 - 56))
                  + x86("test", "rax", "rax"));
        emit_jmp_label(&lbl_ω, JMP_JE);
        bb_emit_x86(x86_jmp_reg("rax"));
        emit_sep_rule('-'); emit_label_define_bb(pl_step_ball_lbl);
        bb_emit_x86(x86("comment",
            "PL EXCEPTION IN FLIGHT (r15 armed): a thrown ball received at a clause's own failure port must propagate past every remaining sibling clause, never retry one -- restore the outer choice"
            " barrier this predicate's own choice point had saved, then concede.")
                  + x86("mov", "r13", RDQ(emit_pl_fb(), _kt0 - 40)));
        emit_jmp_label(&lbl_ω, JMP_JMP);
        for (int _ak = 1; _ak < n_alt; _ak++) {
            int _kt = g_emit.flat_frame_bytes;
            emit_sep_rule('-'); emit_label_define_bb(g_emitter.codegen_flat_chain_body_alt_tr[_ak]);
            if (getenv("SCRIP_ZD_RTCHECK")) {
                bb_label_t _sk; emit_label_initf(&_sk, "%s_stepck", g_emitter.codegen_flat_chain_body_alt_tr[_ak]->name);
                bb_emit_x86(x86("cmp", "rsp", emit_pl_fb())); emit_jmp_label(&_sk, JMP_JE); bb_emit_x86(x86_step_zchk()); emit_label_define_bb(&_sk);
            }
            bb_emit_x86(x86("comment",
                "PL ALTERNATIVE TRAMPOLINE (rung 2, Jcon ir_MoveLabel): advance F.CUR to the NEXT alternative, or zero it on the last one so the step concedes, then enter this clause head unificatio"
                "n.")
                      + pl_clause_zero(_ak)
                      + (_ak + 1 < n_alt ? x86("lea", "rax", "extlbl", (uint64_t)(uintptr_t)g_emitter.codegen_flat_chain_body_alt_tr[_ak + 1]) : x86("xor", "eax", "eax"))
                      + x86("mov", RDQ(emit_pl_fb(), _kt - 56), "rax")
                      + IF(_ak + 1 >= n_alt,
                            x86("mov", "r13", RDQ(emit_pl_fb(), _kt - 40))));
            emit_jmp_label(pl_alt_target(_ak), JMP_JMP);
        }
        if (ix_np) {
            int _kt = g_emit.flat_frame_bytes; bb_label_t * _chain = pl_alt_target(0);
            bb_label_t ** _det = (bb_label_t **)alloca(sizeof(bb_label_t *) * (size_t)n_alt); for (int _ak = 0; _ak < n_alt; _ak++) _det[_ak] = (bb_label_t *)0;
            bb_label_t * _fail = (bb_label_t *)0;
            bb_label_t *** _sentp = (bb_label_t ***)alloca(sizeof(bb_label_t **) * (size_t)ix_np);
            for (int _pp = 0; _pp < ix_np; _pp++) {
            IxP & P = ixp[_pp];
            bb_label_t * _nxt = _pp + 1 < ix_np ? ixp[_pp + 1].sw : _chain;
            bb_label_t ** _sent = (bb_label_t **)alloca(sizeof(bb_label_t *) * (size_t)(P.nset + 1)); for (int _s = 0; _s < P.nset; _s++) _sent[_s] = (bb_label_t *)0; _sentp[_pp] = _sent;
            auto _to = [&](int code) -> bb_label_t * {
                if (code == -2) return _nxt;
                if (code <= -3) { int _s = -3 - code; if (!_sent[_s]) _sent[_s] = emit_label_alloc("%s_ixset%d_p%d", fam, _s, _pp); return _sent[_s]; }
                if (code == -1) { if (!_fail) _fail = emit_label_alloc("%s_ixfail", fam); return _fail; }
                if (!_det[code]) _det[code] = (code == n_alt - 1) ? g_emitter.codegen_flat_chain_body_alt_tr[code] : emit_label_alloc("%s_ixdet%d", fam, code); return _det[code]; };
            pl_ix_arm_t * _arms = (pl_ix_arm_t *)alloca(sizeof(pl_ix_arm_t) * (size_t)(P.n > 0 ? P.n : 1));
            for (int _q = 0; _q < P.n; _q++) { _arms[_q].tag = P.tag[_q]; _arms[_q].val = P.val[_q]; _arms[_q].to = _to(P.code[_q]); }
            bb_label_t * _fbr = _to(P.fbr), * _fba = _to(P.fba), * _fbi = _to(P.fbi);
            emit_sep_rule('-'); emit_label_define_bb(P.sw);
            { extern void xa_pl_switch(int, const pl_ix_arm_t *, int, bb_label_t *, bb_label_t *, bb_label_t *, bb_label_t *); xa_pl_switch(P.slot, _arms, P.n, _fbr, _fba, _fbi, _nxt); }
            }
            for (int _ak = 0; _ak < n_alt - 1; _ak++) {
                if (!_det[_ak]) continue;
                emit_sep_rule('-'); emit_label_define_bb(_det[_ak]);
                bb_emit_x86(x86("comment",
                    "PL SWITCH: this clause is the only one the first argument's key can match -- entered as the chain's last alternative is: no next alternative, B back to the outer choice")
                          + x86("mov", RDQ(emit_pl_fb(), _kt - 56), 0L)
                          + x86("mov", "r13", RDQ(emit_pl_fb(), _kt - 40)));
                emit_jmp_label(pl_alt_target(_ak), JMP_JMP);
            }
            if (_fail) {
                emit_sep_rule('-'); emit_label_define_bb(_fail);
                bb_emit_x86(x86("comment",
                    "PL SWITCH: no clause's first head argument can match the first argument's key -- concede through the step with no next alternative and B back to the outer choice")
                          + x86("mov", RDQ(emit_pl_fb(), _kt - 56), 0L)
                          + x86("mov", "r13", RDQ(emit_pl_fb(), _kt - 40)));
                emit_jmp_label(alt_step_lbl, JMP_JMP);
            }
            for (int _pp = 0; _pp < ix_np; _pp++) {
            IxP & P = ixp[_pp]; bb_label_t ** _sent = _sentp[_pp];
            for (int _s = 0; _s < P.nset; _s++) {
                if (!_sent[_s]) continue;
                const int _m = P.set_len[_s]; const int * _it = P.sets + P.set_beg[_s];
                bb_label_t ** _tl = (bb_label_t **)alloca(sizeof(bb_label_t *) * (size_t)_m);
                for (int _j = 1; _j < _m; _j++) _tl[_j] = emit_label_alloc("%s_ixset%d_p%d_%d", fam, _s, _pp, _j);
                emit_sep_rule('-'); emit_label_define_bb(_sent[_s]);
                bb_emit_x86(x86("comment", "PL SWITCH V2 (ARCH-PROLOG-C-OUT-OF-THE-BOX 11.4): the first argument's key leaves two or more candidate clauses -- the choice stays open (B is this "
                                           "frame's header) and the step walks ONLY this set, in source order, through its own trampolines")
                          + x86("lea", "rax", "extlbl", (uint64_t)(uintptr_t)_tl[1])
                          + x86("mov", RDQ(emit_pl_fb(), _kt - 56), "rax"));
                emit_jmp_label(pl_alt_target(_it[0]), JMP_JMP);
                for (int _j = 1; _j < _m; _j++) {
                    emit_sep_rule('-'); emit_label_define_bb(_tl[_j]);
                    bb_emit_x86(pl_clause_zero(_it[_j])
                              + (_j + 1 < _m ? x86("lea", "rax", "extlbl", (uint64_t)(uintptr_t)_tl[_j + 1]) : x86("xor", "eax", "eax"))
                              + x86("mov", RDQ(emit_pl_fb(), _kt - 56), "rax")
                              + IF(_j + 1 >= _m, x86("mov", "r13", RDQ(emit_pl_fb(), _kt - 40))));
                    emit_jmp_label(pl_alt_target(_it[_j]), JMP_JMP);
                }
            }
            }
        }
    }
    if (g_emit.flat_jmp_entry) {
        if (!bare) {
        if (g_emit.flat_pat) {
            emit_sep_rule('-'); emit_label_define_bb(&lbl_res);
            bb_emit_x86(IF(blob_frame_bytes() > 0, x86_rsp_load64("rbp", 24)) + x86("add", "rsp", 32L));
        } else if (icn_gen_regime() && g_emit.flat_gen) {
            emit_sep_rule('-'); emit_label_define_bb(&lbl_res);
            {
              bb_emit_x86( x86("mov", "rbp", "rax")
                         +
                             IF(x86_trace_taps_on(),
                             x86("mov", "rcx", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_runtime, "g_runtime") + x86("mov", "rcx", RDQ("rcx", GRT_AT(line))) +
                                 x86("shl", "rcx", 1L) +
                             x86("mov", "rdx", RDQ("rax", 64)) + x86("and", "rdx", 1L) + x86("or", "rcx", "rdx") + x86("mov", RDQ("rax", 64), "rcx"))
                         + icn_gen_line_resume() + icn_entry_gva()); }
        }
        }
    }
    if (!bare && !_top_hoist) {
    emit_sep_rule('-'); emit_label_define_bb(&lbl_β);
    if (g_emit_cfg && g_emit_cfg->n_alts > 0) {
        int _kt = g_emit.flat_frame_bytes;
        bb_label_t * _step = alt_step_lbl;
        bb_label_t * _resume = emit_label_alloc("%s_βres", fam);
        if (emit_pl_scc_armed()) {
            bb_label_t * _go = emit_label_alloc("%s_βgo", fam);
            bb_emit_x86(x86("test", "r15", "r15"));
            emit_jmp_label(_go, JMP_JE);
            bb_emit_x86(x86("mov", "rax", "r12") + x86("and", "rax", X86_PL_TR_ARENA_MASK) + x86("mov", "rax", RDQ("rax", 32)) + x86("test", "rax", "rax"));
            emit_jmp_label(&lbl_ω, JMP_JE);
            bb_emit_x86(x86("lea", "rdx", RDQ(emit_pl_fb(), _kt)) + x86("cmp", "rax", "rdx"));
            emit_jmp_label(&lbl_ω, JMP_JGE);
            emit_label_define_bb(_go);
        } else {
            bb_emit_x86( x86("test", "r15", "r15"));
            emit_jmp_label(&lbl_ω, JMP_JNE);
        }
        bb_emit_x86(x86("mov", "rax", RDQ(emit_pl_fb(), _kt - 48)) + x86("mov", RDQ(emit_pl_fb(), _kt - 48), 0L) + x86("test", "rax", "rax"));
        emit_jmp_label(_resume, JMP_JNE);
        emit_jmp_label(_step ? _step : &lbl_ω, JMP_JMP);
        emit_label_define_bb(_resume);
        bb_emit_x86(x86_jmp_reg("rax"));
    } else {
        bb_label_t *resume_tgt = &lbl_ω;
        for (int i = 0; i < n; i++) if (g_planner.codegen_flat_chain_body_nodes[i]->op == IR_SUSPEND) { resume_tgt = betas[i]; break; }
        if (g_emit_cfg && g_emit_cfg->body_root)
            { int i = nidx(g_planner.codegen_flat_chain_body_nodes, n, g_emit_cfg->body_root); if (i >= 0 && (g_planner.codegen_flat_chain_body_nodes[i]->op == IR_DISJUNCTION)) { resume_tgt = lbls[i];
                } }
        if (g_emit_cfg && g_emit_cfg->body_root)
            { int i = nidx(g_planner.codegen_flat_chain_body_nodes, n, g_emit_cfg->body_root);
                if (i >= 0 && (ir_kind0(g_planner.codegen_flat_chain_body_nodes[i]) == IR_CALL_BUILTIN_GEN)) { resume_tgt = betas[i]; } }
        { extern int sn4_defer_resume(void); const int _dres = sn4_defer_resume();
          if (getenv("SCRIP_RESUME_WHY"))
              fprintf(stderr, "[RESUME-GATE] dres=%d flat_pat=%d cfg=%d body_root=%p\n", _dres, g_emit.flat_pat, g_emit_cfg ? 1 : 0, (void *)(g_emit_cfg ? g_emit_cfg->body_root : (IR_t *)0));
          if (_dres && g_emit.flat_pat && g_emit_cfg && g_emit_cfg->body_root) {
            int _f1 = 0; for (int i = 0; i < n; i++) if (g_planner.codegen_flat_chain_body_nodes[i]->op == IR_MATCH_FENCE1) { _f1 = 1; break; }
            int _sd = resume_sealed_defer_on_root(g_planner.codegen_flat_chain_body_nodes, n, g_emit_cfg->body_root);
            int _f1r = _f1; { const char * _fre = getenv("SCRIP_FENCE_RESUME");
                if (!(_fre && *_fre == '0') && g_emit_cfg->body_root && g_emit_cfg->pf.body_seam != 0) _f1r = 0; }
            if (getenv("SCRIP_RESUME_WHY")) { const IR_t * _br = g_emit_cfg->body_root; int _found = 0;
                { int i = nidx(g_planner.codegen_flat_chain_body_nodes, n, _br); if (i >= 0) _found = 1; }
                int _wnc = 0, _wlf = 0, _wfn = 0; sn4_blob_choice_scan(&_wnc, &_wlf, &_wfn);
                fprintf(stderr, "[RESUME-WHY] flat_pat=%d dres=%d f1=%d f1r=%d sd=%d body_root_op=%d tier=%d seal=%d in_nodes=%d n=%d nc=%d lf=%d fn=%d bfb=%d spgd=%d sph=%d fam=%s\n",
                    g_emit.flat_pat, _dres, _f1, _f1r,
                    _sd, _br ? (int)_br->op : -1, _br ? g_emit_cfg->pf.body_seam : -1, _br ? (int)_br->seal : -1, _found, n, _wnc, _wlf, _wfn, blob_frame_bytes(), _spgd, _sph, fam ? fam : "?"); }
            if (!_f1r) { int i = nidx(g_planner.codegen_flat_chain_body_nodes, n, g_emit_cfg->body_root);
                if (i >= 0 && (resume_carrier_ok(g_planner.codegen_flat_chain_body_nodes[i], _sd))) { resume_tgt = betas[i]; } }
            if (!_f1 && !_sd && resume_tgt == &lbl_ω && sn4_alt_carrier() && g_emit_cfg->body_root &&
                (g_emit_cfg->body_root->op == IR_MATCH_ALTERNATE || g_emit_cfg->body_root->op == IR_DISJUNCTION) && !g_emit_cfg->body_root->seal) {
                int _nc = 0, _lf = 0, _fn = 0; sn4_blob_choice_scan(&_nc, &_lf, &_fn);
                if (_nc == 1 && !_fn) { int i = nidx(g_planner.codegen_flat_chain_body_nodes, n, g_emit_cfg->body_root); if (i >= 0) { resume_tgt = betas[i];
                    } } }
            if (!_sd && resume_tgt == &lbl_ω && sn4_alt_carrier()) {
                int _nc = 0, _lf = 0, _fn = 0; sn4_blob_choice_scan(&_nc, &_lf, &_fn);
                if (_nc >= 1) {
                    int _mok = 1;
                    for (int i = 0; i < n && _mok; i++) { int _o = (int)g_planner.codegen_flat_chain_body_nodes[i]->op;
                        if (_o == IR_DISJUNCTION) _mok = 0;
                        else if (_o == IR_MATCH_ALTERNATE && g_planner.codegen_flat_chain_body_nodes[i]->seal) _mok =
                            0; }
                    int _ti = -1;
                    if (_mok) for (int i = 0; i < n; i++) { int _o = (int)g_planner.codegen_flat_chain_body_nodes[i]->op;
                        if (_o == IR_GOTO || _o == IR_SUCCEED || _o == IR_FAIL) continue;
                        IR_t * _tt = _pl->zn[i].gt; if (_tt && _tt->op == IR_SUCCEED) { if (_ti >= 0) { _ti = -2; break; } _ti = i; } }
                    if (_mok && _ti >= 0 && g_planner.codegen_flat_chain_body_nodes[_ti]->op != IR_MATCH_FENCE1 && g_planner.codegen_flat_chain_body_nodes[_ti]->op != IR_MATCH_FENCE0) resume_tgt =
                        betas[_ti];
                } } } }
        long _rpop = 0;
        if (g_emit_cfg && g_emit_cfg->body_root && g_emit_cfg->body_root->op == IR_MATCH_ABORT) { int i = nidx(g_planner.codegen_flat_chain_body_nodes, n, g_emit_cfg->body_root);
            if (i >= 0) { resume_tgt = lbls[i]; int _da = _pl->zn[i].arr;
                if (_spf && _spgd >= 0 && _da >= 0 && _spgd > _da) _rpop = _spgd - _da; } }
        if (resume_tgt == &lbl_ω && _spf && _spgd > 0 && _rpop == 0) _rpop = _spgd;
        if (icn_gen_regime() && g_emit.flat_gen && g_emitter.g_suspend_resume_slot >= 0)
            bb_emit_x86( x86("mov", "rax", FRQ(g_emitter.g_suspend_resume_slot)) + x86_jmp_reg("rax"));
        else { if (_rpop > 0) bb_emit_x86(x86("add", "rsp", _rpop)); emit_jmp_label(resume_tgt, JMP_JMP); }
    }
    }
    { extern int g_flat_frame_floor; int _wire_stub = ((g_emit.flat_jmp_entry && g_flat_frame_floor > 0) || (g_emit.flat_lcl_proc && !(g_emit_cfg && g_emit_cfg->cell_frame))) ? 1 : 0;
    int _blob_wire = (!_wire_stub && g_emit.flat_jmp_entry && g_emit.flat_pat) ? 1 : 0;
    if (!bare && !_top_hoist) {
    emit_sep_rule('-'); emit_label_define_bb(&lbl_γ);
    if (xa_flat_class_c_pred()) {
        int _has_nret = 0; for (int _ni = 0; _ni < n; _ni++) if (ir_floater_kind(g_planner.codegen_flat_chain_body_nodes[_ni]) == 3) { _has_nret = 1; break; }
        if (_has_nret) emit_label_define_bb(emit_floater_label(1));
    }
    if (g_emit.zframe_graph || (g_emit_cfg && g_emit_cfg->cell_frame && g_emit.flat_lcl_proc)) { extern void xa_flat_zframe_epilogue_γ(void); xa_flat_zframe_epilogue_γ(); }
    else if (_blob_wire) {
        int _bfb = blob_frame_bytes(); int _sd = (_bfb > 0) ? 0 : (_spgd >= 0) ? _spgd + _sph : -1;
            if (_bfb <= 0 && _sd < 0) bb_emit_x86(x86_bomb("frameless stored-pattern thunk: its gamma exits leave the spine at different or unknown depths"));
        bb_emit_x86(IF(_bfb > 0,
            x86("mov", "rcx", RDQ("rbp", blob_cont_copy() ? -16 : 16)) + x86("push", "rbp") + x86("push", "rcx") + x86("mov", "rcx", RDQ("rbp", blob_cont_copy() ? -8 : 8)) + x86("push", "rcx") +
            x86_lea_ext("rax", &lbl_res) + x86("push", "rax") + x86("mov", "rbp", RDQ("rbp", 0)) + x86_jmp_reg("rcx")) +
            IF(_bfb <= 0,
            x86("mov", "rdx", RDQ("rsp", _sd + 8)) + x86("mov", "rcx", RDQ("rsp", _sd)) + x86("sub", "rsp", 8L) + x86("push", "rdx") + x86("push", "rcx") + x86_lea_ext("rax", &lbl_res) +
            x86("push", "rax") + x86_jmp_reg("rcx")));
    }
    else if (icn_gen_regime() && g_emit.flat_gen) {
        bb_emit_x86( icn_gen_line_save()
                  + x86("mov", "rdx", "rbp")
                  + x86_lea_ext("rax", &lbl_res) + x86("mov", RDQ("rdx", 32), "rax")
                  + x86("mov", "rcx", RDQ("rdx", 8))
                  + x86("mov", "rbp", RDQ("rdx", 0))
         + x86("mov32", "eax", (long)DT_S) + x86_jmp_reg("rcx"));
    }
    else if (xa_flat_class_c_pred() && !g_runtime.eval.rt_fragment_emit) { sig_prepare_chain(fam); xa_flat_chain_epilogue_sig(1, fam); }
    else { if (xa_flat_class_c_pred()) xa_flat_chain_epilogue(); bb_emit_x86((_wire_stub ? bb_glue_wire_γ() : bb_glue_outer_γ())); }
    }
    if (!bare && !_top_hoist) {
    emit_sep_rule('-'); emit_label_define_bb(&lbl_ω);
    if (g_emit.zframe_graph || (g_emit_cfg && g_emit_cfg->cell_frame && g_emit.flat_lcl_proc)) { extern void xa_flat_zframe_epilogue_ω(void); xa_flat_zframe_epilogue_ω(); }
    else if (_blob_wire) { extern int sn4_blob_casmark(void);
        bb_emit_x86(IF(blob_frame_bytes() > 0, IF(sn4_blob_casmark(), x86("mov", "r12", RDQ("rbp", -40))) + x86("mov", "rsp", "rbp") + x86("pop", "rbp")) +
        IF(blob_frame_bytes() <= 0 && _sph > 0, IF(_sph == 32, x86("mov", "r12", RDQ("rsp", 8))) + x86("add", "rsp", (long)_sph)) +
        (blob_omega_ret() ? x86("add", "rsp", 8L) + x86("ret") : x86_rsp_load64("rcx", 8) + x86("add", "rsp", 16L) + x86("jmp", "rcx")));
        for (int _k = 0; _k < _wpn; _k++) { emit_label_define_bb(_wpl[_k]); bb_emit_x86(x86("add", "rsp", (long)_wpd[_k])); emit_jmp_label(&lbl_ω, JMP_JMP); }
        for (int _k = 0; _k < _tpn; _k++) { emit_label_define_bb(_tpl[_k]); bb_emit_x86(x86("add", "rsp", (long)_tpd[_k])); emit_jmp_label(_tpt[_k], JMP_JMP); } }
    else if (icn_gen_regime() && g_emit.flat_gen) {
        { bb_emit_x86( icn_trace_tap((strncmp(prefix, "proc_", 5) == 0) ? prefix + 5 : prefix, 5, 0)); }
        bb_emit_x86( x86("mov", "rcx", RDQ("rbp", 8))
                  + x86("mov", "rsp", RDQ("rbp", 24))
                  + x86("mov", "rbp", RDQ("rbp", 0))
                  + x86("mov32", "eax", (long)DT_FAIL) + x86_jmp_reg("rcx"));
    }
    else if (xa_flat_class_c_pred() && !g_runtime.eval.rt_fragment_emit) { sig_prepare_chain(fam); xa_flat_chain_epilogue_sig(0, fam); }
    else { if (xa_flat_class_c_pred()) xa_flat_chain_epilogue(); bb_emit_x86((_wire_stub ? bb_glue_wire_ω() : bb_glue_outer_ω())); } } }
    { extern int g_flat_dc_np; extern long g_last_dc_off; g_last_dc_off = -1;
      if (g_flat_dc_np >= 0) {
          if (!g_emit.flat_jmp_entry || !g_emit.flat_lex || g_emit.flat_gen || g_emit.flat_pat)
              { fprintf(stderr, "FATAL PL-DC: driver-armed graph is not det-lexical jmp-entry (jmp=%d lex=%d gen=%d pat=%d)\n", g_emit.flat_jmp_entry, g_emit.flat_lex, g_emit.flat_gen,
              g_emit.flat_pat); abort(); }
          bb_label_t lbl_dc; emit_label_initf(&lbl_dc, "%s_dcα", fam);
          emit_sep_rule('-'); emit_label_define_bb(&lbl_dc);
          g_emit.flat_dc_body_p = _blk_graph ? &lbl_αblk : (g_emit.zframe_graph || (g_emit_cfg && g_emit_cfg->cell_frame && g_emit.flat_lcl_proc)) ? &lbl_α : &lbl_α_body;
          { extern void xa_flat_dc_stub(void); xa_flat_dc_stub(); }
          g_emit.flat_dc_body_p = (bb_label_t *)0;
          if (!g_is_text) g_last_dc_off = (long)lbl_dc.offset;
      } }
    emit_gc_map_data(fam); g_emitter.g_gc_sites_armed = 0;
    if (text_externalise && g_is_text) {
        data_buf_flush_pending_label();
        xa_dispatch(XA_FLAT_DATA_SECTION);
        data_buf_reset();
    }
    return 0;
}
long g_last_dc_off = -1;
}
