#include "descr.h"
#include <stdlib.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern __attribute__((visibility("hidden"))) unsigned char rtx_gate_misc;
extern __attribute__((visibility("hidden"))) unsigned char rtx_gate_alloc;
extern __attribute__((visibility("hidden"))) unsigned char rtx_gate_str;
extern __attribute__((visibility("hidden"))) unsigned char rtx_gate_call;
extern __attribute__((visibility("hidden"))) unsigned char rtx_gate_leaf;
extern __attribute__((visibility("hidden"))) unsigned char rtx_gate_arith;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static unsigned char rtx_env_on(const char *name, unsigned char dflt) { const char *e = getenv(name); if (!e || !*e) return dflt; return (unsigned char)(e[0] != '0'); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__attribute__((constructor)) static void rtx_gates_init(void) { rtx_gate_misc = rtx_env_on("SCRIP_RTX_MISC", 1); rtx_gate_alloc = rtx_env_on("SCRIP_RTX_ALLOC", 1); rtx_gate_str = rtx_env_on("SCRIP_RTX_STR", 1); rtx_gate_call = rtx_env_on("SCRIP_RTX_CALL", 1); rtx_gate_leaf = rtx_env_on("SCRIP_RTX_LEAF", 1); rtx_gate_arith = rtx_env_on("SCRIP_RTX_ARITH", 1); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
_Static_assert(DT_SNUL == 0,  "rtx_abi.inc hardcodes DT_SNUL 0; descr.h drifted -- the asm tag compares would link fine and silently mis-compare");
_Static_assert(DT_S    == 1,  "rtx_abi.inc hardcodes DT_S 1; descr.h drifted -- update src/runtime/rtx/rtx_abi.inc to match");
_Static_assert(DT_P    == 3,  "rtx_abi.inc hardcodes DT_P 3; descr.h drifted -- str_concat_d's pattern guard would stop routing to pat_cat");
_Static_assert(DT_X    == 15, "rtx_abi.inc hardcodes DT_X 15; descr.h drifted -- str_concat_d's pattern guard would stop routing to pat_cat");
_Static_assert(DT_FAIL == 99, "rtx_abi.inc hardcodes DT_FAIL 99; descr.h drifted -- FAILDESCR precedence in the null-identity arm would break");
