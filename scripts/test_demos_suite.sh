#!/usr/bin/env bash
source "$(dirname "${BASH_SOURCE[0]}")/lib_one_runner.sh" && one_runner_guard "${0##*/}" "${DEMOS_DIR:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)/corpus/demos/${1:-}}" || exit 2
# test_demos_suite.sh <lang> [--no-write] -- THE DEMO SUITE ROW: every demo is a test AND a workhorse benchmark (Lon 2026-09-27, in-chat to
# the ceo, verbatim: "ALso remember that all demos are also benchmarks. They are work-horse benchmarks." and "So demos are test suites in
# that they have a small input (a sample of the larger work horse input) and a ref file. All demos are benchmarks since they can be run at
# wall clock and perf values extracted. They are not run in loops."; ceo CEO-1312/1313; coo COO-206).
# THE TEST ROLE, graded here: every program under corpus/demos/<lang> (the DONE-WHEN's own census: *.sno *.icn *.pl *.sc *.pas *.raku
# *.reb *.scrip, three levels deep) that its CONTAINERS.tsv does not declare a container or library block is run on its SAMPLE input --
# stdin from <stem>.in (else <stem>.input), arguments from its <stem>.cmdline run_args (or an Icon <stem>.argv), its heap, stack and
# compile_args from its sidecars (RULES.md clause 8 (f): the runner types none of its own) -- in mode 3 and in mode 4, in a scratch copy
# of its directory with SNO_LIB its own directory then corpus/include (the scorecard's SELFDIR:include), and its stdout is compared byte for byte with <stem>.ref, cut from the language's oracle. A program with no ref
# grades FAIL, counted and named, never skipped. The row is <lang>-demos: both_modes_pass over the programs, written through
# util_score_row.py --column demos by the language's LANES seat (CEO-1232), with a progress row per program and mode (class benchmark,
# suite <lang>-demos). THE BENCHMARK ROLE is the <stem>.workhorse sidecar (lib_declared_arena.sh declared_workhorse_beside): the full input,
# run once at wall clock by the timing instruments, never here and never in a loop; this runner only counts the declarations.
# EXIT: 0 every graded program passes both modes; 1 a red named; 2 could not measure.
L="${1:-}"; shift || true
NOWRITE=0; [ "${1:-}" = --no-write ] && NOWRITE=1
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
S4E="${S4E_HOME:-$(cd "$HERE/../.." && pwd)}"
G=test_demos_suite
refuse() { echo "⛔ REFUSED(2) [$G $L]: $*" >&2; exit 2; }
case "$L" in icon|prolog|snobol4|snocone|scrip|pascal|raku|rebus) ;; *) refuse "usage: test_demos_suite.sh <lang> [--no-write] -- lang one of icon prolog snobol4 snocone scrip pascal raku rebus (got '${L}')";; esac
DD="${DEMOS_DIR:-$S4E/corpus/demos/$L}"
SCRIP="$HERE/../scrip"; RT_DIR="${RT_DIR:-$HERE/../out}"; TMO="${DEMO_TIMEOUT:-60}"
[ -d "$DD" ] || refuse "no demo tree at $DD"
[ -x "$SCRIP" ] || refuse "no scrip binary at $SCRIP -- run make first"
"$HERE/util_require_fresh.sh" --gate "$G" "$SCRIP" "$RT_DIR/libscrip_rt.so" || exit 2
. "$HERE/lib_declared_arena.sh" 2>/dev/null || refuse "lib_declared_arena.sh unloadable -- the one reader of a declared arena"
. "$HERE/lib_progress.sh" 2>/dev/null || refuse "lib_progress.sh unloadable"
. "$HERE/lib_icon_ipl_isolation.sh" 2>/dev/null || refuse "lib_icon_ipl_isolation.sh unloadable -- the one reader of an Icon .argv"
IS_BOARD=0; one_runner_suite_is_a_board "$DD" && IS_BOARD=1
CONT="$DD/CONTAINERS.tsv"
mapfile -t PROGS < <(cd "$DD" && find . -maxdepth 3 -type f \( -name '*.sno' -o -name '*.icn' -o -name '*.pl' -o -name '*.sc' -o -name '*.pas' -o -name '*.raku' -o -name '*.reb' -o -name '*.scrip' \) | sed 's|^\./||' | LC_ALL=C sort)
[ "${#PROGS[@]}" -gt 0 ] || refuse "no demo program under $DD -- a population of zero is not a green board"
T="$(mktemp -d)"; trap 'rm -rf "$T"' EXIT
PROG_ROWS="$T/progress.tsv"; : > "$PROG_ROWS"
# cmdline_words <prog> <1=compile_args|2=run_args> -- the unit's declared words from its <stem>.cmdline, through the harness's ONE reader
cmdline_words() { python3 - "$1" "$2" "$HERE" <<'PY'
import os, sys
sys.path.insert(0, sys.argv[3])
import corpus_suite_harness as h
decl, _ = h.cmdline_declarations(sys.argv[1])
w = decl.get(os.path.splitext(os.path.basename(sys.argv[1]))[0], (None, None))[int(sys.argv[2]) - 1]
print(" ".join(w or []))
PY
}
verdict() { if [ "$1" -eq 124 ]; then echo HANG; elif [ "$1" -ge 128 ]; then echo CRASH; elif cmp -s "$2" "$3"; then echo PASS; else echo FAIL; fi; }
TOTAL=0; CONTN=0; BOTH=0; M3P=0; M4P=0; WH=0; NAMED=""
printf '%-40s %-6s %-6s %s\n' program m3 m4 note
for r in "${PROGS[@]}"; do
    if [ -f "$CONT" ] && awk -F'\t' -v n="$r" '$1 == n { f = 1 } END { exit !f }' "$CONT"; then CONTN=$((CONTN+1)); continue; fi
    TOTAL=$((TOTAL+1)); f="$DD/$r"; stem="${f%.*}"; ref="$stem.ref"; d="$(dirname "$f")"; b="$(basename "$f")"
    if [ -s "$stem.workhorse" ]; then declared_workhorse_beside "$f" > /dev/null || refuse "$r: its .workhorse sidecar is refused (the reader said why above)"; WH=$((WH+1)); fi
    if [ ! -s "$ref" ]; then
        for m in m3 m4; do printf 'benchmark\t%s-demos\t%s\tdemos/%s/%s\t%s\tFAIL\t0\tno-ref\n' "$L" "$L" "$L" "$r" "$m" >> "$PROG_ROWS"; done
        printf '%-40s %-6s %-6s %s\n' "$r" FAIL FAIL "no .ref: a demo without its oracle's answer cannot be graded, and is counted, not skipped"; NAMED="$NAMED $r(no-ref)"; continue
    fi
    sw="$(declared_switches_beside "$f")" || refuse "$r: a .heap or .stack sidecar is refused (the reader said why above)"
    ca="$(cmdline_words "$f" 1)" || refuse "$r: its .cmdline is refused (the reader said why above)"
    ra="$(cmdline_words "$f" 2)" || refuse "$r: its .cmdline is refused"
    args=(); [ -n "$ra" ] && read -r -a args <<<"$ra"
    case "$b" in *.icn) if ipl_argv_read "$f" _av 2>/dev/null; then args=("${_av[@]}"); fi ;; esac
    in=/dev/null; for e in in input; do [ -f "$stem.$e" ] && { in="$stem.$e"; break; }; done
    W="$T/w.$TOTAL"; mkdir -p "$W" && cp -a "$d"/. "$W"/
    # shellcheck disable=SC2086
    ( cd "$W" && SNO_LIB="$W:$S4E/corpus/include" timeout "$TMO" "$SCRIP" $sw --run $ca "$b" ${args[@]+-- "${args[@]}"} < "$in" > "$W/.m3" 2> "$W/.m3e" ); r3=$?
    v3="$(verdict "$r3" "$W/.m3" "$ref")"
    # shellcheck disable=SC2086
    if ( cd "$W" && SNO_LIB="$W:$S4E/corpus/include" timeout "$TMO" "$SCRIP" --compile $ca "$b" < /dev/null > "$W/.p.s" 2> "$W/.p.cc" ) && [ -s "$W/.p.s" ] \
       && cc -m64 -no-pie "$W/.p.s" -o "$W/.p4" -L"$RT_DIR" -lscrip_rt -lm -Wl,-rpath,"$RT_DIR" >> "$W/.p.cc" 2>&1; then
        # shellcheck disable=SC2086
        ( cd "$W" && SNO_LIB="$W:$S4E/corpus/include" timeout "$TMO" "$W/.p4" $sw ${args[@]+"${args[@]}"} < "$in" > "$W/.m4" 2> "$W/.m4e" ); r4=$?
        v4="$(verdict "$r4" "$W/.m4" "$ref")"
    else v4=FAIL; r4=125; fi
    note=""; [ "$v3" = PASS ] || note="m3 rc=$r3$(head -c 90 "$W/.m3e" 2>/dev/null | tr '\n' ' ' | sed 's/^/ /')"
    [ "$v4" = PASS ] || note="$note${note:+; }m4 rc=$r4$( [ "$r4" = 125 ] && head -c 90 "$W/.p.cc" | tr '\n' ' ' | sed 's/^/ compile: /')"
    [ "$v3" = PASS ] && M3P=$((M3P+1)); [ "$v4" = PASS ] && M4P=$((M4P+1))
    if [ "$v3" = PASS ] && [ "$v4" = PASS ]; then BOTH=$((BOTH+1)); else NAMED="$NAMED $r(m3=$v3,m4=$v4)"; fi
    printf 'benchmark\t%s-demos\t%s\tdemos/%s/%s\tm3\t%s\t0\t%s\n' "$L" "$L" "$L" "$r" "$v3" "sample" >> "$PROG_ROWS"
    printf 'benchmark\t%s-demos\t%s\tdemos/%s/%s\tm4\t%s\t0\t%s\n' "$L" "$L" "$L" "$r" "$v4" "sample" >> "$PROG_ROWS"
    printf '%-40s %-6s %-6s %s\n' "$r" "$v3" "$v4" "$note"
    rm -rf "$W"
