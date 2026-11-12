#include "runtime/lib/bn_head.h"
#include "runtime/lib/bn_05_pl.h"
#include "runtime/lib/bn_11_pl.h"
#include "runtime/lib/bn_27_pl.h"
#include "runtime/lib/bn_30_pl.h"
#include "runtime/lib/bn_31_pl.h"
#include "runtime/lib/bn_34_pl.h"
extern void *rt_ws_alloc_descr(size_t);
extern void *rt_pl_ball_permission3(const char *, const char *, DESCR_t);
static int pl_getc_cp_raw(FILE *f, char *seq, int *seqn);
static void *pl_past_eos_ball(void);
static int pl_rd_eintr(FILE *f, pl_tr_ctx_t *cx);
extern FILE *fh_get(int);
extern unsigned rt_dtax_gen;
extern char * pl_tr_base_of(const char *);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int junction_mirror_op(int op) { switch (op) { case TT_LT: return TT_GT; case TT_GT: return TT_LT; case TT_LE: return TT_GE; case TT_GE: return TT_LE; default: return op; } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gram_set(const char *qname, const char *body, int flavor) {
    for (int i = 0; i < gram_n; i++) if (!strcmp(GRAM(i).qname, qname)) { GRAM(i).body = rt_heap_strdup_c(body); GRAM(i).flavor = flavor; return; }
    { gram_ent_t *e = &CV_PUSH(g_runtime.bn.gram, gram_ent_t); e->qname = rt_heap_strdup_c(qname); e->body = rt_heap_strdup_c(body); e->flavor = flavor; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_grammar_register(const char *qname, const char *body, int flavor) { if (qname && body) gram_set(qname, body, flavor); }
int rt_grammar_count(void) { return gram_n; }
const char *rt_grammar_qname(int i) { return (i >= 0 && i < gram_n) ? GRAM(i).qname : NULL; }
const char *rt_grammar_body(int i) { return (i >= 0 && i < gram_n) ? GRAM(i).body : NULL; }
int rt_grammar_flavor(int i) { return (i >= 0 && i < gram_n) ? GRAM(i).flavor : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pas_heap_release(long m) {
    if (m <= 0) return;
    for (long n = m; n <= g_runtime.pas.pas_heap_ctr && n < g_runtime.pas.pas_heap_cap; n++) { g_runtime.pas.pas_heap[n] = INTVAL(0); g_runtime.pas.pas_heap_dead[n] = 0; }
    if (m - 1 < g_runtime.pas.pas_heap_ctr) g_runtime.pas.pas_heap_ctr = m - 1;
    {
        long w = 0;
        for (long i = 0; i < g_runtime.pas.pas_heap_nfree; i++) if (g_runtime.pas.pas_heap_free[i] < m) g_runtime.pas.pas_heap_free[w++] = g_runtime.pas.pas_heap_free[i];
        g_runtime.pas.pas_heap_nfree = w;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pas_gc_roots(void) {
    extern void rt_gc_visit_descr(DESCR_t *);
    long top = (g_runtime.pas.pas_heap_ctr < g_runtime.pas.pas_heap_cap) ? g_runtime.pas.pas_heap_ctr : g_runtime.pas.pas_heap_cap - 1;
    for (long n = 1; n <= top; n++) rt_gc_visit_descr(&g_runtime.pas.pas_heap[n]);
    gv_gc_root(&g_runtime.pas.pas_tf);
}
int pl_alias_idx(const char *nm) {
    extern int fh_alias_idx(const char *);
    extern int fh_current_input(void);
    extern int fh_current_output(void);
    if (!strcmp(nm, "current_input")) return fh_current_input();
    if (!strcmp(nm, "current_output")) return fh_current_output();
    return fh_alias_idx(nm);
}
static int pl_fh_octet(int ix) { return ix >= 0 && ix < FH_N && g_fh[ix].enc && !strcmp(g_fh[ix].enc, "octet"); }
DESCR_t pl_mk_stream(int idx) {
    extern int prolog_atom_intern(const char *);
    DESCR_t *kids = (DESCR_t *)rt_ws_alloc_descr(1);
    kids[0] = INTVAL((long long)idx);
    DESCR_t c = {0};
    c.v = (DTYPE_t)DT_PLREF;
    c.slen = (uint32_t)prolog_functor_intern(prolog_atom_intern("$stream"), 1);
    c.p = (void *)kids;
    return c;
}
int pl_stream_resolve(DESCR_t s, int out, int textop, void **ball) {
    extern FILE *fh_get(int);
    extern int prolog_atom_intern(const char *);
    extern void *rt_pl_ball_instantiation(void);
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    extern void *rt_pl_ball_permission3(const char *, const char *, DESCR_t);
    DESCR_t d = rt_pl_deref_val(s);
    const char *nm;
    int idx = -1;
    *ball = (void *)0;
    if (pl_val_unbound(d)) { *ball = rt_pl_ball_instantiation(); return -1; }
    nm = pl_atom_str(d);
    if (nm) {
        if (!strcmp(nm, "user_input")) idx = 0;
        else if (!strcmp(nm, "user_output")) idx = 1;
        else if (!strcmp(nm, "user_error")) idx = 2;
        else idx = pl_alias_idx(nm);
        if (idx < 0 || !fh_get(idx)) { *ball = rt_pl_ball_kind2("existence_error", "stream", d); return -1; }
    } else if (d.v == (DTYPE_t)DT_PLREF && plc_fid_name(d.slen) == prolog_atom_intern("$stream_pair") && plc_fid_arity(d.slen) == 2) {
        return pl_stream_resolve(((DESCR_t *)d.p)[out > 0 ? 1 : 0], out, textop, ball);
    } else if (d.v == (DTYPE_t)DT_PLREF && plc_fid_name(d.slen) == prolog_atom_intern("$stream") && plc_fid_arity(d.slen) == 1) {
        DESCR_t a = rt_pl_deref_val(((DESCR_t *)d.p)[0]);
        if (a.v != DT_I || (int)a.i < 0 || (int)a.i >= FH_N || !fh_get((int)a.i)) { *ball = rt_pl_ball_kind2("existence_error", "stream", d); return -1; }
        idx = (int)a.i;
    } else {
        *ball = rt_pl_ball_kind2("domain_error", "stream_or_alias", d);
        return -1;
    }
    {
        int is_in = (g_fh[idx].mode == 'r');
        if (out > 0 && is_in) { *ball = rt_pl_ball_permission3("output", "stream", d); return -1; }
        if (out == 0 && !is_in && idx != 0) { *ball = rt_pl_ball_permission3("input", "stream", d); return -1; }
        if (out < 0) out = !is_in && idx != 0;
        if (textop > 0 && g_fh[idx].type == 'b') { *ball = rt_pl_ball_permission3(out ? "output" : "input", "binary_stream", d); return -1; }
        if (textop < 0 && g_fh[idx].type != 'b' && !pl_fh_octet(idx)) { *ball = rt_pl_ball_permission3(out ? "output" : "input", "text_stream", d); return -1; }
    }
    return idx;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
PL_CX_LEAF_HEAD(pipe, 2)
    {
        extern int fh_alloc(FILE *);
        extern void *rt_pl_ball_kind1(const char *, const char *);
        int fds[2];
        FILE * rf = NULL, * wf = NULL;
        ok = 0;
        if (pipe(fds) != 0) cx->ball = rt_pl_ball_kind1("resource_error", "open_files");
        else if (!(rf = fdopen(fds[0], "r"))) {
            close(fds[0]);
            close(fds[1]);
            cx->ball = rt_pl_ball_kind1("resource_error", "open_files");
        } else if (!(wf = fdopen(fds[1], "w"))) {
            fclose(rf);
            close(fds[1]);
            cx->ball = rt_pl_ball_kind1("resource_error", "open_files");
        } else {
            int ri = fh_alloc(rf), wi = fh_alloc(wf);
            g_fh[ri].mode = 'r';
            g_fh[wi].mode = 'w';
            ok = plw_unify_vals(args[0], pl_mk_stream(ri), cx) && plw_unify_vals(args[1], pl_mk_stream(wi), cx);
            if (!ok) { fclose(rf); fclose(wf); fh_free(ri); fh_free(wi); }
        }
    }
PL_CX_LEAF_TAIL
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
PL_CX_LEAF_HEAD(open_null_stream, 1)
    {
        extern int fh_alloc(FILE *);
        extern void *rt_pl_ball_kind1(const char *, const char *);
        FILE * fp = fopen("/dev/null", "w");
        ok = 0;
        if (!fp) cx->ball = rt_pl_ball_kind1("resource_error", "open_files");
        else { int idx = fh_alloc(fp); g_fh[idx].mode = 'w'; ok = plw_unify_vals(args[0], pl_mk_stream(idx), cx); if (!ok) { fclose(fp); fh_free(idx); } }
    }
PL_CX_LEAF_TAIL
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_in_is_text(void) {
    extern int fh_current_input(void);
    extern int fh_is_untranslated(int);
    int ix = fh_current_input();
    return ix >= 0 && ix < FH_N && g_fh[ix].type != 'b' && !fh_is_untranslated(ix);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_getc_mirror(FILE *f, const char *seq, int n) {
    extern int fh_current_input(void);
    int ix = fh_current_input();
    if (ix < 3 || ix >= FH_N || g_fh[ix].fp != f || n <= 0) return;
    for (int k = 0; k < g_fh[ix].nmir; k++) { FILE *m = g_fh[g_fh[ix].mir[k]].fp; if (m) fwrite(seq, 1, (size_t)n, m); }
}
static int pl_getc_cp(FILE *f, char *seq, int *seqn) {
    char sq[8];
    int sn = 0, c = pl_getc_cp_raw(f, seq ? seq : sq, &sn);
    if (seqn) *seqn = sn;
    if (c != EOF) pl_getc_mirror(f, seq ? seq : sq, sn);
    if (c >= 0x80 && sn == 1 && pl_swi_dialect() && pl_in_is_text()) { c = 0xFFFD; if (seq) memcpy(seq, "\xEF\xBF\xBD", 3); if (seqn) *seqn = 3; }
    return c;
}
static int pl_getc_cp_raw(FILE *f, char *seq, int *seqn) {
    int b0, n, i, cp;
    if (seqn) *seqn = 0;
    b0 = fgetc(f);
    if (b0 == EOF) return EOF;
    if (seq) seq[0] = (char)b0;
    if (seqn) *seqn = 1;
    if ((unsigned char)b0 < 0x80 || !pl_in_is_text()) return b0 & 0xFF;
    n = utf8_seqlen((unsigned char)b0);
    if (n < 2) return b0 & 0xFF;
    cp = b0 & (0xFF >> (n + 1));
    for (i = 1; i < n; i++) {
        int c = fgetc(f);
        if (c == EOF) { if (seqn) *seqn = 1; return b0 & 0xFF; }
        if ((c & 0xC0) != 0x80) { ungetc(c, f); if (seqn) *seqn = 1; return b0 & 0xFF; }
        if (seq) seq[i] = (char)c;
        if (seqn) *seqn = i + 1;
        cp = (cp << 6) | (c & 0x3F);
    }
    return cp;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_peekc_cp(FILE *f, char *seq, int *seqn) {
    long pos = ftell(f);
    int i, sn = 0, cp = pl_getc_cp(f, seq, &sn);
    if (seqn) *seqn = sn;
    if (cp == EOF) return EOF;
    if (pos >= 0) { if (fseek(f, pos, SEEK_SET) == 0) return cp; }
    for (i = sn - 1; i >= 0; i--) ungetc((unsigned char)seq[i], f);
    return cp;
}
PL_CX_LEAF_HEAD(get_char, 1)
    {
        extern FILE *fh_cur_in_fp(void);
        char sq[8];
        int sn = 0;
        int c, r = 0;
        if ((cx->ball = pl_past_eos_ball())) { ok = 0; rt_pl_tr_gc_sync(cx->tr); return FAILDESCR; }
        do c = pl_getc_cp(fh_cur_in_fp(), sq, &sn);
        while (c == EOF && (r = pl_rd_eintr(fh_cur_in_fp(), cx)) == 1);
        if (r == 2) ok = 0;
        else if (c == EOF) ok = plw_unify_vals(args[0], pl_mk_atom("end_of_file"), cx);
        else ok = plw_unify_vals(args[0], pl_mk_atom_dup(sq, (size_t)sn), cx);
    }
PL_CX_LEAF_TAIL
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
PL_CX_LEAF_HEAD(peek_char, 1)
    {
        extern FILE *fh_cur_in_fp(void);
        char sq[8];
        int sn = 0;
        int c, r = 0;
        if ((cx->ball = pl_past_eos_ball())) { ok = 0; rt_pl_tr_gc_sync(cx->tr); return FAILDESCR; }
        do c = pl_peekc_cp(fh_cur_in_fp(), sq, &sn);
        while (c == EOF && (r = pl_rd_eintr(fh_cur_in_fp(), cx)) == 1);
        if (r == 2) ok = 0;
        else if (c == EOF) ok = plw_unify_vals(args[0], pl_mk_atom("end_of_file"), cx);
        else ok = plw_unify_vals(args[0], pl_mk_atom_dup(sq, (size_t)sn), cx);
    }
PL_CX_LEAF_TAIL
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
PL_CX_LEAF_HEAD(get_code, 1)
    {
        extern FILE *fh_cur_in_fp(void);
        char sq[8];
        int sn = 0;
        int c, r = 0;
        if ((cx->ball = pl_past_eos_ball())) { ok = 0; rt_pl_tr_gc_sync(cx->tr); return FAILDESCR; }
        do c = pl_getc_cp(fh_cur_in_fp(), sq, &sn);
        while (c == EOF && (r = pl_rd_eintr(fh_cur_in_fp(), cx)) == 1);
        ok = r != 2 && plw_unify_vals(args[0], INTVAL(c == EOF ? -1LL : (long long)c), cx);
    }
PL_CX_LEAF_TAIL
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
PL_CX_LEAF_HEAD(peek_code, 1)
    {
        extern FILE *fh_cur_in_fp(void);
        char sq[8];
        int sn = 0;
        int c, r = 0;
        if ((cx->ball = pl_past_eos_ball())) { ok = 0; rt_pl_tr_gc_sync(cx->tr); return FAILDESCR; }
        do c = pl_peekc_cp(fh_cur_in_fp(), sq, &sn);
        while (c == EOF && (r = pl_rd_eintr(fh_cur_in_fp(), cx)) == 1);
        ok = r != 2 && plw_unify_vals(args[0], INTVAL(c == EOF ? -1LL : (long long)c), cx);
    }
PL_CX_LEAF_TAIL
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *pl_byte_cur_ball(int out) {
    extern int fh_current_output(void);
    extern int fh_current_input(void);
    extern void *rt_pl_ball_permission3(const char *, const char *, DESCR_t);
    int ix = out ? fh_current_output() : fh_current_input();
    DESCR_t who = !pl_gnu_dialect() ? pl_mk_stream(ix) : pl_mk_atom(out ? "current_output_stream" : "current_input_stream");
    return (ix >= 0 && ix < FH_N && g_fh[ix].type != 'b' && !pl_fh_octet(ix)) ? rt_pl_ball_permission3(out ? "output" : "input", "text_stream", who) : (void *)0;
}
static void *pl_past_eos_ball(void) {
    extern int fh_current_input(void);
    extern int fh_eof(int);
    extern FILE *fh_cur_in_fp(void);
    extern void *rt_pl_ball_permission3(const char *, const char *, DESCR_t);
    int ix = fh_current_input();
    FILE *fp = fh_cur_in_fp();
    DESCR_t who = !pl_gnu_dialect() ? pl_mk_stream(ix) : pl_mk_atom("current_input_stream");
    return (ix >= 3 && ix < FH_N && fp && feof(fp) && fh_eof(ix) == 'e') ? rt_pl_ball_permission3("input", "past_end_of_stream", who) : (void *)0;
}
void *pl_text_cur_ball(void) {
    extern int fh_current_input(void);
    extern void *rt_pl_ball_permission3(const char *, const char *, DESCR_t);
    int ix = fh_current_input();
    DESCR_t who = !pl_gnu_dialect() ? pl_mk_stream(ix) : pl_mk_atom("current_input_stream");
    return (ix >= 0 && ix < FH_N && g_fh[ix].type == 'b') ? rt_pl_ball_permission3("input", "binary_stream", who) : (void *)0;
}
PL_CX_LEAF_HEAD(get_byte, 1)
    {
        extern FILE *fh_cur_in_fp(void);
        int c, r = 0;
        if ((cx->ball = pl_byte_cur_ball(0)) || (cx->ball = pl_past_eos_ball())) { ok = 0; rt_pl_tr_gc_sync(cx->tr); return FAILDESCR; }
        do c = fgetc(fh_cur_in_fp());
        while (c == EOF && (r = pl_rd_eintr(fh_cur_in_fp(), cx)) == 1);
        ok = r != 2 && plw_unify_vals(args[0], INTVAL(c == EOF ? -1LL : (long long)(unsigned char)c), cx);
    }
PL_CX_LEAF_TAIL
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
PL_CX_LEAF_HEAD(peek_byte, 1)
    {
        extern FILE *fh_cur_in_fp(void);
        int c, r = 0;
        if ((cx->ball = pl_byte_cur_ball(0)) || (cx->ball = pl_past_eos_ball())) { ok = 0; rt_pl_tr_gc_sync(cx->tr); return FAILDESCR; }
        do c = fgetc(fh_cur_in_fp());
        while (c == EOF && (r = pl_rd_eintr(fh_cur_in_fp(), cx)) == 1);
        if (c != EOF) ungetc(c, fh_cur_in_fp());
        else clearerr(fh_cur_in_fp());
        ok = r != 2 && plw_unify_vals(args[0], INTVAL(c == EOF ? -1LL : (long long)(unsigned char)c), cx);
    }
PL_CX_LEAF_TAIL
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
PL_CX_LEAF_HEAD(unget_char, 1)
    {
        extern FILE *fh_cur_in_fp(void);
        extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
        extern void *rt_pl_ball_instantiation(void);
        DESCR_t v = rt_pl_deref_val(args[0]);
        const char *cs = pl_val_unbound(v) ? (const char *)0 : pl_atom_str(v);
        int n = cs && cs[0] && !pl_is_nil(v) ? utf8_seqlen((unsigned char)cs[0]) : 0;
        ok = 0;
        if ((cx->ball = pl_text_cur_ball())) ok = 0;
        else if (pl_val_unbound(v)) cx->ball = rt_pl_ball_instantiation();
        else if (!n || n != (int)strlen(cs)) cx->ball = rt_pl_ball_kind2("type_error", "character", v);
        else { for (int i = n - 1; i >= 0; i--) ungetc((unsigned char)cs[i], fh_cur_in_fp()); ok = 1; }
    }
PL_CX_LEAF_TAIL
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
PL_CX_LEAF_HEAD(unget_code, 1)
    {
        extern FILE *fh_cur_in_fp(void);
        extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
        extern void *rt_pl_ball_kind1(const char *, const char *);
        extern void *rt_pl_ball_instantiation(void);
        DESCR_t v = rt_pl_deref_val(args[0]);
        ok = 0;
        if ((cx->ball = pl_text_cur_ball())) ok = 0;
        else if (pl_val_unbound(v)) cx->ball = rt_pl_ball_instantiation();
        else if (v.v != (DTYPE_t)DT_I) cx->ball = rt_pl_ball_kind2("type_error", "integer", v);
        else if (v.i < 1 || v.i > 255) cx->ball = rt_pl_ball_kind1("representation_error", "character_code");
        else { ungetc((int)v.i, fh_cur_in_fp()); ok = 1; }
    }
PL_CX_LEAF_TAIL
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
PL_CX_LEAF_HEAD(unget_byte, 1)
    {
        extern FILE *fh_cur_in_fp(void);
        extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
        extern void *rt_pl_ball_instantiation(void);
        DESCR_t v = rt_pl_deref_val(args[0]);
        ok = 0;
        if ((cx->ball = pl_byte_cur_ball(0))) ok = 0;
        else if (pl_val_unbound(v)) cx->ball = rt_pl_ball_instantiation();
        else if (v.v != (DTYPE_t)DT_I || v.i < 0 || v.i > 255) cx->ball = rt_pl_ball_kind2("type_error", "byte", v);
        else { ungetc((int)v.i, fh_cur_in_fp()); ok = 1; }
    }
PL_CX_LEAF_TAIL
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_read_esc_body(FILE *in, char **bp, size_t *np, size_t *kp) {
    size_t k = *kp;
    int e = fgetc(in), d;
    if (e == EOF) { *kp = k; return 2; }
    if (!pl_rb_room(bp, np, k + 2)) return -1;
    (*bp)[k++] = (char)e;
    if (e >= '0' && e <= '7') {
        while ((d = fgetc(in)) != EOF && d >= '0' && d <= '7') { if (!pl_rb_room(bp, np, k + 2)) return -1; (*bp)[k++] = (char)d; }
    } else if (e == 'x' || e == 'X') {
        while ((d = fgetc(in)) != EOF && isxdigit((unsigned char)d)) { if (!pl_rb_room(bp, np, k + 2)) return -1; (*bp)[k++] = (char)d; }
    } else {
        *kp = k;
        return 0;
    }
    if (d == '\\') { if (!pl_rb_room(bp, np, k + 2)) return -1; (*bp)[k++] = (char)d; } else if (d != EOF) ungetc(d, in);
    *kp = k;
    return 0;
}
static int pl_tok_alpha(int c) { return c < 0x80 ? (isalnum(c) || c == '_') : iswalpha((wint_t)c); }
static int pl_tok_symbol(int c) { return c > 0 && c < 0x80 && strchr("#$&*+-./:<=>?@\\^~", c); }
static uint32_t pl_sha_rol(uint32_t x, int n) { return (x << n) | (x >> (32 - n)); }
static uint32_t pl_sha_ror(uint32_t x, int n) { return (x >> n) | (x << (32 - n)); }
static size_t pl_sha_pad(const unsigned char *m, size_t n, unsigned char *b, size_t nb) {
    uint64_t bits = (uint64_t)n * 8;
    memset(b, 0, nb);
    if (n) memcpy(b, m, n);
    b[n] = 0x80;
    for (int i = 0; i < 8; i++) b[nb - 1 - i] = (unsigned char)(bits >> (8 * i));
    return nb;
}
static uint32_t pl_sha_be32(const unsigned char *p) { return (uint32_t)p[0] << 24 | (uint32_t)p[1] << 16 | (uint32_t)p[2] << 8 | (uint32_t)p[3]; }
static void pl_sha1(const unsigned char *m, size_t n, unsigned char *out) {
    uint32_t h[5] = { 0x67452301u, 0xEFCDAB89u, 0x98BADCFEu, 0x10325476u, 0xC3D2E1F0u };
    size_t nb = ((n + 8) / 64 + 1) * 64;
    unsigned char b[nb];
    pl_sha_pad(m, n, b, nb);
    for (size_t o = 0; o < nb; o += 64) {
        uint32_t w[80], a = h[0], c1 = h[1], c2 = h[2], c3 = h[3], c4 = h[4], f, k, t;
        for (int i = 0; i < 16; i++) w[i] = pl_sha_be32(b + o + 4 * i);
        for (int i = 16; i < 80; i++) w[i] = pl_sha_rol(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
        for (int i = 0; i < 80; i++) {
            if (i < 20) {
                f = (c1 & c2) | (~c1 & c3);
                k = 0x5A827999u;
            } else if (i < 40) {
                f = c1 ^ c2 ^ c3;
                k = 0x6ED9EBA1u;
            } else if (i < 60) {
                f = (c1 & c2) | (c1 & c3) | (c2 & c3);
                k = 0x8F1BBCDCu;
            } else {
                f = c1 ^ c2 ^ c3;
                k = 0xCA62C1D6u;
            }
            t = pl_sha_rol(a, 5) + f + c4 + k + w[i];
            c4 = c3;
            c3 = c2;
            c2 = pl_sha_rol(c1, 30);
            c1 = a;
            a = t;
        }
        h[0] += a;
        h[1] += c1;
        h[2] += c2;
        h[3] += c3;
        h[4] += c4;
    }
    for (int i = 0; i < 20; i++) out[i] = (unsigned char)(h[i / 4] >> (24 - 8 * (i % 4)));
}
static void pl_sha256(const unsigned char *m, size_t n, unsigned char *out) {
    static const uint32_t k[64] = { 0x428a2f98u, 0x71374491u, 0xb5c0fbcfu, 0xe9b5dba5u, 0x3956c25bu, 0x59f111f1u, 0x923f82a4u, 0xab1c5ed5u, 0xd807aa98u, 0x12835b01u, 0x243185beu, 0x550c7dc3u,
        0x72be5d74u, 0x80deb1feu, 0x9bdc06a7u, 0xc19bf174u, 0xe49b69c1u, 0xefbe4786u, 0x0fc19dc6u, 0x240ca1ccu, 0x2de92c6fu, 0x4a7484aau, 0x5cb0a9dcu, 0x76f988dau, 0x983e5152u, 0xa831c66du,
        0xb00327c8u, 0xbf597fc7u, 0xc6e00bf3u, 0xd5a79147u, 0x06ca6351u, 0x14292967u, 0x27b70a85u, 0x2e1b2138u, 0x4d2c6dfcu, 0x53380d13u, 0x650a7354u, 0x766a0abbu, 0x81c2c92eu, 0x92722c85u,
        0xa2bfe8a1u, 0xa81a664bu, 0xc24b8b70u, 0xc76c51a3u, 0xd192e819u, 0xd6990624u, 0xf40e3585u, 0x106aa070u, 0x19a4c116u, 0x1e376c08u, 0x2748774cu, 0x34b0bcb5u, 0x391c0cb3u, 0x4ed8aa4au,
        0x5b9cca4fu, 0x682e6ff3u, 0x748f82eeu, 0x78a5636fu, 0x84c87814u, 0x8cc70208u, 0x90befffau, 0xa4506cebu, 0xbef9a3f7u, 0xc67178f2u };
    uint32_t h[8] = { 0x6a09e667u, 0xbb67ae85u, 0x3c6ef372u, 0xa54ff53au, 0x510e527fu, 0x9b05688cu, 0x1f83d9abu, 0x5be0cd19u };
    size_t nb = ((n + 8) / 64 + 1) * 64;
    unsigned char b[nb];
    pl_sha_pad(m, n, b, nb);
    for (size_t o = 0; o < nb; o += 64) {
        uint32_t w[64], v[8], t1, t2;
        for (int i = 0; i < 16; i++) w[i] = pl_sha_be32(b + o + 4 * i);
        for (int i = 16; i < 64; i++) {
            uint32_t s0 = pl_sha_ror(w[i - 15], 7) ^ pl_sha_ror(w[i - 15], 18) ^ (w[i - 15] >> 3), s1 = pl_sha_ror(w[i - 2], 17) ^ pl_sha_ror(w[i - 2], 19) ^ (w[i - 2] >> 10);
            w[i] = w[i - 16] + s0 + w[i - 7] + s1;
        }
        for (int i = 0; i < 8; i++) v[i] = h[i];
        for (int i = 0; i < 64; i++) {
            t1 = v[7] + (pl_sha_ror(v[4], 6) ^ pl_sha_ror(v[4], 11) ^ pl_sha_ror(v[4], 25)) + ((v[4] & v[5]) ^ (~v[4] & v[6])) + k[i] + w[i];
            t2 = (pl_sha_ror(v[0], 2) ^ pl_sha_ror(v[0], 13) ^ pl_sha_ror(v[0], 22)) + ((v[0] & v[1]) ^ (v[0] & v[2]) ^ (v[1] & v[2]));
            for (int j = 7; j > 0; j--) v[j] = v[j - 1];
            v[4] += t1;
            v[0] = t1 + t2;
        }
        for (int i = 0; i < 8; i++) h[i] += v[i];
    }
    for (int i = 0; i < 32; i++) out[i] = (unsigned char)(h[i / 4] >> (24 - 8 * (i % 4)));
}
PL_CX_LEAF_HEAD(sha_hash, 3)
    {
        extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
        DESCR_t l = rt_pl_deref_val(args[0]), al = rt_pl_deref_val(args[1]);
        const char *an = pl_atom_str(al);
        size_t n = 0;
        ok = 0;
        for (DESCR_t q = l; pl_is_cons(q); q = rt_pl_deref_val(((DESCR_t *)q.p)[1])) n++;
        {
            unsigned char m[n ? n : 1], h[32];
            size_t i = 0;
            int hl = 0;
            for (DESCR_t q = l; pl_is_cons(q); q = rt_pl_deref_val(((DESCR_t *)q.p)[1])) { DESCR_t e = rt_pl_deref_val(((DESCR_t *)q.p)[0]); m[i++] = (unsigned char)(e.v == DT_I ? e.i : 0); }
            if (an && !strcmp(an, "sha1")) {
                pl_sha1(m, n, h);
                hl = 20;
            } else if (an && !strcmp(an, "sha256")) {
                pl_sha256(m, n, h);
                hl = 32;
            } else cx->ball = rt_pl_ball_kind2("domain_error", "algorithm", al);
            if (hl) { DESCR_t el[hl]; for (int j = 0; j < hl; j++) el[j] = INTVAL(h[j]); ok = plw_unify_vals(args[2], pl_list_from_arr(el, hl), cx); }
        }
    }
PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(put_token, 2)
    {
        extern int fh_last_char(int);
        extern FILE *fh_get(int);
        extern void *rt_pl_ball_instantiation(void);
        extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
        void *b = (void *)0;
        int idx = pl_stream_resolve(args[0], 1, 0, &b);
        DESCR_t t = rt_pl_deref_val(args[1]);
        const char *tk = pl_atom_str(t);
        ok = 0;
        if (b) cx->ball = b;
        else if (pl_val_unbound(t)) cx->ball = rt_pl_ball_instantiation();
        else if (!tk) cx->ball = rt_pl_ball_kind2("type_error", "atom", t);
        else {
            FILE *fp = fh_get(idx);
            int lc = tk[0] ? fh_last_char(idx) : -1, c = (unsigned char)tk[0];
            if (lc > 0 && ((pl_tok_alpha(lc) && pl_tok_alpha(c)) || (pl_tok_symbol(lc) && pl_tok_symbol(c)) || (c == '(' && lc != '(' && !isspace(lc)))) fputc(' ', fp);
            fputs(tk, fp);
            ok = 1;
        }
    }
PL_CX_LEAF_TAIL
DESCR_t pl_gs_arg(DESCR_t t, int i, int n) { DESCR_t z = FAILDESCR; if (t.v != (DTYPE_t)DT_PLREF || plc_fid_arity(t.slen) != n) return z; return rt_pl_deref_val(((DESCR_t *)t.p)[i]); }
DESCR_t pl_gs_fdstream(int fd, const char *m) { extern int fh_alloc(FILE *); int idx = fh_alloc(fdopen(fd, m)); g_fh[idx].mode = m[0]; return pl_mk_stream(idx); }
int pl_gs_unix_addr(struct sockaddr_un *u, const char *p) {
    memset(u, 0, sizeof *u);
    u->sun_family = AF_UNIX;
    if (!p || strlen(p) >= sizeof u->sun_path) { errno = ENAMETOOLONG; return -1; }
    memcpy(u->sun_path, p, strlen(p) + 1);
    return 0;
}
static int pl_gs_inet_addr(struct sockaddr_in *in, const char *host, long port) {
    struct addrinfo hints, *res = (struct addrinfo *)0;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    if (!host || getaddrinfo(host, (const char *)0, &hints, &res) != 0 || !res) return -1;
    memcpy(in, res->ai_addr, sizeof *in);
    freeaddrinfo(res);
    in->sin_port = htons((unsigned short)port);
    return 0;
}
int pl_gs_sock(const char *op, DESCR_t a, DESCR_t res, pl_tr_ctx_t *cx) {
    if (!strcmp(op, "socket")) {
        const char *d = pl_atom_str(a);
        int fd = socket(d && !strcmp(d, "AF_UNIX") ? AF_UNIX : AF_INET, SOCK_STREAM, 0), one = 1;
        if (fd < 0 || setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof one) != 0) return pl_gnu_os_fail(cx);
        return plw_unify_vals(res, INTVAL(fd), cx);
    }
    if (!strcmp(op, "sclose")) { if (a.v != (DTYPE_t)DT_I || a.i < 2) { errno = EBADF; return pl_gnu_os_fail(cx); } return close((int)a.i) == 0 ? 1 : pl_gnu_os_fail(cx); }
    if (!strcmp(op, "sbind_unix")) {
        DESCR_t fd = pl_gs_arg(a, 0, 2);
        const char *p = pl_atom_str(pl_gs_arg(a, 1, 2));
        struct sockaddr_un u;
        if (pl_gs_unix_addr(&u, p) != 0) return pl_gnu_os_fail(cx);
        unlink(p);
        return bind((int)fd.i, (struct sockaddr *)&u, sizeof u) == 0 ? 1 : pl_gnu_os_fail(cx);
    }
    if (!strcmp(op, "sbind_inet")) {
        DESCR_t fd = pl_gs_arg(a, 0, 2), pt = pl_gs_arg(a, 1, 2);
        struct sockaddr_in in;
        socklen_t l = sizeof in;
        memset(&in, 0, sizeof in);
        in.sin_family = AF_INET;
        in.sin_addr.s_addr = htonl(INADDR_ANY);
        in.sin_port = htons((unsigned short)pt.i);
        if (bind((int)fd.i, (struct sockaddr *)&in, sizeof in) != 0 || getsockname((int)fd.i, (struct sockaddr *)&in, &l) != 0) return pl_gnu_os_fail(cx);
        return plw_unify_vals(res, INTVAL(ntohs(in.sin_port)), cx);
    }
    if (!strcmp(op, "slisten")) { DESCR_t fd = pl_gs_arg(a, 0, 2), n = pl_gs_arg(a, 1, 2); return listen((int)fd.i, (int)n.i) == 0 ? 1 : pl_gnu_os_fail(cx); }
    if (!strcmp(op, "saccept")) {
        struct sockaddr_storage ss;
        socklen_t l = sizeof ss;
        int c = accept((int)a.i, (struct sockaddr *)&ss, &l), c2;
        const char *who = "";
        char hb[INET_ADDRSTRLEN];
        DESCR_t v[3];
        if (c < 0 || (c2 = dup(c)) < 0) return pl_gnu_os_fail(cx);
        if (ss.ss_family == AF_INET && inet_ntop(AF_INET, &((struct sockaddr_in *)&ss)->sin_addr, hb, sizeof hb)) who = hb;
        v[0] = pl_gs_fdstream(c, "r");
        v[1] = pl_gs_fdstream(c2, "w");
        v[2] = pl_mk_atom_dup(who, strlen(who));
        return plw_unify_vals(res, pl_gnu_fn("acc", 3, v), cx);
    }
    if (!strcmp(op, "sconn_unix") || !strcmp(op, "sconn_inet")) {
        int unx = op[6] == 'u', c2;
        DESCR_t fd = pl_gs_arg(a, 0, unx ? 2 : 3), v[2];
        struct sockaddr_un u;
        struct sockaddr_in in;
        int rc = unx ? pl_gs_unix_addr(&u, pl_atom_str(pl_gs_arg(a, 1, 2))) : pl_gs_inet_addr(&in, pl_atom_str(pl_gs_arg(a, 1, 3)), (long)pl_gs_arg(a, 2, 3).i);
        if (rc != 0) return unx ? pl_gnu_os_fail(cx) : 0;
        if (connect((int)fd.i, unx ? (struct sockaddr *)&u : (struct sockaddr *)&in, unx ? sizeof u : sizeof in) != 0 || (c2 = dup((int)fd.i)) < 0) return pl_gnu_os_fail(cx);
        v[0] = pl_gs_fdstream((int)fd.i, "r");
        v[1] = pl_gs_fdstream(c2, "w");
        return plw_unify_vals(res, pl_gnu_fn("st", 2, v), cx);
    }
    if (!strcmp(op, "hostaddr")) {
        struct sockaddr_in in;
        char hb[INET_ADDRSTRLEN];
        if (pl_gs_inet_addr(&in, pl_atom_str(a), 0) != 0 || !inet_ntop(AF_INET, &in.sin_addr, hb, sizeof hb)) return 0;
        return plw_unify_vals(res, pl_mk_atom_dup(hb, strlen(hb)), cx);
    }
    return 0;
}
static void pl_fx_canon(char *p) {
    size_t n = strlen(p), ns = 0, w = 0;
    int ab = p[0] == '/';
    char *sg[n + 1];
    char o[n + 2];
    for (char *q = p; *q;) {
        while (*q == '/') q++;
        if (!*q) break;
        char *t = q;
        while (*q && *q != '/') q++;
        if (*q) *q++ = 0;
        if (!strcmp(t, ".")) continue;
        if (!strcmp(t, "..") && ns > 0 && strcmp(sg[ns - 1], "..")) { ns--; continue; }
        if (!strcmp(t, "..") && ab && ns == 0) continue;
        sg[ns++] = t;
    }
    if (ab) o[w++] = '/';
    for (size_t i = 0; i < ns; i++) { size_t l = strlen(sg[i]); if (i) o[w++] = '/'; memcpy(o + w, sg[i], l); w += l; }
    if (!w) o[w++] = '.';
    o[w] = 0;
    memcpy(p, o, w + 1);
}
static DESCR_t pl_fx_target(const char *link, int n) {
    struct stat ls;
    if (lstat(link, &ls) != 0 || !S_ISLNK(ls.st_mode)) return pl_mk_atom(link);
    if (n <= 0) return FAILDESCR;
    size_t cn = (size_t)ls.st_size + 1, ln = strlen(link);
    char c[cn + 1];
    ssize_t k = readlink(link, c, cn);
    if (k < 0 || (size_t)k >= cn) return FAILDESCR;
    c[k] = 0;
    char nx[ln + (size_t)k + 2];
    if (c[0] == '/') memcpy(nx, c, (size_t)k + 1);
    else { const char *sl = strrchr(link, '/'); size_t d = sl ? (size_t)(sl - link) : 0; memcpy(nx, link, d); if (sl) nx[d++] = '/'; memcpy(nx + d, c, (size_t)k + 1); }
    pl_fx_canon(nx);
    return pl_fx_target(nx, n - 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_alarm_take(pl_tr_ctx_t *cx) {
    extern void *rt_pl_ball_make(DESCR_t *, int);
    volatile int32_t *d = (volatile int32_t *)(pl_tr_base_of(cx->tr) + PL_TR_ALARM_DUE_OFF);
    DESCR_t a;
    if (!*d) return 0;
    *d = 0;
    if (*(int64_t *)(pl_tr_base_of(cx->tr) + PL_TR_ALARM_DEADLINE_OFF) > 0) *(int64_t *)(pl_tr_base_of(cx->tr) + PL_TR_ALARM_DEADLINE_OFF) *= -1;
    a = pl_mk_atom("time_limit_exceeded");
    cx->ball = rt_pl_ball_make(&a, 1);
    return 1;
}
static int pl_rd_eintr(FILE *f, pl_tr_ctx_t *cx) { if (!f || !ferror(f) || errno != EINTR) return 0; clearerr(f); return pl_alarm_take(cx) ? 2 : 1; }
static int64_t pl_mono_us(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return (int64_t)t.tv_sec * 1000000 + (int64_t)t.tv_nsec / 1000; }
static void pl_alarm_on(int sig, siginfo_t *si, void *uc) { (void)sig; (void)uc; if (si && si->si_value.sival_ptr) *(volatile int32_t *)((char *)si->si_value.sival_ptr + PL_TR_ALARM_DUE_OFF) = 1; }
static int pl_alarm_arm(char *h, int64_t us) {
    struct itimerspec it;
    memset(&it, 0, sizeof it);
    if (!*(int64_t *)(h + PL_TR_ALARM_MADE_OFF)) {
        struct sigaction sa, old;
        struct sigevent ev;
        if (us <= 0) return 0;
        if (sigaction(SIGALRM, (struct sigaction *)0, &old) != 0) return -1;
        if (!(old.sa_flags & SA_SIGINFO) || old.sa_sigaction != pl_alarm_on) {
            memset(&sa, 0, sizeof sa);
            sa.sa_sigaction = pl_alarm_on;
            sa.sa_flags = SA_SIGINFO;
            sigemptyset(&sa.sa_mask);
            if (sigaction(SIGALRM, &sa, (struct sigaction *)0) != 0) return -1;
        }
        memset(&ev, 0, sizeof ev);
        ev.sigev_notify = SIGEV_THREAD_ID;
        ev.sigev_signo = SIGALRM;
        ev.sigev_value.sival_ptr = h;
        ev._sigev_un._tid = (pid_t)syscall(SYS_gettid);
        if (timer_create(CLOCK_MONOTONIC, &ev, (timer_t *)(h + PL_TR_ALARM_TIMER_OFF)) != 0) return -1;
        *(int64_t *)(h + PL_TR_ALARM_MADE_OFF) = 1;
    }
    if (us > 0) { it.it_value.tv_sec = (time_t)(us / 1000000); it.it_value.tv_nsec = (long)(us % 1000000) * 1000; }
    return timer_settime(*(timer_t *)(h + PL_TR_ALARM_TIMER_OFF), 0, &it, (struct itimerspec *)0);
}
PL_CX_LEAF_HEAD(alarm_push, 2)
    {
        char *h = pl_tr_base_of(cx->tr);
        DESCR_t t = rt_pl_deref_val(args[0]);
        double sec = t.v == (DTYPE_t)DT_I ? (double)t.i : t.v == (DTYPE_t)DT_R ? t.r : 0.0;
        int64_t now = pl_mono_us(), prev = *(int64_t *)(h + PL_TR_ALARM_DEADLINE_OFF), nw = now + (int64_t)(sec * 1e6), d = (prev > 0 && prev < nw) ? prev : nw;
        *(int64_t *)(h + PL_TR_ALARM_DEADLINE_OFF) = d;
        if (d - now <= 0) *(volatile int32_t *)(h + PL_TR_ALARM_DUE_OFF) = 1;
        else pl_alarm_arm(h, d - now);
        ok = plw_unify_vals(args[1], INTVAL((long long)prev), cx);
    }
PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(alarm_pop, 1)
    {
        char *h = pl_tr_base_of(cx->tr);
        DESCR_t p = rt_pl_deref_val(args[0]);
        int64_t prev = p.v == (DTYPE_t)DT_I ? (int64_t)p.i : 0, now = pl_mono_us(), cur = *(int64_t *)(h + PL_TR_ALARM_DEADLINE_OFF);
        int fired = prev < 0 || (cur < 0 && -cur == prev);
        *(int64_t *)(h + PL_TR_ALARM_DEADLINE_OFF) = (fired && prev > 0) ? -prev : prev;
        if (!prev || fired) {
            pl_alarm_arm(h, 0);
            *(volatile int32_t *)(h + PL_TR_ALARM_DUE_OFF) = 0;
        } else if (prev <= now) {
            pl_alarm_arm(h, 0);
            *(volatile int32_t *)(h + PL_TR_ALARM_DUE_OFF) = 1;
        } else {
            *(volatile int32_t *)(h + PL_TR_ALARM_DUE_OFF) = 0;
            pl_alarm_arm(h, prev - now);
        }
        ok = 1;
    }
PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(os_sleep, 1)
    {
        DESCR_t a = rt_pl_deref_val(args[0]);
        double sec = a.v == (DTYPE_t)DT_I ? (double)a.i : a.v == (DTYPE_t)DT_R ? a.r : -1.0;
        struct timespec ts, rem;
        ok = sec >= 0;
        ts.tv_sec = (time_t)(sec > 0 ? sec : 0);
        ts.tv_nsec = (long)((sec - (double)ts.tv_sec) * 1e9);
        if (ts.tv_nsec < 0) ts.tv_nsec = 0;
        while (ok && nanosleep(&ts, &rem) != 0) { if (errno != EINTR || pl_alarm_take(cx)) ok = 0; ts = rem; }
    }
PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(proc_wait, 3)
    {
        DESCR_t p = rt_pl_deref_val(args[0]), t = rt_pl_deref_val(args[1]);
        double sec = t.v == (DTYPE_t)DT_I ? (double)t.i : t.v == (DTYPE_t)DT_R ? t.r : -1.0;
        int64_t end = sec >= 0 ? pl_mono_us() + (int64_t)(sec * 1e6) : 0;
        int ws = 0, up = 0;
        pid_t got = -1;
        struct timespec ts = { 0, 10000000 };
        DESCR_t v[1];
        for (;;) {
            got = waitpid((pid_t)p.i, &ws, sec >= 0 ? WNOHANG : 0);
            if (got < 0 && errno == EINTR) { if (pl_alarm_take(cx)) { up = 1; break; } continue; }
            if (got != 0 || pl_mono_us() >= end) break;
            if (nanosleep(&ts, (struct timespec *)0) != 0 && errno == EINTR && pl_alarm_take(cx)) { up = 1; break; }
        }
        if (up) ok = 0;
        else if (got < 0) ok = pl_os_errv(args[2], cx);
        else if (got == 0) ok = plw_unify_vals(args[2], pl_mk_atom("timeout"), cx);
        else if (WIFEXITED(ws)) {
            v[0] = INTVAL(WEXITSTATUS(ws));
            ok = plw_unify_vals(args[2], pl_gnu_fn("exit", 1, v), cx);
        } else {
            v[0] = INTVAL(WIFSIGNALED(ws) ? WTERMSIG(ws) : 0);
            ok = plw_unify_vals(args[2], pl_gnu_fn("killed", 1, v), cx);
        }
    }
PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(fx_readlink, 2)
    {
        const char *p = pl_atom_str(rt_pl_deref_val(args[0]));
        struct stat ls;
        ok = 0;
        if (p && lstat(p, &ls) == 0 && S_ISLNK(ls.st_mode)) {
            size_t cn = (size_t)ls.st_size + 1;
            char c[cn + 1];
            ssize_t k = readlink(p, c, cn);
            DESCR_t t = (k >= 0 && (size_t)k < cn) ? pl_fx_target(p, 20) : FAILDESCR;
            if (!IS_FAIL_fn(t)) { c[k] = 0; DESCR_t lt[2] = { pl_mk_atom(c), t }; ok = plw_unify_vals(args[1], pl_gnu_fn("l", 2, lt), cx); }
        }
    }
PL_CX_LEAF_TAIL
DESCR_t bt_grammar_register(DESCR_t *args, int nargs) {
    if (nargs == 3) {
        const char *qn = VARVAL_fn(args[0]);
        const char *body = VARVAL_fn(args[1]);
        const char *flv = VARVAL_fn(args[2]);
        if (qn && body) gram_set(qn, body, flv ? atoi(flv) : 0);
        return INTVAL(1);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
int bn_array(DESCR_t *args, int nargs, DESCR_t *out, int op) {
    extern DESCR_t sno_array_d(DESCR_t a, DESCR_t init);
    (void)op;
    if (nargs < 1) return 0;
    *out = sno_array_d(args[0], nargs >= 2 ? args[1] : NULVCL);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_dat_field_of_any_live(const char *name) {
    extern int dat_type_count(void);
    extern int dat_type_nfields(int);
    extern const char *dat_type_field(int, int);
    extern int dat_type_live(int);
    if (!name || !name[0]) return 0;
    for (int c = 0; c < dat_type_count(); c++) {
        if (!dat_type_live(c)) continue;
        for (int f = 0; f < dat_type_nfields(c); f++) { const char *fn2 = dat_type_field(c, f); if (fn2 && !strcmp(fn2, name)) return 1; }
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_builtin_synonym_add(const char *newname, const char *oldname) {
    if (!newname || !oldname) return;
    rt_dtax_gen++;
    for (int i = 0; i < g_rt_syn_n; i++) if (!strcmp(SYN(i).newname, newname)) { SYN(i).oldname = oldname; return; }
    { rt_syn_t *e = &CV_PUSH(g_runtime.bn.rt_syn, rt_syn_t); e->newname = newname; e->oldname = oldname; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rt_builtin_synonym(const char *name) {
    if (!name) return (const char *)0;
    for (int i = 0; i < g_rt_syn_n; i++) if (!strcmp(SYN(i).newname, name)) return SYN(i).oldname;
    return (const char *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_array_bound(const char *p, size_t m, int64_t *v) {
    extern int rt_num_arg_d(const DESCR_t *, int64_t *);
    char b[m + 1];
    memcpy(b, p, m);
    b[m] = 0;
    DESCR_t s = m ? (DESCR_t){ .v = DT_S, .slen = (uint32_t)m, .s = b } : NULVCL;
    return rt_num_arg_d(&s, v);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t sno_array_nest(const int64_t *lb, const int64_t *dm, int nd, int k, DESCR_t init) {
    extern ARBLK_t *array_new(int lo, int hi);
    extern void rt_sno_dumpno_undo(void);
    ARBLK_t *a = array_new((int)lb[k], (int)(lb[k] + dm[k] - 1));
    if (!a) return FAILDESCR;
    if (k > 0) { a->dumpno = 0; rt_sno_dumpno_undo(); }
    for (int64_t j = 0; j < dm[k]; j++) a->data[j] = k + 1 < nd ? sno_array_nest(lb, dm, nd, k + 1, init) : init;
    DESCR_t d;
    memset(&d, 0, sizeof d);
    d.v = DT_A;
    d.slen = 0;
    d.arr = a;
    return d;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t sno_array_from_proto(const char *proto, DESCR_t init) {
    int nd = 1;
    for (const char *q = proto; *q; q++) nd += *q == ',';
    int64_t lb[nd], dm[nd], nel = 1;
    const char *p = proto;
    for (int k = 0; k < nd; k++) {
        size_t m = strcspn(p, ":,");
        int64_t lo = 1, hi = 0;
        if (p[m] == ':') { if (!sno_array_bound(p, m, &lo)) { core_runtime_error(65, "array first argument lower bound is not integer"); return FAILDESCR; } p += m + 1; m = strcspn(p, ","); }
        if (!sno_array_bound(p, m, &hi)) { core_runtime_error(66, "array first argument upper bound is not integer"); return FAILDESCR; }
        if (__builtin_sub_overflow(hi, lo, &dm[k]) || dm[k] < 0 || __builtin_add_overflow(dm[k], 1, &dm[k])) {
            core_runtime_error(67, "array dimension is zero, negative or out of range");
            return FAILDESCR;
        }
        if (lo < INT_MIN || hi > INT_MAX || __builtin_mul_overflow(nel, dm[k], &nel) || nel > INT_MAX) { core_runtime_error(68, "array size exceeds maximum permitted"); return FAILDESCR; }
        lb[k] = lo;
        p += m + (p[m] == ',');
    }
    DESCR_t r = sno_array_nest(lb, dm, nd, 0, init);
    if (r.v == DT_A && r.arr) ((ARBLK_t *)r.arr)->proto = rt_heap_strdup_c(proto);
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t sno_array_d(DESCR_t a, DESCR_t init) {
    extern int rt_num_arg_d(const DESCR_t *, int64_t *);
    extern ARBLK_t *array_new(int lo, int hi);
    int64_t n = 0;
    if (rt_num_arg_d(&a, &n)) {
        if (n < 1) { core_runtime_error(67, "array dimension is zero, negative or out of range"); return FAILDESCR; }
        if (n > INT_MAX) { core_runtime_error(68, "array size exceeds maximum permitted"); return FAILDESCR; }
        ARBLK_t *b = array_new(1, (int)n);
        if (!b) return FAILDESCR;
        if (!(init.v == DT_SNUL && init.slen == 0)) for (int64_t k = 0; k < n; k++) b->data[k] = init;
        DESCR_t d;
        memset(&d, 0, sizeof d);
        d.v = DT_A;
        d.slen = 0;
        d.arr = b;
        return d;
    }
    if (a.v != DT_S) { core_runtime_error(64, "array first argument is not integer or string"); return FAILDESCR; }
    return sno_array_from_proto(rt_cstr_d(a), init);
}
