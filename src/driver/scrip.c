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
    switch (n->t) {
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
    for (IR_t *cur = g->entry; cur && cur->t != IR_SUCCEED && cur->t != IR_FAIL && nc < 256; cur = cur->γ) chain[nc++] = cur;
    if (nc == 0 || nc >= 256) return NULL;
    for (int i = 0; i < nc; i++) if (chain[i]->t == IR_BINOP || chain[i]->t == IR_LIT_I || chain[i]->t == IR_LIT_S || chain[i]->t == IR_LIT_F || chain[i]->t == IR_LIT_NUL) return NULL;
    IR_t *stk[256]; int sp = 0;
    for (int i = 0; i < nc; i++) {
        IR_t *n = chain[i];
        int ar = icn_rt_arity(n);
        if (ar < 0 || ar > sp) return NULL;
        if (n->t == IR_CALL) {
            if (ar != 1) return NULL;
            n->n_operands = 0; if (!ir_operand_push(n, stk[sp - 1])) return NULL; sp -= 1;
            IR_LIT(n).dval = 0.0;
        } else if (ar == 2) {
            n->β = stk[sp - 1];
            n->α = stk[sp - 2];
            sp -= 2;
        } else if (ar == 1) {
            n->α = stk[sp - 1]; sp -= 1;
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
        if (arms[i]->t != IR_LIT_I && arms[i]->t != IR_LIT_S) return 0;
    }
    return 1;
}
static int icn_alt_safe_kind(IR_e t) {
    return t == IR_ALT || t == IR_CALL || t == IR_EVERY || t == IR_FAIL ||
           t == IR_SUCCEED || t == IR_LIT_I || t == IR_LIT_S || t == IR_LIT_F || t == IR_LIT_NUL;
}
static int icn_graph_has_alt(const IR_graph_t *g) {
    for (int ni = 0; ni < g->n; ni++) if (g->all[ni] && g->all[ni]->t == IR_ALT) return 1;
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
    if (!ae || ae->t != want) return (IR_t *)0;
    if (ae->γ && ae->γ->t != IR_SUCCEED) return (IR_t *)0;
    return ae;
}
static int icn_scan_fn_lit_arg(IR_t *nd, IR_e want) {
    return icn_scan_lit_entry(nd, want) != (IR_t *)0;
}
static int icn_scan_tab_arg_ok(IR_t *nd) {
    IR_graph_t **sblks = (IR_graph_t **)(intptr_t) IR_EXEC(nd).counter;
    IR_t *ae = (sblks && (int)IR_LIT(nd).ival == 1 && sblks[0]) ? sblks[0]->entry : (IR_t *)0;
    if (!ae) return 0;
    if (ae->γ && ae->γ->t != IR_SUCCEED) return 0;
    if (ae->t == IR_LIT_I && IR_LIT(ae).ival >= 1) return 1;
    if (ae->t == IR_CALL && IR_LIT(ae).dval == 3.0 && IR_LIT(ae).sval && (!strcmp(IR_LIT(ae).sval, "any") || !strcmp(IR_LIT(ae).sval, "match") || !strcmp(IR_LIT(ae).sval, "many") || !strcmp(IR_LIT(ae).sval, "upto") || !strcmp(IR_LIT(ae).sval, "find") || !strcmp(IR_LIT(ae).sval, "bal")) && icn_scan_fn_lit_arg(ae, IR_LIT_S)) return 1;
    return 0;
}
static int icn_graph_var_assigned_or_param(stage2_t *s2, int gi, IR_graph_t *g, const char *name);
static int icn_scan_subgraph_safe(stage2_t *s2, int gi, IR_graph_t *g, IR_graph_t *sg, int depth) {
    if (!sg || !sg->all || sg->n <= 0 || depth > 16) return 0;
    for (int i = 0; i < sg->n; i++) {
        IR_t *nd = sg->all[i];
        if (!nd) continue;
        if (!icn_scan_safe_kind(nd->t)) return 0;
        if (nd->t == IR_VAR) {
            if (IR_LIT(nd).sval && IR_LIT(nd).sval[0] == '&') { if (!icn_keyword_supported(IR_LIT(nd).sval)) return 0; }
            else if (IR_EXEC(nd).state == 1 || !IR_LIT(nd).sval || !icn_graph_var_assigned_or_param(s2, gi, g, IR_LIT(nd).sval)) return 0;
        }
        if (nd->t == IR_KEYWORD && !icn_keyword_supported(IR_LIT(nd).sval)) return 0;
        if (nd->t == IR_CALL) {
            if (!IR_LIT(nd).sval) return 0;
            if (!strcmp(IR_LIT(nd).sval, "any") || !strcmp(IR_LIT(nd).sval, "match") || !strcmp(IR_LIT(nd).sval, "many") || !strcmp(IR_LIT(nd).sval, "upto")) { if (!(IR_LIT(nd).dval == 3.0 && icn_scan_fn_lit_arg(nd, IR_LIT_S))) return 0; }
            else if (!strcmp(IR_LIT(nd).sval, "tab")) { if (!(IR_LIT(nd).dval == 3.0 && icn_scan_tab_arg_ok(nd))) return 0; }
            else if (!strcmp(IR_LIT(nd).sval, "move")) { if (!(IR_LIT(nd).dval == 3.0 && icn_scan_fn_lit_arg(nd, IR_LIT_I))) return 0; }
            else if (!strcmp(IR_LIT(nd).sval, "pos")) { IR_t *pe = icn_scan_lit_entry(nd, IR_LIT_I); if (!(IR_LIT(nd).dval == 3.0 && pe && IR_LIT(pe).ival >= 1)) return 0; }
            else if (!strcmp(IR_LIT(nd).sval, "find")) { IR_t *fe = icn_scan_lit_entry(nd, IR_LIT_S); if (!(IR_LIT(nd).dval == 3.0 && fe && IR_LIT(fe).sval && IR_LIT(fe).sval[0] && strlen(IR_LIT(fe).sval) <= 32)) return 0; }
            else if (!strcmp(IR_LIT(nd).sval, "bal")) { IR_t *be = icn_scan_lit_entry(nd, IR_LIT_S); if (!(IR_LIT(nd).dval == 3.0 && be && IR_LIT(be).sval && IR_LIT(be).sval[0] && !strchr(IR_LIT(be).sval, 40) && !strchr(IR_LIT(be).sval, 41))) return 0; }
            else if (!(!strcmp(IR_LIT(nd).sval, "write") || !strcmp(IR_LIT(nd).sval, "writes"))) return 0;
        }
        if (nd->t == IR_BINOP && IR_LIT(nd).ival != BINOP_CONCAT) return 0;
        if (nd->t == IR_GEN_SCAN) {
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
    if (!r || r->t != IR_GEN_SCAN || IR_LIT(r).dval != 1.0) return 0;
    IR_graph_t *bsg = (IR_graph_t *)(intptr_t) IR_LIT(r).ival;
    IR_t *bt = bsg ? bsg->entry : (IR_t *)0;
    int gd = 0;
    while (bt && bt->γ && bt->γ->t != IR_SUCCEED && bt->γ->t != IR_FAIL && gd++ < 512) bt = bt->γ;
    if (bt && (bt->t == IR_LIT_I || bt->t == IR_LIT_S)) return 1;
    if (bt && bt->t == IR_VAR && IR_LIT(bt).sval && IR_LIT(bt).sval[0] != '&') return 1;
    if (bt && bt->t == IR_CALL && IR_LIT(bt).dval == 3.0 && IR_LIT(bt).sval && (!strcmp(IR_LIT(bt).sval, "tab") || !strcmp(IR_LIT(bt).sval, "move") || !strcmp(IR_LIT(bt).sval, "pos") || !strcmp(IR_LIT(bt).sval, "any") || !strcmp(IR_LIT(bt).sval, "match") || !strcmp(IR_LIT(bt).sval, "many") || !strcmp(IR_LIT(bt).sval, "upto") || !strcmp(IR_LIT(bt).sval, "find") || !strcmp(IR_LIT(bt).sval, "bal"))) return 1;
    return 0;
}
static int icn_local_assign_rhs_ok(IR_t *nd) {
    IR_t *r = (nd->n_operands > 0) ? nd->operands[0] : nd->α;
    if (!r) return 0;
    if (r->t == IR_LIT_I || r->t == IR_LIT_S) return 1;
    if (r->t == IR_VAR && IR_LIT(r).sval && IR_LIT(r).sval[0] != '&') return 1;
    if (r->t == IR_BINOP && (IR_LIT(r).ival == BINOP_ADD || IR_LIT(r).ival == BINOP_SUB || IR_LIT(r).ival == BINOP_MUL || IR_LIT(r).ival == BINOP_DIV || IR_LIT(r).ival == BINOP_MOD)) return 1;
    if (r->t == IR_GEN_SCAN) return icn_gen_scan_body_slotful(r);
    return 0;
}
static int icn_assign_safe_kind(IR_e t) {
    return t == IR_ASSIGN || t == IR_VAR || t == IR_CALL || t == IR_SUCCEED || t == IR_FAIL ||
           t == IR_LIT_I || t == IR_LIT_S || t == IR_LIT_F || t == IR_LIT_NUL ||
           t == IR_BINOP || t == IR_IF || t == IR_WHILE || t == IR_UNTIL || t == IR_REPEAT ||
           t == IR_BREAK || t == IR_NEXT || t == IR_CONJ || t == IR_GEN_SCAN;
}
static int icn_graph_has_local_assign(const IR_graph_t *g) {
    extern int g_icn_globals_nv;
    for (int ni = 0; ni < g->n; ni++) {
        IR_t *nd = g->all[ni];
        if (nd && nd->t == IR_ASSIGN && IR_LIT(nd).sval && !(g_icn_globals_nv && is_global(IR_LIT(nd).sval))) return 1;
    }
    return 0;
}
static int icn_graph_has_binop(const IR_graph_t *g) {
    for (int ni = 0; ni < g->n; ni++) if (g->all[ni] && g->all[ni]->t == IR_BINOP) return 1;
    return 0;
}
static int icn_graph_var_assigned_or_param(stage2_t *s2, int gi, IR_graph_t *g, const char *name) {
    for (int i = 0; i < g->n; i++) { IR_t *m = g->all[i]; if (m && m->t == IR_ASSIGN && IR_LIT(m).sval && !strcmp(IR_LIT(m).sval, name)) return 1; }
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
            if (icn_kind_native_stub(nd->t)) return 0;
            if (has_lassign && !icn_assign_safe_kind(nd->t)) return 0;
            if (has_lassign && nd->t == IR_BINOP && !((IR_LIT(nd).ival >= BINOP_LT && IR_LIT(nd).ival <= BINOP_NE) || IR_LIT(nd).ival == BINOP_ADD || IR_LIT(nd).ival == BINOP_SUB || IR_LIT(nd).ival == BINOP_MUL || IR_LIT(nd).ival == BINOP_DIV || IR_LIT(nd).ival == BINOP_MOD)) return 0;
            if (has_lassign && has_binop && (nd->t == IR_LIT_F || nd->t == IR_LIT_NUL)) return 0;
            if (has_lassign && nd->t == IR_CALL && !(IR_LIT(nd).sval && (!strcmp(IR_LIT(nd).sval, "write") || !strcmp(IR_LIT(nd).sval, "writes")))) return 0;
            if (for_run && nd->t == IR_CALL && (IR_LIT(nd).dval == 3.0 || (IR_LIT(nd).sval && rt_proc_is_registered(IR_LIT(nd).sval)))) { int _icn_io = IR_LIT(nd).sval && (!strcmp(IR_LIT(nd).sval,"write")||!strcmp(IR_LIT(nd).sval,"writes")||!strcmp(IR_LIT(nd).sval,"writeln")||!strcmp(IR_LIT(nd).sval,"nl")||!strcmp(IR_LIT(nd).sval,"halt")); if (!_icn_io) return 0; }
            if (nd->t == IR_GEN_SCAN) {
                if (IR_LIT(nd).dval != 1.0) return 0;
                IR_graph_t *ssg = (IR_graph_t *)(intptr_t) IR_EXEC(nd).counter;
                IR_graph_t *bsg = (IR_graph_t *)(intptr_t) IR_LIT(nd).ival;
                if (!icn_scan_subgraph_safe(s2, gi, g, ssg, 0) || !icn_scan_subgraph_safe(s2, gi, g, bsg, 0)) return 0;
                if (nd->γ && nd->γ->t == IR_CALL && !icn_gen_scan_body_slotful(nd)) return 0;
            }
            { extern int g_icn_globals_nv;
              if (nd->t == IR_VAR && IR_EXEC(nd).state == 1 && !g_icn_globals_nv) return 0;
              if (nd->t == IR_VAR && IR_LIT(nd).sval && IR_LIT(nd).sval[0] != '&' && IR_EXEC(nd).state != 1 && !icn_graph_var_assigned_or_param(s2, gi, g, IR_LIT(nd).sval)) return 0;
              if (nd->t == IR_ASSIGN && IR_LIT(nd).sval) {
                  int lhs_global = is_global(IR_LIT(nd).sval);
                  if (lhs_global && g_icn_globals_nv) { /* nv global assign: bb_gvar_assign_icn (BUILT) */ }
                  else if (icn_local_assign_rhs_ok(nd)) { /* wave-1 local assign: bb_assign_local (lit/var rhs) */ }
                  else return 0; /* other rhs shapes: native store arm not built -> clean EXCISE, never abort */
              } }
            if (has_alt) {
                if (!icn_alt_safe_kind(nd->t)) return 0;
                if (nd->t == IR_ALT && !icn_alt_arms_all_simple_lit(g, nd)) return 0;
            }
        }
    }
    return 1;
}
/*====================================================================================================================*/
/*--------------------------------------------------------------------------------------------------------------------*/
static int pl_flat_arith_leaf_simple(const IR_t *o) {
    if (!o) return 0;
    return o->t == IR_LIT_I || o->t == IR_LOGICVAR;
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
    return o->t == IR_LIT_I || o->t == IR_LIT_F || o->t == IR_LOGICVAR;
}
static int pl_flat_goal_is_simple(const IR_t *g) {
    if (!g) return 0;
    switch (g->t) {
    case IR_SUCCEED: case IR_CUT: case IR_ATOM: return 1;
    case IR_BUILTIN: {
        const char *fn = IR_LIT(g).sval ? IR_LIT(g).sval : "";
        if (!strcmp(fn, "is")) {
            const IR_t *lhs = g->α, *rhs = g->β;
            if (!lhs || lhs->t != IR_LOGICVAR || !rhs) return 0;
            if (rhs->t == IR_ATOM)
                return IR_LIT(rhs).sval && (!strcmp(IR_LIT(rhs).sval, "pi") || !strcmp(IR_LIT(rhs).sval, "e"));
            if (rhs->t != IR_ARITH) return 0;
            const char *rop = IR_LIT(rhs).sval ? IR_LIT(rhs).sval : "+";
            const IR_t *r0 = (rhs->n_operands > 0) ? rhs->operands[0] : NULL;
            const IR_t *r1 = (rhs->n_operands > 1) ? rhs->operands[1] : NULL;
            int floaty = pl_arith_op_floaty(rop)
                      || (r0 && r0->t == IR_LIT_F)
                      || (r1 && r1->t == IR_LIT_F);
            if (floaty) {
                if (r0 && r1) return pl_flat_arith_leaf_float_ok(r0) && pl_flat_arith_leaf_float_ok(r1);
                if (r0 && !r1) return pl_flat_arith_leaf_float_ok(r0);
                return 0;
            }
            if (r0 && r1) return pl_flat_arith_leaf_simple(r0) && pl_flat_arith_leaf_simple(r1);
            if (r0 && !r1) return pl_flat_arith_leaf_simple(r0);
            return 0;
        }
        if (!strcmp(fn, "succ") && IR_LIT(g).ival == 2 && g->α && g->β) {
            int a0ok = (g->α->t == IR_LOGICVAR || g->α->t == IR_LIT_I);
            int a1ok = (g->β->t == IR_LOGICVAR || g->β->t == IR_LIT_I);
            return a0ok && a1ok;
        }
        if (!strcmp(fn, "plus") && IR_LIT(g).ival == 3 && ir_call_arg(g,0) && ir_call_arg(g,1) && ir_call_arg(g,2)) {
            IR_t *a0 = ir_call_arg(g,0), *a1 = ir_call_arg(g,1), *a2 = ir_call_arg(g,2);
            int a0ok = (a0->t == IR_LOGICVAR || a0->t == IR_LIT_I);
            int a1ok = (a1->t == IR_LOGICVAR || a1->t == IR_LIT_I);
            int a2ok = (a2->t == IR_LOGICVAR || a2->t == IR_LIT_I);
            return a0ok && a1ok && a2ok;
        }
        int is_io = (!strcmp(fn, "write") || !strcmp(fn, "writeln") || !strcmp(fn, "print") || !strcmp(fn, "nl") || !strcmp(fn, "halt"));
        if (!is_io) return 0;
        if (IR_LIT(g).ival >= 1) { const IR_t *a = ir_call_arg(g,0); if (!a || (a->t != IR_ATOM && a->t != IR_LIT_I && a->t != IR_LIT_F && a->t != IR_LOGICVAR)) return 0; }
        return 1;
    }
    case IR_UNIFY: {
        const IR_t *l = (g->n_operands > 0) ? g->operands[0] : NULL, *r = (g->n_operands > 1) ? g->operands[1] : NULL;
        if (!l || !r) return 1;
        int l_var = (l->t == IR_LOGICVAR), r_var = (r->t == IR_LOGICVAR);
        int l_con = (l->t == IR_LIT_I || l->t == IR_LIT_F || l->t == IR_ATOM);
        int r_con = (r->t == IR_LIT_I || r->t == IR_LIT_F || r->t == IR_ATOM);
        return (l_var && r_con) || (l_con && r_var) || (l_con && r_con);
    }
    default: return 0;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_flat_body_root(IR_graph_t *g) {
    if (!g || !g->all) return NULL;
    IR_t *gconj = NULL;
    for (int i = 0; i < g->n; i++) {
        IR_t *nd = g->all[i];
        if (nd && nd->t == IR_GCONJ) { if (gconj) return NULL; gconj = nd; }
    }
    if (!gconj) {
        if (g->nslots > 0) return NULL;
        return (g->entry && g->entry->t == IR_SUCCEED) ? g->entry : NULL;
    }
    bb_conj_state_t *zs = (bb_conj_state_t *)(intptr_t)IR_LIT(gconj).ival;
    if (!zs || !zs->goals || zs->ngoals <= 0) return NULL;
    for (int i = 0; i < zs->ngoals; i++) if (!pl_flat_goal_is_simple(zs->goals[i])) return NULL;
    return gconj;
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
        if (nd->t == IR_CHOICE || nd->t == IR_GOAL || nd->t == IR_BUILTIN || nd->t == IR_CUT ||
            nd->t == IR_DISJ || nd->t == IR_ITE || nd->t == IR_CATCH || nd->t == IR_ARITH ||
            nd->t == IR_STRUCT) return 0;
    }
    if (ar == 0) {
        if (cg->entry->t != IR_SUCCEED) return 0;
        *units_out = NULL; return 1;
    }
    IR_t *gconj = NULL;
    for (int i = 0; i < cg->n; i++) if (cg->all[i] && cg->all[i]->t == IR_GCONJ) { if (gconj) return 0; gconj = cg->all[i]; }
    if (!gconj) return 0;
    bb_conj_state_t *zs = (bb_conj_state_t *)(intptr_t)IR_LIT(gconj).ival;
    if (!zs || !zs->goals || zs->ngoals != ar) return 0;
    for (int i = 0; i < ar; i++) {
        IR_t *u = zs->goals[i];
        if (!u || u->t != IR_UNIFY || u->n_operands < 2 || !u->operands[0] || !u->operands[1]) return 0;
        if (u->operands[0]->t != IR_LOGICVAR || (int)IR_LIT(u->operands[0]).ival != i) return 0;
        if (u->operands[1]->t == IR_ATOM && IR_LIT(u->operands[1]).sval) continue;
        if (u->operands[1]->t == IR_LIT_I) continue;
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
        if (a->t == IR_LOGICVAR) { if ((int)IR_LIT(a).ival < 0 || (int)IR_LIT(a).ival >= 64) return 0; continue; }
        if (a->t == IR_ATOM && IR_LIT(a).sval) continue;
        if (a->t == IR_LIT_I) continue;
        return 0;
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
    if (!cg || !cg->entry || cg->entry->t != IR_CHOICE) return NULL;
    if (ar > 2) return NULL;
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
        for (int j = 0; j < ar; j++) st->consts[k][j] = units ? units[j]->β : NULL;
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
/* PL-GZ-5c: a callee whose graph entry is a CHOICE over 2..4 RULE clauses (facts = empty-body rules).
 * Validation recurses through clause bodies; the visiting list breaks self/mutual recursion cycles
 * (a graph already on the list is being validated up-stack — assume ok here; a real failure
 * surfaces at the outer frame). */
static IR_graph_t *g_gz_visiting[16]; static int g_gz_nvisiting = 0;
static int pl_gz_choice_rule_clauses(IR_graph_t *cg, int ar, bb_choice_state_t **bc_out) {
    if (!cg || !cg->entry || cg->entry->t != IR_CHOICE || ar > 2) return 0;
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
    if (gg->t == IR_SUCCEED || gg->t == IR_FAIL || gg->t == IR_CUT) return 1;
    if (gg->t == IR_GOAL) {
        bb_goal_state_t *zc = NULL; int ar2 = 0;
        IR_graph_t *cg2 = pl_gz_goal_callee(gg, &zc, &ar2);
        if (!cg2 || ar2 > 2) return 0;
        if (!pl_gz_call_args_ok(zc, ar2)) return 0;
        if (cg2->entry && cg2->entry->t == IR_CHOICE)
            return pl_gz_choice_rule_clauses(cg2, ar2, NULL);
        return 1;
    }
    if (gg->t == IR_UNIFY) {
        IR_t *l = (gg->n_operands > 0) ? gg->operands[0] : NULL, *r = (gg->n_operands > 1) ? gg->operands[1] : NULL;
        if (!l || !r) return 0;
        int lv = (l->t == IR_LOGICVAR), rv = (r->t == IR_LOGICVAR);
        int lc = (l->t == IR_ATOM || l->t == IR_LIT_I || l->t == IR_LIT_F), rc = (r->t == IR_ATOM || r->t == IR_LIT_I || r->t == IR_LIT_F);
        return (lv && (rv || rc)) || (rv && lc) || (lc && rc);
    }
    if (gg->t == IR_BUILTIN && IR_LIT(gg).sval && !strcmp(IR_LIT(gg).sval, "nl") && IR_LIT(gg).ival == 0) return 1;
    if (gg->t == IR_BUILTIN && IR_LIT(gg).sval && !strcmp(IR_LIT(gg).sval, "write") && IR_LIT(gg).ival == 1 && ir_call_arg(gg,0)) {
        const IR_t *wa0 = ir_call_arg(gg,0);
        return wa0->t == IR_ATOM || wa0->t == IR_LIT_I || wa0->t == IR_LIT_F || wa0->t == IR_LOGICVAR;
    }
    if (gg->t == IR_BUILTIN && IR_LIT(gg).sval && !strcmp(IR_LIT(gg).sval, "is") && IR_LIT(gg).ival == 2 && gg->α && gg->β) {
        if (gg->α->t != IR_LOGICVAR) return 0;
        IR_t *rhs = gg->β;
        IR_t *q0 = (rhs->n_operands > 0) ? rhs->operands[0] : NULL, *q1 = (rhs->n_operands > 1) ? rhs->operands[1] : NULL;
        int rhs_const = (rhs->t == IR_LIT_I) || ((rhs->t == IR_ARITH) && pl_gz_arith_const(rhs));
        int rhs_varop = (rhs->t == IR_LOGICVAR) || (rhs->t == IR_ARITH && IR_LIT(rhs).sval && q0 && q1 && q0->t == IR_LOGICVAR && q1->t == IR_LIT_I);
        int rhs_bivar = (rhs->t == IR_ARITH && IR_LIT(rhs).sval && q0 && q1 && q0->t == IR_LOGICVAR && q1->t == IR_LOGICVAR);
        return rhs_const || rhs_varop || rhs_bivar;
    }
    if (gg->t == IR_BUILTIN && IR_LIT(gg).sval && IR_LIT(gg).ival == 2 && gg->α && gg->β) {
        const char *fn = IR_LIT(gg).sval;
        int is_cmp = (strcmp(fn,"<")==0||strcmp(fn,">")==0||strcmp(fn,">=")==0||strcmp(fn,"=<")==0||strcmp(fn,"=:=")==0||strcmp(fn,"=\\=")==0);
        if (!is_cmp) return 0;
        return (gg->α->t == IR_LIT_I || gg->α->t == IR_LOGICVAR) && (gg->β->t == IR_LIT_I || gg->β->t == IR_LOGICVAR);
    }
    return 0;
}
static int pl_gz_rule_clause(IR_graph_t *cg, int ar, bb_conj_state_t **zs_out) {
    if (!cg || !cg->entry || !cg->all) return 0;
    if (cg->nslots < ar || cg->nslots > 16) return 0;
    for (int i = 0; i < cg->n; i++) {
        IR_t *nd = cg->all[i];
        if (!nd) continue;
        if (nd->t == IR_CHOICE || nd->t == IR_DISJ ||
            nd->t == IR_ITE || nd->t == IR_CATCH || nd->t == IR_STRUCT) return 0;
        if (nd->t == IR_BUILTIN) {
            if (IR_LIT(nd).sval && !strcmp(IR_LIT(nd).sval, "nl") && IR_LIT(nd).ival == 0) continue;
            if (IR_LIT(nd).sval && !strcmp(IR_LIT(nd).sval, "write") && IR_LIT(nd).ival == 1) continue;
            if (IR_LIT(nd).sval && !strcmp(IR_LIT(nd).sval, "is") && IR_LIT(nd).ival == 2) continue;
            const char *fn = IR_LIT(nd).sval ? IR_LIT(nd).sval : "";
            int is_cmp = (strcmp(fn,"<")==0||strcmp(fn,">")==0||strcmp(fn,">=")==0||strcmp(fn,"=<")==0||strcmp(fn,"=:=")==0||strcmp(fn,"=\\=")==0);
            if (!is_cmp) return 0;
        }
        if (nd->t == IR_LOGICVAR && ((int)IR_LIT(nd).ival < 0 || (int)IR_LIT(nd).ival >= cg->nslots)) return 0;
    }
    IR_t *gconj = NULL;
    for (int i = 0; i < cg->n; i++) if (cg->all[i] && cg->all[i]->t == IR_GCONJ) { if (gconj) return 0; gconj = cg->all[i]; }
    if (!gconj) return 0;
    bb_conj_state_t *zs = (bb_conj_state_t *)(intptr_t)IR_LIT(gconj).ival;
    if (!zs || !zs->goals || zs->ngoals < ar || zs->ngoals > 32) return 0;
    for (int i = 0; i < ar; i++) {
        IR_t *u = zs->goals[i];
        if (!u || u->t != IR_UNIFY || u->n_operands < 2 || !u->operands[0] || !u->operands[1]) return 0;
        IR_t *u0 = u->operands[0], *u1 = u->operands[1];
        if (u0->t != IR_LOGICVAR || (int)IR_LIT(u0).ival != i) return 0;
        if (u1->t == IR_LOGICVAR) { if ((int)IR_LIT(u1).ival < 0 || (int)IR_LIT(u1).ival >= cg->nslots) return 0; continue; }
        if (u1->t == IR_ATOM && IR_LIT(u1).sval) continue;
        if (u1->t == IR_LIT_I) continue;
        return 0;
    }
    for (int i = ar; i < zs->ngoals; i++) if (!pl_gz_rule_body_goal_ok(zs->goals[i])) return 0;
    *zs_out = zs;
    return 1;
}
static int pl_gz_rule_inline_check(IR_t *gg) {
    bb_goal_state_t *zc = NULL; int ar = 0;
    IR_graph_t *cg = pl_gz_goal_callee(gg, &zc, &ar);
    if (!cg) return 0;
    if (ar > 2) return 0;
    if (!pl_gz_call_args_ok(zc, ar)) return 0;
    if (cg->entry && cg->entry->t == IR_CHOICE)
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
        if (!gg || gg->t != IR_GOAL) continue;
        bb_goal_state_t *zc2 = NULL; int ar2 = 0;
        if (!pl_gz_goal_callee(gg, &zc2, &ar2)) return -1;
        for (int ai = 0; ai < ar2 && ai < 2; ai++)
            if (zc2->args[ai] && zc2->args[ai]->t != IR_LOGICVAR) nsynth++;
    }
    return nsynth;
}
static IR_t * pl_gz_arith_slot_map(const IR_t *nd, int ar, int lbase) {
    if (!nd) return NULL;
    if (nd->t == IR_LIT_I) { IR_t *c = pl_gz_det_node(IR_LIT_I); if (c) IR_LIT(c).ival = IR_LIT(nd).ival; return c; }
    if (nd->t == IR_LOGICVAR) { return pl_gz_lv(pl_gz_slot_map((int)IR_LIT(nd).ival, ar, lbase)); }
    if (nd->t == IR_ARITH) {
        IR_t *c = pl_gz_det_node(IR_ARITH); if (!c) return NULL;
        IR_LIT(c).sval = IR_LIT(nd).sval; IR_LIT(c).ival = IR_LIT(nd).ival;
        IR_t *m0 = (nd->n_operands > 0) ? nd->operands[0] : NULL, *m1 = (nd->n_operands > 1) ? nd->operands[1] : NULL;
        if (m0) { c->α = pl_gz_arith_slot_map(m0, ar, lbase); if (!c->α) return NULL; }
        if (m1) { c->β = pl_gz_arith_slot_map(m1, ar, lbase); if (!c->β) return NULL; }
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
        if (u1 && u1->t == IR_LOGICVAR) {
            if ((int)IR_LIT(u1).ival == i) continue;
            cu = pl_gz_det_node(IR_CELL_UNIFY);
            if (!cu) return 0;
            cu->α = pl_gz_lv(i); cu->β = pl_gz_lv(pl_gz_slot_map((int)IR_LIT(u1).ival, ar, lbase));
            if (!cu->α || !cu->β) return 0;
        } else {
            cu = pl_gz_det_node(IR_CELL_UNIFY);
            if (!cu) return 0;
            cu->α = pl_gz_lv(i); cu->β = u1;
            if (!cu->α) return 0;
        }
        if (!head) head = cu; else tail->γ = cu;
        tail = cu;
    }
    for (int i = ar; i < zs->ngoals; i++) {
        IR_t *gg = zs->goals[i];
        IR_t *nn = NULL;
        if (gg->t == IR_SUCCEED) continue;
        if (gg->t == IR_GOAL) {
            bb_goal_state_t *zc2 = NULL; int ar2 = 0;
            IR_graph_t *cg2 = pl_gz_goal_callee(gg, &zc2, &ar2);
            if (!cg2 || ar2 > 2 || !pl_gz_call_args_ok(zc2, ar2)) return 0;
            pl_gz_callee_t *ce2 = pl_gz_callee_get_any(gg, cg2, ar2, callees, ncallees);
            if (!ce2) return 0;
            pl_gz_call_state_t *cs2 = (pl_gz_call_state_t *)GC_MALLOC(sizeof *cs2);
            if (!cs2) return 0;
            memset(cs2, 0, sizeof *cs2);
            cs2->callee = ce2; cs2->nargs = ar2;
            for (int ai = 0; ai < ar2; ai++) {
                IR_t *a = zc2->args[ai];
                if (a->t == IR_LOGICVAR) {
                    cs2->args[ai] = pl_gz_lv(pl_gz_slot_map((int)IR_LIT(a).ival, ar, lbase));
                    if (!cs2->args[ai]) return 0;
                    continue;
                }
                int kk = synth_next++;
                IR_t *cu = pl_gz_det_node(IR_CELL_UNIFY);
                if (!cu) return 0;
                cu->α = pl_gz_lv(kk); cu->β = a;
                if (!cu->α) return 0;
                if (!head) head = cu; else tail->γ = cu;
                tail = cu;
                cs2->args[ai] = pl_gz_lv(kk);
                if (!cs2->args[ai]) return 0;
            }
            cs2->child_slot = ce->arity + ce->nlocals + ce->nchild;
            ce->nchild++;
            nn = pl_gz_det_node(IR_CELL_CALL);
            if (!nn) return 0;
            IR_LIT(nn).ival = (int64_t)(intptr_t)cs2;
        } else if (gg->t == IR_FAIL) {
            nn = pl_gz_det_node(IR_FAIL);
        } else if (gg->t == IR_CUT) {
            nn = pl_gz_det_node(IR_CELL_CUT);
        } else if (gg->t == IR_UNIFY) {
            nn = pl_gz_det_node(IR_CELL_UNIFY);
            if (!nn) return 0;
            IR_t *g0 = (gg->n_operands > 0) ? gg->operands[0] : NULL, *g1 = (gg->n_operands > 1) ? gg->operands[1] : NULL;
            if (!g0 || !g1) return 0;
            nn->α = (g0->t == IR_LOGICVAR) ? pl_gz_lv(pl_gz_slot_map((int)IR_LIT(g0).ival, ar, lbase)) : g0;
            nn->β = (g1->t == IR_LOGICVAR) ? pl_gz_lv(pl_gz_slot_map((int)IR_LIT(g1).ival, ar, lbase)) : g1;
            if (!nn->α || !nn->β) return 0;
        } else if (IR_LIT(gg).sval && !strcmp(IR_LIT(gg).sval, "nl")) {
            nn = pl_gz_det_node(IR_DET_NL);
        } else if (gg->t == IR_BUILTIN && IR_LIT(gg).sval && !strcmp(IR_LIT(gg).sval, "is") && IR_LIT(gg).ival == 2 && gg->α && gg->β) {
            nn = pl_gz_det_node(IR_DET_IS);
            if (!nn) return 0;
            nn->α = pl_gz_lv(pl_gz_slot_map((int)IR_LIT(gg->α).ival, ar, lbase));
            nn->β = pl_gz_arith_slot_map(gg->β, ar, lbase);
            if (!nn->α || !nn->β) return 0;
        } else if (gg->t == IR_BUILTIN && IR_LIT(gg).sval && IR_LIT(gg).ival == 2 && gg->α && gg->β) {
            const char *fn = IR_LIT(gg).sval;
            int is_cmp = (strcmp(fn,"<")==0||strcmp(fn,">")==0||strcmp(fn,">=")==0||strcmp(fn,"=<")==0||strcmp(fn,"=:=")==0||strcmp(fn,"=\\=")==0);
            if (!is_cmp) return 0;
            nn = pl_gz_det_node(IR_DET_CMP);
            if (!nn) return 0;
            IR_LIT(nn).sval = fn;
            nn->α = (gg->α->t == IR_LOGICVAR) ? pl_gz_lv(pl_gz_slot_map((int)IR_LIT(gg->α).ival, ar, lbase)) : gg->α;
            nn->β = (gg->β->t == IR_LOGICVAR) ? pl_gz_lv(pl_gz_slot_map((int)IR_LIT(gg->β).ival, ar, lbase)) : gg->β;
            if (!nn->α || !nn->β) return 0;
        } else {
            nn = pl_gz_det_node(IR_DET_WRITE);
            if (!nn) return 0;
            IR_t *w0 = ir_call_arg(gg, 0);
            if      (w0->t == IR_ATOM)  IR_LIT(nn).sval = IR_LIT(w0).sval;
            else if (w0->t == IR_LIT_I) { IR_LIT(nn).sval = NULL; IR_LIT(nn).ival = IR_LIT(w0).ival; }
            else { IR_LIT(nn).sval = NULL; IR_LIT(nn).ival = 0; nn->α = pl_gz_lv(pl_gz_slot_map((int)IR_LIT(w0).ival, ar, lbase)); if (!nn->α) return 0; }
        }
        if (!nn) return 0;
        if (!head) head = nn; else tail->γ = nn;
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
    if (cg->entry && cg->entry->t == IR_CHOICE) {
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
    if (!root || root->t == IR_SUCCEED) return 1;
    if (root->t == IR_GCONJ) {
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
    if (gg->t == IR_ITE) {
        bb_ite_state_t *zi = (bb_ite_state_t *)(intptr_t)IR_LIT(gg).ival;
        if (!zi) return 0;
        if (!pl_gz_count_synth_root(zi->cond_root, nsynth)) return 0;
        if (!pl_gz_count_synth_root(zi->then_root, nsynth)) return 0;
        if (!pl_gz_count_synth_root(zi->else_root, nsynth)) return 0;
        return 1;
    }
    if (gg->t != IR_GOAL) return 1;
    IR_t **uu = NULL; int aa = 0;
    if (pl_gz_fact_inline(gg, &uu, &aa)) return 1;
    if (pl_gz_choice_inline(gg)) return 1;
    bb_goal_state_t *zc = NULL; int ar = 0;
    IR_graph_t *cg = pl_gz_goal_callee(gg, &zc, &ar);
    if (!cg) return 0;
    for (int ai = 0; ai < ar && ai < 2; ai++)
        if (zc->args[ai] && zc->args[ai]->t != IR_LOGICVAR) (*nsynth)++;
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
    if (nd->t == IR_LIT_I) return 1;
    if (nd->t != IR_ARITH || !IR_LIT(nd).sval) return 0;
    const char *op = IR_LIT(nd).sval;
    if (strcmp(op,"+")==0||strcmp(op,"-")==0||strcmp(op,"*")==0||strcmp(op,"/")==0||strcmp(op,"mod")==0||strcmp(op,"rem")==0||strcmp(op,"abs")==0) {
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
    if (!root || root->t == IR_SUCCEED) return 1;
    if (root->t == IR_GCONJ) {
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
    for (IR_t *g = head; g; g = g->γ) {
        if (g->t == IR_CELL_CHOICE || g->t == IR_CELL_CALL) return 0;
        if (g->t == IR_CELL_ITE) {
            pl_gz_ite_state_t *is = (pl_gz_ite_state_t *)(intptr_t)IR_LIT(g).ival;
            if (!is || !pl_gz_chain_det(is->then_head) || !pl_gz_chain_det(is->else_head)) return 0;
        }
    }
    return 1;
}
static int pl_gz_build_goal(IR_t *gg, IR_t **head, IR_t **tail, int *synth_next, int *cslot, pl_gz_callee_t **callees, int *ncallees) {
    if (!gg) return 0;
    if (gg->t == IR_SUCCEED) return 1;
    if (gg->t == IR_ITE) {
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
        if (!*head) *head = cn; else (*tail)->γ = cn;
        *tail = cn;
        return 1;
    }
    if (gg->t == IR_FAIL) {
        IR_t *fnode = pl_gz_det_node(IR_FAIL);
        if (!fnode) return 0;
        if (!*head) *head = fnode; else (*tail)->γ = fnode;
        *tail = fnode;
        return 1;
    }
    if (gg->t == IR_GOAL) {
        IR_t **units = NULL; int ar = 0;
        if (pl_gz_fact_inline(gg, &units, &ar)) {
            bb_goal_state_t *zc = (bb_goal_state_t *)(intptr_t)IR_LIT(gg).ival;
            for (int ai = 0; ai < ar; ai++) {
                IR_t *cu = pl_gz_det_node(IR_CELL_UNIFY);
                if (!cu) return 0;
                cu->α = zc->args[ai];
                cu->β = units[ai]->β;
                if (!*head) *head = cu; else (*tail)->γ = cu;
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
            if (!*head) *head = cn; else (*tail)->γ = cn;
            *tail = cn;
            return 1;
        }
        bb_goal_state_t *zc = NULL;
        IR_graph_t *cg = pl_gz_goal_callee(gg, &zc, &ar);
        if (!cg || ar > 2 || !pl_gz_call_args_ok(zc, ar)) return 0;
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
            if (a->t == IR_LOGICVAR) { cs->args[ai] = a; continue; }
            int kk = (*synth_next)++;
            IR_t *cu = pl_gz_det_node(IR_CELL_UNIFY);
            if (!cu) return 0;
            cu->α = pl_gz_lv(kk); cu->β = a;
            if (!cu->α) return 0;
            if (!*head) *head = cu; else (*tail)->γ = cu;
            *tail = cu;
            cs->args[ai] = pl_gz_lv(kk);
            if (!cs->args[ai]) return 0;
        }
        IR_t *cn = pl_gz_det_node(IR_CELL_CALL);
        if (!cn) return 0;
        IR_LIT(cn).ival = (int64_t)(intptr_t)cs;
        if (!*head) *head = cn; else (*tail)->γ = cn;
        *tail = cn;
        return 1;
    }
    IR_t *nn = NULL;
    if (gg->t == IR_UNIFY) {
        nn = pl_gz_det_node(IR_CELL_UNIFY);
        if (nn) { nn->α = (gg->n_operands > 0) ? gg->operands[0] : NULL; nn->β = (gg->n_operands > 1) ? gg->operands[1] : NULL; }
    } else if (gg->t == IR_BUILTIN && IR_LIT(gg).sval && strcmp(IR_LIT(gg).sval, "nl") == 0 && IR_LIT(gg).ival == 0) {
        nn = pl_gz_det_node(IR_DET_NL);
    } else if (gg->t == IR_BUILTIN && IR_LIT(gg).sval && strcmp(IR_LIT(gg).sval, "write") == 0 && IR_LIT(gg).ival == 1 && ir_call_arg(gg,0)) {
        IR_t *wz0 = ir_call_arg(gg,0);
        if (wz0->t == IR_ATOM && IR_LIT(wz0).sval) { nn = pl_gz_det_node(IR_DET_WRITE); if (nn) IR_LIT(nn).sval = IR_LIT(wz0).sval; }
        else if (wz0->t == IR_LIT_I)          { nn = pl_gz_det_node(IR_DET_WRITE); if (nn) { IR_LIT(nn).sval = NULL; IR_LIT(nn).ival = IR_LIT(wz0).ival; } }
        else if (wz0->t == IR_LOGICVAR)       { nn = pl_gz_det_node(IR_DET_WRITE); if (nn) { IR_LIT(nn).sval = NULL; IR_LIT(nn).ival = 0; nn->α = wz0; } }
        else return 0;
    } else if (gg->t == IR_BUILTIN && IR_LIT(gg).sval && strcmp(IR_LIT(gg).sval, "is") == 0 && IR_LIT(gg).ival == 2 && gg->α && gg->β) {
        IR_t *lhs = gg->α, *rhs = gg->β;
        if (lhs->t != IR_LOGICVAR) return 0;
        int rhs_is_const = (rhs->t == IR_LIT_I) || ((rhs->t == IR_ARITH) && pl_gz_arith_const(rhs));
        IR_t *w0 = (rhs->n_operands > 0) ? rhs->operands[0] : NULL, *w1 = (rhs->n_operands > 1) ? rhs->operands[1] : NULL;
        int rhs_is_var_op = (rhs->t == IR_LOGICVAR) ||
                            (rhs->t == IR_ARITH && IR_LIT(rhs).sval && w0 && w1 &&
                             w0->t == IR_LOGICVAR && w1->t == IR_LIT_I);
        if (!rhs_is_const && !rhs_is_var_op) return 0;
        nn = pl_gz_det_node(IR_DET_IS);
        if (nn) { nn->α = lhs; nn->β = rhs; }
    } else if (gg->t == IR_BUILTIN && IR_LIT(gg).sval && IR_LIT(gg).ival == 2 && gg->α && gg->β) {
        const char *fn = IR_LIT(gg).sval;
        int is_arith_cmp = (strcmp(fn,"<")==0||strcmp(fn,">")==0||strcmp(fn,">=")==0||strcmp(fn,"=<")==0||strcmp(fn,"=:=")==0||strcmp(fn,"=\\=")==0);
        if (!is_arith_cmp) return 0;
        IR_t *la = gg->α, *ra = gg->β;
        if (la->t != IR_LIT_I && la->t != IR_LOGICVAR) return 0;
        if (ra->t != IR_LIT_I && ra->t != IR_LOGICVAR) return 0;
        nn = pl_gz_det_node(IR_DET_CMP);
        if (nn) { IR_LIT(nn).sval = fn; nn->α = la; nn->β = ra; }
    } else {
        return 0;
    }
    if (!nn) return 0;
    if (!*head) *head = nn; else (*tail)->γ = nn;
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
        if (!nd || nd->t != IR_ITE) continue;
        bb_ite_state_t *zi = (bb_ite_state_t *)(intptr_t)IR_LIT(nd).ival;
        if (!zi) return NULL;
        IR_t *rr[3] = { zi->cond_root, zi->then_root, zi->else_root };
        for (int k = 0; k < 3; k++)
            if (rr[k] && rr[k]->t == IR_GCONJ) { if (nclaimed >= 8) return NULL; claimed[nclaimed++] = rr[k]; }
    }
    for (int i = 0; i < g->n; i++) {
        IR_t *nd = g->all[i];
        if (!nd) continue;
        if (nd->t == IR_GCONJ) {
            int cl = 0;
            for (int k = 0; k < nclaimed; k++) if (claimed[k] == nd) { cl = 1; break; }
            if (!cl) { if (ngconj >= 2) return NULL; gconjs[ngconj++] = nd; }
        }
        if (nd->t == IR_DISJ) {
            if (softdisj) return NULL;
            int na = 0;
            IR_t * const *arms = bb_operand_aux_get(g, nd, &na);
            if (!arms || na != 2 || !arms[0] || !arms[1]) return NULL;
            if (arms[1]->t != IR_SUCCEED) return NULL;
            softdisj = nd; soft_arm0 = arms[0];
        }
        if (nd->t == IR_GOAL) { IR_t **uu = NULL; int aa = 0; if (!pl_gz_fact_inline(nd, &uu, &aa) && !pl_gz_choice_inline(nd) && !pl_gz_rule_inline_check(nd)) return NULL; }
        if (nd->t == IR_CHOICE || nd->t == IR_CUT ||
            nd->t == IR_CATCH) return NULL;
        if (nd->t == IR_STRUCT) {
            if (g_gz_no_struct_ptr) return NULL;
            int parent_unify = 0;
            for (int j = 0; j < g->n; j++) {
                IR_t *p = g->all[j];
                if (p && p->t == IR_UNIFY && ((p->n_operands > 0 && p->operands[0] == nd) || (p->n_operands > 1 && p->operands[1] == nd))) { parent_unify = 1; break; }
            }
            if (!parent_unify) return NULL;
        }
        if (nd->t == IR_LOGICVAR && ((int)IR_LIT(nd).ival < 0 || (int)IR_LIT(nd).ival >= 64)) return NULL;
        if (nd->t == IR_UNIFY) {
            IR_t *l = (nd->n_operands > 0) ? nd->operands[0] : NULL, *r = (nd->n_operands > 1) ? nd->operands[1] : NULL;
            if (!l || !r) return NULL;
            int ls = (l->t == IR_STRUCT), rs = (r->t == IR_STRUCT);
            if (ls || rs) {
                int lok = ls || l->t == IR_LOGICVAR || l->t == IR_ATOM || l->t == IR_LIT_I || l->t == IR_LIT_F;
                int rok = rs || r->t == IR_LOGICVAR || r->t == IR_ATOM || r->t == IR_LIT_I || r->t == IR_LIT_F;
                if (!lok || !rok) return NULL;
                continue;
            }
            int lv = (l->t == IR_LOGICVAR), rv = (r->t == IR_LOGICVAR);
            int lc = (l->t == IR_ATOM || l->t == IR_LIT_I || l->t == IR_LIT_F), rc = (r->t == IR_ATOM || r->t == IR_LIT_I || r->t == IR_LIT_F);
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
        if (soft_arm0->t == IR_GCONJ) {
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
            if (!lastA || lastA->t != IR_FAIL) return NULL;
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
    } else if (g->entry && g->entry->t == IR_ITE) {
        IR_t *cur = g->entry;
        while (cur && cur->t != IR_SUCCEED && cur->t != IR_FAIL && ng < 64) {
            if (cur->t != IR_GCONJ) goals_buf[ng++] = cur;
            cur = cur->γ;
        }
    } else if (!(g->entry && g->entry->t == IR_SUCCEED)) {
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
    qf->α = head;
    qf->β = headB;
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
    const IR_t *lhs = g->α, *rhs = g->β;
    if (!lhs || lhs->t != IR_LIT_I || !rhs || rhs->t != IR_ARITH) return 0;
    const char *rop = IR_LIT(rhs).sval ? IR_LIT(rhs).sval : "+";
    if (pl_arith_op_floaty(rop)) return 0;
    const IR_t *e0 = (rhs->n_operands > 0) ? rhs->operands[0] : NULL, *e1 = (rhs->n_operands > 1) ? rhs->operands[1] : NULL;
    if (!e0 || !e1) return 0;
    if (e0->t == IR_LIT_F || e1->t == IR_LIT_F) return 0;
    return pl_flat_arith_leaf_simple(e0) && pl_flat_arith_leaf_simple(e1);
}
static int pl_findall_term_buildable(const IR_t *a) {
    if (!a) return 0;
    switch (a->t) {
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
        switch (nd->t) {
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
    if (g->t == IR_FAIL || g->t == IR_SUCCEED) return 1;
    if (g->t == IR_ATOM && IR_LIT(g).sval && (!strcmp(IR_LIT(g).sval,"true")||!strcmp(IR_LIT(g).sval,"fail")||!strcmp(IR_LIT(g).sval,"false"))) return 1;
    if (g->t != IR_GOAL) return 0;
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
        if (nd && nd->t == IR_GCONJ) { if (gconj) return 0; gconj = nd; }
    }
    if (!gconj) return 0;
    bb_conj_state_t *zs = (bb_conj_state_t *)(intptr_t)IR_LIT(gconj).ival;
    if (!zs || !zs->goals || zs->ngoals < 1) return 0;
    for (int i = 0; i < gg->n; i++) {
        const IR_t *nd = gg->all[i];
        if (!nd) continue;
        switch (nd->t) {
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
    if (g->t == IR_GOAL) return pl_findall_goal_admissible(g);
    if (g->t == IR_BUILTIN && IR_LIT(g).sval) {
        static const char *mset[] = { "is", "=:=", "=\\=", "<", ">", "=<", ">=", NULL };
        for (int k = 0; mset[k]; k++)
            if (!strcmp(IR_LIT(g).sval, mset[k])) return g->α && g->β && pl_findall_term_buildable(g->α) && pl_findall_term_buildable(g->β);
    }
    return 0;
}
static int pl_rich_node_emittable(const IR_t *nd) {
    if (!nd) return 1;
    switch (nd->t) {
    case IR_GCONJ: case IR_GOAL:
    case IR_CHOICE: case IR_DISJ:
    case IR_SUCCEED: case IR_FAIL: case IR_CUT: case IR_ITE_COMMIT: case IR_ITE_GATE:
    case IR_LOGICVAR: case IR_ATOM: case IR_STRUCT:
    case IR_LIT_I: case IR_LIT_F: case IR_LIT_S: case IR_LIT_NUL:
        return 1;
    case IR_ITE: {
        bb_ite_state_t *zi = (bb_ite_state_t *)(intptr_t)IR_LIT(nd).ival;
        if (zi && !pl_ite_then_branch_trivial(zi->then_)) return 0;
        return 1;
    }
    case IR_CATCH: {
        bb_catch_state_t *zc = (bb_catch_state_t *)(intptr_t)IR_LIT(nd).ival;
        if (!zc || !zc->goal_g || !zc->rec_g) return 0;
        return zc->catcher ? pl_findall_term_buildable(zc->catcher) : 0;
    }
    case IR_UNIFY: {
        const IR_t *l = (nd->n_operands > 0) ? nd->operands[0] : NULL, *r = (nd->n_operands > 1) ? nd->operands[1] : NULL;
        int lk = l ? (int)l->t : -1, rk = r ? (int)r->t : -1;
        if (lk == IR_ARITH || rk == IR_ARITH) return 0;
        return 1;
    }
    case IR_ARITH:
        return 1;
    case IR_BUILTIN: {
        const char *fn = IR_LIT(nd).sval ? IR_LIT(nd).sval : "";
        if (!strcmp(fn, "is")) return pl_flat_goal_is_simple(nd) || pl_rich_is_lint_simple(nd);
        static const char *ok[] = { "write", "writeln", "print", "nl", "halt", NULL };
        for (int k = 0; ok[k]; k++) if (!strcmp(fn, ok[k])) return 1;
        static const char *acmp[] = { ">", "<", ">=", "=<", "<=", "=:=", "=\\=", NULL };
        for (int k = 0; acmp[k]; k++)
            if (!strcmp(fn, acmp[k])) return pl_flat_arith_leaf_simple(nd->α) && pl_flat_arith_leaf_simple(nd->β);
        if (nd->α && nd->β &&
            (!strcmp(fn,"==")||!strcmp(fn,"\\==")||!strcmp(fn,"@<")||!strcmp(fn,"@>")||!strcmp(fn,"@=<")||!strcmp(fn,"@>=")))
            return 1;
        static const char *ttest[] = { "var","nonvar","atom","atomic","number","integer",
                                        "float","compound","callable","is_list","ground", NULL };
        for (int k = 0; ttest[k]; k++) if (!strcmp(fn, ttest[k])) return ir_call_arg(nd,0) != NULL;
        if (!strcmp(fn,"succ")) return IR_LIT(nd).ival==2 && nd->α && nd->β;
        if (!strcmp(fn,"throw")) return IR_LIT(nd).ival==1 && ir_call_arg(nd,0) && pl_findall_term_buildable(ir_call_arg(nd,0));
        if (!strcmp(fn,"findall")) {
            bb_findall_state_t *fs = (bb_findall_state_t *)(intptr_t)IR_LIT(nd).ival;
            if (!fs || !fs->goal_node || !fs->tmpl || !fs->result) return 0;
            if (!pl_findall_term_buildable(fs->tmpl) || !pl_findall_term_buildable(fs->result)) return 0;
            if (pl_findall_goal_graph_simple(fs->gcfg, fs->goal_node)) return pl_findall_goal_admissible(fs->goal_node);
            return pl_findall_goal_conj_admissible(fs->gcfg, fs->goal_node);
        }
        if (!strcmp(fn,"nb_setval")||!strcmp(fn,"nb_getval"))
            return IR_LIT(nd).ival==2 && ir_call_arg(nd,0) && ir_call_arg(nd,1) && pl_findall_term_buildable(ir_call_arg(nd,0)) && pl_findall_term_buildable(ir_call_arg(nd,1));
        if (!strcmp(fn,"aggregate_all"))
            return IR_LIT(nd).ival==3 && ir_call_arg(nd,0) && ir_call_arg(nd,1) && ir_call_arg(nd,2) && pl_findall_term_buildable(ir_call_arg(nd,0)) && pl_findall_term_buildable(ir_call_arg(nd,1)) && pl_findall_term_buildable(ir_call_arg(nd,2));
        if (!strcmp(fn,"plus")) return IR_LIT(nd).ival==3 && ir_call_arg(nd,0) && ir_call_arg(nd,1) && ir_call_arg(nd,2);
        if (!strcmp(fn,"sort")||!strcmp(fn,"msort")) return ir_call_arg(nd,0) && ir_call_arg(nd,1);
        if (!strcmp(fn,"format")) return ir_call_arg(nd,0) && (IR_LIT(nd).ival==1 || IR_LIT(nd).ival==2);
        if (!strcmp(fn,"numbervars")) return IR_LIT(nd).ival==3 && ir_call_arg(nd,0) && ir_call_arg(nd,1) && ir_call_arg(nd,2);
        if (!strcmp(fn,"copy_term")) return ir_call_arg(nd,0) && ir_call_arg(nd,1);
        static const char *atom2[] = { "atom_length","upcase_atom","downcase_atom","string_length",
            "string_upper","string_lower","atom_string","string_to_atom", NULL };
        for (int k = 0; atom2[k]; k++) if (!strcmp(fn, atom2[k])) return ir_call_arg(nd,0) && ir_call_arg(nd,1);
        if (!strcmp(fn,"atom_concat")||!strcmp(fn,"string_concat")) return ir_call_arg(nd,0) && ir_call_arg(nd,1) && ir_call_arg(nd,2);
        static const char *achars[] = { "atom_chars","atom_codes","string_chars","string_codes", NULL };
        for (int k = 0; achars[k]; k++) if (!strcmp(fn, achars[k])) return ir_call_arg(nd,0) && ir_call_arg(nd,1);
        if (!strcmp(fn,"char_type")) return IR_LIT(nd).ival==2 && ir_call_arg(nd,0) && ir_call_arg(nd,1);
        if (!strcmp(fn,"number_string")||!strcmp(fn,"atom_number")) return ir_call_arg(nd,0) && ir_call_arg(nd,1);
        if (!strcmp(fn,"functor")) return IR_LIT(nd).ival==3 && ir_call_arg(nd,0) && ir_call_arg(nd,1) && ir_call_arg(nd,2);
        if (!strcmp(fn,"arg")) return IR_LIT(nd).ival==3 && ir_call_arg(nd,0) && ir_call_arg(nd,1) && ir_call_arg(nd,2);
        if (!strcmp(fn,"=..")) return ir_call_arg(nd,0) && ir_call_arg(nd,1);
        if (!strcmp(fn,"term_to_atom")||!strcmp(fn,"term_string")) return ir_call_arg(nd,0) && ir_call_arg(nd,1);
        if (!strcmp(fn,"writeq")||!strcmp(fn,"write_canonical")) return ir_call_arg(nd,0) != NULL;
        if (!strcmp(fn,"atomic_list_concat")||!strcmp(fn,"concat_atom")) return ir_call_arg(nd,0) && (IR_LIT(nd).ival==2 || IR_LIT(nd).ival==3);
        return 0;
    }
    default:
        return 0;
    }
}
static int pl_rich_graph_ok(IR_graph_t *g) {
    if (!g || !g->all) return 0;
    for (int i = 0; i < g->n; i++) {
        IR_t *nd = g->all[i];
        if (!pl_rich_node_emittable(nd)) return 0;
        if (nd && nd->t == IR_CHOICE) {
            bb_choice_state_t *zc = (bb_choice_state_t *)(intptr_t)IR_LIT(nd).ival;
            if (zc && zc->bodies)
                for (int b = 0; b < zc->nbodies; b++)
                    if (zc->bodies[b] && !pl_rich_graph_ok(zc->bodies[b])) return 0;
        }
        if (nd && nd->t == IR_CATCH) {
            bb_catch_state_t *zk = (bb_catch_state_t *)(intptr_t)IR_LIT(nd).ival;
            if (!zk || !pl_rich_graph_ok(zk->goal_g) || !pl_rich_graph_ok(zk->rec_g)) return 0;
        }
    }
    return 1;
}
static IR_t * pl_rich_body_root(IR_graph_t *main_g) {
    if (!main_g || !main_g->entry) return NULL;
    if (!pl_rich_graph_ok(main_g)) return NULL;
    int npred = resolve_bb_pred_count();
    for (int i = 0; i < npred; i++) {
        const char *nm = resolve_bb_pred_name_at(i);
        if (!nm) continue;
        IR_graph_t *pg = resolve_bb_graph_at(i);
        if (!pg) continue;
        if (!pl_rich_graph_ok(pg)) return NULL;
    }
    if (main_g->body_root) return main_g->body_root;
    {
        IR_t *gconj = NULL;
        for (int i = 0; i < main_g->n; i++) {
            IR_t *nd = main_g->all[i];
            if (nd && nd->t == IR_GCONJ) { if (gconj) { gconj = NULL; break; } gconj = nd; }
        }
        if (gconj) return gconj;
    }
    return main_g->entry;
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
    for (int fi = argi; fi < argc; fi++) {
        const char *d = strrchr(argv[fi], '.');
        if (d && (strcmp(d,".pl")==0 || strcmp(d,".icn")==0 ||
                  strcmp(d,".raku")==0 || strcmp(d,".reb")==0 ||
                  strcmp(d,".sc")==0 || strcmp(d,".scrip")==0 || strcmp(d,".md")==0))
            has_non_sno = 1;
        if (d && strcmp(d,".pl")==0) is_prolog = 1;
        if (d && strcmp(d,".icn")==0) is_icon = 1;
        if (d && strcmp(d,".raku")==0) is_raku = 1;
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
        for (int _pi = 0; _pi < s2->proc_count; _pi++) {
            int idx = s2->proc_table[_pi].bb_idx;
            const char *pname = s2->proc_table[_pi].name ? s2->proc_table[_pi].name : "?";
            if (idx < 0 || idx >= s2->bbp.count || !s2->bbp.table[idx]) continue;
            fprintf(stdout, "; proc %s\n", pname);
            bb_print(s2->bbp.table[idx], stdout);
        }
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
            extern bb_box_fn bb_build_flat(IR_t * nd);
            extern void rt_proc_register(const char *name, void *entry, const char **pnames, int nparams);
            extern void rt_proc_set_builder(bb_box_fn (*builder)(void *entry));
            extern void rt_proc_reset(void);
            int main_bb_idx = -1;
            rt_proc_reset();
            rt_proc_set_builder((bb_box_fn (*)(void *))bb_build_flat);
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
                rt_proc_register(pname, s2->bbp.table[idx]->entry, pn, np);
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
            fflush(stdout);
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
            g_gz_no_struct_ptr = 1;
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
                fflush(stdout);
                return rc;
            }
            IR_t *flat_root = pl_flat_body_root(pl_main);
            if (!flat_root) {
                IR_t *rich_root = pl_rich_body_root(pl_main);
                if (!rich_root) {
                    fprintf(stderr, "[SMX] --compile --target=x86: Prolog mode-4 covers the hello-world + "
                                    "unify/arith + facts/choice/call tiers; this program has a construct not "
                                    "yet wired (PLG-9e+).\n");
                    return 1;
                }
                printf("  .intel_syntax noprefix\n");
                printf("  .text\n");
                extern int codegen_pl_pred_table(FILE * out);
                int nrows = codegen_pl_pred_table(stdout);
                printf("  .globl main\n");
                printf("main:\n");
                printf("  push rbp\n");
                printf("  mov rbp, rsp\n");
                printf("  call rt_main_init@PLT\n");
                if (nrows > 0) {
                    printf("  lea rdi, [rip + .Lpl_pred_table]\n");
                    printf("  mov esi, %d\n", nrows);
                    printf("  call rt_pl_table_install@PLT\n");
                }
                if (pl_main->nslots > 0) {
                    printf("  mov edi, %d\n", pl_main->nslots);
                    printf("  call rt_env_alloc@PLT\n");
                }
                printf("  call rt_frame@PLT\n");
                printf("  mov rdi, rax\n");
                printf("  xor esi, esi\n");
                printf("  call main_\xce\xb1\n");
                printf("  xor eax, eax\n");
                printf("  pop rbp\n");
                printf("  ret\n");
                g_frame_active = 1;
                extern int codegen_pl_catch_blocks(IR_graph_t * main_g, FILE * out);
                int rcc = codegen_pl_catch_blocks(pl_main, stdout);
                int rcp = codegen_clause_dispatch(stdout);
                extern IR_graph_t *g_emit_cfg;
                IR_graph_t *save_cfg = g_emit_cfg; g_emit_cfg = pl_main;
                int rcm = codegen_flat_build(rich_root, stdout, "main");
                g_emit_cfg = save_cfg;
                g_frame_active = 0;
                xa_emit_strtab_rodata();
                fflush(stdout);
                return (rcc || rcp || rcm) ? 1 : 0;
            }
            printf("  .intel_syntax noprefix\n");
            printf("  .text\n");
            printf("  .globl main\n");
            printf("main:\n");
            printf("  push rbp\n");
            printf("  mov rbp, rsp\n");
            if (pl_main->nslots > 0) {
                printf("  mov edi, %d\n", pl_main->nslots);
                printf("  call rt_env_alloc@PLT\n");
            }
            printf("  call rt_frame@PLT\n");
            printf("  mov rdi, rax\n");
            printf("  xor esi, esi\n");
            printf("  call main_\xce\xb1\n");
            printf("  xor eax, eax\n");
            printf("  pop rbp\n");
            printf("  ret\n");
            g_frame_active = 1;
            int rc = codegen_flat_build(flat_root, stdout, "main");
            g_frame_active = 0;
            xa_emit_strtab_rodata();
            fflush(stdout);
            return rc;
        }
        {
            extern int gvar_flat_chain_build_text(IR_graph_t * g, FILE * out, const char * prefix);
            extern void xa_emit_strtab_rodata(void);
            extern int g_frame_active;
            extern void rt_proc_reset(void);
            extern void rt_proc_register(const char * name, void * entry, const char ** pnames, int nparams);
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
            static int sno_pidx_buf[64];
            int n_procs = 0;
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
                rt_proc_register(pname, s2->bbp.table[idx]->entry, pn, np);
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
                printf("sno_proc_startup:\n  push rbp\n  mov rbp, rsp\n  call rt_proc_reset@PLT\n");
                for (int i = 0; i < n_procs; i++) {
                    ProcEntry *pe = &s2->proc_table[sno_pidx_buf[i]];
                    printf("  lea rdi, [rip + .Lsno_pn%d]\n", i);
                    printf("  xor rsi, rsi\n");
                    printf("  lea rdx, [rip + .Lsno_pnames%d]\n", i);
                    printf("  mov ecx, %d\n", pe->nparams);
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
            else printf("  call rt_proc_reset@PLT\n");
            printf("  call rt_frame@PLT\n  mov rdi, rax\n  xor esi, esi\n");
            printf("  call sno_flat_\xce\xb1\n");
            printf("  xor eax, eax\n  pop rbp\n  ret\n");
            int rc = gvar_flat_chain_build_text(sbbg, stdout, "sno_flat");
            g_frame_active = 0;
            xa_emit_strtab_rodata();
            fflush(stdout);
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
            extern bb_box_fn bb_build_flat(IR_t * nd);
            extern void rt_proc_register(const char *name, void *entry, const char **pnames, int nparams);
            extern void rt_proc_set_builder(bb_box_fn (*builder)(void *entry));
            extern void rt_proc_reset(void);
            extern bb_box_fn descr_flat_chain_build_proc(IR_t * entry, const char ** pnames, int np);
            extern void rt_proc_set_fn(const char *name, bb_box_fn fn);
            extern int g_frame_active;
            if ((is_icon || is_raku) && !icn_graph_native_emittable_mode(s2, 1)) {
                fprintf(stderr, "[SMX] --run: mode-3 native emitter does not yet cover this program "
                                "(a box has no MEDIUM_BINARY arm — Icon scan/keyword/cset/gen-alt/suspend, "
                                "or Raku map/grep). EXCISED — mode-2 (--interp) is the oracle for this rung.\n");
                return 0;
            }
            int main_bb_idx = -1;
            rt_proc_reset();
            rt_proc_set_builder((bb_box_fn (*)(void *))bb_build_flat);
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
                rt_proc_register(pname, s2->bbp.table[idx]->entry, pn, np);
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
                        if (nd && nd->t == IR_RECORD_DEF && IR_LIT(nd).sval) dat_register(IR_LIT(nd).sval);
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
            (void)fn(rt_frame(), 0);
            goto run_done;
        }
        if (is_prolog) {
            extern DESCR_t IR_interp_once(IR_graph_t * bbg);
            extern Term **g_resolve_env;
            extern bb_box_fn bb_build_flat(IR_t * nd);
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
            int nslots = pl_main->nslots > 0 ? pl_main->nslots : 1;
            g_resolve_env = (Term **)GC_MALLOC((size_t)(nslots + 8) * sizeof(Term *));
            IR_t *gz_root = pl_gz_admit(pl_main);
            if (gz_root) {
                extern bb_box_fn pl_gz_build(IR_t * nd);
                bb_box_fn gzfn = pl_gz_build(gz_root);
                if (gzfn) { (void)gzfn(rt_frame(), 0); goto run_done; }
            }
            IR_t *flat_root = pl_flat_body_root(pl_main);
            if (flat_root) {
                g_frame_active = 1;
                bb_box_fn pfn = bb_build_flat(flat_root);
                g_frame_active = 0;
                if (pfn) { (void)pfn(rt_frame(), 0); goto run_done; }
            }
            fprintf(stderr, "[PBB] FATAL: --run: program not admitted by pl_gz_admit or pl_flat_body_root — "
                            "no native blob covers it. This is a bug in the GZ admission layer or the flat tier. "
                            "Aborting (PL-GZ FENCE: interp fallback deleted 2026-06-06).\n");
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
            extern void rt_proc_register(const char *name, void *entry, const char **pnames, int nparams);
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
                if (np > 0) {
                    pn = (const char **)calloc((size_t)np, sizeof(const char *));
                    for (int k = 0; k < np && k < s2->proc_table[_pi].lower_sc.n; k++)
                        pn[k] = s2->proc_table[_pi].lower_sc.e[k].name;
                }
                rt_proc_register(pname, s2->bbp.table[idx]->entry, pn, np);
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
