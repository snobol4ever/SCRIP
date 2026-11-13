#include "parsers/raku/rk_syntax/rks_head.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ir/ct_arena.h"
#include "parsers/snobol4/scrip_cc.h"
#include "parsers/raku/rk_syntax/rks_01_sh.h"
#include "parsers/raku/rk_syntax/rks_02_sh.h"
#include "parsers/raku/rk_syntax/rks_03_sh.h"
#include "parsers/raku/rk_syntax/rks_04_sh.h"
#include "parsers/raku/rk_syntax/rks_05_sh.h"
#include "parsers/raku/rk_syntax/rks_06_sh.h"
#include "parsers/raku/rk_syntax/rks_09_sh.h"
#include "parsers/raku/rk_core_names.inc"
#include "parsers/raku/raku_setting.inc"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int is_numeric_other_cp(int c) {
    return (c >= 0xB2 && c <= 0xB3) || c == 0xB9 || (c >= 0xBC && c <= 0xBE) || (c >= 0x2070 && c <= 0x2079 && c != 0x2071 && c != 0x2072 && c != 0x2073) || (c >= 0x2080 && c <= 0x2089) ||
           (c >= 0x2150 && c <= 0x2189) || (c >= 0x2460 && c <= 0x249B) || (c >= 0x24EA && c <= 0x24FF) || (c >= 0x2776 && c <= 0x2793) || (c >= 0x3007 && c <= 0x3007) ||
           (c >= 0x3021 && c <= 0x3029) || (c >= 0x3038 && c <= 0x303A) || (c >= 0x3192 && c <= 0x3195) || (c >= 0x3220 && c <= 0x3229) || (c >= 0x3248 && c <= 0x324F) ||
           (c >= 0x3251 && c <= 0x325F) || (c >= 0x3280 && c <= 0x3289) || (c >= 0x32B1 && c <= 0x32BF) || (c >= 0x10107 && c <= 0x10133) || (c >= 0x10140 && c <= 0x10178) ||
           (c >= 0x12400 && c <= 0x1246E) || (c >= 0x1369 && c <= 0x137C) || (c >= 0x1F100 && c <= 0x1F10C);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int core_name_cmp(const void *a, const void *b) { return strcmp(*(const char *const *) a, rk_pool + *(const unsigned *) b); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int core_has(const unsigned *tab, size_t n, const char *s, int len) {
    if (len <= 0) return 0;
    char *buf = (char *) ct_alloc((size_t) len + 1);
    memcpy(buf, s, (size_t) len); buf[len] = 0;
    const char *key = buf;
    return bsearch(&key, tab, n, sizeof *tab, core_name_cmp) != NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_pseudo_pkg(const char *s, int n) {
    static const char *const ps[] = { "GLOBAL", "OUR", "MY", "CORE", "SETTING", "OUTER", "CALLER", "DYNAMIC", "PROCESS", "COMPILING", "UNIT", "LEXICAL", "CLIENT", "OUTERS", "CALLERS", "EXPORT", 0 };
    for (int i = 0; ps[i]; i++) if ((int) strlen(ps[i]) == n && !memcmp(ps[i], s, (size_t) n)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int is_name_n(RkP *p, const char *s, int n) {
    int i0 = 0; while (i0 + 1 < n && !(s[i0] == ':' && s[i0 + 1] == ':')) i0++;
    if (i0 + 1 >= n && is_pseudo_pkg(s, n)) return 1;
    while (n > 0) {
        int i = 0; while (i + 1 < n && !(s[i] == ':' && s[i + 1] == ':')) i++;
        if (i + 1 < n && is_pseudo_pkg(s, i)) return 1;
        break;
    }
    if (n >= 2 && s[n - 1] == ':' && s[n - 2] == ':') n -= 2;
    if (n <= 0) return 1;
    if (user_name_index(p, s, n) >= 0) return 1;
    if (core_has(rk_core_names, sizeof rk_core_names / sizeof *rk_core_names, s, n)) return 1;
    if (p->lang_e && core_has(rk_core_e_names, sizeof rk_core_e_names / sizeof *rk_core_e_names, s, n)) return 1;
    if (i0 + 1 < n && i0 > 0 && user_name_index(p, s, i0) >= 0) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_is_core_routine(const char *s) {
    return s && *s >= 'a' && *s <= 'z' && !strchr(s, ':') && core_has(rk_core_routines, sizeof rk_core_routines / sizeof *rk_core_routines, s, (int) strlen(s));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_is_core_type(const char *s) {
    static const char *const vals[] = { "True", "False", "Less", "Same", "More", "Inf", "NaN", "Empty", "Nil", 0 };
    static const char *const natives[] = { "int", "int8", "int16", "int32", "int64", "uint", "uint8", "uint16", "uint32", "uint64", "byte", "num", "num32", "num64", "str", "bit", "array", "buf8", "buf16", "buf32", "buf64", "blob8", "blob16", "blob32", "blob64", 0 };
    for (int i = 0; s && natives[i]; i++) if (!strcmp(natives[i], s)) return 1;
    if (!s || !(*s >= 'A' && *s <= 'Z') || strchr(s, ':')) return 0;
    for (int i = 0; vals[i]; i++) if (!strcmp(vals[i], s)) return 0;
    return core_has(rk_core_names, sizeof rk_core_names / sizeof *rk_core_names, s, (int) strlen(s));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int routine_visible(RkP *p, const char *s, int n) {
    if (n <= 0) return 1;
    char *buf = (char *) ct_alloc((size_t) n + 1); buf[0] = '&'; memcpy(buf + 1, s, (size_t) n);
    if (user_name_index(p, buf, n + 1) >= 0 || user_name_index(p, s, n) >= 0) return 1;
    if (core_has(rk_core_routines, sizeof rk_core_routines / sizeof *rk_core_routines, s, n)) return 1;
    if (p->lang_e && core_has(rk_core_e_routines, sizeof rk_core_e_routines / sizeof *rk_core_e_routines, s, n)) return 1;
    return core_has(rk_core_names, sizeof rk_core_names / sizeof *rk_core_names, s, n);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void add_mystery(RkP *p, int pos, int len) {
    note_core_use(p, pos, len);
    int c0 = cp_at(p, pos);
    if (p->myst_off || routine_visible(p, p->s + pos, len) || !is_alpha_cp(c0) || is_numeric_other_cp(c0) || (c0 >= 0x80 && cp_len(p, pos) == len)) return;
    if (len == 9 && !memcmp(p->s + pos, "GLOBALish", 9)) return;
    for (int i = p->nmyst - 1; i >= 0; i--) if (p->myst[i].pos == pos) return;
    RK_GROW(p->myst, p->nmyst, p->cmyst, RkMyst);
    p->myst[p->nmyst].pos = pos; p->myst[p->nmyst].len = len; p->myst[p->nmyst].depth = p->depth;
    p->nmyst++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int core_export_has(const char *s, int n) {
    for (int i = 0; RK_SETTING_MODULES[i].name; i++) if (!strcmp(RK_SETTING_MODULES[i].name, "CORE")) {
        const char *e = RK_SETTING_MODULES[i].exports;
        for (int k = 0; e[k];) {
            int j = k; while (e[j] && e[j] != ' ') j++;
            if (j - k == n && !memcmp(e + k, s, (size_t) n)) return 1;
            k = e[j] ? j + 1 : j;
        }
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void note_core_use(RkP *p, int pos, int len) {
    if (!core_export_has(p->s + pos, len)) return;
    RK_GROW(p->core_uses, p->ncore_uses, p->ccore_uses, RkMyst);
    p->core_uses[p->ncore_uses].pos = pos; p->core_uses[p->ncore_uses].len = len; p->core_uses[p->ncore_uses].depth = 0;
    p->ncore_uses++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void resolve_mysteries(RkP *p, int depth) {
    int k = 0;
    for (int i = 0; i < p->nmyst; i++) {
        RkMyst m = p->myst[i];
        if (m.depth > depth) { if (routine_visible(p, p->s + m.pos, m.len)) continue; m.depth = depth; }
        p->myst[k++] = m;
    }
    p->nmyst = k;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void scope_enter(RkP *p) { p->depth++; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void scope_leave(RkP *p) {
    resolve_mysteries(p, p->depth - 1);
    p->depth--;
    while (p->nnames > 0 && p->names[p->nnames - 1].depth > p->depth) p->nnames--;
    int k = 0;
    for (int i = 0; i < p->nuops; i++) if (p->uops[i].depth <= p->depth || p->uops[i].depth >= 1000) p->uops[k++] = p->uops[i];
    p->nuops = k;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int uniname_cmp(const void *a, const void *b) { return strcmp((const char *) a, rk_pool + ((const RkUniName *) b)->name); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static unsigned uniname_cp(const char *s, int n) {
    while (n > 0 && *s == ' ') { s++; n--; }
    while (n > 0 && s[n - 1] == ' ') n--;
    if (n <= 0) return 0;
    char *buf = (char *) ct_alloc((size_t) n + 1); int k = 0;
    int digits = 1;
    for (int i = 0; i < n; i++) { char c = s[i]; if (c < '0' || c > '9') digits = 0; buf[k++] = (char) (c >= 'a' && c <= 'z' ? c - 32 : c); }
    buf[k] = 0;
    if (digits) return (unsigned) strtoul(buf, NULL, 10);
    const RkUniName *u = (const RkUniName *) bsearch(buf, rk_uninames, sizeof rk_uninames / sizeof *rk_uninames, sizeof *rk_uninames, uniname_cmp);
    if (u) return (unsigned) u->cp;
    static const char *const alg[] = { "CJK UNIFIED IDEOGRAPH-", "CJK COMPATIBILITY IDEOGRAPH-", "TANGUT IDEOGRAPH-", "KHITAN SMALL SCRIPT CHARACTER-", "NUSHU CHARACTER-", 0 };
    for (int i = 0; alg[i]; i++) {
        size_t l = strlen(alg[i]);
        if (strncmp(buf, alg[i], l) || !buf[l]) continue;
        char *e; unsigned long v = strtoul(buf + l, &e, 16);
        return *e ? 0 : (unsigned) v;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
unsigned rk_uniname_cp(const char *s, int n) { return uniname_cp(s, n); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int uniseq_cmp(const void *a, const void *b) { return strcmp((const char *) a, rk_pool + ((const RkUniSeq *) b)->name); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rk_uniname_seq(const char *s, int n) {
    while (n > 0 && *s == ' ') { s++; n--; }
    while (n > 0 && s[n - 1] == ' ') n--;
    if (n <= 0) return NULL;
    char *buf = (char *) ct_alloc((size_t) n + 1);
    for (int i = 0; i < n; i++) buf[i] = (char) (s[i] >= 'a' && s[i] <= 'z' ? s[i] - 32 : s[i]);
    buf[n] = 0;
    const RkUniSeq *q = (const RkUniSeq *) bsearch(buf, rk_uniseqs, sizeof rk_uniseqs / sizeof *rk_uniseqs, sizeof *rk_uniseqs, uniseq_cmp);
    return q ? rk_pool + q->cps : NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void add_user_op(RkP *p, char cat, const char *sym, int len, int prec) {
    if (len <= 0) return;
    RK_GROW(p->uops, p->nuops, p->cuops, RkUserOp);
    RkUserOp *u = &p->uops[p->nuops++];
    char *d = (char *) ct_alloc((size_t) len * 4 + 1); int k = 0;
    for (int i = 0; i < len; i++) {
        if (sym[i] == '\\' && i + 2 < len && (sym[i + 1] == 'x' || sym[i + 1] == 'c') && sym[i + 2] == '[') {
            int j = i + 3; unsigned v = 0;
            if (sym[i + 1] == 'c') { while (j < len && sym[j] != ']') j++; v = uniname_cp(sym + i + 3, j - i - 3); if (!v) { d[k++] = sym[i]; continue; } }
            else while (j < len && sym[j] != ']') { char h = sym[j]; v = v * 16 + (unsigned) (h >= 'a' ? h - 'a' + 10 : h >= 'A' ? h - 'A' + 10 : h - '0'); j++; }
            if (v < 0x80) d[k++] = (char) v;
            else if (v < 0x800) { d[k++] = (char) (0xC0 | (v >> 6)); d[k++] = (char) (0x80 | (v & 0x3F)); }
            else if (v < 0x10000) { d[k++] = (char) (0xE0 | (v >> 12)); d[k++] = (char) (0x80 | ((v >> 6) & 0x3F)); d[k++] = (char) (0x80 | (v & 0x3F)); }
            else { d[k++] = (char) (0xF0 | (v >> 18)); d[k++] = (char) (0x80 | ((v >> 12) & 0x3F)); d[k++] = (char) (0x80 | ((v >> 6) & 0x3F)); d[k++] = (char) (0x80 | (v & 0x3F)); }
            i = j; continue;
        }
        if (sym[i] == '\\' && i + 1 < len) i++;
        d[k++] = sym[i];
    }
    d[k] = 0;
    u->sym = d; u->len = k; u->prec = prec; u->depth = p->depth; u->cat = cat; u->assoc = AS_LEFT; u->flags = 0;
    { int q = k - 1; while (q > 0 && (((unsigned char) d[q]) & 0xC0) == 0x80) q--; int l; u->wordend = is_word_cp(rk_decode(d, k, q, &l)); }
    if (p->B && cat == 't') rkb_user_op(p->B, 't', d, k, d, k, 0);
    if (p->B && (cat == 'i' || cat == 'p' || cat == 'P')) {
        const RkOp *tab = cat == 'i' ? rk_infix : cat == 'p' ? rk_prefix : rk_postfix;
        int builtin = 0;
        for (int i = 0; tab[i].sym; i++) if (!strcmp(tab[i].sym, d)) { builtin = 1; break; }
        if (!builtin) rkb_user_op(p->B, cat, d, k, d, k, prec);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int line_end(RkP *p, int pos) { while (pos < p->n && p->s[pos] != '\n') pos++; return pos < p->n ? pos + 1 : pos; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int blank_line_at(RkP *p, int pos) {
    int q = pos;
    q = NSPAN(p, q, ck(is_hspace_cp));
    return q >= p->n || p->s[q] == '\n' || p->s[q] == '\r';
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_pod(RkP *p, int pos) {
    int q = pos;
    q = NSPAN(p, q, ck(is_hspace_cp));
    if (ch(p, q) != '=') return -1;
    int c = cp_at(p, q + 1);
    if (!(is_word_cp(c) || c == '\\')) return -1;
    q++;
    int nl; int e = pod_ident(p, q, &nl);
    if (nl == 6 && !memcmp(p->s + q, "finish", 6)) { p->finished = 1; p->finish_from = line_end(p, e); return p->n; }
    if (nl == 5 && !memcmp(p->s + q, "begin", 5)) {
        int r = e;
        r = NSPAN(p, r, ck(is_hspace_cp));
        int bl; int be = pod_ident(p, r, &bl);
        if (bl == 0) panic_at(p, q, "Pod block without a name after =begin");
        const char *name = p->s + r;
        if (bl == 6 && !memcmp(name, "finish", 6)) { p->finished = 1; p->finish_from = line_end(p, be); return p->n; }
        int depth = 1;
        int l = line_end(p, be);
        while (l < p->n) {
            int t = l;
            t = NSPAN(p, t, ck(is_hspace_cp));
            if (p->s[t] == '=' && t + 1 < p->n) {
                int dl; int de = pod_ident(p, t + 1, &dl);
                int u = NSPAN(p, de, ck(is_hspace_cp));
                int nl2; pod_ident(p, u, &nl2);
                if (nl2 == bl && !memcmp(p->s + u, name, (size_t) bl)) {
                    if (dl == 5 && !memcmp(p->s + t + 1, "begin", 5)) depth++;
                    else if (dl == 3 && !memcmp(p->s + t + 1, "end", 3)) { if (--depth == 0) return line_end(p, u); }
                }
            }
            l = line_end(p, l);
        }
        panic_at(p, pos, "Couldn't find =end %.*s", bl, name);
    }
    if (nl == 6 && !memcmp(p->s + q, "config", 6)) {
        int l = line_end(p, e);
        while (l < p->n && !blank_line_at(p, l) && (p->s[l] == ' ' || p->s[l] == '\t') && p->s[l] != '=') l = line_end(p, l);
        return l;
    }
    int l = line_end(p, e);
    while (l < p->n && !blank_line_at(p, l)) {
        int t = NSPAN(p, l, ck(is_hspace_cp));
        if (p->s[t] == '=' && t + 1 < p->n && (is_word_cp(cp_at(p, t + 1)))) break;
        l = line_end(p, l);
    }
    return l;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_block(RkP *p, int pos) {
    if (ch(p, pos) != '{') {
        int m = marked_ws_from(p, pos);
        int at = m >= 0 ? m : pos;
        if (at > 0 && p->s[at - 1] == '}') panic_at(p, at, "Missing block (whitespace needed before curlies taken as a hash subscript?)");
        panic_at(p, pos, "Missing block");
    }
    scope_enter(p);
    int e = r_blockoid(p, pos);
    scope_leave(p);
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_blockoid(RkP *p, int pos) {
    if (at_lit(p, pos, "{YOU_ARE_HERE}")) panic_at(p, pos, "Reserved use of {YOU_ARE_HERE} outside of a setting");
    if (ch(p, pos) != '{') panic_at(p, pos, "Missing block");
    int noend = p->quote_block; p->quote_block = 0;
    int sg = p->goal; int sq = p->qsigil; int sr = p->in_reduce; int sm = p->in_meta; int sa = p->invocant_ok; int sd = p->in_decl; int sl = p->leftsigil; int ss = p->scope;
    const char *su = p->ustop; int sul = p->ustop_len; int s5 = p->p5isms;
    p->goal = 0; p->qsigil = 0; p->in_reduce = 0; p->in_meta = 0; p->in_decl = 0; p->scope = 0; p->ustop = NULL; p->ustop_len = 0;
    int bk = p->next_bk ? p->next_bk : BK_BLOCK; p->next_bk = 0;
    int sbk = p->bk; tree_t *scl = p->cur_list;
    tree_t *list = p->build ? ast_node_new(TT_SEQ_EXPR) : NULL;
    p->bk = bk; p->cur_list = list;
    int e = r_statementlist(p, pos + 1);
    p->bk = sbk; p->cur_list = scl;
    if (p->build) {
        p->blk_last = p->last_list; p->blk_n = p->last_nstmts; p->blk_list = list;
        p->tv = (bk == BK_GIVEN || bk == BK_CATCH) ? NULL : rkb_block_seq(p->B, list, bk, 0);
    }
    p->goal = sg; p->qsigil = sq; p->in_reduce = sr; p->in_meta = sm; p->invocant_ok = sa; p->in_decl = sd; p->leftsigil = sl; p->scope = ss; p->ustop = su; p->ustop_len = sul; p->p5isms = s5;
    if (ch(p, e) != '}') {
        if (e >= p->n) panic_at(p, e, "Missing block (couldn't find final '}' of block starting at line %d)", line_of(p, pos));
        panic_at(p, e, "Confused");
    }
    return noend ? e + 1 : r_ENDSTMT(p, e + 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_statementlist(RkP *p, int pos) {
    int q = ws(p, pos);
    RkList *last = NULL; int n = 0;
    for (;;) {
        if (p->finished) { q = p->n; break; }
        int c = ch(p, q);
        if (q >= p->n || c == ')' || c == ']' || c == '}') break;
        p->stmt_list = NULL;
        int e = r_statement(p, q);
        if (e < 0) break;
        last = p->stmt_list; n++;
        q = r_eat_terminator(p, e);
        q = ws(p, q);
    }
    p->last_list = last; p->last_nstmts = n;
    return q;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void add_use_lib(RkP *p, int from, int to) {
    int q = from;
    while (q < to) {
        int parent = 1; const char *lit_s = NULL; int lit_n = 0; int prog = 0;
        int e = q;
        if (at_lit(p, q, "$*PROGRAM")) {
            prog = 1; e = q + 9;
            while (e < p->n && p->s[e] != ',' && p->s[e] != ';' && p->s[e] != '\n') {
                if (at_lit(p, e, ".parent(")) { parent = atoi(p->s + e + 8); e += 8; continue; }
                if (at_lit(p, e, ".parent")) { parent = 1; e += 7; continue; }
                if (p->s[e] == '\'' || p->s[e] == '"') { char qc = p->s[e]; int b = e + 1; int t = b; while (t < p->n && p->s[t] != qc) t++; lit_s = p->s + b; lit_n = t - b; e = t + 1; continue; }
                e++;
            }
        }
        else if (p->s[q] == '\'' || p->s[q] == '"') { char qc = p->s[q]; int b = q + 1; int t = b; while (t < to && p->s[t] != qc) t++; lit_s = p->s + b; lit_n = t - b; e = t + 1; }
        else if (p->s[q] == '<') { int b = q + 1; int t = b; while (t < to && p->s[t] != '>') t++; lit_s = p->s + b; lit_n = t - b; e = t + 1; }
        if (lit_s) {
            size_t flen = strlen(p->file);
            size_t dcap = flen + 2;
            size_t bcap = flen + (size_t) lit_n + 4;
            char *buf = (char *) ct_alloc(bcap);
            if (prog) {
                char *dir = (char *) ct_alloc(dcap); snprintf(dir, dcap, "%s", p->file);
                for (int k = 0; k < parent; k++) { char *sl = strrchr(dir, '/'); if (sl) *sl = 0; else snprintf(dir, dcap, "."); }
                snprintf(buf, bcap, "%s/%.*s", dir, lit_n, lit_s);
            }
            else if (lit_n && lit_s[0] == '/') snprintf(buf, bcap, "%.*s", lit_n, lit_s);
            else { char *dir = (char *) ct_alloc(dcap); snprintf(dir, dcap, "%s", p->file); char *sl = strrchr(dir, '/'); if (sl) *sl = 0; else snprintf(dir, dcap, "."); snprintf(buf, bcap, "%s/%.*s",
                dir, lit_n, lit_s); }
            RK_GROW(p->libs, p->nlibs, p->clibs, char *);
            p->libs[p->nlibs++] = ct_strndup0(buf, (int) strlen(buf));
            if (!prog && lit_n && lit_s[0] != '/') { RK_GROW(p->libs, p->nlibs, p->clibs, char *); p->libs[p->nlibs++] = ct_strndup0(lit_s, lit_n); }
        }
        q = e > q ? e : q + 1;
        while (q < to && (p->s[q] == ',' || asc_space((unsigned char) p->s[q]))) q++;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int import_module(RkP *p, const char *name) {
    if (p->load_depth >= 4) return 0;
    char *rel = (char *) ct_alloc(strlen(name) + 1); int k = 0;
    for (int i = 0; name[i]; i++) { if (name[i] == ':' && name[i + 1] == ':') { rel[k++] = '/'; i++; } else rel[k++] = name[i]; }
    rel[k] = 0;
    static const char *const exts[] = { ".rakumod", ".pm6", ".pm" };
    for (int i = 0; RK_SETTING_MODULES[i].name; i++) if (!strcmp(RK_SETTING_MODULES[i].name, name)) {
        size_t pl = strlen(name) + 20;
        char *path = (char *) ct_alloc(pl);
        snprintf(path, pl, "<setting>/%s.rakumod", rel);
        return import_module_src(p, RK_SETTING_MODULES[i].src, (int) strlen(RK_SETTING_MODULES[i].src), path);
    }
    for (int l = p->nlibs - 1; l >= 0; l--) {
        for (int x = 0; x < 6; x++) {
            const char *ext = exts[x % 3], *sub = x < 3 ? "" : "lib/";
            size_t pl = strlen(p->libs[l]) + strlen(rel) + strlen(ext) + 6;
            char *path = (char *) ct_alloc(pl); snprintf(path, pl, "%s/%s%s%s", p->libs[l], sub, rel, ext);
            FILE *f = fopen(path, "rb");
            if (!f) continue;
            fseek(f, 0, SEEK_END); long n = ftell(f); rewind(f);
            char *src = (char *) ct_alloc((size_t) n + 1);
            size_t got = fread(src, 1, (size_t) n, f); fclose(f); src[got] = 0;
            return import_module_src(p, src, (int) got, path);
        }
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_module_name(RkP *p, int pos) {
    int e = r_longname(p, pos);
    if (e < 0) return -1;
    if (ch(p, e) == '[') { int a = r_arglist(p, e + 1); a = ws(p, a); if (ch(p, a) != ']') panic_at(p, a, "Unable to parse generic role; couldn't find final ']'"); e = a + 1; }
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_use_like(RkP *p, int pos, const char *what) {
    int q = pos;
    int v = r_version(p, q);
    if (v >= 0) {
        if (!strcmp(what, "use")) {
            if (!p->comp_unit_begin) panic_at(p, q, "Too late to switch language version. Must be used as the very first statement.");
            if (p->build) p->tv = rkb_use(p->B, p->s + q, v - q, NULL);
            return ws(p, v);
        }
        return ws(p, v);
    }
    int m = r_module_name(p, q);
    if (m < 0) panic_at(p, q, "Malformed %s", what);
    char *name = ct_strndup0(p->s + q, name_part_len(p, q, m));
    if (!strcmp(what, "use") && !strcmp(name, "parameters")) panic_at(p, q, "use parameters not yet implemented. Sorry.");
    if (!strcmp(name, "isms")) p->p5isms = !strcmp(what, "use");
    if (!strcmp(name, "strict")) p->lax = !strcmp(what, "no");
    int e = m;
    int c = cp_at(p, e);
    int args_from = e;
    RkList *uargs = NULL;
    if (is_space_cp(c) || c == '#') {
        int t = ws(p, e);
        if (!stdstopper(p, t)) { int a = r_arglist(p, e); e = a; uargs = takelst(p); }
    }
    static const char *const pragmas[] = { "lib", "soft", "strict", "fatal", "MONKEY", "dynamic-scope", "isms", "variables", "attributes", "invocant", "parameters", "experimental", "newline",
                                           "internals", "nqp", "precompilation", "trace", "worries", "v6", "BUILDPLAN", 0 };
    int is_pragma = !strncmp(name, "MONKEY-", 7);
    for (int i = 0; pragmas[i]; i++) if (!strcmp(name, pragmas[i])) is_pragma = 1;
    if (!strcmp(what, "use") && !strcmp(name, "lib")) add_use_lib(p, ws(p, args_from), e);
    else if (!strcmp(what, "use") && !is_pragma) { if (!import_module(p, name)) p->myst_off = 1; }
    if (p->build) p->tv = rkb_use(p->B, p->s + q, name_part_len(p, q, m), uargs);
    return ws(p, e);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_statement_control(RkP *p, int pos) {
    int c = ch(p, pos);
    if (!is_alpha_cp(c)) return -1;
    int e;
    if ((e = kok(p, pos, "if")) >= 0 || (e = kok(p, pos, "with")) >= 0) {
        int q = r_xblock(p, e, 0);
        const char **kws = NULL; RkList **cs = NULL; tree_t **bs = NULL; int n = 0, ck = 0, cc = 0, cb = 0; tree_t *els = NULL;
        RK_GROW(kws, n, ck, const char *); RK_GROW(cs, n, cc, RkList *); RK_GROW(bs, n, cb, tree_t *);
        kws[n] = at_lit(p, pos, "with") ? "with" : "if"; cs[n] = p->xb_cond; bs[n] = p->xb_blk; n++;
        for (;;) {
            int t = ws(p, q);
            int f;
            if (at_lit(p, t, "else") && (f = hs(p, t + 4)) && at_lit(p, f, "if") && !wordch_at(p, f + 2)) panic_at(p, t, "In Raku, please use \"elsif\" instead of \"else if\"");
            if ((f = kw(p, t, "elif")) >= 0) panic_at(p, t, "In Raku, please use \"elsif\" instead of \"elif\"");
            if ((f = kok(p, t, "elsif")) >= 0 || (f = kok(p, t, "orwith")) >= 0) { q = r_xblock(p, f, 0); RK_GROW(kws, n, ck, const char *); RK_GROW(cs, n, cc, RkList *);
                RK_GROW(bs, n, cb, tree_t *); kws[n] = at_lit(p, t, "orwith") ? "orwith" : "elsif"; cs[n] = p->xb_cond; bs[n] = p->xb_blk; n++; continue; }
            if ((f = kw(p, t, "else")) >= 0) { int g = ws(p, f); int h = r_pblock(p, g, 0); els = p->tv; if (p->build) p->tv = rkb_if(p->B, n, kws, cs, bs, els); return h; }
            if (p->build) p->tv = rkb_if(p->B, n, kws, cs, bs, NULL);
            return q;
        }
    }
    if ((e = kok(p, pos, "unless")) >= 0 || (e = kok(p, pos, "without")) >= 0) {
        int q = r_xblock(p, e, 0);
        if (p->build) p->tv = rkb_unless(p->B, p->xb_cond, p->xb_blk, at_lit(p, pos, "without"));
        int t = ws(p, q);
        if (kw(p, t, "else") >= 0 || kw(p, t, "elsif") >= 0 || kw(p, t, "orwith") >= 0) panic_at(p, t, "\"%s\" does not take \"%.*s\", please rewrite using \"if\"", at_lit(p, pos,
            "unless") ? "unless" : "without", kw(p, t, "else") >= 0 ? 4 : kw(p, t, "elsif") >= 0 ? 5 : 6, p->s + t);
        return q;
    }
    if ((e = kok(p, pos, "while")) >= 0 || (e = kok(p, pos, "until")) >= 0) {
        int q = r_xblock(p, e, 0);
        if (p->build) { rkb_set_after_line(p->B, q); p->tv = rkb_while(p->B, ch(p, pos) == 'u', p->xb_cond, p->xb_blk); }
        return q;
    }
    if ((e = kok(p, pos, "repeat")) >= 0) {
        int f;
        if ((f = kok(p, e, "while")) >= 0 || (f = kok(p, e, "until")) >= 0) {
            int q = r_xblock(p, f, 0);
            if (p->build) { rkb_set_after_line(p->B, q); p->tv = rkb_repeat(p->B, ch(p, e) == 'u', p->xb_blk, p->xb_cond); }
            return q;
        }
        int q = r_pblock(p, e, 0);
        tree_t *blk = p->tv;
        int t = ws(p, q);
        if ((f = kok(p, t, "while")) < 0 && (f = kok(p, t, "until")) < 0) panic_at(p, t, "Missing \"while\" or \"until\"");
        int x = r_EXPR(p, f, 0);
        if (x < 0) panic_at(p, f, "Missing expression");
        if (p->build) { RkList *cx = takelst(p); rkb_set_after_line(p->B, x); p->tv = rkb_repeat(p->B, ch(p, t) == 'u', blk, cx); p->ctl_ns = 1; }
        return x;
    }
    if ((e = kok(p, pos, "for")) >= 0) {
        if (ch(p, e) == '(') {
            int d = 1; int t = e + 1; int semis = 0;
            while (t < p->n && d) { if (p->s[t] == '(') d++; else if (p->s[t] == ')') d--; else if (p->s[t] == ';' && d == 1) semis++; t++; }
            if (semis == 2) panic_at(p, pos, "Unsupported use of C-style \"for (;;)\" loop; in Raku please use \"loop (;;)\"");
        }
        int q = r_xblock(p, e, 2);
        if (p->build) { rkb_set_after_line(p->B, q); p->tv = rkb_for(p->B, p->xb_cond, p->xb_sig, p->xb_blk); }
        return q;
    }
    if ((e = kok(p, pos, "whenever")) >= 0) { int q = r_xblock(p, e, 2); p->tv = NULL; return q; }
    if ((e = kw_end(p, pos, "foreach")) >= 0) panic_at(p, pos, "Unsupported use of 'foreach'; in Raku please use 'for'");
    if ((e = kok(p, pos, "loop")) >= 0) {
        int q = e; RkList *le0 = NULL, *le1 = NULL, *le2 = NULL; int parens = 0;
        if (ch(p, q) == '(') {
            parens = 1;
            int t = ws(p, q + 1); int n = 0;
            int x = r_EXPR(p, t, 0); if (x >= 0) { le0 = takelst(p); t = ws(p, x); n = 1; }
            if (ch(p, t) == ';') { t = ws(p, t + 1); n = 2; x = r_EXPR(p, t, 0); if (x >= 0) { le1 = takelst(p); t = ws(p, x); }
                if (ch(p, t) == ';') { t = ws(p, t + 1); n = 3; x = r_EXPR(p, t, 0); if (x >= 0) { le2 = takelst(p); t = ws(p, x); } } }
            if (n == 3 && ch(p, t) == ')') q = ws(p, t + 1);
            else if (ch(p, t) == ')') panic_at(p, t,
                n == 0 ? "Malformed loop spec (expected 3 semicolon-separated expressions)" : "Malformed loop spec (expected 3 semicolon-separated expressions but got %d)", n);
            else if (ch(p, t) == ';') panic_at(p, t, "Malformed loop spec (expected 3 semicolon-separated expressions but got more)");
            else panic_at(p, t, "Malformed loop spec");
        }
        int b = r_block(p, q);
        if (p->build) { rkb_set_after_line(p->B, b); p->tv = rkb_loop(p->B, le0, le1, le2, parens, p->tv); }
        return b;
    }
    if ((e = kok(p, pos, "need")) >= 0) {
        int q = e;
        for (;;) {
            if (r_version(p, q) >= 0) panic_at(p, q, "In case of using pragma, use \"use\" instead (e.g., \"use v6;\", \"use v6.c;\").");
            int m = r_module_name(p, q); if (m < 0) panic_at(p, q, "Malformed need");
            { const char *nm = ct_strndup0(p->s + q, name_part_len(p, q, m)); if (!import_module(p, nm) && p->load_depth < 4) panic_at(p, q, "Could not find %s", nm); }
            q = ws(p, m);
            if (ch(p, q) == ',') { q = ws(p, q + 1); continue; }
            p->tv = NULL;
            return q;
        }
    }
    if ((e = lit(p, pos, "import")) >= 0 && !wordch_at(p, e)) {
        int q = ws(p, e);
        int m = r_module_name(p, q);
        if (m < 0) panic_at(p, q, "Malformed import");
        if (!is_pseudo_pkg(p->s + q, name_part_len(p, q, m))) p->myst_off = 1;
        int t = m;
        if (is_space_cp(cp_at(p, t)) || ch(p, t) == '#') { int u = ws(p, t); if (!stdstopper(p, u)) t = r_arglist(p, t); }
        p->tv = NULL;
        return ws(p, t);
    }
    if ((e = lit(p, pos, "no")) >= 0 && !wordch_at(p, e) && is_space_cp(cp_at(p, e))) { int g = ws(p, e); int f = r_use_like(p, g, "no"); p->tv = NULL; if (p->build && at_lit(p, g, "strict") && !wordch_at(p, g + 6)) p->tv = rkb_use(p->B, "no-strict", 9, NULL); return f; }
    if ((e = lit(p, pos, "use")) >= 0 && !wordch_at(p, e) && is_space_cp(cp_at(p, e))) { int f = r_use_like(p, ws(p, e), "use"); p->ctl_ns = 1; return f; }
    if ((e = lit(p, pos, "DOC")) >= 0 && is_hspace_cp(cp_at(p, e))) { int u = hs(p, e); int f = lit(p, u, "use"); if (f >= 0 && !wordch_at(p, f)) return r_use_like(p, ws(p, f), "use"); }
    if ((e = kok(p, pos, "require")) >= 0) {
        p->myst_off = 1;
        int q = e;
        int m = r_module_name(p, q);
        if (m < 0) m = r_variable(p, q);
        if (m < 0 && ch(p, q) != '$' && ch(p, q) != '@') m = r_term(p, q);
        if (m < 0) panic_at(p, q, "Malformed require");
        q = ws(p, m);
        if (!stdstopper(p, q)) { int x = r_EXPR(p, q, 0); if (x >= 0) q = x; }
        p->tv = NULL;
        return q;
    }
    if ((e = kok(p, pos, "given")) >= 0) { p->xb_bk = BK_GIVEN; int q = r_xblock(p, e, 2); if (p->build) p->tv = rkb_given(p->B, p->xb_cond, p->blk_list); return q; }
    if ((e = kok(p, pos, "when")) >= 0) {
        tree_t *list = p->cur_list; int bk = p->bk;
        int q = r_xblock(p, e, 0);
        if (p->build) { rkb_when(p->B, list, bk, p->xb_cond, p->xb_blk); p->tv = NULL; }
        return q;
    }
    if ((e = kok(p, pos, "default")) >= 0) {
        tree_t *list = p->cur_list; int bk = p->bk;
        int q = r_block(p, e);
        if (p->build) { if (bk == BK_GIVEN || bk == BK_CATCH) rkb_default(p->B, list, p->tv); else if (list) ast_push(list, p->tv); p->tv = NULL; }
        return q;
    }
    if ((e = lit(p, pos, "CATCH")) >= 0 || (e = lit(p, pos, "CONTROL")) >= 0 || (e = lit(p, pos, "QUIT")) >= 0) {
        if (wordch_at(p, e)) return -1;
        int q = ws(p, e);
        if (ch(p, q) != '{') return -1;
        int saved = p->build ? rkb_swap_nonwhen(p->B, 0) : 0;
        p->next_bk = BK_CATCH;
        int b = r_block(p, q);
        if (p->build) { int nw = rkb_swap_nonwhen(p->B, saved); p->tv = rkb_catch(p->B, p->blk_list, nw == 0); }
        return b;
    }
    return -1;
}
static int core_name_declared(RkP *p, const char *s, int n) {
    char *buf = (char *) ct_alloc((size_t) n + 2); buf[0] = '&'; memcpy(buf + 1, s, (size_t) n);
    return user_name_index(p, buf, n + 1) >= 0 || user_name_index(p, s, n) >= 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void import_core_used(RkP *p) {
    if (p->core_imported || !p->ncore_uses || !strncmp(p->file, "<setting>/", 10)) return;
    for (int i = 0; i < p->ncore_uses; i++) if (!core_name_declared(p, p->s + p->core_uses[i].pos, p->core_uses[i].len)) {
        p->core_imported = 1;
        import_module(p, "CORE");
        return;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_comp_unit(RkP *p) {
    int q = 0;
    if (at_lit(p, 0, "\xef\xbb\xbf")) q = 3;
    if (at_lit(p, q, "#!")) q = line_end(p, q);
    p->comp_unit_begin = 1;
    q = ws(p, q);
    tree_t *list = p->build ? ast_node_new(TT_SEQ_EXPR) : NULL;
    p->cur_list = list; p->bk = BK_MAIN; p->last_real_semi = 1;
    int v = lit(p, q, "use");
    if (v >= 0 && is_space_cp(cp_at(p, v))) {
        int t = ws(p, v);
        int ve = r_version(p, t);
        if (ve >= 0) {
            if (!(ch(p, t + 1) == '6')) panic_at(p, t, "No compiler available for Raku %.*s", ve - t, p->s + t);
            if (ve - t >= 4 && p->s[t + 3] != 'c' && p->s[t + 3] != 'd') p->lang_e = 1;
            if (p->build) { int semi, last; tree_t *u = rkb_use(p->B, p->s + t, ve - t, NULL); int e0 = ve; stmt_end_info(p, e0, &semi, &last); rkb_control(p->B, list, BK_MAIN, q, u, 1, semi, last); }
            q = r_eat_terminator(p, ws(p, ve));
        }
    }
    p->comp_unit_begin = 0;
    scope_enter(p);
    int e = r_statementlist(p, q);
    if (p->build) { if (p->last_real_semi) rkb_empty(p->B, list, BK_MAIN); if (p->finished) rkb_finish(p->B, list, p->s + p->finish_from, p->n - p->finish_from); p->tv = rkb_program(p->B, list); }
    e = ws(p, e);
    if (e < p->n && !p->finished) {
        int c = ch(p, e);
        if (c == ')' || c == ']' || c == '}') panic_at(p, e, "Unexpected closing bracket");
        panic_at(p, e, "Confused");
    }
    if (p->nhere) panic_at(p, p->n, "Ending delimiter %s not found", p->here[0].delim);
    while (p->npkgs) package_close(p);
    import_core_used(p);
    resolve_mysteries(p, 0);
    int setting = !strncmp(p->file, "<setting>/", 10);
    for (int i = 0; !p->myst_off && i < p->nmyst; i++) {
        RkMyst m = p->myst[i];
        if (setting && m.len > 5 && !strncmp(p->s + m.pos, "__rk_", 5)) continue;
        panic_at(p, m.pos, "Undeclared %s: %.*s used at line %d", (p->s[m.pos] >= 'a' || p->s[m.pos] == '&') ? "routine" : "name", m.len, p->s + m.pos, line_of(p, m.pos));
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_check_into(RkP *p, const char *src, int len, const char *path, RkP *out) {
    (void) out;
    p->s = src; p->n = len; p->file = path ? path : "<stdin>";
    p->wsmark = (int *) ct_alloc(sizeof(int) * ((size_t) len + 2));
    memset(p->wsmark, 0, sizeof(int) * ((size_t) len + 2));
    p->endmark = (unsigned char *) ct_alloc((size_t) len + 2);
    memset(p->endmark, 0, (size_t) len + 2);
    if (setjmp(p->jb)) return 1;
    r_comp_unit(p);
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_syntax_check(const char *src, int len, const char *path, char **err) {
    RkP *p = (RkP *) ct_alloc(sizeof(RkP));
    memset(p, 0, sizeof *p);
    p->s = src; p->n = len; p->file = path ? path : "<stdin>";
    p->wsmark = (int *) ct_alloc(sizeof(int) * ((size_t) len + 2));
    memset(p->wsmark, 0, sizeof(int) * ((size_t) len + 2));
    p->endmark = (unsigned char *) ct_alloc((size_t) len + 2);
    memset(p->endmark, 0, (size_t) len + 2);
    if (setjmp(p->jb)) {
        int line = line_of(p, p->err_pos);
        int col = 1; for (int i = p->err_pos - 1; i >= 0 && src[i] != '\n'; i--) col++;
        if (err) {
            int n = snprintf(NULL, 0, "%s:%d:%d: raku syntax error: %s", p->file, line, col, p->msg);
            *err = (char *) ct_alloc((size_t) n + 1); snprintf(*err, (size_t) n + 1, "%s:%d:%d: raku syntax error: %s", p->file, line, col, p->msg);
        }
        return 1;
    }
    r_comp_unit(p);
    if (err) *err = NULL;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_build_module(RkP *p, const char *src, int len, const char *path) {
    RkP *top = p->top ? p->top : p;
    for (int i = 0; i < top->nmodpaths; i++) if (!strcmp(top->modpaths[i], path)) return;
    RK_GROW(top->modpaths, top->nmodpaths, top->cmodpaths, const char *);
    top->modpaths[top->nmodpaths++] = path;
    RkP *m = (RkP *) ct_alloc(sizeof(RkP));
    memset(m, 0, sizeof *m);
    m->s = src; m->n = len; m->file = path; m->build = 1; m->B = rkb_new(src, len); rkb_set_st_lookup(m->B, state_name_of, m);
    m->myst_off = 1; m->lax = 1; m->load_depth = p->load_depth + 1; m->top = top;
    for (int i = 0; i < p->nlibs; i++) { RK_GROW(m->libs, m->nlibs, m->clibs, char *); m->libs[m->nlibs++] = p->libs[i]; }
    m->wsmark = (int *) ct_alloc(sizeof(int) * ((size_t) len + 2));
    memset(m->wsmark, 0, sizeof(int) * ((size_t) len + 2));
    m->endmark = (unsigned char *) ct_alloc((size_t) len + 2);
    memset(m->endmark, 0, (size_t) len + 2);
    if (setjmp(m->jb)) return;
    r_comp_unit(m);
    if (!m->tv) return;
    RK_GROW(top->mtrees, top->nmtrees, top->cmtrees, tree_t *);
    top->mtrees[top->nmtrees++] = m->tv;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_mod_names(const tree_t *t, char ***v, int *n, int *c) {
    if (!t) return;
    if ((t->t == TT_VAR || t->t == TT_FNC || t->t == TT_QLIT) && t->v.sval && t->v.sval[0]) { RK_GROW(*v, *n, *c, char *); (*v)[(*n)++] = t->v.sval; }
    for (int i = 0; i < t->n; i++) rk_mod_names(t->c[i], v, n, c);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *rk_mod_decl_name(const tree_t *d) {
    if (!d || d->n < 1 || !d->c[0] || d->c[0]->t != TT_VAR) return NULL;
    if (d->t != TT_SUB_DECL && d->t != TT_CLASS_DECL && d->t != TT_ROLE_DECL && d->t != TT_GRAMMAR_DECL) return NULL;
    return d->c[0]->v.sval;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_mod_subj(tree_t *s) {
    if (!s || s->t != TT_STMT) return s;
    for (int i = 0; i < s->n; i++) { tree_t *a = s->c[i]; if (a && a->t == TT_ATTR && a->v.sval && !strcmp(a->v.sval, ":subj")) return a->n > 0 ? a->c[0] : NULL; }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_mod_decls(tree_t *t, tree_t ***v, int *n, int *c) {
    for (int i = 0; t && i < t->n; i++) {
        tree_t *d = rk_mod_subj(t->c[i]);
        if (!d) continue;
        if (d->t == TT_MODULE_DECL) { rk_mod_decls(d, v, n, c); continue; }
        if (rk_mod_decl_name(d)) { RK_GROW(*v, *n, *c, tree_t *); (*v)[(*n)++] = t->c[i]; }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_mod_named(char **v, int n, const char *nm) {
    size_t l = strlen(nm);
    const char *dl = strchr(nm, '$');
    if (dl) l = (size_t) (dl - nm);
    for (int i = 0; i < n; i++) if (!strncmp(v[i], nm, l) && (v[i][l] == 0 || v[i][l] == '$')) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_mod_assigns(tree_t *t, tree_t ***v, int *n, int *c) {
    for (int i = 0; t && i < t->n; i++) {
        tree_t *d = rk_mod_subj(t->c[i]);
        if (!d) continue;
        if (d->t == TT_MODULE_DECL) { rk_mod_assigns(d, v, n, c); continue; }
        if (d->t == TT_ASSIGN && d->n >= 2 && d->c[0] && d->c[0]->t == TT_VAR && d->c[0]->v.sval && (d->c[0]->slen & 4)) { RK_GROW(*v, *n, *c, tree_t *); (*v)[(*n)++] = t->c[i]; }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_mod_begins(tree_t *t, tree_t ***v, int *n, int *c) {
    for (int i = 0; t && i < t->n; i++) {
        tree_t *d = rk_mod_subj(t->c[i]);
        if (!d) continue;
        if (d->t == TT_MODULE_DECL) { rk_mod_begins(d, v, n, c); continue; }
        if (d->t == TT_FNC && d->v.sval && (!strcmp(d->v.sval, "__rk_phaser_BEGIN") || !strcmp(d->v.sval, "__rk_phaser_END"))) { RK_GROW(*v, *n, *c, tree_t *); (*v)[(*n)++] = t->c[i]; }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_splice_modules(RkP *p, tree_t *t) {
    if (!t || !p->nmtrees) return t;
    tree_t **cand = NULL; int nc = 0, cc = 0;
    for (int k = 0; k < p->nmtrees; k++) rk_mod_decls(p->mtrees[k], &cand, &nc, &cc);
    tree_t **bgn = NULL; int nb = 0, cbg = 0;
    for (int k = 0; k < p->nmtrees; k++) rk_mod_begins(p->mtrees[k], &bgn, &nb, &cbg);
    if (!nc && !nb) return t;
    tree_t **acand = NULL; int na = 0, ca = 0;
    for (int k = 0; k < p->nmtrees; k++) rk_mod_assigns(p->mtrees[k], &acand, &na, &ca);
    char *ataken = (char *) ct_alloc((size_t) na + 1);
    memset(ataken, 0, (size_t) na + 1);
    char **names = NULL; int nn = 0, cn = 0;
    rk_mod_names(t, &names, &nn, &cn);
    for (int i = 0; i < nb; i++) rk_mod_names(bgn[i], &names, &nn, &cn);
    char *taken = (char *) ct_alloc((size_t) nc + 1);
    memset(taken, 0, (size_t) nc + 1);
    for (int grew = 1; grew; ) {
        grew = 0;
        for (int i = 0; i < nc; i++) {
            if (taken[i]) continue;
            const tree_t *d = rk_mod_subj(cand[i]);
            if (!rk_mod_named(names, nn, rk_mod_decl_name(d))) continue;
            taken[i] = 1; grew = 1;
            rk_mod_names(d, &names, &nn, &cn);
        }
        for (int i = 0; i < na; i++) {
            if (ataken[i]) continue;
            const tree_t *a = rk_mod_subj(acand[i]);
            if (!rk_mod_named(names, nn, a->c[0]->v.sval)) continue;
            ataken[i] = 1; grew = 1;
            rk_mod_names(a, &names, &nn, &cn);
        }
    }
    tree_t *nt = ast_node_new(t->t);
    nt->v = t->v; nt->line = t->line; nt->slen = t->slen;
    extern tree_t *ast_stmt_new(tree_e kind);
    extern tree_t *ast_attr_int(const char *tag, int ival);
    extern tree_t *ast_attr_expr(const char *tag, tree_t *expr);
    for (int i = 0; i < na; i++) {
        if (!ataken[i]) continue;
        tree_t *st = acand[i];
        if (st->t != TT_STMT) { st = ast_stmt_new(TT_STMT); ast_push(st, ast_attr_int(":line", 0)); ast_push(st, ast_attr_int(":stno", 0)); ast_push(st, ast_attr_expr(":subj", acand[i])); }
        ast_push(nt, st);
    }
    for (int i = 0; i < nc; i++) {
        if (!taken[i]) continue;
        tree_t *st = cand[i];
        if (st->t != TT_STMT) { st = ast_stmt_new(TT_STMT); ast_push(st, ast_attr_int(":line", 0)); ast_push(st, ast_attr_int(":stno", 0)); ast_push(st, ast_attr_expr(":subj", cand[i])); }
        ast_push(nt, st);
    }
    for (int i = 0; i < nb; i++) if (bgn[i]->t != TT_STMT) { tree_t *st = ast_stmt_new(TT_STMT); ast_push(st, ast_attr_int(":line", 0)); ast_push(st, ast_attr_int(":stno", 0)); ast_push(st, ast_attr_expr(":subj", bgn[i])); bgn[i] = st; }
    int lastuse = -1;
    for (int i = 0; i < t->n; i++) { tree_t *u = rk_mod_subj(t->c[i]); if (u && u->t == TT_USE_DECL) lastuse = i; }
    if (lastuse < 0) for (int i = 0; i < nb; i++) ast_push(nt, bgn[i]);
    for (int i = 0; i < t->n; i++) { ast_push(nt, t->c[i]); if (i == lastuse) for (int j = 0; j < nb; j++) ast_push(nt, bgn[j]); }
    return nt;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rk_parse_tree(const char *src, int len, const char *path, char **errmsg) {
    RkP *p = (RkP *) ct_alloc(sizeof(RkP));
    memset(p, 0, sizeof *p);
    p->s = src; p->n = len; p->file = path ? path : "<stdin>"; p->build = 1; p->B = rkb_new(src, len); rkb_set_st_lookup(p->B, state_name_of, p);
    p->myst_off = 1; p->lax = 1;
    p->wsmark = (int *) ct_alloc(sizeof(int) * ((size_t) len + 2));
    memset(p->wsmark, 0, sizeof(int) * ((size_t) len + 2));
    p->endmark = (unsigned char *) ct_alloc((size_t) len + 2);
    memset(p->endmark, 0, (size_t) len + 2);
    if (setjmp(p->jb)) {
        int line = line_of(p, p->err_pos);
        int col = 1; for (int i = p->err_pos - 1; i >= 0 && src[i] != '\n'; i--) col++;
        if (errmsg) {
            int n = snprintf(NULL, 0, "%s:%d:%d: raku syntax error: %s", p->file, line, col, p->msg);
            *errmsg = (char *) ct_alloc((size_t) n + 1); snprintf(*errmsg, (size_t) n + 1, "%s:%d:%d: raku syntax error: %s", p->file, line, col, p->msg);
        }
        return NULL;
    }
    r_comp_unit(p);
    if (errmsg) *errmsg = NULL;
    return rk_splice_modules(p, p->tv);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_syntax_file(const char *path) {
    FILE *f = fopen(path, "rb");
    if (!f) { fprintf(stderr, "scrip: cannot open '%s'\n", path); return 2; }
    fseek(f, 0, SEEK_END); long n = ftell(f); rewind(f);
    char *src = (char *) ct_alloc((size_t) n + 1);
    if (fread(src, 1, (size_t) n, f) != (size_t) n) { fclose(f); fprintf(stderr, "scrip: short read on '%s'\n", path); return 2; }
    src[n] = 0; fclose(f);
    char *err = NULL;
    int rc = rk_syntax_check(src, (int) n, path, &err);
    if (rc) fprintf(stderr, "%s\n", err);
    return rc;
}
