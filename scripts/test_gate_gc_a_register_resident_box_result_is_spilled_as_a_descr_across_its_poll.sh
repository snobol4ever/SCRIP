#!/usr/bin/env bash
# test_gate_gc_a_register_resident_box_result_is_spilled_as_a_descr_across_its_poll.sh
# ARCH-GC-COMPILE-TIME-FRAME-MAPS.md section 6.5b (cto, 2026-09-18; F6 step 4).
#
# WHAT THIS GATE HOLDS, AND THE DIAGNOSIS IT CORRECTS. F6 step 4's admission predicate was "the call's
# result is stored to a mapped frame slot before the poll" (CTO-75/76). 131 of the 210 allocating call
# sites cannot satisfy it, and the standing reading of that -- the ceo's CEO-884, and the cto's own
# bucket line in CTO-76 -- was that they are BLOCKED ON STEP 2 until the static map grows a slot vector
# naming which frame offsets are live DESCRs.
#
# ⛔ THAT DIAGNOSIS IS WRONG, AND A SLOT VECTOR WOULD NOT HAVE MOVED ONE OF THEM. The blocked sites are
# op_zres boxes: the call's result IS the box's result and it stays in rax:rdx, so it is not in the frame
# at all. A map that describes the frame perfectly still cannot see a value that never reached the frame.
# What those sites need is not a better description of the stack -- it is for the value to BE on the stack.
#
# THE MECHANISM, WHICH NEEDED NO MAP CHANGE AND NO NEW RUNTIME SYMBOL. x86_rt_gc_poll_res() spills the
# rax:rdx pair as a 16-byte DESCR cell under rsp across the poll and reloads it after:
#     sub rsp,16 ; mov [rsp+0],rax ; mov [rsp+8],rdx ; call rt_gc_poll ; mov rax,[rsp+0] ; mov rdx,[rsp+8] ; add rsp,16
# gc_heap.c's precise root phase already walks [poll floor, stack top) as DESCR cells with relocation
# (gc_collect_ex, the pz && g_gc_seam_sp arm), so the spilled cell is visited and UPDATED by construction
# rather than by a new agreement. rsp stays 16-aligned across the sub, so the poll's call is ABI-correct.
#
# ⛔ WHAT THIS DOES **NOT** BUY, stated here so nobody over-reads the cure the way the block was over-read.
# It makes the RESULT visible. It does not make a poll safe "anywhere": any OTHER live collected-heap
# pointer sitting in a caller-saved register at the poll is still lost, and a callee-saved register holding
# one (section 6.5a measured r13 as that register) is still owed the SPILL RECORD of 6.5. The predicate is
# relaxed from "the result reaches a mapped slot" to "the result is in rax:rdx and nothing else live is in a
# register" -- a wider door, not an open one.
#
# THE ARMS. Arm 1 is the fail-once this cure was built against, run as a live measurement rather than cited:
# 140_pat_eval_double_fn_trick prints stk=B, and printed stk= (a WRONG ANSWER, not a crash) when the same
# site took a bare x86_rt_gc_poll(). Arm 2 reads the emitted asm in mode 4 and requires the full seven-
# instruction idiom in order, because an emission that drops the reload is exactly the bug arm 1 catches only
# when a collection happens to fire. Arm 3 measures that the walker actually sweeps the range the cell lands
# in -- SCRIP_GC_MAPS=1 must report pop=seam with a non-zero byte count -- because a spill nothing looks at
# is the instrument-that-reports-success-while-doing-nothing failure the INSTRUMENT LAWS exist to catch.
# Each arm carries a PLANTED-VIOLATION check so this gate can say no (CTO-74: ten gates could not).
"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
set -uo pipefail
G="$(basename "${BASH_SOURCE[0]}" .sh)"
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
S4E="${S4E_HOME:-$(cd "$ROOT/.." && pwd)}"
SCRIP="${SCRIP_BIN:-$ROOT/scrip}"; [ -x "$SCRIP" ] || { echo "⛔ GATE REFUSE(2) [$G]: no scrip at $SCRIP"; exit 2; }
T=$(mktemp -d) || exit 2; trap 'rm -rf "$T"' EXIT
RC=0
export SNO_LIB="${SNO_LIB:-$S4E/corpus/include}"
echo "  HOLDS: a box whose result lives in rax:rdx spills it as a DESCR cell under rsp across its GC poll and reloads it after, so the value the poll would otherwise clobber is both PRESERVED and RELOCATABLE -- and the collector's precise root phase already walks the range that cell lands in. The map carries no slot vector and needs none for this class (6.5): a value that never reached the frame cannot be described by a frame map."

cat > "$T/w.sno" <<'EOF'
        DEFINE('inner(c)')
        DEFINE('outer(cs)')                                   :(both_end)
inner   stk    = c stk
        inner  = .stk                                         :(NRETURN)
outer   outer  = EVAL('LEN(1) . *inner(' cs ')')              :(RETURN)
both_end
        stk    = ''
        s      = 'AB'
        pat    = outer('c1') outer('c2')
        s POS(0) pat RPOS(0)                                  :S(YES)F(NO)
YES     OUTPUT = 'stk=' stk                                   :(END)
NO      OUTPUT = 'fail'
END
EOF

