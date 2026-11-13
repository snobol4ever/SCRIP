#include "runtime/unification/un_head.h"
#include "ir/ct_arena.h"
#include "runtime/pl_rat.h"
#include "runtime/core/utf8.h"
#include "runtime/rt/prolog_atom.h"
#include <ctype.h>
#include "runtime/unification/un_01_pl.h"
#include "runtime/unification/un_02_pl.h"
#include "runtime/unification/un_03_pl.h"
#include "runtime/unification/un_04_pl.h"
#include "runtime/unification/un_05_pl.h"
extern int rt_pl_eng_inst_is(DESCR_t);
extern int64_t rt_pl_eng_inst_id(DESCR_t);
extern int rt_pl_dialect_swi(void);
extern int rt_pl_dialect_xsb(void);
extern int rt_pl_dialect_gnu(void);
extern int rt_pl_flag_true(const char *nm);
extern const char *prolog_atom_name(int id);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
FILE *plc_out(void) { extern FILE *fh_cur_out_fp(void); FILE *f = fh_cur_out_fp(); return f ? f : stdout; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_utf8_run(const char *q) {
    int n, k;
    if ((unsigned char)*q < 0x80) return 1;
    n = utf8_seqlen((unsigned char)*q);
    if (n < 2) return 1;
    for (k = 1; k < n; k++) if (((unsigned char)q[k] & 0xC0) != 0x80) return 1;
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_has_non_ascii(const char *s) { for (; *s; s++) if ((unsigned char)*s >= 0x80) return 1; return 0; }
static void plc_wt_atom_n(FILE *fp, const char *name, size_t len, int quoted) {
    extern int rt_pl_fp_max_code(FILE *);
    int noesc = quoted & PLC_WQ_NOESC, qna = quoted & PLC_WQ_NONASCII, xesc = quoted & PLC_WQ_XESC, maxc = -1;
    quoted &= 1;
    if (!name) { name = "?"; len = 1; }
    if (quoted && plc_has_non_ascii(name)) { int adv; maxc = rt_pl_fp_max_code(fp); for (const char *q = name; *q && maxc < 0x10FFFF; q += adv) if (rt_pl_u8_get(q, &adv) > maxc) { qna = 1; break; } }
    if (quoted && (plc_atom_needs_quoting(name) || memchr(name, 0, len) || (qna && plc_has_non_ascii(name)))) {
        fputc('\'', fp);
        for (const char *q = name; q < name + len; q++) {
            if (noesc && *q != '\'') { fputc(*q, fp); continue; }
            switch (*q) {
                case '\'':
                fputs(!noesc && rt_pl_dialect_swi() ? "\\'" : "''", fp);
                break;
                case '\\':
                fputs("\\\\", fp);
                break;
                case '\n':
                fputs("\\n", fp);
                break;
                case '\t':
                fputs("\\t", fp);
                break;
                case '\r':
                fputs("\\r", fp);
                break;
                case '\a':
                fputs("\\a", fp);
                break;
                case '\b':
                fputs("\\b", fp);
                break;
                case '\f':
                fputs("\\f", fp);
                break;
                case '\v':
                fputs("\\v", fp);
                break;
                case '\0':
                fputs("\\x0\\", fp);
                break;
                default:
                {
                    int _sl = plc_utf8_run(q), _adv, _cp = _sl > 1 ? rt_pl_u8_get(q, &_adv) : 0;
                    if (_sl > 1 && maxc < 0) maxc = rt_pl_fp_max_code(fp);
                    if (_sl > 1 && _cp > maxc) {
                        if (xesc) fprintf(fp, "\\x%X\\", (unsigned)_cp);
                        else fprintf(fp, _cp > 0xFFFF ? "\\U%08X" : "\\u%04X", (unsigned)_cp);
                        q += _sl - 1;
                    } else if (_sl > 1) {
                        for (int _k = 0; _k < _sl; _k++) fputc(q[_k], fp);
                        q += _sl - 1;
                    } else if (!isprint((unsigned char)*q)) fprintf(fp, "\\x%x\\", (unsigned)(unsigned char)*q);
                    else fputc(*q, fp);
                    break;
                }
            }
        }
        fputc('\'', fp);
    } else fwrite(name, 1, len, fp);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_wt_atom(FILE *fp, const char *name, int quoted) { plc_wt_atom_n(fp, name, name ? strlen(name) : 1, quoted); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_wt_string(FILE *fp, pl_cell_t *d, int quoted) {
    const char *b = d->s ? d->s : "";
    uint32_t n = d->s ? d->slen : 0;
    if (!(quoted & 1)) { fwrite(b, 1, n, fp); return; }
    fputc('"', fp);
    for (uint32_t i = 0; i < n; i++) {
        unsigned char c = (unsigned char)b[i];
        if ((quoted & PLC_WQ_NOESC) && c != '"') { fputc(c, fp); continue; }
        switch (c) {
            case '"':
            fputs("\\\"", fp);
            break;
            case '\\':
            fputs("\\\\", fp);
            break;
            case '\n':
            fputs("\\n", fp);
            break;
            case '\t':
            fputs("\\t", fp);
            break;
            case '\r':
            fputs("\\r", fp);
            break;
            case '\a':
            fputs("\\a", fp);
            break;
            case '\b':
            fputs("\\b", fp);
            break;
            case '\f':
            fputs("\\f", fp);
            break;
            case '\v':
            fputs("\\v", fp);
            break;
            default:
            if (c < 0x20 || c == 0x7f) fprintf(fp, "\\x%x\\", (unsigned)c);
            else fputc(c, fp);
        }
    }
    fputc('"', fp);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_is_graphic_char(int ch) { static const char *const g = "#$&*+-./:<=>?@\\^~"; return ch && strchr(g, ch) != (const char *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int plc_is_op_atom(pl_cell_t *d) {
    int p = 0, l = 0, r = 0;
    int nid = (int)d->v == DT_PLATOM ? (int)d->i : -1;
    if (nid >= 0 && nid == g_runtime.atoms.atom_dot && rt_pl_dialect_swi()) return 1;
    return nid >= 0 && (plc_op_info_id(nid, 2, &p, &l, &r) || plc_op_info_id(nid, 1, &p, &l, &r));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_first_char(pl_cell_t *c, int quoted, int ignore_ops, int numbervars) { return plc_first_char_d(c, quoted, ignore_ops, numbervars, LONG_MAX); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_alpha_glue(int ch) { return isalnum((unsigned char)ch) || ch == '_' || ch >= 0x80; }
static int plc_last_char(pl_cell_t *c, int quoted, int ignore_ops, int numbervars) { return plc_last_char_d(c, quoted, ignore_ops, numbervars, LONG_MAX); }
static int plc_portray_hit(pl_cell_t *d, plc_vmap *m) {
    extern DESCR_t rt_pl_goal_gen_h_c(DESCR_t goal, DESCR_t *argv, int n, void **hslot, void **ball, pl_tr_ctx_t *cx);
    extern int fh_alloc(FILE *);
    extern int fh_find_fp(FILE *);
    extern void fh_free(int);
    extern int fh_current_output(void);
    extern void fh_set_output(int);
    if (m->pthrown || !d) return 0;
    {
        pl_cell_t g = m->pgoal ? *m->pgoal : plc_atom_id_cell(prolog_atom_intern("portray"));
        DESCR_t a[2] = { *d, m->popts ? *m->popts : *d };
        void *h = (void *)0;
        void *b = (void *)0;
        int sv = fh_current_output();
        int slot = fh_find_fp(m->fp);
        int own = slot < 0;
        DESCR_t r;
        if (own) slot = fh_alloc(m->fp);
        if (slot < 0) return 0;
        if (own) { extern const char *fh_alias_name(int); extern void fh_set_alias(int, const char *); fh_set_alias(slot, fh_alias_name(sv)); }
        fflush(m->fp);
        fh_set_output(slot);
        r = rt_pl_goal_gen_h_c(g, a, m->pgoal ? 2 : 1, &h, &b, m->cx);
        if (!b && IS_FAIL_fn(r)) { extern void *rt_pl_ball_take(void); b = rt_pl_ball_take(); }
        fflush(m->fp);
        fh_set_output(sv);
        if (own) fh_free(slot);
        if (h) { extern void rt_proc_drop_frame_h(void **hslot); m->pheld++; rt_proc_drop_frame_h(&h); }
        if (b) { m->pthrown = b; return 0; }
        return !IS_FAIL_fn(r);
    }
}
#define PLC_KID(cl, dp, mp) do { if (wx) plc_wt((cl), quoted, ignore_ops, numbervars, max_depth, (dp), (mp), m); else plc_wt_lv((cl), quoted, ignore_ops, numbervars, max_depth, (dp), (mp), m); } \
    while (0)
static void plc_wt_lv(pl_cell_t *c, int quoted, int ignore_ops, int numbervars, long max_depth, long depth, int maxp, plc_vmap *m) {
    FILE *fp = m->fp;
    const pl_tr_ctx_t *wx = m->portray ? m->cx : (const pl_tr_ctx_t *)0;
    plc_wsref rd = { -1, (pl_cell_t *)0 };
    if (!c) { plc_wt_atom(fp, "[]", quoted); return; }
    pl_cell_t *d = pl_deref(c);
    if (wx) plc_wsref_set(wx, &rd, d);
    if (m->pthrown || (m->portray && (m->portray(d, m) || m->pthrown))) return;
    if (wx) d = plc_wsref_get(wx, &rd);
    if (max_depth > 0 && depth >= max_depth) { fprintf(fp, "..."); return; }
    int tg = (int)d->v;
    if (pl_cell_unbound(d)) { { const char *vn = plc_vname(m, d); if (vn) fputs(vn, fp); else fprintf(fp, "_G%d", plc_vindex(m, d)); } return; }
    if (tg == DT_S) { plc_wt_string(fp, d, quoted); return; }
    if (tg == DT_PLATOM) { plc_wt_atom_n(fp, plc_atom_text(d), plc_atom_len(d), quoted); return; }
    if (tg == DT_I) { plc_put_int(m, (long)d->i); return; }
    if (tg == DT_R) { plc_put_float(m, d); return; }
    if (tg == DT_BIG) { extern char *rt_big_str(DESCR_t); char *bs = rt_big_str(*d); fputs(bs ? bs : "0", fp); return; }
    if (pl_rat_is(*d)) { pl_rat_fput(*d, fp); return; }
    if (rt_pl_eng_inst_is(*d)) { fprintf(fp, "<engine>(%ld,0x%lx)", (long)rt_pl_eng_inst_id(*d), (unsigned long)(uintptr_t)d->u); return; }
    if (tg != DT_PLREF) { { const char *vn = plc_vname(m, d); if (vn) fputs(vn, fp); else fprintf(fp, "_G%d", plc_vindex(m, d)); } return; }
    int fnid = plc_fid_name(d->slen), ar = plc_fid_arity(d->slen);
    pl_cell_t *aa = (pl_cell_t *)d->p;
    const char *fn = prolog_atom_name(fnid);
    if (!fn) fn = "?";
    if (numbervars && strcmp(fn, "$VAR") == 0 && ar == 1) {
        pl_cell_t *n = pl_deref(&aa[0]);
        if ((int)n->v == DT_I) {
            long num = (long)n->i;
            int letter = (int)(num % 26);
            long suf = num / 26;
            if (suf == 0) fprintf(fp, "%c", 'A' + letter);
            else fprintf(fp, "%c%ld", 'A' + letter, suf);
            return;
        }
        if ((int)n->v == DT_PLATOM || (int)n->v == DT_S) { const char *vn = plc_atom_text(n); fputs(vn ? vn : "", fp); return; }
    }
    if (numbervars && strcmp(fn, "$VARNAME") == 0 && ar == 1) {
        pl_cell_t *n = pl_deref(&aa[0]);
        if ((int)n->v == DT_PLATOM || (int)n->v == DT_S) { const char *vn = plc_atom_text(n); fprintf(fp, "%s", vn ? vn : "_"); return; }
    }
    if ((!ignore_ops || (ignore_ops & 2)) && fnid == g_runtime.atoms.atom_dot && ar == 2) {
        if (m->xstr && !wx && plc_xsb_codes(m, d)) return;
        if (max_depth > 0 && !wx && rt_pl_dialect_gnu()) {
            long rem = max_depth - depth;
            int first = 1;
            pl_cell_t *gc = d;
            fputc('[', fp);
            for (;;) {
                pl_cell_t *ca = (pl_cell_t *)gc->p;
                pl_cell_t *hd = pl_deref(&ca[0]);
                int hv = pl_cell_unbound(hd);
                int hc = ((int)hd->v == DT_PLREF);
                int emit;
                pl_cell_t *tl;
                if (rem > 0 && (!first || hc || hv)) rem--;
                emit = (rem != 0 || first || hv);
                if (emit) { if (!first) fputs(m->space_args || m->next_arg ? ", " : ",", fp); PLC_KID(&ca[0], (hv && rem == 0) ? max_depth - 1 : max_depth - rem, 999); }
                tl = pl_deref(&ca[1]);
                if (rem == 0) { if (!emit || !plc_is_nil(tl)) fputs("|...", fp); break; }
                first = 0;
                if ((int)tl->v == DT_PLREF && plc_fid_name(tl->slen) == g_runtime.atoms.atom_dot && plc_fid_arity(tl->slen) == 2 && tl->p) { gc = tl; continue; }
                if (!plc_is_nil(tl)) { fputc('|', fp); PLC_KID(tl, max_depth - rem, 999); }
                break;
            }
            fputc(']', fp);
            return;
        }
        extern void *rt_pl_ball_kind1(const char *, const char *);
        pl_cell_t *cur = d;
        long n = 0;
        pl_cell_t *tort = d;
        plc_wsref rc = { -1, (pl_cell_t *)0 }, rtt = { -1, (pl_cell_t *)0 };
        if (wx) { plc_wsref_set(wx, &rc, cur); plc_wsref_set(wx, &rtt, tort); }
        fputc('[', fp);
        for (;;) {
            pl_cell_t *ca = (pl_cell_t *)cur->p;
            pl_cell_t *tl;
            if (max_depth > 0 && n > 0 && depth + (m->xstr ? 0 : 1) + n >= max_depth) { fprintf(fp, "|..."); break; }
            if (n) fputs(m->space_args || m->next_arg ? ", " : ",", fp);
            PLC_KID(&ca[0], depth + 1 + (m->xstr ? 0 : n), 999);
            n++;
            if (wx) { cur = plc_wsref_get(wx, &rc); tort = plc_wsref_get(wx, &rtt); ca = (pl_cell_t *)cur->p; }
            tl = pl_deref(&ca[1]);
            if ((int)tl->v == DT_PLREF && plc_fid_name(tl->slen) == g_runtime.atoms.atom_dot && plc_fid_arity(tl->slen) == 2 && tl->p) {
                cur = tl;
                if (cur == tort && max_depth <= 0) { fprintf(fp, "|..."); if (!m->pthrown) m->pthrown = rt_pl_ball_kind1("resource_error", "cyclic_term"); break; }
                if ((n & (n - 1)) == 0) tort = cur;
                if (wx) { plc_wsref_set(wx, &rc, cur); plc_wsref_set(wx, &rtt, tort); }
                continue;
            }
            if (!plc_is_nil(tl)) { if (max_depth > 0 && depth + (m->xstr ? 0 : 1) + n >= max_depth) fputs("|...", fp); else { fputc('|', fp); PLC_KID(tl, depth + 1 + n, 999); } }
            break;
        }
        fputc(']', fp);
        return;
    }
    if ((ar & 1) && strcmp(fn, "$dict") == 0) {
        PLC_KID(&aa[0], depth+1, 0);
        if (wx) { d = plc_wsref_get(wx, &rd); aa = (pl_cell_t *)d->p; }
        fputc('{', fp);
        for (int i = 1; i < ar; i += 2) {
            if (i > 1) fputs(m->next_arg ? ", " : ",", fp);
            PLC_KID(&aa[i + 1], depth+1, 0);
            if (wx) { d = plc_wsref_get(wx, &rd); aa = (pl_cell_t *)d->p; }
            fputc(':', fp);
            if (plc_is_graphic_char(plc_first_char(&aa[i], quoted, ignore_ops, numbervars))) fputc(' ', fp);
            PLC_KID(&aa[i], depth+1, 999);
            if (wx) { d = plc_wsref_get(wx, &rd); aa = (pl_cell_t *)d->p; }
        }
        fputc('}', fp);
        return;
    }
    if ((!ignore_ops || (ignore_ops & 4)) && ar == 1 && strcmp(fn, "{}") == 0) { fputs(m->space_args ? "{ " : "{", fp); PLC_KID(&aa[0], depth+1, 1200); fputs(m->space_args ? " }" : "}", fp); return; }
    if (!ignore_ops && ar == 2 && strcmp(fn, "{}") == 0 && plc_op_is_postfix(fn)) {
        pl_cell_t *b0 = pl_deref(&aa[0]);
        int pp = 0, lm = 0, rm = 0;
        if ((int)b0->v == DT_PLREF && plc_fid_arity(b0->slen) == 1 && !strcmp(prolog_atom_name(plc_fid_name(b0->slen)), "{}") && plc_op_info(fn, 1, &pp, &lm, &rm)) {
            if (pp > maxp) fputc('(', fp);
            PLC_KID(&aa[1], depth+1, lm);
            if (wx) { d = plc_wsref_get(wx, &rd); aa = (pl_cell_t *)d->p; }
            fputc(' ', fp);
            PLC_KID(&aa[0], depth+1, 0);
            if (pp > maxp) fputc(')', fp);
            return;
        }
    }
    {
        int op_prec = 0, lmax = 0, rmax = 0;
        if (!ignore_ops && (ar == 1 || ar == 2) && plc_op_info(fn, ar, &op_prec, &lmax, &rmax)) {
            long kdl = max_depth > 0 ? max_depth - (depth + 1) : LONG_MAX;
            int kel = kdl <= 0;
            int wrap = op_prec > maxp;
            if (wrap) fputc('(', fp);
            if (ar == 2) {
                int xsbp = rt_pl_dialect_xsb() && !quoted;
                int lop = !kel && !xsbp && plc_is_op_atom(pl_deref(&aa[0])), rop = !kel && !xsbp && plc_is_op_atom(pl_deref(&aa[1]));
                if (lop) fputc('(', fp);
                PLC_KID(&aa[0], depth+1, lop ? 1200 : lmax);
                if (wx) { d = plc_wsref_get(wx, &rd); aa = (pl_cell_t *)d->p; }
                if (lop) fputc(')', fp);
                if (isalnum((unsigned char)fn[0]) || fn[0] == '_') {
                    if (rt_pl_dialect_swi() && !m->space_args) {
                        int lead = plc_alpha_glue((lop || (!kel && plc_term_prio(&aa[0], ignore_ops) > lmax)) ? ')' : plc_last_char_d(&aa[0], quoted, ignore_ops, numbervars, kdl));
                        int rf = (rop || (!kel && plc_term_prio(&aa[1], ignore_ops) > rmax)) ? '(' : plc_first_char_d(&aa[1], quoted, ignore_ops, numbervars, kdl);
                        fprintf(fp, "%s%s%s", lead ? " " : "", fn, (lead || plc_alpha_glue(rf)) ? " " : "");
                    } else fprintf(fp, " %s ", fn);
                } else if (!strcmp(fn, ",") || !strcmp(fn, "|")) {
                    fputc(fn[0], fp);
                    if (m->space_args || m->next_arg) fputc(' ', fp);
                } else if (m->space_args || (rt_pl_dialect_xsb() && strcmp(fn, ";"))) {
                    fputc(' ', fp);
                    plc_wt_atom(fp, fn, quoted);
                    fputc(' ', fp);
                } else if (!fn[0] && !quoted) {
                    fputc(' ', fp);
                } else {
                    int lglue = !lop && plc_is_graphic_char((unsigned char)fn[0]) && (kel || plc_term_prio(&aa[0], ignore_ops) <= lmax) &&
                        plc_is_graphic_char(plc_last_char_d(&aa[0], quoted, ignore_ops, numbervars, kdl));
                    if (lglue) fputc(' ', fp);
                    plc_wt_atom(fp, fn, quoted && strcmp(fn, ";"));
                    if (lglue || (!rop && (kel || plc_term_prio(&aa[1], ignore_ops) <= rmax) && plc_is_graphic_char(plc_first_char_d(&aa[1], quoted, ignore_ops, numbervars, kdl)))) fputc(' ', fp);
                }
                if (rop) fputc('(', fp);
                PLC_KID(&aa[1], depth+1, rop ? 1200 : rmax);
                if (rop) fputc(')', fp);
            } else if (plc_op_is_postfix(fn)) {
                int lglue = plc_is_graphic_char((unsigned char)fn[0]) && (kel || plc_term_prio(&aa[0], ignore_ops) <= lmax) &&
                    plc_is_graphic_char(plc_last_char_d(&aa[0], quoted, ignore_ops, numbervars, kdl));
                PLC_KID(&aa[0], depth+1, lmax);
                if (isalnum((unsigned char)fn[0]) || fn[0] == '_') fprintf(fp, " %s", fn);
                else { if (lglue) fputc(' ', fp); plc_wt_atom(fp, fn, quoted); }
            } else {
                {
                    pl_cell_t *a0 = pl_deref(&aa[0]);
                    int ap = kel ? 0 : plc_term_prio(a0, ignore_ops);
                    int alnum_op = isalnum((unsigned char)fn[0]) || fn[0] == '_';
                    extern int rt_big_sign(DESCR_t);
                    int a_num = !kel && (((int)a0->v == DT_I && a0->i >= 0) || ((int)a0->v == DT_R && a0->r >= 0) || ((int)a0->v == DT_BIG && rt_big_sign(*a0) >= 0));
                    int a_opatom = !kel && !(rt_pl_dialect_xsb() && !quoted) && plc_is_op_atom(a0);
                    int a_digit = !kel && !strcmp(fn, "-") && isdigit(plc_first_char(a0, quoted, ignore_ops, numbervars));
                    int swi_numop = rt_pl_dialect_swi() && !kel && !strcmp(fn, "-") && (a_num || a_digit);
                    int xsbw = rt_pl_dialect_xsb() && !quoted;
                    int needp = (ap > rmax) || a_opatom || (!swi_numop && !xsbw && ((a_num && !strcmp(fn, "-")) || (!rt_pl_dialect_xsb() && a_digit)));
                    int sep = needp || swi_numop || rt_pl_dialect_xsb() || (!alnum_op && plc_is_graphic_char(plc_first_char_d(&aa[0], quoted, ignore_ops, numbervars, kdl)));
                    if (alnum_op) fprintf(fp, "%s ", fn);
                    else plc_wt_atom(fp, fn, quoted);
                    if ((sep || m->space_args) && !alnum_op) fputc(' ', fp);
                    if (needp) fputc('(', fp);
                    PLC_KID(&aa[0], depth+1, needp ? 1200 : rmax);
                    if (needp) fputc(')', fp);
                }
            }
            if (wrap) fputc(')', fp);
            return;
        }
    }
    {
        int xopf = m->xopf && quoted && ignore_ops && ((fn[0] == '.' && fn[1] == 0) || plc_op_info(fn, 1, &(int){0}, &(int){0}, &(int){0}) || plc_op_info(fn, 2, &(int){0}, &(int){0}, &(int){0}));
        if (xopf) fputc('(', fp);
        plc_wt_atom(fp, fn, quoted || (fn[0] == '.' && fn[1] == 0 && !rt_pl_dialect_xsb()));
        if (xopf) fputc(')', fp);
    }
    fprintf(fp, "(");
    if (max_depth > 0 && ar > 0 && depth + 1 >= max_depth && rt_pl_dialect_xsb() && !wx) { fputs("...)", fp); return; }
    int nb = ar;
    if (max_depth > 0 && !wx && rt_pl_dialect_gnu() && max_depth - depth < ar) nb = (int)(max_depth - depth);
    for (int i = 0; i < nb; i++) {
        int opq = quoted && rt_pl_dialect_xsb() && !ignore_ops && plc_is_op_atom(pl_deref(&aa[i])) && strcmp(plc_atom_text(pl_deref(&aa[i])), "|");
        if (i) fputs(m->space_args || m->next_arg ? ", " : ",", fp);
        if (opq) fputc('(', fp);
        PLC_KID(&aa[i], depth+1, 999);
        if (opq) fputc(')', fp);
        if (wx) { d = plc_wsref_get(wx, &rd); aa = (pl_cell_t *)d->p; }
    }
    if (nb < ar) fputs(m->space_args || m->next_arg ? ", ..." : ",...", fp);
    fprintf(fp, ")");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#undef PLC_KID
static plc_fz_ent_t *plc_fz_slot(plc_fz_t *z, void *blk) {
    long i = (long)(((uintptr_t)blk >> 4) * 0x9E3779B97F4A7C15ull >> 20) & z->mask;
    while (z->tab[i].blk && z->tab[i].blk != blk) i = (i + 1) & z->mask;
    return &z->tab[i];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_fz_run(void *wv) {
    plc_fz_t *w = (plc_fz_t *)wv;
    for (;;) {
        pl_cell_t *d = plr_resolve(pl_deref(w->s));
        int ar = (int)d->v == DT_PLREF ? plc_fid_arity(d->slen) : 0;
        plc_fz_ent_t *e = ar > 0 ? plc_fz_slot(w, d->p) : (plc_fz_ent_t *)0;
        if (e && e->blk) {
            pl_cell_t *nb = e->nb;
            if ((!e->done || w->dag) && nb[ar].v == DT_SNUL) {
                pl_cell_t *kc = (pl_cell_t *)rt_ws_alloc_descr(2);
                nb[ar] = plc_fz_var(w, ++w->next_k);
                kc[0] = pl_make_compound(plc_fid_name(d->slen), ar, nb);
                kc[1] = w->knodes;
                w->knodes = pl_make_compound(g_runtime.atoms.atom_dot, 2, kc);
            }
            *w->d = nb[ar].v != DT_SNUL ? nb[ar] : pl_make_compound(plc_fid_name(d->slen), ar, nb);
        } else if (e) {
            pl_cell_t *nb;
            if (!plr_room(&w->ag)) return plr_more(&w->ag, &w->lg, plc_fz_run, w);
            nb = (pl_cell_t *)rt_ws_alloc_descr((size_t)ar + 3);
            memset(&nb[ar], 0, sizeof nb[ar]);
            nb[ar + 1] = pl_make_int((int64_t)(intptr_t)w->d);
            nb[ar + 2] = pl_make_int((int64_t)(e - w->tab));
            e->blk = d->p;
            e->nb = nb;
            e->done = 0;
            *w->d = pl_make_compound(plc_fid_name(d->slen), ar, nb);
            plr_push(&w->ag, d->p, nb, (uint64_t)ar);
        } else *w->d = pl_cell_unbound(d) ? pl_make_ref(d, (int)d->slen) : *d;
        for (;;) {
            plr_ent_t *t;
            if (plr_empty(&w->ag)) return 0;
            t = plr_peek(&w->ag);
            if (!t->n) { w->tab[((pl_cell_t *)t->y)[2].i].done = 1; plr_drop(&w->ag); continue; }
            w->s = (pl_cell_t *)t->x;
            w->d = (pl_cell_t *)t->y;
            t->x = (void *)(w->s + 1);
            t->y = (void *)(w->d + 1);
            t->n--;
            break;
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_fz_count_visit(pl_cell_t *d, void *ctx) { if ((int)d->v == DT_PLREF) ++*(long *)ctx; return PLR_V_GO; }
PLR_WALK1(plc_fz_count_walk, plc_fz_count_visit, pl_deref) pl_cell_t plc_factorize_x(pl_cell_t *c, int named, int dag) {
    long n = 0, cap = 64;
    pl_cell_t out, subs = pl_make_atom(g_runtime.atoms.atom_nil), *args = (pl_cell_t *)rt_ws_alloc_descr(2);
    plc_fz_t w;
    (void)plc_fz_count_walk(c, (void *)&n);
    while (cap < 2 * n + 2) cap <<= 1;
    {
        plc_fz_ent_t tab[cap];
        memset(tab, 0, sizeof tab);
        memset(&out, 0, sizeof out);
        plr_stk_init(&w.ag);
        plr_stk_init(&w.lg);
        w.s = c;
        w.d = &out;
        w.next_k = 0;
        w.named = named;
        w.dag = dag;
        w.knodes = pl_make_atom(g_runtime.atoms.atom_nil);
        w.tab = tab;
        w.mask = cap - 1;
        (void)plc_fz_run(&w);
    }
    for (pl_cell_t kl = w.knodes; (int)kl.v == DT_PLREF; kl = ((pl_cell_t *)kl.p)[1]) {
        pl_cell_t node = ((pl_cell_t *)kl.p)[0], *nb = (pl_cell_t *)node.p, *eq = (pl_cell_t *)rt_ws_alloc_descr(2), *cons = (pl_cell_t *)rt_ws_alloc_descr(2);
        int ar = plc_fid_arity(node.slen);
        *(pl_cell_t *)(intptr_t)nb[ar + 1].i = nb[ar];
        eq[0] = nb[ar];
        eq[1] = node;
        cons[0] = pl_make_compound(prolog_atom_intern("="), 2, eq);
        cons[1] = subs;
        subs = pl_make_compound(g_runtime.atoms.atom_dot, 2, cons);
    }
    args[0] = out;
    args[1] = subs;
    return pl_make_compound(prolog_atom_intern("@"), 2, args);
}
static pl_cell_t plc_factorize(pl_cell_t *c, int named) { return plc_factorize_x(c, named, 0); }
void plc_wt(pl_cell_t *c, int quoted, int ignore_ops, int numbervars, long max_depth, long depth, int maxp, plc_vmap *m) {
    const pl_tr_ctx_t *wx = m->portray ? m->cx : (const pl_tr_ctx_t *)0;
    long mark = plc_ws_mark(wx);
    if (!depth && max_depth <= 0 && c && !pl_acyclic_walk(c)) {
        pl_cell_t *f = (pl_cell_t *)rt_ws_alloc_descr(1);
        plc_wsref rf = { -1, (pl_cell_t *)0 };
        *f = plc_factorize(c, numbervars);
        if (wx) plc_wsref_set(wx, &rf, f);
        fputs("@(", m->fp);
        plc_wt_lv(&((pl_cell_t *)f->p)[0], quoted, ignore_ops, numbervars, 0, 1, 999, m);
        if (wx) f = plc_wsref_get(wx, &rf);
        fputc(',', m->fp);
        plc_wt_lv(&((pl_cell_t *)f->p)[1], quoted, ignore_ops, numbervars, 0, 1, 999, m);
        fputc(')', m->fp);
        plc_ws_end(wx, mark);
        return;
    }
    plc_wt_lv(c, quoted, ignore_ops, numbervars, max_depth, depth, maxp, m);
    plc_ws_end(wx, mark);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_write(pl_cell_t *c, plc_vmap *m) { plc_wt(c, 0, 0, 1, 0, 0, rt_pl_dialect_xsb() ? 1000 : 1200, m); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void plc_writeq(pl_cell_t *c, plc_vmap *m) { plc_wt(c, 1, 0, 1, 0, 0, rt_pl_dialect_xsb() ? 1000 : 1200, m); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_write_canonical(pl_cell_t *c, plc_vmap *m) { plc_wt(c, 1, (rt_pl_dialect_swi() || rt_pl_dialect_xsb()) ? 3 : PLC_WC_IGNORE_OPS, 0, 0, 0, 1200, m); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void plc_atoms_ready(void) { extern void prolog_atom_init(void); if (g_runtime.atoms.atom_dot < 0) prolog_atom_init(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_write_cell(void *cell) {
    plc_atoms_ready();
    plc_vmap m;
    m.n = 0;
    m.vnc = 0;
    m.ibase = 0;
    m.inamed = 0;
    m.ipfx = 1;
    m.xstr = 0;
    m.fprec = -1;
    m.fspec = 0;
    m.xopf = 0;
    m.fp = plc_out();
    m.portray = (int (*)(pl_cell_t *, plc_vmap *))0;
    m.pthrown = (void *)0;
    m.cx = (pl_tr_ctx_t *)0;
    m.pheld = 0;
    m.pgoal = (pl_cell_t *)0;
    m.popts = (pl_cell_t *)0;
    m.space_args = 0;
    m.next_arg = 0;
    plc_write((pl_cell_t *)cell, &m);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_write_cell_fp(void *cell, FILE *fp) {
    plc_atoms_ready();
    plc_vmap m;
    m.n = 0;
    m.vnc = 0;
    m.ibase = 0;
    m.inamed = 0;
    m.ipfx = 1;
    m.xstr = 0;
    m.fprec = -1;
    m.fspec = 0;
    m.xopf = 0;
    m.fp = fp ? fp : plc_out();
    m.portray = (int (*)(pl_cell_t *, plc_vmap *))0;
    m.pthrown = (void *)0;
    m.cx = (pl_tr_ctx_t *)0;
    m.pheld = 0;
    m.pgoal = (pl_cell_t *)0;
    m.popts = (pl_cell_t *)0;
    m.space_args = 0;
    m.next_arg = 0;
    plc_write((pl_cell_t *)cell, &m);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_write_cell_fp_p(void *cell, FILE *fp, int maxp) {
    plc_atoms_ready();
    plc_vmap m;
    m.n = 0;
    m.vnc = 0;
    m.ibase = 0;
    m.inamed = 0;
    m.ipfx = 1;
    m.xstr = 0;
    m.fprec = -1;
    m.fspec = 0;
    m.xopf = 0;
    m.fp = fp ? fp : plc_out();
    m.portray = (int (*)(pl_cell_t *, plc_vmap *))0;
    m.pthrown = (void *)0;
    m.cx = (pl_tr_ctx_t *)0;
    m.pheld = 0;
    m.pgoal = (pl_cell_t *)0;
    m.popts = (pl_cell_t *)0;
    m.space_args = 0;
    m.next_arg = 0;
    plc_wt((pl_cell_t *)cell, 0, 0, 1, 0, 0, maxp, &m);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_writeq_cell(void *cell) {
    plc_atoms_ready();
    plc_vmap m;
    m.n = 0;
    m.vnc = 0;
    m.ibase = 0;
    m.inamed = 0;
    m.ipfx = 1;
    m.xstr = 0;
    m.fprec = -1;
    m.fspec = 0;
    m.xopf = 0;
    m.fp = plc_out();
    m.portray = (int (*)(pl_cell_t *, plc_vmap *))0;
    m.pthrown = (void *)0;
    m.cx = (pl_tr_ctx_t *)0;
    m.pheld = 0;
    m.pgoal = (pl_cell_t *)0;
    m.popts = (pl_cell_t *)0;
    m.space_args = 0;
    m.next_arg = 0;
    plc_writeq((pl_cell_t *)cell, &m);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_write_canonical_cell(void *cell) {
    plc_atoms_ready();
    plc_vmap m;
    m.n = 0;
    m.vnc = 0;
    m.ibase = 0;
    m.inamed = 0;
    m.ipfx = 1;
    m.xstr = 0;
    m.fprec = -1;
    m.fspec = 0;
    m.xopf = 0;
    m.fp = plc_out();
    m.portray = (int (*)(pl_cell_t *, plc_vmap *))0;
    m.pthrown = (void *)0;
    m.cx = (pl_tr_ctx_t *)0;
    m.pheld = 0;
    m.pgoal = (pl_cell_t *)0;
    m.popts = (pl_cell_t *)0;
    m.space_args = 0;
    m.next_arg = 0;
    plc_write_canonical((pl_cell_t *)cell, &m);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_display_cell_ball(void *cell) {
    plc_atoms_ready();
    plc_vmap m;
    m.n = 0;
    m.vnc = 0;
    m.ibase = 0;
    m.inamed = 0;
    m.ipfx = 1;
    m.xstr = 0;
    m.fprec = -1;
    m.fspec = 0;
    m.xopf = 0;
    m.fp = plc_out();
    m.portray = (int (*)(pl_cell_t *, plc_vmap *))0;
    m.pthrown = (void *)0;
    m.cx = (pl_tr_ctx_t *)0;
    m.pheld = 0;
    m.pgoal = (pl_cell_t *)0;
    m.popts = (pl_cell_t *)0;
    m.space_args = 0;
    m.next_arg = 0;
    plc_wt((pl_cell_t *)cell, 0, 1, 0, 0, 0, 1200, &m);
    return m.pthrown;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_display_cell(void *cell) { rt_pl_display_cell_ball(cell); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_write_term_cell(void *term_cell, void *opts_cell, void *cxp) {
    plc_atoms_ready();
    plc_vmap m;
    m.n = 0;
    m.vnc = 0;
    m.ibase = 0;
    m.inamed = 0;
    m.ipfx = 1;
    m.xstr = 0;
    m.fprec = -1;
    m.fspec = 0;
    m.xopf = 0;
    m.fp = plc_out();
    m.portray = (int (*)(pl_cell_t *, plc_vmap *))0;
    m.pthrown = (void *)0;
    m.cx = (pl_tr_ctx_t *)0;
    m.pheld = 0;
    m.pgoal = (pl_cell_t *)0;
    m.popts = (pl_cell_t *)0;
    m.space_args = 0;
    m.next_arg = 0;
    int portable = 0, quoted = 0, ignore_ops = 0, numbervars = 0, prio = rt_pl_dialect_xsb() ? 1000 : 1200, noesc = 0, qna = 0, partial = 0, xesc = !rt_pl_flag_true("character_escapes_unicode"),
        nolists = 0, nobraces = 0, keeplists = 0;
    long max_depth = 0;
    pl_cell_t *lst = opts_cell ? pl_deref((pl_cell_t *)opts_cell) : (pl_cell_t *)0;
    while (lst && (int)lst->v == DT_PLREF && plc_fid_arity(lst->slen) == 2) {
        pl_cell_t *pair = (pl_cell_t *)lst->p;
        pl_cell_t *o = pl_deref(&pair[0]);
        if ((int)o->v == DT_PLREF && plc_fid_arity(o->slen) == 1) {
            const char *on = prolog_atom_name(plc_fid_name(o->slen));
            pl_cell_t *oa = (pl_cell_t *)o->p;
            if (on && !strcmp(on, "quoted")) quoted = plc_opt_is_true(o);
            else if (on && !strcmp(on, "character_escapes")) noesc = !plc_opt_is_true(o);
            else if (on && !strcmp(on, "quote_non_ascii")) qna = plc_opt_is_true(o);
            else if (on && !strcmp(on, "character_escapes_unicode")) xesc = !plc_opt_is_true(o);
            else if (on && !strcmp(on, "partial")) partial = plc_opt_is_true(o);
            else if (on && !strcmp(on, "priority")) {
                pl_cell_t *a = pl_deref(&oa[0]);
                if ((int)a->v == DT_I && a->i >= 0 && a->i <= 1200) prio = (int)a->i;
            } else if (on && !strcmp(on, "ignore_ops")) ignore_ops = plc_opt_is_true(o);
            else if (on && (!strcmp(on, "no_lists") || !strcmp(on, "dotlists"))) nolists = plc_opt_is_true(o);
            else if (on && !strcmp(on, "brace_terms")) nobraces = !plc_opt_is_true(o);
            else if (on && !strcmp(on, "$lists")) keeplists = plc_opt_is_true(o);
            else if (on && !strcmp(on, "numbervars")) numbervars = plc_opt_is_true(o);
            else if (on && !strcmp(on, "portable") && rt_pl_dialect_xsb()) portable = plc_opt_is_true(o);
            else if (on && !strcmp(on, "float_precision") && rt_pl_dialect_xsb()) {
                pl_cell_t *a = pl_deref(&oa[0]);
                if ((int)a->v == DT_I && a->i >= 0) m.fprec = (int)a->i;
            } else if (on && !strcmp(on, "float_specifier") && rt_pl_dialect_xsb()) {
                pl_cell_t *a = pl_deref(&oa[0]);
                const char *an = (int)a->v == DT_PLATOM ? plc_atom_text(a) : (const char *)0;
                if (an && an[0] && !an[1] && strchr("feg", an[0])) m.fspec = an[0];
            } else if (on && !strcmp(on, "integer_prefix") && rt_pl_dialect_xsb()) m.ipfx = plc_opt_is_true(o);
            else if (on && (!strcmp(on, "integer_base") || !strcmp(on, "radix")) && rt_pl_dialect_xsb()) {
                pl_cell_t *a = pl_deref(&oa[0]);
                const char *an = (int)a->v == DT_PLATOM ? plc_atom_text(a) : (const char *)0;
                if ((int)a->v == DT_I) {
                    m.ibase = (int)a->i;
                    m.inamed = 0;
                } else if (an && !strcmp(an, "hex")) {
                    m.ibase = 16;
                    m.inamed = 1;
                } else if (an && !strcmp(an, "octal")) {
                    m.ibase = 8;
                    m.inamed = 1;
                } else m.ibase = 0;
            } else if (on && !strcmp(on, "space_args")) m.space_args = plc_opt_is_true(o);
            else if (on && !strcmp(on, "spacing")) {
                pl_cell_t *a = pl_deref(&oa[0]);
                m.next_arg = (int)a->v == DT_PLATOM && !strcmp(plc_atom_text(a), "next_argument");
            } else if (on && !strcmp(on, "portray_goal") && cxp) {
                m.pgoal = pl_deref(&oa[0]);
                m.popts = pl_deref((pl_cell_t *)opts_cell);
                m.cx = (pl_tr_ctx_t *)cxp;
                m.portray = plc_portray_hit;
            } else if (on && !strcmp(on, "max_depth")) {
                pl_cell_t *a = pl_deref(&oa[0]);
                if ((int)a->v == DT_I) max_depth = (long)a->i;
            } else if (on && !strcmp(on, "variable_names")) {
                m.vnc++;
                m.vnl = pl_deref((pl_cell_t *)opts_cell);
            }
        } else if ((int)o->v == DT_PLATOM && (rt_pl_dialect_swi() || rt_pl_dialect_xsb())) {
            const char *bn = plc_atom_text(o);
            if (bn && !strcmp(bn, "quoted")) quoted = 1;
            else if (bn && !strcmp(bn, "ignore_ops")) ignore_ops = 1;
            else if (bn && !strcmp(bn, "numbervars")) numbervars = 1;
            else if (bn && !strcmp(bn, "quote_non_ascii")) qna = 1;
            else if (bn && !strcmp(bn, "character_escapes_unicode")) xesc = 0;
            else if (bn && !strcmp(bn, "partial")) partial = 1;
            else if (bn && !strcmp(bn, "space_args")) m.space_args = 1;
        }
        lst = pl_deref(&pair[1]);
    }
    if (ignore_ops && rt_pl_dialect_swi()) ignore_ops = 1 | (nolists ? 0 : 2) | (nobraces ? 0 : 4);
    if (rt_pl_dialect_xsb()) { if (portable) ignore_ops = 1 | 2 | 4; m.xstr = ignore_ops != 1; m.xopf = 1; }
    if (ignore_ops == 1 && keeplists) ignore_ops = 1 | 2;
    pl_cell_t *tt = pl_deref((pl_cell_t *)term_cell);
    int br = 0, p1 = 0, p2 = 0, lm, rm;
    if (partial && (int)tt->v == DT_PLATOM) { const char *an = plc_atom_text(tt); br = (plc_op_info(an, 1, &p1, &lm, &rm) && p1 > prio) || (plc_op_info(an, 2, &p2, &lm, &rm) && p2 > prio); }
    if (br) fputc('(', m.fp);
    plc_wt((pl_cell_t *)term_cell, quoted ? (1 | (noesc ? PLC_WQ_NOESC : 0) | (qna ? PLC_WQ_NONASCII : 0) | (xesc ? PLC_WQ_XESC : 0)) : 0, ignore_ops, numbervars, max_depth, 0, prio, &m);
    if (br) fputc(')', m.fp);
    return m.pthrown;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
pl_cell_t *plc_fmt_next_arg(pl_cell_t **args) {
    pl_cell_t *a = *args ? pl_deref(*args) : (pl_cell_t *)0;
    if (a && (int)a->v == DT_PLREF && pl_arity(a) == 2) { pl_cell_t *aa = (pl_cell_t *)pl_compound_heap(a); pl_cell_t *h = pl_deref(&aa[0]); *args = pl_deref(&aa[1]); return h; }
    return (pl_cell_t *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void plc_fb_raw(plc_fb *f, const char *s, size_t n) {
    if (!n) return;
    if (f->n + n + 1 > f->cap) { size_t c = f->cap ? f->cap : 512; while (c < f->n + n + 1) c *= 2; char *nb = (char *)ct_grow(f->b, c); if (!nb) return; f->b = nb; f->cap = c; }
    memcpy(f->b + f->n, s, n);
    f->n += n;
    f->b[f->n] = '\0';
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void plc_fb_add(plc_fb *f, const char *s, size_t n) {
    size_t at = f->n;
    plc_fb_raw(f, s, n);
    for (size_t k = at; k < f->n; k++) if (f->b[k] == '\n') { f->seg = k + 1; f->nf = 0; f->cadj = 0; f->segadj = 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
size_t plc_fb_colat(plc_fb *f, size_t at) {
    size_t k = at, col = 0;
    while (k > 0 && f->b[k - 1] != '\n') k--;
    for (; k < at; k++) if (((unsigned char)f->b[k] & 0xC0u) != 0x80u) col++;
    return (size_t)((long)col + f->cadj);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void plc_fb_stop(plc_fb *f, long target) {
    long cur = (long)plc_fb_colat(f, f->n), pad = target - cur;
    if (pad < 0) f->cadj += pad;
    if (pad > 0 && f->nf == 0) {
        for (long i = 0; i < pad; i++) plc_fb_raw(f, " ", 1);
    } else if (pad > 0) {
        size_t base = f->seg, segn = f->n - f->seg;
        int nf = f->nf, fi = 0;
        char seg[segn + 1];
        if (segn) memcpy(seg, f->b + base, segn);
        f->n = base;
        if (f->b) f->b[f->n] = '\0';
        long w[nf > 0 ? nf : 1];
        for (int i = 0; i < nf; i++) w[i] = pad / nf;
        for (long r = 0; r < pad % nf; r++) w[rt_pl_dialect_xsb() ? nf - 1 - r : (nf / 2 + r) % nf]++;
        for (size_t k = 0; k <= segn; k++) {
            while (fi < nf && f->fpos[fi] - base == k) { for (long j = 0; j < w[fi]; j++) plc_fb_u8(f, (unsigned)f->fchr[fi]); fi++; }
            if (k < segn) plc_fb_raw(f, seg + k, 1);
        }
    }
    f->seg = f->n;
    f->nf = 0;
    f->segadj = 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *plc_fb_term(plc_fb *f, pl_cell_t *t, int kind, int quoted, int ignore_ops) {
    extern FILE *fh_memsink_open(char **, size_t *);
    char *bp = (char *)0;
    size_t bn = 0;
    FILE *ms = fh_memsink_open(&bp, &bn);
    plc_vmap m;
    if (!ms) return (void *)0;
    m.n = 0;
    m.vnc = 0;
    m.ibase = 0;
    m.inamed = 0;
    m.ipfx = 1;
    m.xstr = 0;
    m.fprec = -1;
    m.fspec = 0;
    m.xopf = 0;
    m.fp = ms;
    m.portray = (int (*)(pl_cell_t *, plc_vmap *))0;
    m.pthrown = (void *)0;
    m.cx = (pl_tr_ctx_t *)0;
    m.pheld = 0;
    m.pgoal = (pl_cell_t *)0;
    m.popts = (pl_cell_t *)0;
    m.space_args = 0;
    m.next_arg = 0;
    if (kind == 4) {
        extern int rt_proc_is_registered(const char *);
        m.portray = rt_proc_is_registered("portray/1") ? plc_portray_hit : (int (*)(pl_cell_t *, plc_vmap *))0;
        m.cx = f->cx;
        plc_write(t, &m);
    } else if (kind == 0) plc_write(t, &m);
    else if (kind == 1) plc_writeq(t, &m);
    else if (kind == 2) plc_write_canonical(t, &m);
    else plc_wt(t, quoted, ignore_ops, 1, -1, 0, 1200, &m);
    fclose(ms);
    plc_fb_add(f, bp ? bp : "", bn);
    return m.pthrown;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_term_string_cell(void *term_cell, void *str_cell, int as_str, pl_tr_ctx_t *cx) {
    extern FILE *fh_memsink_open(char **, size_t *);
    char *buf = (char *)0;
    size_t len = 0;
    FILE *ms = fh_memsink_open(&buf, &len);
    if (!ms) { return 0; }
    plc_vmap m;
    m.n = 0;
    m.vnc = 0;
    m.ibase = 0;
    m.inamed = 0;
    m.ipfx = 1;
    m.xstr = 0;
    m.fprec = -1;
    m.fspec = 0;
    m.xopf = 0;
    m.fp = ms;
    m.portray = (int (*)(pl_cell_t *, plc_vmap *))0;
    m.pthrown = (void *)0;
    m.cx = (pl_tr_ctx_t *)0;
    m.pheld = 0;
    m.pgoal = (pl_cell_t *)0;
    m.popts = (pl_cell_t *)0;
    m.space_args = 0;
    m.next_arg = 0;
    plc_writeq((pl_cell_t *)term_cell, &m);
    if (fclose(ms) != 0) { return 0; }
    return plc_text_out(str_cell, buf ? buf : "", as_str, cx);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
pl_cell_t plc_copy_cx(pl_cell_t *c, int atts, pl_tr_ctx_t *cx) {
    pl_cell_t out;
    plc_cp_t w;
    memset(&out, 0, sizeof out);
    plr_stk_init(&w.ag);
    plr_stk_init(&w.lg);
    w.s = c;
    w.d = &out;
    w.atts = atts;
    w.cx = cx;
    w.nfr = 0;
    memset(w.cc, 0, sizeof w.cc);
    (void)plc_copy_run(&w);
    return out;
}
pl_cell_t plc_copy_x(pl_cell_t *c, int atts) { return plc_copy_cx(c, atts, (pl_tr_ctx_t *)0); }
pl_cell_t plc_copy(pl_cell_t *c) { return plc_copy_x(c, 0); }
static pl_cell_t plc_copy_shared(pl_cell_t *c, int atts, pl_tr_ctx_t *cx) {
    pl_cell_t out;
    plc_cp_t w;
    memset(&out, 0, sizeof out);
    plr_stk_init(&w.ag);
    plr_stk_init(&w.lg);
    w.s = c;
    w.d = &out;
    w.atts = atts;
    w.cx = cx;
    w.nfr = 0;
    memset(w.cc, 0, sizeof w.cc);
    (void)plc_copy_share_run(&w);
    return out;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
pl_cell_t rt_pl_cell_snapshot(void *cell) { return plc_copy((pl_cell_t *)cell); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_copy_term_cell(void *term_cell, void *copy_cell, pl_tr_ctx_t *cx) {
    int garm = plc_guard_arm();
    pl_cell_t copy = plc_copy_cx((pl_cell_t *)term_cell, 1, cx);
    if (plc_guard_refused(garm, cx)) return 0;
    if (!plc_unify_cells_cx((pl_cell_t *)copy_cell, &copy, cx)) { return 0; }
    return 1;
}
void rt_pl_undo_goal_copy(DESCR_t *g, DESCR_t *out, pl_tr_ctx_t *cx) { *out = plc_copy_cx((pl_cell_t *)g, 1, cx); }
void rt_pl_residue_recopy(DESCR_t *el, int n, pl_tr_ctx_t *cx) { if (pl_tr_residue_on(cx)) for (int i = 0; i < n; i++) el[i] = plc_copy_cx((pl_cell_t *)&el[i], 1, cx); }
int rt_pl_copy_term_share_cell(void *term_cell, void *copy_cell, pl_tr_ctx_t *cx) {
    if (!rt_pl_dialect_swi()) return rt_pl_copy_term_cell(term_cell, copy_cell, cx);
    int garm = plc_guard_arm();
    pl_cell_t copy = plc_copy_shared((pl_cell_t *)term_cell, 1, cx);
    if (plc_guard_refused(garm, cx)) return 0;
    return plc_unify_cells_cx((pl_cell_t *)copy_cell, &copy, cx) ? 1 : 0;
}
int rt_pl_copy_keep_cell(void *keep_cell, void *src_cell, void *dst_cell, void *atts_cell, pl_tr_ctx_t *cx) {
    pl_cell_t *a = pl_deref((pl_cell_t *)atts_cell), copy;
    if ((int)a->v != DT_I) return 0;
    for (pl_cell_t *l = pl_deref((pl_cell_t *)keep_cell); (int)l->v == DT_PLREF && plc_fid_arity(l->slen) == 2; l = pl_deref(&((pl_cell_t *)l->p)[1])) {
        pl_cell_t *v = pl_deref(&((pl_cell_t *)l->p)[0]);
        if ((int)v->v == DT_PLVAR && v->p == (void *)v) v->v = PLR_VCOPY;
    }
    int garm = plc_guard_arm();
    copy = plc_copy_shared((pl_cell_t *)src_cell, a->i != 0, cx);
    int gref = plc_guard_refused(garm, cx);
    for (pl_cell_t *l = pl_deref((pl_cell_t *)keep_cell); (int)l->v == DT_PLREF && plc_fid_arity(l->slen) == 2; l = pl_deref(&((pl_cell_t *)l->p)[1])) {
        pl_cell_t *v = pl_deref(&((pl_cell_t *)l->p)[0]);
        if (v->v == PLR_VCOPY && v->p == (void *)v) v->v = (DTYPE_t)DT_PLVAR;
    }
    if (gref) return 0;
    return plc_unify_cells_cx((pl_cell_t *)dst_cell, &copy, cx) ? 1 : 0;
}
