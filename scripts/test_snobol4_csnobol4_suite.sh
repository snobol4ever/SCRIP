#!/usr/bin/env bash
# test_snobol4_csnobol4_suite.sh — Phil Budne's CSNOBOL4 test suite, graded against its OWN oracle.
#
# Suite: corpus/packages/snobol4/csnobol4_suite (vendored, unmodified). A NAME.sno with a sibling NAME.ref
# is a graded pair; a NAME.sno with no .ref (e.g. bench.sno, a support/data file) is not a pair and is
# skipped by construction — never a hand-curated exclusion list.
#
# ⛔ THE ORACLE IS CSNOBOL4, NOT sbl — `sbl -bf` false-reds on 30 of these ~120 programs (CSNOBOL4-only
# extensions SPITBOL never claims: ORD, &DUMP, popen, ...; RULES.md FACT RULE s261). Primary grading is
# byte-exact against the vendored .ref; the live csnobol4 binary (csnobol4_bin(), lib_oracle_flags.sh) is
# run as a full third arm (informational, same shape as test_snoflake_suite.sh's triangulation arm) AND as
# the TIEBREAK/REGENERATION authority: when SCRIP's mode-3 output disagrees with .ref, "does live csnobol4
# ALSO disagree with .ref?" — if so the .ref pin itself is the suspect, flagged REGEN-CANDIDATE rather than
# folded into the SCRIP fail count. Invoked with `-b` (suppress startup banner), matching Budne's own
# test/run.sh convention (`ARGS="$IARGS -b"`).
#
# ⭐ NORMALIZATION, sourced directly from Budne's own test/tests.in + test.dump.sh + test.trace.sh (the
# suite author's OWN definition of a matching DUMP/TRACE test, not a guess): 4 `dump`-type programs
# (a, dump, diag1, diag2 — &DUMP=1 output) mask `MAXLNGTH = NNN` -> `MAXLNGTH = xxx` and drop `&FILL = '..'`
# lines (both environment/build-dependent, not semantic); 4 `trace`-type programs (ftrace, spit, trace1,
# trace2 — TRACE() output) mask a trailing `time = N.NN` -> `time = xxx` (wall-clock, inherently
# nondeterministic). Applied identically to SCRIP's output, live csnobol4's output, AND the .ref before
# comparison — normalizing noise, not favoring a side. No other test type is touched.
#
# Ladder (per mode, m3 --run / m4 --compile+link+run), decided in this order:
#   HANG    killed by the timeout (rc=124)
#   CRASH   killed by a fatal signal (rc>=128) — or, m4 only, compile/link itself failed ("(CC)" suffix)
#   PASS    output byte-identical to .ref
#   REJECT  output differs, rc!=0 (parse/runtime refusal — didn't reach a normal exit)
#   FAIL    output differs, rc=0 (ran to completion, wrong answer)
#
# ⚠ DIALECT: this suite targets CSNOBOL4, not SPITBOL; SCRIP follows SPITBOL semantics (RULES.md). Known
# dialect-distance classes (lowercase `end`, missing -INCLUDE targets, CSNOBOL4-only builtins) are NOT
# filtered out here — see FINDING-2026-08-27-seat06-csnobol4_suite-triage-eight-classes-three-are-not-scrip-bugs.md
# for the read on which reds are confirmed SCRIP defects vs. corpus/dialect mismatches. This runner reports
# every red by name and leaves classification to that document and follow-on cure rows.
#
# Stdin convention (Budne's own, matching test_csnobol4_budne_suite.sh's STDIN_TESTS list): these programs
# carry their test data after their own END statement; split at the first bare "END" line, feed the tail
# as stdin, and run the head as the program.
#
# ⛔ ALL arms run with cwd in a scratch dir, never in the corpus tree — an OUTPUT unit-I/O association can
# create OR OVERWRITE files named by its argument in the cwd (SPITBOL/CSNOBOL4 dialect). The whole suite is
# COPIED (never symlinked) into the scratch run dir first: a same-directory `-INCLUDE "sibling.sno"` (e.g.
# line.sno -> line2.sno) still resolves cwd-relatively, but a write-target fixture (e.g. openo.tst, a
# pre-committed empty companion file openo.sno's file-I/O test writes into) lands on the scratch copy, never
# the tracked original. ⭐ MEASURED, THE HARD WAY (seat12, same session): the first cut of this script used
# symlinks here, and running it wrote real content into two tracked corpus fixtures (openo.tst, test.bin)
# through the symlink — reverted, cured to a copy before this landed. Never symlink a suite directory a
# program under test might open for writing.
#
# ⛔ A missing suite, compiler, RT, or the csnobol4 oracle REFUSES with rc=2 — never a silent skip-as-success.
# Exit: 0 iff FAIL+REJECT+CRASH+HANG == 0 in BOTH modes over the printed denominator.
set -u
HERE="$(cd "$(dirname "$0")" && pwd)"; SD="$HERE/.."; ROOT="$(cd "$SD/.." && pwd)"
SUITE="${CSNOBOL4_SUITE:-$ROOT/corpus/packages/snobol4/csnobol4_suite}"
SCRIP="$SD/scrip"; RT_DIR="$SD/out"; TIMEOUT="${TIMEOUT:-8}"

