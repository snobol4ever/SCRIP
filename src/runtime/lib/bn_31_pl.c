#include "runtime/lib/bn_head.h"
#include "runtime/lib/bn_head_pl_cx_leaf_head.h"
#include "runtime/lib/bn_head_pl_bt.h"
#include "runtime/lib/bn_head_pl_meta_row.h"
#include <poll.h>
#include <sys/stat.h>
#include <utime.h>
#include "runtime/rt/rt_arena.h"
#include "runtime/rt/prolog_atom.h"
#include "runtime/core/runtime_shim.h"
#include "driver/driver_globals.h"
#include <errno.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include "runtime/lib/bn_05_pl.h"
#include "runtime/lib/bn_06_pl.h"
#include "runtime/lib/bn_11_pl.h"
#include "runtime/lib/bn_27_pl.h"
#include "runtime/lib/bn_29_pl.h"
#include "runtime/lib/bn_30_pl.h"
#include "runtime/lib/bn_31_pl.h"
#include "runtime/pl_meta_specs.inc"
#include "runtime/core/g_runtime_bn.h"
#include <time.h>
#include "runtime/core/core_etc.h"
#include "runtime/rt/gc_heap_hb_aggb.h"
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
#include "runtime/rt/rt_pl_trail_pl_tr_ctx_s.h"
#include "runtime/rt/rt_pl_trail.h"
#include "ir/descr_tags_dt_platom.h"
extern void *rt_ws_alloc_descr(size_t);
extern int rt_proc_is_registered(const char *name);
extern int fh_capture_begin(char **, size_t *, int *);
extern void fh_capture_end(int, int);
extern int fh_capture_begin(char **, size_t *, int *);
extern void fh_capture_end(int, int);
extern int fh_capture_begin(char **, size_t *, int *);
extern void fh_capture_end(int, int);
extern const char *rt_pl_db_cp_nth(void *, int64_t, int *);
DESCR_t pl_mk_atom_dup(const char *s, size_t n) {
    extern int prolog_atom_intern_n(const char *, size_t);
    DESCR_t d = {0};
    d.v = DT_PLATOM;
    d.slen = 0;
    d.i = prolog_atom_intern_n(s, n);
    return d;
}
DESCR_t pl_text_list(const char *s, int codes) {
    gv_t el;
    int n = 0;
    el.p = 0;
    el.len = 0;
    el.cap = 0;
    el.esz = 0;
    el.kind = 0;
    el.pad = 0;
    for (; *s; s++) {
        DESCR_t a;
        if (codes) a = INTVAL((long long)(unsigned char)*s);
        else { char c2[2] = { *s, 0 }; a = pl_mk_atom_dup(c2, 1); }
        *(DESCR_t *)gv_push(&el, (uint16_t)HB_DVEC, (uint32_t)sizeof(DESCR_t), "pl_text_list") = a;
        n++;
    }
    return pl_list_from_arr((DESCR_t *)el.p, n);
}
DESCR_t pl_text_dlist(const char *s, size_t len, int codes, DESCR_t tail) {
    DESCR_t acc = tail;
    for (size_t i = len; i > 0; i--) { DESCR_t e; if (codes) e = INTVAL((long long)(unsigned char)s[i - 1]); else { char c2[2] = { s[i - 1], 0 }; e = pl_mk_atom_dup(c2, 1); } acc = pl_cons(e, acc); }
    return acc;
}
PL_CX_LEAF_HEAD(name, 2)
    {
        extern void *rt_pl_ball_instantiation(void);
        extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
        char b[4096];
        const char *s;
        DESCR_t a = rt_pl_deref_val(args[0]);
        if (a.v == DT_I || a.v == DT_R || a.v == DT_BIG || pl_atom_str(a)) {
            pl_cell_text(a, b, sizeof b, &s);
            ok = plw_unify_vals(args[1], pl_text_list(s, 1), cx);
        } else if ((int)a.v == DT_PLREF) {
            cx->ball = rt_pl_ball_kind2("type_error", "atomic", a);
            ok = 0;
        } else {
            DESCR_t num;
            const char *t = pl_list_text_heap(args[1]);
            if (!t) {
                ok = 0;
                if (pl_val_unbound(rt_pl_deref_val(args[1]))) cx->ball = rt_pl_ball_instantiation();
            } else ok = plw_unify_vals(args[0], pl_parse_number(t, &num) && !(pl_swi_dialect() && isspace((unsigned char)t[0])) ? num : pl_mk_atom_dup(t, strlen(t)), cx);
        }
    }
PL_CX_LEAF_TAIL
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
PL_CX_LEAF_HEAD(telling, 1)
    {
        extern int fh_current_output(void);
        ok =
            plw_unify_vals(args[0],
            fh_current_output() == 1 ? pl_mk_atom("user") : !g_rt_bn.pl_edin_out_name[0] ? pl_mk_stream(fh_current_output()) :
            pl_mk_atom_dup(g_rt_bn.pl_edin_out_name, strlen(g_rt_bn.pl_edin_out_name)), cx);
    }
PL_CX_LEAF_TAIL
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
PL_CX_LEAF_HEAD(seeing, 1)
    {
        extern int fh_current_input(void);
        ok =
            plw_unify_vals(args[0],
            fh_current_input() == 0 ? pl_mk_atom("user") : !g_rt_bn.pl_edin_in_name[0] ? pl_mk_stream(fh_current_input()) :
            pl_mk_atom_dup(g_rt_bn.pl_edin_in_name, strlen(g_rt_bn.pl_edin_in_name)), cx);
    }
