#include "runtime/lib/bn_head.h"
#include "runtime/core/core_error.h"
#include <sys/stat.h>
#include <math.h>
#include "runtime/rt/rt_arena.h"
#include "driver/driver_globals.h"
#include <errno.h>
#include <fcntl.h>
#include "runtime/pas_bi_names.h"
#include "runtime/lib/bn_03_sh.h"
#include "runtime/lib/bn_10_pas.h"
#include "runtime/lib/bn_11_pl.h"
#include "runtime/lib/bn_15_rk.h"
#include "runtime/lib/bn_17_pas.h"
#include "runtime/lib/bn_25_sh.h"
#include "runtime/lib/bn_37_pas.h"
#include "runtime/lib/bn_38_pas.h"
extern void *rt_ws_alloc_descr(size_t);
extern FILE *fh_get(int);
int pas_bi_chain_id(int id, const char *fn, DESCR_t *args, int nargs, DESCR_t *out, long *rq) {
    switch (id) {
        case PBI_sqr:
        goto L_pbi_1;
        case PBI_alloc:
        goto L_pbi_2;
        case PBI_alloc_rec:
        goto L_pbi_3;
        case PBI_dispose:
        goto L_pbi_10;
        case PBI_dispose_k:
        goto L_pbi_18;
        case PBI_tagmark:
        goto L_pbi_29;
        case PBI_tagset_chk:
        goto L_pbi_34;
        case PBI_tagwhole:
        goto L_pbi_40;
        case PBI_udiv:
        case PBI_umod:
        goto L_pbi_46;
        case PBI_s2wide:
        goto L_pbi_52;
        case PBI_wide2s:
        goto L_pbi_64;
        case PBI_heapfail:
        goto L_pbi_79;
        case PBI_qchk:
        goto L_pbi_80;
        case PBI_vcheck:
        goto L_pbi_81;
        case PBI_mark:
        goto L_pbi_88;
        case PBI_release:
        goto L_pbi_89;
        case PBI_alpha_str:
        goto L_pbi_90;
        case PBI_rdecode:
        goto L_pbi_91;
        case PBI_rpatch:
        goto L_pbi_92;
        case PBI_view:
        goto L_pbi_93;
        case PBI_random:
        goto L_pbi_94;
        case PBI_pow:
        goto L_pbi_100;
        case PBI_mathf:
        goto L_pbi_112;
        case PBI_randomize:
        goto L_pbi_171;
        case PBI_stringofchar:
        goto L_pbi_172;
        case PBI_sstr_fit:
        goto L_pbi_900;
        case PBI_glob_nth:
        goto L_pbi_901;
        case PBI_valcode:
        goto L_pbi_902;
        case PBI_valval:
        goto L_pbi_902;
        case PBI_addr:
        goto L_pbi_903;
        case PBI_addr_var:
        goto L_pbi_904;
        case PBI_ptr_inc:
        goto L_pbi_905;
        case PBI_ptr_eq:
        goto L_pbi_906;
        case PBI_ptr_diff:
        goto L_pbi_907;
        case PBI_pdx:
        goto L_pbi_911;
        case PBI_row_get:
        goto L_pbi_912;
        case PBI_ioflow:
        goto L_pbi_914;
        case PBI_ioresult:
        goto L_pbi_915;
        case PBI_ioerrmsg:
        goto L_pbi_916;
        case PBI_ioexit:
        goto L_pbi_917;
        case PBI_row_set:
        goto L_pbi_913;
        case PBI_pchar_of:
        goto L_pbi_908;
        case PBI_pstrlen:
        goto L_pbi_909;
        case PBI_pstr:
        goto L_pbi_910;
        case PBI_strf:
        goto L_pbi_182;
        case PBI_dynlen:
        goto L_pbi_196;
        case PBI_arrlit:
        goto L_pbi_197;
        case PBI_move:
        goto L_pbi_198;
        case PBI_strmove:
        goto L_pbi_215;
        case PBI_now:
        goto L_pbi_230;
        case PBI_ticks:
        goto L_pbi_239;
        case PBI_fileexists:
        goto L_pbi_245;
        case PBI_rounde:
        goto L_pbi_246;
        case PBI_len:
        goto L_pbi_252;
        case PBI_bound:
        goto L_pbi_258;
        case PBI_ustr:
        goto L_pbi_265;
        case PBI_fill_from:
        goto L_pbi_272;
        case PBI_strfill:
        goto L_pbi_278;
        case PBI_fill:
        goto L_pbi_288;
        case PBI_format:
        goto L_pbi_289;
        case PBI_open:
        goto L_pbi_299;
        case PBI_rebase:
        goto L_pbi_305;
        case PBI_dynset:
        goto L_pbi_314;
        case PBI_setlength:
        goto L_pbi_315;
        case PBI_strcmp:
        goto L_pbi_329;
        case PBI_ca_pack:
        goto L_pbi_342;
        case PBI_ca_unpack:
        goto L_pbi_343;
        case PBI_arr_copy:
        goto L_pbi_344;
        case PBI_arr_of:
        goto L_pbi_345;
        case PBI_stdfile:
        goto L_pbi_368;
        case PBI_ca_encode:
        goto L_pbi_369;
        case PBI_field_set:
        goto L_pbi_396;
        case PBI_field_idx_set:
        goto L_pbi_404;
        case PBI_field_idx_update:
        goto L_pbi_413;
        case PBI_deref:
        goto L_pbi_424;
        case PBI_deref_set:
        goto L_pbi_425;
        case PBI_fassign:
        goto L_pbi_426;
        case PBI_rewrite:
        goto L_pbi_427;
        case PBI_append:
        goto L_pbi_455;
        case PBI_reset:
        goto L_pbi_470;
        case PBI_fclose:
        goto L_pbi_493;
        case PBI_read_s:
        case PBI_read_s_f:
        goto L_pbi_500;
        case PBI_copy:
        goto L_pbi_513;
        case PBI_upcase:
        goto L_pbi_527;
        case PBI_paramcount:
        goto L_pbi_537;
        case PBI_paramstr:
        goto L_pbi_538;
        case PBI_std_mute:
        goto L_pbi_548;
        case PBI_ferase:
        goto L_pbi_549;
        case PBI_pchar_deref:
        goto L_pbi_563;
        case PBI_ord_check:
        goto L_pbi_564;
        case PBI_resundef:
        goto L_pbi_574;
        case PBI_rterr:
        goto L_pbi_575;
        case PBI_unhandled:
        goto L_pbi_576;
        case PBI_range_check:
        goto L_pbi_582;
        case PBI_str_to_int:
        goto L_pbi_603;
        case PBI_str_delete:
        goto L_pbi_604;
        case PBI_str_insert:
        goto L_pbi_622;
        case PBI_in:
        goto L_pbi_640;
        case PBI_tbuf_get:
        case PBI_tbuf_set:
        case PBI_tget:
        case PBI_tput:
        goto L_pbi_648;
        case PBI_fbuf_get:
        case PBI_fbuf_set:
        case PBI_feof_t:
        case PBI_fget:
        case PBI_fput:
        case PBI_frac:
        case PBI_fread:
        case PBI_fread_range:
        case PBI_fwrite:
        case PBI_fwrite_range:
        case PBI_treset:
        case PBI_trewrite:
        goto L_pbi_674;
        case PBI_setrange:
        goto L_pbi_775;
        case PBI_set:
        goto L_pbi_785;
        case PBI_setdif:
        case PBI_seteq:
        case PBI_setint:
        case PBI_setne:
        case PBI_setuni:
        case PBI_subset:
        case PBI_super:
        goto L_pbi_792;
        default:
        break;
    }
    L_pbi_1:
    ;
    if (id == PBI_sqr && nargs == 1) { *out = c_rt_pas_sqr(args, nargs); return 1; }
    L_pbi_2:
    ;
    if (id == PBI_alloc && nargs == 0) { long n = pas_heap_take(); DESCR_t *c = pas_heap_cell(n, 1); if (c) *c = INTVAL(0); *out = INTVAL(n); return 1; }
    L_pbi_3:
    ;
    if (id == PBI_alloc_rec && nargs == 1) { *out = pas_alloc_rec(args, nargs); return 1; }
    L_pbi_10:
    ;
    if (id == PBI_dispose && nargs == 1) { *out = pas_dispose(args, nargs); return 1; }
    L_pbi_18:
    ;
    if (id == PBI_dispose_k && nargs == 3) {
        if (!IS_INT_fn(args[0]) || args[0].i <= 0) pas_file_err("6.6.5.3", "dispose of a pointer that is nil or undefined, so it identifies no variable", NULL);
        long long n = args[0].i;
        int tagged = n < g_runtime.pas.pas_heap_cap && (g_runtime.pas.pas_heap_dead[n] & 2);
        if (!tagged) pas_file_err("6.6.5.3", "dispose(p, k1, ..., kn) names variants for a variable not created with new(p, c1, ..., cn)", NULL);
        if (IS_INT_fn(args[1]) && IS_INT_fn(args[2]) && pas_tag_ne(args[1].i, args[2].i))
            pas_file_err("6.6.5.3", "the variant constants of dispose are not those of the new that created the variable", NULL);
        pas_heap_give(n);
        *out = NULVCL;
        return 1;
    }
    L_pbi_29:
    ;
    if (id == PBI_tagmark && nargs == 1) { if (IS_INT_fn(args[0]) && args[0].i > 0 && args[0].i < g_runtime.pas.pas_heap_cap) g_runtime.pas.pas_heap_dead[args[0].i] |= 2; *out = NULVCL; return 1; }
    L_pbi_34:
    ;
    if (id == PBI_tagset_chk && nargs == 3) {
        if (IS_INT_fn(args[0]) && args[0].i > 0 && args[0].i < g_runtime.pas.pas_heap_cap && (g_runtime.pas.pas_heap_dead[args[0].i] & 2) && IS_INT_fn(args[1]) && IS_INT_fn(args[2]) &&
            pas_tag_ne(args[1].i, args[2].i)) pas_file_err("6.6.5.3", "the tag-field of a variable created with new(p, c1, ..., cn) is assigned a value that activates a different variant", NULL);
        *out = NULVCL;
        return 1;
    }
    L_pbi_40:
    ;
    if (id == PBI_tagwhole && nargs == 1) {
        if (IS_INT_fn(args[0]) && args[0].i > 0 && args[0].i < g_runtime.pas.pas_heap_cap && (g_runtime.pas.pas_heap_dead[args[0].i] & 2))
            pas_file_err("6.6.5.3", "a variable created with new(p, c1, ..., cn) is accessed as a whole by a factor, an assignment or an actual-parameter", NULL);
        *out = NULVCL;
        return 1;
    }
    L_pbi_46:
    ;
    if ((id == PBI_udiv || id == PBI_umod) && nargs == 2) {
        unsigned long long x = IS_INT_fn(args[0]) ? (unsigned long long)args[0].i : 0ULL, y = IS_INT_fn(args[1]) ? (unsigned long long)args[1].i : 0ULL;
        if (!y) pas_file_err("6.7.2.2", "the divisor of div or mod is zero", NULL);
        *out = INTVAL((long long)(fn[7] == 'd' ? x / y : x % y));
        return 1;
    }
    L_pbi_52:
    ;
    if (id == PBI_s2wide && nargs == 4) {
        const char *src = VARVAL_fn(args[0]);
        if (!src) src = "";
        if (args[1].v == DT_A && args[1].arr && IS_INT_fn(args[2]) && IS_INT_fn(args[3])) {
            ARBLK_t *b = (ARBLK_t *)args[1].arr;
            long k = (long)args[2].i, n = (long)args[3].i, l = (long)strlen(src), m = n > 0 ? (l < n - 1 ? l : n - 1) : 0;
            for (long i = 0; i < m; i++) if (k + i >= b->lo && k + i <= b->hi) b->data[k + i - b->lo] = INTVAL((long long)(unsigned char)src[i]);
            if (n > 0 && k + m >= b->lo && k + m <= b->hi) b->data[k + m - b->lo] = INTVAL(0);
        }
        *out = NULVCL;
        return 1;
    }
    L_pbi_64:
    ;
    if (id == PBI_wide2s && nargs == 2) {
        if (args[0].v == DT_A && args[0].arr && IS_INT_fn(args[1])) {
            ARBLK_t *b = (ARBLK_t *)args[0].arr;
            long k = (long)args[1].i;
            long l = 0;
            for (long i = k; i <= b->hi && i >= b->lo && IS_INT_fn(b->data[i - b->lo]) && b->data[i - b->lo].i != 0; i++) l++;
            char *o = (char *)rt_wsb_alloc((size_t)l + 1);
            for (long i = 0; i < l; i++) o[i] = (char)b->data[k + i - b->lo].i;
            o[l] = '\0';
            *out = (DESCR_t){ .v = DT_S, .s = o, .slen = (uint32_t)l };
            return 1;
        }
        *out = STRVAL((char *)"");
        return 1;
    }
    L_pbi_79:
    ;
    if (id == PBI_heapfail && nargs == 1) { if (IS_INT_fn(args[0]) && args[0].i) { *out = INTVAL(0); return 1; } fflush(NULL); fprintf(stderr, "Runtime error 204 at $0\n"); exit(204); }
    L_pbi_80:
    ;
    if (id == PBI_qchk && nargs == 1) { if (IS_INT_fn(args[0]) && args[0].i < 0) { fflush(NULL); fprintf(stderr, "Runtime error 215 at $0\n"); exit(215); } *out = args[0]; return 1; }
    L_pbi_81:
    ;
    if (id == PBI_vcheck && nargs == 2) {
        long long tv = IS_INT_fn(args[0]) ? args[0].i : -2, m = IS_INT_fn(args[1]) ? args[1].i : 0;
        if (tv == -1) tv = 1;
        if (tv >= 0 && tv <= 62 && !((m >> tv) & 1)) pas_file_err("6.5.3.3", "a component of a variant is accessed while the tag-field selects a different variant", NULL);
        *out = NULVCL;
        return 1;
    }
    L_pbi_88:
    ;
    if (id == PBI_mark && nargs == 0) { *out = INTVAL(g_runtime.pas.pas_heap_ctr + 1); return 1; }
    L_pbi_89:
    ;
    if (id == PBI_release && nargs == 1) { if (IS_INT_fn(args[0])) pas_heap_release(args[0].i); *out = NULVCL; return 1; }
    L_pbi_90:
    ;
    if (id == PBI_alpha_str && nargs == 2) { *out = pas_chars_of(args[0], IS_INT_fn(args[1]) ? (long)args[1].i : 0); return 1; }
    L_pbi_91:
    ;
    if (id == PBI_rdecode && nargs == 4) { *out = pas_rdecode(args[0], (long)pas_ord_of(args[1]), (long)pas_ord_of(args[2]), (long)pas_ord_of(args[3])); return 1; }
    L_pbi_92:
    ;
    if (id == PBI_rpatch && nargs == 5) { *out = pas_rpatch(args[0], (long)pas_ord_of(args[1]), (long)pas_ord_of(args[2]), (long)pas_ord_of(args[3]), args[4]); return 1; }
    L_pbi_93:
    ;
    if (id == PBI_view && nargs == 4) { *out = pas_view_make(args, nargs); return 1; }
    L_pbi_94:
    ;
    if (id == PBI_random && nargs <= 1) {
        if (nargs == 1 && IS_INT_fn(args[0])) {
            long long l = args[0].i;
            if (l >= -2147483648LL && l <= 2147483647LL) {
                if (l < 0) l++;
                *out = INTVAL((long long)(((unsigned long long)pas_mt_u32rand() * (long long)l) >> 32));
            } else {
                unsigned long long a = pas_mt_u32rand();
                a |= ((unsigned long long)pas_mt_u32rand() << 32) & 0x7FFFFFFFFFFFFFFFull;
                *out = INTVAL(l ? (long long)(a % (unsigned long long)l) : 0);
            }
        } else *out = REALVAL((double)pas_mt_u32rand() / 4294967296.0);
        return 1;
    }
    L_pbi_100:
    ;
    if (id == PBI_pow && nargs == 2) {
        if (IS_INT_fn(args[0]) && IS_INT_fn(args[1]) && args[1].i >= 0) {
            long long r = 1, b = args[0].i, e = args[1].i;
            while (e > 0) { if (e & 1) r *= b; b *= b; e >>= 1; }
            *out = INTVAL(r);
        } else {
            double x = IS_REAL_fn(args[0]) ? args[0].r : (IS_INT_fn(args[0]) ? (double)args[0].i : 0.0);
            double y = IS_REAL_fn(args[1]) ? args[1].r : (IS_INT_fn(args[1]) ? (double)args[1].i : 0.0);
            *out = REALVAL(pow(x, y));
        }
        return 1;
    }
    L_pbi_112:
    ;
    if (id == PBI_mathf && (nargs == 2 || nargs == 3) && IS_INT_fn(args[0])) {
        double x = IS_REAL_fn(args[1]) ? args[1].r : (IS_INT_fn(args[1]) ? (double)args[1].i : 0.0);
        double y = nargs == 3 ? (IS_REAL_fn(args[2]) ? args[2].r : (IS_INT_fn(args[2]) ? (double)args[2].i : 0.0)) : 0.0;
        double r = 0.0;
        switch ((int)args[0].i) {
            case 1:
            r = tan(x);
            break;
            case 2:
            r = asin(x);
            break;
            case 3:
            r = acos(x);
            break;
            case 4:
            r = sinh(x);
            break;
            case 5:
            r = cosh(x);
            break;
            case 6:
            r = tanh(x);
            break;
            case 7:
            r = log2(x);
            break;
            case 8:
            r = 1.0 / tan(x);
            break;
            case 9:
            r = x * (M_PI / 180.0);
            break;
            case 10:
            r = x * (180.0 / M_PI);
            break;
            case 11:
            r = asinh(x);
            break;
            case 12:
            r = acosh(x);
            break;
            case 13:
            r = atanh(x);
            break;
            case 101:
            r = atan2(x, y);
            break;
            case 102:
            r = hypot(x, y);
            break;
            case 103:
            r = log(y) / log(x);
            break;
            default:
            r = 0.0;
        }
        *out = REALVAL(r);
        return 1;
    }
    L_pbi_171:
    ;
    if (id == PBI_randomize && nargs == 0) { g_runtime.pasrnd.seed = (unsigned)time(NULL) ^ ((unsigned)getpid() << 16); *out = NULVCL; return 1; }
    L_pbi_172:
    ;
    if (id == PBI_stringofchar && nargs == 2) {
        const char *c = VARVAL_fn(args[0]);
        long long n = IS_INT_fn(args[1]) ? args[1].i : 0;
        if (n < 0) n = 0;
        char *o = rt_wsb_alloc((size_t)n + 1);
        memset(o, IS_INT_fn(args[0]) ? (int)(unsigned char)args[0].i : (c && c[0] ? c[0] : ' '), (size_t)n);
        o[n] = '\0';
        *out = STRVAL(o);
        return 1;
    }
    L_pbi_903:
    ;
    if (id == PBI_addr && nargs == 2) { *out = pas_addr(args, nargs); return 1; }
    L_pbi_904:
    ;
    if (id == PBI_addr_var && nargs == 1) { *out = pas_addr_var(args, nargs); return 1; }
    L_pbi_905:
    ;
    if (id == PBI_ptr_inc && nargs == 2) { *out = pas_ptr_inc(args, nargs); return 1; }
    L_pbi_906:
    ;
    if (id == PBI_ptr_eq && nargs == 2) { *out = pas_ptr_eq(args, nargs); return 1; }
    L_pbi_911:
    ;
    if (id == PBI_pdx && nargs == 4) { *out = pas_pdx(args, nargs); return 1; }
    L_pbi_914:
    ;
    if (id == PBI_ioflow && nargs == 1) { g_runtime.pasio.ioflow = 1; *out = INTVAL(0); return 1; }
    L_pbi_915:
    ;
    if (id == PBI_ioresult && nargs == 1) { *out = INTVAL(g_runtime.pasio.inoutres); g_runtime.pasio.inoutres = 0; return 1; }
    L_pbi_916:
    ;
    if (id == PBI_ioerrmsg && nargs == 1) { *out = STRVAL(rt_heap_strdup_c(pas_io_message(IS_INT_fn(args[0]) ? args[0].i : 0))); return 1; }
    L_pbi_917:
    ;
    if (id == PBI_ioexit && nargs == 1) { long c = IS_INT_fn(args[0]) ? (long) args[0].i : 0; fflush(NULL); fprintf(stderr, "Runtime error %ld at $0\n", c); exit((int) c); }
    L_pbi_912:
    ;
    if (id == PBI_row_get && nargs == 5) { *out = pas_row_get(args, nargs); return 1; }
    L_pbi_913:
    ;
    if (id == PBI_row_set && nargs == 6) { *out = pas_row_set(args, nargs); return 1; }
    L_pbi_907:
    ;
    if (id == PBI_ptr_diff && nargs == 2) { *out = pas_ptr_diff(args, nargs); return 1; }
    L_pbi_908:
    ;
    if (id == PBI_pchar_of && nargs == 1) { *out = pas_pchar_of(args, nargs); return 1; }
    L_pbi_909:
    ;
    if (id == PBI_pstrlen && nargs == 1) { *out = pas_pstrlen(args, nargs); return 1; }
    L_pbi_910:
    ;
    if (id == PBI_pstr && nargs == 1) { *out = pas_pstr(args, nargs); return 1; }
    L_pbi_902:
    ;
    if ((id == PBI_valcode || id == PBI_valval) && nargs == 2) {
        long long iv = 0;
        double dv = 0;
        int real = IS_INT_fn(args[1]) && args[1].i;
        int ep = pas_val_scan(IS_INT_fn(args[0]) ? "" : VARVAL_fn(args[0]), real, &iv, &dv);
        if (id == PBI_valcode) *out = INTVAL(ep);
        else if (real) *out = REALVAL(ep ? 0.0 : dv);
        else *out = INTVAL(ep ? 0 : iv);
        return 1;
    }
    L_pbi_901:
    ;
    if (id == PBI_glob_nth && nargs == 2) {
        const char *pat = IS_INT_fn(args[0]) ? NULL : VARVAL_fn(args[0]);
        long long k = IS_INT_fn(args[1]) ? args[1].i : 0;
        glob_t g;
        memset(&g, 0, sizeof g);
        const char *r = "";
        if (pat && k >= 0 && glob(pat, 0, NULL, &g) == 0 && (size_t)k < g.gl_pathc) { const char *sl = strrchr(g.gl_pathv[k], '/'); r = sl ? sl + 1 : g.gl_pathv[k]; }
        size_t rl = strlen(r);
        char *o = rt_wsb_alloc(rl + 1);
        memcpy(o, r, rl + 1);
        globfree(&g);
        *out = STRVAL(o);
        return 1;
    }
    L_pbi_900:
    ;
    if (id == PBI_sstr_fit && nargs == 2) {
        long long n = IS_INT_fn(args[1]) ? args[1].i : 0;
        const char *c = IS_INT_fn(args[0]) ? NULL : VARVAL_fn(args[0]);
        if (!c || n < 0 || (long long)pas_slen(args[0], c) <= n) { *out = args[0]; return 1; }
        char *o = rt_wsb_alloc((size_t)n + 1);
        memcpy(o, c, (size_t)n);
        o[n] = '\0';
        *out = BSTRVAL(o, n);
        return 1;
    }
    L_pbi_182:
    ;
    if (id == PBI_strf && nargs == 4) {
        long long w = IS_INT_fn(args[1]) ? args[1].i : -1, p = IS_INT_fn(args[2]) ? args[2].i : -1;
        size_t cap = (size_t)(w > 0 ? w : 0) + 96;
        char *b = rt_wsb_alloc(cap), *o = rt_wsb_alloc(cap);
        if (IS_REAL_fn(args[0]) && p >= 0) {
            char _fxb[(size_t)(w > 255 ? w : 255) + 64];
            pas_fmt_fixed_double(args[0].r, (int)(w < 0 ? 0 : w), (int)(p > 255 ? 255 : p), _fxb);
            snprintf(o, cap, "%s", _fxb);
        } else if (IS_REAL_fn(args[0])) {
            int prec = w < 0 ? 16 : (w - 8 < 1 ? 1 : (w - 8 > 16 ? 16 : (int)w - 8));
            pas_real_str_exp(args[0].r, b, (int)cap, prec, 3, IS_INT_fn(args[3]) && args[3].i);
            int sp = b[0] == '-' ? 0 : 1, len = (int)strlen(b), fw = w < 0 ? len + sp : (len + sp > w ? len + sp : (int)w);
            snprintf(o, cap, "%*s", fw, b);
        } else snprintf(o, cap, "%*lld", (int)(w < 0 ? 0 : w), IS_INT_fn(args[0]) ? (long long)args[0].i : 0LL);
        *out = STRVAL(o);
        return 1;
    }
    L_pbi_196:
    ;
    if (id == PBI_dynlen && nargs == 1) { ARBLK_t *b = (args[0].v == DT_A && args[0].arr) ? (ARBLK_t *) args[0].arr : NULL; *out = INTVAL(b ? (long long)b->hi - b->lo + 1 : 0); return 1; }
    L_pbi_197:
    ;
    if (id == PBI_arrlit) { DESCR_t d = pas_dyn_block(nargs); for (int k = 0; k < nargs; k++) ((ARBLK_t *) d.arr)->data[k] = args[k]; *out = d; return 1; }
    L_pbi_198:
    ;
    if (id == PBI_move && nargs == 6) {
        DESCR_t *dd = NULL, *sd = NULL;
        long long dk = 0, sk = 0, dn = -1, sn = -1;
        long long bytes = IS_INT_fn(args[4]) ? (long long)args[4].i : 0, esz = IS_INT_fn(args[5]) ? (long long)args[5].i : 0;
        long long n = esz > 0 ? bytes / esz : bytes;
        ARBLK_t *rdb = (args[0].v == DT_A && args[0].arr) ? (ARBLK_t *) args[0].arr : NULL, *rsb = (args[2].v == DT_A && args[2].arr) ? (ARBLK_t *) args[2].arr : NULL;
        if (rdb && rsb && rdb->ewidth && rsb->ewidth) {
            long long rdk = (IS_INT_fn(args[1]) ? (long long) args[1].i : rdb->lo) - rdb->lo, rsk = (IS_INT_fn(args[3]) ? (long long) args[3].i : rsb->lo) - rsb->lo;
            long long rdn = ((long long) rdb->hi - rdb->lo + 1 - rdk) * rdb->ewidth, rsn = ((long long) rsb->hi - rsb->lo + 1 - rsk) * rsb->ewidth, rn = bytes < rdn ? bytes : rdn;
            if (rn > rsn) rn = rsn;
            if (rdk >= 0 && rsk >= 0 && rn > 0) memmove((char *) rdb->data + rdk * rdb->ewidth, (char *) rsb->data + rsk * rsb->ewidth, (size_t) rn);
            *out = args[0];
            return 1;
        }
        if (rdb && rsb && (rdb->ewidth != 0) != (rsb->ewidth != 0)) {
            long long mdi = IS_INT_fn(args[1]) ? (long long) args[1].i : rdb->lo, msi = IS_INT_fn(args[3]) ? (long long) args[3].i : rsb->lo;
            for (long long t = 0; t < n; t++) {
                DESCR_t g[2] = { args[2], INTVAL(msi + t) };
                DESCR_t v = pas_arr_get(g, 2);
                DESCR_t st[3] = { args[0], INTVAL(mdi + t), v };
                if (v.v == DT_FAIL) break;
                pas_arr_set(st, 3);
            }
            *out = args[0];
            return 1;
        }
        if (pas_move_side(args[0], args[1], &dd, &dk, &dn) && pas_move_side(args[2], args[3], &sd, &sk, &sn)) {
            if (dn >= 0 && n > dn - dk) n = dn - dk;
            if (sn >= 0 && n > sn - sk) n = sn - sk;
            if (dd == sd && dk > sk) for (long long t = n - 1; t >= 0; t--) dd[dk + t] = pas_agg_copy(sd[sk + t]);
            else for (long long t = 0; t < n; t++) dd[dk + t] = pas_agg_copy(sd[sk + t]);
        }
        *out = args[0];
        return 1;
    }
    L_pbi_215:
    ;
    if (id == PBI_strmove && nargs == 5) {
        const char *d = VARVAL_fn(args[0]), *sv = VARVAL_fn(args[2]);
        if (!d) d = "";
        if (!sv) sv = "";
        long long dl = (long long)strlen(d), sl = (long long)strlen(sv), j = IS_INT_fn(args[1]) ? (long long)args[1].i : 1, i = IS_INT_fn(args[3]) ? (long long)args[3].i : 1;
        long long n = IS_INT_fn(args[4]) ? (long long)args[4].i : 0, total = dl > j - 1 + n ? dl : j - 1 + n;
        if (n < 0 || j < 1 || i < 1) { *out = STRVAL((char *)d); return 1; }
        char *o = rt_wsb_alloc((size_t)total + 1);
        memcpy(o, d, (size_t)dl);
        if (total > dl) memset(o + dl, ' ', (size_t)(total - dl));
        for (long long t = 0; t < n; t++) o[j - 1 + t] = (i - 1 + t < sl) ? sv[i - 1 + t] : ' ';
        o[total] = '\0';
        *out = STRVAL(o);
        return 1;
    }
    L_pbi_230:
    ;
    if (id == PBI_now && nargs == 0) {
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        time_t t = ts.tv_sec;
        struct tm lt;
        localtime_r(&t, &lt);
        *out = REALVAL(((double)t + (double)lt.tm_gmtoff) / 86400.0 + 25569.0 + (double)ts.tv_nsec / 1e9 / 86400.0);
        return 1;
    }
    L_pbi_239:
    ;
    if (id == PBI_ticks && nargs == 0) { struct timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts); *out = INTVAL((long long)ts.tv_sec * 1000LL + (long long)(ts.tv_nsec / 1000000L)); return 1; }
    L_pbi_245:
    ;
    if (id == PBI_fileexists && nargs == 1) { const char *sv = VARVAL_fn(args[0]); struct stat st; *out = INTVAL(sv && *sv && stat(sv, &st) == 0 && S_ISREG(st.st_mode) ? 1 : 0); return 1; }
    L_pbi_246:
    ;
    if (id == PBI_rounde && nargs == 1) {
        double d = IS_REAL_fn(args[0]) ? args[0].r : (double)(IS_INT_fn(args[0]) ? args[0].i : 0), e = rint(d);
        if (!(e > -9223372036854775809.0 && e < 9223372036854775808.0)) { fflush(NULL); fprintf(stderr, "Runtime error 207 at $0\n"); exit(207); }
        *out = INTVAL((long long)e);
        return 1;
    }
    L_pbi_252:
    ;
    if (id == PBI_len && nargs == 1) {
        if (args[0].v == DT_A && args[0].arr) { const ARBLK_t *b = (const ARBLK_t *) args[0].arr; *out = INTVAL((long long)b->hi - b->lo + 1); return 1; }
        const char *sv = VARVAL_fn(args[0]);
        *out = INTVAL(sv ? (long long)pas_slen(args[0], sv) : 0);
        return 1;
    }
    L_pbi_258:
    ;
    if (id == PBI_bound && nargs == 2) {
        int hi = IS_INT_fn(args[1]) && args[1].i != 0;
        if (args[0].v == DT_A && args[0].arr) { const ARBLK_t *b = (const ARBLK_t *) args[0].arr; *out = INTVAL(hi ? (long long)b->hi : (long long)b->lo); return 1; }
        const char *sv = VARVAL_fn(args[0]);
        *out = INTVAL(hi ? (sv ? (long long)strlen(sv) : 0) : 1);
        return 1;
    }
    L_pbi_265:
    ;
    if (id == PBI_ustr && nargs == 1) {
        int cap = 24;
        char *o = rt_wsb_alloc((size_t)cap);
        snprintf(o, (size_t)cap, "%llu", IS_INT_fn(args[0]) ? (unsigned long long)args[0].i : 0ULL);
        *out = STRVAL(o);
        return 1;
    }
    L_pbi_272:
    ;
    if (id == PBI_fill_from && nargs == 5) {
        pas_fill_from(args[0], IS_INT_fn(args[1]) ? (long long)args[1].i : 0, IS_INT_fn(args[2]) ? (long long)args[2].i : 0, IS_INT_fn(args[4]) ? (long long)args[4].i : 0,
            IS_INT_fn(args[3]) ? (long long)args[3].i : 0);
        *out = args[0];
        return 1;
    }
    L_pbi_278:
    ;
    if (id == PBI_strfill && nargs == 4) {
        const char *sv = VARVAL_fn(args[0]);
        if (!sv) sv = "";
        long long len = (long long)pas_slen(args[0], sv), idx = IS_INT_fn(args[1]) ? (long long)args[1].i : 1, cnt = IS_INT_fn(args[2]) ? (long long)args[2].i : 0;
        char *o = rt_wsb_alloc((size_t)len + 1);
        memcpy(o, sv, (size_t)len);
        o[len] = '\0';
        for (long long i = idx; i < idx + cnt && i <= len; i++) if (i >= 1) o[i - 1] = (char)(IS_INT_fn(args[3]) ? args[3].i : 0);
        *out = BSTRVAL(o, len);
        return 1;
    }
    L_pbi_288:
    ;
    if (id == PBI_fill && nargs == 2) { pas_fill(args[0], IS_INT_fn(args[1]) ? (long long)args[1].i : 0); *out = args[0]; return 1; }
    L_pbi_289:
    ;
    if (id == PBI_format && nargs >= 1) {
        const char *f = VARVAL_fn(args[0]);
        if (!f) f = "";
        int ab = nargs == 2 && args[1].v == DT_A && args[1].arr && !((ARBLK_t *) args[1].arr)->ewidth;
        int na = ab ? (int)(((ARBLK_t *) args[1].arr)->hi - ((ARBLK_t *) args[1].arr)->lo + 1) : nargs - 1;
        size_t n = pas_format(f, ab ? ((ARBLK_t *) args[1].arr)->data : args + 1, na, NULL, 0);
        char *o = rt_wsb_alloc(n + 1);
        pas_format(f, ab ? ((ARBLK_t *) args[1].arr)->data : args + 1, na, o, n);
        o[n] = '\0';
        *out = STRVAL(o);
        return 1;
    }
    L_pbi_299:
    ;
    if (id == PBI_open && nargs == 1) {
        DESCR_t c = pas_agg_copy(args[0]);
        if (c.v == DT_A && c.arr && ((ARBLK_t *) c.arr)->ndim == 1 && c.arr != args[0].arr) { ARBLK_t *b = (ARBLK_t *) c.arr; b->hi = b->hi - b->lo; b->lo = 0; }
        *out = c;
        return 1;
    }
    L_pbi_305:
    ;
    if (id == PBI_rebase && nargs == 2) {
        if (args[0].v == DT_A && args[0].arr && ((ARBLK_t *) args[0].arr)->ndim == 1 && IS_INT_fn(args[1])) {
            ARBLK_t *b = (ARBLK_t *) args[0].arr;
            b->hi = (int) (args[1].i + (b->hi - b->lo));
            b->lo = (int) args[1].i;
        }
        *out = args[0];
        return 1;
    }
    L_pbi_314:
    ;
    if (id == PBI_dynset && nargs >= 4) { *out = pas_dynset(args[0], args[1], IS_INT_fn(args[2]) ? (int)args[2].i : 1, args + 3, nargs - 3); return 1; }
    L_pbi_315:
    ;
    if (id == PBI_setlength && nargs == 2) {
        const char *s = VARVAL_fn(args[0]);
        if (!s) s = "";
        long oldlen = (long)pas_slen(args[0], s);
        long newlen = IS_INT_fn(args[1]) ? (long)args[1].i : 0;
        if (newlen < 0) newlen = 0;
        long copy = oldlen < newlen ? oldlen : newlen;
        char *o = rt_wsb_alloc((size_t)newlen + 1);
        if (copy > 0) memcpy(o, s, (size_t)copy);
        if (newlen > copy) memset(o + copy, ' ', (size_t)(newlen - copy));
        o[newlen] = '\0';
        *out = BSTRVAL(o, newlen);
        return 1;
    }
    L_pbi_329:
    ;
    if (id == PBI_strcmp && nargs == 2) {
        const char *sa = VARVAL_fn(args[0]);
        if (!sa) sa = "";
        const char *sb = VARVAL_fn(args[1]);
        if (!sb) sb = "";
        size_t la = (args[0].v == DT_S && args[0].slen != 0xFFFFFFFFu) ? (size_t)args[0].slen : strlen(sa);
        size_t lb = (args[1].v == DT_S && args[1].slen != 0xFFFFFFFFu) ? (size_t)args[1].slen : strlen(sb);
        size_t lm = la < lb ? la : lb;
        int c = lm ? memcmp(sa, sb, lm) : 0;
        if (c == 0) c = (la < lb) ? -1 : (la > lb) ? 1 : 0;
        *out = INTVAL(c < 0 ? -1 : c > 0 ? 1 : 0);
        return 1;
    }
    L_pbi_342:
    ;
    if (id == PBI_ca_pack && nargs >= 1) { *out = pas_chars_of(args[0], (nargs >= 2 && IS_INT_fn(args[1])) ? (long)args[1].i : 0); return 1; }
    L_pbi_343:
    ;
    if (id == PBI_ca_unpack && nargs >= 1) { *out = args[0]; return 1; }
    L_pbi_344:
    ;
    if (id == PBI_arr_copy && nargs == 1) { *out = pas_agg_copy(args[0]); return 1; }
    L_pbi_345:
    ;
    if (id == PBI_arr_of && nargs == 3) {
        long long lo = IS_INT_fn(args[0]) ? args[0].i : 0;
        long long hi = IS_INT_fn(args[1]) ? args[1].i : 0;
        long long n = hi - lo + 1;
        if (n < 1) n = 1;
        ARBLK_t *b = (ARBLK_t *) rt_gcheap_alloc(HB_ARR, sizeof(ARBLK_t));
        b->id = rt_agg_serial_list();
        b->dumpno = rt_sno_dumpno_next();
        b->lo = (int) lo;
        b->hi = (int) hi;
        b->ndim = 1;
        b->lo2 = 0;
        b->hi2 = 0;
        b->proto_bare = 0;
        b->data = (DESCR_t *) rt_ws_alloc_descr((size_t) n);
        for (long long k = 0; k < n; k++) b->data[k] = pas_agg_copy(args[2]);
        DESCR_t d = {0};
        d.v = DT_A;
        d.slen = 0;
        d.arr = b;
        *out = d;
        return 1;
    }
    L_pbi_368:
    ;
    if (id == PBI_stdfile && nargs == 1) {
        extern void fh_ensure_init(void);
        fh_ensure_init();
        *out = FHVAL(IS_INT_fn(args[0]) && args[0].i >= 1 ? (args[0].i >= 3 ? pas_stdout_fh() : (args[0].i >= 2 ? 2 : 1)) : 0);
        return 1;
    }
    L_pbi_369:
    ;
    if (id == PBI_ca_encode && nargs == 3) {
        if (args[0].v == DT_A && args[0].arr) { *out = pas_agg_copy(args[0]); return 1; }
        const char *sa = VARVAL_fn(args[0]);
        if (!sa) sa = "";
        long lo = IS_INT_fn(args[1]) ? (long)args[1].i : 0;
        long hi = IS_INT_fn(args[2]) ? (long)args[2].i : -1;
        long sl = (long)strlen(sa);
        if (hi < lo + sl - 1) hi = lo + sl - 1;
        if (hi < lo) hi = lo;
        ARBLK_t *b = (ARBLK_t *) rt_gcheap_alloc(HB_ARR, sizeof(ARBLK_t));
        b->id = rt_agg_serial_list();
        b->dumpno = rt_sno_dumpno_next();
        b->lo = (int) lo;
        b->hi = (int) hi;
        b->ndim = 1;
        b->lo2 = 0;
        b->hi2 = 0;
        b->proto_bare = 0;
        b->data = (DESCR_t *) rt_ws_alloc_descr((size_t) (hi - lo + 1));
        for (long k = lo; k <= hi; k++) b->data[k - lo] = INTVAL(k - lo < sl ? (unsigned char) sa[k - lo] : 0);
        DESCR_t d = {0};
        d.v = DT_A;
        d.slen = 0;
        d.arr = b;
        *out = d;
        return 1;
    }
    L_pbi_396:
    ;
    if (id == PBI_field_set && nargs == 3) { *out = pas_field_set(args, nargs); return 1; }
    L_pbi_404:
    ;
    if (id == PBI_field_idx_set && nargs == 4) {
        DESCR_t *hc = pas_heap_ref(args[0], fn);
        long fidx = IS_INT_fn(args[1]) ? args[1].i : 0;
        long eidx = IS_INT_fn(args[2]) ? args[2].i : 0;
        unsigned char ch = pas_ch_of(args[3]);
        if (hc->v == DT_A && hc->arr && eidx >= 1) { ARBLK_t *b = (ARBLK_t *) hc->arr; if (fidx >= b->lo && fidx <= b->hi) b->data[fidx - b->lo] = pas_str_setch(b->data[fidx - b->lo], eidx, ch); }
        *out = args[3];
        return 1;
    }
    L_pbi_413:
    ;
    if (id == PBI_field_idx_update && nargs == 4) {
        long fidx = IS_INT_fn(args[1]) ? args[1].i : 0;
        long eidx = IS_INT_fn(args[2]) ? args[2].i : 0;
        unsigned char ch = pas_ch_of(args[3]);
        if (args[0].v == DT_A && args[0].arr && eidx >= 1) {
            ARBLK_t *b = (ARBLK_t *) args[0].arr;
            if (fidx >= b->lo && fidx <= b->hi) b->data[fidx - b->lo] = pas_str_setch(b->data[fidx - b->lo], eidx, ch);
        }
        *out = args[0];
        return 1;
    }
    L_pbi_424:
    ;
    if (id == PBI_deref && nargs == 1) { *out = pas_deref(args, nargs); return 1; }
    L_pbi_425:
    ;
    if (id == PBI_deref_set && nargs == 2) { *out = pas_deref_set(args, nargs); return 1; }
    L_pbi_426:
    ;
    if (id == PBI_fassign && nargs == 1) { const char *s = VARVAL_fn(args[0]); if (!s) s = ""; *out = STRVAL(rt_heap_strdup_c(s)); return 1; }
    L_pbi_427:
    ;
    if (id == PBI_rewrite && (nargs == 1 || nargs == 2)) {
        extern int fh_alloc(FILE *);
        const char *stale_nm = NULL;
        if (IS_FH_fn(args[0])) {
            int oidx = (int)args[0].i;
            FILE *ofp = fh_get(oidx);
            if (ofp) { fflush(ofp); rewind(ofp); if (ftruncate(fileno(ofp), 0) == 0) { *out = args[0]; return 1; } }
            if (oidx >= 0 && oidx < FH_N) stale_nm = g_fh[oidx].name;
        }
        const char *nm = (stale_nm && stale_nm[0]) ? stale_nm : IS_STR_fn(args[0]) ? VARVAL_fn(args[0]) : NULL;
        long long hcode = (IS_INT_fn(args[0]) && args[0].i < 0) ? 2 * (-args[0].i) + 1 : (nargs == 2 && IS_INT_fn(args[1])) ? args[1].i : -1;
        if ((!nm || !nm[0]) && hcode >= 0) {
            extern const char *pas_main_arg(int);
            const char *pa = pas_main_arg((int)(hcode >> 1));
            if (pa) nm = pa;
            else { extern void fh_ensure_init(void); fh_ensure_init(); *out = FHVAL((hcode & 1) ? 1 : 0); return 1; }
        }
        if ((!nm || !nm[0]) && nargs == 2 && !IS_INT_fn(args[1])) nm = VARVAL_fn(args[1]);
        if (nm && nm[0] && (!strcmp(nm, "input") || !strcmp(nm, "output"))) nm = "";
        FILE *fp;
        if (nm && nm[0]) { fp = fopen(nm, "w+"); } else { char tmpl[] = "/tmp/scrip_pas_XXXXXX"; int fd = mkstemp(tmpl); if (fd >= 0) unlink(tmpl); fp = (fd >= 0) ? fdopen(fd, "w+") : (FILE *)0; }
        if (!fp && pas_io_fail(errno)) { *out = STRVAL(rt_heap_strdup_c(nm ? nm : "")); return 1; }
        if (!fp) { *out = FAILDESCR; return 1; }
        int idx = fh_alloc(fp);
        if (idx < 0) { fclose(fp); *out = FAILDESCR; return 1; }
        if (nm && nm[0]) g_fh[idx].name = rt_heap_strdup_c(nm);
        *out = FHVAL(idx);
        return 1;
    }
    L_pbi_455:
    ;
    if (id == PBI_append && (nargs == 1 || nargs == 2)) {
        extern int fh_alloc(FILE *);
        const char *stale_nm = NULL;
        if (IS_FH_fn(args[0])) { int oidx = (int)args[0].i; FILE *ofp = fh_get(oidx); if (ofp) { *out = args[0]; return 1; } if (oidx >= 0 && oidx < FH_N) stale_nm = g_fh[oidx].name; }
        const char *nm = (stale_nm && stale_nm[0]) ? stale_nm : VARVAL_fn(args[0]);
        if ((!nm || !nm[0]) && nargs == 2 && !IS_INT_fn(args[1])) nm = VARVAL_fn(args[1]);
        if (!nm || !nm[0]) { *out = FAILDESCR; return 1; }
        FILE *fp = fopen(nm, "a+");
        if (!fp && pas_io_fail(errno)) { *out = STRVAL(rt_heap_strdup_c(nm)); return 1; }
        if (!fp) { *out = FAILDESCR; return 1; }
        int idx = fh_alloc(fp);
        if (idx < 0) { fclose(fp); *out = FAILDESCR; return 1; }
        g_fh[idx].name = rt_heap_strdup_c(nm);
        *out = FHVAL(idx);
        return 1;
    }
    L_pbi_470:
    ;
    if (id == PBI_reset && (nargs == 1 || nargs == 2)) {
        extern int fh_alloc(FILE *);
        const char *stale_nm = NULL;
        if (IS_FH_fn(args[0])) { int oidx = (int)args[0].i; FILE *ofp = fh_get(oidx); if (ofp) { rewind(ofp); *out = args[0]; return 1; } if (oidx >= 0 && oidx < FH_N) stale_nm = g_fh[oidx].name; }
        int hvar = IS_INT_fn(args[0]) && args[0].i < 0;
        long long hcode = hvar ? 2 * (-args[0].i) : (nargs == 2 && IS_INT_fn(args[1])) ? args[1].i : -1;
        const char *nm = (stale_nm && stale_nm[0]) ? stale_nm : hvar ? NULL : VARVAL_fn(args[0]);
        if ((!nm || !nm[0]) && hcode >= 0) {
            extern const char *pas_main_arg(int);
            const char *pa = pas_main_arg((int)(hcode >> 1));
            if (pa) nm = pa;
            else { extern void fh_ensure_init(void); fh_ensure_init(); *out = FHVAL((hcode & 1) ? 1 : 0); return 1; }
        }
        if ((!nm || !nm[0]) && nargs == 2 && !IS_INT_fn(args[1])) nm = VARVAL_fn(args[1]);
        if (nm && !strcmp(nm, "input")) { extern void fh_ensure_init(void); fh_ensure_init(); *out = FHVAL(0); return 1; }
        if (!nm || !nm[0]) { *out = FAILDESCR; return 1; }
        FILE *fp = fopen(nm, "r");
        if (!fp && pas_io_fail(errno)) { *out = IS_FH_fn(args[0]) ? args[0] : STRVAL(rt_heap_strdup_c(nm)); return 1; }
        if (!fp) { fflush(stdout); fprintf(stderr, "Runtime error 2 at $0\n"); exit(2); }
        int idx = fh_alloc(fp);
        if (idx < 0) { fclose(fp); *out = FAILDESCR; return 1; }
        g_fh[idx].name = rt_heap_strdup_c(nm);
        *out = FHVAL(idx);
        return 1;
    }
    L_pbi_493:
    ;
    if (id == PBI_fclose && nargs == 1) {
        extern FILE *fh_get(int);
        extern void fh_free(int);
        if (g_runtime.pasio.ioflow && (!IS_FH_fn(args[0]) || !fh_get((int)args[0].i))) g_runtime.pasio.inoutres = 103;
        if (IS_FH_fn(args[0])) { int idx = (int)args[0].i; FILE *fp = fh_get(idx); if (fp && fp != stdout && fp != stderr && fp != stdin) { fflush(fp); fclose(fp); fh_free(idx); } }
        *out = NULVCL;
        return 1;
    }
    L_pbi_500:
    ;
    if ((id == PBI_read_s && nargs == 0) || (id == PBI_read_s_f && nargs == 1)) {
        extern FILE *fh_get(int);
        FILE *f = nargs == 1 && IS_FH_fn(args[0]) ? fh_get((int)args[0].i) : stdin;
        if (!f) f = stdin;
        size_t cap = 64, n = 0;
        char *r = (char *)rt_wsb_alloc(cap);
        int ch;
        while ((ch = fgetc(f)) != EOF && ch != '\n') { if (n + 1 >= cap) { char *g = (char *)rt_wsb_alloc(cap * 2); memcpy(g, r, n); r = g; cap *= 2; } r[n++] = (char)ch; }
        if (ch == '\n') ungetc('\n', f);
        r[n] = '\0';
        *out = (DESCR_t){ .v = DT_S, .s = r, .slen = (uint32_t)n };
        return 1;
    }
    L_pbi_513:
    ;
    if (id == PBI_copy && nargs == 3) {
        const char *sv = VARVAL_fn(args[0]);
        long long len = sv ? (long long)pas_slen(args[0], sv) : 0, i = IS_INT_fn(args[1]) ? args[1].i : 1, n = IS_INT_fn(args[2]) ? args[2].i : 0;
        if (i < 1) i = 1;
        long long avail = len - (i - 1);
        if (avail < 0) avail = 0;
        if (n > avail) n = avail;
        if (n < 0) n = 0;
        char *r = (char *)rt_wsb_alloc((size_t)n + 1);
        if (n > 0) memcpy(r, sv + (i - 1), (size_t)n);
        r[n] = '\0';
        *out = (DESCR_t){ .v = DT_S, .s = r, .slen = (uint32_t)n };
        return 1;
    }
    L_pbi_527:
    ;
    if (id == PBI_upcase && nargs == 1) {
        if (IS_INT_fn(args[0])) { long long c = args[0].i; *out = INTVAL(c >= 'a' && c <= 'z' ? c - 32 : c); return 1; }
        const char *sv = VARVAL_fn(args[0]);
        size_t n = sv ? strlen(sv) : 0;
        char *r = (char *)rt_wsb_alloc(n + 1);
        for (size_t k = 0; k < n; k++) r[k] = (sv[k] >= 'a' && sv[k] <= 'z') ? (char)(sv[k] - 32) : sv[k];
        r[n] = '\0';
        *out = (DESCR_t){ .v = DT_S, .s = r, .slen = (uint32_t)n };
        return 1;
    }
    L_pbi_537:
    ;
    if (id == PBI_paramcount && nargs == 0) { extern int pas_main_argc(void); *out = INTVAL(pas_main_argc()); return 1; }
    L_pbi_538:
    ;
    if (id == PBI_paramstr && nargs == 1) {
        extern const char *pas_main_arg(int);
        const char *a = IS_INT_fn(args[0]) ? pas_main_arg((int)args[0].i) : (const char *)0;
        size_t n = a ? strlen(a) : 0;
        char *r = (char *)rt_wsb_alloc(n + 1);
        if (n) memcpy(r, a, n);
        r[n] = '\0';
        *out = (DESCR_t){ .v = DT_S, .s = r, .slen = (uint32_t)n };
        return 1;
    }
    L_pbi_548:
    ;
    if (id == PBI_std_mute && nargs == 0) { fflush(stdout); int nul = open("/dev/null", O_WRONLY); if (nul >= 0) { dup2(nul, 1); close(nul); } *out = NULVCL; return 1; }
    L_pbi_549:
    ;
    if (id == PBI_ferase && nargs == 1) {
        extern FILE *fh_get(int);
        extern void fh_free(int);
        const char *nm = NULL;
        if (IS_FH_fn(args[0])) {
            int idx = (int)args[0].i;
            if (idx >= 0 && idx < FH_N) nm = g_fh[idx].name;
            FILE *fp = fh_get(idx);
            if (fp && fp != stdout && fp != stderr && fp != stdin) { fflush(fp); fclose(fp); fh_free(idx); }
        } else nm = VARVAL_fn(args[0]);
        if (nm && nm[0]) remove(nm);
        *out = NULVCL;
        return 1;
    }
    L_pbi_563:
    ;
    if (id == PBI_pchar_deref && nargs == 1) { const char *s = VARVAL_fn(args[0]); *out = INTVAL(s && s[0] ? (long long)(unsigned char)s[0] : 0); return 1; }
    L_pbi_564:
    ;
    if (id == PBI_ord_check && (nargs == 4 || nargs == 5)) {
        if (IS_INT_fn(args[0]) && IS_INT_fn(args[1]) && IS_INT_fn(args[2]) && (args[0].i < args[1].i || args[0].i > args[2].i)) {
            const char *wn = VARVAL_fn(args[3]);
            char w[fmt_len("%s -- the value %lld is outside %lld..%lld", wn, (long long)args[0].i, (long long)args[1].i, (long long)args[2].i)];
            snprintf(w, sizeof w, "%s -- the value %lld is outside %lld..%lld", wn, (long long)args[0].i, (long long)args[1].i, (long long)args[2].i);
            pas_file_err(nargs == 5 ? VARVAL_fn(args[4]) : "6.6.6.4", w, NULL);
        }
        *out = args[0];
        return 1;
    }
    L_pbi_574:
    ;
    if (id == PBI_resundef && nargs == 1) { *out = INTVAL(args[0].v == DT_SNUL ? 1L : 0L); return 1; }
    L_pbi_575:
    ;
    if (id == PBI_rterr && nargs == 2) { pas_file_err(VARVAL_fn(args[0]), VARVAL_fn(args[1]), NULL); *out = NULVCL; return 1; }
    L_pbi_576:
    ;
    if (id == PBI_unhandled && nargs == 2) {
        fflush(stdout);
        fprintf(stderr, "An unhandled exception occurred at $0000000000000000:\n%s: %s\n", VARVAL_fn(args[0]), VARVAL_fn(args[1]));
        fflush(stderr);
        exit(217);
    }
    L_pbi_582:
    ;
    if (id == PBI_range_check && nargs == 5) {
        long long lo = IS_INT_fn(args[1]) ? args[1].i : 0, hi = IS_INT_fn(args[2]) ? args[2].i : 0;
        const char *cl = VARVAL_fn(args[3]);
        uint64_t bits[PAS_SET_BYTES / 8];
        pas_set_bits(args[0], (unsigned char *)bits);
        int out_of = 0;
        for (int w = 0; w < PAS_SET_BYTES / 8 && !out_of; w++) {
            long long b0 = 64LL * w, b1 = b0 + 63, s = lo > b0 ? lo - b0 : 0, e = hi < b1 ? hi - b0 : 63;
            uint64_t allow = (hi < b0 || lo > b1 || lo > hi) ? 0 : (e - s == 63) ? ~0ULL : (((1ULL << (e - s + 1)) - 1) << s);
            if (bits[w] & ~allow) out_of = 1;
        }
        if (out_of && cl && cl[0]) pas_file_err(cl, "a member of the set value lies outside the base-type of the set variable it is given to", NULL);
        if (out_of) { fflush(NULL); fprintf(stderr, "Runtime error 201 at $0\n"); exit(201); }
        *out = args[0];
        return 1;
    }
    if (id == PBI_range_check && (nargs == 3 || nargs == 4)) {
        long long v = IS_INT_fn(args[0]) ? args[0].i : 0;
        long long lo = IS_INT_fn(args[1]) ? args[1].i : 0;
        long long hi = IS_INT_fn(args[2]) ? args[2].i : 0;
        if ((v < lo || v > hi) && nargs == 4) pas_file_err(VARVAL_fn(args[3]), "the value is not assignment-compatible with the subrange type of the variable it is given to", NULL);
        if (v < lo || v > hi) { fflush(NULL); fprintf(stderr, "Runtime error 201 at $0\n"); exit(201); }
        *out = args[0];
        return 1;
    }
    L_pbi_603:
    ;
    if (id == PBI_str_to_int && nargs == 1) { const char *s = VARVAL_fn(args[0]); *out = INTVAL(s ? strtoll(s, NULL, 10) : 0); return 1; }
    L_pbi_604:
    ;
    if (id == PBI_str_delete && nargs == 3) {
        const char *s = VARVAL_fn(args[0]);
        if (!s) s = "";
        size_t len = pas_slen(args[0], s);
        long long pos = IS_INT_fn(args[1]) ? args[1].i : 1;
        long long cnt = IS_INT_fn(args[2]) ? args[2].i : 0;
        if (pos < 1) pos = 1;
        if ((size_t)pos > len || cnt <= 0) { *out = args[0]; return 1; }
        size_t start = (size_t)pos - 1;
        size_t n = (size_t)cnt;
        if (start + n > len) n = len - start;
        char *buf = rt_heap_alloc_c(len - n + 1);
        memcpy(buf, s, start);
        memcpy(buf + start, s + start + n, len - start - n);
        buf[len - n] = '\0';
        *out = BSTRVAL(buf, len - n);
        return 1;
    }
    L_pbi_622:
    ;
    if (id == PBI_str_insert && nargs == 3) {
        char cb1 = IS_INT_fn(args[0]) ? (char)args[0].i : 0;
        const char *src = IS_INT_fn(args[0]) ? &cb1 : VARVAL_fn(args[0]);
        if (!src) src = "";
        const char *s = VARVAL_fn(args[1]);
        if (!s) s = "";
        size_t slen = IS_INT_fn(args[0]) ? 1 : pas_slen(args[0], src), len = pas_slen(args[1], s);
        long long pos = IS_INT_fn(args[2]) ? args[2].i : 1;
        if (pos < 1) pos = 1;
        if ((size_t)pos > len + 1) pos = (long long)len + 1;
        size_t at = (size_t)pos - 1;
        char *buf = rt_heap_alloc_c(len + slen + 1);
        memcpy(buf, s, at);
        memcpy(buf + at, src, slen);
        memcpy(buf + at + slen, s + at, len - at);
        buf[len + slen] = '\0';
        *out = BSTRVAL(buf, len + slen);
        return 1;
    }
    L_pbi_640:
    ;
    if (id == PBI_in && nargs == 2) {
        long e = pas_ord_of(args[0]);
        DESCR_t v = args[1];
        int hit = 0;
        if (e >= 0 && e < PAS_SET_BYTES * 8) {
            if (IS_STR_fn(v) && v.slen == PAS_SET_BYTES && v.s) hit = ((unsigned char)v.s[e / 8] >> (e % 8)) & 1;
            else if (IS_INT_fn(v)) hit = e < 64 && ((v.i >> e) & 1L);
        }
        *out = INTVAL(hit ? 1 : 0);
        return 1;
    }
    L_pbi_648:
    ;
    if (id == PBI_tget || id == PBI_tput || id == PBI_tbuf_get || id == PBI_tbuf_set) {
        extern FILE *fh_get(int);
        static struct { int has; int c; } tb[512];
        if (!IS_FH_fn(args[0])) { extern void core_runtime_error(int code, const char *msg); core_runtime_error(103, "file not open (reset or rewrite it first)"); *out = FAILDESCR; return 1; }
        int idx = (int)args[0].i;
        FILE *fp = fh_get(idx);
        if (!fp || idx < 0 || idx >= 512) { *out = FAILDESCR; return 1; }
        if (id == PBI_tbuf_get) {
            int c = fgetc(fp);
            if (c == EOF) { *out = INTVAL((long long)' '); return 1; }
            ungetc(c, fp);
            if (c == '\n') c = ' ';
            *out = INTVAL((long long)(unsigned char)c);
            return 1;
        }
        if (id == PBI_tbuf_set && nargs == 2) {
            tb[idx].has = 1;
            tb[idx].c = IS_INT_fn(args[1]) ? (int)args[1].i : (VARVAL_fn(args[1]) ? (unsigned char)VARVAL_fn(args[1])[0] : ' ');
            *out = NULVCL;
            return 1;
        }
        if (id == PBI_tget) { int c = fgetc(fp); if (c == EOF) pas_file_err("6.6.5.2", "end-of-file is true immediately prior to get", fn); *out = NULVCL; return 1; }
        if (id == PBI_tput) { if (tb[idx].has) fputc(tb[idx].c, fp); tb[idx].has = 0; *out = NULVCL; return 1; }
        *out = FAILDESCR;
        return 1;
    }
    L_pbi_674:
    ;
    if (!strncmp(fn, "__pas_f", 7) || id == PBI_treset || id == PBI_trewrite) {
        extern int fh_alloc(FILE *);
        extern FILE *fh_get(int);
        if (id == PBI_trewrite && (nargs == 1 || nargs == 2)) {
            FILE *fp = (FILE *)0;
            int idx = -1;
            if (IS_FH_fn(args[0])) { idx = (int)args[0].i; fp = fh_get(idx); if (fp) { rewind(fp); if (ftruncate(fileno(fp), 0) != 0) fp = (FILE *)0; } }
            if (!fp) {
                const char *nm = IS_FH_fn(args[0]) ? (const char *)0 : VARVAL_fn(args[0]);
                if ((!nm || !nm[0]) && nargs == 2) nm = VARVAL_fn(args[1]);
                if (nm && nm[0]) fp = fopen(nm, "w+b");
                else { char tmpl[] = "/tmp/scrip_pas_XXXXXX"; int fd = mkstemp(tmpl); if (fd >= 0) unlink(tmpl); fp = (fd >= 0) ? fdopen(fd, "w+b") : (FILE *)0; }
                if (!fp) { *out = FAILDESCR; return 1; }
                idx = fh_alloc(fp);
                if (idx < 0) { fclose(fp); *out = FAILDESCR; return 1; }
            }
            if (idx >= 0) pas_tf_set(idx, INTVAL(0), 0, 1);
            *out = FHVAL(idx);
            return 1;
        }
        int pas_tf_read(FILE *fp, DESCR_t *o);
        void pas_tf_write(FILE *fp, DESCR_t v);
        void pas_file_err(const char *clause, const char *what, const char *proc);
        if (id == PBI_treset && (nargs == 1 || nargs == 2)) {
            FILE *fp = (FILE *)0;
            int idx = -1;
            if (IS_FH_fn(args[0])) { idx = (int)args[0].i; fp = fh_get(idx); if (fp) { fflush(fp); rewind(fp); } }
            if (!fp) {
                const char *nm = VARVAL_fn(args[0]);
                if ((!nm || !nm[0]) && nargs == 2) nm = VARVAL_fn(args[1]);
                if (!nm || !nm[0]) {
                    pas_file_err("6.6.5.2", "the file is undefined immediately prior to reset (it has neither been rewritten nor associated with an external file)", "__pas_treset");
                    *out = FAILDESCR;
                    return 1;
                }
                fp = fopen(nm, "rb");
                if (!fp && pas_io_fail(errno)) { *out = IS_FH_fn(args[0]) ? args[0] : STRVAL(rt_heap_strdup_c(nm)); return 1; }
                if (!fp) { fflush(NULL); fprintf(stderr, "Runtime error 2 at $0\n"); exit(2); }
                idx = fh_alloc(fp);
                if (idx < 0) { fclose(fp); *out = FAILDESCR; return 1; }
            }
            if (idx >= 0) { DESCR_t b = INTVAL(0); int h = pas_tf_read(fp, &b); pas_tf_set(idx, b, h, 2); }
            *out = FHVAL(idx);
            return 1;
        }
        if (!IS_FH_fn(args[0])) { pas_file_err("6.6.5.2", "the file is undefined (it has been neither reset nor rewritten)", fn); *out = FAILDESCR; return 1; }
        int idx = (int)args[0].i;
        FILE *fp = fh_get(idx);
        if (!fp || idx < 0 || idx >= PTF_N) { pas_file_err("6.6.5.2", "the file is undefined (no open stream stands behind it)", fn); *out = FAILDESCR; return 1; }
        if (id == PBI_fbuf_get && nargs == 1) { *out = PTF_HAS(idx) ? PTF_BUF(idx) : INTVAL(0); return 1; }
        if (id == PBI_fbuf_set && nargs == 2) { pas_tf_set(idx, args[1], 1, PTF_MODE(idx)); *out = NULVCL; return 1; }
        if (id == PBI_fput && nargs == 1) {
            if (PTF_MODE(idx) != 1) pas_file_err("6.6.5.2", "the file mode is not Generation immediately prior to put", fn);
            if (!PTF_HAS(idx)) pas_file_err("6.6.5.2", "the buffer-variable is undefined immediately prior to put", fn);
            pas_tf_write(fp, PTF_BUF(idx));
            pas_tf_set(idx, PTF_BUF(idx), 0, PTF_MODE(idx));
            *out = NULVCL;
            return 1;
        }
        if (id == PBI_fget && nargs == 1) {
            if (PTF_MODE(idx) != 2) pas_file_err("6.6.5.2", "the file mode is not Inspection immediately prior to get", fn);
            if (!PTF_HAS(idx)) pas_file_err("6.6.5.2", "end-of-file is true immediately prior to get", fn);
            { DESCR_t b = INTVAL(0); int h = pas_tf_read(fp, &b); pas_tf_set(idx, b, h, PTF_MODE(idx)); }
            *out = NULVCL;
            return 1;
        }
        if (id == PBI_fwrite && nargs == 2) {
            if (PTF_MODE(idx) != 1) pas_file_err("6.6.5.2", "the file mode is not Generation immediately prior to write", fn);
            pas_tf_write(fp, args[1]);
            *out = NULVCL;
            return 1;
        }
        if (id == PBI_fwrite_range && nargs == 4) {
            if (PTF_MODE(idx) != 1) pas_file_err("6.6.5.2", "the file mode is not Generation immediately prior to write", fn);
            long _lo = pas_ord_of(args[2]), _hi = pas_ord_of(args[3]), _ov = pas_ord_of(args[1]);
            if (_ov < _lo || _ov > _hi) pas_file_err("6.4.3.5", "the value written is not assignment-compatible with the file's subrange component-type", fn);
            pas_tf_write(fp, args[1]);
            *out = NULVCL;
            return 1;
        }
        if (id == PBI_fread && nargs == 1) {
            if (PTF_MODE(idx) != 2) pas_file_err("6.6.5.2", "the file mode is not Inspection immediately prior to read", fn);
            if (!PTF_HAS(idx)) pas_file_err("6.9.1", "the buffer-variable is undefined immediately prior to read (end-of-file is true)", fn);
            DESCR_t v = PTF_BUF(idx);
            { DESCR_t b = INTVAL(0); int h = pas_tf_read(fp, &b); pas_tf_set(idx, b, h, PTF_MODE(idx)); }
            *out = v;
            return 1;
        }
        if (id == PBI_fread_range && nargs == 3) {
            if (PTF_MODE(idx) != 2) pas_file_err("6.6.5.2", "the file mode is not Inspection immediately prior to read", fn);
            if (!PTF_HAS(idx)) pas_file_err("6.9.1", "the buffer-variable is undefined immediately prior to read (end-of-file is true)", fn);
            DESCR_t v = PTF_BUF(idx);
            { DESCR_t b = INTVAL(0); int h = pas_tf_read(fp, &b); pas_tf_set(idx, b, h, PTF_MODE(idx)); }
            long _lo = pas_ord_of(args[1]), _hi = pas_ord_of(args[2]), _ov = pas_ord_of(v);
            if (_ov < _lo || _ov > _hi) pas_file_err("6.9.1", "the value read is not assignment-compatible with the subrange of the destination variable", fn);
            *out = v;
            return 1;
        }
        if (id == PBI_feof_t && nargs == 1) { *out = INTVAL(PTF_HAS(idx) ? 0 : 1); return 1; }
        *out = FAILDESCR;
        return 1;
    }
    L_pbi_775:
    ;
    if (id == PBI_setrange && nargs == 2) {
        unsigned char *buf = (unsigned char *)rt_str_alloc(PAS_SET_BYTES);
        memset(buf, 0, PAS_SET_BYTES);
        long lo = pas_ord_of(args[0]), hi = pas_ord_of(args[1]);
        if (lo < 0) lo = 0;
        if (hi >= PAS_SET_BYTES * 8) hi = PAS_SET_BYTES * 8 - 1;
        for (long e = lo; e <= hi; e++) buf[e / 8] |= (unsigned char)(1u << (e % 8));
        *out = BSTRVAL((char *)buf, PAS_SET_BYTES);
        return 1;
    }
    L_pbi_785:
    ;
    if (id == PBI_set) {
        unsigned char *buf = (unsigned char *)rt_str_alloc(PAS_SET_BYTES);
        memset(buf, 0, PAS_SET_BYTES);
        for (int k = 0; k < nargs; k++) { long e = pas_ord_of(args[k]); if (e >= 0 && e < PAS_SET_BYTES * 8) buf[e / 8] |= (unsigned char)(1u << (e % 8)); }
        *out = BSTRVAL((char *)buf, PAS_SET_BYTES);
        return 1;
    }
    L_pbi_792:
    ;
    if (nargs == 2 && (id == PBI_setuni || id == PBI_setint || id == PBI_setdif || id == PBI_subset || id == PBI_super || id == PBI_seteq || id == PBI_setne)) {
        uint64_t a[PAS_SET_BYTES / 8], b[PAS_SET_BYTES / 8], w[PAS_SET_BYTES / 8];
        pas_set_bits(args[0], (unsigned char *)a);
        pas_set_bits(args[1], (unsigned char *)b);
        if (id == PBI_subset || id == PBI_super) {
            const uint64_t *x = id == PBI_subset ? a : b;
            const uint64_t *y = id == PBI_subset ? b : a;
            int ok = 1;
            for (int i = 0; i < PAS_SET_BYTES / 8; i++) if (x[i] & ~y[i]) { ok = 0; break; }
            *out = INTVAL(ok);
            return 1;
        }
        if (id == PBI_seteq || id == PBI_setne) { int eq = memcmp(a, b, PAS_SET_BYTES) == 0; *out = INTVAL((id == PBI_seteq ? eq : !eq) ? 1 : 0); return 1; }
        if (id == PBI_setuni) for (int i = 0; i < PAS_SET_BYTES / 8; i++) w[i] = a[i] | b[i];
        else if (id == PBI_setint) for (int i = 0; i < PAS_SET_BYTES / 8; i++) w[i] = a[i] & b[i];
        else for (int i = 0; i < PAS_SET_BYTES / 8; i++) w[i] = a[i] & ~b[i];
        unsigned char *r = (unsigned char *)rt_str_alloc(PAS_SET_BYTES);
        memcpy(r, w, PAS_SET_BYTES);
        *out = BSTRVAL((char *)r, PAS_SET_BYTES);
        return 1;
    }
    return 0;
}
