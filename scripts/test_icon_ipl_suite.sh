#!/usr/bin/env bash
# test_icon_ipl_suite.sh -- grades corpus/packages/icon/ipl: the vendored Icon Program Library (851
# .icn files across gprogs/procs/incl/gincl/progs/gprocs) -- a REAL upstream code library, not a test
# suite: it ships ZERO .std reference outputs (confirmed: `find $PKG -iname '*.std'` -> 0 files, every
# run re-confirms this structurally rather than assuming it). Task icon-ipl-runner-and-denominator
# (hq_T mint, 2026-09-03): "a package suite with no runner is not a suite, it is a directory."
#
# TWO GRADING TIERS, NEVER CONFLATED (the task's own words: "Compile-graded-only entries must be
# COUNTED AS SUCH and never folded into a run-graded pass rate"):
#   compile-graded -- ./scrip --compile succeeds, OR (for a file with no `procedure main`) fails with
#                      EXACTLY the expected "no entry point" signal after real, non-trivial emission --
#                      i.e. the file's own content parsed/lowered/emitted cleanly; it is a library
#                      module, not a program, and that is not a defect.
#   run-graded     -- would additionally diff execution output against a NAME.std oracle. The
#                      population is counted structurally (files under $PKG with a same-named .std),
#                      not hardcoded to zero, so this self-corrects the day anyone vendors references.
#
# CLASSES THE REDS FALL INTO (grouped by directly-observed signal, same discipline as
# FINDING-2026-09-03-seat01-icon-arizona-...-nine-vendoring-gaps...: group by shared signature, never
# force one bucket; a genuinely new shape is reported by name, not absorbed):
#   nomain_ok -- no `procedure main`; sole failure is the expected mode-4-driver "main BB graph not
#                found" signal AFTER a non-trivial .s emission. Compile-graded PASS (library module).
#   linkgap   -- "icon: link: cannot open X (linked from Y)". SCRIP's link resolver
#                (icon_driver.c:26-45, icn_resolve_links) is SINGLE-DIRECTORY-ONLY: it looks for
#                "<dirname of the linking file>/<name>.icn" and nowhere else, no search path, no
#                ICONPATH/LPATH support anywhere in the tree (checked: zero hits). IPL's own
#                progs/procs/gprocs/incl/gincl split is upstream's NORMAL organization -- a progs/
#                file linking a procs/ helper is the library working as designed, not a corpus defect.
#                KNOWN, NAMED, NOT cured by this row: this task is a runner+census row (GOAL: "print
#                the denominator ... classify the reds"), same scope line the GNU Prolog and Arizona
#                vendor-suite rows drew before it -- a real fix here means either a genuine link
#                search-path feature in the compiler (shared-node, cross-language blast radius) or
#                vendoring/renaming ~hundreds of cross-links (the Arizona finding did this AT ONE
#                FILE'S SCALE, by hand, for 5 names -- not a pattern that scales to IPL's population
#                sight-unseen). Left for a follow-up row.
#   parseerr  -- "parse error" on stdout/stderr -- genuine frontend rejection.
#   timeout   -- exceeded $TIMEOUT.
#   other     -- any other non-zero, non-timeout signal. Reported individually (name + first stderr
#                line) so nothing new is ever silently folded into an existing bucket.
#
# SCORE.md: this suite is compile-graded-only (run_graded=0, honestly, because no oracle exists) --
# per the task's own warning, its numbers are written as bare key=value pairs, NEVER as a bald N/851
# fraction, so `util_score_row.py`'s cell_fractions() (which scores the ONE LEADERBOARD's overall
# per-language % from every bare \d+/\d+ it finds) cannot mistake "compiles" for "verified correct".
# Same convention GNU_SUITE already set: `lib=56` is a bare count, never `56/62`, for exactly this
# reason (test_prolog_gnu_suite.sh's own comment block explains it at the OK/LIB split).
#
# Usage: bash scripts/test_icon_ipl_suite.sh [-v]   (-v prints every file's classification inline)
set -uo pipefail
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${SCRIP:-$HERE/../scrip}"
PKG="$S4E/corpus/packages/icon/ipl"
TIMEOUT="${IPL_SUITE_TIMEOUT:-8}"
VERBOSE=0; [ "${1:-}" = "-v" ] && VERBOSE=1

