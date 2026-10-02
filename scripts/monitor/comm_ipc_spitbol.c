/*
 * comm_ipc_spitbol.c -- the COMM channel of demos/scrip/infinite_snobol4, cloned from the sync-step monitor's IPC
 * (Lon 2026-10-02: "So, clone the code for the IPC sync-step monitor and use it for this COMM as well."; ceo CEO-1419).
 * Same two named pipes and RS-terminated records as monitor_ipc_sync.c and the fork's monitor_ipc_spitbol.c, turned into
 * request/reply: the parent writes one request record to the request pipe, this side reads it, answers with one reply
 * record on the reply pipe, and the parent reads that before it sends the next. SPITBOL x64 LOAD() ABI, the fork's
 * convention (struct ldescr, scblk arguments, a string result's length in the f byte, so a
 * received record longer than 255 bytes is refused rather than cut).
 *   LOAD('COMM_OPEN(STRING,STRING)STRING', 'comm_ipc_spitbol.so')   request path, reply path -> request path
 *   LOAD('COMM_RECV()STRING', 'comm_ipc_spitbol.so')                 the next request; FAIL at end of stream
 *   LOAD('COMM_REPLY(STRING)STRING', 'comm_ipc_spitbol.so')          one reply record
 *   LOAD('COMM_CLOSE()STRING', 'comm_ipc_spitbol.so')
 * Entry names come lower-case (the fork folds a LOAD name under -b) and upper-case (under -bf it does not).
 * Build: gcc -shared -fPIC -O2 -Wall -o comm_ipc_spitbol.so comm_ipc_spitbol.c
 */
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/uio.h>
#define RS 0x1e
typedef long int_t;
typedef double real_t;
struct ldescr { union { int_t i; real_t f; } a; char f; unsigned int v; };
struct spitblk_sc { long typ; long len; char str[]; };
#define LA_ALIST struct ldescr *retval, unsigned nargs, struct ldescr *args
typedef int lret_t;
#define LDESCR_STR 'S'
#define RETSTR(CP, LEN) do { retval->a.i = (int_t)(uintptr_t)(CP); retval->f = (char)(LEN); retval->v = LDESCR_STR; return 1; } while (0)
#define RETNULL do { retval->a.i = 0; retval->f = 0; retval->v = LDESCR_STR; return 1; } while (0)
#define RETFAIL return 0
static struct spitblk_sc *scblk(int n, struct ldescr *args) { return (struct spitblk_sc *)(uintptr_t)args[n].a.i; }
static int slen(int n, struct ldescr *args) { struct spitblk_sc *sc = scblk(n, args); return sc ? (int)sc->len : 0; }
static const char *sptr(int n, struct ldescr *args) { struct spitblk_sc *sc = scblk(n, args); return sc ? sc->str : 0; }
static int req_fd = -1, rep_fd = -1;
static char req_path[4096];
static char rec[256];
static char inbuf[65536];
static int in_have = 0, in_pos = 0;
static int copy_arg(int n, char *buf, int bufsz, struct ldescr *args) {
    int len = slen(n, args); const char *p = sptr(n, args);
    if (len <= 0 || len >= bufsz || !p) return -1;
    memcpy(buf, p, len); buf[len] = 0; return len;
}
lret_t comm_open(LA_ALIST) {
    char rp[4096];
    int n = copy_arg(0, req_path, sizeof(req_path), args);
    if (n < 0 || copy_arg(1, rp, sizeof(rp), args) < 0) RETFAIL;
    if (req_fd >= 0) close(req_fd);
    if (rep_fd >= 0) close(rep_fd);
    req_fd = open(req_path, O_RDONLY);
    if (req_fd < 0) RETFAIL;
    rep_fd = open(rp, O_WRONLY);
    if (rep_fd < 0) { close(req_fd); req_fd = -1; RETFAIL; }
    in_have = in_pos = 0;
    RETSTR(req_path, n > 255 ? 255 : n);
}
lret_t comm_recv(LA_ALIST) {
    int len = 0;
    if (req_fd < 0) RETFAIL;
    for (;;) {
        if (in_pos >= in_have) {
            ssize_t r = read(req_fd, inbuf, sizeof(inbuf));
            if (r <= 0) RETFAIL;
            in_have = (int)r; in_pos = 0;
        }
        char c = inbuf[in_pos++];
        if (c == RS) break;
        if (len >= (int)sizeof(rec)) RETFAIL;
        rec[len++] = c;
    }
    RETSTR(rec, len);
}
lret_t comm_reply(LA_ALIST) {
    struct iovec iov[2];
    char rs = RS;
    const char *p = sptr(0, args); int n = slen(0, args);
    if (rep_fd < 0) RETFAIL;
    if (!p) { p = ""; n = 0; }
    iov[0].iov_base = (void *)p; iov[0].iov_len = (size_t)n;
    iov[1].iov_base = &rs; iov[1].iov_len = 1;
    if (writev(rep_fd, iov, 2) != (ssize_t)(n + 1)) RETFAIL;
    RETNULL;
}
lret_t comm_close(LA_ALIST) {
    if (req_fd >= 0) { close(req_fd); req_fd = -1; }
    if (rep_fd >= 0) { close(rep_fd); rep_fd = -1; }
    RETNULL;
}
lret_t COMM_OPEN(LA_ALIST) { return comm_open(retval, nargs, args); }
lret_t COMM_RECV(LA_ALIST) { return comm_recv(retval, nargs, args); }
lret_t COMM_REPLY(LA_ALIST) { return comm_reply(retval, nargs, args); }
lret_t COMM_CLOSE(LA_ALIST) { return comm_close(retval, nargs, args); }
