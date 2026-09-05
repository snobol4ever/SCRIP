#!/usr/bin/env bash
# scripts/test_icon_jcon_suite.sh — grades SCRIP m3+m4 against the vendored JCON test suite
# (corpus/packages/icon/jcon_tests/: 91 .icn, 83 with a .std oracle, 21 with a .dat companion).
# Row jcon-tests-vendor-script-run. Self-contained. Run from anywhere with no env vars.
#
# FOUR-WAY VERDICT, not the usual PASS/FAIL/REFUSED: a jcon test is real, unmodified upstream Icon
# (see corpus/packages/icon/jcon_tests/README.md -- deliberately NOT semicolon-converted, unlike
# jcon-compiler/), so a program written in a construct SCRIP's semicolon-required dialect rejects at
# PARSE time is not a runtime defect -- it is a DIALECT GAP, and this suite's own GOAL requires it be
# counted and NAMED as its own class (REJECT), never silently folded into FAIL or dropped. Detected by
# scrip's own stable signature: "icon: parse error" on stderr, both modes (verified: m3 and m4 both
# print it verbatim and both exit rc=1 on the identical construct).
#
# .dat COMPANION CONVENTION: JCON's own `addtest` harness feeds a .dat file BOTH as argv[1] AND as
# stdin (`./prog file.dat <file.dat >file.std`) -- reproduced here exactly, not stdin-only (a jcon test
# reading *args as well as reading stdin would silently see an empty argv otherwise).
#
# CWD FIX (seat02, 2026-09-05, icon-jcon-suite-39 11th pass): a jcon program may ALSO open its own .dat
# companion by a bare relative literal (e.g. fncs1.icn's `open("fncs1.dat")`), which this script's own
# invocation cwd never satisfied before -- FAIL was actually "file not found", not a compiler defect,
# a pure instrument bug (confirmed: fncs1 diffs 0 lines once its cwd is right). Each test now runs from
# its own private $WORK/<name>.rundir/ (never $CORPUS itself -- a program that WRITES a scratch file by
# relative name, e.g. loadfunc.icn's tmp.icn/foo.baz, must never touch the tracked corpus working tree)
# with the .dat companion copied in under its own basename; only the KNOWN .dat convention is covered,
# not arbitrary other literal filenames a program might reference (recent.icn's `open(".")` and its own
# `open("recent.dat")` remain unaddressed -- moot today since it dies earlier on an unrelated sortf bug,
# see FINDING-2026-09-05-seat02-icon-jcon-suite-census-11th-pass*.md).
#
# NO-ORACLE SOURCES EXCLUDED, NOT GRADED AS MISSING: link1/link2/load1/load2/tpp1-5 have no .std by
# design (dynamic-load tests meaningless for a compile-once model; template-preprocessor inputs, not
# standalone programs -- see README.md). Globbing only *.icn with a matching *.std sidesteps them
# without a hardcoded exclude list that would silently go stale.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"

set -uo pipefail

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
. "$HERE/lib_flag_gate.sh" 2>/dev/null || { echo "⛔ REFUSED TO GRADE: lib_flag_gate.sh unloadable" >&2; exit 2; }
ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP:-$ROOT/scrip}"
RT_SO="${RT_SO:-$ROOT/out/libscrip_rt.so}"
CORPUS="${CORPUS:-$S4E/corpus/packages/icon/jcon_tests}"
MODE="all"
TIMEOUT=20

while [[ $# -gt 0 ]]; do
    case "$1" in
        --mode)   MODE="$2";   shift 2 ;;
        --scrip)  SCRIP="$2";  shift 2 ;;
        --corpus) CORPUS="$2"; shift 2 ;;
        --timeout) TIMEOUT="$2"; shift 2 ;;
        *) flaggate_reject "$1" "--mode --scrip --corpus --timeout" ;;
    esac
done

if [ ! -x "$SCRIP" ]; then
    echo "⛔ REFUSED TO GRADE: no scrip binary at $SCRIP — run make" >&2
    exit 2
fi
# ⛔⭐ STALE-BINARY PREFLIGHT (row harness-and-ladder-runner-refuse-on-a-stale-binary-like-the-artifact-regen-
# does, ceo -> hq_T 2026-09-04). The line above proves a binary EXISTS; this one proves it is the binary this
# tree describes. ceo's witness, twice on 2026-09-04: a 10:57 binary graded at 14:03 read RED, then GREEN after
# an incremental make -- a vendor board is exactly where that is least visible, because a plausible all-FAIL
# table is this class's normal output. NO LOGIC HERE: util_require_fresh.sh sources gate_require_fresh from
# lib_gate.sh, the ONE authority (hq_B 4c7253e99) -- never a second copy of the staleness rule.
"$HERE/util_require_fresh.sh" --gate test_icon_jcon_suite "$SCRIP" "${RT_DIR:-$HERE/../out}/libscrip_rt.so" || exit 2
if [ ! -d "$CORPUS" ]; then
    echo "⛔ REFUSED TO GRADE: no jcon_tests corpus at $CORPUS" >&2
    exit 2
fi

