#!/usr/bin/env bash
# util_radius_sr3_fix.sh — COMPILE-TIME MD5 BLAST RADIUS for SCRIP_SR3_FIX (s113).
# The fix adds the missing UNION TAG CHECK to the emit.cpp IR_SAVE_RESTORE role-3
# wire-adopt shim (IR_LIT is a union; sval aliases ival, so an IR_LIT_INTEGER gamma
# was strncmp'd as a pointer).  Emits each program twice (SR3_FIX=1 default,
# SR3_FIX=0 killswitch) and reports how many differ.  Per GOAL-SNOBOL4-100 LAWS:
# never grade a codegen change on a single board run — measure the radius first.
# Programs that CRASH the emitter under the killswitch are counted separately:
# they are the fix's customers, not radius movers.
#
# Usage: bash scripts/util_radius_sr3_fix.sh [DIR ...]
set -u
SCRIP=${SCRIP:-/home/claude/SCRIP/scrip}
OUT=${OUT:-/tmp/radius_sr3}
rm -rf "$OUT"; mkdir -p "$OUT"
DIRS=${*:-"/home/claude/corpus/probe/bb /home/claude/corpus/programs/snobol4 /home/claude/corpus/crosscheck"}
tot=0; diffn=0; failn=0; curedn=0
for d in $DIRS; do
    [ -d "$d" ] || continue
    for f in $(find "$d" -name '*.sno' | sort); do
        b=$(echo "$f" | md5sum | cut -c1-12)
        SCRIP_SR3_FIX=1 timeout 25 "$SCRIP" --compile "$f" < /dev/null > "$OUT/$b.on.s"  2>/dev/null
        SCRIP_SR3_FIX=0 timeout 25 "$SCRIP" --compile "$f" < /dev/null > "$OUT/$b.off.s" 2>/dev/null
        roff=$?
        if [ -s "$OUT/$b.on.s" ] && [ $roff -eq 139 ]; then curedn=$((curedn+1)); echo "CURED  $f"; rm -f "$OUT/$b.on.s" "$OUT/$b.off.s"; continue; fi
        if [ ! -s "$OUT/$b.on.s" ] || [ ! -s "$OUT/$b.off.s" ]; then failn=$((failn+1)); rm -f "$OUT/$b.on.s" "$OUT/$b.off.s"; continue; fi
        tot=$((tot+1))
        a=$(md5sum < "$OUT/$b.on.s" | cut -d' ' -f1)
        c=$(md5sum < "$OUT/$b.off.s" | cut -d' ' -f1)
        if [ "$a" != "$c" ]; then diffn=$((diffn+1)); echo "MOVED  $f"; fi
        rm -f "$OUT/$b.on.s" "$OUT/$b.off.s"
    done
done
echo "----"
echo "RADIUS: compared=$tot  differing=$diffn  cured-emitter-crash=$curedn  emit-failed=$failn"