done
SCRIP_HASH="$(git -C "$HERE/.." rev-parse --short HEAD 2>/dev/null || echo '?')"; CORP_HASH="$(git -C "$S4E/corpus" rev-parse --short HEAD 2>/dev/null || echo '?')"
echo "DEMOS_BOARD lang=$L total=$TOTAL both_modes_pass=$BOTH m3_pass=$M3P m4_pass=$M4P containers=$CONTN workhorse_declared=$WH -- SCRIP $SCRIP_HASH corpus $CORP_HASH RT_OPT=-O0 (the sample input against each demo's oracle ref)"
[ -n "$NAMED" ] && echo "  NOT BOTH-MODES PASS:$NAMED"
[ "$WH" -lt "$TOTAL" ] && echo "  ⚠ WORKHORSE: $((TOTAL-WH)) of $TOTAL demo(s) declare no <stem>.workhorse -- the benchmark role is owed (CEO-1313)"
# progress rows: the canonical tree always, a fixture only into the table it names explicitly (S4E_PROGRESS_DB), as the testpgms runner does
if [ "$IS_BOARD" = 1 ] || [ -n "${S4E_PROGRESS_DB:-}" ]; then
    progress_append_rows_tsv "$PROG_ROWS" || refuse "the progress database did not take this run's rows -- no row is written without them (CEO-750)"
fi
if [ "$IS_BOARD" = 1 ]; then
    if [ "$NOWRITE" = 0 ]; then
        python3 "$HERE/util_score_row.py" write --lang "$L" --column demos --modes m3,m4 --measurer "${S4E_SEAT:-}" \
            --suite-pass "$BOTH" --suite-total "$TOTAL" \
            --text "$L-demos both_modes_pass=$BOTH/$TOTAL · m3 $M3P/$TOTAL · m4 $M4P/$TOTAL · containers $CONTN out of the population · workhorse declared $WH/$TOTAL (\`test_demos_suite.sh $L\`)" \
            || echo "⚠ SUITE ROW NOT WRITTEN -- the writer's refusal above says why"
    fi
else echo "suite row: $DD is not the canonical demo tree -- a fixture, never written"; fi
[ "$BOTH" -eq "$TOTAL" ]
