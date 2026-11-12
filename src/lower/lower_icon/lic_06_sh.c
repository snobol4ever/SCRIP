#include "lower/lower_icon/lic_head.h"
#include "lower/lower_icon/lic_01_sh.h"
#include "lower/lower_icon/lic_02_sh.h"
#include "lower/lower_icon/lic_03_sh.h"
#include "lower/lower_icon/lic_04_sh.h"
#include "lower/lower_icon/lic_05_sh.h"
#include "lower/lower_icon/lic_06_sh.h"
IR_t * lower_node(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * dummy = NULL;
    if (!res) res = &dummy;
    cx->beta = ω;
    int after_line = cx->after_line;
    cx->after_line = 0;
    cx->key_after_line = after_line;
    if (!t) { IR_t * s = lic_build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    t = pm_fn_form(cx, t);
    if (t && cx->prog && cx->prog->v.ival && t->n == 2 && (t->t == TT_CSET_UNION || t->t == TT_CSET_DIFF || t->t == TT_CSET_INTER))
        return lower_node(cx, pm_call(t->t == TT_CSET_UNION ? "ICN$UUNION" : t->t == TT_CSET_DIFF ? "ICN$UDIFF" : "ICN$UINTER", (tree_t *) t->c[0], (tree_t *) t->c[1]), γ, ω, res);
    if (lc_is_binop(t->t)) {
        { int64_t fb = 0; int fr = 0; if (icn_const_step(t, &fb, &fr) && fr) { IR_t * nd = lic_build(cx, IR_LIT_REAL, γ, ω); double d; memcpy(&d, &fb, 8); IR_LIT(nd).dval = d; *res = nd; return nd; } }
        int64_t bcode = lc_binop_code(t->t);
        if (bcode == BINOP_CONCAT) bcode = BINOP_CONCAT_FRACDIGIT;
        int64_t bemit = (bcode == BINOP_ADD) ? (int64_t)BINOP_ADD_BIG : (bcode == BINOP_SUB) ? (int64_t)BINOP_SUB_BIG : (bcode == BINOP_MUL) ? (int64_t)BINOP_MUL_BIG : bcode;
        int is_relop = (bcode >= BINOP_LT && bcode <= BINOP_NE) || (bcode >= BINOP_SLT && bcode <= BINOP_SNE) || bcode == BINOP_EQV || bcode == BINOP_NEQV;
        int is_arith = (bcode >= BINOP_ADD && bcode <= BINOP_MOD) || bcode == BINOP_POW;
        int alit = 0, blit = 0;
        { int64_t fb = 0; int fr = 0; alit = icn_const_step(t->c[0], &fb, &fr); fb = 0; fr = 0; blit = icn_const_step(t->c[1], &fb, &fr); }
        IR_t * op = lic_build(cx, is_relop ? IR_BINOP_TEST : IR_BINOP, γ, ω);
        IR_LIT(op).ival = is_relop ? bcode : bemit;
        IR_t * cb2 = (is_arith && !blit) ? lic_build(cx, IR_COERCE_NUMERIC, op, ω) : NULL;
        if (cb2) IR_LIT(cb2).ival = 102 | COERCE_ERR_FAILURE_CONVERTIBLE | ((bcode == BINOP_POW) ? COERCE_KEEP_INT : 0) | COERCE_OP_TAG(bcode) | COERCE_OP_SELF_RIGHT;
        IR_t * ca2 = (is_arith && !alit) ? lic_build(cx, IR_COERCE_NUMERIC, cb2 ? cb2 : op, ω) : NULL;
        if (ca2) IR_LIT(ca2).ival = 102 | COERCE_ERR_FAILURE_CONVERTIBLE | COERCE_OP_TAG(bcode);
        IR_t * bsucc = ca2 ? ca2 : (cb2 ? cb2 : op);
        int late_deref = icn_operand_derefs_late(cx, t->c[0]);
        IR_t * lr = NULL, * rr = NULL, * ea = NULL, * eb = NULL, * lβ = NULL, * rβ = NULL;
        if (late_deref) {
            eb = lic_lower(cx, t->c[1], NULL, ω, &rr);
            rβ = cx->beta;
            cx->after_line = (t->line > 0) ? t->line : -1;
            ea = lic_lower(cx, t->c[0], bsucc, rβ, &lr);
            lβ = cx->beta;
        } else {
            ea = lic_lower(cx, t->c[0], NULL, ω, &lr);
            lβ = cx->beta;
            cx->after_line = (t->line > 0) ? t->line : -1;
            eb = lic_lower(cx, t->c[1], bsucc, lβ, &rr);
            rβ = cx->beta;
        }
        IR_t * opfail = (rβ && rβ != ω && rβ != op) ? rβ : ((lβ && lβ != ω && lβ != op) ? lβ : NULL);
        if (is_relop && opfail) lic_ω_to(op, opfail);
        if (late_deref) lc_γ_to(rr, ea);
        else lc_γ_to(lr, eb);
        if (ca2) { ir_operand_push(ca2, lr); ir_operand_push(ca2, rr); }
        if (cb2) { ir_operand_push(cb2, rr); ir_operand_push(cb2, lr); }
        ir_operand_push(op, ca2 ? ca2 : lr);
        ir_operand_push(op, cb2 ? cb2 : rr);
        cx->beta = (rβ && rβ != ω && rβ != op) ? rβ : ((lβ && lβ != ω && lβ != op) ? lβ : ω);
        *res = op;
        return late_deref ? eb : ea;
    }
    if (is_unop_tt(t->t)) {
        {
            int64_t fb = 0;
            int fr = 0;
            if (icn_const_step(t, &fb, &fr)) {
                if (fr) { IR_t * nd = lic_build(cx, IR_LIT_REAL, γ, ω); double d; memcpy(&d, &fb, 8); IR_LIT(nd).dval = d; *res = nd; return nd; }
                IR_t * nd = lic_build(cx, IR_LIT_INTEGER, γ, ω);
                IR_LIT(nd).ival = fb;
                *res = nd;
                return nd;
            }
        }
        IR_e uop_kind = (t->t == TT_NONNULL) ? IR_UNOP_TEST : IR_UNOP;
        IR_t * op = lic_build(cx, uop_kind, γ, ω);
        IR_LIT(op).ival = (long long) t->t;
        IR_t * orr = NULL;
        if (t->t == TT_MNS || t->t == TT_PLS) {
            IR_t * co = lic_build(cx, IR_COERCE_NUMERIC, op, ω);
            IR_LIT(co).ival = 102 | COERCE_ERR_FAILURE_CONVERTIBLE | ((t->t == TT_MNS) ? COERCE_OP_UNARY_NEG : COERCE_OP_UNARY_POS);
            IR_t * ea = lic_lower(cx, t->c[0], co, ω, &orr);
            ir_operand_push(co, orr);
            ir_operand_push(op, co);
            *res = op;
            return ea;
        }
        IR_t * ea = lic_lower(cx, t->c[0], op, ω, &orr);
        ir_operand_push(op, orr);
        *res = op;
        IR_t * uβ = cx->beta;
        if (uop_kind == IR_UNOP_TEST && uβ && uβ != ω && uβ != op) { lic_ω_to(op, uβ); cx->beta = uβ; }
        return ea;
    }
    switch (t->t) {
        case TT_ILIT:
        { IR_t * nd = lic_build(cx, IR_LIT_INTEGER, γ, ω); IR_LIT(nd).ival = t->v.ival; *res = nd; return nd; }
        case TT_FLIT:
        { IR_t * nd = lic_build(cx, IR_LIT_REAL, γ, ω); IR_LIT(nd).dval = t->v.dval; *res = nd; return nd; }
        case TT_QLIT:
        { IR_t * nd = lic_build(cx, IR_LIT_STRING, γ, ω); IR_LIT(nd).sval = t->v.sval; icn_attach_lit_len(cx, nd, t->slen); *res = nd; return nd; }
        case TT_CSET:
        {
            int cn;
            const char * canon = icn_cset_canon(t->v.sval, t->slen, &cn);
            IR_t * nd = lic_build(cx, IR_LIT_CHARSET, γ, ω);
            IR_LIT(nd).sval = canon;
            icn_attach_lit_len(cx, nd, cn);
            *res = nd;
            return nd;
        }
        case TT_NULL:
        {
            if (t->n > 0 && t->c[0]) {
                IR_t * op = lic_build(cx, IR_UNOP_TEST, γ, ω);
                IR_LIT(op).ival = (long long) TT_NULL;
                IR_t * orr = NULL;
                IR_t * ea = lic_lower(cx, t->c[0], op, ω, &orr);
                ir_operand_push(op, orr);
                *res = op;
                IR_t * nβ = cx->beta;
                if (nβ && nβ != ω && nβ != op) { lic_ω_to(op, nβ); cx->beta = nβ; }
                return ea;
            }
            IR_t * nd = lic_build(cx, IR_FAIL, γ, ω);
            *res = nd;
            return nd;
        }
        case TT_BANG_BINARY:
        {
            const tree_t * lhs = (t->n > 0) ? t->c[0] : NULL;
            const tree_t * rhs = (t->n > 1) ? t->c[1] : NULL;
            tree_t * callee;
            if (lhs && icn_callee_is_name(cx, lhs)) {
                char * un = (cx->prog && cx->prog->v.ival && strncmp(lhs->v.sval, "ICN$", 4) && !icn_is_proc(lhs->v.sval)) ? icn_unicon_row(lhs->v.sval) : NULL;
                callee = ast_node_new(TT_QLIT);
                callee->v.sval = un ? un : lhs->v.sval;
                callee->slen = (int) strlen(callee->v.sval);
            } else callee = (tree_t *) lhs;
            IR_t * cr = NULL;
            IR_t * ce = lic_lower(cx, callee, NULL, ω, &cr);
            IR_t * prevβ = cx->beta;
            IR_t * drf = lic_build(cx, IR_DEREF, γ, ω);
            IR_t * nd = lic_build(cx, IR_CALL_VALUE, drf, NULL);
            ir_operand_push(nd, cr);
            if (rhs) {
                IR_LIT(nd).sval = (char *) "apply";
                IR_t * ar = NULL;
                IR_t * ae = lic_lower(cx, rhs, NULL, prevβ ? prevβ : ω, &ar);
                prevβ = cx->beta;
                lc_γ_to(cr, ae);
                ir_operand_push(nd, ar);
                lic_ω_to(nd, prevβ ? prevβ : ω);
                lc_γ_to(ar, nd);
            } else {
                lic_ω_to(nd, prevβ ? prevβ : ω);
                lc_γ_to(cr, nd);
            }
            lic_ω_to(drf, nd);
            ir_operand_push(drf, nd);
            cx->beta = nd;
            *res = drf;
            return ce;
        }
        case TT_VAR:
        { if (t->v.sval && t->v.sval[0] == '&') return lc_key(cx, t, t->v.sval, γ, ω, res); IR_t * nd = lic_build(cx, IR_VAR, γ, ω); IR_LIT(nd).sval = t->v.sval; *res = nd; return nd; }
        case TT_KEYWORD:
        return lc_key(cx, t, t->v.sval, γ, ω, res);
        case TT_NUL:
        { IR_t * nd = lic_build(cx, IR_VAR, γ, ω); IR_LIT(nd).sval = (char *) "&null"; *res = nd; return nd; }
        case TT_PMATCH:
        return lower_pmatch(cx, t, γ, ω, res);
        case TT_REGEX:
        return lic_lower(cx, rx_translate(t), γ, ω, res);
        case TT_PAND:
        case TT_CAPT_COND_ASGN:
        case TT_CAPT_IMMED_ASGN:
        case TT_CAPT_CURSOR:
        case TT_DEFER:
        case TT_ALT:
        { tree_t * pw = pm_patfn(cx, t); if (pw) return lic_lower(cx, pw, γ, ω, res); }
        fprintf(stderr, "icon: line %d: a Unicon pattern operator is parsed but not lowered yet (ARCH-UNICON step 4b: the pattern type on the SNOBOL4 boxes)\n", t->line);
        exit(1);
        case TT_FIELD:
        {
            IR_t * nd = lic_build(cx, IR_FIELD_GET, γ, ω);
            IR_LIT(nd).sval = (t->n > 1 && t->c[1]) ? t->c[1]->v.sval : t->v.sval;
            nd->seal = icn_field_hint(cx->prog, IR_LIT(nd).sval);
            IR_t * br = NULL;
            IR_t * ea = lic_lower(cx, t->c[0], nd, ω, &br);
            ir_operand_push(nd, br);
            *res = nd;
            return ea;
        }
        case TT_LISTCOMP:
        { tree_t * lcf = th_listcomp(cx, t); return lic_lower(cx, lcf, γ, ω, res); }
        case TT_FNC:
        {
            const tree_t * fn = (t->n > 0) ? t->c[0] : NULL;
            { tree_t * pw = pm_patfn(cx, t); if (pw) return lic_lower(cx, pw, γ, ω, res); }
            { tree_t * thf = th_form(cx, t); if (thf) return lic_lower(cx, thf, γ, ω, res); }
            {
                const char * vn = icn_variable_lit(cx, t);
                if (vn) {
                    tree_t * tg = icn_variable_lit_target(cx, vn);
                    if (!tg) return lc_key(cx, t, "&fail", γ, ω, res);
                    if (tg->t == TT_KEYWORD) return lc_key(cx, t, tg->v.sval, γ, ω, res);
                    return lic_lower(cx, tg, γ, ω, res);
                }
            }
            { tree_t * vc = icn_variable_case(cx, t); if (vc) return lic_lower(cx, vc, γ, ω, res); }
            if (!fn || icn_callee_is_name(cx, fn)) {
                const char * nm = (fn && fn->t == TT_VAR) ? fn->v.sval : "?";
                cx->after_line = after_line;
                if (fn && !strcmp(nm, "match") && t->n == 2 && t->c[1] && !icn_never_pattern(t->c[1]) && t->slen != -5 && cx->prog && cx->prog->v.ival && !icn_is_proc(nm) &&
                    !icn_builtin_reassigned(nm)) {
                    const char * pv = t->c[1]->t == TT_VAR && t->c[1]->v.sval && t->c[1]->v.sval[0] != '&' ? t->c[1]->v.sval : icn_synth_local(cx);
                    const int pvar = pv == t->c[1]->v.sval;
                    const char * sv = icn_synth_local(cx);
                    const char * ev = icn_synth_local(cx);
                    tree_t * pq = ast_node_new(TT_QLIT);
                    pq->v.sval = (char *) "pattern";
                    pq->slen = 7;
                    tree_t * plain = pm_call("match", pm_var(pv), NULL);
                    plain->slen = -5;
                    tree_t * viap =
                        pm_node(TT_CONJ, pm_node(TT_ASSIGN, pm_var(sv), pm_var("&pos"), NULL), pm_node(TT_PMATCH, pm_var(pv), NULL, NULL), pm_node(TT_ASSIGN, pm_var(ev), pm_var("&pos"), NULL));
                    ast_push(viap, pm_node(TT_ASSIGN, pm_var("&pos"), pm_var(sv), NULL));
                    ast_push(viap, pm_var(ev));
                    tree_t * sel = pm_node(TT_IF, pm_node(TT_LEQ, pm_call("type", pm_var(pv), NULL), pq, NULL), pm_node(TT_LIMIT, viap, pm_int(1), NULL), plain);
                    return lic_lower(cx, pvar ? sel : pm_node(TT_CONJ, pm_node(TT_ASSIGN, pm_var(pv), (tree_t *) t->c[1], NULL), sel, NULL), γ, ω, res);
                }
                int bal4 = fn && !strcmp(nm, "bal") && t->n >= 5 && t->n <= 7 && t->c[2] && t->c[3] && (t->c[2]->t == TT_CSET || t->c[2]->t == TT_QLIT) &&
                    (t->c[3]->t == TT_CSET || t->c[3]->t == TT_QLIT) && t->c[2]->v.sval && !strcmp(t->c[2]->v.sval, "(") && t->c[3]->v.sval && !strcmp(t->c[3]->v.sval, ")");
                if (fn && (bal4 || (icn_scan_window_name(nm) && t->n >= 3 && t->n <= 5)) && t->c[1] && t->c[bal4 ? 4 : 2] && !icn_is_proc(nm) && !icn_builtin_reassigned(nm)) {
                    const int sub = bal4 ? 4 : 2;
                    const char * tmp = icn_synth_local(cx);
                    tree_t * tv1 = ast_node_new(TT_VAR);
                    tv1->v.sval = (char *) tmp;
                    tree_t * asg = ast_node_new(TT_ASSIGN);
                    ast_push(asg, tv1);
                    ast_push(asg, (tree_t *) t->c[1]);
                    tree_t * tv2 = ast_node_new(TT_VAR);
                    tv2->v.sval = (char *) tmp;
                    tree_t * one = ast_node_new(TT_FNC);
                    if (!strcmp(nm, "match")) one->slen = -5;
                    ast_push(one, (tree_t *) fn);
                    ast_push(one, tv2);
                    tree_t * sc = ast_node_new(TT_SCAN);
                    ast_push(sc, (tree_t *) t->c[sub]);
                    ast_push(sc, one);
                    for (int w = sub + 1; w < t->n; w++) ast_push(sc, (tree_t *) t->c[w]);
                    if (t->n == sub + 1) { ast_push(sc, ast_node_new(TT_NUL)); ast_push(sc, ast_node_new(TT_NUL)); }
                    tree_t * cj = ast_node_new(TT_CONJ);
                    ast_push(cj, asg);
                    ast_push(cj, sc);
                    return lic_lower(cx, cj, γ, ω, res);
                }
                if (fn && !strcmp(nm, "variable") && t->n == 2 && t->c[1] && t->c[1]->t == TT_VAR && t->c[1]->v.sval && t->c[1]->v.sval[0] != '&' && !icn_is_proc(nm) && !icn_builtin_reassigned(nm) &&
                    !icn_is_local(cx, nm)) {
                    const char * kws[2] = { "&subject", "&pos" };
                    tree_t * alt = ast_node_new(TT_ALTERNATE);
                    for (int k = 0; k < 2; k++) {
                        tree_t * q = ast_node_new(TT_QLIT);
                        q->v.sval = (char *) kws[k];
                        q->slen = (int) strlen(kws[k]);
                        tree_t * eq = ast_node_new(TT_IDENTICAL);
                        ast_push(eq, (tree_t *) t->c[1]);
                        ast_push(eq, q);
                        tree_t * kv = ast_node_new(TT_VAR);
                        kv->v.sval = (char *) kws[k];
                        tree_t * cj = ast_node_new(TT_CONJ);
                        ast_push(cj, eq);
                        ast_push(cj, kv);
                        ast_push(alt, cj);
                    }
                    tree_t * par = ast_node_new(TT_SEQ_EXPR);
                    ast_push(par, (tree_t *) t->c[1]);
                    tree_t * call = ast_node_new(TT_FNC);
                    ast_push(call, (tree_t *) fn);
                    ast_push(call, par);
                    ast_push(alt, call);
                    return lic_lower(cx, alt, γ, ω, res);
                }
                if (!icn_call_yields_a_variable(cx, nm)) return lic_lower_call(cx, nm, t, 1, t->n - 1, γ, ω, res);
                IR_t * cr = NULL;
                IR_t * drf = lic_build(cx, IR_DEREF, γ, ω);
                IR_t * centry = lic_lower_call(cx, nm, t, 1, t->n - 1, drf, ω, &cr);
                IR_t * cβ = cx->beta;
                if (cβ && cβ != ω && cβ != drf) lic_ω_to(drf, cβ);
                ir_operand_push(drf, cr);
                *res = drf;
                return centry;
            }
            IR_t * cr = NULL;
            IR_t * ce = lic_lower(cx, fn, NULL, ω, &cr);
            IR_t * prevβ = cx->beta;
            IR_t * drf = lic_build(cx, IR_DEREF, γ, ω);
            IR_t * nd = lic_build(cx, IR_CALL_VALUE, drf, NULL);
            IR_t * prev = cr;
            ir_operand_push(nd, cr);
            for (int i = 1; i < t->n; i++) {
                IR_t * ar = NULL;
                IR_t * ae = NULL;
                if (icn_arg_stages(cx, t->c[i]) || (t->c[i] && t->c[i]->t == TT_ITERATE && t->c[i]->n > 0 && icn_arg_stages(cx, t->c[i]->c[0]))) {
                    cx->beta = prevβ ? prevβ : ω;
                    ae = lower_lvalue_var(cx, t->c[i], prevβ ? prevβ : ω, &ar);
                    if (ae && ar && ar->op == IR_VAR_REF) ar->pat_static = 1;
                }
                if (!ae || !ar) { if (i == t->n - 1) cx->after_line = (t->line > 0) ? t->line : fn->line; ae = lic_lower(cx, t->c[i], NULL, prevβ ? prevβ : ω, &ar); }
                prevβ = cx->beta;
                lc_γ_to(prev, ae);
                prev = ar;
                ir_operand_push(nd, ar);
            }
            lic_ω_to(nd, prevβ ? prevβ : ω);
            { long own = (t->line > 0) ? t->line : fn->line; long vline = (after_line != 0 && after_line != own) ? 0 : own; lc_γ_to(prev, (vline > 0) ? icn_line_mark(cx, (int) vline, nd) : nd); }
            lic_ω_to(drf, nd);
            ir_operand_push(drf, nd);
            cx->beta = nd;
            *res = drf;
            return ce;
        }
        case TT_RANDOM:
        {
            IR_t * rn = lic_build(cx, IR_RANDOM, NULL, ω);
            const tree_t * b0 = (t->n > 0) ? t->c[0] : NULL;
            if (!b0) { IR_t * f = lic_build(cx, IR_FAIL, γ, ω); *res = f; return f; }
            IR_t * ar = NULL;
            IR_t * ae;
            ae = lic_lower(cx, b0, NULL, ω, &ar);
            lc_γ_to(ar, rn);
            ir_operand_push(rn, ar);
            IR_t * drf = lic_build(cx, IR_DEREF, γ, ω);
            lc_γ_to(rn, drf);
            ir_operand_push(drf, rn);
            *res = drf;
            return ae;
        }
        case TT_IDX:
        {
            IR_t * vr = NULL;
            IR_t * e = lower_idx_var(cx, t, ω, &vr);
            IR_t * idxβ = cx->beta;
            if (vr && vr->op == IR_SUBSCRIPT && vr->n_operands == 2) vr->seal = IR_SEAL_SUBSCRIPT_VALUE;
            IR_t * drf = lic_build(cx, IR_DEREF, γ, idxβ ? idxβ : ω);
            lc_γ_to(vr, drf);
            ir_operand_push(drf, vr);
            *res = drf;
            return e;
        }
        case TT_DEREF:
        {
            const tree_t * b0 = (t->n > 0) ? t->c[0] : NULL;
            if (!b0) { IR_t * f = lic_build(cx, IR_FAIL, γ, ω); *res = f; return f; }
            IR_t * ar = NULL;
            IR_t * ae = lic_lower(cx, b0, NULL, ω, &ar);
            IR_t * drβ = cx->beta;
            IR_t * drf = lic_build(cx, IR_DEREF, γ, drβ ? drβ : ω);
            lc_γ_to(ar, drf);
            ir_operand_push(drf, ar);
            *res = drf;
            return ae;
        }
        case TT_MAKELIST:
        case TT_VLIST:
        return lower_make_list(cx, t, γ, ω, res);
        case TT_ASSIGN:
        {
            const tree_t * lhs = pm_fn_form(cx, t->c[0]);
            const tree_t * rhs = t->c[1];
            if (lhs && lhs->t == TT_PMATCH && lhs->n == 2 && lhs->c[0] && lhs->c[0]->t == TT_VAR && lhs->c[0]->v.sval && lhs->c[1] && rhs) return lower_pmatch_repl(cx, lhs, rhs, γ, ω, res);
            {
                const char * vn = icn_variable_lit(cx, lhs);
                if (vn) {
                    tree_t * tg = icn_variable_lit_target(cx, vn);
                    if (!tg) return lc_key(cx, t, "&fail", γ, ω, res);
                    tree_t * as = ast_node_new(TT_ASSIGN);
                    ast_push(as, tg);
                    ast_push(as, (tree_t *) rhs);
                    return lic_lower(cx, as, γ, ω, res);
                }
            }
            if (lhs && (lhs->t == TT_VAR || lhs->t == TT_KEYWORD) && lhs->v.sval && lhs->v.sval[0] == '&') {
                IR_t * ka = lic_build(cx, IR_KW_ASSIGN, γ, ω);
                IR_LIT(ka).sval = lhs->v.sval;
                IR_t * vr = NULL;
                IR_t * entry = lic_lower(cx, rhs, ka, ω, &vr);
                ir_operand_push(ka, vr);
                *res = ka;
                return entry;
            }
            if (lhs && lhs->t == TT_VAR) {
                IR_t * asn = lic_build(cx, IR_ASSIGN, γ, ω);
                IR_LIT(asn).sval = lhs->v.sval;
                IR_t * vtc = NULL;
                IR_t * vt = icn_trace_named_prep(cx, "__trace_value", lhs->v.sval, asn, ω, &vtc);
                IR_t * vr = NULL;
                IR_t * entry = lic_lower(cx, rhs, vt ? vt : asn, ω, &vr);
                ir_operand_push(asn, vr);
                if (vr && vtc) ir_operand_push(vtc, vr);
                *res = asn;
                return entry;
            }
            {
                int var_road = cx->var_road;
                cx->var_road = 0;
                IR_t * b4 = cx->beta;
                if (lhs && lhs->t == TT_IDX && lhs->n == 2 && lhs->c[0] && lhs->c[1] && !var_road && rhs && (icn_tree_is_literal(rhs) || (rhs->t == TT_VAR && rhs->v.sval && rhs->v.sval[0] != '&'))) {
                    const tree_t * b0 = lhs->c[0];
                    IR_t * br = NULL;
                    IR_t * be = lower_idx_base(cx, b0, ω, &br);
                    if (be && br) {
                        IR_t * prevβ = (b0->t == TT_VAR || icn_tree_is_kw_var(b0)) ? NULL : (b0->t == TT_IDX && cx->beta == b4) ? NULL : cx->beta;
                        IR_t * ir = NULL;
                        IR_t * ie = lic_lower(cx, lhs->c[1], NULL, prevβ ? prevβ : ω, &ir);
                        lc_γ_to(br, ie);
                        IR_t * lvbeta = (cx->beta != b4) ? cx->beta : NULL;
                        IR_t * asn = lic_build(cx, IR_ASSIGN_VAR, γ, ω);
                        IR_t * rr = NULL;
                        IR_t * re = lic_lower(cx, rhs, asn, lvbeta ? lvbeta : ω, &rr);
                        lc_γ_to(ir, re);
                        ir_operand_push(asn, br);
                        ir_operand_push(asn, ir);
                        ir_operand_push(asn, rr);
                        *res = asn;
                        return be;
                    }
                }
                IR_t * lv = NULL;
                IR_t * lve = lhs ? lower_lvalue_var(cx, lhs, ω, &lv) : NULL;
                if (lve && lv) {
                    icn_field_var_store_only(lv);
                    IR_t * lvbeta = (cx->beta != b4) ? cx->beta : NULL;
                    IR_t * asn = lic_build(cx, IR_ASSIGN_VAR, γ, ω);
                    IR_t * rr = NULL;
                    IR_t * re = lic_lower(cx, rhs, asn, lvbeta ? lvbeta : ω, &rr);
                    lc_γ_to(lv, re);
                    ir_operand_push(asn, lv);
                    ir_operand_push(asn, rr);
                    *res = asn;
                    return lve;
                }
            }
            if (getenv("SCRIP_ASSIGN_DIAG"))
                fprintf(stderr, "[ASSIGN-LV] nameless assign: lhs tree type %d, %d children, line %d\n", lhs ? (int) lhs->t : -1, lhs ? lhs->n : -1, lhs ? (int) lhs->line : -1);
            {
                tree_t * call = ast_node_new(TT_FNC);
                tree_t * fnv = ast_node_new(TT_VAR);
                fnv->v.sval = (char *) "runerr";
                ast_push(call, fnv);
                tree_t * code = ast_node_new(TT_ILIT);
                code->v.ival = 111;
                ast_push(call, code);
                ast_push(call, (tree_t *) lhs);
                tree_t * seq = ast_node_new(TT_CONJ);
                ast_push(seq, (tree_t *) rhs);
                ast_push(seq, call);
                return lic_lower(cx, seq, γ, ω, res);
            }
        }
        case TT_AUGOP:
        {
            const tree_t * lhs = t->c[0];
            const tree_t * rhs = t->c[1];
            if ((int) t->v.ival == AUGOP_SCAN && lhs && rhs && (lhs->t == TT_ALTERNATE || lhs->t == TT_IDX || lhs->t == TT_FIELD)) {
                IR_t * asn = lic_build(cx, IR_ASSIGN_VAR, γ, ω);
                IR_t * sres = NULL;
                IR_t * lv = NULL;
                IR_t * entry = lower_scan_impl(cx, NULL, lhs, rhs, NULL, NULL, asn, ω, &sres, &lv);
                if (entry && lv && sres) { ir_operand_push(asn, lv); ir_operand_push(asn, sres); *res = asn; return entry; }
            }
            if ((int) t->v.ival == AUGOP_SCAN && lhs && rhs) {
                tree_t * scn = ast_node_new(TT_SCAN);
                ast_push(scn, (tree_t *) lhs);
                ast_push(scn, (tree_t *) rhs);
                tree_t * as = ast_node_new(TT_ASSIGN);
                ast_push(as, (tree_t *) lhs);
                ast_push(as, scn);
                as->line = t->line;
                return lic_lower(cx, as, γ, ω, res);
            }
            if (lhs && lhs->t == TT_VAR && rhs && augop_binop_tt((int) t->v.ival) != (tree_e) 0) {
                tree_t * bo = ast_node_new(augop_binop_tt((int) t->v.ival));
                ast_push(bo, (tree_t *) lhs);
                ast_push(bo, (tree_t *) rhs);
                bo->line = t->line;
                tree_t * as = ast_node_new(TT_ASSIGN);
                ast_push(as, (tree_t *) lhs);
                ast_push(as, bo);
                as->line = t->line;
                return lic_lower(cx, as, γ, ω, res);
            }
            if ((int) t->v.ival == AUGOP_CONJ && lhs && rhs) { tree_t * as = ast_node_new(TT_ASSIGN); ast_push(as, (tree_t *) lhs); ast_push(as, (tree_t *) rhs); return lic_lower(cx, as, γ, ω, res); }
            int aop = (int) t->v.ival;
            {
                IR_t * b4 = cx->beta;
                IR_t * lv = NULL;
                IR_t * lve = (lhs && rhs) ? lower_lvalue_var(cx, lhs, ω, &lv) : NULL;
                if (lve && lv) {
                    icn_field_var_store_only(lv);
                    IR_t * asn = lic_build(cx, IR_ASSIGN_VAR, γ, ω);
                    IR_t * op = NULL, * ca = NULL, * cb = NULL;
                    IR_t * dr = lic_build(cx, IR_DEREF, icn_augop_build(cx, aop, asn, ω, &op, &ca, &cb), ω);
                    ir_operand_push(dr, lv);
                    IR_t * lvbeta = (cx->beta != b4) ? cx->beta : NULL;
                    IR_t * rω = lvbeta ? lvbeta : ω;
                    IR_t * rr = NULL;
                    IR_t * re = lic_lower(cx, rhs, dr, rω, &rr);
                    lc_γ_to(lv, re);
                    { IR_t * rβ = cx->beta; if (rβ && rβ != rω && rβ != op && rβ != dr) { lic_ω_to(op, rβ); cx->beta = rβ; } else if (lvbeta) { lic_ω_to(op, lvbeta); cx->beta = lvbeta; } }
                    icn_augop_operands(aop, op, ca, cb, dr, rr);
                    ir_operand_push(asn, lv);
                    ir_operand_push(asn, op);
                    *res = asn;
                    return lve;
                }
            }
            {
                IR_t * b4 = cx->beta;
                IR_t * lr = NULL;
                IR_t * ea = (lhs && rhs) ? lic_lower(cx, lhs, NULL, ω, &lr) : NULL;
                if (ea && lr) {
                    IR_t * lvbeta = (cx->beta != b4) ? cx->beta : NULL;
                    IR_t * asn = lic_build(cx, IR_ASSIGN_VAR, γ, ω);
                    IR_t * op = NULL, * ca = NULL, * cb = NULL;
                    IR_t * dr = lic_build(cx, IR_DEREF, icn_augop_build(cx, aop, asn, ω, &op, &ca, &cb), ω);
                    ir_operand_push(dr, lr);
                    IR_t * rω = lvbeta ? lvbeta : ω;
                    IR_t * rr = NULL;
                    IR_t * re = lic_lower(cx, rhs, dr, rω, &rr);
                    lc_γ_to(lr, re);
                    { IR_t * rβ = cx->beta; if (rβ && rβ != rω && rβ != op && rβ != dr) { lic_ω_to(op, rβ); cx->beta = rβ; } else if (lvbeta) { lic_ω_to(op, lvbeta); cx->beta = lvbeta; } }
                    icn_augop_operands(aop, op, ca, cb, dr, rr);
                    ir_operand_push(asn, lr);
                    ir_operand_push(asn, op);
                    *res = asn;
                    return ea;
                }
            }
            { IR_t * nd = lic_build(cx, IR_FAIL, γ, ω); *res = nd; return nd; }
        }
        case TT_RETURN:
        {
            IR_t * ret = lic_build(cx, IR_RETURN, cx->psucc ? cx->psucc : γ, ω);
            IR_t * rtc = NULL;
            IR_t * rtn = icn_trace_named_prep(cx, "__trace_return", cx->pname, ret, ω, &rtc);
            IR_t * vtgt = rtn ? rtn : ret;
            int kwv = cx->scan_sp > 0 && t->n > 0 && t->c[0] && icn_tree_is_kw_scan(t->c[0]);
            IR_t * kvr = NULL;
            IR_t * kent = kwv ? lic_lower(cx, t->c[0], vtgt, cx->pfail ? cx->pfail : ω, &kvr) : NULL;
            if (cx->scan_sp > 0) {
                IR_t * tgt = kwv ? kent : ret;
                for (int _k = 0; _k < cx->scan_sp; _k++) { IR_t * lv = lic_build(cx, IR_SCAN, NULL, NULL); ir_operand_push(lv, icn_scan_at(cx, _k)); lc_γ_to(lv, tgt); lc_ω_to(lv, tgt); tgt = lv; }
                IR_t * tramp = IR_node_alloc(cx->g, IR_GOTO);
                lc_γ_to(tramp, tgt);
                lc_ω_to(tramp, tgt);
                vtgt = tramp;
            }
            if (t->n > 0 && t->c[0]) {
                IR_t * vr = kwv ? kvr : NULL;
                IR_t * entry = kwv ? (cx->scan_sp > 0 ? vtgt : kent) : lower_result_operand(cx, t->c[0], vtgt, cx->pfail ? cx->pfail : ω, &vr);
                ir_operand_push(ret, vr);
                if (vr && rtc) ir_operand_push(rtc, vr);
                *res = ret;
                return (cx->want_lines && t->line > 0) ? icn_line_hook(cx, t->line, entry) : entry;
            }
            *res = ret;
            return (cx->want_lines && t->line > 0) ? icn_line_hook(cx, t->line, vtgt) : vtgt;
        }
        case TT_PROC_FAIL:
        { IR_t * nd = lic_build(cx, IR_FAIL, γ, ω); *res = nd; return nd; }
        case TT_LOOP_BREAK:
        {
            int k = 1;
            const tree_t * ch = (t->n >= 1) ? t->c[0] : NULL;
            while (ch && ch->t == TT_LOOP_BREAK) { k++; ch = (ch->n >= 1) ? ch->c[0] : NULL; }
            if (!ch) {
                int idx = cx->loop_sp - k;
                icn_loop_fr * lfk = icn_loop_at(cx, idx);
                IR_t * lx = lfk ? lfk->exit : NULL;
                if (!lx) lx = cx->loop_exit;
                if (!lx) { IR_t * nd = lic_build(cx, IR_FAIL, γ, ω); *res = nd; return nd; }
                IR_t * nullv = lic_build(cx, IR_VAR, NULL, lx);
                IR_LIT(nullv).sval = (char *) "&null";
                IR_t * gt = lic_build(cx, IR_GOTO, lx, lx);
                IR_t * asn0 = lic_build(cx, IR_ASSIGN, gt, gt);
                IR_LIT(asn0).sval = (char *) "__break_result";
                ir_operand_push(asn0, nullv);
                lic_γ_to(nullv, asn0);
                if (cx->scan_sp > cx->loop_next_ssp) {
                    IR_t * tgt = nullv;
                    for (int _k = cx->loop_next_ssp; _k < cx->scan_sp; _k++) {
                        IR_t * lv = lic_build(cx, IR_SCAN, NULL, NULL);
                        icn_mark_γ_fail_conduit(lv);
                        IR_LIT(lv).dval = 3.0;
                        ir_operand_push(lv, icn_scan_at(cx, _k));
                        lc_γ_to(lv, tgt);
                        lc_ω_to(lv, tgt);
                        tgt = lv;
                    }
                    *res = gt;
                    return tgt;
                }
                *res = gt;
                return nullv;
            } else {
                IR_t * cur_exit = cx->loop_exit, * cur_fail = cx->loop_fail;
                if (k > 1) { icn_loop_fr * lfx = icn_loop_at(cx, cx->loop_sp - k); if (lfx) { cur_exit = lfx->exit; cur_fail = lfx->fail; } }
                IR_t * exit_goto = cur_exit ? lic_build(cx, IR_GOTO, cur_exit, cur_exit) : lic_build(cx, IR_FAIL, γ, ω);
                IR_t * fail_goto = cur_fail ? lic_build(cx, IR_GOTO, cur_fail, cur_fail) : exit_goto;
                int idx = cx->loop_sp - 1 - k;
                IR_t * se = cx->loop_exit, * sn = cx->loop_next, * sf = cx->loop_fail;
                { icn_loop_fr * lfi = icn_loop_at(cx, idx); if (lfi) { cx->loop_exit = lfi->exit; cx->loop_next = lfi->next; cx->loop_fail = lfi->fail; } }
                IR_t * ev = NULL;
                IR_t * en = lic_lower(cx, ch, NULL, fail_goto, &ev);
                cx->loop_exit = se;
                cx->loop_next = sn;
                cx->loop_fail = sf;
                IR_t * asn = lic_build(cx, IR_ASSIGN, exit_goto, exit_goto);
                IR_LIT(asn).sval = (char *) "__break_result";
                { IR_t * evf = icn_arm_result(ev); if (evf) { ir_operand_push(asn, evf); lic_γ_to(evf, asn); } else if (!ev) lic_γ_to(en, asn); }
                cx->loop_break_beta = cx->beta;
                if (cx->scan_sp > cx->loop_next_ssp) {
                    IR_t * tgt = en;
                    for (int _k = cx->loop_next_ssp; _k < cx->scan_sp; _k++) {
                        IR_t * lv = lic_build(cx, IR_SCAN, NULL, NULL);
                        icn_mark_γ_fail_conduit(lv);
                        IR_LIT(lv).dval = 3.0;
                        ir_operand_push(lv, icn_scan_at(cx, _k));
                        lc_γ_to(lv, tgt);
                        lc_ω_to(lv, tgt);
                        tgt = lv;
                    }
                    *res = exit_goto;
                    return tgt;
                }
                *res = exit_goto;
                return en;
            }
        }
        case TT_LOOP_NEXT:
        {
            IR_t * ln = cx->loop_next;
            IR_t * nd;
            if (ln && cx->scan_sp > cx->loop_next_ssp) {
                for (int _k = cx->loop_next_ssp; _k < cx->scan_sp; _k++) {
                    IR_t * lv = lic_build(cx, IR_SCAN, NULL, NULL);
                    icn_mark_γ_fail_conduit(lv);
                    ir_operand_push(lv, icn_scan_at(cx, _k));
                    lc_γ_to(lv, ln);
                    lc_ω_to(lv, ln);
                    ln = lv;
                }
                nd = lic_build(cx, IR_GOTO, NULL, NULL);
                lc_γ_to(nd, ln);
                lc_ω_to(nd, ln);
            } else if (ln) nd = lic_build(cx, IR_GOTO, ln, ln);
            else return lc_key(cx, t, "&fail", γ, ω, res);
            *res = nd;
            return nd;
        }
        case TT_LOCAL:
        case TT_STATIC_DECL:
        { IR_t * s = lic_build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
        case TT_INITIAL:
        {
            IR_t * ini = lic_build(cx, IR_INITIAL, NULL, γ);
            if (t->n > 0 && t->c[0]) { IR_t * br = NULL; IR_t * be = lic_lower(cx, t->c[0], γ, γ, &br); lc_γ_to(ini, be); ir_operand_push(ini, br); } else { lc_γ_to(ini, γ); }
            *res = ini;
            return ini;
        }
        case TT_SUSPEND:
        {
            IR_t * sn = lic_build(cx, IR_SUSPEND, cx->psucc ? cx->psucc : γ, ω);
            IR_LIT(sn).dval = 1.0;
            IR_LIT(sn).sval = (char *) cx->pname;
            bb_src_note(sn, "suspend", t->line);
            IR_t * ev = NULL;
            IR_t * e_entry = sn;
            IR_t * eβ = NULL;
            int kwv = cx->scan_sp > 0 && t->n > 0 && t->c[0] && icn_tree_is_kw_scan(t->c[0]);
            if (t->n > 0 && t->c[0]) {
                cx->beta = ω;
                if (!kwv && icn_trace_operand_has_var(cx, t->c[0])) sn->pat_static = 1;
                e_entry = kwv ? lic_lower(cx, t->c[0], sn, ω, &ev) : lower_trace_operand(cx, t->c[0], sn, ω, &ev);
                if (cx->beta && cx->beta != ω) eβ = cx->beta;
            }
            ir_operand_push(sn, ev);
            IR_t * rrt;
            if (t->n > 1 && t->c[1]) { IR_t * dv = NULL; rrt = lic_lower(cx, t->c[1], eβ ? eβ : ω, eβ ? eβ : ω, &dv); sn->seal = 1; } else rrt = eβ ? eβ : ω;
            if (cx->scan_sp > 0) {
                IR_t * ytgt = cx->psucc ? cx->psucc : γ;
                IR_t * rrt0 = rrt;
                IR_t * lvs[cx->scan_sp];
                int nlv = 0;
                for (int _k = cx->scan_sp - 1; _k >= 0; _k--) {
                    IR_t * lv = lic_build(cx, IR_SCAN, NULL, NULL);
                    IR_LIT(icn_scan_at(cx, _k)).dval = 3.0;
                    ir_operand_push(lv, icn_scan_at(cx, _k));
                    ir_operand_push(lv, NULL);
                    ir_operand_push(lv, NULL);
                    if (nlv) { lc_γ_to(lvs[nlv - 1], lv); lc_ω_to(lvs[nlv - 1], lv); }
                    lvs[nlv++] = lv;
                }
                lc_γ_to(lvs[nlv - 1], kwv ? e_entry : ytgt);
                lc_ω_to(lvs[nlv - 1], rrt0);
                for (int _i = 0; _i < nlv; _i++) lvs[_i]->operands[2] = (_i > 0) ? lvs[_i - 1] : rrt;
                lc_γ_to(sn, kwv ? ytgt : lvs[0]);
                rrt = lvs[nlv - 1];
                if (kwv) e_entry = lvs[0];
            }
            ir_operand_push(sn, rrt);
            *res = sn;
            return e_entry;
        }
        case TT_CASE:
        return lower_case(cx, t, γ, ω, res);
        case TT_CONJ:
        case TT_SEQ_EXPR:
        return lower_seq_impl(cx, t, γ, ω, res, 0);
        case TT_SECTION:
        case TT_SECTION_PLUS:
        case TT_SECTION_MINUS:
        {
            if (t->n < 3 || !t->c[0] || !t->c[1] || !t->c[2]) { IR_t * s = lic_build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
            int sec_variant = (t->t == TT_SECTION_PLUS) ? 1 : (t->t == TT_SECTION_MINUS) ? 2 : 0;
            IR_t * sec = lic_build(cx, IR_SUBSCRIPT, γ, ω);
            if (sec_variant == 1) IR_LIT(sec).sval = "+";
            else if (sec_variant == 2) IR_LIT(sec).sval = "-";
            IR_t * ar = NULL;
            IR_t * ae = lic_lower(cx, t->c[0], NULL, ω, &ar);
            IR_t * aβ = cx->beta;
            IR_t * ωa = (aβ && aβ != ω && aβ != sec) ? aβ : ω;
            IR_t * br = NULL;
            IR_t * be = lic_lower(cx, t->c[1], NULL, ωa, &br);
            lc_γ_to(ar, be);
            IR_t * bβ = cx->beta;
            IR_t * ωb = (bβ && bβ != ωa && bβ != sec) ? bβ : ωa;
            IR_t * cr = NULL;
            IR_t * ce = lic_lower(cx, t->c[2], sec_variant ? NULL : sec, ωb, &cr);
            lc_γ_to(br, ce);
            IR_t * cβ = cx->beta;
            IR_t * ωc = (cβ && cβ != ωb && cβ != sec) ? cβ : ωb;
            if (sec_variant) {
                IR_t * op = lic_build(cx, IR_BINOP, sec, ωc);
                IR_LIT(op).ival = (sec_variant == 1) ? BINOP_ADD : BINOP_SUB;
                ir_operand_push(op, br);
                ir_operand_push(op, cr);
                lic_γ_to(cr, op);
                cr = op;
            }
            ir_operand_push(sec, ar);
            ir_operand_push(sec, br);
            ir_operand_push(sec, cr);
            if (ωc != ω) lic_ω_to(sec, ωc);
            cx->beta = ωc;
            *res = sec;
            return ae;
        }
        case TT_NOT:
        return lower_not(cx, t, γ, ω, res);
        case TT_ALTERNATE:
        return lower_alt(cx, t, γ, ω, res);
        case TT_ITERATE:
        {
            IR_t * nd = lic_build(cx, IR_ITERATE, γ, ω);
            if (cx->prog && cx->prog->v.ival) IR_LIT(nd).sval = (char *) "ubang";
            IR_t * orr = NULL;
            IR_t * ee = lic_lower(cx, (t->n > 0) ? t->c[0] : NULL, NULL, ω, &orr);
            IR_t * opβ = cx->beta;
            ir_operand_push(nd, orr);
            lc_γ_to(orr, nd);
            if (opβ && opβ != ω) lc_ω_to_β(nd, opβ);
            cx->beta = nd;
            *res = nd;
            return ee;
        }
        case TT_IF:
        return lic_lower_if(cx, t, γ, ω, res);
        case TT_WHILE:
        return lic_lower_while(cx, t, γ, ω, res);
        case TT_UNTIL:
        return lower_until(cx, t, γ, ω, res);
        case TT_REPEAT:
        return lic_lower_repeat(cx, t, γ, ω, res);
        case TT_TO:
        case TT_TO_BY:
        return lower_to(cx, t, γ, ω, res);
        case TT_EVERY:
        return lower_every(cx, t, γ, ω, res);
        case TT_SCAN:
        {
            if (t->n < 2 || !t->c[0] || !t->c[1]) { IR_t * gs = lic_build(cx, IR_FAIL, γ, ω); *res = gs; return gs; }
            IR_t * sc = lower_scan_impl(cx, t->c[0], NULL, t->c[1], t->n > 2 ? t->c[2] : NULL, t->n > 3 ? t->c[3] : NULL, γ, ω, res, NULL);
            if (sc) return sc;
            { IR_t * gs = lic_build(cx, IR_FAIL, γ, ω); *res = gs; return gs; }
        }
        case TT_STMT:
        { const tree_t * sub = lic_stmt_subj(t); if (sub) return lic_lower(cx, sub, γ, ω, res); IR_t * s = lic_build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
        case TT_CREATE:
        {
            IR_t * nd = lic_build(cx, IR_CREATE, γ, ω);
            IR_LIT(nd).sval = cx->pname;
            IR_t * coret = lic_build(cx, IR_CORET, NULL, NULL);
            IR_t * cofail = lic_build(cx, IR_COFAIL, NULL, NULL);
            IR_t * bv = NULL;
            IR_t * b_entry = lic_lower(cx, (t->n > 0) ? t->c[0] : NULL, coret, cofail, &bv);
            IR_t * body_beta = cx->beta;
            if (body_beta) lic_γ_to(coret, body_beta);
            ir_operand_push(coret, bv);
            ir_operand_push(nd, b_entry);
            cx->beta = ω;
            *res = nd;
            return nd;
        }
        case TT_ACTIVATE:
        {
            IR_t * nd = lic_build(cx, IR_ACTIVATE, γ, ω);
            IR_LIT(nd).sval = cx->pname;
            const tree_t * xt = (t->n > 1) ? t->c[0] : NULL;
            const tree_t * ct = (t->n > 1) ? t->c[1] : t->c[0];
            IR_t * b4 = cx->beta;
            IR_t * cr = NULL;
            IR_t * c_entry = lic_lower(cx, ct, nd, ω, &cr);
            IR_t * cβ = (cx->beta != b4 && cx->beta != ω) ? cx->beta : NULL;
            if (cβ) lic_ω_to(nd, cβ);
            IR_t * entry = c_entry;
            ir_operand_push(nd, cr);
            if (xt) { IR_t * xr = NULL; entry = lic_lower(cx, xt, c_entry, ω, &xr); ir_operand_push(nd, xr); }
            if (cx->want_lines && t->line > 0) entry = icn_line_hook(cx, t->line, entry);
            cx->beta = cβ ? cβ : ω;
            *res = nd;
            return entry;
        }
        case TT_REPALT:
        {
            IR_graph_t * g = cx->g;
            const char * fl = icn_synth_local(cx);
            IR_t * nv = lic_build(cx, IR_VAR, NULL, NULL);
            IR_LIT(nv).sval = (char *) "&null";
            IR_t * a0 = lic_build(cx, IR_ASSIGN, NULL, ω);
            IR_LIT(a0).sval = (char *) fl;
            ir_operand_push(a0, nv);
            lic_γ_to(nv, a0);
            IR_t * dj = lc_build(g, IR_DISJUNCTION, NULL, NULL);
            lic_γ_to(dj, γ);
            lc_γ_to(a0, dj);
            int before = g->n;
            IR_t * one = lic_build(cx, IR_LIT_INTEGER, NULL, NULL);
            IR_LIT(one).ival = 1;
            IR_t * a1 = lic_build(cx, IR_ASSIGN, dj, dj);
            IR_LIT(a1).sval = (char *) fl;
            ir_operand_push(a1, one);
            lic_γ_to(one, a1);
            cx->beta = dj;
            IR_t * er = NULL;
            IR_t * ee = lic_lower(cx, (t->n > 0) ? t->c[0] : NULL, one, dj, &er);
            IR_t * ab = cx->beta;
            int ab_in_arm = 0;
            if (ab && ab != dj) for (int k = before; k < g->n; k++) if (g->all[k] == ab) { ab_in_arm = 1; break; }
            for (int k = before; k < g->n; k++) {
                IR_t * x = g->all[k];
                if (!x) continue;
                if (x->ω.node == dj) { memcpy(x->ω.sz, "φ", 3); x->ω.sz[3] = 0; }
                if (x->γ.node == dj) { if ((x->op == IR_GOTO && x->ω.node == dj) || icn_γ_is_fail_conduit(x)) { memcpy(x->γ.sz, "φ", 3); } else { memcpy(x->γ.sz, "σ", 3); } x->γ.sz[3] = 0; }
            }
            ir_operand_push(dj, ee);
            ir_operand_push(dj, ab_in_arm ? ab : dj);
            ir_operand_push(dj, icn_arm_result(er));
            IR_LIT(dj).ival = 1;
            IR_t * tv = lic_build(cx, IR_VAR, NULL, NULL);
            IR_LIT(tv).sval = (char *) fl;
            IR_t * tt = lic_build(cx, IR_UNOP_TEST, nv, ω);
            IR_LIT(tt).ival = (long long) TT_NONNULL;
            ir_operand_push(tt, tv);
            lic_γ_to(tv, tt);
            lic_ω_to(dj, tv);
            cx->beta = dj;
            *res = dj;
            return nv;
        }
        case TT_LIMIT:
        return lower_limit_impl(cx, t, γ, ω, res, 0);
        case TT_LCONCAT:
        {
            if (t->n < 2 || !t->c[0] || !t->c[1]) { IR_t * s = lic_build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
            IR_t * op = lic_build(cx, IR_BINOP, γ, ω);
            IR_LIT(op).ival = BINOP_LCONCAT;
            IR_t * lr = NULL;
            IR_t * ee = lic_lower(cx, t->c[0], NULL, ω, &lr);
            IR_t * lβ = cx->beta;
            IR_t * rr = NULL;
            IR_t * re = lic_lower(cx, t->c[1], op, lβ, &rr);
            lc_γ_to(lr, re);
            ir_operand_push(op, lr);
            ir_operand_push(op, rr);
            *res = op;
            return ee;
        }
        case TT_SWAP:
        {
            const tree_t * lt = (t->n > 0) ? t->c[0] : NULL;
            const tree_t * rt2 = (t->n > 1) ? t->c[1] : NULL;
            if (icn_tree_is_literal(lt)) return lower_runerr_111(cx, rt2, lt, γ, ω, res);
            if (icn_tree_is_literal(rt2)) return lower_runerr_111(cx, lt, rt2, γ, ω, res);
            int plain_l = lt && lt->t == TT_VAR && lt->v.sval && lt->v.sval[0] != '&' && icn_is_local(cx, lt->v.sval);
            int plain_r = rt2 && rt2->t == TT_VAR && rt2->v.sval && rt2->v.sval[0] != '&' && icn_is_local(cx, rt2->v.sval);
            int kw_l = lt && (lt->t == TT_VAR || lt->t == TT_KEYWORD) && lt->v.sval && lt->v.sval[0] == '&';
            int kw_r = rt2 && (rt2->t == TT_VAR || rt2->t == TT_KEYWORD) && rt2->v.sval && rt2->v.sval[0] == '&';
            if (kw_l && kw_r) {
                IR_t * lv_old = lic_build(cx, IR_KW_ICON, NULL, ω);
                IR_LIT(lv_old).sval = (char *) lt->v.sval;
                IR_t * rv_old = lic_build(cx, IR_KW_ICON, NULL, ω);
                IR_LIT(rv_old).sval = (char *) rt2->v.sval;
                lc_γ_to(lv_old, rv_old);
                IR_t * write_l = lic_build(cx, IR_KW_ASSIGN, NULL, ω);
                IR_LIT(write_l).sval = (char *) lt->v.sval;
                ir_operand_push(write_l, rv_old);
                IR_t * write_r = lic_build(cx, IR_KW_ASSIGN, γ, ω);
                IR_LIT(write_r).sval = (char *) rt2->v.sval;
                ir_operand_push(write_r, lv_old);
                lc_γ_to(rv_old, write_l);
                lc_γ_to(write_l, write_r);
                *res = write_l;
                return lv_old;
            }
            if (kw_l || kw_r) {
                IR_t * kv_old = lic_build(cx, IR_KW_ICON, NULL, ω);
                IR_t * pv_old = lic_build(cx, IR_VAR, NULL, ω);
                const tree_t * kw_tree = kw_l ? lt : rt2;
                const tree_t * pl_tree = kw_l ? rt2 : lt;
                IR_LIT(kv_old).sval = (char *) kw_tree->v.sval;
                IR_LIT(pv_old).sval = (char *) pl_tree->v.sval;
                lc_γ_to(kv_old, pv_old);
                if (kw_l) {
                    IR_t * write_kw = lic_build(cx, IR_KW_ASSIGN, NULL, ω);
                    IR_LIT(write_kw).sval = kw_tree->v.sval;
                    ir_operand_push(write_kw, pv_old);
                    IR_t * write_plain = lic_build(cx, IR_ASSIGN, γ, ω);
                    IR_LIT(write_plain).sval = pl_tree->v.sval;
                    ir_operand_push(write_plain, kv_old);
                    lc_γ_to(pv_old, write_kw);
                    lc_γ_to(write_kw, write_plain);
                    *res = write_kw;
                    return kv_old;
                } else {
                    const char * snap_nm = icn_synth_local(cx);
                    IR_t * snap = lic_build(cx, IR_ASSIGN, NULL, ω);
                    IR_LIT(snap).sval = (char *) snap_nm;
                    ir_operand_push(snap, pv_old);
                    IR_t * snap_v = lic_build(cx, IR_VAR, NULL, NULL);
                    IR_LIT(snap_v).sval = (char *) snap_nm;
                    IR_t * write_plain = lic_build(cx, IR_ASSIGN, NULL, ω);
                    IR_LIT(write_plain).sval = pl_tree->v.sval;
                    ir_operand_push(write_plain, kv_old);
                    IR_t * write_kw = lic_build(cx, IR_KW_ASSIGN, γ, ω);
                    IR_LIT(write_kw).sval = kw_tree->v.sval;
                    ir_operand_push(write_kw, snap_v);
                    lc_γ_to(pv_old, snap);
                    lc_γ_to(snap, write_plain);
                    lc_γ_to(write_plain, snap_v);
                    lc_γ_to(snap_v, write_kw);
                    *res = write_plain;
                    return kv_old;
                }
            }
            if (!(plain_l && plain_r)) {
                IR_t * b4 = cx->beta;
                IR_t * xr = NULL;
                IR_t * xe = lower_lvalue_var(cx, lt, ω, &xr);
                IR_t * xbeta = (cx->beta != b4) ? cx->beta : NULL;
                IR_t * yr = NULL;
                IR_t * ye = xe ? lower_lvalue_var(cx, rt2, xbeta ? xbeta : ω, &yr) : NULL;
                if (xe && ye) { lc_γ_to(xr, ye); IR_t * nd = lic_build(cx, IR_SWAP_VAR, γ, ω); lc_γ_to(yr, nd); ir_operand_push(nd, xr); ir_operand_push(nd, yr); *res = nd; return xe; }
            }
            IR_t * nd = lic_build(cx, IR_SWAP, γ, ω);
            IR_t * r_tc = NULL;
            IR_t * r_vt = (rt2 && rt2->t == TT_VAR) ? icn_trace_named_prep(cx, "__trace_value", rt2->v.sval, nd, ω, &r_tc) : NULL;
            IR_t * lr = NULL;
            IR_t * lr_entry = lic_lower(cx, lt, r_vt ? r_vt : nd, ω, &lr);
            if (r_tc && lr) ir_operand_push(r_tc, lr);
            IR_t * l_tc = NULL;
            IR_t * l_vt = (lt && lt->t == TT_VAR) ? icn_trace_named_prep(cx, "__trace_value", lt->v.sval, lr_entry, ω, &l_tc) : NULL;
            IR_t * rr = NULL;
            IR_t * rr_entry = lic_lower(cx, rt2, l_vt ? l_vt : lr_entry, ω, &rr);
            if (l_tc && rr) ir_operand_push(l_tc, rr);
            ir_operand_push(nd, lr);
            ir_operand_push(nd, rr);
            *res = nd;
            return rr_entry;
        }
        case TT_REVASSIGN:
        {
            const tree_t * lhs = t->c[0];
            const tree_t * rhs = (t->n > 1) ? t->c[1] : NULL;
            if (lhs &&
                (lhs->t == TT_IDX || lhs->t == TT_ITERATE || lhs->t == TT_SECTION || lhs->t == TT_SECTION_PLUS || lhs->t == TT_SECTION_MINUS || lhs->t == TT_FIELD || lhs->t == TT_RANDOM ||
                lhs->t == TT_NULL || lhs->t == TT_NONNULL || lhs->t == TT_CONJ || lhs->t == TT_ASSIGN || lhs->t == TT_SWAP || lhs->t == TT_REVSWAP || lhs->t == TT_REVASSIGN || lhs->t == TT_AUGOP)) {
                IR_t * b4 = cx->beta;
                IR_t * vr = NULL;
                IR_t * entry = lower_lvalue_var(cx, lhs, ω, &vr);
                IR_t * lvbeta = (cx->beta != b4) ? cx->beta : NULL;
                IR_t * nd = lic_build(cx, IR_REV_ASSIGN_VAR, γ, lvbeta ? lvbeta : ω);
                IR_t * b5 = cx->beta;
                IR_t * rr = NULL;
                IR_t * re = lic_lower(cx, rhs, NULL, lvbeta ? lvbeta : ω, &rr);
                IR_t * rbeta = (cx->beta != b5) ? cx->beta : NULL;
                if (rbeta) lic_ω_to(nd, rbeta);
                lc_γ_to(vr, re);
                lc_γ_to(rr, nd);
                ir_operand_push(nd, vr);
                ir_operand_push(nd, rr);
                cx->beta = nd;
                *res = nd;
                return entry;
            }
            if (lhs && lhs->t == TT_VAR) {
                IR_t * nd = lic_build(cx, IR_REV_ASSIGN, γ, ω);
                IR_t * lr = NULL;
                IR_t * le = lic_lower(cx, lhs, NULL, ω, &lr);
                IR_t * b4 = cx->beta;
                IR_t * rr = NULL;
                IR_t * re = lic_lower(cx, rhs, NULL, ω, &rr);
                IR_t * rbeta = (cx->beta != b4) ? cx->beta : NULL;
                if (rbeta) lic_ω_to(nd, rbeta);
                else if (rr && icn_tree_is_cursor_mover(rhs)) lc_ω_to_β(nd, rr);
                IR_t * tvc = NULL;
                IR_t * tv = icn_trace_named_prep(cx, "__trace_value", (lhs->v.sval && lhs->v.sval[0] != '&') ? lhs->v.sval : NULL, nd, ω, &tvc);
                if (tvc) lc_γ_to(tvc, nd);
                lc_γ_to(lr, re);
                lc_γ_to(rr, tv ? tv : nd);
                if (tv && rr) ir_operand_push(tvc, rr);
                ir_operand_push(nd, rr);
                ir_operand_push(nd, lr);
                cx->beta = nd;
                *res = nd;
                return le;
            }
            if (icn_tree_is_literal(lhs)) return lower_runerr_111(cx, rhs, lhs, γ, ω, res);
            IR_t * nd = lic_build(cx, IR_FAIL, γ, ω);
            IR_t * lr = NULL;
            lic_lower(cx, lhs, nd, ω, &lr);
            IR_t * rr = NULL;
            lic_lower(cx, rhs, nd, ω, &rr);
            ir_operand_push(nd, lr);
            ir_operand_push(nd, rr);
            *res = nd;
            return nd;
        }
        case TT_REVSWAP:
        {
            const tree_t * lt = (t->n > 0) ? t->c[0] : NULL;
            const tree_t * rt2 = (t->n > 1) ? t->c[1] : NULL;
            int name_l = lt && (lt->t == TT_VAR || lt->t == TT_KEYWORD) && lt->v.sval;
            int name_r = rt2 && (rt2->t == TT_VAR || rt2->t == TT_KEYWORD) && rt2->v.sval;
            if (name_l && name_r) {
                IR_t * nd = lic_build(cx, IR_REV_SWAP, γ, ω);
                IR_LIT(nd).sval = (char *) lt->v.sval;
                IR_t * rc = lic_build(cx, IR_LIT_STRING, NULL, NULL);
                IR_LIT(rc).sval = (char *) rt2->v.sval;
                ir_operand_push(nd, rc);
                cx->beta = nd;
                *res = nd;
                return nd;
            }
            if (icn_tree_is_literal(lt)) return lower_runerr_111(cx, rt2, lt, γ, ω, res);
            if (icn_tree_is_literal(rt2)) return lower_runerr_111(cx, lt, rt2, γ, ω, res);
            {
                IR_t * b0 = cx->beta;
                IR_t * xr = NULL;
                IR_t * xe = lower_lvalue_var(cx, lt, ω, &xr);
                IR_t * xbeta = (cx->beta != b0) ? cx->beta : NULL;
                IR_t * yr = NULL;
                IR_t * ye = xe ? lower_lvalue_var(cx, rt2, xbeta ? xbeta : ω, &yr) : NULL;
                IR_t * ybeta = (cx->beta != (xbeta ? xbeta : b0)) ? cx->beta : NULL;
                if (xe && ye) {
                    IR_t * back = ybeta ? ybeta : (xbeta ? xbeta : ω);
                    lc_γ_to(xr, ye);
                    IR_t * db = lic_build(cx, IR_DEREF, NULL, back);
                    ir_operand_push(db, yr);
                    lc_γ_to(yr, db);
                    IR_t * da = lic_build(cx, IR_DEREF, NULL, back);
                    ir_operand_push(da, xr);
                    lc_γ_to(db, da);
                    IR_t * n1 = lic_build(cx, IR_REV_ASSIGN_VAR, NULL, back);
                    ir_operand_push(n1, yr);
                    ir_operand_push(n1, da);
                    lc_γ_to(da, n1);
                    IR_t * n2 = lic_build(cx, IR_REV_ASSIGN_VAR, γ, ω);
                    ir_operand_push(n2, xr);
                    ir_operand_push(n2, db);
                    lc_γ_to(n1, n2);
                    lic_ω_to(n2, n1);
                    cx->beta = n2;
                    *res = n2;
                    return xe;
                }
                cx->beta = b0;
            }
            IR_t * nd = lic_build(cx, IR_FAIL, γ, ω);
            IR_t * lr = NULL;
            lic_lower(cx, lt, nd, ω, &lr);
            IR_t * rr = NULL;
            lic_lower(cx, rt2, nd, ω, &rr);
            ir_operand_push(nd, lr);
            ir_operand_push(nd, rr);
            *res = nd;
            return nd;
        }
        case TT_RECORD:
        {
            extern void record_register(const char *spec);
            const char * rname = t->v.sval;
            icn_note_record_name(rname);
            if (rname) {
                size_t specn = strlen(rname) + 3;
                for (int k = 0; k < t->n; k++) specn += 1 + ((t->c[k] && t->c[k]->v.sval) ? strlen(t->c[k]->v.sval) : 0);
                char spec[specn];
                int pos = 0;
                pos += snprintf(spec + pos, sizeof spec - pos, "%s(", rname);
                for (int k = 0; k < t->n; k++) {
                    if (k) pos += snprintf(spec + pos, sizeof spec - pos, ",");
                    if (t->c[k] && t->c[k]->v.sval) pos += snprintf(spec + pos, sizeof spec - pos, "%s", t->c[k]->v.sval);
                }
                snprintf(spec + pos, sizeof spec - pos, ")");
                record_register(spec);
            }
            IR_t * s = lic_build(cx, IR_SUCCEED, γ, ω);
            *res = s;
            return s;
        }
        case TT_INVOCABLE:
        case TT_LINK:
        default:
        { IR_t * s = lic_build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    }
}
