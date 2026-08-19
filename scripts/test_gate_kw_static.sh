#!/usr/bin/env bash
# test_gate_kw_static.sh — KW-STATIC gate (GOAL-SNOBOL4-100 D-3, rungs KW-1..KW-4)
#
# Runs corpus/probe/kw/ in BOTH modes against the live-oracle .ref files.
# KW-1 (census) minted the witnesses; the gate reads 0/4 at census time and
# must read 4/4 before KW-4 is done.  Record: FINDING-2026-08-19-s146.
#
# Usage:  bash scripts/test_gate_kw_static.sh [--mode 3|4|both] [--verbose]
# Exit:   0 = every witness PASS in every selected mode; 1 = at least one DIFF.
set -uo pipefail
SCRIP_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
PROBE_DIR="${KW_PROBE_DIR:-/home/claude/corpus/probe/kw}"
SCRIP_BIN="${SCRIP_BIN:-$SCRIP_DIR/scrip}"
MODE="both"; VERBOSE=0; ARMED="${SCRIP_KW_STATIC:-}"
while [[ $# -gt 0 ]]; do
    case "$1" in
        --mode) MODE="$2"; shift 2 ;;
        --verbose|-v) VERBOSE=1; shift ;;
        --armed) ARMED=1; shift ;;
        --with-b1) WITH_B1=1; shift ;;
        --legacy) ARMED=0; shift ;;
        *) echo "unknown option: $1"; exit 2 ;;
    esac
