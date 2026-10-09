/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef RX_H
#define RX_H
#include <stddef.h>
enum { RXF_I = 1, RXF_M = 2, RXF_S = 4, RXF_R = 8 };
typedef struct RxCap { char kind; int idx; const char *name; int nlen; int rep; const char *name2; int nlen2; struct RxCap **kids; int nkids; } RxCap;
typedef struct RxEv { const RxCap *cap; int from, to, sub; } RxEv;
typedef struct RxEnv {
    void *ud;
    const char *(*rule)(void *ud, const char *name, int nlen, int *flags);
    unsigned (*fold)(void *ud, unsigned cp, int mode);
    int (*prop)(void *ud, const char *name, int nlen, unsigned cp);
} RxEnv;
typedef struct RxProg RxProg;
const RxCap *const *rx_tops(const RxProg *p, int *n);
typedef struct RxMatch { int ok; int from, to; int nev; RxEv *ev; const RxProg *prog; } RxMatch;
RxProg *rx_compile(const char *src, int flags, const RxEnv *env, const char **err);
int rx_exec(const RxProg *p, const char *subj, int slen, int start, int anchored, RxMatch *m);
#endif
