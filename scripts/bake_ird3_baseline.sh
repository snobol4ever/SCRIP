#!/usr/bin/env bash
# bake_ird3_baseline.sh — capture behavioral baselines for the IRD-3 sweep gate.
# Usage: bash scripts/bake_ird3_baseline.sh <outdir>
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO="$(cd "$HERE/.." && pwd)"
SCRIP="$REPO/scrip"
OUT="${1:?outdir required}"
mkdir -p "$OUT"
run() { local name="$1"; shift; timeout 300 "$@" > "$OUT/$name.log" 2>&1; echo "rc=$?" >> "$OUT/$name.log"; echo "  $name done"; }
run smoke_icon    bash "$HERE/test_smoke_icon.sh"
run smoke_prolog  bash "$HERE/test_smoke_prolog.sh"
run smoke_snobol4 bash "$HERE/test_smoke_snobol4.sh"
run smoke_raku    bash "$HERE/test_raku_smoke.sh"
# sno per-file interp sweep (PID-agnostic: stdout+rc only)
SNOSWEEP="$OUT/sno_interp_sweep.txt"; : > "$SNOSWEEP"
find "$REPO/test/snobol4" -name '*.sno' | sort | while read -r f; do
    out=$(timeout 8 "$SCRIP" --run "$f" < /dev/null 2>/dev/null); rc=$?
    printf '%s rc=%d md5=%s\n' "${f#$REPO/}" "$rc" "$(printf '%s' "$out" | md5sum | cut -d' ' -f1)" >> "$SNOSWEEP"
done
echo "  sno_interp_sweep done ($(wc -l < "$SNOSWEEP") files)"
# pascal x5 interp
PASSWEEP="$OUT/pas_interp_sweep.txt"; : > "$PASSWEEP"
for p in boolchain boolassign alias m4wexpr sieve; do
    f="$S4E/corpus/tests/pascal/$p.pas"
    [ -f "$f" ] || { echo "$p MISSING" >> "$PASSWEEP"; continue; }
    out=$(timeout 16 "$SCRIP" --run "$f" < /dev/null 2>/dev/null); rc=$?
    printf '%s rc=%d md5=%s\n' "$p" "$rc" "$(printf '%s' "$out" | md5sum | cut -d' ' -f1)" >> "$PASSWEEP"
done
echo "  pas_interp_sweep done"
# icon per-file interp sweep over test/icon
ICNSWEEP="$OUT/icn_interp_sweep.txt"; : > "$ICNSWEEP"
find "$REPO/test/icon" -name '*.icn' | sort | while read -r f; do
    out=$(timeout 8 "$SCRIP" --run "$f" < /dev/null 2>/dev/null); rc=$?
    printf '%s rc=%d md5=%s\n' "${f#$REPO/}" "$rc" "$(printf '%s' "$out" | md5sum | cut -d' ' -f1)" >> "$ICNSWEEP"
done
echo "  icn_interp_sweep done ($(wc -l < "$ICNSWEEP") files)"
# prolog per-file interp sweep over test/prolog
PLSWEEP="$OUT/pl_interp_sweep.txt"; : > "$PLSWEEP"
find "$REPO/test/prolog" -name '*.pl' | sort | while read -r f; do
    out=$(timeout 8 "$SCRIP" --run "$f" < /dev/null 2>/dev/null); rc=$?
    printf '%s rc=%d md5=%s\n' "${f#$REPO/}" "$rc" "$(printf '%s' "$out" | md5sum | cut -d' ' -f1)" >> "$PLSWEEP"
done
echo "  pl_interp_sweep done ($(wc -l < "$PLSWEEP") files)"
# snocone per-file interp sweep (test/snocone + corpus/crosscheck/snocone)
SCOSWEEP="$OUT/sco_interp_sweep.txt"; : > "$SCOSWEEP"
{ find "$REPO/test/snocone" -name '*.sc' 2>/dev/null; find $S4E/corpus/crosscheck/snocone -name '*.sc' 2>/dev/null; } | sort | while read -r f; do
    out=$(timeout 8 "$SCRIP" --run "$f" < /dev/null 2>/dev/null); rc=$?
    printf '%s rc=%d md5=%s\n' "${f#$REPO/}" "$rc" "$(printf '%s' "$out" | md5sum | cut -d' ' -f1)" >> "$SCOSWEEP"
done
echo "  sco_interp_sweep done ($(wc -l < "$SCOSWEEP") files)"
run smoke_rebus   bash "$HERE/test_smoke_rebus.sh"
echo "BASELINE BAKED at $OUT"
