#!/usr/bin/env bash
# util_parser_grid.sh [lang ...] -- THE PARSER GRID: THE STAND-ALONE C PARSER, SCRIP AND SPITBOL ON THE PARSE CLOCK, ONE FILE LIST
# FOR ALL THREE (Lon 2026-09-29, in-chat to the ceo, verbatim: "The goal is parser.sc same as C and 2x faster than SPITBOL." and "We
# want focus on accuracy and speed of parser.sc as prep for replacing all SCRIP parsers."; MODE QUARTET, CEO-1364; the coo's row
# parser-sc-speed-grid-c-scrip-spitbol-on-the-parse-clock-lon-2026-09-29-ceo-1364, committed from the ceo's scratch pcmp.sh, which
# is .github/wip-patches/pcmp-parsers-vs-clean-sbl-2026-09-29.sh). A MEASUREMENT: it writes no row, no score cell, no progress append.
#
# THE BAR (the row's): SCRIP's parse clock equal to C's (C/SCRIP >= 1.00) and SPITBOL/SCRIP >= 2.00 on every parser.
#
# LANGUAGES: the arguments, else every bootstrap/parser_<lang>.sc whose driver reads PARSER_FILES (six today; Raku joins by itself
# when the rewritten parser_raku.sc prints trees through that driver). A language named whose driver does not read the list REFUSES.
# THE FILE LIST, ONE FOR ALL THREE ENGINES, DERIVED HERE AND PRINTED WITH ITS COUNT: every corpus program of the language (the tree
# gate's corpus population: *.<ext> outside .git and corpus/library, not ALL.*), less each file that ENDS an engine's run before
# its PARSER-METRICS line. Measured on 2026-09-29: the C Icon parser exits the process on a link it cannot open, and SPITBOL stops on
# a run-time error in the parser program (field function argument is wrong datatype) -- either way every later file goes unparsed.
# Each engine runs the list; when a run dies, the files from the last "== <file>" it printed are run ALONE until one dies alone,
# that file leaves the list and the scan resumes after it (a buffered engine can die past the last header it flushed; a file that
# dies only in company leaves as the last one begun). The count each engine dropped is printed beside the list; GRID_OUT keeps names.
# THE THREE ENGINES, each reading PARSER_FILES=<list> and printing "== <file>" then the file's tree, and one line
# PARSER-METRICS files= bytes= parse_first_us= parse_us= whose parse_us is the clock:
#   C      out/parser_<lang> (src/tools/parser_main.c): times only its parser -- no lowering, no I/O; a link or include is resolved
#          outside the clock (IPATH names the corpus's IPL procs, since the parser looks beside its own binary, which is out/).
#   SCRIP  the bootstrap chain + bootstrap/parser_<lang>.sc compiled ONCE to a mode-4 binary; the driver times only Init*() +
#          Src ? Compiland + Pop() -- reading a file and printing its tree are outside the clock.
#   SBL    sbl_clean_bin -bf (the clean benchmark oracle, never the monitor-hooked x64 fork: GOAL-SNOCONE-100 cursor 29k) running
#          scrip --transpile of the same chain; the same driver, the same clock. TIME() reads nanoseconds in both, measured.
#   SCRIP and SBL both run -s2000m -d8000m -i64m; SCRIP_DIAG=0 for the compile and every run. The median of GRID_RUNS (3) runs.
# COLUMNS: files, bytes, C ms, SCRIP ms, SBL ms, SBL/SCRIP, C/SCRIP, trees -- each file's section of SCRIP's output against the same
# file's section of SPITBOL's: SCRIP==SBL when every file agrees, else DIFFER n/N, how many and never how (Lon 2026-09-29: "It shows
# only how many do not match, not in what ay do they not match."; the details come from the full dump of a mismatching file, kept
# under GRID_OUT). The comparison is per file whatever a section holds, so it reads the per-file tree hash unchanged once the
# dumpers print one number per file instead of the tree (the cfo's row, CEO-1364).
# THE RUNTIME MEASURED IS A CHECKOUT'S OWN: GRID_ROOT=<a SCRIP checkout> (default: this one) names the scrip, out/libscrip_rt.so,
# out/parser_<lang> and bootstrap/ measured, each refused when older than that checkout's src/. RT_OPT IS READ BACK, NEVER ASSUMED:
# the linked runtime's tag (Makefile: RT_TAG, an md5 of RT_OPT|ZCFLAGS) against the tag of the checkout's default RT_OPT and, when
# it differs, against the tag of the RT_OPT this run was handed in its environment; neither agreeing REFUSES rc=2, because a number
# without its RT_OPT is unlabelled (RULES.md INSTRUMENT LAWS). THE SPEED RUNTIME (-O2 -DRT_DIAG=0, Lon 2026-09-29) IS BUILT BY HAND
# IN A SCRATCH WORKTREE and read here beside the -O0 default: GRID_ROOT=<worktree> RT_OPT='<its RT_OPT>' util_parser_grid.sh.
# No -O2 arm lives in this file (RULES.md NO -O2 BUILDS, CEO-1246: the -O2 build is a by-hand instrument; the grid only reads it).
# rc 0 = every language measured; rc 2 = something could not be measured (named on its line). Never two heavy runs in flight
# (CEO-1342): the whole grid is one heavy run and prints the load at its start and end.
set -uo pipefail
G=util_parser_grid
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; SELF="$(cd "$HERE/.." && pwd)"
refuse() { echo "⛔ REFUSE(2) [$G]: $*"; exit 2; }
W="$(cd "${GRID_ROOT:-$SELF}" 2>/dev/null && pwd)" || refuse "GRID_ROOT=${GRID_ROOT:-} is not a directory"
S4E="${S4E_HOME:-$(cd "$SELF/.." && pwd)}"; CORPUS="${CORPUS:-$S4E/corpus}"
[ -d "$CORPUS/tests" ] || refuse "no corpus at $CORPUS"
[ -x "$W/scripts/util_require_fresh.sh" ] || refuse "$W is not a SCRIP checkout (no scripts/util_require_fresh.sh)"
"$W/scripts/util_require_fresh.sh" --gate "$G" "$W/scrip" "$W/out/libscrip_rt.so" || exit 2
. "$HERE/lib_oracle_flags.sh" 2>/dev/null || refuse "cannot load lib_oracle_flags.sh -- the ONE oracle-path authority"
SBL="$(sbl_clean_bin)" || refuse "no clean benchmark oracle (sbl_clean_bin)"
B="$W/bootstrap"; CHAIN="$B/global.sc $B/case.sc $B/assign.sc $B/match.sc $B/counter.sc $B/stack.sc $B/tree.sc $B/ShiftReduce.sc $B/tdump.sc $B/gen.sc $B/qize.sc $B/semantic.sc $B/omega.sc $B/trace.sc"
for f in $CHAIN; do [ -f "$f" ] || refuse "chain file missing: $f"; done
SW="-s2000m -d8000m -i64m"; RUNS="${GRID_RUNS:-3}"; TMO="${GRID_TIMEOUT:-3000}"; export SCRIP_DIAG=0
case "$RUNS" in ''|*[!0-9]*|0) refuse "GRID_RUNS=$RUNS is not a positive count" ;; esac
bi() { env -u RT_OPT make -s -C "$W" ${1:+"RT_OPT=$1"} buildinfo 2>/dev/null | sed -n "s/^$2 *: *//p" | sed 's/ *$//'; }
lt="$(readlink "$W/out/libscrip_rt.so" | sed -n 's/^libscrip_rt-\([0-9a-f]*\)\.so$/\1/p')"
[ -n "$lt" ] || refuse "$W/out/libscrip_rt.so names no tagged runtime -- make"
if [ "$(bi '' RT_TAG)" = "$lt" ]; then OPT="$(bi '' RT_OPT) (the checkout's default)"
elif [ -n "${RT_OPT:-}" ] && [ "$(bi "$RT_OPT" RT_TAG)" = "$lt" ]; then OPT="$RT_OPT"
else refuse "the linked runtime libscrip_rt-$lt.so was not built with the checkout's default RT_OPT (tag $(bi '' RT_TAG))${RT_OPT:+ nor with RT_OPT='$RT_OPT' (tag $(bi "$RT_OPT" RT_TAG))} -- name the RT_OPT it was built with in RT_OPT"; fi
if [ -n "${GRID_OUT:-}" ]; then T="$GRID_OUT"; mkdir -p "$T" || refuse "cannot create GRID_OUT=$T"; else T=$(mktemp -d) || exit 2; trap 'rm -rf "$T"' EXIT; fi
declare -A EXT=([snobol4]=sno [snocone]=sc [icon]=icn [prolog]=pl [rebus]=reb [pascal]=pas [raku]=raku)
LANGS="$*"; if [ -z "$LANGS" ]; then for L in snobol4 snocone icon prolog rebus pascal raku; do grep -q "PARSER_FILES" "$B/parser_$L.sc" 2>/dev/null && LANGS="$LANGS $L"; done; fi
[ -n "$LANGS" ] || refuse "no bootstrap/parser_<lang>.sc reads PARSER_FILES"
tree="$(git -C "$W" rev-parse --short HEAD 2>/dev/null)$(git -C "$W" diff --quiet 2>/dev/null || echo -dirty)"
echo "PARSER GRID [$G]: tree $tree corpus $(git -C "$CORPUS" rev-parse --short HEAD 2>/dev/null) RT_OPT=$OPT runtime libscrip_rt-$lt.so sbl $(sbl_oracle_fingerprint "$SBL" | cut -d' ' -f1-2) switches $SW SCRIP_DIAG=0 median of $RUNS load $(cut -d' ' -f1-3 /proc/loadavg)"
printf '%-8s %5s %9s | %9s | %9s | %9s | %9s | %7s | %s\n' lang files bytes "C ms" "SCRIP ms" "SBL ms" "SBL/SCRIP" "C/SCRIP" trees
RC=0; NL=0; NC=0; N2=0
run() {   # $1 engine, $2 list, $3 output stem: one run of that engine over the list
    case "$1" in
    C)     ( cd "$CORPUS" && PARSER_FILES="$2" IPATH="$CORPUS/packages/icon/ipl/procs" timeout "$TMO" "$W/out/parser_$L" < /dev/null > "$3.out" 2> "$3.err" ) ;;
    SCRIP) PARSER_FILES="$2" timeout "$TMO" "$T/$L.bin" $SW < /dev/null > "$3.out" 2> "$3.err" ;;
    SBL)   PARSER_FILES="$2" timeout "$TMO" "$SBL" -bf $SW "$T/$L.sno" < /dev/null > "$3.out" 2> "$3.err" ;;
    esac
}
metric() { grep '^PARSER-METRICS ' "$1" | tail -1 | grep -o " $2=[0-9]*" | cut -d= -f2; }
whole() { [ "$(metric "$1.err" files)" = "$(wc -l < "$2")" ]; }   # the run reached its PARSER-METRICS line over every file of the list
scan() {  # $1 engine: drop from $T/$L.list each file that ends the engine's run; the dropped names in $T/$L.gone.$1
    local e="$1" rest="$T/$L.rest" last culprit f; : > "$T/$L.gone.$e"; cp "$T/$L.list" "$rest"
    : > "$T/$L.none"; run "$e" "$T/$L.none" "$T/$L.smoke"; whole "$T/$L.smoke" "$T/$L.none" || return 1
    while [ -s "$rest" ]; do
        run "$e" "$rest" "$T/$L.scan"; whole "$T/$L.scan" "$rest" && break
        last="$(grep '^== ' "$T/$L.scan.out" | tail -1 | cut -c4-)"; culprit=""
        while IFS= read -r f; do
            printf '%s\n' "$f" > "$T/$L.one"; run "$e" "$T/$L.one" "$T/$L.onerun"; whole "$T/$L.onerun" "$T/$L.one" || { culprit="$f"; break; }
        done < <(awk -v x="$last" 'x == "" || $0 == x { f = 1 } f' "$rest")
        [ -n "$culprit" ] || culprit="$last"; [ -n "$culprit" ] || return 1
        echo "$culprit" >> "$T/$L.gone.$e"
        awk -v x="$culprit" 'f; $0 == x { f = 1 }' "$rest" > "$rest.2"; mv "$rest.2" "$rest"
    done
    grep -vxF -f "$T/$L.gone.$e" "$T/$L.list" > "$T/$L.list.2"; mv "$T/$L.list.2" "$T/$L.list"
}
med() { printf '%s\n' "$@" | sort -n | sed -n "$(( ($# + 1) / 2 ))p"; }
ms() { awk -v u="$1" 'BEGIN { if (u == "") printf "-"; else printf "%.1f", u / 1000 }'; }
x() { awk -v r="$1" -v s="$2" 'BEGIN { if (r == "" || s == "" || s == 0) printf "-"; else printf "%.2fx", r / s }'; }
for L in $LANGS; do
    ext="${EXT[$L]:-}"; [ -n "$ext" ] || { echo "$L ⛔ REFUSE(2): not a parser language (${!EXT[*]})"; RC=2; continue; }
    grep -q "PARSER_FILES" "$B/parser_$L.sc" 2>/dev/null || { echo "$L ⛔ REFUSE(2): bootstrap/parser_$L.sc reads no PARSER_FILES list"; RC=2; continue; }
    [ -x "$W/out/parser_$L" ] && [ "$W/out/parser_$L" -nt "$W/src/tools/parser_main.c" ] || { echo "$L ⛔ REFUSE(2): out/parser_$L missing or older than src/tools/parser_main.c -- make parsers"; RC=2; continue; }
    cat $CHAIN "$B/parser_$L.sc" > "$T/$L.sc"
    "$W/scrip" --compile "$T/$L.sc" -o "$T/$L.s" < /dev/null > "$T/$L.cc.err" 2>&1 && gcc -m64 -no-pie -rdynamic "$T/$L.s" -Wl,-rpath,"$W/out" -L"$W/out" -lscrip_rt -lm -lpthread -o "$T/$L.bin" 2>> "$T/$L.cc.err" \
        || { echo "$L ⛔ REFUSE(2): the .sc chain did not compile or link to a mode-4 binary ($(head -1 "$T/$L.cc.err" | cut -c1-100))"; RC=2; continue; }
    "$W/scrip" --transpile "$T/$L.sc" > "$T/$L.sno" 2> "$T/$L.tr.err" && [ -s "$T/$L.sno" ] || { echo "$L ⛔ REFUSE(2): scrip --transpile of the chain failed"; RC=2; continue; }
    sbl_clean_refuse_if_load "$T/$L.sno" > /dev/null || { echo "$L ⛔ REFUSE(2): the transpiled chain calls LOAD(), unverified on the clean oracle"; RC=2; continue; }
    find "$CORPUS" -type f -name "*.$ext" -not -name 'ALL.*' -not -path '*/.git/*' -not -path "$CORPUS/library/*" | sort > "$T/$L.list"
    np=$(wc -l < "$T/$L.list"); [ "$np" -gt 0 ] || { echo "$L ⛔ REFUSE(2): no corpus program ends in .$ext"; RC=2; continue; }
    bad=""; for e in C SCRIP SBL; do scan "$e" || { bad="$e"; break; }; done
    [ -z "$bad" ] || { echo "$L ⛔ REFUSE(2): $bad dies before its first file (an empty list does not reach PARSER-METRICS; see $bad's stderr under GRID_OUT)"; RC=2; continue; }
    nf=$(wc -l < "$T/$L.list"); [ "$nf" -gt 0 ] || { echo "$L ⛔ REFUSE(2): every file ends some engine's run"; RC=2; continue; }
    tc=(); ts=(); tb=(); short=""
    for k in $(seq "$RUNS"); do
        for e in C SCRIP SBL; do run "$e" "$T/$L.list" "$T/$L.$e"; whole "$T/$L.$e" "$T/$L.list" || short="$short $e"; done
        tc+=($(metric "$T/$L.C.err" parse_us)); ts+=($(metric "$T/$L.SCRIP.err" parse_us)); tb+=($(metric "$T/$L.SBL.err" parse_us))
    done
    [ -z "$short" ] || { echo "$L ⛔ REFUSE(2): a timed run did not reach PARSER-METRICS over all $nf files:$short"; RC=2; continue; }
    c=$(med "${tc[@]}"); s=$(med "${ts[@]}"); b=$(med "${tb[@]}")
    nd=$(awk 'FNR == 1 { k++ } /^== / { nm = substr($0, 4); if (k == 1) a[nm] = ""; else b[nm] = ""; next } k == 1 { a[nm] = a[nm] $0 "\n"; next } { b[nm] = b[nm] $0 "\n" }
              END { for (n in a) if (!(n in b) || a[n] != b[n]) d++; for (n in b) if (!(n in a)) d++; print d + 0 }' "$T/$L.SCRIP.out" "$T/$L.SBL.out")
    t=SCRIP==SBL; [ "$nd" = 0 ] || t="DIFFER $nd/$nf"
    printf '%-8s %5s %9s | %9s | %9s | %9s | %9s | %7s | %s\n' "$L" "$nf" "$(metric "$T/$L.SCRIP.err" bytes)" "$(ms "$c")" "$(ms "$s")" "$(ms "$b")" "$(x "$b" "$s")" "$(x "$c" "$s")" "$t"
    echo "   $L list: $nf of $np corpus programs; dropped because the file ended the run -- C $(wc -l < "$T/$L.gone.C"), SCRIP $(wc -l < "$T/$L.gone.SCRIP"), SBL $(wc -l < "$T/$L.gone.SBL"); runs C ${tc[*]} SCRIP ${ts[*]} SBL ${tb[*]} us"
    NL=$((NL + 1)); awk -v c="$c" -v s="$s" 'BEGIN { exit !(s > 0 && c / s >= 1.0) }' && NC=$((NC + 1)); awk -v b="$b" -v s="$s" 'BEGIN { exit !(s > 0 && b / s >= 2.0) }' && N2=$((N2 + 1))
done
echo "BAR [$G]: $NC of $NL parsers at C's parse clock (C/SCRIP >= 1.00), $N2 of $NL at SPITBOL/SCRIP >= 2.00; load at end $(cut -d' ' -f1-3 /proc/loadavg)${GRID_OUT:+; lists, dropped names and dumps kept under $GRID_OUT}"
exit $RC
