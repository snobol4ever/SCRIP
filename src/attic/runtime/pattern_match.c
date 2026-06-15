/* ATTIC: dead code excised from src/runtime/pattern_match.c (commit base 2a35216) — unreachable from main per --gc-sections oracle. Provenance only; not compiled. */
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_ref(const char *name) {
    fprintf(stderr, "[B0] BOMB pat_ref: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_assign_imm(DESCR_t child, DESCR_t var) {
    fprintf(stderr, "[B0] BOMB pat_assign_imm: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_assign_cond(DESCR_t child, DESCR_t var) {
    fprintf(stderr, "[B0] BOMB pat_assign_cond: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_assign_callcap(DESCR_t child, const char *fnc_name, DESCR_t *args, int nargs) {
    fprintf(stderr, "[B0] BOMB pat_assign_callcap: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_assign_callcap_named(DESCR_t child, const char *fnc_name, DESCR_t *args, int nargs, char **arg_names, int n_arg_names) {
    fprintf(stderr, "[B0] BOMB pat_assign_callcap_named: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_assign_callcap_named_imm(DESCR_t child, const char *fnc_name, DESCR_t *args, int nargs, char **arg_names, int n_arg_names) {
    fprintf(stderr, "[B0] BOMB pat_assign_callcap_named_imm: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t var_as_pattern(DESCR_t v) {
    fprintf(stderr, "[B0] BOMB var_as_pattern: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_user_call(const char *name, DESCR_t *args, int nargs) {
    fprintf(stderr, "[B0] BOMB pat_user_call: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_at_cursor(const char *varname) {
    fprintf(stderr, "[B0] BOMB pat_at_cursor: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_exec_stmt_pat(void *blob_α, const char *subj_name, int has_repl)
{
    (void)blob_α;
    (void)subj_name;
    (void)has_repl;
    STACKLESS_ABORT("rt_exec_stmt_pat");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_match_blob(void *blob_α,
                         const char *subj_name,
                         int has_repl)
{
    (void)blob_α;
    (void)subj_name;
    (void)has_repl;
    STACKLESS_ABORT("rt_match_blob");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_lit(const char *s)
{
    (void)s;
    STACKLESS_ABORT("rt_pat_lit");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_refname(const char *name)
{
    (void)name;
    STACKLESS_ABORT("rt_pat_refname");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_span(void)
{
    STACKLESS_ABORT("rt_pat_span");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_break(void)
{
    STACKLESS_ABORT("rt_pat_break");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_breakx(void)
{
    STACKLESS_ABORT("rt_pat_breakx");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_any(void)
{
    STACKLESS_ABORT("rt_pat_any");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_notany(void)
{
    STACKLESS_ABORT("rt_pat_notany");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_len(void)
{
    STACKLESS_ABORT("rt_pat_len");
}
void rt_pat_pos(void)
{
    STACKLESS_ABORT("rt_pat_pos");
}
void rt_pat_rpos(void)
{
    STACKLESS_ABORT("rt_pat_rpos");
}
void rt_pat_tab(void)
{
    STACKLESS_ABORT("rt_pat_tab");
}
void rt_pat_rtab(void)
{
    STACKLESS_ABORT("rt_pat_rtab");
}
void rt_pat_arb(void)
{
    STACKLESS_ABORT("rt_pat_arb");
}
void rt_pat_rem(void)
{
    STACKLESS_ABORT("rt_pat_rem");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_dcap_clear(void) { g_rt_dcap_n = 0; }
void rt_cap_assign(const char *varname, const char *base, int len)
{
    if (!varname || !*varname) return;
    if (len < 0) len = 0;
    if (g_rt_dcap_active) { rt_dcap_record(varname, base, len); return; }
    char *copy = (char *)GC_MALLOC((size_t)len + 1);
    if (copy) { if (len > 0 && base) memcpy(copy, base, (size_t)len); copy[len] = '\0'; }
    DESCR_t matched = { .v = DT_S, .slen = (uint32_t)len, .s = copy ? copy : "" };
    NV_SET_fn(varname, matched);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_fence(void)
{
    STACKLESS_ABORT("rt_pat_fence");
}
void rt_pat_fail(void)
{
    STACKLESS_ABORT("rt_pat_fail");
}
void rt_pat_abort(void)
{
    STACKLESS_ABORT("rt_pat_abort");
}
void rt_pat_succeed(void)
{
    STACKLESS_ABORT("rt_pat_succeed");
}
void rt_pat_bal(void)
{
    STACKLESS_ABORT("rt_pat_bal");
}
void rt_pat_eps(void)
{
    STACKLESS_ABORT("rt_pat_eps");
}
void rt_pat_arbno(void)
{
    STACKLESS_ABORT("rt_pat_arbno");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_fence1(void)
{
    STACKLESS_ABORT("rt_pat_fence1");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_cat(void)
{
    STACKLESS_ABORT("rt_pat_cat");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_alt(void)
{
    STACKLESS_ABORT("rt_pat_alt");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_deref(void)
{
    STACKLESS_ABORT("rt_pat_deref");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_capture(const char *varname, int kind)
{
    (void)varname;
    (void)kind;
    STACKLESS_ABORT("rt_pat_capture");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_capture_fn(const char *fname, int is_imm, const char *namelist)
{
    (void)fname;
    (void)is_imm;
    (void)namelist;
    STACKLESS_ABORT("rt_pat_capture_fn");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_capture_fn_args(const char *fname, int is_imm, int nargs)
{
    (void)fname;
    (void)is_imm;
    (void)nargs;
    STACKLESS_ABORT("rt_pat_capture_fn_args");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_usercall(const char *fname)
{
    (void)fname;
    STACKLESS_ABORT("rt_pat_usercall");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_usercall_args(const char *fname, int nargs)
{
    (void)fname;
    (void)nargs;
    STACKLESS_ABORT("rt_pat_usercall_args");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_match_variant(const char *subj_name, int has_repl)
{
    (void)subj_name;
    (void)has_repl;
    STACKLESS_ABORT("rt_match_variant");
}
