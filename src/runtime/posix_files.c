#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <grp.h>
#include <poll.h>
#include <pwd.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include "driver/driver_data.h"
#include "driver/driver_globals.h"
#include "posix_leaves.h"
#include "runtime/rt/rt_arena.h"
#include "runtime/rt/rt_list_view.h"
#include "ir/descr_tags_dt_platom.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t pf_str(const char *s) { return STRVAL(rt_heap_strdup_c(s)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t pf_fh(int fd, const char *mode, const char *name) {
    FILE *fp = fdopen(fd, mode);
    int idx;
    if (!fp) return FAILDESCR;
    setvbuf(fp, NULL, _IONBF, 0);
    idx = fh_alloc(fp);
    if (idx < 0) { fclose(fp); return FAILDESCR; }
    g_fh[idx].name = rt_heap_strdup_c(name);
    return FHVAL(idx);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pf_fd(DESCR_t d) { FILE *fp = d.v == DT_FH ? fh_get((int) d.i) : NULL; return fp ? fileno(fp) : -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t pf_readlink(const char *p) {
    for (size_t n = 256; n < ((size_t) 1 << 30); n *= 2) {
        char *b = (char *) rt_heap_alloc_c(n);
        ssize_t r = readlink(p, b, n);
        if (r < 0) return px_fail();
        if ((size_t) r < n) return STRVAL(rt_heap_strndup_c(b, (size_t) r));
    }
    return px_fail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t bt_ICNx24UREADLINK(DESCR_t *args, int nargs) { const char *p = nargs == 1 ? px_str(args[0]) : NULL; px_begin(); return p ? pf_readlink(p) : FAILDESCR; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t bt_ICNx24USYMLINK(DESCR_t *args, int nargs) {
    const char *a = nargs == 2 ? px_str(args[0]) : NULL, *b = nargs == 2 ? px_str(args[1]) : NULL;
    px_begin();
    if (!a || !b) return FAILDESCR;
    return symlink(a, b) == 0 ? NULVCL : px_fail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t pf_owner(int user, unsigned id) {
    struct passwd *pw = user ? getpwuid((uid_t) id) : NULL;
    struct group *gr = user ? NULL : getgrgid((gid_t) id);
    if (pw) return pf_str(pw->pw_name);
    if (gr) return pf_str(gr->gr_name);
    return px_fmt("%u", id);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t pf_stat_rec(const struct stat *st, DESCR_t link) {
    char *m = rt_heap_strdup_c("----------");
    mode_t x = st->st_mode;
    m[0] = S_ISLNK(x) ? 'l' : S_ISREG(x) ? '-' : S_ISDIR(x) ? 'd' : S_ISCHR(x) ? 'c' : S_ISBLK(x) ? 'b' : S_ISFIFO(x) ? '|' : S_ISSOCK(x) ? 's' : '-';
    for (int i = 0; i < 9; i++) if (x & (0400 >> i)) m[1 + i] = "rwxrwxrwx"[i];
    if (x & S_ISUID) m[3] = m[3] == 'x' ? 's' : 'S';
    if (x & S_ISGID) m[6] = m[6] == 'x' ? 's' : 'S';
    if (x & S_ISVTX) m[9] = m[9] == 'x' ? 't' : 'T';
    DESCR_t f[14] = { INTVAL((int64_t) st->st_dev), INTVAL((int64_t) st->st_ino), STRVAL(m), INTVAL((int64_t) st->st_nlink), pf_owner(1, st->st_uid), pf_owner(0, st->st_gid),
        INTVAL((int64_t) st->st_rdev), INTVAL((int64_t) st->st_size), INTVAL((int64_t) st->st_atime), INTVAL((int64_t) st->st_mtime), INTVAL((int64_t) st->st_ctime), INTVAL((int64_t) st->st_blksize),
        INTVAL((int64_t) st->st_blocks), link };
    return px_rec("posix_stat", f, 14);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t bt_ICNx24ULSTAT(DESCR_t *args, int nargs) {
    const char *p = nargs == 1 ? px_str(args[0]) : NULL;
    struct stat st;
    px_begin();
    if (!p) return FAILDESCR;
    return lstat(p, &st) == 0 ? pf_stat_rec(&st, NULVCL) : px_fail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t bt_ICNx24USTAT(DESCR_t *args, int nargs) {
    struct stat st;
    DESCR_t link = NULVCL;
    px_begin();
    if (nargs < 1) return FAILDESCR;
    if (args[0].v == DT_S) {
        const char *p = rt_cstr_d(args[0]);
        if (lstat(p, &st) != 0) return px_fail();
        if (S_ISLNK(st.st_mode)) { link = pf_readlink(p); if (link.v != DT_S) return link; }
        return pf_stat_rec(&st, link);
    }
    int fd = pf_fd(args[0]);
    if (fd < 0) return FAILDESCR;
    return fstat(fd, &st) == 0 ? pf_stat_rec(&st, link) : px_fail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t bt_ICNx24UPIPE(DESCR_t *args, int nargs) {
    int fds[2];
    (void) args;
    px_begin();
    if (nargs != 0) return FAILDESCR;
    if (pipe(fds) != 0) return px_fail();
    DESCR_t f[2] = { pf_fh(fds[0], "r", "pipe"), pf_fh(fds[1], "w", "pipe") };
    return f[0].v == DT_FH && f[1].v == DT_FH ? px_list(f, 2) : px_fail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t bt_ICNx24UFILEPAIR(DESCR_t *args, int nargs) {
    int fds[2];
    (void) args;
    px_begin();
    if (nargs != 0) return FAILDESCR;
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, fds) != 0) return px_fail();
    DESCR_t f[2] = { pf_fh(fds[0], "r+", "filepair"), pf_fh(fds[1], "r+", "filepair") };
    return f[0].v == DT_FH && f[1].v == DT_FH ? px_list(f, 2) : px_fail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t bt_ICNx24UFDUP(DESCR_t *args, int nargs) {
    int s = nargs == 2 ? pf_fd(args[0]) : -1, d = nargs == 2 ? pf_fd(args[1]) : -1;
    px_begin();
    if (s < 0 || d < 0) return FAILDESCR;
    fflush(fh_get((int) args[0].i));
    if (dup2(s, d) < 0) return px_fail();
    __fpurge(fh_get((int) args[1].i));
    return NULVCL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pf_lock(int fd, int cmd, const char *o, DESCR_t *out) {
    struct flock fl;
    char *p;
    const char *start = strchr(o, ','), *len;
    if (!start || !(len = strchr(start + 1, ','))) return 0;
    fl.l_type = o[0] == 'r' ? F_RDLCK : o[0] == 'w' ? F_WRLCK : F_UNLCK;
    start++;
    len++;
    fl.l_whence = *start == '+' ? SEEK_CUR : *start == '-' ? SEEK_END : SEEK_SET;
    fl.l_start = strtol(*start == '+' || *start == '-' ? start + 1 : start, &p, 10);
    fl.l_len = strtol(len, &p, 10);
    if (fcntl(fd, cmd, &fl) < 0) { *out = px_fail(); return 1; }
    DESCR_t f[2] =
        { px_fmt("%c,%s%ld,%ld", fl.l_type == F_RDLCK ? 'r' : fl.l_type == F_WRLCK ? 'w' : 'u', fl.l_whence == SEEK_CUR ? "+" : fl.l_whence == SEEK_END ? "-" : "", (long) fl.l_start, (long) fl.l_len),
        INTVAL((int64_t) fl.l_pid) };
    *out = px_rec("posix_lock", f, 2);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t bt_ICNx24UFCNTL(DESCR_t *args, int nargs) {
    int fd = nargs >= 2 ? pf_fd(args[0]) : -1, cmd, r;
    const char *a = nargs >= 2 ? px_str(args[1]) : NULL, *o = nargs >= 3 && args[2].v == DT_S ? rt_cstr_d(args[2]) : "";
    int64_t oi = nargs >= 3 && args[2].v == DT_I ? args[2].i : 0;
    DESCR_t res;
    px_begin();
    if (fd < 0 || !a || !*a) return FAILDESCR;
    switch (*a) {
        case 'F':
        cmd = F_SETFL;
        break;
        case 'f':
        cmd = F_GETFL;
        break;
        case 'X':
        cmd = F_SETFD;
        break;
        case 'x':
        cmd = F_GETFD;
        break;
        case 'L':
        cmd = F_SETLK;
        break;
        case 'l':
        cmd = F_GETLK;
        break;
        case 'W':
        cmd = F_SETLKW;
        break;
        case 'O':
        cmd = F_SETOWN;
        break;
        case 'o':
        cmd = F_GETOWN;
        break;
        default:
        return FAILDESCR;
    }
    if (cmd == F_SETLK || cmd == F_GETLK || cmd == F_SETLKW) return pf_lock(fd, cmd, o, &res) ? res : FAILDESCR;
    if (cmd == F_SETFL) { oi = 0; for (const char *c = o; *c; c++) { if (*c == 'd') oi |= O_NONBLOCK; else if (*c == 'a') oi |= O_APPEND; else return FAILDESCR; } }
    r = fcntl(fd, cmd, (long) oi);
    if (r < 0) return px_fail();
    if (cmd == F_GETFL) return px_fmt("%s%s", r & O_APPEND ? "a" : "", r & O_NONBLOCK ? "d" : "");
    return INTVAL(r);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pf_pending(DESCR_t d) { FILE *fp = d.v == DT_FH ? fh_get((int) d.i) : NULL; return fp && fp->_IO_read_ptr < fp->_IO_read_end; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t bt_ICNx24USELECT(DESCR_t *args, int nargs) {
    int total = nargs, timeout = -1, n = 0, ready = 0;
    DESCR_t *el;
    int ne;
    for (int k = 0; k < nargs; k++) if (rt_lv_is_list(args[k], &el, &ne)) total += ne;
    DESCR_t *fs = (DESCR_t *) rt_heap_alloc_c((size_t) (total + 1) * sizeof *fs), *rs = (DESCR_t *) rt_heap_alloc_c((size_t) (total + 1) * sizeof *rs);
    struct pollfd *pf = (struct pollfd *) rt_heap_alloc_c((size_t) (total + 1) * sizeof *pf);
    px_begin();
    for (int k = 0; k < nargs; k++) {
        if (k == nargs - 1 && args[k].v == DT_I) { timeout = (int) args[k].i; continue; }
        if (args[k].v == DT_FH) { fs[n++] = args[k]; continue; }
        if (!rt_lv_is_list(args[k], &el, &ne)) return FAILDESCR;
        for (int j = 0; j < ne; j++) { if (el[j].v != DT_FH) return FAILDESCR; fs[n++] = el[j]; }
    }
    for (int i = 0; i < n; i++) { pf[i].fd = pf_fd(fs[i]); pf[i].events = POLLIN; pf[i].revents = 0; if (pf[i].fd < 0) return FAILDESCR; if (pf_pending(fs[i])) { timeout = 0; } }
    int r = poll(pf, (nfds_t) n, timeout);
    if (r < 0 && errno != EINTR) return px_fail();
    for (int i = 0; i < n; i++) if (pf_pending(fs[i]) || (r > 0 && (pf[i].revents & (POLLIN | POLLHUP | POLLERR)))) rs[ready++] = fs[i];
    return px_list(rs, ready);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t bt_ICNx24URECEIVE(DESCR_t *args, int nargs) {
    int fd = nargs == 1 && args[0].v == DT_FH ? pf_fd(args[0]) : -1;
    if (fd < 0) return FAILDESCR;
    px_begin();
    ssize_t n = recv(fd, NULL, 0, MSG_PEEK | MSG_TRUNC);
    if (n < 0) return px_fail();
    char *buf = (char *) rt_heap_alloc_c((size_t) n + 1);
    struct sockaddr_storage sa;
    socklen_t sl = sizeof sa;
    if (recvfrom(fd, buf, (size_t) n, 0, (struct sockaddr *) &sa, &sl) < 0) return px_fail();
    char host[NI_MAXHOST], serv[NI_MAXSERV];
    if (getnameinfo((struct sockaddr *) &sa, sl, host, sizeof host, serv, sizeof serv, NI_NUMERICSERV) != 0) return px_fail();
    DESCR_t f[2] = { px_fmt("%s:%s", host, serv), BSTRVAL(buf, n) };
    DatType *t = dat_find_type("posix_message");
    if (!t) t = dat_register("posix_message(addr,msg)");
    return dat_construct(t, f, 2);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t bt_ICNx24USEND(DESCR_t *args, int nargs) {
    if (nargs != 2 || args[0].v != DT_S || args[1].v != DT_S) return FAILDESCR;
    px_begin();
    const char *addr = rt_cstr_d(args[0]);
    const char *c = strrchr(addr, ':');
    if (!c) { errno = EINVAL; return px_fail(); }
    char host[c - addr + 1];
    memcpy(host, addr, (size_t) (c - addr));
    host[c - addr] = 0;
    struct addrinfo hint = { 0 }, *ai = NULL;
    hint.ai_family = AF_INET;
    hint.ai_socktype = SOCK_DGRAM;
    if (getaddrinfo(*host ? host : NULL, c + 1, &hint, &ai) != 0 || !ai) { errno = EADDRNOTAVAIL; return px_fail(); }
    int fd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
    if (fd < 0) { freeaddrinfo(ai); return px_fail(); }
    ssize_t n = sendto(fd, args[1].s, (size_t) args[1].slen, 0, ai->ai_addr, ai->ai_addrlen);
    int e = errno;
    close(fd);
    freeaddrinfo(ai);
    if (n < 0) { errno = e; return px_fail(); }
    return NULVCL;
}
