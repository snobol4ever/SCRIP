#!/usr/bin/env bash
# WREG WIRE CENSUS -- the CORRECT instrument for "did the wires land?".
#
# ⛔ WHY THIS FILE EXISTS (s15b conviction, and it is the SECOND seat in a row):
#   s14 convicted the WREG claim gate for using `grep -c` (counts LINES) and printing the result
#   as `mentions` (occurrences).  s15 then measured wire emission with `grep -c 'lea r10'` against
#   emitter output that pads mnemonics with MULTIPLE SPACES (`lea              r10`).  That pattern
#   CANNOT MATCH.  It returned 0 on an arm that was emitting the wires correctly, and the 0 was
#   read as "the site glue never fired" -- producing two false claims that had to be retracted in
#   the same seat.  ONE INSTRUMENT FAMILY, TWO SEATS, SAME DEFECT: a grep whose UNITS were never
#   checked against its own input.  A zero from a grep is a CLAIM ABOUT TEXT and must be proven
#   like any other claim.  Hence: every pattern here is whitespace-tolerant (` +`) and word-
#   anchored (`\b`), and the script SELF-TESTS those patterns against a synthetic line before it
#   reports a single number.  If the self-test fails the script EXITS NONZERO and reports nothing,
#   because a silent zero is exactly the failure mode being prevented.
SCRIP=${SCRIP:-/home/claude/SCRIP/scrip}
CORPUS=${CORPUS:-/home/claude/corpus/probe}
P_ENT='lea +r10\b'; P_EXG='jmp +r10\b'; P_EXO='jmp +r11\b'; P_BLOB='^proc_PAT\$[0-9]+_α:'; P_PROC='lea +rcx, \[rip'
probe=$(printf '                        lea              r10, [rip + .Lx1]\n                                     jmp   r10\nproc_PAT$0_α:\n')
t=0
echo "$probe" | grep -qE "$P_ENT"  || { echo "SELF-TEST FAIL: entry pattern"; t=1; }
echo "$probe" | grep -qE "$P_EXG"  || { echo "SELF-TEST FAIL: gamma pattern"; t=1; }
echo "$probe" | grep -qE "$P_BLOB" || { echo "SELF-TEST FAIL: blob-label pattern"; t=1; }
[ $t -eq 0 ] || { echo "⛔ INSTRUMENT SELF-TEST FAILED -- refusing to report numbers (s15b law)."; exit 2; }
echo "=== WREG WIRE CENSUS (self-test PASSED: patterns match padded emitter text) ==="
printf "%-24s %6s %6s %6s %6s %6s\n" program blobs entry jmp_r10 jmp_r11 proc_e
tb=0; te=0; tg=0
for f in "$CORPUS"/*.sno; do
  b=$(basename "$f" .sno); [ -n "$1" ] && [ "$b" != "$1" ] && continue
  s=$(SCRIP_WREG=${SCRIP_WREG:-1} "$SCRIP" --compile "$f" 2>/dev/null < /dev/null) || continue
  nb=$(echo "$s" | grep -cE "$P_BLOB"); [ "$nb" = 0 ] && continue
  ne=$(echo "$s" | grep -cE "$P_ENT"); ng=$(echo "$s" | grep -cE "$P_EXG")
  no=$(echo "$s" | grep -cE "$P_EXO"); np=$(echo "$s" | grep -cE "$P_PROC")
  printf "%-24s %6s %6s %6s %6s %6s\n" "$b" "$nb" "$ne" "$ng" "$no" "$np"
  tb=$((tb+nb)); te=$((te+ne)); tg=$((tg+ng))
done
echo "---"
printf "TOTALS  blobs=%s  blob_entries=%s  gamma_exits=%s\n" $tb $te $tg
echo "LAW: gamma_exits MUST equal blobs (every blob converts its own exit pair)."
echo "     blob_entries >= blobs is EXPECTED (several call sites may target one blob)."
[ "$tg" = "$tb" ] && echo "VERDICT: exits==blobs OK" || echo "⛔ VERDICT: exits($tg) != blobs($tb) -- unconverted blob exits exist"
