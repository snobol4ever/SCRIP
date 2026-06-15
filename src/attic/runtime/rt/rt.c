/* DEAD CODE — excised from src/runtime/rt/rt.c
 * Reason: sm_opcode_name (weak) — SM opcode debug helper; SM excised, no callers.
 *         _is_pat_fnc_name, _expr_is_pat (weak) — superseded by strong definitions
 *         in src/driver/interp_hooks.c; weak fallbacks never reached.
 *         GC oracle confirmed dead 2026-06-14.
 */
__attribute__((weak)) const char *sm_opcode_name(SM_op_t op) { (void)op; return "?"; }
__attribute__((weak)) int _is_pat_fnc_name(const char *s)    { (void)s; return 0; }
__attribute__((weak)) int _expr_is_pat(tree_t *e)             { (void)e; return 0; }
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_vstack_depth(void) { STACKLESS_ABORT("rt_vstack_depth"); return 0; }
DESCR_t rt_vstack_pop(void) { STACKLESS_ABORT("rt_vstack_pop"); return FAILDESCR; }
/*--------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _rt_IDENT(DESCR_t *a, int n)
{
    if (n == 1) return IS_NULL_fn(a[0]) ? NULVCL : FAILDESCR;
    const char *s1 = VARVAL_fn(a[0]); if (!s1) s1 = "";
    const char *s2 = VARVAL_fn(a[1]); if (!s2) s2 = "";
    return (strcmp(s1, s2) == 0) ? NULVCL : FAILDESCR;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _rt_DIFFER(DESCR_t *a, int n)
{
    if (n == 1) return IS_NULL_fn(a[0]) ? FAILDESCR : NULVCL;
    const char *s1 = VARVAL_fn(a[0]); if (!s1) s1 = "";
    const char *s2 = VARVAL_fn(a[1]); if (!s2) s2 = "";
    return (strcmp(s1, s2) != 0) ? NULVCL : FAILDESCR;
}
void rt_register_expressions(const rt_expression_entry *tbl)
{
    if (!tbl) return;
    for (; tbl->name && g_expression_reg_count < EXPRESSION_REG_MAX; tbl++) {
        g_expression_reg[g_expression_reg_count].name = tbl->name;
        g_expression_reg[g_expression_reg_count].fn   = tbl->fn;
        g_expression_reg_count++;
    }
}
void rt_init_cap(void **slot_ptr, void *child_fn, const char *varname, int immediate)
{
    if (!slot_ptr || !child_fn) return;
    cap_t *c = bb_cap_new((bb_box_fn)child_fn, NULL, varname, NULL, immediate);
    *slot_ptr = c;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_init_cap_call(void **slot_ptr, void *child_fn, const char *fnc_name)
{
    if (!slot_ptr || !child_fn) return;
    cap_t *c = bb_cap_new_call((bb_box_fn)child_fn, NULL, fnc_name, NULL, 0, NULL, 0, 0);
    *slot_ptr = c;
}
void rt_init_arbno(void **slot_ptr, void *child_fn)
{
    if (!slot_ptr || !child_fn) return;
    *slot_ptr = bb_arbno_new(child_fn, NULL);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void *chunk_reg_lookup(const char *name)
{
    if (!name || !*name) return NULL;
    for (int i = 0; i < g_expression_reg_count; i++) {
        if (strcmp(g_expression_reg[i].name, name) == 0)
            return g_expression_reg[i].fn;
    }
    return NULL;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static DESCR_t call_native_chunk(const char *fname, void *fn,
                                  DESCR_t *args, int nargs)
{
    static const DESCR_t SNUL_D = { DT_SNUL, {0}, 0, NULL };
    const char *entry = fname ? FUNC_ENTRY_fn(fname) : NULL;
    const char *retname = (entry && fname && strcmp(entry, fname) != 0 && FNCEX_fn(entry)) ? entry : fname;
    if (!retname) retname = fname ? fname : "";
    DESCR_t saved_ret = NV_GET_fn(retname);
    NV_SET_fn(retname, SNUL_D);
    DESCR_t saved_p[32];
    const char *pnames[32];
    int nbound = 0;
    for (int k = 0; k < nargs && k < 32; k++) {
        const char *pname = FUNC_PARAM_fn(fname, k);
        if (!pname || !*pname) break;
        pnames[nbound] = pname;
        saved_p[nbound] = NV_GET_fn(pname);
        NV_SET_fn(pname, args[k]);
        nbound++;
    }
    int nl = fname ? FUNC_NLOCALS_fn(fname) : 0;
    if (nl > 32) nl = 32;
    DESCR_t saved_l[32];
    const char *lnames[32];
    for (int k = 0; k < nl; k++) {
        const char *lname = FUNC_LOCAL_fn(fname, k);
        lnames[k] = lname ? lname : "";
        saved_l[k] = NV_GET_fn(lnames[k]);
        NV_SET_fn(lnames[k], SNUL_D);
    }
    typedef void (*chunk_fn_t)(void);
    chunk_fn_t cfn = (chunk_fn_t)fn;
    g_native_chunk_depth++;
    cfn();
    g_native_chunk_depth--;
    DESCR_t result = NV_GET_fn(retname);
    for (int k = nl - 1; k >= 0; k--)
        NV_SET_fn(lnames[k], saved_l[k]);
    for (int k = nbound - 1; k >= 0; k--)
        NV_SET_fn(pnames[k], saved_p[k]);
    NV_SET_fn(retname, saved_ret);
    return result;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _rt_usercall(const char *name, DESCR_t *args, int nargs)
{
    if (!name || !*name) return FAILDESCR;
    void *fn = chunk_reg_lookup(name);
    if (fn) return call_native_chunk(name, fn, args, nargs);
    DESCR_t nv = NV_GET_fn(name);
    if (!IS_FAIL_fn(nv) && nv.v == DT_E && nv.ptr) {
        typedef DESCR_t (*cfn_t)(DESCR_t *, int);
        cfn_t cfn = (cfn_t)nv.ptr;
        return cfn(args, nargs);
    }
    return FAILDESCR;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_init(int argc, char **argv)
{
    (void)argc; (void)argv;
    setvbuf(stdout, NULL, _IOLBF, 0);
    bb_pool_init();
    { extern void pat_pool_init(void); pat_pool_init(); }
    core_lib_init();
    trail_init(&g_resolve_trail);
    prolog_atom_init();
    register_fn("IDENT",  _rt_IDENT,  1, 2);
    register_fn("DIFFER", _rt_DIFFER, 1, 2);
    g_user_call_hook = _rt_usercall;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_push_int(int64_t v)
{
    (void)v;
    STACKLESS_ABORT("rt_push_int");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_halt_tos(void)
{
    STACKLESS_ABORT("rt_halt_tos");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_push_str(const char *s, uint32_t slen)
{
    (void)s;
    (void)slen;
    STACKLESS_ABORT("rt_push_str");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pop_write_int_nl(void)
{
    STACKLESS_ABORT("rt_pop_write_int_nl");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pop_write_any_nl(void)
{
    STACKLESS_ABORT("rt_pop_write_any_nl");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pop_nv_set(const char *name)
{
    (void)name;
    STACKLESS_ABORT("rt_pop_nv_set");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_nv_get(const char *name)
{
    (void)name;
    STACKLESS_ABORT("rt_nv_get");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_nv_set(const char *name)
{
    (void)name;
    STACKLESS_ABORT("rt_nv_set");
}
rt_subj_t rt_subject_load(const char *name, const char *lit)
{
    rt_subj_t r;
    if (lit) {
        r.base = lit;
        r.len  = (long)strlen(lit);
    } else {
        DESCR_t d = VARVAL_d_fn(NV_GET_fn(name ? name : ""));
        if (d.v == DT_S || d.v == DT_SNUL) {
            r.base = d.s ? d.s : "";
            r.len  = d.slen ? (long)d.slen : (long)strlen(r.base);
        } else {
            const char *s = VARVAL_fn(NV_GET_fn(name ? name : ""));
            r.base = s ? s : "";
            r.len  = (long)strlen(r.base);
        }
    }
    g_subject_dbg_base = r.base;
    g_subject_dbg_len  = r.len;
    return r;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_match_lit(const char *subj, long subj_len, const char *lit, long lit_len, int anchored, long *m_start, long *m_end)
{
    if (!subj) subj = "";
    if (!lit)  lit  = "";
    if (lit_len < 0) lit_len = (long)strlen(lit);
    long max_start = subj_len - lit_len;
    if (max_start < 0) return 0;
    long last_start = anchored ? 0 : max_start;
    for (long start = 0; start <= last_start; start++) {
        if (lit_len == 0 || memcmp(subj + start, lit, (size_t)lit_len) == 0) {
            if (m_start) *m_start = start;
            if (m_end)   *m_end   = start + lit_len;
            return 1;
        }
    }
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pop_void(void)
{
    STACKLESS_ABORT("rt_pop_void");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pop_store_i64(int64_t *slot)
{
    (void)slot;
    STACKLESS_ABORT("rt_pop_store_i64");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_push_stored_i64(const int64_t *slot)
{
    (void)slot;
    STACKLESS_ABORT("rt_push_stored_i64");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pop_store_descr(DESCR_t *slot)
{
    (void)slot;
    STACKLESS_ABORT("rt_pop_store_descr");
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_case_eq(const DESCR_t *sel, const DESCR_t *key)
{
    if (!sel || !key) return 0;
    if (sel->v == DT_I && key->v == DT_I) return sel->i == key->i;
    { const char *ss = VARVAL_fn(*sel); const char *ks = VARVAL_fn(*key); if (ss && ks) return strcmp(ss, ks) == 0; return ss == ks; }
}
void rt_frame_enter(int nparams)
{
    (void)nparams;
    STACKLESS_ABORT("rt_frame_enter");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_frame_leave(void)
{
    if (g_rt_frame_depth > 0) g_rt_frame_depth--;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_call_builtin(const char *name, int nargs)
{
    (void)name;
    (void)nargs;
    STACKLESS_ABORT("rt_call_builtin");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_load_frame(int slot)
{
    (void)slot;
    STACKLESS_ABORT("rt_load_frame");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_store_frame(int slot)
{
    (void)slot;
    STACKLESS_ABORT("rt_store_frame");
}
int rt_last_ok(void)
{
    return g_last_ok;
}
void rt_set_last_ok(int ok)
{
    g_last_ok = ok ? 1 : 0;
}
void rt_push_expression_descr(int64_t entry_pc, int64_t arity)
{
    (void)entry_pc;
    (void)arity;
    STACKLESS_ABORT("rt_push_expression_descr");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_concat(void)
{
    STACKLESS_ABORT("rt_concat");
}
void rt_gen_concat(void)
{
    STACKLESS_ABORT("rt_gen_concat");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_set_stno(int64_t stno)
{
    extern void comm_stno(int n);
    kw_stno = stno;
    comm_stno((int)stno);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_push_null(void)
{
    STACKLESS_ABORT("rt_push_null");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_push_real_bits(uint64_t bits)
{
    (void)bits;
    STACKLESS_ABORT("rt_push_real_bits");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_push_null_noflip(void)
{
    STACKLESS_ABORT("rt_push_null_noflip");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_push_expr(void *ptr)
{
    (void)ptr;
    STACKLESS_ABORT("rt_push_expr");
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_unop_neg(void)
{
    STACKLESS_ABORT("rt_unop_neg");
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_unop_pos(void)
{
    STACKLESS_ABORT("rt_unop_pos");
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_unop_nonnull(void)
{
    STACKLESS_ABORT("rt_unop_nonnull");
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_unop_null_test(void)
{
    STACKLESS_ABORT("rt_unop_null_test");
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_unop_not(void)
{
    STACKLESS_ABORT("rt_unop_not");
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_unop_size(void)
{
    STACKLESS_ABORT("rt_unop_size");
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_field_get(const char *fname)
{
    (void)fname;
    STACKLESS_ABORT("rt_field_get");
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_field_set(const char *fname)
{
    (void)fname;
    STACKLESS_ABORT("rt_field_set");
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_idx_get(void)
{
    STACKLESS_ABORT("rt_idx_get");
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_idx_set(void)
{
    STACKLESS_ABORT("rt_idx_set");
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_limit_begin(DESCR_t *max_slot, int64_t *count_slot)
{
    (void)max_slot;
    (void)count_slot;
    STACKLESS_ABORT("rt_limit_begin");
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_limit_more(DESCR_t *max_slot, int64_t *count_slot)
{
    (void)max_slot;
    (void)count_slot;
    STACKLESS_ABORT("rt_limit_more");
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_limit_inc(int64_t *count_slot)
{
    (void)count_slot;
    STACKLESS_ABORT("rt_limit_inc");
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_toby_real(DESCR_t *cur_slot, int64_t lo_bits, int64_t hi_bits, int64_t step_bits, int reset)
{
    (void)cur_slot;
    (void)lo_bits;
    (void)hi_bits;
    (void)step_bits;
    (void)reset;
    STACKLESS_ABORT("rt_toby_real");
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_define_entry(void)
{
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_define(void)
{
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_unhandled_sm(int op)
{
    fprintf(stderr, "libscrip_rt: unhandled SM opcode %d in emitted binary (M5 territory)\n", op);
    abort();
}
/*--------------------------------------------------------------------------------------------------------------------*/
Term **resolve_bb_env_install(Term **env)
{
    extern Term **g_resolve_env;
    Term **prev = g_resolve_env;
    g_resolve_env = env;
    return prev;
}
static DESCR_t _rt_nv_fold_get(const char *raw)
{
    if (!raw || !*raw) return NULVCL;
    char *n = GC_strdup(raw);
    return NV_GET_fn(n);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void _rt_nv_fold_set(const char *raw, DESCR_t val)
{
    if (!raw || !*raw) return;
    char *n = GC_strdup(raw);
    NV_SET_fn(n, val);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_call(const char *name, int nargs)
{
    (void)name;
    (void)nargs;
    STACKLESS_ABORT("rt_call");
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_do_return(int kind, int cond)
{
    (void)kind;
    (void)cond;
    STACKLESS_ABORT("rt_do_return");
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_do_nreturn(const char *fname, int cond)
{
    (void)fname;
    (void)cond;
    STACKLESS_ABORT("rt_do_nreturn");
    return 0;
}
void *rt_cs_new(const char *chars)
{
    rt_cs_t *z = GC_MALLOC(sizeof(rt_cs_t));
    z->chars = chars ? chars : "";
    z->delta = 0;
    return z;
}
static void rt_register_cap(cap_t *c)
{
    for (int i = 0; i < g_rt_cap_count; i++)
        if (g_rt_cap_list[i] == c) return;
    if (g_rt_cap_count < RT_MAX_CAPTURES)
        g_rt_cap_list[g_rt_cap_count++] = c;
}
/* DEAD CODE — excised from src/runtime/rt/rt.c (+ decl from src/runtime/rt/rt.h)
 * Reason: rt_in_native_chunk — 0 callers (none in runtime/driver/templates, not
 *         emitted: no x86 call-site → not in ROOTS_EMIT). Its sole input
 *         g_native_chunk_depth is a static never written anywhere (permanently 0,
 *         so the predicate was a constant 0). Unmasked by the batch-4 weak-stub
 *         removals; fixpoint iteration. GC oracle confirmed dead 2026-06-15.
 */
static int     g_native_chunk_depth = 0;
int rt_in_native_chunk(void) { return g_native_chunk_depth > 0; }
