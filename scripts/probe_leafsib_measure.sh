#!/usr/bin/env bash
# probe_leafsib_measure.sh - s131 SIBLING WIDENING instrument (GOAL-SNOBOL4-100 R-0 leaf class).
# Runs corpus/probe/leafsib/ in BOTH media against the checked-in .ref oracle output, under BOTH
# killswitch arms.  A sibling is RED iff its stdout differs from its .ref (a SEGV, an abort, a hang
# and a silent wrong answer all count the same - the .ref is the only truth).  No pipelines around
# the compiler: rc is captured directly so a 139 is never laundered into a 0 by `head`.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
cd "$(dirname "$0")/.." || exit 1
P=$S4E/corpus/probe/leafsib
SIBS="${SIBS:-span tab rtab rem arb bal break breakx arb_flat_red arb_flat_grn bal_flat_red bal_flat_grn}"   # the eight one-per-member siblings, plus the s193 PADDED pairs.  ⛔ THE EIGHT ARE INERT AT HEAD: all eight emit BYTE-IDENTICAL --compile output under BOTH killswitch arms, because they take the STORED road (`t = (... | ARB . I)`) whose cell is already rbp-resident by blob-frame authority, so they are green for a reason unrelated to the switch they exist to test.  The padded pairs put the same leaf on an INLINE ALT arm, where the cell IS a raw flat rsp coordinate, and are the members that still discriminate the arms (row alt-arb-bal-witness, s193).
case "${SCRIP_SPAN_FRAME-}" in 0) ARM=0;; *) ARM=1;; esac   # ⛔ COMPUTED, NEVER TYPED: this must mirror sn4_span_frame() (emit.cpp) EXACTLY -- `(e && *e == '0') ? 0 : 1`, i.e. UNSET IS ON.  It read `${SCRIP_SPAN_FRAME:-0}` from s131 until s193 and so labelled every default-arm run `=0` after the s188 flip (d3251f23) made unset mean ON: the instrument printed the OFF label over ON-arm numbers for five sessions.
printf '=== leafsib  SCRIP_SPAN_FRAME=%s ===\n' "$ARM"
printf '%-14s | %-22s | %-22s\n' "sib" "m3 (--run BINARY)" "m4 (--compile TEXT)"
printf -- '---------------|------------------------|------------------------\n'
pass3=0; pass4=0; tot=0
for f in $SIBS; do
    src="$P/leafsib_$f.sno"; ref="$P/leafsib_$f.ref"
    [ -f "$src" ] || continue
    tot=$((tot+1))
    timeout 20 ./scrip --run "$src" < /dev/null > /tmp/ls_m3.out 2>/dev/null; r3=$?
    d3="FAIL"; if [ $r3 -eq 0 ] && cmp -s /tmp/ls_m3.out "$ref"; then d3="pass"; pass3=$((pass3+1)); fi
    rm -f /tmp/ls_bin /tmp/ls.s
    timeout 90 ./scrip --compile "$src" < /dev/null > /tmp/ls.s 2>/dev/null; rc=$?
    d4="FAIL"; r4="c$rc"
    if [ $rc -eq 0 ] && [ -s /tmp/ls.s ]; then
        if gcc -no-pie -o /tmp/ls_bin /tmp/ls.s out/libscrip_rt.so > /dev/null 2>&1; then
            timeout 20 /tmp/ls_bin < /dev/null > /tmp/ls_m4.out 2>/dev/null; r4=$?
            if [ "$r4" -eq 0 ] && cmp -s /tmp/ls_m4.out "$ref"; then d4="pass"; pass4=$((pass4+1)); fi
        else r4="gcc"; fi
    fi
    printf '%-14s | rc=%-4s %-14s | rc=%-4s %-14s\n' "$f" "$r3" "$d3" "$r4" "$d4"
done
printf -- '---------------|------------------------|------------------------\n'
printf '%-14s | m3 %d/%d%-17s | m4 %d/%d\n' "TOTAL" "$pass3" "$tot" "" "$pass4" "$tot"
