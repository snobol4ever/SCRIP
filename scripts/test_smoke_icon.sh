#!/usr/bin/env bash
# test_smoke_icon.sh — per-frontend smoke for Icon, run in TWO modes.
#   mode 2 = --interp (BB oracle)         — HARD GATE: must be all-PASS (build sanity).
#   mode 3 = --run    (stackless native)  — TRACKED: climbs to all-PASS as GZ rungs rebuild
#                                            each box family stackless (GROUND ZERO 3).
# Exit 0 iff mode-2 is all-PASS AND mode-3 PASS count >= $MODE3_MIN (regression floor).
# AUTHORS: Lon Jones Cherryholmes · Jeffrey Cooper M.D. · Claude Sonnet  DATE: 2026-05-30
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${HERE}/../scrip"
MODE3_MIN="${MODE3_MIN:-1}"
P2=0; F2=0; P3=0; F3=0; N=0

icon() {
    local label="$1" expected="$2"
    local tmp; tmp=$(mktemp /tmp/icn_XXXXXX.icn)
    cat > "$tmp"
    N=$((N+1))
    local a2 a3 r2 r3
    a2=$(timeout 8 "$SCRIP" --interp "$tmp" 2>/dev/null </dev/null)
    a3=$(timeout 8 "$SCRIP" --run    "$tmp" 2>/dev/null </dev/null)
    rm -f "$tmp"
    if [ "$a2" = "$expected" ]; then r2="m2 PASS"; P2=$((P2+1)); else r2="m2 FAIL"; F2=$((F2+1)); fi
    if [ "$a3" = "$expected" ]; then r3="m3 PASS"; P3=$((P3+1)); else r3="m3 FAIL"; F3=$((F3+1)); fi
    printf "  [%s] [%s] %s\n" "$r2" "$r3" "$label"
}

echo "=== Icon smoke (mode 2 = --interp, mode 3 = --run) ==="

icon "write_str" "hello" << 'EOF'
procedure main()
  write("hello");
end
EOF

icon "write_int" "42" << 'EOF'
procedure main()
  write(42);
end
EOF

icon "arith" "5" << 'EOF'
procedure main()
  write(2 + 3);
end
EOF

icon "string_op" "abcd" << 'EOF'
procedure main()
  write("ab" || "cd");
end
EOF

icon "if_expr" "big" << 'EOF'
procedure main()
  x := 10;
  if x > 5 then write("big"); else write("small");
end
EOF

icon "every" "$(printf '1\n2\n3')" << 'EOF'
procedure main()
  every write(1 to 3);
end
EOF

echo ""
echo "mode-2 (--interp):  PASS=$P2 FAIL=$F2  / $N   (HARD GATE)"
echo "mode-3 (--run):     PASS=$P3 FAIL=$F3  / $N   (tracked; floor MODE3_MIN=$MODE3_MIN)"
[ "$F2" -eq 0 ] && [ "$P3" -ge "$MODE3_MIN" ]
