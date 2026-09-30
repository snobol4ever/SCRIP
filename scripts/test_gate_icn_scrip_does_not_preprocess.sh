#!/usr/bin/env bash
"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
# test_gate_icn_scrip_does_not_preprocess.sh -- SCRIP DOES NOT PREPROCESS: MACRO PROCESSING IS A PRE-STEP THAT GENERATES THE SOURCE
# SCRIP PARSES (Lon 2026-09-30 07:0x CDT, in-chat to the ceo, verbatim: "SCRIP should not handle pre-processing from macro processors.
# Put in place the pre-step to generate the source after the pre-process step, and then send to SCRIP after processing."; RULES.md
# FACT RULE SCRIP DOES NOT PREPROCESS; GOAL-CEO.md CEO-1366).
#
# THREE ARMS. (1) The ipp port is gone from the Icon lexer: src/parsers/icon/icon_lex.c names neither icn_pp_text nor any ipp_
# function. (2) scrip -E is an unknown option: it refuses with rc != 0 and does not print the preprocessed text. (3) A directive
# reaching the parser is a NAMED PARSE ERROR, never a silent expansion or a silent skip: a program whose first line is "$define N 3"
# is refused in mode 3 and in mode 4 with an error that names the directive. The pre-step for Icon is SCRIP'S OWN standalone
# preprocessor tool shipped beside scrip (the ipp port moved out of the lexer into src/tools/), run by the runner (the coo's row
# runners-run-the-pre-step-...) -- never the oracle's icont -E, which Lon ruled out as part of the product (07:4x: "Do not use the
# oracle's pre-process step if it is part of the product ... instead provide the equivalent SCRIP functionality"); icont -E is only
# the gate that proves the tool's output identical on the corpus. MEASURED RED at de0302f83: icon_lex.c carries the port (icn_lex_init
# runs icn_pp_text at line 965), scrip -E echoes the preprocessed text, and the witness prints 3 in both modes. Exit 0 = all three
# arms; 1 = an arm fails; 2 = cannot measure.
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"; G="$(basename "${BASH_SOURCE[0]}" .sh)"
SCRIP="${SCRIP_BIN:-$ROOT/scrip}"; [ -x "$SCRIP" ] || { echo "⛔ GATE REFUSE(2) [$G]: no scrip at $SCRIP"; exit 2; }
LEX="$ROOT/src/parsers/icon/icon_lex.c"; [ -f "$LEX" ] || { echo "⛔ GATE REFUSE(2) [$G]: no $LEX"; exit 2; }
T=$(mktemp -d) || exit 2; trap 'rm -rf "$T"' EXIT
RC=0
n=$(grep -c 'icn_pp_text\|ipp_' "$LEX"); [ "$n" -eq 0 ] && echo "  PASS icon_lex.c names no preprocessor" || { RC=1; echo "  FAIL icon_lex.c still carries the ipp port ($n lines name icn_pp_text or ipp_)"; }
printf '$define N 3\nprocedure main();\n  write(N);\nend\n' > "$T/w.icn"
( cd "$T" && timeout 20 "$SCRIP" -E w.icn < /dev/null > e.out 2> e.err ); erc=$?
if [ "$erc" -ne 0 ] && ! grep -q 'write(3)' "$T/e.out"; then echo "  PASS scrip -E refuses (rc=$erc)"; else RC=1; echo "  FAIL scrip -E still preprocesses (rc=$erc, output: $(head -c 80 "$T/e.out" | tr '\n' ' '))"; fi
( cd "$T" && timeout 20 "$SCRIP" w.icn < /dev/null > m3.out 2> m3.err ); m3rc=$?
if [ "$m3rc" -ne 0 ] && grep -q 'define' "$T/m3.err"; then echo "  PASS mode 3 refuses the directive with a named error"; else RC=1; echo "  FAIL mode 3 accepted a \$define line (rc=$m3rc, stdout: $(head -c 40 "$T/m3.out" | tr '\n' ' '))"; fi
( cd "$T" && timeout 20 "$SCRIP" --compile -o w.s w.icn < /dev/null > m4.out 2> m4.err ); m4rc=$?
if [ "$m4rc" -ne 0 ] && grep -q 'define' "$T/m4.err"; then echo "  PASS mode 4 refuses the directive with a named error"; else RC=1; echo "  FAIL mode 4 compiled a \$define line (rc=$m4rc)"; fi
[ "$RC" = 0 ] && echo "GATE PASS(0) [$G]: SCRIP carries no preprocessor and names a directive as a parse error" || echo "⛔ GATE FAIL(1) [$G]: SCRIP still preprocesses"
exit $RC
