#!/usr/bin/env bash
# test_gate_harness_refusal_is_rc2.sh -- THE ACCEPTANCE GATE for row harness-refusal-exit-code-unified-on-rc-2
# (ceo CEO-233 -> hq_T 2026-09-04, minted on hq_T's own receipt: two refusal codes in one harness).
#
# THE DEFECT: corpus_suite_harness.py's refuse() exited rc=3 by this file's own local convention, while
# lib_gate.sh's three-code ladder and every bash gate say a refusal is rc=2 -- 0 measured-and-clean · 1
# measured-and-red · 2 COULD NOT MEASURE. It became untenable when the stale-binary preflight landed IN the
# harness (row harness-and-ladder-runner-refuse-on-a-stale-binary-...) and correctly exited 2: one tool then
# carried TWO refusal codes, so no caller could ask "did it refuse?" without knowing which refusal it hit.
#
# ⛔ THE POINT OF THIS GATE IS THAT rc=1 MUST NOT MOVE. Unifying refusals downward onto 2 is one edit away from
# collapsing "could not measure" into "measured and red", which is the false-green this whole harness exists to
# prevent -- so ARM 3 grades a REAL suite with a REAL red and asserts rc=1 survives, and ARM 4 asserts a clean
# suite still exits 0. A gate that only proved "refusals are 2" would pass on a harness that returned 2 for
# everything.
#
# ARMS: 1 behavioural refusal (missing suite file) -> rc=2 · 2 the source carries ZERO sys.exit(3) arms and
# refuse() exits 2 · 3 a red board still exits 1 (the distinction that must survive) · 4 a clean run still
# exits 0 · 5 the staleness refusal, which was already 2, is unchanged -- both refusal paths now agree.
# EXIT: 0 all arms · 1 an arm failed · 2 REFUSED (no python3/harness/master, or an unbuilt tree).
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
S4E="${S4E_HOME:-$(cd "$ROOT/.." && pwd)}"
H="$HERE/corpus_suite_harness.py"
MASTER="$S4E/corpus/tests/rebus"
[ -f "$H" ] || { echo "⛔ REFUSED-TO-GRADE rc=2: harness not found at $H"; exit 2; }
command -v python3 >/dev/null 2>&1 || { echo "⛔ REFUSED-TO-GRADE rc=2: python3 not on PATH"; exit 2; }
[ -f "$MASTER/ALL.reb" ] && [ -f "$MASTER/ALL.ref" ] || { echo "⛔ REFUSED-TO-GRADE rc=2: rebus master absent at $MASTER -- arms 3/4 cannot distinguish a red board from a refusal"; exit 2; }
"$HERE/util_require_fresh.sh" --gate test_gate_harness_refusal_is_rc2 >/dev/null 2>&1 || {
    echo "⛔ REFUSED-TO-GRADE rc=2: this tree's binary is stale or unbuilt -- arms 3/4 run real programs. Run 'make'."; exit 2; }
