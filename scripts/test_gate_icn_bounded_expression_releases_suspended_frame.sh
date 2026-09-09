#!/usr/bin/env bash
# stale-binary preflight (row test-gate-scripts-that-grade-scrip-refuse-on-a-stale-binary-census-widened, hq_T 2026-09-05)
"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
# test_gate_icn_bounded_expression_releases_suspended_frame.sh — CEO-456 R5, the omega release, as a gate (R6).
#
# A bounded expression that leaves a callee generator SUSPENDED must release that frame at its omega (iconx
# Op_Unmark resets rsp below the ef_marker, dropping every suspended frame above it). If SCRIP retained the
# frame, a loop would grow the stack once per iteration and die ERROR 246 long before a million laps. The
# witness: f suspends and is never resumed; one million bounded calls, then two hundred thousand more in a
# condition, both modes, against icont's counts. PASS = both modes print the oracle's two lines and exit 0.
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP_BIN:-$ROOT/scrip}"; [ -x "$SCRIP" ] || { echo "⛔ REFUSE(2): no scrip at $SCRIP"; exit 2; }
T=$(mktemp -d) || exit 2; trap 'rm -rf "$T"' EXIT
cat > "$T/r5.icn" <<'ICN'
procedure f(); suspend 1; suspend 2; end
procedure main();
   local i, n;
   n := 0;
   every i := 1 to 1000000 do { f(); n +:= 1 };
   write(n);
   every i := 1 to 200000 do { if f() = 1 then n +:= 1 };
   write(n);
end
ICN
printf '1000000\n1200000\n' > "$T/want"
fail=0
timeout 120 "$SCRIP" "$T/r5.icn" </dev/null > "$T/m3" 2>&1; rc3=$?
if [ $rc3 -ne 0 ] || ! cmp -s "$T/m3" "$T/want"; then echo "⛔ FAIL m3 rc=$rc3: $(head -c 160 "$T/m3" | tr '\n' ' ')"; fail=1; fi
if "$SCRIP" --compile "$T/r5.icn" > "$T/r5.s" 2>/dev/null && gcc -c "$T/r5.s" -o "$T/r5.o" 2>/dev/null && gcc "$T/r5.o" -L"$ROOT/out" -lscrip_rt -lm -Wl,-rpath,"$ROOT/out" -o "$T/r5.bin" 2>/dev/null; then
  timeout 120 "$T/r5.bin" </dev/null > "$T/m4" 2>&1; rc4=$?
  if [ $rc4 -ne 0 ] || ! cmp -s "$T/m4" "$T/want"; then echo "⛔ FAIL m4 rc=$rc4: $(head -c 160 "$T/m4" | tr '\n' ' ')"; fail=1; fi
else echo "⛔ REFUSE(2): mode-4 build of the witness failed -- cannot measure"; exit 2; fi
[ $fail = 0 ] && echo "✅ PASS: a bounded expression releases its suspended callee frame at omega -- 1,200,000 laps, both modes, no ERROR 246 (CEO-456 R5)"
exit $fail
