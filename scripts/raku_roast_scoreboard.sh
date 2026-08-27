#!/usr/bin/env bash
# raku_roast_scoreboard.sh — RK-100-0b. THE coverage instrument for the RAKU-100 ladder.
#
# Runs every IN-TIER roast 6.c file under scrip and classifies the result. Coverage claims
# for GOAL-RAKU-100.md come ONLY from this script's stdout (the handoff_status.sh law applied
# to coverage): never from prose, never from a hand estimate.
#
# Tier table (GOAL-RAKU-100.md; PROPOSED, Lon ratifies before the denominator is frozen):
#   EXCLUDED : S01 perl5-interop, S15 unicode/NFG, S26 pod, S22            (~100)
#   TIER-C   : S17 concurrency, S24 testing                                 (~68)
#   IN-TIER  : everything else                                              (~986)
#
# Roast files are .t; ".t" is NOT in SCRIP's closed first-dispatch extension list, so each
# file is staged to a CONTENT-IDENTICAL .raku temp. The bytes are never modified: "unmodified"
# in the ladder's completion definition refers to the test content, not the filename.
#
# Usage:  bash scripts/raku_roast_scoreboard.sh [--mode4] [--limit N] [--section S03]
#   --mode4   also compile+run each file via mode 4 (slow; default is mode 3 only)
#   --limit N stop after N in-tier files (smoke the instrument itself)
#   --section only files whose manifest path starts with this prefix
set -u
cd "$(dirname "$0")/.." || exit 2
ROOT=$(pwd)
SCRIP="$ROOT/scrip"
MANIFEST="$ROOT/refs/rakudo-main/t/spectest.data.6.c"
ROAST="$ROOT/refs/roast"
OUT="$ROOT/../.github/RAKU-COVERAGE.md"
DO_M4=0; LIMIT=0; SECTION=""
while [ $# -gt 0 ]; do
  case "$1" in
    --mode4) DO_M4=1; shift ;;
    --limit) LIMIT="$2"; shift 2 ;;
    --section) SECTION="$2"; shift 2 ;;
    *) echo "unknown arg: $1" >&2; exit 2 ;;
  esac
