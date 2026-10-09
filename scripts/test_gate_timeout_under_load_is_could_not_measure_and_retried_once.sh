#!/usr/bin/env bash
# test_gate_timeout_under_load_is_could_not_measure_and_retried_once.sh -- A PER-PROGRAM TIMEOUT WHILE THE 1-MINUTE LOAD EXCEEDS THE
# CORES IS COULD NOT MEASURE: THE HARNESS RE-RUNS THAT ONE PROGRAM ONCE, AND ONLY A SECOND TIMEOUT GRADES HANG (ceo CEO-1335; row
# instruments-a-per-program-timeout-while-load1-exceeds-the-cores-is-could-not-measure-retried-once-serially-before-fail-and-the-
# retry-stamped-on-the-progress-row-ceo-1335; the coo 2026-10-09).
# MEASURED: at load 39 two Prolog rung entries that run in 0.1 to 0.6 s read m4 TIMEOUT under a graded run (CEO-1335's origin), and
# at load ~25 pass 42's RakBench lost point_class_add's iter angle to rc 124 the same way -- the timeout measured the scheduler.
# THE FIXTURE, under mktemp, through corpus_suite_harness.classify itself: a program that times out on its first run (it leaves a
# marker and sleeps past a 1 s timeout) and answers on its second, the load planted through lib_fanout's FANOUT_PROC seam.
#   ARM 1  load planted over the cores: the first timeout is retried, the retry PASSES, the verdict says retried=1 and both loads
#   ARM 2  FAIL-ONCE, the retry disabled (S4E_TIMEOUT_RETRY=0): the same program at the same load reads HANG
#   ARM 3  load planted under the cores: a timeout is graded HANG at once, never retried (the program ran once: one marker write)
#   ARM 4  a program that never answers, under load: retried once, HANG, and the detail says it timed out twice
#   ARM 5  the progress note of a retried verdict carries retried=1 and load1=<first>/<second> (the row's stamp)
# THE PACKAGE RUNNERS' HALF -- util_timeout_retry.sh, timeout's own argv for a runner's graded run (lib_progress.sh's $TIMEOUT_RETRY):
#   ARM 6  load over the cores, stdin a pipe, 2>&1: the first timeout is retried, the retry answers from the SAME stdin, only the
#          standing attempt's bytes come out in the program's own interleaving, rc 0, and a stamp line names the key and the mode
#   ARM 7  load under the cores: rc 124 at once, run once, no stamp
#   ARM 8  FAIL-ONCE, S4E_TIMEOUT_RETRY=0: plain timeout -- rc 124, run once, the first attempt's partial output stands
#   ARM 9  util_progress_append.py attaches a stamp to the row it names (m3 to m3 only, never another program), consumes the file,
#          and says aloud a stamp that names no row
#   ARM 10 test_pascal_fpc_suite.sh over a scratch suite (a program that never finishes, FPC_SUITE_RUN_TIMEOUT=1): under planted
#          load its m3 and m4 rows read FAIL timeout-at-1s carrying retried=1 ... timeout-twice; under a cool load the same rows
#          carry no retry -- the runner's graded timeouts go through the tool and its stamps reach the progress table
# EXIT: 0 all arms pass · 1 an arm failed · 2 could not measure.
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
[ -f "$HERE/corpus_suite_harness.py" ] && [ -f "$HERE/lib_fanout.py" ] || { echo "GATE UNPROVEN(2): the harness or lib_fanout.py is missing"; exit 2; }
W="$(mktemp -d)" || { echo "GATE UNPROVEN(2): mktemp failed"; exit 2; }; trap 'rm -rf "$W"' EXIT
mkdir -p "$W/hot" "$W/cool"
printf '999.00 999.00 999.00 9/999 1\n' > "$W/hot/loadavg"; printf '0.10 0.10 0.10 1/99 1\n' > "$W/cool/loadavg"
out="$(cd "$W" && python3 - "$HERE" "$W" <<'PY'
import os, sys
sys.path.insert(0, sys.argv[1]); w = sys.argv[2]
import corpus_suite_harness as h
def once_then_answer(d):
    os.makedirs(d, exist_ok=True)
    return ["bash", "-c", "echo run >> runs; if [ -e marker ]; then echo ok; else touch marker; sleep 5; fi"], d
