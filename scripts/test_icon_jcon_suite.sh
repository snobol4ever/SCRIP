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
# NO-ORACLE SOURCES EXCLUDED, NOT GRADED AS MISSING: link1/link2/load1/load2/tpp1-5 have no .std by
# design (dynamic-load tests meaningless for a compile-once model; template-preprocessor inputs, not
# standalone programs -- see README.md). Globbing only *.icn with a matching *.std sidesteps them
# without a hardcoded exclude list that would silently go stale.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"

set -uo pipefail

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
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
        *) echo "Usage: $0 [--mode all|m3|m4] [--scrip PATH] [--corpus PATH] [--timeout N]" >&2; exit 1 ;;
    esac
done

if [ ! -x "$SCRIP" ]; then
    echo "⛔ REFUSED TO GRADE: no scrip binary at $SCRIP — run make" >&2
    exit 2
fi
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
    if [ -f "$dat" ]; then IN="$dat"; extra_args=(-- "$dat"); fi
    case "$mode" in
        m3)
            timeout "$TIMEOUT" "$SCRIP" --run "$icn" "${extra_args[@]}" < "$IN" > "$outfile" 2>"$errf"
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
                timeout "$TIMEOUT" "$bin" "${extra_args[@]}" < "$IN" > "$outfile" 2>"$errf"
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

case "$MODE" in
    all) run_mode m3; run_mode m4 ;;
    m3)  run_mode m3 ;;
    m4)  run_mode m4 ;;
    *) echo "Usage: $0 [--mode all|m3|m4] [--scrip PATH] [--corpus PATH] [--timeout N]" >&2; exit 1 ;;
esac

total="${m3_TOTAL:-${m4_TOTAL:-0}}"
m3p="${m3_PASS:-}"; m4p="${m4_PASS:-}"
echo "JCON_SUITE_BOARD total=$total m3_pass=${m3p:-n/a} m4_pass=${m4p:-n/a}"
