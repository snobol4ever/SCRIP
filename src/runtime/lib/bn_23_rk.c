#include "runtime/lib/bn_head.h"
#include <sys/stat.h>
#include "runtime/rt/rt_arena.h"
#include "runtime/core/runtime_shim.h"
#include "driver/driver.h"
#include "driver/driver_globals.h"
#include "runtime/builtin_leaf.h"
#include <errno.h>
#include <ctype.h>
#include <fcntl.h>
#include "runtime/lib/bn_03_sh.h"
#include "runtime/lib/bn_08_sh.h"
#include "runtime/lib/bn_14_rk.h"
#include "runtime/lib/bn_16_rk.h"
#include "runtime/lib/bn_17_pas.h"
#include "runtime/lib/bn_18_rk.h"
#include "runtime/lib/bn_20_pl.h"
#include "runtime/lib/bn_23_rk.h"
extern void *rt_ws_alloc_descr(size_t);
extern int rt_proc_is_registered(const char *name);
extern RK_DIR *opendir(const char *);
extern struct rk_dirent *readdir(RK_DIR *);
extern int closedir(RK_DIR *);
extern FILE *fh_get(int);
DESCR_t bt_IO(DESCR_t *args, int nargs) {
    char sb[64];
    const char *s = args[0].v == DT_A ? rk_av_joined(rk_av(args[0]), " ", "", "") : IS_REAL_fn(args[0]) ? rk_real_str(args[0].r, sb, (int)sizeof sb) : to_cstring(args[0], sb, sizeof sb);
    if (!s) s = "";
    size_t n = strlen(s);
    if ((nargs - 1) == 0 && args[0].v != DT_A) { return rk_io_mk(s); }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
void rk_io_init(void) {
    static int rk_io_reg = 0;
    if (rk_io_reg) return;
    rk_io_reg = 1;
    extern void record_register(const char *spec);
    extern void class_inherit(const char *child, const char *parent);
    rk_exc_init();
    record_register("IO::Path(path,cwd)");
    record_register("X::IO(message)");
    class_inherit("X::IO", "Exception");
    static const char *const sub[] = { "X::IO::DoesNotExist", "X::IO::Unlink", "X::IO::Mkdir", "X::IO::Rmdir", "X::IO::Rename", "X::IO::Copy", "X::IO::Chmod", "X::IO::Dir", NULL };
    for (int i = 0; sub[i]; i++) { char spec[strlen(sub[i]) + 16]; snprintf(spec, sizeof spec, "%s(message)", sub[i]); record_register(spec); class_inherit(sub[i], "X::IO"); }
}
static const char *rk_io_cwdstr(DESCR_t d) { const char *t = VARVAL_fn(FIELD_GET_fn(d, "cwd")); return t ? t : ""; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_io_mk_in(const char *path, const char *cwd) { rk_io_init(); return DATCON_fn("IO::Path", STRVAL(rt_heap_strdup_c(path)), STRVAL(rt_heap_strdup_c(cwd))); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rk_io_mk(const char *path) { char *cw = rt_wsb_alloc(4096); if (!getcwd(cw, 4096)) strcpy(cw, "/"); return rk_io_mk_in(path, cw); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *rk_io_abs(DESCR_t d) {
    const char *p = rk_io_pathstr(d);
    if (p[0] == '/') return p;
    const char *cw = rk_io_cwdstr(d);
    char *o = rt_wsb_alloc(strlen(cw) + strlen(p) + 3);
    size_t cl = strlen(cw);
    if (!p[0]) strcpy(o, cw);
    else sprintf(o, "%s%s%s", cw, cl && cw[cl - 1] == '/' ? "" : "/", p);
    return o;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *rk_io_clean(const char *p) {
    size_t n = strlen(p);
    char *o = rt_wsb_alloc(n + 2);
    size_t k = 0;
    int lead = p[0] == '/';
    for (size_t i = 0; i < n; ) {
        if (p[i] == '/') { if (k == 0 || o[k - 1] != '/') o[k++] = '/'; i++; continue; }
        if (p[i] == '.' && (i + 1 == n || p[i + 1] == '/') && (k == 0 || o[k - 1] == '/')) { i += (i + 1 == n) ? 1 : 2; continue; }
        o[k++] = p[i++];
    }
    if (k > 1 && o[k - 1] == '/') k--;
    if (k == 0) { o[0] = lead ? '/' : '.'; k = 1; }
    o[k] = '\0';
    return o;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *rk_io_basename(const char *p) {
    size_t n = strlen(p);
    while (n > 1 && p[n - 1] == '/') n--;
    if (n == 1 && p[0] == '/') return "/";
    size_t b = n;
    while (b > 0 && p[b - 1] != '/') b--;
    char *o = rt_wsb_alloc(n - b + 1);
    memcpy(o, p + b, n - b);
    o[n - b] = '\0';
    return o;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *rk_io_dirname(const char *p) {
    size_t n = strlen(p);
    while (n > 1 && p[n - 1] == '/') n--;
    size_t b = n;
    while (b > 0 && p[b - 1] != '/') b--;
    if (b == 0) return ".";
    while (b > 1 && p[b - 1] == '/') b--;
    char *o = rt_wsb_alloc(b + 1);
    memcpy(o, p, b);
    o[b] = '\0';
    return o;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *rk_io_join(const char *dir, const char *name) {
    size_t dl = strlen(dir);
    char *o = rt_wsb_alloc(dl + strlen(name) + 2);
    if (!strcmp(dir, ".") && 0) sprintf(o, "%s", name);
    else sprintf(o, "%s%s%s", dir, dl && dir[dl - 1] == '/' ? "" : "/", name);
    return o;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *rk_io_err(int e) {
    const char *m = strerror(e);
    char *o = rt_wsb_alloc(strlen(m) + 1);
    strcpy(o, m);
    if (o[0] >= 'A' && o[0] <= 'Z') o[0] = (char) tolower((unsigned char) o[0]);
    return o;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_io_maxpos(const char *m) {
    static const struct {
        const char *n;
        int p;
    } t[] = { { "spurt", 1 }, { "copy", 1 }, { "rename", 1 }, { "move", 1 }, { "chmod", 1 }, { "child", 1 }, { "add", 1 }, { "sibling", 1 }, { "parent", 1 }, { "mkdir", 1 }, { "absolute", 1 },
        { "relative", 1 }, { "readchars", 1 }, { "read", 1 }, { "seek", 2 }, { "write", 1 }, { "extension", 1 }, { "lines", 1 }, { "words", 1 }, { "open", 0 }, { "slurp", 0 }, { "dir", 0 }, { NULL,
        0 } };
    for (int i = 0; t[i].n; i++) if (!strcmp(m, t[i].n)) return t[i].p;
    return 99;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_io_split(const char *m, DESCR_t *a, int na, DESCR_t **kv_out) {
    int mp = rk_io_maxpos(m), npos = 0;
    while (npos < na && npos < mp && !rk_is_pair(a[npos])) { if (mp != 99 && IS_STR_fn(a[npos]) && npos + 1 < na && na - npos >= 2 && !strcmp(m, "lines") && 0) break; npos++; }
    if ((!strcmp(m, "lines") || !strcmp(m, "words")) && npos == 1 && !IS_INT_fn(a[0])) npos = 0;
    int rest = na - npos;
    DESCR_t *kv = (DESCR_t *) rt_ws_alloc_descr((size_t) rest + 1);
    int k = 0;
    for (int i = npos; i < na; ) {
        if (rk_is_pair(a[i])) { rk_av_t p = rk_av(a[i]); kv[k++] = p.el[0]; kv[k++] = p.n > 1 ? p.el[1] : NULVCL; i++; } else { kv[k++] = a[i]; if (i + 1 < na) kv[k++] = a[i + 1]; i += 2; }
    }
    *kv_out = kv;
    return npos | (k << 16);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_io_text_of(DESCR_t d, char **buf, size_t *len) {
    const char *p = rk_io_arg(d);
    FILE *f = fopen(p, "rb");
    if (!f) return 0;
    fseek(f, 0, SEEK_END);
    long n = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (n < 0) n = 0;
    char *b = rt_wsb_alloc((size_t) n + 1);
    size_t r = n ? fread(b, 1, (size_t) n, f) : 0;
    b[r] = '\0';
    fclose(f);
    *buf = b;
    *len = r;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_io_lines_of(const char *t, size_t n, int chomp) {
    int cnt = 0;
    for (size_t i = 0; i < n; i++) if (t[i] == '\n') cnt++;
    if (n && t[n - 1] != '\n') cnt++;
    DESCR_t *el = cnt ? (DESCR_t *) rt_ws_alloc_descr((size_t) cnt) : NULL;
    int c = 0;
    size_t i = 0;
    while (i < n) {
        size_t j = i;
        while (j < n && t[j] != '\n') j++;
        size_t e = j;
        int nl = j < n;
        if (chomp && e > i && t[e - 1] == '\r') e--;
        size_t len = chomp ? e - i : (nl ? j + 1 - i : j - i);
        char *b = rt_wsb_alloc(len + 1);
        memcpy(b, t + i, len);
        b[len] = '\0';
        el[c++] = STRVAL(b);
        i = nl ? j + 1 : j;
    }
    return rk_mk_arr(el, c);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_io_words_of(const char *t, size_t n) {
    int cnt = 0;
    { size_t i = 0; while (i < n) { while (i < n && isspace((unsigned char) t[i])) i++; if (i < n) { cnt++; while (i < n && !isspace((unsigned char) t[i])) i++; } } }
    DESCR_t *el = cnt ? (DESCR_t *) rt_ws_alloc_descr((size_t) cnt) : NULL;
    int c = 0;
    size_t i = 0;
    while (i < n) {
        while (i < n && isspace((unsigned char) t[i])) i++;
        size_t j = i;
        while (j < n && !isspace((unsigned char) t[j])) j++;
        if (j > i) { char *b = rt_wsb_alloc(j - i + 1); memcpy(b, t + i, j - i); b[j - i] = '\0'; el[c++] = STRVAL(b); }
        i = j;
    }
    return rk_mk_arr(el, c);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_io_open(DESCR_t path, DESCR_t *kv, int nkv) {
    const char *p = rk_io_arg(path);
    const char *mode = "r";
    int mc = 'r';
    if (rk_io_flag(kv, nkv, "w", 0)) {
        mode = "w";
        mc = 'w';
    } else if (rk_io_flag(kv, nkv, "a", 0)) {
        mode = "a";
        mc = 'a';
    } else if (rk_io_flag(kv, nkv, "rw", 0)) {
        mode = "r+";
        mc = '+';
    } else if (rk_io_flag(kv, nkv, "x", 0)) {
        mode = "wx";
        mc = 'w';
    } else if (rk_io_flag(kv, nkv, "update", 0)) {
        mode = "r+";
        mc = '+';
    }
    for (int i = 0; i + 1 < nkv; i += 2) if (!strcmp(rk_cstr(kv[i]), "mode")) {
        const char *m = rk_cstr(kv[i + 1]);
        if (!strcmp(m, "wo")) { mode = "w"; mc = 'w'; } else if (!strcmp(m, "ro")) { mode = "r"; mc = 'r'; } else if (!strcmp(m, "rw")) { mode = "r+"; mc = '+'; }
    }
    FILE *f = fopen(p, mode);
    if (!f && mc == '+') { f = fopen(p, "w+"); }
    if (!f) { char m[strlen(p) + 96]; snprintf(m, sizeof m, "Failed to open file %s: %s", rk_io_is(path) ? rk_io_abs(path) : p, rk_io_err(errno)); rk_io_fail("X::AdHoc", m); return FAILDESCR; }
    int idx = fh_alloc(f);
    g_fh[idx].name = rt_heap_strdup_c(p);
    g_fh[idx].mode = (char) mc;
    g_fh[idx].type = 't';
    g_fh[idx].closed = 0;
    g_fh[idx].eof = 0;
    return FHVAL(idx);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_io_getline(FILE *f, char **out, size_t *len) {
    size_t cap = 0, n = 0;
    char *b = NULL;
    int ch, any = 0;
    while ((ch = fgetc(f)) != EOF) {
        any = 1;
        if (n + 2 > cap) { size_t nc = cap ? cap * 2 : 128; char *nb = rt_wsb_alloc(nc); if (n) memcpy(nb, b, n); b = nb; cap = nc; }
        b[n++] = (char) ch;
        if (ch == '\n') break;
    }
    if (!any) return 0;
    b[n] = '\0';
    *out = b;
    *len = n;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_io_fh_method(const char *m, DESCR_t *args, int nargs, DESCR_t *out) {
    int idx = (int) args[0].i;
    FILE *f = fh_get(idx);
    DESCR_t *a = args + 2;
    int na = nargs - 2;
    DESCR_t True = (DESCR_t){ .v = DT_BOOL, .i = 1 };
    DESCR_t *fkv = NULL;
    int fnk = 0;
    if (strcmp(m, "say") && strcmp(m, "print") && strcmp(m, "put") && strcmp(m, "printf")) { int sp = rk_io_split(m, a, na, &fkv); na = sp & 0xFFFF; fnk = sp >> 16; }
    if (!strcmp(m, "close")) { if (f && idx >= 3) { fclose(f); g_fh[idx].closed = 1; fh_free(idx); } *out = True; return 1; }
    if (!f) { if (!strcmp(m, "opened")) { *out = (DESCR_t){ .v = DT_BOOL, .i = 0 }; return 1; } return 0; }
    if (!strcmp(m, "say")) { if (f != stdout) fflush(stdout); rk_io_write_args(f, a, na, 1, 1); *out = True; return 1; }
    if (!strcmp(m, "print")) { if (f != stdout) fflush(stdout); rk_io_write_args(f, a, na, 0, 0); *out = True; return 1; }
    if (!strcmp(m, "put")) { if (f != stdout) fflush(stdout); rk_io_write_args(f, a, na, 0, 1); *out = True; return 1; }
    if (!strcmp(m, "printf") && na >= 1) { DESCR_t r; if (script_try_call_builtin_by_name("sprintf", a, na, &r) && !IS_FAIL_fn(r)) { fputs(rk_cstr(r), f); } *out = True; return 1; }
    if (!strcmp(m, "flush")) { fflush(f); *out = True; return 1; }
    if (!strcmp(m, "opened")) { *out = (DESCR_t){ .v = DT_BOOL, .i = 1 }; return 1; }
    if (!strcmp(m, "eof")) { int ch = fgetc(f); if (ch == EOF) { *out = True; return 1; } ungetc(ch, f); *out = (DESCR_t){ .v = DT_BOOL, .i = 0 }; return 1; }
    if (!strcmp(m, "get")) {
        char *l;
        size_t n;
        if (!rk_io_getline(f, &l, &n)) { *out = rk_typeobj("Str"); return 1; }
        while (n && (l[n - 1] == '\n' || l[n - 1] == '\r')) n--;
        l[n] = '\0';
        *out = STRVAL(l);
        return 1;
    }
    if (!strcmp(m, "getc")) {
        int ch = fgetc(f);
        if (ch == EOF) { *out = rk_typeobj("Str"); return 1; }
        size_t cl = ch >= 0xF0 ? 4 : ch >= 0xE0 ? 3 : ch >= 0xC0 ? 2 : 1;
        char *b = rt_wsb_alloc(cl + 1);
        b[0] = (char) ch;
        for (size_t i = 1; i < cl; i++) { int c2 = fgetc(f); if (c2 == EOF) { cl = i; break; } b[i] = (char) c2; }
        b[cl] = '\0';
        *out = STRVAL(b);
        return 1;
    }
    if (!strcmp(m, "slurp") || !strcmp(m, "slurp-rest") || !strcmp(m, "lines") || !strcmp(m, "words")) {
        size_t cap = 4096, n = 0;
        char *b = rt_wsb_alloc(cap);
        int ch;
        while ((ch = fgetc(f)) != EOF) { if (n + 2 > cap) { char *nb = rt_wsb_alloc(cap * 2); memcpy(nb, b, n); b = nb; cap *= 2; } b[n++] = (char) ch; }
        b[n] = '\0';
        if (m[0] == 's') { *out = STRVAL(b); return 1; }
        if (m[0] == 'l') { *out = rk_io_lines_of(b, n, rk_io_flag(fkv, fnk, "chomp", 1)); return 1; }
        *out = rk_io_words_of(b, n);
        return 1;
    }
    if (!strcmp(m, "readchars") || !strcmp(m, "read")) {
        long want = (na >= 1 && IS_INT_fn(a[0])) ? (long) a[0].i : 65536;
        char *b = rt_wsb_alloc((size_t) want * 4 + 1);
        size_t n = 0;
        long chars = 0;
        int ch;
        while (chars < want && (ch = fgetc(f)) != EOF) {
            b[n++] = (char) ch;
            if (((unsigned char) ch & 0xC0) != 0x80) {
                int more = ch >= 0xF0 ? 3 : ch >= 0xE0 ? 2 : ch >= 0xC0 ? 1 : 0;
                for (int k = 0; k < more; k++) { int c2 = fgetc(f); if (c2 == EOF) break; b[n++] = (char) c2; }
                chars++;
            }
        }
        b[n] = '\0';
        *out = STRVAL(b);
        return 1;
    }
    if (!strcmp(m, "seek") && na >= 1) {
        long off = IS_INT_fn(a[0]) ? (long) a[0].i : 0;
        int wh = SEEK_SET;
        if (na >= 2) {
            const char *w = rk_cstr(a[1]);
            if (strstr(w, "Current")) wh = SEEK_CUR;
            else if (strstr(w, "End")) wh = SEEK_END;
            else if (IS_INT_fn(a[1])) wh = a[1].i == 1 ? SEEK_CUR : a[1].i == 2 ? SEEK_END : SEEK_SET;
        }
        *out = (DESCR_t){ .v = DT_BOOL, .i = fseek(f, off, wh) == 0 };
        return 1;
    }
    if (!strcmp(m, "tell")) { *out = INTVAL(ftell(f)); return 1; }
    if (!strcmp(m, "path") || !strcmp(m, "IO")) { *out = rk_io_mk(g_fh[idx].name ? g_fh[idx].name : ""); return 1; }
    if (!strcmp(m, "t")) { *out = (DESCR_t){ .v = DT_BOOL, .i = isatty(fileno(f)) }; return 1; }
    if (!strcmp(m, "native-descriptor")) { *out = INTVAL(fileno(f)); return 1; }
    if (!strcmp(m, "write") && na >= 1) { fputs(rk_cstr(a[0]), f); *out = True; return 1; }
    if (!strcmp(m, "encoding")) { *out = STRVAL(rt_heap_strdup_c("utf8")); return 1; }
    if (!strcmp(m, "Bool") || !strcmp(m, "defined") || !strcmp(m, "so")) { *out = True; return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_io_stat(DESCR_t d, struct stat *st) { return stat(rk_io_abs(d), st) == 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_io_dir_list(DESCR_t d, DESCR_t *kv, int nkv, DESCR_t *out) {
    (void) kv;
    (void) nkv;
    const char *dir = rk_io_pathstr(d);
    RK_DIR *dp = opendir(rk_io_abs(d));
    if (!dp) { char m[strlen(dir) + 80]; snprintf(m, sizeof m, "Failed to get the directory contents of '%s': %s", dir, rk_io_err(errno)); return rk_io_fail("X::IO::Dir", m); }
    int cap = 64, n = 0;
    DESCR_t *el = (DESCR_t *) rt_ws_alloc_descr((size_t) cap);
    struct rk_dirent *de;
    while ((de = readdir(dp))) {
        if (!strcmp(de->d_name, ".") || !strcmp(de->d_name, "..")) continue;
        if (n == cap) { DESCR_t *ne = (DESCR_t *) rt_ws_alloc_descr((size_t) cap * 2); memcpy(ne, el, sizeof(DESCR_t) * (size_t) n); el = ne; cap *= 2; }
        el[n++] = rk_io_mk_in(!strcmp(dir, ".") ? de->d_name : rk_io_join(dir, de->d_name), rk_io_cwdstr(d));
    }
    closedir(dp);
    *out = rk_mk_arr(el, n);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_io_path_method(const char *m, DESCR_t *args, int nargs, DESCR_t *out) {
    DESCR_t d = args[0];
    DESCR_t *a = args + 2;
    int na = nargs - 2;
    const char *p = rk_io_pathstr(d);
    DESCR_t True = (DESCR_t){ .v = DT_BOOL, .i = 1 }, False = (DESCR_t){ .v = DT_BOOL, .i = 0 };
    struct stat st;
    DESCR_t *pos = a, *kv;
    int sp = rk_io_split(m, a, na, &kv);
    int npos = sp & 0xFFFF, nkv = sp >> 16;
    if (!strcmp(m, "Str") || !strcmp(m, "path")) { *out = STRVAL(rt_heap_strdup_c(p)); return 1; }
    if (!strcmp(m, "gist") || !strcmp(m, "raku") || !strcmp(m, "perl")) {
        char *o = rt_wsb_alloc(strlen(p) * 2 + strlen(rk_io_cwdstr(d)) + 120);
        if (m[0] == 'g') sprintf(o, "\"%s\".IO", p);
        else sprintf(o, "IO::Path.new(\"%s\", :SPEC(IO::Spec::Unix), :CWD(\"%s\"))", p, rk_io_cwdstr(d));
        *out = STRVAL(o);
        return 1;
    }
    if (!strcmp(m, "IO")) { *out = d; return 1; }
    if (!strcmp(m, "Bool") || !strcmp(m, "so") || !strcmp(m, "defined")) { *out = True; return 1; }
    if (!strcmp(m, "basename")) { *out = STRVAL(rt_heap_strdup_c(rk_io_basename(p))); return 1; }
    if (!strcmp(m, "dirname")) { *out = STRVAL(rt_heap_strdup_c(rk_io_dirname(p))); return 1; }
    if (!strcmp(m, "volume")) { *out = STRVAL(rt_heap_strdup_c("")); return 1; }
    if (!strcmp(m, "extension")) { const char *b = rk_io_basename(p); const char *dot = strrchr(b, '.'); *out = STRVAL(rt_heap_strdup_c(dot && dot != b ? dot + 1 : "")); return 1; }
    if (!strcmp(m, "is-absolute")) { *out = p[0] == '/' ? True : False; return 1; }
    if (!strcmp(m, "is-relative")) { *out = p[0] == '/' ? False : True; return 1; }
    if (!strcmp(m, "absolute")) {
        if (npos >= 1) { DESCR_t b = rk_io_mk_in(rk_cstr(pos[0]), rk_io_cwdstr(d)); DESCR_t t = rk_io_mk_in(p, rk_io_abs(b)); *out = STRVAL(rt_heap_strdup_c(rk_io_abs(t))); return 1; }
        *out = STRVAL(rt_heap_strdup_c(rk_io_abs(d)));
        return 1;
    }
    if (!strcmp(m, "relative")) {
        const char *ab = rk_io_abs(d), *bs = npos >= 1 ? rk_cstr(pos[0]) : rk_io_cwdstr(d);
        size_t bl = strlen(bs);
        while (bl > 1 && bs[bl - 1] == '/') bl--;
        if (!strncmp(ab, bs, bl) && (ab[bl] == '/' || ab[bl] == '\0')) { const char *r = ab + bl; while (*r == '/') r++; *out = STRVAL(rt_heap_strdup_c(*r ? r : ".")); return 1; }
        *out = STRVAL(rt_heap_strdup_c(ab));
        return 1;
    }
    if (!strcmp(m, "parent")) {
        int up = (npos >= 1 && IS_INT_fn(pos[0])) ? (int) pos[0].i : 1;
        const char *q = p;
        for (int i = 0; i < up; i++) q = rk_io_dirname(q);
        *out = rk_io_mk_in(q, rk_io_cwdstr(d));
        return 1;
    }
    if ((!strcmp(m, "child") || !strcmp(m, "add")) && npos >= 1) { *out = rk_io_mk_in(rk_io_join(p, rk_cstr(pos[0])), rk_io_cwdstr(d)); return 1; }
    if (!strcmp(m, "sibling") && npos >= 1) { *out = rk_io_mk_in(rk_io_join(rk_io_dirname(p), rk_cstr(pos[0])), rk_io_cwdstr(d)); return 1; }
    if (!strcmp(m, "cleanup") || !strcmp(m, "resolve")) {
        if (m[0] == 'r') { char *rb = rt_wsb_alloc(4097); if (realpath(rk_io_abs(d), rb)) { *out = rk_io_mk_in(rb, rk_io_cwdstr(d)); return 1; } }
        *out = rk_io_mk_in(rk_io_clean(p), rk_io_cwdstr(d));
        return 1;
    }
    if (!strcmp(m, "e")) { *out = rk_io_stat(d, &st) ? True : False; return 1; }
    if (!strcmp(m, "f")) { *out = (rk_io_stat(d, &st) && S_ISREG(st.st_mode)) ? True : False; return 1; }
    if (!strcmp(m, "d")) { *out = (rk_io_stat(d, &st) && S_ISDIR(st.st_mode)) ? True : False; return 1; }
    if (!strcmp(m, "l")) { *out = (lstat(rk_io_abs(d), &st) == 0 && S_ISLNK(st.st_mode)) ? True : False; return 1; }
    if (!strcmp(m, "r") || !strcmp(m, "w") || !strcmp(m, "x")) { *out = access(rk_io_abs(d), m[0] == 'r' ? R_OK : m[0] == 'w' ? W_OK : X_OK) == 0 ? True : False; return 1; }
    if (!strcmp(m, "rw")) { *out = access(rk_io_abs(d), R_OK | W_OK) == 0 ? True : False; return 1; }
    if (!strcmp(m, "rwx")) { *out = access(rk_io_abs(d), R_OK | W_OK | X_OK) == 0 ? True : False; return 1; }
    if (!strcmp(m, "z")) { *out = (rk_io_stat(d, &st) && S_ISREG(st.st_mode) && st.st_size == 0) ? True : False; return 1; }
    if (!strcmp(m, "s")) { if (rk_io_stat(d, &st) && S_ISREG(st.st_mode)) { *out = INTVAL((long) st.st_size); return 1; } *out = FAILDESCR; return 1; }
    if (!strcmp(m, "modified") || !strcmp(m, "accessed") || !strcmp(m, "changed")) {
        if (!rk_io_stat(d, &st)) { *out = FAILDESCR; return 1; }
        *out = REALVAL((double) (m[0] == 'm' ? st.st_mtime : m[0] == 'a' ? st.st_atime : st.st_ctime));
        return 1;
    }
    if (!strcmp(m, "mode")) { if (!rk_io_stat(d, &st)) { *out = FAILDESCR; return 1; } *out = INTVAL((long) (st.st_mode & 07777)); return 1; }
    if (!strcmp(m, "inode")) { if (!rk_io_stat(d, &st)) { *out = FAILDESCR; return 1; } *out = INTVAL((long) st.st_ino); return 1; }
    if (!strcmp(m, "slurp")) {
        char *b;
        size_t n;
        if (!rk_io_text_of(d, &b, &n)) {
            char mm[strlen(p) + 96];
            snprintf(mm, sizeof mm, "Failed to open file %s: %s", rk_io_abs(d), rk_io_err(errno));
            rk_io_fail("X::AdHoc", mm);
            *out = FAILDESCR;
            return 1;
        }
        *out = STRVAL(b);
        return 1;
    }
    if (!strcmp(m, "lines") || !strcmp(m, "words")) {
        char *b;
        size_t n;
        if (!rk_io_text_of(d, &b, &n)) {
            char mm[strlen(p) + 96];
            snprintf(mm, sizeof mm, "Failed to open file %s: %s", rk_io_abs(d), rk_io_err(errno));
            rk_io_fail("X::AdHoc", mm);
            *out = FAILDESCR;
            return 1;
        }
        *out = m[0] == 'l' ? rk_io_lines_of(b, n, rk_io_flag(kv, nkv, "chomp", 1)) : rk_io_words_of(b, n);
        return 1;
    }
    if (!strcmp(m, "spurt") && npos >= 1) {
        const char *mode = rk_io_flag(kv, nkv, "append", 0) ? "a" : rk_io_flag(kv, nkv, "createonly", 0) ? "wx" : "w";
        FILE *f = fopen(rk_io_abs(d), mode);
        if (!f) { char mm[strlen(p) + 96]; snprintf(mm, sizeof mm, "Failed to open file %s: %s", rk_io_abs(d), rk_io_err(errno)); rk_io_fail("X::AdHoc", mm); *out = FAILDESCR; return 1; }
        out_write_descr(f, pos[0], 0);
        fclose(f);
        *out = True;
        return 1;
    }
    if (!strcmp(m, "open")) { *out = rk_io_open(d, kv, nkv); return 1; }
    if (!strcmp(m, "unlink")) {
        if (unlink(rk_io_abs(d)) == 0 || errno == ENOENT) { *out = True; return 1; }
        char mm[strlen(p) + 96];
        snprintf(mm, sizeof mm, "Failed to remove the file '%s': %s", rk_io_abs(d), rk_io_err(errno));
        rk_io_fail("X::IO::Unlink", mm);
        *out = FAILDESCR;
        return 1;
    }
    if (!strcmp(m, "mkdir")) {
        int mode = (npos >= 1 && IS_INT_fn(pos[0])) ? (int) pos[0].i : 0777;
        if (mkdir(rk_io_abs(d), (mode_t) mode) == 0 || (errno == EEXIST && rk_io_stat(d, &st) && S_ISDIR(st.st_mode))) { *out = d; return 1; }
        char mm[strlen(p) + 96];
        snprintf(mm, sizeof mm, "Failed to create directory '%s' with mode '0o%o': %s", rk_io_abs(d), mode, rk_io_err(errno));
        rk_io_fail("X::IO::Mkdir", mm);
        *out = FAILDESCR;
        return 1;
    }
    if (!strcmp(m, "rmdir")) {
        if (rmdir(rk_io_abs(d)) == 0) { *out = True; return 1; }
        char mm[strlen(p) + 96];
        snprintf(mm, sizeof mm, "Failed to remove the directory '%s': %s", rk_io_abs(d), rk_io_err(errno));
        rk_io_fail("X::IO::Rmdir", mm);
        *out = FAILDESCR;
        return 1;
    }
    if ((!strcmp(m, "copy") || !strcmp(m, "rename") || !strcmp(m, "move")) && npos >= 1) {
        const char *to = rk_io_is(pos[0]) ? rk_io_abs(pos[0]) : rk_cstr(pos[0]);
        if (m[0] == 'c') {
            if (rk_io_flag(kv, nkv, "createonly", 0) && access(to, F_OK) == 0) { *out = FAILDESCR; return rk_io_fail("X::IO::Copy", "Failed to copy: destination exists"); }
            char *b;
            size_t n;
            if (!rk_io_text_of(d, &b, &n)) {
                char mm[strlen(p) + 96];
                snprintf(mm, sizeof mm, "Failed to copy '%s': %s", rk_io_abs(d), rk_io_err(errno));
                rk_io_fail("X::IO::Copy", mm);
                *out = FAILDESCR;
                return 1;
            }
            FILE *f = fopen(to, "wb");
            if (!f) { char mm[strlen(to) + 96]; snprintf(mm, sizeof mm, "Failed to copy to '%s': %s", to, rk_io_err(errno)); rk_io_fail("X::IO::Copy", mm); *out = FAILDESCR; return 1; }
            fwrite(b, 1, n, f);
            fclose(f);
            *out = True;
            return 1;
        }
        if (rename(rk_io_abs(d), to) == 0) { *out = True; return 1; }
        char mm[strlen(p) + strlen(to) + 96];
        snprintf(mm, sizeof mm, "Failed to rename '%s' to '%s': %s", rk_io_abs(d), to, rk_io_err(errno));
        rk_io_fail("X::IO::Rename", mm);
        *out = FAILDESCR;
        return 1;
    }
    if (!strcmp(m, "chmod") && npos >= 1 && IS_INT_fn(pos[0])) { *out = chmod(rk_io_abs(d), (mode_t) pos[0].i) == 0 ? True : False; return 1; }
    if (!strcmp(m, "dir")) return rk_io_dir_list(d, kv, nkv, out);
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_proc_find_user(const char *nm) { extern int rt_proc_is_registered(const char *); return rt_proc_is_registered(nm); }
static int rk_io_name(const char *fn) {
    static const char *const nm[] = { "open", "close", "slurp", "spurt", "unlink", "mkdir", "rmdir", "copy", "rename", "move", "chmod", "dir", "chdir", "make-temp-file", "make-temp-dir", NULL };
    for (int i = 0; nm[i]; i++) if (!strcmp(fn, nm[i])) return 1;
    return 0;
}
static int rk_io_fn(const char *fn, DESCR_t *a, int n, DESCR_t *out) {
    DESCR_t *pos = a, *kv = NULL;
    int npos = 0;
    while (npos < n && !rk_is_pair(a[npos])) npos++;
    int nkv = 0;
    { DESCR_t *kk; int sp2 = rk_io_split("none", a + npos, n - npos, &kk); kv = kk; nkv = sp2 >> 16; }
    DESCR_t True = (DESCR_t){ .v = DT_BOOL, .i = 1 };
    if (!strcmp(fn, "open") && npos >= 1) { *out = rk_io_open(pos[0], kv, nkv); return 1; }
    if (!strcmp(fn, "close") && npos >= 1) { DESCR_t ar[2] = { pos[0], STRVAL((char *) "close") }; if (IS_FH_fn(pos[0])) return rk_io_fh_method("close", ar, 2, out); *out = True; return 1; }
    static const char *const pm[] = { "slurp", "spurt", "unlink", "mkdir", "rmdir", "copy", "rename", "move", "chmod", "dir", NULL };
    for (int i = 0; pm[i]; i++) if (!strcmp(fn, pm[i]) && npos >= 1) {
        if (!strcmp(fn, "slurp") && IS_FH_fn(pos[0])) { DESCR_t ar[2] = { pos[0], STRVAL((char *) "slurp") }; return rk_io_fh_method("slurp", ar, 2, out); }
        DESCR_t path = rk_io_is(pos[0]) ? pos[0] : rk_io_mk(rk_cstr(pos[0]));
        DESCR_t ar[2 + n];
        ar[0] = path;
        ar[1] = STRVAL((char *) fn);
        for (int k = 1; k < n; k++) ar[1 + k] = a[k];
        return rk_io_path_method(fn, ar, 1 + n, out);
    }
    if (!strcmp(fn, "dir") && npos == 0) { DESCR_t d = rk_io_mk("."); DESCR_t ar[2] = { d, STRVAL((char *) "dir") }; return rk_io_path_method("dir", ar, 2, out); }
    if (!strcmp(fn, "chdir") && npos >= 1) { *out = chdir(rk_io_arg(pos[0])) == 0 ? True : (DESCR_t){ .v = DT_BOOL, .i = 0 }; return 1; }
    if (!strcmp(fn, "make-temp-file") || !strcmp(fn, "make-temp-dir")) {
        char t[] = "/tmp/rk_tmp_XXXXXX";
        int isd = fn[10] == 'd';
        if (isd) { if (!mkdtemp(t)) return 0; } else { int fd = mkstemp(t); if (fd < 0) return 0; close(fd); }
        for (int i = 0; i + 1 < nkv; i += 2) if (!isd && !strcmp(rk_cstr(kv[i]), "content")) { FILE *f = fopen(t, "wb"); if (f) { out_write_descr(f, kv[i + 1], 0); fclose(f); } }
        *out = rk_io_mk(t);
        return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int bt___rk_named_call(DESCR_t *args, int nargs, DESCR_t *out, long *rq) {
    if (nargs >= 2) {
        extern const char *rt_proc_pname(const char *name, int k);
        char pb[256];
        const char *pname = to_cstring(args[0], pb, sizeof pb);
        int npos = IS_INT_fn(args[1]) ? (int)args[1].i : 0;
        if (!strcmp(pname, "hash") && !rt_proc_find_user(pname)) {
            DESCR_t h = TABLE_VAL(table_new());
            for (int i = 2 + npos; i + 1 < nargs; i += 2) h = rk_hash_store(h, args[i], args[i + 1]);
            for (int i = 0; i < npos && 2 + i < nargs; i++) { DESCR_t el = args[2 + i]; if (rk_is_pair(el)) { rk_av_t pp = rk_av(el); h = rk_hash_store(h, pp.el[0], pp.n > 1 ? pp.el[1] : NULVCL); } }
            *out = h;
            return 1;
        }
        if (rk_io_name(pname)) {
            DESCR_t ar[nargs];
            int an = 0;
            for (int i = 0; i < npos && 2 + i < nargs; i++) ar[an++] = args[2 + i];
            for (int i = 2 + npos; i + 1 < nargs; i += 2) { DESCR_t pr[2] = { args[i], args[i + 1] }; ar[an++] = rk_mk_pair(pr[0], pr[1]); }
            DESCR_t r;
            if (rk_io_fn(pname, ar, an, &r)) { *out = r; return 1; }
            *out = FAILDESCR;
            return 1;
        }
        if (npos < 0) npos = 0;
        if (!rt_proc_find_user(pname)) {
            int none = 0;
            const char *w = rk_multi_named_winner(pname, &args[2], npos, &args[2 + npos], nargs > 2 + npos ? (nargs - 2 - npos) / 2 : 0, &none);
            if (w) pname = w;
            else if (!none) {
                extern void rt_script_die_surface(const char *msg);
                char m[fmt_len("Cannot resolve caller %s(...); none of these signatures matches", pname)];
                snprintf(m, sizeof m, "Cannot resolve caller %s(...); none of these signatures matches", pname);
                rt_script_die_surface(m);
                *out = FAILDESCR;
                return 1;
            }
        }
        int np = 0;
        while (rt_proc_pname(pname, np)) np++;
        extern int rt_proc_named_rest(const char *name);
        int nrest = rt_proc_named_rest(pname);
        int nrx = nrest > 0 ? nrest - 1 : -1;
        int nsl = npos > np ? npos : np;
        if (nrx + 1 > nsl) nsl = nrx + 1;
        if (nsl < 1) nsl = 1;
        DESCR_t slots[nsl];
        for (int i = 0; i < nsl; i++) slots[i] = NULVCL;
        int maxslot = 0;
        DESCR_t hh = NULVCL;
        for (int i = 0; i < npos && (2 + i) < nargs; i++) { slots[i] = args[2 + i]; if (i + 1 > maxslot) maxslot = i + 1; }
        for (int i = 2 + npos; i + 1 < nargs; i += 2) {
            char kb[128];
            const char *k = to_cstring(args[i], kb, sizeof kb);
            int found = -1;
            for (int s = 0; s < np; s++) { if (s == nrx) continue; const char *pn = rt_proc_pname(pname, s); if (pn && k && !strcmp(pn, k)) { found = s; break; } }
            if (found < 0) { if (nrx < 0) continue; hh = rk_hash_store(hh, args[i], args[i + 1]); continue; }
            slots[found] = args[i + 1];
            if (found + 1 > maxslot) maxslot = found + 1;
        }
        if (nrx >= 0) { slots[nrx] = hh.v == DT_T ? hh : TABLE_VAL(table_new()); if (nrx + 1 > maxslot) maxslot = nrx + 1; }
        int total = np > maxslot ? np : maxslot;
        *out = invoke_method_proc_rq(pname, slots, total, rq);
        return 1;
    }
    return 0;
}
DESCR_t bt___rk_rep(DESCR_t *args, int nargs) {
    if (nargs == 2) {
        char sb[256];
        const char *src = to_cstring(args[0], sb, sizeof sb);
        long long n = rk_ival(args[1]);
        if (!src) src = "";
        if (n < 0) n = 0;
        size_t L = strlen(src);
        char *buf = rt_wsb_alloc(L * (size_t)(n > 0 ? n : 0) + 1);
        size_t p = 0;
        for (long long k = 0; k < n; k++) { memcpy(buf + p, src, L); p += L; }
        buf[p] = '\0';
        return STRVAL(buf);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_pre_src(DESCR_t *args, int nargs) {
    if (nargs == 2) { const char *p = rk_cstr(args[1]); return !strcmp(rk_cstr(args[0]), "*PROGRAM") ? rk_io_mk(p) : STRVAL(rt_heap_strdup_c(p)); }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_val(DESCR_t *args, int nargs) {
    if (nargs >= 1) {
        const char *v = rk_cstr(args[0]);
        char *ep;
        long long iv = strtoll(v, &ep, 10);
        if (*v && !*ep) { return INTVAL(iv); }
        double dv = strtod(v, &ep);
        if (*v && !*ep && !isspace((unsigned char) v[0])) { return REALVAL(dv); }
        return args[0];
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_io(DESCR_t *args, int nargs) {
    DESCR_t _bo = FAILDESCR;
    DESCR_t *const out = &_bo;
    if (nargs >= 1) { const char *nm = rk_cstr(args[0]); if (rk_io_fn(nm, args + 1, nargs - 1, out)) return _bo; return FAILDESCR; }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
