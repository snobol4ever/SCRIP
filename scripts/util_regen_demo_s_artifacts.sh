#!/usr/bin/env bash
# util_regen_demo_s_artifacts.sh — regenerate x64 demo .s artifacts and commit to corpus.
# GRACEFUL-SKIP (harmonized with benchmark/feature regen, session 10): emit to a temp file,
# gcc -c it, and only mv over the committed .s on assembler-accept. A program whose --compile
# crashes/empties or whose .s is assembler-rejected leaves its last-good committed .s UNTOUCHED
# and is flagged — NEVER truncate-then-fail. scrip --compile is deterministic, so an unchanged
# compiler yields no mv and no commit (idempotent). The .s is the honest current output, never pinned.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP:-$ROOT/scrip}"
CORPUS="${CORPUS:-$S4E/corpus}"
DEMO="$CORPUS/demos"

if [ ! -x "$SCRIP" ]; then echo "SKIP  scrip not found: $SCRIP"; exit 0; fi
if [ ! -d "$DEMO" ]; then echo "SKIP  corpus demo dir not found: $DEMO"; exit 0; fi
TMPD="$(mktemp -d)"; trap 'rm -rf "$TMPD"' EXIT   # PER-INVOCATION scratch: /tmp/demo_<name>.* was program-keyed and /tmp/demo_as_err.txt outright fixed, so two seat roots regenerating at once collided (s169 seat2 FINDING §7.5)

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
# ⛔⭐⭐ RESOLVE EACH NAME TO ITS PATH -- THE FLAT `$f.sno` LOOKUP THIS REPLACED HAD BEEN SILENTLY
# REGENERATING NOTHING SINCE THE s272 CORPUS RE-GRID (hq_P, 2026-08-27).  The re-grid moved demos from
# corpus/demos/<name>.sno to corpus/demos/snobol4/<family>/<name>.sno.  This loop cd'd to corpus/demos and
# tested `[ -f "$f.sno" ]`, so after the move EVERY one of the 21 sanctioned names failed that test, took
# the `continue`, and the script then printed "No changes -- demo artifacts already current."
# ⛔ THAT IS A FALSE GREEN, NOT A GAP: the script reported SUCCESS while doing NOTHING, so the handoff rule
# "if the session touched codegen, regenerate the .s artifacts" has been a no-op for every seat since the
# re-grid, and the committed demo .s files silently froze while the compiler moved underneath them -- the
# exact hello.s fossil this script's own header was written to prevent, reintroduced by a path change.
# MEASURED at the time of the fix: a fresh --compile of calculator-1-match differs from its committed .s
# by 1,749 diff lines, none of them from tonight's cure (it is another seat's label-prefix change).
# ⭐ THE CLASS, and it is the third instance this month: A GUARD OR A LOOKUP KEYED ON A PATH IS NOT KEYED
# ON THE THING, IT IS KEYED ON A COINCIDENCE THAT A REORGANISATION CAN END -- same shape as the inert
# */programs/lon/* guards in util_oracle_flag_sweep.sh and test_gate_argnote_sweep.sh.  Resolving by SEARCH
# and REFUSING on an unresolvable member ties the script to the FILE rather than to its address.
# ⛔ AND THE SKIP IS NOW A REFUSAL, per RULES.md: a tool that cannot do its job exits non-zero rather than
# reporting success.  These 21 names are the SANCTIONED SET -- a member that does not resolve is a real
# error (a renamed or deleted demo), never something to pass over quietly.
echo "Emitting + verifying demo .s (resolve-by-search; refuses on an unresolvable sanctioned name)..."
RESOLVE_RC=0
for f in $DEMOS; do
    hits="$(find "$DEMO" -name "$f.sno" -type f | sort)"
    n="$(printf '%s\n' "$hits" | grep -c . || true)"
    if [ "$n" -eq 0 ]; then
        echo "  ⛔ REFUSE  $f — no $f.sno anywhere under $DEMO (sanctioned set member unresolvable; the corpus moved or the demo was renamed/deleted)"; RESOLVE_RC=2; continue
    fi
    if [ "$n" -gt 1 ]; then
        echo "  ⛔ REFUSE  $f — $n files named $f.sno under $DEMO; ambiguous, not guessing:"; printf '      %s\n' $hits; RESOLVE_RC=2; continue
    fi
    src="$hits"; dst="${src%.sno}.s"
    tmp="$TMPD/demo_$f.s"
    # ⛔ COMPILE VIA A $DEMO-RELATIVE PATH (cwd is $DEMO), NEVER the absolute $src from find: since
    # perf-per-statement-loc-emission the compiler emits `.file 1 "<path as given>"`, so an absolute
    # path bakes THIS SEAT ROOT into a committed artifact and every other root regenerates it back --
    # a permanent churn war between /home/claude_P and /home/claude_C. The benchmark regen already
    # passes a bare basename; this is the same discipline. hq_P 2026-08-28.
    if ! timeout 90 "$SCRIP" --compile "${src#$DEMO/}" > "$tmp" 2>/dev/null; then
        echo "  SKIP  $f.s — --compile failed (committed .s untouched)"; continue
    fi
    if [ ! -s "$tmp" ]; then
        echo "  SKIP  $f.s — empty emit (committed .s untouched)"; continue
    fi
    if ! gcc -c "$tmp" -o "$TMPD/demo_$f.o" 2>"$TMPD/demo_as_err.txt"; then
        echo "  SKIP  $f.s — assembler-rejected (committed .s untouched)"; continue
    fi
    if [ -f "$dst" ] && cmp -s "$tmp" "$dst"; then
        echo "  same  ${dst#$CORPUS/}"
    else
        mv "$tmp" "$dst"; echo "  upd   ${dst#$CORPUS/}"
    fi
done
[ "$RESOLVE_RC" -eq 0 ] || { echo "⛔ GATE REFUSES: one or more sanctioned demo names did not resolve (see above). NOT reporting artifacts current -- that is the false green this refusal exists to stop."; exit "$RESOLVE_RC"; }

echo "Committing to corpus..."
cd "$CORPUS"
for f in $DEMOS; do
    d="$(find "$DEMO" -name "$f.s" -type f | sort | head -1)"
    [ -n "$d" ] && git add "${d#$CORPUS/}" 2>/dev/null || true
done
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
