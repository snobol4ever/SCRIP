#!/usr/bin/env bash
# test_swi_suite.sh — GOAL-PROLOG-100-SWI RUNG SWI-0 step 0.2
# Run every extracted SWI probe under scrip m3 (--run), scrip m4 (--compile), and gprolog (oracle),
# diff each against its .expected, and report PASS/FAIL/SKIP/ORACLE_MISS per source file + total.
#
# Usage:
#   scripts/test_swi_suite.sh <probe_root_dir> [--modes=run,compile,gprolog]
# The probe_root_dir contains one subdir per source file (as emitted by swi_extract_tests.py).
set -u
ROOT="${1:-corpus/tests/prolog/swi}"
# ⛔ THE DEFAULT WAS `./scrip` -- CWD-RELATIVE, so this runner graded whatever happened to be in the
# caller's working directory, or nothing at all, and it had no existence check to notice either way
# (cured 2026-09-04 with the staleness preflight below; every sibling runner already derives it from
# the script's own location, which is the only basis that does not move when the caller moves).
SCRIP="${SCRIP:-$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)/scrip}"
GPROLOG="${GPROLOG:-gprolog}"
MODES="${MODES:-run,compile,gprolog}"
TIMEOUT="${TIMEOUT:-8}"

have_gprolog=0
command -v "$GPROLOG" >/dev/null 2>&1 && have_gprolog=1

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
# ⛔⭐ STALE-BINARY PREFLIGHT (row harness-and-ladder-runner-refuse-on-a-stale-binary-like-the-artifact-regen-
# does, ceo -> hq_T 2026-09-04). Existence is not currency: a binary that IS there can still predate the tree
# whose SHA the board stamps on the verdict. NO LOGIC HERE -- util_require_fresh.sh sources gate_require_fresh
# from lib_gate.sh, the ONE authority (hq_B 4c7253e99), never a second copy.
"$HERE/util_require_fresh.sh" --gate test_swi_suite "$SCRIP" "${RT_DIR:-$HERE/../out}/libscrip_rt.so" || exit 2
clean() { grep -E '^(ok|fail)$' | head -1; }   # probes print exactly ok|fail; strip banners/warnings
run_scrip() {  # $1=mode(run/compile) $2=file  -> ok|fail
    case "$1" in
      run)     timeout "$TIMEOUT" "$SCRIP" --run "$2" < /dev/null 2>/dev/null | clean ;;
      compile) timeout "$TIMEOUT" bash "$HERE/run_prolog_via_x86_backend.sh" "$2" < /dev/null 2>/dev/null | clean ;;
    esac
}
run_gprolog() { # $1=file  -> ok|fail  (init-goal consult+halt runs BEFORE the top-level banner query)
    timeout "$TIMEOUT" "$GPROLOG" --init-goal "consult('$1'), halt" < /dev/null 2>/dev/null | clean
}

tot_pass=0 tot_fail=0 tot_skip=0 tot_omiss=0
declare -a FAILS
for d in "$ROOT"/*/; do
    [ -d "$d" ] || continue
    base=$(basename "$d")
    p=0 fl=0 sk=0 om=0
    for x in "$d"*.pl.xfail; do [ -e "$x" ] && sk=$((sk+1)); done
    for f in "$d"*.pl; do
        [ -e "$f" ] || continue
        exp=$(cat "${f%.pl}.expected" 2>/dev/null)
        okall=1; omiss=0
        IFS=',' read -ra MS <<< "$MODES"
        for m in "${MS[@]}"; do
            case "$m" in
              run)     got=$(run_scrip run "$f") ;;
              compile) got=$(run_scrip compile "$f") ;;
              gprolog) [ "$have_gprolog" = 1 ] || continue; got=$(run_gprolog "$f"); \
                       if [ "$got" != "$exp" ]; then omiss=1; continue; fi ;;
              *) continue ;;
            esac
            if [ "$m" != "gprolog" ] && [ "$got" != "$exp" ]; then
                okall=0
                FAILS+=("$base/$(basename "$f") [$m] exp='$exp' got='$got'")
            fi
        done
        [ "$omiss" = 1 ] && om=$((om+1))
        if [ "$okall" = 1 ]; then p=$((p+1)); else fl=$((fl+1)); fi
    done
    printf "%-28s PASS=%-3d FAIL=%-3d SKIP=%-3d ORACLE_MISS=%-3d\n" "$base" "$p" "$fl" "$sk" "$om"
    tot_pass=$((tot_pass+p)); tot_fail=$((tot_fail+fl)); tot_skip=$((tot_skip+sk)); tot_omiss=$((tot_omiss+om))
done
echo "-------------------------------------------------------------"
printf "TOTAL  PASS=%d FAIL=%d SKIP=%d ORACLE_MISS=%d\n" "$tot_pass" "$tot_fail" "$tot_skip" "$tot_omiss"
if [ "${VERBOSE:-0}" = 1 ] && [ "${#FAILS[@]}" -gt 0 ]; then
    echo "--- failures ---"
    for x in "${FAILS[@]}"; do echo "  $x"; done
fi
[ "$tot_fail" = 0 ]
