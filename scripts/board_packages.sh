#!/usr/bin/env bash
# board_packages.sh — runs EVERY vendored package/third-party suite gate in one place and prints the
# denominator, so "the package suites" stops being a phrase and becomes a number nobody assembles by
# hand (MASTER-PLAN I21; task board-packages-into-make-test-reported-then-blocking).
#
# THE DENOMINATOR is corpus/packages/<lang>/<name>, walked from the live tree every run, never a
# hardcoded count -- the same shrink-detection concern board_icon_master.sh already established for
# the Icon master suite (RULES.md § the denominator law): a package directory added or removed changes
# this board's `suites=` with no script edit, and one that stops being graded shows as UNPROVEN rather
# than silently vanishing from the count.
#
# THE RUNNER MAP below is MEASURED, not named-by-convention (seat13, 2026-09-03): built by grepping
# scripts/*.sh for literal "packages/<lang>/<dir>" path references and reading every hit, because the
# naming does not derive algorithmically -- three icon jcon-* directories share one suite script, and
# two csnobol4_suite runners exist (test_snobol4_csnobol4_suite.sh and test_csnobol4_budne_suite.sh);
# only the former is wired here, since the latter still runs a "mode-2 (--run)" arm and CLAUDE.md
# records modes 1 and 2 as DELETED. A package directory absent from the map is UNPROVEN with a reason:
# either "no gate wired yet", or for icon/jcon-compiler and icon/jcon-ref specifically, "generator input
# for util_regen_jcon_generated_sources.sh, not a graded suite" -- named rather than folded into the
# same bucket as a directory nobody has gated yet, because those are two different facts.
#
# GRADED vs UNPROVEN follows every runner's own convention (RULES.md: a test that cannot measure
# REFUSES rc=2): rc=2, a missing runner, or this board's own per-suite timeout firing is UNPROVEN;
# rc=0 or any other nonzero is GRADED (clean or red respectively) -- a suite that ran and found real
# failures was still MEASURED, which is a different fact from one nothing could run at all.
#
# REPORTED, NOT BLOCKING (the task's own D1 vs D5 staging): a graded suite being RED does not fail this
# script's OWN exit code today -- only a board-wide inability to measure (scrip missing, no packages/
# tree at all) does, matching board_icon_master.sh's "a board that cannot measure refuses" split from
# "measured a suite that is legitimately red". The RED/UNPROVEN counts below are the report; turning
# them into a hard floor is D5's ratchet, not this landing's.
#
# Every constituent suite already rewrites its OWN SCORE.md vendor cell via util_score_row.py --
# Arizona, JCON, fpc, GNU and SWI already did; Snoflake and CSNOBOL4 were the two missing it and were
# given the same call this same session, matching the other five byte-for-byte in shape (same task,
# same seat, same day). This board calls util_score_row.py for none of them on purpose: that helper's
# own rule is that it is handed numbers a caller already measured and runs no suite of its own, and a
# second writer for the same cell is exactly the twin-writer class its own file header warns against.
set -uo pipefail   # NOT -e: one suite's nonzero rc must never abort the board surveying all of them (board_icon_master.sh, same reasoning)
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
S4E="${S4E_HOME:-$(cd "$HERE/../.." && pwd)}"
SCRIP_BIN="${SCRIP:-$HERE/../scrip}"
PKGROOT="${PKGROOT:-$S4E/corpus/packages}"
PER_SUITE_TIMEOUT="${PER_SUITE_TIMEOUT:-600}"

# ⛔ A BOARD THAT CANNOT MEASURE REFUSES rc=2 -- never skip-as-success (RULES.md).
if [ ! -x "$SCRIP_BIN" ]; then echo "⛔ BOARD REFUSES (rc=2): scrip not built at $SCRIP_BIN"; exit 2; fi
if [ ! -d "$PKGROOT" ]; then echo "⛔ BOARD REFUSES (rc=2): no packages/ tree at $PKGROOT"; exit 2; fi

