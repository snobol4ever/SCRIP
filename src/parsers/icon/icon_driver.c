#include "icon_driver.h"
#include "ir/ct_arena.h"
#include "icon_lex.h"
#include "icon_parse.h"
#include "parsers/snobol4/scrip_cc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>
#include "ir/ast_tt_capt_immed_asgn.h"
#include "ir/ast.h"
#include "ir/ast_node_new.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char * icn_read_file(const char * path) {
    FILE * f = fopen(path, "rb");
    if (!f) return NULL;
    fseek(f, 0, SEEK_END); long sz = ftell(f); fseek(f, 0, SEEK_SET);
    char * buf = (char *) ct_alloc((size_t) sz + 1);
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
static char * icn_link_open(const char * dir, const char * nm, char * out, size_t outsz, char * tried, size_t triedsz, int unicon) {
    if (triedsz) tried[0] = '\0';
    char * src = NULL;
    const char * vars[2]; vars[0] = getenv("IPATH"); vars[1] = getenv("ICONPATH");
    for (int v = 0; v < 2; v++) {
        const char * p = vars[v];
        while (p && *p) {
            const char * colon = strchr(p, ':'); size_t seg = colon ? (size_t)(colon - p) : strlen(p);
            if (seg > 0) {
                char d[seg + 1]; memcpy(d, p, seg); d[seg] = '\0';
                snprintf(out, outsz, "%s/%s.icn", d, nm);
                icn_link_note(tried, triedsz, out);
                src = icn_read_file(out);
                if (src) return src;
            }
            if (!colon) break;
            p = colon + 1;
        }
    }
    { char exe[PATH_MAX];
      if (realpath("/proc/self/exe", exe)) { char * sl = strrchr(exe, '/'); if (sl) { *sl = '\0';
          for (int k = 0; k < 2; k++) {
              snprintf(out, outsz, "%s/../corpus/packages/icon/%s/%s.icn", exe, (k ^ (unicon != 0)) ? "unicon_ipl/procs" : "ipl/procs", nm);
              icn_link_note(tried, triedsz, out);
              src = icn_read_file(out);
              if (src) return src;
          } } } }
    snprintf(out, outsz, "%s/%s.icn", dir, nm);
    icn_link_note(tried, triedsz, out);
    src = icn_read_file(out);
    if (src) return src;
    snprintf(out, outsz, "%s/%s.icn", dir, nm);
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * const icn_posix_record_fns[] = { "stat", "lstat", "getpw", "getgr", "gettimeofday", "getrusage", "gethost", "getserv", "fcntl", "receive" };
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_calls_posix_record(const tree_t * t) {
    if (!t) return 0;
    if (t->t == TT_FNC && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR && t->c[0]->v.sval)
        for (size_t i = 0; i < sizeof icn_posix_record_fns / sizeof *icn_posix_record_fns; i++) if (!strcmp(t->c[0]->v.sval, icn_posix_record_fns[i])) return 1;
    for (int i = 0; i < t->n; i++) if (icn_calls_posix_record(t->c[i])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_has_link_to(const tree_t * prog, const char * nm) {
    for (int s = 0; s < prog->n; s++) {
        const tree_t * subj = icn_stmt_subject(prog->c[s]);
        if (subj && subj->t == TT_LINK) for (int k = 0; k < subj->n; k++) if (subj->c[k] && subj->c[k]->v.sval && !strcmp(subj->c[k]->v.sval, nm)) return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_link_posix_implicitly(tree_t * prog) {
    if (!prog->v.ival || !icn_calls_posix_record(prog) || icn_has_link_to(prog, "posix")) return 0;
    tree_t * top = ast_node_new(TT_LINK), * v = ast_node_new(TT_VAR), * st = ast_stmt_new(TT_STMT);
    v->v.sval = intern_n("posix", 5);
    v->slen = 5;
    ast_push(top, v);
    ast_push(st, ast_attr_int(":line", 0));
    ast_push(st, ast_attr_int(":stno", 0));
    ast_push(st, ast_attr_expr(":subj", top));
    ast_push(prog, st);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void icn_resolve_links(tree_t * prog, const char * filename) {
    if (!prog) return;
    const char * slash = strrchr(filename, '/'); char dir[slash ? (size_t)(slash - filename) + 1 : 2];
    if (slash) { size_t dl = (size_t)(slash - filename); memcpy(dir, filename, dl); dir[dl] = '\0'; } else { dir[0] = '.'; dir[1] = '\0'; }
    const char ** loaded = (const char **) ct_grow(NULL, 16 * sizeof *loaded); int nloaded = 0, nloadcap = 16;
    { const char * base = slash ? slash + 1 : filename; const char * dot = strrchr(base, '.'); size_t bl = dot ? (size_t)(dot - base) : strlen(base);
      char * own = (char *) ct_alloc(bl + 1); memcpy(own, base, bl); own[bl] = '\0'; loaded[nloaded++] = own; }
    for (int again = 1; again; ) {
    for (int scan = 0; scan < prog->n; scan++) {
        const tree_t * subj = icn_stmt_subject(prog->c[scan]);
        if (!subj || subj->t != TT_LINK) continue;
        for (int k = 0; k < subj->n; k++) {
            const char * nm = (subj->c[k] && subj->c[k]->v.sval) ? subj->c[k]->v.sval : NULL;
            if (!nm) continue;
            int dup = 0; for (int d = 0; d < nloaded; d++) if (!strcmp(loaded[d], nm)) { dup = 1; break; }
            if (dup) continue;
            if (nloaded == nloadcap) { nloadcap *= 2; loaded = (const char **) ct_grow((void *) loaded, (size_t) nloadcap * sizeof *loaded); }
            loaded[nloaded++] = nm;
            size_t pneed = strlen(dir); const char * pev[2] = { getenv("IPATH"), getenv("ICONPATH") }; for (int v = 0; v < 2; v++) if (pev[v] && strlen(pev[v]) > pneed) pneed = strlen(pev[v]);
            if (pneed < PATH_MAX + sizeof "/../corpus/packages/icon/ipl/procs") pneed = PATH_MAX + sizeof "/../corpus/packages/icon/ipl/procs";
            size_t per = strlen(nm) + sizeof ", /.icn", tneed = 1 + 2 * (pneed + per);
            for (int v = 0; v < 2; v++) if (pev[v]) { tneed += strlen(pev[v]) + per; for (const char * q = pev[v]; *q; q++) if (*q == ':') tneed += per; }
            char path[pneed + strlen(nm) + sizeof "/.icn"]; char tried[tneed];
            char * src = icn_link_open(dir, nm, path, sizeof path, tried, sizeof tried, prog->v.ival != 0);
            if (!src) { fprintf(stderr, "icon: link: cannot open %s.icn (linked from %s); tried: %s\n", nm, filename, tried); exit(1); }
            IcnLexer lx2; icn_pp_set_source_path(path); icn_lex_init(&lx2, src); lx2.unicon = (prog && prog->v.ival) || icn_source_is_unicon(src);
            IcnParser p2; icn_parse_init(&p2, &lx2);
            tree_t * sub_ast = NULL;
            CODE_t * sp = icn_parse_file(&p2, &sub_ast);
            if (p2.had_error) { fprintf(stderr, "icon: parse error in linked file %s: %s\n", path, p2.errmsg); exit(1); }
            if (lx2.pp_fatals) { fprintf(stderr, "icon: %d preprocessing error%s in linked file %s\n", lx2.pp_fatals, lx2.pp_fatals == 1 ? "" : "s", path); exit(1); }
            ct_drop(sp);
            { const char * lb = strrchr(path, '/'); lb = lb ? lb + 1 : path;
              if (sub_ast) for (int j = 0; j < sub_ast->n; j++) if (sub_ast->c[j]) { if (sub_ast->c[j]->t == TT_STMT) ast_push(sub_ast->c[j], ast_attr_leaf(":file", lb)); ast_push(prog, sub_ast->c[j]); } }
        }
    }
        again = icn_link_posix_implicitly(prog);
    }
    ct_drop((void *) loaded);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_pkg_file_is(const char * path, const char * pkg) {
    char * src = icn_read_file(path);
    if (!src) return 0;
    IcnLexer lx; icn_lex_init(&lx, src); lx.unicon = 1;
    IcnToken a = icn_lex_next(&lx);
    while (a.kind == TK_LINK || a.kind == TK_IMPORT || a.kind == TK_GLOBAL || a.kind == TK_INVOCABLE)
        do a = icn_lex_next(&lx); while (a.kind == TK_IDENT || a.kind == TK_STRING || a.kind == TK_COMMA || a.kind == TK_SEMICOL);
    IcnToken b = icn_lex_next(&lx);
    int ok = a.kind == TK_PACKAGE && (b.kind == TK_IDENT || b.kind == TK_STRING) && b.val.sval.data && !strcmp(b.val.sval.data, pkg);
    ct_drop(src);
    return ok;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char * icn_dir_join(const char * d, const char * name, const char * ext) { size_t n = strlen(d) + strlen(name) + strlen(ext) + 2; char * s = (char *) ct_alloc(n); snprintf(s, n, "%s/%s%s", d, name, ext); return s; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void icn_pkg_dir_files(const char * d, const char * pkg, char *** out, int * n, int * cap) {
    char * one = icn_dir_join(d, pkg, ".icn");
    int named = access(one, R_OK) == 0 && icn_pkg_file_is(one, pkg);
    if (named) { if (*n == *cap) { *cap = *cap ? *cap * 2 : 4; *out = (char **) ct_grow(*out, (size_t) *cap * sizeof **out); } (*out)[(*n)++] = one; } else ct_drop(one);
    DIR * dp = opendir(d);
    if (!dp) return;
    struct dirent * de;
    while ((de = readdir(dp))) {
        size_t l = strlen(de->d_name);
        if (l < 5 || strcmp(de->d_name + l - 4, ".icn")) continue;
        if (named && l == strlen(pkg) + 4 && !strncmp(de->d_name, pkg, l - 4)) continue;
        one = icn_dir_join(d, de->d_name, "");
        if (!icn_pkg_file_is(one, pkg)) { ct_drop(one); continue; }
        if (*n == *cap) { *cap = *cap ? *cap * 2 : 4; *out = (char **) ct_grow(*out, (size_t) *cap * sizeof **out); }
        (*out)[(*n)++] = one;
    }
    closedir(dp);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void icn_resolve_imports(tree_t * prog, const char * filename) {
    if (!prog) return;
    const char * slash = strrchr(filename, '/'); char dir[slash ? (size_t)(slash - filename) + 1 : 2];
    if (slash) { size_t dl = (size_t)(slash - filename); memcpy(dir, filename, dl); dir[dl] = '\0'; } else { dir[0] = '.'; dir[1] = '\0'; }
    const char ** seen = NULL; int nseen = 0, cseen = 0;
    for (int scan = 0; scan < prog->n; scan++) {
        const tree_t * subj = icn_stmt_subject(prog->c[scan]);
        if (!subj || subj->t != TT_USE_DECL) continue;
        for (int k = 0; k < subj->n; k++) {
            const char * nm = (subj->c[k] && subj->c[k]->v.sval) ? subj->c[k]->v.sval : NULL;
            if (!nm) continue;
            int dup = 0; for (int d = 0; d < nseen; d++) if (!strcmp(seen[d], nm)) { dup = 1; break; }
            if (dup) continue;
            if (nseen == cseen) { cseen = cseen ? cseen * 2 : 8; seen = (const char **) ct_grow((void *) seen, (size_t) cseen * sizeof *seen); }
            seen[nseen++] = nm;
            char ** files = NULL; int nf = 0, cf = 0;
            icn_pkg_dir_files(dir, nm, &files, &nf, &cf);
            const char * pev[2] = { getenv("IPATH"), getenv("ICONPATH") };
            for (int v = 0; v < 2 && !nf; v++) for (const char * q = pev[v]; q && *q && !nf;) {
                const char * colon = strchr(q, ':'); size_t seg = colon ? (size_t)(colon - q) : strlen(q);
                if (seg > 0) { char dd[seg + 1]; memcpy(dd, q, seg); dd[seg] = '\0'; icn_pkg_dir_files(dd, nm, &files, &nf, &cf); }
                if (!colon) break;
                q = colon + 1;
            }
            if (!nf) { fprintf(stderr, "icon: import: cannot find package %s (imported from %s); searched %s and IPATH\n", nm, filename, dir); exit(1); }
            for (int f = 0; f < nf; f++) {
                char * src = icn_read_file(files[f]);
                if (!src) { fprintf(stderr, "icon: import: cannot read %s\n", files[f]); exit(1); }
                IcnLexer lx2; icn_pp_set_source_path(files[f]); icn_lex_init(&lx2, src); lx2.unicon = (prog && prog->v.ival) || icn_source_is_unicon(src);
                IcnParser p2; icn_parse_init(&p2, &lx2);
                tree_t * sub_ast = NULL;
                CODE_t * sp = icn_parse_file(&p2, &sub_ast);
                if (p2.had_error) { fprintf(stderr, "icon: parse error in imported file %s: %s\n", files[f], p2.errmsg); exit(1); }
                ct_drop(sp);
                const char * pkgname = NULL;
                if (sub_ast) for (int j = 0; j < sub_ast->n && !pkgname; j++) { const tree_t * sj = icn_stmt_subject(sub_ast->c[j]); if (sj && sj->t == TT_MODULE_DECL) pkgname = sj->v.sval; }
                const char * lb = strrchr(files[f], '/'); lb = lb ? lb + 1 : files[f];
                if (sub_ast) for (int j = 0; j < sub_ast->n; j++) if (sub_ast->c[j]) {
                    if (sub_ast->c[j]->t == TT_STMT) { ast_push(sub_ast->c[j], ast_attr_leaf(":file", lb)); if (pkgname) ast_push(sub_ast->c[j], ast_attr_leaf(":pkg", pkgname)); }
                    ast_push(prog, sub_ast->c[j]);
                }
            }
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
    if (n->t == TT_VAR && n->v.sval && !strcmp(n->v.sval, "ICN$CRIT")) for (int k = 0; k < np; k++) if (procs[k]->v.sval && (!strcmp(procs[k]->v.sval, "lock") || !strcmp(procs[k]->v.sval, "unlock"))) keep[k] = 1;
    if (n->t == TT_VAR && n->v.sval) { const char * qn = strrchr(n->v.sval, ':'); qn = (qn && qn > n->v.sval && qn[-1] == ':') ? qn + 1 : n->v.sval; for (int k = 0; k < np; k++) if (!keep[k] && procs[k]->v.sval && !strcmp(procs[k]->v.sval, qn)) keep[k] = 1; }
    for (int i = 0; i < n->n; i++) icn_mark_proc_refs(n->c[i], procs, np, keep);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void icn_prune_unreachable_procs(tree_t * prog) {
    if (!prog) return;
    int np = 0; for (int i = 0; i < prog->n; i++) { const tree_t * s = icn_top_subject(prog->c[i]); if (s && s->t == TT_PROC_DECL) np++; }
    if (np == 0) return;
    const tree_t ** procs = (const tree_t **) ct_zalloc((size_t) np, sizeof *procs); char * keep = (char *) ct_zalloc((size_t) np, 1); char * walked = (char *) ct_zalloc((size_t) np, 1); int k = 0;
    for (int i = 0; i < prog->n; i++) { const tree_t * s = icn_top_subject(prog->c[i]); if (s && s->t == TT_PROC_DECL) procs[k++] = s; }
    for (int i = 0; i < prog->n; i++) { const tree_t * s = icn_top_subject(prog->c[i]); if (!s || s->t != TT_INVOCABLE) continue;
        for (int j = 0; j < s->n; j++) { const char * nm = s->c[j] ? s->c[j]->v.sval : NULL; if (!nm) continue;
            if (!strcmp(nm, "all")) { ct_drop(procs); ct_drop(keep); ct_drop(walked); return; }
            for (int q = 0; q < np; q++) if (procs[q]->v.sval && !strcmp(procs[q]->v.sval, nm)) keep[q] = 1; } }
    for (int q = 0; q < np; q++) if (procs[q]->v.sval && !strcmp(procs[q]->v.sval, "main")) keep[q] = 1;
    for (int i = 0; i < prog->n; i++) { const tree_t * s = icn_top_subject(prog->c[i]); if (s && s->t != TT_PROC_DECL) icn_mark_proc_refs(s, procs, np, keep); }
    int again = 1; while (again) { again = 0; for (int q = 0; q < np; q++) if (keep[q] && !walked[q]) { walked[q] = 1; icn_mark_proc_refs(procs[q], procs, np, keep); again = 1; } }
    int w = 0; for (int i = 0; i < prog->n; i++) { const tree_t * s = icn_top_subject(prog->c[i]); int drop = 0;
        if (s && s->t == TT_PROC_DECL) for (int q = 0; q < np; q++) if (procs[q] == s) { drop = !keep[q]; break; }
        if (!drop) prog->c[w++] = prog->c[i]; }
    prog->n = w; ct_drop(procs); ct_drop(keep); ct_drop(walked);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void *icon_parse_dialect(const char *source, const char *filename, int unicon) {
    tree_t *ast = NULL;
    tree_t **out_ast = &ast;
    if (!filename) filename = "<stdin>";
    IcnLexer lx;
    icn_pp_set_source_path(filename);
    icn_lex_init(&lx, source);
    lx.unicon = unicon || icn_source_is_unicon(source);
    IcnParser parser;
    icn_parse_init(&parser, &lx);
    CODE_t *prog = icn_parse_file(&parser, out_ast);
    if (parser.had_error) {
        if (parser.oracle_line) { extern void core_error_voice_at(int, const char *, const char *, long, long); size_t bn = strlen(filename) + 1; char *base = (char *) ct_alloc(bn); icn_pp_source_base(base, bn); size_t tn = strlen(parser.oracle_tok) + strlen(parser.errmsg) + 8; char *tx = (char *) ct_alloc(tn); snprintf(tx, tn, "\"%s\": %s", parser.oracle_tok, parser.errmsg); core_error_voice_at(703, tx, base, parser.oracle_line, 0); }
        else fprintf(stderr, "icon: parse error in %s: %s\n", filename, parser.errmsg);
        ct_drop(prog);
        if (out_ast) *out_ast = NULL;
        exit(1);
    }
    if (lx.pp_fatals) { fprintf(stderr, "icon: %d preprocessing error%s in %s\n", lx.pp_fatals, lx.pp_fatals == 1 ? "" : "s", filename); exit(1); }
    (void)prog;
    if (ast) ast->v.ival = lx.unicon;
    return ast;
}
void *icon_compile_parse(const char *source, const char *filename) { return icon_parse_dialect(source, filename, 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void icon_compile_finish(void *parsed, const char *filename, tree_t **out_ast) {
    if (!filename) filename = "<stdin>";
    if (out_ast) { *out_ast = (tree_t *)parsed; if (*out_ast) { icn_resolve_imports(*out_ast, filename); icn_resolve_links(*out_ast, filename); } }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void icon_compile_as(const char *source, const char *filename, tree_t **out_ast, int unicon) {
    if (out_ast) *out_ast = NULL;
    icon_compile_finish(icon_parse_dialect(source, filename, unicon), filename, out_ast);
}
void icon_compile(const char *source, const char *filename, tree_t **out_ast) { icon_compile_as(source, filename, out_ast, 0); }
