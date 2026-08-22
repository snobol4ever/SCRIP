#!/usr/bin/env bash
# board_sno15_ident.sh — CORRECTNESS board for the FIFTEEN working-set demos (s158 scope widening).
# Generalizes test_demo_full_3way.sh (which covered only claws5 + treebank-list) to every .sno in the
# CLAWS5 / TREEBANK / JSON / CALCULATOR families: base programs, *-match, and *-match-fence.
# Encodes the s107 recipes verbatim: oracle temp-prepend (-CASE 0 control card + tab &TRIM = 0 — NEVER
# patch the corpus source), sbl heap -d512m -i64m (this build REJECTS -P), per-family sbl stack bump,
# ulimit -s unlimited for SCRIP both modes (raw SIGSEGV otherwise).
# Reports IDENT/DIVERGE PER MODE, because a program failing in exactly one mode is a MODE34 violation.
# NO TIMING HERE BY DESIGN: a ratio on a diverging mode is meaningless (s158 ~8x trap).
#
# B-9 FIX (BOARD): every non-zero exit under `timeout` was previously collapsed to the single label
# "RC!=0" for both m3 and m4 -- a HANG (timeout's own exit 124) and a CRASH (shell's 128+signal, e.g.
# 139=SIGSEGV, 134=SIGABRT) read identically. Per FINDING-2026-08-12n the 13 broken demo-board programs
# are AT LEAST THREE separate mechanisms (a resumable-generator hang, an R12 arena overrun crash, a wild-
# rbx crash) -- a board that cannot tell hang from crash cannot route a failure to its owner. classify_rc()
# below turns the raw $? into HANG / CRASH(sig) / RC=<n> so the printed column carries that distinction.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
S4A="${S4E_ASSETS:-$([ -d "$S4E/x64" ] && echo "$S4E" || echo /home/claude)}"   # D-17b: ASSET root -- oracles/vendor trees live at the HQ root on this machine (Lon: seats carry ONLY .github/SCRIP/corpus); a root owning its own x64 (HQ, or a full standalone clone-set) is self-contained.
set -u
classify_rc() {  # $1 = exit code from `timeout N cmd`
  local rc=$1
  if [ "$rc" -eq 124 ]; then echo "HANG"
  elif [ "$rc" -ge 128 ]; then echo "CRASH($((rc-128)))"
  else echo "RC=$rc"; fi
}
SC=${SC:-$S4E/SCRIP}; D=${D:-$S4E/corpus/programs/snobol4/demo}
. "$(dirname "${BASH_SOURCE[0]}")/lib_oracle_flags.sh" 2>/dev/null || { echo "REFUSING: cannot load lib_oracle_flags.sh -- the ONE oracle-flag authority (s200/s255)." >&2; exit 3; }
SBL=${SBL:-$S4A/x64/bin/sbl}   # CORRECTNESS oracle -- identity board, no timing, x64/bin/sbl stays authoritative
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
          treebank treebank-match treebank-match-fence \
          json-match json-match-fence \
          calculator-1 calculator-1-match calculator-1-match-fence \
          calculator-2 calculator-2-match calculator-2-match-fence; do
  src="$D/$nm.sno"; inp=$(inp_for "$nm"); xf=$(xf_for "$nm"); note=""
  [ -f "$src" ] || { printf '%-26s %-10s %-10s %s\n' "$nm" - - "NO SOURCE"; nfail=$((nfail+1)); continue; }
  # ---- oracle: temp-prepend the control card, never touch the corpus file ----
  printf -- '-CASE 0\n\t&TRIM = 0\n' > "$W/$nm.sbl.sno"; cat "$src" >> "$W/$nm.sbl.sno"
  if ! timeout $TMO "$SBL" $(sbl_lang_flags) -d512m -i64m $xf "$W/$nm.sbl.sno" < "$inp" > "$W/$nm.sbl" 2>/dev/null; then
    printf '%-26s %-10s %-10s %s\n' "$nm" - - "SBL FAILED"; nfail=$((nfail+1)); continue; fi
  # ---- mode 3 (in-process native) ----
  timeout $TMO "$SC/scrip" --run "$src" < "$inp" > "$W/$nm.m3" 2>/dev/null; rc3=$?
  if [ "$rc3" -eq 0 ]; then
    cmp -s "$W/$nm.sbl" "$W/$nm.m3" && m3=IDENT || m3=DIVERGE
  else m3=$(classify_rc "$rc3"); fi
  # ---- mode 4 (compile via as+gcc) ----
  if timeout $TMO "$SC/scrip" --compile "$src" > "$W/$nm.s" 2>/dev/null \
     && gcc -no-pie "$W/$nm.s" -L"$SC/out" -lscrip_rt -lm -Wl,-rpath,"$SC/out" -o "$W/$nm.prog" 2>/dev/null; then
    timeout $TMO "$W/$nm.prog" < "$inp" > "$W/$nm.m4" 2>/dev/null; rc4=$?
    if [ "$rc4" -eq 0 ]; then
      cmp -s "$W/$nm.sbl" "$W/$nm.m4" && m4=IDENT || m4=DIVERGE
    else m4=$(classify_rc "$rc4"); fi
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
