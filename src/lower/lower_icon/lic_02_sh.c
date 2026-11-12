#include "lower/lower_icon/lic_head.h"
#include "lower/lower_icon/lic_01_sh.h"
#include "lower/lower_icon/lic_02_sh.h"
#include "lower/lower_icon/lic_03_sh.h"
#include "lower/lower_icon/lic_04_sh.h"
static int pm_const_int(const tree_t * a, int64_t * v);
static rx_t * rx_regex(rxs_t * st);
static tree_t * rx_tr(const rx_t * r, int line);
tree_t * icn_variable_lit_tree(const char * nm) { tree_t * v = ast_node_new(nm[0] == '&' ? TT_KEYWORD : TT_VAR); v->v.sval = (char *) nm; return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t * pm_node(tree_e k, tree_t * a, tree_t * b, tree_t * c) { tree_t * n = ast_node_new(k); if (a) ast_push(n, a); if (b) ast_push(n, b); if (c) ast_push(n, c); return n; }
tree_t * pm_var(const char * nm) { tree_t * n = ast_node_new(TT_VAR); n->v.sval = (char *) nm; return n; }
tree_t * pm_int(int64_t v) { tree_t * n = ast_node_new(TT_ILIT); n->v.ival = v; return n; }
static int pm_flatten(const tree_t * t, const tree_t ** out, int cap) {
    if (t->t == TT_CAT && t->n == 2) { int a = pm_flatten(t->c[0], out, cap); return a < 0 ? a : (a + pm_flatten(t->c[1], out + a, cap - a)); }
    if (cap < 1) return -1;
    out[0] = t;
    return 1;
}
static int pm_count(const tree_t * t) { return (t->t == TT_CAT && t->n == 2) ? pm_count(t->c[0]) + pm_count(t->c[1]) : 1; }
static void pm_retag_alpha(IR_graph_t * g, int from, IR_t * target) {
    for (int k = from; k < g->n; k++) { IR_t * x = g->all[k]; if (x && x->γ.node == target && (unsigned char) x->γ.sz[0] == 0xce && (unsigned char) x->γ.sz[1] == 0xb2) lc_γ_to(x, target); }
}
tree_t * pm_call(const char * fn, tree_t * a0, tree_t * a1) { tree_t * c = ast_node_new(TT_FNC); ast_push(c, pm_var(fn)); if (a0) ast_push(c, a0); if (a1) ast_push(c, a1); return c; }
static tree_t * pm_empty_str(void) { tree_t * q = ast_node_new(TT_QLIT); q->v.sval = (char *) ""; return q; }
static rx_t * rx_mk(char k, const char * s, int n, rx_t * a, rx_t * b) { rx_t * r = ct_zalloc(1, sizeof * r); r->k = k; r->s = s; r->n = n; r->a = a; r->b = b; return r; }
static void rx_fail(rxs_t * st, const char * m) { fprintf(stderr, "icon: line %d: regular expression: %s\n", st->line, m); exit(1); }
static void rx_ws(rxs_t * st) { while (st->p < st->e && isspace((unsigned char) *st->p)) st->p++; }
static int rx_at(rxs_t * st, char c) { rx_ws(st); return st->p < st->e && *st->p == c; }
static int rx_word(char c) { return isalnum((unsigned char) c) || c == '_'; }
static char * rx_quoted(rxs_t * st, int * n) {
    char q = *st->p++;
    char * o = ct_zalloc(1, (size_t) (st->e - st->p) + 1);
    int m = 0;
    while (st->p < st->e && *st->p != q) { char c = *st->p++; if (c == '\\' && st->p < st->e) { c = *st->p++; c = c == 'n' ? '\n' : c == 't' ? '\t' : c; } o[m++] = c; }
    if (st->p >= st->e) rx_fail(st, "unterminated string");
    st->p++;
    *n = m;
    return o;
}
static rx_t * rx_set(rxs_t * st, char k) {
    char * o = ct_zalloc(1, (size_t) (st->e - st->p) * 256 + 1);
    int m = 0, have = 0;
    char prev = 0;
    if (k == 'N') st->p++;
    while (st->p < st->e && *st->p != ']') {
        char c = *st->p++;
        if (isspace((unsigned char) c)) continue;
        if (c == '-' && have && st->p < st->e && *st->p != ']') { char hi = *st->p++; for (int x = (unsigned char) prev + 1; x <= (unsigned char) hi; x++) o[m++] = (char) x; have = 0; continue; }
        o[m++] = c;
        prev = c;
        have = 1;
    }
    if (st->p >= st->e) rx_fail(st, "unterminated [");
    st->p++;
    return rx_mk(k, o, m, NULL, NULL);
}
static rx_t * rx_atom(rxs_t * st) {
    rx_ws(st);
    if (st->p >= st->e) rx_fail(st, "operand expected");
    char c = *st->p;
    int n = 0;
    if (rx_word(c)) { const char * b = st->p; while (st->p < st->e && rx_word(*st->p)) st->p++; return rx_mk('I', b, (int) (st->p - b), NULL, NULL); }
    if (c == '"') { char * o = rx_quoted(st, &n); return rx_mk('S', o, n, NULL, NULL); }
    if (c == '\'') { char * o = rx_quoted(st, &n); return rx_mk('C', o, n, NULL, NULL); }
    if (c == '.') { st->p++; return rx_mk('D', NULL, 0, NULL, NULL); }
    if (c == '(') { st->p++; rx_t * r = rx_regex(st); if (!rx_at(st, ')')) rx_fail(st, ") expected"); st->p++; return rx_mk('P', NULL, 0, r, NULL); }
    if (c == '[') { st->p++; rx_ws(st); return rx_set(st, (st->p < st->e && *st->p == '^') ? 'N' : 'A'); }
    rx_fail(st, "unexpected character");
    return NULL;
}
static rx_t * rx_post(rxs_t * st) {
    rx_t * r = rx_atom(st);
    for (;;) {
        if (rx_at(st, '*')) {
            st->p++;
            r = rx_mk('k', NULL, 0, r, NULL);
        } else if (rx_at(st, '+')) {
            st->p++;
            r = rx_mk('o', NULL, 0, r, NULL);
        } else if (rx_at(st, '?')) {
            st->p++;
            r = rx_mk('q', NULL, 0, r, NULL);
        } else if (rx_at(st, '{')) {
            st->p++;
            rx_ws(st);
            int cnt = 0;
            if (st->p >= st->e || !isdigit((unsigned char) *st->p)) rx_fail(st, "repeat count expected");
            while (st->p < st->e && isdigit((unsigned char) *st->p)) cnt = cnt * 10 + (*st->p++ - '0');
            if (!rx_at(st, '}')) rx_fail(st, "} expected");
            st->p++;
            if (cnt < 1) rx_fail(st, "regex occurrences may not be zero yet");
            rx_t * base = r;
            for (int i = 2; i <= cnt; i++) r = rx_mk('c', NULL, 0, r, base);
        } else return r;
    }
}
static rx_t * rx_seq(rxs_t * st) { rx_t * l = rx_post(st); rx_ws(st); if (st->p < st->e && (rx_word(*st->p) || strchr("\"'.([", *st->p))) return rx_mk('c', NULL, 0, l, rx_seq(st)); return l; }
static rx_t * rx_regex(rxs_t * st) {
    rx_ws(st);
    if (st->p >= st->e || *st->p == ')') return rx_mk('e', NULL, 0, NULL, NULL);
    rx_t * l = rx_seq(st);
    if (rx_at(st, '|')) { st->p++; return rx_mk('b', NULL, 0, l, rx_regex(st)); }
    return l;
}
static tree_t * rx_lit(tree_e k, const char * s, int n) { tree_t * q = ast_node_new(k); q->v.sval = lp_strdup(s ? s : ""); q->slen = n; return q; }
static tree_t * rx_arb(tree_t * a) { return pm_call("Arbno", a, NULL); }
static tree_t * rx_cat(tree_t * a, tree_t * b) { return pm_call("ICN$UPCAT", a, b); }
static tree_t * rx_opt(tree_t * a) { return pm_node(TT_ALT, a, rx_lit(TT_QLIT, "", 0), NULL); }
static tree_t * rx_notany_nl(void) { return pm_call("NotAny", rx_lit(TT_QLIT, "\n", 1), NULL); }
static tree_t * rx_tail(const rx_t * r, int line, int arb) { tree_t * last = rx_lit(TT_QLIT, r->s + r->n - 1, 1); if (arb) return rx_arb(last); return rx_opt(last); }
static tree_t * rx_tr(const rx_t * r, int line) {
    switch (r->k) {
        case 'e':
        return rx_lit(TT_QLIT, "", 0);
        case 'I':
        case 'S':
        return rx_lit(TT_QLIT, r->s, r->n);
        case 'C':
        return rx_lit(TT_CSET, r->s, r->n);
        case 'D':
        return rx_notany_nl();
        case 'A':
        return pm_call("Any", rx_lit(TT_CSET, r->s, r->n), NULL);
        case 'N':
        return pm_call("NotAny", rx_lit(TT_CSET, r->s, r->n), NULL);
        case 'P':
        return rx_tr(r->a, line);
        case 'c':
        return rx_cat(rx_tr(r->a, line), rx_tr(r->b, line));
        case 'b':
        return pm_node(TT_ALT, rx_tr(r->a, line), rx_tr(r->b, line), NULL);
        case 'k':
        {
            const rx_t * a = r->a;
            if (a->k == 'I') return a->n > 1 ? rx_cat(rx_lit(TT_QLIT, a->s, a->n - 1), rx_tail(a, line, 1)) : rx_arb(rx_lit(TT_QLIT, a->s, a->n));
            if (a->k == 'D') { fprintf(stderr, "icon: line %d: regular expression: * is not supported on .\n", line); exit(1); }
            if (a->k == 'A') return rx_arb(rx_lit(TT_CSET, a->s, a->n));
            if (a->k == 'P') return rx_arb(rx_tr(a->a, line));
            return rx_arb(rx_tr(a, line));
        }
        case 'o':
        {
            const rx_t * a = r->a;
            if (a->k == 'I') return rx_cat(rx_lit(TT_QLIT, a->s, a->n), rx_tail(a, line, 1));
            if (a->k == 'S') return rx_cat(rx_tr(a, line), rx_arb(rx_tr(a, line)));
            if (a->k == 'D') return rx_cat(rx_notany_nl(), rx_arb(rx_notany_nl()));
            if (a->k == 'C') { fprintf(stderr, "icon: line %d: regular expression: + is not supported on a cset\n", line); exit(1); }
            rx_t kl = *r;
            kl.k = 'k';
            return rx_cat(rx_tr(a, line), rx_tr(&kl, line));
        }
        default:
        {
            const rx_t * a = r->a;
            if (a->k == 'I') return a->n > 1 ? rx_cat(rx_lit(TT_QLIT, a->s, a->n - 1), rx_tail(a, line, 0)) : rx_opt(rx_lit(TT_QLIT, a->s, a->n));
            if (a->k == 'S' || a->k == 'C') return rx_opt(rx_tr(a, line));
            if (a->k == 'D') { fprintf(stderr, "icon: line %d: regular expression: ? is not supported on .\n", line); exit(1); }
            return rx_opt(rx_tr(a, line));
        }
    }
}
tree_t * rx_translate(const tree_t * t) {
    rxs_t st = { t->v.sval ? t->v.sval : "", NULL, t->line };
    st.e = st.p + (t->slen > 0 ? (size_t) t->slen : strlen(st.p));
    rx_t * r = rx_regex(&st);
    rx_ws(&st);
    if (st.p < st.e) rx_fail(&st, "unexpected text");
    return rx_tr(r, t->line);
}
static tree_t * th_var(const char * nm) { tree_t * v = pm_var(nm); v->slen = (int) strlen(nm); return v; }
static tree_t * th_call(const char * fn, tree_t * a) { tree_t * c = ast_node_new(TT_FNC); ast_push(c, th_var(fn)); ast_push(c, a); return c; }
static tree_t * th_co_call(icx_t * cx, const tree_t * t) {
    const char * tv = icn_synth_local(cx);
    tree_t * ca = ast_node_new(TT_FNC);
    ast_push(ca, th_var(tv));
    for (int i = 1; i < t->n; i++) ast_push(ca, (tree_t *) t->c[i]);
    ca->slen = -5;
    tree_t * act = NULL;
    if (t->n > 1) {
        tree_t * lst = ast_node_new(TT_MAKELIST);
        for (int i = 1; i < t->n; i++) ast_push(lst, (tree_t *) t->c[i]);
        act = pm_node(TT_ACTIVATE, lst, th_var(tv), NULL);
    } else act = pm_node(TT_ACTIVATE, th_var(tv), NULL, NULL);
    tree_t * q = ast_node_new(TT_QLIT);
    q->v.sval = (char *) "co-expression";
    q->slen = 13;
    tree_t * sel = pm_node(TT_IF, pm_node(TT_LEQ, pm_call("type", th_var(tv), NULL), q, NULL), act, ca);
    return pm_node(TT_CONJ, pm_node(TT_ASSIGN, th_var(tv), (tree_t *) t->c[0], NULL), sel, NULL);
}
static tree_t * th_lock_wait(const char * mv) {
    tree_t * yield = pm_node(TT_ALTERNATE, pm_node(TT_ACTIVATE, pm_call("ICN$SCHED", NULL, NULL), NULL, NULL), pm_call("runerr", pm_int(177), NULL), NULL);
    tree_t * got = pm_node(TT_UNTIL, pm_call("ICN$LOCK_TRY", th_var(mv), pm_int(1)), yield, NULL);
    return pm_node(TT_LIMIT, pm_node(TT_ALTERNATE, got, icn_variable_lit_tree("&null"), NULL), pm_int(1), NULL);
}
static tree_t * th_unlock_tree(icx_t * cx, tree_t * m) {
    const char * uv = icn_synth_local(cx);
    tree_t * sched = pm_node(TT_ALTERNATE, pm_node(TT_ACTIVATE, pm_call("ICN$SCHED", NULL, NULL), NULL, NULL), icn_variable_lit_tree("&null"), NULL);
    return pm_node(TT_CONJ, pm_node(TT_ASSIGN, th_var(uv), th_call("ICN$UNLOCK", m), NULL), pm_node(TT_LIMIT, sched, pm_int(1), NULL), th_var(uv));
}
static const char * th_sigh(icx_t * cx) {
    for (int i = 0; i < cx->ngn; i++) if (cx->gn[i] && !strcmp(cx->gn[i], "$sigh")) return "$sigh";
    const char * nm = "$sigh";
    { extern void global_register(const char *); global_register(nm); }
    lc_vec_push(&g_lower.icn.lower_icon_proc_gnv, &nm);
    cx->gn = (const char **) g_lower.icn.lower_icon_proc_gnv.data;
    cx->ngn = g_lower.icn.lower_icon_proc_gnv.n;
    return nm;
}
static tree_t * th_poll(const char * sg, const char * sv) {
    tree_t * hc = ast_node_new(TT_FNC);
    ast_push(hc, pm_node(TT_IDX, th_var(sg), th_var(sv), NULL));
    ast_push(hc, pm_call("ICN$USIGNAME", th_var(sv), NULL));
    tree_t * loop = pm_node(TT_WHILE, pm_node(TT_ASSIGN, th_var(sv), pm_call("ICN$USIGTAKE", NULL, NULL), NULL), hc, NULL);
    return pm_node(TT_ALTERNATE, loop, icn_variable_lit_tree("&null"), NULL);
}
static tree_t * th_yield(void) { return pm_node(TT_ALTERNATE, pm_node(TT_ACTIVATE, pm_call("ICN$SCHED", NULL, NULL), NULL, NULL), pm_call("runerr", pm_int(177), NULL), NULL); }
tree_t * th_form(icx_t * cx, const tree_t * t) {
    const tree_t * fn = t->n > 0 ? t->c[0] : NULL;
    if (fn && fn->t != TT_FIELD && t->slen != -5 && cx->prog && cx->prog->v.ival && !icn_callee_is_name(cx, fn)) return th_co_call(cx, t);
    if (!fn || fn->t != TT_VAR || !fn->v.sval || !cx->prog || !cx->prog->v.ival) return NULL;
    if (cx->has_trap && t->n == 3 && !strcmp(fn->v.sval, "trap") && !icn_is_proc("trap")) {
        const char * hv = icn_synth_local(cx);
        const char * sg = th_sigh(cx);
        tree_t * ini = pm_node(TT_ASSIGN, th_var(hv), (tree_t *) t->c[2], NULL);
        tree_t * reg = pm_node(TT_ALTERNATE, pm_node(TT_NONNULL, th_var(sg), NULL, NULL), pm_node(TT_ASSIGN, th_var(sg), th_call("list", pm_int(64)), NULL), NULL);
        tree_t * slot = pm_node(TT_IDX, th_var(sg), pm_call("ICN$USIGTRAP", (tree_t *) t->c[1], th_var(hv)), NULL);
        return pm_node(TT_CONJ, ini, pm_node(TT_CONJ, reg, pm_node(TT_ASSIGN, slot, th_var(hv), NULL), NULL), NULL);
    }
    if (cx->has_trap && t->slen != -6 && !icn_is_proc(fn->v.sval) &&
        (!strcmp(fn->v.sval, "delay") || !strcmp(fn->v.sval, "read") || !strcmp(fn->v.sval, "reads") || !strcmp(fn->v.sval, "select") || !strcmp(fn->v.sval, "wait") || !strcmp(fn->v.sval, "system") ||
        !strcmp(fn->v.sval, "receive"))) {
        const char * sg = th_sigh(cx);
        const char * rv = icn_synth_local(cx), * sv = icn_synth_local(cx);
        tree_t * call = ast_node_new(TT_FNC);
        for (int i = 0; i < t->n; i++) ast_push(call, (tree_t *) t->c[i]);
        call->slen = -6;
        return pm_node(TT_IF, pm_node(TT_ASSIGN, th_var(rv), call, NULL), pm_node(TT_CONJ, th_poll(sg, sv), th_var(rv), NULL), pm_node(TT_CONJ, th_poll(sg, sv), icn_variable_lit_tree("&fail"), NULL));
    }
    if (!strcmp(fn->v.sval, "spawn") && t->n >= 2 && t->c[1] && t->c[1]->t == TT_CREATE && t->c[1]->n >= 1 && !icn_is_proc("spawn")) {
        tree_t * once = pm_node(TT_ALTERNATE, pm_node(TT_LIMIT, (tree_t *) t->c[1]->c[0], pm_int(1), NULL), icn_variable_lit_tree("&null"), NULL);
        tree_t * body = pm_node(TT_CONJ, once, pm_call("ICN$THREAD_END", NULL, NULL), pm_node(TT_REPEAT, pm_node(TT_ACTIVATE, pm_call("ICN$SCHED", NULL, NULL), NULL, NULL), NULL, NULL));
        return th_call("ICN$THREAD", pm_node(TT_CREATE, body, NULL, NULL));
    }
    if (!strcmp(fn->v.sval, "spawn") && t->n >= 2 && t->c[1] && t->c[1]->t != TT_CREATE && !icn_is_proc("spawn")) return th_call("ICN$THREAD", (tree_t *) t->c[1]);
    if (!strcmp(fn->v.sval, "ICN$RECV") && t->n == 4 && t->slen != -6 && t->c[3] && t->c[3]->t == TT_ILIT && t->c[3]->v.ival == 1 && t->c[2] && t->c[2]->t == TT_VAR && t->c[2]->v.sval &&
        !strcmp(t->c[2]->v.sval, "&null")) {
        const char * tv = icn_synth_local(cx), * vv = icn_synth_local(cx);
        tree_t * rc = pm_call("ICN$RECV", th_var(tv), (tree_t *) t->c[2]);
        ast_push(rc, (tree_t *) t->c[3]);
        rc->slen = -6;
        tree_t * yield = pm_node(TT_ALTERNATE, pm_node(TT_ACTIVATE, pm_call("ICN$SCHED", NULL, NULL), NULL, NULL), pm_call("runerr", pm_int(177), NULL), NULL);
        tree_t * brk = pm_node(TT_LOOP_BREAK, th_var(vv), NULL, NULL);
        tree_t * body = pm_node(TT_REPEAT, pm_node(TT_IF, pm_node(TT_ASSIGN, th_var(vv), rc, NULL), brk, yield), NULL, NULL);
        return pm_node(TT_CONJ, pm_node(TT_ASSIGN, th_var(tv), (tree_t *) t->c[1], NULL), body, NULL);
    }
    if (!strcmp(fn->v.sval, "wait") && t->n == 2 && t->c[1] && !icn_is_proc("wait")) {
        const char * tv = icn_synth_local(cx);
        tree_t * yield = pm_node(TT_ALTERNATE, pm_node(TT_ACTIVATE, pm_call("ICN$SCHED", NULL, NULL), NULL, NULL), pm_call("runerr", pm_int(177), NULL), NULL);
        tree_t * tq = ast_node_new(TT_QLIT);
        tq->v.sval = (char *) "integer";
        tq->slen = 7;
        tree_t * cvw =
            pm_node(TT_IF, pm_call("ICN$CV_WAIT", th_var(tv), NULL),
            pm_node(TT_CONJ, pm_node(TT_UNTIL, pm_call("ICN$CV_WOKEN", NULL, NULL), th_yield(), NULL), pm_node(TT_UNTIL, pm_call("ICN$LOCK_TRY", th_var(tv), pm_int(1)), th_yield(), NULL), NULL),
            pm_node(TT_UNTIL, pm_call("ICN$CV_WOKEN", NULL, NULL), th_yield(), NULL));
        tree_t * byint = pm_node(TT_IF, pm_node(TT_LT, th_var(tv), pm_int(0), NULL), cvw, pm_call("ICN$UWAIT", th_var(tv), NULL));
        return pm_node(TT_CONJ, pm_node(TT_ASSIGN, th_var(tv), (tree_t *) t->c[1], NULL),
            pm_node(TT_IF, pm_node(TT_LEQ, pm_call("type", th_var(tv), NULL), tq, NULL), byint, pm_node(TT_UNTIL, th_call("ICN$DONE", th_var(tv)), yield, NULL)), NULL);
    }
    if (!strcmp(fn->v.sval, "lock") && t->n == 2 && t->c[1] && !icn_is_proc("lock")) {
        const char * mv = icn_synth_local(cx);
        return pm_node(TT_CONJ, pm_node(TT_ASSIGN, th_var(mv), (tree_t *) t->c[1], NULL), th_lock_wait(mv), th_var(mv));
    }
    if (!strcmp(fn->v.sval, "trylock") && t->n == 2 && t->c[1] && !icn_is_proc("trylock")) return th_call("ICN$LOCK_TRY", (tree_t *) t->c[1]);
    if (!strcmp(fn->v.sval, "unlock") && t->n == 2 && t->c[1] && !icn_is_proc("unlock")) return th_unlock_tree(cx, (tree_t *) t->c[1]);
    if (!strcmp(fn->v.sval, "ICN$CRIT") && t->n == 3) {
        const char * mv = icn_synth_local(cx), * rv = icn_synth_local(cx);
        tree_t * bad = pm_node(TT_CONJ, icn_is_proc("unlock") ? th_call("unlock", th_var(mv)) : th_unlock_tree(cx, th_var(mv)), icn_variable_lit_tree("&fail"), NULL);
        tree_t * body = pm_node(TT_ALTERNATE, pm_node(TT_ASSIGN, th_var(rv), pm_node(TT_LIMIT, (tree_t *) t->c[2], pm_int(1), NULL), NULL), bad, NULL);
        tree_t * cj = ast_node_new(TT_CONJ);
        ast_push(cj, pm_node(TT_ASSIGN, th_var(mv), (tree_t *) t->c[1], NULL));
        ast_push(cj, icn_is_proc("lock") ? th_call("lock", th_var(mv)) : th_lock_wait(mv));
        ast_push(cj, body);
        ast_push(cj, icn_is_proc("unlock") ? th_call("unlock", th_var(mv)) : th_unlock_tree(cx, th_var(mv)));
        ast_push(cj, th_var(rv));
        return cj;
    }
    return NULL;
}
tree_t * th_listcomp(icx_t * cx, const tree_t * t) {
    const char * lv = icn_synth_local(cx);
    tree_t * put_c = ast_node_new(TT_FNC);
    ast_push(put_c, th_var("put"));
    ast_push(put_c, th_var(lv));
    ast_push(put_c, (tree_t *) t->c[0]);
    tree_t * cj = ast_node_new(TT_CONJ);
    ast_push(cj, pm_node(TT_ASSIGN, th_var(lv), ast_node_new(TT_MAKELIST), NULL));
    ast_push(cj, pm_node(TT_ALTERNATE, pm_node(TT_EVERY, put_c, NULL, NULL), icn_variable_lit_tree("&null"), NULL));
    ast_push(cj, pm_node(TT_GT, pm_node(TT_SIZE, th_var(lv), NULL, NULL), pm_int(0), NULL));
    ast_push(cj, th_var(lv));
    return cj;
}
tree_t * pm_patfn(icx_t * cx, const tree_t * t) {
    static const struct {
        const char * nm;
        tree_e tt;
        char k;
    } pf[] = { {"Len", TT_LEN, 'N'}, {"Pos", TT_POS, 'N'}, {"Rpos", TT_RPOS, 'N'}, {"Tab", TT_TAB, 'N'}, {"Rtab", TT_RTAB, 'N'}, {"Any", TT_ANY, 'K'}, {"NotAny", TT_NOTANY, 'K'}, {"Span", TT_SPAN,
        'K'}, {"Break", TT_BREAK, 'K'}, {"Breakx", TT_BREAKX, 'K'}, {"Nspan", TT_SPAN, 'S'}, {"Arb", TT_ARB, '0'}, {"Rem", TT_REM, '0'}, {"Bal", TT_BAL, '0'}, {"Fail", TT_FAIL, '0'}, {"Succeed",
        TT_SUCCEED, '0'}, {"Abort", TT_ABORT, '0'}, {"Fence", TT_FENCE, 'F'}, {"Arbno", TT_ARBNO, 'A'} };
    if ((t->t == TT_CAPT_COND_ASGN || t->t == TT_CAPT_IMMED_ASGN) && t->n >= 2 && t->c[0] && t->c[1] && t->c[1]->t == TT_VAR && t->c[1]->v.sval) {
        const int imm = t->t == TT_CAPT_IMMED_ASGN;
        char nb[fmt_len("\005%s", t->c[1]->v.sval)];
        snprintf(nb, sizeof nb, "\005%s", t->c[1]->v.sval);
        tree_t * q = ast_node_new(TT_QLIT);
        q->v.sval = imm ? lp_strdup(nb) : t->c[1]->v.sval;
        q->slen = (int) strlen(q->v.sval);
        tree_t * c3 = ast_node_new(TT_FNC);
        ast_push(c3, pm_var("SNO$PBC"));
        ast_push(c3, pm_int((int64_t) TT_CAPT_COND_ASGN));
        ast_push(c3, q);
        ast_push(c3, (tree_t *) t->c[0]);
        if (imm) return pm_node(TT_CAT, c3, pm_imm_commit(), NULL);
        return c3;
    }
    if (t->t == TT_CAPT_CURSOR && t->n >= 1 && t->c[0] && t->c[0]->t == TT_VAR && t->c[0]->v.sval) {
        char nb[fmt_len("\005\003%s", t->c[0]->v.sval)];
        snprintf(nb, sizeof nb, "\005\003%s", t->c[0]->v.sval);
        tree_t * q = ast_node_new(TT_QLIT);
        q->v.sval = lp_strdup(nb);
        q->slen = (int) strlen(nb);
        return pm_call("ICN$UCURSOR", q, NULL);
    }
    if (t->t == TT_DEFER && t->v.sval) {
        char nb[fmt_len("*%s", t->v.sval)];
        snprintf(nb, sizeof nb, "*%s", t->v.sval);
        tree_t * q = ast_node_new(TT_QLIT);
        q->v.sval = lp_strdup(nb);
        q->slen = (int) strlen(nb);
        tree_t * pd = pm_call("SNO$PDEF", q, t->n >= 1 ? (tree_t *) t->c[0] : NULL);
        if (t->n >= 2) ast_push(pd, (tree_t *) t->c[1]);
        return pd;
    }
    if (t->t == TT_PAND && t->n >= 2) return pm_call("ICN$UPCAT", (tree_t *) t->c[0], (tree_t *) t->c[1]);
    if (t->t == TT_ALT && t->n >= 2) return pm_call("SNO$PBALT", (tree_t *) t->c[0], (tree_t *) t->c[1]);
    if (t->t != TT_FNC || t->n < 1 || !t->c[0] || t->c[0]->t != TT_VAR || !t->c[0]->v.sval) return NULL;
    const char * nm = t->c[0]->v.sval;
    int na = t->n - 1;
    if (icn_is_proc(nm) || icn_is_local(cx, nm) || icn_is_own_global(cx, nm)) return NULL;
    for (int i = 0; i < (int) (sizeof pf / sizeof pf[0]); i++) {
        if (strcmp(pf[i].nm, nm)) continue;
        tree_t * kt = pm_int((int64_t) pf[i].tt);
        switch (pf[i].k) {
            case 'N':
            if (na == 1 && (pf[i].tt == TT_POS || pf[i].tt == TT_TAB)) {
                int64_t pn1;
                if (pm_const_int(t->c[1], &pn1)) return pm_call("SNO$PBN", pm_int((int64_t) (pn1 > 0 ? pf[i].tt : (pf[i].tt == TT_POS ? TT_RPOS : TT_RTAB))), pm_int(pn1 > 0 ? pn1 - 1 : -pn1));
                const char * pnv = icn_synth_local(cx);
                tree_t * fwd = pm_call("SNO$PBN", kt, pm_node(TT_SUB, pm_var(pnv), pm_int(1), NULL));
                tree_t * bwd = pm_call("SNO$PBN", pm_int((int64_t) (pf[i].tt == TT_POS ? TT_RPOS : TT_RTAB)), pm_node(TT_MNS, pm_var(pnv), NULL, NULL));
                return pm_node(TT_CONJ, pm_node(TT_ASSIGN, pm_var(pnv), (tree_t *) t->c[1], NULL), pm_node(TT_IF, pm_node(TT_GT, pm_var(pnv), pm_int(0), NULL), fwd, bwd), NULL);
            }
            return na == 1 ? pm_call("SNO$PBN", kt, (tree_t *) t->c[1]) : NULL;
            case 'K':
            return na == 1 ? pm_call("SNO$PBK", kt, (tree_t *) t->c[1]) : NULL;
            case 'S':
            return na == 1 ? pm_call("SNO$PBALT", pm_call("SNO$PBK", kt, (tree_t *) t->c[1]), pm_empty_str()) : NULL;
            case '0':
            return na == 0 ? pm_call("SNO$PB0", kt, NULL) : NULL;
            case 'A':
            return na == 1 ? pm_call("SNO$PARB", (tree_t *) t->c[1], NULL) : NULL;
            default:
            return na == 0 ? pm_call("SNO$PB0", kt, NULL) : (na == 1 ? pm_call("SNO$PFEN", (tree_t *) t->c[1], NULL) : NULL);
        }
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pm_const_int(const tree_t * a, int64_t * v) {
    if (a->t == TT_ILIT) { *v = a->v.ival; return 1; }
    if (a->t == TT_MNS && a->n == 1 && a->c[0] && a->c[0]->t == TT_ILIT) { *v = -a->c[0]->v.ival; return 1; }
    return 0;
}
static IR_t * pm_fn_box(icx_t * cx, const tree_t * e, IR_t ** en) {
    static const struct {
        const char * nm;
        IR_e op;
        char k;
    } bx[] = { {"Len", IR_MATCH_LEN, 'N'}, {"Rpos", IR_MATCH_RPOS, 'N'}, {"Rtab", IR_MATCH_RTAB, 'N'}, {"Pos", IR_MATCH_POS, 'P'}, {"Tab", IR_MATCH_TAB, 'P'}, {"Any", IR_MATCH_ANY, 'S'}, {"NotAny",
        IR_MATCH_NOTANY, 'S'}, {"Span", IR_MATCH_SPAN, 'S'}, {"Break", IR_MATCH_BREAK, 'S'}, {"Breakx", IR_MATCH_BREAKX, 'S'}, {"Rem", IR_MATCH_REM, '0'}, {"Arb", IR_MATCH_ARB, '0'} };
    if (e->t != TT_FNC || e->n < 1 || !e->c[0] || e->c[0]->t != TT_VAR || !e->c[0]->v.sval || icn_is_proc(e->c[0]->v.sval)) return NULL;
    for (int i = 0; i < (int) (sizeof bx / sizeof bx[0]); i++) {
        if (strcmp(bx[i].nm, e->c[0]->v.sval)) continue;
        const tree_t * a = e->n > 1 ? e->c[1] : NULL;
        IR_e op = bx[i].op;
        if (bx[i].k == '0') { if (e->n != 1) return NULL; return lc_build(cx->g, op, NULL, NULL); }
        if (e->n != 2 || !a) return NULL;
        if (bx[i].k == 'S') { if ((a->t != TT_QLIT && a->t != TT_CSET) || !a->v.sval) return NULL; IR_t * nd = lc_build(cx->g, op, NULL, NULL); IR_LIT(nd).sval = a->v.sval; return nd; }
        if (bx[i].k == 'P') {
            int64_t n;
            if (!pm_const_int(a, &n)) return NULL;
            IR_t * nd = lc_build(cx->g, n > 0 ? op : (op == IR_MATCH_POS ? IR_MATCH_RPOS : IR_MATCH_RTAB), NULL, NULL);
            IR_LIT(nd).ival = n > 0 ? n - 1 : -n;
            return nd;
        }
        { int64_t n; if (pm_const_int(a, &n)) { IR_t * nd = lc_build(cx->g, op, NULL, NULL); IR_LIT(nd).ival = n; return nd; } }
        IR_t * nd = lc_build(cx->g, op, NULL, NULL);
        *en = (IR_t *) a;
        return nd;
    }
    return NULL;
}
static IR_t * pm_defer_box(icx_t * cx, const tree_t * e, IR_t ** argt) {
    const char * nm = NULL;
    if (e->t == TT_VAR && e->v.sval && e->v.sval[0] != '&' && icn_is_own_global(cx, e->v.sval) && !icn_is_proc(e->v.sval)) nm = e->v.sval;
    else {
        int hn = ++g_lower.icn.pm_hidden_n;
        char b[fmt_len("$pmg%d", hn)];
        snprintf(b, sizeof b, "$pmg%d", hn);
        nm = lp_strdup(b);
        { extern void global_register(const char *); global_register(nm); }
        lc_vec_push(&g_lower.icn.lower_icon_proc_gnv, &nm);
        cx->gn = (const char **) g_lower.icn.lower_icon_proc_gnv.data;
        cx->ngn = g_lower.icn.lower_icon_proc_gnv.n;
        tree_t * v = pm_patfn(cx, e);
        *argt = pm_node(TT_ASSIGN, pm_var(nm), v ? v : (tree_t *) e, NULL);
    }
    IR_t * nd = lc_build(cx->g, IR_MATCH_DEFER, NULL, NULL);
    IR_LIT(nd).sval = (char *) nm;
    return nd;
}
static IR_t * lower_pm_boxes(icx_t * cx, const tree_t * pat, IR_t * γ, IR_t * ω, IR_t ** res) {
    int ne = pm_count(pat);
    const tree_t * el[ne];
    IR_t * nd[ne];
    IR_t * en[ne];
    IR_t * ag[ne];
    pm_flatten(pat, el, ne);
    int commit = 0;
    for (int i = 0; i < ne; i++) {
        const tree_t * e = el[i];
        IR_t * argt = NULL;
        ag[i] = NULL;
        if (e->t == TT_QLIT) {
            nd[i] = lc_build(cx->g, IR_MATCH_LIT, NULL, NULL);
            IR_LIT(nd[i]).sval = e->v.sval ? e->v.sval : (char *) "";
        } else {
            nd[i] = pm_fn_box(cx, e, &argt);
            if (!nd[i]) { nd[i] = pm_defer_box(cx, e, &argt); commit = 1; }
        }
        en[i] = nd[i];
        ag[i] = argt;
    }
    IR_t * cm = NULL;
    IR_t * fl = NULL;
    IR_t * sy = NULL;
    IR_t * pp = NULL;
    if (commit) { cm = lc_build(cx->g, IR_MATCH_CASMARK, NULL, NULL); IR_LIT(cm).ival = 0; IR_LIT(cm).sval = (char *) (cx->pname ? cx->pname : ""); lic_ω_to(cm, ω); }
    for (int i = 0; i < ne; i++) {
        if (!ag[i]) continue;
        IR_t * fb = ω;
        if (i > 0) { fb = lic_build(cx, IR_GOTO, NULL, ω); lc_γ_to_β(fb, nd[i - 1]); } else if (commit) { fb = lic_build(cx, IR_GOTO, NULL, ω); lc_γ_to_β(fb, cm); }
        IR_t * vr = NULL;
        int before = cx->g->n;
        en[i] = lic_lower(cx, (const tree_t *) ag[i], nd[i], fb, &vr);
        pm_retag_alpha(cx->g, before, nd[i]);
        if (vr && nd[i]->op != IR_MATCH_DEFER) ir_operand_push(nd[i], vr);
        if (!en[i]) en[i] = nd[i];
    }
    if (commit) {
        fl = lc_build(cx->g, IR_MATCH_FENCE0, NULL, NULL);
        IR_LIT(fl).ival = SNO_FENCE_LIT_FLUSH;
        sy = lc_build(cx->g, IR_MATCH_CASMARK, NULL, NULL);
        IR_LIT(sy).ival = 2;
        lc_γ_to(sy, fl);
        lc_ω_to_β(fl, sy);
        pp = lc_build(cx->g, IR_MATCH_CASMARK, NULL, NULL);
        IR_LIT(pp).ival = 1;
        lc_γ_to(fl, pp);
        lc_ω_to_β(sy, nd[ne - 1]);
        lic_γ_to(pp, γ);
        lc_ω_to_β(pp, fl);
        { IR_t * s0 = lc_build(cx->g, IR_MATCH_CASMARK, NULL, NULL); IR_LIT(s0).ival = 2; lic_ω_to(s0, ω); lc_γ_to(cm, s0); lc_γ_to(s0, en[0]); }
    }
    for (int i = 0; i < ne; i++) {
        if (i + 1 < ne) lc_γ_to(nd[i], en[i + 1]);
        else if (commit) lc_γ_to(nd[i], sy);
        else lic_γ_to(nd[i], γ);
        if (i > 0) lc_ω_to_β(nd[i], nd[i - 1]);
        else if (commit) lc_ω_to_β(nd[i], cm);
        else lic_ω_to(nd[i], ω);
    }
    IR_t * g = lic_build(cx, IR_GOTO, NULL, ω);
    lc_γ_to_β(g, nd[ne - 1]);
    cx->beta = g;
    *res = nd[ne - 1];
    return commit ? cm : en[0];
}
IR_t * lower_pmatch(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    if (t->n == 1 && t->c[0]) return lower_pm_boxes(cx, t->c[0], γ, ω, res);
    if (t->n < 2 || !t->c[0] || !t->c[1]) { IR_t * f = lic_build(cx, IR_FAIL, γ, ω); *res = f; return f; }
    const char * nx = icn_synth_local(cx);
    const char * si = icn_synth_local(cx);
    const char * en = icn_synth_local(cx);
    tree_t * to =
        pm_node(TT_TO, pm_var(nx), pm_node(TT_IF, pm_node(TT_GT, pm_var(nx), pm_node(TT_SIZE, pm_var("&subject"), NULL, NULL), NULL), pm_var(nx), pm_node(TT_SIZE, pm_var("&subject"), NULL, NULL)),
        NULL);
    tree_t * pass = pm_node(TT_CONJ, pm_node(TT_ASSIGN, pm_var(si), to, NULL), pm_node(TT_ASSIGN, pm_var("&pos"), pm_var(si), NULL), pm_node(TT_PMATCH, (tree_t *) t->c[1], NULL, NULL));
    ast_push(pass, pm_node(TT_ASSIGN, pm_var(en), pm_var("&pos"), NULL));
    ast_push(pass, pm_node(TT_ASSIGN, pm_var(nx), pm_var(en), NULL));
    ast_push(pass, pm_node(TT_SECTION, pm_var("&subject"), pm_var(si), pm_var(en)));
    tree_t * body = pm_node(TT_REPALT, pm_node(TT_LIMIT, pass, pm_int(1), NULL), NULL, NULL);
    tree_t * all = pm_node(TT_CONJ, pm_node(TT_ASSIGN, pm_var(nx), pm_int(1), NULL), pm_node(TT_SCAN, (tree_t *) t->c[0], body, NULL), NULL);
    return lic_lower(cx, all, γ, ω, res);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * lower_pmatch_repl(icx_t * cx, const tree_t * m, const tree_t * repl, IR_t * γ, IR_t * ω, IR_t ** res) {
    const char * nx = icn_synth_local(cx);
    const char * si = icn_synth_local(cx);
    const char * en = icn_synth_local(cx);
    const char * rv = icn_synth_local(cx);
    const char * sn = m->c[0]->v.sval;
    tree_t * to =
        pm_node(TT_TO, pm_var(nx), pm_node(TT_IF, pm_node(TT_GT, pm_var(nx), pm_node(TT_SIZE, pm_var("&subject"), NULL, NULL), NULL), pm_var(nx), pm_node(TT_SIZE, pm_var("&subject"), NULL, NULL)),
        NULL);
    tree_t * pass = pm_node(TT_CONJ, pm_node(TT_ASSIGN, pm_var(si), to, NULL), pm_node(TT_ASSIGN, pm_var("&pos"), pm_var(si), NULL), pm_node(TT_PMATCH, (tree_t *) m->c[1], NULL, NULL));
    ast_push(pass, pm_node(TT_ASSIGN, pm_var(en), pm_var("&pos"), NULL));
    tree_t * adv =
        pm_node(TT_IF, pm_node(TT_GT, pm_node(TT_SIZE, pm_var(rv), NULL, NULL), pm_int(0), NULL), pm_node(TT_ADD, pm_var(si), pm_node(TT_SIZE, pm_var(rv), NULL, NULL), NULL),
        pm_node(TT_ADD, pm_var(si), pm_int(1), NULL));
    tree_t * one =
        pm_node(TT_CONJ, pm_node(TT_SCAN, pm_var(sn), pass, NULL), pm_node(TT_ASSIGN, pm_var(rv), (tree_t *) repl, NULL),
        pm_node(TT_ASSIGN, pm_node(TT_SECTION, pm_var(sn), pm_var(si), pm_var(en)), pm_var(rv), NULL));
    ast_push(one, pm_node(TT_ASSIGN, pm_var(nx), adv, NULL));
    ast_push(one, pm_var(sn));
    tree_t * body = pm_node(TT_REPALT, pm_node(TT_LIMIT, one, pm_int(1), NULL), NULL, NULL);
    tree_t * all = pm_node(TT_CONJ, pm_node(TT_ASSIGN, pm_var(nx), pm_int(1), NULL), body, NULL);
    return lic_lower(cx, all, γ, ω, res);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const tree_t * pm_fn_form(icx_t * cx, const tree_t * t) {
    if (!t || t->t != TT_FNC || t->n != 3 || !t->c[0] || t->c[0]->t != TT_VAR || !t->c[0]->v.sval || strcmp(t->c[0]->v.sval, "pattern_match") || icn_is_proc(t->c[0]->v.sval)) return t;
    return pm_node(TT_PMATCH, (tree_t *) t->c[1], (tree_t *) t->c[2], NULL);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * icn_synth_local(icx_t * cx) {
    char b[32];
    snprintf(b, sizeof b, "$rep%d", g_lower.icn.synth_locals.n + 1);
    const char * nm = lp_strdup(b);
    lc_vec_push(&g_lower.icn.synth_locals, &nm);
    lc_vec_push(&g_lower.icn.lower_icon_proc_lnv, &nm);
    cx->ln = (const char **) g_lower.icn.lower_icon_proc_lnv.data;
    cx->nln = g_lower.icn.lower_icon_proc_lnv.n;
    return nm;
}
