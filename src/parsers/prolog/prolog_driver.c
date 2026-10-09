#include "prolog_driver.h"
#include "ct_arena.h"
#include "ct_vec.h"
#include "prolog_parse.h"
#include "prolog_lower.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
enum { PL_CONSULT_DEPTH_MAX = 16 };
static cv_t g_pl_consulted_v;
#define g_pl_consulted ((const char **)g_pl_consulted_v.p)
static int g_pl_consulted_n = 0;
static int g_pl_consult_depth = 0;
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
        for (int k = 0; k < (has_ext ? 1 : 4); k++) {
            size_t o = at ? 0 : dlen;
            if (o) memcpy(cand, from_file, o);
            snprintf(cand + o, sizeof cand - o, "%s%s", spec, has_ext ? "" : sfx[k]);
            probe = fopen(cand, "r"); if (probe) { fclose(probe); return ct_strdup(cand); } } }
    return (char *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * pl_consult_spec(const tree_t *t) {
    if (!t || !t->v.sval) return (const char *)0;
    if (t->t == TT_QLIT || t->t == TT_NAME) return t->v.sval;
    return (const char *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_consult_one(tree_t *prog, const char *spec, const char *from_file) {
    char *path = pl_consult_resolve(spec, from_file); FILE *f; long flen; char *src; tree_t *sub = (tree_t *)0;
    if (!path) { fprintf(stderr, "scrip: prolog: consult: cannot find source for '%s' (tried alongside %s and the working directory)\n", spec, from_file ? from_file : "<stdin>"); return; }
    for (int i = 0; i < g_pl_consulted_n; i++) if (!strcmp(g_pl_consulted[i], path)) { ct_drop(path); return; }
    cv_reserve(&g_pl_consulted_v, (uint32_t)sizeof(const char *), (uint64_t)g_pl_consulted_n + 1, "g_pl_consulted"); g_pl_consulted[g_pl_consulted_n++] = path;
    f = fopen(path, "r"); if (!f) return;
    fseek(f, 0, SEEK_END); flen = ftell(f); rewind(f);
    src = (char *)ct_alloc((size_t)flen + 1); if (!src) { fclose(f); return; }
    if (fread(src, 1, (size_t)flen, f) != (size_t)flen) src[0] = src[0];
    src[flen] = '\0'; fclose(f);
    prolog_compile(src, path, &sub);
    ct_drop(src);
    if (sub) for (int i = 0; i < sub->n; i++) if (sub->c[i]) ast_push(prog, sub->c[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_consult_scan_goal(tree_t *g, cv_t *pending, int *npending) {
    if (!g) return;
    if (g->t == TT_FNC && g->v.sval && g->n == 2 && (!strcmp(g->v.sval, ",") || !strcmp(g->v.sval, ";") || !strcmp(g->v.sval, "->"))) {
        pl_consult_scan_goal(g->c[0], pending, npending); pl_consult_scan_goal(g->c[1], pending, npending); return; }
    if (g->t == TT_FNC && g->v.sval && g->n == 1 && (!strcmp(g->v.sval, "consult") || !strcmp(g->v.sval, "ensure_loaded"))) {
        const char *sp = pl_consult_spec(g->c[0]);
        if (sp) { CV_PUSH(*pending, const char *) = sp; (*npending)++; g->t = TT_QLIT; g->v.sval = (char *) "true"; g->n = 0; }
        return; }
    if (g->t == TT_MAKELIST && g->n > 0) {
        int all = 1;
        for (int k = 0; k < g->n; k++) if (!pl_consult_spec(g->c[k])) { all = 0; break; }
        if (!all) return;
        for (int k = 0; k < g->n; k++) { CV_PUSH(*pending, const char *) = pl_consult_spec(g->c[k]); (*npending)++; }
        g->t = TT_QLIT; g->v.sval = (char *) "true"; g->n = 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * pl_stmt_subj(const tree_t *s) {
    if (!s) return (tree_t *)0;
    for (int i = 0; i < s->n; i++) { const tree_t *a = s->c[i]; if (a && a->t == TT_ATTR && a->v.sval && !strcmp(a->v.sval, ":subj")) return a->n > 0 ? a->c[0] : (tree_t *)0; }
    return (tree_t *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_splice_consults(tree_t *prog, const char *from_file) {
    cv_t pending = {0, 0, 0, 0}; int npending = 0;
    if (!prog || g_pl_consult_depth >= PL_CONSULT_DEPTH_MAX) return;
    for (int r = 0; r < prog->n; r++) {
        tree_t *subj = pl_stmt_subj(prog->c[r]);
        if (!subj || subj->t == TT_CHOICE || subj->t == TT_CLAUSE) continue;
        pl_consult_scan_goal(subj, &pending, &npending);
    }
    if (!npending) return;
    g_pl_consult_depth++;
    for (int i = 0; i < npending; i++) pl_consult_one(prog, CV_AT(pending, const char *, i), from_file);
    g_pl_consult_depth--;
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
    tree_t *prog = prolog_lower(pl);
    if (out_ast && prog) { *out_ast = prog; pl_splice_consults(*out_ast, filename); }
}
