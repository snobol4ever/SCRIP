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
extern void ir_set_print_width(int w);
extern void ir_print_node_nl(const tree_t *e, FILE *f);
#include "core.h"
#include "sil_macros.h"
#include "runtime_shim.h"
#include "lower.h"
#include "IR_interp_state.h"
#include "bb_build.h"
#include "emit.h"
#include "emit_bb.h"
#include "emit_core.h"
#include "scrip_sm.h"
#include "sync_monitor.h"
extern DESCR_t pat_at_cursor(const char *varname);
static void stmt_init(void) {}
extern DESCR_t      eval_expr(const char *src);
extern const char  *exec_code(DESCR_t code_block);
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
static int icn_rt_arity(const IR_t *n) {
    switch (n->op) {
    case IR_LIT_I: case IR_LIT_S: case IR_LIT_F: case IR_LIT_NUL:
    case IR_VAR:   case IR_KEYWORD: return 0;
    case IR_BINOP: case IR_BINOP_GEN: return 2;
    case IR_TO:    case IR_TO_BY:     return 2;
    case IR_UNOP:  case IR_NEG: case IR_POS: case IR_NONNULL: case IR_NOT: case IR_SIZE: return 1;
    case IR_EVERY: return 1;
    case IR_CALL:  return (int)IR_LIT(n).ival;
    default:       return -1;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * icn_ring_to_tree(IR_graph_t *g) {
    if (!g || !g->entry) return NULL;
    IR_t *chain[256]; int nc = 0;
    for (IR_t *cur = g->entry; cur && cur->op != IR_SUCCEED && cur->op != IR_FAIL && nc < 256; cur = cur->γ.node) chain[nc++] = cur;
    if (nc == 0 || nc >= 256) return NULL;
    for (int i = 0; i < nc; i++) if (chain[i]->op == IR_BINOP || chain[i]->op == IR_LIT_I || chain[i]->op == IR_LIT_S || chain[i]->op == IR_LIT_F || chain[i]->op == IR_LIT_NUL) return NULL;
    IR_t *stk[256]; int sp = 0;
    for (int i = 0; i < nc; i++) {
        IR_t *n = chain[i];
        int ar = icn_rt_arity(n);
        if (ar < 0 || ar > sp) return NULL;
        if (n->op == IR_CALL) {
            if (ar != 1) return NULL;
            n->n_operands = 0; if (!ir_operand_push(n, stk[sp - 1])) return NULL; sp -= 1;
            IR_LIT(n).dval = 0.0;
        } else if (ar == 2) {
            n->n_operands = 0;
            if (!ir_operand_push(n, stk[sp - 2])) return NULL;
            if (!ir_operand_push(n, stk[sp - 1])) return NULL;
            sp -= 2;
        } else if (ar == 1) {
            n->n_operands = 0;
            if (!ir_operand_push(n, stk[sp - 1])) return NULL;
            sp -= 1;
        }
        stk[sp++] = n;
    }
    if (sp != 1) return NULL;
    return stk[0];
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int icn_kind_native_stub(IR_e t) {
    return t == IR_GEN_ALT || t == IR_KEYWORD || t == IR_PROC_GEN ||
           t == IR_CSET_UNION || t == IR_CSET_DIFF || t == IR_CSET_INTER || t == IR_CSET_COMPL ||
           t == IR_SUSPEND ||
           t == IR_LIST_BANG ||
           t == IR_BINOP_GEN ||
           t == IR_MAP || t == IR_GREP;
}
static int icn_alt_arms_all_simple_lit(const IR_graph_t *g, IR_t *alt) {
    int n = 0;
    IR_t * const * arms = bb_operand_aux_get(g, alt, &n);
    if (!arms || n <= 0 || n > 5) return 0;
    for (int i = 0; i < n; i++) {
        if (!arms[i]) return 0;
        if (arms[i]->op != IR_LIT_I && arms[i]->op != IR_LIT_S) return 0;
    }
    return 1;
}
static int icn_alt_safe_kind(IR_e t) {
    return t == IR_ALT || t == IR_CALL || t == IR_EVERY || t == IR_FAIL ||
           t == IR_SUCCEED || t == IR_LIT_I || t == IR_LIT_S || t == IR_LIT_F || t == IR_LIT_NUL;
}
static int icn_graph_has_alt(const IR_graph_t *g) {
    for (int ni = 0; ni < g->n; ni++) if (g->all[ni] && g->all[ni]->op == IR_ALT) return 1;
    return 0;
}
static int icn_keyword_supported(const char *kw) {
    if (!kw) return 0;
    if (kw[0] == '&') kw++;
    return !strcmp(kw, "subject") || !strcmp(kw, "pos") || !strcmp(kw, "null") || !strcmp(kw, "fail");
}
static int icn_scan_safe_kind(IR_e t) {
    return t == IR_SUCCEED || t == IR_FAIL ||
           t == IR_LIT_I || t == IR_LIT_S || t == IR_LIT_F || t == IR_LIT_NUL ||
           t == IR_VAR || t == IR_KEYWORD || t == IR_GEN_SCAN || t == IR_CALL || t == IR_BINOP || t == IR_EVERY;
}
static IR_t *icn_scan_lit_entry(IR_t *nd, IR_e want) {
    IR_graph_t **sblks = (IR_graph_t **)(intptr_t) IR_EXEC(nd).counter;
    IR_t *ae = (sblks && (int)IR_LIT(nd).ival == 1 && sblks[0]) ? sblks[0]->entry : (IR_t *)0;
    if (!ae || ae->op != want) return (IR_t *)0;
    if (ae->γ.node && ae->γ.node->op != IR_SUCCEED) return (IR_t *)0;
    return ae;
}
static int icn_scan_fn_lit_arg(IR_t *nd, IR_e want) {
    return icn_scan_lit_entry(nd, want) != (IR_t *)0;
}
static int icn_scan_tab_arg_ok(IR_t *nd) {
    IR_graph_t **sblks = (IR_graph_t **)(intptr_t) IR_EXEC(nd).counter;
    IR_t *ae = (sblks && (int)IR_LIT(nd).ival == 1 && sblks[0]) ? sblks[0]->entry : (IR_t *)0;
    if (!ae) return 0;
    if (ae->γ.node && ae->γ.node->op != IR_SUCCEED) return 0;
    if (ae->op == IR_LIT_I && IR_LIT(ae).ival >= 1) return 1;
    if (ae->op == IR_CALL && IR_LIT(ae).dval == 3.0 && IR_LIT(ae).sval && (!strcmp(IR_LIT(ae).sval, "any") || !strcmp(IR_LIT(ae).sval, "match") || !strcmp(IR_LIT(ae).sval, "many") || !strcmp(IR_LIT(ae).sval, "upto") || !strcmp(IR_LIT(ae).sval, "find") || !strcmp(IR_LIT(ae).sval, "bal")) && icn_scan_fn_lit_arg(ae, IR_LIT_S)) return 1;
    return 0;
}
static int icn_graph_var_assigned_or_param(stage2_t *s2, int gi, IR_graph_t *g, const char *name);
static int icn_scan_subgraph_safe(stage2_t *s2, int gi, IR_graph_t *g, IR_graph_t *sg, int depth) {
    if (!sg || !sg->all || sg->n <= 0 || depth > 16) return 0;
    for (int i = 0; i < sg->n; i++) {
        IR_t *nd = sg->all[i];
        if (!nd) continue;
        if (!icn_scan_safe_kind(nd->op)) return 0;
        if (nd->op == IR_VAR) {
            if (IR_LIT(nd).sval && IR_LIT(nd).sval[0] == '&') { if (!icn_keyword_supported(IR_LIT(nd).sval)) return 0; }
            else if (IR_EXEC(nd).state == 1 || !IR_LIT(nd).sval || !icn_graph_var_assigned_or_param(s2, gi, g, IR_LIT(nd).sval)) return 0;
        }
        if (nd->op == IR_KEYWORD && !icn_keyword_supported(IR_LIT(nd).sval)) return 0;
        if (nd->op == IR_CALL) {
            if (!IR_LIT(nd).sval) return 0;
            if (!strcmp(IR_LIT(nd).sval, "any") || !strcmp(IR_LIT(nd).sval, "match") || !strcmp(IR_LIT(nd).sval, "many") || !strcmp(IR_LIT(nd).sval, "upto")) { if (!(IR_LIT(nd).dval == 3.0 && icn_scan_fn_lit_arg(nd, IR_LIT_S))) return 0; }
            else if (!strcmp(IR_LIT(nd).sval, "tab")) { if (!(IR_LIT(nd).dval == 3.0 && icn_scan_tab_arg_ok(nd))) return 0; }
            else if (!strcmp(IR_LIT(nd).sval, "move")) { if (!(IR_LIT(nd).dval == 3.0 && icn_scan_fn_lit_arg(nd, IR_LIT_I))) return 0; }
            else if (!strcmp(IR_LIT(nd).sval, "pos")) { IR_t *pe = icn_scan_lit_entry(nd, IR_LIT_I); if (!(IR_LIT(nd).dval == 3.0 && pe && IR_LIT(pe).ival >= 1)) return 0; }
            else if (!strcmp(IR_LIT(nd).sval, "find")) { IR_t *fe = icn_scan_lit_entry(nd, IR_LIT_S); if (!(IR_LIT(nd).dval == 3.0 && fe && IR_LIT(fe).sval && IR_LIT(fe).sval[0] && strlen(IR_LIT(fe).sval) <= 32)) return 0; }
            else if (!strcmp(IR_LIT(nd).sval, "bal")) { IR_t *be = icn_scan_lit_entry(nd, IR_LIT_S); if (!(IR_LIT(nd).dval == 3.0 && be && IR_LIT(be).sval && IR_LIT(be).sval[0] && !strchr(IR_LIT(be).sval, 40) && !strchr(IR_LIT(be).sval, 41))) return 0; }
            else if (!(!strcmp(IR_LIT(nd).sval, "write") || !strcmp(IR_LIT(nd).sval, "writes"))) return 0;
        }
        if (nd->op == IR_BINOP && IR_LIT(nd).ival != BINOP_CONCAT) return 0;
        if (nd->op == IR_GEN_SCAN) {
            IR_graph_t *ssg = (IR_graph_t *)(intptr_t) IR_EXEC(nd).counter;
            IR_graph_t *bsg = (IR_graph_t *)(intptr_t) IR_LIT(nd).ival;
            if (!icn_scan_subgraph_safe(s2, gi, g, ssg, depth + 1) || !icn_scan_subgraph_safe(s2, gi, g, bsg, depth + 1)) return 0;
        }
    }
    return 1;
}
static int icn_graph_native_emittable_mode(stage2_t *s2, int for_run);
static int icn_graph_native_emittable(stage2_t *s2) { return icn_graph_native_emittable_mode(s2, 0); }
static int icn_gen_scan_body_slotful(IR_t *r) {
    if (!r || r->op != IR_GEN_SCAN || IR_LIT(r).dval != 1.0) return 0;
    IR_graph_t *bsg = (IR_graph_t *)(intptr_t) IR_LIT(r).ival;
    IR_t *bt = bsg ? bsg->entry : (IR_t *)0;
    int gd = 0;
    while (bt && bt->γ.node && bt->γ.node->op != IR_SUCCEED && bt->γ.node->op != IR_FAIL && gd++ < 512) bt = bt->γ.node;
    if (bt && (bt->op == IR_LIT_I || bt->op == IR_LIT_S)) return 1;
    if (bt && bt->op == IR_VAR && IR_LIT(bt).sval && IR_LIT(bt).sval[0] != '&') return 1;
    if (bt && bt->op == IR_CALL && IR_LIT(bt).dval == 3.0 && IR_LIT(bt).sval && (!strcmp(IR_LIT(bt).sval, "tab") || !strcmp(IR_LIT(bt).sval, "move") || !strcmp(IR_LIT(bt).sval, "pos") || !strcmp(IR_LIT(bt).sval, "any") || !strcmp(IR_LIT(bt).sval, "match") || !strcmp(IR_LIT(bt).sval, "many") || !strcmp(IR_LIT(bt).sval, "upto") || !strcmp(IR_LIT(bt).sval, "find") || !strcmp(IR_LIT(bt).sval, "bal"))) return 1;
    return 0;
}
static int icn_rhs_kind_ok(IR_t *r) {
    if (!r) return 0;
    if (r->op == IR_LIT_I || r->op == IR_LIT_S) return 1;
    if (r->op == IR_VAR && IR_LIT(r).sval && IR_LIT(r).sval[0] != '&') return 1;
    if (r->op == IR_BINOP && (IR_LIT(r).ival == BINOP_ADD || IR_LIT(r).ival == BINOP_SUB || IR_LIT(r).ival == BINOP_MUL || IR_LIT(r).ival == BINOP_DIV || IR_LIT(r).ival == BINOP_MOD || IR_LIT(r).ival == BINOP_CONCAT)) return 1;
    if (r->op == IR_CALL && IR_LIT(r).dval == 0.0) return 1;
    if (r->op == IR_CALL && IR_LIT(r).dval == 1.0) return 1;
    if (r->op == IR_FIELD_GET) return 1;
    if (r->op == IR_CALL && IR_LIT(r).dval == 2.0 && !(IR_LIT(r).sval && (!strcmp(IR_LIT(r).sval,"__rk_bool")||!strcmp(IR_LIT(r).sval,"__rk_try")))) return 1;
    if (r->op == IR_GEN_SCAN) return icn_gen_scan_body_slotful(r);
    return 0;
}
static int icn_local_assign_rhs_ok(IR_t *nd) {
    IR_t *r = (nd->n_operands > 0) ? nd->operands[0] : ((IR_t*)0);
    return icn_rhs_kind_ok(r);
}
static int icn_local_assign_rhs_ok_g(const IR_graph_t *g, IR_t *nd) {
    if (nd->n_operands > 0) return icn_rhs_kind_ok(nd->operands[0]);
    for (int i = 0; i < g->n; i++) { IR_t *p = g->all[i]; if (p && p->γ.node == nd) return icn_rhs_kind_ok(p); }
    return 0;
}
static int icn_rk_arith_operand_ok(IR_t *r) {
    if (!r) return 0;
    if (r->op == IR_LIT_I) return 1;
    if (r->op == IR_BINOP && (IR_LIT(r).ival == BINOP_ADD || IR_LIT(r).ival == BINOP_SUB || IR_LIT(r).ival == BINOP_MUL || IR_LIT(r).ival == BINOP_DIV || IR_LIT(r).ival == BINOP_MOD)) return 1;
    return 0;
}
static int icn_rk_is_jct_call(IR_t *r) {
    return r && r->op == IR_CALL && IR_LIT(r).sval && !strncmp(IR_LIT(r).sval, "__rk_jct_", 9);
}
static int icn_rk_jct_marshallable(IR_t *r) {
    if (!r) return 0;
    if (r->op == IR_LIT_I || r->op == IR_LIT_S || r->op == IR_LIT_F || r->op == IR_LIT_NUL) return 1;
    if (r->op == IR_VAR && IR_LIT(r).sval && IR_LIT(r).sval[0] != '&') return 1;
    if (r->op == IR_CALL && (IR_LIT(r).dval == 2.0 || IR_LIT(r).dval == 3.0 || IR_LIT(r).dval == 5.0)) return 1;
    return 0;
}
static int icn_rk_bool_truthy_emittable(IR_t *nd) {
    if (!nd || nd->op != IR_CALL || !IR_LIT(nd).sval || strcmp(IR_LIT(nd).sval,"__rk_bool") || IR_LIT(nd).dval != 2.0) return 0;
    IR_graph_t **blks = (IR_graph_t **)(intptr_t) IR_EXEC(nd).counter;
    IR_graph_t *cond = blks ? blks[0] : (IR_graph_t *)0;
    if (!cond || !cond->entry) return 0;
    IR_t *e = cond->entry;
    return (e->op == IR_LIT_I || e->op == IR_LIT_S || (e->op == IR_VAR && IR_LIT(e).sval && IR_LIT(e).sval[0] != '&'));
}
static int icn_rk_bool_cond_emittable(IR_t *nd) {
    if (!nd || nd->op != IR_CALL || !IR_LIT(nd).sval || strcmp(IR_LIT(nd).sval, "__rk_bool") || IR_LIT(nd).dval != 2.0) return 0;
    IR_graph_t **blks = (IR_graph_t **)(intptr_t) IR_EXEC(nd).counter;
    IR_graph_t *cond = blks ? blks[0] : (IR_graph_t *)0;
    if (!cond) return 0;
    IR_t *p = cond->entry; IR_t *rel = (IR_t *)0; int gd = 0;
    while (p && gd++ < 256) { if (p->op == IR_BINOP && IR_LIT(p).ival >= BINOP_LT && IR_LIT(p).ival <= BINOP_NE) { rel = p; break; } if (!p->γ.node) break; p = p->γ.node; }
    if (!rel) return 0;
    IR_t *ra = ir_pair_arg(rel, 0); IR_t *rb = ir_pair_arg(rel, 1);
    if (!ra || !rb) { int n = 0; IR_t * const *aux = bb_operand_aux_get(cond, rel, &n); if (aux && n >= 2) { ra = aux[0]; rb = aux[1]; } }
    if ((icn_rk_is_jct_call(ra) || icn_rk_is_jct_call(rb)) && icn_rk_jct_marshallable(ra) && icn_rk_jct_marshallable(rb)) return 1;
    return icn_rk_arith_operand_ok(ra) && icn_rk_arith_operand_ok(rb);
}
static int icn_assign_safe_kind(IR_e t) {
    return t == IR_ASSIGN || t == IR_VAR || t == IR_CALL || t == IR_SUCCEED || t == IR_FAIL ||
           t == IR_LIT_I || t == IR_LIT_S || t == IR_LIT_F || t == IR_LIT_NUL || t == IR_FIELD_GET ||
           t == IR_BINOP || t == IR_IF || t == IR_WHILE || t == IR_UNTIL || t == IR_REPEAT ||
           t == IR_BREAK || t == IR_NEXT || t == IR_CONJ || t == IR_GEN_SCAN;
}
static int icn_graph_has_local_assign(const IR_graph_t *g) {
    extern int g_icn_globals_nv;
    for (int ni = 0; ni < g->n; ni++) {
        IR_t *nd = g->all[ni];
        if (nd && nd->op == IR_ASSIGN && IR_LIT(nd).sval && !(g_icn_globals_nv && is_global(IR_LIT(nd).sval))) return 1;
    }
    return 0;
}
static int icn_graph_has_binop(const IR_graph_t *g) {
    for (int ni = 0; ni < g->n; ni++) if (g->all[ni] && g->all[ni]->op == IR_BINOP) return 1;
    return 0;
}
static int icn_graph_var_assigned_or_param(stage2_t *s2, int gi, IR_graph_t *g, const char *name) {
    for (int i = 0; i < g->n; i++) { IR_t *m = g->all[i]; if (m && m->op == IR_ASSIGN && IR_LIT(m).sval && !strcmp(IR_LIT(m).sval, name)) return 1; }
    for (int p = 0; p < s2->proc_count; p++) {
        if (s2->proc_table[p].bb_idx != gi) continue;
        for (int k = 0; k < s2->proc_table[p].nparams && k < s2->proc_table[p].lower_sc.n; k++)
            if (s2->proc_table[p].lower_sc.e[k].name && !strcmp(s2->proc_table[p].lower_sc.e[k].name, name)) return 1;
    }
    return 0;
}
static int icn_graph_native_emittable_mode(stage2_t *s2, int for_run) {
    if (!s2) return 0;
    for (int gi = 0; gi < s2->bbp.count; gi++) {
        IR_graph_t *g = s2->bbp.table[gi];
        if (!g || !g->all) continue;
        int has_alt = icn_graph_has_alt(g);
        int has_lassign = icn_graph_has_local_assign(g);
        int has_binop = icn_graph_has_binop(g);
        for (int ni = 0; ni < g->n; ni++) {
            IR_t *nd = g->all[ni];
            if (!nd) continue;
            if (nd->op == IR_CASE) return 0;
            if (nd->op == IR_CALL && IR_LIT(nd).dval == 2.0 && IR_LIT(nd).sval && (!strcmp(IR_LIT(nd).sval,"__rk_bool")||!strcmp(IR_LIT(nd).sval,"__rk_try"))) { if (icn_rk_bool_cond_emittable(nd)||icn_rk_bool_truthy_emittable(nd)) {} else return 0; }
            if (nd->op == IR_GEN_SCAN) {
                if (IR_LIT(nd).dval != 1.0) return 0;
                IR_graph_t *ssg = (IR_graph_t *)(intptr_t) IR_EXEC(nd).counter;
                IR_graph_t *bsg = (IR_graph_t *)(intptr_t) IR_LIT(nd).ival;
                if (!icn_scan_subgraph_safe(s2, gi, g, ssg, 0) || !icn_scan_subgraph_safe(s2, gi, g, bsg, 0)) return 0;
                if (nd->γ.node && nd->γ.node->op == IR_CALL && !icn_gen_scan_body_slotful(nd)) return 0;
            }
            { extern int g_icn_globals_nv;
              if (nd->op == IR_VAR && IR_EXEC(nd).state == 1 && !g_icn_globals_nv) return 0;
              if (nd->op == IR_VAR && IR_LIT(nd).sval && IR_LIT(nd).sval[0] != '&' && IR_EXEC(nd).state != 1 && !icn_graph_var_assigned_or_param(s2, gi, g, IR_LIT(nd).sval)) return 0;
              if (nd->op == IR_ASSIGN && IR_LIT(nd).sval) {
                  int lhs_global = is_global(IR_LIT(nd).sval);
                  if (lhs_global && g_icn_globals_nv) { /* nv global assign: bb_gvar_assign_icn (BUILT) */ }
                  else if (icn_local_assign_rhs_ok_g(g, nd)) { /* wave-1 local assign: bb_assign_local (lit/var/binop rhs) */ }
                  else return 0; /* other rhs shapes: native store arm not built -> clean EXCISE, never abort */
              } }
            if (has_alt) {
                if (!icn_alt_safe_kind(nd->op)) return 0;
                if (nd->op == IR_ALT && !icn_alt_arms_all_simple_lit(g, nd)) return 0;
            }
        }
    }
    return 1;
}
/*====================================================================================================================*/
/*--------------------------------------------------------------------------------------------------------------------*/
static int pl_flat_arith_leaf_simple(const IR_t *o) {
    if (!o) return 0;
    return o->op == IR_LIT_I || o->op == IR_LOGICVAR;
}
static int pl_arith_op_floaty(const char *fn) {
    static const char *f[] = { "sqrt", "sin", "cos", "tan", "asin", "acos", "atan", "exp", "log",
                               "float", "float_integer_part", "float_fractional_part",
                               "truncate", "round", "ceiling", "floor", "integer", "/", NULL };
    for (int i = 0; f[i]; i++) if (!strcmp(fn, f[i])) return 1;
    return 0;
}
static int pl_flat_arith_leaf_float_ok(const IR_t *o) {
    if (!o) return 0;
    return o->op == IR_LIT_I || o->op == IR_LIT_F || o->op == IR_LOGICVAR;
}
static int pl_flat_goal_is_simple(const IR_t *g) {
    if (!g) return 0;
    switch (g->op) {
    case IR_SUCCEED: case IR_CUT: case IR_ATOM: return 1;
    case IR_BUILTIN: {
        const char *fn = IR_LIT(g).sval ? IR_LIT(g).sval : "";
        if (!strcmp(fn, "is")) {
            const IR_t *lhs = ir_pair_arg(g,0), *rhs = ir_pair_arg(g,1);
            if (!lhs || lhs->op != IR_LOGICVAR || !rhs) return 0;
            if (rhs->op == IR_ATOM)
                return IR_LIT(rhs).sval && (!strcmp(IR_LIT(rhs).sval, "pi") || !strcmp(IR_LIT(rhs).sval, "e"));
            if (rhs->op != IR_ARITH) return 0;
            const char *rop = IR_LIT(rhs).sval ? IR_LIT(rhs).sval : "+";
            const IR_t *r0 = (rhs->n_operands > 0) ? rhs->operands[0] : NULL;
            const IR_t *r1 = (rhs->n_operands > 1) ? rhs->operands[1] : NULL;
            int floaty = pl_arith_op_floaty(rop)
                      || (r0 && r0->op == IR_LIT_F)
                      || (r1 && r1->op == IR_LIT_F);
            if (floaty) {
                if (r0 && r1) return pl_flat_arith_leaf_float_ok(r0) && pl_flat_arith_leaf_float_ok(r1);
                if (r0 && !r1) return pl_flat_arith_leaf_float_ok(r0);
                return 0;
            }
            if (r0 && r1) return pl_flat_arith_leaf_simple(r0) && pl_flat_arith_leaf_simple(r1);
            if (r0 && !r1) return pl_flat_arith_leaf_simple(r0);
            return 0;
        }
        if (!strcmp(fn, "succ") && IR_LIT(g).ival == 2 && ir_pair_arg(g,0) && ir_pair_arg(g,1)) {
            int a0ok = (ir_pair_arg(g,0)->op == IR_LOGICVAR || ir_pair_arg(g,0)->op == IR_LIT_I);
            int a1ok = (ir_pair_arg(g,1)->op == IR_LOGICVAR || ir_pair_arg(g,1)->op == IR_LIT_I);
            return a0ok && a1ok;
        }
        if (!strcmp(fn, "plus") && IR_LIT(g).ival == 3 && ir_call_arg(g,0) && ir_call_arg(g,1) && ir_call_arg(g,2)) {
            IR_t *a0 = ir_call_arg(g,0), *a1 = ir_call_arg(g,1), *a2 = ir_call_arg(g,2);
            int a0ok = (a0->op == IR_LOGICVAR || a0->op == IR_LIT_I);
            int a1ok = (a1->op == IR_LOGICVAR || a1->op == IR_LIT_I);
            int a2ok = (a2->op == IR_LOGICVAR || a2->op == IR_LIT_I);
            return a0ok && a1ok && a2ok;
        }
        int is_io = (!strcmp(fn, "write") || !strcmp(fn, "writeln") || !strcmp(fn, "print") || !strcmp(fn, "nl") || !strcmp(fn, "halt"));
        if (!is_io) return 0;
        if (IR_LIT(g).ival >= 1) { const IR_t *a = ir_call_arg(g,0); if (!a || (a->op != IR_ATOM && a->op != IR_LIT_I && a->op != IR_LIT_F && a->op != IR_LOGICVAR)) return 0; }
        return 1;
    }
    case IR_UNIFY: {
        const IR_t *l = (g->n_operands > 0) ? g->operands[0] : NULL, *r = (g->n_operands > 1) ? g->operands[1] : NULL;
        if (!l || !r) return 1;
        int l_var = (l->op == IR_LOGICVAR), r_var = (r->op == IR_LOGICVAR);
        int l_con = (l->op == IR_LIT_I || l->op == IR_LIT_F || l->op == IR_ATOM);
        int r_con = (r->op == IR_LIT_I || r->op == IR_LIT_F || r->op == IR_ATOM);
        return (l_var && r_con) || (l_con && r_var) || (l_con && r_con);
    }
    default: return 0;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_graph_t * g_pl_gz_pool = NULL;
static IR_t * pl_gz_det_node(int kind) {
    if (!g_pl_gz_pool) g_pl_gz_pool = IR_alloc(16384, IR_LANG_PL);
    if (!g_pl_gz_pool) return NULL;
    return IR_node_alloc(g_pl_gz_pool, (IR_e)kind);
}
static int pl_gz_fact_clause_units(IR_graph_t *cg, int ar, IR_t ***units_out) {
    if (!cg || !cg->entry || !cg->all) return 0;
    for (int i = 0; i < cg->n; i++) {
        IR_t *nd = cg->all[i];
        if (!nd) continue;
        if (nd->op == IR_CHOICE || nd->op == IR_GOAL || nd->op == IR_BUILTIN || nd->op == IR_CUT ||
            nd->op == IR_DISJ || nd->op == IR_ITE || nd->op == IR_CATCH || nd->op == IR_ARITH ||
            nd->op == IR_STRUCT) return 0;
    }
    if (ar == 0) {
        if (cg->entry->op != IR_SUCCEED) return 0;
        *units_out = NULL; return 1;
    }
    IR_t *gconj = NULL;
    for (int i = 0; i < cg->n; i++) if (cg->all[i] && cg->all[i]->op == IR_GCONJ) { if (gconj) return 0; gconj = cg->all[i]; }
    if (!gconj) return 0;
    bb_conj_state_t *zs = (bb_conj_state_t *)(intptr_t)IR_LIT(gconj).ival;
    if (!zs || !zs->goals || zs->ngoals != ar) return 0;
    for (int i = 0; i < ar; i++) {
        IR_t *u = zs->goals[i];
        if (!u || u->op != IR_UNIFY || u->n_operands < 2 || !u->operands[0] || !u->operands[1]) return 0;
        if (u->operands[0]->op != IR_LOGICVAR || (int)IR_LIT(u->operands[0]).ival != i) return 0;
        if (u->operands[1]->op == IR_ATOM && IR_LIT(u->operands[1]).sval) continue;
        if (u->operands[1]->op == IR_LIT_I) continue;
        return 0;
    }
    *units_out = zs->goals;
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int pl_gz_call_args_ok(bb_goal_state_t *zc, int ar) {
    if (ar > 0 && (!zc->args || zc->nargs < ar)) return 0;
    for (int i = 0; i < ar; i++) {
        IR_t *a = zc->args[i];
        if (!a) return 0;
        if (a->op == IR_LOGICVAR) { if ((int)IR_LIT(a).ival < 0 || (int)IR_LIT(a).ival >= 64) return 0; continue; }
        if (a->op == IR_ATOM && IR_LIT(a).sval) continue;
        if (a->op == IR_LIT_I) continue;
        if (a->op == IR_STRUCT) continue;
    }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_graph_t * pl_gz_goal_callee(IR_t *gg, bb_goal_state_t **zc_out, int *ar_out) {
    bb_goal_state_t *zc = (bb_goal_state_t *)(intptr_t)IR_LIT(gg).ival;
    const char *fn = (zc && zc->callee) ? zc->callee : IR_LIT(gg).sval;
    if (!zc || !fn) return NULL;
    int ar = zc->arity;
    if (ar < 0 || ar > 8) return NULL;
    char key[256]; snprintf(key, sizeof key, "%s/%d", fn, ar);
    Resolve_PredEntry_BB *e = resolve_bb_lookup(key, ar);
    if (!e) return NULL;
    *zc_out = zc; *ar_out = ar;
    return bb_graph_of_pred(e);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int pl_gz_fact_inline(IR_t *gg, IR_t ***units_out, int *arity_out) {
    bb_goal_state_t *zc = NULL; int ar = 0;
    IR_graph_t *cg = pl_gz_goal_callee(gg, &zc, &ar);
    if (!cg) return 0;
    if (!pl_gz_call_args_ok(zc, ar)) return 0;
    if (!pl_gz_fact_clause_units(cg, ar, units_out)) return 0;
    *arity_out = ar;
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static pl_gz_choice_state_t * pl_gz_choice_inline(IR_t *gg) {
    bb_goal_state_t *zc = NULL; int ar = 0;
    IR_graph_t *cg = pl_gz_goal_callee(gg, &zc, &ar);
    if (!cg || !cg->entry || cg->entry->op != IR_CHOICE) return NULL;
    if (ar > 3) return NULL;
    if (!pl_gz_call_args_ok(zc, ar)) return NULL;
    bb_choice_state_t *bc = (bb_choice_state_t *)(intptr_t)IR_LIT(cg->entry).ival;
    if (!bc || !bc->bodies || bc->nbodies < 2 || bc->nbodies > 4) return NULL;
    pl_gz_choice_state_t *st = (pl_gz_choice_state_t *)GC_MALLOC(sizeof *st);
    if (!st) return NULL;
    st->nclauses = bc->nbodies; st->arity = ar; st->mark_slot = 0;
    for (int j = 0; j < ar; j++) st->args[j] = zc->args[j];
    for (int k = 0; k < bc->nbodies; k++) {
        IR_t **units = NULL;
        if (!pl_gz_fact_clause_units(bc->bodies[k], ar, &units)) return NULL;
        for (int j = 0; j < ar; j++) st->consts[k][j] = units ? ((units[j]->n_operands > 1) ? units[j]->operands[1] : NULL) : NULL;
    }
    return st;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_gz_lv(int slot) {
    IR_t *n = pl_gz_det_node(IR_LOGICVAR);
    if (n) IR_LIT(n).ival = slot;
    return n;
}
static int pl_gz_rule_clause(IR_graph_t *cg, int ar, bb_conj_state_t **zs_out);
static int pl_findall_conj_member_admissible(const IR_t *g);
/* PL-GZ-5c: a callee whose graph entry is a CHOICE over 2..4 RULE clauses (facts = empty-body rules).
 * Validation recurses through clause bodies; the visiting list breaks self/mutual recursion cycles
 * (a graph already on the list is being validated up-stack — assume ok here; a real failure
 * surfaces at the outer frame). */
static IR_graph_t *g_gz_visiting[16]; static int g_gz_nvisiting = 0;
static int pl_gz_choice_rule_clauses(IR_graph_t *cg, int ar, bb_choice_state_t **bc_out) {
    if (!cg || !cg->entry || cg->entry->op != IR_CHOICE || ar > 3) return 0;
    bb_choice_state_t *bc = (bb_choice_state_t *)(intptr_t)IR_LIT(cg->entry).ival;
    if (!bc || !bc->bodies || bc->nbodies < 2 || bc->nbodies > 4) return 0;
    for (int v = 0; v < g_gz_nvisiting; v++) if (g_gz_visiting[v] == cg) { if (bc_out) *bc_out = bc; return 1; }
    if (g_gz_nvisiting >= 16) return 0;
    g_gz_visiting[g_gz_nvisiting++] = cg;
    int ok = 1;
    for (int k = 0; k < bc->nbodies && ok; k++) {
        bb_conj_state_t *zsk = NULL;
        if (!pl_gz_rule_clause(bc->bodies[k], ar, &zsk)) ok = 0;
    }
    g_gz_nvisiting--;
    if (ok && bc_out) *bc_out = bc;
    return ok;
}
static int pl_gz_arith_const(const IR_t *nd);
static int pl_gz_rule_body_goal_ok(IR_t *gg) {
    if (!gg) return 0;
    if (gg->op == IR_SUCCEED || gg->op == IR_FAIL || gg->op == IR_CUT) return 1;
    if (gg->op == IR_GOAL) {
        bb_goal_state_t *zc = NULL; int ar2 = 0;
        IR_graph_t *cg2 = pl_gz_goal_callee(gg, &zc, &ar2);
        if (!cg2 || ar2 > 3) return 0;
        if (!pl_gz_call_args_ok(zc, ar2)) return 0;
        if (cg2->entry && cg2->entry->op == IR_CHOICE)
            return pl_gz_choice_rule_clauses(cg2, ar2, NULL);
        return 1;
    }
    if (gg->op == IR_UNIFY) {
        IR_t *l = (gg->n_operands > 0) ? gg->operands[0] : NULL, *r = (gg->n_operands > 1) ? gg->operands[1] : NULL;
        if (!l || !r) return 0;
        int lv = (l->op == IR_LOGICVAR), rv = (r->op == IR_LOGICVAR);
        int lc = (l->op == IR_ATOM || l->op == IR_LIT_I || l->op == IR_LIT_F), rc = (r->op == IR_ATOM || r->op == IR_LIT_I || r->op == IR_LIT_F);
        return (lv && (rv || rc)) || (rv && lc) || (lc && rc);
    }
    if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && !strcmp(IR_LIT(gg).sval, "nl") && IR_LIT(gg).ival == 0) return 1;
    if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && !strcmp(IR_LIT(gg).sval, "write") && IR_LIT(gg).ival == 1 && ir_call_arg(gg,0)) {
        const IR_t *wa0 = ir_call_arg(gg,0);
        return wa0->op == IR_ATOM || wa0->op == IR_LIT_I || wa0->op == IR_LIT_F || wa0->op == IR_LOGICVAR || wa0->op == IR_STRUCT;
    }
    if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && (!strcmp(IR_LIT(gg).sval,"writeq")||!strcmp(IR_LIT(gg).sval,"write_canonical")) && IR_LIT(gg).ival == 1 && ir_call_arg(gg,0)) {
        const IR_t *wa0 = ir_call_arg(gg,0);
        return wa0->op == IR_ATOM || wa0->op == IR_LIT_I || wa0->op == IR_LOGICVAR || wa0->op == IR_STRUCT;
    }
    if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && !strcmp(IR_LIT(gg).sval, "is") && IR_LIT(gg).ival == 2 && ir_pair_arg(gg,0) && ir_pair_arg(gg,1)) {
        if (ir_pair_arg(gg,0)->op != IR_LOGICVAR) return 0;
        IR_t *rhs = ir_pair_arg(gg,1);
        IR_t *q0 = (rhs->n_operands > 0) ? rhs->operands[0] : NULL, *q1 = (rhs->n_operands > 1) ? rhs->operands[1] : NULL;
        int rhs_const = (rhs->op == IR_LIT_I) || (rhs->op == IR_LIT_F) || ((rhs->op == IR_ARITH) && pl_gz_arith_const(rhs));
        int rhs_varop = (rhs->op == IR_LOGICVAR) || (rhs->op == IR_ARITH && IR_LIT(rhs).sval && q0 && q1 && q0->op == IR_LOGICVAR && q1->op == IR_LIT_I);
        int rhs_bivar = (rhs->op == IR_ARITH && IR_LIT(rhs).sval && q0 && q1 && q0->op == IR_LOGICVAR && q1->op == IR_LOGICVAR);
        return rhs_const || rhs_varop || rhs_bivar;
    }
    if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && !strcmp(IR_LIT(gg).sval, "succ") && IR_LIT(gg).ival == 2 && ir_pair_arg(gg,0) && ir_pair_arg(gg,1)) {
        const IR_t *sa0 = ir_pair_arg(gg,0), *sa1 = ir_pair_arg(gg,1);
        return (sa0->op == IR_LOGICVAR || sa0->op == IR_LIT_I) && (sa1->op == IR_LOGICVAR || sa1->op == IR_LIT_I);
    }
    if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && !strcmp(IR_LIT(gg).sval, "plus") && IR_LIT(gg).ival == 3 && ir_call_arg(gg,0) && ir_call_arg(gg,1) && ir_call_arg(gg,2)) {
        const IR_t *pa0 = ir_call_arg(gg,0), *pa1 = ir_call_arg(gg,1), *pa2 = ir_call_arg(gg,2);
        return (pa0->op == IR_LOGICVAR || pa0->op == IR_LIT_I) && (pa1->op == IR_LOGICVAR || pa1->op == IR_LIT_I) && (pa2->op == IR_LOGICVAR || pa2->op == IR_LIT_I);
    }
    if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && IR_LIT(gg).ival == 2 && ir_call_arg(gg,0) && ir_call_arg(gg,1)) {
        const char *fn = IR_LIT(gg).sval;
        int is_atom2 = (!strcmp(fn,"atom_length")||!strcmp(fn,"upcase_atom")||!strcmp(fn,"downcase_atom")||!strcmp(fn,"atom_chars")||!strcmp(fn,"atom_codes")||!strcmp(fn,"string_length")||!strcmp(fn,"string_upper")||!strcmp(fn,"string_lower")||!strcmp(fn,"atom_string")||!strcmp(fn,"string_to_atom")||!strcmp(fn,"number_string")||!strcmp(fn,"atom_number"));
        if (is_atom2) {
            const IR_t *aa0 = ir_call_arg(gg,0), *aa1 = ir_call_arg(gg,1);
            int a0ok = (aa0->op == IR_LOGICVAR || aa0->op == IR_ATOM || aa0->op == IR_LIT_I || aa0->op == IR_LIT_F);
            int a1ok = (aa1->op == IR_LOGICVAR || aa1->op == IR_STRUCT || aa1->op == IR_ATOM);
            return a0ok && a1ok;
        }
    }
    if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && (!strcmp(IR_LIT(gg).sval, "atom_concat")||!strcmp(IR_LIT(gg).sval, "string_concat")) && IR_LIT(gg).ival == 3 && ir_call_arg(gg,0) && ir_call_arg(gg,1) && ir_call_arg(gg,2)) {
        const IR_t *ca0 = ir_call_arg(gg,0), *ca1 = ir_call_arg(gg,1), *ca2 = ir_call_arg(gg,2);
        int a0ok = (ca0->op == IR_LOGICVAR || ca0->op == IR_ATOM); int a1ok = (ca1->op == IR_LOGICVAR || ca1->op == IR_ATOM);
        return a0ok && a1ok && ca2->op == IR_LOGICVAR;
    }
    if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && !strcmp(IR_LIT(gg).sval, "char_type") && IR_LIT(gg).ival == 2 && ir_call_arg(gg,0) && ir_call_arg(gg,1)) {
        const IR_t *ct0 = ir_call_arg(gg,0), *ct1 = ir_call_arg(gg,1);
        int c0ok = (ct0->op == IR_LOGICVAR || ct0->op == IR_ATOM);
        int c1ok = (ct1->op == IR_ATOM || ct1->op == IR_STRUCT);
        return c0ok && c1ok;
    }
    if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && ((!strcmp(IR_LIT(gg).sval,"sort"))||(!strcmp(IR_LIT(gg).sval,"msort"))) && IR_LIT(gg).ival == 2 && ir_call_arg(gg,0) && ir_call_arg(gg,1)) {
        const IR_t *sa0 = ir_call_arg(gg,0), *sa1 = ir_call_arg(gg,1);
        int a0ok = (sa0->op == IR_LOGICVAR || sa0->op == IR_STRUCT || sa0->op == IR_ATOM);
        int a1ok = (sa1->op == IR_LOGICVAR);
        return a0ok && a1ok;
    }
    if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && !strcmp(IR_LIT(gg).sval,"numbervars") && IR_LIT(gg).ival == 3 && ir_call_arg(gg,0) && ir_call_arg(gg,1) && ir_call_arg(gg,2)) {
        const IR_t *nv0 = ir_call_arg(gg,0), *nv1 = ir_call_arg(gg,1), *nv2 = ir_call_arg(gg,2);
        int a0ok = (nv0->op == IR_LOGICVAR || nv0->op == IR_STRUCT || nv0->op == IR_ATOM);
        int a1ok = (nv1->op == IR_LOGICVAR || nv1->op == IR_LIT_I);
        int a2ok = (nv2->op == IR_LOGICVAR);
        return a0ok && a1ok && a2ok;
    }
    if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && (!strcmp(IR_LIT(gg).sval,"term_string")||!strcmp(IR_LIT(gg).sval,"term_to_atom")) && IR_LIT(gg).ival == 2 && ir_call_arg(gg,0) && ir_call_arg(gg,1)) {
        const IR_t *ts0 = ir_call_arg(gg,0), *ts1 = ir_call_arg(gg,1);
        int a0ok = (ts0->op == IR_LOGICVAR || ts0->op == IR_STRUCT || ts0->op == IR_ATOM || ts0->op == IR_LIT_I || ts0->op == IR_ARITH);
        int a1ok = (ts1->op == IR_LOGICVAR);
        return a0ok && a1ok;
    }
    if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && (!strcmp(IR_LIT(gg).sval,"atomic_list_concat")||!strcmp(IR_LIT(gg).sval,"concat_atom")) && IR_LIT(gg).ival == 2 && ir_call_arg(gg,0) && ir_call_arg(gg,1)) {
        const IR_t *al0 = ir_call_arg(gg,0), *al1 = ir_call_arg(gg,1);
        return (al0->op == IR_LOGICVAR || al0->op == IR_STRUCT || al0->op == IR_ATOM) && al1->op == IR_LOGICVAR;
    }
    if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && !strcmp(IR_LIT(gg).sval,"atomic_list_concat") && IR_LIT(gg).ival == 3 && ir_call_arg(gg,0) && ir_call_arg(gg,1) && ir_call_arg(gg,2)) {
        const IR_t *al0 = ir_call_arg(gg,0), *al1 = ir_call_arg(gg,1), *al2 = ir_call_arg(gg,2);
        return (al0->op == IR_LOGICVAR || al0->op == IR_STRUCT || al0->op == IR_ATOM) && (al1->op == IR_LOGICVAR || al1->op == IR_ATOM) && al2->op == IR_LOGICVAR;
    }
    if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && !strcmp(IR_LIT(gg).sval,"copy_term") && IR_LIT(gg).ival == 2 && ir_call_arg(gg,0) && ir_call_arg(gg,1)) {
        const IR_t *ct0 = ir_call_arg(gg,0), *ct1 = ir_call_arg(gg,1);
        int a0ok = (ct0->op == IR_LOGICVAR || ct0->op == IR_STRUCT || ct0->op == IR_ATOM || ct0->op == IR_LIT_I);
        int a1ok = (ct1->op == IR_LOGICVAR || ct1->op == IR_STRUCT || ct1->op == IR_ATOM || ct1->op == IR_LIT_I);
        return a0ok && a1ok;
    }
    if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && (!strcmp(IR_LIT(gg).sval,"nb_setval")||!strcmp(IR_LIT(gg).sval,"nb_getval")) && IR_LIT(gg).ival == 2 && ir_call_arg(gg,0) && ir_call_arg(gg,1)) {
        const IR_t *nk0 = ir_call_arg(gg,0), *nv1 = ir_call_arg(gg,1);
        int a0ok = (nk0->op == IR_ATOM || nk0->op == IR_LOGICVAR);
        int a1ok = (nv1->op == IR_LOGICVAR || nv1->op == IR_ATOM || nv1->op == IR_LIT_I || nv1->op == IR_LIT_F || nv1->op == IR_STRUCT);
        return a0ok && a1ok;
    }
    if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && !strcmp(IR_LIT(gg).sval,"findall")) {
        return pl_findall_conj_member_admissible(gg);
    }
    if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && IR_LIT(gg).ival == 2 && ir_pair_arg(gg,0) && ir_pair_arg(gg,1)) {
        const char *fn = IR_LIT(gg).sval;
        int is_cmp = (strcmp(fn,"<")==0||strcmp(fn,">")==0||strcmp(fn,">=")==0||strcmp(fn,"=<")==0||strcmp(fn,"=:=")==0||strcmp(fn,"=\\=")==0);
        if (is_cmp) return (ir_pair_arg(gg,0)->op == IR_LIT_I || ir_pair_arg(gg,0)->op == IR_LOGICVAR) && (ir_pair_arg(gg,1)->op == IR_LIT_I || ir_pair_arg(gg,1)->op == IR_LOGICVAR);
        int is_tcmp = (strcmp(fn,"@<")==0||strcmp(fn,"@>")==0||strcmp(fn,"@=<")==0||strcmp(fn,"@>=")==0||strcmp(fn,"==")==0||strcmp(fn,"\\==")==0);
        if (!is_tcmp) return 0;
        const IR_t *q0 = ir_pair_arg(gg,0), *q1 = ir_pair_arg(gg,1);
        return (q0->op==IR_LOGICVAR||q0->op==IR_ATOM||q0->op==IR_LIT_I||q0->op==IR_LIT_F) && (q1->op==IR_LOGICVAR||q1->op==IR_ATOM||q1->op==IR_LIT_I||q1->op==IR_LIT_F);
    }
    if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && IR_LIT(gg).ival == 1 && ir_call_arg(gg,0)) {
        const char *fn = IR_LIT(gg).sval;
        int is_ttest = (strcmp(fn,"var")==0||strcmp(fn,"nonvar")==0||strcmp(fn,"atom")==0||strcmp(fn,"atomic")==0||
                        strcmp(fn,"number")==0||strcmp(fn,"integer")==0||strcmp(fn,"float")==0||strcmp(fn,"compound")==0||
                        strcmp(fn,"callable")==0||strcmp(fn,"is_list")==0||strcmp(fn,"ground")==0);
        if (!is_ttest) return 0;
        const IR_t *a0 = ir_call_arg(gg,0);
        return a0->op == IR_LOGICVAR || a0->op == IR_ATOM || a0->op == IR_LIT_I || a0->op == IR_STRUCT;
    }
    if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && !strcmp(IR_LIT(gg).sval, "functor") && IR_LIT(gg).ival == 3 && ir_call_arg(gg,0) && ir_call_arg(gg,1) && ir_call_arg(gg,2)) {
        const IR_t *a0 = ir_call_arg(gg,0), *a1 = ir_call_arg(gg,1), *a2 = ir_call_arg(gg,2);
        int a0ok = (a0->op == IR_LOGICVAR || a0->op == IR_STRUCT || a0->op == IR_ATOM || a0->op == IR_LIT_I);
        int a1ok = (a1->op == IR_LOGICVAR); int a2ok = (a2->op == IR_LOGICVAR);
        return a0ok && a1ok && a2ok;
    }
    if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && !strcmp(IR_LIT(gg).sval, "arg") && IR_LIT(gg).ival == 3 && ir_call_arg(gg,0) && ir_call_arg(gg,1) && ir_call_arg(gg,2)) {
        const IR_t *a0 = ir_call_arg(gg,0), *a1 = ir_call_arg(gg,1), *a2 = ir_call_arg(gg,2);
        int a0ok = (a0->op == IR_LOGICVAR || a0->op == IR_LIT_I);
        int a1ok = (a1->op == IR_LOGICVAR || a1->op == IR_STRUCT); int a2ok = (a2->op == IR_LOGICVAR);
        return a0ok && a1ok && a2ok;
    }
    if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && !strcmp(IR_LIT(gg).sval, "=..") && IR_LIT(gg).ival == 2 && ir_call_arg(gg,0) && ir_call_arg(gg,1)) {
        const IR_t *a0 = ir_call_arg(gg,0), *a1 = ir_call_arg(gg,1);
        int a0ok = (a0->op == IR_LOGICVAR || a0->op == IR_STRUCT); int a1ok = (a1->op == IR_LOGICVAR || a1->op == IR_STRUCT);
        return a0ok && a1ok;
    }
    if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && !strcmp(IR_LIT(gg).sval, "format")) {
        if (IR_LIT(gg).ival == 1) { const IR_t *a0 = ir_call_arg(gg,0); return a0 && a0->op == IR_ATOM && IR_LIT(a0).sval; }
        if (IR_LIT(gg).ival == 2) { const IR_t *a0 = ir_call_arg(gg,0), *a1 = ir_call_arg(gg,1); return a0 && a0->op == IR_ATOM && IR_LIT(a0).sval && a1 && (a1->op == IR_STRUCT || a1->op == IR_ATOM); }
        return 0;
    }
    return 0;
}
static int pl_gz_rule_clause(IR_graph_t *cg, int ar, bb_conj_state_t **zs_out) {
    if (!cg || !cg->entry || !cg->all) return 0;
    if (cg->nslots < ar || cg->nslots > 16) return 0;
    for (int i = 0; i < cg->n; i++) {
        IR_t *nd = cg->all[i];
        if (!nd) continue;
        if (nd->op == IR_CHOICE || nd->op == IR_DISJ ||
            nd->op == IR_ITE || nd->op == IR_CATCH) return 0;
        if (nd->op == IR_BUILTIN) {
            if (IR_LIT(nd).sval && !strcmp(IR_LIT(nd).sval, "nl") && IR_LIT(nd).ival == 0) continue;
            if (IR_LIT(nd).sval && !strcmp(IR_LIT(nd).sval, "write") && IR_LIT(nd).ival == 1) continue;
            if (IR_LIT(nd).sval && (!strcmp(IR_LIT(nd).sval,"writeq")||!strcmp(IR_LIT(nd).sval,"write_canonical")) && IR_LIT(nd).ival == 1) continue;
            if (IR_LIT(nd).sval && !strcmp(IR_LIT(nd).sval, "is") && IR_LIT(nd).ival == 2) continue;
            if (IR_LIT(nd).sval && !strcmp(IR_LIT(nd).sval, "functor") && IR_LIT(nd).ival == 3) continue;
            if (IR_LIT(nd).sval && !strcmp(IR_LIT(nd).sval, "arg") && IR_LIT(nd).ival == 3) continue;
            if (IR_LIT(nd).sval && !strcmp(IR_LIT(nd).sval, "=..") && IR_LIT(nd).ival == 2) continue;
            if (IR_LIT(nd).sval && !strcmp(IR_LIT(nd).sval, "format") && (IR_LIT(nd).ival == 1 || IR_LIT(nd).ival == 2)) continue;
            if (IR_LIT(nd).sval && !strcmp(IR_LIT(nd).sval, "succ") && IR_LIT(nd).ival == 2) continue;
            if (IR_LIT(nd).sval && !strcmp(IR_LIT(nd).sval, "plus") && IR_LIT(nd).ival == 3) continue;
            if (IR_LIT(nd).sval && (!strcmp(IR_LIT(nd).sval,"atom_length")||!strcmp(IR_LIT(nd).sval,"upcase_atom")||!strcmp(IR_LIT(nd).sval,"downcase_atom")||!strcmp(IR_LIT(nd).sval,"atom_chars")||!strcmp(IR_LIT(nd).sval,"atom_codes")||!strcmp(IR_LIT(nd).sval,"string_length")||!strcmp(IR_LIT(nd).sval,"string_upper")||!strcmp(IR_LIT(nd).sval,"string_lower")||!strcmp(IR_LIT(nd).sval,"atom_string")||!strcmp(IR_LIT(nd).sval,"string_to_atom")||!strcmp(IR_LIT(nd).sval,"number_string")||!strcmp(IR_LIT(nd).sval,"atom_number")) && IR_LIT(nd).ival == 2) continue;
            if (IR_LIT(nd).sval && (!strcmp(IR_LIT(nd).sval, "atom_concat")||!strcmp(IR_LIT(nd).sval, "string_concat")) && IR_LIT(nd).ival == 3) continue;
            if (IR_LIT(nd).sval && !strcmp(IR_LIT(nd).sval, "char_type") && IR_LIT(nd).ival == 2) continue;
            if (IR_LIT(nd).sval && (!strcmp(IR_LIT(nd).sval,"sort")||!strcmp(IR_LIT(nd).sval,"msort")) && IR_LIT(nd).ival == 2) continue;
            if (IR_LIT(nd).sval && !strcmp(IR_LIT(nd).sval,"numbervars") && IR_LIT(nd).ival == 3) continue;
            if (IR_LIT(nd).sval && (!strcmp(IR_LIT(nd).sval,"term_string")||!strcmp(IR_LIT(nd).sval,"term_to_atom")) && IR_LIT(nd).ival == 2) continue;
            if (IR_LIT(nd).sval && (!strcmp(IR_LIT(nd).sval,"atomic_list_concat")||!strcmp(IR_LIT(nd).sval,"concat_atom")) && (IR_LIT(nd).ival == 2 || IR_LIT(nd).ival == 3)) continue;
            if (IR_LIT(nd).sval && !strcmp(IR_LIT(nd).sval,"copy_term") && IR_LIT(nd).ival == 2) continue;
            if (IR_LIT(nd).sval && (!strcmp(IR_LIT(nd).sval,"nb_setval")||!strcmp(IR_LIT(nd).sval,"nb_getval")) && IR_LIT(nd).ival == 2) continue;
            if (IR_LIT(nd).sval && !strcmp(IR_LIT(nd).sval,"findall")) continue;
            const char *fn = IR_LIT(nd).sval ? IR_LIT(nd).sval : "";
            int is_ttest = (strcmp(fn,"var")==0||strcmp(fn,"nonvar")==0||strcmp(fn,"atom")==0||strcmp(fn,"atomic")==0||
                            strcmp(fn,"number")==0||strcmp(fn,"integer")==0||strcmp(fn,"float")==0||strcmp(fn,"compound")==0||
                            strcmp(fn,"callable")==0||strcmp(fn,"is_list")==0||strcmp(fn,"ground")==0);
            if (is_ttest && IR_LIT(nd).ival == 1) continue;
            int is_cmp = (strcmp(fn,"<")==0||strcmp(fn,">")==0||strcmp(fn,">=")==0||strcmp(fn,"=<")==0||strcmp(fn,"=:=")==0||strcmp(fn,"=\\=")==0);
            int is_tcmp = (strcmp(fn,"@<")==0||strcmp(fn,"@>")==0||strcmp(fn,"@=<")==0||strcmp(fn,"@>=")==0||strcmp(fn,"==")==0||strcmp(fn,"\\==")==0);
            if (!is_cmp && !is_tcmp) return 0;
        }
        if (nd->op == IR_LOGICVAR && ((int)IR_LIT(nd).ival < 0 || (int)IR_LIT(nd).ival >= cg->nslots)) return 0;
    }
    IR_t *gconj = NULL;
    for (int i = 0; i < cg->n; i++) if (cg->all[i] && cg->all[i]->op == IR_GCONJ) { if (gconj) return 0; gconj = cg->all[i]; }
    if (!gconj) return 0;
    bb_conj_state_t *zs = (bb_conj_state_t *)(intptr_t)IR_LIT(gconj).ival;
    if (!zs || !zs->goals || zs->ngoals < ar || zs->ngoals > 32) return 0;
    for (int i = 0; i < ar; i++) {
        IR_t *u = zs->goals[i];
        if (!u || u->op != IR_UNIFY || u->n_operands < 2 || !u->operands[0] || !u->operands[1]) return 0;
        IR_t *u0 = u->operands[0], *u1 = u->operands[1];
        if (u0->op != IR_LOGICVAR || (int)IR_LIT(u0).ival != i) return 0;
        if (u1->op == IR_LOGICVAR) { if ((int)IR_LIT(u1).ival < 0 || (int)IR_LIT(u1).ival >= cg->nslots) return 0; continue; }
        if (u1->op == IR_ATOM && IR_LIT(u1).sval) continue;
        if (u1->op == IR_LIT_I) continue;
        if (u1->op == IR_STRUCT) continue;
    }
    for (int i = ar; i < zs->ngoals; i++) if (!pl_gz_rule_body_goal_ok(zs->goals[i])) return 0;
    *zs_out = zs;
    return 1;
}
static int pl_gz_rule_inline_check(IR_t *gg) {
    bb_goal_state_t *zc = NULL; int ar = 0;
    IR_graph_t *cg = pl_gz_goal_callee(gg, &zc, &ar);
    if (!cg) return 0;
    if (ar > 3) return 0;
    if (!pl_gz_call_args_ok(zc, ar)) return 0;
    if (cg->entry && cg->entry->op == IR_CHOICE)
        return pl_gz_choice_rule_clauses(cg, ar, NULL);
    bb_conj_state_t *zs = NULL;
    return pl_gz_rule_clause(cg, ar, &zs);
}
static pl_gz_callee_t * pl_gz_callee_get(IR_graph_t *cg, int ar, bb_conj_state_t *zs, pl_gz_callee_t **callees, int *ncallees);
static pl_gz_callee_t * pl_gz_callee_get_any(IR_t *gg, IR_graph_t *cg, int ar, pl_gz_callee_t **callees, int *ncallees);
/* PL-GZ-5c: graph slot s of one clause → frame slot.  Head slots 0..ar-1 are shared (the args);
 * each clause's locals live in its own frame range starting at lbase (single-clause lbase==ar ⇒ identity). */
