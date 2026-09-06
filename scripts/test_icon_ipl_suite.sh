#!/usr/bin/env bash
# test_icon_ipl_suite.sh -- grades corpus/packages/icon/ipl: the vendored Icon Program Library (every .icn
# under gprogs/procs/incl/gincl/progs/gprocs -- COUNT IT, never quote a remembered figure:
#   find corpus/packages/icon/ipl -name '*.icn' | wc -l
# ⛔⭐ THE POPULATION IS 851, AND THIS HEADER WAS "CURED" TO 852 ON 2026-09-05 BY hq_I -- WRONGLY, IN THE
# OTHER DIRECTION. The prose was right and the instrument was wrong. `find -name '*.icn'` returned 852
# because it counted `ALL.icn`, OUR OWN generated container (corpus `3b3b5b874`, landed the same day), not a
# vendored program. ⭐ A STRUCTURAL COUNT IS ONLY AS RIGHT AS THE POPULATION IT WALKS: that find answers
# "how many .icn files are under this directory", never "how many vendored programs are here", and it will
# absorb anything we drop into the package without saying so. I trusted "the runner computes it structurally"
# over a number a human had written down, and that heuristic is what carried the error outward into SCORE.md,
# this header, and my own goal cursor. The accounting that settles it is the package's own and it is exact:
# ALL.csv 78 entries + ALL.excluded.txt 773 named exclusions = 851. The runner REFUSES below if that identity
# ever breaks, so the two instruments in this package can no longer disagree in silence. That is RULES.md
# TRANSCRIPTION IS WHERE PROVENANCE DIES, committed
# inside the very script that measures the thing correctly) -- a REAL upstream code library, not a test
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
#   run-graded     -- diffs execution output against a NAME.std oracle ref, both modes, independently
#                      (RUN_PASS/RUN_FAIL/RUN_CRASH/RUN_HANG). The population is counted structurally
#                      (progs/*.std files, cut by util_cut_icon_ipl_refs.sh from the real Icon oracle --
#                      see that script's own header for the classification discipline: only a
#                      deterministic, input-free, non-usage-banner clean run gets pinned), not hardcoded,
#                      so this self-corrects the day the population changes.
#
# ⛔⛔ EXECUTION (both m3 --run and m4's compiled binary) GOES THROUGH lib_icon_ipl_isolation.sh, NEVER
# a bare `cd $PKG/progs && ...`: IPL ships programs that mutate their own cwd as documented behavior
# (progs/upper.icn, progs/lower.icn rename every file in cwd) -- see that lib's header for the corpus-
# corruption incident this was built from. COMPILE (`--compile` emitting .s) does not execute the
# target program's own logic, so it stays unisolated same as before; only actually RUNNING one -- as the
# source program (m3) or as a linked binary (m4) -- carries the hazard.
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
# per the task's own warning, its numbers are written as bare key=value pairs, NEVER as a bald N-over-the-population
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
# ⛔⭐ THE COMPILE TIER MUST SEE THE LIBRARY'S OWN SHAPE, AND UNTIL 2026-09-05 IT DID NOT (hq_I).
# lib_icon_ipl_isolation.sh has always exported ICONPATH for the RUN tier, but the COMPILE tier below
# invoked scrip with no path at all, so a progs/ program linking a procs/ helper could only ever fail --
# and it did, 354 times, which was 85% of every IPL compile failure and 41% of the whole package. The
# resolver was single-directory-only (cured in icon_driver.c the same sitting); this is the other half,
# and neither half does anything alone. Exported once here so both tiers agree on the search path.
IPL_PATH="$PKG/progs:$PKG/procs:$PKG/gprocs:$PKG/incl:$PKG/gincl"
export ICONPATH="${ICONPATH:-$IPL_PATH}"
VERBOSE=0; [ "${1:-}" = "-v" ] && VERBOSE=1

[ -d "$PKG" ]   || { echo "⛔ GATE REFUSES: corpus subtree missing: $PKG" >&2; exit 2; }
[ -x "$SCRIP" ] || { echo "⛔ GATE REFUSES: scrip not built at $SCRIP" >&2; exit 2; }
# ⛔⭐ STALE-BINARY PREFLIGHT (row harness-and-ladder-runner-refuse-on-a-stale-binary-like-the-artifact-regen-
# does, ceo -> hq_T 2026-09-04). The line above proves a binary EXISTS; this one proves it is the binary this
# tree describes. ceo's witness, twice on 2026-09-04: a 10:57 binary graded at 14:03 read RED, then GREEN after
# an incremental make -- a vendor board is exactly where that is least visible, because a plausible all-FAIL
# table is this class's normal output. NO LOGIC HERE: util_require_fresh.sh sources gate_require_fresh from
# lib_gate.sh, the ONE authority (hq_B 4c7253e99) -- never a second copy of the staleness rule.
"$HERE/util_require_fresh.sh" --gate test_icon_ipl_suite "$SCRIP" "${RT_DIR:-$HERE/../out}/libscrip_rt.so" || exit 2

