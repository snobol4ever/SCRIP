#include "prolog_driver.h"
#include "prolog_parse.h"
#include "prolog_lower.h"
#include "../../parsers/snobol4/scrip_cc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
enum { PL_CONSULT_DEPTH_MAX = 16, PL_CONSULT_FILES_MAX = 256 };
static const char * g_pl_consulted[PL_CONSULT_FILES_MAX];
static int g_pl_consulted_n = 0;
static int g_pl_consult_depth = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char * pl_consult_resolve(const char *spec, const char *from_file) {
    char cand[4096]; const char *slash; size_t dlen; FILE *probe; int has_ext;
    if (!spec || !*spec) return (char *)0;
    has_ext = (strlen(spec) > 3 && !strcmp(spec + strlen(spec) - 3, ".pl"));
    slash = from_file ? strrchr(from_file, '/') : (const char *)0;
    dlen = slash ? (size_t)(slash - from_file) + 1 : 0;
    if (dlen && dlen < sizeof cand - strlen(spec) - 8) {
        memcpy(cand, from_file, dlen); snprintf(cand + dlen, sizeof cand - dlen, "%s%s", spec, has_ext ? "" : ".pl");
        probe = fopen(cand, "r"); if (probe) { fclose(probe); return strdup(cand); } }
    snprintf(cand, sizeof cand, "%s%s", spec, has_ext ? "" : ".pl");
    probe = fopen(cand, "r"); if (probe) { fclose(probe); return strdup(cand); }
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
    for (int i = 0; i < g_pl_consulted_n; i++) if (!strcmp(g_pl_consulted[i], path)) { free(path); return; }
    if (g_pl_consulted_n < PL_CONSULT_FILES_MAX) g_pl_consulted[g_pl_consulted_n++] = path; else { free(path); return; }
    f = fopen(path, "r"); if (!f) return;
    fseek(f, 0, SEEK_END); flen = ftell(f); rewind(f);
    src = (char *)malloc((size_t)flen + 1); if (!src) { fclose(f); return; }
    if (fread(src, 1, (size_t)flen, f) != (size_t)flen) src[0] = src[0];
    src[flen] = '\0'; fclose(f);
    prolog_compile(src, path, &sub);
    free(src);
    if (sub) for (int i = 0; i < sub->n; i++) if (sub->c[i]) ast_push(prog, sub->c[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_consult_scan_goal(tree_t *g, const char **pending, int *npending) {
    if (!g || *npending >= PL_CONSULT_FILES_MAX) return;
    if (g->t == TT_FNC && g->v.sval && g->n == 2 && (!strcmp(g->v.sval, ",") || !strcmp(g->v.sval, ";") || !strcmp(g->v.sval, "->"))) {
        pl_consult_scan_goal(g->c[0], pending, npending); pl_consult_scan_goal(g->c[1], pending, npending); return; }
    if (g->t == TT_FNC && g->v.sval && g->n == 1 && !strcmp(g->v.sval, "consult")) {
        const char *sp = pl_consult_spec(g->c[0]);
        if (sp) { pending[(*npending)++] = sp; g->t = TT_QLIT; g->v.sval = (char *) "true"; g->n = 0; }
        return; }
    if (g->t == TT_MAKELIST && g->n > 0) {
        int all = 1;
        for (int k = 0; k < g->n; k++) if (!pl_consult_spec(g->c[k])) { all = 0; break; }
        if (!all) return;
        for (int k = 0; k < g->n && *npending < PL_CONSULT_FILES_MAX; k++) pending[(*npending)++] = pl_consult_spec(g->c[k]);
        g->t = TT_QLIT; g->v.sval = (char *) "true"; g->n = 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_splice_consults(tree_t *prog, const char *from_file) {
    const char *pending[PL_CONSULT_FILES_MAX]; int npending = 0;
    if (!prog || g_pl_consult_depth >= PL_CONSULT_DEPTH_MAX) return;
    for (int r = 0; r < prog->n; r++) {
        tree_t *s = prog->c[r]; tree_t *subj = s ? lp_s_expr(s, ":subj") : (tree_t *)0;
        if (!subj || subj->t == TT_CHOICE || subj->t == TT_CLAUSE) continue;
        pl_consult_scan_goal(subj, pending, &npending);
    }
    if (!npending) return;
    g_pl_consult_depth++;
    for (int i = 0; i < npending; i++) pl_consult_one(prog, pending[i], from_file);
    g_pl_consult_depth--;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void prolog_compile(const char *source, const char *filename, tree_t **out_ast)
{
    if (!filename) filename = "<stdin>";
    if (out_ast) *out_ast = NULL;
    PlProgram *pl = prolog_parse(source, filename);
    if (!pl) { fprintf(stderr, "prolog_compile: parse failed for %s\n", filename); return; }
    CODE_t *prog = prolog_lower(pl);
    if (out_ast && prog) { *out_ast = code_to_ast(prog); pl_splice_consults(*out_ast, filename); }
}
