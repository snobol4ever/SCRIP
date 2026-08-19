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
# THE SANCTIONED SET (widened 2026-07-26 by Lon directive: "get them all but porter stemmer can be excluded";
# narrowed same day: "exclude expression.s also — we do not want the huge ones").
# Was the s151 five (roman wordcount claws5 treebank-list treebank-array) — which silently left hello.s and four
# other legacy artifacts frozen at 2026-06-25 while the compiler moved a month underneath them, and left the ten
# s158 working-set programs with no artifact at all.  TWO PROGRAMS ARE DELIBERATELY ABSENT ON SIZE, NOT ON
# HEALTH — porter (~37k lines of stemmer pattern tables) and expression (~36k) both compile clean and assemble
# clean; they are out because committing them churns ~73k lines on every codegen commit.  Their .s files are
# NOT left behind stale: a name dropped from this list must have its artifact deleted in the same commit, which
# is the whole lesson of the hello.s fossil.  Re-add a name here if the churn is ever wanted.  Largest member
# now is json at ~18k; that is the standing ceiling unless Lon rules otherwise.
DEMOS="roman wordcount claws5 treebank \
       arithmetic counter hello pattern_test \
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
    if git -c user.name="LCherryholmes" -c user.email="lcherryh@yahoo.com" \
           commit -q -m "x64 demo artifacts: $RUNG"; then
        echo "  Committed:"; git show --stat --oneline HEAD | sed -n '1,40p'
    else
        echo "  COMMIT FAILED — artifacts regenerated and STAGED but NOT committed."; exit 1
    fi
fi
echo "Done."
