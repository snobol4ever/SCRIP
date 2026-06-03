#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <ctype.h>
#include <errno.h>
#include <libgen.h>
#include <setjmp.h>
#include <time.h>
#include <unistd.h>
#include <gc.h>
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
extern void ir_set_print_width(int w);
extern void ir_print_node_nl(const tree_t *e, FILE *f);
#include "core.h"
#include "sil_macros.h"
#include "runtime_shim.h"
#include "lower.h"
#include "IR_interp_state.h"
#include "bb_build.h"
#include "emit.h"
#include "emit_bb.h"
#include "emit_core.h"
#include "scrip_sm.h"
#include "sync_monitor.h"
extern DESCR_t pat_at_cursor(const char *varname);
static void stmt_init(void) {}
extern DESCR_t      eval_expr(const char *src);
extern const char  *exec_code(DESCR_t code_block);
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
/*====================================================================================================================*/
/*====================================================================================================================*/
static int icn_rt_arity(const IR_t *n) {
    switch (n->t) {
    case IR_LIT_I: case IR_LIT_S: case IR_LIT_F: case IR_LIT_NUL:
    case IR_VAR:   case IR_KEYWORD: return 0;
    case IR_BINOP: case IR_BINOP_GEN: return 2;
    case IR_TO:    case IR_TO_BY:     return 2;
    case IR_UNOP:  case IR_NEG: case IR_POS: case IR_NONNULL: case IR_NOT: case IR_SIZE: return 1;
    case IR_EVERY: return 1;
    case IR_CALL:  return (int)n->ival;
    default:       return -1;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * icn_ring_to_tree(IR_graph_t *g) {
    if (!g || !g->entry) return NULL;
    IR_t *chain[256]; int nc = 0;
    for (IR_t *cur = g->entry; cur && cur->t != IR_SUCCEED && cur->t != IR_FAIL && nc < 256; cur = cur->γ) chain[nc++] = cur;
    if (nc == 0 || nc >= 256) return NULL;
    for (int i = 0; i < nc; i++) if (chain[i]->t == IR_BINOP || chain[i]->t == IR_LIT_I || chain[i]->t == IR_LIT_S) return NULL;
    IR_t *stk[256]; int sp = 0;
    for (int i = 0; i < nc; i++) {
        IR_t *n = chain[i];
        int ar = icn_rt_arity(n);
        if (ar < 0 || ar > sp) return NULL;
        if (n->t == IR_CALL) {
            if (ar != 1) return NULL;
            n->α = stk[sp - 1]; sp -= 1;
            n->dval = 0.0;
        } else if (ar == 2) {
            n->β = stk[sp - 1];
            n->α = stk[sp - 2];
            sp -= 2;
        } else if (ar == 1) {
            n->α = stk[sp - 1]; sp -= 1;
        }
        stk[sp++] = n;
    }
    if (sp != 1) return NULL;
    return stk[0];
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int icn_kind_native_stub(IR_e t) {
    return t == IR_GEN_SCAN || t == IR_GEN_ALT || t == IR_KEYWORD || t == IR_PROC_GEN ||
           t == IR_CSET_UNION || t == IR_CSET_DIFF || t == IR_CSET_INTER || t == IR_CSET_COMPL ||
           t == IR_SUSPEND ||
           t == IR_LIST_BANG ||
           t == IR_ALT ||
           t == IR_TO || t == IR_TO_BY || t == IR_BINOP_GEN ||
           t == IR_MAP || t == IR_GREP;
}
static int icn_graph_native_emittable(stage2_t *s2) {
    if (!s2) return 0;
    for (int gi = 0; gi < s2->bbp.count; gi++) {
        IR_graph_t *g = s2->bbp.table[gi];
        if (!g || !g->all) continue;
        for (int ni = 0; ni < g->n; ni++) {
            IR_t *nd = g->all[ni];
            if (nd && icn_kind_native_stub(nd->t)) return 0;
        }
    }
    return 1;
}
/*====================================================================================================================*/
/*--------------------------------------------------------------------------------------------------------------------*/
static int pl_flat_arith_leaf_simple(const IR_t *o) {
    if (!o) return 0;
    return o->t == IR_LIT_I || o->t == IR_LOGICVAR;
}
static int pl_arith_op_floaty(const char *fn) {
    static const char *f[] = { "sqrt", "sin", "cos", "tan", "asin", "acos", "atan", "exp", "log",
                               "float", "float_integer_part", "float_fractional_part",
                               "truncate", "round", "ceiling", "floor", "integer", "/", NULL };
    for (int i = 0; f[i]; i++) if (!strcmp(fn, f[i])) return 1;
    return 0;
}
static int pl_flat_arith_leaf_float_ok(const IR_t *o) {
    if (!o) return 0;
    return o->t == IR_LIT_I || o->t == IR_LIT_F || o->t == IR_LOGICVAR;
}
static int pl_flat_goal_is_simple(const IR_t *g) {
    if (!g) return 0;
    switch (g->t) {
    case IR_SUCCEED: case IR_CUT: case IR_ATOM: return 1;
    case IR_BUILTIN: {
        const char *fn = g->sval ? g->sval : "";
        if (!strcmp(fn, "is")) {
            const IR_t *lhs = g->α, *rhs = g->β;
            if (!lhs || lhs->t != IR_LOGICVAR || !rhs) return 0;
            if (rhs->t == IR_ATOM)
                return rhs->sval && (!strcmp(rhs->sval, "pi") || !strcmp(rhs->sval, "e"));
            if (rhs->t != IR_ARITH) return 0;
            const char *rop = rhs->sval ? rhs->sval : "+";
            int floaty = pl_arith_op_floaty(rop)
                      || (rhs->α && rhs->α->t == IR_LIT_F)
                      || (rhs->β && rhs->β->t == IR_LIT_F);
            if (floaty) {
                if (rhs->α && rhs->β) return pl_flat_arith_leaf_float_ok(rhs->α) && pl_flat_arith_leaf_float_ok(rhs->β);
                if (rhs->α && !rhs->β) return pl_flat_arith_leaf_float_ok(rhs->α);
                return 0;
            }
            if (rhs->α && rhs->β) return pl_flat_arith_leaf_simple(rhs->α) && pl_flat_arith_leaf_simple(rhs->β);
            if (rhs->α && !rhs->β) return pl_flat_arith_leaf_simple(rhs->α);
            return 0;
        }
        int is_io = (!strcmp(fn, "write") || !strcmp(fn, "writeln") || !strcmp(fn, "print") || !strcmp(fn, "nl") || !strcmp(fn, "halt"));
        if (!is_io) return 0;
        if (g->ival >= 1) { const IR_t *a = g->α; if (!a || (a->t != IR_ATOM && a->t != IR_LIT_I && a->t != IR_LOGICVAR)) return 0; }
        return 1;
    }
    case IR_UNIFY: {
        const IR_t *l = g->α, *r = g->β;
        if (!l || !r) return 1;
        int l_var = (l->t == IR_LOGICVAR), r_var = (r->t == IR_LOGICVAR);
        int l_con = (l->t == IR_ATOM || l->t == IR_LIT_I);
        int r_con = (r->t == IR_ATOM || r->t == IR_LIT_I);
        return (l_var && r_con) || (l_con && r_var);
    }
    default: return 0;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_flat_body_root(IR_graph_t *g) {
    if (!g || !g->all) return NULL;
    IR_t *gconj = NULL;
    for (int i = 0; i < g->n; i++) {
        IR_t *nd = g->all[i];
        if (nd && nd->t == IR_GCONJ) { if (gconj) return NULL; gconj = nd; }
    }
    if (!gconj) {
        if (g->nslots > 0) return NULL;
        return (g->entry && g->entry->t == IR_SUCCEED) ? g->entry : NULL;
    }
    bb_conj_state_t *zs = (bb_conj_state_t *)(intptr_t)gconj->ival;
    if (!zs || !zs->goals || zs->ngoals <= 0) return NULL;
    for (int i = 0; i < zs->ngoals; i++) if (!pl_flat_goal_is_simple(zs->goals[i])) return NULL;
    return gconj;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
extern int resolve_bb_pred_count(void);
extern const char *resolve_bb_pred_name_at(int idx);
extern int resolve_bb_pred_arity_at(int idx);
extern IR_t *resolve_bb_entry_node(const char *name, int arity);
extern IR_graph_t *resolve_bb_graph_at(int idx);
static int pl_ite_then_branch_trivial(const IR_t *then_entry) {
    (void)then_entry;
    return 1;
}
static int pl_rich_node_emittable(const IR_t *nd) {
    if (!nd) return 1;
    switch (nd->t) {
    case IR_GCONJ: case IR_GOAL:
    case IR_CHOICE: case IR_DISJ:
    case IR_SUCCEED: case IR_FAIL: case IR_CUT:
    case IR_LOGICVAR: case IR_ATOM: case IR_STRUCT:
    case IR_LIT_I: case IR_LIT_F: case IR_LIT_S: case IR_LIT_NUL:
        return 1;
    case IR_ITE: {
        bb_ite_state_t *zi = (bb_ite_state_t *)(intptr_t)nd->ival;
        if (zi && !pl_ite_then_branch_trivial(zi->then_)) return 0;
        return 1;
    }
    case IR_UNIFY: {
        const IR_t *l = nd->α, *r = nd->β;
        int lk = l ? (int)l->t : -1, rk = r ? (int)r->t : -1;
        if (lk == IR_STRUCT || rk == IR_STRUCT || lk == IR_ARITH || rk == IR_ARITH
            || lk == IR_LIT_F || rk == IR_LIT_F) return 0;
        return 1;
    }
    case IR_ARITH:
        return 1;
    case IR_BUILTIN: {
        const char *fn = nd->sval ? nd->sval : "";
        if (!strcmp(fn, "is")) return pl_flat_goal_is_simple(nd);
        static const char *ok[] = { "write", "writeln", "print", "nl", "halt", NULL };
        for (int k = 0; ok[k]; k++) if (!strcmp(fn, ok[k])) return 1;
        static const char *acmp[] = { ">", "<", ">=", "=<", "<=", "=:=", "=\\=", NULL };
        for (int k = 0; acmp[k]; k++)
            if (!strcmp(fn, acmp[k])) return pl_flat_arith_leaf_simple(nd->α) && pl_flat_arith_leaf_simple(nd->β);
        if (nd->α && nd->β &&
            (!strcmp(fn,"==")||!strcmp(fn,"\\==")||!strcmp(fn,"@<")||!strcmp(fn,"@>")||!strcmp(fn,"@=<")||!strcmp(fn,"@>=")))
            return 1;
        static const char *ttest[] = { "var","nonvar","atom","atomic","number","integer",
                                        "float","compound","callable","is_list","ground", NULL };
        for (int k = 0; ttest[k]; k++) if (!strcmp(fn, ttest[k])) return nd->α != NULL;
        if (!strcmp(fn,"succ")) return nd->ival==2 && nd->α && nd->β;
        if (!strcmp(fn,"plus")) return nd->ival==3 && nd->α && nd->α->γ && nd->α->γ->γ;
        if (!strcmp(fn,"sort")||!strcmp(fn,"msort")) return nd->α && nd->α->γ;
        if (!strcmp(fn,"format")) return nd->α && (nd->ival==1 || nd->ival==2);
        if (!strcmp(fn,"numbervars")) return nd->ival==3 && nd->α && nd->α->γ && nd->α->γ->γ;
        if (!strcmp(fn,"copy_term")) return nd->α && nd->α->γ;
        static const char *atom2[] = { "atom_length","upcase_atom","downcase_atom","string_length",
            "string_upper","string_lower","atom_string","string_to_atom", NULL };
        for (int k = 0; atom2[k]; k++) if (!strcmp(fn, atom2[k])) return nd->α && nd->α->γ;
        if (!strcmp(fn,"atom_concat")||!strcmp(fn,"string_concat")) return nd->α && nd->α->γ && nd->α->γ->γ;
        static const char *achars[] = { "atom_chars","atom_codes","string_chars","string_codes", NULL };
        for (int k = 0; achars[k]; k++) if (!strcmp(fn, achars[k])) return nd->α && nd->α->γ;
        if (!strcmp(fn,"char_type")) return nd->ival==2 && nd->α && nd->α->γ;
        if (!strcmp(fn,"number_string")||!strcmp(fn,"atom_number")) return nd->α && nd->α->γ;
        if (!strcmp(fn,"functor")) return nd->ival==3 && nd->α && nd->α->γ && nd->α->γ->γ;
        if (!strcmp(fn,"arg")) return nd->ival==3 && nd->α && nd->α->γ && nd->α->γ->γ;
        if (!strcmp(fn,"=..")) return nd->α && nd->α->γ;
        if (!strcmp(fn,"term_to_atom")||!strcmp(fn,"term_string")) return nd->α && nd->α->γ;
        if (!strcmp(fn,"writeq")||!strcmp(fn,"write_canonical")) return nd->α != NULL;
        if (!strcmp(fn,"atomic_list_concat")||!strcmp(fn,"concat_atom")) return nd->α && (nd->ival==2 || nd->ival==3);
        return 0;
    }
    default:
        return 0;
    }
}
static int pl_rich_graph_ok(IR_graph_t *g) {
    if (!g || !g->all) return 0;
    for (int i = 0; i < g->n; i++) {
        IR_t *nd = g->all[i];
        if (!pl_rich_node_emittable(nd)) return 0;
        if (nd && nd->t == IR_CHOICE) {
            bb_choice_state_t *zc = (bb_choice_state_t *)(intptr_t)nd->ival;
            if (zc && zc->bodies)
                for (int b = 0; b < zc->nbodies; b++)
                    if (zc->bodies[b] && !pl_rich_graph_ok(zc->bodies[b])) return 0;
        }
    }
    return 1;
}
static IR_t * pl_rich_body_root(IR_graph_t *main_g) {
    if (!main_g || !main_g->entry) return NULL;
    if (!pl_rich_graph_ok(main_g)) return NULL;
    int npred = resolve_bb_pred_count();
    for (int i = 0; i < npred; i++) {
        const char *nm = resolve_bb_pred_name_at(i);
        if (!nm) continue;
        IR_graph_t *pg = resolve_bb_graph_at(i);
        if (!pg) continue;
        if (!pl_rich_graph_ok(pg)) return NULL;
    }
    if (main_g->body_root) return main_g->body_root;
    {
        IR_t *gconj = NULL;
        for (int i = 0; i < main_g->n; i++) {
            IR_t *nd = main_g->all[i];
            if (nd && nd->t == IR_GCONJ) { if (gconj) { gconj = NULL; break; } gconj = nd; }
        }
        if (gconj) return gconj;
    }
    return main_g->entry;
}
/*====================================================================================================================*/
int main(int argc, char **argv)
{
    if (argc >= 3 && strcmp(argv[1], "--audit-per-kind") == 0) {
        fprintf(stderr, "scrip: --audit-per-kind unavailable (audit tool unlinked)\n");
        return 1;
    }
    int mode_interp        = 0;
    int mode_run           = 0;
    int mode_compile       = 0;
    int mode_monitor       = 0;
    int bb_driver          = 0;
    int bb_live            = 0;
    int dump_ast           = 0;
    int dump_ast_bison     = 0;
    int dump_sm            = 0;
    int dump_bb            = 0;
    int dump_sno           = 0;
    int opt_trace          = 0;
    int opt_bench          = 0;
    const char * target_name = NULL;
    int argi = 1;
    while (argi < argc && argv[argi][0] == '-' && argv[argi][1] == '-') {
        if      (strcmp(argv[argi], "--interp")        == 0) { mode_interp        = 1; argi++; }
        else if (strcmp(argv[argi], "--run")           == 0) { mode_run           = 1; argi++; }
        else if (strcmp(argv[argi], "--compile")       == 0) { mode_compile       = 1; if (!target_name) target_name = "x86"; argi++; }
        else if (strcmp(argv[argi], "--monitor")       == 0) { mode_monitor       = 1; argi++; }
        else if (strncmp(argv[argi], "--target=", 9)   == 0) { target_name = argv[argi] + 9; mode_compile = 1; argi++; }
        else if (strcmp(argv[argi], "--bb=brokered")   == 0) { bb_driver          = 1; argi++; }
        else if (strcmp(argv[argi], "--bb=wired")      == 0) { bb_live            = 1; argi++; }
        else if (strcmp(argv[argi], "--dump-ast")      == 0) { dump_ast           = 1; argi++; }
        else if (strcmp(argv[argi], "--dump-ast-bison") == 0) { dump_ast_bison    = 1; argi++; }
        else if (strcmp(argv[argi], "--dump-sm")       == 0) { dump_sm            = 1; argi++; }
        else if (strcmp(argv[argi], "--dump-bb")       == 0) { dump_bb            = 1; argi++; }
        else if (strcmp(argv[argi], "--dump-sno")      == 0) { dump_sno           = 1; argi++; }
        else if (strcmp(argv[argi], "--dump-width")    == 0) {
            if (argi + 1 < argc) { ir_set_print_width(atoi(argv[++argi])); argi++; }
        }
        else if (strcmp(argv[argi], "--trace")         == 0) { opt_trace          = 1; argi++; }
        else if (strcmp(argv[argi], "--bench")         == 0) { opt_bench          = 1; argi++; }
        else if (strcmp(argv[argi], "--case-sensitive") == 0) { argi++; }
        else if (strcmp(argv[argi], "--fold-case")     == 0) {
            fprintf(stderr, "scrip: --fold-case is no longer supported; SCRIP is case-sensitive only\n");
            return 1;
        }
        else break;
    }
    int mode_compile_x86 = (mode_compile && target_name && strcmp(target_name, "x86") == 0);
    if (mode_compile_x86 && (mode_interp || mode_run || mode_monitor)) {
        fprintf(stderr,
            "scrip: --compile (x86) is mutually exclusive with "
            "--interp / --run / --monitor\n");
        return 1;
    }
    if (!mode_interp && !mode_run && !mode_monitor && !mode_compile)
        mode_run = 1;
    if (bb_driver && (mode_run || mode_compile)) {
        fprintf(stderr,
            "scrip: --bb=brokered is only valid under --interp; "
            "--run and --compile force --bb=wired\n");
        return 1;
    }
    if (bb_driver && bb_live) {
        fprintf(stderr,
            "scrip: --bb=brokered and --bb=wired are mutually exclusive\n");
        return 1;
    }
    if (!bb_driver && !bb_live) {
        if (mode_interp) bb_driver = 1;
        else             bb_live   = 1;
    }
    if (bb_live)   g_bb_mode = BB_MODE_LIVE;
    if (bb_driver) g_bb_mode = BB_MODE_BROKERED;
    if (argi >= argc) {
        fprintf(stderr,
            "usage: scrip [mode] [bb] [options] <file> [-- program-args...]\n"
            "\n"
            "Execution modes (default: --run):\n"
            "  --interp         walk the BB port-graph in-process (Icon)\n"
            "  --run            build flat-wired x86 BB blobs in a sealed slab and jump in  [DEFAULT]\n"
            "  --compile        emit standalone x86-64 asm to stdout (links libscrip_rt.so)\n"
            "  --target=ARCH    emit code for the named backend (x86, jvm, js, wasm); implies --compile\n"
            "  --monitor        in-process sync comparator (unavailable)\n"
            "\n"
            "Byrd Box mode (under --interp; --run and --compile force wired):\n"
            "  --bb=brokered    pattern matching via driver/broker  [DEFAULT under --interp]\n"
            "  --bb=wired       live-wired BB blobs in exec memory (requires M-DYN-B* blobs)\n"
            "\n"
            "Diagnostic options:\n"
            "  --dump-ast       print AST after frontend\n"
            "  --dump-bb        print BB-GRAPH for each proc\n"
            "  --dump-sno       transpile AST to portable SNOBOL4 source (GOAL-PARSER-SC-TRANSPILE.md SCT-1)\n"
            "  --trace          MONITOR trace output (diff vs CSNOBOL4)\n"
            "  --bench          print wall-clock time after execution\n"
            "  --dump-ast-bison dump AST via old Bison/Flex parser\n"
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
    for (int fi = argi; fi < argc; fi++) {
        const char *d = strrchr(argv[fi], '.');
        if (d && (strcmp(d,".pl")==0 || strcmp(d,".icn")==0 ||
                  strcmp(d,".raku")==0 || strcmp(d,".reb")==0 ||
                  strcmp(d,".sc")==0 || strcmp(d,".scrip")==0 || strcmp(d,".md")==0))
            has_non_sno = 1;
        if (d && strcmp(d,".pl")==0) is_prolog = 1;
        if (d && strcmp(d,".icn")==0) is_icon = 1;
        if (d && strcmp(d,".raku")==0) is_raku = 1;
    }
    CODE_t *sub = NULL;
    tree_t  *ast_prog = NULL;
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
        const char *input_path = argv[argi];
        {
            char dirbuf[4096];
            strncpy(dirbuf, input_path, sizeof dirbuf - 1);
            dirbuf[sizeof dirbuf - 1] = '\0';
            char *sl = strrchr(dirbuf, '/');
            if (sl) { *sl = '\0'; sno_add_include_dir(dirbuf); }
            else     { sno_add_include_dir("."); }
            const char *core_lib = getenv("SNO_LIB");
            if (core_lib && *core_lib) sno_add_include_dir(core_lib);
            char walk[4096];
            strncpy(walk, input_path, sizeof walk - 1);
            walk[sizeof walk - 1] = '\0';
            char *p = strrchr(walk, '/');
            while (p) {
                *p = '\0';
                char probe[4096];
                snprintf(probe, sizeof probe, "%s/lib", walk);
                struct stat st;
                if (stat(probe, &st) == 0 && S_ISDIR(st.st_mode)) {
                    sno_add_include_dir(walk);
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
        sub = NULL;
        if (lang_polyglot) {
            g_polyglot = 1;
            FILE *f = fopen(input_path, "r");
            if (!f) { fprintf(stderr, "scrip: cannot open '%s'\n", input_path); return 1; }
            fseek(f, 0, SEEK_END); long flen = ftell(f); rewind(f);
            char *src = malloc(flen + 1);
            if (!src) { fprintf(stderr, "scrip: out of memory\n"); return 1; }
            fread(src, 1, flen, f); src[flen] = '\0'; fclose(f);
            tree_t *sub_ast = parse_scrip_polyglot(src, input_path);
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
            MERGE_AST(sub_ast);
        } else if (dump_ast) {
            FILE *f = fopen(input_path, "r");
            if (!f) { fprintf(stderr, "scrip: cannot open '%s'\n", input_path); return 1; }
            if (opt_bench) clock_gettime(CLOCK_MONOTONIC, &_t1);
            tree_t *sub_ast = sno_parse_ast(f, input_path, NULL);
            fclose(f);
            ir_dump_program(sub_ast, stdout);
            return 0;
        } else if (dump_sno) {
            FILE *f = fopen(input_path, "r");
            if (!f) { fprintf(stderr, "scrip: cannot open '%s'\n", input_path); return 1; }
            if (opt_bench) clock_gettime(CLOCK_MONOTONIC, &_t1);
            tree_t *sub_ast = sno_parse_ast(f, input_path, NULL);
            fclose(f);
            MERGE_AST(sub_ast);
        } else {
            FILE *f = fopen(input_path, "r");
            if (!f) { fprintf(stderr, "scrip: cannot open '%s'\n", input_path); return 1; }
            tree_t *sub_ast = sno_parse_ast(f, input_path, dump_ast_bison ? &sub : NULL);
            fclose(f);
            if (dump_ast_bison) { ir_dump_program(sub, stdout); return 0; }
            MERGE_AST(sub_ast);
        }
        if (!ast_prog) {
            fprintf(stderr, "scrip: parse failed for '%s'\n", input_path);
            return 1;
        }
    }
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
    g_opt_trace   = opt_trace;
    g_opt_dump_bb = dump_bb;
    if (dump_sm) {
        fprintf(stderr, "scrip: --dump-sm removed (Stack Machine excised). Use --dump-bb.\n");
        return 1;
    }
    if (dump_sno) {
        extern int tree_to_sno(const tree_t *ast, FILE *out);
        tree_to_sno(ast_prog, stdout);
        return 0;
    }
    if (dump_bb) {
        extern void bb_print(const IR_graph_t * bbg, FILE * fp);
        stage2_t *s2 = sm_preamble(ast_prog);
        if (!s2) { fprintf(stderr, "scrip: sm_preamble failed\n"); return 1; }
        ast_tree_free(ast_prog); ast_prog = NULL;
        for (int _pi = 0; _pi < s2->proc_count; _pi++) {
            int idx = s2->proc_table[_pi].bb_idx;
            const char *pname = s2->proc_table[_pi].name ? s2->proc_table[_pi].name : "?";
            if (idx < 0 || idx >= s2->bbp.count || !s2->bbp.table[idx]) continue;
            fprintf(stdout, "; proc %s\n", pname);
            bb_print(s2->bbp.table[idx], stdout);
        }
        return 0;
    }
    if (mode_compile_x86) {
        extern int codegen_flat_build(IR_t * nd, FILE * out, const char * prefix);
        extern int g_frame_active;
        if (is_icon || is_raku) {
            stage2_t *s2 = sm_preamble(ast_prog);
            if (!s2) return 1;
            ast_tree_free(ast_prog); ast_prog = NULL;
            if ((is_icon || is_raku) && !icn_graph_native_emittable(s2)) {
                fprintf(stderr, "[SMX] --compile --target=x86: mode-4 native emitter does not yet cover "
                                "this program (a box has no MEDIUM_TEXT arm — Icon scan/keyword/cset/gen-alt/"
                                "suspend, or Raku map/grep). EXCISED — mode-2 (--interp) is the oracle for this rung.\n");
                return 0;
            }
            extern bb_box_fn bb_build_flat(IR_t * nd);
            extern void rt_proc_register(const char *name, void *entry, const char **pnames, int nparams);
            extern void rt_proc_set_builder(bb_box_fn (*builder)(void *entry));
            extern void rt_proc_reset(void);
            int main_bb_idx = -1;
            rt_proc_reset();
            rt_proc_set_builder((bb_box_fn (*)(void *))bb_build_flat);
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
                rt_proc_register(pname, s2->bbp.table[idx]->entry, pn, np);
            }
            if (main_bb_idx < 0 || main_bb_idx >= s2->bbp.count || !s2->bbp.table[main_bb_idx] || !s2->bbp.table[main_bb_idx]->entry) {
                fprintf(stderr, "[IBB] FATAL: mode-4 driver: main BB graph not found\n");
                return 1;
            }
            IR_graph_t * bbg = s2->bbp.table[main_bb_idx];
            extern int descr_flat_chain_build_text(IR_t * entry, FILE * out, const char * prefix);
            extern int descr_flat_chain_build_proc_text(IR_t *entry, const char **pnames, int np, FILE *out, const char *pname);
            IR_t *icn_root = icn_ring_to_tree(bbg);
            int use_chain = (icn_root == NULL);
            printf("  .intel_syntax noprefix\n");
            printf("  .text\n");
            g_frame_active = 1;
            int n_procs = 0;
            static char proc_names_buf[64][128];
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
                descr_flat_chain_build_proc_text(s2->bbp.table[idx]->entry, pn, np, stdout, pname);
                if (n_procs < 64) snprintf(proc_names_buf[n_procs++], 128, "%s", pname);
                free(pn);
            }
            if (n_procs > 0) {
                printf("icn_proc_startup:\n");
                printf("  push rbp\n");
                printf("  mov rbp, rsp\n");
                for (int i = 0; i < n_procs; i++) {
                    printf("  .section .rodata\n");
                    printf("  .Lstartup_pname%d: .string \"%s\"\n", i, proc_names_buf[i]);
                    printf("  .section .text\n");
                    printf("  .intel_syntax noprefix\n");
                    printf("  lea rdi, [rip + .Lstartup_pname%d]\n", i);
                    printf("  lea rsi, [rip + icn_proc_%s_\xce\xb1]\n", proc_names_buf[i]);
                    printf("  call rt_proc_set_fn@PLT\n");
                }
                printf("  pop rbp\n");
                printf("  ret\n");
            }
            printf("  .globl main\n");
            printf("main:\n");
            printf("  push rbp\n");
            printf("  mov rbp, rsp\n");
            if (n_procs > 0)
                printf("  call icn_proc_startup\n");
            printf("  call rt_frame@PLT\n");
            printf("  mov rdi, rax\n");
            printf("  xor esi, esi\n");
            printf("  call main_\xce\xb1\n");
            printf("  xor eax, eax\n");
            printf("  pop rbp\n");
            printf("  ret\n");
            int rc;
            {
                extern int g_descr_flat_chain;
                int saved = g_descr_flat_chain; g_descr_flat_chain = 1;
                rc = use_chain ? descr_flat_chain_build_text(bbg->entry, stdout, "main")
                               : codegen_flat_build(icn_root, stdout, "main");
                g_descr_flat_chain = saved;
            }
            g_frame_active = 0;
            fflush(stdout);
            return rc;
        }
        if (is_prolog) {
            extern int codegen_flat_build(IR_t * nd, FILE * out, const char * prefix);
            extern int codegen_clause_dispatch(FILE * out);
            extern int g_frame_active;
            extern void xa_emit_strtab_rodata(void);
            stage2_t *s2 = sm_preamble(ast_prog);
            if (!s2) { fprintf(stderr, "[PBB] mode-4: sm_preamble failed\n"); return 1; }
            ast_tree_free(ast_prog); ast_prog = NULL;
            int main_bb_idx = -1;
            for (int _pi = 0; _pi < s2->proc_count; _pi++)
                if (s2->proc_table[_pi].name && strcmp(s2->proc_table[_pi].name, "main") == 0) { main_bb_idx = s2->proc_table[_pi].bb_idx; break; }
            if (main_bb_idx < 0 || main_bb_idx >= s2->bbp.count || !s2->bbp.table[main_bb_idx]) {
                fprintf(stderr, "[PBB] FATAL: mode-4 driver: Prolog main BB graph not found "
                                "(no initialization goal lowered, or predicate unhandled)\n");
                return 1;
            }
            IR_graph_t *pl_main = s2->bbp.table[main_bb_idx];
            IR_t *flat_root = pl_flat_body_root(pl_main);
            if (!flat_root) {
                IR_t *rich_root = pl_rich_body_root(pl_main);
                if (!rich_root) {
                    fprintf(stderr, "[SMX] --compile --target=x86: Prolog mode-4 covers the hello-world + "
                                    "unify/arith + facts/choice/call tiers; this program has a construct not "
                                    "yet wired (PLG-9e+).\n");
                    return 1;
                }
                printf("  .intel_syntax noprefix\n");
                printf("  .text\n");
                printf("  .globl main\n");
                printf("main:\n");
                printf("  push rbp\n");
                printf("  mov rbp, rsp\n");
                printf("  call rt_main_init@PLT\n");
                if (pl_main->nslots > 0) {
                    printf("  mov edi, %d\n", pl_main->nslots);
                    printf("  call rt_env_alloc@PLT\n");
                }
                printf("  call rt_frame@PLT\n");
                printf("  mov rdi, rax\n");
                printf("  xor esi, esi\n");
                printf("  call main_\xce\xb1\n");
                printf("  xor eax, eax\n");
                printf("  pop rbp\n");
                printf("  ret\n");
                g_frame_active = 1;
                int rcp = codegen_clause_dispatch(stdout);
                extern IR_graph_t *g_emit_cfg;
                IR_graph_t *save_cfg = g_emit_cfg; g_emit_cfg = pl_main;
                int rcm = codegen_flat_build(rich_root, stdout, "main");
                g_emit_cfg = save_cfg;
                g_frame_active = 0;
                xa_emit_strtab_rodata();
                fflush(stdout);
                return (rcp || rcm) ? 1 : 0;
            }
            printf("  .intel_syntax noprefix\n");
            printf("  .text\n");
            printf("  .globl main\n");
            printf("main:\n");
            printf("  push rbp\n");
            printf("  mov rbp, rsp\n");
            if (pl_main->nslots > 0) {
                printf("  mov edi, %d\n", pl_main->nslots);
                printf("  call rt_env_alloc@PLT\n");
            }
            printf("  call rt_frame@PLT\n");
            printf("  mov rdi, rax\n");
            printf("  xor esi, esi\n");
            printf("  call main_\xce\xb1\n");
            printf("  xor eax, eax\n");
            printf("  pop rbp\n");
            printf("  ret\n");
            g_frame_active = 1;
            int rc = codegen_flat_build(flat_root, stdout, "main");
            g_frame_active = 0;
            xa_emit_strtab_rodata();
            fflush(stdout);
            return rc;
        }
        {
            extern void xa_file_header(void);
            extern void emit_io_set_sink(FILE * out);
            extern void emitter_init_text(FILE * out, int mode);
            stage2_t *s2 = sm_preamble(ast_prog);
            if (!s2) { fprintf(stderr, "[SBB] mode-4: sm_preamble failed\n"); return 1; }
            ast_tree_free(ast_prog); ast_prog = NULL;
            int main_bb_idx = -1;
            for (int _pi = 0; _pi < s2->proc_count; _pi++)
                if (s2->proc_table[_pi].name && strcmp(s2->proc_table[_pi].name, "main") == 0) { main_bb_idx = s2->proc_table[_pi].bb_idx; break; }
            IR_graph_t *sbbg = (main_bb_idx >= 0 && main_bb_idx < s2->bbp.count) ? s2->bbp.table[main_bb_idx] : NULL;
            (void)sbbg;
            fprintf(stderr, "[SBB] mode-4: sno_ring_to_tree REMOVED (VIOLATION, Lon 2026-05-31). SNOBOL4 "
                            "mode-4 emission must come from LOWER producing the four-port statement-BB graph "
                            "directly (no ring->tree adapter); pending that wiring. mode-4 uses the SAME boxes "
                            "as mode-3 (TEXT vs BINARY medium) — this is a wiring gap, not a design limit.\n");
            abort();
        }
    }
    if (mode_compile && target_name && strcmp(target_name, "x86") != 0) {
        fprintf(stderr, "[SMX] --target=%s removed (Stack-Machine codegen excised).\n",
                target_name ? target_name : "?");
        ast_tree_free(ast_prog); ast_prog = NULL;
        return 1;
    }
    if (mode_monitor) {
        fprintf(stderr, "[NO-SM-BB] --monitor: trampoline codegen deleted (FACT RULE); unavailable\n");
        ast_tree_free(ast_prog); ast_prog = NULL;
        return 1;
    } else if (mode_interp) {
        extern int g_icn_postfix_resume;
        if (is_icon) g_icn_postfix_resume = 1;
        stage2_t *s2 = sm_preamble(ast_prog);
        if (!s2) return 1;
        ast_tree_free(ast_prog); ast_prog = NULL;
        if (is_icon) {
            extern DESCR_t IR_interp_once(IR_graph_t * bbg);
            int main_bb_idx = -1;
            for (int _pi = 0; _pi < s2->proc_count; _pi++) {
                if (s2->proc_table[_pi].name && strcmp(s2->proc_table[_pi].name, "main") == 0) {
                    main_bb_idx = s2->proc_table[_pi].bb_idx;
                    break;
                }
            }
            if (main_bb_idx < 0 || main_bb_idx >= s2->bbp.count || !s2->bbp.table[main_bb_idx]) {
                fprintf(stderr, "[IBB] FATAL: mode-2 driver: main BB graph not found\n");
                abort();
            }
            (void)IR_interp_once(s2->bbp.table[main_bb_idx]);
            goto run_done;
        }
        if (!is_icon && !is_prolog) {
            extern DESCR_t IR_interp_once(IR_graph_t * bbg);
            int main_bb_idx = -1;
            for (int _pi = 0; _pi < s2->proc_count; _pi++) {
                if (s2->proc_table[_pi].name && strcmp(s2->proc_table[_pi].name, "main") == 0) {
                    main_bb_idx = s2->proc_table[_pi].bb_idx;
                    break;
                }
            }
            if (main_bb_idx < 0 || main_bb_idx >= s2->bbp.count || !s2->bbp.table[main_bb_idx]) {
                fprintf(stderr, "[SBB] FATAL: mode-2 driver: SNOBOL4 main BB graph not found\n");
                abort();
            }
            (void)IR_interp_once(s2->bbp.table[main_bb_idx]);
            goto run_done;
        }
        if (is_prolog) {
            extern DESCR_t IR_interp_once(IR_graph_t * bbg);
            extern Term **g_resolve_env;
            int main_bb_idx = -1;
            for (int _pi = 0; _pi < s2->proc_count; _pi++) {
                if (s2->proc_table[_pi].name && strcmp(s2->proc_table[_pi].name, "main") == 0) {
                    main_bb_idx = s2->proc_table[_pi].bb_idx;
                    break;
                }
            }
            if (main_bb_idx < 0 || main_bb_idx >= s2->bbp.count || !s2->bbp.table[main_bb_idx]) {
                fprintf(stderr, "[PBB] FATAL: mode-2 driver: Prolog main BB graph not found "
                                "(no initialization goal lowered, or predicate unhandled by PLG-1)\n");
                abort();
            }
            IR_graph_t *pl_main = s2->bbp.table[main_bb_idx];
            int nslots = pl_main->nslots > 0 ? pl_main->nslots : 1;
            g_resolve_env = (Term **)GC_MALLOC((size_t)(nslots + 8) * sizeof(Term *));
            (void)IR_interp_once(pl_main);
            goto run_done;
        }
        fprintf(stderr, "[SMX] FATAL: Stack Machine excised. Non-Icon mode-2 (--interp) "
                        "execution is gone. This language has not yet crossed onto Byrd Boxes. "
                        "Aborting (by design).\n");
        abort();
    } else if (mode_run) {
        stage2_t *s2 = sm_preamble(ast_prog);
        if (!s2) return 1;
        ast_tree_free(ast_prog); ast_prog = NULL;
        if (is_icon || is_raku) {
            extern bb_box_fn bb_build_flat(IR_t * nd);
            extern void rt_proc_register(const char *name, void *entry, const char **pnames, int nparams);
            extern void rt_proc_set_builder(bb_box_fn (*builder)(void *entry));
            extern void rt_proc_reset(void);
            extern bb_box_fn descr_flat_chain_build_proc(IR_t * entry, const char ** pnames, int np);
            extern void rt_proc_set_fn(const char *name, bb_box_fn fn);
            extern int g_frame_active;
            if ((is_icon || is_raku) && !icn_graph_native_emittable(s2)) {
                fprintf(stderr, "[SMX] --run: mode-3 native emitter does not yet cover this program "
                                "(a box has no MEDIUM_BINARY arm — Icon scan/keyword/cset/gen-alt/suspend, "
                                "or Raku map/grep). EXCISED — mode-2 (--interp) is the oracle for this rung.\n");
                return 0;
            }
            int main_bb_idx = -1;
            rt_proc_reset();
            rt_proc_set_builder((bb_box_fn (*)(void *))bb_build_flat);
            g_frame_active = 1;
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
                rt_proc_register(pname, s2->bbp.table[idx]->entry, pn, np);
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
                bb_box_fn pfn = descr_flat_chain_build_proc(s2->bbp.table[idx]->entry, pn, np);
                if (pfn) rt_proc_set_fn(pname, pfn);
            }
            {
                extern void *dat_register(const char *spec);
                for (int _gi = 0; _gi < s2->bbp.count; _gi++) {
                    IR_graph_t *g = s2->bbp.table[_gi];
                    if (!g || !g->all) continue;
                    for (int _ni = 0; _ni < g->n; _ni++) {
                        IR_t *nd = g->all[_ni];
                        if (nd && nd->t == IR_RECORD_DEF && nd->sval) dat_register(nd->sval);
                    }
                }
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
            extern void *rt_frame(void);
            extern bb_box_fn descr_flat_chain_build(IR_t * entry);
            IR_t *icn_root = icn_ring_to_tree(bbg);
            bb_box_fn fn;
            if (icn_root) {
                extern int g_descr_flat_chain;
                int saved = g_descr_flat_chain; g_descr_flat_chain = 1;
                fn = bb_build_flat(icn_root);
                g_descr_flat_chain = saved;
            } else {
                fn = descr_flat_chain_build(bbg->entry);
            }
            g_frame_active = 0;
            if (!fn) {
                fprintf(stderr, "[IBB] FATAL: mode-3 driver: bb_build_flat returned NULL — BB template(s) lack MEDIUM_BINARY arm\n");
                abort();
            }
            (void)fn(rt_frame(), 0);
            goto run_done;
        }
        if (is_prolog) {
            extern DESCR_t IR_interp_once(IR_graph_t * bbg);
            extern Term **g_resolve_env;
            extern bb_box_fn bb_build_flat(IR_t * nd);
            extern void *rt_frame(void);
            extern int g_frame_active;
            int main_bb_idx = -1;
            for (int _pi = 0; _pi < s2->proc_count; _pi++) {
                if (s2->proc_table[_pi].name && strcmp(s2->proc_table[_pi].name, "main") == 0) {
                    main_bb_idx = s2->proc_table[_pi].bb_idx;
                    break;
                }
            }
            if (main_bb_idx < 0 || main_bb_idx >= s2->bbp.count || !s2->bbp.table[main_bb_idx]) {
                fprintf(stderr, "[PBB] FATAL: mode-3 driver: Prolog main BB graph not found "
                                "(no initialization goal lowered, or predicate unhandled by PLG-1)\n");
                abort();
            }
            IR_graph_t *pl_main = s2->bbp.table[main_bb_idx];
            int nslots = pl_main->nslots > 0 ? pl_main->nslots : 1;
            g_resolve_env = (Term **)GC_MALLOC((size_t)(nslots + 8) * sizeof(Term *));
            IR_t *flat_root = pl_flat_body_root(pl_main);
            if (flat_root) {
                g_frame_active = 1;
                bb_box_fn pfn = bb_build_flat(flat_root);
                g_frame_active = 0;
                if (pfn) { (void)pfn(rt_frame(), 0); goto run_done; }
            }
            (void)IR_interp_once(pl_main);
            goto run_done;
        }
        {
            if (is_prolog) {
                fprintf(stderr, "[SMX] FATAL: Stack Machine excised. Prolog mode-3 (--run) "
                                "native execution is gone. This language has not yet crossed "
                                "onto Byrd Boxes. Aborting (by design).\n");
                (void)s2;
                abort();
            }
            extern bb_box_fn gvar_flat_chain_build(IR_graph_t * g);
            extern void *rt_frame(void);
            extern int g_frame_active;
            extern void rt_proc_register(const char *name, void *entry, const char **pnames, int nparams);
            extern void rt_proc_set_fn(const char *name, bb_box_fn fn);
            extern void rt_proc_reset(void);
            int main_bb_idx = -1;
            for (int _pi = 0; _pi < s2->proc_count; _pi++)
                if (s2->proc_table[_pi].name && strcmp(s2->proc_table[_pi].name, "main") == 0) { main_bb_idx = s2->proc_table[_pi].bb_idx; break; }
            rt_proc_reset();
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
                    for (int k = 0; k < np && k < s2->proc_table[_pi].lower_sc.n; k++)
                        pn[k] = s2->proc_table[_pi].lower_sc.e[k].name;
                }
                rt_proc_register(pname, s2->bbp.table[idx]->entry, pn, np);
                bb_box_fn pfn = gvar_flat_chain_build(s2->bbp.table[idx]);
                if (pfn) rt_proc_set_fn(pname, pfn);
            }
            g_frame_active = 0;
            IR_graph_t *sbbg = (main_bb_idx >= 0 && main_bb_idx < s2->bbp.count) ? s2->bbp.table[main_bb_idx] : NULL;
            if (sbbg && sbbg->entry) {
                g_frame_active = 1;
                bb_box_fn fn = gvar_flat_chain_build(sbbg);
                g_frame_active = 0;
                if (fn) { (void)fn(rt_frame(), 0); goto run_done; }
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
