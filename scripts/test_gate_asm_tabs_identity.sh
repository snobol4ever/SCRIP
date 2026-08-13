#!/usr/bin/env bash
# test_gate_asm_tabs_identity.sh — TAB-COLUMN REFACTOR GATE (GOAL-RBP-EARN, Lon directive s62b).
# The TAB record format is a REPRESENTATION change, never an OUTPUT change: the .s bytes the sink
# writes must be IDENTICAL with SCRIP_ASM_TABS=1 (records carry TAB-delimited fields) and
# SCRIP_ASM_TABS=0 (legacy space-delimited producers).  A producer missed by the refactor emits a
# record with no TAB, lands on the legacy parse path, and — if that parse disagrees by one byte —
# shows up here as a DIFF.  Same discipline as the JOIN rung: md5 across the demo corpus, 22/22.
# Usage: bash scripts/test_gate_asm_tabs_identity.sh [corpus_dir]
set -u
SCRIP=${SCRIP:-/home/claude/SCRIP/scrip}
DIR=${1:-/home/claude/corpus/programs/snobol4/demo}
WORK=$(mktemp -d); trap 'rm -rf "$WORK"' EXIT
pass=0; fail=0; skip=0; failed=""
for f in "$DIR"/*.sno; do
    [ -e "$f" ] || continue
    b=$(basename "$f" .sno)
    SCRIP_ASM_TABS=0 timeout 60 "$SCRIP" --compile "$f" > "$WORK/$b.off.s" 2>"$WORK/$b.off.err" < /dev/null
    rc0=$?
    SCRIP_ASM_TABS=1 timeout 60 "$SCRIP" --compile "$f" > "$WORK/$b.on.s"  2>"$WORK/$b.on.err"  < /dev/null
    rc1=$?
    if [ $rc0 -ne 0 ] || [ $rc1 -ne 0 ]; then
        if [ $rc0 -eq $rc1 ]; then skip=$((skip+1)); continue; fi
        fail=$((fail+1)); failed="$failed $b(rc $rc0/$rc1)"; continue
    fi
    m0=$(md5sum < "$WORK/$b.off.s" | cut -d' ' -f1)
    m1=$(md5sum < "$WORK/$b.on.s"  | cut -d' ' -f1)
    if [ "$m0" = "$m1" ]; then pass=$((pass+1)); else
        fail=$((fail+1)); failed="$failed $b"
        diff "$WORK/$b.off.s" "$WORK/$b.on.s" | head -12 > "$WORK/$b.diff"
        echo "--- DIFF $b (off vs on) ---"; cat "$WORK/$b.diff"
    fi
done
echo "ASM-TABS IDENTITY: PASS=$pass FAIL=$fail SKIP=$skip  (dir=$DIR)"
[ -n "$failed" ] && echo "FAILED:$failed"
[ $fail -eq 0 ] && { echo "GATE GREEN"; exit 0; }
echo "GATE RED"; exit 1
