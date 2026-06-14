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
  src/parser/prolog/*.c \
  src/emitter/BB_templates/bb_cell_*.cpp \
  src/emitter/BB_templates/bb_det_*.cpp \
  src/emitter/BB_templates/bb_query_frame.cpp \
  src/emitter/BB_templates/bb_callee_frame.cpp \
  2>/dev/null)

# ---- TIER 1: SANCTIONED — frozen, legal forever (the 4 survivors + compile-time-only state) ------
# g_resolve_trail        THE TRAIL — the one Prolog-specific runtime spine (DESIGN §10 survivor).
# g_pl_pred_table/_n     clause DATABASE store — a heap, not a stack ("we need *a* clause store").
# g_rt_pl_nb/_n          nb_setval/nb_getval store — a global mutable var IS the feature, by defn.
# g_stage2               the stage2 PROGRAM object — compile/emit-time, freed before run (ir_delete_all).
# g_pl_nl_arith/_builtins  const char* name tables read at LOWER time only — not runtime state.
SANCTIONED="
g_resolve_trail
g_pl_pred_table
g_pl_pred_n
g_rt_pl_nb
g_rt_pl_nb_n
g_stage2
g_pl_nl_arith
g_pl_nl_builtins
"

# ---- TIER 2: LEGACY-DOOMED — grandfathered, ratchet to zero (each IS a §10 NOT-NEEDED structure) -
# Every symbol here is a control-stack / engine residue in or around resolution.c. They are already
# UNREACHABLE from GZ dispatch but still LINK. Deleting them is PL-BB-DEMOLITION. As each goes, drop
# DOOMED_FLOOR by one. NOTHING may be ADDED here — this list is closed and only shrinks.
DOOMED="
g_resolve_env
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
DOOMED_FLOOR=15   # distinct doomed symbols present today; RATCHET — only ever lower this, never raise.

# ---- enumerate distinct g_* actually present -----------------------------------------------------
PRESENT=$(grep -rhoE '\bg_[a-zA-Z_][a-zA-Z0-9_]*' $PL_FILES 2>/dev/null | sort -u)

in_set() { echo "$2" | tr -s ' \n' '\n' | grep -qx "$1"; }

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
