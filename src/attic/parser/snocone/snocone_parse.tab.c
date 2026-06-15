/* ATTIC: dead code excised from src/parser/snocone/snocone_parse.tab.c (commit base 1308f79) — unreachable from main per --gc-sections oracle. Provenance only; not compiled. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *sc_label_new(ScParseState *st, const char *prefix) {
    static int global_label_seq = 0;
    char buf[64];
    (void)st;
    snprintf(buf, sizeof buf, "%s_%04d", prefix, ++global_label_seq);
    return strdup(buf);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static LoopFrame *sc_loop_find_innermost(ScParseState *st, int want_loop) {
    for (LoopFrame *f = st->loop_top; f; f = f->outer) {
        if (want_loop && !f->is_loop) continue;
        return f;
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* PST-SC-4f: no implicit break gotos in the pure syntax tree — lower handles fallthrough */
static void sc_switch_emit_implicit_break(ScParseState *st, struct SwitchHead *h) {
    (void)st; (void)h;
}
