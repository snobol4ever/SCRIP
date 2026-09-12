#!/usr/bin/env bash
source "$(dirname "${BASH_SOURCE[0]}")/lib_one_runner.sh" && one_runner_guard "${0##*/}" "${S4E_CORPUS:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)/corpus}/packages/prolog/swi_tests" || exit 2
# test_prolog_swi_suite.sh -- THE SWI-PROLOG TEST SUITE BOARD: every plunit CASE the vendored swipl-devel src/Tests tree
# declares, graded by AGREEMENT with real swipl's own verdict, in mode 3 and mode 4.
#
# Lon 2026-09-12, in-chat to cto, verbatim: "Get all SWI tests graded."  Row
# prolog-swi-tests-all-2879-cases-graded-per-case-against-swipl-by-path (cto).  Before this the runner graded the ten
# files that had a top-level .ref (59 plunit UNITS, one verdict per unit) and named the other 230 files in EXCLUDED.md
# with a scrip-side reason -- a red moved out of the denominator (lib_inventory.sh).
#
# THE POPULATION is every *.pl under corpus/packages/prolog/swi_tests that declares a begin_tests( unit, addressed BY
# PATH (basenames repeat across subdirectories).  Beside each sits <file>.ref, cut by scripts/util_swi_cut_refs.sh from
# swipl 9.0.4 running its OWN library(plunit), one line per CASE.  A case is graded when the oracle gave it PASS or FAIL;
# a BLOCKED case, an EMPTY unit and an UNGRADABLE unit (the oracle crashed, aborted or timed out -- ITS reason, never
# ours) are UNGRADED, named per case with the reason, never dropped and never a pass.  The declared population is read
# from the SOURCE (test/2 clause heads), so the identity graded + ungraded == declared holds per file and per board.
#
# THE VERDICT per case is agreement: oracle PASS and scrip pass, or oracle FAIL and scrip FAIL (an agreement control,
# counted separately as hit_fail_agree).  A case passes the SUITE when it agrees in EVERY mode graded (AND per case,
# CEO-372); the SUITES.tsv row states that number over the declared population.
#
# A run is a BOARD (RULES.md ONE RUNNER, ONE BOARD): only the coo runs it, on origin HEAD, once per landing batch.
#
# Usage: bash scripts/test_prolog_swi_suite.sh [--modes m3|m4|m3,m4] [--jobs N] [--file REL.pl] [--name-reds] [--verbose]
#   --file REL.pl   grade one file (a development aid, never a board; writes no row)
#   --name-reds     print every disagreeing case with both verdicts
#   --verbose       also print scrip's raw output for a disagreeing file
set -uo pipefail
GATE_NAME=test_prolog_swi_suite
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
S4E="${S4E_HOME:-$(cd "$HERE/../.." && pwd)}"
SCRIP="$HERE/../scrip"
RT="${RT_DIR:-$HERE/../out}"
SWIT="$S4E/corpus/packages/prolog/swi_tests"
PLUNIT="$S4E/corpus/tests/prolog/plunit.pl"
MATCH_PY="$HERE/util_swi_match.py"
MODES="m3,m4"; JOBS="${SWI_JOBS:-12}"; ONLY_FILE=""; NAME_REDS=0; VERBOSE=0
refuse() { echo "⛔ REFUSED(2) [$GATE_NAME]: $*" >&2; exit 2; }
while [ $# -gt 0 ]; do
    case "$1" in
        --modes) MODES="$2"; shift 2 ;;
        --jobs) JOBS="$2"; shift 2 ;;
        --file) ONLY_FILE="$2"; shift 2 ;;
        --name-reds) NAME_REDS=1; shift ;;
        --verbose) VERBOSE=1; shift ;;
        *) refuse "unknown argument '$1' -- this runner has no silent fall-through" ;;
    esac