static int pl_gz_slot_map(int s, int ar, int lbase) { return s < ar ? s : lbase + (s - ar); }
static int pl_gz_clause_nsynth(bb_conj_state_t *zs, int ar) {
    int nsynth = 0;
    for (int i = ar; i < zs->ngoals; i++) {
        IR_t *gg = zs->goals[i];
        if (!gg || gg->op != IR_GOAL) continue;
        bb_goal_state_t *zc2 = NULL; int ar2 = 0;
        if (!pl_gz_goal_callee(gg, &zc2, &ar2)) return -1;
        for (int ai = 0; ai < ar2 && ai < 3; ai++)
            if (zc2->args[ai] && zc2->args[ai]->op != IR_LOGICVAR) nsynth++;
    }
    return nsynth;
}
static IR_t * pl_gz_arith_slot_map(const IR_t *nd, int ar, int lbase) {
    if (!nd) return NULL;
    if (nd->op == IR_LIT_I) { IR_t *c = pl_gz_det_node(IR_LIT_I); if (c) IR_LIT(c).ival = IR_LIT(nd).ival; return c; }
    if (nd->op == IR_LOGICVAR) { return pl_gz_lv(pl_gz_slot_map((int)IR_LIT(nd).ival, ar, lbase)); }
    if (nd->op == IR_ARITH) {
        IR_t *c = pl_gz_det_node(IR_ARITH); if (!c) return NULL;
        IR_LIT(c).sval = IR_LIT(nd).sval; IR_LIT(c).ival = IR_LIT(nd).ival;
        IR_t *m0 = (nd->n_operands > 0) ? nd->operands[0] : NULL, *m1 = (nd->n_operands > 1) ? nd->operands[1] : NULL;
        if (m0) { IR_t *cm0 = pl_gz_arith_slot_map(m0, ar, lbase); if (!cm0) return NULL; ir_operand_push(c, cm0); }
        if (m1) { IR_t *cm1 = pl_gz_arith_slot_map(m1, ar, lbase); if (!cm1) return NULL; ir_operand_push(c, cm1); }
        return c;
    }
    return NULL;
}
static IR_t * pl_gz_struct_slot_map(const IR_t *nd, int ar, int lbase) {
    if (!nd) return NULL;
    if (nd->op == IR_LOGICVAR) { return pl_gz_lv(pl_gz_slot_map((int)IR_LIT(nd).ival, ar, lbase)); }
    if (nd->op == IR_ATOM || nd->op == IR_LIT_I || nd->op == IR_LIT_F) { return (IR_t *)nd; }
    if (nd->op == IR_STRUCT) {
        IR_t *c = pl_gz_det_node(IR_STRUCT); if (!c) return NULL;
        IR_LIT(c).sval = IR_LIT(nd).sval; IR_LIT(c).ival = IR_LIT(nd).ival;
        for (int i = 0; i < nd->n_operands; i++) {
            IR_t *ch = (nd->n_operands > i) ? nd->operands[i] : NULL;
            IR_t *cm = pl_gz_struct_slot_map(ch, ar, lbase); if (!cm) return NULL;
            ir_operand_push(c, cm);
        }
        return c;
    }
    return NULL;
}
static int pl_gz_rule_callee_body(bb_conj_state_t *zs, IR_graph_t *cg, pl_gz_callee_t *ce, int clause_idx, int lbase, pl_gz_callee_t **callees, int *ncallees) {
    IR_t *head = NULL, *tail = NULL;
    int ar = ce->arity;
    int nlocals_real = cg->nslots - ar;
    int synth_next = lbase + nlocals_real;
    for (int i = 0; i < ar; i++) {
        IR_t *u = zs->goals[i];
        IR_t *u1 = (u->n_operands > 1) ? u->operands[1] : NULL;
        IR_t *cu = NULL;
        if (u1 && u1->op == IR_LOGICVAR) {
            if ((int)IR_LIT(u1).ival == i) continue;
            cu = pl_gz_det_node(IR_CELL_UNIFY);
            if (!cu) return 0;
            IR_t *ca = pl_gz_lv(i), *cb = pl_gz_lv(pl_gz_slot_map((int)IR_LIT(u1).ival, ar, lbase));
            if (!ca || !cb) return 0;
            ir_operand_push(cu, ca); ir_operand_push(cu, cb);
        } else {
            cu = pl_gz_det_node(IR_CELL_UNIFY);
            if (!cu) return 0;
            IR_t *ca = pl_gz_lv(i);
            if (!ca) return 0;
            ir_operand_push(cu, ca);
            if (u1) {
                IR_t *u1m = (u1->op == IR_STRUCT) ? pl_gz_struct_slot_map(u1, ar, lbase) : u1;
                if (!u1m) return 0;
                ir_operand_push(cu, u1m);
            }
        }
        if (!head) head = cu; else { tail->γ.node = cu; memcpy(tail->γ.sz, "α", 3); }
        tail = cu;
    }
    for (int i = ar; i < zs->ngoals; i++) {
        IR_t *gg = zs->goals[i];
        IR_t *nn = NULL;
        if (gg->op == IR_SUCCEED) continue;
        if (gg->op == IR_GOAL) {
            bb_goal_state_t *zc2 = NULL; int ar2 = 0;
            IR_graph_t *cg2 = pl_gz_goal_callee(gg, &zc2, &ar2);
            if (!cg2 || ar2 > 3 || !pl_gz_call_args_ok(zc2, ar2)) return 0;
            pl_gz_callee_t *ce2 = pl_gz_callee_get_any(gg, cg2, ar2, callees, ncallees);
            if (!ce2) return 0;
            pl_gz_call_state_t *cs2 = (pl_gz_call_state_t *)GC_MALLOC(sizeof *cs2);
            if (!cs2) return 0;
            memset(cs2, 0, sizeof *cs2);
            cs2->callee = ce2; cs2->nargs = ar2;
            for (int ai = 0; ai < ar2; ai++) {
                IR_t *a = zc2->args[ai];
                if (a->op == IR_LOGICVAR) {
                    cs2->args[ai] = pl_gz_lv(pl_gz_slot_map((int)IR_LIT(a).ival, ar, lbase));
                    if (!cs2->args[ai]) return 0;
                    continue;
                }
                int kk = synth_next++;
                IR_t *cu = pl_gz_det_node(IR_CELL_UNIFY);
                if (!cu) return 0;
                IR_t *ca = pl_gz_lv(kk);
                if (!ca) return 0;
                ir_operand_push(cu, ca); ir_operand_push(cu, a);
                if (!head) head = cu; else { tail->γ.node = cu; memcpy(tail->γ.sz, "α", 3); }
                tail = cu;
                cs2->args[ai] = pl_gz_lv(kk);
                if (!cs2->args[ai]) return 0;
            }
            cs2->child_slot = ce->arity + ce->nlocals + ce->nchild;
            ce->nchild++;
            nn = pl_gz_det_node(IR_CELL_CALL);
            if (!nn) return 0;
            IR_LIT(nn).ival = (int64_t)(intptr_t)cs2;
        } else if (gg->op == IR_FAIL) {
            nn = pl_gz_det_node(IR_FAIL);
        } else if (gg->op == IR_CUT) {
            nn = pl_gz_det_node(IR_CELL_CUT);
        } else if (gg->op == IR_UNIFY) {
            nn = pl_gz_det_node(IR_CELL_UNIFY);
            if (!nn) return 0;
            IR_t *g0 = (gg->n_operands > 0) ? gg->operands[0] : NULL, *g1 = (gg->n_operands > 1) ? gg->operands[1] : NULL;
            if (!g0 || !g1) return 0;
            IR_t *na = (g0->op == IR_LOGICVAR) ? pl_gz_lv(pl_gz_slot_map((int)IR_LIT(g0).ival, ar, lbase)) : g0;
            IR_t *nb = (g1->op == IR_LOGICVAR) ? pl_gz_lv(pl_gz_slot_map((int)IR_LIT(g1).ival, ar, lbase)) : g1;
            if (!na || !nb) return 0;
            ir_operand_push(nn, na); ir_operand_push(nn, nb);
        } else if (IR_LIT(gg).sval && !strcmp(IR_LIT(gg).sval, "nl")) {
            nn = pl_gz_det_node(IR_DET_NL);
        } else if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && !strcmp(IR_LIT(gg).sval, "is") && IR_LIT(gg).ival == 2 && ir_pair_arg(gg,0) && ir_pair_arg(gg,1)) {
            nn = pl_gz_det_node(IR_DET_IS);
            if (!nn) return 0;
            IR_t *na = pl_gz_lv(pl_gz_slot_map((int)IR_LIT(ir_pair_arg(gg,0)).ival, ar, lbase));
            IR_t *nb = pl_gz_arith_slot_map(ir_pair_arg(gg,1), ar, lbase);
            if (!na || !nb) return 0;
            ir_operand_push(nn, na); ir_operand_push(nn, nb);
        } else if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && IR_LIT(gg).ival == 2 && ir_pair_arg(gg,0) && ir_pair_arg(gg,1)) {
            const char *fn = IR_LIT(gg).sval;
            int is_cmp = (strcmp(fn,"<")==0||strcmp(fn,">")==0||strcmp(fn,">=")==0||strcmp(fn,"=<")==0||strcmp(fn,"=:=")==0||strcmp(fn,"=\\=")==0);
            if (!is_cmp) return 0;
            nn = pl_gz_det_node(IR_DET_CMP);
            if (!nn) return 0;
            IR_LIT(nn).sval = fn;
            IR_t *pc0 = ir_pair_arg(gg,0), *pc1 = ir_pair_arg(gg,1);
            IR_t *na = (pc0->op == IR_LOGICVAR) ? pl_gz_lv(pl_gz_slot_map((int)IR_LIT(pc0).ival, ar, lbase)) : pc0;
            IR_t *nb = (pc1->op == IR_LOGICVAR) ? pl_gz_lv(pl_gz_slot_map((int)IR_LIT(pc1).ival, ar, lbase)) : pc1;
            if (!na || !nb) return 0;
            ir_operand_push(nn, na); ir_operand_push(nn, nb);
        } else {
            nn = pl_gz_det_node(IR_DET_WRITE);
            if (!nn) return 0;
            IR_t *w0 = ir_call_arg(gg, 0);
            if      (w0->op == IR_ATOM)  IR_LIT(nn).sval = IR_LIT(w0).sval;
            else if (w0->op == IR_LIT_I) { IR_LIT(nn).sval = NULL; IR_LIT(nn).ival = IR_LIT(w0).ival; }
            else { IR_LIT(nn).sval = NULL; IR_LIT(nn).ival = 0; IR_t *na = pl_gz_lv(pl_gz_slot_map((int)IR_LIT(w0).ival, ar, lbase)); if (!na) return 0; ir_operand_push(nn, na); }
        }
        if (!nn) return 0;
        if (!head) head = nn; else { tail->γ.node = nn; memcpy(tail->γ.sz, "α", 3); }
        tail = nn;
    }
    if (!head && ce->nclauses > 1) return 0;
    ce->clause_head[clause_idx] = head;
    if (clause_idx == 0) ce->body_head = head;
    return 1;
}
static pl_gz_callee_t * pl_gz_callee_get(IR_graph_t *cg, int ar, bb_conj_state_t *zs, pl_gz_callee_t **callees, int *ncallees) {
    for (int k = 0; k < *ncallees; k++) if (callees[k]->graph_key == (void *)cg) return callees[k];
    if (*ncallees >= 8) return NULL;
    pl_gz_callee_t *ce = (pl_gz_callee_t *)GC_MALLOC(sizeof *ce);
    if (!ce) return NULL;
    memset(ce, 0, sizeof *ce);
    ce->graph_key = (void *)cg; ce->arity = ar; ce->base = 0; ce->mark_slot = 0;
    ce->nclauses = 1;
    ce->frame_node = pl_gz_det_node(IR_CALLEE_FRAME);
    if (!ce->frame_node) return NULL;
    IR_LIT(ce->frame_node).ival = (int64_t)(intptr_t)ce;
    callees[(*ncallees)++] = ce;
    int nsynth = pl_gz_clause_nsynth(zs, ar);
    if (nsynth < 0) return NULL;
    ce->nlocals = (cg->nslots - ar) + nsynth;
    if (!pl_gz_rule_callee_body(zs, cg, ce, 0, ar, callees, ncallees)) return NULL;
    return ce;
}
/* PL-GZ-5c: multi-clause RULE callee — the seed's path/2.  ONE frame: [args | clause-0 locals+synth |
 * clause-1 locals+synth | … | child slots].  mark at [ζ+0], cursor at [ζ+4]; per-clause body chains;
 * the SHELL-FIRST memo makes self/mutual recursion terminate at admit time exactly as in 5b. */
