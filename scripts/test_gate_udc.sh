#!/usr/bin/env bash
# CN-4 GATE -- &USER_DECLARED_CONSTANTS (GOAL-SNOBOL4-100).
# Proves the declaration keyword in BOTH MEDIA (m3 --run BINARY, m4 --compile+gcc TEXT) and BOTH KW killswitch arms.
# ORACLE NOTE: every witness here is ORACLE_FAIL BY CONSTRUCTION -- stock SPITBOL answers error 251 for
# &USER_DECLARED_CONSTANTS exactly as for any undefined &name (measured s148), so refs are SCRIP-pinned, never sbl-diffed.
# g-cn2 (s153): the EVAL/CODE BOUNDARY is gated here too. cn_t1_eval sweeps BOTH T1 arms in BOTH media because the
# defect it guards was arm- AND mode-asymmetric: a runtime fragment inherited the main compile's seal table, so m3 with
# T1 on folded &N against a foreign tree_t and printed EMPTY while m4 -- compiling its fragments in another process --
# printed the value. All four cells must agree, and they must agree with the SEALED CELL, never with the fold.
# WITNESS-AUTHORING TRAP (measured s148): do NOT capture 2>&1 on a witness that both prints and errors -- m4 buffers
# stdout to exit while m3 does not, so the INTERLEAVING differs while both streams are byte-identical. Error witnesses
# therefore carry the .err_sno extension (repo convention) and no .ref, and are checked stream-by-stream below.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
set -u
SCRIP="${SCRIP:-$S4E/SCRIP/scrip}"
RT="${RT:-$S4E/SCRIP/out}"
CN="${CN:-$S4E/corpus/probe/cn}"
pass=0; fail=0
chk() { if [ "$1" = 0 ]; then pass=$((pass+1)); else fail=$((fail+1)); echo "  FAIL: $2"; fi; }
for w in cn_udc_declare cn_udc_reopen; do
  [ -f "$CN/$w.sno" ] || { echo "  FAIL: missing witness $w.sno"; fail=$((fail+1)); continue; }
  "$SCRIP" --compile "$CN/$w.sno" -o "/tmp/gate_$w.s" < /dev/null > /dev/null 2>&1
  gcc -no-pie "/tmp/gate_$w.s" -L"$RT" -lscrip_rt -Wl,-rpath,"$RT" -lm -lpthread -o "/tmp/gate_$w.bin" 2>/dev/null
  for arm in 0 1; do
    SCRIP_KW_STATIC=$arm timeout 20 "$SCRIP" --run "$CN/$w.sno" < /dev/null > /tmp/gate_m3.txt 2>&1
    diff -q /tmp/gate_m3.txt "$CN/$w.ref" > /dev/null 2>&1; chk $? "m3 $w KW_STATIC=$arm"
    SCRIP_KW_STATIC=$arm timeout 20 "/tmp/gate_$w.bin" < /dev/null > /tmp/gate_m4.txt 2>&1
    diff -q /tmp/gate_m4.txt "$CN/$w.ref" > /dev/null 2>&1; chk $? "m4 $w KW_STATIC=$arm"
  done
done
for w in cn_t1_eval cn_t2_eval_boundary; do
"$SCRIP" --compile "$CN/$w.sno" -o "/tmp/gate_$w.s" < /dev/null > /dev/null 2>&1
gcc -no-pie "/tmp/gate_$w.s" -L"$RT" -lscrip_rt -Wl,-rpath,"$RT" -lm -lpthread -o "/tmp/gate_$w.bin" 2>/dev/null
for t1 in 0 1; do
  SCRIP_CONST_T1=$t1 timeout 20 "$SCRIP" --run "$CN/$w.sno" < /dev/null > /tmp/gate_m3.txt 2>/dev/null
  diff -q /tmp/gate_m3.txt "$CN/$w.ref" > /dev/null 2>&1; chk $? "m3 $w CONST_T1=$t1"
  SCRIP_CONST_T1=$t1 timeout 20 "/tmp/gate_$w.bin" < /dev/null > /tmp/gate_m4.txt 2>/dev/null
  diff -q /tmp/gate_m4.txt "$CN/$w.ref" > /dev/null 2>&1; chk $? "m4 $w CONST_T1=$t1"
done
done
w=cn_t1_eval_undecl
"$SCRIP" --compile "$CN/$w.err_sno" -o "/tmp/gate_$w.s" < /dev/null > /dev/null 2>&1
gcc -no-pie "/tmp/gate_$w.s" -L"$RT" -lscrip_rt -Wl,-rpath,"$RT" -lm -lpthread -o "/tmp/gate_$w.bin" 2>/dev/null
timeout 20 "$SCRIP" --run "$CN/$w.err_sno" < /dev/null 2>/dev/null > /tmp/gate_u3.txt
timeout 20 "/tmp/gate_$w.bin" < /dev/null 2>/dev/null > /tmp/gate_u4.txt
diff -q /tmp/gate_u3.txt /tmp/gate_u4.txt > /dev/null 2>&1; chk $? "$w stdout m3==m4"
timeout 20 "$SCRIP" --run "$CN/$w.err_sno" < /dev/null 2>/tmp/gate_ue3.txt > /dev/null
timeout 20 "/tmp/gate_$w.bin" < /dev/null 2>/tmp/gate_ue4.txt > /dev/null
diff -q /tmp/gate_ue3.txt /tmp/gate_ue4.txt > /dev/null 2>&1; chk $? "$w stderr m3==m4"
grep -q "342" /tmp/gate_ue3.txt; chk $? "$w undeclared read in a fragment raises 342"
w=cn_udc_closed
"$SCRIP" --compile "$CN/$w.err_sno" -o "/tmp/gate_$w.s" < /dev/null > /dev/null 2>&1
gcc -no-pie "/tmp/gate_$w.s" -L"$RT" -lscrip_rt -Wl,-rpath,"$RT" -lm -lpthread -o "/tmp/gate_$w.bin" 2>/dev/null
timeout 20 "$SCRIP" --run "$CN/$w.err_sno" < /dev/null 2>/dev/null > /tmp/gate_o3.txt
timeout 20 "/tmp/gate_$w.bin" < /dev/null 2>/dev/null > /tmp/gate_o4.txt
diff -q /tmp/gate_o3.txt /tmp/gate_o4.txt > /dev/null 2>&1; chk $? "$w stdout m3==m4"
timeout 20 "$SCRIP" --run "$CN/$w.err_sno" < /dev/null 2>/tmp/gate_e3.txt > /dev/null
timeout 20 "/tmp/gate_$w.bin" < /dev/null 2>/tmp/gate_e4.txt > /dev/null
diff -q /tmp/gate_e3.txt /tmp/gate_e4.txt > /dev/null 2>&1; chk $? "$w stderr m3==m4"
grep -q "251" /tmp/gate_e3.txt; chk $? "$w raises 251 with the namespace closed"
grep -q "keyword operand is not name of defined keyword" /tmp/gate_e3.txt; chk $? "$w 251 text is oracle-verbatim"
echo "CN-4 GATE: PASS=$pass FAIL=$fail"
[ "$fail" = 0 ] || exit 1
