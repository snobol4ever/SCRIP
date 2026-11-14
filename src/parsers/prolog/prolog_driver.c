#include "prolog_driver.h"
#include "parsers/g_parser.h"
#include "ir/ct_arena.h"
#include "ir/ct_vec.h"
#include "prolog_parse.h"
#include "prolog_lower.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include "ir/ast_tt_capt_immed_asgn.h"
#include "ir/ast.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
enum { PL_CONSULT_DEPTH_MAX = 16 };
#define g_pl_consulted ((const char **)g_parser.pl.consulted_v.p)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_regular_file(const char *p) { struct stat st; return !stat(p, &st) && S_ISREG(st.st_mode); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char * pl_consult_resolve(const char *spec, const char *from_file) {
    const char *const sfx[] = { ".pl", ".pro", ".prolog", ".plt" };
    const char *slash; size_t dlen, sl; FILE *probe; int has_ext = 0;
    if (!spec || !*spec) return (char *)0;
    sl = strlen(spec);
    for (int k = 0; k < 4; k++) if (sl > strlen(sfx[k]) && !strcmp(spec + sl - strlen(sfx[k]), sfx[k])) has_ext = 1;
    slash = from_file ? strrchr(from_file, '/') : (const char *)0;
    dlen = slash ? (size_t)(slash - from_file) + 1 : 0;
    char cand[dlen + sl + sizeof ".prolog"];
    for (int at = dlen ? 0 : 1; at < 2; at++) {
        for (int k = 0; k < (has_ext ? 1 : 5); k++) {
            int exact = has_ext || k == 4;
            size_t o = at ? 0 : dlen;
            if (o) memcpy(cand, from_file, o);
            snprintf(cand + o, sizeof cand - o, "%s%s", spec, exact ? "" : sfx[k]);
            probe = fopen(cand, "r"); if (probe) { fclose(probe); if (!exact || has_ext || pl_regular_file(cand)) return ct_strdup(cand); } } }
    return (char *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * pl_consult_spec(const tree_t *t) {
    if (!t || !t->v.sval) return (const char *)0;
    if (t->t == TT_QLIT || t->t == TT_NAME) return t->v.sval;
    return (const char *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char * pl_slurp(const char *path) {
    FILE *f = fopen(path, "r");
    long flen;
    char *src;
    if (!f) return (char *)0;
    fseek(f, 0, SEEK_END); flen = ftell(f); rewind(f);
    src = (char *)ct_alloc((size_t)flen + 1); if (!src) { fclose(f); return (char *)0; }
    if (fread(src, 1, (size_t)flen, f) != (size_t)flen) src[0] = src[0];
    src[flen] = '\0'; fclose(f);
    return src;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_consult_first(char *path) {
    for (int i = 0; i < g_parser.pl.consulted_n; i++) if (!strcmp(g_pl_consulted[i], path)) { ct_drop(path); return 0; }
    cv_reserve(&g_parser.pl.consulted_v, (uint32_t)sizeof(const char *), (uint64_t)g_parser.pl.consulted_n + 1, "g_pl_consulted"); g_pl_consulted[g_parser.pl.consulted_n++] = path;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_consult_text(tree_t *prog, const char *src, const char *path) {
    tree_t *sub = (tree_t *)0;
    prolog_compile(src, path, &sub);
    if (sub) for (int i = 0; i < sub->n; i++) if (sub->c[i]) ast_push(prog, sub->c[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_consult_one(tree_t *prog, const char *spec, const char *from_file) {
    char *path = pl_consult_resolve(spec, from_file), *src;
    if (!path) { fprintf(stderr, "scrip: prolog: consult: cannot find source for '%s' (tried alongside %s and the working directory)\n", spec, from_file ? from_file : "<stdin>"); return; }
    if (!pl_consult_first(path) || !(src = pl_slurp(path))) return;
    pl_consult_text(prog, src, path);
    ct_drop(src);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_xsb_exports(const char *s) {
    for (const char *l = s; l && *l; l = strchr(l, '\n') ? strchr(l, '\n') + 1 : (const char *)0) {
        while (*l == ' ' || *l == '\t') l++;
        if (l[0] != ':' || l[1] != '-') continue;
        for (l += 2; *l == ' ' || *l == '\t';) l++;
        if (!strncmp(l, "export", 6) && !(isalnum((unsigned char)l[6]) || l[6] == '_')) return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_import_one(tree_t *prog, const char *mod, const char *from_file) {
    const char *slash = from_file ? strrchr(from_file, '/') : (const char *)0;
    size_t dlen = slash ? (size_t)(slash - from_file) + 1 : 0, ml = strlen(mod);
    char xp[dlen + ml + sizeof ".P"], hp[dlen + ml + sizeof ".H"], *path, *src, *hdr;
    snprintf(xp, sizeof xp, "%.*s%s.P", (int)dlen, from_file ? from_file : "", mod);
    snprintf(hp, sizeof hp, "%.*s%s.H", (int)dlen, from_file ? from_file : "", mod);
    path = pl_regular_file(xp) ? ct_strdup(xp) : pl_consult_resolve(mod, from_file);
    if (!path || !pl_consult_first(path) || !(src = pl_slurp(path))) return;
    hdr = pl_regular_file(hp) ? pl_slurp(hp) : (char *)0;
    {
        int is_mod = pl_xsb_exports(src) || (hdr && pl_xsb_exports(hdr));
        size_t n = strlen(src) + (hdr ? strlen(hdr) : 0) + 2 * ml + 64;
        char *all = (char *)ct_alloc(n), *o = all;
        if (is_mod) {
            o += snprintf(o, n, ":- module('");
            for (const char *c = mod; *c; c++) { if (*c == '\'' || *c == '\\') *o++ = '\\'; *o++ = *c; }
            o += snprintf(o, n - (size_t)(o - all), "', []). ");
        }
        if (hdr) snprintf(o, n - (size_t)(o - all), "%s\n%s", hdr, src); else snprintf(o, n - (size_t)(o - all), "%s", src);
        pl_consult_text(prog, all, path);
        ct_drop(all);
    }
    if (hdr) ct_drop(hdr);
    ct_drop(src);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_text_loads_yall(const char *s) {
    for (const char *l = s; l && *l; l = strchr(l, '\n') ? strchr(l, '\n') + 1 : (const char *)0) {
        while (*l == ' ' || *l == '\t') l++;
        if (l[0] != ':' || l[1] != '-') continue;
        for (l += 2; *l == ' ' || *l == '\t';) l++;
        if (strncmp(l, "use_module(", 11)) continue;
        for (l += 11; *l == ' ' || *l == '\t';) l++;
        if (strncmp(l, "library(", 8)) continue;
        for (l += 8; *l == ' ' || *l == '\t';) l++;
        if (!strncmp(l, "yall", 4) && !(isalnum((unsigned char)l[4]) || l[4] == '_')) return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_goal_consults_yall(const tree_t *g, const char *from_file) {
    if (!g || g->t != TT_FNC || !g->v.sval) return 0;
    if (g->n == 2 && (!strcmp(g->v.sval, ",") || !strcmp(g->v.sval, ";") || !strcmp(g->v.sval, "->"))) return pl_goal_consults_yall(g->c[0], from_file) || pl_goal_consults_yall(g->c[1], from_file);
    if (g->n != 1 || (strcmp(g->v.sval, "consult") && strcmp(g->v.sval, "ensure_loaded"))) return 0;
    const char *sp = pl_consult_spec(g->c[0]);
    char *path = sp ? pl_consult_resolve(sp, from_file) : (char *)0, *src = path ? pl_slurp(path) : (char *)0;
    int hit = src && pl_text_loads_yall(src);
    if (src) ct_drop(src);
    if (path) ct_drop(path);
    return hit;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_consults_yall(const PlProgram *pl, const char *from_file) {
    for (const PlClause *cl = pl->head; cl; cl = cl->next) {
        const tree_t *tr = cl->tr;
        if (tr && tr->t == TT_CLAUSE && tr->n == 2 && tr->c[0] && tr->c[0]->t == TT_NUL && pl_goal_consults_yall(tr->c[1], from_file)) return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { const char *spec; int import; } pl_pend_t;
static void pl_consult_scan_goal(tree_t *g, cv_t *pending, int *npending) {
    if (!g) return;
    if (g->t == TT_FNC && g->v.sval && g->n == 2 && (!strcmp(g->v.sval, ",") || !strcmp(g->v.sval, ";") || !strcmp(g->v.sval, "->"))) {
        pl_consult_scan_goal(g->c[0], pending, npending); pl_consult_scan_goal(g->c[1], pending, npending); return; }
    if (g->t == TT_FNC && g->v.sval && g->n == 1 && (!strcmp(g->v.sval, "consult") || !strcmp(g->v.sval, "ensure_loaded"))) {
        const char *sp = pl_consult_spec(g->c[0]);
        if (sp) { CV_PUSH(*pending, pl_pend_t) = (pl_pend_t){ sp, 0 }; (*npending)++; g->t = TT_QLIT; g->v.sval = (char *) "true"; g->n = 0; }
        return; }
    if (g->t == TT_FNC && g->v.sval && g->n == 1 && !strcmp(g->v.sval, "import") && g->c[0] && g->c[0]->t == TT_FNC && g->c[0]->n == 2 && g->c[0]->v.sval && !strcmp(g->c[0]->v.sval, "from")) {
        const char *m = pl_consult_spec(g->c[0]->c[1]);
        if (m) { CV_PUSH(*pending, pl_pend_t) = (pl_pend_t){ m, 1 }; (*npending)++; }
        return; }
    if (g->t == TT_MAKELIST && g->n > 0) {
        int all = 1;
        for (int k = 0; k < g->n; k++) if (!pl_consult_spec(g->c[k])) { all = 0; break; }
        if (!all) return;
        for (int k = 0; k < g->n; k++) { CV_PUSH(*pending, pl_pend_t) = (pl_pend_t){ pl_consult_spec(g->c[k]), 0 }; (*npending)++; }
        g->t = TT_QLIT; g->v.sval = (char *) "true"; g->n = 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * pl_stmt_subj(const tree_t *s) {
    if (!s) return (tree_t *)0;
    for (int i = 0; i < s->n; i++) { const tree_t *a = s->c[i]; if (a && a->t == TT_ATTR && a->v.sval && !strcmp(a->v.sval, ":subj")) return a->n > 0 ? a->c[0] : (tree_t *)0; }
    return (tree_t *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_choice_prelude_only(const tree_t *ch) {
    int k = 0;
    if (!ch || ch->t != TT_CHOICE) return 0;
    for (int j = 0; j < ch->n; j++) if (ch->c[j] && ch->c[j]->t == TT_CLAUSE) { if (ch->c[j]->line != 0) return 0; k++; }
    return k > 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_splice_drop_shadowed(tree_t *prog) {
    int n = prog->n, w = 0;
    for (int r = 0; r < n; r++) {
        const tree_t *s = pl_stmt_subj(prog->c[r]);
        int drop = 0;
        if (s && s->v.sval && pl_choice_prelude_only(s)) for (int q = 0; q < n && !drop; q++) {
            const tree_t *o = pl_stmt_subj(prog->c[q]);
            drop = o && o != s && o->t == TT_CHOICE && o->v.sval && !strcmp(o->v.sval, s->v.sval) && o->n > 0 && !pl_choice_prelude_only(o);
        }
        if (!drop) prog->c[w++] = prog->c[r];
    }
    prog->n = w;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_splice_consults(tree_t *prog, const char *from_file) {
    cv_t pending = {0, 0, 0, 0}; int npending = 0, done = 0, n0, moved = 0;
    if (!prog || g_parser.pl.consult_depth >= PL_CONSULT_DEPTH_MAX) return;
    n0 = prog->n;
    int (*span)[2] = (int (*)[2])ct_zalloc((size_t)n0 + 1, sizeof *span);
    for (int r = 0; r < n0; r++) {
        tree_t *subj = pl_stmt_subj(prog->c[r]);
        if (!subj || subj->t == TT_CHOICE || subj->t == TT_CLAUSE) continue;
        pl_consult_scan_goal(subj, &pending, &npending);
        if (done == npending) continue;
        g_parser.pl.consult_depth++;
        span[r][0] = prog->n;
        for (; done < npending; done++) {
            const pl_pend_t *p = &CV_AT(pending, pl_pend_t, done);
            if (p->import) pl_import_one(prog, p->spec, from_file); else pl_consult_one(prog, p->spec, from_file);
        }
        span[r][1] = prog->n;
        moved += span[r][1] - span[r][0];
        g_parser.pl.consult_depth--;
    }
    if (moved) {
        tree_t **ord = (tree_t **)ct_alloc((size_t)prog->n * sizeof *ord);
        int k = 0;
        for (int r = 0; r < n0; r++) {
            ord[k++] = prog->c[r];
            for (int q = span[r][0]; q < span[r][1]; q++) ord[k++] = prog->c[q];
        }
        for (int q = 0; q < k; q++) prog->c[q] = ord[q];
        ct_drop(ord);
    }
    ct_drop(span);
    if (npending) pl_splice_drop_shadowed(prog);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *prolog_compile_parse(const char *source, const char *filename)
{
    return prolog_parse(source, filename ? filename : "<stdin>");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void prolog_compile(const char *source, const char *filename, tree_t **out_ast)
{
    if (out_ast) *out_ast = NULL;
    prolog_compile_finish(prolog_compile_parse(source, filename), filename, out_ast);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void prolog_compile_finish(void *parsed, const char *filename, tree_t **out_ast)
{
    if (!filename) filename = "<stdin>";
    if (out_ast) *out_ast = NULL;
    PlProgram *pl = (PlProgram *)parsed;
    if (!pl) { fprintf(stderr, "prolog_compile: parse failed for %s\n", filename); return; }
    prolog_preprocess(pl);
    if (pl->nerrors > 0) fprintf(stderr, "prolog: %d parse error(s) in %s\n", pl->nerrors, filename);
    if (pl->nclauses == 0) { if (out_ast) *out_ast = NULL; if (pl->nerrors > 0) exit(1); }
    if (!filename || strcmp(filename, "<prelude>") != 0) prolog_inject_prelude(pl, pl->src);
    pl->consults_yall = pl_consults_yall(pl, filename);
    tree_t *prog = prolog_lower(pl);
    if (out_ast && prog) { *out_ast = prog; pl_splice_consults(*out_ast, filename); }
}
