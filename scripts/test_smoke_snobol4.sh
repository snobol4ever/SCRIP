#!/usr/bin/env bash
# test_smoke_snobol4.sh — SNOBOL4 smoke, MODE-4 ONLY (Lon directive 2026-06-06)
#
# MODE-4 IS THE ONLY GATE. Modes 2 and 3 are NOT run for SNOBOL4.
# Full native pipeline: --compile --target=x86 → as → gcc -no-pie → run binary.
# Gate: exits 0 when all mode-4 cases pass.
# AUTHORS: Lon Jones Cherryholmes · Jeffrey Cooper M.D. · Claude Sonnet 4.6  DATE: 2026-06-06
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${HERE}/../scrip"
OUTDIR="${HERE}/../out"
P4=0; F4=0

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

T0=$SECONDS
echo "=== SNOBOL4 smoke (Mode 4 --compile --target=x86 → as → gcc → run) — HARD GATE ==="

run_m4 "output"  "        OUTPUT = 'hello'
END" "hello"

run_m4 "concat"  "        OUTPUT = 'ab' 'cd'
END" "abcd"

run_m4 "arith"   "        OUTPUT = 2 + 3
END" "5"

run_m4 "pattern" "        S = 'abc'
        S 'b' = 'X'
        OUTPUT = S
END" "aXc"

run_m4 "goto_s"  "        'x' 'x'  :S(HIT)
        OUTPUT = 'miss'
        :(END)
HIT     OUTPUT = 'hit'
END" "hit"

run_m4 "define"  "        DEFINE('DOUBLE(X)')
        OUTPUT = DOUBLE(21)
        :(END)
DOUBLE  DOUBLE = X + X
        RETURN
END" "42"

run_m4 "arith_sm" "        OUTPUT = 2 + 3
END" "5"

T_M4=$((SECONDS-T0))
echo ""
echo "mode-4 (--compile): PASS=$P4 FAIL=$F4   (HARD GATE)"
echo "PASS=$P4 FAIL=$F4"
printf "TIME M4=%ds TOTAL=%ds\n" "$T_M4" "$T_M4"

[ "$F4" -eq 0 ]
