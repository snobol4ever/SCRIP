#!/usr/bin/env bash
# test_gate_c2bb_the_generator_arm_is_a_coroutine_start_not_a_box_entry_from_c.sh
#
# ⛔⭐ WHAT THIS GATE HOLDS, AND WHY IT IS A MEASUREMENT AND NOT AN OPINION (cto 2026-09-21, CTO-123, MODE
# QUARTET, under CEO-1093's re-cut of the C->BB->C->BB eradication).  rt_proc_call_gen_h is TWO roads wearing
# ONE name, and every instrument in the tree charged both of them to the caller:
#   * p->is_generator  -- ct_zalloc a rt_genp_s, scrip_co_ctx_init(&g->co, rt_genp_thread_entry, g),
#     scrip_coexpr_activate.  The box is entered by rt_genp_entry_c ON THE COROUTINE'S OWN pthread STACK,
#     at the site RULES.md/CEO-970 SANCTIONS by name (genp.spine / genp.spine.n2).  The caller's C frame is
#     never underneath emitted code: it is parked in scrip_coexpr_activate on a different stack.
#   * p->jmp_entry     -- rt_proc_call_open then rt_proc_enter, hit as gen_h.enter.  THIS one is a real C->BB
#     entry from the caller's own stack and is exactly what the eradication is for.
# The three by_name_dispatch.c riders (rt_call_value_gen_h :1297, rt_pl_goal_gen_h_c :1457,
# rt_call_value_resume_h :1466) hit BEFORE that branch, so their record says "C entered a box" whichever road
# ran.  test_gate_no_c_to_bb.sh's rule (e) convicts them the same way: it flags any non-rt.c C caller of
# rt_proc_call_gen_h( / rt_proc_resume_frame_h( without regard to the arm.
#
# ⭐ SO THE ARMS NOW NAME THEMSELVES IN THE TRACE -- gen_h.coro and gen_h.coro_resume, added INSIDE rt.c beside
# the existing gen_h.enter.  ⛔ NOTHING WAS UNHOOKED TO GET HERE (MODE line 2: no seat cures its half by
# deleting an rt_c2bb_hit).  The three rider hits are untouched and util_c2bb_coverage.py still reads them; the
# record count per rider call is unchanged and every EMITTED-code caller of the same API -- which no census had
# ever seen take this road -- now names its arm too, so the trace got strictly louder, never quieter.
#
# ⭐ WHAT IS ASSERTED, per witness, over a WITNESS population (never corpus/: ONE RUNNER, ONE BOARD, CEO-523):
#   A  the program still answers its .ref byte for byte -- a trace claim over a wrong answer is worthless
#   B  gen_h.enter is ABSENT: no rider hit on this population reached the one arm that enters a box from C
#   C  #gen_h.coro == #genp.spine + #genp.spine.n2 -- every generator-arm call starts EXACTLY ONE coroutine,
#      and that start is the sanctioned site.  This is the equality that carries the whole finding.
#   D  #gen_h.call_value + #gen_h.pl_goal == #gen_h.coro -- every rider record is accounted for by the
#      coroutine arm, so none of them is unexplained
#   E  #gen_h.resume_frame == #gen_h.coro_resume, and a resume starts NO new coroutine (it is C's share of C)
# ⛔ NON-INERT OR REFUSE (CEO-1044): a witness whose gen_h.coro count is ZERO never took the arm and proves
# nothing, so this REFUSES rc=2 rather than passing.  A silent trace is as available to a broken build as to a
# cure -- the defect 26af9d482 landed on.
#
# ⛔ WHAT THIS GATE DOES NOT SAY, named so nobody reads it as wider than it is: the rider's C frame still holds
# DESCRs (callee, argv, nm) in a C activation no compile-time frame map describes, and at a collection inside
# the coroutine that frame is live on the main thread's stack.  THAT hazard survives this gate untouched and is
# the unmapped-C-frame row, not this one.  What does not survive is the claim that these three sites ENTER A
# BOX FROM C on the road they actually take.
#
# ⭐ FAIL-ONCE IS BUILT IN: FAIL_ONCE=1 plants one synthetic gen_h.enter record into the first witness's trace
# and requires arm B to red on it.  A gate that cannot say no is the instrument this fleet keeps re-finding.
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
cd "$ROOT" || exit 2

WITNESSES="hb_coexpr_genp_scan hb_cv_spine_plain_redo"
TMP="${TMPDIR:-/tmp}/c2bb_genarm_$$"
mkdir -p "$TMP" || exit 2
trap 'rm -rf "$TMP"' EXIT