def runs(d):
    return sum(1 for _ in open(os.path.join(d, "runs"))) if os.path.exists(os.path.join(d, "runs")) else 0
os.environ["FANOUT_PROC"] = w + "/hot"
a, d = once_then_answer(w + "/a1"); v = h.classify(a, 1, "ok", cwd=d)
print("ARM1", v.kind, runs(d), getattr(v, "retry", "") or "-")
os.environ["S4E_TIMEOUT_RETRY"] = "0"
a, d = once_then_answer(w + "/a2"); v = h.classify(a, 1, "ok", cwd=d)
print("ARM2", v.kind, runs(d), getattr(v, "retry", "") or "-")
del os.environ["S4E_TIMEOUT_RETRY"]
os.environ["FANOUT_PROC"] = w + "/cool"
a, d = once_then_answer(w + "/a3"); v = h.classify(a, 1, "ok", cwd=d)
print("ARM3", v.kind, runs(d), getattr(v, "retry", "") or "-")
os.environ["FANOUT_PROC"] = w + "/hot"
os.makedirs(w + "/a4", exist_ok=True)
v = h.classify(["bash", "-c", "echo run >> runs; sleep 5"], 1, "ok", cwd=w + "/a4")
print("ARM4", v.kind, runs(w + "/a4"), "twice" if "twice" in v.detail else "once", getattr(v, "retry", "") or "-")
v5 = h.Verdict("FAIL", b"x", b"", 1)
try:
    v5.retry = "retried=1 load1=31.00/12.50 nproc=16"
    print("ARM5", h._entry_note(False, v5))
except AttributeError:
    print("ARM5 a verdict carries no retry stamp")
PY
)"; rc=$?
[ "$rc" = 0 ] || { echo "GATE UNPROVEN(2): the fixture did not run (rc=$rc): $(printf '%s' "$out" | tail -2 | tr '\n' ' ')"; exit 2; }
checks=0; fails=0
ck() { checks=$((checks+1)); if [ "$1" = ok ]; then printf '  ok    %s\n' "$2"; else printf '  FAIL  %s\n' "$2"; fails=$((fails+1)); fi; }
l() { printf '%s\n' "$out" | grep "^$1 " | head -1; }
a1="$(l ARM1)"; a2="$(l ARM2)"; a3="$(l ARM3)"; a4="$(l ARM4)"; a5="$(l ARM5)"
echo "=== gate: a timeout under load is could-not-measure, retried once, and only a second timeout grades HANG ==="
case "$a1" in "ARM1 PASS 2 retried=1 load1=999.00/999.00 nproc="*) ck ok "load over the cores: the timeout was retried once and the retry PASSED ($a1)";; *) ck no "arm 1: $a1";; esac
case "$a2" in "ARM2 HANG 1 -") ck ok "FAIL-ONCE: with the retry disabled the same program at the same load reads HANG ($a2)";; *) ck no "arm 2: $a2";; esac
case "$a3" in "ARM3 HANG 1 -") ck ok "load under the cores: graded HANG at once, run once, never retried ($a3)";; *) ck no "arm 3: $a3";; esac
case "$a4" in "ARM4 HANG 2 twice retried=1"*) ck ok "a program that never answers, under load: run twice, HANG, the detail says twice";; *) ck no "arm 4: $a4";; esac
case "$a5" in *"retried=1"*"load1=31.00/12.50"*"nproc=16"*) ck ok "the progress note carries the retry stamp ($a5)";; *) ck no "arm 5: $a5";; esac
TR="$HERE/util_timeout_retry.sh"
[ -x "$TR" ] || { echo "GATE UNPROVEN(2): $TR is missing or not executable"; exit 2; }
P='read -r x; echo "in=$x"; echo err1 >&2; echo out2; echo run >> runs; if [ -e marker ]; then echo answered; else touch marker; echo partial; sleep 5; fi'
mkdir -p "$W/t6" "$W/t7" "$W/t8"
o6="$(cd "$W/t6" && printf 'hello\n' | FANOUT_PROC="$W/hot" S4E_TIMEOUT_STAMP="$W/t6.stamp" S4E_TIMEOUT_KEY=unit6 "$TR" 1 bash -c "$P" 2>&1)"; r6=$?
n6="$(wc -l < "$W/t6/runs" 2>/dev/null)"; st6="$(cat "$W/t6.stamp" 2>/dev/null)"
if [ "$r6" = 0 ] && [ "$(printf '%s' "$o6" | tr '\n' '|')" = "in=hello|err1|out2|answered" ] && [ "$n6" = 2 ] && [[ "$st6" == unit6$'\t'm4$'\t'"retried=1 load1=999.00/999.00 nproc="* ]]; then
    ck ok "the tool under load: retried once, the retry read the same stdin, only its bytes in their own order, rc 0, stamp '$(printf '%s' "$st6" | tr '\t' ' ')'"
