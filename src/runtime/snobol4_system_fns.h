#ifndef SNOBOL4_SYSTEM_FNS_H
#define SNOBOL4_SYSTEM_FNS_H
#include <string.h>
static const char * const g_sn4_system_fns[] = {
"ABORT","ALT","ANY","APPLY","ARB","ARBNO","ARG","ARRAY","ATAN","BAL","BREAK","BREAKX","CHAR","CHOP","CLEAR","CODE","COLLECT","CONCAT","CONVERT","COPY","COS","DATA","DATATYPE","DATE","DEFINE","DIFFER","DUMP","DUPL","ENDFILE","EQ","EVAL","EXP","FAIL","FENCE","FIELD","FUNCTION","GE","GT","HOST","IDENT","INPUT","INTEGER","ITEM","LABEL","LCASE","LE","LEN","LEQ","LGE","LGT","LLE","LLT","LN","LNE","LOAD","LOCAL","LPAD","LT","NAME","NE","NOTANY","NUMERIC","OPSYN","OUTPUT","PLS","POS","PROTOTYPE","REAL","REM","REMDR","REPLACE","REVERSE","RPAD","RPOS","RSORT","RTAB","SETEXIT","SIN","SIZE","SORT","SPAN","SQRT","STOPTR","SUBSTR","SUCCEED","TAB","TABLE","TAN","TIME","TRACE","TRIM","UCASE","UNLOAD","VALUE","VDIFFER"};
#define SN4_SYSTEM_FN_COUNT (sizeof(g_sn4_system_fns) / sizeof(g_sn4_system_fns[0]))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sn4_is_system_fn(const char *name) {
    if (!name || !*name) return 0;
    for (unsigned i = 0; i < SN4_SYSTEM_FN_COUNT; i++) if (!strcmp(g_sn4_system_fns[i], name)) return 1;
    return 0;
}
#endif
