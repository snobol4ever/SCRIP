#define _GNU_SOURCE
#include <arpa/inet.h>
#include <grp.h>
#include <netdb.h>
#include <pwd.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <unistd.h>
#include "posix_leaves.h"
#include "runtime/rt/rt_arena.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t pi_str(const char *s) { return STRVAL(rt_heap_strdup_c(s ? s : "")); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t pi_join(char **v) {
    size_t n = 0;
    for (int i = 0; v && v[i]; i++) n += strlen(v[i]) + 1;
    char *b = rt_heap_alloc_c(n + 1), *p = b;
    for (int i = 0; v && v[i]; i++) { if (i) *p++ = ','; p = stpcpy(p, v[i]); }
    *p = 0;
    return STRVAL(b);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t pi_passwd(const struct passwd *pw) {
    DESCR_t f[7] = { pi_str(pw->pw_name), pi_str(pw->pw_passwd), INTVAL(pw->pw_uid), INTVAL(pw->pw_gid), pi_str(pw->pw_gecos), pi_str(pw->pw_dir), pi_str(pw->pw_shell) };
    return px_rec("posix_passwd", f, 7);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t bt_ICNx24UGETPW(DESCR_t *args, int nargs) {
    struct passwd *pw;
    px_begin();
    if (nargs == 0 || IS_NULL_fn(args[0])) pw = getpwent();
    else if (args[0].v == DT_I) pw = getpwuid((uid_t) args[0].i);
    else if (args[0].v == DT_S) pw = getpwnam(rt_cstr_d(args[0]));
    else return FAILDESCR;
    return pw ? pi_passwd(pw) : FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t bt_ICNx24UGETGR(DESCR_t *args, int nargs) {
    struct group *gr;
    px_begin();
    if (nargs == 0 || IS_NULL_fn(args[0])) gr = getgrent();
    else if (args[0].v == DT_I) gr = getgrgid((gid_t) args[0].i);
    else if (args[0].v == DT_S) gr = getgrnam(rt_cstr_d(args[0]));
    else return FAILDESCR;
    if (!gr) return FAILDESCR;
    DESCR_t f[4] = { pi_str(gr->gr_name), pi_str(gr->gr_passwd), INTVAL(gr->gr_gid), pi_join(gr->gr_mem) };
    return px_rec("posix_group", f, 4);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t bt_ICNx24UGETSERV(DESCR_t *args, int nargs) {
    struct servent *sv;
    const char *proto = nargs >= 2 && args[1].v == DT_S ? rt_cstr_d(args[1]) : NULL;
    px_begin();
    if (proto && !getprotobyname(proto)) return FAILDESCR;
    if (nargs == 0 || IS_NULL_fn(args[0])) sv = getservent();
    else if (args[0].v == DT_I) sv = getservbyport((int) htons((unsigned short) args[0].i), proto);
    else if (args[0].v == DT_S) sv = getservbyname(rt_cstr_d(args[0]), proto);
    else return FAILDESCR;
    if (!sv) return FAILDESCR;
    DESCR_t f[4] = { pi_str(sv->s_name), pi_join(sv->s_aliases), INTVAL((int64_t) ntohs((unsigned short) sv->s_port)), pi_str(sv->s_proto) };
    return px_rec("posix_servent", f, 4);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t pi_timeval(long sec, long usec) { DESCR_t f[2] = { INTVAL(sec), INTVAL(usec) }; return px_rec("posix_timeval", f, 2); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t bt_ICNx24UGETTIMEOFDAY(DESCR_t *args, int nargs) {
    struct timeval tv;
    (void) args;
    px_begin();
    if (nargs != 0 || gettimeofday(&tv, NULL) != 0) return nargs ? FAILDESCR : px_fail();
    return pi_timeval(tv.tv_sec, tv.tv_usec);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t bt_ICNx24UGETRUSAGE(DESCR_t *args, int nargs) {
    struct rusage u;
    const char *who = nargs >= 1 && args[0].v == DT_S ? rt_cstr_d(args[0]) : "self";
    int iwho;
    px_begin();
    if (!*who || !strcmp(who, "self")) iwho = RUSAGE_SELF;
    else if (!strcmp(who, "thread")) iwho = RUSAGE_THREAD;
    else if (!strcmp(who, "children")) iwho = RUSAGE_CHILDREN;
    else return FAILDESCR;
    if (getrusage(iwho, &u) != 0) return px_fail();
    DESCR_t f[9] = { pi_timeval(u.ru_utime.tv_sec, u.ru_utime.tv_usec), pi_timeval(u.ru_stime.tv_sec, u.ru_stime.tv_usec), INTVAL(u.ru_maxrss), INTVAL(u.ru_minflt), INTVAL(u.ru_majflt),
        INTVAL(u.ru_inblock), INTVAL(u.ru_oublock), INTVAL(u.ru_nvcsw), INTVAL(u.ru_nivcsw) };
    return px_rec("posix_rusage", f, 9);
}
