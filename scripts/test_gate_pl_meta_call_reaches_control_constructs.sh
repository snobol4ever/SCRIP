#!/usr/bin/env bash
# stale-binary preflight (row test-gate-scripts-that-grade-scrip-refuse-on-a-stale-binary-census-widened, hq_T 2026-09-05)
"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
# test_gate_pl_meta_call_reaches_control_constructs.sh -- A CONTROL CONSTRUCT REACHED THROUGH A VARIABLE GOAL RUNS.
# ⛔⛔ THIS GATE IS THE ROW'S NAMED ACCEPTANCE GATE AND IS **EXPECTED RED** UNTIL THE ROW LANDS. IT IS
# DELIBERATELY **NOT** IN `make test` -- wiring a known-red arm into the blocking set would red the fleet.
# Row: prolog-meta-call-bridge-does-not-reach-builtins-or-operators (hq_C). Run it BY NAME to see the class.
# ⛔ AND READ THIS BEFORE 'CURING' IT: the wrapper-proc mechanism that makes arms (a)/(b) pass is PROVEN and was
# written, measured and REVERTED on purpose, because the bridge's FAILURE path SIGSEGVs (pre-existing -- a plain
# user predicate that fails, reached via call/1, crashes on the UNMODIFIED binary). Landing the wrappers first
# would convert a clean existence_error into a SIGSEGV for every program that meta-calls a control construct.
# ORDER: cure the omega path first, THEN register the wrappers. See
# FINDING-2026-09-06-hq_C-the-prolog-meta-call-bridge-segfaults-whenever-the-meta-called-goal-fails.md
# Row prolog-meta-call-bridge-does-not-reach-builtins-or-operators (hq_C 2026-09-06). THIS GATE COVERS THE CONTROL
# HALF ONLY -- the builtin half (write/1, fail/0, is/2 ...) is hq_R's and is deliberately NOT asserted here.
#
# THE DEFECT THIS PINS: rt_pl_goal_gen_h (src/runtime/by_name_dispatch.c) resolves a variable goal through
# rt_proc_is_registered, and the proc table holds COMPILED CLAUSE PREDICATES only. A control construct is a
# compile-time lowering rule, never a callable runtime entity, so `X = (p,q), call(X)` raised
# existence_error(procedure, ,/2) while the SOURCE-LITERAL `call((p,q))` was green -- the same goal, green or
# red depending only on whether the compiler could SEE it.
# THE CURE: lower_pl_stage2 synthesises and registers a wrapper proc per control name, reusing the machinery it
# already uses for dynamic predicates. The dispatcher is untouched. ⛔ The wrappers are COMPILER-SYNTHESISED on
# purpose: real swipl refuses the same clauses in user source ("No permission to modify static procedure").
#
# WHAT IT PROVES, in five directions so a naive cure cannot pass it:
#   (a) THE CURE: (p,q), (p;q) and (p->q) reached through a variable all run, both modes.
#   (b) ⭐ IF-THEN-ELSE IS NOT DISJUNCTION. X = (Cond->T;E) must COMMIT: when Cond succeeds the else branch must
#       NOT run, and when Cond fails the THEN branch must not. The naive wrapper ';'(A,B) :- (call(A);call(B))
#       passes arm (a) in full and FAILS here -- it runs E after T fails. This arm is the reason the wrapper
#       dispatches on the shape of A at all.
#   (c) ⭐ CUT TRANSPARENCY: a cut inside a meta-called goal must not escape and cut the CALLER's choicepoints.
#       call/1 is a cut barrier in ISO. This is the arm most likely to pass a naive witness and still be wrong.
#   (d) CONTROL: a USER definition of the same name still wins -- synthesis must not shadow the program.
#   (e) The oracle is ASSERTED, never assumed: swipl runs every witness and the gate REFUSES if its answer moved.
# ⛔ Hermetic: every program is written under mktemp; nothing in corpus/ is read or written.
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP:-$ROOT/scrip}"
refuse(){ echo "⛔ REFUSED-TO-GRADE: $*"; exit 2; }
[ -x "$SCRIP" ] || refuse "scrip not built at $SCRIP (a test that cannot measure REFUSES, never skips-as-success)"
SWIPL=/usr/bin/swipl; [ -x "$SWIPL" ] || refuse "no swipl at $SWIPL -- the oracle premise cannot be checked"
[ -f "$ROOT/out/libscrip_rt.so" ] || refuse "no $ROOT/out/libscrip_rt.so -- the m4 arm cannot link"
W="$(mktemp -d "${TMPDIR:-/tmp}/gate_plmeta.XXXXXX")" || refuse "mktemp failed"; trap 'rm -rf "$W"' EXIT
# every witness is built from USER predicates only -- no builtin is ever the meta-called goal, because the
# builtin half of this class is a different lane and must not be able to red this gate.
PRE='p :- write(a).\nq :- write(b).\nyes.\nno :- yes, fail.\n'
printf "${PRE}:- initialization(main).\nmain :- X = (p,q), call(X), nl.\n"                        > "$W/conj.pl"
printf "${PRE}:- initialization(main).\nmain :- X = (p;q), call(X), nl.\n"                        > "$W/disj.pl"
printf "${PRE}:- initialization(main).\nmain :- X = (yes->p), call(X), nl.\n"                     > "$W/arrow.pl"
printf "${PRE}:- initialization(main).\nmain :- X = (yes->p;q), call(X), nl.\n"                   > "$W/ite_t.pl"
printf "${PRE}:- initialization(main).\nmain :- X = (no->p;q), call(X), nl.\n"                    > "$W/ite_f.pl"
printf "${PRE}c(1).\nc(2).\nmain2 :- X = (yes,!), call(X), fail.\nmain2 :- write(second), nl.\n:- initialization(main2).\n" > "$W/cut.pl"
printf "','(_,_) :- write(mine), nl.\n:- initialization(main).\nmain :- X = (p,q), call(X).\n"    > "$W/user.pl"
fails=0; checks=0
ck(){ checks=$((checks+1)); if [ "$1" = ok ]; then printf '  ok    %s\n' "$2"; else printf '  FAIL  %s\n' "$2"; fails=$((fails+1)); fi; }
oracle(){ timeout 30 "$SWIPL" -q -g true -t halt "$1" </dev/null 2>/dev/null; }
echo "=== gate: a control construct reached through a VARIABLE goal must run (control half only) ==="
echo "--- (e) the ORACLE premise, asserted before anything is graded ---"
declare -A EXP
for w in conj disj arrow ite_t ite_f cut; do
  EXP[$w]="$(oracle "$W/$w.pl")"
  [ -n "${EXP[$w]}" ] || refuse "ORACLE PREMISE MOVED: swipl printed nothing for $w.pl -- re-derive rather than grade"
