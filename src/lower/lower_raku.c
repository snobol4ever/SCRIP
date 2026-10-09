#include <string.h>
#include "ct_arena.h"
#include "ct_vec.h"
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "lower.h"
typedef struct {
    int try_depth;
    IR_graph_t * g;
    IR_t * try_catch;
    IR_t * loop_exit;
    IR_t * loop_next;
    IR_t * proc_exit;
    const tree_t * cur_proc;
    uint64_t cur_byref_mask;
    int cur_nparams;
    const char * cur_proc_name;
} rcx_t;
static cv_t g_rk_gram_names;
static cv_t g_rk_class_names;
static cv_t g_rk_multi_names;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_seq_is_logical_and(const tree_t * t) { return t && t->t == TT_SEQ && t->n == 2 && t->v.ival == 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_is_multi_name(const char * nm) { if (!nm) return 0; for (uint32_t i = 0; i < g_rk_multi_names.len; i++) if (!strcmp(CV_AT(g_rk_multi_names, const char *, i), nm)) return 1; return 0; }
static void rk_multi_name_add(const char * base) { if (!base || rk_is_multi_name(base)) return; { const char * nm = ct_strdup(base); CV_PUSH(g_rk_multi_names, const char *) = nm; } }
static int rk_is_grammar_name(const char * nm) { if (!nm) return 0; for (uint32_t i = 0; i < g_rk_gram_names.len; i++) if (!strcmp(CV_AT(g_rk_gram_names, const char *, i), nm)) return 1; return 0; }
static int rk_is_class_name(const char * nm) { if (!nm) return 0; for (uint32_t i = 0; i < g_rk_class_names.len; i++) if (!strcmp(CV_AT(g_rk_class_names, const char *, i), nm)) return 1; return 0; }
static const char * rk_qualified_type_gist(const char * nm) {
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
static int rk_is_modeled_type(const char * ty) {
    if (!ty) return 0;
    static const char * k[] = { "Int", "Num", "Rat", "Str", "Numeric", "Real", "Cool", "Bool", 0 };
    for (int i = 0; k[i]; i++) if (!strcmp(ty, k[i])) return 1;
    return rk_is_class_name(ty);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static uint64_t rk_param_byref_mask(const tree_t * proc, int nparams) {
    uint64_t brm = 0;
    if (!proc) return 0;
    for (int k = 0; k < nparams && k < 64 && (1 + k) < proc->n; k++) {
        const tree_t * pv = proc->c[1 + k];
        if (pv && pv->n > 0 && pv->c[0] && pv->c[0]->t == TT_QLIT && pv->c[0]->v.sval && !strcmp(pv->c[0]->v.sval, "@")) brm |= (1ULL << k);
    }
    return brm;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_name_is_byref(rcx_t * cx, const char * name) {
    if (!cx || !cx->cur_proc || !cx->cur_byref_mask || !name) return 0;
    for (int k = 0; k < cx->cur_nparams && k < 64 && (1 + k) < cx->cur_proc->n; k++) {
        if (!((cx->cur_byref_mask >> k) & 1ULL)) continue;
        const tree_t * pv = cx->cur_proc->c[1 + k];
        if (pv && pv->v.sval && !strcmp(pv->v.sval, name)) return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define RK_VF_BOXED 8
static int rk_var_byref(rcx_t * cx, const tree_t * v) { return v && v->v.sval && ((v->slen & RK_VF_BOXED) || rk_name_is_byref(cx, v->v.sval)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static uint64_t rk_callee_byref_mask(const char * nm) {
    if (!nm) return 0;
    for (int i = 0; i < g_stage2.proc_count; i++) if (g_stage2.proc_table[i].name && !strcmp(g_stage2.proc_table[i].name, nm)) return g_stage2.proc_table[i].byref_mask;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * stmt_subj(const tree_t * s) { return lc_stmt_subj(s); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_method_is_stub(const tree_t * m) {
    if (!m || m->t != TT_SUB_DECL) return 0;
    int bs = (int) m->v.ival;
    if (bs < 1) bs = 1;
    if (m->n - bs != 1) return 0;
    const tree_t * b = m->c[bs];
    return b && b->t == TT_YADA;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * rk_fld_bare(const char * s) { return (s && (s[0] == '.' || s[0] == '!')) ? s + 1 : s; }
static int rk_fld_priv(const char * s) { return (s && s[0] == '!') ? 1 : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int g_rk_user_write_meth = 0;
static const char * RK_LISTLIKE_METHNAMES[] = { "keys", "values", "kv", "sort", "reverse", "grep", "map", "split", "words", "comb", NULL };
static int g_rk_listlike_overridden[10];
static int rk_listlike_idx(const char * nm) { if (!nm) return -1; for (int i = 0; RK_LISTLIKE_METHNAMES[i]; i++) if (!strcmp(RK_LISTLIKE_METHNAMES[i], nm)) return i; return -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * rk_find_type_decl(const tree_t * prog, const char * name) {
    if (!prog || !name) return NULL;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) { const tree_t * sub = stmt_subj(d); if (!sub) continue; d = sub; }
        if (!d || (d->t != TT_CLASS_DECL && d->t != TT_ROLE_DECL)) continue;
        const char * tn = (d->n > 0 && d->c[0] && d->c[0]->v.sval) ? d->c[0]->v.sval : NULL;
        if (tn && !strcmp(tn, name)) return d;
    }
    return NULL;
}
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void γ_to(IR_t * nd, IR_t * t) { if (t && ir_is_generator_kind(t->op)) lc_γ_to_β(nd, t); else lc_γ_to(nd, t); }
static void ω_to(IR_t * nd, IR_t * t) { if (t && ir_is_generator_kind(t->op)) lc_ω_to_β(nd, t); else lc_ω_to(nd, t); }
static tree_t * leaf_sval2(tree_e kind, const char * s) { tree_t * n = ast_node_new(kind); n->v.sval = (char *)s; return n; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * build(rcx_t * cx, IR_e op, IR_t * γ, IR_t * ω) {
    IR_t * nd = lc_build(cx->g, op, γ, ω);
    if (γ && ir_is_generator_kind(γ->op)) lc_γ_to_β(nd, γ);
    if (ω && ir_is_generator_kind(ω->op)) lc_ω_to_β(nd, ω);
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_is_str_subform(const char * nm) {
    static const char * const names[] = { "lc", "uc", "samecase", "tc", "tclc", "fc", "chomp", "chop", "flip", "wordcase", "trim-leading", "trim-trailing", "samemark", "substr", "substr-rw", "index",
        "rindex", "floor", "ceiling", "round", "truncate", "sign", "log2", "log10", "sinh", "cosh", "tanh", "asinh", "acosh", "atanh", "asin", "acos", "sec", "cosec", "cotan", "keys", "values", "kv",
        "pairs", "elems", "end", "ords", "atan2", "is-prime", NULL };
    for (int i = 0; names[i]; i++) if (!strcmp(nm, names[i])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_is_binop(tree_e tt) { switch (tt) { case TT_ADD: case TT_SUB: case TT_MUL: case TT_DIV: case TT_MOD: case TT_POW: case TT_CAT: case TT_XREP: return 1; default: return 0; } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int64_t rk_binop_code(tree_e tt) { switch (tt) { case TT_ADD: return BINOP_ADD_BIG; case TT_SUB: return BINOP_SUB_BIG; case TT_MUL: return BINOP_MUL_BIG; default: return lc_binop_code(tt); } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_io_lower_name(const char * nm) {
    static const char * const names[] = { "open", "close", "slurp", "spurt", "unlink", "mkdir", "rmdir", "copy", "rename", "move", "chmod", "dir", "chdir", "make-temp-file", "make-temp-dir", NULL };
    for (int i = 0; names[i]; i++) if (!strcmp(nm, names[i])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_user_proc_exists(const char * nm) {
    for (int i = 0; i < g_stage2.proc_count; i++) { const char * pn = g_stage2.proc_table[i].name; if (pn && (!strcmp(pn, nm) || (pn[0] == '&' && !strcmp(pn + 1, nm)))) return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_predeclared(const char * nm, int bare) {
    if (!nm || !*nm) return 0;
    static const char * const dyn[] = { "*DISTRO", "*KERNEL", "*VM", "*RAKU", "*PERL", "*PID", "*PROGRAM", "*PROGRAM-NAME", "*CWD", "*EXECUTABLE", "*EXECUTABLE-NAME", "*HOME", "*TMPDIR", "*USER",
        "%*ENV", "@*ARGS", "?FILE", "/", "!", "Order::Less", "Order::Same", "Order::More", "Bool::True", "Bool::False", NULL };
    for (int i = 0; dyn[i]; i++) if (!strcmp(nm, dyn[i])) return 1;
    if (!bare) return 0;
    static const char * const terms[] = { "now", "time", "rand", "Empty", "Less", "Same", "More", NULL };
    for (int i = 0; terms[i]; i++) if (!strcmp(nm, terms[i]) && !rk_is_class_name(nm) && !rk_user_proc_exists(nm)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_to_array_call(const tree_t * rhs) {
    tree_t * f = ast_node_new(TT_FNC);
    f->v.sval = (char *) "__rk_to_array";
    ast_push(f, leaf_sval2(TT_VAR, "__rk_to_array"));
    ast_push(f, (tree_t *) rhs);
    return f;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_to_hash_call(const tree_t * rhs) {
    tree_t * f = ast_node_new(TT_FNC);
    f->v.sval = (char *) "__rk_to_hash";
    ast_push(f, leaf_sval2(TT_VAR, "__rk_to_hash"));
    ast_push(f, (tree_t *) rhs);
    return f;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * rk_assign_rhs(const char * vn, const tree_t * rhs) {
    if (!vn || !rhs) return rhs;
    if (vn[0] == '@') return rk_to_array_call(rhs);
    if (vn[0] == '%') { if (rhs->t == TT_FNC && rhs->n > 0 && rhs->c[0] && rhs->c[0]->v.sval && !strcmp(rhs->c[0]->v.sval, "__rk_hash")) return rhs; return rk_to_hash_call(rhs); }
    return rhs;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_yields_list(const tree_t * t) {
    if (!t) return 0;
    if (t->t == TT_FNC && t->n > 0 && t->c[0] && t->c[0]->v.sval) {
        const char * f = t->c[0]->v.sval;
        return !strcmp(f, "__rk_arr_slice") || !strcmp(f, "__rk_arr_pick") || !strcmp(f, "__rk_hyper_meth");
    }
    if (t->t == TT_METHCALL && t->n > 1 && t->c[1] && t->c[1]->v.sval) { int li = rk_listlike_idx(t->c[1]->v.sval); return li >= 0 && !g_rk_listlike_overridden[li]; }
    if (t->t == TT_SORT || t->t == TT_REVERSE) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_yields_array(const tree_t * t) {
    if (!t) return 0;
    if (t->t == TT_VAR) return (t->slen & 2) != 0;
    return t->t == TT_FNC && t->n > 1 && t->c[0] && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, "__rk_hyper_meth") && rk_yields_array(t->c[1]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_is_relop(tree_e tt) {
    switch (tt) { case TT_LT: case TT_LE: case TT_GT: case TT_GE: case TT_EQ: case TT_NE: case TT_LEQ: case TT_LNE: case TT_LLT: case TT_LLE: case TT_LGT: case TT_LGE: return 1; default: return 0; }
}
static IR_t * lower_rv(rcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
static int rk_proc_known(const char * name);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * rk_excise(rcx_t * cx, IR_t * γ, IR_t * ω, IR_t ** res) { IR_t * nd = build(cx, IR_EXCISED, γ, ω); if (res) *res = nd; return nd; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int trace_wanted(void) { extern long g_trace_budget; return g_trace_budget != 0; }
static int rk_use_is_version(const tree_t * s) { return s && s->t == TT_USE_DECL && s->v.sval && s->v.sval[0] == 'v' && s->v.sval[1] >= '0' && s->v.sval[1] <= '9'; }
static long trace_stmt_line(const tree_t * s) { return (s && !rk_use_is_version(s)) ? s->line : 0; }
static IR_t * trace_stmt_wrap(rcx_t * cx, long line, IR_t * stmt_entry, IR_t * ω) {
    if (line <= 0 || !trace_wanted()) return stmt_entry;
    IR_t * call = build(cx, IR_CALL, stmt_entry, ω);
    IR_LIT(call).sval = "__trace_stmt";
    IR_t * lit = build(cx, IR_LIT_INTEGER, call, ω);
    IR_LIT(lit).ival = line;
    ir_operand_push(call, lit);
    return lit;
}
static int rk_trace_is_block(const char * name) { return name && !strncmp(name, "__blk_", 6); }
static const char * rk_trace_name(const char * name) { return (name && !strcmp(name, "&main")) ? "main" : name; }
static IR_t * trace_call_wrap(rcx_t * cx, const char * name, IR_t * body_entry, IR_t * ω) {
    if (!name || !*name || !trace_wanted() || rk_trace_is_block(name)) return body_entry;
    name = rk_trace_name(name);
    IR_t * call = build(cx, IR_CALL, body_entry, ω);
    IR_LIT(call).sval = "__trace_call";
    IR_t * nm = build(cx, IR_LIT_STRING, call, ω);
    IR_LIT(nm).sval = name;
    ir_operand_push(call, nm);
    return nm;
}
static IR_t * trace_value_prep(rcx_t * cx, const char * name, IR_t * γ, IR_t * ω, IR_t ** call_out) {
    if (!name || !*name || !trace_wanted() || !strncmp(name, "__", 2) || !strncmp(name, "$?", 2)) { *call_out = NULL; return NULL; }
    IR_t * call = build(cx, IR_CALL, γ, ω);
    IR_LIT(call).sval = "__trace_value";
    IR_t * nm = build(cx, IR_LIT_STRING, call, ω);
    IR_LIT(nm).sval = name;
    ir_operand_push(call, nm);
    *call_out = call;
    return nm;
}
static const tree_t * rk_catch_of(const tree_t * t) {
    for (int i = 0; i < t->n; i++) { const tree_t * s = t->c[i]; if (s && s->t == TT_STMT) { const tree_t * sub = stmt_subj(s); if (sub) s = sub; } if (s && s->t == TT_CATCH && s->n > 0) return s; }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * rk_catch_handler(const tree_t * c) {
    const tree_t * h = c->c[0];
    if (!h || (h->t != TT_SEQ && h->t != TT_SEQ_EXPR)) return h;
    int ci = -1;
    for (int i = h->n - 1; i >= 0; i--) { const tree_t * s = h->c[i]; if (s && s->t == TT_STMT) { const tree_t * sub = stmt_subj(s); if (sub) s = sub; } if (s && s->t == TT_CASE) { ci = i; break; } }
    tree_t * rt = ast_node_new(TT_FNC);
    rt->v.sval = (char *) "__rk_rethrow";
    ast_push(rt, leaf_sval2(TT_VAR, "__rk_rethrow"));
    tree_t * nh = ast_node_new(h->t);
    for (int i = 0; i < h->n; i++) {
        const tree_t * s = h->c[i];
        const tree_t * u = s;
        if (u && u->t == TT_STMT) { const tree_t * sub = stmt_subj(u); if (sub) u = sub; }
        if (i != ci) { ast_push(nh, (tree_t *) s); continue; }
        int hasdef = 0;
        for (int k = 1; k < u->n; k += 2) if (u->c[k] && u->c[k]->t == TT_NUL) hasdef = 1;
        if (hasdef) { ast_push(nh, (tree_t *) s); continue; }
        tree_t * nc = ast_node_new(TT_CASE);
        nc->line = u->line;
        for (int k = 0; k < u->n; k++) ast_push(nc, u->c[k]);
        ast_push(nc, ast_node_new(TT_NUL));
        tree_t * rb = ast_node_new(TT_SEQ_EXPR);
        ast_push(rb, rt);
        ast_push(nc, rb);
        ast_push(nh, nc);
    }
    if (ci < 0) ast_push(nh, rt);
    return nh;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_rblock(rcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** last_res) {
    if (t && (t->t == TT_SEQ || t->t == TT_SEQ_EXPR || t->t == TT_PROGRAM) && t->slen != 78) {
        const tree_t * ct = rk_catch_of(t);
        if (ct) {
            tree_t * nb = ast_node_new(t->t);
            nb->line = t->line;
            nb->slen = 78;
            for (int i = 0; i < t->n; i++) ast_push(nb, t->c[i]);
            tree_t * nt = ast_node_new(TT_TRY);
            nt->line = t->line;
            nt->slen = 77;
            ast_push(nt, nb);
            ast_push(nt, (tree_t *) rk_catch_handler(ct));
            IR_t * r = NULL;
            IR_t * e = lower_rv(cx, nt, γ, ω, &r);
            if (last_res) *last_res = r;
            return e;
        }
    }
    if (!t) return (γ && ir_is_generator_kind(γ->op)) ? γ : build(cx, IR_SUCCEED, γ, ω);
    if (t->t != TT_SEQ && t->t != TT_PROGRAM && t->t != TT_SEQ_EXPR) { IR_t * r = NULL; IR_t * e = lower_rv(cx, t, γ, ω, &r); if (last_res) *last_res = r; return e; }
    if (t->n == 0) return (γ && ir_is_generator_kind(γ->op)) ? γ : build(cx, IR_SUCCEED, γ, ω);
    IR_t * succ = γ;
    IR_t * entry = γ;
    int got_last = 0;
    for (int i = t->n - 1; i >= 0; i--) {
        const tree_t * s = t->c[i];
        if (s && s->t == TT_STMT) { const tree_t * sub = stmt_subj(s); if (!sub) continue; s = sub; }
        long line = trace_stmt_line(s);
        if (s && s->t == TT_CATCH) continue;
        if (s && (s->t == TT_SEQ || s->t == TT_SEQ_EXPR || s->t == TT_PROGRAM) && s->n == 0) continue;
        IR_t * gs = succ, * gw = ω;
        if (cx->try_catch) {
            IR_t * pγ = build(cx, IR_CALL, cx->try_catch, succ);
            IR_LIT(pγ).sval = "exc_check";
            IR_t * pω = build(cx, IR_CALL, cx->try_catch, ω);
            IR_LIT(pω).sval = "exc_check";
            gs = pγ;
            gw = pω;
        }
        IR_t * r = NULL;
        IR_t * e = lower_rv(cx, s, gs, gw, &r);
        if (last_res && !got_last) { *last_res = r; got_last = 1; }
        if (e && ir_is_generator_kind(e->op)) { IR_t * tramp = build(cx, IR_GOTO, NULL, NULL); lc_γ_to(tramp, e); lc_ω_to(tramp, e); e = tramp; }
        if (e) e = trace_stmt_wrap(cx, line, e, gw);
        if (e) { entry = e; succ = e; }
    }
    return entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_rcall(rcx_t * cx, const tree_t * t, const char * nm, int from, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * nd = build(cx, IR_CALL, γ, ω);
    IR_LIT(nd).sval = nm;
    int nargs = t->n - from;
    IR_t * prev = NULL;
    IR_t * entry = nd;
    uint64_t brm = rk_callee_byref_mask(nm);
    if (!strcmp(nm, "__blk_close")) brm = ~1ULL;
    if (!strcmp(nm, "__rk_byref_assign")) brm = 1ULL;
    for (int k = 0; k < nargs; k++) {
        const tree_t * argt = t->c[from + k];
        IR_t * ar = NULL;
        IR_t * ae;
        if (((brm >> k) & 1ULL) && argt && argt->t == TT_VAR && argt->v.sval && !rk_var_byref(cx, argt)) {
            IR_t * vr = build(cx, IR_VAR_REF, (k == nargs - 1) ? nd : NULL, ω);
            IR_LIT(vr).sval = argt->v.sval;
            ae = vr;
            ar = vr;
        } else if (((brm >> k) & 1ULL) && argt && argt->t == TT_VAR && argt->v.sval) {
            IR_t * vr = build(cx, IR_VAR, (k == nargs - 1) ? nd : NULL, ω);
            IR_LIT(vr).sval = argt->v.sval;
            ae = vr;
            ar = vr;
        } else if (argt && argt->t == TT_FNC && argt->n == 2 && argt->v.sval && !strcmp(argt->v.sval, "__rk_byval")) {
            ae = lower_rv(cx, argt->c[1], (k == nargs - 1) ? nd : NULL, ω, &ar);
        } else {
            ae = lower_rv(cx, argt, (k == nargs - 1) ? nd : NULL, ω, &ar);
        }
        if (k == 0) entry = ae;
        if (prev && ae) lc_γ_to(prev, ae);
        prev = ar;
        if (ar) ir_operand_push(nd, ar);
    }
    if (res) *res = nd;
    return entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_meth_is_bool(const char * m) {
    return m &&
        (!strcmp(m, "defined") || !strcmp(m, "Bool") || !strcmp(m, "so") || !strcmp(m, "not") || !strcmp(m, "does") || !strcmp(m, "isa") || !strcmp(m, "starts-with") || !strcmp(m, "ends-with") ||
        !strcmp(m, "contains"));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_rcall_bool(rcx_t * cx, const tree_t * t, const char * nm, int from, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * bl = build(cx, IR_CALL, γ, ω);
    IR_LIT(bl).sval = "__rk_mkbool";
    IR_t * inner = NULL;
    IR_t * e = lower_rcall(cx, t, nm, from, bl, ω, &inner);
    if (inner) ir_operand_push(bl, inner);
    if (res) *res = bl;
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_rcall1(rcx_t * cx, const tree_t * recv, const char * nm, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * nd = build(cx, IR_CALL, γ, ω);
    IR_LIT(nd).sval = nm;
    IR_t * ar = NULL;
    IR_t * ae = lower_rv(cx, recv, nd, ω, &ar);
    if (ar) ir_operand_push(nd, ar);
    if (res) *res = nd;
    return ae;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * rk_lower_str_operand(rcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    if (!t || t->t == TT_QLIT || t->t == TT_ILIT || t->t == TT_CAT) return lower_rv(cx, t, γ, ω, res);
    return lower_rcall1(cx, t, "__rk_str", γ, ω, res);
}
static IR_t * lower_rcall_skip1(rcx_t * cx, const tree_t * t, const char * nm, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * nd = build(cx, IR_CALL, γ, ω);
    IR_LIT(nd).sval = nm;
    int total = t->n - 1;
    IR_t * prev = NULL;
    IR_t * entry = nd;
    int k = 0;
    for (int ci = 0; ci < t->n; ci++) {
        if (ci == 1) continue;
        IR_t * ar = NULL;
        IR_t * ae = lower_rv(cx, t->c[ci], (k == total - 1) ? nd : NULL, ω, &ar);
        if (k == 0) entry = ae;
        if (prev && ae) lc_γ_to(prev, ae);
        prev = ar;
        if (ar) ir_operand_push(nd, ar);
        k++;
    }
    if (res) *res = nd;
    return entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_divis_desugar(const tree_t * t) {
    tree_t * md = ast_node_new(TT_MOD);
    ast_push(md, (tree_t *) t->c[0]);
    ast_push(md, (tree_t *) t->c[1]);
    tree_t * z = ast_node_new(TT_ILIT);
    z->v.ival = 0;
    tree_t * eq = ast_node_new(TT_EQ);
    ast_push(eq, md);
    ast_push(eq, z);
    return eq;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_case_match(const tree_t * subj, const tree_t * cond) {
    if (cond->t == TT_FNC && cond->n > 1 && cond->c[0] && cond->c[0]->v.sval && !strcmp(cond->c[0]->v.sval, "__rk_regex")) {
        tree_t * m = ast_node_new(TT_SMATCH);
        ast_push(m, (tree_t *) subj);
        ast_push(m, cond->c[1]);
        ast_push(m, leaf_sval2(TT_QLIT, "match"));
        return m;
    }
    if (cond->t == TT_TO && cond->n > 1) {
        tree_t * ge = ast_node_new(TT_GE);
        ast_push(ge, (tree_t *) subj);
        ast_push(ge, (tree_t *) cond->c[0]);
        tree_t * le = ast_node_new(TT_LE);
        ast_push(le, (tree_t *) subj);
        ast_push(le, (tree_t *) cond->c[1]);
        tree_t * an = ast_node_new(TT_SEQ);
        ast_push(an, ge);
        ast_push(an, le);
        return an;
    }
    if (cond->t == TT_VAR) {
        tree_t * mc = ast_node_new(TT_FNC);
        mc->v.sval = (char *) intern("__rk_when_match");
        tree_t * nmv = ast_node_new(TT_VAR);
        nmv->v.sval = (char *) intern("__rk_when_match");
        ast_push(mc, nmv);
        ast_push(mc, (tree_t *) subj);
        ast_push(mc, (tree_t *) cond);
        return mc;
    }
    tree_t * eq = ast_node_new(cond->t == TT_QLIT ? TT_LEQ : TT_EQ);
    ast_push(eq, (tree_t *) subj);
    ast_push(eq, (tree_t *) cond);
    return eq;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_case_desugar(const tree_t * t) {
    static int cid = 0;
    char nb[48];
    snprintf(nb, sizeof nb, "__case_%d", cid++);
    const char * vn = intern(nb);
    tree_t * chain = NULL;
    for (int i = t->n - 2; i >= 1; i -= 2) {
        const tree_t * cond = t->c[i];
        const tree_t * blk = t->c[i + 1];
        if (cond && cond->t == TT_NUL) { chain = (tree_t *) blk; continue; }
        tree_t * gate = ast_node_new(TT_IF);
        ast_push(gate, rk_case_match(leaf_sval2(TT_VAR, vn), cond));
        ast_push(gate, (tree_t *) blk);
        if (chain) ast_push(gate, chain);
        chain = gate;
    }
    tree_t * seq = ast_node_new(TT_SEQ);
    tree_t * as = ast_node_new(TT_ASSIGN);
    ast_push(as, leaf_sval2(TT_VAR, vn));
    ast_push(as, (tree_t *) t->c[0]);
    ast_push(seq, as);
    if (chain) ast_push(seq, chain);
    return seq;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_cond(rcx_t * cx, const tree_t * c, IR_t * on_true, IR_t * on_false) {
    if (c && c->t == TT_STMT) { const tree_t * sub = stmt_subj(c); if (sub) c = sub; }
    if (!c) return on_true;
    if (c->t == TT_DIVIS && c->n > 1) return lower_cond(cx, rk_divis_desugar(c), on_true, on_false);
    if (rk_is_relop(c->t) && c->n > 1) {
        IR_t * op = build(cx, IR_BINOP_TEST, on_true, on_false);
        IR_LIT(op).ival = lc_binop_code(c->t);
        IR_t * lr = NULL, * rr = NULL;
        IR_t * ea = lower_rv(cx, c->c[0], NULL, on_false, &lr);
        IR_t * eb = lower_rv(cx, c->c[1], op, on_false, &rr);
        γ_to(lr, eb);
        ir_operand_push(op, lr);
        ir_operand_push(op, rr);
        return ea;
    }
    if (c->t == TT_NOT && c->n > 0) return lower_cond(cx, c->c[0], on_false, on_true);
    if (c->t == TT_ALT && c->n > 1) { IR_t * rhs = lower_cond(cx, c->c[1], on_true, on_false); return lower_cond(cx, c->c[0], on_true, rhs); }
    if (c->t == TT_SEQ && c->n > 1) { IR_t * rhs = lower_cond(cx, c->c[1], on_true, on_false); return lower_cond(cx, c->c[0], rhs, on_false); }
    if (c->t == TT_SMATCH && c->n > 2 && c->c[2] && c->c[2]->v.sval && !strcmp(c->c[2]->v.sval, "match")) {
        IR_t * nd = build(cx, IR_CALL, on_true, on_false);
        IR_LIT(nd).sval = "re_test";
        IR_t * sr = NULL, * pr = NULL;
        IR_t * es = lower_rv(cx, c->c[0], NULL, on_false, &sr);
        IR_t * ep = lower_rv(cx, c->c[1], nd, on_false, &pr);
        γ_to(sr, ep);
        ir_operand_push(nd, sr);
        ir_operand_push(nd, pr);
        return es;
    }
    IR_t * bk = build(cx, IR_CALL, on_true, on_false);
    IR_LIT(bk).sval = "__rk_bool";
    IR_t * r = NULL;
    IR_t * e = lower_rv(cx, c, bk, on_false, &r);
    if (r) ir_operand_push(bk, r);
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * rk_lower_logical(rcx_t * cx, const tree_t * t, int is_or, IR_t * γ, IR_t * ω, IR_t ** res) {
    char tn[40];
    snprintf(tn, sizeof tn, "$?logic%d", cx->g->n);
    const char * tname = lp_strdup(tn);
    IR_t * jv = build(cx, IR_VAR, γ, ω);
    IR_LIT(jv).sval = tname;
    IR_t * ab = build(cx, IR_ASSIGN, jv, ω);
    IR_LIT(ab).sval = tname;
    IR_t * rb = NULL;
    IR_t * eb = lower_rv(cx, t->c[1], ab, ω, &rb);
    if (rb) ir_operand_push(ab, rb);
    IR_t * ce = is_or ? lower_cond(cx, leaf_sval2(TT_VAR, tname), jv, eb) : lower_cond(cx, leaf_sval2(TT_VAR, tname), eb, jv);
    IR_t * aa = build(cx, IR_ASSIGN, ce, ω);
    IR_LIT(aa).sval = tname;
    IR_t * ra = NULL;
    IR_t * ea = lower_rv(cx, t->c[0], aa, ω, &ra);
    if (ra) ir_operand_push(aa, ra);
    *res = jv;
    return ea;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_field_set(rcx_t * cx, const tree_t * lhs, const tree_t * rhs, IR_t * γ, IR_t * ω, IR_t ** res) {
    const char * fname = (lhs->t == TT_TWIGIL_FIELD) ? lhs->v.sval : ((lhs->n > 1 && lhs->c[1]) ? lhs->c[1]->v.sval : lhs->v.sval);
    IR_t * nd = build(cx, IR_CALL, γ, ω);
    IR_LIT(nd).sval = (lhs->t == TT_TWIGIL_FIELD) ? "field_set" : "field_set_pub";
    IR_t * nl = build(cx, IR_LIT_STRING, NULL, ω);
    IR_LIT(nl).sval = fname;
    IR_t * or_ = NULL;
    IR_t * entry;
    if (lhs->t == TT_TWIGIL_FIELD) { IR_t * sv = build(cx, IR_VAR, nl, ω); IR_LIT(sv).sval = "self"; or_ = sv; entry = sv; } else { entry = lower_rv(cx, lhs->c[0], nl, ω, &or_); }
    IR_t * vr = NULL;
    IR_t * ev = lower_rv(cx, rhs, nd, ω, &vr);
    γ_to(nl, ev);
    if (or_) ir_operand_push(nd, or_);
    ir_operand_push(nd, nl);
    if (vr) ir_operand_push(nd, vr);
    if (res) *res = nd;
    return entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_take_list(const tree_t * g, const tree_t ** out, int max) {
    const tree_t * gb = (g && g->n > 0) ? g->c[0] : NULL;
    int n = 0;
    if (gb && gb->t == TT_SUSPEND) { if (gb->n < 1 || !gb->c[0]) return -1; out[n++] = gb->c[0]; return n; }
    if (!gb || gb->t != TT_SEQ_EXPR) return -1;
    for (int i = 0; i < gb->n; i++) {
        const tree_t * s = gb->c[i];
        if (s && s->t == TT_STMT) { const tree_t * sub = stmt_subj(s); if (sub) s = sub; }
        if (!s || s->t != TT_SUSPEND || s->n < 1 || !s->c[0] || n >= max) return -1;
        out[n++] = s->c[0];
    }
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * rk_xf_body(rcx_t * cx, const tree_t * xf, int xfk, const char * vname, IR_t * valprod, const tree_t * body, IR_t * pump, IR_t * ω) {
    IR_t * bentry = lower_rblock(cx, body, pump, ω, NULL);
    IR_t * av = build(cx, IR_ASSIGN, bentry, ω);
    IR_LIT(av).sval = vname;
    if (xfk == 1) {
        IR_t * fr = NULL;
        IR_t * fe = lower_rv(cx, xf, av, ω, &fr);
        if (fr) ir_operand_push(av, fr);
        IR_t * au = build(cx, IR_ASSIGN, fe, ω);
        IR_LIT(au).sval = "_";
        ir_operand_push(au, valprod);
        return au;
    }
    if (xfk == 2) {
        IR_t * vr = build(cx, IR_VAR, av, ω);
        IR_LIT(vr).sval = "_";
        ir_operand_push(av, vr);
        IR_t * centry = lower_cond(cx, xf, vr, pump);
        IR_t * au = build(cx, IR_ASSIGN, centry, ω);
        IR_LIT(au).sval = "_";
        ir_operand_push(au, valprod);
        return au;
    }
    ir_operand_push(av, valprod);
    return av;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_proc_nparams(const char * name) {
    for (int i = 0; name && i < g_stage2.proc_count; i++) if (g_stage2.proc_table[i].name && !strcmp(g_stage2.proc_table[i].name, name))
        return (g_stage2.proc_table[i].is_variadic || g_stage2.proc_table[i].named_rest) ? -1 : g_stage2.proc_table[i].nparams;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_user_meth_named(const char * m) {
    size_t L = strlen(m);
    for (int i = 0; i < g_stage2.proc_count; i++) {
        const char * n = g_stage2.proc_table[i].name;
        size_t nl = n ? strlen(n) : 0;
        if (nl > L + 2 && strncmp(n, "__", 2) && !strcmp(n + nl - L, m) && n[nl - L - 1] == '_' && n[nl - L - 2] == '_') return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_fnc2(const char * nm, const tree_t * a, const tree_t * b) {
    tree_t * f = ast_node_new(TT_FNC);
    f->v.sval = (char *) nm;
    ast_push(f, leaf_sval2(TT_VAR, nm));
    if (a) ast_push(f, (tree_t *) a);
    if (b) ast_push(f, (tree_t *) b);
    return f;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * rk_lower_iter_meth(rcx_t * cx, const tree_t * t, const char * mname, IR_t * γ, IR_t * ω, IR_t ** res) {
    int k = !strcmp(mname, "map") ? 1 : !strcmp(mname, "grep") ? 2 : !strcmp(mname, "first") ? 3 : !strcmp(mname, "reduce") ? 4 : !strcmp(mname, "sort") ? 5 : 0;
    const tree_t * fa = t->n == 3 ? t->c[2] : NULL;
    const char * bn = NULL;
    if (fa && fa->t == TT_ANON_BLOCK && fa->v.sval) bn = fa->v.sval;
    else if (fa && fa->t == TT_VAR && fa->v.sval && fa->v.sval[0] == '&' && rk_proc_known(fa->v.sval + 1) && !rk_is_multi_name(fa->v.sval + 1)) bn = fa->v.sval + 1;
    if (!k || !bn || !t->c[0]) return NULL;
    int np = rk_proc_nparams(bn), step = (k == 1 && np > 1) ? np : 1;
    if (k == 4 ? np != 2 : k == 5 ? np != 1 : (np < 0 || (k != 1 && np > 1))) return NULL;
    if (rk_user_meth_named(mname) || (t->c[0]->t == TT_VAR && t->c[0]->v.sval && t->c[0]->v.sval[0] == '%')) return NULL;
    char b1[40], b2[40], b3[40], b4[40];
    int id = cx->g->n;
    snprintf(b1, sizeof b1, "$?isrc%d", id);
    snprintf(b2, sizeof b2, "$?ires%d", id);
    snprintf(b3, sizeof b3, "$?iidx%d", id);
    snprintf(b4, sizeof b4, "$?iel%d", id);
    const char * src = lp_strdup(b1), * acc = lp_strdup(b2), * idx = lp_strdup(b3), * el = lp_strdup(b4);
    IR_t * fin, * fr = NULL;
    if (k <= 2) fin = lower_rcall(cx, rk_fnc2("__rk_iter_done", leaf_sval2(TT_VAR, acc), NULL), "__rk_iter_done", 1, γ, ω, &fr);
    else if (k == 5) fin = lower_rcall(cx, rk_fnc2("__rk_sort_by_keys", leaf_sval2(TT_VAR, src), leaf_sval2(TT_VAR, acc)), "__rk_sort_by_keys", 1, γ, ω, &fr);
    else { fin = build(cx, IR_VAR, γ, ω); IR_LIT(fin).sval = acc; fr = fin; }
    IR_t * va = build(cx, IR_ASSIGN, NULL, ω);
    IR_LIT(va).sval = idx;
    IR_t * to = build(cx, IR_TO, va, fin);
    IR_LIT(to).sval = "ag";
    tree_t * lo = ast_node_new(TT_ILIT);
    lo->v.ival = k == 4 ? 1 : 0;
    tree_t * one = ast_node_new(TT_ILIT);
    one->v.ival = 1;
    tree_t * nel = rk_fnc2("elems", leaf_sval2(TT_VAR, src), NULL);
    if (step > 1) { tree_t * sn = ast_node_new(TT_ILIT); sn->v.ival = step; nel = rk_fnc2("__rk_intdiv", nel, sn); }
    tree_t * hi = ast_node_new(TT_SUB);
    ast_push(hi, nel);
    ast_push(hi, one);
    IR_t * rlo = NULL, * rhi = NULL;
    IR_t * elo = lower_rv(cx, lo, NULL, ω, &rlo);
    IR_t * ehi = lower_rv(cx, hi, to, ω, &rhi);
    γ_to(rlo, ehi);
    ir_operand_push(to, rlo);
    ir_operand_push(to, rhi);
    if (rhi && ir_is_generator_kind(to->op)) lc_γ_to(rhi, to);
    ir_operand_push(va, to);
    tree_t * at = rk_fnc2("__rk_arr_at", leaf_sval2(TT_VAR, src), leaf_sval2(TT_VAR, idx));
    IR_t * bentry, * r = NULL;
    if (k == 1 || k == 4 || k == 5) {
        IR_t * as = build(cx, IR_ASSIGN, to, to);
        IR_LIT(as).sval = acc;
        tree_t * call = k == 4 ? rk_fnc2(bn, leaf_sval2(TT_VAR, acc), at) : rk_fnc2(bn, np ? at : NULL, NULL);
        if (step > 1) {
            call = rk_fnc2(bn, NULL, NULL);
            for (int j = 0; j < step; j++) {
                tree_t * sn = ast_node_new(TT_ILIT);
                sn->v.ival = step;
                tree_t * jn = ast_node_new(TT_ILIT);
                jn->v.ival = j;
                tree_t * mu = ast_node_new(TT_MUL);
                ast_push(mu, leaf_sval2(TT_VAR, idx));
                ast_push(mu, sn);
                tree_t * ad = ast_node_new(TT_ADD);
                ast_push(ad, mu);
                ast_push(ad, jn);
                ast_push(call, rk_fnc2("__rk_arr_at", leaf_sval2(TT_VAR, src), ad));
            }
        }
        if (k == 1 || k == 5) call = rk_fnc2(k == 1 ? "__rk_map_append" : "__rk_grep_append", leaf_sval2(TT_VAR, acc), call);
        bentry = lower_rcall(cx, call, call->v.sval, 1, as, to, &r);
        if (r) ir_operand_push(as, r);
    } else {
        IR_t * hit;
        if (k == 2) {
            IR_t * as = build(cx, IR_ASSIGN, to, to);
            IR_LIT(as).sval = acc;
            hit = lower_rcall(cx, rk_fnc2("__rk_grep_append", leaf_sval2(TT_VAR, acc), leaf_sval2(TT_VAR, el)), "__rk_grep_append", 1, as, to, &r);
            if (r) ir_operand_push(as, r);
        } else {
            IR_t * as = build(cx, IR_ASSIGN, fin, to);
            IR_LIT(as).sval = acc;
            hit = build(cx, IR_VAR, as, to);
            IR_LIT(hit).sval = el;
            ir_operand_push(as, hit);
        }
        IR_t * ce = lower_cond(cx, rk_fnc2(bn, np ? leaf_sval2(TT_VAR, el) : NULL, NULL), hit, to);
        IR_t * ael = build(cx, IR_ASSIGN, ce, to);
        IR_LIT(ael).sval = el;
        bentry = lower_rcall(cx, at, "__rk_arr_at", 1, ael, to, &r);
        if (r) ir_operand_push(ael, r);
    }
    γ_to(va, bentry);
    IR_t * ares = build(cx, IR_ASSIGN, elo, ω);
    IR_LIT(ares).sval = acc;
    tree_t * init = (k <= 2 || k == 5) ? rk_fnc2("__rk_iter_src", NULL, NULL) : k == 3 ? rk_fnc2("__rk_undef", NULL, NULL) : rk_fnc2("__rk_arr_at", leaf_sval2(TT_VAR, src), ast_node_new(TT_ILIT));
    IR_t * eini = lower_rcall(cx, init, init->v.sval, 1, ares, ω, &r);
    if (r) ir_operand_push(ares, r);
    IR_t * asrc = build(cx, IR_ASSIGN, eini, ω);
    IR_LIT(asrc).sval = src;
    IR_t * esi = lower_rcall(cx, rk_fnc2("__rk_iter_src", t->c[0], NULL), "__rk_iter_src", 1, asrc, ω, &r);
    if (r) ir_operand_push(asrc, r);
    *res = fr;
    return esi;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * rk_tern_branch(rcx_t * cx, const tree_t * br, IR_t * at, IR_t * ω) {
    extern int is_global(const char *);
    IR_t * r = NULL;
    const tree_t * v = (br && br->t == TT_ASSIGN && br->n >= 2) ? br->c[0] : NULL;
    if (v && v->t == TT_VAR && v->v.sval && is_global(v->v.sval)) { IR_t * gv = build(cx, IR_VAR, at, ω); IR_LIT(gv).sval = v->v.sval; ir_operand_push(at, gv); return lower_rv(cx, br, gv, ω, &r); }
    IR_t * e = lower_rv(cx, br, at, ω, &r);
    if (r) ir_operand_push(at, r);
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_rv(rcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * dummy = NULL;
    if (!res) res = &dummy;
    if (!t) { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    if (t->t == TT_DIVIS && t->n > 1) { return lower_rv(cx, rk_divis_desugar(t), γ, ω, res); }
    if (rk_is_relop(t->t)) {
        if (t->n < 2) return rk_excise(cx, γ, ω, res);
        IR_t * op = build(cx, IR_BINOP_RELOP_VAL, γ, ω);
        IR_LIT(op).ival = lc_binop_code(t->t);
        IR_t * lr = NULL, * rr = NULL;
        IR_t * ea = lower_rv(cx, t->c[0], NULL, ω, &lr);
        IR_t * eb = lower_rv(cx, t->c[1], op, ω, &rr);
        γ_to(lr, eb);
        ir_operand_push(op, lr);
        ir_operand_push(op, rr);
        *res = op;
        return ea;
    }
    if (t->t == TT_DIV && t->n > 1) { return lower_rcall(cx, t, "__rk_div", 0, γ, ω, res); }
    if (t->t == TT_MOD && t->n > 1) { return lower_rcall(cx, t, "__rk_mod", 0, γ, ω, res); }
    if (rk_is_binop(t->t)) {
        IR_t * op = build(cx, IR_BINOP, γ, ω);
        IR_LIT(op).ival = rk_binop_code(t->t);
        IR_t * lr = NULL, * rr = NULL;
        IR_t * ea, * eb;
        if (t->t == TT_CAT) {
            ea = rk_lower_str_operand(cx, t->c[0], NULL, ω, &lr);
            eb = rk_lower_str_operand(cx, t->c[1], op, ω, &rr);
        } else {
            ea = lower_rv(cx, t->c[0], NULL, ω, &lr);
            eb = lower_rv(cx, t->c[1], op, ω, &rr);
        }
        γ_to(lr, eb);
        ir_operand_push(op, lr);
        ir_operand_push(op, rr);
        *res = op;
        return ea;
    }
    switch (t->t) {
        case TT_ILIT:
        { IR_t * nd = build(cx, IR_LIT_INTEGER, γ, ω); IR_LIT(nd).ival = t->v.ival; *res = nd; return nd; }
        case TT_FLIT:
        { IR_t * nd = build(cx, IR_LIT_REAL, γ, ω); IR_LIT(nd).dval = t->v.dval; *res = nd; return nd; }
        case TT_QLIT:
        { IR_t * nd = build(cx, IR_LIT_STRING, γ, ω); IR_LIT(nd).sval = t->v.sval; *res = nd; return nd; }
        case TT_NUL:
        { IR_t * nd = build(cx, IR_CALL, γ, ω); IR_LIT(nd).sval = "__rk_undef"; *res = nd; return nd; }
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
            if (x && x->t == TT_ILIT && x->v.ival != INT64_MIN) { IR_t * nd = build(cx, IR_LIT_INTEGER, γ, ω); IR_LIT(nd).ival = -x->v.ival; *res = nd; return nd; }
            if (x && x->t == TT_FLIT) { IR_t * nd = build(cx, IR_LIT_REAL, γ, ω); IR_LIT(nd).dval = -x->v.dval; *res = nd; return nd; }
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
            if (rk_is_grammar_name(t->v.sval) || rk_is_class_name(t->v.sval) || (t->v.sval && t->v.sval[0] == 'X' && t->v.sval[1] == ':' && t->v.sval[2] == ':')) {
                IR_t * nd = build(cx, IR_LIT_STRING, γ, ω);
                IR_LIT(nd).sval = t->v.sval;
                *res = nd;
                return nd;
            }
            if (t->v.sval && strchr(t->v.sval, ':') && !rk_predeclared(t->v.sval, 0)) {
                IR_t * nd = build(cx, IR_LIT_STRING, γ, ω);
                IR_LIT(nd).sval = rk_qualified_type_gist(t->v.sval);
                *res = nd;
                return nd;
            }
            if ((t->slen & 1) && t->v.sval && !strcmp(t->v.sval, "Nil") && !rk_is_class_name("Nil")) { IR_t * nd = build(cx, IR_CALL, γ, ω); IR_LIT(nd).sval = "__rk_undef"; *res = nd; return nd; }
            if ((t->slen & 1) && t->v.sval && !strcmp(t->v.sval, "pi") && !rk_is_class_name("pi")) {
                IR_t * nd = build(cx, IR_LIT_REAL, γ, ω);
                IR_LIT(nd).dval = 3.141592653589793;
                *res = nd;
                return nd;
            }
            if ((t->slen & 1) && t->v.sval && !strcmp(t->v.sval, "e") && !rk_is_class_name("e")) {
                IR_t * nd = build(cx, IR_LIT_REAL, γ, ω);
                IR_LIT(nd).dval = 2.718281828459045;
                *res = nd;
                return nd;
            }
            if ((t->slen & 1) && t->v.sval && !strcmp(t->v.sval, "tau") && !rk_is_class_name("tau")) {
                IR_t * nd = build(cx, IR_LIT_REAL, γ, ω);
                IR_LIT(nd).dval = 6.283185307179586;
                *res = nd;
                return nd;
            }
            if ((t->slen & 1) && t->v.sval && !strcmp(t->v.sval, "Inf") && !rk_is_class_name("Inf")) {
                IR_t * nd = build(cx, IR_LIT_REAL, γ, ω);
                IR_LIT(nd).dval = (double) INFINITY;
                *res = nd;
                return nd;
            }
            if ((t->slen & 1) && t->v.sval && !strcmp(t->v.sval, "NaN") && !rk_is_class_name("NaN")) {
                IR_t * nd = build(cx, IR_LIT_REAL, γ, ω);
                IR_LIT(nd).dval = (double) NAN;
                *res = nd;
                return nd;
            }
            if ((t->slen & 1) && t->v.sval && (!strcmp(t->v.sval, "\xcf\x80") || !strcmp(t->v.sval, "\xcf\x84") || !strcmp(t->v.sval, "\xe2\x88\x9e"))) {
                IR_t * nd = build(cx, IR_LIT_REAL, γ, ω);
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
            if ((t->slen & 1) && t->v.sval && !strcmp(t->v.sval, "i") && !rk_is_class_name("i")) { IR_t * nd = build(cx, IR_CALL, γ, ω); IR_LIT(nd).sval = "__rk_mkcplx_i"; *res = nd; return nd; }
            if (rk_var_byref(cx, t)) { IR_t * dr = build(cx, IR_DEREF, γ, ω); IR_t * v = build(cx, IR_VAR, dr, ω); IR_LIT(v).sval = t->v.sval; ir_operand_push(dr, v); *res = dr; return v; }
            IR_t * nd = build(cx, IR_VAR, γ, ω);
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
                IR_t * asA = build(cx, IR_ASSIGN, γ, ω);
                IR_LIT(asA).sval = rhs->c[1]->v.sval;
                IR_t * r2 = NULL;
                IR_t * einit = lower_rcall(cx, rhs, "arr_init", 1, asA, ω, &r2);
                if (r2) ir_operand_push(asA, r2);
                IR_t * asP = build(cx, IR_ASSIGN, einit, ω);
                IR_LIT(asP).sval = t->c[0]->v.sval;
                IR_t * r3 = NULL;
                IR_t * elast = lower_rcall(cx, rhs, "arr_last", 1, asP, ω, &r3);
                if (r3) ir_operand_push(asP, r3);
                *res = asA;
                return elast;
            }
            if (rhs && rk_is_relop(rhs->t)) {
                IR_t * nd = build(cx, IR_ASSIGN, γ, ω);
                IR_LIT(nd).sval = t->c[0]->v.sval;
                IR_t * op = build(cx, IR_BINOP_RELOP_VAL, nd, ω);
                IR_LIT(op).ival = lc_binop_code(rhs->t);
                IR_t * lr = NULL, * rr = NULL;
                IR_t * ea = lower_rv(cx, rhs->c[0], NULL, ω, &lr);
                IR_t * eb = lower_rv(cx, rhs->c[1], op, ω, &rr);
                γ_to(lr, eb);
                ir_operand_push(op, lr);
                ir_operand_push(op, rr);
                ir_operand_push(nd, op);
                *res = nd;
                return ea;
            }
            int decl_only = t->c[1] && t->c[1]->t == TT_NUL && t->c[1]->v.ival == 1;
            IR_t * vtrace_call = NULL;
            IR_t * vtrace = (decl_only || t->v.ival == 1) ? NULL : trace_value_prep(cx, t->c[0]->v.sval, γ, ω, &vtrace_call);
            IR_t * nd = build(cx, IR_ASSIGN, vtrace ? vtrace : γ, ω);
            IR_LIT(nd).sval = t->c[0]->v.sval;
            IR_t * rr = NULL;
            IR_t * e = lower_rv(cx, decl_only ? t->c[1] : rk_assign_rhs(t->c[0]->v.sval, t->c[1]), nd, ω, &rr);
            if (rr) ir_operand_push(nd, rr);
            if (rr && vtrace_call) ir_operand_push(vtrace_call, rr);
            *res = nd;
            return e;
        }
        { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
        case TT_DECL:
        if (t->n > 1 && t->c[1] && t->c[1]->t == TT_VAR) {
            int loop_bind = t->c[0] && t->c[0]->t == TT_VAR && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, "__decl");
            IR_t * vtrace_call = NULL;
            IR_t * vtrace = (t->n > 2 && t->c[2] && !loop_bind) ? trace_value_prep(cx, t->c[1]->v.sval, γ, ω, &vtrace_call) : NULL;
            IR_t * nd = build(cx, IR_ASSIGN, vtrace ? vtrace : γ, ω);
            IR_LIT(nd).sval = t->c[1]->v.sval;
            IR_t * rr = NULL;
            IR_t * e;
            if (t->n > 2 && t->c[2]) {
                e = lower_rv(cx, rk_assign_rhs(t->c[1]->v.sval, t->c[2]), nd, ω, &rr);
            } else {
                IR_t * u = build(cx, IR_CALL, nd, ω);
                IR_LIT(u).sval = (t->c[1]->v.sval && t->c[1]->v.sval[0] == '%') ? "__rk_hash" : "__rk_undef";
                rr = u;
                e = u;
            }
            if (rr) ir_operand_push(nd, rr);
            if (rr && vtrace_call) ir_operand_push(vtrace_call, rr);
            *res = nd;
            return e;
        }
        { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
        case TT_ARR_SET:
        if (t->n > 2 && t->c[0] && t->c[0]->t == TT_VAR) {
            const char * vn = t->c[0]->v.sval;
            if (rk_var_byref(cx, t->c[0])) {
                IR_t * asn = build(cx, IR_ASSIGN_VAR, γ, ω);
                IR_t * vr = build(cx, IR_VAR, NULL, ω);
                IR_LIT(vr).sval = vn;
                IR_t * r2 = NULL;
                IR_t * e = lower_rcall(cx, t, "__rk_arr_set", 0, asn, ω, &r2);
                γ_to(vr, e ? e : asn);
                ir_operand_push(asn, vr);
                if (r2) ir_operand_push(asn, r2);
                *res = asn;
                return vr;
            }
            IR_t * as = build(cx, IR_ASSIGN, γ, ω);
            IR_LIT(as).sval = vn;
            IR_t * r2 = NULL;
            IR_t * e = lower_rcall(cx, t, "__rk_arr_set", 0, as, ω, &r2);
            if (r2) ir_operand_push(as, r2);
            *res = as;
            return e;
        }
        { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
        case TT_USE_DECL:
        { IR_t * nd = build(cx, IR_SUCCEED, γ, ω); *res = nd; return nd; }
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
        if (t->n == 1 && rk_yields_array(t->c[0])) return lower_rcall(cx, t, "rk_write_arr", 0, γ, ω, res);
        if (t->n == 1 && rk_yields_list(t->c[0])) return lower_rcall(cx, t, "rk_write_list", 0, γ, ω, res);
        return lower_rcall(cx, t, "rk_write", 0, γ, ω, res);
        case TT_PRINT:
        case TT_PRINT_FH:
        return lower_rcall(cx, t, "rk_writes", 0, γ, ω, res);
        case TT_DIE:
        return lower_rcall(cx, t, "die", 0, γ, ω, res);
        case TT_TRY:
        if (t->slen != 77 && t->n > 0 && t->c[0] && (t->c[0]->t == TT_SEQ || t->c[0]->t == TT_SEQ_EXPR || t->c[0]->t == TT_PROGRAM)) {
            const tree_t * ob = t->c[0];
            int li = -1;
            for (int i = ob->n - 1; i >= 0; i--) {
                const tree_t * s = ob->c[i];
                if (s && s->t == TT_STMT) { const tree_t * sub = stmt_subj(s); if (sub) s = sub; }
                if (s && s->t == TT_CATCH) continue;
                li = i;
                break;
            }
            const tree_t * ls = li >= 0 ? ob->c[li] : NULL;
            if (ls && ls->t == TT_STMT) { const tree_t * sub = stmt_subj(ls); if (sub) ls = sub; }
            if (ls && ls->t != TT_IF && ls->t != TT_UNLESS && ls->t != TT_WHILE && ls->t != TT_UNTIL && ls->t != TT_REPEAT && ls->t != TT_FOR && ls->t != TT_FOR_RANGE && ls->t != TT_EVERY &&
                ls->t != TT_CASE && ls->t != TT_RETURN && ls->t != TT_NUL && ls->t != TT_INVOKE && ls->t != TT_SEQ && ls->t != TT_SEQ_EXPR && ls->t != TT_ASSIGN && ls->t != TT_REVASSIGN &&
                ls->t != TT_ARR_SET && ls->t != TT_HASH_SET && ls->t != TT_DECL && ls->t != TT_ARR_DECL && ls->t != TT_HASH_DECL && ls->t != TT_RW_DECL && ls->t != TT_STATIC_DECL &&
                ls->t != TT_SUB_DECL && ls->t != TT_CLASS_DECL) {
                char tvb[48];
                snprintf(tvb, sizeof tvb, "__tryv%p", (const void *) t);
                const char * tv = ct_strdup(tvb);
                tree_t * nb = ast_node_new(ob->t);
                for (int i = 0; i < ob->n; i++) {
                    if (i == li) { tree_t * as = ast_node_new(TT_ASSIGN); ast_push(as, leaf_sval2(TT_VAR, tv)); ast_push(as, (tree_t *) ls); ast_push(nb, as); } else ast_push(nb, ob->c[i]);
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
                    if (s && s->t == TT_STMT) { const tree_t * sub = stmt_subj(s); if (sub) s = sub; }
                    if (s && s->t == TT_CATCH && s->n > 0) { handler = s->c[0]; break; }
                }
                IR_t * kexit = build(cx, IR_CALL, γ, ω);
                IR_LIT(kexit).sval = "try_exit";
                IR_t * save = cx->try_catch;
                cx->try_catch = NULL;
                int save_td = cx->try_depth;
                IR_t * centry;
                if (handler) {
                    IR_t * hentry = lower_rblock(cx, handler, γ, ω, NULL);
                    IR_t * clr = build(cx, IR_CALL, hentry, ω);
                    IR_LIT(clr).sval = "exc_clear";
                    IR_t * asg = build(cx, IR_ASSIGN, clr, ω);
                    IR_LIT(asg).sval = "_";
                    IR_t * eg = build(cx, IR_CALL, asg, ω);
                    IR_LIT(eg).sval = "exc_get";
                    ir_operand_push(asg, eg);
                    IR_t * tx = build(cx, IR_CALL, eg, ω);
                    IR_LIT(tx).sval = "try_exit";
                    centry = tx;
                } else {
                    IR_t * clr = build(cx, IR_CALL, γ, ω);
                    IR_LIT(clr).sval = "exc_clear";
                    IR_t * tx = build(cx, IR_CALL, clr, ω);
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
                IR_t * te = build(cx, IR_CALL, bentry, ω);
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
            if (nm && t->n > 1 && rk_is_str_subform(nm) && !rk_proc_known(nm)) {
                tree_t * mc = ast_node_new(TT_METHCALL);
                mc->line = t->line;
                ast_push(mc, t->c[1]);
                ast_push(mc, leaf_sval2(TT_QLIT, nm));
                for (int i = 2; i < t->n; i++) ast_push(mc, t->c[i]);
                return lower_rv(cx, mc, γ, ω, res);
            }
            if (nm && t->n > 2 && (!strcmp(nm, "map") || !strcmp(nm, "grep") || !strcmp(nm, "first")) && !rk_proc_known(nm) && t->c[1] &&
                (t->c[1]->t == TT_ANON_BLOCK || (t->c[1]->t == TT_VAR && t->c[1]->v.sval && t->c[1]->v.sval[0] == '&'))) {
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
            if (nm && !strcmp(nm, "push") && t->n > 1 && t->c[1] && t->c[1]->t == TT_VAR) {
                IR_t * as = build(cx, IR_ASSIGN, γ, ω);
                IR_LIT(as).sval = t->c[1]->v.sval;
                IR_t * r2 = NULL;
                IR_t * e = lower_rcall(cx, t, "push_pure", 1, as, ω, &r2);
                if (r2) ir_operand_push(as, r2);
                *res = as;
                return e;
            }
            if (nm && !strcmp(nm, "hash_set") && t->n > 2 && t->c[1] && (t->c[1]->t == TT_VAR || t->c[1]->t == TT_TWIGIL_FIELD)) {
                const char * vn = t->c[1]->v.sval;
                IR_t * as = build(cx, IR_ASSIGN, γ, ω);
                IR_LIT(as).sval = vn;
                IR_t * r2 = NULL;
                IR_t * e = lower_rcall(cx, t, "hash_set_pure", 1, as, ω, &r2);
                if (r2) ir_operand_push(as, r2);
                *res = as;
                return e;
            }
            return lower_rcall(cx, t, nm, 1, γ, ω, res);
        }
        case TT_STMT:
        { const tree_t * sub = stmt_subj(t); return sub ? lower_rv(cx, sub, γ, ω, res) : (build(cx, IR_SUCCEED, γ, ω)); }
        case TT_IF:
        case TT_UNLESS:
        {
            IR_t * tentry = (t->n > 1 && t->c[1]) ? lower_rblock(cx, t->c[1], γ, ω, NULL) : γ;
            IR_t * eentry = (t->n > 2 && t->c[2]) ? lower_rblock(cx, t->c[2], γ, ω, NULL) : γ;
            IR_t * e = (t->t == TT_UNLESS) ? lower_cond(cx, t->c[0], eentry, tentry) : lower_cond(cx, t->c[0], tentry, eentry);
            *res = e;
            return e;
        }
        case TT_WHILE:
        {
            IR_t * LOOP = build(cx, IR_GOTO, NULL, ω);
            IR_t * sv_exit = cx->loop_exit;
            IR_t * sv_next = cx->loop_next;
            cx->loop_exit = γ;
            cx->loop_next = LOOP;
            IR_t * bentry = (t->n > 1) ? lower_rblock(cx, t->c[1], LOOP, ω, NULL) : LOOP;
            cx->loop_exit = sv_exit;
            cx->loop_next = sv_next;
            IR_t * centry = lower_cond(cx, t->c[0], bentry, γ);
            bb_src_note(centry, "rk_while_cond", 0);
            γ_to(LOOP, centry);
            ω_to(LOOP, centry);
            *res = LOOP;
            return centry;
        }
        case TT_CLOOP:
        {
            IR_t * LOOP = build(cx, IR_GOTO, NULL, ω);
            IR_t * incr_entry = lower_rblock(cx, t->c[2], LOOP, ω, NULL);
            IR_t * sv_exit = cx->loop_exit;
            IR_t * sv_next = cx->loop_next;
            cx->loop_exit = γ;
            cx->loop_next = incr_entry;
            IR_t * bentry = lower_rblock(cx, t->c[3], incr_entry, ω, NULL);
            cx->loop_exit = sv_exit;
            cx->loop_next = sv_next;
            IR_t * centry = lower_cond(cx, t->c[1], bentry, γ);
            bb_src_note(centry, "rk_cloop_cond", 0);
            if (incr_entry && incr_entry != LOOP) bb_src_note(incr_entry, "rk_cloop_incr", 0);
            γ_to(LOOP, centry);
            ω_to(LOOP, centry);
            IR_t * ientry = lower_rblock(cx, t->c[0], LOOP, ω, NULL);
            *res = LOOP;
            return ientry;
        }
        case TT_UNTIL:
        {
            IR_t * LOOP = build(cx, IR_GOTO, NULL, ω);
            IR_t * sv_exit = cx->loop_exit;
            IR_t * sv_next = cx->loop_next;
            cx->loop_exit = γ;
            cx->loop_next = LOOP;
            IR_t * bentry = (t->n > 1) ? lower_rblock(cx, t->c[1], LOOP, ω, NULL) : LOOP;
            cx->loop_exit = sv_exit;
            cx->loop_next = sv_next;
            IR_t * centry = lower_cond(cx, t->c[0], γ, bentry);
            bb_src_note(centry, "rk_until_cond", 0);
            γ_to(LOOP, centry);
            ω_to(LOOP, centry);
            *res = LOOP;
            return centry;
        }
        case TT_REPEAT:
        if (t->v.ival != 0 && t->n > 1) {
            IR_t * BACK = build(cx, IR_GOTO, NULL, ω);
            IR_t * centry = (t->v.ival == 1) ? lower_cond(cx, t->c[1], BACK, γ) : lower_cond(cx, t->c[1], γ, BACK);
            IR_t * sv_exit = cx->loop_exit;
            IR_t * sv_next = cx->loop_next;
            cx->loop_exit = γ;
            cx->loop_next = centry;
            IR_t * bentry = lower_rblock(cx, t->c[0], centry, ω, NULL);
            cx->loop_exit = sv_exit;
            cx->loop_next = sv_next;
            bb_src_note(bentry, "rk_repeat_body", 0);
            bb_src_note(centry, "rk_repeat_cond", 0);
            γ_to(BACK, bentry);
            ω_to(BACK, bentry);
            *res = BACK;
            return bentry;
        } else {
            IR_t * LOOP = build(cx, IR_GOTO, NULL, ω);
            IR_t * sv_exit = cx->loop_exit;
            IR_t * sv_next = cx->loop_next;
            cx->loop_exit = ω;
            cx->loop_next = LOOP;
            IR_t * bentry = (t->n > 0) ? lower_rblock(cx, t->c[0], LOOP, ω, NULL) : LOOP;
            cx->loop_exit = sv_exit;
            cx->loop_next = sv_next;
            if (bentry && bentry != LOOP) bb_src_note(bentry, "rk_loop_body", 0);
            γ_to(LOOP, bentry);
            ω_to(LOOP, bentry);
            *res = LOOP;
            return bentry;
        }
        case TT_LOOP_BREAK:
        case TT_LOOP_NEXT:
        { IR_t * tgt = (t->t == TT_LOOP_BREAK) ? cx->loop_exit : cx->loop_next; if (!tgt) tgt = ω; IR_t * j = build(cx, IR_GOTO, tgt, tgt); *res = NULL; return j; }
        case TT_EVERY:
        if (t->n > 1 && t->c[0] && t->c[0]->t == TT_ITERATE && t->c[0]->n > 0) {
            const char * vname = t->c[0]->v.sval ? t->c[0]->v.sval : "_";
            const tree_t * src = t->c[0]->c[0];
            const tree_t * body = t->c[1];
            const tree_t * xf = NULL;
            int xfk = 0;
            if (src && (src->t == TT_MAP || src->t == TT_GREP) && src->n > 1) { xfk = (src->t == TT_MAP) ? 1 : 2; xf = src->c[0]; src = src->c[1]; }
            if (src && src->t == TT_TO && src->n > 1) {
                IR_t * to = build(cx, IR_TO, NULL, γ);
                IR_LIT(to).sval = "ag";
                IR_t * rlo = NULL, * rhi = NULL;
                IR_t * elo = lower_rv(cx, src->c[0], NULL, ω, &rlo);
                IR_t * ehi = lower_rv(cx, src->c[1], to, ω, &rhi);
                γ_to(rlo, ehi);
                ir_operand_push(to, rlo);
                ir_operand_push(to, rhi);
                if (rhi && ir_is_generator_kind(to->op)) lc_γ_to(rhi, to);
                IR_t * inner = rk_xf_body(cx, xf, xfk, vname, to, body, to, ω);
                γ_to(to, inner);
                *res = to;
                return elo;
            }
            if (src && src->t == TT_GATHER) {
                const tree_t * takes[(src->n > 0 && src->c[0]) ? src->c[0]->n + 1 : 1];
                int ntk = rk_take_list(src, takes, (int) (sizeof takes / sizeof takes[0]));
                if (ntk > 0) {
                    IR_t * succ = γ;
                    for (int k = ntk - 1; k >= 0; k--) {
                        IR_t * r = NULL;
                        IR_t * ek = lower_rv(cx, takes[k], NULL, ω, &r);
                        IR_t * inner = rk_xf_body(cx, xf, xfk, vname, r, body, succ, ω);
                        γ_to(r, inner);
                        succ = ek;
                    }
                    *res = succ;
                    return succ;
                }
            }
            if (src && xfk == 0) {
                static int g_forlist_ctr = 0;
                int fid = ++g_forlist_ctr;
                char iname[48];
                snprintf(iname, sizeof iname, "__foridx_%d", fid);
                const char * in = intern(iname);
                const char * ln;
                int need_matr = 0;
                if (src->t == TT_VAR && src->v.sval) { ln = src->v.sval; } else { char lname[48]; snprintf(lname, sizeof lname, "__forlist_%d", fid); ln = intern(lname); need_matr = 1; }
                tree_t * elc = ast_node_new(TT_FNC);
                elc->v.sval = (char *)"elems";
                ast_push(elc, leaf_sval2(TT_VAR, "elems"));
                ast_push(elc, leaf_sval2(TT_VAR, ln));
                tree_t * one = ast_node_new(TT_ILIT);
                one->v.ival = 1;
                tree_t * hi = ast_node_new(TT_SUB);
                ast_push(hi, elc);
                ast_push(hi, one);
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
                    IR_t * as = build(cx, IR_ASSIGN, NULL, ω);
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
        case TT_SMATCH:
        if (t->n > 2 && t->c[2] && t->c[2]->v.sval && strcmp(t->c[2]->v.sval, "subst")) {
            IR_t * nd = build(cx, IR_CALL, γ, ω);
            IR_LIT(nd).sval = strcmp(t->c[2]->v.sval, "match_global") ? "re_match" : "re_match_global";
            IR_t * sr = NULL, * pr = NULL;
            IR_t * es = lower_rv(cx, t->c[0], NULL, ω, &sr);
            IR_t * ep = lower_rv(cx, t->c[1], nd, ω, &pr);
            γ_to(sr, ep);
            ir_operand_push(nd, sr);
            ir_operand_push(nd, pr);
            *res = nd;
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
            IR_t * va = build(cx, IR_ASSIGN, NULL, ω);
            IR_LIT(va).sval = t->c[0]->v.sval;
            IR_t * to = build(cx, IR_TO, va, γ);
            IR_LIT(to).sval = "ag";
            IR_t * rlo = NULL, * rhi = NULL;
            IR_t * elo = lower_rv(cx, t->c[1], NULL, ω, &rlo);
            IR_t * ehi = lower_rv(cx, t->c[2], to, ω, &rhi);
            γ_to(rlo, ehi);
            ir_operand_push(to, rlo);
            ir_operand_push(to, rhi);
            if (rhi && ir_is_generator_kind(to->op)) lc_γ_to(rhi, to);
            ir_operand_push(va, to);
            IR_t * bentry = lower_rblock(cx, t->c[3], to, ω, NULL);
            γ_to(va, bentry);
            *res = to;
            return elo;
        }
        return rk_excise(cx, γ, ω, res);
        case TT_ARR_GET:
        return lower_rcall(cx, t, "__rk_arr_at", 0, γ, ω, res);
        case TT_HASH_GET:
        return lower_rcall(cx, t, "hash_get", 0, γ, ω, res);
        case TT_HASH_SET:
        if (t->n > 2 && t->c[0] && (t->c[0]->t == TT_VAR || t->c[0]->t == TT_TWIGIL_FIELD)) {
            const char * vn = t->c[0]->t == TT_TWIGIL_FIELD ? t->c[0]->v.sval : (t->c[0]->n > 0 && t->c[0]->c[0] ? t->c[0]->c[0]->v.sval : t->c[0]->v.sval);
            IR_t * as = build(cx, IR_ASSIGN, γ, ω);
            IR_LIT(as).sval = vn;
            IR_t * r2 = NULL;
            IR_t * e = lower_rcall(cx, t, "hash_set_pure", 0, as, ω, &r2);
            if (r2) ir_operand_push(as, r2);
            *res = as;
            return e;
        }
        { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
        case TT_HASH_EXISTS:
        return lower_rcall_bool(cx, t, "hash_exists", 0, γ, ω, res);
        case TT_HASH_DELETE:
        return lower_rcall(cx, t, "hash_delete", 0, γ, ω, res);
        case TT_TO:
        if (t->n > 1) {
            IR_t * to = build(cx, IR_TO, γ, ω);
            IR_LIT(to).sval = "ag";
            IR_t * rlo = NULL, * rhi = NULL;
            IR_t * elo = lower_rv(cx, t->c[0], NULL, ω, &rlo);
            IR_t * ehi = lower_rv(cx, t->c[1], to, ω, &rhi);
            γ_to(rlo, ehi);
            ir_operand_push(to, rlo);
            ir_operand_push(to, rhi);
            if (rhi && ir_is_generator_kind(to->op)) lc_γ_to(rhi, to);
            *res = to;
            return elo;
        }
        { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
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
        { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
        case TT_SEQ:
        if (rk_seq_is_logical_and(t)) return rk_lower_logical(cx, t, 0, γ, ω, res);
        { IR_t * lr = NULL; IR_t * b = lower_rblock(cx, t, γ, ω, &lr); *res = lr ? lr : b; return b; }
        case TT_PROGRAM:
        case TT_SEQ_EXPR:
        { IR_t * lr = NULL; IR_t * b = lower_rblock(cx, t, γ, ω, &lr); *res = lr ? lr : b; return b; }
        case TT_METHCALL:
        {
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
                    IR_t * as = build(cx, IR_ASSIGN, γ, ω);
                    IR_LIT(as).sval = t->c[0]->v.sval;
                    IR_t * r2 = NULL;
                    IR_t * e = lower_rcall_skip1(cx, t, fn, as, ω, &r2);
                    if (r2) ir_operand_push(as, r2);
                    *res = as;
                    return e;
                }
                if (!strcmp(mname, "pop") || !strcmp(mname, "shift")) {
                    const char * mut_fn = !strcmp(mname, "pop") ? "arr_init" : "arr_tail";
                    const char * val_fn = !strcmp(mname, "pop") ? "arr_last" : "__rk_arr_first";
                    IR_t * asA = build(cx, IR_ASSIGN, γ, ω);
                    IR_LIT(asA).sval = t->c[0]->v.sval;
                    IR_t * rmut = NULL;
                    IR_t * emut = lower_rcall1(cx, t->c[0], mut_fn, asA, ω, &rmut);
                    if (rmut) ir_operand_push(asA, rmut);
                    IR_t * rval = NULL;
                    IR_t * eval_ = lower_rcall1(cx, t->c[0], val_fn, emut, ω, &rval);
                    *res = rval;
                    return eval_;
                }
            }
            if (mname && t->n == 2 && t->c[0] && !g_rk_user_write_meth && (!strcmp(mname, "say") || !strcmp(mname, "print"))) {
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
                tree_t * mc = ast_node_new(TT_FNC);
                mc->v.sval = (char *)"__multi_call";
                tree_t * nmv = ast_node_new(TT_VAR);
                nmv->v.sval = (char *)"__multi_call";
                ast_push(mc, nmv);
                tree_t * basq = ast_node_new(TT_QLIT);
                basq->v.sval = (char *)intern(mbase);
                ast_push(mc, basq);
                for (int i = 1; i < t->n; i++) ast_push(mc, t->c[i]);
                return lower_rcall(cx, mc, "__multi_call", 1, γ, ω, res);
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
        { IR_t * nd = build(cx, IR_FIELD_GET, γ, ω); IR_LIT(nd).sval = t->v.sval; IR_t * sv = build(cx, IR_VAR, nd, ω); IR_LIT(sv).sval = "self"; ir_operand_push(nd, sv); *res = nd; return sv; }
        case TT_FIELD:
        {
            const char * fname = (t->n > 1 && t->c[1]) ? t->c[1]->v.sval : t->v.sval;
            IR_t * nd = build(cx, IR_CALL, γ, ω);
            IR_LIT(nd).sval = "field_get_pub";
            IR_t * nl = build(cx, IR_LIT_STRING, nd, ω);
            IR_LIT(nl).sval = fname;
            IR_t * or_ = NULL;
            IR_t * eo = lower_rv(cx, t->c[0], nl, ω, &or_);
            if (or_) ir_operand_push(nd, or_);
            ir_operand_push(nd, nl);
            *res = nd;
            return eo;
        }
        case TT_SUSPEND:
        return rk_excise(cx, γ, ω, res);
        case TT_TERNARY:
        if (t->n > 2) {
            static int tern_n = 0;
            char tn[32];
            snprintf(tn, sizeof tn, "$?tern%d", tern_n++);
            const char * tname = lp_strdup(tn);
            IR_t * jv = build(cx, IR_VAR, γ, ω);
            IR_LIT(jv).sval = tname;
            IR_t * at = build(cx, IR_ASSIGN, jv, ω);
            IR_LIT(at).sval = tname;
            IR_t * af = build(cx, IR_ASSIGN, jv, ω);
            IR_LIT(af).sval = tname;
            IR_t * et = rk_tern_branch(cx, t->c[1], at, ω);
            IR_t * ef = rk_tern_branch(cx, t->c[2], af, ω);
            IR_t * e = lower_cond(cx, t->c[0], et, ef);
            *res = jv;
            return e;
        }
        { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
        case TT_RETURN:
        {
            IR_t * exit_γ = cx->proc_exit ? cx->proc_exit : γ;
            if (t->n > 0 && t->c[0]) {
                IR_t * nd = build(cx, IR_RETURN, exit_γ, ω);
                IR_t * trace = NULL;
                IR_t * trace_entry = nd;
                if (cx->cur_proc_name && *cx->cur_proc_name && trace_wanted() && !rk_trace_is_block(cx->cur_proc_name)) {
                    trace = build(cx, IR_CALL, nd, ω);
                    IR_LIT(trace).sval = "__trace_return";
                    IR_t * nm = build(cx, IR_LIT_STRING, trace, ω);
                    IR_LIT(nm).sval = rk_trace_name(cx->cur_proc_name);
                    ir_operand_push(trace, nm);
                    trace_entry = nm;
                }
                IR_t * value_γ = trace_entry;
                for (int td = 0; td < cx->try_depth; td++) { IR_t * te = build(cx, IR_CALL, value_γ, ω); IR_LIT(te).sval = "try_exit"; value_γ = te; }
                IR_t * r = NULL;
                IR_t * e = lower_rv(cx, t->c[0], value_γ, ω, &r);
                ir_operand_push(nd, r ? r : e);
                if (trace) ir_operand_push(trace, r ? r : e);
                *res = nd;
                return e;
            }
            IR_t * nd = build(cx, IR_RETURN, exit_γ, ω);
            IR_t * rentry = nd;
            for (int td = 0; td < cx->try_depth; td++) { IR_t * te = build(cx, IR_CALL, rentry, ω); IR_LIT(te).sval = "try_exit"; rentry = te; }
            *res = nd;
            return rentry;
        }
        default:
        { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_reclassify_calls(void) {
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_register_proc(const tree_t * proc, const char * name, int nparams) {
    int pi = stage2_proc_grow(&g_stage2);
    g_stage2.proc_table[pi].name = lp_strdup(name);
    g_stage2.proc_table[pi].proc = (tree_t *)(intptr_t) proc;
    g_stage2.proc_table[pi].entry_pc = -1;
    g_stage2.proc_table[pi].bb_idx = -1;
    g_stage2.proc_table[pi].nparams = nparams;
    g_stage2.proc_table[pi].byref_mask = rk_param_byref_mask(proc, nparams);
    {
        const tree_t * lp = (proc && nparams > 0 && nparams < proc->n) ? proc->c[nparams] : (const tree_t *)0;
        if (lp && lp->n > 0 && lp->c[0] && lp->c[0]->t == TT_QLIT && lp->c[0]->v.sval && !strcmp(lp->c[0]->v.sval, "*@")) {
            g_stage2.proc_table[pi].is_variadic = 1;
            g_stage2.proc_table[pi].rest_kind = 1;
        } else if (lp && lp->n > 0 && lp->c[0] && lp->c[0]->t == TT_QLIT && lp->c[0]->v.sval && !strcmp(lp->c[0]->v.sval, "**@")) {
            g_stage2.proc_table[pi].is_variadic = 1;
            g_stage2.proc_table[pi].rest_kind = 2;
        } else if (lp && lp->n > 0 && lp->c[0] && lp->c[0]->t == TT_QLIT && lp->c[0]->v.sval && !strcmp(lp->c[0]->v.sval, "*%")) {
            g_stage2.proc_table[pi].named_rest = nparams;
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_proc_known(const char * name) {
    if (!name) return 0;
    for (int i = 0; i < g_stage2.proc_count; i++) if (g_stage2.proc_table[i].name && !strcmp(g_stage2.proc_table[i].name, name)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_discover_grammars(const tree_t * prog) {
    extern void rt_grammar_register(const char *qname, const char *body, int flavor);
    g_rk_gram_names.len = 0;
    if (!prog) return;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) { const tree_t * sub = stmt_subj(d); if (!sub) continue; d = sub; }
        if (!d || d->t != TT_GRAMMAR_DECL) continue;
        const char * gname = (d->n > 0 && d->c[0] && d->c[0]->v.sval) ? d->c[0]->v.sval : NULL;
        if (!gname || !*gname) continue;
        if (!rk_is_grammar_name(gname)) CV_PUSH(g_rk_gram_names, const char *) = gname;
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
static void rk_register_classes(const tree_t * prog) {
    extern void record_register(const char *spec);
    if (!prog) return;
    g_rk_class_names.len = 0;
    g_rk_user_write_meth = 0;
    memset(g_rk_listlike_overridden, 0, sizeof g_rk_listlike_overridden);
    for (int i = 0; i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) { const tree_t * sub = stmt_subj(d); if (!sub) continue; d = sub; }
        if (!d || (d->t != TT_CLASS_DECL && d->t != TT_ROLE_DECL)) continue;
        const char * cname = (d->n > 0 && d->c[0] && d->c[0]->v.sval) ? d->c[0]->v.sval : NULL;
        if (!cname || !*cname) continue;
        if (rk_type_provides_real_method(d, "say") || rk_type_provides_real_method(d, "print")) g_rk_user_write_meth = 1;
        for (int li = 0; RK_LISTLIKE_METHNAMES[li]; li++) if (rk_type_provides_real_method(d, RK_LISTLIKE_METHNAMES[li])) g_rk_listlike_overridden[li] = 1;
        if (!rk_is_class_name(cname)) CV_PUSH(g_rk_class_names, const char *) = cname;
        size_t specn = strlen(cname) + 3;
        for (int j = 1; j < d->n; j++) if (d->c[j] && d->c[j]->t != TT_SUB_DECL) specn += 1 + (d->c[j]->v.sval ? strlen(d->c[j]->v.sval) : 0);
        char spec[specn];
        int pos = 0;
        pos += snprintf(spec + pos, sizeof(spec) - pos, "%s(", cname);
        int first_field = 1;
        for (int j = 1; j < d->n; j++) {
            const tree_t * ch = d->c[j];
            if (!ch || ch->t == TT_SUB_DECL) continue;
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
            else if (dv->t == TT_FLIT) dat_set_field_default_r(cname, fn, dv->v.dval);
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
            if (!ch || ch->t == TT_SUB_DECL) continue;
            if (!rk_fld_priv(ch->v.sval)) continue;
            const char * fn = rk_fld_bare(ch->v.sval ? ch->v.sval : "");
            if (*fn) dat_set_field_priv(cname, fn);
        }
    }
    extern void class_inherit_multi(const char *child, const char **parents, int nparents);
    for (int i = 0; i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) { const tree_t * sub = stmt_subj(d); if (!sub) continue; d = sub; }
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
        if (ch && ch->t == TT_STMT) { const tree_t * sub = stmt_subj(ch); if (!sub) continue; ch = sub; }
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
    if (t->t == TT_CLASS_DECL || t->t == TT_ROLE_DECL || t->t == TT_MODULE_DECL || t->t == TT_GRAMMAR_DECL) return;
    if (t->t == TT_SUB_DECL) {
        const char * nm = (t->n > 0 && t->c[0] && t->c[0]->v.sval) ? t->c[0]->v.sval : NULL;
        if (nm && *nm && !rk_proc_known(nm)) { rk_register_proc(t, nm, (int) t->v.ival); rk_discover_nested_subs(t); }
        return;
    }
    for (int i = 0; i < t->n; i++) rk_discover_block_subs(t->c[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_discover_procs(const tree_t * prog) {
    if (!prog) return;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) { const tree_t * sub = stmt_subj(d); if (!sub) continue; d = sub; }
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
                rk_register_proc(ch, qname, np);
            }
        } else if (d->t == TT_MODULE_DECL) {
            for (int j = 1; j < d->n; j++) {
                const tree_t * ch = d->c[j];
                if (ch && ch->t == TT_STMT) { const tree_t * sub = stmt_subj(ch); if (!sub) continue; ch = sub; }
                if (!ch || ch->t != TT_SUB_DECL) continue;
                const char * nm = (ch->n > 0 && ch->c[0] && ch->c[0]->v.sval) ? ch->c[0]->v.sval : NULL;
                if (!nm) continue;
                rk_register_proc(ch, nm, (int) ch->v.ival);
            }
        }
    }
    for (int i = 0; i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) { const tree_t * sub = stmt_subj(d); if (!sub) continue; d = sub; }
        if (d && d->t == TT_SUB_DECL) rk_discover_nested_subs(d);
        else if (d && d->t != TT_MODULE_DECL) rk_discover_block_subs(d);
        else if (d && d->t == TT_MODULE_DECL) {
            for (int j = 1; j < d->n; j++) {
                const tree_t * ch = d->c[j];
                if (ch && ch->t == TT_STMT) { const tree_t * sub = stmt_subj(ch); if (!sub) continue; ch = sub; }
                if (ch && ch->t == TT_SUB_DECL) rk_discover_nested_subs(ch);
            }
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * rk_prologue_target(const tree_t * s) {
    if (!s || s->t != TT_UNLESS || s->n < 1 || !s->c[0]) return NULL;
    const tree_t * mc = s->c[0];
    if (mc->t != TT_METHCALL || mc->n < 2 || !mc->c[0] || !mc->c[1] || !mc->c[1]->v.sval || strcmp(mc->c[1]->v.sval, "defined")) return NULL;
    return (mc->c[0]->t == TT_VAR) ? mc->c[0]->v.sval : NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * lower_raku_proc(const tree_t * prog, const tree_t * pd) {
    IR_graph_t * g = IR_alloc(8192);
    rcx_t cx;
    cx.try_depth = 0;
    cx.g = g;
    cx.try_catch = NULL;
    cx.loop_exit = NULL;
    cx.loop_next = NULL;
    cx.cur_proc = pd;
    cx.cur_byref_mask = 0;
    cx.cur_nparams = 0;
    const char * rk_proc_name = NULL;
    for (int _pbi = 0; pd && _pbi < g_stage2.proc_count; _pbi++) if (g_stage2.proc_table[_pbi].proc == pd) {
        cx.cur_byref_mask = g_stage2.proc_table[_pbi].byref_mask;
        cx.cur_nparams = g_stage2.proc_table[_pbi].nparams;
        rk_proc_name = g_stage2.proc_table[_pbi].name;
        break;
    }
    cx.cur_proc_name = rk_proc_name;
    if (pd && pd->t == TT_SUB_DECL && rk_catch_of(pd)) {
        const tree_t * ct = rk_catch_of(pd);
        tree_t * p2 = ast_node_new(pd->t);
        p2->v = pd->v;
        p2->slen = pd->slen;
        p2->line = pd->line;
        int k = 0;
        while (k < pd->n && (k == 0 || (pd->c[k] && pd->c[k]->t == TT_VAR))) ast_push(p2, pd->c[k++]);
        tree_t * nb = ast_node_new(TT_SEQ_EXPR);
        nb->slen = 78;
        for (; k < pd->n; k++) ast_push(nb, pd->c[k]);
        tree_t * nt = ast_node_new(TT_TRY);
        nt->slen = 77;
        ast_push(nt, nb);
        ast_push(nt, (tree_t *) rk_catch_handler(ct));
        ast_push(p2, nt);
        pd = p2;
    }
    IR_t * succ = IR_node_alloc(g, IR_SUCCEED);
    IR_t * fail = IR_node_alloc(g, IR_FAIL);
    IR_t * fall = build(&cx, IR_RETURN, succ, fail);
    IR_t * sentry = fall;
    IR_t * entry = fall;
    cx.proc_exit = succ;
    int is_multi = (pd && pd->n > 0 && pd->c[0] && pd->c[0]->v.sval) && strchr(pd->c[0]->v.sval, '$');
    int rk_np = 0;
    if (pd && !is_multi) for (int k = 1; k < pd->n && pd->c[k] && pd->c[k]->t == TT_VAR; k++) rk_np++;
    int rk_bstart = rk_np + 1, rk_bn = (pd && pd->n > rk_bstart) ? pd->n - rk_bstart : 0;
    const tree_t ** rk_plan = (rk_np > 0) ? (const tree_t **) ct_zalloc((size_t) (rk_bn + rk_np), sizeof(const tree_t *)) : NULL;
    if (rk_plan) {
        int nplan = 0, bi = 0;
        for (int k = 1; k <= rk_np; k++) {
            const tree_t * pv = pd->c[k];
            const char * pn = (pv && pv->t == TT_VAR) ? pv->v.sval : NULL;
            const char * tgt = (bi < rk_bn) ? rk_prologue_target(pd->c[rk_bstart + bi]) : NULL;
            if (pn && tgt && !strcmp(tgt, pn)) rk_plan[nplan++] = pd->c[rk_bstart + bi++];
            if (pn && pv->n >= 1 && pv->c[0] && pv->c[0]->v.sval) {
                const char * ty = pv->c[0]->v.sval;
                if (strstr(ty, ":D") || strstr(ty, ":U") || rk_is_modeled_type(ty)) {
                    tree_t * mc = ast_node_new(TT_FNC);
                    mc->v.sval = (char *)"__param_check";
                    tree_t * nmv = ast_node_new(TT_VAR);
                    nmv->v.sval = (char *)"__param_check";
                    ast_push(mc, nmv);
                    tree_t * tyq = ast_node_new(TT_QLIT);
                    tyq->v.sval = (char *)ty;
                    ast_push(mc, tyq);
                    tree_t * pvr = ast_node_new(TT_VAR);
                    pvr->v.sval = (char *)pn;
                    ast_push(mc, pvr);
                    tree_t * pnq = ast_node_new(TT_QLIT);
                    pnq->v.sval = (char *)pn;
                    ast_push(mc, pnq);
                    rk_plan[nplan++] = mc;
                }
            }
        }
        for (; bi < rk_bn; bi++) rk_plan[nplan++] = pd->c[rk_bstart + bi];
        for (int i = nplan - 1; i >= 0; i--) {
            const tree_t * s = rk_plan[i];
            if (!s) continue;
            if (s->t == TT_STMT) { const tree_t * sub = stmt_subj(s); if (!sub) continue; s = sub; }
            if (s->t == TT_VAR) continue;
            IR_t * r = NULL;
            IR_t * e = lower_rv(&cx, s, sentry, fail, &r);
            if (e) e = trace_stmt_wrap(&cx, trace_stmt_line(s), e, fail);
            if (e) { entry = e; sentry = e; }
        }
        ct_drop(rk_plan);
        g->entry = trace_call_wrap(&cx, rk_proc_name, entry, fail);
        return g;
    }
    for (int i = (pd ? pd->n : 0) - 1; i >= 1; i--) {
        const tree_t * s = pd->c[i];
        if (!s) continue;
        if (s->t == TT_STMT) { const tree_t * sub = stmt_subj(s); if (!sub) continue; s = sub; }
        if (s->t == TT_VAR) continue;
        IR_t * r = NULL;
        IR_t * e = lower_rv(&cx, s, sentry, fail, &r);
        if (e) e = trace_stmt_wrap(&cx, trace_stmt_line(s), e, fail);
        if (e) { entry = e; sentry = e; }
    }
    if (pd && !is_multi) {
        for (int k = pd->n - 1; k >= 1; k--) {
            const tree_t * pv = pd->c[k];
            if (!pv || pv->t != TT_VAR || !pv->v.sval) continue;
            if (pv->n < 1 || !pv->c[0] || !pv->c[0]->v.sval) continue;
            const char * ty = pv->c[0]->v.sval;
            if (!strstr(ty, ":D") && !strstr(ty, ":U") && !rk_is_modeled_type(ty)) continue;
            const char * pn = pv->v.sval;
            tree_t * mc = ast_node_new(TT_FNC);
            mc->v.sval = (char *) "__param_check";
            tree_t * nmv = ast_node_new(TT_VAR);
            nmv->v.sval = (char *) "__param_check";
            ast_push(mc, nmv);
            tree_t * tyq = ast_node_new(TT_QLIT);
            tyq->v.sval = (char *) ty;
            ast_push(mc, tyq);
            tree_t * pvr = ast_node_new(TT_VAR);
            pvr->v.sval = (char *) pn;
            ast_push(mc, pvr);
            tree_t * pnq = ast_node_new(TT_QLIT);
            pnq->v.sval = (char *) pn;
            ast_push(mc, pnq);
            IR_t * r = NULL;
            IR_t * e = lower_rcall(&cx, mc, "__param_check", 1, sentry, fail, &r);
            if (e) { entry = e; sentry = e; }
        }
    }
    g->entry = trace_call_wrap(&cx, rk_proc_name, entry, fail);
    return g;
}
#include "stage2.h"
#include "bb_program.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * rk_gram_class_members(const char * nm) {
    if (!nm) return NULL;
    if (!strcmp(nm, "digit")) return "0123456789";
    if (!strcmp(nm, "alpha")) return "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (!strcmp(nm, "alnum")) return "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (!strcmp(nm, "upper")) return "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (!strcmp(nm, "lower")) return "abcdefghijklmnopqrstuvwxyz";
    if (!strcmp(nm, "space")) return " \t\n\r";
    if (!strcmp(nm, "xdigit")) return "0123456789abcdefABCDEF";
    return NULL;
}
typedef struct { int is_lit; const char * s; int n; } rk_gleaf_t;
static int rk_gram_leaf_cap(const char * b) { return (int) strlen(b) / 2 + 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_gram_seq_leaves(const char * body, rk_gleaf_t * out, int maxlv) {
    if (!body || !out || maxlv < 1) return 0;
    int n = (int) strlen(body);
    int i = 0;
    int nlv = 0;
    while (i < n) {
        while (i < n && (body[i]==' '||body[i]=='\t'||body[i]=='\n'||body[i]=='\r')) i++;
        if (i >= n) break;
        if (nlv >= maxlv) return 0;
        if (body[i] == '"' || body[i] == '\'') {
            char q = body[i++];
            int op = 0;
            out[nlv].s = body + i;
            while (i < n && body[i] != q) { if (body[i] == '\\') return 0; op++; i++; }
            if (i >= n || op == 0) return 0;
            i++;
            out[nlv].n = op;
            out[nlv].is_lit = 1;
            nlv++;
        } else if (body[i] == '<') {
            i++;
            if (i < n && body[i] == '.') i++;
            int s = i;
            char nm[n - i + 1];
            int nl = 0;
            while (i < n && (isalnum((unsigned char)body[i]) || body[i] == '_')) nm[nl++] = body[i++];
            nm[nl] = '\0';
            if (i == s || i >= n || body[i] != '>') return 0;
            i++;
            const char * cs = rk_gram_class_members(nm);
            if (!cs) return 0;
            out[nlv].s = cs;
            out[nlv].n = (int) strlen(cs);
            out[nlv].is_lit = 0;
            nlv++;
        } else return 0;
    }
    return nlv;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_gram_split_alt(const char * body, char * lbuf, int lsz, char * rbuf, int rsz) {
    if (!body || !lbuf || !rbuf) return 0;
    int n = (int) strlen(body);
    int depth = 0;
    for (int i = 0; i < n; i++) {
        char c = body[i];
        if (c == '"' || c == '\'') { char q = c; i++; while (i < n && body[i] != q) i++; continue; }
        if (c == '<') { depth++; continue; }
        if (c == '>') { if (depth > 0) depth--; continue; }
        if (c == '|' && depth == 0) {
            int ll = i;
            while (ll > 0 && (body[ll-1]==' '||body[ll-1]=='\t')) ll--;
            if (ll <= 0 || ll >= lsz) return 0;
            memcpy(lbuf, body, (size_t)ll);
            lbuf[ll] = '\0';
            int rs = i + 1;
            while (rs < n && (body[rs]==' '||body[rs]=='\t')) rs++;
            int rl = n - rs;
            if (rl <= 0 || rl >= rsz) return 0;
            memcpy(rbuf, body + rs, (size_t)rl);
            rbuf[rl] = '\0';
            return 1;
        }
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * rk_gram_build_leaf_chain(IR_graph_t * gg, rk_gleaf_t * lv, int nlv, IR_t * fail_tgt, int beta_tag) {
    IR_t * next = NULL;
    for (int e = nlv - 1; e >= 0; e--) {
        IR_t * nd = lc_build(gg, lv[e].is_lit ? IR_GLIT : IR_GCC, next, NULL);
        IR_LIT(nd).sval = ct_strndup(lv[e].s, (size_t) lv[e].n);
        if (beta_tag) lc_ω_to_β(nd, fail_tgt);
        else lc_ω_to(nd, fail_tgt);
        next = nd;
    }
    return next;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_lower_grammar_boxes(const tree_t * prog) {
    extern int g_opt_dump_bb;
    static int nat = -1;
    if (nat < 0) { const char *e = getenv("RK_GRAM_NATIVE"); nat = (e && e[0] == '0') ? 0 : 1; }
    if (!g_opt_dump_bb && !nat) return;
    if (!prog) return;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) { const tree_t * sub = stmt_subj(d); if (!sub) continue; d = sub; }
        if (!d || d->t != TT_GRAMMAR_DECL) continue;
        const char * gname = (d->n > 0 && d->c[0] && d->c[0]->v.sval) ? d->c[0]->v.sval : NULL;
        if (!gname || !*gname) continue;
        for (int j = 1; j < d->n; j++) {
            const tree_t * rd = d->c[j];
            if (!rd || rd->t != TT_REGEX_DECL) continue;
            const char * rname = (rd->n > 0 && rd->c[0] && rd->c[0]->v.sval) ? rd->c[0]->v.sval : NULL;
            const char * body = (rd->n > 1 && rd->c[1] && rd->c[1]->v.sval) ? rd->c[1]->v.sval : NULL;
            if (!rname || !body) continue;
            char pn[fmt_len("gram__%s__%s", gname, rname)];
            snprintf(pn, sizeof pn, "gram__%s__%s", gname, rname);
            IR_graph_t * gg = IR_alloc(64);
            IR_t * entry = NULL;
            char lbody[strlen(body) + 1];
            char rbody[strlen(body) + 1];
            if (rk_gram_split_alt(body, lbody, (int) sizeof lbody, rbody, (int) sizeof rbody)) {
                rk_gleaf_t lv1[rk_gram_leaf_cap(lbody)];
                int nlv1 = rk_gram_seq_leaves(lbody, lv1, rk_gram_leaf_cap(lbody));
                rk_gleaf_t lv2[rk_gram_leaf_cap(rbody)];
                int nlv2 = rk_gram_seq_leaves(rbody, lv2, rk_gram_leaf_cap(rbody));
                if (nlv1 <= 0 || nlv2 <= 0) continue;
                IR_t * galt = lc_build(gg, IR_GALT, NULL , NULL );
                IR_t * arm2_root = rk_gram_build_leaf_chain(gg, lv2, nlv2, NULL, 0);
                ir_operand_push(galt, arm2_root);
                IR_t * arm1_root = rk_gram_build_leaf_chain(gg, lv1, nlv1, galt, 1 );
                ir_operand_push(galt, arm1_root);
                entry = galt;
            } else {
                rk_gleaf_t lv[rk_gram_leaf_cap(body)];
                int nlv = rk_gram_seq_leaves(body, lv, rk_gram_leaf_cap(body));
                if (nlv <= 0) continue;
                IR_t * next = NULL;
                for (int e = nlv - 1; e >= 0; e--) { IR_t * nd = lc_build(gg, lv[e].is_lit ? IR_GLIT : IR_GCC, next, NULL); IR_LIT(nd).sval = ct_strndup(lv[e].s, (size_t) lv[e].n); next = nd; }
                entry = next;
            }
            gg->entry = entry;
            int gidx = bb_program_add(&g_stage2.bbp, gg);
            int pidx = stage2_proc_grow(&g_stage2);
            g_stage2.proc_table[pidx].name = lp_strdup(pn);
            g_stage2.proc_table[pidx].proc = (tree_t *) rd;
            g_stage2.proc_table[pidx].entry_pc = -1;
            g_stage2.proc_table[pidx].bb_idx = gidx;
            g_stage2.proc_table[pidx].nparams = 0;
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int lower_raku_body(const tree_t *prog, const tree_t *proc) { IR_graph_t * ng = lower_raku_proc(prog, proc); if (!ng || !ng->entry) return -1; return bb_program_add(&g_stage2.bbp, ng); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_graph_assigns(const IR_graph_t * g, const char * nm) {
    for (int i = 0; g && i < g->n; i++) { IR_t * m = g->all[i]; if (m && m->op == IR_ASSIGN && IR_LIT(m).sval && !strcmp(IR_LIT(m).sval, nm)) return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_graph_param(const IR_graph_t * g, const char * nm) { for (int i = 0; g && g->pnames && i < g->nparams; i++) if (g->pnames[i] && !strcmp(g->pnames[i], nm)) return 1; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_file_scope_reads_are_globals(void) {
    extern void global_register(const char * name);
    extern int is_global(const char *);
    IR_graph_t * mg = NULL;
    for (int pi = 0; pi < g_stage2.proc_count; pi++) {
        int bi = g_stage2.proc_table[pi].bb_idx;
        if (bi >= 0 && bi < g_stage2.bbp.count && g_stage2.proc_table[pi].name && !strcmp(g_stage2.proc_table[pi].name, "main")) mg = g_stage2.bbp.table[bi];
    }
    if (!mg) return;
    for (int pi = 0; pi < g_stage2.proc_count; pi++) {
        int bi = g_stage2.proc_table[pi].bb_idx;
        if (bi < 0 || bi >= g_stage2.bbp.count || g_stage2.bbp.table[bi] == mg) continue;
        IR_graph_t * g = g_stage2.bbp.table[bi];
        for (int i = 0; g && i < g->n; i++) {
            IR_t * m = g->all[i];
            const char * nm = (m && m->op == IR_VAR) ? IR_LIT(m).sval : NULL;
            if (!nm || nm[0] == '&' || is_global(nm) || rk_graph_assigns(g, nm) || rk_graph_param(g, nm) || !rk_graph_assigns(mg, nm)) continue;
            int writer = 0;
            for (int pj = 0; pj < g_stage2.proc_count && !writer; pj++) {
                int bj = g_stage2.proc_table[pj].bb_idx;
                if (bj >= 0 && bj < g_stage2.bbp.count && g_stage2.bbp.table[bj] != mg && rk_graph_assigns(g_stage2.bbp.table[bj], nm)) writer = 1;
            }
            if (!writer) global_register(nm);
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_dyn_name(const char * nm) { return nm && (nm[0] == '*' || ((nm[0] == '@' || nm[0] == '%') && nm[1] == '*')); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_unassigned_dynamics_are_globals(void) {
    extern void global_register(const char * name);
    extern int is_global(const char *);
    for (int pi = 0; pi < g_stage2.proc_count; pi++) {
        int bi = g_stage2.proc_table[pi].bb_idx;
        if (bi < 0 || bi >= g_stage2.bbp.count || !g_stage2.bbp.table[bi]) continue;
        IR_graph_t * g = g_stage2.bbp.table[bi];
        for (int i = 0; i < g->n; i++) {
            IR_t * m = g->all[i];
            const char * nm = (m && m->op == IR_VAR) ? IR_LIT(m).sval : NULL;
            if (!rk_dyn_name(nm) || is_global(nm) || rk_graph_param(g, nm)) continue;
            int assigned = 0;
            for (int pj = 0; pj < g_stage2.proc_count && !assigned; pj++) {
                int bj = g_stage2.proc_table[pj].bb_idx;
                if (bj >= 0 && bj < g_stage2.bbp.count && g_stage2.bbp.table[bj] && rk_graph_assigns(g_stage2.bbp.table[bj], nm)) assigned = 1;
            }
            if (!assigned) global_register(nm);
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * rk_eval_call_next(int * pi, int * i) {
    for (; *pi < g_stage2.proc_count; (*pi)++, *i = 0) {
        int bi = g_stage2.proc_table[*pi].bb_idx;
        if (bi < 0 || bi >= g_stage2.bbp.count || !g_stage2.bbp.table[bi]) continue;
        IR_graph_t * g = g_stage2.bbp.table[bi];
        while (*i < g->n) { IR_t * m = g->all[(*i)++]; if (m && (m->op == IR_CALL || m->op == IR_CALL_BUILTIN) && IR_LIT(m).sval && !strcmp(IR_LIT(m).sval, "__rk_eval")) return m; }
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_eval_main_globals(void) {
    extern void global_register(const char * name);
    extern int is_global(const char *);
    IR_graph_t * mg = NULL;
    for (int pi = 0; pi < g_stage2.proc_count; pi++) {
        int bi = g_stage2.proc_table[pi].bb_idx;
        if (bi >= 0 && bi < g_stage2.bbp.count && g_stage2.bbp.table[bi] && g_stage2.proc_table[pi].name && !strcmp(g_stage2.proc_table[pi].name, "main")) mg = g_stage2.bbp.table[bi];
    }
    int pi0 = 0, i0 = 0;
    if (!mg || !rk_eval_call_next(&pi0, &i0)) return;
    for (int i = 0; i < mg->n; i++) {
        IR_t * m = mg->all[i];
        const char * nm = (m && m->op == IR_ASSIGN) ? IR_LIT(m).sval : NULL;
        if (!nm || !nm[0] || nm[0] == '&' || !strncmp(nm, "__", 2) || is_global(nm)) continue;
        const char * bare = (nm[0] == '@' || nm[0] == '%') ? nm + 1 : nm;
        size_t bl = strlen(bare);
        int hit = 0, pj = 0, ij = 0;
        for (IR_t * c = rk_eval_call_next(&pj, &ij); c && !hit; c = rk_eval_call_next(&pj, &ij)) {
            IR_t * a0 = (c->n_operands > 0) ? c->operands[0] : NULL;
            if (!(a0 && a0->op == IR_LIT_STRING && IR_LIT(a0).sval)) { hit = 1; break; }
            for (const char * q = IR_LIT(a0).sval; *q && !hit; q++)
                if ((*q == '$' || *q == '@' || *q == '%') && !strncmp(q + 1, bare, bl) && !(isalnum((unsigned char) q[1 + bl]) || q[1 + bl] == '_' || q[1 + bl] == '-')) hit = 1;
        }
        if (hit) global_register(nm);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void lower_raku_eval_reads_are_globals(int pc0) {
    extern void global_register(const char * name);
    extern int is_global(const char *);
    for (int pi = pc0; pi < g_stage2.proc_count; pi++) {
        int bi = g_stage2.proc_table[pi].bb_idx;
        if (bi < 0 || bi >= g_stage2.bbp.count || !g_stage2.bbp.table[bi]) continue;
        IR_graph_t * g = g_stage2.bbp.table[bi];
        for (int i = 0; i < g->n; i++) {
            IR_t * m = g->all[i];
            const char * nm = (m && m->op == IR_VAR) ? IR_LIT(m).sval : NULL;
            if (!nm || !nm[0] || nm[0] == '&' || !strncmp(nm, "__", 2) || is_global(nm) || rk_graph_assigns(g, nm) || rk_graph_param(g, nm)) continue;
            global_register(nm);
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_register_state_globals(const tree_t * t) {
    extern void global_register(const char * name);
    extern int is_global(const char *);
    if (!t) return;
    if (t->t == TT_VAR && t->v.sval) { const char * n = t->v.sval; const char * b = (n[0] == '@' || n[0] == '%') ? n + 1 : n; if (!strncmp(b, "__rk_st", 7) && !is_global(n)) global_register(n); }
    for (int i = 0; i < t->n; i++) rk_register_state_globals(t->c[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void raku_register_program(stage2_t * s2, const tree_t * prog) {
    extern int polyglot_module_open(stage2_t * s2, const tree_t * s);
    extern void polyglot_module_extend(stage2_t * s2, int mod_idx, const tree_t * s);
    extern void record_register(const char * spec);
    int mod_idx = -1;
    rk_register_state_globals(prog);
    for (int _ci = 0; _ci < prog->n; _ci++) {
        const tree_t * s = prog->c[_ci];
        if (!s || (s->t != TT_STMT && s->t != TT_END)) continue;
        if (mod_idx < 0) mod_idx = polyglot_module_open(s2, s);
        polyglot_module_extend(s2, mod_idx, s);
        tree_t * proc = stmt_attr_expr(stmt_attr_find(s, ":subj"));
        if (!proc) continue;
        if (proc->t == TT_GLOBAL) { for (int _gi = 0; _gi < proc->n; _gi++) if (proc->c[_gi] && proc->c[_gi]->v.sval) global_register(proc->c[_gi]->v.sval); }
        if (proc->t == TT_RECORD && proc->v.sval && *proc->v.sval) {
            size_t specn = strlen(proc->v.sval) + 3;
            for (int _ri = 0; _ri < proc->n; _ri++) specn += 1 + ((proc->c[_ri] && proc->c[_ri]->v.sval) ? strlen(proc->c[_ri]->v.sval) : 0);
            char spec[specn];
            int pos = 0;
            pos += snprintf(spec+pos, sizeof(spec)-pos, "%s(", proc->v.sval);
            for (int _ri = 0; _ri < proc->n && pos < (int)sizeof(spec)-2; _ri++) {
                if (_ri > 0) spec[pos++] = ',';
                const char *fn2 = (proc->c[_ri] && proc->c[_ri]->v.sval) ? proc->c[_ri]->v.sval : "";
                pos += snprintf(spec+pos, sizeof(spec)-pos, "%s", fn2);
            }
            if (pos < (int)sizeof(spec)-1) spec[pos++] = ')';
            spec[pos] = '\0';
            record_register(spec);
        }
        if (proc->t == TT_PROC_DECL) {
            const char *name = NULL;
            if (proc->t == TT_SUB_DECL) {
                if (proc->n > 0 && proc->c[0] && proc->c[0]->t == TT_VAR && proc->c[0]->v.sval && *proc->c[0]->v.sval) name = proc->c[0]->v.sval;
            } else {
                name = (proc->v.sval && *proc->v.sval) ? proc->v.sval :
                    ((proc->n > 0 && proc->c[0] && proc->c[0]->t == TT_VAR && proc->c[0]->v.sval && *proc->c[0]->v.sval) ? proc->c[0]->v.sval : NULL);
            }
            if (name) {
                int _pi = stage2_proc_grow(s2);
                s2->proc_table[_pi].name = name;
                s2->proc_table[_pi].proc = proc;
                s2->proc_table[_pi].entry_pc = -1;
                s2->proc_table[_pi].bb_idx = -1;
                s2->proc_table[_pi].nparams = (int)proc->v.ival;
                s2->proc_table[_pi].byref_mask = 0;
                if (mod_idx >= 0) s2->module_registry.mods[mod_idx].nprocs++;
                if (strcmp(name, "main") == 0 && s2->module_registry.main_mod < 0) s2->module_registry.main_mod = mod_idx;
            }
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_collect_blocks(const tree_t * t, tree_t ** out, int * n, int max) {
    if (!t || *n >= max) return;
    if (t->t == TT_ANON_BLOCK && !t->v.sval) out[(*n)++] = (tree_t *) t;
    for (int i = 0; i < t->n; i++) rk_collect_blocks(t->c[i], out, n, max);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_count_blocks(const tree_t * t) { int n; if (!t) return 0; n = (t->t == TT_ANON_BLOCK && !t->v.sval); for (int i = 0; i < t->n; i++) n += rk_count_blocks(t->c[i]); return n; }
static int rk_var_nodes(const tree_t * t) { int n; if (!t) return 0; n = (t->t == TT_VAR); for (int i = 0; i < t->n; i++) n += rk_var_nodes(t->c[i]); return n; }
static void rk_scan_implicit_params(const tree_t * t, int * topic, const char ** ph, int * nph, int max) {
    if (!t) return;
    if (t->t == TT_ANON_BLOCK || t->t == TT_SUB_DECL) return;
    if (t->t == TT_VAR && t->v.sval) {
        const char * v = t->v.sval;
        if (!strcmp(v, "_")) *topic = 1;
        else if (v[0] == '^') { int seen = 0; for (int i = 0; i < *nph; i++) if (!strcmp(ph[i], v)) seen = 1; if (!seen && *nph < max) ph[(*nph)++] = v; }
    }
    for (int i = 0; i < t->n; i++) rk_scan_implicit_params(t->c[i], topic, ph, nph, max);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_block_tail_is_value(const tree_t * s) {
    if (!s) return 0;
    if (rk_seq_is_logical_and(s)) return 1;
    switch (s->t) {
        case TT_RETURN:
        case TT_NRETURN:
        case TT_PROC_FAIL:
        case TT_SAY:
        case TT_SAY_FH:
        case TT_PRINT:
        case TT_PRINT_FH:
        case TT_IF:
        case TT_UNLESS:
        case TT_CASE:
        case TT_WHILE:
        case TT_UNTIL:
        case TT_REPEAT:
        case TT_FOR:
        case TT_EVERY:
        case TT_DO_WHILE:
        case TT_CLOOP:
        case TT_SEQ:
        case TT_SEQ_EXPR:
        case TT_STMT:
        case TT_SUB_DECL:
        return 0;
        default:
        return 1;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { const char ** v; int n, cap; unsigned char * fl; } rk_ns_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_ns_has(const rk_ns_t * s, const char * x) { for (int i = 0; s && i < s->n; i++) if (!strcmp(s->v[i], x)) return 1; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_ns_addf(rk_ns_t * s, const char * x, int f) {
    if (!x) return;
    for (int i = 0; i < s->n; i++) if (!strcmp(s->v[i], x)) { s->fl[i] |= (unsigned char) f; return; }
    if (s->n >= s->cap) {
        s->cap = s->cap ? s->cap * 2 : 8;
        s->v = (const char **) ct_grow((void *) s->v, sizeof(const char *) * (size_t) s->cap);
        s->fl = (unsigned char *) ct_grow((void *) s->fl, (size_t) s->cap);
    }
    s->fl[s->n] = (unsigned char) f;
    s->v[s->n++] = x;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_ns_add(rk_ns_t * s, const char * x) { rk_ns_addf(s, x, 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_ns_flag(const rk_ns_t * s, const char * x) { for (int i = 0; s && x && i < s->n; i++) if (!strcmp(s->v[i], x)) return s->fl[i]; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_tree_size(const tree_t * t) { int n = 1; for (int i = 0; t && i < t->n; i++) n += rk_tree_size(t->c[i]); return t ? n : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * rk_cap_pname(const tree_t * pv) { if (pv && pv->t == TT_ASSIGN && pv->n) pv = pv->c[0]; return (pv && pv->t == TT_VAR) ? pv->v.sval : NULL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_cap_declared(const tree_t * t, rk_ns_t * out) {
    if (!t || t->t == TT_ANON_BLOCK || t->t == TT_SUB_DECL) return;
    if (t->t == TT_VAR && (t->slen & 4) && t->v.sval) rk_ns_add(out, t->v.sval);
    if (t->t == TT_ITERATE && t->v.sval && strcmp(t->v.sval, "_")) rk_ns_add(out, t->v.sval);
    if (t->t == TT_FOR_RANGE && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR && t->c[0]->v.sval) rk_ns_add(out, t->c[0]->v.sval);
    for (int i = 0; i < t->n; i++) rk_cap_declared(t->c[i], out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_cap_uses(const tree_t * t, rk_ns_t * out) {
    if (!t || t->t == TT_ANON_BLOCK || t->t == TT_SUB_DECL) return;
    if (t->t == TT_VAR && t->v.sval) rk_ns_add(out, t->v.sval);
    for (int i = (t->t == TT_FNC ? 1 : 0); i < t->n; i++) rk_cap_uses(t->c[i], out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_cap_synchronous(const tree_t * parent, int idx) {
    static const char * const meth[] = { "map", "grep", "first", "sort", "reduce", "min", "max", "classify", "categorize", "produce", NULL };
    static const char * const fn[] = { "map", "grep", "first", "sort", "reduce", "__rk_test_lives_ok", "__rk_test_dies_ok", "__rk_test_throws_like", "__rk_test_subtest", "__rk_test_eval_lives_ok",
        "__rk_test_eval_dies_ok", NULL };
    if (!parent) return 0;
    if (parent->t == TT_METHCALL && idx >= 2 && parent->n > 1 && parent->c[1] && parent->c[1]->v.sval) { for (int i = 0; meth[i]; i++) if (!strcmp(parent->c[1]->v.sval, meth[i])) return 1; return 0; }
    if (parent->t == TT_FNC && idx >= 1 && parent->n > 0 && parent->c[0] && parent->c[0]->v.sval) { for (int i = 0; fn[i]; i++) if (!strcmp(parent->c[0]->v.sval, fn[i])) return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_cap_assign_through(tree_t * t, const rk_ns_t * F) {
    if (!t || t->t == TT_ANON_BLOCK || t->t == TT_SUB_DECL) return;
    for (int i = 0; i < t->n; i++) rk_cap_assign_through(t->c[i], F);
    if (t->t == TT_ASSIGN && t->n > 1 && t->c[1] && t->c[1]->t == TT_VAR && t->c[1]->v.sval && rk_ns_has(F, t->c[1]->v.sval)) {
        tree_t * rd = ast_node_new(TT_FNC);
        rd->v.sval = (char *) "__rk_deref";
        ast_push(rd, leaf_sval2(TT_VAR, "__rk_deref"));
        ast_push(rd, t->c[1]);
        t->c[1] = rd;
    }
    if (t->t == TT_METHCALL && t->n > 2 && t->c[0] && t->c[0]->t == TT_VAR && t->c[0]->v.sval && rk_ns_has(F, t->c[0]->v.sval) && t->c[1] && t->c[1]->v.sval &&
        (!strcmp(t->c[1]->v.sval, "push") || !strcmp(t->c[1]->v.sval, "append") || !strcmp(t->c[1]->v.sval, "unshift") || !strcmp(t->c[1]->v.sval, "prepend"))) {
        const char * mn = t->c[1]->v.sval;
        const char * pf = !strcmp(mn, "push") ? "push_pure" : !strcmp(mn, "append") ? "append_pure" : !strcmp(mn, "unshift") ? "unshift_pure" : "prepend_pure";
        tree_t * recv = t->c[0];
        int line = t->line;
        tree_t * inner = ast_node_new(TT_FNC);
        inner->v.sval = (char *) pf;
        ast_push(inner, leaf_sval2(TT_VAR, pf));
        ast_push(inner, leaf_sval2(TT_VAR, recv->v.sval));
        for (int i = 2; i < t->n; i++) ast_push(inner, t->c[i]);
        t->t = TT_FNC;
        t->v.sval = (char *) "__rk_byref_assign";
        t->n = 0;
        t->line = line;
        ast_push(t, leaf_sval2(TT_VAR, "__rk_byref_assign"));
        ast_push(t, recv);
        ast_push(t, inner);
        return;
    }
    if (t->t == TT_HASH_SET && t->n > 2 && t->c[0] && t->c[0]->t == TT_VAR && t->c[0]->v.sval && rk_ns_has(F, t->c[0]->v.sval)) {
        tree_t * inner = ast_node_new(TT_FNC);
        inner->v.sval = (char *) "hash_set_pure";
        ast_push(inner, leaf_sval2(TT_VAR, "hash_set_pure"));
        for (int i = 0; i < t->n; i++) ast_push(inner, t->c[i]);
        tree_t * recv = t->c[0];
        int line = t->line;
        t->t = TT_FNC;
        t->v.sval = (char *) "__rk_byref_assign";
        t->n = 0;
        t->line = line;
        ast_push(t, leaf_sval2(TT_VAR, "__rk_byref_assign"));
        ast_push(t, recv);
        ast_push(t, inner);
        return;
    }
    if (t->t == TT_ASSIGN && t->n > 1 && t->c[0] && t->c[0]->t == TT_VAR && t->c[0]->v.sval && !(t->c[0]->slen & 4) && rk_ns_has(F, t->c[0]->v.sval)) {
        tree_t * lhs = t->c[0], * rhs = t->c[1];
        int line = t->line;
        tree_t tmp = *t;
        (void) tmp;
        t->t = TT_FNC;
        t->v.sval = (char *) "__rk_byref_assign";
        t->n = 0;
        t->line = line;
        ast_push(t, leaf_sval2(TT_VAR, "__rk_byref_assign"));
        ast_push(t, lhs);
        ast_push(t, rhs);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_cap_uses_deep(const tree_t * t, rk_ns_t * out) {
    if (!t) return;
    if (t->t == TT_VAR && t->v.sval) rk_ns_add(out, t->v.sval);
    for (int i = (t->t == TT_FNC ? 1 : 0); i < t->n; i++) rk_cap_uses_deep(t->c[i], out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_cap_declared_deep(const tree_t * t, rk_ns_t * out) {
    if (!t) return;
    if (t->t == TT_VAR && (t->slen & 4) && t->v.sval) rk_ns_add(out, t->v.sval);
    if (t->t == TT_ITERATE && t->v.sval) rk_ns_add(out, t->v.sval);
    if (t->t == TT_FOR_RANGE && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR && t->c[0]->v.sval) rk_ns_add(out, t->c[0]->v.sval);
    if (t->t == TT_ANON_BLOCK) for (int k = 1; k < t->n; k++) { const char * pn = rk_cap_pname(t->c[k]); if (pn) rk_ns_add(out, pn); }
    if (t->t == TT_SUB_DECL) { int np = (int) t->v.ival; for (int k = 0; k < np && 1 + k < t->n; k++) { const char * pn = rk_cap_pname(t->c[1 + k]); if (pn) rk_ns_add(out, pn); } }
    for (int i = 0; i < t->n; i++) rk_cap_declared_deep(t->c[i], out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_cap_is_callee(const tree_t * t, const char * name) { return t && t->t == TT_FNC && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, name); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_cap_name_escapes(const tree_t * t, const char * name, const tree_t * decl) {
    if (!t || t == decl) return 0;
    if (t->t == TT_VAR && t->v.sval && (!strcmp(t->v.sval, name) || (t->v.sval[0] == '&' && !strcmp(t->v.sval + 1, name)))) return 1;
    for (int i = (rk_cap_is_callee(t, name) ? 1 : 0); i < t->n; i++) if (rk_cap_name_escapes(t->c[i], name, decl)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_cap_calls_prepend(tree_t * t, const char * name, const rk_ns_t * F) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_cap_calls_prepend(t->c[i], name, F);
    if (rk_cap_is_callee(t, name)) {
        tree_t ** nc = (tree_t **) ct_alloc(sizeof(tree_t *) * (size_t) (t->n + F->n + 1));
        int k = 0;
        nc[k++] = t->c[0];
        for (int i = 0; i < F->n; i++) nc[k++] = leaf_sval2(TT_VAR, F->v[i]);
        for (int i = 1; i < t->n; i++) nc[k++] = t->c[i];
        t->n = 0;
        for (int i = 0; i < k; i++) ast_push(t, nc[i]);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_cap_nested_find(tree_t * t, tree_t * sd, int np, const rk_ns_t * d) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) {
        tree_t * c = t->c[i];
        if (!c) continue;
        if (c->t == TT_SUB_DECL && c->n > 0 && c->c[0] && c->c[0]->v.sval && *c->c[0]->v.sval && c->c[0]->t == TT_VAR && c->v.ival >= 0 && c->v.ival < 12) {
            const char * nm = c->c[0]->v.sval;
            int cnp = (int) c->v.ival;
            rk_ns_t used = { 0 }, decl = { 0 }, F = { 0 };
            for (int k = cnp + 1; k < c->n; k++) rk_cap_uses_deep(c->c[k], &used);
            for (int k = 1; k <= cnp && k < c->n; k++) {
                const tree_t * pv = c->c[k];
                if (pv && pv->t == TT_ASSIGN && pv->n) rk_cap_uses_deep(pv->c[pv->n - 1], &used);
                { const char * pn = rk_cap_pname(pv); if (pn) rk_ns_add(&decl, pn); }
            }
            for (int k = cnp + 1; k < c->n; k++) rk_cap_declared_deep(c->c[k], &decl);
            for (int k = 0; k < used.n; k++) {
                const char * u = used.v[k];
                if (!strcmp(u, "_") || u[0] == '^' || !strncmp(u, "__", 2) || u[0] == '&' || rk_ns_has(&decl, u) || !rk_ns_has(d, u)) continue;
                rk_ns_add(&F, u);
            }
            if (F.n && F.n + cnp <= 12 && !rk_cap_name_escapes(sd, nm, c)) {
                rk_cap_calls_prepend(sd, nm, &F);
                tree_t ** nc = (tree_t **) ct_alloc(sizeof(tree_t *) * (size_t) (c->n + F.n + 1));
                int k = 0;
                nc[k++] = c->c[0];
                for (int j = 0; j < F.n; j++) { tree_t * pv = leaf_sval2(TT_VAR, F.v[j]); ast_push(pv, leaf_sval2(TT_QLIT, "@")); nc[k++] = pv; }
                for (int j = 1; j < c->n; j++) nc[k++] = c->c[j];
                c->n = 0;
                for (int j = 0; j < k; j++) ast_push(c, nc[j]);
                c->v.ival = cnp + F.n;
                for (int j = 1 + c->v.ival; j < c->n; j++) rk_cap_assign_through(c->c[j], &F);
            }
            rk_cap_nested_find(c, sd, np, d);
            continue;
        }
        if (c->t == TT_ANON_BLOCK) continue;
        rk_cap_nested_find(c, sd, np, d);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_cap_mut_method(const char * m) {
    static const char * const k[] = { "push", "append", "unshift", "prepend", "pop", "shift", "splice", NULL };
    for (int i = 0; m && k[i]; i++) if (!strcmp(m, k[i])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_cap_is_var(const tree_t * t, const char * v) { return t && t->t == TT_VAR && t->v.sval && !strcmp(t->v.sval, v); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_cap_mutated(const tree_t * t, const char * v) {
    if (!t) return 0;
    if ((t->t == TT_ASSIGN || t->t == TT_ARR_SET || t->t == TT_HASH_SET) && t->n > 0 && rk_cap_is_var(t->c[0], v) && !(t->t == TT_ASSIGN && (t->c[0]->slen & 4))) return 1;
    if (t->t == TT_ASSIGN && t->n > 1 && rk_cap_is_var(t->c[0], v) && (t->c[0]->slen & 4)) { rk_ns_t ru = { 0 }; rk_cap_uses_deep(t->c[1], &ru); if (rk_ns_has(&ru, v)) return 1; }
    if (t->t == TT_METHCALL && t->n > 1 && rk_cap_is_var(t->c[0], v) && t->c[1] && rk_cap_mut_method(t->c[1]->v.sval)) return 1;
    if (t->t == TT_FNC && t->n > 1 && t->c[0] && rk_cap_mut_method(t->c[0]->v.sval) && rk_cap_is_var(t->c[1], v)) return 1;
    for (int i = 0; i < t->n; i++) if (rk_cap_mutated(t->c[i], v)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_cap_decl_assigns(const tree_t * t, rk_ns_t * out) {
    if (!t || t->t == TT_ANON_BLOCK || t->t == TT_SUB_DECL) return;
    if (t->t == TT_ASSIGN && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR && (t->c[0]->slen & 4)) rk_ns_add(out, t->c[0]->v.sval);
    for (int i = 0; i < t->n; i++) rk_cap_decl_assigns(t->c[i], out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_cap_boxes(const tree_t * t, const tree_t * sd, int np, const rk_ns_t * own, const rk_ns_t * boxable, rk_ns_t * out) {
    for (int i = 0; t && i < t->n; i++) {
        const tree_t * c = t->c[i];
        if (!c || c->t == TT_SUB_DECL) continue;
        if (c->t == TT_ANON_BLOCK && !rk_cap_synchronous(t, i)) {
            rk_ns_t used = { 0 }, inner = { 0 };
            rk_cap_uses_deep(c, &used);
            rk_cap_declared_deep(c, &inner);
            for (int k = 0; k < used.n; k++) {
                const char * u = used.v[k];
                if (rk_ns_has(&inner, u) || !rk_ns_has(own, u) || !rk_ns_has(boxable, u)) continue;
                int mut = (u[0] == '@' || u[0] == '%');
                for (int j = 1 + np; !mut && j < sd->n; j++) mut = rk_cap_mutated(sd->c[j], u);
                if (mut) rk_ns_add(out, u);
            }
        }
        rk_cap_boxes(c, sd, np, own, boxable, out);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_cap_flag_boxed(tree_t * t, const rk_ns_t * Bx) {
    if (!t) return;
    if (t->t == TT_VAR && t->v.sval && rk_ns_has(Bx, t->v.sval)) t->slen |= RK_VF_BOXED;
    for (int i = 0; i < t->n; i++) { tree_t * c = t->c[i]; if (c && (c->t == TT_ANON_BLOCK || c->t == TT_SUB_DECL)) continue; rk_cap_flag_boxed(c, Bx); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_cap_box_decls(tree_t * t, const rk_ns_t * Bx) {
    if (!t || t->t == TT_ANON_BLOCK || t->t == TT_SUB_DECL) return;
    for (int i = 0; i < t->n; i++) rk_cap_box_decls(t->c[i], Bx);
    if (t->t == TT_ASSIGN && t->n > 1 && t->c[0] && t->c[0]->t == TT_VAR && (t->c[0]->slen & 4) && rk_ns_has(Bx, t->c[0]->v.sval)) {
        const char * v = t->c[0]->v.sval;
        rk_ns_t ru = { 0 };
        rk_cap_uses_deep(t->c[1], &ru);
        tree_t * bx = ast_node_new(TT_FNC);
        bx->line = t->line;
        bx->v.sval = (char *) "__rk_box";
        ast_push(bx, leaf_sval2(TT_VAR, "__rk_box"));
        if (!rk_ns_has(&ru, v)) { ast_push(bx, t->c[1]); t->c[1] = bx; return; }
        tree_t * un = ast_node_new(TT_FNC);
        un->v.sval = (char *) "__rk_undef";
        ast_push(un, leaf_sval2(TT_VAR, "__rk_undef"));
        ast_push(bx, un);
        tree_t * rhs = t->c[1], * lhs = t->c[0];
        tree_t * lv = leaf_sval2(TT_VAR, v);
        lv->slen |= RK_VF_BOXED;
        tree_t * st = ast_node_new(TT_FNC);
        st->line = t->line;
        st->v.sval = (char *) "__rk_byref_assign";
        ast_push(st, leaf_sval2(TT_VAR, "__rk_byref_assign"));
        ast_push(st, lv);
        ast_push(st, rhs);
        t->c[1] = bx;
        tree_t * decl = ast_node_new(TT_ASSIGN);
        decl->line = t->line;
        ast_push(decl, lhs);
        ast_push(decl, bx);
        t->t = TT_SEQ_EXPR;
        t->n = 0;
        ast_push(t, decl);
        ast_push(t, st);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_cap_walk(tree_t * t, const rk_ns_t * encl);
static void rk_cap_block(tree_t * parent, int idx, const rk_ns_t * encl);
static void rk_cap_proc(tree_t * sd) {
    rk_ns_t d = { 0 };
    int np = (int) sd->v.ival;
    for (int k = 0; k < np && 1 + k < sd->n; k++) { const char * pn = rk_cap_pname(sd->c[1 + k]); if (pn) rk_ns_add(&d, pn); }
    for (int i = 1 + np; i < sd->n; i++) rk_cap_declared(sd->c[i], &d);
    rk_ns_t boxable = { 0 }, Bx = { 0 };
    for (int k = 0; k < np && 1 + k < sd->n; k++) { const char * pn = rk_cap_pname(sd->c[1 + k]); if (pn) rk_ns_add(&boxable, pn); }
    for (int i = 1 + np; i < sd->n; i++) rk_cap_decl_assigns(sd->c[i], &boxable);
    rk_cap_boxes(sd, sd, np, &d, &boxable, &Bx);
    for (int i = 0; i < Bx.n; i++) rk_ns_addf(&d, Bx.v[i], 1);
    rk_cap_nested_find(sd, sd, np, &d);
    for (int i = 1 + np; i < sd->n; i++) {
        tree_t * c = sd->c[i];
        if (!c) continue;
        if (c->t == TT_SUB_DECL) rk_cap_proc(c);
        else if (c->t == TT_ANON_BLOCK) rk_cap_block(sd, i, &d);
        else rk_cap_walk(c, &d);
    }
    if (!Bx.n) return;
    for (int i = 1 + np; i < sd->n; i++) { rk_cap_assign_through(sd->c[i], &Bx); rk_cap_flag_boxed(sd->c[i], &Bx); rk_cap_box_decls(sd->c[i], &Bx); }
    int added = 0;
    for (int k = 0; k < np && 1 + k < sd->n; k++) {
        const char * pn = rk_cap_pname(sd->c[1 + k]);
        if (!pn || !rk_ns_has(&Bx, pn)) continue;
        tree_t * bx = ast_node_new(TT_FNC);
        bx->line = sd->line;
        bx->v.sval = (char *) "__rk_box";
        ast_push(bx, leaf_sval2(TT_VAR, "__rk_box"));
        ast_push(bx, leaf_sval2(TT_VAR, pn));
        tree_t * as = ast_node_new(TT_ASSIGN);
        as->line = sd->line;
        ast_push(as, leaf_sval2(TT_VAR, pn));
        ast_push(as, bx);
        ast_push(sd, as);
        for (int j = sd->n - 1; j > 1 + np + added; j--) sd->c[j] = sd->c[j - 1];
        sd->c[1 + np + added] = as;
        added++;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_cap_block(tree_t * parent, int idx, const rk_ns_t * encl) {
    tree_t * B = parent->c[idx];
    rk_ns_t dB = { 0 };
    for (int k = 1; k < B->n; k++) { const char * pn = rk_cap_pname(B->c[k]); if (pn) rk_ns_add(&dB, pn); }
    if (B->n > 0) rk_cap_declared(B->c[0], &dB);
    int wl = rk_cap_synchronous(parent, idx);
    rk_ns_t e2 = { 0 };
    for (int i = 0; encl && i < encl->n; i++) rk_ns_addf(&e2, encl->v[i], encl->fl[i]);
    for (int i = 0; i < dB.n; i++) rk_ns_addf(&e2, dB.v[i], 0);
    if (!wl) for (int i = 0; i < e2.n; i++) e2.fl[i] |= 4;
    if (B->n > 0 && B->c[0]) rk_cap_walk(B->c[0], &e2);
    if (!encl || !encl->n) return;
    rk_ns_t used = { 0 };
    if (B->n > 0) rk_cap_uses(B->c[0], &used);
    rk_ns_t F = { 0 }, FR = { 0 };
    for (int i = 0; i < used.n; i++) {
        const char * u = used.v[i];
        if (!strcmp(u, "_") || u[0] == '^' || !strncmp(u, "__", 2) || rk_ns_has(&dB, u) || !rk_ns_has(encl, u)) continue;
        if (wl && (rk_ns_flag(encl, u) & 15) == 2) continue;
        rk_ns_add(&F, u);
        if (wl || (rk_ns_flag(encl, u) & 1)) rk_ns_add(&FR, u);
    }
    if (!F.n) return;
    const tree_t * body = B->c[0];
    tree_t ** pl = (tree_t **) ct_alloc(sizeof(tree_t *) * (size_t) (F.n + B->n + 24));
    int np = 0;
    for (int i = 0; i < F.n; i++) { tree_t * pv = leaf_sval2(TT_VAR, F.v[i]); if (rk_ns_has(&FR, F.v[i])) ast_push(pv, leaf_sval2(TT_QLIT, "@")); pl[np++] = pv; }
    if (B->n > 1) {
        for (int k = 1; k < B->n; k++) pl[np++] = B->c[k];
    } else {
        int topic = 0, nph = 0, phmax = rk_tree_size(body) + 1;
        const char ** ph = (const char **) ct_alloc(sizeof(const char *) * (size_t) phmax);
        rk_scan_implicit_params(body, &topic, ph, &nph, phmax);
        for (int a = 1; a < nph; a++) { const char * key = ph[a]; int b = a - 1; while (b >= 0 && strcmp(ph[b], key) > 0) { ph[b + 1] = ph[b]; b--; } ph[b + 1] = key; }
        if (nph > 0) { for (int k = 0; k < nph; k++) pl[np++] = leaf_sval2(TT_VAR, ph[k]); } else if (topic) pl[np++] = leaf_sval2(TT_VAR, intern("_"));
    }
    rk_cap_assign_through(B->c[0], &FR);
    B->n = 1;
    for (int k = 0; k < np; k++) ast_push(B, pl[k]);
    tree_t * cl = ast_node_new(TT_FNC);
    cl->line = B->line;
    cl->v.sval = (char *) "__blk_close";
    ast_push(cl, leaf_sval2(TT_VAR, "__blk_close"));
    ast_push(cl, B);
    for (int i = 0; i < F.n; i++) {
        tree_t * av = leaf_sval2(TT_VAR, F.v[i]);
        if (rk_ns_has(&FR, F.v[i])) ast_push(cl, av);
        else { tree_t * bv = ast_node_new(TT_FNC); bv->v.sval = (char *) "__rk_byval"; ast_push(bv, leaf_sval2(TT_VAR, "__rk_byval")); ast_push(bv, av); ast_push(cl, bv); }
    }
    parent->c[idx] = cl;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_cap_walk(tree_t * t, const rk_ns_t * encl) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) {
        tree_t * c = t->c[i];
        if (!c) continue;
        if (c->t == TT_SUB_DECL) rk_cap_proc(c);
        else if (c->t == TT_ANON_BLOCK) rk_cap_block(t, i, encl);
        else rk_cap_walk(c, encl);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_cap_mutated_names(const tree_t * t, rk_ns_t * out) {
    if (!t) return;
    if ((t->t == TT_ASSIGN || t->t == TT_ARR_SET || t->t == TT_HASH_SET) && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR && t->c[0]->v.sval && !(t->t == TT_ASSIGN && (t->c[0]->slen & 4)))
        rk_ns_add(out, t->c[0]->v.sval);
    if (t->t == TT_METHCALL && t->n > 1 && t->c[0] && t->c[0]->t == TT_VAR && t->c[1] && rk_cap_mut_method(t->c[1]->v.sval)) rk_ns_add(out, t->c[0]->v.sval);
    if (t->t == TT_FNC && t->n > 1 && t->c[0] && rk_cap_mut_method(t->c[0]->v.sval) && t->c[1] && t->c[1]->t == TT_VAR) rk_ns_add(out, t->c[1]->v.sval);
    for (int i = 0; i < t->n; i++) rk_cap_mutated_names(t->c[i], out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_cap_block_written(const tree_t * t, const rk_ns_t * fsd, rk_ns_t * out) {
    if (!t || t->t == TT_SUB_DECL) return;
    if (t->t == TT_ANON_BLOCK) {
        rk_ns_t m = { 0 }, inner = { 0 };
        rk_cap_mutated_names(t, &m);
        rk_cap_declared_deep(t, &inner);
        for (int i = 0; i < m.n; i++) if (rk_ns_has(fsd, m.v[i]) && !rk_ns_has(&inner, m.v[i])) rk_ns_add(out, m.v[i]);
        return;
    }
    for (int i = 0; i < t->n; i++) rk_cap_block_written(t->c[i], fsd, out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_cap_is_loop(const tree_t * t) {
    return t && (t->t == TT_WHILE || t->t == TT_UNTIL || t->t == TT_REPEAT || t->t == TT_FOR || t->t == TT_EVERY || t->t == TT_FOR_RANGE || t->t == TT_DO_WHILE || t->t == TT_CLOOP);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_cap_iter_decls(const tree_t * t, int lp, rk_ns_t * out, rk_ns_t * lvs) {
    if (!t || t->t == TT_SUB_DECL || t->t == TT_ANON_BLOCK) return;
    int lp2 = lp || (rk_cap_is_loop(t) && t->t != TT_CLOOP);
    if (lp && t->t == TT_VAR && (t->slen & 4) && t->v.sval) rk_ns_add(out, t->v.sval);
    if (lp && t->t == TT_ITERATE && t->v.sval && strcmp(t->v.sval, "_")) { rk_ns_add(out, t->v.sval); rk_ns_add(lvs, t->v.sval); }
    if (lp2 && t->t == TT_FOR_RANGE && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR && t->c[0]->v.sval) { rk_ns_add(out, t->c[0]->v.sval); rk_ns_add(lvs, t->c[0]->v.sval); }
    for (int i = 0; i < t->n; i++) rk_cap_iter_decls(t->c[i], lp2 || (t->t == TT_CLOOP && i == 3), out, lvs);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_cap_sub_free(const tree_t * t, rk_ns_t * out) {
    if (!t) return;
    if (t->t == TT_SUB_DECL) {
        rk_ns_t u = { 0 }, d = { 0 };
        rk_cap_uses_deep(t, &u);
        rk_cap_declared_deep(t, &d);
        for (int i = 0; i < u.n; i++) if (!rk_ns_has(&d, u.v[i])) rk_ns_add(out, u.v[i]);
        return;
    }
    for (int i = 0; i < t->n; i++) rk_cap_sub_free(t->c[i], out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_cap_file_scope(tree_t * prog) {
    rk_ns_t it = { 0 }, lvs = { 0 }, subn = { 0 }, own = { 0 }, boxable = { 0 }, Bx = { 0 };
    rk_cap_iter_decls(prog, 0, &it, &lvs);
    rk_cap_sub_free(prog, &subn);
    rk_ns_t fsd = { 0 }, bw = { 0 };
    for (int i = 0; i < prog->n; i++) rk_cap_declared(prog->c[i], &fsd);
    rk_cap_block_written(prog, &fsd, &bw);
    for (int i = 0; i < it.n; i++) if (!rk_ns_has(&subn, it.v[i])) rk_ns_addf(&own, it.v[i], 2);
    for (int i = 0; i < bw.n; i++) if (!rk_ns_has(&subn, bw.v[i])) rk_ns_addf(&own, bw.v[i], 10);
    rk_cap_decl_assigns(prog, &boxable);
    for (int i = 0; i < boxable.n; ) { if (rk_ns_has(&lvs, boxable.v[i])) { boxable.v[i] = boxable.v[boxable.n - 1]; boxable.n--; } else i++; }
    rk_cap_boxes(prog, prog, -1, &own, &boxable, &Bx);
    for (int i = 0; i < Bx.n; i++) rk_ns_addf(&own, Bx.v[i], 1);
    rk_cap_walk(prog, &own);
    if (!Bx.n) return;
    rk_cap_assign_through(prog, &Bx);
    rk_cap_flag_boxed(prog, &Bx);
    rk_cap_box_decls(prog, &Bx);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_gw_boxed(const tree_t * t, rk_ns_t * out) {
    if (!t) return;
    if (t->t == TT_VAR && t->v.sval && (t->slen & RK_VF_BOXED)) rk_ns_add(out, t->v.sval);
    for (int i = 0; i < t->n; i++) rk_gw_boxed(t->c[i], out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_gw_collect(const tree_t * t, const rk_ns_t * fsd, const rk_ns_t * excl, rk_ns_t * W) {
    if (!t) return;
    if (t->t == TT_SUB_DECL) {
        rk_ns_t d = { 0 };
        rk_cap_declared_deep(t, &d);
        for (int k = 0; k < fsd->n; k++) {
            const char * v = fsd->v[k];
            if (rk_ns_has(&d, v) || rk_ns_has(excl, v) || rk_ns_has(W, v)) continue;
            for (int j = 1; j < t->n; j++) if (rk_cap_mutated(t->c[j], v)) { rk_ns_add(W, v); break; }
        }
    }
    for (int i = 0; i < t->n; i++) rk_gw_collect(t->c[i], fsd, excl, W);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_gw_rename(tree_t * t, const char * v, const char * to) {
    if (!t) return;
    if (t->t == TT_SUB_DECL || t->t == TT_ANON_BLOCK) { rk_ns_t d = { 0 }; rk_cap_declared_deep(t, &d); if (rk_ns_has(&d, v)) return; }
    if (t->t == TT_VAR && t->v.sval && !strcmp(t->v.sval, v)) t->v.sval = (char *) to;
    for (int i = (t->t == TT_FNC || t->t == TT_SUB_DECL) ? 1 : 0; i < t->n; i++) rk_gw_rename(t->c[i], v, to);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_globalize_file_scope_writes(tree_t * prog) {
    extern void global_register(const char * name);
    rk_ns_t fsd = { 0 }, it = { 0 }, lvs = { 0 }, W = { 0 };
    rk_cap_iter_decls(prog, 0, &it, &lvs);
    rk_gw_boxed(prog, &it);
    for (int i = 0; i < prog->n; i++) rk_cap_declared(prog->c[i], &fsd);
    for (int i = 0; i < fsd.n; ) {
        const char * v = fsd.v[i];
        if (!*v || v[0] == '&' || v[0] == '_' || v[0] == '*' || v[0] == '?' || v[0] == '!' || v[0] == '.' || !strncmp(v, "__", 2)) { fsd.v[i] = fsd.v[fsd.n - 1]; fsd.n--; } else i++;
    }
    rk_gw_collect(prog, &fsd, &it, &W);
    for (int i = 0; i < W.n; i++) { const char * v = W.v[i]; char nb[strlen(v) + 8]; snprintf(nb, sizeof nb, "%s__fs", v); char * to = lp_strdup(nb); rk_gw_rename(prog, v, to); global_register(to); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_tail_return(tree_t * st);
static tree_t * rk_tail_if(tree_t * st) {
    if (!st || (st->t != TT_IF && st->t != TT_UNLESS) || st->n < 2) return st;
    tree_t * nd = ast_node_new(st->t);
    nd->line = st->line;
    nd->v = st->v;
    nd->slen = st->slen;
    for (int i = 0; i < st->n; i++) { tree_t * ch = st->c[i]; ast_push(nd, (i >= 1 && ch && (ch->t == TT_SEQ_EXPR || ch->t == TT_IF || ch->t == TT_UNLESS)) ? rk_tail_return(ch) : ch); }
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_tail_ifs(tree_t * t) {
    if (!t) return;
    if (t->t == TT_SUB_DECL && t->n > 1) {
        int li = t->n - 1;
        tree_t * ls = t->c[li];
        if (ls && ls->t == TT_STMT) { const tree_t * sub = stmt_subj(ls); if (sub) ls = (tree_t *) sub; }
        if (ls && (ls->t == TT_IF || ls->t == TT_UNLESS)) t->c[li] = rk_tail_if(ls);
    }
    for (int i = 0; i < t->n; i++) rk_tail_ifs(t->c[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_tail_return(tree_t * st) {
    if (st && (st->t == TT_IF || st->t == TT_UNLESS)) return rk_tail_if(st);
    if (st && st->t == TT_SEQ_EXPR && st->n > 0) { st->c[st->n - 1] = rk_tail_return(st->c[st->n - 1]); return st; }
    if (!rk_block_tail_is_value(st)) return st;
    tree_t * r = ast_node_new(TT_RETURN);
    r->line = st->line;
    ast_push(r, st);
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_ph_rename(tree_t * t, const char * from, const char * to) {
    if (!t) return;
    if (t->t == TT_VAR && t->v.sval && !strcmp(t->v.sval, from)) t->v.sval = (char *) to;
    for (int i = 0; i < t->n; i++) rk_ph_rename(t->c[i], from, to);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_sub_placeholders(tree_t * t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_sub_placeholders(t->c[i]);
    if (t->t != TT_SUB_DECL || t->v.ival != 0 || t->n < 1) return;
    int topic = 0, nph = 0, phmax = rk_tree_size(t) + 1;
    const char ** ph = (const char **) ct_alloc(sizeof(const char *) * (size_t) phmax);
    for (int i = 1; i < t->n; i++) rk_scan_implicit_params(t->c[i], &topic, ph, &nph, phmax);
    if (nph == 0) return;
    for (int a = 1; a < nph; a++) { const char * key = ph[a]; int b = a - 1; while (b >= 0 && strcmp(ph[b], key) > 0) { ph[b + 1] = ph[b]; b--; } ph[b + 1] = key; }
    int nbody = t->n - 1;
    tree_t ** body = (tree_t **) ct_alloc(sizeof(tree_t *) * (size_t) nbody);
    for (int i = 0; i < nbody; i++) body[i] = t->c[i + 1];
    t->n = 1;
    for (int k = 0; k < nph; k++) ast_push(t, leaf_sval2(TT_VAR, ph[k]));
    for (int i = 0; i < nbody; i++) { for (int k = 0; k < nph; k++) rk_ph_rename(body[i], intern(ph[k] + 1), ph[k]); ast_push(t, body[i]); }
    t->v.ival = nph;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_ph_alias(tree_t * t) {
    if (!t) return;
    if (t->t == TT_ANON_BLOCK && t->n == 1 && t->c[0]) {
        int topic = 0, nph = 0, phmax = rk_tree_size(t->c[0]) + 1;
        const char ** ph = (const char **) ct_alloc(sizeof(const char *) * (size_t) phmax);
        rk_scan_implicit_params(t->c[0], &topic, ph, &nph, phmax);
        for (int k = 0; k < nph; k++) rk_ph_rename(t->c[0], intern(ph[k] + 1), ph[k]);
    }
    for (int i = 0; i < t->n; i++) rk_ph_alias(t->c[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_hoist_anon_blocks(tree_t * prog) {
    if (!prog) return;
    tree_t * blks[rk_count_blocks(prog) + 1];
    int nb = 0;
    rk_collect_blocks(prog, blks, &nb, (int) (sizeof blks / sizeof blks[0]));
    static int g_blk_ctr = 0;
    for (int i = 0; i < nb; i++) {
        extern int rt_proc_is_registered(const char *);
        tree_t * blk = blks[i];
        char nm[64];
        do snprintf(nm, sizeof nm, "__blk_%d", ++g_blk_ctr);
        while (rt_proc_is_registered(nm));
        char * pn = lp_strdup(nm);
        blk->v.sval = pn;
        tree_t * sd = ast_node_new(TT_SUB_DECL);
        sd->v.ival = 0;
        tree_t * nn = ast_node_new(TT_VAR);
        nn->v.sval = pn;
        ast_push(sd, nn);
        const tree_t * body = (blk->n > 0) ? blk->c[0] : NULL;
        if (blk->n > 1) {
            for (int k = 1; k < blk->n; k++) ast_push(sd, blk->c[k]);
            sd->v.ival = blk->n - 1;
        } else {
            int topic = 0, nph = 0;
            const char * ph[rk_var_nodes(body) + 1];
            rk_scan_implicit_params(body, &topic, ph, &nph, (int) (sizeof ph / sizeof ph[0]));
            for (int a = 1; a < nph; a++) { const char * key = ph[a]; int b = a - 1; while (b >= 0 && strcmp(ph[b], key) > 0) { ph[b + 1] = ph[b]; b--; } ph[b + 1] = key; }
            if (nph > 0) {
                for (int k = 0; k < nph; k++) { tree_t * pv = ast_node_new(TT_VAR); pv->v.sval = (char *) ph[k]; ast_push(sd, pv); }
                sd->v.ival = nph;
            } else if (topic) {
                tree_t * pv = ast_node_new(TT_VAR);
                pv->v.sval = (char *) intern("_");
                ast_push(sd, pv);
                sd->v.ival = 1;
            }
        }
        if (body) { for (int k = 0; k < body->n; k++) { tree_t * st = body->c[k]; if (k == body->n - 1) st = rk_tail_return(st); ast_push(sd, st); } ((tree_t *) body)->n = 0; }
        blk->n = 0;
        ast_push(prog, sd);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_rename_main_refs(tree_t * t) {
    if (!t) return;
    if (t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, "main")) {
        t->v.sval = (char *) "&main";
        if (t->n > 0 && t->c[0] && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, "main")) t->c[0]->v.sval = (char *) "&main";
    } else if (t->t == TT_VAR && (t->slen & 1) && t->v.sval && !strcmp(t->v.sval, "main")) t->v.sval = (char *) "&main";
    for (int i = 0; i < t->n; i++) rk_rename_main_refs(t->c[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_listops_to_methcalls(tree_t * t, int is_iter_src) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_listops_to_methcalls(t->c[i], t->t == TT_ITERATE && i == 0);
    if (is_iter_src || (t->t != TT_MAP && t->t != TT_GREP) || t->n < 2 || !t->c[0] || !t->c[1]) return;
    tree_t * body = ast_node_new(TT_SEQ_EXPR);
    ast_push(body, t->c[0]);
    tree_t * blk = ast_node_new(TT_ANON_BLOCK);
    ast_push(blk, body);
    tree_t * src = t->c[1];
    if (t->n > 2) { src = ast_node_new(TT_FNC); src->v.sval = (char *)"__rk_arr"; ast_push(src, leaf_sval2(TT_VAR, "__rk_arr")); for (int i = 1; i < t->n; i++) ast_push(src, t->c[i]); }
    tree_t * m = ast_node_new(TT_METHCALL);
    m->line = t->line;
    ast_push(m, src);
    ast_push(m, leaf_sval2(TT_QLIT, t->t == TT_MAP ? "map" : "grep"));
    ast_push(m, blk);
    *t = *m;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_rename_user_main(tree_t * prog) {
    int found = 0;
    for (int i = 0; prog && i < prog->n; i++) {
        tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) { const tree_t * sub = stmt_subj(d); d = (tree_t *) sub; }
        if (!d || d->t != TT_SUB_DECL || d->n < 1 || !d->c[0] || !d->c[0]->v.sval || strcmp(d->c[0]->v.sval, "main")) continue;
        d->c[0]->v.sval = (char *) "&main";
        if (d->v.sval && !strcmp(d->v.sval, "main")) d->v.sval = (char *) "&main";
        found = 1;
    }
    if (found) rk_rename_main_refs(prog);
}
/*====================================================================================================================================================================================================*/
static const char * rk_ph_of(const tree_t * n) { if (!n || n->t != TT_FNC || !n->v.sval || strncmp(n->v.sval, "__rk_phaser_", 12) != 0) return NULL; return n->v.sval + 12; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_ph_body(tree_t * n) { return (n && n->n >= 2) ? n->c[n->n - 1] : ast_node_new(TT_SEQ_EXPR); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_ph_is_loop(const char * p) { return !strcmp(p, "FIRST") || !strcmp(p, "LAST") || !strcmp(p, "NEXT") || !strcmp(p, "once"); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_ph_rank(const char * p, int mainline) {
    if (!strcmp(p, "BEGIN")) return 0;
    if (!strcmp(p, "CHECK")) return 1;
    if (!strcmp(p, "INIT")) return 2;
    if (!strcmp(p, "ENTER") || !strcmp(p, "PRE")) return 3;
    if (!strcmp(p, "LEAVE") || !strcmp(p, "POST")) return 5;
    if (!strcmp(p, "KEEP")) return mainline ? -1 : 6;
    if (!strcmp(p, "UNDO")) return mainline ? 6 : -1;
    if (!strcmp(p, "END")) return 7;
    if (!strcmp(p, "TEMP")) return -1;
    if (rk_ph_is_loop(p)) return -2;
    return 4;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_ph_unplaced(const char * p) {
    extern void rt_script_die_surface(const char * msg);
    char m[fmt_len("%s { } is implemented at mainline scope and as a loop phaser at the top level of a loop body, not here", p)];
    snprintf(m, sizeof m, "%s { } is implemented at mainline scope and as a loop phaser at the top level of a loop body, not here", p);
    rt_script_die_surface(m);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_ph_inline(tree_t * n) {
    if (!n) return;
    for (int i = 0; i < n->n; i++) { const char * p = rk_ph_of(n->c[i]); if (p) { if (rk_ph_is_loop(p)) rk_ph_unplaced(p); n->c[i] = rk_ph_body(n->c[i]); } rk_ph_inline(n->c[i]); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_ph_place(tree_t * seq, int from, int mainline) {
    int any = 0, n = seq->n, nin = 0;
    tree_t * tail_ret = NULL;
    for (int i = from; i < n; i++) if (rk_ph_of(seq->c[i])) { any = 1; break; }
    if (!any) return;
    tree_t ** in = (tree_t **)ct_alloc(sizeof(tree_t *) * (size_t)(n - from + 1));
    for (int i = from; i < n; i++) in[nin++] = seq->c[i];
    if (!mainline && nin > 0 && in[nin - 1] && in[nin - 1]->t == TT_RETURN) tail_ret = in[--nin];
    seq->n = from;
    for (int rank = 0; rank <= 7; rank++) for (int i = 0; i < nin; i++) {
        tree_t * it = in[i];
        const char * p = rk_ph_of(it);
        int r = p ? rk_ph_rank(p, mainline) : 4;
        if (r == -1) continue;
        if (r == -2 && !mainline) { if (rank == 4) ast_push(seq, it); continue; }
        if (r == -2) r = 4;
        if (r != rank) continue;
        ast_push(seq, p ? rk_ph_body(it) : it);
    }
    if (tail_ret) ast_push(seq, tail_ret);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_ph_join(tree_t * a, tree_t * c) { if (!a) return c; tree_t * s = ast_node_new(TT_SEQ_EXPR); ast_push(s, a); ast_push(s, c); return s; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_ph_store(const char * g, int v) {
    tree_t * a = ast_node_new(TT_ASSIGN);
    ast_push(a, leaf_sval2(TT_VAR, g));
    tree_t * z = ast_node_new(TT_ILIT);
    z->v.ival = v;
    ast_push(a, z);
    return a;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_ph_loop(tree_t * loop, tree_t * body, int * uid) {
    tree_t * first = NULL, * nxt = NULL, * last = NULL;
    int keep = 0;
    for (int i = 0; i < body->n; i++) {
        const char * p = rk_ph_of(body->c[i]);
        if (p && (!strcmp(p, "FIRST") || !strcmp(p, "once"))) first = rk_ph_join(first, rk_ph_body(body->c[i]));
        else if (p && !strcmp(p, "NEXT")) nxt = rk_ph_join(nxt, rk_ph_body(body->c[i]));
        else if (p && !strcmp(p, "LAST")) last = rk_ph_join(last, rk_ph_body(body->c[i]));
        else body->c[keep++] = body->c[i];
    }
    if (keep == body->n) return loop;
    char gb[48];
    snprintf(gb, sizeof gb, "__rk_ph_g%d", (*uid)++);
    const char * g = intern(gb);
    tree_t * inner = ast_node_new(TT_SEQ_EXPR);
    if (first) {
        tree_t * t = ast_node_new(TT_SEQ_EXPR);
        ast_push(t, rk_ph_store(g, 0));
        ast_push(t, first);
        tree_t * iff = ast_node_new(TT_IF);
        ast_push(iff, leaf_sval2(TT_VAR, g));
        ast_push(iff, t);
        ast_push(inner, iff);
    } else if (last) ast_push(inner, rk_ph_store(g, 0));
    for (int i = 0; i < keep; i++) ast_push(inner, body->c[i]);
    if (nxt) ast_push(inner, nxt);
    body->n = 0;
    for (int i = 0; i < inner->n; i++) ast_push(body, inner->c[i]);
    if (!first && !last) return loop;
    tree_t * outer = ast_node_new(TT_SEQ_EXPR);
    ast_push(outer, rk_ph_store(g, 1));
    ast_push(outer, loop);
    if (last) { tree_t * u = ast_node_new(TT_UNLESS); ast_push(u, leaf_sval2(TT_VAR, g)); tree_t * s = ast_node_new(TT_SEQ_EXPR); ast_push(s, last); ast_push(u, s); ast_push(outer, u); }
    return outer;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_ph_walk(tree_t * n, int in_class, int * uid) {
    if (!n) return n;
    int cls = n->t == TT_CLASS_DECL || n->t == TT_ROLE_DECL || n->t == TT_GRAMMAR_DECL || n->t == TT_MODULE_DECL;
    for (int i = 0; i < n->n; i++) n->c[i] = rk_ph_walk(n->c[i], cls, uid);
    if (n->t == TT_SEQ_EXPR) rk_ph_place(n, 0, 0);
    else if (n->t == TT_SUB_DECL) {
        int bs = (int) n->v.ival + (in_class ? 0 : 1);
        if (bs < 1) bs = 1;
        if (bs > n->n) bs = n->n;
        rk_ph_place(n, bs, 0);
    } else if (n->t == TT_WHILE || n->t == TT_UNTIL || n->t == TT_REPEAT || n->t == TT_CLOOP || n->t == TT_EVERY || n->t == TT_FOR_RANGE || n->t == TT_DO_WHILE) {
        tree_t * body = NULL;
        for (int i = n->n - 1; i >= 0; i--) if (n->c[i] && n->c[i]->t == TT_SEQ_EXPR) { body = n->c[i]; break; }
        if (body) return rk_ph_loop(n, body, uid);
    }
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_ph_subj_attr(tree_t * st) {
    if (!st || st->t != TT_STMT) return NULL;
    for (int i = 0; i < st->n; i++) { tree_t * a = st->c[i]; if (a && a->t == TT_ATTR && a->v.sval && !strcmp(a->v.sval, ":subj") && a->n == 1) return a; }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_place_phasers(tree_t * prog) {
    int uid = 0, any = 0;
    if (!prog) return;
    rk_ph_walk(prog, 0, &uid);
    for (int i = 0; i < prog->n; i++) { tree_t * a = rk_ph_subj_attr(prog->c[i]); if (a && rk_ph_of(a->c[0])) { any = 1; break; } }
    if (any) {
        int n = prog->n;
        tree_t ** in = (tree_t **)ct_alloc(sizeof(tree_t *) * (size_t)(n + 1));
        tree_t ** bd = (tree_t **)ct_alloc(sizeof(tree_t *) * (size_t)(n + 1));
        int * rk = (int *)ct_alloc(sizeof(int) * (size_t)(n + 1));
        for (int i = 0; i < n; i++) {
            in[i] = prog->c[i];
            tree_t * a = rk_ph_subj_attr(in[i]);
            tree_t * e = a ? a->c[0] : NULL;
            const char * p = rk_ph_of(e);
            rk[i] = p ? rk_ph_rank(p, 1) : 4;
            if (rk[i] == -2) rk[i] = 4;
            bd[i] = p ? rk_ph_body(e) : NULL;
        }
        prog->n = 0;
        for (int rank = 0; rank <= 7; rank++) for (int i = 0; i < n; i++) { if (rk[i] != rank) continue; if (bd[i]) rk_ph_subj_attr(in[i])->c[0] = bd[i]; ast_push(prog, in[i]); }
    }
    for (int i = 0; i < prog->n; i++) { tree_t * a = rk_ph_subj_attr(prog->c[i]); if (a) rk_ph_inline(a->c[0]); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_take_rewrite(tree_t * t, const char * gname) {
    if (!t || t->t == TT_SUB_DECL) return;
    for (int i = 0; i < t->n; i++) rk_take_rewrite(t->c[i], gname);
    if (t->t == TT_SUSPEND && t->n == 1 && t->c[0]) {
        tree_t * x = t->c[0];
        int line = t->line;
        t->t = TT_METHCALL;
        t->v.sval = NULL;
        t->n = 0;
        t->line = line;
        tree_t * rv = leaf_sval2(TT_VAR, gname);
        rv->slen |= 4;
        (void) rv->slen;
        ast_push(t, leaf_sval2(TT_VAR, gname));
        ast_push(t, leaf_sval2(TT_QLIT, "push"));
        ast_push(t, x);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_desugar_gather(tree_t * t, int * seq) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) {
        if (t->t == TT_ITERATE && i == 0 && t->c[0]) {
            const tree_t * src = t->c[0];
            if ((src->t == TT_MAP || src->t == TT_GREP) && src->n > 1) src = src->c[1];
            if (src && src->t == TT_GATHER) {
                int tkc = rk_tree_size(src) + 1;
                const tree_t ** tk = (const tree_t **) ct_alloc(sizeof(const tree_t *) * (size_t) tkc);
                if (rk_take_list(src, tk, tkc) > 0) continue;
            }
        }
        rk_desugar_gather(t->c[i], seq);
    }
    if (t->t == TT_GATHER && t->n >= 1 && t->c[0]) {
        char nm[48];
        snprintf(nm, sizeof nm, "@__gather%d", (*seq)++);
        const char * gname = intern(nm);
        tree_t * body = t->c[0];
        int line = t->line;
        rk_take_rewrite(body, gname);
        t->t = TT_SEQ_EXPR;
        t->n = 0;
        t->line = line;
        tree_t * dv = leaf_sval2(TT_VAR, gname);
        dv->slen |= 4;
        tree_t * un = ast_node_new(TT_FNC);
        un->v.sval = (char *) "__rk_undef";
        ast_push(un, leaf_sval2(TT_VAR, "__rk_undef"));
        tree_t * as = ast_node_new(TT_ASSIGN);
        as->line = line;
        ast_push(as, dv);
        ast_push(as, un);
        tree_t * res = ast_node_new(TT_FNC);
        res->v.sval = (char *) "__rk_arr_values";
        ast_push(res, leaf_sval2(TT_VAR, "__rk_arr_values"));
        ast_push(res, leaf_sval2(TT_VAR, gname));
        ast_push(t, as);
        ast_push(t, body);
        ast_push(t, res);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_tv(const char * nm) { return leaf_sval2(TT_VAR, nm); }
static tree_t * rk_ti(long long v) { tree_t * n = ast_node_new(TT_ILIT); n->v.ival = v; return n; }
static tree_t * rk_t2(tree_e k, tree_t * a, tree_t * b) { tree_t * n = ast_node_new(k); ast_push(n, a); ast_push(n, b); return n; }
static tree_t * rk_tset(const char * v, tree_t * e) { return rk_t2(TT_ASSIGN, rk_tv(v), e); }
static tree_t * rk_tinc(const char * v, tree_t * by) { return rk_tset(v, rk_t2(TT_ADD, rk_tv(v), by)); }
static tree_t * rk_tsq(tree_t * a, tree_t * b, tree_t * c) { tree_t * s = ast_node_new(TT_SEQ_EXPR); if (a) ast_push(s, a); if (b) ast_push(s, b); if (c) ast_push(s, c); return s; }
static tree_t * rk_tadd(tree_t * s, tree_t * x) { ast_push(s, x); return s; }
static tree_t * rk_tarr(const char * a, const char * i) { return rk_t2(TT_ARR_GET, rk_tv(a), rk_tv(i)); }
static tree_t * rk_tput(const char * a, const char * i, tree_t * v) { tree_t * n = ast_node_new(TT_ARR_SET); ast_push(n, rk_tv(a)); ast_push(n, rk_tv(i)); ast_push(n, v); return n; }
static tree_t * rk_tclamp(const char * v, const char * lim) {
    tree_t * f = ast_node_new(TT_IF);
    ast_push(f, rk_t2(TT_GT, rk_tv(v), rk_tv(lim)));
    ast_push(f, rk_tsq(rk_tset(v, rk_tv(lim)), NULL, NULL));
    return f;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_sort_helper(const char * hname, const char * cmp) {
    tree_t * sd = ast_node_new(TT_SUB_DECL);
    sd->v.ival = 1;
    ast_push(sd, rk_tv(hname));
    tree_t * ps = rk_tv("@src");
    ast_push(ps, leaf_sval2(TT_QLIT, "@"));
    ast_push(sd, ps);
    ast_push(sd, rk_tset("@a", rk_tv("@src")));
    { tree_t * m = ast_node_new(TT_METHCALL); ast_push(m, rk_tv("@a")); ast_push(m, leaf_sval2(TT_QLIT, "elems")); ast_push(sd, rk_tset("n", m)); }
    ast_push(sd, rk_tset("@b", rk_fnc2("__rk_undef", NULL, NULL)));
    ast_push(sd, rk_tset("w", rk_ti(1)));
    tree_t * lt0 = rk_t2(TT_LT, rk_fnc2(cmp, rk_tarr("@a", "r"), rk_tarr("@a", "l")), rk_ti(0));
    tree_t * ifc = ast_node_new(TT_IF);
    ast_push(ifc, lt0);
    ast_push(ifc, rk_tsq(rk_tput("@b", "k", rk_tarr("@a", "r")), rk_tinc("r", rk_ti(1)), NULL));
    ast_push(ifc, rk_tsq(rk_tput("@b", "k", rk_tarr("@a", "l")), rk_tinc("l", rk_ti(1)), NULL));
    tree_t * both = ast_node_new(TT_SEQ);
    both->v.ival = 1;
    ast_push(both, rk_t2(TT_LT, rk_tv("l"), rk_tv("mid")));
    ast_push(both, rk_t2(TT_LT, rk_tv("r"), rk_tv("hi")));
    tree_t * w1 = rk_t2(TT_WHILE, both, rk_tsq(ifc, rk_tinc("k", rk_ti(1)), NULL));
    tree_t * w2 = rk_t2(TT_WHILE, rk_t2(TT_LT, rk_tv("l"), rk_tv("mid")), rk_tsq(rk_tput("@b", "k", rk_tarr("@a", "l")), rk_tinc("l", rk_ti(1)), rk_tinc("k", rk_ti(1))));
    tree_t * w3 = rk_t2(TT_WHILE, rk_t2(TT_LT, rk_tv("r"), rk_tv("hi")), rk_tsq(rk_tput("@b", "k", rk_tarr("@a", "r")), rk_tinc("r", rk_ti(1)), rk_tinc("k", rk_ti(1))));
    tree_t * ib = rk_tsq(rk_tset("mid", rk_t2(TT_ADD, rk_tv("i"), rk_tv("w"))), rk_tclamp("mid", "n"), rk_tset("hi", rk_t2(TT_ADD, rk_tv("i"), rk_t2(TT_MUL, rk_ti(2), rk_tv("w")))));
    rk_tadd(ib, rk_tclamp("hi", "n"));
    rk_tadd(ib, rk_tset("l", rk_tv("i")));
    rk_tadd(ib, rk_tset("r", rk_tv("mid")));
    rk_tadd(ib, rk_tset("k", rk_tv("i")));
    rk_tadd(ib, w1);
    rk_tadd(ib, w2);
    rk_tadd(ib, w3);
    rk_tadd(ib, rk_tinc("i", rk_t2(TT_MUL, rk_ti(2), rk_tv("w"))));
    tree_t * inner = rk_t2(TT_WHILE, rk_t2(TT_LT, rk_tv("i"), rk_tv("n")), ib);
    tree_t * ob = rk_tsq(rk_tset("i", rk_ti(0)), inner, rk_tset("@a", rk_tv("@b")));
    rk_tadd(ob, rk_tset("w", rk_t2(TT_MUL, rk_tv("w"), rk_ti(2))));
    ast_push(sd, rk_t2(TT_WHILE, rk_t2(TT_LT, rk_tv("w"), rk_tv("n")), ob));
    { tree_t * r = ast_node_new(TT_RETURN); tree_t * m = ast_node_new(TT_METHCALL); ast_push(m, rk_tv("@a")); ast_push(m, leaf_sval2(TT_QLIT, "list")); ast_push(r, m); ast_push(sd, r); }
    return sd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * rk_block_decl(const tree_t * prog, const char * nm) {
    for (int i = 0; prog && i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) { const tree_t * sub = stmt_subj(d); if (sub) d = sub; }
        if (d && d->t == TT_SUB_DECL && d->n > 0 && d->c[0] && d->c[0]->v.sval && !strcmp(d->c[0]->v.sval, nm)) return d;
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_sort_cmp_walk(tree_t * prog, tree_t * t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_sort_cmp_walk(prog, t->c[i]);
    if (t->t != TT_METHCALL || t->n != 3 || !t->c[1] || !t->c[1]->v.sval || strcmp(t->c[1]->v.sval, "sort") || !t->c[2] || t->c[2]->t != TT_ANON_BLOCK || !t->c[2]->v.sval || !t->c[0]) return;
    if (t->c[0]->t == TT_VAR && t->c[0]->v.sval && t->c[0]->v.sval[0] == '%') return;
    const tree_t * bd = rk_block_decl(prog, t->c[2]->v.sval);
    if (!bd || bd->v.ival != 2) return;
    char hn[strlen(t->c[2]->v.sval) + 16];
    snprintf(hn, sizeof hn, "__rk_sortc_%s", t->c[2]->v.sval);
    const char * hname = lp_strdup(hn);
    if (!rk_block_decl(prog, hname)) ast_push(prog, rk_sort_helper(hname, t->c[2]->v.sval));
    tree_t * recv = t->c[0];
    t->t = TT_FNC;
    t->v.sval = (char *) hname;
    t->n = 0;
    ast_push(t, rk_tv(hname));
    ast_push(t, recv);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_alpha_name(char * out, size_t cap, const char * base, int n) {
    char sfx[16];
    int k = 0;
    for (; n > 0 && k < 14; n /= 26) sfx[k++] = (char) ('a' + (n - 1) % 26), n -= 1;
    sfx[k] = '\0';
    for (int i = 0, j = k - 1; i < j; i++, j--) { char c = sfx[i]; sfx[i] = sfx[j]; sfx[j] = c; }
    snprintf(out, cap, "%sZ%s", base, sfx);
}
static int rk_is_type_decl(const tree_t * d) { return d && (d->t == TT_CLASS_DECL || d->t == TT_ROLE_DECL); }
static const char * rk_decl_name(const tree_t * d) { return (d->n > 0 && d->c[0] && d->c[0]->v.sval) ? d->c[0]->v.sval : NULL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_count_type_decls(const tree_t * t) { if (!t) return 0; int n = rk_is_type_decl(t) ? 1 : 0; for (int i = 0; i < t->n; i++) n += rk_count_type_decls(t->c[i]); return n; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_rename_type_refs(tree_t * t, const char * from, const char * to) {
    if (!t) return;
    if (t->t == TT_VAR && t->v.sval && !strcmp(t->v.sval, from)) t->v.sval = (char *) to;
    if (t->t == TT_NEW && t->n > 0 && t->c[0] && t->c[0]->t == TT_QLIT && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, from)) t->c[0]->v.sval = (char *) to;
    for (int i = 0; i < t->n; i++) rk_rename_type_refs(t->c[i], from, to);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { const tree_t ** top; int ntop; const char ** seen; int nseen; tree_t ** out; int nout; int uid; } rk_hoist_t;
static int rk_is_top_decl(rk_hoist_t * h, const tree_t * d) { for (int i = 0; i < h->ntop; i++) if (h->top[i] == d) return 1; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_hoist_types_walk(rk_hoist_t * h, tree_t * t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) {
        tree_t * c = t->c[i];
        if (rk_is_type_decl(c) && !rk_is_top_decl(h, c)) {
            const char * nm = rk_decl_name(c);
            int stmt_pos = t->t == TT_ATTR;
            char buf[64];
            if (!nm || !*nm) {
                rk_alpha_name(buf, sizeof buf, "AnonClass", ++h->uid);
                nm = lp_strdup(buf);
            } else {
                int dup = 0;
                for (int k = 0; k < h->nseen; k++) if (!strcmp(h->seen[k], nm)) dup = 1;
                if (dup || rk_is_class_name(nm)) { rk_alpha_name(buf, sizeof buf, nm, ++h->uid); const char * un = lp_strdup(buf); rk_rename_type_refs(t, nm, un); nm = un; }
            }
            if (c->n > 0 && c->c[0]) c->c[0]->v.sval = (char *) nm;
            h->seen[h->nseen++] = nm;
            h->out[h->nout++] = c;
            if (stmt_pos) t->c[i] = ast_node_new(TT_SEQ_EXPR);
            else { tree_t * v = ast_node_new(TT_VAR); v->v.sval = (char *) nm; t->c[i] = v; }
            rk_hoist_types_walk(h, c);
            continue;
        }
        rk_hoist_types_walk(h, c);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_hoist_nested_types(tree_t * prog) {
    if (!prog) return;
    int cap = rk_count_type_decls(prog) + 1;
    const tree_t * top[cap];
    const char * seen[cap];
    tree_t * out[cap];
    rk_hoist_t h = { top, 0, seen, 0, out, 0, 0 };
    for (int i = 0; i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) { const tree_t * sub = stmt_subj(d); if (sub) d = sub; }
        if (rk_is_type_decl(d)) { top[h.ntop++] = d; const char * nm = rk_decl_name(d); if (nm && *nm) seen[h.nseen++] = nm; }
    }
    rk_hoist_types_walk(&h, prog);
    for (int i = 0; i < h.nout; i++) ast_push(prog, out[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { const tree_t ** top; int ntop; tree_t ** out; int nout; } rk_mhoist_t;
static int rk_is_multi_decl(const tree_t * c) { return c && c->t == TT_SUB_DECL && c->n > 0 && c->c[0] && c->c[0]->v.sval && strchr(c->c[0]->v.sval, '$'); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_multi_captures(const tree_t * c, const tree_t * encl) {
    if (!encl) return 0;
    rk_ns_t used = { 0 }, own = { 0 }, outer = { 0 };
    for (int k = 1; k < c->n; k++) rk_cap_uses_deep(c->c[k], &used);
    rk_cap_declared_deep(c, &own);
    rk_cap_declared_deep(encl, &outer);
    for (int k = 0; k < used.n; k++) if (!rk_ns_has(&own, used.v[k]) && rk_ns_has(&outer, used.v[k])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_hoist_multis_walk(rk_mhoist_t * h, tree_t * t, const tree_t * encl) {
    if (!t || t->t == TT_CLASS_DECL || t->t == TT_ROLE_DECL || t->t == TT_GRAMMAR_DECL) return;
    if (t->t == TT_SUB_DECL) encl = t;
    for (int i = 0; i < t->n; i++) {
        tree_t * c = t->c[i];
        if (rk_is_multi_decl(c)) {
            int top = 0;
            for (int k = 0; k < h->ntop; k++) if (h->top[k] == c) top = 1;
            if (!top && !rk_multi_captures(c, encl)) { h->out[h->nout++] = c; t->c[i] = ast_node_new(TT_SEQ_EXPR); rk_hoist_multis_walk(h, c, NULL); continue; }
        }
        rk_hoist_multis_walk(h, c, encl);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_count_multi_decls(const tree_t * t) { if (!t) return 0; int n = rk_is_multi_decl(t) ? 1 : 0; for (int i = 0; i < t->n; i++) n += rk_count_multi_decls(t->c[i]); return n; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_hoist_nested_multis(tree_t * prog) {
    if (!prog) return;
    int cap = rk_count_multi_decls(prog) + 1;
    const tree_t * top[cap];
    tree_t * out[cap];
    rk_mhoist_t h = { top, 0, out, 0 };
    for (int i = 0; i < prog->n; i++) { const tree_t * d = prog->c[i]; if (d && d->t == TT_STMT) { const tree_t * sub = stmt_subj(d); if (sub) d = sub; } if (rk_is_multi_decl(d)) top[h.ntop++] = d; }
    rk_hoist_multis_walk(&h, prog, NULL);
    for (int i = 0; i < h.nout; i++) ast_push(prog, out[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_clone_tree(const tree_t * t) {
    if (!t) return NULL;
    tree_t * c = ast_node_new(t->t);
    c->v = t->v;
    c->line = t->line;
    c->slen = t->slen;
    for (int i = 0; i < t->n; i++) ast_push(c, rk_clone_tree(t->c[i]));
    return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_class_default_tweaks(tree_t * prog) {
    for (int i = 0; i < prog->n; i++) {
        tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) { const tree_t * sub = stmt_subj(d); d = (tree_t *) sub; }
        if (!d || d->t != TT_CLASS_DECL) continue;
        tree_t * seq = ast_node_new(TT_SEQ_EXPR);
        for (int j = 1; j < d->n; j++) {
            const tree_t * ch = d->c[j];
            if (!ch || !ch->v.sval || ch->n < 1 || !ch->c[0]) continue;
            int sig = ch->t == TT_ARR_DECL ? '@' : ch->t == TT_HASH_DECL ? '%' : (ch->t == TT_HAS_DECL && ch->c[0]->t != TT_ILIT && ch->c[0]->t != TT_QLIT && ch->c[0]->t != TT_FLIT) ? '$' : 0;
            if (!sig) continue;
            const char * fn = rk_fld_bare(ch->v.sval);
            if (!*fn) continue;
            tree_t * rhs = rk_clone_tree(ch->c[0]);
            if (sig != '$') {
                tree_t * w = ast_node_new(TT_FNC);
                w->v.sval = (char *) (sig == '@' ? "__rk_to_array" : "__rk_to_hash");
                ast_push(w, leaf_sval2(TT_VAR, w->v.sval));
                ast_push(w, rhs);
                rhs = w;
            }
            tree_t * f1 = ast_node_new(TT_TWIGIL_FIELD), * f2 = ast_node_new(TT_TWIGIL_FIELD);
            f1->v.sval = f2->v.sval = (char *) fn;
            tree_t * cond;
            if (sig == '$') {
                cond = ast_node_new(TT_METHCALL);
                ast_push(cond, f1);
                ast_push(cond, leaf_sval2(TT_QLIT, "defined"));
            } else {
                cond = ast_node_new(TT_FNC);
                cond->v.sval = (char *) "__rk_unset";
                ast_push(cond, leaf_sval2(TT_VAR, "__rk_unset"));
                ast_push(cond, f1);
            }
            tree_t * asg = ast_node_new(TT_ASSIGN);
            ast_push(asg, f2);
            ast_push(asg, rhs);
            tree_t * body = ast_node_new(TT_SEQ_EXPR);
            ast_push(body, asg);
            tree_t * br = ast_node_new(sig == '$' ? TT_UNLESS : TT_IF);
            ast_push(br, cond);
            ast_push(br, body);
            ast_push(seq, br);
        }
        if (seq->n == 0) continue;
        tree_t * tw = NULL;
        for (int j = 1; j < d->n && !tw; j++) { tree_t * ch = d->c[j]; if (ch && ch->t == TT_SUB_DECL && ch->n > 0 && ch->c[0] && ch->c[0]->v.sval && !strcmp(ch->c[0]->v.sval, "TWEAK")) tw = ch; }
        if (!tw) { tw = ast_node_new(TT_SUB_DECL); tw->v.ival = 1; ast_push(tw, leaf_sval2(TT_VAR, "TWEAK")); for (int k = 0; k < seq->n; k++) ast_push(tw, seq->c[k]); ast_push(d, tw); continue; }
        int at = (int) tw->v.ival;
        if (at < 1) at = 1;
        if (at > tw->n) at = tw->n;
        int nold = tw->n - at;
        tree_t ** old = (tree_t **) ct_alloc(sizeof(tree_t *) * (size_t) (nold + 1));
        for (int k = 0; k < nold; k++) old[k] = tw->c[at + k];
        tw->n = at;
        for (int k = 0; k < seq->n; k++) ast_push(tw, seq->c[k]);
        for (int k = 0; k < nold; k++) ast_push(tw, old[k]);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_nested_elem_sets(tree_t * t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_nested_elem_sets(t->c[i]);
    while ((t->t == TT_ARR_SET || t->t == TT_HASH_SET) && t->n == 3 && t->c[0] && (t->c[0]->t == TT_ARR_GET || t->c[0]->t == TT_HASH_GET) && t->c[0]->n >= 2) {
        tree_t * g = t->c[0];
        tree_t * put = ast_node_new(TT_FNC);
        put->v.sval = (char *) "__rk_elem_put";
        ast_push(put, leaf_sval2(TT_VAR, "__rk_elem_put"));
        ast_push(put, leaf_sval2(TT_QLIT, t->t == TT_ARR_SET ? "a" : "h"));
        ast_push(put, rk_clone_tree(g));
        ast_push(put, t->c[1]);
        ast_push(put, t->c[2]);
        t->t = g->t == TT_ARR_GET ? TT_ARR_SET : TT_HASH_SET;
        t->c[0] = g->c[0];
        t->c[1] = g->c[1];
        t->c[2] = put;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_assign_pow_rhs(tree_t * t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_assign_pow_rhs(t->c[i]);
    if (t->t == TT_ASSIGN && t->n == 2 && t->c[1] && t->c[1]->t == TT_POW) {
        tree_t * w = ast_node_new(TT_FNC);
        w->line = t->line;
        w->v.sval = (char *) "__rk_item1";
        ast_push(w, leaf_sval2(TT_VAR, "__rk_item1"));
        ast_push(w, t->c[1]);
        t->c[1] = w;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static stage2_t *rk_stage2_core(const tree_t *prog, int reset_multi, int want_main) {
    rk_hoist_nested_types((tree_t *) prog);
    rk_hoist_nested_multis((tree_t *) prog);
    rk_class_default_tweaks((tree_t *) prog);
    rk_tail_ifs((tree_t *) prog);
    rk_assign_pow_rhs((tree_t *) prog);
    rk_nested_elem_sets((tree_t *) prog);
    rk_place_phasers((tree_t *) prog);
    rk_rename_user_main((tree_t *) prog);
    { int gseq = 0; rk_desugar_gather((tree_t *) prog, &gseq); }
    rk_listops_to_methcalls((tree_t *) prog, 0);
    rk_sub_placeholders((tree_t *) prog);
    rk_ph_alias((tree_t *) prog);
    rk_cap_file_scope((tree_t *) prog);
    rk_globalize_file_scope_writes((tree_t *) prog);
    rk_hoist_anon_blocks((tree_t *) prog);
    rk_sort_cmp_walk((tree_t *) prog, (tree_t *) prog);
    raku_register_program(&g_stage2, prog);
    rk_discover_grammars(prog);
    rk_lower_grammar_boxes(prog);
    rk_register_classes(prog);
    if (reset_multi) g_rk_multi_names.len = 0;
    for (int i = 0; prog && i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) { const tree_t * sub = stmt_subj(d); if (!sub) continue; d = sub; }
        if (!d || d->t != TT_SUB_DECL) continue;
        const char * nm = (d->n > 0 && d->c[0] && d->c[0]->v.sval) ? d->c[0]->v.sval : NULL;
        if (!nm) continue;
        const char * soh = strchr(nm, '$');
        if (!soh) continue;
        int bl = (int)(soh - nm);
        char base[bl + 1];
        memcpy(base, nm, bl);
        base[bl] = 0;
        rk_multi_name_add(base);
    }
    for (int i = 0; prog && i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) { const tree_t * sub = stmt_subj(d); if (!sub) continue; d = sub; }
        if (!d || d->t != TT_CLASS_DECL) continue;
        const char * cn = (d->n > 0 && d->c[0] && d->c[0]->v.sval) ? d->c[0]->v.sval : NULL;
        if (!cn) continue;
        for (int j = 0; j < d->n; j++) {
            const tree_t * m = d->c[j];
            if (!m || m->t != TT_SUB_DECL) continue;
            const char * mn = (m->n > 0 && m->c[0] && m->c[0]->v.sval) ? m->c[0]->v.sval : NULL;
            if (!mn) continue;
            const char * ms = strchr(mn, '$');
            if (!ms) continue;
            int ml = (int)(ms - mn);
            char mb[fmt_len("%s__%.*s", cn, ml, mn)];
            snprintf(mb, sizeof mb, "%s__%.*s", cn, ml, mn);
            rk_multi_name_add(mb);
        }
    }
    rk_discover_procs(prog);
    for (int pi = 0; pi < g_stage2.proc_count; pi++) {
        const tree_t *proc = (const tree_t *) g_stage2.proc_table[pi].proc;
        if (!proc || proc->t != TT_SUB_DECL) continue;
        if (g_stage2.proc_table[pi].bb_idx >= 0) continue;
        int bb_idx = lower_raku_body(prog, proc);
        if (bb_idx >= 0) {
            g_stage2.proc_table[pi].bb_idx = bb_idx;
            const char * pname = g_stage2.proc_table[pi].name;
            const char * _mu = pname ? strchr(pname, '_') : NULL;
            int is_method = (_mu && _mu != pname && _mu[1] == '_');
            int param_start = is_method ? 0 : 1;
            int np = g_stage2.proc_table[pi].nparams;
            Scope *sc = &g_stage2.proc_table[pi].lower_sc;
            sc->n = 0;
            if (is_method) { stage2_scope_reserve(sc, sc->n + 1); sc->e[sc->n].name = lp_strdup("self"); sc->e[sc->n].slot = sc->n; sc->n++; param_start = 1; }
            for (int k = 0; k < np && (k + param_start) < proc->n; k++) {
                stage2_scope_reserve(sc, sc->n + 1);
                const tree_t *pv = proc->c[k + param_start];
                if (!pv || pv->t != TT_VAR || !pv->v.sval) continue;
                sc->e[sc->n].name = lp_strdup(pv->v.sval);
                sc->e[sc->n].slot = sc->n;
                sc->n++;
            }
            g_stage2.bbp.table[bb_idx]->nparams = sc->n;
            g_stage2.bbp.table[bb_idx]->entry_frame = 1;
            g_stage2.bbp.table[bb_idx]->smx = 1;
            g_stage2.proc_table[pi].lex_startup = 1;
            if (sc->n > 0) {
                const char ** _pn = (const char **) ct_zalloc((size_t) sc->n, sizeof(const char *));
                if (_pn) { for (int k = 0; k < sc->n; k++) _pn[k] = sc->e[k].name; g_stage2.bbp.table[bb_idx]->pnames = _pn; }
            }
        }
    }
    int has_main = 0;
    for (int pi = 0; pi < g_stage2.proc_count; pi++) if (g_stage2.proc_table[pi].name && strcmp(g_stage2.proc_table[pi].name, "main") == 0) { has_main = 1; break; }
    if (!has_main && want_main) {
        IR_graph_t * tg = IR_alloc(8192);
        rcx_t tcx;
        tcx.try_depth = 0;
        tcx.g = tg;
        tcx.try_catch = NULL;
        tcx.loop_exit = NULL;
        tcx.loop_next = NULL;
        tcx.cur_proc = NULL;
        tcx.cur_byref_mask = 0;
        tcx.cur_nparams = 0;
        tcx.cur_proc_name = "main";
        IR_t * succ = IR_node_alloc(tg, IR_SUCCEED);
        IR_t * fail = IR_node_alloc(tg, IR_FAIL);
        IR_t * sentry = succ;
        IR_t * entry = succ;
        tcx.proc_exit = succ;
        int has_rk_MAIN = 0;
        for (int pi = 0; pi < g_stage2.proc_count; pi++) if (g_stage2.proc_table[pi].name && strcmp(g_stage2.proc_table[pi].name, "MAIN") == 0) { has_rk_MAIN = 1; break; }
        if (has_rk_MAIN) {
            tree_t * mc = ast_node_new(TT_FNC);
            mc->v.sval = (char *)"MAIN";
            tree_t * nmv = ast_node_new(TT_VAR);
            nmv->v.sval = (char *)"MAIN";
            ast_push(mc, nmv);
            IR_t * r = NULL;
            IR_t * e = lower_rv(&tcx, mc, sentry, sentry, &r);
            if (e) { entry = e; sentry = e; }
        }
        for (int i = prog->n - 1; i >= 0; i--) {
            const tree_t * s = prog->c[i];
            if (!s) continue;
            if (s->t == TT_STMT) { const tree_t * sub = stmt_subj(s); if (!sub) continue; s = sub; }
            if (s->t == TT_SUB_DECL || s->t == TT_CLASS_DECL || s->t == TT_ROLE_DECL || s->t == TT_GRAMMAR_DECL) continue;
            if (s->t == TT_MODULE_DECL) {
                for (int j = s->n - 1; j >= 1; j--) {
                    const tree_t * ch = s->c[j];
                    if (ch && ch->t == TT_STMT) { const tree_t * sub = stmt_subj(ch); if (!sub) continue; ch = sub; }
                    if (!ch || ch->t == TT_SUB_DECL) continue;
                    IR_t * r = NULL;
                    IR_t * e = lower_rv(&tcx, ch, sentry, sentry, &r);
                    if (e) e = trace_stmt_wrap(&tcx, trace_stmt_line(ch), e, sentry);
                    if (e) { entry = e; sentry = e; }
                }
                continue;
            }
            if ((s->t == TT_SEQ || s->t == TT_SEQ_EXPR || s->t == TT_PROGRAM) && s->n == 0) continue;
            IR_t * r = NULL;
            IR_t * e = lower_rv(&tcx, s, sentry, sentry, &r);
            if (e) e = trace_stmt_wrap(&tcx, trace_stmt_line(s), e, sentry);
            if (e) { entry = e; sentry = e; }
        }
        {
            tree_t * ta = ast_node_new(TT_ASSIGN);
            ta->v.ival = 1;
            ast_push(ta, leaf_sval2(TT_VAR, "_"));
            ast_push(ta, ast_node_new(TT_NUL));
            IR_t * tr = NULL;
            IR_t * te = lower_rv(&tcx, ta, sentry, sentry, &tr);
            if (te) { entry = te; sentry = te; }
        }
        tg->entry = trace_call_wrap(&tcx, "main", entry, fail);
        int bb_idx = bb_program_add(&g_stage2.bbp, tg);
        if (bb_idx >= 0) {
            int pi = stage2_proc_grow(&g_stage2);
            g_stage2.proc_table[pi].name = lp_strdup("main");
            g_stage2.proc_table[pi].proc = NULL;
            g_stage2.proc_table[pi].entry_pc = -1;
            g_stage2.proc_table[pi].bb_idx = bb_idx;
            g_stage2.proc_table[pi].nparams = 0;
        }
    }
    rk_reclassify_calls();
    rk_file_scope_reads_are_globals();
    rk_unassigned_dynamics_are_globals();
    rk_eval_main_globals();
    for (int pi = 0; pi < g_stage2.proc_count; pi++) {
        int bi = g_stage2.proc_table[pi].bb_idx;
        if (bi >= 0 && bi < g_stage2.bbp.count && g_stage2.bbp.table[bi]) {
            g_stage2.bbp.table[bi]->entry_frame = 1;
            g_stage2.bbp.table[bi]->smx = 1;
            {
                const tree_t * pr = (const tree_t *) g_stage2.proc_table[pi].proc;
                g_stage2.bbp.table[bi]->block_args = (pr && pr->t == TT_SUB_DECL && !g_stage2.proc_table[pi].is_variadic && !g_stage2.proc_table[pi].named_rest) ? 1 : 0;
            }
        }
    }
    return &g_stage2;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
stage2_t *lower_raku_stage2(const tree_t *prog) { return rk_stage2_core(prog, 1, 1); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * rk_eval_proc_name(int n) { char nb[48]; snprintf(nb, sizeof nb, "EVAL$%d", n); return lp_strdup(nb); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_eval_is_declaration(const tree_t * st) {
    const tree_t * u = st;
    if (u->t == TT_STMT) { const tree_t * sub = stmt_subj(u); if (sub) u = sub; }
    return u->t == TT_SUB_DECL || u->t == TT_CLASS_DECL || u->t == TT_ROLE_DECL || u->t == TT_GRAMMAR_DECL || u->t == TT_USE_DECL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * lower_raku_eval_stage2(const tree_t * prog) {
    int pc0 = g_stage2.proc_count;
    const char * nm = rk_eval_proc_name(pc0);
    tree_t * p2 = ast_node_new(TT_PROGRAM), * sd = ast_node_new(TT_SUB_DECL), * nv = ast_node_new(TT_VAR);
    nv->v.sval = (char *) nm;
    sd->v.ival = 0;
    ast_push(sd, nv);
    int lastb = -1;
    for (int i = prog->n - 1; i >= 0 && lastb < 0; i--) {
        tree_t * st = prog->c[i];
        if (!st) continue;
        const tree_t * u = st;
        if (u->t == TT_STMT) { const tree_t * sub = stmt_subj(u); if (sub) u = sub; }
        if (!rk_eval_is_declaration(st) && !(u->t == TT_SEQ_EXPR && u->n == 0)) lastb = i;
    }
    for (int i = 0; i < prog->n; i++) {
        tree_t * st = prog->c[i];
        if (!st) continue;
        const tree_t * u = st;
        if (u->t == TT_STMT) { const tree_t * sub = stmt_subj(u); if (sub) u = sub; }
        if (rk_eval_is_declaration(st)) ast_push(p2, st);
        else ast_push(sd, i == lastb ? rk_tail_return((tree_t *) u) : st);
    }
    ast_push(p2, sd);
    rk_stage2_core(p2, 0, 0);
    lower_raku_eval_reads_are_globals(pc0);
    return nm;
}
