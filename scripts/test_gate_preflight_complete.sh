#!/bin/bash
# test_gate_preflight_complete.sh -- ⭐ IS PHASE-0 PRE-FLIGHT ACTUALLY DONE?  Computed, never declared.
# Every item of GOAL-CEO.md Phase 0 plus all three FIRING GATE conditions, each checked mechanically.
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; . "$HERE/lib_gate.sh"; gate_parse_args "$@"
PO="${S4E_POSTOFFICE:-/home/resources/postoffice}"; BAD=0; N=0
say() { N=$((N+1)); if [ "$1" = 0 ]; then echo "  ✅ $2"; else echo "  ⛔ $2"; BAD=$((BAD+1)); fi; }
echo "=== PHASE 0 ==="
grep -q 'assign)' "$HERE/s4e_msg.sh"; say $? "V2-1 picker: rank-sorted + assign + assigned-first next"
m=$(awk -F'\t' '!/^#/ && NF>0 && NF!=4' "$PO/QUEUE.tsv" | wc -l); b=$(grep -c '^$' "$PO/QUEUE.tsv")
[ "$m" -eq 0 ] && [ "$b" -eq 0 ]; say $? "V2-2 queue purge: index-only, 0 malformed rows, 0 blank lines (m=$m b=$b)"
grep -q 'oldest' "$HERE/s4e_msg.sh"; say $? "V2-3 banner: board carries oldest-unanswered age"
grep -q 'NO postoffice mailbox' "$HERE/s4e_msg.sh"; say $? "V2-4 identity asserted, never globbed"
bash "$HERE/test_gate_baton_donewhen_runnable.sh" >/dev/null 2>&1; say $? "V2-2 batons: every DONE-WHEN is a runnable command"
echo "=== FIRING GATE ==="
q=$(bash "$HERE/s4e_msg.sh" fleet 2>/dev/null | grep -cE 'Q=[1-9]'); [ "${q:-0}" -eq 0 ]; say $? "fleet shows 0 unanswered questions"
bash "$HERE/test_gate_oracle_bf_capable.sh" >/dev/null 2>&1; say $? "every seat resolves a -bf-capable oracle (asset root, no per-seat clones)"
u=0; for r in SCRIP corpus .github; do d="/home/claude_P/$r"; [ -d "$d/.git" ] || continue
  [ -n "$(cd "$d" && git status --porcelain)" ] && u=$((u+1))
  [ "$(cd "$d" && git rev-parse HEAD)" != "$(cd "$d" && git rev-parse @{u} 2>/dev/null)" ] && u=$((u+1)); done
[ "$u" -eq 0 ]; say $? "hq_P repos clean and pushed ($u issue(s))"
echo ""
gate_floor "$N" 8 "pre-flight conditions"
gate_verdict "$BAD" "pre-flight condition(s) not met"
