#!/usr/bin/env bash
# test_icn_n2_host_reserved_agrees.sh -- N-2 item 3: icn_gen_host_reserved() agrees with what emit.cpp actually
# carves (hq_P 2026-08-28, contract UPDATED 2026-08-29 when the flat_gen arm gained its own reservation carve).
#
# THE CLAIM UNDER TEST:  icn_gen_host_reserve_offset() answers (off>=0, base>=0) for a host that actually carved a
# reservation, and REFUSES (-1/-1) for one that did not -- keyed on icn_gen_host_reserved(), which classifies a host
# by WHICH ARM of emit.cpp's three-arm prologue chain it took: if (zframe_graph) :2829 (never reserves) / else if
# (icn_genframe2() && flat_gen) :2832 (N-2 item 3 flat_gen half, 2026-08-29 -- NOW reserves, mirroring the sibling
# arm) / else if (flat_lcl_proc) :2845 (step 2b -- reserves). ⭐ AS OF 2026-08-29 BOTH NON-zframe ARMS RESERVE, so
# every site in this witness set (none of which contain a forward-referenced callee) must ANSWER, regardless of host
# kind -- the old "flat_gen must refuse" assertion is retired, not the gate itself.
#
# ⛔ WHY THIS IS STILL A GATE AFTER THE FLAT_GEN EXTENSION.  icn_gen_host_reserved() MIRRORS AN else-if CHAIN IN
# ANOTHER TRANSLATION UNIT WITH NO COMPILER CHECK THAT IT STILL DOES.  Proof it still catches drift: extending
# emit.cpp's flat_gen arm to carve a reservation WITHOUT updating this predicate in lockstep was caught by this
# exact selftest discipline before it shipped -- SCRIP_N2_OFFSET_SELFTEST=1 on nested.icn measured
# host=proc_outer expect_off=0 got_off=-1 MISMATCH, because icn_gen_host_reserved() still hardcoded flat_gen->0.
# ⛔ EARLIER HISTORY, kept for why the witness set looks the way it does: before the s282 cure the scan was
# HOST-KIND-BLIND in a different way and returned a plausible `off=0 base=128` for a flat_gen host that reserved
# NOTHING -- MEASURED on suspend_nested's inner(), and on a two-caller witness the SAME generator answered
# `off=0 base=128` from its flat_gen host and `off=128 base=240` from its flat_lcl_proc host. Zero is the most
# dangerous answer available -- indistinguishable from a correct first-slot answer -- so a step-3 consumer would
# have carved into a region that was never reserved and corrupted the host frame three layers from the edit. The
# witness set (one flat_lcl_proc-only host, one flat_gen-only host, one generator reached from both) still exists
# to prove the predicate is keyed on the HOST, not the callee -- only the expected ANSWER per class changed.
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
# ⛔ BOTH HOST KINDS MUST BE PRESENT.  A run that saw only one host kind cannot have exercised the OTHER arm of
# emit.cpp's three-arm chain at all -- untested-where-it-matters is not a pass, even though both kinds now expect
# the SAME answer (respond, don't refuse). This is the predicate-drift guard, not a per-kind behavior check.
flproc=$(awk '/host_flat_lcl_proc=1/' "$tmp/dbg.txt" | wc -l)
flgen=$(awk '/host_flat_lcl_proc=0/' "$tmp/dbg.txt" | wc -l)
[ "$flproc" -gt 0 ] || { echo "⛔ REFUSE: no flat_lcl_proc host observed -- cannot exercise that arm of the chain"; exit 2; }
[ "$flgen" -gt 0 ] || { echo "⛔ REFUSE: no flat_gen host observed -- cannot exercise that arm of the chain"; exit 2; }
bad=0
while read -r line; do
    off=$(sed -n 's/.*off=\([0-9-]*\).*/\1/p' <<<"$line")
    bse=$(sed -n 's/.*base=\([0-9-]*\).*/\1/p' <<<"$line")
    if [ "$off" = -1 ] || [ "$bse" = -1 ]; then echo "⛔ FAIL: host REFUSED off=$off base=$bse but this witness set has no forward references -- every site must answer now that both host kinds reserve: $line"; bad=$((bad+1)); fi
done < "$tmp/dbg.txt"
echo "=== N-2 HOST-RESERVED AGREEMENT GATE: sites=$sites flat_lcl_proc_hosts=$flproc flat_gen_hosts=$flgen violations=$bad ==="
[ "$bad" -eq 0 ] || { echo "⛔ FAIL: $bad site(s) got an unexpected refusal -- icn_gen_host_reserved() likely drifted from emit.cpp's chain again"; exit 1; }
echo "✅ PASS: every host in the fixed witness set (both host kinds) answers with a valid off/base"
exit 0
