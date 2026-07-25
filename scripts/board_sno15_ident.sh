#!/usr/bin/env bash
# board_sno15_ident.sh — CORRECTNESS board for the FIFTEEN working-set demos (s158 scope widening).
# Generalizes test_demo_full_3way.sh (which covered only claws5 + treebank-list) to every .sno in the
# CLAWS5 / TREEBANK / JSON / CALCULATOR families: base programs, *-match, and *-match-fence.
# Encodes the s107 recipes verbatim: oracle temp-prepend (-CASE 0 control card + tab &TRIM = 0 — NEVER
# patch the corpus source), sbl heap -d512m -i64m (this build REJECTS -P), per-family sbl stack bump,
# ulimit -s unlimited for SCRIP both modes (raw SIGSEGV otherwise).
# Reports IDENT/DIVERGE PER MODE, because a program failing in exactly one mode is a MODE34 violation.
# NO TIMING HERE BY DESIGN: a ratio on a diverging mode is meaningless (s158 treebank-array ~8x trap).
set -u
SC=${SC:-/home/claude/SCRIP}; D=${D:-/home/claude/corpus/programs/snobol4/demo}
SBL=${SBL:-/home/claude/x64/bin/sbl}
W=$(mktemp -d); trap 'rm -rf "$W"' EXIT
ulimit -s unlimited
TMO=${TMO:-300}
inp_for() { case $1 in claws5*) echo "$D/CLAWS5inTASA.dat";; treebank*) echo "$D/VBGinTASA.dat";;
  json*) echo "$D/twitter.json";; calculator*) echo "$D/calculator.input";; *) echo "";; esac; }
xf_for()  { case $1 in treebank*) echo "-s256m";; *) echo "";; esac; }
printf '%-26s %-10s %-10s %s\n' PROGRAM M3 M4 NOTE
printf '%.0s-' {1..66}; echo
nid=0; nd3=0; nd4=0; nfail=0
for nm in claws5 claws5-match claws5-match-fence \
          treebank-list treebank-array treebank-match treebank-match-fence \
          json-match json-match-fence \
          calculator-1 calculator-1-match calculator-1-match-fence \
          calculator-2 calculator-2-match calculator-2-match-fence; do
  src="$D/$nm.sno"; inp=$(inp_for "$nm"); xf=$(xf_for "$nm"); note=""
  [ -f "$src" ] || { printf '%-26s %-10s %-10s %s\n' "$nm" - - "NO SOURCE"; nfail=$((nfail+1)); continue; }
  # ---- oracle: temp-prepend the control card, never touch the corpus file ----
  printf -- '-CASE 0\n\t&TRIM = 0\n' > "$W/$nm.sbl.sno"; cat "$src" >> "$W/$nm.sbl.sno"
  if ! timeout $TMO "$SBL" -b -d512m -i64m $xf "$W/$nm.sbl.sno" < "$inp" > "$W/$nm.sbl" 2>/dev/null; then
    printf '%-26s %-10s %-10s %s\n' "$nm" - - "SBL FAILED"; nfail=$((nfail+1)); continue; fi
  # ---- mode 3 (in-process native) ----
  if timeout $TMO "$SC/scrip" --run "$src" < "$inp" > "$W/$nm.m3" 2>/dev/null; then
    cmp -s "$W/$nm.sbl" "$W/$nm.m3" && m3=IDENT || m3=DIVERGE
  else m3="RC!=0"; fi
  # ---- mode 4 (compile via as+gcc) ----
  if timeout $TMO "$SC/scrip" --compile "$src" > "$W/$nm.s" 2>/dev/null \
     && gcc -no-pie "$W/$nm.s" -L"$SC/out" -lscrip_rt -lm -Wl,-rpath,"$SC/out" -o "$W/$nm.prog" 2>/dev/null; then
    if timeout $TMO "$W/$nm.prog" < "$inp" > "$W/$nm.m4" 2>/dev/null; then
      cmp -s "$W/$nm.sbl" "$W/$nm.m4" && m4=IDENT || m4=DIVERGE
    else m4="RC!=0"; fi
  else m4="BUILD-FAIL"; fi
  [ "$m3" = IDENT ] || nd3=$((nd3+1)); [ "$m4" = IDENT ] || nd4=$((nd4+1))
  [ "$m3" = IDENT ] && [ "$m4" = IDENT ] && nid=$((nid+1))
  # a program failing in EXACTLY one mode is a MODE34-IDENTICAL violation — name it
  [ "$m3" != "$m4" ] && note="MODE34-VIOLATION"
  [ -s "$W/$nm.sbl" ] || note="$note (sbl output empty)"
  printf '%-26s %-10s %-10s %s\n' "$nm" "$m3" "$m4" "$note"
done
printf '%.0s-' {1..66}; echo
echo "TRI-IDENTICAL $nid/15   m3 bad=$nd3   m4 bad=$nd4   harness-fail=$nfail"
