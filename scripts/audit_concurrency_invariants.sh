#!/usr/bin/env bash
# audit_concurrency_invariants.sh — the SHARED herding gate for the 3 concurrent BB sessions.
# Enforces the COMPLETION TESTs of both FACT RULES (SHARED-LOWERER + TEMPLATE-ONLY EMISSION) so LOWER
# (one file) and EMITTER (one dispatch + per-box templates) cannot drift into a mess. Run before every
# commit alongside prove_lower.sh and the emitter gates. Exit non-zero on ANY violation.
set -u
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
HQ="${HQ:-$ROOT/../.github}"
# PATHS CORRECTED s169 (seat1, queue row gates-dead-paths): src/lower/lower.c was split into the
# per-language lowerers and src/emitter/emit_core.c is now src/emitter/emit.cpp. Both refs had been
# dangling since the src reorg, so checks (a) and the emitter dup-label check reported "not found"
# instead of enforcing anything.
LOWER_GLOB="$ROOT/src/lower/lower_*.c"
EMIT="$ROOT/src/emitter/emit.cpp"
fail=0
say(){ printf '%s\n' "$*"; }
bad(){ printf 'VIOLATION: %s\n' "$*"; fail=1; }

# --- LOWER (a): no duplicated case TT_ label WITHIN any one role switch ---
# Historically lower.c had 3 role switches (lower_value / lower_pattern / lower_goal); a kind may
# legitimately appear once PER role. Now scanned across the per-language lowerers, src/lower/lower_*.c.
lower_files=$(ls $LOWER_GLOB 2>/dev/null)
if [ -n "$lower_files" ]; then
  roles=0
  for LOWER in $lower_files; do
    roles=$((roles + $(grep -cE '^static IR_t \* lower_(value|pattern|goal)\(' "$LOWER")))
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
    [ -n "$ld" ] && bad "$(basename "$LOWER"): case TT_ label duplicated WITHIN a single role switch:$(printf '%s' "$ld")"
  done
  # A gate that matches nothing must say so rather than pass silently (s169 seat1): the lower_value /
  # lower_pattern / lower_goal role-dispatcher shape this check was written against does not exist in any
  # per-language lowerer, so the scan is VACUOUS until HQ re-specifies it against the current LOWER shape.
  [ "$roles" -eq 0 ] && say "  NOTE (a): 0 lower_(value|pattern|goal) role switches across $(echo $lower_files | wc -w) lowerer(s) — check (a) is VACUOUS, HQ re-spec owed."
else
  bad "no lowerers found at $LOWER_GLOB"
fi

