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
static int px_newmode(const char *mode0, int oldmode) {
    char *mode = rt_heap_strdup_c(mode0), *p = mode, op;
    const char *fields = "ogua", *q;
    int which = 0, do_umask, retmode = oldmode & 07777, mlen = (int) strlen(mode), i;
    if (mlen == 9 || (mlen == 10 && strchr("-ldcb|s", mode[0]))) {
        char *m = mlen == 10 ? mode + 1 : mode;
        int hb = 0, cm = 0;
        if (m[2] == 's' || m[2] == 'S') { hb |= 1; m[2] = m[2] == 's' ? 'x' : '-'; }
        hb <<= 1;
        if (m[5] == 's' || m[5] == 'S') { hb |= 1; m[5] = m[5] == 's' ? 'x' : '-'; }
        hb <<= 1;
        if (m[8] == 't' || m[8] == 'T') { hb |= 1; m[8] = m[8] == 't' ? 'x' : '-'; }
        for (i = 0; i < 9; i++) { cm <<= 1; if (m[i] == '-') cm |= 1; else if (m[i] != "rwxrwxrwx"[i]) break; }
        if (i == 9) return cm | hb << 9;
    }
    for (; (q = strchr(fields, *p)); p++) { if (!*p) return -2; which = *p == 'a' ? 7 : which | 1 << (q - fields); }
    if (!strchr("+=-", *p)) return -2;
    if ((do_umask = which == 0)) which = 7;
    op = *p++;
    if (op == '=') { for (i = 0; i < 3; i++) if (which & (1 << i)) retmode &= ~(7 << (i * 3)) & ~(1 << (i + 9)); op = '+'; }
    while (*p) {
        int value = 0;
        switch (*p++) {
            case 'r':
            value = 4;
            break;
            case 'w':
            value = 2;
            break;
            case 'x':
            value = 1;
            break;
            case 'R':
            if (oldmode & 0444) value = 4;
            break;
            case 'W':
            if (oldmode & 0222) value = 2;
            break;
            case 'X':
            if (oldmode & 0111) value = 1;
            break;
            case 'u':
            value = (oldmode & 0700) >> 6;
            break;
            case 'g':
            value = (oldmode & 0070) >> 3;
            break;
            case 'o':
            value = oldmode & 0007;
            break;
            case 's':
            value = ((which & 4) ? 04000 : 0) | ((which & 2) ? 02000 : 0);
            retmode |= value;
            continue;
            case 't':
            if (which & 1) retmode |= 01000;
            continue;
            default:
            return -2;
        }
        for (i = 0; i < 3; i++) {
            int nvalue = value;
            if (which & (1 << i)) { if (do_umask) { mode_t u = umask(0); umask(u); nvalue = value & ~(int) u; } if (op == '-') retmode &= ~nvalue; else if (op == '+') retmode |= nvalue; }
            value <<= 3;
        }
    }
    return retmode;
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
    mode_t m = 0;
    px_begin();
    if (!p) return FAILDESCR;
    if (args[1].v == DT_I) m = (mode_t) args[1].i;
    else { const char *spec = px_str(args[1]); if (!spec) return FAILDESCR; int nm = px_newmode(spec, stat(p, &st) == 0 ? (int) st.st_mode : 0); if (nm < 0) return FAILDESCR; m = (mode_t) nm; }
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
