#!/usr/bin/env bash
# test_prolog_gnu_suite.sh -- grades corpus/packages/prolog/gnu_prolog: GNU Prolog's OWN vendored
# compiler/library source (62 .pl files -- BipsPl/ = the built-in-predicates library, Pl2Wam/ = the
# Prolog-to-WAM compiler), NOT test programs. There is no external .ref oracle for these files; the
# earlier row `audit-corpus-what-is-ungated` found ZERO runner existed at all and ruled the fix:
# name every file, glob the runnable remainder, NEVER blanket-exclude the directory -- "a blanket
# exclusion and a named list give the same number today and different numbers forever after,
# permanently hiding the parse errors and the live files" (that file's own §2 CATEGORY 3 ruling).
# Per that same warning, this runner does NOT hardcode a file list either -- every run CLASSIFIES
# LIVE via the actual compile-probe signal, so it self-corrects if the vendored files or SCRIP's own
# frontend ever change, instead of silently drifting stale like a cached list would.
#
# THREE LIVE-DERIVED BUCKETS (never a fixed count -- see the classification loop for the exact rule):
#   LIB      -- ⚖️ RE-KEYED by hq_C ruling 2026-09-02 (row prolog-gnu-conformance-ok-fail-print-zero-
#               bytes-both-modes): classified by a PROPERTY OF THE FILE, checked BEFORE any SCRIP
#               invocation -- (1) it contains a GNU-Prolog-internal bootstrap-only directive
#               ($call_c/$call_c_test/$call_c_jump/ensure_linked/built_in), or (2) it is one of two
#               individually-named non-program drivers (Pl2Wam/all.pl, Pl2Wam/whole.pl -- see
#               is_bootstrap_only() below for why). The ORIGINAL key was `./scrip --compile` reaching
#               exactly "[IBB] FATAL: mode-4 driver: main BB graph not found" -- a SCRIP FAILURE
#               SIGNAL, not a file property, so it silently re-classified when commit 3ce7a526 gave
#               SCRIP a universal entry-point synthesis and made that signal structurally unreachable
#               (45 LIB files fell into OK overnight with no corpus or suite change at all). The old
#               rc==1 signal check below is left in place as a dormant fallback, not the key.
#   OK       -- `./scrip --compile` succeeds (rc=0, the file carries its own `:- initialization(...)`
#               directive). Run-graded, triangulated three ways: SCRIP m3 (--run), SCRIP m4
#               (--compile, link, execute), and real `gprolog`, all invoked with ZERO command-line
#               arguments (matching how every other suite runner in this repo invokes a file). These
#               4 files are GNU Prolog's own compiler-driver entry points and no-op cleanly on an
#               empty argument_list (confirmed by reading BipsPl/Pl2Wam source directly -- e.g.
#               Pl2Wam/ciaolib.pl's `go_other1([]) :- !.` clause) -- empty-output agreement across
#               all three arms IS the correct result here, not the vacuous-oracle trap ceo's
#               stdin-freeze incident was about: nothing is cached or minted from this run, and the
#               board prints the literal byte count of each arm's output so a silent "0,0,0" is never
#               hidden behind a bare PASS.
#   REJECT   -- times out. A real, ALREADY-KNOWN class (`misc-single-witness-parser-crashes`' own
#               claim: these hang after a parse-error, a defect in error recovery, not a fresh bug).
#               Documented here, not graded PASS/FAIL, not counted against the exit code.
#   UNEXPECTED -- anything that is none of the above (a rc outside {0,1}, or an rc=1 whose output does
#               NOT contain the LIB signal). Loudly flagged and FAILS the gate -- a genuinely new
#               failure shape must never be silently folded into LIB or REJECT.
#
# AUTHORS: seat05, 2026-08-30 (Lon direct via ceo, row gnu-prolog-suite-runner-and-score)
set -uo pipefail   # deliberately NOT -e: a per-file compile/run failure is DATA the board must finish
                   # printing, never a reason to abort the whole sweep partway through

