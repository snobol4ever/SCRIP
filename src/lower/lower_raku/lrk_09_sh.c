#include "lower/lower_raku/lrk_head.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "ir/ir_node.h"
#include "ir/ir_graph.h"
#include "ir/stage2.h"
#include "parsers/snobol4/scrip_cc.h"
#include "lower/lower_common.h"
#include "lower/lower_raku/lrk_01_sh.h"
#include "lower/lower_raku/lrk_02_rk.h"
#include "lower/lower_raku/lrk_03_rk.h"
#include "lower/lower_raku/lrk_04_rk.h"
#include "lower/lower_raku/lrk_05_rk.h"
#include "lower/lower_raku/lrk_06_rk.h"
#include "lower/lower_raku/lrk_08_rk.h"
#include "lower/lower_raku/lrk_09_sh.h"
#include "lower/lower_raku/lrk_10_rk.h"
#include "lower/lower_raku/lrk_11_rk.h"
#include "lower/lower_raku/lrk_12_rk.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * lower_rv(rcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * dummy = NULL;
    if (!res) res = &dummy;
    if (!t) { IR_t * s = lrk_build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    if (t->t == TT_DIVIS && t->n > 1) { return lower_rv(cx, rk_divis_desugar(t), γ, ω, res); }
    if (rk_is_relop(t->t)) {
        if (t->n < 2) return rk_excise(cx, γ, ω, res);
        IR_t * op = lrk_build(cx, IR_BINOP_RELOP_VAL, γ, ω);
        rk_relop_code(op, t->t);
        IR_t * lr = NULL, * rr = NULL;
        IR_t * ea = lower_rv(cx, rk_numctx(t->t, t->c[0], t->c[1]), NULL, ω, &lr);
        IR_t * eb = lower_rv(cx, rk_numctx(t->t, t->c[1], t->c[0]), op, ω, &rr);
        lrk_γ_to(lr, eb);
        ir_operand_push(op, lr);
        ir_operand_push(op, rr);
        *res = op;
        return ea;
    }
    if (t->t == TT_DIV && t->n > 1) { return lower_rcall(cx, t, "__rk_div", 0, γ, ω, res); }
    if (t->t == TT_POW && t->n > 1 && !(t->c[1] && t->c[1]->t == TT_ILIT && (t->c[1]->v.ival == 1 || t->c[1]->v.ival == 2))) { return lower_rcall(cx, t, "__rk_pow", 0, γ, ω, res); }
    if (t->t == TT_MOD && t->n > 1) { return lower_rcall(cx, t, "__rk_mod", 0, γ, ω, res); }
    if (rk_is_binop(t->t)) {
        IR_t * op = lrk_build(cx, IR_BINOP, γ, ω);
        IR_LIT(op).ival = rk_binop_code(t->t);
        IR_t * lr = NULL, * rr = NULL;
        IR_t * ea, * eb;
        if (t->t == TT_CAT) {
            ea = rk_lower_str_operand(cx, t->c[0], NULL, ω, &lr);
            eb = rk_lower_str_operand(cx, t->c[1], op, ω, &rr);
        } else {
            ea = lower_rv(cx, rk_numctx(t->t, t->c[0], t->c[1]), NULL, ω, &lr);
            eb = lower_rv(cx, rk_numctx(t->t, t->c[1], t->c[0]), op, ω, &rr);
        }
        lrk_γ_to(lr, eb);
        ir_operand_push(op, lr);
        ir_operand_push(op, rr);
        *res = op;
        return ea;
    }
    switch (t->t) {
        case TT_ILIT:
        { IR_t * nd = lrk_build(cx, IR_LIT_INTEGER, γ, ω); IR_LIT(nd).ival = t->v.ival; *res = nd; return nd; }
        case TT_FLIT:
        if (t->n == 1 && t->c[0] && t->c[0]->t == TT_QLIT) {
            tree_t * rl = ast_node_new(TT_FNC);
            rl->line = t->line;
            rl->v.sval = (char *) intern("__rk_ratlit");
            ast_push(rl, leaf_sval2(TT_VAR, "__rk_ratlit"));
            ast_push(rl, t->c[0]);
            return lower_rv(cx, rl, γ, ω, res);
        }
        { IR_t * nd = lrk_build(cx, IR_LIT_REAL, γ, ω); IR_LIT(nd).dval = t->v.dval; *res = nd; return nd; }
        case TT_QLIT:
        { IR_t * nd = lrk_build(cx, IR_LIT_STRING, γ, ω); IR_LIT(nd).sval = t->v.sval; *res = nd; return nd; }
        case TT_NUL:
        if (t->v.ival == 1) return lower_rv(cx, rk_typeobj_tree("Any"), γ, ω, res);
        { IR_t * nd = lrk_build(cx, IR_CALL, γ, ω); IR_LIT(nd).sval = "__rk_undef"; *res = nd; return nd; }
        case TT_NOT:
        {
            if (t->n < 1) return rk_excise(cx, γ, ω, res);
            tree_t * nb = ast_node_new(TT_FNC);
            nb->v.sval = (char *) intern("__rk_notbool");
            ast_push(nb, leaf_sval2(TT_VAR, "__rk_notbool"));
            ast_push(nb, t->c[0]);
            return lower_rv(cx, nb, γ, ω, res);
        }
        case TT_MNS:
        {
            if (t->n < 1) return rk_excise(cx, γ, ω, res);
            const tree_t * x = t->c[0];
            if (x && x->t == TT_ILIT && x->v.ival != INT64_MIN) { IR_t * nd = lrk_build(cx, IR_LIT_INTEGER, γ, ω); IR_LIT(nd).ival = -x->v.ival; *res = nd; return nd; }
            if (x && x->t == TT_FLIT && x->n == 0) { IR_t * nd = lrk_build(cx, IR_LIT_REAL, γ, ω); IR_LIT(nd).dval = -x->v.dval; *res = nd; return nd; }
            tree_t * m = ast_node_new(TT_MUL);
            m->line = t->line;
            ast_push(m, t->c[0]);
            tree_t * m1 = ast_node_new(TT_ILIT);
            m1->v.ival = -1;
            ast_push(m, m1);
            return lower_rv(cx, m, γ, ω, res);
        }
        case TT_VAR:
        {
            if (t->v.sval && t->v.sval[0] == ':' && t->v.sval[1] == ':' && t->v.sval[2] && !strpbrk(t->v.sval + 2, "(<{:$@%&") &&
                (rk_is_class_name(t->v.sval + 2) || rk_is_grammar_name(t->v.sval + 2))) ((tree_t *) t)->v.sval += 2;
            if (rk_is_grammar_name(t->v.sval) || rk_is_class_name(t->v.sval) || (t->v.sval && t->v.sval[0] == 'X' && t->v.sval[1] == ':' && t->v.sval[2] == ':')) {
                IR_t * nd = lrk_build(cx, IR_LIT_STRING, γ, ω);
                IR_LIT(nd).sval = t->v.sval;
                *res = nd;
                return nd;
            }
            if ((t->slen & 1) && rk_is_version_lit(t->v.sval)) { IR_t * nd = lrk_build(cx, IR_LIT_STRING, γ, ω); IR_LIT(nd).sval = t->v.sval; *res = nd; return nd; }
            if (t->v.sval && strchr(t->v.sval, ':') && !(t->slen & RK_VAR_PKG) && !rk_predeclared(t->v.sval, 0)) {
                IR_t * nd = lrk_build(cx, IR_LIT_STRING, γ, ω);
                IR_LIT(nd).sval = rk_qualified_type_gist(t->v.sval);
                *res = nd;
                return nd;
            }
            if ((t->slen & 1) && t->v.sval && !strcmp(t->v.sval, "Nil") && !rk_is_class_name("Nil")) { IR_t * nd = lrk_build(cx, IR_CALL, γ, ω); IR_LIT(nd).sval = "__rk_undef"; *res = nd; return nd; }
            if ((t->slen & 1) && t->v.sval && !strcmp(t->v.sval, "pi") && !rk_is_class_name("pi")) {
                IR_t * nd = lrk_build(cx, IR_LIT_REAL, γ, ω);
                IR_LIT(nd).dval = 3.141592653589793;
                *res = nd;
                return nd;
            }
            if ((t->slen & 1) && t->v.sval && !strcmp(t->v.sval, "e") && !rk_is_class_name("e")) {
                IR_t * nd = lrk_build(cx, IR_LIT_REAL, γ, ω);
                IR_LIT(nd).dval = 2.718281828459045;
                *res = nd;
                return nd;
            }
            if ((t->slen & 1) && t->v.sval && !strcmp(t->v.sval, "tau") && !rk_is_class_name("tau")) {
                IR_t * nd = lrk_build(cx, IR_LIT_REAL, γ, ω);
                IR_LIT(nd).dval = 6.283185307179586;
                *res = nd;
                return nd;
            }
            if ((t->slen & 1) && t->v.sval && !strcmp(t->v.sval, "Inf") && !rk_is_class_name("Inf")) {
                IR_t * nd = lrk_build(cx, IR_LIT_REAL, γ, ω);
                IR_LIT(nd).dval = (double) INFINITY;
                *res = nd;
                return nd;
            }
            if ((t->slen & 1) && t->v.sval && !strcmp(t->v.sval, "NaN") && !rk_is_class_name("NaN")) {
                IR_t * nd = lrk_build(cx, IR_LIT_REAL, γ, ω);
                IR_LIT(nd).dval = (double) NAN;
                *res = nd;
                return nd;
            }
            if ((t->slen & 1) && t->v.sval && (!strcmp(t->v.sval, "\xcf\x80") || !strcmp(t->v.sval, "\xcf\x84") || !strcmp(t->v.sval, "\xe2\x88\x9e"))) {
                IR_t * nd = lrk_build(cx, IR_LIT_REAL, γ, ω);
                IR_LIT(nd).dval = t->v.sval[0] == '\xe2' ? (double) INFINITY : t->v.sval[1] == '\x80' ? 3.141592653589793 : 6.283185307179586;
                *res = nd;
                return nd;
            }
            if ((t->slen & 1) && t->v.sval && !strcmp(t->v.sval, "die") && !rk_user_proc_exists("die") && !rk_is_class_name("die")) {
                tree_t * df = ast_node_new(TT_DIE);
                ast_push(df, leaf_sval2(TT_QLIT, "Died"));
                return lower_rv(cx, df, γ, ω, res);
            }
            if ((t->slen & 1) && t->v.sval && rk_user_proc_exists(t->v.sval) && !rk_is_class_name(t->v.sval)) {
                tree_t * cf = ast_node_new(TT_FNC);
                cf->v.sval = t->v.sval;
                ast_push(cf, leaf_sval2(TT_VAR, t->v.sval));
                return lower_rv(cx, cf, γ, ω, res);
            }
            if (t->v.sval && !strcmp(t->v.sval, "*") && !t->n) {
                tree_t * mc = ast_node_new(TT_FNC);
                mc->v.sval = (char *)"__rk_typeobj";
                tree_t * nmv = ast_node_new(TT_VAR);
                nmv->v.sval = (char *)"__rk_typeobj";
                ast_push(mc, nmv);
                tree_t * bq = ast_node_new(TT_QLIT);
                bq->v.sval = (char *)"Whatever";
                ast_push(mc, bq);
                return lower_rcall(cx, mc, "__rk_typeobj", 1, γ, ω, res);
            }
            if ((t->slen & 1) && t->v.sval && t->v.sval[0] == '&' && t->v.sval[1] && rk_proc_known(t->v.sval + 1)) {
                tree_t * mc = ast_node_new(TT_FNC);
                mc->v.sval = (char *)"__blk_ref";
                tree_t * nmv = ast_node_new(TT_VAR);
                nmv->v.sval = (char *)"__blk_ref";
                ast_push(mc, nmv);
                tree_t * bq = ast_node_new(TT_QLIT);
                bq->v.sval = t->v.sval + 1;
                ast_push(mc, bq);
                return lower_rcall(cx, mc, "__blk_ref", 1, γ, ω, res);
            }
            if (rk_predeclared(t->v.sval, (t->slen & 1) != 0)) {
                tree_t * pf = ast_node_new(TT_FNC);
                pf->v.sval = (char *) "__rk_pre";
                ast_push(pf, leaf_sval2(TT_VAR, "__rk_pre"));
                ast_push(pf, leaf_sval2(TT_QLIT, t->v.sval));
                return lower_rv(cx, pf, γ, ω, res);
            }
            if ((t->slen & 1) && t->v.sval && !strcmp(t->v.sval, "i") && !rk_is_class_name("i")) { IR_t * nd = lrk_build(cx, IR_CALL, γ, ω); IR_LIT(nd).sval = "__rk_mkcplx_i"; *res = nd; return nd; }
            if (rk_var_byref(cx, t)) { IR_t * dr = lrk_build(cx, IR_DEREF, γ, ω); IR_t * v = lrk_build(cx, IR_VAR, dr, ω); IR_LIT(v).sval = t->v.sval; ir_operand_push(dr, v); *res = dr; return v; }
            IR_t * nd = lrk_build(cx, IR_VAR, γ, ω);
            IR_LIT(nd).sval = t->v.sval;
            *res = nd;
            return nd;
        }
        case TT_ASSIGN:
        if (t->n > 1 && t->c[0] && (t->c[0]->t == TT_FIELD || t->c[0]->t == TT_TWIGIL_FIELD)) {
            return lower_field_set(cx, t->c[0], t->c[1], γ, ω, res);
        } else if (t->n > 1 && t->c[0] && t->c[0]->t == TT_VAR) {
            const tree_t * rhs = t->c[1];
            if (rhs && rhs->t == TT_FNC && rhs->n > 1 && rhs->c[0] && rhs->c[0]->v.sval && !strcmp(rhs->c[0]->v.sval, "pop") && rhs->c[1] && rhs->c[1]->t == TT_VAR) {
                IR_t * asA = lrk_build(cx, IR_ASSIGN, γ, ω);
                IR_LIT(asA).sval = rhs->c[1]->v.sval;
                IR_t * r2 = NULL;
                IR_t * einit = lower_rcall(cx, rhs, "arr_init", 1, asA, ω, &r2);
                if (r2) ir_operand_push(asA, r2);
                IR_t * asP = lrk_build(cx, IR_ASSIGN, einit, ω);
                IR_LIT(asP).sval = t->c[0]->v.sval;
                IR_t * r3 = NULL;
                IR_t * elast = lower_rcall(cx, rhs, "arr_last", 1, asP, ω, &r3);
                if (r3) ir_operand_push(asP, r3);
                *res = asA;
                return elast;
            }
            if (rhs && rk_is_relop(rhs->t)) {
                IR_t * nd = lrk_build(cx, IR_ASSIGN, γ, ω);
                IR_LIT(nd).sval = t->c[0]->v.sval;
                IR_t * op = lrk_build(cx, IR_BINOP_RELOP_VAL, nd, ω);
                rk_relop_code(op, rhs->t);
                IR_t * lr = NULL, * rr = NULL;
                IR_t * ea = lower_rv(cx, rhs->c[0], NULL, ω, &lr);
                IR_t * eb = lower_rv(cx, rhs->c[1], op, ω, &rr);
                lrk_γ_to(lr, eb);
                ir_operand_push(op, lr);
                ir_operand_push(op, rr);
                ir_operand_push(nd, op);
                *res = nd;
                return ea;
            }
            int decl_only = t->c[1] && t->c[1]->t == TT_NUL && t->c[1]->v.ival == 1;
            IR_t * vtrace_call = NULL;
            IR_t * vtrace = (decl_only || t->v.ival == 1) ? NULL : trace_value_prep(cx, t->c[0]->v.sval, γ, ω, &vtrace_call);
            IR_t * nd = lrk_build(cx, IR_ASSIGN, vtrace ? vtrace : γ, ω);
            IR_LIT(nd).sval = t->c[0]->v.sval;
            IR_t * rr = NULL;
            IR_t * e = lower_rv(cx, decl_only || t->v.ival == 2 ? t->c[1] : rk_assign_rhs(t->c[0]->v.sval, t->c[1]), nd, ω, &rr);
            if (rr) ir_operand_push(nd, rr);
            if (rr && vtrace_call) ir_operand_push(vtrace_call, rr);
            *res = nd;
            return e;
        }
        { IR_t * s = lrk_build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
        case TT_DECL:
        if (t->n > 1 && t->c[1] && t->c[1]->t == TT_VAR) {
            int loop_bind = t->c[0] && t->c[0]->t == TT_VAR && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, "__decl");
            IR_t * vtrace_call = NULL;
            IR_t * vtrace = (t->n > 2 && t->c[2] && !loop_bind) ? trace_value_prep(cx, t->c[1]->v.sval, γ, ω, &vtrace_call) : NULL;
            IR_t * nd = lrk_build(cx, IR_ASSIGN, vtrace ? vtrace : γ, ω);
            IR_LIT(nd).sval = t->c[1]->v.sval;
            IR_t * rr = NULL;
            IR_t * e;
            if (t->n > 2 && t->c[2]) {
                e = lower_rv(cx, rk_assign_rhs(t->c[1]->v.sval, t->c[2]), nd, ω, &rr);
            } else if (t->c[1]->v.sval && t->c[1]->v.sval[0] != '%' && t->c[1]->v.sval[0] != '@' && t->c[0] && t->c[0]->t == TT_VAR && rk_decl_typeobj_name(t->c[0]->v.sval)) {
                e = lower_rv(cx, rk_typeobj_tree(t->c[0]->v.sval), nd, ω, &rr);
            } else {
                IR_t * u = lrk_build(cx, IR_CALL, nd, ω);
                IR_LIT(u).sval = (t->c[1]->v.sval && t->c[1]->v.sval[0] == '%') ? "__rk_hash" : "__rk_undef";
                rr = u;
                e = u;
            }
            if (rr) ir_operand_push(nd, rr);
            if (rr && vtrace_call) ir_operand_push(vtrace_call, rr);
            *res = nd;
            return e;
        }
        { IR_t * s = lrk_build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
        case TT_ARR_SET:
        if (t->n > 2 && t->c[0] && t->c[0]->t == TT_VAR) {
            const char * vn = t->c[0]->v.sval;
            if (rk_var_byref(cx, t->c[0])) {
                IR_t * asn = lrk_build(cx, IR_ASSIGN_VAR, γ, ω);
                IR_t * vr = lrk_build(cx, IR_VAR, NULL, ω);
                IR_LIT(vr).sval = vn;
                IR_t * r2 = NULL;
                IR_t * e = lower_rcall(cx, t, "__rk_arr_set", 0, asn, ω, &r2);
                lrk_γ_to(vr, e ? e : asn);
                ir_operand_push(asn, vr);
                if (r2) ir_operand_push(asn, r2);
                *res = asn;
                return vr;
            }
            IR_t * as = lrk_build(cx, IR_ASSIGN, γ, ω);
            IR_LIT(as).sval = vn;
            IR_t * r2 = NULL;
            IR_t * e = lower_rcall(cx, t, "__rk_arr_set", 0, as, ω, &r2);
            if (r2) ir_operand_push(as, r2);
            *res = as;
            return e;
        }
        if (t->n > 2 && t->c[0] && t->c[0]->t == TT_TWIGIL_FIELD) {
            tree_t * nv = ast_node_new(TT_FNC);
            nv->v.sval = (char *) "__rk_arr_set";
            nv->line = t->line;
            ast_push(nv, leaf_sval2(TT_VAR, "__rk_arr_set"));
            for (int i = 0; i < t->n; i++) ast_push(nv, t->c[i]);
            return lower_field_set(cx, t->c[0], nv, γ, ω, res);
        }
        { IR_t * s = lrk_build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
        case TT_USE_DECL:
        if ((t->slen & 3) == 3 && t->v.sval) {
            tree_t * df = ast_node_new(TT_DIE);
            size_t ml = strlen(t->v.sval) + 24;
            char * msg = (char *) ct_alloc(ml);
            snprintf(msg, ml, "Could not find %s in:", t->v.sval);
            ast_push(df, leaf_sval2(TT_QLIT, msg));
            return lower_rv(cx, df, γ, ω, res);
        }
        { IR_t * nd = lrk_build(cx, IR_SUCCEED, γ, ω); *res = nd; return nd; }
        case TT_SAY:
        case TT_SAY_FH:
        if (t->t == TT_SAY && t->n == 1 && t->c[0] && (t->c[0]->t == TT_CAPTURE || t->c[0]->t == TT_NAMED_CAPTURE) && t->c[0]->n > 0) {
            const char * fn = t->c[0]->t == TT_CAPTURE ? "__rk_say_capture" : "__rk_say_named_capture";
            tree_t * f = ast_node_new(TT_FNC);
            f->v.sval = (char *) fn;
            ast_push(f, leaf_sval2(TT_VAR, fn));
            ast_push(f, t->c[0]->c[0]);
            return lower_rcall(cx, f, fn, 1, γ, ω, res);
        }
        if (t->t == TT_SAY && t->n > 1) {
            int any = 0;
            for (int i = 0; i < t->n; i++) if (t->c[i] && (t->c[i]->t == TT_CAPTURE || t->c[i]->t == TT_NAMED_CAPTURE) && t->c[i]->n > 0) any = 1;
            if (any) {
                tree_t * w = ast_node_new(TT_SAY);
                for (int i = 0; i < t->n; i++) {
                    tree_t * a = t->c[i];
                    if (a && (a->t == TT_CAPTURE || a->t == TT_NAMED_CAPTURE) && a->n > 0) {
                        const char * fn = a->t == TT_CAPTURE ? "__rk_gist_capture" : "__rk_gist_named_capture";
                        tree_t * f = ast_node_new(TT_FNC);
                        f->v.sval = (char *) fn;
                        ast_push(f, leaf_sval2(TT_VAR, fn));
                        ast_push(f, a->c[0]);
                        a = f;
                    }
                    ast_push(w, a);
                }
                return lower_rcall(cx, w, "rk_write", 0, γ, ω, res);
            }
        }
        if (t->t == TT_SAY && rk_user_meth_named("gist")) {
            tree_t * w = ast_node_new(TT_SAY);
            w->line = t->line;
            for (int i = 0; i < t->n; i++) ast_push(w, rk_yields_array(t->c[i]) || rk_yields_list(t->c[i]) ? t->c[i] : rk_fnc2("__rk_gist_pre", t->c[i], NULL));
            t = w;
        }
        if (t->n == 1 && rk_yields_array(t->c[0])) return lower_rcall(cx, t, "rk_write_arr", 0, γ, ω, res);
        if (t->n == 1 && rk_yields_list(t->c[0])) return lower_rcall(cx, t, "rk_write_list", 0, γ, ω, res);
        return lower_rcall(cx, t, "rk_write", 0, γ, ω, res);
        case TT_PRINT:
        case TT_PRINT_FH:
        if (t->t == TT_PRINT && rk_user_meth_named("Str")) {
            tree_t * w = ast_node_new(TT_PRINT);
            w->line = t->line;
            for (int i = 0; i < t->n; i++) ast_push(w, t->c[i] && t->c[i]->t == TT_QLIT ? t->c[i] : rk_fnc2("__rk_str_pre", t->c[i], NULL));
            t = w;
        }
        return lower_rcall(cx, t, "rk_writes", 0, γ, ω, res);
        case TT_DIE:
        if (t->n == 1 && t->c[0] && t->c[0]->t != TT_QLIT && t->c[0]->t != TT_CAT) return lower_rv(cx, rk_throw_desugar(cx, t->c[0]), γ, ω, res);
        return lower_rcall(cx, t, "die", 0, γ, ω, res);
        case TT_TRY:
        if (t->slen != 77 && t->n > 0 && t->c[0] && (t->c[0]->t == TT_SEQ || t->c[0]->t == TT_SEQ_EXPR || t->c[0]->t == TT_PROGRAM)) {
            const tree_t * ob = t->c[0];
            int li = -1;
            for (int i = ob->n - 1; i >= 0; i--) {
                const tree_t * s = ob->c[i];
                if (s && s->t == TT_STMT) { const tree_t * sub = lrk_stmt_subj(s); if (sub) s = sub; }
                if (s && s->t == TT_CATCH) continue;
                li = i;
                break;
            }
            const tree_t * ls = li >= 0 ? ob->c[li] : NULL;
            if (ls && ls->t == TT_STMT) { const tree_t * sub = lrk_stmt_subj(ls); if (sub) ls = sub; }
            if (ls && ls->t != TT_IF && ls->t != TT_UNLESS && ls->t != TT_WHILE && ls->t != TT_UNTIL && ls->t != TT_REPEAT && ls->t != TT_FOR && ls->t != TT_FOR_RANGE && ls->t != TT_EVERY &&
                ls->t != TT_CASE && ls->t != TT_RETURN && ls->t != TT_NUL && ls->t != TT_SEQ && ls->t != TT_SEQ_EXPR && ls->t != TT_ASSIGN && ls->t != TT_REVASSIGN && ls->t != TT_ARR_SET &&
                ls->t != TT_HASH_SET && ls->t != TT_DECL && ls->t != TT_ARR_DECL && ls->t != TT_HASH_DECL && ls->t != TT_RW_DECL && ls->t != TT_STATIC_DECL && ls->t != TT_SUB_DECL &&
                ls->t != TT_CLASS_DECL) {
                char tvb[48];
                snprintf(tvb, sizeof tvb, "__tryv%p", (const void *) t);
                const char * tv = ct_strdup(tvb);
                tree_t * nb = ast_node_new(ob->t);
                for (int i = 0; i < ob->n; i++) {
                    if (i == li) {
                        tree_t * as = ast_node_new(TT_ASSIGN);
                        ast_push(as, leaf_sval2(TT_VAR, tv));
                        ast_push(as, rk_fnc2("__rk_sink", (tree_t *) ls, NULL));
                        ast_push(nb, as);
                    } else ast_push(nb, ob->c[i]);
                }
                tree_t * nt = ast_node_new(TT_TRY);
                nt->line = t->line;
                nt->slen = 77;
                ast_push(nt, nb);
                if (t->n > 1) ast_push(nt, t->c[1]);
                tree_t * init = ast_node_new(TT_ASSIGN);
                ast_push(init, leaf_sval2(TT_VAR, tv));
                ast_push(init, ast_node_new(TT_NUL));
                tree_t * sq = ast_node_new(TT_SEQ_EXPR);
                ast_push(sq, init);
                ast_push(sq, nt);
                ast_push(sq, leaf_sval2(TT_VAR, tv));
                return lower_rv(cx, sq, γ, ω, res);
            }
            goto try_plain;
        } else {
            try_plain:
            {
                const tree_t * body = (t->n > 0) ? t->c[0] : NULL;
                const tree_t * handler = (t->n > 1) ? t->c[1] : NULL;
                if (!handler && body && (body->t == TT_SEQ || body->t == TT_SEQ_EXPR || body->t == TT_PROGRAM)) for (int i = 0; i < body->n; i++) {
                    const tree_t * s = body->c[i];
                    if (s && s->t == TT_STMT) { const tree_t * sub = lrk_stmt_subj(s); if (sub) s = sub; }
                    if (s && s->t == TT_CATCH && s->n > 0) { handler = rk_catch_handler(s); break; }
                }
                IR_t * kexit = lrk_build(cx, IR_CALL, γ, ω);
                IR_LIT(kexit).sval = "try_exit";
                IR_t * save = cx->try_catch;
                cx->try_catch = NULL;
                int save_td = cx->try_depth;
                IR_t * centry;
                if (handler) {
                    IR_t * hentry = lower_rblock(cx, handler, γ, ω, NULL);
                    IR_t * clr = lrk_build(cx, IR_CALL, hentry, ω);
                    IR_LIT(clr).sval = "exc_clear";
                    IR_t * asg = lrk_build(cx, IR_ASSIGN, clr, ω);
                    IR_LIT(asg).sval = "_";
                    IR_t * eg = lrk_build(cx, IR_CALL, asg, ω);
                    IR_LIT(eg).sval = "exc_get";
                    ir_operand_push(asg, eg);
                    IR_t * tx = lrk_build(cx, IR_CALL, eg, ω);
                    IR_LIT(tx).sval = "try_exit";
                    centry = tx;
                } else {
                    IR_t * clr = lrk_build(cx, IR_CALL, γ, ω);
                    IR_LIT(clr).sval = "exc_clear";
                    IR_t * tx = lrk_build(cx, IR_CALL, clr, ω);
                    IR_LIT(tx).sval = "try_exit";
                    centry = tx;
                }
                cx->try_catch = centry;
                cx->try_depth = save_td + 1;
                const tree_t * lb = body;
                if (body && (body->t == TT_SEQ || body->t == TT_SEQ_EXPR || body->t == TT_PROGRAM) && body->slen != 78) {
                    tree_t * mb = ast_node_new(body->t);
                    mb->line = body->line;
                    mb->slen = 78;
                    for (int i = 0; i < body->n; i++) ast_push(mb, body->c[i]);
                    lb = mb;
                }
                IR_t * bentry = lower_rblock(cx, lb, kexit, ω, NULL);
                cx->try_catch = save;
                cx->try_depth = save_td;
                IR_t * te = lrk_build(cx, IR_CALL, bentry, ω);
                IR_LIT(te).sval = "try_enter";
                *res = kexit;
                return te;
            }
        }
        case TT_ANON_BLOCK:
        {
            const char * bn = t->v.sval ? t->v.sval : "?";
            tree_t * mc = ast_node_new(TT_FNC);
            mc->v.sval = (char *)"__blk_ref";
            tree_t * nmv = ast_node_new(TT_VAR);
            nmv->v.sval = (char *)"__blk_ref";
            ast_push(mc, nmv);
            tree_t * bq = ast_node_new(TT_QLIT);
            bq->v.sval = (char *)bn;
            ast_push(mc, bq);
            return lower_rcall(cx, mc, "__blk_ref", 1, γ, ω, res);
        }
        case TT_INVOKE:
        {
            const char * rn = cx->cur_proc_name;
            if (t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, "&?ROUTINE") && rn && *rn && !strstr(rn, "__") && !strpbrk(rn, "$:<>")) {
                tree_t * rc = ast_node_new(TT_FNC);
                rc->line = t->line;
                rc->v.sval = (char *) rn;
                ast_push(rc, leaf_sval2(TT_VAR, rn));
                for (int i = 1; i < t->n; i++) ast_push(rc, t->c[i]);
                return lower_rv(cx, rc, γ, ω, res);
            }
            tree_t * mc = ast_node_new(TT_FNC);
            mc->v.sval = (char *)"__blk_invoke";
            tree_t * nmv = ast_node_new(TT_VAR);
            nmv->v.sval = (char *)"__blk_invoke";
            ast_push(mc, nmv);
            for (int i = 0; i < t->n; i++) ast_push(mc, t->c[i]);
            return lower_rcall(cx, mc, "__blk_invoke", 1, γ, ω, res);
        }
        case TT_FNC:
        {
            const char * nm = (t->n > 0 && t->c[0]) ? t->c[0]->v.sval : "?";
            if (nm && !strcmp(nm, "EVAL") && !rk_user_proc_exists("EVAL")) return lower_rcall(cx, t, "__rk_eval", 1, γ, ω, res);
            if (nm && !strcmp(nm, "__rk_temp") && t->n >= 2) return lower_rv(cx, t->n >= 3 ? t->c[2] : t->c[1], γ, ω, res);
            if (nm && !rk_is_multi_name(nm)) {
                int named = !strcmp(nm, "__rk_named_call");
                const char * pn = named ? ((t->n > 2 && t->c[1] && t->c[1]->t == TT_QLIT) ? t->c[1]->v.sval : NULL) : nm;
                int rkv = pn ? rk_proc_rest_kind(pn) : 0, vi = (rkv >> 4) - 1, p0 = named ? 3 : 1;
                int np = named ? ((t->n > 2 && t->c[2] && t->c[2]->t == TT_ILIT) ? (int) t->c[2]->v.ival : -1) : t->n - 1;
                const tree_t * lastp = (np == vi + 1 && vi >= 0 && p0 + vi < t->n) ? t->c[p0 + vi] : NULL;
                if (vi >= 0 && np >= vi && !(lastp && lastp->t == TT_FNC && lastp->v.sval && !strcmp(lastp->v.sval, "__rk_slurp"))) {
                    tree_t * nc = rk_fnc2("__rk_named_call", leaf_sval2(TT_QLIT, pn), rk_ti(vi + 1));
                    nc->line = t->line;
                    for (int i = 0; i < vi; i++) ast_push(nc, t->c[p0 + i]);
                    tree_t * sl = rk_fnc2("__rk_slurp", rk_ti(rkv & 15), NULL);
                    for (int i = vi; i < np; i++) ast_push(sl, t->c[p0 + i]);
                    ast_push(nc, sl);
                    if (named) for (int i = p0 + np; i < t->n; i++) ast_push(nc, t->c[i]);
                    return lower_rv(cx, nc, γ, ω, res);
                }
            }
            if (nm && (!strcmp(nm, "last") || !strcmp(nm, "next") || !strcmp(nm, "redo")) && !rk_user_proc_exists(nm)) { IR_t * lc = rk_loop_ctl(cx, t, nm, ω, res); if (lc) return lc; }
            if (nm && !rk_user_proc_exists(nm) && !rk_is_multi_name(nm) && !rk_is_class_name(nm)) {
                int swap = (!strcmp(nm, "split") || !strcmp(nm, "comb") || !strcmp(nm, "first") || !strcmp(nm, "roll")) && t->n == 3;
                if (swap || (!strcmp(nm, "words") && t->n == 2)) {
                    tree_t * mc = ast_node_new(TT_METHCALL);
                    mc->line = t->line;
                    ast_push(mc, swap ? t->c[2] : t->c[1]);
                    ast_push(mc, leaf_sval2(TT_QLIT, nm));
                    if (swap) ast_push(mc, t->c[1]);
                    return lower_rv(cx, mc, γ, ω, res);
                }
                if (!strcmp(nm, "samewith") && cx->cur_proc_name && *cx->cur_proc_name && !strstr(cx->cur_proc_name, "__")) {
                    tree_t * sc = ast_node_new(TT_FNC);
                    sc->line = t->line;
                    sc->v.sval = (char *) cx->cur_proc_name;
                    ast_push(sc, leaf_sval2(TT_VAR, cx->cur_proc_name));
                    for (int i = 1; i < t->n; i++) ast_push(sc, t->c[i]);
                    return lower_rv(cx, sc, γ, ω, res);
                }
                if (!strcmp(nm, "not") && t->n == 2) { tree_t * nt = ast_node_new(TT_NOT); nt->line = t->line; ast_push(nt, t->c[1]); return lower_rv(cx, nt, γ, ω, res); }
                {
                    static const struct {
                        const char * n;
                        int shape;
                    } fsub[] = { { "item", 1 }, { "chr", 1 }, { "ord", 1 }, { "gist", 1 }, { "append", 1 }, { "prepend", 1 }, { "rotate", 1 }, { "head", 2 }, { "tail", 2 }, { "pick", 2 }, { "list",
                        3 }, { "chrs", 3 }, { "minmax", 3 }, { "unique", 3 }, { "repeated", 3 }, { "reduce", 4 }, { "produce", 4 }, { "classify", 4 }, { "categorize", 4 }, { NULL, 0 } };
                    int shape = 0;
                    for (int i = 0; fsub[i].n; i++) if (!strcmp(nm, fsub[i].n)) shape = fsub[i].shape;
                    if (shape && t->n >= 2 && !(shape == 2 && t->n < 3)) {
                        int first_item = (shape == 2 || shape == 4) ? 2 : (shape == 3 ? 1 : 0);
                        tree_t * recv = NULL;
                        if (shape == 1) recv = t->c[1];
                        else if (t->n - first_item == 1) recv = t->c[first_item];
                        else {
                            recv = ast_node_new(TT_FNC);
                            recv->v.sval = (char *) "__rk_arr";
                            ast_push(recv, leaf_sval2(TT_VAR, "__rk_arr"));
                            for (int i = first_item; i < t->n; i++) ast_push(recv, t->c[i]);
                        }
                        tree_t * mc = ast_node_new(TT_METHCALL);
                        mc->line = t->line;
                        ast_push(mc, recv);
                        ast_push(mc, leaf_sval2(TT_QLIT, nm));
                        if (shape == 1) for (int i = 2; i < t->n; i++) ast_push(mc, t->c[i]);
                        else if (shape == 2 || shape == 4) ast_push(mc, t->c[1]);
                        return lower_rv(cx, mc, γ, ω, res);
                    }
                }
                if ((!strcmp(nm, "set") || !strcmp(nm, "bag") || !strcmp(nm, "mix")) && t->n >= 1) {
                    tree_t * ar = ast_node_new(TT_FNC);
                    ar->v.sval = (char *) "__rk_arr";
                    ast_push(ar, leaf_sval2(TT_VAR, "__rk_arr"));
                    for (int i = 1; i < t->n; i++) ast_push(ar, t->c[i]);
                    tree_t * mc = ast_node_new(TT_METHCALL);
                    mc->line = t->line;
                    ast_push(mc, ar);
                    ast_push(mc, leaf_sval2(TT_QLIT, nm[0] == 's' ? "__rk_new_Set" : nm[0] == 'b' ? "__rk_new_Bag" : "__rk_new_Mix"));
                    return lower_rv(cx, mc, γ, ω, res);
                }
                if (!strcmp(nm, "hash") || !strcmp(nm, "slip")) {
                    tree_t * ar = ast_node_new(TT_FNC);
                    ar->v.sval = (char *) "__rk_arr";
                    ast_push(ar, leaf_sval2(TT_VAR, "__rk_arr"));
                    for (int i = 1; i < t->n; i++) ast_push(ar, t->c[i]);
                    if (!strcmp(nm, "hash")) {
                        tree_t * h = ast_node_new(TT_FNC);
                        h->v.sval = (char *) "__rk_to_hash";
                        ast_push(h, leaf_sval2(TT_VAR, "__rk_to_hash"));
                        ast_push(h, ar);
                        return lower_rv(cx, h, γ, ω, res);
                    }
                    tree_t * mc = ast_node_new(TT_METHCALL);
                    ast_push(mc, ar);
                    ast_push(mc, leaf_sval2(TT_QLIT, "Slip"));
                    return lower_rv(cx, mc, γ, ω, res);
                }
            }
            if (nm && t->n >= 1 && rk_io_lower_name(nm) && !rk_user_proc_exists(nm) && !rk_is_multi_name(nm) && !rk_is_class_name(nm)) {
                tree_t * io = ast_node_new(TT_FNC);
                io->v.sval = (char *) "__rk_io";
                ast_push(io, leaf_sval2(TT_VAR, "__rk_io"));
                ast_push(io, leaf_sval2(TT_QLIT, nm));
                for (int i = 1; i < t->n; i++) ast_push(io, t->c[i]);
                return lower_rcall(cx, io, "__rk_io", 1, γ, ω, res);
            }
            if (nm && rk_is_multi_name(nm)) {
                tree_t * mc = ast_node_new(TT_FNC);
                mc->v.sval = (char *)"__multi_call";
                tree_t * nmv = ast_node_new(TT_VAR);
                nmv->v.sval = (char *)"__multi_call";
                ast_push(mc, nmv);
                tree_t * basq = ast_node_new(TT_QLIT);
                basq->v.sval = (char *)nm;
                ast_push(mc, basq);
                for (int i = 1; i < t->n; i++) ast_push(mc, t->c[i]);
                return lower_rcall(cx, mc, "__multi_call", 1, γ, ω, res);
            }
            if (nm && t->n == 2 && !strcmp(nm, "undefine") && !rk_proc_known(nm) && t->c[1] && t->c[1]->t == TT_VAR && t->c[1]->v.sval && t->c[1]->v.sval[0] != '&') {
                char sg = t->c[1]->v.sval[0];
                tree_t * as = ast_node_new(TT_ASSIGN);
                as->line = t->line;
                ast_push(as, rk_clone_tree(t->c[1]));
                ast_push(as, sg == '@' ? rk_fnc2("__rk_arr", NULL, NULL) : sg == '%' ? rk_fnc2("__rk_hash", NULL, NULL) : rk_typeobj_tree("Any"));
                return lower_rv(cx, as, γ, ω, res);
            }
            if (nm && t->n > 1 && rk_is_str_subform(nm) && !rk_proc_known(nm)) {
                tree_t * mc = ast_node_new(TT_METHCALL);
                mc->line = t->line;
                ast_push(mc, t->c[1]);
                ast_push(mc, leaf_sval2(TT_QLIT, nm));
                for (int i = 2; i < t->n; i++) ast_push(mc, t->c[i]);
                return lower_rv(cx, mc, γ, ω, res);
            }
            if (nm && !strcmp(nm, "__rk_named_call") && t->n > 4 && t->c[1] && t->c[1]->t == TT_QLIT && t->c[1]->v.sval && !strcmp(t->c[1]->v.sval, "first") && !rk_proc_known("first") && t->c[2] &&
                t->c[2]->t == TT_ILIT && t->c[2]->v.ival >= 1 && t->n >= 3 + t->c[2]->v.ival) {
                int np = (int) t->c[2]->v.ival;
                tree_t * lst = ast_node_new(TT_FNC);
                lst->v.sval = (char *)"__rk_flat";
                ast_push(lst, leaf_sval2(TT_VAR, "__rk_flat"));
                for (int i = 4; i < 3 + np; i++) ast_push(lst, t->c[i]);
                tree_t * mc = ast_node_new(TT_METHCALL);
                mc->line = t->line;
                ast_push(mc, lst);
                ast_push(mc, leaf_sval2(TT_QLIT, "first"));
                ast_push(mc, t->c[3]);
                for (int i = 3 + np; i + 1 < t->n; i += 2) {
                    tree_t * pr = ast_node_new(TT_FNC);
                    pr->v.sval = (char *)"__rk_pair";
                    ast_push(pr, leaf_sval2(TT_VAR, "__rk_pair"));
                    ast_push(pr, t->c[i]);
                    ast_push(pr, t->c[i + 1]);
                    ast_push(mc, pr);
                }
                return lower_rv(cx, mc, γ, ω, res);
            }
            if (nm && t->n > 2 && (!strcmp(nm, "map") || !strcmp(nm, "grep") || !strcmp(nm, "first")) && !rk_proc_known(nm) && t->c[1] &&
                (!strcmp(nm, "first") || t->c[1]->t == TT_ANON_BLOCK || (t->c[1]->t == TT_VAR && t->c[1]->v.sval && t->c[1]->v.sval[0] == '&'))) {
                tree_t * lst = ast_node_new(TT_FNC);
                lst->v.sval = (char *)"__rk_arr";
                ast_push(lst, leaf_sval2(TT_VAR, "__rk_arr"));
                for (int i = 2; i < t->n; i++) ast_push(lst, t->c[i]);
                tree_t * mc = ast_node_new(TT_METHCALL);
                mc->line = t->line;
                ast_push(mc, lst);
                ast_push(mc, leaf_sval2(TT_QLIT, nm));
                ast_push(mc, t->c[1]);
                return lower_rv(cx, mc, γ, ω, res);
            }
            if (nm && t->n > 1 && (!strcmp(nm, "shift") || !strcmp(nm, "pop") || !strcmp(nm, "unshift")) && !rk_proc_known(nm) && t->c[1] && t->c[1]->t == TT_VAR && t->c[1]->v.sval &&
                t->c[1]->v.sval[0] == '@') {
                tree_t * mc = ast_node_new(TT_METHCALL);
                mc->line = t->line;
                ast_push(mc, t->c[1]);
                ast_push(mc, leaf_sval2(TT_QLIT, nm));
                for (int i = 2; i < t->n; i++) ast_push(mc, t->c[i]);
                return lower_rv(cx, mc, γ, ω, res);
            }
            if (nm && !strcmp(nm, "so")) nm = "__rk_mkbool";
            if (nm && !strcmp(nm, "trim")) nm = "str_trim";
            if (nm && !strcmp(nm, "any")) nm = "__rk_jct_any";
            else if (nm && !strcmp(nm, "all")) nm = "__rk_jct_all";
            else if (nm && !strcmp(nm, "one")) nm = "__rk_jct_one";
            else if (nm && !strcmp(nm, "none")) nm = "__rk_jct_none";
            if (nm && (!strcmp(nm, "shift") || !strcmp(nm, "pop")) && t->n > 2) {
                char xm[96];
                snprintf(xm, sizeof xm, "Too many positionals passed; expected 1 argument but got %d", t->n - 1);
                tree_t * dd = ast_node_new(TT_DIE);
                ast_push(dd, leaf_sval2(TT_QLIT, lp_strdup(xm)));
                return lower_rv(cx, dd, γ, ω, res);
            }
            if (nm && !strcmp(nm, "push") && t->n > 1 && t->c[1] && t->c[1]->t == TT_VAR) {
                IR_t * as = lrk_build(cx, IR_ASSIGN, γ, ω);
                IR_LIT(as).sval = t->c[1]->v.sval;
                IR_t * r2 = NULL;
                IR_t * e = lower_rcall(cx, t, "push_pure", 1, as, ω, &r2);
                if (r2) ir_operand_push(as, r2);
                *res = as;
                return e;
            }
            if (nm && !strcmp(nm, "push") && t->n > 1 && t->c[1] && t->c[1]->t == TT_FNC && t->c[1]->n > 0 && t->c[1]->c[0] && t->c[1]->c[0]->v.sval && !strcmp(t->c[1]->c[0]->v.sval, "__rk_arr_lit"))
                return lower_rcall(cx, t, "push_pure", 1, γ, ω, res);
            if (nm && !strcmp(nm, "hash_set") && t->n > 2 && t->c[1] && (t->c[1]->t == TT_VAR || t->c[1]->t == TT_TWIGIL_FIELD)) {
                const char * vn = t->c[1]->v.sval;
                IR_t * as = lrk_build(cx, IR_ASSIGN, γ, ω);
                IR_LIT(as).sval = vn;
                IR_t * r2 = NULL;
                IR_t * e = lower_rcall(cx, t, "hash_set_pure", 1, as, ω, &r2);
                if (r2) ir_operand_push(as, r2);
                *res = as;
                return e;
            }
            if (nm && !g_lower.rk.eval_road && rk_certainly_undeclared(nm)) { fprintf(stderr, "scrip: raku: Undeclared routine: %s used at line %d\n", nm, t->line); exit(1); }
            return lower_rcall(cx, t, nm, 1, γ, ω, res);
        }
        case TT_STMT:
        { const tree_t * sub = lrk_stmt_subj(t); return sub ? lower_rv(cx, sub, γ, ω, res) : (lrk_build(cx, IR_SUCCEED, γ, ω)); }
        case TT_IF:
        case TT_UNLESS:
        {
            IR_t * tentry = (t->n > 1 && t->c[1]) ? lower_rblock(cx, t->c[1], γ, ω, NULL) : γ;
            IR_t * eentry = (t->n > 2 && t->c[2]) ? lower_rblock(cx, t->c[2], γ, ω, NULL) : γ;
            IR_t * sv_om = cx->cond_om;
            cx->cond_om = ω;
            IR_t * e = (t->t == TT_UNLESS) ? lower_cond(cx, t->c[0], eentry, tentry) : lower_cond(cx, t->c[0], tentry, eentry);
            cx->cond_om = sv_om;
            *res = e;
            return e;
        }
        case TT_WHILE:
        {
            IR_t * LOOP = lrk_build(cx, IR_GOTO, NULL, ω);
            rk_loopfr_t lf;
            rk_loop_open(cx, &lf, γ, LOOP);
            IR_t * bentry = (t->n > 1 && !rk_empty_body(t->c[1])) ? lower_rblock(cx, t->c[1], LOOP, ω, NULL) : LOOP;
            rk_loop_close(cx, &lf, bentry);
            IR_t * centry = lower_cond(cx, t->c[0], bentry, γ);
            bb_src_note(centry, "rk_while_cond", 0);
            lrk_γ_to(LOOP, centry);
            lrk_ω_to(LOOP, centry);
            *res = LOOP;
            return centry;
        }
        case TT_CLOOP:
        {
            IR_t * LOOP = lrk_build(cx, IR_GOTO, NULL, ω);
            IR_t * incr_entry = lower_rblock(cx, t->c[2], LOOP, ω, NULL);
            rk_loopfr_t lf;
            rk_loop_open(cx, &lf, γ, incr_entry);
            IR_t * bentry = lower_rblock(cx, t->c[3], incr_entry, ω, NULL);
            rk_loop_close(cx, &lf, bentry);
            IR_t * centry = lower_cond(cx, t->c[1], bentry, γ);
            bb_src_note(centry, "rk_cloop_cond", 0);
            if (incr_entry && incr_entry != LOOP) bb_src_note(incr_entry, "rk_cloop_incr", 0);
            lrk_γ_to(LOOP, centry);
            lrk_ω_to(LOOP, centry);
            IR_t * ientry = lower_rblock(cx, t->c[0], LOOP, ω, NULL);
            *res = LOOP;
            return ientry;
        }
        case TT_UNTIL:
        {
            IR_t * LOOP = lrk_build(cx, IR_GOTO, NULL, ω);
            rk_loopfr_t lf;
            rk_loop_open(cx, &lf, γ, LOOP);
            IR_t * bentry = (t->n > 1 && !rk_empty_body(t->c[1])) ? lower_rblock(cx, t->c[1], LOOP, ω, NULL) : LOOP;
            rk_loop_close(cx, &lf, bentry);
            IR_t * centry = lower_cond(cx, t->c[0], γ, bentry);
            bb_src_note(centry, "rk_until_cond", 0);
            lrk_γ_to(LOOP, centry);
            lrk_ω_to(LOOP, centry);
            *res = LOOP;
            return centry;
        }
        case TT_REPEAT:
        if (t->v.ival != 0 && t->n > 1) {
            IR_t * BACK = lrk_build(cx, IR_GOTO, NULL, ω);
            IR_t * centry = (t->v.ival == 1) ? lower_cond(cx, t->c[1], BACK, γ) : lower_cond(cx, t->c[1], γ, BACK);
            rk_loopfr_t lf;
            rk_loop_open(cx, &lf, γ, centry);
            IR_t * bentry = lower_rblock(cx, t->c[0], centry, ω, NULL);
            rk_loop_close(cx, &lf, bentry);
            bb_src_note(bentry, "rk_repeat_body", 0);
            bb_src_note(centry, "rk_repeat_cond", 0);
            lrk_γ_to(BACK, bentry);
            lrk_ω_to(BACK, bentry);
            *res = BACK;
            return bentry;
        } else {
            IR_t * LOOP = lrk_build(cx, IR_GOTO, NULL, ω);
            rk_loopfr_t lf;
            rk_loop_open(cx, &lf, ω, LOOP);
            IR_t * bentry = (t->n > 0) ? lower_rblock(cx, t->c[0], LOOP, ω, NULL) : LOOP;
            rk_loop_close(cx, &lf, bentry);
            if (bentry && bentry != LOOP) bb_src_note(bentry, "rk_loop_body", 0);
            lrk_γ_to(LOOP, bentry);
            lrk_ω_to(LOOP, bentry);
            *res = LOOP;
            return bentry;
        }
        case TT_STATIC_DECL:
        if (t->n == 2 && t->c[0] && t->c[0]->v.sval && t->c[1]) return lower_rv(cx, rk_state_desugar(t), γ, ω, res);
        return rk_excise(cx, γ, ω, res);
        case TT_LABEL_DEF:
        {
            cx->loop_label = (t->n == 1 && (rk_is_loop_node(t->c[0]) || rk_is_multi_for(t->c[0]))) ? t->v.sval : NULL;
            IR_t * le = lower_rv(cx, t->n ? t->c[0] : NULL, γ, ω, res);
            cx->loop_label = NULL;
            if (!t->v.sval || !t->n || !lrk_rk_reads_var(t->c[0], t->v.sval)) return le;
            tree_t * decl = ast_node_new(TT_ASSIGN);
            ast_push(decl, leaf_sval2(TT_VAR, t->v.sval));
            ast_push(decl, leaf_sval2(TT_QLIT, t->v.sval));
            IR_t * rd = NULL;
            return lower_rv(cx, decl, le, ω, &rd);
        }
        case TT_LOOP_BREAK:
        case TT_LOOP_NEXT:
        { IR_t * tgt = !cx->loops ? NULL : (t->t == TT_LOOP_BREAK) ? cx->loops->exit : cx->loops->next; if (!tgt) tgt = ω; IR_t * j = lrk_build(cx, IR_GOTO, tgt, tgt); *res = NULL; return j; }
        case TT_EVERY:
        if (t->slen != 82 && t->n > 1 && t->c[0] && t->c[0]->t == TT_ITERATE && t->c[0]->n > 0 && (!t->c[0]->v.sval || !strcmp(t->c[0]->v.sval, "_"))) {
            char sn[48];
            snprintf(sn, sizeof sn, "__forsv_%d", ++g_lower.rk.lower_rv_forlist_ctr);
            const char * sv = intern(sn);
            tree_t * lp = ast_node_new(TT_EVERY);
            lp->line = t->line;
            lp->slen = 82;
            for (int i = 0; i < t->n; i++) ast_push(lp, t->c[i]);
            tree_t * sa = ast_node_new(TT_ASSIGN);
            ast_push(sa, leaf_sval2(TT_VAR, sv));
            ast_push(sa, leaf_sval2(TT_VAR, "_"));
            tree_t * ra = ast_node_new(TT_ASSIGN);
            ast_push(ra, leaf_sval2(TT_VAR, "_"));
            ast_push(ra, leaf_sval2(TT_VAR, sv));
            tree_t * rw = ast_node_new(TT_ASSIGN);
            ast_push(rw, leaf_sval2(TT_VAR, "_"));
            ast_push(rw, leaf_sval2(TT_VAR, sv));
            IR_t * r1 = NULL, * r2 = NULL, * rl = NULL, * rs = NULL;
            IR_t * eg = lower_rv(cx, ra, γ, ω, &r1);
            IR_t * ew = lower_rv(cx, rw, ω, ω, &r2);
            IR_t * el = lower_rv(cx, lp, eg, ew, &rl);
            IR_t * es = lower_rv(cx, sa, el, ω, &rs);
            *res = rl;
            return es;
        }
        if (t->n > 1 && t->c[0] && t->c[0]->t == TT_ITERATE && t->c[0]->n > 0) {
            const char * vname = t->c[0]->v.sval ? t->c[0]->v.sval : "_";
            const tree_t * src = t->c[0]->c[0];
            const tree_t * body = t->c[1];
            const tree_t * xf = NULL;
            int xfk = 0;
            if (src && (src->t == TT_MAP || src->t == TT_GREP) && src->n > 1) { xfk = (src->t == TT_MAP) ? 1 : 2; xf = src->c[0]; src = src->c[1]; }
            if (src && src->t == TT_FNC && src->v.sval && rk_proc_is_generator(src->v.sval)) {
                IR_t * rcall = NULL;
                IR_t * centry = lower_rv(cx, src, NULL, γ, &rcall);
                rk_loopfr_t lf;
                rk_loop_open(cx, &lf, γ, rcall);
                IR_t * inner = rk_xf_body(cx, xf, xfk, vname, rcall, body, rcall, ω);
                rk_loop_close(cx, &lf, inner);
                lrk_γ_to(rcall, inner);
                *res = rcall;
                return centry;
            }
            if (src && src->t == TT_TO && src->n > 1) {
                IR_t * to = lrk_build(cx, IR_TO, NULL, γ);
                IR_LIT(to).sval = "ag";
                IR_t * rlo = NULL, * rhi = NULL;
                IR_t * elo = lower_rv(cx, src->c[0], NULL, ω, &rlo);
                IR_t * ehi = lower_rv(cx, src->c[1], to, ω, &rhi);
                lrk_γ_to(rlo, ehi);
                ir_operand_push(to, rlo);
                ir_operand_push(to, rhi);
                if (rhi && ir_is_generator_kind(to->op)) lc_γ_to(rhi, to);
                rk_loopfr_t lf;
                rk_loop_open(cx, &lf, γ, to);
                IR_t * inner = rk_xf_body(cx, xf, xfk, vname, to, body, to, ω);
                rk_loop_close(cx, &lf, inner);
                lrk_γ_to(to, inner);
                *res = to;
                return elo;
            }
            if (src && xfk == 0) {
                int fid = ++g_lower.rk.lower_rv_forlist_ctr;
                char iname[48];
                snprintf(iname, sizeof iname, "__foridx_%d", fid);
                const char * in = intern(iname);
                const char * ln;
                int need_matr = 0;
                if (src->t == TT_VAR && src->v.sval) { ln = src->v.sval; } else { char lname[48]; snprintf(lname, sizeof lname, "__forlist_%d", fid); ln = intern(lname); need_matr = 1; }
                tree_t * hi = ast_node_new(TT_ILIT);
                hi->v.ival = 1LL << 62;
                tree_t * lo = ast_node_new(TT_ILIT);
                lo->v.ival = 0;
                tree_t * bind = ast_node_new(TT_DECL);
                ast_push(bind, leaf_sval2(TT_VAR, "__decl"));
                ast_push(bind, leaf_sval2(TT_VAR, vname));
                tree_t * ag = ast_node_new(TT_ARR_GET);
                ast_push(ag, leaf_sval2(TT_VAR, ln));
                ast_push(ag, leaf_sval2(TT_VAR, in));
                ast_push(bind, ag);
                tree_t * nb = ast_node_new(TT_SEQ_EXPR);
                {
                    tree_t * has = rk_fnc2("__rk_iter_has", leaf_sval2(TT_VAR, ln), leaf_sval2(TT_VAR, in));
                    tree_t * no = ast_node_new(TT_NOT);
                    ast_push(no, has);
                    tree_t * brk = ast_node_new(TT_SEQ_EXPR);
                    ast_push(brk, ast_node_new(TT_LOOP_BREAK));
                    tree_t * cond = ast_node_new(TT_IF);
                    ast_push(cond, no);
                    ast_push(cond, brk);
                    ast_push(nb, cond);
                }
                ast_push(nb, bind);
                if (body) {
                    if (body->t == TT_SEQ || body->t == TT_SEQ_EXPR || body->t == TT_PROGRAM) { for (int k = 0; k < body->n; k++) ast_push(nb, body->c[k]); } else ast_push(nb, (tree_t *)body);
                }
                tree_t * fr = ast_node_new(TT_FOR_RANGE);
                ast_push(fr, leaf_sval2(TT_VAR, in));
                ast_push(fr, lo);
                ast_push(fr, hi);
                ast_push(fr, nb);
                tree_t * ex = ast_node_new(TT_ILIT);
                ex->v.ival = 0;
                ast_push(fr, ex);
                if (need_matr) {
                    IR_t * as = lrk_build(cx, IR_ASSIGN, NULL, ω);
                    IR_LIT(as).sval = ln;
                    IR_t * rr = NULL;
                    IR_t * einit = lower_rv(cx, src, as, ω, &rr);
                    if (rr) ir_operand_push(as, rr);
                    IR_t * fre = lower_rv(cx, fr, γ, ω, res);
                    lc_γ_to(as, fre);
                    return einit;
                }
                return lower_rv(cx, fr, γ, ω, res);
            }
            return rk_excise(cx, γ, ω, res);
        }
        return rk_excise(cx, γ, ω, res);
        case TT_CASE:
        if (t->n >= 1 && t->c[0]) return lower_rv(cx, rk_case_desugar(t), γ, ω, res);
        return rk_excise(cx, γ, ω, res);
        case TT_RX_ROOT:
        return lower_rv(cx, t->c[0], γ, ω, res);
        case TT_SMATCH:
        if (t->n == 3 && t->c[2] && t->c[2]->v.sval && !strcmp(t->c[2]->v.sval, "match") && t->c[1] && t->c[1]->t == TT_RX_ROOT && !t->c[1]->v.ival && (!t->c[1]->v.sval || !t->c[1]->v.sval[0]) &&
            t->c[1]->n == 2 && rk_rx_core(t->c[1]->c[1]) && rk_rx_boxes_on()) return rk_lower_smatch_boxes(cx, t, γ, ω, res);
        if (t->n > 3 && t->c[3] && t->c[2] && t->c[2]->v.sval && !strcmp(t->c[2]->v.sval, "match")) {
            tree_t * call = ast_node_new(TT_FNC);
            call->line = t->line;
            ast_push(call, t->c[0]);
            ast_push(call, t->c[1]);
            for (int k = 1; k < t->c[3]->n; k++) ast_push(call, t->c[3]->c[k]);
            return lower_rcall(cx, call, "re_matchx", 0, γ, ω, res);
        }
        if (t->n > 2 && t->c[2] && t->c[2]->v.sval && strcmp(t->c[2]->v.sval, "subst")) {
            int plain = !strcmp(t->c[2]->v.sval, "match");
            IR_t * last = NULL;
            if (plain) { last = lrk_build(cx, IR_CALL, γ, ω); IR_LIT(last).sval = (char *) "__rk_match_last"; }
            IR_t * nd = lrk_build(cx, IR_CALL, plain ? last : γ, plain ? last : ω);
            IR_LIT(nd).sval = strcmp(t->c[2]->v.sval, "match_global") ? "re_match" : "re_match_global";
            IR_t * sr = NULL, * pr = NULL;
            IR_t * es = lower_rv(cx, t->c[0], NULL, ω, &sr);
            IR_t * ep = lower_rv(cx, t->c[1], nd, ω, &pr);
            lrk_γ_to(sr, ep);
            ir_operand_push(nd, sr);
            ir_operand_push(nd, pr);
            *res = plain ? last : nd;
            return es;
        }
        return rk_excise(cx, γ, ω, res);
        case TT_GATHER:
        return rk_excise(cx, γ, ω, res);
        case TT_MAP:
        return rk_excise(cx, γ, ω, res);
        case TT_GREP:
        return rk_excise(cx, γ, ω, res);
        case TT_REVERSE:
        return lower_rcall(cx, t, "array_reverse", 0, γ, ω, res);
        case TT_FOR_RANGE:
        if (t->n > 3 && t->c[0] && t->c[0]->t == TT_VAR) {
            IR_t * va = lrk_build(cx, IR_ASSIGN, NULL, ω);
            IR_LIT(va).sval = t->c[0]->v.sval;
            IR_t * to = lrk_build(cx, IR_TO, va, γ);
            IR_LIT(to).sval = "ag";
            IR_t * rlo = NULL, * rhi = NULL;
            IR_t * elo = lower_rv(cx, t->c[1], NULL, ω, &rlo);
            IR_t * ehi = lower_rv(cx, t->c[2], to, ω, &rhi);
            lrk_γ_to(rlo, ehi);
            ir_operand_push(to, rlo);
            ir_operand_push(to, rhi);
            if (rhi && ir_is_generator_kind(to->op)) lc_γ_to(rhi, to);
            ir_operand_push(va, to);
            rk_loopfr_t lf;
            rk_loop_open(cx, &lf, γ, to);
            IR_t * bentry = lower_rblock(cx, t->c[3], to, ω, NULL);
            rk_loop_close(cx, &lf, bentry);
            lrk_γ_to(va, bentry);
            *res = to;
            return elo;
        }
        return rk_excise(cx, γ, ω, res);
        case TT_ARR_GET:
        if (t->n == 2 && t->c[0] && t->c[1] && t->c[1]->t == TT_NUL) return lower_rv(cx, t->c[0], γ, ω, res);
        return lower_rcall(cx, t, "__rk_arr_at", 0, γ, ω, res);
        case TT_HASH_GET:
        return lower_rcall(cx, t, "hash_get", 0, γ, ω, res);
        case TT_HASH_SET:
        if (t->n > 2 && t->c[0] && (t->c[0]->t == TT_VAR || t->c[0]->t == TT_TWIGIL_FIELD)) {
            const char * vn = t->c[0]->t == TT_TWIGIL_FIELD ? t->c[0]->v.sval : (t->c[0]->n > 0 && t->c[0]->c[0] ? t->c[0]->c[0]->v.sval : t->c[0]->v.sval);
            IR_t * as = lrk_build(cx, IR_ASSIGN, γ, ω);
            IR_LIT(as).sval = vn;
            IR_t * r2 = NULL;
            IR_t * e = lower_rcall(cx, t, "hash_set_pure", 0, as, ω, &r2);
            if (r2) ir_operand_push(as, r2);
            *res = as;
            return e;
        }
        { IR_t * s = lrk_build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
        case TT_HASH_EXISTS:
        return lower_rcall_bool(cx, t, "hash_exists", 0, γ, ω, res);
        case TT_HASH_DELETE:
        return lower_rcall(cx, t, "hash_delete", 0, γ, ω, res);
        case TT_TO:
        if (t->n > 1) {
            IR_t * to = lrk_build(cx, IR_TO, γ, ω);
            IR_LIT(to).sval = "ag";
            IR_t * rlo = NULL, * rhi = NULL;
            IR_t * elo = lower_rv(cx, t->c[0], NULL, ω, &rlo);
            IR_t * ehi = lower_rv(cx, t->c[1], to, ω, &rhi);
            lrk_γ_to(rlo, ehi);
            ir_operand_push(to, rlo);
            ir_operand_push(to, rhi);
            if (rhi && ir_is_generator_kind(to->op)) lc_γ_to(rhi, to);
            *res = to;
            return elo;
        }
        { IR_t * s = lrk_build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
        case TT_SORT:
        return lower_rcall(cx, t, "array_sort", 0, γ, ω, res);
        case TT_FH_CAPTURE:
        return lower_rcall(cx, t, "fh_capture", 0, γ, ω, res);
        case TT_CAPTURE:
        return lower_rcall(cx, t, "re_capture", 0, γ, ω, res);
        case TT_NAMED_CAPTURE:
        return lower_rcall(cx, t, "re_named_capture", 0, γ, ω, res);
        case TT_ALT:
        if (t->n > 1) return rk_lower_logical(cx, t, 1, γ, ω, res);
        { IR_t * s = lrk_build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
        case TT_SEQ:
        if (lrk_rk_seq_is_logical_and(t)) return rk_lower_logical(cx, t, 0, γ, ω, res);
        { IR_t * lr = NULL; IR_t * b = lower_rblock(cx, t, γ, ω, &lr); *res = lr ? lr : b; return b; }
        case TT_PROGRAM:
        case TT_SEQ_EXPR:
        if (t->t == TT_SEQ_EXPR && !t->n) { IR_t * nd = lrk_build(cx, IR_CALL, γ, ω); IR_LIT(nd).sval = "__rk_undef"; *res = nd; return nd; }
        { IR_t * lr = NULL; IR_t * b = lower_rblock(cx, t, γ, ω, &lr); *res = lr ? lr : b; return b; }
        case TT_METHCALL:
        {
            if (t->n == 2 && t->c[0] && t->c[1] && t->c[1]->t == TT_QLIT && t->c[1]->v.sval && (!strcmp(t->c[1]->v.sval, "throw") || !strcmp(t->c[1]->v.sval, "rethrow")) &&
                !rk_user_meth_named(t->c[1]->v.sval)) return lower_rv(cx, rk_throw_desugar(cx, t->c[0]), γ, ω, res);
            if (t->n > 1 && t->c[0] && t->c[0]->t == TT_TO && t->c[0]->n == 2) {
                tree_t * ra = ast_node_new(TT_FNC);
                ra->v.sval = (char *)"__rk_range_arr";
                ast_push(ra, leaf_sval2(TT_VAR, "__rk_range_arr"));
                ast_push(ra, t->c[0]->c[0]);
                ast_push(ra, t->c[0]->c[1]);
                tree_t * mc = ast_node_new(TT_METHCALL);
                mc->line = t->line;
                ast_push(mc, ra);
                for (int i = 1; i < t->n; i++) ast_push(mc, t->c[i]);
                return lower_rv(cx, mc, γ, ω, res);
            }
            const char * mname = (t->n > 1 && t->c[1]) ? t->c[1]->v.sval : NULL;
            if (mname && (!strcmp(mname, "substr") || !strcmp(mname, "substr-rw")) && t->n >= 3 && t->c[0]) {
                int wc = 0;
                for (int i = 2; i < t->n; i++) if (t->c[i] && t->c[i]->t == TT_ANON_BLOCK && t->c[i]->v.sval) wc = 1;
                if (wc) {
                    tree_t * nm = ast_node_new(TT_METHCALL);
                    nm->line = t->line;
                    ast_push(nm, t->c[0]);
                    ast_push(nm, t->c[1]);
                    tree_t * posarg = NULL;
                    for (int i = 2; i < t->n; i++) {
                        tree_t * a = t->c[i];
                        if (a && a->t == TT_ANON_BLOCK && a->v.sval) {
                            tree_t * iv = ast_node_new(TT_INVOKE);
                            ast_push(iv, a);
                            tree_t * ln = ast_node_new(TT_METHCALL);
                            ast_push(ln, t->c[0]);
                            ast_push(ln, leaf_sval2(TT_QLIT, "chars"));
                            if (i == 3 && posarg) { tree_t * sb = ast_node_new(TT_SUB); ast_push(sb, ln); ast_push(sb, posarg); ln = sb; }
                            ast_push(iv, ln);
                            a = iv;
                        }
                        if (i == 2) posarg = a;
                        ast_push(nm, a);
                    }
                    return lower_rv(cx, nm, γ, ω, res);
                }
            }
            if (mname && !strcmp(mname, "match") && t->n > 2 && t->c[2] && t->c[2]->t == TT_FNC && t->c[2]->n > 1 && t->c[2]->c[0] && t->c[2]->c[0]->v.sval &&
                !strcmp(t->c[2]->c[0]->v.sval, "__rk_regex")) {
                int glob = t->n > 3 && t->c[3] && t->c[3]->t == TT_FNC && t->c[3]->c[0] && t->c[3]->c[0]->v.sval && !strcmp(t->c[3]->c[0]->v.sval, "__rk_mkbool");
                tree_t * m = ast_node_new(TT_SMATCH);
                m->line = t->line;
                ast_push(m, t->c[0]);
                ast_push(m, t->c[2]->c[1]);
                ast_push(m, leaf_sval2(TT_QLIT, glob ? "match_global" : "match"));
                return lower_rv(cx, m, γ, ω, res);
            }
            if (mname) { IR_t * ie = rk_lower_iter_meth(cx, t, mname, γ, ω, res); if (ie) return ie; }
            if (mname && t->c[0] && t->c[0]->t == TT_VAR) {
                if (!strcmp(mname, "push") || !strcmp(mname, "unshift") || !strcmp(mname, "append") || !strcmp(mname, "prepend")) {
                    const char * fn = !strcmp(mname, "push") ? "push_pure" : !strcmp(mname, "append") ? "append_pure" : !strcmp(mname, "unshift") ? "unshift_pure" : "prepend_pure";
                    IR_t * as = lrk_build(cx, IR_ASSIGN, γ, ω);
                    IR_LIT(as).sval = t->c[0]->v.sval;
                    IR_t * r2 = NULL;
                    IR_t * e = lower_rcall_skip1(cx, t, fn, as, ω, &r2);
                    if (r2) ir_operand_push(as, r2);
                    *res = as;
                    return e;
                }
                if ((!strcmp(mname, "pop") || !strcmp(mname, "shift")) && t->n > 2) {
                    char xm[96];
                    snprintf(xm, sizeof xm, "Too many positionals passed; expected 1 argument but got %d", t->n - 1);
                    tree_t * dd = ast_node_new(TT_DIE);
                    ast_push(dd, leaf_sval2(TT_QLIT, lp_strdup(xm)));
                    return lower_rv(cx, dd, γ, ω, res);
                }
                if (!strcmp(mname, "pop") || !strcmp(mname, "shift")) {
                    const char * mut_fn = !strcmp(mname, "pop") ? "arr_init" : "arr_tail";
                    const char * val_fn = !strcmp(mname, "pop") ? "arr_last" : "__rk_shift_val";
                    char tb[40];
                    snprintf(tb, sizeof tb, "$?pms%d", cx->g->n);
                    const char * tmp = lp_strdup(tb);
                    tree_t * sq = ast_node_new(TT_SEQ_EXPR);
                    sq->line = t->line;
                    tree_t * a1 = ast_node_new(TT_ASSIGN);
                    ast_push(a1, leaf_sval2(TT_VAR, tmp));
                    ast_push(a1, rk_fnc2(val_fn, t->c[0], NULL));
                    tree_t * a2 = ast_node_new(TT_ASSIGN);
                    ast_push(a2, leaf_sval2(TT_VAR, t->c[0]->v.sval));
                    ast_push(a2, rk_fnc2(mut_fn, t->c[0], NULL));
                    ast_push(sq, a1);
                    ast_push(sq, a2);
                    ast_push(sq, leaf_sval2(TT_VAR, tmp));
                    return lower_rv(cx, sq, γ, ω, res);
                }
            }
            if (mname && t->n == 2 && t->c[0] && !g_lower.rk.user_write_meth && (!strcmp(mname, "say") || !strcmp(mname, "print"))) {
                const tree_t * inv = t->c[0];
                const char * wfn = !strcmp(mname, "print") ? "rk_writes" : "rk_write";
                if (!strcmp(mname, "say")) { if (rk_yields_array(inv)) wfn = "rk_write_arr"; else if (rk_yields_list(inv)) wfn = "rk_write_list"; }
                return lower_rcall1(cx, inv, wfn, γ, ω, res);
            }
            if (mname && rk_meth_is_bool(mname)) return lower_rcall_bool(cx, t, "meth_call", 0, γ, ω, res);
            return lower_rcall(cx, t, "meth_call", 0, γ, ω, res);
        }
        case TT_NEW:
        {
            const char * cls = (t->n > 0 && t->c[0] && t->c[0]->v.sval) ? t->c[0]->v.sval : NULL;
            char mbase[cls ? fmt_len("%s__new", cls) : 1];
            mbase[0] = 0;
            if (cls) snprintf(mbase, sizeof mbase, "%s__new", cls);
            if (cls && rk_is_multi_name(mbase)) {
                tree_t * mc = ast_node_new(TT_METHCALL);
                mc->line = t->line;
                ast_push(mc, rk_typeobj_tree(cls));
                ast_push(mc, leaf_sval2(TT_QLIT, "new"));
                for (int i = 1; i < t->n; i++) ast_push(mc, t->c[i]);
                return lower_rv(cx, mc, γ, ω, res);
            }
            if (cls && !rk_is_class_name(cls) && (!strcmp(cls, "Int") || !strcmp(cls, "Num") || !strcmp(cls, "Str")) && t->n <= 2) {
                if (t->n == 1 && cls[0] == 'S') { tree_t * e = leaf_sval2(TT_QLIT, ""); return lower_rv(cx, e, γ, ω, res); }
                tree_t * mc = ast_node_new(TT_METHCALL);
                mc->line = t->line;
                ast_push(mc, t->n == 2 ? t->c[1] : rk_ti(0));
                ast_push(mc, leaf_sval2(TT_QLIT, cls));
                return lower_rv(cx, mc, γ, ω, res);
            }
            if (cls && !rk_is_class_name(cls) && (!strcmp(cls, "Array") || !strcmp(cls, "List"))) {
                tree_t * ar = rk_fnc2(cls[0] == 'A' ? "__rk_arr_lit" : "__rk_arr", NULL, NULL);
                ar->line = t->line;
                for (int i = 1; i < t->n; i++) ast_push(ar, t->c[i]);
                return lower_rv(cx, ar, γ, ω, res);
            }
            if (cls && !rk_is_class_name(cls) && !strcmp(cls, "Failure") && t->n <= 2) {
                tree_t * fc = rk_fnc2("__rk_failure", t->n == 2 ? t->c[1] : NULL, NULL);
                fc->line = t->line;
                return lower_rv(cx, fc, γ, ω, res);
            }
            if (cls && !rk_is_class_name(cls) && !strcmp(cls, "Slip")) {
                tree_t * ar = rk_fnc2("__rk_arr", NULL, NULL);
                ar->line = t->line;
                for (int i = 1; i < t->n; i++) ast_push(ar, t->c[i]);
                return lower_rv(cx, rk_fnc2("__rk_arr_slip", ar, NULL), γ, ω, res);
            }
            if (cls && !rk_is_class_name(cls) && !strcmp(cls, "Hash") && t->n == 1) { tree_t * h = rk_fnc2("__rk_hash", NULL, NULL); return lower_rv(cx, h, γ, ω, res); }
            if (cls && (t->n == 3 || t->n == 1) && !rk_is_class_name(cls) && (!strcmp(cls, "Rat") || !strcmp(cls, "FatRat"))) {
                tree_t * rn = ast_node_new(TT_FNC);
                rn->line = t->line;
                rn->v.sval = (char *) intern("__rk_ratnew");
                ast_push(rn, leaf_sval2(TT_VAR, "__rk_ratnew"));
                ast_push(rn, leaf_sval2(TT_QLIT, cls));
                ast_push(rn, t->n == 3 ? t->c[1] : ({ tree_t * z = ast_node_new(TT_ILIT); z->v.ival = 0; z; }));
                ast_push(rn, t->n == 3 ? t->c[2] : ({ tree_t * z = ast_node_new(TT_ILIT); z->v.ival = 1; z; }));
                return lower_rv(cx, rn, γ, ω, res);
            }
            if (cls && !rk_is_class_name(cls) && (!strcmp(cls, "Set") || !strcmp(cls, "SetHash") || !strcmp(cls, "Bag") || !strcmp(cls, "BagHash") || !strcmp(cls, "Mix") || !strcmp(cls, "MixHash"))) {
                tree_t * ar = ast_node_new(TT_FNC);
                ar->v.sval = (char *) "__rk_arr";
                ast_push(ar, leaf_sval2(TT_VAR, "__rk_arr"));
                for (int i = 1; i < t->n; i++) ast_push(ar, t->c[i]);
                char mn[fmt_len("__rk_new_%s", cls)];
                snprintf(mn, sizeof mn, "__rk_new_%s", cls);
                tree_t * mc = ast_node_new(TT_METHCALL);
                mc->line = t->line;
                ast_push(mc, ar);
                ast_push(mc, leaf_sval2(TT_QLIT, intern(mn)));
                return lower_rv(cx, mc, γ, ω, res);
            }
            return lower_rcall(cx, t, "obj_new", 0, γ, ω, res);
        }
        case TT_TWIGIL_FIELD:
        { IR_t * nd = lrk_build(cx, IR_FIELD_GET, γ, ω); IR_LIT(nd).sval = t->v.sval; IR_t * sv = lrk_build(cx, IR_VAR, nd, ω); IR_LIT(sv).sval = "self"; ir_operand_push(nd, sv); *res = nd; return sv; }
        case TT_FIELD:
        {
            const char * fname = (t->n > 1 && t->c[1]) ? t->c[1]->v.sval : t->v.sval;
            IR_t * nd = lrk_build(cx, IR_CALL, γ, ω);
            IR_LIT(nd).sval = "field_get_pub";
            IR_t * nl = lrk_build(cx, IR_LIT_STRING, nd, ω);
            IR_LIT(nl).sval = fname;
            IR_t * or_ = NULL;
            IR_t * eo = lower_rv(cx, t->c[0], nl, ω, &or_);
            if (or_) ir_operand_push(nd, or_);
            ir_operand_push(nd, nl);
            *res = nd;
            return eo;
        }
        case TT_SUSPEND:
        if (t->v.ival == 1 && t->n > 0 && t->c[0] && rk_user_meth_named("take")) {
            tree_t * mc = ast_node_new(TT_METHCALL);
            ast_push(mc, t->c[0]);
            ast_push(mc, leaf_sval2(TT_QLIT, "take"));
            return lower_rv(cx, mc, γ, ω, res);
        }
        if (cx->cur_proc_name && rk_proc_is_generator(cx->cur_proc_name)) {
            IR_t * sn = lrk_build(cx, IR_SUSPEND, cx->proc_exit ? cx->proc_exit : γ, ω);
            IR_LIT(sn).dval = 1.0;
            IR_LIT(sn).sval = (char *) cx->cur_proc_name;
            IR_t * ev = NULL;
            IR_t * e_entry = sn;
            if (t->n > 0 && t->c[0]) e_entry = lower_rv(cx, t->c[0], sn, ω, &ev);
            ir_operand_push(sn, ev);
            ir_operand_push(sn, γ);
            sn->seal = (γ && ir_is_generator_kind(γ->op)) ? 0 : 1;
            *res = sn;
            return e_entry;
        }
        return lower_rcall(cx, t, "__rk_take_dyn", 0, γ, ω, res);
        return rk_excise(cx, γ, ω, res);
        case TT_TERNARY:
        if (t->n > 2) {
            char tn[32];
            snprintf(tn, sizeof tn, "$?tern%d", g_lower.rk.lower_rv_tern_n++);
            const char * tname = lp_strdup(tn);
            IR_t * jv = lrk_build(cx, IR_VAR, γ, ω);
            IR_LIT(jv).sval = tname;
            int dt = rk_diverges(t->c[1]), df = rk_diverges(t->c[2]);
            IR_t * at = dt ? NULL : lrk_build(cx, IR_ASSIGN, jv, ω);
            if (at) IR_LIT(at).sval = tname;
            IR_t * af = df ? NULL : lrk_build(cx, IR_ASSIGN, jv, ω);
            if (af) IR_LIT(af).sval = tname;
            IR_t * et = at ? rk_tern_branch(cx, t->c[1], at, ω) : lower_rv(cx, t->c[1], jv, ω, NULL);
            IR_t * ef = af ? rk_tern_branch(cx, t->c[2], af, ω) : lower_rv(cx, t->c[2], jv, ω, NULL);
            IR_t * e = lower_cond(cx, t->c[0], et, ef);
            *res = jv;
            return e;
        }
        { IR_t * s = lrk_build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
        case TT_RETURN:
        {
            IR_t * exit_γ = cx->proc_exit ? cx->proc_exit : γ;
            if (t->n > 0 && t->c[0]) {
                IR_t * nd = lrk_build(cx, IR_RETURN, exit_γ, ω);
                IR_t * trace = NULL;
                IR_t * trace_entry = nd;
                if (cx->cur_proc_name && *cx->cur_proc_name && trace_wanted() && !rk_trace_is_block(cx->cur_proc_name)) {
                    trace = lrk_build(cx, IR_CALL, nd, ω);
                    IR_LIT(trace).sval = "__trace_return";
                    IR_t * nm = lrk_build(cx, IR_LIT_STRING, trace, ω);
                    IR_LIT(nm).sval = rk_trace_name(cx->cur_proc_name);
                    ir_operand_push(trace, nm);
                    trace_entry = nm;
                }
                IR_t * value_γ = trace_entry;
                for (int td = 0; td < cx->try_depth; td++) { IR_t * te = lrk_build(cx, IR_CALL, value_γ, ω); IR_LIT(te).sval = "try_exit"; value_γ = te; }
                IR_t * r = NULL;
                IR_t * e = lower_rv(cx, t->c[0], value_γ, ω, &r);
                ir_operand_push(nd, r ? r : e);
                if (trace) ir_operand_push(trace, r ? r : e);
                *res = nd;
                return e;
            }
            IR_t * nd = lrk_build(cx, IR_RETURN, exit_γ, ω);
            IR_t * rentry = nd;
            for (int td = 0; td < cx->try_depth; td++) { IR_t * te = lrk_build(cx, IR_CALL, rentry, ω); IR_LIT(te).sval = "try_exit"; rentry = te; }
            *res = nd;
            return rentry;
        }
        default:
        { IR_t * s = lrk_build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    }
}
