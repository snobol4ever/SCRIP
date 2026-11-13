#include "runtime/lib/bn_head.h"
#include "runtime/rt/rt_arena.h"
#include "runtime/rt/prolog_atom.h"
#include "driver/driver_globals.h"
#include "runtime/core/utf8.h"
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "runtime/lib/bn_05_pl.h"
#include "runtime/lib/bn_11_pl.h"
#include "runtime/lib/bn_20_pl.h"
#include "runtime/lib/bn_27_pl.h"
#include "runtime/lib/bn_29_pl.h"
#include "runtime/lib/bn_30_pl.h"
#include "runtime/lib/bn_31_pl.h"
#include "runtime/lib/bn_33_pl.h"
#include "runtime/lib/bn_34_pl.h"
extern void *rt_ws_alloc_descr(size_t);
extern void *rt_pl_ball_permission3(const char *, const char *, DESCR_t);
extern FILE *fh_get(int);
DESCR_t dop_pl_set_input(DESCR_t *args, int nargs) {
    extern void fh_set_input(int);
    int idx;
    if (nargs != 1 || (idx = pl_stream_idx(args[0], 0)) < 0) return FAILDESCR;
    fh_set_input(idx);
    return pl_ok();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pl_mk_cmp1(const char *f, DESCR_t a0) {
    extern int prolog_atom_intern(const char *);
    DESCR_t *kids = (DESCR_t *)rt_ws_alloc_descr(1);
    kids[0] = a0;
    DESCR_t c = {0};
    c.v = (DTYPE_t)DT_PLREF;
    c.slen = (uint32_t)prolog_functor_intern(prolog_atom_intern(f), 1);
    c.p = (void *)kids;
    return c;
}
int pl_sp_stream_live(int i) { extern FILE *fh_get(int); return fh_get(i) != (FILE *)0; }
static int pl_sp_is_input(int i) { return (i == 0) || (i >= 3 && g_fh[i].mode == 'r'); }
int pl_sp_named(DESCR_t d) {
    const char *nm = pl_atom_str(d);
    if (!nm) return -1;
    if (!strcmp(nm, "user_input")) return 0;
    if (!strcmp(nm, "user_output")) return 1;
    if (!strcmp(nm, "user_error")) return 2;
    return pl_alias_idx(nm);
}
static const char *pl_sp_eos_name(int i) {
    extern FILE *fh_get(int);
    FILE *fp = fh_get(i);
    int c;
    if (!fp) return (const char *)0;
    if (feof(fp)) return "past";
    c = fgetc(fp);
    if (c == EOF) { clearerr(fp); return "at"; }
    ungetc(c, fp);
    return "not";
}
static void pl_fh_push(int **v, int *n, int x) { int *q = (int *)rt_gcheap_grow_block((void *)*v, (uint16_t)HB_WSB, (uint64_t)(*n + 1) * sizeof(int)); if (!q) return; q[*n] = x; *v = q; (*n)++; }
static void pl_fh_addal(int i, const char *nm) {
    extern int prolog_atom_intern(const char *);
    if (i >= 3 && !g_fh[i].alias) fh_set_alias(i, nm);
    else pl_fh_push(&g_fh[i].xal, &g_fh[i].nxal, prolog_atom_intern(nm));
}
static int pl_sp_nprops(int i) { return PL_SP_NPROP + g_fh[i].nxal + (i >= 3 ? g_fh[i].nmir : 0); }
static int pl_sp_xprop(int i, int j, DESCR_t *out) {
    extern const char *prolog_atom_name(int);
    if (j < g_fh[i].nxal) { const char *a = prolog_atom_name(g_fh[i].xal[j]); *out = pl_mk_cmp1("alias", pl_mk_atom_dup(a, strlen(a))); return 1; }
    j -= g_fh[i].nxal;
    if (i < 3) return 0;
    if (j < g_fh[i].nmir) { *out = pl_mk_cmp1("mirror", pl_mk_stream(g_fh[i].mir[j])); return 1; }
    return 0;
}
static int pl_sp_prop(int i, int pidx, DESCR_t *out) {
    if (pidx >= PL_SP_NPROP) return pl_sp_xprop(i, pidx - PL_SP_NPROP, out);
    if (pidx == 14) {
        FILE *tf = g_fh[i].fp;
        int fd = tf && !g_fh[i].sink ? fileno(tf) : -1, t = g_fh[i].tty ? g_fh[i].tty == 't' : (fd >= 0 && isatty(fd));
        if (pl_gnu_dialect()) return 0;
        *out = pl_mk_cmp1("tty", pl_mk_atom(t ? "true" : "false"));
        return 1;
    }
    if (pidx == 13) {
        char b = g_fh[i].buf ? g_fh[i].buf : i == 2 ? 'n' : i < 3 ? 'l' : 'b';
        if (!pl_gnu_dialect()) return 0;
        *out = pl_mk_cmp1("buffering", pl_mk_atom(b == 'n' ? "none" : b == 'l' ? "line" : "block"));
        return 1;
    }
    switch (pidx) {
        case 0:
        if (i < 3 || !g_fh[i].name) return 0;
        *out = pl_mk_cmp1("file_name", pl_mk_atom_dup(g_fh[i].name, strlen(g_fh[i].name)));
        return 1;
        case 1:
        *out = pl_mk_cmp1("mode", pl_mk_atom(pl_sp_mode_name(i)));
        return 1;
        case 2:
        if (!pl_sp_is_input(i)) return 0;
        *out = pl_mk_atom("input");
        return 1;
        case 3:
        if (pl_sp_is_input(i)) return 0;
        *out = pl_mk_atom("output");
        return 1;
        case 4:
        *out = pl_mk_cmp1("type", pl_mk_atom(g_fh[i].type == 'b' ? "binary" : "text"));
        return 1;
        case 5:
        { const char *e; if (!pl_sp_is_input(i) || !(e = pl_sp_eos_name(i))) return 0; *out = pl_mk_cmp1("end_of_stream", pl_mk_atom(e)); return 1; }
        case 6:
        {
            extern int fh_eof(int);
            int e = (i == 0) ? 'r' : fh_eof(i);
            *out = pl_mk_cmp1("eof_action", pl_mk_atom(e == 'r' ? "reset" : e == 'e' ? "error" : e == 'c' ? "eof_code" : pl_sp_is_input(i) ? "eof_code" : "error"));
            return 1;
        }
        case 7:
        {
            const char *a = (i == 0) ? "user_input" : (i == 1) ? "user_output" : (i == 2) ? "user_error" : (i >= 3) ? g_fh[i].alias : (const char *)0;
            if (!a) return 0;
            *out = pl_mk_cmp1("alias", pl_mk_atom_dup(a, strlen(a)));
            return 1;
        }
        case 8:
        { extern const char *fh_encoding(int); const char *e; if (g_fh[i].type == 'b') return 0; e = fh_encoding(i); if (!e) return 0; *out = pl_mk_cmp1("encoding", pl_mk_atom(e)); return 1; }
        case 9:
        { if (g_fh[i].type == 'b') return 0; { extern int fh_bom(int); *out = pl_mk_cmp1("bom", pl_mk_atom(fh_bom(i) ? "true" : "false")); } return 1; }
        case 10:
        {
            extern FILE *fh_get(int);
            FILE *fp = fh_get(i);
            long off;
            if (!fp || i < 3) return 0;
            off = ftell(g_fh[i].raw ? g_fh[i].raw : fp);
            if (off < 0) {
                extern int fh_in_position(int, long *, long *, long *);
                extern long fh_pipe_bytes(int);
                long c, l, p;
                if (g_fh[i].mode != 'r' || fh_in_position(i, &c, &l, &p) != 1) return 0;
                *out = pl_mk_cmp1("position", pl_gnu_fn("$stream_position", 4, (DESCR_t[]) { INTVAL(c), INTVAL(l + 1), INTVAL(p), INTVAL(fh_pipe_bytes(i)) }));
                return 1;
            }
            {
                extern int fh_in_position(int, long *, long *, long *);
                long c, l, p;
                if (g_fh[i].mode == 'r' && fh_in_position(i, &c, &l, &p) == 1) {
                    *out = pl_mk_cmp1("position", pl_gnu_fn("$stream_position", 4, (DESCR_t[]) { INTVAL((long long)off), INTVAL(l + 1), INTVAL(p), INTVAL((long long)off) }));
                    return 1;
                }
            }
            *out = pl_mk_cmp1("position", pl_mk_cmp1("$stream_position", INTVAL((long long)off)));
            return 1;
        }
        case 11:
        { extern int fh_repos(int); *out = pl_mk_cmp1("reposition", pl_mk_atom(i >= 3 && fh_repos(i) ? "true" : "false")); return 1; }
        case 12:
        *out = pl_mk_cmp1("buffer", pl_mk_atom(i == 2 ? "false" : i < 2 ? "line" : "full"));
        return 1;
        default:
        return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_enc_norm(const char *s, char *out, size_t n) {
    size_t i, k = 0;
    if (!s) return 0;
    for (i = 0; s[i]; i++) { char c = s[i]; if (c == '-' || c == '_') continue; if (c >= 'A' && c <= 'Z') c = (char)(c - 'A' + 'a'); if (k + 1 >= n) return 0; out[k++] = c; }
    out[k] = '\0';
    return k != 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *const pl_bom_enc[] = { "UTF-8", "UTF-8", "UTF-16BE", "UTF-16LE", "UTF-32BE", "UTF-32LE" };
static int pl_skip_bom(FILE *fp) {
    long pos = ftell(fp);
    unsigned long x = 0;
    size_t n = 0, len = 0;
    int k = 0, ch;
    while (n < 4 && (ch = fgetc(fp)) != EOF) x |= (unsigned long)(unsigned char)ch << (8 * n++);
    if (n == 4 && x == 0xFFFE0000UL) {
        k = 4;
        len = 4;
    } else if (n == 4 && x == 0x0000FEFFUL) {
        k = 5;
        len = 4;
    } else if (n >= 3 && (x & 0xFFFFFFUL) == 0xBFBBEFUL) {
        k = 1;
        len = 3;
    } else if (n >= 2 && (x & 0xFFFFUL) == 0xFFFEUL) {
        k = 2;
        len = 2;
    } else if (n >= 2 && (x & 0xFFFFUL) == 0xFEFFUL) {
        k = 3;
        len = 2;
    }
    if (pos >= 0 && fseek(fp, pos + (long)len, SEEK_SET) == 0) return k;
    while (n > len) { n--; ungetc((int)((x >> (8 * n)) & 0xFF), fp); }
    return k;
}
static int pl_enc_kind(const char *e) {
    if (!strcmp(e, "UTF-16BE")) return 1;
    if (!strcmp(e, "UTF-16LE")) return 2;
    if (!strcmp(e, "UTF-32BE")) return 3;
    if (!strcmp(e, "UTF-32LE") || !strcmp(e, "wchar_t")) return 4;
    if (!strcmp(e, "ISO-8859-1") || !strcmp(e, "US-ASCII") || !strcmp(e, "octet")) return 5;
    return 0;
}
static const char *pl_enc_canon(const char *eb) {
    static const char *const m[][2] = { { "utf8", "UTF-8" }, { "text", "UTF-8" }, { "utf16be", "UTF-16BE" }, { "utf16le", "UTF-16LE" }, { "unicodebe", "UTF-16BE" }, { "unicodele", "UTF-16LE" },
        { "utf32be", "UTF-32BE" }, { "utf32le", "UTF-32LE" }, { "wchart", "wchar_t" }, { "octet", "octet" }, { "isolatin1", "ISO-8859-1" }, { "iso88591", "ISO-8859-1" }, { "latin1", "ISO-8859-1" },
        { "ascii", "US-ASCII" }, { "usascii", "US-ASCII" }, { 0, 0 } };
    for (int i = 0; m[i][0]; i++) if (!strcmp(eb, m[i][0])) return m[i][1];
    return (const char *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_open_opts(DESCR_t *args, int nargs, int idx, pl_tr_ctx_t *cx, int *bom_opt) {
    extern void *rt_pl_ball_instantiation(void);
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    extern void fh_set_untranslated(int, int);
    DESCR_t o;
    if (nargs < 4) return 1;
    o = rt_pl_deref_val(args[3]);
    while (pl_is_cons(o)) {
        DESCR_t *kids = (DESCR_t *)o.p;
        DESCR_t opt = rt_pl_deref_val(kids[0]);
        const char *on;
        if (pl_val_unbound(opt)) { cx->ball = rt_pl_ball_instantiation(); return 0; }
        if (opt.v != (DTYPE_t)DT_PLREF || plc_fid_arity(opt.slen) != 1) { cx->ball = rt_pl_ball_kind2("domain_error", "stream_option", opt); return 0; }
        on = prolog_atom_name(plc_fid_name(opt.slen));
        if (on && strcmp(on, "alias") && strcmp(on, "type") && strcmp(on, "reposition") && strcmp(on, "eof_action") && strcmp(on, "bom") && strcmp(on, "encoding") && pl_swi_dialect() &&
            !pl_float_flag("iso", "true")) {
            o = rt_pl_deref_val(kids[1]);
            continue;
        }
        {
            DESCR_t a = rt_pl_deref_val(((DESCR_t *)opt.p)[0]);
            const char *as = pl_atom_str(a);
            if (pl_val_unbound(a)) { cx->ball = rt_pl_ball_instantiation(); return 0; }
            if (on && !strcmp(on, "alias")) {
                extern void fh_set_alias(int, const char *);
                if (!as) { cx->ball = rt_pl_ball_kind2("domain_error", "stream_option", opt); return 0; }
                {
                    extern int fh_alias_idx(const char *);
                    extern void *rt_pl_ball_permission3(const char *, const char *, DESCR_t);
                    int ex = fh_alias_idx(as);
                    if ((ex >= 0 && ex != idx) || !strcmp(as, "user_input") || !strcmp(as, "user_output") || !strcmp(as, "user_error")) {
                        cx->ball = rt_pl_ball_permission3("open", "source_sink", opt);
                        return 0;
                    }
                }
                fh_set_alias(idx, as);
            } else if (on && !strcmp(on, "type")) {
                if (!as || (strcmp(as, "text") && strcmp(as, "binary"))) { cx->ball = rt_pl_ball_kind2("domain_error", "stream_option", opt); return 0; }
                if (idx >= 0 && idx < FH_N) g_fh[idx].type = (char)(as[0] == 'b' ? 'b' : 't');
                fh_set_untranslated(idx, as[0] == 'b');
            } else if (on && !strcmp(on, "reposition")) {
                extern void fh_set_repos(int, int);
                if (!as || (strcmp(as, "true") && strcmp(as, "false"))) { cx->ball = rt_pl_ball_kind2("domain_error", "stream_option", opt); return 0; }
                fh_set_repos(idx, as[0] == 't');
            } else if (on && !strcmp(on, "eof_action")) {
                extern void fh_set_eof(int, int);
                if (!as || (strcmp(as, "error") && strcmp(as, "eof_code") && strcmp(as, "reset"))) {
                    cx->ball = pl_swi_dialect() && as ? rt_pl_ball_kind2("domain_error", "eof_action", a) : rt_pl_ball_kind2("domain_error", "stream_option", opt);
                    return 0;
                }
                fh_set_eof(idx, !strcmp(as, "error") ? 'e' : !strcmp(as, "reset") ? 'r' : 'c');
            } else if (on && !strcmp(on, "bom")) {
                if (!as || (strcmp(as, "true") && strcmp(as, "false"))) { cx->ball = rt_pl_ball_kind2("domain_error", "stream_option", opt); return 0; }
                if (bom_opt) *bom_opt = (as[0] == 't');
            } else if (on && !strcmp(on, "encoding")) {
                extern void fh_set_encoding(int, const char *);
                char eb[(as ? strlen(as) : 0) + 1];
                const char *en;
                if (!as || !pl_enc_norm(as, eb, sizeof eb)) { cx->ball = rt_pl_ball_kind2("domain_error", "stream_option", opt); return 0; }
                en = pl_enc_canon(eb);
                if (!en) { cx->ball = rt_pl_ball_kind2("domain_error", "stream_option", opt); return 0; }
                fh_set_encoding(idx, en);
            } else if (on && !strcmp(on, "buffering") && pl_gnu_dialect()) {
                if (!as || (strcmp(as, "none") && strcmp(as, "line") && strcmp(as, "block"))) { cx->ball = rt_pl_ball_kind2("domain_error", "stream_option", opt); return 0; }
                g_fh[idx].buf = as[0] == 'n' ? 'n' : as[0] == 'l' ? 'l' : 'b';
            } else if (on && !strcmp(on, "mirror") && pl_gnu_dialect()) {
                int m = pl_stream_idx_ball(a, 1, 0, cx);
                if (m < 0) return 0;
                pl_fh_push(&g_fh[idx].mir, &g_fh[idx].nmir, m);
            } else {
                cx->ball = rt_pl_ball_kind2("domain_error", "stream_option", opt);
                return 0;
            }
        }
        o = rt_pl_deref_val(kids[1]);
    }
    if (pl_val_unbound(o)) { cx->ball = rt_pl_ball_instantiation(); return 0; }
    if (!pl_is_nil(o)) { cx->ball = rt_pl_ball_kind2("type_error", "list", rt_pl_deref_val(args[3])); return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_open_pipe(DESCR_t *args, int nargs, DESCR_t f, DESCR_t m, pl_tr_ctx_t *cx) {
    extern int fh_alloc(FILE *);
    extern int fh_is_untranslated(int);
    extern void fh_free(int);
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    extern void *rt_pl_ball_instantiation(void);
    DESCR_t c = rt_pl_deref_val(((DESCR_t *)f.p)[0]);
    const char *cmd = pl_atom_str(c), *md = pl_atom_str(m);
    const char *pm = md && !strcmp(md, "read") ? "r" : md && !strcmp(md, "write") ? "w" : (const char *)0;
    FILE *fp;
    int idx, bom_opt = -1;
    if (pl_val_unbound(c)) { cx->ball = rt_pl_ball_instantiation(); return 0; }
    if (!cmd) { cx->ball = rt_pl_ball_kind2("type_error", "atom", c); return 0; }
    if (!pm && md && (!strcmp(md, "append") || !strcmp(md, "update"))) { cx->ball = rt_pl_ball_kind2("existence_error", "source_sink", f); return 0; }
    if (!pm) { cx->ball = rt_pl_ball_kind2("domain_error", "io_mode", m); return 0; }
    fflush((FILE *)0);
    fp = popen(cmd, pm);
    if (!fp) { cx->ball = rt_pl_ball_kind2("existence_error", "source_sink", f); return 0; }
    if (pm[0] == 'w') signal(SIGPIPE, SIG_IGN);
    idx = fh_alloc(fp);
    if (idx < 0) { pclose(fp); return 0; }
    g_fh[idx].mode = pm[0];
    g_fh[idx].repos = 0;
    if (!pl_open_opts(args, nargs, idx, cx, &bom_opt)) { pclose(fp); fh_free(idx); return 0; }
    if (pm[0] == 'r' && bom_opt != 0 && g_fh[idx].type != 'b' && !fh_is_untranslated(idx)) {
        extern void fh_set_bom(int, int);
        extern void fh_set_encoding(int, const char *);
        extern int fh_transcode(int, int, const char *);
        int ch = fgetc(fp), bk = 0;
        if (ch != EOF) ungetc(ch, fp);
        if (ch == 0xEF || ch == 0xFE || ch == 0xFF || ch == 0) bk = pl_skip_bom(fp);
        fh_set_bom(idx, bk != 0);
        if (bk && strcmp(pl_bom_enc[bk], "UTF-8")) fh_set_encoding(idx, pl_bom_enc[bk]);
        if (bk && pl_enc_kind(pl_bom_enc[bk]) && !fh_transcode(idx, pl_enc_kind(pl_bom_enc[bk]), pm)) { pclose(fp); fh_free(idx); return 0; }
    }
    return plw_unify_vals(args[2], pl_mk_stream(idx), cx);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_open_leaf(DESCR_t *args, int nargs, pl_tr_ctx_t *cx) {
    extern int fh_alloc(FILE *);
    extern int rt_pl_dialect_xsb(void);
    extern int prolog_atom_intern(const char *);
    extern void *rt_pl_ball_instantiation(void);
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    extern void *rt_pl_ball_permission3(const char *, const char *, DESCR_t);
    extern int fh_is_untranslated(int);
    extern void fh_set_bom(int, int);
    char fb[4096], mb[64];
    const char *fn, *md;
    FILE *fp;
    const char *fmode;
    DESCR_t f = rt_pl_deref_val(args[0]);
    DESCR_t m = rt_pl_deref_val(args[1]);
    if (pl_val_unbound(f) || pl_val_unbound(m)) { cx->ball = rt_pl_ball_instantiation(); return 0; }
    if (m.v != (DTYPE_t)DT_S && m.v != (DTYPE_t)DT_PLATOM) { cx->ball = rt_pl_ball_kind2("type_error", "atom", m); return 0; }
    if (!pl_val_unbound(rt_pl_deref_val(args[2]))) {
        extern void *rt_pl_ball_culprit1(const char *, DESCR_t);
        cx->ball = rt_pl_ball_culprit1("uninstantiation_error", rt_pl_deref_val(args[2]));
        return 0;
    }
    if (f.v == (DTYPE_t)DT_PLREF && rt_pl_dialect_xsb() && plc_fid_arity(f.slen) == 1 && plc_fid_name(f.slen) == prolog_atom_intern("atom") && pl_atom_str(m) && !strcmp(pl_atom_str(m), "read")) {
        DESCR_t a0 = rt_pl_deref_val(((DESCR_t *)f.p)[0]);
        const char *txt = pl_atom_str(a0);
        FILE *tf;
        int ti;
        if (pl_val_unbound(a0)) { cx->ball = rt_pl_ball_instantiation(); return 0; }
        if (txt) {
            tf = tmpfile();
            if (!tf || (txt[0] && fwrite(txt, 1, strlen(txt), tf) != strlen(txt))) { if (tf) fclose(tf); cx->ball = rt_pl_ball_kind2("resource_error", "memory", f); return 0; }
            rewind(tf);
            ti = fh_alloc(tf);
            if (ti < 0) { fclose(tf); return 0; }
            g_fh[ti].mode = 'r';
            g_fh[ti].name = rt_heap_strdup_c("atom stream");
            return plw_unify_vals(args[2], pl_mk_stream(ti), cx);
        }
    }
    if (f.v == (DTYPE_t)DT_PLREF && plc_fid_arity(f.slen) == 1 && plc_fid_name(f.slen) == prolog_atom_intern("pipe")) return pl_open_pipe(args, nargs, f, m, cx);
    if (f.v != (DTYPE_t)DT_S && f.v != (DTYPE_t)DT_PLATOM) { cx->ball = rt_pl_ball_kind2("domain_error", "source_sink", f); return 0; }
    if (!pl_cell_text(args[0], fb, sizeof fb, &fn) || !pl_cell_text(args[1], mb, sizeof mb, &md)) return 0;
    fmode = !strcmp(md, "read") ? "r" : !strcmp(md, "write") ? "w" : !strcmp(md, "append") ? "a" : !strcmp(md, "update") ? "r+" : (const char *)0;
    if (!fmode) { cx->ball = rt_pl_ball_kind2("domain_error", "io_mode", m); return 0; }
    if (nargs >= 4) {
        extern int fh_alias_idx(const char *);
        extern int prolog_atom_intern(const char *);
        DESCR_t ol;
        for (ol = rt_pl_deref_val(args[3]); pl_is_cons(ol); ol = rt_pl_deref_val(((DESCR_t *)ol.p)[1])) {
            DESCR_t oe = rt_pl_deref_val(((DESCR_t *)ol.p)[0]), oa;
            const char *as;
            if (oe.v != (DTYPE_t)DT_PLREF || plc_fid_arity(oe.slen) != 1 || plc_fid_name(oe.slen) != prolog_atom_intern("alias")) continue;
            oa = rt_pl_deref_val(((DESCR_t *)oe.p)[0]);
            as = pl_atom_str(oa);
            if (as && (fh_alias_idx(as) >= 0 || !strcmp(as, "user_input") || !strcmp(as, "user_output") || !strcmp(as, "user_error"))) {
                cx->ball = rt_pl_ball_permission3("open", "source_sink", oe);
                return 0;
            }
        }
    }
    fp = fopen(fn, fmode);
    if (!fp) { cx->ball = (errno == EACCES || errno == EPERM || errno == EROFS) ? rt_pl_ball_permission3("open", "source_sink", f) : rt_pl_ball_kind2("existence_error", "source_sink", f); return 0; }
    {
        int idx = fh_alloc(fp);
        if (idx < 0) { fclose(fp); return 0; }
        g_fh[idx].name = rt_heap_strdup_c(fn);
        g_fh[idx].mode = (char) fmode[0];
        if (fmode[0] == 'a') { off_t e = lseek(fileno(fp), 0, SEEK_END); g_fh[idx].pos_off = e > 0 ? (long)e : 0; }
        {
            int bom_opt = -1;
            if (!pl_open_opts(args, nargs, idx, cx, &bom_opt)) { fclose(fp); fh_free(idx); return 0; }
            if (g_fh[idx].type != 'b' && !fh_is_untranslated(idx)) {
                extern void fh_set_encoding(int, const char *);
                extern int fh_transcode(int, int, const char *);
                int bk = fmode[0] == 'r' && bom_opt != 0 ? pl_skip_bom(fp) : 0;
                int wbom = fmode[0] != 'r' && bom_opt == 1 && (fmode[0] == 'w' || ftell(fp) == 0);
                const char *en = bk ? pl_bom_enc[bk] : g_fh[idx].enc ? g_fh[idx].enc : pl_flag_encoding();
                if (fmode[0] == 'r') fh_set_bom(idx, bk != 0);
                if (strcmp(en, "UTF-8")) fh_set_encoding(idx, en);
                if (pl_enc_kind(en) && !fh_transcode(idx, pl_enc_kind(en), fmode)) { fclose(fp); fh_free(idx); return 0; }
                if (wbom) { fputs("\357\273\277", g_fh[idx].fp); fh_set_bom(idx, 1); }
            }
        }
        return plw_unify_vals(args[2], pl_mk_stream(idx), cx);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_set_stream_bool(const char *as) { return as && (!strcmp(as, "true") || !strcmp(as, "false")); }
PL_CX_LEAF_HEAD(set_stream, 2)
    {
        extern void *rt_pl_ball_instantiation(void);
        extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
        extern void fh_set_alias(int, const char *);
        extern int fh_alias_idx(const char *);
        extern void fh_set_eof(int, int);
        extern void fh_set_encoding(int, const char *);
        extern int fh_transcode(int, int, const char *);
        extern int fh_untranscode(int);
        int idx = pl_stream_idx_ball(args[0], -1, 0, cx);
        DESCR_t p = rt_pl_deref_val(args[1]), a;
        const char *on, *as;
        ok = 0;
        if (idx < 0) goto done;
        if (pl_val_unbound(p)) { cx->ball = rt_pl_ball_instantiation(); goto done; }
        if (p.v != (DTYPE_t)DT_PLREF || plc_fid_arity(p.slen) != 1) { cx->ball = rt_pl_ball_kind2("domain_error", "stream_attribute", p); goto done; }
        on = prolog_atom_name(plc_fid_name(p.slen));
        a = rt_pl_deref_val(((DESCR_t *)p.p)[0]);
        as = pl_atom_str(a);
        if (pl_val_unbound(a)) { cx->ball = rt_pl_ball_instantiation(); goto done; }
        if (!on) cx->ball = rt_pl_ball_kind2("domain_error", "stream_attribute", p);
        else if (!strcmp(on, "alias")) {
            if (!as) { cx->ball = rt_pl_ball_kind2("type_error", "atom", a); goto done; }
            { int j = fh_alias_idx(as); if (j >= 0 && j != idx) fh_set_alias(j, (const char *)0); }
            fh_set_alias(idx, as);
            ok = 1;
        } else if (!strcmp(on, "eof_action")) {
            if (!as) { cx->ball = rt_pl_ball_kind2("type_error", "atom", a); goto done; }
            if (!strcmp(as, "error") || !strcmp(as, "eof_code") || !strcmp(as, "reset")) fh_set_eof(idx, !strcmp(as, "error") ? 'e' : !strcmp(as, "reset") ? 'r' : 'c');
            ok = 1;
        } else if (!strcmp(on, "type")) {
            if (!as || (strcmp(as, "text") && strcmp(as, "binary"))) { cx->ball = rt_pl_ball_kind2("domain_error", "type", a); goto done; }
            g_fh[idx].type = as[0] == 'b' ? 'b' : 't';
            ok = 1;
        } else if (!strcmp(on, "encoding")) {
            char eb[(as ? strlen(as) : 0) + 1];
            const char *en = as && pl_enc_norm(as, eb, sizeof eb) ? pl_enc_canon(eb) : (const char *)0;
            int k;
            if (!en) { cx->ball = rt_pl_ball_kind2("domain_error", "encoding", a); goto done; }
            k = pl_enc_kind(en);
            if (k) ok = fh_transcode(idx, k, g_fh[idx].mode == 'r' ? "r" : "w");
            else { if (g_fh[idx].raw) fh_untranscode(idx); ok = 1; }
            if (ok) fh_set_encoding(idx, strcmp(en, "UTF-8") ? en : (const char *)0);
        } else if (!strcmp(on, "line_position")) {
            if (a.v != DT_I) { cx->ball = rt_pl_ball_kind2("type_error", "integer", a); goto done; }
            { extern int fh_query_position(int, long *, long *, long *); long qc, ql, qp; (void)fh_query_position(idx, &qc, &ql, &qp); }
            g_fh[idx].pos_lpos = (long)a.i;
            g_fh[idx].gpos_lpos = (long)a.i;
            ok = 1;
        } else if (!strcmp(on, "file_name")) {
            extern char *rt_heap_strdup_c(const char *);
            if (!as) { cx->ball = rt_pl_ball_kind2("type_error", "atom", a); goto done; }
            g_fh[idx].name = rt_heap_strdup_c(as);
            ok = 1;
        } else if (!strcmp(on, "buffer")) {
            ok = as && (!strcmp(as, "full") || !strcmp(as, "line") || !strcmp(as, "false"));
            if (!ok) cx->ball = rt_pl_ball_kind2("domain_error", "buffer", a);
        } else if (!strcmp(on, "buffer_size") || !strcmp(on, "timeout")) {
            ok = a.v == DT_I || a.v == DT_R || (as && !strcmp(as, "infinite"));
            if (!ok) cx->ball = rt_pl_ball_kind2("type_error", "integer", a);
        } else if (!strcmp(on, "close_on_abort") || !strcmp(on, "close_on_exec") || !strcmp(on, "tty") || !strcmp(on, "record_position")) {
            ok = pl_set_stream_bool(as);
            if (!ok) cx->ball = rt_pl_ball_kind2("type_error", "bool", a);
            else if (!strcmp(on, "tty")) g_fh[idx].tty = as[0] == 't' ? 't' : 'f';
        } else if (!strcmp(on, "newline")) {
            ok = as && (!strcmp(as, "posix") || !strcmp(as, "dos") || !strcmp(as, "detect"));
            if (!ok) cx->ball = rt_pl_ball_kind2("domain_error", "newline", a);
        } else if (!strcmp(on, "representation_errors")) {
            ok = as && (!strcmp(as, "error") || !strcmp(as, "prolog") || !strcmp(as, "xml"));
            if (!ok) cx->ball = rt_pl_ball_kind2("domain_error", "representation_errors", a);
        } else cx->ball = rt_pl_ball_kind2("domain_error", "stream_attribute", p);
        done:
        ;
    }
PL_CX_LEAF_TAIL
DESCR_t dop_pl_at_end_of_stream(DESCR_t *args, int nargs) {
    extern FILE *fh_cur_in_fp(void);
    int c;
    (void)args;
    if (nargs != 0) return FAILDESCR;
    c = fgetc(fh_cur_in_fp());
    if (c == EOF) { clearerr(fh_cur_in_fp()); return pl_ok(); }
    ungetc(c, fh_cur_in_fp());
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t dop_pl_get0(DESCR_t *args, int nargs) { (void)args; (void)nargs; return FAILDESCR; }
DESCR_t dop_pl_seen(DESCR_t *args, int nargs) { (void)args; if (nargs != 0) return FAILDESCR; pl_edin_revert(0); return pl_ok(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_close_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx) {
    extern FILE *fh_get(int);
    extern void fh_free(int);
    extern int fh_current_output(void);
    extern int fh_current_input(void);
    extern void fh_set_output(int);
    extern void fh_set_input(int);
    void *b1 = (void *)0, *b2 = (void *)0;
    int idx;
    if (nargs < 1 || nargs > 2) return FAILDESCR;
    pl_atoms_ready();
    rt_pl_tr_gc_sync(cx->tr);
    if (nargs == 2 && !pl_val_unbound(rt_pl_deref_val(args[0]))) {
        extern void *rt_pl_ball_instantiation(void);
        extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
        extern int prolog_atom_intern(const char *);
        DESCR_t o = rt_pl_deref_val(args[1]), cur, e, v;
        const char *vs;
        for (cur = o; pl_is_cons(cur); cur = rt_pl_deref_val(((DESCR_t *)cur.p)[1])) {
            e = rt_pl_deref_val(((DESCR_t *)cur.p)[0]);
            if (pl_val_unbound(e)) { cx->ball = rt_pl_ball_instantiation(); return FAILDESCR; }
            if (e.v == (DTYPE_t)DT_PLREF && plc_fid_arity(e.slen) == 1 && plc_fid_name(e.slen) == prolog_atom_intern("force") && pl_val_unbound(rt_pl_deref_val(((DESCR_t *)e.p)[0]))) {
                cx->ball = rt_pl_ball_instantiation();
                return FAILDESCR;
            }
            if (!(e.v == (DTYPE_t)DT_PLREF && plc_fid_name(e.slen) == prolog_atom_intern("force") && plc_fid_arity(e.slen) == 1 && (vs = pl_atom_str(v = rt_pl_deref_val(((DESCR_t *)e.p)[0]))) &&
                (!strcmp(vs, "true") || !strcmp(vs, "false")))) {
                cx->ball = rt_pl_ball_kind2("domain_error", "close_option", e);
                return FAILDESCR;
            }
        }
        if (pl_val_unbound(cur)) { cx->ball = rt_pl_ball_instantiation(); return FAILDESCR; }
        if (!pl_is_nil(cur)) { cx->ball = rt_pl_ball_kind2("type_error", "list", o); return FAILDESCR; }
    }
    {
        extern int prolog_atom_intern(const char *);
        DESCR_t pd = rt_pl_deref_val(args[0]);
        if (pd.v == (DTYPE_t)DT_PLREF && plc_fid_name(pd.slen) == prolog_atom_intern("$stream_pair") && plc_fid_arity(pd.slen) == 2) {
            for (int side = 0; side < 2; side++) {
                void *pb = (void *)0;
                int pi = pl_stream_resolve(((DESCR_t *)pd.p)[side], side, 0, &pb);
                if (pi >= 3) { FILE *pfp = fh_get(pi); if (pfp) fclose(pfp); fh_free(pi); if (fh_current_output() == pi) fh_set_output(1); if (fh_current_input() == pi) fh_set_input(0); }
            }
            rt_pl_tr_gc_sync(cx->tr);
            return pl_ok();
        }
    }
    idx = pl_stream_resolve(args[0], 1, 0, &b1);
    if (idx < 0) idx = pl_stream_resolve(args[0], 0, 0, &b2);
    if (idx < 0) { cx->ball = b2 ? b2 : b1; rt_pl_tr_gc_sync(cx->tr); return FAILDESCR; }
    if (idx >= 3 && g_fh[idx].tsk) {
        extern void *rt_pl_ball_kind1(const char *, const char *);
        cx->ball = rt_pl_ball_kind1("system_error", "special stream: needs appropriate close predicate");
        rt_pl_tr_gc_sync(cx->tr);
        return FAILDESCR;
    }
    if (idx >= 3) { FILE *fp = fh_get(idx); if (fp) fclose(fp); fh_free(idx); if (fh_current_output() == idx) fh_set_output(1); if (fh_current_input() == idx) fh_set_input(0); }
    rt_pl_tr_gc_sync(cx->tr);
    return pl_ok();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
PL_CX_LEAF_HEAD(at_end_of_stream_s, 1)
    {
        extern int fh_current_input(void);
        extern void fh_set_input(int);
        int idx = pl_stream_idx_ball(args[0], 0, 0, cx);
        ok = 0;
        if (idx >= 0) { int sv = fh_current_input(); fh_set_input(idx); ok = (dop_pl_at_end_of_stream((DESCR_t *)0, 0).v == (DTYPE_t)DT_I); fh_set_input(sv); }
    }
PL_CX_LEAF_TAIL
static int pl_gs_sidx(DESCR_t s) {
    DESCR_t k;
    if (s.v != (DTYPE_t)DT_PLREF || plc_fid_arity(s.slen) != 1 || plc_fid_name(s.slen) != prolog_atom_intern("$stream")) return -1;
    k = rt_pl_deref_val(((DESCR_t *)s.p)[0]);
    return (k.v == (DTYPE_t)DT_I && k.i >= 0 && k.i < FH_N && g_fh[k.i].fp) ? (int)k.i : -1;
}
int pl_gs_ts(const char *op, DESCR_t a, DESCR_t res, pl_tr_ctx_t *cx) {
    extern int fh_alloc(FILE *);
    extern void fh_set_alias(int, const char *);
    if (!strcmp(op, "in") || !strcmp(op, "out")) {
        const char *txt = op[0] == 'i' ? pl_atom_str(a) : "";
        FILE *fp = tmpfile();
        int idx;
        if (!fp || !txt || (txt[0] && fwrite(txt, 1, strlen(txt), fp) != strlen(txt))) { if (fp) fclose(fp); return pl_gnu_os_fail(cx); }
        rewind(fp);
        idx = fh_alloc(fp);
        g_fh[idx].mode = op[0] == 'i' ? 'r' : 'w';
        g_fh[idx].tsk = op[0];
        g_fh[idx].name = rt_heap_strdup_c("constant term stream");
        return plw_unify_vals(res, pl_mk_stream(idx), cx);
    }
    if (!strcmp(op, "info")) {
        extern int fh_repos(int);
        int idx = pl_gs_sidx(a);
        DESCR_t v[4];
        char ty[2] = { 0, 0 }, md[2] = { 0, 0 };
        long off;
        if (idx < 0) return 0;
        off = g_fh[idx].fp ? ftell(g_fh[idx].fp) : 0;
        ty[0] = g_fh[idx].type == 'b' ? 'b' : 't';
        md[0] = idx == 0 || g_fh[idx].mode == 'r' ? 'r' : 'w';
        v[0] = pl_mk_atom(ty);
        v[1] = pl_mk_atom(md);
        v[2] = pl_mk_atom(idx < 3 || fh_repos(idx) ? "true" : "false");
        v[3] = INTVAL(off > 0 ? 1 : 0);
        return plw_unify_vals(res, pl_gnu_fn("si", 4, v), cx);
    }
    if (!strcmp(op, "settype") || !strcmp(op, "seteof") || !strcmp(op, "setbuf")) {
        extern void fh_set_eof(int, int);
        int idx = pl_gs_sidx(pl_gs_arg(a, 0, 2));
        const char *k = pl_atom_str(pl_gs_arg(a, 1, 2));
        if (idx < 0 || !k) return 0;
        if (op[3] == 't') g_fh[idx].type = k[0] == 'b' ? 'b' : 't';
        else if (op[3] == 'e') fh_set_eof(idx, k[0] == 'e' && k[1] == 'r' ? 'e' : k[0] == 'e' ? 'c' : 'r');
        else g_fh[idx].buf = k[0] == 'n' ? 'n' : k[0] == 'l' ? 'l' : 'b';
        return 1;
    }
    if (!strcmp(op, "pos")) {
        extern int fh_in_position(int, long *, long *, long *);
        int idx = pl_gs_sidx(a);
        long ch = 0, ln = 0, lp = 0, off = idx >= 0 && g_fh[idx].fp ? ftell(g_fh[idx].fp) : -1;
        DESCR_t v[4];
        if (off < 0 || fh_in_position(idx, &ch, &ln, &lp) != 1) return 0;
        v[0] = INTVAL(off);
        v[1] = INTVAL(ch);
        v[2] = INTVAL(ln);
        v[3] = INTVAL(lp);
        return plw_unify_vals(res, pl_gnu_fn("$stream_position", 4, v), cx);
    }
    if (!strcmp(op, "linecol")) {
        extern int fh_in_position(int, long *, long *, long *);
        int idx = pl_gs_sidx(a);
        long ch = 0, ln = 0, lp = 0;
        DESCR_t v[2];
        if (idx < 0 || fh_in_position(idx, &ch, &ln, &lp) != 1) return 0;
        v[0] = INTVAL(ln + 1);
        v[1] = INTVAL(lp + 1);
        return plw_unify_vals(res, pl_gnu_fn("lc", 2, v), cx);
    }
    if (!strcmp(op, "setlinecol")) {
        int idx = pl_gs_sidx(pl_gs_arg(a, 0, 3));
        long l = (long)pl_gs_arg(a, 1, 3).i - 1, c = (long)pl_gs_arg(a, 2, 3).i - 1, ln = 0, lp = 0, off;
        FILE *fp = idx >= 0 ? g_fh[idx].fp : (FILE *)0;
        int ch = 0;
        if (!fp || l < 0 || c < 0 || (off = ftell(fp)) < 0 || fseek(fp, 0, SEEK_SET) != 0) return 0;
        while (ln < l && (ch = fgetc(fp)) != EOF) if (ch == '\n') ln++;
        while (ch != EOF && ln == l && lp < c && (ch = fgetc(fp)) != EOF && ch != '\n') lp++;
        if (ln == l && lp == c) return 1;
        fseek(fp, off, SEEK_SET);
        return 0;
    }
    if (!strcmp(op, "seek")) {
        int idx = pl_gs_sidx(pl_gs_arg(a, 0, 3));
        const char *w = pl_atom_str(pl_gs_arg(a, 1, 3));
        DESCR_t o = pl_gs_arg(a, 2, 3);
        FILE *fp = idx >= 0 ? g_fh[idx].fp : (FILE *)0;
        long n;
        if (!fp || !w || fseek(fp, (long)o.i, w[0] == 'b' ? SEEK_SET : w[0] == 'c' ? SEEK_CUR : SEEK_END) != 0 || (n = ftell(fp)) < 0) return 0;
        return plw_unify_vals(res, INTVAL(n), cx);
    }
    if (!strcmp(op, "sp_multi")) { int idx = pl_atom_str(a) ? pl_sp_named(a) : pl_gs_sidx(a); return idx >= 0 && idx < FH_N && g_fh[idx].nxal + (idx >= 3 ? g_fh[idx].nmir : 0) > 0; }
    if (!strcmp(op, "unmark")) { int idx = pl_gs_sidx(a); if (idx >= 0) g_fh[idx].tsk = 0; return idx >= 0; }
    if (!strcmp(op, "kind")) { int idx = pl_gs_sidx(a); char k[2] = { idx >= 0 && g_fh[idx].tsk ? g_fh[idx].tsk : 'n', 0 }; return plw_unify_vals(res, pl_mk_atom(k), cx); }
    if (!strcmp(op, "take")) {
        int idx = pl_gs_sidx(a);
        FILE *fp = idx >= 0 ? g_fh[idx].fp : (FILE *)0;
        char *b = (char *)0;
        size_t bn = 0, k = 0, got;
        long end;
        if (!fp || fflush(fp) != 0 || (end = ftell(fp)) < 0 || fseek(fp, 0, SEEK_SET) != 0) return pl_gnu_os_fail(cx);
        while (pl_rb_room(&b, &bn, k + 4096) && (got = fread(b + k, 1, 4096, fp)) > 0) k += got;
        fseek(fp, end, SEEK_SET);
        return plw_unify_vals(res, pl_mk_atom_dup(b ? b : "", k), cx);
    }
    if (!strcmp(op, "alias")) { int idx = pl_gs_sidx(pl_gs_arg(a, 0, 2)); const char *nm = pl_atom_str(pl_gs_arg(a, 1, 2)); if (idx < 0 || !nm) return 0; pl_fh_addal(idx, nm); return 1; }
    if (!strcmp(op, "realias")) {
        extern int fh_alias_idx(const char *);
        int idx = pl_gs_sidx(pl_gs_arg(a, 0, 2));
        const char *nm = pl_atom_str(pl_gs_arg(a, 1, 2));
        int j;
        if (idx < 0 || !nm) return 0;
        j = fh_alias_idx(nm);
        if (j >= 0 && j != idx) {
            if (g_fh[j].alias && !strcmp(g_fh[j].alias, nm)) fh_set_alias(j, (const char *)0);
            for (int k = 0; k < g_fh[j].nxal; k++) {
                if (strcmp(prolog_atom_name(g_fh[j].xal[k]), nm)) continue;
                for (int m = k; m + 1 < g_fh[j].nxal; m++) g_fh[j].xal[m] = g_fh[j].xal[m + 1];
                g_fh[j].nxal--;
                break;
            }
        }
        pl_fh_addal(idx, nm);
        return 1;
    }
    if (!strcmp(op, "mir_add") || !strcmp(op, "mir_del")) {
        int idx = pl_gs_sidx(pl_gs_arg(a, 0, 2)), m = pl_gs_sidx(pl_gs_arg(a, 1, 2)), w = 0, hit = 0;
        if (idx < 0 || m < 0) return 0;
        if (op[4] == 'a') { for (int k = 0; k < g_fh[idx].nmir; k++) if (g_fh[idx].mir[k] == m) return 1; pl_fh_push(&g_fh[idx].mir, &g_fh[idx].nmir, m); return 1; }
        for (int k = 0; k < g_fh[idx].nmir; k++) { if (!hit && g_fh[idx].mir[k] == m) { hit = 1; continue; } g_fh[idx].mir[w++] = g_fh[idx].mir[k]; }
        g_fh[idx].nmir = w;
        return hit;
    }
    return 0;
}
PL_CX_LEAF_HEAD(pl_cs_nth, 4)
    {
        DESCR_t iv = rt_pl_deref_val(args[0]);
        int si;
        ok = 0;
        if (iv.v == DT_I) {
            si = (int)iv.i - 1;
            if (si >= 3 && si < FH_N && pl_sp_stream_live(si) && g_fh[si].name) ok = plw_unify_vals(args[1], pl_mk_atom_dup(g_fh[si].name, strlen(g_fh[si].name)), cx) &&
                plw_unify_vals(args[2], pl_mk_atom(pl_sp_mode_name(si)), cx) && plw_unify_vals(args[3], pl_mk_stream(si), cx);
        }
    }
PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(pl_ioarg, 2)
    {
        extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
        extern void *rt_pl_ball_kind1(const char *, const char *);
        extern void *rt_pl_ball_instantiation(void);
        extern void *rt_pl_ball_permission3(const char *, const char *, DESCR_t);
        extern int fh_current_output(void);
        extern int fh_current_input(void);
        const char *k = pl_atom_str(rt_pl_deref_val(args[0]));
        DESCR_t c = rt_pl_deref_val(args[1]);
        const char *cs;
        int out, sx;
        ok = 1;
        if (!k) return pl_ok();
        out = !strncmp(k, "put_", 4);
        sx = out ? fh_current_output() : fh_current_input();
        if (!strncmp(k, "in_byte", 7)) {
            if (!pl_val_unbound(c) && (c.v != DT_I || c.i < -1 || c.i > 255)) cx->ball = rt_pl_ball_kind2("type_error", "in_byte", c);
        } else if (k[strlen(k) - 1] == '1' && sx >= 0 && sx < FH_N && g_fh[sx].type == 'b') {
            DESCR_t who = !pl_gnu_dialect() ? pl_mk_stream(sx) : pl_mk_atom(out ? "current_output_stream" : "current_input_stream");
            cx->ball = rt_pl_ball_permission3(out ? "output" : "input", "binary_stream", who);
        } else if (!strncmp(k, "put_code", 8)) {
            if (pl_val_unbound(c)) cx->ball = rt_pl_ball_instantiation();
            else if (c.v != DT_I) cx->ball = rt_pl_ball_kind2("type_error", "integer", c);
            else if (c.i < 0 || c.i > 0x10FFFF) cx->ball = rt_pl_ball_kind1("representation_error", "character_code");
        } else if (!strncmp(k, "put_char", 8)) {
            if (pl_val_unbound(c)) cx->ball = rt_pl_ball_instantiation();
            else if (!(cs = pl_atom_str(c)) || pl_is_nil(c) || !cs[0] || utf8_seqlen((unsigned char)cs[0]) != (int)strlen(cs)) cx->ball = rt_pl_ball_kind2("type_error", "character", c);
        } else if (!strncmp(k, "in_code", 7)) {
            if (!pl_val_unbound(c) && c.v != DT_I) cx->ball = rt_pl_ball_kind2("type_error", "integer", c);
            else if (!pl_val_unbound(c) && (c.i < -1 || c.i > 0x10FFFF)) cx->ball = rt_pl_ball_kind1("representation_error", "in_character_code");
        } else if (!strncmp(k, "in_char", 7) && !pl_val_unbound(c)) {
            cs = pl_atom_str(c);
            if (!cs || pl_is_nil(c) || (strcmp(cs, "end_of_file") && (!cs[0] || utf8_seqlen((unsigned char)cs[0]) != (int)strlen(cs)))) cx->ball = rt_pl_ball_kind2("type_error", "in_character", c);
        }
        if (cx->ball) ok = 0;
    }
PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(pl_sp_check, 2)
    {
        extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
        extern int prolog_atom_intern(const char *);
        static const char *const pn[] = { "file_name", "mode", "input", "output", "alias", "position", "end_of_stream", "eof_action", "reposition", "type", "encoding", "bom", "newline", "buffer", 0 };
        DESCR_t s = rt_pl_deref_val(args[0]), p = rt_pl_deref_val(args[1]);
        const char *nm = (const char *)0;
        int i;
        ok = 1;
        if (!pl_val_unbound(s)) {
            if (s.v == (DTYPE_t)DT_PLREF && plc_fid_name(s.slen) == prolog_atom_intern("$stream") && plc_fid_arity(s.slen) == 1) {
                DESCR_t k = rt_pl_deref_val(((DESCR_t *)s.p)[0]);
                if (k.v != DT_I || k.i < 0 || k.i >= FH_N || !pl_sp_stream_live((int)k.i)) cx->ball = rt_pl_ball_kind2("existence_error", "stream", s);
            } else if (pl_sp_named(s) < 0) cx->ball = rt_pl_ball_kind2("domain_error", "stream", s);
        }
        if (!cx->ball && !pl_val_unbound(p)) {
            nm = pl_atom_str(p);
            if (!nm && p.v == (DTYPE_t)DT_PLREF) nm = prolog_atom_name(plc_fid_name(p.slen));
            for (i = 0; nm && pn[i]; i++) if (!strcmp(nm, pn[i])) break;
            if ((!nm || !pn[i]) && !(nm && (!strcmp(nm, "mirror") || !strcmp(nm, "buffering")) && pl_gnu_dialect()) && !(nm && !strcmp(nm, "tty") && !pl_gnu_dialect())) cx->ball =
                rt_pl_ball_kind2("domain_error", "stream_property", p);
        }
        if (cx->ball) ok = 0;
    }
PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(pl_sp_count, 1) { long long n = 0; for (int i = 0; i < FH_N; i++) n += pl_sp_nprops(i); ok = plw_unify_vals(args[0], INTVAL(n), cx); } PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(pl_sp_nth, 3)
    {
        extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
        DESCR_t iv = rt_pl_deref_val(args[0]);
        DESCR_t sd = rt_pl_deref_val(args[1]);
        DESCR_t pv;
        int k, si, pi, want = -1, named;
        ok = 0;
        named = pl_atom_str(sd) != (const char *)0;
        if (named && (want = pl_sp_named(sd)) < 0) cx->ball = rt_pl_ball_kind2("existence_error", "stream", sd);
        else if (iv.v == DT_I) {
            k = (int)iv.i - 1;
            for (si = 0; si < FH_N && k >= pl_sp_nprops(si); si++) k -= pl_sp_nprops(si);
            pi = k;
            DESCR_t pa = rt_pl_deref_val(args[2]);
            int asked = pa.v == (DTYPE_t)DT_PLREF && plc_fid_arity(pa.slen) == 1 && plc_fid_name(pa.slen) == prolog_atom_intern("buffering");
            if (pi == 13 && !asked) si = -1;
            if (si >= 0 && si < FH_N && pl_sp_stream_live(si) && (named ? si == want : plw_unify_vals(args[1], pl_mk_stream(si), cx)) && pl_sp_prop(si, pi, &pv)) ok = plw_unify_vals(args[2], pv, cx);
        }
    }
PL_CX_LEAF_TAIL
