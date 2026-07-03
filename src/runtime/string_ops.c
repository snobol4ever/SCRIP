#include "core.h"
#include "sil_macros.h"
#include "coerce.h"
#include "string_ops.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int so_is_list(DESCR_t v) {
    if (v.v != DT_DATA || !v.u) return 0;
    DESCR_t t = FIELD_GET_fn(v, "gen_type");
    return t.v == DT_S && t.s && !strcmp(t.s, "list");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t str_concat_d(DESCR_t a, DESCR_t b) {
    extern const char *rk_obj_stringify(DESCR_t d, int use_gist);
    if (IS_FAIL_fn(a) || IS_FAIL_fn(b)) return FAILDESCR;
    if (IS_NULL_fn(a)) return b;
    if (IS_NULL_fn(b)) return a;
    if (so_is_list(a) && so_is_list(b)) {
        static int so_list_reg = 0;
        if (!so_list_reg) { DEFDAT_fn("list(frame_elems,frame_size,gen_type)"); so_list_reg = 1; }
        int64_t an = FIELD_GET_fn(a, "frame_size").i, bn = FIELD_GET_fn(b, "frame_size").i;
        DESCR_t *ae = (DESCR_t *)FIELD_GET_fn(a, "frame_elems").ptr;
        DESCR_t *be = (DESCR_t *)FIELD_GET_fn(b, "frame_elems").ptr;
        int64_t n = an + bn;
        DESCR_t *ne = (DESCR_t *)GC_malloc((size_t)((n > 0 ? n : 1) * (int64_t)sizeof(DESCR_t)));
        for (int64_t i = 0; i < an; i++) ne[i] = ae ? ae[i] : NULVCL;
        for (int64_t i = 0; i < bn; i++) ne[an + i] = be ? be[i] : NULVCL;
        DESCR_t ep; ep.v = DT_DATA; ep.slen = 0; ep.ptr = (void *)ne;
        return DATCON_fn("list", ep, INTVAL(n), STRVAL("list"));
    }
    const char *asp, *bsp;
    if (a.v == DT_DATA) asp = rk_obj_stringify(a, 0); else { DESCR_t as = descr_to_str(a); asp = (as.v == DT_S || as.v == DT_SNUL) ? VARVAL_fn(as) : NULL; }
    if (b.v == DT_DATA) bsp = rk_obj_stringify(b, 0); else { DESCR_t bs = descr_to_str(b); bsp = (bs.v == DT_S || bs.v == DT_SNUL) ? VARVAL_fn(bs) : NULL; }
    if (!asp) asp = "";
    if (!bsp) bsp = "";
    size_t al = strlen(asp), bl = strlen(bsp);
    char *buf = GC_malloc(al + bl + 1);
    memcpy(buf, asp, al);
    memcpy(buf + al, bsp, bl);
    buf[al + bl] = '\0';
    return STRVAL(buf);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *real_str(double r, char *buf, int bufsz) {
    for (int p = 15; p <= 17; p++) {
        snprintf(buf, bufsz, "%.*g", p, r);
        char *end; double back = strtod(buf, &end);
        if (back == r) break;
    }
    if (!strchr(buf, '.') && !strchr(buf, 'e') && !strchr(buf, 'E') && !strchr(buf, 'n') && !strchr(buf, 'N'))
        strncat(buf, ".0", bufsz - strlen(buf) - 1);
    return buf;
}
