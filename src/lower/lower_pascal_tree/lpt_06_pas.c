#include "lower/lower_pascal_tree/lpt_head.h"
#include "lower/g_lower_pas.h"
#include <stdio.h>
#include <stdlib.h>
#include "ir/ast.h"
#include "parsers/snobol4/scrip_cc.h"
#include "lower/lower_pascal_types.h"
#include "lower/lower_pascal_tree/lpt_01_pas.h"
#include "lower/lower_pascal_tree/lpt_02_pas.h"
#include "lower/lower_pascal_tree/lpt_03_pas.h"
#include "lower/lower_pascal_tree/lpt_04_sh.h"
#include "lower/lower_pascal_tree/lpt_05_pas.h"
#include "lower/lower_pascal_tree/lpt_06_pas.h"
#include "lower/lower_pascal_tree/lpt_07_sh.h"
#include "lower/lower_pascal_tree/lpt_08_pas.h"
#include "lower/lower_pascal_tree/lpt_11_sh.h"
static int pas_is_int_typename(const char *t);
static int pas_is_tfilevar(const char *name);
static int pas_is_boolvar(const char *name);
int pas_scope_has(const char *name) {
    const char *req[] = { "true", "false", "maxint", "input", "output", "nil", "char", "boolean", "text", "string", "widechar", "paramcount", "pointer" };
    if (!name) return 1;
    if (pas_is_int_typename(name) || pas_is_realtypename(name)) return 1;
    for (size_t i = 0; i < sizeof req / sizeof req[0]; i++) if (!strcmp(name, req[i])) return 1;
    for (int i = g_lower_pas.sem.pas_scope.n - 1; i >= 0; i--) if (g_lower_pas.sem.pas_scope.defs[i].name && !strcmp(g_lower_pas.sem.pas_scope.defs[i].name, name)) return 1;
    return 0;
}
const char *pas_arrtype_elem(const char *name) { return pty_tn_elem(name); }
long long pas_enumtype_high(const char *n) {
    if (!n) return -1;
    for (int i = 0; i < g_lower_pas.sem.pas_nenum; i++) if (g_lower_pas.sem.pas_enumtypes[i].name && !strcmp(g_lower_pas.sem.pas_enumtypes[i].name, n)) return g_lower_pas.sem.pas_enumtypes[i].high;
    return -1;
}
static int pas_enumtype_min_size(const char *n, long long *out) {
    if (!n) return 0;
    for (int i = 0; i < g_lower_pas.sem.pas_nenum; i++) if (g_lower_pas.sem.pas_enumtypes[i].name && !strcmp(g_lower_pas.sem.pas_enumtypes[i].name, n)) {
        *out = g_lower_pas.sem.pas_enumtypes[i].min_size;
        return 1;
    }
    return 0;
}
static int pas_subtype_isenum(const char *n) {
    if (!n) return 0;
    for (int i = 0; i < g_lower_pas.sem.pas_nsubtype; i++) if (g_lower_pas.sem.pas_subtypes[i].name && !strcmp(g_lower_pas.sem.pas_subtypes[i].name, n)) return g_lower_pas.sem.pas_subtypes[i].isenum;
    return 0;
}
long long pas_subtype_high(const char *n) {
    if (!n) return -1;
    for (int i = 0; i < g_lower_pas.sem.pas_nsubtype; i++) if (g_lower_pas.sem.pas_subtypes[i].name && !strcmp(g_lower_pas.sem.pas_subtypes[i].name, n)) return g_lower_pas.sem.pas_subtypes[i].high;
    return -1;
}
long long pas_subtype_low(const char *n) {
    if (!n) return 0;
    for (int i = 0; i < g_lower_pas.sem.pas_nsubtype; i++) if (g_lower_pas.sem.pas_subtypes[i].name && !strcmp(g_lower_pas.sem.pas_subtypes[i].name, n)) return g_lower_pas.sem.pas_subtypes[i].low;
    return 0;
}
void pas_subvar_add(const char *n, long long lo, long long hi) {
    if (n) {
        if (g_lower_pas.sem.pas_nsubvar >= g_lower_pas.sem.pas_subvars_cap) g_lower_pas.sem.pas_subvars =
            pas_pool_grow(g_lower_pas.sem.pas_subvars, &g_lower_pas.sem.pas_subvars_cap, sizeof *g_lower_pas.sem.pas_subvars, 16);
        g_lower_pas.sem.pas_subvars[g_lower_pas.sem.pas_nsubvar].name = ct_strdup(n);
        g_lower_pas.sem.pas_subvars[g_lower_pas.sem.pas_nsubvar].uid = pas_scope_uid(n);
        g_lower_pas.sem.pas_subvars[g_lower_pas.sem.pas_nsubvar].low = lo;
        g_lower_pas.sem.pas_subvars[g_lower_pas.sem.pas_nsubvar].high = hi;
        g_lower_pas.sem.pas_nsubvar++;
    }
}
int pas_subvar_get(const char *n, long long *lo, long long *hi) {
    if (!n) return 0;
    int u = pas_scope_uid(n);
    for (int i = g_lower_pas.sem.pas_nsubvar - 1; i >= 0; i--) if (g_lower_pas.sem.pas_subvars[i].name && !strcmp(g_lower_pas.sem.pas_subvars[i].name, n) && g_lower_pas.sem.pas_subvars[i].uid == u) {
        *lo = g_lower_pas.sem.pas_subvars[i].low;
        *hi = g_lower_pas.sem.pas_subvars[i].high;
        return 1;
    }
    return 0;
}
const char *pas_typealias_get(const char *n) {
    if (!n) return NULL;
    for (int i = g_lower_pas.sem.pas_ntypealias - 1; i >= 0; i--) if (g_lower_pas.sem.pas_typealias[i].name && !strcmp(g_lower_pas.sem.pas_typealias[i].name, n))
        return g_lower_pas.sem.pas_typealias[i].target;
    return NULL;
}
void pas_scalarvartype_add(const char *vn, const char *tn) {
    if (vn && tn && g_lower_pas.sem.pas_nscalarvartype >= g_lower_pas.sem.pas_scalarvartype_cap) {
        g_lower_pas.sem.pas_scalarvartype_cap = g_lower_pas.sem.pas_scalarvartype_cap ? g_lower_pas.sem.pas_scalarvartype_cap * 2 : 256;
        g_lower_pas.sem.pas_scalarvartype = ct_grow(g_lower_pas.sem.pas_scalarvartype, (size_t)g_lower_pas.sem.pas_scalarvartype_cap * sizeof *g_lower_pas.sem.pas_scalarvartype);
    }
    if (vn && tn) {
        g_lower_pas.sem.pas_scalarvartype[g_lower_pas.sem.pas_nscalarvartype].vname = ct_strdup(vn);
        g_lower_pas.sem.pas_scalarvartype[g_lower_pas.sem.pas_nscalarvartype].tname = ct_strdup(tn);
        g_lower_pas.sem.pas_scalarvartype[g_lower_pas.sem.pas_nscalarvartype].is_global = (g_lower_pas.sem.pas_ldepth == 0);
        g_lower_pas.sem.pas_scalarvartype[g_lower_pas.sem.pas_nscalarvartype].uid = pas_scope_uid(vn);
        g_lower_pas.sem.pas_nscalarvartype++;
    }
}
const char *pas_scalarvartype_get(const char *vn) {
    if (!vn) return NULL;
    int u = pas_scope_uid(vn);
    for (int i = g_lower_pas.sem.pas_nscalarvartype - 1; i >= 0; i--)
        if (g_lower_pas.sem.pas_scalarvartype[i].vname && g_lower_pas.sem.pas_scalarvartype[i].uid == u && !strcmp(g_lower_pas.sem.pas_scalarvartype[i].vname, vn))
        return g_lower_pas.sem.pas_scalarvartype[i].tname;
    for (int i = g_lower_pas.sem.pas_nscalarvartype - 1; i >= 0; i--) if (g_lower_pas.sem.pas_scalarvartype[i].vname && !strcmp(g_lower_pas.sem.pas_scalarvartype[i].vname, vn))
        return g_lower_pas.sem.pas_scalarvartype[i].tname;
    return NULL;
}
void pas_tfcomp_add(const char *n, long long lo, long long hi, int ischar) {
    if (n) {
        if (g_lower_pas.sem.pas_ntfcomp >= g_lower_pas.sem.pas_tfcomp_cap) g_lower_pas.sem.pas_tfcomp =
            pas_pool_grow(g_lower_pas.sem.pas_tfcomp, &g_lower_pas.sem.pas_tfcomp_cap, sizeof *g_lower_pas.sem.pas_tfcomp, 16);
        g_lower_pas.sem.pas_tfcomp[g_lower_pas.sem.pas_ntfcomp].name = ct_strdup(n);
        g_lower_pas.sem.pas_tfcomp[g_lower_pas.sem.pas_ntfcomp].low = lo;
        g_lower_pas.sem.pas_tfcomp[g_lower_pas.sem.pas_ntfcomp].high = hi;
        g_lower_pas.sem.pas_tfcomp[g_lower_pas.sem.pas_ntfcomp].ischar = ischar;
        g_lower_pas.sem.pas_ntfcomp++;
    }
}
static long long pas_settype_hi(const char *name) {
    if (!name) return -1;
    for (int i = 0; i < g_lower_pas.sem.pas_nsettype; i++) if (g_lower_pas.sem.pas_settypes[i].name && !strcmp(g_lower_pas.sem.pas_settypes[i].name, name)) return g_lower_pas.sem.pas_settypes[i].hi;
    return -1;
}
static long long pas_settype_pack(const char *name) {
    if (!name) return 0;
    for (int i = 0; i < g_lower_pas.sem.pas_nsettype; i++) if (g_lower_pas.sem.pas_settypes[i].name && !strcmp(g_lower_pas.sem.pas_settypes[i].name, name)) return g_lower_pas.sem.pas_settypes[i].pack;
    return 0;
}
int pas_is_setvar(const char *name) { return pty_name_is(name, PTK_SET) > 0; }
int pas_is_singlevar(const char *name) { const pty_t *t = pty_name_type(name); return t && t->k == PTK_REAL && t->name && !strcmp(t->name, "single"); }
static int pas_is_tfilevar(const char *name) { return pty_name_is(name, PTK_FILE) > 0; }
int pas_is_tfiletype(const char *name) {
    if (!name) return 0;
    for (int i = 0; i < g_lower_pas.sem.pas_ntfiletype; i++) if (g_lower_pas.sem.pas_tfiletypes[i].name && !strcmp(g_lower_pas.sem.pas_tfiletypes[i].name, name)) return 1;
    return 0;
}
int pas_is_tfile_node(const tree_t *e) { return e && e->t == TT_VAR && e->v.sval && pas_is_tfilevar(e->v.sval); }
static int pas_is_boolvar(const char *name) { return pty_name_is(name, PTK_BOOL) > 0; }
int pas_func_returns_stringish(const char *fname) {
    const char *t = fname ? pas_scalarvartype_get(fname) : NULL;
    for (int guard = 0; t && guard < 8; guard++) {
        if (pas_arrtype_high(t) >= 0) return 0;
        if (!strcmp(t, "string") || !strcmp(t, "ansistring") || !strcmp(t, "shortstring") || !strcmp(t, "widestring") || !strcmp(t, "unicodestring")) return 1;
        const char *al = pas_typealias_get(t);
        if (!al || !strcmp(al, t)) break;
        t = al;
    }
    return 0;
}
int pas_not_a_set(tree_t *e) {
    if (!e) return 0;
    if (e->t == TT_ILIT || e->t == TT_FLIT) return 1;
    if (e->t != TT_VAR || !e->v.sval || pas_is_setvar(e->v.sval)) return 0;
    const char *tn = pas_scalarvartype_get(e->v.sval);
    return tn && !pas_is_settype(tn);
}
static int pas_var_unsigned(const char *name) {
    if (!name) return 0;
    const char *tn = pas_scalarvartype_get(name);
    long long lo, hi;
    if (tn && (pas_is_unsigned_inttypename(tn) || (pas_subtype_high(tn) >= 0 && pas_subtype_low(tn) >= 0))) return 1;
    return pas_subvar_get(name, &lo, &hi) && lo >= 0;
}
int pas_unsigned_sum(tree_t *e, int *nv) {
    if (!e) return 0;
    if (e->t == TT_ILIT) return e->v.ival >= 0;
    if (e->t == TT_VAR) { if (!pas_var_unsigned(e->v.sval)) return 0; (*nv)++; return 1; }
    return e->t == TT_ADD && e->n == 2 && pas_unsigned_sum(e->c[0], nv) && pas_unsigned_sum(e->c[1], nv);
}
int pas_is_realtypename(const char *t) { return t && (!strcmp(t, "real") || !strcmp(t, "single") || !strcmp(t, "double") || !strcmp(t, "extended") || !strcmp(t, "comp")); }
static int pas_is_int_typename(const char *t) {
    return t &&
        (!strcmp(t, "integer") || !strcmp(t, "longint") || !strcmp(t, "shortint") || !strcmp(t, "byte") || !strcmp(t, "word") || !strcmp(t, "cardinal") || !strcmp(t, "int64") || !strcmp(t, "qword") ||
        !strcmp(t, "smallint") || !strcmp(t, "longword") || !strcmp(t, "uint8") || !strcmp(t, "uint16") || !strcmp(t, "uint32") || !strcmp(t, "uint64") || !strcmp(t, "int8") || !strcmp(t, "int16") ||
        !strcmp(t, "int32") || !strcmp(t, "nativeint") || !strcmp(t, "nativeuint") || !strcmp(t, "sizeint") || !strcmp(t, "sizeuint") || !strcmp(t, "ptrint") || !strcmp(t, "ptruint"));
}
static int pas_var_typename_is(const char *name, int (*pred)(const char *)) {
    const char *t = name ? pas_scalarvartype_get(name) : NULL;
    for (int guard = 0; t && guard < 8; guard++) { if (pred(t)) return 1; const char *al = pas_typealias_get(t); if (!al || !strcmp(al, t)) break; t = al; }
    return 0;
}
int pas_var_is_real(const char *name) { return pas_var_typename_is(name, pas_is_realtypename); }
int pas_var_every_decl_real(const char *name) {
    int n = 0;
    for (int i = 0; name && i < g_lower_pas.sem.pas_nscalarvartype; i++) if (g_lower_pas.sem.pas_scalarvartype[i].vname && !strcmp(g_lower_pas.sem.pas_scalarvartype[i].vname, name)) {
        const char *t = g_lower_pas.sem.pas_scalarvartype[i].tname;
        for (int guard = 0; t && guard < 8 && !pas_is_realtypename(t); guard++) { const char *al = pas_typealias_get(t); if (!al || !strcmp(al, t)) break; t = al; }
        if (!t || !pas_is_realtypename(t)) return 0;
        n++;
    }
    return n > 0;
}
char pas_expr_lit_class(tree_t *e) {
    if (!e) return 0;
    if (e->t == TT_FLIT) return 'r';
    if (e->t == TT_QLIT) return (e->v.sval && strlen(e->v.sval) == 1) ? 'c' : 's';
    if (e->t == TT_FNC && e->n >= 2 && e->c[0] && e->c[0]->v.sval && !strcmp(e->c[0]->v.sval, "__pas_chrlit")) return 'c';
    if (e->t == TT_EQ && e->n == 2 && e->c[0] && e->c[1] && e->c[0]->t == TT_ILIT && e->c[1]->t == TT_ILIT) return 'b';
    return 0;
}
char pas_typename_class(const char *t) {
    char c = 0;
    for (int guard = 0; t && guard < 8 && !c; guard++) {
        const char *ua = pas_typealias_get(t);
        if (ua && strcmp(ua, t)) { t = ua; continue; }
        if (pas_is_int_typename(t)) c = 'i';
        else if (pas_is_realtypename(t)) c = 'r';
        else if (!strcmp(t, "char")) c = 'c';
        else if (pas_is_booltype(t)) c = 'b';
        else if (pas_enumtype_high(t) >= 0) c = 'e';
        else { const char *al = pas_typealias_get(t); if (!al || !strcmp(al, t)) break; t = al; }
    }
    return c;
}
static char pas_var_decl_class(const char *name) {
    char k = 0;
    if (!name || pas_is_chararr(name) || pas_is_strarr(name)) return 0;
    if (pty_name_agg(name) > 0) return 0;
    for (int i = 0; name && i < g_lower_pas.sem.pas_nscalarvartype; i++) if (g_lower_pas.sem.pas_scalarvartype[i].vname && !strcmp(g_lower_pas.sem.pas_scalarvartype[i].vname, name)) {
        char c = pas_typename_class(g_lower_pas.sem.pas_scalarvartype[i].tname);
        if (!c || (k && k != c)) return 0;
        k = c;
    }
    return k;
}
static const char *pas_int_name(const char *n) { return (n && g_lower_pas.sem.pas_int16 && !strcmp(n, "integer")) ? "smallint" : n; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pas_ord_var_bounds(const char *name, long long *lo, long long *hi) {
    int n = 0;
    long long l = 0, h = -1;
    if (!name || pas_is_chararr(name) || pas_is_strarr(name)) return 0;
    if (pty_name_agg(name) > 0) return 0;
    for (int i = 0; i < g_lower_pas.sem.pas_nscalarvartype; i++) if (g_lower_pas.sem.pas_scalarvartype[i].vname && !strcmp(g_lower_pas.sem.pas_scalarvartype[i].vname, name)) {
        const char *t = g_lower_pas.sem.pas_scalarvartype[i].tname;
        long long cl = 0, ch = -1;
        int ok = 0;
        for (int guard = 0; t && guard < 8 && !ok; guard++) {
            if (!strcmp(t, "integer")) {
                cl = g_lower_pas.sem.pas_int16 ? -32768LL : -2147483647LL;
                ch = g_lower_pas.sem.pas_int16 ? 32767LL : 2147483647LL;
                ok = 1;
            } else if (!strcmp(t, "char")) {
                cl = 0;
                ch = 255;
                ok = 1;
            } else if (pas_is_booltype(t)) {
                cl = 0;
                ch = 1;
                ok = 1;
            } else if (pas_enumtype_high(t) >= 0 && pas_subtype_high(t) < 0) {
                cl = 0;
                ch = pas_enumtype_high(t);
                ok = 1;
            } else {
                const char *al = pas_typealias_get(t);
                if (!al || !strcmp(al, t)) break;
                t = al;
            }
        }
        if (!ok || (n && (cl != l || ch != h))) return 0;
        l = cl;
        h = ch;
        n++;
    }
    if (!n) return 0;
    *lo = l;
    *hi = h;
    return 1;
}
static const char *pas_class_name(char k) {
    switch (k) {
        case 'i':
        return "integer-type";
        case 'r':
        return "real-type";
        case 'c':
        return "char-type";
        case 'b':
        return "Boolean-type";
        case 'e':
        return "an enumerated-type";
        case 's':
        return "a string-type";
        default:
        return "?";
    }
}
void pas_sign_operand(tree_t *e, char op) {
    char k = pas_expr_lit_class(e);
    if (!k && e && e->t == TT_VAR && e->v.sval) k = pas_var_decl_class(e->v.sval);
    if (!k || k == 'i' || k == 'r') return;
    fprintf(stderr, "pascal: ISO 7185 6.7.2.2 violation: the monadic %c is applied to an operand of %s, and it takes only integer-type or real-type (table 4)\n", op, pas_class_name(k));
    g_lower_pas.sem.pas_iso_errors++;
}
void pas_set_member_ordinal(tree_t *e) {
    char k = pas_expr_lit_class(e);
    if (!k && e && e->t == TT_VAR && e->v.sval) k = pas_var_decl_class(e->v.sval);
    if (k != 'r' && k != 's') return;
    fprintf(stderr, "pascal: ISO 7185 6.7.1 violation: a member-designator of a set-constructor is of %s, and the type of its expressions shall be an ordinal-type\n", pas_class_name(k));
    g_lower_pas.sem.pas_iso_errors++;
}
static void pas_variant_values(tree_t *e, long long *v, int *n, int cap) {
    if (!e) return;
    if (e->t == TT_ADD && e->n == 2) { pas_variant_values(e->c[0], v, n, cap); pas_variant_values(e->c[1], v, n, cap); return; }
    if (e->t == TT_EQ && e->n == 2 && e->c[1] && e->c[1]->t == TT_ILIT && *n < cap) v[(*n)++] = e->c[1]->v.ival;
}
void pas_variant_constants_in_tag_type(const char *tag_type, PNodeList *arms) {
    const char *t = tag_type;
    long long lo = 0, hi = -1;
    for (int guard = 0; t && guard < 8 && hi < lo; guard++) {
        if (pas_is_booltype(t)) {
            lo = 0;
            hi = 1;
        } else if (pas_subtype_high(t) >= 0) {
            lo = pas_subtype_low(t);
            hi = pas_subtype_high(t);
        } else if (pas_enumtype_high(t) >= 0) {
            lo = 0;
            hi = pas_enumtype_high(t);
        } else {
            const char *al = pas_typealias_get(t);
            if (!al || !strcmp(al, t)) break;
            t = al;
        }
    }
    if (hi < lo || hi - lo >= 4096 || !arms) return;
    long long v[4096];
    int n = 0;
    unsigned char seen[4096];
    memset(seen, 0, sizeof seen);
    for (int i = 0; i < arms->count; i++) pas_variant_values(arms->items[i], v, &n, 4096);
    for (int i = 0; i < n; i++) {
        if (v[i] < lo || v[i] > hi) {
            fprintf(stderr, "pascal: ISO 7185 6.4.3.3 violation: a case-constant of the variant-part denotes %lld, which is not a value of its tag-type %s (%lld..%lld)\n", v[i], tag_type, lo, hi);
            g_lower_pas.sem.pas_iso_errors++;
            return;
        }
        if (seen[v[i] - lo]++) {
            fprintf(stderr, "pascal: ISO 7185 6.4.3.3 violation: the value %lld of the tag-type %s is denoted by more than one case-constant of the variant-part\n", v[i], tag_type);
            g_lower_pas.sem.pas_iso_errors++;
            return;
        }
    }
    for (long long x = lo; x <= hi; x++) if (!seen[x - lo]) {
        fprintf(stderr, "pascal: ISO 7185 6.4.3.3 violation: the case-constants of the variant-part do not denote the value %lld of its tag-type %s, and they shall denote every value of it\n", x,
            tag_type);
        g_lower_pas.sem.pas_iso_errors++;
        return;
    }
}
int pas_typealias_is_char(const char *t) {
    for (int guard = 0; t && guard < 8; guard++) { const char *al = pas_typealias_get(t); if (!al || !strcmp(al, t)) return 0; if (!strcmp(al, "char")) return 1; t = al; }
    return 0;
}
static int pas_class_compatible(char to, char from) { return to == from || (to == 'r' && from == 'i'); }
void pas_value_compat(const char *vn, tree_t *rhs, const char *clause, const char *what) {
    char to = pas_var_decl_class(vn), from = pas_expr_lit_class(rhs);
    if (!to || !from || pas_class_compatible(to, from)) return;
    fprintf(stderr, "pascal: ISO 7185 %s violation: %s '%s' is given a value of %s, which is not assignment-compatible with its %s (6.4.6)\n", clause, what, vn, pas_class_name(from),
        pas_class_name(to));
    g_lower_pas.sem.pas_iso_errors++;
}
void pas_deref_assign_compat(tree_t *lhs, tree_t *rhs) {
    if (!lhs || lhs->t != TT_FNC || lhs->n != 2 || !lhs->c[0] || !lhs->c[0]->v.sval || strcmp(lhs->c[0]->v.sval, "__pas_deref")) return;
    const char *tn = pas_ptrexpr_target(lhs->c[1]);
    char to = pas_typename_class(tn), from = pas_expr_lit_class(rhs);
    if (!from && to == 'c' && rhs && rhs->t == TT_ILIT) from = 'i';
    if (!to || !from || pas_class_compatible(to, from)) return;
    fprintf(stderr,
        "pascal: ISO 7185 6.8.2.2 violation: the variable referenced through a pointer of the domain type '%s' is given a value of %s, which is not assignment-compatible with its %s (6.4.6)\n", tn,
        pas_class_name(from), pas_class_name(to));
    g_lower_pas.sem.pas_iso_errors++;
}
int pas_wrap_sbits_of_type(const char *t) {
    for (int g = 0; t && g < 8; g++) {
        t = pas_int_name(t);
        if (!strcmp(t, "shortint") || !strcmp(t, "int8")) return 8;
        if (!strcmp(t, "smallint") || !strcmp(t, "int16")) return 16;
        if (!strcmp(t, "longint") || !strcmp(t, "int32") || !strcmp(t, "integer")) return 32;
        const char *al = pas_typealias_get(t);
        if (!al || !strcmp(al, t)) break;
        t = al;
    }
    return 0;
}
void pas_for_const_bounds(const char *cv, tree_t *from, tree_t *to, int down) {
    if (!cv || !from || !to || from->t != TT_ILIT || to->t != TT_ILIT) return;
    long long lo = 0, hi = -1, a = from->v.ival, b = to->v.ival;
    int n = 0;
    for (int i = 0; i < g_lower_pas.sem.pas_nsubvar; i++) if (g_lower_pas.sem.pas_subvars[i].name && !strcmp(g_lower_pas.sem.pas_subvars[i].name, cv)) {
        if (n && (g_lower_pas.sem.pas_subvars[i].low != lo || g_lower_pas.sem.pas_subvars[i].high != hi)) return;
        lo = g_lower_pas.sem.pas_subvars[i].low;
        hi = g_lower_pas.sem.pas_subvars[i].high;
        n++;
    }
    if (!n) return;
    { const char *vt = pas_scalarvartype_get(cv); if (vt && strcmp(vt, "?") && pas_subtype_high(vt) < 0) return; }
    if (down ? a < b : a > b) return;
    if (a >= lo && a <= hi && b >= lo && b <= hi) return;
    fprintf(stderr,
        "pascal: ISO 7185 6.8.3.9 violation: the for-statement over '%s' (of type %lld..%lld) runs from %lld to %lld, and a value outside the"
        " control-variable's type is not assignment-compatible with it\n", cv, lo, hi, a, b);
    g_lower_pas.sem.pas_iso_errors++;
}
int pas_is_signed_intexpr(tree_t *e) {
    static const char *const T[] = { "integer", "longint", "int64", "smallint", "shortint" };
    if (!e) return 0;
    if (e->t == TT_ILIT) return 1;
    if (e->t == TT_VAR && e->v.sval && !pas_array_isdyn(e->v.sval)) {
        long long hi;
        const char *t = pas_array_high_get(e->v.sval, &hi) ? NULL : pas_scalarvartype_get(e->v.sval);
        for (int k = 0; t && k < 5; k++) if (!strcmp(t, T[k])) return 1;
        return 0;
    }
    if ((e->t == TT_ADD || e->t == TT_SUB || e->t == TT_MUL || e->t == TT_MOD) && e->n == 2) return pas_is_signed_intexpr(e->c[0]) && pas_is_signed_intexpr(e->c[1]);
    if (e->t == TT_FNC && e->n == 3 && e->c[0] && e->c[0]->v.sval && (!strcmp(e->c[0]->v.sval, "iand") || !strcmp(e->c[0]->v.sval, "ior") || !strcmp(e->c[0]->v.sval, "ixor")))
        return pas_is_signed_intexpr(e->c[1]) && pas_is_signed_intexpr(e->c[2]);
    return 0;
}
static const char *pas_cond_var_nonbool_type(tree_t *e) {
    if (!e || e->t != TT_VAR || !e->v.sval || pas_is_boolvar(e->v.sval)) return NULL;
    const char *found = NULL;
    for (int i = 0; i < g_lower_pas.sem.pas_nscalarvartype; i++) if (g_lower_pas.sem.pas_scalarvartype[i].vname && !strcmp(g_lower_pas.sem.pas_scalarvartype[i].vname, e->v.sval)) {
        const char *t = g_lower_pas.sem.pas_scalarvartype[i].tname;
        for (int guard = 0; t && guard < 8; guard++) { const char *al = pas_typealias_get(t); if (!al || !strcmp(al, t)) break; t = al; }
        if (!t || pas_is_booltype(t) || !(pas_enumtype_high(t) >= 0 || pas_is_int_typename(t) || pas_is_realtypename(t) || !strcmp(t, "char"))) return NULL;
        found = g_lower_pas.sem.pas_scalarvartype[i].tname;
    }
    return found;
}
tree_t *pas_cond_bool(tree_t *e, const char *stmt, const char *clause) {
    const char *t = pas_cond_var_nonbool_type(e);
    if (t) {
        fprintf(stderr, "pascal: ISO 7185 %s violation: the Boolean-expression of %s is the variable '%s' of type %s, which is not the required-type Boolean\n", clause, stmt, e->v.sval, t);
        g_lower_pas.sem.pas_iso_errors++;
    }
    return lpt_pas_cond(e);
}
tree_t *pas_bool(tree_t *e) { return e; }
long long pas_range_fit_size(long long lo, long long hi) {
    if (lo >= 0) { if (hi <= 255) return 1; if (hi <= 65535) return 2; return 4; }
    if (lo >= -128 && hi <= 127) return 1;
    if (lo >= -32768 && hi <= 32767) return 2;
    return 4;
}
int pas_lohi_bits(tree_t *a) {
    long long sz = 8;
    int lit = a && a->t == TT_ILIT, neg = a && a->t == TT_MNS && a->n == 1 && a->c[0] && a->c[0]->t == TT_ILIT;
    if (lit || neg) {
        long long v = lit ? a->v.ival : -a->c[0]->v.ival;
        if (v >= -128 && v <= 127) return 8;
        if (v >= 0 && v <= 255) return 4;
        return (v >= -32768 && v <= 65535) ? 8 : (v >= -2147483648LL && v <= 4294967295LL) ? 16 : 32;
    }
    int named = a && (a->t == TT_VAR || a->t == TT_IDX || a->t == TT_FIELD || (a->t == TT_FNC && a->n >= 1 && a->c[0] && a->c[0]->v.sval && pas_is_func(a->c[0]->v.sval)));
    const pty_t *t = named ? pty_of_low(a) : NULL;
    if (!t || t->k != PTK_INT) return 32;
    if (!(t->name && pas_sizeof_lookup(t->name, &sz))) sz = pas_range_fit_size(t->lo, t->hi);
    return sz == 1 ? (t->lo >= 0 ? 4 : 8) : sz == 2 ? 8 : sz == 4 ? 16 : 32;
}
int pas_sizeof_builtin_size(const char *n, long long *out) {
    if (!n) return 0;
    n = pas_int_name(n);
    static const struct {
        const char *n;
        long long sz;
    } T[] = { {"integer",4},{"longint",4},{"shortint",1},{"byte",1},{"word",2},{"smallint",2}, {"cardinal",4},{"longword",4},{"int64",8},{"qword",8},{"single",4},{"real",8}, {"double",8},{"extended",
        10},{"comp",8},{"char",1},{"widechar",1}, {"boolean",1},{"bytebool",1},{"wordbool",2},{"longbool",4},{"qwordbool",8},{"pointer",8}, {"ptruint",8},{"ptrint",8},{"int8",1},{"int16",2},{"int32",
        4},{"uint8",1}, {"uint16",2},{"uint32",4},{"uint64",8},{"nativeint",8},{"nativeuint",8}, {"codepointer",8}, };
    for (size_t i = 0; i < sizeof(T) / sizeof(T[0]); i++) if (!strcmp(n, T[i].n)) { *out = T[i].sz; return 1; }
    return 0;
}
int pas_sizeof_lookup(const char *name, long long *out) {
    if (!name) return 0;
    if (pas_sizeof_builtin_size(name, out)) return 1;
    if (pas_arrtype_high(name) >= 0 && pas_tsz_get(name, out)) return 1;
    { const char *al = pas_typealias_get(name); if (al && strcmp(al, name)) return pas_sizeof_lookup(al, out); }
    {
        long long eh = pas_enumtype_high(name);
        if (eh >= 0) { long long ms; if (!pas_enumtype_min_size(name, &ms)) ms = 4; long long fit = pas_range_fit_size(0, eh); *out = fit > ms ? fit : ms; return 1; }
    }
    if (pas_is_settype(name)) { long long pk = pas_settype_pack(name); if (pk > 0) { *out = pk; return 1; } long long sh = pas_settype_hi(name); *out = (sh >= 0 && sh <= 31) ? 4 : 32; return 1; }
    { long long sh = pas_subtype_high(name); if (sh >= 0) { *out = pas_range_fit_size(pas_subtype_low(name), sh); return 1; } }
    { long long lo, hi; if (pas_subvar_get(name, &lo, &hi)) { *out = pas_range_fit_size(lo, hi); return 1; } }
    if (pas_rectype_nf(name) > 0 && pas_rectype_total_size(name, out)) return 1;
    if (pas_rvt(name)) { const char *rt = pas_with_sel_rtype(leaf_s(TT_VAR, name)); return (rt && pas_rectype_total_size(rt, out)) ? 1 : 0; }
    if (pty_name_agg(name) > 0) return pas_tsz_get(name, out);
    if (pas_is_charvar(name)) { *out = 1; return 1; }
    if (pas_is_boolvar(name)) { *out = 1; return 1; }
    if (pas_ptrvar_target(name)) { *out = 8; return 1; }
    { const char *tn = pas_scalarvartype_get(name); if (tn && strcmp(tn, name)) return pas_sizeof_lookup(tn, out); }
    return pas_tsz_get(name, out);
}
static int pas_ordinal_builtin_bound(const char *n, long long *lo, long long *hi) {
    if (!n) return 0;
    n = pas_int_name(n);
    static const struct {
        const char *n;
        long long lo;
        long long hi;
    } T[] = { {"byte",0,255},{"shortint",-128,127},{"word",0,65535},{"smallint",-32768,32767}, {"longword",0,4294967295LL},{"cardinal",0,4294967295LL},{"uint32",0,4294967295LL}, {"longint",
        -2147483648LL,2147483647LL},{"integer",-2147483648LL,2147483647LL}, {"int32",-2147483648LL,2147483647LL},{"int8",-128,127},{"uint8",0,255}, {"int16",-32768,32767},{"uint16",0,65535}, {"int64",
        (long long)0x8000000000000000ULL,0x7FFFFFFFFFFFFFFFLL}, {"qword",0,-1},{"uint64",0,-1},{"nativeint",(long long)0x8000000000000000ULL,0x7FFFFFFFFFFFFFFFLL}, {"nativeuint",0,-1},{"ptruint",0,
        -1},{"sizeuint",0,-1},{"ptrint",(long long)0x8000000000000000ULL,0x7FFFFFFFFFFFFFFFLL},{"sizeint",(long long)0x8000000000000000ULL,0x7FFFFFFFFFFFFFFFLL},{"char",0,255}, {"widechar",0,255},
        {"boolean",0,1},{"bytebool",0,255}, };
    for (size_t i = 0; i < sizeof(T) / sizeof(T[0]); i++) if (!strcmp(n, T[i].n)) { *lo = T[i].lo; *hi = T[i].hi; return 1; }
    return 0;
}
int pas_ordinal_bound_lookup(const char *name, long long *lo, long long *hi) {
    if (!name) return 0;
    if (pas_ordinal_builtin_bound(name, lo, hi)) return 1;
    { const char *al = pas_typealias_get(name); if (al && strcmp(al, name)) return pas_ordinal_bound_lookup(al, lo, hi); }
    { long long sh = pas_subtype_high(name); if (sh >= 0) { *lo = pas_subtype_low(name); *hi = sh; return 1; } }
    if (pas_subvar_get(name, lo, hi)) return 1;
    if (pty_name_agg(name) > 0) return 0;
    if (pas_rvt(name)) return 0;
    if (pas_is_charvar(name)) { *lo = 0; *hi = 255; return 1; }
    if (pas_is_boolvar(name)) { *lo = 0; *hi = 1; return 1; }
    { const char *tn = pas_scalarvartype_get(name); if (tn && strcmp(tn, name)) return pas_ordinal_bound_lookup(tn, lo, hi); }
    return 0;
}
int pas_raw_int_w(const pty_t *et, long long *w, long long *sg) {
    if (et && et->k == PTK_BOOL) { *w = 1; *sg = 0; return 1; }
    if (et && et->k == PTK_CHAR) { *w = 1; *sg = 0; return 1; }
    if (et && et->k == PTK_ENUM && et->hi >= 0 && et->hi < 256) { *w = 1; *sg = 0; return 1; }
    if (!et || et->k != PTK_INT || !et->name) return 0;
    const char *n = pas_int_name(et->name);
    if (!strcmp(n, "byte") || !strcmp(n, "uint8")) { *w = 1; *sg = 0; return 1; }
    if (!strcmp(n, "shortint") || !strcmp(n, "int8")) { *w = 1; *sg = 1; return 1; }
    if (!strcmp(n, "word") || !strcmp(n, "uint16")) { *w = 2; *sg = 0; return 1; }
    if (!strcmp(n, "smallint") || !strcmp(n, "int16")) { *w = 2; *sg = 1; return 1; }
    if (!strcmp(n, "longword") || !strcmp(n, "cardinal") || !strcmp(n, "uint32")) { *w = 4; *sg = 0; return 1; }
    if (!strcmp(n, "longint") || !strcmp(n, "integer") || !strcmp(n, "int32")) { *w = 4; *sg = 1; return 1; }
    if (!strcmp(n, "int64")) { *w = 8; *sg = 1; return 1; }
    if (!strcmp(n, "qword") || !strcmp(n, "uint64")) { *w = 8; *sg = 0; return 1; }
    if (et->bounded && et->lo >= 0 && et->hi < 256) { *w = 1; *sg = 0; return 1; }
    return 0;
}
int pas_index_is_enum(long long v) {
    if (g_lower_pas.sem.pas_pend_sub_high >= 0 && g_lower_pas.sem.pas_pend_sub_high == v) return g_lower_pas.sem.pas_last_const_enum;
    if (g_lower_pas.sem.pas_pend_typename && pas_subtype_high(g_lower_pas.sem.pas_pend_typename) >= 0 && pas_subtype_high(g_lower_pas.sem.pas_pend_typename) == v)
        return pas_subtype_isenum(g_lower_pas.sem.pas_pend_typename);
    return 0;
}
long long pas_index_bound(long long v, int want_hi) {
    long long lo = 0, hi = -1;
    if (g_lower_pas.sem.pas_pend_sub_high >= 0 && g_lower_pas.sem.pas_pend_sub_high == v) {
        lo = g_lower_pas.sem.pas_pend_sub_low;
        hi = v;
    } else if (g_lower_pas.sem.pas_pend_typename && pas_subtype_high(g_lower_pas.sem.pas_pend_typename) >= 0 && pas_subtype_high(g_lower_pas.sem.pas_pend_typename) == v) {
        lo = pas_subtype_low(g_lower_pas.sem.pas_pend_typename);
        hi = v;
    } else if (v >= 0) {
        lo = 0;
        hi = v;
    } else if (g_lower_pas.sem.pas_pend_typename) {
        long long bl, bh;
        if (pas_ordinal_bound_lookup(g_lower_pas.sem.pas_pend_typename, &bl, &bh) && bh >= bl && bh - bl < 65536) { lo = bl; hi = bh; }
    }
    return want_hi ? hi : lo;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *pas_sig_value_type(const char *sig, int idx, int var_too) {
    if (!sig || strlen(sig) < 3) return NULL;
    const char *p = sig + 2;
    int at = 0;
    while (*p && *p != ')') {
        const char *q = p;
        int dep = 0;
        while (*q && !(dep == 0 && (*q == ';' || *q == ')'))) { if (*q == '(') dep++; else if (*q == ')') dep--; q++; }
        char k = *p;
        int cnt = (k == 'v' || k == 'r') ? atoi(p + 1) : 1;
        if (idx < at + cnt) {
            const char *c = (k == 'v' || (var_too && k == 'r')) ? memchr(p, ':', (size_t)(q - p)) : NULL;
            if (!c) return NULL;
            char *t = (char *)ct_zalloc(1, (size_t)(q - c));
            memcpy(t, c + 1, (size_t)(q - c - 1));
            return t;
        }
        at += cnt;
        p = (*q == ';') ? q + 1 : q;
    }
    return NULL;
}
void pas_formal_subranges(const char *sig, PNodeList *params) {
    for (int i = 0, fi = 0; params && i < params->count; i++) {
        tree_t *id = params->items[i];
        if (!id || !id->v.sval || !strncmp(id->v.sval, "__pas_pe", 8)) continue;
        const char *t = pas_sig_value_type(sig, fi++, 1);
        if (t && pas_subtype_high(t) >= 0) pas_subvar_add(id->v.sval, pas_subtype_low(t), pas_subtype_high(t));
    }
}
long long pas_elem_size(const char *an) {
    long long ah = -1, z = -1;
    if (an && !pas_array_isdyn(an) && pas_array_high_get(an, &ah) && pas_sizeof_lookup(an, &z) && z > 0) return z / (ah - pas_array_low(an) + 1);
    const char *t = an ? pas_scalarvartype_get(an) : NULL;
    if (t && pas_arrtype_elem(t)) t = pas_arrtype_elem(t);
    if (t) { const pty_t *at = pty_type_by_name(t); if (at && at->k == PTK_ARRAY) { const char *bn = at->base ? pty_name_of(at->base) : NULL; if (!bn) return -1; t = bn; } }
    if (t && pas_sizeof_builtin_size(t, &z) && z > 0) return z;
    if (t && pas_sizeof_lookup(t, &z) && z > 0) return z;
    return -1;
}
void pas_formal_tfiles(const char *sig, PNodeList *params) {
    for (int i = 0, fi = 0; params && i < params->count; i++) {
        tree_t *id = params->items[i];
        if (!id || !id->v.sval || !strncmp(id->v.sval, "__pas_pe", 8)) continue;
        const char *t = pas_sig_value_type(sig, fi++, 1);
        if (t && pas_is_tfiletype(t)) { pas_tfcomp_add(id->v.sval, 0, -1, 0); }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pas_formal_is_dynarr(const char *callee, int idx) { const PasDef *cd = pas_pf_lookup(callee); const char *t = cd ? pas_sig_value_type(cd->sig, idx, 1) : NULL; return t && pas_arrtype_isdyn(t); }
int pas_formal_is_aoc(const char *callee, int idx) { const PasDef *cd = pas_pf_lookup(callee); const char *t = cd ? pas_sig_value_type(cd->sig, idx, 1) : NULL; return t && !strcmp(t, "$open$const"); }
int pas_formal_elem_is_string(const char *callee, int idx) {
    const PasDef *cd = pas_pf_lookup(callee);
    const char *t = cd ? pas_sig_value_type(cd->sig, idx, 1) : NULL;
    if (!t) return 0;
    if (!strncmp(t, "$open$", 6)) t += 6;
    else if (pas_arrtype_isdyn(t) && pas_arrtype_elem(t)) t = pas_arrtype_elem(t);
    return pas_string_type_kind(t) || !strcmp(t, "string");
}
void pas_value_actuals_check(const char *callee, const PasDef *cd, PNodeList *args) {
    for (int i = 0; i + 1 < args->count; i += 2) {
        const char *st = pas_sig_value_type(cd->sig, i / 2, 1);
        if (st && pas_string_type_kind(st) && args->items[i] && args->items[i]->t != TT_QLIT && pas_is_charexpr(args->items[i])) args->items[i] = pas_concat_operand(args->items[i]);
    }
    for (int i = 0; i + 1 < args->count; i += 2) {
        const char *t = pas_sig_value_type(cd->sig, i / 2, 0);
        tree_t *a = args->items[i];
        const char *at = NULL;
        const char *rt = t ? NULL : pas_sig_value_type(cd->sig, i / 2, 1);
        long long _lo, _hi;
        if (rt && pas_subtype_high(rt) < 0 && strcmp(rt, "char") && !pas_is_booltype(rt) && a && a->t == TT_VAR && a->v.sval && !pas_is_setvar(a->v.sval) && pas_subvar_get(a->v.sval, &_lo, &_hi)) {
            fprintf(stderr,
                "pascal: ISO 7185 6.6.3.3 violation: the actual-parameter %d of '%s' is a variable of the subrange type %lld..%lld, which is not the type %s of its variable formal-parameter\n",
                i / 2 + 1, callee, _lo, _hi, rt);
            g_lower_pas.sem.pas_iso_errors++;
            continue;
        }
        if (!t || !a || (strcmp(t, "integer") && strcmp(t, "real") && strcmp(t, "char") && strcmp(t, "boolean"))) continue;
        if (a->t == TT_FNC && a->n == 2 && a->c[0] && a->c[0]->v.sval && !strcmp(a->c[0]->v.sval, "__pas_chrlit")) at = "char";
        else if (a->t == TT_FLIT) at = "real";
        else if (a->t == TT_QLIT && a->v.sval && strlen(a->v.sval) > 1) at = "string";
        else if (a->t == TT_ILIT && !strcmp(t, "char")) at = "non-char ordinal";
        if (!at || !strcmp(at, t)) continue;
        fprintf(stderr, "pascal: ISO 7185 6.6.3.2 violation: the actual-parameter %d of '%s' is a %s value, which is not assignment-compatible with the type %s of its value formal-parameter\n",
            i / 2 + 1, callee, at, t);
        g_lower_pas.sem.pas_iso_errors++;
    }
}
pval E_expression(const tree_t *n) { pval o = E_expression_raw(n); int c = pty_capped(n); if (c > 0 && o.node) o.node = mk_fnc2("__pas_sstr_fit", o.node, ilit(c)); pty_note_low(o.node, n); return o; }
pval E_program_in(const tree_t *n) {
    pval v[9];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    int bi = n->n - 1;
    while (bi > 0 && n->c[bi] && n->c[bi]->t == TT_ATTR) bi--;
    int hasf = bi > 1 && n->c[1] && n->c[1]->t == TT_VLIST;
    if (IS(n, TT_PROGRAM)) {
        { PasDef *ent = pas_scope_push(&g_lower_pas.sem.pas_scope.defs, &g_lower_pas.sem.pas_scope.n, &g_lower_pas.sem.pas_scope.cap, "main"); ent->rid = 1; ent->depth = -1; }
        v[2].str = (n->c[0] && n->c[0]->t == TT_BLOCK) ? "program" : tok_s(n->c[0]);
        v[3] = E_file_id_list_opt(hasf ? n->c[1] : NULL);
        v[5] = E_block((tok_events(n), n->c[bi]));
        {
            tree_t *body = v[5].node;
            const char **gn;
            long long *gh;
            int gc = pty_global_aggs(&gn, &gh);
            if (gc > 0 || g_lower_pas.sem.pas_nhdrfile > 0 || g_lower_pas.sem.pas_nscalarvartype > 0) {
                tree_t *combined = ast_node_new(TT_PROGRAM);
                for (int i = 0; i < gc; i++) ast_push(combined, bin(TT_ASSIGN, leaf_s(TT_VAR, gn[i]), mk_array_init(gn[i], gh[i])));
                for (int i = 0; i < g_lower_pas.sem.pas_nscalarvartype; i++) {
                    const char *vn = g_lower_pas.sem.pas_scalarvartype[i].vname;
                    long long _ah;
                    if (!vn || !g_lower_pas.sem.pas_scalarvartype[i].is_global || pas_is_func(vn) || pas_is_proc(vn) || pas_array_high_get(vn, &_ah) || pas_array_isdyn(vn)) continue;
                    if (pas_var_typename_is(vn, pas_is_int_typename) || pas_var_typename_is(vn, pas_is_booltype)) ast_push(combined, bin(TT_ASSIGN, leaf_s(TT_VAR, vn), ilit(0)));
                    else if (pas_var_typename_is(vn, pas_is_realtypename)) ast_push(combined, bin(TT_ASSIGN, leaf_s(TT_VAR, vn), flit(0.0)));
                }
                if (g_lower_pas.sem.pas_mode_iso || !g_lower_pas.sem.pas_seen_mode_directive) for (int i = 0; i < g_lower_pas.sem.pas_nhdrfile; i++) {
                    const char *hn = g_lower_pas.sem.pas_hdrfiles[i], *ht = hn ? pas_scalarvartype_get(hn) : NULL;
                    if (ht && !strcmp(ht, "text")) ast_push(combined, bin(TT_ASSIGN, leaf_s(TT_VAR, hn), ilit(-(long long)(i + 1))));
                }
                if (body && body->t == TT_PROGRAM) { for (int i = 0; i < body->n; i++) ast_push(combined, body->c[i]); } else if (body) ast_push(combined, body);
                body = combined;
            }
            if (g_lower_pas.sem.pas_ioflow) {
                tree_t *_nb = ast_node_new(TT_PROGRAM);
                ast_push(_nb, mk_fnc1("__pas_ioflow", ilit(1)));
                if (body && body->t == TT_PROGRAM) { for (int i = 0; i < body->n; i++) ast_push(_nb, body->c[i]); } else if (body) ast_push(_nb, body);
                body = _nb;
            }
            body = pas_trace_prepend_tap_off(body);
            tree_t *mainp = mk_proc("main", NULL, body, 0, 0, NULL, 0);
            emit_proc(&g_lower_pas.sem.pascal_procs, mainp);
            for (int i = 0; i < g_lower_pas.sem.pascal_procs.count; i++) pas_pf_resolve(g_lower_pas.sem.pascal_procs.items[i]);
            tree_t *root = ast_stmt_new(TT_PROGRAM);
            for (int i = 0; i < g_lower_pas.sem.pascal_procs.count; i++) ast_push(root, g_lower_pas.sem.pascal_procs.items[i]);
            out.node = root;
        }
        return out;
    }
    elab_bad("program", n);
    return out;
}
long long pas_index_low_of(const tree_t *idx) {
    if (idx && idx->t == TT_SUBRANGE) return g_lower_pas.sem.pas_pend_sub_low;
    if (idx && idx->t == TT_VAR && idx->v.sval && pas_subtype_high(idx->v.sval) >= 0) return pas_subtype_low(idx->v.sval);
    return 0;
}
