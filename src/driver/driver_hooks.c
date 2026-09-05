#include "driver_private.h"
extern DESCR_t rt_call_named_proc(const char *name, DESCR_t *args, int nargs);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t _eval_str_impl_fn(const char *s) {
    extern DESCR_t eval_string_transient(const char *s);
    return eval_string_transient(s);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int _label_exists_fn(const char *name) {
    return label_lookup(name) != NULL;
}
DESCR_t _builtin_IDENT(DESCR_t *args, int nargs);
DESCR_t _builtin_DIFFER(DESCR_t *args, int nargs);
DESCR_t _builtin_DATA(DESCR_t *args, int nargs);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t _usercall_hook(const char *name, DESCR_t *args, int nargs) {
    if (strcmp(name, "IDENT") == 0)  return _builtin_IDENT(args, nargs);
    if (strcmp(name, "DIFFER") == 0) return _builtin_DIFFER(args, nargs);
    if (strcmp(name, "DATA") == 0)   return _builtin_DATA(args, nargs);
    if (strcmp(name, "ITEM") == 0 && nargs >= 2) {
        if (nargs >= 3) return subscript_get2(args[0], args[1], args[2]);
        return subscript_get(args[0], args[1]);
    }
    if (strcmp(name, "ITEM_SET") == 0 && nargs >= 3) {
        DESCR_t rhs = args[0], arr = args[1], idx = args[2];
        if (nargs >= 4) { subscript_set2(arr, idx, args[3], rhs); }
        else            { subscript_set(arr, idx, rhs); }
        return rhs;
    }
    {
        DatType *_dt = dat_find_type(name);
        if (_dt) return dat_construct(_dt, args, nargs);
        int _fi = 0;
        DatType *_ft = dat_find_field(name, &_fi);
        if (_ft && nargs >= 1) return dat_field_get(name, args[0]);
        size_t _nlen = strlen(name);
        if (_nlen > 4 && strcmp(name + _nlen - 4, "_SET") == 0 && nargs >= 2) {
            char _fname[128];
            size_t _flen = _nlen - 4;
            if (_flen >= sizeof(_fname)) _flen = sizeof(_fname) - 1;
            memcpy(_fname, name, _flen); _fname[_flen] = '\0';
            DESCR_t *_cell = data_field_ptr(_fname, args[1]);
            if (_cell) { *_cell = args[0]; return args[0]; }
        }
    }
    const tree_t *_body = NULL;
    if (!_body) {
        for (int _i = 0; _i < g_stage2.proc_count; _i++) {
            if (g_stage2.proc_table[_i].name && strcmp(g_stage2.proc_table[_i].name, name) == 0) {
                return proc_table_call(_i, args, nargs);
            }
        }
    }
    extern int rt_g_want_name;
    int _wn_pre = rt_g_want_name;
    if (FNCEX_fn(name)) {
        DESCR_t _r = rt_call_named_proc(name, args, nargs); if (!IS_FAIL_fn(_r)) return _r; const char *_ent = FUNC_ENTRY_fn(name);
        if (_ent && strcmp(_ent, name) != 0) { _r = rt_call_named_proc(_ent, args, nargs); if (!IS_FAIL_fn(_r)) return _r; }
    }
    rt_g_want_name = _wn_pre;
    return call_user_function(name, args, nargs);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void ir_dump_program(const tree_t *prog, FILE *f) {
    if (!prog) { fprintf(f, "(NULL-PROGRAM)\n"); return; }
    for (int i = 0; i < prog->n; i++) {
        const tree_t *s = prog->c[i];
        if (!s) continue;
        fprintf(f, "(STMT");
        const char *lbl  = stmt_attr_str(stmt_attr_find(s, ":lbl"));
        int has_eq = stmt_attr_find(s, ":eq") != NULL;
        if (lbl)         fprintf(f, " :lbl %s", lbl);
        if (has_eq)      fprintf(f, " :eq");
        if (s->t == TT_END) fprintf(f, " :end");
        tree_t *subj = stmt_attr_expr(stmt_attr_find(s, ":subj"));
        tree_t *pat  = stmt_attr_expr(stmt_attr_find(s, ":pat"));
        tree_t *repl = stmt_attr_expr(stmt_attr_find(s, ":repl"));
        if (subj) { fprintf(f, " :subj "); ir_print_node(subj, f); }
        if (pat)  { fprintf(f, " :pat ");  ir_print_node(pat, f);  }
        if (repl) { fprintf(f, " :repl "); ir_print_node(repl, f); }
        const char *go  = goto_node_str(stmt_goto_find(s, TT_GOTO_U));
        const char *goS = goto_node_str(stmt_goto_find(s, TT_GOTO_S));
        const char *goF = goto_node_str(stmt_goto_find(s, TT_GOTO_F));
        if (go)  fprintf(f, " :go %s",  go);
        if (goS) fprintf(f, " :goS %s", goS);
        if (goF) fprintf(f, " :goF %s", goF);
        fprintf(f, ")\n");
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t _builtin_IDENT(DESCR_t *args, int nargs) {
    if (nargs == 1) return IS_NULL_fn(args[0]) ? NULVCL : FAILDESCR;
    if (nargs >= 2) {
        int a_null = IS_NULL_fn(args[0]), b_null = IS_NULL_fn(args[1]);
        if (a_null && b_null) return NULVCL;
        if (a_null || b_null) return FAILDESCR;
        if (args[0].v != args[1].v) return FAILDESCR;
        const char *sa = VARVAL_fn(args[0]), *sb = VARVAL_fn(args[1]);
        if (!sa) sa = ""; if (!sb) sb = "";
        return strcmp(sa, sb) == 0 ? NULVCL : FAILDESCR;
    }
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t _builtin_DIFFER(DESCR_t *args, int nargs) {
    if (nargs == 1) return IS_NULL_fn(args[0]) ? FAILDESCR : NULVCL;
    if (nargs >= 2) {
        int a_null = IS_NULL_fn(args[0]), b_null = IS_NULL_fn(args[1]);
        if (a_null && b_null) return FAILDESCR;
        if (a_null || b_null) return NULVCL;
        if (args[0].v != args[1].v) return NULVCL;
        const char *sa = VARVAL_fn(args[0]), *sb = VARVAL_fn(args[1]);
        if (!sa) sa = ""; if (!sb) sb = "";
        return strcmp(sa, sb) != 0 ? NULVCL : FAILDESCR;
    }
    return FAILDESCR;
}
extern DESCR_t EVAL_fn(DESCR_t);
extern DESCR_t code(const char *);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t _builtin_EVAL(DESCR_t *args, int nargs) {
    if (nargs < 1) return FAILDESCR;
    return EVAL_fn(args[0]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t _builtin_CODE(DESCR_t *args, int nargs) {
    if (nargs < 1) return FAILDESCR;
    const char *s = VARVAL_fn(args[0]);
    if (!s || !*s) return FAILDESCR;
    return code(s);
}
static const char *PAT_FNC_NAMES[] = {
    "ANY","NOTANY","SPAN","BREAK","BREAKX","LEN","POS","RPOS","TAB","RTAB",
    "ARB","ARBNO","REM","FAIL","SUCCEED","FENCE","ABORT","BAL","CALL", NULL
};
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int _is_pat_fnc_name(const char *s) {
    if (!s) return 0;
    for (int i = 0; PAT_FNC_NAMES[i]; i++)
        if (strcmp(s, PAT_FNC_NAMES[i]) == 0) return 1;
    return 0;
}
