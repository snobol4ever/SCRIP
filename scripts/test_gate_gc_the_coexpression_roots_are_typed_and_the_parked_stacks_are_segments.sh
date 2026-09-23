#!/usr/bin/env bash
# test_gate_gc_the_coexpression_roots_are_typed_and_the_parked_stacks_are_segments.sh -- row
# gc-the-coexpression-stacks-take-typed-visits-from-their-structs-and-the-seven-gc-zeta-frame-calls-in-gc-coexpr-roots-go
# (cto, 2026-09-19, MODE TRIO; ARCH-GC-COMPILE-TIME-FRAME-MAPS.md section 6.2f; law RULES.md FACT RULE THE COLLECTOR
# GUESSES NOTHING, CEO-812; Lon 2026-09-19 in-chat: "r13 for SNOBOL4 and Icon points to the subject string, &subject").
#
# WHAT WAS THERE. gc_coexpr_roots swept SEVEN regions word by word with gc_zeta_frame: the parked main thread's whole
# [stack] mapping, the root context's gc_spill[6] and xmit[2], every linked context's gc_spill[6], xmit[2] and
# entry_arg+8..56, and every parked co-expression's whole 8 MB thread stack (which rt_coexpr.c ALSO registered as a
# GC root range, so it was swept twice). MEASURED FROM THE CODE, NOT FROM THE BATON'S READING: gc_spill was not a
# DESCR array -- scrip_coswitch's inline asm stored rbx, rbp, r12, r13, r14, r15, the raw callee-saved register file
# at the switch -- and NOTHING ever reloaded a register from it, so the collector's rewrite of those six words never
# reached a register: a conservative visit that could keep a dead block alive and could not keep a live pointer
# right. xmit IS one DESCR (bb_activate stages rsi:rdx, bb_coret rdi:rsi, the runtime writes exactly those words).
# entry_arg is TWO records: the Icon create package (r12,r13,r14,r15,rbx,csav5 at +8..+56) and rt.c's generator-
# thread record rt_genp_s (rbx,r12,r13,r14,r15 at +8..+48, co.thread at +48).
#
# WHAT IS HERE. (1) xmit is a DESCR_t field visited by rt_gc_visit_descr -- typed by its own tag. (2) gc_spill is
# DELETED; scrip_coswitch records park_sp (the parked thread's rsp) instead. (3) each record is tagged sigma_live at
# creation (scan_depth > 0: the creator was inside a string scan, so the marshalled r13 IS a heap subject pointer)
# and only then is its r13 word visited raw -- the package's r13 (rt_coexpr.c owns the struct) and the generator
# record's word at +24 (pinned by rt.c's own _Static_assert offsetof(regs)==8 and its `movq 24(%rdi), %r13`; arm 7
# reads both lines because rt.c is another seat's file). (4) the parked stacks are SEGMENTS of the same population
# as the running stack: one iterator hands every segment (parked main, each parked co-expression, then the running
# thread from the poll's floor) to the ONE remaining stack-walker call, reported under pop=parked; the walker row
# replaces that call by the mapped walk. gc_zeta_frame_calls 11 -> 4 (the registered ranges, the segments, the
# pz seam, the heap interior); the rt_coexpr.c stack root range is gone. (5) THE SWITCH RECORD (bb_activate,
# bb_coret): r13 is spilled as ONE tagged DESCR cell {DT_S, slen=r15d, s=r13} on the thread's own spine before the
# switch and reloaded after it, because a collection on the OTHER thread cannot rewrite a parked thread's register.
#
# THE ACCIDENT THIS GATE NAMES (measured with gdb and objdump on 2026-09-19): the subject block of hb_coexpr_sigma
# moved 0x7ffe6d61b200 -> 0x7ffe6d61b1b0 under SCRIP_GC_STRESS=8 and r13 FOLLOWED it, although no frame of ours saves
# r13 on the switch path (scrip_coswitch, scrip_coexpr_activate, scrip_coret push rbp only). glibc's sem_wait slow
# path (libc.so.6 +0xa5000) pushes rbp, r13, r12, rbx: the parked thread's r13 lived in a libc stack word, the
# whole-stack sweep rewrote it, and `pop %r13` on wake restored the relocated pointer. So the register survived a
# collection across a co-expression switch ONLY through glibc's register allocation plus a sweep of libc's frames.
# The switch record replaces the accident with the design (section 6.5: registers are made visible as tagged cells).
#
# THE INSTRUMENT THAT MAKES THE FAIL-ONCES DETERMINISTIC: SCRIP_GC_PLANT_SHIFT=N forwards EVERY live block N bytes up
# at EVERY collection over a kept fill at the arena start (two-pass slide: down-movers ascending, up-movers
# descending), so a stale copy of any heap address is wrong after every collection. Without it a conservative
# regime keeps dead blocks alive through stale frame slots and the subject rarely slides (six witness shapes, one
# 80-byte slide in one cadence). Every arm below runs under it. It is a plant, default off, no effect when unset.
#
# ARMS. (1) census + structure: gc_zeta_frame_calls <= 4, gc_spill absent, no stack root range in rt_coexpr.c.
# (2) the three witnesses match their iconx-cut refs at SCRIP_GC_STRESS=1,3,5,8 under the shift plant in BOTH modes
# (band printed). (3) EMISSION: every `call scrip_coexpr_activate` and `call scrip_coret` in the sigma witness's
# mode-4 asm carries the record idiom before it and the r13 reload after it, and SCRIP_GC_SWITCH_RECORD_PLANT=1
# removes it (the plant is seen). (4) THE RECORD IS LOAD-BEARING: with the parked sweep bounded at park_sp
# (SCRIP_GC_COEXPR_PLANT=2 -- glibc's words excluded, the accident switched off) the sigma witness matches its ref
# WITH the record and NOT without it, both modes. (5) THE TYPED RECORD VISIT IS LOAD-BEARING: hb_coexpr_create
# matches its ref and fails when SCRIP_GC_COEXPR_PLANT=1 skips the record visits, both modes. (6) the reporter
# prints [GC-COEXPR] with sigma>=1 on the create witness (its package is tagged) and sigma=0 on the sigma witness
# (created outside any scan) -- the tag discriminates. (7) the cross-file pin for the generator record's r13 word.
#
# MEASURED AND RECORDED, NOT HIDDEN. (a) The parked segments are still swept over their WHOLE mappings (8 MB each;
# the reporter read 617 MB over 98 ranges on the sigma witness) because every runtime call that can park a thread
# (the by-name generator call, rt_call_value_gen_h -> rt_proc_call_gen_h) still relies on glibc's words for its
# caller's r13; the bound at park_sp is the polls row's step once every parking site carries a record -- Lon
# 2026-09-19: do not let optimization stand in the way of a working GC. (b) Arizona transmit.icn and coexpr.icn lose
# their &errout trace output under SCRIP_GC_STRESS=5 on origin acbb36b00 (12 -> 2 and 18 -> 0 lines, rc 0), the
# same on a clean base build and on this tree -- pre-existing, handed to the ceo's Icon seat.
"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
set -uo pipefail
G="$(basename "${BASH_SOURCE[0]}" .sh)"
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP_BIN:-$ROOT/scrip}"; [ -x "$SCRIP" ] || { echo "⛔ GATE REFUSE(2) [$G]: no scrip at $SCRIP"; exit 2; }
T=$(mktemp -d) || exit 2; trap 'rm -rf "$T"' EXIT
RC=0
WD="$ROOT/scripts/gc_witnesses"
for w in parked sigma create; do for x in icn ref; do [ -f "$WD/hb_coexpr_$w.$x" ] || { echo "⛔ GATE REFUSE(2) [$G]: missing $WD/hb_coexpr_$w.$x"; exit 2; }; done; done
[ -f "$ROOT/src/runtime/rt/rt.c" ] || { echo "⛔ GATE REFUSE(2) [$G]: rt.c missing"; exit 2; }
export SHIFT=4096
m4build() { "$SCRIP" --compile "$1" > "$2.s" 2>/dev/null && gcc "$2.s" -o "$2" -L"$ROOT/out" -lscrip_rt -lm -lpthread -Wl,-rpath,"$ROOT/out" 2>/dev/null; }
run3() { local st="$1"; shift; ( cd "$T" && env SCRIP_GC_PLANT_SHIFT="$SHIFT" SCRIP_GC_STRESS="$st" "$@" timeout 120 "$SCRIP" "$WIT" 2>/dev/null </dev/null | tr -d '\0' ); }
run4() { local st="$1" bin="$2"; shift 2; ( cd "$T" && env SCRIP_GC_PLANT_SHIFT="$SHIFT" SCRIP_GC_STRESS="$st" "$@" timeout 120 "$bin" 2>/dev/null </dev/null | tr -d '\0' ); }
echo "  HOLDS: the seven coexpression sweeps are typed visits or one segment walk (gc_zeta_frame_calls <= 4); the transmitted value is a DESCR visited by tag; a record created inside a scan has its subject word visited; the two switch boxes carry the tagged r13 record; every fail-once is run live under SCRIP_GC_PLANT_SHIFT=$SHIFT, where every block moves at every collection"
n=$(python3 "$HERE/util_gc_census.py" conservative 2>/dev/null | grep -oE 'gc_zeta_frame_calls=[0-9]+' | cut -d= -f2)
spill=$(grep -rl 'gc_spill' "$ROOT/src" 2>/dev/null | wc -l)
rng=$(grep -c 'rt_gc_root_range_add((const char \*)new_ctx->stk_lo' "$ROOT/src/runtime/rt/rt_coexpr.c")
if [ -n "$n" ] && [ "$n" -le 4 ] && [ "$spill" = 0 ] && [ "$rng" = 0 ]; then echo "  arm 1 PASS: gc_zeta_frame_calls=$n (11 at acbb36b00), gc_spill in 0 files, the coexpression stack is no longer a registered root range"
else echo "  arm 1 FAIL: gc_zeta_frame_calls=${n:-unread} gc_spill_files=$spill stack_root_range_lines=$rng"; RC=1; fi
band=""; bad=0
for w in parked sigma create; do WIT="$WD/hb_coexpr_$w.icn"; ref="$(cat "$WD/hb_coexpr_$w.ref")"
    m4build "$WIT" "$T/$w" || { echo "  arm 2 FAIL: $w does not build in mode 4"; RC=1; bad=1; continue; }
    for st in 1 3 5 8; do o3="$(run3 "$st")"; o4="$(run4 "$st" "$T/$w")"
        if [ "$o3" = "$ref" ] && [ "$o4" = "$ref" ]; then band="$band $w@$st:ok"; else band="$band $w@$st:m3=$([ "$o3" = "$ref" ] && echo ok || echo RED),m4=$([ "$o4" = "$ref" ] && echo ok || echo RED)"; bad=1; fi
    done
