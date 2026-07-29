#!/usr/bin/env bash
# scripts/util_rtx_claims.sh -- THE RTX LEDGER GATE.
#
# WHY THIS EXISTS
# ---------------------------------------------------------------------------
# .github/RTX-CLAIMS.md specifies this script and asserts it is run at session
# start and session close: "a hand-maintained ledger will rot ... so the ledger
# is checked by a script that derives truth from the tree, not from itself."
#
# ⛔ IT HAD NEVER BEEN WRITTEN. Measured s212-ICN: absent from scripts/, while
# three sessions' prose asserted it was the thing keeping the ledger honest.
# s209-SN4 recorded in the ledger's own message board that it "took the OUT:/FREE
# rows at face value ... that was luck, not verification." s212-ICN then declined
# to port rt_binop_overload purely on a hand-asserted allocation row. Both were
# correct BY LUCK. This closes that.
#
# The ledger is PROSE and stays prose -- humans read it. This script does not
# rewrite it; it recomputes the tree's truth and reports where the two disagree.
#
# THE FIVE ASSERTIONS (RTX-CLAIMS.md "THE GATE"):
#   1 DOUBLE-CLAIM     symbol OUT: to two ladders                    -> FATAL
#   2 PHANTOM-LEDGER   ledger symbol, no live def AND no live call   -> FATAL
#   3 STALE-PORTED     row not DONE for a symbol already assembly    -> FATAL
#   4 UNLEDGERED-HOT   symbol above threshold, absent from ledger    -> warn
#   5 STALE-CHECKOUT   an OUT: older than 2 sessions                 -> warn
#
# EXIT: 0 = clean (warnings allowed), 1 = at least one FATAL.
#
# ⚠ SCOPE, STATED SO IT IS NOT OVERSOLD: this checks the ledger against the
# tree. It CANNOT check that a ladder honoured a claim it read, and it cannot
# see work in an unpushed clone -- the s202 ancestry check and handoff_status.sh
# remain separate obligations. It makes rot cheap to detect, not impossible.

set -uo pipefail

SCRIP_ROOT="${SCRIP_ROOT:-$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)}"
GITHUB_ROOT="${GITHUB_ROOT:-}"
CORPUS_ROOT="${CORPUS_ROOT:-}"
CUR_SESSION="${CUR_SESSION:-0}"
HOT_MIN="${HOT_MIN:-100}"

for cand in "$GITHUB_ROOT" "$SCRIP_ROOT/../.github" "$HOME/.github" "$SCRIP_ROOT/../dotgithub"; do
  [ -n "$cand" ] && [ -f "$cand/RTX-CLAIMS.md" ] && GITHUB_ROOT="$(cd "$cand" && pwd)" && break
done
for cand in "$CORPUS_ROOT" "$SCRIP_ROOT/../corpus" "$HOME/corpus"; do
  [ -n "$cand" ] && [ -d "$cand/programs" ] && CORPUS_ROOT="$(cd "$cand" && pwd)" && break
done

LEDGER="${GITHUB_ROOT:-}/RTX-CLAIMS.md"
if [ ! -f "$LEDGER" ]; then
  echo "RTX-CLAIMS GATE: cannot locate RTX-CLAIMS.md (set GITHUB_ROOT=)" >&2; exit 1
fi
echo "=== RTX LEDGER GATE ==="
echo "    ledger  = $LEDGER"
echo "    scrip   = $SCRIP_ROOT"
echo "    corpus  = ${CORPUS_ROOT:-<none: surface checks limited>}"
echo

TMP="$(mktemp -d)"; trap 'rm -rf "$TMP"' EXIT
FATAL=0; WARN=0
fatal() { echo "  ⛔ FATAL  $*"; FATAL=$((FATAL+1)); }
warn()  { echo "  ⚠  WARN   $*"; WARN=$((WARN+1)); }

