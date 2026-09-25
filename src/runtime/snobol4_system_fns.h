#ifndef SNOBOL4_SYSTEM_FNS_H
#define SNOBOL4_SYSTEM_FNS_H
#include <string.h>
#include <stdlib.h>
static const char * const g_sn4_system_fns[] = {
"ANY","APPLY","ARBNO","ARG","ARRAY","ATAN","BREAK","BREAKX","CHAR","CHOP","CLEAR","CODE","COLLECT","CONVERT","COPY","COS","DATA","DATATYPE","DATE","DEFINE","DIFFER","DUMP","DUPL","ENDFILE","EQ","EVAL","EXP","FENCE","FIELD","GE","GT","HOST","IDENT","INPUT","INTEGER","ITEM","LE","LEN","LEQ","LGE","LGT","LLE","LLT","LN","LNE","LOAD","LOCAL","LPAD","LT","NE","NOTANY","OPSYN","OUTPUT","POS","PROTOTYPE","REMDR","REPLACE","REVERSE","RPAD","RPOS","RSORT","RTAB","SETEXIT","SIN","SIZE","SORT","SPAN","SQRT","STOPTR","SUBSTR","TAB","TABLE","TAN","TIME","TRACE","TRIM","UNLOAD"};
#define SN4_SYSTEM_FN_COUNT (sizeof(g_sn4_system_fns) / sizeof(g_sn4_system_fns[0]))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static __attribute__((unused)) int sn4_is_system_fn(const char *name) {
    if (!name || !*name) return 0;
    { int lo = 0, hi = (int)SN4_SYSTEM_FN_COUNT - 1;
      while (lo <= hi) { int mid = (lo + hi) >> 1; const char *e = g_sn4_system_fns[mid];
        int c = (e[0] != name[0]) ? ((int)(unsigned char)e[0] - (int)(unsigned char)name[0]) : strcmp(e, name);
        if (c == 0) return 1; if (c < 0) lo = mid + 1; else hi = mid - 1; } }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static __attribute__((unused)) int sn4_sysfn_protected(const char *name) {
    return sn4_is_system_fn(name);
}
#endif
