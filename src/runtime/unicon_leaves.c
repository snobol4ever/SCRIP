#define _GNU_SOURCE
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "ir/descr.h"
#include "posix_leaves.h"
#include "runtime/core/datatype.h"
#include "runtime/core/table.h"
#include "runtime/rt/rt_arena.h"
#include "runtime/rt/rt_list_view.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int um_int(DESCR_t d, int64_t *out) {
    char *e;
    if (d.v == DT_I) { *out = d.i; return 1; }
    if (d.v == DT_R) { if (!isfinite(d.r)) return 0; *out = (int64_t) d.r; return 1; }
    if (d.v != DT_S || IS_CSET_fn(d)) return 0;
    const char *s = rt_cstr_d(d);
    while (isspace((unsigned char) *s)) s++;
    if (!*s) return 0;
    long long v = strtoll(s, &e, 10);
    if (*e == '.' || *e == 'e' || *e == 'E') { double r = strtod(s, &e); v = (long long) r; }
    while (isspace((unsigned char) *e)) e++;
    if (*e) return 0;
    *out = (int64_t) v;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int um_cset_has_all(DESCR_t c, DESCR_t a) {
    const unsigned char *bits = cset_blk_bits(c.s);
    const char *s;
    size_t n;
    if (a.v == DT_S) { s = IS_CSET_fn(a) ? a.s : rt_cstr_d(a); n = descr_slen(a); } else if (a.v == DT_I) { DESCR_t t = px_fmt("%lld", (long long) a.i); s = t.s; n = t.slen; } else return 0;
    for (size_t i = 0; i < n; i++) if (!((bits[(unsigned char) s[i] >> 3] >> ((unsigned char) s[i] & 7)) & 1)) return 0;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int um_field_named(DESCR_t r, DESCR_t a) {
    if (a.v != DT_S || IS_CSET_fn(a)) return 0;
    for (int i = 0; i < r.u->type->nfields; i++) if (strlen(r.u->type->fields[i]) == a.slen && !memcmp(r.u->type->fields[i], a.s, a.slen)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t bt_ICNx24UMEMBER(DESCR_t *args, int nargs) {
    extern int core_icn_error(int, DESCR_t);
    DESCR_t x = args[0], *el;
    int n;
    int64_t k;
    if (nargs < 1) return FAILDESCR;
    if (x.v == DT_T && x.tbl) { for (int i = 1; i < nargs; i++) if (!table_has_d(x.tbl, args[i])) return FAILDESCR; return nargs > 1 ? args[nargs - 1] : x; }
    if (IS_CSET_fn(x)) { for (int i = 1; i < nargs; i++) if (!um_cset_has_all(x, args[i])) return FAILDESCR; return x; }
    if (IS_DATA_INST_fn(x) && x.u && rt_lv_is_list(x, &el, &n)) {
        for (int i = 1; i < nargs; i++) if (!um_int(args[i], &k) || k == 0 || k > n || k < -n) return FAILDESCR;
        return nargs > 1 ? args[nargs - 1] : x;
    }
    if (IS_DATA_INST_fn(x) && x.u && x.u->type) { for (int i = 1; i < nargs; i++) if (!um_field_named(x, args[i])) return FAILDESCR; return x; }
    core_icn_error(122, x);
    return FAILDESCR;
}
