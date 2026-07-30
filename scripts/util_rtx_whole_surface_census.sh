#!/usr/bin/env bash
# util_rtx_whole_surface_census.sh — 0(d) OVER THE *WHOLE* STILL-C RUNTIME SURFACE.
#
# Minted s227 (2026-07-30) on the s226 rule: "0(d) IS NOT A FILTER OVER A STATIC
# SHORTLIST — run it over the WHOLE still-C surface or the symbols it would
# promote are never candidates."  s226 measured rt_flat_wire_adopt at static
# rank 16/46 and dynamic rank 4/20 — a FALSE NEGATIVE, the silent direction.
# Three sessions running, static call-site rank has been anti-correlated with
# dynamic hotness.  This script removes the static shortlist from the loop.
#
# SURFACE DERIVATION (from the TREE, never a checked-in list — the phantom class):
#   every T symbol defined by an object compiled from src/runtime/**.c,
#   MINUS the already-ported RTX_FUNC names in src/runtime/rtx/*.S,
#   MINUS their c_* fallbacks,
#   MINUS the rtx scaffolding itself.
# What remains is exactly "C runtime code that could still be replaced by asm".
#
# MEASUREMENT: util_rtx_count_syms.sh per program (it counts arbitrary,
# still-pure-C symbols, which util_rtx_arm_census.sh structurally cannot).
# Totals are summed across the corpus and ranked by ENTRIES.
#
# Usage: bash scripts/util_rtx_whole_surface_census.sh [corpus_dir] [out_tsv]
set -u
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
CORPUS="${1:-/home/claude/corpus/benchmarks/snobol4}"
OUT="${2:-/tmp/whole_surface_census.tsv}"
SO="$ROOT/out/libscrip_rt.so"
[ -f "$SO" ] || { echo "FATAL: $SO missing — run make libscrip_rt first"; exit 1; }
[ -x "$ROOT/scrip" ] || { echo "FATAL: $ROOT/scrip missing"; exit 1; }
TMP="$(mktemp -d)"; trap 'rm -rf "$TMP"' EXIT
# ---- 1. runtime-defined T symbols, straight off the objects -------------------
: > "$TMP/rt_syms.txt"
for o in $(find "$ROOT/out" -name '*.o' -newermt '1970-01-01' 2>/dev/null); do
  b="$(basename "$o" .o)"
  find "$ROOT/src/runtime" -name "$b.c" -o -name "$b.cpp" 2>/dev/null | grep -q . || continue
  nm --defined-only "$o" 2>/dev/null | awk '$2=="T"{print $3}' >> "$TMP/rt_syms.txt"
done
sort -u "$TMP/rt_syms.txt" -o "$TMP/rt_syms.txt"
# ---- 2. subtract what is already assembly ------------------------------------
grep -rhoP 'RTX_FUNC\(\s*\K[A-Za-z0-9_]+' "$ROOT"/src/runtime/rtx/*.S 2>/dev/null | sort -u > "$TMP/ported.txt"
nm -D --defined-only "$SO" | awk '$2=="T"{print $3}' | sort -u > "$TMP/dyn.txt"
: > "$TMP/cand.txt"
while read -r s; do
  case "$s" in c_*|rtx_*|_*) continue ;; esac
  grep -qx "$s" "$TMP/ported.txt" && continue          # already asm
  grep -qx "$s" "$TMP/dyn.txt"    || continue          # must be dynamically visible to interpose
  echo "$s" >> "$TMP/cand.txt"
done < "$TMP/rt_syms.txt"
N=$(wc -l < "$TMP/cand.txt")
echo "STILL-C RUNTIME SURFACE: $N symbols (ported: $(wc -l < "$TMP/ported.txt"))"
[ "$N" -gt 0 ] || { echo "FATAL: derived zero candidates"; exit 1; }
# ---- 3. count every candidate on every program -------------------------------
: > "$TMP/tot.txt"
progs=$(ls "$CORPUS"/*.sno 2>/dev/null)
[ -n "$progs" ] || { echo "FATAL: no .sno in $CORPUS"; exit 1; }
for p in $progs; do
  nm_p="$(basename "$p" .sno)"
  inp=""; [ -f "${p%.sno}.input" ] && inp="${p%.sno}.input"
  COUNTSYMS_IN="${inp:-/dev/null}" \
    bash "$ROOT/scripts/util_rtx_count_syms.sh" "$p" $(cat "$TMP/cand.txt") \
    > "$TMP/r.txt" 2>"$TMP/r.err"
  if ! grep -q 'DYNAMIC CALL COUNT' "$TMP/r.txt"; then
    echo "  $nm_p: NO DATA (see stderr)"; continue
  fi
  awk 'NR>2 && NF>=2 && $2+0>0 {print $1"\t"$2}' "$TMP/r.txt" >> "$TMP/tot.txt"
  n_hot=$(awk 'NR>2 && NF>=2 && $2+0>0' "$TMP/r.txt" | wc -l)
  echo "  $nm_p: $n_hot symbols with nonzero entries"
done
# ---- 4. rank ------------------------------------------------------------------
awk -F'\t' '{e[$1]+=$2; p[$1]++} END{for(k in e) printf "%s\t%d\t%d\n", k, e[k], p[k]}' "$TMP/tot.txt" \
  | sort -t$'\t' -k2,2nr > "$OUT"
echo
echo "=== WHOLE-SURFACE 0(d) RANK — still-C runtime symbols, $CORPUS ==="
printf "%-40s %16s %8s\n" SYMBOL ENTRIES PROGS
head -40 "$OUT" | while IFS=$'\t' read -r s e n; do printf "%-40s %16s %8s\n" "$s" "$e" "$n"; done
echo "(full ranking: $OUT)"
