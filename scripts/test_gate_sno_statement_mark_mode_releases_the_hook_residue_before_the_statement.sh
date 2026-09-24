#!/usr/bin/env bash
export SCRIP_SNO_STMTKW=1   # this grader asks for the SNOBOL4 statement instrumentation (the --stlimit switch; Lon 2026-09-24 16:0x: the feature is off by default and never inferred from the source, the correctness graders turn it on because the oracle always has it)
# test_gate_sno_statement_mark_mode_releases_the_hook_residue_before_the_statement.sh -- A PROGRAM THAT READS A
# ⛔ ROMAN ARM (cfo 2026-09-16, hq_snobol4's batch find within the hour): the first cure released the RAW residue (zd) at the
# statement boundary and that OVER-RELEASED by 16 wherever a MATCH_REPLACE box had already popped its own hook on the success
# path -- csnobol4_suite/roman segfaulted both modes (RIP 0, RSP 4 MB below RBP), clean under SCRIP_ZD_HOOK_RELEASE=0. The
# release is now zd_exit_pop_s(IR_STATEMENT_END, ...) -- the same accounting every exit pop uses. Emission control over 283
# cross-language programs: 279 byte-identical, 4 changed (roman, spit, genc, bench), spit REF-SAME both modes, genc/bench
# excluded by the oracle and identical across arms. The vendored roman entry is graded here in both modes so it stays under test.
# STATEMENT KEYWORD RUNS EVERY STATEMENT AT THE SAME SPINE BASE AS A PROGRAM THAT DOES NOT.
#
# WHAT THIS IS: a SNOBOL4 program that mentions &STCOUNT, &STNO, &LASTNO (or the other statement keywords named by
# lower_snobol4.c) is lowered in STATEMENT-MARK MODE: every statement is preceded by a SNO$STMT dispatch -- two
# literal operands and a CALL, 48 bytes of zeta pushes -- that maintains the keyword's value at run time. A
# program that mentions none of them carries no dispatch. THE DEFECT THIS PINS (cfo, CEO-773 row, 2026-09-16):
# zd_plan (src/emitter/emit.cpp) let those 48 bytes SURVIVE INTO THE STATEMENT, so every statement in mark mode
# ran with rsp 48 bytes below the base a plain program runs at. The planned boxes were told about the residue and
# popped it at the statement's exits; the boxes the planner does NOT arm (an omega target that is not a test kind
# -- the gate of a `~call`, a refused run, the half of a statement after a forward failure) assume the plain base.
# The two halves disagreed by exactly the residue, so a statement that FAILED FORWARD past a user-function call
# and then left through FRETURN or RETURN popped a string descriptor where the trampoline expected its
# continuation and jumped into .rodata (aisnobol TEST and SIR, and every SNOLISP list print, died there). A second
# face of the same bookkeeping: a run that spans a statement and the BLANK statement after it carried the first
# statement's match watermark into the second, whose STATEMENT_END then popped 64 bytes it never pushed.
#
# THE CURE: at IR_STATEMENT_BEGIN the planner releases the dispatch's bytes (the CALL's gamma pop grows by the
# depth so far and the depth restarts at zero) and resets the per-statement watermarks. Mark mode is then
# byte-for-byte the plain lowering from STATEMENT_BEGIN on, in both media. SCRIP_ZD_HOOK_RELEASE=0 restores the
# pre-cure planner and is this gate's fail-once control.
#
# ARMS (each graded against the live sbl -bf oracle, in BOTH modes):
#   a1  the row's shape: a function statement `(~ATOM(L) ATOM(CDR(L))) :F(FRETURN)` under &STCOUNT
#   a2  an assignment of a negated user call inside a function: `F = ~ATOM(L) :S(RETURN)F(FRETURN)`
#   a3  a pattern with a conditional capture, then a BLANK statement, then RETURN (the watermark face)
#   c1  a2 with no statement keyword -- the plain lowering, which the cure must not touch
#   k1  the control: a1 in m3 with SCRIP_ZD_HOOK_RELEASE=0 must NOT match the oracle (it popped a descriptor as its
#       continuation on every pre-cure run measured); if it does, the switch is inert or the pre-cure planner has
#       changed underneath this gate: UNPROVEN, not green. a3 is NOT a control arm on purpose: its pre-cure
#       over-pop lands on a corrupted-but-often-survivable spine and read correct 3 runs in 6 -- a coin is not a control.
#
# EXIT: 0 every cure arm matches SPITBOL and the control arm failed. 1 a cure arm regressed. 2 UNPROVEN.
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"; cd "$ROOT"
. "$HERE/lib_gate.sh" 2>/dev/null || { echo "REFUSING: cannot load lib_gate.sh -- the ONE gate-honesty authority." >&2; exit 3; }
gate_parse_args "$@"
SCRIP="${SCRIP:-$ROOT/scrip}"; RT="${RT_DIR:-$ROOT/out}"
gate_require_exec "$SCRIP" "scrip binary"
gate_require "$RT/libscrip_rt.so" "runtime library"
gate_require_fresh "$ROOT" src "$SCRIP" "$RT/libscrip_rt.so"
ORACLE=/home/resources/x64/bin/sbl
[ -x "$ORACLE" ] || { echo "UNPROVEN(2): correctness oracle absent at $ORACLE -- this gate grades against SPITBOL, never against SCRIP's own output"; exit 2; }
T="$(mktemp -d)" || exit 2; trap 'rm -rf "$T"' EXIT
HDR=" DATA('CONS(CAR,CDR)')
 NIL = CONS('','')
 X = CONS('A',NIL)
 DEFINE('ATOM(A)') :(ATOM.END)
