#include "lower_snobol4.h"
#include "ast.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define SNO_LOOP_STACK_MAX 64
typedef struct {
    FILE *out;
    int   lines;
    int   in_stmt;
    const char *pending_label;
    const char *break_lbl[SNO_LOOP_STACK_MAX];
    const char *cont_lbl [SNO_LOOP_STACK_MAX];
    int   loop_top;
    int   if_seq;
    char  linebuf[16384];
    int   linelen;
    int   last_was_return;
} core_ctx_t;
/*--------------------------------------------------------------------------------------------------------------------*/
static void emit(core_ctx_t *c, const char *fmt, ...);
static void emit_nl(core_ctx_t *c);
static void emit_node(core_ctx_t *c, const tree_t *n);
static void emit_stmt(core_ctx_t *c, const tree_t *stmt);
static void codegen_program(core_ctx_t *c, const tree_t *prog);
#include <stdarg.h>
#define SNO_LINEBUF       16384
#define SNO_LINE_SPLIT_AT   900
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
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
static const char *sval_or(const tree_t *n, const char *fallback) {
    if (!n) return fallback;
    if (n->v.sval) return n->v.sval;
    return fallback;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static const char *label_of(const tree_t *n, const char *fallback) {
    if (!n) return fallback;
    if (n->n > 0 && n->c[0] && n->c[0]->v.sval) return n->c[0]->v.sval;
    if (n->v.sval) return n->v.sval;
    return fallback;
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
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
    case TT_FLIT:
        emit(c, "%s", sval_or(e, "0.0"));
        break;
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
        emit(c, "%s(", sval_or(e, "?FN?"));
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
    default:
        emit(c, "'?TT_%d?'", (int)e->t);
        break;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
        emit(c, "\tDEFINE('%s')\t:(%s_end)", proto, fname);
        emit_nl(c);
        if (subj->c[2] && subj->c[2]->t == TT_PROGRAM && subj->c[2]->n > 0) {
            int j;
            c->pending_label = fname;
            for (j = 0; j < subj->c[2]->n; j++) emit_node(c, subj->c[2]->c[j]);
            if (c->pending_label) {
                emit(c, "%s\tOUTPUT =", label_sanitize(c->pending_label));
                emit_nl(c);
                c->pending_label = NULL;
            }
        } else {
            emit(c, "%s\tOUTPUT =", fname);
            emit_nl(c);
        }
        if (!c->last_was_return) {
            emit(c, "\t:(RETURN)");
            emit_nl(c);
        }
        emit(c, "%s_end\tOUTPUT =", fname);
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
                        emit(c, "%s\tOUTPUT =", label_sanitize(c->pending_label));
                        emit_nl(c);
                        c->pending_label = NULL;
                    }
                } else {
                    emit(c, "%s\tOUTPUT =", label_sanitize(Lelse));
                    emit_nl(c);
                }
            }
            emit(c, "%s\tOUTPUT =", label_sanitize(Lendif));
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
                emit(c, "%s\tOUTPUT =", label_sanitize(c->pending_label)); emit_nl(c);
                c->pending_label = NULL;
            }
            emit(c, "%s\t", label_sanitize(Ltop));
            emit_expr(c, subj->c[0]);
            emit(c, "\t:F(%s)", label_sanitize(Lend));
            emit_nl(c);
            if (c->loop_top < SNO_LOOP_STACK_MAX) {
                c->break_lbl[c->loop_top] = Lend;
                c->cont_lbl [c->loop_top] = Ltop;
                c->loop_top++;
            }
            if (subj->c[1] && subj->c[1]->t == TT_PROGRAM) {
                int j;
                for (j = 0; j < subj->c[1]->n; j++) emit_node(c, subj->c[1]->c[j]);
            } else if (subj->c[1]) {
                emit_node(c, subj->c[1]);
            }
            if (c->loop_top > 0) c->loop_top--;
            emit(c, "\t:(%s)", label_sanitize(Ltop)); emit_nl(c);
            emit(c, "%s\tOUTPUT =", label_sanitize(Lend)); emit_nl(c);
            return;
        }
        if (subj->t == TT_DO_WHILE && subj->n >= 4) {
            const char *Lcont = sval_or(subj->c[2], "_Lcont");
            const char *Lend  = sval_or(subj->c[3], "_Lend");
            int seq = ++c->if_seq;
            char Ltop[32];
            snprintf(Ltop, sizeof Ltop, "_Ldotop_%04d", seq);
            if (c->pending_label) {
                emit(c, "%s\tOUTPUT =", label_sanitize(c->pending_label)); emit_nl(c);
                c->pending_label = NULL;
            }
            c->pending_label = NULL;
            if (c->loop_top < SNO_LOOP_STACK_MAX) {
                c->break_lbl[c->loop_top] = Lend;
                c->cont_lbl [c->loop_top] = Lcont;
                c->loop_top++;
            }
            {
                const char *Ltop_dup = strdup(Ltop);
                c->pending_label = Ltop_dup;
                if (subj->c[0] && subj->c[0]->t == TT_PROGRAM && subj->c[0]->n > 0) {
                    int j;
                    for (j = 0; j < subj->c[0]->n; j++) emit_node(c, subj->c[0]->c[j]);
                } else if (subj->c[0]) {
                    emit_node(c, subj->c[0]);
                }
                if (c->pending_label) {
                    emit(c, "%s\tOUTPUT =", label_sanitize(c->pending_label));
                    emit_nl(c);
                    c->pending_label = NULL;
                }
            }
            if (c->loop_top > 0) c->loop_top--;
            emit(c, "%s\t", label_sanitize(Lcont));
            emit_expr(c, subj->c[1]);
            emit(c, "\t:S(%s)", label_sanitize(Ltop));
            emit_nl(c);
            emit(c, "%s\tOUTPUT =", label_sanitize(Lend)); emit_nl(c);
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
                emit(c, "%s\tOUTPUT =", label_sanitize(c->pending_label)); emit_nl(c);
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
            if (c->loop_top < SNO_LOOP_STACK_MAX) {
                c->break_lbl[c->loop_top] = Lend;
                c->cont_lbl [c->loop_top] = Lcont;
                c->loop_top++;
            }
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
            emit(c, "%s\tOUTPUT =", label_sanitize(Lend)); emit_nl(c);
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
        if (subj->t == TT_LOOP_BREAK || subj->t == TT_LOOP_NEXT) {
            const char *tgt = NULL;
            if (c->loop_top > 0) {
                tgt = (subj->t == TT_LOOP_BREAK)
                    ? c->break_lbl[c->loop_top - 1]
                    : c->cont_lbl [c->loop_top - 1];
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
    if (lbl && !subj && !go_s && !go_f && !go_u) {
        emit(c, "OUTPUT =");
    }
    if (go_s || go_f || go_u) {
        emit(c, "\t:");
        if (go_s) emit(c, "S(%s)", label_sanitize(label_of(go_s, "L")));
        if (go_f) emit(c, "F(%s)", label_sanitize(label_of(go_f, "L")));
        if (go_u) emit(c, "(%s)",  label_sanitize(label_of(go_u, "L")));
    }
    emit_nl(c);
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
static void codegen_program(core_ctx_t *c, const tree_t *prog) {
    int i;
    if (!prog) return;
    for (i = 0; i < prog->n; i++) emit_node(c, prog->c[i]);
}
/*--------------------------------------------------------------------------------------------------------------------*/
int tree_to_sno(const tree_t *ast, FILE *out) {
    core_ctx_t c;
    memset(&c, 0, sizeof c);
    c.out = out;
    if (!ast || !out) return -1;
    emit(&c, "* Generated by scrip --dump-sno  (SCT-1c, lower_sno.c)");
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
/*====================================================================================================================*/
/*====================================================================================================================*/
#include "lower_snobol4_internal.h"
#include "IR_interp_state.h"
#include <gc/gc.h>
#include <stdint.h>
static IR_t * wire_det_builtin1(lcx_t cx, const tree_t * arg_t, const char * fn, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
/*====================================================================================================================*/
static IR_t * nalloc(lcx_t cx, IR_e kind);
static void set_succ_fail(IR_t * n, IR_t * γ_in, IR_t * ω_in);
static IR_t * ret(IR_t * n, IR_t ** α_out, IR_t ** β_out, IR_t * α, IR_t * β);
static IR_t * lower_unhandled(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
static IR_t * lower_program(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
static IR_t * wire_seq(lcx_t cx, IR_e kind, const tree_t * const * kids, int nkids, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
static IR_t * wire_alt(lcx_t cx, IR_e kind, const tree_t * const * kids, int nkids, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
static IR_t * wire_if(lcx_t cx, const tree_t * e, int else_succeeds, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
static int flatten_seq(const tree_t * e, tree_e kind, const tree_t ** out, int cap);
static int tm(const tree_t * e, tree_e kind, int nargs, ...);
static int tm_g(const tree_t * e, tree_e kind, const char * tag, int nargs, ...);
static int kind_is_resumable(IR_e t);
static int tt_is_relational(tree_e t);
static int tt_to_binop(tree_e t);
static IR_t * emit_leaf(lcx_t cx, IR_t * n, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
static IR_graph_t * lower_value_subgraph(lcx_t cx, const tree_t * e);
static IR_t * v_literal(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
static IR_t * v_unop(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
static IR_t * v_binop(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
static IR_t * v_to(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
static IR_t * v_if(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
static IR_t * v_conj(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
static IR_t * v_alt(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
static IR_t * v_every(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
static IR_t * v_while(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
static IR_t * v_until(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
static IR_t * v_repeat(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
static IR_t * v_not(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
static IR_t * v_assign(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
static IR_t * v_loop_break(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
static IR_t * v_loop_next(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
static IR_t * v_det_call(lcx_t cx, const tree_t * e, int allow_generator, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
/*====================================================================================================================*/
/*====================================================================================================================*/
/*====================================================================================================================*/
static int fold_concat_const(const tree_t * e, char * buf, size_t cap, size_t * len) {
    if (!e) return 0;
    if (e->t == TT_QLIT) { const char * s = e->v.sval ? e->v.sval : ""; size_t n = strlen(s); if (*len + n >= cap) return 0; memcpy(buf + *len, s, n); *len += n; return 1; }
    if (e->t == TT_SEQ && e->n == 2) return fold_concat_const(e->c[0], buf, cap, len) && fold_concat_const(e->c[1], buf, cap, len);
    return 0;
}
static IR_t * v_seq_concat_pair(lcx_t cx, const tree_t * lhs, const tree_t * rhs, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (cx.lang == IR_LANG_SNO) {
        char fbuf[8192]; size_t flen = 0;
        if (fold_concat_const(lhs, fbuf, sizeof fbuf, &flen) && fold_concat_const(rhs, fbuf, sizeof fbuf, &flen)) {
            fbuf[flen] = '\0';
            IR_t * lit = nalloc(cx, IR_LIT_S);
            if (!lit) return NULL;
            IR_LIT(lit).sval = GC_strdup(fbuf);
            set_succ_fail(lit, γ_in, ω_in);
            return ret(lit, α_out, β_out, lit, ω_in);
        }
    }
    IR_t * node = nalloc(cx, IR_SEQ);
    if (!node) return NULL;
    IR_LIT(node).dval = 1.0;
    IR_graph_t * lblk = lower_value_subgraph(cx, lhs);
    if (!lblk) return NULL;
    IR_graph_t * rblk = lower_value_subgraph(cx, rhs);
    if (!rblk) { IR_free(lblk); return NULL; }
    IR_EXEC(node).counter = (int64_t)(intptr_t) lblk;
    IR_LIT(node).ival    = (int64_t)(intptr_t) rblk;
    set_succ_fail(node, γ_in, ω_in);
    return ret(node, α_out, β_out, node  , ω_in  );
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * sno_conj(lcx_t cx, const tree_t * const * kids, int nk, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (nk == 1) return lower_program(cx, kids[0], γ_in, ω_in, α_out, β_out);
    tree_t * left = (tree_t *) kids[0];
    for (int i = 1; i < nk; i++) {
        tree_t * pair = ast_node_new(TT_SEQ);
        if (!pair) return NULL;
        ast_push(pair, left);
        ast_push(pair, (tree_t *) kids[i]);
        left = pair;
    }
    return v_seq_concat_pair(cx, left->c[0], left->c[1], γ_in, ω_in, α_out, β_out);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_e sno_assign_kind(const tree_t * rhs_t) {
    IR_e ak = IR_ASSIGN;
    if      (rhs_t->t == TT_QLIT || rhs_t->t == TT_CSET) ak = IR_ASSIGN_LIT_S;
    else if (rhs_t->t == TT_ILIT)                        ak = IR_ASSIGN_LIT_I;
    else if (rhs_t->t == TT_VAR || rhs_t->t == TT_NAME)  ak = IR_ASSIGN_VAR;
    else if (rhs_t->t == TT_SEQ || rhs_t->t == TT_SEQ_EXPR) ak = IR_ASSIGN_CONCAT;
    else if (rhs_t->t == TT_FNC)                          ak = IR_ASSIGN_CALL;
    return ak;
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * sno_fnc_call(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    IR_t * call = nalloc(cx, IR_CALL); if (!call) return NULL;
    IR_LIT(call).sval = e->v.sval ? e->v.sval : "";
    IR_LIT(call).ival = e->n;
    IR_LIT(call).dval = (e->v.sval && !strcmp(e->v.sval, "DEFINE")) ? 5.0 : 2.0;
    if (e->n > 0) {
        IR_graph_t ** blks = (IR_graph_t **) calloc((size_t) e->n, sizeof(IR_graph_t *));
        if (!blks) return NULL;
        for (int i = 0; i < e->n; i++) {
            blks[i] = lower_value_subgraph(cx, e->c[i]);
            if (!blks[i]) { free(blks); return NULL; }
        }
        IR_EXEC(call).counter = (int64_t)(intptr_t) blks;
    }
    set_succ_fail(call, γ_in, ω_in);
    return ret(call, α_out, β_out, call  , ω_in  );
}
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * v_scan(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    const tree_t * subj_t = NULL, * pat_t = NULL;
    if (!tm(e, TT_SCAN, 2, &subj_t, &pat_t) || !subj_t || !pat_t) return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    const tree_t * repl_t = (e->n >= 3) ? e->c[2] : NULL;
    if (repl_t && subj_t->t != TT_VAR) return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    IR_t * sc = nalloc(cx, IR_SCAN);
    if (!sc) return NULL;
    IR_graph_t * pat_blk = IR_alloc(256, IR_LANG_SNO);
    if (!pat_blk) return NULL;
    IR_t * psucc = IR_node_alloc(pat_blk, IR_SUCCEED);
    IR_t * pfail = IR_node_alloc(pat_blk, IR_FAIL);
    IR_t * pα = NULL, * pβ = NULL;
    lcx_t pcx = { pat_blk, ROLE_PATTERN, 0, IR_LANG_SNO, NULL, NULL };
    IR_t * pat_entry = lower_program(pcx, pat_t, psucc, pfail, &pα, &pβ);
    if (!pat_entry) { IR_free(pat_blk); return NULL; }
    (void) pβ;
    pat_blk->entry = pα ? pα : pat_entry;
    IR_EXEC(sc).counter = (int64_t)(intptr_t)pat_blk;
    if (repl_t) {
        IR_LIT(sc).sval = subj_t->v.sval ? subj_t->v.sval : "";
        IR_LIT(sc).ival = 1;
        IR_graph_t * subj_blk = lower_value_subgraph(cx, subj_t);
        if (!subj_blk) { IR_free(pat_blk); return NULL; }
        IR_graph_t * repl_blk = lower_value_subgraph(cx, repl_t);
        if (!repl_blk) { IR_free(pat_blk); return NULL; }
        IR_t * scan_aux[2]; scan_aux[0] = (IR_t *)(void *)subj_blk; scan_aux[1] = (IR_t *)(void *)repl_blk;
        if (!ir_operand_push(sc, scan_aux[0]) || !ir_operand_push(sc, scan_aux[1])) { IR_free(pat_blk); return NULL; }
        IR_t * rα = NULL, * rβ = NULL;
        IR_t * repln = lower_program(cx, repl_t, sc, ω_in, &rα, &rβ);
        if (!repln) { IR_free(pat_blk); return NULL; }
        (void) rβ;
        set_succ_fail(sc, γ_in, ω_in);
        return ret(sc, α_out, β_out, rα ? rα : sc, ω_in);
    }
    IR_graph_t * subj_blk = lower_value_subgraph(cx, subj_t);
    if (!subj_blk) { IR_free(pat_blk); return NULL; }
    if (subj_t->t == TT_VAR) IR_LIT(sc).sval = subj_t->v.sval ? subj_t->v.sval : "";
    IR_t * scan_aux[1]; scan_aux[0] = (IR_t *)(void *)subj_blk;
    if (!ir_operand_push(sc, scan_aux[0])) { IR_free(pat_blk); return NULL; }
    IR_t * sα = NULL, * sβ = NULL;
    IR_t * subj = lower_program(cx, subj_t, sc, ω_in, &sα, &sβ);
    if (!subj) { IR_free(pat_blk); return NULL; }
    (void) sβ;
    set_succ_fail(sc, γ_in, ω_in);
    return ret(sc, α_out, β_out, sα ? sα : sc, ω_in);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
/*====================================================================================================================*/
/*====================================================================================================================*/
/*====================================================================================================================*/
/*====================================================================================================================*/
char * cset_try_fold(const tree_t * t) { (void) t; return NULL; }
static int pat_cset_arg(const tree_t * arg, const char ** sval_out, double * varflag_out) {
    if (!arg) return 0;
    if (arg->t == TT_QLIT) { *sval_out = arg->v.sval ? arg->v.sval : ""; *varflag_out = 0.0; return 1; }
    if (arg->t == TT_VAR)  { *sval_out = arg->v.sval ? arg->v.sval : ""; *varflag_out = 1.0; return 1; }
    { char * cs = cset_try_fold(arg); if (!cs) return 0; *sval_out = cs; *varflag_out = 0.0; return 1; }
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
static int sno_seq_is_pattern(const tree_t * e) {
    if (!e) return 0;
    switch (e->t) {
    case TT_LEN: case TT_POS: case TT_RPOS: case TT_TAB: case TT_RTAB:
    case TT_SPAN: case TT_ANY: case TT_NOTANY: case TT_BREAK: case TT_BREAKX:
    case TT_ARB: case TT_REM: case TT_BAL: case TT_ARBNO: case TT_ALT:
        return 1;
    case TT_VAR:
        return (e->v.sval && (!strcmp(e->v.sval, "REM") || !strcmp(e->v.sval, "FAIL") || !strcmp(e->v.sval, "SUCCEED") ||
                              !strcmp(e->v.sval, "ARB") || !strcmp(e->v.sval, "FENCE") || !strcmp(e->v.sval, "ABORT"))) ? 1 : 0;
    case TT_SEQ: case TT_CAT: {
        for (int i = 0; i < e->n; i++) if (sno_seq_is_pattern(e->c[i])) return 1;
        return 0;
    }
    default:
        return 0;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int sno_pattern_buildable(const tree_t * e) {
    if (!e) return 0;
    if (e->t == TT_QLIT) return 1;
    if (e->t == TT_LEN || e->t == TT_POS || e->t == TT_RPOS || e->t == TT_TAB || e->t == TT_RTAB)
        return (e->n >= 1 && e->c[0] && e->c[0]->t == TT_ILIT) ? 1 : 0;
    if (e->t == TT_SPAN || e->t == TT_ANY || e->t == TT_NOTANY || e->t == TT_BREAK || e->t == TT_BREAKX)
        return (e->n >= 1 && e->c[0] && e->c[0]->t == TT_QLIT) ? 1 : 0;
    if (e->t == TT_ALT) {
        if (e->n < 2) return 0;
        for (int i = 0; i < e->n; i++) if (!sno_pattern_buildable(e->c[i])) return 0;
        return 1;
    }
    if (e->t == TT_SEQ || e->t == TT_CAT) {
        const tree_t * kids[64];
        int nk = flatten_seq(e, e->t, kids, 64);
        if (nk < 1) return 0;
        int has_pat = 0;
        for (int i = 0; i < nk; i++) {
            if (!sno_pattern_buildable(kids[i])) return 0;
            if (sno_seq_is_pattern(kids[i])) has_pat = 1;
        }
        return has_pat;
    }
    if (e->t == TT_VAR && e->v.sval &&
        (!strcmp(e->v.sval, "FAIL") || !strcmp(e->v.sval, "REM") || !strcmp(e->v.sval, "SUCCEED") ||
         !strcmp(e->v.sval, "ARB")  || !strcmp(e->v.sval, "FENCE") || !strcmp(e->v.sval, "ABORT")))
        return 1;
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_pattern_build(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    IR_t * n = NULL;
    switch (e->t) {
    case TT_QLIT:
        n = nalloc(cx, IR_PATTERN_LIT); if (!n) return NULL;
        IR_LIT(n).sval = e->v.sval ? e->v.sval : "";
        return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out);
    case TT_LEN: case TT_POS: case TT_RPOS: case TT_TAB: case TT_RTAB: {
        if (e->n < 1 || !e->c[0] || e->c[0]->t != TT_ILIT) return NULL;
        IR_e k = (e->t == TT_LEN) ? IR_PATTERN_LEN : (e->t == TT_POS) ? IR_PATTERN_POS : (e->t == TT_RPOS) ? IR_PATTERN_RPOS : (e->t == TT_TAB) ? IR_PATTERN_TAB : IR_PATTERN_RTAB;
        n = nalloc(cx, k); if (!n) return NULL;
        IR_LIT(n).ival = e->c[0]->v.ival;
        return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out);
    }
    case TT_SPAN: case TT_ANY: case TT_NOTANY: case TT_BREAK: case TT_BREAKX: {
        if (e->n < 1 || !e->c[0] || e->c[0]->t != TT_QLIT) return NULL;
        IR_e k = (e->t == TT_ANY) ? IR_PATTERN_ANY : (e->t == TT_NOTANY) ? IR_PATTERN_NOTANY : (e->t == TT_SPAN) ? IR_PATTERN_SPAN : (e->t == TT_BREAK) ? IR_PATTERN_BREAK : IR_PATTERN_BREAKX;
        n = nalloc(cx, k); if (!n) return NULL;
        IR_LIT(n).sval = e->c[0]->v.sval ? e->c[0]->v.sval : "";
        return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out);
    }
    case TT_VAR: {
        if (!e->v.sval) return NULL;
        IR_e k;
        if      (!strcmp(e->v.sval, "FAIL"))    k = IR_PATTERN_FAIL;
        else if (!strcmp(e->v.sval, "REM"))     k = IR_PATTERN_REM;
        else if (!strcmp(e->v.sval, "SUCCEED")) k = IR_PATTERN_SUCCEED;
        else if (!strcmp(e->v.sval, "ARB"))     k = IR_PATTERN_ARB;
        else if (!strcmp(e->v.sval, "FENCE"))   k = IR_PATTERN_FENCE;
        else if (!strcmp(e->v.sval, "ABORT"))   k = IR_PATTERN_ABORT;
        else return NULL;
        n = nalloc(cx, k); if (!n) return NULL;
        return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out);
    }
    case TT_ALT: {
        if (e->n < 2 || !e->c[0]) return NULL;
        IR_t * headα = NULL; IR_t * prev = NULL; IR_t * acc = NULL;
        for (int i = 0; i < e->n; i++) {
            IR_t * kα = NULL, * kβ = NULL;
            IR_t * k = lower_pattern_build(cx, e->c[i], NULL, ω_in, &kα, &kβ);
            if (!k) return NULL;
            if (prev) { prev->γ.node = kα; memcpy(prev->γ.sz, "α", 3); } else headα = kα;
            prev = k;
            if (i == 0) { acc = k; continue; }
            IR_t * a = nalloc(cx, IR_PATTERN_ALT); if (!a) return NULL;
            ir_operand_push(a, acc); ir_operand_push(a, k);
            a->ω.node = ω_in; memcpy(a->ω.sz, "α", 3);
            prev->γ.node = a; memcpy(prev->γ.sz, "α", 3); prev = a; acc = a;
        }
        prev->γ.node = NULL;
        set_succ_fail(prev, γ_in, ω_in);
        return ret(prev, α_out, β_out, headα, ω_in);
    }
    case TT_SEQ: case TT_CAT: {
        const tree_t * kids[64];
        int nk = flatten_seq(e, e->t, kids, 64);
        if (nk < 1) return NULL;
        if (nk == 1) return lower_pattern_build(cx, kids[0], γ_in, ω_in, α_out, β_out);
        IR_t * headα = NULL; IR_t * prev = NULL; IR_t * acc = NULL; IR_t * lastβ = ω_in;
        for (int i = 0; i < nk; i++) {
            IR_t * kα = NULL, * kβ = NULL;
            IR_t * k = lower_pattern_build(cx, kids[i], NULL, ω_in, &kα, &kβ);
            if (!k) return NULL;
            if (prev) { prev->γ.node = kα; memcpy(prev->γ.sz, "α", 3); } else headα = kα;
            prev = k; lastβ = kβ;
            if (i == 0) { acc = k; continue; }
            IR_t * c = nalloc(cx, IR_PATTERN_CAT); if (!c) return NULL;
            ir_operand_push(c, acc); ir_operand_push(c, k);
            c->ω.node = ω_in; memcpy(c->ω.sz, "α", 3);
            prev->γ.node = c; memcpy(prev->γ.sz, "α", 3); prev = c; acc = c;
        }
        prev->γ.node = NULL;
        set_succ_fail(prev, γ_in, ω_in);
        return ret(prev, α_out, β_out, headα, lastβ);
    }
    default:
        return NULL;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
IR_t * lower_pattern(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    IR_t * n = NULL;
    switch (e->t) {
    case TT_QLIT: n = nalloc(cx, IR_PAT_LIT); if (n) IR_LIT(n).sval = e->v.sval ? e->v.sval : ""; return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out);
    case TT_ARB:  return emit_leaf(cx, nalloc(cx, IR_PAT_ARB), γ_in, ω_in, α_out, β_out);
    case TT_REM:  return emit_leaf(cx, nalloc(cx, IR_PAT_REM), γ_in, ω_in, α_out, β_out);
    case TT_BAL:  return emit_leaf(cx, nalloc(cx, IR_PAT_BAL), γ_in, ω_in, α_out, β_out);
    case TT_SPAN: case TT_ANY: case TT_NOTANY: case TT_BREAK: case TT_BREAKX: {
        if (e->n < 1 || !e->c[0]) return NULL;
        const char * sv = NULL; double vf = 0.0;
        if (!pat_cset_arg(e->c[0], &sv, &vf)) return NULL;
        IR_e k = (e->t==TT_SPAN)?((vf!=0.0)?IR_PAT_SPAN_VAR:IR_PAT_SPAN) : (e->t==TT_ANY)?IR_PAT_ANY
               : (e->t==TT_NOTANY)?IR_PAT_NOTANY : (e->t==TT_BREAKX)?IR_PAT_BREAKX : IR_PAT_BREAK;
        n = nalloc(cx, k); if (!n) return NULL;
        IR_LIT(n).sval = sv;
        if (e->t==TT_SPAN) IR_LIT(n).ival = (vf!=0.0)?1:0;
        else               IR_LIT(n).dval = vf;
        if (e->t==TT_BREAKX) IR_LIT(n).ival = 1; else if (e->t==TT_BREAK) IR_LIT(n).ival = 0;
        return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out);
    }
    case TT_SEQ: case TT_CAT: {
        const tree_t * kids[64];
        int nk = flatten_seq(e, e->t, kids, 64);
        if (nk < 1) return NULL;
        if (nk == 1) return lower_program(cx, kids[0], γ_in, ω_in, α_out, β_out);
        return wire_seq(cx, IR_PAT_CAT, kids, nk, γ_in, ω_in, α_out, β_out);
    }
    case TT_ALT:
        if (e->n < 1) return NULL;
        return wire_alt(cx, IR_PAT_ALT, (const tree_t * const *) e->c, e->n, γ_in, ω_in, α_out, β_out);
    case TT_LEN: {
        if (e->n < 1 || !e->c[0]) return NULL;
        n = nalloc(cx, IR_PAT_LEN); if (!n) return NULL;
        if (e->c[0]->t == TT_VAR) { IR_LIT(n).sval = e->c[0]->v.sval ? e->c[0]->v.sval : ""; IR_LIT(n).dval = 1.0; }
        else { IR_LIT(n).ival = e->c[0]->v.ival; IR_LIT(n).dval = 0.0; }
        return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out);
    }
    case TT_POS: case TT_RPOS: {
        if (e->n < 1 || !e->c[0]) return NULL;
        n = nalloc(cx, IR_PAT_POS); if (!n) return NULL;
        int is_rpos = (e->t == TT_RPOS);
        if (e->c[0]->t == TT_VAR) {
            IR_LIT(n).sval = e->c[0]->v.sval ? e->c[0]->v.sval : "";
            IR_LIT(n).dval = is_rpos ? 1.0 : 2.0;
        } else {
            IR_LIT(n).ival = e->c[0]->v.ival;
            IR_LIT(n).sval = is_rpos ? "r" : NULL;
            IR_LIT(n).dval = 0.0;
        }
        lcx_t bx = cx; bx.bounded = 1;
        return emit_leaf(bx, n, γ_in, ω_in, α_out, β_out);
    }
    case TT_TAB: case TT_RTAB: {
        if (e->n < 1 || !e->c[0]) return NULL;
        int is_rtab = (e->t == TT_RTAB);
        n = nalloc(cx, is_rtab ? IR_PAT_RTAB : IR_PAT_TAB); if (!n) return NULL;
        if (e->c[0]->t == TT_VAR) {
            IR_LIT(n).sval = e->c[0]->v.sval ? e->c[0]->v.sval : "";
            IR_LIT(n).dval = is_rtab ? 1.0 : 2.0;
        } else {
            IR_LIT(n).ival = e->c[0]->v.ival;
            IR_LIT(n).sval = is_rtab ? "r" : NULL;
            IR_LIT(n).dval = 0.0;
        }
        return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out);
    }
    case TT_FENCE: {
        n = nalloc(cx, IR_PAT_FENCE); if (!n) return NULL;
        if (e->n > 0 && e->c[0]) {
            IR_t * fα = NULL, * fβ = NULL;
            lcx_t bx = cx; bx.bounded = 1;
            set_succ_fail(n, γ_in, ω_in);
            IR_t * inner = lower_program(cx, e->c[0], n, ω_in, &fα, &fβ);
            if (!inner) return NULL;
            return ret(n, α_out, β_out, fα, ω_in  );
        }
        lcx_t bx = cx; bx.bounded = 1;
        return emit_leaf(bx, n, γ_in, ω_in, α_out, β_out);
    }
    case TT_ABORT: {
        n = nalloc(cx, IR_PAT_ABORT); if (!n) return NULL;
        lcx_t bx = cx; bx.bounded = 1;
        return emit_leaf(bx, n, γ_in, ω_in, α_out, β_out);
    }
    case TT_FAIL: {
        n = nalloc(cx, IR_FAIL); if (!n) return NULL;
        lcx_t bx = cx; bx.bounded = 1;
        return emit_leaf(bx, n, γ_in, ω_in, α_out, β_out);
    }
    case TT_SUCCEED: {
        n = nalloc(cx, IR_SUCCEED); if (!n) return NULL;
        return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out);
    }
    case TT_ARBNO: {
        if (e->n < 1 || !e->c[0]) return NULL;
        int inner_cap = 64;
        IR_graph_t * inner_blk = IR_alloc(inner_cap, IR_LANG_SNO);
        if (!inner_blk) return NULL;
        IR_t * isucc = IR_node_alloc(inner_blk, IR_SUCCEED);
        IR_t * ifail = IR_node_alloc(inner_blk, IR_FAIL);
        IR_t * iα = NULL, * iβ = NULL;
        lcx_t icx = { inner_blk, ROLE_PATTERN, 0, 0 };
        IR_t * inner_entry = lower_program(icx, e->c[0], isucc, ifail, &iα, &iβ);
        if (!inner_entry) { IR_free(inner_blk); return NULL; }
        inner_blk->entry = iα;
        n = nalloc(cx, IR_PAT_ARBNO); if (!n) { IR_free(inner_blk); return NULL; }
        int stack_cap = 64;
        bb_arbno_state_t * az = (bb_arbno_state_t *)GC_MALLOC(sizeof *az);
        az->inner = inner_blk;
        az->pos_stack = (int *)GC_MALLOC((size_t)stack_cap * sizeof(int));
        az->cap = stack_cap;
        az->saved_delta = 0;
        IR_EXEC(n).counter = (int64_t)(intptr_t)az;
        return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out);
    }
    case TT_CAPT_COND_ASGN: {
        if (e->n < 1 || !e->c[0]) return NULL;
        n = nalloc(cx, IR_PAT_ASSIGN_COND); if (!n) return NULL;
        IR_LIT(n).sval = (e->n > 1 && e->c[1] && e->c[1]->v.sval) ? e->c[1]->v.sval : NULL;
        set_succ_fail(n, γ_in, ω_in);
        IR_t * iα = NULL, * iβ = NULL;
        IR_t * inner = lower_program(cx, e->c[0], n, ω_in, &iα, &iβ);
        if (!inner) return NULL;
        if (!ir_operand_push(n, iα)) return NULL;
        return ret(n, α_out, β_out, n, iβ ? iβ : ω_in);
    }
    case TT_CAPT_IMMED_ASGN: {
        if (e->n < 1 || !e->c[0]) return NULL;
        n = nalloc(cx, IR_PAT_ASSIGN_IMM); if (!n) return NULL;
        IR_LIT(n).sval = (e->n > 1 && e->c[1] && e->c[1]->v.sval) ? e->c[1]->v.sval : NULL;
        set_succ_fail(n, γ_in, ω_in);
        IR_t * iα = NULL, * iβ = NULL;
        IR_t * inner = lower_program(cx, e->c[0], n, ω_in, &iα, &iβ);
        if (!inner) return NULL;
        if (!ir_operand_push(n, iα)) return NULL;
        return ret(n, α_out, β_out, n, iβ ? iβ : ω_in);
    }
    case TT_CAPT_CURSOR: {
        if (e->n < 1 || !e->c[0] || !e->c[0]->v.sval) return NULL;
        n = nalloc(cx, IR_PAT_ATP); if (!n) return NULL;
        IR_LIT(n).sval = e->c[0]->v.sval;
        return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out);
    }
    case TT_DEFER: {
        if (e->n < 1 || !e->c[0] || !e->c[0]->v.sval) return NULL;
        n = nalloc(cx, IR_PAT_DEFER); if (!n) return NULL;
        IR_LIT(n).sval = e->c[0]->v.sval;
        IR_LIT(n).ival = 1;
        return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out);
    }
    case TT_VAR: {
        if (!e->v.sval) return NULL;
        const char * nm = e->v.sval;
        IR_e pk = (IR_e) 0; int is_prim = 1; int bnd = 0;
        if      (!strcmp(nm, "ARB"))     pk = IR_PAT_ARB;
        else if (!strcmp(nm, "REM"))     pk = IR_PAT_REM;
        else if (!strcmp(nm, "BAL"))     pk = IR_PAT_BAL;
        else if (!strcmp(nm, "ABORT"))   { pk = IR_PAT_ABORT; bnd = 1; }
        else if (!strcmp(nm, "FAIL"))    { pk = IR_FAIL;      bnd = 1; }
        else if (!strcmp(nm, "SUCCEED")) pk = IR_SUCCEED;
        else if (!strcmp(nm, "FENCE"))   { pk = IR_PAT_FENCE; bnd = 1; }
        else is_prim = 0;
        if (is_prim) {
            n = nalloc(cx, pk); if (!n) return NULL;
            lcx_t bx = cx; if (bnd) bx.bounded = 1;
            return emit_leaf(bx, n, γ_in, ω_in, α_out, β_out);
        }
        n = nalloc(cx, IR_PAT_DEFER); if (!n) return NULL;
        IR_LIT(n).sval = nm;
        IR_LIT(n).ival = 0;
        return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out);
    }
    default:
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    }
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * sno_value_shared(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    switch (e->t) {
    case TT_ILIT: case TT_FLIT: case TT_QLIT: case TT_CSET:
    case TT_NUL:  case TT_NULL: case TT_VAR:  case TT_NAME: case TT_KEYWORD:
        return v_literal(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_SUCCEED: { IR_t * n = nalloc(cx, IR_SUCCEED); if (!n) return NULL; return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out); }
    case TT_FAIL:    { IR_t * n = nalloc(cx, IR_FAIL);    if (!n) return NULL; lcx_t bx = cx; bx.bounded = 1; return emit_leaf(bx, n, γ_in, ω_in, α_out, β_out); }
    case TT_MATCH_UNARY:
        return v_unop(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_MNS: case TT_PLS: case TT_SIZE: case TT_NONNULL:
    case TT_RANDOM: case TT_CSET_COMPL: case TT_ITERATE: case TT_INTERROGATE:
        return v_unop(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_ADD: case TT_SUB: case TT_MUL: case TT_DIV: case TT_MOD: case TT_POW:
    case TT_LT:  case TT_LE:  case TT_GT:  case TT_GE:  case TT_EQ:  case TT_NE:
    case TT_CAT: case TT_LCONCAT:
    case TT_LLT: case TT_LLE: case TT_LGT: case TT_LGE: case TT_LEQ: case TT_LNE:
        return v_binop(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_TO: case TT_TO_BY:
        return v_to(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_IF:
        return v_if(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_SEQ: case TT_SEQ_EXPR:
        return v_conj(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_ALTERNATE: case TT_ALT:
        return v_alt(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_EVERY:
        return v_every(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_WHILE:
        return v_while(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_UNTIL:
        return v_until(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_REPEAT:
        return v_repeat(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_NOT:
        return v_not(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_LOOP_BREAK:
        return v_loop_break(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_LOOP_NEXT:
        return v_loop_next(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_ASSIGN:
        return v_assign(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_RETURN:
    case TT_NRETURN:
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_SUSPEND:
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_INITIAL:
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_FNC: case TT_PROC_FAIL: case TT_SWAP: case TT_AUGOP: case TT_REVASSIGN: case TT_REVSWAP: case TT_LIMIT: case TT_CASE: {
        if (e->n >= 2 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval) {
            const char * fn = e->c[0]->v.sval;
            if (e->n == 2 && (!strcmp(fn, "write") || !strcmp(fn, "writes")))
                return wire_det_builtin1(cx, e->c[1], fn, γ_in, ω_in, α_out, β_out);
        }
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    }
    case TT_SCAN:
        return v_scan(cx, e, γ_in, ω_in, α_out, β_out);
    default:
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_sno_assign(lcx_t cx, const tree_t * e, IR_t * γ, IR_t * ω, IR_t ** α, IR_t ** β) {
    const tree_t * lhs_t = NULL, * rhs_t = NULL;
    if (!tm(e, TT_ASSIGN, 2, &lhs_t, &rhs_t)) return NULL;
    if (!lhs_t || !rhs_t) return lower_unhandled(cx, e, γ, ω, α, β);
    int lhs_is_var = (lhs_t->t == TT_VAR);
    int lhs_is_kw  = (lhs_t->t == TT_KEYWORD);
    if (!lhs_is_var && !lhs_is_kw) return lower_unhandled(cx, e, γ, ω, α, β);
    if (lhs_is_var && rhs_t && rhs_t->t != TT_QLIT && sno_pattern_buildable(rhs_t)) {
        IR_t * da = nalloc(cx, IR_DTP_ASSIGN);
        if (da) {
            IR_LIT(da).sval = lhs_t->v.sval ? lhs_t->v.sval : "";
            IR_t * pα = NULL, * pβ = NULL;
            IR_t * pat = lower_pattern_build(cx, rhs_t, da, ω, &pα, &pβ);
            if (pat) {
                ir_operand_push(da, pat);
                set_succ_fail(da, γ, ω);
                return ret(da, α, β, pα ? pα : da, ω);
            }
        }
    }
    IR_e ak = IR_ASSIGN;
    if (lhs_is_var && rhs_t) ak = sno_assign_kind(rhs_t);
    IR_t * as = nalloc(cx, ak);
    if (!as) return NULL;
    IR_LIT(as).sval = lhs_t->v.sval ? lhs_t->v.sval : "";
    IR_t * rα = NULL, * rβ = NULL;
    IR_t * rhs = lower_program(cx, rhs_t, as, ω, &rα, &rβ);
    if (!rhs) return NULL;
    (void) rβ;
    set_succ_fail(as, γ, ω);
    return ret(as, α, β, rα, ω);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_sno_value(lcx_t cx, const tree_t * e, IR_t * γ, IR_t * ω, IR_t ** α, IR_t ** β) {
    switch (e->t) {
    case TT_FNC: case TT_PROC_FAIL: case TT_SWAP: case TT_AUGOP: case TT_REVASSIGN: case TT_REVSWAP: case TT_LIMIT: case TT_CASE:
        return sno_fnc_call(cx, e, γ, ω, α, β);
    case TT_SEQ: case TT_SEQ_EXPR: {
        const tree_t * kids[64]; int nk = flatten_seq(e, e->t, kids, 64);
        if (nk < 1) return NULL;
        return sno_conj(cx, kids, nk, γ, ω, α, β);
    }
    case TT_ASSIGN:
        return lower_sno_assign(cx, e, γ, ω, α, β);
    default:
        return sno_value_shared(cx, e, γ, ω, α, β);
    }
}
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * lower_sno(lcx_t cx, const tree_t * e, IR_t * γ, IR_t * ω, IR_ref_t * α, IR_ref_t * β) {
    IR_t * aα = NULL, * aβ = NULL;
    IR_t * r;
    switch (cx.role) {
    case ROLE_PATTERN: r = lower_pattern(cx, e, γ, ω, &aα, &aβ); break;
    case ROLE_VALUE:
    default:           r = lower_sno_value(cx, e, γ, ω, &aα, &aβ); break;
    }
    return iref(r, α, β, aα, aβ);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * nalloc(lcx_t cx, IR_e kind) { return IR_node_alloc(cx.bbg, kind); }
/*--------------------------------------------------------------------------------------------------------------------*/
static void set_succ_fail(IR_t * n, IR_t * γ_in, IR_t * ω_in) {
    if (!n) return;
    if (!n->γ.node && γ_in) { n->γ.node = γ_in; memcpy(n->γ.sz, "α", 3); }
    if (!n->ω.node && ω_in) { n->ω.node = ω_in; memcpy(n->ω.sz, "α", 3); }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * ret(IR_t * n, IR_t ** α_out, IR_t ** β_out, IR_t * α, IR_t * β) {
    if (α_out) *α_out = α;
    if (β_out) *β_out = β;
    return n;
}
static IR_t * lower_unhandled(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    (void) γ_in; (void) ω_in;
    fprintf(stderr, "[lower] UNHANDLED role=%d kind=%d\n", (int)cx.role, e ? (int)e->t : -1);
    return ret(NULL, α_out, β_out, NULL, NULL);
}
static IR_t * wire_seq(lcx_t cx, IR_e kind, const tree_t * const * kids, int nkids, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (nkids < 1) return NULL;
    IR_t * node = nalloc(cx, kind);
    if (!node) return NULL;
    IR_t * entry[64]; IR_t * resume[64]; IR_t * apply[64];
    if (nkids > 64) return NULL;
    for (int i = nkids - 1; i >= 0; i--) {
        if (!kids[i]) return NULL;
        IR_t * γi = (i + 1 < nkids) ? entry[i + 1] : node;
        IR_t * αi = NULL, * βi = NULL;
        IR_t * c = lower_program(cx, kids[i], γi, ω_in  , &αi, &βi);
        if (!c) return NULL;
        apply[i] = c; entry[i] = αi ? αi : c; resume[i] = βi;
    }
    for (int i = 1; i < nkids; i++) {
        IR_t * tgt = ω_in;
        for (int j = i - 1; j >= 0; j--) {
            if (resume[j] && resume[j] != ω_in) { tgt = resume[j]; break; }
        }
        apply[i]->ω.node = tgt; memcpy(apply[i]->ω.sz, "α", 3);
    }
    if (kind == IR_GCONJ) {
        bb_conj_state_t * zs = (bb_conj_state_t *)GC_MALLOC(sizeof *zs);
        if (zs) {
            zs->goals = (IR_t **)GC_MALLOC((size_t)nkids * sizeof(IR_t *));
            if (zs->goals) { for (int i = 0; i < nkids; i++) zs->goals[i] = apply[i]; zs->ngoals = nkids; IR_LIT(node).ival = (int64_t)(intptr_t)zs; }
        }
    }
    set_succ_fail(node, γ_in, ω_in);
    return ret(node, α_out, β_out, entry[0], resume[nkids - 1]);
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * wire_alt(lcx_t cx, IR_e kind, const tree_t * const * kids, int nkids, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (nkids < 1) return NULL;
    IR_t * node = nalloc(cx, kind);
    if (!node) return NULL;
    IR_t * entry[64]; IR_t * resume[64]; IR_t * apply[64];
    if (nkids > 64) return NULL;
    IR_t * arm_succ = (kind == IR_DISJ) ? γ_in : node;
    for (int j = nkids - 1; j >= 0; j--) {
        if (!kids[j]) return NULL;
        IR_t * ωj = (j + 1 < nkids) ? entry[j + 1] : ω_in;
        IR_t * αj = NULL, * βj = NULL;
        IR_t * arm = lower_program(cx, kids[j], arm_succ, ωj, &αj, &βj);
        if (!arm) return NULL;
        if (!arm->γ.node) { arm->γ.node = arm_succ; memcpy(arm->γ.sz, "α", 3); }
        apply[j] = arm; entry[j] = αj ? αj : arm; resume[j] = βj;
    }
    bb_operand_aux_set(cx.bbg, node, apply, nkids);
    set_succ_fail(node, γ_in, ω_in);
    return ret(node, α_out, β_out, entry[0], node  );
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
static int flatten_seq(const tree_t * e, tree_e kind, const tree_t ** out, int cap) {
    int n = 0;
    if (!e) return 0;
    if (e->t == kind) {
        for (int i = 0; i < e->n; i++) { int got = flatten_seq(e->c[i], kind, out + n, cap - n); n += got; if (n >= cap) break; }
        return n;
    }
    if (n < cap) out[n++] = e;
    return n;
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static int tm(const tree_t * e, tree_e kind, int nargs, ...) {
    if (!e || e->t != kind || e->n < nargs) return 0;
    va_list ap; va_start(ap, nargs);
    for (int i = 0; i < nargs; i++) { const tree_t ** slot = va_arg(ap, const tree_t **); if (slot) *slot = e->c[i]; }
    va_end(ap);
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int tm_g(const tree_t * e, tree_e kind, const char * tag, int nargs, ...) {
    if (!e || e->t != kind || e->n < nargs) return 0;
    if (tag && (!e->v.sval || strcmp(e->v.sval, tag) != 0)) return 0;
    va_list ap; va_start(ap, nargs);
    for (int i = 0; i < nargs; i++) { const tree_t ** slot = va_arg(ap, const tree_t **); if (slot) *slot = e->c[i]; }
    va_end(ap);
    return 1;
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static int kind_is_resumable(IR_e t) {
    return t == IR_TO || t == IR_TO_BY || t == IR_UPTO || t == IR_ALT || t == IR_BINOP_GEN || t == IR_ITERATE || t == IR_LIMIT || t == IR_PROC_GEN ||
           t == IR_EVERY || t == IR_REPEAT || t == IR_SUSPEND || t == IR_SCAN || t == IR_LIST_BANG || t == IR_KEY_GEN || t == IR_FIND_GEN || t == IR_SEQ_GEN || t == IR_GATHER ||
           t == IR_GEN_SCAN || t == IR_CONJ || t == IR_MAP || t == IR_GREP ||
           t == IR_PAT_LIT || t == IR_PAT_ARB || t == IR_PAT_REM || t == IR_PAT_SPAN || t == IR_PAT_SPAN_VAR || t == IR_PAT_ANY || t == IR_PAT_NOTANY ||
           t == IR_PAT_BREAK || t == IR_PAT_BREAKX || t == IR_PAT_LEN || t == IR_PAT_TAB || t == IR_PAT_RTAB || t == IR_PAT_ARBNO || t == IR_PAT_DEFER ||
           t == IR_PAT_BAL ||
           t == IR_PAT_ASSIGN_COND || t == IR_PAT_ASSIGN_IMM || t == IR_PAT_ATP;
}
static IR_t * emit_leaf(lcx_t cx, IR_t * n, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!n) return NULL;
    set_succ_fail(n, γ_in, ω_in);
    IR_t * resume = (!cx.bounded && kind_is_resumable(n->op)) ? n : ω_in;
    return ret(n, α_out, β_out, n, resume);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * v_literal(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    IR_t * n = NULL;
    switch (e->t) {
    case TT_ILIT:    n = nalloc(cx, IR_LIT_I);   if (n) IR_LIT(n).ival = e->v.ival; break;
    case TT_FLIT:    n = nalloc(cx, IR_LIT_F);   if (n) IR_LIT(n).dval = e->v.dval; break;
    case TT_QLIT:    n = nalloc(cx, IR_LIT_S);   if (n) IR_LIT(n).sval = e->v.sval ? e->v.sval : ""; break;
    case TT_CSET:    n = nalloc(cx, IR_LIT_S);   if (n) IR_LIT(n).sval = e->v.sval ? e->v.sval : ""; break;
    case TT_NUL: case TT_NULL: n = nalloc(cx, IR_LIT_NUL); break;
    case TT_VAR:     n = nalloc(cx, IR_VAR);     if (n) IR_LIT(n).sval = e->v.sval; break;
    case TT_NAME:    n = nalloc(cx, IR_VAR);     if (n) IR_LIT(n).sval = e->v.sval; break;
    case TT_KEYWORD: n = nalloc(cx, IR_KEYWORD); if (n) IR_LIT(n).sval = e->v.sval; break;
    default: return NULL;
    }
    return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * v_unop(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 1 || !e->c[0]) return NULL;
    if (e->t == TT_ITERATE) {
        IR_t * bang = nalloc(cx, IR_LIST_BANG);
        if (!bang) return NULL;
        IR_t * oα = NULL, * oβ = NULL;
        IR_t * operand = lower_program(cx, e->c[0], NULL, ω_in, &oα, &oβ);
        if (!operand) return NULL;
        if (!ir_operand_push(bang, oα ? oα : operand)) return NULL;
        set_succ_fail(bang, γ_in, ω_in);
        return ret(bang, α_out, β_out, bang, bang);
    }
    IR_t * un = nalloc(cx, IR_UNOP);
    if (!un) return NULL;
    IR_LIT(un).sval = e->v.sval;
    IR_LIT(un).ival = (int64_t) e->t;
    IR_t * eα = NULL, * eβ = NULL;
    IR_t * child = lower_program(cx, e->c[0], un  , ω_in, &eα, &eβ);
    if (!child) return NULL;
    set_succ_fail(un, γ_in, ω_in);
    return ret(un, α_out, β_out, eα, eβ);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static int tt_is_relational(tree_e t) {
    return t==TT_LT||t==TT_LE||t==TT_GT||t==TT_GE||t==TT_EQ||t==TT_NE
         ||t==TT_LLT||t==TT_LLE||t==TT_LGT||t==TT_LGE||t==TT_LEQ||t==TT_LNE;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
static int tt_to_binop(tree_e t) {
    switch (t) {
    case TT_ADD: return BINOP_ADD; case TT_SUB: return BINOP_SUB; case TT_MUL: return BINOP_MUL;
    case TT_DIV: return BINOP_DIV; case TT_MOD: return BINOP_MOD; case TT_POW: return BINOP_POW;
    case TT_LT:  return BINOP_LT;  case TT_LE:  return BINOP_LE;  case TT_GT:  return BINOP_GT;
    case TT_GE:  return BINOP_GE;  case TT_EQ:  return BINOP_EQ;  case TT_NE:  return BINOP_NE;
    case TT_CAT: case TT_LCONCAT: return BINOP_CONCAT;
    case TT_LLT: return BINOP_SLT; case TT_LLE: return BINOP_SLE; case TT_LGT: return BINOP_SGT;
    case TT_LGE: return BINOP_SGE; case TT_LEQ: return BINOP_SEQ; case TT_LNE: return BINOP_SNE;
    default:     return BINOP_ADD;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * v_binop(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    IR_t * bin = nalloc(cx, IR_BINOP);
    if (!bin) return NULL;
    IR_LIT(bin).sval = e->v.sval;
    IR_LIT(bin).ival = (int64_t) tt_to_binop(e->t);
    IR_LIT(bin).dval = tt_is_relational(e->t) ? 1.0 : 0.0;
    IR_t * e1α=NULL, * e1β=NULL, * e2α=NULL, * e2β=NULL;
    IR_t * c1 = lower_program(cx, e->c[0], NULL  , ω_in, &e1α, &e1β);
    if (!c1) return NULL;
    IR_t * c2 = lower_program(cx, e->c[1], bin  , e1β  , &e2α, &e2β);
    if (!c2) return NULL;
    if (!c1->γ.node) { c1->γ.node = e2α; memcpy(c1->γ.sz, "α", 3); }
    IR_t * binops[2] = { c1, c2 };
    bb_operand_aux_set(cx.bbg, bin, binops, 2);
    set_succ_fail(bin, γ_in, ω_in);
    return ret(bin, α_out, β_out, e1α, e2β);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static int to_by_const_step(const tree_t * s, int64_t * out_bits, int * is_real) {
    if (!s) return 0;
    if (s->t == TT_ILIT) { *out_bits = s->v.ival; *is_real = 0; return 1; }
    if (s->t == TT_FLIT) { double d = s->v.dval; memcpy(out_bits, &d, sizeof(double)); *is_real = 1; return 1; }
    if ((s->t == TT_MNS || s->t == TT_PLS) && s->n >= 1 && s->c[0]) {
        if (!to_by_const_step(s->c[0], out_bits, is_real)) return 0;
        if (s->t == TT_MNS) {
            if (*is_real) { double d; memcpy(&d, out_bits, sizeof(double)); d = -d; memcpy(out_bits, &d, sizeof(double)); }
            else *out_bits = -(*out_bits);
        }
        return 1;
    }
    return 0;
}
static IR_t * v_to(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    IR_t * node = nalloc(cx, (e->t == TT_TO_BY) ? IR_TO_BY : IR_TO);
    if (!node) return NULL;
    IR_LIT(node).sval = "ag";
    if (e->t == TT_TO_BY && e->n >= 3 && e->c[2]) {
        int64_t bits = 0; int isr = 0;
        if (to_by_const_step(e->c[2], &bits, &isr)) { IR_LIT(node).ival = bits; if (isr) IR_LIT(node).sval = "ar"; }
    }
    IR_t * fα=NULL,*fβ=NULL,*tα=NULL,*tβ=NULL;
    IR_t * lo = lower_program(cx, e->c[0], NULL  , ω_in, &fα, &fβ);
    if (!lo) return NULL;
    IR_t * hi = lower_program(cx, e->c[1], node  , fβ  , &tα, &tβ);
    if (!hi) return NULL;
    if (!lo->γ.node) { lo->γ.node = tα; memcpy(lo->γ.sz, "α", 3); }
    ir_operand_push(node, lo);
    ir_operand_push(node, hi);
    set_succ_fail(node, γ_in, ω_in);
    return ret(node, α_out, β_out, fα, node  );
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * wire_if(lcx_t cx, const tree_t * e, int else_succeeds, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    IR_t * node = nalloc(cx, IR_IF);
    if (!node) return NULL;
    lcx_t cb = cx; cb.bounded = 1;
    IR_t * c1α=NULL,*c1β=NULL, *c2α=NULL,*c2β=NULL, *c3α=NULL,*c3β=NULL;
    IR_t * thenα = NULL, * elseα = NULL;
    IR_t * c2 = lower_program(cx, e->c[1], γ_in, ω_in, &c2α, &c2β);
    if (!c2) return NULL;
    thenα = c2α;
    if (e->n >= 3 && e->c[2]) {
        IR_t * c3 = lower_program(cx, e->c[2], γ_in, ω_in, &c3α, &c3β);
        if (!c3) return NULL;
        elseα = c3α;
    } else {
        elseα = else_succeeds ? γ_in : ω_in;
    }
    IR_t * c1 = lower_program(cb, e->c[0], thenα  , elseα  , &c1α, &c1β);
    if (!c1) return NULL;
    if (!ir_operand_push(node, c1α)) return NULL;
    set_succ_fail(node, γ_in, ω_in);
    return ret(node, α_out, β_out, c1α, node  );
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * v_if(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    return wire_if(cx, e, 0, γ_in, ω_in, α_out, β_out);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static lcx_t bounded(lcx_t cx) { cx.bounded = 1; return cx; }
/*--------------------------------------------------------------------------------------------------------------------*/
static lcx_t with_loop(lcx_t cx, IR_t * lω, IR_t * lnext) { cx.loop_ω = lω; cx.loop_next = lnext; return cx; }
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_graph_t * lower_value_subgraph(lcx_t cx, const tree_t * e) {
    IR_graph_t * blk = IR_alloc(256, cx.lang);
    if (!blk) return NULL;
    IR_t * vfail = IR_node_alloc(blk, IR_FAIL);
    lcx_t vcx = cx; vcx.bbg = blk;
    IR_t * eα = NULL, * eβ = NULL;
    IR_t * en = lower_program(vcx, e, NULL, vfail, &eα, &eβ);
    if (!en) { IR_free(blk); return NULL; }
    (void) eβ;
    blk->entry = eα ? eα : en;
    return blk;
}
static IR_t * v_conj(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    const tree_t * kids[64];
    int nk = flatten_seq(e, e->t, kids, 64);
    if (nk < 1) return NULL;
    return wire_seq(cx, IR_CONJ, kids, nk, γ_in, ω_in, α_out, β_out);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * v_alt(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 1) return NULL;
    return wire_alt(cx, IR_ALT, (const tree_t * const *) e->c, e->n, γ_in, ω_in, α_out, β_out);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * v_every(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 1 || !e->c[0]) return NULL;
    IR_t * ev = nalloc(cx, IR_EVERY);
    if (!ev) return NULL;
    IR_t * g1α=NULL,*g1β=NULL;
    IR_t * gen = lower_program(cx, e->c[0], NULL  , ev  , &g1α, &g1β);
    if (!gen) return NULL;
    if (e->n >= 2 && e->c[1]) {
        IR_t * b2α=NULL,*b2β=NULL;
        IR_t * body = lower_program(bounded(cx), e->c[1], g1β  , g1β  , &b2α, &b2β);
        if (!body) return NULL;
        if (!gen->γ.node) { gen->γ.node = b2α; memcpy(gen->γ.sz, "α", 3); }
    } else {
        if (!gen->γ.node) { gen->γ.node = g1β; memcpy(gen->γ.sz, "α", 3); }
    }
    if (!ir_operand_push(ev, g1α)) return NULL;
    set_succ_fail(ev, γ_in, ω_in);
    return ret(ev, α_out, β_out, g1α, ω_in  );
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * v_while(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 1 || !e->c[0]) return NULL;
    IR_t * wh = nalloc(cx, IR_WHILE);
    if (!wh) return NULL;
    IR_t * c1α=NULL,*c1β=NULL;
    IR_t * cond = lower_program(bounded(cx), e->c[0], NULL  , wh  , &c1α, &c1β);
    if (!cond) return NULL;
    if (e->n >= 2 && e->c[1]) {
        IR_t * b2α=NULL,*b2β=NULL;
        IR_t * body = lower_program(with_loop(bounded(cx), γ_in, c1α), e->c[1], c1α  , c1α  , &b2α, &b2β);
        if (!body) return NULL;
        if (!cond->γ.node) { cond->γ.node = b2α; memcpy(cond->γ.sz, "α", 3); }
    } else {
        if (!cond->γ.node) { cond->γ.node = c1α; memcpy(cond->γ.sz, "α", 3); }
    }
    if (!ir_operand_push(wh, c1α)) return NULL;
    set_succ_fail(wh, γ_in, ω_in);
    return ret(wh, α_out, β_out, c1α, ω_in);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * v_until(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 1 || !e->c[0]) return NULL;
    IR_t * un = nalloc(cx, IR_UNTIL);
    if (!un) return NULL;
    IR_t * c1α=NULL,*c1β=NULL;
    IR_t * cond = lower_program(bounded(cx), e->c[0], un  , NULL  , &c1α, &c1β);
    if (!cond) return NULL;
    if (e->n >= 2 && e->c[1]) {
        IR_t * b2α=NULL,*b2β=NULL;
        IR_t * body = lower_program(with_loop(bounded(cx), γ_in, c1α), e->c[1], c1α  , c1α  , &b2α, &b2β);
        if (!body) return NULL;
        if (!cond->ω.node) { cond->ω.node = b2α; memcpy(cond->ω.sz, "α", 3); }
    } else {
        if (!cond->ω.node) { cond->ω.node = c1α; memcpy(cond->ω.sz, "α", 3); }
    }
    if (!ir_operand_push(un, c1α)) return NULL;
    set_succ_fail(un, γ_in, ω_in);
    return ret(un, α_out, β_out, c1α, ω_in);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * v_repeat(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 1 || !e->c[0]) return NULL;
    IR_t * rp = nalloc(cx, IR_REPEAT);
    if (!rp) return NULL;
    IR_t * eα=NULL,*eβ=NULL;
    IR_t * body = lower_program(with_loop(bounded(cx), γ_in, rp), e->c[0], rp  , rp  , &eα, &eβ);
    if (!body) return NULL;
    if (!ir_operand_push(rp, eα)) return NULL;
    set_succ_fail(rp, γ_in, ω_in);
    rp->γ.node = eα; memcpy(rp->γ.sz, "α", 3);
    return ret(rp, α_out, β_out, eα, ω_in);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * v_det_call(lcx_t cx, const tree_t * e, int allow_generator, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    IR_t * call = nalloc(cx, IR_CALL); if (!call) return NULL;
    IR_LIT(call).sval = e->c[0]->v.sval;
    int nargs = e->n - 1;
    IR_LIT(call).ival = nargs;
    IR_LIT(call).dval = 3.0;
    if (nargs > 0) {
        IR_graph_t ** blks = (IR_graph_t **) calloc((size_t) nargs, sizeof(IR_graph_t *));
        if (!blks) return NULL;
        lcx_t ac = cx; ac.role = ROLE_VALUE;
        for (int i = 0; i < nargs; i++) {
            blks[i] = lower_value_subgraph(ac, e->c[i + 1]);
            if (!blks[i]) { free(blks); return NULL; }
        }
        IR_EXEC(call).counter = (int64_t)(intptr_t) blks;
    }
    set_succ_fail(call, γ_in, ω_in);
    IR_t * call_beta = (allow_generator) ? call : ω_in;
    return ret(call, α_out, β_out, call, call_beta);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * v_loop_break(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    (void)γ_in;
    IR_t * br = nalloc(cx, IR_BREAK);
    if (!br) return NULL;
    IR_t * tgt = cx.loop_ω ? cx.loop_ω : ω_in;
    set_succ_fail(br, tgt, tgt);
    return ret(br, α_out, β_out, br, tgt);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * v_loop_next(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    (void)γ_in; (void)e;
    IR_t * nx = nalloc(cx, IR_NEXT);
    if (!nx) return NULL;
    IR_t * tgt = cx.loop_next ? cx.loop_next : ω_in;
    set_succ_fail(nx, tgt, tgt);
    return ret(nx, α_out, β_out, nx, tgt);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * v_not(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 1 || !e->c[0]) return NULL;
    IR_t * nt = nalloc(cx, IR_NOT);
    if (!nt) return NULL;
    IR_t * eα=NULL,*eβ=NULL;
    IR_t * ce = lower_program(bounded(cx), e->c[0], ω_in  , nt  , &eα, &eβ);
    if (!ce) return NULL;
    bb_operand_aux_set(cx.bbg, nt, &ce, 1);
    set_succ_fail(nt, γ_in, ω_in);
    return ret(nt, α_out, β_out, eα, ω_in  );
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * v_assign(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    const tree_t * lhs_t = NULL, * rhs_t = NULL;
    if (!tm(e, TT_ASSIGN, 2, &lhs_t, &rhs_t)) return NULL;
    if (!lhs_t || !rhs_t) return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    int lhs_is_var = (lhs_t->t == TT_VAR);
    if (!lhs_is_var) return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    IR_t * as = nalloc(cx, IR_ASSIGN);
    if (!as) return NULL;
    IR_LIT(as).sval = lhs_t->v.sval ? lhs_t->v.sval : "";
    IR_t * rα = NULL, * rβ = NULL;
    IR_t * rhs = lower_program(cx, rhs_t, as  , ω_in, &rα, &rβ);
    if (!rhs) return NULL;
    (void) rβ;
    set_succ_fail(as, γ_in, ω_in);
    return ret(as, α_out, β_out, rα, ω_in);
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_program(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!e) { if (α_out) *α_out = γ_in; if (β_out) *β_out = ω_in; return NULL; }
    IR_ref_t rα = {0}, rβ = {0};
    IR_t * r = lower_sno(cx, e, γ_in, ω_in, &rα, &rβ);
    if (α_out) *α_out = rα.node;
    if (β_out) *β_out = rβ.node;
    return r;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * wire_det_builtin1(lcx_t cx, const tree_t * arg_t, const char * fn, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!arg_t) return NULL;
    IR_t * call = nalloc(cx, IR_CALL);
    if (!call) return NULL;
    IR_LIT(call).sval = fn;
    IR_LIT(call).ival = 1;
    IR_LIT(call).dval = 1.0;
    lcx_t av = cx; av.role = ROLE_VALUE;
    IR_t * aα = NULL, * aβ = NULL;
    IR_t * a = lower_program(av, arg_t, call  , ω_in, &aα, &aβ);
    if (!a) return NULL;
    set_succ_fail(call, γ_in, ω_in);
    IR_t * call_resume = g_icn_postfix_resume ? aβ : ω_in;
    return ret(call, α_out, β_out, aα, call_resume);
}
