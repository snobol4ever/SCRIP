#include "icon_lex.h"
#include "ct_arena.h"
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>
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
        *buf = ct_grow(*buf, *cap);
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
            if (lx->col == 1 && lx->pos + 5 <= lx->src_len && !strncmp(lx->src + lx->pos, "#line", 5)) {
                size_t q = lx->pos + 5; long n = 0;
                while (q < lx->src_len && (lx->src[q] == ' ' || lx->src[q] == '\t')) q++;
                if (q < lx->src_len && isdigit((unsigned char)lx->src[q])) { while (q < lx->src_len && isdigit((unsigned char)lx->src[q])) n = 10 * n + (lx->src[q++] - '0'); lx->line = (int)n; }
            }
            while (lex_cur(lx) && lex_cur(lx) != '\n')
                lex_advance(lx);
            continue;
        }
        if (lex_cur(lx) == '$') {
            char nx = lex_peek1(lx);
            if (nx == '(' || nx == ')' || nx == '<' || nx == '>') break;
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
static char icn_esc_simple(char esc) {
    switch (esc) {
        case 'b': case 'B': return (char)0x08;
        case 'd': case 'D': return (char)0x7f;
        case 'e': case 'E': return (char)0x1b;
        case 'f': case 'F': return (char)0x0c;
        case 'l': case 'L': return (char)0x0a;
        case 'n': case 'N': return (char)0x0a;
        case 'r': case 'R': return (char)0x0d;
        case 't': case 'T': return (char)0x09;
        case 'v': case 'V': return (char)0x0b;
        case '8': return (char)0x08;
        case '9': return (char)0x09;
        default: return esc;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
                case 'x': { int v = 0, nd = 0;
                    while (nd < 2 && isxdigit((unsigned char)lex_cur(lx))) { char h = lex_advance(lx); v = v * 16 + (isdigit((unsigned char)h) ? h - '0' : (tolower((unsigned char)h) - 'a' + 10)); nd++; }
                    buf_push(&buf, &len, &cap, (char)v); break; }
                case '^': { char cc = lex_cur(lx) ? lex_advance(lx) : 0; buf_push(&buf, &len, &cap, (char)(cc & 0x1f)); break; }
                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': { int v = esc - '0', nd = 1;
                    while (nd < 3 && lex_cur(lx) >= '0' && lex_cur(lx) <= '7') { v = v * 8 + (lex_advance(lx) - '0'); nd++; }
                    buf_push(&buf, &len, &cap, (char)v); break; }
                default:   buf_push(&buf, &len, &cap, icn_esc_simple(esc)); break;
            }
        } else if (c == '_' && (lex_cur(lx) == '\n' || (lex_cur(lx) == '\r' && lex_peek1(lx) == '\n'))) {
            if (lex_cur(lx) == '\r') lex_advance(lx);
            lex_advance(lx);
            while (lex_cur(lx) == ' ' || lex_cur(lx) == '\t') lex_advance(lx);
        } else {
            buf_push(&buf, &len, &cap, c);
        }
    }
    if (!lex_cur(lx)) { ct_drop(buf); return make_error(lx, "unterminated string literal"); }
    lex_advance(lx);
    if (!buf) buf = ct_strdup("");
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
                case 'x': { int v = 0, nd = 0;
                    while (nd < 2 && isxdigit((unsigned char)lex_cur(lx))) { char h = lex_advance(lx); v = v * 16 + (isdigit((unsigned char)h) ? h - '0' : (tolower((unsigned char)h) - 'a' + 10)); nd++; }
                    c = (char)v; break; }
                case '^': { c = (char)((lex_cur(lx) ? lex_advance(lx) : 0) & 0x1f); break; }
                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': { int v = esc - '0', nd = 1;
                    while (nd < 3 && lex_cur(lx) >= '0' && lex_cur(lx) <= '7') { v = v * 8 + (lex_advance(lx) - '0'); nd++; }
                    c = (char)v; break; }
                default:   c = icn_esc_simple(esc); break;
            }
        }
        buf_push(&buf, &len, &cap, c);
    }
    if (!lex_cur(lx)) { ct_drop(buf); return make_error(lx, "unterminated cset literal"); }
    lex_advance(lx);
    if (!buf) buf = ct_strdup("");
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
        ct_drop(buf);
        IcnToken t = make_tok(TK_INT, line, col);
        t.val.ival = val;
        return t;
    }
    while (isdigit((unsigned char)lex_cur(lx)))
        buf_push(&buf, &len, &cap, lex_advance(lx));
    if ((lex_cur(lx) == 'r' || lex_cur(lx) == 'R') && !is_real) {
        int radix = (int)strtol(buf, NULL, 10);
        ct_drop(buf); buf = NULL; len = 0; cap = 0;
        lex_advance(lx);
        while (isalnum((unsigned char)lex_cur(lx)))
            buf_push(&buf, &len, &cap, lex_advance(lx));
        if (!buf) buf = ct_strdup("0");
        unsigned long long val = 0; int big = 0;
        for (int i = 0; i < len; i++) {
            char c = buf[i];
            int d = isdigit((unsigned char)c) ? c - '0'
                  : islower((unsigned char)c) ? c - 'a' + 10
                  : c - 'A' + 10;
            if (val > (9223372036854775807ULL - (unsigned)d) / (unsigned)radix) big = 1;
            val = val * (unsigned)radix + (unsigned)d;
        }
        if (big) {
            size_t bl = strlen(buf); char *txt = (char *)ct_alloc(bl + 16); snprintf(txt, bl + 16, "%dr%s", radix, buf); ct_drop(buf);
            IcnToken t = make_tok(TK_BIGINT, line, col); t.val.sval.data = txt; t.val.sval.len = strlen(txt);
            return t;
        }
        ct_drop(buf);
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
        errno = 0;
        long _iv = strtol(buf, NULL, 10);
        if (errno == ERANGE) { t = make_tok(TK_BIGINT, line, col); t.val.sval.data = ct_strdup(buf); t.val.sval.len = strlen(buf); }
        else { t = make_tok(TK_INT, line, col); t.val.ival = _iv; }
    }
    ct_drop(buf);
    return t;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IcnToken scan_ident(IcnLexer *lx) {
    int line = lx->line, col = lx->col;
    char *buf = NULL; int len = 0, cap = 0;
    while (isalnum((unsigned char)lex_cur(lx)) || lex_cur(lx) == '_')
        buf_push(&buf, &len, &cap, lex_advance(lx));
    if (!buf) buf = ct_strdup("");
    IcnTkKind kind = lookup_keyword(buf);
    IcnToken t = make_tok(kind, line, col);
    if (kind == TK_IDENT) {
        t.val.sval.data = buf;
        t.val.sval.len  = (size_t)len;
    } else {
        ct_drop(buf);
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
                    if (lex_cur(lx) == ':' && lex_peek1(lx) == '=') {
                        lex_advance(lx); lex_advance(lx);
                        return make_tok(TK_AUGIDENTICAL, line, col);
                    }
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
                    if (lex_cur(lx) == '=') {
                        lex_advance(lx);
                        if (lex_cur(lx) == ':' && lex_peek1(lx) == '=') {
                            lex_advance(lx); lex_advance(lx);
                            return make_tok(TK_AUGNOTIDENT, line, col);
                        }
                        return make_tok(TK_NOTIDENT, line, col);
                    }
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
                if (lex_cur(lx) == '|') {
                    lex_advance(lx);
                    if (lex_cur(lx) == ':' && lex_peek1(lx) == '=') {
                        lex_advance(lx); lex_advance(lx);
                        return make_tok(TK_AUGLCONCAT, line, col);
                    }
                    return make_tok(TK_LCONCAT, line, col);
                }
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
        case '&':
            if (lex_cur(lx) == ':' && lex_peek1(lx) == '=') {
                lex_advance(lx); lex_advance(lx);
                return make_tok(TK_AUGAND, line, col);
            }
            return make_tok(TK_AND, line, col);
        case '\\': return make_tok(TK_BACKSLASH, line, col);
        case '!': return make_tok(TK_BANG, line, col);
        case '?':
            if (lex_cur(lx) == ':' && lex_peek1(lx) == '=') {
                lex_advance(lx); lex_advance(lx);
                return make_tok(TK_AUGSCAN, line, col);
            }
            return make_tok(TK_QMARK, line, col);
        case '@':
            if (lex_cur(lx) == ':' && lex_peek1(lx) == '=') {
                lex_advance(lx); lex_advance(lx);
                return make_tok(TK_AUGAT, line, col);
            }
            return make_tok(TK_AT, line, col);
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
        case '$':
            if (lex_cur(lx) == '(') { lex_advance(lx); return make_tok(TK_LBRACE, line, col); }
            if (lex_cur(lx) == ')') { lex_advance(lx); return make_tok(TK_RBRACE, line, col); }
            if (lex_cur(lx) == '<') { lex_advance(lx); return make_tok(TK_LBRACK, line, col); }
            if (lex_cur(lx) == '>') { lex_advance(lx); return make_tok(TK_RBRACK, line, col); }
            while (lex_cur(lx) && lex_cur(lx) != '\n') lex_advance(lx);
            return lex_one(lx);
        default: {
            char msg[64];
            snprintf(msg, sizeof(msg), "unexpected character '%c' (0x%02x)", c, (unsigned char)c);
            return make_error(lx, msg);
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct IcnPpDef { struct IcnPpDef *next; int nlen, vlen; char inuse; char *s; } IcnPpDef;
typedef struct IcnPpFile { struct IcnPpFile *prev; char *fname; long lno; FILE *fp; const char *mem; size_t mlen, mpos; int ifdepth; } IcnPpFile;
typedef struct IcnPpBuf { struct IcnPpBuf *prev; IcnPpDef *defn; char *ptr, *stop, *lim; } IcnPpBuf;
typedef struct {
    IcnPpFile nofile, *curfile; IcnPpBuf *bstack, *bfree; char *buf, *bnxt, *bstop, *blim; IcnPpDef **cbin;
    char *lbuf; size_t llen; char *tbuf; size_t tcap; int ifdepth, fatals;
} IcnPp;
static char icn_pp_src[1024] = "";
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void icn_pp_set_source_path(const char *path) {
    if (!path) { icn_pp_src[0] = 0; return; }
    size_t pl = strlen(path); if (pl >= sizeof icn_pp_src) pl = sizeof icn_pp_src - 1;
    memcpy(icn_pp_src, path, pl); icn_pp_src[pl] = 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void icn_pp_source_base(char *out, size_t n) {
    if (!out || n == 0) return;
    const char *slash = strrchr(icn_pp_src, '/');
    const char *base  = slash ? slash + 1 : icn_pp_src;
    size_t bl = strlen(base);
    if (bl >= n) bl = n - 1;
    memcpy(out, base, bl); out[bl] = 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void ipp_pfatal(IcnPp *pp, const char *s1, char *s2) {
    fprintf(stderr, "File %s; Line %ld # ", pp->curfile->fname, pp->curfile->lno);
    if (s2 && *s2) { size_t n = strlen(s2); if (n > 0 && s2[n - 1] == '\n') s2[n - 1] = '\0'; fprintf(stderr, "\"%s\": ", s2); }
    fprintf(stderr, "%s\n", s1);
    pp->fatals++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IcnPpDef *ipp_dlookup(IcnPp *pp, char *name, int len, char *val) {
    if (len < 0) len = (int)strlen(name);
    if (len == 0) return NULL;
    unsigned int t = 0; for (int i = 0; i < len; i++) t = 37 * t + (unsigned char)name[i];
    IcnPpDef **p = &pp->cbin[t % 256], *d;
    while ((d = *p) != NULL) {
        if (d->nlen == len && strncmp(name, d->s, (size_t)len) == 0) {
            if (val == NULL) { *p = d->next; ct_drop(d->s); ct_drop(d); return NULL; }
            if (val != name && strcmp(val, d->s + d->nlen) != 0) ipp_pfatal(pp, "value redefined", name);
            return d;
        }
        p = &d->next;
    }
    if (val == name || val == NULL) return NULL;
    size_t nlen = strlen(name), vlen = strlen(val);
    d = (IcnPpDef *)ct_zalloc(1, sizeof *d);
    d->nlen = (int)nlen; d->vlen = (int)vlen; d->inuse = 0;
    d->s = (char *)ct_alloc(nlen + vlen + 1); memcpy(d->s, name, nlen); memcpy(d->s + nlen, val, vlen + 1);
    d->next = pp->cbin[t % 256]; pp->cbin[t % 256] = d;
    return d;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int ipp_getc(IcnPpFile *f) {
    if (f->fp) return getc(f->fp);
    return f->mpos < f->mlen ? (unsigned char)f->mem[f->mpos++] : EOF;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *ipp_rline(IcnPp *pp, IcnPpFile *f) {
    int c = ipp_getc(f);
    if (c == EOF) return NULL;
    size_t n = 0;
    for (;;) {
        if (n + 3 > pp->llen) { pp->llen = pp->llen ? pp->llen * 2 : 128; pp->lbuf = (char *)ct_grow(pp->lbuf, pp->llen); }
        if (c == '\n' || c == EOF) break;
        pp->lbuf[n++] = (char)c;
        c = ipp_getc(f);
    }
    pp->lbuf[n++] = '\n'; pp->lbuf[n] = '\0';
    return pp->lbuf;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void ipp_pushdef(IcnPp *pp, IcnPpDef *d) {
    d->inuse = 1;
    IcnPpBuf *b = pp->bfree;
    if (b == NULL) b = (IcnPpBuf *)ct_zalloc(1, sizeof *b); else pp->bfree = b->prev;
    b->prev = pp->bstack; b->defn = d; b->ptr = pp->bnxt; b->stop = pp->bstop; b->lim = pp->blim;
    pp->bstack = b;
    pp->bnxt = pp->bstop = d->s + d->nlen;
    pp->blim = pp->bnxt + d->vlen;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void ipp_pushline(IcnPp *pp, const char *fname, long lno) {
    size_t need = strlen(fname) + 40;
    if (need > pp->tcap) { pp->tcap = need * 2; pp->tbuf = (char *)ct_grow(pp->tbuf, pp->tcap); }
    snprintf(pp->tbuf, pp->tcap, "#line %ld \"%s\"\n", lno, fname);
    pp->bnxt = pp->tbuf;
    pp->bstop = pp->blim = pp->tbuf + strlen(pp->tbuf);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void ipp_push_file(IcnPp *pp, IcnPpFile *fs, const char *fname) {
    fs->prev = pp->curfile; fs->fname = ct_strdup(fname); fs->lno = 0; fs->ifdepth = pp->ifdepth;
    ipp_pushline(pp, fs->fname, 0L);
    pp->curfile = fs;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int ipp_ppopen(IcnPp *pp, char *fname) {
    for (IcnPpFile *fs = pp->curfile; fs->fname != NULL; fs = fs->prev)
        if (strcmp(fname, fs->fname) == 0) { ipp_pfatal(pp, "circular include", fname); return 1; }
    FILE *f = fopen(fname, "r");
    if (f == NULL) return 0;
    IcnPpFile *fs = (IcnPpFile *)ct_zalloc(1, sizeof *fs);
    fs->fp = f;
    ipp_push_file(pp, fs, fname);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *ipp_wskip(char *s) {
    char c;
    while (isspace((unsigned char)(c = *s))) s++;
    if (c == '#') while ((c = *++s) != 0) ;
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *ipp_nskip(char *s) {
    char c;
    while (isdigit((unsigned char)(c = *++s))) ;
    if (c == 'r' || c == 'R') { while (isalnum((unsigned char)(c = *++s))) ; return s; }
    if (c == '.') while (isdigit((unsigned char)(c = *++s))) ;
    if (c == 'e' || c == 'E') { c = s[1]; if (c == '+' || c == '-') s++; while (isdigit((unsigned char)(c = *++s))) ; }
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *ipp_matchq(char *s) {
    char c, q = *s;
    if (q == '\0') return s;
    while ((c = *++s) != q && c != '\0') if (c == '\\') if (*++s == '\0') return s;
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *ipp_getidt(char *dst, char *src) {
    char c;
    while (isalnum((unsigned char)(c = *src)) || c == '_') { *dst++ = c; src++; }
    *dst = '\0';
    return src;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *ipp_getfnm(char *dst, char *src) {
    if (*src != '"') return ipp_getidt(dst, src);
    char *lim = ipp_matchq(src);
    if (*lim != '"') { *dst = '\0'; return lim; }
    while (++src < lim) if ((*dst++ = *src) == '\\') dst[-1] = *++src;
    *dst = '\0';
    return lim + 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *ipp_tryfile(const char *dir, const char *name) {
    size_t dl = dir ? strlen(dir) : 0, nl = strlen(name);
    char *path = (char *)ct_alloc(dl + nl + 2);
    if (dir && dl) { memcpy(path, dir, dl); path[dl] = '/'; memcpy(path + dl + 1, name, nl + 1); } else memcpy(path, name, nl + 1);
    FILE *f = fopen(path, "r");
    if (f) { fclose(f); return path; }
    ct_drop(path);
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *ipp_pathfind(const char *name) {
    char *hit = ipp_tryfile(NULL, name);
    if (hit) return hit;
    const char *lp = getenv("LPATH");
    if (!lp) lp = ".";
    while (*lp) {
        while (*lp == ' ' || *lp == ':') lp++;
        const char *e = lp; while (*e && *e != ' ' && *e != ':') e++;
        if (e > lp) { char *dir = ct_strndup(lp, (size_t)(e - lp)); hit = ipp_tryfile(dir, name); ct_drop(dir); if (hit) return hit; }
        lp = e;
    }
    size_t cap = 256; char *exe = NULL; ssize_t el;
    for (;;) { exe = (char *)ct_grow(exe, cap); el = readlink("/proc/self/exe", exe, cap); if (el < 0 || (size_t)el < cap) break; cap *= 2; }
    if (el > 0) {
        exe[el] = '\0';
        char *es = strrchr(exe, '/');
        if (es) {
            *es = '\0';
            for (int k = 0; k < 2 && !hit; k++) {
                const char *sub = k ? "../corpus/packages/icon/ipl/gincl" : "../corpus/packages/icon/ipl/incl";
                size_t dl = strlen(exe) + strlen(sub) + 2; char *dir = (char *)ct_alloc(dl);
                snprintf(dir, dl, "%s/%s", exe, sub); hit = ipp_tryfile(dir, name); ct_drop(dir);
            }
        }
    }
    ct_drop(exe);
    if (hit) return hit;
    const char *sl = strrchr(icn_pp_src, '/');
    if (sl && name[0] != '/') { char *dir = ct_strndup(icn_pp_src, (size_t)(sl - icn_pp_src)); hit = ipp_tryfile(dir, name); ct_drop(dir); if (hit) return hit; }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *ipp_define(IcnPp *pp, char *s) {
    char c, *name, *val;
    if (isalpha((unsigned char)(c = *s)) || c == '_') s = ipp_getidt(name = s - 1, s);
    else return "$define: missing name";
    if (*s == '(') return "$define: \"(\" after name requires preceding space";
    val = s = ipp_wskip(s);
    if (*s != '\0') {
        while ((c = *s) != '\0' && c != '#') {
            if (c == '"' || c == '\'') { s = ipp_matchq(s); if (*s == '\0') return "$define: unterminated literal"; }
            s++;
        }
        while (isspace((unsigned char)s[-1])) s--;
    }
    *s = '\0';
    ipp_dlookup(pp, name, -1, val);
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *ipp_undef(IcnPp *pp, char *s) {
    char c, *name;
    if (isalpha((unsigned char)(c = *s)) || c == '_') s = ipp_getidt(name = s - 1, s);
    else return "$undef: missing name";
    if (*ipp_wskip(s) != '\0') return "$undef: too many arguments";
    ipp_dlookup(pp, name, -1, NULL);
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *ipp_include(IcnPp *pp, char *s) {
    char *fname;
    s = ipp_getfnm(fname = s - 1, s);
    if (*fname == '\0') return "$include: invalid file name";
    if (*ipp_wskip(s) != '\0') return "$include: too many arguments";
    char *full = ipp_pathfind(fname);
    if (!full || !ipp_ppopen(pp, full)) ipp_pfatal(pp, "cannot open", fname);
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *ipp_setline(IcnPp *pp, char *s) {
    long n; char c, *fname;
    if (!isdigit((unsigned char)(c = *s))) return "$line: no line number";
    n = c - '0';
    while (isdigit((unsigned char)(c = *++s))) n = 10 * n + c - '0';
    s = ipp_wskip(s);
    if (isalpha((unsigned char)(c = *s)) || c == '_' || c == '"') { s = ipp_getfnm(fname = s - 1, s); if (*fname == '\0') return "$line: invalid file name"; }
    else fname = NULL;
    if (*ipp_wskip(s) != '\0') return "$line: too many arguments";
    pp->curfile->lno = n;
    if (fname != NULL) pp->curfile->fname = ct_strdup(fname);
    ipp_pushline(pp, pp->curfile->fname, pp->curfile->lno);
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void ipp_skipcode(IcnPp *pp, int doelse, int report);
static void ipp_ppdir(IcnPp *pp, char *s);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *ipp_ifxdef(IcnPp *pp, char *s, int f) {
    char c, *name;
    pp->ifdepth++;
    if (isalpha((unsigned char)(c = *s)) || c == '_') s = ipp_getidt(name = s - 1, s);
    else return "$ifdef/$ifndef: missing name";
    if (*ipp_wskip(s) != '\0') return "$ifdef/$ifndef: too many arguments";
    if ((ipp_dlookup(pp, name, -1, name) != NULL) ^ f) ipp_skipcode(pp, 1, 1);
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *ipp_elsedir(IcnPp *pp, char *s) {
    if (pp->ifdepth <= pp->curfile->ifdepth) return "unexpected $else";
    if (*s != '\0') ipp_pfatal(pp, "extraneous arguments on $else/$endif", s);
    ipp_skipcode(pp, 0, 1);
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *ipp_endif(IcnPp *pp, char *s) {
    if (pp->ifdepth <= pp->curfile->ifdepth) return "unexpected $endif";
    if (*s != '\0') ipp_pfatal(pp, "extraneous arguments on $else/$endif", s);
    pp->ifdepth--;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void ipp_skipcode(IcnPp *pp, int doelse, int report) {
    char c, *p, *cmd;
    while ((p = pp->buf = ipp_rline(pp, pp->curfile)) != NULL) {
        pp->curfile->lno++;
        if (pp->buf[1] == 'l' && pp->buf[2] == 'i' && pp->buf[3] == 'n' && pp->buf[4] == 'e' && pp->buf[0] == '#' && pp->buf[5] == ' ') { ipp_ppdir(pp, pp->buf + 1); continue; }
        while (isspace((unsigned char)(c = *p))) p++;
        if (c != '$' || (ispunct((unsigned char)p[1]) && p[1] != ' ')) continue;
        p = ipp_wskip(p + 1);
        p = ipp_getidt(cmd = p - 1, p);
        p = ipp_wskip(p);
        if (cmd[0] == 'i' && cmd[1] == 'f') { pp->ifdepth++; ipp_skipcode(pp, 0, 0); }
        else if (strcmp(cmd, "line") == 0) ipp_setline(pp, p);
        else if (strcmp(cmd, "endif") == 0 || (doelse == 1 && strcmp(cmd, "else") == 0)) {
            if (*p != '\0') ipp_pfatal(pp, "extraneous arguments on $else/$endif", p);
            if (cmd[1] == 'n') pp->ifdepth--;
            if (report) ipp_pushline(pp, pp->curfile->fname, pp->curfile->lno);
            return;
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void ipp_ppdir(IcnPp *pp, char *s) {
    char b0 = pp->buf[0], *cmd; const char *errmsg;
    pp->bnxt = (char *)"\n"; pp->bstop = pp->blim = pp->bnxt + 1;
    s = ipp_wskip(s);
    s = ipp_getidt(cmd = s - 1, s);
    s = ipp_wskip(s);
    int is_setline = 0;
    if      (!strcmp(cmd, "define"))  errmsg = ipp_define(pp, s);
    else if (!strcmp(cmd, "undef"))   errmsg = ipp_undef(pp, s);
    else if (!strcmp(cmd, "ifdef"))   errmsg = ipp_ifxdef(pp, s, 1);
    else if (!strcmp(cmd, "ifndef"))  errmsg = ipp_ifxdef(pp, s, 0);
    else if (!strcmp(cmd, "else"))    errmsg = ipp_elsedir(pp, s);
    else if (!strcmp(cmd, "endif"))   errmsg = ipp_endif(pp, s);
    else if (!strcmp(cmd, "include")) errmsg = ipp_include(pp, s);
    else if (!strcmp(cmd, "line"))  { errmsg = ipp_setline(pp, s); is_setline = 1; }
    else if (!strcmp(cmd, "error"))   { ipp_pfatal(pp, "explicit $error", s); errmsg = NULL; }
    else { ipp_pfatal(pp, "invalid preprocessing directive", cmd); return; }
    if (errmsg != NULL && (!is_setline || b0 != '#')) ipp_pfatal(pp, errmsg, NULL);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int ipp_ppch(IcnPp *pp) {
    int c, f; char *p; IcnPpDef *d;
    for (;;) {
        if (pp->bnxt < pp->bstop) return ((int)*pp->bnxt++) & 0xFF;
        if (pp->bnxt < pp->blim) {
            f = (unsigned char)*pp->bnxt;
            if (isalpha(f) || f == '_') {
                p = pp->bnxt + 1;
                while (p < pp->blim && (isalnum(c = (unsigned char)*p) || c == '_')) p++;
                pp->bstop = p;
                if ((d = ipp_dlookup(pp, pp->bnxt, (int)(p - pp->bnxt), pp->bnxt)) == NULL || d->inuse == 1) { pp->bnxt++; return f; }
                pp->bnxt = p;
                ipp_pushdef(pp, d);
                continue;
            }
            p = pp->bnxt++;
            while (p < pp->blim) {
                c = (unsigned char)*p;
                if (isalpha(c) || c == '_') { pp->bstop = p; return f; }
                else if (isdigit(c)) p = ipp_nskip(p);
                else if (c == '#') { pp->bstop = pp->blim; return f; }
                else if (c == '"' || c == '\'') { p = ipp_matchq(p); if (*p != '\0') p++; }
                else p++;
            }
            pp->bstop = pp->blim;
            return f;
        }
        if (pp->bstack != NULL) {
            IcnPpBuf *b = pp->bstack;
            b->defn->inuse = 0;
            pp->bnxt = b->ptr; pp->bstop = b->stop; pp->blim = b->lim;
            pp->bstack = b->prev; b->prev = pp->bfree; pp->bfree = b;
            continue;
        }
        if ((pp->buf = ipp_rline(pp, pp->curfile)) != NULL) {
            p = pp->bnxt = pp->bstop = pp->blim = pp->buf;
            pp->curfile->lno++;
            while (isspace(c = (unsigned char)*p)) p++;
            if (c == '$' && (!ispunct((unsigned char)p[1]) || p[1] == ' ')) ipp_ppdir(pp, p + 1);
            else if (pp->buf[1] == 'l' && pp->buf[2] == 'i' && pp->buf[3] == 'n' && pp->buf[4] == 'e' && pp->buf[0] == '#' && pp->buf[5] == ' ') ipp_ppdir(pp, p + 1);
            else { pp->bnxt = pp->buf; pp->blim = pp->buf + strlen(pp->buf); pp->bstop = pp->bnxt; }
        } else {
            if (pp->curfile->ifdepth != pp->ifdepth) { ipp_pfatal(pp, "unterminated $if", NULL); pp->ifdepth = pp->curfile->ifdepth; }
            IcnPpFile *fs = pp->curfile;
            pp->curfile = fs->prev;
            if (fs->fp && fs->fp != stdin) fclose(fs->fp);
            if (pp->curfile == &pp->nofile) return EOF;
            ipp_pushline(pp, pp->curfile->fname, pp->curfile->lno);
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void ipp_init(IcnPp *pp) {
    static const char *pre[] = { "_UNIX", "_ASCII", "_CO_EXPRESSIONS", "_KEYBOARD_FUNCTIONS", "_LARGE_INTEGERS", "_PIPES", "_SYSTEM_FUNCTION" };
    memset(pp, 0, sizeof *pp);
    pp->cbin = (IcnPpDef **)ct_zalloc(256, sizeof *pp->cbin);
    pp->curfile = &pp->nofile;
    for (int k = 0; k < (int)(sizeof pre / sizeof *pre); k++) ipp_dlookup(pp, (char *)pre[k], -1, (char *)"1");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *icn_pp_text(const char *src, int *fatals) {
    IcnPp pp; ipp_init(&pp);
    IcnPpFile *fs = (IcnPpFile *)ct_zalloc(1, sizeof *fs);
    fs->mem = src; fs->mlen = strlen(src);
    ipp_push_file(&pp, fs, icn_pp_src[0] ? icn_pp_src : "stdin");
    char *out = NULL; int olen = 0, ocap = 0, c;
    while ((c = ipp_ppch(&pp)) != EOF) buf_push(&out, &olen, &ocap, (char)c);
    if (!out) out = ct_strdup("");
    if (fatals) *fatals = pp.fatals;
    return out;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int icn_pp_echo(const char *path) {
    IcnPp pp; ipp_init(&pp);
    int is_stdin = !strcmp(path, "-");
    FILE *f = is_stdin ? stdin : fopen(path, "r");
    if (!f) { fprintf(stderr, "scrip: cannot open %s\n", path); return -1; }
    IcnPpFile *fs = (IcnPpFile *)ct_zalloc(1, sizeof *fs);
    fs->fp = f;
    ipp_push_file(&pp, fs, is_stdin ? "stdin" : path);
    fprintf(stderr, "%s:\n", is_stdin ? "stdin" : path);
    int c;
    while ((c = ipp_ppch(&pp)) != EOF) putchar(c);
    return pp.fatals;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void icn_lex_init(IcnLexer *lx, const char *src) {
    memset(lx, 0, sizeof(*lx));
    if (src) src = icn_pp_text(src, &lx->pp_fatals);
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
        case TK_BIGINT:    return "BIGINT";
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
        case TK_AUGLCONCAT: return "|||:=";
        case TK_AUGAT: return "@:=";
        case TK_AUGIDENTICAL: return "===:=";
        case TK_AUGNOTIDENT: return "~===:=";
        case TK_AUGAND: return "&:=";
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
