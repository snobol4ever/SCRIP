#ifndef SNOBOL4_SYSTEM_FNS_H
#define SNOBOL4_SYSTEM_FNS_H
#include <string.h>
static const char * const g_sn4_system_fns[] = {
"ANY","APPLY","ARBNO","ARG","ARRAY","ATAN","BREAK","BREAKX","CHAR","CHOP","CLEAR","CODE","COLLECT","CONVERT","COPY","COS","DATA","DATATYPE","DATE","DEFINE","DIFFER","DUMP","DUPL","ENDFILE","EQ","EVAL","EXP","FENCE","FIELD","GE","GT","HOST","IDENT","INPUT","INTEGER","ITEM","LE","LEN","LEQ","LGE","LGT","LLE","LLT","LN","LNE","LOAD","LOCAL","LPAD","LT","NE","NOTANY","OPSYN","OUTPUT","POS","PROTOTYPE","REMDR","REPLACE","REVERSE","RPAD","RPOS","RSORT","RTAB","SETEXIT","SIN","SIZE","SORT","SPAN","SQRT","STOPTR","SUBSTR","TAB","TABLE","TAN","TIME","TRACE","TRIM","UNLOAD"};
#define SN4_SYSTEM_FN_COUNT (sizeof(g_sn4_system_fns) / sizeof(g_sn4_system_fns[0]))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sn4_is_system_fn(const char *name) {
    if (!name || !*name) return 0;
    for (unsigned i = 0; i < SN4_SYSTEM_FN_COUNT; i++) if (!strcmp(g_sn4_system_fns[i], name)) return 1;
    return 0;
}
#endif
