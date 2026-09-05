#!/usr/bin/env bash
# test_gate_runners_refuse_on_a_stale_binary.sh -- THE ACCEPTANCE GATE for row harness-and-ladder-runner-
# refuse-on-a-stale-binary-like-the-artifact-regen-does (ceo -> hq_T 2026-09-04).
#
# THE DEFECT (ceo's witness, twice in one day, 2026-09-04): a fetched-not-merged clone at 12:23 and a 10:57
# binary graded at 14:03 both produced FALSE REDS -- the second read RED and then GREEN after nothing but an
# incremental make. Only test_corpus_snobol4.sh had a staleness preflight (gate_require_fresh, hq_B 4c7253e99);
# the ladder runners, the Python suite harness and every vendor-suite runner had none, so each of them would
# grade whatever binary happened to be in the tree and stamp the verdict with a git SHA that is not evidence
# about the artifact that ran (FINDING-2026-08-30-hq_C-the-snobol4-board-grades-whatever-scrip-exists-and-
# labels-that-verdict-with-git-head.md).
#
# ⛔⭐ WHAT THIS GATE IS REALLY DEFENDING IS THE ABSENCE OF A SECOND COPY. The staleness idea existed twice in
# bash before this row (gate_require_fresh in lib_gate.sh, assert_binary_current in lib_build_currency.sh);
# ceo cured a wrong basis in one (3d12ca54 -- "IT IS NOT A SUPERSET, IT IS WRONG") and the cure never reached
# the other, so the identical defect regenerated and was cured a SECOND time nine days later. ARM 10 exists so
# that copy number three -- especially a Python one, invisible to any grep for the bash symbol -- cannot land
# quietly. ⭐ The cost of a copy is never the duplicated lines; it is that curing one copy makes everybody
# believe the class is dead.
#
# ARMS 1-4 the shim itself · 5-6 the ladder body (all seven languages inherit it) · 7 the Python harness ·
# 8 a vendor runner · 9 the CENSUS with a printed denominator, so a NEW grader added without the preflight is
# caught by this gate rather than by the next false board · 10 the one-copy invariant · 11 no skip-as-success ·
# 12 the PROBE contract (SCRIP_STALE_PROBE_SRC, the ceo's DONE-WHEN shape) · 13 the deliberate stale run is LOUD
# (SCRIP_ALLOW_STALE=1, banner on both streams, never for a MISSING artifact) · 14 and RECORDED (no SCORE.md write)
# · 15 a SECOND census, widened past ARM 9's suite runners to every test_gate_* THAT ITSELF EXECUTES ./scrip
# (row test-gate-scripts-that-grade-scrip-refuse-on-a-stale-binary-census-widened, hq_P -> ceo -> hq_T
# 2026-09-05): hq_P's SETEXIT/ERRLIMIT resume gate was written the same day gate_require_fresh landed and did
# not inherit it, graded whatever binary lay in the tree, and produced a plausible FULL-RED board -- ARM 9
# would not have caught it, because a test_gate_* script is not a test_*_suite.sh runner and was never in that
# census's glob. ARM 15 closes that gap the same way ARM 9 closed the runner one: printed denominator,
# gates=<n> uncovered=<m>, so a NEW gate that executes ./scrip without the guard is caught here rather than by
# the next false board.
# EXIT: 0 all arms · 1 an arm failed · 2 REFUSED (could not measure -- a stale/unbuilt tree, or no corpus).
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
S4E="${S4E_HOME:-$(cd "$ROOT/.." && pwd)}"
SHIM="$HERE/util_require_fresh.sh"
SCRIP_BIN="${SCRIP:-$ROOT/scrip}"
MASTER="$S4E/corpus/tests/snobol4"

