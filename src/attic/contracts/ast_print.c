/* DEAD CODE — excised from src/contracts/ast_print.c
 * Reason: ir_set_print_width / ir_get_print_width — only caller was --dump-width (CLI removed 2026-06-14).
 *         ast_print.c uses ast_print_width directly; no external callers remain.
 *         GC oracle confirmed dead 2026-06-14.
 */
void ir_set_print_width(int w) { if (w > 0) ast_print_width = w; }
int  ir_get_print_width(void)  { return ast_print_width; }
