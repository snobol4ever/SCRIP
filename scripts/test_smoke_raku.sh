#!/usr/bin/env bash
# test_smoke_raku.sh — per-frontend smoke for Raku, run in ALL THREE execution modes.
#   mode 2 = --interp  (BB port-walker oracle over the lowered IR) — HARD GATE: must be all-PASS.
#   mode 3 = --run     (stackless native x86)   — TRACKED: climbs to all-PASS as RK-EMIT lands.
#   mode 4 = --compile (standalone x86-64 asm)   — TRACKED: emit asm -> as -> link libscrip_rt -> run.
# ⛔ POLICY (GOAL-RAKU-BB.md "TESTING DIRECTIVE — ALWAYS RUN ALL THREE MODES", Lon 2026-05-31): every Raku
#    execution test runs --interp, --run AND --compile on the SAME program and reports all three. Never report
#    a mode-2 number alone. Modes 3/4 are by-design SMX abort until RK-EMIT (the bb_rk_*.cpp templates) is built,
#    so they read 0/N today (floors MODE3_MIN/MODE4_MIN default 0); raise the floors as 3/4 come back.
# Exit 0 iff mode-2 is all-PASS AND mode-3 PASS >= $MODE3_MIN AND mode-4 PASS >= $MODE4_MIN.
# AUTHORS: Lon Jones Cherryholmes · Jeffrey Cooper M.D. · Claude Sonnet · Claude Opus  DATE: 2026-05-31 (3-mode)
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${HERE}/../scrip"
RT_SO="${HERE}/../out/libscrip_rt.so"
MODE3_MIN="${MODE3_MIN:-0}"
MODE4_MIN="${MODE4_MIN:-0}"
P2=0; F2=0; P3=0; F3=0; P4=0; F4=0; N=0

