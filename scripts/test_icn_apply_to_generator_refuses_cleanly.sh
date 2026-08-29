#!/usr/bin/env bash
# test_icn_apply_to_generator_refuses_cleanly.sh -- ACCEPTANCE INSTRUMENT for task
# icon-apply-to-generator-segv-bb-call-value-has-no-n2-awareness.
#
# WHAT THIS CHECKS. `gen ! [10])` (Icon apply, dynamically calling a jmp-entry generator procedure through a
# runtime VALUE) SIGSEGV'd in the default build, rc=139, 3/3 on a pristine tree -- seat10's original finding,
# corroborated by hq_B. MEASURED (not assumed) that NONE of the three candidate landings work: the native spine
# transfer reads garbage as the N-2 region pointer at [rsp+16] (armed, default); the SAME spine transfer ALSO
# SIGSEGVs 3/3 with SCRIP_ICN_GENFRAME2=0 (a second, independent, pre-existing defect -- this is not purely an
# N-2 regression); and forcing the "one-shot C window" fallback SIGSEGVs a THIRD way because rt_genp_spine_enter
# (src/runtime/rt/rt.c) still delivers gamma/omega through rcx/rdx, the pre-N-1 protocol every other caller left
# behind at THE CROSSING. Indirect dispatch through IR_CALL_VALUE to a generator is UNRULED (GOAL-ICON-100.md N-2
# FINDING 2026-08-29: "Indirect dispatch (IR_CALL_VALUE, 70 sites) stays UNRULED (hq_C's one-shape-test design)").
#
# ⛔ THIS IS A REFUSAL TEST, NOT A CORRECTNESS TEST for the apply-to-generator construct itself. The fix
# (src/runtime/by_name_dispatch.c, rt_call_value_spine_prep) does not make `gen ! [10])` work -- it converts the
# silent SIGSEGV into a loud, named rt_bomb refusal (rc=134), matching the CEO-ruled precedent for the sibling
# flat_gen-host call-site refusal ("REFUSE LOUDLY under arming, rt_bomb, named message"). If a future row lands a
# real fix, THIS witness's assertion should be loosened from "refuses with rc=134" to "prints 10, rc=0" -- until
# then, rc=134 is the floor this row earned and rc=139 (or any other crash) is a regression back to the live bug.
#
# CONTROL ARMS (must keep working -- prove the refusal is scoped to jmp-entry generators only, not every apply
# or every indirect call):
#   apply_det      -- `square ! [7])` (apply to a DETERMINISTIC proc value) must print 49, rc=0, both modes.
#   direct_gen     -- `write(gen(10))` (a DIRECT, static call to a generator -- bcps_spine_gen_arm, a completely
#                      different call site this fix never touches) must print 10, rc=0, both modes.
#   value_call_gen -- `procs := [gen]; write(procs[1](10))` (a plain, non-apply, indirect VALUE call to a
#                      generator through a subscript -- a bare identifier callee like `p(10)` resolves as a static
#                      name lookup in this front-end and never reaches IR_CALL_VALUE at all, "Undefined function",
#                      a separate pre-existing quirk this row does not touch; subscripting forces genuine runtime
#                      resolution) must ALSO refuse cleanly (rc=134), proving the fix covers both cv_is_apply()
#                      arms of bb_call_value.cpp (both call into the same rt_call_value_spine_prep), not apply alone.
#
# USAGE: bash scripts/test_icn_apply_to_generator_refuses_cleanly.sh
# EXIT:  0 = crash witness refuses cleanly (rc=134, named BOMB on stderr) AND all three control arms CORRECT,
#            both modes.
#        1 = crash witness still SIGSEGVs (or anything else unexpected), or a control arm regressed.
#        2 = REFUSED, could not measure (compiler/runtime missing).
set -u
cd "$(dirname "$0")/.." || { echo "⛔ REFUSE rc=2: cannot reach SCRIP root"; exit 2; }
ROOT="$PWD"
[ -x "$ROOT/scrip" ] || { echo "⛔ REFUSE rc=2: ./scrip is not built."; exit 2; }
[ -f "$ROOT/out/libscrip_rt.so" ] || { echo "⛔ REFUSE rc=2: out/libscrip_rt.so missing -- mode-4 cannot link."; exit 2; }
WORK="$(mktemp -d)" || { echo "⛔ REFUSE rc=2: no scratch dir"; exit 2; }
trap 'rm -rf "$WORK"' EXIT
mkw() { printf '%s\n' "$2" > "$WORK/$1.icn"; }
mkw apply_gen_segv 'procedure gen(x)
   suspend x;
