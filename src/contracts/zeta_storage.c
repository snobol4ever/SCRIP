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
typedef struct { const IR_graph_t * g; const char * name; int first_scope; int n_scopes; int nslots; int region; int resume_off; int zeta_mark_off; int first_vslot; int n_vslots; } zls_graph_t;
static zls_entry_t  ze[ZLS_MAX_ENTRIES];  static int ze_n = 0;
static zls_pfield_t zf[ZLS_MAX_FIELDS];   static int zf_n = 0;
static zls_scope_t  zs[ZLS_MAX_SCOPES];   static int zs_n = 0;
static zls_graph_t  zg[ZLS_MAX_GRAPHS];   static int zg_n = 0;
static zls_vslot_t  zv[ZLS_MAX_VSLOTS];   static int zv_n = 0;
static zls_mark_t   zm[ZLS_MAX_MARKS];    static int zm_n = 0;
static zls_entry_t * zx[ZLS_MAX_ENTRIES]; static int zx_n = 0;
typedef struct { const IR_t * nd; int min_off; int span; } zls_ageom_t;
static zls_ageom_t  za[1024];             static int za_n = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void zls_reset(void) { ze_n = 0; zf_n = 0; zs_n = 0; zg_n = 0; zv_n = 0; zm_n = 0; zx_n = 0; za_n = 0; }
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
        /* BB-OWNED-ζ statement-scope pivot (this session): the existing 16B grant only ever used its first
         * 4 bytes (head.cursor, an int32 counter read/written as FR(op_off) by bb_match_head.cpp) -- 12 bytes
         * of padding sat unused.  Reusing 8 of those bytes (off+8) for the saved rt_zls_mark() pointer needs
         * NO change to the slot count (still "return 1", still one 16B quad) and therefore cannot shift any
         * later node's offset -- the safest possible way to add a field.  ZK_PTR_GC to match the precedent
         * set by IR_MATCH_ARBNO's own COLLECTION pointer (arbno2.COLLECTION ptr below): both are pointers
         * into the SAME zeta arena, whose address-range rooting (rt_zls_alloc's GC_add_roots widening,
         * zeta_alloc.c) already covers them regardless of tag, but the tag is recorded honestly for
         * whichever future consumer reads it, not left as an untagged raw word next to a tagged sibling. */
        zls_entry(nd, scope_id, off); zls_field(scope_id, off, 4, ZK_RAW, 0, "head.cursor", nd); zls_field(scope_id, off + 8, 8, ZK_PTR_GC, 0, "head.zeta_mark (BB-OWNED-zeta statement-scope saved rt_zls_mark() pointer)", nd); zls_field(scope_id, off + 16, 8, ZK_PTR_GC, 0, "head.zls2_mark (ZC_PORT_ALLOC only: saved rt_zls2_mark() cursor; released by head's own omega-choke on failure / IR_MATCH_RELEASE on success — the ZLS2 twin of head.zeta_mark, widened to a second quad because the first quad's padding is spent)", nd); zls_field(scope_id, off + 24, 8, ZK_RAW, 0, "head.pad (unused)", nd); return 2;
    case IR_MATCH_SPAN:
        zls_entry(nd, scope_id, off); zls_field(scope_id, off, 16, ZK_RAW, 0, "span.cnt/cur", nd); return 1;
    case IR_MATCH_BREAK: case IR_MATCH_BREAKX:
        zls_entry(nd, scope_id, off); zls_field(scope_id, off, 16, ZK_RAW, 0, "break.cnt/cur", nd); return 1;
    case IR_MATCH_ARB:
        /* ZLS2 second consumer (Claude Sonnet 5, 2026-07-08) -- the "natural sibling" GOAL-SNOBOL4-BB.md names
         * for extending BB-OWNED-zeta past ARBNO. ARB's existing 16B grant already carried 8B of unused pad
         * (bytes 0-3 = matched-length counter, 4-7 = saved start position, 8-15 = pad) -- the SAME shape
         * IR_MATCH_HEAD's zeta_mark reuse found above, so the ZLS2 activation-block save-slot fits in the
         * EXISTING quad with zero widening: no later node's offset shifts, the safest possible way to add the
         * field (mirrors head.zeta_mark, not ARBNO's wider 2-quad case -- ARB only ever used 8B of its 16B and
         * already had exactly 8B of pad to spend). */
        zls_entry(nd, scope_id, off); zls_field(scope_id, off, 8, ZK_RAW, 0, "arb.cnt/cur (matched-length +0 4B, saved-start +4 4B)", nd); zls_field(scope_id, off + 8, 8, ZK_PTR_GC, 0, "arb.zls2 activation block ptr (save-slot-in-frame, ZC_PORT_ALLOC only: reuses this node's existing pad, same reuse precedent as IR_MATCH_HEAD.zeta_mark; block itself is a separate ZLS2 allocation, header +0 chains the previous activation's ptr)", nd); return 1;
    case IR_MATCH_REM:
        zls_entry(nd, scope_id, off); zls_field(scope_id, off, 16, ZK_RAW, 0, "match.cursor save", nd); return 1;
    case IR_MATCH_ARBNO:
        if (IR_LIT(nd).ival == 0) { zls_entry(nd, scope_id, off); zls_field(scope_id, off, 16, ZK_RAW, 0, "arbno.entry/yield/before cursors (3x4B + pad; phases 1/2 read via operand[0]; ZC_PORT_PLAIN state home — under ZC_PORT_ALLOC state moves into the per-activation ZLS2 block and this quad idles)", nd); zls_field(scope_id, off + 16, 8, ZK_PTR_GC, 0, "arbno.zls2 activation block ptr (save-slot-in-frame, ZC_PORT_ALLOC only: current activation's ZLS2 block; block header +0 chains the previous activation's ptr, popped back here at role 2's true exit — the MATCH_HEAD zeta_mark precedent, widened to its own quad because v1's first quad has only 4B of pad)", nd); zls_field(scope_id, off + 24, 8, ZK_RAW, 0, "arbno.pad (unused)", nd); return 2; }
        if (IR_LIT(nd).ival == 3) { zls_entry(nd, scope_id, off); zls_field(scope_id, off, 16, ZK_RAW, 0, "arbno2.owner quad low: entry/yield/i/cap (4x4B; phases 4/5 read via operand[0])", nd); zls_field(scope_id, off + 16, 8, ZK_PTR_GC, 0, "arbno2.COLLECTION ptr (rt_zcol_push-grown per-iteration elements: 16B header {prev_rZ, cur_before} + body-subgraph slot range)", nd); zls_field(scope_id, off + 24, 8, ZK_RAW, 0, "arbno2.pad (unused)", nd); return 2; }
        return 0;
    case IR_MATCH_ASSIGN_SAVE:
        zls_entry(nd, scope_id, off); zls_field(scope_id, off, 8, ZK_PTR_GC, 0, "capture.stack GC_MALLOC_ATOMIC u32[] ([0]=cap, frames from [1]; box α-push/β-pop)", nd); zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "capture.stack gen(+8,4B)/sp(+12,4B)", nd); return 1;
    case IR_MATCH_ALTERNATE:
        zls_entry(nd, scope_id, off); zls_field(scope_id, off, 8, ZK_RAW, 0, "alt.entry cursor save (+0 4B, +4 pad)", nd); zls_field(scope_id, off + 8, 8, ZK_PTR_CODE, 0, "alt.resume continuation (&JOIN reload arm; SAVE.β dispatches jmp [slot+8] — replay of the succeeded alternative's forward-fail)", nd); return 1;
    case IR_SCAN:
        zls_entry(nd, scope_id, off); zls_field(scope_id, off, 16, ZK_DESCR, 0, "scan.value (? body result carried out of the scan env for an outer consumer)", nd); return 1;
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
    r->zeta_mark_off = base + k * 16;
    zls_field(root, r->zeta_mark_off, 8, ZK_RAW, 0, "graph-scope zeta mark (rt_zls_mark snapshot, prologue-stashed, epilogue-released)", (const IR_t *)0);
    zls_field(root, r->zeta_mark_off + 8, 8, ZK_RAW, 0, "zeta_mark.pad (unused)", (const IR_t *)0);
    k += 1;
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
    /* ARBNO v2 (ZB-5) COLLECTION geometry: a phase-3 owner brackets its body subgraph with operands[0]=first
     * and operands[1]=last body node BY ALLOCATION (the optimizer never reorders/compacts g->all, so the
     * index window is exact and its grants are CONTIGUOUS — offsets are handed out in node order).  Element
     * layout = 16B header {prev_rZ, cur_before} + the window's slot range; the emitter repoints rZ to
     * elem+16-min_off so body boxes' [r12+off] land per-iteration (ARCH-ZETA-LOCAL-STORAGE.md section 5f). */
    for (int i = 0; i < g->n; i++) {
        IR_t * nd = g->all[i];
        if (!nd || nd->op != IR_MATCH_ARBNO || IR_LIT(nd).ival != 3 || nd->n_operands < 2) continue;
        int i0 = -1, i1 = -1;
        for (int j = 0; j < g->n; j++) { if (g->all[j] == nd->operands[0]) i0 = j; if (g->all[j] == nd->operands[1]) i1 = j; }
        if (i0 < 0 || i1 < 0) { fprintf(stderr, "zls: arbno2 geometry — body bracket operands not found in g->all\n"); abort(); }
        if (i0 > i1) { int t = i0; i0 = i1; i1 = t; }
        int mn = 0x7fffffff, mx = 0;
        for (int j = i0; j <= i1; j++) {
            const zls_entry_t * e = g->all[j] ? zx_find(g->all[j]) : (const zls_entry_t *)0;
            if (!e) continue;
            if (e->off < mn) mn = e->off;
            for (int f = 0; f < zf_n; f++) if (zf[f].nd == g->all[j] && zf[f].off + zf[f].size > mx) mx = zf[f].off + zf[f].size;
        }
        if (za_n >= (int)(sizeof za / sizeof *za)) { fprintf(stderr, "zls: arbno2 geometry table overflow (%d)\n", (int)(sizeof za / sizeof *za)); abort(); }
        if (mn == 0x7fffffff) za[za_n++] = (zls_ageom_t){ nd, 16, 0 };
        else                  za[za_n++] = (zls_ageom_t){ nd, mn, mx - mn };
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int zls_arbno_geom(const IR_t * nd, int * min_off, int * span) {
    for (int i = 0; i < za_n; i++) if (za[i].nd == nd) { if (min_off) *min_off = za[i].min_off; if (span) *span = za[i].span; return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* zls2_geom — the ZLS2 per-activation frame-protocol geometry authority (Lon directive, 2026-07-08 session:
 * "templates instrumented through x86's α/β/γ/ω with code injection of stack frame bump, restore on
 * backtrack, and release on exit").  This file already OWNS the save-slot layout (the zls_field grants above
 * name "arb.zls2" at +8 and "arbno.zls2" at +16), so the per-node protocol geometry lives here too — ONE
 * authority, queried at the emit dispatch point (the zls_arbno_geom precedent) and promoted into g_emit for
 * the central port hook in x86_asm.h.  Inputs: nd = the node; base_off = the node's ALREADY-RESOLVED frame
 * base (x86_scratch_off for ARB, op_off for ARBNO — owner resolution via operand[0] for phases 1/2 happens
 * at dispatch, exactly as the templates read it today).  Outputs: *slot_off = absolute frame offset of the
 * activation-block save slot; *k = ZLS2 block size (the former template-private ARB_ZLS2_K/ARBNO_ZLS2_K
 * constants, relocated here — block layouts documented at the grants above and in the templates).  Returns
 * the ZLS2_* ops bitmask (zeta_choices.h): which injections the port hook fires for this node.  The grant IS
 * the ω-death classifier: ZLS2_RELEASE is awarded ONLY to roles whose jmp-ω sites are STATICALLY all
 * activation-death (construct-role knowledge — never the op_omega_is_death chain-window classifier, recorded
 * broken in bb_match_arbno.cpp's L(9) note).  ⛔ RULING (Lon, 2026-07-08 s7): ONLY α AND ω PARTICIPATE IN
 * MEMORY ALLOCATION.  Under pure LIFO the ω that lands on a β has already put the cursor right (its own
 * RELEASE fired at death), so a β restore is always a no-op — exactly what the ALLOC-flavor traces observed
 * ("silent no-op restores under clean LIFO").  ZLS2_RESTORE is therefore never granted; frames of roles whose
 * own ω is an alias (ARBNO ph0) die at ph2's release / the statement backstop.  Per-role mapping:
 *   IR_MATCH_ARB        BUMP|RELEASE          α pushes; its single exhaust ω is always this activation's
 *                                             death → release.
 *   IR_MATCH_ARBNO ph0  BUMP                  α pushes.  Its β-arm jmp ω is the BODY-ENTRY ALIAS → never
 *                                             release here; ph2's release (same slot, chained) frees it.
 *   IR_MATCH_ARBNO ph1  0                     forward-flow landing; its jmp ω aliases into ph2 → no ops.
 *   IR_MATCH_ARBNO ph2  RELEASE               its single jmp ω is the construct's outer-fail true death →
 *                                             unchain + release ph0's frame (and stray body frames below).
 *   ARBNO v2 (ph3/4/5)  0                     rt_zcol COLLECTION machinery, not ZLS2 — untouched.
 * Returns 0 (no participation) for every other node kind — the hook stays inert exactly where it is today. */
int zls2_geom(const IR_t * nd, int base_off, int * slot_off, long * k) {
    if (!nd || base_off < 0) return 0;
    if (nd->op == IR_MATCH_ARB)   { if (slot_off) *slot_off = base_off + 8;  if (k) *k = 16; return ZLS2_BUMP | ZLS2_RELEASE; }
    if (nd->op == IR_MATCH_ARBNO) {
        long ph = IR_LIT(nd).ival;
        int ops = ph == 0 ? ZLS2_BUMP : ph == 2 ? ZLS2_RELEASE : 0;
        if (!ops) return 0;   /* ph1 / v2: outputs UNWRITTEN — bytes>0 with ops==0 is the DORMANT direct-sub arm's key (sub ZR,K!) and must never be reachable from this path */
        if (slot_off) *slot_off = base_off + 16;
        if (k) *k = 32;
        return ops;
    }
    return 0;
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
int zls_g_zeta_mark(const IR_graph_t * g) { zls_graph_t * r = zls_g_find(g); return r ? r->zeta_mark_off : -1; }
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