S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
[ -f "$HERE/lib_progress.sh" ] && . "$HERE/lib_progress.sh"   # CEO-331: a runner that grades with its OWN loop appends its own rows
. "$HERE/lib_flag_gate.sh" 2>/dev/null || { echo "⛔ REFUSED-TO-GRADE: lib_flag_gate.sh unloadable"; exit 2; }
. "$HERE/lib_inventory.sh" 2>/dev/null || { echo "⛔ REFUSED-TO-GRADE: lib_inventory.sh unloadable"; exit 2; }
[ $# -eq 0 ] || flaggate_reject "$1" "(none -- set GNU_SUITE_VERBOSE / GNU_SUITE_CLASSIFY_TIMEOUT / GNU_SUITE_RUN_TIMEOUT via environment instead)"
SCRIP="${HERE}/../scrip"
RT_SO="${HERE}/../out/libscrip_rt.so"
PKG="$S4E/corpus/packages/prolog/gnu_prolog"
CLASSIFY_TIMEOUT="${GNU_SUITE_CLASSIFY_TIMEOUT:-10}"
RUN_TIMEOUT="${GNU_SUITE_RUN_TIMEOUT:-15}"
GPROLOG_BIN="$(command -v gprolog || true)"
VERBOSE="${GNU_SUITE_VERBOSE:-0}"

# Named individually per the ruling (neither contains a bootstrap-only directive, so the content
# grep below can't catch them): Pl2Wam/all.pl is a bare sequence of `:- include(...)` directives
# concatenating the other Pl2Wam sources, with its one real `:- initialization(go).` line commented
# out in this vendored copy; Pl2Wam/whole.pl's `:- include(bip_list).` names a file absent from this
# vendored subset (a corpus completeness gap, tracked separately -- see handoff/ledger, not fixed here).
BOOTSTRAP_ONLY_NAMED=("Pl2Wam/all.pl" "Pl2Wam/whole.pl")

is_bootstrap_only() {  # $1 = rel path (for the named list), $2 = full path (for the content grep)
    local rel="$1" f="$2" n
    for n in "${BOOTSTRAP_ONLY_NAMED[@]}"; do [ "$rel" = "$n" ] && return 0; done
    grep -qE '\$call_c|ensure_linked|built_in' "$f"
}

[ -d "$PKG" ]        || { echo "⛔ REFUSED-TO-GRADE: $PKG missing"; exit 2; }
[ -x "$SCRIP" ]      || { echo "⛔ REFUSED-TO-GRADE: scrip not built"; exit 2; }
# ⛔⭐ STALE-BINARY PREFLIGHT (row harness-and-ladder-runner-refuse-on-a-stale-binary-like-the-artifact-regen-
# does, ceo -> hq_T 2026-09-04). The line above proves a binary EXISTS; this one proves it is the binary this
# tree describes. ceo's witness, twice on 2026-09-04: a 10:57 binary graded at 14:03 read RED, then GREEN after
# an incremental make -- a vendor board is exactly where that is least visible, because a plausible all-FAIL
# table is this class's normal output. NO LOGIC HERE: util_require_fresh.sh sources gate_require_fresh from
# lib_gate.sh, the ONE authority (hq_B 4c7253e99) -- never a second copy of the staleness rule.
"$HERE/util_require_fresh.sh" --gate test_prolog_gnu_suite "$SCRIP" "${RT_DIR:-$HERE/../out}/libscrip_rt.so" || exit 2
[ -f "$RT_SO" ]      || { echo "⛔ REFUSED-TO-GRADE: $RT_SO missing (m4 link needs it)"; exit 2; }
[ -n "$GPROLOG_BIN" ] || { echo "⛔ REFUSED-TO-GRADE: gprolog not on PATH"; exit 2; }

TMP="$(mktemp -d /tmp/gnu_suite_XXXXXX)"
trap 'rm -rf "$TMP"' EXIT

mapfile -t FILES < <(find "$PKG" -name "*.pl" | sort)
TOTAL=${#FILES[@]}
[ "$TOTAL" -gt 0 ] || { echo "⛔ REFUSED-TO-GRADE: zero .pl files found under $PKG"; exit 2; }

LIB=0; OK_TOTAL=0; OK_PASS=0; OK_FAIL=0; REJECT=0; UNEXPECTED=0; LADDER=0
UNEXPECTED_NAMES=(); REJECT_NAMES=(); OK_FAIL_NAMES=()
declare -A LADDER_RUNG_COUNT LADDER_RUNG_NAMES

echo "=== GNU Prolog vendored suite ($TOTAL files, $PKG) ==="

for f in "${FILES[@]}"; do
    base="$(basename "$f" .pl)"
    rel="${f#"$PKG"/}"
    out="$TMP/${base}.s"
    probe_log="$TMP/${base}.probe"

    if is_bootstrap_only "$rel" "$f"; then
        LIB=$((LIB+1))
        [ "$VERBOSE" -eq 1 ] && echo "  LIB (bootstrap-only file, no SCRIP invocation) $rel"
        continue
    fi

    timeout "$CLASSIFY_TIMEOUT" "$SCRIP" --compile "$f" -o "$out" < /dev/null > "$probe_log" 2>&1
    rc=$?

    if [ "$rc" -eq 124 ]; then
        REJECT=$((REJECT+1)); REJECT_NAMES+=("$rel")
        [ "$VERBOSE" -eq 1 ] && echo "  REJECT (hang after parse error, known class) $rel"
        continue
    fi

    if [ "$rc" -eq 0 ]; then
        OK_TOTAL=$((OK_TOTAL+1))
        # -- triangulate: SCRIP m3, SCRIP m4 (link+run the .s this classify pass already produced), gprolog --
        m3_out=$(timeout "$RUN_TIMEOUT" "$SCRIP" --run "$f" < /dev/null 2>/dev/null)
        bin="$TMP/${base}.bin"
        m4_out=""
        if gcc -no-pie "$out" -L "${HERE}/../out" -lscrip_rt -Wl,-rpath,"${HERE}/../out" -o "$bin" 2>/dev/null; then
            m4_out=$(timeout "$RUN_TIMEOUT" "$bin" < /dev/null 2>/dev/null)
        fi
        # ⛔⭐ --init-goal RUNS *BEFORE* --consult-file, SO THE FILE'S OWN `:- initialization(...)`
        # DIRECTIVE NEVER FIRES (verified by hand: --init-goal halt produces silent empty output on a
        # file proven, by a separate plain-consult run, to print real text on init -- the exact
        # "oracle answers when it should refuse" shape this project keeps re-discovering, this time in
        # a fresh binary rather than a fresh flag). The correct, ALREADY-PROVEN invocation lives in
        # test_bench_prolog_modes.sh: --query-goal (runs AFTER consult, so the file's own
        # initialization has already executed) + an explicit banner-strip, reused verbatim rather than
        # re-derived, since it is already battle-tested.
        gp_raw=$(timeout "$RUN_TIMEOUT" "$GPROLOG_BIN" --consult-file "$f" --query-goal halt < /dev/null 2>/dev/null)
        # ⛔⭐ THE OLD `^error:`/`^warning:` ANCHORS NEVER MATCHED A REAL GPROLOG LINE (row
        # prolog-gnu-suite-ciaolib-use-module-warning): gprolog's own compile-time diagnostics are
        # prefixed `<path>:<line>[-<line>]: warning: ...` / `<path>:<line>[-<line>]: fatal error: ...`
        # -- never bare `warning:`/`error:` at column 0 -- so EVERY file where gprolog warns (an
        # unrecognized directive, e.g. `use_module/1`) or hard-fails to compile (e.g. `use_module/2`
        # with a non-atom module term, or a bootstrap-only `$call_c`/`$call_c_test` builtin) leaked
        # gprolog's own tooling chatter into gp_out as if it were program output. Measured: 41 of 62
        # files in this suite were OK_FAIL for exactly this reason (m3=m4=0B, gprolog non-zero), all
        # of it gprolog's own diagnostic text, not a real SCRIP divergence -- see the FINDING for the
        # full board before/after. Matched by line-number prefix instead of line start; the trailing
        # summary line gets its own literal match since it carries no line number.
        gp_out=$(printf '%s\n' "$gp_raw" | grep -vE '^GNU Prolog|^Compiled |^By Daniel|^Copyright|^compiling |compiled, |^\| \?-|^error:|^warning:|cannot be redefined|:[0-9]+(-[0-9]+)?: *(fatal error|error|warning):|^compilation failed$')

        # ⛔⭐ CEO-331: one progress row per program per mode, from a runner that grades with its OWN loop (the coverage
        # report read gnu as MISSING, 0 of 62). ⛔ THIS SUITE'S VERDICT IS A THREE-WAY AGREEMENT m3=m4=gprolog, so a
        # per-MODE outcome cannot be derived from it without lying about what was compared -- a mode is not graded
        # independently here. Both rows carry the SAME agreement verdict and the note names the comparison, rather than
        # inventing a per-mode result the runner never computed.
        if command -v progress_append >/dev/null 2>&1; then
            if [ "$m3_out" = "$m4_out" ] && [ "$m4_out" = "$gp_out" ]; then _gv=PASS; else _gv=FAIL; fi
            progress_append package gnu prolog "$rel" m3 "$_gv" 0 "three-way m3=m4=gprolog" || true
            progress_append package gnu prolog "$rel" m4 "$_gv" 0 "three-way m3=m4=gprolog" || true
        fi
        if [ "$m3_out" = "$m4_out" ] && [ "$m4_out" = "$gp_out" ]; then
            OK_PASS=$((OK_PASS+1))
            [ "$VERBOSE" -eq 1 ] && echo "  OK PASS $rel (m3=m4=gprolog, $(printf '%s' "$m3_out" | wc -c) bytes)"
        else
            OK_FAIL=$((OK_FAIL+1)); OK_FAIL_NAMES+=("$rel")
            echo "  OK FAIL $rel -- m3/$( printf '%s' "$m3_out" | wc -c)B m4/$( printf '%s' "$m4_out" | wc -c)B gprolog/$( printf '%s' "$gp_out" | wc -c)B disagree"
            if [ "$VERBOSE" -eq 1 ]; then
                echo "    m3:      $(printf '%s' "$m3_out" | head -c 200)"
                echo "    m4:      $(printf '%s' "$m4_out" | head -c 200)"
                echo "    gprolog: $(printf '%s' "$gp_out" | head -c 200)"
            fi
        fi
        continue
    fi

    # rc == 2 with the ladder-refusal shape (lower_prolog.c pl_refuse()): a construct not yet built on
    # the construct ladder. Named and counted by rung for diagnostic visibility, and the RUNG ITSELF is
    # never cured here -- a fleet seat never touches a rung (RULES.md sec THE PROLOG REBUILD GATE;
    # ARCH-PROLOG-BYRD-BOX-TRANSLATION.md sec E). ⛔ BUT per lib_inventory.sh's ARM 8 ("a program the
    # ORACLE grades and WE do not is GRADED and RED, never removed from the denominator"): gprolog can
    # run every one of these files, so a ladder gap is OUR compiler's gap, not an oracle ruling or an
    # owed-ref-cutting task -- excluding it from OK_TOTAL was a red moved out of the denominator one
    # level up from the case ARM 8 was written for. It counts as an ordinary OK/FAIL entry; SCRIP's own
    # refusal to lower the construct is itself the divergence from gprolog's real output, so there is
    # nothing left to triangulate.
    rung="$(grep -oP 'is not on the ladder yet -- rung \K[0-9]+' "$probe_log" | head -1)"
    if [ "$rc" -eq 2 ] && [ -n "$rung" ]; then
        LADDER=$((LADDER+1))
        LADDER_RUNG_COUNT[$rung]=$(( ${LADDER_RUNG_COUNT[$rung]:-0} + 1 ))
        LADDER_RUNG_NAMES[$rung]="${LADDER_RUNG_NAMES[$rung]:-}${LADDER_RUNG_NAMES[$rung]:+ }$rel"
        OK_TOTAL=$((OK_TOTAL+1)); OK_FAIL=$((OK_FAIL+1)); OK_FAIL_NAMES+=("$rel(ladder rung $rung)")
        [ "$VERBOSE" -eq 1 ] && echo "  OK FAIL (ladder rung $rung not built yet, SCRIP cannot lower this construct) $rel"
        continue
    fi

    # rc == 1 (or any other non-timeout, non-zero code): must be exactly the LIB signal to count as LIB
    if grep -q "\[IBB\] FATAL: mode-4 driver: main BB graph not found" "$probe_log"; then
        LIB=$((LIB+1))
        [ "$VERBOSE" -eq 1 ] && echo "  LIB (no entry point, compile-graded) $rel"
    else
        UNEXPECTED=$((UNEXPECTED+1)); UNEXPECTED_NAMES+=("$rel (rc=$rc)")
        echo "  ⛔ UNEXPECTED $rel -- rc=$rc, not the known LIB/OK/REJECT shape:"
        sed 's/^/      /' "$probe_log" | head -5
    fi
done

echo ""
echo "-- REJECT (known hang-after-parse-error class, misc-single-witness-parser-crashes -- documented, not graded): $REJECT --"
for n in "${REJECT_NAMES[@]:-}"; do [ -n "$n" ] && echo "   $n"; done

echo ""
echo "-- LADDER (construct not yet on the ladder -- a SUBSET of OK_FAIL above, named by rung for diagnostic visibility; the rung itself is never cured by a fleet seat, RULES.md sec THE PROLOG REBUILD GATE): $LADDER --"
for rung in $(printf '%s\n' "${!LADDER_RUNG_COUNT[@]}" | sort -n); do
    echo "   rung $rung: ${LADDER_RUNG_COUNT[$rung]} --${LADDER_RUNG_NAMES[$rung]}"
done

if [ "$UNEXPECTED" -gt 0 ]; then
    echo ""
    echo "-- UNEXPECTED (genuinely new failure shape, not LIB/OK/REJECT -- needs investigation): $UNEXPECTED --"
    for n in "${UNEXPECTED_NAMES[@]}"; do echo "   $n"; done
fi

echo ""
echo "GNU_SUITE_BOARD total=$TOTAL lib=$LIB ok=$OK_TOTAL ok_pass=$OK_PASS/$OK_TOTAL ok_fail=$OK_FAIL reject=$REJECT ladder=$LADDER(subset of ok_fail) unexpected=$UNEXPECTED"
# ⭐ THE PACKAGE LOCKDOWN inventory line, via the shared body (lib_inventory.sh) -- never a second copy
# of the arithmetic. LIB (bootstrap/library files, no independent behavior) is a ruling about what the
# file IS, so it is UNGRADABLE/CONTAINER_OR_LIBRARY; REJECT (hang after parse error, a known, filed,
# fixable defect in error recovery) is owed work, so it is UNGRADED/TIMEOUT; OK_TOTAL already includes
# the former LADDER entries per ARM 8 above, so graded_stream is OK_TOTAL with nothing left over.
INV_PACKAGE=gnu_prolog; INV_DIR="$PKG"; INV_EXT=".pl"
INV_LINE="$(inventory_line "$OK_TOTAL" 0)"
if [ -n "$INV_LINE" ]; then echo "$INV_LINE"; else echo "⚠ inventory refused (above) -- the board line still stands; the inventory does not" >&2; fi
# ⛔ ONE LEADERBOARD (RULES.md FACT RULE, Lon 2026-09-03 ~16:05: "any run of a test suite by any
# session will update the ONE LEADERBOARD"). This records the board line printed just above into
# .github/SCORE.md -- it RUNS NOTHING, it only writes down what this script already measured.
# ⛔ NON-FATAL BY DESIGN: a bookkeeping failure must never turn a real measurement into a red board,
# because a gate that goes red for a reason unrelated to the code is a gate people route around. It
# warns and names the unrecorded row instead; it has no silent path.
python3 "$HERE/util_score_row.py" write --lang prolog --column vendor --suite GNU \
    --measurer "${S4E_SEAT:-}" --text "ok_pass=$OK_PASS/$OK_TOTAL ok_fail=$OK_FAIL reject=$REJECT ladder=$LADDER(of ok_fail) unexpected=$UNEXPECTED lib=$LIB of total=$TOTAL${INV_LINE:+ · $INV_LINE (\`test_prolog_gnu_suite.sh\`)}" \
    || echo "⚠ SCORE.md NOT UPDATED -- record this row by hand (the REFUSED line above says why)"


# ⛔⭐ POPULATION FLOOR (row every-board-wrapper-refuses-on-a-zero-population-instead-of-passing-
# vacuously, hq_T 2026-09-04): the bucket-sum check just below PASSES vacuously at TOTAL=0 (0==0),
# and OK_FAIL/UNEXPECTED read 0 too when nothing was discovered -- refuse first.
"$HERE/util_require_population.sh" --gate test_prolog_gnu_suite "$TOTAL" 1 "prolog GNU source files discovered" || exit 2
[ "$((LIB + OK_TOTAL + REJECT + UNEXPECTED))" -eq "$TOTAL" ] || { echo "⛔ BUCKET COUNTS DON'T SUM TO TOTAL -- instrument bug, refusing to trust the board"; exit 2; }
[ "$OK_FAIL" -eq 0 ] && [ "$UNEXPECTED" -eq 0 ]