ATOM DIFFER(DATATYPE(A),'CONS') :S(RETURN)F(FRETURN)
ATOM.END"
mk() { { [ "$1" = c1 ] || printf ' Q = &STCOUNT\n'; printf '%s\n%s\nEND\n' "$HDR" "$2"; } > "$T/$1.sno"; }
mk a1 " DEFINE('FN(L)') :(FN.END)
FN (~ATOM(L) ATOM(CDR(L))) :F(FRETURN)
 FN = 'ok' :(RETURN)
FN.END
 OUTPUT = 'r:' FN(X)
 OUTPUT = 'after'"
A2=" DEFINE('F(L)Z') :(F.END)
F F = ~ATOM(L) :S(RETURN)F(FRETURN)
F.END
 OUTPUT = 'r:' F(X)
 OUTPUT = 'after'"
mk a2 "$A2"
mk a3 " DEFINE('F(P)LBL') :(F.END)
F P IDENT(LBL) BREAK('(') . LBL

 F = LBL :(RETURN)
F.END
 OUTPUT = F('ABC(X)')
 OUTPUT = 'after'"
mk c1 "$A2"
run_m3() { timeout 60s "$SCRIP" "$T/$1.sno" < /dev/null 2>&1 | tr '\n' '/'; }
run_m4() { "$SCRIP" --compile -o "$T/$1.s" "$T/$1.sno" < /dev/null >/dev/null 2>&1 || { echo COMPILE-FAILED; return; }
           gcc -no-pie "$T/$1.s" -o "$T/$1.x" -L"$RT" -lscrip_rt -Wl,-rpath,"$RT" -lm -lpthread >/dev/null 2>&1 || { echo LINK-FAILED; return; }
           timeout 60s "$T/$1.x" < /dev/null 2>&1 | tr '\n' '/'; }
graded=0; bad=0
for a in a1 a2 a3 c1; do
    want="$(timeout 60s "$ORACLE" -bf "$T/$a.sno" < /dev/null 2>&1 | tr '\n' '/')"
    [ -n "$want" ] || { echo "UNPROVEN(2): oracle produced no output for $a -- refusing to grade SCRIP against nothing"; exit 2; }
    for m in m3 m4; do
        got="$(run_$m "$a")"; graded=$((graded+1))
        if [ "$got" = "$want" ]; then echo "  ok   $a $m -- matches SPITBOL [$want]"
        else echo "  RED  $a $m -- got [$got] want [$want]"; bad=$((bad+1)); fi
    done
done
ROMAN="${CORPUS_ROOT:-$(cd "$ROOT/.." && pwd)}/corpus/packages/snobol4/csnobol4_suite/roman.sno"
if [ -f "$ROMAN" ] && [ -f "${ROMAN%.sno}.ref" ]; then
    cp "$ROMAN" "$T/roman.sno"; want="$(tr '\n' '/' < "${ROMAN%.sno}.ref")"
    for m in m3 m4; do got="$(run_$m roman)"
        if [ "$got" = "$want" ]; then echo "  ok   roman $m -- the csnobol4 package entry matches its shipped ref (345 lines)"
        else echo "  RED  roman $m -- csnobol4_suite/roman diverged from its shipped ref (rc=139 with RSP 4 MB below RBP is the over-release shape: the boundary release must be the exit-pop amount, not the raw residue, because a match box releases its own hook on the success path)"; bad=$((bad+1)); fi
    done
else echo "  SKIP roman -- csnobol4_suite/roman.sno or its ref not present at $ROMAN (the eight minted arms still bind)"; fi
ctl_fail=0
w1="$(timeout 60s "$ORACLE" -bf "$T/a1.sno" < /dev/null 2>&1 | tr '\n' '/')"; g1="$(SCRIP_ZD_HOOK_RELEASE=0 run_m3 a1)"; [ "$g1" = "$w1" ] || ctl_fail=1
echo "  control: with SCRIP_ZD_HOOK_RELEASE=0, k1 m3 got [$g1] want [$w1] -- $([ "$ctl_fail" -eq 1 ] && echo 'pre-cure planner FAILS (the switch is live)' || echo 'pre-cure planner PASSED (inert switch?)')"
echo "STATEMENT-MARK-HOOK-RELEASE graded=$graded bad=$bad control_failed=$ctl_fail"
[ "$graded" -eq 8 ] || { echo "UNPROVEN(2): expected 8 graded arms, got $graded -- a runner that cannot measure never prints the success shape"; exit 2; }
[ "$ctl_fail" -eq 1 ] || { echo "UNPROVEN(2): the control arm matched SPITBOL with the cure switched off -- SCRIP_ZD_HOOK_RELEASE is inert or the pre-cure planner changed; this gate cannot tell a cure from a coincidence"; exit 2; }
if [ "$bad" -ne 0 ]; then echo "GATE RED(1) [sno-statement-mark-mode-releases-the-hook-residue]: a statement in statement-mark mode ran at a different spine base than the plain lowering and its forward-failure or blank-statement path left the trampoline a descriptor instead of a continuation"; exit 1; fi
echo "GATE GREEN(0) [sno-statement-mark-mode-releases-the-hook-residue]: a program that reads a statement keyword runs every statement at the plain base in both modes; the pre-cure planner fails the control"
exit 0