# ---------------------------------------------------------------------------
# TREE TRUTH 1 -- symbols that are ALREADY ASSEMBLY. Step 0(e)'s instrument.
# Match the RTX_FUNC(name) macro and a bare `name:` label at column 0.
grep -rhoE '^[[:space:]]*RTX_FUNC\([A-Za-z_][A-Za-z_0-9]*\)' \
     "$SCRIP_ROOT/src" --include=*.S 2>/dev/null \
  | sed -E 's/.*RTX_FUNC\(([A-Za-z_][A-Za-z_0-9]*)\).*/\1/' > "$TMP/asm_syms"
grep -rhoE '^[A-Za-z_][A-Za-z_0-9]*:' "$SCRIP_ROOT/src" --include=*.S 2>/dev/null \
  | tr -d ':' >> "$TMP/asm_syms"
sort -u "$TMP/asm_syms" -o "$TMP/asm_syms"

# TREE TRUTH 2 -- symbols with a C definition (exported or c_-renamed fallback).
grep -rhoE '^[[:space:]]*[A-Za-z_][A-Za-z_0-9 \*]*[[:space:]]\**(c_)?rt_[A-Za-z_0-9]*\(' \
     "$SCRIP_ROOT/src" --include=*.c 2>/dev/null \
  | grep -oE '(c_)?rt_[A-Za-z_0-9]*' | sort -u > "$TMP/c_syms"
grep -rhoE '\b(NV_GET_fn|NV_SET_fn|to_int|str_concat_d|junction_is)\b' \
     "$SCRIP_ROOT/src" --include=*.c --include=*.S 2>/dev/null | sort -u >> "$TMP/c_syms"
sort -u "$TMP/c_syms" -o "$TMP/c_syms"

# TREE TRUTH 3 -- static call surface, LIVE artifacts only, @PLT only (step 0(f)).
: > "$TMP/live_calls"
if [ -n "${CORPUS_ROOT:-}" ]; then
  for lang in icon snobol4 prolog; do
    : > "$TMP/live_$lang"
    while IFS= read -r f; do
      head -1 "$f" 2>/dev/null | grep -q 'intel_syntax' && echo "$f"
    done < <(find "$CORPUS_ROOT/programs/$lang" "$CORPUS_ROOT/benchmarks/$lang" \
                  -name '*.s' 2>/dev/null) > "$TMP/live_$lang"
    n=$(wc -l < "$TMP/live_$lang" 2>/dev/null || echo 0)
    echo "    live $lang artifacts: $n"
    if [ "$n" -gt 0 ]; then
      xargs -a "$TMP/live_$lang" grep -hoE 'call[[:space:]]+[A-Za-z_][A-Za-z_0-9]*@PLT' 2>/dev/null \
        | sed -E 's/.*call[[:space:]]+([A-Za-z_][A-Za-z_0-9]*)@PLT/\1/' \
        | sort | uniq -c | awk -v L="$lang" '{print $2" "L" "$1}' >> "$TMP/live_calls"
    fi
  done
fi
echo

# ---------------------------------------------------------------------------
# LEDGER TRUTH -- every table row naming a symbol in a `code` cell.
grep -oE '^\|[[:space:]]*`[A-Za-z_][A-Za-z_0-9]*`[[:space:]]*\|.*' "$LEDGER" > "$TMP/rows" || true
awk -F'`' '{print $2}' "$TMP/rows" \
  | grep -vE '^(FREE|DONE|OUT|BLOCKED|static|ALL|—|-)$' \
  | grep -E '^[A-Za-z_][A-Za-z_0-9]*$' | sort -u > "$TMP/ledger_syms"
echo "    ledger rows: $(wc -l < "$TMP/rows")  distinct symbols: $(wc -l < "$TMP/ledger_syms")"
echo

state_of() { grep -m1 -E "^\|[[:space:]]*\`$1\`[[:space:]]*\|" "$TMP/rows"; }

echo "--- 1. DOUBLE-CLAIM (a symbol OUT: to two ladders) ---"
while IFS= read -r sym; do
  row="$(state_of "$sym")"
  n=$(grep -oE 'OUT:[A-Z0-9-]+-RTX' <<<"$row" | sort -u | wc -l)
  [ "$n" -gt 1 ] && fatal "$sym is OUT: to $n ladders: $(grep -oE 'OUT:[A-Z0-9-]+-RTX' <<<"$row" | sort -u | tr '\n' ' ')"