done
if [ "$bad" = 0 ]; then echo "  arm 2 PASS: three witnesses match their iconx refs at SCRIP_GC_STRESS=1,3,5,8 in both modes under the shift plant --$band"
else echo "  arm 2 FAIL: a witness lost a root under forced motion --$band"; RC=1; fi
rec_ok=0; rec_bad=0
while IFS= read -r ln; do
    pre="$(sed -n "$((ln-8)),$((ln-1))p" "$T/sigma.s")"; post="$(sed -n "$((ln+1)),$((ln+8))p" "$T/sigma.s")"
    if echo "$pre" | grep -q 'mov  *dword ptr \[rsp + 0\], 2' && echo "$pre" | grep -q 'mov  *dword ptr \[rsp + 4\], r15d' && echo "$pre" | grep -q 'mov  *qword ptr \[rsp + 8\], r13' && echo "$post" | grep -q 'mov  *r13, qword ptr \[rsp + 8\]'; then rec_ok=$((rec_ok+1)); else rec_bad=$((rec_bad+1)); fi
done < <(grep -nE 'call +(scrip_coexpr_activate|scrip_coret)@PLT' "$T/sigma.s" | cut -d: -f1)
SCRIP_GC_SWITCH_RECORD_PLANT=1 "$SCRIP" --compile "$WD/hb_coexpr_sigma.icn" > "$T/sigma_np.s" 2>/dev/null
# ⛔ THE PLANTED COUNT IS TAKEN AT THE SWITCH SITES ONLY (cto 2026-09-23, CTO-149): a file-wide grep for the r13 spill counted
# the sigma-recording safe-point polls (x86_rt_gc_poll_rec_sigma spills {DT_S, r15d, r13} at [rsp+0..8] before rt_gc_point_arr_c)
# as switch records -- 12 of them on this witness once the match templates polled -- and this arm read FAIL on a tree where the
# plant removed every switch record. The record is a property of the switch CALL, so it is read in the same eight-line window
# before scrip_coexpr_activate/scrip_coret that arm 3's own rec_ok reading uses; a poll's spill is not a switch record.
np=0
while IFS= read -r ln; do
    pre="$(sed -n "$((ln-8)),$((ln-1))p" "$T/sigma_np.s")"
    if echo "$pre" | grep -q 'mov  *qword ptr \[rsp + 8\], r13'; then np=$((np+1)); fi
