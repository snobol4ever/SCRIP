/* ATTIC: dead code excised from src/driver/interp_call.c (commit base 2a35216) — unreachable from main per --gc-sections oracle. Provenance only; not compiled. */
/*--------------------------------------------------------------------------------------------------------------------*/
int shadow_get(const char *name, DESCR_t *out) {
    for (int d = call_depth - 1; d >= 0; d--) {
        CallFrame *fr = &call_stack[d];
        for (int j = 0; j < fr->nshadow; j++)
            if (strcmp(fr->shadow[j].name, name) == 0) { *out = fr->shadow[j].val; return 1; }
    }
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int is_current_frame_local(const char *name) {
    if (call_depth <= 0 || !name) return 0;
    CallFrame *fr = &call_stack[call_depth - 1];
    if (!fr->saved_names) return 0;
    for (int i = 0; i < fr->nsaved; i++) {
        if (fr->saved_names[i] && strcmp(fr->saved_names[i], name) == 0)
            return 1;
    }
    return 0;
}