done < "$TMP/ledger_syms"
[ "$FATAL" -eq 0 ] && echo "  ok"
echo

echo "--- 2. PHANTOM-LEDGER (no live definition AND no live call site) ---"
P=0
while IFS= read -r sym; do
  hasdef=0
  grep -qx "$sym" "$TMP/asm_syms" && hasdef=1
  grep -qx "$sym" "$TMP/c_syms" && hasdef=1
  grep -qx "c_$sym" "$TMP/c_syms" && hasdef=1
  hascall=0
  [ -s "$TMP/live_calls" ] && grep -q "^$sym " "$TMP/live_calls" && hascall=1
  if [ "$hasdef" -eq 0 ] && [ "$hascall" -eq 0 ]; then
    grep -qE 'static' <<<"$(state_of "$sym")" \
      && echo "  note: $sym is ledgered *static* -- no exported def expected, skipping" \
      || { fatal "$sym: no definition in src/ and no live @PLT call site -- ledger rot"; P=1; }
  fi
done < "$TMP/ledger_syms"
[ "$P" -eq 0 ] && echo "  ok"
echo

echo "--- 3. STALE-PORTED (already assembly but the row does not say DONE) ---"
S=0
while IFS= read -r sym; do
  if grep -qx "$sym" "$TMP/asm_syms"; then
    row="$(state_of "$sym")"
    grep -q 'DONE' <<<"$row" || { fatal "$sym is assembly in src/**/*.S but its row is not DONE (step 0(e))"; S=1; }
  fi
done < "$TMP/ledger_syms"
[ "$S" -eq 0 ] && echo "  ok"
echo

echo "--- 4. UNLEDGERED-HOT (>= $HOT_MIN static sites in a live surface, no row) ---"
U=0
if [ -s "$TMP/live_calls" ]; then
  awk -v m="$HOT_MIN" '$3 >= m {print $1" "$2" "$3}' "$TMP/live_calls" | sort -k3 -rn | while read -r sym lang n; do
    grep -qx "$sym" "$TMP/ledger_syms" && continue
    grep -qx "$sym" "$TMP/asm_syms" && continue
    echo "  ⚠  WARN   $sym: $n static sites in $lang, absent from the ledger"
  done
  U=$(awk -v m="$HOT_MIN" '$3 >= m {print $1}' "$TMP/live_calls" | sort -u | while read -r s; do
        grep -qx "$s" "$TMP/ledger_syms" || grep -qx "$s" "$TMP/asm_syms" || echo x; done | wc -l)
  WARN=$((WARN+U))
fi
[ "${U:-0}" -eq 0 ] && echo "  ok"
echo

echo "--- 5. STALE-CHECKOUT (an OUT: older than 2 sessions) ---"
T=0
if [ "$CUR_SESSION" != "0" ]; then
  while IFS= read -r sym; do
    row="$(state_of "$sym")"
    s=$(grep -oE 'OUT:[A-Z0-9-]+-RTX:s([0-9]+)' <<<"$row" | grep -oE 's[0-9]+$' | tr -d 's' | head -1)
    [ -z "$s" ] && continue
    age=$(( CUR_SESSION - s ))
    [ "$age" -gt 2 ] && { warn "$sym checked out at s$s, $age sessions ago -- confirm or release"; T=1; }
  done < "$TMP/ledger_syms"
else
  echo "  skipped (pass CUR_SESSION=<n> to enable)"
  T=1
fi
[ "$T" -eq 0 ] && echo "  ok"
echo

echo "============================================================"
if [ "$FATAL" -gt 0 ]; then
  echo "RTX LEDGER GATE: BLOCKED — $FATAL fatal, $WARN warning(s)"
  exit 1
fi
echo "RTX LEDGER GATE: CLEAN — 0 fatal, $WARN warning(s)"
exit 0