PL_CX_LEAF_TAIL
DESCR_t pl_gnu_fn(const char *f, int ar, const DESCR_t *a) {
    extern int prolog_atom_intern(const char *);
    DESCR_t *kids = (DESCR_t *)rt_ws_alloc_descr(ar);
    DESCR_t c = {0};
    for (int i = 0; i < ar; i++) kids[i] = a[i];
    c.v = (DTYPE_t)DT_PLREF;
    c.slen = (uint32_t)prolog_functor_intern(prolog_atom_intern(f), ar);
    c.p = (void *)kids;
    return c;
}
DESCR_t pl_gnu_fn1(const char *f, DESCR_t a) { return pl_gnu_fn(f, 1, &a); }
DESCR_t pl_gnu_dt(time_t t) {
    struct tm tm;
    DESCR_t a[6];
    localtime_r(&t, &tm);
    a[0] = INTVAL(tm.tm_year + 1900);
    a[1] = INTVAL(tm.tm_mon + 1);
    a[2] = INTVAL(tm.tm_mday);
    a[3] = INTVAL(tm.tm_hour);
    a[4] = INTVAL(tm.tm_min);
    a[5] = INTVAL(tm.tm_sec);
    return pl_gnu_fn("dt", 6, a);
}
static DESCR_t *pl_gnu_vec_push(gv_t *el) { return (DESCR_t *)gv_push(el, (uint16_t)HB_DVEC, (uint32_t)sizeof(DESCR_t), "gnu_os_list"); }
static void pl_bt_visit(const void *map, const char *base, void *a) {
    pl_bt_t *b = (pl_bt_t *)a;
    const gc_frame_map_t *m = (const gc_frame_map_t *)map;
    (void)base;
    if (!m || !m->graph_name || !strchr(m->graph_name, '/')) return;
    if (b->nm && b->n < b->cap) b->nm[b->n] = m->graph_name;
    b->n++;
}
PL_CX_LEAF_HEAD(pl_backtrace, 2)
    {
        DESCR_t mx = rt_pl_deref_val(args[0]);
        pl_bt_t c = { (const char **)0, 0, 0 };
        int n;
        (void)rt_gc_frames_visit(pl_bt_visit, &c);
        n = c.n;
        if (mx.v == DT_I && mx.i >= 0 && mx.i < n) n = (int)mx.i;
        {
            const char *nm[n > 0 ? n : 1];
            DESCR_t fr[n > 0 ? n : 1];
            pl_bt_t f = { nm, 0, n };
            (void)rt_gc_frames_visit(pl_bt_visit, &f);
            if (f.n < n) n = f.n;
            for (int i = 0; i < n; i++) {
                DESCR_t at = pl_mk_atom_dup(nm[i], strlen(nm[i])), a3[3];
                a3[0] = INTVAL(i + 1);
                a3[1] = pl_gnu_fn1("call", at);
                a3[2] = at;
                fr[i] = pl_gnu_fn("frame", 3, a3);
            }
            ok = plw_unify_vals(args[1], pl_list_from_arr(fr, n), cx);
        }
    }
