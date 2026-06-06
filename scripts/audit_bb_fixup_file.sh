#!/usr/bin/env bash
# audit_bb_fixup_file.sh — per-file TEMPLATE SPEC v2 + FACT RULES checker for GOAL-BB-FIXUP.md
# Usage: bash scripts/audit_bb_fixup_file.sh <file>
# rc=0: file is clean (all counts zero); rc=1: violations found (counts printed).
# Checks (comments stripped via perl unless noted):
#   emit_blind    — pBB->[αβγω] direct neighbor dereferences (EMIT-BLIND)
#   neighbor_walk — local-alias neighbor walks: lf->t / fin->.*->t / a0->t / a1->t (EMIT-BLIND)
#   bsize         — b.size( (hand-encoded byte-size arithmetic)
#   raw_bytes     — x86_Lrec|x86_Jrec|x86_Drec|x86_b[123]( |bytes(|u8(|u32le|u64le
#   medium_branch — IF(MEDIUM_BINARY|IF(MEDIUM_MACRO_DEF (instruction pairs instead of x86())
#   emit_fmt      — emit_fmt( (format string instead of inline literal)
#   line_comments — // line comments (no C comments allowed per SPEC)
#   blank_lines   — blank lines in source (raw, not stripped)
#   port_english  — PORT_ALPHA|PORT_BETA|PORT_GAMMA|PORT_OMEGA (must use α β γ ω)
#   local_vars    — local variable declarations inside *_str() body (no locals per SPEC)
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
cd "$ROOT"
if [ $# -ne 1 ]; then
    echo "Usage: $0 <bb_*.cpp file>" >&2
    exit 2
fi
f="$1"
if [ ! -f "$f" ]; then
    echo "ERROR: file not found: $f" >&2
    exit 2
fi
strip() { perl -0777 -pe 's{/\*.*?\*/}{}gs; s{//[^\n]*}{}g' "$1"; }
emit_blind=$(strip "$f"    | grep -cE 'pBB->[αβγω]' || true)
neighbor_walk=$(strip "$f" | grep -cE '\b(lf|fin|a0|a1|arm|node)->(α|β|γ|ω|t)\b' || true)
bsize=$(strip "$f"         | grep -cE 'b\.size\(' || true)
raw_bytes=$(strip "$f"     | grep -cE 'x86_Lrec|x86_Jrec|x86_Drec|x86_b[123]\(|bytes\(|u8\(|u32le|u64le' || true)
medium_branch=$(strip "$f" | grep -cE 'IF\(MEDIUM_BINARY|IF\(MEDIUM_MACRO_DEF' || true)
emit_fmt=$(strip "$f"      | grep -cE 'emit_fmt\(' || true)
line_comments=$(grep -cE '^\s*//' "$f" || true)
blank_lines=$(grep -c '^[[:space:]]*$' "$f" || true)
port_english=$(strip "$f"  | grep -cE 'PORT_ALPHA|PORT_BETA|PORT_GAMMA|PORT_OMEGA' || true)
local_vars=$(strip "$f"    | grep -cE '^\s+(int|long|size_t|std::string|const char \*|IR_t \*|void \*|uint64_t|int64_t)[[:space:]]' || true)
total=$((emit_blind + neighbor_walk + bsize + raw_bytes + medium_branch + emit_fmt + line_comments + blank_lines + port_english + local_vars))
name="$(basename "$f")"
echo "=== audit_bb_fixup_file: $name ==="
printf "  emit_blind    (pBB->[αβγω]):          %d\n" "$emit_blind"
printf "  neighbor_walk (alias->t):              %d\n" "$neighbor_walk"
printf "  bsize         (b.size():              %d\n" "$bsize"
printf "  raw_bytes     (bytes(/u8/u32le/etc):  %d\n" "$raw_bytes"
printf "  medium_branch (IF(MEDIUM_BINARY...):  %d\n" "$medium_branch"
printf "  emit_fmt      (emit_fmt():            %d\n" "$emit_fmt"
printf "  line_comments (// ...):               %d\n" "$line_comments"
printf "  blank_lines   (empty lines):          %d\n" "$blank_lines"
printf "  port_english  (PORT_ALPHA/etc):       %d\n" "$port_english"
printf "  local_vars    (decls in _str body):   %d\n" "$local_vars"
echo "  ---"
printf "  TOTAL violations:                     %d\n" "$total"
if [ "$total" -eq 0 ]; then
    echo "CLEAN: $name"
    exit 0
else
    echo "DIRTY: $name  ($total violations)"
    exit 1
fi
