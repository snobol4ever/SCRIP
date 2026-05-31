#define BB_DEFINE_NAMES
#include "lower.h"
#include "../../frontend/snobol4/scrip_cc.h"
#include "../ast/ast.h"
#include "ast_clone.h"
#include "../../runtime/interp/gen_runtime.h"
#include "../../runtime/interp/resolve_runtime.h"
#include "../../driver/polyglot.h"
#include "../runtime/rt/rt_protected.h"
#include "../../frontend/icon/icon_lex.h"
#include "../../frontend/raku/raku_driver.h"
#include "../../frontend/raku/raku_re.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <gc/gc.h>
#include "core.h"
#include "coerce.h"
#include "../frontend/prolog/prolog_atom.h"
#include <math.h>
#include <stddef.h>
#define LOWER_UNHANDLED_WORDS 4
static int          g_in_proc_body;
static int          g_in_gen_proc_body;
static Scope    *g_proc_scope;
static unsigned long long g_unhandled_kinds[LOWER_UNHANDLED_WORDS];
#define RK_GRAMMAR_NAMES_MAX 64
static const char *g_rk_grammar_names[RK_GRAMMAR_NAMES_MAX];
static int g_rk_grammar_name_count = 0;
static int          g_in_value_ctx;
static const tree_t *g_hoist_alt   = NULL;
#define LOWER_LOOP_STACK_MAX 64
static struct { const char *cont; const char *end; } g_loop_stack[LOWER_LOOP_STACK_MAX];
static int g_loop_sp = 0;
static int g_resolve_initialization_seen = 0;
static void loop_push(const char *cont, const char *end) {
    if (g_loop_sp < LOWER_LOOP_STACK_MAX) { g_loop_stack[g_loop_sp].cont = cont; g_loop_stack[g_loop_sp].end = end; g_loop_sp++; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void loop_pop(void) { if (g_loop_sp > 0) g_loop_sp--; }
static int g_loop_label_seq = 0;
static char *lower_fresh_label(const char *prefix) {
    char buf[64];
    snprintf(buf, sizeof buf, "%s_%d", prefix, g_loop_label_seq++);
    return strdup(buf);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *g_sc_func_name = NULL;
static int           g_hoist_entry = -1;
static int           g_hoist_slot  = -1;
extern int g_lang;
static tree_t *attr_expr_of(const tree_t *s, const char *tag)
{
    tree_t *a = stmt_attr_find(s, tag);
    return a ? stmt_attr_expr(a) : NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *attr_str_of(const tree_t *s, const char *tag)
{
    tree_t *a = stmt_attr_find(s, tag);
    return stmt_attr_str(a);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int attr_int_of(const tree_t *s, const char *tag)
{
    const char *sv = attr_str_of(s, tag);
    return sv ? atoi(sv) : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void build_proc_scope(Scope *sc, const tree_t *proc, int body_start)
{
    sc->n = 0;
    if (proc->t == TT_SUB_DECL) {
        int nparams = (int)proc->v.ival;
        int is_method = (proc->n > 0 && proc->c[0] && proc->c[0]->v.sval && strstr(proc->c[0]->v.sval, "__") != NULL);
        if (is_method) {
            scope_add(sc, "self");
            for (int i = 1; i < nparams && i < proc->n && i <= FRAME_SLOT_MAX; i++) {
                tree_t *pn = proc->c[i];
                if (pn && pn->t == TT_VAR && pn->v.sval) scope_add(sc, pn->v.sval);
            }
            for (int i = nparams; i < proc->n; i++) {
                if (proc->c[i]) expression_scope_walk(sc, proc->c[i]);
            }
        } else {
            for (int i = 1; i <= nparams && i < proc->n && i <= FRAME_SLOT_MAX; i++) {
                tree_t *pn = proc->c[i];
                if (pn && pn->t == TT_VAR && pn->v.sval) scope_add(sc, pn->v.sval);
            }
            for (int i = nparams + 1; i < proc->n; i++) {
                if (proc->c[i]) expression_scope_walk(sc, proc->c[i]);
            }
        }
        return;
    }
    tree_t *plist = (proc->t == TT_PROC_DECL && proc->n >= 2) ? proc->c[1] : NULL;
    tree_t *bnode = (proc->t == TT_PROC_DECL && proc->n >= 3) ? proc->c[2] : NULL;
    int nparams = plist ? plist->n : 0;
    for (int i = 0; i < nparams && i < FRAME_SLOT_MAX; i++) {
        tree_t *pn = plist->c[i];
        if (pn && pn->v.sval) scope_add(sc, pn->v.sval);
    }
    int bn = bnode ? bnode->n : 0;
    for (int i = 0; i < bn; i++)
        expression_scope_walk(sc, bnode->c[i]);
    for (int i = 0; i < bn; i++) {
        tree_t *ch = bnode->c[i];
        if (!ch || ch->t != TT_INITIAL) continue;
        for (int ai = 0; ai < ch->n; ai++) {
            tree_t *as = ch->c[ai];
            if (!as || as->t != TT_ASSIGN || as->n < 1) continue;
            tree_t *lhs = as->c[0];
            if (!lhs || lhs->t != TT_VAR || !lhs->v.sval) continue;
            int w = 0;
            for (int r = 0; r < sc->n; r++) {
                if (sc->e[r].name && strcmp(sc->e[r].name, lhs->v.sval) == 0) continue;
                if (w != r) sc->e[w] = sc->e[r]; w++;
            }
            sc->n = w;
            for (int k = 0; k < sc->n; k++) sc->e[k].slot = k;
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void lower_proc_skeletons(void)
{
    for (int pi = 0; pi < g_stage2.proc_count; pi++) {
        const char *nm = g_stage2.proc_table[pi].name;
        if (!nm || !*nm) continue;
        tree_t *proc = g_stage2.proc_table[pi].proc;
        if (proc && proc->t == TT_PROC_DECL) {
            int body_start = 0;
            Scope sc; build_proc_scope(&sc, proc, body_start);
            g_stage2.proc_table[pi].lower_sc = sc;
            IR_graph_t *_irb = lower_proc_body(proc);
            g_stage2.proc_table[pi].bb_idx = _irb ? bb_program_add(&g_stage2.bbp, _irb) : -1;
            g_stage2.proc_table[pi].is_generator = 0;
            if (_irb) {
                for (int _k = 0; _k < _irb->n; _k++) {
                    if (_irb->all[_k] && _irb->all[_k]->t == IR_SUSPEND) { g_stage2.proc_table[pi].is_generator = 1; break; }
                }
            }
            continue;
        }
        if (proc) {
            int body_start = 0;
            Scope sc; build_proc_scope(&sc, proc, body_start);
            g_stage2.proc_table[pi].lower_sc = sc;
            IR_graph_t *_irb = lower_proc_body(proc);
            g_stage2.proc_table[pi].bb_idx = _irb ? bb_program_add(&g_stage2.bbp, _irb) : -1;
            g_stage2.proc_table[pi].is_generator = 0;
            if (_irb) {
                for (int _k = 0; _k < _irb->n; _k++) {
                    if (_irb->all[_k] && _irb->all[_k]->t == IR_SUSPEND) { g_stage2.proc_table[pi].is_generator = 1; break; }
                }
            }
            if (!g_stage2.proc_table[pi].is_generator) {
                tree_t *stack[256]; int sp = 0;
                stack[sp++] = proc;
                while (sp > 0) {
                    tree_t *n = stack[--sp];
                    if (!n) continue;
                    if (n->t == TT_SUSPEND) { g_stage2.proc_table[pi].is_generator = 1; break; }
                    for (int _ci = 0; _ci < n->n && sp < 254; _ci++) if (n->c[_ci]) stack[sp++] = n->c[_ci];
                }
            }
        }
    }
    for (int b = 0; b < RESOLVE_PRED_TABLE_SIZE_FWD; b++) {
        for (Resolve_PredEntry *e = g_stage2.resolve_pred_table.buckets[b]; e; e = e->next) {
            if (!e->key || !*e->key) continue;
            const char *slash = strrchr(e->key, '/');
            int arity = slash ? atoi(slash + 1) : 0;
            IR_graph_t *ir_body = lower_pl_predicate(e->choice);
            int resolve_bb_idx = ir_body ? bb_program_add(&g_stage2.bbp, ir_body) : -1;
            Resolve_PredEntry_BB *bb = resolve_bb_register(e->key, arity, resolve_bb_idx);
            if (bb && e->choice) {
                PlScope sc; sc.n = 0;
                bb->lower_sc = sc;
            }
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int     g_ghoist_seq  = 0;
static tree_t *g_ghoist_defs[256];
static int     g_ghoist_ndef = 0;
static void lower_hoist_gather_in_expr(tree_t * e) {
    if (!e) return;
    for (int i = 0; i < e->n; i++) lower_hoist_gather_in_expr(e->c[i]);
    if (e->t != TT_GATHER) return;
    char gname[32]; snprintf(gname, sizeof gname, "__gather_%d", g_ghoist_seq++);
    tree_t * def = ast_node_new(TT_SUB_DECL); def->v.ival = 0;
    tree_t * dn  = ast_node_new(TT_VAR); dn->v.sval = intern(gname);
    expr_add_child(def, dn);
    tree_t * body = (e->n >= 1) ? e->c[0] : NULL;
    if (body) for (int i = 0; i < body->n; i++) expr_add_child(def, body->c[i]);
    if (g_ghoist_ndef < 256) g_ghoist_defs[g_ghoist_ndef++] = def;
    e->t      = TT_FNC;
    e->v.sval = intern(gname);
    e->n      = 0;
    e->c      = NULL;
    tree_t * cn = ast_node_new(TT_VAR); cn->v.sval = intern(gname);
    expr_add_child(e, cn);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void lower_gather_hoist_pass(const tree_t * prog) {
    if (!prog) return;
    g_ghoist_seq  = 0;
    g_ghoist_ndef = 0;
    for (int i = 0; i < prog->n; i++) {
        tree_t * st = prog->c[i];
        if (!st || st->t != TT_STMT) continue;
        for (int j = 0; j < st->n; j++) {
            tree_t * ch = st->c[j];
            if (!ch) continue;
            if (ch->t == TT_ATTR) { for (int k = 0; k < ch->n; k++) lower_hoist_gather_in_expr(ch->c[k]); continue; }
            lower_hoist_gather_in_expr(ch);
        }
    }
    if (!g_ghoist_ndef) return;
    int old_n = prog->n;
    int new_n = old_n + g_ghoist_ndef;
    char * block = (char *)malloc(sizeof(size_t) + (size_t)new_n * sizeof(tree_t *));
    tree_t ** new_c = (tree_t **)(block + sizeof(size_t));
    *(size_t *)block = (size_t)new_n;
    for (int i = 0; i < g_ghoist_ndef; i++) {
        tree_t * st = ast_stmt_new(TT_STMT);
        expr_add_child(st, ast_attr_int(":lang",  LANG_RAKU));
        expr_add_child(st, ast_attr_int(":line",  0));
        expr_add_child(st, ast_attr_int(":stno",  0));
        expr_add_child(st, ast_attr_expr(":subj", g_ghoist_defs[i]));
        new_c[i] = st;
    }
    for (int i = 0; i < old_n; i++) new_c[g_ghoist_ndef + i] = prog->c[i];
    if (prog->c) free((char *)prog->c - sizeof(size_t));
    ((tree_t *)prog)->c = new_c;
    ((tree_t *)prog)->n = new_n;
    for (int i = 0; i < g_ghoist_ndef; i++) {
        tree_t *proc = g_ghoist_defs[i];
        if (!proc) continue;
        const char *name = (proc->v.sval && *proc->v.sval) ? proc->v.sval
                         : ((proc->n > 0 && proc->c[0] && proc->c[0]->t == TT_VAR && proc->c[0]->v.sval && *proc->c[0]->v.sval)
                            ? proc->c[0]->v.sval : NULL);
        if (!name) continue;
        int _pi = stage2_proc_grow(&g_stage2);
        g_stage2.proc_table[_pi].name     = name;
        g_stage2.proc_table[_pi].proc     = proc;
        g_stage2.proc_table[_pi].entry_pc = -1;
        g_stage2.proc_table[_pi].bb_idx   = -1;
        g_stage2.proc_table[_pi].nparams  = (int)proc->v.ival;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void lower_class_prescan(const tree_t *prog)
{
    for (int ci = 0; ci < prog->n; ci++) {
        const tree_t *s = prog->c[ci];
        if (!s || s->t != TT_STMT) continue;
        const tree_t *subj = attr_expr_of(s, ":subj");
        if (!subj || subj->t != TT_CLASS_DECL) continue;
        const char *cname = (subj->n >= 1 && subj->c[0] && subj->c[0]->v.sval) ? subj->c[0]->v.sval : "";
        for (int i = 1; i < subj->n; i++) {
            tree_t *item = subj->c[i];
            if (!item || item->t != TT_SUB_DECL) continue;
            const char *shortname = (item->n > 0 && item->c[0] && item->c[0]->v.sval) ? item->c[0]->v.sval : "";
            int nparams = (int)item->v.ival;
            char fullname[256]; snprintf(fullname, sizeof fullname, "%s__%s", cname, shortname);
            const char *fname = intern(fullname);
            if (item->n > 0 && item->c[0]) item->c[0]->v.sval = (char *)fname;
            int _pi = stage2_proc_grow(&g_stage2);
            g_stage2.proc_table[_pi].name     = fname;
            g_stage2.proc_table[_pi].proc     = item;
            g_stage2.proc_table[_pi].entry_pc = -1;
            g_stage2.proc_table[_pi].bb_idx   = -1;
            g_stage2.proc_table[_pi].nparams  = nparams;
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void lower_grammar_prescan(const tree_t *prog)
{
    g_rk_grammar_name_count = 0;
    for (int ci = 0; ci < prog->n; ci++) {
        const tree_t *s = prog->c[ci];
        if (!s || s->t != TT_STMT) continue;
        const tree_t *subj = attr_expr_of(s, ":subj");
        if (!subj || subj->t != TT_GRAMMAR_DECL) continue;
        const char *gname = (subj->n >= 1 && subj->c[0] && subj->c[0]->v.sval) ? subj->c[0]->v.sval : "";
        if (*gname && g_rk_grammar_name_count < RK_GRAMMAR_NAMES_MAX)
            g_rk_grammar_names[g_rk_grammar_name_count++] = intern(gname);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
stage2_t *lower(const tree_t *prog)
{
    if (!prog || prog->t != TT_PROGRAM) return NULL;
    stage2_reset();
    polyglot_init(&g_stage2, prog, polyglot_lang_mask(prog));
    g_in_proc_body = 0;
    g_proc_scope   = NULL;
    g_loop_label_seq = 0;
    g_sc_func_name = NULL;
    g_resolve_initialization_seen = 0;
    for (int i = 0; i < LOWER_UNHANDLED_WORDS; i++) g_unhandled_kinds[i] = 0;
    int has_raku = 0;
    for (int i = 0; i < prog->n && !has_raku; i++) { const tree_t * s = prog->c[i]; if (s && s->t == TT_STMT && attr_int_of(s, ":lang") == LANG_RAKU) has_raku = 1; }
    if (has_raku) lower_gather_hoist_pass(prog);
    if (has_raku) lower_class_prescan(prog);
    if (has_raku) lower_grammar_prescan(prog);
    lower_proc_skeletons();
    g_stage2.lang = g_lang;
    return &g_stage2;
}
/*====================================================================================================================================================================================================*/
/* ===== ICON GENERATOR LOWERING (the model: four-port threaded, derived from JCON ir_a_*) ===== */
/*====================================================================================================================================================================================================*/
IR_t *lower_new_Intlit(IR_graph_t *bbg, struct tree_t *e) {
    IR_t *bb = IR_node_alloc(bbg, IR_LIT_I);
    if (!bb) return NULL;
    bb->ival = e->v.ival;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Reallit(IR_graph_t *bbg, struct tree_t *e) {
    IR_t *bb = IR_node_alloc(bbg, IR_LIT_F);
    if (!bb) return NULL;
    bb->dval = e->v.dval;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Stringlit(IR_graph_t *bbg, struct tree_t *e) {
    IR_t *bb = IR_node_alloc(bbg, IR_LIT_S);
    if (!bb) return NULL;
    bb->sval = e->v.sval ? e->v.sval : "";
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Csetlit(IR_graph_t *bbg, struct tree_t *e) {
    IR_t *bb = IR_node_alloc(bbg, IR_LIT_S);
    if (!bb) return NULL;
    bb->sval = e->v.sval ? e->v.sval : "";
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Global(IR_graph_t *bbg, struct tree_t *e) {
    (void)e;
    return IR_node_alloc(bbg, IR_SUCCEED);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern int is_suspendable(struct tree_t *e);
extern IR_t *lower_expr_node(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_Binop(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    IR_t *lhs = lower_expr_node(bbg, e->c[0]);
    if (!lhs) return NULL;
    IR_t *rhs = lower_expr_node(bbg, e->c[1]);
    if (!rhs) return NULL;
    if (e->t == TT_LCONCAT) {
        IR_t *bb = IR_node_alloc(bbg, IR_LCONCAT);
        if (!bb) return NULL;
        bb->α = lhs; bb->β = rhs;
        return bb;
    }
    BinopKind op = BINOP_ADD;
    int is_relop  = 0;
    int is_strrel = 0;
    switch (e->t) {
    case TT_ADD: op = BINOP_ADD;    break;
    case TT_SUB: op = BINOP_SUB;    break;
    case TT_MUL: op = BINOP_MUL;    break;
    case TT_DIV: op = BINOP_DIV;    break;
    case TT_MOD: op = BINOP_MOD;    break;
    case TT_POW: op = BINOP_POW;    break;
    case TT_LT:  op = BINOP_LT; is_relop = 1; break;
    case TT_LE:  op = BINOP_LE; is_relop = 1; break;
    case TT_GT:  op = BINOP_GT; is_relop = 1; break;
    case TT_GE:  op = BINOP_GE; is_relop = 1; break;
    case TT_EQ:  op = BINOP_EQ; is_relop = 1; break;
    case TT_NE:  op = BINOP_NE; is_relop = 1; break;
    case TT_CAT: op = BINOP_CONCAT; break;
    case TT_LLT: op = BINOP_SLT; is_strrel = 1; break;
    case TT_LLE: op = BINOP_SLE; is_strrel = 1; break;
    case TT_LGT: op = BINOP_SGT; is_strrel = 1; break;
    case TT_LGE: op = BINOP_SGE; is_strrel = 1; break;
    case TT_LEQ: op = BINOP_SEQ; is_strrel = 1; break;
    case TT_LNE: op = BINOP_SNE; is_strrel = 1; break;
    default: return NULL;
    }
    int is_gen = is_strrel ? 0 : (is_suspendable(e->c[0]) || is_suspendable(e->c[1]));
    IR_t *bb = IR_node_alloc(bbg, is_gen ? IR_BINOP_GEN : IR_BINOP);
    if (!bb) return NULL;
    bb->α     = lhs;
    bb->β     = rhs;
    bb->ival  = (int64_t)op;
    bb->state = is_strrel ? 1 : is_relop;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Binop_ag(IR_graph_t *bbg, struct tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    BinopKind op = BINOP_ADD;
    int is_relop  = 0;
    int is_strrel = 0;
    switch (e->t) {
    case TT_ADD: op = BINOP_ADD;    break;
    case TT_SUB: op = BINOP_SUB;    break;
    case TT_MUL: op = BINOP_MUL;    break;
    case TT_DIV: op = BINOP_DIV;    break;
    case TT_MOD: op = BINOP_MOD;    break;
    case TT_POW: op = BINOP_POW;    break;
    case TT_LT:  op = BINOP_LT; is_relop = 1; break;
    case TT_LE:  op = BINOP_LE; is_relop = 1; break;
    case TT_GT:  op = BINOP_GT; is_relop = 1; break;
    case TT_GE:  op = BINOP_GE; is_relop = 1; break;
    case TT_EQ:  op = BINOP_EQ; is_relop = 1; break;
    case TT_NE:  op = BINOP_NE; is_relop = 1; break;
    case TT_CAT: op = BINOP_CONCAT; break;
    case TT_LLT: op = BINOP_SLT; is_strrel = 1; break;
    case TT_LLE: op = BINOP_SLE; is_strrel = 1; break;
    case TT_LGT: op = BINOP_SGT; is_strrel = 1; break;
    case TT_LGE: op = BINOP_SGE; is_strrel = 1; break;
    case TT_LEQ: op = BINOP_SEQ; is_strrel = 1; break;
    case TT_LNE: op = BINOP_SNE; is_strrel = 1; break;
    default: return NULL;
    }
    int is_gen = is_strrel ? 0 : (is_suspendable(e->c[0]) || is_suspendable(e->c[1]));
    if (is_gen) return NULL;
    IR_t *l_αo = NULL, *l_βo = NULL;
    IR_t *lhs = lower_expr_threaded_b(bbg, e->c[0], NULL, ω_in, &l_αo, &l_βo, 0);
    if (!lhs) return NULL;
    IR_t *lhs_entry = l_αo ? l_αo : lhs;
    IR_t *r_αo = NULL, *r_βo = NULL;
    IR_t *rhs = lower_expr_threaded_b(bbg, e->c[1], NULL, ω_in, &r_αo, &r_βo, 0);
    if (!rhs) return NULL;
    IR_t *rhs_entry = r_αo ? r_αo : rhs;
    IR_t *bb = IR_node_alloc(bbg, IR_BINOP);
    if (!bb) return NULL;
    bb->ival  = (int64_t)op;
    bb->state = is_strrel ? 1 : is_relop;
    if (!lhs->γ) lhs->γ = rhs_entry;
    if (!lhs->ω) lhs->ω = ω_in;
    if (!rhs->γ) rhs->γ = bb;
    if (!rhs->ω) rhs->ω = ω_in;
    if (!bb->γ && γ_in) bb->γ = γ_in;
    if (!bb->ω && ω_in) bb->ω = ω_in;
    if (α_out) *α_out = lhs_entry;
    if (β_out) *β_out = ω_in;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Lconcat_ag(IR_graph_t *bbg, struct tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    IR_t *l_αo = NULL, *l_βo = NULL;
    IR_t *lhs = lower_expr_threaded_b(bbg, e->c[0], NULL, ω_in, &l_αo, &l_βo, 0);
    if (!lhs) return NULL;
    IR_t *lhs_entry = l_αo ? l_αo : lhs;
    IR_t *r_αo = NULL, *r_βo = NULL;
    IR_t *rhs = lower_expr_threaded_b(bbg, e->c[1], NULL, ω_in, &r_αo, &r_βo, 0);
    if (!rhs) return NULL;
    IR_t *rhs_entry = r_αo ? r_αo : rhs;
    IR_t *bb = IR_node_alloc(bbg, IR_LCONCAT);
    if (!bb) return NULL;
    if (!lhs->γ) lhs->γ = rhs_entry;
    if (!lhs->ω) lhs->ω = ω_in;
    if (!rhs->γ) rhs->γ = bb;
    if (!rhs->ω) rhs->ω = ω_in;
    if (!bb->γ && γ_in) bb->γ = γ_in;
    if (!bb->ω && ω_in) bb->ω = ω_in;
    if (α_out) *α_out = lhs_entry;
    if (β_out) *β_out = ω_in;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Sectionop_ag(IR_graph_t *bbg, struct tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out) {
    if (e->n < 3 || !e->c[0] || !e->c[1] || !e->c[2]) return NULL;
    IR_t *b_αo = NULL, *b_βo = NULL;
    IR_t *base = lower_expr_threaded_b(bbg, e->c[0], NULL, ω_in, &b_αo, &b_βo, 0);
    if (!base) return NULL;
    IR_t *base_entry = b_αo ? b_αo : base;
    IR_t *i1_αo = NULL, *i1_βo = NULL;
    IR_t *i1 = lower_expr_threaded_b(bbg, e->c[1], NULL, ω_in, &i1_αo, &i1_βo, 0);
    if (!i1) return NULL;
    IR_t *i1_entry = i1_αo ? i1_αo : i1;
    IR_t *i2_αo = NULL, *i2_βo = NULL;
    IR_t *i2 = lower_expr_threaded_b(bbg, e->c[2], NULL, ω_in, &i2_αo, &i2_βo, 0);
    if (!i2) return NULL;
    IR_t *i2_entry = i2_αo ? i2_αo : i2;
    IR_t *bb = IR_node_alloc(bbg, IR_SECTION);
    if (!bb) return NULL;
    bb->ival = (e->t == TT_SECTION) ? 0 : (e->t == TT_SECTION_PLUS) ? 1 : 2;
    if (!base->γ) base->γ = i1_entry;
    if (!base->ω) base->ω = ω_in;
    if (!i1->γ) i1->γ = i2_entry;
    if (!i1->ω) i1->ω = ω_in;
    if (!i2->γ) i2->γ = bb;
    if (!i2->ω) i2->ω = ω_in;
    if (!bb->γ && γ_in) bb->γ = γ_in;
    if (!bb->ω && ω_in) bb->ω = ω_in;
    if (α_out) *α_out = base_entry;
    if (β_out) *β_out = ω_in;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Idx_ag(IR_graph_t *bbg, struct tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    IR_t *b_αo = NULL, *b_βo = NULL;
    IR_t *base = lower_expr_threaded_b(bbg, e->c[0], NULL, ω_in, &b_αo, &b_βo, 0);
    if (!base) return NULL;
    IR_t *base_entry = b_αo ? b_αo : base;
    IR_t *x_αo = NULL, *x_βo = NULL;
    IR_t *idx = lower_expr_threaded_b(bbg, e->c[1], NULL, ω_in, &x_αo, &x_βo, 0);
    if (!idx) return NULL;
    IR_t *idx_entry = x_αo ? x_αo : idx;
    IR_t *bb = IR_node_alloc(bbg, IR_IDX);
    if (!bb) return NULL;
    if (!base->γ) base->γ = idx_entry;
    if (!base->ω) base->ω = ω_in;
    if (!idx->γ) idx->γ = bb;
    if (!idx->ω) idx->ω = ω_in;
    if (!bb->γ && γ_in) bb->γ = γ_in;
    if (!bb->ω && ω_in) bb->ω = ω_in;
    if (α_out) *α_out = base_entry;
    if (β_out) *β_out = ω_in;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Idx_set_ag(IR_graph_t *bbg, struct tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    if (e->c[0]->t != TT_IDX) return NULL;
    if (e->c[0]->n < 2 || !e->c[0]->c[0] || !e->c[0]->c[1]) return NULL;
    IR_t *b_αo = NULL, *b_βo = NULL;
    IR_t *base = lower_expr_threaded_b(bbg, e->c[0]->c[0], NULL, ω_in, &b_αo, &b_βo, 0);
    if (!base) return NULL;
    IR_t *base_entry = b_αo ? b_αo : base;
    IR_t *x_αo = NULL, *x_βo = NULL;
    IR_t *idx = lower_expr_threaded_b(bbg, e->c[0]->c[1], NULL, ω_in, &x_αo, &x_βo, 0);
    if (!idx) return NULL;
    IR_t *idx_entry = x_αo ? x_αo : idx;
    IR_t *r_αo = NULL, *r_βo = NULL;
    IR_t *rhs = lower_expr_threaded_b(bbg, e->c[1], NULL, ω_in, &r_αo, &r_βo, 0);
    if (!rhs) return NULL;
    IR_t *rhs_entry = r_αo ? r_αo : rhs;
    IR_t *bb = IR_node_alloc(bbg, IR_IDX_SET);
    if (!bb) return NULL;
    if (!base->γ) base->γ = idx_entry;
    if (!base->ω) base->ω = ω_in;
    if (!idx->γ) idx->γ = rhs_entry;
    if (!idx->ω) idx->ω = ω_in;
    if (!rhs->γ) rhs->γ = bb;
    if (!rhs->ω) rhs->ω = ω_in;
    if (!bb->γ && γ_in) bb->γ = γ_in;
    if (!bb->ω && ω_in) bb->ω = ω_in;
    if (α_out) *α_out = base_entry;
    if (β_out) *β_out = ω_in;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_If(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 1 || !e->c[0]) return NULL;
    IR_t *cond = lower_expr_node(bbg, e->c[0]);
    if (!cond) return NULL;
    IR_t *then_nd = NULL;
    IR_t *else_nd = NULL;
    if (e->n >= 2 && e->c[1]) {
        then_nd = lower_expr_node(bbg, e->c[1]);
        if (!then_nd) return NULL;
    }
    if (e->n >= 3 && e->c[2]) {
        else_nd = lower_expr_node(bbg, e->c[2]);
        if (!else_nd) return NULL;
    }
    IR_t *bb = IR_node_alloc(bbg, IR_IF);
    if (!bb) return NULL;
    bb->α = cond;
    if (then_nd) bb->β = then_nd;
    if (else_nd) bb->ω = else_nd;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_If_ag(IR_graph_t *bbg, struct tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out) {
    if (e->n < 1 || !e->c[0]) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_IF);
    if (!bb) return NULL;
    IR_t *cond_αo = NULL, *cond_βo = NULL;
    IR_t *cond = lower_expr_threaded_b(bbg, e->c[0], bb, bb, &cond_αo, &cond_βo, 0);
    if (!cond) return NULL;
    IR_t *cond_entry = cond_αo ? cond_αo : cond;
    if (!cond->γ) cond->γ = bb;
    if (!cond->ω) cond->ω = bb;
    IR_t *then_entry = NULL;
    IR_t *else_entry = NULL;
    if (e->n >= 2 && e->c[1]) {
        IR_t *αo = NULL, *βo = NULL;
        IR_t *then_apply = lower_expr_threaded_b(bbg, e->c[1], γ_in, ω_in, &αo, &βo, 0);
        if (!then_apply) return NULL;
        then_entry = αo ? αo : then_apply;
        if (!then_apply->γ && γ_in) then_apply->γ = γ_in;
        if (!then_apply->ω && ω_in) then_apply->ω = ω_in;
    }
    if (e->n >= 3 && e->c[2]) {
        IR_t *αo = NULL, *βo = NULL;
        IR_t *else_apply = lower_expr_threaded_b(bbg, e->c[2], γ_in, ω_in, &αo, &βo, 0);
        if (!else_apply) return NULL;
        else_entry = αo ? αo : else_apply;
        if (!else_apply->γ && γ_in) else_apply->γ = γ_in;
        if (!else_apply->ω && ω_in) else_apply->ω = ω_in;
    }
    bb->γ = then_entry ? then_entry : γ_in;
    bb->ω = else_entry ? else_entry : γ_in;
    if (α_out) *α_out = cond_entry;
    if (β_out) *β_out = ω_in;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern int fold_signed_lit(struct tree_t *n, int64_t *iv, double *dv, int *is_real);
IR_t *lower_new_ToBy(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    if (e->t == TT_TO) {
        IR_t *bb = IR_node_alloc(bbg, IR_TO);
        if (!bb) return NULL;
        int lo_lit = (e->c[0]->t == TT_ILIT);
        int hi_lit = (e->c[1]->t == TT_ILIT);
        if (lo_lit && hi_lit) {
            bb->α = NULL;
            bb->β = NULL;
            bb->ival = e->c[0]->v.ival;
            int64_t to_i = e->c[1]->v.ival;
            memcpy(&bb->dval, &to_i, 8);
        } else {
            IR_t *lo = lower_expr_node(bbg, e->c[0]);
            IR_t *hi = lower_expr_node(bbg, e->c[1]);
            if (!lo || !hi) return NULL;
            bb->α = lo;
            bb->β = hi;
        }
        return bb;
    }
    struct tree_t *lo_n = e->c[0];
    struct tree_t *hi_n = e->c[1];
    struct tree_t *by_n = (e->n >= 3 && e->c[2]) ? e->c[2] : NULL;
    int64_t step_i = 1; double step_r = 1.0; int step_is_real = 0;
    if (by_n) (void)fold_signed_lit(by_n, &step_i, &step_r, &step_is_real);
    int is_real = (lo_n->t == TT_FLIT || hi_n->t == TT_FLIT || step_is_real);
    IR_t *lo = lower_expr_node(bbg, lo_n);
    IR_t *hi = lower_expr_node(bbg, hi_n);
    if (!lo || !hi) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_TO_BY);
    if (!bb) return NULL;
    bb->α = lo;
    bb->β = hi;
    if (is_real) {
        double by_r = step_is_real ? step_r : (double)step_i;
        int64_t bi; memcpy(&bi, &by_r, 8);
        bb->ival = bi;
        bb->sval = "r";
    } else {
        bb->ival = step_i;
        bb->sval = "i";
    }
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_ToBy_ag(IR_graph_t *bbg, struct tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out) {
    IR_t *bb = lower_new_ToBy(bbg, e);
    if (!bb) return NULL;
    if (bb->α && bb->β) {
        IR_t *lo = bb->α;
        IR_t *hi = bb->β;
        bb->α = NULL;
        bb->β = NULL;
        if (e->t == TT_TO) { bb->sval = "ag"; }
        else { bb->sval = (bb->sval && bb->sval[0] == 'r') ? "ar" : "ai"; }
        if (!lo->γ) lo->γ = hi;
        if (!lo->ω) lo->ω = ω_in;
        if (!hi->γ) hi->γ = bb;
        if (!hi->ω) hi->ω = ω_in;
        if (!bb->γ && γ_in) bb->γ = γ_in;
        if (!bb->ω && ω_in) bb->ω = ω_in;
        if (α_out) *α_out = lo;
        if (β_out) *β_out = bb;
        return bb;
    }
    if (!bb->γ && γ_in) bb->γ = γ_in;
    if (!bb->ω && ω_in) bb->ω = ω_in;
    if (α_out) *α_out = bb;
    if (β_out) *β_out = bb;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern IR_t *lower_expr_threaded_b(IR_graph_t *bbg, struct tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out, int bounded);
IR_t *lower_new_Every(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 1 || !e->c[0]) return NULL;
    IR_t *gen = NULL;
    IR_t *gen_chain_entry = NULL;
    if (e->c[0]->t == TT_TO || e->c[0]->t == TT_TO_BY) {
        IR_t *αo = NULL, *βo = NULL;
        gen = lower_expr_threaded_b(bbg, e->c[0], NULL, NULL, &αo, &βo, 1);
        gen_chain_entry = αo ? αo : gen;
    } else {
        gen = lower_expr_node(bbg, e->c[0]);
        gen_chain_entry = gen;
    }
    if (!gen) return NULL;
    IR_t *body = NULL;
    if (e->n >= 2 && e->c[1]) {
        body = lower_expr_node(bbg, e->c[1]);
        if (!body) return NULL;
    }
    IR_t *bb = IR_node_alloc(bbg, IR_EVERY);
    if (!bb) return NULL;
    bb->α = gen_chain_entry;
    bb->β = body;
    if (gen->α == NULL && gen->β == NULL) {
        if (body) {
            gen->γ = body;
            body->γ = gen;
            body->ω = gen;
        } else {
            gen->γ = gen;
        }
        gen->ω = bb;
    }
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int lic_gen_kind_raw(IR_e k) {
    return k == IR_ALT || k == IR_BINOP_GEN || k == IR_ITERATE || k == IR_LIMIT ||
           k == IR_PROC_GEN || k == IR_LIST_BANG || k == IR_KEY_GEN || k == IR_FIND_GEN || k == IR_SEQ_GEN;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int lic_is_gen_node(IR_t *e) {
    if (!e) return 0;
    if (e->t == IR_ASSIGN) return lic_is_gen_node(e->β);
    return lic_gen_kind_raw(e->t);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int lic_body_bears_gen(IR_t *e, int depth) {
    if (!e || depth > 64) return 0;
    if (lic_gen_kind_raw(e->t)) return 1;
    if (e->t == IR_ASSIGN) return lic_body_bears_gen(e->β, depth + 1);
    for (IR_t *a = e->α; a; a = a->γ) if (lic_body_bears_gen(a, depth + 1)) return 1;
    if (e->β && lic_body_bears_gen(e->β, depth + 1)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Every_ag(IR_graph_t *bbg, struct tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out) {
    if (e->n < 1 || !e->c[0]) return NULL;
    IR_t *gen = NULL;
    IR_t *gen_chain_entry = NULL;
    if (e->c[0]->t == TT_ASSIGN && e->c[0]->n >= 2 && e->c[0]->c[1] &&
        e->c[0]->c[1]->t == TT_TO &&
        e->c[0]->c[0] && e->c[0]->c[0]->t == TT_VAR) {
        IR_t *αo = NULL, *βo = NULL;
        IR_t *togen = lower_new_ToBy_ag(bbg, e->c[0]->c[1], NULL, NULL, &αo, &βo);
        if (togen && togen->α == NULL && togen->β == NULL) {
            IR_t *lhs = lower_expr_node(bbg, e->c[0]->c[0]);
            if (!lhs || lhs->t != IR_VAR || !lhs->sval) return NULL;
            IR_t *store = IR_node_alloc(bbg, IR_ASSIGN);
            if (!store) return NULL;
            store->α   = lhs;
            store->β   = togen;
            store->ival = 1;
            IR_t *do_body = NULL;
            if (e->n >= 2 && e->c[1]) { do_body = lower_expr_node(bbg, e->c[1]); if (!do_body) return NULL; }
            IR_t *bb = IR_node_alloc(bbg, IR_EVERY);
            if (!bb) return NULL;
            gen_chain_entry = αo ? αo : togen;
            bb->α = gen_chain_entry;
            bb->β = store;
            togen->γ = store;
            if (do_body) { store->γ = do_body; do_body->γ = togen; do_body->ω = togen; } else { store->γ = togen; }
            togen->ω = bb;
            bb->ival = 1;
            if (!bb->γ && γ_in) bb->γ = γ_in;
            if (!bb->ω && ω_in) bb->ω = ω_in;
            if (α_out) *α_out = bb;
            if (β_out) *β_out = ω_in;
            return bb;
        }
    }
    if (e->c[0]->t == TT_TO || e->c[0]->t == TT_TO_BY) {
        IR_t *αo = NULL, *βo = NULL;
        gen = lower_new_ToBy_ag(bbg, e->c[0], NULL, NULL, &αo, &βo);
        gen_chain_entry = αo ? αo : gen;
    } else {
        gen = lower_expr_node(bbg, e->c[0]);
        gen_chain_entry = gen;
    }
    if (!gen) return NULL;
    IR_t *body = NULL;
    if (e->n >= 2 && e->c[1]) { body = lower_expr_node(bbg, e->c[1]); if (!body) return NULL; }
    IR_t *bb = IR_node_alloc(bbg, IR_EVERY);
    if (!bb) return NULL;
    bb->α = gen_chain_entry;
    bb->β = body;
    if (gen->α == NULL && gen->β == NULL) {
        if (body) { gen->γ = body; body->γ = gen; body->ω = gen; } else { gen->γ = gen; }
        gen->ω = bb;
        bb->ival = 1;
    } else if (lic_is_gen_node(gen) && body && body->t == IR_SEQ_EXPR) {
        gen->γ = bb; gen->ω = bb;
        body->γ = bb; body->ω = bb;
        bb->ival = 3;
    } else if (lic_is_gen_node(gen) && !(body && lic_body_bears_gen(body, 0))) {
        if (body) { gen->γ = body; body->γ = gen; body->ω = gen; } else { gen->γ = gen; }
        gen->ω = bb;
        bb->ival = 2;
    }
    if (!bb->γ && γ_in) bb->γ = γ_in;
    if (!bb->ω && ω_in) bb->ω = ω_in;
    if (α_out) *α_out = bb;
    if (β_out) *β_out = ω_in;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Compound(IR_graph_t *bbg, struct tree_t *e) {
    (void)bbg; (void)e;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_ProcBody(IR_graph_t *bbg, struct tree_t *e) {
    (void)bbg; (void)e;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Call(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 1 || !e->c[0] || e->c[0]->t != TT_VAR || !e->c[0]->v.sval) return NULL;
    int nargs = e->n - 1;
    if (nargs == 1 && strcmp(e->c[0]->v.sval, "key") == 0 && e->c[1]) {
        IR_t *tbl = lower_expr_node(bbg, e->c[1]);
        if (!tbl) return NULL;
        IR_t *bb = IR_node_alloc(bbg, IR_KEY_GEN);
        if (!bb) return NULL;
        bb->α = tbl;
        return bb;
    }
    if ((nargs == 2 || nargs == 3 || nargs == 4)
        && strcmp(e->c[0]->v.sval, "find") == 0
        && e->c[1] && e->c[2]) {
        IR_t **args2 = calloc((size_t)nargs, sizeof(IR_t *));
        if (!args2) return NULL;
        for (int j = 0; j < nargs; j++) {
            args2[j] = lower_expr_node(bbg, e->c[1+j]);
            if (!args2[j]) { free(args2); return NULL; }
        }
        IR_t *bb = IR_node_alloc(bbg, IR_FIND_GEN);
        if (!bb) { free(args2); return NULL; }
        if (nargs >= 1) bb->α = args2[0];
        if (nargs >= 2) bb->β = args2[1];
        if (nargs >= 3) bb->γ = args2[2];
        if (nargs >= 4) bb->ω = args2[3];
        free(args2);
        bb->ival = nargs;
        return bb;
    }
    if ((nargs == 0 || nargs == 1 || nargs == 2)
        && strcmp(e->c[0]->v.sval, "seq") == 0) {
        IR_t **args2 = NULL;
        if (nargs > 0) {
            args2 = calloc((size_t)nargs, sizeof(IR_t *));
            if (!args2) return NULL;
            for (int j = 0; j < nargs; j++) {
                args2[j] = lower_expr_node(bbg, e->c[1+j]);
                if (!args2[j]) { free(args2); return NULL; }
            }
        }
        IR_t *bb = IR_node_alloc(bbg, IR_SEQ_GEN);
        if (!bb) { free(args2); return NULL; }
        if (args2 && nargs >= 1) bb->α = args2[0];
        if (args2 && nargs >= 2) bb->β = args2[1];
        if (args2) free(args2);
        bb->ival = nargs;
        return bb;
    }
    IR_t **args = NULL;
    if (nargs > 0) {
        args = calloc((size_t)nargs, sizeof(IR_t *));
        if (!args) return NULL;
        for (int j = 0; j < nargs; j++) {
            args[j] = lower_expr_node(bbg, e->c[1+j]);
            if (!args[j]) { free(args); return NULL; }
        }
    }
    IR_t *bb = IR_node_alloc(bbg, IR_CALL);
    if (!bb) { if (args) free(args); return NULL; }
    bb->sval = e->c[0]->v.sval;
    bb->ival = nargs;
    if (args && nargs >= 1) {
        bb->α = args[0];
        for (int j = 1; j < nargs; j++) args[j-1]->γ = args[j];
    }
    if (args) free(args);
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Field(IR_graph_t *bbg, struct tree_t *e) {
    const char *fname = FIELD_NAME(e);
    if (!fname || e->n < 1 || !e->c[0]) return NULL;
    IR_t *obj = lower_expr_node(bbg, e->c[0]);
    if (!obj) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_FIELD_GET);
    if (!bb) return NULL;
    bb->sval = fname;
    bb->α = obj;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Sectionop(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 3 || !e->c[0] || !e->c[1] || !e->c[2]) return NULL;
    IR_t *base = lower_expr_node(bbg, e->c[0]);
    if (!base) return NULL;
    IR_t *i1 = lower_expr_node(bbg, e->c[1]);
    if (!i1) return NULL;
    IR_t *i2 = lower_expr_node(bbg, e->c[2]);
    if (!i2) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_SECTION);
    if (!bb) return NULL;
    bb->α = base;
    bb->β = i1;
    i1->γ = i2;
    bb->ival = (e->t == TT_SECTION) ? 0 : (e->t == TT_SECTION_PLUS) ? 1 : 2;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Alt(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 1) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_ALT);
    if (!bb) return NULL;
    IR_t *prev = NULL;
    for (int j = 0; j < e->n; j++) {
        if (!e->c[j]) return NULL;
        IR_t *arm = lower_expr_node(bbg, e->c[j]);
        if (!arm) return NULL;
        arm->γ = bb;
        if (j == 0) bb->α = arm;
        else prev->ω = arm;
        prev = arm;
    }
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Alt_ag(IR_graph_t *bbg, struct tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out) {
    if (e->n < 1) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_ALT);
    if (!bb) return NULL;
    IR_t *arm_apply[64];
    IR_t *arm_entry[64];
    int n_arms = e->n;
    if (n_arms > 64) return NULL;
    for (int j = 0; j < n_arms; j++) {
        if (!e->c[j]) return NULL;
        IR_t *αo = NULL, *βo = NULL;
        IR_t *arm = lower_expr_threaded_b(bbg, e->c[j], bb, ω_in, &αo, &βo, 0);
        if (!arm) return NULL;
        if (!arm->γ) arm->γ = bb;
        arm_apply[j] = arm;
        arm_entry[j] = αo ? αo : arm;
    }
    for (int j = 0; j < n_arms; j++) {
        if (j + 1 < n_arms) { if (!arm_apply[j]->ω) arm_apply[j]->ω = arm_entry[j + 1]; }
        else                { if (!arm_apply[j]->ω) arm_apply[j]->ω = ω_in; }
    }
    if (!bb->γ && γ_in) bb->γ = γ_in;
    if (!bb->ω && ω_in) bb->ω = ω_in;
    if (α_out) *α_out = arm_entry[0];
    if (β_out) *β_out = ω_in;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Conjunction(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    IR_t *e1 = lower_expr_node(bbg, e->c[0]);
    if (!e1) return NULL;
    IR_t *e2 = lower_expr_node(bbg, e->c[1]);
    if (!e2) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_CONJ);
    if (!bb) return NULL;
    bb->α = e1;
    bb->β = e2;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Conjunction_ag(IR_graph_t *bbg, struct tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_CONJ);
    if (!bb) return NULL;
    IR_t *l_αo = NULL, *l_βo = NULL;
    IR_t *left = lower_expr_threaded_b(bbg, e->c[0], NULL, ω_in, &l_αo, &l_βo, 0);
    if (!left) return NULL;
    IR_t *left_entry = l_αo ? l_αo : left;
    IR_t *r_αo = NULL, *r_βo = NULL;
    IR_t *right = lower_expr_threaded_b(bbg, e->c[1], bb, ω_in, &r_αo, &r_βo, 0);
    if (!right) return NULL;
    IR_t *right_entry = r_αo ? r_αo : right;
    if (!left->γ)  left->γ  = right_entry;
    if (!left->ω)  left->ω  = ω_in;
    if (!right->γ) right->γ = bb;
    if (!right->ω) right->ω = ω_in;
    if (!bb->γ && γ_in) bb->γ = γ_in;
    if (!bb->ω && ω_in) bb->ω = ω_in;
    if (α_out) *α_out = left_entry;
    if (β_out) *β_out = ω_in;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Not(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 1 || !e->c[0]) return NULL;
    IR_t *inner = lower_expr_node(bbg, e->c[0]);
    if (!inner) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_NOT);
    if (!bb) return NULL;
    bb->α = inner;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_While(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 1 || !e->c[0]) return NULL;
    IR_t *cond = lower_expr_node(bbg, e->c[0]);
    if (!cond) return NULL;
    IR_t *body = NULL;
    if (e->n >= 2 && e->c[1]) {
        body = lower_expr_node(bbg, e->c[1]);
        if (!body) return NULL;
    }
    IR_t *bb = IR_node_alloc(bbg, IR_WHILE);
    if (!bb) return NULL;
    bb->α = cond;
    bb->β = body;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Until(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 1 || !e->c[0]) return NULL;
    IR_t *cond = lower_expr_node(bbg, e->c[0]);
    if (!cond) return NULL;
    IR_t *body = NULL;
    if (e->n >= 2 && e->c[1]) {
        body = lower_expr_node(bbg, e->c[1]);
        if (!body) return NULL;
    }
    IR_t *bb = IR_node_alloc(bbg, IR_UNTIL);
    if (!bb) return NULL;
    bb->α = cond;
    bb->β = body;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Repeat(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 1 || !e->c[0]) return NULL;
    IR_t *body = lower_expr_node(bbg, e->c[0]);
    if (!body) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_REPEAT);
    if (!bb) return NULL;
    bb->α = body;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Limitation(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    IR_t *gen = lower_expr_node(bbg, e->c[0]);
    if (!gen) return NULL;
    IR_t *lim = lower_expr_node(bbg, e->c[1]);
    if (!lim) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_LIMIT);
    if (!bb) return NULL;
    bb->α = gen;
    bb->β = lim;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Scan(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 1 || !e->c[0]) return NULL;
    IR_t *subj = lower_expr_node(bbg, e->c[0]);
    if (!subj) return NULL;
    IR_t *body = NULL;
    if (e->n >= 2 && e->c[1]) {
        body = lower_expr_node(bbg, e->c[1]);
        if (!body) return NULL;
    }
    IR_t *bb = IR_node_alloc(bbg, IR_GEN_SCAN);
    if (!bb) return NULL;
    bb->α = subj;
    if (body) bb->β = body;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Case(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 1 || !e->c[0]) return NULL;
    IR_t **children = calloc((size_t)e->n, sizeof(IR_t *));
    if (!children) return NULL;
    for (int j = 0; j < e->n; j++) {
        if (!e->c[j]) { free(children); return NULL; }
        children[j] = lower_expr_node(bbg, e->c[j]);
        if (!children[j]) { free(children); return NULL; }
    }
    IR_t *bb = IR_node_alloc(bbg, IR_CASE);
    if (!bb) { free(children); return NULL; }
    if (e->n > 0) bb->α = children[0];
    for (int j = 1; j < e->n; j++) children[j-1]->γ = children[j];
    free(children);
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Return(IR_graph_t *bbg, struct tree_t *e) {
    IR_t *bb = IR_node_alloc(bbg, IR_RETURN);
    if (!bb) return NULL;
    if (e->n >= 1 && e->c[0]) {
        IR_t *retval = lower_expr_node(bbg, e->c[0]);
        if (!retval) return NULL;
        bb->α = retval;
    }
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Suspend(IR_graph_t *bbg, struct tree_t *e) {
    if (bbg->lang != IR_LANG_RKU) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_SUSPEND);
    if (!bb) return NULL;
    if (e->n >= 1 && e->c[0]) {
        IR_t *val = lower_expr_node(bbg, e->c[0]);
        if (!val) return NULL;
        bb->α = val;
    }
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Break(IR_graph_t *bbg, struct tree_t *e) {
    (void)e;
    return IR_node_alloc(bbg, IR_BREAK);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Next(IR_graph_t *bbg, struct tree_t *e) {
    (void)e;
    return IR_node_alloc(bbg, IR_NEXT);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Initial(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 1 || !e->c[0]) {
        IR_t *bb = IR_node_alloc(bbg, IR_SUCCEED);
        return bb;
    }
    IR_t *body = lower_expr_node(bbg, e->c[0]);
    if (!body) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_INITIAL);
    if (!bb) return NULL;
    bb->α = body;
    bb->ival = 0;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Unop(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 1 || !e->c[0]) {
        if (e->t == TT_NULL) return IR_node_alloc(bbg, IR_LIT_NUL);
        return NULL;
    }
    IR_t *inner = lower_expr_node(bbg, e->c[0]);
    if (!inner) return NULL;
    IR_e kind;
    switch (e->t) {
    case TT_MNS:         kind = IR_NEG;         break;
    case TT_PLS:         kind = IR_POS;         break;
    case TT_SIZE:        kind = IR_SIZE;        break;
    case TT_NULL:        kind = IR_NULL_TEST;   break;
    case TT_NONNULL:     kind = IR_NONNULL;     break;
    case TT_RANDOM:      kind = IR_RANDOM;      break;
    case TT_CSET_COMPL:  kind = IR_CSET_COMPL;  break;
    case TT_ITERATE:     kind = IR_LIST_BANG;   break;
    case TT_MATCH_UNARY: {
        IR_t *bb = IR_node_alloc(bbg, IR_CALL);
        if (!bb) return NULL;
        bb->sval = "match";
        bb->α = inner;
        return bb;
    }
    default: return NULL;
    }
    IR_t *bb = IR_node_alloc(bbg, kind);
    if (!bb) return NULL;
    bb->α = inner;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_AugOp(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    IR_t *lhs = lower_expr_node(bbg, e->c[0]);
    if (!lhs) return NULL;
    IR_t *rhs = lower_expr_node(bbg, e->c[1]);
    if (!rhs) return NULL;
    BinopKind op = BINOP_ADD;
    switch ((AugOp_e)e->v.ival) {
    case AUGOP_ADD:    op = BINOP_ADD;    break;
    case AUGOP_SUB:    op = BINOP_SUB;    break;
    case AUGOP_MUL:    op = BINOP_MUL;    break;
    case AUGOP_DIV:    op = BINOP_DIV;    break;
    case AUGOP_MOD:    op = BINOP_MOD;    break;
    case AUGOP_POW:    op = BINOP_POW;    break;
    case AUGOP_CONCAT: op = BINOP_CONCAT; break;
    case AUGOP_EQ:     op = BINOP_EQ;     break;
    case AUGOP_LT:     op = BINOP_LT;     break;
    case AUGOP_LE:     op = BINOP_LE;     break;
    case AUGOP_GT:     op = BINOP_GT;     break;
    case AUGOP_GE:     op = BINOP_GE;     break;
    case AUGOP_NE:     op = BINOP_NE;     break;
    default:           return NULL;
    }
    IR_t *binop = IR_node_alloc(bbg, IR_BINOP);
    if (!binop) return NULL;
    binop->α = lhs;
    binop->β = rhs;
    binop->ival = (int64_t)op;
    IR_t *asgn = IR_node_alloc(bbg, IR_ASSIGN);
    if (!asgn) return NULL;
    IR_t *lhs2 = lower_expr_node(bbg, e->c[0]);
    if (!lhs2) return NULL;
    asgn->α = lhs2;
    asgn->β = binop;
    return asgn;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Create(IR_graph_t *bbg, struct tree_t *e) {
    (void)bbg; (void)e;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Mutual(IR_graph_t *bbg, struct tree_t *e) {
    (void)bbg; (void)e;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Key(IR_graph_t *bbg, struct tree_t *e) {
    (void)bbg; (void)e;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Invocable(IR_graph_t *bbg, struct tree_t *e) {
    (void)bbg; (void)e;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Link(IR_graph_t *bbg, struct tree_t *e) {
    (void)bbg; (void)e;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_RepAlt(IR_graph_t *bbg, struct tree_t *e) {
    (void)bbg; (void)e;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_CoexpList(IR_graph_t *bbg, struct tree_t *e) {
    (void)bbg; (void)e;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Var(IR_graph_t *bbg, struct tree_t *e) {
    if (!e->v.sval) return NULL;
    if (e->v.sval[0] == '&') {
        IR_t *bb = IR_node_alloc(bbg, IR_KEYWORD);
        if (!bb) return NULL;
        bb->sval = e->v.sval;
        return bb;
    }
    IR_t *bb = IR_node_alloc(bbg, IR_VAR);
    if (!bb) return NULL;
    bb->sval = e->v.sval;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Keyword(IR_graph_t *bbg, struct tree_t *e) {
    if (!e->v.sval) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_KEYWORD);
    if (!bb) return NULL;
    bb->sval = e->v.sval;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Assign(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    if (e->c[0]->t == TT_FIELD) {
        const char *fname = FIELD_NAME(e->c[0]);
        if (!fname) return NULL;
        IR_t *obj = lower_expr_node(bbg, e->c[0]->c[0]);
        if (!obj) return NULL;
        IR_t *rhs = lower_expr_node(bbg, e->c[1]);
        if (!rhs) return NULL;
        IR_t *bb = IR_node_alloc(bbg, IR_FIELD_SET);
        if (!bb) return NULL;
        bb->sval = fname;
        bb->α = obj;
        bb->β = rhs;
        return bb;
    }
    if (e->c[0]->t == TT_IDX) {
        if (e->c[0]->n < 2 || !e->c[0]->c[0] || !e->c[0]->c[1]) return NULL;
        IR_t *base = lower_expr_node(bbg, e->c[0]->c[0]);
        if (!base) return NULL;
        IR_t *idx  = lower_expr_node(bbg, e->c[0]->c[1]);
        if (!idx) return NULL;
        IR_t *rhs  = lower_expr_node(bbg, e->c[1]);
        if (!rhs) return NULL;
        IR_t *bb = IR_node_alloc(bbg, IR_IDX_SET);
        if (!bb) return NULL;
        bb->α = base;
        bb->β = idx;
        idx->γ = rhs;
        return bb;
    }
    if (e->c[0]->t != TT_VAR) return NULL;
    if (e->c[0]->v.sval && e->c[0]->v.sval[0] == '&') return NULL;
    IR_t *lhs = lower_expr_node(bbg, e->c[0]);
    if (!lhs) return NULL;
    IR_t *rhs = lower_expr_node(bbg, e->c[1]);
    if (!rhs) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_ASSIGN);
    if (!bb) return NULL;
    bb->α = lhs;
    bb->β = rhs;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Swap(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    if (e->c[0]->t != TT_VAR || e->c[1]->t != TT_VAR) return NULL;
    if (!e->c[0]->v.sval || !e->c[1]->v.sval) return NULL;
    if (e->c[0]->v.sval[0] == '&' || e->c[1]->v.sval[0] == '&') return NULL;
    IR_t *lhs = lower_expr_node(bbg, e->c[0]);
    if (!lhs) return NULL;
    IR_t *rhs = lower_expr_node(bbg, e->c[1]);
    if (!rhs) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_SWAP);
    if (!bb) return NULL;
    bb->α = lhs;
    bb->β = rhs;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_SeqExpr(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 1) return NULL;
    IR_t **stmts = calloc((size_t)e->n, sizeof(IR_t *));
    if (!stmts) return NULL;
    for (int j = 0; j < e->n; j++) {
        if (!e->c[j]) { free(stmts); return NULL; }
        stmts[j] = lower_expr_node(bbg, e->c[j]);
        if (!stmts[j]) { free(stmts); return NULL; }
    }
    IR_t *bb = IR_node_alloc(bbg, IR_SEQ_EXPR);
    if (!bb) { free(stmts); return NULL; }
    if (e->n > 0) bb->α = stmts[0];
    for (int j = 1; j < e->n; j++) stmts[j-1]->γ = stmts[j];
    bb->ival = e->n;
    free(stmts);
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Decl(IR_graph_t *bbg, struct tree_t *e) {
    (void)e;
    return IR_node_alloc(bbg, IR_SUCCEED);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Idx(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    IR_t *base = lower_expr_node(bbg, e->c[0]);
    if (!base) return NULL;
    IR_t *idx  = lower_expr_node(bbg, e->c[1]);
    if (!idx) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_IDX);
    if (!bb) return NULL;
    bb->α = base;
    bb->β = idx;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Identical(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    IR_t *lhs = lower_expr_node(bbg, e->c[0]);
    if (!lhs) return NULL;
    IR_t *rhs = lower_expr_node(bbg, e->c[1]);
    if (!rhs) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_IDENTICAL);
    if (!bb) return NULL;
    bb->α = lhs;
    bb->β = rhs;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_ProcFail(IR_graph_t *bbg, struct tree_t *e) {
    (void)e;
    return IR_node_alloc(bbg, IR_FAIL);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_CsetBinop(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    IR_t *lhs = lower_expr_node(bbg, e->c[0]);
    if (!lhs) return NULL;
    IR_t *rhs = lower_expr_node(bbg, e->c[1]);
    if (!rhs) return NULL;
    IR_e kind = (e->t == TT_CSET_UNION) ? IR_CSET_UNION
                 : (e->t == TT_CSET_DIFF)  ? IR_CSET_DIFF
                                           : IR_CSET_INTER;
    IR_t *bb = IR_node_alloc(bbg, kind);
    if (!bb) return NULL;
    bb->α = lhs;
    bb->β = rhs;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Record(IR_graph_t *bbg, struct tree_t *e) {
    if (!e->v.sval) return NULL;
    char spec[512]; int pos = 0;
    pos += snprintf(spec+pos, sizeof(spec)-pos, "%s(", e->v.sval);
    for (int i = 0; i < e->n && pos < (int)sizeof(spec)-2; i++) {
        if (i > 0) spec[pos++] = ',';
        const char *fn2 = (e->c[i] && e->c[i]->v.sval) ? e->c[i]->v.sval : "";
        pos += snprintf(spec+pos, sizeof(spec)-pos, "%s", fn2);
    }
    if (pos < (int)sizeof(spec)-1) spec[pos++] = ')';
    spec[pos] = '\0';
    IR_t *bb = IR_node_alloc(bbg, IR_RECORD_DEF);
    if (!bb) return NULL;
    bb->sval = GC_strdup(spec);
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Makelist(IR_graph_t *bbg, struct tree_t *e) {
    int n = e->n;
    IR_t **args = (n > 0) ? calloc((size_t)n, sizeof(IR_t *)) : NULL;
    for (int j = 0; j < n; j++) {
        if (!e->c[j]) { if (args) free(args); return NULL; }
        args[j] = lower_expr_node(bbg, e->c[j]);
        if (!args[j]) { free(args); return NULL; }
    }
    IR_t *bb = IR_node_alloc(bbg, IR_CALL);
    if (!bb) { if (args) free(args); return NULL; }
    bb->sval = "MAKELIST";
    bb->ival = n;
    if (args && n >= 1) {
        bb->α = args[0];
        for (int j = 0; j + 1 < n; j++) args[j]->γ = args[j + 1];
    }
    if (args) free(args);
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern int is_suspendable(tree_t *e);
DESCR_t binop_apply(BinopKind op, DESCR_t lv, DESCR_t rv, int *rel_fail) {
    *rel_fail = 0;
    if (IS_FAIL_fn(lv) || IS_FAIL_fn(rv)) return FAILDESCR;
    int either_real = (IS_REAL_fn(lv) || IS_REAL_fn(rv));
    double ld = IS_REAL_fn(lv) ? lv.r : (double)(IS_INT_fn(lv) ? lv.i : 0);
    double rd = IS_REAL_fn(rv) ? rv.r : (double)(IS_INT_fn(rv) ? rv.i : 0);
    long   li = IS_INT_fn(lv) ? lv.i : (long)lv.r;
    long   ri = IS_INT_fn(rv) ? rv.i : (long)rv.r;
    DESCR_t real_result;
    switch (op) {
        case BINOP_ADD: if (either_real) { real_result.v=DT_R; real_result.r=ld+rd; return real_result; } return INTVAL(li + ri);
        case BINOP_SUB: if (either_real) { real_result.v=DT_R; real_result.r=ld-rd; return real_result; } return INTVAL(li - ri);
        case BINOP_MUL: if (either_real) { real_result.v=DT_R; real_result.r=ld*rd; return real_result; } return INTVAL(li * ri);
        case BINOP_DIV: if (either_real) { if (rd == 0.0) return FAILDESCR; real_result.v=DT_R; real_result.r=ld/rd; return real_result; } return ri ? INTVAL(li / ri) : FAILDESCR;
        case BINOP_MOD: return ri ? INTVAL(li % ri) : FAILDESCR;
        case BINOP_POW: { double base = either_real ? ld : (double)li; double exp2 = either_real ? rd : (double)ri; double r = pow(base, exp2); real_result.v = DT_R; real_result.r = r; return real_result; }
        case BINOP_LT: *rel_fail = !(either_real ? ld <  rd : li <  ri); return *rel_fail ? FAILDESCR : rv;
        case BINOP_LE: *rel_fail = !(either_real ? ld <= rd : li <= ri); return *rel_fail ? FAILDESCR : rv;
        case BINOP_GT: *rel_fail = !(either_real ? ld >  rd : li >  ri); return *rel_fail ? FAILDESCR : rv;
        case BINOP_GE: *rel_fail = !(either_real ? ld >= rd : li >= ri); return *rel_fail ? FAILDESCR : rv;
        case BINOP_EQ: *rel_fail = !(either_real ? ld == rd : li == ri); return *rel_fail ? FAILDESCR : rv;
        case BINOP_NE: *rel_fail = !(either_real ? ld != rd : li != ri); return *rel_fail ? FAILDESCR : rv;
        case BINOP_CONCAT: {
            DESCR_t ls_d; ls_d = descr_to_str_icn(lv);
            DESCR_t rs_d; rs_d = descr_to_str_icn(rv);
            if (IS_FAIL_fn(ls_d) || IS_FAIL_fn(rs_d)) return FAILDESCR;
            const char *ls = ls_d.s ? ls_d.s : "";
            const char *rs = rs_d.s ? rs_d.s : "";
            size_t ll = ls_d.slen > 0 ? (size_t)ls_d.slen : strlen(ls);
            size_t rl = rs_d.slen > 0 ? (size_t)rs_d.slen : strlen(rs);
            char *buf = GC_malloc(ll + rl + 1);
            memcpy(buf, ls, ll); memcpy(buf + ll, rs, rl); buf[ll + rl] = '\0';
            { DESCR_t r2; r2.v = DT_S; r2.slen = (int)(ll + rl); r2.s = buf; return r2; }
        }
        case BINOP_SLT: case BINOP_SLE: case BINOP_SGT:
        case BINOP_SGE: case BINOP_SEQ: case BINOP_SNE: {
            DESCR_t ls_d = descr_to_str_icn(lv);
            DESCR_t rs_d = descr_to_str_icn(rv);
            const char *ls = (!IS_FAIL_fn(ls_d) && ls_d.s) ? ls_d.s : "";
            const char *rs = (!IS_FAIL_fn(rs_d) && rs_d.s) ? rs_d.s : "";
            int cmp = strcmp(ls, rs);
            int ok;
            switch (op) {
            case BINOP_SLT: ok = (cmp <  0); break;
            case BINOP_SLE: ok = (cmp <= 0); break;
            case BINOP_SGT: ok = (cmp >  0); break;
            case BINOP_SGE: ok = (cmp >= 0); break;
            case BINOP_SEQ: ok = (cmp == 0); break;
            case BINOP_SNE: ok = (cmp != 0); break;
            default:            ok = 0;           break;
            }
            *rel_fail = !ok;
            return ok ? rv : FAILDESCR;
        }
        default: return FAILDESCR;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int fold_signed_lit(tree_t *n, int64_t *iv, double *dv, int *is_real) {
    if (!n) return 0;
    double sign = 1.0;
    while (n && (n->t == TT_MNS || n->t == TT_PLS)) {
        if (n->t == TT_MNS) sign = -sign;
        if (n->n < 1 || !n->c[0]) return 0;
        n = n->c[0];
    }
    if (!n) return 0;
    if (n->t == TT_ILIT) {
        if (sign < 0.0) { *iv = -n->v.ival; } else { *iv = n->v.ival; }
        *is_real = 0;
        return 1;
    }
    if (n->t == TT_FLIT) {
        *dv = sign * n->v.dval;
        *is_real = 1;
        return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t *lower_upto(const char *cset, const char *hay) {
    if (!cset || !hay) return NULL;
    IR_graph_t *bbg = IR_alloc(4, IR_LANG_ICN);
    if (!bbg) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_UPTO);
    if (!bb) return NULL;
    bb->sval    = cset;
    bb->counter = 0;
    bb->α = bb;
    bb->β = bb;
    bb->γ = NULL;
    bb->ω = NULL;
    bbg->entry = bb;
    return bbg;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t *lower_proc_gen(GeneratorState *gs) {
    if (!gs) return NULL;
    IR_graph_t *bbg = IR_alloc(4, IR_LANG_ICN);
    if (!bbg) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_PROC_GEN);
    if (!bb) return NULL;
    bb->counter = (int64_t)(uintptr_t)gs;
    bb->α      = bb;
    bb->β      = bb;
    bb->γ      = NULL;
    bb->ω      = NULL;
    bbg->entry = bb;
    return bbg;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_expr_node(IR_graph_t *bbg, tree_t *e) {
    if (!bbg || !e) return NULL;
    switch (e->t) {
    case TT_ILIT:           return lower_new_Intlit(bbg, e);
    case TT_FLIT:           return lower_new_Reallit(bbg, e);
    case TT_QLIT:           return lower_new_Stringlit(bbg, e);
    case TT_CSET:           return lower_new_Csetlit(bbg, e);
    case TT_VAR:            return lower_new_Var(bbg, e);
    case TT_KEYWORD:        return lower_new_Keyword(bbg, e);
    case TT_SCAN:           return lower_new_Scan(bbg, e);
    case TT_ASSIGN:         return lower_new_Assign(bbg, e);
    case TT_SWAP:           return lower_new_Swap(bbg, e);
    case TT_FNC:            return lower_new_Call(bbg, e);
    case TT_IF:             return lower_new_If(bbg, e);
    case TT_TO:             return lower_new_ToBy(bbg, e);
    case TT_TO_BY:          return lower_new_ToBy(bbg, e);
    case TT_EVERY:          return lower_new_Every(bbg, e);
    case TT_WHILE:          return lower_new_While(bbg, e);
    case TT_UNTIL:          return lower_new_Until(bbg, e);
    case TT_SEQ_EXPR:       return lower_new_SeqExpr(bbg, e);
    case TT_ADD: case TT_SUB: case TT_MUL: case TT_DIV: case TT_MOD: case TT_POW:
    case TT_LT:  case TT_LE:  case TT_GT:  case TT_GE:  case TT_EQ:  case TT_NE:
    case TT_CAT: case TT_LCONCAT:
    case TT_LLT: case TT_LLE: case TT_LGT: case TT_LGE: case TT_LEQ: case TT_LNE:
                            return lower_new_Binop(bbg, e);
    case TT_GLOBAL:         return lower_new_Global(bbg, e);
    case TT_LOCAL:          return lower_new_Decl(bbg, e);
    case TT_STATIC_DECL:    return lower_new_Decl(bbg, e);
    case TT_INITIAL:        return lower_new_Initial(bbg, e);
    case TT_RETURN:         return lower_new_Return(bbg, e);
    case TT_SUSPEND:        return lower_new_Suspend(bbg, e);
    case TT_SEQ:            return lower_new_Conjunction(bbg, e);
    case TT_SIZE:           return lower_new_Unop(bbg, e);
    case TT_IDX:            return lower_new_Idx(bbg, e);
    case TT_SECTION:        return lower_new_Sectionop(bbg, e);
    case TT_SECTION_PLUS:   return lower_new_Sectionop(bbg, e);
    case TT_SECTION_MINUS:  return lower_new_Sectionop(bbg, e);
    case TT_CASE:           return lower_new_Case(bbg, e);
    case TT_NOT:            return lower_new_Not(bbg, e);
    case TT_REPEAT:         return lower_new_Repeat(bbg, e);
    case TT_ALTERNATE:      return lower_new_Alt(bbg, e);
    case TT_LIMIT:          return lower_new_Limitation(bbg, e);
    case TT_AUGOP:          return lower_new_AugOp(bbg, e);
    case TT_LOOP_BREAK:     return lower_new_Break(bbg, e);
    case TT_LOOP_NEXT:      return lower_new_Next(bbg, e);
    case TT_PROC_FAIL:      return lower_new_ProcFail(bbg, e);
    case TT_IDENTICAL:      return lower_new_Identical(bbg, e);
    case TT_NONNULL:        return lower_new_Unop(bbg, e);
    case TT_NULL:           return lower_new_Unop(bbg, e);
    case TT_RANDOM:         return lower_new_Unop(bbg, e);
    case TT_MATCH_UNARY:    return lower_new_Unop(bbg, e);
    case TT_MNS:            return lower_new_Unop(bbg, e);
    case TT_PLS:            return lower_new_Unop(bbg, e);
    case TT_CSET_COMPL:     return lower_new_Unop(bbg, e);
    case TT_CSET_UNION:     return lower_new_CsetBinop(bbg, e);
    case TT_CSET_DIFF:      return lower_new_CsetBinop(bbg, e);
    case TT_CSET_INTER:     return lower_new_CsetBinop(bbg, e);
    case TT_FIELD:          return lower_new_Field(bbg, e);
    case TT_RECORD:         return lower_new_Record(bbg, e);
    case TT_MAKELIST:       return lower_new_Makelist(bbg, e);
    case TT_ITERATE:        return lower_new_Unop(bbg, e);
    default:                return NULL;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t *lower_proc_body(tree_t *proc) {
    if (!proc) return NULL;
    tree_t **body_arr = NULL;
    int body_off = 0;
    int n_stmts = 0;
    if (proc->t == TT_PROC_DECL) {
        tree_t *body_node = (proc->n >= 3) ? proc->c[2] : NULL;
        if (!body_node) return NULL;
        body_arr = body_node->c;
        n_stmts = body_node->n;
    } else if (proc->t == TT_SUB_DECL) {
        int nparams = (int)proc->v.ival;
        body_off = nparams + 1;
        if (proc->n <= body_off) return NULL;
        body_arr = proc->c;
        n_stmts = proc->n - body_off;
    } else {
        return NULL;
    }
    if (n_stmts <= 0 || !body_arr) return NULL;
    int _lang_tag = (proc->t == TT_SUB_DECL) ? IR_LANG_RKU : IR_LANG_ICN;
    IR_graph_t *bbg = IR_alloc(4096, _lang_tag);
    if (!bbg) return NULL;
    IR_t **stmt_nodes = calloc((size_t)n_stmts, sizeof(IR_t *));
    if (!stmt_nodes) { IR_free(bbg); return NULL; }
    IR_t **stmt_entries = calloc((size_t)n_stmts, sizeof(IR_t *));
    if (!stmt_entries) { free(stmt_nodes); IR_free(bbg); return NULL; }
    int built = 0;
    IR_t *fail_term = IR_node_alloc(bbg, IR_FAIL);
    if (!fail_term) { free(stmt_nodes); free(stmt_entries); IR_free(bbg); return NULL; }
    IR_t *succ = fail_term;
    for (int i = n_stmts - 1; i >= 0; i--) {
        tree_t *st = body_arr[body_off + i];
        if (!st) continue;
        IR_t *aα = NULL, *aβ = NULL;
        IR_t *bb = lower_expr_threaded_b(bbg, st, succ, succ, &aα, &aβ, 1);
        if (!bb) { free(stmt_nodes); free(stmt_entries); IR_free(bbg); return NULL; }
        stmt_nodes[i]   = bb;
        stmt_entries[i] = aα ? aα : bb;
        succ = stmt_entries[i];
        built++;
    }
    IR_t *seq = IR_node_alloc(bbg, IR_SEQ);
    if (!seq) { free(stmt_nodes); free(stmt_entries); IR_free(bbg); return NULL; }
    { IR_t *head = NULL; for (int i = 0; i < n_stmts; i++) if (stmt_entries[i]) { head = stmt_entries[i]; break; } seq->α = head; }
    seq->ival = built;
    {
        IR_t *prev = NULL;
        for (int i = 0; i < n_stmts; i++) {
            IR_t *cur_entry = stmt_entries[i];
            if (!cur_entry) continue;
            int prev_terminal_kind = prev && (prev->t == IR_RETURN || prev->t == IR_FAIL ||
                                              prev->t == IR_BREAK  || prev->t == IR_NEXT);
            if (prev && !prev->ω && !prev_terminal_kind) prev->ω = cur_entry;
            prev = stmt_nodes[i];
        }
        if (prev) {
            int prev_terminal = (prev->t == IR_RETURN || prev->t == IR_FAIL ||
                                 prev->t == IR_BREAK  || prev->t == IR_NEXT);
            if (!prev->γ && !prev_terminal) prev->γ = fail_term;
            if (!prev->ω && !prev_terminal) prev->ω = fail_term;
        }
    }
    free(stmt_nodes);
    free(stmt_entries);
    bbg->entry = seq;
    return bbg;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t *lower_expr_top(struct tree_t *e) {
    if (!e) return NULL;
    IR_graph_t *bbg = IR_alloc(256, IR_LANG_ICN);
    if (!bbg) return NULL;
    IR_t *bb = lower_expr_node(bbg, e);
    if (!bb) { IR_free(bbg); return NULL; }
    bbg->entry = bb;
    return bbg;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int kind_is_resumable(IR_e t) {
    return t == IR_TO || t == IR_TO_BY || t == IR_UPTO || t == IR_ALT ||
           t == IR_BINOP_GEN || t == IR_ITERATE || t == IR_LIMIT || t == IR_PROC_GEN ||
           t == IR_EVERY || t == IR_REPEAT || t == IR_SUSPEND || t == IR_SCAN ||
           t == IR_LIST_BANG || t == IR_KEY_GEN || t == IR_FIND_GEN || t == IR_SEQ_GEN ||
           t == IR_GEN_SCAN || t == IR_CONJ;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *gen_leaf(IR_t *bb, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out, int bounded) {
    if (!bb) return NULL;
    if (!bb->γ && γ_in) bb->γ = γ_in;
    if (!bb->ω && ω_in) bb->ω = ω_in;
    if (α_out) *α_out = bb;
    if (β_out) *β_out = (!bounded && kind_is_resumable(bb->t)) ? bb : ω_in;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int tree_is_leaf(const tree_t *e) {
    if (!e) return 0;
    switch (e->t) {
    case TT_ILIT: case TT_FLIT: case TT_QLIT: case TT_CSET:
    case TT_VAR:  case TT_KEYWORD:
    case TT_LOOP_BREAK: case TT_LOOP_NEXT: case TT_PROC_FAIL:
        return 1;
    default:
        return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_expr_threaded_b(IR_graph_t *bbg, tree_t *e,
                                 IR_t *γ_in, IR_t *ω_in,
                                 IR_t **α_out, IR_t **β_out, int bounded) {
    if (!bbg || !e) return NULL;
    int is_leaf = tree_is_leaf(e);
    if (e->t == TT_ADD || e->t == TT_SUB || e->t == TT_MUL || e->t == TT_DIV ||
        e->t == TT_MOD || e->t == TT_POW ||
        e->t == TT_LT  || e->t == TT_LE  || e->t == TT_GT  || e->t == TT_GE  ||
        e->t == TT_EQ  || e->t == TT_NE  || e->t == TT_CAT ||
        e->t == TT_LLT || e->t == TT_LLE || e->t == TT_LGT ||
        e->t == TT_LGE || e->t == TT_LEQ || e->t == TT_LNE) {
        if (e->n >= 2 && e->c[0] && e->c[1]) {
            IR_t *ag = lower_new_Binop_ag(bbg, e, γ_in, ω_in, α_out, β_out);
            if (ag) return ag;
        }
    }
    if (e->t == TT_LCONCAT && e->n >= 2 && e->c[0] && e->c[1]) {
        IR_t *ag = lower_new_Lconcat_ag(bbg, e, γ_in, ω_in, α_out, β_out);
        if (ag) return ag;
    }
    if ((e->t == TT_SECTION || e->t == TT_SECTION_PLUS || e->t == TT_SECTION_MINUS) &&
        e->n >= 3 && e->c[0] && e->c[1] && e->c[2]) {
        IR_t *ag = lower_new_Sectionop_ag(bbg, e, γ_in, ω_in, α_out, β_out);
        if (ag) return ag;
    }
    if (e->t == TT_IDX && e->n >= 2 && e->c[0] && e->c[1]) {
        IR_t *ag = lower_new_Idx_ag(bbg, e, γ_in, ω_in, α_out, β_out);
        if (ag) return ag;
    }
    if (e->t == TT_ASSIGN && e->n >= 2 && e->c[0] && e->c[1] && e->c[0]->t == TT_IDX) {
        IR_t *ag = lower_new_Idx_set_ag(bbg, e, γ_in, ω_in, α_out, β_out);
        if (ag) return ag;
    }
    if (e->t == TT_IF && e->n >= 1 && e->c[0]) {
        IR_t *ag = lower_new_If_ag(bbg, e, γ_in, ω_in, α_out, β_out);
        if (ag) return ag;
    }
    if (e->t == TT_SEQ && e->n >= 2 && e->c[0] && e->c[1]) {
        IR_t *ag = lower_new_Conjunction_ag(bbg, e, γ_in, ω_in, α_out, β_out);
        if (ag) return ag;
    }
    if (e->t == TT_ALTERNATE && e->n >= 1) {
        IR_t *ag = lower_new_Alt_ag(bbg, e, γ_in, ω_in, α_out, β_out);
        if (ag) return ag;
    }
    if (e->t == TT_EVERY && e->n >= 1 && e->c[0]) {
        IR_t *ag = lower_new_Every_ag(bbg, e, γ_in, ω_in, α_out, β_out);
        if (ag) return ag;
    }
    if ((e->t == TT_TO || e->t == TT_TO_BY) && e->n >= 2 && e->c[0] && e->c[1]) {
        IR_t *ag = lower_new_ToBy_ag(bbg, e, γ_in, ω_in, α_out, β_out);
        if (ag) return ag;
    }
    IR_t *bb = lower_expr_node(bbg, e);
    if (!bb) return NULL;
    if (e->t == TT_ASSIGN && e->n >= 2 && e->c[0] && e->c[1] && e->c[0]->t == TT_VAR &&
        e->c[0]->v.sval && e->c[0]->v.sval[0] != '&' &&
        bb->t == IR_ASSIGN && bb->α && bb->β) {
        IR_t *rhs = bb->β;
        if (!rhs->γ) rhs->γ = bb;
        if (!rhs->ω) rhs->ω = ω_in;
        bb->ival = 1;
        if (!bb->γ && γ_in) bb->γ = γ_in;
        if (!bb->ω && ω_in) bb->ω = ω_in;
        if (α_out) *α_out = rhs;
        if (β_out) *β_out = ω_in;
        return bb;
    }
    if (e->t == TT_FNC && e->n >= 1 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval &&
        bb->t == IR_CALL && (int)bb->ival >= 0) {
        int nargs = e->n - 1;
        int any_gen = 0;
        for (int j = 0; j < nargs; j++) {
            if (e->c[1 + j] && is_suspendable(e->c[1 + j])) { any_gen = 1; break; }
        }
        if (!any_gen) {
            if (nargs > 0 && bb->α) {
                IR_t *ax = bb->α;
                for (int j = 0; j < nargs && ax; j++) {
                    if (!ax->ω) ax->ω = ω_in;
                    IR_t *next_ax = ax->γ;
                    if (j == nargs - 1) {
                        if (!ax->γ || ax->γ == bb) ax->γ = bb;
                        next_ax = NULL;
                    }
                    ax = next_ax;
                }
            }
            bb->dval = 1.0;
            if (!bb->γ && γ_in) bb->γ = γ_in;
            if (!bb->ω && ω_in) bb->ω = ω_in;
            if (α_out) *α_out = (nargs > 0 && bb->α) ? bb->α : bb;
            if (β_out) *β_out = ω_in;
            return bb;
        }
    }
    return gen_leaf(bb, γ_in, ω_in, α_out, β_out, is_leaf ? 1 : bounded);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_expr_threaded(IR_graph_t *bbg, tree_t *e,
                               IR_t *γ_in, IR_t *ω_in,
                               IR_t **α_out, IR_t **β_out) {
    return lower_expr_threaded_b(bbg, e, γ_in, ω_in, α_out, β_out, 0);
}
/*====================================================================================================================================================================================================*/
/* ===== PROLOG CLAUSE LOWERING (mirrors the Icon four-port model) ===== */
/*====================================================================================================================================================================================================*/
static IR_t *lower_pl_term(IR_graph_t *bbg, const tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out);
static IR_t *lower_pl_goal(IR_graph_t *bbg, const tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out);
static int   resolve_goal_is_bounded(const tree_t *e);
static IR_t *lower_pl_new_Alt(IR_graph_t *bbg, const tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out);
static IR_t *lower_pl_new_Ite(IR_graph_t *bbg, const tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out);
static IR_t *lower_pl_new_Unify(IR_graph_t *bbg, const tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out);
static IR_t *lower_pl_new_Compare(IR_graph_t *bbg, const tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out);
static IR_t *lower_pl_new_Conj(IR_graph_t *bbg, const tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out);
static IR_t *lower_pl_new_Call(IR_graph_t *bbg, const char *fn, const tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out);
static IR_t *lower_pl_new_Builtin(IR_graph_t *bbg, const char *fn, const tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out);
static int flatten_comma(const tree_t *e, const tree_t **out, int cap);
static IR_t *resolve_leaf(IR_t *bb, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out) {
    if (!bb) return NULL;
    bb->γ = γ_in; bb->ω = ω_in;
    if (α_out) *α_out = bb;
    if (β_out) *β_out = ω_in;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t *lower_pl_new_Alt(IR_graph_t *bbg, const tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out) {
    IR_t *bα = NULL, *bβ = NULL;
    IR_t *b = lower_pl_goal(bbg, e->c[1], γ_in, ω_in, &bα, &bβ); if (!b) return NULL;
    IR_t *aα = NULL, *aβ = NULL;
    IR_t *a = lower_pl_goal(bbg, e->c[0], γ_in, bα,  &aα, &aβ); if (!a) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_DISJ); if (!bb) return NULL;
    bb->α = aα; bb->β = bα;
    bb->γ = γ_in; bb->ω = ω_in;
    if (α_out) *α_out = aα; if (β_out) *β_out = bα;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t *lower_pl_new_Ite(IR_graph_t *bbg, const tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out) {
    const tree_t *cond = e->c[0];
    const tree_t *then_ = e->c[1];
    const tree_t *else_ = (e->n >= 3) ? e->c[2] : NULL;
    IR_t *bα = NULL, *bβ = NULL, *b = NULL;
    if (else_) { b = lower_pl_goal(bbg, else_, γ_in, ω_in, &bα, &bβ); if (!b) return NULL; }
    else       { b = IR_node_alloc(bbg, IR_FAIL); if (!b) return NULL; resolve_leaf(b, γ_in, ω_in, &bα, &bβ); }
    IR_t *tα = NULL, *tβ = NULL;
    IR_t *t = lower_pl_goal(bbg, then_, γ_in, ω_in, &tα, &tβ); if (!t) return NULL;
    IR_t *cα = NULL, *cβ = NULL;
    IR_t *c = lower_pl_goal(bbg, cond, tα, bα, &cα, &cβ); if (!c) return NULL;
    IR_t *ite = IR_node_alloc(bbg, IR_ITE); if (!ite) return NULL;
    ite->α = cα; ite->γ = γ_in; ite->ω = ω_in;
    {
        bb_ite_state_t *zi = (bb_ite_state_t *)GC_MALLOC_UNCOLLECTABLE(sizeof *zi);
        if (zi) { zi->cond = cα; zi->then_ = tα; zi->else_ = bα; ite->ival = (int64_t)(intptr_t)zi; }
    }
    if (α_out) *α_out = ite; if (β_out) *β_out = ω_in;
    return ite;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t *lower_pl_new_Unify(IR_graph_t *bbg, const tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out) {
    IR_t *lα = NULL, *lβ = NULL, *rα = NULL, *rβ = NULL;
    IR_t *lhs = lower_pl_term(bbg, e->c[0], γ_in, ω_in, &lα, &lβ); if (!lhs) return NULL;
    IR_t *rhs = lower_pl_term(bbg, e->c[1], γ_in, ω_in, &rα, &rβ); if (!rhs) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_UNIFY); if (!bb) return NULL;
    bb->α = lα; bb->β = rα; bb->γ = γ_in; bb->ω = ω_in;
    if (α_out) *α_out = bb; if (β_out) *β_out = bb;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t *lower_pl_new_Compare(IR_graph_t *bbg, const tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out) {
    if (e->n < 2) return NULL;
    IR_t *lα=NULL,*lβ=NULL,*rα=NULL,*rβ=NULL;
    IR_t *lhs = lower_pl_term(bbg,e->c[0],γ_in,ω_in,&lα,&lβ); if(!lhs) return NULL;
    IR_t *rhs = lower_pl_term(bbg,e->c[1],γ_in,ω_in,&rα,&rβ); if(!rhs) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_BUILTIN); if (!bb) return NULL;
    bb->sval = (e->t==TT_GT)?">":(e->t==TT_LT)?"<":(e->t==TT_GE)?">=":(e->t==TT_LE)?"<=":(e->t==TT_EQ)?"=:=":"=\=";
    bb->α = lα; bb->β = rα; bb->γ = γ_in; bb->ω = ω_in;
    if (α_out) *α_out = bb; if (β_out) *β_out = bb;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int resolve_node_is_resumable(const IR_t *nd) {
    if (!nd) return 0;
    return nd->t == IR_GOAL || nd->t == IR_CHOICE || nd->t == IR_DISJ;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t *lower_pl_new_Conj(IR_graph_t *bbg, const tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out) {
    const tree_t *flat[256];
    int n = flatten_comma(e, flat, 256);
    if (n <= 0) return NULL;
    if (n == 1) return lower_pl_goal(bbg, flat[0], γ_in, ω_in, α_out, β_out);
    IR_t **gα = (IR_t **)GC_MALLOC((size_t)n * sizeof(IR_t *));
    IR_t **gβ = (IR_t **)GC_MALLOC((size_t)n * sizeof(IR_t *));
    IR_t **gnodes = (IR_t **)GC_MALLOC((size_t)n * sizeof(IR_t *));
    {
        IR_t *bb = lower_pl_goal(bbg, flat[n-1], γ_in, NULL, &gα[n-1], &gβ[n-1]);
        if (!bb) return NULL; gnodes[n-1] = bb;
    }
    for (int i = n-2; i >= 0; i--) {
        IR_t *my_γ = gα[i+1];
        IR_t *my_ω = (i == 0) ? ω_in : NULL;
        IR_t *bb = lower_pl_goal(bbg, flat[i], my_γ, my_ω, &gα[i], &gβ[i]);
        if (!bb) return NULL; gnodes[i] = bb;
    }
    gβ[0] = resolve_node_is_resumable(gnodes[0]) ? gnodes[0] : ω_in;
    for (int i = 1; i < n; i++) {
        gβ[i] = resolve_node_is_resumable(gnodes[i]) ? gnodes[i] : gβ[i-1];
    }
    for (int i = 1; i < n; i++) if (!gnodes[i]->ω) gnodes[i]->ω = gβ[i-1];
    IR_t *seq = IR_node_alloc(bbg, IR_GCONJ); if (!seq) return NULL;
    seq->α = gα[0]; seq->γ = γ_in; seq->ω = ω_in;
    {
        bb_conj_state_t *zs = (bb_conj_state_t *)GC_MALLOC_UNCOLLECTABLE(sizeof *zs);
        if (zs) {
            zs->goals  = (IR_t **)GC_MALLOC_UNCOLLECTABLE((size_t)n * sizeof(IR_t *));
            zs->ngoals = n;
            for (int i = 0; i < n; i++) zs->goals[i] = gnodes[i];
            seq->ival = (int64_t)(intptr_t)zs;
        }
    }
    if (α_out) *α_out = seq;
    if (β_out) *β_out = seq;
    return seq;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t *lower_pl_new_Call(IR_graph_t *bbg, const char *fn, const tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out) {
    int n = e ? e->n : 0;
    IR_t *bb = IR_node_alloc(bbg, IR_GOAL); if (!bb) return NULL;
    bb_goal_state_t *zc = (bb_goal_state_t *)GC_MALLOC_UNCOLLECTABLE(sizeof *zc);
    zc->callee=fn; zc->arity=n; zc->cs=NULL;
    zc->args = (n>0) ? (IR_t **)GC_MALLOC_UNCOLLECTABLE((size_t)n*sizeof(IR_t *)) : NULL;
    zc->nargs=0;
    for (int i=0; i<n; i++) {
        IR_t *aα=NULL,*aβ=NULL;
        IR_t *a=lower_pl_term(bbg,e->c[i],γ_in,ω_in,&aα,&aβ); if(!a) return NULL;
        zc->args[zc->nargs++]=aα;
    }
    bb->sval=fn; bb->ival=(int64_t)(intptr_t)zc;
    if (n>=1) bb->α=zc->args[0];
    if (n>=2) bb->β=zc->args[1];
    bb->γ=γ_in; bb->ω=ω_in;
    if (α_out) *α_out=bb; if (β_out) *β_out=bb;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
enum { RESOLVE_BI_NONE=0, RESOLVE_BI_AB, RESOLVE_BI_CHAIN, RESOLVE_BI_CHAIN_ABOLISH, RESOLVE_BI_TYPETEST };
static int resolve_builtin_style(const char *fn, int n) {
    if (strcmp(fn,"write")==0||strcmp(fn,"writeln")==0||strcmp(fn,"is")==0
        ||strcmp(fn,">")==0||strcmp(fn,"<")==0||strcmp(fn,">=")==0||strcmp(fn,"<=")==0
        ||strcmp(fn,"=:=")==0||strcmp(fn,"=\\=")==0||(strcmp(fn,"succ")==0&&n==2)
        ||strcmp(fn,"==")==0||strcmp(fn,"\\==")==0
        ||strcmp(fn,"@<")==0||strcmp(fn,"@>")==0||strcmp(fn,"@=<")==0||strcmp(fn,"@>=")==0) return RESOLVE_BI_AB;
    if (n==1 && (strcmp(fn,"var")==0||strcmp(fn,"nonvar")==0||strcmp(fn,"atom")==0||strcmp(fn,"atomic")==0
        ||strcmp(fn,"number")==0||strcmp(fn,"integer")==0||strcmp(fn,"float")==0||strcmp(fn,"compound")==0
        ||strcmp(fn,"callable")==0||strcmp(fn,"is_list")==0||strcmp(fn,"ground")==0)) return RESOLVE_BI_TYPETEST;
    if ((strcmp(fn,"functor")==0 && n==3) || (strcmp(fn,"arg")==0 && n==3)
        || (strcmp(fn,"=..")==0 && n==2)
        || (strcmp(fn,"atom_length")==0 && n==2) || (strcmp(fn,"atom_concat")==0 && n==3)
        || (strcmp(fn,"atom_chars")==0 && n==2)  || (strcmp(fn,"atom_codes")==0 && n==2)
        || (strcmp(fn,"upcase_atom")==0 && n==2) || (strcmp(fn,"downcase_atom")==0 && n==2)
        || (strcmp(fn,"char_type")==0 && n==2)
        || (strcmp(fn,"atom_string")==0 && n==2) || (strcmp(fn,"number_string")==0 && n==2)
        || (strcmp(fn,"string_upper")==0 && n==2) || (strcmp(fn,"string_lower")==0 && n==2)
        || (strcmp(fn,"string_concat")==0 && n==3) || (strcmp(fn,"string_length")==0 && n==2)
        || (strcmp(fn,"string_chars")==0 && n==2)  || (strcmp(fn,"string_codes")==0 && n==2)
        || (strcmp(fn,"term_to_atom")==0 && n==2)  || (strcmp(fn,"term_string")==0 && n==2)
        || (strcmp(fn,"atom_number")==0 && n==2)
        || (strcmp(fn,"copy_term")==0 && n==2)
        || (strcmp(fn,"atomic_list_concat")==0 && (n==2||n==3)) || (strcmp(fn,"concat_atom")==0 && n==2)
        || (strcmp(fn,"string_to_atom")==0 && n==2)) return RESOLVE_BI_CHAIN;
    if ((strcmp(fn,"sort")==0||strcmp(fn,"msort")==0) && n==2) return RESOLVE_BI_CHAIN;
    if (strcmp(fn,"format")==0 && (n==1||n==2)) return RESOLVE_BI_CHAIN;
    if (strcmp(fn,"numbervars")==0 && n==3) return RESOLVE_BI_CHAIN;
    if ((strcmp(fn,"writeq")==0||strcmp(fn,"write_canonical")==0||strcmp(fn,"print")==0) && n==1) return RESOLVE_BI_CHAIN;
    if ((strcmp(fn,"retract")==0||strcmp(fn,"retractall")==0) && n==1) return RESOLVE_BI_CHAIN;
    if (strcmp(fn,"abolish")==0 && n==1) return RESOLVE_BI_CHAIN_ABOLISH;
    if (strcmp(fn,"plus")==0 && n==3) return RESOLVE_BI_CHAIN;
    if ((strcmp(fn,"nb_setval")==0||strcmp(fn,"nb_getval")==0) && n==2) return RESOLVE_BI_CHAIN;
    if (strcmp(fn,"aggregate_all")==0 && n==3) return RESOLVE_BI_CHAIN;
    return RESOLVE_BI_NONE;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t *lower_pl_new_Builtin(IR_graph_t *bbg, const char *fn, const tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out) {
    int style = resolve_builtin_style(fn, e->n);
    if (style == RESOLVE_BI_NONE) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_BUILTIN); if (!bb) return NULL;
    bb->sval = fn; bb->ival = e->n;
    if (style == RESOLVE_BI_AB) {
        if (e->n>=1) { IR_t *aα=NULL,*aβ=NULL; IR_t *a=lower_pl_term(bbg,e->c[0],γ_in,ω_in,&aα,&aβ); if(!a) return NULL; bb->α=aα; }
        if (e->n>=2) { IR_t *bα=NULL,*bβ=NULL; IR_t *b=lower_pl_term(bbg,e->c[1],γ_in,ω_in,&bα,&bβ); if(!b) return NULL; bb->β=bα; }
        bb->γ=γ_in; bb->ω=ω_in;
        if (α_out) *α_out=bb; if (β_out) *β_out=bb;
        return bb;
    }
    if (style == RESOLVE_BI_TYPETEST) {
        IR_t *aα=NULL,*aβ=NULL; IR_t *a=lower_pl_term(bbg,e->c[0],γ_in,ω_in,&aα,&aβ); if(!a) return NULL; bb->α=aα;
        bb->γ=γ_in; bb->ω=ω_in;
        if (α_out) *α_out=bb; if (β_out) *β_out=bb;
        return bb;
    }
    IR_t *prev = NULL;
    for (int i=0; i<e->n; i++) {
        IR_t *aα=NULL,*aβ=NULL; IR_t *a=lower_pl_term(bbg,e->c[i],NULL,NULL,&aα,&aβ); if(!a) return NULL;
        if (i==0) bb->α=aα; else prev->γ=aα;
        prev = aα;
    }
    bb->γ=γ_in; bb->ω=ω_in;
    if (α_out) *α_out=bb;
    if (β_out) *β_out = (style==RESOLVE_BI_CHAIN_ABOLISH) ? ω_in : bb;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int resolve_is_arith_functor(const char *fn, int arity) {
    if (!fn) return 0;
    if (arity == 0)
        return strcmp(fn,"pi")==0 || strcmp(fn,"e")==0;
    if (arity == 1)
        return strcmp(fn,"-")==0 || strcmp(fn,"+")==0 || strcmp(fn,"abs")==0 || strcmp(fn,"sign")==0
            || strcmp(fn,"sqrt")==0 || strcmp(fn,"sin")==0 || strcmp(fn,"cos")==0 || strcmp(fn,"tan")==0
            || strcmp(fn,"asin")==0 || strcmp(fn,"acos")==0 || strcmp(fn,"atan")==0
            || strcmp(fn,"exp")==0 || strcmp(fn,"log")==0
            || strcmp(fn,"float")==0 || strcmp(fn,"integer")==0
            || strcmp(fn,"float_integer_part")==0 || strcmp(fn,"float_fractional_part")==0
            || strcmp(fn,"truncate")==0 || strcmp(fn,"round")==0
            || strcmp(fn,"ceiling")==0 || strcmp(fn,"floor")==0
            || strcmp(fn,"\\")==0 || strcmp(fn,"msb")==0;
    if (arity == 2)
        return strcmp(fn,"+")==0 || strcmp(fn,"-")==0 || strcmp(fn,"*")==0 || strcmp(fn,"/")==0
            || strcmp(fn,"//")==0 || strcmp(fn,"mod")==0 || strcmp(fn,"rem")==0 || strcmp(fn,"div")==0
            || strcmp(fn,"**")==0 || strcmp(fn,"^")==0
            || strcmp(fn,"min")==0 || strcmp(fn,"max")==0 || strcmp(fn,"gcd")==0
            || strcmp(fn,"/\\")==0 || strcmp(fn,"\\/")==0 || strcmp(fn,"xor")==0
            || strcmp(fn,"<<")==0 || strcmp(fn,">>")==0;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t *lower_pl_term(IR_graph_t *bbg, const tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out) {
    if (!e) return NULL;
    switch (e->t) {
    case TT_ILIT: { IR_t *bb = IR_node_alloc(bbg, IR_LIT_I); if (!bb) return NULL; bb->ival = e->v.ival; return resolve_leaf(bb, γ_in, ω_in, α_out, β_out); }
    case TT_FLIT: { IR_t *bb = IR_node_alloc(bbg, IR_LIT_F); if (!bb) return NULL; bb->dval = e->v.dval; return resolve_leaf(bb, γ_in, ω_in, α_out, β_out); }
    case TT_QLIT: case TT_NAME: { IR_t *bb = IR_node_alloc(bbg, IR_ATOM); if (!bb) return NULL; bb->sval = e->v.sval; return resolve_leaf(bb, γ_in, ω_in, α_out, β_out); }
    case TT_VAR: { IR_t *bb = IR_node_alloc(bbg, IR_LOGICVAR); if (!bb) return NULL; bb->ival = e->v.ival; bb->sval = NULL; return resolve_leaf(bb, γ_in, ω_in, α_out, β_out); }
    case TT_MAKELIST: {
        int has_tail = (e->v.ival == 1);
        int n_elem   = has_tail ? e->n - 1 : e->n;
        IR_t *rest;
        if (has_tail && e->n >= 1) {
            IR_t *tα=NULL,*tβ=NULL; rest = lower_pl_term(bbg, e->c[e->n-1], NULL, NULL, &tα, &tβ); if (!rest) return NULL; rest = tα;
        } else {
            IR_t *nil = IR_node_alloc(bbg, IR_ATOM); if (!nil) return NULL; nil->sval = "[]"; rest = nil;
        }
        for (int i = n_elem - 1; i >= 0; i--) {
            IR_t *hα=NULL,*hβ=NULL; IR_t *head = lower_pl_term(bbg, e->c[i], NULL, NULL, &hα, &hβ); if (!head) return NULL;
            IR_t *cons = IR_node_alloc(bbg, IR_STRUCT); if (!cons) return NULL;
            cons->sval = "."; cons->ival = 2;
            cons->α = hα; hα->γ = rest;
            rest = cons;
        }
        rest->γ = γ_in; rest->ω = ω_in;
        if (α_out) *α_out = rest; if (β_out) *β_out = rest;
        return rest;
    }
    case TT_ADD: case TT_SUB: case TT_MUL: case TT_DIV: case TT_MOD: {
        if (e->n < 2) return NULL;
        IR_t *lα = NULL, *lβ = NULL, *rα = NULL, *rβ = NULL;
        IR_t *lhs = lower_pl_term(bbg, e->c[0], NULL, NULL, &lα, &lβ); if (!lhs) return NULL;
        IR_t *rhs = lower_pl_term(bbg, e->c[1], NULL, NULL, &rα, &rβ); if (!rhs) return NULL;
        IR_t *bb = IR_node_alloc(bbg, IR_ARITH); if (!bb) return NULL;
        bb->sval = (e->t==TT_ADD)?"+":(e->t==TT_SUB)?"-":(e->t==TT_MUL)?"*":(e->t==TT_DIV)?"/":"mod";
        bb->ival = 2;
        bb->α = lα; bb->β = rα;
        bb->γ = γ_in; bb->ω = ω_in;
        if (α_out) *α_out = bb; if (β_out) *β_out = bb;
        return bb;
    }
    case TT_FNC: {
        const char *fn = e->v.sval;
        if (!fn) return NULL;
        if (resolve_is_arith_functor(fn, e->n)) {
            IR_t *bb = IR_node_alloc(bbg, IR_ARITH); if (!bb) return NULL;
            bb->sval = fn; bb->ival = e->n;
            if (e->n >= 1) {
                IR_t *lα=NULL,*lβ=NULL; IR_t *l = lower_pl_term(bbg, e->c[0], NULL, NULL, &lα, &lβ); if (!l) return NULL; bb->α = lα;
            }
            if (e->n >= 2) {
                IR_t *rα=NULL,*rβ=NULL; IR_t *r = lower_pl_term(bbg, e->c[1], NULL, NULL, &rα, &rβ); if (!r) return NULL; bb->β = rα;
            }
            bb->γ = γ_in; bb->ω = ω_in;
            if (α_out) *α_out = bb; if (β_out) *β_out = bb;
            return bb;
        }
        if (e->n == 0) { IR_t *bb = IR_node_alloc(bbg, IR_ATOM); if (!bb) return NULL; bb->sval = fn; return resolve_leaf(bb, γ_in, ω_in, α_out, β_out); }
        {
            IR_t *st = IR_node_alloc(bbg, IR_STRUCT); if (!st) return NULL;
            st->sval = fn; st->ival = e->n; st->γ = γ_in; st->ω = ω_in;
            IR_t *prev = NULL;
            for (int i = 0; i < e->n; i++) {
                IR_t *aα = NULL, *aβ = NULL;
                IR_t *arg = lower_pl_term(bbg, e->c[i], NULL, NULL, &aα, &aβ); if (!arg) return NULL;
                if (i == 0) st->α = aα; else if (prev) prev->γ = aα;
                prev = aα;
            }
            if (α_out) *α_out = st; if (β_out) *β_out = st;
            return st;
        }
    }
    default: return NULL;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int flatten_comma(const tree_t *e, const tree_t **out, int cap) {
    if (!e || cap <= 0) return 0;
    if (e->t == TT_FNC && e->v.sval && strcmp(e->v.sval,",")==0 && e->n==2) {
        int L = flatten_comma(e->c[0], out, cap);
        return L + flatten_comma(e->c[1], out+L, cap-L);
    }
    out[0] = e; return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int resolve_goal_is_bounded(const tree_t *e) {
    if (!e) return 0;
    if (e->t == TT_CUT) return 1;
    if (e->t == TT_UNIFY) return 1;
    if (e->t==TT_GT||e->t==TT_LT||e->t==TT_GE||e->t==TT_LE||e->t==TT_EQ||e->t==TT_NE) return 1;
    if (e->t == TT_FNC && e->v.sval && strcmp(e->v.sval,",")==0 && e->n==2)
        return resolve_goal_is_bounded(e->c[0]) && resolve_goal_is_bounded(e->c[1]);
    if (e->t == TT_FNC && e->v.sval && strcmp(e->v.sval,";")==0 && e->n==2) return 0;
    if (e->t == TT_IF && e->n >= 2) {
        const tree_t *then_ = e->c[1];
        const tree_t *else_ = (e->n >= 3) ? e->c[2] : NULL;
        if (then_ && !resolve_goal_is_bounded(then_)) return 0;
        if (else_ && !resolve_goal_is_bounded(else_)) return 0;
        return 1;
    }
    if (e->t == TT_UNIFY || (e->t==TT_FNC && e->v.sval && strcmp(e->v.sval,"=")==0 && e->n==2)) return 1;
    if (e->t == TT_QLIT && e->v.sval) {
        const char *fn = e->v.sval;
        if (strcmp(fn,"true")==0||strcmp(fn,"otherwise")==0||strcmp(fn,"fail")==0||strcmp(fn,"false")==0) return 1;
        if (strcmp(fn,"nl")==0||strcmp(fn,"!")==0) return 1;
        return 0;
    }
    if (e->t == TT_VAR) return 0;
    if (e->t != TT_FNC || !e->v.sval) return 0;
    const char *fn = e->v.sval;
    if (strcmp(fn,"true")==0||strcmp(fn,"otherwise")==0||strcmp(fn,"fail")==0||strcmp(fn,"false")==0||strcmp(fn,"nl")==0) return 1;
    if (resolve_builtin_style(fn, e->n) != RESOLVE_BI_NONE) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t *lower_pl_goal(IR_graph_t *bbg, const tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out) {
    if (!e) return NULL;
    if (getenv("SCRIP_PL_BOUNDED_TRACE")) {
        const char *gn = (e->t==TT_FNC && e->v.sval) ? e->v.sval : (e->t==TT_QLIT && e->v.sval) ? e->v.sval : (e->t==TT_CUT) ? "!" : (e->t==TT_VAR) ? "<var>" : "<op>";
        fprintf(stderr, "[PL-BOUNDED] goal=%-12s bounded=%d\n", gn, resolve_goal_is_bounded(e));
    }
    if (e->t == TT_FNC && e->v.sval && strcmp(e->v.sval,",")==0 && e->n==2) {
        return lower_pl_new_Conj(bbg, e, γ_in, ω_in, α_out, β_out);
    }
    if (e->t == TT_FNC && e->v.sval && strcmp(e->v.sval,";")==0 && e->n==2) {
        return lower_pl_new_Alt(bbg, e, γ_in, ω_in, α_out, β_out);
    }
    if (e->t == TT_IF && e->n >= 2) {
        return lower_pl_new_Ite(bbg, e, γ_in, ω_in, α_out, β_out);
    }
    if (e->t == TT_UNIFY || (e->t==TT_FNC && e->v.sval && strcmp(e->v.sval,"=")==0 && e->n==2)) {
        return lower_pl_new_Unify(bbg, e, γ_in, ω_in, α_out, β_out);
    }
    if (e->t==TT_GT||e->t==TT_LT||e->t==TT_GE||e->t==TT_LE||e->t==TT_EQ||e->t==TT_NE) {
        return lower_pl_new_Compare(bbg, e, γ_in, ω_in, α_out, β_out);
    }
    if (e->t == TT_CUT) return resolve_leaf(IR_node_alloc(bbg, IR_CUT), γ_in, ω_in, α_out, β_out);
    if (e->t == TT_QLIT && e->v.sval) {
        const char *fn = e->v.sval;
        if (strcmp(fn,"true")==0||strcmp(fn,"otherwise")==0) return resolve_leaf(IR_node_alloc(bbg,IR_SUCCEED),γ_in,ω_in,α_out,β_out);
        if (strcmp(fn,"fail")==0||strcmp(fn,"false")==0)     return resolve_leaf(IR_node_alloc(bbg,IR_FAIL),γ_in,ω_in,α_out,β_out);
        if (strcmp(fn,"nl")==0)   { IR_t *bb=IR_node_alloc(bbg,IR_BUILTIN); if(!bb) return NULL; bb->sval=fn; return resolve_leaf(bb,γ_in,ω_in,α_out,β_out); }
        if (strcmp(fn,"!")==0)    return resolve_leaf(IR_node_alloc(bbg,IR_CUT),γ_in,ω_in,α_out,β_out);
        return lower_pl_new_Call(bbg, fn, e, γ_in, ω_in, α_out, β_out);
    }
    if (e->t == TT_VAR) {
        IR_t *bb = IR_node_alloc(bbg, IR_GOAL); if (!bb) return NULL;
        bb_goal_state_t *zc = (bb_goal_state_t *)GC_MALLOC_UNCOLLECTABLE(sizeof *zc);
        zc->callee = "call"; zc->arity = 1; zc->cs = NULL;
        zc->args = (IR_t **)GC_MALLOC_UNCOLLECTABLE(sizeof(IR_t *)); zc->nargs = 0;
        IR_t *aα = NULL, *aβ = NULL;
        IR_t *a = lower_pl_term(bbg, e, γ_in, ω_in, &aα, &aβ); if (!a) return NULL;
        zc->args[zc->nargs++] = aα;
        bb->sval = "call"; bb->ival = (int64_t)(intptr_t)zc;
        bb->α = zc->args[0];
        bb->γ = γ_in; bb->ω = ω_in;
        if (α_out) *α_out = bb;
        if (β_out) *β_out = bb;
        return bb;
    }
    if (e->t != TT_FNC || !e->v.sval) return NULL;
    const char *fn = e->v.sval;
    if (strcmp(fn,"true")==0||strcmp(fn,"otherwise")==0) return resolve_leaf(IR_node_alloc(bbg,IR_SUCCEED),γ_in,ω_in,α_out,β_out);
    if (strcmp(fn,"fail")==0||strcmp(fn,"false")==0)     return resolve_leaf(IR_node_alloc(bbg,IR_FAIL),γ_in,ω_in,α_out,β_out);
    if (strcmp(fn,"nl")==0) { IR_t *bb=IR_node_alloc(bbg,IR_BUILTIN); if(!bb) return NULL; bb->sval=fn; return resolve_leaf(bb,γ_in,ω_in,α_out,β_out); }
    if (resolve_builtin_style(fn, e->n) != RESOLVE_BI_NONE) return lower_pl_new_Builtin(bbg, fn, e, γ_in, ω_in, α_out, β_out);
    if (strcmp(fn,"phrase")==0 && (e->n==2 || e->n==3)) {
        const tree_t *nt = e->c[0];
        const char *callee = NULL; int nt_arity = 0; const tree_t * const *nt_args = NULL;
        if (nt && (nt->t==TT_QLIT || nt->t==TT_NAME) && nt->v.sval) { callee = nt->v.sval; nt_arity = 0; }
        else if (nt && nt->t==TT_FNC && nt->v.sval) { callee = nt->v.sval; nt_arity = nt->n; nt_args = (const tree_t * const *)nt->c; }
        else return NULL;
        int call_arity = nt_arity + 2;
        IR_t *bb = IR_node_alloc(bbg, IR_GOAL); if (!bb) return NULL;
        bb_goal_state_t *zc = (bb_goal_state_t *)GC_MALLOC_UNCOLLECTABLE(sizeof *zc);
        zc->callee=callee; zc->arity=call_arity; zc->cs=NULL;
        zc->args = (IR_t **)GC_MALLOC_UNCOLLECTABLE((size_t)call_arity*sizeof(IR_t *));
        zc->nargs=0;
        for (int i=0; i<nt_arity; i++) {
            IR_t *aα=NULL,*aβ=NULL; IR_t *a=lower_pl_term(bbg,nt_args[i],γ_in,ω_in,&aα,&aβ); if(!a) return NULL;
            zc->args[zc->nargs++]=aα;
        }
        { IR_t *aα=NULL,*aβ=NULL; IR_t *a=lower_pl_term(bbg,e->c[1],γ_in,ω_in,&aα,&aβ); if(!a) return NULL; zc->args[zc->nargs++]=aα; }
        if (e->n==3) { IR_t *aα=NULL,*aβ=NULL; IR_t *a=lower_pl_term(bbg,e->c[2],γ_in,ω_in,&aα,&aβ); if(!a) return NULL; zc->args[zc->nargs++]=aα; }
        else { IR_t *nil = IR_node_alloc(bbg, IR_ATOM); if (!nil) return NULL; nil->sval="[]"; nil->γ=γ_in; nil->ω=ω_in; zc->args[zc->nargs++]=nil; }
        bb->sval=callee; bb->ival=(int64_t)(intptr_t)zc;
        if (call_arity>=1) bb->α=zc->args[0];
        if (call_arity>=2) bb->β=zc->args[1];
        bb->γ=γ_in; bb->ω=ω_in;
        if (α_out) *α_out=bb; if (β_out) *β_out=bb;
        return bb;
    }
    if (strcmp(fn,"throw")==0 && e->n==1) {
        IR_t *bb = IR_node_alloc(bbg, IR_BUILTIN); if (!bb) return NULL; bb->sval="throw";
        IR_t *tα=NULL,*tβ=NULL; IR_t *t = lower_pl_term(bbg, e->c[0], γ_in, ω_in, &tα, &tβ); if (!t) return NULL;
        bb->α = tα;
        bb->γ = γ_in; bb->ω = ω_in;
        if (α_out) *α_out = bb; if (β_out) *β_out = ω_in;
        return bb;
    }
    if (strcmp(fn,"catch")==0 && e->n==3) {
        IR_t *bb = IR_node_alloc(bbg, IR_CATCH); if (!bb) return NULL;
        bb_catch_state_t *zc = (bb_catch_state_t *)GC_MALLOC_UNCOLLECTABLE(sizeof *zc); if (!zc) return NULL;
        IR_t *cα=NULL,*cβ=NULL; IR_t *c = lower_pl_term(bbg, e->c[1], NULL, NULL, &cα, &cβ); if (!c) return NULL;
        zc->catcher = cα;
        IR_graph_t *gcfg = IR_alloc(128, IR_LANG_PL); if (!gcfg) return NULL;
        IR_t *gα=NULL,*gβ=NULL; IR_t *g = lower_pl_goal(gcfg, e->c[0], NULL, NULL, &gα, &gβ); if (!g) return NULL;
        gcfg->entry = gα ? gα : g;
        zc->goal_g = gcfg;
        IR_graph_t *rcfg = IR_alloc(128, IR_LANG_PL); if (!rcfg) return NULL;
        IR_t *rα=NULL,*rβ=NULL; IR_t *r = lower_pl_goal(rcfg, e->c[2], NULL, NULL, &rα, &rβ); if (!r) return NULL;
        rcfg->entry = rα ? rα : r;
        zc->rec_g = rcfg;
        bb->ival = (int64_t)(intptr_t)zc;
        bb->α = cα;
        bb->γ = γ_in; bb->ω = ω_in;
        if (α_out) *α_out = bb; if (β_out) *β_out = ω_in;
        return bb;
    }
    if (strcmp(fn,"findall")==0 && e->n==3) {
        IR_t *bb = IR_node_alloc(bbg, IR_BUILTIN); if (!bb) return NULL; bb->sval="findall"; bb->ival=0;
        bb_findall_state_t *fs = (bb_findall_state_t *)GC_MALLOC_UNCOLLECTABLE(sizeof *fs);
        IR_t *tα=NULL,*tβ=NULL; IR_t *t=lower_pl_term(bbg,e->c[0],NULL,NULL,&tα,&tβ); if(!t) return NULL; fs->tmpl=tα;
        IR_t *rα=NULL,*rβ=NULL; IR_t *r=lower_pl_term(bbg,e->c[2],NULL,NULL,&rα,&rβ); if(!r) return NULL; fs->result=rα;
        IR_graph_t *gcfg = IR_alloc(128, IR_LANG_PL); if (!gcfg) return NULL;
        IR_t *gα=NULL,*gβ=NULL; IR_t *g=lower_pl_goal(gcfg,e->c[1],NULL,NULL,&gα,&gβ); if(!g) return NULL;
        gcfg->entry = gα ? gα : g;
        fs->gcfg = gcfg;
        bb->sval="findall"; bb->ival=(int64_t)(intptr_t)fs;
        bb->γ=γ_in; bb->ω=ω_in;
        if (α_out) *α_out=bb; if (β_out) *β_out=bb;
        return bb;
    }
    return lower_pl_new_Call(bbg, fn, e, γ_in, ω_in, α_out, β_out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_graph_t *lower_pl_clause_body(const tree_t *clause, int n_args) {
    if (!clause || clause->t != TT_CLAUSE) return NULL;
    int n_body = clause->n - n_args;
    int n_total = n_args + (n_body > 0 ? n_body : 1);
    IR_graph_t *bbg = IR_alloc(128, IR_LANG_PL); if (!bbg) return NULL;
    const tree_t **stmts = (const tree_t **)calloc((size_t)n_total, sizeof(tree_t *));
    if (!stmts) { IR_free(bbg); return NULL; }
    int n_stmts = 0;
    for (int i=0; i<n_args; i++) if (clause->c[i]) stmts[n_stmts++] = clause->c[i];
    for (int i=0; i<n_body; i++) if (clause->c[n_args+i]) stmts[n_stmts++] = clause->c[n_args+i];
    if (n_stmts == 0) {
        IR_t *bb = IR_node_alloc(bbg, IR_SUCCEED); if (!bb) { free(stmts); IR_free(bbg); return NULL; }
        bb->γ = NULL; bb->ω = NULL;
        bbg->entry = bb; free(stmts); return bbg;
    }
    IR_t **nα = (IR_t **)calloc((size_t)n_stmts, sizeof(IR_t *));
    IR_t **nβ = (IR_t **)calloc((size_t)n_stmts, sizeof(IR_t *));
    IR_t **gnodes = (IR_t **)calloc((size_t)n_stmts, sizeof(IR_t *));
    if (!nα || !nβ || !gnodes) { free(stmts); free(nα); free(nβ); free(gnodes); IR_free(bbg); return NULL; }
    IR_t *succ = NULL;
    for (int i = n_stmts-1; i >= 0; i--) {
        const tree_t *st = stmts[i];
        IR_t *aα = NULL, *aβ = NULL;
        IR_t *bb;
        if (i < n_args) {
            IR_t *lα=NULL,*lβ=NULL,*rα=NULL,*rβ=NULL;
            IR_t *slot_var = IR_node_alloc(bbg, IR_LOGICVAR); if (!slot_var) goto fail;
            slot_var->ival = i; slot_var->sval = NULL;
            resolve_leaf(slot_var, succ, NULL, &lα, &lβ);
            IR_t *head_ir = lower_pl_term(bbg, st, succ, NULL, &rα, &rβ); if (!head_ir) goto fail;
            IR_t *uni = IR_node_alloc(bbg, IR_UNIFY); if (!uni) goto fail;
            uni->α = lα; uni->β = rα; uni->γ = succ; uni->ω = NULL;
            aα = uni; aβ = uni; bb = uni;
        } else {
            bb = lower_pl_goal(bbg, st, succ, NULL, &aα, &aβ); if (!bb) goto fail;
        }
        nα[i] = aα ? aα : bb;
        nβ[i] = aβ ? aβ : bb;
        gnodes[i] = bb;
        succ = nα[i];
    }
    {
        IR_t *eff_β_prev = NULL;
        for (int i = 0; i < n_stmts; i++) {
            int resumable = resolve_node_is_resumable(gnodes[i]);
            if (i > 0 && !gnodes[i]->ω) gnodes[i]->ω = eff_β_prev;
            eff_β_prev = resumable ? gnodes[i] : eff_β_prev;
        }
    }
    {
        IR_t *seq = IR_node_alloc(bbg, IR_GCONJ);
        if (!seq) goto fail;
        seq->α = nα[0]; seq->γ = NULL; seq->ω = NULL;
        bb_conj_state_t *zs = (bb_conj_state_t *)GC_MALLOC_UNCOLLECTABLE(sizeof *zs);
        if (zs) {
            zs->goals  = (IR_t **)GC_MALLOC_UNCOLLECTABLE((size_t)n_stmts * sizeof(IR_t *));
            zs->ngoals = n_stmts;
            for (int i = 0; i < n_stmts; i++) zs->goals[i] = gnodes[i];
            seq->ival = (int64_t)(intptr_t)zs;
        }
        bbg->entry = seq;
    }
    free(stmts); free(nα); free(nβ); free(gnodes);
    return bbg;
fail:
    free(stmts); free(nα); free(nβ); free(gnodes); IR_free(bbg); return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long resolve_clause_first_arg_key(const tree_t *clause, int n_args) {
    if (n_args < 1 || !clause || clause->n < 1) return RESOLVE_IDX_VAR;
    const tree_t *a0 = clause->c[0];
    if (!a0) return RESOLVE_IDX_VAR;
    switch (a0->t) {
    case TT_VAR: case TT_DEFER: case TT_INDIRECT: return RESOLVE_IDX_VAR;
    case TT_ILIT: return RESOLVE_IDX_INT(a0->v.ival);
    case TT_FLIT: return RESOLVE_IDX_FLT;
    case TT_QLIT: case TT_NAME: return RESOLVE_IDX_ATOM(prolog_atom_intern(a0->v.sval ? a0->v.sval : "[]"));
    case TT_MAKELIST: {
        int has_tail = (a0->v.ival == 1);
        int n_elem = has_tail ? a0->n - 1 : a0->n;
        if (n_elem <= 0 && !has_tail) return RESOLVE_IDX_ATOM(prolog_atom_intern("[]"));
        return RESOLVE_IDX_CMP(prolog_atom_intern("."), 2);
    }
    case TT_FNC: return RESOLVE_IDX_CMP(prolog_atom_intern(a0->v.sval ? a0->v.sval : "[]"), a0->n);
    default: return RESOLVE_IDX_VAR;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t *lower_pl_predicate(tree_t *choice) {
    if (!choice || choice->t != TT_CHOICE || choice->n < 1) return NULL;
    const char *_csl = choice->v.sval ? strrchr(choice->v.sval, '/') : NULL;
    int arity = _csl ? atoi(_csl+1) : 0;
    if (choice->n == 1) return lower_pl_clause_body(choice->c[0], arity);
    IR_graph_t *bbg = IR_alloc(64, IR_LANG_PL); if (!bbg) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_CHOICE); if (!bb) { IR_free(bbg); return NULL; }
    bb_choice_state_t *zc = (bb_choice_state_t *)GC_MALLOC_UNCOLLECTABLE(sizeof *zc);
    zc->bodies  = (IR_graph_t **)GC_MALLOC_UNCOLLECTABLE((size_t)choice->n * sizeof(IR_graph_t *));
    zc->nbodies = 0; zc->cur = 0; zc->mark = 0; zc->saved_env = NULL;
    zc->idx_key = (long *)GC_MALLOC_UNCOLLECTABLE((size_t)choice->n * sizeof(long));
    zc->idx_ok = 0;
    for (int i=0; i<choice->n; i++) {
        IR_graph_t *body = lower_pl_clause_body(choice->c[i], arity);
        if (!body) { IR_free(bbg); return NULL; }
        zc->idx_key[zc->nbodies] = resolve_clause_first_arg_key(choice->c[i], arity);
        zc->bodies[zc->nbodies++] = body;
    }
    zc->idx_ok = (arity >= 1) ? 1 : 0;
    bb->ival = (int64_t)(intptr_t)zc;
    bb->γ = NULL; bb->ω = NULL;
    bbg->entry = bb;
    return bbg;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_pl_threaded(IR_graph_t *bbg, const struct tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out) {
    return lower_pl_goal(bbg, e, γ_in, ω_in, α_out, β_out);
}
/*====================================================================================================================================================================================================*/
/* ===== SNOBOL4 PATTERN LOWERING (build_node: sp/fp = the same gamma/omega ports) ===== */
/*====================================================================================================================================================================================================*/
static int count_tree(const tree_t * t) {
    if (!t) return 0;
    int n = 1;
    for (int i = 0; i < t->n; i++) n += count_tree(t->c[i]);
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * cset_kw_value(const char * kw) {
    if (!kw) return NULL;
    if (strcmp(kw, "UCASE")  == 0) return "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (strcmp(kw, "LCASE")  == 0) return "abcdefghijklmnopqrstuvwxyz";
    if (strcmp(kw, "DIGITS") == 0) return "0123456789";
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int cset_fold_len(const tree_t * t) {
    if (!t) return -1;
    if (t->t == TT_QLIT) return (int)strlen(t->v.sval ? t->v.sval : "");
    if (t->t == TT_KEYWORD) { const char * v = cset_kw_value(t->v.sval); return v ? (int)strlen(v) : -1; }
    if (t->t == TT_SEQ || t->t == TT_CAT) { int total = 0; for (int i = 0; i < t->n; i++) { int l = cset_fold_len(t->c[i]); if (l < 0) return -1; total += l; } return total; }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int cset_fold_fill(const tree_t * t, char * buf, int * pos) {
    if (!t) return 0;
    if (t->t == TT_QLIT) { const char * s = t->v.sval ? t->v.sval : ""; int l = (int)strlen(s); memcpy(buf + *pos, s, (size_t)l); *pos += l; return 1; }
    if (t->t == TT_KEYWORD) { const char * v = cset_kw_value(t->v.sval); if (!v) return 0; int l = (int)strlen(v); memcpy(buf + *pos, v, (size_t)l); *pos += l; return 1; }
    if (t->t == TT_SEQ || t->t == TT_CAT) { for (int i = 0; i < t->n; i++) if (!cset_fold_fill(t->c[i], buf, pos)) return 0; return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char * cset_try_fold(const tree_t * t) {
    int n = cset_fold_len(t);
    if (n < 0) return NULL;
    char * buf = (char *)GC_malloc((size_t)n + 1);
    if (!buf) return NULL;
    int pos = 0;
    if (!cset_fold_fill(t, buf, &pos)) return NULL;
    buf[pos] = 0;
    return buf;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int flatten_cat_count(const tree_t * t) {
    if (!t) return 0;
    if (t->t == TT_SEQ || t->t == TT_CAT) {
        int n = 0;
        for (int i = 0; i < t->n; i++) n += flatten_cat_count(t->c[i]);
        return n;
    }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int flatten_cat_fill(const tree_t * t, const tree_t ** out, int idx) {
    if (!t) return idx;
    if (t->t == TT_SEQ || t->t == TT_CAT) {
        for (int i = 0; i < t->n; i++) idx = flatten_cat_fill(t->c[i], out, idx);
        return idx;
    }
    out[idx++] = t;
    return idx;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_graph_t * build_node(IR_graph_t * bbg, const tree_t * t, IR_t * sp, IR_t * fp);
static IR_graph_t * build_node(IR_graph_t * bbg, const tree_t * t, IR_t * sp, IR_t * fp) {
    if (!t) return sp;
    IR_t * bb = NULL;
    switch (t->t) {
    case TT_QLIT: {
        bb = IR_node_alloc(bbg, IR_PAT_LIT);
        bb->sval = t->v.sval ? t->v.sval : "";
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case TT_ARB: {
        bb = IR_node_alloc(bbg, IR_PAT_ARB);
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case TT_REM: {
        bb = IR_node_alloc(bbg, IR_PAT_REM);
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case TT_ABORT: {
        bb = IR_node_alloc(bbg, IR_PAT_ABORT);
        bb->α = bb; bb->β = fp; bb->γ = fp; bb->ω = fp;
        return bb;
    }
    case TT_SPAN: {
        if (t->n < 1 || !t->c[0]) return NULL;
        if (t->c[0]->t != TT_QLIT && t->c[0]->t != TT_VAR) {
            char * cs = cset_try_fold(t->c[0]); if (!cs) return NULL;
            bb = IR_node_alloc(bbg, IR_PAT_SPAN);
            bb->sval = cs; bb->ival = 0;
            bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
            return bb;
        }
        bb = IR_node_alloc(bbg, IR_PAT_SPAN);
        bb->sval = t->c[0]->v.sval ? t->c[0]->v.sval : "";
        bb->ival = (t->c[0]->t == TT_VAR) ? 1 : 0;
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case TT_ANY: {
        if (t->n < 1 || !t->c[0]) return NULL;
        if (t->c[0]->t != TT_QLIT && t->c[0]->t != TT_VAR) {
            char * cs = cset_try_fold(t->c[0]); if (!cs) return NULL;
            bb = IR_node_alloc(bbg, IR_PAT_ANY);
            bb->sval = cs; bb->dval = 0.0;
            bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
            return bb;
        }
        bb = IR_node_alloc(bbg, IR_PAT_ANY);
        bb->sval = t->c[0]->v.sval ? t->c[0]->v.sval : "";
        bb->dval = (t->c[0]->t == TT_VAR) ? 1.0 : 0.0;
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case TT_BREAK: {
        if (t->n < 1 || !t->c[0]) return NULL;
        if (t->c[0]->t != TT_QLIT && t->c[0]->t != TT_VAR) {
            char * cs = cset_try_fold(t->c[0]); if (!cs) return NULL;
            bb = IR_node_alloc(bbg, IR_PAT_BREAK);
            bb->sval = cs; bb->ival = 0; bb->dval = 0.0;
            bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
            return bb;
        }
        bb = IR_node_alloc(bbg, IR_PAT_BREAK);
        bb->sval = t->c[0]->v.sval ? t->c[0]->v.sval : "";
        bb->ival = 0;
        bb->dval = (t->c[0]->t == TT_VAR) ? 1.0 : 0.0;
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case TT_BREAKX: {
        if (t->n < 1 || !t->c[0]) return NULL;
        if (t->c[0]->t != TT_QLIT && t->c[0]->t != TT_VAR) {
            char * cs = cset_try_fold(t->c[0]); if (!cs) return NULL;
            bb = IR_node_alloc(bbg, IR_PAT_BREAK);
            bb->sval = cs; bb->ival = 1; bb->dval = 0.0;
            bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
            return bb;
        }
        bb = IR_node_alloc(bbg, IR_PAT_BREAK);
        bb->sval = t->c[0]->v.sval ? t->c[0]->v.sval : "";
        bb->ival = 1;
        bb->dval = (t->c[0]->t == TT_VAR) ? 1.0 : 0.0;
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case TT_FENCE: {
        IR_t * inner = (t->n > 0 && t->c[0]) ? build_node(bbg, t->c[0], sp, fp) : sp;
        if (t->n > 0 && !inner) return NULL;
        bb = IR_node_alloc(bbg, IR_PAT_FENCE);
        bb->α = bb; bb->β = bb;
        bb->γ = inner ? inner : sp; bb->ω = fp;
        return bb;
    }
    case TT_ARBNO: {
        if (t->n < 1 || !t->c[0]) return NULL;
        int inner_cap = count_tree(t->c[0]) * 8 + 16;
        IR_graph_t * inner_blk = IR_alloc(inner_cap, IR_LANG_SNO);
        if (!inner_blk) return NULL;
        IR_t * inner_entry = build_node(inner_blk, t->c[0], NULL, NULL);
        if (!inner_entry) { IR_free(inner_blk); return NULL; }
        inner_blk->entry = inner_entry;
        bb = IR_node_alloc(bbg, IR_PAT_ARBNO);
        int stack_cap = 64;
        bb_arbno_state_t * az = (bb_arbno_state_t *)GC_MALLOC(sizeof *az);
        az->inner = inner_blk;
        az->pos_stack = (int *)GC_MALLOC((size_t)stack_cap * sizeof(int));
        az->cap = stack_cap;
        az->saved_delta = 0;
        bb->counter = (int64_t)(intptr_t)az;
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case TT_SEQ:
    case TT_CAT: {
        int nflat = flatten_cat_count(t);
        if (nflat == 0) return sp;
        const tree_t ** leaves = (const tree_t **)GC_malloc(nflat * sizeof(const tree_t *));
        flatten_cat_fill(t, leaves, 0);
        if (nflat == 1) return build_node(bbg, leaves[0], sp, fp);
        IR_t * chain = sp;
        IR_t ** entries = (IR_t **)GC_malloc(nflat * sizeof(IR_t *));
        for (int i = nflat - 1; i >= 0; i--) {
            IR_t * e = build_node(bbg, leaves[i], chain, fp);
            if (!e) return NULL;
            entries[i] = e;
            chain = e;
        }
        for (int i = 0; i < nflat - 1; i++) {
            IR_t * a = entries[i], * b = entries[i+1];
            if (a && b && b->ω == fp) b->ω = a->β ? a->β : fp;
        }
        return entries[0];
    }
    case TT_ALT: {
        if (t->n == 0) return fp;
        if (t->n == 1) return build_node(bbg, t->c[0], sp, fp);
        IR_t * alt_fail = fp;
        IR_t * first    = NULL;
        for (int i = t->n - 1; i >= 0; i--) {
            IR_t * e = build_node(bbg, t->c[i], sp, alt_fail);
            if (!e) return NULL;
            first    = e;
            alt_fail = e;
        }
        return first;
    }
    case TT_CAPT_COND_ASGN: {
        if (t->n < 1) return NULL;
        bb = IR_node_alloc(bbg, IR_PAT_ASSIGN_COND);
        bb->sval = (t->n > 1 && t->c[1] && t->c[1]->v.sval) ? t->c[1]->v.sval : NULL;
        bb->γ = sp;
        bb->ω = fp;
        IR_t * inner = build_node(bbg, t->c[0], bb, fp);
        if (!inner) return NULL;
        bb->α = inner;
        bb->β = inner->β;
        return bb;
    }
    case TT_CAPT_IMMED_ASGN: {
        if (t->n < 1) return NULL;
        bb = IR_node_alloc(bbg, IR_PAT_ASSIGN_IMM);
        bb->sval = (t->n > 1 && t->c[1] && t->c[1]->v.sval) ? t->c[1]->v.sval : NULL;
        bb->γ = sp;
        bb->ω = fp;
        IR_t * inner = build_node(bbg, t->c[0], bb, fp);
        if (!inner) return NULL;
        bb->α = inner;
        bb->β = inner->β;
        return bb;
    }
    case TT_CAPT_CURSOR: {
        if (t->n < 1 || !t->c[0] || !t->c[0]->v.sval) return NULL;
        bb = IR_node_alloc(bbg, IR_PAT_ATP);
        bb->sval = t->c[0]->v.sval;
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case TT_LEN: {
        if (t->n < 1 || !t->c[0]) return NULL;
        if (t->c[0]->t != TT_ILIT && t->c[0]->t != TT_VAR) return NULL;
        bb = IR_node_alloc(bbg, IR_PAT_LEN);
        if (t->c[0]->t == TT_VAR) { bb->sval = t->c[0]->v.sval; bb->dval = 1.0; }
        else { bb->ival = t->c[0]->v.ival; bb->dval = 0.0; }
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case TT_NOTANY: {
        if (t->n < 1 || !t->c[0]) return NULL;
        if (t->c[0]->t != TT_QLIT && t->c[0]->t != TT_VAR) {
            char * cs = cset_try_fold(t->c[0]); if (!cs) return NULL;
            bb = IR_node_alloc(bbg, IR_PAT_NOTANY);
            bb->sval = cs; bb->dval = 0.0;
            bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
            return bb;
        }
        bb = IR_node_alloc(bbg, IR_PAT_NOTANY);
        bb->sval = t->c[0]->v.sval ? t->c[0]->v.sval : "";
        bb->dval = (t->c[0]->t == TT_VAR) ? 1.0 : 0.0;
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case TT_POS: {
        if (t->n < 1 || !t->c[0]) return NULL;
        if (t->c[0]->t != TT_ILIT && t->c[0]->t != TT_VAR) return NULL;
        bb = IR_node_alloc(bbg, IR_PAT_POS);
        if (t->c[0]->t == TT_VAR) { bb->sval = t->c[0]->v.sval; bb->dval = 2.0; }
        else { bb->ival = t->c[0]->v.ival; bb->sval = NULL; bb->dval = 0.0; }
        bb->α = bb; bb->β = fp; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case TT_RPOS: {
        if (t->n < 1 || !t->c[0]) return NULL;
        if (t->c[0]->t != TT_ILIT && t->c[0]->t != TT_VAR) return NULL;
        bb = IR_node_alloc(bbg, IR_PAT_POS);
        if (t->c[0]->t == TT_VAR) { bb->sval = t->c[0]->v.sval; bb->dval = 1.0; }
        else { bb->ival = t->c[0]->v.ival; bb->sval = "r"; bb->dval = 0.0; }
        bb->α = bb; bb->β = fp; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case TT_TAB: {
        if (t->n < 1 || !t->c[0]) return NULL;
        if (t->c[0]->t != TT_ILIT && t->c[0]->t != TT_VAR) return NULL;
        bb = IR_node_alloc(bbg, IR_PAT_TAB);
        if (t->c[0]->t == TT_VAR) { bb->sval = t->c[0]->v.sval; bb->dval = 2.0; }
        else { bb->ival = t->c[0]->v.ival; bb->sval = NULL; bb->dval = 0.0; }
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case TT_RTAB: {
        if (t->n < 1 || !t->c[0]) return NULL;
        if (t->c[0]->t != TT_ILIT && t->c[0]->t != TT_VAR) return NULL;
        bb = IR_node_alloc(bbg, IR_PAT_TAB);
        if (t->c[0]->t == TT_VAR) { bb->sval = t->c[0]->v.sval; bb->dval = 1.0; }
        else { bb->ival = t->c[0]->v.ival; bb->sval = "r"; bb->dval = 0.0; }
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case TT_FNC: {
        if (!t->v.sval || t->n < 1 || !t->c[0]) return NULL;
        const char *fn = t->v.sval;
        const tree_t *arg = t->c[0];
        const char *sarg = (arg->t == TT_QLIT && arg->v.sval) ? arg->v.sval : NULL;
        int64_t iarg = (arg->t == TT_ILIT) ? arg->v.ival : 0;
        if (!strcmp(fn, "SPAN") && sarg) {
            bb = IR_node_alloc(bbg, IR_PAT_SPAN);
            bb->sval = sarg; bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp; return bb;
        }
        if (!strcmp(fn, "ANY") && sarg) {
            bb = IR_node_alloc(bbg, IR_PAT_ANY);
            bb->sval = sarg; bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp; return bb;
        }
        if (!strcmp(fn, "BREAK") && sarg) {
            bb = IR_node_alloc(bbg, IR_PAT_BREAK);
            bb->sval = sarg; bb->ival = 0; bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp; return bb;
        }
        if (!strcmp(fn, "BREAKX") && sarg) {
            bb = IR_node_alloc(bbg, IR_PAT_BREAK);
            bb->sval = sarg; bb->ival = 1; bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp; return bb;
        }
        if (!strcmp(fn, "NOTANY") && sarg) {
            bb = IR_node_alloc(bbg, IR_PAT_NOTANY);
            bb->sval = sarg; bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp; return bb;
        }
        if (!strcmp(fn, "LEN")) {
            bb = IR_node_alloc(bbg, IR_PAT_LEN);
            bb->ival = (arg->t == TT_ILIT) ? iarg : 0;
            bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp; return bb;
        }
        if (!strcmp(fn, "POS")) {
            bb = IR_node_alloc(bbg, IR_PAT_POS);
            bb->ival = iarg; bb->sval = NULL;
            bb->α = bb; bb->β = fp; bb->γ = sp; bb->ω = fp; return bb;
        }
        if (!strcmp(fn, "TAB")) {
            bb = IR_node_alloc(bbg, IR_PAT_TAB);
            bb->ival = iarg; bb->sval = NULL;
            bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp; return bb;
        }
        if (!strcmp(fn, "ARBNO") && t->n == 1) {
            IR_graph_t *inner_blk = IR_alloc(count_tree(arg) * 8 + 32, IR_LANG_SNO);
            if (!inner_blk) return NULL;
            IR_t *inner_entry = build_node(inner_blk, arg, NULL, NULL);
            if (!inner_entry) { IR_free(inner_blk); return NULL; }
            inner_blk->entry = inner_entry;
            bb = IR_node_alloc(bbg, IR_PAT_ARBNO);
            bb_arbno_state_t * az = (bb_arbno_state_t *)GC_MALLOC(sizeof *az);
            az->inner = inner_blk;
            az->pos_stack = (int *)GC_MALLOC(64 * sizeof(int));
            az->cap = 64;
            az->saved_delta = 0;
            bb->counter = (int64_t)(intptr_t)az;
            bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp; return bb;
        }
        return NULL;
    }
    case TT_VAR: {
        if (!t->v.sval || !t->v.sval[0]) return NULL;
        bb = IR_node_alloc(bbg, IR_PAT_DEFER);
        bb->sval = t->v.sval;
        bb->ival = 0;
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case TT_DEFER: {
        if (t->n < 1 || !t->c[0] || !t->c[0]->v.sval) return NULL;
        bb = IR_node_alloc(bbg, IR_PAT_DEFER);
        bb->sval = t->c[0]->v.sval;
        bb->ival = 1;
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    default:
        return NULL;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * IR_lower_pat(const tree_t * pat_tree) {
    if (!pat_tree) return NULL;
    int cap = count_tree(pat_tree) * 8 + 32;
    IR_graph_t * bbg = IR_alloc(cap, IR_LANG_SNO);
    if (!bbg) return NULL;
    IR_t * entry = build_node(bbg, pat_tree, NULL, NULL);
    if (!entry) { IR_free(bbg); return NULL; }
    bbg->entry = entry;
    return bbg;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int count_patnd(const PATND_t * p) {
    if (!p) return 0;
    int n = 1;
    for (int i = 0; i < p->nchildren; i++) n += count_patnd(p->children ? p->children[i] : NULL);
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * build_patnd(IR_graph_t * bbg, PATND_t * pp, IR_t * sp, IR_t * fp) {
    if (!pp) return sp;
    IR_t * bb = NULL;
    switch (pp->kind) {
    case XCHR: {
        bb = IR_node_alloc(bbg, IR_PAT_LIT);
        bb->sval = pp->STRVAL_fn ? pp->STRVAL_fn : "";
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case XSPNC: {
        bb = IR_node_alloc(bbg, IR_PAT_SPAN);
        bb->sval = pp->STRVAL_fn ? pp->STRVAL_fn : "";
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case XBRKC: {
        bb = IR_node_alloc(bbg, IR_PAT_BREAK);
        bb->sval = pp->STRVAL_fn ? pp->STRVAL_fn : "";
        bb->ival = 0;
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case XBRKX: {
        bb = IR_node_alloc(bbg, IR_PAT_BREAK);
        bb->sval = pp->STRVAL_fn ? pp->STRVAL_fn : "";
        bb->ival = 1;
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case XANYC: {
        bb = IR_node_alloc(bbg, IR_PAT_ANY);
        bb->sval = pp->STRVAL_fn ? pp->STRVAL_fn : "";
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case XNNYC: {
        bb = IR_node_alloc(bbg, IR_PAT_NOTANY);
        bb->sval = pp->STRVAL_fn ? pp->STRVAL_fn : "";
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case XLNTH: {
        bb = IR_node_alloc(bbg, IR_PAT_LEN);
        bb->ival = pp->num;
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case XPOSI: {
        bb = IR_node_alloc(bbg, IR_PAT_POS);
        bb->ival = pp->num; bb->sval = NULL;
        bb->α = bb; bb->β = fp; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case XRPSI: {
        bb = IR_node_alloc(bbg, IR_PAT_POS);
        bb->ival = pp->num; bb->sval = "r";
        bb->α = bb; bb->β = fp; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case XTB: {
        bb = IR_node_alloc(bbg, IR_PAT_TAB);
        bb->ival = pp->num; bb->sval = NULL;
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case XRTB: {
        bb = IR_node_alloc(bbg, IR_PAT_TAB);
        bb->ival = pp->num; bb->sval = "r";
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case XFARB: {
        bb = IR_node_alloc(bbg, IR_PAT_ARB);
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case XSTAR: {
        bb = IR_node_alloc(bbg, IR_PAT_REM);
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case XEPS: {
        bb = IR_node_alloc(bbg, IR_PAT_LIT);
        bb->sval = "";
        bb->α = bb; bb->β = fp; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case XFAIL: {
        bb = IR_node_alloc(bbg, IR_FAIL);
        bb->α = bb; bb->β = fp; bb->γ = fp; bb->ω = fp;
        return bb;
    }
    case XABRT: {
        bb = IR_node_alloc(bbg, IR_PAT_ABORT);
        bb->α = bb; bb->β = fp; bb->γ = fp; bb->ω = fp;
        return bb;
    }
    case XFNCE: {
        IR_t * inner = sp;
        if (pp->nchildren > 0 && pp->children && pp->children[0]) {
            inner = build_patnd(bbg, pp->children[0], sp, fp);
            if (!inner) return NULL;
        }
        bb = IR_node_alloc(bbg, IR_PAT_FENCE);
        bb->α = bb; bb->β = bb;
        bb->γ = inner ? inner : sp; bb->ω = fp;
        return bb;
    }
    case XCAT: {
        if (pp->nchildren == 0) return sp;
        if (pp->nchildren == 1) return build_patnd(bbg, pp->children[0], sp, fp);
        IR_t * chain = sp;
        IR_t ** entries = (IR_t **)GC_malloc(pp->nchildren * sizeof(IR_t *));
        for (int i = pp->nchildren - 1; i >= 0; i--) {
            IR_t * e = build_patnd(bbg, pp->children[i], chain, fp);
            if (!e) return NULL;
            entries[i] = e;
            chain = e;
        }
        for (int i = 0; i < pp->nchildren - 1; i++) {
            IR_t * a = entries[i], * b = entries[i+1];
            if (a && b && b->ω == fp) b->ω = a->β ? a->β : fp;
        }
        return entries[0];
    }
    case XOR: {
        if (pp->nchildren == 0) return fp;
        if (pp->nchildren == 1) return build_patnd(bbg, pp->children[0], sp, fp);
        IR_t * alt_fail = fp;
        IR_t * first    = NULL;
        for (int i = pp->nchildren - 1; i >= 0; i--) {
            IR_t * e = build_patnd(bbg, pp->children[i], sp, alt_fail);
            if (!e) return NULL;
            first    = e;
            alt_fail = e;
        }
        return first;
    }
    case XARBN: {
        if (pp->nchildren < 1 || !pp->children || !pp->children[0]) return NULL;
        int inner_cap = count_patnd(pp->children[0]) * 8 + 16;
        IR_graph_t * inner_blk = IR_alloc(inner_cap, IR_LANG_SNO);
        if (!inner_blk) return NULL;
        IR_t * inner_entry = build_patnd(inner_blk, pp->children[0], NULL, NULL);
        if (!inner_entry) { IR_free(inner_blk); return NULL; }
        inner_blk->entry = inner_entry;
        bb = IR_node_alloc(bbg, IR_PAT_ARBNO);
        bb_arbno_state_t * az = (bb_arbno_state_t *)GC_MALLOC(sizeof *az);
        az->inner = inner_blk;
        az->pos_stack = (int *)GC_MALLOC(64 * sizeof(int));
        az->cap = 64;
        az->saved_delta = 0;
        bb->counter = (int64_t)(intptr_t)az;
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case XDSAR: {
        bb = IR_node_alloc(bbg, IR_PAT_DEFER);
        bb->sval = pp->STRVAL_fn ? pp->STRVAL_fn : "";
        bb->ival = 0;
        bb->α = bb; bb->β = fp; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case XFNME: {
        if (pp->nchildren < 1 || !pp->children || !pp->children[0]) return NULL;
        bb = IR_node_alloc(bbg, IR_PAT_ASSIGN_IMM);
        bb->sval = (pp->STRVAL_fn && pp->STRVAL_fn[0]) ? pp->STRVAL_fn :
                   ((pp->var.v == DT_N && pp->var.s) ? pp->var.s : NULL);
        bb->γ = sp;
        bb->ω = fp;
        IR_t * inner = build_patnd(bbg, pp->children[0], bb, fp);
        if (!inner) return NULL;
        bb->α = inner;
        bb->β = inner->β;
        return bb;
    }
    case XNME: {
        if (pp->nchildren < 1 || !pp->children || !pp->children[0]) return NULL;
        bb = IR_node_alloc(bbg, IR_PAT_ASSIGN_COND);
        bb->sval = (pp->STRVAL_fn && pp->STRVAL_fn[0]) ? pp->STRVAL_fn :
                   ((pp->var.v == DT_N && pp->var.s) ? pp->var.s : NULL);
        bb->γ = sp;
        bb->ω = fp;
        IR_t * inner = build_patnd(bbg, pp->children[0], bb, fp);
        if (!inner) return NULL;
        bb->α = inner;
        bb->β = inner->β;
        return bb;
    }
    case XATP: {
        if (!pp->STRVAL_fn || pp->STRVAL_fn[0] != '@') return NULL;
        if (pp->nargs < 1 || !pp->args || !pp->args[0].s || !pp->args[0].s[0]) return NULL;
        bb = IR_node_alloc(bbg, IR_PAT_ATP);
        bb->sval = pp->args[0].s;
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    default:
        return NULL;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * patnd_to_bb_graph(PATND_t * pp) {
    if (!pp) return NULL;
    int cap = count_patnd(pp) * 8 + 32;
    IR_graph_t * bbg = IR_alloc(cap, IR_LANG_SNO);
    if (!bbg) return NULL;
    IR_t * entry = build_patnd(bbg, pp, NULL, NULL);
    if (!entry) { IR_free(bbg); return NULL; }
    bbg->entry = entry;
    return bbg;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void tree_set_kids(IR_t * nd, IR_t ** ch, int n) {
    if (!nd || n <= 0) return;
    bb_pat_kids_state_t * zk = (bb_pat_kids_state_t *)GC_MALLOC(sizeof *zk);
    if (!zk) return;
    zk->kids = (IR_t **)GC_MALLOC((size_t)n * sizeof(IR_t *));
    if (!zk->kids) return;
    for (int i = 0; i < n; i++) zk->kids[i] = ch[i];
    zk->nkids = n;
    nd->counter = (int64_t)(intptr_t)zk;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * build_patnd_tree(IR_graph_t * bbg, PATND_t * pp) {
    if (!pp) return NULL;
    switch (pp->kind) {
    case XCAT: {
        if (pp->nchildren == 0) return NULL;
        if (pp->nchildren == 1) return build_patnd_tree(bbg, pp->children[0]);
        IR_t * nd = IR_node_alloc(bbg, IR_PAT_CAT);
        if (!nd) return NULL;
        IR_t ** kids = (IR_t **)GC_MALLOC((size_t)pp->nchildren * sizeof(IR_t *));
        if (!kids) return NULL;
        for (int i = 0; i < pp->nchildren; i++) {
            kids[i] = build_patnd_tree(bbg, pp->children[i]);
            if (!kids[i]) return NULL;
        }
        tree_set_kids(nd, kids, pp->nchildren);
        nd->α = nd; nd->β = nd; nd->γ = NULL; nd->ω = NULL;
        return nd;
    }
    case XOR: {
        if (pp->nchildren == 0) return NULL;
        if (pp->nchildren == 1) return build_patnd_tree(bbg, pp->children[0]);
        IR_t * nd = IR_node_alloc(bbg, IR_PAT_ALT);
        if (!nd) return NULL;
        IR_t ** kids = (IR_t **)GC_MALLOC((size_t)pp->nchildren * sizeof(IR_t *));
        if (!kids) return NULL;
        for (int i = 0; i < pp->nchildren; i++) {
            kids[i] = build_patnd_tree(bbg, pp->children[i]);
            if (!kids[i]) return NULL;
        }
        tree_set_kids(nd, kids, pp->nchildren);
        nd->α = nd; nd->β = nd; nd->γ = NULL; nd->ω = NULL;
        return nd;
    }
    case XFNCE: {
        IR_t * nd = IR_node_alloc(bbg, IR_PAT_FENCE);
        if (!nd) return NULL;
        nd->α = nd; nd->β = nd; nd->γ = NULL; nd->ω = NULL;
        if (pp->nchildren > 0 && pp->children && pp->children[0]) {
            IR_t * inner = build_patnd_tree(bbg, pp->children[0]);
            if (!inner) return NULL;
            IR_t * one[1] = { inner };
            tree_set_kids(nd, one, 1);
        }
        return nd;
    }
    case XFNME: {
        if (pp->nchildren < 1 || !pp->children || !pp->children[0]) return NULL;
        IR_t * inner = build_patnd_tree(bbg, pp->children[0]);
        if (!inner) return NULL;
        IR_t * nd = IR_node_alloc(bbg, IR_PAT_ASSIGN_IMM);
        if (!nd) return NULL;
        nd->sval = (pp->STRVAL_fn && pp->STRVAL_fn[0]) ? pp->STRVAL_fn :
                   ((pp->var.v == DT_N && pp->var.s) ? pp->var.s : NULL);
        nd->α = nd; nd->β = nd; nd->γ = NULL; nd->ω = NULL;
        IR_t * one[1] = { inner };
        tree_set_kids(nd, one, 1);
        return nd;
    }
    case XNME: {
        if (pp->nchildren < 1 || !pp->children || !pp->children[0]) return NULL;
        IR_t * inner = build_patnd_tree(bbg, pp->children[0]);
        if (!inner) return NULL;
        IR_t * nd = IR_node_alloc(bbg, IR_PAT_ASSIGN_COND);
        if (!nd) return NULL;
        nd->sval = (pp->STRVAL_fn && pp->STRVAL_fn[0]) ? pp->STRVAL_fn :
                   ((pp->var.v == DT_N && pp->var.s) ? pp->var.s : NULL);
        nd->α = nd; nd->β = nd; nd->γ = NULL; nd->ω = NULL;
        IR_t * one[1] = { inner };
        tree_set_kids(nd, one, 1);
        return nd;
    }
    case XARBN: {
        if (pp->nchildren < 1 || !pp->children || !pp->children[0]) return NULL;
        int inner_cap = count_patnd(pp->children[0]) * 8 + 16;
        IR_graph_t * inner_blk = IR_alloc(inner_cap, IR_LANG_SNO);
        if (!inner_blk) return NULL;
        IR_t * inner_entry = build_patnd_tree(inner_blk, pp->children[0]);
        if (!inner_entry) { IR_free(inner_blk); return NULL; }
        inner_blk->entry = inner_entry;
        IR_t * nd = IR_node_alloc(bbg, IR_PAT_ARBNO);
        if (!nd) return NULL;
        bb_arbno_state_t * az = (bb_arbno_state_t *)GC_MALLOC(sizeof *az);
        if (!az) return NULL;
        az->kids = (IR_t **)GC_MALLOC(sizeof(IR_t *));
        if (!az->kids) return NULL;
        az->kids[0] = inner_entry;
        az->nkids = 1;
        az->inner = inner_blk;
        az->pos_stack = (int *)GC_MALLOC(64 * sizeof(int));
        az->cap = 64;
        az->saved_delta = 0;
        nd->counter = (int64_t)(intptr_t)az;
        nd->α = nd; nd->β = nd; nd->γ = NULL; nd->ω = NULL;
        return nd;
    }
    default:
        return build_patnd(bbg, pp, NULL, NULL);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * patnd_to_bb_tree(PATND_t * pp) {
    if (!pp) return NULL;
    int cap = count_patnd(pp) * 8 + 32;
    IR_graph_t * bbg = IR_alloc(cap, IR_LANG_SNO);
    if (!bbg) return NULL;
    IR_t * entry = build_patnd_tree(bbg, pp);
    if (!entry) { IR_free(bbg); return NULL; }
    bbg->entry = entry;
    return bbg;
}
/*====================================================================================================================================================================================================*/
/* ===== LOWER CONTEXT (kw_canonicalize, expression_scope_walk) ===== */
/*====================================================================================================================================================================================================*/
char *kw_canonicalize(const char *raw)
{
    if (!raw) raw = "";
    size_t n = strlen(raw);
    char *buf = GC_MALLOC(n + 1);
    for (size_t i = 0; i < n; i++) buf[i] = (char)toupper((unsigned char)raw[i]);
    buf[n] = '\0';
    return buf;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void expression_scope_walk(Scope *sc, tree_t *e)
{
    if (!e) return;
    if (e->t == TT_GLOBAL) {
        for (int i = 0; i < e->n; i++)
            if (e->c[i] && e->c[i]->v.sval)
                scope_add(sc, e->c[i]->v.sval);
        return;
    }
    if (e->t == TT_INITIAL) return;
    if (e->t == TT_VAR && e->v.sval && e->v.sval[0] != '&' && !is_global(e->v.sval))
        scope_add(sc, e->v.sval);
    for (int i = 0; i < e->n; i++)
        expression_scope_walk(sc, e->c[i]);
}
