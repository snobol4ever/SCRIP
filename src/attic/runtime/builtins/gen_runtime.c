/* DEAD CODE — excised from src/runtime/builtins/gen_runtime.c
 * Reason: sm_yield_to_caller — Stack Machine yield stub; SM excised.
 *         No callers after SM removal. GC oracle confirmed dead 2026-06-14.
 */
int sm_yield_to_caller(DESCR_t v) { (void)v; return 0; }
