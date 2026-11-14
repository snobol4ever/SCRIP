#include "snocone_driver.h"
#include "ir/ct_arena.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ir/ast_tt_capt_immed_asgn.h"
#include "ir/ast.h"
#include "ir/ast_node_new.h"
tree_t *snocone_parse_tree(const char *src, const char *filename);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *snocone_compile_parse(const char *source, const char *filename)
{
    return snocone_parse_tree(source, filename ? filename : "<stdin>");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void snocone_compile_finish(void *parsed, const char *filename, tree_t **out_ast)
{
    (void)filename;
    if (out_ast) *out_ast = (tree_t *)parsed;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void snocone_compile(const char *source, const char *filename, tree_t **out_ast)
{
    if (out_ast) *out_ast = NULL;
    snocone_compile_finish(snocone_compile_parse(source, filename), filename, out_ast);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { const char **paths; int n, cap; } sci_imports_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sci_import_seen(sci_imports_t *im, const char *real) {
    for (int i = 0; i < im->n; i++) if (!strcmp(im->paths[i], real)) return 1;
    if (im->n == im->cap) {
        int nc = im->cap ? im->cap * 2 : 8;
        const char **np = ct_alloc(sizeof *np * (size_t)nc);
        if (im->n) memcpy(np, im->paths, sizeof *np * (size_t)im->n);
        im->paths = np;
        im->cap = nc;
    }
    im->paths[im->n++] = ct_strdup(real);
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *sci_read_file(const char *path, size_t *len) {
    FILE *f = fopen(path, "r");
    long n;
    char *s;
    if (!f) return (char *)0;
    if (fseek(f, 0, SEEK_END) != 0) { fclose(f); return (char *)0; }
    n = ftell(f);
    rewind(f);
    if (n < 0) { fclose(f); return (char *)0; }
    s = ct_alloc((size_t)n + 2);
    if (fread(s, 1, (size_t)n, f) != (size_t)n) { fclose(f); ct_drop(s); return (char *)0; }
    fclose(f);
    s[n] = 0;
    s[n + 1] = 0;
    *len = (size_t)n;
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *sci_dirname(const char *path) {
    const char *sl = strrchr(path, '/');
    size_t n = sl ? (size_t)(sl - path) : 1;
    char *d = ct_alloc(n + 1);
    memcpy(d, sl ? path : ".", n);
    d[n] = 0;
    return d;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *sci_try_path(const char *dir, const char *sub, const char *name) {
    size_t n = strlen(dir) + strlen(sub) + strlen(name) + 5;
    char *out = ct_alloc(n);
    snprintf(out, n, "%s%s/%s.sc", dir, sub, name);
    if (access(out, R_OK) == 0) return out;
    ct_drop(out);
    return (char *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *sci_exe_dir(void) {
    for (size_t cap = 256; cap <= 1u << 20; cap *= 2) {
        char buf[cap];
        ssize_t n = readlink("/proc/self/exe", buf, cap);
        if (n < 0) return (char *)0;
        if ((size_t)n < cap) { buf[n] = 0; return sci_dirname(buf); }
    }
    return (char *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *sci_import_locate(const char *name, const char *base) {
    char *hit = base && *base ? sci_try_path(base, "", name) : (char *)0;
    char *exe = hit ? (char *)0 : sci_exe_dir();
    if (!hit && exe) hit = sci_try_path(exe, "/bootstrap", name);
    if (!hit && exe) hit = sci_try_path(exe, "/../bootstrap", name);
    if (!hit && exe) hit = sci_try_path(exe, "/../corpus/library", name);
    if (!hit && exe) hit = sci_try_path(exe, "/../../corpus/library", name);
    return hit;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sci_splice(tree_t *out, tree_t *prog, sci_imports_t *im, const char *base);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sci_import(tree_t *out, tree_t *node, sci_imports_t *im, const char *base) {
    const char *nm = node->c[0]->v.sval;
    char *path = sci_import_locate(nm, base);
    size_t len = 0;
    char *src;
    tree_t *sub = (tree_t *)0;
    if (!path) { fprintf(stderr, "snocone:%d: error: cannot locate import '%s' (looked beside the importer, then bootstrap/ and corpus/library/ beside scrip)\n", node->line, nm); exit(1); }
    if (sci_import_seen(im, path)) return;
    src = sci_read_file(path, &len);
    if (!src) { fprintf(stderr, "snocone:%d: error: cannot read import '%s'\n", node->line, path); exit(1); }
    snocone_compile(src, path, &sub);
    if (!sub) { fprintf(stderr, "snocone:%d: error: import '%s' did not parse\n", node->line, path); exit(1); }
    sci_splice(out, sub, im, sci_dirname(path));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sci_splice(tree_t *out, tree_t *prog, sci_imports_t *im, const char *base) {
    for (int i = 0; i < prog->n; i++) {
        tree_t *node = prog->c[i];
        if (node && node->t == TT_IMPORT && node->n == 1 && node->c[0] && node->c[0]->v.sval) sci_import(out, node, im, base);
        else ast_push(out, node);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *snocone_resolve_imports(tree_t *prog, const char *path) {
    sci_imports_t im = { (const char **)0, 0, 0 };
    tree_t *out;
    int any = 0;
    if (!prog) return prog;
    for (int i = 0; i < prog->n; i++) if (prog->c[i] && prog->c[i]->t == TT_IMPORT) any = 1;
    if (!any) return prog;
    out = ast_node_new(TT_PROGRAM);
    out->line = prog->line;
    sci_splice(out, prog, &im, path && *path ? sci_dirname(path) : ".");
    return out;
}
