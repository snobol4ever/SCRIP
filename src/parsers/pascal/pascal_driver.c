#include "pascal_driver.h"
#include "ast.h"
#include "ct_arena.h"
#include "../snobol4/scrip_cc.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
extern tree_t *pascal_prog_result;
extern tree_t *pascal_parse_string(const char *src);
#include "pascal_prelude_units.inc"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *pascal_compile_parse(const char *src, const char *filename) {
    (void)filename;
    pascal_prog_result = NULL;
    return pascal_parse_string(src);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *pascal_dialect_directive(const char *name) {
    static const char *const D[][2] = { { "iso", "{$mode iso}" }, { "fpc", "{$mode fpc}" }, { "objfpc", "{$mode objfpc}" }, { "delphi", "{$mode delphi}" }, { "tp", "{$mode tp}" }, { "macpas", "{$mode macpas}" }, { "delphiunicode", "{$mode delphiunicode}" } };
    for (size_t i = 0; name && i < sizeof D / sizeof D[0]; i++) if (!strcmp(name, D[i][0])) return D[i][1];
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pascal_compile(const char *src, const char *filename, tree_t **out_ast) {
    if (out_ast) *out_ast = NULL;
    pascal_compile_finish(pascal_compile_parse(src, filename), filename, out_ast);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pas_is_part(const tree_t *t, const char *w) { return t && t->t == TT_PART && t->v.sval && !strcmp(t->v.sval, w); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t *pas_uses_of(const tree_t *part) {
    for (int i = 0; part && i < part->n; i++) if (pas_is_part(part->c[i], "uses") && part->c[i]->n > 0) return part->c[i]->c[0];
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *pas_slurp(const char *path) {
    FILE *f = fopen(path, "rb");
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    long n = ftell(f);
    rewind(f);
    char *s = ct_alloc((size_t)n + 1);
    size_t got = fread(s, 1, (size_t)n, f);
    s[got] = '\0';
    fclose(f);
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *pas_unit_file(const char *dir, const char *name) {
    DIR *d = opendir(dir);
    if (!d) return NULL;
    size_t nl = strlen(name);
    char *found = NULL;
    for (struct dirent *e = readdir(d); e && !found; e = readdir(d)) {
        const char *fn = e->d_name;
        size_t fl = strlen(fn);
        if (fl > nl && !strncasecmp(fn, name, nl) && (!strcasecmp(fn + nl, ".pas") || !strcasecmp(fn + nl, ".pp"))) {
            size_t pl = strlen(dir) + fl + 2;
            found = ct_alloc(pl);
            snprintf(found, pl, "%s/%s", dir, fn);
        }
    }
    closedir(d);
    return found;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *pas_prelude_src(const char *name) {
    for (int i = 0; PAS_PRELUDE_UNITS[i].name; i++) if (!strcasecmp(PAS_PRELUDE_UNITS[i].name, name)) return PAS_PRELUDE_UNITS[i].src;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pas_named_in(const tree_t *list, const char *name) {
    for (int i = 0; list && i < list->n; i++) {
        const tree_t *u = list->c[i];
        const tree_t *nm = (u && u->t == TT_MODULE_DECL && u->n > 0) ? u->c[0] : u;
        if (nm && nm->v.sval && !strcasecmp(nm->v.sval, name)) return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pas_units_visit(const char *dir, const char *from, const tree_t *uses, tree_t *forest, tree_t *visiting) {
    for (int i = 0; uses && i < uses->n; i++) {
        const char *name = uses->c[i] ? uses->c[i]->v.sval : NULL;
        if (!name || pas_named_in(forest, name) || pas_named_in(visiting, name)) continue;
        char *path = pas_unit_file(dir, name);
        const char *src = path ? pas_slurp(path) : pas_prelude_src(name);
        if (!src) { fprintf(stderr, "pascal: uses %s in %s: no unit %s.pas beside it and no SCRIP prelude unit of that name\n", name, from, name); return 0; }
        tree_t *u = pascal_parse_string(src);
        if (!u || u->t != TT_MODULE_DECL || u->n < 3) { fprintf(stderr, "pascal: uses %s in %s: %s does not parse as a unit\n", name, from, path ? path : "the prelude unit"); return 0; }
        if (!u->c[0]->v.sval || strcasecmp(u->c[0]->v.sval, name)) { fprintf(stderr, "pascal: uses %s in %s: the file declares unit %s\n", name, from, u->c[0]->v.sval ? u->c[0]->v.sval : "?"); return 0; }
        ast_push(visiting, u->c[0]);
        if (!pas_units_visit(dir, path ? path : name, pas_uses_of(u->c[1]), forest, visiting)) return 0;
        if (!pas_units_visit(dir, path ? path : name, pas_uses_of(u->c[2]), forest, visiting)) return 0;
        ast_push(forest, u);
    }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pas_names_system(const tree_t *t) {
    static const char *const S[] = { "strlen", "bsrqword", "bsrdword", "bsfqword", "bsfdword", "popcnt", 0 };
    if (!t) return 0;
    if (t->t == TT_VAR && t->v.sval) for (int i = 0; S[i]; i++) if (!strcasecmp(t->v.sval, S[i])) return 1;
    for (int i = 0; i < t->n; i++) if (pas_names_system(t->c[i])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *pas_compose_units(tree_t *prog, const char *filename) {
    const tree_t *block = NULL;
    for (int i = 0; prog && i < prog->n; i++) if (prog->c[i] && prog->c[i]->t == TT_BLOCK) block = prog->c[i];
    const tree_t *uses = pas_uses_of(block);
    int sys = pas_names_system(block);
    if (!uses && !sys) return prog;
    const char *slash = strrchr(filename, '/');
    size_t dl = slash ? (size_t)(slash - filename) : 1;
    char *dir = ct_alloc(dl + 1);
    if (slash) memcpy(dir, filename, dl); else dir[0] = '.';
    dir[dl] = '\0';
    tree_t *forest = ast_node_new(TT_PART);
    forest->v.sval = ct_strdup("units");
    tree_t *visiting = ast_node_new(TT_VLIST);
    if (uses && !pas_units_visit(dir, filename, uses, forest, visiting)) return NULL;
    if (sys) {
        tree_t *su = ast_node_new(TT_VLIST), *sn = ast_node_new(TT_VAR);
        sn->v.sval = ct_strdup("system");
        ast_push(su, sn);
        if (!pas_units_visit(dir, filename, su, forest, visiting)) return NULL;
    }
    ast_push(prog, forest);
    return prog;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pascal_compile_finish(void *parsed, const char *filename, tree_t **out_ast) {
    if (!filename) filename = "<stdin>";
    if (out_ast) *out_ast = NULL;
    tree_t *prog = (tree_t *)parsed;
    if (!prog) {
        fprintf(stderr, "pascal: parse error in %s\n", filename);
        return;
    }
    if (prog->t == TT_MODULE_DECL) {
        fprintf(stderr, "pascal: %s is a unit, not a program: compile the program that uses it\n", filename);
        return;
    }
    prog = pas_compose_units(prog, filename);
    if (out_ast) *out_ast = prog;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
