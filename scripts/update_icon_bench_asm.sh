#!/usr/bin/env bash
# update_icon_bench_asm.sh — produce + MAINTAIN side-by-side x86 .s artifacts for the Icon
# benchmark corpus.
#
# For each <bench>.icn matching GLOB, compile it with the CURRENT SCRIP emitter
# (--compile --target=x86). If it compiles to non-empty output that ASSEMBLES cleanly with
# `as`, write/update the sibling <bench>.s. The .s is rewritten ONLY when its CANONICALIZED
# content actually changes — address-derived bb<NNNNN> labels (which vary run-to-run under
# ASLR but keep a stable order of appearance) are renumbered to a deterministic bb<00001>
# sequence, so a git diff reflects a REAL emitter change, never run-to-run churn.
#
# Programs that REFUSE (native arm pending), fail to compile, or fail to assemble are
# REPORTED and their existing .s is left untouched (legacy artifact retained until the
# emitter covers them; a later run regenerates it automatically).
#
# Usage:
#   scripts/update_icon_bench_asm.sh [GLOB]
#     GLOB defaults to '*.icn' (the whole Icon benchmark corpus).
#   ICON_CORPUS=<dir>   override the corpus icon dir (default /home/claude/corpus/benchmarks/icon)
#   CHECK=1             dry-run: report what WOULD change, write nothing (exit 1 if any drift)
#
# Exit: 0 normally; with CHECK=1, 0 if every .s is already current, 1 if any would change.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
set -u
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SCRIP="$ROOT/scrip"
CORPUS="${ICON_CORPUS:-$S4E/corpus/benchmarks/icon}"
GLOB="${1:-*.icn}"
CHECK="${CHECK:-0}"
[ -x "$SCRIP" ] || { echo "FATAL: $SCRIP not built (run scripts/build_scrip.sh)"; exit 2; }
[ -d "$CORPUS" ] || { echo "FATAL: corpus dir $CORPUS not found"; exit 2; }
case "$CORPUS" in
  */corpus/icon|*/corpus/icon/*)
    echo "REFUSED: $CORPUS is the Icon rung TEST-PROGRAM suite, not a .s-artifact tree."
    echo "         Lon directive 2026-07-11: .s artifacts are maintained for the DEMO and BENCHMARK"
    echo "         corpora ONLY, never for the ~299 rung test programs. Pointing ICON_CORPUS here is"
    echo "         what minted 294 stale artifacts that then LIED about the compiler's real output."
    echo "         Sweep the COMPILER (scrip --compile), not the artifacts. See PROC-ICON-BENCH-ASM.md."
    exit 2 ;;
esac
TMP="$(mktemp -d)"; trap 'rm -rf "$TMP"' EXIT
canon_pl="$TMP/canon.pl"
cat > "$canon_pl" <<'PERL'
my %m; my $i = 0;
local $/; my $s = <>;
$s =~ s/(?<=[A-Za-z])(\d{3,})_/ (exists $m{$1} ? $m{$1} : ($m{$1} = sprintf("%05d", ++$i))) . "_" /ge;
print $s;
PERL
upd=0; new=0; same=0; exc=0; cerr=0; aerr=0; nd=0; total=0; drift=0
for icn in "$CORPUS"/$GLOB; do
  [ -f "$icn" ] || continue
  total=$((total + 1))
  base="${icn%.icn}"; s="$base.s"; name="$(basename "$base")"
  raw="$TMP/$name.raw"; can="$TMP/$name.s"
  if ! timeout 30 "$SCRIP" --compile --target=x86 "$icn" < /dev/null > "$raw" 2>"$TMP/err"; then
    if grep -q '\[SMX\]' "$TMP/err"; then echo "REFUSED  $name"; exc=$((exc + 1));
    else echo "CERR     $name"; cerr=$((cerr + 1)); fi
    continue
  fi
  if grep -q '\[SMX\]' "$TMP/err" || [ ! -s "$raw" ]; then echo "REFUSED  $name"; exc=$((exc + 1)); continue; fi
  raw2="$TMP/$name.raw2"; raw3="$TMP/$name.raw3"
  timeout 30 "$SCRIP" --compile --target=x86 "$icn" < /dev/null > "$raw2" 2>/dev/null
  timeout 30 "$SCRIP" --compile --target=x86 "$icn" < /dev/null > "$raw3" 2>/dev/null
  perl "$canon_pl" < "$raw"  > "$can"
  perl "$canon_pl" < "$raw2" > "$TMP/$name.s2"
  perl "$canon_pl" < "$raw3" > "$TMP/$name.s3"
  if ! diff -q "$can" "$TMP/$name.s2" >/dev/null 2>&1 || ! diff -q "$can" "$TMP/$name.s3" >/dev/null 2>&1; then
    echo "NONDET   $name  (emission order varies run-to-run — not maintainable until deterministic)"; nd=$((nd + 1)); continue
  fi
  asmnote=""
  if ! as "$can" -o "$TMP/$name.o" 2>"$TMP/aserr"; then asmnote="  [ASMWARN: not standalone-assemblable]"; aerr=$((aerr + 1)); fi
  if [ -f "$s" ] && diff -q "$s" "$can" >/dev/null 2>&1; then
    same=$((same + 1)); continue
  fi
  if [ -f "$s" ]; then label="UPDATED"; upd=$((upd + 1)); else label="NEW"; new=$((new + 1)); fi
  drift=$((drift + 1))
  if [ "$CHECK" = "1" ]; then echo "WOULD-$label  $name$asmnote";
  else cp "$can" "$s"; echo "$label  $name$asmnote"; fi
done
echo "--- icon bench .s: total=$total new=$new updated=$upd unchanged=$same refused=$exc nondet=$nd compile-err=$cerr asm-warn=$aerr ---"
if [ "$CHECK" = "1" ] && [ "$drift" -gt 0 ]; then
  echo "CHECK: $drift artifact(s) out of date — run scripts/update_icon_bench_asm.sh to refresh."; exit 1
fi
exit 0