else ck no "arm 6: rc=$r6 runs=$n6 out=[$(printf '%s' "$o6" | tr '\n' '|')] stamp=[$st6]"; fi
o7="$(cd "$W/t7" && printf 'hello\n' | FANOUT_PROC="$W/cool" S4E_TIMEOUT_STAMP="$W/t7.stamp" "$TR" 1 bash -c "$P" 2>&1)"; r7=$?
n7="$(wc -l < "$W/t7/runs" 2>/dev/null)"
if [ "$r7" = 124 ] && [ "$n7" = 1 ] && [ ! -e "$W/t7.stamp" ]; then ck ok "the tool under a cool load: rc 124 at once, run once, no stamp"
else ck no "arm 7: rc=$r7 runs=$n7 stamp=$([ -e "$W/t7.stamp" ] && echo written || echo none)"; fi
o8="$(cd "$W/t8" && printf 'hello\n' | FANOUT_PROC="$W/hot" S4E_TIMEOUT_RETRY=0 S4E_TIMEOUT_STAMP="$W/t8.stamp" "$TR" 1 bash -c "$P" 2>&1)"; r8=$?
n8="$(wc -l < "$W/t8/runs" 2>/dev/null)"
if [ "$r8" = 124 ] && [ "$n8" = 1 ] && printf '%s' "$o8" | grep -q partial; then ck ok "FAIL-ONCE: S4E_TIMEOUT_RETRY=0 is plain timeout -- rc 124, run once, the partial attempt stands"
else ck no "arm 8: rc=$r8 runs=$n8 out=[$(printf '%s' "$o8" | tr '\n' '|')]"; fi
printf 'package\tfx\tpascal\tgamma\tm3\tPASS\t0\t\npackage\tfx\tpascal\tgamma\tm4\tFAIL\t0\ttimeout-at-1s\npackage\tfx\tpascal\tdelta\tm3\tPASS\t0\t\n' > "$W/t9.rows"
printf 'gamma\tm3\tretried=1 load1=31.00/12.50 nproc=16\nnobody\tm4\tretried=1 load1=40.00/41.00 nproc=16 timeout-twice\n' > "$W/t9.stamp"
e9="$(S4E_TIMEOUT_STAMP="$W/t9.stamp" S4E_PROGRESS_DB="$W/t9.tsv" python3 "$HERE/util_progress_append.py" rows-tsv "$W/t9.rows" 2>&1)"; r9=$?
g3="$(awk -F'\t' '$8=="gamma" && $9=="m3" {print $12}' "$W/t9.tsv" 2>/dev/null)"; g4="$(awk -F'\t' '$8=="gamma" && $9=="m4" {print $12}' "$W/t9.tsv" 2>/dev/null)"
d3="$(awk -F'\t' '$8=="delta" {print $12}' "$W/t9.tsv" 2>/dev/null)"
if [ "$r9" = 0 ] && [[ "$g3" == *"retried=1 load1=31.00/12.50 nproc=16"* ]] && [[ "$g4" != *retried* ]] && [[ "$d3" != *retried* ]] && [ ! -e "$W/t9.stamp" ] && printf '%s' "$e9" | grep -q 'matched no row (key nobody'; then
    ck ok "the writer: gamma's m3 row carries the stamp, gamma m4 and delta do not, the file is consumed, the stamp naming no row is said aloud"
