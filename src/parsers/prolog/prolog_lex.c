#include "prolog_lex.h"
#include "ct_arena.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#define PEEK(n)  ((unsigned char)p[(n)])
#define ADV(n)   (p += (n))
#define NL()     (lx->line++)
#define SYNC()   (lx->pos = (int)(p - lx->src))
enum { CC_DIGIT = 1, CC_UPPER = 2, CC_LOWER = 4, CC_USCORE = 8, CC_LAYOUT = 16, CC_GRAPHIC = 32 };
static const unsigned char CC[256] = {
    [' '] = CC_LAYOUT, ['\t'] = CC_LAYOUT, ['\n'] = CC_LAYOUT, ['\r'] = CC_LAYOUT, ['\f'] = CC_LAYOUT, ['\v'] = CC_LAYOUT, ['_'] = CC_USCORE,
    ['0'] = CC_DIGIT, ['1'] = CC_DIGIT, ['2'] = CC_DIGIT, ['3'] = CC_DIGIT, ['4'] = CC_DIGIT, ['5'] = CC_DIGIT, ['6'] = CC_DIGIT, ['7'] = CC_DIGIT, ['8'] = CC_DIGIT, ['9'] = CC_DIGIT,
    ['A'] = CC_UPPER, ['B'] = CC_UPPER, ['C'] = CC_UPPER, ['D'] = CC_UPPER, ['E'] = CC_UPPER, ['F'] = CC_UPPER, ['G'] = CC_UPPER, ['H'] = CC_UPPER, ['I'] = CC_UPPER, ['J'] = CC_UPPER,
    ['K'] = CC_UPPER, ['L'] = CC_UPPER, ['M'] = CC_UPPER, ['N'] = CC_UPPER, ['O'] = CC_UPPER, ['P'] = CC_UPPER, ['Q'] = CC_UPPER, ['R'] = CC_UPPER, ['S'] = CC_UPPER, ['T'] = CC_UPPER,
    ['U'] = CC_UPPER, ['V'] = CC_UPPER, ['W'] = CC_UPPER, ['X'] = CC_UPPER, ['Y'] = CC_UPPER, ['Z'] = CC_UPPER,
    ['a'] = CC_LOWER, ['b'] = CC_LOWER, ['c'] = CC_LOWER, ['d'] = CC_LOWER, ['e'] = CC_LOWER, ['f'] = CC_LOWER, ['g'] = CC_LOWER, ['h'] = CC_LOWER, ['i'] = CC_LOWER, ['j'] = CC_LOWER,
    ['k'] = CC_LOWER, ['l'] = CC_LOWER, ['m'] = CC_LOWER, ['n'] = CC_LOWER, ['o'] = CC_LOWER, ['p'] = CC_LOWER, ['q'] = CC_LOWER, ['r'] = CC_LOWER, ['s'] = CC_LOWER, ['t'] = CC_LOWER,
    ['u'] = CC_LOWER, ['v'] = CC_LOWER, ['w'] = CC_LOWER, ['x'] = CC_LOWER, ['y'] = CC_LOWER, ['z'] = CC_LOWER,
    ['+'] = CC_GRAPHIC, ['-'] = CC_GRAPHIC, ['*'] = CC_GRAPHIC, ['/'] = CC_GRAPHIC, ['\\'] = CC_GRAPHIC, ['^'] = CC_GRAPHIC, ['<'] = CC_GRAPHIC, ['>'] = CC_GRAPHIC, ['='] = CC_GRAPHIC,
    ['~'] = CC_GRAPHIC, ['?'] = CC_GRAPHIC, ['@'] = CC_GRAPHIC, ['#'] = CC_GRAPHIC, ['&'] = CC_GRAPHIC, [':'] = CC_GRAPHIC, ['$'] = CC_GRAPHIC, ['.'] = CC_GRAPHIC,
};
enum { D_OTHER = 0, D_EOF, D_LAYOUT, D_NL, D_PERCENT, D_SLASH, D_SQUOTE, D_DQUOTE, D_BQUOTE, D_UPPER, D_USCORE, D_ZERO, D_DIGIT, D_LOWER,
       D_LPAREN, D_RPAREN, D_LBRACK, D_RBRACK, D_PIPE, D_COMMA, D_BANG, D_SEMI, D_LBRACE, D_RBRACE, D_DOT, D_GRAPHIC };
