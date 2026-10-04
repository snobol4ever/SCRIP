/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef SNOCONE_LEX_H
#define SNOCONE_LEX_H
#include <stddef.h>
typedef struct LexCtx {
    const char *p;
    int         line;
    int         last_kind;
    char       *strbuf;
    int         strpos;
    int         strcap;
} LexCtx;
int         sc_kind_is_value   (int kind);
int         sc_kind_has_payload(int kind);
const char *sc2_kind_name      (int kind);
#endif
