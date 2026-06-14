/* DEAD CODE — declaration excised from src/contracts/SM.h
 * Reason: sm_opcode_name — SM opcode debug helper; SM excised, no callers.
 *         GC oracle confirmed dead 2026-06-14.
 */
const char *sm_opcode_name(SM_op_t op);

/* DEAD CODE — additionally excised from src/contracts/ast_print.c
 * Reason: ir_set_print_width / ir_get_print_width — only caller was --dump-width (removed).
 *         ast_print.c uses ast_print_width directly on line 69; no external readers.
 *         GC oracle confirmed dead 2026-06-14.
 */
