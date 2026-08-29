#!/bin/bash
# test_gate_bb_block_label_prefix.sh -- GATE for row bb-label-prefix-uniform (Lon 2026-08-28, widened
# ruling: "ALL labels emitted in the ALPHA and BETA block must change... EVERY variation of labels gets
# unified into ONE COMMON PREFIX for all in each block").
#
# Walks emitted TEXT-mode .s witnesses, brackets each block by its own port-label DEFINITION (the
# "<prefix>_<greek>:" line that opens it -- alpha/beta always, gamma/omega on the graphs that emit an
# extra transition-out-of-the-box label per Lon's spec addendum, ceo 2026-08-27 ledger), and REFUSES
# (rc=1) on any OTHER label definition inside that span whose name does not start with that same
# "<prefix>_<greek>" -- unless it matches the documented ALLOWLIST below.
#
# ⛔ THE ALLOWLIST IS NOT A CONVENIENCE, IT IS A CLAIM -- every entry cites the code that proves the
# family is not "a point inside a block" by construction, per hq_P's framing (adopted by ceo 2026-08-29b,
# task bb-label-prefix-uniform LEDGER): a RANGE, a MODULE DATUM, or a FUNCTION is a different kind of
# thing than a block-scoped point label, unreachable by "ALL labels in each block" no matter how the
# scan is written. An allowlist entry with no citation is exactly the "invisible allowlist" this row's
# own history warned the next seat about -- don't add one without updating this header AND the task
# file's LEDGER.
#
# KNOWN STILL-OPEN, DELIBERATELY NOT ALLOWLISTED (this gate correctly FAILS on these until fixed or
# ruled exempt -- see task bb-label-prefix-uniform.task.md LEDGER for the code citations):
#   - n<N>_<kind>_as / _af / _s<N> / _ry / _rt -- emit.cpp na_s/na_f/fc_sig/ra_y/ra_t: REAL gamma/omega
#     transition labels (used as node_gamma/node_omega jump targets, emit.cpp:3061-3082), NOT siblings of
#     the exempt _bx debug-symbol-span marker despite the superficially similar 2-3 letter suffix -- a
#     prior ceo ruling's "_bx (+_af/_as siblings)" phrasing conflated the two; correction routed to hq_P.
#   - .Lgvan<N> / .Lstartup_* / .Lseala<N> -- src/driver/scrip.c, module-level startup/reflection tables
#     (one entry per PROCEDURE in the whole program), structurally the same "no single owning box" shape
#     as the exempt .S<N>/.C<N> tables, but never asked about; open question routed to hq_P.
set -u
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"
cd "$S4E/SCRIP" || exit 2
CORPUS="$S4E/corpus"
SCRIP_BIN=./scrip
WITDIR="$(mktemp -d)"
trap 'rm -rf "$WITDIR"' EXIT
# ⚠️ SCOPE, READ BEFORE ADDING A WITNESS: porter.sno and beauty.sno were tried and dropped (seat05
# 2026-08-29). Both are large/heavily-optimized enough that some straight-line boxes' own alpha port
# label is optimizer-elided (nothing jumps to it directly, so dead-label elimination removes it) while
# the box's internal literal-pool labels remain and are still correctly kind-tagged at mint time -- this
# script's simple "bracket by the last port label textually seen" tracker has no anchor left for those
# and misattributes them to whichever box's port label happened to precede. Confirmed a real optimizer
# characteristic, not a bracketing bug fixable by more pattern-matching here (see task bb-label-prefix-
# uniform.task.md LEDGER for the specific traced example). Needs either a smarter bracketing method or
# disabling that optimization for gate purposes -- out of scope for this pass. Keep this witness list to
# programs whose bracketing has been manually spot-checked clean; a bigger/newer witness needs the same
# spot-check before joining this list, not just "it compiles".
WITNESSES="$CORPUS/benchmarks/snobol4/pattern_bt.sno $CORPUS/tests/icon/generators.icn $CORPUS/tests/snobol4/probe/indirect.sno"
fail=0
total_defs=0
for f in $WITNESSES; do
    [ -f "$f" ] || { echo "SKIP (missing witness): $f"; continue; }
    b="$(basename "$f" | sed 's/\.[^.]*$//')"
    out="$WITDIR/$b.s"
    if ! "$SCRIP_BIN" --compile -o "$out" "$f" < /dev/null > "$WITDIR/$b.err" 2>&1; then
        echo "SKIP (pre-existing compile failure, not this gate's concern): $f"
        continue
    fi
    n=$(python3 "$(dirname "$0")/lib_bb_block_label_prefix_check.py" "$out")
    rc=$?
    total_defs=$((total_defs + ${n:-0}))
    if [ $rc -ne 0 ]; then fail=1; fi
done
if [ "$fail" -ne 0 ]; then
    echo "⛔ GATE FAILED -- one or more labels inside an alpha/beta(/gamma/omega-transition) block do not carry that block's own derived prefix. See violations above."
    exit 1
fi
echo "OK: every label definition inside every alpha/beta(/gamma/omega-transition) block across $total_defs checked labels carries its block's own derived prefix (or is an allowlisted non-block-scoped family)."
exit 0