[ -x "$SHIM" ] || { echo "⛔ REFUSED-TO-GRADE rc=2: shim under test missing: $SHIM"; exit 2; }
[ -x "$SCRIP_BIN" ] || { echo "⛔ REFUSED-TO-GRADE rc=2: scrip not built at $SCRIP_BIN -- this gate needs a CURRENT tree to prove the control arm"; exit 2; }
[ -f "$MASTER/ALL.csv" ] || { echo "⛔ REFUSED-TO-GRADE rc=2: snobol4 master absent at $MASTER -- arms 5/6 cannot distinguish a staleness refusal from a missing-corpus refusal"; exit 2; }
# ⭐ THE CONTROL ARM IS ALSO THIS GATE'S OWN PRECONDITION: every stale-arm below points a runner at a
# deliberately old COPY of the real binary, so the real one must itself be current or a refusal proves nothing.
if ! "$SHIM" --gate gate-precondition >/dev/null 2>&1; then
    echo "⛔ REFUSED-TO-GRADE rc=2: this tree's own scrip/libscrip_rt.so is stale -- run 'make', then re-run."
    "$SHIM" --gate gate-precondition
    exit 2
fi

W="$(mktemp -d "${TMPDIR:-/tmp}/gate_stale_runners.XXXXXX")" || { echo "⛔ REFUSED-TO-GRADE rc=2: mktemp failed"; exit 2; }
trap 'rm -rf "$W"' EXIT
OLD="$W/scrip.old"; cp "$SCRIP_BIN" "$OLD"; touch -d "2020-01-01T00:00:00" "$OLD"

fails=0; checks=0
ck() { checks=$((checks+1)); if [ "$1" = ok ]; then printf '  ok    %s\n' "$2"; else printf '  FAIL  %s\n' "$2"; fails=$((fails+1)); fi; }
# run_rc <expected-rc> <label> -- runs the rest of the args, captures rc AND output, checks rc.
STALE_RE='older than tracked source'

echo "=== gate: every suite runner refuses rc=2 on a stale binary, like the artifact regen does ==="

echo "--- ARM 1 (control): the shim PASSES on the real, current tree -- the guard is not always-on ---"
out="$("$SHIM" --gate arm1 2>&1)"; rc=$?
[ "$rc" = 0 ] && ck ok "current scrip + libscrip_rt.so -> rc=0" || ck no "must PASS on a current tree; got rc=$rc -- $out"

echo "--- ARM 2: the shim REFUSES rc=2 on a stale artifact, naming it AND the newer source ---"
out="$("$SHIM" --gate arm2 "$OLD" 2>&1)"; rc=$?
[ "$rc" = 2 ] && ck ok "stale artifact -> rc=2" || ck no "must REFUSE rc=2; got rc=$rc -- $out"
grep -q "$STALE_RE" <<<"$out" && ck ok "refusal names the staleness cause" || ck no "refusal must name the cause -- $out"
grep -q "scrip.old" <<<"$out" && ck ok "refusal names the STALE ARTIFACT" || ck no "refusal must name the artifact -- $out"
# ⛔ the refusal must name a path that EXISTS: it once printed "src/src/parsers/..." (the src-subdir prefix
# applied to a git ls-files path that already carried it), which is an unactionable verdict -- the reader
# cannot open the file they are told is newer. Cured in lib_gate.sh with this row.
_nf="$(sed -n 's/^.*older than tracked source //p' <<<"$out" | head -1)"
[ -n "$_nf" ] && [ -e "$ROOT/$_nf" ] && ck ok "the named newer source is a REAL path ($_nf)" \
                                     || ck no "refusal names a path that does not exist: '${_nf:-<none>}'"

echo "--- ARM 3: the shim REFUSES rc=2 on a MISSING artifact (never a pass-by-skip) ---"
out="$("$SHIM" --gate arm3 "$W/does-not-exist" 2>&1)"; rc=$?
[ "$rc" = 2 ] && ck ok "missing artifact -> rc=2" || ck no "must REFUSE rc=2; got rc=$rc -- $out"
grep -qi "not built" <<<"$out" && ck ok "refusal says the artifact is not built" || ck no "refusal must say not built -- $out"

echo "--- ARM 4: the shim REFUSES rc=2 on an unknown flag (lib_flag_gate convention) ---"
out="$("$SHIM" --nonesuch 2>&1)"; rc=$?
[ "$rc" = 2 ] && ck ok "unknown flag -> rc=2" || ck no "must REFUSE rc=2; got rc=$rc -- $out"