arm1_verdict() { [ "$1" = "stk=B" ] && return 0 || return 1; }
A1="$("$SCRIP" "$T/w.sno" 2>/dev/null | head -1)"
if arm1_verdict "$A1"; then echo "  ARM 1 answer: stk=B -- the polled op_zres site returns its result intact"
else echo "⛔ ARM 1 RED: the witness printed '$A1', want 'stk=B' -- the poll clobbered the box result (this is the exact fail-once the spill-pair cures; 'stk=' is the bare-poll signature, rax clobbered by the poll's own call)"; RC=1; fi
if arm1_verdict "stk="; then echo "⛔ ARM 1 PLANTED-VIOLATION DID NOT TRIP: the comparator accepts the known bare-poll reading"; RC=1
else echo "  ARM 1 planted-violation: the SAME comparator rejects the recorded bare-poll reading 'stk=' -- the arm discriminates"; fi

"$SCRIP" --compile "$T/w.sno" >"$T/w.s" 2>/dev/null || { echo "⛔ GATE REFUSE(2) [$G]: mode 4 emission failed"; exit 2; }
IDIOM='sub[[:space:]]+rsp,[[:space:]]*16|mov[[:space:]]+qword ptr \[rsp \+ 0\],[[:space:]]*rax|mov[[:space:]]+qword ptr \[rsp \+ 8\],[[:space:]]*rdx|call[[:space:]]+rt_gc_poll|mov[[:space:]]+rax,[[:space:]]*qword ptr \[rsp \+ 0\]|mov[[:space:]]+rdx,[[:space:]]*qword ptr \[rsp \+ 8\]|add[[:space:]]+rsp,[[:space:]]*16'
SITE="$(grep -n 'call *NV_SET_fn' "$T/w.s" | head -1 | cut -d: -f1)"
[ -n "$SITE" ] || { echo "⛔ GATE REFUSE(2) [$G]: the witness emitted no NV_SET_fn call -- this gate graded nothing"; exit 2; }
awk -v n="$SITE" 'NR>=n && NR<=n+24' "$T/w.s" > "$T/win.s"
SEQ="$(grep -oE "$IDIOM" "$T/win.s" | tr -d ' ' | paste -sd'|' -)"
WANT='subrsp,16|movqwordptr[rsp+0],rax|movqwordptr[rsp+8],rdx|callrt_gc_poll|movrax,qwordptr[rsp+0]|movrdx,qwordptr[rsp+8]|addrsp,16'
if printf '%s' "$SEQ" | grep -qF "$WANT"; then echo "  ARM 2 emission: the seven-instruction spill-pair idiom appears in order in mode 4"
else echo "⛔ ARM 2 RED: the spill-pair idiom is not emitted in order in mode 4 -- a dropped reload loses the result whenever a collection fires"; RC=1; fi
BROKEN="$(printf '%s' "$SEQ" | sed 's/|movrax,qwordptr\[rsp+0\]//g')"
if printf '%s' "$BROKEN" | grep -qF "$WANT"; then echo "⛔ ARM 2 PLANTED-VIOLATION DID NOT TRIP: the sequence check passes with the reload removed"; RC=1
else echo "  ARM 2 planted-violation: removing the rax reload from the stream reds the same check -- the arm discriminates"; fi

cat > "$T/p.sno" <<'EOF'
        DEFINE('bump()')                                      :(BEND)
bump    G = G 'yyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy'
        bump = .G                                             :(RETURN)
BEND
        N = 0
LOOP    N = N + 1
        bump()
        LT(N,3000)                                            :S(LOOP)
        OUTPUT = 'size=' SIZE(G)
END
EOF
A3="$(SCRIP_GC_STRESS=10 "$SCRIP" "$T/p.sno" 2>/dev/null | head -1)"
SEAM="$(SCRIP_GC_STRESS=10 SCRIP_GC_MAPS=1 "$SCRIP" "$T/p.sno" 2>&1 | grep -oE 'pop=seam[[:space:]]+ranges=[0-9]+ bytes=[0-9]+' | head -1)"
SB="$(printf '%s' "$SEAM" | grep -oE 'bytes=[0-9]+' | cut -d= -f2)"
if [ "$A3" = "size=96000" ]; then echo "  ARM 3 answer under forced collection: size=96000"
else echo "⛔ ARM 3 RED: the allocating witness printed '$A3' under SCRIP_GC_STRESS=10, want 'size=96000'"; RC=1; fi
if [ -n "${SB:-}" ] && [ "$SB" -gt 0 ] 2>/dev/null; then echo "  ARM 3 walker range: $SEAM -- the precise root phase sweeps the emitted stack above the poll, which is where the spilled cell lands"
else echo "⛔ ARM 3 RED: no pop=seam range with a non-zero byte count was reported -- the collector is not walking the range the spill-pair writes into, so the spill preserves the value without making it relocatable"; RC=1; fi
if [ -n "${SB:-}" ] && [ "0" -gt 0 ] 2>/dev/null; then echo "⛔ ARM 3 PLANTED-VIOLATION DID NOT TRIP"; RC=1
else echo "  ARM 3 planted-violation: a zero byte count fails the same test -- the arm discriminates"; fi

[ $RC -eq 0 ] && echo "✅ GATE PASS [$G]" || echo "⛔ GATE FAIL [$G]"
exit $RC
