/* DEAD CODE — excised from src/emitter/emit_core.c
 * GC oracle (--gc-sections, 2026-06-14) confirmed all dead.
 *
 * emit_mode_set — emit-mode selector; superseded by emitter_init_text/binary;
 *                 called only from tools/emit_per_kind_audit.c (not in main build).
 * emitter_init_macro_def — macro-def init alias; only called from tools/demo_template_productions.c.
 * ef_u32, ef_u64 — u32/u64 emit wrappers; only callers were ef_t3c (dead).
 * ef_t3c — old 3-column asm formatter; superseded by x86() templates; zero live callers.
 * emit_call_label — PLT-relative call emitter; no live callers (x86("call",...) handles all cases).
 * emit_text_stno_banner — statement-number comment banner; no live callers.
 * emit_text_rawf — raw fprintf to emit sink; no live callers.
 * bb_is_generator, bb_walk_rec, bb_walk — IR-graph walkers; IR-walk at runtime forbidden (rule).
 */
void emit_mode_set(bb_emit_mode_t m, FILE *out) {
    bb_emit_mode = m; bb_emit_out = out;
    switch (m) {
    case EMIT_TEXT:         g_platform=BB_PLATFORM_X86;  g_medium=BB_MEDIUM_TEXT;      g_use_sm_macros=0; break;
    case EMIT_TEXT_INLINE:  g_platform=BB_PLATFORM_X86;  g_medium=BB_MEDIUM_TEXT;      g_use_sm_macros=1; break;
    case EMIT_MACRO_DEF:    g_platform=BB_PLATFORM_X86;  g_medium=BB_MEDIUM_MACRO_DEF; g_use_sm_macros=0; break;
    case EMIT_BINARY_WIRED: g_platform=BB_PLATFORM_X86;  g_medium=BB_MEDIUM_BINARY;    g_use_sm_macros=0; break;
    case EMIT_JVM:          g_platform=BB_PLATFORM_JVM;  g_medium=BB_MEDIUM_TEXT;      g_use_sm_macros=0; break;
    case EMIT_JS:           g_platform=BB_PLATFORM_JS;   g_medium=BB_MEDIUM_TEXT;      g_use_sm_macros=0; break;
    case EMIT_NET:          g_platform=BB_PLATFORM_NET;  g_medium=BB_MEDIUM_TEXT;      g_use_sm_macros=0; break;
    case EMIT_WASM:         g_platform=BB_PLATFORM_WASM; g_medium=BB_MEDIUM_TEXT;      g_use_sm_macros=0; break;
    }
}
void  emitter_init_macro_def(FILE *out) { emitter_init_text(out, TEXT_MODE_DEFINITION); }
static void ef_u32(uint32_t v) { bb_emit_u32(v); }
static void ef_u64(uint64_t v) { bb_emit_u64(v); }
static void ef_t3c(const char *mnem, const char *fmt, ...) {
    char buf[256]; buf[0] = '\0';
    if (fmt) { va_list ap; va_start(ap,fmt); vsnprintf(buf,sizeof(buf),fmt,ap); va_end(ap); }
    fprintf(bb_emit_out, "%s %s\n", mnem ? mnem : "", buf);
}
void emit_call_label(bb_label_t *target) {
    if (g_is_text) { ef_t3c_jmp("call", target->name); g_emit_pos += 5; }
    else { ef_b1(0xE8); bb_emit_patch_rel32(target); }
}
void emit_text_stno_banner(int stno, int lineno, const char *src_text) { (void)stno; (void)lineno; (void)src_text; }
void emit_text_rawf(const char *fmt, ...) { (void)fmt; }
int bb_is_generator(IR_e k) {
    if (k >= IR_PAT_LIT && k <= IR_PAT_DEFER) return 1;
    if (k >= IR_CHOICE  && k <= IR_GOAL)      return 1;
    if (k >= IR_TO      && k <= IR_PROC_GEN)  return 1;
    if (k == IR_SCAN || k == IR_TO_BY || k == IR_EVERY || k == IR_WHILE || k == IR_LIMIT || k == IR_SUSPEND) return 1;
    return 0;
}
static void bb_walk_rec(IR_t *nd, void (*visit)(IR_t *, void *), void *ctx) {
    if (!nd) return;
    int id = bb_node_id(nd);
    for (int i = 0; i < g_vcount; i++) if (g_visited[i] == id) return;
    if (g_vcount < IR_WALK_MAX) g_visited[g_vcount++] = id;
    visit(nd, ctx);
    if (nd->op != IR_SCAN) for (int i = 0; i < nd->n_operands; i++) bb_walk_rec(nd->operands[i], visit, ctx);
    bb_walk_rec(nd->γ.node, visit, ctx); bb_walk_rec(nd->ω.node, visit, ctx);
}
void bb_walk(IR_graph_t *cfg, void (*visit)(IR_t *, void *), void *ctx) {
    if (!cfg || !cfg->entry) return;
    g_vcount = 0;
    bb_walk_rec(cfg->entry, visit, ctx);
}
