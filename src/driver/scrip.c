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
#include "../frontend/snobol4/scrip_cc.h"
#include "../frontend/snocone/snocone_driver.h"
#include "../frontend/prolog/prolog_driver.h"
#include "../frontend/prolog/term.h"
#include "../frontend/prolog/prolog_runtime.h"
#include "../frontend/prolog/prolog_atom.h"
#include "../frontend/prolog/prolog_builtin.h"
#include "../frontend/prolog/pl_broker.h"
#include "../frontend/icon/icon_driver.h"
#include "../frontend/raku/raku_driver.h"
#include "../frontend/rebus/rebus_lower.h"
#include "../runtime/interp/gen.h"
#include "../frontend/icon/icon_lex.h"
#include "../include/bb_box.h"
extern void ir_print_node   (const tree_t *e, FILE *f);
extern void ir_set_print_width(int w);
extern void ir_print_node_nl(const tree_t *e, FILE *f);
#include "core.h"
#include "sil_macros.h"
#include "runtime_shim.h"
#include "lower.h"
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
#include "../runtime/interp/gen_runtime.h"
#include "../runtime/interp/resolve_runtime.h"
#include "driver/polyglot.h"
#include "../tools/emit_per_kind_audit.h"
/*====================================================================================================================================================================================================*/
/* ICON MODE-3 RING->TREE ADAPTER (Path 1-lite, Icon --run only). The unified lower2 emits a postfix      */
/* gamma-chain for Icon expressions: operands precede their operator in gamma-order, every node has       */
/* alpha=beta=NULL, and operands are read from the AG ring at exec time (the mode-2 oracle's model). The  */
/* mode-3 flat emitter (emit_bb.c walk_bb_flat / flat_drive_*) and the GROUND-ZERO templates expect the   */
/* OLD tree-shape (operands in alpha/beta children, as the deleted lower_icn.c blob d2d8c8e1 built them:  */
/* bb->alpha=lhs; bb->beta=rhs / bb->alpha=args[0]). This adapter un-flattens the straight-line chain     */
/* into that tree by postfix evaluation (each kind's operand arity drives how many preceding nodes become */
/* its children), relinks alpha/beta, and returns the root for bb_build_flat. Fails SOFT (returns NULL)   */
/* on any shape outside the GZ-1/2/3 straight-line subset so the caller falls back to the prior behavior  */
/* (no regression for control-flow rungs not yet rebuilt). Postfix order verified empirically via         */
/* --dump-bb (write(2+3): LIT_I 2 -> LIT_I 3 -> IR_BINOP -> IR_CALL). lower.c and the templates are        */
/* UNTOUCHED; this is a mode-3 Icon emitter-input adapter, not a lowerer change.                          */
/*====================================================================================================================================================================================================*/
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * icn_ring_to_tree(IR_graph_t *g) {
    if (!g || !g->entry) return NULL;
    IR_t *chain[256]; int nc = 0;
    for (IR_t *cur = g->entry; cur && cur->t != IR_SUCCEED && cur->t != IR_FAIL && nc < 256; cur = cur->γ) chain[nc++] = cur;
    if (nc == 0 || nc >= 256) return NULL;
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
/*====================================================================================================================================================================================================*/
/* SNOBOL4 MODE-3 RING->TREE ADAPTER (SBL-M3-STACKLESS, 2026-05-31). The SNOBOL4 program graph is a       */
/* gamma/omega-threaded CFG: leading landing IR_SUCCEED node(s) at statement boundaries, each statement a */
/* postfix gamma-chain of its sub-expressions ending at the next landing / PSUCC. This adapter recognizes */
/* ONLY the shapes for which a STACKLESS box exists today — currently the single-statement literal assign */
/* `name = 'literal'` (landing -> IR_LIT_S -> IR_ASSIGN -> PSUCC): it folds the lit onto the assign's      */
/* alpha (postfix) and returns the assign as root. EVERY other shape (multi-statement, IR_SCAN, IR_GOTO,  */
/* IR_SEQ concat via isolated sub-graphs, arith, user-proc) returns NULL — the caller then SOFT-fails     */
/* (honest stderr, clean exit, NO abort). No value stack is created anywhere (Lon directive). As more     */
/* stackless boxes land, widen this adapter; it is the shared front-end both A-was-rejected and B use.    */
/*====================================================================================================================================================================================================*/
static IR_t * sno_ring_to_tree(IR_graph_t *g) {
    if (!g || !g->entry) return NULL;
    IR_t *start = g->entry;
    int guard = 0;
    while (start && start->t == IR_SUCCEED && start->γ && guard++ < 64) start = start->γ;
    if (!start) return NULL;
    IR_t *chain[64]; int nc = 0;
    for (IR_t *cur = start; cur && cur->t != IR_SUCCEED && cur->t != IR_FAIL && nc < 64; cur = cur->γ) chain[nc++] = cur;
    if (nc == 2 && chain[0]->t == IR_LIT_S && chain[1]->t == IR_ASSIGN && chain[1]->sval) {
        chain[1]->α = chain[0];
        return chain[1];
    }
    /* SBL-M3-ARITH (2026-05-31): `name = lit op lit` — the postfix chain is LIT_I -> LIT_I -> BINOP ->     */
    /* ASSIGN. Fold the two ints onto the binop (α=first, β=second — postfix order), then the binop onto    */
    /* the assign's α, and return the assign as root. The binop is the GZ-3 RO-int stackless box; the       */
    /* assign reads its ζ-frame slot (bb_sno_assign IR_BINOP arm). Bounded single-shot, NO value stack.     */
    if (nc == 4 && chain[0]->t == IR_LIT_I && chain[1]->t == IR_LIT_I
        && chain[2]->t == IR_BINOP && chain[3]->t == IR_ASSIGN && chain[3]->sval) {
        chain[2]->α = chain[0];
        chain[2]->β = chain[1];
        chain[3]->α = chain[2];
        return chain[3];
    }
    return NULL;
}
/*====================================================================================================================================================================================================*/
/* PROLOG MODE-3 NATIVE FLAT-WALK ROOT RECOGNIZER (PLG-8-native, 2026-05-31). The interim mode-3 route (PLG-8) ran Prolog --run through bb_exec_once (the mode-2 interpreter + the AG ring on IR_graph_t) */
/* — correct output, but that is the RING path, which is the mode-2 idiom only (Lon directive: rings are for mode-2 interp; WRONG for mode-3). Mode-3 must EMIT code+data INSIDE the boxes with values    */
/* flowing UP the BB graph chain via per-box slots, exactly as test_sno_1.c / test_icon.c do (POS0/BIRD/mult_V live in the box and the consumer reads the producer's slot — no ring, no value stack). This  */
/* recognizer returns the flat-walk ROOT (the principal IR_GCONJ in the main graph) for the shapes whose BB templates already emit a correct stackless box with NO ring traffic — today the ground          */
/* hello-world tier: a single-clause body that is a conjunction of constant-arg builtins (write/writeln/print/nl/halt) and/or IR_SUCCEED/IR_CUT, with NO logic-variable slots (nslots==0) and NO             */
/* user-proc call / choice / disjunction / ite / unify / arith goal. For every richer shape it returns NULL and the driver keeps the proven interim bb_exec_once route (zero regression) — the same          */
/* widen-as-you-go gate sno_ring_to_tree uses for SNOBOL4. The GCONJ carries its goal entries in the bb_conj_state_t sidecar (node->ival); bb_builtin reads each arg from the goal's own alpha at emit time   */
/* (write of an IR_ATOM -> mov rdi, imm64(atom); call rt_pl_write_atom), so the value is a sealed read-only constant read directly by its consumer — no rt_pl_atom_push ring push. Widens rung by rung.       */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_flat_goal_is_simple(const IR_t *g) {
    if (!g) return 0;
    switch (g->t) {
    case IR_SUCCEED: case IR_CUT: case IR_ATOM: return 1;
    case IR_BUILTIN: {
        const char *fn = g->sval ? g->sval : "";
        int is_io = (!strcmp(fn, "write") || !strcmp(fn, "writeln") || !strcmp(fn, "print") || !strcmp(fn, "nl") || !strcmp(fn, "halt"));
        if (!is_io) return 0;
        if (g->ival >= 1) { const IR_t *a = g->α; if (!a || (a->t != IR_ATOM && a->t != IR_LIT_I)) return 0; }
        return 1;
    }
    default: return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_flat_body_root(IR_graph_t *g) {
    if (!g || !g->all || g->nslots > 0) return NULL;
    IR_t *gconj = NULL;
    for (int i = 0; i < g->n; i++) {
        IR_t *nd = g->all[i];
        if (nd && nd->t == IR_GCONJ) { if (gconj) return NULL; gconj = nd; }
    }
    if (!gconj) return (g->entry && g->entry->t == IR_SUCCEED) ? g->entry : NULL;
    bb_conj_state_t *zs = (bb_conj_state_t *)(intptr_t)gconj->ival;
    if (!zs || !zs->goals || zs->ngoals <= 0) return NULL;
    for (int i = 0; i < zs->ngoals; i++) if (!pl_flat_goal_is_simple(zs->goals[i])) return NULL;
    return gconj;
}
/*====================================================================================================================================================================================================*/
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
    for (int fi = argi; fi < argc; fi++) {
        const char *d = strrchr(argv[fi], '.');
        if (d && (strcmp(d,".pl")==0 || strcmp(d,".icn")==0 ||
                  strcmp(d,".raku")==0 || strcmp(d,".reb")==0 ||
                  strcmp(d,".sc")==0 || strcmp(d,".scrip")==0 || strcmp(d,".md")==0))
            has_non_sno = 1;
        if (d && strcmp(d,".pl")==0) is_prolog = 1;
        if (d && strcmp(d,".icn")==0) is_icon = 1;
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
        } else if (lang_snocone || lang_prolog || lang_icon || lang_raku || lang_rebus) {
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
    extern void SNO_INIT_fn(void);
    SNO_INIT_fn();
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
        if (is_icon) {
            /* MODE-4 (BB-native x86, GROUND ZERO 3): emit a standalone GAS .s — a C-ABI `main` wrapper that  */
            /* fetches the per-sequence frame (rt_frame), passes it as ζ (rdi) with the α entry selector      */
            /* (esi=0), and calls the flat BB body, then the body itself via codegen_flat_build (the SAME BB  */
            /* templates mode-3 emits, in MEDIUM_TEXT). g_frame_active makes the prologue/epilogue use the    */
            /* Icon stackless r12-frame form. The .s links libscrip_rt.so (rt_write_*, rt_frame, rt_call_*).  */
            stage2_t *s2 = sm_preamble(ast_prog);
            if (!s2) return 1;
            ast_tree_free(ast_prog); ast_prog = NULL;
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
            IR_t *icn_root = icn_ring_to_tree(bbg);
            if (!icn_root) {
                fprintf(stderr, "[IBB] mode-4: ring->tree adapter could not linearize this graph (control-flow "
                                "rung not yet rebuilt for mode-4); no asm emitted.\n");
                return 1;
            }
            printf("  .intel_syntax noprefix\n");
            printf("  .text\n");
            printf("  .globl main\n");
            printf("main:\n");
            printf("  push rbp\n");
            printf("  mov rbp, rsp\n");
            printf("  call rt_frame@PLT\n");
            printf("  mov rdi, rax\n");
            printf("  xor esi, esi\n");
            printf("  call main_α\n");
            printf("  xor eax, eax\n");
            printf("  pop rbp\n");
            printf("  ret\n");
            g_frame_active = 1;
            int rc = codegen_flat_build(icn_root, stdout, "main");
            g_frame_active = 0;
            fflush(stdout);
            return rc;
        }
        if (!is_prolog) {
            /* SBL-M4-STACKLESS (2026-05-31, Opus 4.8): SNOBOL4 mode-4 BB-native x86 emission, REBUILT.
               The mode-4 emission scaffolding (codegen_flat_build + the XA wrap templates xa_file_header/
               xa_file_footer/xa_flat_*) was never deleted by SMX-4 — only THIS driver stitch was. Re-stitched
               for SNOBOL4: lower → find main BB graph → sno_ring_to_tree (the same stackless statement-BB
               adapter mode-3 uses) → emit a complete .intel_syntax assembly program to stdout:
                 xa_file_header  (.globl main; rt_gc_init; rt_set_lang; rt_register_expressions)
                 glue            (xor esi,esi  [fresh-entry dispatch]; call <prefix>_α)
                 xa_file_footer  (rt_finalize; pop rbp; ret)
                 codegen_flat_build(root)  (the statement BB as a standalone <prefix>_α function — same
                                            four-port stackless body as mode-3, via the TEXT arms)
               The assembled+linked binary (as → gcc -no-pie -lscrip_rt --allow-shlib-undefined) runs the box.
               SNOBOL4-only; any shape sno_ring_to_tree can't flatten yet → honest soft-fail (no abort). NO
               value stack (Lon directive). */
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
            IR_t *sroot = sbbg ? sno_ring_to_tree(sbbg) : NULL;
            if (!sroot) {
                fprintf(stderr, "[SBB] mode-4: SNOBOL4 program shape not yet flat-emittable "
                                "(stackless boxes pending; only single-statement literal assign wired). No native emit.\n");
                return 1;
            }
            FILE *out = stdout;
            emit_mode_set(EMIT_TEXT, out);
            emit_io_set_sink(out);
            emitter_init_text(out, 0);
            xa_file_header();
            fprintf(out, "  xor esi, esi\n  call stmt0_\xCE\xB1\n");
            /* Close main HERE (rt_finalize; pop rbp; ret; .size) but do NOT emit the .note.GNU-stack yet —
               xa_file_footer bundles the note WITH the ret, which would strand the statement-BB body (emitted
               after) in the linker-discarded .note.GNU-stack section. Emit the body in .text, THEN the note last. */
            fprintf(out, "call rt_finalize@PLT\npop rbp\nret\n.size main, .-main\n");
            g_frame_active = 1;
            int rc = codegen_flat_build(sroot, out, "stmt0");
            g_frame_active = 0;
            fprintf(out, ".section .note.GNU-stack\n");
            fflush(out);
            return rc == 0 ? 0 : 1;
        }
        fprintf(stderr, "[SMX] --compile --target=x86: Prolog mode-4 pending (BB graph not yet wired).\n");
        ast_tree_free(ast_prog); ast_prog = NULL;
        return 1;
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
        if (is_icon) g_icn_postfix_resume = 1;   /* mode-2 port-walker: wire deterministic-builtin CALL resume to arg resume (re-pump generator args); Icon-only */
        stage2_t *s2 = sm_preamble(ast_prog);
        if (!s2) return 1;
        ast_tree_free(ast_prog); ast_prog = NULL;
        if (is_icon) {
            extern DESCR_t bb_exec_once(IR_graph_t * bbg);
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
            (void)bb_exec_once(s2->bbp.table[main_bb_idx]);
            goto run_done;
        }
        if (!is_icon && !is_prolog) {
            extern DESCR_t bb_exec_once(IR_graph_t * bbg);
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
            (void)bb_exec_once(s2->bbp.table[main_bb_idx]);
            goto run_done;
        }
        if (is_prolog) {
            extern DESCR_t bb_exec_once(IR_graph_t * bbg);
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
            (void)bb_exec_once(pl_main);
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
        if (is_icon) {
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
            extern int g_frame_active;
            extern void *rt_frame(void);
            g_frame_active = 1;
            IR_t *icn_root = icn_ring_to_tree(bbg);
            bb_box_fn fn = bb_build_flat(icn_root ? icn_root : bbg->entry);
            g_frame_active = 0;
            if (!fn) {
                fprintf(stderr, "[IBB] FATAL: mode-3 driver: bb_build_flat returned NULL — BB template(s) lack MEDIUM_BINARY arm\n");
                abort();
            }
            (void)fn(rt_frame(), 0);
            goto run_done;
        }
        if (is_prolog) {
            extern DESCR_t bb_exec_once(IR_graph_t * bbg);
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
            /* PLG-8-native (2026-05-31): for the proven hello-world shape, EMIT the boxes natively via    */
            /* bb_build_flat (code+data in the boxes; values up the chain in per-box slots; NO ring) and    */
            /* run the JIT'd box, exactly as Icon/SNOBOL4 mode-3 do. Every richer shape -> pl_flat_body_root */
            /* returns NULL and we keep the proven interim bb_exec_once route (no regression). Widen rung-   */
            /* by-rung as the flat templates are verified for choice/unify/arith/user-call.                 */
            IR_t *flat_root = pl_flat_body_root(pl_main);
            if (flat_root) {
                g_frame_active = 1;
                bb_box_fn pfn = bb_build_flat(flat_root);
                g_frame_active = 0;
                if (pfn) { (void)pfn(rt_frame(), 0); goto run_done; }
            }
            (void)bb_exec_once(pl_main);
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
            /* SBL-M3-STACKLESS (2026-05-31): SNOBOL4 mode-3 via bb_build_flat over a STACKLESS box graph.
               NO value stack (Lon directive). sno_ring_to_tree returns a root only for shapes with a
               working stackless box (today: single-statement literal assign); NULL otherwise -> honest
               soft-fail (no abort), so the gap is loud but the process stays clean. */
            extern bb_box_fn bb_build_flat(IR_t * nd);
            extern void *rt_frame(void);
            extern int g_frame_active;
            int main_bb_idx = -1;
            for (int _pi = 0; _pi < s2->proc_count; _pi++)
                if (s2->proc_table[_pi].name && strcmp(s2->proc_table[_pi].name, "main") == 0) { main_bb_idx = s2->proc_table[_pi].bb_idx; break; }
            IR_graph_t *sbbg = (main_bb_idx >= 0 && main_bb_idx < s2->bbp.count) ? s2->bbp.table[main_bb_idx] : NULL;
            IR_t *sroot = sbbg ? sno_ring_to_tree(sbbg) : NULL;
            if (!sroot) {
                fprintf(stderr, "[SBB] mode-3: SNOBOL4 program shape not yet flat-emittable "
                                "(stackless boxes pending; only single-statement literal assign wired). "
                                "No native run; use --interp.\n");
                goto run_done;
            }
            g_frame_active = 1;
            bb_box_fn sfn = bb_build_flat(sroot);
            g_frame_active = 0;
            if (!sfn) {
                fprintf(stderr, "[SBB] mode-3: bb_build_flat returned NULL (stackless template lacks BINARY arm)\n");
                goto run_done;
            }
            (void)sfn(rt_frame(), 0);
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
