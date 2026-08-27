#!/usr/bin/env bash
# scripts/test_gate_pl_no_new_global.sh — GUARANTEE no new global variable enters the Prolog path.
#
# Enforces the FACT RULE "NO NEW GLOBAL FOR ANY NOT-NEEDED STRUCTURE" (GOAL-PROLOG-BB.md).
# The four-port + frame-cell model means Prolog needs exactly ONE runtime spine — the TRAIL.
# Every other DESIGN §10 "NOT NEEDED" structure (choice-point stack, environment stack, value
# stack, trail-mark snapshot stack, exception-frame stack, meta-rail engine, …) must NOT be
# re-introduced as a global. This gate is the standing guard.
#
# Two checks over the Prolog-owned source set:
#   (1) NEW-GLOBAL  : any g_* symbol that is NOT on the frozen allowlist  → FAIL (names it).
#                     This is the hard guarantee: you cannot add a global without amending the
#                     allowlist below, and the allowlist forbids adding one to build a §10 item.
#   (2) DOOMED-RATCHET : the count of LEGACY-DOOMED g_* still present must be <= FLOOR.
#                     These already exist (they ARE §10 structures, all in/around resolution.c,
#                     slated for deletion per PL-BB-DEMOLITION). The floor only ever DROPS; when
#                     you delete a doomed global, lower DOOMED_FLOOR. End state: 0.
#
# Self-contained. Run from anywhere. Authors: LCherryholmes · Jeffrey Cooper M.D. · Claude Sonnet
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
cd "$ROOT"