end
procedure main()
   every write(gen ! [10]);
end'
mkw apply_det 'procedure square(x)
   return x * x;
end
procedure main()
   write(square ! [7]);
end'
mkw direct_gen 'procedure gen(x)
   suspend x;
end
procedure main()
   write(gen(10));
end'
mkw value_call_gen 'procedure gen(x)
   suspend x;
end
procedure main()
   procs := [gen];
   write(procs[1](10));
end'
run_m3() { local w="$1"
    ( cd "$WORK" && timeout 10s "$ROOT/scrip" --run "$w.icn" </dev/null >"$w.m3.out" 2>"$w.m3.err" ); echo $?; }
run_m4() { local w="$1"
    if ( cd "$WORK" && timeout 10s "$ROOT/scrip" --compile -o "$w.s" "$w.icn" </dev/null ) >/dev/null 2>&1 \
       && ( cd "$WORK" && as -o "$w.o" "$w.s" ) >/dev/null 2>&1 \
       && ( cd "$WORK" && gcc -o "$w.bin" "$w.o" -L"$ROOT/out" -lscrip_rt -Wl,-rpath,"$ROOT/out" ) >/dev/null 2>&1; then
        ( cd "$WORK" && timeout 10s "./$w.bin" </dev/null >"$w.m4.out" 2>"$w.m4.err" ); echo $?
    else
        echo "NOBUILD"
    fi }
FAIL=0
rc3="$(run_m3 apply_gen_segv)"; rc4="$(run_m4 apply_gen_segv)"
echo "apply_gen_segv:  m3 rc=$rc3   m4 rc=$rc4   (want 134 = clean rt_bomb refusal; 139 = still the live SIGSEGV)"
if [ "$rc3" = "139" ] || [ "$rc4" = "139" ]; then
    echo "⛔ FAIL: still SIGSEGVing -- the live crash this row exists to cure is NOT cured."; FAIL=1
elif [ "$rc3" != "134" ] || [ "$rc4" != "134" ]; then
    echo "⛔ FAIL: rc is neither 134 (clean refusal) nor 139 (known crash) in at least one mode -- an unclassified outcome, treated as a failure."; FAIL=1
else
    grep -q "BOMB" "$WORK/apply_gen_segv.m3.err" 2>/dev/null || { echo "⛔ FAIL: m3 rc=134 but no BOMB message on stderr -- rc=134 might be an unrelated abort, not this refusal."; FAIL=1; }
    grep -q "BOMB" "$WORK/apply_gen_segv.m4.err" 2>/dev/null || { echo "⛔ FAIL: m4 rc=134 but no BOMB message on stderr -- rc=134 might be an unrelated abort, not this refusal."; FAIL=1; }
fi
check_control() { local w="$1" exp="$2" rc3 rc4 got3 got4
    rc3="$(run_m3 "$w")"; got3="$(cat "$WORK/$w.m3.out" 2>/dev/null)"
    rc4="$(run_m4 "$w")"; got4="$(cat "$WORK/$w.m4.out" 2>/dev/null)"
    echo "$w:  m3 rc=$rc3 out='$got3'   m4 rc=$rc4 out='$got4'   (want rc=0 out='$exp' both modes)"
    if [ "$rc3" != "0" ] || [ "$got3" != "$exp" ] || [ "$rc4" != "0" ] || [ "$got4" != "$exp" ]; then
        echo "⛔ FAIL: control arm '$w' regressed."; FAIL=1
    fi }
check_control apply_det "49"
check_control direct_gen "10"
rc3="$(run_m3 value_call_gen)"; rc4="$(run_m4 value_call_gen)"
echo "value_call_gen:  m3 rc=$rc3   m4 rc=$rc4   (want 134 = the shared gate also covers the non-apply indirect-value-call arm)"
[ "$rc3" = "134" ] && [ "$rc4" = "134" ] || { echo "⛔ FAIL: the non-apply indirect-value-call arm did not refuse the same way."; FAIL=1; }
echo "=== tree: $(git -C "$ROOT" rev-parse --short HEAD 2>/dev/null || echo '?') ==="
if [ "$FAIL" -eq 0 ]; then echo "✅ ALL-GREEN"; exit 0; else echo "⛔ FAIL — see above"; exit 1; fi
