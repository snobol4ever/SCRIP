#define _POSIX_C_SOURCE 200809L
#include "icon_lex.h"
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char lex_cur(const IcnLexer *lx) {
    if (lx->pos >= lx->src_len) return '\0';
    return lx->src[lx->pos];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char lex_peek1(const IcnLexer *lx) {
    if (lx->pos + 1 >= lx->src_len) return '\0';
    return lx->src[lx->pos + 1];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char lex_advance(IcnLexer *lx) {
    if (lx->pos >= lx->src_len) return '\0';
    char c = lx->src[lx->pos++];
    if (c == '\n') { lx->line++; lx->col = 1; } else { lx->col++; }
    return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void buf_push(char **buf, int *len, int *cap, char c) {
    if (*len + 2 > *cap) {
        *cap = (*cap) ? (*cap) * 2 : 32;
        *buf = realloc(*buf, *cap);
    }
    (*buf)[(*len)++] = c;
    (*buf)[*len] = '\0';
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IcnToken make_tok(IcnTkKind kind, int line, int col) {
    IcnToken t;
    memset(&t, 0, sizeof(t));
    t.kind = kind; t.line = line; t.col = col;
    return t;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IcnToken make_error(IcnLexer *lx, const char *msg) {
    snprintf(lx->errmsg, sizeof(lx->errmsg), "line %d col %d: %s",
             lx->line, lx->col, msg);
    lx->had_error = 1;
    IcnToken t = make_tok(TK_ERROR, lx->line, lx->col);
    return t;
}
typedef struct { const char *word; IcnTkKind kind; } KwEntry;
static const KwEntry keywords[] = {
    {"to",         TK_TO},
    {"by",         TK_BY},
    {"every",      TK_EVERY},
    {"do",         TK_DO},
    {"if",         TK_IF},
    {"then",       TK_THEN},
    {"else",       TK_ELSE},
    {"while",      TK_WHILE},
    {"until",      TK_UNTIL},
    {"repeat",     TK_REPEAT},
    {"return",     TK_RETURN},
    {"suspend",    TK_SUSPEND},
    {"fail",       TK_FAIL},
    {"break",      TK_BREAK},
    {"next",       TK_NEXT},
    {"not",        TK_NOT},
    {"procedure",  TK_PROCEDURE},
    {"end",        TK_END},
    {"global",     TK_GLOBAL},
    {"local",      TK_LOCAL},
    {"static",     TK_STATIC},
    {"record",     TK_RECORD},
    {"link",       TK_LINK},
    {"invocable",  TK_INVOCABLE},
    {"case",       TK_CASE},
    {"of",         TK_OF},
    {"default",    TK_DEFAULT},
    {"create",     TK_CREATE},
    {"initial",    TK_INITIAL},
    {NULL,         TK_EOF}
};
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IcnTkKind lookup_keyword(const char *word) {
    for (int i = 0; keywords[i].word; i++)
        if (strcmp(keywords[i].word, word) == 0)
            return keywords[i].kind;
    return TK_IDENT;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void skip_ws(IcnLexer *lx) {
    for (;;) {
        while (lex_cur(lx) && isspace((unsigned char)lex_cur(lx)))
            lex_advance(lx);
        if (lex_cur(lx) == '#') {
            const char *line_start = lx->src + lx->pos;
            while (lex_cur(lx) && lex_cur(lx) != '\n')
                lex_advance(lx);
            size_t line_len = (size_t)((lx->src + lx->pos) - line_start);
            if (line_len >= 10) {
                extern int g_jcon;
                char tmp[32]; size_t cpy = line_len < 31 ? line_len : 31;
                memcpy(tmp, line_start, cpy); tmp[cpy] = '\0';
                if (strstr(tmp, "SRC: JCON") || strstr(tmp, "SRC:JCON"))
                    g_jcon = 1;
            }
            continue;
        }
        if (lex_cur(lx) == '$') {
            while (lex_cur(lx) && lex_cur(lx) != '\n')
                lex_advance(lx);
            continue;
        }
        if (lex_cur(lx) == '-') {
            size_t save = lx->pos;
            lex_advance(lx);
            while (lex_cur(lx) == ' ' || lex_cur(lx) == '\t') lex_advance(lx);
            const char *rest = lx->src + lx->pos;
            if (strncmp(rest, "IMPORT", 6) == 0 || strncmp(rest, "EXPORT", 6) == 0) {
                while (lex_cur(lx) && lex_cur(lx) != '\n') lex_advance(lx);
                continue;
            }
            lx->pos = save;
        }
        break;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IcnToken scan_string(IcnLexer *lx) {
    int line = lx->line, col = lx->col;
    lex_advance(lx);
    char *buf = NULL; int len = 0, cap = 0;
    while (lex_cur(lx) && lex_cur(lx) != '"') {
        char c = lex_advance(lx);
        if (c == '\\') {
            char esc = lex_advance(lx);
            switch (esc) {
                case 'n':  buf_push(&buf, &len, &cap, '\n'); break;
                case 't':  buf_push(&buf, &len, &cap, '\t'); break;
                case 'r':  buf_push(&buf, &len, &cap, '\r'); break;
                case '\\': buf_push(&buf, &len, &cap, '\\'); break;
                case '"':  buf_push(&buf, &len, &cap, '"');  break;
                case '\'': buf_push(&buf, &len, &cap, '\''); break;
                case 'b':  buf_push(&buf, &len, &cap, '\b'); break;
                case 'd':  buf_push(&buf, &len, &cap, (char)0x7f); break;
                case 'e':  buf_push(&buf, &len, &cap, (char)0x1b); break;
                case 'f':  buf_push(&buf, &len, &cap, '\f'); break;
                case 'l':  buf_push(&buf, &len, &cap, '\n'); break;
                case 'v':  buf_push(&buf, &len, &cap, '\v'); break;
                case 'x': case 'X': { int v = 0, nd = 0;
                    while (nd < 2 && isxdigit((unsigned char)lex_cur(lx))) { char h = lex_advance(lx); v = v * 16 + (isdigit((unsigned char)h) ? h - '0' : (tolower((unsigned char)h) - 'a' + 10)); nd++; }
                    buf_push(&buf, &len, &cap, (char)v); break; }
                case '^': { char cc = lex_cur(lx) ? lex_advance(lx) : 0; buf_push(&buf, &len, &cap, (char)(cc & 0x1f)); break; }
                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': { int v = esc - '0', nd = 1;
                    while (nd < 3 && lex_cur(lx) >= '0' && lex_cur(lx) <= '7') { v = v * 8 + (lex_advance(lx) - '0'); nd++; }
                    buf_push(&buf, &len, &cap, (char)v); break; }
                default:   buf_push(&buf, &len, &cap, '\\');
                           buf_push(&buf, &len, &cap, esc);  break;
            }
        } else {
            buf_push(&buf, &len, &cap, c);
        }
    }
    if (!lex_cur(lx)) { free(buf); return make_error(lx, "unterminated string literal"); }
    lex_advance(lx);
    if (!buf) buf = strdup("");
    IcnToken t = make_tok(TK_STRING, line, col);
    t.val.sval.data = buf;
    t.val.sval.len  = (size_t)len;
    return t;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IcnToken scan_cset(IcnLexer *lx) {
    int line = lx->line, col = lx->col;
    lex_advance(lx);
    char *buf = NULL; int len = 0, cap = 0;
    while (lex_cur(lx) && lex_cur(lx) != '\'') {
        char c = lex_advance(lx);
        if (c == '\\' && lex_cur(lx)) {
            char esc = lex_advance(lx);
            switch (esc) {
                case '\'': c = '\''; break;
                case '\\': c = '\\'; break;
                case 'n':  c = '\n'; break;
                case 't':  c = '\t'; break;
                case 'r':  c = '\r'; break;
                case 'b':  c = '\b'; break;
                case 'd':  c = (char)0x7f; break;
                case 'e':  c = (char)0x1b; break;
                case 'f':  c = '\f'; break;
                case 'l':  c = '\n'; break;
                case 'v':  c = '\v'; break;
                case '"':  c = '"'; break;
                case 'x': case 'X': { int v = 0, nd = 0;
                    while (nd < 2 && isxdigit((unsigned char)lex_cur(lx))) { char h = lex_advance(lx); v = v * 16 + (isdigit((unsigned char)h) ? h - '0' : (tolower((unsigned char)h) - 'a' + 10)); nd++; }
                    c = (char)v; break; }
                case '^': { c = (char)((lex_cur(lx) ? lex_advance(lx) : 0) & 0x1f); break; }
                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': { int v = esc - '0', nd = 1;
                    while (nd < 3 && lex_cur(lx) >= '0' && lex_cur(lx) <= '7') { v = v * 8 + (lex_advance(lx) - '0'); nd++; }
                    c = (char)v; break; }
                default:   buf_push(&buf, &len, &cap, c); c = esc; break;
            }
        }
        buf_push(&buf, &len, &cap, c);
    }
    if (!lex_cur(lx)) { free(buf); return make_error(lx, "unterminated cset literal"); }
    lex_advance(lx);
    if (!buf) buf = strdup("");
    IcnToken t = make_tok(TK_CSET, line, col);
    t.val.sval.data = buf;
    t.val.sval.len  = (size_t)len;
    return t;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IcnToken scan_number(IcnLexer *lx) {
    int line = lx->line, col = lx->col;
    char *buf = NULL; int len = 0, cap = 0;
    int is_real = 0;
    if (lex_cur(lx) == '0' &&
        (lex_peek1(lx) == 'x' || lex_peek1(lx) == 'X')) {
        buf_push(&buf, &len, &cap, lex_advance(lx));
        buf_push(&buf, &len, &cap, lex_advance(lx));
        while (isxdigit((unsigned char)lex_cur(lx)))
            buf_push(&buf, &len, &cap, lex_advance(lx));
        long val = strtol(buf, NULL, 16);
        free(buf);
        IcnToken t = make_tok(TK_INT, line, col);
        t.val.ival = val;
        return t;
    }
    while (isdigit((unsigned char)lex_cur(lx)))
        buf_push(&buf, &len, &cap, lex_advance(lx));
    if ((lex_cur(lx) == 'r' || lex_cur(lx) == 'R') && !is_real) {
        int radix = (int)strtol(buf, NULL, 10);
        free(buf); buf = NULL; len = 0; cap = 0;
        lex_advance(lx);
        while (isalnum((unsigned char)lex_cur(lx)))
            buf_push(&buf, &len, &cap, lex_advance(lx));
        if (!buf) buf = strdup("0");
        unsigned long long val = 0;
        for (int i = 0; i < len; i++) {
            char c = buf[i];
            int d = isdigit((unsigned char)c) ? c - '0'
                  : islower((unsigned char)c) ? c - 'a' + 10
                  : c - 'A' + 10;
            val = val * (unsigned)radix + (unsigned)d;
        }
        free(buf);
        IcnToken t = make_tok(TK_INT, line, col);
        t.val.ival = (long long)val;
        return t;
    }
    if (lex_cur(lx) == '.' && (isdigit((unsigned char)lex_peek1(lx)) || len > 0)) {
        is_real = 1;
        buf_push(&buf, &len, &cap, lex_advance(lx));
        while (isdigit((unsigned char)lex_cur(lx)))
            buf_push(&buf, &len, &cap, lex_advance(lx));
    }
    if (lex_cur(lx) == 'e' || lex_cur(lx) == 'E') {
        is_real = 1;
        buf_push(&buf, &len, &cap, lex_advance(lx));
        if (lex_cur(lx) == '+' || lex_cur(lx) == '-')
            buf_push(&buf, &len, &cap, lex_advance(lx));
        while (isdigit((unsigned char)lex_cur(lx)))
            buf_push(&buf, &len, &cap, lex_advance(lx));
    }
    IcnToken t;
    if (is_real) {
        t = make_tok(TK_REAL, line, col);
        t.val.fval = strtod(buf, NULL);
    } else {
        t = make_tok(TK_INT, line, col);
        t.val.ival = strtol(buf, NULL, 10);
    }
    free(buf);
    return t;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IcnToken scan_ident(IcnLexer *lx) {
    int line = lx->line, col = lx->col;
    char *buf = NULL; int len = 0, cap = 0;
    while (isalnum((unsigned char)lex_cur(lx)) || lex_cur(lx) == '_')
        buf_push(&buf, &len, &cap, lex_advance(lx));
    if (!buf) buf = strdup("");
    IcnTkKind kind = lookup_keyword(buf);
    IcnToken t = make_tok(kind, line, col);
    if (kind == TK_IDENT) {
        t.val.sval.data = buf;
        t.val.sval.len  = (size_t)len;
    } else {
        free(buf);
    }
    return t;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IcnToken lex_one(IcnLexer *lx) {
    skip_ws(lx);
    int line = lx->line, col = lx->col;
    char c = lex_cur(lx);
    if (c == '\0') return make_tok(TK_EOF, line, col);
    if (c == '"')  return scan_string(lx);
    if (c == '\'') return scan_cset(lx);
    if (isdigit((unsigned char)c)) return scan_number(lx);
    if (isalpha((unsigned char)c) || c == '_') return scan_ident(lx);
    lex_advance(lx);
    switch (c) {
        case '+':
            if (lex_cur(lx) == '+') {
                lex_advance(lx);
                if (lex_cur(lx) == ':' && lex_peek1(lx) == '=') {
                    lex_advance(lx); lex_advance(lx);
                    return make_tok(TK_AUGCSET_UNION, line, col);
                }
                return make_tok(TK_PLUSPLUS, line, col);
            }
            if (lex_cur(lx) == ':' && lex_peek1(lx) == '=') {
                lex_advance(lx); lex_advance(lx);
                return make_tok(TK_AUGPLUS, line, col);
            }
            if (lex_cur(lx) == ':') {
                lex_advance(lx);
                return make_tok(TK_PLUSCOLON, line, col);
            }
            return make_tok(TK_PLUS, line, col);
        case '-':
            if (lex_cur(lx) == '-') {
                lex_advance(lx);
                if (lex_cur(lx) == ':' && lex_peek1(lx) == '=') {
                    lex_advance(lx); lex_advance(lx);
                    return make_tok(TK_AUGCSET_DIFF, line, col);
                }
                return make_tok(TK_MINUSMINUS, line, col);
            }
            if (lex_cur(lx) == ':' && lex_peek1(lx) == '=') {
                lex_advance(lx); lex_advance(lx);
                return make_tok(TK_AUGMINUS, line, col);
            }
            if (lex_cur(lx) == ':') {
                lex_advance(lx);
                return make_tok(TK_MINUSCOLON, line, col);
            }
            if (lex_cur(lx) == '>') {
                lex_advance(lx);
                return make_tok(TK_MINUS, line, col);
            }
            return make_tok(TK_MINUS, line, col);
        case '*':
            if (lex_cur(lx) == '*') {
                lex_advance(lx);
                if (lex_cur(lx) == ':' && lex_peek1(lx) == '=') {
                    lex_advance(lx); lex_advance(lx);
                    return make_tok(TK_AUGCSET_INTER, line, col);
                }
                return make_tok(TK_STARSTAR, line, col);
            }
            if (lex_cur(lx) == ':' && lex_peek1(lx) == '=') {
                lex_advance(lx); lex_advance(lx);
                return make_tok(TK_AUGSTAR, line, col);
            }
            return make_tok(TK_STAR, line, col);
        case '/':
            if (lex_cur(lx) == ':' && lex_peek1(lx) == '=') {
                lex_advance(lx); lex_advance(lx);
                return make_tok(TK_AUGSLASH, line, col);
            }
            return make_tok(TK_SLASH, line, col);
        case '%':
            if (lex_cur(lx) == ':' && lex_peek1(lx) == '=') {
                lex_advance(lx); lex_advance(lx);
                return make_tok(TK_AUGMOD, line, col);
            }
            return make_tok(TK_MOD, line, col);
        case '^':
            if (lex_cur(lx) == ':' && lex_peek1(lx) == '=') {
                lex_advance(lx); lex_advance(lx);
                return make_tok(TK_AUGPOW, line, col);
            }
            return make_tok(TK_CARET, line, col);
        case '<':
            if (lex_cur(lx) == '<') {
                lex_advance(lx);
                if (lex_cur(lx) == '=') {
                    lex_advance(lx);
                    if (lex_cur(lx) == ':' && lex_peek1(lx) == '=') {
                        lex_advance(lx); lex_advance(lx);
                        return make_tok(TK_AUGSLE, line, col);
                    }
                    return make_tok(TK_SLE, line, col);
                }
                if (lex_cur(lx) == ':' && lex_peek1(lx) == '=') {
                    lex_advance(lx); lex_advance(lx);
                    return make_tok(TK_AUGSLT, line, col);
                }
                return make_tok(TK_SLT, line, col);
            }
            if (lex_cur(lx) == '=') {
                lex_advance(lx);
                if (lex_cur(lx) == ':' && lex_peek1(lx) == '=') {
                    lex_advance(lx); lex_advance(lx);
                    return make_tok(TK_AUGLE, line, col);
                }
                return make_tok(TK_LE, line, col);
            }
            if (lex_cur(lx) == '-') {
                lex_advance(lx);
                if (lex_cur(lx) == '>') { lex_advance(lx); return make_tok(TK_VALSWAP, line, col); }
                return make_tok(TK_REVASSIGN, line, col);
            }
            if (lex_cur(lx) == ':' && lex_peek1(lx) == '=') {
                lex_advance(lx); lex_advance(lx);
                return make_tok(TK_AUGLT, line, col);
            }
            return make_tok(TK_LT, line, col);
        case '>':
            if (lex_cur(lx) == '>') {
                lex_advance(lx);
                if (lex_cur(lx) == '=') {
                    lex_advance(lx);
                    if (lex_cur(lx) == ':' && lex_peek1(lx) == '=') {
                        lex_advance(lx); lex_advance(lx);
                        return make_tok(TK_AUGSGE, line, col);
                    }
                    return make_tok(TK_SGE, line, col);
                }
                if (lex_cur(lx) == ':' && lex_peek1(lx) == '=') {
                    lex_advance(lx); lex_advance(lx);
                    return make_tok(TK_AUGSGT, line, col);
                }
                return make_tok(TK_SGT, line, col);
            }
            if (lex_cur(lx) == '=') {
                lex_advance(lx);
                if (lex_cur(lx) == ':' && lex_peek1(lx) == '=') {
                    lex_advance(lx); lex_advance(lx);
                    return make_tok(TK_AUGGE, line, col);
                }
                return make_tok(TK_GE, line, col);
            }
            if (lex_cur(lx) == ':' && lex_peek1(lx) == '=') {
                lex_advance(lx); lex_advance(lx);
                return make_tok(TK_AUGGT, line, col);
            }
            return make_tok(TK_GT, line, col);
        case '=':
            if (lex_cur(lx) == '=') {
                lex_advance(lx);
                if (lex_cur(lx) == '=') {
                    lex_advance(lx);
                    return make_tok(TK_IDENTICAL, line, col);
                }
                if (lex_cur(lx) == ':' && lex_peek1(lx) == '=') {
                    lex_advance(lx); lex_advance(lx);
                    return make_tok(TK_AUGSEQ, line, col);
                }
                return make_tok(TK_SEQ, line, col);
            }
            if (lex_cur(lx) == ':' && lex_peek1(lx) == '=') {
                lex_advance(lx); lex_advance(lx);
                return make_tok(TK_AUGEQ, line, col);
            }
            return make_tok(TK_EQ, line, col);
        case '~':
            if (lex_cur(lx) == '=') {
                lex_advance(lx);
                if (lex_cur(lx) == '=') {
                    lex_advance(lx);
                    if (lex_cur(lx) == '=') { lex_advance(lx); return make_tok(TK_NOTIDENT, line, col); }
                    if (lex_cur(lx) == ':' && lex_peek1(lx) == '=') {
                        lex_advance(lx); lex_advance(lx);
                        return make_tok(TK_AUGSNE, line, col);
                    }
                    return make_tok(TK_SNE, line, col);
                }
                if (lex_cur(lx) == ':' && lex_peek1(lx) == '=') {
                    lex_advance(lx); lex_advance(lx);
                    return make_tok(TK_AUGNE, line, col);
                }
                return make_tok(TK_NEQ, line, col);
            }
            return make_tok(TK_TILDE, line, col);
        case '|':
            if (lex_cur(lx) == '|') {
                lex_advance(lx);
                if (lex_cur(lx) == '|') { lex_advance(lx); return make_tok(TK_LCONCAT, line, col); }
                if (lex_cur(lx) == ':' && lex_peek1(lx) == '=') {
                    lex_advance(lx); lex_advance(lx);
                    return make_tok(TK_AUGCONCAT, line, col);
                }
                return make_tok(TK_CONCAT, line, col);
            }
            return make_tok(TK_BAR, line, col);
        case ':':
            if (lex_cur(lx) == '=') {
                lex_advance(lx);
                if (lex_cur(lx) == ':') { lex_advance(lx); return make_tok(TK_SWAP, line, col); }
                return make_tok(TK_ASSIGN, line, col);
            }
            return make_tok(TK_COLON, line, col);
        case '&': return make_tok(TK_AND, line, col);
        case '\\': return make_tok(TK_BACKSLASH, line, col);
        case '!': return make_tok(TK_BANG, line, col);
        case '?':
            if (lex_cur(lx) == ':' && lex_peek1(lx) == '=') {
                lex_advance(lx); lex_advance(lx);
                return make_tok(TK_AUGSCAN, line, col);
            }
            return make_tok(TK_QMARK, line, col);
        case '@': return make_tok(TK_AT, line, col);
        case '.':
            if (isdigit((unsigned char)lex_cur(lx))) {
                lx->pos--; lx->col--;
                return scan_number(lx);
            }
            return make_tok(TK_DOT, line, col);
        case '(': return make_tok(TK_LPAREN, line, col);
        case ')': return make_tok(TK_RPAREN, line, col);
        case '{': return make_tok(TK_LBRACE, line, col);
        case '}': return make_tok(TK_RBRACE, line, col);
        case '[': return make_tok(TK_LBRACK, line, col);
        case ']': return make_tok(TK_RBRACK, line, col);
        case ',': return make_tok(TK_COMMA, line, col);
        case ';': return make_tok(TK_SEMICOL, line, col);
        default: {
            char msg[64];
            snprintf(msg, sizeof(msg), "unexpected character '%c' (0x%02x)", c, (unsigned char)c);
            return make_error(lx, msg);
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pp_word_char(char c) { return isalnum((unsigned char)c) || c == '_'; }
typedef struct { char *name; char *val; } PpDef;
static char *pp_subst_span(const char *s, size_t len, const PpDef *defs, int ndefs) {
    char *out = NULL; int olen = 0, ocap = 0; size_t i = 0;
    while (i < len) {
        char c = s[i];
        if (c == '"' || c == '\'') { char q = c; buf_push(&out, &olen, &ocap, s[i++]);
            while (i < len) { char d = s[i]; buf_push(&out, &olen, &ocap, d); i++; if (d == '\\' && i < len) { buf_push(&out, &olen, &ocap, s[i++]); } else if (d == q) break; }
            continue; }
        if (pp_word_char(c) && !isdigit((unsigned char)c)) {
            size_t ws = i; while (i < len && pp_word_char(s[i])) i++;
            size_t wl = i - ws; const char *rep = NULL;
            for (int k = ndefs - 1; k >= 0; k--) if (strlen(defs[k].name) == wl && !strncmp(defs[k].name, s + ws, wl)) { rep = defs[k].val; break; }
            if (rep) { for (const char *p = rep; *p; p++) buf_push(&out, &olen, &ocap, *p); }
            else     { for (size_t k = ws; k < i; k++) buf_push(&out, &olen, &ocap, s[k]); }
            continue; }
        buf_push(&out, &olen, &ocap, c); i++;
    }
    if (!out) out = strdup("");
    return out;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char icn_pp_src[1024] = "";
/* ICN-PROGNAME (s238): this static now holds the FULL source path, not the directory slice it held before.  NO NEW GLOBAL -- the storage is the same single file-scope array, repurposed; the include-resolution reader below derives its directory into a stack buffer, and icn_pp_source_base() derives the stem.  &progname needs the program STEM (Arizona init.r:212 `prog_name = name`, published as kywd_prog and read by keyword.r:384), which the directory slice had thrown away; keeping two arrays would have been a parallel-array global and is exactly what the FACT RULE forbids.                                                                                       */
void icn_pp_set_source_path(const char *path) {
    if (!path) { icn_pp_src[0] = 0; return; }
    size_t pl = strlen(path); if (pl >= sizeof icn_pp_src) pl = sizeof icn_pp_src - 1;
    memcpy(icn_pp_src, path, pl); icn_pp_src[pl] = 0;
}
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Program stem for &progname: basename with one trailing extension stripped, matching `icont -s foo.icn -x` whose icode file (and therefore &progname) is `foo`.  VERIFIED against a live Arizona Icon 9.5.25a built from refs/icon-master: a probe named rung36_jcon_kwds_probe.icn printed progname=rung36_jcon_kwds_probe.  Writes into a caller buffer so no storage is added.                                                                                                                                                                                                                                */
void icn_pp_source_base(char *out, size_t n) {
    if (!out || n == 0) return;
    const char *slash = strrchr(icn_pp_src, '/');
    const char *base  = slash ? slash + 1 : icn_pp_src;
    const char *dot   = strrchr(base, '.');
    size_t bl = dot ? (size_t)(dot - base) : strlen(base);
    if (bl >= n) bl = n - 1;
    memcpy(out, base, bl); out[bl] = 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *pp_expand(const char *body, const PpDef *defs, int ndefs) {
    char *cur = strdup(body ? body : "");
    for (int r = 0; r < 8; r++) {
        char *nx = pp_subst_span(cur, strlen(cur), defs, ndefs);
        int same = !strcmp(nx, cur);
        free(cur); cur = nx;
        if (same) break;
    }
    return cur;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void icn_pp_run(const char *src, char **out, int *olen, int *ocap, PpDef *defs, int *ndefs, int depth) {
    size_t i = 0, n = strlen(src); int at_bol = 1;
    while (i < n) {
        char c = src[i];
        if (at_bol && c == '$') {
            size_t j = i + 1;
            while (j < n && (src[j] == ' ' || src[j] == '\t')) j++;
            if (j + 6 <= n && !strncmp(src + j, "define", 6) && (j + 6 == n || !pp_word_char(src[j + 6]))) {
                j += 6; while (j < n && (src[j] == ' ' || src[j] == '\t')) j++;
                size_t ns = j; while (j < n && pp_word_char(src[j])) j++;
                size_t ne = j;
                if (ne > ns && (j >= n || src[j] != '(')) {
                    while (j < n && (src[j] == ' ' || src[j] == '\t')) j++;
                    size_t vs = j, ve = j;
                    while (j < n && src[j] != '\n') {
                        char d = src[j];
                        if (d == '#') break;
                        if (d == '"' || d == '\'') { char q = d; j++; while (j < n && src[j] != '\n') { if (src[j] == '\\' && j + 1 < n) j++; else if (src[j] == q) break; j++; } if (j < n && src[j] == q) j++; ve = j; continue; }
                        j++; ve = j;
                    }
                    while (ve > vs && (src[ve - 1] == ' ' || src[ve - 1] == '\t' || src[ve - 1] == '\r')) ve--;
                    if (ve > vs && src[ve - 1] == ';') { ve--; while (ve > vs && (src[ve - 1] == ' ' || src[ve - 1] == '\t')) ve--; }
                    if (*ndefs < 128) { defs[*ndefs].name = strndup(src + ns, ne - ns); defs[*ndefs].val = pp_subst_span(src + vs, ve - vs, defs, *ndefs); (*ndefs)++; }
                }
            } else if (j + 7 <= n && !strncmp(src + j, "include", 7) && (j + 7 == n || !pp_word_char(src[j + 7]))) {
                j += 7; while (j < n && (src[j] == ' ' || src[j] == '\t')) j++;
                if (j < n && src[j] == '"' && depth < 8) {
                    j++; size_t fs = j; while (j < n && src[j] != '"' && src[j] != '\n') j++;
                    if (j < n && src[j] == '"') {
                        char nm[512]; size_t fl = j - fs; if (fl >= sizeof nm) fl = sizeof nm - 1;
                        memcpy(nm, src + fs, fl); nm[fl] = 0;
                        char path[1600];
                        if (nm[0] == '/') snprintf(path, sizeof path, "%s", nm);
                        else { char dir[1024]; const char *sl = strrchr(icn_pp_src, '/'); if (sl) { size_t dl = (size_t)(sl - icn_pp_src); if (dl >= sizeof dir) dl = sizeof dir - 1; memcpy(dir, icn_pp_src, dl); dir[dl] = 0; } else { dir[0] = '.'; dir[1] = 0; } snprintf(path, sizeof path, "%s/%s", dir, nm); }   /* ICN-PROGNAME (s238): directory derived on demand from the full path now held in icn_pp_src; identical resolution to the old directory-only static, including the "." fallback when the path carries no slash. */
                        FILE *f = fopen(path, "rb");
                        if (f) {
                            fseek(f, 0, SEEK_END); long fz = ftell(f); fseek(f, 0, SEEK_SET);
                            char *fsrc = (char *)malloc((size_t)fz + 1);
                            if (fsrc) { size_t rd = fread(fsrc, 1, (size_t)fz, f); fsrc[rd] = 0; icn_pp_run(fsrc, out, olen, ocap, defs, ndefs, depth + 1); free(fsrc); }
                            fclose(f);
                        }
                    }
                }
            }
            while (i < n && src[i] != '\n') { buf_push(out, olen, ocap, ' '); i++; }
            continue;
        }
        if (c == '#') { while (i < n && src[i] != '\n') { buf_push(out, olen, ocap, src[i++]); } continue; }
        if (c == '"' || c == '\'') { char q = c; buf_push(out, olen, ocap, src[i++]);
            while (i < n) { char d = src[i]; buf_push(out, olen, ocap, d); i++; if (d == '\\' && i < n) { buf_push(out, olen, ocap, src[i++]); } else if (d == q || d == '\n') break; }
            at_bol = 0; continue; }
        if (pp_word_char(c) && !isdigit((unsigned char)c) && *ndefs > 0) {
            size_t ws = i; while (i < n && pp_word_char(src[i])) i++;
            size_t wl = i - ws; const char *rep = NULL;
            for (int k = *ndefs - 1; k >= 0; k--) if (strlen(defs[k].name) == wl && !strncmp(defs[k].name, src + ws, wl)) { rep = defs[k].val; break; }
            if (rep) { char *ex = pp_expand(rep, defs, *ndefs); for (const char *p = ex; *p; p++) buf_push(out, olen, ocap, *p); free(ex); }
            else     { for (size_t k = ws; k < i; k++) buf_push(out, olen, ocap, src[k]); }
            at_bol = 0; continue; }
        buf_push(out, olen, ocap, c);
        at_bol = (c == '\n') || (at_bol && (c == ' ' || c == '\t')); i++;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *icn_preprocess(const char *src) {
    PpDef defs[128]; int ndefs = 0;
    static const char *pre[] = { "_UNIX", "_JAVA", "_ASCII", "_CO_EXPRESSIONS", "_LARGE_INTEGERS", "_PIPES", "_SYSTEM_FUNCTION" };
    for (int k = 0; k < 7 && ndefs < 128; k++) { defs[ndefs].name = strdup(pre[k]); defs[ndefs].val = strdup("1"); ndefs++; }
    char *out = NULL; int olen = 0, ocap = 0;
    icn_pp_run(src, &out, &olen, &ocap, defs, &ndefs, 0);
    if (!out) out = strdup("");
    for (int k = 0; k < ndefs; k++) { free(defs[k].name); free(defs[k].val); }
    return out;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void icn_lex_init(IcnLexer *lx, const char *src) {
    memset(lx, 0, sizeof(*lx));
    if (src && strchr(src, '$')) src = icn_preprocess(src);
    lx->src     = src;
    lx->src_len = strlen(src);
    lx->pos     = 0;
    lx->line    = 1;
    lx->col     = 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IcnToken icn_lex_next(IcnLexer *lx) {
    if (lx->had_error == -1) {
    }
    return lex_one(lx);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *icn_tk_name(IcnTkKind kind) {
    switch (kind) {
        case TK_EOF:       return "EOF";
        case TK_ERROR:     return "ERROR";
        case TK_INT:       return "INT";
        case TK_REAL:      return "REAL";
        case TK_STRING:    return "STRING";
        case TK_CSET:      return "CSET";
        case TK_IDENT:     return "IDENT";
        case TK_PLUS:      return "+";
        case TK_MINUS:     return "-";
        case TK_STAR:      return "*";
        case TK_SLASH:     return "/";
        case TK_MOD:       return "%";
        case TK_CARET:     return "^";
        case TK_LT:        return "<";
        case TK_LE:        return "<=";
        case TK_GT:        return ">";
        case TK_GE:        return ">=";
        case TK_EQ:        return "=";
        case TK_NEQ:       return "~=";
        case TK_SLT:       return "<<";
        case TK_SLE:       return "<<=";
        case TK_SGT:       return ">>";
        case TK_SGE:       return ">>=";
        case TK_SEQ:       return "==";
        case TK_SNE:       return "~==";
        case TK_CONCAT:    return "||";
        case TK_LCONCAT:   return "|||";
        case TK_ASSIGN:    return ":=";
        case TK_SWAP:      return ":=:";
        case TK_REVASSIGN: return "<-";
        case TK_AUGPLUS:   return "+:=";
        case TK_AUGMINUS:  return "-:=";
        case TK_AUGSTAR:   return "*:=";
        case TK_AUGSLASH:  return "/:=";
        case TK_AUGMOD:    return "%:=";
        case TK_AUGPOW:    return "^:=";
        case TK_AUGCONCAT: return "||:=";
        case TK_AUGCSET_UNION: return "++:=";
        case TK_AUGCSET_DIFF:  return "--:=";
        case TK_AUGCSET_INTER: return "**:=";
        case TK_AUGSCAN:   return "?:=";
        case TK_AUGEQ:     return "=:=";
        case TK_AUGSEQ:    return "==:=";
        case TK_AUGLT:     return "<:=";
        case TK_AUGLE:     return "<=:=";
        case TK_AUGGT:     return ">:=";
        case TK_AUGGE:     return ">=:=";
        case TK_AUGNE:     return "~=:=";
        case TK_AUGSLT:    return "<<:=";
        case TK_AUGSLE:    return "<<=:=";
        case TK_AUGSGT:    return ">>:=";
        case TK_AUGSGE:    return ">>=:=";
        case TK_AUGSNE:    return "~==:=";
        case TK_VALSWAP:   return "<->";
        case TK_IDENTICAL: return "===";
        case TK_NOTIDENT:  return "~===";
        case TK_PLUSCOLON: return "+:";
        case TK_MINUSCOLON: return "-:";
        case TK_PLUSPLUS:  return "++";
        case TK_MINUSMINUS: return "--";
        case TK_STARSTAR:  return "**";
        case TK_AND:       return "&";
        case TK_BAR:       return "|";
        case TK_BACKSLASH: return "\\";
        case TK_BANG:      return "!";
        case TK_QMARK:     return "?";
        case TK_AT:        return "@";
        case TK_TILDE:     return "~";
        case TK_DOT:       return ".";
        case TK_TO:        return "to";
        case TK_BY:        return "by";
        case TK_EVERY:     return "every";
        case TK_DO:        return "do";
        case TK_IF:        return "if";
        case TK_THEN:      return "then";
        case TK_ELSE:      return "else";
        case TK_WHILE:     return "while";
        case TK_UNTIL:     return "until";
        case TK_REPEAT:    return "repeat";
        case TK_RETURN:    return "return";
        case TK_SUSPEND:   return "suspend";
        case TK_FAIL:      return "fail";
        case TK_BREAK:     return "break";
        case TK_NEXT:      return "next";
        case TK_NOT:       return "not";
        case TK_PROCEDURE: return "procedure";
        case TK_END:       return "end";
        case TK_GLOBAL:    return "global";
        case TK_LOCAL:     return "local";
        case TK_STATIC:    return "static";
        case TK_RECORD:    return "record";
        case TK_LINK:      return "link";
        case TK_INVOCABLE: return "invocable";
        case TK_CASE:      return "case";
        case TK_OF:        return "of";
        case TK_DEFAULT:   return "default";
        case TK_CREATE:    return "create";
        case TK_INITIAL:   return "initial";
        case TK_LPAREN:    return "(";
        case TK_RPAREN:    return ")";
        case TK_LBRACE:    return "{";
        case TK_RBRACE:    return "}";
        case TK_LBRACK:    return "[";
        case TK_RBRACK:    return "]";
        case TK_COMMA:     return ",";
        case TK_SEMICOL:   return ";";
        case TK_COLON:     return ":";
        default:           return "???";
    }
}
