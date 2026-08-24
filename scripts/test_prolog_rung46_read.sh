#!/usr/bin/env bash
# test_prolog_rung46_read.sh — PL-ISO-7a driver: read/1 + read_term/2 term reading from current input.
# The main rung suite feeds every case `< /dev/null`, so it can only exercise read/1's end_of_file
# contract (rung46_read_*.pl). This driver feeds REAL terms on stdin and checks all three engine
# paths (interp/run via --run, compile via the x86 backend with stdin forwarded) against the gprolog
# 1.4.5 oracle. Term shapes stay in canonical functional notation (atom/int/list/compound/nested):
# infix-operator reading (1+2, X=5) is the documented PL-ISO-7a follow-up (full-parser reader).
# Self-contained. Authors: LCherryholmes · Jeffrey Cooper M.D. · Claude Opus
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${SCRIP:-$HERE/../scrip}"
LIBRT="${LIBRT:-$HERE/../out/libscrip_rt.so}"
GPROLOG="${GPROLOG:-gprolog}"
PROG="$(mktemp /tmp/rung46_XXXXXX.pl)"
printf ':- initialization((main, halt)).\nmain :- read(X), write(X), nl.\n' > "$PROG"
PROG2="$(mktemp /tmp/rung46b_XXXXXX.pl)"
printf ':- initialization((main, halt)).\nmain :- read(X), read(Y), write(X-Y), nl.\n' > "$PROG2"
trap 'rm -f "$PROG" "$PROG2"' EXIT
oracle() { printf '%s' "$2" | "$GPROLOG" --consult-file "$1" 2>/dev/null | grep -vE 'GNU Prolog|Compiled |By Daniel|Copyright|compiling |compiled,|bytes written|initialization|\?-|^$' | tail -1; }
run_interp() { printf '%s' "$2" | timeout 8 "$SCRIP" --run "$1" 2>/dev/null; }
run_compile() {
    local pl="$1" inp="$2" W; W="$(mktemp -d)"
    ( timeout 8 "$SCRIP" --compile --target=x86 "$pl" > "$W/p.s" 2>/dev/null < /dev/null ) || { rm -rf "$W"; return 1; }
    as --64 -o "$W/p.o" "$W/p.s" 2>/dev/null || { rm -rf "$W"; return 1; }
    gcc -no-pie -o "$W/pbin" "$W/p.o" "$LIBRT" -lm -lstdc++ -Wl,-rpath,"$(dirname "$LIBRT")" 2>/dev/null || { rm -rf "$W"; return 1; }
    printf '%s' "$inp" | timeout 8 "$W/pbin" 2>/dev/null; rm -rf "$W"
}
[ -x "$SCRIP" ] || { echo "⛔ REFUSED-TO-GRADE scrip not built"; exit 2; }
command -v "$GPROLOG" >/dev/null 2>&1 || { echo "⛔ REFUSED-TO-GRADE gprolog oracle not installed"; exit 2; }
echo "=== rung46_read: read/1 + read_term/2 real-term reading (PL-ISO-7a) — oracle=gprolog 1.4.5 ==="
PASS=0; FAIL=0
check() {
    local prog="$1" inp="$2" want got
    want="$(oracle "$prog" "$inp")"
    for mode in interp compile; do
        if [ "$mode" = interp ]; then got="$(run_interp "$prog" "$inp")"; else got="$(run_compile "$prog" "$inp")"; fi
        if [ "$got" = "$want" ]; then echo "  PASS [$mode] read <$inp> -> $got"; PASS=$((PASS+1)); else echo "  FAIL [$mode] read <$inp>: want=<$want> got=<$got>"; FAIL=$((FAIL+1)); fi
    done
}
check "$PROG" 'hello.'
check "$PROG" '42.'
check "$PROG" '[1,2,3].'
check "$PROG" 'f(g(x),[a,b]).'
check "$PROG" 'parent(tom,bob).'
check "$PROG2" 'alpha. beta.'
echo ""
echo "PASS=$PASS FAIL=$FAIL"
[ "$FAIL" -eq 0 ]
