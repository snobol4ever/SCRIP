/* DEAD CODE — excised from src/runtime/rt/rt.c
 * Reason: sm_opcode_name (weak) — SM opcode debug helper; SM excised, no callers.
 *         _is_pat_fnc_name, _expr_is_pat (weak) — superseded by strong definitions
 *         in src/driver/interp_hooks.c; weak fallbacks never reached.
 *         GC oracle confirmed dead 2026-06-14.
 */
__attribute__((weak)) const char *sm_opcode_name(SM_op_t op) { (void)op; return "?"; }
__attribute__((weak)) int _is_pat_fnc_name(const char *s)    { (void)s; return 0; }
__attribute__((weak)) int _expr_is_pat(tree_t *e)             { (void)e; return 0; }
