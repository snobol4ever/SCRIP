#!/usr/bin/env bash
# test_gate_master_shrink_refusal.sh — the executable DONE-WHEN for row
# `snobol4-master-guard-sync-and-builder-shrink-refusal` (hq_C, 2026-08-30).
#
# WHAT IT PROVES, and why each arm exists:
#   A. NEGATIVE — a would-collapse rebuild REFUSES rc=2 and writes NOTHING. This is the arm that matters:
#      hq_P and seat14 each reproduced `rm ALL.* && rebuild` turning a 1576-entry cut-over master into TWO
#      entries, with the build reporting SUCCESS. The guard must stop `0 < pairs << base`.
#   B. POSITIVE — the guard can say YES. A criterion nobody has watched go green is not a criterion
#      (RULES.md § TWO-PART PROOF): if this arm is missing, a builder that refused unconditionally would
#      pass arm A forever and nobody would notice until a real rebuild was needed.
#   C. ORDERING — zero pairs over a healthy master is the SUCCESS state of consolidation, not a collapse,
#      and must exit 0 with the master intact. With the collapse test placed first, `0 * 4 < 97` refused
#      raku and prolog and told them not to delete ALL.* — correct advice for a disaster, addressed to a
#      success. This arm pins the order.
#   D. PROCEDURE — the cut-over scratch-verify procedure is documented in the builder's own header, not
#      only in law prose. A procedure that lives only in a task file is not reachable from the tool.
#
# ⛔ EVERY ARM RUNS ON A SCRATCH COPY. This gate must never be able to damage the live master — which is
# the very failure it exists to prevent. It reads `corpus/tests/snobol4` and writes only under mktemp.
#
# Usage: bash scripts/test_gate_master_shrink_refusal.sh [--informational]
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
cd "$ROOT"
GATE_NAME="master-shrink-refusal"
GATE_STRICT=1
. "$HERE/lib_gate.sh"
gate_parse_args "$@"

BUILDER="$ROOT/scripts/util_build_master_suite.py"
LIVE="$ROOT/../corpus/tests/snobol4"
gate_require "$BUILDER" "the master builder"
gate_require "$LIVE/ALL.sno" "the live snobol4 master"
gate_require "$LIVE/ALL.csv" "the live snobol4 attribute database"

FAILS=0
good() { echo "  ✓ $*"; }
bad()  { echo "  ✗ $*"; FAILS=$((FAILS + 1)); }

W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
mkscratch() {  # $1 = dest root; copies the live language tree under it
    rm -rf "$1"; mkdir -p "$1/corpus/tests/snobol4"
    cp -r "$LIVE/." "$1/corpus/tests/snobol4/" 2>/dev/null
}

echo "== A. NEGATIVE: a would-collapse rebuild must REFUSE rc=2 and write nothing"
mkscratch "$W/a"
before="$(md5sum "$W/a/corpus/tests/snobol4/ALL.sno" | cut -d' ' -f1)"
out="$(S4E_HOME="$W/a" python3 "$BUILDER" --lang snobol4 2>&1)"; rc=$?
after="$(md5sum "$W/a/corpus/tests/snobol4/ALL.sno" | cut -d' ' -f1)"
if [ "$rc" -eq 2 ] && printf '%s' "$out" | grep -q 'COLLAPSE'; then good "refused rc=2 naming COLLAPSE"
else bad "expected rc=2 with a COLLAPSE message, got rc=$rc"; fi
if [ "$before" = "$after" ]; then good "master byte-identical after the refusal (nothing written)"
else bad "⛔ THE MASTER CHANGED during a refusal — the guard wrote before checking"; fi

echo "== B. POSITIVE: the guard must be able to say YES"
mkscratch "$W/b"
python3 - "$W/b/corpus/tests/snobol4" <<'PY'
import sys, os
sys.path.insert(0, os.path.join(os.getcwd(), "scripts"))
import corpus_suite_harness as h
T = sys.argv[1]; p = os.path.join(T, "ALL.sno")
es = h.read_suite(p, os.path.join(T, "ALL.ref"),
                  in_path=h.sidecar_in_path(p), x_path=h.sidecar_xfail_path(p))
h.write_suite(es[:3], p, os.path.join(T, "ALL.ref"))
# ⛔ the sidecars still name entries that no longer exist; read_stdin_sidecar REFUSES on that (correctly),
# so a fixture that leaves them behind tests the sidecar guard instead of the shrink guard.
for f in ("ALL.in", "ALL.xfail"):
    q = os.path.join(T, f)
    if os.path.exists(q): os.remove(q)
PY
out="$(S4E_HOME="$W/b" python3 "$BUILDER" --lang snobol4 2>&1)"; rc=$?
n="$(grep -c ';END;\*' "$W/b/corpus/tests/snobol4/ALL.sno" 2>/dev/null || echo 0)"
if [ "$rc" -eq 0 ]; then good "proceeded rc=0 on a legitimately-absorbing tree (master now ${n} line-entries)"
else bad "expected rc=0 when pairs*4 >= base, got rc=$rc — the guard cannot say YES"; fi

echo "== C. ORDERING: zero pairs over a healthy master is SUCCESS, not collapse"
mkscratch "$W/c"
# ⛔ RECURSIVE, and -maxdepth 1 is why the first version of this arm failed: a copy of the language tree
# carries .sno files under config/ too, so a depth-1 delete left ONE pair behind, the builder correctly took
# the COLLAPSE branch at pairs=1, and the arm reported an ORDER REGRESSION THAT DOES NOT EXIST. The fixture
# was wrong, not the builder. Reaching "zero absorbable pairs" means every .sno but the master, at any depth.
find "$W/c/corpus/tests/snobol4" -name '*.sno' ! -name 'ALL.sno' -delete 2>/dev/null
out="$(S4E_HOME="$W/c" python3 "$BUILDER" --lang snobol4 2>&1)"; rc=$?
if [ "$rc" -eq 0 ] && printf '%s' "$out" | grep -q 'MASTER CURRENT'; then good "zero pairs over a full master exits 0 as MASTER CURRENT"
elif printf '%s' "$out" | grep -q 'COLLAPSE'; then bad "⛔ ORDER REGRESSION: zero pairs took the COLLAPSE branch — that refuses every language the moment its consolidation SUCCEEDS"
else bad "expected rc=0 MASTER CURRENT, got rc=$rc"; fi

echo "== D. PROCEDURE: the cut-over rule is in the builder's own header"
if grep -q 'CUT-OVER LANGUAGES RE-VERIFY IN A SCRATCH TREE' "$BUILDER"; then good "scratch-verify procedure documented in the builder"
else bad "the cut-over procedure is not in the builder header — it lives only in law prose"; fi

gate_verdict "$FAILS" "failed assertion(s)"
