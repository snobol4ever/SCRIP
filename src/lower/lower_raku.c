#include <string.h>
#include "g_runtime.h"
#include <unistd.h>
#include "ct_arena.h"
#include "ct_vec.h"
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "lower.h"
#include "g_lower.h"
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_seq_is_logical_and(const tree_t * t) { return t && t->t == TT_SEQ && t->n == 2 && t->v.ival == 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_is_multi_name(const char * nm) {
    if (!nm) return 0;
    for (uint32_t i = 0; i < g_lower.rk.multi_names.len; i++) if (!strcmp(CV_AT(g_lower.rk.multi_names, const char *, i), nm)) return 1;
    return 0;
}
static void rk_multi_name_add(const char * base) { if (!base || rk_is_multi_name(base)) return; { const char * nm = ct_strdup(base); CV_PUSH(g_lower.rk.multi_names, const char *) = nm; } }
static int rk_is_grammar_name(const char * nm) {
    if (!nm) return 0;
    for (uint32_t i = 0; i < g_lower.rk.gram_names.len; i++) if (!strcmp(CV_AT(g_lower.rk.gram_names, const char *, i), nm)) return 1;
    return 0;
}
static int rk_is_class_name(const char * nm) {
    if (!nm) return 0;
    for (uint32_t i = 0; i < g_lower.rk.class_names.len; i++) if (!strcmp(CV_AT(g_lower.rk.class_names, const char *, i), nm)) return 1;
    return 0;
}
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
#define RK_VF_RAW 512
#define RK_PV_COPY 1024
#define RK_PV_RW 2048
#define RK_PV_NAMED 4096
#define RK_PV_REQ 8192
#define RK_PV_OPT 16384
static int rk_is_scalar_bind(const tree_t * t);
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
static const char * const RK_LISTLIKE_METHNAMES[] = { "keys", "values", "kv", "sort", "reverse", "grep", "map", "split", "words", "comb", NULL };
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
static int rk_proc_is_generator(const char * name);
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
static void rk_relop_code(IR_t * op, tree_e tt) { IR_LIT(op).ival = lc_binop_code(tt); if (IR_LIT(op).ival >= BINOP_LT && IR_LIT(op).ival <= BINOP_NE) op->strict |= IR_STRICT_RELOP_NUMIFY_BLANK; }
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
static const char * const rk_shadowable_terms[] = { "now", "time", "rand", "Empty", "Less", "Same", "More", NULL };
static int rk_predeclared(const char * nm, int bare) {
    if (!nm || !*nm) return 0;
    static const char * const dyn[] = { "*DISTRO", "*KERNEL", "*VM", "*RAKU", "*PERL", "*PID", "*PROGRAM", "*PROGRAM-NAME", "*CWD", "*EXECUTABLE", "*EXECUTABLE-NAME", "*HOME", "*TMPDIR", "*USER",
        "%*ENV", "@*ARGS", "?FILE", "/", "!", "Order::Less", "Order::Same", "Order::More", "Bool::True", "Bool::False", NULL };
    for (int i = 0; dyn[i]; i++) if (!strcmp(nm, dyn[i])) return 1;
    if (!bare) return 0;
    for (int i = 0; rk_shadowable_terms[i]; i++) if (!strcmp(nm, rk_shadowable_terms[i]) && !rk_is_class_name(nm) && !rk_user_proc_exists(nm)) return 1;
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
    if (t->t == TT_METHCALL && t->n > 1 && t->c[1] && t->c[1]->v.sval) { int li = rk_listlike_idx(t->c[1]->v.sval); return li >= 0 && !g_lower.rk.listlike_overridden[li]; }
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { IR_t * entry; IR_t * tail; } rkrx_t;
typedef struct { int maxp; int nnamed; const char ** named; } rkrx_groups_t;
static long long rk_rx_fl(const tree_t * t) { return (t && t->n > 0 && t->c[0] && t->c[0]->t == TT_ILIT) ? t->c[0]->v.ival : 0; }
static void rk_rx_tag(IR_ref_t * r, const char * tg) { memcpy(r->sz, tg, 3); r->sz[3] = 0; }
static int rk_rx_tagged(const IR_ref_t * r, unsigned char b1) { return (unsigned char) r->sz[0] == 0xcf && (unsigned char) r->sz[1] == b1; }
static int rk_rx_atomic(const tree_t * a) {
    if (!a) return 0;
    if (a->t == TT_RX_ANY) return 1;
    if (a->t == TT_RX_LIT) return (rk_rx_fl(a) & 33) == 0 && a->slen > 0 && a->v.sval && (int) strlen(a->v.sval) == a->slen;
    if (a->t == TT_RX_CLASS) { if (a->v.ival & 1) return 0; for (int i = 0; i < a->n; i++) if (!a->c[i] || (a->c[i]->t != TT_RX_CRANGE && a->c[i]->t != TT_RX_CNAME)) return 0; return 1; }
    if (a->t == TT_RX_GROUP && a->n == 1) return rk_rx_atomic(a->c[0]);
    return 0;
}
static int rk_rx_has_cap(const tree_t * t) { if (!t) return 0; if (t->t == TT_RX_CAP) return 1; for (int i = 0; i < t->n; i++) if (rk_rx_has_cap(t->c[i])) return 1; return 0; }
static int rk_rx_core(const tree_t * t) {
    if (!t) return 0;
    switch (t->t) {
        case TT_RX_SEQ:
        case TT_RX_GROUP:
        case TT_RX_ALT:
        if ((t->t == TT_RX_SEQ && t->v.ival) || (t->t == TT_RX_ALT && t->v.ival == 2)) return 0;
        for (int i = 0; i < t->n; i++) if (!rk_rx_core(t->c[i])) return 0;
        return 1;
        case TT_RX_LIT:
        case TT_RX_ANY:
        case TT_RX_CLASS:
        return rk_rx_atomic(t);
        case TT_RX_QUANT:
        return t->n == 3 && rk_rx_atomic(t->c[0]) && t->c[1] && t->c[1]->t == TT_ILIT && t->c[2] && t->c[2]->t == TT_ILIT;
        case TT_RX_CAP:
        return t->n == 1 && !rk_rx_has_cap(t->c[0]) && rk_rx_core(t->c[0]);
        case TT_RX_ANCHOR:
        return t->v.ival == 0 || t->v.ival == 1;
        case TT_RX_NULL:
        return 1;
        default:
        return 0;
    }
}
static int rk_rx_count_caps(const tree_t * t) { int n = 0; if (!t) return 0; if (t->t == TT_RX_CAP) n = 1; for (int i = 0; i < t->n; i++) n += rk_rx_count_caps(t->c[i]); return n; }
static void rk_rx_scan_groups(const tree_t * t, rkrx_groups_t * gr) {
    if (!t) return;
    if (t->t == TT_RX_CAP) {
        if (t->slen > 0 && t->v.sval) {
            int seen = 0;
            for (int i = 0; i < gr->nnamed; i++) if (!strcmp(gr->named[i], t->v.sval)) seen = 1;
            if (!seen) gr->named[gr->nnamed++] = t->v.sval;
        } else if ((int) t->v.ival > gr->maxp) gr->maxp = (int) t->v.ival;
    }
    for (int i = 0; i < t->n; i++) rk_rx_scan_groups(t->c[i], gr);
}
static int rk_rx_slot(const tree_t * cap, const rkrx_groups_t * gr) {
    if (cap->slen <= 0) return (int) cap->v.ival;
    for (int i = 0; i < gr->nnamed; i++) if (!strcmp(gr->named[i], cap->v.sval)) return gr->maxp + 1 + i;
    return 0;
}
static char * rk_rx_blob(const tree_t * a, long mn, long mx, long flags) {
    while (a->t == TT_RX_GROUP && a->n == 1) a = a->c[0];
    {
        size_t cap = 80 + (a->t == TT_RX_LIT ? (size_t) a->slen : (size_t) a->n * 48);
        char * b = (char *) ct_alloc(cap);
        int k = snprintf(b, cap, "%c%ld,%ld,%ld;", a->t == TT_RX_LIT ? 'L' : a->t == TT_RX_ANY ? 'A' : 'C', mn, mx, flags);
        if (a->t == TT_RX_LIT) { memcpy(b + k, a->v.sval, (size_t) a->slen); b[k + a->slen] = 0; return b; }
        for (int i = 0; a->t == TT_RX_CLASS && i < a->n; i++) {
            const tree_t * it = a->c[i];
            if (it->t == TT_RX_CRANGE) k += snprintf(b + k, cap - (size_t) k, "%sr%lld,%lld;", it->v.ival ? "-" : "", it->c[0]->v.ival, it->c[1]->v.ival);
            else { long long f = rk_rx_fl(it); k += snprintf(b + k, cap - (size_t) k, "%s%c%s;", (f & 2) ? "-" : "", (f & 1) ? 'N' : 'n', it->v.sval); }
        }
        return b;
    }
}
static rkrx_t rk_rx_lower(rcx_t * cx, const tree_t * t, IR_t * succ, IR_t * fail, IR_t * land, const rkrx_groups_t * gr);
static void rk_rx_retag_alpha(IR_graph_t * g, int from, IR_t * target) {
    for (int k = from; k < g->n; k++) { IR_t * x = g->all[k]; if (x && x->γ.node == target && (unsigned char) x->γ.sz[0] == 0xce && (unsigned char) x->γ.sz[1] == 0xb2) lc_γ_to(x, target); }
}
static rkrx_t rk_rx_leaf(rcx_t * cx, IR_e op, IR_t * succ, IR_t * fail) { IR_t * nd = lc_build(cx->g, op, succ, NULL); lc_ω_to_β(nd, fail); return (rkrx_t) { nd, nd }; }
static rkrx_t rk_rx_seq(rcx_t * cx, const tree_t * t, IR_t * succ, IR_t * fail, IR_t * land, const rkrx_groups_t * gr) {
    IR_graph_t * g = cx->g;
    int ne = t->n;
    IR_t * S = lc_build(g, IR_GOTO, succ, NULL);
    IR_t * ent[ne > 0 ? ne : 1], * res[ne > 0 ? ne : 1];
    int lo[ne > 0 ? ne : 1];
    lc_ω_to_β(S, fail);
    for (int i = 0; i < ne; i++) {
        int before = g->n, rb = before;
        rkrx_t r = rk_rx_lower(cx, t->c[i], S, S, land, gr);
        IR_t * ti = NULL;
        while (rb < g->n && g->all[rb] && g->all[rb]->op == IR_GOTO && g->all[rb]->γ.node == S && g->all[rb]->ω.node == S && g->all[rb]->n_operands == 0) rb++;
        for (int k = before; k < g->n; k++) {
            IR_t * x = g->all[k];
            if (!x) continue;
            if (x->ω.node == S) rk_rx_tag(&x->ω, "φ");
            if (x->γ.node == S) { if (x->op == IR_GOTO && x->ω.node == S) rk_rx_tag(&x->γ, "φ"); else { rk_rx_tag(&x->γ, "σ"); ti = x; } }
        }
        ent[i] = r.entry;
        res[i] = ti ? ti : ((rb < g->n) ? g->all[rb] : r.entry);
        lo[i] = before;
    }
    for (int i = 0; i < ne; i++) {
        IR_t * nxt = (i + 1 < ne) ? ent[i + 1] : succ;
        IR_t * prv = (i > 0) ? res[i - 1] : fail;
        int hi = (i + 1 < ne) ? lo[i + 1] : g->n;
        for (int k = lo[i]; k < g->n && k < hi; k++) {
            IR_t * x = g->all[k];
            if (!x || x == S) continue;
            if (x->ω.node == S && rk_rx_tagged(&x->ω, 0x86)) { x->ω.node = prv; rk_rx_tag(&x->ω, "β"); }
            if (x->γ.node == S && rk_rx_tagged(&x->γ, 0x86)) { x->γ.node = prv; rk_rx_tag(&x->γ, "β"); }
            if (x->γ.node == S && rk_rx_tagged(&x->γ, 0x83)) { x->γ.node = nxt; x->γ.sz[0] = 0; }
        }
    }
    for (int k = 0; k < g->n; k++) {
        IR_t * x = g->all[k];
        if (!x || x == S) continue;
        if (x->ω.node == S && rk_rx_tagged(&x->ω, 0x86)) { x->ω.node = fail; rk_rx_tag(&x->ω, "β"); }
        if (x->γ.node == S && rk_rx_tagged(&x->γ, 0x86)) { x->γ.node = fail; rk_rx_tag(&x->γ, "β"); }
        if (x->γ.node == S && rk_rx_tagged(&x->γ, 0x83)) { x->γ.node = succ; x->γ.sz[0] = 0; }
    }
    S->γ.node = succ;
    S->γ.sz[0] = 0;
    return (rkrx_t) { ne > 0 ? ent[0] : succ, ne > 0 ? res[ne - 1] : succ };
}
static rkrx_t rk_rx_alt(rcx_t * cx, const tree_t * t, IR_t * succ, IR_t * fail, IR_t * land, const rkrx_groups_t * gr) {
    IR_graph_t * g = cx->g;
    IR_t * A = lc_build(g, IR_MATCH_ALTERNATE, succ, NULL);
    lc_ω_to_β(A, fail);
    for (int i = 0; i < t->n; i++) {
        int before = g->n;
        rkrx_t r = rk_rx_lower(cx, t->c[i], A, A, land, gr);
        IR_t * ri = (before < g->n) ? g->all[before] : r.entry, * ti = NULL;
        for (int k = before; k < g->n; k++) {
            IR_t * x = g->all[k];
            if (!x) continue;
            if (x->ω.node == A) rk_rx_tag(&x->ω, "φ");
            if (x->γ.node == A) { if (x->op == IR_GOTO && x->ω.node == A) rk_rx_tag(&x->γ, "φ"); else { rk_rx_tag(&x->γ, "σ"); ti = x; } }
        }
        if (ti) ri = ti;
        ir_operand_push(A, r.entry);
        ir_operand_push(A, ri);
    }
    IR_LIT(A).ival = (long) t->n;
    return (rkrx_t) { A, A };
}
static rkrx_t rk_rx_lower(rcx_t * cx, const tree_t * t, IR_t * succ, IR_t * fail, IR_t * land, const rkrx_groups_t * gr) {
    IR_graph_t * g = cx->g;
    switch (t->t) {
        case TT_RX_SEQ:
        return rk_rx_seq(cx, t, succ, fail, land, gr);
        case TT_RX_ALT:
        return rk_rx_alt(cx, t, succ, fail, land, gr);
        case TT_RX_GROUP:
        return t->n == 1 ? rk_rx_lower(cx, t->c[0], succ, fail, land, gr) : rk_rx_seq(cx, t, succ, fail, land, gr);
        case TT_RX_LIT:
        { rkrx_t r = rk_rx_leaf(cx, IR_MATCH_LIT, succ, fail); IR_LIT(r.entry).sval = t->v.sval; return r; }
        case TT_RX_ANY:
        case TT_RX_CLASS:
        { rkrx_t r = rk_rx_leaf(cx, IR_MATCH_RKRUN, succ, fail); IR_LIT(r.entry).sval = rk_rx_blob(t, 1, 1, 0); return r; }
        case TT_RX_QUANT:
        {
            long mn = (long) t->c[1]->v.ival, mx = (long) t->c[2]->v.ival;
            rkrx_t r = rk_rx_leaf(cx, IR_MATCH_RKRUN, succ, fail);
            IR_LIT(r.entry).sval = rk_rx_blob(t->c[0], mn, mx < 0 ? 65535 : mx, (long) (t->v.ival & 3));
            return r;
        }
        case TT_RX_ANCHOR:
        { rkrx_t r = rk_rx_leaf(cx, t->v.ival == 0 ? IR_MATCH_POS : IR_MATCH_RPOS, succ, fail); IR_LIT(r.entry).ival = 0; return r; }
        case TT_RX_CAP:
        {
            int slot = rk_rx_slot(t, gr);
            IR_t * cape = lc_build(g, IR_MATCH_RKCAPE, succ, NULL);
            IR_t * caps = lc_build(g, IR_MATCH_RKCAPS, NULL, NULL);
            rkrx_t body;
            ir_operand_push(cape, land);
            ir_operand_push(caps, land);
            IR_LIT(cape).ival = slot;
            IR_LIT(caps).ival = slot;
            lc_ω_to_β(caps, fail);
            body = rk_rx_lower(cx, t->c[0], cape, caps, land, gr);
            lc_γ_to(caps, body.entry);
            lc_ω_to_β(cape, body.tail);
            return (rkrx_t) { caps, cape };
        }
        case TT_RX_VAR:
        {
            IR_t * mv = lc_build(g, IR_MATCH_VALUE, succ, NULL), * vr = NULL, * ve;
            tree_t * vt = ast_node_new(TT_VAR);
            vt->v.sval = t->v.sval;
            lc_ω_to_β(mv, fail);
            { int before = g->n; ve = lower_rv(cx, vt, mv, fail, &vr); rk_rx_retag_alpha(g, before, mv); }
            if (vr) ir_operand_push(mv, vr);
            return (rkrx_t) { ve ? ve : mv, mv };
        }
        default:
        return rk_rx_leaf(cx, IR_GOTO, succ, fail);
    }
}
static IR_t * rk_lower_smatch_boxes(rcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_graph_t * g = cx->g;
    const tree_t * rx = t->c[1]->c[1];
    int ncap = rk_rx_count_caps(rx);
    const char * named[ncap > 0 ? ncap : 1];
    rkrx_groups_t gr = { -1, 0, named };
    IR_t * last, * clr, * head, * release, * land, * sv = NULL, * se;
    rkrx_t p;
    rk_rx_scan_groups(rx, &gr);
    {
        int n = gr.maxp + 1 + gr.nnamed;
        size_t cap = 24;
        char * names;
        for (int i = 0; i < gr.nnamed; i++) cap += strlen(gr.named[i]) + 1;
        cap += (size_t) (gr.maxp + 1) + 1;
        names = (char *) ct_alloc(cap);
        {
            int k = snprintf(names, cap, "%d\n", n);
            for (int i = 0; i <= gr.maxp; i++) names[k++] = '\n';
            for (int i = 0; i < gr.nnamed; i++) k += snprintf(names + k, cap - (size_t) k, "%s\n", gr.named[i]);
            names[k] = 0;
        }
        last = build(cx, IR_CALL, γ, ω);
        IR_LIT(last).sval = (char *) "__rk_match_last";
        clr = build(cx, IR_CALL, last, ω);
        IR_LIT(clr).sval = (char *) "__rk_match_clear";
        head = lc_build(g, IR_MATCH_BEGIN, NULL, clr);
        release = lc_build(g, IR_MATCH_END, last, NULL);
        ir_operand_push(release, head);
        land = lc_build(g, IR_MATCH_RKLAND, release, NULL);
        IR_LIT(land).sval = names;
    }
    p = rk_rx_lower(cx, rx, land, head, land, &gr);
    lc_ω_to_β(land, p.tail);
    lc_γ_to(head, p.entry);
    { int before = g->n; se = lower_rv(cx, t->c[0], head, ω, &sv); rk_rx_retag_alpha(g, before, head); }
    if (sv) ir_operand_push(head, sv);
    *res = last;
    return se ? se : head;
}
static int rk_rx_boxes_on(void) { const char * e = getenv("RK_RX_BOXES"); return !(e && e[0] == '0'); }
static int rk_proc_known(const char * name);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_refuse(const tree_t * t, const char * what) { fprintf(stderr, "scrip: raku: line %d: REFUSED -- %s\n", t ? t->line : 0, what); exit(2); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * rk_excise(rcx_t * cx, IR_t * γ, IR_t * ω, IR_t ** res) { IR_t * nd = build(cx, IR_EXCISED, γ, ω); if (res) *res = nd; return nd; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int trace_wanted(void) { return g_runtime.abi.trace_budget != 0; }
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
        for (int k = 1; k < u->n; k++) if (u->c[k] && u->c[k]->t == TT_ARM && u->c[k]->n == 1) hasdef = 1;
        if (hasdef) { ast_push(nh, (tree_t *) s); continue; }
        tree_t * nc = ast_node_new(TT_CASE);
        nc->line = u->line;
        for (int k = 0; k < u->n; k++) ast_push(nc, u->c[k]);
        tree_t * rb = ast_node_new(TT_SEQ_EXPR);
        ast_push(rb, rt);
        tree_t * da = ast_node_new(TT_ARM);
        ast_push(da, rb);
        ast_push(nc, da);
        ast_push(nh, nc);
    }
    if (ci < 0) ast_push(nh, rt);
    return nh;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_arm_chain(const tree_t * const * st, int n, const char * subj);
static void rk_arm_body(tree_t * seq, int from);
static int rk_block_has_arm(const tree_t * t);
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
    if (t && (t->t == TT_SEQ || t->t == TT_SEQ_EXPR || t->t == TT_PROGRAM) && rk_block_has_arm(t)) {
        tree_t * ch = rk_arm_chain((const tree_t * const *) t->c, t->n, "_");
        return lower_rblock(cx, ch, γ, ω, last_res);
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
static int rk_param_is_rw(const tree_t * pv);
static tree_t * rk_ti(long long v);
static int rk_callee_param_rw(const char * nm, int k) {
    for (int i = 0; nm && i < g_stage2.proc_count; i++) if (g_stage2.proc_table[i].name && !strcmp(g_stage2.proc_table[i].name, nm)) {
        const tree_t * pr = (const tree_t *) g_stage2.proc_table[i].proc;
        return pr && pr->t == TT_SUB_DECL && (1 + k) < pr->n && rk_param_is_rw(pr->c[1 + k]);
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_rcall(rcx_t * cx, const tree_t * t, const char * nm, int from, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * nd = build(cx, rk_proc_is_generator(nm) ? IR_PROC_GEN : IR_CALL, γ, ω);
    IR_LIT(nd).sval = nm;
    int nargs = t->n - from;
    for (int k = 0; nm && nm[0] != '_' && k < nargs; k++) {
        const tree_t * a0 = t->c[from + k];
        if (a0 && (a0->t == TT_ILIT || a0->t == TT_FLIT || a0->t == TT_QLIT) && rk_callee_param_rw(nm, k)) {
            tree_t * rf = ast_node_new(TT_FNC);
            rf->v.sval = (char *) "__rk_rw_refuse";
            ast_push(rf, leaf_sval2(TT_VAR, "__rk_rw_refuse"));
            ast_push(rf, leaf_sval2(TT_QLIT, nm));
            ast_push(rf, rk_ti(k));
            ast_push(rf, (tree_t *) a0);
            return lower_rv(cx, rf, γ, ω, res);
        }
    }
    IR_t * prev = NULL;
    IR_t * tgt = nd;
    if (nd->op == IR_PROC_GEN) { tgt = build(cx, IR_GOTO, NULL, NULL); lc_γ_to(tgt, nd); lc_ω_to(tgt, nd); }
    IR_t * entry = tgt;
    uint64_t brm = rk_callee_byref_mask(nm);
    if (!strcmp(nm, "__blk_close")) brm = ~1ULL;
    if (!strcmp(nm, "__rk_byref_assign")) brm = 1ULL;
    for (int k = 0; k < nargs; k++) {
        const tree_t * argt = t->c[from + k];
        IR_t * ar = NULL;
        IR_t * ae;
        if (((brm >> k) & 1ULL) && argt && argt->t == TT_VAR && argt->v.sval && !rk_var_byref(cx, argt)) {
            IR_t * vr = build(cx, IR_VAR_REF, (k == nargs - 1) ? tgt : NULL, ω);
            IR_LIT(vr).sval = argt->v.sval;
            ae = vr;
            ar = vr;
        } else if (((brm >> k) & 1ULL) && argt && argt->t == TT_VAR && argt->v.sval) {
            IR_t * vr = build(cx, IR_VAR, (k == nargs - 1) ? tgt : NULL, ω);
            IR_LIT(vr).sval = argt->v.sval;
            ae = vr;
            ar = vr;
        } else if (argt && argt->t == TT_FNC && argt->n == 2 && argt->v.sval && !strcmp(argt->v.sval, "__rk_byval")) {
            ae = lower_rv(cx, argt->c[1], (k == nargs - 1) ? tgt : NULL, ω, &ar);
        } else {
            ae = lower_rv(cx, argt, (k == nargs - 1) ? tgt : NULL, ω, &ar);
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
    if (t->t == TT_VAR && t->v.sval && !strcmp(t->v.sval, "!")) {
        tree_t * mc = ast_node_new(TT_METHCALL);
        ast_push(mc, (tree_t *) t);
        ast_push(mc, leaf_sval2(TT_QLIT, "Str"));
        return lower_rv(cx, mc, γ, ω, res);
    }
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
static tree_t * rk_fnc2(const char * nm, const tree_t * a, const tree_t * b);
static const tree_t * rk_closure_block(const tree_t * t);
static tree_t * rk_fnc_cl(const char * nm, const tree_t * cl, const tree_t * a, const tree_t * b);
static int rk_proc_nparams(const char * name);
static const tree_t * rk_block_decl(const tree_t * prog, const char * nm);
static int rk_is_code_var(const tree_t * t) { size_t n = (t && t->t == TT_VAR && t->v.sval) ? strlen(t->v.sval) : 0; return n > 6 && !strcmp(t->v.sval + n - 6, "__code"); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_code_call1(const tree_t * prog, const tree_t * code, const tree_t * topic, int * ok) {
    const tree_t * cl = rk_closure_block(code) ? code : NULL, * B = cl ? cl->c[1] : code;
    *ok = 0;
    if (rk_is_code_var(code)) { tree_t * iv = ast_node_new(TT_INVOKE); ast_push(iv, (tree_t *) code); if (topic) ast_push(iv, (tree_t *) topic); *ok = 1; return iv; }
    if (!B || B->t != TT_ANON_BLOCK || !B->v.sval) return NULL;
    const tree_t * bd = prog ? rk_block_decl(prog, B->v.sval) : NULL;
    int np = prog ? (bd ? (int) bd->v.ival : -1) : rk_proc_nparams(B->v.sval), nref = cl ? cl->n - 2 : 0;
    if (np < 0 || np - nref > 1) return NULL;
    *ok = 1;
    return rk_fnc_cl(B->v.sval, cl, np - nref == 1 ? topic : NULL, NULL);
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
    { int ok = 0; tree_t * bc = rk_code_call1(NULL, cond, subj, &ok); if (ok) return rk_fnc2("__rk_mkbool", bc, NULL); }
    if (cond->t != TT_VAR && cond->t != TT_ILIT && cond->t != TT_FLIT) return rk_fnc2("__rk_smartmatch", subj, cond);
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
static tree_t * rk_arm_chain(const tree_t * const * st, int n, const char * subj) {
    tree_t * rest = NULL;
    const tree_t * run[n > 0 ? n : 1];
    int nr = 0;
    for (int i = n - 1; i >= -1; i--) {
        const tree_t * u = i >= 0 ? st[i] : NULL;
        if (u && u->t == TT_STMT) { const tree_t * sub = stmt_subj(u); if (sub) u = sub; }
        int arm = u && u->t == TT_ARM && u->n >= 1;
        if (i >= 0 && !arm) { run[nr++] = st[i]; continue; }
        if (nr) { tree_t * sq = ast_node_new(TT_SEQ_EXPR); for (int k = nr - 1; k >= 0; k--) ast_push(sq, (tree_t *) run[k]); if (rest) ast_push(sq, rest); rest = sq; nr = 0; }
        if (i < 0) break;
        if (u->n == 1) { rest = (tree_t *) u->c[0]; continue; }
        tree_t * gate = ast_node_new(TT_IF);
        ast_push(gate, rk_case_match(leaf_sval2(TT_VAR, subj), u->c[0]));
        if (u->c[1] && u->c[1]->t == TT_SEQ_EXPR) rk_arm_body((tree_t *) u->c[1], 0);
        ast_push(gate, (tree_t *) u->c[1]);
        if (rest) ast_push(gate, rest);
        rest = gate;
    }
    return rest;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_block_has_arm(const tree_t * t) {
    for (int i = 0; t && i < t->n; i++) { const tree_t * u = t->c[i]; if (u && u->t == TT_STMT) { const tree_t * sub = stmt_subj(u); if (sub) u = sub; } if (u && u->t == TT_ARM) return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_case_desugar(const tree_t * t) {
    int cid = g_lower.rk.rk_case_desugar_cid++;
    char nb[48], sb[48];
    snprintf(nb, sizeof nb, "__case_%d", cid);
    snprintf(sb, sizeof sb, "__casesv_%d", cid);
    const char * vn = intern(nb), * sv = intern(sb);
    tree_t * chain = rk_arm_chain((const tree_t * const *) t->c + 1, t->n - 1, vn);
    tree_t * seq = ast_node_new(TT_SEQ);
    tree_t * as = ast_node_new(TT_ASSIGN);
    ast_push(as, leaf_sval2(TT_VAR, vn));
    ast_push(as, (tree_t *) t->c[0]);
    ast_push(seq, as);
    tree_t * sa = ast_node_new(TT_ASSIGN);
    ast_push(sa, leaf_sval2(TT_VAR, sv));
    ast_push(sa, leaf_sval2(TT_VAR, "_"));
    ast_push(seq, sa);
    tree_t * ta = ast_node_new(TT_ASSIGN);
    ast_push(ta, leaf_sval2(TT_VAR, "_"));
    ast_push(ta, leaf_sval2(TT_VAR, vn));
    ast_push(seq, ta);
    if (chain) ast_push(seq, chain);
    tree_t * ra = ast_node_new(TT_ASSIGN);
    ast_push(ra, leaf_sval2(TT_VAR, "_"));
    ast_push(ra, leaf_sval2(TT_VAR, sv));
    ast_push(seq, ra);
    return seq;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_cond(rcx_t * cx, const tree_t * c, IR_t * on_true, IR_t * on_false) {
    if (c && c->t == TT_STMT) { const tree_t * sub = stmt_subj(c); if (sub) c = sub; }
    if (!c) return on_true;
    if (c->t == TT_DIVIS && c->n > 1) return lower_cond(cx, rk_divis_desugar(c), on_true, on_false);
    if (rk_is_relop(c->t) && c->n > 1) {
        IR_t * op = build(cx, IR_BINOP_TEST, on_true, on_false);
        rk_relop_code(op, c->t);
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
static const tree_t * rk_closure_block(const tree_t * t) {
    if (t && t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, "__blk_close") && t->n > 1 && t->c[1] && t->c[1]->t == TT_ANON_BLOCK && t->c[1]->v.sval) return t->c[1];
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_fnc_cl(const char * nm, const tree_t * cl, const tree_t * a, const tree_t * b) {
    tree_t * f = rk_fnc2(nm, NULL, NULL);
    for (int i = 2; cl && i < cl->n; i++) ast_push(f, cl->c[i]);
    if (a) ast_push(f, (tree_t *) a);
    if (b) ast_push(f, (tree_t *) b);
    return f;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_ti(long long v);
static tree_t * rk_iter_call(const char * bn, const tree_t * cl, const tree_t * cv, const tree_t * a, const tree_t * b) {
    if (!cv) return rk_fnc_cl(bn, cl, a, b);
    tree_t * iv = ast_node_new(TT_INVOKE);
    ast_push(iv, (tree_t *) cv);
    if (a) ast_push(iv, (tree_t *) a);
    if (b) ast_push(iv, (tree_t *) b);
    return iv;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_first_adverb(const tree_t * a, int * fend, int * fmode) {
    if (!a || a->t != TT_FNC || !a->v.sval || strcmp(a->v.sval, "__rk_pair") || a->n != 3 || !a->c[1] || a->c[1]->t != TT_QLIT || !a->c[1]->v.sval) return 0;
    const tree_t * b = a->c[2];
    if (!b || b->t != TT_FNC || !b->v.sval || strcmp(b->v.sval, "__rk_mkbool") || b->n != 2 || !b->c[1] || b->c[1]->t != TT_ILIT) return 0;
    const char * k = a->c[1]->v.sval;
    int m = !strcmp(k, "k") ? 1 : !strcmp(k, "kv") ? 2 : !strcmp(k, "p") ? 3 : !strcmp(k, "v") ? 0 : !strcmp(k, "end") ? -2 : -1;
    if (m == -1) return 0;
    if (b->c[1]->v.ival) { if (m == -2) *fend = 1; else *fmode = m; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_proc_slurpy_only(const char * name, int nref) {
    for (int i = 0; name && i < g_stage2.proc_count; i++) if (g_stage2.proc_table[i].name && !strcmp(g_stage2.proc_table[i].name, name)) return g_stage2.proc_table[i].is_variadic &&
        !g_stage2.proc_table[i].named_rest && g_stage2.proc_table[i].nparams == nref + 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_throws_like_box_attrs(const tree_t * t) {
    tree_t * nt = ast_node_new(TT_FNC);
    nt->v.sval = t->v.sval;
    nt->line = t->line;
    nt->slen = 80;
    for (int i = 0; i < t->n; i++) {
        const tree_t * a = t->c[i];
        int pr = i >= 3 && a && a->t == TT_FNC && a->v.sval && !strcmp(a->v.sval, "__rk_pair") && a->n == 3 && a->c[1] && a->c[1]->t == TT_QLIT && a->c[1]->v.sval;
        if (!pr) { ast_push(nt, (tree_t *) a); continue; }
        tree_t * ex = leaf_sval2(TT_VAR, t->c[1]->v.sval);
        tree_t * mc = ast_node_new(TT_METHCALL);
        ast_push(mc, leaf_sval2(TT_VAR, t->c[1]->v.sval));
        ast_push(mc, leaf_sval2(TT_QLIT, a->c[1]->v.sval));
        tree_t * um = rk_fnc2("__rk_exc_umeth", ex, leaf_sval2(TT_QLIT, a->c[1]->v.sval));
        tree_t * tn = ast_node_new(TT_TERNARY);
        ast_push(tn, um);
        ast_push(tn, rk_fnc2("__rk_tl_got", a->c[2], mc));
        ast_push(tn, a->c[2]);
        tree_t * np = rk_fnc2("__rk_pair", a->c[1], tn);
        ast_push(nt, np);
    }
    return nt;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_throw_desugar(rcx_t * cx, const tree_t * e) {
    char xn[40];
    snprintf(xn, sizeof xn, "$?xe%d", cx->g->n);
    const char * xv = lp_strdup(xn);
    tree_t * as = ast_node_new(TT_ASSIGN);
    ast_push(as, leaf_sval2(TT_VAR, xv));
    ast_push(as, (tree_t *) e);
    tree_t * mc = ast_node_new(TT_METHCALL);
    ast_push(mc, leaf_sval2(TT_VAR, xv));
    ast_push(mc, leaf_sval2(TT_QLIT, "message"));
    tree_t * tn = ast_node_new(TT_TERNARY);
    ast_push(tn, rk_fnc2("__rk_exc_umsg", leaf_sval2(TT_VAR, xv), NULL));
    ast_push(tn, mc);
    ast_push(tn, rk_fnc2("__rk_undef", NULL, NULL));
    tree_t * sq = ast_node_new(TT_SEQ_EXPR);
    ast_push(sq, as);
    ast_push(sq, rk_fnc2("__rk_die_m", leaf_sval2(TT_VAR, xv), tn));
    return sq;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_first_ix(const char * src, const char * idx, int fend) {
    if (!fend) return leaf_sval2(TT_VAR, idx);
    tree_t * e1 = ast_node_new(TT_SUB);
    ast_push(e1, rk_fnc2("elems", leaf_sval2(TT_VAR, src), NULL));
    ast_push(e1, rk_ti(1));
    tree_t * rv = ast_node_new(TT_SUB);
    ast_push(rv, e1);
    ast_push(rv, leaf_sval2(TT_VAR, idx));
    return rv;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * rk_lower_iter_meth(rcx_t * cx, const tree_t * t, const char * mname, IR_t * γ, IR_t * ω, IR_t ** res) {
    int keyed = t->n == 4 && t->c[2] && t->c[2]->t == TT_QLIT && t->c[2]->v.sval && !strcmp(t->c[2]->v.sval, "as") &&
        (!strcmp(mname, "unique") || !strcmp(mname, "squish") || !strcmp(mname, "repeated"));
    int k = !strcmp(mname, "map") ? 1 : !strcmp(mname, "grep") ? 2 : !strcmp(mname, "first") ? 3 : !strcmp(mname, "reduce") ? 4 : !strcmp(mname, "sort") ? 5 : 0;
    if (!k) k = !strcmp(mname, "produce") ? 6 : keyed ? 7 : 0;
    int fend = 0, fmode = 0, fadv = k == 3 && t->n > 3;
    for (int i = 3; fadv && i < t->n; i++) if (!rk_first_adverb(t->c[i], &fend, &fmode)) fadv = 0;
    if (!fadv) fend = fmode = 0;
    const tree_t * fa = keyed ? t->c[3] : (t->n == 3 || fadv) ? t->c[2] : NULL;
    if ((k == 2 || k == 3) && t->slen != 79 && fa && fa->t == TT_VAR && fa->v.sval && fa->v.sval[0] && !strchr("&@%$", fa->v.sval[0]) && !rk_is_code_var(fa) && !rk_proc_known(fa->v.sval)) {
        char cn[48];
        snprintf(cn, sizeof cn, "$?fc%d__code", cx->g->n);
        const char * cvn = lp_strdup(cn);
        tree_t * mc = ast_node_new(TT_METHCALL);
        mc->line = t->line;
        for (int i = 0; i < t->n; i++) ast_push(mc, i == 2 ? leaf_sval2(TT_VAR, cvn) : t->c[i]);
        tree_t * as = ast_node_new(TT_ASSIGN);
        ast_push(as, leaf_sval2(TT_VAR, cvn));
        ast_push(as, (tree_t *) fa);
        tree_t * sq = ast_node_new(TT_SEQ_EXPR);
        ast_push(sq, as);
        ast_push(sq, mc);
        tree_t * mm = ast_node_new(TT_METHCALL);
        mm->line = t->line;
        mm->slen = 79;
        for (int i = 0; i < t->n; i++) ast_push(mm, t->c[i]);
        tree_t * tn = ast_node_new(TT_TERNARY);
        ast_push(tn, rk_fnc2("__rk_is_code", fa, NULL));
        ast_push(tn, sq);
        ast_push(tn, mm);
        return lower_rv(cx, tn, γ, ω, res);
    }
    const tree_t * cl = rk_closure_block(fa) ? fa : NULL;
    const char * bn = NULL;
    int scalar_code = k == 1 && fa && fa->t == TT_VAR && fa->v.sval && fa->v.sval[0] && !strchr("&@%$", fa->v.sval[0]) && !rk_proc_known(fa->v.sval);
    const tree_t * cv = (rk_is_code_var(fa) || scalar_code) ? fa : NULL;
    if (cl) fa = cl->c[1];
    if (fa && fa->t == TT_ANON_BLOCK && fa->v.sval) bn = fa->v.sval;
    else if (fa && fa->t == TT_VAR && fa->v.sval && fa->v.sval[0] == '&' && rk_proc_known(fa->v.sval + 1) && !rk_is_multi_name(fa->v.sval + 1)) bn = fa->v.sval + 1;
    if (!k || !(bn || (cv && k != 5)) || !t->c[0]) return NULL;
    int np = cv ? (k == 4 || k == 6 ? 2 : 1) : rk_proc_nparams(bn), nref = cl ? cl->n - 2 : 0;
    if (np < 0 && !cv && k >= 1 && k <= 3 && rk_proc_slurpy_only(bn, nref)) np = 1 + nref;
    if (np >= 0 && (np -= nref) < 0) return NULL;
    int step = (k == 1 && np > 1) ? np : 1;
    if ((k == 4 || k == 6) ? np != 2 : k == 5 ? np != 1 : (np < 0 || (k != 1 && np > 1))) return NULL;
    if (rk_user_meth_named(mname) || (k > 3 && t->c[0]->t == TT_VAR && t->c[0]->v.sval && t->c[0]->v.sval[0] == '%')) return NULL;
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
    else if (k >= 6) {
        const char * dn = k == 6 ? "__rk_produce_done" : "__rk_keyed_done";
        fin = lower_rcall(cx, rk_fnc2(dn, leaf_sval2(TT_VAR, acc), NULL), dn, 1, γ, ω, &fr);
    } else {
        fin = build(cx, IR_VAR, γ, ω);
        IR_LIT(fin).sval = acc;
        fr = fin;
    }
    IR_t * va = build(cx, IR_ASSIGN, NULL, ω);
    IR_LIT(va).sval = idx;
    IR_t * to = build(cx, IR_TO, va, fin);
    IR_LIT(to).sval = "ag";
    tree_t * lo = ast_node_new(TT_ILIT);
    lo->v.ival = (k == 4 || k == 6) ? 1 : 0;
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
    tree_t * at = rk_fnc2("__rk_arr_at", leaf_sval2(TT_VAR, src), rk_first_ix(src, idx, fend));
    IR_t * bentry, * r = NULL;
    if (k == 1 || k == 4 || k == 5 || k == 6 || k == 7) {
        IR_t * as = build(cx, IR_ASSIGN, to, to);
        IR_LIT(as).sval = acc;
        tree_t * call = k == 4 ? rk_iter_call(bn, cl, cv, leaf_sval2(TT_VAR, acc), at) : rk_iter_call(bn, cl, cv, np ? at : NULL, NULL);
        if (step > 1) {
            call = rk_iter_call(bn, cl, cv, NULL, NULL);
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
        if (k == 6) call = rk_fnc2("__rk_produce_step", leaf_sval2(TT_VAR, acc), rk_iter_call(bn, cl, cv, rk_fnc2("__rk_produce_last", leaf_sval2(TT_VAR, acc), NULL), at));
        if (k == 7) { tree_t * kc = rk_iter_call(bn, cl, cv, np ? at : NULL, NULL); call = rk_fnc2("__rk_keyed_step", leaf_sval2(TT_VAR, acc), at); ast_push(call, kc); }
        bentry = call->t == TT_INVOKE ? lower_rv(cx, call, as, to, &r) : lower_rcall(cx, call, call->v.sval, 1, as, to, &r);
        if (r) ir_operand_push(as, r);
    } else {
        IR_t * hit;
        if (k == 2) {
            IR_t * as = build(cx, IR_ASSIGN, to, to);
            IR_LIT(as).sval = acc;
            hit = lower_rcall(cx, rk_fnc2("__rk_grep_append", leaf_sval2(TT_VAR, acc), leaf_sval2(TT_VAR, el)), "__rk_grep_append", 1, as, to, &r);
            if (r) ir_operand_push(as, r);
        } else if (fmode) {
            IR_t * as = build(cx, IR_ASSIGN, fin, to);
            IR_LIT(as).sval = acc;
            tree_t * sh = rk_fnc2("__rk_first_shape", leaf_sval2(TT_VAR, src), rk_first_ix(src, idx, fend));
            ast_push(sh, rk_ti(fmode));
            IR_t * sr = NULL;
            hit = lower_rcall(cx, sh, "__rk_first_shape", 1, as, to, &sr);
            if (sr) ir_operand_push(as, sr);
        } else {
            IR_t * as = build(cx, IR_ASSIGN, fin, to);
            IR_LIT(as).sval = acc;
            hit = build(cx, IR_VAR, as, to);
            IR_LIT(hit).sval = el;
            ir_operand_push(as, hit);
        }
        IR_t * ce = lower_cond(cx, rk_iter_call(bn, cl, cv, np ? leaf_sval2(TT_VAR, el) : NULL, NULL), hit, to);
        IR_t * ael = build(cx, IR_ASSIGN, ce, to);
        IR_LIT(ael).sval = el;
        bentry = lower_rcall(cx, at, "__rk_arr_at", 1, ael, to, &r);
        if (r) ir_operand_push(ael, r);
    }
    γ_to(va, bentry);
    IR_t * ares = build(cx, IR_ASSIGN, elo, ω);
    IR_LIT(ares).sval = acc;
    tree_t * init = (k <= 2 || k == 5) ? rk_fnc2("__rk_iter_src", NULL, NULL) : k == 3 ? rk_fnc2("__rk_undef", NULL, NULL) : k == 6 ? rk_fnc2("__rk_produce_init", leaf_sval2(TT_VAR, src), NULL) :
        k == 7 ? rk_fnc2("__rk_keyed_init", rk_ti(mname[0] == 'u' ? 0 : mname[0] == 's' ? 1 : 2), NULL) : rk_fnc2("__rk_arr_at", leaf_sval2(TT_VAR, src), ast_node_new(TT_ILIT));
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
        rk_relop_code(op, t->t);
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
    if (t->t == TT_POW && t->n > 1 && !(t->c[1] && t->c[1]->t == TT_ILIT && (t->c[1]->v.ival == 1 || t->c[1]->v.ival == 2))) { return lower_rcall(cx, t, "__rk_pow", 0, γ, ω, res); }
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
        if (t->n == 1 && t->c[0] && t->c[0]->t == TT_QLIT) {
            tree_t * rl = ast_node_new(TT_FNC);
            rl->line = t->line;
            rl->v.sval = (char *) intern("__rk_ratlit");
            ast_push(rl, leaf_sval2(TT_VAR, "__rk_ratlit"));
            ast_push(rl, t->c[0]);
            return lower_rv(cx, rl, γ, ω, res);
        }
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
            if (x && x->t == TT_FLIT && x->n == 0) { IR_t * nd = build(cx, IR_LIT_REAL, γ, ω); IR_LIT(nd).dval = -x->v.dval; *res = nd; return nd; }
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
                rk_relop_code(op, rhs->t);
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
            IR_t * e = lower_rv(cx, decl_only || t->v.ival == 2 ? t->c[1] : rk_assign_rhs(t->c[0]->v.sval, t->c[1]), nd, ω, &rr);
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
        if (t->n > 2 && t->c[0] && t->c[0]->t == TT_TWIGIL_FIELD) {
            tree_t * nv = ast_node_new(TT_FNC);
            nv->v.sval = (char *) "__rk_arr_set";
            nv->line = t->line;
            ast_push(nv, leaf_sval2(TT_VAR, "__rk_arr_set"));
            for (int i = 0; i < t->n; i++) ast_push(nv, t->c[i]);
            return lower_field_set(cx, t->c[0], nv, γ, ω, res);
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
                if (s && s->t == TT_STMT) { const tree_t * sub = stmt_subj(s); if (sub) s = sub; }
                if (s && s->t == TT_CATCH) continue;
                li = i;
                break;
            }
            const tree_t * ls = li >= 0 ? ob->c[li] : NULL;
            if (ls && ls->t == TT_STMT) { const tree_t * sub = stmt_subj(ls); if (sub) ls = sub; }
            if (ls && ls->t != TT_IF && ls->t != TT_UNLESS && ls->t != TT_WHILE && ls->t != TT_UNTIL && ls->t != TT_REPEAT && ls->t != TT_FOR && ls->t != TT_FOR_RANGE && ls->t != TT_EVERY &&
                ls->t != TT_CASE && ls->t != TT_RETURN && ls->t != TT_NUL && ls->t != TT_SEQ && ls->t != TT_SEQ_EXPR && ls->t != TT_ASSIGN && ls->t != TT_REVASSIGN && ls->t != TT_ARR_SET &&
                ls->t != TT_HASH_SET && ls->t != TT_DECL && ls->t != TT_ARR_DECL && ls->t != TT_HASH_DECL && ls->t != TT_RW_DECL && ls->t != TT_STATIC_DECL && ls->t != TT_SUB_DECL &&
                ls->t != TT_CLASS_DECL) {
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
                    if (s && s->t == TT_CATCH && s->n > 0) { handler = rk_catch_handler(s); break; }
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
            if (nm && !strcmp(nm, "__rk_test_throws_like") && t->slen != 80 && t->n > 3 && t->c[1] && t->c[1]->t == TT_VAR) return lower_rv(cx, rk_throws_like_box_attrs(t), γ, ω, res);
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
        if (t->slen != 82 && t->n > 1 && t->c[0] && t->c[0]->t == TT_ITERATE && t->c[0]->n > 0 && !t->c[0]->v.sval) {
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
                IR_t * sv_exit = cx->loop_exit, * sv_next = cx->loop_next;
                cx->loop_exit = γ;
                cx->loop_next = rcall;
                IR_t * inner = rk_xf_body(cx, xf, xfk, vname, rcall, body, rcall, ω);
                cx->loop_exit = sv_exit;
                cx->loop_next = sv_next;
                γ_to(rcall, inner);
                *res = rcall;
                return centry;
            }
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
            if (plain) { last = build(cx, IR_CALL, γ, ω); IR_LIT(last).sval = (char *) "__rk_match_last"; }
            IR_t * nd = build(cx, IR_CALL, plain ? last : γ, plain ? last : ω);
            IR_LIT(nd).sval = strcmp(t->c[2]->v.sval, "match_global") ? "re_match" : "re_match_global";
            IR_t * sr = NULL, * pr = NULL;
            IR_t * es = lower_rv(cx, t->c[0], NULL, ω, &sr);
            IR_t * ep = lower_rv(cx, t->c[1], nd, ω, &pr);
            γ_to(sr, ep);
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
            IR_t * sv_exit = cx->loop_exit, * sv_next = cx->loop_next;
            cx->loop_exit = γ;
            cx->loop_next = to;
            IR_t * bentry = lower_rblock(cx, t->c[3], to, ω, NULL);
            cx->loop_exit = sv_exit;
            cx->loop_next = sv_next;
            γ_to(va, bentry);
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
        if (cx->cur_proc_name && rk_proc_is_generator(cx->cur_proc_name)) {
            IR_t * sn = build(cx, IR_SUSPEND, cx->proc_exit ? cx->proc_exit : γ, ω);
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
            IR_t * jv = build(cx, IR_VAR, γ, ω);
            IR_LIT(jv).sval = tname;
            int dt = t->c[1] && t->c[1]->t == TT_RETURN, df = t->c[2] && t->c[2]->t == TT_RETURN;
            IR_t * at = dt ? NULL : build(cx, IR_ASSIGN, jv, ω);
            if (at) IR_LIT(at).sval = tname;
            IR_t * af = df ? NULL : build(cx, IR_ASSIGN, jv, ω);
            if (af) IR_LIT(af).sval = tname;
            IR_t * et = at ? rk_tern_branch(cx, t->c[1], at, ω) : lower_rv(cx, t->c[1], jv, ω, NULL);
            IR_t * ef = af ? rk_tern_branch(cx, t->c[2], af, ω) : lower_rv(cx, t->c[2], jv, ω, NULL);
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
static void rk_discover_grammars(const tree_t * prog) {
    extern void rt_grammar_register(const char *qname, const char *body, int flavor);
    g_lower.rk.gram_names.len = 0;
    if (!prog) return;
    rk_discover_lexical_regexes(prog);
    for (int i = 0; i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) { const tree_t * sub = stmt_subj(d); if (!sub) continue; d = sub; }
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
static void rk_register_classes(const tree_t * prog) {
    extern void record_register(const char *spec);
    if (!prog) return;
    g_lower.rk.class_names.len = 0;
    g_lower.rk.user_write_meth = 0;
    memset(g_lower.rk.listlike_overridden, 0, sizeof g_lower.rk.listlike_overridden);
    for (int i = 0; i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) { const tree_t * sub = stmt_subj(d); if (!sub) continue; d = sub; }
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
    int rk_gen = rk_proc_is_generator(rk_proc_name);
    IR_t * fall = rk_gen ? build(&cx, IR_GOTO, fail, fail) : build(&cx, IR_RETURN, succ, fail);
    if (rk_gen) { g->cell_frame = 1; g->spine_cells = 1; }
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
    if (t->t == TT_ARM) *topic |= 2;
    if (t->t == TT_VAR && t->v.sval) {
        const char * v = t->v.sval;
        if (!strcmp(v, "_")) *topic |= 1;
        else if (v[0] == '^') { int seen = 0; for (int i = 0; i < *nph; i++) if (!strcmp(ph[i], v)) seen = 1; if (!seen && *nph < max) ph[(*nph)++] = v; }
    }
    for (int i = 0; i < t->n; i++) rk_scan_implicit_params(t->c[i], topic, ph, nph, max);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_topic_default(tree_t * seq) {
    tree_t * def = ast_node_new(TT_METHCALL);
    ast_push(def, leaf_sval2(TT_VAR, "_"));
    ast_push(def, leaf_sval2(TT_QLIT, "defined"));
    tree_t * set = ast_node_new(TT_ASSIGN);
    ast_push(set, leaf_sval2(TT_VAR, "_"));
    ast_push(set, rk_fnc2("__rk_undef", NULL, NULL));
    tree_t * arm = ast_node_new(TT_SEQ_EXPR);
    ast_push(arm, set);
    tree_t * un = ast_node_new(TT_UNLESS);
    ast_push(un, def);
    ast_push(un, arm);
    ast_push(seq, un);
    for (int i = seq->n - 1; i > 0; i--) { tree_t * x = seq->c[i]; seq->c[i] = seq->c[i - 1]; seq->c[i - 1] = x; }
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
static int rk_param_is_rw(const tree_t * pv) { if (pv && pv->t == TT_ASSIGN && pv->n) pv = pv->c[0]; return pv && pv->t == TT_VAR && (pv->slen & RK_PV_RW); }
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
    if (t->t == TT_TWIGIL_FIELD) rk_ns_add(out, intern("self"));
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
    if (rk_is_scalar_bind(t)) return;
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
    if (t->t == TT_ASSIGN && t->v.ival != 2 && t->n > 1 && t->c[0] && t->c[0]->t == TT_VAR && (t->c[0]->slen & 4) && rk_ns_has(Bx, t->c[0]->v.sval)) {
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
static int rk_cap_is_package(const tree_t * t) { return t && (t->t == TT_CLASS_DECL || t->t == TT_ROLE_DECL); }
static void rk_cap_proc_m(tree_t * sd, int mth);
static void rk_bind_names(const tree_t * t, rk_ns_t * out, const rk_ns_t * skip);
static void rk_bind_unbox(tree_t * t, const rk_ns_t * Bx);
static void rk_cap_proc(tree_t * sd) { rk_cap_proc_m(sd, 0); }
static void rk_cap_proc_m(tree_t * sd, int mth) {
    rk_ns_t d = { 0 };
    int np = (int) sd->v.ival - (mth ? 1 : 0);
    if (mth) rk_ns_add(&d, intern("self"));
    for (int k = 0; k < np && 1 + k < sd->n; k++) { const char * pn = rk_cap_pname(sd->c[1 + k]); if (pn) rk_ns_add(&d, pn); }
    for (int i = 1 + np; i < sd->n; i++) rk_cap_declared(sd->c[i], &d);
    rk_ns_t boxable = { 0 }, Bx = { 0 };
    for (int k = 0; k < np && 1 + k < sd->n; k++) { const char * pn = rk_cap_pname(sd->c[1 + k]); if (pn) rk_ns_add(&boxable, pn); }
    for (int i = 1 + np; i < sd->n; i++) rk_cap_decl_assigns(sd->c[i], &boxable);
    rk_cap_boxes(sd, sd, np, &d, &boxable, &Bx);
    for (int i = 1 + np; i < sd->n; i++) rk_bind_names(sd->c[i], &Bx, NULL);
    for (int k = 0; k < np && 1 + k < sd->n; k++) { const char * pn = rk_cap_pname(sd->c[1 + k]); if (pn && rk_param_is_rw(sd->c[1 + k])) rk_ns_add(&Bx, pn); }
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
    for (int i = 1 + np; i < sd->n; i++) { rk_cap_assign_through(sd->c[i], &Bx); rk_cap_flag_boxed(sd->c[i], &Bx); rk_cap_box_decls(sd->c[i], &Bx); rk_bind_unbox(sd->c[i], &Bx); }
    int added = 0;
    for (int k = 0; k < np && 1 + k < sd->n; k++) {
        const char * pn = rk_cap_pname(sd->c[1 + k]);
        if (!pn || !rk_ns_has(&Bx, pn)) continue;
        const char * wrap = rk_param_is_rw(sd->c[1 + k]) ? "__rk_cell_of" : "__rk_box";
        tree_t * bx = ast_node_new(TT_FNC);
        bx->line = sd->line;
        bx->v.sval = (char *) wrap;
        ast_push(bx, leaf_sval2(TT_VAR, wrap));
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
        if (c->t == TT_SUB_DECL) rk_cap_proc_m(c, rk_cap_is_package(t));
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
    rk_bind_names(prog, &Bx, &subn);
    for (int i = 0; i < Bx.n; i++) rk_ns_addf(&own, Bx.v[i], 1);
    rk_cap_walk(prog, &own);
    if (!Bx.n) { rk_bind_unbox(prog, &Bx); return; }
    rk_cap_assign_through(prog, &Bx);
    rk_cap_flag_boxed(prog, &Bx);
    rk_cap_box_decls(prog, &Bx);
    rk_bind_unbox(prog, &Bx);
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
    for (int i = 0; i < st->n; i++) {
        tree_t * ch = st->c[i];
        ast_push(nd, (i >= 1 && ch && (ch->t == TT_SEQ_EXPR || ch->t == TT_IF || ch->t == TT_UNLESS || ch->t == TT_CASE)) ? rk_tail_return(ch) : ch);
    }
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_set_tail(tree_t * st, const char * tmp) {
    if (!st) return st;
    if (st->t == TT_SEQ_EXPR && st->n > 0) { st->c[st->n - 1] = rk_set_tail(st->c[st->n - 1], tmp); return st; }
    if (st->t == TT_IF || st->t == TT_UNLESS || st->t == TT_CASE) {
        if (st->n < 2) return st;
        for (int i = (st->t == TT_CASE ? 1 : 1); i < st->n; i++) {
            tree_t * ch = st->c[i];
            if (st->t == TT_CASE && ch && ch->t == TT_ARM && ch->n >= 1) { ch->c[ch->n - 1] = rk_set_tail(ch->c[ch->n - 1], tmp); continue; }
            if (ch && (ch->t == TT_SEQ_EXPR || ch->t == TT_IF || ch->t == TT_UNLESS || ch->t == TT_CASE)) st->c[i] = rk_set_tail(ch, tmp);
        }
        return st;
    }
    if (!rk_block_tail_is_value(st)) return st;
    tree_t * as = ast_node_new(TT_ASSIGN);
    as->line = st->line;
    ast_push(as, leaf_sval2(TT_VAR, tmp));
    ast_push(as, st);
    return as;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_value_wrap(tree_t * st) {
    char nb[48];
    snprintf(nb, sizeof nb, "__vf_%d", g_lower.rk.rk_case_desugar_cid++);
    const char * tmp = intern(nb);
    tree_t * seq = ast_node_new(TT_SEQ);
    seq->line = st->line;
    tree_t * as = ast_node_new(TT_ASSIGN);
    ast_push(as, leaf_sval2(TT_VAR, tmp));
    ast_push(as, rk_fnc2("__rk_undef", NULL, NULL));
    ast_push(seq, as);
    ast_push(seq, rk_set_tail(st, tmp));
    ast_push(seq, leaf_sval2(TT_VAR, tmp));
    return seq;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_is_value_form(const tree_t * t) { return t && (t->t == TT_IF || t->t == TT_UNLESS || (t->t == TT_CASE && t->n > 1)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_value_forms(tree_t * t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_value_forms(t->c[i]);
    if (t->t == TT_ASSIGN && t->n == 2 && rk_is_value_form(t->c[1])) t->c[1] = rk_value_wrap(t->c[1]);
    if (t->t == TT_ITERATE && t->n > 0 && rk_is_value_form(t->c[0])) t->c[0] = rk_value_wrap(t->c[0]);
    if (t->t == TT_FNC && t->n == 2 && t->c[0] && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, "__rk_phaser_do") && t->c[1] && t->c[1]->t == TT_SEQ_EXPR && t->c[1]->n > 0 &&
        rk_is_value_form(t->c[1]->c[t->c[1]->n - 1])) t->c[1]->c[t->c[1]->n - 1] = rk_value_wrap(t->c[1]->c[t->c[1]->n - 1]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_tail_case(tree_t * st);
static int rk_block_has_arm(const tree_t * t);
static tree_t * rk_arm_chain(const tree_t * const * st, int n, const char * subj);
static void rk_arm_body(tree_t * seq, int from) {
    if (!seq || from >= seq->n) return;
    tree_t tmp = *seq;
    tmp.c = seq->c + from;
    tmp.n = seq->n - from;
    if (!rk_block_has_arm(&tmp)) return;
    tree_t * ch = rk_arm_chain((const tree_t * const *) seq->c + from, seq->n - from, "_");
    seq->n = from;
    while (ch && ch->t == TT_SEQ_EXPR && ch->n > 0) { for (int i = 0; i < ch->n - 1; i++) ast_push(seq, ch->c[i]); ch = ch->c[ch->n - 1]; }
    if (ch) ast_push(seq, ch);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_tail_ifs(tree_t * t, int in_pkg) {
    if (!t) return;
    if (t->t == TT_SUB_DECL && t->n > 1) {
        rk_arm_body(t, in_pkg ? (int) t->v.ival : (int) t->v.ival + 1);
        int li = t->n - 1;
        tree_t * ls = t->c[li];
        if (ls && ls->t == TT_STMT) { const tree_t * sub = stmt_subj(ls); if (sub) ls = (tree_t *) sub; }
        if (ls && (ls->t == TT_IF || ls->t == TT_UNLESS)) t->c[li] = rk_tail_if(ls);
        else if (ls && ls->t == TT_CASE && ls->n > 1) t->c[li] = rk_tail_case(ls);
    }
    for (int i = 0; i < t->n; i++) rk_tail_ifs(t->c[i], t->t == TT_CLASS_DECL || t->t == TT_ROLE_DECL || t->t == TT_GRAMMAR_DECL);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_tail_case(tree_t * st) {
    tree_t * nd = ast_node_new(TT_CASE);
    nd->line = st->line;
    nd->v = st->v;
    nd->slen = st->slen;
    for (int i = 0; i < st->n; i++) {
        tree_t * ch = st->c[i];
        if (i >= 1 && ch && ch->t == TT_ARM && ch->n >= 1) {
            tree_t * arm = ast_node_new(TT_ARM);
            arm->line = ch->line;
            arm->v = ch->v;
            arm->slen = ch->slen;
            for (int k = 0; k < ch->n; k++) ast_push(arm, k == ch->n - 1 ? rk_tail_return(ch->c[k]) : ch->c[k]);
            ch = arm;
        }
        ast_push(nd, ch);
    }
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_tail_return(tree_t * st) {
    if (st && st->t == TT_CASE && st->n > 1) return rk_tail_case(st);
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
static void rk_named_ph_scan(const tree_t * t, rk_ns_t * out) {
    if (!t || t->t == TT_ANON_BLOCK || t->t == TT_SUB_DECL) return;
    if (t->t == TT_VAR && t->v.sval && t->v.sval[0] == ':' && ((t->v.sval[1] >= 'a' && t->v.sval[1] <= 'z') || (t->v.sval[1] >= 'A' && t->v.sval[1] <= 'Z') || t->v.sval[1] == '_'))
        rk_ns_add(out, t->v.sval);
    for (int i = 0; i < t->n; i++) rk_named_ph_scan(t->c[i], out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_named_ph_rename(tree_t * t, const char * from, const char * to) {
    if (!t || t->t == TT_ANON_BLOCK || t->t == TT_SUB_DECL) return;
    if (t->t == TT_VAR && t->v.sval && !strcmp(t->v.sval, from)) t->v.sval = (char *) to;
    for (int i = 0; i < t->n; i++) rk_named_ph_rename(t->c[i], from, to);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_named_placeholders(tree_t * t) {
    rk_ns_t np = { 0 };
    for (int i = 1 + (int) t->v.ival; i < t->n; i++) rk_named_ph_scan(t->c[i], &np);
    if (!np.n) return;
    int at = 1 + (int) t->v.ival;
    for (int k = 0; k < np.n; k++) {
        const char * nm = intern(np.v[k] + 1);
        for (int i = at; i < t->n; i++) rk_named_ph_rename(t->c[i], np.v[k], nm);
        ast_push(t, NULL);
        for (int j = t->n - 1; j > at; j--) t->c[j] = t->c[j - 1];
        t->c[at++] = leaf_sval2(TT_VAR, nm);
        t->v.ival++;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_sub_placeholders(tree_t * t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_sub_placeholders(t->c[i]);
    if (t->t != TT_SUB_DECL || t->v.ival != 0 || t->n < 1) return;
    int topic = 0, nph = 0, phmax = rk_tree_size(t) + 1;
    const char ** ph = (const char **) ct_alloc(sizeof(const char *) * (size_t) phmax);
    for (int i = 1; i < t->n; i++) rk_scan_implicit_params(t->c[i], &topic, ph, &nph, phmax);
    if (nph == 0) { rk_named_placeholders(t); return; }
    for (int a = 1; a < nph; a++) { const char * key = ph[a]; int b = a - 1; while (b >= 0 && strcmp(ph[b], key) > 0) { ph[b + 1] = ph[b]; b--; } ph[b + 1] = key; }
    int nbody = t->n - 1;
    tree_t ** body = (tree_t **) ct_alloc(sizeof(tree_t *) * (size_t) nbody);
    for (int i = 0; i < nbody; i++) body[i] = t->c[i + 1];
    t->n = 1;
    for (int k = 0; k < nph; k++) ast_push(t, leaf_sval2(TT_VAR, ph[k]));
    for (int i = 0; i < nbody; i++) { for (int k = 0; k < nph; k++) rk_ph_rename(body[i], intern(ph[k] + 1), ph[k]); ast_push(t, body[i]); }
    t->v.ival = nph;
    rk_named_placeholders(t);
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
    for (int i = 0; i < nb; i++) {
        extern int rt_proc_is_registered(const char *);
        tree_t * blk = blks[i];
        char nm[64];
        do snprintf(nm, sizeof nm, "__blk_%d", ++g_lower.rk.rk_hoist_anon_blocks_blk_ctr);
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
                if (body && (topic & 2)) rk_topic_default((tree_t *) body);
            }
        }
        if (body) rk_arm_body((tree_t *) body, 0);
        if (body) { for (int k = 0; k < body->n; k++) { tree_t * st = body->c[k]; if (k == body->n - 1 && !(blk->slen & 64)) st = rk_tail_return(st); ast_push(sd, st); } ((tree_t *) body)->n = 0; }
        if (blk->slen & 64) sd->slen |= 64;
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
static void rk_ph_collect_vars(const tree_t * t, rk_ns_t * out) {
    if (!t) return;
    if (t->t == TT_VAR && t->v.sval) rk_ns_add(out, t->v.sval);
    for (int i = 0; i < t->n; i++) rk_ph_collect_vars(t->c[i], out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_ph_bare_asg(const tree_t * e) {
    return e && e->t == TT_ASSIGN && e->n == 2 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval && (e->c[0]->slen & 4) && e->c[1] &&
        (e->c[1]->t == TT_NUL || (e->c[1]->t == TT_FNC && e->c[1]->v.sval && !strcmp(e->c[1]->v.sval, "__rk_undef")));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_ph_bare_decl_hits(const tree_t * e, const rk_ns_t * set) {
    if (!e) return 0;
    if (rk_ph_bare_asg(e)) return rk_ns_has(set, e->c[0]->v.sval);
    if (e->t == TT_SEQ_EXPR && e->n >= 2 && e->c[0] && e->c[0]->t == TT_ASSIGN && e->c[0]->n == 2 && e->c[0]->c[0] && e->c[0]->c[0]->t == TT_VAR && e->c[0]->c[0]->v.sval &&
        !strncmp(e->c[0]->c[0]->v.sval, "__destr_", 8)) for (int i = 1; i < e->n; i++)
        if (e->c[i] && e->c[i]->t == TT_ASSIGN && e->c[i]->n == 2 && e->c[i]->c[0] && e->c[i]->c[0]->t == TT_VAR && e->c[i]->c[0]->v.sval && (e->c[i]->c[0]->slen & 4) &&
        rk_ns_has(set, e->c[i]->c[0]->v.sval)) return 1;
    return 0;
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
        {
            rk_ns_t seen = { 0 };
            for (int i = 0; i < n; i++) if (bd[i] && rk[i] <= 3) rk_ph_collect_vars(bd[i], &seen);
            for (int i = 0; i < n; i++) if (!bd[i] && rk[i] == 4) { tree_t * a = rk_ph_subj_attr(in[i]); if (a && rk_ph_bare_decl_hits(a->c[0], &seen)) rk[i] = 0; }
        }
        prog->n = 0;
        for (int rank = 0; rank <= 7; rank++) for (int i = 0; i < n; i++) { if (rk[i] != rank) continue; if (bd[i]) rk_ph_subj_attr(in[i])->c[0] = bd[i]; ast_push(prog, in[i]); }
    }
    for (int i = 0; i < prog->n; i++) { tree_t * a = rk_ph_subj_attr(prog->c[i]); if (a) rk_ph_inline(a->c[0]); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_ti(long long v);
static tree_t * rk_t2(tree_e k, tree_t * a, tree_t * b);
static int rk_subtree_has_any_suspend(const tree_t * n) {
    if (!n) return 0;
    if (n->t == TT_SUSPEND) return 1;
    if (n->t == TT_GATHER) return 0;
    for (int i = 0; i < n->n; i++) if (rk_subtree_has_any_suspend(n->c[i])) return 1;
    return 0;
}
static int rk_subtree_has_nested_suspend(const tree_t * n) {
    if (!n) return 0;
    if (n->t == TT_GATHER) return 0;
    if (n->t == TT_ANON_BLOCK || n->t == TT_SUB_DECL) return rk_subtree_has_any_suspend(n);
    for (int i = 0; i < n->n; i++) if (rk_subtree_has_nested_suspend(n->c[i])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_proc_is_generator(const char * name) {
    if (!name) return 0;
    for (int i = 0; i < g_stage2.proc_count; i++) if (g_stage2.proc_table[i].name && !strcmp(g_stage2.proc_table[i].name, name)) return g_stage2.proc_table[i].is_generator;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_mark_generators(void) {
    for (int pi = 0; pi < g_stage2.proc_count; pi++) {
        const tree_t * proc = (const tree_t *) g_stage2.proc_table[pi].proc;
        if (!proc || proc->t != TT_SUB_DECL) continue;
        int np = (int) proc->v.ival;
        g_stage2.proc_table[pi].is_generator = (proc->slen & 64) != 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_gather_blockify(tree_t * t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_gather_blockify(t->c[i]);
    if (t->t == TT_GATHER && t->n >= 1 && t->c[0] && t->c[0]->t != TT_ANON_BLOCK) {
        tree_t * body = ast_node_new(TT_SEQ_EXPR);
        ast_push(body, t->c[0]);
        tree_t * blk = ast_node_new(TT_ANON_BLOCK);
        blk->line = t->line;
        ast_push(blk, body);
        t->c[0] = blk;
    }
    if (t->t == TT_GATHER && t->n >= 1 && t->c[0] && t->c[0]->t == TT_ANON_BLOCK) {
        t->c[0]->slen |= 64;
        for (int k = 0; k < t->c[0]->n; k++) if (rk_subtree_has_nested_suspend(t->c[0]->c[k])) t->slen |= 128;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_gath_var(const char * pfx, int id, int decl) {
    char nm[strlen(pfx) + 24];
    snprintf(nm, sizeof nm, "%s%d", pfx, id);
    tree_t * v = leaf_sval2(TT_VAR, intern(nm));
    if (decl) v->slen |= 4;
    return v;
}
static tree_t * rk_gath_asg(tree_t * lhs, tree_t * rhs) { tree_t * a = ast_node_new(TT_ASSIGN); ast_push(a, lhs); ast_push(a, rhs); return a; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_gather_pump(tree_t * gcall, tree_t * limit) {
    int id = ++g_lower.rk.rk_gather_ctr;
    tree_t * seq = ast_node_new(TT_SEQ_EXPR);
    seq->line = gcall->line;
    ast_push(seq, rk_gath_asg(rk_gath_var("@__pump", id, 1), rk_fnc2("__rk_undef", NULL, NULL)));
    if (limit) { ast_push(seq, rk_gath_asg(rk_gath_var("$__plim", id, 1), limit)); ast_push(seq, rk_gath_asg(rk_gath_var("$__pcnt", id, 1), rk_ti(0))); }
    tree_t * body = ast_node_new(TT_SEQ_EXPR);
    { tree_t * mc = ast_node_new(TT_METHCALL); ast_push(mc, rk_gath_var("@__pump", id, 0)); ast_push(mc, leaf_sval2(TT_QLIT, "push")); ast_push(mc, rk_gath_var("$__pv", id, 0)); ast_push(body, mc); }
    if (limit) {
        ast_push(body, rk_gath_asg(rk_gath_var("$__pcnt", id, 0), rk_t2(TT_ADD, rk_gath_var("$__pcnt", id, 0), rk_ti(1))));
        tree_t * brk = ast_node_new(TT_SEQ_EXPR);
        ast_push(brk, ast_node_new(TT_LOOP_BREAK));
        tree_t * cond = ast_node_new(TT_IF);
        ast_push(cond, rk_t2(TT_GE, rk_gath_var("$__pcnt", id, 0), rk_gath_var("$__plim", id, 0)));
        ast_push(cond, brk);
        ast_push(body, cond);
    }
    tree_t * it = ast_node_new(TT_ITERATE);
    { char vn[48]; snprintf(vn, sizeof vn, "$__pv%d", id); it->v.sval = (char *) intern(vn); }
    ast_push(it, gcall);
    tree_t * ev = ast_node_new(TT_EVERY);
    ev->line = gcall->line;
    ast_push(ev, it);
    ast_push(ev, body);
    ast_push(seq, ev);
    ast_push(seq, rk_fnc2("__rk_arr_values", rk_gath_var("@__pump", id, 0), NULL));
    return seq;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_gath_eager_meth(const char * m) {
    static const char ms[][12] = { "elems", "sum", "join", "list", "Array", "List", "Seq", "eager", "cache", "flat", "reverse", "sort", "min", "max", "minmax", "unique", "Str", "gist", "perl", "raku",
        "end", "keys", "values", "kv", "pairs", "antipairs", "pick", "roll", "rotate", "squish", "tail", "Bool", "so", "not", "Numeric", "Int", "contains", "starts-with", "ends-with", "chars", "" };
    for (int i = 0; ms[i][0]; i++) if (!strcmp(ms[i], m)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_sq_seq1(tree_t * a) { tree_t * q = ast_node_new(TT_SEQ_EXPR); ast_push(q, a); return q; }
static tree_t * rk_sq_if(tree_t * cond, tree_t * a, tree_t * b) { tree_t * f = ast_node_new(TT_IF); ast_push(f, cond); ast_push(f, rk_sq_seq1(a)); if (b) ast_push(f, rk_sq_seq1(b)); return f; }
static tree_t * rk_sq_take(tree_t * v) { tree_t * q = ast_node_new(TT_SUSPEND); ast_push(q, v); return q; }
static tree_t * rk_sq_invoke(tree_t * fnv, tree_t * a, tree_t * b, tree_t * c) {
    tree_t * iv = ast_node_new(TT_INVOKE);
    ast_push(iv, fnv);
    if (a) ast_push(iv, a);
    if (b) ast_push(iv, b);
    if (c) ast_push(iv, c);
    return iv;
}
static tree_t * rk_sq_fnc(const char * nm, tree_t * a, tree_t * b, tree_t * c, tree_t * d, tree_t * e) {
    tree_t * f = rk_fnc2(nm, a, b);
    if (c) ast_push(f, c);
    if (d) ast_push(f, d);
    if (e) ast_push(f, e);
    return f;
}
static tree_t * rk_sq_brk(void) { return ast_node_new(TT_LOOP_BREAK); }
static tree_t * rk_sq_at(tree_t * arr, long long i) { return rk_t2(TT_ARR_GET, arr, rk_ti(i)); }
static tree_t * rk_seq_to_gather(tree_t * t) {
    int excl = t->c[1] && t->c[1]->t == TT_ILIT && t->c[1]->v.ival;
    tree_t * seeds = t->c[2], * end = t->c[3];
    int id = ++g_lower.rk.rk_gather_ctr;
    tree_t * body = ast_node_new(TT_SEQ_EXPR);
    body->line = t->line;
    tree_t * sq = rk_fnc2("__rk_arr", NULL, NULL);
    tree_t * gen = NULL;
    if (seeds && seeds->t == TT_FNC && seeds->v.sval && !strcmp(seeds->v.sval, "__rk_arr")) {
        for (int i = 1; i < seeds->n; i++) { if (i == seeds->n - 1 && seeds->c[i] && seeds->c[i]->t == TT_ANON_BLOCK) gen = seeds->c[i]; else ast_push(sq, seeds->c[i]); }
    } else if (seeds && seeds->t == TT_ANON_BLOCK) gen = seeds;
    else if (seeds) ast_push(sq, seeds);
    ast_push(body, rk_gath_asg(rk_gath_var("@__sq", id, 1), sq));
    if (gen) {
        ast_push(body, rk_gath_asg(rk_gath_var("$__sgb", id, 1), gen));
        ast_push(body, rk_gath_asg(rk_gath_var("$__sga", id, 1), rk_fnc2("__rk_seq_arity", rk_gath_var("$__sgb", id, 0), NULL)));
    }
    int infinite = !end || (end->t == TT_VAR && end->v.sval && !strcmp(end->v.sval, "*"));
    tree_t * ev = end, * tail = NULL;
    if (end && end->t == TT_FNC && end->v.sval && !strcmp(end->v.sval, "__rk_arr") && end->n >= 2) { ev = end->c[1]; tail = end; }
    ast_push(body, rk_gath_asg(rk_gath_var("$__sev", id, 1), infinite ? ast_node_new(TT_NUL) : ev));
    ast_push(body, rk_gath_asg(rk_gath_var("@__sp", id, 1), rk_sq_fnc("__rk_seq_plan", rk_gath_var("@__sq", id, 0), rk_ti(gen ? 1 : 0), rk_gath_var("$__sev", id, 0), NULL, NULL)));
    ast_push(body, rk_gath_asg(rk_gath_var("$__sk", id, 1), rk_ti(0)));
    ast_push(body, rk_gath_asg(rk_gath_var("$__sp1", id, 1), ast_node_new(TT_NUL)));
    ast_push(body, rk_gath_asg(rk_gath_var("$__sp2", id, 1), ast_node_new(TT_NUL)));
    ast_push(body, rk_gath_asg(rk_gath_var("$__sp3", id, 1), ast_node_new(TT_NUL)));
    tree_t * lb = ast_node_new(TT_SEQ_EXPR);
    ast_push(lb, rk_gath_asg(rk_gath_var("$__sv", id, 1), ast_node_new(TT_NUL)));
    {
        tree_t * elems = ast_node_new(TT_METHCALL);
        ast_push(elems, rk_gath_var("@__sq", id, 0));
        ast_push(elems, leaf_sval2(TT_QLIT, "elems"));
        tree_t * from_seed = rk_gath_asg(rk_gath_var("$__sv", id, 0), rk_t2(TT_ARR_GET, rk_gath_var("@__sq", id, 0), rk_gath_var("$__sk", id, 0)));
        tree_t * next;
        if (gen) {
            tree_t * a0 = rk_gath_asg(rk_gath_var("$__sv", id, 0), rk_sq_invoke(rk_gath_var("$__sgb", id, 0), NULL, NULL, NULL));
            tree_t * a1 = rk_gath_asg(rk_gath_var("$__sv", id, 0), rk_sq_invoke(rk_gath_var("$__sgb", id, 0), rk_gath_var("$__sp1", id, 0), NULL, NULL));
            tree_t * a2 = rk_gath_asg(rk_gath_var("$__sv", id, 0), rk_sq_invoke(rk_gath_var("$__sgb", id, 0), rk_gath_var("$__sp2", id, 0), rk_gath_var("$__sp1", id, 0), NULL));
            tree_t * a3 =
                rk_gath_asg(rk_gath_var("$__sv", id, 0), rk_sq_invoke(rk_gath_var("$__sgb", id, 0), rk_gath_var("$__sp3", id, 0), rk_gath_var("$__sp2", id, 0), rk_gath_var("$__sp1", id, 0)));
            next =
                rk_sq_if(rk_t2(TT_EQ, rk_gath_var("$__sga", id, 0), rk_ti(0)), a0,
                rk_sq_if(rk_t2(TT_EQ, rk_gath_var("$__sga", id, 0), rk_ti(1)), a1, rk_sq_if(rk_t2(TT_EQ, rk_gath_var("$__sga", id, 0), rk_ti(2)), a2, a3)));
        } else next =
            rk_gath_asg(rk_gath_var("$__sv", id, 0),
            rk_sq_fnc("__rk_seq_next", rk_sq_at(rk_gath_var("@__sp", id, 0), 0), rk_sq_at(rk_gath_var("@__sp", id, 0), 1), rk_gath_var("$__sp1", id, 0), NULL, NULL));
        ast_push(lb, rk_sq_if(rk_t2(TT_LT, rk_gath_var("$__sk", id, 0), elems), from_seed, next));
    }
    if (!infinite) {
        ast_push(lb,
            rk_gath_asg(rk_gath_var("$__sst", id, 1),
            rk_sq_fnc("__rk_seq_stop", rk_gath_var("$__sv", id, 0), rk_gath_var("$__sev", id, 0), rk_sq_at(rk_gath_var("@__sp", id, 0), 0), rk_sq_at(rk_gath_var("@__sp", id, 0), 2),
            rk_ti(gen ? 1 : 0))));
        ast_push(lb,
            rk_sq_if(rk_t2(TT_EQ, rk_gath_var("$__sst", id, 0), rk_ti(4)),
            rk_sq_if(rk_sq_invoke(rk_gath_var("$__sev", id, 0), rk_gath_var("$__sv", id, 0), NULL, NULL), rk_gath_asg(rk_gath_var("$__sst", id, 0), rk_ti(1)),
            rk_gath_asg(rk_gath_var("$__sst", id, 0), rk_ti(0))), NULL));
        ast_push(lb, rk_sq_if(rk_t2(TT_EQ, rk_gath_var("$__sst", id, 0), rk_ti(2)), rk_sq_brk(), NULL));
        {
            tree_t * q = ast_node_new(TT_SEQ_EXPR);
            if (!excl) ast_push(q, rk_sq_take(rk_gath_var("$__sev", id, 0)));
            ast_push(q, rk_sq_brk());
            tree_t * f = ast_node_new(TT_IF);
            ast_push(f, rk_t2(TT_EQ, rk_gath_var("$__sst", id, 0), rk_ti(3)));
            ast_push(f, q);
            ast_push(lb, f);
        }
        {
            tree_t * q = ast_node_new(TT_SEQ_EXPR);
            if (!excl) ast_push(q, rk_sq_take(rk_gath_var("$__sv", id, 0)));
            ast_push(q, rk_sq_brk());
            tree_t * f = ast_node_new(TT_IF);
            ast_push(f, rk_t2(TT_EQ, rk_gath_var("$__sst", id, 0), rk_ti(1)));
            ast_push(f, q);
            ast_push(lb, f);
        }
    }
    ast_push(lb, rk_sq_take(rk_gath_var("$__sv", id, 0)));
    ast_push(lb, rk_gath_asg(rk_gath_var("$__sp3", id, 0), rk_gath_var("$__sp2", id, 0)));
    ast_push(lb, rk_gath_asg(rk_gath_var("$__sp2", id, 0), rk_gath_var("$__sp1", id, 0)));
    ast_push(lb, rk_gath_asg(rk_gath_var("$__sp1", id, 0), rk_gath_var("$__sv", id, 0)));
    ast_push(lb, rk_gath_asg(rk_gath_var("$__sk", id, 0), rk_t2(TT_ADD, rk_gath_var("$__sk", id, 0), rk_ti(1))));
    { tree_t * loop = ast_node_new(TT_WHILE); ast_push(loop, rk_ti(1)); ast_push(loop, lb); ast_push(body, loop); }
    if (tail) for (int i = 2; i < tail->n; i++) ast_push(body, rk_sq_take(tail->c[i]));
    tree_t * g = ast_node_new(TT_GATHER);
    g->line = t->line;
    ast_push(g, body);
    if (infinite) g->slen |= 256;
    return g;
}
static void rk_return_outside_routine(tree_t * t, int in_routine) {
    if (!t) return;
    if (t->t == TT_SUB_DECL || (t->t == TT_ANON_BLOCK && t->slen == 1)) in_routine = 1;
    if (t->t == TT_RETURN && !in_routine) { t->t = TT_DIE; t->n = 0; ast_push(t, leaf_sval2(TT_QLIT, "Attempt to return outside of any Routine")); return; }
    for (int i = 0; i < t->n; i++) rk_return_outside_routine(t->c[i], in_routine);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_seq_desugar(tree_t * t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_seq_desugar(t->c[i]);
    for (int i = 0; i < t->n; i++) { tree_t * c = t->c[i]; if (c && c->t == TT_FNC && c->v.sval && !strcmp(c->v.sval, "__rk_seq") && c->n == 4) t->c[i] = rk_seq_to_gather(c); }
}
static void rk_lazy_stages(tree_t * t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_lazy_stages(t->c[i]);
    if (t->t != TT_METHCALL || t->n != 3 || !t->c[0] || t->c[0]->t != TT_GATHER || !t->c[1] || !t->c[1]->v.sval || !t->c[2] || t->c[2]->t != TT_ANON_BLOCK) return;
    const char * m = t->c[1]->v.sval;
    int k = !strcmp(m, "map") ? 1 : !strcmp(m, "grep") ? 2 : !strcmp(m, "first") ? 3 : 0;
    if (!k) return;
    int id = ++g_lower.rk.rk_gather_ctr;
    char vn[48];
    snprintf(vn, sizeof vn, "__lx%d", id);
    const char * lx = intern(vn);
    tree_t * it = ast_node_new(TT_ITERATE);
    it->v.sval = (char *) lx;
    ast_push(it, t->c[0]);
    tree_t * inv = rk_sq_invoke(t->c[2], leaf_sval2(TT_VAR, lx), NULL, NULL);
    tree_t * lb = ast_node_new(TT_SEQ_EXPR);
    if (k == 1) ast_push(lb, rk_sq_take(inv));
    else {
        tree_t * q = ast_node_new(TT_SEQ_EXPR);
        ast_push(q, rk_sq_take(leaf_sval2(TT_VAR, lx)));
        if (k == 3) ast_push(q, rk_sq_brk());
        tree_t * f = ast_node_new(TT_IF);
        ast_push(f, inv);
        ast_push(f, q);
        ast_push(lb, f);
    }
    tree_t * ev = ast_node_new(TT_EVERY);
    ev->line = t->line;
    ast_push(ev, it);
    ast_push(ev, lb);
    tree_t * g = ast_node_new(TT_GATHER);
    g->line = t->line;
    ast_push(g, rk_sq_seq1(ev));
    if (k != 3 && (t->c[0]->slen & 256)) g->slen |= 256;
    long long lz = (t->c[0]->t == TT_GATHER && t->c[0]->v.ival == 1) ? 1 : 0;
    if (k == 3) {
        t->t = TT_ARR_GET;
        t->n = 0;
        t->v.sval = NULL;
        ast_push(t, g);
        ast_push(t, rk_ti(0));
    } else {
        t->t = TT_GATHER;
        t->n = 0;
        t->v.sval = NULL;
        t->v.ival = lz;
        t->slen = g->slen;
        ast_push(t, g->c[0]);
    }
}
static tree_t * rk_gather_callnode(const tree_t * g) {
    const tree_t * c = g->c[0];
    const tree_t * cl = rk_closure_block(c) ? c : NULL;
    const tree_t * blk = cl ? cl->c[1] : c;
    if (!blk || blk->t != TT_ANON_BLOCK || !blk->v.sval) return NULL;
    tree_t * f = ast_node_new(TT_FNC);
    f->v.sval = blk->v.sval;
    f->line = g->line;
    f->slen |= 32;
    ast_push(f, leaf_sval2(TT_VAR, blk->v.sval));
    for (int i = 2; cl && i < cl->n; i++) ast_push(f, cl->c[i]);
    return f;
}
static tree_t * rk_gather_seqnode(const tree_t * g) {
    const tree_t * c = g->c[0];
    tree_t * cl = rk_closure_block(c) ? (tree_t *) c : NULL;
    if (!cl) { if (!c || c->t != TT_ANON_BLOCK || !c->v.sval) return NULL; cl = rk_fnc2("__blk_close", c, NULL); }
    tree_t * q = rk_fnc2("__rk_seq_new", cl, (g->v.ival == 1 || (g->slen & 256)) ? rk_ti(1) : NULL);
    q->line = g->line;
    return q;
}
static void rk_gather_consumers(tree_t * t, tree_t * parent, int idx) {
    if (!t) return;
    if (t->t == TT_GATHER && t->n >= 1 && t->c[0] && parent) {
        tree_t * rep = NULL;
        int dyn = (t->slen & 128) != 0, inf = (t->slen & 256) != 0 || t->v.ival == 1;
        tree_t * call = dyn ? NULL : rk_gather_callnode(t);
        if (call && (parent->t == TT_ITERATE || ((parent->t == TT_MAP || parent->t == TT_GREP) && idx == 1))) rep = call;
        else if (call && !inf && parent->t == TT_ASSIGN && idx == 1 && parent->c[0] && parent->c[0]->t == TT_VAR && parent->c[0]->v.sval && parent->c[0]->v.sval[0] == '@') rep =
            rk_gather_pump(call, NULL);
        else if (call && !inf && (parent->t == TT_SAY || parent->t == TT_PRINT)) rep = rk_gather_pump(call, NULL);
        else if (call && parent->t == TT_ARR_GET && idx == 0 && parent->n > 1 && parent->c[1] && parent->c[1]->t == TT_ILIT && parent->c[1]->v.ival >= 0) rep =
            rk_gather_pump(call, rk_ti(parent->c[1]->v.ival + 1));
        else if (call && parent->t == TT_METHCALL && idx == 0 && parent->n > 1 && parent->c[1] && parent->c[1]->v.sval) {
            const char * m = parent->c[1]->v.sval;
            if (!strcmp(m, "head") && parent->n > 2 && parent->c[2]) rep = rk_gather_pump(call, parent->c[2]);
            else if (!strcmp(m, "head")) rep = rk_gather_pump(call, rk_ti(1));
            else if (!inf && rk_gath_eager_meth(m)) rep = rk_gather_pump(call, NULL);
        }
        if (!rep) rep = rk_gather_seqnode(t);
        if (!rep) rk_refuse(t, "a gather whose body is not a hoisted block");
        parent->c[idx] = rep;
        t = rep;
    }
    for (int i = 0; i < t->n; i++) rk_gather_consumers(t->c[i], t, i);
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
static tree_t * rk_sort_helper(const char * hname, const char * cmp, int nref) {
    tree_t * sd = ast_node_new(TT_SUB_DECL);
    sd->v.ival = 1 + nref;
    ast_push(sd, rk_tv(hname));
    tree_t * ps = rk_tv("@src");
    ast_push(ps, leaf_sval2(TT_QLIT, "@"));
    ast_push(sd, ps);
    tree_t * cc = rk_fnc2(cmp, NULL, NULL);
    for (int i = 0; i < nref; i++) {
        char rn[24];
        snprintf(rn, sizeof rn, "$?cr%d", i);
        tree_t * rp = rk_tv(lp_strdup(rn));
        ast_push(rp, leaf_sval2(TT_QLIT, "@"));
        ast_push(sd, rp);
        ast_push(cc, rk_tv(lp_strdup(rn)));
    }
    ast_push(cc, rk_tarr("@a", "r"));
    ast_push(cc, rk_tarr("@a", "l"));
    ast_push(sd, rk_tset("@a", rk_tv("@src")));
    { tree_t * m = ast_node_new(TT_METHCALL); ast_push(m, rk_tv("@a")); ast_push(m, leaf_sval2(TT_QLIT, "elems")); ast_push(sd, rk_tset("n", m)); }
    ast_push(sd, rk_tset("@b", rk_fnc2("__rk_undef", NULL, NULL)));
    ast_push(sd, rk_tset("w", rk_ti(1)));
    tree_t * lt0 = rk_t2(TT_LT, cc, rk_ti(0));
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
static const tree_t * rk_new_sub(const tree_t * cdecl, const char * mn) {
    for (int j = 1; cdecl && j < cdecl->n; j++) {
        const tree_t * ch = cdecl->c[j];
        if (ch && ch->t == TT_SUB_DECL && ch->n > 0 && ch->c[0] && ch->c[0]->v.sval && !strcmp(ch->c[0]->v.sval, mn) && !rk_method_is_stub(ch)) return ch;
    }
    return NULL;
}
static const char * rk_new_parent(const tree_t * cdecl, int * bad) {
    const tree_t * tn = cdecl->c[0];
    const char * parent = NULL;
    for (int ti = 0; tn && ti < tn->n; ti++) { const char * s = tn->c[ti] ? tn->c[ti]->v.sval : NULL; if (!s || !s[0]) continue; if (s[0] != 'i' || parent) { *bad = 1; return NULL; } parent = s + 1; }
    return parent;
}
static int rk_new_chain_ok(const tree_t * prog, const char * cls, int depth, int * hooks) {
    const tree_t * d = rk_find_type_decl(prog, cls);
    if (!d || d->t != TT_CLASS_DECL || depth > prog->n) return 0;
    for (int j = 1; j < d->n; j++) {
        const tree_t * ch = d->c[j];
        const char * mn = (ch && ch->t == TT_SUB_DECL && ch->n > 0 && ch->c[0]) ? ch->c[0]->v.sval : NULL;
        if (mn && (!strcmp(mn, "new") || !strncmp(mn, "BUILD$", 6) || !strncmp(mn, "TWEAK$", 6) || !strncmp(mn, "new$", 4))) return 0;
    }
    if (rk_new_sub(d, "BUILD") || rk_new_sub(d, "TWEAK")) *hooks = 1;
    int bad = 0;
    const char * parent = rk_new_parent(d, &bad);
    if (bad) return 0;
    return parent ? rk_new_chain_ok(prog, parent, depth + 1, hooks) : 1;
}
static tree_t * rk_new_call(const char * proc, tree_t * self) { tree_t * f = ast_node_new(TT_FNC); f->v.sval = (char *) proc; ast_push(f, rk_tv(proc)); ast_push(f, self); return f; }
static void rk_new_hooks(const tree_t * prog, const char * cls, const char * hook, tree_t * sd, const tree_t * site, char ** pnames) {
    const tree_t * d = rk_find_type_decl(prog, cls);
    int bad = 0;
    const char * parent = rk_new_parent(d, &bad);
    if (parent) rk_new_hooks(prog, parent, hook, sd, site, pnames);
    const tree_t * h = rk_new_sub(d, hook);
    if (!h) return;
    char pn[fmt_len("%s__%s", cls, hook)];
    snprintf(pn, sizeof pn, "%s__%s", cls, hook);
    tree_t * f = rk_new_call(lp_strdup(pn), rk_tv("s"));
    if (!strcmp(hook, "BUILD")) {
        int bs = (int) h->v.ival;
        for (int p = 1; p < bs && p < h->n; p++) {
            const char * pk = (h->c[p] && h->c[p]->v.sval) ? h->c[p]->v.sval : NULL;
            if (!pk || !*pk) continue;
            tree_t * arg = NULL;
            for (int k = 0; 1 + 2 * k + 1 < site->n; k++) if (!strcmp(site->c[1 + 2 * k]->v.sval, pk)) arg = rk_tv(pnames[k]);
            ast_push(f, arg ? arg : rk_fnc2("__rk_undef", NULL, NULL));
        }
    }
    ast_push(sd, f);
}
static int rk_new_site_ok(const tree_t * t) {
    if (!t || t->t != TT_NEW || t->n < 1 || !t->c[0] || t->c[0]->t != TT_QLIT || !t->c[0]->v.sval || (t->n - 1) % 2) return 0;
    for (int k = 1; k < t->n; k += 2) if (!t->c[k] || t->c[k]->t != TT_QLIT || !t->c[k]->v.sval) return 0;
    return 1;
}
static void rk_new_build_walk(tree_t * prog, tree_t * t, int * seq) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_new_build_walk(prog, t->c[i], seq);
    if (!rk_new_site_ok(t)) return;
    const char * cls = t->c[0]->v.sval;
    int hooks = 0;
    if (!rk_new_chain_ok(prog, cls, 0, &hooks) || !hooks) return;
    int nkv = (t->n - 1) / 2;
    char hn[fmt_len("__rk_newp_%s_%d", cls, *seq)];
    snprintf(hn, sizeof hn, "__rk_newp_%s_%d", cls, (*seq)++);
    const char * hname = lp_strdup(hn);
    char * pnames[nkv > 0 ? nkv : 1];
    tree_t * sd = ast_node_new(TT_SUB_DECL);
    sd->v.ival = nkv;
    ast_push(sd, rk_tv(hname));
    for (int k = 0; k < nkv; k++) { char pn[fmt_len("__rk_nv%d", k)]; snprintf(pn, sizeof pn, "__rk_nv%d", k); pnames[k] = lp_strdup(pn); ast_push(sd, rk_tv(pnames[k])); }
    tree_t * al = ast_node_new(TT_FNC);
    al->v.sval = (char *) "__rk_new_alloc";
    ast_push(al, rk_tv("__rk_new_alloc"));
    ast_push(al, leaf_sval2(TT_QLIT, cls));
    for (int k = 0; k < nkv; k++) { ast_push(al, leaf_sval2(TT_QLIT, t->c[1 + 2 * k]->v.sval)); ast_push(al, rk_tv(pnames[k])); }
    ast_push(sd, rk_tset("s", al));
    rk_new_hooks(prog, cls, "BUILD", sd, t, pnames);
    ast_push(sd, rk_fnc2("__rk_new_check", leaf_sval2(TT_QLIT, cls), rk_tv("s")));
    rk_new_hooks(prog, cls, "TWEAK", sd, t, pnames);
    { tree_t * r = ast_node_new(TT_RETURN); ast_push(r, rk_fnc2("__rk_new_defaults", leaf_sval2(TT_QLIT, cls), rk_tv("s"))); ast_push(sd, r); }
    ast_push(prog, sd);
    tree_t * vals[nkv > 0 ? nkv : 1];
    for (int k = 0; k < nkv; k++) vals[k] = t->c[2 + 2 * k];
    t->t = TT_FNC;
    t->v.sval = (char *) hname;
    t->n = 0;
    ast_push(t, rk_tv(hname));
    for (int k = 0; k < nkv; k++) ast_push(t, vals[k]);
}
static void rk_smartmatch_topic_walk(tree_t * t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_smartmatch_topic_walk(t->c[i]);
    if (t->t != TT_FNC || !t->v.sval || strcmp(t->v.sval, "__rk_smartmatch") || t->n != 3) return;
    tree_t * r = t->c[2];
    if (!r || r->t != TT_METHCALL || r->n < 2 || !r->c[0] || r->c[0]->t != TT_VAR || !r->c[0]->v.sval || strcmp(r->c[0]->v.sval, "_")) return;
    char b[64];
    snprintf(b, sizeof b, "__smt%p", (const void *) t);
    const char * sv = ct_strdup(b);
    snprintf(b, sizeof b, "__smr%p", (const void *) t);
    const char * rs = ct_strdup(b);
    tree_t * lhs = t->c[1];
    tree_t * inner = rk_fnc2("__rk_smartmatch", rk_tv("_"), r);
    t->t = TT_SEQ_EXPR;
    t->v.sval = NULL;
    t->n = 0;
    ast_push(t, rk_gath_asg(rk_tv(sv), rk_tv("_")));
    ast_push(t, rk_gath_asg(rk_tv("_"), lhs));
    ast_push(t, rk_gath_asg(rk_tv(rs), inner));
    ast_push(t, rk_gath_asg(rk_tv("_"), rk_tv(sv)));
    ast_push(t, rk_tv(rs));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_smartmatch_code_walk(const tree_t * prog, tree_t * t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_smartmatch_code_walk(prog, t->c[i]);
    if (t->t != TT_FNC || !t->v.sval || strcmp(t->v.sval, "__rk_smartmatch") || t->n != 3) return;
    int ok = 0;
    tree_t * bc = rk_code_call1(prog, t->c[2], t->c[1], &ok);
    if (!ok) return;
    t->v.sval = (char *) "__rk_mkbool";
    t->c[0] = leaf_sval2(TT_VAR, "__rk_mkbool");
    t->c[1] = bc;
    t->n = 2;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_subst_code_walk(tree_t * t, int * seq) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_subst_code_walk(t->c[i], seq);
    if (t->t != TT_FNC || !t->v.sval || strcmp(t->v.sval, "re_subst") || t->n < 8) return;
    const tree_t * cl = rk_closure_block(t->c[3]) ? t->c[3] : NULL, * B = cl ? cl->c[1] : t->c[3];
    if (!B || B->t != TT_ANON_BLOCK || !B->v.sval) return;
    char sv[32];
    snprintf(sv, sizeof sv, "__rk_sst%d", (*seq)++);
    const char * st = lp_strdup(sv);
    tree_t * op = rk_fnc2("__rk_subst_open", t->c[1], t->c[2]);
    for (int i = 4; i < t->n; i++) ast_push(op, t->c[i]);
    tree_t * wh = ast_node_new(TT_WHILE);
    ast_push(wh, rk_fnc2("__rk_subst_next", rk_tv(st), NULL));
    ast_push(wh, rk_fnc2("__rk_subst_put", rk_tv(st), rk_fnc_cl(B->v.sval, cl, NULL, NULL)));
    tree_t * fin = rk_fnc2("__rk_subst_close", rk_tv(st), NULL);
    int line = t->line;
    t->t = TT_SEQ_EXPR;
    t->v.sval = NULL;
    t->n = 0;
    t->line = line;
    ast_push(t, rk_tset(st, op));
    ast_push(t, wh);
    ast_push(t, fin);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_parse_actions_walk(tree_t * t, int * seq) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_parse_actions_walk(t->c[i], seq);
    if (t->t != TT_METHCALL || t->n < 3 || !t->c[1] || !t->c[1]->v.sval || (strcmp(t->c[1]->v.sval, "parse") && strcmp(t->c[1]->v.sval, "subparse"))) return;
    int ai = -1;
    for (int i = 2; i < t->n; i++) {
        const tree_t * a = t->c[i];
        if (a && a->t == TT_FNC && a->v.sval && !strcmp(a->v.sval, "__rk_pair") && a->n == 3 && a->c[1] && a->c[1]->t == TT_QLIT && a->c[1]->v.sval && !strcmp(a->c[1]->v.sval, "actions")) ai = i;
    }
    int positional = ai < 0 && t->n == 4 && t->c[3] && !(t->c[3]->t == TT_FNC && t->c[3]->v.sval && !strcmp(t->c[3]->v.sval, "__rk_pair"));
    if (positional) ai = 3;
    if (ai < 0) return;
    char b1[32], b2[32], b3[32], b4[32];
    snprintf(b1, sizeof b1, "__rk_ga%d", *seq);
    snprintf(b2, sizeof b2, "__rk_gm%d", *seq);
    snprintf(b3, sizeof b3, "__rk_gp%d", *seq);
    snprintf(b4, sizeof b4, "__rk_gi%d", (*seq)++);
    const char * ga = lp_strdup(b1), * gm = lp_strdup(b2), * gp = lp_strdup(b3), * gi = lp_strdup(b4);
    tree_t * act = positional ? t->c[ai] : t->c[ai]->c[2];
    tree_t * pc = ast_node_new(TT_METHCALL);
    pc->line = t->line;
    for (int i = 0; i < t->n; i++) if (i != ai) ast_push(pc, t->c[i]);
    tree_t * call = rk_fnc2("__rk_named_call", rk_fnc2("__rk_arr_at", rk_tv(gp), rk_tv(gi)), rk_ti(2));
    ast_push(call, rk_tv(ga));
    ast_push(call, rk_fnc2("__rk_arr_at", rk_tv(gp), rk_t2(TT_ADD, rk_tv(gi), rk_ti(1))));
    tree_t * wh = ast_node_new(TT_WHILE);
    ast_push(wh, rk_t2(TT_LT, rk_tv(gi), rk_fnc2("elems", rk_tv(gp), NULL)));
    ast_push(wh, rk_tsq(call, rk_tinc(gi, rk_ti(2)), NULL));
    int line = t->line;
    t->t = TT_SEQ_EXPR;
    t->v.sval = NULL;
    t->n = 0;
    t->line = line;
    ast_push(t, rk_tset(ga, act));
    ast_push(t, rk_tset(gm, pc));
    ast_push(t, rk_tset(gp, rk_fnc2("__rk_actions_plan", rk_tv(gm), rk_tv(ga))));
    ast_push(t, rk_tset(gi, rk_ti(0)));
    ast_push(t, wh);
    ast_push(t, rk_tv(gm));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_make_walk(tree_t * t, const char * slash) {
    if (!t) return;
    if (t->t == TT_SUB_DECL) {
        slash = "/";
        for (int k = 0; k < (int) t->v.ival && 1 + k < t->n; k++) { const tree_t * pv = t->c[1 + k]; if (pv && pv->t == TT_VAR && pv->v.sval && !strcmp(pv->v.sval, "__slash")) slash = "__slash"; }
    }
    for (int i = 0; i < t->n; i++) rk_make_walk(t->c[i], slash);
    if (t->t != TT_FNC || !t->v.sval || strcmp(t->v.sval, "make") || t->n != 2 || rk_proc_known("make")) return;
    tree_t * arg = t->c[1];
    t->v.sval = (char *) "__rk_make";
    t->c[0] = leaf_sval2(TT_VAR, "__rk_make");
    t->c[1] = leaf_sval2(TT_VAR, slash);
    ast_push(t, arg);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_hyper_meth_walk(tree_t * t, int * seq) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_hyper_meth_walk(t->c[i], seq);
    if (t->t != TT_FNC || !t->v.sval || strcmp(t->v.sval, "__rk_hyper_meth") || t->n < 3 || !t->c[2] || t->c[2]->t != TT_QLIT || !t->c[2]->v.sval) return;
    const tree_t * recv = t->c[1];
    if (!recv || (recv->t == TT_VAR && recv->v.sval && recv->v.sval[0] == '%')) return;
    char hv[32];
    snprintf(hv, sizeof hv, "__rk_hy%d", (*seq)++);
    const char * h = lp_strdup(hv);
    tree_t * mc = ast_node_new(TT_METHCALL);
    mc->line = t->line;
    ast_push(mc, leaf_sval2(TT_VAR, intern("_")));
    for (int i = 2; i < t->n; i++) ast_push(mc, t->c[i]);
    tree_t * body = ast_node_new(TT_SEQ_EXPR);
    ast_push(body, mc);
    tree_t * blk = ast_node_new(TT_ANON_BLOCK);
    blk->line = t->line;
    ast_push(blk, body);
    tree_t * map = ast_node_new(TT_METHCALL);
    map->line = t->line;
    ast_push(map, rk_tv(h));
    ast_push(map, leaf_sval2(TT_QLIT, "map"));
    ast_push(map, blk);
    tree_t * done = rk_fnc2("__rk_hyper_done", map, rk_tv(h));
    tree_t * as = rk_tset(h, (tree_t *) recv);
    int line = t->line;
    t->t = TT_SEQ_EXPR;
    t->v.sval = NULL;
    t->n = 0;
    t->line = line;
    ast_push(t, as);
    ast_push(t, done);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_subtest_walk(tree_t * t, int * seq) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_subtest_walk(t->c[i], seq);
    if (t->t != TT_FNC || !t->v.sval || strcmp(t->v.sval, "__rk_test_subtest")) return;
    int bi = -1, ni = -1;
    for (int i = 1; i < t->n; i++) { const tree_t * a = t->c[i]; if (a && ((a->t == TT_ANON_BLOCK && a->v.sval) || rk_closure_block(a))) { if (bi >= 0) return; bi = i; } else if (ni < 0) ni = i; }
    if (bi < 0) return;
    const tree_t * cl = rk_closure_block(t->c[bi]) ? t->c[bi] : NULL;
    const char * bn = cl ? cl->c[1]->v.sval : t->c[bi]->v.sval;
    char sv[32];
    snprintf(sv, sizeof sv, "__rk_st%d", (*seq)++);
    const char * st = lp_strdup(sv);
    tree_t * open = rk_tset(st, rk_fnc2("__rk_test_subtest_open", ni >= 0 ? t->c[ni] : NULL, NULL));
    tree_t * body = rk_fnc_cl(bn, cl, NULL, NULL), * close = rk_fnc2("__rk_test_subtest_close", rk_tv(st), NULL);
    t->t = TT_SEQ_EXPR;
    t->v.sval = NULL;
    t->n = 0;
    ast_push(t, open);
    ast_push(t, body);
    ast_push(t, close);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_sort_cmp_walk(tree_t * prog, tree_t * t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_sort_cmp_walk(prog, t->c[i]);
    if (t->t != TT_METHCALL || t->n != 3 || !t->c[1] || !t->c[1]->v.sval || strcmp(t->c[1]->v.sval, "sort") || !t->c[2] || !t->c[0]) return;
    const tree_t * cl = rk_closure_block(t->c[2]) ? t->c[2] : NULL, * B = cl ? cl->c[1] : t->c[2];
    if (B->t != TT_ANON_BLOCK || !B->v.sval) return;
    if (t->c[0]->t == TT_VAR && t->c[0]->v.sval && t->c[0]->v.sval[0] == '%') return;
    int nref = cl ? cl->n - 2 : 0;
    const tree_t * bd = rk_block_decl(prog, B->v.sval);
    if (!bd || bd->v.ival != 2 + nref) return;
    char hn[strlen(B->v.sval) + 16];
    snprintf(hn, sizeof hn, "__rk_sortc_%s", B->v.sval);
    const char * hname = lp_strdup(hn);
    if (!rk_block_decl(prog, hname)) ast_push(prog, rk_sort_helper(hname, B->v.sval, nref));
    tree_t * recv = t->c[0];
    t->t = TT_FNC;
    t->v.sval = (char *) hname;
    t->n = 0;
    ast_push(t, rk_tv(hname));
    ast_push(t, recv);
    for (int i = 2; cl && i < cl->n; i++) ast_push(t, cl->c[i]);
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
static int rk_is_type_decl(const tree_t * d) { return d && (d->t == TT_CLASS_DECL || d->t == TT_ROLE_DECL || d->t == TT_GRAMMAR_DECL); }
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
            char buf[(nm ? strlen(nm) : 0) + 32];
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
            int sig = ch->t == TT_ARR_DECL ? '@' : ch->t == TT_HASH_DECL ? '%' :
                (ch->t == TT_HAS_DECL && ch->c[0]->t != TT_ILIT && ch->c[0]->t != TT_QLIT && !(ch->c[0]->t == TT_FLIT && ch->c[0]->n == 0)) ? '$' : 0;
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
static tree_t * rk_range_call_of(tree_t * to, const char * fn, int flagged) {
    tree_t * w = ast_node_new(TT_FNC);
    w->line = to->line;
    w->v.sval = (char *) fn;
    ast_push(w, leaf_sval2(TT_VAR, fn));
    ast_push(w, to->c[0]);
    ast_push(w, to->c[1]);
    if (flagged || to->v.ival) { tree_t * fl = ast_node_new(TT_ILIT); fl->v.ival = to->v.ival; ast_push(w, fl); }
    return w;
}
static tree_t * rk_range_val_of(tree_t * to) { return rk_range_call_of(to, "__rk_range_val", 1); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_range_method_name(const char * m) {
    static const char * const names[] = { "WHAT", "raku", "perl", "gist", "min", "max", "minmax", "excludes-min", "excludes-max", "bounds", NULL };
    for (int i = 0; m && names[i]; i++) if (!strcmp(m, names[i])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_user_terms_find(const tree_t * t, int * declared) {
    if (!t) return;
    if (t->t == TT_ASSIGN && t->n >= 1 && t->c[0] && t->c[0]->t == TT_VAR && (t->c[0]->slen & 1) && t->c[0]->v.sval) {
        for (int k = 0; rk_shadowable_terms[k]; k++) if (!strcmp(t->c[0]->v.sval, rk_shadowable_terms[k])) declared[k] = 1;
    }
    for (int i = 0; i < t->n; i++) rk_user_terms_find(t->c[i], declared);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_user_terms_unmark(tree_t * t, const int * declared) {
    if (!t) return;
    if (t->t == TT_VAR && (t->slen & 1) && t->v.sval) for (int k = 0; rk_shadowable_terms[k]; k++) if (declared[k] && !strcmp(t->v.sval, rk_shadowable_terms[k])) t->slen &= ~1;
    for (int i = 0; i < t->n; i++) rk_user_terms_unmark(t->c[i], declared);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_user_terms(tree_t * prog) { int declared[sizeof rk_shadowable_terms / sizeof *rk_shadowable_terms] = { 0 }; rk_user_terms_find(prog, declared); rk_user_terms_unmark(prog, declared); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_range_end_is_infinite(const tree_t * e) { return e && e->t == TT_VAR && e->v.sval && (!strcmp(e->v.sval, "Inf") || !strcmp(e->v.sval, "*") || !strcmp(e->v.sval, "∞")); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_infinite_range_gather(tree_t * to, int * seq) {
    char * k = (char *) ct_alloc(32);
    snprintf(k, 32, "__rk_inf%d", (*seq)++);
    tree_t * lo = to->c[0];
    if (to->v.ival & 1) lo = rk_t2(TT_ADD, lo, rk_ti(1));
    tree_t * step = ast_node_new(TT_SEQ_EXPR);
    tree_t * tk = ast_node_new(TT_SUSPEND);
    ast_push(tk, rk_tv(k));
    ast_push(step, tk);
    ast_push(step, rk_tset(k, rk_t2(TT_ADD, rk_tv(k), rk_ti(1))));
    tree_t * body = ast_node_new(TT_SEQ_EXPR);
    ast_push(body, rk_tset(k, lo));
    ast_push(body, rk_t2(TT_WHILE, rk_ti(1), step));
    tree_t * g = ast_node_new(TT_GATHER);
    g->line = to->line;
    g->v.ival = 1;
    ast_push(g, body);
    return g;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_range_iterates(const tree_t * t, int i) {
    static const char * const meths[] = { "head", "map", "grep", "first", "list", "Seq", "Array", "flat", "skip", "kv", "pairs", "values", "rotor", "batch", "toggle", "produce", "lazy", "cache",
        "iterator", NULL };
    if (t->t == TT_ITERATE || (t->t == TT_ARR_GET && i == 0)) return 1;
    if (t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, "__rk_arr_slice") && i == 1) return 1;
    if (t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, "__rk_range_arr")) return 0;
    if (t->t != TT_METHCALL || i != 0 || t->n < 2 || !t->c[1] || t->c[1]->t != TT_QLIT || !t->c[1]->v.sval) return 0;
    for (int k = 0; meths[k]; k++) if (!strcmp(t->c[1]->v.sval, meths[k])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_infinite_ranges(tree_t * t, int * seq, int insub) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) {
        tree_t * c = t->c[i];
        int sub = ((t->t == TT_ARR_GET || t->t == TT_ARR_SET || t->t == TT_HASH_GET || t->t == TT_HASH_SET) && i > 0) || (t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, "__rk_arr_slice") && i > 1);
        rk_infinite_ranges(c, seq, sub);
        if (!c) continue;
        int star = c->t == TT_VAR && c->v.sval && !strcmp(c->v.sval, "*");
        if (star && ((t->t == TT_TO && t->n == 2 && i == 1 && !insub) || (t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, "__rk_in_range") && i == 3))) {
            t->c[i] = rk_tv("Inf");
            t->c[i]->slen = 1;
            continue;
        }
        if (c->t == TT_FNC && c->v.sval && !strcmp(c->v.sval, "__rk_range_arr") && c->n >= 3 && rk_range_end_is_infinite(c->c[2]) && !rk_range_end_is_infinite(c->c[1])) {
            tree_t * to = rk_t2(TT_TO, c->c[1], c->c[2]);
            to->line = c->line;
            to->v.ival = (c->n >= 4 && c->c[3] && c->c[3]->t == TT_ILIT) ? c->c[3]->v.ival : 0;
            t->c[i] = rk_infinite_range_gather(to, seq);
            continue;
        }
        if (c->t != TT_TO || c->n != 2 || sub || !rk_range_end_is_infinite(c->c[1]) || rk_range_end_is_infinite(c->c[0]) || !rk_range_iterates(t, i)) continue;
        t->c[i] = rk_infinite_range_gather(c, seq);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_xx_value_is_pure(const tree_t * e) {
    if (!e) return 1;
    if (e->t == TT_QLIT || e->t == TT_ILIT || e->t == TT_FLIT || e->t == TT_NUL || e->t == TT_VAR) return 1;
    if (e->t == TT_SEQ_EXPR && e->n == 1) return rk_xx_value_is_pure(e->c[0]);
    if (e->t == TT_TO && e->n == 2) return rk_xx_value_is_pure(e->c[0]) && rk_xx_value_is_pure(e->c[1]);
    if (e->t != TT_FNC || !e->v.sval || strcmp(e->v.sval, "__rk_arr")) return 0;
    for (int i = 1; i < e->n; i++) if (!rk_xx_value_is_pure(e->c[i])) return 0;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_xx_thunks(tree_t * t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) {
        tree_t * c = t->c[i];
        rk_xx_thunks(c);
        if (!c || c->t != TT_FNC || c->n != 3 || !c->v.sval || strcmp(c->v.sval, "__rk_arr_xx")) continue;
        tree_t * cnt = c->c[2];
        if (rk_range_end_is_infinite(cnt)) {
            tree_t * tk = ast_node_new(TT_SUSPEND);
            ast_push(tk, c->c[1]);
            tree_t * g = ast_node_new(TT_GATHER);
            g->line = c->line;
            ast_push(g, rk_t2(TT_WHILE, rk_ti(1), tk));
            t->c[i] = g;
            continue;
        }
        if (!cnt || rk_xx_value_is_pure(c->c[1])) continue;
        tree_t * to = rk_t2(TT_TO, rk_ti(0), cnt);
        to->v.ival = 2;
        to->line = c->line;
        tree_t * body = ast_node_new(TT_SEQ_EXPR);
        ast_push(body, c->c[1]);
        tree_t * blk = ast_node_new(TT_ANON_BLOCK);
        ast_push(blk, body);
        tree_t * mc = ast_node_new(TT_METHCALL);
        mc->line = c->line;
        ast_push(mc, to);
        ast_push(mc, leaf_sval2(TT_QLIT, "map"));
        ast_push(mc, blk);
        t->c[i] = mc;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_array_single_arg(tree_t * t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_array_single_arg(t->c[i]);
    if (t->t != TT_FNC || t->n != 2 || !t->v.sval || strcmp(t->v.sval, "__rk_arr_lit") || !t->c[0] || t->c[0]->t != TT_VAR) return;
    const tree_t * x = t->c[1];
    if (!x || x->t == TT_QLIT || x->t == TT_ILIT || x->t == TT_FLIT || x->t == TT_NUL || x->t == TT_TO) return;
    if (x->t == TT_VAR && (!x->v.sval || x->v.sval[0] != '@')) return;
    t->v.sval = (char *) "__rk_arr_spread";
    t->c[0] = leaf_sval2(TT_VAR, "__rk_arr_spread");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_range_values(tree_t * t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_range_values(t->c[i]);
    if (t->t == TT_ASSIGN && t->n == 2 && t->c[0] && t->c[0]->t == TT_VAR && t->c[0]->v.sval && t->c[0]->v.sval[0] != '@' && t->c[0]->v.sval[0] != '%' && t->c[0]->v.sval[0] != '&' && t->c[1] &&
        t->c[1]->t == TT_TO && t->c[1]->n == 2) t->c[1] = rk_range_val_of(t->c[1]);
    if (t->t == TT_DECL && t->n >= 2 && t->c[t->n - 1] && t->c[t->n - 1]->t == TT_TO && t->c[t->n - 1]->n == 2 && t->c[t->n - 2] && t->c[t->n - 2]->t == TT_VAR && t->c[t->n - 2]->v.sval &&
        t->c[t->n - 2]->v.sval[0] != '@' && t->c[t->n - 2]->v.sval[0] != '%') t->c[t->n - 1] = rk_range_val_of(t->c[t->n - 1]);
    if (t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, "__rk_arr_lit")) for (int i = 1; i < t->n; i++) if (t->c[i] && t->c[i]->t == TT_TO && t->c[i]->n == 2) {
        if (t->n == 2) {
            tree_t * w = ast_node_new(TT_FNC);
            w->line = t->line;
            w->v.sval = (char *) "__rk_to_array";
            ast_push(w, leaf_sval2(TT_VAR, "__rk_to_array"));
            ast_push(w, rk_range_call_of(t->c[i], "__rk_range_arr", 0));
            *t = *w;
            break;
        }
        t->c[i] = rk_range_val_of(t->c[i]);
    }
    if (t->t == TT_FNC && t->v.sval && strncmp(t->v.sval, "__", 2)) for (int i = 1; i < t->n; i++) if (t->c[i] && t->c[i]->t == TT_TO && t->c[i]->n == 2) t->c[i] = rk_range_val_of(t->c[i]);
    if (t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, "__rk_arr")) for (int i = 1; i < t->n; i++) if (t->c[i] && t->c[i]->t == TT_TO && t->c[i]->n == 2) t->c[i] = rk_range_val_of(t->c[i]);
    if (t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, "__rk_arr_xx") && t->n == 3 && t->c[1] && t->c[1]->t == TT_TO && t->c[1]->n == 2) t->c[1] = rk_range_val_of(t->c[1]);
    if (t->t == TT_METHCALL) for (int i = 2; i < t->n; i++) if (t->c[i] && t->c[i]->t == TT_TO && t->c[i]->n == 2) t->c[i] = rk_range_val_of(t->c[i]);
    if (t->t == TT_INVOKE) for (int i = 1; i < t->n; i++) if (t->c[i] && t->c[i]->t == TT_TO && t->c[i]->n == 2) t->c[i] = rk_range_val_of(t->c[i]);
    if (t->t == TT_FNC && t->v.sval && (!strcmp(t->v.sval, "__rk_in_range") || !strcmp(t->v.sval, "__rk_numify") || !strcmp(t->v.sval, "__rk_smartmatch"))) {
        for (int i = 1; i < t->n && i < (strcmp(t->v.sval, "__rk_smartmatch") ? 2 : 3); i++) if (t->c[i] && t->c[i]->t == TT_TO && t->c[i]->n == 2) t->c[i] = rk_range_val_of(t->c[i]);
    }
    if (t->t == TT_SAY || t->t == TT_PRINT || t->t == TT_REVERSE || t->t == TT_SORT) for (int i = 0; i < t->n; i++) if (t->c[i] && t->c[i]->t == TT_TO && t->c[i]->n == 2) t->c[i] =
        rk_range_val_of(t->c[i]);
    if (t->t == TT_METHCALL && t->n >= 2 && t->c[0] && t->c[0]->t == TT_TO && t->c[0]->n == 2 && t->c[1] && t->c[1]->t == TT_QLIT && rk_range_method_name(t->c[1]->v.sval)) t->c[0] =
        rk_range_val_of(t->c[0]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { const char * name; const char * type; } rk_tv_t;
typedef struct { rk_tv_t * v; int n, cap; rk_ns_t classes; rk_ns_t subsets; int ssn; } rk_tvenv_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_tv_enforced(const char * ty, const rk_ns_t * cls) {
    if (!ty) return 0;
    if (!strcmp(ty, "Int") || !strcmp(ty, "Str") || !strcmp(ty, "Num") || !strcmp(ty, "Bool") || rk_is_class_name(ty)) return 1;
    for (int i = 0; cls && i < cls->n; i++) if (!strcmp(cls->v[i], ty)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_tv_push(rk_tvenv_t * e, const char * name, const char * type) {
    if (e->n >= e->cap) { e->cap = e->cap ? e->cap * 2 : 16; e->v = (rk_tv_t *) ct_grow(e->v, sizeof(rk_tv_t) * (size_t) e->cap); }
    e->v[e->n].name = name;
    e->v[e->n].type = type;
    e->n++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * rk_tv_find(rk_tvenv_t * e, const char * name) { for (int i = e->n - 1; i >= 0; i--) if (!strcmp(e->v[i].name, name)) return e->v[i].type; return NULL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_ss_collect(const tree_t * t, rk_ns_t * out);
static tree_t * rk_ss_call(const char * nm, tree_t * arg);
static tree_t * rk_tv_check(rk_tvenv_t * e, tree_t * val, const char * type, const char * name, int element);
static tree_t * rk_tv_check_plain(tree_t * val, const char * type, const char * name, int element) {
    tree_t * w = ast_node_new(TT_FNC);
    w->line = val->line;
    w->v.sval = (char *) "__rk_assign_check";
    ast_push(w, leaf_sval2(TT_VAR, "__rk_assign_check"));
    ast_push(w, leaf_sval2(TT_QLIT, type));
    ast_push(w, val);
    ast_push(w, leaf_sval2(TT_QLIT, name));
    tree_t * el = ast_node_new(TT_ILIT);
    el->v.ival = element;
    ast_push(w, el);
    return w;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_tv_check(rk_tvenv_t * e, tree_t * val, const char * type, const char * name, int element) {
    if (!rk_ns_has(&e->subsets, type)) return rk_tv_check_plain(val, type, name, element);
    char tn[24];
    snprintf(tn, sizeof tn, "__ssv%d", e->ssn++);
    const char * tmp = intern(tn);
    int line = val->line;
    tree_t * seq = ast_node_new(TT_SEQ_EXPR);
    seq->line = line;
    tree_t * as = ast_node_new(TT_ASSIGN);
    as->line = line;
    ast_push(as, leaf_sval2(TT_VAR, tmp));
    ast_push(as, val);
    ast_push(seq, as);
    tree_t * blk = ast_node_new(TT_SEQ_EXPR);
    ast_push(blk, rk_tv_check_plain(leaf_sval2(TT_VAR, tmp), type, name, element));
    tree_t * u = ast_node_new(TT_UNLESS);
    u->line = line;
    ast_push(u, rk_ss_call(type, leaf_sval2(TT_VAR, tmp)));
    ast_push(u, blk);
    ast_push(seq, u);
    ast_push(seq, leaf_sval2(TT_VAR, tmp));
    return seq;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_tv_walk(tree_t * t, rk_tvenv_t * e) {
    if (!t) return;
    int mark = e->n;
    int scope = t->t == TT_SUB_DECL || t->t == TT_ANON_BLOCK || t->t == TT_SEQ_EXPR || t->t == TT_CLASS_DECL;
    if (t->t == TT_SUB_DECL) for (int i = 1; i < t->n; i++) {
        tree_t * c = t->c[i];
        if (c && c->t == TT_VAR && c->v.sval && (c->n == 0 || (c->n >= 1 && c->c[0] && c->c[0]->t == TT_QLIT))) rk_tv_push(e, c->v.sval, NULL);
    }
    if (t->t == TT_ANON_BLOCK) for (int i = 1; i < t->n; i++) { tree_t * c = t->c[i]; if (c && c->t == TT_VAR && c->v.sval) rk_tv_push(e, c->v.sval, NULL); }
    for (int i = 0; i < t->n; i++) {
        tree_t * c = t->c[i];
        if (!c) continue;
        if (c->t == TT_DECL && c->n >= 2 && c->c[0] && c->c[0]->t == TT_VAR && c->c[1] && c->c[1]->t == TT_VAR && c->c[1]->v.sval && c->c[0]->v.sval && rk_tv_enforced(c->c[0]->v.sval, &e->classes)) {
            rk_tv_walk(c, e);
            const char * ty = c->c[0]->v.sval, * nm = c->c[1]->v.sval;
            rk_tv_push(e, nm, ty);
            if (c->n >= 3 && c->c[2] && nm[0] != '@' && nm[0] != '%') c->c[2] = rk_tv_check(e, c->c[2], ty, nm, 0);
            continue;
        }
        if (c->t == TT_DECL && c->n >= 1) { rk_tv_walk(c, e); tree_t * v = c->c[c->n >= 3 ? 1 : 0]; if (v && v->t == TT_VAR && v->v.sval) rk_tv_push(e, v->v.sval, NULL); continue; }
        rk_tv_walk(c, e);
        if (c->t == TT_ASSIGN && c->n == 2 && c->c[0] && c->c[0]->t == TT_VAR && c->c[0]->v.sval) {
            const char * nm = c->c[0]->v.sval, * ty = rk_tv_find(e, nm);
            if (ty && nm[0] != '@' && nm[0] != '%') c->c[1] = rk_tv_check(e, c->c[1], ty, nm, 0);
            else if (!ty && nm[0] != '@' && nm[0] != '%' && !rk_tv_find(e, nm)) rk_tv_push(e, nm, NULL);
        }
        if ((c->t == TT_ARR_SET || c->t == TT_HASH_SET) && c->n == 3 && c->c[0] && c->c[0]->t == TT_VAR && c->c[0]->v.sval) {
            const char * ty = rk_tv_find(e, c->c[0]->v.sval);
            if (ty) c->c[2] = rk_tv_check(e, c->c[2], ty, c->c[0]->v.sval, 1);
        }
        if (c->t == TT_METHCALL && c->n >= 3 && c->c[0] && c->c[0]->t == TT_VAR && c->c[0]->v.sval && c->c[0]->v.sval[0] == '@' && c->c[1] && c->c[1]->t == TT_QLIT && c->c[1]->v.sval &&
            (!strcmp(c->c[1]->v.sval, "push") || !strcmp(c->c[1]->v.sval, "append") || !strcmp(c->c[1]->v.sval, "unshift") || !strcmp(c->c[1]->v.sval, "prepend"))) {
            const char * ty = rk_tv_find(e, c->c[0]->v.sval);
            if (ty) for (int k = 2; k < c->n; k++) if (c->c[k] && c->c[k]->t != TT_VAR && c->c[k]->t != TT_FNC) c->c[k] = rk_tv_check(e, c->c[k], ty, c->c[0]->v.sval, 1);
        }
    }
    if (scope) e->n = mark;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_tv_classes(const tree_t * t, rk_ns_t * out) {
    if (!t) return;
    if ((t->t == TT_CLASS_DECL || t->t == TT_ROLE_DECL) && t->n > 0 && t->c[0] && t->c[0]->v.sval) rk_ns_add(out, t->c[0]->v.sval);
    for (int i = 0; i < t->n; i++) rk_tv_classes(t->c[i], out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_typed_vars(tree_t * prog) {
    rk_tvenv_t e = { 0 };
    rk_tv_classes(prog, &e.classes);
    rk_ss_collect(prog, &e.subsets);
    for (int i = 0; i < e.subsets.n; i++) rk_ns_add(&e.classes, e.subsets.v[i]);
    rk_tv_walk(prog, &e);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_range_end_nonint(const tree_t * e) {
    if (!e) return 0;
    if (e->t == TT_FLIT) return 1;
    if (e->t == TT_QLIT && e->v.sval) { char * ep; if (!*e->v.sval) return 1; strtoll(e->v.sval, &ep, 10); return *ep != 0; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_has_qlit_e(const tree_t * t) {
    if (!t) return 0;
    if (t->t == TT_QLIT && t->v.sval && (!strcmp(t->v.sval, "e") || !strncmp(t->v.sval, "e.", 2))) return 1;
    for (int i = 0; i < t->n; i++) if (rk_has_qlit_e(t->c[i])) return 1;
    return 0;
}
static int rk_uses_v6e(const tree_t * t) {
    if (!t) return 0;
    if (t->t == TT_USE_DECL && t->v.sval && !strcmp(t->v.sval, "v6") && rk_has_qlit_e(t)) return 1;
    for (int i = 0; i < t->n; i++) if (rk_uses_v6e(t->c[i])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_sprintf_names(tree_t * t, int v6e) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_sprintf_names(t->c[i], v6e);
    if (t->t == TT_FNC && t->v.sval && (!strcmp(t->v.sval, "sprintf") || !strcmp(t->v.sval, "printf")) && t->n >= 1 && !rk_proc_known(t->v.sval)) {
        const char * nn = !strcmp(t->v.sval, "sprintf") ? (v6e ? "__rk_sprintf" : "__rk_sprintf_d") : (v6e ? "__rk_printf" : "__rk_printf_d");
        t->v.sval = (char *) nn;
        if (t->c[0] && t->c[0]->t == TT_VAR) t->c[0]->v.sval = (char *) nn;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_nonint_ranges(tree_t * t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) {
        tree_t * c = t->c[i];
        rk_nonint_ranges(c);
        if (c && c->t == TT_TO && c->n == 2 && !c->v.ival && (rk_range_end_nonint(c->c[0]) || rk_range_end_nonint(c->c[1]))) {
            tree_t * w = ast_node_new(TT_FNC);
            w->line = c->line;
            w->v.sval = (char *) "__rk_range_arr";
            ast_push(w, leaf_sval2(TT_VAR, "__rk_range_arr"));
            ast_push(w, c->c[0]);
            ast_push(w, c->c[1]);
            t->c[i] = w;
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_exclusive_ranges(tree_t * t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) {
        tree_t * c = t->c[i];
        rk_exclusive_ranges(c);
        if (!c || c->t != TT_TO || c->n != 2 || !c->v.ival) continue;
        if (c->v.ival != 2 || rk_range_end_nonint(c->c[0]) || rk_range_end_nonint(c->c[1])) { t->c[i] = rk_range_call_of(c, "__rk_range_arr", 0); continue; }
        tree_t * hi = c->c[1];
        if (hi && hi->t == TT_ILIT) {
            tree_t * h = ast_node_new(TT_ILIT);
            h->line = hi->line;
            h->v.ival = hi->v.ival - 1;
            hi = h;
        } else {
            if (hi && hi->t == TT_VAR && hi->v.sval && hi->v.sval[0] == '@') { tree_t * el = ast_node_new(TT_METHCALL); ast_push(el, hi); ast_push(el, leaf_sval2(TT_QLIT, "elems")); hi = el; }
            tree_t * ex = ast_node_new(TT_FNC);
            ex->line = c->line;
            ex->v.sval = (char *) "__rk_exend";
            ast_push(ex, leaf_sval2(TT_VAR, "__rk_exend"));
            ast_push(ex, hi);
            hi = ex;
        }
        c->c[1] = hi;
        c->v.ival = 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_elem_store_value(tree_t * st) {
    if (!st || st->t != TT_ARR_SET || st->n != 3 || !st->c[0] || st->c[0]->t != TT_VAR || !st->c[1]) return st;
    tree_t * ix = st->c[1];
    if (ix->t != TT_ILIT && ix->t != TT_QLIT && !(ix->t == TT_VAR && ix->n == 0)) return st;
    tree_t * get = ast_node_new(TT_ARR_GET);
    get->line = st->line;
    ast_push(get, rk_clone_tree(st->c[0]));
    ast_push(get, rk_clone_tree(ix));
    tree_t * seq = ast_node_new(TT_SEQ_EXPR);
    seq->line = st->line;
    ast_push(seq, st);
    ast_push(seq, get);
    return seq;
}
static void rk_elem_store_values(tree_t * t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_elem_store_values(t->c[i]);
    int from = t->t == TT_METHCALL ? 2 : t->t == TT_FNC || t->t == TT_INVOKE ? 1 : t->t == TT_SAY || t->t == TT_PRINT || t->t == TT_RETURN ? 0 : t->t == TT_ASSIGN ? 1 : -1;
    if (from < 0) return;
    for (int i = from; i < t->n; i++) t->c[i] = rk_elem_store_value(t->c[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_is_scalar_name(const tree_t * v) {
    return v && v->t == TT_VAR && v->v.sval && !(v->slen & 1) && v->v.sval[0] != '@' && v->v.sval[0] != '%' && v->v.sval[0] != '&' && strncmp(v->v.sval, "__", 2);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_is_scalar_bind(const tree_t * t) { return t && t->t == TT_ASSIGN && t->v.ival == 2 && t->n > 1 && rk_is_scalar_name(t->c[0]); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_fnc_of(const char * fn, int line) { tree_t * f = ast_node_new(TT_FNC); f->line = line; f->v.sval = (char *) fn; ast_push(f, leaf_sval2(TT_VAR, fn)); return f; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_bind_ref(tree_t * r) {
    if (rk_is_scalar_name(r) && r->n == 0) { r->slen |= RK_VF_RAW; return r; }
    int el = r && r->t == TT_ARR_GET && r->n >= 2;
    tree_t * f = rk_fnc_of(el ? "__rk_elem_ref" : "__rk_box", r ? r->line : 0);
    if (el) { ast_push(f, r->c[0]); ast_push(f, r->c[1]); } else ast_push(f, r);
    return f;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_bind_refs(tree_t * t, int in_blk) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_bind_refs(t->c[i], in_blk || (t->c[i] && t->c[i]->t == TT_ANON_BLOCK));
    int ebind = t->t == TT_ARR_SET && t->v.ival == 2 && t->n == 3;
    if (in_blk) { if (rk_is_scalar_bind(t) || ebind) t->v.ival = 0; return; }
    if (rk_is_scalar_bind(t)) { t->c[0]->slen |= RK_VF_RAW; t->c[1] = rk_bind_ref(t->c[1]); return; }
    if (ebind) {
        tree_t * c0 = t->c[0], * c1 = t->c[1], * c2 = rk_bind_ref(t->c[2]);
        tree_t * f = rk_fnc_of("__rk_elem_bind", t->line);
        ast_push(f, c0);
        ast_push(f, c1);
        ast_push(f, c2);
        t->n = 0;
        if (c0->t == TT_VAR && c0->v.sval && c0->v.sval[0] == '@') {
            t->t = TT_ASSIGN;
            t->v.ival = 2;
            ast_push(t, rk_clone_tree(c0));
            ast_push(t, f);
        } else {
            t->t = TT_SEQ_EXPR;
            t->v.ival = 0;
            ast_push(t, f);
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_bind_names(const tree_t * t, rk_ns_t * out, const rk_ns_t * skip) {
    if (!t || t->t == TT_ANON_BLOCK || t->t == TT_SUB_DECL) return;
    if (t->t == TT_VAR && (t->slen & RK_VF_RAW) && t->v.sval && !(skip && rk_ns_has(skip, t->v.sval))) rk_ns_add(out, t->v.sval);
    for (int i = 0; i < t->n; i++) rk_bind_names(t->c[i], out, skip);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_bind_unbox(tree_t * t, const rk_ns_t * Bx) {
    if (!t || t->t == TT_ANON_BLOCK || t->t == TT_SUB_DECL) return;
    for (int i = 0; i < t->n; i++) rk_bind_unbox(t->c[i], Bx);
    if (t->t == TT_VAR && (t->slen & RK_VF_RAW)) t->slen &= ~RK_VF_BOXED;
    if (rk_is_scalar_bind(t) && !rk_ns_has(Bx, t->c[0]->v.sval)) { tree_t * d = rk_fnc_of("__rk_deref", t->line); ast_push(d, t->c[1]); t->c[1] = d; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_rw_shadows(const tree_t * t, const char * v) {
    if (!t) return 0;
    if (t->t == TT_ANON_BLOCK || t->t == TT_SUB_DECL) return 1;
    if (t->t == TT_EVERY && t->n > 0 && t->c[0] && t->c[0]->t == TT_ITERATE) { const char * iv = t->c[0]->v.sval ? t->c[0]->v.sval : "_"; return !strcmp(iv, v); }
    if (t->t == TT_FOR_RANGE && t->n > 0 && t->c[0] && t->c[0]->v.sval) return !strcmp(t->c[0]->v.sval, v);
    return t->t == TT_CASE && !strcmp(v, "_");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_rw_assigns(const tree_t * t, const char * v) {
    if (!t || rk_rw_shadows(t, v)) return 0;
    if (t->t == TT_ASSIGN && t->n > 1 && t->c[0] && t->c[0]->t == TT_VAR && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, v)) return 1;
    for (int i = 0; i < t->n; i++) if (rk_rw_assigns(t->c[i], v)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_rw_write_back(tree_t * t, const char * v, const char * arr, const char * k) {
    if (!t || rk_rw_shadows(t, v)) return;
    for (int i = 0; i < t->n; i++) {
        tree_t * c = t->c[i];
        rk_rw_write_back(c, v, arr, k);
        if (!(c && c->t == TT_ASSIGN && c->n > 1 && c->c[0] && c->c[0]->t == TT_VAR && c->c[0]->v.sval && !strcmp(c->c[0]->v.sval, v))) continue;
        tree_t * st = ast_node_new(TT_ARR_SET);
        st->line = c->line;
        ast_push(st, leaf_sval2(TT_VAR, arr));
        ast_push(st, leaf_sval2(TT_VAR, k));
        ast_push(st, leaf_sval2(TT_VAR, v));
        tree_t * sq = ast_node_new(TT_SEQ_EXPR);
        sq->line = c->line;
        ast_push(sq, c);
        ast_push(sq, st);
        ast_push(sq, leaf_sval2(TT_VAR, v));
        t->c[i] = sq;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_rw_loops(tree_t * t, int * seq) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_rw_loops(t->c[i], seq);
    if (t->t != TT_EVERY || t->n != 2 || !t->c[0] || t->c[0]->t != TT_ITERATE || t->c[0]->n != 1) return;
    tree_t * it = t->c[0], * src = it->c[0], * body = t->c[1];
    if (!src || src->t != TT_VAR || !src->v.sval || src->v.sval[0] != '@' || (it->slen & RK_PV_COPY)) return;
    const char * v = it->v.sval ? it->v.sval : "_";
    int asg = 0;
    for (int i = 0; body && i < body->n; i++) if (rk_rw_assigns(body->c[i], v)) asg = 1;
    if (body && body->t == TT_ASSIGN && rk_rw_assigns(body, v)) asg = 1;
    if (!asg) return;
    char kb[32];
    snprintf(kb, sizeof kb, "__rwk_%d", (*seq)++);
    const char * k = intern(kb);
    rk_rw_write_back(body, v, src->v.sval, k);
    tree_t * inc = ast_node_new(TT_ASSIGN);
    ast_push(inc, leaf_sval2(TT_VAR, k));
    tree_t * add = ast_node_new(TT_ADD);
    ast_push(add, leaf_sval2(TT_VAR, k));
    tree_t * one = ast_node_new(TT_ILIT);
    one->v.ival = 1;
    ast_push(add, one);
    ast_push(inc, add);
    tree_t * nb = ast_node_new(TT_SEQ_EXPR);
    nb->line = t->line;
    ast_push(nb, inc);
    ast_push(nb, body);
    t->c[1] = nb;
    tree_t * ev = ast_node_new(TT_EVERY);
    ev->line = t->line;
    ev->slen = t->slen;
    ast_push(ev, it);
    ast_push(ev, nb);
    tree_t * init = ast_node_new(TT_ASSIGN);
    ast_push(init, leaf_sval2(TT_VAR, k));
    tree_t * m1 = ast_node_new(TT_ILIT);
    m1->v.ival = -1;
    ast_push(init, m1);
    t->t = TT_SEQ_EXPR;
    t->n = 0;
    ast_push(t, init);
    ast_push(t, ev);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { const char ** nm; unsigned long long * mask; int * np; int n, cap; } rk_rwmap_t;
static void rk_rw_subs(const tree_t * t, rk_rwmap_t * m) {
    if (!t) return;
    if (t->t == TT_SUB_DECL && t->n > 0 && t->c[0] && t->c[0]->v.sval && !strchr(t->c[0]->v.sval, '$') && t->v.ival > 0 && t->v.ival < 64) {
        unsigned long long mk = 0;
        int np = (int) t->v.ival;
        for (int k = 0; k < np && 1 + k < t->n; k++) if (rk_param_is_rw(t->c[1 + k])) mk |= 1ULL << k;
        if (mk) {
            if (m->n >= m->cap) {
                m->cap = m->cap ? m->cap * 2 : 8;
                m->nm = (const char **) ct_grow((void *) m->nm, sizeof(const char *) * (size_t) m->cap);
                m->mask = (unsigned long long *) ct_grow((void *) m->mask, sizeof(unsigned long long) * (size_t) m->cap);
                m->np = (int *) ct_grow((void *) m->np, sizeof(int) * (size_t) m->cap);
            }
            m->nm[m->n] = t->c[0]->v.sval;
            m->mask[m->n] = mk;
            m->np[m->n++] = np;
        }
    }
    for (int i = 0; i < t->n; i++) rk_rw_subs(t->c[i], m);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_rw_calls(tree_t * t, const rk_rwmap_t * m, int in_blk) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_rw_calls(t->c[i], m, in_blk || (t->c[i] && t->c[i]->t == TT_ANON_BLOCK));
    if (in_blk || t->t != TT_FNC || !t->v.sval) return;
    for (int j = 0; j < m->n; j++) {
        if (strcmp(m->nm[j], t->v.sval) || t->n - 1 != m->np[j]) continue;
        for (int k = 0; k < m->np[j]; k++) {
            if (!(m->mask[j] & (1ULL << k))) continue;
            tree_t * a = t->c[1 + k];
            if (rk_is_scalar_name(a) && a->n == 0) a->slen |= RK_VF_RAW;
            else if (a && a->t == TT_ARR_GET && a->n >= 2) t->c[1 + k] = rk_bind_ref(a);
        }
        return;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_user_sub_named(const tree_t * prog, const char * nm) {
    size_t l = strlen(nm);
    for (int i = 0; prog && i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) { const tree_t * sub = stmt_subj(d); if (sub) d = sub; }
        const char * dn = (d && d->t == TT_SUB_DECL && d->n > 0 && d->c[0]) ? d->c[0]->v.sval : NULL;
        if (dn && !strncmp(dn, nm, l) && (dn[l] == 0 || dn[l] == '$')) return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_minmax_fns(tree_t * t, const tree_t * prog, int umax, int umin) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_minmax_fns(t->c[i], prog, umax, umin);
    if (t->t != TT_FNC || !t->v.sval || t->n < 2 || !t->c[0] || t->c[0]->t != TT_VAR) return;
    for (int i = 1; t->n > 2 && i < t->n; i++) {
        const tree_t * a = t->c[i];
        if (a && ((a->t == TT_VAR && a->v.sval && a->v.sval[0] == '@') || (a->t == TT_FNC && a->v.sval && !strncmp(a->v.sval, "__rk_arr", 8)))) return;
    }
    int mx = !strcmp(t->v.sval, "max"), mn = !strcmp(t->v.sval, "min");
    if ((!mx || umax) && (!mn || umin)) return;
    const char * nn = mx ? "__rk_arr_max" : "__rk_arr_min";
    t->v.sval = (char *) nn;
    t->c[0]->v.sval = (char *) nn;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_top_subj(tree_t * st) { return st && st->t == TT_STMT ? (tree_t *) stmt_subj(st) : st; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_role_nparams(const tree_t * rd) {
    int k = 0;
    for (int i = 0; rd && rd->n > 0 && rd->c[0] && i < rd->c[0]->n; i++) { const tree_t * q = rd->c[0]->c[i]; if (q && q->t == TT_QLIT && q->v.sval && q->v.sval[0] == 'r') k++; }
    return k;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_role_specialized(tree_t * rd, const char * spec, const tree_t * args) {
    int k = rk_role_nparams(rd);
    const tree_t * vals[k > 0 ? k : 1];
    if (!args) {
        tree_t * any = ast_node_new(TT_FNC);
        any->v.sval = (char *) "__rk_typeobj";
        ast_push(any, leaf_sval2(TT_VAR, "__rk_typeobj"));
        ast_push(any, leaf_sval2(TT_QLIT, "Any"));
        for (int i = 0; i < k; i++) vals[i] = any;
    } else if (k == 1) vals[0] = args;
    else if (args && args->t == TT_FNC && args->v.sval && !strcmp(args->v.sval, "__rk_arr") && args->n - 1 == k) for (int i = 0; i < k; i++) vals[i] = args->c[1 + i];
    else return NULL;
    const char * names[k > 0 ? k : 1];
    int ni = 0;
    for (int i = 0; i < rd->c[0]->n; i++) {
        const tree_t * q = rd->c[0]->c[i];
        if (!q || q->t != TT_QLIT || !q->v.sval || q->v.sval[0] != 'r') continue;
        const char * nm = q->v.sval + 1;
        names[ni++] = intern(!strncmp(nm, "::", 2) ? nm + 2 : nm);
    }
    tree_t * cl = spec ? rk_clone_tree(rd) : rd;
    if (spec) cl->c[0]->v.sval = (char *) intern(spec);
    cl->c[0]->n = 0;
    for (int j = 1; j < cl->n; j++) {
        tree_t * m = cl->c[j];
        if (!m || m->t != TT_SUB_DECL) continue;
        int at = m->v.ival > 0 ? (int) m->v.ival : 1;
        for (int x = k - 1; x >= 0; x--) {
            tree_t * v = leaf_sval2(TT_VAR, names[x]);
            v->slen |= 4;
            tree_t * as = ast_node_new(TT_ASSIGN);
            as->line = m->line;
            ast_push(as, v);
            ast_push(as, rk_clone_tree(vals[x]));
            ast_push(m, NULL);
            for (int y = m->n - 1; y > at; y--) m->c[y] = m->c[y - 1];
            m->c[at] = as;
        }
    }
    return cl;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_role_find(tree_t * t, const char * base) {
    if (!t) return NULL;
    tree_t * d = rk_top_subj(t);
    if (d && d->t == TT_ROLE_DECL && d->n > 0 && d->c[0] && d->c[0]->v.sval && !strcmp(d->c[0]->v.sval, base) && rk_role_nparams(d)) return d;
    for (int i = 0; i < t->n; i++) { tree_t * r = rk_role_find(t->c[i], base); if (r) return r; }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_role_spec_in(tree_t * t, tree_t * root, rk_ns_t * made) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) {
        tree_t * cd = rk_top_subj(t->c[i]);
        if (cd && cd != t->c[i] && cd->t != TT_CLASS_DECL) rk_role_spec_in(cd, root, made);
        if (!cd || cd->t != TT_CLASS_DECL || cd->n < 1 || !cd->c[0]) { if (cd == t->c[i]) rk_role_spec_in(t->c[i], root, made); continue; }
        for (int k = 1; k < cd->n; k++) rk_role_spec_in(cd->c[k], root, made);
        for (int tq = 0; tq < cd->c[0]->n; tq++) {
            tree_t * q = cd->c[0]->c[tq];
            if (!q || q->t != TT_QLIT || !q->v.sval || q->v.sval[0] != 'd' || q->n < 1) continue;
            const char * full = q->v.sval + 1;
            const char * br = strchr(full, '[');
            if (!br) continue;
            int seen = -1;
            for (int m = 0; m < made->n; m++) if (!strcmp(made->v[m], full)) seen = m;
            if (seen >= 0) { char sb[(size_t) (br - full) + 40]; snprintf(sb, sizeof sb, "d%.*s__rs%d", (int) (br - full), full, seen); q->v.sval = (char *) intern(sb); continue; }
            char base[br - full + 1];
            memcpy(base, full, (size_t) (br - full));
            base[br - full] = 0;
            tree_t * rd = rk_role_find(root, base);
            if (!rd) continue;
            char sn[(size_t) (br - full) + 40];
            snprintf(sn, sizeof sn, "%s__rs%d", base, made->n);
            const char * spec = intern(sn);
            tree_t * cl = rk_role_specialized(rd, spec, q->c[0]);
            if (!cl) continue;
            rk_ns_add(made, intern(full));
            { char dq[strlen(spec) + 2]; snprintf(dq, sizeof dq, "d%s", spec); q->v.sval = (char *) intern(dq); }
            tree_t * ins = cl;
            if (t->c[i]->t == TT_STMT) {
                extern tree_t *ast_stmt_new(tree_e kind);
                extern tree_t *ast_attr_int(const char *tag, int ival);
                extern tree_t *ast_attr_expr(const char *tag, tree_t *expr);
                ins = ast_stmt_new(TT_STMT);
                ast_push(ins, ast_attr_int(":line", 0));
                ast_push(ins, ast_attr_int(":stno", 0));
                ast_push(ins, ast_attr_expr(":subj", cl));
            }
            ast_push(t, NULL);
            for (int y = t->n - 1; y > i; y--) t->c[y] = t->c[y - 1];
            t->c[i] = ins;
            i++;
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_role_unbind(tree_t * t) {
    if (!t) return;
    tree_t * d = rk_top_subj(t);
    if (d && d->t == TT_ROLE_DECL && rk_role_nparams(d)) (void) rk_role_specialized(d, NULL, NULL);
    for (int i = 0; i < t->n; i++) rk_role_unbind(t->c[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_role_specialize(tree_t * prog) { if (!prog) return; rk_ns_t made = { 0 }; rk_role_spec_in(prog, prog, &made); rk_role_unbind(prog); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_ss_collect(const tree_t * t, rk_ns_t * out) {
    if (!t) return;
    if (t->t == TT_SUB_DECL && t->n > 0 && t->c[0] && t->c[0]->v.sval && !strncmp(t->c[0]->v.sval, "__ssf_", 6)) rk_ns_add(out, t->c[0]->v.sval + 6);
    for (int i = 0; i < t->n; i++) rk_ss_collect(t->c[i], out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_ss_call(const char * nm, tree_t * arg) {
    char b[strlen(nm) + 8];
    snprintf(b, sizeof b, "__ssf_%s", nm);
    tree_t * f = rk_fnc_of(intern(b), arg ? arg->line : 0);
    ast_push(f, arg);
    return f;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_ss_param_guard(const char * ty, const char * pn, int line) {
    tree_t * chk = rk_fnc_of("__param_check", line);
    ast_push(chk, leaf_sval2(TT_QLIT, ty));
    ast_push(chk, leaf_sval2(TT_VAR, pn));
    ast_push(chk, leaf_sval2(TT_QLIT, pn));
    tree_t * blk = ast_node_new(TT_SEQ_EXPR);
    ast_push(blk, chk);
    tree_t * u = ast_node_new(TT_UNLESS);
    u->line = line;
    ast_push(u, rk_ss_call(ty, leaf_sval2(TT_VAR, pn)));
    ast_push(u, blk);
    return u;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_ss_take_where(tree_t * pv) {
    for (int x = 0; x < pv->n; x++) {
        tree_t * c = pv->c[x];
        if (c && c->t == TT_FNC && c->v.sval && !strcmp(c->v.sval, "__rk_where") && c->n >= 2) {
            tree_t * wx = c->c[1];
            for (int y = x + 1; y < pv->n; y++) pv->c[y - 1] = pv->c[y];
            pv->n--;
            return wx;
        }
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_ss_rewrite(tree_t * t, const rk_ns_t * ss, int in_pkg) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_ss_rewrite(t->c[i], ss, t->t == TT_CLASS_DECL || t->t == TT_ROLE_DECL || t->t == TT_GRAMMAR_DECL);
    if (t->t == TT_FNC && t->n == 3 && t->v.sval && !strcmp(t->v.sval, "__rk_smartmatch") && t->c[2] && t->c[2]->t == TT_VAR && t->c[2]->n == 0 && t->c[2]->v.sval && rk_ns_has(ss, t->c[2]->v.sval)) {
        char b[strlen(t->c[2]->v.sval) + 8];
        snprintf(b, sizeof b, "__ssf_%s", t->c[2]->v.sval);
        const char * fn = intern(b);
        tree_t * arg = t->c[1];
        t->v.sval = (char *) fn;
        t->n = 0;
        ast_push(t, leaf_sval2(TT_VAR, fn));
        ast_push(t, arg);
        return;
    }
    if (t->t == TT_METHCALL && t->n == 3 && t->c[1] && t->c[1]->t == TT_QLIT && t->c[1]->v.sval && (!strcmp(t->c[1]->v.sval, "does") || !strcmp(t->c[1]->v.sval, "isa")) && t->c[2] &&
        (t->c[2]->t == TT_QLIT || (t->c[2]->t == TT_VAR && t->c[2]->n == 0)) && t->c[2]->v.sval && rk_ns_has(ss, t->c[2]->v.sval)) {
        char b[strlen(t->c[2]->v.sval) + 8];
        snprintf(b, sizeof b, "__ssf_%s", t->c[2]->v.sval);
        const char * fn = intern(b);
        tree_t * arg = t->c[0];
        t->t = TT_FNC;
        t->v.sval = (char *) fn;
        t->n = 0;
        ast_push(t, leaf_sval2(TT_VAR, fn));
        ast_push(t, arg);
        return;
    }
    if (t->t == TT_ANON_BLOCK) { for (int k = 1; k < t->n; k++) if (t->c[k] && t->c[k]->t == TT_VAR) rk_ss_take_where(t->c[k]); return; }
    if (t->t != TT_SUB_DECL || t->n < 1 || !t->c[0]) return;
    int multi = t->c[0]->v.sval && strchr(t->c[0]->v.sval, '$') != NULL;
    int ismeth = in_pkg, first = 1, last = ismeth ? (int) t->v.ival - 1 : (int) t->v.ival, body = last + 1;
    tree_t * guards[last >= first ? 3 * (last - first + 1) : 1];
    int ng = 0;
    for (int k = first; k <= last && k < t->n; k++) {
        tree_t * pv = t->c[k];
        if (pv && pv->t == TT_ASSIGN && pv->n) pv = pv->c[0];
        if (!pv || pv->t != TT_VAR || !pv->v.sval) continue;
        tree_t * wx = rk_ss_take_where(pv);
        if (multi) continue;
        if (wx && ng < 30) {
            tree_t * as = ast_node_new(TT_ASSIGN);
            as->line = t->line;
            ast_push(as, leaf_sval2(TT_VAR, "_"));
            ast_push(as, leaf_sval2(TT_VAR, pv->v.sval));
            guards[ng++] = as;
            tree_t * chk = rk_fnc_of("__param_check", t->line);
            ast_push(chk, leaf_sval2(TT_QLIT, "where"));
            ast_push(chk, leaf_sval2(TT_VAR, pv->v.sval));
            ast_push(chk, leaf_sval2(TT_QLIT, pv->v.sval));
            tree_t * blk = ast_node_new(TT_SEQ_EXPR);
            ast_push(blk, chk);
            tree_t * u = ast_node_new(TT_UNLESS);
            u->line = t->line;
            tree_t * sm = rk_fnc_of("__rk_smartmatch", t->line);
            ast_push(sm, leaf_sval2(TT_VAR, "_"));
            ast_push(sm, wx);
            ast_push(u, sm);
            ast_push(u, blk);
            guards[ng++] = u;
        }
        if (pv->v.sval[0] == '@' || pv->v.sval[0] == '%' || pv->n < 1 || !pv->c[0] || pv->c[0]->t != TT_QLIT || !pv->c[0]->v.sval || !rk_ns_has(ss, pv->c[0]->v.sval) || ng >= 31) continue;
        guards[ng++] = rk_ss_param_guard(pv->c[0]->v.sval, pv->v.sval, t->line);
        for (int x = 1; x < pv->n; x++) pv->c[x - 1] = pv->c[x];
        pv->n--;
    }
    if (!ng) return;
    for (int g = 0; g < ng; g++) { ast_push(t, NULL); for (int y = t->n - 1; y > body + g; y--) t->c[y] = t->c[y - 1]; t->c[body + g] = guards[g]; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { tree_t * sd; int rank, nreq, nmax; } rk_mc_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * rk_ss_base_of(const tree_t * t, const char * name) {
    if (!t) return NULL;
    if (t->t == TT_SUB_DECL && t->n > 0 && t->c[0] && t->c[0]->v.sval && !strncmp(t->c[0]->v.sval, "__ssf_", 6) && !strcmp(t->c[0]->v.sval + 6, name)) {
        for (int i = 1; i < t->n; i++) {
            const tree_t * c = t->c[i];
            if (c && c->t == TT_IF && c->n > 0 && c->c[0] && c->c[0]->t == TT_METHCALL && c->c[0]->n == 3 && c->c[0]->c[2] && c->c[0]->c[2]->v.sval) return c->c[0]->c[2]->v.sval;
        }
        return NULL;
    }
    for (int i = 0; i < t->n; i++) { const char * b = rk_ss_base_of(t->c[i], name); if (b) return b; }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_ss_rank(const tree_t * root, const rk_ns_t * ss, const char * ty, int depth) {
    if (!ty || !*ty || depth > 6) return 0;
    size_t bl = strcspn(ty, ":");
    char b[bl + 1];
    memcpy(b, ty, bl);
    b[bl] = 0;
    if (!strcmp(b, "Any") || !strcmp(b, "Mu")) return 0;
    if (!strcmp(b, "Cool")) return 1;
    if (!strcmp(b, "Numeric") || !strcmp(b, "Real") || !strcmp(b, "Stringy") || !strcmp(b, "Positional") || !strcmp(b, "Iterable") || !strcmp(b, "Associative") || !strcmp(b, "Callable")) return 2;
    if (rk_ns_has(ss, b)) return 10 + rk_ss_rank(root, ss, rk_ss_base_of(root, b), depth + 1);
    return 3;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * rk_ss_where_of(const tree_t * pv) {
    for (int x = 0; x < pv->n; x++) { const tree_t * c = pv->c[x]; if (c && c->t == TT_FNC && c->v.sval && !strcmp(c->v.sval, "__rk_where") && c->n >= 2) return c->c[1]; }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_ss_arg(int j) { tree_t * g = ast_node_new(TT_ARR_GET); ast_push(g, leaf_sval2(TT_VAR, "@__a")); tree_t * ix = ast_node_new(TT_ILIT); ix->v.ival = j; ast_push(g, ix); return g; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_ss_if(tree_t * cond, tree_t * inner) {
    tree_t * blk = ast_node_new(TT_SEQ_EXPR);
    ast_push(blk, inner);
    tree_t * f = ast_node_new(TT_IF);
    ast_push(f, cond);
    ast_push(f, blk);
    return f;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_ss_core_type(const char * b) {
    static const char * const core[] = { "Int", "Num", "Str", "Bool", "Rat", "FatRat", "List", "Array", "Hash", "Range", "Numeric", "Real", "Stringy", "Positional", "Iterable", "Seq", "Pair", "Map",
        "Set", "Bag", "Mix", NULL };
    for (int i = 0; core[i]; i++) if (!strcmp(core[i], b)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_ss_cand_chain(const rk_ns_t * ss, const rk_ns_t * classes, tree_t * sd, const char * cname, int c) {
    tree_t * call = rk_fnc_of(cname, sd->line);
    for (int j = 0; j < c; j++) ast_push(call, rk_ss_arg(j));
    tree_t * ret = ast_node_new(TT_RETURN);
    ast_push(ret, call);
    tree_t * inner = ret;
    for (int j = c - 1; j >= 0; j--) {
        tree_t * pv = sd->c[1 + j];
        if (pv && pv->t == TT_ASSIGN && pv->n) pv = pv->c[0];
        const tree_t * wx = rk_ss_where_of(pv);
        if (wx) {
            tree_t * as = ast_node_new(TT_ASSIGN);
            ast_push(as, leaf_sval2(TT_VAR, "_"));
            ast_push(as, leaf_sval2(TT_VAR, pv->v.sval));
            tree_t * sm = rk_fnc_of("__rk_smartmatch", sd->line);
            ast_push(sm, leaf_sval2(TT_VAR, "_"));
            ast_push(sm, rk_clone_tree(wx));
            tree_t * seq = ast_node_new(TT_SEQ_EXPR);
            ast_push(seq, as);
            ast_push(seq, rk_ss_if(sm, inner));
            inner = seq;
        }
        const char * ty = (pv->n > 0 && pv->c[0] && pv->c[0]->t == TT_QLIT) ? pv->c[0]->v.sval : NULL;
        if (ty && *ty) {
            size_t bl = strcspn(ty, ":");
            char b[bl + 1];
            memcpy(b, ty, bl);
            b[bl] = 0;
            tree_t * test = NULL;
            if (rk_ns_has(ss, b)) test = rk_ss_call(b, leaf_sval2(TT_VAR, pv->v.sval));
            else if (rk_ss_core_type(b) || rk_ns_has(classes, b)) {
                test = ast_node_new(TT_METHCALL);
                ast_push(test, leaf_sval2(TT_VAR, pv->v.sval));
                ast_push(test, leaf_sval2(TT_QLIT, "does"));
                ast_push(test, leaf_sval2(TT_QLIT, intern(b)));
            }
            if (test) inner = rk_ss_if(test, inner);
        }
    }
    tree_t * seq = ast_node_new(TT_SEQ_EXPR);
    for (int j = 0; j < c; j++) {
        tree_t * pv = sd->c[1 + j];
        if (pv && pv->t == TT_ASSIGN && pv->n) pv = pv->c[0];
        tree_t * as = ast_node_new(TT_ASSIGN);
        ast_push(as, leaf_sval2(TT_VAR, pv->v.sval));
        ast_push(as, rk_ss_arg(j));
        ast_push(seq, as);
    }
    ast_push(seq, inner);
    return seq;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_ss_multi_list(tree_t * t, const rk_ns_t * ss, const rk_ns_t * classes, tree_t * root, int * ctr) {
    int n = t->n;
    char * done = (char *) ct_alloc((size_t) n + 1);
    memset(done, 0, (size_t) n + 1);
    for (int i = 0; i < n; i++) {
        tree_t * d0 = rk_top_subj(t->c[i]);
        const char * n0 = (d0 && d0->t == TT_SUB_DECL && d0->n > 0 && d0->c[0]) ? d0->c[0]->v.sval : NULL;
        if (done[i] || !n0 || !strchr(n0, '$')) continue;
        size_t bl = strcspn(n0, "$");
        char base[bl + 1];
        memcpy(base, n0, bl);
        base[bl] = 0;
        int ncmax = n - i > 0 ? n - i : 1;
        rk_mc_t cands[ncmax];
        int idx[ncmax], nc = 0, valid = 1, constrained = 0;
        for (int k = i; k < n && valid; k++) {
            tree_t * dk = rk_top_subj(t->c[k]);
            const char * nk = (dk && dk->t == TT_SUB_DECL && dk->n > 0 && dk->c[0]) ? dk->c[0]->v.sval : NULL;
            if (!nk) continue;
            if (!strchr(nk, '$')) { if (!strcmp(nk, base)) valid = 0; continue; }
            if (strncmp(nk, base, bl) || nk[bl] != '$') continue;
            done[k] = 1;
            if (nc >= ncmax) { valid = 0; break; }
            rk_mc_t * m = &cands[nc];
            m->sd = dk;
            m->rank = 0;
            m->nreq = (int) dk->v.ival;
            m->nmax = (int) dk->v.ival;
            if (dk->v.ival < 0 || dk->v.ival > 12) { valid = 0; break; }
            int seen_opt = 0;
            for (int q = 1; q <= (int) dk->v.ival && q < dk->n; q++) {
                tree_t * pv = dk->c[q];
                if (pv && pv->t == TT_ASSIGN && pv->n) pv = pv->c[0];
                if (!pv || pv->t != TT_VAR || !pv->v.sval || pv->v.sval[0] == '@' || pv->v.sval[0] == '%' || pv->v.sval[0] == '&' || !pv->v.sval[0]) { valid = 0; break; }
                if ((pv->slen & RK_PV_NAMED) || (pv->n > 0 && pv->c[0] && pv->c[0]->t == TT_QLIT && pv->c[0]->v.sval && pv->c[0]->v.sval[0] == '*')) { valid = 0; break; }
                if (pv->slen & RK_PV_OPT) { seen_opt = 1; m->nreq--; } else if (seen_opt) { valid = 0; break; }
                const char * ty = (pv->n > 0 && pv->c[0] && pv->c[0]->t == TT_QLIT) ? pv->c[0]->v.sval : NULL;
                const tree_t * wx = rk_ss_where_of(pv);
                if (wx || (ty && rk_ns_has(ss, ty))) constrained = 1;
                m->rank += rk_ss_rank(root, ss, ty, 0) + (wx ? 5 : 0);
            }
            idx[nc++] = k;
        }
        if (!valid || !constrained || nc == 0) continue;
        for (int a = 1; a < nc; a++) { rk_mc_t key = cands[a]; int b = a - 1; while (b >= 0 && cands[b].rank < key.rank) { cands[b + 1] = cands[b]; b--; } cands[b + 1] = key; }
        char * names[ncmax];
        for (int a = 0; a < nc; a++) { char nb[bl + 24]; snprintf(nb, sizeof nb, "__mc%d_%s", (*ctr)++, base); names[a] = (char *) intern(nb); cands[a].sd->c[0]->v.sval = names[a]; }
        tree_t * disp = ast_node_new(TT_SUB_DECL);
        disp->v.ival = 1;
        disp->line = cands[0].sd->line;
        ast_push(disp, leaf_sval2(TT_VAR, intern(base)));
        tree_t * ap = leaf_sval2(TT_VAR, "@__a");
        ast_push(ap, leaf_sval2(TT_QLIT, "*@"));
        ast_push(disp, ap);
        int cmin = 99, cmax = 0;
        for (int a = 0; a < nc; a++) { if (cands[a].nreq < cmin) cmin = cands[a].nreq; if (cands[a].nmax > cmax) cmax = cands[a].nmax; }
        for (int c = cmin; c <= cmax; c++) {
            tree_t * grp = ast_node_new(TT_SEQ_EXPR);
            int any = 0;
            for (int a = 0; a < nc; a++) if (cands[a].nreq <= c && c <= cands[a].nmax) { ast_push(grp, rk_ss_cand_chain(ss, classes, cands[a].sd, names[a], c)); any = 1; }
            if (!any) continue;
            tree_t * eq = ast_node_new(TT_EQ);
            tree_t * el = ast_node_new(TT_METHCALL);
            ast_push(el, leaf_sval2(TT_VAR, "@__a"));
            ast_push(el, leaf_sval2(TT_QLIT, "elems"));
            tree_t * lit = ast_node_new(TT_ILIT);
            lit->v.ival = c;
            ast_push(eq, el);
            ast_push(eq, lit);
            tree_t * f = ast_node_new(TT_IF);
            ast_push(f, eq);
            ast_push(f, grp);
            ast_push(disp, f);
        }
        tree_t * df = ast_node_new(TT_DIE);
        char msg[bl + 64];
        snprintf(msg, sizeof msg, "Cannot resolve caller %s(...); none of these signatures matches", base);
        ast_push(df, leaf_sval2(TT_QLIT, intern(msg)));
        ast_push(disp, df);
        tree_t * ins = disp;
        if (t->c[idx[0]]->t == TT_STMT) {
            extern tree_t *ast_stmt_new(tree_e kind);
            extern tree_t *ast_attr_int(const char *tag, int ival);
            extern tree_t *ast_attr_expr(const char *tag, tree_t *expr);
            ins = ast_stmt_new(TT_STMT);
            ast_push(ins, ast_attr_int(":line", 0));
            ast_push(ins, ast_attr_int(":stno", 0));
            ast_push(ins, ast_attr_expr(":subj", disp));
        }
        ast_push(t, NULL);
        for (int y = t->n - 1; y > idx[0]; y--) t->c[y] = t->c[y - 1];
        t->c[idx[0]] = ins;
        n = t->n;
        char * nd = (char *) ct_alloc((size_t) n + 1);
        memset(nd, 0, (size_t) n + 1);
        for (int y = 0; y < n - 1 && y < (int) idx[0]; y++) nd[y] = done[y];
        for (int y = idx[0]; y < n - 1; y++) nd[y + 1] = done[y];
        done = nd;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_ss_multi(tree_t * t, const rk_ns_t * ss, const rk_ns_t * classes, tree_t * root, int * ctr) {
    if (!t) return;
    if (t->t != TT_CLASS_DECL && t->t != TT_ROLE_DECL && t->t != TT_GRAMMAR_DECL) rk_ss_multi_list(t, ss, classes, root, ctr);
    for (int i = 0; i < t->n; i++) rk_ss_multi(t->c[i], ss, classes, root, ctr);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_ss_is_check(const tree_t * d) { return d && d->t == TT_SUB_DECL && d->n > 0 && d->c[0] && d->c[0]->v.sval && !strncmp(d->c[0]->v.sval, "__ssf_", 6); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_ss_hoist(tree_t * list) {
    extern tree_t *ast_stmt_new(tree_e kind);
    extern tree_t *ast_attr_int(const char *tag, int ival);
    extern tree_t *ast_attr_expr(const char *tag, tree_t *expr);
    if (!list) return;
    for (int i = 0; i < list->n; i++) {
        tree_t * st = list->c[i];
        tree_t * pk = rk_top_subj(st);
        if (pk && (pk->t == TT_CLASS_DECL || pk->t == TT_ROLE_DECL || pk->t == TT_GRAMMAR_DECL)) {
            for (int k = 1; k < pk->n; k++) {
                if (!rk_ss_is_check(pk->c[k])) continue;
                tree_t * chk = pk->c[k];
                for (int y = k + 1; y < pk->n; y++) pk->c[y - 1] = pk->c[y];
                pk->n--;
                k--;
                tree_t * ins = ast_stmt_new(TT_STMT);
                ast_push(ins, ast_attr_int(":line", 0));
                ast_push(ins, ast_attr_int(":stno", 0));
                ast_push(ins, ast_attr_expr(":subj", chk));
                ast_push(list, NULL);
                for (int y = list->n - 1; y > i; y--) list->c[y] = list->c[y - 1];
                list->c[i] = ins;
                i++;
            }
        }
        if (pk && pk != st) rk_ss_hoist(pk);
        else if (st && st->t != TT_STMT) rk_ss_hoist(st);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static stage2_t *rk_stage2_core(const tree_t *prog, int reset_multi, int want_main) {
    {
        rk_ns_t ss = { 0 }, cls = { 0 };
        int mcn = 0;
        rk_ss_collect(prog, &ss);
        if (ss.n) rk_ss_hoist((tree_t *) prog);
        rk_tv_classes(prog, &cls);
        rk_ss_multi((tree_t *) prog, &ss, &cls, (tree_t *) prog, &mcn);
        rk_ss_rewrite((tree_t *) prog, &ss, 0);
    }
    rk_role_specialize((tree_t *) prog);
    rk_hoist_nested_types((tree_t *) prog);
    rk_hoist_nested_multis((tree_t *) prog);
    rk_class_default_tweaks((tree_t *) prog);
    rk_value_forms((tree_t *) prog);
    rk_tail_ifs((tree_t *) prog, 0);
    rk_typed_vars((tree_t *) prog);
    rk_sprintf_names((tree_t *) prog, rk_uses_v6e(prog));
    rk_user_terms((tree_t *) prog);
    { int iseq = 0; rk_infinite_ranges((tree_t *) prog, &iseq, 0); }
    rk_xx_thunks((tree_t *) prog);
    rk_array_single_arg((tree_t *) prog);
    rk_range_values((tree_t *) prog);
    rk_exclusive_ranges((tree_t *) prog);
    rk_bind_refs((tree_t *) prog, 0);
    rk_minmax_fns((tree_t *) prog, prog, rk_user_sub_named(prog, "max"), rk_user_sub_named(prog, "min"));
    { int rseq = 0; rk_rw_loops((tree_t *) prog, &rseq); }
    { rk_rwmap_t rm = { 0 }; rk_rw_subs(prog, &rm); if (rm.n) rk_rw_calls((tree_t *) prog, &rm, 0); }
    rk_elem_store_values((tree_t *) prog);
    rk_nonint_ranges((tree_t *) prog);
    rk_nested_elem_sets((tree_t *) prog);
    rk_place_phasers((tree_t *) prog);
    rk_rename_user_main((tree_t *) prog);
    rk_return_outside_routine((tree_t *) prog, 0);
    rk_seq_desugar((tree_t *) prog);
    rk_lazy_stages((tree_t *) prog);
    rk_gather_blockify((tree_t *) prog);
    rk_listops_to_methcalls((tree_t *) prog, 0);
    { int hseq = 0; rk_hyper_meth_walk((tree_t *) prog, &hseq); }
    rk_sub_placeholders((tree_t *) prog);
    rk_ph_alias((tree_t *) prog);
    rk_cap_file_scope((tree_t *) prog);
    rk_globalize_file_scope_writes((tree_t *) prog);
    rk_hoist_anon_blocks((tree_t *) prog);
    rk_gather_consumers((tree_t *) prog, NULL, 0);
    rk_sort_cmp_walk((tree_t *) prog, (tree_t *) prog);
    { int sseq = 0; rk_subtest_walk((tree_t *) prog, &sseq); }
    rk_smartmatch_topic_walk((tree_t *) prog);
    rk_smartmatch_code_walk(prog, (tree_t *) prog);
    { int sseq = 0; rk_subst_code_walk((tree_t *) prog, &sseq); }
    { int pseq = 0; rk_parse_actions_walk((tree_t *) prog, &pseq); }
    rk_make_walk((tree_t *) prog, "/");
    { int nseq = 0; rk_new_build_walk((tree_t *) prog, (tree_t *) prog, &nseq); }
    raku_register_program(&g_stage2, prog);
    rk_discover_grammars(prog);
    rk_register_classes(prog);
    if (reset_multi) g_lower.rk.multi_names.len = 0;
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
    {
        extern int rt_binop_overload_armed_by(const char *pn);
        g_stage2.rk_infix_armed = 0;
        for (int i = 0; i < g_stage2.proc_count; i++) if (rt_binop_overload_armed_by(g_stage2.proc_table[i].name)) { g_stage2.rk_infix_armed = 1; break; }
    }
    rk_mark_generators();
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
            if (!g_stage2.proc_table[pi].is_generator) { g_stage2.bbp.table[bb_idx]->entry_frame = 1; g_stage2.bbp.table[bb_idx]->smx = 1; }
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
                    if (!ch || ch->t == TT_SUB_DECL || ch->t == TT_REGEX_DECL) continue;
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
            if (!g_stage2.proc_table[pi].is_generator) { g_stage2.bbp.table[bi]->entry_frame = 1; g_stage2.bbp.table[bi]->smx = 1; }
            {
                const tree_t * pr = (const tree_t *) g_stage2.proc_table[pi].proc;
                g_stage2.bbp.table[bi]->block_args = (pr && pr->t == TT_SUB_DECL && !g_stage2.proc_table[pi].is_variadic && !g_stage2.proc_table[pi].named_rest) ? 1 : 0;
            }
        }
    }
    return &g_stage2;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_701_decls(const tree_t * t, const char * nm, const tree_t ** one) {
    if (!t) return 0;
    int n = 0;
    if (t->t == TT_SUB_DECL && t->n > 0 && t->c[0] && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, nm)) { n++; *one = t; }
    for (int i = 0; i < t->n; i++) n += rk_701_decls(t->c[i], nm, one);
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_701_has_default(const tree_t * sd) {
    for (int i = 1; i < sd->n; i++) {
        const tree_t * u = sd->c[i];
        if (!u || u->t != TT_UNLESS || u->n < 1 || !u->c[0] || u->c[0]->t != TT_METHCALL || u->c[0]->n < 2) continue;
        const tree_t * q = u->c[0]->c[1];
        if (u->c[0]->c[0] && u->c[0]->c[0]->t == TT_VAR && q && q->t == TT_QLIT && q->v.sval && !strcmp(q->v.sval, "defined")) return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_701_numeric_text(const char * s) { if (!s) return 0; char * e = NULL; if (*s == '+' || *s == '-') s++; if (!*s) return 0; strtod(s, &e); return e && e != s; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * rk_701_lit_type(const tree_t * a) { return !a ? "Any" : a->t == TT_ILIT ? "Int" : a->t == TT_QLIT ? "Str" : a->t == TT_FLIT ? "Num" : "Any"; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_701_never(const char * ty, const tree_t * a) {
    if (!ty || !a) return 0;
    int numeric = !strcmp(ty, "Int") || !strcmp(ty, "Num") || !strcmp(ty, "Rat") || !strcmp(ty, "Numeric");
    if (numeric && a->t == TT_QLIT && !rk_701_numeric_text(a->v.sval)) return 1;
    if (!strcmp(ty, "Str") && a->t == TT_ILIT) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * rk_701_find(const tree_t * prog, const tree_t * t, int line, int * at) {
    if (!t) return NULL;
    if (t->line > 0) line = t->line;
    int lit = 0;
    for (int k = 1; t->t == TT_FNC && k < t->n; k++) if (t->c[k] && (t->c[k]->t == TT_QLIT || t->c[k]->t == TT_ILIT)) lit = 1;
    if (lit && t->v.sval && strncmp(t->v.sval, "__", 2) && t->n >= 2 && t->c[0] && t->c[0]->t == TT_VAR && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, t->v.sval)) {
        const tree_t * sd = NULL;
        int nd = rk_701_decls(prog, t->v.sval, &sd);
        int np = 0, ok = nd == 1 && sd && !rk_701_has_default(sd);
        for (int k = 1; ok && k < sd->n && sd->c[k] && sd->c[k]->t == TT_VAR; k++) {
            const tree_t * pv = sd->c[k];
            if (!pv->v.sval || !*pv->v.sval || strchr("@%&", pv->v.sval[0])) ok = 0;
            if (pv->n >= 1 && pv->c[0] && pv->c[0]->v.sval && strpbrk(pv->c[0]->v.sval, ":()*|\\")) ok = 0;
            np++;
        }
        if (ok && np == t->n - 1) for (int k = 1; k <= np; k++) {
            const tree_t * pv = sd->c[k];
            const char * ty = (pv->n >= 1 && pv->c[0]) ? pv->c[0]->v.sval : NULL;
            if (rk_701_never(ty, t->c[k])) { *at = line; return t; }
        }
    }
    for (int i = 0; i < t->n; i++) { const tree_t * r = rk_701_find(prog, t->c[i], line, at); if (r) return r; }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int lower_raku_refuse(const tree_t * prog, const char * file) {
    int line = 0;
    const tree_t * call = rk_701_find(prog, prog, 0, &line);
    if (!call) return 0;
    const tree_t * sd = NULL;
    rk_701_decls(prog, call->v.sval, &sd);
    size_t an = 1, sn = 1;
    for (int k = 1; k < call->n; k++) an += strlen(rk_701_lit_type(call->c[k])) + 2;
    for (int k = 1; k < sd->n && sd->c[k] && sd->c[k]->t == TT_VAR; k++) {
        const tree_t * pv = sd->c[k];
        sn += strlen(pv->v.sval) + 4 + ((pv->n >= 1 && pv->c[0] && pv->c[0]->v.sval) ? strlen(pv->c[0]->v.sval) + 1 : 0);
    }
    char args[an], sig[sn];
    size_t al = 0, sl = 0;
    args[0] = sig[0] = 0;
    for (int k = 1; k < call->n; k++) al += (size_t) snprintf(args + al, an - al, "%s%s", k > 1 ? ", " : "", rk_701_lit_type(call->c[k]));
    for (int k = 1; k < sd->n && sd->c[k] && sd->c[k]->t == TT_VAR; k++) {
        const tree_t * pv = sd->c[k];
        const char * ty = (pv->n >= 1 && pv->c[0] && pv->c[0]->v.sval) ? pv->c[0]->v.sval : NULL;
        sl += (size_t) snprintf(sig + sl, sn - sl, "%s%s%s$%s", k > 1 ? ", " : "", ty ? ty : "", ty ? " " : "", pv->v.sval);
    }
    char msg[fmt_len("call %s(%s) can never succeed against the declared signature (%s)", call->v.sval, args, sig)];
    snprintf(msg, sizeof msg, "call %s(%s) can never succeed against the declared signature (%s)", call->v.sval, args, sig);
    extern void core_error_voice_at(int code, const char *msg, const char *file, long line, long stno);
    core_error_voice_at(701, msg, file, line, 0);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_source_terms(tree_t * t, const char * given, const char * absolute) {
    for (int i = 0; t && i < t->n; i++) {
        tree_t * c = t->c[i];
        rk_source_terms(c, given, absolute);
        if (!c || c->t != TT_VAR || !c->v.sval) continue;
        int prog = !strcmp(c->v.sval, "*PROGRAM"), name = !strcmp(c->v.sval, "*PROGRAM-NAME"), file = !strcmp(c->v.sval, "?FILE");
        if (!prog && !name && !file) continue;
        tree_t * f = ast_node_new(TT_FNC);
        f->line = c->line;
        f->v.sval = (char *) "__rk_pre_src";
        ast_push(f, leaf_sval2(TT_VAR, "__rk_pre_src"));
        ast_push(f, leaf_sval2(TT_QLIT, prog ? "*PROGRAM" : "*PROGRAM-NAME"));
        ast_push(f, leaf_sval2(TT_QLIT, file ? absolute : given));
        t->c[i] = f;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void lower_raku_source(tree_t * prog, const char * file) {
    if (!prog || !file || !*file) return;
    const char * absolute = file;
    if (file[0] != '/') {
        char * cwd = NULL;
        for (size_t n = 256; n < ((size_t) 1 << 24) && !cwd; n *= 2) { char * b = (char *) ct_alloc(n); if (getcwd(b, n)) cwd = b; else ct_drop(b); }
        if (cwd) {
            const char * rel = (file[0] == '.' && file[1] == '/') ? file + 2 : file;
            char joined[fmt_len("%s/%s", cwd, rel)];
            snprintf(joined, sizeof joined, "%s/%s", cwd, rel);
            absolute = lp_strdup(joined);
            ct_drop(cwd);
        }
    }
    rk_source_terms(prog, lp_strdup(file), absolute);
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
