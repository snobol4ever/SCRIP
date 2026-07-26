#!/usr/bin/env bash
# util_regen_demo_s_artifacts.sh — regenerate x64 demo .s artifacts and commit to corpus.
# GRACEFUL-SKIP (harmonized with benchmark/feature regen, session 10): emit to a temp file,
# gcc -c it, and only mv over the committed .s on assembler-accept. A program whose --compile
# crashes/empties or whose .s is assembler-rejected leaves its last-good committed .s UNTOUCHED
# and is flagged — NEVER truncate-then-fail. scrip --compile is deterministic, so an unchanged
# compiler yields no mv and no commit (idempotent). The .s is the honest current output, never pinned.
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP:-$ROOT/scrip}"
CORPUS="${CORPUS:-/home/claude/corpus}"
DEMO="$CORPUS/programs/snobol4/demo"

if [ ! -x "$SCRIP" ]; then echo "SKIP  scrip not found: $SCRIP"; exit 0; fi
if [ ! -d "$DEMO" ]; then echo "SKIP  corpus demo dir not found: $DEMO"; exit 0; fi

cd "$DEMO"
# THE SANCTIONED SET (widened 2026-07-26 by Lon directive: "get them all but porter stemmer can be excluded").
# Was the s151 five (roman wordcount claws5 treebank-list treebank-array) — which silently left hello.s and four
# other legacy artifacts frozen at 2026-06-25 while the compiler moved a month underneath them, and left the ten
# s158 working-set programs with no artifact at all.  PORTER IS DELIBERATELY ABSENT: its emit is ~37k lines
# (mostly stemmer pattern tables) and would churn that on every codegen commit; it compiles and assembles fine,
# so re-add it here if the churn is ever wanted.  Every name below is verified compile-clean + assembler-accepted.
DEMOS="roman wordcount claws5 treebank-list treebank-array \
       arithmetic counter hello pattern_test expression \
       calculator-1 calculator-1-match calculator-1-match-fence \
       calculator-2 calculator-2-match calculator-2-match-fence \
       claws5-match claws5-match-fence \
       json json-match json-match-fence \
       treebank-match treebank-match-fence"
echo "Emitting + verifying demo .s (graceful-skip)..."
for f in $DEMOS; do
    [ -f "$f.sno" ] || { echo "  SKIP  $f — no .sno"; continue; }
    tmp="/tmp/demo_$f.s"
    if ! timeout 90 "$SCRIP" --compile "$f.sno" > "$tmp" 2>/dev/null; then
        echo "  SKIP  $f.s — --compile failed (committed .s untouched)"; continue
    fi
    if [ ! -s "$tmp" ]; then
        echo "  SKIP  $f.s — empty emit (committed .s untouched)"; continue
    fi
    if ! gcc -c "$tmp" -o "/tmp/demo_$f.o" 2>/tmp/demo_as_err.txt; then
        echo "  SKIP  $f.s — assembler-rejected (committed .s untouched)"; continue
    fi
    if [ -f "$f.s" ] && cmp -s "$tmp" "$f.s"; then
        echo "  same  $f.s"
    else
        mv "$tmp" "$f.s"; echo "  upd   $f.s"
    fi
done

echo "Committing to corpus..."
cd "$CORPUS"
for f in $DEMOS; do git add "programs/snobol4/demo/$f.s" 2>/dev/null || true; done
if git diff --cached --quiet; then
    echo "  No changes — demo artifacts already current."
else
    RUNG="${1:-regen}"
    git commit -m "x64 demo artifacts: $RUNG"
    echo "  Committed."
fi
echo "Done."
