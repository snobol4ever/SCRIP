#!/usr/bin/env bash
# util_rtx_symbol_inventory.sh — regenerate the RTX replacement inventory MECHANICALLY.
#
# Sweeps every committed SNOBOL4 .s artifact for call/jmp targets, subtracts the
# program-local label classes the emitter mints (proc_/xchain/snoch/xcat/alt/dol/
# smatch/xscan/xgvarg/fn_/P_/L_/Ln_/s_/g_/a_/arb/ucall/flat_/main_ + bare registers),
# and prints what remains: the runtime symbols the blob actually calls.
#
# WHY MECHANICAL: the hand-written table in .github/ARCH-SNOBOL4-RTX.md section 5
# is a snapshot and WILL rot as the emitter changes. This script is the ground truth.
# Run it at the start of any RTX rung and diff against the doc.
#
# Usage: bash scripts/util_rtx_symbol_inventory.sh [corpus_root]   (default /home/claude/corpus)
set -uo pipefail
CORPUS="${1:-/home/claude/corpus}"
[ -d "$CORPUS" ] || { echo "FAIL: corpus not found at $CORPUS"; exit 1; }
mapfile -t FILES < <(find "$CORPUS" -path '*snobol4*' -name '*.s' 2>/dev/null)
[ "${#FILES[@]}" -gt 0 ] || { echo "FAIL: no SNOBOL4 .s artifacts under $CORPUS"; exit 1; }
LOCAL='^(proc_|xchain|snoch|xcat|alt[0-9]|dol[0-9]|smatch|xscan|xgvarg|xcap|fn_|P_|L_|Ln_|s_|g_|m_|a_|arb[0-9]|ucall|flat_|main_|vowelinstem|stemmer|cons_|cvc_|doublec_|record_register|execute_code_dyn|comm_stno|qword|r[0-9a-z]{1,3}$|scan_retry)'
printf '# RTX symbol inventory — generated %s from %d .s artifacts under %s\n' "$(date -u +%Y-%m-%dT%H:%M:%SZ)" "${#FILES[@]}" "$CORPUS"
printf '# count symbol   (count = static call/jmp sites across all artifacts, NOT dynamic weight)\n'
grep -ohE '(call|jmp)[[:space:]]+[A-Za-z_][A-Za-z0-9_]*' "${FILES[@]}" 2>/dev/null \
  | awk '{print $2}' | grep -vE "$LOCAL" | sort | uniq -c | sort -rn