static pl_gz_callee_t * pl_gz_callee_get_choice(IR_graph_t *cg, int ar, bb_choice_state_t *bc, pl_gz_callee_t **callees, int *ncallees) {
    for (int k = 0; k < *ncallees; k++) if (callees[k]->graph_key == (void *)cg) return callees[k];
    if (*ncallees >= 8) return NULL;
    pl_gz_callee_t *ce = (pl_gz_callee_t *)GC_MALLOC(sizeof *ce);
    if (!ce) return NULL;
    memset(ce, 0, sizeof *ce);
    ce->graph_key = (void *)cg; ce->arity = ar; ce->base = 0; ce->mark_slot = 0;
    ce->nclauses = bc->nbodies;
    ce->frame_node = pl_gz_det_node(IR_CALLEE_FRAME);
    if (!ce->frame_node) return NULL;
    IR_LIT(ce->frame_node).ival = (int64_t)(intptr_t)ce;
    callees[(*ncallees)++] = ce;
    bb_conj_state_t *zsk[4]; int lbase[4]; int total = 0;
    for (int k = 0; k < bc->nbodies; k++) {
        zsk[k] = NULL;
        if (!pl_gz_rule_clause(bc->bodies[k], ar, &zsk[k])) return NULL;
        int ns = pl_gz_clause_nsynth(zsk[k], ar);
        if (ns < 0) return NULL;
        lbase[k] = ar + total;
        total += (bc->bodies[k]->nslots - ar) + ns;
    }
    ce->nlocals = total;
    for (int k = 0; k < bc->nbodies; k++)
        if (!pl_gz_rule_callee_body(zsk[k], bc->bodies[k], ce, k, lbase[k], callees, ncallees)) return NULL;
    return ce;
}
static pl_gz_callee_t * pl_gz_callee_get_any(IR_t *gg, IR_graph_t *cg, int ar, pl_gz_callee_t **callees, int *ncallees) {
    (void)gg;
    if (cg->entry && cg->entry->op == IR_CHOICE) {
        bb_choice_state_t *bc = NULL;
        if (!pl_gz_choice_rule_clauses(cg, ar, &bc)) return NULL;
        return pl_gz_callee_get_choice(cg, ar, bc, callees, ncallees);
    }
    bb_conj_state_t *zs = NULL;
    if (!pl_gz_rule_clause(cg, ar, &zs)) return NULL;
    return pl_gz_callee_get(cg, ar, zs, callees, ncallees);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int pl_gz_count_synth_goal(IR_t *gg, int *nsynth);
static int pl_gz_count_synth_root(IR_t *root, int *nsynth) {
    if (!root || root->op == IR_SUCCEED) return 1;
    if (root->op == IR_GCONJ) {
        bb_conj_state_t *zs = (bb_conj_state_t *)(intptr_t)IR_LIT(root).ival;
        if (!zs || !zs->goals || zs->ngoals <= 0 || zs->ngoals > 64) return 0;
        for (int i = 0; i < zs->ngoals; i++)
            if (!pl_gz_count_synth_goal(zs->goals[i], nsynth)) return 0;
        return 1;
    }
    return pl_gz_count_synth_goal(root, nsynth);
}
static int pl_gz_count_synth_goal(IR_t *gg, int *nsynth) {
    if (!gg) return 1;
    if (gg->op == IR_ITE) {
        bb_ite_state_t *zi = (bb_ite_state_t *)(intptr_t)IR_LIT(gg).ival;
        if (!zi) return 0;
        if (!pl_gz_count_synth_root(zi->cond_root, nsynth)) return 0;
        if (!pl_gz_count_synth_root(zi->then_root, nsynth)) return 0;
        if (!pl_gz_count_synth_root(zi->else_root, nsynth)) return 0;
        return 1;
    }
    if (gg->op != IR_GOAL) {
        if (gg->op == IR_BUILTIN && IR_LIT(gg).sval) {
            const char *fn = IR_LIT(gg).sval;
            int is_ttest = (strcmp(fn,"var")==0||strcmp(fn,"nonvar")==0||strcmp(fn,"atom")==0||strcmp(fn,"atomic")==0||
                            strcmp(fn,"number")==0||strcmp(fn,"integer")==0||strcmp(fn,"float")==0||strcmp(fn,"compound")==0||
                            strcmp(fn,"callable")==0||strcmp(fn,"is_list")==0||strcmp(fn,"ground")==0);
            if (is_ttest && IR_LIT(gg).ival == 1) {
                IR_t *a0 = ir_call_arg(gg,0);
                if (a0 && a0->op != IR_LOGICVAR) (*nsynth)++;
                return 1;
            }
            if ((!strcmp(fn,"functor") && IR_LIT(gg).ival == 3) ||
                (!strcmp(fn,"arg")     && IR_LIT(gg).ival == 3) ||
                (!strcmp(fn,"=..")     && IR_LIT(gg).ival == 2)) {
                for (int ai = 0; ai < (int)IR_LIT(gg).ival; ai++) {
                    IR_t *ax = ir_call_arg(gg, ai);
                    if (ax && ax->op != IR_LOGICVAR && (ax->op == IR_STRUCT || ax->op == IR_ATOM || ax->op == IR_LIT_I)) (*nsynth)++;
                }
                return 1;
            }
            int is_tcmp = (strcmp(fn,"@<")==0||strcmp(fn,"@>")==0||strcmp(fn,"@=<")==0||strcmp(fn,"@>=")==0||strcmp(fn,"==")==0||strcmp(fn,"\\==")==0);
            if (is_tcmp && IR_LIT(gg).ival == 2) {
                IR_t *a0 = ir_pair_arg(gg,0), *a1 = ir_pair_arg(gg,1);
                if (a0 && a0->op != IR_LOGICVAR) (*nsynth)++;
                if (a1 && a1->op != IR_LOGICVAR) (*nsynth)++;
            }
            if (!strcmp(fn,"format") && IR_LIT(gg).ival == 2) {
                IR_t *a1 = ir_call_arg(gg,1);
                if (a1 && a1->op != IR_LOGICVAR) (*nsynth)++;
            }
            if (!strcmp(fn,"succ") && IR_LIT(gg).ival == 2) {
                IR_t *sa0 = ir_pair_arg(gg,0), *sa1 = ir_pair_arg(gg,1);
                if (sa0 && sa0->op != IR_LOGICVAR) (*nsynth)++;
                if (sa1 && sa1->op != IR_LOGICVAR) (*nsynth)++;
            }
            if (!strcmp(fn,"plus") && IR_LIT(gg).ival == 3) {
                for (int ai = 0; ai < 3; ai++) { IR_t *px = ir_call_arg(gg, ai); if (px && px->op != IR_LOGICVAR) (*nsynth)++; }
            }
            if (!strcmp(fn,"write") && IR_LIT(gg).ival == 1) {
                IR_t *wa0 = ir_call_arg(gg,0);
                if (wa0 && wa0->op == IR_STRUCT) (*nsynth)++;
            }
            if ((!strcmp(fn,"writeq")||!strcmp(fn,"write_canonical")) && IR_LIT(gg).ival == 1) {
                IR_t *wa0 = ir_call_arg(gg,0);
                if (wa0 && (wa0->op == IR_STRUCT || wa0->op == IR_ATOM)) (*nsynth)++;
            }            if ((!strcmp(fn,"atom_length")||!strcmp(fn,"upcase_atom")||!strcmp(fn,"downcase_atom")||!strcmp(fn,"atom_chars")||!strcmp(fn,"atom_codes")||!strcmp(fn,"string_length")||!strcmp(fn,"string_upper")||!strcmp(fn,"string_lower")||!strcmp(fn,"atom_string")||!strcmp(fn,"string_to_atom")||!strcmp(fn,"number_string")||!strcmp(fn,"atom_number")) && IR_LIT(gg).ival == 2) {
                IR_t *aa0 = ir_call_arg(gg,0), *aa1 = ir_call_arg(gg,1);
                if (aa0 && aa0->op != IR_LOGICVAR) (*nsynth)++;
                if (aa1 && aa1->op != IR_LOGICVAR) (*nsynth)++;
            }
            if ((!strcmp(fn,"atom_concat")||!strcmp(fn,"string_concat")) && IR_LIT(gg).ival == 3) {
                IR_t *ca0 = ir_call_arg(gg,0), *ca1 = ir_call_arg(gg,1); if (ca0 && ca0->op != IR_LOGICVAR) (*nsynth)++; if (ca1 && ca1->op != IR_LOGICVAR) (*nsynth)++;
            }
            if (!strcmp(fn,"char_type") && IR_LIT(gg).ival == 2) {
                IR_t *ct0 = ir_call_arg(gg,0), *ct1 = ir_call_arg(gg,1);
                if (ct0 && ct0->op != IR_LOGICVAR) (*nsynth)++;
                if (ct1 && ct1->op != IR_LOGICVAR) (*nsynth)++;
            }
            if ((!strcmp(fn,"sort")||!strcmp(fn,"msort")) && IR_LIT(gg).ival == 2) {
                IR_t *sa0 = ir_call_arg(gg,0);
                if (sa0 && sa0->op != IR_LOGICVAR) (*nsynth)++;
            }
            if (!strcmp(fn,"numbervars") && IR_LIT(gg).ival == 3) {
                IR_t *nv0 = ir_call_arg(gg,0), *nv1 = ir_call_arg(gg,1);
                if (nv0 && nv0->op != IR_LOGICVAR) (*nsynth)++;
                if (nv1 && nv1->op != IR_LOGICVAR) (*nsynth)++;
            }
            if ((!strcmp(fn,"term_string")||!strcmp(fn,"term_to_atom")) && IR_LIT(gg).ival == 2) {
                IR_t *ts0 = ir_call_arg(gg,0);
                if (ts0 && ts0->op != IR_LOGICVAR) (*nsynth)++;
            }
            if (!strcmp(fn,"copy_term") && IR_LIT(gg).ival == 2) {
                IR_t *ct0 = ir_call_arg(gg,0), *ct1 = ir_call_arg(gg,1);
                if (ct0 && ct0->op != IR_LOGICVAR) (*nsynth)++;
                if (ct1 && ct1->op != IR_LOGICVAR) (*nsynth)++;
            }
            if ((!strcmp(fn,"nb_setval")||!strcmp(fn,"nb_getval")) && IR_LIT(gg).ival == 2) {
                IR_t *nk0 = ir_call_arg(gg,0), *nv1 = ir_call_arg(gg,1);
                if (nk0 && nk0->op != IR_LOGICVAR) (*nsynth)++;
                if (nv1 && nv1->op != IR_LOGICVAR) (*nsynth)++;
            }
            if (!strcmp(fn,"findall")) { /* bb_findall_state_t already built by lower_prolog; no extra synth slots */ }
            if ((!strcmp(fn,"atomic_list_concat")||!strcmp(fn,"concat_atom")) && IR_LIT(gg).ival == 2) {
                IR_t *al0 = ir_call_arg(gg,0);
                if (al0 && al0->op != IR_LOGICVAR) (*nsynth)++;
            }
            if (!strcmp(fn,"atomic_list_concat") && IR_LIT(gg).ival == 3) {
                IR_t *al0 = ir_call_arg(gg,0), *al1 = ir_call_arg(gg,1);
                if (al0 && al0->op != IR_LOGICVAR) (*nsynth)++;
                if (al1 && al1->op != IR_LOGICVAR) (*nsynth)++;
            }
            return 1;
        }
        return 1;
    }
    IR_t **uu = NULL; int aa = 0;
    if (pl_gz_fact_inline(gg, &uu, &aa)) return 1;
    if (pl_gz_choice_inline(gg)) return 1;
    bb_goal_state_t *zc = NULL; int ar = 0;
    IR_graph_t *cg = pl_gz_goal_callee(gg, &zc, &ar);
    if (!cg) return 0;
    for (int ai = 0; ai < ar && ai < 3; ai++)
        if (zc->args[ai] && zc->args[ai]->op != IR_LOGICVAR) (*nsynth)++;
    return 1;
}
static int pl_gz_count_synth(IR_t **buf, int n, int *nsynth) {
    for (int i = 0; i < n; i++)
        if (!pl_gz_count_synth_goal(buf[i], nsynth)) return 0;
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int pl_gz_arith_const(const IR_t *nd) {
    if (!nd) return 0;
    if (nd->op == IR_LIT_I) return 1;
    if (nd->op == IR_LIT_F) return 1;
    if (nd->op != IR_ARITH || !IR_LIT(nd).sval) return 0;
    const char *op = IR_LIT(nd).sval;
    if (strcmp(op,"+")==0||strcmp(op,"-")==0||strcmp(op,"*")==0||strcmp(op,"/")==0||strcmp(op,"//")==0||
        strcmp(op,"mod")==0||strcmp(op,"rem")==0||strcmp(op,"abs")==0||strcmp(op,"sign")==0||
        strcmp(op,"/\\")==0||strcmp(op,"\\/")==0||strcmp(op,"xor")==0||strcmp(op,">>")==0||strcmp(op,"<<")==0||
        strcmp(op,"max")==0||strcmp(op,"min")==0||strcmp(op,"gcd")==0||strcmp(op,"^")==0||strcmp(op,"**")==0||
        strcmp(op,"truncate")==0||strcmp(op,"integer")==0||strcmp(op,"round")==0||strcmp(op,"ceiling")==0||strcmp(op,"floor")==0||strcmp(op,"msb")==0||
        strcmp(op,"sqrt")==0||strcmp(op,"sin")==0||strcmp(op,"cos")==0||strcmp(op,"tan")==0||
        strcmp(op,"asin")==0||strcmp(op,"acos")==0||strcmp(op,"atan")==0||strcmp(op,"exp")==0||strcmp(op,"log")==0||
        strcmp(op,"float")==0||strcmp(op,"float_integer_part")==0||strcmp(op,"float_fractional_part")==0||
        strcmp(op,"pi")==0||strcmp(op,"e")==0||strcmp(op,"inf")==0||strcmp(op,"infinity")==0) {
        if (nd->n_operands > 0 && nd->operands[0] && !pl_gz_arith_const(nd->operands[0])) return 0;
        if (nd->n_operands > 1 && nd->operands[1] && !pl_gz_arith_const(nd->operands[1])) return 0;
        return 1;
    }
    return 0;
}
static int pl_gz_build_goal(IR_t *gg, IR_t **head, IR_t **tail, int *synth_next, int *cslot, pl_gz_callee_t **callees, int *ncallees);
static int pl_gz_build_root(IR_t *root, IR_t **rhead, int *synth_next, int *cslot, pl_gz_callee_t **callees, int *ncallees) {
    IR_t *h = NULL, *t = NULL;
    *rhead = NULL;
    if (!root || root->op == IR_SUCCEED) return 1;
    if (root->op == IR_GCONJ) {
        bb_conj_state_t *zs = (bb_conj_state_t *)(intptr_t)IR_LIT(root).ival;
        if (!zs || !zs->goals || zs->ngoals <= 0 || zs->ngoals > 64) return 0;
        for (int i = 0; i < zs->ngoals; i++)
            if (!pl_gz_build_goal(zs->goals[i], &h, &t, synth_next, cslot, callees, ncallees)) return 0;
    } else {
        if (!pl_gz_build_goal(root, &h, &t, synth_next, cslot, callees, ncallees)) return 0;
    }
    *rhead = h;
    return 1;
}
static int pl_gz_chain_det(IR_t *head) {
    for (IR_t *g = head; g; g = g->γ.node) {
        if (g->op == IR_CELL_CHOICE || g->op == IR_CELL_CALL) return 0;
        if (g->op == IR_CELL_ITE) {
            pl_gz_ite_state_t *is = (pl_gz_ite_state_t *)(intptr_t)IR_LIT(g).ival;
            if (!is || !pl_gz_chain_det(is->then_head) || !pl_gz_chain_det(is->else_head)) return 0;
        }
    }
    return 1;
}
static IR_t * pl_gz_arith_to_struct(const IR_t *nd) {
    if (!nd) return NULL;
    if (nd->op != IR_ARITH) return (IR_t *)nd;
    IR_t *c = pl_gz_det_node(IR_STRUCT); if (!c) return NULL;
    IR_LIT(c).sval = IR_LIT(nd).sval; IR_LIT(c).ival = IR_LIT(nd).ival;
    for (int i = 0; i < nd->n_operands; i++) {
        IR_t *ch = pl_gz_arith_to_struct(nd->operands[i]); if (!ch) return NULL;
        ir_operand_push(c, ch);
    }
    return c;
}
static int pl_gz_build_goal(IR_t *gg, IR_t **head, IR_t **tail, int *synth_next, int *cslot, pl_gz_callee_t **callees, int *ncallees) {
    if (!gg) return 0;
    if (gg->op == IR_SUCCEED) return 1;
    if (gg->op == IR_ITE) {
        bb_ite_state_t *zi = (bb_ite_state_t *)(intptr_t)IR_LIT(gg).ival;
        if (!zi) return 0;
        pl_gz_ite_state_t *is = (pl_gz_ite_state_t *)GC_MALLOC(sizeof *is);
        if (!is) return 0;
        memset(is, 0, sizeof *is);
        if (!pl_gz_build_root(zi->cond_root, &is->cond_head, synth_next, cslot, callees, ncallees)) return 0;
        if (!pl_gz_build_root(zi->then_root, &is->then_head, synth_next, cslot, callees, ncallees)) return 0;
        if (!pl_gz_build_root(zi->else_root, &is->else_head, synth_next, cslot, callees, ncallees)) return 0;
        if (!pl_gz_chain_det(is->then_head) || !pl_gz_chain_det(is->else_head)) return 0;
        if (*cslot + 1 > 62) return 0;
        is->gate_slot = (*cslot)++;
        IR_t *cn = pl_gz_det_node(IR_CELL_ITE);
        if (!cn) return 0;
        IR_LIT(cn).ival = (int64_t)(intptr_t)is;
        if (!*head) *head = cn; else { (*tail)->γ.node = cn; memcpy((*tail)->γ.sz, "α", 3); }
        *tail = cn;
        return 1;
    }
    if (gg->op == IR_FAIL) {
        IR_t *fnode = pl_gz_det_node(IR_FAIL);
        if (!fnode) return 0;
        if (!*head) *head = fnode; else { (*tail)->γ.node = fnode; memcpy((*tail)->γ.sz, "α", 3); }
        *tail = fnode;
        return 1;
    }
    if (gg->op == IR_GOAL) {
        IR_t **units = NULL; int ar = 0;
        if (pl_gz_fact_inline(gg, &units, &ar)) {
            bb_goal_state_t *zc = (bb_goal_state_t *)(intptr_t)IR_LIT(gg).ival;
            for (int ai = 0; ai < ar; ai++) {
                IR_t *cu = pl_gz_det_node(IR_CELL_UNIFY);
                if (!cu) return 0;
                IR_t *ca = zc->args[ai], *cb = ir_pair_arg(units[ai], 1);
                if (ca) ir_operand_push(cu, ca); if (cb) ir_operand_push(cu, cb);
                if (!*head) *head = cu; else { (*tail)->γ.node = cu; memcpy((*tail)->γ.sz, "α", 3); }
                *tail = cu;
            }
            return 1;
        }
        pl_gz_choice_state_t *st = pl_gz_choice_inline(gg);
        if (st) {
            if (*cslot + 2 > 62) return 0;
            st->mark_slot = *cslot; *cslot += 2;
            IR_t *cn = pl_gz_det_node(IR_CELL_CHOICE);
            if (!cn) return 0;
            IR_LIT(cn).ival = (int64_t)(intptr_t)st;
            if (!*head) *head = cn; else { (*tail)->γ.node = cn; memcpy((*tail)->γ.sz, "α", 3); }
            *tail = cn;
            return 1;
        }
        bb_goal_state_t *zc = NULL;
        IR_graph_t *cg = pl_gz_goal_callee(gg, &zc, &ar);
        if (!cg || ar > 3 || !pl_gz_call_args_ok(zc, ar)) return 0;
        pl_gz_callee_t *ce = pl_gz_callee_get_any(gg, cg, ar, callees, ncallees);
        if (!ce) return 0;
        pl_gz_call_state_t *cs = (pl_gz_call_state_t *)GC_MALLOC(sizeof *cs);
        if (!cs) return 0;
        memset(cs, 0, sizeof *cs);
        cs->callee = ce; cs->nargs = ar;
        if (*cslot + 1 > 62) return 0;
        cs->child_slot = (*cslot)++;
        for (int ai = 0; ai < ar; ai++) {
            IR_t *a = zc->args[ai];
            if (a->op == IR_LOGICVAR) { cs->args[ai] = a; continue; }
            int kk = (*synth_next)++;
            IR_t *cu = pl_gz_det_node(IR_CELL_UNIFY);
            if (!cu) return 0;
            IR_t *ca = pl_gz_lv(kk);
            if (!ca) return 0;
            ir_operand_push(cu, ca); ir_operand_push(cu, a);
            if (!*head) *head = cu; else { (*tail)->γ.node = cu; memcpy((*tail)->γ.sz, "α", 3); }
            *tail = cu;
            cs->args[ai] = pl_gz_lv(kk);
            if (!cs->args[ai]) return 0;
        }
        IR_t *cn = pl_gz_det_node(IR_CELL_CALL);
        if (!cn) return 0;
        IR_LIT(cn).ival = (int64_t)(intptr_t)cs;
        if (!*head) *head = cn; else { (*tail)->γ.node = cn; memcpy((*tail)->γ.sz, "α", 3); }
        *tail = cn;
        return 1;
    }
    IR_t *nn = NULL;
    if (gg->op == IR_UNIFY) {
        nn = pl_gz_det_node(IR_CELL_UNIFY);
        if (nn) { IR_t *na = ir_pair_arg(gg, 0), *nb = ir_pair_arg(gg, 1); if (na) ir_operand_push(nn, na); if (nb) ir_operand_push(nn, nb); }
    } else if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && strcmp(IR_LIT(gg).sval, "nl") == 0 && IR_LIT(gg).ival == 0) {
        nn = pl_gz_det_node(IR_DET_NL);
    } else if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && strcmp(IR_LIT(gg).sval, "write") == 0 && IR_LIT(gg).ival == 1 && ir_call_arg(gg,0)) {
        IR_t *wz0 = ir_call_arg(gg,0);
        if (wz0->op == IR_ATOM && IR_LIT(wz0).sval) { nn = pl_gz_det_node(IR_DET_WRITE); if (nn) IR_LIT(nn).sval = IR_LIT(wz0).sval; }
        else if (wz0->op == IR_LIT_I)          { nn = pl_gz_det_node(IR_DET_WRITE); if (nn) { IR_LIT(nn).sval = NULL; IR_LIT(nn).ival = IR_LIT(wz0).ival; } }
        else if (wz0->op == IR_LOGICVAR)       { nn = pl_gz_det_node(IR_DET_WRITE); if (nn) { IR_LIT(nn).sval = NULL; IR_LIT(nn).ival = 0; ir_operand_push(nn, wz0); } }
        else if (wz0->op == IR_STRUCT) {
            int kk = (*synth_next)++; IR_t *cu = pl_gz_det_node(IR_CELL_UNIFY); if (!cu) return 0;
            IR_t *ca = pl_gz_lv(kk); if (!ca) return 0;
            ir_operand_push(cu, ca); ir_operand_push(cu, wz0);
            if (!*head) *head = cu; else { (*tail)->γ.node = cu; memcpy((*tail)->γ.sz, "α", 3); } *tail = cu;
            IR_t *sv = pl_gz_lv(kk); if (!sv) return 0;
            nn = pl_gz_det_node(IR_DET_WRITE); if (nn) { IR_LIT(nn).sval = NULL; IR_LIT(nn).ival = 0; ir_operand_push(nn, sv); }
        } else return 0;
    } else if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && (!strcmp(IR_LIT(gg).sval,"writeq")||!strcmp(IR_LIT(gg).sval,"write_canonical")) && IR_LIT(gg).ival == 1 && ir_call_arg(gg,0)) {
        int wmode = !strcmp(IR_LIT(gg).sval,"writeq") ? 1 : 2;
        IR_t *wz0 = ir_call_arg(gg,0);
        IR_t *sv = NULL;
        if (wz0->op == IR_LOGICVAR) { sv = wz0; }
        else if (wz0->op == IR_ATOM || wz0->op == IR_STRUCT) {
            int kk = (*synth_next)++; IR_t *cu = pl_gz_det_node(IR_CELL_UNIFY); if (!cu) return 0;
            IR_t *ca = pl_gz_lv(kk); if (!ca) return 0;
            ir_operand_push(cu, ca); ir_operand_push(cu, wz0);
            if (!*head) *head = cu; else { (*tail)->γ.node = cu; memcpy((*tail)->γ.sz, "α", 3); } *tail = cu;
            sv = pl_gz_lv(kk); if (!sv) return 0;
        } else return 0;
        nn = pl_gz_det_node(IR_DET_WRITE); if (nn) { IR_LIT(nn).sval = NULL; IR_LIT(nn).ival = wmode; ir_operand_push(nn, sv); }
    } else if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && strcmp(IR_LIT(gg).sval, "is") == 0 && IR_LIT(gg).ival == 2 && ir_pair_arg(gg,0) && ir_pair_arg(gg,1)) {
        IR_t *lhs = ir_pair_arg(gg,0), *rhs = ir_pair_arg(gg,1);
        if (lhs->op != IR_LOGICVAR) return 0;
        int rhs_is_const = (rhs->op == IR_LIT_I) || (rhs->op == IR_LIT_F) || ((rhs->op == IR_ARITH) && pl_gz_arith_const(rhs));
        IR_t *w0 = (rhs->n_operands > 0) ? rhs->operands[0] : NULL, *w1 = (rhs->n_operands > 1) ? rhs->operands[1] : NULL;
        int rhs_is_var_op = (rhs->op == IR_LOGICVAR) ||
                            (rhs->op == IR_ARITH && IR_LIT(rhs).sval && w0 && w1 &&
                             w0->op == IR_LOGICVAR && w1->op == IR_LIT_I);
        if (!rhs_is_const && !rhs_is_var_op) return 0;
        nn = pl_gz_det_node(IR_DET_IS);
        if (nn) { ir_operand_push(nn, lhs); ir_operand_push(nn, rhs); }
    } else if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && !strcmp(IR_LIT(gg).sval, "functor") && IR_LIT(gg).ival == 3 && ir_call_arg(gg,0) && ir_call_arg(gg,1) && ir_call_arg(gg,2)) {
        IR_t *a0 = ir_call_arg(gg,0), *a1 = ir_call_arg(gg,1), *a2 = ir_call_arg(gg,2);
        IR_t *s0 = NULL, *s1 = NULL, *s2 = NULL;
        if (a0->op == IR_LOGICVAR) { s0 = a0; }
        else {
            int kk = (*synth_next)++;
            IR_t *cu = pl_gz_det_node(IR_CELL_UNIFY); if (!cu) return 0;
            IR_t *ca = pl_gz_lv(kk); if (!ca) return 0;
            ir_operand_push(cu, ca); ir_operand_push(cu, a0);
            if (!*head) *head = cu; else { (*tail)->γ.node = cu; memcpy((*tail)->γ.sz, "α", 3); } *tail = cu;
            s0 = pl_gz_lv(kk); if (!s0) return 0;
        }
        s1 = (a1->op == IR_LOGICVAR) ? a1 : NULL; if (!s1) return 0;
        s2 = (a2->op == IR_LOGICVAR) ? a2 : NULL; if (!s2) return 0;
        nn = pl_gz_det_node(IR_DET_FUNCTOR);
        if (nn) { ir_operand_push(nn, s0); ir_operand_push(nn, s1); ir_operand_push(nn, s2); }
    } else if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && !strcmp(IR_LIT(gg).sval, "arg") && IR_LIT(gg).ival == 3 && ir_call_arg(gg,0) && ir_call_arg(gg,1) && ir_call_arg(gg,2)) {
        IR_t *a0 = ir_call_arg(gg,0), *a1 = ir_call_arg(gg,1), *a2 = ir_call_arg(gg,2);
        IR_t *s0 = NULL, *s1 = NULL, *s2 = NULL;
        if (a0->op == IR_LOGICVAR) { s0 = a0; }
        else if (a0->op == IR_LIT_I) {
            int kk = (*synth_next)++;
            IR_t *cu = pl_gz_det_node(IR_CELL_UNIFY); if (!cu) return 0;
            IR_t *ca = pl_gz_lv(kk); if (!ca) return 0;
            ir_operand_push(cu, ca); ir_operand_push(cu, a0);
            if (!*head) *head = cu; else { (*tail)->γ.node = cu; memcpy((*tail)->γ.sz, "α", 3); } *tail = cu;
            s0 = pl_gz_lv(kk); if (!s0) return 0;
        } else return 0;
        if (a1->op == IR_LOGICVAR) { s1 = a1; }
        else if (a1->op == IR_STRUCT) {
            int kk = (*synth_next)++;
            IR_t *cu = pl_gz_det_node(IR_CELL_UNIFY); if (!cu) return 0;
            IR_t *ca = pl_gz_lv(kk); if (!ca) return 0;
            ir_operand_push(cu, ca); ir_operand_push(cu, a1);
            if (!*head) *head = cu; else { (*tail)->γ.node = cu; memcpy((*tail)->γ.sz, "α", 3); } *tail = cu;
            s1 = pl_gz_lv(kk); if (!s1) return 0;
        } else return 0;
        s2 = (a2->op == IR_LOGICVAR) ? a2 : NULL; if (!s2) return 0;
        nn = pl_gz_det_node(IR_DET_ARG);
        if (nn) { ir_operand_push(nn, s0); ir_operand_push(nn, s1); ir_operand_push(nn, s2); }
    } else if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && !strcmp(IR_LIT(gg).sval, "=..") && IR_LIT(gg).ival == 2 && ir_call_arg(gg,0) && ir_call_arg(gg,1)) {
        IR_t *a0 = ir_call_arg(gg,0), *a1 = ir_call_arg(gg,1);
        IR_t *s0 = NULL, *s1 = NULL;
        if (a0->op == IR_LOGICVAR) { s0 = a0; }
        else if (a0->op == IR_STRUCT) {
            int kk = (*synth_next)++;
            IR_t *cu = pl_gz_det_node(IR_CELL_UNIFY); if (!cu) return 0;
            IR_t *ca = pl_gz_lv(kk); if (!ca) return 0;
            ir_operand_push(cu, ca); ir_operand_push(cu, a0);
            if (!*head) *head = cu; else { (*tail)->γ.node = cu; memcpy((*tail)->γ.sz, "α", 3); } *tail = cu;
            s0 = pl_gz_lv(kk); if (!s0) return 0;
        } else return 0;
        if (a1->op == IR_LOGICVAR) { s1 = a1; }
        else if (a1->op == IR_STRUCT) {
            int kk = (*synth_next)++;
            IR_t *cu = pl_gz_det_node(IR_CELL_UNIFY); if (!cu) return 0;
            IR_t *ca = pl_gz_lv(kk); if (!ca) return 0;
            ir_operand_push(cu, ca); ir_operand_push(cu, a1);
            if (!*head) *head = cu; else { (*tail)->γ.node = cu; memcpy((*tail)->γ.sz, "α", 3); } *tail = cu;
            s1 = pl_gz_lv(kk); if (!s1) return 0;
        } else return 0;
        nn = pl_gz_det_node(IR_DET_UNIV);
        if (nn) { ir_operand_push(nn, s0); ir_operand_push(nn, s1); }
    } else if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && ((!strcmp(IR_LIT(gg).sval, "succ") && IR_LIT(gg).ival == 2 && ir_pair_arg(gg,0) && ir_pair_arg(gg,1)) ||
               (!strcmp(IR_LIT(gg).sval, "plus") && IR_LIT(gg).ival == 3 && ir_call_arg(gg,0) && ir_call_arg(gg,1) && ir_call_arg(gg,2)))) {
        int spar = (int)IR_LIT(gg).ival;
        IR_t *ss[3] = { NULL, NULL, NULL };
        for (int ai = 0; ai < spar; ai++) {
            IR_t *ax = (spar == 2) ? ir_pair_arg(gg, ai) : ir_call_arg(gg, ai);
            if (ax->op == IR_LOGICVAR) { ss[ai] = ax; continue; }
            if (ax->op != IR_LIT_I) return 0;
            int kk = (*synth_next)++;
            IR_t *cu = pl_gz_det_node(IR_CELL_UNIFY); if (!cu) return 0;
            IR_t *ca = pl_gz_lv(kk); if (!ca) return 0;
            ir_operand_push(cu, ca); ir_operand_push(cu, ax);
            if (!*head) *head = cu; else { (*tail)->γ.node = cu; memcpy((*tail)->γ.sz, "α", 3); } *tail = cu;
            ss[ai] = pl_gz_lv(kk); if (!ss[ai]) return 0;
        }
        nn = pl_gz_det_node(IR_DET_SUCC_PLUS);
        if (nn) { IR_LIT(nn).ival = spar; for (int ai = 0; ai < spar; ai++) ir_operand_push(nn, ss[ai]); }
    } else if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && ((!strcmp(IR_LIT(gg).sval,"atom_length")||!strcmp(IR_LIT(gg).sval,"upcase_atom")||!strcmp(IR_LIT(gg).sval,"downcase_atom")||!strcmp(IR_LIT(gg).sval,"atom_chars")||!strcmp(IR_LIT(gg).sval,"atom_codes")||!strcmp(IR_LIT(gg).sval,"string_length")||!strcmp(IR_LIT(gg).sval,"string_upper")||!strcmp(IR_LIT(gg).sval,"string_lower")||!strcmp(IR_LIT(gg).sval,"atom_string")||!strcmp(IR_LIT(gg).sval,"string_to_atom")||!strcmp(IR_LIT(gg).sval,"number_string")||!strcmp(IR_LIT(gg).sval,"atom_number")) && IR_LIT(gg).ival == 2)) {
        const char *aofn = IR_LIT(gg).sval;
        IR_t *aa0 = ir_call_arg(gg,0), *aa1 = ir_call_arg(gg,1);
        IR_t *sa0 = NULL, *sa1 = NULL;
        if (aa0->op == IR_LOGICVAR) { sa0 = aa0; }
        else if (aa0->op == IR_ATOM || aa0->op == IR_LIT_I || aa0->op == IR_LIT_F) {
            int kk = (*synth_next)++; IR_t *cu = pl_gz_det_node(IR_CELL_UNIFY); if (!cu) return 0;
            IR_t *ca = pl_gz_lv(kk); if (!ca) return 0;
            ir_operand_push(cu, ca); ir_operand_push(cu, aa0);
            if (!*head) *head = cu; else { (*tail)->γ.node = cu; memcpy((*tail)->γ.sz, "α", 3); } *tail = cu;
            sa0 = pl_gz_lv(kk); if (!sa0) return 0;
        } else return 0;
        sa1 = (aa1->op == IR_LOGICVAR) ? aa1 : NULL;
        if (!sa1 && (aa1->op == IR_STRUCT || aa1->op == IR_ATOM)) {
            int kk = (*synth_next)++; IR_t *cu = pl_gz_det_node(IR_CELL_UNIFY); if (!cu) return 0;
            IR_t *ca = pl_gz_lv(kk); if (!ca) return 0;
            ir_operand_push(cu, ca); ir_operand_push(cu, aa1);
            if (!*head) *head = cu; else { (*tail)->γ.node = cu; memcpy((*tail)->γ.sz, "α", 3); } *tail = cu;
            sa1 = pl_gz_lv(kk); if (!sa1) return 0;
        }
        if (!sa1) return 0;
        nn = pl_gz_det_node(IR_DET_ATOM_OP);
        if (nn) { IR_LIT(nn).sval = aofn; IR_LIT(nn).ival = 2; ir_operand_push(nn, sa0); ir_operand_push(nn, sa1); }
    } else if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && (!strcmp(IR_LIT(gg).sval, "atom_concat")||!strcmp(IR_LIT(gg).sval, "string_concat")) && IR_LIT(gg).ival == 3 && ir_call_arg(gg,0) && ir_call_arg(gg,1) && ir_call_arg(gg,2)) {
        const char *catfn = IR_LIT(gg).sval;
        IR_t *ca0 = ir_call_arg(gg,0), *ca1 = ir_call_arg(gg,1), *ca2 = ir_call_arg(gg,2);
        IR_t *sc0 = NULL, *sc1 = NULL, *sc2 = NULL;
        if (ca0->op == IR_LOGICVAR) { sc0 = ca0; }
        else if (ca0->op == IR_ATOM) {
            int kk = (*synth_next)++; IR_t *cu = pl_gz_det_node(IR_CELL_UNIFY); if (!cu) return 0;
            IR_t *ca = pl_gz_lv(kk); if (!ca) return 0;
            ir_operand_push(cu, ca); ir_operand_push(cu, ca0);
            if (!*head) *head = cu; else { (*tail)->γ.node = cu; memcpy((*tail)->γ.sz, "α", 3); } *tail = cu;
            sc0 = pl_gz_lv(kk); if (!sc0) return 0;
        } else return 0;
        if (ca1->op == IR_LOGICVAR) { sc1 = ca1; }
        else if (ca1->op == IR_ATOM) {
            int kk = (*synth_next)++; IR_t *cu = pl_gz_det_node(IR_CELL_UNIFY); if (!cu) return 0;
            IR_t *ca = pl_gz_lv(kk); if (!ca) return 0;
            ir_operand_push(cu, ca); ir_operand_push(cu, ca1);
            if (!*head) *head = cu; else { (*tail)->γ.node = cu; memcpy((*tail)->γ.sz, "α", 3); } *tail = cu;
            sc1 = pl_gz_lv(kk); if (!sc1) return 0;
        } else return 0;
        sc2 = (ca2->op == IR_LOGICVAR) ? ca2 : NULL; if (!sc2) return 0;
        nn = pl_gz_det_node(IR_DET_ATOM_OP);
        if (nn) { IR_LIT(nn).sval = catfn; IR_LIT(nn).ival = 3; ir_operand_push(nn, sc0); ir_operand_push(nn, sc1); ir_operand_push(nn, sc2); }
    } else if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && !strcmp(IR_LIT(gg).sval, "char_type") && IR_LIT(gg).ival == 2 && ir_call_arg(gg,0) && ir_call_arg(gg,1)) {
        IR_t *ct0 = ir_call_arg(gg,0), *ct1 = ir_call_arg(gg,1);
        IR_t *sc = NULL, *st = NULL;
        if (ct0->op == IR_LOGICVAR) { sc = ct0; }
        else if (ct0->op == IR_ATOM) {
            int kk = (*synth_next)++; IR_t *cu = pl_gz_det_node(IR_CELL_UNIFY); if (!cu) return 0;
            IR_t *ca = pl_gz_lv(kk); if (!ca) return 0;
            ir_operand_push(cu, ca); ir_operand_push(cu, ct0);
            if (!*head) *head = cu; else { (*tail)->γ.node = cu; memcpy((*tail)->γ.sz, "α", 3); } *tail = cu;
            sc = pl_gz_lv(kk); if (!sc) return 0;
        } else return 0;
        if (ct1->op == IR_LOGICVAR) { st = ct1; }
        else if (ct1->op == IR_ATOM || ct1->op == IR_STRUCT) {
            int kk = (*synth_next)++; IR_t *cu = pl_gz_det_node(IR_CELL_UNIFY); if (!cu) return 0;
            IR_t *ca = pl_gz_lv(kk); if (!ca) return 0;
            ir_operand_push(cu, ca); ir_operand_push(cu, ct1);
            if (!*head) *head = cu; else { (*tail)->γ.node = cu; memcpy((*tail)->γ.sz, "α", 3); } *tail = cu;
            st = pl_gz_lv(kk); if (!st) return 0;
        } else return 0;
        if (!st) return 0;
        nn = pl_gz_det_node(IR_DET_CHAR_TYPE);
        if (nn) { IR_LIT(nn).ival = 2; ir_operand_push(nn, sc); ir_operand_push(nn, st); }
    } else if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && (!strcmp(IR_LIT(gg).sval,"sort")||!strcmp(IR_LIT(gg).sval,"msort")) && IR_LIT(gg).ival == 2 && ir_call_arg(gg,0) && ir_call_arg(gg,1)) {
        int do_msort = (!strcmp(IR_LIT(gg).sval,"msort")) ? 1 : 0;
        IR_t *sa0 = ir_call_arg(gg,0), *sa1 = ir_call_arg(gg,1);
        IR_t *slist = NULL;
        if (sa0->op == IR_LOGICVAR) { slist = sa0; }
        else if (sa0->op == IR_STRUCT || sa0->op == IR_ATOM) {
            int kk = (*synth_next)++; IR_t *cu = pl_gz_det_node(IR_CELL_UNIFY); if (!cu) return 0;
            IR_t *ca = pl_gz_lv(kk); if (!ca) return 0;
            ir_operand_push(cu, ca); ir_operand_push(cu, sa0);
            if (!*head) *head = cu; else { (*tail)->γ.node = cu; memcpy((*tail)->γ.sz, "α", 3); } *tail = cu;
            slist = pl_gz_lv(kk); if (!slist) return 0;
        } else return 0;
        if (sa1->op != IR_LOGICVAR) return 0;
        nn = pl_gz_det_node(IR_DET_SORT);
        if (nn) { IR_LIT(nn).ival = do_msort; ir_operand_push(nn, slist); ir_operand_push(nn, sa1); }
    } else if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && !strcmp(IR_LIT(gg).sval,"numbervars") && IR_LIT(gg).ival == 3 && ir_call_arg(gg,0) && ir_call_arg(gg,1) && ir_call_arg(gg,2)) {
        IR_t *nv0 = ir_call_arg(gg,0), *nv1 = ir_call_arg(gg,1), *nv2 = ir_call_arg(gg,2);
        IR_t *st = NULL, *ss = NULL;
        if (nv0->op == IR_LOGICVAR) { st = nv0; }
        else if (nv0->op == IR_STRUCT || nv0->op == IR_ATOM) {
            int kk = (*synth_next)++; IR_t *cu = pl_gz_det_node(IR_CELL_UNIFY); if (!cu) return 0;
            IR_t *ca = pl_gz_lv(kk); if (!ca) return 0;
            ir_operand_push(cu, ca); ir_operand_push(cu, nv0);
            if (!*head) *head = cu; else { (*tail)->γ.node = cu; memcpy((*tail)->γ.sz, "α", 3); } *tail = cu;
            st = pl_gz_lv(kk); if (!st) return 0;
        } else return 0;
        if (nv1->op == IR_LOGICVAR) { ss = nv1; }
        else if (nv1->op == IR_LIT_I) {
            int kk = (*synth_next)++; IR_t *cu = pl_gz_det_node(IR_CELL_UNIFY); if (!cu) return 0;
            IR_t *ca = pl_gz_lv(kk); if (!ca) return 0;
            ir_operand_push(cu, ca); ir_operand_push(cu, nv1);
            if (!*head) *head = cu; else { (*tail)->γ.node = cu; memcpy((*tail)->γ.sz, "α", 3); } *tail = cu;
            ss = pl_gz_lv(kk); if (!ss) return 0;
        } else return 0;
        if (nv2->op != IR_LOGICVAR) return 0;
        nn = pl_gz_det_node(IR_DET_NUMBERVARS);
        if (nn) { ir_operand_push(nn, st); ir_operand_push(nn, ss); ir_operand_push(nn, nv2); }
    } else if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && (!strcmp(IR_LIT(gg).sval,"term_string")||!strcmp(IR_LIT(gg).sval,"term_to_atom")) && IR_LIT(gg).ival == 2 && ir_call_arg(gg,0) && ir_call_arg(gg,1)) {
        IR_t *ts0 = ir_call_arg(gg,0), *ts1 = ir_call_arg(gg,1);
        IR_t *st = NULL;
        if (ts0->op == IR_LOGICVAR) { st = ts0; }
        else if (ts0->op == IR_STRUCT || ts0->op == IR_ATOM || ts0->op == IR_LIT_I) {
            int kk = (*synth_next)++; IR_t *cu = pl_gz_det_node(IR_CELL_UNIFY); if (!cu) return 0;
            IR_t *ca = pl_gz_lv(kk); if (!ca) return 0;
            ir_operand_push(cu, ca); ir_operand_push(cu, ts0);
            if (!*head) *head = cu; else { (*tail)->γ.node = cu; memcpy((*tail)->γ.sz, "α", 3); } *tail = cu;
            st = pl_gz_lv(kk); if (!st) return 0;
        } else if (ts0->op == IR_ARITH) {
            IR_t *as = pl_gz_arith_to_struct(ts0); if (!as) return 0;
            int kk = (*synth_next)++; IR_t *cu = pl_gz_det_node(IR_CELL_UNIFY); if (!cu) return 0;
            IR_t *ca = pl_gz_lv(kk); if (!ca) return 0;
            ir_operand_push(cu, ca); ir_operand_push(cu, as);
            if (!*head) *head = cu; else { (*tail)->γ.node = cu; memcpy((*tail)->γ.sz, "α", 3); } *tail = cu;
            st = pl_gz_lv(kk); if (!st) return 0;
        } else return 0;
        if (ts1->op != IR_LOGICVAR) return 0;
        nn = pl_gz_det_node(IR_DET_TERM_STRING);
        if (nn) { ir_operand_push(nn, st); ir_operand_push(nn, ts1); }
    } else if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && (!strcmp(IR_LIT(gg).sval,"findall")||!strcmp(IR_LIT(gg).sval,"aggregate_all"))) {
        if (!*head) *head = gg; else { (*tail)->γ.node = gg; memcpy((*tail)->γ.sz, "α", 3); }
        *tail = gg; gg->γ.node = NULL;
        return 1;
    } else if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && !strcmp(IR_LIT(gg).sval,"copy_term") && IR_LIT(gg).ival == 2 && ir_call_arg(gg,0) && ir_call_arg(gg,1)) {
        IR_t *ct0 = ir_call_arg(gg,0), *ct1 = ir_call_arg(gg,1);
        IR_t *sc = NULL;
        if (ct0->op == IR_LOGICVAR) { sc = ct0; }
        else if (ct0->op == IR_STRUCT || ct0->op == IR_ATOM || ct0->op == IR_LIT_I) {
            int kk = (*synth_next)++; IR_t *cu = pl_gz_det_node(IR_CELL_UNIFY); if (!cu) return 0;
            IR_t *ca = pl_gz_lv(kk); if (!ca) return 0;
            ir_operand_push(cu, ca); ir_operand_push(cu, ct0);
            if (!*head) *head = cu; else { (*tail)->γ.node = cu; memcpy((*tail)->γ.sz, "α", 3); } *tail = cu;
            sc = pl_gz_lv(kk); if (!sc) return 0;
        } else return 0;
        IR_t *sd = NULL;
        if (ct1->op == IR_LOGICVAR) { sd = ct1; }
        else if (ct1->op == IR_STRUCT || ct1->op == IR_ATOM || ct1->op == IR_LIT_I) {
            int kk = (*synth_next)++; IR_t *cu = pl_gz_det_node(IR_CELL_UNIFY); if (!cu) return 0;
            IR_t *ca = pl_gz_lv(kk); if (!ca) return 0;
            ir_operand_push(cu, ca); ir_operand_push(cu, ct1);
            if (!*head) *head = cu; else { (*tail)->γ.node = cu; memcpy((*tail)->γ.sz, "α", 3); } *tail = cu;
            sd = pl_gz_lv(kk); if (!sd) return 0;
        } else return 0;
        nn = pl_gz_det_node(IR_DET_COPY_TERM);
        if (nn) { ir_operand_push(nn, sc); ir_operand_push(nn, sd); }
    } else if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && (!strcmp(IR_LIT(gg).sval,"nb_setval")||!strcmp(IR_LIT(gg).sval,"nb_getval")) && IR_LIT(gg).ival == 2 && ir_call_arg(gg,0) && ir_call_arg(gg,1)) {
        int nb_set = !strcmp(IR_LIT(gg).sval,"nb_setval");
        IR_t *nk0 = ir_call_arg(gg,0), *nv1 = ir_call_arg(gg,1);
        IR_t *skey = NULL, *sval = NULL;
        if (nk0->op == IR_LOGICVAR) { skey = nk0; }
        else if (nk0->op == IR_ATOM) {
            int kk = (*synth_next)++; IR_t *cu = pl_gz_det_node(IR_CELL_UNIFY); if (!cu) return 0;
            IR_t *ca = pl_gz_lv(kk); if (!ca) return 0;
            ir_operand_push(cu, ca); ir_operand_push(cu, nk0);
            if (!*head) *head = cu; else { (*tail)->γ.node = cu; memcpy((*tail)->γ.sz, "α", 3); } *tail = cu;
            skey = pl_gz_lv(kk); if (!skey) return 0;
        } else return 0;
        if (nv1->op == IR_LOGICVAR) { sval = nv1; }
        else if (nv1->op == IR_STRUCT || nv1->op == IR_ATOM || nv1->op == IR_LIT_I || nv1->op == IR_LIT_F) {
            int kk = (*synth_next)++; IR_t *cu = pl_gz_det_node(IR_CELL_UNIFY); if (!cu) return 0;
            IR_t *ca = pl_gz_lv(kk); if (!ca) return 0;
            ir_operand_push(cu, ca); ir_operand_push(cu, nv1);
            if (!*head) *head = cu; else { (*tail)->γ.node = cu; memcpy((*tail)->γ.sz, "α", 3); } *tail = cu;
            sval = pl_gz_lv(kk); if (!sval) return 0;
        } else return 0;
        nn = pl_gz_det_node(nb_set ? IR_DET_NB_SETVAL : IR_DET_NB_GETVAL);
        if (nn) { ir_operand_push(nn, skey); ir_operand_push(nn, sval); }
    } else if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && (!strcmp(IR_LIT(gg).sval,"atomic_list_concat")||!strcmp(IR_LIT(gg).sval,"concat_atom")) && IR_LIT(gg).ival == 2 && ir_call_arg(gg,0) && ir_call_arg(gg,1)) {
        const char *alcfn = IR_LIT(gg).sval;
        IR_t *al0 = ir_call_arg(gg,0), *al1 = ir_call_arg(gg,1);
        IR_t *slist = NULL;
        if (al0->op == IR_LOGICVAR) { slist = al0; }
        else if (al0->op == IR_STRUCT || al0->op == IR_ATOM) {
            int kk = (*synth_next)++; IR_t *cu = pl_gz_det_node(IR_CELL_UNIFY); if (!cu) return 0;
            IR_t *ca = pl_gz_lv(kk); if (!ca) return 0;
            ir_operand_push(cu, ca); ir_operand_push(cu, al0);
            if (!*head) *head = cu; else { (*tail)->γ.node = cu; memcpy((*tail)->γ.sz, "α", 3); } *tail = cu;
            slist = pl_gz_lv(kk); if (!slist) return 0;
        } else return 0;
        if (al1->op != IR_LOGICVAR) return 0;
        nn = pl_gz_det_node(IR_DET_ATOM_OP);
        if (nn) { IR_LIT(nn).sval = alcfn; IR_LIT(nn).ival = 2; ir_operand_push(nn, slist); ir_operand_push(nn, al1); }
    } else if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && !strcmp(IR_LIT(gg).sval,"atomic_list_concat") && IR_LIT(gg).ival == 3 && ir_call_arg(gg,0) && ir_call_arg(gg,1) && ir_call_arg(gg,2)) {
        IR_t *al0 = ir_call_arg(gg,0), *al1 = ir_call_arg(gg,1), *al2 = ir_call_arg(gg,2);
        IR_t *slist = NULL, *ssep = NULL;
        if (al0->op == IR_LOGICVAR) { slist = al0; }
        else if (al0->op == IR_STRUCT || al0->op == IR_ATOM) {
            int kk = (*synth_next)++; IR_t *cu = pl_gz_det_node(IR_CELL_UNIFY); if (!cu) return 0;
            IR_t *ca = pl_gz_lv(kk); if (!ca) return 0;
            ir_operand_push(cu, ca); ir_operand_push(cu, al0);
            if (!*head) *head = cu; else { (*tail)->γ.node = cu; memcpy((*tail)->γ.sz, "α", 3); } *tail = cu;
            slist = pl_gz_lv(kk); if (!slist) return 0;
        } else return 0;
        if (al1->op == IR_LOGICVAR) { ssep = al1; }
        else if (al1->op == IR_ATOM) {
            int kk = (*synth_next)++; IR_t *cu = pl_gz_det_node(IR_CELL_UNIFY); if (!cu) return 0;
            IR_t *ca = pl_gz_lv(kk); if (!ca) return 0;
            ir_operand_push(cu, ca); ir_operand_push(cu, al1);
            if (!*head) *head = cu; else { (*tail)->γ.node = cu; memcpy((*tail)->γ.sz, "α", 3); } *tail = cu;
            ssep = pl_gz_lv(kk); if (!ssep) return 0;
        } else return 0;
        if (al2->op != IR_LOGICVAR) return 0;
        nn = pl_gz_det_node(IR_DET_ATOM_OP);
        if (nn) { IR_LIT(nn).sval = "atomic_list_concat"; IR_LIT(nn).ival = 3; ir_operand_push(nn, slist); ir_operand_push(nn, ssep); ir_operand_push(nn, al2); }
    } else if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && IR_LIT(gg).ival == 2 && ir_pair_arg(gg,0) && ir_pair_arg(gg,1)) {
        const char *fn = IR_LIT(gg).sval;
        int is_arith_cmp = (strcmp(fn,"<")==0||strcmp(fn,">")==0||strcmp(fn,">=")==0||strcmp(fn,"=<")==0||strcmp(fn,"=:=")==0||strcmp(fn,"=\\=")==0);
        int is_tcmp = (strcmp(fn,"@<")==0||strcmp(fn,"@>")==0||strcmp(fn,"@=<")==0||strcmp(fn,"@>=")==0||strcmp(fn,"==")==0||strcmp(fn,"\\==")==0);
        if (!is_arith_cmp && !is_tcmp && strcmp(fn,"format") != 0) return 0;
        if (!strcmp(fn,"format")) {
            IR_t *fa0 = ir_call_arg(gg,0); if (!fa0 || fa0->op != IR_ATOM || !IR_LIT(fa0).sval) return 0;
            const char *fmt_str = IR_LIT(fa0).sval;
            IR_t *a1 = ir_call_arg(gg,1); if (!a1) return 0;
            IR_t *list_slot = NULL;
            if (a1->op == IR_LOGICVAR) { list_slot = a1; }
            else {
                int kk = (*synth_next)++;
                IR_t *cu = pl_gz_det_node(IR_CELL_UNIFY); if (!cu) return 0;
                IR_t *ca = pl_gz_lv(kk); if (!ca) return 0;
                ir_operand_push(cu, ca); ir_operand_push(cu, a1);
                if (!*head) *head = cu; else { (*tail)->γ.node = cu; memcpy((*tail)->γ.sz, "α", 3); } *tail = cu;
                list_slot = pl_gz_lv(kk); if (!list_slot) return 0;
            }
            nn = pl_gz_det_node(IR_DET_FORMAT);
            if (nn) { IR_LIT(nn).sval = fmt_str; IR_LIT(nn).ival = 2; ir_operand_push(nn, list_slot); }
        } else {
        IR_t *la = ir_pair_arg(gg,0), *ra = ir_pair_arg(gg,1);
        if (is_arith_cmp) {
            if (la->op != IR_LIT_I && la->op != IR_LOGICVAR) return 0;
            if (ra->op != IR_LIT_I && ra->op != IR_LOGICVAR) return 0;
            nn = pl_gz_det_node(IR_DET_CMP);
            if (nn) { IR_LIT(nn).sval = fn; ir_operand_push(nn, la); ir_operand_push(nn, ra); }
        } else {
            IR_t *sl = NULL, *sr = NULL;
            if (la->op == IR_LOGICVAR) { sl = la; }
            else {
                int kl = (*synth_next)++;
                IR_t *cu = pl_gz_det_node(IR_CELL_UNIFY); if (!cu) return 0;
                IR_t *ca = pl_gz_lv(kl); if (!ca) return 0;
                ir_operand_push(cu, ca); ir_operand_push(cu, la);
                if (!*head) *head = cu; else { (*tail)->γ.node = cu; memcpy((*tail)->γ.sz, "α", 3); } *tail = cu;
                sl = pl_gz_lv(kl); if (!sl) return 0;
            }
            if (ra->op == IR_LOGICVAR) { sr = ra; }
            else {
                int kr = (*synth_next)++;
                IR_t *cu = pl_gz_det_node(IR_CELL_UNIFY); if (!cu) return 0;
                IR_t *ca = pl_gz_lv(kr); if (!ca) return 0;
                ir_operand_push(cu, ca); ir_operand_push(cu, ra);
                if (!*head) *head = cu; else { (*tail)->γ.node = cu; memcpy((*tail)->γ.sz, "α", 3); } *tail = cu;
                sr = pl_gz_lv(kr); if (!sr) return 0;
            }
            nn = pl_gz_det_node(IR_DET_CMP);
            if (nn) { IR_LIT(nn).sval = fn; ir_operand_push(nn, sl); ir_operand_push(nn, sr); }
        }
        }
    } else if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && IR_LIT(gg).ival == 1 && ir_call_arg(gg,0)) {
        const char *fn = IR_LIT(gg).sval;
        int is_ttest = (strcmp(fn,"var")==0||strcmp(fn,"nonvar")==0||strcmp(fn,"atom")==0||strcmp(fn,"atomic")==0||
                        strcmp(fn,"number")==0||strcmp(fn,"integer")==0||strcmp(fn,"float")==0||strcmp(fn,"compound")==0||
                        strcmp(fn,"callable")==0||strcmp(fn,"is_list")==0||strcmp(fn,"ground")==0);
        if (!is_ttest && strcmp(fn,"format") != 0) return 0;
        if (!strcmp(fn,"format")) {
            IR_t *fa0 = ir_call_arg(gg,0);
            if (!fa0 || fa0->op != IR_ATOM || !IR_LIT(fa0).sval) return 0;
            nn = pl_gz_det_node(IR_DET_FORMAT);
            if (nn) { IR_LIT(nn).sval = IR_LIT(fa0).sval; IR_LIT(nn).ival = 1; }
        } else {
        IR_t *a0 = ir_call_arg(gg,0);
        IR_t *slot_node = NULL;
        if (a0->op == IR_LOGICVAR) {
            slot_node = a0;
        } else {
            int kk = (*synth_next)++;
            IR_t *cu = pl_gz_det_node(IR_CELL_UNIFY);
            if (!cu) return 0;
            IR_t *ca = pl_gz_lv(kk); if (!ca) return 0;
            ir_operand_push(cu, ca); ir_operand_push(cu, a0);
            if (!*head) *head = cu; else { (*tail)->γ.node = cu; memcpy((*tail)->γ.sz, "α", 3); }
            *tail = cu;
            slot_node = pl_gz_lv(kk); if (!slot_node) return 0;
        }
        nn = pl_gz_det_node(IR_DET_TYPE_TEST);
        if (nn) { IR_LIT(nn).sval = fn; ir_operand_push(nn, slot_node); }
        }
    } else if (gg->op == IR_BUILTIN && IR_LIT(gg).sval && !strcmp(IR_LIT(gg).sval, "format") && IR_LIT(gg).ival == 2) {
        IR_t *fa0 = ir_call_arg(gg,0); if (!fa0 || fa0->op != IR_ATOM || !IR_LIT(fa0).sval) return 0;
        return 0;
    }
    if (!nn) return 0;
    if (!*head) *head = nn; else { (*tail)->γ.node = nn; memcpy((*tail)->γ.sz, "α", 3); }
    *tail = nn;
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int g_gz_no_struct_ptr = 0;
static IR_t * pl_gz_admit(IR_graph_t *g) {
    if (!g || !g->all || g->nslots > 64) return NULL;
    IR_t *gconjs[2] = {NULL, NULL}; int ngconj = 0;
    IR_t *softdisj = NULL; IR_t *soft_arm0 = NULL;
    IR_t *claimed[8]; int nclaimed = 0;
    for (int i = 0; i < g->n; i++) {
        IR_t *nd = g->all[i];
        if (!nd || nd->op != IR_ITE) continue;
        bb_ite_state_t *zi = (bb_ite_state_t *)(intptr_t)IR_LIT(nd).ival;
        if (!zi) return NULL;
        IR_t *rr[3] = { zi->cond_root, zi->then_root, zi->else_root };
        for (int k = 0; k < 3; k++)
            if (rr[k] && rr[k]->op == IR_GCONJ) { if (nclaimed >= 8) return NULL; claimed[nclaimed++] = rr[k]; }
    }
    for (int i = 0; i < g->n; i++) {
        IR_t *nd = g->all[i];
        if (!nd) continue;
        if (nd->op == IR_GCONJ) {
            int cl = 0;
            for (int k = 0; k < nclaimed; k++) if (claimed[k] == nd) { cl = 1; break; }
            if (!cl) { if (ngconj >= 2) return NULL; gconjs[ngconj++] = nd; }
        }
        if (nd->op == IR_DISJ) {
            if (softdisj) return NULL;
            int na = 0;
            IR_t * const *arms = bb_operand_aux_get(g, nd, &na);
            if (!arms || na != 2 || !arms[0] || !arms[1]) return NULL;
            if (arms[1]->op != IR_SUCCEED) return NULL;
            softdisj = nd; soft_arm0 = arms[0];
        }
        if (nd->op == IR_GOAL) { IR_t **uu = NULL; int aa = 0; if (!pl_gz_fact_inline(nd, &uu, &aa) && !pl_gz_choice_inline(nd) && !pl_gz_rule_inline_check(nd)) return NULL; }
        if (nd->op == IR_CHOICE || nd->op == IR_CUT ||
            nd->op == IR_CATCH) return NULL;
        if (nd->op == IR_STRUCT) {
            if (g_gz_no_struct_ptr) return NULL;
            int parent_ok = 0;
            for (int j = 0; j < g->n && !parent_ok; j++) {
                IR_t *p = g->all[j];
                if (!p) continue;
                if (p->op == IR_UNIFY && ((p->n_operands > 0 && p->operands[0] == nd) || (p->n_operands > 1 && p->operands[1] == nd))) { parent_ok = 1; break; }
                if (p->op == IR_GOAL) { bb_goal_state_t *zc2 = (bb_goal_state_t *)(intptr_t)IR_LIT(p).ival; if (zc2 && zc2->args) for (int ai = 0; ai < zc2->nargs && !parent_ok; ai++) if (zc2->args[ai] == nd) parent_ok = 1; }
                if (p->op == IR_BUILTIN) { for (int ai = 0; ir_call_arg(p, ai) && !parent_ok; ai++) if (ir_call_arg(p, ai) == nd) parent_ok = 1; }
                if (p->op == IR_BUILTIN && IR_LIT(p).sval && (!strcmp(IR_LIT(p).sval,"findall")||!strcmp(IR_LIT(p).sval,"aggregate_all"))) {
                    bb_findall_state_t *pfs = (bb_findall_state_t *)(intptr_t)IR_LIT(p).ival;
                    if (pfs && (pfs->tmpl == nd || pfs->result == nd)) parent_ok = 1;
                }
                if (p->op == IR_STRUCT) for (int oi = 0; oi < p->n_operands && !parent_ok; oi++) if (p->operands[oi] == nd) parent_ok = 1;
            }
            if (!parent_ok) return NULL;
        }
        if (nd->op == IR_LOGICVAR && ((int)IR_LIT(nd).ival < 0 || (int)IR_LIT(nd).ival >= 64)) return NULL;
        if (nd->op == IR_UNIFY) {
            IR_t *l = (nd->n_operands > 0) ? nd->operands[0] : NULL, *r = (nd->n_operands > 1) ? nd->operands[1] : NULL;
            if (!l || !r) return NULL;
            int ls = (l->op == IR_STRUCT), rs = (r->op == IR_STRUCT);
            if (ls || rs) {
                int lok = ls || l->op == IR_LOGICVAR || l->op == IR_ATOM || l->op == IR_LIT_I || l->op == IR_LIT_F;
                int rok = rs || r->op == IR_LOGICVAR || r->op == IR_ATOM || r->op == IR_LIT_I || r->op == IR_LIT_F;
                if (!lok || !rok) return NULL;
                continue;
            }
            int lv = (l->op == IR_LOGICVAR), rv = (r->op == IR_LOGICVAR);
            int lc = (l->op == IR_ATOM || l->op == IR_LIT_I || l->op == IR_LIT_F), rc = (r->op == IR_ATOM || r->op == IR_LIT_I || r->op == IR_LIT_F);
            if (!((lv && (rv || rc)) || (rv && lc) || (lc && rc))) return NULL;
        }
    }
    IR_t *gconj = NULL;
    IR_t *outer2 = NULL;
    if (softdisj) {
        if (!soft_arm0) return NULL;
        IR_t *outer = NULL;
        for (int k = 0; k < ngconj; k++) {
            bb_conj_state_t *zo = (bb_conj_state_t *)(intptr_t)IR_LIT(gconjs[k]).ival;
            if (!zo || !zo->goals || zo->ngoals < 1 || zo->goals[0] != softdisj) continue;
            outer = gconjs[k];
            if (zo->ngoals > 1) outer2 = gconjs[k];
            break;
        }
        if (!outer) return NULL;
        if (soft_arm0->op == IR_GCONJ) {
            if (ngconj != 2 || soft_arm0 == outer || (soft_arm0 != gconjs[0] && soft_arm0 != gconjs[1])) return NULL;
            gconj = soft_arm0;
        } else if (ngconj != 1) {
            return NULL;
        }
        if (outer2) {
            IR_t *lastA = soft_arm0;
            if (gconj) {
                bb_conj_state_t *za = (bb_conj_state_t *)(intptr_t)IR_LIT(gconj).ival;
                if (!za || !za->goals || za->ngoals < 1) return NULL;
                lastA = za->goals[za->ngoals - 1];
            }
            if (!lastA || lastA->op != IR_FAIL) return NULL;
        }
    } else {
        if (ngconj > 1) return NULL;
        gconj = (ngconj == 1) ? gconjs[0] : NULL;
    }
    IR_t *goals_buf[64]; int ng = 0;
    if (gconj) {
        bb_conj_state_t *zs = (bb_conj_state_t *)(intptr_t)IR_LIT(gconj).ival;
        if (!zs || !zs->goals || zs->ngoals <= 0 || zs->ngoals > 64) return NULL;
        for (int i = 0; i < zs->ngoals; i++) goals_buf[ng++] = zs->goals[i];
    } else if (softdisj) {
        goals_buf[ng++] = soft_arm0;
    } else if (g->entry && g->entry->op == IR_ITE) {
        IR_t *cur = g->entry;
        while (cur && cur->op != IR_SUCCEED && cur->op != IR_FAIL && ng < 64) {
            if (cur->op != IR_GCONJ) goals_buf[ng++] = cur;
            cur = cur->γ.node;
        }
    } else if (!(g->entry && g->entry->op == IR_SUCCEED)) {
        return NULL;
    }
    IR_t *goalsB_buf[64]; int ngB = 0;
    if (outer2) {
        bb_conj_state_t *zo = (bb_conj_state_t *)(intptr_t)IR_LIT(outer2).ival;
        if (!zo || !zo->goals || zo->ngoals < 2 || zo->ngoals > 64) return NULL;
        for (int i = 1; i < zo->ngoals; i++) goalsB_buf[ngB++] = zo->goals[i];
    }
    IR_t *head = NULL, *tail = NULL;
    int nsynth = 0;
    if (!pl_gz_count_synth(goals_buf, ng, &nsynth)) return NULL;
    if (!pl_gz_count_synth(goalsB_buf, ngB, &nsynth)) return NULL;
    int ncells = g->nslots + nsynth;
    int synth_next = g->nslots;
    int cslot = ncells;
    pl_gz_callee_t *callees[8]; int ncallees = 0;
    for (int i = 0; i < ng; i++)
        if (!pl_gz_build_goal(goals_buf[i], &head, &tail, &synth_next, &cslot, callees, &ncallees)) return NULL;
    IR_t *headB = NULL, *tailB = NULL;
    for (int i = 0; i < ngB; i++)
        if (!pl_gz_build_goal(goalsB_buf[i], &headB, &tailB, &synth_next, &cslot, callees, &ncallees)) return NULL;
    IR_t *qf = pl_gz_det_node(IR_QUERY_FRAME);
    if (!qf) return NULL;
    if (head || headB) ir_operand_push(qf, head);
    if (headB) ir_operand_push(qf, headB);
    IR_LIT(qf).ival = ncells;
    IR_LIT(qf).dval = (outer2 && headB) ? 2.0 : (softdisj ? 1.0 : 0.0);
    return qf;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
extern int resolve_bb_pred_count(void);
extern const char *resolve_bb_pred_name_at(int idx);
extern int resolve_bb_pred_arity_at(int idx);
extern IR_t *resolve_bb_entry_node(const char *name, int arity);
extern IR_graph_t *resolve_bb_graph_at(int idx);
static int pl_ite_then_branch_trivial(const IR_t *then_entry) {
    (void)then_entry;
    return 1;
}
static int pl_rich_is_lint_simple(const IR_t *g) {
    const IR_t *lhs = ir_pair_arg(g,0), *rhs = ir_pair_arg(g,1);
    if (!lhs || lhs->op != IR_LIT_I || !rhs || rhs->op != IR_ARITH) return 0;
    const char *rop = IR_LIT(rhs).sval ? IR_LIT(rhs).sval : "+";
    if (pl_arith_op_floaty(rop)) return 0;
    const IR_t *e0 = (rhs->n_operands > 0) ? rhs->operands[0] : NULL, *e1 = (rhs->n_operands > 1) ? rhs->operands[1] : NULL;
    if (!e0 || !e1) return 0;
    if (e0->op == IR_LIT_F || e1->op == IR_LIT_F) return 0;
    return pl_flat_arith_leaf_simple(e0) && pl_flat_arith_leaf_simple(e1);
}
static int pl_findall_term_buildable(const IR_t *a) {
    if (!a) return 0;
    switch (a->op) {
    case IR_ATOM: case IR_LIT_I: case IR_LIT_F: case IR_LOGICVAR:
        return 1;
    case IR_STRUCT: {
        int ar = (int)IR_LIT(a).ival;
        for (int i = 0; i < ar; i++) { const IR_t *c = ir_call_arg(a, i); if (!c || !pl_findall_term_buildable(c)) return 0; }
        return 1;
    }
    case IR_ARITH: {
        int ar = (int)IR_LIT(a).ival;
        const IR_t *f0 = (a->n_operands > 0) ? a->operands[0] : NULL, *f1 = (a->n_operands > 1) ? a->operands[1] : NULL;
        if (ar >= 1 && (!f0 || !pl_findall_term_buildable(f0))) return 0;
        if (ar >= 2 && (!f1 || !pl_findall_term_buildable(f1))) return 0;
        return 1;
    }
    default: return 0;
    }
}
extern int resolve_bb_pred_count(void);
extern const char *resolve_bb_pred_name_at(int idx);
extern int resolve_bb_pred_arity_at(int idx);
static int pl_findall_goal_graph_simple(const IR_graph_t *gg, const IR_t *goal) {
    if (!gg || !gg->all || !goal) return 0;
    int nctl = 0;
    for (int i = 0; i < gg->n; i++) {
        const IR_t *nd = gg->all[i];
        if (!nd) continue;
        switch (nd->op) {
        case IR_GOAL: case IR_GCONJ: case IR_DISJ: case IR_ITE: case IR_CHOICE:
        case IR_CUT: case IR_BUILTIN: case IR_UNIFY: case IR_FAIL: case IR_SUCCEED:
            nctl++;
            if (nd != goal) return 0;
            break;
        default: break;
        }
    }
    return nctl == 1;
}
static int pl_findall_goal_admissible(const IR_t *g) {
    if (!g) return 0;
    if (g->op == IR_FAIL || g->op == IR_SUCCEED) return 1;
    if (g->op == IR_ATOM && IR_LIT(g).sval && (!strcmp(IR_LIT(g).sval,"true")||!strcmp(IR_LIT(g).sval,"fail")||!strcmp(IR_LIT(g).sval,"false"))) return 1;
    if (g->op != IR_GOAL) return 0;
    bb_goal_state_t *zc = (bb_goal_state_t *)(intptr_t)IR_LIT(g).ival;
    const char *gfn = (zc && zc->callee) ? zc->callee : IR_LIT(g).sval;
    if (!gfn) return 0;
    int ar = zc ? zc->arity : 0;
    if (ar > 0 && (!zc || !zc->args || zc->nargs < ar)) return 0;
    for (int i = 0; i < ar; i++) if (!zc->args[i] || !pl_findall_term_buildable(zc->args[i])) return 0;
    int npred = resolve_bb_pred_count();
    int gfn_len = (int)strlen(gfn);
    for (int i = 0; i < npred; i++) {
        const char *nm = resolve_bb_pred_name_at(i);
        if (!nm || resolve_bb_pred_arity_at(i) != ar) continue;
        const char *slash = strrchr(nm, '/');
        int nmlen = slash ? (int)(slash - nm) : (int)strlen(nm);
        if (nmlen == gfn_len && strncmp(nm, gfn, (size_t)nmlen) == 0) return 1;
    }
    return 0;
}
static int pl_findall_conj_member_admissible(const IR_t *g);
static int pl_findall_goal_conj_admissible(const IR_graph_t *gg, const IR_t *goal) {
    if (!gg || !gg->all || !goal) return 0;
    const IR_t *gconj = NULL;
    for (int i = 0; i < gg->n; i++) {
        const IR_t *nd = gg->all[i];
        if (nd && nd->op == IR_GCONJ) { if (gconj) return 0; gconj = nd; }
    }
    if (!gconj) return 0;
    bb_conj_state_t *zs = (bb_conj_state_t *)(intptr_t)IR_LIT(gconj).ival;
    if (!zs || !zs->goals || zs->ngoals < 1) return 0;
    for (int i = 0; i < gg->n; i++) {
        const IR_t *nd = gg->all[i];
        if (!nd) continue;
        switch (nd->op) {
        case IR_GOAL: case IR_GCONJ: case IR_DISJ: case IR_ITE: case IR_CHOICE:
        case IR_CUT: case IR_BUILTIN: case IR_UNIFY: case IR_FAIL: case IR_SUCCEED: {
            if (nd == gconj) break;
            int member = 0;
            for (int j = 0; j < zs->ngoals; j++) if (zs->goals[j] == nd) { member = 1; break; }
            if (!member) return 0;
            break;
        }
        default: break;
        }
    }
    for (int j = 0; j < zs->ngoals; j++) if (!pl_findall_conj_member_admissible(zs->goals[j])) return 0;
    return 1;
}
static int pl_findall_conj_member_admissible(const IR_t *g) {
    if (!g) return 0;
    if (g->op == IR_GOAL) return pl_findall_goal_admissible(g);
    if (g->op == IR_BUILTIN && IR_LIT(g).sval) {
        static const char *mset[] = { "is", "=:=", "=\\=", "<", ">", "=<", ">=", NULL };
        for (int k = 0; mset[k]; k++)
            if (!strcmp(IR_LIT(g).sval, mset[k])) return ir_pair_arg(g,0) && ir_pair_arg(g,1) && pl_findall_term_buildable(ir_pair_arg(g,0)) && pl_findall_term_buildable(ir_pair_arg(g,1));
    }
    return 0;
}
/*====================================================================================================================*/
int main(int argc, char **argv)
{
    if (argc >= 3 && strcmp(argv[1], "--audit-per-kind") == 0) {
        fprintf(stderr, "scrip: --audit-per-kind unavailable (audit tool unlinked)\n");
        return 1;
    }
    int mode_interp        = 0;
    int mode_run           = 0;
    int mode_compile       = 0;
    int mode_monitor       = 0;
    int dump_ast           = 0;
    int dump_ast_bison     = 0;
    int dump_sm            = 0;
    int dump_bb            = 0;
    int dump_bb2           = 0;
    int dump_sno           = 0;
    int opt_trace          = 0;
    int opt_bench          = 0;
    const char * target_name = NULL;
    int argi = 1;
    while (argi < argc && argv[argi][0] == '-' && argv[argi][1] == '-') {
        if      (strcmp(argv[argi], "--interp")        == 0) { mode_interp        = 1; argi++; }
        else if (strcmp(argv[argi], "--run")           == 0) { mode_run           = 1; argi++; }
        else if (strcmp(argv[argi], "--compile")       == 0) { mode_compile       = 1; if (!target_name) target_name = "x86"; argi++; }
        else if (strcmp(argv[argi], "--monitor")       == 0) { mode_monitor       = 1; argi++; }
        else if (strncmp(argv[argi], "--target=", 9)   == 0) { target_name = argv[argi] + 9; mode_compile = 1; argi++; }
        else if (strcmp(argv[argi], "--dump-ast")      == 0) { dump_ast           = 1; argi++; }
        else if (strcmp(argv[argi], "--dump-ast-bison") == 0) { dump_ast_bison    = 1; argi++; }
        else if (strcmp(argv[argi], "--dump-sm")       == 0) { dump_sm            = 1; argi++; }
        else if (strcmp(argv[argi], "--dump-bb")       == 0) { dump_bb            = 1; argi++; }
        else if (strcmp(argv[argi], "--dump-bb2")      == 0) { dump_bb2           = 1; argi++; }
        else if (strcmp(argv[argi], "--dump-sno")      == 0) { dump_sno           = 1; argi++; }
        else if (strcmp(argv[argi], "--dump-width")    == 0) {
            if (argi + 1 < argc) { ir_set_print_width(atoi(argv[++argi])); argi++; }
        }
        else if (strcmp(argv[argi], "--trace")         == 0) { opt_trace          = 1; argi++; }
        else if (strcmp(argv[argi], "--bench")         == 0) { opt_bench          = 1; argi++; }
        else if (strncmp(argv[argi], "--icn-globals=", 14) == 0) {
            extern int g_icn_globals_nv;
            const char * v = argv[argi] + 14;
            if      (strcmp(v, "nv")   == 0) g_icn_globals_nv = 1;
            else if (strcmp(v, "slot") == 0) g_icn_globals_nv = 0;
            else { fprintf(stderr, "scrip: --icn-globals= must be slot or nv\n"); return 1; }
            argi++;
        }
        else if (strcmp(argv[argi], "--case-sensitive") == 0) { argi++; }
        else if (strcmp(argv[argi], "--fold-case")     == 0) {
            fprintf(stderr, "scrip: --fold-case is no longer supported; SCRIP is case-sensitive only\n");
            return 1;
        }
        else break;
    }
    int mode_compile_x86 = (mode_compile && target_name && strcmp(target_name, "x86") == 0);
    if (mode_compile_x86 && (mode_interp || mode_run || mode_monitor)) {
        fprintf(stderr,
            "scrip: --compile (x86) is mutually exclusive with "
            "--interp / --run / --monitor\n");
        return 1;
    }
    if (!mode_interp && !mode_run && !mode_monitor && !mode_compile)
        mode_run = 1;
    if (argi >= argc) {
        fprintf(stderr,
            "usage: scrip [mode] [options] <file> [-- program-args...]\n"
            "\n"
            "Execution modes (default: --run):\n"
            "  --interp         walk the BB port-graph in-process (Icon)\n"
            "  --run            build flat-wired x86 BB blobs in a sealed slab and jump in  [DEFAULT]\n"
            "  --compile        emit standalone x86-64 asm to stdout (links libscrip_rt.so)\n"
            "  --target=ARCH    emit code for the named backend (x86, jvm, js, wasm); implies --compile\n"
            "  --monitor        in-process sync comparator (unavailable)\n"
            "\n"
            "Diagnostic options:\n"
            "  --dump-ast       print AST after frontend\n"
            "  --dump-bb        print BB-GRAPH for each proc\n"
            "  --dump-sno       transpile AST to portable SNOBOL4 source (GOAL-PARSER-SC-TRANSPILE.md SCT-1)\n"
            "  --trace          MONITOR trace output (diff vs CSNOBOL4)\n"
            "  --bench          print wall-clock time after execution\n"
            "  --dump-ast-bison dump AST via old Bison/Flex parser\n"
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
    CODE_t *sub = NULL;
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
        sub = NULL;
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
        } else if (dump_sno) {
            FILE *f = fopen(input_path, "r");
            if (!f) { fprintf(stderr, "scrip: cannot open '%s'\n", input_path); return 1; }
            if (opt_bench) clock_gettime(CLOCK_MONOTONIC, &_t1);
            tree_t *sub_ast = sno_parse_ast(f, input_path, NULL);
            fclose(f);
            MERGE_AST(sub_ast);
        } else {
            FILE *f = fopen(input_path, "r");
            if (!f) { fprintf(stderr, "scrip: cannot open '%s'\n", input_path); return 1; }
            tree_t *sub_ast = sno_parse_ast(f, input_path, dump_ast_bison ? &sub : NULL);
            fclose(f);
            if (dump_ast_bison) { ir_dump_program(sub, stdout); return 0; }
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
    g_opt_trace   = opt_trace;
    g_opt_dump_bb = dump_bb;
    if (dump_sm) {
        fprintf(stderr, "scrip: --dump-sm removed (Stack Machine excised). Use --dump-bb.\n");
        return 1;
    }
    if (dump_sno) {
        extern int tree_to_sno(const tree_t *ast, FILE *out);
        tree_to_sno(ast_prog, stdout);
        return 0;
    }
    if (dump_bb) {
        extern void bb_print(const IR_graph_t * bbg, FILE * fp);
        extern int g_icn_postfix_resume;
        if (is_icon) g_icn_postfix_resume = 1;
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
            bb_print(s2->bbp.table[idx], stdout);
        }
        free(seen_all);
        return 0;
    }
    if (dump_bb2) {
        extern void bb_print(const IR_graph_t * bbg, FILE * fp);
        extern IR_graph_t * lower_icon(const tree_t *);
        extern IR_graph_t * lower_snobol4(const tree_t *);
        extern IR_graph_t * lower_raku(const tree_t *);
        extern IR_graph_t * lower_pascal(const tree_t *);
        extern IR_graph_t * lower_prolog(const tree_t *);
        if (is_icon) {
            extern int lower_icon_enum(const tree_t *, const tree_t **, int);
            extern IR_graph_t * lower_icon_proc(const tree_t *, const tree_t *);
            const tree_t * procs[256];
            int np = lower_icon_enum(ast_prog, procs, 256);
            if (np <= 0) { IR_graph_t * g = lower_icon(ast_prog); if (g) bb_print(g, stdout); return 0; }
            for (int _pi = 0; _pi < np; _pi++) {
                const char * nm = procs[_pi]->v.sval ? procs[_pi]->v.sval : "?";
                IR_graph_t * g = lower_icon_proc(ast_prog, procs[_pi]);
                if (!g) continue;
                fprintf(stdout, "; proc %s\n", nm);
                bb_print(g, stdout);
            }
            return 0;
        }
        if (is_pascal) {
            extern int lower_pascal_enum(const tree_t *, const tree_t **, int);
            extern IR_graph_t * lower_pascal_proc(const tree_t *, const tree_t *);
            const tree_t * procs[256];
            int np = lower_pascal_enum(ast_prog, procs, 256);
            if (np <= 0) { IR_graph_t * g = lower_pascal(ast_prog); if (g) bb_print(g, stdout); return 0; }
            for (int _pi = 0; _pi < np; _pi++) {
                const char * nm = procs[_pi]->v.sval ? procs[_pi]->v.sval : "?";
                IR_graph_t * g = lower_pascal_proc(ast_prog, procs[_pi]);
                if (!g) continue;
                fprintf(stdout, "; proc %s\n", nm);
                bb_print(g, stdout);
            }
            return 0;
        }
        if (is_raku) {
            extern int lower_raku_enum(const tree_t *, const tree_t **, int);
            extern IR_graph_t * lower_raku_proc(const tree_t *, const tree_t *);
            const tree_t * procs[256];
            int np = lower_raku_enum(ast_prog, procs, 256);
            if (np <= 0) { IR_graph_t * g = lower_raku(ast_prog); if (g) bb_print(g, stdout); return 0; }
            for (int _pi = 0; _pi < np; _pi++) {
                const char * nm = (procs[_pi]->n > 0 && procs[_pi]->c[0] && procs[_pi]->c[0]->v.sval) ? procs[_pi]->c[0]->v.sval : "?";
                IR_graph_t * g = lower_raku_proc(ast_prog, procs[_pi]);
                if (!g) continue;
                fprintf(stdout, "; proc %s\n", nm);
                bb_print(g, stdout);
            }
            return 0;
        }
        IR_graph_t * g = is_raku ? lower_raku(ast_prog) : is_prolog ? lower_prolog(ast_prog) : is_pascal ? lower_pascal(ast_prog) : lower_snobol4(ast_prog);
        if (!g) { fprintf(stderr, "scrip: --dump-bb2 lowering returned NULL\n"); return 1; }
        fprintf(stdout, "; proc main\n");
        bb_print(g, stdout);
        return 0;
    }
    if (mode_compile_x86) {
        extern int codegen_flat_build(IR_t * nd, FILE * out, const char * prefix);
        extern int g_frame_active;
        if (is_icon || is_raku) {
            extern int g_icn_postfix_resume;
            if (is_icon) g_icn_postfix_resume = 1;
            stage2_t *s2 = sm_preamble(ast_prog);
            if (!s2) return 1;
            ast_tree_free(ast_prog); ast_prog = NULL;
            if ((is_icon || is_raku) && !icn_graph_native_emittable(s2)) {
                fprintf(stderr, "[SMX] --compile --target=x86: mode-4 native emitter does not yet cover "
                                "this program (a box has no MEDIUM_TEXT arm — Icon scan/keyword/cset/gen-alt/"
                                "suspend, or Raku map/grep). EXCISED — mode-2 (--interp) is the oracle for this rung.\n");
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
            }
            if (main_bb_idx < 0 || main_bb_idx >= s2->bbp.count || !s2->bbp.table[main_bb_idx] || !s2->bbp.table[main_bb_idx]->entry) {
                fprintf(stderr, "[IBB] FATAL: mode-4 driver: main BB graph not found\n");
                return 1;
            }
            IR_graph_t * bbg = s2->bbp.table[main_bb_idx];
            extern int descr_flat_chain_build_text(IR_t * entry, FILE * out, const char * prefix);
            extern int descr_flat_chain_build_proc_text(IR_t *entry, const char **pnames, int np, FILE *out, const char *pname);
            IR_t *icn_root = icn_ring_to_tree(bbg);
            int use_chain = (icn_root == NULL);
            printf("  .intel_syntax noprefix\n");
            printf("  .text\n");
            g_frame_active = 1;
            int n_procs = 0;
            static char proc_names_buf[64][128];
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
                descr_flat_chain_build_proc_text(s2->bbp.table[idx]->entry, pn, np, stdout, pname);
                if (n_procs < 64) snprintf(proc_names_buf[n_procs++], 128, "%s", pname);
                free(pn);
            }
            if (n_procs > 0) {
                printf("icn_proc_startup:\n");
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
                for (int i = 0; i < n_procs; i++) {
                    printf("  .section .rodata\n");
                    printf("  .Lstartup_pname%d: .string \"%s\"\n", i, proc_names_buf[i]);
                    printf("  .section .text\n");
                    printf("  .intel_syntax noprefix\n");
                    printf("  lea rdi, [rip + .Lstartup_pname%d]\n", i);
                    printf("  lea rsi, [rip + icn_proc_%s_\xce\xb1]\n", proc_names_buf[i]);
                    printf("  call rt_proc_set_fn@PLT\n");
                }
                printf("  pop rbp\n");
                printf("  ret\n");
            }
            printf("  .globl main\n");
            printf("main:\n");
            printf("  push rbp\n");
            printf("  mov rbp, rsp\n");
            if (n_procs > 0)
                printf("  call icn_proc_startup\n");
            printf("  call rt_frame@PLT\n");
            printf("  mov rdi, rax\n");
            printf("  xor esi, esi\n");
            printf("  call main_\xce\xb1\n");
            printf("  xor eax, eax\n");
            printf("  pop rbp\n");
            printf("  ret\n");
            int rc;
            {
                extern int g_descr_flat_chain;
                int saved = g_descr_flat_chain; g_descr_flat_chain = 1;
                { extern IR_graph_t *g_emit_cfg; g_emit_cfg = bbg; }
                rc = use_chain ? descr_flat_chain_build_text(bbg->entry, stdout, "main")
                               : codegen_flat_build(icn_root, stdout, "main");
                g_descr_flat_chain = saved;
            }
            g_frame_active = 0;
            extern void xa_emit_strtab_rodata(void);
            xa_emit_strtab_rodata();
            fflush(stdout);
            ir_delete_all(s2);
            return rc;
        }
        if (is_prolog) {
            extern int codegen_flat_build(IR_t * nd, FILE * out, const char * prefix);
            extern int codegen_clause_dispatch(FILE * out);
            extern int g_frame_active;
            extern void xa_emit_strtab_rodata(void);
            stage2_t *s2 = sm_preamble(ast_prog);
            if (!s2) { fprintf(stderr, "[PBB] mode-4: sm_preamble failed\n"); return 1; }
            ast_tree_free(ast_prog); ast_prog = NULL;
            int main_bb_idx = -1;
            for (int _pi = 0; _pi < s2->proc_count; _pi++)
                if (s2->proc_table[_pi].name && strcmp(s2->proc_table[_pi].name, "main") == 0) { main_bb_idx = s2->proc_table[_pi].bb_idx; break; }
            if (main_bb_idx < 0 || main_bb_idx >= s2->bbp.count || !s2->bbp.table[main_bb_idx]) {
                fprintf(stderr, "[PBB] FATAL: mode-4 driver: Prolog main BB graph not found "
                                "(no initialization goal lowered, or predicate unhandled)\n");
                return 1;
            }
            IR_graph_t *pl_main = s2->bbp.table[main_bb_idx];
            g_gz_no_struct_ptr = 0;
            IR_t *gz_root = pl_gz_admit(pl_main);
            g_gz_no_struct_ptr = 0;
            if (gz_root) {
                extern int pl_gz_codegen(IR_t * nd, FILE * out, const char * prefix);
                printf("  .intel_syntax noprefix\n");
                printf("  .text\n");
                printf("  .globl main\n");
                printf("main:\n");
                printf("  push rbp\n");
                printf("  mov rbp, rsp\n");
                printf("  call rt_frame@PLT\n");
                printf("  mov rdi, rax\n");
                printf("  xor esi, esi\n");
                printf("  call main_\xce\xb1\n");
                printf("  xor eax, eax\n");
                printf("  pop rbp\n");
                printf("  ret\n");
                int rc = pl_gz_codegen(gz_root, stdout, "main");
                extern void xa_emit_strtab_rodata(void);
                xa_emit_strtab_rodata();
                fflush(stdout);
                ir_delete_all(s2);
                return rc;
            }
            fprintf(stderr, "[PL-GZ FENCE] --compile --target=x86: program not admitted by pl_gz_admit — the GZ cell path is the only Prolog backend (flat + rich/heap-env tiers DELETED per PL-GZ GUT, 2026-06-13).\n");
            return 1;
        }
        {
            extern int gvar_flat_chain_build_text(IR_graph_t * g, FILE * out, const char * prefix);
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
            extern int g_sno_m4_dense_nid;
            g_flat_node_id = 0;
            g_sno_m4_dense_nid = 1;
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
            static int sno_pidx_buf[64];
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
                gvar_flat_chain_build_text(s2->bbp.table[idx], stdout, pname);
                { extern int g_emit_frame_caller_dl; g_emit_frame_caller_dl = -1; }
                if (n_procs < 64) sno_pidx_buf[n_procs++] = _pi;
            }
            if (n_procs > 0) {
                printf("  .section .rodata\n");
                for (int i = 0; i < n_procs; i++) {
                    ProcEntry *pe = &s2->proc_table[sno_pidx_buf[i]];
                    printf("  .Lsno_pn%d: .string \"%s\"\n", i, pe->name);
                    for (int k = 0; k < pe->nparams && k < pe->lower_sc.n; k++)
                        printf("  .Lsno_pp%d_%d: .string \"%s\"\n", i, k, pe->lower_sc.e[k].name ? pe->lower_sc.e[k].name : "");
                    printf("  .Lsno_pnames%d:\n", i);
                    for (int k = 0; k < pe->nparams && k < pe->lower_sc.n; k++) printf("  .quad .Lsno_pp%d_%d\n", i, k);
                    printf("  .quad 0\n");
                }
                printf("  .section .text\n  .intel_syntax noprefix\n");
                printf("sno_proc_startup:\n  push rbp\n  mov rbp, rsp\n  call core_lib_init@PLT\n  call rt_proc_reset@PLT\n");
                for (int i = 0; i < n_procs; i++) {
                    ProcEntry *pe = &s2->proc_table[sno_pidx_buf[i]];
                    printf("  lea rdi, [rip + .Lsno_pn%d]\n", i);
                    printf("  lea rsi, [rip + .Lsno_pnames%d]\n", i);
                    printf("  mov edx, %d\n", pe->nparams);
                    printf("  call rt_proc_register@PLT\n");
                    printf("  lea rdi, [rip + .Lsno_pn%d]\n", i);
                    printf("  lea rsi, [rip + %s_\xce\xb1]\n", pe->name);
                    printf("  call rt_proc_set_fn@PLT\n");
                    int _fidx = pe->bb_idx;
                    if (_fidx >= 0 && _fidx < s2->bbp.count && s2->bbp.table[_fidx] && s2->bbp.table[_fidx]->nslots > 0) {
                        printf("  lea rdi, [rip + .Lsno_pn%d]\n", i);
                        printf("  mov esi, %d\n", s2->bbp.table[_fidx]->nslots - 1);
                        printf("  mov edx, %d\n", pe->decl_level);
                        printf("  call rt_proc_set_frame@PLT\n");
                    }
                }
                printf("  pop rbp\n  ret\n");
            }
            printf("  .globl main\nmain:\n  push rbp\n  mov rbp, rsp\n");
            if (n_procs > 0) printf("  call sno_proc_startup\n");
            else printf("  call core_lib_init@PLT\n  call rt_proc_reset@PLT\n");
            printf("  call rt_frame@PLT\n  mov rdi, rax\n  xor esi, esi\n");
            printf("  call sno_flat_\xce\xb1\n");
            printf("  xor eax, eax\n  pop rbp\n  ret\n");
            int rc = gvar_flat_chain_build_text(sbbg, stdout, "sno_flat");
            g_frame_active = 0;
            xa_emit_strtab_rodata();
            fflush(stdout);
            ir_delete_all(s2);
            return rc;
        }
    }
    if (mode_compile && target_name && strcmp(target_name, "x86") != 0) {
        fprintf(stderr, "[SMX] --target=%s removed (Stack-Machine codegen excised).\n",
                target_name ? target_name : "?");
        ast_tree_free(ast_prog); ast_prog = NULL;
        return 1;
    }
    if (mode_monitor) {
        fprintf(stderr, "[NO-SM-BB] --monitor: trampoline codegen deleted (FACT RULE); unavailable\n");
        ast_tree_free(ast_prog); ast_prog = NULL;
        return 1;
    } else if (mode_interp) {
        extern int g_icn_postfix_resume;
        if (is_icon) g_icn_postfix_resume = 1;
        stage2_t *s2 = sm_preamble(ast_prog);
        if (!s2) return 1;
        ast_tree_free(ast_prog); ast_prog = NULL;
        if (is_icon) {
            extern DESCR_t IR_interp_once(IR_graph_t * bbg);
            int main_bb_idx = -1;
            for (int _pi = 0; _pi < s2->proc_count; _pi++) {
                if (s2->proc_table[_pi].name && strcmp(s2->proc_table[_pi].name, "main") == 0) {
                    main_bb_idx = s2->proc_table[_pi].bb_idx;
                    break;
                }
            }
            if (main_bb_idx < 0 || main_bb_idx >= s2->bbp.count || !s2->bbp.table[main_bb_idx]) {
                fprintf(stderr, "[IBB] FATAL: mode-2 driver: main BB graph not found\n");
                abort();
            }
            (void)IR_interp_once(s2->bbp.table[main_bb_idx]);
            goto run_done;
        }
        if (!is_icon && !is_prolog) {
            extern DESCR_t IR_interp_once(IR_graph_t * bbg);
            int main_bb_idx = -1;
            for (int _pi = 0; _pi < s2->proc_count; _pi++) {
                if (s2->proc_table[_pi].name && strcmp(s2->proc_table[_pi].name, "main") == 0) {
                    main_bb_idx = s2->proc_table[_pi].bb_idx;
                    break;
                }
            }
            if (main_bb_idx < 0 || main_bb_idx >= s2->bbp.count || !s2->bbp.table[main_bb_idx]) {
                fprintf(stderr, "[SBB] FATAL: mode-2 driver: SNOBOL4 main BB graph not found\n");
                abort();
            }
            (void)IR_interp_once(s2->bbp.table[main_bb_idx]);
            goto run_done;
        }
        if (is_prolog) {
            extern DESCR_t IR_interp_once(IR_graph_t * bbg);
            extern Term **g_resolve_env;
            int main_bb_idx = -1;
            for (int _pi = 0; _pi < s2->proc_count; _pi++) {
                if (s2->proc_table[_pi].name && strcmp(s2->proc_table[_pi].name, "main") == 0) {
                    main_bb_idx = s2->proc_table[_pi].bb_idx;
                    break;
                }
            }
            if (main_bb_idx < 0 || main_bb_idx >= s2->bbp.count || !s2->bbp.table[main_bb_idx]) {
                fprintf(stderr, "[PBB] FATAL: mode-2 driver: Prolog main BB graph not found "
                                "(no initialization goal lowered, or predicate unhandled by PLG-1)\n");
                abort();
            }
            IR_graph_t *pl_main = s2->bbp.table[main_bb_idx];
            int nslots = pl_main->nslots > 0 ? pl_main->nslots : 1;
            g_resolve_env = (Term **)GC_MALLOC((size_t)(nslots + 8) * sizeof(Term *));
            (void)IR_interp_once(pl_main);
            goto run_done;
        }
        fprintf(stderr, "[SMX] FATAL: Stack Machine excised. Non-Icon mode-2 (--interp) "
                        "execution is gone. This language has not yet crossed onto Byrd Boxes. "
                        "Aborting (by design).\n");
        abort();
    } else if (mode_run) {
        extern int g_icn_postfix_resume;
        if (is_icon) g_icn_postfix_resume = 1;
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
            }
            if ((is_icon || is_raku) && !icn_graph_native_emittable_mode(s2, 1)) {
                fprintf(stderr, "[SMX] --run: mode-3 native emitter does not yet cover this program "
                                "(a box has no MEDIUM_BINARY arm — Icon scan/keyword/cset/gen-alt/suspend, "
                                "or Raku map/grep). EXCISED — mode-2 (--interp) is the oracle for this rung.\n");
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
                bb_box_fn pfn = descr_flat_chain_build_proc(s2->bbp.table[idx]->entry, pn, np);
                if (pfn) rt_proc_set_fn(pname, pfn);
            }
            {
                extern void *dat_register(const char *spec);
                for (int _gi = 0; _gi < s2->bbp.count; _gi++) {
                    IR_graph_t *g = s2->bbp.table[_gi];
                    if (!g || !g->all) continue;
                    for (int _ni = 0; _ni < g->n; _ni++) {
                        IR_t *nd = g->all[_ni];
                        if (nd && nd->op == IR_RECORD_DEF && IR_LIT(nd).sval) dat_register(IR_LIT(nd).sval);
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
            IR_t *icn_root = icn_ring_to_tree(bbg);
            bb_box_fn fn;
            { extern IR_graph_t *g_emit_cfg; g_emit_cfg = bbg; }
            if (icn_root) {
                extern int g_descr_flat_chain;
                int saved = g_descr_flat_chain; g_descr_flat_chain = 1;
                fn = bb_build_flat(icn_root);
                g_descr_flat_chain = saved;
            } else {
                fn = descr_flat_chain_build(bbg->entry);
            }
            g_frame_active = 0;
            if (!fn) {
                fprintf(stderr, "[IBB] FATAL: mode-3 driver: bb_build_flat returned NULL — BB template(s) lack MEDIUM_BINARY arm\n");
                abort();
            }
            ir_delete_all(s2);
            (void)fn(rt_frame(), 0);
            goto run_done;
        }
        if (is_prolog) {
            extern DESCR_t IR_interp_once(IR_graph_t * bbg);
            extern Term **g_resolve_env;
            extern void *rt_frame(void);
            extern int g_frame_active;
            int main_bb_idx = -1;
            for (int _pi = 0; _pi < s2->proc_count; _pi++) {
                if (s2->proc_table[_pi].name && strcmp(s2->proc_table[_pi].name, "main") == 0) {
                    main_bb_idx = s2->proc_table[_pi].bb_idx;
                    break;
                }
            }
            if (main_bb_idx < 0 || main_bb_idx >= s2->bbp.count || !s2->bbp.table[main_bb_idx]) {
                fprintf(stderr, "[PBB] FATAL: mode-3 driver: Prolog main BB graph not found "
                                "(no initialization goal lowered, or predicate unhandled by PLG-1)\n");
                abort();
            }
            IR_graph_t *pl_main = s2->bbp.table[main_bb_idx];
            IR_t *gz_root = pl_gz_admit(pl_main);
            if (gz_root) {
                extern bb_box_fn pl_gz_build(IR_t * nd);
                bb_box_fn gzfn = pl_gz_build(gz_root);
                if (gzfn) { ir_delete_all(s2); (void)gzfn(rt_frame(), 0); goto run_done; }
            }
            fprintf(stderr, "[PL-GZ FENCE] --run: program not admitted by pl_gz_admit — the GZ cell path is "
                            "the only Prolog execution path (flat + rich/heap-env tiers DELETED per PL-GZ GUT, "
                            "2026-06-13). Aborting.\n");
            abort();
        }
        {
            if (is_prolog) {
                fprintf(stderr, "[SMX] FATAL: Stack Machine excised. Prolog mode-3 (--run) "
                                "native execution is gone. This language has not yet crossed "
                                "onto Byrd Boxes. Aborting (by design).\n");
                (void)s2;
                abort();
            }
            extern bb_box_fn gvar_flat_chain_build(IR_graph_t * g);
            extern void *rt_frame(void);
            extern int g_frame_active;
            extern void rt_proc_register(const char *name, const char **pnames, int nparams);
            extern void rt_proc_set_fn(const char *name, bb_box_fn fn);
            extern void rt_proc_reset(void);
            int main_bb_idx = -1;
            for (int _pi = 0; _pi < s2->proc_count; _pi++)
                if (s2->proc_table[_pi].name && strcmp(s2->proc_table[_pi].name, "main") == 0) { main_bb_idx = s2->proc_table[_pi].bb_idx; break; }
            rt_proc_reset();
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
                bb_box_fn pfn = gvar_flat_chain_build(s2->bbp.table[idx]);
                { extern int g_emit_frame_caller_dl; g_emit_frame_caller_dl = -1; }
                if (pfn) rt_proc_set_fn(pname, pfn);
            }
            g_frame_active = 0;
            IR_graph_t *sbbg = (main_bb_idx >= 0 && main_bb_idx < s2->bbp.count) ? s2->bbp.table[main_bb_idx] : NULL;
            if (sbbg && sbbg->entry) {
                g_frame_active = 1;
                bb_box_fn fn = gvar_flat_chain_build(sbbg);
                g_frame_active = 0;
                ir_delete_all(s2);
                if (fn) { (void)fn(rt_frame(), 0); goto run_done; }
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