# ---- the Prolog-owned source set the gate polices ------------------------------------------------
PL_FILES=$(ls \
  src/runtime/unification.c \
  src/runtime/arithmetic.c \
  src/runtime/builtins/resolution.c \
  src/lower/lower_prolog.c \
  src/frontend/prolog/*.c \
  src/templates/bb/bb_cell_*.cpp \
  src/templates/bb/bb_det_*.cpp \
  src/templates/bb/bb_query_frame.cpp \
  src/templates/bb/bb_callee_frame.cpp \
  2>/dev/null)

# ---- TIER 1: SANCTIONED — frozen, legal forever (the 4 survivors + compile-time-only state) ------
# g_pl_det_done/_n/_v   RSP-F-1 DET-classifier lower-time memo (s96) — compile-time only, written
#                        and read exclusively inside lower_prolog.c before emit; never runtime state.
#                        Sanctioned s99 after notarizing the gate FAILed at pristine 733e9c36.
# g_resolve_trail        THE TRAIL — the one Prolog-specific runtime spine (DESIGN §10 survivor).
# g_pl_pred_table/_n     clause DATABASE store — a heap, not a stack ("we need *a* clause store").
# g_rt_pl_nb/_n          nb_setval/nb_getval store — a global mutable var IS the feature, by defn.
# g_stage2               the stage2 PROGRAM object — compile/emit-time, freed before run (ir_delete_all).
# g_pl_nl_arith/_builtins  const char* name tables read at LOWER time only — not runtime state.
# g_pl_throw_ball        the single IN-FLIGHT thrown term (one value, like errno) — NOT the §10 #7
#                        exception-frame STACK; the catch FRAMES are box frame cells, only the ball
#                        crossing a C-call return needs a global. Cleared on catch-match / consumed once.
# g_pl_dyn_pred_table/_n/_cap  DYNAMIC clause store (assert/retract/abolish) — a heap clause DB, the
#                        §10-sanctioned "we need *a* clause store" (the mutable sibling of the static
#                        g_pl_pred_table). NOT a control/value stack: per-activation control still lives
#                        in box frame cells + the trail. The store holds Term* clause copies keyed by
#                        (functor-name, arity); ASSERTZ inserts, RETRACT scan+deletes, ABOLISH clears.
# g_pl_trail             THE TRAIL, cell form — PL-DESCR-2 sub-flip 2's inline-cell sibling of g_resolve_trail:
#                        (cell_addr, old 16-byte word) entries for the live DESCR_t LVA path. Same DESIGN §10
#                        survivor role (the one Prolog runtime spine); g_resolve_trail is kept only for the
#                        legacy by-Term* helpers until PL-BB-DEMOLITION removes them. Not a value/control stack.
# g_uinfix/_n/_cap       PARSE-TIME user-operator table (the `:- op(P,xfx|xfy|yfx,Name)` directive registry),
#                        in src/frontend/prolog/prolog_parse.c. Compile-time-only metadata read by find_binop
#                        during PARSING; never touched at runtime — like g_pl_nl_* it is not a §10 control/value
#                        spine. Grows by realloc only while consuming op directives between clauses.
# g_pl_env_area          PL-AREAS-3 ENVIRONMENT area (E / R15) — the mmap bump region rt_enter allocates callee frames
#                        off, with O(1) backtrack-reset + deterministic-pop reclamation. SANCTIONED per the ratified
#                        PIVOT (top of GOAL-PROLOG-BB.md): a contiguous DATA region (the WAM's memory half, language-
#                        neutral), NOT the §10 #2 environment-CONTROL stack — control stays in the four ports + frame
#                        cells; the boxes drive control, this is a data arena (the direct sibling of g_pl_trail's area).
# g_pb_fresh_ctr         LOWER-TIME fresh bound-var NAME counter ("_$B%d") in prolog_lower.c. Compile-time-only, like
#                        g_uinfix / g_pl_nl_* — never touched at runtime; a monotonic name source, not a §10 spine.
# g_pl_disj_ctr          LOWER-TIME aux-predicate NAME counter ("$disj%d") in lower_prolog.c — the exact sibling of
#                        g_pb_fresh_ctr. Minted by the cut-free (A;B;C) -> aux-predicate transform (762aafc5) when a
#                        clause-internal disjunction is lifted into a synthetic TT_CHOICE; the name lands in
#                        g_stage2.resolve_pred_table (itself SANCTIONED compile/emit-time, freed before run). A static
#                        int in one lower file, never read at runtime, holds no control/value state — not a §10 spine.
#                        Allowlisted 2026-07-25: un-listed pre-existing red since 762aafc5 (2026-07-19, 229 commits),
#                        same silent-red class as g_pl_functor_slot_ctr. Gate FAILed at pristine HEAD 04eeaa8f.
# g_pl_functor_slot_ctr  functor/3 fresh-var PRINT-ID counter (unification.c). A single monotonic int source for naming
#                        the vars functor(F,N,T) creates so the writer can print them; write-side metadata, NOT a control
#                        stack. Added by PL-ISO-5 (cce2b25e); allowlisted 2026-07-13 (was an un-listed pre-existing red).
# g_pl_copy_slot_ctr     copy_term/2 fresh-var PRINT-ID counter + its mode gate (unification.c). Same print-id class as
# g_pl_copy_slot_mode    g_pl_functor_slot_ctr; the mode flag scopes id-assignment so sort/@< copy-walks leave ids untouched.
#                        Two scalars, not a §10 stack. Added by PL-ISO-5 (cce2b25e); allowlisted 2026-07-13.
# g_core_errjmp_stk      SHARED exception-longjmp infrastructure — 64-deep jmp_buf stack covering the whole runtime
# g_core_errjmp_n        (arithmetic.c, by_name_dispatch.c). Defined in the core binary (scrip.c / core.c), NOT a
#                        §10 Prolog-specific control stack; it is the language-neutral catch-frame used by every
#                        frontend's error path (Icon, SNOBOL4, Prolog alike). Referenced via extern in arithmetic.c
#                        (always was) and now also in the dop_call/rt_call_arr gateways (REGAIN-2 s102). Pre-existing
#                        in the gate-scanned set via arithmetic.c; first made visible by the s102 floor-fix extern refs.
# g_pl_flags             ISO PROLOG-FLAG STORE (unification.c). The `current_prolog_flag/2` + `set_prolog_flag/2`
# g_pl_flags_n           registry — genuinely per-process mutable configuration state (double_quotes/unknown are RW;
#                        bounded/max_integer/min_integer are RO). A small fixed keyed table, NOT a §10 control stack;
#                        the direct sibling of g_uinfix's parse-time op registry. Sanctioned by Lon directive (PL-ISO-12).
SANCTIONED="
g_resolve_trail
g_pl_trail
g_pl_env_area
g_pl_det_done
g_pl_det_n
g_pl_det_v
g_pl_pred_table
g_pl_pred_n
g_rt_pl_nb
g_rt_pl_nb_n
g_stage2
g_pl_nl_arith
g_pl_nl_builtins
g_pl_throw_ball
g_pl_dyn_pred_table
g_pl_dyn_pred_n
g_pl_dyn_pred_cap
g_uinfix
g_uinfix_n
g_uinfix_cap
g_pb_fresh_ctr
g_pl_disj_ctr
g_pl_functor_slot_ctr
g_pl_copy_slot_ctr
g_pl_copy_slot_mode
g_core_errjmp_stk
g_core_errjmp_n
g_pl_flags
g_pl_flags_n
"

# ---- TIER 2: LEGACY-DOOMED — grandfathered, ratchet to zero (each IS a §10 NOT-NEEDED structure) -
# Every symbol here is a control-stack / engine residue in or around resolution.c. They are already
# UNREACHABLE from GZ dispatch but still LINK. Deleting them is PL-BB-DEMOLITION. As each goes, drop
# DOOMED_FLOOR by one. NOTHING may be ADDED here — this list is closed and only shrinks.
DOOMED="
g_resolve_bfr
g_resolve_cp_stamp
g_resolve_catch_top
g_resolve_catch_stack
g_resolve_mark_top
g_resolve_mark_stack
g_resolve_cut_flag
g_resolve_cut_barrier
g_resolve_bb_table
g_resolve_bb_count
g_meta_compat
g_meta_builtins
g_resolve_active
g_resolve_exception
g_resolve_nb_store
g_resolve_nb_count
"
DOOMED_FLOOR=14   # PL-DESCR-2 sub-flip 2 deleted g_resolve_env (the Term* shadow env). RATCHET — only ever lower.

# ---- enumerate distinct g_* actually present -----------------------------------------------------
PRESENT=$(grep -rhoE '\bg_[a-zA-Z_][a-zA-Z0-9_]*' $PL_FILES 2>/dev/null | sort -u)

in_set() { grep -qx "$1" <<< "$(tr -s ' \n' '\n' <<< "$2")"; }

new_globals=""
doomed_present=0
for sym in $PRESENT; do
    if in_set "$sym" "$SANCTIONED"; then
        continue
    elif in_set "$sym" "$DOOMED"; then
        doomed_present=$((doomed_present + 1))
    else
        new_globals="$new_globals $sym"
    fi
done

rc=0
echo "=== PL no-new-global gate ==="
if [ -n "$new_globals" ]; then
    echo "  FAIL  NEW GLOBAL(S) — not on the allowlist:$new_globals"
    echo "        A new g_* entered the Prolog path. If it implements ANY DESIGN §10 NOT-NEEDED"
    echo "        structure (choice-point/env/value/mark/exception stack, meta-rail, …) it is a"
    echo "        FACT-RULE violation: put the state in a FRAME CELL or the TRAIL, not a global."
    echo "        If it is genuinely a survivor (heap store / compile-time const), add it to"
    echo "        SANCTIONED in this gate WITH a one-line justification."
    rc=1
else
    echo "  PASS  no new globals (every g_* is SANCTIONED or DOOMED)."
fi

if [ "$doomed_present" -gt "$DOOMED_FLOOR" ]; then
    echo "  FAIL  DOOMED-RATCHET broke: $doomed_present present > floor $DOOMED_FLOOR (a doomed pattern re-expanded)."
    rc=1
else
    echo "  PASS  doomed-ratchet: $doomed_present / floor $DOOMED_FLOOR (target 0 via PL-BB-DEMOLITION)."
fi
exit $rc
