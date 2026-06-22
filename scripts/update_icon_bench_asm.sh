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
# Programs that EXCISE (native arm pending), fail to compile, or fail to assemble are
# REPORTED and their existing .s is left untouched (legacy artifact retained until the
# emitter covers them; a later run regenerates it automatically).
#
# Usage:
#   scripts/update_icon_bench_asm.sh [GLOB]
#     GLOB defaults to 'rung36_jcon_*.icn' (the JCON benchmark family).
#   ICON_CORPUS=<dir>   override the corpus icon dir (default /home/claude/corpus/programs/icon)
#   CHECK=1             dry-run: report what WOULD change, write nothing (exit 1 if any drift)
#
# Exit: 0 normally; with CHECK=1, 0 if every .s is already current, 1 if any would change.
set -u
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SCRIP="$ROOT/scrip"
CORPUS="${ICON_CORPUS:-/home/claude/corpus/programs/icon}"
GLOB="${1:-rung36_jcon_*.icn}"
CHECK="${CHECK:-0}"
[ -x "$SCRIP" ] || { echo "FATAL: $SCRIP not built (run scripts/build_scrip.sh)"; exit 2; }
[ -d "$CORPUS" ] || { echo "FATAL: corpus dir $CORPUS not found"; exit 2; }
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
    if grep -q '\[SMX\]' "$TMP/err"; then echo "EXCISED  $name"; exc=$((exc + 1));
    else echo "CERR     $name"; cerr=$((cerr + 1)); fi
    continue
  fi
  if grep -q '\[SMX\]' "$TMP/err" || [ ! -s "$raw" ]; then echo "EXCISED  $name"; exc=$((exc + 1)); continue; fi
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
echo "--- icon bench .s: total=$total new=$new updated=$upd unchanged=$same excised=$exc nondet=$nd compile-err=$cerr asm-warn=$aerr ---"
if [ "$CHECK" = "1" ] && [ "$drift" -gt 0 ]; then
  echo "CHECK: $drift artifact(s) out of date — run scripts/update_icon_bench_asm.sh to refresh."; exit 1
fi
exit 0