done
[[ -x "$SCRIP_BIN" ]] || { echo "GATE BLOCKED: no scrip binary at $SCRIP_BIN (run make -j8 scrip)"; exit 2; }
[[ -d "$PROBE_DIR" ]] || { echo "GATE BLOCKED: no probe dir at $PROBE_DIR (clone corpus)"; exit 2; }
[[ -n "$ARMED" ]] && export SCRIP_KW_STATIC="$ARMED"
WORK="$(mktemp -d)"; trap 'rm -rf "$WORK"' EXIT
pass=0; fail=0; failed_names=()
# a witness may ship a .dat beside it -- that file is its stdin (kw_trim_effect needs one to
# show that &TRIM actually trims).  Absent, stdin is /dev/null.
stdin_for() { local d="${1%.sno}.dat"; [[ -f "$d" ]] && echo "$d" || echo /dev/null; }
run_mode3() { timeout 30 "$SCRIP_BIN" --run "$1" < "$(stdin_for "$1")" > "$2" 2>&1; }
run_mode4() {
    local sno="$1" out="$2" s="$WORK/m4.s" exe="$WORK/m4.bin"
    timeout 30 "$SCRIP_BIN" --compile "$sno" < /dev/null > "$s" 2>/dev/null || return 1
    gcc -no-pie -o "$exe" "$s" -L"$SCRIP_DIR/out" -lscrip_rt -lm > /dev/null 2>&1 || return 1
    LD_LIBRARY_PATH="$SCRIP_DIR/out" timeout 30 "$exe" < "$(stdin_for "$sno")" > "$out" 2>&1
}
for ref in "$PROBE_DIR"/*.ref; do
    [[ -e "$ref" ]] || { echo "GATE BLOCKED: no .ref files in $PROBE_DIR"; exit 2; }
    base="$(basename "$ref" .ref)"; sno="$PROBE_DIR/$base.sno"
    # kw_unset_datatype lives in this directory but is NOT a KW-STATIC row: it is the
    # B1 handoff witness (unset variable yields a NULL tag instead of the null string),
    # HQ-owned and identical on both killswitch arms.  Counting it would understate this
    # gate's score with a defect this ladder does not fix.  Run it with --with-b1.
    if [[ "$base" == "kw_unset_datatype" && "${WITH_B1:-0}" != "1" ]]; then continue; fi
    # kw_pattern_family is likewise NOT a KW-STATIC row.  It pins the s147 CN-2
    # regression (&ARB/&BAL/&REM/&FAIL/&FENCE/&ABORT/&SUCCEED raising error 342 on
    # READ instead of yielding their primitive patterns, manual Ch.16 p.187-188) and
    # PASSES ON BOTH KILLSWITCH ARMS, because that fix lives outside the killswitch.
    # A row identical on both arms measures nothing about ARMING, so scoring it here
    # would inflate the legacy arm with a result the killswitch does not control --
    # the same reasoning that excludes kw_unset_datatype just above, in the opposite
    # direction.  It is run unconditionally as a STANDING PIN after the scored loop.
    if [[ "$base" == "kw_pattern_family" ]]; then continue; fi
    [[ -f "$sno" ]] || { echo "SKIP  $base (no .sno beside .ref)"; continue; }
    for m in 3 4; do
        [[ "$MODE" == "both" || "$MODE" == "$m" ]] || continue
        out="$WORK/$base.m$m"
        if [[ "$m" == 3 ]]; then run_mode3 "$sno" "$out"; else run_mode4 "$sno" "$out"; fi
        if diff -q "$ref" "$out" > /dev/null 2>&1; then
            pass=$((pass+1)); [[ $VERBOSE -eq 1 ]] && echo "PASS  $base m$m"
        else
            fail=$((fail+1)); failed_names+=("$base m$m")
            echo "DIFF  $base m$m"
            [[ $VERBOSE -eq 1 ]] && diff "$ref" "$out" | head -20
        fi
    done
done
echo "-----------------------------------------------------------------------"
pin_ref="$PROBE_DIR/kw_pattern_family.ref"; pin_sno="$PROBE_DIR/kw_pattern_family.sno"; pin_bad=0
if [[ -f "$pin_ref" && -f "$pin_sno" ]]; then
    for m in 3 4; do
        [[ "$MODE" == "both" || "$MODE" == "$m" ]] || continue
        pout="$WORK/kw_pattern_family.pin.m$m"
        if [[ "$m" == 3 ]]; then run_mode3 "$pin_sno" "$pout"; else run_mode4 "$pin_sno" "$pout"; fi
        if diff -q "$pin_ref" "$pout" > /dev/null 2>&1; then echo "PIN   kw_pattern_family m$m  OK (arm-independent)"
        else pin_bad=1; echo "PIN   kw_pattern_family m$m  BROKEN — the primitive-pattern keyword family regressed again (s147 class: &ARB..&SUCCEED must read as PATTERN, manual Ch.16 p.187-188)"; fi
    done
fi
echo "-----------------------------------------------------------------------"
echo "KW-STATIC GATE: $pass PASS / $((pass+fail)) total   (mode=$MODE, SCRIP_KW_STATIC=${SCRIP_KW_STATIC:-unset/legacy})"
if [[ "${SCRIP_KW_STATIC:-0}" = "1" ]]; then
    echo "  ARM=ARMED — GRADE THIS ARM.  Expected today: all rows but kw_bare_shadow + kw_protected_write."
    echo "  Those two are ROUTED, NOT REGRESSIONS: kw_bare_shadow = HQ's B1 (an unset variable yields a"
    echo "  NULL-tagged descriptor where the oracle gives the null string, manual p.24); kw_protected_write"
    echo "  needs the &ERRLIMIT -> statement-failure mechanism SCRIP does not have yet (rung KW-5)."
else
    echo "  ARM=LEGACY — A LOW SCORE HERE IS BY DESIGN, NOT A CATASTROPHE (HQ guidance g1, s147)."
    echo "  These witnesses encode the TARGET keyword table, which only the ARMED arm implements; the legacy"
    echo "  arm is kept byte-identical to pre-KW-2 behaviour on purpose.  Grade with --armed; the legacy run's"
    echo "  only job is to prove the killswitch still isolates the feature."
fi
if [[ $pin_bad -ne 0 ]]; then
    echo "STANDING PIN BROKEN — kw_pattern_family; this is a CORRECTNESS regression independent of the KW-STATIC score above."
    exit 1
fi
if [[ $fail -gt 0 ]]; then
    echo "FAILING: ${failed_names[*]}"
    echo "GATE RED — see FINDING-2026-08-19-s146-KW1-census-keyword-truth-table.md"
    exit 1
fi
echo "GATE GREEN"
exit 0