static const unsigned char DC[256] = {
    [0] = D_EOF, [' '] = D_LAYOUT, ['\t'] = D_LAYOUT, ['\r'] = D_LAYOUT, ['\f'] = D_LAYOUT, ['\v'] = D_LAYOUT, ['\n'] = D_NL, ['%'] = D_PERCENT, ['/'] = D_SLASH,
    ['\''] = D_SQUOTE, ['"'] = D_DQUOTE, ['`'] = D_BQUOTE, ['_'] = D_USCORE, ['0'] = D_ZERO,
    ['1'] = D_DIGIT, ['2'] = D_DIGIT, ['3'] = D_DIGIT, ['4'] = D_DIGIT, ['5'] = D_DIGIT, ['6'] = D_DIGIT, ['7'] = D_DIGIT, ['8'] = D_DIGIT, ['9'] = D_DIGIT,
    ['A'] = D_UPPER, ['B'] = D_UPPER, ['C'] = D_UPPER, ['D'] = D_UPPER, ['E'] = D_UPPER, ['F'] = D_UPPER, ['G'] = D_UPPER, ['H'] = D_UPPER, ['I'] = D_UPPER, ['J'] = D_UPPER,
    ['K'] = D_UPPER, ['L'] = D_UPPER, ['M'] = D_UPPER, ['N'] = D_UPPER, ['O'] = D_UPPER, ['P'] = D_UPPER, ['Q'] = D_UPPER, ['R'] = D_UPPER, ['S'] = D_UPPER, ['T'] = D_UPPER,
    ['U'] = D_UPPER, ['V'] = D_UPPER, ['W'] = D_UPPER, ['X'] = D_UPPER, ['Y'] = D_UPPER, ['Z'] = D_UPPER,
    ['a'] = D_LOWER, ['b'] = D_LOWER, ['c'] = D_LOWER, ['d'] = D_LOWER, ['e'] = D_LOWER, ['f'] = D_LOWER, ['g'] = D_LOWER, ['h'] = D_LOWER, ['i'] = D_LOWER, ['j'] = D_LOWER,
    ['k'] = D_LOWER, ['l'] = D_LOWER, ['m'] = D_LOWER, ['n'] = D_LOWER, ['o'] = D_LOWER, ['p'] = D_LOWER, ['q'] = D_LOWER, ['r'] = D_LOWER, ['s'] = D_LOWER, ['t'] = D_LOWER,
    ['u'] = D_LOWER, ['v'] = D_LOWER, ['w'] = D_LOWER, ['x'] = D_LOWER, ['y'] = D_LOWER, ['z'] = D_LOWER,
    ['('] = D_LPAREN, [')'] = D_RPAREN, ['['] = D_LBRACK, [']'] = D_RBRACK, ['|'] = D_PIPE, [','] = D_COMMA, ['!'] = D_BANG, [';'] = D_SEMI, ['{'] = D_LBRACE, ['}'] = D_RBRACE,
    ['.'] = D_DOT, ['+'] = D_GRAPHIC, ['-'] = D_GRAPHIC, ['*'] = D_GRAPHIC, ['\\'] = D_GRAPHIC, ['^'] = D_GRAPHIC, ['<'] = D_GRAPHIC, ['>'] = D_GRAPHIC, ['='] = D_GRAPHIC,
    ['~'] = D_GRAPHIC, ['?'] = D_GRAPHIC, ['@'] = D_GRAPHIC, ['#'] = D_GRAPHIC, ['&'] = D_GRAPHIC, [':'] = D_GRAPHIC, ['$'] = D_GRAPHIC,
};
#define CLASS(c)      (CC[(unsigned char)(c)])
#define is_digit(c)   (CLASS(c) & CC_DIGIT)
#define is_upper(c)   (CLASS(c) & CC_UPPER)
#define is_lower(c)   (CLASS(c) & CC_LOWER)
#define is_alnum(c)   (CLASS(c) & (CC_DIGIT | CC_UPPER | CC_LOWER))
#define is_idcont(c)  (CLASS(c) & (CC_DIGIT | CC_UPPER | CC_LOWER | CC_USCORE))
#define is_layout(c)  (CLASS(c) & CC_LAYOUT)
#define is_graphic(c) (CLASS(c) & CC_GRAPHIC)
static int hexval(int c) { if (c >= '0' && c <= '9') return c - '0'; if (c >= 'a' && c <= 'f') return c - 'a' + 10; if (c >= 'A' && c <= 'F') return c - 'A' + 10; return -1; }
static int digval(int c) { if (c >= '0' && c <= '9') return c - '0'; if (c >= 'a' && c <= 'z') return c - 'a' + 10; if (c >= 'A' && c <= 'Z') return c - 'A' + 10; return -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline Token tok(TkKind kind, char *text, int line) {
    Token t; t.kind = kind; t.text = text; t.ival = 0; t.fval = 0.0; t.line = line; t.big = 0; t.adj = 0; t.len = -1;
    return t;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static Token err_tok(int line, const char *msg) {
    return tok(TK_ERROR, ct_strdup(msg), line);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *span_text(const char *s, const char *e, int seps) {
    if (!seps) return ct_strndup(s, (size_t)(e - s));
    char *o = (char *)ct_alloc((size_t)(e - s) + 1); size_t n = 0;
    for (; s < e; s++) if (*s != '_' && *s != ' ') o[n++] = *s;
    o[n] = '\0';
    return o;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline void sb_put(Lexer *lx, int c) {
    if (lx->slen + 1 >= lx->scap) { lx->scap = lx->scap ? lx->scap * 2 : 256; lx->sbuf = (char *)ct_grow(lx->sbuf, (size_t)lx->scap); }
    lx->sbuf[lx->slen++] = (char)c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sb_utf8(Lexer *lx, int cp) {
    if (cp < 0x80) { sb_put(lx, cp); return; }
    if (cp < 0x800) { sb_put(lx, 0xC0 | (cp >> 6)); sb_put(lx, 0x80 | (cp & 0x3F)); return; }
    if (cp < 0x10000) { sb_put(lx, 0xE0 | (cp >> 12)); sb_put(lx, 0x80 | ((cp >> 6) & 0x3F)); sb_put(lx, 0x80 | (cp & 0x3F)); return; }
    sb_put(lx, 0xF0 | (cp >> 18)); sb_put(lx, 0x80 | ((cp >> 12) & 0x3F)); sb_put(lx, 0x80 | ((cp >> 6) & 0x3F)); sb_put(lx, 0x80 | (cp & 0x3F));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *sb_text(Lexer *lx) {
    char *b = (char *)ct_alloc((size_t)lx->slen + 1);
    if (!b) return ct_strdup("");
    memcpy(b, lx->sbuf, (size_t)lx->slen); b[lx->slen] = 0;
    return b;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int u8_decode(const unsigned char *u, int *cp) {
    if ((u[0] & 0xE0) == 0xC0 && (u[1] & 0xC0) == 0x80) { *cp = ((u[0] & 0x1F) << 6) | (u[1] & 0x3F); return 2; }
    if ((u[0] & 0xF0) == 0xE0 && (u[1] & 0xC0) == 0x80 && (u[2] & 0xC0) == 0x80) { *cp = ((u[0] & 0x0F) << 12) | ((u[1] & 0x3F) << 6) | (u[2] & 0x3F); return 3; }
    if ((u[0] & 0xF8) == 0xF0 && (u[1] & 0xC0) == 0x80 && (u[2] & 0xC0) == 0x80 && (u[3] & 0xC0) == 0x80) {
        *cp = ((u[0] & 0x07) << 18) | ((u[1] & 0x3F) << 12) | ((u[2] & 0x3F) << 6) | (u[3] & 0x3F); return 4; }
    *cp = u[0]; return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int decode_escape(Lexer *lx, const char **pp, int *code) {
    const char *p = *pp; int e = PEEK(0), v = 0, h, i, want;
    if (e) ADV(1);
    if (e == '\n') NL();
    switch (e) {
        case 'a': *code = 7;   break;
        case 'b': *code = 8;   break;
        case 'f': *code = 12;  break;
        case 'n': *code = 10;  break;
        case 'r': *code = 13;  break;
        case 't': *code = 9;   break;
        case 'v': *code = 11;  break;
        case 'e': *code = 27;  break;
        case 's': *code = 32;  break;
        case 'd': *code = 127; break;
        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7':
            v = e - '0';
            while (PEEK(0) >= '0' && PEEK(0) <= '7') { v = v * 8 + (PEEK(0) - '0'); ADV(1); }
            if (PEEK(0) == '8' || PEEK(0) == '9') { *pp = p; return -1; }
            if (PEEK(0) == '\\') ADV(1);
            *code = v; break;
        case 'x':
            if (hexval(PEEK(0)) < 0) { *pp = p; return -1; }
            while ((h = hexval(PEEK(0))) >= 0) { v = v * 16 + h; ADV(1); }
            if (is_alnum(PEEK(0))) { *pp = p; return -1; }
            if (PEEK(0) == '\\') ADV(1);
            *code = v; break;
        case 'u': case 'U':
            want = (e == 'u') ? 4 : 8;
            for (i = 0; i < want; i++) { if ((h = hexval(PEEK(0))) < 0) { *pp = p; return -1; } v = v * 16 + h; ADV(1); }
            *code = v; break;
        case '\\': case '\'': case '"': case '`': *code = e; break;
        case 'c':  while (is_layout(PEEK(0))) { if (PEEK(0) == '\n') NL(); ADV(1); } *pp = p; return 0;
        case '\n': *pp = p; return 0;
        default:   *code = e; *pp = p; return -1;
    }
    *pp = p; return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int prolog_u_letter(const char *s, int *adv) {
    const unsigned char *u = (const unsigned char *)s; int cp, n;
    if (u[0] < 0x80) { *adv = 1; return 0; }
    if ((u[0] & 0xE0) == 0xC0 && (u[1] & 0xC0) == 0x80) { n = 2; cp = ((u[0] & 0x1F) << 6) | (u[1] & 0x3F); }
    else if ((u[0] & 0xF0) == 0xE0 && (u[1] & 0xC0) == 0x80 && (u[2] & 0xC0) == 0x80) { n = 3; cp = ((u[0] & 0x0F) << 12) | ((u[1] & 0x3F) << 6) | (u[2] & 0x3F); }
    else if ((u[0] & 0xF8) == 0xF0 && (u[1] & 0xC0) == 0x80 && (u[2] & 0xC0) == 0x80 && (u[3] & 0xC0) == 0x80) { n = 4; cp = ((u[0] & 0x07) << 18) | ((u[1] & 0x3F) << 12) | ((u[2] & 0x3F) << 6) | (u[3] & 0x3F); }
    else { *adv = 1; return 0; }
    *adv = n;
    if (cp < 0xC0 || cp == 0xD7 || cp == 0xF7 || (cp >= 0x2000 && cp < 0x2C00) || (cp >= 0x3000 && cp < 0x3040) || (cp >= 0xFE00 && cp < 0xFE70) || (cp >= 0xFF00 && cp < 0xFF10) || (cp >= 0xE000 && cp < 0xF900) || (cp >= 0x1F000 && cp < 0x1FB00)) return 0;
    if (cp <= 0xDE) return 1;
    if (cp <= 0xFF) return 2;
    if (cp < 0x180) { if (cp < 0x138) return (cp & 1) ? 2 : 1; if (cp >= 0x139 && cp < 0x149) return (cp & 1) ? 1 : 2; if (cp >= 0x14A && cp < 0x178) return (cp & 1) ? 2 : 1;
        if (cp == 0x178) return 1; if (cp >= 0x179 && cp < 0x17F) return (cp & 1) ? 1 : 2; return 2; }
    if (cp == 0x386 || (cp >= 0x388 && cp <= 0x38F && cp != 0x38B && cp != 0x38D) || (cp >= 0x391 && cp <= 0x3AB && cp != 0x3A2)) return 1;
    if (cp >= 0x400 && cp < 0x430) return 1;
    if (cp >= 0x460 && cp < 0x500) return (cp & 1) ? 2 : 1;
    if (cp >= 0x531 && cp <= 0x556) return 1;
    return 2;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static Token lexer_next_raw(Lexer *lx) {
    const char *p, *s; int line, seps = 0, radix = 0, ndig = 0, code = 0, st, c = 0, q = 0, n, uk; TkKind kind = TK_ATOM; Token t; char msg[32];
    if (lx->fenced) return tok(TK_EOF, (char *)"", lx->line);
    if (lx->has_peek) { lx->has_peek = 0; return lx->peek; }
    p = lx->src + lx->pos;
S_START:
    line = lx->line; s = p; c = PEEK(0);
    if (c >= 0x80)                                                                                                                             goto S_HI;
    switch (DC[c]) {
    case D_LAYOUT:                                                   {  ADV(1);                                                                goto S_START;       }
    case D_NL:                                                       {  NL(); ADV(1);                                                          goto S_START;       }
    case D_PERCENT:                                                  {  ADV(1);                                                                goto S_LCOMMENT;    }
    case D_SLASH:   if (PEEK(1) == '*')                              {  ADV(2);                                                                goto S_BCOMMENT;    }
                                                                     {  ADV(1);                                                                goto S_GRAPHIC;     }
    case D_EOF:                                                      {  SYNC(); return tok(TK_EOF, (char *)"", line);                                              }
    case D_SQUOTE:                                                   {  ADV(1); q = '\''; kind = TK_ATOM;     lx->slen = 0;                    goto S_QUOTED;      }
    case D_DQUOTE:                                                   {  ADV(1); q = '"';  kind = TK_STRING;   lx->slen = 0;                    goto S_QUOTED;      }
    case D_BQUOTE:                                                   {  ADV(1); q = '`';  kind = TK_BQSTRING; lx->slen = 0;                    goto S_QUOTED;      }
    case D_UPPER:                                                    {  ADV(1);                                                                goto S_VAR;         }
    case D_USCORE:                                                   {  ADV(1);                                                                goto S_UNDERSCORE;  }
    case D_ZERO:    if (PEEK(1) == '\'')                             {  ADV(2);                                                                goto S_CHARCODE;    }
                    if (PEEK(1) == 'x' || PEEK(1) == 'o' || PEEK(1) == 'b') {  radix = PEEK(1) == 'b' ? 2 : PEEK(1) == 'o' ? 8 : 16; ndig = 0; ADV(2); goto S_RADIX; }
                                                                     {  ADV(1);                                                                goto S_INT;         }
    case D_DIGIT:                                                    {  ADV(1);                                                                goto S_INT;         }
    case D_LOWER:                                                    {  ADV(1);                                                                goto S_WORD;        }
    case D_LPAREN:                                                   {  t = tok(TK_LPAREN, (char *)"(", line); t.adj = (p > lx->src && !is_layout((unsigned char)p[-1])); ADV(1); SYNC(); return t;                               }
    case D_RPAREN:                                                   {  ADV(1); SYNC(); return tok(TK_RPAREN,   (char *)")",  line);                               }
    case D_LBRACK:  if (PEEK(1) == ']')                              {  ADV(2); SYNC(); return tok(TK_ATOM,     (char *)"[]", line);                               }
                                                                     {  ADV(1); SYNC(); return tok(TK_LBRACKET, (char *)"[",  line);                               }
    case D_RBRACK:                                                   {  ADV(1); SYNC(); return tok(TK_RBRACKET, (char *)"]",  line);                               }
    case D_PIPE:                                                     {  ADV(1); SYNC(); return tok(TK_PIPE,     (char *)"|",  line);                               }
    case D_COMMA:                                                    {  ADV(1); SYNC(); return tok(TK_COMMA,    (char *)",",  line);                               }
    case D_BANG:                                                     {  ADV(1); SYNC(); return tok(TK_CUT,      (char *)"!",  line);                               }
    case D_SEMI:                                                     {  ADV(1); SYNC(); return tok(TK_SEMI,     (char *)";",  line);                               }
    case D_LBRACE:                                                   {  ADV(1); SYNC(); return tok(TK_LBRACE,   (char *)"{",  line);                               }
    case D_RBRACE:                                                   {  ADV(1); SYNC(); return tok(TK_RBRACE,   (char *)"}",  line);                               }
    case D_DOT:     if (PEEK(1) == '\0' || is_layout(PEEK(1)) || PEEK(1) == '%') {  ADV(1); SYNC(); return tok(TK_DOT, (char *)".", line);                         }
                                                                     {  ADV(1);                                                                goto S_GRAPHIC;     }
    case D_GRAPHIC:                                                  {  ADV(1);                                                                goto S_GRAPHIC;     }
    default:                                                         {  ADV(1);                                                                goto LX_UNEXPECTED; }
    }
S_HI:
    uk = prolog_u_letter(p, &n);
    if (uk == 1)                                                     {  ADV(n);                                                                goto S_VAR;         }
    if (uk == 2)                                                     {  ADV(n);                                                                goto S_WORD;        }
                                                                     {  ADV(1);                                                                goto LX_UNEXPECTED; }
S_LCOMMENT:
    if (PEEK(0) == '\0')                                                                                                                       goto S_START;
    if (PEEK(0) == '\n')                                                                                                                       goto S_START;
                                                                     {  ADV(1);                                                                goto S_LCOMMENT;    }
S_BCOMMENT:
    if (PEEK(0) == '\0')                                                                                                                       goto S_START;
    if (PEEK(0) == '*' && PEEK(1) == '/')                            {  ADV(2);                                                                goto S_START;       }
    if (PEEK(0) == '\n')                                             {  NL(); ADV(1);                                                          goto S_BCOMMENT;    }
                                                                     {  ADV(1);                                                                goto S_BCOMMENT;    }
S_VAR:
    if (is_idcont(PEEK(0)))                                          {  ADV(1);                                                                goto S_VAR;         }
    if (PEEK(0) >= 0x80 && prolog_u_letter(p, &n))                   {  ADV(n);                                                                goto S_VAR;         }
                                                                     {  SYNC(); return tok(TK_VAR, ct_strndup(s, (size_t)(p - s)), line);                          }
S_WORD:
    if (is_idcont(PEEK(0)))                                          {  ADV(1);                                                                goto S_WORD;        }
    if (PEEK(0) >= 0x80 && prolog_u_letter(p, &n))                   {  ADV(n);                                                                goto S_WORD;        }
                                                                     {  SYNC(); return tok(TK_ATOM, ct_strndup(s, (size_t)(p - s)), line);                         }
S_UNDERSCORE:
    if (is_idcont(PEEK(0)))                                          {  ADV(1);                                                                goto S_VAR;         }
    if (PEEK(0) >= 0x80 && prolog_u_letter(p, &n))                   {  ADV(n);                                                                goto S_VAR;         }
                                                                     {  SYNC(); return tok(TK_ANON, (char *)"_", line);                                            }
S_INT:
    if (is_digit(PEEK(0)))                                           {  ADV(1);                                                                goto S_INT;         }
    if (PEEK(0) == '_')                                              {  seps = 1; ADV(1);                                                      goto S_INT;         }
    if (PEEK(0) == ' ' && is_digit(PEEK(1)))                         {  seps = 1; ADV(1);                                                      goto S_INT;         }
    if (PEEK(0) == '\'')                                             {  radix = 0; for (const char *r = s; r < p && radix <= 36; r++) if (is_digit(*r)) radix = radix * 10 + (*r - '0');
                                                                        c = digval(PEEK(1)); if (radix >= 2 && radix <= 36 && c >= 0 && c < radix) { ADV(1); s = p; goto S_RDIGITS; } goto LX_INT; }
    if ((PEEK(0) == 'e' || PEEK(0) == 'E') && (is_digit(PEEK(1)) || ((PEEK(1) == '+' || PEEK(1) == '-') && is_digit(PEEK(2))))) {  ADV(1);    goto S_EXP_SIGN;    }
    if (PEEK(0) == '.' && is_digit(PEEK(1)))                         {  ADV(1);                                                                goto S_FRAC;        }
                                                                                                                                               goto LX_INT;
S_RDIGITS:
    c = digval(PEEK(0)); if (c >= 0 && c < radix)                    {  ADV(1);                                                                goto S_RDIGITS;     }
                                                                     {  t = tok(TK_INT, ct_strndup(s, (size_t)(p - s)), line); t.ival = (long)(unsigned long long)strtoull(t.text, NULL, radix);
                                                                        SYNC(); return t;                                                                          }
S_EXP_SIGN:
    if (PEEK(0) == '+' || PEEK(0) == '-')                            {  ADV(1);                                                                goto S_EXP_DIG;     }
                                                                                                                                               goto S_EXP_DIG;
S_EXP_DIG:
    if (is_digit(PEEK(0)))                                           {  ADV(1);                                                                goto S_EXP_DIG;     }
                                                                                                                                               goto LX_FLOAT;
S_FRAC:
    if (is_digit(PEEK(0)))                                           {  ADV(1);                                                                goto S_FRAC;        }
    if (PEEK(0) == 'e' || PEEK(0) == 'E')                            {  ADV(1);                                                                goto S_FRAC_EXP_SIGN; }
                                                                                                                                               goto S_FRAC_END;
S_FRAC_EXP_SIGN:
    if (PEEK(0) == '+' || PEEK(0) == '-')                            {  ADV(1);                                                                goto S_FRAC_EXP_DIG; }
                                                                                                                                               goto S_FRAC_EXP_DIG;
S_FRAC_EXP_DIG:
    if (is_digit(PEEK(0)))                                           {  ADV(1);                                                                goto S_FRAC_EXP_DIG; }
                                                                                                                                               goto S_FRAC_END;
S_FRAC_END:
    if ((PEEK(0) == 'N' && PEEK(1) == 'a' && PEEK(2) == 'N') || (PEEK(0) == 'I' && PEEK(1) == 'n' && PEEK(2) == 'f')) {
                                                                        { double fv = PEEK(0) == 'N' ? NAN : INFINITY; ADV(3); t = tok(TK_FLOAT, span_text(s, p, seps), line); t.fval = fv; } SYNC(); return t; }
                                                                                                                                               goto LX_FLOAT;
S_RADIX:
    c = hexval(PEEK(0)); if (c >= 0 && c < radix)                    {  ndig++; ADV(1);                                                        goto S_RADIX;       }
    if (PEEK(0) == '_' && ndig)                                      {  seps = 1; ADV(1);                                                      goto S_RADIX;       }
    if (PEEK(0) == ' ' && ndig)                                      {  c = hexval(PEEK(1)); if (c >= 0 && c < radix) { seps = 1; ADV(1); goto S_RADIX; }         goto S_RADIX_END;   }
                                                                                                                                               goto S_RADIX_END;
S_RADIX_END:
    if (!ndig || hexval(PEEK(0)) >= 0 || is_alnum(PEEK(0)))          {  SYNC(); return err_tok(line, "malformed radix integer");                                   }
    t = tok(TK_INT, span_text(s, p, seps), line); errno = 0;
    { unsigned long long uv = strtoull(t.text + 2, NULL, radix);
      if (errno == ERANGE || uv > (unsigned long long)LLONG_MAX) { t.big = 1; SYNC(); return t; }
      t.ival = (long)uv; }
    SYNC(); return t;
S_CHARCODE:
    if (PEEK(0) == '\\')                                             {  ADV(1); st = decode_escape(lx, &p, &code);
                                                                        if (st != 1) { SYNC(); return err_tok(line, "invalid escape sequence in character code constant"); }
                                                                        t = tok(TK_INT, (char *)"0'c", line); t.ival = (long)code; SYNC(); return t;                }
    if (PEEK(0) == '\'' && PEEK(1) == '\'')                          {  ADV(2); t = tok(TK_INT, (char *)"0'c", line); t.ival = (long)'\''; SYNC(); return t;        }
                                                                     {  n = u8_decode((const unsigned char *)p, &code); if (PEEK(0) == '\n') NL(); if (PEEK(0)) ADV(n);
                                                                        t = tok(TK_INT, (char *)"0'c", line); t.ival = (long)code; SYNC(); return t;                }
S_QUOTED:
    if (PEEK(0) == '\0')                                             {  SYNC(); return err_tok(line, kind == TK_ATOM ? "unterminated quoted atom" : "unterminated string"); }
    if (PEEK(0) == q && PEEK(1) == q)                                {  sb_put(lx, q); ADV(2);                                                 goto S_QUOTED;      }
    if (PEEK(0) == q)                                                {  ADV(1); SYNC(); t = tok(kind, sb_text(lx), line); t.len = lx->slen; return t;                                       }
    if (PEEK(0) == '\\')                                             {  ADV(1); st = decode_escape(lx, &p, &code); if (st == 1) sb_utf8(lx, code);
                                                                        else if (st < 0) { SYNC(); return err_tok(line, kind == TK_ATOM ? "invalid escape sequence in quoted atom"
                                                                                                                                         : "invalid escape sequence in double-quoted token"); }
                                                                                                                                               goto S_QUOTED;      }
    if (PEEK(0) == '\n' || PEEK(0) == '\t')                          {  SYNC(); return err_tok(line, kind == TK_ATOM ? "unescaped layout character in quoted atom"
                                                                                                                    : "unescaped layout character in double-quoted token"); }
                                                                     {  sb_put(lx, PEEK(0)); ADV(1);                                           goto S_QUOTED;      }
S_GRAPHIC:
    if (is_graphic(PEEK(0)))                                         {  ADV(1);                                                                goto S_GRAPHIC;     }
    if (p - s == 2 && s[0] == ':' && s[1] == '-')                    {  SYNC(); return tok(TK_NECK,  (char *)":-", line);                                          }
    if (p - s == 2 && s[0] == '?' && s[1] == '-')                    {  SYNC(); return tok(TK_QUERY, (char *)"?-", line);                                          }
                                                                     {  SYNC(); return tok(TK_OP, ct_strndup(s, (size_t)(p - s)), line);                           }
LX_INT:
    t = tok(TK_INT, span_text(s, p, seps), line); errno = 0; t.ival = (long)strtoll(t.text, NULL, 10); t.big = (errno == ERANGE); SYNC(); return t;
LX_FLOAT:
    t = tok(TK_FLOAT, span_text(s, p, seps), line); t.fval = atof(t.text); SYNC(); return t;
LX_UNEXPECTED:
    snprintf(msg, sizeof msg, "unexpected '%c'", (char)c); SYNC(); return err_tok(line, msg);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
Token lexer_next(Lexer *lx) {
    Token t = lexer_next_raw(lx);
    lx->last_kind = t.kind;
    return t;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
Token lexer_peek(Lexer *lx) {
    if (!lx->has_peek) {
        lx->peek     = lexer_next_raw(lx);
        lx->has_peek = 1;
    }
    return lx->peek;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void lexer_init(Lexer *lx, const char *src) {
    lx->src      = src ? src : "";
    lx->pos      = 0;
    lx->line     = 1;
    lx->has_peek = 0;
    lx->last_kind = TK_EOF;
    lx->fenced = 0;
    lx->sbuf = NULL; lx->slen = 0; lx->scap = 0;
    memset(&lx->peek, 0, sizeof lx->peek);
}
