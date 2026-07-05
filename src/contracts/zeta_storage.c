#include <string.h>
#include <stdlib.h>
#include "zeta_storage.h"
#include "zeta_choices.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern const char * bb_op_name(IR_e k);
extern int is_global(const char *);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define ZLS_MAX_ENTRIES 65536
#define ZLS_MAX_FIELDS  131072
#define ZLS_MAX_SCOPES  4096
#define ZLS_MAX_GRAPHS  256
#define ZLS_MAX_VSLOTS  4096
#define ZLS_MAX_MARKS   8192
typedef struct { const IR_t * nd; int scope_id; int off; } zls_entry_t;
typedef struct { int scope_id; int off; int size; unsigned char kind; unsigned char audit; const char * what; const IR_t * nd; } zls_pfield_t;
typedef struct { const char * name; int off; } zls_vslot_t;
typedef struct { const IR_graph_t * g; const char * name; int start_n; } zls_mark_t;
typedef struct { const IR_graph_t * g; const char * name; int first_scope; int n_scopes; int nslots; int region; int resume_off; int first_vslot; int n_vslots; } zls_graph_t;
static zls_entry_t  ze[ZLS_MAX_ENTRIES];  static int ze_n = 0;
static zls_pfield_t zf[ZLS_MAX_FIELDS];   static int zf_n = 0;
static zls_scope_t  zs[ZLS_MAX_SCOPES];   static int zs_n = 0;
static zls_graph_t  zg[ZLS_MAX_GRAPHS];   static int zg_n = 0;
static zls_vslot_t  zv[ZLS_MAX_VSLOTS];   static int zv_n = 0;
static zls_mark_t   zm[ZLS_MAX_MARKS];    static int zm_n = 0;
static zls_entry_t * zx[ZLS_MAX_ENTRIES]; static int zx_n = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void zls_reset(void) { ze_n = 0; zf_n = 0; zs_n = 0; zg_n = 0; zv_n = 0; zm_n = 0; zx_n = 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void zls_group_mark(const IR_graph_t * g, const char * name) {
    if (!g || !name) return;
    if (zm_n >= ZLS_MAX_MARKS) { fprintf(stderr, "zls: mark table overflow (%d)\n", ZLS_MAX_MARKS); abort(); }
    zm[zm_n++] = (zls_mark_t){ g, name, g->n };
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static zls_graph_t * zls_g_find(const IR_graph_t * g) { for (int i = 0; i < zg_n; i++) if (zg[i].g == g) return &zg[i]; return (zls_graph_t *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void zls_graph_name(const IR_graph_t * g, const char * name) {
    if (!g || !name) return;
    zls_graph_t * r = zls_g_find(g);
    if (r) { r->name = name; if (r->first_scope >= 0 && r->first_scope < zs_n) zs[r->first_scope].name = name; return; }
    if (zg_n >= ZLS_MAX_GRAPHS) { fprintf(stderr, "zls: graph table overflow (%d)\n", ZLS_MAX_GRAPHS); abort(); }
    zg[zg_n] = (zls_graph_t){ g, name, -1, 0, 0, 0, -1, -1, 0 };
    zg_n++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zx_cmp(const void * a, const void * b) { const zls_entry_t * x = *(zls_entry_t * const *)a; const zls_entry_t * y = *(zls_entry_t * const *)b; return (x->nd > y->nd) - (x->nd < y->nd); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const zls_entry_t * zx_find(const IR_t * nd) {
    int lo = 0, hi = zx_n - 1;
    while (lo <= hi) { int m = (lo + hi) / 2; if (zx[m]->nd == nd) return zx[m]; if (zx[m]->nd < nd) lo = m + 1; else hi = m - 1; }
    return (const zls_entry_t *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void zls_field(int scope_id, int off, int size, int kind, int audit, const char * what, const IR_t * nd) {
    if (zf_n >= ZLS_MAX_FIELDS) { fprintf(stderr, "zls: field table overflow (%d)\n", ZLS_MAX_FIELDS); abort(); }
    zf[zf_n++] = (zls_pfield_t){ scope_id, off, size, (unsigned char)kind, (unsigned char)audit, what, nd };
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void zls_entry(const IR_t * nd, int scope_id, int off) {
    if (ze_n >= ZLS_MAX_ENTRIES) { fprintf(stderr, "zls: entry table overflow (%d)\n", ZLS_MAX_ENTRIES); abort(); }
    ze[ze_n] = (zls_entry_t){ nd, scope_id, off };
    zx[zx_n++] = &ze[ze_n];
    ze_n++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zls_grant(const IR_t * nd, int scope_id, int off) {
    switch (nd->op) {
    case IR_TO: case IR_TO_BY:
        zls_entry(nd, scope_id, off); zls_field(scope_id, off, 16, ZK_DESCR, 0, "to.value", nd); zls_field(scope_id, off + 16, 8, ZK_RAW, 0, "to.I counter", nd); zls_field(scope_id, off + 24, 8, ZK_RAW, 0, "to.limit", nd); return 2;
    case IR_MAKE_LIST: {
        zls_entry(nd, scope_id, off); zls_field(scope_id, off, 16, ZK_DESCR, 0, "list.value", nd);
        for (int j = 0; j < nd->n_operands; j++) zls_field(scope_id, off + 16 * (1 + j), 16, ZK_DESCR, 0, "list.elem", nd);
        return 1 + nd->n_operands; }
    case IR_SCAN_ENTER:
        zls_entry(nd, scope_id, off); zls_field(scope_id, off, 8, ZK_RAW, 0, "scan.leave out3 sigma (transient reg out-area; dead at safe points)", nd); zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "scan.leave out3 delta", nd); zls_field(scope_id, off + 16, 8, ZK_RAW, 0, "scan.leave out3 Delta", nd); zls_field(scope_id, off + 24, 8, ZK_RAW, 0, "scan.pad (unused)", nd); return 2;
    case IR_MATCH_HEAD:
        zls_entry(nd, scope_id, off); zls_field(scope_id, off, 16, ZK_RAW, 0, "head.cursor", nd); return 1;
    case IR_MATCH_SPAN:
        zls_entry(nd, scope_id, off); zls_field(scope_id, off, 16, ZK_RAW, 0, "span.cnt/cur", nd); return 1;
    case IR_MATCH_BREAK: case IR_MATCH_BREAKX:
        zls_entry(nd, scope_id, off); zls_field(scope_id, off, 16, ZK_RAW, 0, "break.cnt/cur", nd); return 1;
    case IR_MATCH_ARB: case IR_MATCH_REM:
        zls_entry(nd, scope_id, off); zls_field(scope_id, off, 16, ZK_RAW, 0, "match.cursor save", nd); return 1;
    case IR_MATCH_ARBNO:
        if (nd->n_operands) return 0;
        zls_entry(nd, scope_id, off); zls_field(scope_id, off, 16, ZK_RAW, 0, "arbno.entry/yield/before cursors (3x4B + pad; phases 1/2 read via operand[0])", nd); return 1;
    case IR_MATCH_ASSIGN_SAVE:
        zls_entry(nd, scope_id, off); zls_field(scope_id, off, 8, ZK_PTR_GC, 0, "capture.stack GC_MALLOC_ATOMIC u32[] ([0]=cap, frames from [1]; box α-push/β-pop)", nd); zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "capture.stack gen(+8,4B)/sp(+12,4B)", nd); return 1;
    case IR_MATCH_ALTERNATE:
        zls_entry(nd, scope_id, off); zls_field(scope_id, off, 16, ZK_RAW, 0, "alt.cursor save", nd); return 1;
    case IR_SCAN_TAB: case IR_SCAN_MOVE:
        zls_entry(nd, scope_id, off); zls_field(scope_id, off, 16, ZK_DESCR, 0, "scan.value", nd); zls_field(scope_id, off + 16, 8, ZK_RAW, 0, "scan.r14 data-backtrack save", nd); zls_field(scope_id, off + 24, 8, ZK_RAW, 0, "scan.pad (unused)", nd); return 2;
    case IR_SCAN_UPTO: case IR_SCAN_FIND: case IR_SCAN_MATCH: case IR_SCAN_BAL:
        zls_entry(nd, scope_id, off); zls_field(scope_id, off, 16, ZK_DESCR, 0, "scan.value", nd); zls_field(scope_id, off + 16, 8, ZK_RAW, 0, "scan.cursor", nd); zls_field(scope_id, off + 24, 8, ZK_RAW, 0, "scan.len/counter", nd); return 2;
    case IR_INITIAL:
        zls_entry(nd, scope_id, off); zls_field(scope_id, off, 8, ZK_RAW, 0, "initial.pad (unused low half)", nd); zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "initial.once flag (0->1)", nd); return 1;
    case IR_ITERATE:
        zls_entry(nd, scope_id, off); zls_field(scope_id, off, 16, ZK_DESCR, 0, "iterate.value", nd); zls_field(scope_id, off + 16, 8, ZK_RAW, 0, "iterate.index i (alpha=0, beta inc)", nd); zls_field(scope_id, off + 24, 8, ZK_RAW, 0, "iterate.pad (unused)", nd); return 2;
    case IR_LIMIT:
        zls_entry(nd, scope_id, off); zls_field(scope_id, off, 16, ZK_DESCR, 0, "limit.value", nd); zls_field(scope_id, off + 16, 8, ZK_RAW, 0, "limit.counter", nd); zls_field(scope_id, off + 24, 8, ZK_RAW, 0, "limit.pad (unused)", nd); return 2;
    case IR_REPALT:
        zls_entry(nd, scope_id, off); zls_field(scope_id, off, 16, ZK_DESCR, 0, "repalt.value", nd); zls_field(scope_id, off + 16, 8, ZK_RAW, 0, "repalt.yielded flag (clear/yield/test)", nd); zls_field(scope_id, off + 24, 8, ZK_RAW, 0, "repalt.pad (unused)", nd); return 2;
    case IR_REV_ASSIGN: case IR_REV_ASSIGN_VAR:
        zls_entry(nd, scope_id, off); zls_field(scope_id, off, 16, ZK_DESCR, 0, "revasg.value", nd); zls_field(scope_id, off + 16, 16, ZK_DESCR, 0, "revasg.saved old value (beta restore; LIVE across suspension — GC must trace)", nd); return 2;
    case IR_KEYWORD_ICON: case IR_KEYWORD_ICON_GEN:
        zls_entry(nd, scope_id, off); zls_field(scope_id, off, 16, ZK_DESCR, 0, "kw.value", nd); zls_field(scope_id, off + 16, 16, ZK_RAW, 0, "kw.gen counter", nd); return 2;
    case IR_KEYWORD_SNOBOL4:
        zls_entry(nd, scope_id, off); zls_field(scope_id, off, 16, ZK_DESCR, 0, "kw.value", nd); return 1;
    case IR_DEREF: case IR_ASSIGN_VAR: case IR_RANDOM: case IR_SWAP_VAR:
        zls_entry(nd, scope_id, off); zls_field(scope_id, off, 16, ZK_DESCR, 0, "value", nd); return 1;
    case IR_KEYWORD_ASSIGN:
        zls_entry(nd, scope_id, off); zls_field(scope_id, off, 16, ZK_DESCR, 0, "kwset.value", nd); return 1;
    case IR_CREATE:
        zls_entry(nd, scope_id, off);
        zls_field(scope_id, off,      8, ZK_RAW, 0, "coexpr.handle ctx* (malloc'd non-GC — never trace/relocate)", nd);
        zls_field(scope_id, off + 8,  8, ZK_RAW, 0, "coexpr.handle pad (unwritten)", nd);
        zls_field(scope_id, off + 16, 8, ZK_RAW, 0, "coexpr.marshal r12 (copied out by scrip_coexpr_create; dead at return)", nd);
        zls_field(scope_id, off + 24, 8, ZK_RAW, 0, "coexpr.marshal r13", nd);
        zls_field(scope_id, off + 32, 8, ZK_RAW, 0, "coexpr.marshal r14", nd);
        zls_field(scope_id, off + 40, 8, ZK_RAW, 0, "coexpr.marshal r15", nd);
        zls_field(scope_id, off + 48, 8, ZK_RAW, 0, "coexpr.marshal rbx", nd);
        zls_field(scope_id, off + 56, 8, ZK_RAW, 0, "coexpr.marshal rbp", nd);
        return 4;
    case IR_ASSIGN:
        zls_entry(nd, scope_id, off); zls_field(scope_id, off, 16, ZK_DESCR, 0, "assign.value", nd); return 1;
    case IR_INDIRECT_GOTO: case IR_DISJUNCTION:
        zls_entry(nd, scope_id, off); zls_field(scope_id, off, 16, ZK_DESCR, 0, "gate.value", nd); zls_field(scope_id, off + 16, 8, ZK_PTR_CODE, 0, "gate.stored resume target", nd); zls_field(scope_id, off + 24, 8, ZK_RAW, 0, "gate.pad (unused)", nd); return 2;
    case IR_CALL_BUILTIN_GEN:
        zls_entry(nd, scope_id, off); zls_field(scope_id, off, 16, ZK_DESCR, 0, "call.value", nd);
        for (int j = 0; j < nd->n_operands; j++) zls_field(scope_id, off + 16 * (1 + j), 16, ZK_DESCR, 0, "call.argv", nd);
        zls_field(scope_id, off + 16 * (1 + nd->n_operands), 8, ZK_RAW, 0, "callgen.resume position (alpha=0, runtime writes next start)", nd);
        zls_field(scope_id, off + 16 * (1 + nd->n_operands) + 8, 8, ZK_RAW, 0, "callgen.pad (unused)", nd);
        return 2 + nd->n_operands;
    default:
        if (nd->op == IR_CALL || ir_is_call_kind(nd->op)) {
            zls_entry(nd, scope_id, off); zls_field(scope_id, off, 16, ZK_DESCR, 0, "call.value", nd);
            for (int j = 0; j < nd->n_operands; j++) zls_field(scope_id, off + 16 * (1 + j), 16, ZK_DESCR, 0, "call.argv", nd);
            return 1 + nd->n_operands;
        }
        if (ir_node_produces_value(nd->op)) { zls_entry(nd, scope_id, off); zls_field(scope_id, off, 16, ZK_DESCR, 0, "value", nd); return 1; }
        return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zls_scope_new(int parent, int klass, const char * name) {
    if (zs_n >= ZLS_MAX_SCOPES) { fprintf(stderr, "zls: scope table overflow (%d)\n", ZLS_MAX_SCOPES); abort(); }
    zs[zs_n] = (zls_scope_t){ zs_n, parent, klass, name, -1, 0, 0x7fffffff, 0 };
    return zs_n++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void zls_build(IR_graph_t * g) {
    if (!g) return;
    zls_graph_t * r = zls_g_find(g);
    if (r && r->first_scope >= 0) return;
    if (!r) {
        if (zg_n >= ZLS_MAX_GRAPHS) { fprintf(stderr, "zls: graph table overflow (%d)\n", ZLS_MAX_GRAPHS); abort(); }
        zg[zg_n] = (zls_graph_t){ g, (const char *)0, -1, 0, 0, 0, -1, -1, 0 }; r = &zg[zg_n]; zg_n++;
    }
    static char anon[ZLS_MAX_GRAPHS][8]; int gi = (int)(r - zg);
    if (!r->name) { snprintf(anon[gi], sizeof anon[gi], "g%d", gi); r->name = anon[gi]; }
    int root = zls_scope_new(-1, ZSC_FN, r->name);
    r->first_scope = root; r->n_scopes = 1;
    int mfirst[ZLS_MAX_SCOPES]; int mstart[ZLS_MAX_SCOPES]; int nl = 0;
    for (int mi = 0; mi < zm_n; mi++) {
        if (zm[mi].g != g) continue;
        int sc = zls_scope_new(root, ZSC_GROUP, zm[mi].name);
        mfirst[nl] = sc; mstart[nl] = zm[mi].start_n; nl++;
        r->n_scopes++;
    }
    int base = 16 + (g->nparams > 0 ? g->nparams * 16 : 0);
    int k = 0;
    r->first_vslot = zv_n;
    for (int i = 0; i < g->nparams && g->pnames; i++) if (g->pnames[i]) {
        if (zv_n >= ZLS_MAX_VSLOTS) { fprintf(stderr, "zls: vslot table overflow (%d)\n", ZLS_MAX_VSLOTS); abort(); }
        zv[zv_n++] = (zls_vslot_t){ g->pnames[i], 16 + i * 16 }; r->n_vslots++;
        zls_field(root, 16 + i * 16, 16, ZK_DESCR, 0, "param", (const IR_t *)0);
    }
    int cur = 0;
    for (int i = 0; i < g->n; i++) {
        IR_t * nd = g->all[i];
        if (!nd) continue;
        while (cur < nl && i >= mstart[cur]) cur++;
        int sc = (cur > 0) ? mfirst[cur - 1] : root;
        k += zls_grant(nd, sc, base + k * 16);
    }
    r->resume_off = -1;
    for (int i = 0; i < g->n; i++) if (g->all[i] && g->all[i]->op == IR_SUSPEND) {
        r->resume_off = base + k * 16;
        zls_field(root, r->resume_off, 8, ZK_PTR_CODE, 0, "gen-proc resume continuation", (const IR_t *)0);
        zls_field(root, r->resume_off + 8, 8, ZK_RAW, 0, "resume.pad (unused)", (const IR_t *)0);
        k += 1; break;
    }
    for (int i = 0; i < g->n; i++) {
        IR_t * nd = g->all[i];
        if (!nd) continue;
        const char * vn = (const char *)0;
        if (nd->op == IR_ASSIGN) vn = IR_LIT(nd).sval;
        else if (nd->op == IR_REV_ASSIGN && nd->n_operands > 1 && nd->operands[1]) vn = IR_LIT(nd->operands[1]).sval;
        else if (nd->op == IR_VAR || nd->op == IR_VAR_REF) vn = IR_LIT(nd).sval;
        if (!vn || vn[0] == '&' || is_global(vn)) continue;
        int have = 0; for (int v = r->first_vslot; v < r->first_vslot + r->n_vslots; v++) if (zv[v].name && strcmp(zv[v].name, vn) == 0) { have = 1; break; }
        if (have) continue;
        if (zv_n >= ZLS_MAX_VSLOTS) { fprintf(stderr, "zls: vslot table overflow (%d)\n", ZLS_MAX_VSLOTS); abort(); }
        zv[zv_n++] = (zls_vslot_t){ vn, base + k * 16 }; r->n_vslots++;
        zls_field(root, base + k * 16, 16, ZK_DESCR, 0, "local", (const IR_t *)0);
        k++;
    }
    r->nslots = k;
    r->region = base + k * 16;
    for (int f = 0; f < zf_n; f++) {
        int sid = zf[f].scope_id;
        if (sid < root || sid >= zs_n) continue;
        if (zf[f].off < zs[sid].lo_off) zs[sid].lo_off = zf[f].off;
        if (zf[f].off + zf[f].size > zs[sid].hi_off) zs[sid].hi_off = zf[f].off + zf[f].size;
        if (zs[sid].first_field < 0) zs[sid].first_field = f;
        zs[sid].n_fields++;
    }
    qsort(zx, zx_n, sizeof(zls_entry_t *), zx_cmp);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int zls_off(const IR_t * nd) { const zls_entry_t * e = zx_find(nd); return e ? e->off : -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int zls_scope_of(const IR_t * nd) { const zls_entry_t * e = zx_find(nd); return e ? e->scope_id : -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int zls_g_nslots(const IR_graph_t * g) { zls_graph_t * r = zls_g_find(g); return r ? r->nslots : -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int zls_g_region(const IR_graph_t * g) { zls_graph_t * r = zls_g_find(g); return r ? r->region : -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int zls_g_resume(const IR_graph_t * g) { zls_graph_t * r = zls_g_find(g); return r ? r->resume_off : -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int zls_g_vslot_count(const IR_graph_t * g) { zls_graph_t * r = zls_g_find(g); return r ? r->n_vslots : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * zls_g_vslot_get(const IR_graph_t * g, int i, int * off) {
    zls_graph_t * r = zls_g_find(g);
    if (!r || i < 0 || i >= r->n_vslots) { if (off) *off = -1; return (const char *)0; }
    if (off) *off = zv[r->first_vslot + i].off;
    return zv[r->first_vslot + i].name;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * zk_name(int k) { return k == ZK_DESCR ? "DESCR" : k == ZK_RAW ? "RAW" : k == ZK_PTR_GC ? "PTR_GC" : k == ZK_PTR_CODE ? "PTR_CODE" : "?"; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * zsc_name(int k) { return k == ZSC_FN ? "FN" : k == ZSC_GROUP ? "GROUP" : k == ZSC_ITER ? "ITER" : k == ZSC_PAT ? "PAT" : k == ZSC_COEXPR ? "COEXPR" : "?"; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void zls_dump(FILE * fp) {
    static const char * alloc_names[]    = { "BUMP_INFINITE", "BUMP_LIFO", "MALLOC", "GC" };
    static const char * col_names[]      = { "MALLOC", "ARENA", "GC" };
    static const char * selfload_names[] = { "OFF", "PLANE_CELL", "ASSERT", "STATIC" };
    static const char * init_names[]     = { "ZERO", "NONE", "CLONE" };
    fprintf(fp, "; ZETA LOCAL STORAGE (ZLS layout, ZB-2) — choices: ALLOC=%s COLLECTION=%s SELFLOAD=%s INIT=%s POISON=%s TELEM=%s OVF=%s ARENA_MB=%d PROMOTE=%s\n",
        alloc_names[ZC_ALLOC], col_names[ZC_COLLECTION], selfload_names[ZC_SELFLOAD], init_names[ZC_INIT],
        ZC_POISON == ZC_POISON_FILL ? "FILL" : "OFF", ZC_TELEMETRY == ZC_TELEM_ON ? "ON" : "OFF", ZC_OVERFLOW == ZC_OVF_BOMB ? "BOMB" : "GUARD", (int)ZC_ARENA_MB, ZC_PROMOTE == ZC_PROMOTE_GATE ? "GATE" : "ON");
    fprintf(fp, "; kinds: DESCR = 16B t.p pair (GC traces payload) | RAW = int/cursor/counter (GC skips) | PTR_GC = heap pointer (GC traces+fixes) | PTR_CODE = continuation (GC skips, never relocates)\n");
    fprintf(fp, "; (audit) = kind provisional pending template audit — 2026-07-05 burndown: all shipped grants template-verified, audit=0; any NEW grant lands audit=1 until verified\n");
    for (int i = 0; i < zg_n; i++) {
        zls_graph_t * r = &zg[i];
        if (r->first_scope < 0) continue;
        fprintf(fp, "; graph %d '%s' — slots=%d region_end=%d resume=%d vslots=%d scopes=%d\n", i, r->name ? r->name : "?", r->nslots, r->region, r->resume_off, r->n_vslots, r->n_scopes);
        for (int s = r->first_scope; s < r->first_scope + r->n_scopes; s++) {
            if (zs[s].n_fields == 0 && zs[s].klass == ZSC_GROUP) { fprintf(fp, ";   scope %-3d %-6s %-24s parent=%-3d (no ZLS fields)\n", zs[s].id, zsc_name(zs[s].klass), zs[s].name ? zs[s].name : "?", zs[s].parent); continue; }
            fprintf(fp, ";   scope %-3d %-6s %-24s parent=%-3d [%d..%d)\n", zs[s].id, zsc_name(zs[s].klass), zs[s].name ? zs[s].name : "?", zs[s].parent, zs[s].n_fields ? zs[s].lo_off : 0, zs[s].n_fields ? zs[s].hi_off : 0);
            for (int f = 0; f < zf_n; f++) if (zf[f].scope_id == s) {
                const char * on = "-"; char onb[16];
                if (zf[f].nd) { on = bb_op_name(zf[f].nd->op); if (!on) { snprintf(onb, sizeof onb, "op%d", (int)zf[f].nd->op); on = onb; } }
                fprintf(fp, ";     +%-5d %-3d %-8s %-36s %s%s\n", zf[f].off, zf[f].size, zk_name(zf[f].kind), zf[f].what ? zf[f].what : "", on, zf[f].audit ? "  (audit)" : "");
            }
        }
        for (int v = r->first_vslot; v < r->first_vslot + r->n_vslots; v++) fprintf(fp, ";   vslot +%-5d 16  DESCR    %s\n", zv[v].off, zv[v].name ? zv[v].name : "?");
    }
}