stamp="SCRIP $(git -C "$ROOT" rev-parse --short HEAD 2>/dev/null || echo '?')$(git -C "$ROOT" diff --quiet 2>/dev/null || echo -DIRTY)"

[ -x "$ROOT/scrip" ] || { echo "⛔ GATE REFUSES (2): no ./scrip to run -- nothing was measured [$stamp]"; exit 2; }

count_site() { awk -F'\t' -v s="$2" '$1 == s { n++ } END { print n + 0 }' "$1"; }

fails=0
plant_done=0
for w in $WITNESSES; do
  src="scripts/gc_witnesses/$w.icn"
  ref="scripts/gc_witnesses/$w.ref"
  [ -f "$src" ] || { echo "⛔ GATE REFUSES (2): witness $src is missing -- the population is empty, which is not a pass [$stamp]"; exit 2; }
  [ -f "$ref" ] || { echo "⛔ GATE REFUSES (2): $ref is missing, so arm A cannot grade the answer [$stamp]"; exit 2; }

  tr="$TMP/$w.tr"; out="$TMP/$w.out"
  rm -f "$tr"
  SCRIP_HEAP_MB="${SCRIP_HEAP_MB:-1}" SCRIP_C2BB_TRACE="$tr" timeout 120s "$ROOT/scrip" "$src" > "$out" 2>"$TMP/$w.err"
  rc=$?
  [ -f "$tr" ] || { echo "⛔ GATE REFUSES (2): $w produced NO trace file at all (rc=$rc) -- the build carries no rt_c2bb_hit or the run died before one [$stamp]"; exit 2; }

  if [ "${FAIL_ONCE:-0}" = 1 ] && [ "$plant_done" = 0 ]; then
    printf 'gen_h.enter\tPLANT_FAIL_ONCE\t?\t?\n' >> "$tr"
    plant_done=1
    echo "   FAIL_ONCE: planted one synthetic gen_h.enter record into $w's trace"
  fi

  coro=$(count_site "$tr" gen_h.coro)
  coro_res=$(count_site "$tr" gen_h.coro_resume)
  spine=$(( $(count_site "$tr" genp.spine) + $(count_site "$tr" genp.spine.n2) ))
  enter=$(count_site "$tr" gen_h.enter)
  rider=$(( $(count_site "$tr" gen_h.call_value) + $(count_site "$tr" gen_h.pl_goal) ))
  rider_res=$(count_site "$tr" gen_h.resume_frame)

  if [ "$coro" -eq 0 ]; then
    echo "⛔ GATE REFUSES (2): $w never took the generator arm (gen_h.coro=0), so it is INERT for this claim and its silence is not evidence [$stamp]"
    exit 2
  fi

  echo "-- $w  rc=$rc  coro=$coro coro_resume=$coro_res spine=$spine enter=$enter rider=$rider rider_resume=$rider_res"

  if [ "$rc" -ne 0 ] || ! diff -q "$out" "$ref" >/dev/null 2>&1; then
    echo "   ⛔ A: $w does not answer its .ref (rc=$rc) -- a trace claim over a wrong answer is worthless"
    diff "$ref" "$out" | head -6
    fails=$((fails + 1))
  fi
  if [ "$enter" -ne 0 ]; then
    echo "   ⛔ B: gen_h.enter fired $enter time(s) -- a rider DID reach the jmp_entry arm, which IS a C->BB entry from the caller's stack. This gate's whole claim is that arm stays untaken here; re-measure before believing any attribution built on it."
    fails=$((fails + 1))
  fi
  if [ "$coro" -ne "$spine" ]; then
    echo "   ⛔ C: gen_h.coro=$coro but sanctioned coroutine starts=$spine. One generator-arm call must start exactly one coroutine; an inequality means the arm reaches a box some other way and the sanction does not cover it."
    fails=$((fails + 1))
  fi
  if [ "$rider" -ne "$coro" ]; then
    echo "   ⛔ D: rider records=$rider but generator-arm calls=$coro -- $((rider - coro)) rider record(s) are unexplained by the coroutine road."
    fails=$((fails + 1))
  fi
  if [ "$rider_res" -ne "$coro_res" ]; then
    echo "   ⛔ E: gen_h.resume_frame=$rider_res but gen_h.coro_resume=$coro_res -- the resume rider reached something other than an existing coroutine."
    fails=$((fails + 1))
  fi
done

if [ "$fails" -gt 0 ]; then
  echo "⛔ GATE FAILED: $fails assertion(s) red [$stamp]"
  exit 1
fi
echo "✅ the generator arm of rt_proc_call_gen_h is a SANCTIONED coroutine start, not a C->BB box entry, and every rider record on the witness population is accounted for by it [$stamp]"
exit 0
