#include "runtime/lib/bn_head.h"
#include <sys/utsname.h>
#include "ir/rk_opname.h"
#include "runtime/core/nv.h"
#include <math.h>
#include "runtime/rt/rt_arena.h"
#include "runtime/core/runtime_shim.h"
#include "driver/driver_globals.h"
#include "driver/driver_data.h"
#include "runtime/rx.h"
#include "runtime/core/utf8.h"
#include "runtime/builtin_leaf.h"
#include <ctype.h>
#include <fcntl.h>
#include "runtime/lib/bn_03_sh.h"
#include "runtime/lib/bn_05_pl.h"
#include "runtime/lib/bn_07_rk.h"
#include "runtime/lib/bn_08_sh.h"
#include "runtime/lib/bn_09_rk.h"
#include "runtime/lib/bn_12_rk.h"
#include "runtime/lib/bn_13_rk.h"
#include "runtime/lib/bn_14_rk.h"
#include "runtime/lib/bn_15_rk.h"
#include "runtime/lib/bn_16_rk.h"
#include "runtime/lib/bn_19_rk.h"
#include "runtime/lib/bn_20_pl.h"
#include "runtime/lib/bn_21_sh.h"
#include "runtime/lib/bn_23_rk.h"
#include "runtime/lib/bn_31_pl.h"
extern gv_t g_call_args;
void rt_call_args_need(int n);
extern void *rt_ws_alloc_descr(size_t);
static DESCR_t rk_rx_sub(const Match *m, const char *subj, DESCR_t orig, int lo, int hi, int from, int to);
static void rk_apply_actions(DESCR_t m, DESCR_t actions, DESCR_t key);
static int rk_strmeth_open(DESCR_t d, int use_gist, long *rq, DESCR_t *out);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int gram_get_flavor(const char *qname) { for (int i = 0; i < gram_n; i++) if (!strcmp(GRAM(i).qname, qname)) return GRAM(i).flavor; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *gram_get(const char *qname) { for (int i = 0; i < gram_n; i++) if (!strcmp(GRAM(i).qname, qname)) return GRAM(i).body; return NULL; }
int rt_grammar_has_top(const char *gname) { if (!gname) return 0; char qn[fmt_len("%s::TOP", gname)]; snprintf(qn, sizeof qn, "%s::TOP", gname); return gram_get(qn) != NULL; }
int rk_match_is_nil(DESCR_t d) { if (!(IS_DATA_INST_fn(d) && d.u && d.u->type && d.u->type->name && !strcmp(d.u->type->name, "Match"))) return 0; return FIELD_GET_fn(d, "ok").i == 0; }
int rk_is_truthy(DESCR_t v) {
    extern int rt_is_truthy(DESCR_t);
    if (rk_match_is_nil(v)) return 0;
    if (rk_failure_is(v)) { rk_failure_handle(v); return 0; }
    if ((v.v == DT_A && v.arr) || (v.v == DT_T && v.tbl)) return rk_agg_truth(v);
    if (rk_match_is(v)) return 1;
    if (rk_qh_is(v)) return rk_qh_n(v) > 0;
    if (rk_typeobj_name(v)) return 0;
    if (v.v == DT_S && v.s && v.slen != 0xFFFFFFFFu && !rk_typeobj_name(v)) return v.s[0] != '\0';
    if (v.v == DT_ORDER) return (v.i != 0);
    if (v.v == DT_DATA) { extern int rk_rat_is(DESCR_t); extern int rk_rat_truthy(DESCR_t); if (rk_rat_is(v)) return rk_rat_truthy(v); }
    if (v.v == DT_DATA || v.v == DT_FH) return 1;
    return rt_is_truthy(v);
}
static int rk_cp_off(const char *s, int boff) { int n = 0; for (int i = 0; i < boff && s[i]; i++) if (((unsigned char) s[i] & 0xC0) != 0x80) n++; return n; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_match_rec(DESCR_t orig, int from, int to, int ok, DESCR_t text, DESCR_t pos, DESCR_t named) {
    static int rk_match_reg = 0;
    if (!rk_match_reg) { DEFDAT_fn("Match(orig,from,to,ok,text,positional,named,made)"); rk_match_reg = 1; }
    return DATCON_fn("Match", orig, INTVAL(from), INTVAL(to), INTVAL(ok), text, pos, named, NULVCL);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_match_nil(void) { return rk_match_rec(STRVAL(rt_heap_strdup_c("")), 0, 0, 0, STRVAL(rt_heap_strdup_c("")), rk_mk_arr(NULL, 0), TABLE_VAL(table_new())); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_match_span(DESCR_t orig, const char *subj, int bs, int be, DESCR_t pos, DESCR_t named) {
    int len = be > bs ? be - bs : 0;
    char *t = rt_wsb_alloc((size_t) len + 1);
    if (len) memcpy(t, subj + bs, (size_t) len);
    t[len] = '\0';
    return rk_match_rec(orig, rk_cp_off(subj, bs), rk_cp_off(subj, bs + len), 1, STRVAL(t), pos, named);
}
static const char *rk_ev_nm(const Match *m, int off) { return off >= 0 ? m->blk + off : NULL; }
static DESCR_t rk_rx_inst(const Match *m, const char *subj, DESCR_t orig, int i) { int *e = RK_EV(m, i); return rk_rx_sub(m, subj, orig, e[7], i, e[5], e[6]); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_ev_has_name(const int *e, const Match *m, const char *nm) { const char *a = rk_ev_nm(m, e[2]), *b = rk_ev_nm(m, e[3]); return (a && !strcmp(a, nm)) || (b && !strcmp(b, nm)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_rx_slot(const Match *m, const char *subj, DESCR_t orig, const int *evs, int n, int rep) {
    if (n == 0) return rep ? rk_unmark(rk_mk_arr(NULL, 0)) : NULVCL;
    if (!rep && n == 1) return rk_rx_inst(m, subj, orig, evs[0]);
    DESCR_t el[n];
    for (int k = 0; k < n; k++) el[k] = rk_rx_inst(m, subj, orig, evs[k]);
    return rep ? rk_unmark(rk_mk_arr(el, n)) : rk_mk_arr(el, n);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_rx_sub(const Match *m, const char *subj, DESCR_t orig, int lo, int hi, int from, int to) {
    int tops[hi > lo ? hi - lo : 1], nt = 0;
    for (int i = hi - 1; i >= lo; i = RK_EV(m, i)[7] - 1) tops[nt++] = i;
    for (int a = 0, b = nt - 1; a < b; a++, b--) { int t = tops[a]; tops[a] = tops[b]; tops[b] = t; }
    int maxp = -1;
    for (int a = 0; a < nt; a++) { int *e = RK_EV(m, tops[a]); if (e[0] == 0 && e[1] > maxp) maxp = e[1]; }
    DESCR_t pos[maxp + 1 > 0 ? maxp + 1 : 1];
    for (int idx = 0; idx <= maxp; idx++) {
        int evs[nt + 1], n = 0, rep = 0;
        for (int a = 0; a < nt; a++) { int *e = RK_EV(m, tops[a]); if (e[0] == 0 && e[1] == idx) { evs[n++] = tops[a]; rep |= e[4]; } }
        pos[idx] = rk_rx_slot(m, subj, orig, evs, n, rep);
    }
    DESCR_t named = TABLE_VAL(table_new());
    const char *seen[2 * nt + 1];
    int nseen = 0;
    for (int a = 0; a < nt; a++) {
        int *e = RK_EV(m, tops[a]);
        if (e[0] != 1) continue;
        for (int w = 0; w < 2; w++) {
            const char *nm = rk_ev_nm(m, e[2 + w]);
            if (!nm || !nm[0]) continue;
            int dup = 0;
            for (int q = 0; q < nseen; q++) if (!strcmp(seen[q], nm)) dup = 1;
            if (dup) continue;
            seen[nseen++] = nm;
            int evs[nt + 1], n = 0, rep = 0;
            for (int b = 0; b < nt; b++) { int *f = RK_EV(m, tops[b]); if (f[0] == 1 && rk_ev_has_name(f, m, nm)) { evs[n++] = tops[b]; rep |= f[4]; } }
            named = rk_hash_store(named, STRVAL((char *) nm), rk_rx_slot(m, subj, orig, evs, n, rep));
        }
    }
    return rk_match_span(orig, subj, from, to, rk_mk_arr(pos, maxp + 1), named);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rk_match_obj_in(const Match *m, const char *subj, DESCR_t orig) {
    if (!m->matched || !subj) return rk_match_nil();
    DESCR_t sub[m->ngroups > 0 ? m->ngroups : 1];
    int npos = 0;
    DESCR_t named = TABLE_VAL(table_new());
    for (int g = 0; g < m->ngroups; g++) {
        int evs[m->nev + 1], n = 0;
        for (int i = 0; i < m->nev; i++) { int *e = RK_EV(m, i); if (e[8] == g || e[9] == g) evs[n++] = i; }
        DESCR_t v = rk_rx_slot(m, subj, orig, evs, n, match_group_repeatable(m, g) || n > 1);
        const char *nm = match_group_name(m, g);
        if (nm && nm[0]) named = rk_hash_store(named, STRVAL((char *) nm), v);
        else sub[npos++] = v;
    }
    return rk_match_span(orig, subj, m->full_start, m->full_end, rk_mk_arr(sub, npos), named);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rk_match_obj(const Match *m, const char *subj) { return rk_match_obj_in(m, subj, STRVAL(rt_heap_strdup_c(subj ? subj : ""))); }
int grammar_parse_core(const char *gname, const char *subj, DESCR_t *out, int partial, DESCR_t actions) {
    if (!gname) gname = "";
    if (!subj) subj = "";
    char qn[fmt_len("%s::TOP", gname)];
    snprintf(qn, sizeof qn, "%s::TOP", gname);
    const char *body = gram_get(qn);
    if (!body) { *out = FAILDESCR; return 1; }
    int topflv = gram_get_flavor(qn);
    RxEnv env;
    RxProg *prog = rk_rx_compile(&env, gname, body, topflv == 0 ? RXF_R : topflv == 1 ? (RXF_R | RXF_S) : 0);
    if (!prog) { *out = FAILDESCR; return 1; }
    int slen = (int) strlen(subj);
    RxMatch rm;
    Match m;
    int ok = rx_exec(prog, subj, slen, 0, partial ? 1 : 3, &rm);
    rk_rx_to_match(&m, prog, &rm);
    *out = ok ? rk_match_obj_in(&m, subj, STRVAL(rt_heap_strdup_c(subj))) : rk_match_nil();
    if (ok && actions.v != 0 && !IS_FAIL_fn(actions)) rk_apply_actions(*out, actions, STRVAL("TOP"));
    return 1;
}
int rk_uni_is_name(const char *nm) { for (int i = 0; nm && g_rk_uni_types[i]; i++) if (!strcmp(nm, g_rk_uni_types[i])) return 1; return 0; }
PL_CX_LEAF_HEAD(argv, 1) {
    extern int rt_main_args_count(void);
    extern const char *rt_main_arg_at(int);
    extern const char *rt_main_progname(void);
    DESCR_t l = pl_nil();
    for (int i = rt_main_args_count() - 1; i >= -1; i--) { const char *a = i < 0 ? rt_main_progname() : rt_main_arg_at(i); if (!a) a = ""; l = pl_cons(pl_mk_atom_dup(a, strlen(a)), l); }
    ok = plw_unify_vals(args[0], l, cx);
}
PL_CX_LEAF_TAIL
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rk_hash_list(DESCR_t h, int what) {
    TBBLK_t *t = (h.v == DT_T && h.tbl) ? h.tbl : NULL;
    int n = t ? t->size : 0, per = what == 'w' ? 2 : 1, c = 0;
    DESCR_t *r = n ? (DESCR_t *) rt_ws_alloc_descr((size_t) n * (size_t) per) : NULL;
    for (unsigned i = 0; t && i < t->ord_len; i++) {
        DESCR_t key = t->ord[i];
        if (key.v == DT_RAW) continue;
        TBPAIR_t *e = table_find_pair_d(t, key);
        if (!e) continue;
        const char *kt = rk_cstr(key);
        DESCR_t kd = rk_txt_elem(kt, strlen(kt));
        if (what == 'k') r[c++] = kd;
        else if (what == 'v') r[c++] = rt_deref(e->val);
        else if (what == 'p') {
            extern ARBLK_t *array_new(int lo, int hi);
            if (!rk_is_cell(e->val)) { ARBLK_t *bx = array_new(0, 0); bx->data[0] = e->val; e->val = (DESCR_t){ .v = DT_N, .slen = 1, .ptr = (void *) &bx->data[0] }; }
            r[c++] = rk_mk_pair_cell(kd, e->val);
        } else {
            r[c++] = kd;
            r[c++] = rt_deref(e->val);
        }
    }
    return rk_mark_list(rk_mk_arr(r, c));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_pre_env(void) {
    extern char **environ;
    DESCR_t h = TABLE_VAL(table_new());
    for (char **e = environ; e && *e; e++) {
        const char *eq = strchr(*e, '=');
        if (!eq) continue;
        size_t kl = (size_t) (eq - *e);
        char *k = rt_wsb_alloc(kl + 1);
        memcpy(k, *e, kl);
        k[kl] = '\0';
        h = rk_hash_store(h, STRVAL(k), STRVAL(rt_heap_strdup_c(eq + 1)));
    }
    return h;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_pre_args(void) {
    extern int rt_main_args_count(void);
    extern const char *rt_main_arg_at(int);
    int n = rt_main_args_count();
    DESCR_t *r = n > 0 ? (DESCR_t *) rt_ws_alloc_descr((size_t) n) : NULL;
    for (int i = 0; i < n; i++) { const char *a = rt_main_arg_at(i); r[i] = rk_txt_elem(a ? a : "", a ? strlen(a) : 0); }
    return rk_unmark(rk_mk_arr(r, n));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *rk_os_release(const char *key) {
    FILE *f = fopen("/etc/os-release", "r");
    if (!f) return "";
    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (sz < 0) sz = 0;
    char all[(size_t) sz + 1];
    size_t got = fread(all, 1, (size_t) sz, f);
    all[got] = '\0';
    fclose(f);
    size_t kl = strlen(key);
    for (char *ln = all; *ln; ) {
        char *nl = strchr(ln, '\n');
        size_t len = nl ? (size_t) (nl - ln) : strlen(ln);
        if (len > kl && !strncmp(ln, key, kl) && ln[kl] == '=') {
            const char *v = ln + kl + 1;
            size_t n = len - kl - 1;
            while (n && v[n - 1] == '"') n--;
            if (n && v[0] == '"') { v++; n--; }
            return rt_heap_strndup_c(v, n);
        }
        ln += len + (nl ? 1 : 0);
    }
    return "";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_dyn_str(const char *s) { return STRVAL(rt_heap_strdup_c(s ? s : "")); }
static DESCR_t rk_dyn_obj(const char *nm) {
    static int reg = 0;
    if (!reg) {
        DEFDAT_fn("Distro(name,version,auth,release,desc,is-win,path-sep)");
        DEFDAT_fn("Kernel(name,version,release,hardware,arch,bits)");
        DEFDAT_fn("VM(name,version,osname,prefix,precomp-ext,precomp-target)");
        DEFDAT_fn("Compiler(name,backend,version,auth)");
        DEFDAT_fn("Raku(name,version,auth,desc,compiler,signature)");
        reg = 1;
    }
    DESCR_t no = { .v = DT_BOOL, .i = 0 };
    if (!strcmp(nm, "*DISTRO")) {
        const char *id = rk_os_release("ID"), *vid = rk_os_release("VERSION_ID"), *url = rk_os_release("HOME_URL"), *pretty = rk_os_release("PRETTY_NAME"), *fullv = rk_os_release("VERSION");
        const char *vsrc = fullv[0] ? fullv : vid;
        char ver[strlen(vsrc) + 2];
        size_t vk = 0;
        ver[vk++] = 'v';
        for (const char *c = vsrc; *c; c++) { if (*c == ' ') ver[vk++] = '.'; else if (*c != '(' && *c != ')') ver[vk++] = *c; }
        ver[vk] = '\0';
        return DATCON_fn("Distro", rk_dyn_str(id[0] ? id : "linux"), rk_dyn_str(ver), rk_dyn_str(url), rk_dyn_str(vid), rk_dyn_str(pretty), no, rk_dyn_str(":"));
    }
    if (!strcmp(nm, "*KERNEL")) {
        struct utsname u;
        memset(&u, 0, sizeof u);
        uname(&u);
        const char *uv = u.version[0] == '#' ? u.version + 1 : u.version;
        char ver[strlen(uv) + 2];
        size_t k = 0;
        ver[k++] = 'v';
        for (const char *c = uv; *c; c++) ver[k++] = (*c == '-' || *c == ' ' || *c == ':') ? '.' : *c;
        ver[k] = '\0';
        char low[strlen(u.sysname) + 1];
        size_t i = 0;
        for (; u.sysname[i]; i++) low[i] = (char) tolower((unsigned char) u.sysname[i]);
        low[i] = '\0';
        return DATCON_fn("Kernel", rk_dyn_str(low), rk_dyn_str(ver), rk_dyn_str(u.release), rk_dyn_str(u.machine), rk_dyn_str(u.machine), INTVAL((long) (sizeof(void *) * 8)));
    }
    if (!strcmp(nm, "*VM")) return DATCON_fn("VM", rk_dyn_str("moar"), rk_dyn_str("v2022.12"), rk_dyn_str("linux"), rk_dyn_str("/usr"), rk_dyn_str("moarvm"), rk_dyn_str("mbc"));
    DESCR_t comp = DATCON_fn("Compiler", rk_dyn_str("rakudo"), rk_dyn_str("moar"), rk_dyn_str("v2022.12"), rk_dyn_str("Yet Another Society"));
    return DATCON_fn("Raku", rk_dyn_str("Raku"), rk_dyn_str("v6.d"), rk_dyn_str("The Perl Foundation"), rk_typeobj("Str"), comp, rk_typeobj("Blob"));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_pre_value(const char *nm, DESCR_t *out) {
    extern const char *rt_main_progname(void);
    if (!strcmp(nm, "!")) { DESCR_t ex = NV_GET_fn("!"); *out = rk_exc_is(ex) ? ex : NULVCL; return 1; }
    if (!strcmp(nm, "/")) { *out = (g_match.matched && g_subject) ? rk_match_obj(&g_match, g_subject) : rk_match_nil(); return 1; }
    if (!strcmp(nm, "Bool::True") || !strcmp(nm, "Bool::False")) { *out = (DESCR_t){ .v = DT_BOOL, .i = nm[6] == 'T' }; return 1; }
    if (!strncmp(nm, "Order::", 7)) nm += 7;
    if (!strcmp(nm, "Less")) { *out = (DESCR_t){ .v = DT_ORDER, .i = -1 }; return 1; }
    if (!strcmp(nm, "Same")) { *out = (DESCR_t){ .v = DT_ORDER, .i = 0 }; return 1; }
    if (!strcmp(nm, "More")) { *out = (DESCR_t){ .v = DT_ORDER, .i = 1 }; return 1; }
    if (!strcmp(nm, "Empty")) { DESCR_t em = rk_mk_arr(NULL, 0); ((ARBLK_t *) em.arr)->proto = rk_proto_slip; *out = em; return 1; }
    if (!strcmp(nm, "time")) { *out = INTVAL((long) time(NULL)); return 1; }
    if (!strcmp(nm, "now")) { struct timespec ts; clock_gettime(CLOCK_REALTIME, &ts); *out = REALVAL((double) ts.tv_sec + (double) ts.tv_nsec / 1e9); return 1; }
    if (!strcmp(nm, "rand")) { *out = REALVAL((double) random() / ((double) RAND_MAX + 1.0)); return 1; }
    if (!strcmp(nm, "*PID")) { *out = INTVAL((long) getpid()); return 1; }
    if (!strcmp(nm, "*DISTRO") || !strcmp(nm, "*KERNEL") || !strcmp(nm, "*VM") || !strcmp(nm, "*RAKU") || !strcmp(nm, "*PERL")) { *out = rk_dyn_obj(nm); return 1; }
    if (!strcmp(nm, "*PROGRAM")) { const char *p = rt_main_progname(); *out = rk_io_mk(p && *p ? p : "-e"); return 1; }
    if (!strcmp(nm, "*PROGRAM-NAME") || !strcmp(nm, "?FILE")) { const char *p = rt_main_progname(); *out = STRVAL(rt_heap_strdup_c(p && *p ? p : "-e")); return 1; }
    if (!strcmp(nm, "*CWD")) { char *b = rt_wsb_alloc(4096); *out = rk_io_mk(getcwd(b, 4096) ? b : ""); return 1; }
    if (!strcmp(nm, "*EXECUTABLE") || !strcmp(nm, "*EXECUTABLE-NAME")) {
        char *b = rt_wsb_alloc(4096);
        ssize_t n = readlink("/proc/self/exe", b, 4095);
        if (n < 0) n = 0;
        b[n] = '\0';
        *out = nm[11] == '-' ? STRVAL(rt_heap_strdup_c(b)) : rk_io_mk(b);
        return 1;
    }
    if (!strcmp(nm, "*HOME")) { const char *h = getenv("HOME"); *out = rk_io_mk(h ? h : ""); return 1; }
    if (!strcmp(nm, "*TMPDIR")) { const char *h = getenv("TMPDIR"); *out = rk_io_mk(h && *h ? h : "/tmp"); return 1; }
    if (!strcmp(nm, "*USER")) { const char *h = getenv("USER"); *out = STRVAL(rt_heap_strdup_c(h ? h : "")); return 1; }
    if (!strcmp(nm, "%*ENV")) { *out = rk_pre_env(); return 1; }
    if (!strcmp(nm, "@*ARGS")) { *out = rk_pre_args(); return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_list_coercion_meth(const char *m, int nargs) {
    static const char *const names[] = { "list", "flat", "Array", "List", "Slip", "Seq", "eager", "cache", NULL };
    if (nargs != 2 || !m) return 0;
    for (int i = 0; names[i]; i++) if (!strcmp(m, names[i])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_cp_to_byte(const char *s, int cp) { int b = 0; while (cp > 0 && s[b]) { b++; while (((unsigned char) s[b] & 0xC0) == 0x80) b++; cp--; } return b; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_match_ncaps(DESCR_t d) {
    DESCR_t pa = FIELD_GET_fn(d, "positional"), na = FIELD_GET_fn(d, "named");
    ARBLK_t *pb = (pa.v == DT_A && pa.arr) ? (ARBLK_t *) pa.arr : NULL;
    int np = pb ? pb->hi - pb->lo + 1 : 0, total = 0;
    DESCR_t hn = rk_hash_list(na, 'w');
    ARBLK_t *hb = (hn.v == DT_A && hn.arr) ? (ARBLK_t *) hn.arr : NULL;
    int nh = hb ? (hb->hi - hb->lo + 1) / 2 : 0;
    for (int i = 0; i < np; i++) { rk_av_t v = rk_av(pb->data[i]); total += (pb->data[i].v == DT_A && v.n) ? v.n : 1; }
    for (int i = 0; i < nh; i++) { rk_av_t v = rk_av(hb->data[2 * i + 1]); total += (hb->data[2 * i + 1].v == DT_A && v.n) ? v.n : 1; }
    return total;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_match_caps_fill(DESCR_t d, DESCR_t *keys, DESCR_t *vals, long *from) {
    DESCR_t pa = FIELD_GET_fn(d, "positional"), na = FIELD_GET_fn(d, "named");
    ARBLK_t *pb = (pa.v == DT_A && pa.arr) ? (ARBLK_t *) pa.arr : NULL;
    int np = pb ? pb->hi - pb->lo + 1 : 0, n = 0;
    DESCR_t hn = rk_hash_list(na, 'w');
    ARBLK_t *hb = (hn.v == DT_A && hn.arr) ? (ARBLK_t *) hn.arr : NULL;
    int nh = hb ? (hb->hi - hb->lo + 1) / 2 : 0;
    for (int i = 0; i < np + nh; i++) {
        DESCR_t key = i < np ? INTVAL(i) : hb->data[2 * (i - np)], val = i < np ? pb->data[i] : hb->data[2 * (i - np) + 1];
        rk_av_t items = rk_av(val);
        int cnt = (val.v == DT_A) ? items.n : 1;
        for (int k = 0; k < cnt; k++) {
            DESCR_t it = (val.v == DT_A) ? items.el[k] : val;
            if (!IS_DATA_INST_fn(it) || !FIELD_GET_fn(it, "ok").i) continue;
            keys[n] = key;
            vals[n] = it;
            from[n] = FIELD_GET_fn(it, "from").i;
            n++;
        }
    }
    for (int i = 1; i < n; i++) for (int j = i; j > 0 && from[j - 1] > from[j]; j--) {
        DESCR_t tk = keys[j];
        keys[j] = keys[j - 1];
        keys[j - 1] = tk;
        DESCR_t tv = vals[j];
        vals[j] = vals[j - 1];
        vals[j - 1] = tv;
        long tf = from[j];
        from[j] = from[j - 1];
        from[j - 1] = tf;
    }
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_actions_plan_rec(DESCR_t m, DESCR_t actions, DESCR_t key, DESCR_t plan, int bomb) {
    {
        int total = rk_match_ncaps(m);
        DESCR_t keys[total + 1], vals[total + 1];
        long from[total + 1];
        int n = rk_match_caps_fill(m, keys, vals, from);
        for (int i = 0; i < n; i++) rk_actions_plan_rec(vals[i], actions, keys[i], plan, bomb);
    }
    if (!(IS_STR_fn(key) && key.s && key.s[0] != RK_TY)) return;
    const char *cname = rk_typeobj_name(actions);
    if (!cname && IS_STR_fn(actions) && actions.s) cname = actions.s;
    if (!cname && actions.v == DT_DATA && IS_DATA_INST_fn(actions) && actions.u) { DATINST_t *di = (DATINST_t *) actions.u; cname = di->type ? di->type->name : NULL; }
    if (!cname) return;
    char kb[256];
    const char *kn = to_cstring(key, kb, sizeof kb);
    const char *tryk[2] = { kn, NULL };
    char tb[128];
    const char *symtxt = NULL;
    {
        DESCR_t hn = rk_hash_list(FIELD_GET_fn(m, "named"), 'w');
        ARBLK_t *hb = (hn.v == DT_A && hn.arr) ? (ARBLK_t *) hn.arr : NULL;
        int nh = hb ? (hb->hi - hb->lo + 1) / 2 : 0;
        for (int q = 0; q < nh; q++) {
            char sb[64];
            const char *kk = to_cstring(hb->data[2 * q], sb, sizeof sb);
            if (kk && !strcmp(kk, "sym") && IS_DATA_INST_fn(hb->data[2 * q + 1])) { const char *tt = to_cstring(FIELD_GET_fn(hb->data[2 * q + 1], "text"), tb, sizeof tb); symtxt = tt ? tt : ""; }
        }
    }
    char symk[strlen(kn) + (symtxt ? strlen(symtxt) : 0) + 16];
    if (symtxt) { snprintf(symk, sizeof symk, "%s:sym<%s>", kn, symtxt); tryk[1] = symk; }
    for (int t = 0; t < 2; t++) {
        if (!tryk[t]) continue;
        int fidx;
        const char *rmc = resolve_method_chain(cname, tryk[t], &fidx);
        char *procname = rt_heap_fmt("%s__%s", rmc, tryk[t]);
        if (!meth_is_user_proc(procname)) continue;
        if (bomb) {
            extern DESCR_t rt_c2bb_bomb(const char *site, const char *name);
            rt_c2bb_bomb("grammar actions from C (the lowered .parse loops over __rk_actions_plan and the box calls each action)", procname);
        }
        rk_arr_append((ARBLK_t *) plan.arr, STRVAL(procname));
        rk_arr_append((ARBLK_t *) plan.arr, m);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_actions_plan(DESCR_t m, DESCR_t actions) {
    DESCR_t plan = rk_mk_arr(NULL, 0);
    if (rk_match_is(m) && actions.v != 0 && !IS_FAIL_fn(actions)) rk_actions_plan_rec(m, actions, STRVAL("TOP"), plan, 0);
    return plan;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_apply_actions(DESCR_t m, DESCR_t actions, DESCR_t key) { rk_actions_plan_rec(m, actions, key, rk_mk_arr(NULL, 0), 1); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_match_method(const char *m, DESCR_t *args, int nargs, DESCR_t *out) {
    DESCR_t d = args[0];
    int ok = (int) FIELD_GET_fn(d, "ok").i;
    DESCR_t pa = FIELD_GET_fn(d, "positional"), na = FIELD_GET_fn(d, "named");
    extern const char *rk_obj_stringify(DESCR_t d, int use_gist);
    if (!strcmp(m, "Str")) { *out = FIELD_GET_fn(d, "text"); return 1; }
    if (!strcmp(m, "gist")) { *out = STRVAL((char *) rk_obj_stringify(d, 1)); return 1; }
    if (!strcmp(m, "raku") || !strcmp(m, "perl")) { *out = STRVAL((char *) rk_raku_str(d, 0)); return 1; }
    if (!strcmp(m, "Bool") || !strcmp(m, "so") || !strcmp(m, "defined")) { *out = (DESCR_t){ .v = DT_BOOL, .i = ok }; return 1; }
    if (!strcmp(m, "not")) { *out = (DESCR_t){ .v = DT_BOOL, .i = !ok }; return 1; }
    if (!strcmp(m, "from")) { *out = FIELD_GET_fn(d, "from"); return 1; }
    if (!strcmp(m, "to") || !strcmp(m, "pos")) { *out = FIELD_GET_fn(d, "to"); return 1; }
    if (!strcmp(m, "orig")) { *out = FIELD_GET_fn(d, "orig"); return 1; }
    if (!strcmp(m, "elems")) { ARBLK_t *b = (pa.v == DT_A && pa.arr) ? (ARBLK_t *) pa.arr : NULL; *out = INTVAL(b ? b->hi - b->lo + 1 : 0); return 1; }
    if (!strcmp(m, "list") || !strcmp(m, "Array") || !strcmp(m, "List")) { *out = pa; return 1; }
    if (!strcmp(m, "hash") || !strcmp(m, "Hash") || !strcmp(m, "Map")) { *out = rk_hash_copy(na); return 1; }
    if (!strcmp(m, "prematch") || !strcmp(m, "postmatch")) {
        const char *o = VARVAL_fn(FIELD_GET_fn(d, "orig"));
        if (!o) o = "";
        int bf = rk_cp_to_byte(o, (int) FIELD_GET_fn(d, "from").i), bt = rk_cp_to_byte(o, (int) FIELD_GET_fn(d, "to").i);
        const char *st = m[0] == 'p' && m[1] == 'r' ? o : o + bt;
        size_t len = m[1] == 'r' ? (size_t) bf : strlen(o + bt);
        char *t = rt_wsb_alloc(len + 1);
        memcpy(t, st, len);
        t[len] = '\0';
        *out = STRVAL(t);
        return 1;
    }
    if (!strcmp(m, "Int") || !strcmp(m, "Num") || !strcmp(m, "Numeric") || !strcmp(m, "Real") || !strcmp(m, "Rat")) {
        char nb[64];
        const char *t = to_cstring(FIELD_GET_fn(d, "text"), nb, sizeof nb);
        if (!t) t = "";
        char *e;
        long long iv = strtoll(t, &e, 10);
        if (!strcmp(m, "Int") || (!strcmp(m, "Numeric") && !*e && *t)) { *out = INTVAL(iv); return 1; }
        *out = (DESCR_t){ .v = DT_R, .r = strtod(t, NULL) };
        return 1;
    }
    if (!strcmp(m, "put") || !strcmp(m, "note") || !strcmp(m, "print")) {
        char nb[64];
        const char *t = to_cstring(FIELD_GET_fn(d, "text"), nb, sizeof nb);
        if (m[0] == 'n') fprintf(stderr, "%s\n", t ? t : "");
        else printf("%s%s", t ? t : "", m[1] == 'u' ? "\n" : "");
        *out = (DESCR_t){ .v = DT_BOOL, .i = 1 };
        return 1;
    }
    if (!strcmp(m, "made") || !strcmp(m, "ast")) { DESCR_t mv = FIELD_GET_fn(d, "made"); *out = mv; return 1; }
    if (!strcmp(m, "caps") || !strcmp(m, "chunks")) {
        int chunks = m[1] == 'h';
        int total = rk_match_ncaps(d);
        DESCR_t keys[total + 1], vals[total + 1];
        long from[total + 1];
        int n = rk_match_caps_fill(d, keys, vals, from);
        DESCR_t r[2 * n + 3];
        int rn = 0;
        if (!chunks) { for (int i = 0; i < n; i++) r[rn++] = rk_mk_pair(keys[i], vals[i]); *out = rk_mk_arr(r, rn); return 1; }
        const char *txt = VARVAL_fn(FIELD_GET_fn(d, "text"));
        if (!txt) txt = "";
        long base = FIELD_GET_fn(d, "from").i, cur = 0, tlen = (long) utf8_strlen(txt);
        for (int i = 0; i <= n; i++) {
            long stop = i < n ? from[i] - base : tlen;
            if (stop > cur) {
                int b0 = rk_cp_to_byte(txt, (int) cur), b1 = rk_cp_to_byte(txt, (int) stop);
                char *t = (char *) rt_wsb_alloc((size_t) (b1 - b0) + 1);
                memcpy(t, txt + b0, (size_t) (b1 - b0));
                t[b1 - b0] = 0;
                r[rn++] = rk_mk_pair(STRVAL(rt_heap_strdup_c("~")), STRVAL(t));
            }
            if (i < n) { r[rn++] = rk_mk_pair(keys[i], vals[i]); cur = FIELD_GET_fn(vals[i], "to").i - base; }
        }
        *out = rk_mk_arr(r, rn);
        return 1;
    }
    if (!strcmp(m, "keys") || !strcmp(m, "values") || !strcmp(m, "pairs") || !strcmp(m, "kv")) {
        int w = m[0] == 'k' && m[1] == 'e' ? 'k' : m[0] == 'v' ? 'v' : m[0] == 'p' ? 'p' : 'w';
        DESCR_t hn = rk_hash_list(na, w);
        ARBLK_t *hb = (hn.v == DT_A && hn.arr) ? (ARBLK_t *) hn.arr : NULL;
        int nh = hb ? hb->hi - hb->lo + 1 : 0;
        ARBLK_t *pb = (pa.v == DT_A && pa.arr) ? (ARBLK_t *) pa.arr : NULL;
        int np = pb ? pb->hi - pb->lo + 1 : 0;
        DESCR_t r[(w == 'w' ? 2 * np : np) + nh + 1];
        int n = 0;
        for (int i = 0; i < np; i++) { if (w == 'k' || w == 'w') r[n++] = INTVAL(i); if (w == 'v' || w == 'w') r[n++] = pb->data[i]; if (w == 'p') r[n++] = rk_mk_pair(INTVAL(i), pb->data[i]); }
        for (int i = 0; i < nh; i++) r[n++] = hb->data[i];
        *out = rk_mk_arr(r, n);
        return 1;
    }
    return rt_str_method(m, FIELD_GET_fn(d, "text"), &args[2], nargs - 2, out);
}
int bt___rk_gist_pre(DESCR_t *args, int nargs, DESCR_t *out, long *rq) { if (nargs == 1) { if (!rk_strmeth_open(args[0], 1, rq, out)) *out = args[0]; return 1; } return 0; }
int bt___rk_str_pre(DESCR_t *args, int nargs, DESCR_t *out, long *rq) { if (nargs == 1) { if (!rk_strmeth_open(args[0], 0, rq, out)) *out = args[0]; return 1; } return 0; }
int bt___rk_str(DESCR_t *args, int nargs, DESCR_t *out, long *rq) {
    if (nargs == 1) {
        extern const char *rk_obj_stringify(DESCR_t d, int use_gist);
        DESCR_t a = args[0];
        if (rk_strmeth_open(a, 0, rq, out)) return 1;
        { extern int rk_rat_zero_den(DESCR_t); if (rk_rat_zero_den(a)) { rt_script_die_surface("Attempt to divide by zero when coercing Rational to Str"); *out = FAILDESCR; return 1; } }
        if (a.v == DT_BOOL) { *out = STRVAL(rt_heap_strdup_c(a.i ? "True" : "False")); return 1; }
        if (a.v == DT_ORDER) { *out = STRVAL(rt_heap_strdup_c(a.i < 0 ? "Less" : (a.i > 0 ? "More" : "Same"))); return 1; }
        if (a.v == DT_SNUL || rk_typeobj_name(a)) { *out = STRVAL(rt_heap_strdup_c("")); return 1; }
        if (rk_match_is(a)) { *out = FIELD_GET_fn(a, "text"); return 1; }
        if (IS_REAL_fn(a) || IS_INT_fn(a) || a.v == DT_BIG) { *out = STRVAL(rt_heap_strdup_c(rk_cstr(a))); return 1; }
        if ((a.v == DT_A && a.arr) || (a.v == DT_T && a.tbl) || a.v == DT_DATA) {
            const char *st = a.v == DT_DATA ? rk_obj_stringify(a, 0) : a.v == DT_A ? rk_arr_text(a) : rk_tbl_text(a);
            *out = STRVAL(rt_heap_strdup_c(st ? st : ""));
            return 1;
        }
        *out = a;
        return 1;
    }
    return 0;
}
DESCR_t bt___rk_all_true(DESCR_t *args, int nargs) {
    if (nargs == 1) { rk_av_t a = rk_av(args[0]); int ok = 1; for (int i = 0; i < a.n && ok; i++) ok = rk_is_truthy(rk_av_elem(a, i)); return (DESCR_t){ .v = DT_BOOL, .i = ok }; }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_match_last(DESCR_t *args, int nargs) {
    if (nargs == 0) { return (g_match.matched && g_subject) ? rk_match_obj(&g_match, g_subject) : rk_match_nil(); }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_match_clear(DESCR_t *args, int nargs) { if (nargs == 0) { memset(&g_match, 0, sizeof g_match); return NULVCL; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_actions_plan(DESCR_t *args, int nargs) { if (nargs == 2) { return rk_actions_plan(args[0], args[1]); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
static DESCR_t bta_hash_keys(DESCR_t *args, int nargs, int _bop) {
    if (nargs >= 1) { int what = (_bop == 0 || _bop == 3) ? 'k' : (_bop == 1) ? 'v' : 'p'; if ((_bop == 0 || _bop == 3) && (_bop == 3)) what = 'w'; return rk_hash_list(args[0], what); }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_hash_pairs(DESCR_t *args, int nargs) { { const DESCR_t _r = bta_hash_keys(args, nargs, 2); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt_hash_kv(DESCR_t *args, int nargs) { { const DESCR_t _r = bta_hash_keys(args, nargs, 3); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt_hash_values(DESCR_t *args, int nargs) { { const DESCR_t _r = bta_hash_keys(args, nargs, 1); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt_hash_keys(DESCR_t *args, int nargs) { { const DESCR_t _r = bta_hash_keys(args, nargs, 0); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
void rt_main_progname_stage(const char *s) { char *c = s ? ct_strdup(s) : 0; ct_drop((void *) g_runtime.bn.main_progname); g_runtime.bn.main_progname = c; }
const char *rt_main_progname(void) { return g_runtime.bn.main_progname ? g_runtime.bn.main_progname : ""; }
void rt_main_args_gc_root(void) { extern void rt_gc_visit_descr(DESCR_t *); rt_gc_visit_descr(&g_runtime.bn.main_args_descr); }
int rt_main_args_count(void) { return g_runtime.bn.main_args_n < 0 ? 0 : g_runtime.bn.main_args_n; }
const char *rt_main_arg_at(int i) { return (i >= 0 && i < g_runtime.bn.main_args_n && g_runtime.bn.main_args_v) ? g_runtime.bn.main_args_v[i] : (const char *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_match_is(DESCR_t d) { return IS_DATA_INST_fn(d) && d.u && d.u->type && d.u->type->name && !strcmp(d.u->type->name, "Match"); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rk_match_text(DESCR_t d) { const char *t = VARVAL_fn(FIELD_GET_fn(d, "text")); return t ? t : ""; }
static const char *rk_idx_label(int i) { char *l = rt_wsb_alloc(16); sprintf(l, "%d", i); return l; }
static int rk_match_caps(DESCR_t d, rk_cap_t *cs, int cap) {
    int n = 0;
    DESCR_t pa = FIELD_GET_fn(d, "positional"), na = FIELD_GET_fn(d, "named");
    if (pa.v == DT_A && pa.arr) {
        ARBLK_t *b = (ARBLK_t *) pa.arr;
        for (int i = 0; i <= b->hi - b->lo; i++) {
            DESCR_t v = b->data[i];
            if (rk_match_is(v)) {
                if (n < cap) { cs[n].from = (int) FIELD_GET_fn(v, "from").i; cs[n].label = rk_idx_label(i); cs[n].m = v; }
                n++;
            } else if (v.v == DT_A && v.arr) {
                ARBLK_t *l = (ARBLK_t *) v.arr;
                for (int j = 0; j <= l->hi - l->lo; j++) if (rk_match_is(l->data[j])) {
                    if (n < cap) { cs[n].from = (int) FIELD_GET_fn(l->data[j], "from").i; cs[n].label = rk_idx_label(i); cs[n].m = l->data[j]; }
                    n++;
                }
            }
        }
    }
    if (na.v == DT_T && na.tbl) {
        for (unsigned i = 0; i < na.tbl->ord_len; i++) {
            DESCR_t k = na.tbl->ord[i];
            if (k.v == DT_RAW) continue;
            TBPAIR_t *e = table_find_pair_d(na.tbl, k);
            if (!e) continue;
            const char *ks = VARVAL_fn(k) ? VARVAL_fn(k) : "";
            DESCR_t v = rt_deref(e->val);
            if (rk_match_is(v)) {
                if (n < cap) { cs[n].from = (int) FIELD_GET_fn(v, "from").i; cs[n].label = ks; cs[n].m = v; }
                n++;
            } else if (v.v == DT_A && v.arr) {
                ARBLK_t *l = (ARBLK_t *) v.arr;
                for (int j = 0; j <= l->hi - l->lo; j++) if (rk_match_is(l->data[j])) {
                    if (n < cap) { cs[n].from = (int) FIELD_GET_fn(l->data[j], "from").i; cs[n].label = ks; cs[n].m = l->data[j]; }
                    n++;
                }
            }
        }
    }
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static size_t rk_match_gist_len(DESCR_t d, int depth) {
    size_t n = strlen(rk_match_text(d)) + 16;
    int nc = rk_match_caps(d, NULL, 0);
    rk_cap_t cs[nc > 0 ? nc : 1];
    rk_match_caps(d, cs, nc);
    for (int i = 0; i < nc; i++) n += 16 + (size_t) depth * 2 + strlen(cs[i].label) + rk_match_gist_len(cs[i].m, depth + 1);
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_match_gist_into(DESCR_t d, int depth, char *b, size_t *bp) {
    *bp += (size_t) sprintf(b + *bp, "\xEF\xBD\xA2%s\xEF\xBD\xA3", rk_match_text(d));
    int nc = rk_match_caps(d, NULL, 0);
    rk_cap_t cs[nc > 0 ? nc : 1];
    rk_match_caps(d, cs, nc);
    for (int i = 1; i < nc; i++) { rk_cap_t t = cs[i]; int j = i - 1; while (j >= 0 && cs[j].from > t.from) { cs[j + 1] = cs[j]; j--; } cs[j + 1] = t; }
    for (int i = 0; i < nc; i++) { *bp += (size_t) sprintf(b + *bp, "\n%*s%s => ", depth + 1, "", cs[i].label); rk_match_gist_into(cs[i].m, depth + 1, b, bp); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rk_match_render(DESCR_t d, int use_gist) {
    if (!rk_match_is(d)) return NULL;
    if (!(int) FIELD_GET_fn(d, "ok").i) return use_gist ? "Nil" : "";
    if (!use_gist) return rk_match_text(d);
    size_t need = rk_match_gist_len(d, 0) + 64, bp = 0;
    char *b = (char *) rt_str_alloc((long) need);
    rk_match_gist_into(d, 0, b, &bp);
    b[bp] = '\0';
    return b;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rk_cplx_str(DESCR_t d) {
    extern void rt_cplx_parts(DESCR_t, double *, double *);
    double re, im;
    rt_cplx_parts(d, &re, &im);
    char rb[64], ib[64];
    rk_real_str(re, rb, sizeof rb);
    rk_real_str(fabs(im), ib, sizeof ib);
    char *out = rt_wsb_alloc(160);
    snprintf(out, 160, "%s%s%si", rb, im < 0 ? "-" : "+", ib);
    return out;
}
const char *rk_dyn_text(DESCR_t d, int use_gist) {
    static const char *const dyn[] = { "Distro", "Kernel", "VM", "Compiler", "Raku" };
    if (!IS_DATA_INST_fn(d) || !d.u || !d.u->type || !d.u->type->name) return NULL;
    int hit = 0;
    for (size_t i = 0; i < sizeof dyn / sizeof dyn[0]; i++) if (!strcmp(d.u->type->name, dyn[i])) hit = 1;
    if (!hit) return NULL;
    extern DESCR_t c_dat_field_get(const char *field, DESCR_t obj);
    const char *nm = rk_cstr(rtx_try_dat_field_get("name", d));
    if (!use_gist) return nm;
    const char *vs = rk_cstr(rtx_try_dat_field_get("version", d));
    if (*vs == 'v') vs++;
    size_t n = strlen(nm) + strlen(vs) + 4;
    char *r = rt_wsb_alloc(n);
    snprintf(r, n, "%s (%s)", nm, vs);
    return r;
}
const char *rk_obj_default_raku(DESCR_t d) {
    { const char *dt = rk_dyn_text(d, 1); if (dt) return dt; }
    extern int dat_attributes(const char *name, const char **out, int max);
    extern int dat_field_is_private(const char *cls, const char *field);
    extern DESCR_t c_dat_field_get(const char *field, DESCR_t obj);
    const char *cn = d.u->type->name;
    const char *as[bnd_dat_need(cn, 4)];
    int an = dat_attributes(cn, as, (int)(sizeof as / sizeof as[0]));
    const char *piece[an > 0 ? an : 1];
    int np = 0;
    size_t total = strlen(cn) + 8;
    for (int i = 0; i < an; i++) {
        if (!as[i] || dat_field_is_private(cn, as[i])) continue;
        const char *vs = rk_raku_str(rtx_try_dat_field_get(as[i], d), 0);
        size_t pl = strlen(as[i]) + 4 + strlen(vs) + 2;
        char *pc = rt_wsb_alloc(pl + 1);
        snprintf(pc, pl + 1, "%s => %s", as[i], vs);
        piece[np++] = pc;
        total += pl;
    }
    char *r = rt_wsb_alloc(total + 1);
    size_t o = (size_t)snprintf(r, total + 1, "%s.new", cn);
    if (np) { r[o++] = '('; for (int i = 0; i < np; i++) o += (size_t)snprintf(r + o, total + 1 - o, "%s%s", i ? ", " : "", piece[i]); r[o++] = ')'; r[o] = '\0'; }
    return r;
}
int rk_agg_truth(DESCR_t v) {
    if (v.v == DT_T) return v.tbl && v.tbl->size > 0;
    if (!v.arr) return 0;
    rk_av_t a = rk_av(v);
    if (!rk_is_jct(v)) return a.n > 0;
    const char *fl = ((ARBLK_t *) v.arr)->proto;
    int hits = 0;
    for (int i = 0; i < a.n; i++) hits += rk_is_truthy(a.el[i]) ? 1 : 0;
    return fl == rk_proto_jct_any ? hits > 0 : fl == rk_proto_jct_all ? hits == a.n : fl == rk_proto_jct_one ? hits == 1 : hits == 0;
}
int rk_mu_method(const char *m, DESCR_t self, int nmargs, DESCR_t *out) {
    if (rk_uni_method(m, self, nmargs, out)) return 1;
    if (nmargs) return 0;
    if (!strcmp(m, "defined") || !strcmp(m, "Bool") || !strcmp(m, "so") || !strcmp(m, "DEFINITE")) { *out = (DESCR_t){ .v = DT_BOOL, .i = 1 }; return 1; }
    if (!strcmp(m, "not")) { *out = (DESCR_t){ .v = DT_BOOL, .i = 0 }; return 1; }
    if (!strcmp(m, "gist") || !strcmp(m, "raku") || !strcmp(m, "perl")) { *out = STRVAL(rk_obj_default_raku(self)); return 1; }
    if (!strcmp(m, "Str") && rk_dyn_text(self, 0)) { *out = STRVAL((char *) rk_dyn_text(self, 0)); return 1; }
    if (!strcmp(m, "Str")) {
        size_t sl = strlen(self.u->type->name) + 32;
        char *r = rt_wsb_alloc(sl);
        snprintf(r, sl, "%s<%lu>", self.u->type->name, (unsigned long)(uintptr_t)self.u);
        *out = STRVAL(r);
        return 1;
    }
    if (!strcmp(m, "item") || !strcmp(m, "self") || !strcmp(m, "head") || !strcmp(m, "tail") || !strcmp(m, "min") || !strcmp(m, "max") || !strcmp(m, "pick") || !strcmp(m, "roll")) {
        *out = self;
        return 1;
    }
    if (!strcmp(m, "elems")) { *out = INTVAL(1); return 1; }
    if (!strcmp(m, "list") || !strcmp(m, "Seq") || !strcmp(m, "flat") || !strcmp(m, "cache") || !strcmp(m, "Slip")) { *out = rk_mark_list(rk_mk_arr(&self, 1)); return 1; }
    return 0;
}
static int rk_strmeth_open(DESCR_t d, int use_gist, long *rq, DESCR_t *out) {
    if (!rq || !IS_DATA_INST_fn(d) || !d.u || !d.u->type || !d.u->type->name || rk_uni_is_name(d.u->type->name)) return 0;
    { extern int rk_rat_is(DESCR_t); if (rk_rat_is(d) || rk_match_is(d)) return 0; }
    const char *mname = use_gist ? "gist" : "Str";
    const char *rmc = resolve_method_chain(d.u->type->name, mname, NULL);
    char proc[fmt_len("%s__%s", rmc, mname)];
    snprintf(proc, sizeof proc, "%s__%s", rmc, mname);
    if (!meth_is_user_proc(proc)) return 0;
    rt_call_args_need(1);
    CALL_ARGS[0] = d;
    *out = rk_proc_descr_rq(proc, 1, rq);
    return 1;
}