PL_CX_LEAF_TAIL
DESCR_t pl_fx_err(void) {
    int e = errno;
    DESCR_t a[2] = { pl_mk_atom(e == ENOENT ? "enoent" : (e == EACCES || e == EPERM) ? "eperm" : "other"), pl_mk_atom(strerror(e)) };
    return pl_gnu_fn("e", 2, a);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_ct_n(DESCR_t t) { return t.v == (DTYPE_t)DT_PLREF ? plc_fid_arity(t.slen) : 0; }
DESCR_t pl_os_err_of(int e) {
    extern const char *strerrorname_np(int);
    const char *n = strerrorname_np(e);
    size_t k = n ? strlen(n) : 0;
    char lb[k + 9];
    DESCR_t a[2];
    if (!n) { memcpy(lb, "eunknown", 9); k = 8; } else { for (size_t i = 0; i < k; i++) lb[i] = (char)tolower((unsigned char)n[i]); lb[k] = 0; }
    a[0] = pl_mk_atom_dup(lb, k);
    a[1] = pl_mk_atom(strerror(e));
    return pl_gnu_fn("$oserr", 2, a);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_sk_idx(DESCR_t s) {
    DESCR_t k;
    s = rt_pl_deref_val(s);
    if (s.v != (DTYPE_t)DT_PLREF || plc_fid_arity(s.slen) != 1 || plc_fid_name(s.slen) != prolog_atom_intern("$sock")) return -1;
    k = rt_pl_deref_val(((DESCR_t *)s.p)[0]);
    return (k.v == (DTYPE_t)DT_I && k.i >= 3 && k.i < FH_N && g_fh[k.i].fp && g_fh[k.i].tsk == 's') ? (int)k.i : -1;
}
int pl_sk_fd(DESCR_t s) { int i = pl_sk_idx(s); if (i < 0) { errno = EBADF; return -1; } return fileno(g_fh[i].fp); }
static DESCR_t pl_sk_new(int fd) {
    extern int fh_alloc(FILE *);
    FILE *fp = fdopen(fd, "r+");
    int idx, e;
    DESCR_t v[1];
    if (!fp) { e = errno; close(fd); errno = e; return FAILDESCR; }
    idx = fh_alloc(fp);
    g_fh[idx].tsk = 's';
    v[0] = INTVAL(idx);
    return pl_gnu_fn("$sock", 1, v);
}
static DESCR_t pl_sk_gai_err(int rc) {
    DESCR_t a[2];
    a[0] = pl_mk_atom(rc == EAI_NONAME ? "eai_noname" : rc == EAI_AGAIN ? "eai_again" : rc == EAI_FAIL ? "eai_fail" : "eai_system");
    a[1] = pl_mk_atom(gai_strerror(rc));
    return pl_gnu_fn("$oserr", 2, a);
}
static int pl_sk_addr(DESCR_t a, struct sockaddr_storage *ss, socklen_t *ln, int bnd, DESCR_t *err) {
    DESCR_t x, pt;
    const char *h;
    struct sockaddr_in *in = (struct sockaddr_in *)ss;
    struct addrinfo hints, *res = (struct addrinfo *)0;
    int rc;
    a = rt_pl_deref_val(a);
    memset(ss, 0, sizeof *ss);
    *err = pl_os_err_of(EINVAL);
    if (pl_ct_n(a) == 1) {
        if (pl_gs_unix_addr((struct sockaddr_un *)ss, pl_atom_str(pl_ct_a(a, 0))) != 0) { *err = pl_os_err_of(errno); return 0; }
        *ln = (socklen_t)sizeof(struct sockaddr_un);
        return 1;
    }
    if (pl_ct_n(a) != 2) return 0;
    x = pl_ct_a(a, 0);
    pt = pl_ct_a(a, 1);
    h = pl_atom_str(x);
    if (pt.v != (DTYPE_t)DT_I) return 0;
    *ln = (socklen_t)sizeof *in;
    in->sin_family = AF_INET;
    in->sin_port = htons((unsigned short)pt.i);
    if (bnd && (!h || !*h)) { in->sin_addr.s_addr = htonl(INADDR_ANY); return 1; }
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    if ((rc = getaddrinfo(h ? h : "", (const char *)0, &hints, &res)) != 0 || !res) { *err = pl_sk_gai_err(rc ? rc : EAI_NONAME); return 0; }
    in->sin_addr = ((struct sockaddr_in *)res->ai_addr)->sin_addr;
    freeaddrinfo(res);
    return 1;
}
static DESCR_t pl_sk_ip(const struct in_addr *a) {
    const unsigned char *b = (const unsigned char *)a;
    DESCR_t v[4] = { INTVAL(b[0]), INTVAL(b[1]), INTVAL(b[2]), INTVAL(b[3]) };
    return pl_gnu_fn("ip", 4, v);
}
PL_CX_LEAF_HEAD(sk_socket, 2)
    {
        const char *d = pl_atom_str(rt_pl_deref_val(args[0]));
        int u = d && !strcmp(d, "unix"), g = d && !strcmp(d, "udp");
        int fd = socket(u ? AF_UNIX : AF_INET, (g ? SOCK_DGRAM : SOCK_STREAM) | SOCK_CLOEXEC, 0);
        DESCR_t s;
        if (fd < 0) ok = pl_os_errv(args[1], cx);
        else { s = pl_sk_new(fd); ok = IS_FAIL_fn(s) ? pl_os_errv(args[1], cx) : plw_unify_vals(args[1], s, cx); }
    }
PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(sk_close, 2)
    {
        int i = pl_sk_idx(args[0]);
        FILE *fp;
        if (i < 0) { errno = EBADF; ok = pl_os_errv(args[1], cx); } else { fp = g_fh[i].fp; fh_free(i); ok = fclose(fp) == 0 ? pl_os_okv(args[1], cx) : pl_os_errv(args[1], cx); }
    }
PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(sk_bind, 3)
    {
        int fd = pl_sk_fd(args[0]);
        struct sockaddr_storage ss;
        struct sockaddr_in in;
        socklen_t ln = 0, l = (socklen_t)sizeof in;
        DESCR_t ae;
        if (fd < 0) ok = pl_os_errv(args[2], cx);
        else if (!pl_sk_addr(args[1], &ss, &ln, 1, &ae)) ok = plw_unify_vals(args[2], ae, cx);
        else if (bind(fd, (struct sockaddr *)&ss, ln) != 0) ok = pl_os_errv(args[2], cx);
        else if (ss.ss_family != AF_INET) ok = pl_os_okv(args[2], cx);
        else ok = getsockname(fd, (struct sockaddr *)&in, &l) == 0 ? plw_unify_vals(args[2], INTVAL(ntohs(in.sin_port)), cx) : pl_os_errv(args[2], cx);
    }
PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(sk_accept, 2)
    {
        int fd = pl_sk_fd(args[0]), c = -1, up = 0;
        struct sockaddr_storage ss;
        socklen_t l = (socklen_t)sizeof ss;
        DESCR_t v[2];
        ok = 1;
        if (fd >= 0) {
            for (;;) {
                c = accept(fd, (struct sockaddr *)&ss, &l);
                if (c >= 0) fcntl(c, F_SETFD, FD_CLOEXEC);
                if (c >= 0 || errno != EINTR) break;
                if (pl_alarm_take(cx)) { up = 1; break; }
                l = (socklen_t)sizeof ss;
            }
        }
        if (up) ok = 0;
        else if (c < 0) ok = pl_os_errv(args[1], cx);
        else {
            v[0] = pl_sk_new(c);
            v[1] = ss.ss_family == AF_INET ? pl_sk_ip(&((struct sockaddr_in *)&ss)->sin_addr) : pl_mk_atom("");
            ok = IS_FAIL_fn(v[0]) ? pl_os_errv(args[1], cx) : plw_unify_vals(args[1], pl_gnu_fn("acc", 2, v), cx);
        }
    }
PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(sk_connect, 3)
    {
        int fd = pl_sk_fd(args[0]), rc = -1, up = 0, se = 0;
        struct sockaddr_storage ss;
        socklen_t ln = 0, sl = (socklen_t)sizeof se;
        DESCR_t ae;
        struct pollfd pf;
        ok = 1;
        if (fd < 0) ok = pl_os_errv(args[2], cx);
        else if (!pl_sk_addr(args[1], &ss, &ln, 0, &ae)) ok = plw_unify_vals(args[2], ae, cx);
        else {
            rc = connect(fd, (struct sockaddr *)&ss, ln);
            if (rc != 0 && errno == EINTR) {
                for (;;) { pf.fd = fd; pf.events = POLLOUT; pf.revents = 0; if (poll(&pf, 1, -1) >= 0) break; if (errno != EINTR) break; if (pl_alarm_take(cx)) { up = 1; break; } }
                if (!up && getsockopt(fd, SOL_SOCKET, SO_ERROR, &se, &sl) == 0) { rc = se ? -1 : 0; if (se) errno = se; }
            }
            if (up) ok = 0;
            else ok = rc == 0 ? pl_os_okv(args[2], cx) : pl_os_errv(args[2], cx);
        }
    }
PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(sk_open, 2)
    {
        int fd = pl_sk_fd(args[0]), a = -1, b = -1;
        DESCR_t v[2];
        if (fd >= 0) { a = fcntl(fd, F_DUPFD_CLOEXEC, 3); b = a >= 0 ? fcntl(fd, F_DUPFD_CLOEXEC, 3) : -1; }
        if (a < 0 || b < 0) {
            if (a >= 0) close(a);
            ok = pl_os_errv(args[1], cx);
        } else {
            v[0] = pl_gs_fdstream(a, "r");
            v[1] = pl_gs_fdstream(b, "w");
            ok = plw_unify_vals(args[1], pl_gnu_fn("$stream_pair", 2, v), cx);
        }
    }
PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(sk_setopt, 4)
    {
        int fd = pl_sk_fd(args[0]), lv = SOL_SOCKET, nm = -1, v;
        const char *o = pl_atom_str(rt_pl_deref_val(args[1]));
        DESCR_t n = rt_pl_deref_val(args[2]);
        v = n.v == (DTYPE_t)DT_I ? (int)n.i : 1;
        if (o && !strcmp(o, "reuseaddr")) nm = SO_REUSEADDR;
        else if (o && !strcmp(o, "nodelay")) { lv = IPPROTO_TCP; nm = TCP_NODELAY; } else if (o && !strcmp(o, "broadcast")) nm = SO_BROADCAST;
        else if (o && !strcmp(o, "keepalive")) nm = SO_KEEPALIVE;
        else if (o && !strcmp(o, "sndbuf")) nm = SO_SNDBUF;
        else if (o && !strcmp(o, "rcvbuf")) nm = SO_RCVBUF;
        if (fd >= 0 && nm < 0) errno = EINVAL;
        ok = (fd >= 0 && nm >= 0 && setsockopt(fd, lv, nm, &v, sizeof v) == 0) ? pl_os_okv(args[3], cx) : pl_os_errv(args[3], cx);
    }
PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(sk_fileno, 2) { int fd = pl_sk_fd(args[0]); ok = fd >= 0 && plw_unify_vals(args[1], INTVAL(fd), cx); } PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(sk_hostname, 1)
    {
        long hm = sysconf(_SC_HOST_NAME_MAX);
        size_t n = (size_t)(hm > 0 ? hm : 255) + 1;
        char h[n];
        ok = gethostname(h, n) == 0;
        if (ok) { h[n - 1] = 0; ok = plw_unify_vals(args[0], pl_mk_atom_dup(h, strlen(h)), cx); }
    }
PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(sk_resolve, 2)
    {
        DESCR_t x = rt_pl_deref_val(args[0]);
        struct addrinfo hints, *res = (struct addrinfo *)0;
        int rc;
        if (pl_ct_n(x) == 4) {
            struct sockaddr_in in;
            char hb[NI_MAXHOST];
            unsigned char *b = (unsigned char *)&in.sin_addr;
            memset(&in, 0, sizeof in);
            in.sin_family = AF_INET;
            for (int i = 0; i < 4; i++) b[i] = (unsigned char)pl_ct_a(x, i).i;
            rc = getnameinfo((struct sockaddr *)&in, sizeof in, hb, sizeof hb, (char *)0, 0, NI_NAMEREQD);
            ok = rc == 0 ? plw_unify_vals(args[1], pl_mk_atom_dup(hb, strlen(hb)), cx) : plw_unify_vals(args[1], pl_sk_gai_err(rc), cx);
        } else {
            const char *h = pl_atom_str(x);
            memset(&hints, 0, sizeof hints);
            hints.ai_family = AF_INET;
            rc = h ? getaddrinfo(h, (const char *)0, &hints, &res) : EAI_NONAME;
            if (rc != 0 || !res) ok = plw_unify_vals(args[1], pl_sk_gai_err(rc ? rc : EAI_NONAME), cx);
            else { DESCR_t ip = pl_sk_ip(&((struct sockaddr_in *)res->ai_addr)->sin_addr); freeaddrinfo(res); ok = plw_unify_vals(args[1], ip, cx); }
        }
    }
PL_CX_LEAF_TAIL
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
PL_CX_LEAF_HEAD(fx_stat, 2)
    {
        const char *p = pl_atom_str(rt_pl_deref_val(args[0]));
        struct stat sb;
        DESCR_t t[4];
        ok = 0;
        if (p && stat(p, &sb) != 0) ok = plw_unify_vals(args[1], pl_fx_err(), cx);
        else if (p) {
            t[0] = INTVAL((long long)sb.st_mode);
            t[1] = REALVAL((double)sb.st_atime);
            t[2] = REALVAL((double)sb.st_mtime);
            t[3] = REALVAL((double)sb.st_ctime);
            ok = plw_unify_vals(args[1], pl_gnu_fn("st", 4, t), cx);
        }
    }
PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(fx_utime, 4)
    {
        const char *p = pl_atom_str(rt_pl_deref_val(args[0]));
        DESCR_t ta = rt_pl_deref_val(args[1]), tm = rt_pl_deref_val(args[2]);
        struct utimbuf ub;
        ok = 0;
        if (p && ta.v == (DTYPE_t)DT_I && tm.v == (DTYPE_t)DT_I) {
            ub.actime = (time_t)ta.i;
            ub.modtime = (time_t)tm.i;
            ok = utime(p, &ub) == 0 ? pl_os_okv(args[3], cx) : plw_unify_vals(args[3], pl_fx_err(), cx);
        }
    }
PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(gnu_directory_files, 2)
    {
        extern void *rt_dir_open(const char *);
        extern const char *rt_dir_next(void *);
        extern void rt_dir_close(void *);
        const char *p = pl_gnu_path_arg(args[0], cx), *e;
        void *dh = (void *)0;
        gv_t el;
        int n = 0;
        ok = 0;
        el.p = 0;
        el.len = 0;
        el.cap = 0;
        el.esz = 0;
        el.kind = 0;
        el.pad = 0;
        if (p && !(dh = rt_dir_open(p))) {
            extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
            if (pl_swi_dialect() && (errno == ENOENT || errno == ENOTDIR)) cx->ball = rt_pl_ball_kind2("existence_error", errno == ENOENT ? "file" : "directory", rt_pl_deref_val(args[0]));
            else pl_gnu_os_fail(cx);
        } else if (p) {
            while ((e = rt_dir_next(dh))) { *pl_gnu_vec_push(&el) = pl_mk_atom_dup(e, strlen(e)); n++; }
            rt_dir_close(dh);
            ok = plw_unify_vals(args[1], pl_list_from_arr((DESCR_t *)el.p, n), cx);
        }
        pl_gnu_ctx(cx, "directory_files", 2);
    }
PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(gnu_environ_list, 1)
    {
        extern char **environ;
        gv_t el;
        int n = 0;
        el.p = 0;
        el.len = 0;
        el.cap = 0;
        el.esz = 0;
        el.kind = 0;
        el.pad = 0;
        for (char **ep = environ; ep && *ep; ep++) {
            const char *eq = strchr(*ep, '=');
            DESCR_t kv[2];
            if (!eq) continue;
            kv[0] = pl_mk_atom_dup(*ep, (size_t)(eq - *ep));
            kv[1] = pl_mk_atom_dup(eq + 1, strlen(eq + 1));
            *pl_gnu_vec_push(&el) = pl_gnu_fn("=", 2, kv);
            n++;
        }
        ok = plw_unify_vals(args[0], pl_list_from_arr((DESCR_t *)el.p, n), cx);
    }
PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(gnu_file_props, 2)
    {
        const char *p = pl_gnu_path_arg(args[0], cx), *ty;
        struct stat st;
        char *rp;
        DESCR_t el[12];
        int n = 0;
        ok = 0;
        if (p && stat(p, &st) != 0) ok = pl_gnu_os_fail(cx);
        else if (p) {
            rp = (char *)rt_wsb_alloc(PATH_MAX + 1);
            ty = S_ISREG(st.st_mode) ? "regular" : S_ISDIR(st.st_mode) ? "directory" : S_ISFIFO(st.st_mode) ? "fifo" : S_ISSOCK(st.st_mode) ? "socket" : S_ISCHR(st.st_mode) ? "character_device" :
                S_ISBLK(st.st_mode) ? "block_device" : "unknown";
            el[n++] = pl_gnu_fn1("absolute_file_name", pl_mk_atom_dup(p, strlen(p)));
            if (realpath(p, rp)) el[n++] = pl_gnu_fn1("real_file_name", pl_mk_atom_dup(rp, strlen(rp)));
            el[n++] = pl_gnu_fn1("type", pl_mk_atom(ty));
            el[n++] = pl_gnu_fn1("size", INTVAL(st.st_size));
            if (access(p, R_OK) == 0) el[n++] = pl_gnu_fn1("permission", pl_mk_atom("read"));
            if (access(p, W_OK) == 0) el[n++] = pl_gnu_fn1("permission", pl_mk_atom("write"));
            if (access(p, X_OK) == 0) el[n++] = pl_gnu_fn1("permission", pl_mk_atom(S_ISDIR(st.st_mode) ? "search" : "execute"));
            el[n++] = pl_gnu_fn1("creation", pl_gnu_dt(st.st_ctime));
            el[n++] = pl_gnu_fn1("last_access", pl_gnu_dt(st.st_atime));
            el[n++] = pl_gnu_fn1("last_modification", pl_gnu_dt(st.st_mtime));
            ok = plw_unify_vals(args[1], pl_list_from_arr(el, n), cx);
        }
        pl_gnu_ctx(cx, "file_property", 2);
    }
PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(format3, 3)
    {
        extern int fh_current_output(void);
        extern void fh_set_output(int);
        extern int fh_capture_begin(char **, size_t *, int *);
        extern void fh_capture_end(int, int);
        extern int prolog_atom_intern(const char *);
        extern int rt_pl_cap_refuses(unsigned long long, unsigned long long);
        extern void *rt_pl_cap_error(void);
        DESCR_t d = rt_pl_deref_val(args[0]);
        int kind = 0;
        int idx;
        ok = 0;
        if (d.v == (DTYPE_t)DT_PLREF && plc_fid_arity(d.slen) == 1) {
            int f = plc_fid_name(d.slen);
            kind = f == prolog_atom_intern("atom") ? 1 : f == prolog_atom_intern("string") ? 4 : f == prolog_atom_intern("codes") ? 2 : f == prolog_atom_intern("chars") ? 3 : 0;
        }
        if (d.v == (DTYPE_t)DT_PLREF && plc_fid_arity(d.slen) == 2) { int f = plc_fid_name(d.slen); kind = f == prolog_atom_intern("codes") ? 5 : f == prolog_atom_intern("chars") ? 6 : 0; }
        idx = kind ? -1 : pl_stream_idx_ball(args[0], 1, 1, cx);
        if (idx >= 0) {
            int sv = fh_current_output();
            fh_set_output(idx);
            ok = (rt_pl_dop_format_c(args + 1, 2, cx).v == (DTYPE_t)DT_I);
            fh_set_output(sv);
            ok = pl_out_ioerr(idx, args[0], cx) && ok;
        } else if (kind) {
            {
                char *buf = 0;
                size_t sz = 0;
                int sv = -1;
                int cap = fh_capture_begin(&buf, &sz, &sv);
                if (cap >= 0) {
                    DESCR_t r = rt_pl_dop_format_c(args + 1, 2, cx);
                    fh_capture_end(cap, sv);
                    if (r.v == (DTYPE_t)DT_I && kind >= 5) ok = plw_unify_vals(((DESCR_t *)d.p)[0], pl_text_dlist(buf ? buf : "", sz, kind == 5, ((DESCR_t *)d.p)[1]), cx);
                    else if (r.v == (DTYPE_t)DT_I && kind == 4 && rt_pl_cap_refuses((unsigned long long)sz + 1ULL, 1ULL)) { cx->ball = rt_pl_cap_error(); ok = 0; }
                    else if (r.v == (DTYPE_t)DT_I) ok =
                        plw_unify_vals(((DESCR_t *)d.p)[0],
                        kind == 1 ? pl_mk_atom_dup(buf ? buf : "", sz) : kind == 4 ? rt_pl_make_string_cell(buf ? buf : "") : pl_text_list(buf ? buf : "", kind == 2), cx);
                }
            }
        }
    }
PL_CX_LEAF_TAIL
DESCR_t rt_pl_dop_pl_cp_nth_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *root) {
    int ok = 0;
    int ar = 0;
    const char *key;
    const char *sl;
    if (nargs != 3) return FAILDESCR;
    pl_atoms_ready();
    rt_pl_tr_gc_sync(cx->tr);
    {
        DESCR_t iv = rt_pl_deref_val(args[0]);
        if (iv.v == DT_I && (key = rt_pl_db_cp_nth(root, iv.i, &ar)) != (const char *)0 && (sl = strrchr(key, '/')) != (const char *)0) ok =
            plw_unify_vals(args[1], pl_mk_atom_dup(key, (size_t)(sl - key)), cx) && plw_unify_vals(args[2], INTVAL((long long)ar), cx);
    }
    rt_pl_tr_gc_sync(cx->tr);
    return ok ? pl_ok() : FAILDESCR;
}
static DESCR_t pl_pp_head(DESCR_t h, DESCR_t *mod) {
    extern const char *prolog_atom_name(int);
    h = rt_pl_deref_val(h);
    while (h.v == (DTYPE_t)DT_PLREF && plc_fid_arity(h.slen) == 2 && !strcmp(prolog_atom_name(plc_fid_name(h.slen)), ":")) {
        if (mod) *mod = ((DESCR_t *)h.p)[0];
        h = rt_pl_deref_val(((DESCR_t *)h.p)[1]);
    }
    return h;
}
static int pl_pp_ctl(const char *nm, int ar) {
    static const char *const c[] = { ",", ";", "->", "call", "!", "catch", "throw", "true", "fail", 0 };
    static const int ca[] = { 2, 2, 2, 1, 0, 3, 1, 0, 0 };
    for (int i = 0; c[i]; i++) if (ca[i] == ar && !strcmp(c[i], nm)) return 1;
    return 0;
}
int rt_pl_meta_builtin(const char *nm, int ar, int gnu, const char *const **toks) {
    for (const pl_meta_row_t *r = gnu ? pl_meta_rows_gnu : pl_meta_rows_swi; nm && r->nm; r++) if (r->ar == ar && !strcmp(r->nm, nm)) { *toks = pl_meta_tok + r->tok; return 1; }
    return 0;
}
static int pl_pp_meta(void *root, const char *nm, int ar, const char *const **toks, const long **ids) {
    extern int rt_pl_db_key_meta(void *, const char *, const long **);
    size_t nl = nm ? strlen(nm) : 0;
    char key[nl + 24];
    *toks = (const char *const *)0;
    *ids = (const long *)0;
    if (!nm || ar <= 0) return 0;
    snprintf(key, sizeof key, "%s/%d", nm, ar);
    if (root && rt_pl_db_key_meta(root, key, ids) == ar) return 1;
    *ids = (const long *)0;
    return rt_pl_meta_builtin(nm, ar, pl_gnu_dialect(), toks);
}
static DESCR_t pl_pp_meta_term(const char *nm, int ar, const char *const *toks, const long *ids) {
    extern const char *prolog_atom_name(int);
    DESCR_t a[ar > 0 ? ar : 1];
    for (int k = 0; k < ar; k++) {
        const char *t = toks ? toks[k] : prolog_atom_name((int)ids[k]);
        if (!t) t = "?";
        a[k] = (isdigit((unsigned char)t[0]) && !t[1]) ? INTVAL(t[0] - '0') : pl_mk_atom_dup(t, strlen(t));
    }
    return pl_gnu_fn(nm, ar, a);
}
static const char *pl_pp_prop(DESCR_t h, void *root, int64_t i, const char **file, long *line) {
    extern int rt_proc_is_registered(const char *);
    extern int rt_pl_db_key_line(void *, const char *);
    extern int rt_pl_db_key_mf(void *, const char *);
    extern int rt_pl_db_key_kind(void *, const char *);
    extern const char *rt_pl_db_key_file(void *, const char *);
    extern int rt_pl_db_key_nclauses(void *, const char *);
    extern int pl_pi_is_builtin(const char *, int);
    extern const char *prolog_atom_name(int);
    const char *nm, *fp = (const char *)0;
    const char *const *mt;
    const long *mi;
    int ar = 0, kind;
    h = pl_pp_head(h, (DESCR_t *)0);
    if (h.v == (DTYPE_t)DT_PLREF) { nm = prolog_atom_name(plc_fid_name(h.slen)); ar = plc_fid_arity(h.slen); } else nm = pl_atom_str(h);
    if (!nm || i < 1) return (const char *)0;
    {
        size_t nl = strlen(nm);
        char key[nl + 24];
        snprintf(key, sizeof key, "%s/%d", nm, ar);
        kind = rt_pl_db_key_kind(root, key);
        if (kind == 1) fp = rt_pl_db_key_file(root, key);
        if (pl_gnu_dialect()) {
            const char *gp[10];
            int n = 0, ln = kind ? rt_pl_db_key_line(root, key) : 0, asv;
            if (kind == 2) fp = rt_pl_db_key_file(root, key);
            asv = kind == 2 && !fp;
            if (asv) fp = "user_input";
            if (kind) {
                gp[n++] = kind == 2 ? "dynamic" : "static";
                gp[n++] = kind == 2 ? "public" : "private";
                gp[n++] = rt_pl_db_key_mf(root, key) ? "multifile" : "monofile";
                gp[n++] = "user";
                if (fp) gp[n++] = "prolog_file";
                if (ln > 0 || asv) gp[n++] = "prolog_line";
                if (fp) gp[n++] = "file";
                gp[n++] = "defined";
            } else if (pl_pi_is_builtin(nm, ar) || pl_pp_ctl(nm, ar) || rt_proc_is_registered(key)) {
                gp[n++] = "static";
                gp[n++] = "private";
                gp[n++] = "monofile";
                gp[n++] = strncmp(nm, "fd_", 3) ? "built_in" : "built_in_fd";
                gp[n++] = "native_code";
                if (pl_pp_ctl(nm, ar)) gp[n++] = "control_construct";
                if (pl_pp_meta(root, nm, ar, &mt, &mi)) gp[n++] = "meta_predicate";
                gp[n++] = "defined";
            }
            if (i > n) return (const char *)0;
            if (!strcmp(gp[i - 1], "prolog_file") || !strcmp(gp[i - 1], "file")) *file = fp;
            if (!strcmp(gp[i - 1], "prolog_line")) *line = ln;
            return gp[i - 1];
        }
    }
    {
        const char *gp[12];
        int n = 0;
        if (kind) {
            char tk[strlen(nm) + 32];
            gp[n++] = kind == 2 ? "dynamic" : "static";
            if (fp) gp[n++] = "file";
            gp[n++] = "defined";
            snprintf(tk, sizeof tk, "$tbl %s/%d", nm, ar);
            if (rt_pl_db_key_kind(root, tk)) gp[n++] = "tabled";
        } else if (pl_pi_is_builtin(nm, ar) && !(!strcmp(nm, "?-") || !strcmp(nm, ":-") || !strcmp(nm, "|"))) {
            gp[n++] = "built_in";
            gp[n++] = "defined";
            gp[n++] = "static";
        }
        if (kind) { char nk[strlen(nm) + 24]; snprintf(nk, sizeof nk, "%s/%d", nm, ar); if (rt_pl_db_key_nclauses(root, nk) >= 0) gp[n++] = "number_of_clauses"; }
        if (n) gp[n++] = "visible";
        { extern int pl_pi_is_iso(const char *, int); if (n && pl_pi_is_iso(nm, ar)) gp[n++] = "iso"; }
        if (pl_pp_meta(root, nm, ar, &mt, &mi)) gp[n++] = "meta_predicate";
        if (i > n) return (const char *)0;
        if (!strcmp(gp[i - 1], "file")) *file = fp;
        return gp[i - 1];
    }
}
DESCR_t rt_pl_dop_pl_pp_count_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *root) {
    int ok = 0;
    if (nargs != 2) return FAILDESCR;
    pl_atoms_ready();
    rt_pl_tr_gc_sync(cx->tr);
    { DESCR_t h = rt_pl_deref_val(args[0]); const char *fp; long ln; long long n = 0; while (pl_pp_prop(h, root, n + 1, &fp, &ln)) n++; ok = plw_unify_vals(args[1], INTVAL(n), cx); }
    rt_pl_tr_gc_sync(cx->tr);
    return ok ? pl_ok() : FAILDESCR;
}
DESCR_t rt_pl_dop_pl_pp_nth_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *root) {
    int ok = 0;
    if (nargs != 3) return FAILDESCR;
    pl_atoms_ready();
    rt_pl_tr_gc_sync(cx->tr);
    {
        DESCR_t iv = rt_pl_deref_val(args[0]);
        const char *fp = (const char *)0;
        long ln = 0;
        const char *pr = iv.v == DT_I ? pl_pp_prop(rt_pl_deref_val(args[1]), root, iv.i, &fp, &ln) : (const char *)0;
        if (pr && !strcmp(pr, "meta_predicate")) {
            DESCR_t hh = pl_pp_head(rt_pl_deref_val(args[1]), (DESCR_t *)0);
            const char *hn = hh.v == (DTYPE_t)DT_PLREF ? prolog_atom_name(plc_fid_name(hh.slen)) : pl_atom_str(hh);
            int ha = hh.v == (DTYPE_t)DT_PLREF ? plc_fid_arity(hh.slen) : 0;
            const char *const *mt;
            const long *mi;
            ok = hn && pl_pp_meta(root, hn, ha, &mt, &mi) && plw_unify_vals(args[2], pl_gnu_fn1(pr, pl_pp_meta_term(hn, ha, mt, mi)), cx);
        } else if (pr && !strcmp(pr, "number_of_clauses")) {
            extern int rt_pl_db_key_nclauses(void *, const char *);
            DESCR_t hh = pl_pp_head(rt_pl_deref_val(args[1]), (DESCR_t *)0);
            const char *hn = hh.v == (DTYPE_t)DT_PLREF ? prolog_atom_name(plc_fid_name(hh.slen)) : pl_atom_str(hh);
            int ha = hh.v == (DTYPE_t)DT_PLREF ? plc_fid_arity(hh.slen) : 0;
            char hk[(hn ? strlen(hn) : 0) + 24];
            snprintf(hk, sizeof hk, "%s/%d", hn ? hn : "", ha);
            ok = hn && plw_unify_vals(args[2], pl_gnu_fn1(pr, INTVAL(rt_pl_db_key_nclauses(root, hk))), cx);
        } else if (pr && !strcmp(pr, "prolog_line")) ok = plw_unify_vals(args[2], pl_gnu_fn1(pr, INTVAL(ln)), cx);
        else if (pr && fp) ok = plw_unify_vals(args[2], pl_gnu_fn1(pr, pl_mk_atom_dup(fp, strlen(fp))), cx);
        else if (pr) ok = plw_unify_vals(args[2], pl_mk_atom_dup(pr, strlen(pr)), cx);
    }
    rt_pl_tr_gc_sync(cx->tr);
    return ok ? pl_ok() : FAILDESCR;
}
PL_CX_LEAF_HEAD(pl_pp_guard, 1)
    {
        extern void *rt_pl_ball_kind2_pi(const char *, const char *, DESCR_t, const char *, int);
        extern void *rt_pl_ball_instantiation(void);
        DESCR_t m = NULVCL;
        DESCR_t h = pl_pp_head(args[0], &m);
        ok = 1;
        if (pl_iso_unbound(h)) { cx->ball = rt_pl_ball_instantiation(); ok = 0; }
        if (ok && (h.v == DT_I || h.v == DT_R) && pl_swi_dialect()) ok = 0;
        else if (h.v == DT_I || h.v == DT_R) { cx->ball = rt_pl_ball_kind2_pi("type_error", "callable", h, "predicate_property", 2); ok = 0; }
        if (ok && m.v != (DTYPE_t)DT_SNUL && pl_val_unbound(rt_pl_deref_val(m))) ok = plw_unify_vals(m, pl_mk_atom_dup("user", 4), cx);
    }
PL_CX_LEAF_TAIL
