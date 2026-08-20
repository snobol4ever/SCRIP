#!/usr/bin/env bash
# test_gate_pl_gz2.sh — PL-GZ-2 gate: the hello rung on the Proebsting-pure path.
# Probe 1 (hello class): m2 == m3 == m4 stdout BYTE-IDENTICAL, and BOTH m3 and m4 took the
#   new path (m3 stderr has NO INTERP-FALLBACK banner; m4 .s HAS gzq labels).
# Probe 2 (non-admitted: X = a): BOTH branches refuse identically through the ONE shared
#   pl_gz_admit (m3 stderr HAS the INTERP-FALLBACK marker; m4 .s has NO gzq labels).
# PL-M34 equal-sets LAW enforced at the new-path boundary from day one.
set -u
cd "$(dirname "$0")/.."
SCRIP=./scrip
RT=out/libscrip_rt.so
TMP=$(mktemp -d)
trap 'rm -rf "$TMP"' EXIT
fail() { echo "GATE-PL-GZ2 FAIL: $1"; exit 1; }
[ -x "$SCRIP" ] || fail "scrip binary missing (make scrip)"
[ -f "$RT" ]    || fail "libscrip_rt.so missing (make libscrip_rt)"

printf ':- initialization(main).\nmain :- write(hello), nl.\n' > "$TMP/hello.pl"
"$SCRIP" --run "$TMP/hello.pl" </dev/null > "$TMP/o2" 2>/dev/null || fail "hello m2 rc"
"$SCRIP" --run    "$TMP/hello.pl" </dev/null > "$TMP/o3" 2>"$TMP/e3" || fail "hello m3 rc"
grep -q "INTERP-FALLBACK" "$TMP/e3" && fail "hello m3 fell back to the interpreter (GZ path not taken)"
"$SCRIP" --compile --target=x86 "$TMP/hello.pl" > "$TMP/h.s" 2>/dev/null || fail "hello m4 compile rc"
grep -q "gzq" "$TMP/h.s" || fail "hello m4 .s lacks gzq labels (GZ path not taken)"
as -o "$TMP/h.o" "$TMP/h.s" 2>/dev/null || fail "hello m4 as"
gcc -no-pie "$TMP/h.o" -L out -lscrip_rt -Wl,-rpath,"$PWD/out" -o "$TMP/h.bin" 2>/dev/null || fail "hello m4 link"
"$TMP/h.bin" </dev/null > "$TMP/o4" || fail "hello m4 run rc"
cmp -s "$TMP/o2" "$TMP/o3" || fail "hello m2 vs m3 stdout differ"
cmp -s "$TMP/o2" "$TMP/o4" || fail "hello m2 vs m4 stdout differ"
printf 'hello\n' | cmp -s - "$TMP/o2" || fail "hello m2 output is not 'hello\\n'"

printf ':- initialization(main).\nmain :- X = f(a), write(X), nl.\n' > "$TMP/neg.pl"
"$SCRIP" --run "$TMP/neg.pl" </dev/null > "$TMP/n3" 2>"$TMP/ne3" || fail "neg m3 rc"
grep -q "INTERP-FALLBACK" "$TMP/ne3" || fail "neg m3 did NOT show the loud fallback (GZ wrongly admitted?)"
"$SCRIP" --compile --target=x86 "$TMP/neg.pl" > "$TMP/n.s" 2>/dev/null || fail "neg m4 compile rc"
grep -q "gzq" "$TMP/n.s" && fail "neg m4 .s has gzq labels (GZ wrongly admitted)"

echo "GATE-PL-GZ2 PASS: hello m2==m3==m4 byte-identical on the GZ path; non-admitted refused identically by both branches"
exit 0
