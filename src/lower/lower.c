#define BB_DEFINE_NAMES
#include "lower.h"
#include "lower_ctx.h"
#include "../../frontend/snobol4/scrip_cc.h"
#include "../ast/ast.h"
#include "ast_clone.h"
#include "lower_pat_dcg.h"
#include "lower_graph.h"
#include "lower_clause.h"
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
            BB_graph_t *_irb = lower_proc_body(proc);
            g_stage2.proc_table[pi].bb_idx = _irb ? bb_program_add(&g_stage2.bbp, _irb) : -1;
            g_stage2.proc_table[pi].is_generator = 0;
            if (_irb) {
                for (int _k = 0; _k < _irb->n; _k++) {
                    if (_irb->all[_k] && _irb->all[_k]->t == BB_SUSPEND) { g_stage2.proc_table[pi].is_generator = 1; break; }
                }
            }
            continue;
        }
        if (proc) {
            int body_start = 0;
            Scope sc; build_proc_scope(&sc, proc, body_start);
            g_stage2.proc_table[pi].lower_sc = sc;
            BB_graph_t *_irb = lower_proc_body(proc);
            g_stage2.proc_table[pi].bb_idx = _irb ? bb_program_add(&g_stage2.bbp, _irb) : -1;
            g_stage2.proc_table[pi].is_generator = 0;
            if (_irb) {
                for (int _k = 0; _k < _irb->n; _k++) {
                    if (_irb->all[_k] && _irb->all[_k]->t == BB_SUSPEND) { g_stage2.proc_table[pi].is_generator = 1; break; }
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
            BB_graph_t *ir_body = lower_pl_predicate(e->choice);
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