done
[ -d "$SWIT" ] || refuse "no vendored package at $SWIT -- a package that is absent is not a package that is failing"
[ -f "$PLUNIT" ] || refuse "$PLUNIT missing"
[ -x "$SCRIP" ] || refuse "scrip not built"
"$HERE/util_require_fresh.sh" --gate "$GATE_NAME" "$SCRIP" "$RT/libscrip_rt.so" || exit 2
. "$HERE/lib_gate.sh"
. "$HERE/lib_inventory.sh"
[ -f "$HERE/lib_progress.sh" ] && . "$HERE/lib_progress.sh"
WORK="$(mktemp -d /tmp/swi_board_XXXXXX)"; trap 'rm -rf "$WORK"' EXIT
printf 'main :- run_tests.\n:- initialization(main).\n' > "$WORK/wrap.pl"
FILES="$WORK/files.txt"
grep -rl --include='*.pl' 'begin_tests(' "$SWIT" | sort > "$FILES"
if [ -n "$ONLY_FILE" ]; then grep -F -- "/$ONLY_FILE" "$FILES" > "$FILES.one" && mv "$FILES.one" "$FILES" || refuse "--file $ONLY_FILE matches no shipped test file"; fi
NFILES=$(wc -l < "$FILES")
[ "$NFILES" -gt 0 ] || refuse "no plunit files found under $SWIT"
cat > "$WORK/grade_one.sh" <<'GEOF'
#!/usr/bin/env bash
f="$1"; mode="$2"
rel="${f#"$SWIT"/}"; ref="${f%.pl}.ref"; od="$WORK/out/${rel%.pl}"; mkdir -p "$od"
act="$od/$mode.actual"; : > "$act"
if [ ! -f "$ref" ]; then echo "NOREF" > "$od/$mode.tsv"; exit 0; fi
if [ "$mode" = "m4" ]; then
    if timeout 60 "$SCRIP" --compile "$PLUNIT" "$f" "$WORK/wrap.pl" > "$od/m4.s" 2>"$od/m4.err" && [ -s "$od/m4.s" ] \
       && gcc -no-pie "$od/m4.s" -L"$RT" -lscrip_rt -lm -Wl,-rpath,"$RT" -o "$od/m4.bin" 2>>"$od/m4.err"; then
        timeout 60 "$od/m4.bin" < /dev/null > "$act" 2>"$od/m4.run.err"
    fi
    rm -f "$od/m4.s" "$od/m4.bin"
else
    timeout 60 "$SCRIP" --run "$PLUNIT" "$f" "$WORK/wrap.pl" < /dev/null > "$act" 2>"$od/m3.err"
