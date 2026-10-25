/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef PL_LEX_H
#define PL_LEX_H
typedef enum {
    TK_EOF = 0,
    TK_ATOM,
    TK_VAR,
    TK_ANON,
    TK_INT,
    TK_FLOAT,
    TK_STRING,
    TK_BQSTRING,
    TK_LPAREN,
    TK_RPAREN,
    TK_LBRACKET,
    TK_RBRACKET,
    TK_PIPE,
    TK_COMMA,
    TK_DOT,
    TK_LBRACE,
    TK_RBRACE,
    TK_DOTDOT,
    TK_OP,
    TK_NECK,
    TK_QUERY,
    TK_CUT,
    TK_SEMI,
    TK_ERROR
} TkKind;
typedef struct {
    TkKind  kind;
    char   *text;
    long    ival;
    double  fval;
    int     line;
    int     big;
    int     adj;
    int     uni;
    const char *swi;
    int     len;
    int     start;
    int     end;
    struct tree_t *pc;
} Token;
typedef struct {
    const char *src;
    int         pos;
    int         line;
    Token       peek;
    int         has_peek;
    TkKind      last_kind;
    TkKind      prev_kind;
    const char *last_text;
    const char *prev_text;
    int         fenced;
    char       *sbuf;
    int         slen;
    int         scap;
    struct tree_t *pc;
    int         run0;
    int         iso;
    int         swi;
    int         tstart;
    int         last_end;
    int        *cm;
    int         ncm;
    int         ccm;
} Lexer;
void lexer_init(Lexer *lx, const char *src);
Token lexer_next(Lexer *lx);
Token lexer_peek(Lexer *lx);
Token lexer_expect(Lexer *lx, TkKind kind, const char *context);
void token_free(Token *t);
int prolog_escape_bytes(const char *raw, char *out, int cap);
int prolog_u_nd_zero(unsigned c);
const char *tk_name(TkKind kind);
static inline int u8_decode(const unsigned char *u, int *cp) {
    if ((u[0] & 0xE0) == 0xC0 && (u[1] & 0xC0) == 0x80) { *cp = ((u[0] & 0x1F) << 6) | (u[1] & 0x3F); return 2; }
    if ((u[0] & 0xF0) == 0xE0 && (u[1] & 0xC0) == 0x80 && (u[2] & 0xC0) == 0x80) { *cp = ((u[0] & 0x0F) << 12) | ((u[1] & 0x3F) << 6) | (u[2] & 0x3F); return 3; }
    if ((u[0] & 0xF8) == 0xF0 && (u[1] & 0xC0) == 0x80 && (u[2] & 0xC0) == 0x80 && (u[3] & 0xC0) == 0x80) {
        *cp = ((u[0] & 0x07) << 18) | ((u[1] & 0x3F) << 12) | ((u[2] & 0x3F) << 6) | (u[3] & 0x3F); return 4; }
    *cp = u[0]; return 1;
}
#endif
