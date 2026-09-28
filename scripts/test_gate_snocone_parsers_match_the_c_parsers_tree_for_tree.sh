#!/usr/bin/env bash
# test_gate_snocone_parsers_match_the_c_parsers_tree_for_tree.sh -- THE SELF-HOSTED PARSERS PRINT THE SAME TREE AS SCRIP'S OWN
# PARSERS, TREE FOR TREE (Lon 2026-09-27, in-chat to hq_snocone, verbatim: "Write a C function to dump the AST. Write an
# equivalent Snocone function to dump the tree. ... Get the trees to match 100%."; ceo CEO-1340, the row's DONE-WHEN).
#
# THE INSTRUMENT: out/parser_<lang> (src/tools/parser_main.c, one frontend per build, printing through ir_dump_tree in
# src/ir/ast_print.c) and the .sc chain + bootstrap/parser_<lang>.sc printing through TreeDump in bootstrap/tdump.sc -- ONE
# FORMAT, byte for byte: one node per line, two spaces per depth, "(TT_KIND value" then the children then ")"; strings quoted with
# the CQize escapes, integers decimal, reals as %g; the five bookkeeping attributes :line :lline :file :stno :src are printed by
# NEITHER side (the one named exclusion, the same list in both dumpers: the C --dump-ast omits them at top level too).
#
# POPULATION: per language, the SAMPLE per landing is the cat of corpus/benchmarks/<lang>/*.<ext> in sorted order; with
# GATE_POPULATION=corpus or under the bus's closure run (S4E_DONE_WHEN_RUN=1) it is the cat of EVERY corpus program of the
# language (every *.<ext> outside .git and corpus/library, not ALL.*), sorted -- the whole-corpus run is a closure run, one at a
# time (CEO-1341). The C parser reads the cat as ONE input (the SNOBOL4 main splits it at END lines itself); the .sc parser reads
# the same cat on scrip m3 at the largest arena. Raku is REPORTED, NOT GRADED: parser_raku.sc is a recognizer, frozen by Lon until
# hq_raku's parser is stable.
# VERDICT per language: files, bytes, the C dump's lines, the .sc dump's lines, MATCH or DIFF with the first differing line pair,
# or REFUSED naming which side refused. rc 0 = every graded language MATCH; rc 1 = a DIFF or a refusal on one side; rc 2 = could
# not measure (no binary, no chain, a timeout). RED BY MEASUREMENT on 2026-09-28: every graded language DIFFs (the five shape classes
# are in GOAL-SNOCONE-100.md's cursor); it turns green as Lon's canonical forms land in the .sc parsers.
"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
set -uo pipefail
G="$(basename "${BASH_SOURCE[0]}" .sh)"
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"; S4E="${S4E_HOME:-$(cd "$ROOT/.." && pwd)}"
SCRIP="${SCRIP_BIN:-$ROOT/scrip}"; [ -x "$SCRIP" ] || { echo "⛔ GATE REFUSE(2) [$G]: no scrip at $SCRIP"; exit 2; }
CORPUS="${CORPUS:-$S4E/corpus}"; [ -d "$CORPUS/tests" ] || { echo "⛔ GATE REFUSE(2) [$G]: no corpus at $CORPUS"; exit 2; }
B="$ROOT/bootstrap"; CHAIN="$B/global.sc $B/case.sc $B/assign.sc $B/match.sc $B/counter.sc $B/stack.sc $B/tree.sc $B/ShiftReduce.sc $B/tdump.sc $B/gen.sc $B/qize.sc $B/semantic.sc $B/omega.sc $B/trace.sc"
for f in $CHAIN; do [ -f "$f" ] || { echo "⛔ GATE REFUSE(2) [$G]: chain file missing: $f"; exit 2; }; done
POP="${GATE_POPULATION:-sample}"; [ "${S4E_DONE_WHEN_RUN:-}" = 1 ] && POP=corpus
TMO="${GATE_TIMEOUT:-600}"; [ "$POP" = corpus ] && TMO="${GATE_TIMEOUT:-7200}"
T=$(mktemp -d) || exit 2; trap 'rm -rf "$T"' EXIT
declare -A EXT=([snobol4]=sno [snocone]=sc [icon]=icn [prolog]=pl [rebus]=reb [raku]=raku [pascal]=pas)
RC=0; GRADED=0; MATCHED=0
echo "TREE-FOR-TREE [$G]: population=$POP, C parsers out/parser_<lang> vs the .sc chain on scrip m3 (-s4096m -d16384m), timeout ${TMO}s per side"
for L in snobol4 snocone icon prolog rebus pascal raku; do
    ext=${EXT[$L]}; exe="$ROOT/out/parser_$L"
    [ -x "$exe" ] || { echo "  $L: ⛔ REFUSE(2) no $exe -- make parsers"; RC=2; continue; }
    if [ "$POP" = corpus ]; then find "$CORPUS" -type f -name "*.$ext" -not -name 'ALL.*' -not -path '*/.git/*' -not -path "$CORPUS/library/*" | sort > "$T/$L.files"
    else ls "$CORPUS/benchmarks/$L"/*."$ext" 2>/dev/null | sort > "$T/$L.files"; fi
    nf=$(wc -l < "$T/$L.files"); [ "$nf" -gt 0 ] || { echo "  $L: ⛔ REFUSE(2) empty population ($POP)"; RC=2; continue; }
    xargs -d '\n' cat < "$T/$L.files" > "$T/$L.cat"; nb=$(wc -c < "$T/$L.cat")
    cat $CHAIN "$B/parser_$L.sc" > "$T/$L.chain.sc"
    ( cd "$CORPUS/benchmarks/$L" 2>/dev/null || cd "$CORPUS"; SNO_LIB="$CORPUS/include" timeout "$TMO" "$exe" - < "$T/$L.cat" > "$T/$L.c.dump" 2> "$T/$L.c.err" ); crc=$?
    timeout "$TMO" "$SCRIP" -s4096m -d16384m "$T/$L.chain.sc" < "$T/$L.cat" > "$T/$L.sc.dump" 2> "$T/$L.sc.err"; src=$?
    cl=$(wc -l < "$T/$L.c.dump"); sl=$(wc -l < "$T/$L.sc.dump")
    tag=""; [ "$L" = raku ] && tag=" (REPORTED, NOT GRADED: parser_raku.sc is a frozen recognizer)"
    if [ $crc -eq 124 ] || [ $src -eq 124 ]; then echo "  $L: ⛔ COULD NOT MEASURE(2) files=$nf bytes=$nb -- timeout ${TMO}s on $([ $crc -eq 124 ] && echo C)$([ $src -eq 124 ] && echo ' .sc')$tag"; [ "$L" = raku ] || RC=2; continue; fi
    if grep -q '^Parse Error' "$T/$L.c.dump" || [ $crc -ge 2 ] || grep -q '^Parse Error' "$T/$L.sc.dump"; then
        who=""; { grep -q '^Parse Error' "$T/$L.c.dump" || [ $crc -ge 2 ]; } && who="C(rc=$crc: $(head -1 "$T/$L.c.err" | cut -c1-80))"; grep -q '^Parse Error' "$T/$L.sc.dump" && who="$who .sc"
        echo "  $L: REFUSED files=$nf bytes=$nb C=$cl lines .sc=$sl lines -- refused by: $who$tag"; [ "$L" = raku ] || RC=1; continue; fi
    [ "$L" = raku ] || GRADED=$((GRADED + 1))
    if cmp -s "$T/$L.c.dump" "$T/$L.sc.dump"; then echo "  $L: MATCH files=$nf bytes=$nb lines=$cl$tag"; [ "$L" = raku ] || MATCHED=$((MATCHED + 1))
    else first=$(diff "$T/$L.c.dump" "$T/$L.sc.dump" | grep -m2 '^[<>]' | cut -c1-70 | tr '\n' ' '); echo "  $L: DIFF files=$nf bytes=$nb C=$cl lines .sc=$sl lines -- first: $first$tag"; [ "$L" = raku ] || RC=1; fi
done
[ $RC -eq 2 ] && { echo "⛔ GATE REFUSE(2) [$G]: a language could not be measured (see above)"; exit 2; }
if [ $RC -eq 0 ]; then echo "✅ GATE PASS(0) [$G]: $MATCHED/$GRADED graded languages print the same tree from the C parser and the .sc parser ($POP population)"
else echo "⛔ GATE FAIL(1) [$G]: $MATCHED/$GRADED graded languages match; the rest differ or refuse ($POP population) -- the shape classes are in the cursor"; fi
exit $RC
