/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef ICON_PARSE_H
#define ICON_PARSE_H
#include "icon_lex.h"
#include "../snobol4/scrip_cc.h"
typedef struct {
    IcnLexer   *lex;
    IcnToken    cur;
    IcnToken    peek;
    IcnTkKind   prev_kind;
    int         had_error;
    char        errmsg[512];
} IcnParser;
void     icn_parse_init(IcnParser *p, IcnLexer *lex);
CODE_t *icn_parse_file(IcnParser *p, tree_t **out_ast);
tree_t  *icn_parse_expr(IcnParser *p);
#endif
