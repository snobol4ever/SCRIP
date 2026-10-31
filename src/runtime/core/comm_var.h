#ifndef COMM_VAR_H
#define COMM_VAR_H
#include <stdint.h>
#include "ir/descr.h"
#include "runtime/core/trace.h"
#ifdef __cplusplus
extern "C" {
#endif
void comm_stno(int n);
void comm_var(const char *name, DESCR_t val, const char *file, long line, long long stno);
void comm_var_hook(const char *name, DESCR_t val, const char *file, long line, long long stno, int hook);
int comm_var_active(void);
const char * stmt_src_get_file(void);
void mon_emit_label_bin(int64_t stno);
void comm_call(const char *fname);
void comm_return(const char *fname, DESCR_t retval);
void comm_var_hook_p(const char *name, DESCR_t val, const char *file, long line, long long stno, int hook, trace_pend_t *pend);
void comm_var_open(const char *name, DESCR_t val, long long stno, trace_pend_t *pend);
#ifdef __cplusplus
}
#endif
#endif
