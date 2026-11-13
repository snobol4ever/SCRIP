#include "lower/lower_raku/lrk_head.h"
#include <string.h>
#include "ir/ir_node.h"
#include "ir/ir_graph.h"
#include "ir/stage2.h"
#include "runtime/builtins/gen_runtime.h"
#include "lower/lower_common.h"
#include "lower/lower_raku/lrk_01_sh.h"
#include "lower/lower_raku/lrk_02_rk.h"
#include "lower/lower_raku/lrk_03_rk.h"
#include "lower/lower_raku/lrk_04_rk.h"
#include "lower/lower_raku/lrk_06_rk.h"
#include "lower/lower_raku/lrk_09_sh.h"
static void rk_register_proc_at(const tree_t * proc, const char * name, int nparams, int lastix);
int rk_is_grammar_name(const char * nm) {
    if (!nm) return 0;
    for (uint32_t i = 0; i < g_lower.rk.gram_names.len; i++) if (!strcmp(CV_AT(g_lower.rk.gram_names, const char *, i), nm)) return 1;
    return 0;
}
const char * rk_qualified_type_gist(const char * nm) {
    const char * p = strrchr(nm, ':');
    const char * shortname = p ? p + 1 : nm;
    size_t ln = strlen(shortname);
    char * buf = (char *)ct_alloc(ln + 3);
    buf[0] = '(';
    memcpy(buf + 1, shortname, ln);
    buf[ln + 1] = ')';
    buf[ln + 2] = '\0';
    return buf;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_is_version_lit(const char * s) {
    if (!s || s[0] != 'v' || !isdigit((unsigned char) s[1])) return 0;
    for (const char * p = s + 1; *p; p++) if (!isdigit((unsigned char) *p) && *p != '.' && *p != '*' && !(*p == '+' && !p[1]) && !(isalpha((unsigned char) *p) && p[-1] == '.' && !p[1])) return 0;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_is_modeled_type(const char * ty) {
    if (!ty) return 0;
    static const char * const k[] = { "Int", "Num", "Rat", "Str", "Numeric", "Real", "Cool", "Bool", 0 };
    for (int i = 0; k[i]; i++) if (!strcmp(ty, k[i])) return 1;
    return rk_is_class_name(ty);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static uint64_t rk_param_byref_mask(const tree_t * proc, int nparams) {
    uint64_t brm = 0;
    if (!proc) return 0;
    for (int k = 0; k < nparams && k < 64 && (1 + k) < proc->n; k++) {
        const tree_t * pv = proc->c[1 + k];
        if (pv && pv->n > 0 && pv->c[0] && pv->c[0]->t == TT_QLIT && pv->c[0]->v.sval && !strcmp(pv->c[0]->v.sval, "@") && !(pv->slen & RK_PV_COPY)) brm |= (1ULL << k);
    }
    return brm;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_method_is_stub(const tree_t * m) {
    if (!m || m->t != TT_SUB_DECL) return 0;
    int bs = (int) m->v.ival;
    if (bs < 1) bs = 1;
    if (m->n - bs != 1) return 0;
    const tree_t * b = m->c[bs];
    return b && b->t == TT_YADA;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * rk_fld_bare(const char * s) { return (s && (s[0] == '.' || s[0] == '!')) ? s + 1 : s; }
static int rk_fld_priv(const char * s) { return (s && s[0] == '!') ? 1 : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * const RK_LISTLIKE_METHNAMES[] = { "keys", "values", "kv", "sort", "reverse", "grep", "map", "split", "words", "comb", NULL };
static int rk_listlike_idx(const char * nm) { if (!nm) return -1; for (int i = 0; RK_LISTLIKE_METHNAMES[i]; i++) if (!strcmp(RK_LISTLIKE_METHNAMES[i], nm)) return i; return -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_type_provides_real_method(const tree_t * decl, const char * mname) {
    if (!decl || !mname) return 0;
    for (int j = 1; j < decl->n; j++) {
        const tree_t * ch = decl->c[j];
        if (!ch || ch->t != TT_SUB_DECL || rk_method_is_stub(ch)) continue;
        const char * nm = (ch->n > 0 && ch->c[0] && ch->c[0]->v.sval) ? ch->c[0]->v.sval : NULL;
        if (!nm) continue;
        const char * dollar = strchr(nm, '$');
        size_t nl = dollar ? (size_t)(dollar - nm) : strlen(nm);
        if (strlen(mname) == nl && !strncmp(nm, mname, nl)) return 1;
    }
    return 0;
}
void lrk_γ_to(IR_t * nd, IR_t * t) { if (t && ir_is_generator_kind(t->op)) lc_γ_to_β(nd, t); else lc_γ_to(nd, t); }
void lrk_ω_to(IR_t * nd, IR_t * t) { if (t && ir_is_generator_kind(t->op)) lc_ω_to_β(nd, t); else lc_ω_to(nd, t); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_loop_open(rcx_t * cx, rk_loopfr_t * lf, IR_t * exit, IR_t * next) {
    lf->label = cx->loop_label;
    lf->exit = exit;
    lf->next = next;
    lf->redo = NULL;
    lf->up = cx->loops;
    cx->loop_label = NULL;
    cx->loops = lf;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_loop_close(rcx_t * cx, rk_loopfr_t * lf, IR_t * body) { cx->loops = lf->up; if (lf->redo && body) { lrk_γ_to(lf->redo, body); lrk_ω_to(lf->redo, body); } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_is_binop(tree_e tt) { switch (tt) { case TT_ADD: case TT_SUB: case TT_MUL: case TT_DIV: case TT_MOD: case TT_POW: case TT_CAT: case TT_XREP: return 1; default: return 0; } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int64_t rk_binop_code(tree_e tt) { switch (tt) { case TT_ADD: return BINOP_ADD_BIG; case TT_SUB: return BINOP_SUB_BIG; case TT_MUL: return BINOP_MUL_BIG; default: return lc_binop_code(tt); } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_relop_code(IR_t * op, tree_e tt) {
    IR_LIT(op).ival = lc_binop_code(tt);
    if (IR_LIT(op).ival >= BINOP_LT && IR_LIT(op).ival <= BINOP_NE) op->strict |= IR_STRICT_RELOP_NUMIFY_BLANK;
    if (IR_LIT(op).ival >= BINOP_SLT && IR_LIT(op).ival <= BINOP_SNE) op->strict |= IR_STRICT_RELOP_NUM_TEXT;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_io_lower_name(const char * nm) {
    static const char * const names[] = { "open", "close", "slurp", "spurt", "unlink", "mkdir", "rmdir", "copy", "rename", "move", "chmod", "dir", "chdir", "make-temp-file", "make-temp-dir", NULL };
    for (int i = 0; names[i]; i++) if (!strcmp(nm, names[i])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_yields_list(const tree_t * t) {
    if (!t) return 0;
    if (t->t == TT_FNC && t->n > 0 && t->c[0] && t->c[0]->v.sval) {
        const char * f = t->c[0]->v.sval;
        return !strcmp(f, "__rk_arr_slice") || !strcmp(f, "__rk_arr_pick") || !strcmp(f, "__rk_hyper_meth");
    }
    if (t->t == TT_METHCALL && t->n > 1 && t->c[1] && t->c[1]->v.sval) { int li = rk_listlike_idx(t->c[1]->v.sval); return li >= 0 && !g_lower.rk.listlike_overridden[li]; }
    if (t->t == TT_SORT || t->t == TT_REVERSE) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_yields_array(const tree_t * t) {
    if (!t) return 0;
    if (t->t == TT_VAR) return (t->slen & 2) != 0;
    return t->t == TT_FNC && t->n > 1 && t->c[0] && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, "__rk_hyper_meth") && rk_yields_array(t->c[1]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_is_relop(tree_e tt) {
    switch (tt) { case TT_LT: case TT_LE: case TT_GT: case TT_GE: case TT_EQ: case TT_NE: case TT_LEQ: case TT_LNE: case TT_LLT: case TT_LLE: case TT_LGT: case TT_LGE: return 1; default: return 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * lower_cond(rcx_t * cx, const tree_t * c, IR_t * on_true, IR_t * on_false) {
    if (c && c->t == TT_STMT) { const tree_t * sub = lrk_stmt_subj(c); if (sub) c = sub; }
    if (!c) return on_true;
    if (c->t == TT_DIVIS && c->n > 1) return lower_cond(cx, rk_divis_desugar(c), on_true, on_false);
    if (c->t == TT_DIE && cx->cond_om) { IR_t * dr = NULL; return lower_rv(cx, c, on_true, cx->cond_om, &dr); }
    if (rk_is_relop(c->t) && c->n > 1) {
        IR_t * op = lrk_build(cx, IR_BINOP_TEST, on_true, on_false);
        rk_relop_code(op, c->t);
        IR_t * lr = NULL, * rr = NULL;
        IR_t * ea = lower_rv(cx, c->c[0], NULL, on_false, &lr);
        IR_t * eb = lower_rv(cx, c->c[1], op, on_false, &rr);
        lrk_γ_to(lr, eb);
        ir_operand_push(op, lr);
        ir_operand_push(op, rr);
        return ea;
    }
    if (c->t == TT_NOT && c->n > 0) return lower_cond(cx, c->c[0], on_false, on_true);
    if (c->t == TT_ALT && c->n > 1) { IR_t * rhs = lower_cond(cx, c->c[1], on_true, on_false); return lower_cond(cx, c->c[0], on_true, rhs); }
    if (c->t == TT_SEQ && c->n > 1) { IR_t * rhs = lower_cond(cx, c->c[1], on_true, on_false); return lower_cond(cx, c->c[0], rhs, on_false); }
    if (c->t == TT_SMATCH && c->n > 3 && c->c[3] && c->c[2] && c->c[2]->v.sval && !strcmp(c->c[2]->v.sval, "match")) {
        tree_t * call = ast_node_new(TT_FNC);
        call->line = c->line;
        ast_push(call, c->c[0]);
        ast_push(call, c->c[1]);
        for (int k = 1; k < c->c[3]->n; k++) ast_push(call, c->c[3]->c[k]);
        IR_t * res = NULL;
        return lower_rcall(cx, call, "re_testx", 0, on_true, on_false, &res);
    }
    if (c->t == TT_SMATCH && c->n > 2 && c->c[2] && c->c[2]->v.sval && !strcmp(c->c[2]->v.sval, "match")) {
        IR_t * nd = lrk_build(cx, IR_CALL, on_true, on_false);
        IR_LIT(nd).sval = "re_test";
        IR_t * sr = NULL, * pr = NULL;
        IR_t * es = lower_rv(cx, c->c[0], NULL, on_false, &sr);
        IR_t * ep = lower_rv(cx, c->c[1], nd, on_false, &pr);
        lrk_γ_to(sr, ep);
        ir_operand_push(nd, sr);
        ir_operand_push(nd, pr);
        return es;
    }
    IR_t * bk = lrk_build(cx, IR_CALL, on_true, on_false);
    IR_LIT(bk).sval = "__rk_bool";
    IR_t * r = NULL;
    IR_t * e = lower_rv(cx, c, bk, on_false, &r);
    if (r) ir_operand_push(bk, r);
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * rk_lower_logical(rcx_t * cx, const tree_t * t, int is_or, IR_t * γ, IR_t * ω, IR_t ** res) {
    char tn[40];
    snprintf(tn, sizeof tn, "$?logic%d", cx->g->n);
    const char * tname = lp_strdup(tn);
    IR_t * jv = lrk_build(cx, IR_VAR, γ, ω);
    IR_LIT(jv).sval = tname;
    IR_t * ab = lrk_build(cx, IR_ASSIGN, jv, ω);
    IR_LIT(ab).sval = tname;
    IR_t * rb = NULL;
    IR_t * eb = lower_rv(cx, t->c[1], ab, ω, &rb);
    if (rb) ir_operand_push(ab, rb);
    IR_t * ce = is_or ? lower_cond(cx, leaf_sval2(TT_VAR, tname), jv, eb) : lower_cond(cx, leaf_sval2(TT_VAR, tname), eb, jv);
    IR_t * aa = lrk_build(cx, IR_ASSIGN, ce, ω);
    IR_LIT(aa).sval = tname;
    IR_t * ra = NULL;
    IR_t * ea = lower_rv(cx, t->c[0], aa, ω, &ra);
    if (ra) ir_operand_push(aa, ra);
    *res = jv;
    return ea;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * lower_field_set(rcx_t * cx, const tree_t * lhs, const tree_t * rhs, IR_t * γ, IR_t * ω, IR_t ** res) {
    const char * fname = (lhs->t == TT_TWIGIL_FIELD) ? lhs->v.sval : ((lhs->n > 1 && lhs->c[1]) ? lhs->c[1]->v.sval : lhs->v.sval);
    IR_t * nd = lrk_build(cx, IR_CALL, γ, ω);
    IR_LIT(nd).sval = (lhs->t == TT_TWIGIL_FIELD) ? "field_set" : "field_set_pub";
    IR_t * nl = lrk_build(cx, IR_LIT_STRING, NULL, ω);
    IR_LIT(nl).sval = fname;
    IR_t * or_ = NULL;
    IR_t * entry;
    if (lhs->t == TT_TWIGIL_FIELD) { IR_t * sv = lrk_build(cx, IR_VAR, nl, ω); IR_LIT(sv).sval = "self"; or_ = sv; entry = sv; } else { entry = lower_rv(cx, lhs->c[0], nl, ω, &or_); }
    IR_t * vr = NULL;
    IR_t * ev = lower_rv(cx, rhs, nd, ω, &vr);
    lrk_γ_to(nl, ev);
    if (or_) ir_operand_push(nd, or_);
    ir_operand_push(nd, nl);
    if (vr) ir_operand_push(nd, vr);
    if (res) *res = nd;
    return entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * rk_xf_body(rcx_t * cx, const tree_t * xf, int xfk, const char * vname, IR_t * valprod, const tree_t * body, IR_t * pump, IR_t * ω) {
    IR_t * bentry = lower_rblock(cx, body, pump, ω, NULL);
    IR_t * av = lrk_build(cx, IR_ASSIGN, bentry, ω);
    IR_LIT(av).sval = vname;
    if (xfk == 1) {
        IR_t * fr = NULL;
        IR_t * fe = lower_rv(cx, xf, av, ω, &fr);
        if (fr) ir_operand_push(av, fr);
        IR_t * au = lrk_build(cx, IR_ASSIGN, fe, ω);
        IR_LIT(au).sval = "_";
        ir_operand_push(au, valprod);
        return au;
    }
    if (xfk == 2) {
        IR_t * vr = lrk_build(cx, IR_VAR, av, ω);
        IR_LIT(vr).sval = "_";
        ir_operand_push(av, vr);
        IR_t * centry = lower_cond(cx, xf, vr, pump);
        IR_t * au = lrk_build(cx, IR_ASSIGN, centry, ω);
        IR_LIT(au).sval = "_";
        ir_operand_push(au, valprod);
        return au;
    }
    ir_operand_push(av, valprod);
    return av;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_reclassify_calls(void) {
    extern int rt_builtin_is_known(const char *);
    for (int gi = 0; gi < g_stage2.bbp.count; gi++) {
        IR_graph_t * g = g_stage2.bbp.table[gi];
        if (!g) continue;
        for (int i = 0; i < g->n; i++) {
            IR_t * nd = g->all[i];
            if (!nd || nd->op != IR_CALL) continue;
            const char * fn = IR_LIT(nd).sval;
            if (!fn || !fn[0]) continue;
            if (!strcmp(fn, "__rk_bool")) continue;
            if (rk_proc_known(fn)) nd->op = IR_CALL_PROC_STAGED;
            else if (rt_builtin_is_known(fn)) nd->op = IR_CALL_BUILTIN;
        }
    }
}
static void rk_register_proc(const tree_t * proc, const char * name, int nparams) { rk_register_proc_at(proc, name, nparams, nparams); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_register_proc_at(const tree_t * proc, const char * name, int nparams, int lastix) {
    int pi = stage2_proc_grow(&g_stage2);
    g_stage2.proc_table[pi].name = lp_strdup(name);
    g_stage2.proc_table[pi].proc = (tree_t *)(intptr_t) proc;
    g_stage2.proc_table[pi].entry_pc = -1;
    g_stage2.proc_table[pi].bb_idx = -1;
    g_stage2.proc_table[pi].nparams = nparams;
    g_stage2.proc_table[pi].byref_mask = rk_param_byref_mask(proc, nparams);
    {
        const tree_t * lp = (proc && lastix > 0 && lastix < proc->n) ? proc->c[lastix] : (const tree_t *)0;
        if (lp && lp->n > 0 && lp->c[0] && lp->c[0]->t == TT_QLIT && lp->c[0]->v.sval && !strcmp(lp->c[0]->v.sval, "*@")) {
            g_stage2.proc_table[pi].is_variadic = 1;
            g_stage2.proc_table[pi].rest_kind = 1;
        } else if (lp && lp->n > 0 && lp->c[0] && lp->c[0]->t == TT_QLIT && lp->c[0]->v.sval && !strcmp(lp->c[0]->v.sval, "**@")) {
            g_stage2.proc_table[pi].is_variadic = 1;
            g_stage2.proc_table[pi].rest_kind = 2;
        } else if (lp && lp->n > 0 && lp->c[0] && lp->c[0]->t == TT_QLIT && lp->c[0]->v.sval && !strcmp(lp->c[0]->v.sval, "+@")) {
            g_stage2.proc_table[pi].is_variadic = 1;
            g_stage2.proc_table[pi].rest_kind = 3;
        } else if (lp && lp->n > 0 && lp->c[0] && lp->c[0]->t == TT_QLIT && lp->c[0]->v.sval && !strcmp(lp->c[0]->v.sval, "+")) {
            g_stage2.proc_table[pi].is_variadic = 1;
            g_stage2.proc_table[pi].rest_kind = 4;
        } else if (lp && lp->n > 0 && lp->c[0] && lp->c[0]->t == TT_QLIT && lp->c[0]->v.sval && !strcmp(lp->c[0]->v.sval, "*%")) {
            g_stage2.proc_table[pi].named_rest = nparams;
        }
        for (int k = 1; proc && k < lastix && k < proc->n; k++) {
            const tree_t * sp = proc->c[k];
            const char * sq = (sp && sp->n > 0 && sp->c[0] && sp->c[0]->t == TT_QLIT) ? sp->c[0]->v.sval : NULL;
            int kd = !sq ? 0 : !strcmp(sq, "*@") ? 1 : !strcmp(sq, "**@") ? 2 : !strcmp(sq, "+@") ? 3 : !strcmp(sq, "+") ? 4 : 0;
            if (kd) { g_stage2.proc_table[pi].rest_kind = kd + 16 * (k + (lastix < nparams ? 1 : 0)); break; }
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_proc_rest_kind(const char * name) {
    for (int i = 0; name && i < g_stage2.proc_count; i++) if (g_stage2.proc_table[i].name && !strcmp(g_stage2.proc_table[i].name, name)) return g_stage2.proc_table[i].is_variadic ? 0 :
        g_stage2.proc_table[i].rest_kind;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_proc_known(const char * name) {
    if (!name) return 0;
    for (int i = 0; i < g_stage2.proc_count; i++) if (g_stage2.proc_table[i].name && !strcmp(g_stage2.proc_table[i].name, name)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_discover_lexical_regexes(const tree_t * t) {
    extern void rt_grammar_register(const char *qname, const char *body, int flavor);
    if (!t || t->t == TT_GRAMMAR_DECL) return;
    if (t->t == TT_REGEX_DECL) {
        const char * rname = (t->n > 0 && t->c[0] && t->c[0]->v.sval) ? t->c[0]->v.sval : NULL;
        const char * body = (t->n > 1 && t->c[1] && t->c[1]->v.sval) ? t->c[1]->v.sval : NULL;
        if (rname && body) { char qn[fmt_len("::%s", rname)]; snprintf(qn, sizeof qn, "::%s", rname); rt_grammar_register(qn, body, (int) t->v.ival); }
        return;
    }
    for (int i = 0; i < t->n; i++) rk_discover_lexical_regexes(t->c[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_discover_grammars(const tree_t * prog) {
    extern void rt_grammar_register(const char *qname, const char *body, int flavor);
    g_lower.rk.gram_names.len = 0;
    if (!prog) return;
    rk_discover_lexical_regexes(prog);
    for (int i = 0; i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) { const tree_t * sub = lrk_stmt_subj(d); if (!sub) continue; d = sub; }
        if (!d || d->t != TT_GRAMMAR_DECL) continue;
        const char * gname = (d->n > 0 && d->c[0] && d->c[0]->v.sval) ? d->c[0]->v.sval : NULL;
        if (!gname || !*gname) continue;
        if (!rk_is_grammar_name(gname)) CV_PUSH(g_lower.rk.gram_names, const char *) = gname;
        for (int j = 1; j < d->n; j++) {
            const tree_t * rd = d->c[j];
            if (!rd || rd->t != TT_REGEX_DECL) continue;
            const char * rname = (rd->n > 0 && rd->c[0] && rd->c[0]->v.sval) ? rd->c[0]->v.sval : NULL;
            const char * body = (rd->n > 1 && rd->c[1] && rd->c[1]->v.sval) ? rd->c[1]->v.sval : NULL;
            if (!rname || !body) continue;
            char qn[fmt_len("%s::%s", gname, rname)];
            snprintf(qn, sizeof qn, "%s::%s", gname, rname);
            rt_grammar_register(qn, body, (int) rd->v.ival);
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_register_classes(const tree_t * prog) {
    extern void record_register(const char *spec);
    if (!prog) return;
    g_lower.rk.class_names.len = 0;
    g_lower.rk.user_write_meth = 0;
    memset(g_lower.rk.listlike_overridden, 0, sizeof g_lower.rk.listlike_overridden);
    for (int i = 0; i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) { const tree_t * sub = lrk_stmt_subj(d); if (!sub) continue; d = sub; }
        if (!d || (d->t != TT_CLASS_DECL && d->t != TT_ROLE_DECL)) continue;
        const char * cname = (d->n > 0 && d->c[0] && d->c[0]->v.sval) ? d->c[0]->v.sval : NULL;
        if (!cname || !*cname) continue;
        if (rk_type_provides_real_method(d, "say") || rk_type_provides_real_method(d, "print")) g_lower.rk.user_write_meth = 1;
        for (int li = 0; RK_LISTLIKE_METHNAMES[li]; li++) if (rk_type_provides_real_method(d, RK_LISTLIKE_METHNAMES[li])) g_lower.rk.listlike_overridden[li] = 1;
        if (!rk_is_class_name(cname)) CV_PUSH(g_lower.rk.class_names, const char *) = cname;
        size_t specn = strlen(cname) + 3;
        for (int j = 1; j < d->n; j++) if (d->c[j] && d->c[j]->t != TT_SUB_DECL && d->c[j]->t != TT_REGEX_DECL) specn += 1 + (d->c[j]->v.sval ? strlen(d->c[j]->v.sval) : 0);
        char spec[specn];
        int pos = 0;
        pos += snprintf(spec + pos, sizeof(spec) - pos, "%s(", cname);
        int first_field = 1;
        for (int j = 1; j < d->n; j++) {
            const tree_t * ch = d->c[j];
            if (!ch || ch->t == TT_SUB_DECL || ch->t == TT_REGEX_DECL) continue;
            if (!first_field) { if (pos < (int)sizeof(spec) - 2) spec[pos++] = ','; }
            const char * fn = rk_fld_bare(ch->v.sval ? ch->v.sval : "");
            pos += snprintf(spec + pos, sizeof(spec) - pos, "%s", fn);
            first_field = 0;
        }
        if (pos < (int)sizeof(spec) - 1) spec[pos++] = ')';
        spec[pos] = '\0';
        record_register(spec);
        extern void dat_add_method(const char *type, const char *mname);
        for (int j = 1; j < d->n; j++) {
            const tree_t * ch = d->c[j];
            if (!ch || ch->t != TT_SUB_DECL || rk_method_is_stub(ch)) continue;
            const char * mname = (ch->n > 0 && ch->c[0] && ch->c[0]->v.sval) ? ch->c[0]->v.sval : NULL;
            if (!mname) continue;
            const char * dollar = strchr(mname, '$');
            if (dollar) {
                int bl = (int)(dollar - mname);
                char base[bl + 1];
                memcpy(base, mname, bl);
                base[bl] = '\0';
                dat_add_method(cname, base);
                extern void dat_mark_method_multi(const char *type, const char *mname);
                dat_mark_method_multi(cname, base);
            } else dat_add_method(cname, mname);
            if (!strcmp(mname, "BUILD")) {
                extern void dat_set_build_key(const char *cls, const char *key);
                int bs = (int)ch->v.ival;
                if (bs < 1) bs = 1;
                dat_set_build_key(cname, NULL);
                for (int p = 1; p < bs; p++) { const tree_t * pp = ch->c[p]; const char * pk = (pp && pp->v.sval) ? pp->v.sval : NULL; if (pk && *pk) dat_set_build_key(cname, pk); }
            }
        }
        extern void dat_set_field_default_i(const char *cls, const char *field, int64_t v);
        extern void dat_set_field_default_s(const char *cls, const char *field, const char *v);
        extern void dat_set_field_default_r(const char *cls, const char *field, double v);
        for (int j = 1; j < d->n; j++) {
            const tree_t * ch = d->c[j];
            if (!ch || ch->t != TT_HAS_DECL || ch->n < 1) continue;
            const char * fn = rk_fld_bare(ch->v.sval ? ch->v.sval : "");
            const tree_t * dv = ch->c[0];
            if (!dv) continue;
            if (dv->t == TT_ILIT) dat_set_field_default_i(cname, fn, dv->v.ival);
            else if (dv->t == TT_QLIT) dat_set_field_default_s(cname, fn, dv->v.sval);
            else if (dv->t == TT_FLIT && dv->n == 0) dat_set_field_default_r(cname, fn, dv->v.dval);
        }
        extern void dat_set_field_required(const char *cls, const char *field);
        for (int j = 1; j < d->n; j++) {
            const tree_t * ch = d->c[j];
            if (!ch || ch->t != TT_HAS_DECL || ch->n != 0) continue;
            const char * fn = rk_fld_bare(ch->v.sval ? ch->v.sval : "");
            if (*fn) dat_set_field_required(cname, fn);
        }
        extern void dat_set_field_rw(const char *cls, const char *field);
        for (int j = 1; j < d->n; j++) {
            const tree_t * ch = d->c[j];
            if (!ch || ch->t != TT_RW_DECL) continue;
            const char * fn = rk_fld_bare(ch->v.sval ? ch->v.sval : "");
            if (*fn) dat_set_field_rw(cname, fn);
        }
        extern void dat_add_handles(const char *cls, const char *meth, const char *field);
        for (int j = 1; j < d->n; j++) {
            const tree_t * ch = d->c[j];
            if (!ch || ch->t != TT_HANDLES_DECL) continue;
            const char * fn = rk_fld_bare(ch->v.sval ? ch->v.sval : "");
            if (!*fn) continue;
            const char * words = (ch->n > 0 && ch->c[0] && ch->c[0]->v.sval) ? ch->c[0]->v.sval : "";
            char wbuf[fmt_len("%s", words)];
            snprintf(wbuf, sizeof wbuf, "%s", words);
            char * sp = wbuf;
            char * tok;
            while ((tok = strtok(sp, " \t")) != (char *)0) { sp = (char *)0; if (*tok) dat_add_handles(cname, tok, fn); }
        }
        extern void dat_set_field_sigil(const char *cls, const char *field, int sig);
        for (int j = 1; j < d->n; j++) {
            const tree_t * ch = d->c[j];
            if (!ch || (ch->t != TT_ARR_DECL && ch->t != TT_HASH_DECL)) continue;
            const char * fn = rk_fld_bare(ch->v.sval ? ch->v.sval : "");
            if (*fn) dat_set_field_sigil(cname, fn, ch->t == TT_ARR_DECL ? '@' : '%');
        }
        extern void dat_set_field_priv(const char *cls, const char *field);
        for (int j = 1; j < d->n; j++) {
            const tree_t * ch = d->c[j];
            if (!ch || ch->t == TT_SUB_DECL || ch->t == TT_REGEX_DECL) continue;
            if (!rk_fld_priv(ch->v.sval)) continue;
            const char * fn = rk_fld_bare(ch->v.sval ? ch->v.sval : "");
            if (*fn) dat_set_field_priv(cname, fn);
        }
    }
    extern void class_inherit_multi(const char *child, const char **parents, int nparents);
    for (int i = 0; i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) { const tree_t * sub = lrk_stmt_subj(d); if (!sub) continue; d = sub; }
        if (!d || d->t != TT_CLASS_DECL) continue;
        const char * cname = (d->n > 0 && d->c[0] && d->c[0]->v.sval) ? d->c[0]->v.sval : NULL;
        const tree_t * tn = (d->n > 0) ? d->c[0] : NULL;
        if (cname && tn && tn->n > 0) {
            const char * pl[tn->n];
            int np = 0;
            const char * rls[tn->n];
            int nr = 0;
            for (int ti = 0; ti < tn->n; ti++) {
                const char * s = tn->c[ti] ? tn->c[ti]->v.sval : NULL;
                if (!s || !s[0]) continue;
                if (s[0] == 'i') pl[np++] = s + 1;
                else if (s[0] == 'd') rls[nr++] = s + 1;
            }
            if (np > 0) class_inherit_multi(cname, pl, np);
            extern void class_compose_role(const char *child, const char *role);
            for (int ri = 0; ri < nr; ri++) class_compose_role(cname, rls[ri]);
            extern void rt_script_die_surface(const char *msg);
            const tree_t * cdecl = rk_find_type_decl(prog, cname);
            for (int ri = 0; ri < nr; ri++) {
                const tree_t * rdecl = rk_find_type_decl(prog, rls[ri]);
                if (!rdecl) continue;
                for (int j = 1; j < rdecl->n; j++) {
                    const tree_t * ch = rdecl->c[j];
                    if (!ch || ch->t != TT_SUB_DECL || !rk_method_is_stub(ch)) continue;
                    const char * rm = (ch->n > 0 && ch->c[0] && ch->c[0]->v.sval) ? ch->c[0]->v.sval : NULL;
                    if (!rm) continue;
                    int sat = rk_type_provides_real_method(cdecl, rm);
                    for (int rj = 0; rj < nr && !sat; rj++) { const tree_t * od = rk_find_type_decl(prog, rls[rj]); if (rk_type_provides_real_method(od, rm)) sat = 1; }
                    if (!sat) {
                        char _m[fmt_len("Method '%s' must be implemented by class %s because it is required by role %s", rm, cname, rls[ri])];
                        snprintf(_m, sizeof _m, "Method '%s' must be implemented by class %s because it is required by role %s", rm, cname, rls[ri]);
                        rt_script_die_surface(_m);
                    }
                }
            }
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_discover_nested_subs(const tree_t * d) {
    if (!d || d->t != TT_SUB_DECL) return;
    int np = (int) d->v.ival;
    if (np < 0) np = 0;
    for (int k = np + 1; k < d->n; k++) {
        const tree_t * ch = d->c[k];
        if (ch && ch->t == TT_STMT) { const tree_t * sub = lrk_stmt_subj(ch); if (!sub) continue; ch = sub; }
        if (!ch || ch->t != TT_SUB_DECL) continue;
        const char * nm = (ch->n > 0 && ch->c[0] && ch->c[0]->v.sval) ? ch->c[0]->v.sval : NULL;
        if (!nm || !*nm) continue;
        if (!rk_proc_known(nm)) rk_register_proc(ch, nm, (int) ch->v.ival);
        rk_discover_nested_subs(ch);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_discover_block_subs(const tree_t * t) {
    if (!t) return;
    if (t->t == TT_CLASS_DECL || t->t == TT_ROLE_DECL || t->t == TT_GRAMMAR_DECL) return;
    if (t->t == TT_MODULE_DECL) {
        for (int j = 1; j < t->n; j++) {
            const tree_t * ch = t->c[j];
            if (ch && ch->t == TT_STMT) ch = lrk_stmt_subj(ch);
            if (!ch || ch->t != TT_SUB_DECL) continue;
            const char * nm = (ch->n > 0 && ch->c[0] && ch->c[0]->v.sval) ? ch->c[0]->v.sval : NULL;
            if (!nm || !*nm) continue;
            if (!rk_proc_known(nm)) rk_register_proc(ch, nm, (int) ch->v.ival);
            rk_discover_nested_subs(ch);
        }
        return;
    }
    if (t->t == TT_SUB_DECL) {
        const char * nm = (t->n > 0 && t->c[0] && t->c[0]->v.sval) ? t->c[0]->v.sval : NULL;
        if (nm && *nm && !rk_proc_known(nm)) { rk_register_proc(t, nm, (int) t->v.ival); rk_discover_nested_subs(t); }
        return;
    }
    for (int i = 0; i < t->n; i++) rk_discover_block_subs(t->c[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_discover_procs(const tree_t * prog) {
    if (!prog) return;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) { const tree_t * sub = lrk_stmt_subj(d); if (!sub) continue; d = sub; }
        if (!d) continue;
        if (d->t == TT_SUB_DECL) {
            const char * nm = (d->n > 0 && d->c[0] && d->c[0]->v.sval) ? d->c[0]->v.sval : NULL;
            if (!nm) continue;
            int np = (int) d->v.ival;
            rk_register_proc(d, nm, np);
        } else if (d->t == TT_CLASS_DECL || d->t == TT_ROLE_DECL) {
            const char * cname = (d->n > 0 && d->c[0] && d->c[0]->v.sval) ? d->c[0]->v.sval : NULL;
            if (!cname || !*cname) continue;
            for (int j = 1; j < d->n; j++) {
                const tree_t * ch = d->c[j];
                if (!ch || ch->t != TT_SUB_DECL || rk_method_is_stub(ch)) continue;
                const char * mname = (ch->n > 0 && ch->c[0] && ch->c[0]->v.sval) ? ch->c[0]->v.sval : NULL;
                if (!mname) continue;
                char qname[fmt_len("%s__%s", cname, mname)];
                snprintf(qname, sizeof qname, "%s__%s", cname, mname);
                int np = (int) ch->v.ival;
                rk_register_proc_at(ch, qname, np, np - 1);
            }
        } else if (d->t == TT_MODULE_DECL) {
            for (int j = 1; j < d->n; j++) {
                const tree_t * ch = d->c[j];
                if (ch && ch->t == TT_STMT) { const tree_t * sub = lrk_stmt_subj(ch); if (!sub) continue; ch = sub; }
                if (!ch || ch->t != TT_SUB_DECL) continue;
                const char * nm = (ch->n > 0 && ch->c[0] && ch->c[0]->v.sval) ? ch->c[0]->v.sval : NULL;
                if (!nm) continue;
                rk_register_proc(ch, nm, (int) ch->v.ival);
            }
        }
    }
    for (int i = 0; i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) { const tree_t * sub = lrk_stmt_subj(d); if (!sub) continue; d = sub; }
        if (d && d->t == TT_SUB_DECL) rk_discover_nested_subs(d);
        else if (d && d->t != TT_MODULE_DECL) rk_discover_block_subs(d);
        else if (d && d->t == TT_MODULE_DECL) {
            for (int j = 1; j < d->n; j++) {
                const tree_t * ch = d->c[j];
                if (ch && ch->t == TT_STMT) { const tree_t * sub = lrk_stmt_subj(ch); if (!sub) continue; ch = sub; }
                if (ch && ch->t == TT_SUB_DECL) rk_discover_nested_subs(ch);
            }
        }
    }
}
