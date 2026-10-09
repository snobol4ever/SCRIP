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
echo "population: $checks arm(s), $fails failed (a mktemp fixture through corpus_suite_harness.classify, the load planted through FANOUT_PROC)"
[ "$fails" = 0 ] && { echo "GATE PASS(0) [timeout_under_load_is_could_not_measure_and_retried_once]: $checks of $checks arms hold"; exit 0; }
echo "GATE FAIL(1) [timeout_under_load_is_could_not_measure_and_retried_once]: $fails of $checks arms red"; exit 1
