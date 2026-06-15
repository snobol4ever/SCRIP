/* ATTIC: dead code excised from src/emitter/emit_bb.c (commit base 2a35216) — unreachable from main per --gc-sections oracle. Provenance only; not compiled. */
int bb_slot_alloc32(IR_t *nd) {
    int off = g_flat_slot_count;
    g_flat_slot_count += 32;
    if (g_bb_slotmap_n < BB_SLOTMAP_MAX) { g_bb_slotmap[g_bb_slotmap_n].key = nd; g_bb_slotmap[g_bb_slotmap_n].off = off; g_bb_slotmap_n++; }
    return off;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void data_buf_three_col(const char *lbl, const char *act, const char *got) {
    const char *L = lbl ? lbl : "";
    const char *A = act ? act : "";
    const char *G = got ? got : "";
    if (g_flat_data_pending_lbl[0]) {
        data_buf_appendf("%s\n", g_flat_data_pending_lbl);
        g_flat_data_pending_lbl[0] = '\0';
    }
    char line[576]; int o = 0; int first = 1;
    if (*L) { o += snprintf(line+o, sizeof(line)-o, "%s", L); first = 0; }
    if (*A) { o += snprintf(line+o, sizeof(line)-o, "%s%s", first?"":" ", A); first = 0; }
    if (*G) { o += snprintf(line+o, sizeof(line)-o, "%s%s", first?"":" ", G); }
    data_buf_appendf("%s\n", line);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void data_buf_pend_label(const char *name) {
    if (g_flat_data_pending_lbl[0])
        data_buf_appendf("%s\n", g_flat_data_pending_lbl);
    snprintf(g_flat_data_pending_lbl, sizeof(g_flat_data_pending_lbl), "%s:", name ? name : "");
}
void lower_flat_set_intern_str(const char *(*fn)(const char *)) { g_flat_intern_str = fn; }
/*--------------------------------------------------------------------------------------------------------------------*/
static void data_buf_remember_label(const char *name) {
    if (g_flat_data_block_nlbls >= FLAT_DATA_LBL_MAX) return;
    snprintf(g_flat_data_block_lbls[g_flat_data_block_nlbls], sizeof(g_flat_data_block_lbls[0]), "%s", name ? name : "");
    g_flat_data_block_nlbls++;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void data_buf_emit_block_comment(void) { g_flat_data_block_nlbls = 0; }
/*--------------------------------------------------------------------------------------------------------------------*/
void resolve_call_block_label(char *dst, size_t dsz, const char *name, int arity) {
    char san[128]; size_t j = 0;
    size_t namelen = 0;
    if (name) { const char *slash = strrchr(name, '/'); namelen = slash ? (size_t)(slash - name) : strlen(name); }
    for (size_t i = 0; name && i < namelen && j + 1 < sizeof san; i++) {
        char c = name[i];
        san[j++] = ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_') ? c : '_';
    }
    san[j] = '\0';
    snprintf(dst, dsz, ".Lplpred_%s_%d", san, arity);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void sub_label(char *dst, size_t dsz, const char *name) {
    char san[128]; size_t j = 0;
    for (size_t i = 0; name && name[i] && j + 1 < sizeof san; i++) {
        char c = name[i];
        san[j++] = ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_') ? c : '_';
    }
    san[j] = '\0';
    snprintf(dst, dsz, ".Lrksub_%s", san);
}
/*--------------------------------------------------------------------------------------------------------------------*/
int resolve_emit_callee_block_body(const char *name, int arity, bb_label_t *bγ, bb_label_t *bω, bb_label_t *bβ) {
    IR_t *pentry = resolve_bb_entry_node(name, arity);
    if (!pentry) return 0;
    walk_bb_flat(pentry, bγ, bω, bβ);
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void bb_prepare_capture_arbno(IR_t *nd, int imm) {
    if (!PLATFORM_X86) return;
    bb_box_fn   child_fn = (bb_box_fn)g_emit.child_fn;
    const char *name     = g_emit.op_name1;
    const char *fnc_name = (g_emit.op_name2 && g_emit.op_name2[0]) ? g_emit.op_name2 : NULL;
    g_emit.bb_rt_obj   = NULL;
    g_emit.bb_child_lbl = NULL;
    g_emit.bb_child_fn  = (void *)child_fn;
    if (nd->op == IR_PAT_ARBNO) {
        g_emit.bb_rt_obj = NULL;
        if (MEDIUM_TEXT) {
            emit_comment("# BOX ARBNO()");
            xa_dispatch(XA_BB_PTR_SLOT);
            g_emit.bb_child_lbl = child_fn ? child_cache_get_lbl(child_fn) : NULL;
            if (g_emit.bb_child_lbl && g_cap_fixup_cb) {
                char buf[256];
                snprintf(buf, sizeof(buf), "%s|%s", g_emit.bb_ptr_slot_lbl, g_emit.bb_child_lbl);
                g_cap_fixup_cb((void*)2, buf);
            }
        }
        return;
    }
    if (!name || name[0] == 0) return;
    if (fnc_name) {
        g_emit.bb_rt_obj = bb_cap_new_call(child_fn, NULL, fnc_name, NULL, 0, NULL, 0, 0);
        if (MEDIUM_TEXT) {
            char banner[320];
            snprintf(banner, sizeof(banner), "# BOX CALLCAP(%s)", fnc_name);
            emit_comment(banner);
            xa_dispatch(XA_BB_PTR_SLOT);
            g_emit.bb_child_lbl = child_fn ? child_cache_get_lbl(child_fn) : NULL;
            if (g_emit.bb_child_lbl && g_cap_fixup_cb) {
                char buf[512];
                snprintf(buf, sizeof(buf), "%s|%s|%s|0|1", g_emit.bb_ptr_slot_lbl, g_emit.bb_child_lbl, fnc_name);
                g_cap_fixup_cb((void*)1, buf);
            }
        }
        return;
    }
    g_emit.bb_rt_obj = bb_cap_new(child_fn, NULL, name, NULL, imm);
    if (MEDIUM_TEXT) {
        char banner[320];
        snprintf(banner, sizeof(banner), "# BOX %s(%s)", imm ? "CAP_IMM" : "CAP_COND", name);
        emit_comment(banner);
        xa_dispatch(XA_BB_PTR_SLOT);
        g_emit.bb_child_lbl = child_fn ? child_cache_get_lbl(child_fn) : NULL;
        if (g_emit.bb_child_lbl && g_cap_fixup_cb) {
            char buf[512];
            snprintf(buf, sizeof(buf), "%s|%s|%s|%s|0", g_emit.bb_ptr_slot_lbl, g_emit.bb_child_lbl, name, imm ? "1" : "0");
            g_cap_fixup_cb((void*)1, buf);
        }
    }
}
int pl_catch_block_index(IR_t *nd) {
    for (int i = 0; i < g_pl_catch_n; i++) if (g_pl_catch_nodes[i] == nd) return i;
    return -1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void walk_bb_register_child_label(IR_t *nd, const char *α_label) {
    bb_box_fn fn = child_cache_get(nd);
    if (fn) child_cache_set_lbl(fn, α_label);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void lower_flat_reset(void) { g_flat_slot_count = 0; g_flat_node_id = 0; }
/*--------------------------------------------------------------------------------------------------------------------*/
/* ATTIC: dead-code sweep batch 4 (origin src/emitter/emit_bb.c, base 4c9b6bd) — lower_flat_set_cap_fixup: setter for g_cap_fixup_cb, never called; macro alias bb_flat_set_cap_fixup_cb never invoked. GC-oracle dead. Provenance only; not compiled. */
void lower_flat_set_cap_fixup(void (*cb)(void *cap_ptr, const char *child_α_label)) { g_cap_fixup_cb = cb; }
