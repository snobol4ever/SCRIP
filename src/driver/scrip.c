#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <ctype.h>
#include <errno.h>
#include <libgen.h>
#include <setjmp.h>
#include <time.h>
#include <unistd.h>
#include <gc.h>
#include "../parser/snobol4/scrip_cc.h"
#include "../parser/snocone/snocone_driver.h"
#include "../parser/prolog/prolog_driver.h"
#include "../parser/prolog/term.h"
#include "../parser/prolog/prolog_runtime.h"
#include "../parser/prolog/prolog_atom.h"
#include "../parser/prolog/prolog_builtin.h"
#include "../parser/icon/icon_driver.h"
#include "../parser/pascal/pascal_driver.h"
#include "../parser/raku/raku_driver.h"
#include "../parser/rebus/rebus_lower.h"
#include "../runtime/builtins/gen.h"
#include "../parser/icon/icon_lex.h"
#include "../include/bb_box.h"
extern void ir_print_node   (const tree_t *e, FILE *f);
extern void ir_print_node_nl(const tree_t *e, FILE *f);
extern int pl_dyn_is_marked(const char *name, int arity);
#include "core.h"
#include "sil_macros.h"
#include "runtime_shim.h"
#include "lower.h"
#include "emit.h"
#include "bb_build.h"
#include "scrip_sm.h"
extern DESCR_t pat_at_cursor(const char *varname);
static void stmt_init(void) {}
extern DESCR_t      eval_expr(const char *src);
extern int exec_stmt(const char *subj_name,
                          DESCR_t    *subj_var,
                          DESCR_t     pat,
                          DESCR_t    *repl,
                          int         has_repl);
