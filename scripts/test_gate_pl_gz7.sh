#!/usr/bin/env bash
# GATE PL-GZ-7 (ITE-commit canon, m2 oracle to paper §4.5 / SWI C_LCUT).
# Pins the LANDED half of PL-GZ-7: the m2 oracle now commits the condition of (Cond -> Then ; Else)
# at first success (IR_ITE marks the CP ledger at entry; IR_ITE_COMMIT on cond.γ truncates the
# condition's choicepoints; IR_ITE_GATE on cond.ω routes post-commit condition exhaustion to ω
# instead of Else — the paper §4.5 E1.fail gate), and \+ / \= share the same bounded condition.
# The headline ( a(X),X>=2 -> true ; X=0 ) fail-driven prints exactly 2 (was 2 3 0), matching m4.
# This gate pins m2 == m3 == m4 == canon on the commit shapes via the legacy ITE path; the ITE is
# semidet at construct level (β = ω_in), agreeing with the m4 flat_drive_ite β→ω tombstone.
# The new-path §4.5 box (IR_CELL_ITE admission onto the GZ substrate) is the rung's REMAINING half
# and is not yet gated here.
set -u
cd "$(dirname "$0")/.."
SCRIP=./scrip
RT=out/libscrip_rt.so
TMP=$(mktemp -d)
trap 'rm -rf "$TMP"' EXIT
fail() { echo "GATE-PL-GZ7 FAIL: $1"; exit 1; }
[ -x "$SCRIP" ] || fail "scrip binary missing (make scrip)"
[ -f "$RT" ]    || fail "libscrip_rt.so missing (make libscrip_rt)"
m4_run() { # $1=name ; compiles $TMP/$1.pl -> runs -> stdout in $TMP/$1.o4
  local NM=$1
  "$SCRIP" --compile --target=x86 "$TMP/$NM.pl" > "$TMP/$NM.s" 2>/dev/null || fail "$NM m4 compile rc"
  as -o "$TMP/$NM.o" "$TMP/$NM.s" 2>/dev/null || fail "$NM m4 as"
  gcc -no-pie "$TMP/$NM.o" -L out -lscrip_rt -Wl,-rpath,"$PWD/out" -o "$TMP/$NM.bin" 2>/dev/null || fail "$NM m4 link"
  "$TMP/$NM.bin" </dev/null > "$TMP/$NM.o4" || fail "$NM m4 run rc"
}
pin() { # $1=name $2=program(printf-format) $3=expected_stdout(printf-format) — m2 == m3 == m4 == canon
  local NM=$1 PRG=$2 EXP=$3
  printf "$PRG" > "$TMP/$NM.pl"
  "$SCRIP" --interp "$TMP/$NM.pl" </dev/null > "$TMP/$NM.o2" 2>/dev/null || fail "$NM m2 rc"
  "$SCRIP" --run    "$TMP/$NM.pl" </dev/null > "$TMP/$NM.o3" 2>/dev/null || fail "$NM m3 rc"
  m4_run "$NM"
  printf "$EXP" | cmp -s - "$TMP/$NM.o2" || fail "$NM m2 output not canon (got: $(tr '\n' ' ' < "$TMP/$NM.o2"))"
  cmp -s "$TMP/$NM.o2" "$TMP/$NM.o3" || fail "$NM m2 vs m3 stdout differ"
  cmp -s "$TMP/$NM.o2" "$TMP/$NM.o4" || fail "$NM m2 vs m4 stdout differ"
}
# headline: the condition a(X),X>=2 commits at the first success (X=2); the X=3 alternative and the
# Else (X=0) are both dead — exactly 2. This is the WAM-CP-9 / paper §4.5 case.
pin commit ':- initialization(main).\na(1). a(2). a(3).\nf(X) :- ( a(X), X >= 2 -> true ; X = 0 ).\nmain :- ( f(R), write(R), nl, fail ; true ).\n' '2\n'
# bare-call condition: a single-solution call as the whole condition commits and Then runs once.
pin barecall ':- initialization(main).\na(1).\nmain :- ( ( a(X) -> true ; fail ), write(X), nl, fail ; true ).\n' '1\n'
# fresh re-entry: an enclosing generator m(X) re-drives the ITE with a NEW binding each solution;
# the condition is re-evaluated fresh (commit state reset on each forward entry) — s, 2, 3.
pin reentry ':- initialization(main).\nm(1). m(2). m(3).\nmain :- ( m(X), ( X >= 2 -> write(X) ; write(s) ), nl, fail ; true ).\n' 's\n2\n3\n'
# semidet construct: ITE redo bypasses to ω (β = ω_in), so the Else generator m(X) is NOT re-satisfied
# on backtracking — single 1, agreeing with the m4 β→ω tombstone.
pin semidet ':- initialization(main).\nm(1). m(2).\nmain :- ( ( fail -> true ; m(X) ), write(X), nl, fail ; true ).\n' '1\n'
# \+ and \= share the bounded condition: \+ a(2) succeeds (y); a redo-driven \+ fail commits its
# condition (k printed once); 1 \= 2 succeeds (u).
pin negbound ':- initialization(main).\na(1).\nmain :- ( \\+ a(2) -> write(y) ; write(n) ), nl, ( \\+ fail, write(k), nl, fail ; true ), ( 1 \\= 2 -> write(u) ; write(v) ), nl.\n' 'y\nk\nu\n'
echo "GATE-PL-GZ7 PASS: ITE-commit canon m2 == m3 == m4 (commit · barecall · reentry · semidet · negbound)"
