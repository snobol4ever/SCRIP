#include "lower_snobol4.h"
#include "ct_arena.h"
#include "ct_vec.h"
#include "ast.h"
#include "../parsers/icon/icon_lex.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef struct {
    FILE *out;
    int   lines;
    int   in_stmt;
    const char *pending_label;
    cv_t  break_lbl;
    cv_t  cont_lbl;
    int   loop_top;
    int   if_seq;
    char  linebuf[16384];
    int   linelen;
    int   last_was_return;
} core_ctx_t;
static void emit(core_ctx_t *c, const char *fmt, ...);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void loop_push(core_ctx_t *c, const char *brk, const char *cont) {
    cv_reserve(&c->break_lbl, (uint32_t) sizeof(const char *), (uint64_t) c->loop_top + 1, "break_lbl");
    cv_reserve(&c->cont_lbl, (uint32_t) sizeof(const char *), (uint64_t) c->loop_top + 1, "cont_lbl");
    CV_AT(c->break_lbl, const char *, c->loop_top) = brk;
    CV_AT(c->cont_lbl, const char *, c->loop_top) = cont;
    c->loop_top++;
}
static void emit_nl(core_ctx_t *c);
static void emit_node(core_ctx_t *c, const tree_t *n);
static void emit_stmt(core_ctx_t *c, const tree_t *stmt);
static void codegen_program(core_ctx_t *c, const tree_t *prog);
#include <stdarg.h>
#define SNO_LINEBUF       16384
#define SNO_LINE_SPLIT_AT   900
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void emit(core_ctx_t *c, const char *fmt, ...) {
    va_list ap; va_start(ap, fmt);
    int remaining = SNO_LINEBUF - c->linelen - 1;
    if (remaining < 0) remaining = 0;
    int n = vsnprintf(c->linebuf + c->linelen, remaining, fmt, ap);
    va_end(ap);
    if (n > 0) {
        if (n > remaining) n = remaining;
        c->linelen += n;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int core_in_quoted(const char *buf, int upto) {
    int in_sq = 0, in_dq = 0;
    for (int i = 0; i < upto; i++) {
        char ch = buf[i];
        if (in_sq) { if (ch == '\'') in_sq = 0; continue; }
        if (in_dq) { if (ch == '"')  in_dq = 0; continue; }
        if (ch == '\'') in_sq = 1;
        else if (ch == '"') in_dq = 1;
    }
    return in_sq || in_dq;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int core_find_split(const char *buf, int len, int target) {
    if (target > len) target = len;
    for (int i = target; i >= 8; i--) {
        char ch = buf[i];
        if (ch == ' ' || ch == '\t') {
            if (!core_in_quoted(buf, i)) return i;
        }
    }
    for (int i = target + 1; i < len; i++) {
        char ch = buf[i];
        if (ch == ' ' || ch == '\t') {
            if (!core_in_quoted(buf, i)) return i;
        }
    }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void emit_nl(core_ctx_t *c) {
    int len = c->linelen;
    const char *buf = c->linebuf;
    {
        int newflag = 0;
        if (len >= 9) {
            if (memcmp(buf + len - 9, ":(RETURN)", 9) == 0) newflag = 1;
        }
        if (len >= 10) {
            if (memcmp(buf + len - 10, ":(FRETURN)", 10) == 0) newflag = 1;
            if (memcmp(buf + len - 10, ":(NRETURN)", 10) == 0) newflag = 1;
        }
        c->last_was_return = newflag;
    }
    if (len == 0) {
        fputc('\n', c->out);
        c->lines++;
        c->in_stmt = 0;
        return;
    }
    if (len <= SNO_LINE_SPLIT_AT) {
        fwrite(buf, 1, len, c->out);
        fputc('\n', c->out);
        c->lines++;
    } else {
        int start = 0;
        while (len - start > SNO_LINE_SPLIT_AT) {
            int sub_len = len - start;
            int idx = core_find_split(buf, len, start + SNO_LINE_SPLIT_AT);
            if (idx < 0 || idx <= start) {
                break;
            }
            fwrite(buf + start, 1, idx - start, c->out);
            fputc('\n', c->out);
            c->lines++;
            fputc('+', c->out);
            start = idx + 1;
        }
        if (start < len) {
            fwrite(buf + start, 1, len - start, c->out);
        }
        fputc('\n', c->out);
        c->lines++;
    }
    c->linelen = 0;
    c->linebuf[0] = '\0';
    c->in_stmt = 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *sval_or(const tree_t *n, const char *fallback) {
    if (!n) return fallback;
    if (n->v.sval) return n->v.sval;
    return fallback;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *label_of(const tree_t *n, const char *fallback) {
    if (!n) return fallback;
    if (n->n > 0 && n->c[0] && n->c[0]->v.sval) return n->c[0]->v.sval;
    if (n->v.sval) return n->v.sval;
    return fallback;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *label_sanitize(const char *raw) {
    if (!raw || !*raw) return raw;
    if (raw[0] != '_') return raw;
    static char ring[4][256];
    static int  idx = 0;
    char *buf = ring[idx]; idx = (idx + 1) & 3;
    const char *rest = raw;
    while (*rest == '_') rest++;
    if (!*rest) {
        snprintf(buf, 256, "L_");
        return buf;
    }
    if (((*rest >= 'A' && *rest <= 'Z') ||
         (*rest >= 'a' && *rest <= 'z') ||
         (*rest >= '0' && *rest <= '9'))) {
        snprintf(buf, 256, "%s_", rest);
    } else {
        snprintf(buf, 256, "L%s_", rest);
    }
    return buf;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void emit_expr(core_ctx_t *c, const tree_t *e) {
    if (!e) { emit(c, "''"); return; }
    switch (e->t) {
    case TT_QLIT: {
        const char *s = sval_or(e, "");
        int has_sq = strchr(s, '\'') != NULL;
        int has_dq = strchr(s, '"')  != NULL;
        if (!has_sq)        emit(c, "'%s'", s);
        else if (!has_dq)   emit(c, "\"%s\"", s);
        else                emit(c, "'%s'/*BOTH-QUOTES*/", s);
        break;
    }
    case TT_ILIT:
        emit(c, "%lld", e->v.ival);
        break;
    case TT_FLIT: {
        char rb[64];
        snprintf(rb, sizeof rb, "%.15g", e->v.dval);
        if (strtod(rb, NULL) != e->v.dval) snprintf(rb, sizeof rb, "%.17g", e->v.dval);
        if (!strpbrk(rb, ".eEni")) strcat(rb, ".");
        emit(c, "%s", rb);
        break;
    }
    case TT_VAR:
        emit(c, "%s", label_sanitize(sval_or(e, "?VAR?")));
        break;
    case TT_KEYWORD:
        emit(c, "&%s", sval_or(e, "?KW?"));
        break;
    case TT_NUL:
        emit(c, "''");
        break;
    case TT_MNS:         emit(c, "(-");  emit_expr(c, e->c[0]); emit(c, ")"); break;
    case TT_PLS:         emit(c, "(+");  emit_expr(c, e->c[0]); emit(c, ")"); break;
    case TT_NOT:         emit(c, "(~");  emit_expr(c, e->c[0]); emit(c, ")"); break;
    case TT_INTERROGATE: emit(c, "(?");  emit_expr(c, e->c[0]); emit(c, ")"); break;
    case TT_DEFER:       emit(c, "(*");  emit_expr(c, e->c[0]); emit(c, ")"); break;
    case TT_INDIRECT:    emit(c, "($");  emit_expr(c, e->c[0]); emit(c, ")"); break;
    case TT_NAME:        emit(c, "(.");  emit_expr(c, e->c[0]); emit(c, ")"); break;
    case TT_CAPT_CURSOR:
        emit(c, "(@"); emit_expr(c, e->c[0]); emit(c, ")");
        break;
    case TT_ADD: case TT_SUB: case TT_MUL: case TT_DIV: {
        const char *op =
            e->t == TT_ADD ? " + " :
            e->t == TT_SUB ? " - " :
            e->t == TT_MUL ? " * " :
                             " / " ;
        int i;
        if (e->n == 0) { emit(c, "0"); break; }
        if (e->n == 1) { emit_expr(c, e->c[0]); break; }
        emit(c, "(");
        emit_expr(c, e->c[0]);
        for (i = 1; i < e->n; i++) {
            emit(c, "%s", op);
            emit_expr(c, e->c[i]);
        }
        emit(c, ")");
        break;
    }
    case TT_POW: emit(c, "("); emit_expr(c, e->c[0]); emit(c, " ** "); emit_expr(c, e->c[1]); emit(c, ")"); break;
    case TT_SEQ:
    case TT_CAT: {
        int i;
        if (e->n == 0) { emit(c, "''"); break; }
        emit(c, "(");
        for (i = 0; i < e->n; i++) {
            if (i) emit(c, " ");
            emit_expr(c, e->c[i]);
        }
        emit(c, ")");
        break;
    }
    case TT_ALT: {
        int i;
        if (e->n == 0) { emit(c, "FAIL"); break; }
        emit(c, "(");
        for (i = 0; i < e->n; i++) {
            if (i) emit(c, " | ");
            emit_expr(c, e->c[i]);
        }
        emit(c, ")");
        break;
    }
    case TT_VLIST: {
        int i;
        emit(c, "(");
        for (i = 0; i < e->n; i++) {
            if (i) emit(c, ", ");
            emit_expr(c, e->c[i]);
        }
        emit(c, ")");
        break;
    }
    case TT_CAPT_COND_ASGN:
        emit(c, "("); emit_expr(c, e->c[0]); emit(c, " . "); emit_expr(c, e->c[1]); emit(c, ")");
        break;
    case TT_CAPT_IMMED_ASGN:
        emit(c, "("); emit_expr(c, e->c[0]); emit(c, " $ "); emit_expr(c, e->c[1]); emit(c, ")");
        break;
    case TT_FNC: {
        int i;
        emit(c, "%s(", label_sanitize(sval_or(e, "?FN?")));
        for (i = 0; i < e->n; i++) {
            if (i) emit(c, ",");
            emit_expr(c, e->c[i]);
        }
        emit(c, ")");
        break;
    }
    case TT_IDX: {
        int n_kids = e->n;
        const tree_t *base = e->c[0];
        int indirect = (base && base->t == TT_INDIRECT && base->n == 1);
        if (indirect) base = base->c[0];
        if (indirect) emit(c, "$");
        emit(c, "ITEM(");
        emit_expr(c, base);
        int i;
        for (i = 1; i < n_kids; i++) {
            emit(c, ",");
            emit_expr(c, e->c[i]);
        }
        emit(c, ")");
        break;
    }
    case TT_POS: case TT_RPOS: case TT_ANY: case TT_NOTANY: case TT_SPAN: case TT_BREAK: case TT_BREAKX:
    case TT_LEN: case TT_TAB: case TT_RTAB: {
        static const struct { tree_e k; const char *nm; } prim[] = {
            {TT_POS,"POS"},{TT_RPOS,"RPOS"},{TT_ANY,"ANY"},{TT_NOTANY,"NOTANY"},{TT_SPAN,"SPAN"},{TT_BREAK,"BREAK"},
            {TT_BREAKX,"BREAKX"},{TT_LEN,"LEN"},{TT_TAB,"TAB"},{TT_RTAB,"RTAB"}};
        const char *nm = "?PRIM?";
        int i;
        for (i = 0; i < (int)(sizeof prim / sizeof prim[0]); i++) if (prim[i].k == e->t) nm = prim[i].nm;
        emit(c, "%s(", nm);
        for (i = 0; i < e->n; i++) {
            if (i) emit(c, ",");
            emit_expr(c, e->c[i]);
        }
        emit(c, ")");
        break;
    }
    case TT_ARB:     emit(c, "ARB"); break;
    case TT_REM:     emit(c, "REM"); break;
    case TT_BAL:     emit(c, "BAL"); break;
    case TT_FAIL:    emit(c, "FAIL"); break;
    case TT_SUCCEED: emit(c, "SUCCEED"); break;
    case TT_ABORT:   emit(c, "ABORT"); break;
    case TT_FENCE:
        if (e->n == 0) emit(c, "FENCE");
        else { emit(c, "FENCE("); emit_expr(c, e->c[0]); emit(c, ")"); }
        break;
    case TT_ARBNO:
        if (e->n == 1) { emit(c, "ARBNO("); emit_expr(c, e->c[0]); emit(c, ")"); }
        else           { emit(c, "ARBNO()");   }
        break;
    case TT_ASSIGN:
        emit(c, "("); emit_expr(c, e->c[0]); emit(c, " = "); emit_expr(c, e->c[1]); emit(c, ")");
        break;
    case TT_SCAN:
        emit(c, "("); emit_expr(c, e->c[0]); emit(c, " ? "); emit_expr(c, e->c[1]); emit(c, ")");
        break;
    case TT_AUGOP: {
        const char *op =
            e->v.ival == TK_AUGMINUS ? " - " :
            e->v.ival == TK_AUGSTAR  ? " * " :
            e->v.ival == TK_AUGSLASH ? " / " :
            e->v.ival == TK_AUGPOW   ? " ** " :
                                       " + " ;
        emit(c, "("); emit_expr(c, e->c[0]); emit(c, " = (");
        emit_expr(c, e->c[0]); emit(c, "%s", op); emit_expr(c, e->c[1]);
        emit(c, "))");
        break;
    }
    default:
        emit(c, "'?TT_%d?'", (int)e->t);
        break;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void emit_stmt(core_ctx_t *c, const tree_t *s) {
    int i;
    const tree_t *subj = NULL, *pat = NULL, *repl = NULL;
    const tree_t *go_s = NULL, *go_f = NULL, *go_u = NULL;
    const tree_t *lbl = NULL;
    int has_eq = 0;
    if (!s) { emit_nl(c); return; }
    if (s->t == TT_STMT) {
        for (i = 0; i < s->n; i++) {
            const tree_t *ch = s->c[i];
            const char *tag = sval_or(ch, "");
            if      (!strcmp(tag, ":subj")) subj = ch->n ? ch->c[0] : NULL;
            else if (!strcmp(tag, ":pat"))  pat  = ch->n ? ch->c[0] : NULL;
            else if (!strcmp(tag, ":repl")) repl = ch->n ? ch->c[0] : NULL;
            else if (!strcmp(tag, ":eq"))   has_eq = 1;
            else if (!strcmp(tag, ":goS") || ch->t == TT_GOTO_S) go_s = ch;
            else if (!strcmp(tag, ":goF") || ch->t == TT_GOTO_F) go_f = ch;
            else if (!strcmp(tag, ":go")  || ch->t == TT_GOTO_U) go_u = ch;
            else if (!strcmp(tag, ":lbl")) lbl  = ch;
        }
    }
    if (subj && subj->t == TT_DEFINE && subj->n >= 3) {
        const char *fname = (subj->c[0] && subj->c[0]->v.sval) ? subj->c[0]->v.sval : "_fn";
        const char *proto = (subj->c[1] && subj->c[1]->v.sval) ? subj->c[1]->v.sval : "_fn()";
        char sproto[1024]; int sp = 0;
        {
            char nm[256]; int nl = 0;
            for (const char *q = proto; ; q++) {
                if (*q && *q != '(' && *q != ')' && *q != ',') { if (nl < (int)sizeof nm - 1) nm[nl++] = *q; continue; }
                nm[nl] = '\0';
                sp += snprintf(sproto + sp, sizeof sproto - (size_t)sp, "%s", nl ? label_sanitize(nm) : "");
                nl = 0;
                if (!*q || sp >= (int)sizeof sproto - 2) break;
                sproto[sp++] = *q; sproto[sp] = '\0';
            }
        }
        fname = ct_strdup(label_sanitize(fname));
        emit(c, "\tDEFINE('%s')\t:(%s_end)", sproto, fname);
        emit_nl(c);
        if (subj->c[2] && subj->c[2]->t == TT_PROGRAM && subj->c[2]->n > 0) {
            int j;
            c->pending_label = fname;
            for (j = 0; j < subj->c[2]->n; j++) emit_node(c, subj->c[2]->c[j]);
            if (c->pending_label) {
                emit(c, "%s", label_sanitize(c->pending_label));
                emit_nl(c);
                c->pending_label = NULL;
            }
        } else {
            emit(c, "%s", fname);
            emit_nl(c);
        }
        if (!c->last_was_return) {
            emit(c, "\t:(RETURN)");
            emit_nl(c);
        }
        emit(c, "%s_end", fname);
        emit_nl(c);
        return;
    }
    if (subj) {
        const char *outer_label = NULL;
        if (lbl && !c->pending_label) {
            const char *lname = label_of(lbl, NULL);
            if (lname) {
                c->pending_label = lname;
                outer_label = lname;
            }
        }
        (void)outer_label;
        if (subj->t == TT_IF && subj->n >= 2) {
            int has_else = (subj->n >= 3 && subj->c[2]);
            int seq = ++c->if_seq;
            char Lelse[32], Lendif[32];
            snprintf(Lelse,  sizeof Lelse,  "_Lelse_%04d",  seq);
            snprintf(Lendif, sizeof Lendif, "_Lendif_%04d", seq);
            if (c->pending_label) { emit(c, "%s\t", label_sanitize(c->pending_label)); c->pending_label = NULL; }
            else                  { emit(c, "\t"); }
            emit_expr(c, subj->c[0]);
            emit(c, "\t:F(%s)", label_sanitize(has_else ? Lelse : Lendif));
            emit_nl(c);
            if (subj->c[1] && subj->c[1]->t == TT_PROGRAM) {
                int j;
                for (j = 0; j < subj->c[1]->n; j++) emit_node(c, subj->c[1]->c[j]);
            } else if (subj->c[1]) {
                emit_node(c, subj->c[1]);
            }
            if (has_else) {
                emit(c, "\t:(%s)", label_sanitize(Lendif));
                emit_nl(c);
                if (subj->c[2] && subj->c[2]->t == TT_PROGRAM && subj->c[2]->n > 0) {
                    int j;
                    c->pending_label = Lelse;
                    for (j = 0; j < subj->c[2]->n; j++) emit_node(c, subj->c[2]->c[j]);
                    if (c->pending_label) {
                        emit(c, "%s", label_sanitize(c->pending_label));
                        emit_nl(c);
                        c->pending_label = NULL;
                    }
                } else {
                    emit(c, "%s", label_sanitize(Lelse));
                    emit_nl(c);
                }
            }
            emit(c, "%s", label_sanitize(Lendif));
            emit_nl(c);
            return;
        }
        if (subj->t == TT_WHILE && subj->n >= 2) {
            int seq = ++c->if_seq;
            char Ltop_buf[32], Lend_buf[32];
            const char *Ltop, *Lend;
            snprintf(Ltop_buf, sizeof Ltop_buf, "_Ltop_%04d", seq);
            snprintf(Lend_buf, sizeof Lend_buf, "_Lend_%04d", seq);
            Ltop = (subj->n >= 4 && subj->c[2]) ? sval_or(subj->c[2], Ltop_buf) : Ltop_buf;
            Lend = (subj->n >= 4 && subj->c[3]) ? sval_or(subj->c[3], Lend_buf) : Lend_buf;
            if (c->pending_label) {
                emit(c, "%s", label_sanitize(c->pending_label)); emit_nl(c);
                c->pending_label = NULL;
            }
            emit(c, "%s\t", label_sanitize(Ltop));
            emit_expr(c, subj->c[0]);
            emit(c, "\t:F(%s)", label_sanitize(Lend));
            emit_nl(c);
            loop_push(c, Lend, Ltop);
            if (subj->c[1] && subj->c[1]->t == TT_PROGRAM) {
                int j;
                for (j = 0; j < subj->c[1]->n; j++) emit_node(c, subj->c[1]->c[j]);
            } else if (subj->c[1]) {
                emit_node(c, subj->c[1]);
            }
            if (c->loop_top > 0) c->loop_top--;
            emit(c, "\t:(%s)", label_sanitize(Ltop)); emit_nl(c);
            emit(c, "%s", label_sanitize(Lend)); emit_nl(c);
            return;
        }
        if (subj->t == TT_DO_WHILE && subj->n >= 2) {
            int seq = ++c->if_seq;
            char Ltop[32], Lcont_buf[32], Lend_buf[32];
            const char *Lcont, *Lend;
            snprintf(Ltop, sizeof Ltop, "_Ldotop_%04d", seq);
            snprintf(Lcont_buf, sizeof Lcont_buf, "_Ldocont_%04d", seq);
            snprintf(Lend_buf,  sizeof Lend_buf,  "_Ldoend_%04d",  seq);
            Lcont = (subj->n >= 4 && subj->c[2]) ? sval_or(subj->c[2], Lcont_buf) : Lcont_buf;
            Lend  = (subj->n >= 4 && subj->c[3]) ? sval_or(subj->c[3], Lend_buf)  : Lend_buf;
            if (c->pending_label) {
                emit(c, "%s", label_sanitize(c->pending_label)); emit_nl(c);
                c->pending_label = NULL;
            }
            c->pending_label = NULL;
            loop_push(c, Lend, Lcont);
            {
                const char *Ltop_dup = ct_strdup(Ltop);
                c->pending_label = Ltop_dup;
                if (subj->c[0] && subj->c[0]->t == TT_PROGRAM && subj->c[0]->n > 0) {
                    int j;
                    for (j = 0; j < subj->c[0]->n; j++) emit_node(c, subj->c[0]->c[j]);
                } else if (subj->c[0]) {
                    emit_node(c, subj->c[0]);
                }
                if (c->pending_label) {
                    emit(c, "%s", label_sanitize(c->pending_label));
                    emit_nl(c);
                    c->pending_label = NULL;
                }
            }
            if (c->loop_top > 0) c->loop_top--;
            emit(c, "%s\t", label_sanitize(Lcont));
            emit_expr(c, subj->c[1]);
            emit(c, "\t:S(%s)", label_sanitize(Ltop));
            emit_nl(c);
            emit(c, "%s", label_sanitize(Lend)); emit_nl(c);
            return;
        }
        if (subj->t == TT_FOR && (subj->n == 4 || subj->n >= 5)) {
            const tree_t *init = NULL;
            const tree_t *cond, *step, *body;
            const char *Lcont, *Lend;
            int seq = ++c->if_seq;
            char Ltop[32], Lcont_buf[32], Lend_buf[32];
            snprintf(Ltop,      sizeof Ltop,      "_Lfortop_%04d",  seq);
            snprintf(Lcont_buf, sizeof Lcont_buf, "_Lforcont_%04d", seq);
            snprintf(Lend_buf,  sizeof Lend_buf,  "_Lforend_%04d",  seq);
            if (subj->n == 4) {
                init  = subj->c[0];
                cond  = subj->c[1];
                step  = subj->c[2];
                body  = subj->c[3];
                Lcont = Lcont_buf;
                Lend  = Lend_buf;
            } else {
                cond  = subj->c[0];
                step  = subj->c[1];
                body  = subj->c[2];
                Lcont = sval_or(subj->c[3], Lcont_buf);
                Lend  = sval_or(subj->c[4], Lend_buf);
            }
            if (c->pending_label) {
                emit(c, "%s", label_sanitize(c->pending_label)); emit_nl(c);
                c->pending_label = NULL;
            }
            if (init) {
                emit(c, "\t");
                emit_expr(c, init);
                emit_nl(c);
            }
            emit(c, "%s\t", label_sanitize(Ltop));
            emit_expr(c, cond);
            emit(c, "\t:F(%s)", label_sanitize(Lend));
            emit_nl(c);
            loop_push(c, Lend, Lcont);
            if (body && body->t == TT_PROGRAM) {
                int j;
                for (j = 0; j < body->n; j++) emit_node(c, body->c[j]);
            } else if (body) {
                emit_node(c, body);
            }
            if (c->loop_top > 0) c->loop_top--;
            emit(c, "%s\t", label_sanitize(Lcont));
            emit_expr(c, step);
            emit(c, "\t:(%s)", label_sanitize(Ltop));
            emit_nl(c);
            emit(c, "%s", label_sanitize(Lend)); emit_nl(c);
            return;
        }
        if (subj->t == TT_GOTO_U || subj->t == TT_GOTO_S || subj->t == TT_GOTO_F) {
            const char *tgt = label_of(subj, "L");
            if (c->pending_label) { emit(c, "%s\t", label_sanitize(c->pending_label)); c->pending_label = NULL; }
            else                  { emit(c, "\t"); }
            if      (subj->t == TT_GOTO_U) emit(c, ":(%s)",  label_sanitize(tgt));
            else if (subj->t == TT_GOTO_S) emit(c, ":S(%s)", label_sanitize(tgt));
            else                           emit(c, ":F(%s)", label_sanitize(tgt));
            emit_nl(c);
            return;
        }
        if (subj->t == TT_RETURN || subj->t == TT_PROC_FAIL || subj->t == TT_NRETURN) {
            const char *tgt = (subj->t == TT_RETURN)    ? "RETURN"
                            : (subj->t == TT_PROC_FAIL) ? "FRETURN"
                                                        : "NRETURN";
            if (c->pending_label) { emit(c, "%s\t", label_sanitize(c->pending_label)); c->pending_label = NULL; }
            else                  { emit(c, "\t"); }
            emit(c, ":(%s)", label_sanitize(tgt));
            emit_nl(c);
            return;
        }
        if (subj->t == TT_CASE && subj->n >= 1) {
            int seq = ++c->if_seq;
            int npairs = (subj->n - 1) / 2;
            int k, default_idx = -1;
            char swd_raw[32], swd[40], Lend[32];
            char (*Lcase)[32] = NULL;
            snprintf(swd_raw, sizeof swd_raw, "_swd_%04d", seq);
            snprintf(swd, sizeof swd, "%s", label_sanitize(swd_raw));
            snprintf(Lend, sizeof Lend, "_Lswend_%04d", seq);
            Lcase = (char (*)[32])ct_alloc((size_t)(npairs > 0 ? npairs : 1) * 32);
            for (k = 0; k < npairs; k++) snprintf(Lcase[k], 32, "_Lswc_%04d_%02d", seq, k);
            if (c->pending_label) {
                emit(c, "%s", label_sanitize(c->pending_label)); emit_nl(c);
                c->pending_label = NULL;
            }
            emit(c, "\t%s = ", swd);
            emit_expr(c, subj->c[0]);
            emit_nl(c);
            for (k = 0; k < npairs; k++) {
                const tree_t *val = subj->c[1 + 2 * k];
                if (val && val->t == TT_NUL) { default_idx = k; continue; }
                emit(c, "\tIDENT(%s,", swd);
                emit_expr(c, val);
                emit(c, ")\t:S(%s)", label_sanitize(Lcase[k]));
                emit_nl(c);
            }
            if (default_idx >= 0) { emit(c, "\t:(%s)", label_sanitize(Lcase[default_idx])); emit_nl(c); }
            else                  { emit(c, "\t:(%s)", label_sanitize(Lend));             emit_nl(c); }
            loop_push(c, Lend, (c->loop_top > 0) ? CV_AT(c->cont_lbl, const char *, c->loop_top - 1) : NULL);
            for (k = 0; k < npairs; k++) {
                const tree_t *body = subj->c[2 + 2 * k];
                c->pending_label = ct_strdup(Lcase[k]);
                if (body && body->t == TT_PROGRAM && body->n > 0) {
                    int j;
                    for (j = 0; j < body->n; j++) emit_node(c, body->c[j]);
                } else if (body) {
                    emit_node(c, body);
                }
                if (c->pending_label) {
                    emit(c, "%s", label_sanitize(c->pending_label)); emit_nl(c);
                    c->pending_label = NULL;
                }
                emit(c, "\t:(%s)", label_sanitize(Lend)); emit_nl(c);
            }
            if (c->loop_top > 0) c->loop_top--;
            emit(c, "%s", label_sanitize(Lend)); emit_nl(c);
            ct_drop(Lcase);
            return;
        }
        if (subj->t == TT_LOOP_BREAK || subj->t == TT_LOOP_NEXT) {
            const char *tgt = NULL;
            if (c->loop_top > 0) {
                tgt = (subj->t == TT_LOOP_BREAK)
                    ? CV_AT(c->break_lbl, const char *, c->loop_top - 1)
                    : CV_AT(c->cont_lbl, const char *, c->loop_top - 1);
            }
            if (!tgt) tgt = (subj->t == TT_LOOP_BREAK) ? "BREAK_NOLOOP" : "CONT_NOLOOP";
            if (c->pending_label) { emit(c, "%s\t", label_sanitize(c->pending_label)); c->pending_label = NULL; }
            else                  { emit(c, "\t"); }
            emit(c, ":(%s)", label_sanitize(tgt));
            emit_nl(c);
            return;
        }
    }
    if (c->pending_label) {
        emit(c, "%s\t", label_sanitize(c->pending_label));
        c->pending_label = NULL;
    } else if (lbl) {
        emit(c, "%s\t", label_sanitize(label_of(lbl, "L")));
    } else {
        emit(c, "\t");
    }
    if (subj && subj->t == TT_ASSIGN && subj->n >= 2
            && subj->c[0] && (subj->c[0]->t == TT_SEQ || subj->c[0]->t == TT_CAT)
            && subj->c[0]->n >= 2) {
        const tree_t *lhs = subj->c[0];
        emit_expr(c, lhs->c[0]);
        emit(c, "  ");
        emit_expr(c, lhs->c[1]);
        emit(c, "  =");
        if (subj->c[1]) { emit(c, "  "); emit_expr(c, subj->c[1]); }
        if (pat)  { emit(c, " "); emit_expr(c, pat); }
    } else {
        if (subj) emit_expr(c, subj);
        if (pat)  { emit(c, " "); emit_expr(c, pat); }
        if (has_eq) {
            emit(c, " =");
            if (repl) { emit(c, " "); emit_expr(c, repl); }
        } else if (repl) {
            emit(c, " = "); emit_expr(c, repl);
        }
    }
    if (go_s || go_f || go_u) {
        emit(c, "\t:");
        if (go_s) emit(c, "S(%s)", label_sanitize(label_of(go_s, "L")));
        if (go_f) emit(c, "F(%s)", label_sanitize(label_of(go_f, "L")));
        if (go_u) emit(c, "(%s)",  label_sanitize(label_of(go_u, "L")));
    }
    emit_nl(c);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void emit_node(core_ctx_t *c, const tree_t *n) {
    if (!n) return;
    switch (n->t) {
    case TT_STMT:
        emit_stmt(c, n);
        break;
    case TT_PROGRAM:
        codegen_program(c, n);
        break;
    case TT_DEFINE:
        {
            int i;
            for (i = 0; i < n->n; i++) emit_node(c, n->c[i]);
        }
        break;
    case TT_RETURN:
        emit(c, "\t:(RETURN)");
        emit_nl(c);
        break;
    case TT_PROC_FAIL:
        emit(c, "\t:(FRETURN)");
        emit_nl(c);
        break;
    case TT_NRETURN:
        emit(c, "\t:(NRETURN)");
        emit_nl(c);
        break;
    case TT_END:
        break;
    default:
        if (c->pending_label) {
            emit(c, "%s\t", label_sanitize(c->pending_label));
            c->pending_label = NULL;
        } else {
            emit(c, "\t");
        }
        emit_expr(c, n);
        emit_nl(c);
        break;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void codegen_program(core_ctx_t *c, const tree_t *prog) {
    int i;
    if (!prog) return;
    for (i = 0; i < prog->n; i++) emit_node(c, prog->c[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int tree_to_sno(const tree_t *ast, FILE *out) {
    core_ctx_t c;
    memset(&c, 0, sizeof c);
    c.out = out;
    if (!ast || !out) return -1;
    emit(&c, "* Generated by scrip --dump-sno  (SCT-1c, tree_to_sno.c)");
    emit_nl(&c);
    emit(&c, "-CASE 0");
    emit_nl(&c);
    emit(&c, "\t&FULLSCAN = 1");
    emit_nl(&c);
    if (ast->t == TT_PROGRAM) codegen_program(&c, ast);
    else                      emit_node(&c, ast);
    emit(&c, "END");
    emit_nl(&c);
    return c.lines;
}
