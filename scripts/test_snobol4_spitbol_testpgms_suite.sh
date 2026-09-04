#!/usr/bin/env bash
# test_snobol4_spitbol_testpgms_suite.sh -- SPITBOL's OWN test programs 1-4, graded in both modes against refs
# cut LIVE from the shared correctness oracle (row snobol4-spitbol-testpgms-vendored-as-a-package-suite-with-a-
# runner-and-a-score-cell, Lon 2026-09-04 17:57 CDT via ceo: "SPITBOL's own testpgms 1-4 must run").
#
# ⛔⭐ REFS ARE CUT LIVE, EVERY RUN, AND NOTHING IS STORED. A stored .ref proves only "unchanged since someone
# cut it" -- it cannot tell a cured compiler from a ref that was cut while the compiler was broken, and this
# suite's oracle is KNOWN to die mid-run on three of its four programs, which is exactly the condition under
# which a stored ref is most likely to be a frozen truncation. Cutting live also means the day hq_P's oracle
# row lands, tests 2-4 start being scored with no edit here.
#
# ⛔⛔ AN ORACLE THAT DIES IS UNSCORED AND COUNTED AS SUCH -- never a pass, never a fail, never a ref (row
# every-ref-cutting-path-refuses-when-the-oracle-dies-mid-cut). MEASURED 2026-09-04 on the shared oracle:
# test1 rc=0 with 120 lines; tests 2, 3 and 4 rc=139 (SIGSEGV) after 8, 5 and 5 lines. Those truncations LOOK
# like answers -- that is the whole danger -- so the run status decides, never the byte count.
# ⭐ WHY COUNTED RATHER THAN SKIPPED: an unscored program is a hole in the denominator, and a board that
# silently shrinks its denominator to what happened to work reports a percentage of the wrong thing. The board
# line carries total, scored AND unscored, and names each unscored program with its signal.
#
# ⛔ EVERY PROGRAM RUNS IN A SCRATCH CWD, never the vendored directory (precedent: the Icon Arizona runner
# c96cb087d and the gimpel runner, same class, same day -- a suite program that opens a work file leaves the
# vendored tree DIRTY, and util_score_row then correctly refuses the row this run exists to write).
#
# EXIT: 0 every SCORED program passes both modes · 1 a scored program is red · 2 REFUSED (unbuilt/stale tree,
# missing suite, oracle unreachable, or ZERO programs scored -- a run that graded nothing is never green).
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; SD="$(cd "$HERE/.." && pwd)"; ROOT="$(cd "$SD/.." && pwd)"
SUITE="${SPITBOL_TESTPGMS_SUITE:-$ROOT/corpus/packages/snobol4/spitbol_testpgms}"
SCRIP="${SCRIP:-$SD/scrip}"; RT_DIR="${RT_DIR:-$SD/out}"; T="${TIMEOUT:-60}"
[ -x "$SCRIP" ] || { echo "⛔ REFUSE(rc=2): no scrip binary at $SCRIP -- build first (make)"; exit 2; }
"$HERE/util_require_fresh.sh" --gate test_snobol4_spitbol_testpgms_suite "$SCRIP" "$RT_DIR/libscrip_rt.so" || exit 2
[ -d "$SUITE" ] || { echo "⛔ REFUSE(rc=2): no suite at $SUITE"; exit 2; }
[ -f "$SUITE/testpgms.in" ] || { echo "⛔ REFUSE(rc=2): $SUITE/testpgms.in missing -- every program reads it on stdin"; exit 2; }
. "$HERE/lib_oracle_flags.sh" 2>/dev/null || { echo "⛔ REFUSE(rc=2): cannot load lib_oracle_flags.sh -- the ONE oracle-flag authority; a private fallback would grade a DIFFERENT LANGUAGE (s189: -bf is the only correct arm)"; exit 2; }
SBL="$(sbl_correctness_bin)" || exit 2
FLAGS="$(sbl_lang_flags)"
W="$(mktemp -d "${TMPDIR:-/tmp}/spitbol_testpgms.XXXXXX")" || { echo "⛔ REFUSE(rc=2): mktemp failed"; exit 2; }
trap 'rm -rf "$W"' EXIT
cp -a "$SUITE"/. "$W/" || { echo "⛔ REFUSE(rc=2): could not copy the suite to a scratch cwd -- refusing to grade in the vendored dir"; exit 2; }

