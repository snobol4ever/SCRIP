#!/usr/bin/env bash
"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP_BIN:-$ROOT/scrip}"; [ -x "$SCRIP" ] || { echo "⛔ REFUSE(2): no scrip at $SCRIP"; exit 2; }
command -v gcc >/dev/null || { echo "⛔ REFUSE(2): no gcc for mode 4"; exit 2; }
T=$(mktemp -d) || exit 2; trap 'rm -rf "$T"' EXIT
cat > "$T/w1.icn" <<'ICN'
procedure main()
   local c, s, t, i;
   c := list(2); s := "abc"; t := table(); i := 1;
   &error := 200;
   write("01 |||:= ", image(c |||:= s) | ("ERR " || &errornumber));
   write("02 ~==   ", image([] ~== "x") | ("ERR " || &errornumber));
   write("03 t(t)  ", image(t(t)) | ("ERR " || &errornumber));
   write("04 ||    ", image(&lcase || numeric) | ("ERR " || &errornumber));
   write("05 <=    ", image(i <= []) | ("ERR " || &errornumber));
   write("06 right ", image(right("","")) | ("ERR " || &errornumber));
   write("07 close ", image(close("F")) | ("ERR " || &errornumber));
   write("08 trim  ", image(trim(&lcase,[])) | ("ERR " || &errornumber));
   write("09 pos   ", image(pos("a")) | ("ERR " || &errornumber));
   write("10 |||   ", image(list(10) ||| "abc") | ("ERR " || &errornumber));
   write("11 *null ", image(*&null) | ("ERR " || &errornumber));
   write("12 *proc ", image(*main) | ("ERR " || &errornumber));
   write("13 ?null ", image(?&null) | ("ERR " || &errornumber));
   write("14 sub   ", image(nowhere[1]) | ("ERR " || &errornumber));
   write("15 call  ", image(nowhere(1)) | ("ERR " || &errornumber));
   write("16 disp  ", image(display(,[])) | ("ERR " || &errornumber));
end
ICN
cat > "$T/w1.want" <<'WANT'
01 |||:= ERR 108
02 ~==   ERR 103
03 t(t)  ERR 106
04 ||    ERR 103
05 <=    ERR 102
06 right ERR 101
07 close ERR 105
08 trim  ERR 104
09 pos   ERR 101
10 |||   ERR 108
11 *null ERR 112
12 *proc ERR 112
13 ?null ERR 113
14 sub   ERR 114
15 call  ERR 106
16 disp  ERR 105
WANT
cat > "$T/w2.icn" <<'ICN'
procedure main()
   local L, e;
   &error := 200;
   L := [1, 2.5, "5", "s", 'ab', "", [1], table()];
   write("A ", image(1 || "x"), image(2.5 || "x"), image("s" || "x"), image('ab' || "x"), image("" || "x"));
   write("B ", image(*""), image(*"abc"), image(*'ab'), image(*[1,2]), image(*table()));
   write("C ", image("3" <= 5), image(2 <= 2.5), image("a" ~== "b"), image(1 ~== 2));
   write("D ", image([1] ||| [2]), image([] ||| [1]));
   write("E ", image(trim("ab ")), image(right("ab")), image(trim("ab ", 5)), image(right("ab", 2.9)));
   write("F ", image(?"abc"), image(?5), image(L[2]), image(L[-1]));
   write("G ", &errornumber)
end
ICN
cat > "$T/w2.want" <<'WANT'
A "1x""2.5x""sx""abx""x"
B 03220
C 52.5"b""2"
D list_6(2)list_9(1)
E "ab""b""ab ""ab"
F "a"32.5table_1(0)
WANT
fail=0
for w in w1 w2; do
  ( cd "$T" && timeout 60 "$SCRIP" "$w.icn" </dev/null > "$w.m3" 2>&1 ); rc3=$?
  if [ "$rc3" = 0 ] && cmp -s "$T/$w.m3" "$T/$w.want"; then echo "  PASS  m3 $w"; else echo "  FAIL  m3 $w rc=$rc3"; diff "$T/$w.want" "$T/$w.m3" | head -8 | sed 's/^/        /'; fail=1; fi
  if ( cd "$T" && "$SCRIP" --compile "$w.icn" > "$w.s" 2>/dev/null && gcc -c "$w.s" -o "$w.o" 2>/dev/null && gcc "$w.o" -L"$ROOT/out" -lscrip_rt -lm -Wl,-rpath,"$ROOT/out" -o "$w.bin" 2>/dev/null ); then
    ( cd "$T" && timeout 60 "./$w.bin" </dev/null > "$w.m4" 2>&1 ); rc4=$?
    if [ "$rc4" = 0 ] && cmp -s "$T/$w.m4" "$T/$w.want"; then echo "  PASS  m4 $w"; else echo "  FAIL  m4 $w rc=$rc4"; diff "$T/$w.want" "$T/$w.m4" | head -8 | sed 's/^/        /'; fail=1; fi
  else echo "  FAIL  m4 $w: no binary"; fail=1; fi
done
cat > "$T/s1.sno" <<'SNO'
	X =
	OUTPUT = SIZE(X)
	OUTPUT = SIZE("abc")
	OUTPUT = "a" 5
END
SNO
printf '0\n3\na5\n' > "$T/s1.want"
( cd "$T" && timeout 60 "$SCRIP" s1.sno </dev/null > s1.out 2>&1 )
if cmp -s "$T/s1.out" "$T/s1.want"; then echo "  PASS  snobol4 control arm: SIZE and concatenation keep their own discipline"; else echo "  FAIL  snobol4 control arm"; diff "$T/s1.want" "$T/s1.out" | head -6 | sed 's/^/        /'; fail=1; fi
if [ "$fail" = 0 ]; then echo "✅ PASS: Icon's argument type discipline -- 16 refusals with icont's own codes, 24 conversions still accepted, both modes, SNOBOL4 unaffected"; exit 0; fi
echo "⛔ FAIL: an Icon operation coerces where iconx raises, or raises where iconx converts (see the FAIL rows)"; exit 1
