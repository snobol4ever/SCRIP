#include "ct_arena.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "lower.h"
#include "parsers/snocone/snocone_driver.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_e lsc_prim_kind(const char *s) {
    if (!s) return TT_VAR;
    static const struct {
        const char *n;
        tree_e k;
    } m[] = { {"ANY",TT_ANY},{"NOTANY",TT_NOTANY},{"SPAN",TT_SPAN},{"BREAK",TT_BREAK},{"BREAKX",TT_BREAKX}, {"LEN",TT_LEN},{"POS",TT_POS},{"RPOS",TT_RPOS},{"TAB",TT_TAB},{"RTAB",TT_RTAB}, {"ARB",
        TT_ARB},{"ARBNO",TT_ARBNO},{"REM",TT_REM},{"FAIL",TT_FAIL},{"SUCCEED",TT_SUCCEED}, {"FENCE",TT_FENCE},{"FLUSH",TT_FLUSH},{"ABORT",TT_ABORT},{"BAL",TT_BAL},{NULL,TT_VAR} };
    for (int i = 0; m[i].n; i++) if (strcmp(s, m[i].n) == 0) return m[i].k;
    return TT_VAR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *lsc_qlit(const char *txt) { tree_t *q = ast_node_new(TT_QLIT); q->v.sval = ct_strdup(txt); return q; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *lsc_join_vars(const tree_t *l) {
    size_t len = 1;
    for (int i = 0; i < l->n; i++) len += strlen(l->c[i]->v.sval) + 1;
    char *out = ct_alloc(len);
    out[0] = 0;
    for (int i = 0; i < l->n; i++) { if (i) strcat(out, ","); strcat(out, l->c[i]->v.sval); }
    return out;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *lsc_expr(tree_t *e) {
    if (!e) return e;
    for (int i = 0; i < e->n; i++) e->c[i] = lsc_expr(e->c[i]);
    if (e->t == TT_FNC && e->n == 2 && e->c[0]->t == TT_QLIT && e->c[1]->t == TT_ARGS) {
        char *nm = e->c[0]->v.sval;
        tree_e k = lsc_prim_kind(nm);
        tree_t *f = ast_node_new(k == TT_VAR ? TT_FNC : k);
        if (k == TT_VAR || k == TT_ARB || k == TT_BAL || k == TT_REM || k == TT_FAIL || k == TT_SUCCEED || k == TT_ABORT) f->v.sval = nm;
        for (int i = 0; i < e->c[1]->n; i++) ast_push(f, e->c[1]->c[i]);
        f->line = e->line;
        return f;
    }
    if (e->t == TT_IDX && e->n == 2 && e->c[1]->t == TT_VLIST) {
        tree_t *f = ast_node_new(TT_IDX);
        ast_push(f, e->c[0]);
        for (int i = 0; i < e->c[1]->n; i++) ast_push(f, e->c[1]->c[i]);
        f->line = e->line;
        return f;
    }
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static STMT_t *lsc_row(CODE_t *code, tree_t *node);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void lsc_body(CODE_t *code, tree_t *prog) {
    STMT_t *head = NULL, *tail = NULL;
    for (int i = 0; i < prog->n; i++) { STMT_t *s = lsc_row(code, prog->c[i]); if (!head) head = s; else tail->next = s; tail = s; }
    int i = 0;
    for (STMT_t *s = head, *nxt; s; s = nxt) { nxt = s->next; prog->c[i++] = stmt_to_ast(s); ct_drop(s); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static STMT_t *lsc_row(CODE_t *code, tree_t *node) {
    STMT_t *s = stmt_new();
    s->lineno = node->line;
    if (node->t == TT_LABEL) { s->label = ct_strdup(node->c[0]->v.sval); s->stno = ++code->nstmts; return s; }
    if (node->t == TT_STRUCT) {
        char *fl = lsc_join_vars(node->c[1]);
        size_t len = strlen(node->c[0]->v.sval) + strlen(fl) + 3;
        char *spec = ct_alloc(len);
        snprintf(spec, len, "%s(%s)", node->c[0]->v.sval, fl);
        tree_t *call = ast_node_new(TT_FNC);
        call->v.sval = ct_strdup("DATA");
        ast_push(call, lsc_qlit(spec));
        call->line = node->line;
        node = call;
    } else if (node->t == TT_DEFINE) {
        char *al = lsc_join_vars(node->c[1]);
        char *ll = lsc_join_vars(node->c[2]);
        size_t len = strlen(node->c[0]->v.sval) + strlen(al) + strlen(ll) + 3;
        char *sig = ct_alloc(len);
        snprintf(sig, len, "%s(%s)%s", node->c[0]->v.sval, al, ll);
        lsc_body(code, node->c[3]);
        tree_t *def = ast_node_new(TT_DEFINE);
        ast_push(def, node->c[0]);
        ast_push(def, lsc_qlit(sig));
        ast_push(def, node->c[3]);
        def->line = node->line;
        node = def;
    } else if (node->t == TT_GOTO_U && node->n == 1) {
        node->v.sval = node->c[0]->v.sval;
        node->n = 0;
    } else if (node->t == TT_IF || node->t == TT_WHILE || node->t == TT_DO_WHILE || node->t == TT_FOR || node->t == TT_CASE) {
        for (int i = 0; i < node->n; i++) { if (node->c[i]->t == TT_PROGRAM) lsc_body(code, node->c[i]); else node->c[i] = lsc_expr(node->c[i]); }
    } else {
        node = lsc_expr(node);
    }
    s->subject = node;
    if (node->t == TT_ASSIGN && node->n == 2 && node->c[0] && node->c[1]) {
        tree_t *lhs = node->c[0];
        if (lhs->t == TT_INDIRECT && lhs->n == 1 && lhs->c[0] && lhs->c[0]->t == TT_QLIT && lhs->c[0]->v.sval && lhs->c[0]->v.sval[0] && lhs->c[0]->v.sval[0] != '&') {
            tree_t *v = ast_node_new(TT_VAR);
            v->v.sval = ct_strdup(lhs->c[0]->v.sval);
            lhs = v;
        }
        if (lhs->t == TT_VAR && lhs->v.sval) { s->subject = lhs; s->replacement = node->c[1]; s->has_eq = 1; }
    }
    s->stno = ++code->nstmts;
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { const char **paths; int n, cap; } lsc_imports_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int lsc_import_seen(lsc_imports_t *im, const char *real) {
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
static char *lsc_read_file(const char *path, size_t *len) {
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
static char *lsc_dirname(const char *path) {
    const char *sl = strrchr(path, '/');
    size_t n = sl ? (size_t)(sl - path) : 1;
    char *d = ct_alloc(n + 1);
    memcpy(d, sl ? path : ".", n);
    d[n] = 0;
    return d;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *lsc_try_path(const char *dir, const char *sub, const char *name) {
    size_t n = strlen(dir) + strlen(sub) + strlen(name) + 5;
    char *out = ct_alloc(n);
    snprintf(out, n, "%s%s/%s.sc", dir, sub, name);
    if (access(out, R_OK) == 0) return out;
    ct_drop(out);
    return (char *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *lsc_exe_dir(void) {
    for (size_t cap = 256; cap <= 1u << 20; cap *= 2) {
        char buf[cap];
        ssize_t n = readlink("/proc/self/exe", buf, cap);
        if (n < 0) return (char *)0;
        if ((size_t)n < cap) { buf[n] = 0; return lsc_dirname(buf); }
    }
    return (char *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *lsc_import_locate(const char *name, const char *base) {
    char *hit = base && *base ? lsc_try_path(base, "", name) : (char *)0;
    char *exe = hit ? (char *)0 : lsc_exe_dir();
    if (!hit && exe) hit = lsc_try_path(exe, "/bootstrap", name);
    if (!hit && exe) hit = lsc_try_path(exe, "/../bootstrap", name);
    if (!hit && exe) hit = lsc_try_path(exe, "/../corpus/library", name);
    if (!hit && exe) hit = lsc_try_path(exe, "/../../corpus/library", name);
    return hit;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void lsc_splice(tree_t *out, tree_t *prog, lsc_imports_t *im, const char *base);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void lsc_import(tree_t *out, tree_t *node, lsc_imports_t *im, const char *base) {
    const char *nm = node->c[0]->v.sval;
    char *path = lsc_import_locate(nm, base);
    size_t len = 0;
    char *src;
    tree_t *sub = (tree_t *)0;
    if (!path) { fprintf(stderr, "snocone:%d: error: cannot locate import '%s' (looked beside the importer, then bootstrap/ and corpus/library/ beside scrip)\n", node->line, nm); exit(1); }
    if (lsc_import_seen(im, path)) return;
    src = lsc_read_file(path, &len);
    if (!src) { fprintf(stderr, "snocone:%d: error: cannot read import '%s'\n", node->line, path); exit(1); }
    snocone_compile(src, path, &sub);
    if (!sub) { fprintf(stderr, "snocone:%d: error: import '%s' did not parse\n", node->line, path); exit(1); }
    lsc_splice(out, sub, im, lsc_dirname(path));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void lsc_splice(tree_t *out, tree_t *prog, lsc_imports_t *im, const char *base) {
    for (int i = 0; i < prog->n; i++) {
        tree_t *node = prog->c[i];
        if (node && node->t == TT_IMPORT && node->n == 1 && node->c[0] && node->c[0]->v.sval) lsc_import(out, node, im, base);
        else ast_push(out, node);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *snocone_resolve_imports(tree_t *prog, const char *path) {
    lsc_imports_t im = { (const char **)0, 0, 0 };
    tree_t *out;
    int any = 0;
    if (!prog) return prog;
    for (int i = 0; i < prog->n; i++) if (prog->c[i] && prog->c[i]->t == TT_IMPORT) any = 1;
    if (!any) return prog;
    out = ast_node_new(TT_PROGRAM);
    out->line = prog->line;
    lsc_splice(out, prog, &im, path && *path ? lsc_dirname(path) : ".");
    return out;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static CODE_t *lsc_desugar(tree_t *prog) {
    CODE_t *code = ct_zalloc(1, sizeof *code);
    for (int i = 0; i < prog->n; i++) { STMT_t *s = lsc_row(code, prog->c[i]); if (!code->head) code->head = s; else code->tail->next = s; code->tail = s; }
    return code;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *lower_snocone_tree_at(tree_t *pruned, const char *path) { if (!pruned) return NULL; return code_to_ast(lsc_desugar(snocone_resolve_imports(pruned, path))); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *lower_snocone_tree(tree_t *pruned) { return lower_snocone_tree_at(pruned, (const char *)0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