echo "--- ARM 5: the LADDER body refuses rc=2 on a stale binary (all seven languages inherit this) ---"
out="$(SCRIP="$OLD" timeout 120 bash "$HERE/test_snobol4_ladder.sh" --to 0 2>&1)"; rc=$?
[ "$rc" = 2 ] && ck ok "ladder --to 0 with a stale binary -> rc=2" || ck no "must REFUSE rc=2; got rc=$rc -- $out"
grep -q "$STALE_RE" <<<"$out" && ck ok "ladder refusal names staleness (not a missing-master refusal)" \
                              || ck no "ladder refused for the WRONG reason -- $out"

echo "--- ARM 6 (scope): --list is NOT grading, so it still works on a stale tree ---"
out="$(SCRIP="$OLD" timeout 120 bash "$HERE/test_snobol4_ladder.sh" --list 2>&1)"; rc=$?
[ "$rc" = 0 ] && ck ok "ladder --list with a stale binary -> rc=0 (listing consults no binary)" \
              || ck no "--list must not demand a rebuild; got rc=$rc -- $out"

echo "--- ARM 7: the PYTHON harness refuses rc=2 on a stale binary (same bash function, not a copy) ---"
out="$(SCRIP="$OLD" timeout 120 python3 "$HERE/corpus_suite_harness.py" run "$MASTER/ALL.sno" "$MASTER/ALL.ref" --modes m3 2>&1)"; rc=$?
[ "$rc" = 2 ] && ck ok "harness run with a stale binary -> rc=2" || ck no "must REFUSE rc=2; got rc=$rc -- $(head -c 400 <<<"$out")"
grep -q "$STALE_RE" <<<"$out" && ck ok "harness refusal names staleness" || ck no "harness refused for the wrong reason -- $(head -c 400 <<<"$out")"

echo "--- ARM 8: a VENDOR-suite runner refuses rc=2 on a stale binary ---"
out="$(SCRIP="$OLD" timeout 120 bash "$HERE/test_icon_ipl_suite.sh" 2>&1)"; rc=$?
[ "$rc" = 2 ] && ck ok "test_icon_ipl_suite.sh with a stale binary -> rc=2" || ck no "must REFUSE rc=2; got rc=$rc -- $(head -c 400 <<<"$out")"
grep -q "$STALE_RE" <<<"$out" && ck ok "vendor refusal names staleness" || ck no "vendor runner refused for the wrong reason -- $(head -c 400 <<<"$out")"

echo "--- ARM 9 (census, PRINTED DENOMINATOR): every suite runner that grades with \$SCRIP calls the preflight ---"
graders=0; wired=0; missing=""
for f in "$HERE"/test_*_suite.sh; do
    grep -q '"\$SCRIP"' "$f" || continue
    graders=$((graders+1))
    if grep -q 'util_require_fresh.sh' "$f"; then wired=$((wired+1)); else missing="$missing $(basename "$f")"; fi
done
echo "    graders=$graders wired=$wired"
[ "$graders" -ge 15 ] && ck ok "census floor: $graders graders examined (zero-examined would be indistinguishable from all-clean)" \
                      || ck no "census examined only $graders graders -- the glob moved; a floor this low cannot prove anything"
[ -z "$missing" ] && ck ok "all $graders \$SCRIP-grading suite runners carry the staleness preflight" \
                  || ck no "grader(s) with NO staleness preflight:$missing"
for extra in lib_ladder.sh corpus_suite_harness.py; do
    grep -q 'util_require_fresh' "$HERE/$extra" && ck ok "$extra calls the preflight" || ck no "$extra lost its preflight call"
done

echo "--- ARM 10 (the one-copy invariant): the staleness rule is DEFINED exactly once ---"
defs=$(grep -rl '^gate_require_fresh()' "$HERE" 2>/dev/null | wc -l)
[ "$defs" = 1 ] && ck ok "gate_require_fresh() defined in exactly 1 file (lib_gate.sh)" \
                || ck no "gate_require_fresh() defined in $defs files -- a second copy is how this class survives its own cure"
# a Python reimplementation would not match the bash symbol at all, so look for the shape instead.
if grep -qE 'ls-files.*Makefile|newest.*tracked.*mtime' "$HERE/corpus_suite_harness.py"; then
    ck no "corpus_suite_harness.py appears to reimplement the staleness comparison -- it must SHELL OUT to the one authority"
else
    ck ok "corpus_suite_harness.py carries no staleness logic of its own"
fi