mapfile -t ALL_PKGS < <(find "$PKGROOT" -mindepth 2 -maxdepth 2 -type d | sed "s#^$PKGROOT/##" | sort)
SUITES=${#ALL_PKGS[@]}
if [ "$SUITES" -eq 0 ]; then echo "⛔ BOARD REFUSES (rc=2): packages/ tree enumerated ZERO leaf directories at $PKGROOT"; exit 2; fi

runner_for() {  # $1 = "<lang>/<name>" -> echoes the grading script's basename, or nothing if unmapped
  case "$1" in
    icon/arizona_tests)      echo test_icon_arizona_suite.sh ;;
    icon/jcon_tests)         echo test_icon_jcon_suite.sh ;;
    pascal/fpc_tests)        echo test_pascal_fpc_suite.sh ;;
    prolog/gnu_prolog)       echo test_prolog_gnu_suite.sh ;;
    prolog/swi_tests)        echo test_prolog_swi_suite.sh ;;
    snobol4/csnobol4_suite)  echo test_snobol4_csnobol4_suite.sh ;;
    snobol4/snoflake_suite)  echo test_snoflake_suite.sh ;;
    *) : ;;
  esac
}
unproven_reason_for() {  # $1 = "<lang>/<name>" -> why runner_for left it unmapped (only called then)
  case "$1" in
    icon/jcon-compiler|icon/jcon-ref) echo "generator input for util_regen_jcon_generated_sources.sh, not a graded suite" ;;
    *) echo "no gate wired yet" ;;
  esac
}

GRADED=0; UNPROVEN=0; RED=0
DETAIL=()
for pkg in "${ALL_PKGS[@]}"; do
  runner="$(runner_for "$pkg")"
  if [ -z "$runner" ]; then
    UNPROVEN=$((UNPROVEN+1)); DETAIL+=("UNPROVEN $pkg -- $(unproven_reason_for "$pkg")"); continue
  fi
  if [ ! -f "$HERE/$runner" ]; then
    UNPROVEN=$((UNPROVEN+1)); DETAIL+=("UNPROVEN $pkg -- mapped runner $runner is missing from scripts/"); continue
  fi
  out=$(cd "$S4E/SCRIP" && timeout "$PER_SUITE_TIMEOUT" bash "scripts/$runner" 2>&1); rc=$?
  # Cosmetic preview only -- the authoritative per-suite numbers already live in each runner's own
  # SCORE.md vendor-cell write, not here; this line just saves a reader one extra terminal round trip.
  line="$(printf '%s\n' "$out" | grep -E '_BOARD |Suite totals:|^mode-4' | tail -1)"
  if [ "$rc" = 124 ] || [ "$rc" = 2 ]; then
    UNPROVEN=$((UNPROVEN+1))
    extra=""; [ "$rc" = 124 ] && extra=" (this board's own ${PER_SUITE_TIMEOUT}s per-suite timeout fired)"
    DETAIL+=("UNPROVEN $pkg ($runner) -- rc=$rc, could not measure$extra")
  else
    GRADED=$((GRADED+1))
    if [ "$rc" = 0 ]; then
      DETAIL+=("CLEAN    $pkg ($runner) -- ${line:-rc=0, no preview line matched}")
    else
      RED=$((RED+1)); DETAIL+=("RED      $pkg ($runner) rc=$rc -- ${line:-no preview line matched}")
    fi
  fi
done

echo "=== packages board — corpus/packages/<lang>/* vendored/third-party suites ==="
echo "denominator: every immediate subdirectory of corpus/packages/<lang>/ is one vendored package, walked from the live tree each run (not a hardcoded count); a package with no mapped runner counts UNPROVEN rather than being silently dropped"
for d in "${DETAIL[@]}"; do echo "  $d"; done
echo "PACKAGES_BOARD suites=$SUITES graded=$GRADED unproven=$UNPROVEN red=$RED clean=$((GRADED-RED))"
echo "full per-suite detail: each graded suite's own SCORE.md vendor-cell write (.github/SCORE.md); rerun a single suite: bash scripts/<runner-name-above>"
if [ "$RED" -gt 0 ]; then
  echo "⚠ $RED of $GRADED graded suite(s) are RED -- REPORTED only at this stage (D1), not a board failure; D5 turns this into a floor (see the task's own GOAL)"
fi
echo "✅ PACKAGES BOARD MEASURED: $GRADED/$SUITES graded ($RED red, $((GRADED-RED)) clean), $UNPROVEN unproven"
