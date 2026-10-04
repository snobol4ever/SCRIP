#include <stdio.h>
#include "ct_arena.h"
#include <stdlib.h>
#include <string.h>
#include "snocone_lex.h"
#include "snocone_parse.tab.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int is_alpha(int c)        { return ((c | 32) >= 'a' && (c | 32) <= 'z') || c == '_'; }
static inline int is_digit(int c)        { return c >= '0' && c <= '9'; }
static inline int is_idstart(int c)      { return ((c | 32) >= 'a' && (c | 32) <= 'z') || c >= 0x80; }
static inline int is_idcont(int c)       { return is_idstart(c) || is_digit(c) || c == '_' || c == '.'; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int is_value_starter(int c) {
    return is_alpha(c) || c >= 0x80 || is_digit(c) || c == '\'' || c == '"' ||
           c == '(';
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int is_rws_char(int c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\f' ||
           c == '\n' || c == '\0';
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int is_rws_at(const char *p, int n) {
    int c0 = (unsigned char)p[n];
    if (is_rws_char(c0)) return 1;
    if (c0 == '/') {
        int c1 = (unsigned char)p[n + 1];
        if (c1 == '/' || c1 == '*') return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sc_kind_is_value(int kind) {
    switch (kind) {
    case T_IDENT: case T_INT: case T_REAL: case T_STR: case T_KEYWORD: case T_RPAREN: case T_RBRACK: return 1;
    default: return 0;
    }
}
typedef struct { const char *word; int kind; } KwEntry;
static const KwEntry KW_TABLE[] = {
    { "if",       T_IF       },
    { "else",     T_ELSE     },
    { "while",    T_WHILE    },
    { "do",       T_DO       },
    { "for",      T_FOR      },
    { "switch",   T_SWITCH   },
    { "case",     T_CASE     },
    { "default",  T_DEFAULT  },
    { "break",    T_BREAK    },
    { "continue", T_CONTINUE },
    { "goto",     T_GOTO     },
    { "procedure", T_DEFINE },
    { "function",   T_DEFINE },
    { "procedure",  T_DEFINE },
    { "return",   T_RETURN   },
    { "freturn",  T_FRETURN  },
    { "nreturn",  T_NRETURN  },
    { "struct",   T_STRUCT   },
    { NULL,       T_IDENT       }
};
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int kw_lookup_at(const char *s, int idx) {
    if (KW_TABLE[idx].word == NULL)              return T_IDENT;
    if (strcmp(s, KW_TABLE[idx].word) == 0)      return KW_TABLE[idx].kind;
    return kw_lookup_at(s, idx + 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int classify_keyword_range(const char *start, const char *end) {
    int n = (int)(end - start);
    if (n <= 0 || n > 32) return T_IDENT;
    char buf[n + 1];
    memcpy(buf, start, n);
    buf[n] = '\0';
    return kw_lookup_at(buf, 0);
}
#define ADV(n)    (p += (n))
#define PEEK(n)   ((unsigned char)p[n])
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int emit_kind(LexCtx *ctx, SC_STYPE *yylval, const char *p, int kind) {
    yylval->str = NULL;
    ctx->p = p;
    ctx->last_kind = kind;
    return kind;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *sc_tok_dup(const char *s, int n) { char *d = (char *)ct_alloc((size_t)n + 1); memcpy(d, s, (size_t)n); d[n] = '\0'; return d; }
static inline void sc_str_put(LexCtx *ctx, char c) {
    if (ctx->strpos + 1 >= ctx->strcap) { int nc = ctx->strcap ? ctx->strcap * 2 : 8; ctx->strbuf = (char *)ct_grow(ctx->strbuf, (size_t)nc); ctx->strcap = nc; }
    ctx->strbuf[ctx->strpos++] = c;
}
static inline int emit_value(LexCtx *ctx, SC_STYPE *yylval, const char *p, const char *tok_start, int kind) {
    yylval->str = sc_tok_dup(tok_start, (int)(p - tok_start));
    ctx->p = p;
    ctx->last_kind = kind;
    return kind;
}
#define EMIT(k)    return emit_kind(ctx, yylval, p, (k))
#define EMIT_V(k)  return emit_value(ctx, yylval, p, tok_start, (k))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sc_lex(SC_STYPE *yylval, ScParseState *st) {
    LexCtx     *ctx        = st->ctx;
    const char *p          = ctx->p;
    const char *tok_start  = NULL;
    int         had_ws     = 0;
    int         last_value = sc_kind_is_value(ctx->last_kind);
S_WS:
    if (PEEK(0) == ' '  )                                          {  had_ws = 1; ADV(1);                                  goto S_WS;        }
    if (PEEK(0) == '\t' )                                          {  had_ws = 1; ADV(1);                                  goto S_WS;        }
    if (PEEK(0) == '\r' )                                          {  had_ws = 1; ADV(1);                                  goto S_WS;        }
    if (PEEK(0) == '\f' )                                          {  had_ws = 1; ADV(1);                                  goto S_WS;        }
    if (PEEK(0) == '\n' )                                          {  ctx->line++; had_ws = 1; ADV(1);                     goto S_CONT;      }
    if (PEEK(0) == '/'  && PEEK(1) == '/'  )                       {  had_ws = 1; ADV(2);                                  goto S_LCOMMENT;  }
    if (PEEK(0) == '/'  && PEEK(1) == '*'  )                       {  had_ws = 1; ADV(2);                                  goto S_BCOMMENT;  }
                                                                                                                           goto S_DISPATCH;
S_CONT:
    if (PEEK(0) == '+'  )                                          {  ADV(1);                                              goto S_WS;        }
    if (PEEK(0) == '.'  )                                          {  ADV(1);                                              goto S_WS;        }
                                                                                                                           goto S_WS;
S_LCOMMENT:
    if (PEEK(0) == '\0' )                                                                                                  goto S_DISPATCH;
    if (PEEK(0) == '\n' )                                                                                                  goto S_WS;
                                                                   {  ADV(1);                                              goto S_LCOMMENT;  }
S_BCOMMENT:
    if (PEEK(0) == '\0' )                                                                                                  goto S_DISPATCH;
    if (PEEK(0) == '*'  )                                          {  ADV(1);                                              goto S_BC_STAR;   }
    if (PEEK(0) == '\n' )                                          {  ctx->line++; ADV(1);                                 goto S_BCOMMENT;  }
                                                                   {  ADV(1);                                              goto S_BCOMMENT;  }
S_BC_STAR:
    if (PEEK(0) == '/'  )                                          {  ADV(1);                                              goto S_WS;        }
    if (PEEK(0) == '*'  )                                          {  ADV(1);                                              goto S_BC_STAR;   }
    if (PEEK(0) == '\0' )                                                                                                  goto S_DISPATCH;
                                                                   {  ADV(1);                                              goto S_BCOMMENT;  }
S_DISPATCH:
    if (had_ws && last_value && is_value_starter(PEEK(0)))         {  EMIT(T_CONCAT);                 }
    if (had_ws && last_value && PEEK(0) == '&' && is_alpha(PEEK(1))) { EMIT(T_CONCAT);               }
    if (PEEK(0) == '\0' )                                                                                                  goto LX_EOF;
    if (PEEK(0) == '\'' )                                          {  ctx->strpos = 0; ADV(1);                             goto S_STR1;      }
    if (PEEK(0) == '"'  )                                          {  ctx->strpos = 0; ADV(1);                             goto S_STR2;      }
    if (is_idstart(PEEK(0)))                                       {  tok_start = p; ADV(1);                               goto S_IDENT;     }
    if (is_digit(PEEK(0)))                                         {  tok_start = p; ADV(1);                               goto S_INT;       }
    if (PEEK(0) == '.'  )                                                                                                  goto S_OP_DOT;
    if (PEEK(0) == '&'  && is_alpha(PEEK(1)))                      {  ADV(1); tok_start = p;                               goto S_KEYWORD;   }
    if (PEEK(0) == '('  )                                          {  ADV(1);                                              goto LX_LPAREN;    }
    if (PEEK(0) == ')'  )                                          {  ADV(1);                                              goto LX_RPAREN;    }
    if (PEEK(0) == '['  )                                          {  ADV(1);                                              goto LX_LBRACK;    }
    if (PEEK(0) == ']'  )                                          {  ADV(1);                                              goto LX_RBRACK;    }
    if (PEEK(0) == '{'  )                                          {  ADV(1);                                              goto LX_LBRACE;    }
    if (PEEK(0) == '}'  )                                          {  ADV(1);                                              goto LX_RBRACE;    }
    if (PEEK(0) == ','  )                                          {  ADV(1);                                              goto LX_COMMA;     }
    if (PEEK(0) == ';'  )                                          {  ADV(1);                                              goto LX_SEMICOLON; }
    if (PEEK(0) == ':'  )                                                                                                  goto S_OP_COLON;
    if (PEEK(0) == '='  )                                                                                                  goto S_OP_EQ;
    if (PEEK(0) == '!'  )                                                                                                  goto S_OP_BANG;
    if (PEEK(0) == '<'  )                                                                                                  goto S_OP_LT;
    if (PEEK(0) == '>'  )                                                                                                  goto S_OP_GT;
    if (PEEK(0) == '+'  )                                                                                                  goto S_OP_PLUS;
    if (PEEK(0) == '-'  )                                                                                                  goto S_OP_MINUS;
    if (PEEK(0) == '*'  )                                                                                                  goto S_OP_STAR;
    if (PEEK(0) == '/'  )                                                                                                  goto S_OP_SLASH;
    if (PEEK(0) == '^'  )                                                                                                  goto S_OP_CARET;
    if (PEEK(0) == '|'  )                                                                                                  goto S_OP_PIPE;
    if (PEEK(0) == '?'  )                                                                                                  goto S_OP_QUEST;
    if (PEEK(0) == '$'  )                                                                                                  goto S_OP_DOLLAR;
    if (PEEK(0) == '&'  )                                                                                                  goto S_OP_AMP;
    if (PEEK(0) == '@'  )                                                                                                  goto S_OP_AT;
    if (PEEK(0) == '#'  )                                                                                                  goto S_OP_POUND;
    if (PEEK(0) == '%'  )                                                                                                  goto S_OP_PERCENT;
    if (PEEK(0) == '~'  )                                                                                                  goto S_OP_TILDE;
                                                                   {  tok_start = p; ADV(1);                               goto TT_UNKNOWN;   }
S_IDENT:
    if (is_idcont(PEEK(0)))                                        {  ADV(1);                                              goto S_IDENT;     }
    if (PEEK(0) == '('  )                                                                                                  goto LX_CALL;
                                                                   {                                                       goto LX_IDENT;     }
S_KEYWORD:
    if (is_idcont(PEEK(0)))                                        {  ADV(1);                                              goto S_KEYWORD;   }
                                                                                                                           goto TT_KEYWORD;
S_INT:
    if (is_digit(PEEK(0)))                                         {  ADV(1);                                              goto S_INT;       }
    if (PEEK(0) == '.' && is_digit(PEEK(1)))                       {  ADV(1);                                              goto S_FRAC;      }
    if (PEEK(0) == '.' )                                           {  ADV(1);                                              goto LX_REAL;      }
    if (PEEK(0) == 'e' || PEEK(0) == 'E')                          {  ADV(1);                                              goto S_EXP_SIGN;  }
    if (PEEK(0) == 'd' || PEEK(0) == 'D')                          {  ADV(1);                                              goto S_EXP_SIGN;  }
                                                                                                                           goto LX_INT;
S_FRAC:
    if (is_digit(PEEK(0)))                                         {  ADV(1);                                              goto S_FRAC;      }
    if (PEEK(0) == 'e' || PEEK(0) == 'E')                          {  ADV(1);                                              goto S_EXP_SIGN;  }
    if (PEEK(0) == 'd' || PEEK(0) == 'D')                          {  ADV(1);                                              goto S_EXP_SIGN;  }
                                                                                                                           goto LX_REAL;
S_EXP_SIGN:
    if (PEEK(0) == '+' || PEEK(0) == '-')                          {  ADV(1);                                              goto S_EXP_DIG;   }
                                                                                                                           goto S_EXP_DIG;
S_EXP_DIG:
    if (is_digit(PEEK(0)))                                         {  ADV(1);                                              goto S_EXP_DIG;   }
                                                                                                                           goto LX_REAL;
S_STR1:
    if (PEEK(0) == '\0' )                                                                                                  goto LX_STR;
    if (PEEK(0) == '\'' && PEEK(1) == '\'' )                       {  sc_str_put(ctx, '\''); ADV(2);           goto S_STR1;      }
    if (PEEK(0) == '\'' )                                          {  ADV(1);                                              goto LX_STR;       }
    if (PEEK(0) == '\n' )                                          {  ctx->line++; sc_str_put(ctx, '\n'); ADV(1); goto S_STR1;   }
                                                                   {  sc_str_put(ctx, *p); ADV(1);              goto S_STR1;     }
S_STR2:
    if (PEEK(0) == '\0' )                                                                                                  goto LX_STR;
    if (PEEK(0) == '"'  && PEEK(1) == '"'  )                       {   sc_str_put(ctx, '"'); ADV(2);           goto S_STR2;      }
    if (PEEK(0) == '"'  )                                          {  ADV(1);                                              goto LX_STR;       }
    if (PEEK(0) == '\n' )                                          {  ctx->line++; sc_str_put(ctx, '\n'); ADV(1); goto S_STR2;   }
                                                                   {  sc_str_put(ctx, *p); ADV(1);              goto S_STR2;     }
S_OP_COLON:
                                                                   {  ADV(1);                                              goto LX_COLON;     }
S_OP_EQ:
    if (had_ws && last_value && (is_rws_at(p, 1) || PEEK(1) == ';' || PEEK(1) == ')' || PEEK(1) == ']')) {  ADV(1);             goto TT_ASSIGN;    }
    if (had_ws && last_value)                                      {  EMIT(T_CONCAT);                 }
                                                                   {  ADV(1);                                              goto LX_UN_EQUAL;  }
S_OP_BANG:
    if (had_ws && last_value && is_rws_at(p, 1))                   {  ADV(1);                                              goto LX_EXP;       }
    if (had_ws && last_value)                                      {  EMIT(T_CONCAT);                 }
                                                                   {  ADV(1);                                              goto LX_UN_BANG;   }
S_OP_LT:
                                                                   {  tok_start = p; ADV(1);                               goto TT_UNKNOWN;   }
S_OP_GT:
                                                                   {  tok_start = p; ADV(1);                               goto TT_UNKNOWN;   }
S_OP_PLUS:
    if (PEEK(1) == '=' )                                           {  ADV(2);                                              goto LX_PLUS_ASSIGN;  }
    if (had_ws && last_value && is_rws_at(p, 1))                   {  ADV(1);                                              goto TT_ADD;       }
    if (had_ws && last_value)                                      {  EMIT(T_CONCAT);                 }
                                                                   {  ADV(1);                                              goto LX_UN_PLUS;   }
S_OP_MINUS:
    if (PEEK(1) == '=' )                                           {  ADV(2);                                              goto LX_MINUS_ASSIGN; }
    if (had_ws && last_value && is_rws_at(p, 1))                   {  ADV(1);                                              goto TT_SUB;       }
    if (had_ws && last_value)                                      {  EMIT(T_CONCAT);                 }
                                                                   {  ADV(1);                                              goto LX_UN_MINUS;  }
S_OP_STAR:
    if (had_ws && last_value && PEEK(1) == '*' && is_rws_at(p, 2)) {  ADV(2);                                              goto LX_EXP;       }
    if (PEEK(1) == '=' )                                           {  ADV(2);                                              goto LX_STAR_ASSIGN;  }
    if (had_ws && last_value && is_rws_at(p, 1))                   {  ADV(1);                                              goto TT_MUL;       }
    if (had_ws && last_value)                                      {  EMIT(T_CONCAT);                 }
                                                                   {  ADV(1);                                              goto LX_UN_STAR;   }
S_OP_SLASH:
    if (PEEK(1) == '=' )                                           {  ADV(2);                                              goto LX_SLASH_ASSIGN; }
    if (had_ws && last_value && is_rws_at(p, 1))                   {  ADV(1);                                              goto TT_DIV;       }
    if (had_ws && last_value)                                      {  EMIT(T_CONCAT);                 }
                                                                   {  ADV(1);                                              goto LX_UN_SLASH;  }
S_OP_CARET:
    if (PEEK(1) == '=' )                                           {  ADV(2);                                              goto LX_CARET_ASSIGN; }
    if (had_ws && last_value && is_rws_at(p, 1))                   {  ADV(1);                                              goto LX_EXP;       }
    if (had_ws && last_value)                                      {  EMIT(T_CONCAT);                 }
                                                                   {  tok_start = p; ADV(1);                               goto TT_UNKNOWN;   }
S_OP_PIPE:
    if (had_ws && last_value && is_rws_at(p, 1))                   {  ADV(1);                                              goto TT_ALT;       }
    if (had_ws && last_value)                                      {  EMIT(T_CONCAT);                 }
                                                                   {  ADV(1);                                              goto LX_UN_PIPE;   }
S_OP_QUEST:
    if (had_ws && last_value && is_rws_at(p, 1))                   {  ADV(1);                                              goto LX_MATCH;     }
    if (had_ws && last_value)                                      {  EMIT(T_CONCAT);                 }
                                                                   {  ADV(1);                                              goto LX_UN_QUEST;  }
S_OP_DOLLAR:
    if (had_ws && last_value && is_rws_at(p, 1))                   {  ADV(1);                                              goto LX_IMM_ASSIGN;}
    if (had_ws && last_value)                                      {  EMIT(T_CONCAT);                 }
                                                                   {  ADV(1);                                              goto LX_UN_DOLLAR; }
S_OP_DOT:
    if (had_ws && last_value && is_rws_at(p, 1))                   {  ADV(1);                                              goto LX_COND_ASSIGN;  }
    if (had_ws && last_value)                                      {  EMIT(T_CONCAT);                 }
                                                                   {  ADV(1);                                              goto LX_UN_DOT;    }
S_OP_AMP:
    if (had_ws && last_value && is_rws_at(p, 1))                   {  ADV(1);                                              goto LX_AMP;       }
    if (had_ws && last_value)                                      {  EMIT(T_CONCAT);                 }
                                                                   {  ADV(1);                                              goto LX_UN_AMP;    }
S_OP_AT:
    if (had_ws && last_value && is_rws_at(p, 1))                   {  ADV(1);                                              goto LX_AT;        }
    if (had_ws && last_value)                                      {  EMIT(T_CONCAT);                 }
                                                                   {  ADV(1);                                              goto LX_UN_AT;     }
S_OP_POUND:
    if (had_ws && last_value && is_rws_at(p, 1))                   {  ADV(1);                                              goto LX_POUND;     }
    if (had_ws && last_value)                                      {  EMIT(T_CONCAT);                 }
                                                                   {  ADV(1);                                              goto LX_UN_POUND;  }
S_OP_PERCENT:
    if (had_ws && last_value && is_rws_at(p, 1))                   {  ADV(1);                                              goto LX_PERCENT;   }
    if (had_ws && last_value)                                      {  EMIT(T_CONCAT);                 }
                                                                   {  ADV(1);                                              goto LX_UN_PERCENT;}
S_OP_TILDE:
    if (had_ws && last_value && is_rws_at(p, 1))                   {  ADV(1);                                              goto LX_TILDE;     }
    if (had_ws && last_value)                                      {  EMIT(T_CONCAT);                 }
                                                                   {  ADV(1);                                              goto LX_UN_TILDE;  }
LX_EOF:           return 0;
LX_LPAREN:        EMIT(T_LPAREN);
LX_RPAREN:        EMIT(T_RPAREN);
LX_LBRACK:        EMIT(T_LBRACK);
LX_RBRACK:        EMIT(T_RBRACK);
LX_LBRACE:        EMIT(T_LBRACE);
LX_RBRACE:        EMIT(T_RBRACE);
LX_COMMA:         EMIT(T_COMMA);
LX_SEMICOLON:     EMIT(T_SEMICOLON);
LX_COLON:         EMIT(T_COLON);
TT_ASSIGN:        EMIT(T_2EQUAL);
LX_MATCH:         EMIT(T_2QUEST);
TT_ALT:           EMIT(T_2PIPE);
TT_ADD:           EMIT(T_2PLUS);
TT_SUB:           EMIT(T_2MINUS);
TT_MUL:           EMIT(T_2STAR);
TT_DIV:           EMIT(T_2SLASH);
LX_EXP:           EMIT(T_2CARET);
LX_IMM_ASSIGN:    EMIT(T_2DOLLAR);
LX_COND_ASSIGN:   EMIT(T_2DOT);
LX_AMP:           EMIT(T_2AMP);
LX_AT:            EMIT(T_2AT);
LX_POUND:         EMIT(T_2POUND);
LX_PERCENT:       EMIT(T_2PERCENT);
LX_TILDE:         EMIT(T_2TILDE);
LX_PLUS_ASSIGN:   EMIT(T_PLUS_ASSIGN);
LX_MINUS_ASSIGN:  EMIT(T_MINUS_ASSIGN);
LX_STAR_ASSIGN:   EMIT(T_STAR_ASSIGN);
LX_SLASH_ASSIGN:  EMIT(T_SLASH_ASSIGN);
LX_CARET_ASSIGN:  EMIT(T_CARET_ASSIGN);
LX_UN_PLUS: if (is_rws_at(p, 0)) { tok_start = p - 1; goto TT_UNKNOWN; } EMIT(T_1PLUS);
LX_UN_MINUS: if (is_rws_at(p, 0)) { tok_start = p - 1; goto TT_UNKNOWN; } EMIT(T_1MINUS);
LX_UN_STAR: if (is_rws_at(p, 0)) { tok_start = p - 1; goto TT_UNKNOWN; } EMIT(T_1STAR);
LX_UN_SLASH: if (is_rws_at(p, 0)) { tok_start = p - 1; goto TT_UNKNOWN; } EMIT(T_1SLASH);
LX_UN_PERCENT: if (is_rws_at(p, 0)) { tok_start = p - 1; goto TT_UNKNOWN; } EMIT(T_1PERCENT);
LX_UN_AT: if (is_rws_at(p, 0)) { tok_start = p - 1; goto TT_UNKNOWN; } EMIT(T_1AT);
LX_UN_TILDE: if (is_rws_at(p, 0)) { tok_start = p - 1; goto TT_UNKNOWN; } EMIT(T_1TILDE);
LX_UN_DOLLAR: if (is_rws_at(p, 0)) { tok_start = p - 1; goto TT_UNKNOWN; } EMIT(T_1DOLLAR);
LX_UN_DOT: if (is_rws_at(p, 0)) { tok_start = p - 1; goto TT_UNKNOWN; } EMIT(T_1DOT);
LX_UN_POUND: if (is_rws_at(p, 0)) { tok_start = p - 1; goto TT_UNKNOWN; } EMIT(T_1POUND);
LX_UN_PIPE: if (is_rws_at(p, 0)) { tok_start = p - 1; goto TT_UNKNOWN; } EMIT(T_1PIPE);
LX_UN_EQUAL: if (is_rws_at(p, 0)) { tok_start = p - 1; goto TT_UNKNOWN; } EMIT(T_1EQUAL);
LX_UN_QUEST: if (is_rws_at(p, 0)) { tok_start = p - 1; goto TT_UNKNOWN; } EMIT(T_1QUEST);
LX_UN_AMP: if (is_rws_at(p, 0)) { tok_start = p - 1; goto TT_UNKNOWN; } EMIT(T_1AMP);
LX_UN_BANG: if (is_rws_at(p, 0)) { tok_start = p - 1; goto TT_UNKNOWN; } EMIT(T_1BANG);
LX_INT:           EMIT_V(T_INT);
LX_REAL:          EMIT_V(T_REAL);
LX_CALL:
    if (classify_keyword_range(tok_start, p) != T_IDENT) goto LX_IDENT;
    if (ctx->last_kind == T_DEFINE)                      goto LX_IDENT;
    {
        yylval->str = sc_tok_dup(tok_start, (int)(p - tok_start));
        ADV(1);
        ctx->p = p;
        ctx->last_kind = T_CALL;
        return T_CALL;
    }
LX_IDENT:
    {
        int kind = classify_keyword_range(tok_start, p);
        yylval->str = sc_tok_dup(tok_start, (int)(p - tok_start));
        ctx->p = p; ctx->last_kind = kind; return kind;
    }
TT_KEYWORD:
    {
        yylval->str = sc_tok_dup(tok_start, (int)(p - tok_start));
        ctx->p = p; ctx->last_kind = T_KEYWORD; return T_KEYWORD;
    }
LX_STR:
    {
        yylval->str = ctx->strpos ? sc_tok_dup(ctx->strbuf, (int)strnlen(ctx->strbuf, (size_t)ctx->strpos)) : sc_tok_dup("", 0);
        ctx->p = p; ctx->last_kind = T_STR; return T_STR;
    }
TT_UNKNOWN:       EMIT_V(T_UNKNOWN);
}
