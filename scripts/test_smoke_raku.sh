#!/usr/bin/env bash
# test_smoke_raku.sh — per-frontend smoke for Raku, run in BOTH native execution modes.
#   mode 3 = --run     (in-process stackless native x86 BB blobs) — the primary mode.
#   mode 4 = --compile (standalone x86-64 asm -> as -> link libscrip_rt -> run).
# NOTE (2026-06-15): the IR-graph interpreter (mode 2 / --interp) was DELETED — nothing walks an IR graph to
#    interpret it in any mode. The harness no longer invokes --interp; m3 and m4 are the only modes. A program
#    either runs natively (PASS) or is cleanly declined with a loud [SMX] banner (EXCISED) — there is no oracle
#    fallback. DONE BAR: m3 AND m4 each PASS-or-EXCISED with ZERO silent FAIL (no abort, no miscompile).
# Exit 0 iff mode-3 has zero FAIL AND mode-4 has zero FAIL AND m3 PASS >= $MODE3_MIN AND m4 PASS >= $MODE4_MIN.
# AUTHORS: Lon Jones Cherryholmes · Jeffrey Cooper M.D. · Claude Sonnet · Claude Opus  DATE: 2026-05-31 (3-mode; de-interp'd to 2-mode 2026-06-15)
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${HERE}/../scrip"
RT_SO="${HERE}/../out/libscrip_rt.so"
MODE3_MIN="${MODE3_MIN:-0}"
MODE4_MIN="${MODE4_MIN:-0}"
P3=0; F3=0; X3=0; P4=0; F4=0; X4=0; N=0

