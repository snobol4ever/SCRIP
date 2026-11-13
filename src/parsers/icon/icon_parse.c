#include "icon_parse.h"
#include "ir/ct_arena.h"
#include "ir/ast.h"
#include "parsers/snobol4/scrip_cc.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *e_leaf_sval(tree_e k, const char *s, int len) {
    tree_t *e = ast_node_new(k);
    if (len >= 0) { e->v.sval = intern_n(s, len); e->slen = len; }
    else          { e->v.sval = intern(s); e->slen = e->v.sval ? (int)strlen(e->v.sval) : 0; }
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *e_unary(tree_e k, tree_t *child) {
    return expr_unary(k, child);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *e_binary(tree_e k, tree_t *left, tree_t *right) {
    return expr_binary(k, left, right);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *e_binary_at(int line, tree_e k, tree_t *left, tree_t *right) {
    tree_t *e = expr_binary(k, left, right);
    if (e && line > 0) e->line = line;
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *e_unary_at(int line, tree_e k, tree_t *child) {
    tree_t *e = expr_unary(k, child);
    if (e && line > 0) e->line = line;
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void parser_error(IcnParser *p, const char *msg) {
    if (!p->had_error) {
        size_t n = (size_t)fmt_len("line %d: %s (got %s)", p->cur.line, msg, icn_tk_name(p->cur.kind)); p->errmsg = (char *)ct_alloc(n);
        snprintf(p->errmsg, n, "line %d: %s (got %s)", p->cur.line, msg, icn_tk_name(p->cur.kind));
        p->had_error = 1;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void parser_oracle_error(IcnParser *p, const char *msg) {
    if (!p->had_error) { p->had_error = 1; p->errmsg = (char *)msg; p->oracle_line = p->cur.line; p->oracle_tok = icn_tk_name(p->cur.kind); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void case_sep(IcnParser *p);
static IcnToken advance(IcnParser *p) {
    p->prev_kind = p->cur.kind; p->tb_line = p->cur.line; p->tb_col = p->cur.col;
    p->cur  = p->peek;
    p->peek = icn_lex_next(p->lex);
    return p->cur;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int check(IcnParser *p, IcnTkKind kind) { return p->cur.kind == kind; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int match(IcnParser *p, IcnTkKind kind) {
    if (p->cur.kind == kind) { advance(p); return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int expect(IcnParser *p, IcnTkKind kind, const char *ctx) {
    if (p->cur.kind == kind) { advance(p); return 1; }
    char msg[fmt_len("%s: expected %s", ctx, icn_tk_name(kind))];
    snprintf(msg, sizeof(msg), "%s: expected %s", ctx, icn_tk_name(kind));
    parser_error(p, msg);
    return 0;
}
static tree_t *parse_expr(IcnParser *p);
static tree_t *parse_ctrl(IcnParser *p);
static tree_t *parse_stmt(IcnParser *p);
static tree_t *parse_do_clause(IcnParser *p);
static tree_t *parse_block_or_expr(IcnParser *p);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_begins_nexpr(IcnTkKind k) {
    switch (k) {
    case TK_INT: case TK_REAL: case TK_STRING: case TK_CSET: case TK_IDENT: case TK_COLONCOLON: case TK_SEND: case TK_SENDB: case TK_RECV: case TK_RECVB: case TK_PUNEVAL: case TK_PUNEVAL2: case TK_PSETCUR: case TK_REGEX:
    case TK_PLUS: case TK_MINUS: case TK_STAR: case TK_SLASH: case TK_CARET:
    case TK_BANG: case TK_QMARK: case TK_AT: case TK_TILDE: case TK_DOT:
    case TK_BACKSLASH: case TK_AND: case TK_BAR: case TK_CONCAT: case TK_LCONCAT:
    case TK_PLUSPLUS: case TK_MINUSMINUS: case TK_STARSTAR:
    case TK_EQ: case TK_NEQ: case TK_SEQ: case TK_SNE: case TK_IDENTICAL: case TK_NOTIDENT:
    case TK_LPAREN: case TK_LBRACE: case TK_LBRACK:
    case TK_IF: case TK_CASE: case TK_WHILE: case TK_UNTIL: case TK_REPEAT:
    case TK_EVERY: case TK_SUSPEND: case TK_RETURN: case TK_FAIL: case TK_BREAK:
    case TK_NEXT: case TK_NOT: case TK_CREATE: case TK_THREAD: case TK_CRITICAL:
        return 1;
    default:
        return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void push_child(tree_t *parent, tree_t *child) {
    expr_add_child(parent, child);
}
static tree_t *parse_table_literal(IcnParser *p, int line, tree_t *key) {
    tree_t *f = ast_node_new(TT_FNC);
    if (line > 0) f->line = line;
    push_child(f, e_leaf_sval(TT_VAR, "table", -1));
    for (;;) {
        if (!key) advance(p);
        expect(p, TK_COLON, "table literal");
        if (key) push_child(f, key);
        push_child(f, parse_expr(p));
        if (!check(p, TK_SEMICOL)) break;
        advance(p);
        key = check(p, TK_DEFAULT) ? NULL : parse_expr(p);
    }
    expect(p, TK_RBRACK, "table literal");
    return f;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *parse_primary(IcnParser *p) {
    int line = p->cur.line;
    IcnToken t = p->cur;
    if (t.kind == TK_INT) {
        advance(p);
        tree_t *e = ast_node_new(TT_ILIT);
        e->v.ival = t.val.ival;
        return e;
    }
    if (t.kind == TK_BIGINT) {
        advance(p);
        tree_t *call = ast_node_new(TT_FNC);
        push_child(call, e_leaf_sval(TT_VAR, "integer", -1));
        push_child(call, e_leaf_sval(TT_QLIT, t.val.sval.data, (int)t.val.sval.len));
        return call;
    }
    if (t.kind == TK_REAL) {
        advance(p);
        tree_t *e = ast_node_new(TT_FLIT);
        e->v.dval = t.val.fval;
        return e;
    }
    if (t.kind == TK_STRING) {
        advance(p);
        return e_leaf_sval(TT_QLIT, t.val.sval.data, (int)t.val.sval.len);
    }
    if (t.kind == TK_CSET) {
        advance(p);
        return e_leaf_sval(TT_CSET, t.val.sval.data, (int)t.val.sval.len);
    }
    if (t.kind == TK_REGEX) {
        advance(p);
        tree_t * rx = e_leaf_sval(TT_REGEX, t.val.sval.data, (int)t.val.sval.len);
        rx->line = t.line;
        return rx;
    }
    if (t.kind == TK_PUNEVAL || t.kind == TK_PUNEVAL2) {
        advance(p);
        IcnLexer sub; icn_lex_init(&sub, t.val.sval.data ? t.val.sval.data : ""); sub.unicon = p->lex->unicon;
        IcnParser sp; icn_parse_init(&sp, &sub);
        tree_t * inner = parse_expr(&sp);
        if (sp.had_error || !inner) { parser_error(p, "expected an expression between backquotes"); return NULL; }
        tree_t * df = ast_node_new(TT_DEFER); push_child(df, inner); df->line = t.line; if (t.kind == TK_PUNEVAL2) df->slen = -2;
        int nq = t.kind == TK_PUNEVAL2 ? 2 : 1; int tl = (int) t.val.sval.len; char * bq = ct_alloc((size_t) (tl + 5));
        for (int i = 0; i < nq; i++) bq[i] = '`';
        memcpy(bq + nq, t.val.sval.data, (size_t) tl);
        for (int i = 0; i < nq; i++) bq[nq + tl + i] = '`';
        bq[tl + 2 * nq] = 0;
        tree_t * tq = ast_node_new(TT_QLIT); tq->v.sval = bq; tq->slen = tl + 2 * nq; push_child(df, tq);
        return df;
    }
    if (t.kind == TK_COLONCOLON) {
        advance(p);
        if (p->cur.kind != TK_IDENT) { parser_error(p, "expected a name after ::"); return NULL; }
        char qn[fmt_len("::%s", p->cur.val.sval.data)]; snprintf(qn, sizeof(qn), "::%s", p->cur.val.sval.data);
        advance(p);
        tree_t *ev = e_leaf_sval(TT_VAR, qn, -1); ev->line = t.line; return ev;
    }
    if (t.kind == TK_IDENT) {
        advance(p);
        if (check(p, TK_COLONCOLON) && p->peek.kind == TK_IDENT) {
            advance(p);
            char qn[fmt_len("%s::%s", t.val.sval.data, p->cur.val.sval.data)]; snprintf(qn, sizeof(qn), "%s::%s", t.val.sval.data, p->cur.val.sval.data);
            advance(p);
            tree_t *ev = e_leaf_sval(TT_VAR, qn, -1); ev->line = t.line; return ev;
        }
        tree_t *ev = e_leaf_sval(TT_VAR, t.val.sval.data, (int)t.val.sval.len); ev->line = t.line; return ev;
    }
    if (t.kind == TK_AND) {
        advance(p);
        const char *kwname = NULL;
        if (p->cur.kind == TK_IDENT) {
            kwname = p->cur.val.sval.data;
        } else {
            kwname = icn_tk_name(p->cur.kind);
            int ok = 1;
            for (const char *c = kwname; *c; c++)
                if (!isalpha((unsigned char)*c) && *c != '_') { ok = 0; break; }
            if (!ok) kwname = NULL;
        }
        if (!kwname) { parser_error(p, "expected keyword name after &"); return NULL; }
        char name[fmt_len("&%s", kwname)]; snprintf(name, sizeof(name), "&%s", kwname);
        advance(p);
        tree_t *ek = e_leaf_sval(TT_VAR, name, -1); ek->line = t.line; return ek;
    }
    if (t.kind == TK_LPAREN) {
        advance(p);
        if (check(p, TK_RPAREN)) { advance(p); return ast_node_new(TT_SEQ_EXPR); }
        tree_t *first = check(p, TK_COMMA) ? ast_node_new(TT_NUL) : parse_expr(p);
        if (check(p, TK_SEMICOL)) parser_oracle_error(p, "missing right parenthesis");
        if (check(p, TK_COMMA)) {
            tree_t *seq = ast_node_new(TT_CONJ);
            push_child(seq, first);
            while (check(p, TK_COMMA)) {
                advance(p);
                if (check(p, TK_RPAREN)) { push_child(seq, ast_node_new(TT_NUL)); break; }
                if (check(p, TK_COMMA)) { push_child(seq, ast_node_new(TT_NUL)); continue; }
                push_child(seq, parse_expr(p));
            }
            expect(p, TK_RPAREN, "mutual evaluation");
            return seq;
        }
        expect(p, TK_RPAREN, "grouped expression");
        return first;
    }
    if (t.kind == TK_LBRACK) {
        advance(p);
        if (p->lex->unicon && check(p, TK_COLON)) {
            advance(p);
            tree_t *lc = ast_node_new(TT_LISTCOMP);
            if (t.line > 0) lc->line = t.line;
            push_child(lc, parse_expr(p));
            expect(p, TK_COLON, "list comprehension");
            expect(p, TK_RBRACK, "list comprehension");
            return lc;
        }
        tree_t *lst = ast_node_new(TT_MAKELIST);
        if (t.line > 0) lst->line = t.line;
        if (!check(p, TK_RBRACK)) {
            if (p->lex->unicon && check(p, TK_DEFAULT)) return parse_table_literal(p, t.line, NULL);
            tree_t *first = check(p, TK_COMMA) ? ast_node_new(TT_NUL) : parse_expr(p);
            if (p->lex->unicon && check(p, TK_COLON)) return parse_table_literal(p, t.line, first);
            push_child(lst, first);
            while (check(p, TK_COMMA)) {
                advance(p);
                if (check(p, TK_RBRACK)) { push_child(lst, ast_node_new(TT_NUL)); break; }
                if (check(p, TK_COMMA)) { push_child(lst, ast_node_new(TT_NUL)); continue; }
                push_child(lst, parse_expr(p));
            }
        }
        expect(p, TK_RBRACK, "list literal");
        return lst;
    }
    if (t.kind == TK_FAIL) {
        advance(p);
        return ast_node_new(TT_PROC_FAIL);
    }
    if (t.kind == TK_BREAK) {
        advance(p);
        tree_t *e = ast_node_new(TT_LOOP_BREAK);
        if (icn_begins_nexpr(p->cur.kind))
            push_child(e, parse_expr(p));
        return e;
    }
    if (t.kind == TK_NEXT) {
        advance(p);
        return ast_node_new(TT_LOOP_NEXT);
    }
    if (t.kind == TK_CASE   || t.kind == TK_RETURN || t.kind == TK_SUSPEND ||
        t.kind == TK_IF     || t.kind == TK_EVERY  || t.kind == TK_WHILE   ||
        t.kind == TK_UNTIL  || t.kind == TK_REPEAT || t.kind == TK_CREATE || t.kind == TK_THREAD || t.kind == TK_CRITICAL) {
        return parse_ctrl(p);
    }
    if (t.kind == TK_LBRACE) {
        return parse_block_or_expr(p);
    }
    parser_error(p, "expected expression");
    advance(p);
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *parse_postfix(IcnParser *p) {
    tree_t *n = parse_primary(p);
    if (!n) return NULL;
    for (;;) {
        int line = p->cur.line; (void)line;
        if (check(p, TK_LPAREN)) {
            advance(p);
            tree_t *call = ast_node_new(TT_FNC);
            push_child(call, n);
            if (!check(p, TK_RPAREN)) {
                do {
                    tree_t *arg;
                    if (check(p, TK_COMMA) || check(p, TK_RPAREN))
                        arg = e_leaf_sval(TT_VAR, "&null", -1);
                    else {
                        arg = parse_expr(p);
                        if (!arg) break;
                    }
                    push_child(call, arg);
                } while (match(p, TK_COMMA));
            }
            expect(p, TK_RPAREN, "function call");
            n = call;
        } else if (check(p, TK_LBRACK)) {
            advance(p);
            if (check(p, TK_RBRACK)) {
                advance(p);
                n = e_binary(TT_IDX, n, e_leaf_sval(TT_VAR, "&null", -1));
                continue;
            }
            tree_t *idx = parse_expr(p);
            if (check(p, TK_COLON)) {
                advance(p);
                tree_t *hi = parse_expr(p);
                expect(p, TK_RBRACK, "section");
                tree_t *sec = ast_node_new(TT_SECTION);
                push_child(sec, n); push_child(sec, idx); push_child(sec, hi);
                n = sec;
            } else if (check(p, TK_PLUSCOLON)) {
                advance(p);
                tree_t *len = parse_expr(p);
                expect(p, TK_RBRACK, "section+:");
                tree_t *sec = ast_node_new(TT_SECTION_PLUS);
                push_child(sec, n); push_child(sec, idx); push_child(sec, len);
                n = sec;
            } else if (check(p, TK_MINUSCOLON)) {
                advance(p);
                tree_t *len = parse_expr(p);
                expect(p, TK_RBRACK, "section-:");
                tree_t *sec = ast_node_new(TT_SECTION_MINUS);
                push_child(sec, n); push_child(sec, idx); push_child(sec, len);
                n = sec;
            } else {
                n = e_binary(TT_IDX, n, idx);
                while (check(p, TK_COMMA)) { advance(p); push_child(n, parse_expr(p)); }
                expect(p, TK_RBRACK, "subscript");
            }
        } else if (check(p, TK_LBRACE) && (p->prev_kind == TK_IDENT || p->prev_kind == TK_RPAREN || p->prev_kind == TK_STRING)) {
            advance(p);
            tree_t *call = ast_node_new(TT_FNC);
            push_child(call, n);
            tree_t *lst = ast_node_new(TT_MAKELIST);
            if (!check(p, TK_RBRACE)) {
                do {
                    tree_t *arg = (check(p, TK_COMMA) || check(p, TK_RBRACE)) ? e_leaf_sval(TT_VAR, "&null", -1) : parse_expr(p);
                    if (!arg) break;
                    tree_t *cr = ast_node_new(TT_CREATE); push_child(cr, arg); push_child(lst, cr);
                } while (match(p, TK_COMMA));
            }
            push_child(call, lst);
            expect(p, TK_RBRACE, "brace call");
            n = call;
        } else if (check(p, TK_DOT)) {
            advance(p);
            int word = p->lex->unicon && p->cur.kind >= TK_CLASS && p->cur.kind <= TK_CRITICAL;
            if (p->cur.kind != TK_IDENT && !word) { parser_error(p, "expected field name"); break; }
            IcnToken fname = p->cur;
            if (word) { fname.val.sval.data = (char *) icn_tk_name(fname.kind); fname.val.sval.len = strlen(fname.val.sval.data); }
            advance(p);
            tree_t *fe = ast_node_new(TT_FIELD);
            push_child(fe, n);
            push_child(fe, e_leaf_sval(TT_VAR, fname.val.sval.data, (int)fname.val.sval.len));
            n = fe;
        } else {
            break;
        }
    }
    return n;
}
static tree_t *parse_unary(IcnParser *p);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *icn_msg_call(const char *fn, tree_t *t, tree_t *v, int blocking) {
    tree_t *call = ast_node_new(TT_FNC);
    tree_t *flag = ast_node_new(TT_ILIT);
    flag->v.ival = blocking;
    push_child(call, e_leaf_sval(TT_VAR, fn, -1));
    push_child(call, t ? t : e_leaf_sval(TT_VAR, "&null", -1));
    push_child(call, v ? v : e_leaf_sval(TT_VAR, "&null", -1));
    push_child(call, flag);
    return call;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *parse_repalt(IcnParser *p) {
    if (check(p, TK_BAR)) { advance(p); return e_unary(TT_REPALT, parse_unary(p)); }
    if (check(p, TK_CONCAT))  { advance(p); return e_unary(TT_REPALT, e_unary(TT_REPALT, parse_unary(p))); }
    if (check(p, TK_LCONCAT)) { advance(p); return e_unary(TT_REPALT, e_unary(TT_REPALT, e_unary(TT_REPALT, parse_unary(p)))); }
    return parse_postfix(p);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *parse_limit(IcnParser *p) {
    tree_t *n = parse_unary(p);
    if (!n) return NULL;
    for (;;) {
        if (check(p, TK_BACKSLASH)) {
            advance(p);
            tree_t *lim = parse_unary(p);
            n = e_binary(TT_LIMIT, n, lim);
        } else if (check(p, TK_SEND) || check(p, TK_SENDB)) {
            int bl = check(p, TK_SENDB);
            advance(p);
            n = icn_msg_call("ICN$SEND", icn_begins_nexpr(p->cur.kind) ? parse_unary(p) : NULL, n, bl);
        } else if (check(p, TK_RECV) || check(p, TK_RECVB)) {
            int bl = check(p, TK_RECVB);
            advance(p);
            n = icn_msg_call("ICN$RECV", parse_unary(p), n, bl);
        } else break;
    }
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *parse_unary(IcnParser *p) {
    if (check(p, TK_PSETCUR)) { int ol = p->cur.line; advance(p); return e_unary_at(ol, TT_CAPT_CURSOR, parse_unary(p)); }
    int line = p->cur.line; (void)line;
    if (check(p, TK_MINUS))     { int ol = p->cur.line; advance(p); return e_unary_at(ol, TT_MNS,        parse_unary(p)); }
    if (check(p, TK_PLUS))      { int ol = p->cur.line; advance(p); return e_unary_at(ol, TT_PLS,        parse_unary(p)); }
    if (check(p, TK_BANG))      { int ol = p->cur.line; advance(p); return e_unary_at(ol, TT_ITERATE,    parse_unary(p)); }
    if (check(p, TK_STAR))      { int ol = p->cur.line; advance(p); return e_unary_at(ol, TT_SIZE,       parse_unary(p)); }
    if (check(p, TK_BACKSLASH)) { int ol = p->cur.line; advance(p); return e_unary_at(ol, TT_NONNULL,    parse_unary(p)); }
    if (check(p, TK_SLASH))     { int ol = p->cur.line; advance(p); return e_unary_at(ol, TT_NULL,       parse_unary(p)); }
    if (check(p, TK_NOT))       { int ol = p->cur.line; advance(p); return e_unary_at(ol, TT_NOT,        parse_unary(p)); }
    if (check(p, TK_QMARK))     { int ol = p->cur.line; advance(p); return e_unary_at(ol, TT_RANDOM,     parse_unary(p)); }
    if (check(p, TK_AT))        { int atline = p->cur.line; advance(p); tree_t *na = e_unary(TT_ACTIVATE, parse_unary(p)); if (na) na->line = atline; return na; }
    if (check(p, TK_SEND) || check(p, TK_SENDB)) { int bl = check(p, TK_SENDB); advance(p); return icn_msg_call("ICN$SEND", NULL, NULL, bl); }
    if (check(p, TK_RECV) || check(p, TK_RECVB)) { int bl = check(p, TK_RECVB); advance(p); return icn_msg_call("ICN$RECV", icn_begins_nexpr(p->cur.kind) ? parse_unary(p) : NULL, NULL, bl); }
    if (check(p, TK_TILDE))     { int ol = p->cur.line; advance(p); return e_unary_at(ol, TT_CSET_COMPL, parse_unary(p)); }
    if (check(p, TK_DOT))       { int ol = p->cur.line; advance(p); return e_unary_at(ol, TT_DEREF,      parse_unary(p)); }
    if (check(p, TK_PLUSPLUS))   { int ol = p->cur.line; advance(p); return e_unary_at(ol, TT_PLS, e_unary_at(ol, TT_PLS, parse_unary(p))); }
    if (check(p, TK_MINUSMINUS)) { int ol = p->cur.line; advance(p); return e_unary_at(ol, TT_MNS, e_unary_at(ol, TT_MNS, parse_unary(p))); }
    if (check(p, TK_STARSTAR))   { int ol = p->cur.line; advance(p); return e_unary_at(ol, TT_SIZE, e_unary_at(ol, TT_SIZE, parse_unary(p))); }
    if (check(p, TK_CARET)) { advance(p); tree_t *arg = parse_unary(p);
        tree_t *cfn = ast_node_new(TT_FNC); push_child(cfn, e_leaf_sval(TT_VAR, "ICN$REFRESH", -1)); push_child(cfn, arg);
        return cfn; }
    if (check(p, TK_EQ)) { advance(p); tree_t *arg = parse_unary(p);
        tree_t *mfn = ast_node_new(TT_FNC); push_child(mfn, e_leaf_sval(TT_VAR, "match", -1)); push_child(mfn, arg);
        tree_t *tfn = ast_node_new(TT_FNC); push_child(tfn, e_leaf_sval(TT_VAR, "tab", -1)); push_child(tfn, mfn);
        return tfn; }
    return parse_repalt(p);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *parse_pow(IcnParser *p) {
    tree_t *n = parse_limit(p);
    if (!n) return NULL;
    if (check(p, TK_CARET)) {
        int ol = p->cur.line;
        advance(p);
        tree_t *rhs = parse_pow(p);
        n = e_binary_at(ol, TT_POW, n, rhs);
    }
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *parse_mul(IcnParser *p) {
    tree_t *n = parse_pow(p);
    if (!n) return NULL;
    for (;;) {
        tree_e k;
        if      (check(p, TK_STAR))  k = TT_MUL;
        else if (check(p, TK_SLASH)) k = TT_DIV;
        else if (check(p, TK_MOD))   k = TT_MOD;
        else break;
        int ol = p->cur.line;
        advance(p);
        n = e_binary_at(ol, k, n, parse_pow(p));
    }
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *parse_add(IcnParser *p) {
    tree_t *n = parse_mul(p);
    if (!n) return NULL;
    for (;;) {
        tree_e k;
        if      (check(p, TK_PLUS))  k = TT_ADD;
        else if (check(p, TK_MINUS)) k = TT_SUB;
        else if (check(p, TK_PASSNM)) k = TT_CAPT_COND_ASGN;
        else if (check(p, TK_PIMD))   k = TT_CAPT_IMMED_ASGN;
        else break;
        int ol = p->cur.line;
        advance(p);
        n = e_binary_at(ol, k, n, parse_mul(p));
    }
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *parse_cset(IcnParser *p) {
    tree_t *n = parse_add(p);
    if (!n) return NULL;
    for (;;) {
        tree_e k;
        if      (check(p, TK_PLUSPLUS))   k = TT_CSET_UNION;
        else if (check(p, TK_MINUSMINUS)) k = TT_CSET_DIFF;
        else if (check(p, TK_STARSTAR))   k = TT_CSET_INTER;
        else if (check(p, TK_BANG)) {
            advance(p);
            n = e_binary(TT_BANG_BINARY, n, parse_add(p));
            continue;
        }
        else break;
        int ol = p->cur.line;
        advance(p);
        n = e_binary_at(ol, k, n, parse_add(p));
    }
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *parse_concat(IcnParser *p) {
    tree_t *n = parse_cset(p);
    if (!n) return NULL;
    for (;;) {
        tree_e k;
        if      (check(p, TK_LCONCAT)) k = TT_LCONCAT;
        else if (check(p, TK_CONCAT))  k = TT_CAT;
        else break;
        int ol = p->cur.line;
        advance(p);
        n = e_binary_at(ol, k, n, parse_cset(p));
    }
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_relop(IcnTkKind k) {
    return k==TK_LT || k==TK_LE || k==TK_GT || k==TK_GE ||
           k==TK_EQ || k==TK_NEQ ||
           k==TK_SLT || k==TK_SLE || k==TK_SGT || k==TK_SGE ||
           k==TK_SEQ || k==TK_SNE ||
           k==TK_IDENTICAL || k==TK_NOTIDENT;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_e relop_ekind(IcnTkKind k) {
    switch (k) {
        case TK_LT:  return TT_LT;   case TK_LE:  return TT_LE;
        case TK_GT:  return TT_GT;   case TK_GE:  return TT_GE;
        case TK_EQ:  return TT_EQ;   case TK_NEQ: return TT_NE;
        case TK_SLT: return TT_LLT;  case TK_SLE: return TT_LLE;
        case TK_SGT: return TT_LGT;  case TK_SGE: return TT_LGE;
        case TK_SEQ: return TT_LEQ;  case TK_SNE: return TT_LNE;
        case TK_IDENTICAL: return TT_IDENTICAL;  case TK_NOTIDENT: return TT_NIDENTICAL;
        default:     return TT_EQ;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *parse_rel(IcnParser *p) {
    tree_t *n = parse_concat(p);
    if (!n) return NULL;
    while (is_relop(p->cur.kind)) {
        tree_e k = relop_ekind(p->cur.kind);
        int ol = p->cur.line;
        advance(p);
        n = e_binary_at(ol, k, n, parse_concat(p));
    }
    return n;
}
static tree_t *parse_assign(IcnParser *p);
static tree_t *parse_scan(IcnParser *p);
static tree_t *parse_alt(IcnParser *p);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *parse_and(IcnParser *p) {
    tree_t *n = parse_scan(p);
    if (!n) return NULL;
    if (!check(p, TK_AND)) return n;
    tree_t *conj = ast_node_new(TT_CONJ);
    push_child(conj, n);
    while (check(p, TK_AND)) {
        advance(p);
        push_child(conj, parse_scan(p));
    }
    return conj;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *parse_to(IcnParser *p) {
    tree_t *n = parse_alt(p);
    if (!n) return NULL;
    while (check(p, TK_TO) || check(p, TK_POR)) {
        if (check(p, TK_POR)) { int ol = p->cur.line; advance(p); n = e_binary_at(ol, TT_ALT, n, parse_alt(p)); continue; }
        advance(p);
        tree_t *limit = parse_alt(p);
        if (check(p, TK_BY)) {
            advance(p);
            tree_t *step = parse_alt(p);
            tree_t *tby = ast_node_new(TT_TO_BY);
            push_child(tby, n); push_child(tby, limit); push_child(tby, step);
            n = tby;
        } else {
            n = e_binary(TT_TO, n, limit);
        }
    }
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *parse_alt(IcnParser *p) {
    tree_t *n = parse_rel(p);
    if (!n) return NULL;
    if (check(p, TK_PAND)) { int ol = p->cur.line; advance(p); return e_binary_at(ol, TT_PAND, n, parse_alt(p)); }
    if (!check(p, TK_BAR)) return n;
    tree_t *alt = ast_node_new(TT_ALTERNATE);
    push_child(alt, n);
    while (check(p, TK_BAR)) {
        advance(p);
        push_child(alt, parse_rel(p));
    }
    return alt;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_augop(IcnTkKind k) {
    return k==TK_AUGPLUS || k==TK_AUGMINUS || k==TK_AUGSTAR ||
           k==TK_AUGSLASH || k==TK_AUGMOD  || k==TK_AUGPOW  || k==TK_AUGCONCAT || k==TK_AUGLCONCAT ||
           k==TK_AUGCSET_UNION || k==TK_AUGCSET_DIFF || k==TK_AUGCSET_INTER ||
           k==TK_AUGSCAN || k==TK_AUGAT || k==TK_AUGIDENTICAL || k==TK_AUGNOTIDENT || k==TK_AUGAND ||
           k==TK_AUGEQ    || k==TK_AUGSEQ  ||
           k==TK_AUGLT    || k==TK_AUGLE   || k==TK_AUGGT  || k==TK_AUGGE || k==TK_AUGNE ||
           k==TK_AUGSLT   || k==TK_AUGSLE  || k==TK_AUGSGT || k==TK_AUGSGE || k==TK_AUGSNE;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *parse_pmatch(IcnParser *p) {
    tree_t *n = parse_to(p);
    if (!n) return NULL;
    while (check(p, TK_PMATCH)) { int ol = p->cur.line; advance(p); n = e_binary_at(ol, TT_PMATCH, n, parse_to(p)); }
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *parse_activate(IcnParser *p) {
    tree_t *n = parse_pmatch(p);
    if (!n) return NULL;
    while (check(p, TK_AT)) {
        int atline = p->cur.line;
        advance(p);
        n = e_binary(TT_ACTIVATE, n, parse_pmatch(p));
        if (n) n->line = atline;
    }
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *parse_assign(IcnParser *p) {
    tree_t *n = parse_activate(p);
    if (!n) return NULL;
    if (check(p, TK_ASSIGN)) {
        int ol = p->cur.line;
        advance(p);
        return e_binary_at(ol, TT_ASSIGN, n, parse_assign(p));
    }
    if (check(p, TK_REVASSIGN)) {
        int ol = p->cur.line;
        advance(p);
        return e_binary_at(ol, TT_REVASSIGN, n, parse_assign(p));
    }
    if (check(p, TK_SWAP)) {
        int ol = p->cur.line;
        advance(p);
        return e_binary_at(ol, TT_SWAP, n, parse_assign(p));
    }
    if (check(p, TK_VALSWAP)) {
        int ol = p->cur.line;
        advance(p);
        return e_binary_at(ol, TT_REVSWAP, n, parse_assign(p));
    }
    if (is_augop(p->cur.kind)) {
        IcnTkKind aug = p->cur.kind;
        int augl = p->cur.line;
        advance(p);
        tree_t *rhs = parse_assign(p);
        tree_t *op = ast_node_new(TT_AUGOP);
        AugOp_e aop = AUGOP_ADD;
        switch (aug) {
        case TK_AUGPLUS:        aop = AUGOP_ADD;       break;
        case TK_AUGMINUS:       aop = AUGOP_SUB;       break;
        case TK_AUGSTAR:        aop = AUGOP_MUL;       break;
        case TK_AUGSLASH:       aop = AUGOP_DIV;       break;
        case TK_AUGMOD:         aop = AUGOP_MOD;       break;
        case TK_AUGPOW:         aop = AUGOP_POW;       break;
        case TK_AUGCONCAT:      aop = AUGOP_CONCAT;    break;
        case TK_AUGLCONCAT:     aop = AUGOP_LCONCAT;   break;
        case TK_AUGAT:          aop = AUGOP_ACTIVATE;  break;
        case TK_AUGIDENTICAL:   aop = AUGOP_IDENTICAL; break;
        case TK_AUGNOTIDENT:    aop = AUGOP_NIDENTICAL; break;
        case TK_AUGAND:         aop = AUGOP_CONJ;      break;
        case TK_AUGCSET_UNION:  aop = AUGOP_CSET_UNION; break;
        case TK_AUGCSET_DIFF:   aop = AUGOP_CSET_DIFF;  break;
        case TK_AUGCSET_INTER:  aop = AUGOP_CSET_INTER; break;
        case TK_AUGSCAN:        aop = AUGOP_SCAN;      break;
        case TK_AUGEQ:          aop = AUGOP_EQ;        break;
        case TK_AUGSEQ:         aop = AUGOP_SEQ;       break;
        case TK_AUGLT:          aop = AUGOP_LT;        break;
        case TK_AUGLE:          aop = AUGOP_LE;        break;
        case TK_AUGGT:          aop = AUGOP_GT;        break;
        case TK_AUGGE:          aop = AUGOP_GE;        break;
        case TK_AUGNE:          aop = AUGOP_NE;        break;
        case TK_AUGSLT:         aop = AUGOP_SLT;       break;
        case TK_AUGSLE:         aop = AUGOP_SLE;       break;
        case TK_AUGSGT:         aop = AUGOP_SGT;       break;
        case TK_AUGSGE:         aop = AUGOP_SGE;       break;
        case TK_AUGSNE:         aop = AUGOP_SNE;       break;
        default:                aop = AUGOP_ADD;       break;
        }
        op->v.ival = (long)aop;
        if (augl > 0) op->line = augl;
        push_child(op, n); push_child(op, rhs);
        return op;
    }
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *parse_scan(IcnParser *p) {
    tree_t *n = parse_assign(p);
    if (!n) return NULL;
    if (check(p, TK_QMARK)) {
        advance(p);
        return e_binary(TT_SCAN, n, parse_scan(p));
    }
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *parse_expr(IcnParser *p) {
    return parse_and(p);
}
static tree_t *icn_tree_copy(const tree_t *n) {
    tree_t *r = ast_node_new(n->t);
    r->v = n->v; r->start = n->start; r->end = n->end; r->line = n->line; r->slen = n->slen;
    for (int i = 0; i < n->n; i++) push_child(r, icn_tree_copy(n->c[i]));
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *icn_unlock_of(const tree_t *m) {
    tree_t *f = ast_node_new(TT_FNC);
    push_child(f, e_leaf_sval(TT_VAR, "unlock", -1));
    push_child(f, icn_tree_copy(m));
    return f;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *icn_unlock_chain(const IcnCrit *st, int n) {
    tree_t *c = NULL;
    for (; st && n; st = st->up, n--) {
        tree_t *u = icn_unlock_of(st->m);
        if (!c) c = u;
        else { tree_t *j = ast_node_new(TT_CONJ); push_child(j, c); push_child(j, u); c = j; }
    }
    return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void icn_lock_fill(tree_t *q, const IcnCrit *st) {
    if (!st) return;
    icn_lock_fill(q, st->up);
    tree_t *f = ast_node_new(TT_FNC);
    push_child(f, e_leaf_sval(TT_VAR, "lock", -1));
    push_child(f, icn_tree_copy(st->m));
    push_child(q, f);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *icn_conj2(tree_t *a, tree_t *b) {
    tree_t *c = ast_node_new(TT_CONJ);
    push_child(c, a); push_child(c, b);
    return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *icn_unlock_after(tree_t *e, const IcnCrit *st, int n);
static int icn_crit_plain(const tree_t *e);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *icn_crit_exit(tree_t *n, const IcnCrit *st) {
    if (n->t == TT_PROC_FAIL || (n->t == TT_RETURN && n->n == 0)) return icn_conj2(icn_unlock_chain(st, -1), n);
    n->c[0] = icn_unlock_after(n->c[0], st, -1);
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *icn_unlock_after(tree_t *e, const IcnCrit *st, int n) {
    tree_t *one = ast_node_new(TT_ILIT); one->v.ival = 1;
    tree_t *me = ast_node_new(TT_FNC);
    push_child(me, one); push_child(me, e); push_child(me, icn_unlock_chain(st, n));
    if (icn_crit_plain(e)) return me;
    tree_t *one2 = ast_node_new(TT_ILIT); one2->v.ival = 1;
    tree_t *lim = ast_node_new(TT_LIMIT);
    push_child(lim, icn_conj2(icn_unlock_chain(st, n), e_leaf_sval(TT_VAR, "&fail", -1))); push_child(lim, one2);
    tree_t *alt = ast_node_new(TT_ALTERNATE);
    push_child(alt, me); push_child(alt, lim);
    return alt;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *icn_susp_rewrite(tree_t *s, const IcnCrit *st) {
    tree_t *one = ast_node_new(TT_ILIT); one->v.ival = 1;
    tree_t *me = ast_node_new(TT_FNC);
    push_child(me, one); push_child(me, s->c[0]); push_child(me, icn_unlock_chain(st, -1));
    s->c[0] = me;
    tree_t *q = ast_node_new(TT_SEQ_EXPR);
    icn_lock_fill(q, st);
    if (s->n > 1) { push_child(q, s->c[1]); s->c[1] = q; }
    else push_child(s, q);
    tree_t *b = ast_node_new(TT_SEQ_EXPR);
    push_child(b, s);
    icn_lock_fill(b, st);
    return b;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_crit_conj(tree_t *par, int i, const IcnCrit *st, int n) {
    tree_t *c = par->c[i], *pv = i > 0 ? par->c[i - 1] : NULL;
    if (par->t == TT_CONJ && i == par->n - 1 && pv && pv->t == TT_FNC && pv->n > 0 && pv->c[0]->t == TT_VAR && !strcmp(pv->c[0]->v.sval, "unlock")) {
        push_child(par, c); par->c[i] = icn_unlock_chain(st, n);
        return i + 1;
    }
    par->c[i] = icn_conj2(icn_unlock_chain(st, n), c);
    return i;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_exit_index(const tree_t *n) {
    int k = 0;
    for (; n && n->t == TT_LOOP_BREAK; n = n->n ? n->c[0] : NULL) k++;
    return k + (n && n->t == TT_LOOP_NEXT);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t **icn_break_slot(tree_t *n) {
    while (n->n && n->c[0]->t == TT_LOOP_BREAK) n = n->c[0];
    return n->n && n->c[0]->t != TT_LOOP_NEXT ? &n->c[0] : NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_crit_plain(const tree_t *e) { return e && (e->t == TT_ILIT || e->t == TT_FLIT || e->t == TT_QLIT || e->t == TT_CSET || (e->t == TT_VAR && e->v.sval && e->v.sval[0] != '&')); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_leaves(const IcnCrit *st, int depth, int k) {
    int n = 0, at = st->loops + depth;
    for (; st && k > at - st->loops; st = st->up) n++;
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void icn_crit_walk(tree_t *n, const IcnCrit *st, int depth) {
    for (int i = 0; i < n->n; i++) {
        tree_t *c = n->c[i];
        if (!c || c->t == TT_CREATE) continue;
        if (c->t == TT_FNC && c->n > 0 && c->c[0]->t == TT_VAR && c->c[0]->v.sval && !strcmp(c->c[0]->v.sval, "ICN$CRIT")) continue;
        if (c->t == TT_LOOP_BREAK) {
            tree_t **e = icn_break_slot(c), *h = ast_node_new(TT_SEQ_EXPR);
            if (e) { push_child(h, *e); icn_crit_walk(h, st, depth); *e = h->c[0]; }
            int lv = icn_leaves(st, depth, icn_exit_index(c));
            if (!lv) continue;
            if (e) *e = icn_unlock_after(*e, st, lv); else i = icn_crit_conj(n, i, st, lv);
            continue;
        }
        icn_crit_walk(c, st, depth + (c->t == TT_WHILE || c->t == TT_UNTIL || c->t == TT_REPEAT || c->t == TT_EVERY));
        if (c->t == TT_RETURN || c->t == TT_PROC_FAIL) n->c[i] = icn_crit_exit(c, st);
        else if (c->t == TT_LOOP_NEXT && icn_leaves(st, depth, 1)) i = icn_crit_conj(n, i, st, icn_leaves(st, depth, 1));
        else if (c->t == TT_SUSPEND) n->c[i] = icn_susp_rewrite(c, st);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *parse_ctrl(IcnParser *p) {
    int line = p->cur.line;
    if (check(p, TK_RETURN)) {
        advance(p);
        tree_t *e = ast_node_new(TT_RETURN);
        e->line = line;
        if (!check(p, TK_SEMICOL) && !check(p, TK_RPAREN) &&
            !check(p, TK_EOF)  && !check(p, TK_THEN) &&
            !check(p, TK_ELSE) && !check(p, TK_DO) && !check(p, TK_RBRACE) && !check(p, TK_END))
            push_child(e, parse_expr(p));
        return e;
    }
    if (check(p, TK_FAIL)) {
        advance(p);
        tree_t *e = ast_node_new(TT_PROC_FAIL);
        e->line = line;
        return e;
    }
    if (check(p, TK_SUSPEND)) {
        advance(p);
        tree_t *e = ast_node_new(TT_SUSPEND);
        e->line = line;
        if (icn_begins_nexpr(p->cur.kind)) push_child(e, parse_expr(p));
        else push_child(e, e_leaf_sval(TT_VAR, "&null", -1));
        tree_t *body = parse_do_clause(p);
        if (body) push_child(e, body);
        return e;
    }
    if (check(p, TK_BREAK)) {
        advance(p);
        tree_t *e = ast_node_new(TT_LOOP_BREAK);
        if (icn_begins_nexpr(p->cur.kind))
            push_child(e, parse_expr(p));
        return e;
    }
    if (check(p, TK_NEXT))  { advance(p); return ast_node_new(TT_LOOP_NEXT); }
    if (check(p, TK_IF)) {
        advance(p);
        tree_t *e = ast_node_new(TT_IF);
        push_child(e, parse_expr(p));
        expect(p, TK_THEN, "if/then");
        push_child(e, parse_expr(p));
        if (match(p, TK_ELSE)) push_child(e, parse_expr(p));
        return e;
    }
    if (check(p, TK_EVERY)) {
        advance(p);
        tree_t *e = ast_node_new(TT_EVERY); p->loops++;
        push_child(e, parse_expr(p));
        tree_t *body = parse_do_clause(p);
        if (body) push_child(e, body);
        p->loops--;
        return e;
    }
    if (check(p, TK_WHILE)) {
        advance(p);
        tree_t *e = ast_node_new(TT_WHILE); p->loops++;
        push_child(e, parse_expr(p));
        tree_t *body = parse_do_clause(p);
        if (body) push_child(e, body);
        p->loops--;
        return e;
    }
    if (check(p, TK_UNTIL)) {
        advance(p);
        tree_t *e = ast_node_new(TT_UNTIL); p->loops++;
        push_child(e, parse_expr(p));
        tree_t *body = parse_do_clause(p);
        if (body) push_child(e, body);
        p->loops--;
        return e;
    }
    if (check(p, TK_REPEAT)) {
        advance(p);
        tree_t *e = ast_node_new(TT_REPEAT); p->loops++;
        push_child(e, parse_expr(p));
        p->loops--;
        return e;
    }
    if (check(p, TK_CREATE)) {
        advance(p);
        tree_t *e = ast_node_new(TT_CREATE);
        push_child(e, parse_expr(p));
        return e;
    }
    if (check(p, TK_THREAD)) {
        advance(p);
        tree_t *cr = ast_node_new(TT_CREATE);
        push_child(cr, parse_expr(p));
        tree_t *sp = ast_node_new(TT_FNC);
        push_child(sp, e_leaf_sval(TT_VAR, "spawn", -1));
        push_child(sp, cr);
        return sp;
    }
    if (check(p, TK_CRITICAL)) {
        advance(p);
        tree_t *cs = ast_node_new(TT_FNC);
        push_child(cs, e_leaf_sval(TT_VAR, "ICN$CRIT", -1));
        push_child(cs, parse_unary(p));
        expect(p, TK_COLON, "critical region");
        IcnCrit fr = { cs->c[1], p->crit, p->loops };
        p->crit = &fr;
        tree_t *holder = ast_node_new(TT_SEQ_EXPR);
        push_child(holder, parse_expr(p));
        icn_crit_walk(holder, &fr, 0);
        p->crit = fr.up;
        tree_t *cb = holder->c[0];
        if (icn_crit_plain(cb)) return cb;
        push_child(cs, cb);
        return cs;
    }
    if (check(p, TK_CASE)) {
        advance(p);
        tree_t *e = ast_node_new(TT_CASE);
        push_child(e, parse_expr(p));
        expect(p, TK_OF, "case expression");
        expect(p, TK_LBRACE, "case body");
        int ndflt = 0;
        while (!check(p, TK_RBRACE) && !check(p, TK_EOF)) {
            if (check(p, TK_DEFAULT)) {
                advance(p);
                expect(p, TK_COLON, "case default");
                if (ndflt++) parser_error(p, "case default: duplicate default clause");
                push_child(e, ast_node_new(TT_NUL));
                push_child(e, parse_expr(p));
                case_sep(p);
                continue;
            }
            push_child(e, parse_expr(p));
            expect(p, TK_COLON, "case clause");
            push_child(e, parse_expr(p));
            case_sep(p);
        }
        expect(p, TK_RBRACE, "case body end");
        return e;
    }
    return parse_and(p);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void case_sep(IcnParser *p) {
    if (check(p, TK_RBRACE)) return;
    if (match(p, TK_SEMICOL)) { if (check(p, TK_RBRACE)) parser_error(p, "case body: a semicolon separates case clauses and may not follow the last one"); return; }
    char m[256]; snprintf(m, sizeof m, "case body: expected ';' between case clauses after the token at line %d col %d (SCRIP Icon: a newline is whitespace)", p->tb_line, p->tb_col);
    parser_error(p, m);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *parse_block_or_expr(IcnParser *p) {
    if (!check(p, TK_LBRACE)) return parse_expr(p);
    advance(p);
    tree_t *seq = ast_node_new(TT_SEQ_EXPR);
    int nc = 0;
    while (!check(p, TK_RBRACE) && !check(p, TK_EOF)) {
        int sline = p->cur.line;
        if (check(p, TK_SEMICOL)) { advance(p); push_child(seq, e_leaf_sval(TT_VAR, "&null", -1)); nc++; continue; }
        tree_t *s = parse_stmt(p);
        if (!s) break;
        if (s->line <= 0) s->line = sline;
        push_child(seq, s);
        nc++;
    }
    if (nc > 0 && p->prev_kind == TK_SEMICOL) { push_child(seq, e_leaf_sval(TT_VAR, "&null", -1)); nc++; }
    expect(p, TK_RBRACE, "compound block");
    if (nc == 1) return seq->c[0];
    return seq;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *parse_do_clause(IcnParser *p) {
    if (check(p, TK_DO)) { advance(p); return parse_expr(p); }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void stmt_end(IcnParser *p, const char *what) {
    if (check(p, TK_RBRACE)) { match(p, TK_SEMICOL); return; }
    if (check(p, TK_SEMICOL)) { advance(p); return; }
    const char *f = "%s: expected ';' after the token at line %d col %d (SCRIP Icon: every statement ends in ';', a newline is whitespace, only the last expression of a { } block is bare)";
    int ml = snprintf((char *)0, 0, f, what, p->tb_line, p->tb_col);
    char m[ml + 1]; snprintf(m, (size_t)ml + 1, f, what, p->tb_line, p->tb_col);
    parser_error(p, m);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *parse_stmt(IcnParser *p) {
    if (check(p, TK_EVERY)) {
        advance(p);
        tree_t *e = ast_node_new(TT_EVERY); p->loops++;
        push_child(e, parse_expr(p));
        tree_t *body = parse_do_clause(p);
        if (body) push_child(e, body);
        p->loops--;
        stmt_end(p, "every statement");
        return e;
    }
    if (check(p, TK_IF)) {
        advance(p);
        tree_t *e = ast_node_new(TT_IF);
        push_child(e, parse_expr(p));
        expect(p, TK_THEN, "if/then");
        push_child(e, parse_expr(p));
        if (match(p, TK_ELSE)) push_child(e, parse_expr(p));
        stmt_end(p, "if statement");
        return e;
    }
    if (check(p, TK_WHILE)) {
        advance(p);
        tree_t *e = ast_node_new(TT_WHILE); p->loops++;
        push_child(e, parse_expr(p));
        tree_t *body = parse_do_clause(p);
        if (body) push_child(e, body);
        p->loops--;
        stmt_end(p, "while statement");
        return e;
    }
    if (check(p, TK_UNTIL)) {
        advance(p);
        tree_t *e = ast_node_new(TT_UNTIL); p->loops++;
        push_child(e, parse_expr(p));
        tree_t *body = parse_do_clause(p);
        if (body) push_child(e, body);
        p->loops--;
        stmt_end(p, "until statement");
        return e;
    }
    if (check(p, TK_REPEAT)) {
        advance(p);
        tree_t *e = ast_node_new(TT_REPEAT); p->loops++;
        push_child(e, parse_expr(p));
        p->loops--;
        stmt_end(p, "repeat statement");
        return e;
    }
    if (check(p, TK_RETURN)) {
        advance(p);
        tree_t *e = ast_node_new(TT_RETURN);
        if (!check(p, TK_SEMICOL) && !check(p, TK_RBRACE) && !check(p, TK_END) &&
            !check(p, TK_EOF) && !check(p, TK_ELSE))
            push_child(e, parse_expr(p));
        stmt_end(p, "return statement");
        return e;
    }
    if (check(p, TK_SUSPEND)) {
        int susline = p->cur.line;
        advance(p);
        tree_t *e = ast_node_new(TT_SUSPEND);
        e->line = susline;
        if (icn_begins_nexpr(p->cur.kind)) push_child(e, parse_expr(p));
        else push_child(e, e_leaf_sval(TT_VAR, "&null", -1));
        tree_t *body = parse_do_clause(p);
        if (body) push_child(e, body);
        stmt_end(p, "suspend statement");
        return e;
    }
    if (check(p, TK_FAIL)) {
        advance(p);
        stmt_end(p, "fail statement");
        return ast_node_new(TT_PROC_FAIL);
    }
    if (check(p, TK_INITIAL)) {
        advance(p);
        tree_t *e = ast_node_new(TT_INITIAL);
        push_child(e, parse_expr(p));
        stmt_end(p, "initial statement");
        return e;
    }
    if (check(p, TK_CASE)) {
        tree_t *e = parse_expr(p);
        stmt_end(p, "case statement");
        return e;
    }
    if (check(p, TK_LOCAL) || check(p, TK_STATIC)) {
        int is_static = check(p, TK_STATIC);
        advance(p);
        tree_t *e = ast_node_new(is_static ? TT_STATIC_DECL : TT_LOCAL);
        tree_t *out = ast_node_new(TT_PROGRAM);
        push_child(out, e);
        while (!check(p, TK_SEMICOL) && !check(p, TK_EOF)) {
            if (p->cur.kind == TK_IDENT) {
                int dline = p->cur.line;
                tree_t *lhs = e_leaf_sval(TT_VAR, p->cur.val.sval.data, (int)p->cur.val.sval.len);
                push_child(e, e_leaf_sval(TT_VAR, p->cur.val.sval.data, (int)p->cur.val.sval.len));
                advance(p);
                if (match(p, TK_ASSIGN)) {
                    tree_t *asn = e_binary_at(dline, TT_ASSIGN, lhs, parse_assign(p));
                    if (is_static) {
                        tree_t *ini = ast_node_new(TT_INITIAL);
                        ini->line = dline;
                        push_child(ini, asn);
                        push_child(out, ini);
                    } else push_child(out, asn);
                }
            }
            if (!match(p, TK_COMMA)) break;
        }
        stmt_end(p, "local or static declaration");
        return out->n > 1 ? out : e;
    }
    tree_t *e = parse_expr(p);
    if (check(p, TK_ELSE) || check(p, TK_THEN)) { match(p, TK_SEMICOL); return e; }
    stmt_end(p, "expression statement");
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *parse_record(IcnParser *p) {
    expect(p, TK_RECORD, "record");
    if (p->cur.kind != TK_IDENT) { parser_error(p, "expected record name"); return NULL; }
    IcnToken name_tok = p->cur; advance(p);
    tree_t *e = ast_node_new(TT_RECORD);
    e->v.sval = intern_n(name_tok.val.sval.data, (int)name_tok.val.sval.len);
    expect(p, TK_LPAREN, "record fields");
    while (!check(p, TK_RPAREN) && !check(p, TK_EOF)) {
        if (p->cur.kind == TK_IDENT) {
            push_child(e, e_leaf_sval(TT_VAR, p->cur.val.sval.data, (int)p->cur.val.sval.len));
            advance(p);
        }
        if (!match(p, TK_COMMA)) break;
    }
    expect(p, TK_RPAREN, "record fields");
    match(p, TK_SEMICOL);
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void icn_body_push(tree_t *body, tree_t *s) { if (s->t == TT_PROGRAM) { for (int i = 0; i < s->n; i++) ast_push(body, s->c[i]); } else ast_push(body, s); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *icn_param_call(const char *f, tree_t *a, tree_t *b) {
    tree_t *c = ast_node_new(TT_FNC);
    push_child(c, e_leaf_sval(TT_VAR, f, -1));
    push_child(c, a);
    if (b) push_child(c, b);
    return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_param_errno(const char *t, int tl) {
    const char *nm[] = {"integer", "numeric", "string", "cset", "file", "list", "set", "table", "window"};
    int en[] = {101, 102, 103, 104, 105, 108, 119, 124, 140};
    for (int i = 0; i < 9; i++) if ((int)strlen(nm[i]) == tl && !strncmp(nm[i], t, tl)) return en[i];
    return 123;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void parse_param_coercion(IcnParser *p, const char *v, int vl, tree_t *coer) {
    int cl = p->cur.line;
    advance(p);
    const char *t = NULL; int tl = 0; tree_t *d = NULL;
    if (check(p, TK_IDENT)) {
        t = intern_n(p->cur.val.sval.data, (int)p->cur.val.sval.len); tl = (int)p->cur.val.sval.len;
        advance(p);
        if (check(p, TK_COLON)) { advance(p); d = parse_primary(p); }
    }
    else d = parse_primary(p);
    if (d) push_child(coer, e_binary_at(cl, TT_ASSIGN, e_unary_at(cl, TT_NULL, e_leaf_sval(TT_VAR, v, vl)), d));
    if (!t || (d && d->t == TT_MAKELIST && d->n == 0)) return;
    int en = icn_param_errno(t, tl);
    int conv = (en >= 101 && en <= 104) || (tl == 4 && !strncmp(t, "real", 4));
    tree_t *ni = ast_node_new(TT_ILIT); ni->v.ival = en;
    tree_t *err = icn_param_call("runerr", ni, e_leaf_sval(TT_VAR, v, vl));
    if (conv) {
        tree_t *cv = icn_param_call(t, e_leaf_sval(TT_VAR, v, vl), NULL);
        push_child(coer, e_binary_at(cl, TT_ASSIGN, e_leaf_sval(TT_VAR, v, vl), e_binary_at(cl, TT_ALTERNATE, cv, err)));
    }
    else {
        tree_t *ty = icn_param_call("type", e_leaf_sval(TT_VAR, v, vl), NULL);
        push_child(coer, e_binary_at(cl, TT_ALTERNATE, e_binary_at(cl, TT_LEQ, ty, e_leaf_sval(TT_QLIT, t, tl)), err));
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *parse_proc_kw(IcnParser *p, IcnTkKind kw, int has_body) {
    expect(p, kw, kw == TK_METHOD ? "method" : "procedure");
    if (p->cur.kind != TK_IDENT) { parser_error(p, "expected procedure name"); return NULL; }
    IcnToken name_tok = p->cur; advance(p);
    tree_t **params = NULL; int nparams = 0, pcap = 0; int is_variadic = 0;
    tree_t *coer = ast_node_new(TT_PROGRAM);
    expect(p, TK_LPAREN, "procedure params");
    while (!check(p, TK_RPAREN) && !check(p, TK_EOF)) {
        if (p->cur.kind == TK_IDENT) {
            if (nparams+1 > pcap) { pcap = pcap ? pcap*2 : 4; params = ct_grow(params, pcap*sizeof(tree_t*)); }
            params[nparams++] = e_leaf_sval(TT_VAR, p->cur.val.sval.data, (int)p->cur.val.sval.len);
            advance(p);
            if (p->lex->unicon && check(p, TK_COLON)) parse_param_coercion(p, params[nparams-1]->v.sval, params[nparams-1]->slen, coer);
            if (check(p, TK_LBRACK)) { advance(p); match(p, TK_RBRACK); is_variadic = 1; break; }
        }
        if (!match(p, TK_COMMA)) break;
    }
    expect(p, TK_RPAREN, "procedure params");
    match(p, TK_SEMICOL);
    tree_t **stmts = NULL; int nstmts = 0, scap = 0;
    while (has_body && !check(p, TK_END) && !check(p, TK_EOF) && !p->had_error) {
        int sline = p->cur.line;
        tree_t *s = parse_stmt(p);
        if (s && s->line <= 0) s->line = sline;
        if (s) {
            if (nstmts+1 > scap) { scap = scap ? scap*2 : 8; stmts = ct_grow(stmts, scap*sizeof(tree_t*)); }
            stmts[nstmts++] = s;
        }
    }
    int end_line = p->cur.line;
    if (has_body) expect(p, TK_END, "end of procedure");
    const char *procname = intern_n(name_tok.val.sval.data, (int)name_tok.val.sval.len);
    tree_t *vlist = ast_node_new(TT_VLIST);
    vlist->v.ival = is_variadic;
    for (int i = 0; i < nparams; i++) ast_push(vlist, params[i]);
    tree_t *body = ast_node_new(TT_PROGRAM);
    icn_body_push(body, coer);
    for (int i = 0; i < nstmts; i++) icn_body_push(body, stmts[i]);
    tree_t *proc = ast_node_new(TT_PROC_DECL);
    proc->v.sval = procname;
    proc->slen = end_line;
    ast_push(proc, e_leaf_sval(TT_VAR, procname, -1));
    ast_push(proc, vlist);
    if (has_body) ast_push(proc, body);
    ct_drop(params); ct_drop(stmts);
    return proc;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *parse_proc(IcnParser *p) { return parse_proc_kw(p, TK_PROCEDURE, 1); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *parse_name_list(IcnParser *p, const char *what, int defaults, tree_t *coer) {
    tree_t *vl = ast_node_new(TT_VLIST);
    expect(p, TK_LPAREN, what);
    while (!check(p, TK_RPAREN) && !check(p, TK_EOF) && !p->had_error) {
        if (p->cur.kind != TK_IDENT) { parser_error(p, "expected a name"); break; }
        tree_t *v = e_leaf_sval(TT_VAR, p->cur.val.sval.data, (int)p->cur.val.sval.len);
        push_child(vl, v);
        advance(p);
        if (coer && p->lex->unicon && check(p, TK_COLON)) parse_param_coercion(p, v->v.sval, v->slen, coer);
        if (coer && check(p, TK_LBRACK)) { advance(p); expect(p, TK_RBRACK, what); vl->v.ival = 1; break; }
        if (defaults && match(p, TK_COLON)) { tree_t *d = parse_expr(p); if (d) ast_push(v, d); }
        if (!match(p, TK_COMMA)) break;
    }
    expect(p, TK_RPAREN, what);
    return vl;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *parse_initially(IcnParser *p) {
    expect(p, TK_INITIALLY, "initially");
    int sline = p->cur.line;
    tree_t *coer = ast_node_new(TT_PROGRAM);
    tree_t *params = check(p, TK_LPAREN) ? parse_name_list(p, "initially params", 0, coer) : ast_node_new(TT_VLIST);
    match(p, TK_SEMICOL);
    tree_t *body = ast_node_new(TT_PROGRAM);
    icn_body_push(body, coer);
    while (!check(p, TK_END) && !check(p, TK_EOF) && !p->had_error) {
        int l = p->cur.line; tree_t *s = parse_stmt(p);
        if (s && s->line <= 0) s->line = l;
        if (s) icn_body_push(body, s);
    }
    tree_t *proc = ast_node_new(TT_PROC_DECL);
    proc->v.sval = intern_n("initially", 9);
    proc->slen = p->cur.line; proc->line = sline;
    ast_push(proc, e_leaf_sval(TT_VAR, "initially", -1));
    ast_push(proc, params);
    ast_push(proc, body);
    return proc;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *parse_class(IcnParser *p) {
    int cline = p->cur.line;
    expect(p, TK_CLASS, "class");
    if (p->cur.kind != TK_IDENT) { parser_error(p, "expected class name"); return NULL; }
    IcnToken name_tok = p->cur; advance(p);
    tree_t *supers = ast_node_new(TT_VLIST);
    while (match(p, TK_COLON)) {
        if (p->cur.kind != TK_IDENT) { parser_error(p, "expected superclass name"); return NULL; }
        push_child(supers, e_leaf_sval(TT_VAR, p->cur.val.sval.data, (int)p->cur.val.sval.len));
        advance(p);
    }
    tree_t *fields = parse_name_list(p, "class fields", 1, NULL);
    match(p, TK_SEMICOL);
    tree_t *cls = ast_node_new(TT_CLASS_DECL);
    cls->v.sval = intern_n(name_tok.val.sval.data, (int)name_tok.val.sval.len);
    cls->line = cline;
    ast_push(cls, supers);
    ast_push(cls, fields);
    while (!check(p, TK_END) && !check(p, TK_INITIALLY) && !check(p, TK_EOF) && !p->had_error) {
        if (check(p, TK_METHOD)) { tree_t * m = parse_proc_kw(p, TK_METHOD, 1); if (m) ast_push(cls, m); }
        else if (check(p, TK_ABSTRACT)) { advance(p); tree_t * m = parse_proc_kw(p, TK_METHOD, 0); if (m) ast_push(cls, m); }
        else if (check(p, TK_RECORD)) { tree_t * r = parse_record(p); if (r) ast_push(cls, r); }
        else { parser_error(p, "class body: expected method, abstract method, initially or end"); break; }
        match(p, TK_SEMICOL);
    }
    if (check(p, TK_INITIALLY)) { tree_t * ini = parse_initially(p); if (ini) ast_push(cls, ini); }
    expect(p, TK_END, "end of class");
    match(p, TK_SEMICOL);
    return cls;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void icn_parse_init(IcnParser *p, IcnLexer *lex) {
    memset(p, 0, sizeof(*p));
    p->lex  = lex;
    p->cur  = icn_lex_next(lex);
    p->peek = icn_lex_next(lex);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
CODE_t *icn_parse_file(IcnParser *p, tree_t **out_ast) {
    tree_t *ast_prog = ast_stmt_new(TT_PROGRAM);
    while (!check(p, TK_EOF) && !p->had_error) {
        tree_t *top = NULL;
        if (check(p, TK_PROCEDURE)) {
            top = parse_proc(p);
        } else if (check(p, TK_RECORD)) {
            top = parse_record(p);
        } else if (check(p, TK_CLASS)) {
            top = parse_class(p);
        } else if (check(p, TK_PACKAGE) || check(p, TK_IMPORT)) {
            int is_pkg = check(p, TK_PACKAGE);
            advance(p);
            top = ast_node_new(is_pkg ? TT_MODULE_DECL : TT_USE_DECL);
            while (!check(p, TK_SEMICOL) && !check(p, TK_EOF) && !p->had_error) {
                if (p->cur.kind == TK_IDENT || p->cur.kind == TK_STRING) {
                    push_child(top, e_leaf_sval(TT_VAR, p->cur.val.sval.data, (int)p->cur.val.sval.len));
                    advance(p);
                } else { parser_error(p, is_pkg ? "expected a package name" : "expected a package name to import"); break; }
                if (is_pkg || !match(p, TK_COMMA)) break;
            }
            if (top->n > 0 && top->c[0]) top->v.sval = top->c[0]->v.sval;
            match(p, TK_SEMICOL);
        } else if (check(p, TK_GLOBAL)) {
            advance(p);
            top = ast_node_new(TT_GLOBAL);
            while (!check(p, TK_SEMICOL) && !check(p, TK_EOF)) {
                if (p->cur.kind == TK_IDENT) {
                    push_child(top, e_leaf_sval(TT_VAR, p->cur.val.sval.data, (int)p->cur.val.sval.len));
                    advance(p);
                }
                if (!match(p, TK_COMMA)) break;
            }
            match(p, TK_SEMICOL);
        } else if (check(p, TK_LINK) || check(p, TK_INVOCABLE)) {
            int is_link = check(p, TK_LINK);
            advance(p);
            top = ast_node_new(is_link ? TT_LINK : TT_INVOCABLE);
            while (!check(p, TK_SEMICOL) && !check(p, TK_EOF)) {
                if (p->cur.kind == TK_IDENT || p->cur.kind == TK_STRING) {
                    push_child(top, e_leaf_sval(TT_VAR, p->cur.val.sval.data, (int)p->cur.val.sval.len));
                    advance(p);
                } else {
                    advance(p);
                }
                if (check(p, TK_COLON) && p->peek.kind == TK_INT) { advance(p); advance(p); }
                if (!match(p, TK_COMMA)) break;
            }
            match(p, TK_SEMICOL);
        } else {
            parser_error(p, p->lex->unicon ? "expected 'procedure', 'record', 'class', 'package', 'import', 'global', 'link', or 'invocable'" : "expected 'procedure', 'record', 'global', 'link', or 'invocable'");
            break;
        }
        if (top) {
            tree_t *ast_st = ast_stmt_new(TT_STMT);
            push_child(ast_st, ast_attr_int(":line", 0));
            push_child(ast_st, ast_attr_int(":stno", 0));
            push_child(ast_st, ast_attr_expr(":subj", top));
            push_child(ast_prog, ast_st);
        }
    }
    if (out_ast) *out_ast = ast_prog;
    return NULL;
}
