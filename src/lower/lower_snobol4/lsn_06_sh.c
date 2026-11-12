#include "lower/lower_snobol4/lsn_head.h"
#include "lower/lower_snobol4/lsn_01_sno.h"
#include "lower/lower_snobol4/lsn_02_sno.h"
#include "lower/lower_snobol4/lsn_03_sno.h"
#include "lower/lower_snobol4/lsn_04_sno.h"
#include "lower/lower_snobol4/lsn_05_sno.h"
#include "lower/lower_snobol4/lsn_06_sh.h"
#include "lower/lower_snobol4/lsn_08_sno.h"
#include "lower/lower_snobol4/lsn_09_sno.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * sx_lower(scx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    if (!t) { IR_t * nd = lc_build(cx->g, IR_LIT_STRING, γ, ω); IR_LIT(nd).sval = (char *) ""; if (res) *res = nd; return nd; }
    if (sno_mkpat_here(t)) return sno_mkpat_emit(cx, t, γ, ω, res);
    switch (t->t) {
        case TT_ILIT:
        { IR_t * nd = lc_build(cx->g, IR_LIT_INTEGER, γ, ω); IR_LIT(nd).ival = t->v.ival; if (res) *res = nd; return nd; }
        case TT_FLIT:
        { IR_t * nd = lc_build(cx->g, IR_LIT_REAL, γ, ω); IR_LIT(nd).dval = t->v.dval; if (res) *res = nd; return nd; }
        case TT_QLIT:
        { IR_t * nd = lc_build(cx->g, IR_LIT_STRING, γ, ω); IR_LIT(nd).sval = t->v.sval ? t->v.sval : (char *) ""; if (res) *res = nd; return nd; }
        case TT_NUL:
        { IR_t * nd = lc_build(cx->g, IR_LIT_STRING, γ, ω); IR_LIT(nd).sval = (char *) ""; if (res) *res = nd; return nd; }
        case TT_VAR:
        { sno_reg_var(t->v.sval); IR_t * nd = lc_build(cx->g, IR_VAR, γ, ω); IR_LIT(nd).sval = t->v.sval; if (res) *res = nd; return nd; }
        case TT_KEYWORD:
        {
            if (t->v.sval) {
                char cb[fmt_len("&%s", t->v.sval[0] == '&' ? t->v.sval + 1 : t->v.sval)];
                snprintf(cb, sizeof cb, "&%s", t->v.sval[0] == '&' ? t->v.sval + 1 : t->v.sval);
                const tree_t * cv = sno_const_val(cb);
                if (cv) return sx_lower(cx, cv, γ, ω, res);
            }
            IR_t * nd = lc_build(cx->g, IR_KW_SNOBOL4, γ, ω);
            IR_LIT(nd).sval = t->v.sval ? t->v.sval : (char *) "";
            if (res) *res = nd;
            return nd;
        }
        case TT_DEFER:
        {
            const char * bn = sno_expr_collect((t->n > 0) ? t->c[0] : NULL);
            IR_t * mk = lc_build(cx->g, IR_CALL, γ, ω);
            IR_LIT(mk).sval = (char *) "SNO$MKEXPR";
            IR_t * nl = lc_build(cx->g, IR_LIT_DSTAR, mk, ω);
            IR_LIT(nl).sval = (char *) bn;
            ir_operand_push(mk, nl);
            if (res) *res = mk;
            return nl;
        }
        case TT_NAME:
        {
            if (t->n < 1 || !t->c[0]) sno_fatal("name operator with no operand", NULL);
            if (t->c[0]->t == TT_VAR && t->c[0]->v.sval) {
                sno_reg_var(t->c[0]->v.sval);
                IR_t * nl = lc_build(cx->g, IR_LIT_NAME, γ, ω);
                IR_LIT(nl).sval = t->c[0]->v.sval;
                if (res) *res = nl;
                return nl;
            }
            if (t->c[0]->t == TT_IDX && t->c[0]->n >= 2) {
                const tree_t * ix = t->c[0];
                IR_t * vr = NULL;
                IR_t * entry = sx_subscript_lv(cx, ix->c[0], (const tree_t * const *) &ix->c[1], ix->n - 1, ω, &vr);
                if (γ) lc_γ_to(vr, γ);
                if (res) *res = vr;
                return entry;
            }
            if (t->c[0]->t == TT_FNC) {
                const tree_t * fn = t->c[0];
                const char * fname = fn->v.sval;
                int argbase = 0;
                if (!fname && fn->n > 0 && fn->c[0] && fn->c[0]->t == TT_VAR) { fname = fn->c[0]->v.sval; argbase = 1; }
                if (sno_item_argbase(fn) >= 0) {
                    IR_t * vr = NULL;
                    IR_t * entry = sx_subscript_lv(cx, fn->c[argbase], (const tree_t * const *) &fn->c[argbase + 1], fn->n - argbase - 1, ω, &vr);
                    if (γ) lc_γ_to(vr, γ);
                    if (res) *res = vr;
                    return entry;
                }
                extern int rt_dat_field_of_any(const char *);
                if (fname && (fn->n - argbase) == 1 && rt_dat_field_of_any(fname)) {
                    IR_t * br = NULL;
                    IR_t * ea = sx_lower(cx, fn->c[argbase], NULL, ω, &br);
                    IR_t * fv = lc_build(cx->g, IR_FIELD_VAR, γ, ω);
                    IR_LIT(fv).sval = (char *) lp_strdup(fname);
                    lc_γ_to(br, fv);
                    ir_operand_push(fv, br);
                    if (res) *res = fv;
                    return ea;
                }
                IR_t * cv = NULL;
                IR_t * e1 = sno_lower_byname(cx, t->c[0], γ, ω, &cv);
                if (res) *res = cv;
                return e1;
            }
            if (t->c[0]->t == TT_KEYWORD && t->c[0]->v.sval) {
                const char * kn = t->c[0]->v.sval;
                const char * cb = sno_lead_name("&&", kn[0] == '&' ? kn + 1 : kn);
                IR_t * nl = lc_build(cx->g, IR_LIT_NAME, γ, ω);
                IR_LIT(nl).sval = (char *) cb;
                if (res) *res = nl;
                return nl;
            }
            if (t->c[0]->t == TT_INDIRECT && t->c[0]->n > 0 && t->c[0]->c[0]) return sx_nameval(cx, t->c[0]->c[0], γ, ω, res);
            if (t->c[0]->t == TT_DEFER && t->c[0]->n > 0 && t->c[0]->c[0] && t->c[0]->c[0]->t == TT_VAR && t->c[0]->c[0]->v.sval) {
                sno_reg_var(t->c[0]->c[0]->v.sval);
                IR_t * nl = lc_build(cx->g, IR_LIT_NAME, γ, ω);
                IR_LIT(nl).sval = t->c[0]->c[0]->v.sval;
                if (res) *res = nl;
                return nl;
            }
            if (t->c[0]->t == TT_DEFER) {
                IR_t * mk = lc_build(cx->g, IR_CALL, γ, ω);
                IR_LIT(mk).sval = (char *) "SNO$XNAME";
                IR_t * nr = NULL, * nr2 = NULL, * after = mk;
                const tree_t * de = t->c[0]->n > 0 ? t->c[0]->c[0] : NULL;
                if (de && de->t != TT_VAR && sno_name_wrappable(de)) {
                    tree_t * nm = ast_node_new(TT_NAME);
                    ast_push(nm, (tree_t *) de);
                    tree_t * nt = ast_node_new(TT_DEFER);
                    ast_push(nt, nm);
                    after = sx_lower(cx, nt, mk, ω, &nr2);
                }
                IR_t * ne = sx_lower(cx, t->c[0], after, ω, &nr);
                if (nr) ir_operand_push(mk, nr);
                if (nr2) ir_operand_push(mk, nr2);
                if (res) *res = mk;
                return ne;
            }
            if (sno_pe_patfn(t->c[0]->t)) {
                IR_t * chk = lc_build(cx->g, IR_CALL, γ, ω);
                IR_LIT(chk).sval = (char *) "SNO$CHKNM";
                IR_t * nlit = lc_build(cx->g, IR_LIT_STRING, chk, ω);
                IR_LIT(nlit).sval = (char *) "";
                IR_t * wm = lc_build(cx->g, IR_CALL, nlit, ω);
                IR_LIT(wm).sval = (char *) "SNO$WANTNM";
                IR_t * wl = lc_build(cx->g, IR_LIT_STRING, wm, ω);
                IR_LIT(wl).sval = (char *) "";
                ir_operand_push(wm, wl);
                IR_t * ar = NULL;
                IR_t * e = sx_lower(cx, t->c[0], wl, ω, &ar);
                ir_operand_push(chk, ar);
                ir_operand_push(chk, nlit);
                if (res) *res = chk;
                return e;
            }
            sno_fatal("name operator over this form is outside the landed subset", NULL);
        }
        case TT_ANY:
        case TT_NOTANY:
        case TT_SPAN:
        case TT_BREAK:
        case TT_BREAKX:
        {
            if (t->n < 1) sno_fatal("charset pattern function with missing operand", NULL);
            IR_t * mk = lc_build(cx->g, IR_CALL, γ, ω);
            IR_LIT(mk).sval = (char *) "SNO$PBK";
            IR_t * kt = lc_build(cx->g, IR_LIT_INTEGER, NULL, ω);
            IR_LIT(kt).ival = (int64_t) t->t;
            IR_t * va = NULL;
            IR_t * ea = sx_lower(cx, t->c[0], NULL, ω, &va);
            lc_γ_to(kt, ea);
            lc_γ_to(va, mk);
            ir_operand_push(mk, kt);
            ir_operand_push(mk, va);
            if (res) *res = mk;
            return kt;
        }
        case TT_LEN:
        case TT_TAB:
        case TT_RTAB:
        case TT_POS:
        case TT_RPOS:
        {
            if (t->n < 1) sno_fatal("integer pattern function with missing operand", NULL);
            IR_t * mk = lc_build(cx->g, IR_CALL, γ, ω);
            IR_LIT(mk).sval = (char *) "SNO$PBN";
            IR_t * kt = lc_build(cx->g, IR_LIT_INTEGER, NULL, ω);
            IR_LIT(kt).ival = (int64_t) t->t;
            IR_t * va = NULL;
            IR_t * ea = sx_lower(cx, t->c[0], NULL, ω, &va);
            lc_γ_to(kt, ea);
            lc_γ_to(va, mk);
            ir_operand_push(mk, kt);
            ir_operand_push(mk, va);
            if (res) *res = mk;
            return kt;
        }
        case TT_ARB:
        case TT_REM:
        case TT_BAL:
        case TT_FAIL:
        case TT_SUCCEED:
        case TT_ABORT:
        case TT_FLUSH:
        {
            if (t->v.sval && sno_predef_registered(t->v.sval)) return sx_call_named(cx, t->v.sval, t, 0, γ, ω, res);
            IR_t * mk = lc_build(cx->g, IR_CALL, γ, ω);
            IR_LIT(mk).sval = (char *) "SNO$PB0";
            IR_t * kt = lc_build(cx->g, IR_LIT_INTEGER, mk, ω);
            IR_LIT(kt).ival = (int64_t) t->t;
            ir_operand_push(mk, kt);
            if (res) *res = mk;
            return kt;
        }
        case TT_CAPT_COND_ASGN:
        case TT_CAPT_IMMED_ASGN:
        {
            const tree_t * tgt = (t->n > 1) ? t->c[1] : NULL;
            const char * vn = (tgt && tgt->t == TT_VAR) ? tgt->v.sval : sno_capt_name(tgt);
            if (vn) sno_reg_var(vn);
            if (!vn) vn = sno_capt_item_name(tgt);
            if (!vn && tgt && tgt->t == TT_DEFER) {
                const tree_t * di = (tgt->n > 0) ? tgt->c[0] : NULL;
                const char * bn = ((di && di->t == TT_INDIRECT && di->n > 0 && di->c[0]) || (di && di->t == TT_IDX && di->n > 0) || sno_item_argbase(di) >= 0) ? sno_expr_collect_nm(di) :
                    sno_expr_collect(di);
                char pb[fmt_len("*%s", bn)];
                snprintf(pb, sizeof pb, "*%s", bn);
                vn = lp_strdup(pb);
            }
            IR_t * mk = lc_build(cx->g, IR_CALL, γ, ω);
            IR_LIT(mk).sval = (char *) "SNO$PBC";
            IR_t * kt = lc_build(cx->g, IR_LIT_INTEGER, NULL, ω);
            IR_LIT(kt).ival = (int64_t) t->t;
            IR_t * nl;
            IR_t * es;
            IR_t * vs = NULL;
            if (vn) {
                int vn_star = vn[0] == '*' && sno_expr_is_thunk(vn + 1);
                nl = lc_build(cx->g, vn_star ? IR_LIT_DSTAR : IR_LIT_STRING, NULL, ω);
                IR_LIT(nl).sval = (char *) (vn_star ? vn + 1 : vn);
                es = sx_lower(cx, t->c[0], NULL, ω, &vs);
                lc_γ_to(kt, nl);
                lc_γ_to(nl, es);
            } else if (tgt && tgt->t == TT_INDIRECT) {
                IR_t * nv = NULL;
                IR_t * en = sx_lower(cx, (tgt->n > 0) ? tgt->c[0] : NULL, NULL, ω, &nv);
                es = sx_lower(cx, t->c[0], NULL, ω, &vs);
                lc_γ_to(kt, en);
                lc_γ_to(nv, es);
                nl = nv;
            } else if (tgt && tgt->t == TT_FNC) {
                IR_t * nv = NULL;
                IR_t * en = sno_lower_byname(cx, tgt, NULL, ω, &nv);
                es = sx_lower(cx, t->c[0], NULL, ω, &vs);
                lc_γ_to(kt, en);
                lc_γ_to(nv, es);
                nl = nv;
            } else {
                sno_fatal("capture target in a runtime-built pattern is not a simple variable", NULL);
                return NULL;
            }
            lc_γ_to(vs, mk);
            ir_operand_push(mk, kt);
            ir_operand_push(mk, nl);
            ir_operand_push(mk, vs);
            if (res) *res = mk;
            return kt;
        }
        case TT_ARBNO:
        {
            if (t->n < 1) sno_fatal("ARBNO with missing operand", NULL);
            IR_t * mk = lc_build(cx->g, IR_CALL, γ, ω);
            IR_LIT(mk).sval = (char *) "SNO$PARB";
            IR_t * vi = NULL;
            IR_t * ei = sx_lower(cx, t->c[0], NULL, ω, &vi);
            lc_γ_to(vi, mk);
            ir_operand_push(mk, vi);
            if (res) *res = mk;
            return ei;
        }
        case TT_FENCE:
        {
            if (t->n == 0) {
                IR_t * mk0 = lc_build(cx->g, IR_CALL, γ, ω);
                IR_LIT(mk0).sval = (char *) "SNO$PB0";
                IR_t * kt0 = lc_build(cx->g, IR_LIT_INTEGER, mk0, ω);
                IR_LIT(kt0).ival = (int64_t) TT_FENCE;
                ir_operand_push(mk0, kt0);
                if (res) *res = mk0;
                return kt0;
            }
            IR_t * mk = lc_build(cx->g, IR_CALL, γ, ω);
            IR_LIT(mk).sval = (char *) "SNO$PFEN";
            IR_t * vi = NULL;
            IR_t * ei = sx_lower(cx, t->c[0], NULL, ω, &vi);
            lc_γ_to(vi, mk);
            ir_operand_push(mk, vi);
            if (res) *res = mk;
            return ei;
        }
        case TT_ALT:
        {
            if (t->n < 2) sno_fatal("alternation with missing operand", NULL);
            IR_t * mk = lc_build(cx->g, IR_CALL, γ, ω);
            IR_LIT(mk).sval = (char *) "SNO$PBALT";
            IR_t * vl = NULL;
            IR_t * el = sx_lower(cx, t->c[0], NULL, ω, &vl);
            IR_t * vr = NULL;
            IR_t * er = sx_lower(cx, t->c[1], NULL, ω, &vr);
            lc_γ_to(vl, er);
            lc_γ_to(vr, mk);
            ir_operand_push(mk, vl);
            ir_operand_push(mk, vr);
            if (res) *res = mk;
            return el;
        }
        case TT_ADD:
        case TT_SUB:
        case TT_MUL:
        case TT_DIV:
        case TT_POW:
        case TT_SEQ:
        case TT_CAT:
        if (t->n < 2) sno_fatal("binary operator with missing operand", NULL);
        return sx_binop(cx, t, sno_binop_code(t->t), γ, ω, res);
        case TT_MNS:
        case TT_PLS:
        {
            if (t->n < 1) sno_fatal("unary operator with missing operand", NULL);
            IR_t * op = lc_build(cx->g, IR_UNOP, γ, ω);
            IR_LIT(op).ival = (long long) t->t;
            op->strict = 2;
            IR_t * ar = NULL;
            IR_t * ea = sx_lower(cx, t->c[0], op, ω, &ar);
            ir_operand_push(op, ar);
            if (res) *res = op;
            return ea;
        }
        case TT_INDIRECT:
        {
            if (t->n < 1) sno_fatal("indirect reference with no operand", NULL);
            if (t->c[0] && t->c[0]->t == TT_NAME && !(g_lower.sno.uses_xname && sno_is_xname(t->c[0]))) {
                IR_t * dr = lc_build(cx->g, IR_DEREF, γ, ω);
                IR_t * nr = NULL;
                IR_t * ne = sx_lower(cx, t->c[0], NULL, ω, &nr);
                lc_γ_to(nr, dr);
                ir_operand_push(dr, nr);
                if (res) *res = dr;
                return ne;
            }
            if (g_lower.sno.uses_xname) {
                IR_graph_t * g = cx->g;
                char * tn = sno_scan_tmp("SNO$XR");
                char * xn = sno_scan_tmp("SNO$XT");
                IR_t * join = lc_build(g, IR_VAR, γ, ω);
                IR_LIT(join).sval = tn;
                IR_t * asn_d = lc_build(g, IR_ASSIGN, join, ω);
                IR_LIT(asn_d).sval = tn;
                IR_t * dr = lc_build(g, IR_DEREF, asn_d, ω);
                ir_operand_push(asn_d, dr);
                tree_t * ev = ast_node_new(TT_FNC);
                ev->v.sval = (char *) "EVAL";
                tree_t * xv = ast_node_new(TT_VAR);
                xv->v.sval = xn;
                ast_push(ev, xv);
                IR_t * asn_e = lc_build(g, IR_ASSIGN, join, ω);
                IR_LIT(asn_e).sval = tn;
                IR_t * evr = NULL;
                IR_t * ee = sx_lower(cx, ev, asn_e, ω, &evr);
                ir_operand_push(asn_e, evr);
                char * vn = sno_scan_tmp("SNO$XV");
                IR_t * rd = lc_build(g, IR_VAR, dr, ω);
                IR_LIT(rd).sval = vn;
                ir_operand_push(dr, rd);
                IR_t * xa = lc_build(g, IR_ASSIGN, ee, rd);
                IR_LIT(xa).sval = xn;
                IR_t * xt = lc_build(g, IR_CALL, xa, rd);
                IR_LIT(xt).sval = (char *) "SNO$XTHUNK";
                ir_operand_push(xa, xt);
                IR_t * av = lc_build(g, IR_ASSIGN, xt, ω);
                IR_LIT(av).sval = vn;
                IR_t * vr = NULL;
                IR_t * ve = sx_nameval(cx, t->c[0], av, ω, &vr);
                ir_operand_push(av, vr);
                ir_operand_push(xt, vr);
                if (res) *res = join;
                return ve;
            }
            IR_t * dr = lc_build(cx->g, IR_DEREF, γ, ω);
            IR_t * vr = NULL;
            IR_t * ve = sx_nameval(cx, t->c[0], dr, ω, &vr);
            ir_operand_push(dr, vr);
            if (res) *res = dr;
            return ve;
        }
        case TT_IDX:
        {
            IR_t * cur = NULL;
            IR_t * entry = sx_idx_container(cx, t, ω, &cur);
            if (cur && cur->op == IR_SUBSCRIPT && t->n == 2 && IR_LIT(cur).sval && !strcmp(IR_LIT(cur).sval, "container-only")) {
                IR_LIT(cur).sval = "container-value";
                if (sno_sub_val_on()) { lc_γ_to(cur, γ); if (res) *res = cur; return entry; }
            }
            IR_t * dr = lc_build(cx->g, IR_DEREF, γ, ω);
            lc_γ_to(cur, dr);
            ir_operand_push(dr, cur);
            if (res) *res = dr;
            return entry;
        }
        case TT_OPSYN:
        {
            const char * name = t->v.sval;
            if (!name || !*name) sno_fatal("OPSYN operator expression with no symbol", NULL);
            { const char * _tg = sno_t4_target(name, t->n); if (_tg && (sno_predef_registered(_tg) || sno_tdef_registered(_tg))) return sx_call_named(cx, _tg, t, 0, γ, ω, res); }
            return sx_call_named(cx, name, t, 0, γ, ω, res);
        }
        case TT_FNC:
        {
            const char * name = t->v.sval;
            int argbase = 0;
            if (!name && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR) { name = t->c[0]->v.sval; argbase = 1; }
            if (!name) sno_fatal("call with no resolvable name", NULL);
            if (!strcmp(name, "DEFINE")) {
                char fnb[(t->n > argbase && t->c[argbase] && t->c[argbase]->t == TT_QLIT && t->c[argbase]->v.sval) ? strlen(t->c[argbase]->v.sval) + 1 : 1];
                fnb[0] = 0;
                if (t->n > argbase && t->c[argbase] && t->c[argbase]->t == TT_QLIT && t->c[argbase]->v.sval) {
                    const char * sp = t->c[argbase]->v.sval;
                    int k = 0;
                    for (; sp[k] && sp[k] != '(' && sp[k] != ' '; k++) fnb[k] = sp[k];
                    fnb[k] = 0;
                }
                if (sno_define_entry_computed(t, argbase)) fnb[0] = 0;
                if (fnb[0] && sno_predef_registered(fnb) && !sno_def_entry_absent(t, argbase) && t->c[argbase] && sno_qlit_fold(t->c[argbase])) {
                    sno_def_t d;
                    sno_parse_define(sno_qlit_fold(t->c[argbase]), sno_define_entry_opt(t, argbase), &d);
                    IR_t * nd = lc_build(cx->g, IR_LIT_STRING, γ, ω);
                    IR_LIT(nd).sval = (char *) "";
                    IR_t * bind = lc_build(cx->g, IR_DEFINE_BIND, nd, ω);
                    IR_LIT(bind).sval = lp_strdup(d.fname);
                    sno_bind_attach_entry(cx->g, bind, d.entry, ω);
                    sno_bind_attach_proto(cx->g, bind, &d, ω);
                    if (res) *res = nd;
                    return bind;
                }
                if (fnb[0] && sno_predef_registered(fnb) && !sno_fname_is_multiproto(fnb) && !sno_def_entry_absent(t, argbase)) {
                    IR_t * nd = lc_build(cx->g, IR_LIT_STRING, γ, ω);
                    IR_LIT(nd).sval = (char *) "";
                    if (res) *res = nd;
                    return nd;
                }
                if (fnb[0] && !({ g_runtime.eval.rt_fragment_emit; }) && !sno_fname_is_multiproto(fnb) && !sno_def_entry_absent(t, argbase))
                    sno_fatal(
                    "DEFINE in this expression position is outside the landed subset (literal-prototype DEFINE in a statement subject only; pattern/replacement-field and fragment DEFINE pending)",
                    NULL);
            }
            if (!strcmp(name, "CODE") && (t->n - argbase) == 1 && cx->prog_nstmt > 0) {
                IR_t * ar = NULL;
                IR_t * ae = sx_lower(cx, t->c[argbase], NULL, ω, &ar);
                IR_t * cl = lc_build(cx->g, IR_CALL, γ, ω);
                IR_LIT(cl).sval = (char *) "CODE";
                IR_t * bn = lc_build(cx->g, IR_LIT_INTEGER, cl, ω);
                IR_LIT(bn).ival = (int64_t) cx->prog_nstmt;
                lc_γ_to(ar, bn);
                ir_operand_push(cl, ar);
                ir_operand_push(cl, bn);
                if (cx->prog_endline > 0) { IR_t * el = lc_build(cx->g, IR_LIT_INTEGER, cl, ω); IR_LIT(el).ival = (int64_t) cx->prog_endline; lc_γ_to(bn, el); ir_operand_push(cl, el); }
                if (res) *res = cl;
                return ae;
            }
            {
                int lex = 0;
                long c1 = 0, c2 = 0;
                int rk = sno_pred_relop(name, &lex, &c1, &c2);
                if (rk >= 0 && t->n - argbase >= 1 && t->n - argbase <= 2) return sx_pred_cmp(cx, t, argbase, lex, rk, c1, c2, γ, ω, res);
            }
            {
                int nid = t->n - argbase;
                if (sno_ident_inline_on() && nid >= 1 && nid <= 2 && !sno_predef_registered(name)) {
                    if (!strcmp(name, "IDENT")) return sx_ident_differ(cx, t, argbase, 0, γ, ω, res);
                    if (!strcmp(name, "DIFFER")) return sx_ident_differ(cx, t, argbase, 1, γ, ω, res);
                }
            }
            return sx_call_named(cx, name, t, argbase, γ, ω, res);
        }
        case TT_WHILE:
        case TT_UNTIL:
        {
            const tree_t * C = (t->n > 0) ? t->c[0] : NULL;
            const tree_t * B = (t->n > 1) ? t->c[1] : NULL;
            if (!C) sno_fatal("loop with no condition", NULL);
            int is_until = (t->t == TT_UNTIL);
            IR_t * gate = lc_build(cx->g, IR_GOTO, NULL, NULL);
            IR_t * cr = NULL;
            IR_t * ce = is_until ? sx_lower(cx, C, γ, lc_build(cx->g, IR_SETEXIT_TEST, gate, gate), &cr) : sx_lower(cx, C, gate, lc_build(cx->g, IR_SETEXIT_TEST, γ, γ), &cr);
            IR_t * sv_exit = cx->loop_exit;
            IR_t * sv_next = cx->loop_next;
            cx->loop_exit = γ;
            cx->loop_next = ce;
            IR_t * be = B ? sco_branch(cx, B, ce, ω) : ce;
            cx->loop_exit = sv_exit;
            cx->loop_next = sv_next;
            lc_γ_to(gate, be);
            if (res) *res = NULL;
            return ce;
        }
        case TT_DO_WHILE:
        {
            const tree_t * B = (t->n > 0) ? t->c[0] : NULL;
            const tree_t * C = (t->n > 1) ? t->c[1] : NULL;
            if (!C) sno_fatal("do-while without condition outside the landed subset", NULL);
            IR_t * gate = lc_build(cx->g, IR_GOTO, NULL, NULL);
            IR_t * cr = NULL;
            IR_t * ce = sx_lower(cx, C, gate, lc_build(cx->g, IR_SETEXIT_TEST, γ, γ), &cr);
            IR_t * sv_exit = cx->loop_exit;
            IR_t * sv_next = cx->loop_next;
            cx->loop_exit = γ;
            cx->loop_next = ce;
            IR_t * be = B ? sco_branch(cx, B, ce, ω) : ce;
            cx->loop_exit = sv_exit;
            cx->loop_next = sv_next;
            lc_γ_to(gate, be);
            if (res) *res = NULL;
            return be;
        }
        case TT_FOR:
        {
            const tree_t * INIT = (t->n > 0) ? t->c[0] : NULL;
            const tree_t * C = (t->n > 1) ? t->c[1] : NULL;
            const tree_t * STEP = (t->n > 2) ? t->c[2] : NULL;
            const tree_t * B = (t->n > 3) ? t->c[3] : NULL;
            if (!C) sno_fatal("for-loop without condition outside the landed subset", NULL);
            IR_t * gate = lc_build(cx->g, IR_GOTO, NULL, NULL);
            IR_t * cr = NULL;
            IR_t * ce = sx_lower(cx, C, gate, lc_build(cx->g, IR_SETEXIT_TEST, γ, γ), &cr);
            IR_t * se = STEP ? sx_lower(cx, STEP, ce, ω, NULL) : ce;
            IR_t * sv_exit = cx->loop_exit;
            IR_t * sv_next = cx->loop_next;
            cx->loop_exit = γ;
            cx->loop_next = se;
            IR_t * be = B ? sco_branch(cx, B, se, ω) : se;
            cx->loop_exit = sv_exit;
            cx->loop_next = sv_next;
            lc_γ_to(gate, be);
            IR_t * ie = INIT ? sx_lower(cx, INIT, ce, ω, NULL) : ce;
            if (res) *res = NULL;
            return ie;
        }
        case TT_LOOP_BREAK:
        case TT_LOOP_NEXT:
        {
            if (t->n > 0 && t->c[0]) sno_fatal("labeled break/next outside the SCO-CF-3 subset", NULL);
            IR_t * tgt = (t->t == TT_LOOP_BREAK) ? cx->loop_exit : cx->loop_next;
            if (!tgt) sno_fatal("break/next outside any loop", NULL);
            IR_t * j = lc_build(cx->g, IR_GOTO, tgt, NULL);
            if (res) *res = NULL;
            return j;
        }
        case TT_SCAN:
        return sx_scan(cx, t, γ, ω, res);
        case TT_ASSIGN:
        return sx_assign(cx, t, γ, ω, res);
        case TT_IF:
        {
            const tree_t * C = (t->n > 0) ? t->c[0] : NULL;
            const tree_t * TH = (t->n > 1) ? t->c[1] : NULL;
            const tree_t * EL = (t->n > 2) ? t->c[2] : NULL;
            if (!C) sno_fatal("TT_IF with no condition", NULL);
            IR_t * th_entry = TH ? sco_branch(cx, TH, γ, ω) : γ;
            IR_t * el_entry = EL ? sco_branch(cx, EL, γ, ω) : γ;
            IR_t * cr = NULL;
            IR_t * ce = sx_lower(cx, C, th_entry, lc_build(cx->g, IR_SETEXIT_TEST, el_entry, el_entry), &cr);
            if (res) *res = NULL;
            return ce;
        }
        case TT_NOT:
        {
            const tree_t * inner = (t->n > 0) ? t->c[0] : NULL;
            if (!inner) sno_fatal("TT_NOT with no operand", NULL);
            IR_t * gate = lc_build(cx->g, IR_LIT_STRING, γ, NULL);
            IR_LIT(gate).sval = (char *) "";
            IR_t * r = NULL;
            IR_t * fl = lc_build(cx->g, IR_GOTO, ω, ω);
            IR_t * e = sx_lower(cx, inner, fl, gate, &r);
            if (res) *res = gate;
            return e;
        }
        case TT_CASE:
        {
            if (t->n < 1 || !t->c[0]) sno_fatal("TT_CASE with no subject", NULL);
            const tree_t * subj = t->c[0];
            const tree_t * def_body = NULL;
            for (int i = 1; i + 1 < t->n; i += 2) if (t->c[i] && t->c[i]->t == TT_NUL) def_body = t->c[i + 1];
            IR_t * sv_exit = cx->loop_exit;
            IR_t * sv_next = cx->loop_next;
            IR_t * else_tgt;
            if (def_body) { cx->loop_exit = γ; IR_t * db = sco_branch(cx, def_body, γ, ω); cx->loop_exit = sv_exit; else_tgt = db; } else else_tgt = γ;
            IR_t * chain = else_tgt;
            tree_t * swv = ast_node_new(TT_VAR);
            swv->v.sval = sno_scan_tmp("SWD");
            for (int i = t->n - 2; i >= 1; i -= 2) {
                const tree_t * v = t->c[i];
                const tree_t * b = t->c[i + 1];
                if (v && v->t == TT_NUL) continue;
                cx->loop_exit = γ;
                IR_t * body_entry = sco_branch(cx, b, γ, ω);
                cx->loop_exit = sv_exit;
                tree_t * idc = ast_node_new(TT_FNC);
                idc->v.sval = (char *) "IDENT";
                ast_push(idc, swv);
                ast_push(idc, (tree_t *) v);
                IR_t * ir = NULL;
                IR_t * te = sx_lower(cx, idc, body_entry, lc_build(cx->g, IR_SETEXIT_TEST, chain, chain), &ir);
                chain = te;
            }
            tree_t * swa = ast_node_new(TT_ASSIGN);
            ast_push(swa, swv);
            ast_push(swa, (tree_t *) subj);
            IR_t * swr = NULL;
            chain = sx_lower(cx, swa, chain, else_tgt, &swr);
            cx->loop_exit = sv_exit;
            cx->loop_next = sv_next;
            if (res) *res = NULL;
            return chain;
        }
        case TT_AUGOP:
        {
            const tree_t * L = (t->n > 0) ? t->c[0] : NULL;
            if (!L || L->t != TT_VAR || !L->v.sval || t->n < 2) sno_fatal("TT_AUGOP outside the landed subset (simple-variable lhs only)", NULL);
            int code;
            switch ((int) t->v.ival) {
                case TK_AUGPLUS:
                code = 0;
                break;
                case TK_AUGMINUS:
                code = 1;
                break;
                case TK_AUGSTAR:
                code = 2;
                break;
                case TK_AUGSLASH:
                code = 3;
                break;
                case TK_AUGMOD:
                code = 4;
                break;
                case TK_AUGPOW:
                code = (int)BINOP_POW_PROMOTE;
                break;
                default:
                sno_fatal("TT_AUGOP operator outside the landed subset", NULL);
                code = 0;
            }
            sno_reg_var(L->v.sval);
            IR_t * asn = lc_build(cx->g, IR_ASSIGN, γ, ω);
            IR_LIT(asn).sval = L->v.sval;
            IR_t * vr = NULL;
            IR_t * e = sx_binop(cx, t, code, asn, ω, &vr);
            ir_operand_push(asn, vr);
            if (res) *res = asn;
            return e;
        }
        case TT_RETURN:
        case TT_NRETURN:
        case TT_PROC_FAIL:
        {
            const char * rl = (t->t == TT_PROC_FAIL) ? "FRETURN" : (t->t == TT_NRETURN) ? "NRETURN" : "RETURN";
            IR_t * j = lc_build(cx->g, IR_GOTO, bb_label_landing(rl), NULL);
            if (res) *res = NULL;
            if (t->t == TT_RETURN && t->n > 0 && t->c[0] && cx->result_name) {
                sno_reg_var(cx->result_name);
                IR_t * asn = lc_build(cx->g, IR_ASSIGN, j, j);
                IR_LIT(asn).sval = (char *) cx->result_name;
                IR_t * vr = NULL;
                IR_t * e = sx_lower(cx, t->c[0], asn, j, &vr);
                ir_operand_push(asn, vr);
                return e;
            }
            return j;
        }
        case TT_GOTO_U:
        case TT_GOTO_S:
        case TT_GOTO_F:
        {
            const char * nm = (t->n > 0 && t->c[0] && t->c[0]->v.sval) ? t->c[0]->v.sval : t->v.sval;
            if (!nm || !nm[0]) sno_fatal("goto with no resolvable label", NULL);
            IR_t * land = bb_label_landing(nm);
            if (!land && (!strcmp(nm, "CONTINUE") || !strcmp(nm, "SCONTINUE") || !strcmp(nm, "ABORT"))) {
                land = lc_build(cx->g, IR_GOTO_DEFERRED, bb_label_landing("END"), NULL);
                IR_LIT(land).sval = lp_strdup(nm);
            }
            if (!land) sno_fatal("goto to unknown label", nm);
            IR_t * taken = lc_build(cx->g, IR_GOTO, land, NULL);
            if (res) *res = NULL;
            if (t->t == TT_GOTO_U) return taken;
            return lc_build(cx->g, IR_GOTO, (t->t == TT_GOTO_S) ? taken : γ, (t->t == TT_GOTO_S) ? γ : taken);
        }
        case TT_CAPT_CURSOR:
        {
            const char * cvn = sno_cursor_target((t->n > 0) ? t->c[0] : NULL);
            if (!cvn) sno_fatal("@ cursor-position capture target is not a simple variable", NULL);
            IR_t * mk = lc_build(cx->g, IR_CALL, γ, ω);
            IR_LIT(mk).sval = (char *) "SNO$PCUR";
            int cvn_star = cvn[0] == '*' && sno_expr_is_thunk(cvn + 1);
            IR_t * nl = lc_build(cx->g, cvn_star ? IR_LIT_DSTAR : IR_LIT_STRING, mk, ω);
            IR_LIT(nl).sval = (char *) (cvn_star ? cvn + 1 : cvn);
            ir_operand_push(mk, nl);
            if (res) *res = mk;
            return nl;
        }
        case TT_VLIST:
        {
            if (t->n <= 1) { const tree_t * first = (t->n > 0) ? t->c[0] : NULL; return sx_lower(cx, first, γ, ω, res); }
            {
                int cf = 1;
                for (int j = 0; j + 1 < t->n && cf; j++) cf = sno_vl_quiet(t->c[j]);
                if (cf) {
                    char * tn = sno_scan_tmp("SNO$VL");
                    IR_t * join = lc_build(cx->g, IR_VAR, γ, ω);
                    IR_LIT(join).sval = tn;
                    if (res) *res = join;
                    return sno_vlist_arms(cx, t, 0, tn, join, ω);
                }
            }
            IR_graph_t * g = cx->g;
            IR_t * dj = lc_build(g, IR_DISJUNCTION, γ, ω);
            int n = t->n;
            IR_t * resv[n];
            for (int j = 0; j < n; j++) {
                int before = g->n;
                IR_t * ar = NULL;
                IR_t * ej = sx_lower(cx, t->c[j], dj, dj, &ar);
                for (int k = before; k < g->n; k++) {
                    IR_t * x = g->all[k];
                    if (!x) continue;
                    if (x->ω.node == dj) { memcpy(x->ω.sz, "φ", 3); x->ω.sz[3] = 0; }
                    if (x->γ.node == dj) { if (x->op == IR_GOTO && x->ω.node == dj) memcpy(x->γ.sz, "φ", 3); else memcpy(x->γ.sz, "σ", 3); x->γ.sz[3] = 0; }
                }
                ir_operand_push(dj, ej);
                ir_operand_push(dj, dj);
                resv[j] = ar;
            }
            for (int j = 0; j < n; j++) ir_operand_push(dj, sno_arm_result(resv[j]));
            IR_LIT(dj).ival = (long) n;
            { extern void fc_vdj_register(const IR_t *); fc_vdj_register(dj); }
            if (res) *res = dj;
            return dj;
        }
        case TT_INTERROGATE:
        {
            if (t->n < 1 || !t->c[0]) sno_fatal("? interrogation with no operand", NULL);
            IR_t * nul = lc_build(cx->g, IR_LIT_STRING, γ, ω);
            IR_LIT(nul).sval = (char *) "";
            IR_t * entry = sx_lower(cx, t->c[0], nul, ω, NULL);
            if (res) *res = nul;
            return entry;
        }
        default:
        { char buf[64]; snprintf(buf, sizeof buf, "tree kind %d", (int) t->t); sno_fatal("expression form not in the landed subset", buf); return NULL; }
    }
}