progs=""; for f in "$W"/test*.spt; do [ -f "$f" ] || continue; progs="$progs $(basename "$f" .spt)"; done
[ -n "$progs" ] || { echo "⛔ REFUSE(rc=2): zero test*.spt programs found under $SUITE"; exit 2; }
TOTAL=0; SCORED=0; UNSCR=0; M3P=0; M3F=0; M4P=0; M4F=0; UNSCR_LINES=""; RED_LINES=""
for p in $progs; do
    TOTAL=$((TOTAL+1))
    src="$W/$p.spt"
    # ── the oracle, in the scratch dir, fed the shared stdin file. Status FIRST, bytes never.
    ora="$W/$p.oracle"
    (cd "$W" && timeout "$T" "$SBL" $FLAGS "$p.spt" < "$W/testpgms.in" > "$ora" 2>/dev/null); orc=$?
    if [ "$orc" -ne 0 ]; then
        UNSCR=$((UNSCR+1))
        # ⛔ NAME A SIGNAL ONLY WHEN 128+n IS A PLAUSIBLE SIGNAL NUMBER. A program may legitimately EXIT with a
        # code above 128, and this oracle does: measured on test2 in consecutive runs of this very runner --
        # rc=139 (a real SIGSEGV) once, rc=231 the next, which would decode to "signal 103" and there is no
        # signal 103. That is SPITBOL's own error exit, and calling it a signal would put a fabricated fact in
        # a board line. ⭐ The nondeterminism is itself the ceo's witness holding up ("sbl -bf SIGSEGVs on about
        # HALF its ERROR 212 runs"): the same program, the same input, two different failure modes -- which is
        # exactly why the run STATUS decides here and the output bytes never do.
        why="exited rc=$orc"
        if [ "$orc" -ge 129 ] && [ "$orc" -le 192 ]; then why="KILLED BY SIGNAL $((orc-128)) (rc=$orc)"; fi
        [ "$orc" -eq 124 ] && why="TIMED OUT after ${T}s"
        UNSCR_LINES="$UNSCR_LINES  UNSCORED  $p  oracle $why after $(grep -c . "$ora" 2>/dev/null || echo 0) line(s) -- truncated output is not ground truth, no ref cut
"
        continue
    fi
    SCORED=$((SCORED+1))
    # ── mode 3 and mode 4, same scratch cwd, same stdin.
    m3="$W/$p.m3"; (cd "$W" && timeout "$T" "$SCRIP" "$p.spt" < "$W/testpgms.in" > "$m3" 2>/dev/null); r3=$?
    if cmp -s "$ora" "$m3"; then M3P=$((M3P+1)); else M3F=$((M3F+1)); RED_LINES="$RED_LINES  RED  $p m3 (rc=$r3, first diff: $(diff "$ora" "$m3" 2>/dev/null | head -2 | tr '\n' ' ' | cut -c1-100))
"; fi
    s4="$W/$p.s"; b4="$W/$p.bin"
    (cd "$W" && timeout "$T" "$SCRIP" --compile "$p.spt" > "$s4" 2>/dev/null) </dev/null
    m4="$W/$p.m4"; r4=0
    if [ -s "$s4" ] && gcc -no-pie "$s4" -L"$RT_DIR" -lscrip_rt -Wl,-rpath,"$RT_DIR" -o "$b4" 2>/dev/null; then
        (cd "$W" && timeout "$T" "$b4" < "$W/testpgms.in" > "$m4" 2>/dev/null); r4=$?
    else
        : > "$m4"; r4=125
    fi
    if cmp -s "$ora" "$m4"; then M4P=$((M4P+1)); else M4F=$((M4F+1)); RED_LINES="$RED_LINES  RED  $p m4 (rc=$r4, first diff: $(diff "$ora" "$m4" 2>/dev/null | head -2 | tr '\n' ' ' | cut -c1-100))
"; fi
done
SCRIP_HASH="$(git -C "$SD" rev-parse --short HEAD 2>/dev/null || echo '?')"
CORP_HASH="$(git -C "$ROOT/corpus" rev-parse --short HEAD 2>/dev/null || echo '?')"
echo "SPITBOL_TESTPGMS_BOARD total=$TOTAL scored=$SCORED unscored=$UNSCR m3_pass=$M3P m3_fail=$M3F m4_pass=$M4P m4_fail=$M4F -- SCRIP $SCRIP_HASH corpus $CORP_HASH RT_OPT=-O0 oracle=sbl-bf refs cut live"
printf '%s' "$UNSCR_LINES"
printf '%s' "$RED_LINES"
# ⛔ ZERO SCORED IS UNMEASURED, NEVER GREEN. If the oracle ever dies on all four, every counter above reads 0
# and the verdict `m3_fail=0 && m4_fail=0` would be a perfect green board over an empty population.
if [ "$SCORED" -eq 0 ]; then
    echo "⛔ REFUSE(rc=2): ZERO of $TOTAL programs were scored -- the oracle died on every one, so this run measured nothing."
    exit 2
fi
if [ -f "$HERE/lib_gate.sh" ]; then
    . "$HERE/lib_gate.sh" 2>/dev/null || true
    if command -v gate_score_row >/dev/null 2>&1; then
        GATE_NAME=test_snobol4_spitbol_testpgms_suite
        gate_score_row snobol4 vendor "spitbol_testpgms $M3P/$SCORED m3 · $M4P/$SCORED m4 (of $TOTAL shipped, $UNSCR UNSCORED because the shared oracle SIGSEGVs on them, refs cut live, \`test_snobol4_spitbol_testpgms_suite.sh\`)" "m3,m4" || true
    fi
fi
[ "$M3F" = 0 ] && [ "$M4F" = 0 ]
