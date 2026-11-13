#include "lower/lower_pascal_tree/lpt_head.h"
#include "lower/g_lower_pas.h"
#include <stdio.h>
#include "runtime/core/g_runtime.h"
#include "ir/ast.h"
#include "lower/lower_pascal_types.h"
#include "lower/lower_pascal_tree/lpt_01_pas.h"
#include "lower/lower_pascal_tree/lpt_02_pas.h"
#include "lower/lower_pascal_tree/lpt_03_pas.h"
#include "lower/lower_pascal_tree/lpt_04_sh.h"
#include "lower/lower_pascal_tree/lpt_05_pas.h"
#include "lower/lower_pascal_tree/lpt_06_pas.h"
#include "lower/lower_pascal_tree/lpt_07_sh.h"
#include "lower/lower_pascal_tree/lpt_08_pas.h"
#include "lower/lower_pascal_tree/lpt_09_sh.h"
#include "lower/lower_pascal_tree/lpt_11_sh.h"
#include "lower/lower_pascal_tree/lpt_12_pas.h"
static int pas_tfcomp_range(const char *n, long long *lo, long long *hi);
tree_t *mk_call_raw(const char *name, PNodeList *args) {
    if (name && !strcmp(name, "fini.mute")) return mk_fnc0("__pas_std_mute");
    if (name && !g_lower_pas.sem.pas_dialect_fpc && (!strcmp(name, "random") || !strcmp(name, "randomize") || !strcmp(name, "stringofchar")) && !pas_is_func(name) && !pas_is_proc(name) &&
        !pas_scalarvartype_get(name)) pas_scope_require(name);
    if (name && !strcmp(name, "ord") && args && args->count >= 1) {
        tree_t *a = args->items[0];
        if (a && a->t == TT_FNC && a->n >= 2 && a->c[0] && a->c[0]->v.sval && !strcmp(a->c[0]->v.sval, "__pas_chrlit")) return a->c[1];
        if (a && a->t == TT_IDX) a->v.ival = 0;
        if (pas_is_charexpr(a) || pas_is_boolexpr(a)) return bin(TT_ADD, a, ilit(0));
        return a;
    }
    if (name && (!strcmp(name, "pack") || !strcmp(name, "unpack")) && args && args->count >= 5) {
        int ispack = !strcmp(name, "pack");
        tree_t *a = args->items[ispack ? 0 : 2];
        tree_t *i = args->items[ispack ? 2 : 4];
        tree_t *z = args->items[ispack ? 4 : 0];
        long long zhi = -1;
        long long zlo = 1;
        if (z && z->t == TT_VAR && z->v.sval) {
            if (!pas_array_high_get(z->v.sval, &zhi)) zhi = -1;
            if (pas_is_chararr(z->v.sval)) { long long _cl = pas_chararr_lo(z->v.sval); if (_cl > 0) zlo = _cl; } else { zlo = pas_array_low(z->v.sval); }
        }
        if (a && a->t == TT_VAR && a->v.sval && z && z->t == TT_VAR && z->v.sval && zhi >= 0) {
            long long ahi = -1;
            int _ahok = pas_array_high_get(a->v.sval, &ahi);
            long long alo = pas_array_low(a->v.sval);
            long long cnt = zhi - zlo + 1;
            if (i && (pas_is_charexpr(i) || (i->t == TT_FNC && i->n >= 2 && i->c[0] && i->c[0]->v.sval && !strcmp(i->c[0]->v.sval, "__pas_chrlit")))) {
                fprintf(stderr, "pascal: ISO 7185 6.6.5.4 violation: the ordinal parameter of %s is not assignment-compatible with the index-type of the unpacked array '%s'\n", name, a->v.sval);
                g_lower_pas.sem.pas_iso_errors++;
            } else if (_ahok && cnt > 0 && i && i->t == TT_ILIT) {
                long long st = i->v.ival;
                if (st < alo || st + cnt - 1 > ahi) {
                    fprintf(stderr, "pascal: ISO 7185 6.6.5.4 violation: %s over the unpacked array '%s' declared [%lld..%lld] reads components [%lld..%lld], which is outside its index-type\n", name,
                        a->v.sval, alo, ahi, st, st + cnt - 1);
                    g_lower_pas.sem.pas_iso_errors++;
                }
            }
        }
        {
            tree_t *src = ispack ? a : z;
            if (src && src->t == TT_VAR && src->v.sval && !pas_array_is_param(src->v.sval) && !pas_assigned_get(src->v.sval) && !pas_array_is_zero_size(src->v.sval)) {
                fprintf(stderr, "pascal: ISO 7185 6.6.5.4 violation: %s reads the array '%s', whose components have never been assigned a value\n", name, src->v.sval);
                g_lower_pas.sem.pas_iso_errors++;
            }
        }
        char _cvb[24];
        snprintf(_cvb, sizeof _cvb, "__pas_pk%d", g_lower_pas.sem.pkn++);
        const char *_cv = ct_strdup(_cvb);
        tree_t *zi = ast_node_new(TT_IDX);
        ast_push(zi, pas_tree_clone(z));
        ast_push(zi, leaf_s(TT_VAR, _cv));
        tree_t *ai = ast_node_new(TT_IDX);
        ast_push(ai, pas_tree_clone(a));
        ast_push(ai, bin(TT_ADD, pas_tree_clone(i), bin(TT_SUB, leaf_s(TT_VAR, _cv), ilit(zlo))));
        tree_t *body = ispack ? mk_assign(zi, ai) : mk_assign(ai, zi);
        tree_t *f = ast_node_new(TT_FOR);
        ast_push(f, leaf_s(TT_VAR, _cv));
        ast_push(f, ilit(zlo));
        ast_push(f, ilit(zhi));
        ast_push(f, body);
        return f;
    }
    if (name && !strcmp(name, "get") && args && args->count >= 1 && args->items[0] && args->items[0]->t == TT_VAR && args->items[0]->v.sval && !strcmp(args->items[0]->v.sval, "input") &&
        pas_is_stdstream("input") && !pas_ptrvar_target("input")) return mk_fnc0("__pas_read_c");
    if (name && !strcmp(name, "put") && args && args->count >= 1 && args->items[0] && args->items[0]->t == TT_VAR && args->items[0]->v.sval && !strcmp(args->items[0]->v.sval, "output") &&
        pas_is_stdstream("output") && !pas_ptrvar_target("output")) {
        PNodeList *pa = pnl_new();
        pnl_push(pa, mk_chrlit(leaf_s(TT_VAR, "__pas_outbuf")));
        pnl_push(pa, ilit(-1));
        return mk_call_raw("write", pa);
    }
    if (name && (!strcmp(name, "get") || !strcmp(name, "put")) && args && args->count >= 1 && args->items[0] && args->items[0]->t == TT_VAR && args->items[0]->v.sval &&
        pas_is_filevar(args->items[0]->v.sval) && !pas_is_stdstream(args->items[0]->v.sval)) {
        return mk_fnc1(!strcmp(name, "get") ? "__pas_tget" : "__pas_tput", args->items[0]);
    }
    if (name && args && args->count >= 1 && pas_is_tfile_node(args->items[0])) {
        tree_t *fa = args->items[0];
        if (!strcmp(name, "readln") || !strcmp(name, "writeln")) {
            fprintf(stderr, "pascal: ISO 7185 %s violation: the file parameter of '%s' shall be a textfile, but '%s' is a file of a non-text component-type\n",
                !strcmp(name, "readln") ? "6.9.2" : "6.9.4", name, fa->v.sval ? fa->v.sval : "<file>");
            g_lower_pas.sem.pas_iso_errors++;
        }
        if (!strcmp(name, "get")) return mk_fnc1("__pas_fget", fa);
        if (!strcmp(name, "put")) return mk_fnc1("__pas_fput", fa);
        if (!strcmp(name, "eof")) return mk_fnc1("__pas_feof_t", fa);
        if (!strcmp(name, "reset")) return pas_is_hdrfile(fa->v.sval) ? mk_assign(fa, mk_set_bin("__pas_treset", pas_tree_clone(fa), leaf_s(TT_QLIT, fa->v.sval))) :
            mk_assign(fa, mk_fnc1("__pas_treset", pas_tree_clone(fa)));
        if (!strcmp(name, "rewrite")) return pas_is_hdrfile(fa->v.sval) ? mk_assign(fa, mk_set_bin("__pas_trewrite", pas_tree_clone(fa), leaf_s(TT_QLIT, fa->v.sval))) :
            mk_assign(fa, mk_fnc1("__pas_trewrite", pas_tree_clone(fa)));
        if (!strcmp(name, "read") || !strcmp(name, "write")) {
            PNodeList *stmts = pnl_new();
            for (int i = 2; i + 1 < args->count; i += 2) {
                tree_t *v = args->items[i];
                long long _rlo, _rhi;
                if (!strcmp(name, "read") && v && v->t == TT_VAR && v->v.sval && pas_subvar_get(v->v.sval, &_rlo, &_rhi)) {
                    tree_t *rc = ast_node_new(TT_FNC);
                    ast_push(rc, leaf_s(TT_VAR, "__pas_fread_range"));
                    ast_push(rc, pas_tree_clone(fa));
                    ast_push(rc, ilit(_rlo));
                    ast_push(rc, ilit(_rhi));
                    pnl_push(stmts, mk_assign(v, rc));
                } else if (!strcmp(name, "write") && fa && fa->t == TT_VAR && fa->v.sval && pas_tfcomp_range(fa->v.sval, &_rlo, &_rhi)) {
                    tree_t *wc = ast_node_new(TT_FNC);
                    ast_push(wc, leaf_s(TT_VAR, "__pas_fwrite_range"));
                    ast_push(wc, pas_tree_clone(fa));
                    ast_push(wc, v);
                    ast_push(wc, ilit(_rlo));
                    ast_push(wc, ilit(_rhi));
                    pnl_push(stmts, wc);
                } else if (!strcmp(name, "read")) pnl_push(stmts, mk_assign(v, mk_fnc1("__pas_fread", pas_tree_clone(fa))));
                else pnl_push(stmts, mk_set_bin("__pas_fwrite", pas_tree_clone(fa), v));
            }
            return seq_of(stmts);
        }
    }
    if (name && !strcmp(name, "chr") && args && args->count >= 1) {
        tree_t *a = args->items[0];
        if (a && a->t != TT_ILIT) a = pas_ord_check(a, 0, 255, "chr(x) of a value that is the ordinal number of no char-type value");
        return mk_chrlit(a);
    }
    if (name && (!strcmp(name, "pred") || !strcmp(name, "succ")) && args && args->count >= 1) {
        int up = !strcmp(name, "succ");
        long long lo, hi;
        tree_t *a = args->items[0];
        tree_t *v = bin(up ? TT_ADD : TT_SUB, a, ilit(1));
        if (a && a->t == TT_VAR && pas_ord_var_bounds(a->v.sval, &lo, &hi)) v = pas_ord_check(v, lo, hi, up ? "succ(x) of the largest value of its type" : "pred(x) of the smallest value of its type");
        if (a && ((a->t == TT_FNC && a->n >= 2 && a->c[0] && a->c[0]->v.sval && !strcmp(a->c[0]->v.sval, "__pas_chrlit")) || pas_is_charexpr(a))) return mk_chrlit(v);
        if (a && pas_is_boolexpr(a)) return bin(TT_NE, v, ilit(0));
        return v;
    }
    if (name && (!strcmp(name, "inc") || !strcmp(name, "dec")) && args && args->count >= 1) {
        tree_t *v = args->items[0];
        tree_t *delta = (args->count >= 3) ? args->items[2] : ilit(1);
        tree_e op = !strcmp(name, "inc") ? TT_ADD : TT_SUB;
        if (pas_ptr_typed(v)) return mk_assign(v, mk_fnc2("__pas_ptr_inc", pas_tree_clone(v), op == TT_ADD ? delta : bin(TT_SUB, ilit(0), delta)));
        return mk_assign(v, bin(op, pas_tree_clone(v), delta));
    }
    if (name && !strcmp(name, "setlength") && args && args->count >= 4 && args->items[0] && args->items[0]->t == TT_VAR && args->items[0]->v.sval && pas_array_isdyn(args->items[0]->v.sval)) {
        tree_t *v = args->items[0];
        int depth = pas_array_isdyn(v->v.sval);
        const char *et = pas_scalarvartype_get(v->v.sval);
        for (int g = 0; et && pas_arrtype_isdyn(et) && pas_arrtype_elem(et) && g < 8; g++) et = pas_arrtype_elem(et);
        tree_t *proto = ilit(0);
        if (pas_arrrec_find(v->v.sval, NULL) > 0) proto = mk_rec_block_of(pas_art(v->v.sval), 0);
        else if (et && pas_rectype_nf(et) > 0) proto = mk_rec_init_type(et);
        else if (et && pas_is_realtypename(et)) proto = flit(0.0);
        else if (et && (pas_string_type_kind(et) || !strcmp(et, "string"))) proto = leaf_s(TT_QLIT, "");
        else if (et && !strcmp(et, "char")) proto = mk_chrlit(ilit(0));
        tree_t *f = ast_node_new(TT_FNC);
        ast_push(f, leaf_s(TT_VAR, "__pas_dynset"));
        ast_push(f, pas_tree_clone(v));
        ast_push(f, proto);
        ast_push(f, ilit(depth));
        for (int i = 2; i < args->count; i += 2) ast_push(f, args->items[i]);
        return mk_assign(v, f);
    }
    if (name && !strcmp(name, "setlength") && args && args->count >= 4 && args->items[0] && args->items[0]->t == TT_IDX) {
        tree_t *v = args->items[0];
        const char *et = NULL;
        const pty_t *tl = NULL;
        int depth = pas_dyn_depth(v, &et), tdepth = depth > 0 ? 0 : pas_ty_dyn_depth(v, &tl);
        if (depth > 0 || tdepth > 0) {
            for (int g = 0; et && pas_arrtype_isdyn(et) && pas_arrtype_elem(et) && g < 8; g++) et = pas_arrtype_elem(et);
            tree_t *proto = ilit(0);
            if (tdepth > 0) { proto = pas_ty_dyn_proto(tl); depth = tdepth; } else if (et && pas_rectype_nf(et) > 0) proto = mk_rec_init_type(et);
            else if (et && pas_is_realtypename(et)) proto = flit(0.0);
            else if (et && (pas_string_type_kind(et) || !strcmp(et, "string"))) proto = leaf_s(TT_QLIT, "");
            else if (et && !strcmp(et, "char")) proto = mk_chrlit(ilit(0));
            tree_t *f = ast_node_new(TT_FNC);
            ast_push(f, leaf_s(TT_VAR, "__pas_dynset"));
            ast_push(f, pas_tree_clone(v));
            ast_push(f, proto);
            ast_push(f, ilit(depth));
            for (int i = 2; i < args->count; i += 2) ast_push(f, args->items[i]);
            return mk_assign(v, f);
        }
    }
    if (name && !strcmp(name, "length") && !pas_is_func(name) && !pas_is_proc(name) && args && args->count >= 1 && pas_pchar_typed(args->items[0])) return mk_fnc1("__pas_pstrlen", args->items[0]);
    if (name && !strcmp(name, "length") && !pas_is_func(name) && !pas_is_proc(name) && args && args->count >= 1 && args->items[0] && args->items[0]->t == TT_VAR) {
        long long d2[4];
        if (pas_array_dims(args->items[0]->v.sval, d2)) return ilit(d2[1] - d2[0] + 1);
    }
    if (name && !strcmp(name, "length") && args && args->count >= 1 && pas_is_dynexpr(args->items[0])) return mk_fnc1("__pas_dynlen", args->items[0]);
    if (name && !strcmp(name, "length") && !pas_is_func(name) && args && args->count >= 1 && args->items[0] && args->items[0]->t == TT_VAR && !pas_var_string_kind(args->items[0]->v.sval)) {
        long long hi = 0;
        if (pas_array_high_get(args->items[0]->v.sval, &hi)) return ilit(hi - pas_array_low(args->items[0]->v.sval) + 1);
    }
    if (name && !pas_is_func(name) && !pas_is_proc(name) && g_lower_pas.sem.pas_seen_mode_directive && !g_lower_pas.sem.pas_mode_iso) {
        if (!strcmp(name, "random")) return (args && args->count >= 1) ? mk_fnc1("__pas_random", args->items[0]) : mk_fnc0("__pas_random");
        if (!strcmp(name, "randomize")) return mk_fnc0("__pas_randomize");
        if (!strcmp(name, "stringofchar") && args && args->count >= 3) return mk_fnc2("__pas_stringofchar", args->items[0], args->items[2]);
        if (!strcmp(name, "str") && args && args->count >= 4) {
            tree_t *v = args->items[0], *w = args->items[1], *p = ilit(-1), *dst = args->items[args->count - 2];
            if (v && IS(v, TT_FNC) && v->n >= 1 && IS(v->c[0], TT_VAR) && v->c[0]->v.sval && !strcmp(v->c[0]->v.sval, "__pas_xr_str")) return mk_assign(dst, v);
            if (pas_xr_low(v)) {
                if (w && w->t == TT_ILIT && w->v.ival == -3 && args->count >= 6) { w = args->items[2]; p = args->items[3]; }
                tree_t *xs = ast_node_new(TT_FNC);
                ast_push(xs, leaf_s(TT_VAR, "__pas_xr_str"));
                ast_push(xs, ilit(0));
                ast_push(xs, v);
                ast_push(xs, w && w->t == TT_ILIT && w->v.ival < 0 ? ilit(-1) : w);
                ast_push(xs, p);
                return mk_assign(dst, xs);
            }
            if (w && w->t == TT_ILIT && w->v.ival == -3 && args->count >= 6) { w = args->items[2]; p = args->items[3]; }
            if (w && w->t == TT_ILIT && w->v.ival < 0 && w->v.ival != -3 && pas_is_qword(v) && v->t != TT_ILIT) return mk_assign(dst, mk_fnc1("__pas_ustr", v));
            tree_t *f = ast_node_new(TT_FNC);
            ast_push(f, leaf_s(TT_VAR, "__pas_strf"));
            ast_push(f, v);
            ast_push(f, (w && w->t == TT_ILIT && w->v.ival < 0) ? ilit(-1) : w);
            ast_push(f, p);
            ast_push(f, ilit(g_lower_pas.sem.pas_mode_iso || !g_lower_pas.sem.pas_seen_mode_directive ? 0 : 1));
            return mk_assign(dst, f);
        }
    }
    if (name && !strcmp(name, "><") && args && args->count >= 3) {
        tree_t *x = args->items[0], *y = args->items[2];
        if (pas_not_a_set(x) || pas_not_a_set(y)) {
            fprintf(stderr, "pascal: ISO 7185 6.7.2.4 violation: '><' is the set symmetric difference and takes two set operands, and an operand here is not a set\n");
            g_lower_pas.sem.pas_iso_errors++;
        }
        return mk_set_bin("__pas_setuni", mk_set_bin("__pas_setdif", x, y), mk_set_bin("__pas_setdif", pas_tree_clone(y), pas_tree_clone(x)));
    }
    if (name && (!strcmp(name, "shl") || !strcmp(name, "shr") || !strcmp(name, "xor")) && args && args->count >= 3 && !pas_is_func(name) && !pas_is_proc(name)) {
        tree_t *x = args->items[0], *y = args->items[2];
        if (!strcmp(name, "xor")) return (pas_is_boolexpr(x) && pas_is_boolexpr(y)) ? bin(TT_NE, x, y) : mk_fnc2("ixor", x, y);
        if (!strcmp(name, "shr") && pas_is_qword(x) && x->t != TT_ILIT && y && y->t == TT_ILIT) {
            long long c = y->v.ival & 63;
            return c == 0 ? x : mk_fnc2("__pas_udiv", x, ilit(c == 63 ? INT64_MIN : (1LL << c)));
        }
        if (!strcmp(name, "shr") && pas_is_qword(x) && x->t != TT_ILIT) {
            tree_t *n1 = ast_node_new(TT_MNS), *n2 = ast_node_new(TT_MNS);
            ast_push(n1, mk_fnc2("iand", y, ilit(63)));
            ast_push(n2, mk_fnc2("iand", pas_tree_clone(y), ilit(63)));
            tree_t *keep = ast_node_new(TT_FNC);
            ast_push(keep, leaf_s(TT_VAR, "icom"));
            ast_push(keep, mk_fnc2("ishift", mk_fnc2("ishift", ilit(INT64_MIN), n2), ilit(1)));
            return mk_fnc2("iand", mk_fnc2("ishift", x, n1), keep);
        }
        if (!strcmp(name, "shr")) { tree_t *ng = ast_node_new(TT_MNS); ast_push(ng, y); y = ng; }
        return mk_fnc2("ishift", x, y);
    }
    if (name && !strcmp(name, "setlength") && args && args->count >= 4) {
        tree_t *v = args->items[0];
        tree_t *n = args->items[2];
        return mk_assign(v, mk_fnc2("__pas_setlength", pas_tree_clone(v), n));
    }
    if (name && (!strcmp(name, "low") || !strcmp(name, "high")) && !pas_is_func(name) && !pas_is_proc(name) && args && args->count >= 1) {
        tree_t *v0 = args->items[0];
        const char *an = (v0 && v0->t == TT_VAR) ? v0->v.sval : ((v0 && v0->t == TT_IDX && v0->n == 2 && v0->c[0] && v0->c[0]->t == TT_VAR) ? v0->c[0]->v.sval : NULL);
        long long d2[4];
        if (an && pas_array_dims(an, d2)) { int col = v0->t == TT_IDX; return ilit(name[0] == 'l' ? d2[col ? 2 : 0] : d2[col ? 3 : 1]); }
    }
    if (name && (!strcmp(name, "low") || !strcmp(name, "high")) && args && args->count >= 1 && pas_is_dynexpr(args->items[0])) return !strcmp(name, "low") ? ilit(0) :
        bin(TT_SUB, mk_fnc1("__pas_dynlen", args->items[0]), ilit(1));
    if (name && (!strcmp(name, "low") || !strcmp(name, "high")) && args && args->count >= 1) {
        tree_t *v = args->items[0];
        long long hi;
        if (v && v->t == TT_VAR && v->v.sval && pas_array_high_get(v->v.sval, &hi)) { return ilit(!strcmp(name, "low") ? pas_array_nonparam_low(v->v.sval) : hi); }
        if (v && v->t == TT_VAR && v->v.sval) { long long lo2, hi2; if (pas_ordinal_bound_lookup(v->v.sval, &lo2, &hi2)) return ilit(!strcmp(name, "low") ? lo2 : hi2); }
        if (v && v->t == TT_VAR && v->v.sval && pas_enumtype_high(v->v.sval) >= 0) return ilit(!strcmp(name, "low") ? 0 : pas_enumtype_high(v->v.sval));
        if (v && v->t == TT_VAR && v->v.sval && !pas_arrtype_isdyn(v->v.sval) && pas_arrtype_high(v->v.sval) >= 0 && pas_arrtype_ncols(v->v.sval) < 0)
            return ilit(!strcmp(name, "low") ? pas_arrtype_lo(v->v.sval) : pas_arrtype_high(v->v.sval));
        if (v && v->t == TT_VAR && v->v.sval && !pas_var_string_kind(v->v.sval) && pas_sconst_get(v->v.sval)) return ilit(name[0] == 'l' ? 0 : (long long)strlen(pas_sconst_get(v->v.sval)) - 1);
        if (v && v->t == TT_QLIT && v->v.sval) return ilit(name[0] == 'l' ? 0 : (long long)strlen(v->v.sval) - 1);
        if (v && v->t == TT_VAR && v->v.sval) {
            int sk = pas_var_string_kind(v->v.sval);
            if (sk == 1 && pty_shortmax(v) > 0) return ilit(name[0] == 'l' ? 0 : pty_shortmax(v));
            if (sk) {
                int _zb = sk == 1 && (g_lower_pas.sem.pas_zerobased_strings & 1);
                return !strcmp(name, "low") ? ilit(sk == 2 || _zb ? 0 : 1) :
                    (sk == 2 ? ilit(255) : (_zb ? bin(TT_SUB, mk_fnc1("length", pas_tree_clone(v)), ilit(1)) : mk_fnc1("length", pas_tree_clone(v))));
            }
        }
        if (v && v->t == TT_VAR && v->v.sval) { long long _tl, _th; if (pty_array_bounds(pty_name_type(v->v.sval), &_tl, &_th)) return ilit(name[0] == 'l' ? _tl : _th); }
    }
    if (name && (!strcmp(name, "low") || !strcmp(name, "high")) && args && args->count >= 1 && args->items[0] && args->items[0]->t == TT_FNC && args->items[0]->n == 2 && args->items[0]->c[0] &&
        args->items[0]->c[0]->v.sval && !strcmp(args->items[0]->c[0]->v.sval, "__pas_deref")) {
        const char *dt = pas_ptrexpr_target(args->items[0]->c[1]);
        if (dt && !pas_arrtype_isdyn(dt) && pas_arrtype_high(dt) >= 0 && pas_arrtype_ncols(dt) < 0) return ilit(!strcmp(name, "low") ? pas_arrtype_lo(dt) : pas_arrtype_high(dt));
    }
    if (name && (!strcmp(name, "low") || !strcmp(name, "high") || !strcmp(name, "length")) && !pas_is_func(name) && !pas_is_proc(name) && args && args->count >= 1 && args->items[0] &&
        args->items[0]->t == TT_IDX && args->items[0]->n == 2) return name[0] == 'l' &&
        name[1] == 'e' ? mk_fnc1("__pas_len", args->items[0]) : mk_fnc2("__pas_bound", args->items[0], ilit(name[0] == 'h'));
    if (name && !strcmp(name, "stringtowidechar") && args && args->count >= 6) {
        tree_t *x = args->items[2];
        if (x && x->t == TT_IDX && x->n == 2 && x->c[0] && x->c[0]->t == TT_VAR) {
            tree_t *f = ast_node_new(TT_FNC);
            ast_push(f, leaf_s(TT_VAR, "__pas_s2wide"));
            ast_push(f, args->items[0]);
            ast_push(f, x->c[0]);
            ast_push(f, x->c[1]);
            ast_push(f, args->items[4]);
            return f;
        }
    }
    if (name && (!strcmp(name, "widestring") || !strcmp(name, "ansistring") || !strcmp(name, "unicodestring")) && args && args->count >= 1 && pas_pchar_castname(args->items[0]) &&
        args->items[0]->c[1] && args->items[0]->c[1]->t == TT_IDX && args->items[0]->c[1]->n == 2 && args->items[0]->c[1]->c[0] && args->items[0]->c[1]->c[0]->t == TT_VAR &&
        !pas_var_string_kind(args->items[0]->c[1]->c[0]->v.sval)) {
        tree_t *x = args->items[0]->c[1];
        tree_t *f = ast_node_new(TT_FNC);
        ast_push(f, leaf_s(TT_VAR, "__pas_wide2s"));
        ast_push(f, x->c[0]);
        ast_push(f, x->c[1]);
        return f;
    }
    if (name && !strcmp(name, "sizeof") && args && args->count >= 1) {
        if (args->items[0] && args->items[0]->t == TT_FNC && args->items[0]->n == 2 && args->items[0]->c[0] && args->items[0]->c[0]->v.sval && !strcmp(args->items[0]->c[0]->v.sval, "__pas_deref") &&
            args->items[0]->c[1]) {
            const pty_t *_dpt = pty_of_low(args->items[0]->c[1]);
            if (_dpt && _dpt->k == PTK_POINTER && pty_is_pchar(_dpt)) return ilit(1);
            if (_dpt && _dpt->k == PTK_POINTER && _dpt->base && pty_sizeof_flat(_dpt->base) > 0) return ilit(pty_sizeof_flat(_dpt->base));
        }
        { long long _tz = pty_sizeof_low(args->items[0]); if (_tz > 0) return ilit(_tz); }
        if (args->items[0] && args->items[0]->t == TT_IDX && args->items[0]->n == 2 && args->items[0]->c[0] && args->items[0]->c[0]->t == TT_VAR) {
            long long z = pas_elem_size(args->items[0]->c[0]->v.sval);
            if (z > 0) return ilit(z);
            const pty_t *at = pty_name_type(args->items[0]->c[0]->v.sval);
            if (at && at->k == PTK_ARRAY && at->base && at->base->name && pas_sizeof_lookup(at->base->name, &z) && z > 0) return ilit(z);
        }
        tree_t *a = args->items[0];
        if (a && a->t == TT_VAR && a->v.sval) {
            long long sz;
            if (pas_sizeof_lookup(a->v.sval, &sz)) return ilit(sz);
            const pty_t *vt = pty_name_type(a->v.sval);
            if (vt && vt->name && strcmp(vt->name, a->v.sval) && pas_sizeof_lookup(vt->name, &sz) && sz > 0) return ilit(sz);
        }
    }
    if (name && !strcmp(name, "stringcodepage") && args && args->count >= 1) return ilit(g_lower_pas.sem.pas_codepage);
    if (name && !strcmp(name, "ismanagedtype") && args && args->count >= 1) {
        tree_t *a = args->items[0];
        const char *tn = (a && a->t == TT_VAR && a->v.sval) ? pas_scalarvartype_get(a->v.sval) : NULL;
        { int _guard = 0; while (tn) { const char *_al = pas_typealias_get(tn); if (!_al || !strcmp(_al, tn) || _guard++ >= 8) break; tn = _al; } }
        int managed = tn && (!strcmp(tn, "ansistring") || !strcmp(tn, "unicodestring") || !strcmp(tn, "widestring") || !strcmp(tn, "variant"));
        return bin(TT_NE, ilit(managed ? 1 : 0), ilit(0));
    }
    if (name && (!strcmp(name, "strlen") || !strcmp(name, "strpas")) && args && args->count == 2 && pas_pchar_typed(args->items[0]))
        return mk_fnc1(name[3] == 'l' ? "__pas_pstrlen" : "__pas_pstr", args->items[0]);
    if (name && !strcmp(name, "assigned") && !pas_is_func(name) && !pas_is_proc(name) && args && args->count >= 1) {
        if (pas_ptr_typed(args->items[0])) return bin(TT_EQ, mk_fnc2("__pas_ptr_eq", args->items[0], ilit(0)), ilit(0));
        return bin(TT_NE, args->items[0], ilit(0));
    }
    if (name && !strcmp(name, "boolean") && args && args->count >= 1) return bin(TT_NE, mk_fnc2("iand", args->items[0], ilit(255)), ilit(0));
    if (name && (!strcmp(name, "char") || !strcmp(name, "ansichar") || !strcmp(name, "widechar")) && args && args->count >= 1 && !pas_is_func(name) && !pas_is_proc(name) && !pas_scope_uid(name) &&
        g_lower_pas.sem.pas_dialect_fpc) return mk_chrlit(args->items[0]);
    if (name && strcmp(name, "char") && strcmp(name, "widechar") && strcmp(name, "ansichar") && strcmp(name, "boolean") && args && args->count >= 1) {
        long long _tsz;
        int _isboolfam = !strcmp(name, "bytebool") || !strcmp(name, "wordbool") || !strcmp(name, "longbool") || !strcmp(name, "qwordbool");
        const char *_tcn = name;
        { const char *_al = pas_typealias_get(_tcn); int _guard = 0; while (_al && strcmp(_al, _tcn) && _guard++ < 8) { _tcn = _al; _al = pas_typealias_get(_tcn); } }
        if ((!_isboolfam || pas_is_boolexpr(args->items[0])) && pas_sizeof_builtin_size(_tcn, &_tsz)) {
            if (_isboolfam || (_tsz != 1 && _tsz != 2 && _tsz != 4)) {
                tree_t *_a0 = args->items[0];
                return (_a0 && _a0->t == TT_VAR && _a0->v.sval && pas_var_is_boolfam(_a0->v.sval)) ? bin(TT_ADD, _a0, ilit(0)) : _a0;
            }
            long long _full = 1LL << (_tsz * 8);
            tree_t *_m = mk_fnc2("iand", args->items[0], ilit(_full - 1));
            if (pas_is_unsigned_inttypename(_tcn)) return _m;
            long long _half = _full / 2;
            return bin(TT_SUB, bin(TT_MOD, bin(TT_ADD, _m, ilit(_half)), ilit(_full)), ilit(_half));
        }
    }
    if (name && !strcmp(name, "swapendian") && args && args->count >= 1) {
        tree_t *a = args->items[0];
        long long sz = 4;
        if (a && a->t == TT_VAR && a->v.sval) pas_sizeof_lookup(a->v.sval, &sz);
        tree_t *e = ast_node_new(TT_FNC);
        ast_push(e, leaf_s(TT_VAR, "__pas_swapendian"));
        ast_push(e, a);
        ast_push(e, ilit(sz));
        return e;
    }
    if (name && !strcmp(name, "addr") && args && args->count >= 1) return pas_addr_of(args->items[0]);
    long long _fw = 1;
    if (name && args && args->count >= 5 && !strncmp(name, "fill", 4) && strchr("bwdq", name[4]) && !strcmp(name + 5, name[4] == 'b' ? "yte" : name[4] == 'w' ? "ord" : "word") && !pas_is_func(name) &&
        !pas_is_proc(name)) {
        long long w = name[4] == 'b' ? 1 : name[4] == 'w' ? 2 : name[4] == 'd' ? 4 : 8;
        tree_t *c = args->items[2];
        args->items[2] = (c && c->t == TT_ILIT) ? ilit(c->v.ival * w) : bin(TT_MUL, c, ilit(w));
        _fw = w;
        name = "fillchar";
    }
    if (name && !strcmp(name, "fillchar") && args && args->count >= 5 && args->items[0] && args->items[0]->t == TT_VAR && args->items[0]->v.sval && args->items[2] && args->items[2]->t == TT_ILIT) {
        long long _lz = -1;
        const pty_t *_lt = pty_name_type(args->items[0]->v.sval);
        long long _tz = pty_sizeof_name(args->items[0]->v.sval);
        if (_tz < 0 && _lt && _lt->name && !pas_sizeof_lookup(_lt->name, &_tz)) _tz = -1;
        if (!pas_sizeof_lookup(args->items[0]->v.sval, &_lz) && _tz > 0 && _tz == args->items[2]->v.ival) {
            tree_t *z = ast_node_new(TT_FNC);
            ast_push(z, leaf_s(TT_VAR, "sizeof"));
            ast_push(z, leaf_s(TT_VAR, args->items[0]->v.sval));
            args->items[2] = z;
        }
    }
    if (name && !strcmp(name, "fillchar") && args && args->count >= 5) {
        tree_t *dst = args->items[0];
        tree_t *val = args->items[4];
        tree_t *cnt = args->items[2];
        long long _wsz = -1;
        if (dst && dst->t == TT_VAR && dst->v.sval && cnt &&
            ((cnt->t == TT_FNC && cnt->n == 2 && cnt->c[0] && cnt->c[0]->v.sval && !strcmp(cnt->c[0]->v.sval, "sizeof") && cnt->c[1] && cnt->c[1]->t == TT_VAR && cnt->c[1]->v.sval &&
            !strcmp(cnt->c[1]->v.sval, dst->v.sval)) || (cnt->t == TT_ILIT && pas_sizeof_lookup(dst->v.sval, &_wsz) && _wsz == cnt->v.ival))) return mk_fnc2("__pas_fill", dst, val);
        if (dst && dst->t == TT_IDX && dst->n == 2 && dst->c[0] && dst->c[0]->t == TT_VAR && dst->c[0]->v.sval && cnt) {
            const char *an = dst->c[0]->v.sval;
            long long ah = -1, tot = -1, esz = 0;
            if (pas_var_string_kind(an)) {
                tree_t *f = ast_node_new(TT_FNC);
                ast_push(f, leaf_s(TT_VAR, "__pas_strfill"));
                ast_push(f, leaf_s(TT_VAR, an));
                ast_push(f, dst->c[1]);
                ast_push(f, cnt);
                ast_push(f, val);
                return mk_assign(leaf_s(TT_VAR, an), f);
            }
            const pty_t *at = pty_name_type(an);
            if (pas_array_isdyn(an) || pas_array_high_get(an, &ah) || (at && at->k == PTK_ARRAY && at->nix <= 1)) {
                esz = pas_elem_size(an);
                if (esz <= 0 && at && at->k == PTK_ARRAY && at->base && at->base->name && !pas_sizeof_lookup(at->base->name, &esz)) esz = 0;
                tree_t *f = ast_node_new(TT_FNC);
                ast_push(f, leaf_s(TT_VAR, "__pas_fill_from"));
                ast_push(f, leaf_s(TT_VAR, an));
                ast_push(f, dst->c[1]);
                ast_push(f, cnt);
                ast_push(f, val);
                ast_push(f, ilit(esz > 0 ? esz : 0));
                return f;
            }
        }
        long long fhi = -1, flo = 0;
        tree_t *fdst = NULL;
        if (dst && dst->t == TT_VAR && dst->v.sval && pas_array_high_get(dst->v.sval, &fhi)) {
            flo = pas_array_low(dst->v.sval);
            fdst = leaf_s(TT_VAR, dst->v.sval);
        } else if (dst && dst->t == TT_FNC && dst->n >= 2 && dst->c[0] && dst->c[0]->v.sval && !strcmp(dst->c[0]->v.sval, "__pas_deref")) {
            const char *ptn = pas_ptrexpr_target(dst->c[1]);
            long long phi = ptn ? pas_arrtype_high(ptn) : -1;
            if (phi >= 0) { fhi = phi; flo = pas_arrtype_lo(ptn); fdst = mk_deref(pas_tree_clone(dst->c[1])); }
        }
        if (fdst) {
            char _fcb[24];
            snprintf(_fcb, sizeof _fcb, "__pas_fc%d", g_lower_pas.sem.fcn++);
            const char *_fcv = ct_strdup(_fcb);
            tree_t *fidx = ast_node_new(TT_IDX);
            ast_push(fidx, fdst);
            ast_push(fidx, leaf_s(TT_VAR, _fcv));
            tree_t *fbody = mk_assign(fidx, val);
            tree_t *floop = ast_node_new(TT_FOR);
            ast_push(floop, leaf_s(TT_VAR, _fcv));
            ast_push(floop, ilit(flo));
            ast_push(floop, ilit(fhi));
            ast_push(floop, fbody);
            return floop;
        }
        long long _pw = dst ? pty_sizeof_low(dst) : -1;
        const pty_t *_et = dst ? pty_of_low(dst) : NULL;
        if (_pw <= 0 && _et && (_et->k == PTK_CHAR || _et->k == PTK_BOOL) && !(_et->name && pas_sizeof_lookup(_et->name, &_pw))) _pw = 1;
        if (_pw <= 0 && _et && (_et->k == PTK_INT || _et->k == PTK_ENUM) && !(_et->name && pas_sizeof_lookup(_et->name, &_pw))) _pw = pas_range_fit_size(_et->lo, _et->hi);
        tree_t *_n = NULL;
        if (cnt && _pw > 0) _n = cnt->t == TT_ILIT ? ilit(cnt->v.ival / _pw) : _pw == 1 ? cnt : (cnt->t == TT_MUL && cnt->n == 2 && cnt->c[1] && cnt->c[1]->t == TT_ILIT && cnt->c[1]->v.ival == _pw) ?
            cnt->c[0] : NULL;
        tree_t *_pa = (dst && _n && val && _et && (_pw == _fw || (val->t == TT_ILIT && val->v.ival == 0))) ? pas_addr_of_cell(pas_tree_clone(dst)) : NULL;
        if (_pa && (_et->k == PTK_INT || _et->k == PTK_CHAR || _et->k == PTK_BOOL || _et->k == PTK_ENUM)) {
            const pty_t *_vt = pty_of_low(val);
            tree_t *_v = val;
            if (_et->k == PTK_CHAR && !(_vt && _vt->k == PTK_CHAR)) _v = mk_chrlit(val);
            else if (_et->k != PTK_CHAR && _vt && _vt->k == PTK_CHAR) _v = bin(TT_ADD, val, ilit(0));
            const char *_fcv = ct_fmt("__pas_fc%d", g_lower_pas.sem.fcn++);
            tree_t *_loop = ast_node_new(TT_FOR);
            ast_push(_loop, leaf_s(TT_VAR, _fcv));
            ast_push(_loop, ilit(0));
            ast_push(_loop, _n->t == TT_ILIT ? ilit(_n->v.ival - 1) : bin(TT_SUB, _n, ilit(1)));
            ast_push(_loop, mk_fnc2("__pas_deref_set", mk_fnc2("__pas_ptr_inc", _pa, leaf_s(TT_VAR, _fcv)), _v));
            return _loop;
        }
    }
    if (name && !strcmp(name, "trunc") && args && args->count >= 1) return mk_fnc2("__pas_trunc", args->items[0], ilit(g_lower_pas.sem.pas_dialect_fpc ? 2 : 1));
    if (name && !strcmp(name, "int") && args && args->count >= 1) return mk_fnc1("__pas_trunc", args->items[0]);
    if (name && !strcmp(name, "round") && args && args->count >= 1) return mk_fnc1(g_lower_pas.sem.pas_dialect_fpc ? "__pas_rounde" : "__pas_round", args->items[0]);
    if (name && !strcmp(name, "halt") && (!args || args->count == 0)) return mk_fnc0("__pas_halt");
    if (name && !strcmp(name, "halt") && args && args->count >= 1) return mk_fnc1("__pas_halt", args->items[0]);
    if (name && !strcmp(name, "frac") && args && args->count >= 1) return mk_fnc1("__pas_frac", args->items[0]);
    if (name && !strcmp(name, "assert") && args && args->count >= 1) return mk_fnc1("__pas_assert", args->items[0]);
    if (name && !strcmp(name, "mkdir") && args && args->count >= 1) return mk_fnc1("__pas_mkdir", args->items[0]);
    if (name && !strcmp(name, "rmdir") && args && args->count >= 1) return mk_fnc1("__pas_rmdir", args->items[0]);
    if (name && !strcmp(name, "upcase") && !pas_is_func(name) && args && args->count >= 1) return mk_fnc1("__pas_upcase", args->items[0]);
    if (name && !strcmp(name, "paramstr") && !pas_is_func(name) && args && args->count >= 1) return mk_fnc1("__pas_paramstr", args->items[0]);
    if (name && !strcmp(name, "paramcount") && !pas_is_func(name)) return mk_fnc0("__pas_paramcount");
    if (name && !strcmp(name, "copy") && !pas_is_func(name) && args && args->count >= 3) {
        tree_t *e = ast_node_new(TT_FNC);
        ast_push(e, leaf_s(TT_VAR, "__pas_copy"));
        ast_push(e, pas_concat_operand(args->items[0]));
        ast_push(e, args->items[2]);
        ast_push(e, args->count >= 6 ? args->items[4] : ilit(2147483647));
        return e;
    }
    if (name && !strcmp(name, "concat") && !pas_is_func(name) && args && args->count >= 1) {
        tree_t *e = pas_concat_operand(args->items[0]);
        for (int i = 2; i < args->count; i += 2) e = bin(TT_CAT, e, pas_concat_operand(args->items[i]));
        return args->count >= 3 ? e : bin(TT_CAT, e, leaf_s(TT_QLIT, ""));
    }
    if (name && !strcmp(name, "pos") && args && args->count >= 3) {
        tree_t *needle = args->items[0];
        if (pas_is_charexpr(needle)) needle = mk_chr_wrap(needle);
        tree_t *e = ast_node_new(TT_FNC);
        ast_push(e, leaf_s(TT_VAR, "__pas_pos"));
        ast_push(e, needle);
        ast_push(e, args->items[2]);
        return e;
    }
    if (name && !strcmp(name, "abs") && args && args->count >= 1) return mk_fnc1("__pas_abs", args->items[0]);
    if (name && !strcmp(name, "sin") && args && args->count >= 1) return mk_fnc1("__pas_sin", args->items[0]);
    if (name && !strcmp(name, "cos") && args && args->count >= 1) return mk_fnc1("__pas_cos", args->items[0]);
    if (name && !strcmp(name, "exp") && args && args->count >= 1) return mk_fnc1("__pas_exp", args->items[0]);
    if (name && !strcmp(name, "sqrt") && args && args->count >= 1) return mk_fnc1("__pas_sqrt", args->items[0]);
    if (name && !strcmp(name, "ln") && args && args->count >= 1) return mk_fnc1("__pas_ln", args->items[0]);
    if (name && !strcmp(name, "arctan") && args && args->count >= 1) return mk_fnc1("__pas_arctan", args->items[0]);
    if (name && args && args->count >= 1 && !pas_is_func(name) && !pas_is_proc(name)) {
        static const char *const m1[] = { "tan", "arcsin", "arccos", "sinh", "cosh", "tanh", "log2", "cot", "degtorad", "radtodeg", "arcsinh", "arccosh", "arctanh" };
        static const char *const m2[] = { "arctan2", "hypot", "logn" };
        for (int k = 0; k < (int)(sizeof m1 / sizeof m1[0]); k++) if (!strcmp(name, m1[k])) {
            tree_t *f = ast_node_new(TT_FNC);
            ast_push(f, leaf_s(TT_VAR, "__pas_mathf"));
            ast_push(f, ilit(k + 1));
            ast_push(f, bin(TT_ADD, args->items[0], flit(0.0)));
            return f;
        }
        for (int k = 0; k < (int)(sizeof m2 / sizeof m2[0]); k++) if (!strcmp(name, m2[k]) && args->count >= 3) {
            tree_t *f = ast_node_new(TT_FNC);
            ast_push(f, leaf_s(TT_VAR, "__pas_mathf"));
            ast_push(f, ilit(101 + k));
            ast_push(f, bin(TT_ADD, args->items[0], flit(0.0)));
            ast_push(f, bin(TT_ADD, args->items[2], flit(0.0)));
            return f;
        }
    }
    if (name && !strcmp(name, "odd") && args && args->count >= 1) return bin(TT_NE, bin(TT_MOD, args->items[0], ilit(2)), ilit(0));
    if (name && (!strcmp(name, "lo") || !strcmp(name, "hi")) && args && args->count == 2 && !pas_is_func(name) && !pas_is_proc(name)) {
        int b = pas_lohi_bits(args->items[0]);
        tree_t *x = name[0] == 'h' ? mk_fnc2("ishift", args->items[0], ilit(-b)) : args->items[0];
        return mk_fnc2("iand", x, ilit(b == 32 ? 4294967295LL : (1LL << b) - 1));
    }
    if (name && args && args->count >= 1 && !pas_is_func(name) && !pas_is_proc(name) && pas_enumtype_high(name) >= 0) return args->items[0];
    if (name && !strcmp(name, "eof") && (!args || args->count == 0)) return mk_fnc0("__pas_eof");
    if (name && !strcmp(name, "eoln") && (!args || args->count == 0)) return mk_fnc0("__pas_eoln");
    if (name && (!strcmp(name, "eof") || !strcmp(name, "eoln")) && args && args->count >= 1) {
        tree_t *fa = args->items[0];
        int isf = (fa && fa->t == TT_VAR && fa->v.sval && pas_is_filevar(fa->v.sval)) || pas_is_filevar_elem(fa);
        int isstd = fa && fa->t == TT_VAR && fa->v.sval && pas_is_stdstream(fa->v.sval);
        const char *base = !strcmp(name, "eof") ? "__pas_eof" : "__pas_eoln";
        if (isf && !isstd) { const char *fn2 = !strcmp(name, "eof") ? "__pas_eof_f" : "__pas_eoln_f"; return mk_fnc1(fn2, pas_tree_clone(fa)); }
        return mk_fnc0(base);
    }
    if (name && (!strcmp(name, "GetBufCh") || !strcmp(name, "getbufch")) && args && args->count >= 1) {
        tree_t *fa = args->items[0];
        int isstd = fa && fa->t == TT_VAR && fa->v.sval && pas_is_stdstream(fa->v.sval);
        if (fa && fa->t == TT_VAR && fa->v.sval && !isstd) return mk_fnc1("__pas_getbufch_f", pas_tree_clone(fa));
        return mk_fnc0("__pas_getbufch");
    }
    if (name && !strcmp(name, "delete") && args && args->count >= 6) {
        tree_t *sv = args->items[0];
        tree_t *pos = args->items[2];
        tree_t *cnt = args->items[4];
        return mk_assign(sv, mk_fnc3("__pas_str_delete", pas_tree_clone(sv), pos, cnt));
    }
    if (name && !strcmp(name, "insert") && args && args->count >= 6) {
        tree_t *src = args->items[0];
        tree_t *sv = args->items[2];
        tree_t *pos = args->items[4];
        if (src && src->t == TT_FNC && src->n == 2 && src->c[0] && src->c[0]->v.sval && !strcmp(src->c[0]->v.sval, "__pas_chrlit") && src->c[1] && src->c[1]->t == TT_ILIT && src->c[1]->v.ival != 0) {
            char _cb[2];
            _cb[0] = (char)src->c[1]->v.ival;
            _cb[1] = '\0';
            src = leaf_s(TT_QLIT, ct_strdup(_cb));
        }
        return mk_assign(sv, mk_fnc3("__pas_str_insert", src, pas_tree_clone(sv), pos));
    }
    if (name && !strcmp(name, "val") && !pas_is_func(name) && !pas_is_proc(name) && args && args->count >= 5) {
        tree_t *sv = args->items[0], *vv = args->items[2], *cv = args->items[4];
        tree_t *rl = ilit(pty_answer(vv, PTK_REAL) > 0);
        PNodeList *vl = pnl_new();
        pnl_push(vl, mk_assign(pas_tree_clone(cv), mk_fnc2("__pas_valcode", pas_tree_clone(sv), pas_tree_clone(rl))));
        pnl_push(vl, mk_assign(vv, mk_fnc2("__pas_valval", sv, rl)));
        return seq_of(vl);
    }
    if (name && !strcmp(name, "readstr") && args && args->count >= 4) {
        tree_t *sv = args->items[0];
        tree_t *v = args->items[2];
        tree_t *rhs = mk_fnc1("__pas_str_to_int", sv);
        if (g_lower_pas.sem.pas_range_check_on && v && v->t == TT_VAR && v->v.sval) { long long _lo, _hi; if (pas_subvar_get(v->v.sval, &_lo, &_hi)) rhs = pas_range_wrap(rhs, _lo, _hi, NULL); }
        return mk_assign(v, rhs);
    }
    if (name && !strcmp(name, "readln") && (!args || args->count == 0)) return mk_fnc0("__pas_readln");
    if (name && (!strcmp(name, "readln") || !strcmp(name, "read")) && args && args->count >= 1) {
        int isln = !strcmp(name, "readln");
        int start = 0;
        tree_t *fstream = NULL;
        tree_t *fa = args->items[0];
        if (pas_is_filevar_elem(fa)) {
            fstream = fa;
            start = 2;
        } else if (fa && fa->t == TT_VAR && fa->v.sval && pas_is_filevar(fa->v.sval)) {
            if (!pas_is_stdstream(fa->v.sval)) fstream = fa;
            start = 2;
        }
        PNodeList *stmts = pnl_new();
        for (int i = start; i + 1 < args->count; i += 2) {
            tree_t *v = pas_vt_unwrap_read(args->items[i]);
            if (v && v->t == TT_VAR && v->v.sval && pas_is_chararr(v->v.sval)) {
                long long rlo = pas_array_low(v->v.sval);
                long long rhi = -1;
                pas_array_high_get(v->v.sval, &rhi);
                char _rdcb[24];
                snprintf(_rdcb, sizeof _rdcb, "__pas_rdi%d", g_lower_pas.sem.rdcn++);
                const char *_rdcv = ct_strdup(_rdcb);
                tree_t *ridx = ast_node_new(TT_IDX);
                ast_push(ridx, leaf_s(TT_VAR, v->v.sval));
                ast_push(ridx, leaf_s(TT_VAR, _rdcv));
                tree_t *rval = fstream ? mk_fnc1("__pas_read_c_f", pas_tree_clone(fstream)) : mk_fnc0("__pas_read_c");
                tree_t *rloop = ast_node_new(TT_FOR);
                ast_push(rloop, leaf_s(TT_VAR, _rdcv));
                ast_push(rloop, ilit(rlo));
                ast_push(rloop, ilit(rhi));
                ast_push(rloop, mk_assign(ridx, rval));
                pnl_push(stmts, rloop);
                continue;
            }
            int isc = v && ((v->t == TT_VAR && v->v.sval && pas_is_charvar(v->v.sval)) || pas_is_charexpr(v));
            if (!isc && v && v->t == TT_VAR && v->v.sval && pas_var_string_kind(v->v.sval)) {
                pnl_push(stmts, mk_assign(v, fstream ? mk_fnc1("__pas_read_s_f", pas_tree_clone(fstream)) : mk_fnc0("__pas_read_s")));
                continue;
            }
            int isr = !isc && v && v->t == TT_VAR && v->v.sval && pas_var_is_real(v->v.sval);
            tree_t *rv = isr ? (fstream ? mk_set_bin("__pas_read_i_f", pas_tree_clone(fstream), ilit(1)) : mk_fnc1("__pas_read_i", ilit(1))) : fstream ?
                mk_fnc1(isc ? "__pas_read_c_f" : "__pas_read_i_f", pas_tree_clone(fstream)) : mk_fnc0(isc ? "__pas_read_c" : "__pas_read_i");
            if (g_lower_pas.sem.pas_dialect_fpc && !isc) rv = fstream ? mk_fnc3("__pas_read_i_f", pas_tree_clone(fstream), ilit(isr), ilit(1)) : mk_fnc2("__pas_read_i", ilit(isr), ilit(1));
            {
                long long _lo, _hi;
                if (!isr && g_lower_pas.sem.pas_range_check_on && v && v->t == TT_VAR && v->v.sval && !pas_is_setvar(v->v.sval) && pas_subvar_get(v->v.sval, &_lo, &_hi)) rv =
                    pas_range_wrap(rv, _lo, _hi, "6.9.1");
            }
            pnl_push(stmts, mk_assign(v, rv));
        }
        if (isln) { if (fstream) pnl_push(stmts, mk_fnc1("__pas_readln_f", pas_tree_clone(fstream))); else pnl_push(stmts, mk_fnc0("__pas_readln")); }
        return seq_of(stmts);
    }
    if (name && (!strcmp(name, "ReadInt") || !strcmp(name, "readint")) && args && args->count >= 3) {
        tree_t *fa = args->items[0];
        tree_t *fstream = NULL;
        int start = 0;
        if (pas_is_filevar_elem(fa)) {
            fstream = fa;
            start = 2;
        } else if (fa && fa->t == TT_VAR && fa->v.sval && pas_is_filevar(fa->v.sval)) {
            if (!pas_is_stdstream(fa->v.sval)) fstream = fa;
            start = 2;
        }
        PNodeList *stmts = pnl_new();
        for (int i = start; i + 1 < args->count; i += 2) {
            tree_t *v = args->items[i];
            if (fstream) pnl_push(stmts, mk_assign(v, mk_fnc1("__pas_read_i_f", pas_tree_clone(fstream))));
            else pnl_push(stmts, mk_assign(v, mk_fnc0("__pas_read_i")));
        }
        return seq_of(stmts);
    }
    if (name && g_lower_pas.sem.pas_dialect_fpc && !pas_is_func(name) && !pas_is_proc(name) && args && args->count >= 3 && (!strcmp(name, "include") || !strcmp(name, "exclude"))) {
        PNodeList *el = pnl_new();
        pnl_push(el, args->items[2]);
        return mk_assign(args->items[0], mk_set_bin(name[0] == 'i' ? "__pas_setuni" : "__pas_setdif", pas_tree_clone(args->items[0]), mk_set_ctor(el)));
    }
    if (name && g_lower_pas.sem.pas_dialect_fpc && !pas_is_func(name) && !pas_is_proc(name) && !strcmp(name, "move") && args && args->count >= 6 && args->items[0] && args->items[2] &&
        args->items[0]->t == TT_IDX && args->items[0]->n == 2 && args->items[0]->c[0] && args->items[0]->c[0]->t != TT_VAR && pas_is_stringexpr(args->items[0]->c[0]) && args->items[2]->t == TT_IDX &&
        args->items[2]->n == 2 && args->items[2]->c[0] && args->items[2]->c[0]->t == TT_VAR && pas_is_stringexpr(args->items[2]->c[0])) {
        tree_t *f = ast_node_new(TT_FNC);
        ast_push(f, leaf_s(TT_VAR, "__pas_strmove"));
        ast_push(f, leaf_s(TT_VAR, args->items[2]->c[0]->v.sval));
        ast_push(f, args->items[2]->c[1]);
        ast_push(f, args->items[0]->c[0]);
        ast_push(f, args->items[0]->c[1]);
        ast_push(f, args->items[4]);
        return mk_assign(leaf_s(TT_VAR, args->items[2]->c[0]->v.sval), f);
    }
    if (name && g_lower_pas.sem.pas_dialect_fpc && !pas_is_func(name) && !pas_is_proc(name) && !strcmp(name, "move") && args && args->count >= 6 && args->items[0] && args->items[2] &&
        args->items[0]->t == TT_IDX && args->items[0]->n == 2 && args->items[0]->c[0] && args->items[0]->c[0]->t == TT_VAR && args->items[2]->t == TT_IDX && args->items[2]->n == 2 &&
        args->items[2]->c[0] && args->items[2]->c[0]->t == TT_VAR) {
        const char *sa = args->items[0]->c[0]->v.sval, *da = args->items[2]->c[0]->v.sval;
        tree_t *f = ast_node_new(TT_FNC);
        if (pas_var_string_kind(sa) && pas_var_string_kind(da)) {
            ast_push(f, leaf_s(TT_VAR, "__pas_strmove"));
            ast_push(f, leaf_s(TT_VAR, da));
            ast_push(f, args->items[2]->c[1]);
            ast_push(f, leaf_s(TT_VAR, sa));
            ast_push(f, args->items[0]->c[1]);
            ast_push(f, args->items[4]);
            return mk_assign(leaf_s(TT_VAR, da), f);
        }
        long long hi = 0;
        if ((pas_array_isdyn(sa) || pas_array_high_get(sa, &hi)) && (pas_array_isdyn(da) || pas_array_high_get(da, &hi))) {
            long long z = pas_elem_size(da);
            if (z <= 0) z = pas_elem_size(sa);
            ast_push(f, leaf_s(TT_VAR, "__pas_move"));
            ast_push(f, leaf_s(TT_VAR, da));
            ast_push(f, args->items[2]->c[1]);
            ast_push(f, leaf_s(TT_VAR, sa));
            ast_push(f, args->items[0]->c[1]);
            ast_push(f, args->items[4]);
            ast_push(f, ilit(z > 0 ? z : 0));
            return f;
        }
    }
    if (name && g_lower_pas.sem.pas_dialect_fpc && !pas_is_func(name) && !pas_is_proc(name) && !strcmp(name, "move") && args && args->count >= 6 && args->items[0] && args->items[2]) {
        tree_t *a[2] = { args->items[0], args->items[2] }, *base[2] = { NULL, NULL }, *idx[2] = { NULL, NULL };
        int nderef = 0, narr = 0;
        const char *arr = NULL;
        for (int k = 0; k < 2; k++) {
            tree_t *x = a[k];
            if (x->t == TT_FNC && x->n == 2 && x->c[0] && x->c[0]->v.sval && !strcmp(x->c[0]->v.sval, "__pas_deref") && x->c[1]) { base[k] = x->c[1]; idx[k] = ilit(0); nderef++; continue; }
            long long hi = 0;
            if (x->t == TT_IDX && x->n == 2 && x->c[0] && x->c[0]->t == TT_VAR && x->c[0]->v.sval && !pas_var_string_kind(x->c[0]->v.sval) &&
                (pas_array_isdyn(x->c[0]->v.sval) || pas_array_high_get(x->c[0]->v.sval, &hi))) {
                base[k] = x->c[0];
                idx[k] = x->c[1];
                arr = x->c[0]->v.sval;
                narr++;
            }
        }
        if (nderef == 2 && narr == 0) {
            const tree_t *_pe = base[0];
            if (_pe->t == TT_FNC && _pe->n > 1 && _pe->c[0] && _pe->c[0]->v.sval && !strcmp(_pe->c[0]->v.sval, "__pas_ptr_inc")) _pe = _pe->c[1];
            const pty_t *_mp = pty_of_low(_pe);
            long long z = _mp && _mp->k == PTK_POINTER && _mp->base ? pty_sizeof_flat(_mp->base) : 0;
            if (z > 0) {
                tree_t *f = ast_node_new(TT_FNC);
                ast_push(f, leaf_s(TT_VAR, "__pas_move"));
                ast_push(f, base[1]);
                ast_push(f, idx[1]);
                ast_push(f, base[0]);
                ast_push(f, idx[0]);
                ast_push(f, args->items[4]);
                ast_push(f, ilit(z));
                return f;
            }
        }
        if (nderef >= 1 && nderef + narr == 2 && arr) {
            long long z = pas_elem_size(arr);
            tree_t *f = ast_node_new(TT_FNC);
            ast_push(f, leaf_s(TT_VAR, "__pas_move"));
            ast_push(f, base[1]);
            ast_push(f, idx[1]);
            ast_push(f, base[0]);
            ast_push(f, idx[0]);
            ast_push(f, args->items[4]);
            ast_push(f, ilit(z > 0 ? z : 0));
            return f;
        }
    }
    if (name && g_lower_pas.sem.pas_dialect_fpc && !pas_is_func(name) && !pas_is_proc(name) && !strcmp(name, "move") && args && args->count >= 6 && args->items[0] && args->items[2] &&
        args->items[4] && pas_is_stringexpr(args->items[0]) && pas_is_stringexpr(args->items[2])) {
        tree_t *part = ast_node_new(TT_IF), *whole = ast_node_new(TT_IF), *err = mk_fnc1("__pas_rterr", leaf_s(TT_QLIT, "move"));
        ast_push(err, leaf_s(TT_QLIT, "move between two strings copies the whole string or nothing"));
        ast_push(part, bin(TT_NE, pas_tree_clone(args->items[4]), ilit(0)));
        ast_push(part, err);
        ast_push(whole, bin(TT_GE, pas_tree_clone(args->items[4]), bin(TT_ADD, mk_fnc1("length", pas_tree_clone(args->items[0])), ilit(1))));
        ast_push(whole, mk_assign(args->items[2], args->items[0]));
        ast_push(whole, part);
        return whole;
    }
    if (name && g_lower_pas.sem.pas_dialect_fpc && !pas_is_func(name) && !pas_is_proc(name) && !strcmp(name, "assignfile")) return lpt_mk_call("assign", args);
    if (name && g_lower_pas.sem.pas_dialect_fpc && !pas_is_func(name) && !pas_is_proc(name) && !strcmp(name, "closefile")) return lpt_mk_call("close", args);
    if (name && !strcmp(name, "assign") && args && args->count >= 3) { tree_t *fv = args->items[0]; tree_t *nm = args->items[2]; return mk_assign(fv, mk_fnc1("__pas_fassign", pas_alpha_wrap(nm))); }
    if (name && !strcmp(name, "rewrite") && args && args->count >= 1) {
        tree_t *fv = args->items[0];
        int _isorw = g_lower_pas.sem.pas_mode_iso || !g_lower_pas.sem.pas_seen_mode_directive;
        tree_t *dflt = (fv && fv->t == TT_VAR && fv->v.sval) ? ((_isorw && pas_is_hdrfile(fv->v.sval)) ? ilit(1 + 2 * pas_hdrfile_index(fv->v.sval)) : (_isorw ? NULL : leaf_s(TT_QLIT, fv->v.sval))) :
            NULL;
        return mk_assign(fv, dflt ? mk_set_bin("__pas_rewrite", pas_tree_clone(fv), dflt) : mk_fnc1("__pas_rewrite", pas_tree_clone(fv)));
    }
    if (name && !strcmp(name, "append") && args && args->count >= 1) {
        tree_t *fv = args->items[0];
        return mk_assign(fv, (fv && fv->t == TT_VAR && fv->v.sval) ? mk_set_bin("__pas_append", pas_tree_clone(fv), leaf_s(TT_QLIT, fv->v.sval)) : mk_fnc1("__pas_append", pas_tree_clone(fv)));
    }
    if (name && !strcmp(name, "reset") && args && args->count >= 1) {
        tree_t *fv = args->items[0];
        tree_t *dflt = (fv && fv->t == TT_VAR && fv->v.sval) ?
            (((g_lower_pas.sem.pas_mode_iso || !g_lower_pas.sem.pas_seen_mode_directive) && pas_is_hdrfile(fv->v.sval)) ? ilit(2 * pas_hdrfile_index(fv->v.sval)) : leaf_s(TT_QLIT, fv->v.sval)) : NULL;
        return mk_assign(fv, dflt ? mk_set_bin("__pas_reset", pas_tree_clone(fv), dflt) : mk_fnc1("__pas_reset", pas_tree_clone(fv)));
    }
    if (name && !strcmp(name, "close") && args && args->count >= 1) { return mk_fnc1("__pas_fclose", args->items[0]); }
    if (name && !strcmp(name, "erase") && args && args->count >= 1) { return mk_fnc1("__pas_ferase", args->items[0]); }
    if (name && !strcmp(name, "page") && !pas_is_proc(name) && !pas_is_func(name)) {
        PNodeList *pa = pnl_new();
        pnl_push(pa, mk_chrlit(ilit(12)));
        pnl_push(pa, ilit(-1));
        return mk_call_raw("write", pa);
    }
    if (name && !strcmp(name, "new") && args && args->count >= 1) {
        tree_t *pv0 = args->items[0], *pv = pas_vt_unwrap_read(pv0);
        tree_t *pvchk = (pv != pv0 && pv0 && pv0->t == TT_SEQ_EXPR && pv0->n >= 2) ? pv0->c[0] : NULL;
        const char *rt = pas_ptrexpr_target(pv);
        tree_t *alloc = ast_node_new(TT_FNC);
        if (rt) { ast_push(alloc, leaf_s(TT_VAR, "__pas_alloc_rec")); ast_push(alloc, pas_new_target_init(rt)); } else ast_push(alloc, leaf_s(TT_VAR, "__pas_alloc"));
        tree_t *as = mk_assign(pv, alloc);
        if (pvchk) { tree_t *qc = ast_node_new(TT_SEQ_EXPR); ast_push(qc, pvchk); ast_push(qc, as); as = qc; }
        if (args->count >= 4 && rt) {
            const pty_vt_t *vt = pas_vt_find_name(rt);
            if (vt && vt->tagfi >= 0) {
                tree_t *tg = ast_node_new(TT_IDX);
                ast_push(tg, mk_deref(pas_tree_clone(pv)));
                ast_push(tg, ilit(pas_rectype_slot(rt, vt->tagfi)));
                tree_t *q = ast_node_new(TT_SEQ_EXPR);
                ast_push(q, as);
                ast_push(q, mk_assign(tg, args->items[2]));
                ast_push(q, mk_fnc1("__pas_tagmark", pas_tree_clone(pv)));
                return q;
            }
        }
        return as;
    }
    if (name && !strcmp(name, "getmem") && !pas_is_proc(name) && !pas_is_func(name) && args && args->count >= 3) {
        tree_t *pv0 = args->items[0], *pv = pas_vt_unwrap_read(pv0);
        const char *rt = pas_ptrexpr_target(pv);
        tree_t *alloc = ast_node_new(TT_FNC);
        int open_arr = rt && pas_arrtype_high(rt) >= 0 && pas_arrtype_high(rt) == pas_arrtype_lo(rt) && pas_arrtype_ncols(rt) < 0 && !pas_arrtype_ischar(rt);
        if (rt && !open_arr) {
            ast_push(alloc, leaf_s(TT_VAR, "__pas_alloc_rec"));
            ast_push(alloc, pas_new_target_init(rt));
        } else {
            long long lo = open_arr ? pas_arrtype_lo(rt) : 0;
            ast_push(alloc, leaf_s(TT_VAR, "__pas_alloc_rec"));
            ast_push(alloc, mk_fnc2("arr_make", ilit(lo), bin(TT_SUB, bin(TT_ADD, args->items[2], ilit(lo)), ilit(1))));
        }
        tree_t *gm = ast_node_new(TT_IF);
        ast_push(gm, bin(TT_GE, pas_tree_clone(args->items[2]), ilit(0)));
        ast_push(gm, mk_assign(pv, alloc));
        ast_push(gm, mk_assign(pas_tree_clone(pv), mk_fnc1("__pas_heapfail", leaf_s(TT_VAR, "returnnilifgrowheapfails"))));
        return gm;
    }
    if (name && !strcmp(name, "freemem") && !pas_is_proc(name) && !pas_is_func(name) && args && args->count >= 1) return mk_fnc1("__pas_dispose", args->items[0]);
    if (name && !strcmp(name, "mark") && args && args->count >= 1) { tree_t *mk = ast_node_new(TT_FNC); ast_push(mk, leaf_s(TT_VAR, "__pas_mark")); return mk_assign(args->items[0], mk); }
    if (name && !strcmp(name, "release") && args && args->count >= 1) { return mk_fnc1("__pas_release", args->items[0]); }
    if (name && !strcmp(name, "dispose") && args && args->count >= 1) {
        if (pas_with_holds_pointee_of(args->items[0])) {
            fprintf(stderr, "pascal: ISO 7185 6.5.4 violation: dispose removes the identifying-value of a variable while a with-statement still references that variable\n");
            g_lower_pas.sem.pas_iso_errors++;
        }
        if (args->count >= 4) {
            const char *drt = pas_ptrexpr_target(args->items[0]);
            const pty_vt_t *vt = drt ? pas_vt_find_name(drt) : NULL;
            tree_t *tn = ilit(-1);
            if (vt && vt->tagfi >= 0) { tn = ast_node_new(TT_IDX); ast_push(tn, mk_deref(pas_tree_clone(args->items[0]))); ast_push(tn, ilit(pas_rectype_slot(drt, vt->tagfi))); }
            tree_t *dk = ast_node_new(TT_FNC);
            ast_push(dk, leaf_s(TT_VAR, "__pas_dispose_k"));
            ast_push(dk, args->items[0]);
            ast_push(dk, args->items[2]);
            ast_push(dk, tn);
            return dk;
        }
        return mk_fnc1("__pas_dispose", args->items[0]);
    }
    tree_t *e = ast_node_new(TT_FNC);
    int _wstart = 0;
    tree_t *_wstream = NULL;
    int _wstr = name && !strcmp(name, "writestr") && !pas_is_proc(name) && !pas_is_func(name) && args && args->count >= 2;
    if (_wstr) _wstart = 2;
    if (is_pas_io(map_io(name)) && args && args->count >= 2) {
        tree_t *fa = args->items[0];
        if (pas_is_filevar_elem(fa) || (fa && fa->t == TT_VAR && fa->v.sval && pas_is_filevar(fa->v.sval) && !pas_is_stdstream(fa->v.sval))) {
            _wstream = fa;
            _wstart = 2;
        } else if (fa && fa->t == TT_VAR && fa->v.sval && pas_is_stdstream(fa->v.sval)) {
            _wstart = 2;
        }
    }
    ast_push(e, leaf_s(TT_VAR, _wstr ? "__pas_writestr" : map_io(name)));
    if (_wstream) { ast_push(e, pas_tree_clone(_wstream)); ast_push(e, ilit(-9)); }
    if (args) {
        if (is_pas_io(map_io(name)) || _wstr) {
            int _fmt_cont = 0;
            for (int i = _wstart; i + 1 < args->count; i += 2) {
                tree_t *val = args->items[i];
                tree_t *wid = args->items[i + 1];
                int is_char = pas_is_charexpr(val), _uw = 0, _isstr = val && (val->t == TT_QLIT || (pty_of_low(val) && pty_of_low(val)->k == PTK_STRING));
                const char *_enm = (val && val->t == TT_IDX && val->v.ival > 0) ? pas_enumnames_by_idx((int)(val->v.ival - 1)) : NULL;
                if (!_enm && g_lower_pas.sem.pas_dialect_fpc && val && !is_char) _enm = pty_enum_names(val);
                if (_enm) {
                    tree_t *_w = ast_node_new(TT_FNC);
                    ast_push(_w, leaf_s(TT_VAR, "__pas_enum_name"));
                    ast_push(_w, val);
                    ast_push(_w, leaf_s(TT_QLIT, _enm));
                    val = _w;
                } else if (is_char) {
                    val = mk_chr_wrap(val);
                    if (wid->t == TT_ILIT && wid->v.ival == -1) wid = ilit(-2);
                } else if (pas_pchar_typed(val)) {
                    val = mk_fnc1("__pas_pstr", val);
                } else if (pas_is_boolexpr(val) || (val && val->t == TT_VAR && val->v.sval && pas_var_is_boolfam(val->v.sval))) {
                    tree_t *_w = ast_node_new(TT_FNC);
                    ast_push(_w, leaf_s(TT_VAR, "__pas_enum_name"));
                    ast_push(_w, val->t == TT_IDX ? val : bin(TT_NE, val, ilit(0)));
                    int _fpcb = g_lower_pas.sem.pas_seen_mode_directive && !g_lower_pas.sem.pas_mode_iso;
                    ast_push(_w, leaf_s(TT_QLIT, _fpcb ? "FALSE,TRUE" : "false,true"));
                    val = _w;
                    if (wid->t == TT_ILIT && wid->v.ival == -1) wid = ilit(_fpcb ? -4 : 5);
                } else if (pas_xr_low(val)) {
                    tree_t *_xs = ast_node_new(TT_FNC);
                    ast_push(_xs, leaf_s(TT_VAR, "__pas_xr_str"));
                    ast_push(_xs, ilit(0));
                    ast_push(_xs, val);
                    ast_push(_xs, wid->t == TT_ILIT && wid->v.ival < 0 ? ilit(-1) : wid);
                    ast_push(_xs, ilit(-1));
                    val = _xs;
                    wid = ilit(-1);
                } else if (val && val->t == TT_VAR && val->v.sval && pas_is_singlevar(val->v.sval)) {
                    if (wid->t == TT_ILIT && wid->v.ival == -1) wid = ilit(g_lower_pas.sem.pas_seen_mode_directive ? -5 : -12);
                } else if (pas_is_currencyexpr(val)) {
                    if (wid->t == TT_ILIT && wid->v.ival == -1) wid = ilit(-6);
                } else if (pas_is_qword(val) && val->t != TT_ILIT) {
                    if (wid->t == TT_ILIT && wid->v.ival == -1) wid = ilit(g_lower_pas.sem.pas_seen_mode_directive ? -8 : -7);
                    else _uw = 1;
                } else if (val && val->t == TT_VAR && val->v.sval && pas_is_chararr(val->v.sval)) {
                    val = pas_alpha_wrap(val);
                    _isstr = 1;
                } else if (pas_ca_is_read(val)) {
                    val = pas_alpha_wrap(val);
                    _isstr = 1;
                } else if (val && val->t == TT_IDX && val->n >= 2 && val->c[0] && val->c[0]->t == TT_VAR && val->c[0]->v.sval && pas_is_strarr(val->c[0]->v.sval)) {
                    tree_t *_w = ast_node_new(TT_FNC);
                    ast_push(_w, leaf_s(TT_VAR, "__pas_alpha_str"));
                    ast_push(_w, val);
                    ast_push(_w, ilit(pas_strarr_lo(val->c[0]->v.sval)));
                    val = _w;
                }
                if (g_lower_pas.sem.pas_seen_mode_directive && wid->t == TT_ILIT && wid->v.ival == -1) wid = ilit(-4);
                int _cont = _fmt_cont;
                _fmt_cont = wid->t == TT_ILIT && wid->v.ival == -3;
                if (_uw && !_cont) { ast_push(e, val); ast_push(e, ilit(-11)); ast_push(e, wid); ast_push(e, ilit(0)); continue; }
                if (!_cont && g_lower_pas.sem.pas_seen_mode_directive && !g_lower_pas.sem.pas_mode_iso && !(wid->t == TT_ILIT && wid->v.ival < 0)) {
                    ast_push(e, val);
                    ast_push(e, ilit(-10));
                    ast_push(e, wid);
                    ast_push(e, ilit(0));
                    continue;
                }
                if (_isstr && !_cont && !(g_lower_pas.sem.pas_seen_mode_directive && !g_lower_pas.sem.pas_mode_iso) && !(wid->t == TT_ILIT && wid->v.ival < 0)) {
                    ast_push(e, val);
                    ast_push(e, ilit(-13));
                    ast_push(e, wid);
                    ast_push(e, ilit(0));
                    continue;
                }
                ast_push(e, val);
                ast_push(e, wid);
            }
        } else {
            unsigned long long _cam = pas_caparm_mask(name);
            tree_t *_vpost[args->count / 2 + 1];
            tree_t *_vpre[args->count / 2 + 1];
            int _nvpost = 0, _nvpre = 0;
            for (int i = 0; i < args->count; i += 2) {
                tree_t *val = args->items[i];
                int _pidx = i / 2;
                if (val && (pas_is_proc(name) || pas_is_func(name)) && !pas_proc_param_is_var(name, _pidx)) {
                    const pty_t *_xf = pas_formal_type(name, _pidx), *_xa = pty_of_low(val);
                    int _fr = pty_real_rank(_xf), _ar = pty_real_rank(_xa);
                    if (_fr == 2 && IS(val, TT_FLIT)) val = pas_xr_wrap(val, val);
                    else if (_fr == 2 && _ar != 2 && (_ar >= 0 || (_xa && _xa->k == PTK_INT))) val = pas_xr_cvtn(2, val);
                    else if (_fr >= 0 && _fr < 2 && _ar == 2) val = pas_xr_cvtn(_fr == 0 ? 5 : 4, val);
                }
                {
                    long long _rd[4];
                    if (val && (pas_is_proc(name) || pas_is_func(name)) && !pas_proc_param_is_var(name, _pidx) && pas_row_designator(val, _rd)) val = mk_row_call("__pas_row_get", val, _rd, NULL);
                }
                if (val && val->t != TT_ILIT && (pas_is_proc(name) || pas_is_func(name)) && !pas_proc_param_is_var(name, _pidx)) {
                    const pty_t *_wf = pas_formal_type(name, _pidx);
                    const char *_wt = _wf ? pty_name_of(_wf) : NULL;
                    if (_wt) val = pas_wrap_to_tn(val, _wt, 0);
                }
                if (g_lower_pas.sem.pas_ptrcell && val && (pas_is_proc(name) || pas_is_func(name))) {
                    const pty_t *_ft = pas_formal_type(name, _pidx), *_at = pty_of_low(val);
                    int _cl = val->t == TT_FNC && val->n == 2 && val->c[0] && val->c[0]->v.sval && !strcmp(val->c[0]->v.sval, "__pas_chrlit") && val->c[1] && val->c[1]->t == TT_ILIT;
                    if (_ft && pty_is_pchar(_ft) && _cl) {
                        char _cb[2] = { (char) val->c[1]->v.ival, 0 };
                        val = mk_fnc1("__pas_pchar_of", leaf_s(TT_QLIT, ct_strdup(_cb)));
                    } else if (_ft && pty_is_pchar(_ft) && (val->t == TT_QLIT || (_at && (_at->k == PTK_STRING || _at->k == PTK_ARRAY)))) val = mk_fnc1("__pas_pchar_of", val);
                    else if (_ft && _ft->k == PTK_STRING && _at && pty_is_pchar(_at) && !pas_proc_param_is_var(name, _pidx)) val = mk_fnc1("__pas_pstr", val);
                }
                if (val && val->t == TT_SEQ_EXPR && val->n == 2 && val->c[0] && val->c[0]->t == TT_FNC && val->c[0]->c[0] && val->c[0]->c[0]->v.sval &&
                    !strcmp(val->c[0]->c[0]->v.sval, "__pas_tagwhole") && pas_proc_param_is_var(name, _pidx) && val->c[1] && val->c[1]->t == TT_FNC && val->c[1]->n == 2) {
                    tree_t *q = ast_node_new(TT_SEQ_EXPR);
                    ast_push(q, val->c[0]);
                    ast_push(q, val->c[1]->c[1]);
                    val->c[1]->c[1] = q;
                    val = val->c[1];
                }
                if (val && val->t == TT_SEQ_EXPR && val->n == 2 && val->c[0] && val->c[0]->t == TT_FNC && val->c[0]->n >= 1 && val->c[0]->c[0] && val->c[0]->c[0]->v.sval &&
                    !strcmp(val->c[0]->c[0]->v.sval, "__pas_vcheck") && pas_proc_param_is_var(name, _pidx)) {
                    if (pas_is_proc(name)) _vpost[_nvpost++] = pas_tree_clone(val->c[0]);
                    val = val->c[1];
                }
                if (val && val->t == TT_VAR && val->v.sval && pas_is_stdstream(val->v.sval) && !pas_ptrvar_target(val->v.sval) && pas_proc_param_is_var(name, _pidx)) val =
                    mk_fnc1("__pas_stdfile", ilit(!strcmp(val->v.sval, "input") ? 0 : 1));
                else if (val && (pas_is_proc(name) || pas_is_func(name)) && !pas_proc_param_is_var(name, _pidx) && pas_is_aggregate_designator(val)) val = mk_fnc1("__pas_arr_copy", val);
                if (val && val->t == TT_QLIT && val->v.sval && _pidx < 64 && ((_cam >> _pidx) & 1ULL)) {
                    long long _lo = pas_caparm_lo(name, _pidx);
                    long long _hi = _lo + (long long)strlen(val->v.sval) - 1;
                    if (g_lower_pas.sem.pas_dialect_fpc) {
                        const pty_t *_cf = pas_formal_type(name, _pidx);
                        const char *_cn = _cf ? pty_name_of(_cf) : NULL;
                        long long _th = _cn ? pas_arrtype_high(_cn) : -1;
                        if (_th > _hi) _hi = _th;
                    }
                    val = pas_str_to_alpha(val->v.sval, _lo, _hi);
                } else if (pas_ca_is_read(val) && _pidx < 64 && ((_cam >> _pidx) & 1ULL)) {
                    tree_t *_fld = val->n >= 2 ? val->c[1] : NULL;
                    tree_t *_en = ast_node_new(TT_FNC);
                    ast_push(_en, leaf_s(TT_VAR, "__pas_ca_encode"));
                    ast_push(_en, val);
                    ast_push(_en, ilit(pas_caparm_lo(name, _pidx)));
                    {
                        long long _fh = (_fld && pas_is_cafield(_fld)) ? pas_cafield_hi_get(_fld) : -1;
                        long long _fl = (_fld && pas_is_cafield(_fld)) ? pas_cafield_lo_get(_fld) : 0;
                        ast_push(_en, ilit(_fh >= _fl ? pas_caparm_lo(name, _pidx) + (_fh - _fl) : -1));
                    }
                    val = _en;
                    if (pas_is_proc(name) && pas_proc_param_is_var(name, _pidx) && _fld && pas_is_cafield(_fld)) {
                        char *_tn = (char *)ct_zalloc(32, 1);
                        snprintf(_tn, 32, "__pas_catmp_%d", ++g_lower_pas.sem.pas_catmp_seq);
                        pas_local_add(_tn);
                        _vpre[_nvpre++] = bin(TT_ASSIGN, leaf_s(TT_VAR, _tn), val);
                        tree_t *_pk = ast_node_new(TT_FNC);
                        ast_push(_pk, leaf_s(TT_VAR, "__pas_ca_pack"));
                        ast_push(_pk, leaf_s(TT_VAR, _tn));
                        ast_push(_pk, ilit(pas_cafield_lo_get(_fld)));
                        _vpost[_nvpost++] = mk_assign(pas_tree_clone(_fld), _pk);
                        val = leaf_s(TT_VAR, _tn);
                    }
                }
                if (val && ((_pidx < 64 && ((pas_openparm_mask(name) >> _pidx) & 1ULL)) || pas_formal_is_dynarr(name, _pidx)) && val->t == TT_FNC) {
                    tree_t *_al = ast_node_new(TT_FNC);
                    ast_push(_al, leaf_s(TT_VAR, "__pas_arrlit"));
                    if (pas_setcons_elems(val, _al)) { if (pas_formal_elem_is_string(name, _pidx)) for (int _k = 1; _k < _al->n; _k++) _al->c[_k] = pas_concat_operand(_al->c[_k]); val = _al; }
                }
                if (val && _pidx < 64 && ((pas_openparm_mask(name) >> _pidx) & 1ULL)) {
                    if (!pas_proc_param_is_var(name, _pidx)) val = mk_fnc1("__pas_open", val);
                    else if (!pas_is_dynexpr(val) && val->t == TT_VAR && val->v.sval && pas_is_proc(name)) {
                        char *_tn = (char *)ct_zalloc(32, 1);
                        snprintf(_tn, 32, "__pas_catmp_%d", ++g_lower_pas.sem.pas_catmp_seq);
                        pas_local_add(_tn);
                        _vpre[_nvpre++] = bin(TT_ASSIGN, leaf_s(TT_VAR, _tn), mk_fnc1("__pas_open", val));
                        _vpost[_nvpost++] = bin(TT_ASSIGN, pas_tree_clone(val), mk_fnc2("__pas_rebase", leaf_s(TT_VAR, _tn), ilit(pas_array_nonparam_low(val->v.sval))));
                        val = leaf_s(TT_VAR, _tn);
                    }
                }
                if (val && val->t != TT_FLIT && pas_proc_param_is_real(name, _pidx)) val = bin(TT_MUL, val, flit(1.0));
                if (val && val->t == TT_FNC && val->n == 2 && val->c[0] && val->c[0]->v.sval && !strcmp(val->c[0]->v.sval, "__pas_chrlit") && val->c[1] && val->c[1]->t == TT_ILIT &&
                    pas_proc_param_is_string(name, _pidx)) {
                    char _cb[2];
                    _cb[0] = (char)val->c[1]->v.ival;
                    _cb[1] = '\0';
                    val = leaf_s(TT_QLIT, ct_strdup(_cb));
                }
                ast_push(e, val);
            }
            if (_nvpost || _nvpre) {
                tree_t *q = ast_node_new(TT_SEQ_EXPR);
                for (int k = 0; k < _nvpre; k++) ast_push(q, _vpre[k]);
                ast_push(q, e);
                for (int k = 0; k < _nvpost; k++) ast_push(q, _vpost[k]);
                e = q;
            }
        }
    }
    if (_wstr) return mk_assign(args->items[0], e);
    if (is_pas_io(map_io(name))) return (!_wstream && g_lower_pas.sem.pas_crt_out && !g_lower_pas.sem.pas_crt_raw) ? pas_crt_route(e) : pas_split_writes(e);
    return e;
}
static int pas_tfcomp_range(const char *n, long long *lo, long long *hi) {
    if (!n) return 0;
    for (int i = g_lower_pas.sem.pas_ntfcomp - 1; i >= 0; i--)
        if (g_lower_pas.sem.pas_tfcomp[i].name && !strcmp(g_lower_pas.sem.pas_tfcomp[i].name, n) && g_lower_pas.sem.pas_tfcomp[i].high >= g_lower_pas.sem.pas_tfcomp[i].low) {
        *lo = g_lower_pas.sem.pas_tfcomp[i].low;
        *hi = g_lower_pas.sem.pas_tfcomp[i].high;
        return 1;
    }
    return 0;
}
