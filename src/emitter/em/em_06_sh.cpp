#include "emitter/em/em_head.h"
#include "emitter/em/em_01_sh.h"
#include "emitter/em/em_02_sh.h"
#include "emitter/em/em_03_sh.h"
#include "emitter/em/em_04_sh.h"
#include "emitter/em/em_06_sh.h"
extern "C" { extern int is_global(const char *name); }
extern "C" { extern int gva_index_of(const char *name); }
extern "C" {
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void drive_no_template(IR_t *nd) {
    fprintf(stderr,
        "FATAL emit_drive: IR op=%d has NO TEMPLATE in the universal driver -- the switch carries no case for it at all. Every op must be handled; the driver never refuses silently. Implement op=%d."
        "\n",
            nd ? (int)nd->op : -1, nd ? (int)nd->op : -1);
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void emit_drive(IR_t *nd, bb_label_t *lbl_α, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    if (!nd) { drive_guard_refused(nd, __LINE__); return; }
    switch (nd->op) {
    case IR_LIT_STRING: case IR_LIT_DSTAR: case IR_LIT_THUNK: case IR_LIT_INTEGER: case IR_LIT_REAL:
        g_emit.op_off = drive_value_slot(nd); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    case IR_LIT_CHARSET:
    case IR_LIT_NAME:
    case IR_LIT_ATOM:
        g_emit.op_sval = IR_LIT(nd).sval; g_emit.op_off = drive_value_slot(nd); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    case IR_KW_ICON:
    case IR_KW_ICON_GEN:
    case IR_KW_SNOBOL4:
        g_emit.op_sval = IR_LIT(nd).sval; g_emit.op_off = drive_value_slot(nd); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    case IR_KW_ASSIGN:
    case IR_KW_ASSIGN_SNOBOL4:
        g_emit.op_sval = IR_LIT(nd).sval; g_emit.op_off = drive_value_slot(nd); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    case IR_VAR: {
        const char *vn = IR_LIT(nd).sval;
        if (vn && vn[0] == '&') { g_emit.op_sval = vn; g_emit.op_sa = -1; g_emit.op_off = drive_value_slot(nd); }
        else if (vn && is_global(vn) && !graph_has_local(g_emit_cfg, vn)) { g_emit.op_sa = -1; g_emit.op_off = drive_value_slot(nd); g_emit.op_sval = vn;
            g_emit.op_gva_k = g_gva_active ? gva_index_of(vn) : -1; }
        else if (vn) { int voff = zrow(nd)->vso; g_emit.op_sa = voff; g_emit.op_off = (voff != -1) ? drive_value_slot(nd) : -1; }
        else { g_emit.op_sa = -1; g_emit.op_off = -1; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_VAR_REF: {
        const char *vn = IR_LIT(nd).sval; g_emit.op_var_named = nd->pat_static;
        if (vn && is_global(vn) && !graph_has_local(g_emit_cfg, vn)) { g_emit.op_sa = -1; g_emit.op_off = drive_value_slot(nd); g_emit.op_sval = vn;
            g_emit.op_gva_k = g_gva_active ? gva_index_of(vn) : -1; }
        else if (vn) { int voff = zrow(nd)->vso; g_emit.op_sa = voff; g_emit.op_off = drive_value_slot(nd); }
        else { g_emit.op_sa = -1; g_emit.op_off = -1; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_BINOP: case IR_BINOP_TEST: case IR_BINOP_RELOP_VAL: {
        g_emit.op_relop_descr = 0; g_emit.op_num_real = 0; g_emit.op_arith_descr = 0; g_emit.op_gva_k1 = -1; g_emit.op_gva_k2 = -1;
        g_emit.op_imm_a_ok = 0; g_emit.op_imm_b_ok = 0; g_emit.op_imm_a = 0; g_emit.op_imm_b = 0;
        g_emit.op_snul_a_ok = ir_value_is_null_string(bb_child0(nd)); g_emit.op_snul_b_ok = ir_value_is_null_string(bb_child1(nd));
        int sa = -1, sb = -1;
        if (binop_is_num_real(g_emit_cfg, nd)) { int ra = bb_slot_get(bb_child0(nd)), rb = bb_slot_get(bb_child1(nd)); if (ra >= 0 && rb >= 0) { sa = ra; sb = rb; g_emit.op_num_real = 1; } }
        if (!g_emit.op_num_real) { sa = emit_binop_opnd_slot(bb_child0(nd), zrow_opoff(nd, 0), zrow_opvs(nd, 0)); sb = emit_binop_opnd_slot(bb_child1(nd), zrow_opoff(nd, 1), zrow_opvs(nd, 1)); }
        if (sa < 0 || sb < 0) { drive_guard_refused(nd, __LINE__); break; }
        if (!g_emit.op_num_real) {
            IR_t * la = bb_child0(nd), * lb = bb_child1(nd);
            if (la && la->op == IR_LIT_INTEGER && IR_LIT(la).ival >= -2147483648LL && IR_LIT(la).ival <= 2147483647LL) { g_emit.op_imm_a_ok = 1; g_emit.op_imm_a = (long)IR_LIT(la).ival; }
            if (lb && lb->op == IR_LIT_INTEGER && IR_LIT(lb).ival >= -2147483648LL && IR_LIT(lb).ival <= 2147483647LL) { g_emit.op_imm_b_ok = 1; g_emit.op_imm_b = (long)IR_LIT(lb).ival; }
        }
        g_emit.op_sa = sa; g_emit.op_sb = sb; g_emit.op_off = drive_value_slot(nd); g_emit.op_binop_kind = (int)binop_slot_kind(nd);
        DRIVE_PAIR_RESET(); DRIVE_PAIR_DEF_JMP(lbl_β, lbl_ω); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_UNOP: case IR_UNOP_TEST: case IR_NULLTEST_VAR: {
        int sa = emit_binop_opnd_slot(bb_child0(nd), zrow_opoff(nd, 0), zrow_opvs(nd, 0));
        if (sa < 0) { drive_guard_refused(nd, __LINE__); break; }
        g_emit.op_sa = sa; g_emit.op_off = drive_value_slot(nd); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_FIELD_GET: case IR_FIELD_VAR: {
        IR_t * obj = bb_child0(nd);
        int sa = obj ? emit_binop_opnd_slot(obj, zrow_opoff(nd, 0), zrow_opvs(nd, 0)) : -1;
        if (sa < 0) { drive_guard_refused(nd, __LINE__); break; }
        g_emit.op_a_slot = sa; g_emit.op_sval = IR_LIT(nd).sval; g_emit.op_off = drive_value_slot(nd); g_emit.op_seal = nd->seal;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_ASSIGN: {
        const char *vn = IR_LIT(nd).sval;
        if (!vn) {
            fprintf(stderr, "FATAL emit_drive IR_ASSIGN guard: nameless %d-operand assign (assign-through-lvalue-producer: !x/?x element-variable or "
                            "s[i:j] section) — LOWER's TT_ASSIGN terminal arm minted a placeholder; not a missing template. ASSIGN-LV rung, GOAL-IR-IMMUTABLE-EMIT.md.\n", nd->n_operands);
            abort();
        }
        if (is_global(vn) && !graph_has_local(g_emit_cfg, vn)) {
            g_emit.op_sb = -1; g_emit.op_sval = vn; g_emit.op_gva_k = g_gva_active ? gva_index_of(vn) : -1;
            g_emit.op_off = drive_value_slot(nd); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
        }
        { int voff = zrow(nd)->vso;
          if (voff == -1)
              { fprintf(stderr, "[TE-4] IR_ASSIGN local '%s' has no LOWER-granted varslot — grant it in planner_cells_graph (src/planner/planner_stage.c), never allocate in the emitter\n", vn);
              abort();
              }
          g_emit.op_sb = voff; }
        g_emit.op_off = drive_value_slot(nd); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_VAR_FRAME:
    case IR_ASSIGN_FRAME:
        g_emit.op_off = drive_value_slot(nd); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    case IR_CALL: case IR_CALL_BUILTIN: case IR_CALL_BUILTIN_GEN: case IR_CALL_PROC_STAGED: case IR_PROC_GEN: case IR_CALL_PL_BLOCK:
    case IR_CALL_DIRECT: case IR_CALL_PL_LEAF: case IR_CALL_PAS_BI: case IR_CALL_CALLEE_REC: case IR_CALL_FIELD_REC: case IR_CALL_BYNAME: case IR_CALL_TRY: case IR_CALL_METHOD: case IR_CALL_REDISP:
    case IR_CALL_APPLY: case IR_CALL_EVAL: case IR_CALL_GEN:
    case IR_CALL_ICON: case IR_CALL_SNOBOL4: {
        int na = nd->n_operands; drive_arg_slots_reserve(na);
        for (int i = 0; i < na; i++) { IR_t * a = ir_call_arg(nd, i); g_emit.op_arg_slot[i] = a ? zrow_opoff(nd, i) : -1; }
        g_emit.op_arg_slot_n = na;
        { IR_t *_gt = nd->γ.node; int _gg = 0; while (_gt && _gt->op == IR_GOTO && _gg++ < 128) _gt = _gt->γ.node; IR_t *_ot = nd->ω.node; int _og = 0;
            while (_ot && _ot->op == IR_GOTO && _og++ < 128) _ot = _ot->γ.node;
            g_emit.op_sb = (g_emit_cfg && g_emit_cfg->spine_cells && _gt && _ot && _gt == _ot && (unsigned char)nd->ω.sz[0] == 0xce && (unsigned char)nd->ω.sz[1] == 0xb2) ? 1 : 0; }
        DRIVE_PAIR_RESET(); DRIVE_PAIR_DEF_JMP(lbl_β, lbl_ω); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_BEGIN: {
        IR_t *subj = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        g_emit.op_sa = subj ? drive_value_slot(subj) : -1;
        g_emit.op_subj_cell = (subj && zrow(nd)->vrfp >= 0) ? 1 : 0;
        g_emit.op_off = drive_value_slot(nd);
        g_emit.op_frame_extra = zrow(nd)->aext;
        DRIVE_PAIR_RESET();
        if (g_emit.lbl_t0_p) { for (int _q = 0; _q < 3; _q++) { xa_pair_push(NULL, NULL); } xa_pair_push(g_emit.lbl_t0_p, NULL); g_emit.lbl_t0_p = NULL; g_emit.lbl_t0 = NULL;
          xa_pair_push(g_emit.lbl_t0o_p, NULL); g_emit.lbl_t0o_p = NULL; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_END: {
        IR_t *hd = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        g_emit.op_off = hd ? drive_value_slot(hd) : -1;
        g_emit.op_frame_extra = zrow(nd)->aexth;
        if (getenv("SCRIP_EDRIVE_END_DIAG")) fprintf(stderr, "[EDRIVE-END] op_off=%d op_fc_disp(pre)=%d op_tail(pre)=%d\n", g_emit.op_off, g_emit.op_fc_disp, g_emit.op_tail);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_GOTO_DEFERRED: {
        g_emit.op_sval = IR_LIT(nd).sval; g_emit.op_ival = nd->seal; DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_REPLACE: {
        IR_t *hd = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        IR_t *rp = nd->n_operands > 1 ? nd->operands[1] : (IR_t *)0;
        IR_t *sb = nd->n_operands > 2 ? nd->operands[2] : (IR_t *)0;
        g_emit.op_off = hd ? drive_value_slot(hd) : -1;
        g_emit.op_frame_extra = zrow(nd)->aexth;
        g_emit.op_sb = rp ? drive_value_slot(rp) : -1;
        g_emit.op_sa = sb ? drive_value_slot(sb) : -1;
        g_emit.op_sval = IR_LIT(nd).sval;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_ASSIGN_COND: {
        IR_t *box = nd->n_operands > 1 ? nd->operands[1] : (IR_t *)0;
        g_emit.op_off = box ? drive_value_slot(box) : -1; g_emit.op_phase = 1;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_ASSIGN_IMM: {
        IR_t *box = nd->n_operands > 1 ? nd->operands[1] : (IR_t *)0;
        g_emit.op_off = box ? drive_value_slot(box) : -1; g_emit.op_phase = 2;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_ASSIGN_SAVE: {
        g_emit.op_off = drive_value_slot(nd); g_emit.op_phase = 0;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_FENCE0: {
        g_emit.op_off = drive_value_slot(nd); DRIVE_PAIR_RESET(); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β);
        break;
    }
    case IR_MATCH_FENCE1: {
        drive_guard_refused(nd, __LINE__);
        break;
    }
    case IR_MATCH_ABORT: {
        g_emit.op_off = -1; DRIVE_PAIR_RESET(); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_TRAP: {
        g_emit.op_off = -1; DRIVE_PAIR_RESET(); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_ALTERNATE: case IR_MATCH_ARBNO: case IR_SCAN_ALTERNATE: {
        drive_guard_refused(nd, __LINE__);
        break;
    }
    case IR_COERCE_STRING: case IR_COERCE_INTEGER: case IR_COERCE_REAL: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        int sa = a0 ? emit_binop_opnd_slot(a0, zrow_opoff(nd, 0), zrow_opvs(nd, 0)) : -1;
        if (sa < 0) { drive_guard_refused(nd, __LINE__); break; }
        g_emit.op_sa = sa; g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_COERCE_NUMERIC: case IR_CMP_TEST: case IR_IDENT: case IR_DIFFER: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        IR_t * a1 = nd->n_operands > 1 ? nd->operands[1] : (nd->op == IR_COERCE_NUMERIC ? a0 : (IR_t *)0);
        int sa = a0 ? emit_binop_opnd_slot(a0, zrow_opoff(nd, 0), zrow_opvs(nd, 0)) : -1;
            int sb = a1 ? emit_binop_opnd_slot(a1, zrow_opoff(nd, nd->n_operands > 1 ? 1 : 0), zrow_opvs(nd, nd->n_operands > 1 ? 1 : 0)) : -1;
        if (sa < 0 || sb < 0) { drive_guard_refused(nd, __LINE__); break; }
        g_emit.op_sa = sa; g_emit.op_sb = sb; g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_LEN: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        g_emit.op_sa = a0 ? bb_slot_get(a0) : -1; if (a0 && g_emit.op_sa < 0) { int _zsa = zrow(nd)->o0off; if (_zsa >= 0) { g_emit.op_sa = _zsa; g_emit.op_zres = 1;
            } } g_emit.op_off = -1;
        if (a0 && g_emit.op_sa < 0) { drive_guard_refused(nd, __LINE__); break; }
        { const char * _sv = (nd->n_operands == 0 && (uintptr_t)(uint64_t)IR_LIT(nd).ival > (uintptr_t)0xFFFFU) ? IR_LIT(nd).sval : (const char *)0;
            g_emit.op_sval = (_sv && _sv[0] == '*') ? _sv : NULL; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_LIT: {
        g_emit.op_sa = -1; g_emit.op_off = -1;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_ATP: {
        g_emit.op_sa = -1; g_emit.op_off = -1;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_CASMARK: {
        g_emit.op_sa = -1; g_emit.op_off = -1;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_ANY: case IR_MATCH_NOTANY: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        g_emit.op_sa = a0 ? bb_slot_get(a0) : -1; g_emit.op_off = -1;
        if (a0 && g_emit.op_sa < 0) { drive_guard_refused(nd, __LINE__); break; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_SPAN: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        g_emit.op_sa = a0 ? bb_slot_get(a0) : -1;
        if (a0 && g_emit.op_sa < 0) { drive_guard_refused(nd, __LINE__); break; }
        g_emit.x86_scratch_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_RKRUN: case IR_MATCH_RKCAPS: case IR_MATCH_RKCAPE: { DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break; }
    case IR_MATCH_RKLAND: { DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break; }
    case IR_MATCH_BREAK: case IR_MATCH_BREAKX: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        g_emit.op_sa = a0 ? bb_slot_get(a0) : -1;
        if (a0 && g_emit.op_sa < 0) { drive_guard_refused(nd, __LINE__); break; }
        g_emit.x86_scratch_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_TAB: case IR_MATCH_RTAB: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        g_emit.op_off = -1;
        if (!a0) { g_emit.op_sb = (int)IR_LIT(nd).ival; g_emit.op_sa = -1; }
        else if (a0->op == IR_LIT_INTEGER) { g_emit.op_sb = (int)IR_LIT(a0).ival; g_emit.op_sa = -1; }
        else { int sl = bb_slot_get(a0); if (sl < 0) { int _zsa = zrow(nd)->o0off; if (_zsa >= 0) { sl = _zsa; g_emit.op_zres = 1;
            } } if (sl < 0) { drive_guard_refused(nd, __LINE__); break; } g_emit.op_sa = sl; g_emit.op_sb = 0; }
        { const char * _sv = (nd->n_operands == 0 && (uintptr_t)(uint64_t)IR_LIT(nd).ival > (uintptr_t)0xFFFFU) ? IR_LIT(nd).sval : (const char *)0;
            g_emit.op_sval = (_sv && _sv[0] == '*') ? _sv : NULL; }
        g_emit.x86_scratch_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_LAMBDA: {
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_POS: case IR_MATCH_RPOS: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        g_emit.op_off = -1;
        if (!a0) { g_emit.op_sb = (int)IR_LIT(nd).ival; g_emit.op_sa = -1; }
        else if (a0->op == IR_LIT_INTEGER) { g_emit.op_sb = (int)IR_LIT(a0).ival; g_emit.op_sa = -1; }
        else { int sl = bb_slot_get(a0); if (sl < 0) { int _zsa = zrow(nd)->o0off; if (_zsa >= 0) { sl = _zsa; g_emit.op_zres = 1;
            } } if (sl < 0) { drive_guard_refused(nd, __LINE__); break; } g_emit.op_sa = sl; g_emit.op_sb = 0; }
        { const char * _sv = (nd->n_operands == 0 && (uintptr_t)(uint64_t)IR_LIT(nd).ival > (uintptr_t)0xFFFFU) ? IR_LIT(nd).sval : (const char *)0;
            g_emit.op_sval = (_sv && _sv[0] == '*') ? _sv : NULL; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_REM: case IR_MATCH_ARB: case IR_MATCH_BAL: {
        g_emit.x86_scratch_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_VALUE: {
        g_emit.op_sa = -1; g_emit.op_off = -1;
        g_emit.x86_scratch_off = drive_value_slot(nd);
        g_emit.bb_child_fn = (void *) 0; g_emit.bb_child_lbl = (const char *) 0;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_DEFER: {
        g_emit.op_sa = -1; g_emit.op_off = -1;
        g_emit.x86_scratch_off = drive_value_slot(nd);
        g_emit.bb_child_fn = (void *) 0; g_emit.bb_child_lbl = (const char *) 0;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_TO: {
        if (!bb_child0(nd) || !bb_child1(nd)) { drive_guard_refused(nd, __LINE__); break; }
        g_emit.op_sa = drive_value_slot(bb_child0(nd)); g_emit.op_sb = drive_value_slot(bb_child1(nd));
        g_emit.op_num_real = (IR_LIT(nd).sval && strcmp(IR_LIT(nd).sval, "ar") == 0) ? 1 : 0;
        g_emit.op_db_walk = ir_range_kind_is(nd, "db"); g_emit.op_to_trailed = ir_range_trailed(nd); g_emit.op_to_db_last = ir_range_tag_has(nd, 'd', 'e', 't');
        g_emit.op_sc = (g_emit.op_db_walk && nd->n_operands > 2 && nd->operands[2]) ? drive_value_slot(nd->operands[2]) : -1;
        g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_TO_BY: {
        if (!bb_child0(nd) || !bb_child1(nd) || nd->n_operands < 3 || !nd->operands[2]) { drive_guard_refused(nd, __LINE__); break; }
        g_emit.op_sa = drive_value_slot(bb_child0(nd)); g_emit.op_sb = drive_value_slot(bb_child1(nd));
        g_emit.op_sc = drive_value_slot(nd->operands[2]);
        g_emit.op_num_real = (IR_LIT(nd).sval && strcmp(IR_LIT(nd).sval, "ar") == 0) ? 1 : 0;
        g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MAKE_LIST: {
        int na = nd->n_operands; drive_arg_slots_reserve(na);
        for (int i = 0; i < na; i++) { IR_t * a = nd->operands[i]; g_emit.op_arg_slot[i] = a ? drive_value_slot(a) : -1; }
        g_emit.op_arg_slot_n = na;
        g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_REV_ASSIGN: {
        IR_t * lv = nd->n_operands > 1 ? nd->operands[1] : NULL;
        const char * vn = lv ? IR_LIT(lv).sval : NULL;
        if (!vn || nd->n_operands < 1 || !nd->operands[0]) { drive_guard_refused(nd, __LINE__); break; }
        { extern int is_global(const char *);
          if (vn[0] == '&' || (is_global(vn) && !graph_has_local(g_emit_cfg, vn))) { g_emit.op_sb = -1; g_emit.op_sval = vn; g_emit.op_gva_k = (vn[0] != '&' && g_gva_active) ? gva_index_of(vn) : -1; }
          else { int voff = zrow_opvs(nd, 1);
            if (voff == -1) { fprintf(stderr,
                "[TE-4] IR_REV_ASSIGN '%s': not a global (is_global checked, so not the "
                "bb_rev_assign_global path) and no LOWER-granted varslot as a local — "
                "grant it in planner_cells_graph, never in the emitter\n", vn); abort(); }
            g_emit.op_sb = voff; } }
        g_emit.op_off = drive_value_slot(nd);
        g_emit.op_sc = g_emit.op_off + 16;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_REV_SWAP: {
        const char * ln = IR_LIT(nd).sval;
        const char * rn = (nd->n_operands > 0 && nd->operands[0]) ? IR_LIT(nd->operands[0]).sval : NULL;
        if (!ln || !rn) { drive_guard_refused(nd, __LINE__); break; }
        g_emit.op_sval = ln; g_emit.op_name2 = rn;
        g_emit.op_sb = -1; g_emit.op_sa = -1;
        { extern int is_global(const char *);
          if (ln[0] != '&' && !(is_global(ln) && !graph_has_local(g_emit_cfg, ln))) { int voff = zrow(nd)->vso;
              if (voff == -1)
              { fprintf(stderr,
              "[TE-4] IR_REV_SWAP lhs '%s' is neither a global (is_global says no, so not the NV_PTR_fn by-name path) nor a local with a LOWER-granted varslot — grant it in planner_cells_graph (s"
              "crip_ir.c), never allocate in the emitter\n", ln); abort(); } g_emit.op_sb = voff; }
          if (rn[0] != '&' && !(is_global(rn) && !graph_has_local(g_emit_cfg, rn))) { int voff = zrow_opvs(nd, 0);
              if (voff == -1)
              { fprintf(stderr,
              "[TE-4] IR_REV_SWAP rhs '%s' is neither a global (is_global says no, so not the NV_PTR_fn by-name path) nor a local with a LOWER-granted varslot — grant it in planner_cells_graph (s"
              "crip_ir.c), never allocate in the emitter\n", rn); abort(); } g_emit.op_sa = voff; } }
        g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_REV_ASSIGN_VAR: {
        IR_t * v = nd->n_operands > 0 ? nd->operands[0] : NULL;
        IR_t * r = nd->n_operands > 1 ? nd->operands[1] : NULL;
        int sa = v ? drive_value_slot(v) : -1; int sb = r ? drive_value_slot(r) : -1;
        if (sa < 0 || sb < 0) { drive_guard_refused(nd, __LINE__); break; }
        g_emit.op_sa = sb;
        g_emit.op_off = drive_value_slot(nd);
        g_emit.op_sc = g_emit.op_off + 16;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_GOTO:
        DRIVE_PAIR_RESET(); DRIVE_PAIR_JMP(lbl_γ); DRIVE_PAIR_DEF_JMP(lbl_β, lbl_ω); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    case IR_BOUND: {
        g_emit.op_sb = 1; g_emit.op_off = zrow(nd)->loff; g_emit.op_fc_bytes = 0;
        if (g_emit.op_off < 0) { drive_guard_refused(nd, __LINE__); break; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_TABLE_HDR: {
        g_emit.op_off = zrow(nd)->loff; g_emit.op_fc_bytes = 0;
        if (g_emit.op_off < 0) { drive_guard_refused(nd, __LINE__); break; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_UNMARK: {
        IR_t * _mk = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        g_emit.op_sb = 0; g_emit.op_off = _mk ? zrow(nd)->o0off : -1; g_emit.op_fc_bytes = 0;
        if (g_emit.op_off < 0) { drive_guard_refused(nd, __LINE__); break; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_CONJUNCTION:
        { int dst = zrow(nd)->owno; if (dst >= 0 && nd->n_operands > 0 && nd->operands[0]) {
            int src = bb_slot_get(nd->operands[0]);
            if (src >= 0 && dst >= 0 && src != dst && !(g_emit_cfg && g_emit_cfg->spine_cells && g_emit_cfg->pf.home_frame && zrow(nd)->kown == 0)) {
                g_emit.op_sa = src; g_emit.op_off = dst;
            } else {
                g_emit.op_sa = -1; g_emit.op_off = -1;
            }
        } else {
            g_emit.op_sa = -1; g_emit.op_off = -1;
        } }
        DRIVE_PAIR_RESET(); DRIVE_PAIR_JMP(lbl_γ); DRIVE_PAIR_DEF_JMP(lbl_β, lbl_ω); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    case IR_SUCCEED:
    case IR_CUT:
        DRIVE_PAIR_RESET(); DRIVE_PAIR_JMP(lbl_γ); DRIVE_PAIR_DEF_JMP(lbl_β, lbl_ω); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    case IR_DEFINE_BIND: case IR_DEFINE_ADOPT: case IR_DEFINE_SHIM: case IR_DEFINE_RT_SHIM: case IR_DEFINE_RETURN: case IR_DEFINE_FRETURN: case IR_DEFINE_CALL:
        if (ir_define_sr_citizen(nd)) {
        { int64_t v = IR_LIT(nd).ival; int64_t r = (v == 1 || v == 2 || v == 3 || v == 4) ? v : 0; g_emit.op_ival = r; g_emit.op_sval = r ? (const char *)0 : IR_LIT(nd).sval; }
        int na = nd->n_operands; drive_arg_slots_reserve(na);
        for (int i = 0; i < na; i++) { IR_t * a = nd->operands[i]; int s = a ? bb_slot_get(a) : -1; if (s < 0 && a) s = zrow_opoff(nd, i); g_emit.op_arg_slot[i] = s; }
        g_emit.op_arg_slot_n = na;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
        if (!ir_define_is_bind(nd)) { drive_guard_refused(nd, __LINE__); break; }
        DRIVE_PAIR_RESET(); DRIVE_PAIR_JMP(lbl_γ); DRIVE_PAIR_DEF_JMP(lbl_β, lbl_ω); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    case IR_STATEMENT_BEGIN:
    case IR_STATEMENT_END:
    case IR_STMT_MARK:
    case IR_SETEXIT_TEST:
    case IR_STATEMENT:
        g_emit.op_fc_bytes = 0;
        DRIVE_PAIR_RESET(); DRIVE_PAIR_JMP(lbl_γ); DRIVE_PAIR_DEF_JMP(lbl_β, lbl_ω); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    case IR_LINE_MARK:
        DRIVE_PAIR_RESET(); DRIVE_PAIR_JMP(lbl_γ); DRIVE_PAIR_DEF_JMP(lbl_β, lbl_ω); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    case IR_SUSPEND: {
        g_emit.op_activate_proc = IR_LIT(nd).sval;
        IR_t * ev = bb_child0(nd); int sa = ev ? bb_slot_get(ev) : -1;
        if (sa < 0 && ev) sa = zrow(nd)->o0off;
        if (sa < 0) { drive_guard_refused(nd, __LINE__); break; }
        g_emit.op_sa = sa; g_emit.lbl_t0 = g_emitter.g_suspend_dobody_beta ? g_emitter.g_suspend_dobody_beta->name : NULL; g_emit.lbl_t0_p = g_emitter.g_suspend_dobody_beta;
        g_emit.lbl_t1_p = lbl_β; g_emit.lbl_t1 = lbl_β ? lbl_β->name : NULL;
        g_emit.op_sb = g_emitter.g_suspend_resume_slot;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SUBSCRIPT: {
        if (nd->n_operands == 2) {
            IR_t * vb = nd->operands[0]; IR_t * vi = nd->operands[1];
            int va = vb ? drive_value_slot(vb) : -1; int vs = vi ? drive_value_slot(vi) : -1;
            if (va < 0 || vs < 0) { drive_guard_refused(nd, __LINE__); break; }
            g_emit.op_a_slot = va; g_emit.op_sa = vs; g_emit.op_off = drive_value_slot(nd); g_emit.op_seal = nd->seal;
            DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
        }
        IR_t * base = nd->n_operands > 0 ? nd->operands[0] : NULL;
        IR_t * i1 = nd->n_operands > 1 ? nd->operands[1] : NULL;
        IR_t * i2 = nd->n_operands > 2 ? nd->operands[2] : NULL;
        int sa = base ? drive_value_slot(base) : -1;
        int sb = i1 ? drive_value_slot(i1) : -1;
        int sc = i2 ? drive_value_slot(i2) : -1;
        if (sa < 0 || sb < 0 || sc < 0) { drive_guard_refused(nd, __LINE__); break; }
        g_emit.op_a_slot = sa; g_emit.op_sa = sb; g_emit.op_sb = sc;
        g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_RANDOM: {
        IR_t * v = nd->n_operands > 0 ? nd->operands[0] : NULL;
        int sa = v ? drive_value_slot(v) : -1;
        if (sa < 0) { drive_guard_refused(nd, __LINE__); break; }
        g_emit.op_a_slot = sa; g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_UNIFY_CONST: case IR_UNIFY_STRUCT: case IR_UNIFY_FIRST: case IR_UNIFY_VALUE:
        g_emit.op_off = drive_value_slot(nd); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    case IR_DEREF: {
        IR_t * v = nd->n_operands > 0 ? nd->operands[0] : NULL;
        int sa = v ? drive_value_slot(v) : -1;
        if (sa < 0) { drive_guard_refused(nd, __LINE__); break; }
        g_emit.op_a_slot = sa; g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_ASSIGN_VAR: {
        if (nd->n_operands == 3) {
            IR_t * b = nd->operands[0]; IR_t * ix = nd->operands[1]; IR_t * v = nd->operands[2];
            int sb2 = b ? drive_value_slot(b) : -1; int si = ix ? drive_value_slot(ix) : -1; int sv = v ? drive_value_slot(v) : -1;
            if (sb2 < 0 || si < 0 || sv < 0) { drive_guard_refused(nd, __LINE__); break; }
            g_emit.op_a_slot = sb2; g_emit.op_sa = si; g_emit.op_sb = sv; g_emit.op_off = drive_value_slot(nd);
            DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
        }
        IR_t * v = nd->n_operands > 0 ? nd->operands[0] : NULL;
        IR_t * r = nd->n_operands > 1 ? nd->operands[1] : NULL;
        int sa = v ? drive_value_slot(v) : -1; int sb = r ? drive_value_slot(r) : -1;
        if (sa < 0 || sb < 0) { drive_guard_refused(nd, __LINE__); break; }
        g_emit.op_a_slot = sa; g_emit.op_sa = sb; g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SWAP: {
        IR_t * xv = nd->n_operands > 0 ? nd->operands[0] : NULL;
        IR_t * yv = nd->n_operands > 1 ? nd->operands[1] : NULL;
        int sa = xv ? zrow_opvs(nd, 0) : -1;
        int sb = yv ? zrow_opvs(nd, 1) : -1;
        if (sa < 0 || sb < 0) { drive_guard_refused(nd, __LINE__); break; }
        g_emit.op_sa = sa; g_emit.op_sb = sb; g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_PROC_VALUE: {
        g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_CALL_VALUE: {
        IR_t * cal = nd->n_operands > 0 ? nd->operands[0] : NULL;
        int sc = cal ? drive_value_slot(cal) : -1;
        int na2 = nd->n_operands - 1;
        if (sc < 0 || na2 < 0) { drive_guard_refused(nd, __LINE__); break; }
        drive_arg_slots_reserve(na2);
        for (int i = 0; i < na2; i++) { IR_t * a = nd->operands[i + 1]; g_emit.op_arg_slot[i] = a ? drive_value_slot(a) : -1; }
        g_emit.op_arg_slot_n = na2;
        g_emit.op_sval = IR_LIT(nd).sval;
        g_emit.op_sa = sc; g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SWAP_VAR: {
        IR_t * xv = nd->n_operands > 0 ? nd->operands[0] : NULL;
        IR_t * yv = nd->n_operands > 1 ? nd->operands[1] : NULL;
        int sa = xv ? drive_value_slot(xv) : -1; int sb = yv ? drive_value_slot(yv) : -1;
        if (sa < 0 || sb < 0) { drive_guard_refused(nd, __LINE__); break; }
        g_emit.op_a_slot = sa; g_emit.op_sa = sb; g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_LIMIT_GATE: {
        IR_t * cnt = nd->n_operands > 0 ? nd->operands[0] : NULL;
        int sc = cnt ? drive_value_slot(cnt) : -1;
        if (sc < 0) { drive_guard_refused(nd, __LINE__); break; }
        g_emit.op_sa = sc; g_emit.op_off = drive_value_slot(nd); g_emit.op_sb = (g_emit_cfg && g_emit_cfg->pf.home_frame) ? g_emit.op_off + 16 : -1;
        g_emit.op_sc = (g_emit.op_sb >= 0 && zrow(nd)->hwhy) ? g_emit.op_sb + 16 : -1;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_LIMIT: {
        IR_t * gen = nd->n_operands > 0 ? nd->operands[0] : NULL;
        IR_t * cnt = nd->n_operands > 1 ? nd->operands[1] : NULL;
        int sa = gen ? drive_value_slot(gen) : -1;
        int sc = cnt ? drive_value_slot(cnt) : -1;
        { const zeta_plan_t * _pr = g_planner.g_zd_rec_p; int _pi = _pr ? zrow_of(nd) : -1;
            if (_pi >= 0 && _pr->rd && _pr->rdo && _pr->rdo[_pi + 1] > _pr->rdo[_pi] && _pr->rd[_pr->rdo[_pi]].key >= 0) sa = _pr->rd[_pr->rdo[_pi]].key; }
        if (sa < 0 || sc < 0) { drive_guard_refused(nd, __LINE__); break; }
        g_emit.op_sa = sa; g_emit.op_sc = sc;
        g_emit.lbl_t0 = g_emitter.g_limit_gen_beta ? g_emitter.g_limit_gen_beta->name : (lbl_ω ? lbl_ω->name : NULL);
        g_emit.lbl_t0_p = g_emitter.g_limit_gen_beta ? g_emitter.g_limit_gen_beta : lbl_ω;
        g_emit.op_t0_pop = g_emitter.g_limit_t0_pop;
        g_emit.op_off = drive_value_slot(nd);
        g_emit.op_sb = -1;
        for (int _gk = 2; g_emit_cfg && g_emit_cfg->pf.home_frame && _gk < nd->n_operands; _gk++) if (nd->operands[_gk] && nd->operands[_gk]->op == IR_LIMIT_GATE) g_emit.op_sb =
            drive_value_slot(nd->operands[_gk]) + 16;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_CREATE: {
        g_emit.op_off = drive_value_slot(nd);
        g_emit.op_activate_proc = IR_LIT(nd).sval;
        g_emit.lbl_t0 = g_create_body_entry ? g_create_body_entry->name : NULL;
        g_emit.lbl_t0_p = g_create_body_entry;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_ACTIVATE: {
        IR_t * ce = nd->n_operands > 0 ? nd->operands[0] : NULL;
        IR_t * xv = nd->n_operands > 1 ? nd->operands[1] : NULL;
        int sa = ce ? bb_slot_get(ce) : -1;
        if (sa < 0 && ce) sa = zrow(nd)->o0off;
        if (sa < 0) { drive_guard_refused(nd, __LINE__); break; }
        g_emit.op_sa = sa;
        g_emit.op_sb = xv ? bb_slot_get(xv) : -1;
        if (g_emit.op_sb < 0 && xv) g_emit.op_sb = zrow_opoff(nd, 1);
        g_emit.op_activate_proc = IR_LIT(nd).sval;
        g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_CORET: {
        IR_t * pv = nd->n_operands > 0 ? zrow(nd)->cvn : NULL;
        int sa = pv ? bb_slot_get(pv) : -1;
        if (sa < 0 && pv) { int _ns = zrow(nd)->cvnoff; if (_ns >= 0) sa = _ns; }
        if (sa < 0) { drive_guard_refused(nd, __LINE__); break; }
        if (pv->op == IR_CONJUNCTION && zrow(nd)->cvnoff >= 0) sa = zrow(nd)->cvnoff;
        g_emit.op_sa = sa;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_COFAIL: {
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_GATE: {
        g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_GATE_ARM: {
        IR_t * ig = nd->n_operands > 1 ? nd->operands[1] : NULL;
        g_emit.op_off = ig ? drive_value_slot(ig) : -1;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_DISJUNCTION: {
        if (nd->n_operands > 0) { drive_guard_refused(nd, __LINE__); break; }
        int off = drive_value_slot(nd);
        g_emit.op_off = off;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_ITERATE: {
        IR_t * obj = nd->n_operands > 0 ? nd->operands[0] : NULL;
        int sa = obj ? bb_slot_get(obj) : -1;
        if (sa < 0 && obj) sa = zrow(nd)->o0off;
        if (sa < 0) { drive_guard_refused(nd, __LINE__); break; }
        g_emit.op_sa = sa;
        g_emit.op_off = drive_value_slot(nd);
        g_emit.op_sb = g_emit.op_off + 16;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_INITIAL: {
        g_emit.op_off = zrow(nd)->loff;
        if (g_emit.op_off < 0) { drive_guard_refused(nd, __LINE__); break; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_ENTER: {
        IR_t * subj = nd->n_operands > 0 ? nd->operands[0] : NULL;
        int sa = subj ? bb_slot_get(subj) : -1;
        if (sa < 0 && subj) sa = zrow(nd)->o0off;
        if (sa < 0) { drive_guard_refused(nd, __LINE__); break; }
        g_emit.op_sa = sa; g_emit.op_sb = 1;
        g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN: {
        g_emit.op_sb = 0;
        IR_t * enter_nd = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = zrow(nd)->o0off;
        if (g_emit.op_off < 0) { drive_guard_refused(nd, __LINE__); break; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_TAB: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = drive_value_slot(nd);
        if (!a0) { drive_guard_refused(nd, __LINE__); break; }
        if (a0->op == IR_LIT_INTEGER) { g_emit.op_sb = (int)IR_LIT(a0).ival; g_emit.op_sa = -1; }
        else { int sl = bb_slot_get(a0); if (sl < 0) sl = zrow(nd)->o0off; if (sl < 0) { drive_guard_refused(nd, __LINE__); break; } g_emit.op_sa = sl; g_emit.op_sb = 0; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_MOVE: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = drive_value_slot(nd);
        if (!a0) { drive_guard_refused(nd, __LINE__); break; }
        if (a0->op == IR_LIT_INTEGER) { g_emit.op_sb = (int)IR_LIT(a0).ival; g_emit.op_sa = -1; }
        else { int sl = bb_slot_get(a0); if (sl < 0) sl = zrow(nd)->o0off; if (sl < 0) { drive_guard_refused(nd, __LINE__); break; } g_emit.op_sa = sl; g_emit.op_sb = 0; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_POS: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = drive_value_slot(nd);
        if (!a0) { drive_guard_refused(nd, __LINE__); break; }
        if (a0->op == IR_LIT_INTEGER) { g_emit.op_sb = (int)IR_LIT(a0).ival; g_emit.op_sa = -1; }
        else { int sl = bb_slot_get(a0); if (sl < 0) sl = zrow(nd)->o0off; if (sl < 0) { drive_guard_refused(nd, __LINE__); break; } g_emit.op_sa = sl; g_emit.op_sb = 0; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_UPTO: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = drive_value_slot(nd);
        if (a0 && (a0->op == IR_LIT_STRING || a0->op == IR_LIT_CHARSET) && IR_LIT(a0).sval) {
            g_emit.op_name1 = IR_LIT(a0).sval; g_emit.op_sa = -1;
        } else if (a0) {
            int sl = bb_slot_get(a0); if (sl < 0) sl = zrow(nd)->o0off; if (sl < 0) { drive_guard_refused(nd, __LINE__); break; } g_emit.op_sa = sl; g_emit.op_name1 = NULL;
        } else { drive_guard_refused(nd, __LINE__); break; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_ANY: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = drive_value_slot(nd);
        if (a0 && (a0->op == IR_LIT_STRING || a0->op == IR_LIT_CHARSET) && IR_LIT(a0).sval) {
            g_emit.op_name1 = IR_LIT(a0).sval; g_emit.op_sa = -1;
        } else if (a0) {
            int sl = bb_slot_get(a0); if (sl < 0) sl = zrow(nd)->o0off; if (sl < 0) { drive_guard_refused(nd, __LINE__); break; } g_emit.op_sa = sl; g_emit.op_name1 = NULL;
        } else { drive_guard_refused(nd, __LINE__); break; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_MANY: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = drive_value_slot(nd);
        if (a0 && (a0->op == IR_LIT_STRING || a0->op == IR_LIT_CHARSET) && IR_LIT(a0).sval) {
            g_emit.op_name1 = IR_LIT(a0).sval; g_emit.op_sa = -1;
        } else if (a0) {
            int sl = bb_slot_get(a0); g_emit.op_sa = sl; g_emit.op_name1 = NULL;
        } else { g_emit.op_name1 = NULL; g_emit.op_sa = -1; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_FIND: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = drive_value_slot(nd);
        int64_t _fl = (a0 && a0->op == IR_LIT_STRING && IR_LIT(a0).sval) ?
            ((a0->n_operands > 0 && a0->operands[0] && a0->operands[0]->op == IR_LIT_INTEGER) ? IR_LIT(a0->operands[0]).ival : (int64_t)strlen(IR_LIT(a0).sval)) : -1;
        if (_fl >= 1 && _fl <= 32) {
            g_emit.op_name1 = IR_LIT(a0).sval; g_emit.op_sa = -1;
        } else if (a0) {
            int sl = bb_slot_get(a0); if (sl < 0) sl = zrow(nd)->o0off; if (sl < 0) { drive_guard_refused(nd, __LINE__); break; } g_emit.op_sa = sl; g_emit.op_name1 = NULL;
        } else { drive_guard_refused(nd, __LINE__); break; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_MATCH: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = drive_value_slot(nd);
        if (a0 && a0->op == IR_LIT_STRING && IR_LIT(a0).sval) {
            g_emit.op_name1 = IR_LIT(a0).sval; g_emit.op_sa = -1;
        } else if (a0) {
            int sl = bb_slot_get(a0); if (sl < 0) sl = zrow(nd)->o0off; if (sl < 0) { drive_guard_refused(nd, __LINE__); break; } g_emit.op_sa = sl; g_emit.op_name1 = NULL;
        } else { drive_guard_refused(nd, __LINE__); break; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_BAL: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = drive_value_slot(nd);
        if (a0 && (a0->op == IR_LIT_STRING || a0->op == IR_LIT_CHARSET) && IR_LIT(a0).sval) {
            g_emit.op_name1 = IR_LIT(a0).sval; g_emit.op_sa = -1;
        } else if (a0) {
            int sl = bb_slot_get(a0); if (sl < 0) sl = zrow(nd)->o0off; if (sl < 0) { drive_guard_refused(nd, __LINE__); break; } g_emit.op_sa = sl; g_emit.op_name1 = NULL;
        } else { drive_guard_refused(nd, __LINE__); break; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_REPALT: {
        drive_guard_refused(nd, __LINE__); break;
    }
    case IR_FAIL:
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    case IR_RETURN:
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    default:
        drive_no_template(nd); break;
    }
}
extern "C" int emit_zrow_xabs(const IR_t * nd) {
    const zeta_plan_t * r = g_planner.g_zd_rec_p; if (!r || !nd) return INT_MIN;
    int ri = (nd == g_emit.node && g_emit.op_zrow >= 0 && g_emit.op_zrow < r->n && r->nodes[g_emit.op_zrow] == nd) ? g_emit.op_zrow : zrow_of(nd);
    return (ri < 0 || !r->on[ri] || !r->xb || r->xb[ri] == INT_MIN) ? INT_MIN : r->xb[ri] + r->out[ri];
}
}
