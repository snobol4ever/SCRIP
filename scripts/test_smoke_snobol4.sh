#!/usr/bin/env bash
# test_smoke_snobol4.sh — per-frontend smoke for SNOBOL4  (FI-9)
#
# TESTING DIRECTIVE (GOAL-SNOBOL4-BB, Lon 2026-05-31): ALWAYS run ALL THREE modes —
#   mode 2 (--interp), mode 3 (--run / SB-LINEAR), and mode 4 (--compile --target=x86
#   → as → gcc -no-pie -lscrip_rt → run the binary). Mode 2 is the HARD gate (exit 0
#   requires mode-2 all-pass). Modes 3 and 4 are RUN + REPORTED on EVERY invocation
#   (tracked with MODE3_MIN / MODE4_MIN floors, default 0) so the full native picture
#   is always visible — never gate on mode 2 alone without also exercising 3 and 4.
#
# Gate: exits 0 on a clean build when mode-2 is all-pass and modes 3/4 meet their floors.
# AUTHORS: Lon Jones Cherryholmes · Claude Sonnet 4.6 · Claude Opus 4.8  DATE: 2026-04-14 (all-modes 2026-05-31)
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${HERE}/../scrip"
OUTDIR="${HERE}/../out"                 # libscrip_rt.so lives here (mode-4 link)
MODE3_MIN="${MODE3_MIN:-5}"             # tracked floor: mode-3 (--run) PASS must be >= this
MODE4_MIN="${MODE4_MIN:-0}"             # tracked floor: mode-4 (--compile) PASS must be >= this
TAG=2                                   # which per-mode counter run_file updates (2|3|4)
P2=0; F2=0; P3=0; F3=0; P4=0; F4=0

# run_file LABEL SRC EXPECTED MODE
#   MODE is one of: --interp (mode 2) | --run (mode 3) | mode4 (mode 4 native pipeline).
run_file() {
    local label="$1" src="$2" expected="$3" mode="${4:---interp}"
    local tmp; tmp=$(mktemp /tmp/sno_XXXXXX.sno)
    printf '%s\n' "$src" > "$tmp"
    local actual
    if [ "$mode" = "mode4" ]; then
        # FULL mode-4 pipeline: emit x86 → assemble → link against libscrip_rt → run.
        local s o bin
        s=$(mktemp /tmp/sno_XXXXXX.s); o="${s%.s}.o"; bin="${s%.s}.bin"
        if timeout 8 "$SCRIP" --compile --target=x86 "$tmp" < /dev/null > "$s" 2>/dev/null \
           && as "$s" -o "$o" 2>/dev/null \
           && gcc -no-pie "$o" -L"$OUTDIR" -lscrip_rt -Wl,-rpath,"$OUTDIR" -Wl,--allow-shlib-undefined -lm -o "$bin" 2>/dev/null; then
            actual=$(timeout 8 "$bin" < /dev/null 2>/dev/null)
        else
            actual="<mode4-build-failed>"
        fi
        rm -f "$s" "$o" "$bin"
    else
        actual=$(timeout 8 "$SCRIP" $mode "$tmp" 2>/dev/null)
    fi
    rm -f "$tmp"
    if [ "$actual" = "$expected" ]; then
        echo "  PASS $label"; eval "P$TAG=\$((P$TAG+1))"
    else
        echo "  FAIL $label (got: $(echo "$actual"|head -1))"; eval "F$TAG=\$((F$TAG+1))"
    fi
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

echo "=== SNOBOL4 smoke (Mode 2 --interp) — HARD GATE ==="
TAG=2; smoke_six "--interp"
run_file "arith_sm" "        OUTPUT = 2 + 3
END" "5" "--interp"

echo ""
echo "=== SNOBOL4 smoke (Mode 3 --run / SB-LINEAR) — tracked (floor MODE3_MIN=$MODE3_MIN) ==="
TAG=3; smoke_six "--run"

echo ""
echo "=== SNOBOL4 smoke (Mode 4 --compile --target=x86 -> as -> gcc -> run) — tracked (floor MODE4_MIN=$MODE4_MIN) ==="
TAG=4; smoke_six "mode4"

echo ""
echo "mode-2 (--interp):  PASS=$P2 FAIL=$F2   (HARD GATE)"
echo "mode-3 (--run):     PASS=$P3 FAIL=$F3   (tracked; floor MODE3_MIN=$MODE3_MIN)"
echo "mode-4 (--compile): PASS=$P4 FAIL=$F4   (tracked; floor MODE4_MIN=$MODE4_MIN)"
echo "PASS=$((P2+P3+P4)) FAIL=$((F2+F3+F4))"

# Gate: mode-2 must be all-pass; modes 3 & 4 must meet their tracked floors.
[ "$F2" -eq 0 ] && [ "$P3" -ge "$MODE3_MIN" ] && [ "$P4" -ge "$MODE4_MIN" ]