echo "--- ARM 11: no suite runner reports SUCCESS from a run it could not make (skip-as-success) ---"
# ⛔⭐ BEHAVIOURAL, NOT TEXTUAL, AND THAT DISTINCTION CAUGHT A LIVE DEFECT. This arm first grepped for the
# STRING "SKIP scrip not built" -- and its only two hits were the cure COMMENTS this row had just written
# into two runners, quoting the very code it had deleted. A textual arm cannot tell a fixed defect from a
# description of one, so it reported red on cured files while a REAL uncured arm sat one line above in
# test_snobol4_pat_rung_suite.sh ("SKIP pattern dir not found"; exit 0) and went unnamed. ⭐ Stripping
# comments first is what turned the arm from a string search into a statement about what the code DOES.
_ss=""
for f in "$HERE"/test_*_suite.sh; do
    _body="$(grep -vE '^[[:space:]]*#' "$f")"
    # same-line shape: echo SKIP ... ; exit 0     multi-line shape: echo SKIP ... \n ... exit 0 before fi
    if grep -qE '(SKIP|skip)[^;]*;[[:space:]]*exit 0' <<<"$_body" \
       || grep -qzE '(echo|printf)[^\n]*(SKIP|skip)[^\n]*\n[[:space:]]*exit 0' <<<"$_body"; then
        _ss="$_ss $(basename "$f")"
    fi
done
[ -z "$_ss" ] && ck ok "no suite runner exits 0 from a SKIP arm (a run that graded nothing never reports success)" \
              || ck no "skip-as-success still live in:$_ss"

echo "--- ARM 12: the PROBE contract -- SCRIP_STALE_PROBE_SRC names a newer source; the shim refuses on a CURRENT binary ---"
# ⭐ This is the ceo's DONE-WHEN shape: a fail-once proof against a SCRATCH file, never a touched src/ file in the real tree.
touch -d "+1 minute" "$W/newer.c"
out="$(SCRIP_STALE_PROBE_SRC="$W/newer.c" "$SHIM" --gate arm12 2>&1)"; rc=$?
[ "$rc" = 2 ] && ck ok "probe newer than a current binary -> rc=2" || ck no "must REFUSE rc=2 on the probe; got rc=$rc -- $out"
grep -q 'SCRIP_STALE_PROBE_SRC probe' <<<"$out" && ck ok "refusal names the PROBE as the newer source" || ck no "refusal does not name the probe -- $out"
touch -d "2020-01-01T00:00:00" "$W/older.c"
out="$(SCRIP_STALE_PROBE_SRC="$W/older.c" "$SHIM" --gate arm12b 2>&1)"; rc=$?
[ "$rc" = 0 ] && ck ok "a probe OLDER than the tree cannot loosen the verdict -> rc=0" || ck no "an old probe must change nothing; got rc=$rc -- $out"
out="$(SCRIP_STALE_PROBE_SRC="$W/newer.c" timeout 120 python3 "$HERE/corpus_suite_harness.py" run "$MASTER/ALL.sno" "$MASTER/ALL.ref" --modes m3 2>&1)"; rc=$?
[ "$rc" = 2 ] && ck ok "the harness honours the probe through the same shim -> rc=2" || ck no "harness must refuse on the probe; got rc=$rc -- $(head -c 400 <<<"$out")"

echo "--- ARM 13: the deliberate stale run is LOUD -- SCRIP_ALLOW_STALE=1 passes a stale artifact with a banner on BOTH streams ---"
o1="$(SCRIP_ALLOW_STALE=1 "$SHIM" --gate arm13 "$OLD" 2>/dev/null)"; rc=$?
o2="$(SCRIP_ALLOW_STALE=1 "$SHIM" --gate arm13 "$OLD" 2>&1 >/dev/null)"
[ "$rc" = 0 ] && ck ok "stale artifact under the override -> rc=0 (the run proceeds)" || ck no "override must let the run proceed; got rc=$rc -- $o1"
grep -q 'STALE-BINARY OVERRIDE' <<<"$o1" && ck ok "banner on stdout" || ck no "no banner on stdout -- $o1"
grep -q 'STALE-BINARY OVERRIDE' <<<"$o2" && ck ok "banner on stderr" || ck no "no banner on stderr -- $o2"
out="$(SCRIP_ALLOW_STALE=1 "$SHIM" --gate arm13c "$W/does-not-exist" 2>&1)"; rc=$?
[ "$rc" = 2 ] && ck ok "a MISSING artifact is never overridable -> rc=2 (nothing ran, so nothing was deliberately graded)" || ck no "override must not cover a missing artifact; got rc=$rc -- $out"

