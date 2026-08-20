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
  timeout 20 "$SCRIP" --run "$CN/$w.sno" < /dev/null > /tmp/gate_m3.txt 2>&1
  diff -q /tmp/gate_m3.txt "$CN/$w.ref" > /dev/null 2>&1; chk $? "m3 $w"
  timeout 20 "/tmp/gate_$w.bin" < /dev/null > /tmp/gate_m4.txt 2>&1
  diff -q /tmp/gate_m4.txt "$CN/$w.ref" > /dev/null 2>&1; chk $? "m4 $w"
  # KW-4 deleted SCRIP_KW_STATIC, so the old `for arm in 0 1` loop ran the SAME binary path twice and
  # counted it as two passes -- the vacuous-gate class.  One arm, graded once, honest count.
done
for w in cn_t1_eval cn_t2_eval_boundary cn_t1_scalar_fold; do   # ⭐ CN-11 added cn_t1_scalar_fold: it was pinned but only ever RUN in the default arm, and the T1=0 arm is where the shared-runtime keyword cascade shadowed a sealed &Pi with Icon's pi (3.141592653589793 vs the declared 3.14). A fold-tier witness must be swept in the arm where the fold is OFF -- that arm is the one that exercises the runtime read path the fold otherwise hides.
for t1 in 0 1; do
  SCRIP_CONST_T1=$t1 timeout 20 "$SCRIP" --run "$CN/$w.sno" < /dev/null > /tmp/gate_m3.txt 2>/dev/null
  diff -q /tmp/gate_m3.txt "$CN/$w.ref" > /dev/null 2>&1; chk $? "m3 $w CONST_T1=$t1"
  # ⛔ CN-11 -- THE m4 ARM WAS VACUOUS AND IS NOW REAL. The compile+link used to sit ABOVE this loop, so the .s was
  # produced ONCE at the DEFAULT arm and the loop then varied SCRIP_CONST_T1 on the already-built binary. T1 is a
  # LOWERING decision: by the time the binary exists the fold is baked into its instructions, so the env var moved
  # nothing and "m4 $w CONST_T1=0" re-tested the T1=1 program twice -- the s68 vacuous-gate class. Measured proof:
  # with the CN-11 defect present, m3 CONST_T1=0 went red while m4 CONST_T1=0 stayed green on the SAME program.
  # Compiling INSIDE the loop with the arm's env applied is what makes the m4 column a second real medium.
  SCRIP_CONST_T1=$t1 "$SCRIP" --compile "$CN/$w.sno" -o "/tmp/gate_$w.s" < /dev/null > /dev/null 2>&1
  gcc -no-pie "/tmp/gate_$w.s" -L"$RT" -lscrip_rt -Wl,-rpath,"$RT" -lm -lpthread -o "/tmp/gate_$w.bin" 2>/dev/null
  SCRIP_CONST_T1=$t1 timeout 20 "/tmp/gate_$w.bin" < /dev/null > /tmp/gate_m4.txt 2>/dev/null
  diff -q /tmp/gate_m4.txt "$CN/$w.ref" > /dev/null 2>&1; chk $? "m4 $w CONST_T1=$t1"
done
done
# ⭐⭐ CN-DOLLAR-ORACLE (queue row `cn-oracle-rulings`, HQ-61 ORACLE-FAITHFUL CONFIRMED) added cn_indirect_rewrite and cn_indirect_seal to this loop.  The first is a LIVE-ORACLE .ref covering the half its
# sibling could not see -- cn_indirect_is_ordinary_var writes each indirect cell exactly ONCE, so it was green all along against a runtime that sealed the cell and raised 341 on the SECOND write.  The
# second is the s153 RULING-REQUEST pin, rewritten to the ruled table; it went red on the first run after the flip, which is exactly what s153 minted it to do.  Both ride the stderr-SILENT assertion too:
# the oracle assigns in total silence, and a 341 printed before an otherwise-correct stdout is a regression the .ref alone structurally cannot see.
for w in cn_t1_eval_undecl cn_eval_fails_not_aborts cn_indirect_is_ordinary_var cn_indirect_rewrite cn_indirect_seal; do
"$SCRIP" --compile "$CN/$w.sno" -o "/tmp/gate_$w.s" < /dev/null > /dev/null 2>&1
gcc -no-pie "/tmp/gate_$w.s" -L"$RT" -lscrip_rt -Wl,-rpath,"$RT" -lm -lpthread -o "/tmp/gate_$w.bin" 2>/dev/null
timeout 20 "$SCRIP" --run "$CN/$w.sno" < /dev/null 2>/dev/null > /tmp/gate_u3.txt
timeout 20 "/tmp/gate_$w.bin" < /dev/null 2>/dev/null > /tmp/gate_u4.txt
diff -q /tmp/gate_u3.txt "$CN/$w.ref" > /dev/null 2>&1; chk $? "m3 $w == ref"
diff -q /tmp/gate_u4.txt "$CN/$w.ref" > /dev/null 2>&1; chk $? "m4 $w == ref"
timeout 20 "$SCRIP" --run "$CN/$w.sno" < /dev/null 2>/tmp/gate_ue3.txt > /dev/null
timeout 20 "/tmp/gate_$w.bin" < /dev/null 2>/tmp/gate_ue4.txt > /dev/null
[ -s /tmp/gate_ue3.txt ]; [ $? = 1 ]; chk $? "$w m3 stderr SILENT (a caught error is not reported)"
[ -s /tmp/gate_ue4.txt ]; [ $? = 1 ]; chk $? "$w m4 stderr SILENT (a caught error is not reported)"
done
# ⭐⭐ CN-EVAL-FAILS (queue row `cn-oracle-rulings`): this block USED to assert the opposite -- `grep 342 <stderr>` on cn_t1_eval_undecl.err_sno, pinning the ABORT that manual v3.7 p.131 forbids.
# s153 pinned it deliberately ("so KW-5 moves it deliberately rather than silently") and Lon's s168 oracle directive moved it: live sbl FAILS the EVAL and runs on, in total silence.  The three
# witnesses now ride ONE loop because the ruling gave them one shape: a real .sno with a .ref in both media, and an EMPTY stderr -- the silence assertion is not decoration, it is the half that
# catches a regression the .ref cannot see, since core_runtime_error printing before the conversion would leave stdout perfect and stderr full of errors the oracle never emits.
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
