#define _GNU_SOURCE
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <grp.h>
#include <pwd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "driver/driver_data.h"
#include "ir/descr.h"
#include "posix_leaves.h"
#include "runtime/core/g_runtime.h"
#include "runtime/rt/rt_arena.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void px_begin(void) { g_runtime.misc.posix_errno = 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t px_fail(void) { g_runtime.misc.posix_errno = errno; return FAILDESCR; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *px_str(DESCR_t d) { return d.v == DT_S ? rt_cstr_d(d) : NULL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t px_rec(const char *tn, DESCR_t *f, int n) { DatType *t = dat_find_type(tn); return t ? dat_construct(t, f, n) : FAILDESCR; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t px_fmt(const char *fmt, ...) {
    va_list a, b;
    va_start(a, fmt);
    va_copy(b, a);
    int n = vsnprintf(NULL, 0, fmt, a);
    char *s = (char *) rt_heap_alloc_c((size_t) n + 1);
    va_end(a);
    vsnprintf(s, (size_t) n + 1, fmt, b);
    va_end(b);
    return STRVAL(s);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t px_list(DESCR_t *f, int n) { extern DESCR_t rt_make_list(DESCR_t *, int); return rt_make_list(f, n); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t px_mask_str(mode_t perms) { char *s = rt_heap_strdup_c("---------"); for (int i = 0; i < 9; i++) if (perms & (0400 >> i)) s[i] = "rwxrwxrwx"[i]; return STRVAL(s); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int px_mask_parse(const char *s, mode_t *perms) {
    mode_t m = 0;
    if (strlen(s) != 9) return 0;
    for (int i = 0; i < 9; i++) { if (s[i] == "rwxrwxrwx"[i]) m |= 0400 >> i; else if (s[i] != '-') return 0; }
    *perms = m;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int px_chmod_sym(const char *s, mode_t cur, mode_t *out) {
    mode_t m = cur, um = umask(0);
    umask(um);
    for (;;) {
        mode_t who = 0, bits = 0;
        char op;
        for (; *s == 'u' || *s == 'g' || *s == 'o' || *s == 'a'; s++) who |= *s == 'u' ? 0700 : *s == 'g' ? 070 : *s == 'o' ? 07 : 0777;
        if (*s != '+' && *s != '-' && *s != '=') return 0;
        op = *s++;
        for (; *s && *s != ','; s++) { if (*s == 'r') bits |= 0444; else if (*s == 'w') bits |= 0222; else if (*s == 'x') bits |= 0111; else return 0; }
        if (!who) who = 0777 & ~um;
        bits &= who;
        m = op == '+' ? (m | bits) : op == '-' ? (m & ~bits) : ((m & ~who) | bits);
        if (*s != ',') break;
        s++;
    }
    *out = m;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t bt_ICNx24UMKDIR(DESCR_t *args, int nargs) {
    const char *p = nargs >= 1 ? px_str(args[0]) : NULL;
    px_begin();
    if (!p) return FAILDESCR;
    return mkdir(p, nargs >= 2 && args[1].v == DT_I ? (mode_t) args[1].i : 0777) == 0 ? NULVCL : px_fail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t bt_ICNx24URMDIR(DESCR_t *args, int nargs) { const char *p = nargs == 1 ? px_str(args[0]) : NULL; px_begin(); if (!p) return FAILDESCR; return rmdir(p) == 0 ? NULVCL : px_fail(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t bt_ICNx24UCHMOD(DESCR_t *args, int nargs) {
    const char *p = nargs == 2 ? px_str(args[0]) : NULL;
    struct stat st;
    mode_t m = 0, perms;
    px_begin();
    if (!p) return FAILDESCR;
    if (args[1].v == DT_I) m = (mode_t) args[1].i;
    else {
        const char *spec = px_str(args[1]);
        if (!spec) return FAILDESCR;
        if (stat(p, &st) != 0) return px_fail();
        if (px_mask_parse(spec, &perms)) m = perms;
        else if (!px_chmod_sym(spec, st.st_mode & 07777, &m)) return FAILDESCR;
    }
    return chmod(p, m) == 0 ? NULVCL : px_fail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t bt_ICNx24UUMASK(DESCR_t *args, int nargs) {
    mode_t cur = umask(0), perms;
    const char *s;
    px_begin();
    umask(cur);
    if (nargs == 0) return px_mask_str(0777 & ~cur);
    s = px_str(args[0]);
    if (!s || !px_mask_parse(s, &perms)) return FAILDESCR;
    umask(0777 & ~perms);
    return px_mask_str(0777 & ~cur);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t bt_ICNx24USETENV(DESCR_t *args, int nargs) {
    const char *n = nargs == 2 ? px_str(args[0]) : NULL, *v = nargs == 2 ? px_str(args[1]) : NULL;
    px_begin();
    if (!n || !v) return FAILDESCR;
    return setenv(n, v, 1) == 0 ? NULVCL : px_fail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t px_cwd(void) {
    for (size_t n = 256; n < ((size_t) 1 << 30); n *= 2) { char *b = (char *) rt_heap_alloc_c(n); if (getcwd(b, n)) return STRVAL(rt_heap_strdup_c(b)); if (errno != ERANGE) break; }
    return px_fail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t bt_ICNx24UCHDIR(DESCR_t *args, int nargs) {
    const char *p = nargs == 1 ? px_str(args[0]) : NULL;
    px_begin();
    if (nargs == 1 && (!p || chdir(p) != 0)) return p ? px_fail() : FAILDESCR;
    return px_cwd();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t bt_ICNx24USYS_ERRSTR(DESCR_t *args, int nargs) { return nargs == 1 && args[0].v == DT_I ? STRVAL(rt_heap_strdup_c(strerror((int) args[0].i))) : FAILDESCR; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t bt_ICNx24UGETPID(DESCR_t *args, int nargs) { (void) args; return nargs == 0 ? INTVAL((int64_t) getpid()) : FAILDESCR; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t bt_ICNx24UGETPPID(DESCR_t *args, int nargs) { (void) args; return nargs == 0 ? INTVAL((int64_t) getppid()) : FAILDESCR; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t bt_ICNx24UGETPGRP(DESCR_t *args, int nargs) { (void) args; return nargs == 0 ? INTVAL((int64_t) getpgrp()) : FAILDESCR; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t px_user(uid_t u) { struct passwd *pw = getpwuid(u); return pw ? STRVAL(rt_heap_strdup_c(pw->pw_name)) : INTVAL((int64_t) u); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t px_group(gid_t g) { struct group *gr = getgrgid(g); return gr ? STRVAL(rt_heap_strdup_c(gr->gr_name)) : INTVAL((int64_t) g); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t bt_ICNx24UGETUID(DESCR_t *args, int nargs) { (void) args; return nargs == 0 ? px_user(getuid()) : FAILDESCR; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t bt_ICNx24UGETEUID(DESCR_t *args, int nargs) { (void) args; return nargs == 0 ? px_user(geteuid()) : FAILDESCR; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t bt_ICNx24UGETGID(DESCR_t *args, int nargs) { (void) args; return nargs == 0 ? px_group(getgid()) : FAILDESCR; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t bt_ICNx24UGETEGID(DESCR_t *args, int nargs) { (void) args; return nargs == 0 ? px_group(getegid()) : FAILDESCR; }
