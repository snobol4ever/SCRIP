#pragma once
#ifndef SCRIP_IR_H
#define SCRIP_IR_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "descr.h"
#ifndef NULVCL
#  define NULVCL       ((DESCR_t){ .v = DT_SNUL, .slen = 0, .s = "" })
#endif
#ifndef INTVAL
#  define INTVAL(i_)   ((DESCR_t){ .v = DT_I, .i = (int64_t)(i_) })
#endif
#ifndef REALVAL
#  define REALVAL(r_)  ((DESCR_t){ .v = DT_R, .r = (double)(r_) })
#endif
#ifndef STRVAL
#  define STRVAL(s_)   ((DESCR_t){ .v = DT_S, .slen = 0, .s = (s_) })
#endif
typedef enum {
    IR_ACTIVATE,
    IR_ASSIGN,
    IR_ASSIGN_VAR,
    IR_BINOP,
    IR_BINOP_TEST,
    IR_BINOP_RELOP_VAL,
    IR_CALL,
    IR_CALL_BUILTIN,
    IR_CALL_BUILTIN_GEN,
    IR_CALL_BUILTIN_ICON,
    IR_CALL_BUILTIN_SNOBOL4,
    IR_CALL_BUILTIN_PROLOG,
    IR_CALL_PROC_STAGED,
    IR_CALL_VALUE,
    IR_SAVE_RESTORE,   /* SN4-FLAT-PROC (s176) linkage family — SPITBOL manual Ch.8 save/restore protocol citizens, role in ival: 0 = site OPEN (reserved, next slice), 1 = RETURN floater (peek wires, restore rsp/rbp, jmp γ wire), 2 = FRETURN floater (same, ω wire), 3 = WIRE-ADOPT (stub entry: copy header wires + entry-rsp + caller-rbp into the open pcall record) */
    IR_COFAIL,
    IR_CONJUNCTION,
    IR_CORET,
    IR_CREATE,
    IR_CUT,
    IR_DEREF,
    IR_DISJUNCTION,
    IR_INITIAL,
    IR_FAIL,
    IR_FIELD_GET,
    IR_FIELD_VAR,
    IR_NULLTEST_VAR,
    IR_GOTO,
    IR_INDIRECT_GOTO,
    IR_ITERATE,
    IR_KEYWORD_ICON,
    IR_KEYWORD_ICON_GEN,
    IR_KEYWORD_SNOBOL4,
    IR_KEYWORD_ASSIGN,
    IR_LIT_CHARSET,
    IR_LIT_INTEGER,
    IR_LIT_REAL,
    IR_LIT_STRING,
    IR_MAKE_LIST,
    IR_MOVE_LABEL,
    IR_PROC_GEN,
    IR_PROC_VALUE,
    IR_RANDOM,
    IR_RETURN,
    IR_LIMIT,
    IR_REPALT,
    IR_REV_ASSIGN,
    IR_REV_ASSIGN_VAR,
    IR_REV_SWAP,
    IR_SCAN,
    IR_SCAN_ENTER,
    IR_SCAN_ANY,
    IR_SCAN_BAL,
    IR_SCAN_FIND,
    IR_SCAN_MANY,
    IR_SCAN_MATCH,
    IR_SCAN_MOVE,
    IR_SCAN_POS,
    IR_SCAN_TAB,
    IR_SCAN_UPTO,
    IR_SCAN_SEQUENCE,
    IR_SCAN_ALTERNATE,
    IR_SUBSCRIPT,
    IR_SUCCEED,
    IR_SUSPEND,
    IR_SWAP,
    IR_SWAP_VAR,
    IR_TO,
    IR_TO_BY,
    IR_UNOP,
    IR_UNOP_TEST,
    IR_VAR,
    IR_VAR_REF,
    IR_COERCE_STRING,       /* operand-edge coercion (2026-07-10 GEM/operand rung): operand[0] = value node; own slot gets a DT_S DESCR; ival = SPITBOL error code raised on null/non-string (0 = permissive) */
    IR_COERCE_INTEGER,      /* operand[0] = value node; own slot gets a DT_I DESCR; ival = nonint_errcode | (negative_errcode << 16); neg code 0 = negatives allowed */
    IR_COERCE_NUMERIC,      /* SNOBOL4 predicate arg (2026-07-10): operands[0]=self [1]=other; joint INTEGER-vs-REAL decision needs BOTH (either real -> both real); own slot DT_I or DT_R; ival = positional errcode */
    IR_COERCE_REAL,         /* directed cnv:C_double (Icon oarith.r/fstranl.r discipline): operand[0] = value node; own slot gets a DT_R DESCR; ival = errcode raised on unconvertible (0 = permissive -> 0.0) */
    IR_CMP_TEST,            /* predicate compare: operands[0]/[1] = the two coerced values; ival = relop 0..5 (EQ NE LT LE GT GE); gamma = succeed (own slot := null string), omega = fail */
    /* ---- SNOBOL4 pattern family (SN4-PAT) ---------------------------------------------------
     * Re-added onto the post-GZ#5 spine. Amputated wholesale by 8de0fb46 (GZ#5 ENUM-AMPUTATION);
     * design recovered from parent 41b53078. Two families + one sealed-blob ref, mirroring SPITBOL:
     *   IR_MATCH_*   = MATCHERS: the inline needle. One node per pattern element, wired by γ (success)
     *                  / ω (failure) ports; IR_MATCH_ALTERNATE builds the backtrack tree, IR_MATCH_SEQUENCE
     *                  threads concatenation, IR_MATCH_ASSIGN_IMM/_COND do `$`/`.`. Used when a
     *                  pattern is matched directly (`SUBJECT PAT [= REPL]`) — lower_pat_node emits these.
     *   IR_PATTERN_* = STITCH boxes ONLY (D7 pivot d7ba0fd9 → RT build/stitch 52fce031): the per-element
     *                  builders were ABANDONED — FZ-3/FZ-4 constant folding freezes every invariant
     *                  subpattern to IR_REF_INVARIANT, so only VARIANT parts get stitched (CAT/ALT) around
     *                  sealed blobs; + CAPTURE passthrough, DEFER, and IR_DTP_ASSIGN for stored `.`/`$`.
     *   IR_REF_INVARIANT = a compile-time constant-folded pattern sealed into a RO blob (FZ-3,
     *                  18133720): an all-constant (VARIANT-free) subpattern is built ONCE, not per-match.
     * Re-seating is incremental (see GOAL-IR-IMMUTABLE-EMIT.md SN4-PAT ladder): enum first (here),
     * then templates back into the Makefile + emitter dispatch, then lower_snobol4.c, matcher by matcher.
     * Inert until lower_snobol4.c emits them, so this addition keeps Icon + the current build green. */
    IR_MATCH,               /* base/abstract matcher tag */
    IR_MATCH_LIT,
    IR_MATCH_ANY,
    IR_MATCH_NOTANY,
    IR_MATCH_SPAN,
    IR_MATCH_SPAN_VAR,
    IR_MATCH_BREAK,
    IR_MATCH_BREAKX,
    IR_MATCH_LEN,
    IR_MATCH_POS,
    IR_MATCH_RPOS,
    IR_MATCH_TAB,
    IR_MATCH_RTAB,
    IR_MATCH_ARB,
    IR_MATCH_ARBNO,
    IR_MATCH_REM,
    IR_MATCH_BAL,
    IR_MATCH_FENCE1,
    IR_MATCH_ABORT,
    IR_MATCH_SEQUENCE,
    IR_MATCH_ALTERNATE,
    IR_MATCH_ASSIGN_IMM,
    IR_MATCH_ASSIGN_COND,
    IR_MATCH_ASSIGN_SAVE,   /* SN4-PAT-3h: phase-0 cursor SAVE for multi-element capture (its own δ-slot) */
    IR_MATCH_ATP,
    IR_MATCH_CALLOUT,
    IR_MATCH_DEFER,
    IR_MATCH_PATREF,         /* OP-SPLIT (Lon s200 directive, landed s21x-f): the EAGER twin of IR_MATCH_DEFER -- a stored-pattern reference by bare NAME (TT_VAR in pattern position, plus the PATTMP$ eager-call materialization), built from a pattern that CANNOT name itself (manual p.122: only `*` permits the self-reference that recurses).  IR_MATCH_DEFER now carries ONLY the `*` unevaluated arm.  This slice is BEHAVIOR-PRESERVING: every consumer treats the pair identically, including deep-arrival (PATCTX s21x-e: gamma-retention, not star-ness, is the pinning property, so BOTH stay deep in the default regime); the opcode is the provenance carrier that retires the s199 dstar side table and is the classifier axis the dynamic-flat glue work will read under the statement-frame regime. */
    IR_MATCH_VALUE,         /* SN4 kill-manufactured-names (2026-07-22): match a pattern VALUE carried in operand[0]
                             * (a fresh slot, drive_value_slot) instead of a manufactured global name; the eager
                             * TT_FNC pattern-position call lowers its result once into operand[0] and this node
                             * reads FR(op_a_slot) and matches it — DT_P runs the compiled pattern fn, a scalar is
                             * a literal match — no NV_SET/NV_GET, no global slot, no thunk-proc registration (that
                             * per-occurrence name minting is the GLOBAL_MAX/zls flood blocking beauty self-host).
                             * Structural clone of IR_MATCH_DEFER at every plumbing site; only the value SOURCE
                             * differs (operand slot vs op_sval name), so it never owes a *X/DT_X call transfer. */
    IR_MATCH_BEGIN,
    IR_MATCH_END,       /* BB-OWNED-ζ statement-scope pivot: α reads the saved rt_zls_mark() pointer from
                             * its operand[0] (the statement's own IR_MATCH_BEGIN node, via drive_value_slot,
                             * same operand[0]-owner convention IR_MATCH_ARBNO phases 1/2/4/5 already use to
                             * read role 0's slot) and calls rt_zls_release_to, then falls through γ.  Spliced
                             * as the pattern's own new tail (sno_lower_match passes this node's α as `succ`
                             * to the outermost sno_pat_node call, with this node's own γ = the statement's
                             * true sJ) so the success exit gets release_to the same way IR_MATCH_BEGIN's ω
                             * already gives the failure exit release_to for free -- HEAD owns fail (single
                             * fixed choke point, the scanner's own exhaustion); RELEASE owns success (which
                             * has no fixed choke point of its own, since it's whichever element the pattern
                             * happens to end on -- this node IS that fixed point, added for exactly this). */
    IR_MATCH_REPLACE,       /* SN4-REPL (doctrine stages 4/5): the statement's replacement splice, α-only like
                             * RELEASE.  operands: [0]=head (start@FR(slot), end stashed@FRQ(slot+24) by RELEASE
                             * when its dval=1), [1]=replacement value node, [2]=subject value node (raw DESCR
                             * quads, coerced runtime-side exactly as rt_match_enter does); sval=subject var name.
                             * α marshals rt_match_replace(name, sub_lo, sub_hi, start, end, &repl) which builds
                             * prefix+repl+suffix and NV_SET_fn's it; γ = the statement's true sJ.  Sits AFTER the
                             * replacement expression chain (RELEASE.γ → repl chain → this) so a repl expr that
                             * nests its own match cannot stale the end cursor — end left r14 at RELEASE time. */
    IR_MATCH_RETRY,
    IR_MATCH_ADVANCE,
    IR_REF_INVARIANT,       /* sealed constant-folded pattern blob (FZ-3) */
    IR_PATTERN_CAT,      /* STITCH-CAT box (B6 409f62a9/a59f38b8): rt_pattern_stitch_cat */
    IR_PATTERN_ALT,      /* STITCH-ALT box (B3 7a12aedd): rt_pattern_stitch_alt */
    IR_PATTERN_CAPTURE,  /* passthrough since FZ-4 6141434 (Raku passthrough kept) */
    IR_PATTERN_DEFER,    /* *EXPR deferred build */
    IR_DTP_ASSIGN,       /* stored-pattern `.`/`$` capture (B3, DTP frag — see src/include/dtp.h) */
    IR_GOTO_DEFERRED,         /* EVAL/CODE (manual Ch.9): goto whose target label is unknown at lower time — a label
                          * defined only inside a runtime-compiled CODE fragment, a `$X` indirect label name, or a
                          * variable holding a CODE value (the lexer maps `:<C>` direct-goto brackets onto the
                          * plain-name form).  sval = the name as written (`$`-prefixed when indirect).  α marshals
                          * rt_goto_transfer(name), which resolves via the runtime label registry (fragment labels,
                          * which per the manual OVERRIDE same-named main labels), then the LBL__ pseudo-proc table
                          * (main-program labels, exported only when the program uses CODE), then a DT_C variable,
                          * else faults "transfer to undefined label".  The transferee runs nested; when it
                          * terminates, control cascades back here and γ (wired to the graph's exit) unwinds this
                          * chain too — SNOBOL4 gotos never resume their source, so return-here means END. */
    /* ---- RK-GRAM grammar-box family (RK-GRAM-3a) ---------------------------------------------------------------------------
     * Native recursive-descent grammar leaves/edges for Raku `rule`/`token`/`regex` bodies, replacing the runtime gram_expand
     * flatten-to-NFA stopgap (by_name_dispatch.c). Register contract = the ARCH-ICON String-scanning family verbatim: R13=Σ
     * subject base, R14=δ cursor (callee-saved so it stays ambient across subrule recursion), R15=Δ end bound. Four ports keep
     * pattern-lang meaning: α=fresh entry, β=resume/backtrack-retry, γ=match advanced δ, ω=fail. Choice points save δ into the
     * per-activation ζ frame (a δ-snapshot slot, NOT a value stack). INERT until the RK-GRAM-3a lowering seam in lower_raku.c
     * emits them (only migrated rule shapes take this path; others keep the string fallback) — behavior-neutral, build/smoke green. */
    IR_GLIT,             /* literal-match: match fixed string sval at [Σ+δ], bounds δ<Δ; γ advances δ by len, ω on mismatch/EOS */
    IR_GCC,              /* char-class-match: match one char at [Σ+δ] against sealed cset (sval), advance δ by 1; γ on member, ω else */
    IR_GSUBRULE,         /* subrule call: recurse into named rule's box graph (sval=rule name) with Σ/δ/Δ ambient in R13/R14/R15 */
    IR_BOUND,            /* Op_Mark analogue (icon-master interp.r): α saves rsp into its ζ slot then jmp γ (bounded-expression entry); the paired IR_UNMARK restores it, discarding retained suspension cells the bound abandoned */
    IR_UNMARK,
    IR_STATEMENT,        /* s23k ZW-5 slice 0 (DORMANT — lower does not mint yet): the ENTIRE-statement bracket box. alpha = bare label s<stno>_alpha (zero ceremony); gamma = the one whack add rsp,K_total; omega = per-distinct-arrival-depth stub ladder. Design of record: GOAL-SNOBOL4-BB.md s23k addendum + FINDING-2026-08-02d. */           /* Op_Unmark analogue: α restores rsp from operand[0]'s (the paired IR_BOUND's) ζ slot then jmp γ — `rsp = efp-1` in interp.r Op_Unmark; the every-do bounded-exit cut that reclaims abandoned FC carves */
    IR_STATEMENT_BEGIN,  /* s26 (Lon directive 2026-08-02): statement HEAD bracket -- alpha = the statement's attribution label (BEGIN..END brackets the statement's byte range for perf attribution), body = frame setup IF NECESSARY (the FINDING-2026-08-02h STF-bracket population; K=0 pure relay until that rung lands), gamma = jmp to the statement's first BB.  Minted by the post-loop shim in lower_snobol4.c under the zw5_on() regime gate. */
    IR_STATEMENT_END,    /* s26 (Lon directive): statement TERMINAL -- the ZW-5 trailer re-kinded into the pair; gamma jmp carries op_zgpop (the SOLE release authority, staged at the choke from the planner -- semantics unchanged from IR_STATEMENT), omega = the fail-side arrival, dead until the BEGIN frame makes restore depth-free (the per-depth ladder is deprecated and is NOT coming back). */
    IR_OP_COUNT
} IR_e;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int ir_is_scan_kind(IR_e t) { (void) t; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int ir_is_call_kind(IR_e t) {
    return t == IR_CALL_PROC_STAGED || t == IR_CALL_BUILTIN || t == IR_CALL_BUILTIN_GEN || t == IR_CALL_BUILTIN_ICON || t == IR_CALL_BUILTIN_SNOBOL4 || t == IR_CALL_BUILTIN_PROLOG || t == IR_CALL_VALUE;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline IR_e ir_norm_call_kind(IR_e t) { return ir_is_call_kind(t) ? IR_CALL : t; }
typedef enum { BINOP_CAT_ARITH = 0, BINOP_CAT_RELOP = 1, BINOP_CAT_CONCAT = 2, BINOP_CAT_XREP = 3 } binop_cat_t;
typedef struct IR_t IR_t;
typedef struct IR_graph_t IR_graph_t;
typedef struct { IR_t * node; char sz[4]; } IR_ref_t;
struct IR_t {
    IR_e         op;
    IR_ref_t     γ;
    IR_ref_t     ω;
    IR_t       ** operands;
    int           n_operands;
    int           in_scan;   /* ICN-SCAN-STRUCT: 1 iff this node is lexically inside a ? scan body. Set structurally at lower time (TT_SCAN range-mark); read by emit_drive to set g_scan_regs_live per-node. Replaces the emit-order-dependent running counter, whose value depended on the four-port BFS walk order and dropped to 0 mid-body when a scan's leave box (IR_SCAN) was emitted before in-scan keyword-assigns reached via an IR_ACTIVATE fail edge. */
    int           seal;      /* s137 OVER-SEAL (Lon ruling): 1 iff this IR_MATCH_DEFER's target is a STATICALLY right-sealed stored pattern (single-write name whose tree's rightmost element is a fence form, sno_pat_right_sealed with the VAR/DEFER chase) — the defer's γ is then a fence-demarked sync point in the CALLER's activation: α saves an rsp watermark, γ/ω restore it (whacking the callee's entire retained subtree — its frame, its suspend record, everything it carved), and β is an unconditional exhaust (the sealed blob's resume is dead by NCB-2/SZ-1, so the record the whack destroys is never read). Set at lower time (defer build sites), promoted to templates as op_seal by the emit_drive DEFER case. */
    int           pat_static; /* ZD-5 PATREF/DEFER STATIC-SHAPE (s23i): 1 iff this IR_MATCH_DEFER/IR_MATCH_PATREF's target name eligibly resolves (g_sno_seal: single write, fz-safe, main lowering) to a tree whose TRANSITIVE closure over spine-position VAR references contains ZERO TT_DEFER — i.e. the stored pattern provably cannot re-enter any blob (manual p.122: only `*` recurses), so its match runs enter-scan-return with no unmodeled-depth re-entry.  Consumed by zd_plan's dynamic-box scan: a static-shape DEFER/PATREF no longer vetoes the statement quartet's ZD arming; 0 (the calloc default, and every composite `*expr` site) keeps the declined regime — the 135/136 recursive class verbatim.  Set at the three lower_snobol4.c defer/patref build sites; NOT promoted to templates (the interior box itself stays flat either way). */
    union { const char * sval; int64_t ival; double dval; };
};
#define IR_LIT(nd)  (*(nd))
typedef struct {
    const char * sval;
    int64_t      ival;
    double       dval;
} IR_lit_t;
struct IR_graph_t {
    IR_t    * entry;
    IR_t   ** all;
    int            n;
    int            max;
    int            nslots;
    int            nvalue_slots;
    int            jcon_value_region;
    int            nparams;
    const char  ** pnames;
    int            nlocals;
    const char  ** lnames;
    struct IR_vslot_t { const char * name; int off; } * vslots;
    int            n_vslots;
    int            resume_slot;
    int            resumable_callable;
    int            deterministic;
    int            zeta_mark_slot;
    IR_t         * body_root;
    #define AG_RING 16
    DESCR_t        ring[AG_RING];
    int            ring_head;
    int            ring_depth;
};
IR_graph_t * IR_alloc(int max_nodes);
int ir_varslot_of(const IR_graph_t * g, const char * name);
int graph_has_local(const IR_graph_t * g, const char * name);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline IR_t * ir_call_arg(const IR_t * nd, int j) {
    if (!nd || j < 0) return NULL;
    return (nd->n_operands > 0 && j < nd->n_operands) ? nd->operands[j] : NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline IR_t * ir_pair_arg(const IR_t * nd, int j) {
    if (!nd || j < 0 || j > 1) return NULL;
    return (nd->n_operands > 0 && j < nd->n_operands) ? nd->operands[j] : NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline void ag_ring_push(IR_graph_t * cfg, DESCR_t v) {
    if (!cfg) return;
    cfg->ring_head = (cfg->ring_head + 1) % AG_RING;
    cfg->ring[cfg->ring_head] = v;
    if (cfg->ring_depth < AG_RING) cfg->ring_depth++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline DESCR_t ag_ring_peek(const IR_graph_t * cfg, int k) {
    if (!cfg || k < 0 || k >= cfg->ring_depth) return FAILDESCR;
    int idx = (cfg->ring_head - k + AG_RING) % AG_RING;
    return cfg->ring[idx];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline void ag_ring_clear(IR_graph_t * cfg) {
    if (!cfg) return;
    cfg->ring_head  = -1;
    cfg->ring_depth = 0;
}
IR_t       * IR_node_alloc(IR_graph_t * cfg, IR_e t);
int          ir_operand_push(IR_t * nd, IR_t * child);
int  ir_node_produces_value(IR_e op);
void         bb_reset(IR_graph_t * cfg);
void         IR_free(IR_graph_t * cfg);
void         bb_print(const IR_graph_t * cfg, FILE * fp);
void         bb_print_v(const IR_graph_t * cfg, FILE * fp, int verbose);
const char * bb_op_name(IR_e k);
#endif
