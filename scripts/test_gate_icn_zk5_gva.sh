#!/usr/bin/env bash
# test_gate_icn_zk5_gva.sh — ZK-5 GATE: globals_on_stack=0 on the cells arm
# CHARTER: IR_VAR/IR_ASSIGN nodes with a global name must NEVER appear armed (zd_on=1)
# on the FORTH-spine ZD regime.  Globals are process-lifetime storage (GVA/NV), not
# control-flow-lifetime ζ; their stack-cell count must be 0 by construction.
# ONE AUTHORITY: the !_icn_cells conjunct in zd_wl_kind (emit.cpp) is the sole guard;
# this script is its falsification criterion per the ZK-0 FALSIFIABILITY rule.
# WITNESS: global_heavy.icn (three globals, one local binop) -- if any global leaks
# onto the spine the counter > 0 and grep finds it.
# COMPLETION: exit 0 = ZK-5 GVA assertion green.
set -e
SCRIP=${SCRIP:-./scrip}
CORPUS=${CORPUS:-/home/claude/corpus}
PASS=0; FAIL=0
check() {
    local label="$1" prog="$2"
    local out
    out=$(SCRIP_ICN_CELLS=1 SCRIP_ZD_CENSUS=1 timeout 8s "$SCRIP" --run "$prog" </dev/null 2>&1)
    local rc=$?
    # must produce correct output (rc=0) and no globals_on_stack > 0
    local bad
    bad=$(printf '%s\n' "$out" | grep 'globals_on_stack=' | grep -v 'globals_on_stack=0' || true)
    if [ $rc -ne 0 ]; then
        echo "FAIL [$label] scrip rc=$rc"
        FAIL=$((FAIL+1))
    elif [ -n "$bad" ]; then
        echo "FAIL [$label] globals_on_stack>0:"
        printf '%s\n' "$bad"
        FAIL=$((FAIL+1))
    else
        echo "PASS [$label] globals_on_stack=0 on all graphs"
        PASS=$((PASS+1))
    fi
}
# lock 1: inline witness (three globals, write-heavy)
cat > /tmp/zk5_global_witness.icn << 'EOF'
global x, y, z;
procedure main();
  x := 1;
  y := 2;
  z := x + y;
  write(z);
end
EOF
check "global_write_heavy" /tmp/zk5_global_witness.icn
# lock 2: palindrome.icn from corpus (global-heavy ZK-0 table entry)
if [ -f "$CORPUS/programs/icon/palindrome.icn" ]; then
    check "palindrome" "$CORPUS/programs/icon/palindrome.icn"
else
    echo "SKIP palindrome.icn (corpus not found)"
fi
# lock 3: local-only program must still show globals_on_stack=0 (trivially -- no globals)
cat > /tmp/zk5_local_only.icn << 'EOF'
procedure main();
  local x;
  x := 42;
  write(x);
end
EOF
check "local_only" /tmp/zk5_local_only.icn
echo "ZK-5 GVA GATE: PASS=$PASS FAIL=$FAIL"
[ $FAIL -eq 0 ]
