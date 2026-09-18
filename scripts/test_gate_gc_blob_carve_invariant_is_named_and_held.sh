#!/usr/bin/env bash
# test_gate_gc_blob_carve_invariant_is_named_and_held.sh -- the R-4(b) SNOBOL4 stored-pattern blob frame
# (emit.cpp, "R-4(b) BLOB ACTIVATION FRAME") is correct only while its carve is EXACTLY blob_frame_bytes().
# Row gc-the-blob-frame-carve-must-be-exactly-blob-frame-bytes-and-no-reader-of-emit-cpp-can-say-why, criterion
# written by the ceo under CEO-829 on the goal of CEO-824(2).
#
# WHY THIS GATE EXISTS. The cto built the rung-1 map cell for this frame regime by carving 16 more bytes and
# writing the cell at [rsp+0]. It read GREEN on every gc gate, both media, the seven smokes, gc2 and preflight,
# and it cost the SNOBOL4 master ONE entry -- user_function_eval_pos_replace_branch_1, SIGSEGV in mode 3 -- so it
# was withdrawn under CEO-589 (ARCH-GC-COMPILE-TIME-FRAME-MAPS.md 6.2b). MEASURED HERE 2026-09-17: the SNOBOL4
# SMOKE PASSES under the perturbation this gate plants. The only instrument in the tree that saw the defect was a
# ten-minute master over 1974 entries, one of which fails. This gate sees it in seconds.
#
# ⛔ THE INVARIANT, NAMED AS MEASURED AND NOT AS THEORISED (this is what the row was for):
#   THE CARVE IS NOT A SIZE, IT IS A DEPTH CONTRACT THE REGIME'S OWN CONTROL FLOW DEPENDS ON, AND NO READING OF
#   emit.cpp WILL SHOW YOU WHY. Growing it by 16, 32 or 48 bytes -- with every other byte of the emitted program
#   identical, the asm diff is the two `sub rsp` immediates and nothing else -- kills the witness in mode 3 with a
#   store through r12 = 0 inside an IR_MATCH_ASSIGN_COND, which is control arriving at a box whose registers the
#   regime never established. Growing it by 64 or more runs clean again, and mode 4 is unaffected at every size.
#   FOUR HYPOTHESES ARE RULED OUT BY ABLATION, so do not spend the day on them again:
#     (1) NOT absolute stack position -- an environment-size sweep (0, 8, 16, 24, 32, 64 bytes of env padding)
#         leaves the unpadded run green and the padded run red every time; it is deterministic, not a lottery.
#     (2) NOT a stale blob_frame_bytes() reader -- moving the pad INSIDE blob_frame_bytes(), so that every reader
#         (the prologue carve, sn4_choice_rbp_off, fence0_dyn_floor, the suspend arms) moves together, still dies.
#     (3) NOT bb_match_begin's retry_whack literal -- making `lea rsp, [rbp - 56 - op_frame_extra]` follow the pad
#         still dies.
#     (4) NOT (2) and (3) together -- both following the pad still dies.
#   WHAT IS STILL OPEN: which site bakes the depth. It is NOT one of the named formulas, so a seat that changes
#   this carve cannot clear it by reading the emitter; it must RUN THIS GATE, and then the SNOBOL4 master.
#   FOR THE GC ROW SPECIFICALLY: the withdrawn cure did two things and only one was fatal. Writing 16 bytes at
#   [rsp+0] of the UNCHANGED carve leaves the witness green; growing the carve is what kills it. A map cell for
#   this regime must therefore come out of the existing carve, never out of a bigger one -- which is the concrete
#   thing rung 2 needs to know before it types this interior.
#
# ARMS (all blocking): (1) NAMED -- this gate prints the invariant above, because a check whose reason lives only
# in a commit message is the state the row exists to end; (2) WITNESS -- user_function_eval_pos_replace_branch_1
# matches its ref in BOTH modes with no knob set; (3) PLANTED PERTURBATION -- SCRIP_BLOB_CARVE_PAD=1 and =2 must
# each KILL the witness in mode 3: a guard that has never been seen to fire is not known to hold (CEO-554), and if
# these ever come back green the coupling has moved and the invariant must be re-measured, not assumed gone;
# (4) THE PERTURBATION IS A CARVE PERTURBATION -- the emitted programs differ ONLY in `sub rsp` immediates, so arm
# 3 is grading the carve and not a code change.
"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
set -uo pipefail
G="$(basename "${BASH_SOURCE[0]}" .sh)"
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
S4E="${S4E_HOME:-$(cd "$ROOT/.." && pwd)}"
SCRIP="${SCRIP_BIN:-$ROOT/scrip}"; [ -x "$SCRIP" ] || { echo "⛔ GATE REFUSE(2) [$G]: no scrip at $SCRIP"; exit 2; }
CORPUS="$S4E/corpus/tests/snobol4"; [ -f "$CORPUS/ALL.sno" ] || { echo "⛔ GATE REFUSE(2) [$G]: no SNOBOL4 master at $CORPUS/ALL.sno -- the witness is cut from it"; exit 2; }
T=$(mktemp -d) || exit 2; trap 'rm -rf "$T"' EXIT
RC=0
export SNO_LIB="${SNO_LIB:-$S4E/corpus/include}"
echo "  INVARIANT: the R-4(b) blob carve is a DEPTH CONTRACT, not a size -- +16/+32/+48 bytes kills user_function_eval_pos_replace_branch_1 in mode 3 (store through r12=0 in an IR_MATCH_ASSIGN_COND) with the emitted program otherwise byte-identical; +64 and beyond run clean and mode 4 is unaffected; absolute stack position, every blob_frame_bytes() reader moving together, bb_match_begin's retry_whack literal, and both together are RULED OUT by ablation, so the site that bakes the depth is still unnamed and this gate is the only cheap instrument that sees it. A map cell for this frame must come out of the existing carve: writing at [rsp+0] is harmless, growing the carve is not."
python3 "$HERE/corpus_suite_harness.py" extract "$CORPUS/ALL.sno" "$CORPUS/ALL.ref" user_function_eval_pos_replace_branch_1 "$T/w.sno" --out-ref "$T/w.ref" >/dev/null 2>&1 || { echo "⛔ GATE REFUSE(2) [$G]: could not extract the witness entry from the master"; exit 2; }
timeout 60 "$SCRIP" "$T/w.sno" </dev/null > "$T/m3.out" 2>/dev/null; m3rc=$?
"$SCRIP" --compile "$T/w.sno" </dev/null > "$T/base.s" 2>/dev/null && gcc "$T/base.s" -o "$T/w.x" -L"$ROOT/out" -lscrip_rt -Wl,-rpath,"$ROOT/out" -lm -lpthread 2>/dev/null
if [ -x "$T/w.x" ]; then timeout 60 "$T/w.x" </dev/null > "$T/m4.out" 2>/dev/null; m4rc=$?; else m4rc=127; fi
if [ "$m3rc" = 0 ] && [ "$m4rc" = 0 ] && diff -q "$T/m3.out" "$T/w.ref" >/dev/null 2>&1 && diff -q "$T/m4.out" "$T/w.ref" >/dev/null 2>&1; then
    echo "  witness PASS (user_function_eval_pos_replace_branch_1 matches its ref in both modes: $(tr -d '\n' < "$T/w.ref"))"
