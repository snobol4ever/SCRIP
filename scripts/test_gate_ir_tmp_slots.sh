#!/usr/bin/env bash
# test_gate_ir_tmp_slots.sh — GROUND ZERO #5 (GOAL-IR-IMMUTABLE-EMIT) B0 keystone lock.
# Pins the JCON ir_Tmp contract that ir_tmp_slot_assign() establishes in LOWER: every value-PRODUCING
# node carries a result slot (lhs=) and every side-effecting/control node does NOT. Checks STRUCTURE,
# not exact offsets (offsets shift as lower changes; the contract does not). If this gate goes red,
# the keystone the whole JCON-in-SCRIP conversion reads (nd->lhs) has regressed — see
# .github/JCON-TO-SCRIP-IR-MAP.md § "tmp = slot".
# AUTHORS: Lon Jones Cherryholmes · Jeffrey Cooper M.D. · Claude Sonnet  DATE: 2026-06-27
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "${HERE}/.." && pwd)"
SCRIP="${ROOT}/scrip"
[ -x "${SCRIP}" ] || { echo "FAIL: ${SCRIP} not built"; exit 1; }

TMP="$(mktemp -d)"; trap 'rm -rf "${TMP}"' EXIT
PROG="${TMP}/keystone.icn"
printf 'procedure main();x := a + b;write(x);end\n' > "${PROG}"
DUMP="$("${SCRIP}" --dump-ir "${PROG}" < /dev/null 2>&1)"

echo "=== B0 keystone: ir_Tmp slot contract on --dump-ir ==="
fails=0
chk() { # chk "<label>" "<grep-for-line>" "must|mustnot" "lhs="
  local label="$1" line_pat="$2" mode="$3" tok="$4" line
  line="$(printf '%s\n' "${DUMP}" | grep -E "${line_pat}" | head -1)"
  if [ -z "${line}" ]; then echo "  FAIL  ${label}: no '${line_pat}' line in dump"; fails=$((fails+1)); return; fi
  if printf '%s' "${line}" | grep -q "${tok}"; then
    [ "${mode}" = must ] && echo "  ok    ${label} carries ${tok}" || { echo "  FAIL  ${label} must NOT carry ${tok}: ${line}"; fails=$((fails+1)); }
  else
    [ "${mode}" = mustnot ] && echo "  ok    ${label} has no ${tok}" || { echo "  FAIL  ${label} must carry ${tok}: ${line}"; fails=$((fails+1)); }
  fi
}

chk "IR_BINOP (value producer)"  "IR_BINOP"   must    "lhs="
chk "IR_VAR (value producer)"    "IR_VAR"     must    "lhs="
chk "IR_CALL (value producer)"   "IR_CALL"    must    "lhs="
chk "IR_ASSIGN (side-effecting)" "IR_ASSIGN"  mustnot "lhs="
chk "IR_SUCCEED (control port)"  "IR_SUCCEED" mustnot "lhs="
chk "IR_FAIL (control port)"     "IR_FAIL"    mustnot "lhs="

# A binop's operands must themselves be tmp-slotted producers (consumer reads off(operand)).
binop_ops="$(printf '%s\n' "${DUMP}" | grep -E "IR_BINOP" | head -1 | sed -E 's/.*\[([0-9 ]+)\].*/\1/')"
if [ -n "${binop_ops}" ]; then echo "  ok    IR_BINOP operands present: [${binop_ops}]"; else echo "  FAIL  IR_BINOP has no operand list"; fails=$((fails+1)); fi

echo "-------------------------------------------"
if [ "${fails}" -eq 0 ]; then echo "PASS: ir_Tmp slot contract holds."; exit 0; fi
echo "FAIL: ${fails} keystone contract violation(s)."; exit 1