echo "--- ARM 14: the deliberate stale run is RECORDED -- gate_score_row writes NOTHING to SCORE.md under the override ---"
SCORE="$S4E/.github/SCORE.md"
if [ -f "$SCORE" ]; then
    before="$(md5sum "$SCORE" | cut -d' ' -f1)"
    out="$(bash -c 'source "$1" || exit 3; GATE_NAME=arm14; SCRIP_ALLOW_STALE=1 gate_score_row snobol4 board "PASS=0 FAIL=0 (arm14 probe -- must never land)"' _ "$HERE/lib_gate.sh" 2>&1)"; rc=$?
    after="$(md5sum "$SCORE" | cut -d' ' -f1)"
    if [ "$rc" = 0 ] && grep -q 'NOT UPDATED' <<<"$out"; then ck ok "gate_score_row declines under the override and says so"; else ck no "must decline loudly; rc=$rc -- $out"; fi
    [ "$before" = "$after" ] && ck ok "SCORE.md byte-identical across the call" || ck no "SCORE.md CHANGED under the override -- a stale number reached the leaderboard"
else
    ck no "cannot prove arm 14: $SCORE absent"
fi

echo "--- ARM 15 (census #2, PRINTED DENOMINATOR): every test_gate_* that ITSELF EXECUTES ./scrip carries the guard ---"
# Population: a call-shaped reference to the compiled binary -- a $SCRIP/$SCRIP_BIN-style variable actually
# INVOKED as a command (not merely assigned, or tested with -x/-e/-f, or echoed into a message), OR an inline
# $ROOT/scrip / $HERE/scrip invocation with no intermediate variable at all. Excludes this file itself and
# util_require_fresh.sh -- neither one grades a program through a direct call, they orchestrate other graders.
gates2=0; wired2=0; missing2=""
for f in "$HERE"/test_gate_*.sh; do
    body="$(grep -vE '^[[:space:]]*#' "$f")"
    isg=0
    if grep -qE '\$\{?SCRIP(_BIN)?\}?\b' <<<"$body"; then
        while IFS= read -r ln; do
            grep -qE '\$\{?SCRIP(_BIN)?\}?\s*=' <<<"$ln" && continue
            grep -qE '\[\[?[[:space:]]+-[a-zA-Z][[:space:]]+"?\$\{?SCRIP(_BIN)?\}?"?' <<<"$ln" && continue
            grep -qE '(echo|printf)[^$]*\$\{?SCRIP(_BIN)?\}?' <<<"$ln" && continue
            grep -qE '\$\{?SCRIP(_BIN)?\}?\b' <<<"$ln" && { isg=1; break; }
        done <<<"$body"
    fi
    [ "$isg" = 0 ] && grep -qE '"?\$(ROOT|HERE)"?/scrip\b' <<<"$body" && isg=1
    [ "$isg" = 1 ] || continue
    gates2=$((gates2+1))
    if grep -qE 'gate_require_fresh|util_require_fresh\.sh' <<<"$body"; then
        wired2=$((wired2+1))
    else
        missing2="$missing2 $(basename "$f")"
    fi
done
echo "    gates=$gates2 wired=$wired2 uncovered=$((gates2-wired2))"
[ "$gates2" -ge 50 ] && ck ok "census #2 floor: $gates2 scrip-executing test_gate_* examined" \
                     || ck no "census #2 examined only $gates2 gate(s) -- population heuristic may have regressed"
[ -z "$missing2" ] && ck ok "all $gates2 scrip-executing test_gate_* carry gate_require_fresh or the shim" \
                   || ck no "gate(s) that execute ./scrip with NO freshness guard:$missing2"

echo "------------------------------------------------------------"
if [ "$fails" -ne 0 ]; then echo "⛔ GATE FAIL: $fails of $checks check(s) failed"; exit 1; fi
echo "✅ GATE PASS: $checks/$checks checks (graders censused: $graders, gates censused: $gates2)"; exit 0
