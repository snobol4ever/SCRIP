#include "rt/rt_arena.h"
#include "../contracts/pin_va.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <alloca.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <ctype.h>
#include <errno.h>
#include <libgen.h>
#include <setjmp.h>
#include <time.h>
#include <unistd.h>
#include <sys/resource.h>
#include "../parser/snobol4/scrip_cc.h"
#include "../parser/snocone/snocone_driver.h"
#include "../parser/prolog/prolog_driver.h"
#include "../parser/prolog/term.h"
#include "../parser/prolog/prolog_runtime.h"
#include "../parser/prolog/prolog_atom.h"
#include "../parser/prolog/prolog_builtin.h"
#include "../parser/icon/icon_driver.h"
#include "../parser/pascal/pascal_driver.h"
#include "../parser/raku/raku_driver.h"
#include "../parser/rebus/rebus_lower.h"
#include "../runtime/builtins/gen.h"
#include "../parser/icon/icon_lex.h"
#include "../include/bb_box.h"
extern void ir_print_node   (const tree_t *e, FILE *f);
extern void ir_print_node_nl(const tree_t *e, FILE *f);
extern int pl_dyn_is_marked(const char *name, int arity);
#include "core.h"
#include "sil_macros.h"
#include "runtime_shim.h"
#include "lower.h"
#include "emit.h"
#include "bb_build.h"
#include "scrip_sm.h"
extern DESCR_t pat_at_cursor(const char *varname);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void stmt_init(void) {}
/* RTX-FUNC-0 AB posthook: file-statics + one-shot callback for g_emit_chain_posthook */
static IR_graph_t *g_ab_posthook_g = NULL; static int g_ab_posthook_gva = 0;
static void bb_ab_posthook(void) { extern void bb_ab_emit_nodes(IR_graph_t*, int); if (g_ab_posthook_g) bb_ab_emit_nodes(g_ab_posthook_g, g_ab_posthook_gva); }
/* ICN-FR-2: ζ-frame exit-wire thunks for the m3 main graph.  The ζ-frame epilogue does `jmp rcx` (γ) / `jmp rdx` (ω);
 * the main graph needs these to point at exit(0)/exit(1) respectively.  rt_outer_call does NOT preserve rcx/rdx across
 * its asm wrapper, so the driver uses a raw asm block to set them directly before calling the graph function. */
static void icn_zf_exit_γ(void) { exit(0); }
static void icn_zf_exit_ω(void) { exit(1); }
/* Jump to fn with rcx=wire_γ rdx=wire_ω rdi=mf rsi=0 r12=RT_DCAP_TOP.
 * ICN-FR-2: jmp not call — ζ-frame kt is sized for jmp entry (no return-address push on the stack);
 * using call would shift rsp by 8 and corrupt [rsp+kt-24/-16/-8] wire reads.  The main graph exits
 * via icn_zf_exit_γ/ω→exit() and never returns here, so the missing ret is correct. */
static void icn_zf_main_call(void *fn, void *mf, void *wire_γ, void *wire_ω) {
    __asm__ volatile(
        "push %%r12\n\t"
        "sub $8, %%rsp\n\t"
        "mov $0x70000000, %%r12\n\t"
        "mov (%%r12), %%r12\n\t"        /* r12 = *RT_DCAP_TOP */
        "xor %%esi, %%esi\n\t"         /* rsi = 0 */
        "xor %%r14d, %%r14d\n\t"       /* ICN-FR-5: r14=0 → &pos=1 outside any scan (bb_keyword_icon reads r14+1; uninitialized r14 gave &pos=4296041) */
        "jmp *%%rax\n\t"               /* ICN-FR-2: jmp (not call) — ζ-frame kt sized for jmp entry; exits via wire→exit() */
        :
        : "a"(fn), "D"(mf), "c"(wire_γ), "d"(wire_ω)
        : "memory", "rsi", "r8", "r9", "r10", "r11"           /* ICN-FR-5 CLOBBER-FIX: r14 removed — fn never returns (jmp→exit()), so GCC's clobber-driven r14 spill around the call site corrupted the stack used by rt_outer_call in the non-zframe path (image(int) SEGV). r14 is zeroed inside the asm body before the jmp; no caller-side save/restore needed. */
    );
}
extern DESCR_t      eval_expr(const char *src);
extern int exec_stmt(const char *subj_name,
                          DESCR_t    *subj_var,
                          DESCR_t     pat,
                          DESCR_t    *repl,
                          int         has_repl);