extern const char *Σ;
extern int         Ω;
extern int         Δ;
#include "../runtime/builtins/gen_runtime.h"
#include "../runtime/builtins/resolution.h"
#include "driver/polyglot.h"
#include "../tools/emit_per_kind_audit.h"
/*====================================================================================================================*/
/*====================================================================================================================*/
static int keyword_supported(const char *kw) {
    if (!kw) return 0;
    if (kw[0] == '&') kw++;
    return !strcmp(kw, "subject") || !strcmp(kw, "pos") || !strcmp(kw, "null") || !strcmp(kw, "fail");
}
static int scan_safe_kind(IR_e t) {
    return t == IR_SUCCEED || t == IR_FAIL ||
           t == IR_LIT_INTEGER || t == IR_LIT_STRING || t == IR_LIT_REAL || t == IR_OP_COUNT ||
           t == IR_VAR || t == IR_KEYWORD || t == IR_OP_COUNT || t == IR_CALL || ir_is_scan_kind(t) || t == IR_BINOP || t == IR_OP_COUNT || t == IR_CONJ || t == IR_ASSIGN || t == IR_OP_COUNT || t == IR_OP_COUNT || t == IR_OP_COUNT;
}
static int sg_var_assigned(IR_graph_t *sg, const char *name) {
    if (!sg || !sg->all || !name) return 0;
    for (int i = 0; i < sg->n; i++) { IR_t *m = sg->all[i]; if (m && m->op == IR_ASSIGN && IR_LIT(m).sval && !strcmp(IR_LIT(m).sval, name)) return 1; }
    return 0;
}
static IR_t *scan_lit_entry(IR_t *nd, IR_e want) {
    IR_graph_t **sblks = (IR_graph_t **)0;
    IR_t *ae = (sblks && (int)IR_LIT(nd).ival == 1 && sblks[0]) ? sblks[0]->entry : (IR_t *)0;
    if (!ae || ae->op != want) return (IR_t *)0;
    if (ae->γ.node && ae->γ.node->op != IR_SUCCEED) return (IR_t *)0;
    return ae;
}
static int scan_fn_lit_arg(IR_t *nd, IR_e want) {
    return scan_lit_entry(nd, want) != (IR_t *)0;
}
static int scan_fn_cset_arg(IR_t *nd) {
    extern const char *kw_cset_const_str(const char *kw);
    if (scan_lit_entry(nd, IR_LIT_STRING) != (IR_t *)0) return 1;
    IR_graph_t **sblks = (IR_graph_t **)0;
    IR_t *ae = (sblks && (int)IR_LIT(nd).ival == 1 && sblks[0]) ? sblks[0]->entry : (IR_t *)0;
    if (!ae || ae->op != IR_KEYWORD) return 0;
    if (ae->γ.node && ae->γ.node->op != IR_SUCCEED) return 0;
    return kw_cset_const_str(IR_LIT(ae).sval) != (const char *)0;
}
static int graph_var_assigned_or_param(stage2_t *s2, int gi, IR_graph_t *g, const char *name);
static int scan_any_cset_var_ok(stage2_t *s2, int gi, IR_graph_t *g, IR_t *nd) {
    IR_graph_t **sblks = (IR_graph_t **)0;
    IR_t *ae = (sblks && (int)IR_LIT(nd).ival == 1 && sblks[0]) ? sblks[0]->entry : (IR_t *)0;
    if (!ae || ae->op != IR_VAR || !IR_LIT(ae).sval || IR_LIT(ae).sval[0] == '&') return 0;
    if (ae->γ.node && ae->γ.node->op != IR_SUCCEED) return 0;
    return graph_var_assigned_or_param(s2, gi, g, IR_LIT(ae).sval);
}
static int scan_tab_arg_ok(IR_t *nd) {
    IR_graph_t **sblks = (IR_graph_t **)0;
    IR_t *ae = (sblks && (int)IR_LIT(nd).ival == 1 && sblks[0]) ? sblks[0]->entry : (IR_t *)0;
    if (!ae) return 0;
    if (ae->γ.node && ae->γ.node->op != IR_SUCCEED) return 0;
    if (ae->op == IR_LIT_INTEGER && IR_LIT(ae).ival >= 1) return 1;
    if ((ae->op == IR_CALL || ir_is_scan_kind(ae->op)) && IR_LIT(ae).dval == 3.0 && IR_LIT(ae).sval && (!strcmp(IR_LIT(ae).sval, "any") || !strcmp(IR_LIT(ae).sval, "many") || !strcmp(IR_LIT(ae).sval, "upto")) && scan_fn_cset_arg(ae)) return 1;
    if ((ae->op == IR_CALL || ir_is_scan_kind(ae->op)) && IR_LIT(ae).dval == 3.0 && IR_LIT(ae).sval && (!strcmp(IR_LIT(ae).sval, "match") || !strcmp(IR_LIT(ae).sval, "find") || !strcmp(IR_LIT(ae).sval, "bal")) && scan_fn_lit_arg(ae, IR_LIT_STRING)) return 1;
    return 0;
}
static int graph_var_assigned_or_param(stage2_t *s2, int gi, IR_graph_t *g, const char *name);
static int scan_subgraph_safe(stage2_t *s2, int gi, IR_graph_t *g, IR_graph_t *sg, int depth) {
    if (!sg || !sg->all || sg->n <= 0 || depth > 16) return 0;
    for (int i = 0; i < sg->n; i++) {
        IR_t *nd = sg->all[i];
        if (!nd) continue;
        if (!scan_safe_kind(nd->op)) return 0;
        if (nd->op == IR_VAR) {
            if (IR_LIT(nd).sval && IR_LIT(nd).sval[0] == '&') { if (!keyword_supported(IR_LIT(nd).sval)) return 0; }
            else if (!IR_LIT(nd).sval || (!graph_var_assigned_or_param(s2, gi, g, IR_LIT(nd).sval) && !sg_var_assigned(sg, IR_LIT(nd).sval))) return 0;
        }
        if (nd->op == IR_ASSIGN) { if (!IR_LIT(nd).sval || is_global(IR_LIT(nd).sval)) return 0; }
        if (nd->op == IR_KEYWORD && !keyword_supported(IR_LIT(nd).sval)) return 0;
        if (nd->op == IR_CALL || ir_is_scan_kind(nd->op)) {
            if (!IR_LIT(nd).sval) return 0;
            if (!strcmp(IR_LIT(nd).sval, "any")) { if (!(IR_LIT(nd).dval == 3.0 && (scan_fn_cset_arg(nd) || scan_any_cset_var_ok(s2, gi, g, nd)))) return 0; }
            else if (!strcmp(IR_LIT(nd).sval, "many") || !strcmp(IR_LIT(nd).sval, "upto")) { if (!(IR_LIT(nd).dval == 3.0 && scan_fn_cset_arg(nd))) return 0; }
            else if (!strcmp(IR_LIT(nd).sval, "match")) { if (!(IR_LIT(nd).dval == 3.0 && scan_fn_lit_arg(nd, IR_LIT_STRING))) return 0; }
            else if (!strcmp(IR_LIT(nd).sval, "tab")) { if (!(IR_LIT(nd).dval == 3.0 && scan_tab_arg_ok(nd))) return 0; }
            else if (!strcmp(IR_LIT(nd).sval, "move")) { if (!(IR_LIT(nd).dval == 3.0 && scan_fn_lit_arg(nd, IR_LIT_INTEGER))) return 0; }
            else if (!strcmp(IR_LIT(nd).sval, "pos")) { IR_t *pe = scan_lit_entry(nd, IR_LIT_INTEGER); if (!(IR_LIT(nd).dval == 3.0 && pe && IR_LIT(pe).ival >= 1)) return 0; }
            else if (!strcmp(IR_LIT(nd).sval, "find")) { IR_t *fe = scan_lit_entry(nd, IR_LIT_STRING); if (!(IR_LIT(nd).dval == 3.0 && fe && IR_LIT(fe).sval && IR_LIT(fe).sval[0] && strlen(IR_LIT(fe).sval) <= 32)) return 0; }
            else if (!strcmp(IR_LIT(nd).sval, "bal")) { IR_t *be = scan_lit_entry(nd, IR_LIT_STRING); if (!(IR_LIT(nd).dval == 3.0 && be && IR_LIT(be).sval && IR_LIT(be).sval[0] && !strchr(IR_LIT(be).sval, 40) && !strchr(IR_LIT(be).sval, 41))) return 0; }
            else if (!(!strcmp(IR_LIT(nd).sval, "write") || !strcmp(IR_LIT(nd).sval, "writes"))) return 0;
        }
        if (nd->op == IR_BINOP) { int64_t bc = IR_LIT(nd).ival; int is_rel = (bc >= BINOP_LT && bc <= BINOP_NE) || (bc >= BINOP_SLT && bc <= BINOP_SNE); if (bc != BINOP_CONCAT && !is_rel) return 0; }
        if (nd->op == IR_OP_COUNT) {
            IR_graph_t *ssg = (IR_graph_t *)0;
            IR_graph_t *bsg = (IR_graph_t *) 0;
            if (!scan_subgraph_safe(s2, gi, g, ssg, depth + 1) || !scan_subgraph_safe(s2, gi, g, bsg, depth + 1)) return 0;
        }
    }
    return 1;
}
static int graph_native_emittable_mode(stage2_t *s2, int for_run);
static int graph_native_emittable(stage2_t *s2) { return graph_native_emittable_mode(s2, 0); }
static void icn_register_record_types(stage2_t *s2) {
    extern void *dat_register(const char *spec);
    extern void *dat_find_type(const char *name);
    if (!s2) return;
    for (int gi = 0; gi < s2->bbp.count; gi++) {
        IR_graph_t *g = s2->bbp.table[gi];
        if (!g || !g->all) continue;
        for (int ni = 0; ni < g->n; ni++) {
            IR_t *nd = g->all[ni];
            if (nd && nd->op == IR_OP_COUNT && IR_LIT(nd).sval && !dat_find_type(IR_LIT(nd).sval)) dat_register(IR_LIT(nd).sval);
        }
    }
}
static int gen_scan_body_slotful(IR_t *r) {
    if (!r || r->op != IR_OP_COUNT || IR_LIT(r).dval != 1.0) return 0;
    IR_graph_t *bsg = (IR_graph_t *) 0;
    IR_t *bt = bsg ? bsg->entry : (IR_t *)0;
    int gd = 0;
    while (bt && bt->γ.node && bt->γ.node->op != IR_SUCCEED && bt->γ.node->op != IR_FAIL && gd++ < 512) bt = bt->γ.node;
    if (bt && (bt->op == IR_LIT_INTEGER || bt->op == IR_LIT_STRING)) return 1;
    if (bt && bt->op == IR_VAR && IR_LIT(bt).sval && IR_LIT(bt).sval[0] != '&') return 1;
    if (bt && (bt->op == IR_CALL || ir_is_scan_kind(bt->op)) && IR_LIT(bt).dval == 3.0 && IR_LIT(bt).sval && (!strcmp(IR_LIT(bt).sval, "tab") || !strcmp(IR_LIT(bt).sval, "move") || !strcmp(IR_LIT(bt).sval, "pos") || !strcmp(IR_LIT(bt).sval, "any") || !strcmp(IR_LIT(bt).sval, "match") || !strcmp(IR_LIT(bt).sval, "many") || !strcmp(IR_LIT(bt).sval, "upto") || !strcmp(IR_LIT(bt).sval, "find") || !strcmp(IR_LIT(bt).sval, "bal"))) return 1;
    return 0;
}
static int rhs_kind_ok(IR_t *r) {
    if (!r) return 0;
    if (r->op == IR_LIT_INTEGER || r->op == IR_LIT_STRING || r->op == IR_OP_COUNT || r->op == IR_LIT_REAL) return 1;
    if (r->op == IR_VAR && IR_LIT(r).sval && IR_LIT(r).sval[0] != '&') return 1;
    /* not(x): now IR_VAR sval="&null" (no IR_NOT opcode -- see lower_not). Always the fixed null DESCR,
       same safe-known-value guarantee the deleted IR_NOT case gave this gate; accept narrowly, not via
       a blanket keyword exception (other keywords are dynamic runtime state, deliberately still excluded). */
    if (r->op == IR_VAR && IR_LIT(r).sval && !strcmp(IR_LIT(r).sval, "&null")) return 1;
    if (r->op == IR_BINOP && (IR_LIT(r).ival == BINOP_ADD || IR_LIT(r).ival == BINOP_SUB || IR_LIT(r).ival == BINOP_MUL || IR_LIT(r).ival == BINOP_DIV || IR_LIT(r).ival == BINOP_MOD || IR_LIT(r).ival == BINOP_CONCAT)) return 1;
    if (r->op == IR_CALL && IR_LIT(r).dval == 0.0) return 1;
    if (r->op == IR_CALL && IR_LIT(r).dval == 1.0) return 1;
    { extern void *dat_find_type(const char *name); if (r->op == IR_CALL && IR_LIT(r).dval == 3.0 && IR_LIT(r).sval && dat_find_type(IR_LIT(r).sval)) return 1; }
    { extern int rt_builtin_is_known(const char *name); const char *bn = IR_LIT(r).sval; if (r->op == IR_CALL && IR_LIT(r).dval == 3.0 && bn && rt_builtin_is_known(bn)) return 1; }
    if (r->op == IR_OP_COUNT) return 1;
    if (r->op == IR_TO || r->op == IR_TO_BY || r->op == IR_PROC_GEN || r->op == IR_OP_COUNT) return 1;
    if (r->op == IR_MAKE_LIST) return 1;
    if (r->op == IR_CONJ) { IR_t *lv = (r->n_operands > 0) ? r->operands[0] : (IR_t *)0; return lv ? rhs_kind_ok(lv) : 0; }
    { extern int is_global(const char *); if (r->op == IR_ASSIGN && IR_LIT(r).sval && !is_global(IR_LIT(r).sval)) { IR_t *rv = (r->n_operands > 0) ? r->operands[0] : (IR_t *)0; return rv ? rhs_kind_ok(rv) : 0; } }
    if (r->op == IR_OP_COUNT) return 1;
    if (r->op == IR_OP_COUNT) return 1;
    if (r->op == IR_CALL && IR_LIT(r).dval == 2.0 && !(IR_LIT(r).sval && (!strcmp(IR_LIT(r).sval,"__rk_bool")||!strcmp(IR_LIT(r).sval,"__rk_try")))) return 1;
    if (r->op == IR_OP_COUNT) { int64_t u = IR_LIT(r).ival; if (u == TT_MNS || u == TT_PLS || u == TT_SIZE || u == TT_NONNULL || u == TT_NULL || u == TT_NOT) return 1; }
    if (r->op == IR_OP_COUNT || r->op == IR_OP_COUNT || r->op == IR_OP_COUNT || r->op == IR_OP_COUNT || r->op == IR_OP_COUNT) return 1;
    if (r->op == IR_OP_COUNT) return gen_scan_body_slotful(r);
    return 0;
}
static int graph_has_binop(const IR_graph_t *g);
static int local_assign_rhs_ok_g(const IR_graph_t *g, IR_t *nd) {
    IR_t *rhs = (nd->n_operands > 0) ? nd->operands[0] : (IR_t *)0;
    if (!rhs) for (int i = 0; i < g->n; i++) { IR_t *p = g->all[i]; if (p && p->γ.node == nd) { rhs = p; break; } }
    if (rhs && rhs->op == IR_LIT_REAL) return 1;
    return rhs_kind_ok(rhs);
}
static int arith_operand_ok(IR_t *r) {
    if (!r) return 0;
    if (r->op == IR_LIT_INTEGER) return 1;
    if (r->op == IR_BINOP && (IR_LIT(r).ival == BINOP_ADD || IR_LIT(r).ival == BINOP_SUB || IR_LIT(r).ival == BINOP_MUL || IR_LIT(r).ival == BINOP_DIV || IR_LIT(r).ival == BINOP_MOD)) return 1;
    return 0;
}
static int is_jct_call(IR_t *r) {
    return r && r->op == IR_CALL && IR_LIT(r).sval && !strncmp(IR_LIT(r).sval, "__rk_jct_", 9);
}
static int jct_marshallable(IR_t *r) {
    if (!r) return 0;
    if (r->op == IR_LIT_INTEGER || r->op == IR_LIT_STRING || r->op == IR_LIT_REAL || r->op == IR_OP_COUNT) return 1;
    if (r->op == IR_VAR && IR_LIT(r).sval && IR_LIT(r).sval[0] != '&') return 1;
    if (r->op == IR_CALL && (IR_LIT(r).dval == 2.0 || IR_LIT(r).dval == 3.0 || IR_LIT(r).dval == 5.0)) return 1;
    return 0;
}
static int bool_truthy_emittable(IR_t *nd) {
    if (!nd || nd->op != IR_CALL || !IR_LIT(nd).sval || strcmp(IR_LIT(nd).sval,"__rk_bool") || IR_LIT(nd).dval != 2.0) return 0;
    IR_graph_t **blks = (IR_graph_t **)0;
    IR_graph_t *cond = blks ? blks[0] : (IR_graph_t *)0;
    if (!cond || !cond->entry) return 0;
    IR_t *e = cond->entry;
    return (e->op == IR_LIT_INTEGER || e->op == IR_LIT_STRING || (e->op == IR_VAR && IR_LIT(e).sval && IR_LIT(e).sval[0] != '&'));
}
static int bool_cond_emittable(IR_t *nd) {
    if (!nd || nd->op != IR_CALL || !IR_LIT(nd).sval || strcmp(IR_LIT(nd).sval, "__rk_bool") || IR_LIT(nd).dval != 2.0) return 0;
    IR_graph_t **blks = (IR_graph_t **)0;
    IR_graph_t *cond = blks ? blks[0] : (IR_graph_t *)0;
    if (!cond) return 0;
    IR_t *p = cond->entry; IR_t *rel = (IR_t *)0; int gd = 0;
    while (p && gd++ < 256) { if (p->op == IR_BINOP && IR_LIT(p).ival >= BINOP_LT && IR_LIT(p).ival <= BINOP_NE) { rel = p; break; } if (!p->γ.node) break; p = p->γ.node; }
    if (!rel) return 0;
    IR_t *ra = ir_pair_arg(rel, 0); IR_t *rb = ir_pair_arg(rel, 1);
    if ((is_jct_call(ra) || is_jct_call(rb)) && jct_marshallable(ra) && jct_marshallable(rb)) return 1;
    return arith_operand_ok(ra) && arith_operand_ok(rb);
}
static int graph_has_local_assign(const IR_graph_t *g) {
    for (int ni = 0; ni < g->n; ni++) {
        IR_t *nd = g->all[ni];
        if (nd && nd->op == IR_ASSIGN && IR_LIT(nd).sval && !is_global(IR_LIT(nd).sval)) return 1;
    }
    return 0;
}
static int graph_has_binop(const IR_graph_t *g) {
    for (int ni = 0; ni < g->n; ni++) if (g->all[ni] && g->all[ni]->op == IR_BINOP) return 1;
    return 0;
}
static int graph_var_assigned_or_param(stage2_t *s2, int gi, IR_graph_t *g, const char *name) {
    for (int i = 0; i < g->n; i++) { IR_t *m = g->all[i]; if (m && m->op == IR_ASSIGN && IR_LIT(m).sval && !strcmp(IR_LIT(m).sval, name)) return 1; }
    for (int p = 0; p < s2->proc_count; p++) {
        if (s2->proc_table[p].bb_idx != gi) continue;
        for (int k = 0; k < s2->proc_table[p].nparams && k < s2->proc_table[p].lower_sc.n; k++)
            if (s2->proc_table[p].lower_sc.e[k].name && !strcmp(s2->proc_table[p].lower_sc.e[k].name, name)) return 1;
    }
    return 0;
}
static int graph_native_emittable_mode(stage2_t *s2, int for_run) {
    extern int rt_builtin_is_known(const char *name);
    if (!s2) return 0;
    for (int gi = 0; gi < s2->bbp.count; gi++) {
        IR_graph_t *g = s2->bbp.table[gi];
        if (!g || !g->all) continue;
        int has_lassign = graph_has_local_assign(g);
        int has_binop = graph_has_binop(g);
        for (int ni = 0; ni < g->n; ni++) {
            IR_t *nd = g->all[ni];
            if (!nd) continue;
            if (nd->op == IR_CALL && IR_LIT(nd).dval == 2.0 && IR_LIT(nd).sval && strcmp(IR_LIT(nd).sval,"__rk_bool") && strcmp(IR_LIT(nd).sval,"__rk_try") && !rt_builtin_is_known(IR_LIT(nd).sval)) return 0;
            if (nd->op == IR_OP_COUNT) {
                if (nd->n_operands < 1 || !nd->operands[0]) return 0;
                for (int aj = 1; aj < nd->n_operands; aj++) {
                    IR_t *arm = nd->operands[aj];
                    if (!arm || arm->op != IR_OP_COUNT || arm->n_operands < 1 || !arm->operands[0]) return 0;
                }
            }
            if (nd->op == IR_OP_COUNT && (nd->n_operands < 1 || !nd->operands[0])) return 0; /* admit user-defined generator suspend only when the expr-value operand is present (the resume-spine: bb_suspend yields operand[0], β runs operand[1] do-body; native driver landed — pieces 1-5 of DESIGN-ICON-SUSPEND); a malformed suspend with no value operand still REJECTED pre-emission */
            if (0 && nd->op == IR_OP_COUNT) return 0; /* BENCH-F1 native list-element-assign arm in progress: scaffolding present (bb_idx_set + flat_drive_idx_set), but LIT-operand slotting (m3) + global-list value flow unfinished -> pre-emission reject, never abort */
            if (nd->op == IR_OP_COUNT) return 0; /* BENCH-F2 reversible-assign <- : full scaffolding landed (IR_OP_COUNT + lower TT_REVASSIGN + bb_rasgn template + flat_drive_rasgn + dispatch), but rhs-var resolves to wrong frame slot in the conjunction's chain (op_a_slot collides with dest varslot) -> pre-emission reject, never silently wrong, until flat-chain rhs slotting is fixed */            if (nd->op == IR_OP_COUNT) { IR_t *rv = (nd->n_operands > 1) ? nd->operands[1] : (IR_t *)0; if (!rv || !rhs_kind_ok(rv) || rv->op == IR_OP_COUNT) return 0; } /* generator-RHS field-set: rhs slot unfilled (bb_field_set bombs) -> pre-emission reject until generator-into-field value-flow built */
            /* field-get→binop: FIXED — IR_OP_COUNT is now arity-1 in descr_chain_arity, so it consumes its object operand off the RPN stack and the binop reads the field-get result slot (not the leaked object var). veto removed. */
            if (nd->op == IR_OP_COUNT) { IR_t *lv = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0; IR_t *rv = nd->n_operands > 1 ? nd->operands[1] : (IR_t *)0; if (!lv || !rv || lv->op != IR_VAR || rv->op != IR_VAR || !IR_LIT(lv).sval || !IR_LIT(rv).sval) return 0; }
            if (nd->op == IR_CALL && IR_LIT(nd).dval == 2.0 && IR_LIT(nd).sval && (!strcmp(IR_LIT(nd).sval,"__rk_bool")||!strcmp(IR_LIT(nd).sval,"__rk_try"))) { if (bool_cond_emittable(nd)||bool_truthy_emittable(nd)) {} else return 0; }
            if (nd->op == IR_OP_COUNT) {
                if (IR_LIT(nd).dval != 1.0) return 0;
                IR_graph_t *ssg = (IR_graph_t *)0;
                IR_graph_t *bsg = (IR_graph_t *) 0;
                if (!scan_subgraph_safe(s2, gi, g, ssg, 0) || !scan_subgraph_safe(s2, gi, g, bsg, 0)) return 0;
                if (nd->γ.node && (nd->γ.node->op == IR_CALL || ir_is_scan_kind(nd->γ.node->op)) && !gen_scan_body_slotful(nd)) return 0;
            }
            {
              if (nd->op == IR_VAR && IR_LIT(nd).sval && IR_LIT(nd).sval[0] != '&' && !is_global(IR_LIT(nd).sval) && !graph_var_assigned_or_param(s2, gi, g, IR_LIT(nd).sval)) return 0;
              if (nd->op == IR_ASSIGN && IR_LIT(nd).sval) {
                  int lhs_global = is_global(IR_LIT(nd).sval);
                  if (lhs_global) { /* nv global assign: bb_gvar_assign_icn (BUILT) */ }
                  else if (local_assign_rhs_ok_g(g, nd)) { /* wave-1 local assign: bb_assign_local (lit/var/binop rhs) */ }
                  else return 0; /* other rhs shapes: native store arm not built -> pre-emission reject, never abort */
              } }
        }
    }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/* GROUND ZERO #5: Prolog pl_gz and pl_findall codegen subsystem deleted (Icon-only reset). */
/* THE DIRECTIVE (Lon 2026-06-24): every BB-local collection grows geometrically (×2) — NO fixed [N]
   ceiling, NO `> N` overflow fence. Start cap 0 (NULL buf); first push allocates 8. GC-managed, so a
   stale buffer after a grow is reclaimed automatically. Used for the GZ admit goal/claim buffers. */
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/* PL-GZ-5c: a callee whose graph entry is a CHOICE over 2..4 RULE clauses (facts = empty-body rules).
 * Validation recurses through clause bodies; the visiting list breaks self/mutual recursion cycles
 * (a graph already on the list is being validated up-stack — assume ok here; a real failure
 * surfaces at the outer frame). */
static IR_graph_t **g_gz_visiting = NULL; static int g_gz_nvisiting = 0; static int g_gz_visiting_cap = 0;
/*--------------------------------------------------------------------------------------------------------------------*/
/* PL-BB-1 (LOWER half) — sound conservative callee-determinacy marker for the bounded-call rung. A call is `det`
 * (bounded) when the callee yields AT MOST ONE solution, so its retained-closure beta resume is dead and a later
 * increment can pop the E-area frame at gamma; this layer proves only the unambiguously-sound core: a SINGLE-clause
 * callee (entry not IR_CHOICE) whose body holds no surviving choice point — every body node is a det builtin / unify
 * / cut / arith / struct / leaf, or an IR_GOAL to a callee that is itself det (recursed; cycle-broken — a graph
 * already on the determinacy stack is single-clause self/mutual recursion, which adds no choice point, so det for
 * the cycle). ANY nested IR_CHOICE / IR_DISJ / IR_ITE / IR_CATCH / IR_CELL_* makes the clause NON-det (under-marking
 * is safe — marking too FEW calls only forgoes optimization; marking too MANY would drop solutions, so this errs to
 * 0). Multi-clause determinacy via first-arg indexing + cut-commit (the fib/tak shape) is the harder NEXT layer. */
static IR_graph_t **g_gz_det_visiting = NULL; static int g_gz_det_nvisiting = 0; static int g_gz_det_visiting_cap = 0;
/* scan one clause body graph: returns 1 iff choicepoint-free (every node a det builtin / unify / cut / arith /
 * struct / leaf, or an IR_GOAL to a det callee); sets *has_cut if any IR_CUT node is present. ANY nested
 * IR_CHOICE / IR_DISJ / IR_ITE / IR_CATCH / IR_CELL_* is a surviving choice point — not choicepoint-free. */
/*--------------------------------------------------------------------------------------------------------------------*/
/* PL-BB-1 (LOWER half) — sound conservative callee-determinacy marker for the bounded-call rung. A call is `det`
 * (bounded) when the callee yields AT MOST ONE solution, so its retained-closure beta resume is dead and the emitter
 * elides it (bb_cell_call). SINGLE-clause callee: det iff its body is choicepoint-free. MULTI-clause callee (entry
 * IR_CHOICE): det iff Condition A holds — every NON-last clause contains a cut (so if it reaches a solution it has
 * committed away the later clauses + sibling CPs) AND every clause body is choicepoint-free (the last clause needs no
 * cut — nothing follows it). Recursion is cycle-broken: a graph already on the determinacy stack is self/mutual
 * recursion, which adds no choice point (it only deepens), so det for the cycle. Under-marking is safe (forgoes
 * optimization); over-marking would drop solutions, so every ambiguous shape resolves to NOT-det. Condition A marks
 * the cut-guarded recursion idiom (fib: clauses fib(0,_):-! and fib(1,_):-! cut, the last fib(N,_) clause has a det
 * body); complementary-guard determinism (tak's X=<Y / X>Y) is a further layer not claimed here. */
/* A comparison/arith operand admissible by the GZ det path: a bare var, an integer/float literal,
 * or any (possibly nested) arith expression over var/lit leaves using flat-supported ops.  Flattened
 * bottom-up into IR_DET_IS steps at build time, so `X > Y*A+N` and `X =\= (Y-N)*2` are admitted. */
/* PL-BB-2 soft-cut disjunction in a callee body: ( Cond, !, Then ; Else ) is the explicit-cut form of
 * ( Cond -> Then ; Else ).  Admit ONLY when arm0 is a conjunction whose pre-cut Cond is choicepoint-free
 * (semidet builtins / unifications, no user calls), so the cut is a free commit — then map to the IR_ITE
 * cell by splitting arm0 at the cut and attaching the synthesised bb_ite_state_t on the DISJ node, so the
 * count/build passes read it with no graph access.  Any other disjunction shape is rejected (broken=0).  */
/*--------------------------------------------------------------------------------------------------------------------*/
/* PL-GZ-5c: graph slot s of one clause → frame slot.  Head slots 0..ar-1 are shared (the args);
 * each clause's locals live in its own frame range starting at lbase (single-clause lbase==ar ⇒ identity). */
/* Synth temps a comparison goal needs: enough to flatten each arith-expr operand into IR_DET_IS
 * steps before the IR_DET_CMP.  Bare var/lit operands need none. */
/* PL-GZ nested-arith: ops the flat IR_DET_IS emitter arms (var-op-lit / var-op-var) accept.  Any
 * arith node using one of these can be evaluated by a single flat IS over slot/lit operands; a
 * nested expression is flattened bottom-up into a chain of such steps via fresh synth slots. */
/* A var/lit leaf, or a binary tree of supported ops over such leaves. */
/* Number of IR_ARITH nodes in an expression (used to bound synth-slot demand for flattening:
 * a safe upper bound is 2 * this, allowing one slot per node plus one lit-materialisation each). */
/* Emit a flat IR_DET_IS computing dstslot = op(L,R) onto head/tail.  L,R are each a slot var or a
 * literal.  The emitter handles lit/lit (const), var/lit, var/var — not lit/var, so materialise a
 * literal L into a slot when R is a var. */
/* Flatten a (possibly nested) arith expression into a chain of flat IS steps appended to head/tail;
 * returns the leaf for nd (a slot-mapped var, a literal, or a fresh synth-slot var holding an
 * internal sub-expression's value).  Slot mapping is via pl_gz_slot_map(ar,lbase) — pass ar=0,lbase=0
 * for the generic (already-final-slot) path. */
/* PL-GZ-5c: multi-clause RULE callee — the seed's path/2.  ONE frame: [args | clause-0 locals+synth |
 * clause-1 locals+synth | … | child slots].  mark at [ζ+0], cursor at [ζ+4]; per-clause body chains;
 * the SHELL-FIRST memo makes self/mutual recursion terminate at admit time exactly as in 5b. */
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/* build a flat goal chain from a sub-graph entry (follow γ until the sub-graph's IR_SUCCEED/IR_FAIL
 * terminator) — used for catch's goal/recovery sub-graphs, which are NOT GCONJ-wrapped at lower time. */
/*--------------------------------------------------------------------------------------------------------------------*/
static int g_gz_no_struct_ptr = 0;
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
extern int resolve_bb_pred_count(void);
extern const char *resolve_bb_pred_name_at(int idx);
extern int resolve_bb_pred_arity_at(int idx);
extern IR_t *resolve_bb_entry_node(const char *name, int arity);
extern IR_graph_t *resolve_bb_graph_at(int idx);
extern int resolve_bb_pred_count(void);
extern const char *resolve_bb_pred_name_at(int idx);
extern int resolve_bb_pred_arity_at(int idx);
/*====================================================================================================================*/
static void m3_enter_with_rbx(bb_box_fn fn, void *frame, int entry, void *gva_base) {
    register void *r_di asm("rdi") = frame;
    register long  r_si asm("rsi") = (long)entry;
    register void *r_bx asm("rbx") = gva_base;
    __asm__ volatile("call *%[f]" : : [f]"r"(fn), "r"(r_di), "r"(r_si), "r"(r_bx) : "rax","rcx","rdx","r8","r9","r10","r11","memory","cc");
}
/*====================================================================================================================*/
int main(int argc, char **argv)
{
    if (argc >= 3 && strcmp(argv[1], "--audit-per-kind") == 0) {
        fprintf(stderr, "scrip: --audit-per-kind unavailable (audit tool unlinked)\n");
        return 1;
    }
    int mode_run           = 0;
    int mode_compile       = 0;
    int dump_ast           = 0;
    int dump_ir            = 0;
    int dump_ir_verbose    = 0;
    int dump_transpile     = 0;
    int opt_bench          = 0;
    const char * target_name = NULL;
    int argi = 1;
    while (argi < argc && argv[argi][0] == '-' && argv[argi][1] == '-') {
        if      (strcmp(argv[argi], "--run")           == 0) { mode_run       = 1; argi++; }
        else if (strcmp(argv[argi], "--compile")       == 0) { mode_compile   = 1; if (!target_name) target_name = "x86"; argi++; }
        else if (strncmp(argv[argi], "--target=", 9)   == 0) { target_name = argv[argi] + 9; mode_compile = 1; argi++; }
        else if (strcmp(argv[argi], "--dump-ast")      == 0) { dump_ast       = 1; argi++; }
        else if (strcmp(argv[argi], "--dump-ir-verbose") == 0) { dump_ir = 1; dump_ir_verbose = 1; argi++; }
        else if (strcmp(argv[argi], "--dump-ir")       == 0) { dump_ir        = 1; argi++; }
        else if (strcmp(argv[argi], "--transpile")     == 0) { dump_transpile = 1; argi++; }
        else if (strcmp(argv[argi], "--bench")         == 0) { opt_bench      = 1; argi++; }
        else break;
    }
    int mode_compile_x86 = (mode_compile && target_name && strcmp(target_name, "x86") == 0);
    if (mode_compile_x86 && mode_run) {
        fprintf(stderr, "scrip: --compile (x86) is mutually exclusive with --run\n");
        return 1;
    }
    if (!mode_run && !mode_compile)
        mode_run = 1;
    if (argi >= argc) {
        fprintf(stderr,
            "usage: scrip [mode] [options] <file> [-- program-args...]\n"
            "\n"
            "Execution modes (default: --run):\n"
            "  --run            build flat-wired x86 BB blobs in a sealed slab and jump in  [DEFAULT]\n"
            "  --compile        emit standalone x86-64 asm to stdout (links libscrip_rt.so)\n"
            "  --target=ARCH    emit code for the named backend (x86, jvm, js, wasm); implies --compile\n"
            "\n"
            "Diagnostic options:\n"
            "  --dump-ast       print AST after frontend\n"
            "  --dump-ir        print IR/BB-graph for each proc (terse: slot/op refs only)\n"
            "  --dump-ir-verbose  same, plus node-id alongside each slot and the legend line\n"
            "  --transpile      transpile AST to portable SNOBOL4 source\n"
            "  --bench          print wall-clock time after execution\n"
            "\n"
            "Frontend inferred from file extension:\n"
            "  .sno=SNOBOL4  .icn=Icon  .pl=Prolog  .sc=Snocone  .reb=Rebus\n"
        );
        return 1;
    }
    extern void sno_add_include_dir(const char *d);
    struct timespec _t0, _t1, _t2, _t3;
    if (opt_bench) clock_gettime(CLOCK_MONOTONIC, &_t0);
    int first_file_argi = argi; (void)first_file_argi;
    int has_non_sno = 0;
    int is_prolog = 0;
    int is_icon = 0;
    int is_raku = 0;
    int is_pascal = 0;
    for (int fi = argi; fi < argc; fi++) {
        const char *d = strrchr(argv[fi], '.');
        if (d && (strcmp(d,".pl")==0 || strcmp(d,".icn")==0 ||
                  strcmp(d,".raku")==0 || strcmp(d,".reb")==0 ||
                  strcmp(d,".sc")==0 || strcmp(d,".scrip")==0 || strcmp(d,".md")==0))
            has_non_sno = 1;
        if (d && strcmp(d,".pl")==0) is_prolog = 1;
        if (d && strcmp(d,".icn")==0) is_icon = 1;
        if (d && strcmp(d,".raku")==0) is_raku = 1;
        if (d && strcmp(d,".pas")==0) is_pascal = 1;
    }
    tree_t  *ast_prog = NULL;
    #define MERGE_AST(sub_ast) do { \
        if (sub_ast) { \
            if (!ast_prog) { ast_prog = sub_ast; } \
            else { \
                if (ast_prog->n > 0) { \
                    tree_t *_last = ast_prog->c[ast_prog->n-1]; \
                    if (_last && _last->t == TT_END) ast_prog->n--; \
                } \
                for (int _i = 0; _i < (sub_ast)->n; _i++) { \
                    ast_push(ast_prog, (sub_ast)->c[_i]); \
                } \
                if ((sub_ast)->c) free((char *)(sub_ast)->c - sizeof(size_t)); free(sub_ast); \
            } \
        } \
    } while(0)
    for (; argi < argc; argi++) {
        const char *input_path = argv[argi];
        {
            char dirbuf[4096];
            strncpy(dirbuf, input_path, sizeof dirbuf - 1);
            dirbuf[sizeof dirbuf - 1] = '\0';
            char *sl = strrchr(dirbuf, '/');
            if (sl) { *sl = '\0'; sno_add_include_dir(dirbuf); }
            else     { sno_add_include_dir("."); }
            const char *core_lib = getenv("SNO_LIB");
            if (core_lib && *core_lib) sno_add_include_dir(core_lib);
            char walk[4096];
            strncpy(walk, input_path, sizeof walk - 1);
            walk[sizeof walk - 1] = '\0';
            char *p = strrchr(walk, '/');
            while (p) {
                *p = '\0';
                char probe[4096];
                snprintf(probe, sizeof probe, "%s/lib", walk);
                struct stat st;
                if (stat(probe, &st) == 0 && S_ISDIR(st.st_mode)) {
                    sno_add_include_dir(walk);
                    break;
                }
                p = strrchr(walk, '/');
            }
            sno_add_include_dir(".");
        }
        const char *dot = strrchr(input_path, '.');
        int lang_snocone  = dot && strcmp(dot, ".sc")   == 0;
        int lang_prolog   = dot && strcmp(dot, ".pl")   == 0;
        int lang_icon     = dot && strcmp(dot, ".icn")  == 0;
        int lang_raku     = dot && strcmp(dot, ".raku") == 0;
        int lang_rebus    = dot && strcmp(dot, ".reb")  == 0;
        int lang_pascal   = dot && strcmp(dot, ".pas")  == 0;
        int lang_polyglot = dot && (strcmp(dot, ".scrip") == 0 || strcmp(dot, ".md") == 0);
        if (lang_polyglot) {
            g_polyglot = 1;
            FILE *f = fopen(input_path, "r");
            if (!f) { fprintf(stderr, "scrip: cannot open '%s'\n", input_path); return 1; }
            fseek(f, 0, SEEK_END); long flen = ftell(f); rewind(f);
            char *src = malloc(flen + 1);
            if (!src) { fprintf(stderr, "scrip: out of memory\n"); return 1; }
            fread(src, 1, flen, f); src[flen] = '\0'; fclose(f);
            tree_t *sub_ast = parse_scrip_polyglot(src, input_path);
            free(src);
            MERGE_AST(sub_ast);
        } else if (lang_snocone || lang_prolog || lang_icon || lang_raku || lang_rebus || lang_pascal) {
            FILE *f = fopen(input_path, "r");
            if (!f) { fprintf(stderr, "scrip: cannot open '%s'\n", input_path); return 1; }
            fseek(f, 0, SEEK_END); long flen = ftell(f); rewind(f);
            char *src = malloc(flen + 1);
            if (!src) { fprintf(stderr, "scrip: out of memory\n"); return 1; }
            fread(src, 1, flen, f); src[flen] = '\0'; fclose(f);
            tree_t *sub_ast = NULL;
            if (lang_icon)         icon_compile(src, input_path, &sub_ast);
            else if (lang_raku)    raku_compile(src, input_path, &sub_ast);
            else if (lang_prolog)  prolog_compile(src, input_path, &sub_ast);
            else if (lang_rebus)   rebus_compile(src, input_path, &sub_ast);
            else if (lang_pascal)  pascal_compile(src, input_path, &sub_ast);
            else                   snocone_compile(src, input_path, &sub_ast);
            free(src);
            if (dump_ast && sub_ast) {
                ir_dump_program(sub_ast, stdout); return 0;
            }
            MERGE_AST(sub_ast);
        } else if (dump_ast) {
            FILE *f = fopen(input_path, "r");
            if (!f) { fprintf(stderr, "scrip: cannot open '%s'\n", input_path); return 1; }
            if (opt_bench) clock_gettime(CLOCK_MONOTONIC, &_t1);
            tree_t *sub_ast = sno_parse_ast(f, input_path, NULL);
            fclose(f);
            ir_dump_program(sub_ast, stdout);
            return 0;
        } else if (dump_transpile) {
            FILE *f = fopen(input_path, "r");
            if (!f) { fprintf(stderr, "scrip: cannot open '%s'\n", input_path); return 1; }
            if (opt_bench) clock_gettime(CLOCK_MONOTONIC, &_t1);
            tree_t *sub_ast = sno_parse_ast(f, input_path, NULL);
            fclose(f);
            MERGE_AST(sub_ast);
        } else {
            FILE *f = fopen(input_path, "r");
            if (!f) { fprintf(stderr, "scrip: cannot open '%s'\n", input_path); return 1; }
            tree_t *sub_ast = sno_parse_ast(f, input_path, NULL);
            fclose(f);
            MERGE_AST(sub_ast);
        }
        if (!ast_prog) {
            fprintf(stderr, "scrip: parse failed for '%s'\n", input_path);
            return 1;
        }
    }
    if (opt_bench) clock_gettime(CLOCK_MONOTONIC, &_t1);
    const char *input_path = argv[argc - 1];
    if (opt_bench) clock_gettime(CLOCK_MONOTONIC, &_t2);
    if (!ast_prog) {
        fprintf(stderr, "scrip: parse failed for '%s'\n", input_path);
        return 1;
    }
    {
        extern void bb_pool_init(void);
        bb_pool_init();
        { extern void pat_pool_init(void); pat_pool_init(); }
    }
    setvbuf(stdout, NULL, _IOLBF, 0);
    extern void core_lib_init(void);
    core_lib_init();
    stmt_init();
    register_fn("IDENT",  _builtin_IDENT,  1, 2);
    register_fn("DIFFER", _builtin_DIFFER, 1, 2);
    register_fn("EVAL",   _builtin_EVAL,   1, 1);
    register_fn("CODE",   _builtin_CODE,   1, 1);
    register_fn("DATA",   _builtin_DATA,   1, 1);
    register_fn("print",  _builtin_print,  0, 99);
    extern DESCR_t (*g_user_call_hook)(const char *, DESCR_t *, int);
    g_user_call_hook = _usercall_hook;
    {
        extern void core_set_label_exists_hook(int (*fn)(const char *));
        core_set_label_exists_hook(_label_exists_fn);
    }
    {
        extern DESCR_t (*g_eval_pat_hook)(DESCR_t pat);
        g_eval_pat_hook = _eval_pat_impl_fn;
    }
    {
        extern DESCR_t (*g_eval_str_hook)(const char *s);
        g_eval_str_hook = _eval_str_impl_fn;
    }
    g_opt_dump_bb = dump_ir;
    if (dump_transpile) {
        extern int tree_to_sno(const tree_t *ast, FILE *out);
        tree_to_sno(ast_prog, stdout);
        return 0;
    }
    if (dump_ir) {
        extern void bb_print_v(const IR_graph_t * bbg, FILE * fp, int verbose);
        extern void ir_drive_slot_assign(IR_graph_t * g);
        extern int g_postfix_resume;
        if (is_icon) g_postfix_resume = 1;
        stage2_t *s2 = sm_preamble(ast_prog);
        if (!s2) { fprintf(stderr, "scrip: sm_preamble failed\n"); return 1; }
        ast_tree_free(ast_prog); ast_prog = NULL;
        const IR_t ** seen_all = (const IR_t **) calloc(s2->proc_count > 0 ? s2->proc_count : 1, sizeof(const IR_t *));
        int seen_n = 0;
        for (int _pi = 0; _pi < s2->proc_count; _pi++) {
            int idx = s2->proc_table[_pi].bb_idx;
            const char *pname = s2->proc_table[_pi].name ? s2->proc_table[_pi].name : "?";
            if (idx < 0 || idx >= s2->bbp.count || !s2->bbp.table[idx]) continue;
            const IR_t ** all = (const IR_t **) s2->bbp.table[idx]->all;
            int dup = 0;
            for (int s = 0; s < seen_n; s++) if (seen_all[s] == (const IR_t *) all) { dup = 1; break; }
            if (dup) continue;
            seen_all[seen_n++] = (const IR_t *) all;
            fprintf(stdout, "; proc %s\n", pname);
            if (is_icon) ir_drive_slot_assign(s2->bbp.table[idx]); else ir_tmp_slot_assign(s2->bbp.table[idx]);
            bb_print_v(s2->bbp.table[idx], stdout, dump_ir_verbose);
        }
        free(seen_all);
        return 0;
    }
    if (mode_compile_x86) {
        extern int codegen_flat_build(IR_t * nd, FILE * out, const char * prefix);
        extern int g_frame_active;
        if (is_icon || is_raku) {
            extern int g_postfix_resume;
            extern int g_m4_dense_nid; extern void g_bb_alpha_seq_reset(void);
            g_m4_dense_nid = 1; g_bb_alpha_seq_reset();
            if (is_icon) g_postfix_resume = 1;
            stage2_t *s2 = sm_preamble(ast_prog);
            if (!s2) return 1;
            ast_tree_free(ast_prog); ast_prog = NULL;
            if (is_icon) icn_register_record_types(s2);
            if (is_icon) { extern void optimizer_run(IR_graph_t * g); for (int _gi = 0; _gi < s2->bbp.count; _gi++) if (s2->bbp.table[_gi]) optimizer_run(s2->bbp.table[_gi]); }
            if (is_icon) { extern void ir_drive_slot_assign(IR_graph_t * g); for (int _gi = 0; _gi < s2->bbp.count; _gi++) if (s2->bbp.table[_gi]) ir_drive_slot_assign(s2->bbp.table[_gi]); }
            if (is_raku && !graph_native_emittable(s2)) {
                fprintf(stderr, "[SMX] --compile --target=x86: mode-4 native emitter does not yet cover "
                                "this program (a box has no MEDIUM_TEXT arm — Raku map/grep). REJECTED — native BB emission pending (no interpreter fallback).\n");
                return 0;
            }
            extern void rt_proc_register(const char *name, const char **pnames, int nparams);
            extern void rt_proc_reset(void);
            int main_bb_idx = -1;
            rt_proc_reset();
            for (int _pi = 0; _pi < s2->proc_count; _pi++) {
                const char *pname = s2->proc_table[_pi].name;
                if (!pname) continue;
                if (strcmp(pname, "main") == 0) { main_bb_idx = s2->proc_table[_pi].bb_idx; continue; }
                int idx = s2->proc_table[_pi].bb_idx;
                if (idx < 0 || idx >= s2->bbp.count || !s2->bbp.table[idx] || !s2->bbp.table[idx]->entry) continue;
                int np = s2->proc_table[_pi].nparams;
                const char **pn = NULL;
                if (np > 0) {
                    pn = (const char **)calloc((size_t)np, sizeof(const char *));
                    for (int k = 0; k < np && k < s2->proc_table[_pi].lower_sc.n; k++)
                        pn[k] = s2->proc_table[_pi].lower_sc.e[k].name;
                }
                rt_proc_register(pname, pn, np);
                { extern void rt_proc_set_generator(const char *, int); rt_proc_set_generator(pname, s2->proc_table[_pi].is_generator); }
            }
            if (main_bb_idx < 0 || main_bb_idx >= s2->bbp.count || !s2->bbp.table[main_bb_idx] || !s2->bbp.table[main_bb_idx]->entry) {
                fprintf(stderr, "[IBB] FATAL: mode-4 driver: main BB graph not found\n");
                return 1;
            }
            IR_graph_t * bbg = s2->bbp.table[main_bb_idx];
            extern int descr_flat_chain_build_text(IR_t * entry, FILE * out, const char * prefix);
            extern int descr_flat_chain_build_proc_text(IR_t *entry, const char **pnames, int np, FILE *out, const char *pname);
            printf("  .intel_syntax noprefix\n");
            printf("  .text\n");
            g_frame_active = 1;
            extern void gva_collect_reset(void); extern void gva_collect_icon_globals(void); extern int gva_count(void); extern const char *gva_name(int); extern int g_gva_active;
            gva_collect_reset();
            gva_collect_icon_globals();
            int n_gva_icn = gva_count();
            g_gva_active = (n_gva_icn > 0) ? 1 : 0;
            int n_procs = 0;
            int _pnbcap = (s2->proc_count > 0) ? s2->proc_count : 1;
            const char **proc_names_buf = (const char **)malloc((size_t)_pnbcap * sizeof(const char *));
            for (int _pi = 0; _pi < s2->proc_count; _pi++) {
                const char *pname = s2->proc_table[_pi].name;
                if (!pname || strcmp(pname, "main") == 0) continue;
                int idx = s2->proc_table[_pi].bb_idx;
                if (idx < 0 || idx >= s2->bbp.count || !s2->bbp.table[idx] || !s2->bbp.table[idx]->entry) continue;
                int np = s2->proc_table[_pi].nparams;
                const char **pn = NULL;
                if (np > 0) {
                    pn = (const char **)calloc((size_t)np, sizeof(const char *));
                    for (int k = 0; k < np && k < s2->proc_table[_pi].lower_sc.n; k++)
                        pn[k] = s2->proc_table[_pi].lower_sc.e[k].name;
                }
                { extern IR_graph_t *g_emit_cfg; g_emit_cfg = s2->bbp.table[idx]; }
                resolve_call_kinds_descr(s2->bbp.table[idx]);
                { extern int g_gen_proc_active; g_gen_proc_active = s2->proc_table[_pi].is_generator; }
                descr_flat_chain_build_proc_text(s2->bbp.table[idx]->entry, pn, np, stdout, pname);
                { extern int g_gen_proc_active; g_gen_proc_active = 0; }
                proc_names_buf[n_procs++] = pname;
                free(pn);
            }
            int n_cls_emit = 0;
            { extern int dat_type_count(void); n_cls_emit = dat_type_count(); }
            int n_gram_emit = 0;
            { extern int rt_grammar_count(void); n_gram_emit = rt_grammar_count(); }
            if (n_procs > 0 || n_cls_emit > 0 || n_gram_emit > 0) {
                printf("proc_startup:\n");
                printf("  push rbp\n");
                printf("  mov rbp, rsp\n");
                { extern int dat_type_count(void); extern const char *dat_type_name(int); extern int dat_type_nfields(int); extern const char *dat_type_field(int, int);
                  int n_cls = dat_type_count();
                  for (int ci = 0; ci < n_cls; ci++) {
                      const char *cn = dat_type_name(ci); if (!cn || !*cn) continue;
                      printf("  .section .rodata\n");
                      printf("  .Lclassspec%d: .string \"%s(", ci, cn);
                      for (int fj = 0; fj < dat_type_nfields(ci); fj++) { if (fj) printf(","); printf("%s", dat_type_field(ci, fj)); }
                      printf(")\"\n");
                      printf("  .section .text\n  .intel_syntax noprefix\n");
                      printf("  lea rdi, [rip + .Lclassspec%d]\n", ci);
                      printf("  call record_register@PLT\n");
                  } }
                { extern int dat_type_count(void); extern const char *dat_type_name(int); extern int dat_type_nparents(int); extern const char *dat_type_parent_at(int, int);
                  int n_cls = dat_type_count();
                  for (int ci = 0; ci < n_cls; ci++) {
                      const char *cn = dat_type_name(ci); if (!cn || !*cn) continue;
                      int np = dat_type_nparents(ci); if (np <= 0) continue;
                      printf("  .section .rodata\n");
                      printf("  .Lclschild%d: .string \"%s\"\n", ci, cn);
                      for (int pj = 0; pj < np; pj++) printf("  .Lclsp%d_%d: .string \"%s\"\n", ci, pj, dat_type_parent_at(ci, pj));
                      printf("  .balign 8\n  .Lclsparr%d:\n", ci);
                      for (int pj = 0; pj < np; pj++) printf("  .quad .Lclsp%d_%d\n", ci, pj);
                      printf("  .section .text\n  .intel_syntax noprefix\n");
                      printf("  lea rdi, [rip + .Lclschild%d]\n", ci);
                      printf("  lea rsi, [rip + .Lclsparr%d]\n", ci);
                      printf("  mov rdx, %d\n", np);
                      printf("  call class_inherit_multi@PLT\n");
                  } }
                { extern int dat_type_count(void); extern const char *dat_type_name(int); extern int dat_type_nfields(int); extern const char *dat_type_field(int, int);
                  extern int dat_type_field_has_default(int, int); extern DESCR_t dat_type_field_default(int, int);
                  int n_cls = dat_type_count();
                  for (int ci = 0; ci < n_cls; ci++) {
                      const char *cn = dat_type_name(ci); if (!cn || !*cn) continue;
                      for (int fj = 0; fj < dat_type_nfields(ci); fj++) {
                          if (!dat_type_field_has_default(ci, fj)) continue;
                          const char *fn = dat_type_field(ci, fj); if (!fn) continue;
                          DESCR_t dv = dat_type_field_default(ci, fj);
                          printf("  .section .rodata\n");
                          printf("  .Ldefcls%d_%d: .byte ", ci, fj); for (const char *p = cn; *p; p++) printf("%d, ", (int)(unsigned char)*p); printf("0\n");
                          printf("  .Ldeffld%d_%d: .byte ", ci, fj); for (const char *p = fn; *p; p++) printf("%d, ", (int)(unsigned char)*p); printf("0\n");
                          if (dv.v == DT_S) { const char *sv = dv.s ? dv.s : ""; printf("  .Ldefstr%d_%d: .byte ", ci, fj); for (const char *p = sv; *p; p++) printf("%d, ", (int)(unsigned char)*p); printf("0\n"); }
                          else if (dv.v == DT_R) { union { double d; unsigned long long q; } u; u.d = dv.r; printf("  .Ldefdbl%d_%d: .quad %llu\n", ci, fj, u.q); }
                          printf("  .section .text\n  .intel_syntax noprefix\n");
                          printf("  lea rdi, [rip + .Ldefcls%d_%d]\n", ci, fj);
                          printf("  lea rsi, [rip + .Ldeffld%d_%d]\n", ci, fj);
                          if (dv.v == DT_S) { printf("  lea rdx, [rip + .Ldefstr%d_%d]\n", ci, fj); printf("  call dat_set_field_default_s@PLT\n"); }
                          else if (dv.v == DT_R) { printf("  movsd xmm0, qword ptr [rip + .Ldefdbl%d_%d]\n", ci, fj); printf("  call dat_set_field_default_r@PLT\n"); }
                          else { printf("  mov rdx, %lld\n", (long long)dv.i); printf("  call dat_set_field_default_i@PLT\n"); }
                      }
                  } }
                { extern int dat_type_count(void); extern const char *dat_type_name(int); extern int dat_type_nfields(int); extern const char *dat_type_field(int, int);
                  extern int dat_type_field_required(int, int);
                  int n_cls = dat_type_count();
                  for (int ci = 0; ci < n_cls; ci++) {
                      const char *cn = dat_type_name(ci); if (!cn || !*cn) continue;
                      for (int fj = 0; fj < dat_type_nfields(ci); fj++) {
                          if (!dat_type_field_required(ci, fj)) continue;
                          const char *fn = dat_type_field(ci, fj); if (!fn) continue;
                          printf("  .section .rodata\n");
                          printf("  .Lreqcls%d_%d: .byte ", ci, fj); for (const char *p = cn; *p; p++) printf("%d, ", (int)(unsigned char)*p); printf("0\n");
                          printf("  .Lreqfld%d_%d: .byte ", ci, fj); for (const char *p = fn; *p; p++) printf("%d, ", (int)(unsigned char)*p); printf("0\n");
                          printf("  .section .text\n  .intel_syntax noprefix\n");
                          printf("  lea rdi, [rip + .Lreqcls%d_%d]\n", ci, fj);
                          printf("  lea rsi, [rip + .Lreqfld%d_%d]\n", ci, fj);
                          printf("  call dat_set_field_required@PLT\n");
                      }
                  } }
                { extern int dat_type_count(void); extern const char *dat_type_name(int); extern int dat_type_nfields(int); extern const char *dat_type_field(int, int);
                  extern int dat_type_field_rw(int, int);
                  int n_cls = dat_type_count();
                  for (int ci = 0; ci < n_cls; ci++) {
                      const char *cn = dat_type_name(ci); if (!cn || !*cn) continue;
                      for (int fj = 0; fj < dat_type_nfields(ci); fj++) {
                          if (!dat_type_field_rw(ci, fj)) continue;
                          const char *fn = dat_type_field(ci, fj); if (!fn) continue;
                          printf("  .section .rodata\n");
                          printf("  .Lrwcls%d_%d: .byte ", ci, fj); for (const char *p = cn; *p; p++) printf("%d, ", (int)(unsigned char)*p); printf("0\n");
                          printf("  .Lrwfld%d_%d: .byte ", ci, fj); for (const char *p = fn; *p; p++) printf("%d, ", (int)(unsigned char)*p); printf("0\n");
                          printf("  .section .text\n  .intel_syntax noprefix\n");
                          printf("  lea rdi, [rip + .Lrwcls%d_%d]\n", ci, fj);
                          printf("  lea rsi, [rip + .Lrwfld%d_%d]\n", ci, fj);
                          printf("  call dat_set_field_rw@PLT\n");
                      }
                  } }
                { extern int dat_type_count(void); extern const char *dat_type_name(int); extern int dat_type_nfields(int); extern const char *dat_type_field(int, int);
                  extern int dat_type_field_sigil(int, int);
                  int n_cls = dat_type_count();
                  for (int ci = 0; ci < n_cls; ci++) {
                      const char *cn = dat_type_name(ci); if (!cn || !*cn) continue;
                      for (int fj = 0; fj < dat_type_nfields(ci); fj++) {
                          int sg = dat_type_field_sigil(ci, fj); if (sg != '@' && sg != '%') continue;
                          const char *fn = dat_type_field(ci, fj); if (!fn) continue;
                          printf("  .section .rodata\n");
                          printf("  .Lsigcls%d_%d: .byte ", ci, fj); for (const char *p = cn; *p; p++) printf("%d, ", (int)(unsigned char)*p); printf("0\n");
                          printf("  .Lsigfld%d_%d: .byte ", ci, fj); for (const char *p = fn; *p; p++) printf("%d, ", (int)(unsigned char)*p); printf("0\n");
                          printf("  .section .text\n  .intel_syntax noprefix\n");
                          printf("  lea rdi, [rip + .Lsigcls%d_%d]\n", ci, fj);
                          printf("  lea rsi, [rip + .Lsigfld%d_%d]\n", ci, fj);
                          printf("  mov rdx, %d\n", sg);
                          printf("  call dat_set_field_sigil@PLT\n");
                      }
                  } }
                { extern int dat_type_count(void); extern const char *dat_type_name(int); extern int dat_type_nfields(int); extern const char *dat_type_field(int, int);
                  extern int dat_type_field_priv(int, int);
                  int n_cls = dat_type_count();
                  for (int ci = 0; ci < n_cls; ci++) {
                      const char *cn = dat_type_name(ci); if (!cn || !*cn) continue;
                      for (int fj = 0; fj < dat_type_nfields(ci); fj++) {
                          if (!dat_type_field_priv(ci, fj)) continue;
                          const char *fn = dat_type_field(ci, fj); if (!fn) continue;
                          printf("  .section .rodata\n");
                          printf("  .Lprvcls%d_%d: .byte ", ci, fj); for (const char *p = cn; *p; p++) printf("%d, ", (int)(unsigned char)*p); printf("0\n");
                          printf("  .Lprvfld%d_%d: .byte ", ci, fj); for (const char *p = fn; *p; p++) printf("%d, ", (int)(unsigned char)*p); printf("0\n");
                          printf("  .section .text\n  .intel_syntax noprefix\n");
                          printf("  lea rdi, [rip + .Lprvcls%d_%d]\n", ci, fj);
                          printf("  lea rsi, [rip + .Lprvfld%d_%d]\n", ci, fj);
                          printf("  call dat_set_field_priv@PLT\n");
                      }
                  } }
                { extern int dat_type_count(void); extern const char *dat_type_name(int); extern int dat_type_nmethods(int); extern const char *dat_type_method_at(int, int);
                  int n_cls = dat_type_count();
                  for (int ci = 0; ci < n_cls; ci++) {
                      const char *cn = dat_type_name(ci); if (!cn || !*cn) continue;
                      int nm = dat_type_nmethods(ci); if (nm <= 0) continue;
                      for (int mj = 0; mj < nm; mj++) {
                          const char *mn = dat_type_method_at(ci, mj); if (!mn || !*mn) continue;
                          printf("  .section .rodata\n");
                          printf("  .Lmethcls%d_%d: .byte ", ci, mj); for (const char *p = cn; *p; p++) printf("%d, ", (int)(unsigned char)*p); printf("0\n");
                          printf("  .Lmethnm%d_%d: .byte ", ci, mj); for (const char *p = mn; *p; p++) printf("%d, ", (int)(unsigned char)*p); printf("0\n");
                          printf("  .section .text\n  .intel_syntax noprefix\n");
                          printf("  lea rdi, [rip + .Lmethcls%d_%d]\n", ci, mj);
                          printf("  lea rsi, [rip + .Lmethnm%d_%d]\n", ci, mj);
                          printf("  call dat_add_method@PLT\n");
                      }
                  } }
                { extern int dat_type_count(void); extern const char *dat_type_name(int); extern int dat_type_has_build(int); extern int dat_type_nbuild_keys(int); extern const char *dat_type_build_key_at(int, int);
                  int n_cls = dat_type_count();
                  for (int ci = 0; ci < n_cls; ci++) {
                      const char *cn = dat_type_name(ci); if (!cn || !*cn) continue;
                      if (!dat_type_has_build(ci)) continue;
                      printf("  .section .rodata\n");
                      printf("  .Lbldcls%d: .byte ", ci); for (const char *p = cn; *p; p++) printf("%d, ", (int)(unsigned char)*p); printf("0\n");
                      printf("  .Lbldnull%d: .byte 0\n", ci);
                      printf("  .section .text\n  .intel_syntax noprefix\n");
                      printf("  lea rdi, [rip + .Lbldcls%d]\n", ci);
                      printf("  lea rsi, [rip + .Lbldnull%d]\n", ci);
                      printf("  call dat_set_build_key@PLT\n");
                      int nk = dat_type_nbuild_keys(ci);
                      for (int kj = 0; kj < nk; kj++) {
                          const char *kn = dat_type_build_key_at(ci, kj); if (!kn || !*kn) continue;
                          printf("  .section .rodata\n");
                          printf("  .Lbldkey%d_%d: .byte ", ci, kj); for (const char *p = kn; *p; p++) printf("%d, ", (int)(unsigned char)*p); printf("0\n");
                          printf("  .section .text\n  .intel_syntax noprefix\n");
                          printf("  lea rdi, [rip + .Lbldcls%d]\n", ci);
                          printf("  lea rsi, [rip + .Lbldkey%d_%d]\n", ci, kj);
                          printf("  call dat_set_build_key@PLT\n");
                      }
                  } }
                { extern int dat_type_count(void); extern const char *dat_type_name(int); extern int dat_type_nhandles(int); extern const char *dat_type_handles_meth_at(int, int); extern const char *dat_type_handles_fld_at(int, int);
                  int n_cls = dat_type_count();
                  for (int ci = 0; ci < n_cls; ci++) {
                      const char *cn = dat_type_name(ci); if (!cn || !*cn) continue;
                      int nh = dat_type_nhandles(ci); if (nh <= 0) continue;
                      for (int hj = 0; hj < nh; hj++) {
                          const char *hm = dat_type_handles_meth_at(ci, hj); const char *hf = dat_type_handles_fld_at(ci, hj);
                          if (!hm || !*hm || !hf || !*hf) continue;
                          printf("  .section .rodata\n");
                          printf("  .Lhndcls%d_%d: .byte ", ci, hj); for (const char *p = cn; *p; p++) printf("%d, ", (int)(unsigned char)*p); printf("0\n");
                          printf("  .Lhndmeth%d_%d: .byte ", ci, hj); for (const char *p = hm; *p; p++) printf("%d, ", (int)(unsigned char)*p); printf("0\n");
                          printf("  .Lhndfld%d_%d: .byte ", ci, hj); for (const char *p = hf; *p; p++) printf("%d, ", (int)(unsigned char)*p); printf("0\n");
                          printf("  .section .text\n  .intel_syntax noprefix\n");
                          printf("  lea rdi, [rip + .Lhndcls%d_%d]\n", ci, hj);
                          printf("  lea rsi, [rip + .Lhndmeth%d_%d]\n", ci, hj);
                          printf("  lea rdx, [rip + .Lhndfld%d_%d]\n", ci, hj);
                          printf("  call dat_add_handles@PLT\n");
                      }
                  } }
                { extern int dat_type_count(void); extern const char *dat_type_name(int); extern int dat_type_nroles(int); extern const char *dat_type_role_at(int, int);
                  int n_cls = dat_type_count();
                  for (int ci = 0; ci < n_cls; ci++) {
                      const char *cn = dat_type_name(ci); if (!cn || !*cn) continue;
                      int nr = dat_type_nroles(ci); if (nr <= 0) continue;
                      for (int rj = 0; rj < nr; rj++) {
                          const char *rn = dat_type_role_at(ci, rj); if (!rn || !*rn) continue;
                          printf("  .section .rodata\n");
                          printf("  .Lrolechild%d_%d: .byte ", ci, rj); for (const char *p = cn; *p; p++) printf("%d, ", (int)(unsigned char)*p); printf("0\n");
                          printf("  .Lrolename%d_%d: .byte ", ci, rj); for (const char *p = rn; *p; p++) printf("%d, ", (int)(unsigned char)*p); printf("0\n");
                          printf("  .section .text\n  .intel_syntax noprefix\n");
                          printf("  lea rdi, [rip + .Lrolechild%d_%d]\n", ci, rj);
                          printf("  lea rsi, [rip + .Lrolename%d_%d]\n", ci, rj);
                          printf("  call class_compose_role@PLT\n");
                      }
                  } }
                { extern int rt_grammar_count(void); extern const char *rt_grammar_qname(int); extern const char *rt_grammar_body(int); extern int rt_grammar_flavor(int);
                  int n_gram = rt_grammar_count();
                  for (int gi = 0; gi < n_gram; gi++) {
                      const char *qn = rt_grammar_qname(gi); const char *bd = rt_grammar_body(gi);
                      if (!qn || !bd) continue;
                      printf("  .section .rodata\n");
                      printf("  .Lgramqn%d: .byte ", gi); for (const char *p = qn; *p; p++) printf("%d, ", (int)(unsigned char)*p); printf("0\n");
                      printf("  .Lgrambd%d: .byte ", gi); for (const char *p = bd; *p; p++) printf("%d, ", (int)(unsigned char)*p); printf("0\n");
                      printf("  .section .text\n  .intel_syntax noprefix\n");
                      printf("  lea rdi, [rip + .Lgramqn%d]\n", gi);
                      printf("  lea rsi, [rip + .Lgrambd%d]\n", gi);
                      printf("  mov edx, %d\n", rt_grammar_flavor(gi));
                      printf("  call rt_grammar_register@PLT\n");
                  } }
                for (int i = 0; i < n_procs; i++) {
                    printf("  .section .rodata\n");
                    printf("  .Lstartup_pname%d: .string \"%s\"\n", i, proc_names_buf[i]);
                    printf("  .section .text\n");
                    printf("  .intel_syntax noprefix\n");
                    printf("  lea rdi, [rip + .Lstartup_pname%d]\n", i);
                    printf("  lea rsi, [rip + proc_%s_\xce\xb1]\n", proc_names_buf[i]);
                    printf("  call rt_proc_set_fn@PLT\n");
                }
                printf("  pop rbp\n");
                printf("  ret\n");
            }
            free(proc_names_buf);
            if (n_gva_icn > 0) {
                printf("  .section .rodata\n");
                for (int k = 0; k < n_gva_icn; k++) printf("  .Lgvan%d: .string \"%s\"\n", k, gva_name(k));
                printf("  .align 8\n__gva_names:\n");
                for (int k = 0; k < n_gva_icn; k++) printf("  .quad .Lgvan%d\n", k);
                printf("  .section .bss\n  .align 16\n__gva: .space %d, 0\n", n_gva_icn * 16);
                printf("  .section .text\n  .intel_syntax noprefix\n");
            }
            printf("  .globl main\n");
            printf("main:\n");
            printf("  push rbp\n");
            printf("  mov rbp, rsp\n");
            if (n_procs > 0 || n_cls_emit > 0 || n_gram_emit > 0)
                printf("  call proc_startup\n");
            if (n_gva_icn > 0) printf("  lea rdi, [rip + __gva_names]\n  lea rsi, [rip + __gva]\n  mov edx, %d\n  call gva_register@PLT\n  mov rbx, rax\n", n_gva_icn);
            printf("  call rt_frame@PLT\n");
            printf("  mov rdi, rax\n");
            printf("  xor esi, esi\n");
            printf("  call main_\xce\xb1\n");
            printf("  xor eax, eax\n");
            printf("  pop rbp\n");
            printf("  ret\n");
            int rc;
            {
                { extern IR_graph_t *g_emit_cfg; g_emit_cfg = bbg; }
                resolve_call_kinds_descr(bbg);
                rc = descr_flat_chain_build_text(bbg->entry, stdout, "main");
            }
            g_gva_active = 0;
            g_frame_active = 0;
            extern void xa_emit_strtab_rodata(void);
            xa_emit_strtab_rodata();
            fflush(stdout);
            ir_delete_all(s2);
            return rc;
        }
        if (is_prolog) {
            fprintf(stderr, "GROUND ZERO #5: Prolog backend deleted (Icon-only reset; Prolog rebuilds later).\n");
            return 1;
        }
        {
            extern int gvar_flat_chain_build_text(IR_graph_t * g, FILE * out, const char * prefix);
            extern int gvar_flat_chain_build_text_at(IR_graph_t * g, IR_t * entry_node, FILE * out, const char * prefix);
            extern void xa_emit_strtab_rodata(void);
            extern int g_frame_active;
            extern void rt_proc_reset(void);
            extern void rt_proc_register(const char * name, const char ** pnames, int nparams);
            stage2_t *s2 = sm_preamble(ast_prog);
            if (!s2) { fprintf(stderr, "[SBB] mode-4: sm_preamble failed\n"); return 1; }
            ast_tree_free(ast_prog); ast_prog = NULL;
            int main_bb_idx = -1;
            for (int _pi = 0; _pi < s2->proc_count; _pi++)
                if (s2->proc_table[_pi].name && strcmp(s2->proc_table[_pi].name, "main") == 0) { main_bb_idx = s2->proc_table[_pi].bb_idx; break; }
            if (main_bb_idx < 0 || main_bb_idx >= s2->bbp.count || !s2->bbp.table[main_bb_idx] || !s2->bbp.table[main_bb_idx]->entry) {
                fprintf(stderr, "[SBB] FATAL: mode-4 driver: SNOBOL4 main BB graph not found\n");
                return 1;
            }
            IR_graph_t *sbbg = s2->bbp.table[main_bb_idx];
            extern int g_flat_node_id;
            extern int g_m4_dense_nid;
            g_flat_node_id = 0;
            g_m4_dense_nid = 1;
            printf("  .intel_syntax noprefix\n");
            printf("  .text\n");
            rt_proc_reset();
            g_frame_active = 1;
            for (int _pi = 0; _pi < s2->proc_count; _pi++) {
                const char *pname = s2->proc_table[_pi].name;
                if (!pname || strcmp(pname, "main") == 0) continue;
                int idx = s2->proc_table[_pi].bb_idx;
                if (idx < 0 || idx >= s2->bbp.count || !s2->bbp.table[idx] || !s2->bbp.table[idx]->entry) continue;
                int is_dup = 0;
                for (int _pj = _pi + 1; _pj < s2->proc_count; _pj++) { if (s2->proc_table[_pj].name && strcmp(s2->proc_table[_pj].name, pname) == 0) { is_dup = 1; break; } }
                if (is_dup) continue;
                int np = s2->proc_table[_pi].nparams;
                const char **pn = NULL;
                if (np > 0) { pn = (const char **)calloc((size_t)np, sizeof(const char *)); for (int k = 0; k < np && k < s2->proc_table[_pi].lower_sc.n; k++) pn[k] = s2->proc_table[_pi].lower_sc.e[k].name; }
                rt_proc_register(pname, pn, np);
                { extern void rt_proc_set_frame(const char *, int, int); extern void rt_proc_set_byref(const char *, uint64_t);
                  if (s2->bbp.table[idx]->nslots > 0) rt_proc_set_frame(pname, s2->bbp.table[idx]->nslots - 1, s2->proc_table[_pi].decl_level);
                  rt_proc_set_byref(pname, s2->proc_table[_pi].byref_mask); }
            }
            { extern int g_proc_direct_active; extern void proc_collect_reset(void); extern void proc_collect_graph(IR_graph_t *); extern int proc_slot_count(void);
              proc_collect_reset(); proc_collect_graph(sbbg);
              for (int _pi = 0; _pi < s2->proc_count; _pi++) { const char *pn2 = s2->proc_table[_pi].name; if (!pn2 || strcmp(pn2, "main") == 0) continue; int idx2 = s2->proc_table[_pi].bb_idx; if (idx2 < 0 || idx2 >= s2->bbp.count || !s2->bbp.table[idx2] || !s2->bbp.table[idx2]->entry) continue; proc_collect_graph(s2->bbp.table[idx2]); }
              g_proc_direct_active = (proc_slot_count() > 0) ? 1 : 0; }
            int _pbcap = (s2->proc_count > 0) ? s2->proc_count : 1;
            int *pidx_buf = (int *)malloc((size_t)_pbcap * sizeof(int));
            int *peak_buf = (int *)malloc((size_t)_pbcap * sizeof(int));
            int n_procs = 0;
            for (int _pi = 0; _pi < s2->proc_count; _pi++) {
                const char *pname = s2->proc_table[_pi].name;
                if (!pname || strcmp(pname, "main") == 0) continue;
                int idx = s2->proc_table[_pi].bb_idx;
                if (idx < 0 || idx >= s2->bbp.count || !s2->bbp.table[idx] || !s2->bbp.table[idx]->entry) continue;
                int is_dup = 0;
                for (int _pj = _pi + 1; _pj < s2->proc_count; _pj++) { if (s2->proc_table[_pj].name && strcmp(s2->proc_table[_pj].name, pname) == 0) { is_dup = 1; break; } }
                if (is_dup) continue;
                int np = s2->proc_table[_pi].nparams;
                const char **pn = NULL;
                if (np > 0) {
                    pn = (const char **)calloc((size_t)np, sizeof(const char *));
                    for (int k = 0; k < np && k < s2->proc_table[_pi].lower_sc.n; k++)
                        pn[k] = s2->proc_table[_pi].lower_sc.e[k].name;
                }
                rt_proc_register(pname, pn, np);
                { extern void rt_proc_set_frame(const char *, int, int); extern void rt_proc_set_byref(const char *, uint64_t); extern int g_emit_frame_caller_dl;
                  if (s2->bbp.table[idx]->nslots > 0) rt_proc_set_frame(pname, s2->bbp.table[idx]->nslots - 1, s2->proc_table[_pi].decl_level);
                  rt_proc_set_byref(pname, s2->proc_table[_pi].byref_mask);
                  g_emit_frame_caller_dl = (s2->bbp.table[idx]->nslots > 0) ? s2->proc_table[_pi].decl_level : -1; }
                gvar_flat_chain_build_text_at(s2->bbp.table[idx], s2->proc_table[_pi].proc_entry_node, stdout, pname);
                { extern int g_emit_frame_caller_dl; g_emit_frame_caller_dl = -1; }
                { extern int g_last_flat_frame_bytes; peak_buf[n_procs] = g_last_flat_frame_bytes; }
                pidx_buf[n_procs++] = _pi;
            }
            if (n_procs > 0) {
                printf("  .section .rodata\n");
                for (int i = 0; i < n_procs; i++) {
                    ProcEntry *pe = &s2->proc_table[pidx_buf[i]];
                    printf("  .Lpn%d: .string \"%s\"\n", i, pe->name);
                    for (int k = 0; k < pe->nparams && k < pe->lower_sc.n; k++)
                        printf("  .Lpp%d_%d: .string \"%s\"\n", i, k, pe->lower_sc.e[k].name ? pe->lower_sc.e[k].name : "");
                    printf("  .Lpnames%d:\n", i);
                    for (int k = 0; k < pe->nparams && k < pe->lower_sc.n; k++) printf("  .quad .Lpp%d_%d\n", i, k);
                    printf("  .quad 0\n");
                }
                printf("  .section .text\n  .intel_syntax noprefix\n");
                printf("proc_startup:\n  push rbp\n  mov rbp, rsp\n  call core_lib_init@PLT\n  call rt_proc_reset@PLT\n");
                for (int i = 0; i < n_procs; i++) {
                    ProcEntry *pe = &s2->proc_table[pidx_buf[i]];
                    printf("  lea rdi, [rip + .Lpn%d]\n", i);
                    printf("  lea rsi, [rip + .Lpnames%d]\n", i);
                    printf("  mov edx, %d\n", pe->nparams);
                    printf("  call rt_proc_register@PLT\n");
                    printf("  lea rdi, [rip + .Lpn%d]\n", i);
                    printf("  lea rsi, [rip + %s_\xce\xb1]\n", pe->name);
                    printf("  call rt_proc_set_fn@PLT\n");
                    int _fidx = pe->bb_idx;
                    if (_fidx >= 0 && _fidx < s2->bbp.count && s2->bbp.table[_fidx] && s2->bbp.table[_fidx]->nslots > 0) {
                        printf("  lea rdi, [rip + .Lpn%d]\n", i);
                        printf("  mov esi, %d\n", s2->bbp.table[_fidx]->nslots - 1);
                        printf("  mov edx, %d\n", pe->decl_level);
                        printf("  call rt_proc_set_frame@PLT\n");
                    }
                    if (peak_buf[i] > 0) {
                        printf("  lea rdi, [rip + .Lpn%d]\n", i);
                        printf("  mov esi, %d\n", peak_buf[i]);
                        printf("  call rt_proc_set_frame_bytes@PLT\n");
                    }
                }
                printf("  pop rbp\n  ret\n");
            }
            free(pidx_buf); free(peak_buf);
            extern void gva_collect_reset(void); extern void gva_collect_graph(IR_graph_t *); extern int gva_count(void); extern const char *gva_name(int); extern int g_gva_active;
            extern int proc_slot_count(void); extern int g_proc_direct_active;
            gva_collect_reset();
            gva_collect_graph(sbbg);
            int n_gva = gva_count();
            if (n_gva > 0) {
                printf("  .section .rodata\n");
                for (int k = 0; k < n_gva; k++) printf("  .Lgvan%d: .string \"%s\"\n", k, gva_name(k));
                printf("  .align 8\n__gva_names:\n");
                for (int k = 0; k < n_gva; k++) printf("  .quad .Lgvan%d\n", k);
                printf("  .section .bss\n  .align 16\n__gva: .space %d, 0\n", n_gva * 16);
                printf("  .section .text\n  .intel_syntax noprefix\n");
            }
            int n_proc_slot = proc_slot_count();
            if (n_proc_slot > 0) {
                extern const char *proc_slot_name(int);
                printf("  .section .rodata\n");
                for (int k = 0; k < n_proc_slot; k++) printf("  .Lprocn%d: .string \"%s\"\n", k, proc_slot_name(k));
                printf("  .align 8\n__proc_names:\n");
                for (int k = 0; k < n_proc_slot; k++) printf("  .quad .Lprocn%d\n", k);
                printf("  .section .bss\n  .align 8\n__proc: .space %d, 0\n", n_proc_slot * 8);
                printf("  .section .text\n  .intel_syntax noprefix\n");
            }
            printf("  .globl main\nmain:\n  push rbp\n  mov rbp, rsp\n");
            if (n_procs > 0) printf("  call proc_startup\n");
            else printf("  call core_lib_init@PLT\n  call rt_proc_reset@PLT\n");
            if (n_proc_slot > 0) printf("  lea rdi, [rip + __proc]\n  lea rsi, [rip + __proc_names]\n  mov edx, %d\n  call rt_proc_table_fill@PLT\n", n_proc_slot);
            if (n_gva > 0) printf("  lea rdi, [rip + __gva_names]\n  lea rsi, [rip + __gva]\n  mov edx, %d\n  call gva_register@PLT\n  mov rbx, rax\n", n_gva);
            printf("  call rt_frame@PLT\n  mov rdi, rax\n  xor esi, esi\n");
            printf("  call flat_\xce\xb1\n");
            printf("  xor eax, eax\n  pop rbp\n  ret\n");
            g_gva_active = (n_gva > 0) ? 1 : 0;
            int rc = gvar_flat_chain_build_text(sbbg, stdout, "flat");
            g_gva_active = 0;
            g_proc_direct_active = 0;
            g_frame_active = 0;
            xa_emit_strtab_rodata();
            fflush(stdout);
            ir_delete_all(s2);
            return rc;
        }
    }
    if (mode_compile && target_name && strcmp(target_name, "x86") != 0) {
        fprintf(stderr, "[SMX] --target=%s removed (Stack-Machine codegen removed).\n",
                target_name ? target_name : "?");
        ast_tree_free(ast_prog); ast_prog = NULL;
        return 1;
    }
    if (mode_run) {
        extern int g_postfix_resume;
        if (is_icon) g_postfix_resume = 1;
        stage2_t *s2 = sm_preamble(ast_prog);
        if (!s2) return 1;
        ast_tree_free(ast_prog); ast_prog = NULL;
        if (is_icon || is_raku) {
            extern void rt_proc_register(const char *name, const char **pnames, int nparams);
            extern void rt_proc_reset(void);
            extern bb_box_fn descr_flat_chain_build_proc(IR_t * entry, const char ** pnames, int np);
            extern void rt_proc_set_fn(const char *name, bb_box_fn fn);
            extern int g_frame_active;
            int main_bb_idx = -1;
            rt_proc_reset();
            g_frame_active = 1;
            void *m3_gva_arena = (void *)0;
            {
                extern void gva_collect_reset(void); extern void gva_collect_icon_globals(void); extern int gva_count(void); extern const char *gva_name(int); extern int g_gva_active;
                gva_collect_reset();
                gva_collect_icon_globals();
                int n_gva_m3 = gva_count();
                if (n_gva_m3 > 0) {
                    m3_gva_arena = calloc((size_t)n_gva_m3, sizeof(DESCR_t));
                    const char **m3_gva_nms = (const char **)malloc((size_t)n_gva_m3 * sizeof(const char *));
                    for (int _k = 0; _k < n_gva_m3; _k++) m3_gva_nms[_k] = gva_name(_k);
                    if (m3_gva_arena && m3_gva_nms) { gva_register(m3_gva_nms, (DESCR_t *)m3_gva_arena, n_gva_m3); g_gva_active = 1; }
                }
                if (getenv("SCRIP_M3_GVA_TRACE")) fprintf(stderr, "[M3-GVA] m3 globals via rbx-arena: active=%d n_gva=%d\n", g_gva_active, n_gva_m3);
            }
            for (int _pi = 0; _pi < s2->proc_count; _pi++) {
                const char *pname = s2->proc_table[_pi].name;
                if (!pname) continue;
                if (strcmp(pname, "main") == 0) { main_bb_idx = s2->proc_table[_pi].bb_idx; continue; }
                int idx = s2->proc_table[_pi].bb_idx;
                if (idx < 0 || idx >= s2->bbp.count || !s2->bbp.table[idx] || !s2->bbp.table[idx]->entry) continue;
                int np = s2->proc_table[_pi].nparams;
                const char **pn = NULL;
                if (np > 0) {
                    pn = (const char **)calloc((size_t)np, sizeof(const char *));
                    for (int k = 0; k < np && k < s2->proc_table[_pi].lower_sc.n; k++)
                        pn[k] = s2->proc_table[_pi].lower_sc.e[k].name;
                }
                rt_proc_register(pname, pn, np);
                { extern void rt_proc_set_generator(const char *, int); rt_proc_set_generator(pname, s2->proc_table[_pi].is_generator); }
            }
            if (is_icon) icn_register_record_types(s2);
            if (is_icon) { extern void optimizer_run(IR_graph_t * g); for (int _gi = 0; _gi < s2->bbp.count; _gi++) if (s2->bbp.table[_gi]) optimizer_run(s2->bbp.table[_gi]); }
            if (is_icon) { extern void ir_drive_slot_assign(IR_graph_t * g); for (int _gi = 0; _gi < s2->bbp.count; _gi++) if (s2->bbp.table[_gi]) ir_drive_slot_assign(s2->bbp.table[_gi]); }
            if (is_raku && !graph_native_emittable_mode(s2, 1)) {
                fprintf(stderr, "[SMX] --run: mode-3 native emitter does not yet cover this program "
                                "(a box has no MEDIUM_BINARY arm — Raku map/grep). REJECTED — native BB emission pending (no interpreter fallback).\n");
                return 0;
            }
            for (int _pi = 0; _pi < s2->proc_count; _pi++) {
                const char *pname = s2->proc_table[_pi].name;
                if (!pname || strcmp(pname, "main") == 0) continue;
                int idx = s2->proc_table[_pi].bb_idx;
                if (idx < 0 || idx >= s2->bbp.count || !s2->bbp.table[idx] || !s2->bbp.table[idx]->entry) continue;
                int np = s2->proc_table[_pi].nparams;
                const char **pn = NULL;
                if (np > 0) {
                    pn = (const char **)calloc((size_t)np, sizeof(const char *));
                    for (int k = 0; k < np && k < s2->proc_table[_pi].lower_sc.n; k++)
                        pn[k] = s2->proc_table[_pi].lower_sc.e[k].name;
                }
                { extern IR_graph_t *g_emit_cfg; g_emit_cfg = s2->bbp.table[idx]; }
                resolve_call_kinds_descr(s2->bbp.table[idx]);
                { extern void rt_proc_set_generator(const char *, int); rt_proc_set_generator(pname, s2->proc_table[_pi].is_generator); }
                { extern int g_gen_proc_active; g_gen_proc_active = s2->proc_table[_pi].is_generator; }
                bb_box_fn pfn = descr_flat_chain_build_proc(s2->bbp.table[idx]->entry, pn, np);
                { extern int g_gen_proc_active; g_gen_proc_active = 0; }
                if (pfn) rt_proc_set_fn(pname, pfn);
            }
            {
                extern void *dat_register(const char *spec);
                for (int _gi = 0; _gi < s2->bbp.count; _gi++) {
                    IR_graph_t *g = s2->bbp.table[_gi];
                    if (!g || !g->all) continue;
                    for (int _ni = 0; _ni < g->n; _ni++) {
                        IR_t *nd = g->all[_ni];
                        if (nd && nd->op == IR_OP_COUNT && IR_LIT(nd).sval) dat_register(IR_LIT(nd).sval);
                    }
                }
            }
            if (main_bb_idx < 0 || main_bb_idx >= s2->bbp.count || !s2->bbp.table[main_bb_idx]) {
                fprintf(stderr, "[IBB] FATAL: mode-3 driver: main BB graph not found\n");
                abort();
            }
            IR_graph_t * bbg = s2->bbp.table[main_bb_idx];
            if (!bbg->entry) {
                fprintf(stderr, "[IBB] FATAL: mode-3 driver: main BB graph has no entry\n");
                abort();
            }
            extern void *rt_frame(void);
            extern bb_box_fn descr_flat_chain_build(IR_t * entry);
            bb_box_fn fn;
            { extern IR_graph_t *g_emit_cfg; g_emit_cfg = bbg; }
            resolve_call_kinds_descr(bbg);
            fn = descr_flat_chain_build(bbg->entry);
            g_frame_active = 0;
            if (!fn) {
                fprintf(stderr, "[IBB] FATAL: mode-3 driver: bb_build_flat returned NULL — BB template(s) lack MEDIUM_BINARY arm\n");
                abort();
            }
            ir_delete_all(s2);
            { extern int g_gva_active; if (g_gva_active && m3_gva_arena) m3_enter_with_rbx(fn, rt_frame(), 0, m3_gva_arena); else (void)fn(rt_frame(), 0); }
            goto run_done;
        }
        if (is_prolog) {
            fprintf(stderr, "GROUND ZERO #5: Prolog backend deleted (Icon-only reset; Prolog rebuilds later).\n");
            return 1;
        }
        {
            if (is_prolog) {
                fprintf(stderr, "GROUND ZERO #5: Prolog backend deleted (Icon-only reset; Prolog rebuilds later).\n");
                return 1;
            }
            extern bb_box_fn gvar_flat_chain_build(IR_graph_t * g);
            extern bb_box_fn gvar_flat_chain_build_at(IR_graph_t * g, IR_t * entry_node, const char * prefix);
            extern void *rt_frame(void);
            extern int g_frame_active;
            extern void rt_proc_register(const char *name, const char **pnames, int nparams);
            extern void rt_proc_set_fn(const char *name, bb_box_fn fn);
            extern void rt_proc_reset(void);
            int main_bb_idx = -1;
            for (int _pi = 0; _pi < s2->proc_count; _pi++)
                if (s2->proc_table[_pi].name && strcmp(s2->proc_table[_pi].name, "main") == 0) { main_bb_idx = s2->proc_table[_pi].bb_idx; break; }
            rt_proc_reset();
            void *m3_gva_arena = (void *)0;
            if (is_pascal) {
                extern void gva_collect_reset(void); extern void gva_collect_graph(IR_graph_t *); extern int gva_count(void); extern const char *gva_name(int); extern int g_gva_active;
                gva_collect_reset();
                IR_graph_t *_mg = (main_bb_idx >= 0 && main_bb_idx < s2->bbp.count) ? s2->bbp.table[main_bb_idx] : (IR_graph_t *)0;
                if (_mg) gva_collect_graph(_mg);
                int n_gva_m3 = gva_count();
                if (n_gva_m3 > 0) {
                    m3_gva_arena = calloc((size_t)n_gva_m3, sizeof(DESCR_t));
                    const char **m3_gva_nms = (const char **)malloc((size_t)n_gva_m3 * sizeof(const char *));
                    for (int _k = 0; _k < n_gva_m3; _k++) m3_gva_nms[_k] = gva_name(_k);
                    if (m3_gva_arena && m3_gva_nms) { gva_register(m3_gva_nms, (DESCR_t *)m3_gva_arena, n_gva_m3); g_gva_active = 1; }
                }
                if (getenv("SCRIP_M3_GVA_TRACE")) fprintf(stderr, "[M3-GVA] m3 globals via rbx-arena: active=%d n_gva=%d\n", g_gva_active, n_gva_m3);
            }
            g_frame_active = 1;
            for (int _pi = 0; _pi < s2->proc_count; _pi++) {
                const char *pname = s2->proc_table[_pi].name;
                if (!pname || strcmp(pname, "main") == 0) continue;
                int idx = s2->proc_table[_pi].bb_idx;
                if (idx < 0 || idx >= s2->bbp.count || !s2->bbp.table[idx] || !s2->bbp.table[idx]->entry) continue;
                int np = s2->proc_table[_pi].nparams;
                const char **pn = NULL;
                if (np > 0) { pn = (const char **)calloc((size_t)np, sizeof(const char *)); for (int k = 0; k < np && k < s2->proc_table[_pi].lower_sc.n; k++) pn[k] = s2->proc_table[_pi].lower_sc.e[k].name; }
                rt_proc_register(pname, pn, np);
                { extern void rt_proc_set_frame(const char *, int, int); extern void rt_proc_set_byref(const char *, uint64_t);
                  if (s2->bbp.table[idx]->nslots > 0) rt_proc_set_frame(pname, s2->bbp.table[idx]->nslots - 1, s2->proc_table[_pi].decl_level);
                  rt_proc_set_byref(pname, s2->proc_table[_pi].byref_mask); }
            }
            for (int _pi = 0; _pi < s2->proc_count; _pi++) {
                const char *pname = s2->proc_table[_pi].name;
                if (!pname || strcmp(pname, "main") == 0) continue;
                int idx = s2->proc_table[_pi].bb_idx;
                if (idx < 0 || idx >= s2->bbp.count || !s2->bbp.table[idx] || !s2->bbp.table[idx]->entry) continue;
                int np = s2->proc_table[_pi].nparams;
                const char **pn = NULL;
                if (np > 0) {
                    pn = (const char **)calloc((size_t)np, sizeof(const char *));
                    for (int k = 0; k < np && k < s2->proc_table[_pi].lower_sc.n; k++)
                        pn[k] = s2->proc_table[_pi].lower_sc.e[k].name;
                }
                rt_proc_register(pname, pn, np);
                { extern void rt_proc_set_frame(const char *, int, int); extern void rt_proc_set_byref(const char *, uint64_t); extern int g_emit_frame_caller_dl;
                  if (s2->bbp.table[idx]->nslots > 0) rt_proc_set_frame(pname, s2->bbp.table[idx]->nslots - 1, s2->proc_table[_pi].decl_level);
                  rt_proc_set_byref(pname, s2->proc_table[_pi].byref_mask);
                  g_emit_frame_caller_dl = (s2->bbp.table[idx]->nslots > 0) ? s2->proc_table[_pi].decl_level : -1; }
                bb_box_fn pfn = gvar_flat_chain_build_at(s2->bbp.table[idx], s2->proc_table[_pi].proc_entry_node, pname);
                { extern int g_emit_frame_caller_dl; g_emit_frame_caller_dl = -1; }
                { extern int g_last_flat_frame_bytes; extern void rt_proc_set_frame_bytes(const char *, int); rt_proc_set_frame_bytes(pname, g_last_flat_frame_bytes); }
                if (pfn) rt_proc_set_fn(pname, pfn);
            }
            g_frame_active = 0;
            IR_graph_t *sbbg = (main_bb_idx >= 0 && main_bb_idx < s2->bbp.count) ? s2->bbp.table[main_bb_idx] : NULL;
            if (sbbg && sbbg->entry) {
                g_frame_active = 1;
                bb_box_fn fn = gvar_flat_chain_build(sbbg);
                g_frame_active = 0;
                ir_delete_all(s2);
                if (fn) { extern int g_gva_active; if (g_gva_active && m3_gva_arena) m3_enter_with_rbx(fn, rt_frame(), 0, m3_gva_arena); else (void)fn(rt_frame(), 0); { extern int g_gva_active; g_gva_active = 0; } goto run_done; }
            }
            fprintf(stderr, "[SBB] mode-3: SNOBOL4 statement shape not yet flat-emittable (a box lacks a "
                            "MEDIUM_BINARY arm); soft fall — no output for this shape. No abort.\n");
            goto run_done;
        }
    } else if (has_non_sno) {
        (void)sm_preamble;
        fprintf(stderr, "[MODE] FATAL: reached dead has_non_sno branch — mode resolution is broken; "
                        "refusing to silently run the SM interpreter. Aborting.\n");
        abort();
    } else {
        fprintf(stderr, "[MODE] FATAL: reached dead default dispatch branch — no execution mode "
                        "selected; refusing to silently run the SM interpreter. Aborting.\n");
        abort();
    }
run_done:
    if (opt_bench) {
        clock_gettime(CLOCK_MONOTONIC, &_t3);
        double parse_ms = (_t1.tv_sec - _t0.tv_sec)*1e3 + (_t1.tv_nsec - _t0.tv_nsec)/1e6;
        double lower_ms = (_t2.tv_sec - _t1.tv_sec)*1e3 + (_t2.tv_nsec - _t1.tv_nsec)/1e6;
        double exec_ms  = (_t3.tv_sec - _t2.tv_sec)*1e3 + (_t3.tv_nsec - _t2.tv_nsec)/1e6;
        fprintf(stderr, "BENCH parse=%.2fms lower=%.2fms exec=%.2fms total=%.2fms\n",
                parse_ms, lower_ms, exec_ms, parse_ms + lower_ms + exec_ms);
    }
    if (getenv("BINARY_AUDIT") || getenv("SNO_BINARY_BOXES")) {
        extern void bin_audit_print(void);
        bin_audit_print();
    }
    return 0;
}
