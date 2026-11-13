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
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <strings.h>
#include <sys/un.h>
#include "driver/driver_data.h"
#include "driver/driver_globals.h"
#include "ir/ct_arena.h"
#include "ir/descr.h"
#include "posix_leaves.h"
#include "runtime/core/g_runtime.h"
#include "runtime/core/g_runtime_misc.h"
#include "runtime/core/table.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/rt_arena.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void px_begin(void) { g_rt_misc.posix_errno = 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t px_fail(void) { g_rt_misc.posix_errno = errno; return FAILDESCR; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *px_str(DESCR_t d) { return d.v == DT_S ? rt_cstr_d(d) : NULL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t px_rec(const char *tn, DESCR_t *f, int n) { DatType *t = dat_find_type(tn); return t ? dat_construct(t, f, n) : FAILDESCR; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static FILE *px_sock_unix(const char *path, int listen_) {
    struct sockaddr_un su = { 0 };
    int fd, ls;
    if (strlen(path) >= sizeof su.sun_path) { errno = ENAMETOOLONG; return NULL; }
    su.sun_family = AF_UNIX;
    memcpy(su.sun_path, path, strlen(path));
    fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0) return NULL;
    if (listen_) {
        ls = fd;
        fd = bind(ls, (struct sockaddr *) &su, sizeof su) == 0 && listen(ls, 1) == 0 ? accept(ls, NULL, NULL) : -1;
        close(ls);
    } else if (connect(fd, (struct sockaddr *) &su, sizeof su) != 0) {
        close(fd);
        fd = -1;
    }
    FILE *fp = fd >= 0 ? fdopen(fd, "r+") : NULL;
    if (!fp && fd >= 0) close(fd);
    if (fp) setvbuf(fp, NULL, _IOLBF, 0);
    return fp;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { int fd; socklen_t len; struct sockaddr_storage sa; } px_dg_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static ssize_t px_dg_read(void *c, char *b, size_t n) { return recv(((px_dg_t *) c)->fd, b, n, 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static ssize_t px_dg_write(void *c, const char *b, size_t n) { px_dg_t *d = (px_dg_t *) c; return sendto(d->fd, b, n, 0, (struct sockaddr *) &d->sa, d->len); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int px_dg_close(void *c) { px_dg_t *d = (px_dg_t *) c; int r = close(d->fd); ct_drop(d); return r; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static FILE *px_dg_open(int fd, const struct addrinfo *ai) {
    px_dg_t *d = (px_dg_t *) ct_zalloc(1, sizeof *d);
    cookie_io_functions_t io = { px_dg_read, px_dg_write, NULL, px_dg_close };
    d->fd = fd;
    d->len = ai->ai_addrlen;
    memcpy(&d->sa, ai->ai_addr, ai->ai_addrlen);
    FILE *fp = fopencookie(d, "r+", io);
    if (!fp) { ct_drop(d); return NULL; }
    fp->_fileno = fd;
    setvbuf(fp, NULL, _IONBF, 0);
    return fp;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int px_sock_bad(void) { g_rt_misc.icn_errnumber = 1310; g_rt_misc.icn_errtext = "bad socket attribute"; g_rt_misc.icn_err_valid = 1; errno = 0; return -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int px_sock_errno(void) { g_rt_misc.icn_errtext = strerror(errno); g_rt_misc.icn_err_valid = 1; return -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int px_sock_proto(const char *v) {
    const struct {
        const char *n;
        int p;
    } t[] = { { "icmp", 1 }, { "icmpv6", 58 }, { "icmp6", 58 }, { "igmp", 2 }, { "tcp", 6 }, { "udp", 17 }, { "gre", 47 }, { "ospf", 89 }, { "pim", 103 }, { "raw", 255 } };
    if (!*v) return -1;
    for (size_t k = 0; k < sizeof t / sizeof t[0]; k++) if (!strcasecmp(v, t[k].n)) return t[k].p;
    if (strlen(v) > 3) return -1;
    for (const char *q = v; *q; q++) if (*q < '0' || *q > '9') return -1;
    long n = strtol(v, NULL, 10);
    return n <= 255 ? (int) n : -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int px_key_is(const char *s, size_t kl, const char *k) { return strlen(k) == kl && !strncmp(s, k, kl); }
static int px_yesno(const char *v) { return !strcmp(v, "yes") ? 1 : !strcmp(v, "no") ? 0 : -1; }
static int px_digits(const char *v) { if (!*v) return 0; for (const char *q = v; *q; q++) if (*q < '0' || *q > '9') return 0; return 1; }
static int px_addr_part(const char *v, size_t n, struct in_addr *x) { char b[INET_ADDRSTRLEN]; if (n == 0 || n >= sizeof b) return 0; memcpy(b, v, n); b[n] = 0; return inet_pton(AF_INET, b, x) == 1; }
static int px_inet_ok(const char *v) { struct in_addr x; return px_addr_part(v, strlen(v), &x); }
static int px_group_ok(const char *v) {
    struct in_addr x;
    const char *c = strchr(v, ',');
    if (!c) return px_inet_ok(v);
    return px_addr_part(v, (size_t) (c - v), &x) && px_addr_part(c + 1, strlen(c + 1), &x);
}
static int px_mcast_member(int fd, const char *v, struct in_addr mif, int join) {
    const char *c = strchr(v, ',');
    struct in_addr g, src;
    if (!c) {
        struct ip_mreq m;
        if (!px_addr_part(v, strlen(v), &g)) return -1;
        m.imr_multiaddr = g;
        m.imr_interface = mif;
        return setsockopt(fd, IPPROTO_IP, join ? IP_ADD_MEMBERSHIP : IP_DROP_MEMBERSHIP, &m, sizeof m);
    }
    if (!px_addr_part(v, (size_t) (c - v), &g) || !px_addr_part(c + 1, strlen(c + 1), &src)) return -1;
    struct ip_mreq_source m;
    m.imr_multiaddr = g;
    m.imr_sourceaddr = src;
    m.imr_interface = mif;
    return setsockopt(fd, IPPROTO_IP, join ? IP_ADD_SOURCE_MEMBERSHIP : IP_DROP_SOURCE_MEMBERSHIP, &m, sizeof m);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int px_mcast_grp(int fd, struct in_addr g, struct in_addr mif, int join) {
    struct ip_mreq m;
    m.imr_multiaddr = g;
    m.imr_interface = mif;
    return setsockopt(fd, IPPROTO_IP, join ? IP_ADD_MEMBERSHIP : IP_DROP_MEMBERSHIP, &m, sizeof m);
}
static int px_mcast_ssm(int fd, struct in_addr g, struct in_addr src, struct in_addr mif, int join) {
    struct ip_mreq_source m;
    m.imr_multiaddr = g;
    m.imr_sourceaddr = src;
    m.imr_interface = mif;
    return setsockopt(fd, IPPROTO_IP, join ? IP_ADD_SOURCE_MEMBERSHIP : IP_DROP_SOURCE_MEMBERSHIP, &m, sizeof m);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int px_sock_attrs(const DESCR_t *a, int na, int *proto, int *hdrincl) {
    *proto = -1;
    *hdrincl = -1;
    for (int k = 0; k < na; k++) {
        const char *s = px_str(a[k]);
        const char *eq = s ? strchr(s, '=') : NULL;
        if (!eq) return px_sock_bad();
        size_t kl = (size_t) (eq - s);
        const char *v = eq + 1;
        if (px_key_is(s, kl, "proto")) {
            *proto = px_sock_proto(v);
            if (*proto < 0) return px_sock_bad();
        } else if (px_key_is(s, kl, "hdrincl")) {
            if (!strcmp(v, "yes")) *hdrincl = 1;
            else if (!strcmp(v, "no")) *hdrincl = 0;
            else return px_sock_bad();
        } else if (px_key_is(s, kl, "reuseaddr") || px_key_is(s, kl, "reuseport") || px_key_is(s, kl, "broadcast") || px_key_is(s, kl, "mcastloop")) {
            if (px_yesno(v) < 0) return px_sock_bad();
        } else if (px_key_is(s, kl, "ttl") || px_key_is(s, kl, "rcvbuf") || px_key_is(s, kl, "sndbuf")) {
            if (!px_digits(v)) return px_sock_bad();
        } else if (px_key_is(s, kl, "iface")) {
            if (!px_inet_ok(v)) return px_sock_bad();
        } else if (px_key_is(s, kl, "source")) {
            if (!px_inet_ok(v)) return px_sock_bad();
        } else if (px_key_is(s, kl, "join") || px_key_is(s, kl, "leave")) {
            if (!px_group_ok(v)) return px_sock_bad();
        } else return px_sock_bad();
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int px_sock_apply(int fd, const DESCR_t *a, int na, int prebind, const struct in_addr *grp, const struct in_addr *asrc) {
    struct in_addr mif;
    socklen_t ml = sizeof mif;
    if (getsockopt(fd, IPPROTO_IP, IP_MULTICAST_IF, &mif, &ml) != 0) mif.s_addr = htonl(INADDR_ANY);
    int joined = 0, sourced = 0;
    for (int k = 0; k < na; k++) {
        const char *s = px_str(a[k]);
        const char *eq = strchr(s, '=');
        size_t kl = (size_t) (eq - s);
        const char *v = eq + 1;
        int yn = px_yesno(v), iv = yn >= 0 ? yn : (int) strtol(v, NULL, 10), rc = 0;
        unsigned char uc = (unsigned char) iv;
        if (prebind) {
            if (px_key_is(s, kl, "reuseaddr")) rc = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &iv, sizeof iv);
            else if (px_key_is(s, kl, "reuseport")) rc = setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &iv, sizeof iv);
        } else if (px_key_is(s, kl, "broadcast")) rc = setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &iv, sizeof iv);
        else if (px_key_is(s, kl, "rcvbuf")) rc = setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &iv, sizeof iv);
        else if (px_key_is(s, kl, "sndbuf")) rc = setsockopt(fd, SOL_SOCKET, SO_SNDBUF, &iv, sizeof iv);
        else if (px_key_is(s, kl, "ttl")) {
            rc = setsockopt(fd, IPPROTO_IP, IP_TTL, &iv, sizeof iv);
            if (rc == 0) rc = setsockopt(fd, IPPROTO_IP, IP_MULTICAST_TTL, &uc, sizeof uc);
        } else if (px_key_is(s, kl, "mcastloop")) {
            uc = (unsigned char) (yn == 1);
            rc = setsockopt(fd, IPPROTO_IP, IP_MULTICAST_LOOP, &uc, sizeof uc);
        } else if (px_key_is(s, kl, "iface")) {
            struct in_addr x;
            if (!px_addr_part(v, strlen(v), &x)) return px_sock_bad();
            mif = x;
            rc = setsockopt(fd, IPPROTO_IP, IP_MULTICAST_IF, &mif, sizeof mif);
        } else if (px_key_is(s, kl, "source")) {
            struct in_addr src;
            if (!grp || !px_addr_part(v, strlen(v), &src)) return px_sock_bad();
            sourced = 1;
            rc = px_mcast_ssm(fd, *grp, src, mif, 1);
        } else if (px_key_is(s, kl, "join") || px_key_is(s, kl, "leave")) {
            joined |= px_key_is(s, kl, "join");
            rc = px_mcast_member(fd, v, mif, px_key_is(s, kl, "join"));
        }
        if (rc != 0) return px_sock_errno();
    }
    if (!prebind && grp) { int pr = 0; if (asrc) pr = px_mcast_ssm(fd, *grp, *asrc, mif, 1); else if (!joined && !sourced) pr = px_mcast_grp(fd, *grp, mif, 1); if (pr != 0) return px_sock_errno(); }
    return 0;
}
static const struct in_addr *px_bound_group(const struct addrinfo *ai) {
    const struct sockaddr_in *sin = (const struct sockaddr_in *) ai->ai_addr;
    return IN_MULTICAST(ntohl(sin->sin_addr.s_addr)) ? &sin->sin_addr : NULL;
}
FILE *px_sock_open(const char *addr, const char *mode, const DESCR_t *attrs, int nattrs) {
    int proto = -1, hdrincl = -1;
    if (px_sock_attrs(attrs, nattrs, &proto, &hdrincl) < 0) return NULL;
    if (strchr(mode, 'r')) {
        if (proto < 0) { px_sock_bad(); return NULL; }
        int rfd = socket(AF_INET, SOCK_RAW, proto), one_h = 1;
        if (rfd < 0) { px_sock_errno(); return NULL; }
        if (hdrincl == 1 && setsockopt(rfd, IPPROTO_IP, IP_HDRINCL, &one_h, sizeof one_h) != 0) { px_sock_errno(); close(rfd); return NULL; }
        FILE *rf = fdopen(rfd, "r+");
        if (!rf) { px_sock_errno(); close(rfd); return NULL; }
        setvbuf(rf, NULL, _IONBF, 0);
        return rf;
    }
    const char *c = strrchr(addr, ':');
    int listen_ = strchr(mode, 'a') != NULL, udp = strchr(mode, 'u') != NULL, fd = -1, one = 1;
    struct addrinfo hint = { 0 }, *ai = NULL;
    if (!c && !udp) return px_sock_unix(addr, listen_);
    if (!c) { errno = EINVAL; return NULL; }
    char host[c - addr + 1];
    memcpy(host, addr, (size_t) (c - addr));
    host[c - addr] = 0;
    struct in_addr asrc_v, *asrc = NULL;
    char *at = listen_ && udp ? strchr(host, '@') : NULL;
    if (at) { *at = 0; if (inet_pton(AF_INET, host, &asrc_v) != 1) { errno = EINVAL; return NULL; } asrc = &asrc_v; memmove(host, at + 1, strlen(at + 1) + 1); }
    hint.ai_family = AF_INET;
    hint.ai_socktype = udp ? SOCK_DGRAM : SOCK_STREAM;
    hint.ai_flags = listen_ ? AI_PASSIVE : 0;
    if (getaddrinfo(*host ? host : NULL, c + 1, &hint, &ai) != 0 || !ai) { errno = EADDRNOTAVAIL; return NULL; }
    fd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
    if (fd >= 0 && px_sock_apply(fd, attrs, nattrs, 1, NULL, NULL) != 0) { close(fd); fd = -1; }
    if (fd >= 0 && listen_ && udp) {
        setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof one);
        if (bind(fd, ai->ai_addr, ai->ai_addrlen) != 0) {
            close(fd);
            fd = -1;
        } else if (px_sock_apply(fd, attrs, nattrs, 0, px_bound_group(ai), px_bound_group(ai) ? asrc : NULL) != 0) {
            close(fd);
            fd = -1;
        }
    } else if (fd >= 0 && listen_) {
        setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof one);
        int ls = fd;
        fd = bind(ls, ai->ai_addr, ai->ai_addrlen) == 0 && listen(ls, 1) == 0 ? accept(ls, NULL, NULL) : -1;
        close(ls);
    } else if (fd >= 0 && udp) {
        if (px_sock_apply(fd, attrs, nattrs, 0, NULL, NULL) != 0) { close(fd); freeaddrinfo(ai); return NULL; }
        FILE *dg = px_dg_open(fd, ai);
        freeaddrinfo(ai);
        if (!dg) close(fd);
        return dg;
    } else if (fd >= 0 && connect(fd, ai->ai_addr, ai->ai_addrlen) != 0) {
        close(fd);
        fd = -1;
    }
    if (fd >= 0 && !udp && px_sock_apply(fd, attrs, nattrs, 0, NULL, NULL) != 0) { close(fd); fd = -1; }
    freeaddrinfo(ai);
    FILE *fp = fd >= 0 ? fdopen(fd, "r+") : NULL;
    if (!fp && fd >= 0) close(fd);
    if (fp) setvbuf(fp, NULL, udp ? _IONBF : _IOLBF, 0);
    return fp;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void px_mgr_note(int ix, struct in_addr g, struct in_addr src, int join) {
    fh_slot_t *h = &g_fh[ix];
    int w = 0, have = 0;
    for (int k = 0; k < h->nmgr; k += 2) {
        int same = h->mgr[k] == g.s_addr && h->mgr[k + 1] == src.s_addr;
        have |= same;
        if (join || !same) { h->mgr[w] = h->mgr[k]; h->mgr[w + 1] = h->mgr[k + 1]; w += 2; }
    }
    h->nmgr = w;
    if (!join || have) return;
    unsigned int *q = (unsigned int *) rt_gcheap_grow_block((void *) h->mgr, (uint16_t) HB_WSB, (uint64_t) (h->nmgr + 2) * sizeof(unsigned int));
    if (!q) return;
    q[h->nmgr] = g.s_addr;
    q[h->nmgr + 1] = src.s_addr;
    h->mgr = q;
    h->nmgr += 2;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void px_sock_note(int ix, const DESCR_t *a, int na) {
    if (ix < 0 || ix >= FH_N || !g_fh[ix].fp) return;
    int fd = fileno(g_fh[ix].fp);
    struct sockaddr_in b;
    socklen_t bl = sizeof b;
    int bound = fd >= 0 && getsockname(fd, (struct sockaddr *) &b, &bl) == 0 && b.sin_family == AF_INET && IN_MULTICAST(ntohl(b.sin_addr.s_addr));
    for (int k = 0; k < na; k++) {
        const char *s = px_str(a[k]), *eq = s ? strchr(s, '=') : NULL;
        if (!eq) continue;
        size_t kl = (size_t) (eq - s);
        const char *v = eq + 1, *c = strchr(v, ',');
        struct in_addr g, src;
        src.s_addr = 0;
        if (px_key_is(s, kl, "source")) { if (bound && px_addr_part(v, strlen(v), &src)) px_mgr_note(ix, b.sin_addr, src, 1); continue; }
        if (!px_key_is(s, kl, "join") && !px_key_is(s, kl, "leave")) continue;
        if (!px_addr_part(v, c ? (size_t) (c - v) : strlen(v), &g) || (c && !px_addr_part(c + 1, strlen(c + 1), &src))) continue;
        px_mgr_note(ix, g, src, px_key_is(s, kl, "join"));
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t bt_ICNx24UATTRIB(DESCR_t *args, int nargs) {
    if (nargs < 2 || args[0].v != DT_FH) return FAILDESCR;
    FILE *fp = fh_get((int) args[0].i);
    if (!fp) return FAILDESCR;
    int fd = fileno(fp), proto = -1, hdrincl = -1;
    if (px_sock_attrs(args + 1, nargs - 1, &proto, &hdrincl) < 0) return FAILDESCR;
    if (px_sock_apply(fd, args + 1, nargs - 1, 1, NULL, NULL) != 0) return FAILDESCR;
    if (px_sock_apply(fd, args + 1, nargs - 1, 0, NULL, NULL) != 0) return FAILDESCR;
    px_sock_note((int) args[0].i, args + 1, nargs - 1);
    const char *s = px_str(args[1]), *eq = s ? strchr(s, '=') : NULL;
    if (!eq) return FAILDESCR;
    size_t kl = (size_t) (eq - s);
    if (px_key_is(s, kl, "ttl")) { unsigned char t = 0; socklen_t tl = sizeof t; if (getsockopt(fd, IPPROTO_IP, IP_MULTICAST_TTL, &t, &tl) != 0) return px_fail(); return INTVAL((int64_t) t); }
    if (px_key_is(s, kl, "rcvbuf") || px_key_is(s, kl, "sndbuf")) {
        int v = 0;
        socklen_t vl = sizeof v;
        int opt = px_key_is(s, kl, "rcvbuf") ? SO_RCVBUF : SO_SNDBUF;
        if (getsockopt(fd, SOL_SOCKET, opt, &v, &vl) != 0) return px_fail();
        return INTVAL((int64_t) v);
    }
    return STRVAL(rt_heap_strdup_c(eq + 1));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *const px_sock_names[] = { "ttl", "hdrincl", "iface", "groups", "mcastloop", "reuseaddr", "reuseport", "broadcast", "rcvbuf", "sndbuf", NULL };
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t px_sock_get(int fd, int ix, const char *name) {
    if (!strcmp(name, "groups")) {
        int n = ix >= 0 && ix < FH_N ? g_fh[ix].nmgr / 2 : 0;
        if (n == 0) return FAILDESCR;
        DESCR_t el[n];
        for (int k = 0; k < n; k++) {
            struct in_addr g, src;
            char it[2 * INET_ADDRSTRLEN + 1];
            size_t n0 = 0;
            g.s_addr = g_fh[ix].mgr[2 * k];
            src.s_addr = g_fh[ix].mgr[2 * k + 1];
            if (src.s_addr) { if (!inet_ntop(AF_INET, &src, it, INET_ADDRSTRLEN)) return FAILDESCR; n0 = strlen(it); it[n0++] = '@'; }
            if (!inet_ntop(AF_INET, &g, it + n0, INET_ADDRSTRLEN)) return FAILDESCR;
            el[k] = STRVAL(rt_heap_strdup_c(it));
        }
        return px_list(el, n);
    }
    if (!strcmp(name, "ttl")) { unsigned char t = 0; socklen_t tl = sizeof t; if (getsockopt(fd, IPPROTO_IP, IP_MULTICAST_TTL, &t, &tl) != 0) return FAILDESCR; return INTVAL((int64_t) t); }
    if (!strcmp(name, "iface")) {
        struct in_addr a;
        socklen_t al = sizeof a;
        char txt[INET_ADDRSTRLEN];
        if (getsockopt(fd, IPPROTO_IP, IP_MULTICAST_IF, &a, &al) != 0 || !inet_ntop(AF_INET, &a, txt, sizeof txt)) return FAILDESCR;
        return STRVAL(rt_heap_strdup_c(txt));
    }
    if (!strcmp(name, "rcvbuf") || !strcmp(name, "sndbuf")) {
        int v = 0;
        socklen_t vl = sizeof v;
        if (getsockopt(fd, SOL_SOCKET, !strcmp(name, "rcvbuf") ? SO_RCVBUF : SO_SNDBUF, &v, &vl) != 0) return FAILDESCR;
        return INTVAL((int64_t) v);
    }
    if (!strcmp(name, "hdrincl") || !strcmp(name, "mcastloop")) {
        unsigned char c = 0;
        int i = 0, rc;
        socklen_t l = sizeof i;
        if (!strcmp(name, "mcastloop")) { l = sizeof c; rc = getsockopt(fd, IPPROTO_IP, IP_MULTICAST_LOOP, &c, &l); i = c; } else rc = getsockopt(fd, IPPROTO_IP, IP_HDRINCL, &i, &l);
        if (rc != 0) return FAILDESCR;
        return i ? STRVAL(rt_heap_strdup_c("yes")) : NULVCL;
    }
    int opt = !strcmp(name, "reuseaddr") ? SO_REUSEADDR : !strcmp(name, "reuseport") ? SO_REUSEPORT : !strcmp(name, "broadcast") ? SO_BROADCAST : -1;
    if (opt < 0) return FAILDESCR;
    int v = 0;
    socklen_t vl = sizeof v;
    if (getsockopt(fd, SOL_SOCKET, opt, &v, &vl) != 0) return FAILDESCR;
    return v ? STRVAL(rt_heap_strdup_c("yes")) : NULVCL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int px_sock_fd(DESCR_t f) {
    if (f.v != DT_FH) return -1;
    FILE *fp = fh_get((int) f.i);
    if (!fp) return -1;
    int fd = fileno(fp), ty = 0;
    socklen_t tl = sizeof ty;
    return getsockopt(fd, SOL_SOCKET, SO_TYPE, &ty, &tl) == 0 ? fd : -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t px_sock_peek(DESCR_t f, DESCR_t name) {
    int fd = px_sock_fd(f);
    if (fd < 0 || (name.v != DT_S && name.v != DT_SNUL)) return FAILDESCR;
    const char *n = rt_cstr_d(name);
    if (strcmp(n, "*")) return px_sock_get(fd, (int) f.i, n);
    TBBLK_t *tb = table_new();
    for (int k = 0; px_sock_names[k]; k++) { DESCR_t v = px_sock_get(fd, (int) f.i, px_sock_names[k]); if (v.v != DT_FAIL) table_set_descr_d(tb, STRVAL(rt_heap_strdup_c(px_sock_names[k])), v); }
    return TABLE_VAL(tb);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t px_sock_key_at(DESCR_t f, int64_t *cur) {
    int fd = px_sock_fd(f);
    if (fd < 0) return FAILDESCR;
    for (int64_t k = 0, j = 0; px_sock_names[k]; k++) if (px_sock_get(fd, (int) f.i, px_sock_names[k]).v != DT_FAIL && j++ == *cur) return STRVAL(rt_heap_strdup_c(px_sock_names[k]));
    return FAILDESCR;
}
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
