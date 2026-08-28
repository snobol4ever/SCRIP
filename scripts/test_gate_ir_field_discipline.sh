#!/usr/bin/env bash
# test_gate_ir_field_discipline.sh — IR_t FIELD-DISCIPLINE gate for GOAL-IR-IMMUTABLE-EMIT.md.
# JCON discipline: each IR_t value field holds ONE kind of thing. A field NEVER smuggles a pointer
# (children live in operands[] / their own graph, never in an int64 via (intptr_t)); dval is a REAL
# LITERAL, never a discriminant tag (subtypes are op values, per JCON's distinct records). This gate
# is the FORCING FUNCTION: once counter is deleted and {sval,ival,dval} become a union, the type
# system itself forbids these; until then this gate counts the debt and FAILS if it grows. The per-
# language LOWER sessions rebuilt after the field-whack MUST keep this at/under the TARGET — there is
# NO avenue to re-smuggle. Ratchet TARGET down to 0 as constructs convert to the flat operands+edge model.
set -u
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
SRC="$ROOT/src"
# A missing src/ must REFUSE, not silently score zero violations. LIVE() below runs `find "$SRC"
# ...` with no `|| true` guard of its own -- find's "No such file or directory" goes to stderr and
# the loop below it just sees zero lines, indistinguishable from a clean scan of a real tree.
[ -d "$SRC" ] || { echo "UNPROVEN: $SRC missing"; exit 2; }
strip() { sed -E ':a;s@/\*[^*]*\*+([^/*][^*]*\*+)*/@@;ta'; }
LIVE() { find "$SRC" -name '*.c' -o -name '*.cpp' -o -name '*.h' | grep -v '/attic/'; }

# Per-pattern scan over all live (non-attic) src; per-file breakdown to STDERR, numeric total to STDOUT.
scan() {
  local pat="$1" total=0 n f
  while IFS= read -r f; do
    [ -f "$f" ] || continue
    n=$(strip < "$f" | grep -aEc "$pat" 2>/dev/null); n=${n:-0}
    [ "$n" -gt 0 ] && printf "    %-44s %s\n" "${f#$SRC/}" "$n" >&2
    total=$((total + n))
  done < <(LIVE)
  echo "$total"
}

echo "=== IR_t FIELD-DISCIPLINE gate (JCON: one meaning per field, children in operands[], dval is a real literal) ==="

echo "[P1] pointer smuggled THROUGH an IR value field ( .ival/.counter/.dval/.sval = (...)(intptr_t) ):"
P1W=$(scan 'IR_(LIT|EXEC)\([^)]*\)\.(ival|counter|dval|sval)[[:space:]]*=[[:space:]]*\([^;]*\(u?intptr_t\)')
echo "[P2] pointer recovered FROM an IR value field ( (IR_graph_t* ...)(intptr_t) IR_LIT/EXEC(...).field ):"
P2R=$(scan '\(IR_graph_t[^)]*\)[[:space:]]*\(u?intptr_t\)[[:space:]]*IR_(LIT|EXEC)\([^)]*\)\.(ival|counter|dval|sval)')
echo "[P3] dval used as a DISCRIMINANT TAG ( IR_LIT(...).dval (==|!=|>=|<=|>|<|=) <float literal> ):"
P3T=$(scan 'IR_LIT\([^)]*\)\.dval[[:space:]]*(==|!=|>=|<=|>|<|=)[[:space:]]*[0-9]+\.[0-9]+')

SMUGGLE=$((P1W + P2R))
HARD=$((SMUGGLE + P3T))
echo "-------------------------------------------------------------------"
echo "P1 write-smuggle=$P1W  P2 read-smuggle=$P2R  (pointer-through-field=$SMUGGLE)   P3 dval-tag=$P3T"
echo "HARD TOTAL = $HARD   (target 0 — the ival/dval/sval union + counter deletion)"

# TARGET ratchets down as constructs convert. Set to current debt to lock against growth; lower on each rung.
TARGET="${IR_FIELD_DISCIPLINE_TARGET:-119}"
if [ "$HARD" -le "$TARGET" ]; then
  echo "PASS (<= TARGET=$TARGET): discipline held — no NEW field overloading. Ratchet TARGET toward 0 as constructs convert."
  exit 0
fi
echo "FAIL: $HARD > TARGET=$TARGET — a field overload was re-introduced. Children go in operands[]; subtypes are op values; dval is a real literal."
exit 1
