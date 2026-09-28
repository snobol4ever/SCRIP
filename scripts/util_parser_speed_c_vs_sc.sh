#!/usr/bin/env bash
# util_parser_speed_c_vs_sc.sh [sample|corpus] [lang ...] -- PARSE SPEED, THE C PARSERS AGAINST THE SELF-HOSTED .sc PARSERS ON THE
# SAME INPUT (Lon 2026-09-27: "Compare the speed of PARSING the entire set of programs for each language in the corpus repo (i.e.
# one big huge input concat by Unix cat command) with Snocone running parser.sc program in SCRIP versus stand-alone SCRIP parser
# ... I want to claim that the Snocone parsers are FASTER than the C and Bison/flex parsers."). A MEASUREMENT, not the claim:
# published only on the ceo's word (CEO-1340), WORK basis, RT_OPT named.
# Three columns per language, wall clock in seconds on the cat: C = out/parser_<lang>; sc-m3 = the .sc chain run through scrip
# --run (the chain's compile INCLUDED, as a user runs it); sc-m4 = the .sc chain compiled ONCE to a native binary (scrip --compile +
# gcc against out/libscrip_rt.so), the run alone timed. The population is the tree gate's: benchmarks/<lang> (sample) or every
# corpus program of the language (corpus). Where a parser refuses or stops early (SNOBOL4's .sc driver reads to the first END) the
# cell says so instead of a number. Prints the C/.sc dump line counts beside the times so a fast refusal cannot read as a win.
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"; S4E="${S4E_HOME:-$(cd "$ROOT/.." && pwd)}"
SCRIP="$ROOT/scrip"; CORPUS="${CORPUS:-$S4E/corpus}"; LIBDIR="$ROOT/out"
POP="${1:-sample}"; shift || true; LANGS="${*:-snobol4 snocone icon prolog rebus pascal raku}"
B="$ROOT/bootstrap"; CHAIN="$B/global.sc $B/case.sc $B/assign.sc $B/match.sc $B/counter.sc $B/stack.sc $B/tree.sc $B/ShiftReduce.sc $B/tdump.sc $B/gen.sc $B/qize.sc $B/semantic.sc $B/omega.sc $B/trace.sc"
declare -A EXT=([snobol4]=sno [snocone]=sc [icon]=icn [prolog]=pl [rebus]=reb [raku]=raku [pascal]=pas)
TMO="${SPEED_TIMEOUT:-7200}"
T=$(mktemp -d) || exit 2; trap 'rm -rf "$T"' EXIT
now() { date +%s.%N; }
elapsed() { awk -v a="$1" -v b="$2" 'BEGIN { printf "%.3f", b - a }'; }
echo "PARSER SPEED C vs .sc -- population=$POP, RT_OPT=$(grep -m1 '^RT_OPT' "$ROOT/Makefile" | sed 's/#.*//' | cut -d= -f2- | xargs), tree $(git -C "$ROOT" rev-parse --short HEAD), load $(cut -d' ' -f1-3 /proc/loadavg)"
printf '%-8s %6s %10s | %9s %8s | %9s %8s | %9s %8s\n' lang files bytes "C s" lines "sc-m3 s" lines "sc-m4 s" lines
for L in $LANGS; do
    ext=${EXT[$L]}; exe="$ROOT/out/parser_$L"; [ -x "$exe" ] || { echo "$L: no $exe -- make parsers"; continue; }
    if [ "$POP" = corpus ]; then find "$CORPUS" -type f -name "*.$ext" -not -name 'ALL.*' -not -path '*/.git/*' -not -path "$CORPUS/library/*" | sort > "$T/$L.files"
    else ls "$CORPUS/benchmarks/$L"/*."$ext" 2>/dev/null | sort > "$T/$L.files"; fi
    nf=$(wc -l < "$T/$L.files"); xargs -d '\n' cat < "$T/$L.files" > "$T/$L.cat"; nb=$(wc -c < "$T/$L.cat")
    cat $CHAIN "$B/parser_$L.sc" > "$T/$L.chain.sc"
    t0=$(now); ( cd "$CORPUS/benchmarks/$L" 2>/dev/null || cd "$CORPUS"; SNO_LIB="$CORPUS/include" timeout "$TMO" "$exe" - < "$T/$L.cat" > "$T/$L.c.dump" 2>/dev/null ); crc=$?; tc=$(elapsed "$t0" "$(now)")
    [ $crc -eq 124 ] && tc="timeout"; cl=$(wc -l < "$T/$L.c.dump"); grep -q '^Parse Error' "$T/$L.c.dump" && cl="$cl(refused)"
    t0=$(now); timeout "$TMO" "$SCRIP" -s4096m -d16384m "$T/$L.chain.sc" < "$T/$L.cat" > "$T/$L.m3.dump" 2>/dev/null; src=$?; t3=$(elapsed "$t0" "$(now)")
    [ $src -eq 124 ] && t3="timeout"; l3=$(wc -l < "$T/$L.m3.dump"); grep -q '^Parse Error' "$T/$L.m3.dump" && l3="$l3(refused)"
    if "$SCRIP" --compile "$T/$L.chain.sc" -o "$T/$L.s" </dev/null >/dev/null 2>&1 && gcc -m64 -no-pie -rdynamic "$T/$L.s" -Wl,-rpath,"$LIBDIR" -L"$LIBDIR" -lscrip_rt -lm -lpthread -o "$T/$L.bin" 2>/dev/null; then
        t0=$(now); timeout "$TMO" "$T/$L.bin" -s4096m -d16384m < "$T/$L.cat" > "$T/$L.m4.dump" 2>/dev/null; s4=$?; t4=$(elapsed "$t0" "$(now)")
        [ $s4 -eq 124 ] && t4="timeout"; l4=$(wc -l < "$T/$L.m4.dump"); grep -q '^Parse Error' "$T/$L.m4.dump" && l4="$l4(refused)"
    else t4="no-m4"; l4="-"; fi
    note=""; [ "$L" = snobol4 ] && note="  (.sc driver reads to the first END: one program of $nf)"; [ "$L" = raku ] && note="  (parser_raku.sc: recognizer, no tree; frozen)"
    printf '%-8s %6d %10d | %9s %8s | %9s %8s | %9s %8s%s\n' "$L" "$nf" "$nb" "$tc" "$cl" "$t3" "$l3" "$t4" "$l4" "$note"
done
