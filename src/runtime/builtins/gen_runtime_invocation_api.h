#ifndef GEN_RUNTIME_INVOCATION_API_H
#define GEN_RUNTIME_INVOCATION_API_H
#include "ir/descr.h"
#ifdef __cplusplus
extern "C" {
#endif
DESCR_t proc_table_call(int pi, DESCR_t *args, int nargs);
#ifdef __cplusplus
}
#endif
#endif
