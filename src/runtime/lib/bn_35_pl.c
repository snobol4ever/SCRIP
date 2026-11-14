#include "runtime/lib/bn_head.h"
#include "runtime/lib/bn_head_pl_cx_leaf_head.h"
#include "runtime/lib/bn_head_pl_flag.h"
#include <sys/utsname.h>
#include <sys/resource.h>
#include <poll.h>
#include <sys/stat.h>
#include "runtime/rt/prolog_atom.h"
#include "runtime/core/runtime_shim.h"
#include "driver/driver_globals.h"
#include <errno.h>
#include <ctype.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "runtime/lib/bn_05_pl.h"
#include "runtime/lib/bn_11_pl.h"
#include "runtime/lib/bn_27_pl.h"
#include "runtime/lib/bn_31_pl.h"
#include "runtime/lib/bn_32_pl.h"
#include "runtime/lib/bn_33_pl.h"
#include "runtime/lib/bn_34_pl.h"
#include <time.h>
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
#include "runtime/rt/rt_pl_trail_pl_tr_ctx_s.h"
#include "runtime/rt/rt_pl_trail.h"
#include "ir/descr_tags_dt_platom.h"
extern FILE *fh_get(int);
static int pl_gnu_copy(const char *f, const char *t, pl_tr_ctx_t *cx) {
    char b[65536];
    ssize_t n = 0;
    int in = open(f, O_RDONLY), out = -1, e;
    if (in < 0) return pl_gnu_os_fail(cx);
    if ((out = open(t, O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0) { e = errno; close(in); errno = e; return pl_gnu_os_fail(cx); }
    while ((n = read(in, b, sizeof b)) > 0) if (write(out, b, (size_t)n) != n) { n = -1; break; }
    e = errno;
    close(in);
    close(out);
    errno = e;
    return n < 0 ? pl_gnu_os_fail(cx) : 1;
}
static int pl_gs_word(const char *s) { size_t n = 0; for (; s[n]; n++) if (!(isascii((unsigned char)s[n]) && (isalnum((unsigned char)s[n]) || s[n] == '_'))) return 0; return n > 1; }
static int pl_gs_cmp(const void *a, const void *b) { return strcmp(*(const char *const *)a, *(const char *const *)b); }
static DESCR_t pl_gs_compl(const char *pf) {
    extern int prolog_atom_count(void);
    extern const char *prolog_atom_name(int);
    int n = prolog_atom_count(), k = 0;
    size_t pn = strlen(pf);
    DESCR_t l = pl_nil();
    for (int i = 0; i < n; i++) { const char *w = prolog_atom_name(i); if (w && !strncmp(w, pf, pn) && pl_gs_word(w)) k++; }
    {
        const char *hit[k > 0 ? k : 1];
        k = 0;
        for (int i = 0; i < n; i++) { const char *w = prolog_atom_name(i); if (w && !strncmp(w, pf, pn) && pl_gs_word(w)) hit[k++] = w; }
        qsort(hit, (size_t)k, sizeof hit[0], pl_gs_cmp);
        for (int i = k - 1; i >= 0; i--) l = pl_cons(pl_mk_atom(hit[i]), l);
    }
    return l;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_os_poll(DESCR_t a, DESCR_t out, pl_tr_ctx_t *cx) {
    extern FILE *fh_get(int);
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    int pr = a.v == (DTYPE_t)DT_PLREF && plc_fid_arity(a.slen) == 2;
    DESCR_t l = pr ? rt_pl_deref_val(((DESCR_t *)a.p)[0]) : pl_nil(), t = pr ? rt_pl_deref_val(((DESCR_t *)a.p)[1]) : FAILDESCR;
    const char *ts = pl_atom_str(t);
    int n = 0, got = 0, ms = ts && !strcmp(ts, "infinite") ? -1 : t.v == (DTYPE_t)DT_I ? (int)(t.i * 1000) : t.v == (DTYPE_t)DT_R ? (int)(t.r * 1000.0) : -2;
    if (ms == -2) { cx->ball = rt_pl_ball_kind2("type_error", "float", t); return 0; }
    for (DESCR_t c = l; pl_is_cons(c); c = rt_pl_deref_val(((DESCR_t *)c.p)[1])) n++;
    struct pollfd pf[n + 1];
    DESCR_t el[n + 1];
    char bf[n + 1];
    int i = 0;
    for (DESCR_t c = l; pl_is_cons(c); c = rt_pl_deref_val(((DESCR_t *)c.p)[1]), i++) {
        DESCR_t e = rt_pl_deref_val(((DESCR_t *)c.p)[0]);
        void *b = (void *)0;
        int idx = e.v == (DTYPE_t)DT_I ? -1 : pl_stream_resolve(e, 0, 0, &b);
        FILE *fp = idx >= 0 ? fh_get(idx) : (FILE *)0;
        el[i] = e;
        pf[i].fd = e.v == (DTYPE_t)DT_I ? (int)e.i : fp ? fileno(fp) : -1;
        pf[i].events = POLLIN;
        pf[i].revents = 0;
        bf[i] = fp && fp->_IO_read_ptr < fp->_IO_read_end;
        got |= bf[i];
        if (pf[i].fd < 0) { cx->ball = b ? b : rt_pl_ball_kind2("domain_error", "waitable_stream", e); return 0; }
    }
    if (poll(pf, (nfds_t)n, got ? 0 : ms) < 0) return pl_gnu_os_fail(cx);
    DESCR_t r = pl_nil();
    for (i = n - 1; i >= 0; i--) if (bf[i] || (pf[i].revents & (POLLIN | POLLHUP | POLLERR))) r = pl_cons(el[i], r);
    return plw_unify_vals(out, r, cx);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
PL_CX_LEAF_HEAD(gnu_os, 3)
    {
        const char *op = pl_atom_str(rt_pl_deref_val(args[0]));
        DESCR_t a = rt_pl_deref_val(args[1]), b = rt_pl_deref_val(args[2]);
        ok = 0;
        if (!op) ok = 0;
        else if (!strcmp(op, "shell") || !strcmp(op, "system")) {
            const char *c = pl_atom_str(a);
            int st = c ? system(c) : -1;
            if (st == -1) ok = pl_gnu_os_fail(cx);
            else ok = plw_unify_vals(args[2], INTVAL(op[1] == 'h' ? (WIFEXITED(st) ? WEXITSTATUS(st) : 128 + WTERMSIG(st)) : st), cx);
        } else if (!strcmp(op, "unlink") || !strcmp(op, "rmdir")) {
            const char *p = pl_gnu_path_arg(args[1], cx);
            if (p) ok = (op[0] == 'u' ? unlink(p) : rmdir(p)) == 0 ? 1 : pl_gnu_os_fail(cx);
        } else if (!strcmp(op, "rename") || !strcmp(op, "copy")) {
            const char *f = pl_gnu_path_arg(args[1], cx), *t = f ? pl_gnu_path_arg(args[2], cx) : (const char *)0;
            if (t) ok = op[0] == 'r' ? (rename(f, t) == 0 ? 1 : pl_gnu_os_fail(cx)) : pl_gnu_copy(f, t, cx);
        } else if (!strcmp(op, "access")) {
            const char *p = pl_gnu_path_arg(args[1], cx);
            ok = p && b.v == (DTYPE_t)DT_I && access(p, (int)b.i) == 0;
        } else if (!strcmp(op, "st_kind") || !strcmp(op, "st_size") || !strcmp(op, "st_mtime") || !strcmp(op, "st_same")) {
            const char *p = pl_gnu_path_arg(args[1], cx);
            struct stat sb, sb2;
            if (p && stat(p, &sb) == 0) {
                if (!strcmp(op, "st_kind")) ok = plw_unify_vals(args[2], pl_mk_atom(S_ISDIR(sb.st_mode) ? "directory" : S_ISREG(sb.st_mode) ? "file" : "other"), cx);
                else if (!strcmp(op, "st_size")) ok = plw_unify_vals(args[2], INTVAL((long long)sb.st_size), cx);
                else if (!strcmp(op, "st_mtime")) ok = plw_unify_vals(args[2], REALVAL((double)sb.st_mtim.tv_sec + (double)sb.st_mtim.tv_nsec / 1e9), cx);
                else { const char *q = pl_atom_str(b); ok = q && stat(q, &sb2) == 0 && sb.st_dev == sb2.st_dev && sb.st_ino == sb2.st_ino; }
            }
        } else if (!strncmp(op, "fseek", 5) || !strcmp(op, "ftruncate")) {
            extern FILE *fh_get(int);
            FILE *fp = (FILE *)0;
            if ((int)a.v == DT_PLREF && plc_fid_name(a.slen) == prolog_atom_intern("$stream") && plc_fid_arity(a.slen) == 1) {
                DESCR_t sa = rt_pl_deref_val(((DESCR_t *)a.p)[0]);
                if (sa.v == DT_I && (int)sa.i >= 0 && (int)sa.i < FH_N) fp = fh_get((int)sa.i);
            }
            if (fp && b.v == (DTYPE_t)DT_I) {
                if (op[1] == 't') ok = fflush(fp) == 0 && ftruncate(fileno(fp), (off_t)b.i) == 0 && fseek(fp, (long)b.i, SEEK_SET) == 0;
                else ok = fseek(fp, (long)b.i, op[5] == '1' ? SEEK_CUR : op[5] == '2' ? SEEK_END : SEEK_SET) == 0;
            }
        } else if (!strcmp(op, "atom_tag")) {
            extern int prolog_atom_intern_mod(int, int);
            if ((int)a.v == DT_PLREF && plc_fid_arity(a.slen) == 2) {
                DESCR_t m = rt_pl_deref_val(((DESCR_t *)a.p)[0]), n = rt_pl_deref_val(((DESCR_t *)a.p)[1]), r = {0};
                if (m.v == (DTYPE_t)DT_PLATOM && n.v == (DTYPE_t)DT_PLATOM) { r.v = (DTYPE_t)DT_PLATOM; r.i = prolog_atom_intern_mod((int)m.i, (int)n.i); ok = plw_unify_vals(args[2], r, cx); }
            }
        } else if (!strcmp(op, "atom_mod")) {
            extern int prolog_atom_module(int);
            if (a.v == (DTYPE_t)DT_PLATOM) {
                int k = prolog_atom_module((int)a.i);
                DESCR_t r = {0};
                if (k >= 0) { r.v = (DTYPE_t)DT_PLATOM; r.i = k; } else r = pl_mk_atom("usermod");
                ok = plw_unify_vals(args[2], r, cx);
            }
        } else if (!strcmp(op, "cwd")) {
            DESCR_t cwa = pl_os_cwd_atom();
            if (!IS_FAIL_fn(cwa)) ok = plw_unify_vals(args[2], cwa, cx);
        } else if (!strcmp(op, "chdir")) {
            const char *p = pl_gnu_path_arg(args[1], cx);
            ok = p && chdir(p) == 0;
        } else if (!strcmp(op, "setenv") || !strcmp(op, "unsetenv")) {
            const char *nm = pl_atom_str(a), *vv = pl_atom_str(b);
            ok = nm && (op[0] == 'u' ? unsetenv(nm) == 0 : (vv && setenv(nm, vv, 1) == 0));
            g_runtime.scrip_env = 0;
        } else if (!strcmp(op, "hostname")) {
            char h[256];
            if (gethostname(h, sizeof h) != 0) ok = pl_gnu_os_fail(cx);
            else { h[sizeof h - 1] = 0; ok = plw_unify_vals(args[2], pl_mk_atom_dup(h, strlen(h)), cx); }
        } else if (!strcmp(op, "os_version") || !strcmp(op, "architecture")) {
            struct utsname u;
            if (uname(&u) != 0) ok = pl_gnu_os_fail(cx);
            else { const char *v = op[0] == 'o' ? u.release : u.machine; ok = plw_unify_vals(args[2], pl_mk_atom_dup(v, strlen(v)), cx); }
        } else if (!strcmp(op, "kill")) ok = a.v == (DTYPE_t)DT_I && b.v == (DTYPE_t)DT_I && (kill((pid_t)a.i, (int)b.i) == 0 ? 1 : pl_gnu_os_fail(cx));
        else if (!strcmp(op, "popen")) {
            extern int fh_alloc(FILE *);
            int pair = a.v == (DTYPE_t)DT_PLREF && plc_fid_arity(a.slen) == 2;
            const char *cmd = pair ? pl_atom_str(rt_pl_deref_val(((DESCR_t *)a.p)[0])) : (const char *)0;
            const char *m = pair ? pl_atom_str(rt_pl_deref_val(((DESCR_t *)a.p)[1])) : (const char *)0;
            FILE *fp = (FILE *)0;
            fflush(NULL);
            if (cmd && m) fp = popen(cmd, m);
            if (!fp) ok = pl_gnu_os_fail(cx);
            else { int idx = fh_alloc(fp); g_fh[idx].mode = m[0]; ok = plw_unify_vals(args[2], pl_mk_stream(idx), cx); }
        } else if (!strcmp(op, "exec")) {
            extern int fh_alloc(FILE *);
            extern int pipe2(int *, int);
            const char *cmd = pl_atom_str(a);
            int pi[2] = { -1, -1 }, po[2] = { -1, -1 }, pe[2] = { -1, -1 };
            pid_t pid = -1;
            fflush(NULL);
            if (cmd && b.v == (DTYPE_t)DT_PLREF && plc_fid_arity(b.slen) == 4 && pipe2(pi, O_CLOEXEC) == 0 && pipe2(po, O_CLOEXEC) == 0 && pipe2(pe, O_CLOEXEC) == 0) pid = fork();
            if (pid == 0) { dup2(pi[0], 0); dup2(po[1], 1); dup2(pe[1], 2); execl("/bin/sh", "sh", "-c", cmd, (char *)0); _exit(127); }
            if (pid < 0) {
                for (int i = 0; i < 2; i++) { if (pi[i] >= 0) close(pi[i]); if (po[i] >= 0) close(po[i]); if (pe[i] >= 0) close(pe[i]); }
                ok = pl_gnu_os_fail(cx);
            } else {
                DESCR_t *k = (DESCR_t *)b.p;
                int ii = fh_alloc(fdopen(pi[1], "w")), io = fh_alloc(fdopen(po[0], "r")), ie = fh_alloc(fdopen(pe[0], "r"));
                close(pi[0]);
                close(po[1]);
                close(pe[1]);
                g_fh[ii].mode = 'w';
                g_fh[io].mode = 'r';
                g_fh[ie].mode = 'r';
                ok = plw_unify_vals(k[0], pl_mk_stream(ii), cx) && plw_unify_vals(k[1], pl_mk_stream(io), cx) && plw_unify_vals(k[2], pl_mk_stream(ie), cx) &&
                    plw_unify_vals(k[3], INTVAL((long long)pid), cx);
            }
        } else if (!strcmp(op, "poll")) {
            ok = pl_os_poll(a, args[2], cx);
        } else if (!strcmp(op, "wait")) {
            int ws = 0;
            if (a.v != (DTYPE_t)DT_I) ok = 0;
            else if (waitpid((pid_t)a.i, &ws, 0) != (pid_t)a.i) ok = pl_gnu_os_fail(cx);
            else ok = plw_unify_vals(args[2], INTVAL(WIFEXITED(ws) ? WEXITSTATUS(ws) : 128 + WTERMSIG(ws)), cx);
        } else if (!strcmp(op, "date_time")) ok = plw_unify_vals(args[2], pl_gnu_dt(time((time_t *)0)), cx);
        else if (!strcmp(op, "unbind_template")) {
            ok = 1;
            for (int i = 0; a.v == (DTYPE_t)DT_PLREF && i < plc_fid_arity(a.slen); i++) {
                pl_cell_t *c = &((pl_cell_t *)a.p)[i];
                for (;;) {
                    if ((int)c->v == DT_PLVAR && c->p != (void *)c) c = (pl_cell_t *)c->p;
                    else if (c->v == DT_N && c->slen == 1 && c->p) c = (pl_cell_t *)c->p;
                    else if (c->v == DT_N && c->slen == 2 && c->p && ((VCELL_t *)c->p)->cellp) c = ((VCELL_t *)c->p)->cellp;
                    else break;
                }
                if (!pl_cell_unbound(c)) pl_var_make(c, cx, 0);
            }
        } else if (!strcmp(op, "get_time")) {
            struct timespec ts;
            clock_gettime(CLOCK_REALTIME, &ts);
            ok = plw_unify_vals(args[2], REALVAL((double)ts.tv_sec + (double)ts.tv_nsec / 1e9), cx);
        } else if (!strcmp(op, "read_token") || !strcmp(op, "read_atom") || !strcmp(op, "read_integer") || !strcmp(op, "read_number")) ok = pl_gs_read(op, args[2], cx);
        else if (!strncmp(op, "sk_", 3)) ok = pl_gs_sock(op + 3, a, args[2], cx);
        else if (!strncmp(op, "ts_", 3)) ok = pl_gs_ts(op + 3, a, args[2], cx);
        else if (!strcmp(op, "compl")) ok = pl_atom_str(a) && plw_unify_vals(args[2], pl_gs_compl(pl_atom_str(a)), cx);
        else if (!strcmp(op, "get_key")) {
            extern FILE *fh_cur_in_fp(void);
            FILE *fp = fh_cur_in_fp();
            int c = fgetc(fp);
            if (c != '\n') while (fgetc(fp) >= ' ') ;
            ok = plw_unify_vals(args[2], INTVAL(c == EOF ? -1LL : (long long)c), cx);
        } else if (!strcmp(op, "synerr")) {
            pl_flag_t *fl = pl_flag_find("$gnu_synerr");
            ok = fl && fl->is_term && plw_unify_vals(args[2], fl->term, cx);
        } else if (!strcmp(op, "atoms")) {
            extern int prolog_atom_count(void);
            ok = plw_unify_vals(args[2], INTVAL(prolog_atom_count()), cx);
        } else if (!strcmp(op, "heap_grows")) {
            extern long rt_gc_heap_grows(void);
            ok = plw_unify_vals(args[2], INTVAL(rt_gc_heap_grows()), cx);
        } else if (!strcmp(op, "stack_used") || !strcmp(op, "stack_free")) {
            extern void * __libc_stack_end;
            extern long rt_gc_bytes_in_use(void);
            extern long rt_gcheap_free(void);
            const char * k = pl_atom_str(a);
            long used = 0, avail = 0;
            char here;
            struct rlimit rl;
            if (k && !strcmp(k, "local_stack")) {
                extern long rt_stack_stat(int which, const char * sp);
                used = rt_stack_stat(5, &here);
                if (used < 0) used = (char *) __libc_stack_end - &here;
                if (used < 0) used = 0;
                avail = (getrlimit(RLIMIT_STACK, &rl) == 0 && rl.rlim_cur != RLIM_INFINITY) ? (long) rl.rlim_cur - used : (8L << 20) - used;
                ok = 1;
            } else if (k && !strcmp(k, "global_stack")) {
                used = rt_gc_bytes_in_use();
                avail = rt_gcheap_free();
                ok = 1;
            } else if (k && !strcmp(k, "trail_stack")) {
                used = (long) ((uintptr_t) cx->tr & (PL_TR_ARENA_BYTES - 1));
                avail = (long) PL_TR_ARENA_BYTES - used;
                ok = 1;
            } else if (k && !strcmp(k, "cstr_stack")) ok = 1;
            if (ok) ok = plw_unify_vals(args[2], INTVAL(op[6] == 'f' ? (avail > 0 ? avail : 0) : used), cx);
        }
    }
PL_CX_LEAF_TAIL