done
[ -x "$SCRIP" ] || { echo "SCOREBOARD BLOCKED: no scrip binary (run: make scrip)" >&2; exit 2; }
[ -f "$MANIFEST" ] || { echo "SCOREBOARD BLOCKED: no manifest at $MANIFEST (RK-100-0a: clone rakudo into refs/)" >&2; exit 2; }
[ -d "$ROAST" ]   || { echo "SCOREBOARD BLOCKED: no roast at $ROAST (RK-100-0a: clone Raku/roast into refs/)" >&2; exit 2; }
is_excluded() {
  case "$1" in
    S01-*|S15-*|S26-*|S22-*) return 0 ;;
    *) return 1 ;;
  esac
}
is_tier_c() {
  case "$1" in
    S17-*|S24-*) return 0 ;;
    *) return 1 ;;
  esac
}
TMP=$(mktemp -d)
trap 'rm -rf "$TMP"' EXIT
declare -A SEC_TOT SEC_PASS
n_intier=0; n_pass=0; n_fail=0; n_parse=0; n_crash=0; n_notap=0; n_missing=0
n_pass4=0
classify() {
  # $1=stdout file  $2=stderr file  $3=rc  -> echoes one of PASS/FAIL/PARSE-FAIL/CRASH/NO-TAP
  local so="$1" se="$2" rc="$3"
  if grep -q "parse error" "$se" 2>/dev/null; then echo "PARSE-FAIL"; return; fi
  if [ "$rc" -ge 124 ]; then echo "CRASH"; return; fi
  local plan ok notok
  plan=$(grep -cE '^1\.\.[0-9]+' "$so" 2>/dev/null)
  ok=$(grep -cE '^ok [0-9]+' "$so" 2>/dev/null)
  notok=$(grep -cE '^not ok [0-9]+' "$so" 2>/dev/null)
  if [ "$plan" -eq 0 ] && [ "$ok" -eq 0 ] && [ "$notok" -eq 0 ]; then echo "NO-TAP"; return; fi
  if [ "$notok" -gt 0 ]; then echo "FAIL"; return; fi
  if [ "$rc" -ne 0 ]; then echo "FAIL"; return; fi
  echo "PASS"
}
while read -r rel _rest; do
  case "$rel" in ''|'#'*) continue ;; esac
  is_excluded "$rel" && continue
  is_tier_c   "$rel" && continue
  [ -n "$SECTION" ] && case "$rel" in "$SECTION"*) : ;; *) continue ;; esac
  src="$ROAST/$rel"
  n_intier=$((n_intier+1))
  sec=${rel%%/*}
  SEC_TOT[$sec]=$(( ${SEC_TOT[$sec]:-0} + 1 ))
  SEC_PASS[$sec]=${SEC_PASS[$sec]:-0}
  if [ ! -f "$src" ]; then n_missing=$((n_missing+1)); continue; fi
  stage="$TMP/case.raku"
  cp "$src" "$stage"
  so="$TMP/o"; se="$TMP/e"
  timeout 5 "$SCRIP" --run "$stage" > "$so" 2> "$se" < /dev/null
  rc=$?
  verdict=$(classify "$so" "$se" "$rc")
  case "$verdict" in
    PASS)       n_pass=$((n_pass+1)); SEC_PASS[$sec]=$(( ${SEC_PASS[$sec]} + 1 )) ;;
    FAIL)       n_fail=$((n_fail+1)) ;;
    PARSE-FAIL) n_parse=$((n_parse+1)) ;;
    CRASH)      n_crash=$((n_crash+1)) ;;
    NO-TAP)     n_notap=$((n_notap+1)) ;;
  esac
  if [ "$DO_M4" = 1 ] && [ "$verdict" = PASS ]; then
    s4="$TMP/c.s"; o4="$TMP/c.o"; b4="$TMP/c.bin"
    if timeout 20 "$SCRIP" --compile --target=x86 "$stage" > "$s4" 2>/dev/null \
       && as -o "$o4" "$s4" 2>/dev/null \
       && gcc -no-pie -o "$b4" "$o4" -L"$ROOT/out" -lscrip_rt 2>/dev/null; then
      LD_LIBRARY_PATH="$ROOT/out" timeout 10 "$b4" > "$TMP/o4" 2> "$TMP/e4"
      [ "$(classify "$TMP/o4" "$TMP/e4" $?)" = PASS ] && n_pass4=$((n_pass4+1))
    fi
  fi
  [ "$LIMIT" -gt 0 ] && [ "$n_intier" -ge "$LIMIT" ] && break
done < "$MANIFEST"
pct() { [ "$2" -eq 0 ] && { echo "0.0"; return; }; awk -v a="$1" -v b="$2" 'BEGIN{printf "%.1f", (a*100.0)/b}'; }
PCT=$(pct "$n_pass" "$n_intier")
{
  echo "# RAKU-COVERAGE.md — computed by scripts/raku_roast_scoreboard.sh"
  echo
  echo "**Generated:** $(date -u +%Y-%m-%dT%H:%M:%SZ) · scrip \`$(cd "$ROOT" && git rev-parse --short HEAD 2>/dev/null)\` · roast \`$(cd "$ROAST" && git rev-parse --short HEAD 2>/dev/null)\`"
  echo
  echo "This table is the ONLY sanctioned source of a Raku coverage claim (RAKU-100 ladder,"
  echo "COMPLETION DEFINITION). Prose estimates are superseded by whatever this prints."
  echo
  echo "## IN-TIER TOTAL (mode 3 \`--run\`)"
  echo
  echo "| Verdict | Files | % of in-tier |"
  echo "|---------|-------|--------------|"
  echo "| **PASS** | $n_pass | **$PCT%** |"
  echo "| FAIL (runs, some \`not ok\`) | $n_fail | $(pct "$n_fail" "$n_intier")% |"
  echo "| PARSE-FAIL | $n_parse | $(pct "$n_parse" "$n_intier")% |"
  echo "| NO-TAP (ran, emitted no TAP) | $n_notap | $(pct "$n_notap" "$n_intier")% |"
  echo "| CRASH / TIMEOUT | $n_crash | $(pct "$n_crash" "$n_intier")% |"
  echo "| missing from roast tree | $n_missing | — |"
  echo "| **IN-TIER DENOMINATOR** | **$n_intier** | 100% |"
  [ "$DO_M4" = 1 ] && { echo; echo "**mode 4 (\`--compile\`) PASS:** $n_pass4  (both-modes PASS is the ladder's real bar)"; }
  echo
  echo "## PER-SECTION (mode 3)"
  echo
  echo "| Section | PASS | Total | % |"
  echo "|---------|------|-------|---|"
  for sec in $(printf '%s\n' "${!SEC_TOT[@]}" | sort); do
    t=${SEC_TOT[$sec]}; p=${SEC_PASS[$sec]:-0}
    echo "| $sec | $p | $t | $(pct "$p" "$t")% |"
  done
} > "$OUT"
echo "ROAST SCOREBOARD — mode 3 (--run)"
echo "  in-tier denominator : $n_intier"
echo "  PASS                : $n_pass  ($PCT%)"
echo "  FAIL                : $n_fail"
echo "  PARSE-FAIL          : $n_parse"
echo "  NO-TAP              : $n_notap"
echo "  CRASH/TIMEOUT       : $n_crash"
echo "  missing             : $n_missing"
[ "$DO_M4" = 1 ] && echo "  mode-4 PASS         : $n_pass4"
echo "  table written       : $OUT"
exit 0
