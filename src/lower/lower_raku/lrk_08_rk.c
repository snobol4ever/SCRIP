#include "lower/lower_raku/lrk_head.h"
#include <string.h>
#include <ctype.h>
#include "ir/ir_node.h"
#include "ir/ir_graph.h"
#include "ir/stage2.h"
#include "parsers/snobol4/scrip_cc.h"
#include "runtime/builtins/gen_runtime.h"
#include "lower/lower_common.h"
#include "lower/lower_raku/lrk_01_sh.h"
#include "lower/lower_raku/lrk_03_rk.h"
#include "lower/lower_raku/lrk_04_rk.h"
#include "lower/lower_raku/lrk_06_rk.h"
#include "lower/lower_raku/lrk_07_rk.h"
#include "lower/lower_raku/lrk_08_rk.h"
#include "lower/lower_raku/lrk_09_sh.h"
#include "lower/lower_raku/lrk_11_rk.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * rk_tern_branch(rcx_t * cx, const tree_t * br, IR_t * at, IR_t * ω) {
    extern int is_global(const char *);
    IR_t * r = NULL;
    const tree_t * v = (br && br->t == TT_ASSIGN && br->n >= 2) ? br->c[0] : NULL;
    if (v && v->t == TT_VAR && v->v.sval && is_global(v->v.sval)) {
        IR_t * gv = lrk_build(cx, IR_VAR, at, ω);
        IR_LIT(gv).sval = v->v.sval;
        ir_operand_push(at, gv);
        return lower_rv(cx, br, gv, ω, &r);
    }
    IR_t * e = lower_rv(cx, br, at, ω, &r);
    if (r) ir_operand_push(at, r);
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * rk_state_flag(const char * vn) { char fl[strlen(vn) + 8]; snprintf(fl, sizeof fl, "%s_init", (vn[0] == '@' || vn[0] == '%') ? vn + 1 : vn); return intern(fl); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t * rk_state_desugar(const tree_t * t) {
    const char * vn = t->c[0]->v.sval;
    const char * fn = rk_state_flag(vn);
    tree_t * set = ast_node_new(TT_ASSIGN);
    ast_push(set, leaf_sval2(TT_VAR, fn));
    ast_push(set, rk_ti(1));
    tree_t * once = ast_node_new(TT_SEQ_EXPR);
    ast_push(once, set);
    ast_push(once, t->c[1]);
    tree_t * un = ast_node_new(TT_UNLESS);
    ast_push(un, leaf_sval2(TT_VAR, fn));
    ast_push(un, once);
    tree_t * seq = ast_node_new(TT_SEQ_EXPR);
    seq->line = t->line;
    ast_push(seq, un);
    tree_t * dv = leaf_sval2(TT_VAR, vn);
    dv->slen = t->c[0]->slen;
    ast_push(seq, dv);
    return seq;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_diverges(const tree_t * t) { return t && (t->t == TT_RETURN || ((t->t == TT_SEQ_EXPR || t->t == TT_SEQ) && t->n > 0 && rk_diverges(t->c[t->n - 1]))); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_graph_assigns(const IR_graph_t * g, const char * nm) {
    for (int i = 0; g && i < g->n; i++) { IR_t * m = g->all[i]; if (m && m->op == IR_ASSIGN && IR_LIT(m).sval && !strcmp(IR_LIT(m).sval, nm)) return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_graph_param(const IR_graph_t * g, const char * nm) { for (int i = 0; g && g->pnames && i < g->nparams; i++) if (g->pnames[i] && !strcmp(g->pnames[i], nm)) return 1; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_file_scope_reads_are_globals(void) {
    extern void global_register(const char * name);
    extern int is_global(const char *);
    IR_graph_t * mg = NULL;
    for (int pi = 0; pi < g_stage2.proc_count; pi++) {
        int bi = g_stage2.proc_table[pi].bb_idx;
        if (bi >= 0 && bi < g_stage2.bbp.count && g_stage2.proc_table[pi].name && !strcmp(g_stage2.proc_table[pi].name, "main")) mg = g_stage2.bbp.table[bi];
    }
    if (!mg) return;
    for (int pi = 0; pi < g_stage2.proc_count; pi++) {
        int bi = g_stage2.proc_table[pi].bb_idx;
        if (bi < 0 || bi >= g_stage2.bbp.count || g_stage2.bbp.table[bi] == mg) continue;
        IR_graph_t * g = g_stage2.bbp.table[bi];
        for (int i = 0; g && i < g->n; i++) {
            IR_t * m = g->all[i];
            const char * nm = (m && (m->op == IR_VAR || m->op == IR_VAR_REF)) ? IR_LIT(m).sval : NULL;
            if (!nm || nm[0] == '&' || is_global(nm) || rk_graph_assigns(g, nm) || rk_graph_param(g, nm) || !rk_graph_assigns(mg, nm)) continue;
            int writer = 0;
            for (int pj = 0; pj < g_stage2.proc_count && !writer; pj++) {
                int bj = g_stage2.proc_table[pj].bb_idx;
                if (bj >= 0 && bj < g_stage2.bbp.count && g_stage2.bbp.table[bj] != mg && rk_graph_assigns(g_stage2.bbp.table[bj], nm)) writer = 1;
            }
            if (!writer) global_register(nm);
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_dyn_name(const char * nm) { return nm && (nm[0] == '*' || ((nm[0] == '@' || nm[0] == '%') && nm[1] == '*')); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_dyn_known(const char * nm) {
    static const char * const dyn[] = { "*ARGFILES", "*AWAITER", "*COLLATION", "*CWD", "*DEFAULT-READ-ELEMS", "*DISTRO", "*ERR", "*EXCEPTION", "*EXECUTABLE", "*EXECUTABLE-NAME", "*EXIT", "*GROUP",
        "*HOME", "*IN", "*INIT-INSTANT", "*KERNEL", "*OUT", "*PERL", "*PID", "*PROGRAM", "*PROGRAM-NAME", "*RAKU", "*RAT-OVERFLOW", "*REPO", "*SCHEDULER", "*SPEC", "*THREAD", "*TMPDIR", "*TOLERANCE",
        "*TZ", "*USER", "*VM", "*USAGE", "@*ARGS", "%*ENV", "&*chdir", "&*EXIT", NULL };
    for (int i = 0; dyn[i]; i++) if (!strcmp(nm, dyn[i])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_dyn_is(const tree_t * c, const char * nm) { return c && c->t == TT_VAR && c->v.sval && !strcmp(c->v.sval, nm); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_dyn_bound(const tree_t * t, const char * nm) {
    if (!t) return 0;
    if (t->t == TT_ASSIGN && t->n > 0 && rk_dyn_is(t->c[0], nm)) return 1;
    if (t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, "__rk_temp") && t->n > 1 && rk_dyn_is(t->c[1], nm)) return 1;
    for (int i = 0; i < t->n; i++) { if (t->t == TT_SUB_DECL && i > 0 && rk_dyn_is(t->c[i], nm)) return 1; if (rk_dyn_bound(t->c[i], nm)) return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_dyn_unfound(const tree_t * c, const tree_t * prog) {
    if (!c || c->t != TT_VAR || !rk_dyn_name(c->v.sval)) return 0;
    const char * s = strchr(c->v.sval, '*');
    return s && isalpha((unsigned char) s[1]) && !rk_dyn_known(c->v.sval) && !rk_dyn_bound(prog, c->v.sval);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * rk_unfound_first(const tree_t * t, const tree_t * prog) {
    for (int i = 0; t && i < t->n; i++) { if (rk_dyn_unfound(t->c[i], prog)) return t->c[i]->v.sval; const char * r = rk_unfound_first(t->c[i], prog); if (r) return r; }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_unfound_dynamics(tree_t * prog) {
    const char * nm;
    while (prog && (nm = rk_unfound_first(prog, prog))) {
        tree_t * as = ast_node_new(TT_ASSIGN), * at = ast_node_new(TT_ATTR), * st = ast_node_new(TT_STMT);
        ast_push(as, leaf_sval2(TT_VAR, nm));
        ast_push(as, rk_fnc2("__rk_dyn_notfound", leaf_sval2(TT_QLIT, nm), NULL));
        at->v.sval = (char *) ":subj";
        ast_push(at, as);
        ast_push(st, at);
        ast_push(prog, st);
        for (int k = prog->n - 1; k > 0; k--) prog->c[k] = prog->c[k - 1];
        prog->c[0] = st;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_unassigned_dynamics_are_globals(void) {
    extern void global_register(const char * name);
    extern int is_global(const char *);
    for (int pi = 0; pi < g_stage2.proc_count; pi++) {
        int bi = g_stage2.proc_table[pi].bb_idx;
        if (bi < 0 || bi >= g_stage2.bbp.count || !g_stage2.bbp.table[bi]) continue;
        IR_graph_t * g = g_stage2.bbp.table[bi];
        for (int i = 0; i < g->n; i++) {
            IR_t * m = g->all[i];
            const char * nm = (m && m->op == IR_VAR) ? IR_LIT(m).sval : NULL;
            if (!rk_dyn_name(nm) || is_global(nm) || rk_graph_param(g, nm)) continue;
            int assigned = 0;
            for (int pj = 0; pj < g_stage2.proc_count && !assigned; pj++) {
                int bj = g_stage2.proc_table[pj].bb_idx;
                if (bj >= 0 && bj < g_stage2.bbp.count && g_stage2.bbp.table[bj] && rk_graph_assigns(g_stage2.bbp.table[bj], nm)) assigned = 1;
            }
            if (!assigned) global_register(nm);
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * rk_eval_call_next(int * pi, int * i) {
    for (; *pi < g_stage2.proc_count; (*pi)++, *i = 0) {
        int bi = g_stage2.proc_table[*pi].bb_idx;
        if (bi < 0 || bi >= g_stage2.bbp.count || !g_stage2.bbp.table[bi]) continue;
        IR_graph_t * g = g_stage2.bbp.table[bi];
        while (*i < g->n) { IR_t * m = g->all[(*i)++]; if (m && (m->op == IR_CALL || m->op == IR_CALL_BUILTIN) && IR_LIT(m).sval && !strcmp(IR_LIT(m).sval, "__rk_eval")) return m; }
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_eval_main_globals(void) {
    extern void global_register(const char * name);
    extern int is_global(const char *);
    IR_graph_t * mg = NULL;
    for (int pi = 0; pi < g_stage2.proc_count; pi++) {
        int bi = g_stage2.proc_table[pi].bb_idx;
        if (bi >= 0 && bi < g_stage2.bbp.count && g_stage2.bbp.table[bi] && g_stage2.proc_table[pi].name && !strcmp(g_stage2.proc_table[pi].name, "main")) mg = g_stage2.bbp.table[bi];
    }
    int pi0 = 0, i0 = 0;
    if (!mg || !rk_eval_call_next(&pi0, &i0)) return;
    for (int i = 0; i < mg->n; i++) {
        IR_t * m = mg->all[i];
        const char * nm = (m && m->op == IR_ASSIGN) ? IR_LIT(m).sval : NULL;
        if (!nm || !nm[0] || nm[0] == '&' || !strncmp(nm, "__", 2) || is_global(nm)) continue;
        const char * bare = (nm[0] == '@' || nm[0] == '%') ? nm + 1 : nm;
        size_t bl = strlen(bare);
        int hit = 0, pj = 0, ij = 0;
        for (IR_t * c = rk_eval_call_next(&pj, &ij); c && !hit; c = rk_eval_call_next(&pj, &ij)) {
            IR_t * a0 = (c->n_operands > 0) ? c->operands[0] : NULL;
            if (!(a0 && a0->op == IR_LIT_STRING && IR_LIT(a0).sval)) { hit = 1; break; }
            for (const char * q = IR_LIT(a0).sval; *q && !hit; q++)
                if ((*q == '$' || *q == '@' || *q == '%') && !strncmp(q + 1, bare, bl) && !(isalnum((unsigned char) q[1 + bl]) || q[1 + bl] == '_' || q[1 + bl] == '-')) hit = 1;
        }
        if (hit) global_register(nm);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void lower_raku_eval_reads_are_globals(int pc0) {
    extern void global_register(const char * name);
    extern int is_global(const char *);
    for (int pi = pc0; pi < g_stage2.proc_count; pi++) {
        int bi = g_stage2.proc_table[pi].bb_idx;
        if (bi < 0 || bi >= g_stage2.bbp.count || !g_stage2.bbp.table[bi]) continue;
        IR_graph_t * g = g_stage2.bbp.table[bi];
        for (int i = 0; i < g->n; i++) {
            IR_t * m = g->all[i];
            const char * nm = (m && m->op == IR_VAR) ? IR_LIT(m).sval : NULL;
            if (!nm || !nm[0] || nm[0] == '&' || !strncmp(nm, "__", 2) || is_global(nm) || rk_graph_assigns(g, nm) || rk_graph_param(g, nm)) continue;
            global_register(nm);
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_register_state_globals(const tree_t * t) {
    extern void global_register(const char * name);
    extern int is_global(const char *);
    if (!t) return;
    if (t->t == TT_VAR && t->v.sval) { const char * n = t->v.sval; const char * b = (n[0] == '@' || n[0] == '%') ? n + 1 : n; if (!strncmp(b, "__rk_st", 7) && !is_global(n)) global_register(n); }
    if (t->t == TT_STATIC_DECL && t->n == 2 && t->c[0] && t->c[0]->v.sval) { const char * fn = rk_state_flag(t->c[0]->v.sval); if (!strncmp(fn, "__rk_st", 7) && !is_global(fn)) global_register(fn); }
    for (int i = 0; i < t->n; i++) rk_register_state_globals(t->c[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void raku_register_program(stage2_t * s2, const tree_t * prog) {
    extern int polyglot_module_open(stage2_t * s2, const tree_t * s);
    extern void polyglot_module_extend(stage2_t * s2, int mod_idx, const tree_t * s);
    extern void record_register(const char * spec);
    int mod_idx = -1;
    rk_register_state_globals(prog);
    for (int _ci = 0; _ci < prog->n; _ci++) {
        const tree_t * s = prog->c[_ci];
        if (!s || (s->t != TT_STMT && s->t != TT_END)) continue;
        if (mod_idx < 0) mod_idx = polyglot_module_open(s2, s);
        polyglot_module_extend(s2, mod_idx, s);
        tree_t * proc = stmt_attr_expr(stmt_attr_find(s, ":subj"));
        if (!proc) continue;
        if (proc->t == TT_GLOBAL) { for (int _gi = 0; _gi < proc->n; _gi++) if (proc->c[_gi] && proc->c[_gi]->v.sval) global_register(proc->c[_gi]->v.sval); }
        if (proc->t == TT_RECORD && proc->v.sval && *proc->v.sval) {
            size_t specn = strlen(proc->v.sval) + 3;
            for (int _ri = 0; _ri < proc->n; _ri++) specn += 1 + ((proc->c[_ri] && proc->c[_ri]->v.sval) ? strlen(proc->c[_ri]->v.sval) : 0);
            char spec[specn];
            int pos = 0;
            pos += snprintf(spec+pos, sizeof(spec)-pos, "%s(", proc->v.sval);
            for (int _ri = 0; _ri < proc->n && pos < (int)sizeof(spec)-2; _ri++) {
                if (_ri > 0) spec[pos++] = ',';
                const char *fn2 = (proc->c[_ri] && proc->c[_ri]->v.sval) ? proc->c[_ri]->v.sval : "";
                pos += snprintf(spec+pos, sizeof(spec)-pos, "%s", fn2);
            }
            if (pos < (int)sizeof(spec)-1) spec[pos++] = ')';
            spec[pos] = '\0';
            record_register(spec);
        }
        if (proc->t == TT_PROC_DECL) {
            const char *name = NULL;
            if (proc->t == TT_SUB_DECL) {
                if (proc->n > 0 && proc->c[0] && proc->c[0]->t == TT_VAR && proc->c[0]->v.sval && *proc->c[0]->v.sval) name = proc->c[0]->v.sval;
            } else {
                name = (proc->v.sval && *proc->v.sval) ? proc->v.sval :
                    ((proc->n > 0 && proc->c[0] && proc->c[0]->t == TT_VAR && proc->c[0]->v.sval && *proc->c[0]->v.sval) ? proc->c[0]->v.sval : NULL);
            }
            if (name) {
                int _pi = stage2_proc_grow(s2);
                s2->proc_table[_pi].name = name;
                s2->proc_table[_pi].proc = proc;
                s2->proc_table[_pi].entry_pc = -1;
                s2->proc_table[_pi].bb_idx = -1;
                s2->proc_table[_pi].nparams = (int)proc->v.ival;
                s2->proc_table[_pi].byref_mask = 0;
                if (mod_idx >= 0) s2->module_registry.mods[mod_idx].nprocs++;
                if (strcmp(name, "main") == 0 && s2->module_registry.main_mod < 0) s2->module_registry.main_mod = mod_idx;
            }
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_pkg_is(const tree_t * t) { return t && (t->t == TT_MODULE_DECL || t->t == TT_CLASS_DECL) && t->n > 0 && t->c[0] && t->c[0]->v.sval && t->c[0]->v.sval[0]; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_pkg_our_names(const tree_t * t, rk_ns_t * ours, rk_ns_t * mine) {
    if (!t) return;
    if (t->t == TT_VAR && t->v.sval && (t->slen & RK_VAR_OUR)) rk_ns_add(ours, t->v.sval);
    else if (t->t == TT_VAR && t->v.sval && (t->slen & 4)) rk_ns_add(mine, t->v.sval);
    for (int i = 0; i < t->n; i++) if (!rk_pkg_is(t->c[i])) rk_pkg_our_names(t->c[i], ours, mine);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_pkg_rename(tree_t * t, const char * v, const char * to) {
    if (!t) return;
    if (t->t == TT_VAR && t->v.sval && !strcmp(t->v.sval, v)) { t->v.sval = (char *) to; t->slen |= RK_VAR_PKG; }
    for (int i = 0; i < t->n; i++) if (!rk_pkg_is(t->c[i])) rk_pkg_rename(t->c[i], v, to);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_pkg_scope(tree_t * t, const char * path) {
    extern void global_register(const char * name);
    if (!t) return;
    if (rk_pkg_is(t)) {
        const char * mn = t->c[0]->v.sval;
        char np[(path ? strlen(path) + 2 : 0) + strlen(mn) + 1];
        snprintf(np, sizeof np, "%s%s%s", path ? path : "", path ? "::" : "", mn);
        const char * npp = lp_strdup(np);
        rk_ns_t ours = { 0 }, mine = { 0 };
        for (int i = 1; i < t->n; i++) if (!rk_pkg_is(t->c[i])) rk_pkg_our_names(t->c[i], &ours, &mine);
        for (int k = 0; k < ours.n; k++) {
            const char * v = ours.v[k];
            if (rk_ns_has(&mine, v)) continue;
            char sg = (v[0] == '@' || v[0] == '%') ? v[0] : 0;
            char nb[strlen(npp) + strlen(v) + 4];
            snprintf(nb, sizeof nb, "%.*s%s::%s", sg ? 1 : 0, v, npp, sg ? v + 1 : v);
            const char * to = lp_strdup(nb);
            for (int i = 1; i < t->n; i++) if (!rk_pkg_is(t->c[i])) rk_pkg_rename(t->c[i], v, to);
            global_register(to);
        }
        for (int i = 1; i < t->n; i++) rk_pkg_scope(t->c[i], npp);
        return;
    }
    for (int i = 0; i < t->n; i++) rk_pkg_scope(t->c[i], path);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_pkg_refs(tree_t * t) {
    extern void global_register(const char * name);
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_pkg_refs(t->c[i]);
    if (t->t != TT_VAR || !t->v.sval || !(t->slen & RK_VAR_PKG)) return;
    const char * v = t->v.sval;
    char sg = (v[0] == '@' || v[0] == '%') ? v[0] : 0;
    const char * b = sg ? v + 1 : v;
    const char * bare = !strncmp(b, "::", 2) ? b + 2 : !strncmp(b, "GLOBAL::", 8) ? b + 8 : NULL;
    if (bare && *bare && !strstr(bare, "::")) { char nb[strlen(bare) + 2]; snprintf(nb, sizeof nb, "%.*s%s", sg ? 1 : 0, v, bare); t->v.sval = (char *) lp_strdup(nb); t->slen &= ~RK_VAR_PKG; return; }
    global_register(lp_strdup(v));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_class_is_stub(const tree_t * t) { return t && t->t == TT_CLASS_DECL && t->n == 2 && t->c[1] && t->c[1]->t == TT_YADA && t->c[0] && t->c[0]->v.sval; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_class_has_full(const tree_t * t, const char * nm) {
    if (!t) return 0;
    if (t->t == TT_CLASS_DECL && !rk_class_is_stub(t) && t->n > 0 && t->c[0] && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, nm)) return 1;
    for (int i = 0; i < t->n; i++) if (rk_class_has_full(t->c[i], nm)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_class_stubs(const tree_t * prog, tree_t * t) {
    if (!t) return;
    if (rk_class_is_stub(t) && rk_class_has_full(prog, t->c[0]->v.sval)) { t->t = TT_SEQ_EXPR; t->v.sval = NULL; t->n = 0; return; }
    for (int i = 0; i < t->n; i++) rk_class_stubs(prog, t->c[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_class_member_is_decl(const tree_t * c) {
    if (!c) return 1;
    switch (c->t) {
        case TT_HAS_DECL:
        case TT_ARR_DECL:
        case TT_HASH_DECL:
        case TT_HANDLES_DECL:
        case TT_SUB_DECL:
        case TT_CLASS_DECL:
        case TT_ROLE_DECL:
        case TT_MODULE_DECL:
        case TT_GRAMMAR_DECL:
        case TT_PROC_DECL:
        case TT_RECORD_DECL:
        case TT_REGEX_DECL:
        case TT_RW_DECL:
        case TT_USE_DECL:
        case TT_YADA:
        return 1;
        case TT_VAR:
        return 1;
        case TT_DECL:
        return c->n > 1 && c->c[1] && c->c[1]->t == TT_VAR && c->c[1]->v.sval && (c->c[1]->v.sval[0] == '.' || c->c[1]->v.sval[0] == '!');
        default:
        return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_class_body(tree_t * t, int * seq) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_class_body(t->c[i], seq);
    for (int i = 0; t->t != TT_ATTR && i < t->n; i++) {
        tree_t * st = t->c[i];
        tree_t * d = (st && st->t == TT_STMT) ? (tree_t *) lrk_stmt_subj(st) : st;
        if (!d || (d->t != TT_CLASS_DECL && d->t != TT_ROLE_DECL)) continue;
        int nb = 0;
        for (int j = 1; j < d->n; j++) if (!rk_class_member_is_decl(d->c[j])) nb++;
        if (!nb) continue;
        rk_ns_t dn = { 0 };
        for (int j = 1; j < d->n; j++) if (!rk_class_member_is_decl(d->c[j])) rk_cap_declared(d->c[j], &dn);
        char sfx[32];
        snprintf(sfx, sizeof sfx, "__cb%d", (*seq)++);
        for (int k = 0; k < dn.n; k++) {
            const char * v = dn.v[k];
            if (!*v || v[0] == '&' || v[0] == '_' || strstr(v, "::")) continue;
            char nb2[strlen(v) + strlen(sfx) + 1];
            snprintf(nb2, sizeof nb2, "%s%s", v, sfx);
            rk_gw_rename(d, v, lp_strdup(nb2));
        }
        tree_t * body = ast_node_new(TT_SEQ_EXPR);
        body->line = d->line;
        int w = 1;
        for (int j = 1; j < d->n; j++) { if (rk_class_member_is_decl(d->c[j])) d->c[w++] = d->c[j]; else ast_push(body, d->c[j]); }
        d->n = w;
        tree_t * ns = body;
        if (st != d) {
            ns = ast_node_new(TT_STMT);
            ns->line = st->line;
            ns->v = st->v;
            ns->slen = st->slen;
            for (int q = 0; q < st->n; q++) {
                tree_t * a = st->c[q];
                if (!(a && a->t == TT_ATTR && a->v.sval && !strcmp(a->v.sval, ":subj"))) { ast_push(ns, a); continue; }
                tree_t * na = ast_node_new(TT_ATTR);
                na->v = a->v;
                na->line = a->line;
                ast_push(na, body);
                ast_push(ns, na);
            }
        }
        ast_push(t, ns);
        for (int k = t->n - 1; k > i + 1; k--) t->c[k] = t->c[k - 1];
        t->c[i + 1] = ns;
        i++;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_qs_count(const tree_t * t, const char * bare) {
    if (!t || t->t == TT_CLASS_DECL || t->t == TT_ROLE_DECL) return 0;
    int n = 0;
    if (t->t == TT_SUB_DECL && t->n > 0 && t->c[0] && t->c[0]->v.sval) { size_t bl = strcspn(t->c[0]->v.sval, "$"); if (bl == strlen(bare) && !strncmp(t->c[0]->v.sval, bare, bl)) n++; }
    for (int i = 0; i < t->n; i++) n += rk_qs_count(t->c[i], bare);
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_qs_has(const tree_t * t, const char * path, const char * modpath, const char * bare) {
    if (!t) return 0;
    if (t->t == TT_STMT) { const tree_t * sub = lrk_stmt_subj(t); return rk_qs_has(sub, path, modpath, bare); }
    if (t->t != TT_MODULE_DECL && t->t != TT_PROGRAM && t->t != TT_SEQ_EXPR) return 0;
    const char * mn = (t->t == TT_MODULE_DECL && t->n > 0 && t->c[0] && t->c[0]->v.sval) ? t->c[0]->v.sval : NULL;
    char np[(path ? strlen(path) : 0) + (mn ? strlen(mn) : 0) + 4];
    if (mn) snprintf(np, sizeof np, "%s%s%s", path ? path : "", path ? "::" : "", mn);
    else snprintf(np, sizeof np, "%s", path ? path : "");
    for (int i = mn ? 1 : 0; i < t->n; i++) {
        const tree_t * ch = t->c[i];
        if (ch && ch->t == TT_STMT) ch = lrk_stmt_subj(ch);
        if (!ch) continue;
        if (mn && !strcmp(np, modpath) && ch->t == TT_SUB_DECL && ch->n > 0 && ch->c[0] && ch->c[0]->v.sval) {
            size_t bl = strcspn(ch->c[0]->v.sval, "$");
            if (bl == strlen(bare) && !strncmp(ch->c[0]->v.sval, bare, bl)) return 1;
        }
        if (rk_qs_has(ch, mn ? np : path, modpath, bare)) return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * rk_qual_target(const tree_t * prog, const char * v, int * amp) {
    *amp = v && v[0] == '&';
    const char * q = v ? v + *amp : NULL;
    const char * last = q ? strrchr(q, ':') : NULL;
    if (!last || last == q || last[-1] != ':' || !last[1] || last - 1 == q) return NULL;
    char mp[(size_t) (last - 1 - q) + 1];
    memcpy(mp, q, (size_t) (last - 1 - q));
    mp[last - 1 - q] = 0;
    int pseudo = !strcmp(mp, "OUR") || !strcmp(mp, "MY") || !strcmp(mp, "GLOBAL") || !strcmp(mp, "UNIT");
    return (pseudo || rk_qs_has(prog, NULL, mp, last + 1)) && rk_qs_count(prog, last + 1) == 1 ? last + 1 : NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_qualified_sub_refs(const tree_t * prog, tree_t * t) {
    if (!t) return;
    int amp = 0;
    const char * bare = (t->t == TT_INVOKE && t->n >= 1 && t->c[0] && t->c[0]->t == TT_VAR) ? rk_qual_target(prog, t->c[0]->v.sval, &amp) : NULL;
    if (bare) {
        tree_t * f = ast_node_new(TT_FNC);
        f->v.sval = (char *) lp_strdup(bare);
        f->line = t->line;
        ast_push(f, leaf_sval2(TT_VAR, lp_strdup(bare)));
        for (int i = 1; i < t->n; i++) ast_push(f, t->c[i]);
        t->t = TT_FNC;
        t->v.sval = f->v.sval;
        t->n = 0;
        for (int i = 0; i < f->n; i++) ast_push(t, f->c[i]);
    }
    for (int i = 0; i < t->n; i++) rk_qualified_sub_refs(prog, t->c[i]);
    if (t->t != TT_VAR || !t->v.sval || !strchr(t->v.sval, ':')) return;
    bare = rk_qual_target(prog, t->v.sval, &amp);
    if (!bare) return;
    char nb[strlen(bare) + 2];
    snprintf(nb, sizeof nb, "%s%s", amp ? "&" : "", bare);
    t->v.sval = (char *) lp_strdup(nb);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_trans_flag_of(const char * nm) { return (nm && nm[0] && !nm[1] && strchr("cds", nm[0])) ? (nm[0] == 'c' ? 1 : nm[0] == 'd' ? 2 : 4) : 0; }
static int rk_is_mkbool(const tree_t * a, int args) { return a && a->t == TT_FNC && a->v.sval && !strcmp(a->v.sval, "__rk_mkbool") && a->n == args && a->c[1] && a->c[1]->t == TT_ILIT; }
void rk_trans_flags_walk(tree_t * t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_trans_flags_walk(t->c[i]);
    if (t->t != TT_METHCALL || t->n < 3 || !t->c[1] || t->c[1]->t != TT_QLIT || !t->c[1]->v.sval || strcmp(t->c[1]->v.sval, "trans")) return;
    int flags = 0, w = 2;
    for (int i = 2; i < t->n; i++) {
        const tree_t * a = t->c[i];
        if (rk_is_mkbool(a, 3) && a->c[2] && a->c[2]->t == TT_QLIT && rk_trans_flag_of(a->c[2]->v.sval)) { if (a->c[1]->v.ival) flags |= rk_trans_flag_of(a->c[2]->v.sval); continue; }
        if (a && a->t == TT_QLIT && rk_trans_flag_of(a->v.sval) && i + 1 < t->n && rk_is_mkbool(t->c[i + 1], 2)) { if (t->c[i + 1]->c[1]->v.ival) flags |= rk_trans_flag_of(a->v.sval); i++; continue; }
        t->c[w++] = t->c[i];
    }
    t->n = w;
    if (!flags) return;
    tree_t * fl = ast_node_new(TT_ILIT);
    fl->v.ival = flags;
    ast_push(t, rk_fnc2("__rk_pair", leaf_sval2(TT_QLIT, "__trans_flags"), fl));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_subst_adverb_mode(const tree_t * a) {
    if (!a || a->t != TT_FNC || !a->v.sval || strcmp(a->v.sval, "__rk_mkbool") || a->n != 3 || !a->c[1] || a->c[1]->t != TT_ILIT || !a->c[2] || a->c[2]->t != TT_QLIT || !a->c[2]->v.sval) return -1;
    if (!a->c[1]->v.ival) return 0;
    if (!strcmp(a->c[2]->v.sval, "g")) return 1;
    if (!strcmp(a->c[2]->v.sval, "ov")) return 2;
    if (!strcmp(a->c[2]->v.sval, "ex")) return 4;
    return -1;
}
static int rk_subst_x_range(const tree_t * v, long * lo, long * hi) {
    int o = (v->t == TT_TO && v->n >= 2) ? 0 : (v->t == TT_FNC && v->v.sval && !strcmp(v->v.sval, "__rk_range_val") && v->n >= 3) ? 1 : -1;
    if (o < 0 || !v->c[o] || !v->c[o + 1] || v->c[o]->t != TT_ILIT || v->c[o + 1]->t != TT_ILIT) return 0;
    *lo = v->c[o]->v.ival;
    *hi = v->c[o + 1]->v.ival;
    return 1;
}
void rk_subst_meth_walk(tree_t * t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_subst_meth_walk(t->c[i]);
    if (t->t != TT_METHCALL || t->n < 4 || !t->c[1] || t->c[1]->t != TT_QLIT || !t->c[1]->v.sval || strcmp(t->c[1]->v.sval, "subst")) return;
    tree_t * pat = t->c[2], * rep = t->c[3];
    int iscode = rk_closure_block(rep) || (rep && rep->t == TT_ANON_BLOCK && rep->v.sval);
    int rx = pat && pat->t == TT_FNC && pat->v.sval && !strcmp(pat->v.sval, "__rk_regex");
    if (!rx && !(pat && pat->t == TT_QLIT && pat->v.sval)) return;
    long mode = 128, xmin = 0, xmax = 0, nth[t->n];
    int hasx = 0, nnth = 0, zerox = 0;
    for (int i = 4; i < t->n; i++) {
        const tree_t * a = t->c[i];
        int m = rk_subst_adverb_mode(a);
        if (m >= 0) { mode |= m; continue; }
        const tree_t * v = (a && a->t == TT_FNC && a->v.sval && !strcmp(a->v.sval, "__rk_pair") && a->n == 3 && a->c[1] && a->c[1]->t == TT_QLIT && a->c[1]->v.sval) ? a->c[2] : NULL;
        if (!v) return;
        const char * nm = a->c[1]->v.sval;
        if (!strcmp(nm, "x") && v->t == TT_ILIT && v->v.ival == 0) { zerox = 1; continue; }
        if (!strcmp(nm, "x") && v->t == TT_ILIT && v->v.ival >= 1) { mode |= 32; xmin = xmax = v->v.ival; hasx = 1; continue; }
        long rlo = 0, rhi = 0;
        if (!strcmp(nm, "x") && rk_subst_x_range(v, &rlo, &rhi) && rlo >= 1) { mode |= 32; xmin = rlo; xmax = rhi; hasx = 1; continue; }
        if (!strcmp(nm, "x") && v->t == TT_VAR && v->v.sval && !strcmp(v->v.sval, "*")) { mode |= 32; xmin = 0; xmax = -1; hasx = 1; continue; }
        if (!strcmp(nm, "nth") && v->t == TT_ILIT && v->v.ival >= 1) { mode |= 64; nth[nnth++] = v->v.ival; continue; }
        return;
    }
    if (zerox) { tree_t * r = t->c[0]; t->t = r->t; t->v = r->v; t->slen = r->slen; t->n = 0; for (int i = 0; i < r->n; i++) ast_push(t, r->c[i]); return; }
    if (!iscode && !hasx && !nnth) return;
    if (!rx) {
        size_t n = strlen(pat->v.sval);
        char q[2 * n + 1];
        size_t k = 0;
        for (size_t i = 0; i < n; i++) {
            unsigned char c = (unsigned char) pat->v.sval[i];
            if (c < 128 && !((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')) q[k++] = '\\';
            q[k++] = (char) c;
        }
        q[k] = 0;
        pat = leaf_sval2(TT_QLIT, lp_strdup(q));
    }
    tree_t * recv = t->c[0];
    int line = t->line;
    t->t = TT_FNC;
    t->v.sval = (char *) "re_subst";
    t->n = 0;
    t->line = line;
    ast_push(t, leaf_sval2(TT_VAR, "re_subst"));
    ast_push(t, recv);
    ast_push(t, pat);
    ast_push(t, rep);
    long fixed[4] = { mode, 0, xmin, xmax };
    for (int i = 0; i < 4; i++) { tree_t * z = ast_node_new(TT_ILIT); z->v.ival = fixed[i]; ast_push(t, z); }
    for (int i = 0; i < nnth; i++) { tree_t * z = ast_node_new(TT_ILIT); z->v.ival = nth[i]; ast_push(t, z); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_alpha_name(char * out, size_t cap, const char * base, int n) {
    char sfx[16];
    int k = 0;
    for (; n > 0 && k < 14; n /= 26) sfx[k++] = (char) ('a' + (n - 1) % 26), n -= 1;
    sfx[k] = '\0';
    for (int i = 0, j = k - 1; i < j; i++, j--) { char c = sfx[i]; sfx[i] = sfx[j]; sfx[j] = c; }
    snprintf(out, cap, "%sZ%s", base, sfx);
}
static int rk_is_type_decl(const tree_t * d) { return d && (d->t == TT_CLASS_DECL || d->t == TT_ROLE_DECL || d->t == TT_GRAMMAR_DECL); }
static const char * rk_decl_name(const tree_t * d) { return (d->n > 0 && d->c[0] && d->c[0]->v.sval) ? d->c[0]->v.sval : NULL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_count_type_decls(const tree_t * t) { if (!t) return 0; int n = rk_is_type_decl(t) ? 1 : 0; for (int i = 0; i < t->n; i++) n += rk_count_type_decls(t->c[i]); return n; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_rename_type_refs(tree_t * t, const char * from, const char * to) {
    if (!t) return;
    if (t->t == TT_VAR && t->v.sval && !strcmp(t->v.sval, from)) t->v.sval = (char *) to;
    if (t->t == TT_NEW && t->n > 0 && t->c[0] && t->c[0]->t == TT_QLIT && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, from)) t->c[0]->v.sval = (char *) to;
    for (int i = 0; i < t->n; i++) rk_rename_type_refs(t->c[i], from, to);
}
static int rk_is_top_decl(rk_hoist_t * h, const tree_t * d) { for (int i = 0; i < h->ntop; i++) if (h->top[i] == d) return 1; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_hoist_types_walk(rk_hoist_t * h, tree_t * t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) {
        tree_t * c = t->c[i];
        if (rk_is_type_decl(c) && !rk_is_top_decl(h, c)) {
            const char * nm = rk_decl_name(c);
            int stmt_pos = t->t == TT_ATTR;
            char buf[(nm ? strlen(nm) : 0) + 32];
            if (!nm || !*nm) {
                rk_alpha_name(buf, sizeof buf, "AnonClass", ++h->uid);
                nm = lp_strdup(buf);
            } else {
                int dup = 0;
                for (int k = 0; k < h->nseen; k++) if (!strcmp(h->seen[k], nm)) dup = 1;
                if (dup || rk_is_class_name(nm)) { rk_alpha_name(buf, sizeof buf, nm, ++h->uid); const char * un = lp_strdup(buf); rk_rename_type_refs(t, nm, un); nm = un; }
            }
            if (c->n > 0 && c->c[0]) c->c[0]->v.sval = (char *) nm;
            h->seen[h->nseen++] = nm;
            h->out[h->nout++] = c;
            if (stmt_pos) t->c[i] = ast_node_new(TT_SEQ_EXPR);
            else { tree_t * v = ast_node_new(TT_VAR); v->v.sval = (char *) nm; t->c[i] = v; }
            rk_hoist_types_walk(h, c);
            continue;
        }
        rk_hoist_types_walk(h, c);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_hoist_nested_types(tree_t * prog) {
    if (!prog) return;
    int cap = rk_count_type_decls(prog) + 1;
    const tree_t * top[cap];
    const char * seen[cap];
    tree_t * out[cap];
    rk_hoist_t h = { top, 0, seen, 0, out, 0, 0 };
    for (int i = 0; i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) { const tree_t * sub = lrk_stmt_subj(d); if (sub) d = sub; }
        if (rk_is_type_decl(d)) { top[h.ntop++] = d; const char * nm = rk_decl_name(d); if (nm && *nm) seen[h.nseen++] = nm; }
    }
    rk_hoist_types_walk(&h, prog);
    for (int i = 0; i < h.nout; i++) ast_push(prog, out[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_role_nparams(const tree_t * rd) {
    int k = 0;
    for (int i = 0; rd && rd->n > 0 && rd->c[0] && i < rd->c[0]->n; i++) { const tree_t * q = rd->c[0]->c[i]; if (q && q->t == TT_QLIT && q->v.sval && q->v.sval[0] == 'r') k++; }
    return k;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_role_subst_type(tree_t * t, const char * name, const char * ty) {
    if (!t) return;
    if (t->t == TT_NEW && t->n > 0 && t->c[0] && t->c[0]->t == TT_QLIT && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, name)) t->c[0]->v.sval = (char *) ty;
    if (t->t == TT_DECL && t->n > 1 && t->c[0] && t->c[0]->t == TT_VAR && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, name)) t->c[0]->v.sval = (char *) ty;
    for (int i = 0; i < t->n; i++) rk_role_subst_type(t->c[i], name, ty);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_role_specialized(tree_t * rd, const char * spec, const tree_t * args) {
    int k = rk_role_nparams(rd);
    const tree_t * vals[k > 0 ? k : 1];
    if (!args) {
        tree_t * any = ast_node_new(TT_FNC);
        any->v.sval = (char *) "__rk_typeobj";
        ast_push(any, leaf_sval2(TT_VAR, "__rk_typeobj"));
        ast_push(any, leaf_sval2(TT_QLIT, "Any"));
        for (int i = 0; i < k; i++) vals[i] = any;
    } else if (k == 1) vals[0] = args;
    else if (args && args->t == TT_FNC && args->v.sval && !strcmp(args->v.sval, "__rk_arr") && args->n - 1 == k) for (int i = 0; i < k; i++) vals[i] = args->c[1 + i];
    else return NULL;
    const char * names[k > 0 ? k : 1];
    int ni = 0;
    for (int i = 0; i < rd->c[0]->n; i++) {
        const tree_t * q = rd->c[0]->c[i];
        if (!q || q->t != TT_QLIT || !q->v.sval || q->v.sval[0] != 'r') continue;
        const char * nm = q->v.sval + 1;
        names[ni++] = intern(!strncmp(nm, "::", 2) ? nm + 2 : nm);
    }
    tree_t * cl = spec ? rk_clone_tree(rd) : rd;
    if (spec) cl->c[0]->v.sval = (char *) intern(spec);
    cl->c[0]->n = 0;
    for (int x = 0; x < k; x++) {
        const tree_t * tv = vals[x];
        if (!tv || tv->t != TT_FNC || !tv->v.sval || strcmp(tv->v.sval, "__rk_typeobj") || tv->n < 2 || !tv->c[1] || tv->c[1]->t != TT_QLIT || !tv->c[1]->v.sval) continue;
        for (int j = 1; j < cl->n; j++) rk_role_subst_type(cl->c[j], names[x], tv->c[1]->v.sval);
    }
    for (int j = 1; j < cl->n; j++) {
        tree_t * m = cl->c[j];
        if (!m || m->t != TT_SUB_DECL) continue;
        int at = m->v.ival > 0 ? (int) m->v.ival : 1;
        for (int x = k - 1; x >= 0; x--) {
            tree_t * v = leaf_sval2(TT_VAR, names[x]);
            v->slen |= 4;
            tree_t * as = ast_node_new(TT_ASSIGN);
            as->line = m->line;
            ast_push(as, v);
            ast_push(as, rk_clone_tree(vals[x]));
            ast_push(m, NULL);
            for (int y = m->n - 1; y > at; y--) m->c[y] = m->c[y - 1];
            m->c[at] = as;
        }
    }
    return cl;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_role_find(tree_t * t, const char * base) {
    if (!t) return NULL;
    tree_t * d = rk_top_subj(t);
    if (d && d->t == TT_ROLE_DECL && d->n > 0 && d->c[0] && d->c[0]->v.sval && !strcmp(d->c[0]->v.sval, base) && rk_role_nparams(d)) return d;
    for (int i = 0; i < t->n; i++) { tree_t * r = rk_role_find(t->c[i], base); if (r) return r; }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_role_spec_in(tree_t * t, tree_t * root, rk_ns_t * made) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) {
        tree_t * cd = rk_top_subj(t->c[i]);
        if (cd && cd != t->c[i] && cd->t != TT_CLASS_DECL) rk_role_spec_in(cd, root, made);
        if (!cd || cd->t != TT_CLASS_DECL || cd->n < 1 || !cd->c[0]) { if (cd == t->c[i]) rk_role_spec_in(t->c[i], root, made); continue; }
        for (int k = 1; k < cd->n; k++) rk_role_spec_in(cd->c[k], root, made);
        for (int tq = 0; tq < cd->c[0]->n; tq++) {
            tree_t * q = cd->c[0]->c[tq];
            if (!q || q->t != TT_QLIT || !q->v.sval || q->v.sval[0] != 'd' || q->n < 1) continue;
            const char * full = q->v.sval + 1;
            const char * br = strchr(full, '[');
            if (!br) continue;
            int seen = -1;
            for (int m = 0; m < made->n; m++) if (!strcmp(made->v[m], full)) seen = m;
            if (seen >= 0) { char sb[(size_t) (br - full) + 40]; snprintf(sb, sizeof sb, "d%.*s__rs%d", (int) (br - full), full, seen); q->v.sval = (char *) intern(sb); continue; }
            char base[br - full + 1];
            memcpy(base, full, (size_t) (br - full));
            base[br - full] = 0;
            tree_t * rd = rk_role_find(root, base);
            if (!rd) continue;
            char sn[(size_t) (br - full) + 40];
            snprintf(sn, sizeof sn, "%s__rs%d", base, made->n);
            const char * spec = intern(sn);
            tree_t * cl = rk_role_specialized(rd, spec, q->c[0]);
            if (!cl) continue;
            rk_ns_add(made, intern(full));
            { char dq[strlen(spec) + 2]; snprintf(dq, sizeof dq, "d%s", spec); q->v.sval = (char *) intern(dq); }
            tree_t * ins = cl;
            if (t->c[i]->t == TT_STMT) {
                extern tree_t *ast_stmt_new(tree_e kind);
                extern tree_t *ast_attr_int(const char *tag, int ival);
                extern tree_t *ast_attr_expr(const char *tag, tree_t *expr);
                ins = ast_stmt_new(TT_STMT);
                ast_push(ins, ast_attr_int(":line", 0));
                ast_push(ins, ast_attr_int(":stno", 0));
                ast_push(ins, ast_attr_expr(":subj", cl));
            }
            ast_push(t, NULL);
            for (int y = t->n - 1; y > i; y--) t->c[y] = t->c[y - 1];
            t->c[i] = ins;
            i++;
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_role_unbind(tree_t * t) {
    if (!t) return;
    tree_t * d = rk_top_subj(t);
    if (d && d->t == TT_ROLE_DECL && rk_role_nparams(d)) (void) rk_role_specialized(d, NULL, NULL);
    for (int i = 0; i < t->n; i++) rk_role_unbind(t->c[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_role_specialize(tree_t * prog) { if (!prog) return; rk_ns_t made = { 0 }; rk_role_spec_in(prog, prog, &made); rk_role_unbind(prog); }