OUTDIR="$(dirname "$RT_SO")"
WORK="$(mktemp -d)"
trap 'rm -rf "$WORK"' EXIT

# classify one (mode, file) pair. Echoes "KIND" on stdout; KIND in PASS|FAIL|REJECT|CRASH|HANG.
# stdout of the PROGRAM itself is captured to $2 (a path), never mixed with our own echo.
run_one() {
    local mode="$1" icn="$2" want="$3" outfile="$4"
    local base="${icn%.icn}" name errf rc
    name=$(basename "$icn" .icn)
    errf="$WORK/err.txt"; : > "$errf"
    local dat="${base}.dat" IN=/dev/null extra_args=()
    local rundir="$WORK/$name.rundir"; mkdir -p "$rundir"
    if [ -f "$dat" ]; then IN="$dat"; extra_args=(-- "$dat"); cp "$dat" "$rundir/$(basename "$dat")"; fi
    case "$mode" in
        m3)
            ( cd "$rundir" && timeout "$TIMEOUT" "$SCRIP" --run "$icn" "${extra_args[@]}" < "$IN" > "$outfile" 2>"$errf" )
            rc=$?
            ;;
        m4)
            local s="$WORK/$name.s" o="$WORK/$name.o" bin="$WORK/${name}_bin"
            if ! timeout "$TIMEOUT" "$SCRIP" --compile --target=x86 "$icn" < /dev/null > "$s" 2>"$errf"; then
                : > "$outfile"; rc=1
            elif grep -q 'icon: parse error' "$errf"; then
                : > "$outfile"; rc=1
            elif ! as "$s" -o "$o" 2>>"$errf"; then
                : > "$outfile"; rc=1
            elif ! gcc -no-pie "$o" -L"$OUTDIR" -lscrip_rt -Wl,-rpath,"$OUTDIR" -lm -o "$bin" 2>>"$errf"; then
                : > "$outfile"; rc=1
            else
                ( cd "$rundir" && timeout "$TIMEOUT" "$bin" "${extra_args[@]}" < "$IN" > "$outfile" 2>"$errf" )
                rc=$?
            fi
            ;;
    esac
    if [ "$rc" -eq 124 ]; then echo "HANG"; return; fi
    if grep -q 'icon: parse error' "$errf"; then echo "REJECT"; return; fi
    if [ "$rc" -ge 128 ]; then echo "CRASH"; return; fi
    if [ "$rc" -ne 0 ]; then echo "FAIL"; return; fi
    if diff -q "$outfile" "$want" >/dev/null 2>&1; then echo "PASS"; else echo "FAIL"; fi
}

run_mode() {
    local mode="$1"
    local pass=0 fail=0 reject=0 crash=0 hang=0
    local -a reject_names=() fail_names=() crash_names=() hang_names=()
    local icn std kind outfile
    for icn in "$CORPUS"/*.icn; do
        [ -f "$icn" ] || continue
        std="${icn%.icn}.std"
        [ -f "$std" ] || continue   # no-oracle source (link*/load*/tpp*) — excluded, not MISSING
        case "$(basename "$icn")" in tpp.icn) continue;; esac   # tpp.std is jcon PREPROCESSOR TEXT output, not program output (its body is deliberately-invalid Icon like `abc 11`); ungradable by execution — named exclusion, same class as the no-.std sources above
        outfile="$WORK/out.txt"
        kind=$(run_one "$mode" "$icn" "$std" "$outfile")
        name=$(basename "$icn" .icn)
        case "$kind" in
            PASS)   pass=$((pass+1)) ;;
            FAIL)   fail=$((fail+1)); fail_names+=("$name") ;;
            REJECT) reject=$((reject+1)); reject_names+=("$name") ;;
            CRASH)  crash=$((crash+1)); crash_names+=("$name") ;;
            HANG)   hang=$((hang+1)); hang_names+=("$name") ;;
        esac
    done
    local mode_total=$((pass+fail+reject+crash+hang))
    echo "--- jcon ($mode): PASS=$pass FAIL=$fail REJECT=$reject CRASH=$crash HANG=$hang TOTAL=$mode_total ---"
    [ "$reject" -gt 0 ] && echo "    REJECT (dialect gap, semicolon-required): ${reject_names[*]}"
    [ "$fail" -gt 0 ]   && echo "    FAIL (wrong output): ${fail_names[*]}"
    [ "$crash" -gt 0 ]  && echo "    CRASH: ${crash_names[*]}"
    [ "$hang" -gt 0 ]   && echo "    HANG (>${TIMEOUT}s): ${hang_names[*]}"
    eval "${mode}_PASS=$pass"
    # ⛔ TOTAL IS THE LOOP'S OWN DENOMINATOR, NEVER A SEPARATE `ls *.std` RECOUNT: jcon_tests carries one
    # orphaned .std (linking.std) with no matching .icn -- a stray in the vendored upstream, harmless to
    # leave in place (provenance), but counting it as part of TOTAL would assert a witness this suite
    # never actually graded. RULES.md § A PROBE ASSERTS ITS NAMED WITNESS, NEVER A POSITIONAL DENOMINATOR.
    eval "${mode}_TOTAL=$mode_total"
}

