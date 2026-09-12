#include "icon_driver.h"
#include "icon_lex.h"
#include "icon_parse.h"
#include "../snobol4/scrip_cc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char * icn_read_file(const char * path) {
    FILE * f = fopen(path, "rb");
    if (!f) return NULL;
    fseek(f, 0, SEEK_END); long sz = ftell(f); fseek(f, 0, SEEK_SET);
    char * buf = (char *) malloc((size_t) sz + 1);
    if (!buf) { fclose(f); return NULL; }
    size_t got = fread(buf, 1, (size_t) sz, f); fclose(f); buf[got] = '\0';
    return buf;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * icn_stmt_subject(const tree_t * s) {
    if (!s) return NULL;
    if (s->t != TT_STMT) return s;
    for (int i = 0; i < s->n; i++) { const tree_t * a = s->c[i]; if (a && a->t == TT_ATTR && a->v.sval && !strcmp(a->v.sval, ":subj")) return (a->n > 0) ? a->c[0] : NULL; }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void icn_link_note(char * tried, size_t triedsz, const char * cand) {
    if (!tried || !triedsz) return;
    size_t have = strlen(tried); size_t need = strlen(cand) + 2;
    if (have + need >= triedsz) return;
    if (have) { tried[have++] = ','; tried[have++] = ' '; }
    memcpy(tried + have, cand, strlen(cand)); tried[have + strlen(cand)] = '\0';
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char * icn_link_open(const char * dir, const char * nm, char * out, size_t outsz, char * tried, size_t triedsz) {
    if (triedsz) tried[0] = '\0';
    char * src = NULL;
    const char * vars[2]; vars[0] = getenv("IPATH"); vars[1] = getenv("ICONPATH");
    for (int v = 0; v < 2; v++) {
        const char * p = vars[v];
        while (p && *p) {
            const char * colon = strchr(p, ':'); size_t seg = colon ? (size_t)(colon - p) : strlen(p);
            if (seg > 0 && seg < 1000) {
                char d[1024]; memcpy(d, p, seg); d[seg] = '\0';
                snprintf(out, outsz, "%s/%s.icn", d, nm);
                icn_link_note(tried, triedsz, out);
                src = icn_read_file(out);
                if (src) return src;
            }
            if (!colon) break;
            p = colon + 1;
        }
    }
    { char exe[1024]; ssize_t el = readlink("/proc/self/exe", exe, sizeof exe - 1);
      if (el > 0) { exe[el] = '\0'; char * sl = strrchr(exe, '/'); if (sl) { *sl = '\0';
          snprintf(out, outsz, "%s/../corpus/packages/icon/ipl/procs/%s.icn", exe, nm);
          icn_link_note(tried, triedsz, out);
          src = icn_read_file(out);
          if (src) return src; } } }
    snprintf(out, outsz, "%s/%s.icn", dir, nm);
    icn_link_note(tried, triedsz, out);
    src = icn_read_file(out);
    if (src) return src;
    snprintf(out, outsz, "%s/%s.icn", dir, nm);
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void icn_resolve_links(tree_t * prog, const char * filename) {
    if (!prog) return;
    char dir[1024]; const char * slash = strrchr(filename, '/');
    if (slash) { size_t dl = (size_t)(slash - filename); if (dl >= sizeof dir) dl = sizeof dir - 1; memcpy(dir, filename, dl); dir[dl] = '\0'; } else { dir[0] = '.'; dir[1] = '\0'; }
    const char * loaded[64]; int nloaded = 0;
    { const char * base = slash ? slash + 1 : filename; const char * dot = strrchr(base, '.'); size_t bl = dot ? (size_t)(dot - base) : strlen(base);
      char * own = (char *) malloc(bl + 1); memcpy(own, base, bl); own[bl] = '\0'; loaded[nloaded++] = own; }
    for (int scan = 0; scan < prog->n; scan++) {
        const tree_t * subj = icn_stmt_subject(prog->c[scan]);
        if (!subj || subj->t != TT_LINK) continue;
        for (int k = 0; k < subj->n; k++) {
            const char * nm = (subj->c[k] && subj->c[k]->v.sval) ? subj->c[k]->v.sval : NULL;
            if (!nm) continue;
            int dup = 0; for (int d = 0; d < nloaded; d++) if (!strcmp(loaded[d], nm)) { dup = 1; break; }
            if (dup) continue;
            if (nloaded >= 64) { fprintf(stderr, "icon: link: more than 64 linked files (at %s)\n", nm); exit(1); }
            loaded[nloaded++] = nm;
            char path[1200]; char tried[4096]; char * src = icn_link_open(dir, nm, path, sizeof path, tried, sizeof tried);
            if (!src) { fprintf(stderr, "icon: link: cannot open %s.icn (linked from %s); tried: %s\n", nm, filename, tried); exit(1); }
            IcnLexer lx2; icn_pp_set_source_path(path); icn_lex_init(&lx2, src);
            IcnParser p2; icn_parse_init(&p2, &lx2);
            tree_t * sub_ast = NULL;
            CODE_t * sp = icn_parse_file(&p2, &sub_ast);
            if (p2.had_error) { fprintf(stderr, "icon: parse error in linked file %s: %s\n", path, p2.errmsg); exit(1); }
            free(sp);
            if (sub_ast) for (int j = 0; j < sub_ast->n; j++) if (sub_ast->c[j]) ast_push(prog, sub_ast->c[j]);
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * icn_top_subject(const tree_t * s) {
    while (s && s->t == TT_STMT) { const tree_t * subj = NULL; for (int i = 0; i < s->n; i++) { const tree_t * a = s->c[i]; if (a && a->t == TT_ATTR && a->v.sval && !strcmp(a->v.sval, ":subj")) { subj = (a->n > 0) ? a->c[0] : NULL; break; } } s = subj; }
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void icn_mark_proc_refs(const tree_t * n, const tree_t ** procs, int np, char * keep) {
    if (!n) return;
    if (n->t == TT_VAR && n->v.sval) for (int k = 0; k < np; k++) if (!keep[k] && procs[k]->v.sval && !strcmp(procs[k]->v.sval, n->v.sval)) keep[k] = 1;
    for (int i = 0; i < n->n; i++) icn_mark_proc_refs(n->c[i], procs, np, keep);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void icn_prune_unreachable_procs(tree_t * prog) {
    if (!prog) return;
    int np = 0; for (int i = 0; i < prog->n; i++) { const tree_t * s = icn_top_subject(prog->c[i]); if (s && s->t == TT_PROC_DECL) np++; }
    if (np == 0) return;
    const tree_t ** procs = (const tree_t **) calloc((size_t) np, sizeof *procs); char * keep = (char *) calloc((size_t) np, 1); char * walked = (char *) calloc((size_t) np, 1); int k = 0;
    for (int i = 0; i < prog->n; i++) { const tree_t * s = icn_top_subject(prog->c[i]); if (s && s->t == TT_PROC_DECL) procs[k++] = s; }
    for (int i = 0; i < prog->n; i++) { const tree_t * s = icn_top_subject(prog->c[i]); if (!s || s->t != TT_INVOCABLE) continue;
        for (int j = 0; j < s->n; j++) { const char * nm = s->c[j] ? s->c[j]->v.sval : NULL; if (!nm) continue;
            if (!strcmp(nm, "all")) { free(procs); free(keep); free(walked); return; }
            for (int q = 0; q < np; q++) if (procs[q]->v.sval && !strcmp(procs[q]->v.sval, nm)) keep[q] = 1; } }
    for (int q = 0; q < np; q++) if (procs[q]->v.sval && !strcmp(procs[q]->v.sval, "main")) keep[q] = 1;
    for (int i = 0; i < prog->n; i++) { const tree_t * s = icn_top_subject(prog->c[i]); if (s && s->t != TT_PROC_DECL) icn_mark_proc_refs(s, procs, np, keep); }
    int again = 1; while (again) { again = 0; for (int q = 0; q < np; q++) if (keep[q] && !walked[q]) { walked[q] = 1; icn_mark_proc_refs(procs[q], procs, np, keep); again = 1; } }
    int w = 0; for (int i = 0; i < prog->n; i++) { const tree_t * s = icn_top_subject(prog->c[i]); int drop = 0;
        if (s && s->t == TT_PROC_DECL) for (int q = 0; q < np; q++) if (procs[q] == s) { drop = !keep[q]; break; }
        if (!drop) prog->c[w++] = prog->c[i]; }
    prog->n = w; free(procs); free(keep); free(walked);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void icon_compile(const char *source, const char *filename, tree_t **out_ast) {
    if (!filename) filename = "<stdin>";
    if (out_ast) *out_ast = NULL;
    IcnLexer lx;
    icn_pp_set_source_path(filename);
    icn_lex_init(&lx, source);
    IcnParser parser;
    icn_parse_init(&parser, &lx);
    CODE_t *prog = icn_parse_file(&parser, out_ast);
    if (parser.had_error) {
        fprintf(stderr, "icon: parse error in %s: %s\n", filename, parser.errmsg);
        free(prog);
        if (out_ast) *out_ast = NULL;
        exit(1);
    }
    (void)prog;
    if (out_ast && *out_ast) icn_resolve_links(*out_ast, filename);
    if (out_ast && *out_ast) icn_prune_unreachable_procs(*out_ast);
}