done < <(grep -nE 'call +(scrip_coexpr_activate|scrip_coret)@PLT' "$T/sigma_np.s" | cut -d: -f1)
if [ "$rec_ok" -gt 0 ] && [ "$rec_bad" = 0 ] && [ "$np" = 0 ]; then echo "  arm 3 PASS: $rec_ok switch site(s) carry the tagged r13 record before the call and the reload after it; SCRIP_GC_SWITCH_RECORD_PLANT=1 removes it ($np records)"
else echo "  arm 3 FAIL: sites with record=$rec_ok without=$rec_bad, planted emission still carries $np record(s)"; RC=1; fi
WIT="$WD/hb_coexpr_sigma.icn"; ref="$(cat "$WD/hb_coexpr_sigma.ref")"
SCRIP_GC_SWITCH_RECORD_PLANT=1 m4build "$WIT" "$T/sigma_np" || { echo "  arm 4 FAIL: planted mode-4 build failed"; RC=1; }
a4=""; a4bad=0
for st in 1 3 5 8; do
    w3="$(run3 "$st" SCRIP_GC_COEXPR_PLANT=2)"; w4="$(run4 "$st" "$T/sigma" SCRIP_GC_COEXPR_PLANT=2)"
    n3="$(run3 "$st" SCRIP_GC_COEXPR_PLANT=2 SCRIP_GC_SWITCH_RECORD_PLANT=1)"; n4="$(run4 "$st" "$T/sigma_np" SCRIP_GC_COEXPR_PLANT=2)"
    if [ "$w3" = "$ref" ] && [ "$w4" = "$ref" ] && [ "$n3" != "$ref" ] && [ "$n4" != "$ref" ]; then a4="$a4 @$st:record=ok,norecord=red"; else a4="$a4 @$st:record=$([ "$w3" = "$ref" ] && echo ok || echo RED)/$([ "$w4" = "$ref" ] && echo ok || echo RED),norecord=$([ "$n3" != "$ref" ] && echo red || echo GREEN)/$([ "$n4" != "$ref" ] && echo red || echo GREEN)"; a4bad=1; fi
