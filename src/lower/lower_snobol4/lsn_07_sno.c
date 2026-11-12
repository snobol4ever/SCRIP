#include "lower/lower_snobol4/lsn_head.h"
#include "lower/lower_snobol4/lsn_02_sno.h"
#include "lower/lower_snobol4/lsn_03_sno.h"
#include "lower/lower_snobol4/lsn_05_sno.h"
#include "lower/lower_snobol4/lsn_07_sno.h"
extern int snobol4_list_event_count(void);
extern int snobol4_list_event_nend(int);
extern int snobol4_list_event_kind(int);
extern int snobol4_list_event_arg(int);
extern const char *snobol4_list_event_text(int);
extern const char * stmt_src_image(int);
extern const char * stmt_src_file_image(const char *, int);
extern tree_t * ast_attr_int(const char * tag, int ival);
extern tree_t * ast_attr_leaf(const char * tag, const char * val);
extern tree_t * ast_attr_expr(const char * tag, tree_t * expr);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const tree_t * sgoto_expr(const tree_t * s, tree_e kind) {
    for (int i = 0; i < s->n; i++) {
        const tree_t * a = s->c[i];
        if (!a || a->t != kind || a->n == 0 || !a->c[0]) continue;
        const tree_t * g0 = a->c[0];
        if (g0->t == TT_QLIT) return NULL;
        if (g0->t == TT_INDIRECT && g0->n > 0 && g0->c[0] && g0->c[0]->t == TT_VAR) return NULL;
        if (g0->t == TT_GOTO_DIRECT) return NULL;
        return g0;
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const tree_t * sgoto_direct(const tree_t * s, tree_e kind) {
    for (int i = 0; i < s->n; i++) {
        const tree_t * a = s->c[i];
        if (!a || a->t != kind || a->n == 0 || !a->c[0]) continue;
        if (a->c[0]->t == TT_GOTO_DIRECT) return (a->c[0]->n > 0) ? a->c[0]->c[0] : NULL;
        return NULL;
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * sno_expr_const_prefix(const tree_t * t) {
    if (!t) return NULL;
    if (t->t == TT_QLIT) return t->v.sval ? t->v.sval : "";
    if ((t->t == TT_CAT || t->t == TT_SEQ) && t->n >= 1) return sno_expr_const_prefix(t->c[0]);
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sno_goto_specials_impossible(const tree_t * expr) {
    const char * pfx = sno_expr_const_prefix(expr);
    if (!pfx || !*pfx) return 0;
    static const char * const sn3[3] = { "NRETURN", "FRETURN", "RETURN" };
    for (int k = 0; k < 3; k++) { size_t lp = strlen(pfx), ls = strlen(sn3[k]), m = lp < ls ? lp : ls; if (!strncmp(pfx, sn3[k], m)) return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * sno_cset_fold(const tree_t * a) {
    if (!a) return NULL;
    if (a->t == TT_QLIT) return a->v.sval ? a->v.sval : "";
    if (a->t == TT_ILIT) { char nb[24]; snprintf(nb, sizeof nb, "%lld", (long long) a->v.ival); char * ob = (char *) ct_alloc(strlen(nb) + 1); if (!ob) return NULL; strcpy(ob, nb); return ob; }
    if (a->t == TT_KEYWORD && a->v.sval) {
        static const struct { const char * n; const char * v; } kc[] = { { "lcase", "abcdefghijklmnopqrstuvwxyz" }, { "ucase", "ABCDEFGHIJKLMNOPQRSTUVWXYZ" } };
        char lk[strlen(a->v.sval) + 1];
        size_t li = 0;
        for (; a->v.sval[li]; li++) lk[li] = (a->v.sval[li] >= 'A' && a->v.sval[li] <= 'Z') ? (char)(a->v.sval[li] - 'A' + 'a') : a->v.sval[li];
        lk[li] = 0;
        for (size_t k = 0; k < sizeof kc / sizeof *kc; k++) if (!strcmp(lk, kc[k].n)) return kc[k].v;
        {
            char cb[fmt_len("&%s", a->v.sval[0] == '&' ? a->v.sval + 1 : a->v.sval)];
            snprintf(cb, sizeof cb, "&%s", a->v.sval[0] == '&' ? a->v.sval + 1 : a->v.sval);
            const tree_t * cv = sno_const_val(cb);
            if (cv) return sno_cset_fold(cv);
            const tree_t * cp = sno_const_pat(cb);
            if (cp && cp != a) return sno_cset_fold(cp);
        }
    }
    if (a->t == TT_FNC && a->v.sval && (!strcmp(a->v.sval, "CHAR") || !strcmp(a->v.sval, "char")) && a->n == 1 && a->c[0] && a->c[0]->t == TT_ILIT && a->c[0]->v.ival >= 1 && a->c[0]->v.ival <= 255) {
        char * cb = (char *) ct_alloc(2);
        if (!cb) return NULL;
        cb[0] = (char)(unsigned char) a->c[0]->v.ival;
        cb[1] = 0;
        return cb;
    }
    if (a->t == TT_FNC && a->v.sval && !strcmp(a->v.sval, "SUBSTR") && a->n == 3 && a->c[0] && a->c[0]->t == TT_KEYWORD && a->c[0]->v.sval &&
        !strcmp(a->c[0]->v.sval + (a->c[0]->v.sval[0] == '&'), "ALPHABET") && a->c[1] && a->c[1]->t == TT_ILIT && a->c[2] && a->c[2]->t == TT_ILIT) {
        long i = (long) a->c[1]->v.ival, n = (long) a->c[2]->v.ival;
        if (i < 2 || n < 1 || i + n - 1 > 256) return NULL;
        char * cb = (char *) ct_alloc((size_t) n + 1);
        if (!cb) return NULL;
        for (long k = 0; k < n; k++) cb[k] = (char)(unsigned char)(i - 1 + k);
        cb[n] = 0;
        return cb;
    }
    if (a->t == TT_SEQ || a->t == TT_CAT) {
        const char * l = sno_cset_fold((a->n > 0) ? a->c[0] : NULL);
        if (!l) return NULL;
        const char * r = sno_cset_fold((a->n > 1) ? a->c[1] : NULL);
        if (!r) return NULL;
        size_t ln = strlen(l), rn = strlen(r);
        char * buf = (char *) ct_alloc(ln + rn + 1);
        if (!buf) return NULL;
        memcpy(buf, l, ln);
        memcpy(buf + ln, r, rn);
        buf[ln + rn] = 0;
        return buf;
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long sno_prearg_codes(int tt) {
    switch (tt) {
        case TT_ANY:
        return 59L | (59L << 16);
        case TT_BREAK:
        return 69L | (69L << 16);
        case TT_BREAKX:
        return 70L | (70L << 16);
        case TT_NOTANY:
        return 151L | (151L << 16);
        case TT_SPAN:
        return 188L | (188L << 16);
        case TT_LEN:
        return 120L | (121L << 16);
        case TT_POS:
        return 162L | (163L << 16);
        case TT_RTAB:
        return 181L | (182L << 16);
        case TT_TAB:
        return 183L | (184L << 16);
        case TT_RPOS:
        return 185L | (186L << 16);
        default:
        return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * sno_pat_defer_arg_name(const tree_t * inner, int lead_star) {
    if (inner && inner->t == TT_VAR && inner->v.sval && inner->v.sval[0]) {
        char b[fmt_len("%s%s", lead_star ? "*" : "", inner->v.sval)];
        snprintf(b, sizeof b, "%s%s", lead_star ? "*" : "", inner->v.sval);
        return lp_strdup(b);
    }
    const char * xn = sno_expr_collect(inner);
    char b[fmt_len("%s*%s", lead_star ? "*" : "", xn)];
    snprintf(b, sizeof b, "%s*%s", lead_star ? "*" : "", xn);
    return lp_strdup(b);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void sno_pre_req(scx_t * cx, const tree_t * t, IR_t * prim) {
    const tree_t * arg = (t->n > 0) ? t->c[0] : NULL;
    if (arg && arg->t == TT_DEFER && arg->n > 0 && arg->c[0]) arg = arg->c[0];
    if (!arg || arg->t == TT_DEFER) sno_fatal("pattern primitive argument outside the operand-edge subset (missing or deferred *expr argument)", NULL);
    if (cx->npre >= 64) sno_fatal("too many runtime pattern-primitive arguments in one statement (operand-edge pre-chain limit 64)", NULL);
    cx->pre[cx->npre].arg = arg;
    cx->pre[cx->npre].prim = prim;
    cx->pre[cx->npre].str = (t->t == TT_ANY || t->t == TT_NOTANY || t->t == TT_SPAN || t->t == TT_BREAK || t->t == TT_BREAKX);
    cx->pre[cx->npre].codes = sno_prearg_codes(t->t);
    cx->pre[cx->npre].snapg = NULL;
    cx->pre[cx->npre].ppos = 0;
    cx->npre++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char sno_elem_snapg[] = "$elem";
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sno_pat_inline_ok(const tree_t * t) {
    if (!t) return 1;
    switch (t->t) {
        case TT_QLIT:
        return 1;
        case TT_REM:
        return 1;
        case TT_VAR:
        return t->v.sval != NULL;
        case TT_KEYWORD:
        return t->v.sval != NULL;
        case TT_DEFER:
        return t->n > 0 && t->c[0] != NULL;
        case TT_ANY:
        case TT_NOTANY:
        case TT_SPAN:
        case TT_BREAK:
        case TT_BREAKX:
        return (t->n > 0) && sno_cset_fold(t->c[0]) != NULL;
        case TT_LEN:
        case TT_TAB:
        case TT_RTAB:
        case TT_POS:
        case TT_RPOS:
        return (t->n > 0) && t->c[0] && t->c[0]->t == TT_ILIT;
        case TT_SEQ:
        case TT_CAT:
        case TT_ALT:
        { for (int i = 0; i < t->n; i++) if (!sno_pat_inline_ok(t->c[i])) return 0; return 1; }
        case TT_ARBNO:
        {
            if (g_lower.sno.sno_pat_inline_ok_ia < 0) { const char * e = getenv("SCRIP_PAT_INLINE_ARBNO"); g_lower.sno.sno_pat_inline_ok_ia = (!e || *e != '0') ? 1 : 0; }
            return g_lower.sno.sno_pat_inline_ok_ia && t->n > 0 && sno_pat_inline_ok(t->c[0]);
        }
        default:
        return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_rsv_name(const char * nm) { return nm && (!strcmp(nm, "epsilon") || !strcmp(nm, "NULL") || !strcmp(nm, "null")); }
static int sno_rsv_nullval(const tree_t * t) {
    static const char * const pr[] = { "IDENT", "DIFFER", "EQ", "NE", "LT", "LE", "GT", "GE", "LGT", "LLT", "LGE", "LLE", "LEQ", "LNE", NULL };
    if (!t || t->t == TT_NUL || t->t == TT_INTERROGATE || (t->t == TT_QLIT && (!t->v.sval || !t->v.sval[0])) || (t->t == TT_VAR && sno_rsv_name(t->v.sval))) return 1;
    if ((t->t == TT_SEQ || t->t == TT_CAT) && t->n == 2) return sno_rsv_nullval(t->c[0]) && sno_rsv_nullval(t->c[1]);
    if (t->t == TT_FNC && t->v.sval) for (int i = 0; pr[i]; i++) if (!strcmp(t->v.sval, pr[i])) return 1;
    return 0;
}
static int sno_rsv_err(const tree_t * s, const char * nm, const char * what, int quiet) {
    if (quiet) return 1;
    extern const char * stmt_src_get_file(void);
    const char * f = s ? sfind_str(s, ":file") : NULL;
    if (!f || !*f) f = stmt_src_get_file();
    long l = s ? (long) lp_s_int(s, ":lline") : 0;
    if (!l && s) l = (long) lp_s_int(s, ":line");
    fprintf(stderr, "%s:%ld: error: %s is a reserved constant, the zero-length string -- %s\n", (f && *f) ? f : "scrip", l, nm, what);
    return 1;
}
static int sno_rsv_proto(const char * p, const tree_t * s, int quiet) {
    int bad = 0;
    const char * q = p ? strchr(p, '(') : NULL;
    if (!q) return 0;
    for (q++; *q; ) {
        while (*q == ' ' || *q == ',' || *q == ')') q++;
        const char * b = q;
        while (*q && *q != ',' && *q != ')' && *q != ' ') q++;
        size_t n = (size_t)(q - b);
        if (q == b) continue;
        char nm[n + 1];
        memcpy(nm, b, n);
        nm[n] = 0;
        if (sno_rsv_name(nm)) bad += sno_rsv_err(s, nm, "it cannot be a parameter or a local", quiet);
    }
    return bad;
}
static int sno_rsv_stmt(tree_t * s, int quiet) {
    tree_t * sa = NULL;
    tree_t * subj = NULL;
    tree_t * repl = NULL;
    int eq = 0, pat = 0, bad = 0;
    for (int i = 0; i < s->n; i++) {
        tree_t * a = s->c[i];
        if (!a || a->t != TT_ATTR || !a->v.sval) continue;
        if (!strcmp(a->v.sval, ":subj")) { sa = a; subj = a->n > 0 ? a->c[0] : NULL; } else if (!strcmp(a->v.sval, ":repl")) repl = a->n > 0 ? a->c[0] : NULL;
        else if (!strcmp(a->v.sval, ":eq")) eq = 1;
        else if (!strcmp(a->v.sval, ":pat")) pat = 1;
    }
    if (eq && !pat && subj && subj->t == TT_VAR && sno_rsv_name(subj->v.sval)) {
        if (!sno_rsv_nullval(repl)) return sno_rsv_err(s, subj->v.sval, "it cannot be assigned", quiet);
        if (repl && repl->t != TT_NUL) sa->c[0] = repl;
        else { tree_t * q = ast_node_new(TT_QLIT); q->v.sval = (char *) ""; sa->c[0] = q; }
        int k = 0;
        for (int i = 0; i < s->n; i++) { tree_t * a = s->c[i]; if (a && a->t == TT_ATTR && a->v.sval && (!strcmp(a->v.sval, ":eq") || !strcmp(a->v.sval, ":repl"))) continue; s->c[k++] = a; }
        s->n = k;
    }
        else if (eq && subj &&
        ((pat && subj->t == TT_VAR && sno_rsv_name(subj->v.sval)) || (subj->t == TT_SCAN && subj->n > 0 && subj->c[0] && subj->c[0]->t == TT_VAR && sno_rsv_name(subj->c[0]->v.sval))))
        return sno_rsv_err(s, subj->t == TT_VAR ? subj->v.sval : subj->c[0]->v.sval, "it cannot be the subject of a replacement", quiet);
    for (int i = 0; i < s->n; i++) {
        tree_t * a = s->c[i];
        if (!a) continue;
        if (a->t == TT_ATTR && a->v.sval && (!strcmp(a->v.sval, ":lbl") || !strncmp(a->v.sval, ":go", 3))) continue;
        bad += sno_rsv_walk(a, s, quiet);
    }
    return bad;
}
int sno_rsv_walk(tree_t * t, const tree_t * s, int quiet) {
    if (!t) return 0;
    int bad = 0, from = 0;
    switch (t->t) {
        case TT_STMT:
        return sno_rsv_stmt(t, quiet);
        case TT_GOTO_U:
        case TT_GOTO_S:
        case TT_GOTO_F:
        return 0;
        case TT_VAR:
        if (sno_rsv_name(t->v.sval)) { t->t = TT_QLIT; t->v.sval = (char *) ""; t->n = 0; }
        return 0;
        case TT_NAME:
        if (t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR) return 0;
        break;
        case TT_CAPT_COND_ASGN:
        case TT_CAPT_IMMED_ASGN:
        if (t->n > 1 && t->c[1] && t->c[1]->t == TT_VAR && sno_rsv_name(t->c[1]->v.sval)) return sno_rsv_err(s, t->c[1]->v.sval, "it cannot be a capture target", quiet) +
            sno_rsv_walk(t->c[0], s, quiet);
        break;
        case TT_CAPT_CURSOR:
        if (t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR && sno_rsv_name(t->c[0]->v.sval)) return sno_rsv_err(s, t->c[0]->v.sval, "it cannot be a capture target", quiet);
        break;
        case TT_ASSIGN:
        if (t->n == 2 && t->c[0] && t->c[0]->t == TT_VAR && sno_rsv_name(t->c[0]->v.sval)) {
            if (!sno_rsv_nullval(t->c[1])) return sno_rsv_err(s, t->c[0]->v.sval, "it cannot be assigned", quiet);
            if (!t->c[1]) { t->t = TT_QLIT; t->v.sval = (char *) ""; t->n = 0; return 0; }
            *t = *t->c[1];
            return sno_rsv_walk(t, s, quiet);
        }
        break;
        case TT_DEFINE:
        if (t->n > 1 && t->c[1] && t->c[1]->t == TT_QLIT) bad += sno_rsv_proto(t->c[1]->v.sval, s, quiet);
        break;
        case TT_FNC:
        if (!t->v.sval && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR) from = 1;
        {
            const char * fn = t->v.sval ? t->v.sval : (from ? t->c[0]->v.sval : NULL);
            if (fn && !strcmp(fn, "DEFINE") && t->n > from && t->c[from] && t->c[from]->t == TT_QLIT) bad += sno_rsv_proto(t->c[from]->v.sval, s, quiet);
        }
        break;
        default:
        break;
    }
    for (int i = from; i < t->n; i++) bad += sno_rsv_walk(t->c[i], s, quiet);
    return bad;
}
static void sno_list_sb_append(sno_list_sb_t * sb, const char * s, size_t n) {
    if (sb->len + n + 1 > sb->cap) {
        size_t newcap = sb->cap ? sb->cap * 2 : 1024;
        while (newcap < sb->len + n + 1) newcap *= 2;
        char * g = ct_grow(sb->buf, newcap);
        if (!g) return;
        sb->buf = g;
        sb->cap = newcap;
    }
    memcpy(sb->buf + sb->len, s, n);
    sb->len += n;
    sb->buf[sb->len] = '\0';
}
static void sno_list_sb_puts(sno_list_sb_t * sb, const char * s) { sno_list_sb_append(sb, s, strlen(s)); }
static void sno_list_line(sno_list_t * L, const char * s, size_t n) {
    while (n > SNO_LIST_PRLEN) { sno_list_sb_append(&L->sb, s, SNO_LIST_PRLEN); sno_list_sb_append(&L->sb, "\n", 1); s += SNO_LIST_PRLEN; n -= SNO_LIST_PRLEN; }
    sno_list_sb_append(&L->sb, s, n);
    sno_list_sb_append(&L->sb, "\n", 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_list_title(sno_list_t * L) {
    const char * t = L->ttl ? L->ttl : "";
    size_t tl = strlen(t);
    while (tl > SNO_LIST_PRLEN) { sno_list_sb_append(&L->sb, t, SNO_LIST_PRLEN); sno_list_sb_append(&L->sb, "\n", 1); t += SNO_LIST_PRLEN; tl -= SNO_LIST_PRLEN; }
    char msg[32];
    size_t ml = (size_t) snprintf(msg, sizeof msg, "page %d", ++L->lstpg);
    size_t width = SNO_LIST_PGOFS + ml;
    char line[width + 1];
    memset(line, ' ', width);
    memcpy(line, t, tl < width ? tl : width);
    memcpy(line + SNO_LIST_PGOFS, msg, ml);
    sno_list_sb_append(&L->sb, line, width);
    sno_list_sb_append(&L->sb, "\n", 1);
    L->lstlc += 2;
    if (L->stl) { sno_list_line(L, L->stl, strlen(L->stl)); L->lstlc++; }
    sno_list_sb_append(&L->sb, "\n", 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_list_eject(sno_list_t * L) {
    if (L->lstlc != 0) {
        sno_list_sb_puts(&L->sb, "\n\n\n");
        L->lstlc = 3;
        if (!L->headp) {
            time_t now = time(NULL);
            const char * ct = ctime(&now);
            size_t cl = ct ? strlen(ct) : 0;
            if (cl && ct[cl - 1] == '\n') cl--;
            L->headp = 1;
            sno_list_sb_puts(&L->sb, "macro spitbol version " SNO_LIST_VERSION "\nx86-64  ");
            sno_list_sb_append(&L->sb, ct ? ct : "", cl);
            sno_list_sb_puts(&L->sb, "\n\n\n\n");
            L->lstlc += 4;
        }
    }
    sno_list_title(L);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_list_one(sno_list_t * L, const char * field, const char * s, size_t n) {
    if (L->cnttl) { L->cnttl = 0; sno_list_eject(L); } else if (L->lstlc >= SNO_LIST_NP) sno_list_eject(L);
    char buf[SNO_LIST_STNPD + n + 1];
    memcpy(buf, field, SNO_LIST_STNPD);
    memcpy(buf + SNO_LIST_STNPD, s, n);
    sno_list_line(L, buf, SNO_LIST_STNPD + n);
    L->lstlc++;
    if (L->cswdb) { sno_list_sb_append(&L->sb, "\n", 1); L->lstlc++; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_list_blank_field(char * field, int depth) { if (depth > 0) snprintf(field, 24, "%5s%d  ", "", depth); else snprintf(field, 24, "        "); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_list_stmt(sno_list_t * L, const tree_t * snode) {
    int depth = lp_s_int(snode, ":depth");
    const char * fname = sfind_str(snode, ":file");
    int line = lp_s_int(snode, depth > 0 ? ":lline" : ":line");
    const char * raw = depth > 0 ? stmt_src_file_image(fname, line) : stmt_src_image(line);
    const char * txt = (snode->t == TT_STMT) ? sfind_str(snode, ":src") : NULL;
    const char * ent = (snode->t == TT_END) ? sfind_str(snode, ":entry") : NULL;
    char endbuf[(ent && ent[0]) ? fmt_len("END %s", ent) : 4];
    size_t n;
    char field[24];
    if (snode->t == TT_END) {
        if (raw && !strncmp(raw, "END", 3)) txt = raw;
        else if (ent && ent[0]) { snprintf(endbuf, sizeof endbuf, "END %s", ent); txt = endbuf; } else { snprintf(endbuf, sizeof endbuf, "END"); txt = endbuf; }
    }
    if (!txt) txt = "";
    n = strcspn(txt, "\n");
    if (depth > 0) snprintf(field, sizeof field, "%-5d%d  ", lp_s_int(snode, ":stno"), depth);
    else snprintf(field, sizeof field, "%-8d", lp_s_int(snode, ":stno"));
    sno_list_one(L, field, txt, n);
    if (snode->t != TT_STMT || !n || !raw || strlen(raw) != n || memcmp(raw, txt, n)) return;
    sno_list_blank_field(field, depth);
    for (int k = line + 1; (raw = depth > 0 ? stmt_src_file_image(fname, k) : stmt_src_image(k)) && (raw[0] == '+' || raw[0] == '.'); k++) sno_list_one(L, field, raw, strlen(raw));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_list_event(sno_list_t * L, int kind, int arg, const char * text) {
    switch (kind) {
        case 'L':
        L->cswls = 1;
        break;
        case 'N':
        L->cswls = 0;
        break;
        case 'P':
        L->cswpr = 1;
        break;
        case 'Q':
        L->cswpr = 0;
        break;
        case 'D':
        L->cswdb = 1;
        break;
        case 'I':
        L->cswdb = 0;
        break;
        case 'E':
        if (L->cswls) sno_list_eject(L);
        break;
        case 'S':
        if (L->cswls) { int wc = arg ? arg : 1; L->lstlc += wc; if (L->lstlc < SNO_LIST_NP) { for (int k = 0; k < wc; k++) sno_list_sb_append(&L->sb, "\n", 1); } else sno_list_eject(L); }
        break;
        case 'T':
        L->ttl = text;
        L->stl = "";
        L->cnttl = 1;
        break;
        case 'U':
        L->stl = text;
        L->cnttl = 1;
        break;
        case 'C':
        if (L->cswls) { char field[24]; sno_list_blank_field(field, arg); sno_list_one(L, field, text, strlen(text)); }
        break;
        case 'X':
        if (L->cswls && L->cswpr) { char field[24]; sno_list_blank_field(field, arg); sno_list_one(L, field, text, strlen(text)); }
        break;
        default:
        break;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
char * sno_list_build_listing(const tree_t * prog) {
    int nev = snobol4_list_event_count();
    if (nev == 0 || !prog) return NULL;
    sno_list_t L = {0};
    L.lstlc = SNO_LIST_NP;
    L.cswpr = 1;
    int ei = 0, j = 0, prev_line = -1, prev_text = 0;
    const char * prev_file = NULL;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * snode = prog->c[i];
        if (!snode || (snode->t != TT_STMT && snode->t != TT_END)) continue;
        while (ei < nev && snobol4_list_event_nend(ei) <= j) { sno_list_event(&L, snobol4_list_event_kind(ei), snobol4_list_event_arg(ei), snobol4_list_event_text(ei)); ei++; }
        const char * src = (snode->t == TT_STMT) ? sfind_str(snode, ":src") : NULL;
        int line = lp_s_int(snode, ":line");
        const char * file = sfind_str(snode, ":file");
        int same_image = (src && src[0] && prev_text && line == prev_line && ((!file && !prev_file) || (file && prev_file && !strcmp(file, prev_file))));
        if (L.cswls && !same_image) sno_list_stmt(&L, snode);
        prev_line = line;
        prev_file = file;
        prev_text = (src && src[0]) ? 1 : 0;
        j++;
        if (snode->t == TT_END) break;
    }
    if (L.sb.len > 0 && L.sb.buf[L.sb.len - 1] == '\n') { L.sb.len--; L.sb.buf[L.sb.len] = '\0'; }
    return L.sb.buf;
}
tree_t * sno_noexec_program(const tree_t * prog) {
    tree_t * np = ast_node_new(TT_PROGRAM);
    tree_t * st = ast_node_new(TT_STMT);
    tree_t * kw = ast_node_new(TT_KEYWORD);
    kw->v.sval = ct_strdup("CODE");
    tree_t * lit = ast_node_new(TT_ILIT);
    lit->v.ival = 231;
    ast_push(st, ast_attr_int(":line", 0));
    ast_push(st, ast_attr_int(":stno", 1));
    ast_push(st, ast_attr_leaf(":src", ""));
    ast_push(st, ast_attr_expr(":subj", kw));
    ast_push(st, ast_attr_leaf(":eq", ""));
    ast_push(st, ast_attr_expr(":repl", lit));
    ast_push(np, st);
    for (int i = 0; i < prog->n; i++) if (prog->c[i] && prog->c[i]->t == TT_END) { ast_push(np, prog->c[i]); break; }
    return np;
}
