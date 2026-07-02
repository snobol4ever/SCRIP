#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "lower.h"
/*====================================================================================================================================================================================================*/
int g_postfix_resume = 0;
static int icn_const_step(const tree_t * s, int64_t * bits, int * isr);
/*====================================================================================================================================================================================================*/
typedef struct { IR_graph_t * g; IR_t * psucc; IR_t * pfail; const char ** pn; int npn; const char ** ln; int nln; IR_t * last_gen; IR_t * loop_exit; IR_t * loop_next; IR_t * beta; IR_t * conj_resumable; } icx_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_is_local(const icx_t * cx, const char * nm) { if (!nm) return 0; for (int i = 0; i < cx->nln; i++) if (cx->ln[i] && !strcmp(cx->ln[i], nm)) return 1; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void γ_to(IR_t * nd, IR_t * t) { if (t && ir_is_generator_kind(t->op)) lc_γ_to_β(nd, t); else lc_γ_to(nd, t); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void ω_to(IR_t * nd, IR_t * t) { if (t && ir_is_generator_kind(t->op)) lc_ω_to_β(nd, t); else lc_ω_to(nd, t); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * build(icx_t * cx, IR_e op, IR_t * γ, IR_t * ω) {
    IR_t * nd = lc_build(cx->g, op, γ, ω);
    if (γ && ir_is_generator_kind(γ->op)) lc_γ_to_β(nd, γ);
    if (ω && ir_is_generator_kind(ω->op)) lc_ω_to_β(nd, ω);
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * stmt_subj(const tree_t * s) { return lc_stmt_subj(s); }
/*====================================================================================================================================================================================================*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * icn_cset_canon(const char * s) {
    if (!s) return s;
    unsigned char seen[256]; memset(seen, 0, sizeof seen);
    for (const unsigned char * p = (const unsigned char *) s; *p; p++) seen[*p] = 1;
    char buf[257]; int n = 0;
    for (int c = 0; c < 256; c++) if (seen[c]) buf[n++] = (char) c;
    buf[n] = 0; return lp_strdup(buf);
}
static int icn_proc_is_generator(const char * name) { if (!name) return 0; for (int i = 0; i < g_stage2.proc_count; i++) if (g_stage2.proc_table[i].name && !strcmp(g_stage2.proc_table[i].name, name)) return g_stage2.proc_table[i].is_generator; return 0; }
static int icn_call_allow_gen(const char * name) { return name && (icn_proc_is_generator(name) || !strcmp(name, "find") || !strcmp(name, "upto") || !strcmp(name, "key")); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int augop_code(int aop) {
    switch (aop) {
    case AUGOP_ADD: return 0; case AUGOP_SUB: return 1; case AUGOP_MUL: return 2; case AUGOP_DIV: return 3; case AUGOP_MOD: return 4;
    case AUGOP_POW: return 18; case AUGOP_CONCAT: return 11; case AUGOP_EQ: return 9; case AUGOP_SEQ: return 16;
    case AUGOP_LT: return 5; case AUGOP_LE: return 6; case AUGOP_GT: return 7; case AUGOP_GE: return 8; case AUGOP_NE: return 10;
    case AUGOP_SLT: return 12; case AUGOP_SLE: return 13; case AUGOP_SGT: return 14; case AUGOP_SGE: return 15; case AUGOP_SNE: return 17;
    default: return 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_e icn_augop_binop_tt(int a) {
    switch (a) {
    case AUGOP_ADD: return TT_ADD; case AUGOP_SUB: return TT_SUB; case AUGOP_MUL: return TT_MUL; case AUGOP_DIV: return TT_DIV; case AUGOP_MOD: return TT_MOD; case AUGOP_POW: return TT_POW;
    case AUGOP_CONCAT: return TT_CAT; case AUGOP_EQ: return TT_EQ; case AUGOP_LT: return TT_LT; case AUGOP_LE: return TT_LE; case AUGOP_GT: return TT_GT; case AUGOP_GE: return TT_GE; case AUGOP_NE: return TT_NE;
    case AUGOP_SEQ: return TT_LEQ; case AUGOP_SLT: return TT_LLT; case AUGOP_SLE: return TT_LLE; case AUGOP_SGT: return TT_LGT; case AUGOP_SGE: return TT_LGE; case AUGOP_SNE: return TT_LNE;
    case AUGOP_CSET_UNION: return TT_CSET_UNION; case AUGOP_CSET_DIFF: return TT_CSET_DIFF; case AUGOP_CSET_INTER: return TT_CSET_INTER;
    default: return (tree_e) 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_unop_tt(tree_e tt) {
    switch (tt) {
    case TT_MNS: case TT_PLS: case TT_SIZE: case TT_NONNULL: case TT_CSET_COMPL: case TT_INTERROGATE: return 1;
    default: return 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_resumable(const tree_t * t) {
    if (!t) return 0; if (t->t == TT_STMT) t = stmt_subj(t); if (!t) return 0;
    if (t->t == TT_FNC) { const char * nm = (t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR) ? t->c[0]->v.sval : NULL; return icn_call_allow_gen(nm); }
    if (lc_is_binop(t->t)) { for (int i = 0; i < t->n; i++) if (is_resumable(t->c[i])) return 1; return 0; }
    if (t->t == TT_ASSIGN) { if (t->n > 0 && t->c[0] && t->c[0]->t == TT_ITERATE) return 1; return (t->n > 1) ? is_resumable(t->c[1]) : 0; }
    if (t->t == TT_SWAP) { for (int i = 0; i < t->n; i++) if (is_resumable(t->c[i])) return 1; return 0; }
    switch (t->t) {
    case TT_IF: case TT_SCAN: case TT_EVERY: case TT_TO: case TT_TO_BY: case TT_ALTERNATE: case TT_REPEAT: case TT_WHILE: case TT_UNTIL: case TT_REVASSIGN: case TT_ITERATE: return 1;
    default: return 0; }
}
/*====================================================================================================================================================================================================*/
static IR_t * lower(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_seq(icx_t * cx, const tree_t * t, int argbase, int nargs, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_key(icx_t * cx, const tree_t * t, int argbase, int nargs, IR_t * γ, IR_t * ω, IR_t ** res);
static int icn_subtree_has_suspend(const tree_t *n);
static IR_t * lower_if(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_while(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_to(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_make_list(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_every(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_until(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_repeat(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_not(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_alt(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
/*====================================================================================================================================================================================================*/
static IR_t * icn_arg_lower(void * vcx, const tree_t * a, IR_t * F) {
    icx_t * cx = (icx_t *) vcx; IR_t * sps = cx->psucc; IR_t * spf = cx->pfail;
    cx->psucc = NULL; cx->pfail = F;
    IR_t * r = NULL; IR_t * e = lower(cx, a, NULL, F, &r);
    cx->psucc = sps; cx->pfail = spf;
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_graph_t * arg_block(void * vcx, const tree_t * a) { return lc_arg_block(&((icx_t *) vcx)->g, IR_LANG_ICN, icn_arg_lower, vcx, a); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_arg_is_scan_fn(const tree_t * a) { if (!a) return 0; if (a->t == TT_STMT) a = stmt_subj(a); if (!a || a->t != TT_FNC) return 0; const char * nm = (a->n > 0 && a->c[0] && a->c[0]->t == TT_VAR) ? a->c[0]->v.sval : NULL; return nm && (!strcmp(nm, "tab") || !strcmp(nm, "move") || !strcmp(nm, "pos") || !strcmp(nm, "any") || !strcmp(nm, "match") || !strcmp(nm, "many") || !strcmp(nm, "upto") || !strcmp(nm, "find") || !strcmp(nm, "bal")); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_call(icx_t * cx, const char * name, const tree_t * t, int argbase, int nargs, IR_t * γ, IR_t * ω, IR_t ** res) {
    if (name && !strcmp(name, "seq")) { IR_t * sq = lower_seq(cx, t, argbase, nargs, γ, ω, res); if (sq) return sq; }
    if (name && !strcmp(name, "key") && nargs == 1) { IR_t * kg = lower_key(cx, t, argbase, nargs, γ, ω, res); if (kg) return kg; }
    IR_t * call = build(cx, icn_proc_is_generator(name) ? IR_PROC_GEN : IR_CALL, γ, ω); IR_LIT(call).sval = (char *) name;
    if (res) *res = call;
    int chains = name && (!strcmp(name, "write") || !strcmp(name, "writes"));
    int is_cursor_mover = name && (!strcmp(name, "tab") || !strcmp(name, "move"));
    if (!chains) { for (int k = 0; k < nargs; k++) if (is_resumable(t->c[argbase + k])) { if (is_cursor_mover && icn_arg_is_scan_fn(t->c[argbase + k])) continue; chains = 1; break; } }
    IR_t * prev = NULL; IR_t * entry = call; IR_t * aω = ω; IR_t * last_ar = NULL;
    for (int k = 0; k < nargs; k++) {
        const tree_t * a = t->c[argbase + k]; IR_t * ar = NULL;
        IR_t * ae = lower(cx, a, (k == nargs - 1) ? call : NULL, aω, &ar); aω = cx->beta;
        if (k == 0) entry = ae;
        if (prev) γ_to(prev, ae);
        prev = ar;
        if (ar) { ir_operand_push(call, ar); last_ar = ar; }
    }
    if (icn_proc_is_generator(name) && last_ar) lc_γ_to(last_ar, call);
    cx->beta = icn_proc_is_generator(name) ? call : (g_postfix_resume ? aω : ω);
    return entry;
}
/*====================================================================================================================================================================================================*/
static IR_t * lower_idx_var(icx_t * cx, const tree_t * t, IR_t * ω, IR_t ** var_res) {
    /* IDX-UNIFY (GOAL-IR-IMMUTABLE-EMIT, RECON 2 + r1 tvsubs): x[i] / x[i,j,...] as a chain of 2-operand IR_SUBSCRIPT VARIABLE producers (operands[0]=base, [1]=index). The rval/lval MODE that
       JCON rides ON the "[]" operator (irgen.icn:494-499) is classified BY NAME here: IR_SUBSCRIPT yields the variable, the CALLER decides — rvalue wraps the final node in IR_DEREF, assignment
       feeds it to IR_ASSIGN_VAR. An identifier base produces IR_VAR_REF (DT_V over the variable's own cell — ζ varslot or GVA slot), and DT_V flows THROUGH the chain: rt_subscript_var derefs a
       DT_V base internally (canonical subsc, oref.r:710-758 — operate on the value, keep the variable), so the between-level IR_DEREF nodes of phase 1 are retired. Lists: genuine cell pointer.
       Tables: lazy {tbl,key} trap (canonical tvtbl — a read NEVER inserts, assignment does). Strings under a variable base: tvsubs trap {ssvar,pos,len} — assignment splices prefix+src+suffix and
       writes back RECURSIVELY through ssvar (subs_asgn, oasgn.r:345+; the recursive rt_assign_var collapses canonical's type_case, so t["k"][2]:=v lazily re-derefs the tvtbl beneath); trap len
       updates on assign so revassign's β-restore reuses the same trap (canonical rasgn). Strings under a VALUE base stay plain value reads (probe 63). rt_subscript_var/rt_deref/rt_assign_var,
       pattern_match.c. Wiring mirrors TT_SECTION: serial γ_to operand chain, every ω to the construct ω.
       BETA: inherited from the last-lowered operand (the index) — the resume path an index generator leaves behind (every write(s[1 to 3])) must survive, exactly as lower_call's postfix arg machinery preserves it; clobbering cx->beta=ω here killed generator-index resumption (rung16_subscript_sub_every). Final node's γ is re-aimed by the caller (build with NULL γ then γ_to, the TT_SECTION operand precedent). */
    if (t->n < 2 || !t->c[0]) { IR_t * su = build(cx, IR_SUCCEED, NULL, ω); *var_res = su; return su; }
    IR_t * br = NULL; IR_t * entry;
    const tree_t * b0 = t->c[0];
    if (b0->t == TT_VAR && b0->v.sval && b0->v.sval[0] != '&') {
        IR_t * vr = build(cx, IR_VAR_REF, NULL, ω); IR_LIT(vr).sval = b0->v.sval; br = vr; entry = vr;
    } else if (b0->t == TT_IDX) {
        entry = lower_idx_var(cx, b0, ω, &br);
    } else entry = lower(cx, b0, NULL, ω, &br);
    IR_t * cur = br; IR_t * hook = br;
    for (int k = 1; k < t->n; k++) {
        IR_t * ir = NULL; IR_t * ie = lower(cx, t->c[k], NULL, ω, &ir);
        γ_to(hook, ie);
        IR_t * sub = build(cx, IR_SUBSCRIPT, NULL, ω);
        γ_to(ir, sub);
        ir_operand_push(sub, cur); ir_operand_push(sub, ir);
        cur = sub; hook = sub;
    }
    *var_res = cur; return entry;
}
/*====================================================================================================================================================================================================*/
static IR_t * lower_lvalue_var(icx_t * cx, const tree_t * t, IR_t * ω, IR_t ** var_res) {
    /* SWAP-LV: lower ANY supported lvalue tree to a VARIABLE producer (DT_V VCELL), the shared front for
       constructs that need BOTH operands underef (canonical swap's `underef x -> dx` signature, oasgn.r:267).
       Covers the three lvalue kinds the TT_ASSIGN arms already serve individually: identifier → IR_VAR_REF;
       x[i...] → lower_idx_var chain; s[i:j] / s[i+:n] / s[i-:n] → the 3-operand IR_SUBSCRIPT sval="lv" section
       variable (rt_section_var tvsubs trap), the LV-1 shape verbatim incl. the synthetic-BINOP desugar.
       Returns entry, *var_res = the variable node (γ left NULL — caller aims it). NULL = unsupported kind
       (keywords &pos/&subject, fields — own future rungs); caller falls back or bombs loud. */
    if (!t) return NULL;
    if (t->t == TT_VAR && t->v.sval && t->v.sval[0] != '&') {
        IR_t * vr = build(cx, IR_VAR_REF, NULL, ω); IR_LIT(vr).sval = t->v.sval; *var_res = vr; return vr;
    }
    if (t->t == TT_IDX) return lower_idx_var(cx, t, ω, var_res);
    if (t->t == TT_ITERATE && t->n > 0 && t->c[0]) {
        /* !x element-VARIABLE producer (the LV-2 mint, relocated into the shared front — LVALUE-COLLAPSE increment 1): generator-kind, cx->beta = it per the LV-2 rule (a later-lowered resumable
           overrides — most-recent-generator wins, canonical resume order). Consumers get bang-lvalues free: swap (shuffle's `every !x :=: ?x`), augop generic arm, nulltest arm. */
        IR_t * it = build(cx, IR_ITERATE, NULL, ω); IR_LIT(it).ival = 0; IR_LIT(it).sval = "lv";
        const tree_t * b0 = t->c[0]; IR_t * ar = NULL; IR_t * ae;
        if (b0->t == TT_VAR && b0->v.sval && b0->v.sval[0] != '&') { IR_t * vr = build(cx, IR_VAR_REF, NULL, ω); IR_LIT(vr).sval = b0->v.sval; ar = vr; ae = vr; }
        else ae = lower(cx, b0, NULL, ω, &ar);
        lc_γ_to(ar, it); ir_operand_push(it, ar);
        cx->beta = it;
        *var_res = it; return ae;
    }
    if (t->t == TT_RANDOM && t->n > 0 && t->c[0]) {
        /* ?x random-element VARIABLE producer (the LV-3a mint, relocated — operator{0,1}, single-shot: beta deliberately NOT written, so a sibling bang's resume survives). */
        IR_t * rn = build(cx, IR_RANDOM, NULL, ω);
        const tree_t * b0 = t->c[0]; IR_t * ar = NULL; IR_t * ae;
        if (b0->t == TT_VAR && b0->v.sval && b0->v.sval[0] != '&') { IR_t * vr = build(cx, IR_VAR_REF, NULL, ω); IR_LIT(vr).sval = b0->v.sval; ar = vr; ae = vr; }
        else ae = lower(cx, b0, NULL, ω, &ar);
        lc_γ_to(ar, rn); ir_operand_push(rn, ar);
        *var_res = rn; return ae;
    }
    if ((t->t == TT_SECTION || t->t == TT_SECTION_PLUS || t->t == TT_SECTION_MINUS) && t->n >= 3 && t->c[0] && t->c[1] && t->c[2]) {
        int sec_variant = (t->t == TT_SECTION_PLUS) ? 1 : (t->t == TT_SECTION_MINUS) ? 2 : 0;
        IR_t * sec = build(cx, IR_SUBSCRIPT, NULL, ω); IR_LIT(sec).ival = 0; IR_LIT(sec).sval = "lv";
        IR_t * ar = NULL; IR_t * ae; const tree_t * b0 = t->c[0];
        if (b0->t == TT_VAR && b0->v.sval && b0->v.sval[0] != '&') { IR_t * vr = build(cx, IR_VAR_REF, NULL, ω); IR_LIT(vr).sval = b0->v.sval; ar = vr; ae = vr; }
        else ae = lower(cx, b0, NULL, ω, &ar);
        IR_t * br = NULL; IR_t * be = lower(cx, t->c[1], NULL, ω, &br); γ_to(ar, be);
        IR_t * cr = NULL; IR_t * ce = lower(cx, t->c[2], sec_variant ? NULL : sec, ω, &cr); γ_to(br, ce);
        if (sec_variant) { IR_t * op = build(cx, IR_BINOP, sec, ω); IR_LIT(op).ival = (sec_variant == 1) ? BINOP_ADD : BINOP_SUB; ir_operand_push(op, br); ir_operand_push(op, cr); γ_to(cr, op); cr = op; }
        ir_operand_push(sec, ar); ir_operand_push(sec, br); ir_operand_push(sec, cr);
        *var_res = sec; return ae;
    }
    if (t->t == TT_FIELD && t->n > 0 && t->c[0]) {
        /* LVALUE-COLLAPSE increment 2: a.f field-VARIABLE producer — IR_FIELD_GET ival=1 selects bb_field_get's lv arm; rt_field_var (the rt_subscript_var record-arm sibling, data_field_ptr
           name→cell) mints the VCELL over the field cell. Base lowers as a value (record descr is a reference; rt_field_var derefs a DT_V base internally, rt_subscript_var parity). */
        IR_t * fg = build(cx, IR_FIELD_VAR, NULL, ω);
        IR_LIT(fg).sval = (t->n > 1 && t->c[1]) ? t->c[1]->v.sval : t->v.sval;
        IR_t * br = NULL; IR_t * be = lower(cx, t->c[0], NULL, ω, &br);
        lc_γ_to(br, fg); ir_operand_push(fg, br);
        *var_res = fg; return be;
    }
    if ((t->t == TT_NULL || t->t == TT_NONNULL) && t->n > 0 && t->c[0]) {
        /* LVALUE-COLLAPSE increment 2: /x and \x are variable-transparent (canonical ir_rval, irgen.icn:463-466 — '/' '\' arity-1 INHERIT parent rval-ness; onull.r yields the VARIABLE on
           success). The child lowers through this same front; IR_UNOP_TEST sval="lv" selects bb_unop's lv arm — rt_deref for the test, the VARIABLE forwarded on success. Single-shot glue;
           a resumable child (bang) keeps its own β via cx->beta. */
        IR_t * clv = NULL; IR_t * ce = lower_lvalue_var(cx, t->c[0], ω, &clv);
        if (!ce || !clv) return NULL;
        IR_t * ut = build(cx, IR_NULLTEST_VAR, NULL, ω); IR_LIT(ut).sval = (t->t == TT_NONNULL) ? "nonnull" : "null";
        ir_operand_push(ut, clv); lc_γ_to(clv, ut);
        *var_res = ut; return ce;
    }
    return NULL;
}
/*====================================================================================================================================================================================================*/
static int icn_scan_kind_for(const char * s) {
    if (!s) return 0;
    if (!strcmp(s,"tab"))   return (int)IR_SCAN_TAB;
    if (!strcmp(s,"move"))  return (int)IR_SCAN_MOVE;
    if (!strcmp(s,"upto"))  return (int)IR_SCAN_UPTO;
    if (!strcmp(s,"any"))   return (int)IR_SCAN_ANY;
    if (!strcmp(s,"many"))  return (int)IR_SCAN_MANY;
    if (!strcmp(s,"find"))  return (int)IR_SCAN_FIND;
    if (!strcmp(s,"match")) return (int)IR_SCAN_MATCH;
    if (!strcmp(s,"pos"))   return (int)IR_SCAN_POS;
    if (!strcmp(s,"bal"))   return (int)IR_SCAN_BAL;
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void icn_retag_scan_body(IR_graph_t * g, int depth) {
    if (!g || !g->all || depth > 16) return;
    for (int i = 0; i < g->n; i++) {
        IR_t * nd = g->all[i];
        if (!nd) continue;
        if ((nd->op == IR_CALL || nd->op == IR_CALL_BUILTIN) && IR_LIT(nd).sval) { int k = icn_scan_kind_for(IR_LIT(nd).sval); if (k) nd->op = (IR_e) k; }
    }
}
/*====================================================================================================================================================================================================*/
static IR_t * lc_key(icx_t * cx, const tree_t * t, const char * kw, IR_t * γ, IR_t * ω, IR_t ** res) {
    const char * id = (kw && kw[0] == '&') ? kw + 1 : kw;
    if (id && !strcmp(id, "line")) { IR_t * nd = build(cx, IR_LIT_INTEGER, γ, ω); IR_LIT(nd).ival = (t && t->line > 0) ? t->line : 0; *res = nd; return nd; }
    if (id && !strcmp(id, "file")) { IR_t * nd = build(cx, IR_LIT_STRING, γ, ω); IR_LIT(nd).sval = (char *) ""; *res = nd; return nd; }
    IR_t * nd = build(cx, IR_KEYWORD, γ, ω); IR_LIT(nd).sval = (char *) kw; *res = nd; return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * dummy = NULL; if (!res) res = &dummy;
    cx->beta = ω;
    if (!t) { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    if (lc_is_binop(t->t)) {
        { int64_t fb = 0; int fr = 0; if (icn_const_step(t, &fb, &fr) && fr) { IR_t * nd = build(cx, IR_LIT_REAL, γ, ω); double d; memcpy(&d, &fb, 8); IR_LIT(nd).dval = d; *res = nd; return nd; } }
        int64_t bcode = lc_binop_code(t->t); int is_relop = (bcode >= BINOP_LT && bcode <= BINOP_NE) || (bcode >= BINOP_SLT && bcode <= BINOP_SNE);
        IR_t * op = build(cx, is_relop ? IR_BINOP_TEST : IR_BINOP, γ, ω); IR_LIT(op).ival = bcode;
        IR_t * lr = NULL, * rr = NULL; IR_t * ea = lower(cx, t->c[0], NULL, ω, &lr); IR_t * lβ = cx->beta; IR_t * eb = lower(cx, t->c[1], op, lβ, &rr);
        IR_t * rβ = cx->beta;
        IR_t * opfail = (rβ && rβ != ω && rβ != op) ? rβ : ((lβ && lβ != ω && lβ != op) ? lβ : NULL);
        if ((IR_LIT(op).ival >= 5 && IR_LIT(op).ival <= 10) && opfail) ω_to(op, opfail);
        γ_to(lr, eb); ir_operand_push(op, lr); ir_operand_push(op, rr);
        cx->beta = (rβ && rβ != ω && rβ != op) ? rβ : ((lβ && lβ != ω && lβ != op) ? lβ : ω);
        *res = op; return ea; }
    if (is_unop_tt(t->t)) {
        { int64_t fb = 0; int fr = 0; if (icn_const_step(t, &fb, &fr)) { if (fr) { IR_t * nd = build(cx, IR_LIT_REAL, γ, ω); double d; memcpy(&d, &fb, 8); IR_LIT(nd).dval = d; *res = nd; return nd; } IR_t * nd = build(cx, IR_LIT_INTEGER, γ, ω); IR_LIT(nd).ival = fb; *res = nd; return nd; } }
        IR_e uop_kind = (t->t == TT_NONNULL) ? IR_UNOP_TEST : IR_UNOP;
        IR_t * op = build(cx, uop_kind, γ, ω); IR_LIT(op).ival = (long long) t->t; IR_t * orr = NULL; IR_t * ea = lower(cx, t->c[0], op, ω, &orr); ir_operand_push(op, orr); *res = op; return ea; }
    switch (t->t) {
    case TT_ILIT: { IR_t * nd = build(cx, IR_LIT_INTEGER, γ, ω); IR_LIT(nd).ival = t->v.ival; *res = nd; return nd; }
    case TT_FLIT: { IR_t * nd = build(cx, IR_LIT_REAL, γ, ω); IR_LIT(nd).dval = t->v.dval; *res = nd; return nd; }
    case TT_QLIT: { IR_t * nd = build(cx, IR_LIT_STRING, γ, ω); IR_LIT(nd).sval = t->v.sval; *res = nd; return nd; }
    /* CORRECTED (Claude Sonnet 4.6, 2026-06-30): IR_LIT_STRING+ival=1 clobbered sval (union) → crash in bb_scan_any.
       Fix: IR_LIT_CHARSET opcode; only sval set; bb_lit_scalar's CHARSET arm (keyed on op_node_kind) emits the slen=-1 IS_CSET sentinel itself (the old op_ival=1 drive staging was dead — clobber audit 2026-07-02). */
    case TT_CSET: { IR_t * nd = build(cx, IR_LIT_CHARSET, γ, ω); IR_LIT(nd).sval = icn_cset_canon(t->v.sval); *res = nd; return nd; }
    case TT_NULL: { if (t->n > 0 && t->c[0]) { IR_t * op = build(cx, IR_UNOP_TEST, γ, ω); IR_LIT(op).ival = (long long) TT_NULL; IR_t * orr = NULL; IR_t * ea = lower(cx, t->c[0], op, ω, &orr); ir_operand_push(op, orr); *res = op; return ea; } IR_t * nd = build(cx, IR_FAIL, γ, ω); *res = nd; return nd; }
    case TT_VAR: { if (t->v.sval && t->v.sval[0] == '&') return lc_key(cx, t, t->v.sval, γ, ω, res); IR_t * nd = build(cx, IR_VAR, γ, ω); IR_LIT(nd).sval = t->v.sval; *res = nd; return nd; }
    case TT_KEYWORD: return lc_key(cx, t, t->v.sval, γ, ω, res);
    case TT_FIELD: { IR_t * nd = build(cx, IR_FIELD_GET, γ, ω);
        IR_LIT(nd).sval = (t->n > 1 && t->c[1]) ? t->c[1]->v.sval : t->v.sval;
        IR_t * br = NULL; IR_t * ea = lower(cx, t->c[0], nd, ω, &br); ir_operand_push(nd, br); *res = nd; return ea; }
    case TT_FNC: { const tree_t * fn = (t->n > 0) ? t->c[0] : NULL;
        /* PROC-VALUE by-value invoke: a callee that is a DECLARED local (p := sin; p(x)) or any non-identifier
           expression cannot be a by-NAME call — it is a runtime procedure/string/integer VALUE (canonical invoke,
           oref.r) — lower the callee as a value producer and mint IR_CALL_VALUE (operands[0]=callee, [1..]=args;
           rt_call_value resolves proc-value / string-name / integer-selection). Identifier callees NOT declared
           local stay the by-name lower_call route (the resolve pass classifies proc/builtin as before). */
        if (!fn || (fn->t == TT_VAR && fn->v.sval && fn->v.sval[0] != '&' && !icn_is_local(cx, fn->v.sval))) {
            const char * nm = (fn && fn->t == TT_VAR) ? fn->v.sval : "?";
            return lower_call(cx, nm, t, 1, t->n - 1, γ, ω, res);
        }
        IR_t * cr = NULL; IR_t * ce = lower(cx, fn, NULL, ω, &cr);
        IR_t * nd = build(cx, IR_CALL_VALUE, γ, ω);
        IR_t * prev = cr;
        ir_operand_push(nd, cr);
        for (int i = 1; i < t->n; i++) {
            IR_t * ar = NULL; IR_t * ae = lower(cx, t->c[i], NULL, ω, &ar);
            lc_γ_to(prev, ae); prev = ar;
            ir_operand_push(nd, ar);
        }
        lc_γ_to(prev, nd);
        *res = nd; return ce; }
    case TT_RANDOM: {
        /* ASSIGN-LV LV-3a: ?x — classify-by-name (JCON collapses to opfn u_random/"Select"; SCRIP splits per the JCON-ALIGNMENT directive). operator{0,1}: single-shot, no β. rt_random_var rolls the
           canonical LCG once (RandA/RandC/RanScale, oref.r:216; state = g_random, the &random keyword's cell) and mints the LV VCELL family (string-under-variable tvsubs len=1 / list cell / record field /
           table nth-pair lazy trap) or a plain value (cset char, string-value char, ?n int in [1,n], ?0 real). Identifier base → IR_VAR_REF (string tvsubs needs the variable, lower_idx_var parity); rvalue
           = IR_DEREF partner (identity on non-DT_V); the TT_ASSIGN/TT_AUGOP lv arms consume the variable directly. */
        IR_t * rn = build(cx, IR_RANDOM, NULL, ω);
        const tree_t * b0 = (t->n > 0) ? t->c[0] : NULL; if (!b0) { IR_t * f = build(cx, IR_FAIL, γ, ω); *res = f; return f; }
        IR_t * ar = NULL; IR_t * ae;
        if (b0->t == TT_VAR && b0->v.sval && b0->v.sval[0] != '&') { IR_t * vr = build(cx, IR_VAR_REF, NULL, ω); IR_LIT(vr).sval = b0->v.sval; ar = vr; ae = vr; }
        else ae = lower(cx, b0, NULL, ω, &ar);
        lc_γ_to(ar, rn); ir_operand_push(rn, ar);
        IR_t * drf = build(cx, IR_DEREF, γ, ω); lc_γ_to(rn, drf); ir_operand_push(drf, rn);
        *res = drf; return ae; }
    case TT_IDX: {
        /* IDX-UNIFY rvalue: variable-producing IR_SUBSCRIPT chain + IR_DEREF partner replaces lower_call("[]") — see lower_idx_var. */
        IR_t * vr = NULL; IR_t * e = lower_idx_var(cx, t, ω, &vr);
        IR_t * drf = build(cx, IR_DEREF, γ, ω); lc_γ_to(vr, drf); ir_operand_push(drf, vr);
        *res = drf; return e; }
    case TT_MAKELIST: case TT_VLIST: return lower_make_list(cx, t, γ, ω, res);
    case TT_ASSIGN: {
        const tree_t * lhs = t->c[0]; const tree_t * rhs = t->c[1];
        if (lhs && lhs->t == TT_VAR) { IR_t * asn = build(cx, IR_ASSIGN, γ, ω); IR_LIT(asn).sval = lhs->v.sval;
            IR_t * vr = NULL; IR_t * entry = lower(cx, rhs, asn, ω, &vr); ir_operand_push(asn, vr); *res = asn; return entry; }
        { IR_t * b4 = cx->beta;
          IR_t * lv = NULL; IR_t * lve = lhs ? lower_lvalue_var(cx, lhs, ω, &lv) : NULL;
          if (lve && lv) {
            /* LVALUE-COLLAPSE increment 2: ONE generic write path — lower_lvalue_var (TOTAL: identifier/IDX/section/bang/random/field/nulltest) yields the VARIABLE; rhs; IR_ASSIGN_VAR writes
               through (operands[0]=variable, [1]=value; lhs-then-rhs per canonical asgn). β-delta: a resumable lv (bang) sets cx->beta inside the front — rhs ω aims there so a failing rhs
               re-pumps the generator (the deleted LV-2 arm's wiring, now inherited); non-resumable lvalues keep ω. Replaces the six per-kind arms (IDX / LV-1 section / LV-2 iterate /
               LV-3a random / NULLTEST-LV / FIELD_SET) this case carried. */
            IR_t * lvbeta = (cx->beta != b4) ? cx->beta : NULL;
            IR_t * asn = build(cx, IR_ASSIGN_VAR, γ, ω);
            IR_t * rr = NULL; IR_t * re = lower(cx, rhs, asn, lvbeta ? lvbeta : ω, &rr);
            lc_γ_to(lv, re);
            ir_operand_push(asn, lv); ir_operand_push(asn, rr);
            *res = asn; return lve;
          } }
        IR_t * asn = build(cx, IR_ASSIGN, γ, ω); IR_t * lr = NULL, * rr = NULL;
        IR_t * eb = lower(cx, rhs, asn, ω, &rr); IR_t * ea = lower(cx, lhs, eb, ω, &lr);
        ir_operand_push(asn, rr); ir_operand_push(asn, lr); *res = asn; return ea;
    }
    case TT_AUGOP: {
        const tree_t * lhs = t->c[0]; const tree_t * rhs = t->c[1]; int bc = augop_code((int) t->v.ival);
        if (lhs && lhs->t == TT_VAR && rhs && icn_augop_binop_tt((int) t->v.ival) != (tree_e) 0) {
            tree_t * bo = ast_node_new(icn_augop_binop_tt((int) t->v.ival)); ast_push(bo, (tree_t *) lhs); ast_push(bo, (tree_t *) rhs);
            tree_t * as = ast_node_new(TT_ASSIGN); ast_push(as, (tree_t *) lhs); ast_push(as, bo);
            return lower(cx, as, γ, ω, res);
        }
        { IR_t * b4 = cx->beta;
          IR_t * lv = NULL; IR_t * lve = (lhs && rhs) ? lower_lvalue_var(cx, lhs, ω, &lv) : NULL;
          if (lve && lv) {
            /* LVALUE-AUGOP: x[i] OP:= v / s[i:j] OP:= v — the LV-3a once-evaluated rider shape verbatim on the shared lower_lvalue_var front (variable-producer → IR_DEREF reads old →
               IR_BINOP folds rhs → IR_ASSIGN_VAR writes back through the SAME variable; deref and write-back both read lv->tmp). Kills the operand-less IR_BINOP mint-and-abandon fallthrough
               for every lvalue kind lower_lvalue_var serves. */
            IR_t * asn = build(cx, IR_ASSIGN_VAR, γ, ω);
            IR_t * op = build(cx, IR_BINOP, asn, ω); IR_LIT(op).ival = bc;
            IR_t * dr = build(cx, IR_DEREF, NULL, ω);
            ir_operand_push(dr, lv);
            lc_γ_to(lv, dr);
            IR_t * lvbeta = (cx->beta != b4) ? cx->beta : NULL;
            IR_t * rr = NULL; IR_t * re = lower(cx, rhs, op, lvbeta ? lvbeta : ω, &rr);
            lc_γ_to(dr, re);
            ir_operand_push(op, dr); ir_operand_push(op, rr);
            ir_operand_push(asn, lv); ir_operand_push(asn, op);
            *res = asn; return lve;
          }
        }
        IR_t * op = build(cx, IR_BINOP, γ, ω); IR_LIT(op).ival = bc; IR_t * lr = NULL, * rr = NULL;
        IR_t * ea = lower(cx, lhs, NULL, ω, &lr); IR_t * eb = lower(cx, rhs, op, ω, &rr); γ_to(lr, eb); *res = op; return ea;
    }
    case TT_RETURN: { IR_t * ret = build(cx, IR_RETURN, cx->psucc ? cx->psucc : γ, ω);
        if (t->n > 0 && t->c[0]) { IR_t * vr = NULL; IR_t * entry = lower(cx, t->c[0], ret, cx->pfail ? cx->pfail : ω, &vr); ir_operand_push(ret, vr); *res = ret; return entry; }
        *res = ret; return ret; }
    case TT_PROC_FAIL: { IR_t * nd = build(cx, IR_FAIL, γ, ω); *res = nd; return nd; }
    case TT_LOOP_BREAK: { IR_t * lx = cx->loop_exit; IR_t * nd = lx ? build(cx, IR_GOTO, lx, lx) : build(cx, IR_FAIL, γ, ω); *res = nd; return nd; }
    case TT_LOOP_NEXT: { IR_t * ln = cx->loop_next; IR_t * nd = ln ? build(cx, IR_GOTO, ln, ln) : build(cx, IR_FAIL, γ, ω); *res = nd; return nd; }
    case TT_LOCAL: case TT_STATIC_DECL: { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    case TT_INITIAL: {
        /* JCON ir_a_Initial:
           ir.start → expr.start; ir.success = p.ir.success (both body.success AND body.failure go there).
           ir.resume → unreachable (initial blocks never resume).
           SCRIP: IR_INITIAL node gates the body:
             γ = body entry (first call: flag was 0 → set flag=1, jump into body)
             ω = skip path (subsequent calls: flag is 1, jump directly to outer continuation γ)
           body.success → outer γ  (= IR_INITIAL.ω target)
           body.failure → outer γ  (same; initial block always "succeeds" from caller's POV)
           slot: IR_INITIAL.tmp (16 bytes: [+0..+7] DESCR padding, [+8..+15] int64 done-flag init to 0) */
        IR_t * ini = build(cx, IR_INITIAL, NULL, γ);   /* ω = outer γ = "already done" skip path */
        if (t->n > 0 && t->c[0]) {
            /* Lower body; both body.success and body.failure → outer γ (= ini.ω target) */
            IR_t * br = NULL;
            IR_t * be = lower(cx, t->c[0], γ, γ, &br);   /* body.success=γ, body.failure=γ */
            lc_γ_to(ini, be);                              /* ini.γ → body entry */
            ir_operand_push(ini, br);                      /* operand[0] = body result (unused, for completeness) */
        } else {
            lc_γ_to(ini, γ);  /* no body: ini.γ → outer γ directly */
        }
        *res = ini; return ini; }
    case TT_SUSPEND: { IR_t * sn = build(cx, IR_SUSPEND, cx->psucc ? cx->psucc : γ, ω); IR_LIT(sn).dval = 1.0;
        IR_t * ev = NULL; IR_t * e_entry = sn;
        if (t->n > 0 && t->c[0]) { e_entry = lower(cx, t->c[0], sn, cx->pfail ? cx->pfail : ω, &ev); }
        ir_operand_push(sn, ev);
        if (t->n > 1 && t->c[1]) { IR_t * dv = NULL; IR_t * d_entry = lower(cx, t->c[1], γ, γ, &dv); ir_operand_push(sn, d_entry); }
        else ir_operand_push(sn, γ);
        *res = sn; return e_entry; }
    case TT_CASE: {
        /* JCON ir_a_Case (bounded): eval subject once → chain === tests → body → p.success.
           SCRIP: each arm body evaluates into IR_ASSIGN("__case_N") → IR_VAR("__case_N") as result.
           All arms write to distinct local names but we return the FIRST arm's IR_VAR as *res.
           Each arm's IR_ASSIGN writes to a synthetic local variable whose frame slot bb_varslot
           allocates on first use. The outer consumer reads from *res (the first arm's var slot).
           Since only one arm fires per case execution, the slot aliasing via a shared synthetic
           local name is the cleanest: all arms assign to "__case_result", a single var slot. */
        if (t->n < 1 || !t->c[0]) { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
        static const char * CVAR = "__case_result";
        /* Shared result var: all arms assign into CVAR; cvar IR_VAR reads it back → γ.
           Chain: subject → IDENTICAL → body → ASSIGN(CVAR, body_val) → cvar(IR_VAR) → γ
           cvar sits AFTER all assigns; it copies CVAR's frame slot into its own tmp slot
           (that's what bb_var does: op_sa=varslot → op_off=tmp). The outer consumer
           reads cvar->tmp. All arms converge on the same cvar node, so only one slot needed. */
        IR_t * cvar = build(cx, IR_VAR, γ, ω); IR_LIT(cvar).sval = (char *) CVAR;
        /* lower subject (bounded) */
        IR_t * sr = NULL; IR_t * se = lower(cx, t->c[0], NULL, ω, &sr);
        int nc = t->n - 1;
        if (nc <= 0) { cx->beta = ω; *res = cvar; return se; }
        int npairs   = nc / 2;
        int has_dflt = (nc % 2 == 1);
        IR_t * chain_next = ω;
        if (has_dflt) {
            IR_t * dv = NULL; IR_t * de = lower(cx, t->c[t->n - 1], NULL, ω, &dv);
            IR_t * dasn = build(cx, IR_ASSIGN, cvar, ω); IR_LIT(dasn).sval = (char *) CVAR;
            if (dv) ir_operand_push(dasn, dv);
            γ_to(dv ? dv : de, dasn);
            chain_next = de;
        }
        for (int i = npairs - 1; i >= 0; i--) {
            int ki = 1 + i * 2; int bi = ki + 1;
            IR_t * kn = NULL; IR_t * ke = lower(cx, t->c[ki], NULL, ω, &kn);
            IR_t * bv = NULL; IR_t * be = lower(cx, t->c[bi], NULL, ω, &bv);
            /* ASSIGN(CVAR, bv) → cvar (not γ directly; cvar does the final copy) */
            IR_t * asn = build(cx, IR_ASSIGN, cvar, ω); IR_LIT(asn).sval = (char *) CVAR;
            if (bv) ir_operand_push(asn, bv);
            γ_to(bv ? bv : be, asn);
            IR_t * idc = build(cx, IR_CALL_BUILTIN, be, chain_next);
            IR_LIT(idc).sval = (char *) "IDENTICAL";
            ir_operand_push(idc, sr);
            ir_operand_push(idc, kn);
            γ_to(kn, idc);
            chain_next = ke ? ke : idc;
        }
        γ_to(sr, chain_next);
        cx->beta = ω; *res = cvar; return se; }
    case TT_CONJ:
    case TT_SEQ_EXPR: {
        lc_vec Sv; lc_vec_init(&Sv, (int) sizeof(const tree_t *));
        for (int i = 0; i < t->n; i++) { const tree_t * s = t->c[i]; if (s && s->t == TT_STMT) s = stmt_subj(s); if (s) lc_vec_push(&Sv, &s); }
        const tree_t ** S = (const tree_t **) Sv.data; int k = Sv.n;
        if (k == 0) { IR_t * su = build(cx, IR_SUCCEED, γ, ω); *res = su; return su; }
        if (k == 1) return lower(cx, S[0], γ, ω, res);
        IR_t * SEQX = build(cx, IR_CONJUNCTION, γ, ω);
        IR_t ** val = (IR_t **) calloc((size_t) k, sizeof(IR_t *)); IR_t ** ent = (IR_t **) calloc((size_t) k, sizeof(IR_t *)); IR_t * succ = SEQX;
        if (t->t == TT_SEQ_EXPR) {
            IR_t * failt = ω; IR_t * last_beta = ω; IR_t * rb = NULL;
            /* A statement's success into the NEXT statement is a PRODUCE edge — α role, even when the next entry is
               generator-kind (build()'s auto-stamp marks it β, which enters the resume pump with a cold frame; the
               2026-07-01 REPALT symptom: every |1 printed 0 because assign.γ → REPALT.β skipped the fresh α clear).
               Same rule as LIMIT's er→lim restamp: role lives on the EDGE, not the target's kind.  Guarded on the
               edge actually targeting succ — constructs like every re-aim their γ internally and must not be rewired. */
            for (int i = k - 1; i >= 0; i--) { val[i] = NULL; ent[i] = lower(cx, S[i], succ, failt, &val[i]); if (i == k - 1) last_beta = cx->beta; if (!rb && is_resumable(S[i])) rb = cx->beta; if (val[i] && val[i]->γ.node == succ) lc_γ_to(val[i], succ); succ = ent[i]; failt = ent[i]; }
            if (val[k - 1]) ir_operand_push(SEQX, val[k - 1]);
            cx->conj_resumable = rb; cx->beta = last_beta; *res = SEQX; return ent[0];
        }
        IR_t * last_beta = ω; IR_t * rb = NULL;
        IR_t ** bet = (IR_t **) calloc((size_t) k, sizeof(IR_t *));
        IR_t ** jn = (IR_t **) calloc((size_t) k, sizeof(IR_t *));
        /* CONJ FAIL-CHAIN VIA JUNCTIONS (mindfa cross-product fix): the old post-hoc ω_to(val[i], val[lr]) only rewired the conjunct's RESULT node's ω — a bare generator conjunct works (val IS
           the generator) but a wrapping conjunct like `q := !Q` leaves the inner ITERATE's exhaust edge aimed at outer ω, so `every q := !Q & a := !S` never re-pumped the left. Fix: each conjunct
           i>0 is LOWERED WITH a pre-minted IR_GOTO junction as its ω, so EVERY fail edge inside it lands there naturally; the junction is then patched to the nearest resumable LEFT conjunct's
           actual RESUME node (bet[lr] = cx->beta sampled per conjunct — the generator itself, auto-β via γ_to; the ASSIGN's α would re-enter cold). Conjunct 0's failure keeps outer ω (fails the conj). */
        for (int i = k - 1; i >= 0; i--) { val[i] = NULL; cx->beta = ω;
            IR_t * failt = ω; if (i > 0) { jn[i] = build(cx, IR_GOTO, ω, ω); failt = jn[i]; }
            ent[i] = lower(cx, S[i], succ, failt, &val[i]); bet[i] = cx->beta; if (i == k - 1) last_beta = cx->beta; if (!rb && is_resumable(S[i])) rb = cx->beta; if (val[i] && val[i]->γ.node == succ) lc_γ_to(val[i], succ); succ = ent[i]; }
        int lr = -1; for (int i = 0; i < k; i++) { if (i > 0 && jn[i]) { IR_t * tgt = ω; if (lr >= 0) tgt = (bet[lr] && bet[lr] != ω) ? bet[lr] : val[lr]; γ_to(jn[i], tgt); ω_to(jn[i], tgt); } if (is_resumable(S[i])) lr = i; }
        if (val[k - 1]) ir_operand_push(SEQX, val[k - 1]);
        cx->conj_resumable = rb; cx->beta = last_beta; *res = SEQX; return ent[0];
    }
    case TT_SECTION: case TT_SECTION_PLUS: case TT_SECTION_MINUS: {
        if (t->n < 3 || !t->c[0] || !t->c[1] || !t->c[2]) { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
        /* JCON ir_a_Sectionop: val→left→right chain; right.success calls 3-arg opfn → p.ir.success.
           Canonical icont DESUGARS +:/-: at translation time (tcode.c:591-600: traverse i; dup; traverse n;
           plus/minus; sect) — i is evaluated ONCE, i2 := i1 ± n via an ordinary add/sub, then the SAME sect
           op as s[i:j]. SCRIP mirrors that: variant 1/2 inserts a synthetic IR_BINOP(ADD/SUB) whose left
           operand is i1's own producer node (the dup — value read twice, evaluated once) and whose right is
           n; IR_SUBSCRIPT is then always emitted PLAIN (ival=0), so bb_section's one native arm serves all
           three source forms. val.failure→p.failure; left.failure→val.resume; right.failure→left.resume. */
        int sec_variant = (t->t == TT_SECTION_PLUS) ? 1 : (t->t == TT_SECTION_MINUS) ? 2 : 0;
        IR_t * sec = build(cx, IR_SUBSCRIPT, γ, ω);
        IR_LIT(sec).ival = 0;
        /* lower all three operands; wire them in serial: val→left→right[→binop]→sec */
        IR_t * ar = NULL; IR_t * ae = lower(cx, t->c[0], NULL, ω, &ar);
        IR_t * br = NULL; IR_t * be = lower(cx, t->c[1], NULL, ω, &br); γ_to(ar, be);
        IR_t * cr = NULL; IR_t * ce = lower(cx, t->c[2], sec_variant ? NULL : sec, ω, &cr); γ_to(br, ce);
        if (sec_variant) {
            IR_t * op = build(cx, IR_BINOP, sec, ω); IR_LIT(op).ival = (sec_variant == 1) ? BINOP_ADD : BINOP_SUB;
            ir_operand_push(op, br); ir_operand_push(op, cr);
            γ_to(cr, op); cr = op;
        }
        ir_operand_push(sec, ar); /* base string */
        ir_operand_push(sec, br); /* i1 */
        ir_operand_push(sec, cr); /* i2 (plain: n; +:/-:: the synthetic i1±n binop) */
        cx->beta = ω; *res = sec; return ae; }
    case TT_NOT: return lower_not(cx, t, γ, ω, res);
    case TT_ALTERNATE: return lower_alt(cx, t, γ, ω, res);
    case TT_ITERATE: {
        /* JCON: ! is ir_a_Unop with op="!" → ir_opfn(IR_ITERATE, 1, "rval") for lists/strings/tables.
           SCRIP: IR_ITERATE operand[0]=object-node; needs own slot for index counter (op_sb=off+16).
           bb_iterate: α inits counter to 0; loop calls rt_list_bang_at(obj,idx); β incs idx.
           IR_ITERATE is a generator kind (resumable).
           CORRECTED (Claude Sonnet 4.6, 2026-06-30): γ_to(orr,nd) auto-stamps β (because IR_ITERATE
           is generator_kind) → first entry landed at β (resume/inc), skipping α's counter=0 init,
           so iteration always started at element 1 not 0. Fix: lc_γ_to (unconditional α-stamp). */
        IR_t * nd = build(cx, IR_ITERATE, γ, ω);
        IR_t * orr = NULL; IR_t * ee = lower(cx, (t->n > 0) ? t->c[0] : NULL, NULL, ω, &orr);
        ir_operand_push(nd, orr);
        lc_γ_to(orr, nd);   /* forward operand-feed MUST be α-stamped, same fix as lower_to */
        cx->beta = nd; *res = nd; return ee; }
    case TT_IF: return lower_if(cx, t, γ, ω, res);
    case TT_WHILE: return lower_while(cx, t, γ, ω, res);
    case TT_UNTIL: return lower_until(cx, t, γ, ω, res);
    case TT_REPEAT: return lower_repeat(cx, t, γ, ω, res);
    case TT_TO: case TT_TO_BY: return lower_to(cx, t, γ, ω, res);
    case TT_EVERY: return lower_every(cx, t, γ, ω, res);
    case TT_SCAN: {
        /* JCON ir_a_Scan: expr→ScanSwap(save)→:?(op)→body.start; body.success→ScanSwap(restore)→p.success;
           body.failure→ScanSwap(restore)→expr.resume; expr.failure→p.failure.
           SCRIP: Two nodes per scan:
             IR_SCAN_ENTER (op_sb=1): loads the subject DESCR into r13/r14/r15 via rt_scan_enter,
               saves old r13/r14/r15 into frame slots at enter->tmp..enter->tmp+23.
               operand[0]=subject-string-node.
             IR_SCAN (leave, op_sb=0): restores r13/r14/r15 from frame slots.
               operand[0]=enter node (so emit_drive reads enter->tmp for save-area offset at slot-assign time).
           SLOT LINKAGE: IR_SCAN_ENTER gets nd->tmp from ir_drive_slot_assign (k+=2, 32 bytes, covers 24-byte need).
           IR_SCAN leave emit_drive: op_off = operands[0]->tmp (the enter node's own slot).
           This replaces the old IR_LIT(nd).ival approach, which had to write the offset at lower time
           before slots were assigned -- an ordering impossibility. */
        if (t->n < 2 || !t->c[0] || !t->c[1]) {
            /* No subject or no body: degenerate */
            IR_t * gs = build(cx, IR_FAIL, γ, ω); *res = gs; return gs;
        }
        /* IR_SCAN_ENTER: subject evaluates, then enters scan context */
        IR_t * enter = build(cx, IR_SCAN_ENTER, NULL, ω);
        /* IR_SCAN leave-nodes: one for success path, one for failure path (both restore regs).
           operand[0] on each leave node = the enter node, so emit_drive can read enter->tmp. */
        IR_t * leave_succ = build(cx, IR_SCAN, γ, ω);
        IR_t * leave_fail = build(cx, IR_SCAN, ω, ω);
        ir_operand_push(leave_succ, enter);   /* leave_succ.operand[0] = enter (for slot offset) */
        ir_operand_push(leave_fail, enter);   /* leave_fail.operand[0] = enter (for slot offset) */
        /* Lower the body; body.success → leave_succ; body.failure → leave_fail */
        IR_t * bv = NULL; IR_t * b_entry = lower(cx, t->c[1], leave_succ, leave_fail, &bv);
        /* Retag IR_CALL nodes for scan builtins (tab/move/upto/etc) inside the body */
        icn_retag_scan_body(cx->g, 0);
        /* Stitch: enter.γ → b_entry */
        lc_γ_to(enter, b_entry);
        /* Lower subject → enter; subject.failure → p.failure */
        IR_t * sr = NULL; IR_t * s_entry = lower(cx, t->c[0], enter, ω, &sr);
        ir_operand_push(enter, sr);  /* operand[0] = subject DESCR node (AFTER leave nodes push enter) */
        cx->beta = ω; *res = enter; return s_entry; }
    case TT_STMT: { const tree_t * sub = stmt_subj(t); if (sub) return lower(cx, sub, γ, ω, res); IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    case TT_CREATE: {
        /* JCON ir_a_Create (irgen.icn:1035-1058), read against refs/jcon-master directly, 2026-07-01:
             p.ir.start:  ir_Create(target, p.expr.ir.start); Goto p.ir.success
                          -- `create EXPR` itself SUCCEEDS IMMEDIATELY, returning a co-expression VALUE.
                             p.expr is NOT entered here; only its entry LABEL is captured.
             p.ir.resume (bounded): Goto p.ir.failure -- asking `create`'s own result for a 2nd value fails.
             p.expr.ir.success: ir_CoRet(t, p.expr.ir.resume) -- body yields a value + remembers where
                                 to continue if the coexpression is resumed again (@t elsewhere).
             p.expr.ir.failure: ir_CoFail() -- body exhausted; coexpression is now permanently dead.
           SCRIP: IR_CREATE/IR_CORET/IR_COFAIL already reserved in IR.h (unused until this commit).
           Two-node shape, same as TT_SCAN's enter/leave: IR_CORET and IR_COFAIL do NOT sit on the outer
           γ/ω at all (create's own success is unconditional and immediate, independent of the body ever
           running) -- they are the body's OWN success/failure targets, exactly as TT_SCAN's leave_succ/
           leave_fail are the scan-body's success/failure targets, not the enter node's.
           operand[0] on IR_CREATE = the body's entry node (so the runtime knows where a coswitch() into
           this coexpression should land on its first resume -- see bb_create.cpp/runtime design, not yet
           written as of this commit; this LOWER change alone does not make `create` executable).
           CORRECTION found this session (verified against rswitch.c in refs/icon-master/src/common/,
           not assumed): the reference C runtime's actual coswitch() is pthread+semaphore based (one OS
           thread per live coexpression, semaphore pair per switch), NOT ucontext.h and not hand-rolled
           assembly stack-switching as GOAL-IR-IMMUTABLE-EMIT.md's prior punch-list entry guessed -- see
           that file's own correction, same date. LOWER here is written against the IR contract only and
           is agnostic to which switching strategy the runtime ultimately uses underneath it. */
        IR_t * nd = build(cx, IR_CREATE, γ, ω);
        IR_t * coret = build(cx, IR_CORET, NULL, NULL);   /* body.success target; own γ/ω unused by driver */
        IR_t * cofail = build(cx, IR_COFAIL, NULL, NULL); /* body.failure target; own γ/ω unused by driver */
        IR_t * bv = NULL; IR_t * b_entry = lower(cx, (t->n > 0) ? t->c[0] : NULL, coret, cofail, &bv);
        /* JCON ir_CoRet(t, p.expr.ir.resume): the coret's continuation is the BODY'S OWN RESUME POINT --
           on the next `@`, scrip_coret's sem_wait returns and bb_coret's post-call jmp must re-enter the
           body generator at its β (yielding the NEXT value), or, for a non-generator body, the body's
           failure path (-> cofail -> dead: a second activation of a 1-value body fails). cx->beta right
           after the body lower IS that resume point (the γ_to wrapper auto-stamps "β" when the target is
           generator-kind, so the emitter resolves this edge to betas[k], the generator's resume label).
           Without this edge, bb_coret's jmp γ fell to the CHAIN DEFAULT γ-exit -- the resumed body thread
           ran off into the main chain's code (RUNG 5 bring-up segv on the SECOND `@`, gdb-bracketed
           2026-07-01; RUNG 4's own comment had flagged the placeholder as "refined by RUNG 5"). */
        IR_t * body_beta = cx->beta;
        if (body_beta) γ_to(coret, body_beta);
        ir_operand_push(coret, bv);      /* coret.operand[0] = the value the body just produced */
        ir_operand_push(nd, b_entry);    /* create.operand[0] = body's entry node (coswitch target) */
        /* bounded resume (asking the CREATE EXPRESSION's own result for a 2nd value) always fails --
           unconditional ω, mirroring JCON's `/bounded & Goto p.ir.failure`. */
        cx->beta = ω; *res = nd; return nd; }
    case TT_ACTIVATE: {
        /* JCON: unary `@x` desugars to `&null @ x` (irgen.icn:516) and BOTH forms route through the GENERIC
           binary-operator machinery (ir_binary, irgen.icn:430-445) — evaluation order left(xmit) then
           right(coexpr), each sub-expression's failure → p.ir.failure, then ONE op node; the only
           @-specific arm in that shared JCON path is argument order `args := [rv, lv]` (coexpr FIRST,
           irgen.icn:433-434), mirrored here as operand[0]=coexpr, operand[1]=xmit. Activation is BOUNDED
           (a resumed `@`-expression fails: one result per activation — reference semantics, and JCON's
           vDescriptor "Activate" is a 1-shot 2-arg operator per interfacegen.icn:105), so cx->beta = ω
           exactly as TT_CREATE. The parser's unary form arrives with n==1 (coexpr only): the xmit operand
           is simply ABSENT; emit_drive passes op_sb=-1 and bb_activate transmits &null {DT_SNUL,0} inline —
           behaviorally identical to JCON's synthesized a_Key("null") without fabricating an AST node.
           IR_ACTIVATE is in ir_node_produces_value (its tmp receives the DESCR the body coret'd back);
           it is NOT a generator kind — resuming re-activation is `|@c`, ordinary REPALT around this node. */
        IR_t * nd = build(cx, IR_ACTIVATE, γ, ω);
        const tree_t * xt = (t->n > 1) ? t->c[0] : NULL;
        const tree_t * ct = (t->n > 1) ? t->c[1] : t->c[0];
        IR_t * cr = NULL; IR_t * c_entry = lower(cx, ct, nd, ω, &cr);
        IR_t * entry = c_entry;
        ir_operand_push(nd, cr);
        if (xt) { IR_t * xr = NULL; entry = lower(cx, xt, c_entry, ω, &xr); ir_operand_push(nd, xr); }
        cx->beta = ω; *res = nd; return entry; }
    case TT_REPALT: {
        /* JCON ir_a_RepAlt (unbounded): start→MoveLabel(t,ω)→e.start; e.success→MoveLabel(t,start)→p.success;
           e.failure→IndirectGoto(t); p.resume→e.resume.
           SCRIP IR_REPALT: operand[0]=e (the sub-expression). The flat chain driver (flat_drive_repalt in
           emit.cpp) handles the 4-edge wiring internally using bb_repalt_{clear/yield/test}.
           IR_REPALT is a generator kind; the BFS stamps the consumer's backtrack edge as β → REPALT β.
           The β (= consumer asking for next value) pumps e-β directly (no MoveLabel needed at runtime,
           since bb_repalt's test+restart logic encodes JCON's IndirectGoto). */
        IR_t * nd = build(cx, IR_REPALT, γ, ω);
        lc_γ_to(nd, γ);   /* REPALT.γ (the yield's target) is a PRODUCE edge — α even into a generator-kind consumer (LIMIT); build()'s auto-β sent the yield into LIMIT.β's pump → infinite spin on |(1 to 0)\3 */
        IR_t * er = NULL; IR_t * ee = lower(cx, (t->n > 0) ? t->c[0] : NULL, NULL, ω, &er);
        ir_operand_push(nd, er);   /* operand[0] = e root node */
        ir_operand_push(nd, ee);   /* operand[1] = e entry point (may differ from root) */
        cx->beta = nd; *res = nd; return nd; }
    case TT_LIMIT: {
        /* JCON ir_a_Limitation: count gen, counter slot; generator is on-spine; limit node sits
           AFTER the generator in the flat chain (generator.γ → LIMIT.α).
           SCRIP: IR_LIMIT operand[0]=generator-node, operand[1]=count expr — an on-spine value-producer
           whose slot bb_limit reads at RUNTIME (op_sc); literal and variable counts ride the same path
           (clobber audit 2026-07-02). The counter slot lives at op_off+16.
           bb_limit: on each generator success (LIMIT.α): if counter>=limit→ω; else inc+copy+γ.
           β: jmp generator-β (pump again). LIMIT.α is where the chain BFS puts this node. */
        IR_t * lim = build(cx, IR_LIMIT, γ, ω);
        /* lower the count expression (operand[1]) — must resolve to a static integer literal */
        IR_t * lr = NULL; IR_t * ee = lower(cx, (t->n > 1) ? t->c[1] : NULL, lim, ω, &lr);
        IR_t * inner_beta = cx->beta;
        /* lower the generator expression (operand[0]) — its γ flows into lim */
        IR_t * er = NULL; IR_t * ge = lower(cx, (t->n > 0) ? t->c[0] : NULL, lim, ω, &er);
        IR_t * gen_beta = cx->beta;
        ir_operand_push(lim, er);  /* operand[0] = generator root */
        ir_operand_push(lim, lr);  /* operand[1] = count literal */
        /* JCON ir_a_Limitation: limit.success -> expr.ir.start (irgen.icn:137 `Goto p.expr.ir.start`).
           The count's success must enter the GENERATOR, not the gate; gate (LIMIT.a) fires only on
           generator successes (expr.success -> p.ir.success rides through the gate).  Wire-later,
           same idiom as TT_LCONCAT below. */
        γ_to(lr, ge);
        if (lr && lr->γ.node == ge) lc_γ_to(lr, ge);   /* count→generator-entry is a fresh PRODUCE edge — α even when ge is itself generator-kind (|e as the limited expr; a TO's entry is a literal so this only bites REPALT) */
        /* The generator's SUCCESS edge into the gate is a PRODUCE edge — it must enter LIMIT.α (the
           check-inc-copy gate), never LIMIT.β (the resume pump). build()/γ_to's kind-based auto-stamp
           marked it β because IR_LIMIT is (correctly) generator-kind for CONSUMER-backtrack routing;
           the role lives on the EDGE, not the target's kind. Re-stamp α here (JCON irgen.icn:133,
           expr.success → p.ir.success, which rides through the gate). 2026-07-01 wholesale audit:
           symptom was TO.γ → LIMIT.β → TO.β silent spin to exhaustion, empty output. */
        if (er) lc_γ_to(er, lim);
        (void)inner_beta; cx->beta = gen_beta;
        *res = lim; return ee; }
    case TT_LCONCAT: {
        /* JCON: ||| is a_Binop("|||") → ir_OpFunction("|||", 2) → pure-value opfn, never fails.
           SCRIP: route through IR_BINOP with BINOP_CONCAT (same as string ||); the runtime
           rt_str_concat already handles list values as a concat by type dispatch in bb_binop_concat_slot.
           Wire: left→right→op, left.failure→p.failure, right.failure→left.resume. */
        if (t->n < 2 || !t->c[0] || !t->c[1]) { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
        IR_t * op = build(cx, IR_BINOP, γ, ω); IR_LIT(op).ival = BINOP_CONCAT;
        IR_t * lr = NULL; IR_t * ee = lower(cx, t->c[0], NULL, ω, &lr); IR_t * lβ = cx->beta;
        IR_t * rr = NULL; IR_t * re = lower(cx, t->c[1], op, lβ, &rr); γ_to(lr, re);
        ir_operand_push(op, lr); ir_operand_push(op, rr);
        *res = op; return ee; }
    case TT_SWAP: {
        /* JCON: := : is a_Binop(":=:") → ir_augmented_assignment path → ir_Assign(lv,tmp)+ir_Assign(rv,lv_orig).
           SCRIP plain×plain: IR_SWAP carries both var references as operands[0]/[1]; template bb_swap swaps frame slots.
           SWAP-LV (either side subscript/section): both sides lower as VARIABLE producers (lower_lvalue_var —
           canonical swap takes both operands UNDEREF, oasgn.r:267) chained x-then-y per canonical eval order,
           into IR_SWAP_VAR (operands[0]=x-var, [1]=y-var; rt_swap_var does the two writes + same-string tvsubs
           pos adjustment and returns the fresh x deref). operator{0,1}: single-shot, no β wiring (the LV-3a shape). */
        const tree_t * lt = (t->n > 0) ? t->c[0] : NULL; const tree_t * rt2 = (t->n > 1) ? t->c[1] : NULL;
        int plain_l = lt && lt->t == TT_VAR && lt->v.sval && lt->v.sval[0] != '&';
        int plain_r = rt2 && rt2->t == TT_VAR && rt2->v.sval && rt2->v.sval[0] != '&';
        if (!(plain_l && plain_r)) {
            IR_t * xr = NULL; IR_t * xe = lower_lvalue_var(cx, lt, ω, &xr);
            IR_t * yr = NULL; IR_t * ye = xe ? lower_lvalue_var(cx, rt2, ω, &yr) : NULL;
            if (xe && ye) {
                lc_γ_to(xr, ye);
                IR_t * nd = build(cx, IR_SWAP_VAR, γ, ω);
                lc_γ_to(yr, nd);
                ir_operand_push(nd, xr); ir_operand_push(nd, yr);
                *res = nd; return xe;
            }
        }
        IR_t * nd = build(cx, IR_SWAP, γ, ω);
        IR_t * lr = NULL; lower(cx, lt, nd, ω, &lr);
        IR_t * rr = NULL; lower(cx, rt2, nd, ω, &rr);
        ir_operand_push(nd, lr); ir_operand_push(nd, rr); *res = nd; return nd; }
    case TT_REVASSIGN: {
        const tree_t * lhs = t->c[0]; const tree_t * rhs = (t->n > 1) ? t->c[1] : NULL;
        /* Subscript lvalue: x[i] <- v — IDX-UNIFY: the lhs chain yields the VARIABLE (lower_idx_var), IR_REV_ASSIGN_VAR deref-saves the old value, writes through, suspends the value, restores on β
           (canonical rasgn, oasgn.r:142-162, with the trapped-variable arms served by rt_deref/rt_assign_var exactly as IR_ASSIGN_VAR). The by-NAME sibling arm below stays IR_REV_ASSIGN — classify-by-name,
           one template each (JCON-alignment directive). operands[0]=variable (walk preamble op_a_slot re-derivation, IR_ASSIGN_VAR parity), [1]=value (driver stages op_sa). lhs-then-rhs evaluation per
           canonical asgn; rhs→nd PRODUCE edge lc_γ_to α-restamped (EXPORTABLE WIRING RULE — auto-β into a generator-kind target is only right for backtrack edges); cx->beta = nd (the resume IS the construct). */
        if (lhs && lhs->t == TT_IDX) {
            IR_t * vr = NULL; IR_t * entry = lower_idx_var(cx, lhs, ω, &vr);
            IR_t * nd = build(cx, IR_REV_ASSIGN_VAR, γ, ω);
            IR_t * rr = NULL; IR_t * re = lower(cx, rhs, NULL, ω, &rr);
            lc_γ_to(vr, re);
            lc_γ_to(rr, nd);
            ir_operand_push(nd, vr);  /* [0] = variable (walk's op_a_slot re-derivation reads this) */
            ir_operand_push(nd, rr);  /* [1] = rhs value (driver stages op_sa) */
            cx->beta = nd; *res = nd; return entry;
        }
        /* Simple-variable lvalue: x <- v — canonical rasgn (oasgn.r:142-162): save old x, GeneralAsgn(x,v),
           suspend x; on resume GeneralAsgn(x, saved_x) then fail. IR_REV_ASSIGN is GENERATOR-KIND (the resume IS
           the construct); α saves+assigns+produces→γ, β restores→ω (bb_rasgn.cpp, pre-existing template).
           OPERAND ORDER IS LOAD-BEARING (clobber-pattern alignment): operands[0] = RHS value producer —
           walk_bb_node's preamble re-derives op_a_slot = bb_slot_get(operands[0]) AFTER emit_drive, so the
           template's rhs-source field is only correct if the rhs IS operand[0] (the historic scrip.c BENCH-F2
           failure was exactly this collision, dest varslot landing in op_a_slot). operands[1] = lhs IR_VAR as
           NAME CARRIER (IR_SWAP idiom; driver reads IR_LIT(operands[1]).sval → bb_varslot).
           Chain le→re→nd with lc_γ_to α-restamp on the rhs→RASGN PRODUCE edge (EXPORTABLE WIRING RULE —
           build/γ_to auto-β into a generator-kind target is only right for backtrack edges). */
        if (lhs && lhs->t == TT_VAR) {
            IR_t * nd = build(cx, IR_REV_ASSIGN, γ, ω);
            IR_t * lr = NULL; IR_t * le = lower(cx, lhs, NULL, ω, &lr);
            IR_t * rr = NULL; IR_t * re = lower(cx, rhs, NULL, ω, &rr);
            γ_to(lr, re);
            lc_γ_to(rr, nd);
            ir_operand_push(nd, rr);  /* [0] = rhs (walk's op_a_slot re-derivation reads this) */
            ir_operand_push(nd, lr);  /* [1] = lhs var node (name carrier) */
            cx->beta = nd; *res = nd; return le;
        }
        IR_t * nd = build(cx, IR_FAIL, γ, ω);
        IR_t * lr = NULL; lower(cx, lhs, nd, ω, &lr);
        IR_t * rr = NULL; lower(cx, rhs, nd, ω, &rr);
        ir_operand_push(nd, lr); ir_operand_push(nd, rr); *res = nd; return nd; }
    /* ir_a_Record: pure declaration — register record type via record_register(spec) at lower time, no IR node.
       JCON: returns ir_Record(coord, name, fields[]) consumed by bc_records() outside the ir_a_* flow.
       SCRIP: build spec string "name(f1,f2,...)" and call record_register(spec) during lowering.
       TT_RECORD AST: e->v.sval = name; children = field TT_VAR nodes (e->c[k]->v.sval = field name). */
    case TT_RECORD: {
        extern void record_register(const char *spec);
        const char * rname = t->v.sval;
        if (rname) {
            char spec[2048]; int pos = 0;
            pos += snprintf(spec + pos, sizeof spec - pos, "%s(", rname);
            for (int k = 0; k < t->n; k++) {
                if (k) pos += snprintf(spec + pos, sizeof spec - pos, ",");
                if (t->c[k] && t->c[k]->v.sval) pos += snprintf(spec + pos, sizeof spec - pos, "%s", t->c[k]->v.sval);
            }
            snprintf(spec + pos, sizeof spec - pos, ")");
            record_register(spec);
        }
        IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    /* ir_a_Invocable: declares procedures invocable by name — pure annotation, no IR node needed. */
    case TT_INVOCABLE:
    /* ir_a_Link: link declaration — handled at parse/compile level, not at IR lowering. */
    case TT_LINK:
    default: { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    }
}
/*====================================================================================================================================================================================================*/
static IR_t * lower_while(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    const tree_t * C = (t->n > 0) ? t->c[0] : NULL; const tree_t * B = (t->n > 1) ? t->c[1] : NULL;
    /* Sentinel W: condition failure exits the loop (C.failure→W, W is the loop-exit jump to γ).
       Must be IR_GOTO (not IR_FAIL) so the BFS enqueues W and follows W's γ edge to post-loop code.
       γ_to(W,γ) makes W a "jmp to loop-exit γ" node per the LOOP-BACK idiom. */
    IR_t * W = build(cx, IR_GOTO, γ, ω); γ_to(W, γ); ω_to(W, γ);
    IR_t * sle = cx->loop_exit; IR_t * sln = cx->loop_next; cx->loop_exit = γ;
    IR_t * cval = NULL; IR_t * centry = lower(cx, C, NULL, W, &cval);
    cx->loop_next = centry;
    IR_t * bval = NULL; IR_t * b_entry = lower(cx, B, centry, centry, &bval);
    lc_γ_to(cval, b_entry);
    cx->loop_exit = sle; cx->loop_next = sln;
    *res = W; return centry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_until(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    const tree_t * C = (t->n > 0) ? t->c[0] : NULL; const tree_t * B = (t->n > 1) ? t->c[1] : NULL;
    /* Sentinel U: condition SUCCESS exits the loop (C.success→U, U is the loop-exit jump to γ).
       Must be IR_GOTO so BFS follows U's γ edge to post-loop code. */
    IR_t * U = build(cx, IR_GOTO, γ, ω); γ_to(U, γ); ω_to(U, γ);
    IR_t * sle = cx->loop_exit; IR_t * sln = cx->loop_next; cx->loop_exit = γ;
    IR_t * cval = NULL; IR_t * centry = lower(cx, C, U, NULL, &cval);
    cx->loop_next = centry;
    IR_t * bval = NULL; IR_t * b_entry = lower(cx, B, centry, centry, &bval);
    lc_ω_to(cval, b_entry);
    cx->loop_exit = sle; cx->loop_next = sln;
    *res = U; return centry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_repeat(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    const tree_t * B = (t->n > 0) ? t->c[0] : NULL;
    IR_t * H = build(cx, IR_GOTO, NULL, ω);
    IR_t * sle = cx->loop_exit; IR_t * sln = cx->loop_next; cx->loop_exit = γ; cx->loop_next = H;
    IR_t * bval = NULL; IR_t * b_entry = lower(cx, B, H, H, &bval);
    lc_γ_to(H, b_entry); lc_ω_to(H, b_entry);
    cx->loop_exit = sle; cx->loop_next = sln;
    cx->beta = γ; *res = H; return H;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_not(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    /* JCON ir_a_Not: no operator node -- pure ir_Goto success/failure port-swap around the child, plus a
       generic ir_Key(target,"null",&null) on the (swapped) success arm. Mirrored exactly: no IR_NOT opcode;
       "&null" is IR_VAR with sval="&null" -- the SAME live keyword-read path used elsewhere (e.g. the
       parser's elided-call-arg fill-in), not a NOT-specific construct. child.success(γ)->my.ω (fail);
       child.failure(ω)->the &null-producing node, whose own γ is my γ (succeed with &null). */
    IR_t * nullv = build(cx, IR_VAR, γ, ω); IR_LIT(nullv).sval = (char *) "&null";
    IR_t * cr = NULL; IR_t * ce = lower(cx, (t->n > 0) ? t->c[0] : NULL, ω, nullv, &cr);
    cx->beta = ω; *res = nullv; return ce;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_alt(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    int n = t->n; if (n < 1) { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    /* JCON ir_a_Alt, /bounded (unbounded) arm, decoded from irgen.icn:166-199 per the CENTRAL ISOMORPHISM:
       one label variable t per alternation (ir_tmploc → the IR_INDIRECT_GOTO node's own tmp cell, +16);
       p.ir.resume = ir_IndirectGoto(t) → the ig node below, which lower_alt hands out as cx->beta so EVERY
       consumer backtrack edge lands on it (jmp *t → whichever arm last fired's resume — the data-dependent
       target the punch list proved no static edge can express);
       each arm's success = ir_MoveLabel(t, eList[i].resume); Goto p.ir.success → the per-arm ml node below
       (which ALSO copies the arm's value into ig's shared cell — JCON gets convergence from its per-arm
       shared `target` param; SCRIP's lhs⇄tmp equivalent is one node/one slot, *res = ig);
       eList[i].resume decoded: post-arm cx->beta when the arm holds a real resumable (β edge, ival=1) —
       resume produces the arm's next value; otherwise the arm's ωj (α edge, ival=0) — JCON's "a
       non-generator's resume IS its failure": spent arm re-enters the next arm fresh (or the alt's ω).
       cx->beta is force-reset to ωj before each arm lower (literal lowers do not touch it — stale-beta leak
       otherwise); arm cascade (eList[i].failure → eList[i+1].start, last → p.ir.failure) unchanged. */
    IR_t * ig = build(cx, IR_INDIRECT_GOTO, γ, ω);
    IR_t ** entry = (IR_t **) calloc((size_t) n, sizeof(IR_t *));
    for (int j = n - 1; j >= 0; j--) {
        IR_t * ωj = (j + 1 < n) ? entry[j + 1] : ω;
        IR_t * ml = build(cx, IR_MOVE_LABEL, γ, ω);
        cx->beta = ωj;
        IR_t * ar = NULL; entry[j] = lower(cx, t->c[j], ml, ωj, &ar);
        IR_t * ab = cx->beta ? cx->beta : ωj;
        IR_LIT(ml).ival = (ab && ir_is_generator_kind(ab->op)) ? 1 : 0;
        ir_operand_push(ml, ab); ir_operand_push(ml, ig); ir_operand_push(ml, ar);
    }
    cx->beta = ig; *res = ig; return entry[0];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_if(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    const tree_t * C = (t->n > 0) ? t->c[0] : NULL; const tree_t * TH = (t->n > 1) ? t->c[1] : NULL; const tree_t * EL = (t->n > 2) ? t->c[2] : NULL;
    /* JCON ir_a_If, /bounded arm (irgen.icn:583-610) — the SAME label-variable + shared-target mechanism as lower_alt above, two arms selected by the condition instead of a failure cascade:
       ig owns the shared 32-byte cell (*res = ig — one slot serves both branches, the value-convergence fix; the old `*res = then_val` read only then's slot, else's value was never seen);
       per branch an ml copies that branch's value into ig's cell and stores the branch's resume into t (operand[0] = post-branch cx->beta when a real generator lives in the branch — β edge,
       ival=1 — else the if's ω: a spent branch resumes to the WHOLE if's failure, JCON `thenexpr.ir.resume`≡its failure≡p.ir.failure, the condition is bounded and never re-driven);
       cx->beta force-reset to ω before each branch lower (stale-leak guard, the lower_alt idiom); absent else = JCON's a_Key("fail") default = else_entry = ω, no ml. */
    IR_t * ig = build(cx, IR_INDIRECT_GOTO, γ, ω);
    IR_t * ml_th = build(cx, IR_MOVE_LABEL, γ, ω);
    cx->beta = ω;
    IR_t * then_val = NULL; IR_t * then_entry = lower(cx, TH, ml_th, ω, &then_val);
    IR_t * ab_th = cx->beta ? cx->beta : ω;
    IR_LIT(ml_th).ival = (ab_th && ir_is_generator_kind(ab_th->op)) ? 1 : 0;
    ir_operand_push(ml_th, ab_th); ir_operand_push(ml_th, ig); ir_operand_push(ml_th, then_val);
    IR_t * else_entry;
    if (EL) {
        IR_t * ml_el = build(cx, IR_MOVE_LABEL, γ, ω);
        cx->beta = ω;
        IR_t * else_val = NULL; else_entry = lower(cx, EL, ml_el, ω, &else_val);
        IR_t * ab_el = cx->beta ? cx->beta : ω;
        IR_LIT(ml_el).ival = (ab_el && ir_is_generator_kind(ab_el->op)) ? 1 : 0;
        ir_operand_push(ml_el, ab_el); ir_operand_push(ml_el, ig); ir_operand_push(ml_el, else_val);
    } else { else_entry = ω; }
    IR_t * cond_val = NULL; IR_t * cond_entry = lower(cx, C, then_entry, else_entry, &cond_val); (void) cond_val;
    cx->beta = ig; *res = ig; return cond_entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_const_step(const tree_t * s, int64_t * bits, int * isr) {
    if (!s) return 0;
    if (s->t == TT_ILIT) { *bits = s->v.ival; *isr = 0; return 1; }
    if (s->t == TT_FLIT) { double d = s->v.dval; memcpy(bits, &d, 8); *isr = 1; return 1; }
    if ((s->t == TT_MNS || s->t == TT_PLS) && s->n >= 1 && s->c[0]) {
        if (!icn_const_step(s->c[0], bits, isr)) return 0;
        if (s->t == TT_MNS) { if (*isr) { double d; memcpy(&d, bits, 8); d = -d; memcpy(bits, &d, 8); } else *bits = -(*bits); }
        return 1;
    }
    if (s->t == TT_POW && s->n >= 2 && s->c[0] && s->c[1]) {
        int64_t lb = 0, rb = 0; int li = 0, ri = 0;
        if (!icn_const_step(s->c[0], &lb, &li) || !icn_const_step(s->c[1], &rb, &ri)) return 0;
        if (!li && !ri) {
            if (rb >= 0) { int64_t acc = 1; for (int64_t k = 0; k < rb; k++) acc *= lb; *bits = acc; *isr = 0; return 1; }
            if (lb == 1)  { *bits = 1; *isr = 0; return 1; }
            if (lb == -1) { *bits = (rb & 1) ? -1 : 1; *isr = 0; return 1; }
            if (lb == 0)  return 0;
            *bits = 0; *isr = 0; return 1;
        }
        double la, ra, rv; if (li) memcpy(&la, &lb, 8); else la = (double) lb; if (ri) memcpy(&ra, &rb, 8); else ra = (double) rb;
        rv = pow(la, ra); memcpy(bits, &rv, 8); *isr = 1; return 1;
    }
    if ((s->t == TT_ADD || s->t == TT_SUB || s->t == TT_MUL || s->t == TT_DIV || s->t == TT_MOD) && s->n >= 2 && s->c[0] && s->c[1]) {
        int64_t lb = 0, rb = 0; int li = 0, ri = 0;
        if (!icn_const_step(s->c[0], &lb, &li) || !icn_const_step(s->c[1], &rb, &ri)) return 0;
        if (!li && !ri) {
            int64_t r; if (s->t == TT_ADD) r = lb + rb; else if (s->t == TT_SUB) r = lb - rb; else if (s->t == TT_MUL) r = lb * rb; else if (s->t == TT_DIV) { if (rb == 0) return 0; r = lb / rb; } else { if (rb == 0) return 0; r = lb % rb; }
            *bits = r; *isr = 0; return 1;
        }
        double la, ra, rv; if (li) memcpy(&la, &lb, 8); else la = (double) lb; if (ri) memcpy(&ra, &rb, 8); else ra = (double) rb;
        if (s->t == TT_ADD) rv = la + ra; else if (s->t == TT_SUB) rv = la - ra; else if (s->t == TT_MUL) rv = la * ra; else if (s->t == TT_DIV) { if (ra == 0.0) return 0; rv = la / ra; } else { if (ra == 0.0) return 0; rv = fmod(la, ra); }
        memcpy(bits, &rv, 8); *isr = 1; return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_seq(icx_t * cx, const tree_t * t, int argbase, int nargs, IR_t * γ, IR_t * ω, IR_t ** res) {
    int64_t by = 1; const tree_t * byt = (nargs > 1) ? t->c[argbase + 1] : NULL;
    if (byt) { int64_t bb = 1; int isr = 0; if (!icn_const_step(byt, &bb, &isr) || isr) return NULL; by = bb; }
    if (by == 0) return NULL;
    IR_t * to = build(cx, IR_FAIL, γ, ω); IR_LIT(to).sval = (char *) "ag"; IR_LIT(to).ival = by; cx->last_gen = to;
    const tree_t * fromt = (nargs > 0) ? t->c[argbase] : NULL; IR_t * lr; IR_t * ea; IR_t * lβ;
    if (fromt) { ea = lower(cx, fromt, NULL, ω, &lr); lβ = cx->beta; } else { lr = build(cx, IR_LIT_INTEGER, NULL, ω); IR_LIT(lr).ival = 1; ea = lr; lβ = ω; }
    IR_t * mr = build(cx, IR_LIT_INTEGER, to, lβ); IR_LIT(mr).ival = (by > 0) ? INT64_MAX : INT64_MIN; γ_to(lr, mr);
    ir_operand_push(to, lr); ir_operand_push(to, mr); cx->beta = to; *res = to; return ea;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_key(icx_t * cx, const tree_t * t, int argbase, int nargs, IR_t * γ, IR_t * ω, IR_t ** res) {
    (void) nargs;
    IR_t * kg = build(cx, IR_FAIL, γ, ω);
    IR_t * orr = NULL; (void) lower(cx, t->c[argbase], NULL, ω, &orr); ir_operand_push(kg, orr);
    cx->beta = kg; *res = kg; return kg;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_make_list(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * ml = build(cx, IR_MAKE_LIST, γ, ω);
    if (res) *res = ml;
    IR_t * prev = NULL; IR_t * entry = ml; IR_t * aω = ω;
    for (int k = 0; k < t->n; k++) {
        const tree_t * a = t->c[k]; IR_t * ar = NULL;
        IR_t * ae = lower(cx, a, (k == t->n - 1) ? ml : NULL, aω, &ar); aω = cx->beta;
        if (k == 0) entry = ae;
        if (prev) γ_to(prev, ae);
        prev = ar;
        if (ar) ir_operand_push(ml, ar);
    }
    cx->beta = g_postfix_resume ? aω : ω;
    return entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_to(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    int by = (t->t == TT_TO_BY && t->n > 2 && t->c[2]) ? 1 : 0;
    IR_t * to = build(cx, by ? IR_TO_BY : IR_TO, γ, ω); IR_LIT(to).sval = (char *) "ag"; cx->last_gen = to;
    IR_t * lr = NULL; IR_t * ea = lower(cx, t->c[0], NULL, ω, &lr); IR_t * lβ = cx->beta;
    IR_t * mr = NULL; IR_t * em = lower(cx, t->c[1], by ? NULL : to, lβ, &mr); γ_to(lr, em);
    ir_operand_push(to, lr); ir_operand_push(to, mr);
    if (by) {
        IR_t * mβ = cx->beta; IR_t * br = NULL; IR_t * eb = lower(cx, t->c[2], to, mβ, &br); γ_to(mr, eb); (void) eb;
        if (br && ir_is_generator_kind(to->op)) lc_γ_to(br, to);
        ir_operand_push(to, br);
    } else if (mr && ir_is_generator_kind(to->op)) lc_γ_to(mr, to);
    cx->beta = to; *res = to; return ea;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_every(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    (void) γ;
    const tree_t * E = (t->n > 0) ? t->c[0] : NULL; const tree_t * B = (t->n > 1) ? t->c[1] : NULL;
    IR_t * eval = NULL; IR_t * e_entry = lower(cx, E, NULL, ω, &eval); IR_t * gen_beta = cx->beta;
    IR_t * sle = cx->loop_exit; IR_t * sln = cx->loop_next; cx->loop_exit = ω; cx->loop_next = gen_beta;
    IR_t * bval = NULL; (void) bval; IR_t * b_entry;
    if (B) { b_entry = lower(cx, B, gen_beta, gen_beta, &bval); }
    else { b_entry = build(cx, IR_GOTO, gen_beta, gen_beta); }
    cx->loop_exit = sle; cx->loop_next = sln;
    γ_to(eval, b_entry);
    cx->beta = ω; *res = eval ? eval : e_entry; return e_entry;
}
/*====================================================================================================================================================================================================*/
static IR_graph_t * lower_proc_body(icx_t * cx, const tree_t * body) {
    IR_graph_t * g = IR_alloc(8192, IR_LANG_ICN); cx->g = g;
    IR_t * PSUCC = IR_node_alloc(g, IR_SUCCEED); IR_t * PFAIL = IR_node_alloc(g, IR_FAIL);
    cx->psucc = PSUCC; cx->pfail = PFAIL;
    IR_t * succ = icn_subtree_has_suspend(body) ? PFAIL : PSUCC; IR_t * fail = PFAIL;
    for (int i = body->n - 1; i >= 0; i--) {
        const tree_t * s = body->c[i]; if (s && s->t == TT_STMT) { const tree_t * sub = stmt_subj(s); if (!sub) continue; s = sub; } if (!s) continue;
        /* statement-spine PRODUCE edge is α-role even into a generator-kind entry — same guarded restamp as TT_CONJ/TT_SEQ_EXPR (see the comment there; REPALT every|1 symptom, 2026-07-01) */
        IR_t * r = NULL; IR_t * entry = lower(cx, s, succ, fail, &r); if (r && r->γ.node == succ) lc_γ_to(r, succ); succ = entry; fail = entry;
    }
    g->entry = succ; return g;
}
/*====================================================================================================================================================================================================*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void collect_procs_vec(const tree_t * t, lc_vec * out) {
    if (!t) return;
    if (t->t == TT_STMT) { collect_procs_vec(stmt_subj(t), out); return; }
    if (t->t == TT_PROC_DECL) { lc_vec_push(out, &t); return; }
    for (int i = 0; i < t->n; i++) collect_procs_vec(t->c[i], out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void fill_pnames(const tree_t * prog, lc_vec * pn) {
    lc_vec ps; lc_vec_init(&ps, (int) sizeof(const tree_t *));
    collect_procs_vec(prog, &ps);
    for (int i = 0; i < ps.n; i++) if (LC_AT(&ps, const tree_t *, i)->v.sval) lc_vec_push(pn, &LC_AT(&ps, const tree_t *, i)->v.sval);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * lower_icon_proc(const tree_t * prog, const tree_t * pd) {
    static lc_vec pnv; lc_vec_init(&pnv, (int) sizeof(const char *)); fill_pnames(prog, &pnv);
    icx_t cx; memset(&cx, 0, sizeof cx); cx.pn = (const char **) pnv.data; cx.npn = pnv.n;
    /* PROC-VALUE: the per-proc DECLARED-name set (params + `local`/`static` idents) — the lexical truth TT_FNC
       needs to route a computed callee (p(x) where p is a declared local) by VALUE instead of by NAME, and the
       resolve pass needs to keep a declared local named after a builtin out of IR_PROC_VALUE classification. */
    static lc_vec lnv; lc_vec_init(&lnv, (int) sizeof(const char *)); lnv.n = 0;
    if (pd) {
        const tree_t * plist = (pd->n > 1) ? pd->c[1] : NULL;
        for (int i = 0; plist && i < plist->n; i++) if (plist->c[i] && plist->c[i]->v.sval) lc_vec_push(&lnv, &plist->c[i]->v.sval);
        const tree_t * body = (pd->n > 2) ? pd->c[2] : NULL;
        for (int i = 0; body && i < body->n; i++) {
            const tree_t * st = body->c[i]; if (st && st->t == TT_STMT) st = stmt_subj(st);
            if (st && (st->t == TT_LOCAL || st->t == TT_STATIC_DECL))
                for (int k = 0; k < st->n; k++) if (st->c[k] && st->c[k]->v.sval) lc_vec_push(&lnv, &st->c[k]->v.sval);
        }
    }
    cx.ln = (const char **) lnv.data; cx.nln = lnv.n;
    if (pd && pd->n > 2 && pd->c[2]) return lower_proc_body(&cx, pd->c[2]);
    IR_graph_t * g = IR_alloc(64, IR_LANG_ICN); cx.g = g; IR_t * s = build(&cx, IR_SUCCEED, 0, 0); g->entry = s; return g;
}
/*====================================================================================================================================================================================================*/
#include "bb_program.h"
#include "emit.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_subtree_has_suspend(const tree_t *n) {
    if (!n) return 0;
    if (n->t == TT_SUSPEND) return 1;
    if (n->t == TT_PROC_DECL || n->t == TT_SUB_DECL) return 0;
    for (int i = 0; i < n->n; i++) if (icn_subtree_has_suspend(n->c[i])) return 1;
    return 0;
}
static int icn_body_has_suspend(const tree_t *proc) {
    if (!proc) return 0;
    for (int i = 0; i < proc->n; i++) if (icn_subtree_has_suspend(proc->c[i])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int lower_icon_body(const tree_t *prog, const tree_t *proc) {
    IR_graph_t * ng = lower_icon_proc(prog, proc);
    if (!ng || !ng->entry) return -1;
    return bb_program_add(&g_stage2.bbp, ng);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void lower_icon_resolve_call_kinds(void);
void lower_icon_stage2(const tree_t *prog) {
    for (int pi = 0; pi < g_stage2.proc_count; pi++) {
        const tree_t *proc = (const tree_t *) g_stage2.proc_table[pi].proc;
        if (!proc || proc->t != TT_PROC_DECL) continue;
        g_stage2.proc_table[pi].is_generator = icn_body_has_suspend(proc);
    }
    for (int pi = 0; pi < g_stage2.proc_count; pi++) {
        const tree_t *proc = (const tree_t *) g_stage2.proc_table[pi].proc;
        if (!proc || proc->t != TT_PROC_DECL) continue;
        if (g_stage2.proc_table[pi].bb_idx >= 0) continue;
        int bb_idx = lower_icon_body(prog, proc);
        if (bb_idx >= 0) {
            g_stage2.proc_table[pi].bb_idx = bb_idx;
            g_stage2.bbp.table[bb_idx]->nparams = g_stage2.proc_table[pi].nparams;
            const tree_t *plist = (proc->n >= 2) ? proc->c[1] : NULL;
            int np = g_stage2.proc_table[pi].nparams;
            Scope *sc = &g_stage2.proc_table[pi].lower_sc;
            sc->n = 0;
            for (int k = 0; k < np && plist && k < plist->n && sc->n < STAGE2_FRAME_SLOT_MAX; k++) {
                const tree_t *pv = plist->c[k];
                if (!pv || !pv->v.sval) continue;
                sc->e[sc->n].name = lp_strdup(pv->v.sval);
                sc->e[sc->n].slot = sc->n;
                sc->n++;
            }
            /* TE-4 VARSLOT ABSORPTION: stamp param names onto the graph (same source scrip.c's build sites
               extract from lower_sc) so ir_drive_slot_assign interns params at their ABI-fixed 16*(i+1) slots
               — the emitter's own param-interning loop is deleted with the allocator. */
            if (np > 0) {
                const char ** _pn = (const char **)calloc((size_t)np, sizeof(const char *));
                if (_pn) { for (int k = 0; k < np && k < sc->n; k++) _pn[k] = sc->e[k].name; g_stage2.bbp.table[bb_idx]->pnames = _pn; }
            }
        }
    }
    lower_icon_resolve_call_kinds();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* IRM→0 (Lon directive 2026-07-02): call-kind resolution moves HERE from the emitter — the last four ->op
   writes in src/emitter (resolve_call_kinds_descr, deleted) violated both the FACT RULE (emitter never
   mutates IR) and the DO-NOT (never decide a call kind from rt_* at emit time). The decision is compile-time:
   user-proc truth is g_stage2.proc_table (icn_callable_proc_index mirrors the driver's rt_proc_register
   declare-set EXACTLY: named, not main, lowered graph with entry — so registered ≡ table-callable), the
   generator bit is proc_table[pi].is_generator (the same value the driver stamps into rt), and the builtin
   sets are the static name lists in by_name_dispatch.c (rt_builtin_is_known's internal registered-guard is
   reachability-equivalent here: a table-callable name took branch 1/2 before branch 4). Ladder + the
   write/writes exclusion are the old pass verbatim. Runs after both stage2 loops, so every call resolves
   with complete knowledge — identical inputs, identical retags, now BEFORE the IR reaches the emitter. */
static int icn_callable_proc_index(const char * fn) {
    for (int pi = 0; pi < g_stage2.proc_count; pi++) {
        const char * pname = g_stage2.proc_table[pi].name;
        if (!pname || strcmp(pname, fn) != 0) continue;
        if (strcmp(pname, "main") == 0) return -1;
        int idx = g_stage2.proc_table[pi].bb_idx;
        if (idx < 0 || idx >= g_stage2.bbp.count || !g_stage2.bbp.table[idx] || !g_stage2.bbp.table[idx]->entry) return -1;
        return pi;
    }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void lower_icon_resolve_call_kinds(void) {
    extern int rt_builtin_is_generator(const char *);
    extern int rt_builtin_is_known(const char *);
    extern int is_global(const char *);
    for (int gi = 0; gi < g_stage2.bbp.count; gi++) {
        IR_graph_t * g = g_stage2.bbp.table[gi];
        if (!g) continue;
        for (int i = 0; i < g->n; i++) {
            IR_t * nd = g->all[i];
            if (!nd || nd->op != IR_CALL) continue;
            const char * fn = IR_LIT(nd).sval;
            if (!fn || !fn[0]) continue;
            int pi = icn_callable_proc_index(fn);
            if (pi >= 0 && g_stage2.proc_table[pi].is_generator) nd->op = IR_PROC_GEN;
            else if (pi >= 0) nd->op = IR_CALL_PROC_STAGED;
            else if (rt_builtin_is_generator(fn)) nd->op = IR_CALL_BUILTIN;
            else if (strcmp(fn, "write") && strcmp(fn, "writes") && rt_builtin_is_known(fn)) nd->op = IR_CALL_BUILTIN;
        }
        /* PROC-VALUE classification (LOWER-side, the IRM->0 doctrine — every specialization decided here, never
           in the emitter): an IR_VAR in VALUE position whose name (a) is not a param, (b) is never assigned in
           this graph (the exact set ir_drive_slot_assign interns as locals), (c) is not a declared global, and
           (d) IS a known user proc or builtin, denotes the PROCEDURE VALUE (canonical: builtin/proc names are
           pre-bound globals) -> retag to the IR_PROC_VALUE leaf (rt_proc_value mints DT_E{sentinel,name}). */
        for (int i = 0; i < g->n; i++) {
            IR_t * nd = g->all[i];
            if (!nd || nd->op != IR_VAR) continue;
            const char * vn = IR_LIT(nd).sval;
            if (!vn || !vn[0] || vn[0] == '&') continue;
            if (is_global(vn)) continue;
            int skip = 0;
            for (int k = 0; !skip && g->pnames && k < g->nparams; k++) if (g->pnames[k] && !strcmp(g->pnames[k], vn)) skip = 1;
            for (int k = 0; !skip && k < g->n; k++) {
                IR_t * a = g->all[k]; if (!a) continue;
                if (a->op == IR_ASSIGN && IR_LIT(a).sval && !strcmp(IR_LIT(a).sval, vn)) skip = 1;
                if (a->op == IR_REV_ASSIGN && a->n_operands > 1 && a->operands[1] && IR_LIT(a->operands[1]).sval && !strcmp(IR_LIT(a->operands[1]).sval, vn)) skip = 1;
            }
            if (skip) continue;
            int isproc = 0;
            for (int pi = 0; pi < g_stage2.proc_count; pi++) if (g_stage2.proc_table[pi].name && !strcmp(g_stage2.proc_table[pi].name, vn)) { isproc = 1; break; }
            if (isproc || rt_builtin_is_known(vn) || rt_builtin_is_generator(vn) || !strcmp(vn, "push") || !strcmp(vn, "put")) nd->op = IR_PROC_VALUE;
        }
    }
}
