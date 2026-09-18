#include <stdlib.h>
#include "ct_arena.h"
#include <string.h>
#include <stdio.h>
#include "ast.h"
#include "parsers/snobol4/scrip_cc.h"
static char * stmt_src_slice(const STMT_t * s);
static int stmt_line_is_included(int lineno);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *ast_stmt_new(tree_e kind)
{
    tree_t *n = ct_zalloc(1, sizeof *n);
    n->t = kind;
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void ast_tree_free_dyn(tree_t *p) { ast_tree_free(p); }
static tree_t *sa_new(tree_e kind) { return ast_stmt_new(kind); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sa_add(tree_t *parent, tree_t *child)
{
    if (!child) return;
    ast_push(parent, child);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *ast_attr_leaf(const char *tag, const char *val)
{
    tree_t *n = sa_new(TT_ATTR);
    n->v.sval = ct_strdup(tag);
    if (val && val[0]) {
        tree_t *leaf = sa_new(TT_QLIT);
        leaf->v.sval = ct_strdup(val);
        sa_add(n, leaf);
    }
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *attr_leaf(const char *tag, const char *val) { return ast_attr_leaf(tag, val); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *ast_attr_int(const char *tag, int ival)
{
    char buf[32];
    snprintf(buf, sizeof buf, "%d", ival);
    return ast_attr_leaf(tag, buf);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *attr_int(const char *tag, int ival) { return ast_attr_int(tag, ival); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *ast_attr_expr(const char *tag, tree_t *expr)
{
    tree_t *n = sa_new(TT_ATTR);
    n->v.sval = ct_strdup(tag);
    sa_add(n, expr);
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *attr_expr(const char *tag, tree_t *e) { return ast_attr_expr(tag, e); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *make_goto_node(tree_e kind, const char *label, tree_t *expr)
{
    if (!expr && !(label && label[0])) return NULL;
    tree_t *node = sa_new(kind);
    if (expr) {
        sa_add(node, expr);
    } else {
        tree_t *lit = sa_new(TT_QLIT);
        lit->v.sval = ct_strdup(label);
        sa_add(node, lit);
    }
    return node;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *stmt_to_ast(const STMT_t *s)
{
    if (s->is_end) {
        tree_t *node = sa_new(TT_END);
        if (s->label && s->label[0])
            sa_add(node, attr_leaf(":lbl",  s->label));
        if (s->subject && s->subject->t == TT_VAR && s->subject->v.sval)
            sa_add(node, attr_leaf(":entry", s->subject->v.sval));
        sa_add(node, attr_int(":line", s->lineno));
        sa_add(node, attr_int(":lline", s->lline ? s->lline : s->lineno));
        if (s->file && s->file[0]) sa_add(node, attr_leaf(":file", s->file));
        sa_add(node, attr_int(":stno", s->stno));
        return node;
    }
    tree_t *node = sa_new(TT_STMT);
    if (s->label && s->label[0])
        sa_add(node, attr_leaf(":lbl",  s->label));
    sa_add(node, attr_int(":line", s->lineno));
    sa_add(node, attr_int(":lline", s->lline ? s->lline : s->lineno));
    if (s->file && s->file[0]) sa_add(node, attr_leaf(":file", s->file));
    sa_add(node, attr_int(":stno", s->stno));
    { char * ssrc = stmt_src_slice(s);
      if (!s->subject && !s->pattern && !s->replacement && !s->label && !s->goto_s && !s->goto_f && !s->goto_u && !s->goto_s_expr && !s->goto_f_expr && !s->goto_u_expr && !s->is_end) { ct_drop(ssrc); ssrc = ct_strdup(""); }
      if (!ssrc && !s->is_end && stmt_line_is_included(s->lineno)) { char b[160];
        snprintf(b, sizeof b, "%s%s<stmt %d, line %d: source not in main file (INCLUDE)>", s->label ? s->label : "", s->label && s->label[0] ? "  " : "        ", s->stno, s->lineno);
        ssrc = ct_strdup(b); sa_add(node, attr_int(":incl", 1)); }
      if (!ssrc && !s->is_end) { char b[160];
        snprintf(b, sizeof b, "%s%s<stmt %d, line %d: source not resolvable>", s->label ? s->label : "", s->label && s->label[0] ? "  " : "        ", s->stno, s->lineno);
        ssrc = ct_strdup(b); }
      if (ssrc) { sa_add(node, attr_leaf(":src", ssrc)); ct_drop(ssrc); } }
    if (s->subject)
        sa_add(node, attr_expr(":subj", s->subject));
    if (s->pattern)
        sa_add(node, attr_expr(":pat",  s->pattern));
    if (s->has_eq) {
        sa_add(node, attr_leaf(":eq", ""));
        if (s->replacement)
            sa_add(node, attr_expr(":repl", s->replacement));
        else {
            tree_t *empty = sa_new(TT_QLIT);
            empty->v.sval = ct_strdup("");
            sa_add(node, attr_expr(":repl", empty));
        }
    }
    sa_add(node, make_goto_node(TT_GOTO_S, s->goto_s, s->goto_s_expr));
    sa_add(node, make_goto_node(TT_GOTO_F, s->goto_f, s->goto_f_expr));
    sa_add(node, make_goto_node(TT_GOTO_U, s->goto_u, s->goto_u_expr));
    if (s->nofail) sa_add(node, attr_int(":nofail", 1));
    return node;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *code_to_ast(const CODE_t *prog)
{
    tree_t *root = sa_new(TT_PROGRAM);
    for (const STMT_t *s = prog->head; s; s = s->next)
        sa_add(root, stmt_to_ast(s));
    return root;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *stmt_attr_find(const tree_t *stmt, const char *tag)
{
    for (int i = 0; i < stmt->n; i++) {
        tree_t *ch = stmt->c[i];
        if (ch && ch->t == TT_ATTR && ch->v.sval && strcmp(ch->v.sval, tag) == 0)
            return ch;
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *stmt_attr_expr(const tree_t *attr)
{
    if (!attr || attr->n == 0) return NULL;
    return attr->c[0];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *stmt_attr_str(const tree_t *attr)
{
    if (!attr) return NULL;
    if (attr->n > 0 && attr->c[0])
        return attr->c[0]->v.sval;
    return NULL;
}
static char ** g_src_lines = NULL;
static int     g_src_nlines = 0;
static char *  g_src_path = NULL;
static int *   g_incl_ranges = NULL;
static int     g_n_incl_ranges = 0;
static int     g_cap_incl_ranges = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * stmt_src_get_file(void) { return g_src_path; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int stmt_src_nlines(void) { return g_src_nlines; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void stmt_src_set_file(const char * path)
{
    for (int i = 0; i < g_src_nlines; i++) ct_drop(g_src_lines[i]);
    ct_drop(g_src_lines); g_src_lines = NULL; g_src_nlines = 0;
    ct_drop(g_src_path); g_src_path = path ? ct_strdup(path) : NULL;
    g_n_incl_ranges = 0;
    FILE * f = path ? fopen(path, "r") : NULL;
    if (!f) return;
    fseek(f, 0, SEEK_END); long flen = ftell(f); rewind(f);
    if (flen <= 0) { fclose(f); return; }
    char * buf = ct_alloc((size_t) flen + 1);
    if (!buf) { fclose(f); return; }
    size_t got = fread(buf, 1, (size_t) flen, f); buf[got] = 0; fclose(f);
    int cap = 256;
    g_src_lines = ct_alloc((size_t) cap * sizeof(char *));
    if (!g_src_lines) { ct_drop(buf); return; }
    char * p = buf;
    while (*p) {
        char * e = p; while (*e && *e != '\n') e++;
        int len = (int)(e - p);
        while (len > 0 && (p[len - 1] == ' ' || p[len - 1] == '\t' || p[len - 1] == '\r')) len--;
        if (g_src_nlines >= cap) { cap *= 2; char ** g = ct_grow(g_src_lines, (size_t) cap * sizeof(char *)); if (!g) break; g_src_lines = g; }
        char * ln = ct_alloc((size_t) len + 1);
        if (!ln) break;
        memcpy(ln, p, (size_t) len); ln[len] = 0;
        g_src_lines[g_src_nlines++] = ln;
        p = *e ? e + 1 : e;
    }
    ct_drop(buf);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void stmt_src_mark_include_range(int start_line, int end_line)
{
    if (end_line <= start_line) return;
    if (g_n_incl_ranges + 2 > g_cap_incl_ranges) {
        int newcap = g_cap_incl_ranges ? g_cap_incl_ranges * 2 : 16;
        int * g = ct_grow(g_incl_ranges, (size_t) newcap * sizeof(int));
        if (!g) return;
        g_incl_ranges = g; g_cap_incl_ranges = newcap;
    }
    g_incl_ranges[g_n_incl_ranges++] = start_line;
    g_incl_ranges[g_n_incl_ranges++] = end_line;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern int snobol4_incl_depth(void);
static int stmt_line_is_included(int lineno)
{
    if (snobol4_incl_depth() > 0) return 1;
    for (int i = 0; i + 1 < g_n_incl_ranges; i += 2) if (lineno >= g_incl_ranges[i] && lineno < g_incl_ranges[i + 1]) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char * stmt_src_slice(const STMT_t * s)
{
    if (!g_src_nlines || !s || s->lineno < 1 || s->lineno > g_src_nlines || stmt_line_is_included(s->lineno)) return NULL;
    int n1 = s->lineno;
    int n2 = (s->next && s->next->lineno > n1) ? s->next->lineno : n1 + 1;
    if (n2 > g_src_nlines + 1) n2 = g_src_nlines + 1;
    while (n2 - 1 > n1) { const char * t = g_src_lines[n2 - 2]; if (t[0] == '*' || t[0] == '-' || t[0] == 0) n2--; else break; }
    const char * first = g_src_lines[n1 - 1];
    if (first[0] == '*' || first[0] == '-' || first[0] == 0) return NULL;
    if (s->label && s->label[0]) { if (strncmp(first, s->label, strlen(s->label)) != 0) return NULL; }
    else if (first[0] != ' ' && first[0] != '\t') return NULL;
    size_t tot = 0;
    for (int i = n1; i < n2; i++) tot += strlen(g_src_lines[i - 1]) + 1;
    char * out = ct_alloc(tot + 1);
    if (!out) return NULL;
    size_t o = 0;
    for (int i = n1; i < n2; i++) { size_t l = strlen(g_src_lines[i - 1]); memcpy(out + o, g_src_lines[i - 1], l); o += l; out[o++] = '\n'; }
    if (o) o--;
    out[o] = 0;
    return out;
}