done
[ "${EXP[ite_t]}" = "a" ]      || refuse "ORACLE PREMISE MOVED: (yes->p;q) printed [${EXP[ite_t]}], expected a (commit to THEN)"
[ "${EXP[ite_f]}" = "b" ]      || refuse "ORACLE PREMISE MOVED: (no->p;q) printed [${EXP[ite_f]}], expected b (commit to ELSE)"
ck ok "oracle: conj=[${EXP[conj]}] disj=[${EXP[disj]}] arrow=[${EXP[arrow]}] ite_t=[${EXP[ite_t]}] ite_f=[${EXP[ite_f]}] cut=[${EXP[cut]}]"
for m in m3 m4; do
  echo "--- $m ---"
  run(){ if [ "$m" = m3 ]; then timeout 60 "$SCRIP" "$1" </dev/null 2>/dev/null; else
           s="$W/out.s"; b="$W/out.bin"; rm -f "$s" "$b"
           timeout 90 "$SCRIP" --compile -o "$s" "$1" </dev/null >/dev/null 2>&1 || return 97
           timeout 90 gcc -no-pie -o "$b" "$s" -L"$ROOT/out" -lscrip_rt -lm >/dev/null 2>&1 || return 98
           LD_LIBRARY_PATH="$ROOT/out" timeout 60 "$b" </dev/null 2>/dev/null; fi; }
  for w in conj disj arrow; do
    o="$(run "$W/$w.pl")"; [ "$o" = "${EXP[$w]}" ] && ck ok "$m (a) $w through a variable goal -> [$o]" \
      || ck no "$m (a) $w printed [$o], oracle says [${EXP[$w]}]"
  done
  o="$(run "$W/ite_t.pl")"; [ "$o" = "${EXP[ite_t]}" ] && ck ok "$m (b) if-then-else COMMITS to THEN -> [$o]" \
    || ck no "$m (b) ite true-cond printed [$o], oracle says [${EXP[ite_t]}] -- a disjunction wrapper runs the else branch too"
  o="$(run "$W/ite_f.pl")"; [ "$o" = "${EXP[ite_f]}" ] && ck ok "$m (b) if-then-else COMMITS to ELSE -> [$o]" \
    || ck no "$m (b) ite false-cond printed [$o], oracle says [${EXP[ite_f]}]"
  o="$(run "$W/cut.pl")"; [ "$o" = "${EXP[cut]}" ] && ck ok "$m (c) a cut inside a meta-called goal stays local -> [$o]" \
    || ck no "$m (c) cut witness printed [$o], oracle says [${EXP[cut]}] -- the cut escaped the call/1 barrier"
  o="$(run "$W/user.pl")"; [ "$o" = "mine" ] && ck ok "$m (d) a USER definition of the name still wins" \
    || ck no "$m (d) user-defined ,/2 printed [$o], expected mine -- synthesis must not shadow the program"
done
echo "------------------------------------------------------------"
if [ "$fails" -eq 0 ]; then echo "✅ GATE GREEN: control constructs reached through a variable goal run, commit correctly, and do not shadow the program (examined $checks checks)"; exit 0; fi
echo "⛔ GATE FAIL: $fails of $checks checks red"
echo "   CURE: lower_pl_stage2 synthesises a wrapper proc per control name; ';'/2 must dispatch on the shape of its first argument so (C->T;E) commits"
exit 1