[ -d "$SUITE" ] || { echo "⛔ REFUSE(rc=2): suite dir missing: $SUITE"; exit 2; }
[ -x "$SCRIP" ] || { echo "⛔ REFUSE(rc=2): no scrip binary at $SCRIP — build first (make)"; exit 2; }
[ -f "$RT_DIR/libscrip_rt.so" ] || { echo "⛔ REFUSE(rc=2): no $RT_DIR/libscrip_rt.so"; exit 2; }
. "$HERE/lib_oracle_flags.sh" 2>/dev/null || { echo "⛔ REFUSE(rc=2): lib_oracle_flags.sh unloadable"; exit 2; }
CSN="$(csnobol4_bin)" || exit 2

STDIN_TESTS="atn crlf longrec rewind1 sudoku trim0 trim1 uneval2 factor len repl tab words words1"
is_stdin_test() { local n="$1" s; for s in $STDIN_TESTS; do [ "$n" = "$s" ] && return 0; done; return 1; }
DUMP_TESTS="a dump diag1 diag2"; TRACE_TESTS="ftrace spit trace1 trace2"
normalize() { # $1=name $2=text -> echoes text, masked per tests.in's dump/trace convention for that name
    local n="$1" t="$2"
    case " $DUMP_TESTS " in *" $n "*) t="$(printf '%s' "$t" | sed -E -e 's/MAXLNGTH = [0-9]+/MAXLNGTH = xxx/' -e "/^&FILL = '/d")";; esac
    case " $TRACE_TESTS " in *" $n "*) t="$(printf '%s' "$t" | sed -E 's/time = [0-9.eE+-]+$/time = xxx/')";; esac
    printf '%s' "$t"
}

split_at_end() { # $1=src -> writes $2=prog (thru END) $3=stdin-tail
    python3 - "$1" "$2" "$3" << 'PY'
import re, sys
lines = open(sys.argv[1], 'r', errors='replace').read().split('\n')
idx = next((i for i, l in enumerate(lines) if re.match(r'^END\s*$', l)), None)
if idx is None:
    open(sys.argv[2], 'w').write('\n'.join(lines)); open(sys.argv[3], 'w').write('')
else:
    open(sys.argv[2], 'w').write('\n'.join(lines[:idx+1]) + '\n'); open(sys.argv[3], 'w').write('\n'.join(lines[idx+1:]))
PY
}

W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
RUN="$W/run"; mkdir -p "$RUN"
cp -rp "$SUITE"/. "$RUN"/ 2>/dev/null || true
SCRIP_HASH="$(git -C "$SD" rev-parse --short HEAD 2>/dev/null || echo '?')"
CORP_HASH="$(git -C "$ROOT/corpus" rev-parse --short HEAD 2>/dev/null || echo '?')"

compile_m4() { local sno="$1" out="$2" t; t="$(mktemp -d)"
    SNO_LIB="$SUITE" "$SCRIP" --compile "$sno" > "$t/p.s" 2>/dev/null || { rm -rf "$t"; return 1; }
    gcc -c "$t/p.s" -o "$t/p.o" 2>/dev/null || { rm -rf "$t"; return 1; }
    gcc "$t/p.o" -L"$RT_DIR" -lscrip_rt -lm -Wl,-rpath,"$RT_DIR" -o "$out" 2>/dev/null || { rm -rf "$t"; return 1; }
    rm -rf "$t"; return 0; }
status_of() { # $1=got $2=rc $3=exp -> echoes PASS|FAIL|REJECT|CRASH|HANG
    local got="$1" rc="$2" exp="$3"
    if [ "$rc" = 124 ]; then echo HANG
    elif [ "$rc" -ge 128 ]; then echo CRASH
    elif [ "$got" = "$exp" ]; then echo PASS
    elif [ "$rc" != 0 ]; then echo REJECT
    else echo FAIL; fi
}

TOTAL=0
M3_PASS=0; M3_FAIL=0; M3_REJECT=0; M3_CRASH=0; M3_HANG=0; RED3=""
M4_PASS=0; M4_FAIL=0; M4_REJECT=0; M4_CRASH=0; M4_HANG=0; RED4=""
CSN_PASS=0; CSN_FAIL=0
REGEN=0; REGEN_LIST=""