TMP="$(mktemp -d /tmp/ipl_suite_XXXXXX)"
trap 'rm -rf "$TMP"' EXIT

# ⛔ EXCLUDE OUR OWN CONTAINER: ALL.icn is generated by util_build_* from the entries below it, so counting
# it makes the package's concatenation a 852nd "vendored program" -- it parse-errors at line 14125 and lands
# in parseerr, contaminating a VENDOR score with a file we wrote. The population is upstream files only.
mapfile -t FILES < <(find "$PKG" -name "*.icn" ! -name "ALL.icn" | sort)
TOTAL=${#FILES[@]}
[ "$TOTAL" -gt 0 ] || { echo "⛔ GATE REFUSES: zero .icn files found under $PKG" >&2; exit 2; }
# ⛔⭐ CROSS-INSTRUMENT IDENTITY -- the check whose absence let 851 be "corrected" to 852 for a whole session.
# The container machinery accounts for every upstream file exactly once: graded entries + named exclusions.
# If that disagrees with what this runner walks, the two instruments in ONE package disagree and NEITHER
# number can be published, so we REFUSE rather than print a plausible one (rc=2 = could not measure).
if [ -f "$PKG/ALL.csv" ] && [ -f "$PKG/ALL.excluded.txt" ]; then
    _entries=$(( $(wc -l < "$PKG/ALL.csv") - 1 ))
    _excluded=$(wc -l < "$PKG/ALL.excluded.txt")
    _accounted=$(( _entries + _excluded ))
    if [ "$_accounted" -ne "$TOTAL" ]; then
        echo "⛔ GATE REFUSES: population disagreement inside one package -- this runner walks $TOTAL upstream .icn," >&2
        echo "   but the container accounts for $_accounted (ALL.csv $_entries entries + ALL.excluded.txt $_excluded exclusions)." >&2
        echo "   Neither number may be published. Regenerate the container, or find what entered the tree unaccounted." >&2
        exit 2
    fi
fi

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

# ═══ RUN TIER -- every progs/*.icn with a NAME.std (cut by util_cut_icon_ipl_refs.sh) gets EXECUTED,
# both modes independently, and diffed against it. See file header: execution goes through
# lib_icon_ipl_isolation.sh, never a bare cd into $PKG/progs. ═══
. "$HERE/lib_inventory.sh" 2>/dev/null || { echo "⛔ GATE REFUSES: lib_inventory.sh unloadable" >&2; exit 2; }
. "$HERE/lib_icon_ipl_isolation.sh"
. "$HERE/lib_progress.sh" 2>/dev/null || { echo "⛔ GATE REFUSES: lib_progress.sh unloadable -- a run that records nothing is a defect of that run (CEO-331)" >&2; exit 2; }
# ⛔ ipl DOES NOT RECORD AUTOMATICALLY, whatever the ALL.icn pair suggests. CEO-331 lists ipl among the
# packages that "now record automatically" because they have an ALL.<ext> pair -- but this package's
# ALL.icn/ALL.ref are consumed by NO runner (measured 2026-09-06: this script opens ALL.csv only for its
# container identity check and grades by per-program .std; board_packages.sh's runner_for() has no
# icon/ipl case; ALL.ref has ONE commit in its whole history, and a ref nothing regenerates is a ref
# nothing grades). So the automatic path would record a suite nobody runs. This runner has its own loop
# and appends from it. ⭐ A package can satisfy the STRUCTURAL condition for a mechanism and be outside it.
PROGRESS_FAILED=0
ipl_progress() { progress_append package ipl icon "$1" "$2" "$3" || PROGRESS_FAILED=$((PROGRESS_FAILED+1)); }
ipl_isolation_init "$PKG" || { echo "⛔ GATE REFUSES: could not build IPL isolation template" >&2; exit 2; }
trap 'rm -rf "$TMP"; ipl_isolation_cleanup' EXIT

M3_RUN_PASS=0; M3_RUN_FAIL=0; M3_RUN_CRASH=0; M3_RUN_HANG=0
M4_RUN_PASS=0; M4_RUN_FAIL=0; M4_RUN_CRASH=0; M4_RUN_HANG=0
M3_RUN_FAIL_NAMES=(); M4_RUN_FAIL_NAMES=()
M3_RUN_CRASH_NAMES=(); M4_RUN_CRASH_NAMES=(); M3_RUN_HANG_NAMES=(); M4_RUN_HANG_NAMES=()

# ⛔⛔ MAX_BYTES gates every full-content read here too, checked via `wc -c` on the FILE before any
# slurp into a bash variable -- same discipline, same incident, as util_cut_icon_ipl_refs.sh's own
# MAX_BYTES (see that script's header): a timing-out process is not necessarily quiet while it waits, so
# `out3="$(cat ...)"` run UNCONDITIONALLY before checking rc==124 can slurp gigabytes for a result about
# to be discarded as HANG anyway. Here the risk is SCRIP's own output on a genuine bug (an infinite-
# output loop from a miscompiled pattern), not the oracle's -- same shape, same fix. Every .std this
# tier reads is itself already capped at ref-cutting time, so oversized ACTUAL output can never equal
# `$exp` regardless; it is graded RUN_FAIL without ever being read into memory.
MAX_BYTES=1048576

for std in "${STDFILES[@]}"; do
    base="$(basename "$std" .std)"
    icn="$(dirname "$std")/$base.icn"
    [ -f "$icn" ] || continue
    exp="$(cat "$std")"
    # ⭐ NAME.dat STDIN SIDECAR (hq_I 2026-09-05): same convention as arizona/jcon_tests' own .dat
    # fixtures, and the one util_cut_icon_ipl_refs.sh mints these four new refs under -- both tiers
    # must agree per this file's own FACT RULE (a script and its DONE-WHEN, or its sibling ref-cutter,
    # must not disagree).
    dat="$(dirname "$std")/$base.dat"; stdin_src=/dev/null; [ -f "$dat" ] && stdin_src="$dat"
    # ⭐ NAME.argv ARGV SIDECAR (hq_I 2026-09-06, CEO-328). Read through the SHARED reader in
    # lib_icon_ipl_isolation.sh, never re-implemented here -- the ref-cutter mints under exactly this
    # lookup, so a program's ref and its grading cannot disagree about the argv it was given. ⛔ A
    # MALFORMED SIDECAR IS A GRADED FAILURE, NOT A SKIP: grading it with an empty argv would silently
    # compare the program's no-arguments behaviour against a ref cut WITH arguments, and score the
    # difference against SCRIP. That is the false-FAIL shape this package already carries in arizona.
    declare -a IPLARGV=(); ipl_argv_read "$icn" IPLARGV; argv_rc=$?
    if [ "$argv_rc" -eq 2 ]; then
        M3_RUN_FAIL=$((M3_RUN_FAIL+1)); M3_RUN_FAIL_NAMES+=("$base(argv-sidecar-malformed)"); ipl_progress "$base" m3 REFUSE
        M4_RUN_FAIL=$((M4_RUN_FAIL+1)); M4_RUN_FAIL_NAMES+=("$base(argv-sidecar-malformed)"); ipl_progress "$base" m4 REFUSE
        continue
    fi
    # ⛔ Each entry runs with ITS OWN package subdirectory as cwd -- refs are no longer progs-only
    # (gprogs/ carries .std files as of 2026-09-06, CEO-316). A gprogs entry run from progs/ links
    # against the wrong directory and grades a program that never ran properly.
    IPL_ISO_SUBDIR="$(basename "$(dirname "$std")")"; export IPL_ISO_SUBDIR

    # -- m3 (--run): executes the Icon program's own logic directly -- isolated.
    # ⛔ `--` separates SCRIP's own flags from the target program's argv; the oracle needs no separator
    # (`icon prog.icn a b`), and the m4 binary takes them bare. Three shapes, one declaration.
    # ⛔ WRITTEN LONG ON PURPOSE. The clever one-liner `${IPLARGV[@]+-- "${IPLARGV[@]}"}` GLUES the
    # literal to the first element -- measured: with (x, "y z") it yields the two words `-- x` and `y z`,
    # so SCRIP receives "-- x" as a single argument and the program sees one argv entry, not two. It
    # looks right, it runs, and every count downstream would have been quietly off.
    if [ "${#IPLARGV[@]}" -gt 0 ]; then
        ipl_isolation_run "$TMP/${base}.m3.out" "$TIMEOUT" "$stdin_src" "$SCRIP" --run "$icn" -- "${IPLARGV[@]}"
    else
        ipl_isolation_run "$TMP/${base}.m3.out" "$TIMEOUT" "$stdin_src" "$SCRIP" --run "$icn"
    fi
    rc3=$?
    by3=$(wc -c < "$TMP/${base}.m3.out" 2>/dev/null || echo 0)
    if [ "$rc3" -eq 124 ]; then M3_RUN_HANG=$((M3_RUN_HANG+1)); M3_RUN_HANG_NAMES+=("$base"); ipl_progress "$base" m3 HANG
    elif [ "$rc3" -ge 128 ]; then M3_RUN_CRASH=$((M3_RUN_CRASH+1)); M3_RUN_CRASH_NAMES+=("$base(sig$((rc3-128)))"); ipl_progress "$base" m3 CRASH
    elif [ "$by3" -gt "$MAX_BYTES" ]; then M3_RUN_FAIL=$((M3_RUN_FAIL+1)); M3_RUN_FAIL_NAMES+=("$base(oversized:$by3)"); ipl_progress "$base" m3 FAIL
    elif [ "$(cat "$TMP/${base}.m3.out" 2>/dev/null)" = "$exp" ]; then M3_RUN_PASS=$((M3_RUN_PASS+1)); ipl_progress "$base" m3 PASS
    else M3_RUN_FAIL=$((M3_RUN_FAIL+1)); M3_RUN_FAIL_NAMES+=("$base"); ipl_progress "$base" m3 FAIL; fi

    # -- m4 (--compile): emitting .s does not execute the target program, so THAT step is unisolated,
    # same as the compile tier above; only running the linked binary carries the self-mutation hazard.
    s4="$TMP/${base}.m4.s"; bin4="$TMP/${base}.m4.bin"
    "$SCRIP" --compile "$icn" >"$s4" 2>"$TMP/${base}.m4.diag" </dev/null
    if [ -s "$s4" ] && gcc -no-pie "$s4" -L"$HERE/../out" -lscrip_rt -Wl,-rpath,"$HERE/../out" -o "$bin4" 2>/dev/null; then
        if [ "${#IPLARGV[@]}" -gt 0 ]; then
            ipl_isolation_run "$TMP/${base}.m4.out" "$TIMEOUT" "$stdin_src" "$bin4" "${IPLARGV[@]}"
        else
            ipl_isolation_run "$TMP/${base}.m4.out" "$TIMEOUT" "$stdin_src" "$bin4"
        fi
        rc4=$?
        by4=$(wc -c < "$TMP/${base}.m4.out" 2>/dev/null || echo 0)
        if [ "$rc4" -eq 124 ]; then M4_RUN_HANG=$((M4_RUN_HANG+1)); M4_RUN_HANG_NAMES+=("$base"); ipl_progress "$base" m4 HANG
        elif [ "$rc4" -ge 128 ]; then M4_RUN_CRASH=$((M4_RUN_CRASH+1)); M4_RUN_CRASH_NAMES+=("$base(sig$((rc4-128)))"); ipl_progress "$base" m4 CRASH
        elif [ "$by4" -gt "$MAX_BYTES" ]; then M4_RUN_FAIL=$((M4_RUN_FAIL+1)); M4_RUN_FAIL_NAMES+=("$base(oversized:$by4)"); ipl_progress "$base" m4 FAIL
        elif [ "$(cat "$TMP/${base}.m4.out" 2>/dev/null)" = "$exp" ]; then M4_RUN_PASS=$((M4_RUN_PASS+1)); ipl_progress "$base" m4 PASS
        else M4_RUN_FAIL=$((M4_RUN_FAIL+1)); M4_RUN_FAIL_NAMES+=("$base"); ipl_progress "$base" m4 FAIL; fi
    else
        M4_RUN_FAIL=$((M4_RUN_FAIL+1)); M4_RUN_FAIL_NAMES+=("$base(compile/link)"); ipl_progress "$base" m4 FAIL
    fi
    rm -f "$s4" "$bin4" "$TMP/${base}.m3.out" "$TMP/${base}.m4.out"
done

echo ""
echo "-- RUN tier: $RUN_GRADED progs/ programs graded against a .std cut from the real Icon oracle (util_cut_icon_ipl_refs.sh --apply) --"
echo "mode-3 (--run):     RUN_PASS=$M3_RUN_PASS RUN_FAIL=$M3_RUN_FAIL RUN_CRASH=$M3_RUN_CRASH RUN_HANG=$M3_RUN_HANG / $RUN_GRADED"
echo "mode-4 (--compile): RUN_PASS=$M4_RUN_PASS RUN_FAIL=$M4_RUN_FAIL RUN_CRASH=$M4_RUN_CRASH RUN_HANG=$M4_RUN_HANG / $RUN_GRADED"
[ "$VERBOSE" -eq 1 ] && [ ${#M3_RUN_FAIL_NAMES[@]} -gt 0 ] && printf 'm3 RUN_FAIL:%s\n' "$(printf ' %s' "${M3_RUN_FAIL_NAMES[@]}")"
[ "$VERBOSE" -eq 1 ] && [ ${#M4_RUN_FAIL_NAMES[@]} -gt 0 ] && printf 'm4 RUN_FAIL:%s\n' "$(printf ' %s' "${M4_RUN_FAIL_NAMES[@]}")"
[ ${#M3_RUN_CRASH_NAMES[@]} -gt 0 ] && printf 'm3 RUN_CRASH:%s\n' "$(printf ' %s' "${M3_RUN_CRASH_NAMES[@]}")"
[ ${#M4_RUN_CRASH_NAMES[@]} -gt 0 ] && printf 'm4 RUN_CRASH:%s\n' "$(printf ' %s' "${M4_RUN_CRASH_NAMES[@]}")"
[ ${#M3_RUN_HANG_NAMES[@]} -gt 0 ] && printf 'm3 RUN_HANG:%s\n' "$(printf ' %s' "${M3_RUN_HANG_NAMES[@]}")"
[ ${#M4_RUN_HANG_NAMES[@]} -gt 0 ] && printf 'm4 RUN_HANG:%s\n' "$(printf ' %s' "${M4_RUN_HANG_NAMES[@]}")"
echo "IPL_RUN_BOARD run_graded=$RUN_GRADED m3_RUN_PASS=$M3_RUN_PASS m3_RUN_FAIL=$M3_RUN_FAIL m3_RUN_CRASH=$M3_RUN_CRASH m3_RUN_HANG=$M3_RUN_HANG m4_RUN_PASS=$M4_RUN_PASS m4_RUN_FAIL=$M4_RUN_FAIL m4_RUN_CRASH=$M4_RUN_CRASH m4_RUN_HANG=$M4_RUN_HANG"
ipl_isolation_verify_clean "$S4E/corpus" || true

# ⭐ THE PACKAGE LOCKDOWN inventory line, via the shared body (lib_inventory.sh) -- never a second copy
# of the arithmetic (CEO-321: this row is hq_I's). ipl was the LAST Icon runner without it; arizona and
# jcon already print one.
# ⛔ THE GRADED POPULATION IS THE PACKAGE-WIDE .std COUNT ($RUN_GRADED, from `find "$PKG" -name '*.std'`),
# NOT progs/ alone. gprogs/ briefly carried refs on 2026-09-06; they were wrong and were reverted, but the
# discovery is package-wide by construction and lib_icon_ipl_isolation.sh is now subdirectory-aware
# (IPL_ISO_SUBDIR), so a future non-progs ref grades from its own directory instead of silently from progs/.
# ⛔ graded_narrow=0: this suite diffs FULL output against the oracle ref, never by error-number only.
INV_PACKAGE=ipl; INV_DIR="$PKG"; INV_EXT=".icn"
INV_LINE="$(inventory_line "$RUN_GRADED" 0)"
if [ -n "$INV_LINE" ]; then echo "$INV_LINE"; else echo "⚠ inventory refused (above) -- the board lines still stand; the inventory does not" >&2; fi
[ "${PROGRESS_FAILED:-0}" -eq 0 ] || echo "⛔ PROGRESS DB: $PROGRESS_FAILED per-program appends FAILED -- this run is not fully recorded (CEO-331); the board lines stand, the table does not" >&2

# ⭐ THE CLASS SPLIT, IN THE INVENTORY LINE RATHER THAN IN PROSE (CEO-328). It was built here first and
# hq_T RULED it into the shared body the same day (lib_inventory.sh: inventory_split_line), so this runner
# now CALLS it rather than carrying a second copy -- their reason is the keeper: a per-runner reading means
# seven readings of one count and no way to say which is authoritative. The sum invariant travelled with it.
if SPLIT_LINE="$(inventory_split_line)"; then [ -n "$SPLIT_LINE" ] && echo "$SPLIT_LINE"
else SPLIT_LINE=""; echo "⛔ PACKAGE INVENTORY SPLIT REFUSED (rc=2, reason above) -- the classes do not sum to their own buckets, so neither reading is published" >&2; fi
UNG_SPLIT="$(printf '%s' "$SPLIT_LINE" | sed -n 's/.*ungraded_by_class=\([^ ]*\).*/\1/p')"

# ⛔ ONE LEADERBOARD (RULES.md FACT RULE, Lon 2026-09-03 ~16:05: "any run of a test suite by any
# session will update the ONE LEADERBOARD"). This records the boards printed just above into
# .github/SCORE.md -- it RUNS NOTHING, it only writes down what this script already measured.
# ⛔ NON-FATAL BY DESIGN: a bookkeeping failure must never turn a real measurement into a red board.
# ⛔ NO BARE N/TOTAL FRACTION for the COMPILE tier -- see file header: compile-graded is not
# run-graded-correct, and a fraction here would silently enter the ONE LEADERBOARD's automatic
# per-language pass-rate sum (util_score_row.py cell_fractions()) as if it were verified-correct
# output. The RUN tier IS a verified-correctness population (diffed against a real oracle), so ITS
# m3/m4 fractions are reported as fractions -- same convention test_icon_arizona_suite.sh already uses.
python3 "$HERE/util_score_row.py" write --lang icon --column vendor --suite IPL \
    --measurer "${S4E_SEAT:-}" \
    --text "compile_pass=$COMPILE_PASS compile_fail=$COMPILE_FAIL (linkgap=$LINKGAP parseerr=$PARSEERR timeout=$TIMEOUT_N other=$OTHER) of total=$TOTAL · nomain_ok=$NOMAIN_OK of nomain_total=$NOMAIN_TOTAL, hasmain_total=$HASMAIN_TOTAL · run m3 $M3_RUN_PASS/$RUN_GRADED m4 $M4_RUN_PASS/$RUN_GRADED (of $RUN_GRADED oracle-cut · fail m3=$M3_RUN_FAIL m4=$M4_RUN_FAIL, crash m3=$M3_RUN_CRASH m4=$M4_RUN_CRASH, hang m3=$M3_RUN_HANG m4=$M4_RUN_HANG)${INV_LINE:+ · $INV_LINE}${UNG_SPLIT:+ · ungraded_by_class=$UNG_SPLIT} (\`test_icon_ipl_suite.sh\`)" \
    || echo "⚠ SCORE.md NOT UPDATED -- record this row by hand (the REFUSED line above says why)"

# ⛔⭐ POPULATION FLOOR (row every-board-wrapper-refuses-on-a-zero-population-instead-of-passing-
# vacuously, hq_T 2026-09-04): the bucket-sum check just below PASSES vacuously at TOTAL=0 (0+0==0),
# and OTHER=0 too when nothing was discovered -- neither line can tell "examined and clean" from
# "examined nothing", which is exactly what this refuses before either check runs.
"$HERE/util_require_population.sh" --gate test_icon_ipl_suite "$TOTAL" 1 "IPL source files discovered" || exit 2
[ "$((COMPILE_PASS + COMPILE_FAIL))" -eq "$TOTAL" ] || { echo "⛔ BUCKET COUNTS DON'T SUM TO TOTAL -- instrument bug, refusing to trust the board"; exit 2; }
# ⛔ THE SCRIPT'S OWN EXIT CODE MUST AGREE WITH WHAT THE TASK'S DONE-WHEN INDEPENDENTLY RE-DERIVES FROM
# THIS OUTPUT (RUN_PASS>0, every RUN_FAIL/RUN_CRASH/RUN_HANG==0 in both modes) -- a script that reads
# green by rc while the DONE-WHEN reads red from the same text (or vice versa) is exactly the kind of
# disagreement RULES.md's FACT RULE exists to prevent.
[ "$OTHER" -eq 0 ] && [ "$RUN_GRADED" -gt 0 ] \
    && [ "$M3_RUN_FAIL" -eq 0 ] && [ "$M3_RUN_CRASH" -eq 0 ] && [ "$M3_RUN_HANG" -eq 0 ] \
    && [ "$M4_RUN_FAIL" -eq 0 ] && [ "$M4_RUN_CRASH" -eq 0 ] && [ "$M4_RUN_HANG" -eq 0 ]
