/* ATTIC: dead code excised from src/driver/scrip.c (commit base 2a35216) — unreachable from main per --gc-sections oracle. Provenance only; not compiled. */
/*--------------------------------------------------------------------------------------------------------------------*/
static int kind_native_stub(IR_e t) {
    return t == IR_GEN_ALT || t == IR_KEYWORD || t == IR_PROC_GEN ||
           t == IR_CSET_UNION || t == IR_CSET_DIFF || t == IR_CSET_INTER || t == IR_CSET_COMPL ||
           t == IR_SUSPEND ||
           t == IR_LIST_BANG ||
           t == IR_BINOP_GEN ||
           t == IR_MAP || t == IR_GREP;
}
static int local_assign_rhs_ok(IR_t *nd) {
    IR_t *r = (nd->n_operands > 0) ? nd->operands[0] : ((IR_t*)0);
    return rhs_kind_ok(r);
}
static int assign_safe_kind(IR_e t) {
    return t == IR_ASSIGN || t == IR_VAR || t == IR_CALL || t == IR_SUCCEED || t == IR_FAIL ||
           t == IR_LIT_I || t == IR_LIT_S || t == IR_LIT_F || t == IR_LIT_NUL || t == IR_FIELD_GET ||
           t == IR_BINOP || t == IR_IF || t == IR_WHILE || t == IR_UNTIL || t == IR_REPEAT ||
           t == IR_BREAK || t == IR_NEXT || t == IR_CONJ || t == IR_GEN_SCAN ||
           t == IR_GATHER || t == IR_MAP || t == IR_GREP;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int pl_flat_arith_leaf_simple(const IR_t *o) {
    if (!o) return 0;
    return o->op == IR_LIT_I || o->op == IR_LOGICVAR;
}
/*====================================================================================================================*/
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
