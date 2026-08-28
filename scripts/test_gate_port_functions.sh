#!/usr/bin/env bash
# FACT RULE "FOUR PORT FUNCTIONS" (Lon pivot 2026-07-08 s5): every port operation in live code goes through
# x86_alpha/x86_beta/x86_gamma/x86_omega — the string-port operand form x86(<anything>, "α"/"β"/"γ"/"ω") and
# its PORT_* macro spelling are RETIRED (x86_parse aborts on them at emit time). This gate greps for the CALL
# SHAPE (comma + port operand + close-paren), so it catches literal mnemonics, macro operands, AND
# variable/function-call mnemonics alike — the three successive census blind spots the pivot session hit
# (incomplete jcc list; PORT_* macros; x86(cc_expr, "ω")). Comments stripped first; the parked pair
# bb_callee_frame.cpp / bb_query_frame.cpp (non-Makefile, PARK-NEVER-DELETE) is excluded.
SELF_DIR="$(cd "$(dirname "$0")" && pwd)"
. "$SELF_DIR/lib_gate.sh"
cd "$(dirname "$0")/.." || exit 2
gate_require "src/templates" "templates dir"
gate_require "src/emitter" "emitter dir"
total=0; hits=""
while IFS= read -r f; do
  case "$f" in *bb_callee_frame.cpp|*bb_query_frame.cpp) continue;; esac
  m=$(perl -0777 -pe 's{/\*.*?\*/}{}gs; s{//[^\n]*}{}g' "$f" | grep -cE ',[ ]*("α"|"β"|"γ"|"ω"|PORT_ALPHA|PORT_BETA|PORT_GAMMA|PORT_OMEGA)[ ]*\)')
  if [ "$m" -gt 0 ]; then total=$((total+m)); hits="$hits $f($m)"; fi
done < <(find src/templates src/emitter -name '*.cpp' -o -name '*.c' -o -name '*.h')
echo "string-port call-shape operands live (code, comments stripped): $total  (MUST be 0)"
if [ "$total" -ne 0 ]; then echo "OFFENDERS:$hits"; echo "GATE FAIL."; exit 1; fi
echo "OK: all port operations route through the four port functions."
