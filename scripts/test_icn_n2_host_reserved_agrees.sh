#!/usr/bin/env bash
# test_icn_n2_host_reserved_agrees.sh -- N-2 item 3: THE HOST-RESERVED REFUSAL invariant (hq_P 2026-08-28).
#
# THE CLAIM UNDER TEST:  icn_gen_host_reserve_offset() answers ONLY for a host that actually carved a reservation,
# and REFUSES (-1/-1) for one that did not.  icn_gen_host_reserve() is called from exactly one place (emit.cpp:2852),
# inside the THIRD arm of a three-arm prologue chain -- if (zframe_graph) :2829 / else if (icn_genframe2() &&
# flat_gen) :2832 / else if (flat_lcl_proc) :2845 -- so a graph taking either of the first two arms reserves NOTHING.
#
# ⛔ WHY THIS IS A GATE.  Before the s282 cure the scan was HOST-KIND-BLIND and returned a plausible `off=0 base=128`
# for a flat_gen host: MEASURED on suspend_nested's inner(), and on a two-caller witness the SAME generator answered
# `off=0 base=128` from its flat_gen host and `off=128 base=240` from its flat_lcl_proc host.  Zero is the most
# dangerous answer available -- indistinguishable from a correct first-slot answer -- so a step-3 consumer would carve
# into a region that was never reserved and corrupt the host frame three layers from the edit.
# ⛔ AND THE PREDICATE MIRRORS AN else-if CHAIN IN ANOTHER TRANSLATION UNIT WITH NO COMPILER CHECK THAT IT STILL DOES.
# That is precisely what rots silently: add a fourth arm before :2845, or move the reserve call, and the predicate is
# wrong with nothing to say so.  This gate is that missing check.
#
# THREE STATES:  rc=0 invariant holds  |  rc=1 violated  |  rc=2 could not measure (never skip-as-success)
set -u
here=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd); root=$(cd "$here/.." && pwd); cd "$root" || exit 2
scrip="$root/scrip"
[ -x "$scrip" ] || { echo "⛔ REFUSE: no ./scrip at $scrip -- cannot measure"; exit 2; }
tmp=$(mktemp -d) || exit 2; trap 'rm -rf "$tmp"' EXIT
# ⭐ THE WITNESS SET IS FIXED AND SELF-CONTAINED, and it MUST contain both host kinds: a flat_lcl_proc host (main
# calling a generator) and a flat_gen host (a generator calling a generator).  mixed_caller is the load-bearing one --
# ONE generator reached from BOTH host kinds, which is what proves the answer is keyed on the HOST and not the callee.
printf '%s\n' 'procedure gen()'  '   suspend 1;' 'end' 'procedure main()' '   write(gen());' 'end' > "$tmp/single.icn"
printf '%s\n' 'procedure inner()' '   suspend 1 | 2;' 'end' 'procedure outer()' '   suspend inner();' 'end' 'procedure main()' '   every write(outer());' 'end' > "$tmp/nested.icn"
printf '%s\n' 'procedure leaf()' '   suspend 1 | 2;' 'end' 'procedure mid()' '   suspend leaf();' 'end' 'procedure main()' '   every write(leaf());' '   every write(mid());' 'end' > "$tmp/mixed.icn"
: > "$tmp/all.log"
for w in single nested mixed; do
    SCRIP_ICN_GENFRAME2=1 SEAT01_N2_STEP3_DBG=1 timeout 30s "$scrip" --compile -o "$tmp/$w.s" "$tmp/$w.icn" </dev/null 2>>"$tmp/all.log" >/dev/null
done
grep -a 'N2-STEP3-DBG' "$tmp/all.log" > "$tmp/dbg.txt" 2>/dev/null
sites=$(wc -l < "$tmp/dbg.txt")
[ "$sites" -gt 0 ] || { echo "⛔ REFUSE: zero generator call sites observed -- the diagnostic did not fire, so nothing was measured (an empty denominator is not a pass)"; exit 2; }
# ⛔ BOTH CLASSES MUST BE PRESENT.  A run that saw only reserving hosts cannot have exercised the refusal at all, and
# would pass while the cure was entirely absent -- untested-where-it-matters is not a pass.
resv=$(awk '/host_flat_lcl_proc=1/' "$tmp/dbg.txt" | wc -l)
nonr=$(awk '/host_flat_lcl_proc=0/' "$tmp/dbg.txt" | wc -l)
[ "$resv" -gt 0 ] || { echo "⛔ REFUSE: no reserving (flat_lcl_proc) host observed -- cannot test the answering path"; exit 2; }
[ "$nonr" -gt 0 ] || { echo "⛔ REFUSE: no non-reserving (flat_gen) host observed -- cannot test the REFUSAL path, which is the whole point of this gate"; exit 2; }
bad=0
while read -r line; do
    lcl=$(sed -n 's/.*host_flat_lcl_proc=\([0-9-]*\).*/\1/p' <<<"$line")
    off=$(sed -n 's/.*off=\([0-9-]*\).*/\1/p' <<<"$line")
    bse=$(sed -n 's/.*base=\([0-9-]*\).*/\1/p' <<<"$line")
    if [ "$lcl" = 0 ]; then
        if [ "$off" != -1 ] || [ "$bse" != -1 ]; then echo "⛔ FAIL: non-reserving host answered off=$off base=$bse (must be -1/-1): $line"; bad=$((bad+1)); fi
    else
        if [ "$off" = -1 ] || [ "$bse" = -1 ]; then echo "⛔ FAIL: reserving host REFUSED off=$off base=$bse (must answer): $line"; bad=$((bad+1)); fi
    fi
done < "$tmp/dbg.txt"
echo "=== N-2 HOST-RESERVED REFUSAL GATE: sites=$sites reserving=$resv non-reserving=$nonr violations=$bad ==="
[ "$bad" -eq 0 ] || { echo "⛔ FAIL: $bad site(s) disagree with icn_gen_host_reserved()"; exit 1; }
echo "✅ PASS: every non-reserving host refuses (-1/-1); every reserving host answers"
exit 0
