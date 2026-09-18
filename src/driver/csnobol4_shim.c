#include <stdio.h>
#include "ct_arena.h"
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include "include/h.h"
#include "include/snotypes.h"
#include "include/macros.h"
#include "include/lib.h"
#include "equ.h"
#include "res.h"
#include "data.h"
typedef struct { char *name; char *val_str; } CsnNvPair;
typedef void (*csn_step_fn)(int stno, void *arg);
csn_step_fn  g_csn_step_hook = NULL;
void        *g_csn_step_arg  = NULL;
int          g_csn_stno      = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void csn_step_reset(void) { g_csn_stno = 0; }
extern jmp_buf endex_jmpbuf;
static jmp_buf _csn_step_jmp;
static int     _csn_step_target = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void _csn_step_cb(int stno, void *arg) {
    (void)arg;
    if (stno >= _csn_step_target) longjmp(_csn_step_jmp, 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *csn_descr_to_str(ptr_t vp) {
    int vtype = (int)D_V(vp);
    char buf[256];
    if (vtype == S) {
        struct spec sp; struct spec *spp = &sp;
        X_LOCSP(spp, vp);
        int len = (int)S_L(spp);
        if (len <= 0) return ct_strdup("");
        if (len > 4096) len = 4096;
        char *s = ct_alloc((size_t)(len + 1));
        if (!s) return ct_strdup("?");
        memcpy(s, S_SP(spp), (size_t)len);
        s[len] = '\0';
        return s;
    } else if (vtype == I) {
        snprintf(buf, sizeof buf, "%ld", (long)D_A(vp));
        return ct_strdup(buf);
    } else if (vtype == R) {
        real_t rv; int_t ia = D_A(vp);
        memcpy(&rv, &ia, sizeof rv);
        snprintf(buf, sizeof buf, "%g", (double)rv);
        return ct_strdup(buf);
    } else if (vtype == 0) {
        return ct_strdup("");
    } else {
        snprintf(buf, sizeof buf, "<type%d>", vtype);
        return ct_strdup(buf);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int csn_nv_snapshot(CsnNvPair **out_pairs, int *out_count) {
    int cap = 64, n = 0;
    CsnNvPair *pairs = ct_alloc((size_t)cap * sizeof(CsnNvPair));
    if (!pairs) { *out_pairs = NULL; *out_count = 0; return -1; }
    ptr_t bucket = OBLIST;
    ptr_t obend  = OBEND;
    while (1) {
        bucket += DESCR;
        if (D_PTR(bucket) > D_PTR(obend)) break;
        ptr_t node = bucket;
        while (1) {
            node = D_A(node + LNKFLD);
            if (node == 0) break;
            ptr_t vp = node + DESCR;
            if (D_V(vp) == 0 && D_A(vp) == 0) continue;
            struct spec nsp; struct spec *nspp = &nsp;
            X_LOCSP(nspp, node);
            int nlen = (int)S_L(nspp);
            if (nlen <= 0) continue;
            char *name = ct_alloc((size_t)(nlen + 1));
            if (!name) continue;
            memcpy(name, S_SP(nspp), (size_t)nlen);
            name[nlen] = '\0';
            char *val = csn_descr_to_str(vp);
            if (n >= cap) {
                cap *= 2;
                CsnNvPair *tmp = ct_grow(pairs, (size_t)cap * sizeof(CsnNvPair));
                if (!tmp) { ct_drop(name); ct_drop(val); break; }
                pairs = tmp;
            }
            pairs[n].name = name; pairs[n].val_str = val; n++;
        }
    }
    *out_pairs = pairs; *out_count = n; return n;
}
int snobol4_main(int argc, char *argv[]);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int csnobol4_run_steps(const char *core_path, int step_limit,
                       CsnNvPair **out_pairs, int *out_count) {
    *out_pairs = NULL; *out_count = 0;
    if (!core_path || step_limit <= 0) return -1;
    _csn_step_target = step_limit;
    g_csn_stno = 0;
    g_csn_step_hook = _csn_step_cb;
    g_csn_step_arg  = NULL;
    char *argv_csn[] = { (char *)"csnobol4", (char *)core_path, NULL };
    if (setjmp(_csn_step_jmp) == 0) {
        memcpy(&endex_jmpbuf, &_csn_step_jmp, sizeof(jmp_buf));
        snobol4_main(2, argv_csn);
    }
    g_csn_step_hook = NULL;
    csn_nv_snapshot(out_pairs, out_count);
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void csn_nv_snapshot_free(CsnNvPair *pairs, int n) {
    if (!pairs) return;
    for (int i = 0; i < n; i++) { ct_drop(pairs[i].name); ct_drop(pairs[i].val_str); }
    ct_drop(pairs);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void cleanup(void) { }
