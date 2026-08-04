/* rtx_str_shim.c — Pristine-build companion to rtx_str.S (RTX-STR rail, s237).
 *
 * PURPOSE: bench_rtx_3arm.sh requires a PRISTINE .so (port absent entirely) as the
 * honest baseline.  rtx_str.S exports str_concat_d, VARVAL_fn, and defines the hidden
 * rtx_gate_str byte.  Without the .S those three are undefined and the pristine link
 * fails.  This shim supplies all three via thin C wrappers so the pristine .so links
 * and behaves identically to a build that never heard of the asm port.
 *
 * USAGE (Makefile swap — do NOT commit the Makefile edit):
 *   # save ported .so aside, then:
 *   sed 's|rtx_str\.S|rtx_str_shim.c|' Makefile > /tmp/Makefile.pristine
 *   make -f /tmp/Makefile.pristine libscrip_rt
 *   cp out/libscrip_rt.so /tmp/rtx_pristine/
 *   # restore:
 *   cp /tmp/rtx_ported/libscrip_rt.so out/libscrip_rt.so
 *
 * DO NOT add this file to Makefile's RT_PIC_SRCS in a normal build — it would
 * define str_concat_d and VARVAL_fn twice (once here, once from rtx_str.S) and
 * the link would fail with duplicate-symbol errors.
 */
#include "../core/core.h"
#include "../rt/rt_arena.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__attribute__((visibility("hidden"))) unsigned char rtx_gate_str = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t c_str_concat_d(DESCR_t a, DESCR_t b);
DESCR_t str_concat_d(DESCR_t a, DESCR_t b) { return c_str_concat_d(a, b); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
char *c_VARVAL_fn(DESCR_t v);
char *VARVAL_fn(DESCR_t v) { return c_VARVAL_fn(v); }