[ -d "$PKG" ]   || { echo "⛔ GATE REFUSES: corpus subtree missing: $PKG" >&2; exit 2; }
[ -x "$SCRIP" ] || { echo "⛔ GATE REFUSES: scrip not built at $SCRIP" >&2; exit 2; }

TMP="$(mktemp -d /tmp/ipl_suite_XXXXXX)"
trap 'rm -rf "$TMP"' EXIT

mapfile -t FILES < <(find "$PKG" -name "*.icn" | sort)
TOTAL=${#FILES[@]}
[ "$TOTAL" -gt 0 ] || { echo "⛔ GATE REFUSES: zero .icn files found under $PKG" >&2; exit 2; }

mapfile -t STDFILES < <(find "$PKG" -name "*.std")
RUN_GRADED=${#STDFILES[@]}

COMPILE_PASS=0; COMPILE_FAIL=0
NOMAIN_OK=0; LINKGAP=0; PARSEERR=0; TIMEOUT_N=0; OTHER=0
NOMAIN_TOTAL=0; HASMAIN_TOTAL=0
LINKGAP_NAMES=(); PARSEERR_NAMES=(); OTHER_NAMES=(); TIMEOUT_NAMES=()
declare -A OTHER_SIG_COUNT

echo "=== Icon Program Library ($TOTAL .icn files, $PKG; run-graded population=$RUN_GRADED) ==="

for f in "${FILES[@]}"; do
    rel="${f#"$PKG"/}"
    base="$(basename "$f" .icn)"
    out="$TMP/${base}.s"
    log="$TMP/${base}.log"
    has_main=0
    grep -qE '^procedure[[:space:]]+main[[:space:]]*\(' "$f" && has_main=1
    if [ "$has_main" -eq 1 ]; then HASMAIN_TOTAL=$((HASMAIN_TOTAL+1)); else NOMAIN_TOTAL=$((NOMAIN_TOTAL+1)); fi

    timeout "$TIMEOUT" "$SCRIP" --compile "$f" -o "$out" < /dev/null > "$log" 2>&1
    rc=$?

    if [ "$rc" -eq 124 ]; then
        TIMEOUT_N=$((TIMEOUT_N+1)); TIMEOUT_NAMES+=("$rel")
        COMPILE_FAIL=$((COMPILE_FAIL+1))
        [ "$VERBOSE" -eq 1 ] && echo "  TIMEOUT $rel"
        continue
    fi

    if [ "$rc" -eq 0 ]; then
        COMPILE_PASS=$((COMPILE_PASS+1))
        [ "$VERBOSE" -eq 1 ] && echo "  PASS $rel"
        continue
    fi

    # rc != 0, not a timeout -- classify by signal. nomain_ok is checked FIRST and narrowly (has_main
    # must be 0 AND the exact FATAL string) so a has-main file that hits this same string for a
    # genuinely surprising reason falls through to OTHER instead of being silently absorbed. ⚠️ Does
    # NOT additionally require a non-empty $out: measured directly (fresh /tmp path, no stale debris)
    # that this failure path writes NOTHING to -o, ever -- confirming the fatal exit happens before
    # mode-4 ever opens the output file, so a size check here would just always read empty and never
    # fire. Matches the exact bar test_prolog_gnu_suite.sh's sibling LIB check already uses (grep the
    # one known-shape string on the probe log, nothing else) -- and by the compiler's own control flow
    # a link-resolution or parse-error exit() fires strictly earlier and never reaches this string, so
    # the two classes cannot collide in one run.
    if [ "$has_main" -eq 0 ] && grep -q "\[IBB\] FATAL: mode-4 driver: main BB graph not found" "$log"; then
        NOMAIN_OK=$((NOMAIN_OK+1)); COMPILE_PASS=$((COMPILE_PASS+1))
        [ "$VERBOSE" -eq 1 ] && echo "  PASS (nomain, library module) $rel"
        continue
    fi

    COMPILE_FAIL=$((COMPILE_FAIL+1))
    if grep -q "^icon: link: cannot open" "$log"; then
        LINKGAP=$((LINKGAP+1)); LINKGAP_NAMES+=("$rel")
        [ "$VERBOSE" -eq 1 ] && echo "  FAIL (linkgap) $rel"
    elif grep -qi "parse error" "$log"; then
        PARSEERR=$((PARSEERR+1)); PARSEERR_NAMES+=("$rel")
        [ "$VERBOSE" -eq 1 ] && echo "  FAIL (parse error) $rel"
    else
        OTHER=$((OTHER+1)); OTHER_NAMES+=("$rel (rc=$rc)")
        sig="$(head -1 "$log" | tr -s ' ')"
        OTHER_SIG_COUNT["$sig"]=$(( ${OTHER_SIG_COUNT["$sig"]:-0} + 1 ))
        echo "  ⛔ OTHER $rel -- rc=$rc: $(head -1 "$log")"
    fi
done

echo ""
echo "-- linkgap (KNOWN, NAMED, not cured this row -- see file header: SCRIP's link resolver is single-directory-only, icon_driver.c:26-45; IPL's progs/procs split makes cross-dir links the library's normal shape): $LINKGAP --"
if [ "$VERBOSE" -eq 1 ]; then for n in "${LINKGAP_NAMES[@]:-}"; do [ -n "$n" ] && echo "   $n"; done; fi

echo ""
echo "-- parseerr (genuine frontend rejection): $PARSEERR --"
for n in "${PARSEERR_NAMES[@]:-}"; do [ -n "$n" ] && echo "   $n"; done

echo ""
echo "-- timeout (>${TIMEOUT}s): $TIMEOUT_N --"
for n in "${TIMEOUT_NAMES[@]:-}"; do [ -n "$n" ] && echo "   $n"; done

if [ "$OTHER" -gt 0 ]; then
    echo ""
    echo "-- OTHER (unclassified signal, needs investigation): $OTHER --"
    for sig in "${!OTHER_SIG_COUNT[@]}"; do echo "   [${OTHER_SIG_COUNT[$sig]}x] $sig"; done
    for n in "${OTHER_NAMES[@]}"; do echo "   $n"; done
fi

COMPILE_GRADED=$((COMPILE_PASS + COMPILE_FAIL))
echo ""
echo "IPL_SUITE_BOARD total=$TOTAL compile_graded=$COMPILE_GRADED compile_pass=$COMPILE_PASS compile_fail=$COMPILE_FAIL run_graded=$RUN_GRADED nomain_total=$NOMAIN_TOTAL hasmain_total=$HASMAIN_TOTAL nomain_ok=$NOMAIN_OK linkgap=$LINKGAP parseerr=$PARSEERR timeout=$TIMEOUT_N other=$OTHER"
# ⛔ ONE LEADERBOARD (RULES.md FACT RULE, Lon 2026-09-03 ~16:05: "any run of a test suite by any
# session will update the ONE LEADERBOARD"). This records the board line printed just above into
# .github/SCORE.md -- it RUNS NOTHING, it only writes down what this script already measured.
# ⛔ NON-FATAL BY DESIGN: a bookkeeping failure must never turn a real measurement into a red board.
# ⛔ NO BARE N/TOTAL FRACTION -- see file header: compile-graded is not run-graded-correct, and a
# fraction here would silently enter the ONE LEADERBOARD's automatic per-language pass-rate sum
# (util_score_row.py cell_fractions()) as if it were verified-correct output. key=value only.
python3 "$HERE/util_score_row.py" write --lang icon --column vendor --suite IPL \
    --measurer "${S4E_SEAT:-unknown-seat}" \
    --text "compile_pass=$COMPILE_PASS compile_fail=$COMPILE_FAIL (linkgap=$LINKGAP parseerr=$PARSEERR timeout=$TIMEOUT_N other=$OTHER) of total=$TOTAL · nomain_ok=$NOMAIN_OK of nomain_total=$NOMAIN_TOTAL, hasmain_total=$HASMAIN_TOTAL · run_graded=0 (upstream IPL ships no .std oracle) -- no verified-correctness population yet, compile-graded-only (\`test_icon_ipl_suite.sh\`)" \
    || echo "⚠ SCORE.md NOT UPDATED -- record this row by hand (the REFUSED line above says why)"

[ "$((COMPILE_PASS + COMPILE_FAIL))" -eq "$TOTAL" ] || { echo "⛔ BUCKET COUNTS DON'T SUM TO TOTAL -- instrument bug, refusing to trust the board"; exit 2; }
[ "$OTHER" -eq 0 ]
