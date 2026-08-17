#!/usr/bin/env bash
# test_gate_clobarm.sh -- s130 LEAF-SUSPENSION FRAME SLOT gate (GOAL-SNOBOL4-100 R-0 class).
# Runs corpus/probe/clobarm/ in BOTH modes against the checked-in .ref oracle, under whatever
# SCRIP_SPAN_FRAME the caller sets, and prints one PASS/FAIL row per program plus a tally.
SCRIP=${SCRIP:-/home/claude/SCRIP/scrip}
DIR=${DIR:-/home/claude/corpus/probe/clobarm}
RT=${RT:-/home/claude/SCRIP/out/libscrip_rt.so}
pass=0; fail=0
for f in "$DIR"/*.sno; do
  b=$(basename "$f" .sno); ref=$(cat "$DIR/$b.ref" 2>/dev/null)
  m3=$(timeout 15 "$SCRIP" --run "$f" < /dev/null 2>/dev/null); r3=$?
  s=/tmp/clobarm_$b.s; o=/tmp/clobarm_$b.bin; m4=""; r4=1
  if timeout 15 "$SCRIP" --compile "$f" > "$s" 2>/dev/null; then
    if gcc -no-pie -o "$o" "$s" "$RT" > /dev/null 2>&1; then
      m4=$(timeout 15 "$o" < /dev/null 2>/dev/null); r4=$?
    fi
  fi
  v3=FAIL; v4=FAIL
  [ "$m3" = "$ref" ] && v3=PASS
  [ "$m4" = "$ref" ] && v4=PASS
  if [ "$v3" = PASS ] && [ "$v4" = PASS ]; then pass=$((pass+1)); else fail=$((fail+1)); fi
  printf "%-34s m3=%-4s rc=%-4s m4=%-4s rc=%-4s got3=[%s]\n" "$b" "$v3" "$r3" "$v4" "$r4" "$(echo "$m3" | head -1)"
done
echo "clobarm: PASS=$pass FAIL=$fail (both modes must match .ref)"
[ "$fail" -eq 0 ]
