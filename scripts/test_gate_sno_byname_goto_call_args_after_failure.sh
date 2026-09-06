#!/usr/bin/env bash
# test_gate_sno_byname_goto_call_args_after_failure.sh -- A BY-NAME GOTO CALL KEEPS ITS WANT-NAME ACROSS THE SLIM FAST PATH.
#
# WHAT THIS IS: a SNOBOL4 goto whose target is a FUNCTION CALL is invoked BY NAME -- the callee returns a NAME
# via `:(NRETURN)` and the goto transfers to it. This gate pins the case where that call CARRIES ARGUMENTS and
# is taken from a statement whose subject FAILED. Before the cure every b-arm below died
# `ERROR 021 -- function called by name returned a value`, which is a true sentence about the SYMPTOM and names
# the CALLEE. The callee was innocent: its own emitted prologue (bb_define.cpp:93) saves rt_g_want_name into its
# frame and restores it on exit, correctly. The CALLER destroyed the flag first -- rt_proc_call_open_slim
# (src/runtime/rt/rt.c) read it into a local `wn`, cleared the global, and never used `wn` again. NINE sites in
# that file open with that identical line; the other EIGHT hand `wn` to rt_proc_call_prologue(...) /
# rt_proc_call_prologue_lex(..., wn), which stores it. The ninth dropped it, so the callee dutifully saved 0.
#
# ⛔ THE DISCRIMINATOR IS TWO-FACTOR AND NEITHER FACTOR ALONE DOES IT, WHICH IS WHY THE c-ARMS ARE NOT DECORATION:
# c1 (same failed statement, ZERO arguments) and c2 (same one-argument call, statement SUCCEEDS) were GREEN
# BEFORE the cure and must stay green after it. A gate carrying only the b-arms would pass just as happily if a
# future change made every by-name goto call take the non-slim path -- the c-arms are what force the two paths
# to stay distinguishable. Five other axes were ablated and exonerated during the diagnosis and are deliberately
# NOT pinned here: conditional vs unconditional goto arm, one-armed vs two-armed, multiple call sites (the gimpel
# POKEV witness has EIGHT and pruning to one kept it red), callee locals, and depth-3 nesting.
#
# ⭐ EVERY TARGET HERE IS A PLAIN LABEL. That is deliberate. The special-transfer targets (RETURN/FRETURN/NRETURN)
# are the sibling gate test_gate_sno_goto_special_transfer_target.sh.
#
# ⛔ THE b-ARMS GRADE m3 ONLY, AND THE REASON IS A MEASUREMENT, NOT A CONVENIENCE. In m4 all four b-arms SEGV --
# and they segv IDENTICALLY WITH THIS CURE ON AND OFF, so that crash is NOT this defect and is not this cure's
# doing. It is a second, pre-existing defect one layer down: gdb puts the fault inside the CALLEE's own DEFINE
# box (n1_define_bx) with a return address of 0, i.e. the NRETURN floater pair is not seated on the staged call
# path in mode 4 (the s110/s111 arms in bb_call_proc_staged.cpp). Its row is
# snobol4-m4-byname-goto-call-with-args-segvs-in-the-callee-define-box-nreturn-floater-not-seated. Grading m4
# here would leave this gate permanently red for a defect it does not test and could never go blocking; the
# c-arms DO grade both modes, so mode 4 is not silently unexamined -- it is examined on exactly the shapes this
# cure is responsible for.
#
# NON-VACUOUS BY CONSTRUCTION: SCRIP_SLIM_WANTNAME=0 restores the pre-cure clear, and every b-arm is RED under it
# while both c-arms stay GREEN. Prove that before trusting a green: SCRIP_SLIM_WANTNAME=0 bash <this file>
#
# EXIT: 0 all arms match SPITBOL. 1 an arm regressed. 2 UNPROVEN (no built scrip / no oracle).
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"; cd "$ROOT"
. "$HERE/lib_gate.sh" 2>/dev/null || { echo "REFUSING: cannot load lib_gate.sh -- the ONE gate-honesty authority." >&2; exit 3; }
gate_parse_args "$@"
SCRIP="${SCRIP:-$ROOT/scrip}"; RT="${RT_DIR:-$ROOT/out}"
gate_require_exec "$SCRIP" "scrip binary"
gate_require "$RT/libscrip_rt.so" "runtime library"
gate_require_fresh "$ROOT" src "$SCRIP" "$RT/libscrip_rt.so"
T="$(mktemp -d)" || exit 2; trap 'rm -rf "$T"' EXIT
emit() { printf "        DEFINE('G(L)')\n        DEFINE('G2(L,M)')\n        DEFINE('H()')                   :(MAIN)\nG       G   =  .TGT                     :(NRETURN)\nG2      G2  =  .TGT                     :(NRETURN)\nH       H   =  .TGT                     :(NRETURN)\nMAIN    OUTPUT  =  'before'\n        X  =  'q'\n        %s                              %s\n        OUTPUT  =  'not here'\nTGT     OUTPUT  =  'at TGT'\nEND\n" "$2" "$3" > "$T/$1.sno"; }
emit b1 "'a' 'b'"            ':F(G(1))'
emit b2 "'a' 'b'"            ':F(G(X))'
emit b3 "'a' 'b'"            ':F(G2(1,2))'
emit b4 "IDENT('a','b')"     ':F(G(1))'
emit c1 "'a' 'b'"            ':F(H())'
emit c2 "'a' 'a'"            ':S(G(1))'
ORACLE=/home/resources/x64/bin/sbl
[ -x "$ORACLE" ] || { echo "UNPROVEN(2): correctness oracle absent at $ORACLE -- this gate grades against SPITBOL, never against SCRIP's own output"; exit 2; }
bad=0
for a in b1 b2 b3 b4 c1 c2; do
    want="$("$ORACLE" -bf "$T/$a.sno" < /dev/null 2>&1 | sed -n '/^before$/,$p' | tr '\n' '/')"
    [ -n "$want" ] || { echo "  UNPROVEN $a -- oracle produced no graded output; refusing to grade SCRIP against nothing"; exit 2; }
    modes="m3 m4"; case "$a" in b*) modes="m3";; esac
    for m in $modes; do
        if [ "$m" = m3 ]; then
            got="$(timeout 20s "$SCRIP" "$T/$a.sno" < /dev/null 2>&1 | tr '\n' '/')"
        else
            "$SCRIP" --compile -o "$T/$a.s" "$T/$a.sno" < /dev/null >/dev/null 2>&1
            gcc -no-pie "$T/$a.s" -o "$T/$a.x" -L"$RT" -lscrip_rt -Wl,-rpath,"$RT" -lm -lpthread >/dev/null 2>&1 || { echo "  RED  $a $m -- link failed"; bad=1; continue; }
            got="$(timeout 20s "$T/$a.x" < /dev/null 2>&1 | tr '\n' '/')"
        fi
        if [ "$got" = "$want" ]; then echo "  ok   $a $m -- matches SPITBOL [$want]"
        else echo "  RED  $a $m -- got [$got] want [$want]"; bad=1; fi
    done
done
if [ "$bad" -ne 0 ]; then echo "GATE RED(1) [sno-byname-goto-call-args-after-failure]: a by-name goto call with arguments taken from a failed statement lost its want-name"; exit 1; fi
echo "GATE GREEN(0) [sno-byname-goto-call-args-after-failure]: by-name goto calls with literal/variable/two-arg payloads survive the slim fast path after a failed subject (m3; the m4 segv is a separate seated-floater defect with its own row), and both zero-arg and success-arm controls hold in BOTH modes"