raku() {
    local label="$1" expected="$2"
    local tmp; tmp=$(mktemp /tmp/rk_XXXXXX.raku)
    cat > "$tmp"
    N=$((N+1))
    local a2 a3 a4 r2 r3 r4
    a2=$(timeout 8 "$SCRIP" --interp "$tmp" 2>/dev/null </dev/null)
    a3=$(timeout 8 "$SCRIP" --run    "$tmp" 2>/dev/null </dev/null)
    a4=""
    local s4 bin4
    s4=$(mktemp /tmp/rk_XXXXXX.s); bin4=$(mktemp /tmp/rk_XXXXXX.bin); rm -f "$bin4"
    if timeout 8 "$SCRIP" --compile --target=x86 "$tmp" >"$s4" 2>/dev/null </dev/null && [ -s "$s4" ] && [ -f "$RT_SO" ]; then
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

echo "=== Raku smoke ==="

raku "say_str" "hello world" << 'EOF'
sub main() {
    say('hello world');
}
EOF

raku "arith" "42" << 'EOF'
sub main() {
    my $x = 6 * 7;
    say($x);
}
EOF

raku "var" "99" << 'EOF'
sub main() {
    my $x = 99;
    say($x);
}
EOF

raku "while_loop" "$(printf '1\n2\n3')" << 'EOF'
sub main() {
    my $i = 1;
    while ($i <= 3) {
        say($i);
        $i = $i + 1;
    }
}
EOF

raku "string_concat" "abcd" << 'EOF'
sub main() {
    my $s = 'ab' ~ 'cd';
    say($s);
}
EOF

raku "gather_take" "$(printf '10\n20\n30\ndone')" << 'EOF'
sub main() {
    for gather { take(10); take(20); take(30); } -> $v {
        say($v);
    }
    say('done');
}
EOF

raku "map_range" "$(printf '2\n4\n6\ndone')" << 'EOF'
sub main() {
    for map { $_ * 2 } 1..3 -> $v {
        say($v);
    }
    say('done');
}
EOF

raku "grep_range" "$(printf '3\n4\n5\ndone')" << 'EOF'
sub main() {
    for grep { $_ > 2 } 1..5 -> $v {
        say($v);
    }
    say('done');
}
EOF

raku "map_over_gather" "$(printf '2\n4\n6\ndone')" << 'EOF'
sub main() {
    for map { $_ * 2 } gather { take(1); take(2); take(3); } -> $v {
        say($v);
    }
    say('done');
}
EOF

raku "grep_over_gather" "$(printf '2\n4\ndone')" << 'EOF'
sub main() {
    for grep { $_ % 2 == 0 } gather { take(1); take(2); take(3); take(4); } -> $v {
        say($v);
    }
    say('done');
}
EOF

raku "jct_any" "any-hit" << 'EOF'
sub main() {
    my $x = 3;
    if ($x == any(1, 2, 3)) { say('any-hit'); }
    if ($x == any(7, 8, 9)) { say('any-miss-BAD'); }
}
EOF

raku "jct_all" "all-hit" << 'EOF'
sub main() {
    my $x = 3;
    if ($x == all(3, 3, 3)) { say('all-hit'); }
    if ($x == all(3, 3, 4)) { say('all-miss-BAD'); }
}
EOF

raku "jct_one" "one-hit" << 'EOF'
sub main() {
    my $x = 3;
    if ($x == one(1, 3, 5)) { say('one-hit'); }
    if ($x == one(3, 3, 5)) { say('one-miss-BAD'); }
}
EOF

raku "jct_none" "none-hit" << 'EOF'
sub main() {
    my $x = 3;
    if ($x == none(7, 8, 9)) { say('none-hit'); }
    if ($x == none(1, 3, 5)) { say('none-miss-BAD'); }
}
EOF

raku "jct_infix" "$(printf 'pipe\namp')" << 'EOF'
sub main() {
    my $x = 3;
    if ($x == (1 | 2 | 3)) { say('pipe'); }
    if ($x == (3 & 3 & 3)) { say('amp'); }
}
EOF

raku "jct_str" "str-hit" << 'EOF'
sub main() {
    my $s = 'b';
    if ($s eq any('a', 'b')) { say('str-hit'); }
}
EOF

raku "jct_nested" "$(printf 'all-wraps-any\nany-first-in-all')" << 'EOF'
sub main() {
    my $x = 50;
    if ($x == (50 & (50 | 60)))  { say('all-wraps-any'); }
    if ($x == (10 | (50 & 60)))  { say('nest-miss-BAD'); }
    if ($x == ((50 | 60) & 50))  { say('any-first-in-all'); }
}
EOF

raku "list_construct_read" "$(printf '1\n3')" << 'EOF'
sub main() {
    my @a = (3, 1, 2);
    say(@a[1]);
    my @b = (10, 3, 20);
    say(@b[1]);
}
EOF

raku "array_sort" "$(printf '1\n3\n5\n9')" << 'EOF'
sub main() {
    my @a = (5, 3, 9, 1);
    my @s = sort(@a);
    say(@s[0]);
    say(@s[1]);
    say(@s[2]);
    say(@s[3]);
}
EOF

raku "array_elems" "$(printf '3\n0')" << 'EOF'
sub main() {
    my @a = (10, 20, 30);
    say(elems(@a));
    my @e = '';
    say(elems(@e));
}
EOF

raku "array_reverse" "$(printf '3\n2\n1')" << 'EOF'
sub main() {
    my @a = (1, 2, 3);
    my @r = reverse(@a);
    say(@r[0]);
    say(@r[1]);
    say(@r[2]);
}
EOF

raku "str_reverse" "cba" << 'EOF'
sub main() {
    say(reverse('abc'));
}
EOF

echo ""
echo "mode-2 (--interp):   PASS=$P2 FAIL=$F2  / $N   (HARD GATE)"
echo "mode-3 (--run):      PASS=$P3 FAIL=$F3  / $N   (tracked; floor MODE3_MIN=$MODE3_MIN)"
echo "mode-4 (--compile):  PASS=$P4 FAIL=$F4  / $N   (tracked; floor MODE4_MIN=$MODE4_MIN)"
[ "$F2" -eq 0 ] && [ "$P3" -ge "$MODE3_MIN" ] && [ "$P4" -ge "$MODE4_MIN" ]
