#pragma GCC diagnostic ignored "-Wmisleading-indentation"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core.h"
#include "sil_macros.h"
#include "bb_build.h"
#include "bb_pool.h"
#include "emit.h"
#include "lower.h"
#include "ast.h"
#include "bb_box.h"
const char *Σ = NULL;
int         Δ = 0;
int         Ω = 0;
int         g_scan_pre_delta = 0;
int         Σlen = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bin_audit_print(void)
{
}
