#!/usr/bin/env bash
# test_smoke_snobol4.sh — SNOBOL4 smoke, modes 2 + 3 + 4
# Gate: mode-4 HARD. Modes 2 and 3 informational (PASS/FAIL reported, do not block).
# AUTHORS: Lon Jones Cherryholmes · Jeffrey Cooper M.D. · Claude Sonnet 4.6  DATE: 2026-06-08
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${HERE}/../scrip"
OUTDIR="${HERE}/../out"
P2=0; F2=0; P3=0; F3=0; P4=0; F4=0

run_m2() {
    local label="$1" src="$2" expected="$3"
    local tmp; tmp=$(mktemp /tmp/sno_XXXXXX.sno)
    printf '%s\n' "$src" > "$tmp"
    local actual; actual=$(timeout 8 "$SCRIP" --interp "$tmp" < /dev/null 2>/dev/null)
    rm -f "$tmp"
    if [ "$actual" = "$expected" ]; then
        P2=$((P2+1))
    else
        echo "  FAIL-M2 $label (got: $(printf '%s' "$actual" | head -1))"; F2=$((F2+1))
    fi
}

run_m3() {
    local label="$1" src="$2" expected="$3"
    local tmp; tmp=$(mktemp /tmp/sno_XXXXXX.sno)
    printf '%s\n' "$src" > "$tmp"
    local actual; actual=$(timeout 8 "$SCRIP" --run "$tmp" < /dev/null 2>/dev/null)
    rm -f "$tmp"
    if [ "$actual" = "$expected" ]; then
        P3=$((P3+1))
    else
        echo "  FAIL-M3 $label (got: $(printf '%s' "$actual" | head -1))"; F3=$((F3+1))
    fi
}

run_m4() {
    local label="$1" src="$2" expected="$3"
    local tmp; tmp=$(mktemp /tmp/sno_XXXXXX.sno)
    printf '%s\n' "$src" > "$tmp"
    local s o bin
    s=$(mktemp /tmp/sno_XXXXXX.s); o="${s%.s}.o"; bin="${s%.s}.bin"
    local actual
    if timeout 8 "$SCRIP" --compile --target=x86 "$tmp" < /dev/null > "$s" 2>/dev/null \
       && as "$s" -o "$o" 2>/dev/null \
       && gcc -no-pie "$o" -L"$OUTDIR" -lscrip_rt -Wl,-rpath,"$OUTDIR" \
              -Wl,--allow-shlib-undefined -lm -o "$bin" 2>/dev/null; then
        actual=$(timeout 8 "$bin" < /dev/null 2>/dev/null)
    else
        actual="<mode4-build-failed>"
    fi
    rm -f "$tmp" "$s" "$o" "$bin"
    if [ "$actual" = "$expected" ]; then
        echo "  PASS $label"; P4=$((P4+1))
    else
        echo "  FAIL $label (got: $(printf '%s' "$actual" | head -1))"; F4=$((F4+1))
    fi
}

run_all() {
    local label="$1" src="$2" expected="$3"
    run_m2 "$label" "$src" "$expected"
    run_m3 "$label" "$src" "$expected"
    run_m4 "$label" "$src" "$expected"
}

T0=$SECONDS
echo "=== SNOBOL4 smoke (modes 2+3+4) — mode-4 HARD GATE ==="

run_all "output"  "        OUTPUT = 'hello'
END" "hello"

run_all "concat"  "        OUTPUT = 'ab' 'cd'
END" "abcd"

run_all "arith"   "        OUTPUT = 2 + 3
END" "5"

run_all "pattern" "        S = 'abc'
        S 'b' = 'X'
        OUTPUT = S
END" "aXc"

run_all "goto_s"  "        'x' 'x'  :S(HIT)
        OUTPUT = 'miss'
        :(END)
HIT     OUTPUT = 'hit'
END" "hit"

run_all "define"  "        DEFINE('DOUBLE(X)')
        OUTPUT = DOUBLE(21)
        :(END)
DOUBLE  DOUBLE = X + X
        RETURN
END" "42"

run_all "arith_sm" "        OUTPUT = 2 + 3
END" "5"

TT=$((SECONDS-T0))
echo ""
echo "mode-2 (--interp):  PASS=$P2 FAIL=$F2"
echo "mode-3 (--run):     PASS=$P3 FAIL=$F3"
echo "mode-4 (--compile): PASS=$P4 FAIL=$F4   (HARD GATE)"
echo "PASS=$P4 FAIL=$F4"
printf "TIME TOTAL=%ds\n" "$TT"

[ "$F4" -eq 0 ]