else ck no "arm 9: rc=$r9 gamma.m3=[$g3] gamma.m4=[$g4] delta=[$d3] stamp-left=$([ -e "$W/t9.stamp" ] && echo yes || echo no) said=[$(printf '%s' "$e9" | grep -c 'matched no row')]"; fi
ROOT="$(cd "$HERE/.." && pwd)"
if [ -x "$ROOT/scrip" ] && "$HERE/util_require_fresh.sh" --gate test_gate_timeout_under_load_is_could_not_measure_and_retried_once "$ROOT/scrip" "$ROOT/out/libscrip_rt.so" >/dev/null 2>&1; then
    mkdir -p "$W/fpc"
    printf 'program spin(input, output);\nvar i: integer;\nbegin\n  i := 0;\n  while true do i := (i + 1) mod 7;\n  writeln(i)\nend.\n' > "$W/fpc/spin.pas"
    printf '0\n' > "$W/fpc/spin.ref"
    printf 'rank,entry,origin,package,n_lines,stdin,want_rc,heap_kb,stack_kb,compile_args,run_args\n1,spin,fpc_tests__spin,fpc_tests,7,0,0,131072,4096,,\n' > "$W/fpc/ALL.csv"
    for arm in hot cool; do
        (cd "$W" && FANOUT_PROC="$W/$arm" FPC_SUITE="$W/fpc" FPC_SUITE_RUN_TIMEOUT=1 S4E_PROGRESS_DB="$W/fpc.$arm.tsv" S4E_TIMEOUT_STAMP="$W/fpc.$arm.stamp" timeout 300 bash "$HERE/test_pascal_fpc_suite.sh" > "$W/fpc.$arm.out" 2>&1)
    done
    h3="$(awk -F'\t' '$8=="spin" && $9=="m3" {print $10, $12}' "$W/fpc.hot.tsv" 2>/dev/null)"; h4="$(awk -F'\t' '$8=="spin" && $9=="m4" {print $10, $12}' "$W/fpc.hot.tsv" 2>/dev/null)"
    c3="$(awk -F'\t' '$8=="spin" && $9=="m3" {print $10, $12}' "$W/fpc.cool.tsv" 2>/dev/null)"; c4="$(awk -F'\t' '$8=="spin" && $9=="m4" {print $10, $12}' "$W/fpc.cool.tsv" 2>/dev/null)"
    if [[ "$h3" == "FAIL timeout-at-1s retried=1 load1=999.00/999.00 nproc="*" timeout-twice" ]] && [[ "$h4" == "FAIL timeout-at-1s retried=1 load1=999.00/999.00 nproc="*" timeout-twice" ]] && [ "$c3" = "FAIL timeout-at-1s" ] && [ "$c4" = "FAIL timeout-at-1s" ]; then
        ck ok "test_pascal_fpc_suite.sh: under load spin's m3 and m4 rows read FAIL timeout-at-1s retried=1 ... timeout-twice; under a cool load no retry"
    elif [ -z "$h3$c3" ]; then echo "GATE UNPROVEN(2): the fpc runner wrote no progress row for the fixture: $(grep -m1 -E 'REFUS|⛔' "$W/fpc.hot.out" | cut -c1-200)"; exit 2
    else ck no "arm 10: hot m3=[$h3] m4=[$h4] cool m3=[$c3] m4=[$c4]"; fi
else
    echo "GATE UNPROVEN(2): arm 10 grades the fpc runner and needs a current $ROOT/scrip (make first)"; exit 2
fi
echo "population: $checks arm(s), $fails failed (mktemp fixtures through corpus_suite_harness.classify, util_timeout_retry.sh, util_progress_append.py and test_pascal_fpc_suite.sh; the load planted through FANOUT_PROC)"
[ "$fails" = 0 ] && { echo "GATE PASS(0) [timeout_under_load_is_could_not_measure_and_retried_once]: $checks of $checks arms hold"; exit 0; }
echo "GATE FAIL(1) [timeout_under_load_is_could_not_measure_and_retried_once]: $fails of $checks arms red"; exit 1
