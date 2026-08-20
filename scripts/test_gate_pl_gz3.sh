#!/usr/bin/env bash
# test_gate_pl_gz3.sh — PL-GZ-3 gate (3a + 3b): =/2, write(Var), single-clause ground-fact
# head-unify inlined to CELL_UNIFY chains on the Proebsting-pure path.
# Logic vars = frame cells [ζ+8+8i]; every binding trailed; trail-mark/unwind = the one spine.
# Probes A-C (admitted): m2 == m3 == m4 stdout BYTE-IDENTICAL and BOTH took the new path
#   (m3 stderr has NO INTERP-FALLBACK banner; m4 .s HAS gzq labels + CELL_UNIFY boxes).
# Probe D (failure): X = a, X = b — ω/unwind path: EMPTY stdout in all three modes, proving
#   the semidet fail wiring (test/je→ω) in BOTH mediums.
# Probe E (non-admitted: compound X = f(a)): BOTH branches refuse identically through the
#   ONE shared pl_gz_admit (m3 stderr HAS the INTERP-FALLBACK marker; m4 .s has NO gzq labels).
set -u
cd "$(dirname "$0")/.."
SCRIP=./scrip
RT=out/libscrip_rt.so
TMP=$(mktemp -d)
trap 'rm -rf "$TMP"' EXIT
fail() { echo "GATE-PL-GZ3 FAIL: $1"; exit 1; }
[ -x "$SCRIP" ] || fail "scrip binary missing (make scrip)"
[ -f "$RT" ]    || fail "libscrip_rt.so missing (make libscrip_rt)"

run_admitted() { # $1=name $2=program $3=expected_stdout(printf-format)
  local NM=$1 PRG=$2 EXP=$3
  printf "$PRG" > "$TMP/$NM.pl"
  "$SCRIP" --run "$TMP/$NM.pl" </dev/null > "$TMP/$NM.o2" 2>/dev/null || fail "$NM m2 rc"
  "$SCRIP" --run    "$TMP/$NM.pl" </dev/null > "$TMP/$NM.o3" 2>"$TMP/$NM.e3" || fail "$NM m3 rc"
  grep -q "INTERP-FALLBACK" "$TMP/$NM.e3" && fail "$NM m3 fell back to the interpreter (GZ path not taken)"
  "$SCRIP" --compile --target=x86 "$TMP/$NM.pl" > "$TMP/$NM.s" 2>/dev/null || fail "$NM m4 compile rc"
  grep -q "gzq" "$TMP/$NM.s" || fail "$NM m4 .s lacks gzq labels (GZ path not taken)"
  as -o "$TMP/$NM.o" "$TMP/$NM.s" 2>/dev/null || fail "$NM m4 as"
  gcc -no-pie "$TMP/$NM.o" -L out -lscrip_rt -Wl,-rpath,"$PWD/out" -o "$TMP/$NM.bin" 2>/dev/null || fail "$NM m4 link"
  "$TMP/$NM.bin" </dev/null > "$TMP/$NM.o4" || fail "$NM m4 run rc"
  cmp -s "$TMP/$NM.o2" "$TMP/$NM.o3" || fail "$NM m2 vs m3 stdout differ"
  cmp -s "$TMP/$NM.o2" "$TMP/$NM.o4" || fail "$NM m2 vs m4 stdout differ"
  printf "$EXP" | cmp -s - "$TMP/$NM.o2" || fail "$NM m2 output is not expected"
}

run_admitted unify_atom ':- initialization(main).\nmain :- X = world, write(X), nl.\n' 'world\n'
grep -q "CELL_UNIFY" "$TMP/unify_atom.s" || fail "unify_atom m4 .s lacks CELL_UNIFY box"
run_admitted unify_int  ':- initialization(main).\nmain :- X = 42, write(X), nl.\n' '42\n'
run_admitted unify_vv   ':- initialization(main).\nmain :- X = a, Y = X, write(Y), nl.\n' 'a\n'
run_admitted unify_fail ':- initialization(main).\nmain :- X = a, X = b, write(no), nl.\n' ''
run_admitted fact_var   ':- initialization(main).\ngreeting(hello).\nmain :- greeting(X), write(X), nl.\n' 'hello\n'
grep -q "CELL_UNIFY" "$TMP/fact_var.s" || fail "fact_var m4 .s lacks CELL_UNIFY box (fact head-unify not inlined)"
run_admitted fact_pair  ':- initialization(main).\npair(a, 1).\nmain :- pair(X, Y), write(X), write(Y), nl.\n' 'a1\n'
run_admitted fact_fold  ':- initialization(main).\npair(a, 1).\nmain :- pair(a, 1), write(yes), nl.\n' 'yes\n'
run_admitted fact_nofold ':- initialization(main).\ngreeting(hello).\nmain :- greeting(bye), write(yes), nl.\n' ''
run_admitted fact_zero  ':- initialization(main).\ngo.\nmain :- go, write(ok), nl.\n' 'ok\n'

run_admitted consthead  ':- initialization(main).\np(a) :- write(side), nl.\nmain :- p(X), write(X), nl.\n' 'side\na\n'

printf ':- initialization(main).\nq(X) :- Y is X + 1, write(Y), nl.\nmain :- q(1).\n' > "$TMP/neg2.pl"
"$SCRIP" --run "$TMP/neg2.pl" </dev/null > "$TMP/n23" 2>"$TMP/ne23" || fail "neg2 m3 rc"
grep -q "INTERP-FALLBACK" "$TMP/ne23" || fail "neg2 (arith rule body) m3 did NOT show the loud fallback (GZ wrongly admitted?)"
"$SCRIP" --compile --target=x86 "$TMP/neg2.pl" > "$TMP/n2.s" 2>/dev/null || fail "neg2 m4 compile rc"
grep -q "gzq\|gzp" "$TMP/n2.s" && fail "neg2 (arith rule body) m4 .s has gz labels (GZ wrongly admitted)"

printf ':- initialization(main).\nmain :- X = f(a), write(ok), nl.\n' > "$TMP/neg.pl"
"$SCRIP" --run "$TMP/neg.pl" </dev/null > "$TMP/n3" 2>"$TMP/ne3" || fail "neg m3 rc"
grep -q "INTERP-FALLBACK" "$TMP/ne3" || fail "neg m3 did NOT show the loud fallback (GZ wrongly admitted?)"
"$SCRIP" --compile --target=x86 "$TMP/neg.pl" > "$TMP/n.s" 2>/dev/null || fail "neg m4 compile rc"
grep -q "gzq" "$TMP/n.s" && fail "neg m4 .s has gzq labels (GZ wrongly admitted)"

echo "GATE-PL-GZ3 PASS: =/2 (cell↔const, cell↔cell) + write(Var) + single-clause ground-fact head-unify m2==m3==m4 byte-identical on the GZ path; failure unwinds; compound and multi-clause refused identically by both branches"
exit 0
