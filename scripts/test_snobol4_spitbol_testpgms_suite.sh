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
# ⛔⭐⭐ CEO-281 CLAUSE (2) -- TWO PROGRAMS ARE GRADED AGAINST CSNOBOL4, AND THE CHOICE IS RULED, NOT CONVENIENT.
# test5 and test8 are LEGAL SNOBOL4 that SPITBOL declines by I/O ENVIRONMENT rather than by semantics: sbl -bf
# stops test5 with ERROR 116 (`INPUT(.INPUT,,72)` -- an EMPTY channel, where manual v3.7:1929 requires a valid
# integer) and test8 with ERROR 160 (`OUTPUT('TITLE',6,'(14H1THIS IS HAND ,110A1)')` -- a third argument that is
# a FORTRAN FORMAT, not a file name), and in BOTH cases exits 0 carrying a diagnostic instead of an answer.
# A ref cut from that is a frozen error post-mortem carrying `memory used (bytes)` and `execution time msec` --
# implementation-specific and nondeterministic -- so NO second implementation can ever byte-match it.
# ⭐ THE PRECEDENT IS OUR OWN AND IS OLDER THAN THIS ROW: corpus/packages/snobol4/gimpel/ASM_driver.sno:16
# records the identical choice for the identical reason ("This .ref comes from CSNOBOL4 (snobol4 -b), not from
# x64/bin/sbl ... CSNOBOL4 is the other sanctioned SNOBOL4 oracle and runs it"), POKER_driver.sno:6 the same for
# 116 and 248. VERIFIED BY EXECUTION before this arm was written (hq_P 2026-09-05): csnobol4 -b runs test5 to 50
# lines and test8 to a full bridge deal, both rc=0, with ZERO `ERROR NNN --` banners and ZERO post-mortem lines.
# ⛔ IT IS NOT A BLANKET SUBSTITUTE, and the NEGATIVE CONTROL is what keeps that honest: csnobol4 exits rc=1 on
# test1, which sbl answers cleanly in 140 lines. So the set is the ruled TWO **by name** -- never "whatever sbl
# happened to refuse", which is the count-shaped reading the ceo's own wording forbids: a class is a list a
# census produced, never a count.
CSN_GRADED="${SPITBOL_TESTPGMS_CSNOBOL4_SET:-test5 test8}"
CSN=""
for _g in $CSN_GRADED; do
    if [ -f "$SUITE/$_g.spt" ]; then
        CSN="$(csnobol4_bin)" || { echo "⛔ REFUSE(rc=2): $_g is graded against CSNOBOL4 by CEO-281 clause (2) and the oracle is unreachable -- grading it against sbl instead would silently re-cut the error post-mortem this ruling exists to stop"; exit 2; }
        break
    fi
done
W="$(mktemp -d "${TMPDIR:-/tmp}/spitbol_testpgms.XXXXXX")" || { echo "⛔ REFUSE(rc=2): mktemp failed"; exit 2; }
trap 'rm -rf "$W"' EXIT
cp -a "$SUITE"/. "$W/" || { echo "⛔ REFUSE(rc=2): could not copy the suite to a scratch cwd -- refusing to grade in the vendored dir"; exit 2; }

