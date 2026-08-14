#!/usr/bin/env bash
# test_arbno_witnesses.sh — the EARN-4 named witness set vs the LIVE x64 sbl oracle, m3 + m4 end-to-end.
# Rows: arb1 · 181_pat_arbno_defer_tail_stressors · N22..N33 · earn0_disc_arbno_star_fence_{positive,poisoned}.
# m4 is the honest medium for the stack-slot class (s66 law: m3 exit 0 is not exoneration).
# Usage: SCRIP_ARBNO_RBP=<0|1> bash scripts/test_arbno_witnesses.sh <tag>
set -u
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
CORPUS="${CORPUS:-/home/claude/corpus}"
SBL="${SBL:-/home/claude/x64/bin/sbl}"
TAG="${1:-run}"
W=/tmp/arbw.$TAG; mkdir -p "$W"
FILES="$CORPUS/probe/arb1.sno $CORPUS/crosscheck/patterns/181_pat_arbno_defer_tail_stressors.sno"
for n in 22 23 24 25 26 27 28 29 30 31 32 33; do FILES="$FILES $CORPUS/probe/bb/probes/N$n.sno"; done
FILES="$FILES $CORPUS/probe/earn0/earn0_disc_arbno_star_fence_positive.sno $CORPUS/probe/earn0/earn0_disc_arbno_star_fence_poisoned.sno"
printf '%-46s %-8s %-8s\n' witness m3 m4
for f in $FILES; do
    b=$(basename "$f" .sno)
    [ -f "$f" ] || { printf '%-46s MISSING\n' "$b"; continue; }
    "$SBL" -b "$f" < /dev/null > "$W/$b.ora" 2>/dev/null; orc=$?
    timeout 15 "$ROOT/scrip" --run "$f" < /dev/null > "$W/$b.m3" 2>/dev/null; m3rc=$?
    if [ $m3rc -eq $orc ] && cmp -s "$W/$b.ora" "$W/$b.m3"; then m3="PASS"; else m3="rc=$m3rc/$orc"; cmp -s "$W/$b.ora" "$W/$b.m3" || m3="$m3,DIFF"; fi
    s=$(timeout 15 "$ROOT/scrip" --compile "$f" < /dev/null 2>/dev/null)
    if [ -z "$s" ]; then m4="EMITFAIL"; else
        printf '%s' "$s" > "$W/$b.s"
        if as -o "$W/$b.o" "$W/$b.s" 2>"$W/$b.aserr" && gcc -no-pie -o "$W/$b.bin" "$W/$b.o" "$ROOT/out/libscrip_rt.so" -Wl,-rpath,"$ROOT/out" 2>"$W/$b.lderr"; then
            timeout 15 "$W/$b.bin" < /dev/null > "$W/$b.m4" 2>/dev/null; m4rc=$?
            if [ $m4rc -eq $orc ] && cmp -s "$W/$b.ora" "$W/$b.m4"; then m4="PASS"; else m4="rc=$m4rc/$orc"; cmp -s "$W/$b.ora" "$W/$b.m4" || m4="$m4,DIFF"; fi
        else m4="ASM/LINK"; fi
    fi
    printf '%-46s %-8s %-8s\n' "$b" "$m3" "$m4"
done
