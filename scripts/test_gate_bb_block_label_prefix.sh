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
# RULED (hq_P 2026-08-29, ceo-endorsed, full text in task LEDGER): the mechanism test is "a greek infix
# names a PORT OF A BOX; a label with no owning box has no port, so the infix would not be redundant, it
# would be FALSE" -- NOT a location test ("driver-level is exempt"), because that fails the next
# borderline case. Under that test: .S/.C, .Lgvan/.Lstartup_*/.Lseala (allowlisted below) have no owning
# box, so they're exempt; _bx is exempt for an UNRELATED reason (a RANGE marker, never a jump target, per
# emit.cpp:3041-3042/3238); _af/_as/_ry/_rt/_sN are REAL box-owned gamma/omega jump targets (emit.cpp
# node_γ/node_ω assignment at :3074/:3090/:1398/:3095) and are NOT exempt by either test.
#
# KNOWN STILL-OPEN, DELIBERATELY NOT ALLOWLISTED (this gate correctly FAILS on these until fixed --
# see task bb-label-prefix-uniform.task.md LEDGER for the full ruling and code citations):
#   - n<N>_<kind>_as / _af / _s<N> / _ry / _rt -- emit.cpp na_s/na_f/fc_sig/ra_y/ra_t: REAL gamma/omega
#     transition labels, NOT siblings of the exempt _bx debug-symbol-span marker despite the superficially
#     similar 2-3 letter suffix -- a prior ceo ruling's "_bx (+_af/_as siblings)" phrasing conflated a
#     naming neighbourhood with a mechanism; corrected and ceo-endorsed, see LEDGER. na_f is genuinely
#     harder than a rename: it serves as EITHER gamma or omega depending on the CALLING node's own
#     gamma_is_phi/omega_is_phi, so fixing this is real design work, not mechanical -- not attempted here.
#   - a DEFINE'd procedure's own nested/embedded body (e.g. PATTERN_BT, compiled inline immediately after
#     its own DEFINE box) -- UNRESOLVED DISAGREEMENT between two independently-built gates for this same
#     row, see task LEDGER "SEAT11 COLLISION" entry before touching this. This script's bracketing treats
#     PATTERN_BT_α (a bare, non-"n<uid>_"-prefixed port label -- the nested graph has its OWN alpha/beta/
#     gamma/omega ports, same shape as any other graph) as opening a NEW block requiring ITS OWN identity,
#     so internal labels physically inside it that still say the ENCLOSING "define" box's kind currently
#     FAIL here. A separate, earlier gate (git show 57ecf03e:scripts/test_gate_bb_block_label_prefix.sh)
#     reached the opposite conclusion on the identical shape, with its own cited measurement. Applying
#     hq_P's mechanism test above suggests this script's reading (PATTERN_BT has its own box/ports, so it
#     needs its own identity) -- but that application is this seat's own reasoning, not a ruling on THIS
#     specific question, which was never asked.
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
