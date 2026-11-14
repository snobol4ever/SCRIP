#include "parsers/raku/rk_tree/rkt_head.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ir/ct_arena.h"
#include "parsers/snobol4/scrip_cc.h"
#include "parsers/raku/rk_tree.h"
#include "parsers/raku/rk_regex.h"
#include "parsers/raku/rk_syntax.h"
#include "parsers/raku/rk_tree/rkt_01_sh.h"
#include "parsers/raku/rk_tree/rkt_02_sh.h"
#include "parsers/raku/rk_tree/rkt_03_sh.h"
#include "parsers/raku/rk_tree/rkt_05_rk.h"
#include "ir/ast_tt_capt_immed_asgn.h"
#include "ir/ast.h"
#include "ir/ast_node_new.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void sb_c(SB *s, char c) { GROW(s->v, s->n, s->cap, char); s->v[s->n++] = c; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sb_cp(SB *s, unsigned long cp) {
    if (cp < 0x80) sb_c(s, (char) cp);
    else if (cp < 0x800) { sb_c(s, (char) (0xC0 | (cp >> 6))); sb_c(s, (char) (0x80 | (cp & 0x3F))); }
    else if (cp < 0x10000) { sb_c(s, (char) (0xE0 | (cp >> 12))); sb_c(s, (char) (0x80 | ((cp >> 6) & 0x3F))); sb_c(s, (char) (0x80 | (cp & 0x3F))); }
    else { sb_c(s, (char) (0xF0 | (cp >> 18))); sb_c(s, (char) (0x80 | ((cp >> 12) & 0x3F))); sb_c(s, (char) (0x80 | ((cp >> 6) & 0x3F))); sb_c(s, (char) (0x80 | (cp & 0x3F))); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
char *sb_str(SB *s) { sb_c(s, 0); s->n--; return s->v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rk_adhoc_new(const char *cname, TL *named, TL *pos) {
    if (1) return NULL;
    tree_t *val = NULL;
    if (named) for (int i = 0; i + 1 < named->n; i += 2) {
        tree_t *k = named->v[i]; const char *ks = k ? k->v.sval : NULL;
        if (ks && (!strcmp(ks, "payload") || !strcmp(ks, "message"))) { val = named->v[i + 1]; break; }
    }
    if (!val && pos && pos->n > 0) val = pos->v[0];
    if (!val) { val = ast_node_new(TT_QLIT); val->v.sval = (char *) intern(""); }
    return val;
}
static tree_t *rk_interp_primary(RkB *b, const char *s, int *ip, int len) {
    int i = *ip;
    while (i < len && s[i] == ' ') i++;
    if (i < len && (s[i] == '$' || s[i] == '@')) {
        char sig = s[i]; i++;
        SB nm = { 0 }; sb_c(&nm, sig);
        while (i < len && (s[i] == '_' || (s[i] >= 'A' && s[i] <= 'Z') || (s[i] >= 'a' && s[i] <= 'z') || (s[i] >= '0' && s[i] <= '9'))) { sb_c(&nm, s[i]); i++; }
        *ip = i;
        return var_node(b, sb_str(&nm));
    }
    if (i < len && s[i] >= '0' && s[i] <= '9') {
        long v = 0;
        while (i < len && s[i] >= '0' && s[i] <= '9') { v = v * 10 + (s[i] - '0'); i++; }
        *ip = i;
        return rk_ilit(v);
    }
    *ip = i; return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_interp_subexpr(RkB *b, const char *s, int *ip, int len) {
    tree_t *left = rk_interp_primary(b, s, ip, len);
    if (!left) return NULL;
    int i = *ip;
    while (i < len && s[i] == ' ') i++;
    if (i < len && (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/' || s[i] == '%')) {
        char op = s[i]; i++; *ip = i;
        tree_t *right = rk_interp_primary(b, s, ip, len);
        if (!right) return left;
        tree_e k = op == '+' ? TT_ADD : op == '-' ? TT_SUB : op == '*' ? TT_MUL : op == '/' ? TT_DIV : TT_MOD;
        return expr_binary(k, nctx(b, left), nctx(b, right));
    }
    *ip = i; return left;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *lower_interp_str(RkB *b, const char *s) {
    int len = s ? (int) strlen(s) : 0;
    tree_t *result = NULL;
    SB lit = { 0 }; int i = 0;
    while (i < len) {
        if (s[i] == '$' && i + 1 < len && (s[i + 1] == '_' || (s[i + 1] >= 'A' && s[i + 1] <= 'Z') || (s[i + 1] >= 'a' && s[i + 1] <= 'z') || (s[i + 1] == '*' && i + 2 < len && (s[i + 2] == '_' || isalpha((unsigned char) s[i + 2]))))) {
            if (lit.n > 0) { tree_t *lq = leaf_sval(TT_QLIT, sb_str(&lit)); result = result ? expr_binary(TT_CAT, result, lq) : lq; lit.n = 0; }
            i++;
            SB vn = { 0 };
            if (s[i] == '*') { sb_c(&vn, '*'); i++; }
            while (i < len && (s[i] == '_' || (s[i] >= 'A' && s[i] <= 'Z') || (s[i] >= 'a' && s[i] <= 'z') || (s[i] >= '0' && s[i] <= '9'))) { sb_c(&vn, s[i]); i++; }
            while (i + 1 < len && (s[i] == '-' || s[i] == '\'') && isalpha((unsigned char) s[i + 1])) {
                sb_c(&vn, s[i]);
                i++;
                while (i < len && (s[i] == '_' || isalnum((unsigned char) s[i]))) { sb_c(&vn, s[i]); i++; }
            }
            char *vs = sb_str(&vn); const char *st = b->st_look ? b->st_look(b->st_ctx, fmt("$%s", vs), (int) strlen(vs) + 1) : NULL;
            tree_t *var = leaf_sval(TT_VAR, st ? st + 1 : vs);
            result = result ? expr_binary(TT_CAT, result, var) : var;
        }
        else if (s[i] == '$' && i + 2 < len && (s[i + 1] == '!' || s[i + 1] == '.') && (s[i + 2] == '_' || isalpha((unsigned char) s[i + 2]))) {
            if (lit.n > 0) { tree_t *lq = leaf_sval(TT_QLIT, sb_str(&lit)); result = result ? expr_binary(TT_CAT, result, lq) : lq; lit.n = 0; }
            i += 2;
            SB vn = { 0 };
            while (i < len && (s[i] == '_' || isalnum((unsigned char) s[i]))) { sb_c(&vn, s[i]); i++; }
            while (i + 1 < len && (s[i] == '-' || s[i] == '\'') && isalpha((unsigned char) s[i + 1])) {
                sb_c(&vn, s[i]);
                i++;
                while (i < len && (s[i] == '_' || isalnum((unsigned char) s[i]))) { sb_c(&vn, s[i]); i++; }
            }
            tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
            fe->v.sval = (char *) intern(sb_str(&vn));
            result = result ? expr_binary(TT_CAT, result, fe) : fe;
        }
        else if (s[i] == '$' && i + 1 < len && (s[i + 1] == '/' || (s[i + 1] == '!' && !(i + 2 < len && (s[i + 2] == '_' || isalpha((unsigned char) s[i + 2])))) || (s[i + 1] >= '0' && s[i + 1] <= '9') || (s[i + 1] == '<' && strchr(s + i + 2, '>')))) {
            if (lit.n > 0) { tree_t *lq = leaf_sval(TT_QLIT, sb_str(&lit)); result = result ? expr_binary(TT_CAT, result, lq) : lq; lit.n = 0; }
            tree_t *cp;
            if (s[i + 1] == '/') { cp = leaf_sval(TT_VAR, "/"); i += 2; }
            else if (s[i + 1] == '!') { cp = leaf_sval(TT_VAR, "!"); i += 2; }
            else if (s[i + 1] == '<') { int j = i + 2; while (s[j] != '>') j++; cp = ast_node_new(TT_NAMED_CAPTURE); ast_push(cp, leaf_sval(TT_QLIT, trimdup(s + i + 2, j - i - 2))); i = j + 1; }
            else { int j = i + 1, v = 0; while (j < len && s[j] >= '0' && s[j] <= '9') v = v * 10 + (s[j++] - '0'); cp = ast_node_new(TT_CAPTURE); ast_push(cp, rk_ilit(v)); i = j; }
            result = result ? expr_binary(TT_CAT, result, cp) : cp;
        }
        else if (s[i] == '@' && i + 1 < len && (s[i + 1] == '_' || (s[i + 1] >= 'A' && s[i + 1] <= 'Z') || (s[i + 1] >= 'a' && s[i + 1] <= 'z'))) {
            int ne = i + 1;
            while (ne < len && (s[ne] == '_' || isalnum((unsigned char) s[ne]) || ((s[ne] == '-' || s[ne] == '\'') && ne + 1 < len && isalpha((unsigned char) s[ne + 1])))) ne++;
            if (ne >= len || s[ne] != '[') { sb_c(&lit, s[i]); i++; continue; }
            if (lit.n > 0) { tree_t *lq = leaf_sval(TT_QLIT, sb_str(&lit)); result = result ? expr_binary(TT_CAT, result, lq) : lq; lit.n = 0; }
            SB vn = { 0 }; sb_c(&vn, s[i]); i++;
            while (i < ne) { sb_c(&vn, s[i]); i++; }
            char *vname = sb_str(&vn);
            { const char *st = b->st_look ? b->st_look(b->st_ctx, vname, (int) strlen(vname)) : NULL; if (st) vname = (char *) st; }
            tree_t *arrpart;
            if (i < len && s[i] == '[') {
                i++;
                tree_t *idx = rk_interp_subexpr(b, s, &i, len);
                while (i < len && s[i] != ']') i++;
                if (i < len && s[i] == ']') i++;
                if (idx) arrpart = rk_arr_index(b, vname, idx);
                else { arrpart = ast_node_new(TT_METHCALL); ast_push(arrpart, leaf_sval(TT_VAR, vname)); ast_push(arrpart, leaf_sval(TT_QLIT, "join")); ast_push(arrpart, leaf_sval(TT_QLIT, " ")); }
            }
            else arrpart = leaf_sval(TT_VAR, vname);
            result = result ? expr_binary(TT_CAT, result, arrpart) : arrpart;
        }
        else { sb_c(&lit, s[i]); i++; }
    }
    if (lit.n > 0) { tree_t *lq = leaf_sval(TT_QLIT, sb_str(&lit)); result = result ? expr_binary(TT_CAT, result, lq) : lq; }
    if (result && result->t != TT_CAT && result->t != TT_QLIT) result = expr_binary(TT_CAT, leaf_sval(TT_QLIT, ""), result);
    return result ? result : leaf_sval(TT_QLIT, "");
}
static int rx_idstart(int c) { return isalpha(c) || c == '_'; }
static int rx_idchar(int c) { return isalnum(c) || c == '_'; }
static tree_t *rk_rx_expr(RkB *b, const char *flags, const char *s, int len) {
    tree_t *result = NULL;
    SB lit = { 0 };
    for (const char *f = flags; *f; f++) sb_c(&lit, *f);
    int i = 0;
#define RXFLUSH() do { if (lit.n > 0) { tree_t *lq = leaf_sval(TT_QLIT, sb_str(&lit)); result = result ? expr_binary(TT_CAT, result, lq) : lq; lit.v = NULL; lit.n = 0; lit.cap = 0; } } while (0)
#define RXPART(x) do { RXFLUSH(); tree_t *pt = (x); result = result ? expr_binary(TT_CAT, result, pt) : pt; } while (0)
    while (i < len) {
        char c = s[i];
        if (c == '\\' && i + 1 < len) { sb_c(&lit, c); sb_c(&lit, s[i + 1]); i += 2; continue; }
        if (c == '\'') { int j = i + 1; while (j < len && s[j] != '\'') { if (s[j] == '\\' && j + 1 < len) j++; j++; } if (j < len) j++; for (int k = i; k < j; k++) sb_c(&lit, s[k]); i = j; continue; }
        if (c == '$' && i + 1 < len && rx_idstart((unsigned char) s[i + 1])) {
            int j = i + 1;
            while (j < len && (rx_idchar((unsigned char) s[j]) || ((s[j] == '-' || s[j] == '\'') && j + 1 < len && isalpha((unsigned char) s[j + 1])))) j++;
            RXPART(rx_wrap("__rk_rxq", rx_var(b, '$', trimdup(s + i + 1, j - i - 1))));
            i = j; continue;
        }
        if (c == '@' && i + 1 < len && rx_idstart((unsigned char) s[i + 1])) {
            int j = i + 1;
            while (j < len && rx_idchar((unsigned char) s[j])) j++;
            RXPART(rx_wrap("__rk_rxalt", rx_var(b, '@', trimdup(s + i + 1, j - i - 1))));
            i = j; continue;
        }
        if (c == '<' && i + 2 < len && (s[i + 1] == '$' || s[i + 1] == '@') && rx_idstart((unsigned char) s[i + 2])) {
            int j = i + 2;
            while (j < len && rx_idchar((unsigned char) s[j])) j++;
            if (j < len && s[j] == '>') { RXPART(rx_wrap(s[i + 1] == '$' ? "__rk_rxsrc" : "__rk_rxsrcalt", rx_var(b, s[i + 1], trimdup(s + i + 2, j - i - 2)))); i = j + 1; continue; }
        }
        sb_c(&lit, c); i++;
    }
    RXFLUSH();
#undef RXFLUSH
#undef RXPART
    return result ? result : leaf_sval(TT_QLIT, "");
}
static tree_t *rk_rq_opts(const RkRq *q) {
    if (!q->has_adv) return NULL;
    tree_t *o = make_call("__rk_rxopts");
    int mode = (q->g ? 1 : 0) | (q->ov ? 2 : 0) | (q->ex ? 4 : 0) | (q->cont ? 8 : 0) | (q->posn ? 16 : 0) | (q->x_min != -2 ? 32 : 0) | (q->nth_n ? 64 : 0);
    expr_add_child(o, rk_ilit(mode));
    expr_add_child(o, rk_ilit(q->cont ? q->c_val : q->posn ? q->p_val : 0));
    expr_add_child(o, rk_ilit(q->x_min == -2 ? 0 : q->x_min));
    expr_add_child(o, rk_ilit(q->x_min == -2 ? 0 : q->x_max));
    for (int k = 0; k < q->nth_n; k++) expr_add_child(o, rk_ilit(q->nth[k]));
    return o;
}
static tree_t *rk_rx_root(RkB *b, const char *flags, int ps, int pe, tree_t *text) {
    tree_t *r = rk_regex_parse(flags, b->s + ps, pe - ps), *w = ast_node_new(TT_RX_ROOT);
    w->v.sval = r->v.sval; w->slen = r->slen; w->line = text ? text->line : 0;
    ast_push(w, text);
    ast_push(w, r->n > 0 ? r->c[0] : ast_node_new(TT_RX_NULL));
    if (r->v.ival) { tree_t *e = ast_node_new(TT_RX_FAIL); e->v.sval = r->c[0] ? r->c[0]->v.sval : "regex parse error"; w->c[1] = e; }
    return w;
}
static tree_t *rk_rq_smatch(RkB *b, const RkRq *q, tree_t *subj, const char *kind) {
    tree_t *pat = rk_rx_root(b, q->rxflags, q->ps, q->pe, rk_rx_expr(b, q->rxflags, b->s + q->ps, q->pe - q->ps));
    tree_t *m = ast_node_new(TT_SMATCH);
    ast_push(m, subj);
    ast_push(m, pat);
    ast_push(m, leaf_sval(TT_QLIT, kind));
    tree_t *o = rk_rq_opts(q);
    if (o) ast_push(m, o);
    return m;
}
static int rk_rq_assignable(const tree_t *t) { return t && (t->t == TT_VAR || t->t == TT_IDX); }
static tree_t *rk_rq_subst(RkB *b, const RkRq *q, tree_t *subj, RkClosure *cl, int ncl) {
    int nondestructive = q->word[0] == 'S';
    tree_t *pat = rk_rx_expr(b, q->rxflags, b->s + q->ps, q->pe - q->ps);
    tree_t *repl;
    int plain = 1;
    for (int i = q->rs; i < q->re; i++) if (b->s[i] == '$' || b->s[i] == '@' || b->s[i] == '{' || b->s[i] == '\\') plain = 0;
    if (plain) repl = leaf_sval(TT_QLIT, trimdup(b->s + q->rs, q->re - q->rs));
    else {
        tree_t *seq = ast_node_new(TT_SEQ_EXPR);
        expr_add_child(seq, b_dq(b, cl, ncl, q->rs, q->re));
        repl = ast_node_new(TT_ANON_BLOCK);
        expr_add_child(repl, seq);
    }
    tree_t *call = make_call("re_subst");
    expr_add_child(call, subj);
    expr_add_child(call, pat);
    expr_add_child(call, repl);
    int mode = (q->g ? 1 : 0) | (q->ov ? 2 : 0) | (q->ex ? 4 : 0) | (q->cont ? 8 : 0) | (q->posn ? 16 : 0) | (q->x_min != -2 ? 32 : 0) | (q->nth_n ? 64 : 0) | (nondestructive ? 128 : 0);
    expr_add_child(call, rk_ilit(mode));
    expr_add_child(call, rk_ilit(q->cont ? q->c_val : q->posn ? q->p_val : 0));
    expr_add_child(call, rk_ilit(q->x_min == -2 ? 0 : q->x_min));
    expr_add_child(call, rk_ilit(q->x_min == -2 ? 0 : q->x_max));
    for (int k = 0; k < q->nth_n; k++) expr_add_child(call, rk_ilit(q->nth[k]));
    if (nondestructive || !rk_rq_assignable(subj)) return call;
    tree_t *seq = ast_node_new(TT_SEQ_EXPR);
    expr_add_child(seq, expr_binary(TT_ASSIGN, rk_tree_clone(subj), call));
    expr_add_child(seq, leaf_sval(TT_VAR, "/"));
    return seq;
}
tree_t *rk_rq_term(RkB *b, const RkRq *q, RkClosure *cl, int ncl) {
    const char *w = q->word;
    if (!w[0] || !strcmp(w, "rx")) {
        tree_t *c = make_call("__rk_regex");
        expr_add_child(c, rk_rx_root(b, q->rxflags, q->ps, q->pe, rk_rx_expr(b, q->rxflags, b->s + q->ps, q->pe - q->ps)));
        return c;
    }
    tree_t *topic = leaf_sval(TT_VAR, "_");
    if (!strcmp(w, "m") || !strcmp(w, "ms") || !strcmp(w, "mm")) {
        int plain_g = q->g && !q->ov && !q->ex && !q->cont && !q->posn && q->x_min == -2 && !q->nth_n;
        if (plain_g) { RkRq g = *q; g.has_adv = 0; return rk_rq_smatch(b, &g, topic, "match_global"); }
        return rk_rq_smatch(b, q, topic, "match");
    }
    if (!strcmp(w, "s") || !strcmp(w, "S") || !strcmp(w, "ss") || !strcmp(w, "Ss")) return rk_rq_subst(b, q, topic, cl, ncl);
    if (!strcmp(w, "tr") || !strcmp(w, "TR")) {
        int nondestructive = w[0] == 'T' || (q->trf & 8);
        tree_t *call = make_call("re_trans");
        expr_add_child(call, topic);
        expr_add_child(call, leaf_sval(TT_QLIT, trimdup(b->s + q->ps, q->pe - q->ps)));
        expr_add_child(call, leaf_sval(TT_QLIT, trimdup(b->s + q->rs, q->re - q->rs)));
        expr_add_child(call, rk_ilit(q->trf));
        if (nondestructive) return call;
        tree_t *seq = ast_node_new(TT_SEQ_EXPR);
        expr_add_child(seq, expr_binary(TT_ASSIGN, rk_tree_clone(topic), call));
        expr_add_child(seq, rk_tree_clone(topic));
        return seq;
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int dq_closure_end(const char *s, int i, int n) {
    int j = i + 1;
    for (;;) {
        if (j >= n) return -1;
        char c = s[j];
        if (c == '}') return j + 1;
        if (c == '"' || c == '\n') return -1;
        if (c == '{') {
            int k = j + 1;
            while (k < n && s[k] != '}') { if (s[k] == '{' || s[k] == '"' || s[k] == '\n') return -1; k++; }
            if (k >= n) return -1;
            j = k + 1; continue;
        }
        j++;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int ishex(int c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *dq_segment(RkB *b, const char *seg) {
    if (strchr(seg, '$') || strchr(seg, '@')) return lower_interp_str(b, seg);
    return leaf_sval(TT_QLIT, seg);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static RkClosure *closure_at(RkClosure *cl, int ncl, int pos) { for (int i = 0; i < ncl; i++) if (cl[i].pos == pos) return &cl[i]; return NULL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int dq_named_chars(SB *buf, const char *p, int n) {
    int start = buf->n, k = 0;
    while (k < n) {
        int e = k; while (e < n && p[e] != ',') e++;
        const char *seq = rk_uniname_seq(p + k, e - k);
        if (seq) { while (*seq) { char *x; unsigned long v = strtoul(seq, &x, 10); sb_cp(buf, v); seq = *x == ',' ? x + 1 : x; } }
        else { unsigned v = rk_uniname_cp(p + k, e - k); if (!v) { buf->n = start; return 0; } sb_cp(buf, v); }
        k = e + 1;
    }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_u8len(unsigned char c) { return (c & 0xF8) == 0xF0 ? 4 : (c & 0xF0) == 0xE0 ? 3 : (c & 0xE0) == 0xC0 ? 2 : 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *b_dq_fl(RkB *b, RkClosure *cl, int ncl, int from, int to, int fl) {
    const char *s = b->s; SB buf = { 0 };
    int os = from > 0 ? from - 1 : 0; while (os > 0 && ((unsigned char) s[os] & 0xC0) == 0x80) os--;
    int ol = from > 0 ? from - os : 0, cl2 = to < b->len ? rk_u8len((unsigned char) s[to]) : 0;
    typedef struct { int at; int pos; } Mk;
    Mk *mk = NULL; int nmk = 0, cmk = 0, nbrk = 0;
    int interp = (fl & (RKQ_SC | RKQ_AR)) != 0;
#define DQ_MARK(P) do { GROW(mk, nmk, cmk, Mk); mk[nmk].at = buf.n; mk[nmk].pos = (P); nmk++; } while (0)
#define DQ_BRK() do { if (interp) { DQ_MARK(-1); nbrk++; } } while (0)
#define DQ_PUTCP(X) do { unsigned long _c = (X); if ((_c == '$' || _c == '@') && interp) DQ_MARK(-(int) _c); else sb_cp(&buf, _c); } while (0)
    int i = from;
    while (i < to) {
        char c = s[i];
        if (c == '\\' && i + 1 < to) {
            char d = s[i + 1];
            if (!(fl & RKQ_BS)) {
                int l = d == '\\' ? 1 : ol > 0 && i + 1 + ol <= to && !memcmp(s + i + 1, s + os, (size_t) ol) ? ol : cl2 > 0 && i + 1 + cl2 <= to && !memcmp(s + i + 1, s + to, (size_t) cl2) ? cl2 : 0;
                if ((fl & RKQ_QBS) && l) { DQ_BRK(); for (int k = 0; k < l; k++) sb_c(&buf, s[i + 1 + k]); DQ_BRK(); i += 1 + l; continue; }
                sb_c(&buf, c); i++; continue;
            }
            if (d == '{') { sb_c(&buf, '{'); i += 2; continue; }
            if (d == '}') { sb_c(&buf, '}'); i += 2; continue; }
            if (d == 'n') { sb_c(&buf, '\n'); i += 2; continue; }
            if (d == 't') { sb_c(&buf, '\t'); i += 2; continue; }
            if (d == 'x' && i + 2 < to && s[i + 2] == '[') {
                int j = i + 3; while (j < to && (ishex(s[j]) || s[j] == ',' || s[j] == ' ')) j++;
                if (j < to && s[j] == ']' && j > i + 3) {
                    const char *p = s + i + 3;
                    while (p < s + j) { while (p < s + j && (*p == ' ' || *p == ',')) p++; if (p >= s + j) break; char *e; unsigned long cp = strtoul(p, &e, 16); if (e == p) break; DQ_PUTCP(cp);
                        p = e; }
                    i = j + 1; continue;
                }
            }
            if (d == 'o' && i + 2 < to && s[i + 2] == '[') {
                int j = i + 3; while (j < to && ((s[j] >= '0' && s[j] <= '7') || s[j] == ',' || s[j] == ' ')) j++;
                if (j < to && s[j] == ']' && j > i + 3) {
                    const char *p = s + i + 3;
                    while (p < s + j) { while (p < s + j && (*p == ' ' || *p == ',')) p++; if (p >= s + j) break; char *e; unsigned long cp = strtoul(p, &e, 8); if (e == p) break; DQ_PUTCP(cp);
                        p = e; }
                    i = j + 1; continue;
                }
            }
            if (d == 'o' && i + 2 < to && s[i + 2] >= '0' && s[i + 2] <= '7') {
                int j = i + 2;
                while (j < to && s[j] >= '0' && s[j] <= '7') j++;
                DQ_PUTCP(strtoul(spn(b, i + 2, j), NULL, 8));
                i = j; continue;
            }
            if (d == 'x' && i + 2 < to && ishex(s[i + 2])) { int j = i + 2; while (j < to && ishex(s[j])) j++; DQ_PUTCP(strtoul(spn(b, i + 2, j), NULL, 16)); i = j; continue; }
            if (d == 'c' && i + 2 < to && s[i + 2] == '[') {
                int j = i + 3; while (j < to && s[j] != ']') j++;
                int nb0 = buf.n;
                if (j < to && dq_named_chars(&buf, s + i + 3, j - i - 3)) {
                    char keep[buf.n - nb0 + 1];
                    memcpy(keep, buf.v + nb0, (size_t) (buf.n - nb0));
                    int nk = buf.n - nb0;
                    buf.n = nb0;
                    for (int q = 0; q < nk; q++) { if (keep[q] == '$' || keep[q] == '@') DQ_PUTCP((unsigned char) keep[q]); else sb_c(&buf, keep[q]); }
                    i = j + 1;
                    continue;
                }
            }
            if (d == 'c' && i + 2 < to && isdigit((unsigned char) s[i + 2])) { int j = i + 2; while (j < to && isdigit((unsigned char) s[j])) j++; DQ_PUTCP(strtoul(spn(b, i + 2, j), NULL, 10)); i = j; continue; }
            if (d == 'c' && i + 2 < to && strchr("@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_?", s[i + 2])) { DQ_PUTCP(s[i + 2] == '?' ? 127u : (unsigned) (s[i + 2] ^ 0x40)); i += 3; continue; }
            if (d == 'b') { sb_c(&buf, '\b'); i += 2; continue; }
            if (d == 'r') { sb_c(&buf, '\r'); i += 2; continue; }
            if (d == 'a') { sb_c(&buf, '\a'); i += 2; continue; }
            if (d == 'e') { sb_c(&buf, '\033'); i += 2; continue; }
            if (d == 'f') { sb_c(&buf, '\f'); i += 2; continue; }
            if (d == '\\') { sb_c(&buf, '\\'); i += 2; continue; }
            if (d == '"') { sb_c(&buf, '"'); i += 2; continue; }
            if (d == '$' || d == '@') { if (interp) DQ_MARK(-(int) d); else sb_c(&buf, d); i += 2; continue; }
            if (!isalnum((unsigned char) d) && d != '_') {
                int l = rk_u8len((unsigned char) d);
                DQ_BRK();
                for (int k = 0; k < l && i + 1 + k < to; k++) sb_c(&buf, s[i + 1 + k]);
                DQ_BRK();
                i += 1 + l; continue;
            }
            sb_c(&buf, c); i++; continue;
        }
        if ((c == '$' && !(fl & RKQ_SC)) || (c == '@' && !(fl & RKQ_AR))) { if (interp) DQ_MARK(-(int) c); else sb_c(&buf, c); i++; continue; }
        if (c == '{') {
            RkClosure *cc = closure_at(cl, ncl, i);
            int e = cc ? (cc->end > i ? cc->end : dq_closure_end(s, i, to)) : -1;
            if (e > 0) { GROW(mk, nmk, cmk, Mk); mk[nmk].at = buf.n; mk[nmk].pos = i; nmk++; i = e; continue; }
        }
        if (c == '$' || c == '@' || c == '%' || c == '&') {
            RkClosure *cc = closure_at(cl, ncl, i);
            if (cc && cc->end > i && cc->end <= to) { GROW(mk, nmk, cmk, Mk); mk[nmk].at = buf.n; mk[nmk].pos = i; nmk++; i = cc->end; continue; }
        }
        sb_c(&buf, c); i++;
    }
    char *str = sb_str(&buf);
    if (!nmk) return interp ? dq_segment(b, str) : leaf_sval(TT_QLIT, str);
    if (nmk == nbrk && !strchr(str, '$') && !strchr(str, '@')) return leaf_sval(TT_QLIT, str);
    tree_t *acc = NULL; int segstart = 0;
    for (int k = 0; k <= nmk; k++) {
        int segend = k < nmk ? mk[k].at : buf.n;
        char *seg = (char *) ct_alloc((size_t) (segend - segstart) + 1); memcpy(seg, str + segstart, (size_t) (segend - segstart)); seg[segend - segstart] = 0;
        tree_t *st = leaf_sval(TT_QLIT, seg);
        if (interp && (strchr(seg, '$') || strchr(seg, '@'))) st = lower_interp_str(b, seg);
        acc = acc ? expr_binary(TT_CAT, acc, st) : st;
        if (k < nmk) {
            if (mk[k].pos != -1) acc = expr_binary(TT_CAT, acc, mk[k].pos < 0 ? leaf_sval(TT_QLIT, mk[k].pos == -(int) '$' ? "$" : "@") : closure_expr(b, closure_at(cl, ncl, mk[k].pos)));
            segstart = mk[k].at;
        }
    }
    return acc;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *b_sq(RkB *b, int from, int to) {
    SB buf = { 0 };
    for (int i = from; i < to; ) {
        if (b->s[i] == '\\' && i + 1 < to && (b->s[i + 1] == '\'' || b->s[i + 1] == '\\')) { sb_c(&buf, b->s[i + 1]); i += 2; continue; }
        sb_c(&buf, b->s[i]); i++;
    }
    return leaf_sval(TT_QLIT, sb_str(&buf));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *regex_body(RkB *b, int from, int to, int stop) {
    SB buf = { 0 };
    for (int i = from; i < to; i++) {
        if (b->s[i] == '\\' && i + 1 < to && b->s[i + 1] == '/') { sb_c(&buf, '/'); i++; continue; }
        if (b->s[i] == stop) break;
        sb_c(&buf, b->s[i]);
    }
    return sb_str(&buf);
}
static int words_ok(const char *s, int n) {
    int words = 0, inw = 0;
    for (int i = 0; i < n; i++) {
        char c = s[i];
        if (c == ' ' || c == '\t') { inw = 0; continue; }
        if (!(isalnum((unsigned char) c) || strchr("_.:/*+?=!$,;#@|\\-", c))) return 0;
        if (!inw) { words++; inw = 1; }
    }
    return words >= 2;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_smartmatch_term_raw(RkB *b, tree_t *l, RkTerm *x) {
    if (x->kind == TK_QUOTE && !x->npost && !x->npre && x->t) {
        tree_t *t = x->t;
        if (t->t == TT_SMATCH && t->n >= 3) { t->c[0] = l; return t; }
        if (t->t == TT_SEQ_EXPR && t->n == 2 && t->c[0] && t->c[0]->t == TT_ASSIGN && t->c[0]->n == 2 && t->c[0]->c[1] && t->c[0]->c[1]->t == TT_FNC && t->c[0]->c[1]->v.sval
            && (!strcmp(t->c[0]->c[1]->v.sval, "re_subst") || !strcmp(t->c[0]->c[1]->v.sval, "re_trans"))) {
            tree_t *call = t->c[0]->c[1];
            call->c[1] = l;
            if (rk_rq_assignable(l)) { t->c[0]->c[0] = rk_tree_clone(l); if (t->c[1] && t->c[1]->v.sval && !strcmp(t->c[1]->v.sval, "_")) t->c[1] = rk_tree_clone(l); } else return call;
            return t;
        }
        if (t->t == TT_FNC && t->v.sval && (!strcmp(t->v.sval, "re_subst") || !strcmp(t->v.sval, "re_trans"))) { t->c[1] = l; return t; }
        if (t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, "__rk_regex") && t->n == 2) {
            tree_t *m = ast_node_new(TT_SMATCH);
            ast_push(m, l);
            ast_push(m, t->c[1]);
            ast_push(m, leaf_sval(TT_QLIT, "match"));
            return m;
        }
    }
    if ((x->kind == TK_NAME || (x->kind == TK_CALL && x->t && x->t->t == TT_VAR)) && !x->npost && !x->npre && !(x->name && (!strcmp(x->name, "Inf") || !strcmp(x->name, "NaN")))) {
        tree_t *mc = ast_node_new(TT_METHCALL); ast_push(mc, l); ast_push(mc, leaf_sval(TT_QLIT, "does")); ast_push(mc, leaf_sval(TT_QLIT, x->name)); return mc;
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_big_lit(int base, const char *digits, int neg) {
    tree_t *c = make_call("__rk_radix_str");
    expr_add_child(c, rk_ilit(base));
    expr_add_child(c, leaf_sval(TT_QLIT, intern(digits)));
    if (!neg) return c;
    return expr_unary(TT_MNS, c);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_digits_fit(const char *digits, int base) {
    unsigned __int128 acc = 0;
    for (const char *p = digits; *p; p++) {
        int c = (unsigned char) *p, dv = isdigit(c) ? c - '0' : isalpha(c) ? tolower(c) - 'a' + 10 : -1;
        if (dv < 0 || dv >= base) continue;
        acc = acc * (unsigned) base + (unsigned) dv;
        if (acc > (unsigned __int128) 0x7fffffffffffffffULL) return 0;
    }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_set_rad_inner(RkB *b, tree_t *t) { b->rad_inner = t; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_number(RkB *b, RkTerm *it, int from, int to) {
    memset(it, 0, sizeof *it); it->kind = TK_TREE; it->from = from; it->to = it->core_to = to;
    char *t = spn(b, from, to); int len = (int) strlen(t);
    if (len > 3 && t[0] == ':' && isdigit((unsigned char) t[1])) {
        int base = 0, k = 1;
        while (k < len && isdigit((unsigned char) t[k])) base = base * 10 + (t[k++] - '0');
        while (k < len && (t[k] == ' ' || t[k] == '\t')) k++;
        if (base >= 2 && base <= 36 && k < len && t[k] == '<') {
            SB bd = { 0 };
            for (int m = k + 1; m < len && t[m] != '>'; m++) sb_c(&bd, t[m]);
            const char *bs = sb_str(&bd);
            if (strpbrk(bs, ".*") || (bs[0] == '0' && bs[1] && strchr("xdob", bs[1]))) {
                tree_t *c = make_call("__rk_unbase");
                expr_add_child(c, rk_ilit(base));
                expr_add_child(c, leaf_sval(TT_QLIT, intern(bs)));
                expr_add_child(c, rk_ilit(0));
                it->t = c;
                return;
            }
            unsigned __int128 acc = 0;
            int neg = 0, ovf = 0;
            for (k++; k < len && t[k] != '>'; k++) {
                int c = (unsigned char) t[k], dv;
                if (c == '_' || c == ' ') continue;
                if (c == '-' && acc == 0) { neg = 1; continue; }
                if (c == '+' && acc == 0) continue;
                dv = isdigit(c) ? c - '0' : isalpha(c) ? tolower(c) - 'a' + 10 : -1;
                if (dv < 0 || dv >= base) continue;
                if (acc > ((unsigned __int128) 1 << 120)) ovf = 1;
                acc = acc * (unsigned) base + (unsigned) dv;
            }
            if (ovf || acc > (unsigned __int128) 0x7fffffffffffffffULL) {
                SB ds = { 0 };
                for (int m = 1; m < len; m++) { if (t[m] == '<') { for (m++; m < len && t[m] != '>'; m++) if (t[m] != '_' && t[m] != ' ' && t[m] != '-' && t[m] != '+') sb_c(&ds, t[m]); break; } }
                it->t = rk_big_lit(base, sb_str(&ds), neg);
                return;
            }
            it->t = rk_ilit(neg ? -(long long) acc : (long long) acc);
            return;
        }
        if (base >= 2 && k < len && (t[k] == '[' || t[k] == '(') && b->rad_inner) {
            tree_t *c = make_call(t[k] == '(' ? "__rk_unbase" : "__rk_radix_list");
            expr_add_child(c, rk_ilit(base));
            expr_add_child(c, b->rad_inner);
            b->rad_inner = NULL;
            it->t = c;
            return;
        }
    }
    int alld = len > 0; for (int i = 0; i < len; i++) if (!isdigit((unsigned char) t[i])) alld = 0;
    if (alld) { if (len >= 19 && !rk_digits_fit(t, 10)) it->t = rk_big_lit(10, t, 0); else it->t = rk_ilit(atol(t)); return; }
    int i = 0; while (i < len && isdigit((unsigned char) t[i])) i++;
    int fl = 0;
    if (i > 0 && i < len && t[i] == '.' && i + 1 < len && isdigit((unsigned char) t[i + 1])) { i++; while (i < len && isdigit((unsigned char) t[i])) i++; fl = 1; }
    if (i > 0 && i < len && (t[i] == 'e' || t[i] == 'E')) { int j = i + 1; if (j < len && (t[j] == '+' || t[j] == '-')) j++; if (j < len && isdigit((unsigned char) t[j])) { while (j < len &&
        isdigit((unsigned char) t[j])) j++;
        i = j; fl = 1; } }
    if (fl && i == len) { tree_t *e = ast_node_new(TT_FLIT); e->v.dval = atof(t); if (!memchr(t, 'e', (size_t) len) && !memchr(t, 'E', (size_t) len)) { SB rs = { 0 }; for (int k = 0; k < len; k++) sb_c(&rs, t[k]); expr_add_child(e, leaf_sval(TT_QLIT, sb_str(&rs))); } it->t = e; return; }
    SB s = { 0 }; for (int k = 0; k < len; k++) if (t[k] != '_') sb_c(&s, t[k]);
    char *u = sb_str(&s);
    if (!strncmp(u, "Inf", 3) || !strncmp(u, "NaN", 3)) { it->t = var_node(b, u); return; }
    if (strchr(u, '.') || ((strchr(u, 'e') || strchr(u, 'E')) && strncmp(u, "0x", 2))) {
        tree_t *e = ast_node_new(TT_FLIT);
        e->v.dval = atof(u);
        if (strchr(u, '.') && !strchr(u, 'e') && !strchr(u, 'E') && (isdigit((unsigned char) u[0]) || (u[0] == '.' && isdigit((unsigned char) u[1])))) expr_add_child(e, leaf_sval(TT_QLIT, u));
        it->t = e;
        return;
    }
    if (u[0] == '0' && (u[1] == 'b' || u[1] == 'B')) { it->t = rk_digits_fit(u + 2, 2) ? rk_ilit(strtoll(u + 2, NULL, 2)) : rk_big_lit(2, u + 2, 0); return; }
    if (u[0] == '0' && (u[1] == 'o' || u[1] == 'O')) { it->t = rk_digits_fit(u + 2, 8) ? rk_ilit(strtoll(u + 2, NULL, 8)) : rk_big_lit(8, u + 2, 0); return; }
    if (u[0] == '0' && (u[1] == 'd' || u[1] == 'D')) { it->t = rk_digits_fit(u + 2, 10) ? rk_ilit(strtoll(u + 2, NULL, 10)) : rk_big_lit(10, u + 2, 0); return; }
    if (u[0] == '0' && (u[1] == 'x' || u[1] == 'X') && !rk_digits_fit(u + 2, 16)) { it->t = rk_big_lit(16, u + 2, 0); return; }
    int ux = u[0] == '0' && (u[1] == 'x' || u[1] == 'X');
    if (!ux && strlen(u) >= 19 && !rk_digits_fit(u, 10)) { it->t = rk_big_lit(10, u, 0); return; }
    it->t = rk_ilit(strtoll(u, NULL, ux ? 16 : 10));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int b_quote_fl(RkB *b, RkTerm *it, RkClosure *cl, int ncl, int bf, int bt, int qf) {
    if (qf & (RKQ_WW | RKQ_UNSUP)) return 0;
    if ((qf & RKQ_WORDS) && (qf & (RKQ_SC | RKQ_AR | RKQ_HS | RKQ_FN | RKQ_CLOS))) return 0;
    tree_t *t = b_dq_fl(b, cl, ncl, bf, bt, qf);
    if (!(qf & RKQ_WORDS)) { it->t = t; return 1; }
    if (!t || t->t != TT_QLIT || !t->v.sval) return 0;
    it->kind = TK_WORDS; it->name = spn(b, bf, bt);
    it->t = b_words(b, t->v.sval, (int) strlen(t->v.sval), 0);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_words(RkB *b, RkTerm *it, int from, int to, int ifrom, int ito) {
    memset(it, 0, sizeof *it); it->kind = TK_WORDS; it->from = from; it->to = it->core_to = to;
    it->name = spn(b, ifrom, ito);
    it->t = b_words(b, b->s + ifrom, ito - ifrom, 1);
    {
        const char *wt = b->s + ifrom;
        int wl = ito - ifrom, k = 0, nd1 = 0, nd2 = 0, kind = 0;
        while (k < wl && isspace((unsigned char) wt[k])) k++;
        int ws0 = k;
        if (k < wl && (wt[k] == '-' || wt[k] == '+')) k++;
        while (k < wl && isdigit((unsigned char) wt[k])) { k++; nd1++; }
        if (nd1 && k < wl && wt[k] == '/') { k++; while (k < wl && isdigit((unsigned char) wt[k])) { k++; nd2++; } kind = nd2 ? 1 : 0; }
        else if (nd1 && k < wl && wt[k] == '.') { k++; while (k < wl && isdigit((unsigned char) wt[k])) { k++; nd2++; } kind = nd2 ? 2 : 0; }
        int we = k;
        while (k < wl && isspace((unsigned char) wt[k])) k++;
        if (kind && k == wl) {
            SB rs = { 0 };
            for (int q = ws0; q < we; q++) if (wt[q] != '+') sb_c(&rs, wt[q]);
            tree_t *c = make_call("__rk_ratlit");
            expr_add_child(c, leaf_sval(TT_QLIT, sb_str(&rs)));
            it->t = c;
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_package(RkB *b, const char *kind, const char *name, int namelen, RkTrait *tr, int ntr, tree_t *body, tree_t *rsig) {
    (void) b;
    char *nm = trimdup(name ? name : "", name ? namelen : 0);
    tree_e k = !strcmp(kind, "class") ? TT_CLASS_DECL : !strcmp(kind, "role") ? TT_ROLE_DECL : !strcmp(kind, "grammar") ? TT_GRAMMAR_DECL : TT_MODULE_DECL;
    tree_t *cd = ast_node_new(k);
    tree_t *nv = leaf_sval(TT_VAR, nm);
    if (k == TT_CLASS_DECL) {
        for (int i = 0; i < ntr; i++) {
            char tag = !strcmp(tr[i].word, "is") ? 'i' : !strcmp(tr[i].word, "does") ? 'd' : 0;
            if (!tag || !tr[i].name) continue;
            SB s = { 0 }; sb_c(&s, tag); for (const char *c = tr[i].name; *c; c++) sb_c(&s, *c);
            tree_t *q = leaf_sval(TT_QLIT, sb_str(&s));
            if (tag == 'd' && tr[i].args) ast_push(q, tr[i].args);
            ast_push(nv, q);
        }
    }
    if (k == TT_ROLE_DECL) for (int i = 0; rsig && i < rsig->n; i++) {
        tree_t *pv = rsig->c[i];
        if (pv && pv->t == TT_ASSIGN && pv->n) pv = pv->c[0];
        if (!pv || pv->t != TT_VAR || !pv->v.sval) continue;
        const char *pn = pv->v.sval;
        if (!*pn && pv->n && pv->c[0] && pv->c[0]->t == TT_QLIT && pv->c[0]->v.sval && !strncmp(pv->c[0]->v.sval, "::", 2)) pn = pv->c[0]->v.sval;
        if (!*pn) continue;
        SB s = { 0 }; sb_c(&s, 'r'); for (const char *c = pn; *c; c++) sb_c(&s, *c);
        ast_push(nv, leaf_sval(TT_QLIT, sb_str(&s)));
    }
    ast_push(cd, nv);
    for (int i = 0; body && i < body->n; i++) if (body->c[i]) ast_push(cd, body->c[i]);
    return cd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_heredoc_stub(RkB *b, RkTerm *it, int from, int to) {
    (void) b;
    memset(it, 0, sizeof *it);
    it->kind = TK_QUOTE;
    it->from = from;
    it->to = it->core_to = to;
    it->t = leaf_sval(TT_QLIT, "");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_dedent(tree_t *t, int indent, int *bol) {
    if (!t) return;
    if (t->t == TT_CAT) {
        for (int i = 0; i < t->n; i++) rk_dedent(t->c[i], indent, bol);
        return;
    }
    if (t->t != TT_QLIT || !t->v.sval) { *bol = 0; return; }
    SB out = { 0 };
    for (const char *c = t->v.sval; *c; c++) {
        if (*bol) { int k = 0; while (k < indent && (*c == ' ' || *c == '\t')) { k = *c == '\t' ? (k / 8 + 1) * 8 : k + 1; c++; } *bol = 0; if (!*c) break; }
        sb_c(&out, *c);
        if (*c == '\n') *bol = 1;
    }
    t->v.sval = sb_str(&out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_heredoc_body(RkB *b, tree_t *node, int from, int to, int indent, int interp, RkClosure *cl, int ncl) {
    tree_t *t = interp ? b_dq(b, cl, ncl, from, to) : b_sq(b, from, to);
    int bol = 1;
    if (indent > 0) rk_dedent(t, indent, &bol);
    t->line = node->line;
    *node = *t;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_finish(RkB *b, tree_t *list, const char *text, int len) {
    (void) b;
    if (!list) return;
    char *raw = (char *) ct_alloc((size_t) len + 1);
    memcpy(raw, text, (size_t) len);
    raw[len] = 0;
    tree_t *v = leaf_sval(TT_VAR, "=finish");
    v->slen |= 4;
    tree_t *as = expr_binary(TT_ASSIGN, v, leaf_sval(TT_QLIT, raw));
    ast_push(list, as);
    memmove(list->c + 1, list->c, (size_t) (list->n - 1) * sizeof(tree_t *));
    list->c[0] = as;
}
