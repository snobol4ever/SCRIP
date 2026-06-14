/* DEAD CODE — declarations excised from src/driver/interp.h
 * Reason: execute_program_steps (stub removed), g_ir_step_limit/done/jmp
 *         (step-limit interp control — no callers post-M2 deletion).
 * GC oracle: --gc-sections confirmed dead (2026-06-14).
 */
void    execute_program_steps(const tree_t *prog, int n);
extern int     g_ir_step_limit;
extern int     g_ir_steps_done;
extern jmp_buf g_ir_step_jmp;