raku() {
    local label="$1" expected="$2"
    local tmp; tmp=$(mktemp /tmp/rk_XXXXXX.raku)
    cat > "$tmp"
    N=$((N+1))
    local a3 a4 r3 r4
    local e3 e4 s4 bin4
    e3=$(mktemp /tmp/rk_XXXXXX.e3); e4=$(mktemp /tmp/rk_XXXXXX.e4)
    a3=$(timeout 8 "$SCRIP" --run    "$tmp" 2>"$e3" </dev/null)
    a4=""
    s4=$(mktemp /tmp/rk_XXXXXX.s); bin4=$(mktemp /tmp/rk_XXXXXX.bin); rm -f "$bin4"
    if timeout 8 "$SCRIP" --compile --target=x86 "$tmp" >"$s4" 2>"$e4" </dev/null && [ -s "$s4" ] && [ -f "$RT_SO" ]; then
        if gcc -no-pie "$s4" -L"${HERE}/../out" -lscrip_rt -Wl,-rpath,"${HERE}/../out" -o "$bin4" 2>/dev/null; then
            a4=$(timeout 8 "$bin4" 2>/dev/null </dev/null)
        fi
    fi
    # [SMX] on stderr => the native mode DELIBERATELY DECLINES this rung (its bb_*.cpp template is not built
    # yet) => counted EXCISED, NOT FAIL. The done bar (interp now deleted): m3 AND m4 each PASS-or-EXCISED —
    # never a silent FAIL / abort / miscompile.
    local smx3=0 smx4=0
    grep -qE '\[SMX\]' "$e3" && smx3=1
    grep -qE '\[SMX\]' "$e4" && smx4=1
    rm -f "$tmp" "$s4" "$bin4" "$e3" "$e4"
    if   [ "$a3" = "$expected" ]; then r3="m3 PASS"; P3=$((P3+1));
    elif [ "$smx3" -eq 1 ];      then r3="m3 EXCS"; X3=$((X3+1));
    else                              r3="m3 FAIL"; F3=$((F3+1)); fi
    if   [ "$a4" = "$expected" ]; then r4="m4 PASS"; P4=$((P4+1));
    elif [ "$smx4" -eq 1 ];      then r4="m4 EXCS"; X4=$((X4+1));
    else                              r4="m4 FAIL"; F4=$((F4+1)); fi
    printf "  [%s] [%s] %s\n" "$r3" "$r4" "$label"
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

# --- RK-LOWER-5b: in-place hash/array MUTATION (push/pop/arr_set/hash_set/hash_delete) ---

raku "array_push_pop" "$(printf '3\n1\n3\n99\n7\n3')" << 'EOF'
sub main() {
    my @nums = '';
    push(@nums, 1);
    push(@nums, 2);
    push(@nums, 3);
    say(elems(@nums));
    say(arr_get(@nums, 0));
    say(arr_get(@nums, 2));
    @nums[1] = 99;
    say(arr_get(@nums, 1));
    push(@nums, 7);
    my $p = pop(@nums);
    say($p);
    say(elems(@nums));
}
EOF

raku "hash_set_get" "$(printf 'Alice\n31\n1\n0')" << 'EOF'
sub main() {
    my $h = 0;
    hash_set($h, 'name', 'Alice');
    hash_set($h, 'age', '30');
    say(hash_get($h, 'name'));
    hash_set($h, 'age', '31');
    say(hash_get($h, 'age'));
    say(hash_exists($h, 'name'));
    say(hash_exists($h, 'missing'));
}
EOF

raku "hash_sigil_delete" "$(printf 'Raku\n6\n0')" << 'EOF'
sub main() {
    my %h = 0;
    %h<lang> = 'Raku';
    %h<vers> = '6';
    say(%h<lang>);
    say(%h<vers>);
    delete %h<lang>;
    say(hash_exists(%h, 'lang'));
}
EOF

raku "say_jct" "$(printf 'any(1, 2, 3)\nall(a, b)\nnone(10, 20)\none(x, y)')" << 'EOF'
sub main() {
    say(any(1, 2, 3));
    say(all('a', 'b'));
    say(none(10, 20));
    say(one('x', 'y'));
}
EOF

raku "say_list" "1 2 3" << 'EOF'
sub main() {
    my @a = (1, 2, 3);
    say(@a);
}
EOF

raku "bool_truthiness" "$(printf 't\nf\nt\nf')" << 'EOF'
sub main() {
    if (True)  { say('t'); } else { say('f'); }
    if (False) { say('t'); } else { say('f'); }
    my $x = 1;
    if ($x) { say('t'); } else { say('f'); }
    my $y = 0;
    if ($y) { say('t'); } else { say('f'); }
}
EOF

# --- RK-LOWER-5h: top-level statements without sub main() ---

raku "toplevel_no_main" "$(printf 'hello\n42')" << 'EOF'
say("hello");
say(42);
EOF

# --- RK-LOWER-5g: relop-in-assignment stores Bool value (0/1) ---

raku "bool_compare_store" "$(printf '0\n1\n0\n1\n0\n1')" << 'EOF'
sub main() {
    my $a = (1 > 2);
    say($a);
    my $b = (3 > 1);
    say($b);
    my $c = (5 == 6);
    say($c);
    my $d = (7 != 8);
    say($d);
    my $e = (2 <= 1);
    say($e);
    my $f = (10 >= 9);
    say($f);
}
EOF

# --- RK-LOWER-5d: class / method / new / field access ---

raku "class_method" "$(printf '3\n4\n7\n14\nRex\nWoof from Rex')" << 'EOF'
class Point {
    has $.x;
    has $.y;
    method sum() { return $!x + $!y; }
    method scale($factor) { return ($!x + $!y) * $factor; }
}
class Dog {
    has $.name;
    has $.age;
    method greet() { return "Woof from " ~ $!name; }
}
sub main() {
    my $p = Point.new(x => 3, y => 4);
    say($p.x);
    say($p.y);
    say($p.sum());
    say($p.scale(2));
    my $d = Dog.new(name => "Rex", age => 5);
    say($d.name);
    say($d.greet());
}
EOF

# --- string methods (Str: uc/lc/tc/chars/flip/trim) — runtime dispatch, both native modes ---
raku "str_uc" "HELLO" << 'EOF'
sub main() { say("hello".uc); }
EOF

raku "str_lc" "hello" << 'EOF'
sub main() { say("HeLLo".lc); }
EOF

raku "str_tc" "Hello world" << 'EOF'
sub main() { say("hello world".tc); }
EOF

raku "str_chars" "5" << 'EOF'
sub main() { say("hello".chars); }
EOF

raku "str_flip" "cba" << 'EOF'
sub main() { say("abc".flip); }
EOF

raku "str_trim" "hi" << 'EOF'
sub main() { say("  hi  ".trim); }
EOF

raku "str_method_var" "ABC" << 'EOF'
sub main() { my $s = "abc"; say($s.uc); }
EOF

raku "str_method_paren" "WORLD" << 'EOF'
sub main() { say("world".uc()); }
EOF

raku "coerce_int" "42" << 'EOF'
sub main() { my $s = "42"; say($s.Int + 0); }
EOF

raku "coerce_str" "7" << 'EOF'
sub main() { my $n = 7; say($n.Str); }
EOF

raku "str_contains_t" "1" << 'EOF'
sub main() { say("hello".contains("ell")); }
EOF

raku "str_contains_f" "0" << 'EOF'
sub main() { say("hello".contains("xyz")); }
EOF

raku "str_index" "2" << 'EOF'
sub main() { say("hello".index("l")); }
EOF

raku "str_substr2" "ell" << 'EOF'
sub main() { say("hello".substr(1, 3)); }
EOF

raku "str_substr1" "llo" << 'EOF'
sub main() { say("hello".substr(2)); }
EOF

raku "num_abs_pos" "5" << 'EOF'
sub main() { say(5.abs); }
EOF

raku "num_abs_neg" "5" << 'EOF'
sub main() { my $n = 3 - 8; say($n.abs); }
EOF

raku "num_floor" "3" << 'EOF'
sub main() { my $x = 3.7; say($x.floor); }
EOF

raku "num_ceiling" "4" << 'EOF'
sub main() { my $x = 3.2; say($x.ceiling); }
EOF

raku "num_round" "4" << 'EOF'
sub main() { my $x = 3.5; say($x.round); }
EOF

raku "grammar_token" "12345" << 'EOF'
grammar G {
    token TOP { \d+ }
}
sub main() {
    say(G.parse("12345"));
}
EOF

raku "grammar_subrule" "hello" << 'EOF'
grammar G {
    token word { \w+ }
    rule TOP { <word> }
}
sub main() {
    say(G.parse("hello"));
}
EOF

raku "grammar_multi_subrule" "12abc" << 'EOF'
grammar G {
    token num  { \d+ }
    token word { \w+ }
    rule TOP { <num> <word> }
}
sub main() {
    say(G.parse("12abc"));
}
EOF

raku "grammar_nomatch" "nomatch" << 'EOF'
grammar G {
    token TOP { \d+ }
}
sub main() {
    my $r = G.parse("abc");
    if ($r) { say("matched"); } else { say("nomatch"); }
}
EOF

# --- ~~ smartmatch verdict: regex rides the C NFA matcher (re.c); m3/m4 cleanly EXCISE (regex is run-only here) ---
raku "smatch digits => match" "match" <<'EOF'
sub main() { if ('abc123' ~~ /\d+/) { say("match"); } else { say("nomatch"); } }
EOF
raku "smatch anchored => nomatch" "nomatch" <<'EOF'
sub main() { if ('abc' ~~ /^\d+$/) { say("match"); } else { say("nomatch"); } }
EOF
raku "smatch named capture" "hello" <<'EOF'
sub main() { my $s = "hello"; if ($s ~~ /<word>([a-z]+)/) { say($<word>); } }
EOF

echo ""
echo "mode-3 (--run):      PASS=$P3 FAIL=$F3 EXCISED=$X3  / $N   (done bar: PASS or EXCISED, never silent FAIL)"
echo "mode-4 (--compile):  PASS=$P4 FAIL=$F4 EXCISED=$X4  / $N   (done bar: PASS or EXCISED, never silent FAIL)"
# COMPLETION BAR (interp deleted 2026-06-15 — two native modes only): ZERO silent m3/m4 FAIL — every native
# mode is either PASS or a LOUD [SMX] EXCISE (no abort, no miscompile, no oracle fallback). A rung is promoted
# only when BOTH m3 and m4 are accounted for together. Floors retained as a backstop ratchet.
[ "$F3" -eq 0 ] && [ "$F4" -eq 0 ] && [ "$P3" -ge "$MODE3_MIN" ] && [ "$P4" -ge "$MODE4_MIN" ]
