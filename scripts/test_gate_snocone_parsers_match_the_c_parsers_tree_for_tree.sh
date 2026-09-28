#!/usr/bin/env bash
# test_gate_snocone_parsers_match_the_c_parsers_tree_for_tree.sh -- THE SELF-HOSTED PARSERS PRINT THE SAME TREE AS SCRIP'S OWN
# PARSERS, FILE BY FILE (Lon 2026-09-27, in-chat to hq_snocone, verbatim: "Write a C function to dump the AST. Write an
# equivalent Snocone function to dump the tree. ... Get the trees to match 100%."; and 2026-09-28: "So change from concat one big
# file to run each parser seperatly."; ceo CEO-1340, the row's DONE-WHEN).
#
# THE INSTRUMENT: out/parser_<lang> (src/tools/parser_main.c, one frontend per build, printing through ir_dump_tree in
# src/ir/ast_print.c) and the .sc chain + bootstrap/parser_<lang>.sc printing through TreeDump in bootstrap/tdump.sc -- ONE
# FORMAT, byte for byte: one node per line, two spaces per depth, "(TT_KIND value" then the children then ")"; strings quoted with
# the CQize escapes, integers decimal, reals as %g; the five bookkeeping attributes :line :lline :file :stno :src are printed by
# NEITHER side (the one named exclusion, the same list in both dumpers). Each .sc parser reads its whole input in ONE read (Lon
# 2026-09-28: INPUT(.INPUT, 9, '[-f0 -r16777215]') then Src = INPUT) and matches it with ONE POS(0) ... RPOS(0) pattern.
#
# POPULATION, EACH FILE ALONE: per landing the files of corpus/benchmarks/<lang>; with GATE_POPULATION=corpus or under the bus's
# closure run (S4E_DONE_WHEN_RUN=1) every corpus program of the language (every *.<ext> outside .git and corpus/library, not
# ALL.*) -- the whole-corpus run is a closure run, one at a time (CEO-1341). Each file goes to the C parser and to the .sc parser
# compiled ONCE per language to a mode-4 binary (Lon 2026-09-28: "forget mode 3 runs."), run at the largest arena on the file's own
# stdin; the two dumps are compared for THAT file. Raku is not graded (parser_raku.sc is
# a recognizer, frozen by Lon until hq_raku's parser is stable); util_parser_speed_c_vs_sc.sh times it.
# VERDICT per language: files, MATCH, DIFF, REFUSED (by C, by .sc, by both), COULD-NOT-MEASURE (a timeout), and the first DIFF
# file with its first differing line pair. rc 0 = every file of every graded language MATCHes; rc 1 = a DIFF or a one-sided
# refusal; rc 2 = could not measure. A file BOTH parsers refuse counts as agreement (neither built a tree), printed apart.
# RED BY MEASUREMENT on 2026-09-28: the .sc and C trees differ in five shape classes (GOAL-SNOCONE-100.md cursor 2026-09-28b)
# until Lon's canonical forms land in the .sc parsers.
"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
set -uo pipefail
G="$(basename "${BASH_SOURCE[0]}" .sh)"
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"; S4E="${S4E_HOME:-$(cd "$ROOT/.." && pwd)}"
SCRIP="${SCRIP_BIN:-$ROOT/scrip}"; [ -x "$SCRIP" ] || { echo "⛔ GATE REFUSE(2) [$G]: no scrip at $SCRIP"; exit 2; }
CORPUS="${CORPUS:-$S4E/corpus}"; [ -d "$CORPUS/tests" ] || { echo "⛔ GATE REFUSE(2) [$G]: no corpus at $CORPUS"; exit 2; }
B="$ROOT/bootstrap"; CHAIN="$B/global.sc $B/case.sc $B/assign.sc $B/match.sc $B/counter.sc $B/stack.sc $B/tree.sc $B/ShiftReduce.sc $B/tdump.sc $B/gen.sc $B/qize.sc $B/semantic.sc $B/omega.sc $B/trace.sc"
for f in $CHAIN; do [ -f "$f" ] || { echo "⛔ GATE REFUSE(2) [$G]: chain file missing: $f"; exit 2; }; done
POP="${GATE_POPULATION:-sample}"; [ "${S4E_DONE_WHEN_RUN:-}" = 1 ] && POP=corpus
TMO="${GATE_TIMEOUT:-300}"
T=$(mktemp -d) || exit 2; trap 'rm -rf "$T"' EXIT
declare -A EXT=([snobol4]=sno [snocone]=sc [icon]=icn [prolog]=pl [rebus]=reb [pascal]=pas)
refused() { grep -q '^Parse Error' "$1"; }
RC=0; GRADED=0; GREEN=0
echo "TREE-FOR-TREE [$G]: population=$POP, EACH FILE ALONE through out/parser_<lang> and the .sc chain compiled once to a mode-4 binary (-s4096m -d16384m), timeout ${TMO}s per run"
for L in snobol4 snocone icon prolog rebus pascal; do
    ext=${EXT[$L]}; exe="$ROOT/out/parser_$L"
    [ -x "$exe" ] || { echo "  $L: ⛔ REFUSE(2) no $exe -- make parsers"; RC=2; continue; }
    if [ "$POP" = corpus ]; then find "$CORPUS" -type f -name "*.$ext" -not -name 'ALL.*' -not -path '*/.git/*' -not -path "$CORPUS/library/*" | sort > "$T/$L.files"
    else ls "$CORPUS/benchmarks/$L"/*."$ext" 2>/dev/null | sort > "$T/$L.files"; fi
    nf=$(wc -l < "$T/$L.files"); [ "$nf" -gt 0 ] || { echo "  $L: ⛔ REFUSE(2) empty population ($POP)"; RC=2; continue; }
    cat $CHAIN "$B/parser_$L.sc" > "$T/$L.chain.sc"
    "$SCRIP" --compile "$T/$L.chain.sc" -o "$T/$L.s" </dev/null >/dev/null 2>&1 && gcc -m64 -no-pie -rdynamic "$T/$L.s" -Wl,-rpath,"$ROOT/out" -L"$ROOT/out" -lscrip_rt -lm -lpthread -o "$T/$L.bin" 2>/dev/null \
        || { echo "  $L: ⛔ REFUSE(2) the .sc chain did not compile or link to a mode-4 binary"; RC=2; continue; }
    m=0; d=0; rc_=0; rs=0; rb=0; u=0; first=""
    while IFS= read -r f; do
        ( cd "$(dirname "$f")"; SNO_LIB="$CORPUS/include" timeout "$TMO" "$exe" - < "$f" > "$T/c.dump" 2>/dev/null ); crc=$?
        timeout "$TMO" "$T/$L.bin" -s4096m -d16384m < "$f" > "$T/s.dump" 2>/dev/null; src=$?
        if [ $crc -eq 124 ] || [ $src -eq 124 ]; then u=$((u + 1)); continue; fi
        cr=0; sr=0; { refused "$T/c.dump" || [ $crc -ge 2 ]; } && cr=1; refused "$T/s.dump" && sr=1
        if [ $cr = 1 ] && [ $sr = 1 ]; then rb=$((rb + 1))
        elif [ $cr = 1 ]; then rc_=$((rc_ + 1)); [ -n "$first" ] || first="C refused ${f#$CORPUS/}"
        elif [ $sr = 1 ]; then rs=$((rs + 1)); [ -n "$first" ] || first=".sc refused ${f#$CORPUS/}"
        elif cmp -s "$T/c.dump" "$T/s.dump"; then m=$((m + 1))
        else d=$((d + 1)); [ -n "$first" ] || first="DIFF ${f#$CORPUS/}: $(diff "$T/c.dump" "$T/s.dump" | grep -m2 '^[<>]' | cut -c1-60 | tr '\n' ' ')"; fi
    done < "$T/$L.files"
    GRADED=$((GRADED + 1))
    verdict=MATCH; [ $d -gt 0 ] || [ $rc_ -gt 0 ] || [ $rs -gt 0 ] && verdict=RED; [ $u -gt 0 ] && verdict="COULD-NOT-MEASURE($u)"
    echo "  $L: $verdict files=$nf MATCH=$m DIFF=$d REFUSED-by-C=$rc_ REFUSED-by-.sc=$rs refused-by-both=$rb timeout=$u${first:+ -- first: $first}"
    case "$verdict" in MATCH) GREEN=$((GREEN + 1)) ;; RED) [ $RC -eq 2 ] || RC=1 ;; *) RC=2 ;; esac
done
echo "  raku: not graded (parser_raku.sc is a recognizer, frozen by Lon until hq_raku's parser is stable)"
[ $RC -eq 2 ] && { echo "⛔ GATE REFUSE(2) [$G]: a language could not be measured (see above)"; exit 2; }
if [ $RC -eq 0 ]; then echo "✅ GATE PASS(0) [$G]: $GREEN/$GRADED graded languages print the same tree from the C parser and the .sc parser on every file ($POP population)"
else echo "⛔ GATE FAIL(1) [$G]: $GREEN/$GRADED graded languages match on every file ($POP population) -- the shape classes are in the cursor"; fi
exit $RC
