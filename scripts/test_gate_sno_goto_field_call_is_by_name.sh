#!/usr/bin/env bash
# test_gate_sno_goto_field_call_is_by_name.sh -- A CALL IN THE SNOBOL4 GOTO FIELD IS INVOKED BY NAME.
#
# WHAT THIS IS: the RUN-TIME half of the goto-field call construct, landed by hq_P 2026-09-05 (row
# snobol4-goto-field-function-call-is-invoked-by-name-and-scrip-is-exactly-inverted). Its sibling
# test_gate_sno_goto_field_call_parses.sh pins only that the construct PARSES, and says in its own header
# that the run-time semantics are a known open row deliberately left ungraded. This gate is that row's
# end state, so the two together now cover the construct.
#
# THE CONTRACT, graded against /home/resources/x64/bin/sbl -bf and reproduced here as two arms:
#   NRETURN arm -- the callee returns a NAME (GOF = .L3 :(NRETURN)); that NAME is the transfer target.
#   VALUE arm   -- the callee returns a value (GOV = 'L3' :(RETURN)); SPITBOL raises ERROR 021.
#
# BOTH ARMS OR NEITHER, AND THAT IS THE WHOLE POINT OF GRADING THEM TOGETHER: before the cure SCRIP was
# EXACTLY INVERTED -- it accepted the value form the oracle refuses, and refused with ERROR 239 the NRETURN
# form the oracle accepts. Curing only the NRETURN arm would leave half an inversion standing, still green
# on any gate that asked only "does :(GOF()) reach L3". So a one-armed pass here is a RED, not a partial.
#
# ⛔ WHAT THIS GATE DOES NOT CLAIM: that a by-name goto target may be the special transfer RETURN. It may
# not, yet -- `RET = .RETURN :(NRETURN)`, the published STATEF.inc/POKEV.inc idiom, resolves the NAME
# correctly and then dies ERROR 038 "transfer to undefined label: RETURN" because the three function-return
# landings are compile-time per-graph wirings and the by-name target is chosen at run time. That is a
# SECOND, distinct mechanism and it has its own row. Pinning it here would make this gate red for a defect
# it does not test.
#
# EXIT: 0 both arms match SPITBOL. 1 an arm regressed. 2 UNPROVEN (no built scrip).
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"; cd "$ROOT"
. "$HERE/lib_gate.sh" 2>/dev/null || { echo "REFUSING: cannot load lib_gate.sh -- the ONE gate-honesty authority." >&2; exit 3; }
gate_parse_args "$@"
SCRIP="${SCRIP:-$ROOT/scrip}"; RT="${RT_DIR:-$ROOT/out}"
gate_require_exec "$SCRIP" "scrip binary"
gate_require "$RT/libscrip_rt.so" "runtime library"
gate_require_fresh "$ROOT" src "$SCRIP" "$RT/libscrip_rt.so"
T="$(mktemp -d)" || exit 2; trap 'rm -rf "$T"' EXIT
printf "        DEFINE('GOF()')                 :(MAIN)\nGOF     GOF  =  .L3                     :(NRETURN)\nMAIN    OUTPUT  =  'go'                  :(GOF())\n        OUTPUT  =  'NOPE'\nL3      OUTPUT  =  'landed'\nEND\n" > "$T/nret.sno"
printf "        DEFINE('GOV()')                 :(MAIN)\nGOV     GOV  =  'L3'                    :(RETURN)\nMAIN    OUTPUT  =  'go'                  :(GOV())\n        OUTPUT  =  'NOPE'\nL3      OUTPUT  =  'landed'\nEND\n" > "$T/val.sno"
bad=0
for m in m3 m4; do
    if [ "$m" = m3 ]; then
        g="$("$SCRIP" "$T/nret.sno" < /dev/null 2>/dev/null | tr '\n' '/')"
        v="$("$SCRIP" "$T/val.sno" < /dev/null 2>&1)"
    else
        "$SCRIP" --compile -o "$T/nret.s" "$T/nret.sno" < /dev/null >/dev/null 2>&1
        "$SCRIP" --compile -o "$T/val.s"  "$T/val.sno"  < /dev/null >/dev/null 2>&1
        gcc -no-pie "$T/nret.s" -o "$T/nret.x" -L"$RT" -lscrip_rt -Wl,-rpath,"$RT" -lm -lpthread >/dev/null 2>&1 || { echo "  RED  nret $m -- link failed"; bad=1; continue; }
        gcc -no-pie "$T/val.s"  -o "$T/val.x"  -L"$RT" -lscrip_rt -Wl,-rpath,"$RT" -lm -lpthread >/dev/null 2>&1 || { echo "  RED  val $m -- link failed";  bad=1; continue; }
        g="$(timeout 20s "$T/nret.x" < /dev/null 2>/dev/null | tr '\n' '/')"
        v="$(timeout 20s "$T/val.x"  < /dev/null 2>&1)"
    fi
    if [ "$g" = "go/landed/" ]; then echo "  ok   nret $m -- the returned NAME is the transfer target"; else echo "  RED  nret $m -- got [$g] want [go/landed/] (ERROR 239 here is the pre-cure inversion)"; bad=1; fi
    case "$v" in *"ERROR 021"*) echo "  ok   val  $m -- a value return raises ERROR 021" ;; *) echo "  RED  val  $m -- a value return must raise ERROR 021 (SPITBOL parity); got [$(echo "$v" | head -2 | tr '\n' '/')]"; bad=1 ;; esac
done
if [ "$bad" -ne 0 ]; then echo "GATE RED(1) [sno-goto-field-call-is-by-name]: the goto-field call is not by-name in both arms -- half an inversion is still an inversion"; exit 1; fi
echo "GATE GREEN(0) [sno-goto-field-call-is-by-name]: NRETURN name is the target and a value return raises ERROR 021, both modes"
