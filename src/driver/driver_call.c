#include "rt/rt_arena.h"
#include "driver_private.h"
CallFrame  call_stack[CALL_STACK_MAX];
int        call_depth = 0;
InitEnt init_tab[INIT_MAX];
int        init_n = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void init_update_snapshot(char **snames, DESCR_t *svals, int nsaved) {
    for (int ei = 0; ei < init_n; ei++) {
        InitEnt *ent = &init_tab[ei];
        for (int si = 0; si < ent->ns; si++) {
            for (int ni = 0; ni < nsaved; ni++) {
                if (snames[ni] && strcmp(snames[ni], ent->s[si].nm) == 0) {
                    ent->s[si].val = NV_GET_fn(ent->s[si].nm);
                    break;
                }
            }
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void init_save_frame(void) {
    if (frame_depth <= 0) return;
    GenFrame *f = &frame_stack[frame_depth - 1];
    for (int ei = 0; ei < init_n; ei++) {
        InitEnt *ent = &init_tab[ei];
        for (int si = 0; si < ent->ns; si++) {
            int slot = scope_get(&f->sc, ent->s[si].nm);
            if (slot >= 0 && slot < f->env_n) {
                ent->s[si].val = f->env[slot];
            } else {
                ent->s[si].val = NV_GET_fn(ent->s[si].nm);
            }
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void shadow_set_cur(const char *name, DESCR_t val) {
    if (call_depth <= 0) return;
    CallFrame *fr = &call_stack[call_depth - 1];
    for (int j = 0; j < fr->nshadow; j++)
        if (strcmp(fr->shadow[j].name, name) == 0) { fr->shadow[j].val = val; return; }
    if (fr->nshadow < SHADOW_MAX) {
        strncpy(fr->shadow[fr->nshadow].name, name, 63);
        fr->shadow[fr->nshadow].name[63] = '\0';
        fr->shadow[fr->nshadow].val = val;
        fr->nshadow++;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int shadow_has(const char *name) {
    for (int d = call_depth - 1; d >= 0; d--) {
        CallFrame *fr = &call_stack[d];
        for (int j = 0; j < fr->nshadow; j++)
            if (strcmp(fr->shadow[j].name, name) == 0) return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t call_user_function(const char *fname, DESCR_t *args, int nargs)
{
    if (call_depth >= CALL_STACK_MAX) return FAILDESCR;
    int np = FUNC_NPARAMS_fn(fname);
    int nl = FUNC_NLOCALS_fn(fname);
    char *pnames[64]; if (np > 64) np = 64;
    char *lnames[64]; if (nl > 64) nl = 64;
    for (int i = 0; i < np; i++) {
        const char *p = FUNC_PARAM_fn(fname, i);
        pnames[i] = p ? rt_ws_strdup(p) : rt_ws_strdup("");
    }
    for (int i = 0; i < nl; i++) {
        const char *l = FUNC_LOCAL_fn(fname, i);
        lnames[i] = l ? rt_ws_strdup(l) : rt_ws_strdup("");
    }
    char ufname[128];
    {
        size_t flen = strlen(fname);
        if (flen >= sizeof(ufname)) flen = sizeof(ufname)-1;
        for (size_t i = 0; i <= flen; i++) ufname[i] = fname[i];
    }
    const char *entry_pre = FUNC_ENTRY_fn(fname);
    const char *retname = fname;
    if (entry_pre && strcmp(entry_pre, fname) != 0 && FNCEX_fn(entry_pre))
        retname = entry_pre;
    comm_call(retname);
    monitor_quiet_depth++;
    int nsaved = 1 + np + nl;
    char   **snames = rt_ws_alloc((size_t)nsaved * sizeof(char *));
    DESCR_t *svals  = rt_ws_alloc((size_t)nsaved * sizeof(DESCR_t));
    snames[0] = rt_ws_strdup(retname);
    svals[0]  = NV_GET_fn(retname);
    NV_SET_fn(retname, STRVAL(""));
    for (int i = 0; i < np; i++) {
        snames[1+i] = pnames[i];
        if (strcmp(pnames[i], retname) == 0)
            svals[1+i] = svals[0];
        else
            svals[1+i] = NV_GET_fn(pnames[i]);
        NV_SET_fn(pnames[i], (i < nargs) ? args[i] : NULVCL);
    }
    for (int i = 0; i < nl; i++) {
        snames[1+np+i] = lnames[i];
        svals[1+np+i]  = NV_GET_fn(lnames[i]);
        NV_SET_fn(lnames[i], NULVCL);
    }
    monitor_quiet_depth--;
    CallFrame *fr = &call_stack[call_depth++];
    kw_fnclevel = call_depth;
    strncpy(fr->fname, retname, sizeof(fr->fname)-1);
    fr->fname[sizeof(fr->fname)-1] = '\0';
    fr->nshadow = 0;
    for (int i = 0; i < np; i++)
        if (_is_pat_fnc_name(pnames[i]))
            shadow_set_cur(pnames[i], (i < nargs) ? args[i] : NULVCL);
    for (int i = 0; i < nl; i++)
        if (_is_pat_fnc_name(lnames[i]))
            shadow_set_cur(lnames[i], NULVCL);
    fr->saved_names = snames;
    fr->saved_vals  = svals;
    fr->nsaved      = nsaved;
    fr->retval_cell = STRVAL("");
    fr->retval_set  = 0;
    DESCR_t retval = NULVCL;
    const char *saved_Σ    = Σ;
    int         saved_Δ    = Δ;
    int         saved_Ω    = Ω;
    int         saved_Σlen = Σlen;
    int ret_kind = setjmp(fr->ret_env);
    if (ret_kind == 0) {
        const char *entry = FUNC_ENTRY_fn(fname);
        const tree_t *body = entry ? label_lookup(entry) : NULL;
        if (!body) body = label_lookup(fname);
        if (!body) body = label_lookup(ufname);
        if (!body && entry && strcmp(entry, fname) != 0) {
            extern int try_call_builtin_by_name(const char *fn, DESCR_t *args, int nargs, DESCR_t *out);
            DESCR_t _bout;
            { extern int rt_g_want_name; extern int rt_dat_field_of_any(const char *);
              extern DESCR_t rt_field_var(const char *field, DESCR_t obj);
              if (rt_g_want_name && nargs >= 1 && args[0].v == DT_DATA && rt_dat_field_of_any(entry)) {
                  rt_g_want_name = 0; retval = rt_field_var(entry, args[0]); goto fn_done;
              } }
            if (try_call_builtin_by_name(entry, args, nargs, &_bout)) { retval = _bout; goto fn_done; }
        }
        if (!body && !FNCEX_fn(fname) && !FNCEX_fn(ufname)) {
            if (getenv("SCRIP_DEBUG_APPLY"))
                fprintf(stderr, "[call-err5] unresolved '%s' (ufname='%s', nargs=%d)\n", fname ? fname : "(null)", ufname ? ufname : "(null)", nargs);
            core_runtime_error(5, NULL);
            retval = FAILDESCR;
            goto fn_done;
        }
        retval = fr->retval_set ? fr->retval_cell : NV_GET_fn(fr->fname);
        strncpy(kw_rtntype, "RETURN",  sizeof(kw_rtntype)-1);
    } else if (ret_kind == 1) {
        retval = fr->retval_set ? fr->retval_cell : NV_GET_fn(fr->fname);
        strncpy(kw_rtntype, "RETURN",  sizeof(kw_rtntype)-1);
    } else {
        retval = FAILDESCR;
        strncpy(kw_rtntype, "FRETURN", sizeof(kw_rtntype)-1);
    }
fn_done:
    Σ    = saved_Σ;
    Δ    = saved_Δ;
    Ω    = saved_Ω;
    Σlen = saved_Σlen;
    comm_return(retname, retval);
    init_update_snapshot(snames, svals, nsaved);
    monitor_quiet_depth++;
    for (int i = 0; i < nsaved; i++)
        NV_SET_fn(snames[i], svals[i]);
    monitor_quiet_depth--;
    call_depth--;
    kw_fnclevel = call_depth;
    return retval;
}