for sno in "$SUITE"/*.sno; do
    [ -e "$sno" ] || { echo "⛔ REFUSE(rc=2): zero .sno files in $SUITE"; exit 2; }
    name="$(basename "$sno" .sno)"
    ref="$SUITE/$name.ref"
    [ -f "$ref" ] || continue          # not a graded pair (support file) — never hand-curated
    TOTAL=$((TOTAL+1))
    exp="$(normalize "$name" "$(cat "$ref")")"

    prog="$RUN/$name.sno"; inp=/dev/null
    if is_stdin_test "$name"; then
        rm -f "$prog"; split_at_end "$sno" "$prog" "$W/stdin"; inp="$W/stdin"
    fi

    got3="$(cd "$RUN" && SNO_LIB="$SUITE" timeout "$TIMEOUT" "$SCRIP" --run "$prog" < "$inp" 2>&1)"; rc3=$?
    got3="$(normalize "$name" "$got3")"
    st3="$(status_of "$got3" "$rc3" "$exp")"
    case "$st3" in
        PASS) M3_PASS=$((M3_PASS+1));;
        FAIL) M3_FAIL=$((M3_FAIL+1)); RED3="$RED3 $name";;
        REJECT) M3_REJECT=$((M3_REJECT+1)); RED3="$RED3 $name";;
        CRASH) M3_CRASH=$((M3_CRASH+1)); RED3="$RED3 $name";;
        HANG) M3_HANG=$((M3_HANG+1)); RED3="$RED3 $name";;
    esac

    if compile_m4 "$prog" "$W/prog.bin"; then
        got4="$(cd "$RUN" && SNO_LIB="$SUITE" timeout "$TIMEOUT" "$W/prog.bin" < "$inp" 2>&1)"; rc4=$?
        got4="$(normalize "$name" "$got4")"
        st4="$(status_of "$got4" "$rc4" "$exp")"
        case "$st4" in
            PASS) M4_PASS=$((M4_PASS+1));;
            FAIL) M4_FAIL=$((M4_FAIL+1)); RED4="$RED4 $name";;
            REJECT) M4_REJECT=$((M4_REJECT+1)); RED4="$RED4 $name";;
            CRASH) M4_CRASH=$((M4_CRASH+1)); RED4="$RED4 $name";;
            HANG) M4_HANG=$((M4_HANG+1)); RED4="$RED4 $name";;
        esac
    else
        M4_REJECT=$((M4_REJECT+1)); RED4="$RED4 $name(CC)"
    fi

    gotc="$(cd "$RUN" && timeout "$TIMEOUT" "$CSN" -b "$prog" < "$inp" 2>&1)"
    gotc="$(normalize "$name" "$gotc")"
    if [ "$gotc" = "$exp" ]; then CSN_PASS=$((CSN_PASS+1))
    else
        CSN_FAIL=$((CSN_FAIL+1))
        [ "$st3" != PASS ] && { REGEN=$((REGEN+1)); REGEN_LIST="$REGEN_LIST $name"; }
    fi
done

echo "── csnobol4_suite: $TOTAL pairs · SCRIP $SCRIP_HASH · corpus $CORP_HASH · RT_OPT -O0 · timeout ${TIMEOUT}s · oracle csnobol4 (Phil Budne, home dialect) · .ref primary, live csnobol4 = triangulation + tiebreak/regen"
echo "CSNOBOL4_SUITE_BOARD total=$TOTAL m3_PASS=$M3_PASS m3_FAIL=$M3_FAIL m3_REJECT=$M3_REJECT m3_CRASH=$M3_CRASH m3_HANG=$M3_HANG m4_PASS=$M4_PASS m4_FAIL=$M4_FAIL m4_REJECT=$M4_REJECT m4_CRASH=$M4_CRASH m4_HANG=$M4_HANG"
echo "csnobol4 (home dialect, triangulation, informational): PASS=$CSN_PASS FAIL=$CSN_FAIL"
[ -n "$RED3" ] && echo "RED-M3:$RED3"
[ -n "$RED4" ] && echo "RED-M4:$RED4"
[ "$REGEN" -gt 0 ] && echo "REGEN-CANDIDATE ($REGEN, SCRIP m3 disagrees with .ref but so does live csnobol4 — .ref pin may be stale):$REGEN_LIST"

# ⛔ ONE LEADERBOARD (RULES.md FACT RULE, Lon 2026-09-03 ~16:05). Records what this script just
# measured into .github/SCORE.md; runs nothing itself. Non-fatal: a bookkeeping failure must never
# turn a real measurement into a red board. Matches the other package suites (Arizona/JCON/fpc/GNU/SWI);
# this one and snoflake_suite's own runner were the two missing it (board-packages-into-make-test-
# reported-then-blocking, seat13 2026-09-03).
python3 "$HERE/util_score_row.py" write --lang snobol4 --column vendor --suite CSNOBOL4 --modes m3,m4 \
    --measurer "${S4E_SEAT:-unknown-seat}" \
    --text "total=$TOTAL m3 PASS=$M3_PASS FAIL=$M3_FAIL REJECT=$M3_REJECT CRASH=$M3_CRASH HANG=$M3_HANG · m4 PASS=$M4_PASS FAIL=$M4_FAIL REJECT=$M4_REJECT CRASH=$M4_CRASH HANG=$M4_HANG (\`test_snobol4_csnobol4_suite.sh\`)" \
    || echo "⚠ SCORE.md NOT UPDATED -- record this row by hand (the REFUSED line above says why)"

[ "$M3_FAIL" = 0 ] && [ "$M3_REJECT" = 0 ] && [ "$M3_CRASH" = 0 ] && [ "$M3_HANG" = 0 ] && \
[ "$M4_FAIL" = 0 ] && [ "$M4_REJECT" = 0 ] && [ "$M4_CRASH" = 0 ] && [ "$M4_HANG" = 0 ]
