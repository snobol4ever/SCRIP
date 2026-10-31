#ifndef CORE_ERROR_H
#define CORE_ERROR_H
#ifdef __cplusplus
extern "C" {
#endif
#include <setjmp.h>
void core_runtime_error(int code, const char *msg);
void core_runtime_error_soft(int code, const char *msg);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int core_err_is_terminal(int code) {
    switch (code) { case 20: case 21: case 22: case 23: case 26: case 27: case 29: case 30: case 31: case 38: case 39: case 242: return 1; default: return 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int core_err_survives_errlimit(int code) { switch (code) { case 38: case 242: case 332: return 0; default: return 1; } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int core_err_is_fatal(int code) { switch (code) { case 19: case 25: case 35: return 1; default: return 0; } }
#ifdef __cplusplus
}
#endif
#endif