done
if [ "$a4bad" = 0 ]; then echo "  arm 4 PASS: with the parked sweep bounded at park_sp (glibc's words excluded) the subject survives the switch WITH the record and is lost WITHOUT it, both modes --$a4"
else echo "  arm 4 FAIL: the switch record is not what keeps the subject right --$a4"; RC=1; fi
WIT="$WD/hb_coexpr_create.icn"; ref="$(cat "$WD/hb_coexpr_create.ref")"
a5=""; a5bad=0
for st in 1 3 5 8; do
    t3="$(run3 "$st")"; t4="$(run4 "$st" "$T/create")"; p3="$(run3 "$st" SCRIP_GC_COEXPR_PLANT=1)"; p4="$(run4 "$st" "$T/create" SCRIP_GC_COEXPR_PLANT=1)"
    if [ "$t3" = "$ref" ] && [ "$t4" = "$ref" ] && [ "$p3" != "$ref" ] && [ "$p4" != "$ref" ]; then a5="$a5 @$st:typed=ok,planted=red"; else a5="$a5 @$st:typed=$([ "$t3" = "$ref" ] && echo ok || echo RED)/$([ "$t4" = "$ref" ] && echo ok || echo RED),planted=$([ "$p3" != "$ref" ] && echo red || echo GREEN)/$([ "$p4" != "$ref" ] && echo red || echo GREEN)"; a5bad=1; fi
