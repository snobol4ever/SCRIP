#!/usr/bin/env bash
# test_smoke_icon.sh — per-frontend smoke for Icon, run in ALL THREE execution modes.
#   mode 2 = --interp  (BB port-walker oracle)  — HARD GATE: must be all-PASS (build sanity).
#   mode 3 = --run     (stackless native x86)    — TRACKED: climbs to all-PASS as GZ rungs
#                                                   rebuild each box family stackless (GROUND ZERO 3).
#   mode 4 = --compile (standalone x86-64 asm)    — TRACKED: emit asm -> assemble -> link
#                                                   libscrip_rt.so -> run -> compare. Climbs to
#                                                   all-PASS as BB-native compile emission is rebuilt
#                                                   (currently severed by SMX -> 0/N, floor MODE4_MIN).
# ⛔ POLICY (GOAL-ICON-BB.md "ALWAYS TEST ALL THREE MODES"): every Icon execution test runs --interp,
#    --run AND --compile on the SAME program and reports all three. Never test fewer than all three.
# Exit 0 iff mode-2 is all-PASS AND mode-3 PASS >= $MODE3_MIN AND mode-4 PASS >= $MODE4_MIN (floors).
# AUTHORS: Lon Jones Cherryholmes · Jeffrey Cooper M.D. · Claude  DATE: 2026-05-30 (mode-4 added 2026-05-31)
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${HERE}/../scrip"
RT_SO="${HERE}/../out/libscrip_rt.so"
MODE3_MIN="${MODE3_MIN:-1}"
MODE4_MIN="${MODE4_MIN:-0}"
P2=0; F2=0; P3=0; F3=0; P4=0; F4=0; N=0

icon() {
    local label="$1" expected="$2"
    local tmp; tmp=$(mktemp /tmp/icn_XXXXXX.icn)
    cat > "$tmp"
    N=$((N+1))
    local a2 a3 a4 r2 r3 r4
    a2=$(timeout 8 "$SCRIP" --interp "$tmp" 2>/dev/null </dev/null)
    a3=$(timeout 8 "$SCRIP" --run    "$tmp" 2>/dev/null </dev/null)
    # mode 4: --compile emits standalone x86-64 asm to stdout; assemble + link libscrip_rt.so + run.
    a4=""
    local s4 bin4
    s4=$(mktemp /tmp/icn_XXXXXX.s); bin4=$(mktemp /tmp/icn_XXXXXX.bin); rm -f "$bin4"
    if timeout 8 "$SCRIP" --compile "$tmp" >"$s4" 2>/dev/null </dev/null && [ -s "$s4" ] && [ -f "$RT_SO" ]; then
        if gcc -no-pie "$s4" -L"${HERE}/../out" -lscrip_rt -Wl,-rpath,"${HERE}/../out" -o "$bin4" 2>/dev/null; then
            a4=$(timeout 8 "$bin4" 2>/dev/null </dev/null)
        fi
    fi
    rm -f "$tmp" "$s4" "$bin4"
    if [ "$a2" = "$expected" ]; then r2="m2 PASS"; P2=$((P2+1)); else r2="m2 FAIL"; F2=$((F2+1)); fi
    if [ "$a3" = "$expected" ]; then r3="m3 PASS"; P3=$((P3+1)); else r3="m3 FAIL"; F3=$((F3+1)); fi
    if [ "$a4" = "$expected" ]; then r4="m4 PASS"; P4=$((P4+1)); else r4="m4 FAIL"; F4=$((F4+1)); fi
    printf "  [%s] [%s] [%s] %s\n" "$r2" "$r3" "$r4" "$label"
}

echo "=== Icon smoke (mode 2 = --interp, mode 3 = --run, mode 4 = --compile) ==="

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

icon "while" "$(printf '1\n2\n3')" << 'EOF'
procedure main()
  i := 1;
  while i <= 3 do {
    write(i);
    i := i + 1;
  };
end
EOF

icon "until" "$(printf '5\n4\n3\n2\n1')" << 'EOF'
procedure main()
  i := 5;
  until i <= 0 do {
    write(i);
    i := i - 1;
  };
end
EOF

icon "repeat_break" "$(printf '0\n1\n2')" << 'EOF'
procedure main()
  i := 0;
  repeat {
    if i >= 3 then break else write(i);
    i := i + 1;
  };
end
EOF

icon "proc_zeroarg" "42" << 'EOF'
procedure answer()
  return 42;
end
procedure main()
  write(answer());
end
EOF

icon "proc_recursion" "120" << 'EOF'
procedure fact(n)
  if n <= 1 then return 1; else return n * fact(n - 1);
end
procedure main()
  write(fact(5));
end
EOF

icon "bare_if" "done" << 'EOF'
procedure main()
  x := 2;
  if x > 5 then write("big");
  write("done");
end
EOF

echo ""
echo "mode-2 (--interp):   PASS=$P2 FAIL=$F2  / $N   (HARD GATE)"
echo "mode-3 (--run):      PASS=$P3 FAIL=$F3  / $N   (tracked; floor MODE3_MIN=$MODE3_MIN)"
echo "mode-4 (--compile):  PASS=$P4 FAIL=$F4  / $N   (tracked; floor MODE4_MIN=$MODE4_MIN)"
[ "$F2" -eq 0 ] && [ "$P3" -ge "$MODE3_MIN" ] && [ "$P4" -ge "$MODE4_MIN" ]
