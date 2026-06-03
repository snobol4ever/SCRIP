#include "core.h"
#include "sil_macros.h"
#include "coerce.h"
#include "string_ops.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*====================================================================================================================*/
DESCR_t str_concat_d(DESCR_t a, DESCR_t b) {
    if (IS_FAIL_fn(a) || IS_FAIL_fn(b)) return FAILDESCR;
    DESCR_t as = descr_to_str(a);
    DESCR_t bs = descr_to_str(b);
    const char *asp = (as.v == DT_S || as.v == DT_SNUL) ? VARVAL_fn(as) : NULL;
    const char *bsp = (bs.v == DT_S || bs.v == DT_SNUL) ? VARVAL_fn(bs) : NULL;
    if (!asp) asp = "";
    if (!bsp) bsp = "";
    size_t al = strlen(asp), bl = strlen(bsp);
    char *buf = GC_malloc(al + bl + 1);
    memcpy(buf, asp, al);
    memcpy(buf + al, bsp, bl);
    buf[al + bl] = '\0';
    return STRVAL(buf);
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t lconcat_d(DESCR_t a, DESCR_t b) {
    if (IS_FAIL_fn(a) || IS_FAIL_fn(b)) return FAILDESCR;
    if (a.v == DT_DATA && b.v == DT_DATA) {
        DESCR_t atag = FIELD_GET_fn(a, "gen_type");
        DESCR_t btag = FIELD_GET_fn(b, "gen_type");
        if (atag.v == DT_S && atag.s && strcmp(atag.s, "list") == 0 &&
            btag.v == DT_S && btag.s && strcmp(btag.s, "list") == 0) {
            DESCR_t asz_d = FIELD_GET_fn(a, "frame_size");
            DESCR_t bsz_d = FIELD_GET_fn(b, "frame_size");
            int an = (int)(IS_INT_fn(asz_d) ? asz_d.i : 0);
            int bn = (int)(IS_INT_fn(bsz_d) ? bsz_d.i : 0);
            int cn = an + bn;
            DESCR_t *celems = GC_malloc((cn > 0 ? cn : 1) * sizeof(DESCR_t));
            DESCR_t aptr = FIELD_GET_fn(a, "frame_elems");
            DESCR_t bptr = FIELD_GET_fn(b, "frame_elems");
            DESCR_t *ae = (aptr.v == DT_DATA) ? (DESCR_t *)aptr.ptr : NULL;
            DESCR_t *be = (bptr.v == DT_DATA) ? (DESCR_t *)bptr.ptr : NULL;
            for (int i = 0; i < an; i++) celems[i]      = ae ? ae[i] : NULVCL;
            for (int i = 0; i < bn; i++) celems[an + i] = be ? be[i] : NULVCL;
            DESCR_t eptr; eptr.v = DT_DATA; eptr.slen = 0; eptr.ptr = (void *)celems;
            static int list_lcat_d = 0;
            if (!list_lcat_d) { DEFDAT_fn("list(frame_elems,frame_size,gen_type)"); list_lcat_d = 1; }
            return DATCON_fn("list", eptr, INTVAL(cn), STRVAL("list"));
        }
    }
    return str_concat_d(a, b);
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
