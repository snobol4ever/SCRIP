#ifndef TRACE_H
#define TRACE_H
#include "runtime/rt/rt_diag.h"
#include "runtime/core/g_runtime_misc.h"
#include <stdint.h>
#include "ir/descr.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct { char used; int kind; const char *name; const char *tag; const char *cbfn; long eid; } trace_ent_t;
void sno_trace_value(const char *name, DESCR_t val);
void sno_trace_call(const char *fname);
void sno_trace_return(const char *fname, DESCR_t retval);
void rt_trace_return_wire(const char *name, DESCR_t retval, DESCR_t wireval);
int trace_is_active(const char *name);
void rt_trace_stmt(long line);
void rt_trace_call(const char *name, DESCR_t *args, int nargs);
void rt_trace_return(const char *name, DESCR_t retval);
void rt_trace_value(const char *name, DESCR_t val);
void rt_trace_value_sigil(const char *name, DESCR_t val, int hook);
#if RT_DIAG
#define rt_trace_layer_idle() (g_runtime.abi.comm_dbg == 0 && g_runtime.abi.trace_set_n == 0 && g_runtime.abi.monitor_fd < 0 && g_rt_misc.kw_trace <= 0 && g_runtime.abi.kw_ftrace <= 0 && \
    g_runtime.abi.trace == 0)
#else
#define rt_trace_layer_idle() (1)
#endif
typedef enum { TRK_VALUE = 0, TRK_ACCESS, TRK_LABEL, TRK_KEYWORD, TRK_FUNCTION, TRK_CALL, TRK_RETURN, TRK_SUSPEND, TRK_RESUME } trace_kind_t;
void rt_trace_suspend_hook(const char *pname, uint64_t lo, uint64_t hi, long line);
void rt_trace_gen_return_hook(const char *pname, uint64_t lo, uint64_t hi, void *h);
void rt_trace_resume_hook(const char *pname, void *h);
void rt_trace_event(int kind, const char *name, DESCR_t value, long long stno);
void rt_trace_event_args(int kind, const char *name, DESCR_t *args, int nargs, DESCR_t value, long long stno);
typedef struct { DESCR_t args[3]; DESCR_t saved_trace; DESCR_t saved_ftrace; DESCR_t val; DESCR_t hook; } trace_pend_t;
void rt_trace_event_args_i(int kind, const char *name, DESCR_t *args, int nargs, DESCR_t value, long long stno, long island);
void rt_trace_event_args_ip(int kind, const char *name, DESCR_t *args, int nargs, DESCR_t value, long long stno, long island, trace_pend_t *pend, long long rstno);
DESCR_t NV_SET_p_fn(const char *name, DESCR_t val, trace_pend_t *pend);
long rt_value_trace_open(const char *name, trace_pend_t *pend);
void rt_trace_pend_run(trace_pend_t *pend);
void rt_trace_keyword_write_p(const char *kw, int64_t v, long long stno, trace_pend_t *pend);
void rt_stmt_enter_p(long stno, long line, trace_pend_t *pend);
void rt_trace_label_hook_p(const char *name, trace_pend_t *pend);
void rt_trace_pend_close(trace_pend_t *pend);
void rt_pl_port_trace_cb(const char *name, long port);
void rt_call_pend_close(trace_pend_t *pend);
long rt_tweak_open(DESCR_t *out, const char *cname, trace_pend_t *pend);
DESCR_t NV_GET_open(const char *name, trace_pend_t *pend);
void rt_trace_call_hook_p(const char *fname, trace_pend_t *pend);
void rt_trace_return_hook_p(const char *fname, DESCR_t retval, trace_pend_t *pend, long long rstno);
void rt_trace_fail_hook_p(const char *fname, trace_pend_t *pend, long long rstno);
void rt_trace_all_set(int on);
void rt_trace_call_hook(const char *fname);
void rt_trace_fail_hook(const char *fname);
void rt_trace_call_hook_f(const char *fname, int np, void *base);
void rt_trace_return_hook(const char *fname, DESCR_t retval);
void rt_trace_gen_fail_hook(const char *fname, void *h);
#ifdef __cplusplus
}
#endif
#endif