fi
python3 "$MATCH_PY" "$f" "$ref" "$act" > "$od/$mode.tsv"
GEOF
chmod +x "$WORK/grade_one.sh"
: > "$WORK/jobs.txt"
for m in ${MODES//,/ }; do while IFS= read -r f; do printf '%s %s\n' "$f" "$m" >> "$WORK/jobs.txt"; done < "$FILES"; done
echo "grading $NFILES file(s) x modes=$MODES with $JOBS job(s), oracle refs beside the sources"
export SCRIP RT PLUNIT WORK SWIT MATCH_PY
xargs -P "$JOBS" -n 2 -a "$WORK/jobs.txt" bash "$WORK/grade_one.sh"
python3 - "$WORK" "$FILES" "$MODES" "$SWIT" "$NAME_REDS" <<'PY'
import sys, os, collections
work, files, modes, swit, name_reds = sys.argv[1], sys.argv[2], sys.argv[3].split(','), sys.argv[4], sys.argv[5] == '1'
tot = {m: collections.Counter() for m in modes}
declared = 0; ungraded_reason = collections.Counter(); ungraded_cases = 0
both = 0; graded_cases = 0; noref = []
rows = []; reds = []; perfile = []
for f in open(files).read().split('\n'):
    if not f: continue
    rel = f[len(swit) + 1:]; od = os.path.join(work, 'out', rel[:-3])
    per = {}
    for m in modes:
        p = os.path.join(od, m + '.tsv')
        txt = open(p).read() if os.path.exists(p) else ''
        if txt.startswith('NOREF'): noref.append(rel); per = None; break
        per[m] = [l.split('\t') for l in txt.split('\n') if l and '\t' in l]
    if per is None: continue
    first = per[modes[0]]
    n_decl = len(first); declared += n_decl
    fpass = 0; fgraded = 0
    for i, row in enumerate(first):
        label, verdict = row[0], row[1]
        if verdict == 'UNGRADED':
            ungraded_cases += 1; ungraded_reason[row[2].split(' ')[0] if len(row) > 2 else '?'] += 1; continue
        fgraded += 1; graded_cases += 1
        ok_all = True
        for m in modes:
            r = per[m][i] if i < len(per[m]) else [label, 'FAIL', 'no row']
            tot[m]['PASS' if r[1] == 'PASS' else 'FAIL'] += 1
            if r[1] == 'PASS' and 'agreement control' in (r[2] if len(r) > 2 else ''): tot[m]['agree_fail'] += 1
            if r[1] != 'PASS':
                ok_all = False
                if name_reds: reds.append('RED %s:%s:%s:%s' % (rel, label, m, (r[2] if len(r) > 2 else '')[:120]))
            rows.append('package\tswi\tprolog\t%s:%s\t%s\t%s\t0\tcase of %s' % (rel, label, m, 'PASS' if r[1] == 'PASS' else 'FAIL', rel))
        if ok_all: both += 1; fpass += 1
    perfile.append((rel, fpass, fgraded, n_decl - fgraded))
for rel, p, g, u in perfile:
    print('  %-48s %4d/%-4d agree (AND per case)%s' % (rel, p, g, ('  ungraded=%d' % u) if u else ''))
print('SWI_BOARD declared=%d graded=%d %s ungraded=%d' % (declared, graded_cases,
      ' '.join('%s_pass=%d %s_fail=%d %s_agree_fail=%d' % (m, tot[m]['PASS'], m, tot[m]['FAIL'], m, tot[m]['agree_fail']) for m in modes), ungraded_cases))
print('  AND per case (agrees with the oracle in EVERY mode graded, the number the suite row states): %d/%d declared (%d graded, %d ungraded)' % (both, declared, graded_cases, ungraded_cases))
print('  identity: graded %d + ungraded %d == declared %d %s' % (graded_cases, ungraded_cases, declared, '✓' if graded_cases + ungraded_cases == declared else '⛔ DOES NOT SUM'))
print('  UNGRADED by the oracle\'s reason -- named, never a pass: ' + ', '.join('%s=%d' % kv for kv in sorted(ungraded_reason.items(), key=lambda kv: -kv[1])))
if noref: print('  ⛔ %d file(s) with no .ref beside them (UNGRADED, REF_NOT_CUT): %s' % (len(noref), ' '.join(noref)))
for r in reds[:2000]: print('    ' + r)
open(os.path.join(work, 'rows.tsv'), 'w').write('\n'.join(rows) + ('\n' if rows else ''))
open(os.path.join(work, 'summary.txt'), 'w').write('%d %d %d %d\n' % (both, declared, graded_cases, len(noref)))
PY
read -r BOTH DECLARED GRADED NOREF < "$WORK/summary.txt"
if command -v progress_append_rows_tsv >/dev/null 2>&1 && [ -s "$WORK/rows.tsv" ] && [ -z "$ONLY_FILE" ]; then
    progress_append_rows_tsv "$WORK/rows.tsv" >/dev/null || echo "⚠ progress rows NOT recorded"
fi
INV_PACKAGE=swi_tests; INV_DIR="$SWIT"; INV_EXT=".pl"
_shipped_tests=$(wc -l < "$FILES")
_iv="$(inventory_line "$((_shipped_tests - NOREF))" 0)"; _ivrc=$?
[ "$_ivrc" -eq 0 ] && echo "$_iv" || echo "⚠ the package inventory refused (rc=$_ivrc) -- the case board above stands on its own measurement; the FILE census does not"
if [ -n "$ONLY_FILE" ]; then
    echo "SUITES.tsv NOT UPDATED (by design): --file grades one file and cannot produce a suite-wide number."
else
    printf 'SUITE_BOARD suite=swi lang=prolog tier=run pass=%s total=%s runner=test_prolog_swi_suite.sh\n' "$BOTH" "$DECLARED"
    python3 "$HERE/util_score_row.py" write --lang prolog --column vendor --suite SWI \
        --measurer "${S4E_SEAT:-}" --suite-pass "$BOTH" --suite-total "$DECLARED" \
        --text "swi_tests (swipl-devel src/Tests, every plunit case, agreement with swipl 9.0.4 per case) $BOTH/$DECLARED by the AND PER CASE over modes $MODES (\`test_prolog_swi_suite.sh\`)${_iv:+ · $_iv (\`test_prolog_swi_suite.sh\`)}" \
        || echo "⚠ SUITES.tsv NOT UPDATED -- record this row by hand (the REFUSED line above says why)"
fi
[ "$NOREF" -eq 0 ] || { echo "⛔ $NOREF shipped test file(s) carry no oracle ref: cut them with scripts/util_swi_cut_refs.sh --write"; exit 1; }
exit 0