# --- EMITTER (a): no duplicated case IR_ label in emit.cpp (one dispatch case per kind) ---
# PER-SWITCH, not per-file (s169 seat1): emit.cpp now holds 26 switches, so a kind legitimately appears
# once per switch (IR_ACTIVATE is in walk_bb_node_inner AND emit_drive). The old flat
# `grep | sort | uniq -d` over the whole file flagged ~120 kinds the moment the path was corrected from the
# dead emit_core.c -- a FALSE RED. The rule is "one dispatch case per kind WITHIN one switch"; brace-depth
# scoping is what enforces it.
if [ -f "$EMIT" ]; then
  # Comments are stripped, then every `{`, `}` and `case IR_X` is put on its own line so a ONE-LINE switch
  # -- `switch (o) { case IR_MATCH_LIT: ... }`, of which emit.cpp has several -- scopes exactly like a
  # multi-line one. Without that normalisation a one-line switch leaks its labels into the enclosing scope
  # and collides with the next one-line switch (measured: a false IR_MATCH_LIT hit, s169).
  # String literals are deliberately NOT stripped: emit.cpp contains the char literal '"' , and a
  # naive string-strip starts a phantom string there and swallows a real `}` (measured: 1349 open vs 1348
  # close, which silently blinded the whole check). The brace-balance assertion below is what makes that
  # safe -- if a literal ever does unbalance the cheap lexer, this check says so instead of passing.
  norm=$(perl -0777 -pe 's{/\*.*?\*/}{}gs; s{//[^\n]*}{}g' "$EMIT" \
    | sed -E 's/(case IR_[A-Z0-9_]+)/\n\1\n/g; s/\{/\n{\n/g; s/\}/\n}\n/g')
  ob=$(printf '%s\n' "$norm" | grep -c '^{$'); cb=$(printf '%s\n' "$norm" | grep -c '^}$')
  if [ "$ob" -ne "$cb" ]; then
    bad "$(basename "$EMIT"): brace-balance $ob open vs $cb close — the case-label scoper cannot be trusted; fix the lexer before believing this check."
  else
    edups=$(printf '%s\n' "$norm" | awk '
        { if ($0 ~ /switch[ \t]*\(/) pending=1 }
        /^\{$/ { depth++; if (pending) { sp++; sw[sp]=depth; pending=0 }; next }
        /^\}$/ { if (sp>0 && sw[sp]==depth) { for (k in seen) if (index(k, sp ":")==1) delete seen[k]; sp-- } depth--; next }
        /^case IR_[A-Z0-9_]+$/ {
          if (sp>0) { lbl=substr($0,6); key=sp ":" lbl; if (seen[key]++) print "  " lbl }
        }' | sort -u)
    [ -n "$edups" ] && bad "$(basename "$EMIT"): case IR_ label duplicated WITHIN a single switch:$(printf '%s' "$edups")"
  fi
else
  bad "emit.cpp not found at $EMIT"
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
  # RATCHET RE-BASED 8 -> 4 (s169 seat1, queue row gates-dead-paths): util_template_purity_audit.sh
  # was scanning src/templates TWICE (the reorg sed collapsed BB_ and XA_ onto one dir), so it printed
  # DOUBLE. The doubled 8 exactly matched this stale ceiling, so the ratchet never fired and hid its own
  # defect. TRUE count at s169 = 4 (bb_call, bb_call_write_slot, bb_define, bb_match_replace). May not grow.
  PURITY_BASELINE="${PURITY_BASELINE:-4}"
  if [ "$pc" -gt "$PURITY_BASELINE" ]; then
    bad "template purity REGRESSED: $pc side-effects outside templates > baseline $PURITY_BASELINE (see /tmp/_purity.out)"
  fi
fi

# --- (d) FACT RULE blocks byte-identical across the 3 GOAL files (awk first-match, not the buggy sed) ---
# CONSOLIDATION s169 (seat1, gates-dead-paths): GOAL-{SNOBOL4,ICON,PROLOG}-BB.md were consolidated into
# the three GOAL-*-100.md files, and the SHARED-LOWERER / ONE-DISPATCH FACT RULE blocks this check
# byte-compares did NOT survive into them (grep == 0 in all three). Check (d) is therefore UNRUNNABLE, not
# merely mis-pathed: repointing it at the -100 files would only move the failure. HQ decision owed --
# restore the blocks or retire the check. Set CONCURRENCY_SKIP_D=1 to silence until then.
goalfiles="GOAL-SNOBOL4-100.md GOAL-ICON-100.md GOAL-PROLOG-100.md"
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
if [ "${CONCURRENCY_SKIP_D:-0}" = "1" ]; then
  say "  NOTE (d): FACT-RULE block comparison SKIPPED (CONCURRENCY_SKIP_D=1) — blocks lost in the -100 consolidation."
else
  check_block 'SHARED-LOWERER ONE-FILE' 'prove_lower.sh green' 'LOWER FACT RULE'
  check_block 'ONE-DISPATCH CONCURRENCY .FACT RULE' 'util_template_purity_audit.sh clean' 'EMITTER FACT RULE'
fi

[ "$fail" -eq 0 ] && say "OK: concurrency invariants hold (LOWER one-per-role, EMITTER one-dispatch, no stray bytes, FACT RULES byte-identical x3)."
exit "$fail"