else echo "  witness FAIL (m3 rc=$m3rc \"$(tail -1 "$T/m3.out" 2>/dev/null)\", m4 rc=$m4rc \"$(tail -1 "$T/m4.out" 2>/dev/null)\", ref \"$(cat "$T/w.ref")\" -- the entry the withdrawn carve cure cost is red again)"; RC=1; fi
planted_red=0; planted_seen=""
for p in 1 2; do
    prc=$(bash -c 'SCRIP_BLOB_CARVE_PAD=$0 timeout 60 "$1" "$2" </dev/null >"$3" 2>/dev/null; echo $?' "$p" "$SCRIP" "$T/w.sno" "$T/p$p.out" 2>/dev/null)
    planted_seen="$planted_seen pad=$p:rc=$prc"
    [ "$prc" != 0 ] && planted_red=$((planted_red + 1))
done
if [ "$planted_red" = 2 ]; then echo "  planted PASS (a perturbed carve trips the coupling in mode 3 --$planted_seen -- the guard has been SEEN to fire, it is not an inert seam)"
else echo "  planted FAIL (a carve perturbed by 16 and by 32 bytes no longer kills the witness --$planted_seen. This is NOT good news: the coupling has moved, and the invariant above must be RE-MEASURED before anyone trusts it)"; RC=1; fi
SCRIP_BLOB_CARVE_PAD=1 "$SCRIP" --compile "$T/w.sno" </dev/null > "$T/pad.s" 2>/dev/null
d_all=$(diff "$T/base.s" "$T/pad.s" | grep -c '^[<>]'); d_other=$(diff "$T/base.s" "$T/pad.s" | grep '^[<>]' | grep -vc 'sub  *rsp')
if [ "$d_all" -gt 0 ] && [ "$d_other" = 0 ]; then echo "  carve-only PASS (the perturbed program differs from the base in $d_all line(s), every one of them a sub rsp immediate -- arm 3 grades the carve, not a code change)"
else echo "  carve-only FAIL (base vs perturbed differ in $d_all line(s) of which $d_other are not sub rsp -- the knob is changing more than the carve and arm 3 no longer isolates it)"; RC=1; fi
if [ "$RC" = 0 ]; then echo "GATE PASS(0) [$G]: the blob-carve depth contract is named here, a perturbed carve still trips it, and the entry the withdrawn cure cost is green in both modes (examined 4 arms)"
else echo "GATE FAIL(1) [$G]: the blob carve's contract is broken, or the check that holds it stopped discriminating (examined 4 arms)"; fi
echo "    tree: SCRIP=$(git -C "$ROOT" rev-parse --short HEAD 2>/dev/null)$(git -C "$ROOT" diff --quiet 2>/dev/null || echo -DIRTY)  measured $(date -u +%Y-%m-%dT%H:%MZ)"
exit $RC