done
if [ "$a5bad" = 0 ]; then echo "  arm 5 PASS: the inherited subject in a create package survives forced motion through the typed record visit and is lost when SCRIP_GC_COEXPR_PLANT=1 skips it, both modes --$a5"
else echo "  arm 5 FAIL: the typed record visit is not load-bearing --$a5"; RC=1; fi
rc6=$( ( cd "$T" && SCRIP_GC_MAPS=1 SCRIP_GC_PLANT_SHIFT=$SHIFT SCRIP_GC_STRESS=3 timeout 120 "$SCRIP" "$WD/hb_coexpr_create.icn" 2>&1 >/dev/null </dev/null ) | grep '^\[GC-COEXPR\]' | grep -oE 'sigma=[0-9]+' | sort -u | tr '\n' ' ')
rs6=$( ( cd "$T" && SCRIP_GC_MAPS=1 SCRIP_GC_PLANT_SHIFT=$SHIFT SCRIP_GC_STRESS=3 timeout 120 "$SCRIP" "$WD/hb_coexpr_sigma.icn" 2>&1 >/dev/null </dev/null ) | grep -E '^\[GC-COEXPR\]|pop=parked' | grep -oE 'sigma=[0-9]+|pop=parked' | sort -u | tr '\n' ' ')
if echo "$rc6" | grep -qE 'sigma=[1-9]' && echo "$rs6" | grep -q 'sigma=0' && echo "$rs6" | grep -q 'pop=parked'; then echo "  arm 6 PASS: the reporter tags the create-inside-scan package (create: $rc6) and not the scan-free contexts (sigma: $rs6)"
else echo "  arm 6 FAIL: reporter create=[$rc6] sigma=[$rs6]"; RC=1; fi
p1=$(grep -c 'offsetof(rt_genp_s, regs) == 8' "$ROOT/src/runtime/rt/rt.c"); p2=$(grep -c 'movq 24(%rdi), %r13' "$ROOT/src/runtime/rt/rt.c"); p3=$(grep -c 'c->entry_arg + 24' "$ROOT/src/runtime/rt/rt_coexpr.c")
if [ "$p1" -ge 1 ] && [ "$p2" -ge 1 ] && [ "$p3" -ge 1 ]; then echo "  arm 7 PASS: the generator record's r13 word is +24 in rt.c (static assert on regs at +8, the entry asm loads r13 from 24(%rdi)) and rt_coexpr.c visits exactly that word"
else echo "  arm 7 FAIL: cross-file pin broken (assert=$p1 asm=$p2 visitor=$p3)"; RC=1; fi
pl=$( ( cd "$T" && env SCRIP_GC_PLANT_SHIFT="$SHIFT" SCRIP_GC_STRESS=3 timeout 120 "$SCRIP" "$WD/hb_coexpr_create.icn" 2>&1 >/dev/null </dev/null ) | grep -c "^\[GC-SHIFT\] plant:" ); pl=${pl:-0}
if [ "$pl" -ge 1 ]; then echo "  arm 8 PASS: the forced motion this gate grades under is REAL -- the shift plant announced $pl application(s) at SCRIP_GC_PLANT_SHIFT=$SHIFT with nothing asked for, so arms 2-6 moved every block rather than relying on ordinary compaction"
else echo "⛔ GATE REFUSE(2) [$G]: the shift plant applied ZERO times at SCRIP_GC_PLANT_SHIFT=$SHIFT and SCRIP_HEAP_MB=${SCRIP_HEAP_MB:-default} -- gc_plant_shift_bytes() declines whenever arena headroom is not strictly greater than the shift, so EVERY arm here graded ordinary compaction while claiming forced relocation. Nothing is graded and nothing is red: this is a configuration statement (cto 2026-09-21, held by test_gate_gc_the_plant_says_whether_it_applied)"; exit 2; fi
if [ "$RC" = 0 ]; then echo "GATE PASS(0) [$G]: the coexpression roots are typed, the parked stacks are one segment population, the switch record and the record visit are each load-bearing under forced motion (examined 8 arms, three with planted violations)"
else echo "GATE FAIL(1) [$G]: the coexpression roots are not typed or a plant went unseen (examined 8 arms)"; fi
echo "    tree: SCRIP=$(git -C "$ROOT" rev-parse --short HEAD 2>/dev/null)$(git -C "$ROOT" diff --quiet 2>/dev/null || echo -DIRTY)  measured $(date -u +%Y-%m-%dT%H:%MZ)"
exit $RC
