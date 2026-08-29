#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sync_monitor.h"
#include "core.h"
#include "runtime/builtins/gen_runtime.h"
#include "runtime/builtins/resolution.h"
#include "lower.h"
#include "SM.h"
#include "driver.h"
#include "parsers/snobol4/scrip_cc.h"
#include "parsers/prolog/term.h"
#include "parsers/prolog/prolog_atom.h"
#ifdef WITH_CSNOBOL4
typedef struct { char *name; char *val_str; } CsnNvPair;
int  csnobol4_run_steps(const char *core_path, int step_limit,
                        CsnNvPair **out_pairs, int *out_count);
void csn_nv_snapshot_free(CsnNvPair *pairs, int n);
#else
typedef struct { char *name; char *val_str; } CsnNvPair;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int  csnobol4_run_steps(const char *p, int n, CsnNvPair **o, int *c)
    { (void)p;(void)n; *o=NULL;*c=0; return -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void csn_nv_snapshot_free(CsnNvPair *pairs, int n)
    { (void)pairs;(void)n; }
#endif
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void exec_snapshot_take(ExecSnapshot *s) {
    if (!s) return;
    s->nv_count = nv_snapshot(&s->nv_pairs);
    s->kw_stcount      = kw_stcount;
    s->kw_stlimit      = kw_stlimit;
    s->kw_anchor       = kw_anchor;
    s->frame_depth = frame_depth;
    s->resolve_trail_mark   = trail_mark(&g_resolve_trail);
    s->resolve_locals       = NULL;
    s->resolve_locals_count = 0;
    {
        int top = trail_mark(&g_resolve_trail);
        if (top > 0 && g_resolve_trail.stack) {
            s->resolve_locals = malloc((size_t)top * sizeof(*s->resolve_locals));
            int out = 0;
            for (int ti = 0; ti < top; ti++) {
                Term *var = g_resolve_trail.stack[ti];
                if (!var) continue;
                char nbuf[32];
                snprintf(nbuf, sizeof nbuf, "_V%d", var->saved_slot);
                Term *val = term_deref(var);
                char vbuf[256];
                if (!val || val->tag == TT_VAR) {
                    snprintf(vbuf, sizeof vbuf, "_");
                } else if (val->tag == TERM_ATOM) {
                    const char *aname = prolog_atom_name(val->atom_id);
                    snprintf(vbuf, sizeof vbuf, "%s", aname ? aname : "?");
                } else if (val->tag == TERM_INT) {
                    snprintf(vbuf, sizeof vbuf, "%ld", val->ival);
                } else if (val->tag == TERM_FLOAT) {
                    snprintf(vbuf, sizeof vbuf, "%g", val->fval);
                } else if (val->tag == TERM_COMPOUND) {
                    const char *fn = prolog_atom_name(val->compound.functor);
                    snprintf(vbuf, sizeof vbuf, "%s/%d", fn ? fn : "?", val->compound.arity);
                } else {
                    snprintf(vbuf, sizeof vbuf, "<term>");
                }
                s->resolve_locals[out].name    = strdup(nbuf);
                s->resolve_locals[out].val_str = strdup(vbuf);
                out++;
            }
            s->resolve_locals_count = out;
        }
    }
    s->frame_locals       = NULL;
    s->frame_locals_count = 0;
    if (frame_depth > 0) {
        int total = 0;
        for (int fi = 0; fi < frame_depth; fi++)
            total += frame_stack[fi].sc.n;
        if (total > 0) {
            s->frame_locals = malloc((size_t)total * sizeof(NvPair));
            int out = 0;
            for (int fi = 0; fi < frame_depth; fi++) {
                GenFrame *f = &frame_stack[fi];
                for (int si = 0; si < f->sc.n; si++) {
                    s->frame_locals[out].name = f->sc.e[si].name;
                    int slot = f->sc.e[si].slot;
                    s->frame_locals[out].val  = (slot >= 0 && slot < f->env_n)
                                              ? f->env[slot] : NULVCL;
                    out++;
                }
            }
            s->frame_locals_count = out;
        }
    }
    s->last_ok = -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void exec_snapshot_restore(const ExecSnapshot *s) {
    if (!s) return;
    nv_restore(s->nv_pairs, s->nv_count);
    kw_stcount = s->kw_stcount;
    kw_stlimit = s->kw_stlimit;
    kw_anchor  = s->kw_anchor;
    frame_depth = 0;
    trail_unwind(&g_resolve_trail, s->resolve_trail_mark);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void exec_snapshot_free(ExecSnapshot *s) {
    if (!s) return;
    s->nv_pairs = NULL;
    s->nv_count = 0;
    free(s->label_path);
    s->label_path     = NULL;
    s->label_path_n   = 0;
    s->label_path_cap = 0;
    free(s->frame_locals);
    s->frame_locals       = NULL;
    s->frame_locals_count = 0;
    for (int i = 0; i < s->resolve_locals_count; i++) {
        free(s->resolve_locals[i].name);
        free(s->resolve_locals[i].val_str);
    }
    free(s->resolve_locals);
    s->resolve_locals       = NULL;
    s->resolve_locals_count = 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void label_path_append(ExecSnapshot *s, const char *lbl) {
    if (s->label_path_n >= s->label_path_cap) {
        int newcap = s->label_path_cap ? s->label_path_cap * 2 : 16;
        s->label_path = realloc(s->label_path, (size_t)newcap * sizeof(const char *));
        s->label_path_cap = newcap;
    }
    s->label_path[s->label_path_n++] = lbl;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void label_path_print(const char *tag, const ExecSnapshot *s) {
    fprintf(stderr, "  %-4s path:", tag);
    int printed = 0;
    for (int i = 0; i < s->label_path_n; i++) {
        if (!s->label_path[i]) continue;
        fprintf(stderr, "%s[%s]", printed ? " \u2192 " : " ", s->label_path[i]);
        printed++;
    }
    if (!printed) fprintf(stderr, " (no labels reached)");
    fprintf(stderr, "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int snap_diff(const ExecSnapshot *a, const char *a_name,
                     const ExecSnapshot *b, const char *b_name,
                     int verbose) {
    int ndiff = 0;
    for (int i = 0; i < a->nv_count; i++) {
        const char *name = a->nv_pairs[i].name;
        DESCR_t     va   = a->nv_pairs[i].val;
        DESCR_t     vb   = NULVCL;
        int found = 0;
        for (int j = 0; j < b->nv_count; j++) {
            if (strcmp(b->nv_pairs[j].name, name) == 0) {
                vb = b->nv_pairs[j].val; found = 1; break;
            }
        }
        const char *sa = VARVAL_fn(va);
        const char *sb = found ? VARVAL_fn(vb) : "";
        if (!sa) sa = "";
        if (!sb) sb = "";
        if (strcmp(sa, sb) != 0) {
            ndiff++;
            if (verbose)
                fprintf(stderr, "    %-12s  %s=%-20s  %s=%s\n",
                        name, a_name, sa, b_name, sb);
        }
    }
    for (int j = 0; j < b->nv_count; j++) {
        const char *name = b->nv_pairs[j].name;
        int found = 0;
        for (int i = 0; i < a->nv_count; i++) {
            if (strcmp(a->nv_pairs[i].name, name) == 0) { found = 1; break; }
        }
        if (!found) {
            const char *sb = VARVAL_fn(b->nv_pairs[j].val);
            if (!sb) sb = "";
            if (*sb) {
                ndiff++;
                if (verbose)
                    fprintf(stderr, "    %-12s  %s=%-20s  %s=%s\n",
                            name, a_name, "", b_name, sb);
            }
        }
    }
    return ndiff;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sync_monitor_run(const tree_t *prog, int verbose, const char *core_path) {
    (void)prog; (void)verbose; (void)core_path;
    fprintf(stderr, "[NO-SM-BB] sync_monitor: codegen deleted (FACT RULE); unavailable\n");
    return -1;
}