progs=""; for f in "$W"/test*.spt; do [ -f "$f" ] || continue; progs="$progs $(basename "$f" .spt)"; done
[ -n "$progs" ] || { echo "⛔ REFUSE(rc=2): zero test*.spt programs found under $SUITE"; exit 2; }
TOTAL=0; SCORED=0; UNSCR=0; M3P=0; M3F=0; M4P=0; M4F=0; UNSCR_LINES=""; RED_LINES=""
for p in $progs; do
    TOTAL=$((TOTAL+1))
    src="$W/$p.spt"
    # ── WHICH ORACLE GRADES THIS PROGRAM (CEO-281 clause 2). Ruled BY NAME above; never inferred here from a
    # failure, because "the oracle refused it" is exactly the reasoning that would quietly grow this set.
    # ⛔ $cmpt REACHES BOTH MODES BELOW, and that is not a detail: test_snobol4_csnobol4_suite.sh:139 already
    # paid for this lesson -- "A runner that passed only the flag would grade m3 under CSNOBOL4 and m4 under
    # SPITBOL and report the split as a mode divergence -- the wrong answer in the shape hardest to attribute."
    case " $CSN_GRADED " in
        *" $p "*) okind=csnobol4; obin="$CSN"; oflags="-b";     cmpt="--compat=csnobol4" ;;
        *)        okind=sbl;      obin="$SBL"; oflags="$FLAGS"; cmpt="" ;;
    esac
    # ── the oracle, in the scratch dir, fed the shared stdin file. Status FIRST, bytes never.
    ora="$W/$p.oracle"
    (cd "$W" && timeout "$T" "$obin" $oflags "$p.spt" < "$W/testpgms.in" > "$ora" 2>/dev/null); orc=$?
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
    # ⛔⭐⭐ AND rc=0 IS NOT "THE ORACLE ANSWERED". This guard's own header says the run STATUS decides and the
    # byte count never does -- and it was one turn short, because SPITBOL REPORTS A FATAL ERROR AND EXITS 0.
    # Reported by hq_P from the first live run; re-measured here on all eight programs (SCRIP c3948a321,
    # corpus d58a796fa, oracle /home/resources/x64/bin/sbl -bf):
    #   test1 rc=0 140 lines, no diagnostic          -> a real answer
    #   test3 rc=0  47 lines, no diagnostic          -> a real answer
    #   test2 rc=231 ERROR 214, NO post-mortem       -> already unscored by the rc arm above
    #   test4 test5 test7 test8 rc=0, 18 lines       -> ERROR 116 / 248 / 160 + post-mortem
    #   test6 rc=0, 56 lines                         -> ERROR 248 + post-mortem, output TRUNCATED mid-answer
    # ⭐ WHY THIS IS WORSE THAN A MISCOUNT, and it is hq_P's point kept in their words: the whole reason this
    # runner cuts its refs LIVE is that a stored ref cannot tell a cured compiler from a ref frozen while the
    # compiler was broken. Scoring an error post-mortem reintroduces exactly that failure one level up, by
    # minting the frozen-truncation ref FRESH on every run. It also fills the denominator hole the board line
    # exists to expose: unscored read 1 where the honest number is 6.
    # ⛔ BOTH MARKERS ARE REQUIRED, and the negative control is why. The post-mortem block is ERROR-PATH ONLY
    # (test3 completes and carries none, which is why test3 legitimately passes) -- but test2 carries the
    # diagnostic with NO post-mortem, so a diagnostic-only rule and a post-mortem-only rule each miss a real
    # case that the other catches. Requiring both together fires on exactly the five rc=0 error-path programs
    # and on neither of the two that answered.
    # ⛔ THIS DOES NOT DECIDE WHAT TO DO ABOUT THE FIVE. Grading them against csnobol4 (as gimpel's
    # ASM_driver.sno already does for this class) is with the ceo as a ruling; this arm only stops calling a
    # non-answer an answer. The honest board reads WORSE (scored 3, unscored 5) and that is the point.
    # ⛔ THIS ARM IS SPITBOL-SPECIFIC AND IS NOW SCOPED TO THE sbl ORACLE. It keys on SPITBOL's own listing
    # format (the `ERROR NNN --` banner plus its post-mortem block) and on SPITBOL's habit of reporting a FATAL
    # error while exiting 0. CSNOBOL4 does neither: it signals by STATUS -- measured rc=1 on test1 -- so the
    # `orc -ne 0` arm above already covers it, and running this grep over csnobol4 output would only invite a
    # false UNSCORED the day a program legitimately PRINTS the words "ERROR 116 --" as part of its answer.
    # ⭐ test5 is exactly that hazard: its own answer quotes SPITBOL diagnostics back as data.
    _diag=""; _pm=0
    if [ "$okind" = sbl ]; then
        _diag="$(grep -m1 -E 'ERROR [0-9]+ --' "$ora" 2>/dev/null)"
        _pm="$(grep -cE '^(in line|stmts executed|memory used \(bytes\)|memory left \(bytes\))' "$ora" 2>/dev/null || echo 0)"
    fi
    if [ -n "$_diag" ] && [ "$_pm" -ge 2 ]; then
        UNSCR=$((UNSCR+1))
        _errno="$(printf '%s' "$_diag" | sed -n 's/.*\(ERROR [0-9]*\) --.*/\1/p')"
        UNSCR_LINES="$UNSCR_LINES  UNSCORED  $p  oracle REFUSED THE PROGRAM at rc=0 -- ${_errno:-ERROR} plus a post-mortem block after $(grep -c . "$ora" 2>/dev/null || echo 0) line(s): a diagnostic is not an answer, no ref cut
"
        continue
    fi
    SCORED=$((SCORED+1))
    # ── mode 3 and mode 4, same scratch cwd, same stdin.
    m3="$W/$p.m3"; (cd "$W" && timeout "$T" "$SCRIP" $cmpt "$p.spt" < "$W/testpgms.in" > "$m3" 2>/dev/null); r3=$?
    if cmp -s "$ora" "$m3"; then M3P=$((M3P+1)); else M3F=$((M3F+1)); RED_LINES="$RED_LINES  RED  $p m3 (rc=$r3, first diff: $(diff "$ora" "$m3" 2>/dev/null | head -2 | tr '\n' ' ' | cut -c1-100))
"; fi
    s4="$W/$p.s"; b4="$W/$p.bin"
    (cd "$W" && timeout "$T" "$SCRIP" $cmpt --compile "$p.spt" > "$s4" 2>/dev/null) </dev/null
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
echo "SPITBOL_TESTPGMS_BOARD total=$TOTAL scored=$SCORED unscored=$UNSCR m3_pass=$M3P m3_fail=$M3F m4_pass=$M4P m4_fail=$M4F -- SCRIP $SCRIP_HASH corpus $CORP_HASH RT_OPT=-O0 oracle=sbl-bf except {$CSN_GRADED}=csnobol4-b per CEO-281 refs cut live"
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
        gate_score_row snobol4 vendor "spitbol_testpgms $M3P/$SCORED m3 · $M4P/$SCORED m4 (of $TOTAL shipped, $UNSCR UNSCORED -- sbl answers neither, by SIGSEGV or by a fatal listing at rc=0; test5+test8 graded vs csnobol4 -b under --compat=csnobol4 per CEO-281; refs cut live, \`test_snobol4_spitbol_testpgms_suite.sh\`)" "m3,m4" || true
    fi
fi
[ "$M3F" = 0 ] && [ "$M4F" = 0 ]