W="$(mktemp -d "${TMPDIR:-/tmp}/gate_rc2.XXXXXX")" || { echo "⛔ REFUSED-TO-GRADE rc=2: mktemp failed"; exit 2; }
trap 'rm -rf "$W"' EXIT
fails=0; checks=0
ck() { checks=$((checks+1)); if [ "$1" = ok ]; then printf '  ok    %s\n' "$2"; else printf '  FAIL  %s\n' "$2"; fails=$((fails+1)); fi; }
echo "=== gate: the suite harness has ONE refusal code, and it is rc=2 ==="
echo "--- ARM 1: a refusal (missing suite file) exits rc=2, not rc=3 AND NOT rc=1 ---"
# ⛔ THIS ARM FOUND A LIVE DEFECT ONE LAYER BELOW THE ROW: a missing ALL.<ext>/ALL.ref reached Path.read_text()
# and died with a FileNotFoundError TRACEBACK, which Python exits 1 for -- and rc=1 here means "ran fine, some
# entries are RED". The case where NOTHING was graded returned the code for a measured red board. Cured in
# cmd_run with an existence check ahead of every reader; this arm keeps it dead. ⭐ Note the ceo's own DONE-WHEN
# for this row used `--lang snobol4`, which argparse REJECTS (snobol4 is spelled as the empty default), so it
# exited 2 from argparse and read GREEN on a harness that was still crashing. A behavioural arm has to name a
# form the tool actually accepts, or it grades the argument parser.
out="$(timeout 60 python3 "$H" run "$W/none.reb" "$W/none.ref" --lang rebus --modes m3,m4 2>&1)"; rc=$?
[ "$rc" = 2 ] && ck ok "missing suite file -> rc=2" || ck no "must REFUSE rc=2; got rc=$rc -- $(head -c 300 <<<"$out")"
grep -q 'REFUSING' <<<"$out" && ck ok "the refusal says so on stderr" || ck no "refusal not announced -- $(head -c 300 <<<"$out")"
echo "--- ARM 2: the source carries no exit-3 arm at all ---"
n=$(grep -c 'sys.exit(3)' "$H")
[ "$n" = 0 ] && ck ok "zero sys.exit(3) arms in the harness" || ck no "$n sys.exit(3) arm(s) remain -- the second refusal code is still there"
grep -qE '^\s*sys\.exit\(2\)' <<<"$(sed -n '/^def refuse/,/^def /p' "$H")" && ck ok "refuse() itself exits 2" || ck no "refuse() does not exit 2"
echo "--- ARM 3 (the distinction that must survive): a RED board still exits 1, never 2 ---"
# ⛔ Built from the REAL master so the red is a real graded failure, not a synthetic one: one entry whose ref is
# deliberately wrong. A harness that returned 2 for everything would pass every other arm of this gate.
python3 - "$MASTER" "$W" <<'PY'
import sys, pathlib
m, w = pathlib.Path(sys.argv[1]), pathlib.Path(sys.argv[2])
src, ref = (m/'ALL.reb').read_text(), (m/'ALL.ref').read_text()
(w/'ALL.reb').write_text(src)
(w/'ALL.ref').write_text(ref.replace('\n', '\nZZZ-DELIBERATE-MISMATCH\n', 1))
for extra in ('ALL.csv','ALL.in','ALL.xfail','ALL.wantrc'):
    if (m/extra).is_file(): (w/extra).write_text((m/extra).read_text())
PY
out="$(timeout 300 python3 "$H" run "$W/ALL.reb" "$W/ALL.ref" --lang rebus --modes m3,m4 --by-modes-column 2>&1)"; rc=$?
[ "$rc" = 1 ] && ck ok "a graded board with a red exits 1 (could-not-measure did not swallow measured-and-red)" \
              || ck no "a red board must exit 1; got rc=$rc -- $(tail -c 300 <<<"$out")"
echo "--- ARM 4: a clean graded run still exits 0 or 1 by its board, never a refusal ---"
out="$(timeout 300 python3 "$H" run "$MASTER/ALL.reb" "$MASTER/ALL.ref" --lang rebus --modes m3,m4 --by-modes-column 2>&1)"; rc=$?
if [ "$rc" = 0 ] || [ "$rc" = 1 ]; then ck ok "the real master grades and returns a board verdict (rc=$rc), not a refusal" \
else ck no "a runnable master must not refuse; got rc=$rc -- $(tail -c 300 <<<"$out")"; fi
grep -qiE 'suite_board' <<<"$out" && ck ok "the run printed its board" || ck no "no board printed -- $(tail -c 300 <<<"$out")"
echo "--- ARM 5: the staleness refusal (already 2) agrees -- BOTH refusal paths now return one code ---"
cp "$ROOT/scrip" "$W/scrip.old"; touch -d "2020-01-01T00:00:00" "$W/scrip.old"
out="$(SCRIP="$W/scrip.old" timeout 120 python3 "$H" run "$MASTER/ALL.reb" "$MASTER/ALL.ref" --lang rebus --modes m3,m4 --by-modes-column 2>&1)"; rc=$?
[ "$rc" = 2 ] && ck ok "the staleness refusal is rc=2, same code as every other refusal" || ck no "staleness refusal got rc=$rc"
echo "------------------------------------------------------------"
if [ "$fails" -ne 0 ]; then echo "⛔ GATE FAIL: $fails of $checks check(s) failed"; exit 1; fi
echo "✅ GATE PASS: $checks/$checks checks"; exit 0
