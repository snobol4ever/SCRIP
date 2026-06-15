/* DEAD CODE — excised from src/runtime/builtins/gen_runtime.c
 * Reason: sm_yield_to_caller — Stack Machine yield stub; SM excised.
 *         No callers after SM removal. GC oracle confirmed dead 2026-06-14.
 */
int sm_yield_to_caller(DESCR_t v) { (void)v; return 0; }
int  frame_lookup(tree_t *n, long *out) {
    GenFrame *f = &FRAME;
    for (int i=f->gen_depth-1;i>=0;i--) if(f->gen[i].node==n){*out=f->gen[i].cur;return 1;} return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int  frame_lookup_sv(tree_t *n, long *out, const char **sv) {
    GenFrame *f = &FRAME;
    for (int i=f->gen_depth-1;i>=0;i--) if(f->gen[i].node==n){*out=f->gen[i].cur;*sv=f->gen[i].sval;return 1;} return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int frame_env_active(void) {
    return frame_depth > 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t frame_env_load(int slot) {
    if (frame_depth <= 0) return FAILDESCR;
    GenFrame *f = &FRAME;
    if (slot < 0 || slot >= f->env_n) return FAILDESCR;
    return f->env[slot];
}
/*--------------------------------------------------------------------------------------------------------------------*/
void frame_env_store(int slot, DESCR_t val) {
    if (frame_depth <= 0) return;
    GenFrame *f = &FRAME;
    if (slot < 0 || slot >= FRAME_SLOT_MAX) return;
    if (slot >= f->env_n) f->env_n = slot + 1;
    f->env[slot] = val;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int is_suspendable(tree_t *e) {
    if (!e) return 0;
    switch (e->t) {
        case TT_TO: case TT_TO_BY: case TT_ITERATE: case TT_ALTERNATE:
        case TT_SUSPEND: case TT_LIMIT:
        case TT_BANG_BINARY: case TT_SEQ_EXPR:
            return 1;
        case TT_FNC:
            return 1;
        case TT_IDX:
            for (int i = 1; i < e->n; i++)
                if (is_suspendable(e->c[i])) return 1;
            return 0;
        case TT_ASSIGN:
            if (e->n >= 2 && e->c[0] && e->c[0]->t == TT_ITERATE) return 1;
            return (e->n >= 2 && is_suspendable(e->c[1])) ? 1 : 0;
        case TT_REVASSIGN:
            return 1;
        case TT_REVSWAP:
            return 1;
        case TT_ADD: case TT_SUB: case TT_MUL: case TT_DIV: case TT_MOD:
        case TT_LT:  case TT_LE:  case TT_GT:  case TT_GE:
        case TT_EQ:  case TT_NE:
        case TT_IDENTICAL:
        case TT_LCONCAT: case TT_CAT:
                           for (int i = 0; i < e->n; i++)
                if (is_suspendable(e->c[i])) return 1;
            return 0;
        case TT_CSET_COMPL: case TT_CSET_UNION: case TT_CSET_DIFF: case TT_CSET_INTER:
            for (int i = 0; i < e->n; i++)
                if (is_suspendable(e->c[i])) return 1;
            return 0;
        case TT_NONNULL:
            return is_suspendable(e->n > 0 ? e->c[0] : NULL);
        case TT_SCAN:
            return is_suspendable(e->n > 0 ? e->c[0] : NULL)
                || is_suspendable(e->n > 1 ? e->c[1] : NULL);
        case TT_NULL:
            return 0;
        default:
            return 0;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
bb_node_t gen_bb_pump_proc_by_name(const char *name, DESCR_t *args, int nargs) {
    fprintf(stderr, "[SBL] FATAL: gen_bb_pump_proc_by_name: brokered generator path removed\n");
    abort();
    (void)name; (void)args; (void)nargs;
    return (bb_node_t){ NULL, NULL, 0 };
}
int string_section_assign(tree_t *lhs, DESCR_t val) {
    if (!lhs) return 0;
    int kind = lhs->t;
    if (kind != TT_SECTION && kind != TT_SECTION_PLUS &&
        kind != TT_SECTION_MINUS && kind != TT_IDX) return 0;
    if (lhs->n < 2) return 0;
    if (kind == TT_SECTION && lhs->n < 3) return 0;
    extern int     frame_env_active(void);
    extern DESCR_t frame_env_load(int slot);
    extern void    frame_env_store(int slot, DESCR_t val);
    tree_t *bch = lhs->c[0];
    DESCR_t *cell = NULL;
    DESCR_t _gen_base_storage;
    int _gen_bb_var_slot = -1;
    if (bch && bch->t == TT_VAR && g_lang == LANG_ICN && frame_env_active()) {
        _gen_bb_var_slot = (bch->v.sval) ? scope_get(&FRAME.sc, bch->v.sval) : -1;
        if (_gen_bb_var_slot >= 0) {
            _gen_base_storage = frame_env_load(_gen_bb_var_slot);
            cell = &_gen_base_storage;
        }
    }
    if (!cell && bch && bch->t == TT_VAR && frame_depth > 0) {
        int sl = (int)bch->v.ival;
        if (sl >= 0 && sl < FRAME.env_n) cell = &FRAME.env[sl];
    }
    if (!cell) cell = interp_eval_ref(bch);
    if (!cell) return 0;
    DESCR_t base = *cell;
    if (kind == TT_IDX) {
        if (base.v != DT_S && base.v != DT_SNUL) return 0;
    }
    const char *s = (base.v == DT_SNUL) ? "" : VARVAL_fn(base);
    if (!s) s = "";
    int slen = (int)strlen(s);
    int lo = 0, hi = 0;
    if (kind == TT_SECTION) {
        DESCR_t _i1=FAILDESCR;
        DESCR_t _i2=FAILDESCR;
        int i=(int)to_int(_i1), j=(int)to_int(_i2);
        if (i == 0) i = slen + 1; else if (i < 0) i = slen + 1 + i;
        if (j == 0) j = slen + 1; else if (j < 0) j = slen + 1 + j;
        if (i < 1 || i > slen+1 || j < 1 || j > slen+1) return 0;
        lo = i < j ? i : j; hi = i < j ? j : i;
    } else if (kind == TT_SECTION_PLUS) {
        DESCR_t _sp1=FAILDESCR;
        DESCR_t _sp2=FAILDESCR;
        int i=(int)to_int(_sp1), n=(int)to_int(_sp2);
        if (i == 0) i = slen + 1; else if (i < 0) i = slen + 1 + i;
        if (i < 1 || i > slen+1) return 0;
        if (n < 0) return 0;
        if (i + n > slen + 1) return 0;
        lo = i; hi = i + n;
    } else if (kind == TT_SECTION_MINUS) {
        DESCR_t _sm1=FAILDESCR;
        DESCR_t _sm2=FAILDESCR;
        int i=(int)to_int(_sm1), n=(int)to_int(_sm2);
        if (i == 0) i = slen + 1; else if (i < 0) i = slen + 1 + i;
        if (i < 1 || i > slen+1) return 0;
        if (n < 0) return 0;
        if (i - n < 1) return 0;
        lo = i - n; hi = i;
    } else {
        DESCR_t _idx_d = FAILDESCR;
        int i = (int)to_int(_idx_d);
        if (i == 0) return 0;
        if (i < 0) i = slen + 1 + i;
        if (i < 1 || i > slen) return 0;
        lo = i; hi = i + 1;
    }
    const char *vs = VARVAL_fn(val); if (!vs) vs = "";
    int vlen = (int)strlen(vs);
    int prefix = lo - 1;
    int suffix = slen - (hi - 1);
    int newlen = prefix + vlen + suffix;
    char *buf = (char *)GC_malloc((size_t)newlen + 1);
    if (prefix > 0) memcpy(buf, s, (size_t)prefix);
    if (vlen > 0)   memcpy(buf + prefix, vs, (size_t)vlen);
    if (suffix > 0) memcpy(buf + prefix + vlen, s + hi - 1, (size_t)suffix);
    buf[newlen] = '\0';
    tree_t *base_expr = lhs->c[0];
    if (_gen_bb_var_slot >= 0) {
        frame_env_store(_gen_bb_var_slot, STRVAL(buf));
    } else if (base_expr && base_expr->t == TT_VAR && base_expr->v.sval &&
               base_expr->v.sval[0] != '&' &&
               !(frame_depth > 0 && base_expr->v.ival >= 0 && base_expr->v.ival < FRAME.env_n)) {
        set_and_trace(base_expr->v.sval, STRVAL(buf));
    } else {
        *cell = STRVAL(buf);
    }
    return 1;
}
