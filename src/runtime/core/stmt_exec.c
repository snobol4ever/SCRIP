#pragma GCC diagnostic ignored "-Wmisleading-indentation"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gc/gc.h>
#include "core.h"
#include "sil_macros.h"
#include "bb_build.h"
#include "bb_pool.h"
#include "emit_bb.h"
#include "lower.h"
int rt_in_native_chunk(void) __attribute__((weak));
int rt_in_native_chunk(void) { return 0; }
#include "ast.h"
#include "bb_box.h"
/*--------------------------------------------------------------------------------------------------------------------*/
const char *Σ = NULL;
int         Δ = 0;
int         Ω = 0;
int         g_scan_pre_delta = 0;
int         Σlen = 0;
/*--------------------------------------------------------------------------------------------------------------------*/
void bin_audit_print(void)
{
}
/*--------------------------------------------------------------------------------------------------------------------*/
int exec_stmt(const char  *subj_name,
                  DESCR_t     *subj_var,
                  DESCR_t      pat,
                  DESCR_t     *repl,
                  int          has_repl)
{
    (void)subj_name; (void)subj_var; (void)pat; (void)repl; (void)has_repl;
    fprintf(stderr, "[B0] BOMB exec_stmt: legacy executor deleted (B0); DT_P match lands at B3 (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
