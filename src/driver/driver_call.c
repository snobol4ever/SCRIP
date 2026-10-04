#include "rt/rt_arena.h"
#include "driver_private.h"
cv_t       call_stack_v;
int        call_depth = 0;
cv_t       init_tab_v;
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void shadow_set_cur(const char *name, DESCR_t val) {
    if (call_depth <= 0) return;
    CallFrame *fr = &call_stack[call_depth - 1];
    for (int j = 0; j < fr->nshadow; j++)
        if (strcmp(fr->shadow[j].name, name) == 0) { fr->shadow[j].val = val; return; }
    { if (fr->nshadow >= fr->shadow_cap) { int nc = fr->shadow_cap ? fr->shadow_cap * 2 : 8; fr->shadow = (ShadowEntry *)ct_grow(fr->shadow, (size_t)nc * sizeof(ShadowEntry));
            memset(fr->shadow + fr->shadow_cap, 0, (size_t)(nc - fr->shadow_cap) * sizeof(ShadowEntry)); fr->shadow_cap = nc; }
        ShadowEntry *se = &fr->shadow[fr->nshadow]; size_t nl = strlen(name) + 1;
        if (se->ncap < nl) { size_t nc = se->ncap * 2 > nl ? se->ncap * 2 : nl; se->name = (char *)ct_grow(se->name, nc); se->ncap = nc; }
        memcpy(se->name, name, nl);
        se->val = val;
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
    int np = FUNC_NPARAMS_fn(fname);
    int nl = FUNC_NLOCALS_fn(fname);
    char *pnames[np > 0 ? np : 1];
    char *lnames[nl > 0 ? nl : 1];
    for (int i = 0; i < np; i++) {
        const char *p = FUNC_PARAM_fn(fname, i);
        pnames[i] = p ? rt_heap_strdup_c(p) : rt_heap_strdup_c("");
    }
    for (int i = 0; i < nl; i++) {
        const char *l = FUNC_LOCAL_fn(fname, i);
        lnames[i] = l ? rt_heap_strdup_c(l) : rt_heap_strdup_c("");
    }
    char ufname[strlen(fname) + 1];
    memcpy(ufname, fname, sizeof ufname);
    const char *entry_pre = FUNC_ENTRY_fn(fname);
    const char *retname = fname;
    if (entry_pre && strcmp(entry_pre, fname) != 0 && FNCEX_fn(entry_pre))
        retname = entry_pre;
    comm_call(retname);
    monitor_quiet_depth++;
    int nsaved = 1 + np + nl;
    extern void *rt_ws_alloc_descr(size_t);
    char   **snames = rt_pvec_alloc((size_t)nsaved);
    DESCR_t *svals  = rt_ws_alloc_descr((size_t)nsaved);
    snames[0] = rt_heap_strdup_c(retname);
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
    cv_reserve(&call_stack_v, (uint32_t)sizeof(CallFrame), (uint64_t)call_depth + 1, "call_stack"); int fi = call_depth++;
    kw_fnclevel = call_depth;
    { CallFrame *cf = &call_stack[fi]; size_t rl = strlen(retname) + 1;
      if (cf->fname_cap < rl) { size_t nc = cf->fname_cap * 2 > rl ? cf->fname_cap * 2 : rl; cf->fname = (char *)ct_grow(cf->fname, nc); cf->fname_cap = nc; } memcpy(cf->fname, retname, rl); }
    call_stack[fi].nshadow = 0;
    for (int i = 0; i < np; i++)
        if (_is_pat_fnc_name(pnames[i]))
            shadow_set_cur(pnames[i], (i < nargs) ? args[i] : NULVCL);
    for (int i = 0; i < nl; i++)
        if (_is_pat_fnc_name(lnames[i]))
            shadow_set_cur(lnames[i], NULVCL);
    call_stack[fi].saved_names = snames;
    call_stack[fi].saved_vals  = svals;
    call_stack[fi].nsaved      = nsaved;
    call_stack[fi].retval_cell = STRVAL("");
    call_stack[fi].retval_set  = 0;
    DESCR_t retval = NULVCL;
    const char *saved_Σ    = Σ;
    int         saved_Δ    = Δ;
    int         saved_Ω    = Ω;
    int         saved_Σlen = Σlen;
    int ret_kind = setjmp(call_stack[fi].ret_env);
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
            { extern int rt_dat_field_of_any(const char *); extern DESCR_t c_dat_field_get(const char *fname, DESCR_t obj);
              const char *_fld = (entry && rt_dat_field_of_any(entry)) ? entry : ((fname && rt_dat_field_of_any(fname)) ? fname : (const char *)0);
              if (_fld && nargs < 1) { retval = c_dat_field_get(_fld, NULVCL); goto fn_done; } }
            if (getenv("SCRIP_DEBUG_APPLY"))
                fprintf(stderr, "[call-err5] unresolved '%s' (ufname='%s', nargs=%d)\n", fname ? fname : "(null)", ufname ? ufname : "(null)", nargs);
            core_runtime_error(22, "undefined function called");
            retval = FAILDESCR;
            goto fn_done;
        }
        retval = call_stack[fi].retval_set ? call_stack[fi].retval_cell : NV_GET_fn(call_stack[fi].fname);
        strncpy(kw_rtntype, "RETURN",  sizeof(kw_rtntype)-1);
    } else if (ret_kind == 1) {
        retval = call_stack[fi].retval_set ? call_stack[fi].retval_cell : NV_GET_fn(call_stack[fi].fname);
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
