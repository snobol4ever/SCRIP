#!/usr/bin/env bash
# test_smoke_snobol4.sh — per-frontend smoke for SNOBOL4  (FI-9)
# Gate: exits 0 in < 2s on a clean build.
# AUTHORS: Lon Jones Cherryholmes · Claude Sonnet 4.6  DATE: 2026-04-14
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${HERE}/../scrip"
PASS=0; FAIL=0

run_file() {
    local label="$1" src="$2" expected="$3" mode="${4:---interp}"
    local tmp; tmp=$(mktemp /tmp/sno_XXXXXX.sno)
    printf '%s\n' "$src" > "$tmp"
    local actual; actual=$(timeout 8 "$SCRIP" $mode "$tmp" 2>/dev/null)
    rm -f "$tmp"
    if [ "$actual" = "$expected" ]; then echo "  PASS $label"; PASS=$((PASS+1))
    else echo "  FAIL $label (got: $(echo "$actual"|head -1))"; FAIL=$((FAIL+1)); fi
}

smoke_six() {
    local mode="$1"
    run_file "output" "        OUTPUT = 'hello'
END" "hello" "$mode"

    run_file "concat" "        OUTPUT = 'ab' 'cd'
END" "abcd" "$mode"

    run_file "arith" "        OUTPUT = 2 + 3
END" "5" "$mode"

    run_file "pattern" "        S = 'abc'
        S 'b' = 'X'
        OUTPUT = S
END" "aXc" "$mode"

    run_file "goto_s" "        'x' 'x'  :S(HIT)
        OUTPUT = 'miss'
        :(END)
HIT     OUTPUT = 'hit'
END" "hit" "$mode"

    run_file "define" "        DEFINE('DOUBLE(X)')
        OUTPUT = DOUBLE(21)
        :(END)
DOUBLE  DOUBLE = X + X
        RETURN
END" "42" "$mode"
}

echo "=== SNOBOL4 smoke (Mode 2 --interp) ==="
smoke_six "--interp"

run_file "arith_sm" "        OUTPUT = 2 + 3
END" "5" "--interp"

echo ""
echo "=== SNOBOL4 smoke (Mode 3 --run / SB-LINEAR) ==="
smoke_six "--run"

echo ""; echo "PASS=$PASS FAIL=$FAIL"; [ "$FAIL" -eq 0 ]
