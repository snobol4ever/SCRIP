#include <stdlib.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern __attribute__((visibility("hidden"))) unsigned char rtx_gate_misc;
extern __attribute__((visibility("hidden"))) unsigned char rtx_gate_alloc;
extern __attribute__((visibility("hidden"))) unsigned char rtx_gate_str;
extern __attribute__((visibility("hidden"))) unsigned char rtx_gate_call;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static unsigned char rtx_env_on(const char *name, unsigned char dflt) { const char *e = getenv(name); if (!e || !*e) return dflt; return (unsigned char)(e[0] != '0'); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__attribute__((constructor)) static void rtx_gates_init(void) { rtx_gate_misc = rtx_env_on("SCRIP_RTX_MISC", 1); rtx_gate_alloc = rtx_env_on("SCRIP_RTX_ALLOC", 1); rtx_gate_str = rtx_env_on("SCRIP_RTX_STR", 1); rtx_gate_call = rtx_env_on("SCRIP_RTX_CALL", 0); }