extern const char *Σ;
extern int         Ω;
extern int         Δ;
#include "../runtime/builtins/gen_runtime.h"
#include "../runtime/builtins/resolution.h"
#include "driver/polyglot.h"
#include "../tools/emit_per_kind_audit.h"
#include "../contracts/zeta_choices.h"
#include "../runtime/rt/zeta_alloc.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int scrip_symmap(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_SYMMAP"); v = e ? (atoi(e) != 0) : 0; } return v; }   /* R3 SPLICEMAP (s156): emit `.type <blob>_α, @function` + `.size <blob>_α, .-<blob>_α` around every emitted chain.  WHY: the blob labels are `.globl`'d but carry NO type and NO size, so their ELF symbols have st_size=0 and no address RANGE — cachegrind/callgrind therefore attribute every sample inside a blob to `???` instead of to the blob.  Measured s156 on treebank: 92.1% of ALL D1 write misses (168,928 of 183,501) land in `???`, so the α-diet campaign is aiming blind on its own primary metric; the s155 cursor already recorded the same blindness for claws5/json at 97-100%.  This is PURE ELF METADATA — not one instruction byte changes — but it is opt-in so THIS session's default `.s` stays byte-identical and the change is provably inert; the default flip is its own rung (with the `.s` regen it implies), exactly as SCRIP_ARBNO_LATCH and SCRIP_SEQ_FOLD landed. */
static int proc_role3_kind(const IR_graph_t *g) { const IR_t *e = (g && g->entry && g->entry->op == IR_SAVE_RESTORE && IR_LIT(g->entry).ival == 3) ? g->entry : (const IR_t *)0; return !e ? 0 : (e->γ.node && e->γ.node->op == IR_GOTO_DEFERRED) ? 1 : 2; }   /* ⭐ EXPR-THUNK EXITS (GOAL-SNOBOL4-100 bb_probes class B, s96): THE ONE AUTHORITY for what a role-3 WIRE-ADOPT-entered chain IS -- 0 = not role-3; 1 = the DEFINE stub blob (lower_snobol4.c:2048, role-3 -> IR_GOTO_DEFERRED transfer into a body that lives in main's chain: BARE per s62, registered AT its DEFINE site per s57); 2 = the self-contained EXPR$ thunk (lower_snobol4.c:2458, role-3 -> its own body -> IR_SUCCEED/IR_FAIL: owns its wire exits, has NO DEFINE site so it MUST be registered at startup).  MEASURED (f6d/t6m/fence_probe + `S BREAK(',') *DIFFER(X)`): the s62 _bare predicate keyed on the bare role-3 shape, so every EXPR$ thunk was emitted BARE in m4 -- its γ/ω aliased to main_ω -- and the s57 dyn_scope skip left it unregistered ([GZ-10] no stackless slab); m3 crashed on the same graphs through rt_proc_enter's stale rcx/rdx wire delivery (rt.c twin fix). */
static const char *asm_sym_name(const char *nm) { static char b[256]; int j = 0; for (const char *c = nm ? nm : ""; *c && j < 250; c++) { unsigned char u = (unsigned char) *c; if ((u >= 'A' && u <= 'Z') || (u >= 'a' && u <= 'z') || (u >= '0' && u <= '9') || u == '_' || u == '$' || u == '.') b[j++] = (char) u; else j += snprintf(b + j, (size_t) (256 - j), "$%02X", u); } b[j] = 0; return b; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int keyword_supported(const char *kw) {
    if (!kw) return 0;
    if (kw[0] == '&') kw++;
    return !strcmp(kw, "subject") || !strcmp(kw, "pos") || !strcmp(kw, "null") || !strcmp(kw, "fail");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int scan_safe_kind(IR_e t) {
    return t == IR_SUCCEED || t == IR_FAIL ||
           t == IR_LIT_INTEGER || t == IR_LIT_STRING || t == IR_LIT_REAL || t == IR_OP_COUNT ||
           t == IR_VAR || t == IR_KEYWORD_ICON || t == IR_KEYWORD_SNOBOL4 || t == IR_OP_COUNT || t == IR_CALL || ir_is_scan_kind(t) || t == IR_BINOP
        || t == IR_OP_COUNT || t == IR_CONJUNCTION || t == IR_ASSIGN || t == IR_OP_COUNT || t == IR_OP_COUNT || t == IR_OP_COUNT;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sg_var_assigned(IR_graph_t *sg, const char *name) {
    if (!sg || !sg->all || !name) return 0;
    for (int i = 0; i < sg->n; i++) { IR_t *m = sg->all[i]; if (m && m->op == IR_ASSIGN && IR_LIT(m).sval && !strcmp(IR_LIT(m).sval, name)) return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t *scan_lit_entry(IR_t *nd, IR_e want) {
    IR_graph_t **sblks = (IR_graph_t **)0;
    IR_t *ae = (sblks && (int)IR_LIT(nd).ival == 1 && sblks[0]) ? sblks[0]->entry : (IR_t *)0;
    if (!ae || ae->op != want) return (IR_t *)0;
    if (ae->γ.node && ae->γ.node->op != IR_SUCCEED) return (IR_t *)0;
    return ae;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int scan_fn_lit_arg(IR_t *nd, IR_e want) {
    return scan_lit_entry(nd, want) != (IR_t *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int scan_fn_cset_arg(IR_t *nd) {
    extern const char *kw_cset_const_str(const char *kw);
    if (scan_lit_entry(nd, IR_LIT_STRING) != (IR_t *)0) return 1;
    IR_graph_t **sblks = (IR_graph_t **)0;
    IR_t *ae = (sblks && (int)IR_LIT(nd).ival == 1 && sblks[0]) ? sblks[0]->entry : (IR_t *)0;
    if (!ae || (ae->op != IR_KEYWORD_ICON && ae->op != IR_KEYWORD_SNOBOL4)) return 0;
    if (ae->γ.node && ae->γ.node->op != IR_SUCCEED) return 0;
    return kw_cset_const_str(IR_LIT(ae).sval) != (const char *)0;
}
static int graph_var_assigned_or_param(stage2_t *s2, int gi, IR_graph_t *g, const char *name);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int scan_any_cset_var_ok(stage2_t *s2, int gi, IR_graph_t *g, IR_t *nd) {
    IR_graph_t **sblks = (IR_graph_t **)0;
    IR_t *ae = (sblks && (int)IR_LIT(nd).ival == 1 && sblks[0]) ? sblks[0]->entry : (IR_t *)0;
    if (!ae || ae->op != IR_VAR || !IR_LIT(ae).sval || IR_LIT(ae).sval[0] == '&') return 0;
    if (ae->γ.node && ae->γ.node->op != IR_SUCCEED) return 0;
    return graph_var_assigned_or_param(s2, gi, g, IR_LIT(ae).sval);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int scan_tab_arg_ok(IR_t *nd) {
    IR_graph_t **sblks = (IR_graph_t **)0;
    IR_t *ae = (sblks && (int)IR_LIT(nd).ival == 1 && sblks[0]) ? sblks[0]->entry : (IR_t *)0;
    if (!ae) return 0;
    if (ae->γ.node && ae->γ.node->op != IR_SUCCEED) return 0;
    if (ae->op == IR_LIT_INTEGER && IR_LIT(ae).ival >= 1) return 1;
    if ((ae->op == IR_CALL || ir_is_scan_kind(ae->op)) && IR_LIT(ae).dval == 3.0 && IR_LIT(ae).sval
        && (!strcmp(IR_LIT(ae).sval, "any") || !strcmp(IR_LIT(ae).sval, "many") || !strcmp(IR_LIT(ae).sval, "upto")) && scan_fn_cset_arg(ae))
        return 1;
    if ((ae->op == IR_CALL || ir_is_scan_kind(ae->op)) && IR_LIT(ae).dval == 3.0 && IR_LIT(ae).sval
        && (!strcmp(IR_LIT(ae).sval, "match") || !strcmp(IR_LIT(ae).sval, "find") || !strcmp(IR_LIT(ae).sval, "bal")) && scan_fn_lit_arg(ae, IR_LIT_STRING))
        return 1;
    return 0;
}
static int graph_var_assigned_or_param(stage2_t *s2, int gi, IR_graph_t *g, const char *name);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int scan_subgraph_safe(stage2_t *s2, int gi, IR_graph_t *g, IR_graph_t *sg, int depth) {
    if (!sg || !sg->all || sg->n <= 0 || depth > 16) return 0;
    for (int i = 0; i < sg->n; i++) {
        IR_t *nd = sg->all[i];
        if (!nd) continue;
        if (!scan_safe_kind(nd->op)) return 0;
        if (nd->op == IR_VAR) {
            if (IR_LIT(nd).sval && IR_LIT(nd).sval[0] == '&') { if (!keyword_supported(IR_LIT(nd).sval)) return 0; }
            else if (!IR_LIT(nd).sval || (!graph_var_assigned_or_param(s2, gi, g, IR_LIT(nd).sval) && !sg_var_assigned(sg, IR_LIT(nd).sval))) return 0;
        }
        if (nd->op == IR_ASSIGN) { if (!IR_LIT(nd).sval || is_global(IR_LIT(nd).sval)) return 0; }
        if ((nd->op == IR_KEYWORD_ICON || nd->op == IR_KEYWORD_SNOBOL4) && !keyword_supported(IR_LIT(nd).sval)) return 0;
        if (nd->op == IR_CALL || ir_is_scan_kind(nd->op)) {
            if (!IR_LIT(nd).sval) return 0;
            if (!strcmp(IR_LIT(nd).sval, "any")) { if (!(IR_LIT(nd).dval == 3.0 && (scan_fn_cset_arg(nd) || scan_any_cset_var_ok(s2, gi, g, nd)))) return 0; }
            else if (!strcmp(IR_LIT(nd).sval, "many") || !strcmp(IR_LIT(nd).sval, "upto")) { if (!(IR_LIT(nd).dval == 3.0 && scan_fn_cset_arg(nd))) return 0; }
            else if (!strcmp(IR_LIT(nd).sval, "match")) { if (!(IR_LIT(nd).dval == 3.0 && scan_fn_lit_arg(nd, IR_LIT_STRING))) return 0; }
            else if (!strcmp(IR_LIT(nd).sval, "tab")) { if (!(IR_LIT(nd).dval == 3.0 && scan_tab_arg_ok(nd))) return 0; }
            else if (!strcmp(IR_LIT(nd).sval, "move")) { if (!(IR_LIT(nd).dval == 3.0 && scan_fn_lit_arg(nd, IR_LIT_INTEGER))) return 0; }
            else if (!strcmp(IR_LIT(nd).sval, "pos")) { IR_t *pe = scan_lit_entry(nd, IR_LIT_INTEGER); if (!(IR_LIT(nd).dval == 3.0 && pe && IR_LIT(pe).ival >= 1)) return 0; }
            else if (!strcmp(IR_LIT(nd).sval, "find")) {
                IR_t *fe = scan_lit_entry(nd, IR_LIT_STRING); if (!(IR_LIT(nd).dval == 3.0 && fe && IR_LIT(fe).sval && IR_LIT(fe).sval[0] && strlen(IR_LIT(fe).sval) <= 32)) return 0;
            }
            else if (!strcmp(IR_LIT(nd).sval, "bal")) {
                IR_t *be = scan_lit_entry(nd, IR_LIT_STRING);
                if (!(IR_LIT(nd).dval == 3.0 && be && IR_LIT(be).sval && IR_LIT(be).sval[0] && !strchr(IR_LIT(be).sval, 40) && !strchr(IR_LIT(be).sval, 41))) return 0;
            }
            else if (!(!strcmp(IR_LIT(nd).sval, "write") || !strcmp(IR_LIT(nd).sval, "writes"))) return 0;
        }
        if (nd->op == IR_BINOP) { int64_t bc = IR_LIT(nd).ival; int is_rel = (bc >= BINOP_LT && bc <= BINOP_NE) || (bc >= BINOP_SLT && bc <= BINOP_SNE) || bc == BINOP_EQV || bc == BINOP_NEQV; if (bc != BINOP_CONCAT && !is_rel) return 0; }
        if (nd->op == IR_OP_COUNT) {
            IR_graph_t *ssg = (IR_graph_t *)0;
            IR_graph_t *bsg = (IR_graph_t *) 0;
            if (!scan_subgraph_safe(s2, gi, g, ssg, depth + 1) || !scan_subgraph_safe(s2, gi, g, bsg, depth + 1)) return 0;
        }
    }
    return 1;
}
static int graph_native_emittable_mode(stage2_t *s2, int for_run);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int graph_native_emittable(stage2_t *s2) { return graph_native_emittable_mode(s2, 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int gen_scan_body_slotful(IR_t *r) {
    if (!r || r->op != IR_OP_COUNT || IR_LIT(r).dval != 1.0) return 0;
    IR_graph_t *bsg = (IR_graph_t *) 0;
    IR_t *bt = bsg ? bsg->entry : (IR_t *)0;
    int gd = 0;
    while (bt && bt->γ.node && bt->γ.node->op != IR_SUCCEED && bt->γ.node->op != IR_FAIL && gd++ < 512) bt = bt->γ.node;
    if (bt && (bt->op == IR_LIT_INTEGER || bt->op == IR_LIT_STRING)) return 1;
    if (bt && bt->op == IR_VAR && IR_LIT(bt).sval && IR_LIT(bt).sval[0] != '&') return 1;
    if (bt && (bt->op == IR_CALL || ir_is_scan_kind(bt->op)) && IR_LIT(bt).dval == 3.0 && IR_LIT(bt).sval
        && (!strcmp(IR_LIT(bt).sval, "tab") || !strcmp(IR_LIT(bt).sval, "move") || !strcmp(IR_LIT(bt).sval, "pos") || !strcmp(IR_LIT(bt).sval, "any")
            || !strcmp(IR_LIT(bt).sval, "match") || !strcmp(IR_LIT(bt).sval, "many") || !strcmp(IR_LIT(bt).sval, "upto") || !strcmp(IR_LIT(bt).sval, "find") || !strcmp(IR_LIT(bt).sval, "bal")))
        return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rhs_kind_ok(IR_t *r) {
    if (!r) return 0;
    if (r->op == IR_LIT_INTEGER || r->op == IR_LIT_STRING || r->op == IR_OP_COUNT || r->op == IR_LIT_REAL) return 1;
    if (r->op == IR_VAR && IR_LIT(r).sval && IR_LIT(r).sval[0] != '&') return 1;
    if (r->op == IR_VAR && IR_LIT(r).sval && !strcmp(IR_LIT(r).sval, "&null")) return 1;
    if (r->op == IR_BINOP_RELOP_VAL) return 1;   /* RK-ZC-6: bool_compare_store uses BINOP_RELOP_VAL as assign RHS; template bb_binop_relop_val() already handles it; gate was blocking with SMX. */
    if (r->op == IR_BINOP && (IR_LIT(r).ival == BINOP_ADD || IR_LIT(r).ival == BINOP_SUB || IR_LIT(r).ival == BINOP_MUL
                               || IR_LIT(r).ival == BINOP_DIV || IR_LIT(r).ival == BINOP_MOD || IR_LIT(r).ival == BINOP_CONCAT))
        return 1;
    if (ir_norm_call_kind(r->op) == IR_CALL || r->op == IR_UNOP || r->op == IR_FIELD_GET || r->op == IR_PROC_GEN) return 1;
    if (r->op == IR_CALL && IR_LIT(r).dval == 0.0) return 1;
    if (r->op == IR_CALL && IR_LIT(r).dval == 1.0) return 1;
    { extern void *dat_find_type(const char *name); if (r->op == IR_CALL && IR_LIT(r).dval == 3.0 && IR_LIT(r).sval && dat_find_type(IR_LIT(r).sval)) return 1; }
    { extern int rt_builtin_is_known(const char *name); const char *bn = IR_LIT(r).sval; if (r->op == IR_CALL && IR_LIT(r).dval == 3.0 && bn && rt_builtin_is_known(bn)) return 1; }
    if (r->op == IR_OP_COUNT) return 1;
    if (r->op == IR_TO || r->op == IR_TO_BY || r->op == IR_PROC_GEN || r->op == IR_OP_COUNT) return 1;
    if (r->op == IR_MAKE_LIST) return 1;
    if (r->op == IR_CONJUNCTION) { IR_t *lv = (r->n_operands > 0) ? r->operands[0] : (IR_t *)0; return lv ? rhs_kind_ok(lv) : 0; }
    {
        extern int is_global(const char *);
        if (r->op == IR_ASSIGN && IR_LIT(r).sval && !is_global(IR_LIT(r).sval)) {
            IR_t *rv = (r->n_operands > 0) ? r->operands[0] : (IR_t *)0; return rv ? rhs_kind_ok(rv) : 0;
        }
    }
    if (r->op == IR_OP_COUNT) return 1;
    if (r->op == IR_OP_COUNT) return 1;
    if (r->op == IR_CALL && IR_LIT(r).dval == 2.0 && !(IR_LIT(r).sval && (!strcmp(IR_LIT(r).sval,"__rk_bool")||!strcmp(IR_LIT(r).sval,"__rk_try")))) return 1;
    if (r->op == IR_OP_COUNT) { int64_t u = IR_LIT(r).ival; if (u == TT_MNS || u == TT_PLS || u == TT_SIZE || u == TT_NONNULL || u == TT_NULL || u == TT_NOT) return 1; }
    if (r->op == IR_OP_COUNT || r->op == IR_OP_COUNT || r->op == IR_OP_COUNT || r->op == IR_OP_COUNT || r->op == IR_OP_COUNT) return 1;
    if (r->op == IR_OP_COUNT) return gen_scan_body_slotful(r);
    return 0;
}
static int graph_has_binop(const IR_graph_t *g);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int local_assign_rhs_ok_g(const IR_graph_t *g, IR_t *nd) {
    IR_t *rhs = (nd->n_operands > 0) ? nd->operands[0] : (IR_t *)0;
    if (!rhs) for (int i = 0; i < g->n; i++) { IR_t *p = g->all[i]; if (p && p->γ.node == nd) { rhs = p; break; } }
    if (rhs && rhs->op == IR_LIT_REAL) return 1;
    return rhs_kind_ok(rhs);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int arith_operand_ok(IR_t *r) {
    if (!r) return 0;
    if (r->op == IR_LIT_INTEGER) return 1;
    if (r->op == IR_BINOP && (IR_LIT(r).ival == BINOP_ADD || IR_LIT(r).ival == BINOP_SUB || IR_LIT(r).ival == BINOP_MUL || IR_LIT(r).ival == BINOP_DIV || IR_LIT(r).ival == BINOP_MOD)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_jct_call(IR_t *r) {
    return r && r->op == IR_CALL && IR_LIT(r).sval && !strncmp(IR_LIT(r).sval, "__rk_jct_", 9);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int jct_marshallable(IR_t *r) {
    if (!r) return 0;
    if (r->op == IR_LIT_INTEGER || r->op == IR_LIT_STRING || r->op == IR_LIT_REAL || r->op == IR_OP_COUNT) return 1;
    if (r->op == IR_VAR && IR_LIT(r).sval && IR_LIT(r).sval[0] != '&') return 1;
    if (r->op == IR_CALL && (IR_LIT(r).dval == 2.0 || IR_LIT(r).dval == 3.0 || IR_LIT(r).dval == 5.0)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bool_truthy_emittable(IR_t *nd) {
    if (!nd || nd->op != IR_CALL || !IR_LIT(nd).sval || strcmp(IR_LIT(nd).sval,"__rk_bool") || IR_LIT(nd).dval != 2.0) return 0;
    IR_graph_t **blks = (IR_graph_t **)0;
    IR_graph_t *cond = blks ? blks[0] : (IR_graph_t *)0;
    if (!cond || !cond->entry) return 0;
    IR_t *e = cond->entry;
    return (e->op == IR_LIT_INTEGER || e->op == IR_LIT_STRING || (e->op == IR_VAR && IR_LIT(e).sval && IR_LIT(e).sval[0] != '&'));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bool_cond_emittable(IR_t *nd) {
    if (!nd || nd->op != IR_CALL || !IR_LIT(nd).sval || strcmp(IR_LIT(nd).sval, "__rk_bool") || IR_LIT(nd).dval != 2.0) return 0;
    IR_graph_t **blks = (IR_graph_t **)0;
    IR_graph_t *cond = blks ? blks[0] : (IR_graph_t *)0;
    if (!cond) return 0;
    IR_t *p = cond->entry; IR_t *rel = (IR_t *)0; int gd = 0;
    while (p && gd++ < 256) { if (p->op == IR_BINOP && IR_LIT(p).ival >= BINOP_LT && IR_LIT(p).ival <= BINOP_NE) { rel = p; break; } if (!p->γ.node) break; p = p->γ.node; }
    if (!rel) return 0;
    IR_t *ra = ir_pair_arg(rel, 0); IR_t *rb = ir_pair_arg(rel, 1);
    if ((is_jct_call(ra) || is_jct_call(rb)) && jct_marshallable(ra) && jct_marshallable(rb)) return 1;
    return arith_operand_ok(ra) && arith_operand_ok(rb);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int graph_has_local_assign(const IR_graph_t *g) {
    for (int ni = 0; ni < g->n; ni++) {
        IR_t *nd = g->all[ni];
        if (nd && nd->op == IR_ASSIGN && IR_LIT(nd).sval && !is_global(IR_LIT(nd).sval)) return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int graph_has_binop(const IR_graph_t *g) {
    for (int ni = 0; ni < g->n; ni++) if (g->all[ni] && g->all[ni]->op == IR_BINOP) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int graph_var_assigned_or_param(stage2_t *s2, int gi, IR_graph_t *g, const char *name) {
    for (int i = 0; i < g->n; i++) { IR_t *m = g->all[i]; if (m && m->op == IR_ASSIGN && IR_LIT(m).sval && !strcmp(IR_LIT(m).sval, name)) return 1; }
    for (int p = 0; p < s2->proc_count; p++) {
        if (s2->proc_table[p].bb_idx != gi) continue;
        for (int k = 0; k < s2->proc_table[p].nparams && k < s2->proc_table[p].lower_sc.n; k++)
            if (s2->proc_table[p].lower_sc.e[k].name && !strcmp(s2->proc_table[p].lower_sc.e[k].name, name)) return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int graph_native_emittable_mode(stage2_t *s2, int for_run) {
    extern int rt_builtin_is_known(const char *name);
    if (!s2) return 0;
    for (int gi = 0; gi < s2->bbp.count; gi++) {
        IR_graph_t *g = s2->bbp.table[gi];
        if (!g || !g->all) continue;
        int has_lassign = graph_has_local_assign(g);
        int has_binop = graph_has_binop(g);
        for (int ni = 0; ni < g->n; ni++) {
            IR_t *nd = g->all[ni];
            if (!nd) continue;
            if (nd->op == IR_CALL && IR_LIT(nd).dval == 2.0 && IR_LIT(nd).sval && strcmp(IR_LIT(nd).sval,"__rk_bool") && strcmp(IR_LIT(nd).sval,"__rk_try") && !rt_builtin_is_known(IR_LIT(nd).sval))
                return 0;
            if (nd->op == IR_OP_COUNT) {
                if (nd->n_operands < 1 || !nd->operands[0]) return 0;
                for (int aj = 1; aj < nd->n_operands; aj++) {
                    IR_t *arm = nd->operands[aj];
                    if (!arm || arm->op != IR_OP_COUNT || arm->n_operands < 1 || !arm->operands[0]) return 0;
                }
            }
            if (nd->op == IR_OP_COUNT && (nd->n_operands < 1 || !nd->operands[0])) return 0;
            if (0 && nd->op == IR_OP_COUNT) return 0;
            if (nd->op == IR_OP_COUNT) return 0;             if (nd->op == IR_OP_COUNT) {
                IR_t *rv = (nd->n_operands > 1) ? nd->operands[1] : (IR_t *)0; if (!rv || !rhs_kind_ok(rv) || rv->op == IR_OP_COUNT) return 0;
            }
            if (nd->op == IR_OP_COUNT) {
                IR_t *lv = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0; IR_t *rv = nd->n_operands > 1 ? nd->operands[1] : (IR_t *)0;
                if (!lv || !rv || lv->op != IR_VAR || rv->op != IR_VAR || !IR_LIT(lv).sval || !IR_LIT(rv).sval) return 0;
            }
            if (nd->op == IR_CALL && IR_LIT(nd).dval == 2.0 && IR_LIT(nd).sval && (!strcmp(IR_LIT(nd).sval,"__rk_bool")||!strcmp(IR_LIT(nd).sval,"__rk_try"))) {
                if (bool_cond_emittable(nd)||bool_truthy_emittable(nd)) {} else return 0;
            }
            if (nd->op == IR_OP_COUNT) {
                if (IR_LIT(nd).dval != 1.0) return 0;
                IR_graph_t *ssg = (IR_graph_t *)0;
                IR_graph_t *bsg = (IR_graph_t *) 0;
                if (!scan_subgraph_safe(s2, gi, g, ssg, 0) || !scan_subgraph_safe(s2, gi, g, bsg, 0)) return 0;
                if (nd->γ.node && (nd->γ.node->op == IR_CALL || ir_is_scan_kind(nd->γ.node->op)) && !gen_scan_body_slotful(nd)) return 0;
            }
            {
              if (nd->op == IR_VAR && IR_LIT(nd).sval && IR_LIT(nd).sval[0] != '&' && !is_global(IR_LIT(nd).sval) && !graph_var_assigned_or_param(s2, gi, g, IR_LIT(nd).sval)) return 0;
              if (nd->op == IR_ASSIGN && IR_LIT(nd).sval) {
                  int lhs_global = is_global(IR_LIT(nd).sval);
                  if (lhs_global) {  }
                  else if (local_assign_rhs_ok_g(g, nd)) {  }
                  else return 0;
              } }
        }
    }
    return 1;
}
static IR_graph_t **g_gz_visiting = NULL; static int g_gz_nvisiting = 0; static int g_gz_visiting_cap = 0;
static IR_graph_t **g_gz_det_visiting = NULL; static int g_gz_det_nvisiting = 0; static int g_gz_det_visiting_cap = 0;
static int g_gz_no_struct_ptr = 0;
extern int resolve_bb_pred_count(void);
extern const char *resolve_bb_pred_name_at(int idx);
extern int resolve_bb_pred_arity_at(int idx);
extern IR_t *resolve_bb_entry_node(const char *name, int arity);
extern IR_graph_t *resolve_bb_graph_at(int idx);
extern int resolve_bb_pred_count(void);
extern const char *resolve_bb_pred_name_at(int idx);
extern int resolve_bb_pred_arity_at(int idx);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int    g_prog_argc = 0;
static char **g_prog_argv = NULL;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void drive_slots_all(stage2_t * s2) {
    extern void ir_drive_slot_assign(IR_graph_t * g);
    for (int _gi = 0; _gi < s2->bbp.count; _gi++) if (s2->bbp.table[_gi]) ir_drive_slot_assign(s2->bbp.table[_gi]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long parse_mem_arg(const char *s) {
    char *end = NULL; errno = 0; long v = strtol(s, &end, 10); if (errno || end == s || v < 0) return -1;
    if (*end == 'k' || *end == 'K') { v *= 1024L; end++; } else if (*end == 'm' || *end == 'M') { v *= 1024L * 1024L; end++; }
    return (*end == '\0') ? v : -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int apply_stack_limit(long bytes) {
    struct rlimit rl; if (getrlimit(RLIMIT_STACK, &rl) != 0) return -1;
    if (rl.rlim_max != RLIM_INFINITY && (rlim_t)bytes > rl.rlim_max) bytes = (long)rl.rlim_max;
    if (rl.rlim_cur != RLIM_INFINITY && (rlim_t)bytes <= rl.rlim_cur) return 0;
    rl.rlim_cur = (rlim_t)bytes; return setrlimit(RLIMIT_STACK, &rl);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void bbj_str(FILE * fp, const char * s) {
    fputc('"', fp);
    for (const unsigned char * p = (const unsigned char *) (s ? s : ""); *p; p++) {
        if (*p == '"' || *p == '\\') { fputc('\\', fp); fputc(*p, fp); }
        else if (*p == '\n') fputs("\\n", fp);
        else if (*p == '\t') fputs("\\t", fp);
        else if (*p == '\r') fputs("\\r", fp);
        else if (*p < 0x20) fprintf(fp, "\\u%04x", *p);
        else fputc(*p, fp);
    }
    fputc('"', fp);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void bbj_label(FILE * fp, const IR_t * bb) {
    switch (bb->op) {
        case IR_LIT_INTEGER: fprintf(fp, ",\"label\":\"%lld\"", (long long) IR_LIT(bb).ival); break;
        case IR_LIT_REAL: fprintf(fp, ",\"label\":\"%g\"", IR_LIT(bb).dval); break;
        case IR_LIT_STRING: case IR_LIT_CHARSET: if (IR_LIT(bb).sval) { fputs(",\"label\":", fp); bbj_str(fp, IR_LIT(bb).sval); } break;
        case IR_VAR: case IR_ASSIGN: if (IR_LIT(bb).sval) { fputs(",\"label\":", fp); bbj_str(fp, IR_LIT(bb).sval); } break;
        case IR_KEYWORD_ICON: case IR_KEYWORD_ICON_GEN: case IR_KEYWORD_SNOBOL4: if (IR_LIT(bb).sval) { fputs(",\"label\":", fp); bbj_str(fp, IR_LIT(bb).sval); } break;
        case IR_MATCH_LIT: case IR_MATCH_ANY: case IR_MATCH_NOTANY: case IR_MATCH_SPAN: if (IR_LIT(bb).sval) { fputs(",\"label\":", fp); bbj_str(fp, IR_LIT(bb).sval); } break;
        case IR_BINOP: case IR_BINOP_TEST: fprintf(fp, ",\"label\":\"op%lld\"", (long long) IR_LIT(bb).ival); break;
        case IR_CALL: case IR_CALL_PROC_STAGED: case IR_CALL_BUILTIN: case IR_CALL_BUILTIN_GEN: case IR_CALL_BUILTIN_ICON: case IR_CALL_BUILTIN_SNOBOL4: if (IR_LIT(bb).sval) { fputs(",\"label\":", fp); bbj_str(fp, IR_LIT(bb).sval); } break;
        default: break;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bbj_index(const IR_graph_t * g, const IR_t * nd) {
    for (int i = 0; i < g->n; i++) if (g->all[i] == nd) return i;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void bbj_edge(FILE * fp, int * first, int gi, int i, const char * fp_name, const IR_graph_t * g, const IR_ref_t * r) {
    if (!r->node) return;
    int j = bbj_index(g, r->node);
    if (j < 0) return;
    const char * tp = (r->sz[0] && strcmp(r->sz, "β") == 0) ? "beta" : "alpha";
    fprintf(fp, "%s\n  {\"from\":\"b%d_%d\",\"fp\":\"%s\",\"to\":\"b%d_%d\",\"tp\":\"%s\"}", *first ? "" : ",", gi, i, fp_name, gi, j, tp);
    *first = 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void m3_seal_entry_cells(const char *pname, void *fnbase, int alpha_face) {   /* R-1 s94 (Fable 5) THE CELL FILL, one authority for both directions of the TINY cross-chain crossing (x86_jmp_via_cell): the just-sealed chain's label pool is live until the next bb_emit_begin, so <FN>_alpha (DEFINE stub seal, alpha_face=1) / <ENTRY>_body (main seal, alpha_face=0) resolve by name to a stable cell of the ONE allocator (bb_ab_fn_cell_ptr) that the sites/shim already jump through -- no new global, no ordering coupling */
    extern int emit_label_lookup_offset(const char *); extern void * bb_ab_fn_cell_ptr(const char *);
    if (!pname || !fnbase) return;
    char lbl[300], cell[300];
    if (alpha_face) { snprintf(lbl, sizeof lbl, "%s_alpha", pname); snprintf(cell, sizeof cell, "alpha$%s", pname); }   /* raw: the role-4 shim interns fn4+"_alpha" verbatim; _body aliases are asm_sym_name'd (BODY-ALIAS build) */
    else            { snprintf(lbl, sizeof lbl, "%s_body",  asm_sym_name(pname)); snprintf(cell, sizeof cell, "body$%s",  pname); }
    int off = emit_label_lookup_offset(lbl); if (off < 0) return;
    *(void **)bb_ab_fn_cell_ptr(cell) = (void *)((char *)fnbase + off);
}
int main(int argc, char **argv)
{
    if (argc >= 3 && strcmp(argv[1], "--audit-per-kind") == 0) {
        fprintf(stderr, "scrip: --audit-per-kind unavailable (audit tool unlinked)\n");
        return 1;
    }
    int mode_run           = 0;
    int mode_compile       = 0;
    int dump_ast           = 0;
    int dump_ir            = 0;
    int dump_bb            = 0;
    int dump_ir_verbose    = 0;
    int dump_zeta          = 0;
    int dump_transpile     = 0;
    int opt_bench          = 0;
    const char * target_name = NULL;
    const char * output_path = NULL;   /* m4 -o flag (this session): output file for --compile; NULL = stdout */
    int argi = 1;
    while (argi < argc && argv[argi][0] == '-' && argv[argi][1] == '-') {
        if      (strcmp(argv[argi], "--run")           == 0) { mode_run       = 1; argi++; }
        else if (strcmp(argv[argi], "--compile")       == 0) { mode_compile   = 1; if (!target_name) target_name = "x86"; argi++; }
        else if (strncmp(argv[argi], "--target=", 9)   == 0) { target_name = argv[argi] + 9; mode_compile = 1; argi++; }
        else if (strcmp(argv[argi], "--dump-ast")      == 0) { dump_ast       = 1; argi++; }
        else if (strcmp(argv[argi], "--dump-ir-verbose") == 0) { dump_ir = 1; dump_ir_verbose = 1; argi++; }
        else if (strcmp(argv[argi], "--dump-ir")       == 0) { dump_ir        = 1; argi++; }
        else if (strcmp(argv[argi], "--dump-bb")       == 0) { dump_bb        = 1; argi++; }
        else if (strcmp(argv[argi], "--dump-zeta")     == 0) { dump_zeta      = 1; argi++; }
        else if (strcmp(argv[argi], "--transpile")     == 0) { dump_transpile = 1; argi++; }
        else if (strncmp(argv[argi], "--zeta=", 7)     == 0) { extern void rt_zeta_set_mode(int); const char *z = argv[argi] + 7; int zm = strcmp(z, "zls") == 0 ? 0 : strcmp(z, "zls2") == 0 ? 1 : strcmp(z, "zh") == 0 ? 2 : -1; /* 0/1/2 = ZC_ZETA_ZLS/ZC_ZETA_ZLS2/ZC_ZETA_ZH (zeta_choices.h) */ if (zm < 0) { fprintf(stderr, "scrip: bad --zeta=%s (valid: zls, zls2, zh)\n", z); return 2; } rt_zeta_set_mode(zm); argi++; }
        else if (strncmp(argv[argi], "--zeta-port=", 12) == 0) { const char *z = argv[argi] + 12; int pm = strcmp(z, "plain") == 0 ? 0 : strcmp(z, "instrumented") == 0 ? 1 : strcmp(z, "alloc") == 0 ? 2 : strcmp(z, "inline") == 0 ? 3 : strcmp(z, "cstack") == 0 ? 4 : strcmp(z, "forth") == 0 ? 6 : strcmp(z, "heap") == 0 ? 7 : -1; /* ZC_PORT_* (zeta_choices.h): inline = old arena, cstack = ζ on the C stack, forth = cstack superset (per-BB fixed rsp cells + ARBNO linked-frame-chain) = the compiled default; heap = RUNG ZHEAP target (rbx bump frontier), added s206 — it was reachable only as SCRIP_ZETA_PORT=7 while being the #1 rung's target port; owned(5) stays env-only; Z4-5 (GOAL-ZETA-FOUR): forth/cstack/heap are ALIASES routed through the four-config selector — rt_zeta_storage_set derives this axis, so the selector is the single authority and a later flag of EITHER spelling wins WHOLESALE (no mixed axis states); plain/instrumented/alloc/inline keep the direct set until the Z4-9 cut */ if (pm < 0) { fprintf(stderr, "scrip: bad --zeta-port=%s (valid: plain, instrumented, alloc, inline, cstack, forth, heap)\n", z); return 2; } if (pm == ZC_PORT_FORTH) rt_zeta_storage_set(ZC_STORAGE_CELL_STACK); else if (pm == ZC_PORT_CSTACK) rt_zeta_storage_set(ZC_STORAGE_FRAME_RSP); else if (pm == ZC_PORT_HEAP) rt_zeta_storage_set(ZC_STORAGE_CELL_HEAP); else rt_zeta_port_set_mode(pm); argi++; }
        else if (strncmp(argv[argi], "--zeta-storage=", 15) == 0) { const char *z = argv[argi] + 15; if (strcmp(z, "frame-r12") == 0) { fprintf(stderr, "scrip: --zeta-storage=frame-r12 RETIRED s23k ZW-0 (r12 = COND-ASSIGN stack, Lon directive; island zeta-frame technique withdrawn)\n"); return 2; } int sm = strcmp(z, "frame-rsp") == 0 ? 1 : strcmp(z, "cell-stack") == 0 ? 2 : strcmp(z, "cell-heap") == 0 ? 3 : -1; /* ZC_STORAGE (GOAL-ZETA-FOUR Z4-4 slice 2): the four-config selector; the setter derives the legacy port tuple (cell-stack->forth, cell-heap->heap, frame-rsp->cstack; frame-r12 none until Z4-7) so zero downstream seams flip this rung */ if (sm < 0) { fprintf(stderr, "scrip: bad --zeta-storage=%s (valid: frame-r12, frame-rsp, cell-stack, cell-heap)\n", z); return 2; } rt_zeta_storage_set(sm); argi++; }
        else if (strcmp(argv[argi], "--bench")         == 0) { opt_bench      = 1; argi++; }
        else break;
    }
    while (argi < argc && argv[argi][0] == '-' && argv[argi][1] != '-' && argv[argi][1] != '\0' && strchr("sdimo", argv[argi][1])) {
        char sw = argv[argi][1]; const char *rest = argv[argi] + 2; long v;
        if (sw == 'o') { if (*rest == '\0') { if (argi + 1 >= argc) { fprintf(stderr, "scrip: -o needs a filename\n"); return 2; } rest = argv[++argi]; } output_path = rest; argi++; continue; }   /* m4 -o <file> (this session): redirect --compile asm output to file; both -o FILE and -oFILE accepted; string arg, not a memory size */
        if (*rest == '\0') { if (argi + 1 >= argc) { fprintf(stderr, "scrip: -%c needs a value\n", sw); return 2; } rest = argv[++argi]; }
        v = parse_mem_arg(rest); if (v < 0) { fprintf(stderr, "scrip: bad -%c value '%s' (want e.g. 256m, 20m, 65536)\n", sw, rest); return 2; }
        if (sw == 's') { if (apply_stack_limit(v) != 0) { fprintf(stderr, "scrip: -s%ld: could not raise stack limit\n", v); return 2; } }
        else if (sw == 'm') { extern long g_maxlngth; g_maxlngth = v; }
        argi++;
    }
    for (int oi = argi; oi < argc; oi++) { if (strcmp(argv[oi], "--") == 0) break; if (argv[oi][0] == '-' && argv[oi][1] == 'o') { int eat = 1; if (argv[oi][2] != '\0') output_path = argv[oi] + 2; else { if (oi + 1 >= argc || strcmp(argv[oi+1], "--") == 0) { fprintf(stderr, "scrip: -o needs a filename\n"); return 2; } output_path = argv[oi+1]; eat = 2; } for (int mj = oi; mj + eat < argc; mj++) argv[mj] = argv[mj + eat]; argc -= eat; oi--; } }   /* m4 -o POSITION-INDEPENDENT (OPS-1): accept -o FILE / -oFILE after the source file too (bb_probe_matrix.sh call shape); path recorded, slots compacted out so the extension sniff and per-file loops never see them; sweep stops at -- so program argv is untouched */
    int mode_compile_x86 = (mode_compile && target_name && strcmp(target_name, "x86") == 0);
    if (mode_compile_x86 && mode_run) {
        fprintf(stderr, "scrip: --compile (x86) is mutually exclusive with --run\n");
        return 1;
    }
    if (!mode_run && !mode_compile)
        mode_run = 1;
    if (argi >= argc) {
        fprintf(stderr,
            "usage: scrip [mode] [options] <file> [-- program-args...]\n"
            "\n"
            "Execution modes (default: --run):\n"
            "  --run            build flat-wired x86 BB blobs in a sealed slab and jump in  [DEFAULT]\n"
            "  --compile        emit standalone x86-64 asm to stdout (links libscrip_rt.so)\n"
            "  --target=ARCH    emit code for the named backend (x86, jvm, js, wasm); implies --compile\n"
            "\n"
            "Diagnostic options:\n"
            "  --dump-ast       print AST after frontend\n"
            "  --dump-ir        print IR/BB-graph for each proc (terse: slot/op refs only)\n"
            "  --dump-bb        print the Byrd-box graph as JSON (boxes + gamma/omega port edges) for tools/bb_viewer.html\n"
            "  --dump-ir-verbose  same, plus node-id alongside each slot and the legend line\n"
            "  --dump-zeta      print the ZB-2 zeta layout table: scope tree, typed field maps, vslots (post-optimizer)\n"
            "  --transpile      transpile AST to portable SNOBOL4 source\n"
            "  --bench          print wall-clock time after execution\n"
            "\n"
            "Memory options (SPITBOL-compatible; value may end in k or m, e.g. -s256m -m8m):\n"
            "  -sN              max stack space; raises RLIMIT_STACK for deep pattern backtracking (default: OS, 8m)\n"
            "  -mN              max object size -> &MAXLNGTH (default 5m)\n"
            "  -dN -iN          accepted for SPITBOL invocation compatibility (SCRIP's GC arena is not byte-sized)\n"
            "\n"
            "Frontend inferred from file extension:\n"
            "  .sno=SNOBOL4  .icn=Icon  .pl=Prolog  .sc=Snocone  .reb=Rebus\n"
        );
        return 1;
    }
    extern void sno_add_include_dir(const char *d);
    struct timespec _t0, _t1, _t2, _t3;
    if (opt_bench) clock_gettime(CLOCK_MONOTONIC, &_t0);
    int first_file_argi = argi; (void)first_file_argi;
    int has_non_sno = 0;
    int is_prolog = 0;
    int is_icon = 0;
    int is_raku = 0;
    int is_pascal = 0;
    int saw_sno = 0;
    int is_scrip = 0;
    for (int fi = argi; fi < argc; fi++) {
        if (strcmp(argv[fi], "--") == 0) break;
        const char *d = strrchr(argv[fi], '.');
        if (d && (strcmp(d,".pl")==0 || strcmp(d,".icn")==0 ||
                  strcmp(d,".raku")==0 || strcmp(d,".reb")==0 ||
                  strcmp(d,".sc")==0 || strcmp(d,".scrip")==0 || strcmp(d,".md")==0))
            has_non_sno = 1;
        if (d && (strcmp(d,".scrip")==0 || strcmp(d,".md")==0)) is_scrip = 1;
        if (d && strcmp(d,".pl")==0) is_prolog = 1;
        if (d && strcmp(d,".icn")==0) is_icon = 1;
        if (d && strcmp(d,".raku")==0) is_raku = 1;
        if (d && strcmp(d,".pas")==0) is_pascal = 1;
        if (!d || strcmp(d,".sno")==0 || strcmp(d,".sc")==0 || strcmp(d,".reb")==0) saw_sno = 1;
    }
    int is_sno_bb = (saw_sno || is_scrip) && !is_pascal;
    lower_seg_t segs[64];
    int nsegs = 0;
    tree_t  *ast_prog = NULL;
    #define RECORD_SEG(sub_ast, seg_fn) do { \
        if ((sub_ast) && nsegs < 64) { \
            tree_t *_sp = calloc(1, sizeof(tree_t)); \
            if (_sp) { _sp->t = TT_PROGRAM; \
                for (int _si = 0; _si < (sub_ast)->n; _si++) if ((sub_ast)->c[_si]) ast_push(_sp, (sub_ast)->c[_si]); \
                segs[nsegs].prog = _sp; segs[nsegs].fn = (seg_fn); nsegs++; } } \
    } while(0)
    #define MERGE_AST(sub_ast) do { \
        if (sub_ast) { \
            if (!ast_prog) { ast_prog = sub_ast; } \
            else { \
                if (ast_prog->n > 0) { \
                    tree_t *_last = ast_prog->c[ast_prog->n-1]; \
                    if (_last && _last->t == TT_END) ast_prog->n--; \
                } \
                for (int _i = 0; _i < (sub_ast)->n; _i++) { \
                    ast_push(ast_prog, (sub_ast)->c[_i]); \
                } \
                if ((sub_ast)->c) free((char *)(sub_ast)->c - sizeof(size_t)); free(sub_ast); \
            } \
        } \
    } while(0)
    for (; argi < argc; argi++) {
        if (strcmp(argv[argi], "--") == 0) { argi++; g_prog_argv = &argv[argi]; g_prog_argc = argc - argi; break; }
        const char *input_path = argv[argi];
        {
            char rp[4096];
            const char *abs_path = realpath(input_path, rp) ? rp : input_path;
            char dirbuf[4096];
            strncpy(dirbuf, abs_path, sizeof dirbuf - 1);
            dirbuf[sizeof dirbuf - 1] = '\0';
            char *sl = strrchr(dirbuf, '/');
            if (sl) { *sl = '\0'; sno_add_include_dir(strdup(dirbuf)); }
            else     { sno_add_include_dir("."); }
            const char *core_lib = getenv("SNO_LIB");
            if (core_lib && *core_lib) {
                char envb[4096]; strncpy(envb, core_lib, sizeof envb - 1); envb[sizeof envb - 1] = '\0';
                char *sp = envb; char *tk;
                while ((tk = strsep(&sp, ":")) != (char *)0) if (*tk) sno_add_include_dir(strdup(tk));
            }
            char walk[4096];
            strncpy(walk, abs_path, sizeof walk - 1);
            walk[sizeof walk - 1] = '\0';
            char *p = strrchr(walk, '/');
            while (p) {
                *p = '\0';
                char probe[4096];
                snprintf(probe, sizeof probe, "%s/lib", walk);
                struct stat st;
                if (stat(probe, &st) == 0 && S_ISDIR(st.st_mode)) {
                    sno_add_include_dir(strdup(walk));
                    break;
                }
                p = strrchr(walk, '/');
            }
            sno_add_include_dir(".");
        }
        const char *dot = strrchr(input_path, '.');
        int lang_snocone  = dot && strcmp(dot, ".sc")   == 0;
        int lang_prolog   = dot && strcmp(dot, ".pl")   == 0;
        int lang_icon     = dot && strcmp(dot, ".icn")  == 0;
        int lang_raku     = dot && strcmp(dot, ".raku") == 0;
        int lang_rebus    = dot && strcmp(dot, ".reb")  == 0;
        int lang_pascal   = dot && strcmp(dot, ".pas")  == 0;
        int lang_polyglot = dot && (strcmp(dot, ".scrip") == 0 || strcmp(dot, ".md") == 0);
        if (lang_polyglot) {
            g_polyglot = 1;
            FILE *f = fopen(input_path, "r");
            if (!f) { fprintf(stderr, "scrip: cannot open '%s'\n", input_path); return 1; }
            fseek(f, 0, SEEK_END); long flen = ftell(f); rewind(f);
            char *src = malloc(flen + 1);
            if (!src) { fprintf(stderr, "scrip: out of memory\n"); return 1; }
            fread(src, 1, flen, f); src[flen] = '\0'; fclose(f);
            tree_t *sub_ast = parse_scrip_polyglot(src, input_path, segs, &nsegs, 64);
            free(src);
            MERGE_AST(sub_ast);
        } else if (lang_snocone || lang_prolog || lang_icon || lang_raku || lang_rebus || lang_pascal) {
            FILE *f = fopen(input_path, "r");
            if (!f) { fprintf(stderr, "scrip: cannot open '%s'\n", input_path); return 1; }
            fseek(f, 0, SEEK_END); long flen = ftell(f); rewind(f);
            char *src = malloc(flen + 1);
            if (!src) { fprintf(stderr, "scrip: out of memory\n"); return 1; }
            fread(src, 1, flen, f); src[flen] = '\0'; fclose(f);
            tree_t *sub_ast = NULL;
            if (lang_icon)         icon_compile(src, input_path, &sub_ast);
            else if (lang_raku)    raku_compile(src, input_path, &sub_ast);
            else if (lang_prolog)  prolog_compile(src, input_path, &sub_ast);
            else if (lang_rebus)   rebus_compile(src, input_path, &sub_ast);
            else if (lang_pascal)  pascal_compile(src, input_path, &sub_ast);
            else                   snocone_compile(src, input_path, &sub_ast);
            free(src);
            if (dump_ast && sub_ast) {
                ir_dump_program(sub_ast, stdout); return 0;
            }
            lower_entry_fn seg_fn = lower_sno_stage2;
            if      (lang_pascal) seg_fn = lower_pascal_stage2;
            else if (lang_icon)   seg_fn = lower_icon_stage2;
            else if (lang_prolog) seg_fn = lower_pl_stage2;
            else if (lang_raku)   seg_fn = lower_raku_stage2;
            RECORD_SEG(sub_ast, seg_fn);
            MERGE_AST(sub_ast);
        } else if (dump_ast) {
            FILE *f = fopen(input_path, "r");
            if (!f) { fprintf(stderr, "scrip: cannot open '%s'\n", input_path); return 1; }
            if (opt_bench) clock_gettime(CLOCK_MONOTONIC, &_t1);
            tree_t *sub_ast = sno_parse_ast(f, input_path, NULL);
            fclose(f);
            ir_dump_program(sub_ast, stdout);
            return 0;
        } else if (dump_transpile) {
            FILE *f = fopen(input_path, "r");
            if (!f) { fprintf(stderr, "scrip: cannot open '%s'\n", input_path); return 1; }
            if (opt_bench) clock_gettime(CLOCK_MONOTONIC, &_t1);
            tree_t *sub_ast = sno_parse_ast(f, input_path, NULL);
            fclose(f);
            RECORD_SEG(sub_ast, lower_sno_stage2);
            MERGE_AST(sub_ast);
        } else {
            FILE *f = fopen(input_path, "r");
            if (!f) { fprintf(stderr, "scrip: cannot open '%s'\n", input_path); return 1; }
            { extern void stmt_src_set_file(const char *); stmt_src_set_file(input_path); }
            tree_t *sub_ast = sno_parse_ast(f, input_path, NULL);
            fclose(f);
            RECORD_SEG(sub_ast, lower_sno_stage2);
            MERGE_AST(sub_ast);
        }
        if (!ast_prog) {
            fprintf(stderr, "scrip: parse failed for '%s'\n", input_path);
            return 1;
        }
        { extern int sno_nerrors; if (sno_nerrors > 0) { fprintf(stderr, "scrip: %d parse error(s) in '%s' -- no code generated\n", sno_nerrors, input_path); return 1; } }
    }
    if (nsegs == 1) segs[0].prog = ast_prog;
    if (opt_bench) clock_gettime(CLOCK_MONOTONIC, &_t1);
    const char *input_path = argv[argc - 1];
    if (opt_bench) clock_gettime(CLOCK_MONOTONIC, &_t2);
    if (!ast_prog) {
        fprintf(stderr, "scrip: parse failed for '%s'\n", input_path);
        return 1;
    }
    {
        extern void bb_pool_init(void);
        bb_pool_init();
        { extern void pat_pool_init(void); pat_pool_init(); }
    }
    setvbuf(stdout, NULL, _IOLBF, 0);
    extern void core_lib_init(void);
    core_lib_init();
    stmt_init();
    register_fn("IDENT",  _builtin_IDENT,  1, 2);
    register_fn("DIFFER", _builtin_DIFFER, 1, 2);
    register_fn("EVAL",   _builtin_EVAL,   1, 1);
    register_fn("CODE",   _builtin_CODE,   1, 1);
    register_fn("DATA",   _builtin_DATA,   1, 1);
    register_fn("print",  _builtin_print,  0, 99);
    extern DESCR_t (*g_user_call_hook)(const char *, DESCR_t *, int);
    g_user_call_hook = _usercall_hook;
    {
        extern void core_set_label_exists_hook(int (*fn)(const char *));
        core_set_label_exists_hook(_label_exists_fn);
    }
    {
        extern DESCR_t (*g_eval_pat_hook)(DESCR_t pat);
        g_eval_pat_hook = _eval_pat_impl_fn;
    }
    {
        extern DESCR_t (*g_eval_str_hook)(const char *s);
        g_eval_str_hook = _eval_str_impl_fn;
    }
    g_opt_dump_bb = dump_ir;
    if (dump_transpile) {
        extern int tree_to_sno(const tree_t *ast, FILE *out);
        tree_to_sno(ast_prog, stdout);
        return 0;
    }
    if (dump_ir || dump_zeta) {
        extern void bb_print_v(const IR_graph_t * bbg, FILE * fp, int verbose);
        extern void ir_drive_slot_assign(IR_graph_t * g);
        extern void zls_graph_name(const IR_graph_t * g, const char * name);
        extern void zls_dump(FILE * fp);
        extern int g_postfix_resume;
        if (is_icon) g_postfix_resume = 1;
        stage2_t *s2 = sm_preamble(ast_prog, segs, nsegs);
        if (!s2) { fprintf(stderr, "scrip: sm_preamble failed\n"); return 1; }
        ast_tree_free(ast_prog); ast_prog = NULL;
        if (dump_zeta && (is_icon || is_sno_bb)) { extern void optimizer_run(IR_graph_t * g); for (int _gi = 0; _gi < s2->bbp.count; _gi++) if (s2->bbp.table[_gi]) optimizer_run(s2->bbp.table[_gi]); }
        const IR_t ** seen_all = (const IR_t **) calloc(s2->proc_count > 0 ? s2->proc_count : 1, sizeof(const IR_t *));
        int seen_n = 0;
        for (int _pi = 0; _pi < s2->proc_count; _pi++) {
            int idx = s2->proc_table[_pi].bb_idx;
            const char *pname = s2->proc_table[_pi].name ? s2->proc_table[_pi].name : "?";
            if (idx < 0 || idx >= s2->bbp.count || !s2->bbp.table[idx]) continue;
            const IR_t ** all = (const IR_t **) s2->bbp.table[idx]->all;
            int dup = 0;
            for (int s = 0; s < seen_n; s++) if (seen_all[s] == (const IR_t *) all) { dup = 1; break; }
            if (dup) continue;
            seen_all[seen_n++] = (const IR_t *) all;
            if (dump_ir) fprintf(stdout, "; proc %s\n", pname);
            if (is_icon || is_sno_bb) zls_graph_name(s2->bbp.table[idx], pname);
            ir_drive_slot_assign(s2->bbp.table[idx]);
            if (dump_ir) bb_print_v(s2->bbp.table[idx], stdout, dump_ir_verbose);
        }
        if (dump_zeta) zls_dump(stdout);
        free(seen_all);
        return 0;
    }
    if (dump_bb) {
        extern void optimizer_run(IR_graph_t * g);
        extern const char * bb_src_of(const IR_t * nd);
        extern int g_postfix_resume;
        if (is_icon) g_postfix_resume = 1;
        stage2_t *s2 = sm_preamble(ast_prog, segs, nsegs);
        if (!s2) { fprintf(stderr, "scrip: sm_preamble failed\n"); return 1; }
        ast_tree_free(ast_prog); ast_prog = NULL;
        if (is_icon || is_sno_bb || is_prolog) for (int _gi = 0; _gi < s2->bbp.count; _gi++) if (s2->bbp.table[_gi]) optimizer_run(s2->bbp.table[_gi]);
        const IR_graph_t ** gset = (const IR_graph_t **) calloc(s2->proc_count > 0 ? s2->proc_count : 1, sizeof(const IR_graph_t *));
        const char ** gname = (const char **) calloc(s2->proc_count > 0 ? s2->proc_count : 1, sizeof(const char *));
        int gn = 0;
        for (int _pi = 0; _pi < s2->proc_count; _pi++) {
            int idx = s2->proc_table[_pi].bb_idx;
            if (idx < 0 || idx >= s2->bbp.count || !s2->bbp.table[idx]) continue;
            int dup = 0;
            for (int s = 0; s < gn; s++) if (gset[s] == (const IR_graph_t *) s2->bbp.table[idx]) { dup = 1; break; }
            if (dup) continue;
            gname[gn] = s2->proc_table[_pi].name ? s2->proc_table[_pi].name : "?";
            gset[gn++] = s2->bbp.table[idx];
        }
        fputs("{\"meta\":{\"program\":", stdout); bbj_str(stdout, input_path ? input_path : "?");
        fputs(",\"generator\":\"scrip --dump-bb\"},\n \"boxes\":[", stdout);
        int first = 1, stno = 0;
        for (int gi = 0; gi < gn; gi++) {
            const IR_graph_t * g = gset[gi];
            int * stno_of = (int *) calloc(g->n > 0 ? g->n : 1, sizeof(int));
            const char ** src_of = (const char **) calloc(g->n > 0 ? g->n : 1, sizeof(const char *));
            int pend = 0;
            for (int i = 0; i < g->n; i++) {
                if (!g->all[i]) continue;
                const char * src = bb_src_of(g->all[i]);
                if (!src) continue;
                stno++;
                for (int j = pend; j <= i; j++) if (g->all[j]) stno_of[j] = stno;
                src_of[i] = src;
                pend = i + 1;
            }
            for (int i = 0; i < g->n; i++) {
                const IR_t * bb = g->all[i];
                if (!bb) continue;
                const char * opn = bb_op_name(bb->op);
                if (opn && !strncmp(opn, "IR_", 3)) opn += 3;
                fprintf(stdout, "%s\n  {\"id\":\"b%d_%d\",\"kind\":", first ? "" : ",", gi, i); first = 0;
                bbj_str(stdout, opn ? opn : "?");
                bbj_label(stdout, bb);
                if (stno_of[i] > 0) fprintf(stdout, ",\"stmt\":%d", stno_of[i]);
                if (gi > 0) { fputs(",\"proc\":", stdout); bbj_str(stdout, gname[gi]); }
                if (src_of[i]) { fputs(",\"src\":", stdout); bbj_str(stdout, src_of[i]); }
                fputs("}", stdout);
            }
            free(stno_of); free((void *) src_of);
        }
        fputs("],\n \"edges\":[", stdout);
        first = 1;
        if (gn > 0 && gset[0]->n > 0) {
            int e0 = gset[0]->entry ? bbj_index(gset[0], gset[0]->entry) : 0;
            fprintf(stdout, "\n  {\"from\":\"$start\",\"to\":\"b0_%d\",\"tp\":\"alpha\"}", e0 < 0 ? 0 : e0); first = 0;
        }
        for (int gi = 0; gi < gn; gi++) {
            const IR_graph_t * g = gset[gi];
            for (int i = 0; i < g->n; i++) {
                const IR_t * bb = g->all[i];
                if (!bb) continue;
                bbj_edge(stdout, &first, gi, i, "gamma", g, &bb->γ);
                bbj_edge(stdout, &first, gi, i, "omega", g, &bb->ω);
                for (int oj = 0; oj < bb->n_operands; oj++) {
                    int j = bb->operands && bb->operands[oj] ? bbj_index(g, bb->operands[oj]) : -1;
                    if (j < 0) continue;
                    fprintf(stdout, "%s\n  {\"from\":\"b%d_%d\",\"to\":\"b%d_%d\",\"rel\":\"op\",\"slot\":%d}", first ? "" : ",", gi, i, gi, j, oj);
                    first = 0;
                }
            }
        }
        fputs("]}\n", stdout);
        free(gset); free(gname);
        return 0;
    }
    if (mode_compile_x86) {
        extern int g_frame_active;
        if (is_icon || is_raku || is_sno_bb || is_prolog) {
            extern int g_postfix_resume;
            extern int g_m4_dense_nid; extern void g_bb_alpha_seq_reset(void);
            g_m4_dense_nid = 1; g_bb_alpha_seq_reset();
            if (is_icon) g_postfix_resume = 1;
            stage2_t *s2 = sm_preamble(ast_prog, segs, nsegs);
            if (!s2) return 1;
            ast_tree_free(ast_prog); ast_prog = NULL;
            if (is_icon || is_sno_bb || is_prolog) { extern void optimizer_run(IR_graph_t * g); for (int _gi = 0; _gi < s2->bbp.count; _gi++) if (s2->bbp.table[_gi]) optimizer_run(s2->bbp.table[_gi]); }
            extern void rt_proc_register(const char *name, const char **pnames, int nparams);
            extern void rt_proc_reset(void);
            int main_bb_idx = -1;
            rt_proc_reset();
            for (int _pi = 0; _pi < s2->proc_count; _pi++) {
                const char *pname = s2->proc_table[_pi].name;
                if (!pname) continue;
                if (strcmp(pname, "main") == 0) { main_bb_idx = s2->proc_table[_pi].bb_idx; continue; }
                int idx = s2->proc_table[_pi].bb_idx;
                if (idx < 0 || idx >= s2->bbp.count || !s2->bbp.table[idx] || !s2->bbp.table[idx]->entry) continue;
                int np = s2->proc_table[_pi].nparams;
                const char **pn = NULL;
                if (np > 0) {
                    pn = (const char **)calloc((size_t)np, sizeof(const char *));
                    for (int k = 0; k < np && k < s2->proc_table[_pi].lower_sc.n; k++)
                        pn[k] = s2->proc_table[_pi].lower_sc.e[k].name;
                }
                rt_proc_register(pname, pn, np);
                { extern void rt_proc_set_nformals(const char *, int); rt_proc_set_nformals(pname, s2->proc_table[_pi].nformals); }   /* NPSPLIT (s22w): 0 for unsplit frontends -> runtime falls back to nparams */
                { extern void rt_proc_set_generator(const char *, int); rt_proc_set_generator(pname, s2->proc_table[_pi].is_generator); } { extern void rt_proc_set_jmpentry(const char *, int); rt_proc_set_jmpentry(pname, strncmp(pname, "gram__", 6) != 0); }
                { extern void rt_proc_set_variadic(const char *, int); rt_proc_set_variadic(pname, s2->proc_table[_pi].is_variadic); }
                { extern void rt_proc_set_rest_kind(const char *, int); rt_proc_set_rest_kind(pname, s2->proc_table[_pi].rest_kind); }
                { extern void rt_proc_set_named_rest(const char *, int); rt_proc_set_named_rest(pname, s2->proc_table[_pi].named_rest); }
                { extern void rt_proc_set_dyn_scope(const char *, int); rt_proc_set_dyn_scope(pname, s2->proc_table[_pi].dyn_scope); }
                { extern void rt_proc_set_result_name(const char *, const char *); if (s2->proc_table[_pi].result_name) rt_proc_set_result_name(pname, s2->proc_table[_pi].result_name); }
            }
            if (is_icon || is_sno_bb || is_prolog || is_raku || is_pascal) drive_slots_all(s2);
            if (is_raku && !graph_native_emittable(s2)) {
                fprintf(stderr, "[SMX] --compile --target=x86: mode-4 native emitter does not yet cover "
                                "this program (a box has no MEDIUM_TEXT arm — Raku map/grep). REJECTED — native BB emission pending (no interpreter fallback).\n");
                return 0;
            }
            if (main_bb_idx < 0 || main_bb_idx >= s2->bbp.count || !s2->bbp.table[main_bb_idx] || !s2->bbp.table[main_bb_idx]->entry) {
                fprintf(stderr, "[IBB] FATAL: mode-4 driver: main BB graph not found\n");
                return 1;
            }
            IR_graph_t * bbg = s2->bbp.table[main_bb_idx];
            extern bb_box_fn emit_chain(IR_t * entry, FILE * out, const char * prefix);
            g_medium = BB_MEDIUM_TEXT; FILE * _out = stdout; if (output_path) { _out = fopen(output_path, "w"); if (!_out) { perror(output_path); return 1; } } emit_set_sink(_out);   /* m4 -o (06g fix): _out at path scope so the emit_chain body writers below take it too -- the 06f form captured only the emit_textf sink, the body spilled to stdout (measured: 27-line file, 343-line spill) */
            emit_textf("  .intel_syntax noprefix\n");
            emit_textf("  .text\n");
            g_frame_active = 1;
            extern void gva_collect_reset(void); extern void gva_collect_icon_globals(void); extern int gva_count(void); extern const char *gva_name(int); extern int g_gva_active;
            gva_collect_reset();
            gva_collect_icon_globals();
            int n_gva_icn = gva_count();
            g_gva_active = (n_gva_icn > 0) ? 1 : 0;
            int n_procs = 0;
            int _pnbcap = (s2->proc_count > 0) ? s2->proc_count : 1;
            const char **proc_names_buf = (const char **)malloc((size_t)_pnbcap * sizeof(const char *));
            int *proc_nparams_buf = (int *)malloc((size_t)_pnbcap * sizeof(int));
            int *proc_pidx_buf = (int *)malloc((size_t)_pnbcap * sizeof(int));
            int *proc_fb_buf = (int *)malloc((size_t)_pnbcap * sizeof(int));
            int *proc_ispat_buf = (int *)malloc((size_t)_pnbcap * sizeof(int));
            int *proc_zstatic_buf = (int *)malloc((size_t)_pnbcap * sizeof(int));
            /* PL-FR-4 NAME PRE-PASS: for Prolog files, register all graph names in ZLS BEFORE any graph is compiled,
             * so that zls_g_resume_by_name(callee_name) resolves correctly even when the callee is compiled AFTER
             * the caller in the emission order.  Icon/SN4 do this inline (line 913); Prolog needs a pre-pass because
             * multi-clause predicates mutually reference each other via call_proc_staged. */
            if (is_prolog) { extern void zls_graph_name(const IR_graph_t *, const char *); for (int _pi2 = 0; _pi2 < s2->proc_count; _pi2++) { const char *_pn2 = s2->proc_table[_pi2].name; if (!_pn2 || strcmp(_pn2, "main") == 0) continue; int _idx2 = s2->proc_table[_pi2].bb_idx; if (_idx2 >= 0 && _idx2 < s2->bbp.count && s2->bbp.table[_idx2]) zls_graph_name(s2->bbp.table[_idx2], _pn2); } }
            for (int _pi = 0; _pi < s2->proc_count; _pi++) {
                const char *pname = s2->proc_table[_pi].name;
                if (!pname || strcmp(pname, "main") == 0) continue;
                int idx = s2->proc_table[_pi].bb_idx;
                if (idx < 0 || idx >= s2->bbp.count || !s2->bbp.table[idx] || !s2->bbp.table[idx]->entry) continue;
                int np = s2->proc_table[_pi].nparams;
                const char **pn = NULL;
                if (np > 0) {
                    pn = (const char **)calloc((size_t)np, sizeof(const char *));
                    for (int k = 0; k < np && k < s2->proc_table[_pi].lower_sc.n; k++)
                        pn[k] = s2->proc_table[_pi].lower_sc.e[k].name;
                }
                { extern IR_graph_t *g_emit_cfg; g_emit_cfg = s2->bbp.table[idx]; }
                { extern int g_gen_proc_active; g_gen_proc_active = s2->proc_table[_pi].is_generator; }
                { extern int g_flat_frame_floor; extern int zls_g_region(const IR_graph_t *); IR_graph_t *_pg = s2->bbp.table[idx]; int _is_lbl = pname && strncmp(pname, "LBL__", 5) == 0; g_flat_frame_floor = 0; if (_is_lbl || (_pg && _pg->entry && _pg->entry->op == IR_SAVE_RESTORE && IR_LIT(_pg->entry).ival == 3)) { for (int _mi = 0; _mi < s2->proc_count; _mi++) if (s2->proc_table[_mi].name && !strcmp(s2->proc_table[_mi].name, "main")) { int _mx = s2->proc_table[_mi].bb_idx; if (_mx >= 0 && _mx < s2->bbp.count && s2->bbp.table[_mx]) g_flat_frame_floor = zls_g_region(s2->bbp.table[_mx]); break; } } }   /* SN4-FLAT-PROC (s176): DEFINE stubs + LBL__ body procs both need CLASS P wire exit; LBL__ bodies share main's bb_idx so _pg->entry is GOTO not SAVE_RESTORE -- _is_lbl closes that gap (twin of the m3 fix) */
                { extern int emit_jmp_entry_for_patproc(const char*, IR_graph_t*); extern int emit_jmp_entry_for_proc(const char*, int, int, IR_graph_t*); extern void emit_jmp_entry_clear(void); extern int g_flat_dc_np; extern int rt_pl_dc_ok(const char *, int);
                  int _isp = emit_jmp_entry_for_patproc(pname, s2->bbp.table[idx]); if (!_isp) emit_jmp_entry_for_proc(pname, s2->proc_table[_pi].dyn_scope, s2->proc_table[_pi].is_generator, s2->bbp.table[idx]);
                  g_flat_dc_np = (!_isp && rt_pl_dc_ok(pname, np)) ? np : -1; proc_ispat_buf[n_procs] = _isp; }   /* PL-DC s108: arm the direct-call stub for this graph iff the SAME table predicate the site arm reads passes (pat blobs excluded structurally); s112: RECORD the structural exclusion so the startup bake mirrors it — the bake predicate must equal the arming predicate or it references stubs that were never emitted (treebank m4 link regression) */
                { if (is_icon || is_sno_bb || is_prolog) { extern void zls_graph_name(const IR_graph_t *, const char *); zls_graph_name(s2->bbp.table[idx], pname); } }   /* ICN-FR-4: name the graph in the zls registry BEFORE emit_chain so zls_g_resume_by_name(callee_name) works at later call sites in bb_call_proc_staged.  Mirrors dump_ir path. Harmless for non-generators (resume_off stays -1). PL-FR-4: added is_prolog so Prolog generator callee names resolve for pl_zf_resume cursor_off lookup. */
                { char _pfx[256]; snprintf(_pfx, sizeof(_pfx), "proc_%s", asm_sym_name(pname)); int _islbl = pname && strncmp(pname, "LBL__", 5) == 0; IR_graph_t *_bg = s2->bbp.table[idx]; int _bare = (proc_role3_kind(_bg) == 1);   /* ⭐ EXPR-THUNK EXITS (s96): BARE = the DEFINE stub blob ONLY (kind 1); a self-contained EXPR$ thunk (kind 2) keeps its own wire γ/ω exits -- see proc_role3_kind */ if (!_islbl) { emit_sep_rule_c('-'); emit_textf("  .globl %s_\xce\xb1\n", _pfx); if (scrip_symmap()) emit_textf("  .type %s_\xce\xb1, @function\n", _pfx); g_emit.flat_bare_chain = _bare; emit_chain(bb_proc_entry(&s2->proc_table[_pi]), _out, _pfx); g_emit.flat_bare_chain = 0; if (scrip_symmap()) emit_textf("  .size %s_\xce\xb1, .-%s_\xce\xb1\n", _pfx, _pfx); } }   /* SN4-FLAT-PROC (s176): bb_proc_entry, NOT ->entry -- a shared-graph proc (LBL__/DEFINE entry) must bind its α at proc_entry_node; binding at main's entry made the stub's transfer re-run the whole program inside the call (the m4 recursion SEGV) */   /* ⭐⭐⭐ STATEMENT-ORDER (Lon s62 in-chat: "You process each STATEMENT, ONE at a TIME, and in ORDER of the source."): the standalone LBL__ emission is DELETED — it walked the reachable-from-entry-label slice of main's ONE shared graph and laid it down pre-main, which is exactly the out-of-order hoist Lon condemned.  Those statements now emit inside main's own chain at their source position (emit.cpp statement-order seeding), where the entry statement's α is renamed <FN>_body (BODY-ALIAS via bbg->balias below).  The proc_table row STAYS (registration, rt_goto_transfer arm-4 lookup, scc probes all read it); only its separate emission dies.  BARE-CHAIN (s62): role-3 DEFINE stubs keep emitting, without the proc_* wrapper family. */
                { extern void emit_jmp_entry_clear(void); emit_jmp_entry_clear(); }
                { extern int g_gen_proc_active; g_gen_proc_active = 0; }
                { extern int g_last_flat_frame_bytes; proc_fb_buf[n_procs] = (pname && strncmp(pname, "LBL__", 5) == 0) ? 0 : g_last_flat_frame_bytes; }   /* STATEMENT-ORDER (s62): LBL__ rows no longer emit standalone — no fresh measurement exists, and main's geometry governs the shared statements; 0 skips the set_frame_bytes bake */
                { extern int g_last_flat_zstatic; proc_zstatic_buf[n_procs] = (pname && strncmp(pname, "LBL__", 5) == 0) ? 0 : g_last_flat_zstatic; }
                { extern int g_last_flat_frame_bytes, g_last_flat_fp, g_last_flat_uniform; extern void emit_patzeta_register(const char *, int, int, int); if (!(pname && strncmp(pname, "LBL__", 5) == 0)) emit_patzeta_register(pname, g_last_flat_frame_bytes, g_last_flat_fp, g_last_flat_uniform); }   /* PS-3 (s152): emit-side Î¶-size registry feed -- suspension footprint terms for DT_P targets, both modes, before main emission */
                proc_nparams_buf[n_procs] = np;
                proc_pidx_buf[n_procs] = _pi;
                proc_names_buf[n_procs++] = pname ? strdup(pname) : NULL;   /* s62 SAFETY: strdup so the name pointer survives any arena reset between the proc loop and the post-main backfill at line 1309 */
                free(pn);
            }
            int n_cls_emit = 0;
            { extern int dat_type_count(void); n_cls_emit = dat_type_count(); }
            int n_gram_emit = 0;
            { extern int rt_grammar_count(void); n_gram_emit = rt_grammar_count(); }
            if (n_procs > 0 || n_cls_emit > 0 || n_gram_emit > 0) {
                emit_textf("proc_startup:\n");
                emit_textf("  sub rsp, 8\n");
                { extern int dat_type_count(void); extern const char *dat_type_name(int); extern int dat_type_nfields(int); extern const char *dat_type_field(int, int);
                  int n_cls = dat_type_count();
                  for (int ci = 0; ci < n_cls; ci++) {
                      const char *cn = dat_type_name(ci); if (!cn || !*cn) continue;
                      emit_textf("  .section .rodata\n");
                      emit_textf("  .Lclassspec%d: .string \"%s(", ci, cn);
                      for (int fj = 0; fj < dat_type_nfields(ci); fj++) { if (fj) emit_textf(","); emit_textf("%s", dat_type_field(ci, fj)); }
                      emit_textf(")\"\n");
                      emit_textf("  .section .text\n  .intel_syntax noprefix\n");
                      emit_textf("  lea rdi, [rip + .Lclassspec%d]\n", ci);
                      emit_textf("  call record_register@PLT\n");
                  } }
                { extern int dat_type_count(void); extern const char *dat_type_name(int); extern int dat_type_nparents(int); extern const char *dat_type_parent_at(int, int);
                  int n_cls = dat_type_count();
                  for (int ci = 0; ci < n_cls; ci++) {
                      const char *cn = dat_type_name(ci); if (!cn || !*cn) continue;
                      int np = dat_type_nparents(ci); if (np <= 0) continue;
                      emit_textf("  .section .rodata\n");
                      emit_textf("  .Lclschild%d: .string \"%s\"\n", ci, cn);
                      for (int pj = 0; pj < np; pj++) emit_textf("  .Lclsp%d_%d: .string \"%s\"\n", ci, pj, dat_type_parent_at(ci, pj));
                      emit_textf("  .balign 8\n  .Lclsparr%d:\n", ci);
                      for (int pj = 0; pj < np; pj++) emit_textf("  .quad .Lclsp%d_%d\n", ci, pj);
                      emit_textf("  .section .text\n  .intel_syntax noprefix\n");
                      emit_textf("  lea rdi, [rip + .Lclschild%d]\n", ci);
                      emit_textf("  lea rsi, [rip + .Lclsparr%d]\n", ci);
                      emit_textf("  mov rdx, %d\n", np);
                      emit_textf("  call class_inherit_multi@PLT\n");
                  } }
                { extern int dat_type_count(void); extern const char *dat_type_name(int); extern int dat_type_nfields(int); extern const char *dat_type_field(int, int);
                  extern int dat_type_field_has_default(int, int); extern DESCR_t dat_type_field_default(int, int);
                  int n_cls = dat_type_count();
                  for (int ci = 0; ci < n_cls; ci++) {
                      const char *cn = dat_type_name(ci); if (!cn || !*cn) continue;
                      for (int fj = 0; fj < dat_type_nfields(ci); fj++) {
                          if (!dat_type_field_has_default(ci, fj)) continue;
                          const char *fn = dat_type_field(ci, fj); if (!fn) continue;
                          DESCR_t dv = dat_type_field_default(ci, fj);
                          emit_textf("  .section .rodata\n");
                          emit_textf("  .Ldefcls%d_%d: .byte ", ci, fj); for (const char *p = cn; *p; p++) emit_textf("%d, ", (int)(unsigned char)*p); emit_textf("0\n");
                          emit_textf("  .Ldeffld%d_%d: .byte ", ci, fj); for (const char *p = fn; *p; p++) emit_textf("%d, ", (int)(unsigned char)*p); emit_textf("0\n");
                          if (dv.v == DT_S) {
                              const char *sv = dv.s ? dv.s : ""; emit_textf("  .Ldefstr%d_%d: .byte ", ci, fj); for (const char *p = sv; *p; p++) emit_textf("%d, ", (int)(unsigned char)*p);
                              emit_textf("0\n");
                          }
                          else if (dv.v == DT_R) { union { double d; unsigned long long q; } u; u.d = dv.r; emit_textf("  .Ldefdbl%d_%d: .quad %llu\n", ci, fj, u.q); }
                          emit_textf("  .section .text\n  .intel_syntax noprefix\n");
                          emit_textf("  lea rdi, [rip + .Ldefcls%d_%d]\n", ci, fj);
                          emit_textf("  lea rsi, [rip + .Ldeffld%d_%d]\n", ci, fj);
                          if (dv.v == DT_S) { emit_textf("  lea rdx, [rip + .Ldefstr%d_%d]\n", ci, fj); emit_textf("  call dat_set_field_default_s@PLT\n"); }
                          else if (dv.v == DT_R) { emit_textf("  movsd xmm0, qword ptr [rip + .Ldefdbl%d_%d]\n", ci, fj); emit_textf("  call dat_set_field_default_r@PLT\n"); }
                          else { emit_textf("  mov rdx, %lld\n", (long long)dv.i); emit_textf("  call dat_set_field_default_i@PLT\n"); }
                      }
                  } }
                { extern int dat_type_count(void); extern const char *dat_type_name(int); extern int dat_type_nfields(int); extern const char *dat_type_field(int, int);
                  extern int dat_type_field_required(int, int);
                  int n_cls = dat_type_count();
                  for (int ci = 0; ci < n_cls; ci++) {
                      const char *cn = dat_type_name(ci); if (!cn || !*cn) continue;
                      for (int fj = 0; fj < dat_type_nfields(ci); fj++) {
                          if (!dat_type_field_required(ci, fj)) continue;
                          const char *fn = dat_type_field(ci, fj); if (!fn) continue;
                          emit_textf("  .section .rodata\n");
                          emit_textf("  .Lreqcls%d_%d: .byte ", ci, fj); for (const char *p = cn; *p; p++) emit_textf("%d, ", (int)(unsigned char)*p); emit_textf("0\n");
                          emit_textf("  .Lreqfld%d_%d: .byte ", ci, fj); for (const char *p = fn; *p; p++) emit_textf("%d, ", (int)(unsigned char)*p); emit_textf("0\n");
                          emit_textf("  .section .text\n  .intel_syntax noprefix\n");
                          emit_textf("  lea rdi, [rip + .Lreqcls%d_%d]\n", ci, fj);
                          emit_textf("  lea rsi, [rip + .Lreqfld%d_%d]\n", ci, fj);
                          emit_textf("  call dat_set_field_required@PLT\n");
                      }
                  } }
                { extern int dat_type_count(void); extern const char *dat_type_name(int); extern int dat_type_nfields(int); extern const char *dat_type_field(int, int);
                  extern int dat_type_field_rw(int, int);
                  int n_cls = dat_type_count();
                  for (int ci = 0; ci < n_cls; ci++) {
                      const char *cn = dat_type_name(ci); if (!cn || !*cn) continue;
                      for (int fj = 0; fj < dat_type_nfields(ci); fj++) {
                          if (!dat_type_field_rw(ci, fj)) continue;
                          const char *fn = dat_type_field(ci, fj); if (!fn) continue;
                          emit_textf("  .section .rodata\n");
                          emit_textf("  .Lrwcls%d_%d: .byte ", ci, fj); for (const char *p = cn; *p; p++) emit_textf("%d, ", (int)(unsigned char)*p); emit_textf("0\n");
                          emit_textf("  .Lrwfld%d_%d: .byte ", ci, fj); for (const char *p = fn; *p; p++) emit_textf("%d, ", (int)(unsigned char)*p); emit_textf("0\n");
                          emit_textf("  .section .text\n  .intel_syntax noprefix\n");
                          emit_textf("  lea rdi, [rip + .Lrwcls%d_%d]\n", ci, fj);
                          emit_textf("  lea rsi, [rip + .Lrwfld%d_%d]\n", ci, fj);
                          emit_textf("  call dat_set_field_rw@PLT\n");
                      }
                  } }
                { extern int dat_type_count(void); extern const char *dat_type_name(int); extern int dat_type_nfields(int); extern const char *dat_type_field(int, int);
                  extern int dat_type_field_sigil(int, int);
                  int n_cls = dat_type_count();
                  for (int ci = 0; ci < n_cls; ci++) {
                      const char *cn = dat_type_name(ci); if (!cn || !*cn) continue;
                      for (int fj = 0; fj < dat_type_nfields(ci); fj++) {
                          int sg = dat_type_field_sigil(ci, fj); if (sg != '@' && sg != '%') continue;
                          const char *fn = dat_type_field(ci, fj); if (!fn) continue;
                          emit_textf("  .section .rodata\n");
                          emit_textf("  .Lsigcls%d_%d: .byte ", ci, fj); for (const char *p = cn; *p; p++) emit_textf("%d, ", (int)(unsigned char)*p); emit_textf("0\n");
                          emit_textf("  .Lsigfld%d_%d: .byte ", ci, fj); for (const char *p = fn; *p; p++) emit_textf("%d, ", (int)(unsigned char)*p); emit_textf("0\n");
                          emit_textf("  .section .text\n  .intel_syntax noprefix\n");
                          emit_textf("  lea rdi, [rip + .Lsigcls%d_%d]\n", ci, fj);
                          emit_textf("  lea rsi, [rip + .Lsigfld%d_%d]\n", ci, fj);
                          emit_textf("  mov rdx, %d\n", sg);
                          emit_textf("  call dat_set_field_sigil@PLT\n");
                      }
                  } }
                { extern int dat_type_count(void); extern const char *dat_type_name(int); extern int dat_type_nfields(int); extern const char *dat_type_field(int, int);
                  extern int dat_type_field_priv(int, int);
                  int n_cls = dat_type_count();
                  for (int ci = 0; ci < n_cls; ci++) {
                      const char *cn = dat_type_name(ci); if (!cn || !*cn) continue;
                      for (int fj = 0; fj < dat_type_nfields(ci); fj++) {
                          if (!dat_type_field_priv(ci, fj)) continue;
                          const char *fn = dat_type_field(ci, fj); if (!fn) continue;
                          emit_textf("  .section .rodata\n");
                          emit_textf("  .Lprvcls%d_%d: .byte ", ci, fj); for (const char *p = cn; *p; p++) emit_textf("%d, ", (int)(unsigned char)*p); emit_textf("0\n");
                          emit_textf("  .Lprvfld%d_%d: .byte ", ci, fj); for (const char *p = fn; *p; p++) emit_textf("%d, ", (int)(unsigned char)*p); emit_textf("0\n");
                          emit_textf("  .section .text\n  .intel_syntax noprefix\n");
                          emit_textf("  lea rdi, [rip + .Lprvcls%d_%d]\n", ci, fj);
                          emit_textf("  lea rsi, [rip + .Lprvfld%d_%d]\n", ci, fj);
                          emit_textf("  call dat_set_field_priv@PLT\n");
                      }
                  } }
                { extern int dat_type_count(void); extern const char *dat_type_name(int); extern int dat_type_nmethods(int); extern const char *dat_type_method_at(int, int);
                  int n_cls = dat_type_count();
                  for (int ci = 0; ci < n_cls; ci++) {
                      const char *cn = dat_type_name(ci); if (!cn || !*cn) continue;
                      int nm = dat_type_nmethods(ci); if (nm <= 0) continue;
                      for (int mj = 0; mj < nm; mj++) {
                          const char *mn = dat_type_method_at(ci, mj); if (!mn || !*mn) continue;
                          emit_textf("  .section .rodata\n");
                          emit_textf("  .Lmethcls%d_%d: .byte ", ci, mj); for (const char *p = cn; *p; p++) emit_textf("%d, ", (int)(unsigned char)*p); emit_textf("0\n");
                          emit_textf("  .Lmethnm%d_%d: .byte ", ci, mj); for (const char *p = mn; *p; p++) emit_textf("%d, ", (int)(unsigned char)*p); emit_textf("0\n");
                          emit_textf("  .section .text\n  .intel_syntax noprefix\n");
                          emit_textf("  lea rdi, [rip + .Lmethcls%d_%d]\n", ci, mj);
                          emit_textf("  lea rsi, [rip + .Lmethnm%d_%d]\n", ci, mj);
                          emit_textf("  call dat_add_method@PLT\n");
                      }
                  } }
                { extern int dat_type_count(void); extern const char *dat_type_name(int); extern int dat_type_has_build(int);
                  extern int dat_type_nbuild_keys(int); extern const char *dat_type_build_key_at(int, int);
                  int n_cls = dat_type_count();
                  for (int ci = 0; ci < n_cls; ci++) {
                      const char *cn = dat_type_name(ci); if (!cn || !*cn) continue;
                      if (!dat_type_has_build(ci)) continue;
                      emit_textf("  .section .rodata\n");
                      emit_textf("  .Lbldcls%d: .byte ", ci); for (const char *p = cn; *p; p++) emit_textf("%d, ", (int)(unsigned char)*p); emit_textf("0\n");
                      emit_textf("  .Lbldnull%d: .byte 0\n", ci);
                      emit_textf("  .section .text\n  .intel_syntax noprefix\n");
                      emit_textf("  lea rdi, [rip + .Lbldcls%d]\n", ci);
                      emit_textf("  lea rsi, [rip + .Lbldnull%d]\n", ci);
                      emit_textf("  call dat_set_build_key@PLT\n");
                      int nk = dat_type_nbuild_keys(ci);
                      for (int kj = 0; kj < nk; kj++) {
                          const char *kn = dat_type_build_key_at(ci, kj); if (!kn || !*kn) continue;
                          emit_textf("  .section .rodata\n");
                          emit_textf("  .Lbldkey%d_%d: .byte ", ci, kj); for (const char *p = kn; *p; p++) emit_textf("%d, ", (int)(unsigned char)*p); emit_textf("0\n");
                          emit_textf("  .section .text\n  .intel_syntax noprefix\n");
                          emit_textf("  lea rdi, [rip + .Lbldcls%d]\n", ci);
                          emit_textf("  lea rsi, [rip + .Lbldkey%d_%d]\n", ci, kj);
                          emit_textf("  call dat_set_build_key@PLT\n");
                      }
                  } }
                { extern int dat_type_count(void); extern const char *dat_type_name(int); extern int dat_type_nhandles(int);
                  extern const char *dat_type_handles_meth_at(int, int); extern const char *dat_type_handles_fld_at(int, int);
                  int n_cls = dat_type_count();
                  for (int ci = 0; ci < n_cls; ci++) {
                      const char *cn = dat_type_name(ci); if (!cn || !*cn) continue;
                      int nh = dat_type_nhandles(ci); if (nh <= 0) continue;
                      for (int hj = 0; hj < nh; hj++) {
                          const char *hm = dat_type_handles_meth_at(ci, hj); const char *hf = dat_type_handles_fld_at(ci, hj);
                          if (!hm || !*hm || !hf || !*hf) continue;
                          emit_textf("  .section .rodata\n");
                          emit_textf("  .Lhndcls%d_%d: .byte ", ci, hj); for (const char *p = cn; *p; p++) emit_textf("%d, ", (int)(unsigned char)*p); emit_textf("0\n");
                          emit_textf("  .Lhndmeth%d_%d: .byte ", ci, hj); for (const char *p = hm; *p; p++) emit_textf("%d, ", (int)(unsigned char)*p); emit_textf("0\n");
                          emit_textf("  .Lhndfld%d_%d: .byte ", ci, hj); for (const char *p = hf; *p; p++) emit_textf("%d, ", (int)(unsigned char)*p); emit_textf("0\n");
                          emit_textf("  .section .text\n  .intel_syntax noprefix\n");
                          emit_textf("  lea rdi, [rip + .Lhndcls%d_%d]\n", ci, hj);
                          emit_textf("  lea rsi, [rip + .Lhndmeth%d_%d]\n", ci, hj);
                          emit_textf("  lea rdx, [rip + .Lhndfld%d_%d]\n", ci, hj);
                          emit_textf("  call dat_add_handles@PLT\n");
                      }
                  } }
                { extern int dat_type_count(void); extern const char *dat_type_name(int); extern int dat_type_nroles(int); extern const char *dat_type_role_at(int, int);
                  int n_cls = dat_type_count();
                  for (int ci = 0; ci < n_cls; ci++) {
                      const char *cn = dat_type_name(ci); if (!cn || !*cn) continue;
                      int nr = dat_type_nroles(ci); if (nr <= 0) continue;
                      for (int rj = 0; rj < nr; rj++) {
                          const char *rn = dat_type_role_at(ci, rj); if (!rn || !*rn) continue;
                          emit_textf("  .section .rodata\n");
                          emit_textf("  .Lrolechild%d_%d: .byte ", ci, rj); for (const char *p = cn; *p; p++) emit_textf("%d, ", (int)(unsigned char)*p); emit_textf("0\n");
                          emit_textf("  .Lrolename%d_%d: .byte ", ci, rj); for (const char *p = rn; *p; p++) emit_textf("%d, ", (int)(unsigned char)*p); emit_textf("0\n");
                          emit_textf("  .section .text\n  .intel_syntax noprefix\n");
                          emit_textf("  lea rdi, [rip + .Lrolechild%d_%d]\n", ci, rj);
                          emit_textf("  lea rsi, [rip + .Lrolename%d_%d]\n", ci, rj);
                          emit_textf("  call class_compose_role@PLT\n");
                      }
                  } }
                { extern int rt_grammar_count(void); extern const char *rt_grammar_qname(int); extern const char *rt_grammar_body(int); extern int rt_grammar_flavor(int);
                  int n_gram = rt_grammar_count();
                  for (int gi = 0; gi < n_gram; gi++) {
                      const char *qn = rt_grammar_qname(gi); const char *bd = rt_grammar_body(gi);
                      if (!qn || !bd) continue;
                      emit_textf("  .section .rodata\n");
                      emit_textf("  .Lgramqn%d: .byte ", gi); for (const char *p = qn; *p; p++) emit_textf("%d, ", (int)(unsigned char)*p); emit_textf("0\n");
                      emit_textf("  .Lgrambd%d: .byte ", gi); for (const char *p = bd; *p; p++) emit_textf("%d, ", (int)(unsigned char)*p); emit_textf("0\n");
                      emit_textf("  .section .text\n  .intel_syntax noprefix\n");
                      emit_textf("  lea rdi, [rip + .Lgramqn%d]\n", gi);
                      emit_textf("  lea rsi, [rip + .Lgrambd%d]\n", gi);
                      emit_textf("  mov edx, %d\n", rt_grammar_flavor(gi));
                      emit_textf("  call rt_grammar_register@PLT\n");
                  } }
                for (int i = 0; i < n_procs; i++) {
                    ProcEntry *pe = &s2->proc_table[proc_pidx_buf[i]];
                    if (pe->dyn_scope && proc_role3_kind((pe->bb_idx >= 0 && pe->bb_idx < s2->bbp.count) ? s2->bbp.table[pe->bb_idx] : (IR_graph_t *)0) != 2) continue;   /* ⭐ EXPR-THUNK EXITS (s96): the EXPR$ thunk (kind 2) is dyn_scope with NO DEFINE site, so it is registered HERE at startup like the non-dyn rows; every other dyn_scope row keeps the s57 site registration. */   /* ⭐⭐⭐ DEFINE-SITE s57 (Lon): the DEFINE registration lives AT the statement in the shared chain (bb_ab_bind's rt_define_site call) — the startup hoist for dyn_scope procs is DELETED, not duplicated.  Non-dyn (LBL__ pseudo-procs, generators) keep the hoist: they have no statement site. */
                    emit_textf("  .section .rodata\n");
                    emit_textf("  .Lstartup_pname%d: .string \"%s\"\n", i, proc_names_buf[i]);   /* s62: the LBL__<N> row registers under its OWN name — an earlier s62 attempt stripped the prefix so ARG/LOCAL would find "jlab", which MEASURABLY diverged from the sbl oracle on 1017_arg_local (oracle returns the prototype name as written; the stripped-name lookup resolved to the upcased formal and flipped every assertion).  The oracle is the law: register the name the table holds. */
                    if (pe->dyn_scope) {
                        for (int k = 0; k < pe->nparams && k < pe->lower_sc.n; k++)
                            emit_textf("  .Lstartup_pp%d_%d: .string \"%s\"\n", i, k, pe->lower_sc.e[k].name ? pe->lower_sc.e[k].name : "");
                        emit_textf("  .align 8\n  .Lstartup_pnames%d:\n", i);
                        for (int k = 0; k < pe->nparams && k < pe->lower_sc.n; k++) emit_textf("  .quad .Lstartup_pp%d_%d\n", i, k);
                        emit_textf("  .quad 0\n");
                    }
                    emit_textf("  .section .text\n");
                    emit_textf("  .intel_syntax noprefix\n");
                    if (pe->dyn_scope) {
                        emit_textf("  lea rdi, [rip + .Lstartup_pname%d]\n", i);
                        emit_textf("  lea rsi, [rip + .Lstartup_pnames%d]\n", i);
                        emit_textf("  mov edx, %d\n", proc_nparams_buf[i]);
                        emit_textf("  call rt_proc_register@PLT\n");
                        emit_textf("  lea rdi, [rip + .Lstartup_pname%d]\n", i);
                        emit_textf("  mov esi, 1\n");
                        emit_textf("  call rt_proc_set_dyn_scope@PLT\n");
                        if (pe->result_name && strcmp(pe->result_name, pe->name)) {
                            emit_textf("  .section .rodata\n  .Lstartup_prn%d: .string \"%s\"\n  .section .text\n  .intel_syntax noprefix\n", i, pe->result_name);
                            emit_textf("  lea rdi, [rip + .Lstartup_pname%d]\n", i);
                            emit_textf("  lea rsi, [rip + .Lstartup_prn%d]\n", i);
                            emit_textf("  call rt_proc_set_result_name@PLT\n");
                        }
                    }
                    emit_textf("  lea rdi, [rip + .Lstartup_pname%d]\n", i);
                    if (strncmp(proc_names_buf[i], "LBL__", 5) == 0) emit_textf("  lea rsi, [rip + %s_body]\n", asm_sym_name(proc_names_buf[i] + 5));   /* BARE-CHAIN (Lon s62): the LBL__ body chain binds ONE label, <FN>_body — proc_LBL__<FN>_α is gone */
                    else emit_textf("  lea rsi, [rip + proc_%s_\xce\xb1]\n", asm_sym_name(proc_names_buf[i]));
                    emit_textf("  call rt_proc_set_fn@PLT\n");
                    emit_textf("  lea rdi, [rip + .Lstartup_pname%d]\n", i);
                    emit_textf("  mov esi, %d\n", proc_nparams_buf[i]);
                    emit_textf("  call rt_proc_set_nparams@PLT\n");
                    { int _pin = proc_pidx_buf[i]; int _nf = (_pin >= 0 && _pin < s2->proc_count) ? s2->proc_table[_pin].nformals : 0;   /* NPSPLIT (s22w): m4 startup mirrors the direct m3 registration — 0 for unsplit frontends, runtime falls back */
                      emit_textf("  lea rdi, [rip + .Lstartup_pname%d]\n", i);
                      emit_textf("  mov esi, %d\n", _nf);
                      emit_textf("  call rt_proc_set_nformals@PLT\n"); }
                    { int _pi2 = proc_pidx_buf[i];
                      if (is_raku && _pi2 >= 0 && _pi2 < s2->proc_count) { ProcEntry *_pe = &s2->proc_table[_pi2];
                        if (!_pe->dyn_scope) for (int k = 0; k < _pe->nparams && k < _pe->lower_sc.n; k++) {
                            const char *_pn = _pe->lower_sc.e[k].name; if (!_pn) continue;
                            emit_textf("  .section .rodata\n  .Lstartup_qp%d_%d: .string \"%s\"\n  .section .text\n  .intel_syntax noprefix\n", i, k, _pn);
                            emit_textf("  lea rdi, [rip + .Lstartup_pname%d]\n", i);
                            emit_textf("  mov esi, %d\n", k);
                            emit_textf("  lea rdx, [rip + .Lstartup_qp%d_%d]\n", i, k);
                            emit_textf("  call rt_proc_set_pname@PLT\n");
                        } } }
                    if (proc_fb_buf[i] > 0) {
                        emit_textf("  lea rdi, [rip + .Lstartup_pname%d]\n", i);
                        emit_textf("  mov esi, %d\n", proc_fb_buf[i]);
                        emit_textf("  call rt_proc_set_frame_bytes@PLT\n");
                    }
                    if (proc_ispat_buf[i] && proc_zstatic_buf[i]) {   /* PS-1b (s151): mode-4 printed twin of the m3 rt_proc_set_zstatic — only for PAT$ procs (the SNO$MKPAT consumer set) and only when statically proven; unregistered stays the conservative 0 */
                        emit_textf("  lea rdi, [rip + .Lstartup_pname%d]\n", i);
                        emit_textf("  mov esi, 1\n");
                        emit_textf("  call rt_proc_set_zstatic@PLT\n");
                    }
                    if (pe->is_variadic) {
                        emit_textf("  lea rdi, [rip + .Lstartup_pname%d]\n", i);
                        emit_textf("  mov esi, 1\n");
                        emit_textf("  call rt_proc_set_variadic@PLT\n");
                    }
                    if (pe->rest_kind) {   /* the m4 twin of the in-process rt_proc_set_rest_kind — WITHOUT this the standalone binary binds the slurpy tail as a DT_DATA list and .elems/subscripts read garbage, the exact silent-wrong-answer shape of the s2026-07-26b pname replay gap: the startup replay is an ALLOWLIST, not a snapshot, so m3 passing proves nothing about m4.  Emitted only when the fact is set, so every peer language's .s stays byte-identical. */
                        emit_textf("  lea rdi, [rip + .Lstartup_pname%d]\n", i);
                        emit_textf("  mov esi, %d\n", pe->rest_kind);
                        emit_textf("  call rt_proc_set_rest_kind@PLT\n");
                    }
                    if (pe->named_rest) {   /* the m4 twin of the in-process rt_proc_set_named_rest — same ALLOWLIST law as rest_kind directly above: without it the standalone binary leaves the *%h collector slot unbound and every named-arg key silently vanishes while m3 reads clean.  Emits pe->named_rest, never a literal, so a second collector slot cannot be downgraded (the s2026-07-27 hardcoded-mov-esi-1 lesson). */
                        emit_textf("  lea rdi, [rip + .Lstartup_pname%d]\n", i);
                        emit_textf("  mov esi, %d\n", pe->named_rest);
                        emit_textf("  call rt_proc_set_named_rest@PLT\n");
                    }
                    {   /* NCB-1d: record the body's regime for the C transfer fns — the mode-4 twin of the in-process rt_proc_set_jmpentry.  GENP slice-2: the generator flag now ALSO embeds (rt_proc_call_gen_h's per-instance-stack arm discriminates on p->jmp_entry && rt_proc_is_generator — without this twin the m4 runtime took the det one-shot arm and rt_genp_yield aborted with no current coexpression). */
                        emit_textf("  lea rdi, [rip + .Lstartup_pname%d]\n", i);
                        emit_textf("  mov esi, %d\n", strncmp(proc_names_buf[i], "gram__", 6) != 0);
                        emit_textf("  call rt_proc_set_jmpentry@PLT\n");
                        { extern int rt_pl_dc_ok(const char *, int); if (!proc_ispat_buf[i] && rt_pl_dc_ok(proc_names_buf[i], proc_nparams_buf[i])) {   /* PL-DC s108: the m4 twin of the m3 seal registration — s112: predicate now TRULY equals the arming predicate (!ispat && dc_ok), so the label exists iff this bakes; without the ispat conjunct the bake referenced proc_PAT$N_dcα stubs the pat-excluded arming never emitted (treebank m4 link regression) */
                            emit_textf("  lea rdi, [rip + .Lstartup_pname%d]\n", i);
                            emit_textf("  lea rsi, [rip + proc_%s_dc\xce\xb1]\n", asm_sym_name(proc_names_buf[i]));
                            emit_textf("  call rt_proc_set_dcfn@PLT\n"); } }
                        if (pe->is_generator) {
                            emit_textf("  lea rdi, [rip + .Lstartup_pname%d]\n", i);
                            emit_textf("  mov esi, 1\n");
                            emit_textf("  call rt_proc_set_generator@PLT\n");
                        }
                    }
                }
                emit_textf("  add rsp, 8\n");
                emit_textf("  ret\n");
            }
            for (int _fq = 0; _fq < n_procs; _fq++) if (proc_names_buf[_fq]) { free((void *)proc_names_buf[_fq]); proc_names_buf[_fq] = NULL; }   /* s62 SAFETY: free strdup'd names */
            free(proc_names_buf); free(proc_nparams_buf); free(proc_pidx_buf); free(proc_fb_buf); free(proc_zstatic_buf);
            if (n_gva_icn > 0) {
                emit_textf("  .section .rodata\n");
                for (int k = 0; k < n_gva_icn; k++) emit_textf("  .Lgvan%d: .string \"%s\"\n", k, gva_name(k));
                emit_textf("  .align 8\n__gva_names:\n");
                for (int k = 0; k < n_gva_icn; k++) emit_textf("  .quad .Lgvan%d\n", k);
                emit_textf("  .section .text\n  .intel_syntax noprefix\n");
            }
            emit_textf("  .globl main\n");
            emit_textf("main:\n");
            emit_textf("  sub rsp, 8\n");
            emit_textf("  push rdi\n");
            emit_textf("  push rsi\n");
            { const char * hr = getenv("SCRIP_M4_HEADROOM"); if (hr && *hr) { long hb = atol(hr); if (hb > 0) { hb = (hb + 15) & ~15L; emit_textf("  sub rsp, %ld\n", hb); } } }   /* DIAGNOSTIC ONLY, NOT A FIX (s22r) -- see the twin note at the other main emitter.  Tests s22q BLOCK PREDICTION that ~76 m4 failures are ONE corruption class of unarmed readers writing through envp, by moving envp out of reach WITHOUT converting a reader.  Multiple-of-16 preserves the alpha ARRIVAL PARITY contract.  DEFAULT OFF, MUST STAY OFF: a program passing under this pad is CUSHIONED, not correct. */
            if (rt_zeta_mode() != (int)ZC_ZETA) emit_textf("  mov edi, %d\n  call rt_zeta_set_mode@PLT\n", rt_zeta_mode()); /* ZETA SUBSYSTEM bake (Lon 2026-07-09): only when --zeta overrode ZC_ZETA — no flag, no bake, byte-identical; MUST precede proc_startup/core_lib_init (first possible allocation) */
            if (rt_zeta_storage_get() != (int)ZC_STORAGE) emit_textf("  mov edi, %d\n  call rt_zeta_storage_set@PLT\n", rt_zeta_storage_get()); /* ZC_STORAGE bake (GOAL-ZETA-FOUR Z4-4 slice 2): four-config twin of the port bake below — the setter re-derives the legacy tuple at runtime, so a storage-committed .s self-selects everything; placed FIRST so env-only selection resolves before the port predicate reads; no override, no bake, byte-identical */
            if (rt_zeta_port_mode() != (int)ZC_PORT) emit_textf("  mov edi, %d\n  call rt_zeta_port_set_mode@PLT\n", rt_zeta_port_mode()); /* ZETA PORT bake (Lon 2026-07-10): the .s is port-mode-COMMITTED (rsp vs arena arithmetic), so the runtime side must self-select the emit-time mode; only when --zeta-port/env overrode ZC_PORT — no override, no bake, byte-identical */
            emit_textf("  call core_lib_init@PLT\n");
            if (n_procs > 0 || n_cls_emit > 0 || n_gram_emit > 0)
            if (n_procs > 0 || n_cls_emit > 0 || n_gram_emit > 0)
                emit_textf("  call proc_startup\n");
            if (n_gva_icn > 0) emit_textf("  mov edi, %d\n  call rt_gva_island@PLT\n  mov rsi, rax\n  lea rdi, [rip + __gva_names]\n  mov edx, %d\n  call gva_register@PLT\n", n_gva_icn, n_gva_icn);
            { extern int prolog_op_user_count(void); extern int prolog_op_user_get(int, const char **, int *, const char **); int n_uop = prolog_op_user_count();
              if (n_uop > 0) { emit_textf("  .section .rodata\n"); for (int k = 0; k < n_uop; k++) { const char *onm = 0; int opr = 0; const char *oty = 0; if (!prolog_op_user_get(k, &onm, &opr, &oty)) continue; char eb[512]; int ei = 0; for (const char *s = onm ? onm : ""; *s && ei < 508; s++) { if (*s == '\\' || *s == '"') eb[ei++] = '\\'; eb[ei++] = *s; } eb[ei] = 0; emit_textf("  .Lopn%d: .string \"%s\"\n  .Lopt%d: .string \"%s\"\n", k, eb, k, oty ? oty : "xfx"); }
                emit_textf("  .section .text\n  .intel_syntax noprefix\n"); for (int k = 0; k < n_uop; k++) { const char *onm = 0; int opr = 0; const char *oty = 0; if (!prolog_op_user_get(k, &onm, &opr, &oty)) continue; emit_textf("  lea rdi, [rip + .Lopn%d]\n  mov esi, %d\n  lea rdx, [rip + .Lopt%d]\n  call prolog_op_table_add@PLT\n", k, opr, k); } } }
            if (rt_zc_frame_live() == ZC_FRAME_RSP) { /* R12-ERAD: blob self-allocates its FORTH frame; wrapper carves nothing — ICNBENCH-ARGS-RSP (2026-07-18, closes the FENCE): stage argv for the prologue's rt_main_args_fetch bind ([rsp]=argv, [rsp+8]=argc from the push rdi/push rsi preamble) */
                if (bbg->nparams >= 1) emit_textf("  mov rdi, qword ptr [rsp]\n  add rdi, 8\n  mov esi, dword ptr [rsp + 8]\n  sub esi, 1\n  call rt_main_args_stage@PLT\n");
            } else {
            emit_textf("  sub rsp, 65536\n  mov rdi, rsp\n  mov ecx, 8192\n  xor eax, eax\n  rep stosq\n  mov rdi, rsp\n"); /* ZS-1: main zeta frame on the stack */
            if (bbg->nparams >= 1)
                emit_textf("  push rdi\n  sub rsp, 8\n  mov rdi, qword ptr [rsp + 65552]\n  add rdi, 8\n  mov esi, dword ptr [rsp + 65560]\n  sub esi, 1\n  call rt_args_list_from@PLT\n  add rsp, 8\n  pop rdi\n"
                       "  mov qword ptr [rdi + 16], rax\n  mov qword ptr [rdi + 24], rdx\n");
            }
            /* ZW-3 R12-FREE-1 REVERSAL (O-5, OMEGA s27 session): r12 is the LIVE CAS/dcap top register (s23l ruling); seed from [RT_DCAP_TOP] before graph entry so every match-family op_zw arm can read/write r12 directly.  Mirror of the flat_α path already at line 1438.  Mode-3 path uses rt_outer_call (rt.c thunk) which already does push r12 / mov r12,[0x70000000] / call / pop r12. */
            emit_textf("  mov r12, qword ptr [0x70000000]\n");   /* 0x70000000 == RT_DCAP_TOP (rtx_init.c _Static_assert) */
            if (is_prolog && bbg->zframe_graph && !bbg->icn_cells_graph) emit_textf("  call rt_gcheap_warmup@PLT\n  call rt_plw_floor_bypass_on@PLT\n");   /* W1 m4 twins (PL-ZFRAME-RESTORE s13): m3's driver runs BOTH before entry (scrip.c :1635/:1638 area) — the compiled binary got neither.  warmup = W1-Bug1 (s9): dl_iterate_phdr's movaps SEGVs when the lazy rt_gcheap_init fires from a misaligned JIT frame (core bt: rt_plj_alloc→rt_gcheap_init→gc_static_segs_init→dl_iterate_phdr, rsp≡8); running it once here, from main's aligned C context, retires the lazy path.  bypass_on = W1-Bug2: must be a PLT call INTO the .so (a direct [rip+sym] store copy-relocates a dead duplicate into the exe's .bss while dop_call binds locally to the .so's copy).  Placed BEFORE xor esi (rsi caller-saved); r12 seed above survives (callee-saved).  Gated is_prolog: SN4 mains never zframe; Icon zframe mains stay byte-identical.  No clears: the emitted main exits via the zf wires. */
            /* ONE-SHOT BRIDGE (Lon s22p): jmp not call; main_γ / main_ω are defined AFTER the body. */
            { extern unsigned char g_rtcc_on; if (g_rtcc_on) emit_textf("  call rtcc_load_all@PLT\n"); }   /* RC-5-GVA SITE-A TWIN: main_α bridge never established R9=RT_GVA_VA */
            emit_textf("  xor esi, esi\n");
            if (bbg->zframe_graph && !bbg->icn_cells_graph) {   /* ICN-FR-2: ζ-frame main needs γ/ω wires in rcx/rdx on entry — the prologue saves them at [rsp+kt-24/-16] for the epilogue's direct read.  Emit two tiny exit-wire thunks (γ=exit(0), ω=exit(1)) and load their RIP-relative addresses before the jmp.  The thunks sit between main: and main_α — unreachable by fall-through (the jmp skips them), reachable only via the wire-return jmp rcx/rdx from main_γ/ω.  R-ZK-A DEFENCE: icn_cells_graph exclusion prevents double-dispatch when both CELLS and ZFRAME are armed (defence-in-depth; lower_icon.c's stamp loop is the primary enforcement site). */
                emit_textf("  xor r14d, r14d\n");               /* ICN-FR-5: r14=0 → &pos=1 outside any scan (twin of icn_zf_main_call m3 fix) */
                emit_textf("  lea rcx, [rip + .Lmain_zf_γ]\n");
                emit_textf("  lea rdx, [rip + .Lmain_zf_ω]\n");
                emit_textf("  jmp main_\xce\xb1\n");
                emit_textf(".Lmain_zf_γ:\n  xor edi, edi\n  call exit@PLT\n");
                emit_textf(".Lmain_zf_ω:\n  mov edi, 1\n  call exit@PLT\n");
            } else
            emit_textf("  jmp main_\xce\xb1\n");
            int rc;
            {
                { extern IR_graph_t *g_emit_cfg; g_emit_cfg = bbg; }
                { int _na = 0; for (int _q = 0; _q < s2->proc_count; _q++) if (s2->proc_table[_q].name && strncmp(s2->proc_table[_q].name, "LBL__", 5) == 0 && s2->proc_table[_q].proc_entry_node) _na++;
                  if (_na > 0 && bbg->n_balias == 0) { bbg->balias_node = (IR_t **)calloc((size_t)_na, sizeof(IR_t *)); bbg->balias_name = (const char **)calloc((size_t)_na, sizeof(char *));
                      if (bbg->balias_node && bbg->balias_name) for (int _q = 0; _q < s2->proc_count; _q++) { if (!s2->proc_table[_q].name || strncmp(s2->proc_table[_q].name, "LBL__", 5) != 0 || !s2->proc_table[_q].proc_entry_node) continue;
                          if (bbg->n_balias >= _na) break;   /* bounds guard: _na counted the same predicate, but duplicates or race could produce more — never write past the allocation */
                          IR_t * _bn = s2->proc_table[_q].proc_entry_node; int _bgg = 0; while (_bn && (_bn->op == IR_SUCCEED || _bn->op == IR_FAIL || _bn->op == IR_GOTO) && _bn->γ.node && _bgg++ < 65536) _bn = _bn->γ.node;
                          char _ab[300]; snprintf(_ab, sizeof _ab, "%s_body", asm_sym_name(s2->proc_table[_q].name + 5)); bbg->balias_node[bbg->n_balias] = _bn; bbg->balias_name[bbg->n_balias] = strdup(_ab); if (bbg->balias_name[bbg->n_balias]) bbg->n_balias++; } } }   /* BODY-ALIAS (Lon s62): chase each LBL__ entry through the transparent relays to its statement_begin — the SAME chase codegen's HQ-s26 entry bind uses — and stamp the alias pair on main's graph; the emitter renames that statement's α to <FN>_body at its source position.  Consumers already speak this name: role-4 shim fold target, bb_goto_dyn fold arm, and the LBL__ set_fn lea. */
                { extern int g_flat_outer_nparams; g_flat_outer_nparams = bbg->nparams; } /* ICNBENCH-ARGS-RSP: main graph only */
                emit_sep_rule_c('-'); rc = emit_chain(bbg->entry, _out, "main") ? 0 : 1;
                { extern int g_flat_outer_nparams; g_flat_outer_nparams = 0; }
                /* LADDER AB (AB-1): emit IR_FUNC_ACTIVATE activation blocks as dead-code .text after the main chain.
                 * These are jump-target-only until AB-3 flips call sites.  Blocks carry fn_cell$.data + α frame stub. */
                { extern void bb_ab_emit_nodes(IR_graph_t *g, int gva_active); bb_ab_emit_nodes(bbg, g_gva_active); }
                /* STATEMENT-ORDER FB-BACKFILL (s62): LBL__ rows share main's frame — ARG/LOCAL read frame_bytes at runtime to index formals/locals.  At record time (line 926) we wrote 0 because the LBL__ standalone chain no longer emits and g_last_flat_frame_bytes was stale from the prior proc; now main has emitted, g_last_flat_frame_bytes holds its true value, and every LBL__ row that shares main's bb_idx gets that value so the registration call is emitted correctly.  Use proc_table directly (proc_names_buf may be corrupted by emission-time arena activity on large programs). */
                { extern int g_last_flat_frame_bytes; int _main_fb = g_last_flat_frame_bytes; for (int _q = 0; _q < n_procs; _q++) { if (proc_fb_buf[_q] != 0) continue; int _pi2 = proc_pidx_buf[_q]; if (_pi2 < 0 || _pi2 >= s2->proc_count) continue; const char *_qn = s2->proc_table[_pi2].name; if (!_qn || strncmp(_qn, "LBL__", 5) != 0) continue; if (s2->proc_table[_pi2].bb_idx == main_bb_idx) proc_fb_buf[_q] = _main_fb; } }
            }
            g_gva_active = 0;
            g_frame_active = 0;
            extern void xa_emit_strtab_rodata(void);
            xa_emit_strtab_rodata();
            { extern void xa_emit_csettab_rodata(void); xa_emit_csettab_rodata(); }
            emit_textf("  .section .note.GNU-stack,\"\",@progbits\n");
            emit_textf_flush();
            fflush(stdout);
            ir_delete_all(s2);
            return rc;
        }
        {
            extern bb_box_fn emit_chain(IR_t * entry, FILE * out, const char * prefix);
            extern void xa_emit_strtab_rodata(void);
            extern int g_frame_active;
            extern void rt_proc_reset(void);
            extern void rt_proc_register(const char * name, const char ** pnames, int nparams);
            stage2_t *s2 = sm_preamble(ast_prog, segs, nsegs);
            if (!s2) { fprintf(stderr, "[SBB] mode-4: sm_preamble failed\n"); return 1; }
            ast_tree_free(ast_prog); ast_prog = NULL;
            if (is_pascal) { extern void optimizer_run(IR_graph_t * g); for (int _gi = 0; _gi < s2->bbp.count; _gi++) if (s2->bbp.table[_gi]) optimizer_run(s2->bbp.table[_gi]); }
            drive_slots_all(s2);
            int main_bb_idx = -1;
            for (int _pi = 0; _pi < s2->proc_count; _pi++)
                if (s2->proc_table[_pi].name && strcmp(s2->proc_table[_pi].name, "main") == 0) { main_bb_idx = s2->proc_table[_pi].bb_idx; break; }
            if (main_bb_idx < 0 || main_bb_idx >= s2->bbp.count || !s2->bbp.table[main_bb_idx] || !s2->bbp.table[main_bb_idx]->entry) {
                fprintf(stderr, "[SBB] FATAL: mode-4 driver: SNOBOL4 main BB graph not found\n");
                return 1;
            }
            IR_graph_t *sbbg = s2->bbp.table[main_bb_idx];
            extern int g_flat_node_id;
            extern int g_m4_dense_nid;
            g_flat_node_id = 0;
            g_m4_dense_nid = 1;
            g_medium = BB_MEDIUM_TEXT; FILE * _out = stdout; if (output_path) { _out = fopen(output_path, "w"); if (!_out) { perror(output_path); return 1; } } emit_set_sink(_out);   /* m4 -o (06g fix): path-scope _out, twin of the first path */
            emit_textf("  .intel_syntax noprefix\n");
            emit_textf("  .text\n");
            rt_proc_reset();
            g_frame_active = 1;
            for (int _pi = 0; _pi < s2->proc_count; _pi++) {
                const char *pname = s2->proc_table[_pi].name;
                if (!pname || strcmp(pname, "main") == 0) continue;
                int idx = s2->proc_table[_pi].bb_idx;
                if (idx < 0 || idx >= s2->bbp.count || !s2->bbp.table[idx] || !s2->bbp.table[idx]->entry) continue;
                int is_dup = 0;
                for (int _pj = _pi + 1; _pj < s2->proc_count; _pj++) { if (s2->proc_table[_pj].name && strcmp(s2->proc_table[_pj].name, pname) == 0) { is_dup = 1; break; } }
                if (is_dup) continue;
                int np = s2->proc_table[_pi].nparams;
                const char **pn = NULL;
                if (np > 0) {
                    pn = (const char **)calloc((size_t)np, sizeof(const char *));
                    for (int k = 0; k < np && k < s2->proc_table[_pi].lower_sc.n; k++) pn[k] = s2->proc_table[_pi].lower_sc.e[k].name;
                }
                rt_proc_register(pname, pn, np);
                { extern void rt_proc_set_nformals(const char *, int); rt_proc_set_nformals(pname, s2->proc_table[_pi].nformals); }   /* NPSPLIT (s22w): 0 for unsplit frontends -> runtime falls back to nparams */
                { extern void rt_proc_set_frame(const char *, int, int); extern void rt_proc_set_byref(const char *, uint64_t);
                  if (s2->bbp.table[idx]->nslots > 0) rt_proc_set_frame(pname, s2->bbp.table[idx]->nslots - 1, s2->proc_table[_pi].decl_level);
                  rt_proc_set_byref(pname, s2->proc_table[_pi].byref_mask); }
            }
            { extern int g_proc_direct_active; extern void proc_collect_reset(void); extern void proc_collect_graph(IR_graph_t *); extern int proc_slot_count(void);
              proc_collect_reset(); proc_collect_graph(sbbg);
              for (int _pi = 0; _pi < s2->proc_count; _pi++) {
                  const char *pn2 = s2->proc_table[_pi].name; if (!pn2 || strcmp(pn2, "main") == 0) continue; int idx2 = s2->proc_table[_pi].bb_idx;
                  if (idx2 < 0 || idx2 >= s2->bbp.count || !s2->bbp.table[idx2] || !s2->bbp.table[idx2]->entry) continue; proc_collect_graph(s2->bbp.table[idx2]);
              }
              g_proc_direct_active = (proc_slot_count() > 0) ? 1 : 0; }
            if (is_pascal) { extern void gva_collect_reset(void); extern void gva_collect_graph(IR_graph_t *); extern int gva_count(void); extern int g_gva_active; gva_collect_reset(); gva_collect_graph(sbbg); for (int _pgi = 0; _pgi < s2->bbp.count; _pgi++) { if (s2->bbp.table[_pgi] && s2->bbp.table[_pgi] != sbbg) gva_collect_graph(s2->bbp.table[_pgi]); } g_gva_active = (gva_count() > 0) ? 1 : 0; } /* PAS-GVA: activate arena BEFORE proc emission so proc-context globals resolve op_gva_k (M3 already orders it this way); without this, proc bodies emit NV_GET/SET_fn hash calls for true program globals */
            int _pbcap = (s2->proc_count > 0) ? s2->proc_count : 1;
            int *pidx_buf = (int *)malloc((size_t)_pbcap * sizeof(int));
            int *peak_buf = (int *)malloc((size_t)_pbcap * sizeof(int));
            int n_procs = 0;
            for (int _pi = 0; _pi < s2->proc_count; _pi++) {
                const char *pname = s2->proc_table[_pi].name;
                if (!pname || strcmp(pname, "main") == 0) continue;
                int idx = s2->proc_table[_pi].bb_idx;
                if (idx < 0 || idx >= s2->bbp.count || !s2->bbp.table[idx] || !s2->bbp.table[idx]->entry) continue;
                int is_dup = 0;
                for (int _pj = _pi + 1; _pj < s2->proc_count; _pj++) { if (s2->proc_table[_pj].name && strcmp(s2->proc_table[_pj].name, pname) == 0) { is_dup = 1; break; } }
                if (is_dup) continue;
                int np = s2->proc_table[_pi].nparams;
                const char **pn = NULL;
                if (np > 0) {
                    pn = (const char **)calloc((size_t)np, sizeof(const char *));
                    for (int k = 0; k < np && k < s2->proc_table[_pi].lower_sc.n; k++)
                        pn[k] = s2->proc_table[_pi].lower_sc.e[k].name;
                }
                rt_proc_register(pname, pn, np);
                { extern void rt_proc_set_nformals(const char *, int); rt_proc_set_nformals(pname, s2->proc_table[_pi].nformals); }   /* NPSPLIT (s22w): 0 for unsplit frontends -> runtime falls back to nparams */
                { extern void rt_proc_set_frame(const char *, int, int); extern void rt_proc_set_byref(const char *, uint64_t); extern int g_emit_frame_caller_dl;
                  if (s2->bbp.table[idx]->nslots > 0) rt_proc_set_frame(pname, s2->bbp.table[idx]->nslots - 1, s2->proc_table[_pi].decl_level);
                  rt_proc_set_byref(pname, s2->proc_table[_pi].byref_mask);
                  g_emit_frame_caller_dl = (s2->bbp.table[idx]->nslots > 0) ? s2->proc_table[_pi].decl_level : -1; }
                { extern IR_graph_t *g_emit_cfg; g_emit_cfg = s2->bbp.table[idx]; }
                { extern int g_flat_frame_floor; extern int zls_g_region(const IR_graph_t *); IR_graph_t *_pg = s2->bbp.table[idx]; g_flat_frame_floor = 0; if (_pg && _pg->entry && _pg->entry->op == IR_SAVE_RESTORE && IR_LIT(_pg->entry).ival == 3) { for (int _mi = 0; _mi < s2->proc_count; _mi++) if (s2->proc_table[_mi].name && !strcmp(s2->proc_table[_mi].name, "main")) { int _mx = s2->proc_table[_mi].bb_idx; if (_mx >= 0 && _mx < s2->bbp.count && s2->bbp.table[_mx]) g_flat_frame_floor = zls_g_region(s2->bbp.table[_mx]); break; } } }   /* SN4-FLAT-PROC (s176): DEFINE stubs carve a fresh MAIN-layout frame -- floor their region at main's (see emit_jmp_entry_arm_region); cleared by emit_jmp_entry_clear */
                { extern int emit_jmp_entry_for_patproc(const char*, IR_graph_t*); extern int emit_jmp_entry_for_proc(const char*, int, int, IR_graph_t*); extern void emit_jmp_entry_clear(void); if (!emit_jmp_entry_for_patproc(pname, s2->bbp.table[idx])) emit_jmp_entry_for_proc(pname, s2->proc_table[_pi].dyn_scope, s2->proc_table[_pi].is_generator, s2->bbp.table[idx]); }
                { if (is_icon || is_sno_bb || is_prolog) { extern void zls_graph_name(const IR_graph_t *, const char *); zls_graph_name(s2->bbp.table[idx], pname); } }   /* ICN-FR-4: zls name registration twin — proc_entry_node path. PL-FR-4: added is_prolog. */
                { char _pfx[256]; snprintf(_pfx, sizeof(_pfx), "proc_%s", asm_sym_name(pname)); int _islbl = pname && strncmp(pname, "LBL__", 5) == 0; emit_sep_rule_c('-'); if (!_islbl) emit_textf("  .globl %s_\xce\xb1\n", _pfx); if (scrip_symmap()) emit_textf("  .type %s_\xce\xb1, @function\n", _pfx); emit_chain(s2->proc_table[_pi].proc_entry_node, _out, _pfx); if (scrip_symmap()) emit_textf("  .size %s_\xce\xb1, .-%s_\xce\xb1\n", _pfx, _pfx); }
                { extern void emit_jmp_entry_clear(void); emit_jmp_entry_clear(); }
                { extern int g_emit_frame_caller_dl; g_emit_frame_caller_dl = -1; }
                { extern int g_last_flat_frame_bytes; peak_buf[n_procs] = g_last_flat_frame_bytes; }
                pidx_buf[n_procs++] = _pi;
            }
            if (n_procs > 0) {
                emit_textf("  .section .rodata\n");
                for (int i = 0; i < n_procs; i++) {
                    ProcEntry *pe = &s2->proc_table[pidx_buf[i]];
                    emit_textf("  .Lpn%d: .string \"%s\"\n", i, pe->name);
                    for (int k = 0; k < pe->nparams && k < pe->lower_sc.n; k++)
                        emit_textf("  .Lpp%d_%d: .string \"%s\"\n", i, k, pe->lower_sc.e[k].name ? pe->lower_sc.e[k].name : "");
                    emit_textf("  .Lpnames%d:\n", i);
                    for (int k = 0; k < pe->nparams && k < pe->lower_sc.n; k++) emit_textf("  .quad .Lpp%d_%d\n", i, k);
                    emit_textf("  .quad 0\n");
                }
                emit_textf("  .section .text\n  .intel_syntax noprefix\n");
                emit_textf("proc_startup:\n  sub rsp, 8\n  call core_lib_init@PLT\n  call rt_proc_reset@PLT\n");
                for (int i = 0; i < n_procs; i++) {
                    ProcEntry *pe = &s2->proc_table[pidx_buf[i]];
                    emit_textf("  lea rdi, [rip + .Lpn%d]\n", i);
                    emit_textf("  lea rsi, [rip + .Lpnames%d]\n", i);
                    emit_textf("  mov edx, %d\n", pe->nparams);
                    emit_textf("  call rt_proc_register@PLT\n");
                    emit_textf("  lea rdi, [rip + .Lpn%d]\n", i);   /* NPSPLIT (s22w) */
                    emit_textf("  mov esi, %d\n", pe->nformals);
                    emit_textf("  call rt_proc_set_nformals@PLT\n");
                    emit_textf("  lea rdi, [rip + .Lpn%d]\n", i);
                    emit_textf("  lea rsi, [rip + proc_%s_\xce\xb1]\n", asm_sym_name(pe->name));
                    emit_textf("  call rt_proc_set_fn@PLT\n");
                    int _fidx = pe->bb_idx;
                    if (_fidx >= 0 && _fidx < s2->bbp.count && s2->bbp.table[_fidx] && s2->bbp.table[_fidx]->nslots > 0) {
                        emit_textf("  lea rdi, [rip + .Lpn%d]\n", i);
                        emit_textf("  mov esi, %d\n", s2->bbp.table[_fidx]->nslots - 1);
                        emit_textf("  mov edx, %d\n", pe->decl_level);
                        emit_textf("  call rt_proc_set_frame@PLT\n");
                    }
                    if (peak_buf[i] > 0) {
                        emit_textf("  lea rdi, [rip + .Lpn%d]\n", i);
                        emit_textf("  mov esi, %d\n", peak_buf[i]);
                        emit_textf("  call rt_proc_set_frame_bytes@PLT\n");
                    }
                }
                emit_textf("  add rsp, 8\n  ret\n");
            }
            free(pidx_buf); free(peak_buf);
            extern void gva_collect_reset(void); extern void gva_collect_graph(IR_graph_t *); extern int gva_count(void); extern const char *gva_name(int); extern int g_gva_active;
            extern int proc_slot_count(void); extern int g_proc_direct_active;
            if (!is_pascal) { gva_collect_reset(); gva_collect_graph(sbbg); } /* PAS-GVA: Pascal collected pre-proc-emission above; re-reset here would be harmless (deterministic walk) but wasted — non-Pascal keeps the original single-graph collection byte-identical */
            int n_gva = gva_count();
            if (n_gva > 0) {
                emit_textf("  .section .rodata\n");
                for (int k = 0; k < n_gva; k++) emit_textf("  .Lgvan%d: .string \"%s\"\n", k, gva_name(k));
                emit_textf("  .align 8\n__gva_names:\n");
                for (int k = 0; k < n_gva; k++) emit_textf("  .quad .Lgvan%d\n", k);
                emit_textf("  .section .text\n  .intel_syntax noprefix\n");
            }
            int n_proc_slot = proc_slot_count();
            if (n_proc_slot > 0) {
                extern const char *proc_slot_name(int);
                emit_textf("  .section .rodata\n");
                for (int k = 0; k < n_proc_slot; k++) emit_textf("  .Lprocn%d: .string \"%s\"\n", k, proc_slot_name(k));
                emit_textf("  .align 8\n__proc_names:\n");
                for (int k = 0; k < n_proc_slot; k++) emit_textf("  .quad .Lprocn%d\n", k);
                emit_textf("  .section .bss\n  .align 8\n__proc: .space %d, 0\n", n_proc_slot * 8);
                emit_textf("  .section .text\n  .intel_syntax noprefix\n");
            }
            emit_textf("  .globl main\nmain:\n  sub rsp, 8\n  push rdi\n  push rsi\n");
            { const char * hr = getenv("SCRIP_M4_HEADROOM"); if (hr && *hr) { long hb = atol(hr); if (hb > 0) { hb = (hb + 15) & ~15L; emit_textf("  sub rsp, %ld\n", hb); } } }   /* DIAGNOSTIC ONLY, NOT A FIX (s22r) -- s22q measured m4 headroom from graph-entry rsp to the envp array at 344 BYTES against m3 20,048, and predicted ~76 of m4 failures are ONE corruption class of unarmed FR/FRQ/FRQB readers writing through live process state.  This env-gated pad tests that BLOCK PREDICTION by moving the envp array out of reach WITHOUT converting a single reader: if the m4 watermark jumps, the one-authority claim is confirmed AND quantified, giving the reader conversion a measured payoff estimate.  Multiple-of-16 so the alpha ARRIVAL PARITY contract (s22q, two signatories) is preserved.  DEFAULT OFF and it MUST STAY OFF -- s22q: "DO NOT RE-CARVE, the fix is not a bigger backing store"; a passing program under this pad is CUSHIONED, not correct, exactly as m3's 20KB of driver frames cushion it today. */
            if (rt_zeta_mode() != (int)ZC_ZETA) emit_textf("  mov edi, %d\n  call rt_zeta_set_mode@PLT\n", rt_zeta_mode()); /* ZETA SUBSYSTEM bake (Lon 2026-07-09): only when --zeta overrode ZC_ZETA — no flag, no bake, byte-identical; MUST precede proc_startup/core_lib_init (first possible allocation) */
            if (rt_zeta_storage_get() != (int)ZC_STORAGE) emit_textf("  mov edi, %d\n  call rt_zeta_storage_set@PLT\n", rt_zeta_storage_get()); /* ZC_STORAGE bake (GOAL-ZETA-FOUR Z4-4 slice 2): four-config twin of the port bake below — the setter re-derives the legacy tuple at runtime, so a storage-committed .s self-selects everything; placed FIRST so env-only selection resolves before the port predicate reads; no override, no bake, byte-identical */
            if (rt_zeta_port_mode() != (int)ZC_PORT) emit_textf("  mov edi, %d\n  call rt_zeta_port_set_mode@PLT\n", rt_zeta_port_mode()); /* ZETA PORT bake (Lon 2026-07-10): the .s is port-mode-COMMITTED (rsp vs arena arithmetic), so the runtime side must self-select the emit-time mode; only when --zeta-port/env overrode ZC_PORT — no override, no bake, byte-identical */
            if (n_procs > 0) emit_textf("  call proc_startup\n");
            else emit_textf("  call core_lib_init@PLT\n  call rt_proc_reset@PLT\n");
            if (n_proc_slot > 0) emit_textf("  lea rdi, [rip + __proc]\n  lea rsi, [rip + __proc_names]\n  mov edx, %d\n  call rt_proc_table_fill@PLT\n", n_proc_slot);
            if (n_gva > 0) emit_textf("  mov edi, %d\n  call rt_gva_island@PLT\n  mov rsi, rax\n  lea rdi, [rip + __gva_names]\n  mov edx, %d\n  call gva_register@PLT\n", n_gva, n_gva);
            if (rt_zc_frame_live() == ZC_FRAME_RSP) { /* R12-ERAD: blob self-allocates its FORTH frame; wrapper carves nothing — ICNBENCH-ARGS-RSP (2026-07-18, closes the FENCE): stage argv for the prologue's rt_main_args_fetch bind ([rsp]=argv, [rsp+8]=argc from the push rdi/push rsi preamble) */
                if (sbbg->nparams >= 1) emit_textf("  mov rdi, qword ptr [rsp]\n  add rdi, 8\n  mov esi, dword ptr [rsp + 8]\n  sub esi, 1\n  call rt_main_args_stage@PLT\n");
            } else {
            emit_textf("  sub rsp, 65536\n  mov rdi, rsp\n  mov ecx, 8192\n  xor eax, eax\n  rep stosq\n  mov rdi, rsp\n"); /* ZS-1: main zeta frame on the stack */
            if (sbbg->nparams >= 1)
                emit_textf("  push rdi\n  sub rsp, 8\n  mov rdi, qword ptr [rsp + 65552]\n  add rdi, 8\n  mov esi, dword ptr [rsp + 65560]\n  sub esi, 1\n  call rt_args_list_from@PLT\n  add rsp, 8\n  pop rdi\n"
                       "  mov qword ptr [rdi + 16], rax\n  mov qword ptr [rdi + 24], rdx\n");
            }
            /* ZW-3 R12-FREE-1 REVERSAL (s23l): re-add the r12 seed before flat graph entry. */
            /* ONE-SHOT BRIDGE (Lon s22p): main jmps into the graph; flat_γ / flat_ω are the two port
             * landings defined AFTER the body by bb_glue_outer_gamma/omega (codegen_flat_chain_body).
             * GAS resolves the forward refs.  NO call, NO ret, NO eax -- those belong to a C calling
             * convention that no longer exists.  The graph's ports jump to the landings, which call
             * rt_finalize and exit().  xor esi,esi = match start pos = 0. */
            emit_textf("  mov r12, qword ptr [0x70000000]\n");
            emit_textf("  xor esi, esi\n");
            { extern unsigned char g_rtcc_on; if (g_rtcc_on) emit_textf("  call rtcc_load_all@PLT\n"); }   /* RC-5-GVA: main is the first C→generated crossing; load all claimed GPRs (incl. R9=RT_GVA_VA) from the block before any generated code runs. Gate: g_rtcc_on==0 → no-op (killswitch: byte-identical). */
            emit_textf("  jmp flat_\xce\xb1\n"); /* ONE-SHOT: jmp not call; no ret after the graph */
            g_gva_active = (n_gva > 0) ? 1 : 0;
            { extern IR_graph_t *g_emit_cfg; g_emit_cfg = sbbg; }
            { extern int g_flat_outer_nparams; g_flat_outer_nparams = sbbg->nparams; } /* ICNBENCH-ARGS-RSP: main graph only */
            emit_sep_rule_c('-'); int rc = emit_chain(sbbg->entry, _out, "flat") ? 0 : 1;
            { extern int g_flat_outer_nparams; g_flat_outer_nparams = 0; }
            /* LADDER AB (AB-1): emit activation blocks as dead-code after main chain */
            { extern void bb_ab_emit_nodes(IR_graph_t *g, int gva_active); bb_ab_emit_nodes(sbbg, g_gva_active); }
            g_gva_active = 0;
            g_proc_direct_active = 0;
            g_frame_active = 0;
            xa_emit_strtab_rodata();
            { extern void xa_emit_csettab_rodata(void); xa_emit_csettab_rodata(); }
            emit_textf("  .section .note.GNU-stack,\"\",@progbits\n");
            emit_textf_flush();
            fflush(stdout);
            ir_delete_all(s2);
            return rc;
        }
    }
    if (mode_compile && target_name && strcmp(target_name, "x86") != 0) {
        fprintf(stderr, "[SMX] --target=%s removed (Stack-Machine codegen removed).\n",
                target_name ? target_name : "?");
        ast_tree_free(ast_prog); ast_prog = NULL;
        return 1;
    }
    if (mode_run) {
        extern int g_postfix_resume;
        if (is_icon) g_postfix_resume = 1;
        stage2_t *s2 = sm_preamble(ast_prog, segs, nsegs);
        if (!s2) return 1;
        ast_tree_free(ast_prog); ast_prog = NULL;
        if (is_icon || is_raku || is_sno_bb || is_prolog) {
            extern void rt_proc_register(const char *name, const char **pnames, int nparams);
            extern void rt_proc_reset(void);
            extern bb_box_fn emit_chain(IR_t * entry, FILE * out, const char * prefix);
            extern void rt_proc_set_fn(const char *name, bb_box_fn fn);
            extern int g_frame_active;
            int main_bb_idx = -1;
            rt_proc_reset();
            g_frame_active = 1;
            void *m3_gva_arena = (void *)0;
            {
                extern void gva_collect_reset(void); extern void gva_collect_icon_globals(void); extern int gva_count(void); extern const char *gva_name(int); extern int g_gva_active;
                gva_collect_reset();
                gva_collect_icon_globals();
                int n_gva_m3 = getenv("MONITOR_BIN") ? 0 : gva_count();
                if (n_gva_m3 > 0) {
                    { extern DESCR_t *rt_gva_island(int); m3_gva_arena = rt_gva_island(n_gva_m3); }
                    const char **m3_gva_nms = (const char **)malloc((size_t)n_gva_m3 * sizeof(const char *));
                    for (int _k = 0; _k < n_gva_m3; _k++) m3_gva_nms[_k] = gva_name(_k);
                    if (m3_gva_arena && m3_gva_nms) { gva_register(m3_gva_nms, (DESCR_t *)m3_gva_arena, n_gva_m3); g_gva_active = 1; }
                }
                if (getenv("SCRIP_M3_GVA_TRACE")) fprintf(stderr, "[M3-GVA] m3 globals via pinned island: active=%d n_gva=%d\n", g_gva_active, n_gva_m3);
            }
            for (int _pi = 0; _pi < s2->proc_count; _pi++) {
                const char *pname = s2->proc_table[_pi].name;
                if (!pname) continue;
                if (strcmp(pname, "main") == 0) { main_bb_idx = s2->proc_table[_pi].bb_idx; continue; }
                int idx = s2->proc_table[_pi].bb_idx;
                if (idx < 0 || idx >= s2->bbp.count || !s2->bbp.table[idx] || !s2->bbp.table[idx]->entry) continue;
                int np = s2->proc_table[_pi].nparams;
                const char **pn = NULL;
                if (np > 0) {
                    pn = (const char **)calloc((size_t)np, sizeof(const char *));
                    for (int k = 0; k < np && k < s2->proc_table[_pi].lower_sc.n; k++)
                        pn[k] = s2->proc_table[_pi].lower_sc.e[k].name;
                }
                rt_proc_register(pname, pn, np);
                { extern void rt_proc_set_nformals(const char *, int); rt_proc_set_nformals(pname, s2->proc_table[_pi].nformals); }   /* NPSPLIT (s22w): 0 for unsplit frontends -> runtime falls back to nparams */
                { extern void rt_proc_set_generator(const char *, int); rt_proc_set_generator(pname, s2->proc_table[_pi].is_generator); } { extern void rt_proc_set_jmpentry(const char *, int); rt_proc_set_jmpentry(pname, strncmp(pname, "gram__", 6) != 0); }
                { extern void rt_proc_set_variadic(const char *, int); rt_proc_set_variadic(pname, s2->proc_table[_pi].is_variadic); }
                { extern void rt_proc_set_rest_kind(const char *, int); rt_proc_set_rest_kind(pname, s2->proc_table[_pi].rest_kind); }
                { extern void rt_proc_set_named_rest(const char *, int); rt_proc_set_named_rest(pname, s2->proc_table[_pi].named_rest); }
                { extern void rt_proc_set_dyn_scope(const char *, int); rt_proc_set_dyn_scope(pname, s2->proc_table[_pi].dyn_scope); }
                { extern void rt_proc_set_result_name(const char *, const char *); if (s2->proc_table[_pi].result_name) rt_proc_set_result_name(pname, s2->proc_table[_pi].result_name); }
            }
            if (is_icon || is_sno_bb || is_prolog) { extern void optimizer_run(IR_graph_t * g); for (int _gi = 0; _gi < s2->bbp.count; _gi++) if (s2->bbp.table[_gi]) optimizer_run(s2->bbp.table[_gi]); }
            if (is_icon || is_sno_bb || is_prolog || is_raku || is_pascal) drive_slots_all(s2);
            if (is_raku && !graph_native_emittable_mode(s2, 1)) {
                fprintf(stderr, "[SMX] --run: mode-3 native emitter does not yet cover this program "
                                "(a box has no MEDIUM_BINARY arm — Raku map/grep). REJECTED — native BB emission pending (no interpreter fallback).\n");
                return 0;
            }
            /* PL-FR-4 NAME PRE-PASS (mode 3): register all Prolog graph names in ZLS before any graph is compiled. */
            if (is_prolog) { extern void zls_graph_name(const IR_graph_t *, const char *); for (int _pi2 = 0; _pi2 < s2->proc_count; _pi2++) { const char *_pn2 = s2->proc_table[_pi2].name; if (!_pn2 || strcmp(_pn2, "main") == 0) continue; int _idx2 = s2->proc_table[_pi2].bb_idx; if (_idx2 >= 0 && _idx2 < s2->bbp.count && s2->bbp.table[_idx2]) zls_graph_name(s2->bbp.table[_idx2], _pn2); } }
            for (int _pi = 0; _pi < s2->proc_count; _pi++) {
                const char *pname = s2->proc_table[_pi].name;
                if (!pname || strcmp(pname, "main") == 0) continue;
                int idx = s2->proc_table[_pi].bb_idx;
                if (idx < 0 || idx >= s2->bbp.count || !s2->bbp.table[idx] || !s2->bbp.table[idx]->entry) continue;
                int np = s2->proc_table[_pi].nparams;
                const char **pn = NULL;
                if (np > 0) {
                    pn = (const char **)calloc((size_t)np, sizeof(const char *));
                    for (int k = 0; k < np && k < s2->proc_table[_pi].lower_sc.n; k++)
                        pn[k] = s2->proc_table[_pi].lower_sc.e[k].name;
                }
                { extern IR_graph_t *g_emit_cfg; g_emit_cfg = s2->bbp.table[idx]; }
                { extern void rt_proc_set_generator(const char *, int); rt_proc_set_generator(pname, s2->proc_table[_pi].is_generator); } { extern void rt_proc_set_jmpentry(const char *, int); rt_proc_set_jmpentry(pname, strncmp(pname, "gram__", 6) != 0); }
                { extern void rt_proc_set_variadic(const char *, int); rt_proc_set_variadic(pname, s2->proc_table[_pi].is_variadic); }
                { extern void rt_proc_set_rest_kind(const char *, int); rt_proc_set_rest_kind(pname, s2->proc_table[_pi].rest_kind); }
                { extern void rt_proc_set_named_rest(const char *, int); rt_proc_set_named_rest(pname, s2->proc_table[_pi].named_rest); }
                { extern void rt_proc_set_dyn_scope(const char *, int); rt_proc_set_dyn_scope(pname, s2->proc_table[_pi].dyn_scope); }
                { extern void rt_proc_set_result_name(const char *, const char *); if (s2->proc_table[_pi].result_name) rt_proc_set_result_name(pname, s2->proc_table[_pi].result_name); }
                { extern int g_gen_proc_active; g_gen_proc_active = s2->proc_table[_pi].is_generator; }
                { extern int g_flat_frame_floor; extern int zls_g_region(const IR_graph_t *); IR_graph_t *_pg = s2->bbp.table[idx]; int _is_lbl = pname && strncmp(pname, "LBL__", 5) == 0; g_flat_frame_floor = 0; if (_is_lbl || (_pg && _pg->entry && _pg->entry->op == IR_SAVE_RESTORE && IR_LIT(_pg->entry).ival == 3)) { for (int _mi = 0; _mi < s2->proc_count; _mi++) if (s2->proc_table[_mi].name && !strcmp(s2->proc_table[_mi].name, "main")) { int _mx = s2->proc_table[_mi].bb_idx; if (_mx >= 0 && _mx < s2->bbp.count && s2->bbp.table[_mx]) g_flat_frame_floor = zls_g_region(s2->bbp.table[_mx]); break; } } }   /* SN4-FLAT-PROC (s176): DEFINE stubs carve a fresh MAIN-layout frame -- floor their region at main's; LBL__ body procs share main's bb_idx so _pg->entry is GOTO not SAVE_RESTORE -- the _is_lbl arm closes that gap; LBL__ bodies are CLASS P (wire exit via rt_flat_ret_snap, not CLASS C outer whack) because their callers always push a pcall record via rt_proc_call_open */
                { extern int emit_jmp_entry_for_patproc(const char*, IR_graph_t*); extern int emit_jmp_entry_for_proc(const char*, int, int, IR_graph_t*); extern void emit_jmp_entry_clear(void); extern int g_flat_dc_np; extern int rt_pl_dc_ok(const char *, int);
                  int _isp = emit_jmp_entry_for_patproc(pname, s2->bbp.table[idx]); if (!_isp) emit_jmp_entry_for_proc(pname, s2->proc_table[_pi].dyn_scope, s2->proc_table[_pi].is_generator, s2->bbp.table[idx]);
                  g_flat_dc_np = (!_isp && rt_pl_dc_ok(pname, s2->proc_table[_pi].nparams)) ? s2->proc_table[_pi].nparams : -1; }   /* PL-DC s108: m3 twin of the m4 arming (pat blobs excluded structurally) */
                { if (is_icon || is_sno_bb || is_prolog) { extern void zls_graph_name(const IR_graph_t *, const char *); zls_graph_name(s2->bbp.table[idx], pname); } }   /* ICN-FR-4: zls name registration twin — m3 proc loop. PL-FR-4: added is_prolog. */
                int _islbl3 = pname && strncmp(pname, "LBL__", 5) == 0;   /* s91 M3-UNIFY (the s62-owed rung, Fable): LBL__ pseudo-procs are NOT emitted standalone in m3 either -- their bodies live inline in main's STATEMENT-ORDER chain (mirror of the m4 loop above); registered from main's label pool after main emits */
                bb_box_fn pfn = _islbl3 ? NULL : emit_chain(bb_proc_entry(&s2->proc_table[_pi]), NULL, "proc_flat");
                { extern void emit_jmp_entry_clear(void); emit_jmp_entry_clear(); }
                { extern int g_gen_proc_active; g_gen_proc_active = 0; }
                { extern int g_last_flat_frame_bytes; extern void rt_proc_set_frame_bytes(const char *, int); if (!_islbl3) rt_proc_set_frame_bytes(pname, g_last_flat_frame_bytes); }   /* s91: LBL__ rows take MAIN's geometry after main emits (m4 FB-BACKFILL twin) */
                if (pfn) rt_proc_set_fn(pname, pfn);
                if (pfn) m3_seal_entry_cells(pname, (void *)pfn, 1);   /* R-1 s94: alpha$<FN> <- &<FN>_alpha (DEFINE stub shim face) */
                { extern int g_last_flat_zstatic; extern void rt_proc_set_zstatic(const char *, int); if (pfn) rt_proc_set_zstatic(pname, g_last_flat_zstatic); }   /* PS-1b (s151): m3 in-process twin of the m4 printed rt_proc_set_zstatic — makes SNO$MKPAT-minted DT_P carry real zstatic in --run */
                { extern int g_last_flat_frame_bytes, g_last_flat_fp, g_last_flat_uniform; extern void emit_patzeta_register(const char *, int, int, int); if (!_islbl3) emit_patzeta_register(pname, g_last_flat_frame_bytes, g_last_flat_fp, g_last_flat_uniform); }   /* PS-3 (s152): emit-side Î¶-size registry feed -- suspension footprint terms for DT_P targets, both modes, before main emission */
                { extern long g_last_dc_off; extern void rt_proc_set_dcfn(const char *, void *); if (pfn && g_last_dc_off >= 0) rt_proc_set_dcfn(pname, (void *)((char *)pfn + g_last_dc_off)); }   /* PL-DC s108: seal registration — the fixed slot the m3 sites call through */
                { extern int g_last_flat_frame_bytes; extern void rt_proc_set_frame_bytes(const char *, int); if (pfn && g_last_flat_frame_bytes > 0) rt_proc_set_frame_bytes(pname, g_last_flat_frame_bytes); }
            }
            if (main_bb_idx < 0 || main_bb_idx >= s2->bbp.count || !s2->bbp.table[main_bb_idx]) {
                fprintf(stderr, "[IBB] FATAL: mode-3 driver: main BB graph not found\n");
                abort();
            }
            IR_graph_t * bbg = s2->bbp.table[main_bb_idx];
            if (!bbg->entry) {
                fprintf(stderr, "[IBB] FATAL: mode-3 driver: main BB graph has no entry\n");
                abort();
            }
            extern bb_box_fn emit_chain(IR_t * entry, FILE * out, const char * prefix);
            bb_box_fn fn;
            { extern IR_graph_t *g_emit_cfg; g_emit_cfg = bbg; }
            { extern int g_flat_outer_nparams; g_flat_outer_nparams = bbg->nparams; } /* ICNBENCH-ARGS-RSP: main graph only */
            /* RTX-FUNC-0 AB-EMIT-ORDER (m3): arm the posthook so activation blocks emit inside emit_chain's
             * JIT session.  bb_ab_emit_nodes appends INC_act_α into the same bb_emit_buf and the bind-fix
             * C-store writes bb_emit_buf+lbl.offset into fn_cell BEFORE emitter_end() seals the buffer and
             * BEFORE the SNOBOL4 program runs.  The hook is one-shot (emit_chain clears it on fire) so the
             * proc-chain emit_chain calls above never trigger it. */
            { extern void (*g_emit_chain_posthook)(void); extern void bb_ab_emit_nodes(IR_graph_t*, int); extern int g_gva_active;
              g_ab_posthook_g = bbg; g_ab_posthook_gva = g_gva_active;
              g_emit_chain_posthook = bb_ab_posthook; }
            { int _na = 0; for (int _q = 0; _q < s2->proc_count; _q++) if (s2->proc_table[_q].name && strncmp(s2->proc_table[_q].name, "LBL__", 5) == 0 && s2->proc_table[_q].proc_entry_node) _na++;   /* s91 M3-UNIFY: BODY-ALIAS build, twin of the m4 block (Lon s62) -- each LBL__ entry chased through the transparent relays to its statement_begin, stamped <name>_body on main's graph so the emitter names that statement's alpha */
              if (_na > 0 && bbg->n_balias == 0) { bbg->balias_node = (IR_t **)calloc((size_t)_na, sizeof(IR_t *)); bbg->balias_name = (const char **)calloc((size_t)_na, sizeof(char *));
                  if (bbg->balias_node && bbg->balias_name) for (int _q = 0; _q < s2->proc_count; _q++) { if (!s2->proc_table[_q].name || strncmp(s2->proc_table[_q].name, "LBL__", 5) != 0 || !s2->proc_table[_q].proc_entry_node) continue;
                      if (bbg->n_balias >= _na) break;
                      IR_t * _bn = s2->proc_table[_q].proc_entry_node; int _bgg = 0; while (_bn && (_bn->op == IR_SUCCEED || _bn->op == IR_FAIL || _bn->op == IR_GOTO) && _bn->γ.node && _bgg++ < 65536) _bn = _bn->γ.node;
                      char _ab[300]; snprintf(_ab, sizeof _ab, "%s_body", asm_sym_name(s2->proc_table[_q].name + 5)); bbg->balias_node[bbg->n_balias] = _bn; bbg->balias_name[bbg->n_balias] = strdup(_ab); if (bbg->balias_name[bbg->n_balias]) bbg->n_balias++; } } }
            fn = emit_chain(bbg->entry, NULL, "pat_flat");
            if (fn) { extern int emit_label_lookup_offset(const char *); extern int g_last_flat_frame_bytes; extern void rt_proc_set_frame_bytes(const char *, int); int _mfb = g_last_flat_frame_bytes;   /* s91 M3-UNIFY: register every LBL__ row at its inline body label (main's label pool is live until the next bb_emit_begin); frame_bytes = main's (LBL__ rows share main's frame, ARG/LOCAL index off it) */
              for (int _q = 0; _q < s2->proc_count; _q++) { const char * _ln = s2->proc_table[_q].name; if (!_ln || strncmp(_ln, "LBL__", 5) != 0) continue;
                char _ab[300]; snprintf(_ab, sizeof _ab, "%s_body", asm_sym_name(_ln + 5)); int _off = emit_label_lookup_offset(_ab);
                if (_off < 0) { static int _lw = -1; if (_lw < 0) { const char * e = getenv("SCRIP_M3_UNIFY_DIAG"); _lw = (e && *e == '1') ? 1 : 0; } if (_lw) fprintf(stderr, "[M3-UNIFY] %s: body label %s not defined in main chain\n", _ln, _ab); continue; }
                rt_proc_set_fn(_ln, (bb_box_fn)((char *)fn + _off)); if (_mfb > 0) rt_proc_set_frame_bytes(_ln, _mfb);
                m3_seal_entry_cells(_ln + 5, (void *)fn, 0); } }   /* R-1 s94: body$<ENTRY> <- &<ENTRY>_body (the shim's transfer target) */
            { extern int g_flat_outer_nparams; g_flat_outer_nparams = 0; }
            g_frame_active = 0;
            if (!fn) {
                fprintf(stderr, "[IBB] FATAL: mode-3 driver: emit_chain returned NULL — BB template(s) lack MEDIUM_BINARY arm\n");
                abort();
            }
            ir_delete_all(s2);
            void *mf = NULL;
            if (rt_zc_frame_live() != ZC_FRAME_RSP) { mf = alloca(65536); memset(mf, 0, 65536); } /* ZS-1: main zeta frame on the driver's own stack (was rt_frame() arena memo); R12-ERAD: under RSP the blob self-allocates, rdi unused */
            if (mf && bbg->nparams >= 1) { extern DESCR_t rt_args_list_from(char **v, int n); *(DESCR_t *)((char *)mf + 16) = rt_args_list_from(g_prog_argv, g_prog_argc); }
            if (bbg->nparams >= 1) { extern void rt_main_args_stage(char **, int); rt_main_args_stage(g_prog_argv, g_prog_argc); } /* ICNBENCH-ARGS-RSP: staged channel read by the emitted prologue's rt_main_args_fetch under RSP (harmless when non-RSP took the mf store above) */
            { extern void bbprof_start(void); bbprof_start(); }   /* RUNG BBPROF (Lon 2026-07-20): arm the per-box sampler over the sealed ranges; no-op unless SCRIP_BBPROF=1 */
            { extern void rt_gcheap_warmup(void); rt_gcheap_warmup(); }   /* W1-GC-WARMUP (PL-ZFRAME-RESTORE s9): gc_static_segs_init must run from a C frame with guaranteed-aligned RSP before any JIT blob calls rt_plj_alloc; dl_iterate_phdr's movaps SEGVs when called from inside JIT code whose zframe prologue is absent (derive/divide10/log10/ops8/times10 were all killed by this). */
            if (bbg->zframe_graph && !bbg->icn_cells_graph) {   /* ICN-FR-2: ζ-frame main — supply γ/ω exit wires in rcx/rdx before entering the graph.  R-ZK-A DEFENCE: cells-arm graphs (icn_cells_graph=1) use rt_outer_call — they establish their own ___ pin via GLUE-O enter and restore via `mov rsp,___; pop ___`; passing rcx/rdx wires they never read would corrupt the FORTH spine depth at entry. */
                if (!bbg->icn_zframe_gen) { extern int g_plw_floor_bypass; g_plw_floor_bypass = 1; }   /* W1-BUG2-FIX zframe arm (PL-ZFRAME-RESTORE s10): Prolog zframe graphs have no C-stack-resident cells — set bypass flag so dop_call/dop_call_nothrow skip their floor-set, keeping g_plw_unwind_floor=0; plc_dead_cstack's top-of-function !g_plw_unwind_floor guard then short-circuits before fopen/fgets/sscanf. icn_zframe_gen=0 is the discriminant: Icon generators (=1) NEED the floor; Prolog always 0. jmp entry: no restore needed. */
                icn_zf_main_call((void *)fn, mf, (void *)icn_zf_exit_γ, (void *)icn_zf_exit_ω);
            } else
            { extern void rt_outer_call(bb_box_fn, void *, long); extern int g_plw_floor_bypass; int _bypass = is_prolog && bbg->zframe_graph; if (_bypass) g_plw_floor_bypass = 1; /* W1-BUG2-FIX rt_outer_call arm (PL-ZFRAME-RESTORE s10): zframe Prolog graphs guarantee all trail entries are PLJ-heap-resident; bypass flag stops dop_call/dop_call_nothrow from setting the floor so plc_dead_cstack short-circuits at its top-of-function NULL guard. Conditioned on zframe_graph: SCRIP_PL_ZFRAME=0 may trail C-stack cells and must not bypass. Cleared after rt_outer_call returns. SN4/Icon: _bypass only true for Prolog zframe graphs. */ { extern void rtcc_load_all(void); extern unsigned char g_rtcc_on; if (g_rtcc_on) rtcc_load_all(); }   /* RC-5-GVA: main entry is the first C→generated crossing; load all claimed GPRs (incl. R9=RT_GVA_VA) from the block before any generated code runs. Gate: g_rtcc_on==0 → no-op (killswitch). */ rt_outer_call(fn, mf, 0); if (_bypass) g_plw_floor_bypass = 0; } /* R12-EXTERN (Lon s173): mode-3's OUTSIDE seeds the environment register — push r12 / mov r12,[RT_DCAP_TOP] / call / pop r12 (rt.c thunk); twin of the mode-4 wrapper seed, and closes the old in-blob seed's caller-r12 ABI clobber */
            goto run_done;
        }
        {
            extern bb_box_fn emit_chain(IR_t * entry, FILE * out, const char * prefix);
            extern int g_frame_active;
            extern void rt_proc_register(const char *name, const char **pnames, int nparams);
            extern void rt_proc_set_fn(const char *name, bb_box_fn fn);
            extern void rt_proc_reset(void);
            int main_bb_idx = -1;
            for (int _pi = 0; _pi < s2->proc_count; _pi++)
                if (s2->proc_table[_pi].name && strcmp(s2->proc_table[_pi].name, "main") == 0) { main_bb_idx = s2->proc_table[_pi].bb_idx; break; }
            rt_proc_reset();
            void *m3_gva_arena = (void *)0;
            if (is_pascal) {
                extern void gva_collect_reset(void); extern void gva_collect_graph(IR_graph_t *); extern int gva_count(void); extern const char *gva_name(int); extern int g_gva_active;
                gva_collect_reset();
                IR_graph_t *_mg = (main_bb_idx >= 0 && main_bb_idx < s2->bbp.count) ? s2->bbp.table[main_bb_idx] : (IR_graph_t *)0;
                if (_mg) gva_collect_graph(_mg);
                for (int _pi = 0; _pi < s2->proc_count; _pi++) { int _pgi = s2->proc_table[_pi].bb_idx; if (_pgi >= 0 && _pgi < s2->bbp.count && s2->bbp.table[_pgi] && s2->bbp.table[_pgi] != _mg) gva_collect_graph(s2->bbp.table[_pgi]); }
                int n_gva_m3 = getenv("MONITOR_BIN") ? 0 : gva_count();
                if (n_gva_m3 > 0) {
                    { extern DESCR_t *rt_gva_island(int); m3_gva_arena = rt_gva_island(n_gva_m3); }
                    const char **m3_gva_nms = (const char **)malloc((size_t)n_gva_m3 * sizeof(const char *));
                    for (int _k = 0; _k < n_gva_m3; _k++) m3_gva_nms[_k] = gva_name(_k);
                    if (m3_gva_arena && m3_gva_nms) { gva_register(m3_gva_nms, (DESCR_t *)m3_gva_arena, n_gva_m3); g_gva_active = 1; }
                }
                if (getenv("SCRIP_M3_GVA_TRACE")) fprintf(stderr, "[M3-GVA] m3 globals via pinned island: active=%d n_gva=%d\n", g_gva_active, n_gva_m3);
            }
            drive_slots_all(s2);
            g_frame_active = 1;
            for (int _pi = 0; _pi < s2->proc_count; _pi++) {
                const char *pname = s2->proc_table[_pi].name;
                if (!pname || strcmp(pname, "main") == 0) continue;
                int idx = s2->proc_table[_pi].bb_idx;
                if (idx < 0 || idx >= s2->bbp.count || !s2->bbp.table[idx] || !s2->bbp.table[idx]->entry) continue;
                int np = s2->proc_table[_pi].nparams;
                const char **pn = NULL;
                if (np > 0) {
                    pn = (const char **)calloc((size_t)np, sizeof(const char *));
                    for (int k = 0; k < np && k < s2->proc_table[_pi].lower_sc.n; k++) pn[k] = s2->proc_table[_pi].lower_sc.e[k].name;
                }
                rt_proc_register(pname, pn, np);
                { extern void rt_proc_set_nformals(const char *, int); rt_proc_set_nformals(pname, s2->proc_table[_pi].nformals); }   /* NPSPLIT (s22w): 0 for unsplit frontends -> runtime falls back to nparams */
                { extern void rt_proc_set_frame(const char *, int, int); extern void rt_proc_set_byref(const char *, uint64_t);
                  if (s2->bbp.table[idx]->nslots > 0) rt_proc_set_frame(pname, s2->bbp.table[idx]->nslots - 1, s2->proc_table[_pi].decl_level);
                  rt_proc_set_byref(pname, s2->proc_table[_pi].byref_mask); }
            }
            for (int _pi = 0; _pi < s2->proc_count; _pi++) {
                const char *pname = s2->proc_table[_pi].name;
                if (!pname || strcmp(pname, "main") == 0) continue;
                int idx = s2->proc_table[_pi].bb_idx;
                if (idx < 0 || idx >= s2->bbp.count || !s2->bbp.table[idx] || !s2->bbp.table[idx]->entry) continue;
                int np = s2->proc_table[_pi].nparams;
                const char **pn = NULL;
                if (np > 0) {
                    pn = (const char **)calloc((size_t)np, sizeof(const char *));
                    for (int k = 0; k < np && k < s2->proc_table[_pi].lower_sc.n; k++)
                        pn[k] = s2->proc_table[_pi].lower_sc.e[k].name;
                }
                rt_proc_register(pname, pn, np);
                { extern void rt_proc_set_nformals(const char *, int); rt_proc_set_nformals(pname, s2->proc_table[_pi].nformals); }   /* NPSPLIT (s22w): 0 for unsplit frontends -> runtime falls back to nparams */
                { extern void rt_proc_set_frame(const char *, int, int); extern void rt_proc_set_byref(const char *, uint64_t); extern int g_emit_frame_caller_dl;
                  if (s2->bbp.table[idx]->nslots > 0) rt_proc_set_frame(pname, s2->bbp.table[idx]->nslots - 1, s2->proc_table[_pi].decl_level);
                  rt_proc_set_byref(pname, s2->proc_table[_pi].byref_mask);
                  g_emit_frame_caller_dl = (s2->bbp.table[idx]->nslots > 0) ? s2->proc_table[_pi].decl_level : -1; }
                { extern IR_graph_t *g_emit_cfg; g_emit_cfg = s2->bbp.table[idx]; }
                { extern int g_flat_frame_floor; extern int zls_g_region(const IR_graph_t *); IR_graph_t *_pg = s2->bbp.table[idx]; g_flat_frame_floor = 0; if (_pg && _pg->entry && _pg->entry->op == IR_SAVE_RESTORE && IR_LIT(_pg->entry).ival == 3) { for (int _mi = 0; _mi < s2->proc_count; _mi++) if (s2->proc_table[_mi].name && !strcmp(s2->proc_table[_mi].name, "main")) { int _mx = s2->proc_table[_mi].bb_idx; if (_mx >= 0 && _mx < s2->bbp.count && s2->bbp.table[_mx]) g_flat_frame_floor = zls_g_region(s2->bbp.table[_mx]); break; } } }   /* SN4-FLAT-PROC (s176): DEFINE stubs carve a fresh MAIN-layout frame -- floor their region at main's (see emit_jmp_entry_arm_region); cleared by emit_jmp_entry_clear */
                { extern int emit_jmp_entry_for_patproc(const char*, IR_graph_t*); extern int emit_jmp_entry_for_proc(const char*, int, int, IR_graph_t*); extern void emit_jmp_entry_clear(void); if (!emit_jmp_entry_for_patproc(pname, s2->bbp.table[idx])) emit_jmp_entry_for_proc(pname, s2->proc_table[_pi].dyn_scope, s2->proc_table[_pi].is_generator, s2->bbp.table[idx]); }
                { if (is_icon || is_sno_bb) { extern void zls_graph_name(const IR_graph_t *, const char *); zls_graph_name(s2->bbp.table[idx], pname); } }   /* ICN-FR-4: zls name registration twin — m3 second proc loop (proc_entry_node path) */
                bb_box_fn pfn = emit_chain(s2->proc_table[_pi].proc_entry_node, NULL, "proc_flat");
                { extern void emit_jmp_entry_clear(void); emit_jmp_entry_clear(); }
                { extern int g_emit_frame_caller_dl; g_emit_frame_caller_dl = -1; }
                { extern int g_last_flat_frame_bytes; extern void rt_proc_set_frame_bytes(const char *, int); rt_proc_set_frame_bytes(pname, g_last_flat_frame_bytes); }
                if (pfn) rt_proc_set_fn(pname, pfn);
                if (pfn) m3_seal_entry_cells(pname, (void *)pfn, 1);   /* R-1 s94: alpha$<FN> <- &<FN>_alpha (DEFINE stub shim face) */
                { extern int g_last_flat_zstatic; extern void rt_proc_set_zstatic(const char *, int); if (pfn) rt_proc_set_zstatic(pname, g_last_flat_zstatic); }   /* PS-1b (s151): m3 twin (second proc loop — block/EVAL-thunk phase) */
                { extern int g_last_flat_frame_bytes, g_last_flat_fp, g_last_flat_uniform; extern void emit_patzeta_register(const char *, int, int, int); emit_patzeta_register(pname, g_last_flat_frame_bytes, g_last_flat_fp, g_last_flat_uniform); }   /* PS-3 (s152): emit-side Î¶-size registry feed -- suspension footprint terms for DT_P targets, both modes, before main emission */
            }
            g_frame_active = 0;
            IR_graph_t *sbbg = (main_bb_idx >= 0 && main_bb_idx < s2->bbp.count) ? s2->bbp.table[main_bb_idx] : NULL;
            if (sbbg && sbbg->entry) {
                g_frame_active = 1;
                { extern IR_graph_t *g_emit_cfg; g_emit_cfg = sbbg; }
                { extern int g_flat_outer_nparams; g_flat_outer_nparams = sbbg->nparams; } /* ICNBENCH-ARGS-RSP: main graph only */
                bb_box_fn fn = emit_chain(sbbg->entry, NULL, "pat_flat");
                { extern int g_flat_outer_nparams; g_flat_outer_nparams = 0; }
                g_frame_active = 0;
                ir_delete_all(s2);
                if (fn) {
                    void *mf = NULL;
                    if (rt_zc_frame_live() != ZC_FRAME_RSP) { mf = alloca(65536); memset(mf, 0, 65536); } /* ZS-1; R12-ERAD: under RSP the blob self-allocates, rdi unused */
                    if (sbbg->nparams >= 1) { extern void rt_main_args_stage(char **, int); rt_main_args_stage(g_prog_argv, g_prog_argc); } /* ICNBENCH-ARGS-RSP */
                    { extern void bbprof_start(void); bbprof_start(); }   /* RUNG BBPROF (Lon 2026-07-20) */
                    { extern void rt_gcheap_warmup(void); rt_gcheap_warmup(); }   /* W1-GC-WARMUP twin */
                    { extern void rt_outer_call(bb_box_fn, void *, long); rt_outer_call(fn, mf, 0); } /* R12-EXTERN (Lon s173): twin of the primary mode-3 entry above */
                    { extern int g_gva_active; g_gva_active = 0; } goto run_done;
                }
            }
            fprintf(stderr, "[SBB] mode-3: SNOBOL4 statement shape not yet flat-emittable (a box lacks a "
                            "MEDIUM_BINARY arm); soft fall — no output for this shape. No abort.\n");
            goto run_done;
        }
    } else if (has_non_sno) {
        (void)sm_preamble;
        fprintf(stderr, "[MODE] FATAL: reached dead has_non_sno branch — mode resolution is broken; "
                        "refusing to silently run the SM interpreter. Aborting.\n");
        abort();
    } else {
        fprintf(stderr, "[MODE] FATAL: reached dead default dispatch branch — no execution mode "
                        "selected; refusing to silently run the SM interpreter. Aborting.\n");
        abort();
    }
run_done:
    { extern void bbprof_report(void); bbprof_report(); }   /* RUNG BBPROF: stop timer + per-box sample table; no-op if never armed */
    if (opt_bench) {
        clock_gettime(CLOCK_MONOTONIC, &_t3);
        double parse_ms = (_t1.tv_sec - _t0.tv_sec)*1e3 + (_t1.tv_nsec - _t0.tv_nsec)/1e6;
        double lower_ms = (_t2.tv_sec - _t1.tv_sec)*1e3 + (_t2.tv_nsec - _t1.tv_nsec)/1e6;
        double exec_ms  = (_t3.tv_sec - _t2.tv_sec)*1e3 + (_t3.tv_nsec - _t2.tv_nsec)/1e6;
        fprintf(stderr, "BENCH parse=%.2fms lower=%.2fms exec=%.2fms total=%.2fms\n",
                parse_ms, lower_ms, exec_ms, parse_ms + lower_ms + exec_ms);
    }
    if (getenv("BINARY_AUDIT") || getenv("SNO_BINARY_BOXES")) {
        extern void bin_audit_print(void);
        bin_audit_print();
    }
    return 0;
}
