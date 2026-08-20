#!/usr/bin/env bash
# test_gate_pl_gz4.sh — PL-GZ-4a gate: multi-clause ground-fact CHOICE + backtracking on the
# Proebsting-pure path — THE seed-transcription rung (test_pl_1.c edge/2 shape).
# Cursor + trail-mark live in the box's OWN frame row [ζ+off]; redo (β) dispatches on the cursor
# to clause-k's ω which advances + unwinds to the ENTRY mark and falls into clause k+1; the
# conjunction is wired as the Byrd-Box backward redo chain (goal[i].ω → goal[i-1].β) so `fail`
# (the pure-wiring bb_fail leaf) IS the backtracking driver.
# Probes A-E (admitted): m2 == m3 == m4 stdout BYTE-IDENTICAL, both branches on the new path.
# Probe F (non-admitted: 5 clauses > cap): BOTH branches refuse identically via pl_gz_admit.
set -u
cd "$(dirname "$0")/.."
SCRIP=./scrip
RT=out/libscrip_rt.so
TMP=$(mktemp -d)
trap 'rm -rf "$TMP"' EXIT
fail() { echo "GATE-PL-GZ4 FAIL: $1"; exit 1; }
[ -x "$SCRIP" ] || fail "scrip binary missing (make scrip)"
[ -f "$RT" ]    || fail "libscrip_rt.so missing (make libscrip_rt)"

run_admitted() { # $1=name $2=program(printf-format) $3=expected_stdout(printf-format)
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

run_admitted enum3   ':- initialization(main).\nfact(a).\nfact(b).\nfact(c).\nmain :- fact(X), write(X), nl, fail.\n' 'a\nb\nc\n'
grep -q "CELL_CHOICE" "$TMP/enum3.s" || fail "enum3 m4 .s lacks CELL_CHOICE box (choice not on the GZ path)"
run_admitted mixarg  ':- initialization(main).\nedge(a, b).\nedge(b, c).\nedge(b, d).\nmain :- edge(b, X), write(X), nl, fail.\n' 'c\nd\n'
run_admitted midfail ':- initialization(main).\nfact(a).\nfact(b).\nmain :- fact(X), X = b, write(X), nl.\n' 'b\n'
run_admitted nested  ':- initialization(main).\nfact(a).\nfact(b).\ncolor(x).\ncolor(y).\nmain :- fact(F), color(C), write(F), write(C), nl, fail.\n' 'ax\nay\nbx\nby\n'
run_admitted nosol   ':- initialization(main).\nfact(a).\nfact(b).\nmain :- fact(c), write(y), nl.\n' ''
run_admitted softenum ':- initialization(main).\nfact(a).\nfact(b).\nfact(c).\nmain :- fact(X), write(X), nl, fail ; true.\n' 'a\nb\nc\n'
grep -q "PROMOTED" "$TMP/softenum.s" || fail "softenum m4 .s lacks the promoted ω landing (soft tail not wired)"
run_admitted softbind ':- initialization(main).\nmain :- X = a, fail ; true.\n' ''
run_admitted softsemi ':- initialization(main).\nnum(1).\nnum(2).\nmain :- num(N), write(N), nl, N = 2 ; true.\n' '1\n2\n'

printf ':- initialization(main).\nfact(a).\nfact(b).\nmain :- fact(X), write(X), nl, fail ; write(done), nl.\n' > "$TMP/neg2.pl"
"$SCRIP" --run "$TMP/neg2.pl" </dev/null > "$TMP/n23" 2>"$TMP/ne23" || fail "neg2 m3 rc"
grep -q "INTERP-FALLBACK" "$TMP/ne23" || fail "neg2 (non-true right arm) m3 did NOT show the loud fallback (GZ wrongly admitted?)"
"$SCRIP" --compile --target=x86 "$TMP/neg2.pl" > "$TMP/n2.s" 2>/dev/null || fail "neg2 m4 compile rc"
grep -q "gzq" "$TMP/n2.s" && fail "neg2 (non-true right arm) m4 .s has gzq labels (GZ wrongly admitted)"

printf ':- initialization(main).\nbig(a).\nbig(b).\nbig(c).\nbig(d).\nbig(e).\nmain :- big(X), write(X), nl, fail.\n' > "$TMP/neg.pl"
"$SCRIP" --run "$TMP/neg.pl" </dev/null > "$TMP/n3" 2>"$TMP/ne3" || fail "neg m3 rc"
grep -q "INTERP-FALLBACK" "$TMP/ne3" || fail "neg (5 clauses) m3 did NOT show the loud fallback (GZ wrongly admitted?)"
"$SCRIP" --compile --target=x86 "$TMP/neg.pl" > "$TMP/n.s" 2>/dev/null || fail "neg m4 compile rc"
grep -q "gzq" "$TMP/n.s" && fail "neg (5 clauses) m4 .s has gzq labels (GZ wrongly admitted)"

echo "GATE-PL-GZ4 PASS: multi-clause choice + backtracking + query-tail (G ; true) soft-fail promotion (fail-driven enumeration, mixed-arg fold, mid-chain redo, stacked choices, soft tails) m2==m3==m4 byte-identical on the GZ path; over-cap refused identically by both branches"
exit 0
