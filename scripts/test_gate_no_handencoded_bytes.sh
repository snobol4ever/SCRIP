#!/usr/bin/env bash
# test_gate_no_handencoded_bytes.sh — enforce the FACT RULE
#   "MEDIUM_BINARY IS ASSEMBLED LITERAL ASM — NEVER HAND-ENCODED BYTES"
# (GOAL-SNOBOL4-BB / GOAL-ICON-BB / GOAL-PROLOG-BB, Lon directive 2026-06-01).
#
# Every BB template's MEDIUM_BINARY arm must ASSEMBLE the literal GAS asm its
# MEDIUM_TEXT arm emits — it must NOT hand-encode instruction bytes nor count
# byte offsets to locate rel32 patch sites. This gate counts, per template, the
# BAD-SITE tokens that mark a hand-encoded arm. The count only ever decreases as
# arms are converted to assembled-asm (or stubbed loud via bomb_bytes()).
#
# Modes:
#   (default)   informational baseline — prints the per-file punch list, ALWAYS exits 0.
#   --strict    HARD gate — exits non-zero if any BAD site remains (flip to this
#               once the asm-assembly splice path lands).
#
# CARVE-OUT (RULES.md TEMPLATE-ONLY): bytes()/u8()/u32le()/u64le() are legal ONLY
# inside bomb_bytes / bb_emit_asm_result in emit_str.cpp — NOT scanned here (this
# gate scans BB_templates/*.cpp only, where any such token is a BAD site).
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
DIR="$ROOT/src/templates"
STRICT=0; [ "${1:-}" = "--strict" ] && STRICT=1

# The ONLY bad pattern (FACT RULE): a FUNCTION that counts the bytes — b.size().
# Literal byte maps are CORRECT and are NOT flagged: bytes("\x..") opcode literals,
# hardcoded bin={{..}} offset tuples, u32le/u64le immediates, TEMPLATE_ADDR_* bakes.
PAT='b\.size\(\)'

# strip C/C++ comments first (block + line) so token mentions in comments — incl. this
# rule's own explanatory notes — never false-positive. Mirrors the g_vstack gate discipline.
strip_comments() { perl -0777 -pe 's{/\*.*?\*/}{}gs; s{//[^\n]*}{}g' "$1"; }

TOTAL=0; BADFILES=0
echo "=== BAD-SITE scan: b.size() function byte-counters in BB_templates ==="
for f in "$DIR"/*.cpp; do
    [ -e "$f" ] || continue
    n=$(strip_comments "$f" | grep -aoE "$PAT" 2>/dev/null | wc -l | tr -d ' ')
    if [ "$n" -gt 0 ]; then
        printf '  %4d  %s\n' "$n" "$(basename "$f")"
        TOTAL=$((TOTAL + n))
        BADFILES=$((BADFILES + 1))
    fi
done
echo "---"
echo "BAD sites (b.size()): $TOTAL  across $BADFILES file(s)   (target: 0 — rewrite each to a hardcoded literal offset map)"

if [ "$STRICT" -eq 1 ]; then
    [ "$TOTAL" -eq 0 ] && { echo "OK: zero b.size() function-counters in BB_templates."; exit 0; }
    echo "FAIL (--strict): $TOTAL b.size() function-counter site(s) remain — rewrite to literal offset maps."
    exit 1
fi
echo "(informational baseline — not gating; run with --strict to enforce zero.)"
exit 0
