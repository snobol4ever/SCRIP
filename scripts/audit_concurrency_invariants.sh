#!/usr/bin/env bash
# audit_concurrency_invariants.sh — the SHARED herding gate for the 3 concurrent BB sessions.
# Enforces the COMPLETION TESTs of both FACT RULES (SHARED-LOWERER + TEMPLATE-ONLY EMISSION) so LOWER
# (one file) and EMITTER (one dispatch + per-box templates) cannot drift into a mess. Run before every
# commit alongside prove_lower2.sh and the emitter gates. Exit non-zero on ANY violation.
set -u
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
HQ="${HQ:-$ROOT/../.github}"
LOWER="$ROOT/src/lower/lower.c"
EMIT="$ROOT/src/emitter/emit_core.c"
fail=0
say(){ printf '%s\n' "$*"; }
bad(){ printf 'VIOLATION: %s\n' "$*"; fail=1; }

# --- LOWER (a): no duplicated case TT_ label WITHIN any one role switch ---
# lower.c has 3 role switches (lower_value / lower_pattern / lower_goal); a kind may legitimately appear
# once PER role. Split the file on the role-switch function headers and check each block for a dup label.
if [ -f "$LOWER" ]; then
  ld=$(awk '
    # Entering ANY static IR_t * function: a role dispatcher (lower_value/pattern/goal) opens a NEW block and
    # turns counting ON; any OTHER static IR_t * helper (g_term, g_builtin, v_*, wire_*) turns counting OFF so
    # its legitimate TT_QLIT/TT_VAR cases are NOT misattributed to the preceding role switch (a false positive).
    /^static IR_t \* lower_(value|pattern|goal)\(/ { blk++; in_role=1; next; }
    /^static IR_t \* [A-Za-z_]/                   { in_role=0; next; }
    in_role && /case TT_[A-Z0-9_]+/ {
      if (match($0, /case TT_[A-Z0-9_]+/)) {
        lbl=substr($0, RSTART+5, RLENGTH-5); key=blk":"lbl;
        if (seen[key]++) print "  block#"blk" "lbl;
      }
    }' "$LOWER" | sort -u)
  [ -n "$ld" ] && bad "lower.c: case TT_ label duplicated WITHIN a single role switch:$(printf '%s' "$ld")"
else
  bad "lower.c not found at $LOWER"
fi

# --- EMITTER (a): no duplicated case IR_ label in emit_core.c (one dispatch case per kind) ---
if [ -f "$EMIT" ]; then
  edups=$(grep -oE 'case IR_[A-Z0-9_]+' "$EMIT" | sort | uniq -d)
  [ -n "$edups" ] && bad "emit_core.c: duplicated case IR_ label(s): $(echo $edups)"
else
  bad "emit_core.c not found at $EMIT"
fi

# --- EMITTER (c): no NEW bytes-outside-templates beyond the sanctioned baseline ---
# util_template_purity_audit.sh reports the sanctioned fail-loud/rel32 side-effects (FACT baseline) and
# exits nonzero by design. We fail only on a REGRESSION above the known baseline count (PURITY_BASELINE).
if [ -x "$ROOT/scripts/util_template_purity_audit.sh" ]; then
  bash "$ROOT/scripts/util_template_purity_audit.sh" >/tmp/_purity.out 2>&1
  pc=$(grep -oE '[0-9]+ non-binary side-effect' /tmp/_purity.out | grep -oE '^[0-9]+' | head -1)
  pc=${pc:-0}
  # PURITY_BASELINE tracks the count of SANCTIONED fail-loud/rel32 side-effects (FATAL guards) across all
  # templates. Bumped 6 -> 7 (2026-05-31): the 7th is the pre-existing bb_call.cpp GZ-3 text-arm fail-loud
  # (write(binop) result-slot guard) already in HEAD — NOT from the PB-0 SUBJECT box, whose only fail-loud
  # lives inside MEDIUM_BINARY (audit-exempt) so bb_subject.cpp contributes 0. Catches a NEW 8th.
  # Bumped 7 -> 8 (2026-06-01): the 8th is bb_rk_gather.cpp's single FLAT-take fall-loud guard (RK-EMIT-GATHER).
  PURITY_BASELINE="${PURITY_BASELINE:-8}"
  if [ "$pc" -gt "$PURITY_BASELINE" ]; then
    bad "template purity REGRESSED: $pc side-effects outside templates > baseline $PURITY_BASELINE (see /tmp/_purity.out)"
  fi
fi

# --- (d) FACT RULE blocks byte-identical across the 3 GOAL files (awk first-match, not the buggy sed) ---
goalfiles="GOAL-SNOBOL4-BB.md GOAL-ICON-BB.md GOAL-PROLOG-BB.md"
check_block(){ # $1=start-regex $2=end-regex $3=label  (ASCII-only anchors: a multi-byte em-dash defeats awk '.')
  local m="" first="" g h n
  for g in $goalfiles; do
    [ -f "$HQ/$g" ] || { bad "$3: $HQ/$g missing"; return; }
    n=$(awk -v s="$1" -v e="$2" '$0~s{p=1} p{print} $0~e{if(p)exit}' "$HQ/$g" | wc -l)
    [ "$n" -eq 0 ] && { bad "$3: extraction EMPTY in $g (anchor '$1' or '$2' not found — check anchors)"; continue; }
    h=$(awk -v s="$1" -v e="$2" '$0~s{p=1} p{print} $0~e{if(p)exit}' "$HQ/$g" | md5sum | cut -d' ' -f1)
    [ -z "$first" ] && first="$h"
    [ "$h" != "$first" ] && bad "$3: block md5 differs in $g ($h != $first)"
  done
}
check_block 'SHARED-LOWERER ONE-FILE' 'prove_lower2.sh green' 'LOWER FACT RULE'
check_block 'ONE-DISPATCH CONCURRENCY .FACT RULE' 'util_template_purity_audit.sh clean' 'EMITTER FACT RULE'

[ "$fail" -eq 0 ] && say "OK: concurrency invariants hold (LOWER one-per-role, EMITTER one-dispatch, no stray bytes, FACT RULES byte-identical x3)."
exit "$fail"