# ⛔⭐ POPULATION LAW (Lon, ruled 2026-09-04 via hq_B for the Arizona suite; brought to jcon by hq_I
# 2026-09-05 on ceo ruling CEO-294 "jcon onto the arizona population shape"). THE COUNTED POPULATION IS
# EVERY .icn THIS PACKAGE SHIPS, not merely the subset carrying a .std oracle today. A shipped program with
# no oracle is UNGRADED -- zero of the population, never PASS -- and it may NEVER be silently dropped from
# the denominator. SHIPPED/GRADED/GAP are computed fresh on every run, never hand-maintained, and the GAP is
# NAMED rather than merely counted, so a reader can see exactly which programs are carrying a zero.
# ⚠️ THIS MOVES THE PUBLISHED FRACTION AND THAT IS THE POINT, NOT A REGRESSION: jcon reported m3 45/81
# before this landed and reports m3 45/91 after it. Identical passes, honest denominator. Anyone diffing the
# leaderboard across this commit must read it as a denominator change, not as ten programs breaking.
SHIPPED=0; GRADED=0; GAP_NAMES=""
for _icn in "$CORPUS"/*.icn; do
    [ -f "$_icn" ] || continue
    SHIPPED=$((SHIPPED+1))
    _b="$(basename "$_icn" .icn)"
    if [ ! -f "${_icn%.icn}.std" ]; then GAP_NAMES="$GAP_NAMES $_b(no .std shipped upstream)"; continue; fi
    case "$_b" in tpp) GAP_NAMES="$GAP_NAMES tpp(.std is jcon PREPROCESSOR text, not program output)"; continue;; esac
    GRADED=$((GRADED+1))
done
GAP=$((SHIPPED-GRADED))
# ⛔ REFUSE rather than grade a population the census could not read: a zero SHIPPED would make every rate
# below vacuously perfect and print the success shape over nothing (RULES.md -- a test that cannot measure
# REFUSES rc=2, it never skips as success).
if [ "$SHIPPED" -eq 0 ] || [ "$GRADED" -eq 0 ]; then
    echo "⛔ GATE REFUSES(2): censused shipped=$SHIPPED graded=$GRADED under $CORPUS -- nothing to grade" >&2
    exit 2
fi
echo "=== JCON vendored suite (upstream jcon, unmodified standard Icon) — shipped=$SHIPPED graded=$GRADED gap=$GAP ==="

case "$MODE" in
    all) run_mode m3; run_mode m4 ;;
    m3)  run_mode m3 ;;
    m4)  run_mode m4 ;;
    *) echo "Usage: $0 [--mode all|m3|m4] [--scrip PATH] [--corpus PATH] [--timeout N]" >&2; exit 1 ;;
esac

total="${m3_TOTAL:-${m4_TOTAL:-0}}"
m3p="${m3_PASS:-}"; m4p="${m4_PASS:-}"
# ⛔ THE CENSUS AND THE GRADING LOOP MUST AGREE ON WHAT WAS GRADED, and this is the one place that can be
# checked rather than assumed. They apply the SAME two exclusions by two independent walks; if they ever
# diverge, one of them is wrong about the population and every rate printed here is unsound. Refuse loudly
# rather than publish a number whose denominator two parts of this script disagree about.
if [ "$total" -ne "$GRADED" ]; then
    echo "⛔ GATE REFUSES(2): census says graded=$GRADED, the grading loop graded total=$total -- the two"  >&2
    echo "    walks disagree about the population, so neither rate is trustworthy. Reconcile before quoting." >&2
    exit 2
fi
[ -n "$GAP_NAMES" ] && echo "UNGRADED ($GAP of $SHIPPED shipped, zero of the population until graded):$GAP_NAMES"
echo "JCON_SUITE_BOARD shipped=$SHIPPED graded=$GRADED gap=$GAP total=$total m3_pass=${m3p:-n/a} m4_pass=${m4p:-n/a}"
# ⛔ ONE LEADERBOARD (RULES.md FACT RULE, Lon 2026-09-03 ~16:05: "any run of a test suite by any
# session will update the ONE LEADERBOARD"). This records the board line printed just above into
# .github/SCORE.md -- it RUNS NOTHING, it only writes down what this script already measured.
# ⛔ NON-FATAL BY DESIGN: a bookkeeping failure must never turn a real measurement into a red board,
# because a gate that goes red for a reason unrelated to the code is a gate people route around. It
# warns and names the unrecorded row instead; it has no silent path.
python3 "$HERE/util_score_row.py" write --lang icon --column vendor --suite JCON --modes m3,m4 \
    --measurer "${S4E_SEAT:-}" --text "m3 ${m3p:-n/a}/$SHIPPED · m4 ${m4p:-n/a}/$SHIPPED (of $SHIPPED shipped, $GRADED graded, $GAP ungraded, \`test_icon_jcon_suite.sh\`)" \
    || echo "⚠ SCORE.md NOT UPDATED -- record this row by hand (the REFUSED line above says why)"

