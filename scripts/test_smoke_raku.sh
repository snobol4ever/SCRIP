#!/usr/bin/env bash
# test_smoke_raku.sh — per-frontend smoke for Raku, run in BOTH native execution modes.
#   mode 3 = --run     (in-process stackless native x86 BB blobs) — the primary mode.
#   mode 4 = --compile (standalone x86-64 asm -> as -> link libscrip_rt -> run).
# NOTE (2026-06-15): the IR-graph interpreter (mode 2 / --run) was DELETED — nothing walks an IR graph to
#    interpret it in any mode. The harness no longer invokes --run; m3 and m4 are the only modes. A program
#    either runs natively (PASS) or is cleanly refused with a loud [SMX] banner (REFUSED) — there is no oracle
#    fallback. DONE BAR: m3 AND m4 each PASS-or-REFUSED with ZERO silent FAIL (no abort, no miscompile).
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
    local rc3=0 rc4=0
    e3=$(mktemp /tmp/rk_XXXXXX.e3); e4=$(mktemp /tmp/rk_XXXXXX.e4)
    a3=$(timeout 8 "$SCRIP" --run    "$tmp" 2>"$e3" </dev/null); rc3=$?
    a4=""
    s4=$(mktemp /tmp/rk_XXXXXX.s); bin4=$(mktemp /tmp/rk_XXXXXX.bin); rm -f "$bin4"
    if timeout 8 "$SCRIP" --compile --target=x86 "$tmp" >"$s4" 2>"$e4" </dev/null && [ -s "$s4" ] && [ -f "$RT_SO" ]; then
        if gcc -no-pie "$s4" -L"${HERE}/../out" -lscrip_rt -Wl,-rpath,"${HERE}/../out" -o "$bin4" 2>/dev/null; then
            a4=$(timeout 8 "$bin4" 2>/dev/null </dev/null); rc4=$?
        fi
    fi
    # [SMX] on stderr => the native mode DELIBERATELY REFUSES this rung (its bb_*.cpp template is not built
    # yet) => counted REFUSED, NOT FAIL. The done bar (interp now deleted): m3 AND m4 each PASS-or-REFUSED —
    # never a silent FAIL / abort / miscompile.
    # rc != 0 AND NOT smx => crash or bad exit => FAIL even if stdout matched (RK-ZC-7: harness sees rc).
    local smx3=0 smx4=0
    grep -qE '\[SMX\]' "$e3" && smx3=1
    grep -qE '\[SMX\]' "$e4" && smx4=1
    rm -f "$tmp" "$s4" "$bin4" "$e3" "$e4"
    if   [ "$smx3" -eq 1 ];                                           then r3="m3 EXCS"; X3=$((X3+1));
    elif [ "$a3" = "$expected" ] && [ "$rc3" -eq 0 ];                 then r3="m3 PASS"; P3=$((P3+1));
    else                                                                    r3="m3 FAIL"; F3=$((F3+1)); fi
    if   [ "$smx4" -eq 1 ];                                           then r4="m4 EXCS"; X4=$((X4+1));
    elif [ "$a4" = "$expected" ] && [ "$rc4" -eq 0 ];                 then r4="m4 PASS"; P4=$((P4+1));
    else                                                                    r4="m4 FAIL"; F4=$((F4+1)); fi
    printf "  [%s] [%s] %s\n" "$r3" "$r4" "$label"
}
# raku_dies — like raku() but expects a non-zero exit code (die/type-error/access-violation).
# stdout must match expected AND rc must be nonzero; rc=0 or stdout mismatch = FAIL.
raku_dies() {
    local label="$1" expected="$2"
    local tmp; tmp=$(mktemp /tmp/rk_XXXXXX.raku)
    cat > "$tmp"
    N=$((N+1))
    local a3 a4 r3 r4
    local e3 e4 s4 bin4
    local rc3=0 rc4=0
    e3=$(mktemp /tmp/rk_XXXXXX.e3); e4=$(mktemp /tmp/rk_XXXXXX.e4)
    a3=$(timeout 8 "$SCRIP" --run    "$tmp" 2>"$e3" </dev/null); rc3=$?
    a4=""
    local crc4=0
    s4=$(mktemp /tmp/rk_XXXXXX.s); bin4=$(mktemp /tmp/rk_XXXXXX.bin); rm -f "$bin4"
    timeout 8 "$SCRIP" --compile --target=x86 "$tmp" >"$s4" 2>"$e4" </dev/null; crc4=$?
    if [ "$crc4" -eq 0 ] && [ -s "$s4" ] && [ -f "$RT_SO" ]; then
        if gcc -no-pie "$s4" -L"${HERE}/../out" -lscrip_rt -Wl,-rpath,"${HERE}/../out" -o "$bin4" 2>/dev/null; then
            a4=$(timeout 8 "$bin4" 2>/dev/null </dev/null); rc4=$?
        fi
    elif [ "$crc4" -ne 0 ]; then
        rc4="$crc4"
    fi
    local smx3=0 smx4=0
    grep -qE '\[SMX\]' "$e3" && smx3=1
    grep -qE '\[SMX\]' "$e4" && smx4=1
    rm -f "$tmp" "$s4" "$bin4" "$e3" "$e4"
    if   [ "$smx3" -eq 1 ];                                           then r3="m3 EXCS"; X3=$((X3+1));
    elif [ "$a3" = "$expected" ] && [ "$rc3" -ne 0 ];                 then r3="m3 PASS"; P3=$((P3+1));
    else                                                                    r3="m3 FAIL"; F3=$((F3+1)); fi
    if   [ "$smx4" -eq 1 ];                                           then r4="m4 EXCS"; X4=$((X4+1));
    elif [ "$a4" = "$expected" ] && [ "$rc4" -ne 0 ];                 then r4="m4 PASS"; P4=$((P4+1));
    else                                                                    r4="m4 FAIL"; F4=$((F4+1)); fi
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

raku "bool_int_t" "1" << 'EOF'
sub main() { say(5.Bool); }
EOF

raku "bool_int_f" "0" << 'EOF'
sub main() { say(0.Bool); }
EOF

raku "bool_str_empty" "0" << 'EOF'
sub main() { my $s = ""; say($s.Bool); }
EOF

raku "num_succ" "6" << 'EOF'
sub main() { say(5.succ); }
EOF

raku "num_pred" "4" << 'EOF'
sub main() { say(5.pred); }
EOF

raku "str_words" "a b c" << 'EOF'
sub main() { say("a b c".words); }
EOF

raku "str_words_collapse" "hi there" << 'EOF'
sub main() { say("  hi   there  ".words); }
EOF

raku "str_words_elems" "3" << 'EOF'
sub main() { my @a = "a b c".words; say(elems(@a)); }
EOF

raku "str_split" "a b c" << 'EOF'
sub main() { say("a,b,c".split(",")); }
EOF

raku "str_split_elems" "3" << 'EOF'
sub main() { my @a = "a,b,c".split(","); say(elems(@a)); }
EOF

raku "str_comb_elems" "5" << 'EOF'
sub main() { my @a = "hello".comb; say(elems(@a)); }
EOF

raku "str_wordcase" "Hello World" << 'EOF'
sub main() { say("hello world".wordcase); }
EOF

raku "str_chomp" "hi" << 'EOF'
sub main() { say("hi\n".chomp); }
EOF

raku "str_lines_elems" "3" << 'EOF'
sub main() { my @a = "a\nb\nc".lines; say(elems(@a)); }
EOF

raku "list_join_sep" "1-2-3" << 'EOF'
sub main() { my @a = (1, 2, 3); say(@a.join("-")); }
EOF

raku "list_join_default" "123" << 'EOF'
sub main() { my @a = (1, 2, 3); say(@a.join); }
EOF

raku "list_elems_method" "3" << 'EOF'
sub main() { my @a = (10, 20, 30); say(@a.elems); }
EOF

raku "split_then_join" "x|y|z" << 'EOF'
sub main() { my @a = "x,y,z".split(","); say(@a.join("|")); }
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

# --- RK-GRAM: literal strings in rules/tokens. A quoted literal ("hello", '[') now matches its CONTENTS
#     verbatim (quotes stripped) with regex-metacharacters treated as LITERAL (faithful to Raku: a string
#     literal in a regex matches the exact characters, "." is a literal dot not any-char). Fixes gram_expand,
#     which previously copied the quote chars into the NFA pattern so "hello" tried to match the 7 chars
#     including quotes. Shared by both modes (runtime gram_expand). Literal+subrule mixes work. ---
raku "grammar_literal" "hello" << 'EOF'
grammar G { rule TOP { "hello" } }
sub main() { say(G.parse("hello")); }
EOF
raku "grammar_literal_nomatch" "N" << 'EOF'
grammar G { rule TOP { "hello" } }
sub main() { my $r = G.parse("world"); if ($r) { say("Y"); } else { say("N"); } }
EOF
raku "grammar_literal_subrule_mix" "[abc]" << 'EOF'
grammar G { token w { \w+ } rule TOP { "[" <w> "]" } }
sub main() { say(G.parse("[abc]")); }
EOF
raku "grammar_literal_metachar" "OK" << 'EOF'
grammar G { rule TOP { "a.b" } }
sub main() { my $r = G.parse("axb"); if ($r) { say("BAD"); } else { say("OK"); } }
EOF

# --- RK-GRAM: built-in character-class subrules. Raku's predefined named character classes
#     (<digit> <alpha> <alnum> <upper> <lower> <space> <ws> <xdigit>) now resolve to their regex
#     class when a <name> isn't a user-defined subrule (user subrules still win — checked first).
#     Maps in gram_expand via rk_grammar_builtin_class. Quantifiers and inter-token whitespace compose.
#     Shared by both modes. ---
raku "grammar_builtin_digit" "123" << 'EOF'
grammar G { rule TOP { <digit>+ } }
sub main() { say(G.parse("123")); }
EOF
raku "grammar_builtin_alpha" "abc" << 'EOF'
grammar G { rule TOP { <alpha>+ } }
sub main() { say(G.parse("abc")); }
EOF
raku "grammar_builtin_upper_lower" "Hello" << 'EOF'
grammar G { rule TOP { <upper> <lower>+ } }
sub main() { say(G.parse("Hello")); }
EOF
raku "grammar_builtin_space" "1 2" << 'EOF'
grammar G { rule TOP { <digit> <space> <digit> } }
sub main() { say(G.parse("1 2")); }
EOF
raku "grammar_builtin_nomatch" "N" << 'EOF'
grammar G { rule TOP { <digit>+ } }
sub main() { my $r = G.parse("abc"); if ($r) { say("Y"); } else { say("N"); } }
EOF

# --- RK-GRAM-3b NATIVE BOXES: the literal + char-class leaf boxes EXECUTED via the .parse scan-entry
#     trampoline (rk_gram_enter_box -> box alpha, delta read back, full-match = delta==Delta), NOT the NFA
#     fallback. Gated behind RK_GRAM_NATIVE so only THESE five take the native path; output is byte-identical
#     to the NFA path. Both modes: m3 in-process, m4 standalone binary via libscrip_rt (proc registered at
#     startup by the emitted rt_proc_set_fn, then grammar_parse_core's native branch fires). ---
export RK_GRAM_NATIVE=1
raku "gram_native_lit_full" "abc" << 'EOF'
grammar G { rule TOP { "abc" } }
sub main() { say(G.parse("abc")); }
EOF
raku "gram_native_lit_partial_fail" "N" << 'EOF'
grammar G { rule TOP { "abc" } }
sub main() { my $r = G.parse("abcd"); if ($r) { say("Y"); } else { say("N"); } }
EOF
raku "gram_native_cc_digit" "5" << 'EOF'
grammar G { rule TOP { <digit> } }
sub main() { say(G.parse("5")); }
EOF
raku "gram_native_cc_nonmember_fail" "N" << 'EOF'
grammar G { rule TOP { <digit> } }
sub main() { my $r = G.parse("x"); if ($r) { say("Y"); } else { say("N"); } }
EOF
raku "gram_native_cc_twochar_fail" "N" << 'EOF'
grammar G { rule TOP { <digit> } }
sub main() { my $r = G.parse("55"); if ($r) { say("Y"); } else { say("N"); } }
EOF
raku "gram_native_cc_alpha" "a" << 'EOF'
grammar G { rule TOP { <alpha> } }
sub main() { say(G.parse("a")); }
EOF
raku "gram_native_cc_alpha_fail" "N" << 'EOF'
grammar G { rule TOP { <alpha> } }
sub main() { my $r = G.parse("1"); if ($r) { say("Y"); } else { say("N"); } }
EOF
raku "gram_native_cc_upper" "A" << 'EOF'
grammar G { rule TOP { <upper> } }
sub main() { say(G.parse("A")); }
EOF
raku "gram_native_cc_upper_fail" "N" << 'EOF'
grammar G { rule TOP { <upper> } }
sub main() { my $r = G.parse("a"); if ($r) { say("Y"); } else { say("N"); } }
EOF
raku "gram_native_cc_lower" "z" << 'EOF'
grammar G { rule TOP { <lower> } }
sub main() { say(G.parse("z")); }
EOF
raku "gram_native_cc_xdigit" "f" << 'EOF'
grammar G { rule TOP { <xdigit> } }
sub main() { say(G.parse("f")); }
EOF
raku "gram_native_cc_xdigit_fail" "N" << 'EOF'
grammar G { rule TOP { <xdigit> } }
sub main() { my $r = G.parse("g"); if ($r) { say("Y"); } else { say("N"); } }
EOF
raku "gram_native_cc_alnum" "7" << 'EOF'
grammar G { rule TOP { <alnum> } }
sub main() { say(G.parse("7")); }
EOF
raku "gram_native_cc_space" "Y" << 'EOF'
grammar G { rule TOP { <space> } }
sub main() { my $r = G.parse(" "); if ($r) { say("Y"); } else { say("N"); } }
EOF
raku "gram_native_lit_singlechar" "x" << 'EOF'
grammar G { rule TOP { "x" } }
sub main() { say(G.parse("x")); }
EOF
raku "gram_native_lit_with_space" "a b" << 'EOF'
grammar G { rule TOP { "a b" } }
sub main() { say(G.parse("a b")); }
EOF
# --- RK-GRAM-3c NATIVE SEQUENCE: leaf boxes chained (each gamma -> next alpha) into a box graph; delta
#     threads through the chain (box i advances delta, box i+1 resumes at it); tail gamma = full-match exit,
#     any box omega = fail (delta untouched). No NFA on the .parse path for these shapes. THE GATE is
#     gram_seq_digit_alpha (rule TOP { <digit> <alpha> }); the rest are edges (short-input fail, wrong-char
#     fail, literal+class mix, three-element chain, char-class then literal). ---
raku "gram_seq_digit_alpha" "5x" << 'EOF'
grammar G { rule TOP { <digit> <alpha> } }
sub main() { say(G.parse("5x")); }
EOF
raku "gram_seq_digit_alpha_short_fail" "N" << 'EOF'
grammar G { rule TOP { <digit> <alpha> } }
sub main() { my $r = G.parse("5"); if ($r) { say("Y"); } else { say("N"); } }
EOF
raku "gram_seq_digit_alpha_firstwrong_fail" "N" << 'EOF'
grammar G { rule TOP { <digit> <alpha> } }
sub main() { my $r = G.parse("xx"); if ($r) { say("Y"); } else { say("N"); } }
EOF
raku "gram_seq_digit_alpha_secondwrong_fail" "N" << 'EOF'
grammar G { rule TOP { <digit> <alpha> } }
sub main() { my $r = G.parse("55"); if ($r) { say("Y"); } else { say("N"); } }
EOF
raku "gram_seq_lit_class" "a1" << 'EOF'
grammar G { rule TOP { "a" <digit> } }
sub main() { say(G.parse("a1")); }
EOF
raku "gram_seq_class_lit" "9z" << 'EOF'
grammar G { rule TOP { <digit> "z" } }
sub main() { say(G.parse("9z")); }
EOF
raku "gram_seq_three_class" "3aB" << 'EOF'
grammar G { rule TOP { <digit> <lower> <upper> } }
sub main() { say(G.parse("3aB")); }
EOF
raku "gram_seq_three_class_midfail" "N" << 'EOF'
grammar G { rule TOP { <digit> <lower> <upper> } }
sub main() { my $r = G.parse("3XB"); if ($r) { say("Y"); } else { say("N"); } }
EOF
# --- RK-GRAM-3d-m3-fix: IR_GALT (alternation) both-media regression lock.
#     x86("jmp", lbl_t0) was XK_SYM → text-only; binary silently dropped arm jumps,
#     fell through to x86_gamma() — arm-1 never ran, r14=0, final_delta=0 always.
#     Fix: x86_jmp_lblptr(ptr,name) works in both media. ---
raku "gram_alt_arm1_m3" "a" << 'EOF'
grammar G { rule TOP { "a" | "b" } }
sub main() { say(G.parse("a")); }
EOF
raku "gram_alt_arm2_m3" "b" << 'EOF'
grammar G { rule TOP { "a" | "b" } }
sub main() { say(G.parse("b")); }
EOF
raku "gram_alt_nomatch_m3" "N" << 'EOF'
grammar G { rule TOP { "a" | "b" } }
sub main() { my $r = G.parse("c"); if ($r) { say("Y"); } else { say("N"); } }
EOF
raku "gram_alt_cc_m3" "5" << 'EOF'
grammar G { rule TOP { <digit> | <alpha> } }
sub main() { say(G.parse("5")); }
EOF
raku "gram_alt_cc_arm2_m3" "x" << 'EOF'
grammar G { rule TOP { <digit> | <alpha> } }
sub main() { say(G.parse("x")); }
EOF
unset RK_GRAM_NATIVE

# --- RK-OO-A1: attribute mutation (twigil-write + void method-call statement) ---
raku "attr_mutate" "3" << 'EOF'
class Counter { has $.n; method bump() { $!n = $!n + 1; } method val() { return $!n; } }
sub main() { my $c = Counter.new(n => 0); $c.bump(); $c.bump(); $c.bump(); say($c.val()); }
EOF

# --- RK-OO-A2 `is rw`: external accessor write is gated on the `is rw` trait (Raku default is readonly).
#     WITH `is rw` the public accessor returns a writable value -> `$p.x = v` succeeds. WITHOUT it the
#     attribute is immutable and external assignment DIES (X::Assignment::RO "Cannot modify an immutable").
#     Internal `$!x = v` (direct attribute, TWIGIL) is ALWAYS allowed regardless of `is rw`. ---
raku "field_write_rw" "5" << 'EOF'
class P { has $.x is rw; }
sub main() { my $p = P.new(x => 1); $p.x = 5; say($p.x); }
EOF

raku_dies "field_write_ro_dies" "" << 'EOF'
class Q { has $.x; }
sub main() { my $q = Q.new(x => 1); $q.x = 5; say("unreached"); }
EOF

raku "field_write_rw_typed" "99" << 'EOF'
class Box { has Int $.v is rw; }
sub main() { my $b = Box.new(v => 1); $b.v = 99; say($b.v); }
EOF

raku "field_write_rw_inherited" "20" << 'EOF'
class Base { has $.x is rw; }
class Sub is Base { method bump() { $!x = $!x + 5; } }
sub main() { my $s = Sub.new(x => 10); $s.x = 20; say($s.x); }
EOF

# --- RK-OO-A2 privacy: a `$!`-declared attribute gets NO public accessor (Rakudo Attribute.compose only
#     generates an accessor when has_accessor, i.e. the `$.` public twigil). External `$obj.x` on a private
#     attribute finds no method and DIES ("not accessible"); internal `$!x` (direct attribute) ALWAYS works;
#     a public `$.` sibling stays externally accessible. Privacy follows the MRO (inherited private stays
#     private; inherited public stays public). ---
raku_dies "priv_attr_external_dies" "" << 'EOF'
class Secret { has $!code; method reveal() { return $!code; } }
sub main() { my $s = Secret.new(code => 42); say($s.code()); }
EOF

raku "priv_attr_internal_ok" "42" << 'EOF'
class Secret { has $!code; method reveal() { return $!code; } }
sub main() { my $s = Secret.new(code => 42); say($s.reveal()); }
EOF

raku_dies "priv_attr_inherited_dies" "" << 'EOF'
class Base { has $!secret; method peek() { return $!secret; } }
class Derived is Base { }
sub main() { my $d = Derived.new(secret => 5); say($d.secret()); }
EOF

raku "priv_attr_inherited_internal_ok" "5" << 'EOF'
class Base { has $!secret; method peek() { return $!secret; } }
class Derived is Base { }
sub main() { my $d = Derived.new(secret => 5); say($d.peek()); }
EOF

raku "priv_attr_public_sibling_ok" "A" << 'EOF'
class Mix { has $.pub; has $!prv; method both() { return $.pub ~ "-" ~ $!prv; } }
sub main() { my $m = Mix.new(pub => "A", prv => "B"); say($m.pub); }
EOF

raku "priv_attr_mixed_internal" "A-B" << 'EOF'
class Mix { has $.pub; has $!prv; method both() { return $.pub ~ "-" ~ $!prv; } }
sub main() { my $m = Mix.new(pub => "A", prv => "B"); say($m.both()); }
EOF

# --- RK-OO-A2 privacy, no-paren accessor form: in Raku `$obj.attr` and `$obj.attr()` are BOTH accessor
#     method calls (no syntactic distinction), so a private attribute DIES on the no-paren form too. ---
raku_dies "priv_attr_external_noparen_dies" "" << 'EOF'
class Secret { has $!code; method reveal() { return $!code; } }
sub main() { my $s = Secret.new(code => 42); say($s.code); }
EOF

raku "pub_attr_external_noparen_ok" "Rex" << 'EOF'
class Animal { has $.name; }
class Dog is Animal { }
sub main() { my $d = Dog.new(name => "Rex"); say($d.name); }
EOF

# --- RK-OO-A3 privacy (the previously-deferred `@!`/`%!` enforcement): private aggregate attributes get no
#     public accessor either; external access DIES, while the public `@.`/`%.` accessor still resolves. ---
raku_dies "priv_array_attr_external_dies" "" << 'EOF'
class Stack { has @!items; method size() { return 1; } }
sub main() { my $s = Stack.new(); say($s.items); }
EOF

raku_dies "priv_hash_attr_external_dies" "" << 'EOF'
class Cfg { has %!opts; method ok() { return 1; } }
sub main() { my $c = Cfg.new(); say($c.opts); }
EOF

# --- RK-OO-C1/C2/C4: single inheritance (attr inherit, method inherit, override) ---
raku "inherit_attr" "Rex" << 'EOF'
class Animal { has $.name; }
class Dog is Animal { method bark() { return "woof"; } }
sub main() { my $d = Dog.new(name => "Rex"); say($d.name); }
EOF

raku "inherit_method" "4" << 'EOF'
class Animal { has $.name; method legs() { return 4; } }
class Dog is Animal { }
sub main() { my $d = Dog.new(name => "Rex"); say($d.legs()); }
EOF

raku "inherit_override" "Woof" << 'EOF'
class Animal { has $.name; method speak() { return "..."; } }
class Dog is Animal { method speak() { return "Woof"; } }
sub main() { my $d = Dog.new(name => "Rex"); say($d.speak()); }
EOF

# --- RK-OO-C3: C3 MRO linearization. Three-deep chains exercise the MRO walk that the 2-level
#     tests above do not. For a linear chain the C3 linearization is [child, parent, grandparent, ...];
#     method lookup walks it in order (most-derived wins), TWEAK fires it in reverse (least-derived first). ---
raku "mro_method_grandparent" "A" << 'EOF'
class A { method who() { return "A"; } }
class B is A { }
class C is B { }
sub main() { my $c = C.new(); say($c.who()); }
EOF

raku "mro_override_middle_wins" "B" << 'EOF'
class A { method who() { return "A"; } }
class B is A { method who() { return "B"; } }
class C is B { }
sub main() { my $c = C.new(); say($c.who()); }
EOF

raku "mro_attr_grandparent" "Rex" << 'EOF'
class A { has $.name; }
class B is A { }
class C is B { method tag() { return "C"; } }
sub main() { my $c = C.new(name => "Rex"); say($c.name); }
EOF

raku "mro_tweak_order_3" "$(printf 'A\nB\nC')" << 'EOF'
class A { method TWEAK() { say("A"); } }
class B is A { method TWEAK() { say("B"); } }
class C is B { method TWEAK() { say("C"); } }
sub main() { my $c = C.new(); }
EOF

# --- RK-OO-C5: callsame / nextsame / callwith — re-dispatch to the next candidate in the MRO.
#     callsame/nextsame re-invoke the next class's same-named method with the current args;
#     callwith re-invokes it with explicit args. NOTE: a callwith arg that is a bare computed
#     expression (callwith($n + 1)) hits a PRE-EXISTING general call-arg limitation (a binop
#     call-argument is marshaled as a single leaf — reproduces on clean HEAD with abs($x + 10));
#     bind to a var first (my $m = $n + 1; callwith($m)). ---
raku "callsame_2level" "B+A" << 'EOF'
class A { method g() { return "A"; } }
class B is A { method g() { return "B+" ~ callsame(); } }
sub main() { my $b = B.new(); say($b.g()); }
EOF

raku "callsame_3level" "C+B+A" << 'EOF'
class A { method g() { return "A"; } }
class B is A { method g() { return "B+" ~ callsame(); } }
class C is B { method g() { return "C+" ~ callsame(); } }
sub main() { my $c = C.new(); say($c.g()); }
EOF

raku "nextsame_passes_args" "B+A:7" << 'EOF'
class A { method g($n) { return "A:" ~ $n; } }
class B is A { method g($n) { return "B+" ~ nextsame(); } }
sub main() { my $b = B.new(); say($b.g(7)); }
EOF

raku "callwith_new_arg" "B:5/A:99" << 'EOF'
class A { method g($n) { return "A:" ~ $n; } }
class B is A { method g($n) { return "B:" ~ $n ~ "/" ~ callwith(99); } }
sub main() { my $b = B.new(); say($b.g(5)); }
EOF

raku "callwith_var_arg" "B:5/A:15" << 'EOF'
class A { method g($n) { return "A:" ~ $n; } }
class B is A { method g($n) { my $m = $n + 10; return "B:" ~ $n ~ "/" ~ callwith($m); } }
sub main() { my $b = B.new(); say($b.g(5)); }
EOF

# --- RK-OO-C6: multiple inheritance (is A is B) + real c3_merge. Classic diamond
#     D is B is C, both B,C derive A -> C3 linearization [D,B,C,A]. Method lookup picks the
#     most-derived; callsame walks the FULL C3 order (B's callsame reaches sibling C, then A);
#     attributes from every ancestor merge into D. ---
raku "diamond_method_c3" "B" << 'EOF'
class A { method greet() { return "A"; } }
class B is A { method greet() { return "B"; } }
class C is A { method greet() { return "C"; } }
class D is B is C { }
sub main() { my $d = D.new(); say($d.greet()); }
EOF

raku "diamond_callsame_c3order" "B>C>A" << 'EOF'
class A { method greet() { return "A"; } }
class B is A { method greet() { return "B>" ~ callsame(); } }
class C is A { method greet() { return "C>" ~ callsame(); } }
class D is B is C { }
sub main() { my $d = D.new(); say($d.greet()); }
EOF

raku "diamond_attr_merge" "XY" << 'EOF'
class A { has $.base; }
class B is A { }
class C is A { has $.mid; }
class D is B is C { }
sub main() { my $d = D.new(base => "X", mid => "Y"); say($d.base ~ $d.mid); }
EOF

raku "mi_two_parents_methods" "$(printf 'hi\nbye')" << 'EOF'
class Greeter { method hello() { return "hi"; } }
class Farewell { method bye() { return "bye"; } }
class Both is Greeter is Farewell { }
sub main() { my $b = Both.new(); say($b.hello()); say($b.bye()); }
EOF

# --- RK-OO-D1: roles. `role R { ... }` declares a role; `class C does R` FLATTENS R's methods and
#     attributes into C (Rakudo RoleToClassApplier semantics). Composition is compile-time: the role's
#     methods compile once as R__m and the consuming class finds them via a composition-lookup in the
#     resolver — the role is NOT placed in the MRO (does != is). Precedence is own > role > inherited:
#     at each MRO level the class's own method is tried, then its composed roles, before descending.
#     Role attributes become real fields on the instance (merged into the class DatType). Both modes:
#     mode-4 bakes class_compose_role@PLT so the binary's class carries its roles list. ---
raku "role_method_flatten" "hi" << 'EOF'
role Greet { method hello() { return "hi"; } }
class Dog does Greet { has $.name; }
sub main() { my $d = Dog.new(name => "Rex"); say($d.hello()); }
EOF
raku "role_attr_on_consumer" "42" << 'EOF'
role HasId { has $.id; }
class Widget does HasId { }
sub main() { my $w = Widget.new(id => 42); say($w.id); }
EOF
raku "role_own_method_wins" "class" << 'EOF'
role R { method who() { return "role"; } }
class C does R { method who() { return "class"; } }
sub main() { my $c = C.new(); say($c.who()); }
EOF
raku "role_beats_inherited" "role" << 'EOF'
class Base { method who() { return "base"; } }
role R { method who() { return "role"; } }
class C is Base does R { }
sub main() { my $c = C.new(); say($c.who()); }
EOF

# --- RK-OO-D2: role method-conflict detection (Rakudo RoleToClassApplier check_local_method /
#     X::Role::Unresolved::Method). When 2+ composed roles supply the same method name and the consuming
#     class does NOT provide its own method of that name, that is an UNRESOLVED conflict and a COMPILE-TIME
#     error (composition is compile-time). Detection lives in class_compose_role driven by per-type method
#     lists on DatType; it fires before main in both modes (in-process during m3 --run; at binary startup
#     in m4 — and also in-process at m4 COMPILE since lowering runs class_compose_role, so the .s is refused).
#     A class that defines its own same-named method RESOLVES the conflict (class wins). Distinct method
#     names across roles compose without conflict. Conflict case asserts empty stdout (die on stderr). ---
raku_dies "role_conflict_unresolved" "" << 'EOF'
role A { method m() { return "a"; } }
role B { method m() { return "b"; } }
class C does A does B { }
sub main() { my $c = C.new(); say($c.m()); }
EOF
raku "role_conflict_resolved" "C" << 'EOF'
role A { method m() { return "a"; } }
role B { method m() { return "b"; } }
class C does A does B { method m() { return "C"; } }
sub main() { my $c = C.new(); say($c.m()); }
EOF
raku "role_two_roles_distinct" "$(printf 'f\nb')" << 'EOF'
role A { method foo() { return "f"; } }
role B { method bar() { return "b"; } }
class C does A does B { }
sub main() { my $c = C.new(); say($c.foo()); say($c.bar()); }
EOF

# --- RK-OO-D3: required (stub) methods. A role method whose body is the yada `{...}` declares a REQUIREMENT,
#     not an implementation (Rakudo RoleToClassApplier @stubs / X::Role::Unimplemented). The consuming class
#     MUST provide a real method of that name (itself or via a sibling role); otherwise it is a COMPILE-TIME
#     error. `{...}` lexes to a YADA token -> a TT_YADA-bodied TT_SUB_DECL, which the lowerer recognizes as a
#     stub (and does NOT register/compile as a real proc). The requirement is checked in the composition pass
#     from the AST; unsatisfied -> rt_script_die_surface (compile-time death, empty stdout both modes). ---
raku_dies "required_unimplemented" "" << 'EOF'
role R { method needed() {...} }
class C does R { }
sub main() { my $c = C.new(); say("made"); }
EOF
raku "required_by_class" "7" << 'EOF'
role R { method needed() {...} }
class C does R { method needed() { return 7; } }
sub main() { my $c = C.new(); say($c.needed()); }
EOF
raku "required_by_sibling" "ok" << 'EOF'
role Need { method act() {...} }
role Give { method act() { return "ok"; } }
class C does Need does Give { }
sub main() { my $c = C.new(); say($c.act()); }
EOF

# --- RK-OO-D4: role punning. Using a role directly where a class is expected (`Role.new(...)`) auto-puns the
#     role into a class that does only that role (Rakudo RolePunning). Here it falls out of the role-as-DatType
#     registration: a role registers a DatType (fields) and its methods as Role__m, so `Role.new` constructs an
#     instance and method/attribute access resolves through the role's own name exactly like a class. (Pun
#     identity is simplified to the role's own type; the common instantiate/call/access path is correct.) ---
raku "pun_method" "hello" << 'EOF'
role Speaks { method talk() { return "hello"; } }
sub main() { my $s = Speaks.new(); say($s.talk()); }
EOF
raku "pun_attr" "5" << 'EOF'
role Point { has $.x; }
sub main() { my $p = Point.new(x => 5); say($p.x); }
EOF

# --- RK-OO-A2: public-attribute auto-accessor (.x() with parens routes to the field, not a missing proc) ---
raku "accessor_paren" "3" << 'EOF'
class Point { has $.x; has $.y; }
sub main() { my $p = Point.new(x => 3, y => 4); say($p.x()); }
EOF

raku "accessor_method_wins" "105" << 'EOF'
class Box { has $.v; method v() { return $!v + 100; } }
sub main() { my $b = Box.new(v => 5); say($b.v()); }
EOF

raku "accessor_inherited" "Rex" << 'EOF'
class Animal { has $.name; }
class Dog is Animal { method bark() { return "woof"; } }
sub main() { my $d = Dog.new(name => "Rex"); say($d.name()); }
EOF

# --- RK-OO-B1: user `method new` overrides built-in obj_new; `self.bless(k => v)` constructs ---
raku "method_new_override" "37" << 'EOF'
class Temperature { has $.celsius; method new($c) { return self.bless(celsius => $c); } }
sub main() { my $t = Temperature.new(37); say($t.celsius); }
EOF

raku "bless_named" "9" << 'EOF'
class T { has $.c; method mk() { return self.bless(c => 9); } }
sub main() { my $t = T.mk(); say($t.c); }
EOF

raku "type_object_method" "9" << 'EOF'
class C { has $.x; method make($v) { return self.bless(x => $v); } }
sub main() { my $o = C.make(9); say($o.x); }
EOF

# --- RK-OO-A4: typed attributes (type ignored) + constant default values (BUILDPLAN op 400) ---
raku "attr_typed" "3" << 'EOF'
class Point { has Int $.x; has Int $.y; }
sub main() { my $p = Point.new(x => 3, y => 4); say($p.x); }
EOF
raku "attr_default_int" "42" << 'EOF'
class Box { has $.v = 42; }
sub main() { my $b = Box.new(); say($b.v); }
EOF
raku "attr_default_str" "hello" << 'EOF'
class Greeter { has $.greeting = "hello"; }
sub main() { my $g = Greeter.new(); say($g.greeting); }
EOF
raku "attr_default_override" "7" << 'EOF'
class Box { has $.v = 42; }
sub main() { my $b = Box.new(v => 7); say($b.v); }
EOF
raku "attr_typed_default" "100" << 'EOF'
class Cfg { has Int $.limit = 100; }
sub main() { my $c = Cfg.new(); say($c.limit); }
EOF
raku "attr_default_inherited" "4" << 'EOF'
class Animal { has $.legs = 4; }
class Dog is Animal { method describe() { return $!legs; } }
sub main() { my $d = Dog.new(); say($d.describe()); }
EOF

# --- RK-OO-B2 op-800 is-required DATA MODEL: present-case (field supplied) constructs + reads back. The
#     absent-case death is wired in dat_construct but its NATIVE surfacing waits on the die-route prerequisite
#     (uncaught die aborts; caught needs try/CATCH which REFUSES) — so only the present case is asserted here. ---
raku "attr_required_present" "7" << 'EOF'
class Point { has $.x is required; has $.y; }
sub main() { my $p = Point.new(x => 7, y => 9); say($p.x); }
EOF
raku "attr_required_typed_present" "12" << 'EOF'
class Box { has Int $.w is required; }
sub main() { my $b = Box.new(w => 12); say($b.w); }
EOF
raku "attr_required_inherited_present" "5" << 'EOF'
class Base { has $.id is required; }
class Sub is Base { method who() { return $!id; } }
sub main() { my $s = Sub.new(id => 5); say($s.who()); }
EOF

# --- RK-OO-B2 op-800 ABSENT case: a missing required attr now SURFACES natively (die-route) — sets the
#     Rakudo X::Attribute::Required message, flushes stdout, exits 1. stdout is therefore EMPTY (the post-
#     construction say() never runs). Both modes. Resolves the death-surfacing deferred at 386ffeb. ---
raku_dies "attr_required_absent" "" << 'EOF'
class Point { has $.x is required; has $.y; }
sub main() { my $p = Point.new(y => 9); say("unreached"); }
EOF
raku_dies "attr_required_absent_inherited" "" << 'EOF'
class Base { has $.id is required; }
class Sub is Base { method who() { return $!id; } }
sub main() { my $s = Sub.new(); say($s.who()); }
EOF
# --- native die route: an uncaught die surfaces to stderr + exit 1; statements after it do NOT run, so only
#     the pre-die output reaches stdout (the death is on stderr, not asserted here). Both modes. ---
raku_dies "die_uncaught_halts" "before" << 'EOF'
sub main() { say("before"); die("boom"); say("after"); }
EOF

# --- RK-OO-B3: TWEAK submethod auto-fires at construction (Rakudo BUILDALLPLAN). Fired from the dat_construct
#     chokepoint (so obj_new / bless / type-name construct all run it); each class's own TWEAK fires, parent
#     before child (least-derived first), correctly non-inheriting (each TWEAK is a distinctly-named proc). ---
raku "tweak_fires" "$(printf 'tweaked\ndone')" << 'EOF'
class Widget { has $.id; method TWEAK() { say("tweaked"); } }
sub main() { my $w = Widget.new(id => 1); say("done"); }
EOF
raku "build_named_custom" "$(printf '10\n10')" << 'EOF'
class Point { has $.x; has $.y; submethod BUILD(:$x, :$y) { $!x = $x * 2; $!y = $y; } }
sub main() { my $p = Point.new(x => 5, y => 10); say($p.x); say($p.y); }
EOF
raku "build_autofill_suppressed" "$(printf '1\n[]')" << 'EOF'
class C { has $.x; has $.y; submethod BUILD(:$x) { $!x = $x; } }
sub main() { my $c = C.new(x => 1, y => 99); say($c.x); say("[" ~ $c.y ~ "]"); }
EOF
raku "build_inherited_order" "$(printf 'buildA\nbuildB\ntweakB\n1\n2')" << 'EOF'
class A { has $.a; submethod BUILD(:$a) { say("buildA"); $!a = $a; } }
class B is A { has $.b; submethod BUILD(:$b) { say("buildB"); $!b = $b; } method TWEAK() { say("tweakB"); } }
sub main() { my $o = B.new(a => 1, b => 2); say($o.a); say($o.b); }
EOF
raku "build_required_set" "42" << 'EOF'
class R { has $.id is required; submethod BUILD(:$id) { $!id = $id; } }
sub main() { my $r = R.new(id => 42); say($r.id); }
EOF
raku "build_method_keyword" "$(printf '7\n8')" << 'EOF'
class Q { has $.m; has $.n; method BUILD(:$m, :$n) { $!m = $m; $!n = $n; } }
sub main() { my $q = Q.new(m => 7, n => 8); say($q.m); say($q.n); }
EOF
raku "handles_delegate" "$(printf 'vroom\nhalt\ntesla')" << 'EOF'
class Engine { method start() { return "vroom"; } method stop() { return "halt"; } }
class Car { has $.engine handles <start stop>; has $.name; }
sub main() { my $e = Engine.new(); my $c = Car.new(engine => $e, name => "tesla"); say($c.start()); say($c.stop()); say($c.name); }
EOF
raku "handles_with_arg" "105" << 'EOF'
class Adder { method add($n) { return $n + 100; } }
class Wrap { has $.impl handles <add>; }
sub main() { my $a = Adder.new(); my $w = Wrap.new(impl => $a); say($w.add(5)); }
EOF
raku "handles_two_attrs" "$(printf 'LL\nRR')" << 'EOF'
class L { method left() { return "LL"; } }
class R { method right() { return "RR"; } }
class Pair { has $.l handles <left>; has $.r handles <right>; }
sub main() { my $ll = L.new(); my $rr = R.new(); my $p = Pair.new(l => $ll, r => $rr); say($p.left()); say($p.right()); }
EOF
raku "handles_inherited" "GO" << 'EOF'
class Engine { method go() { return "GO"; } }
class Vehicle { has $.engine handles <go>; }
class Truck is Vehicle { }
sub main() { my $e = Engine.new(); my $t = Truck.new(engine => $e); say($t.go()); }
EOF
raku "tweak_derived_attr" "25" << 'EOF'
class Box { has $.w; has $.area; method TWEAK() { $!area = $!w * $!w; } }
sub main() { my $b = Box.new(w => 5); say($b.area); }
EOF
raku "tweak_inherited_order" "$(printf 'base-tweak\nsub-tweak')" << 'EOF'
class Base { has $.x; method TWEAK() { say("base-tweak"); } }
class Sub is Base { has $.y; method TWEAK() { say("sub-tweak"); } }
sub main() { my $s = Sub.new(x => 1, y => 2); }
EOF

# --- RK-OO-F (intro): .^name metamethod — the '^' metaop was LEXER-BLOCKED; now reachable after the verified
#     flex-2.6.4 lexer regen (raku.lex.c rebuilt behavior-equivalent). Runtime handler reads the instance/type's
#     existing class name, so both modes work with no per-class emission. Returns the class name (Rakudo Metamodel). ---
raku "meta_name_instance" "Dog" << 'EOF'
class Dog { has $.name; }
sub main() { my $d = Dog.new(name => "Rex"); say($d.^name); }
EOF
raku "meta_name_typeobj" "Cat" << 'EOF'
class Cat { }
sub main() { say(Cat.^name); }
EOF

# --- RK-OO-F `.WHAT`: returns the type object (modeled as the class-name string, consistent with `.^name`
#     and type-object representation). Instance form `$obj.WHAT` rides the field-read resolver; the paren
#     form (`$obj.WHAT()`, `Class.WHAT()`) rides `meth_call`. Runtime handler reads the existing class name,
#     so both modes work with no per-class emission. (Bare `Class.WHAT` no-paren is the pre-existing
#     bareword-method grammar limitation, not specific to WHAT.) ---
raku "what_instance" "Point" << 'EOF'
class Point { has $.x; }
sub main() { my $p = Point.new(x => 1); say($p.WHAT); }
EOF
raku "what_typeobj_paren" "Cat" << 'EOF'
class Cat { }
sub main() { say(Cat.WHAT()); }
EOF
raku "what_subclass" "Dog" << 'EOF'
class Animal { has $.name; }
class Dog is Animal { }
sub main() { my $d = Dog.new(name => "Rex"); say($d.WHAT); }
EOF

# --- RK-OO-A3: `@.`/`%.` array & hash attributes. Aggregate attrs auto-vivify to an EMPTY aggregate
#     (the `\x01`-separated empty string ""), distinct from a scalar attr's Any. Lexer mints
#     VAR_ARRAY_TWIGIL/VAR_HASH_TWIGIL for @./@!/%./%!; a per-field `sigil` on DatType drives the
#     vivification in dat_construct (and is replayed into the m4 binary via dat_set_field_sigil@PLT,
#     mirroring is-rw). Accessor result is bound to a var before further method calls (existing grammar
#     limit). `.new(field => @var)` plumbs a provided aggregate, overriding the empty default. ---
raku "attr_array_empty" "0" << 'EOF'
class Stack { has @.items; }
sub main() { my $s = Stack.new(); my @e = $s.items; say(@e.elems); }
EOF
raku "attr_array_init" "$(printf '3\n20')" << 'EOF'
class Stack { has @.items; }
sub main() { my @v = 10, 20, 30; my $t = Stack.new(items => @v); my @i = $t.items; say(@i.elems); say(arr_get(@i, 1)); }
EOF
raku "attr_array_inherited" "0" << 'EOF'
class Base { has @.log; }
class Derived is Base { has $.id; }
sub main() { my $d = Derived.new(id => 7); my @l = $d.log; say(@l.elems); }
EOF
raku "attr_array_in_method" "4" << 'EOF'
class Bag { has @.items; method count() { my @c = @.items; return @c.elems; } }
sub main() { my @v = 1, 2, 3, 4; my $b = Bag.new(items => @v); say($b.count()); }
EOF
raku "attr_hash_empty" "0" << 'EOF'
class Config { has %.opts; }
sub main() { my $c = Config.new(); my $h = $c.opts; say(hash_exists($h, 'x')); }
EOF
raku "attr_hash_init" "Raku" << 'EOF'
class Config { has %.opts; }
sub main() { my %seed = ''; hash_set(%seed, 'lang', 'Raku'); my $d = Config.new(opts => %seed); my $h2 = $d.opts; say(hash_get($h2, 'lang')); }
EOF

# --- ~~ smartmatch verdict: regex rides the C NFA matcher (re.c); m3/m4 cleanly REFUSE (regex is run-only here) ---
raku "smatch digits => match" "match" <<'EOF'
sub main() { if ('abc123' ~~ /\d+/) { say("match"); } else { say("nomatch"); } }
EOF
raku "smatch anchored => nomatch" "nomatch" <<'EOF'
sub main() { if ('abc' ~~ /^\d+$/) { say("match"); } else { say("nomatch"); } }
EOF
raku "smatch named capture" "hello" <<'EOF'
sub main() { my $s = "hello"; if ($s ~~ /<word>([a-z]+)/) { say($<word>); } }
EOF

# --- RK-OO-E1..2: multi-dispatch (multi sub). Candidates register under signature-mangled proc names
#     (base$arity$T0$T1...); a call to the base name routes through the __multi_call runtime dispatcher,
#     which enumerates the proc registry, filters by arity + per-arg type acceptance, and invokes the
#     NARROWEST accepting candidate (Rakudo is_narrower: subtype beats supertype, typed beats untyped).
#     Both native modes. ---
raku "multi_arity" "$(printf 'one: x\ntwo: y,z')" << 'EOF'
multi sub greet($a) { say("one: " ~ $a); }
multi sub greet($a, $b) { say("two: " ~ $a ~ "," ~ $b); }
sub main() { greet("x"); greet("y", "z"); }
EOF
raku "multi_type_int_str" "$(printf 'integer 42\nstring hello')" << 'EOF'
multi sub describe(Int $x) { say("integer " ~ $x); }
multi sub describe(Str $x) { say("string " ~ $x); }
sub main() { describe(42); describe("hello"); }
EOF
raku "multi_typed_beats_untyped" "$(printf 'specific int\nfallback')" << 'EOF'
multi sub f(Int $x) { say("specific int"); }
multi sub f($x) { say("fallback"); }
sub main() { f(5); f("str"); }
EOF
raku "multi_subclass_beats_parent" "a dog" << 'EOF'
class Animal { }
class Dog is Animal { }
multi sub describe(Animal $a) { say("an animal"); }
multi sub describe(Dog $d) { say("a dog"); }
sub main() { my $d = Dog.new(); describe($d); }
EOF
raku "multi_two_typed_args" "$(printf 'int+int\nstr+str')" << 'EOF'
multi sub combine(Int $a, Int $b) { say("int+int"); }
multi sub combine(Str $a, Str $b) { say("str+str"); }
sub main() { combine(1, 2); combine("a", "b"); }
EOF

# --- RK-OO-E `multi method`: method-side multi-dispatch (Rakudo MROBasedMethodDispatch over typed candidates).
#     Candidates register as `Class__name$arity$T0...` procs; `meth_call` routes a base-name call with no direct
#     `Class__name` proc through an MRO-scoped dispatcher (mirrors `__multi_call`, invocant threaded as arg0) that
#     filters by arity + per-arg type acceptance and invokes the narrowest. Candidates compose across the C3 MRO. ---
raku "multi_method_type" "$(printf 'int:42\nstr:hi')" << 'EOF'
class Printer {
    multi method show(Int $x) { return "int:" ~ $x; }
    multi method show(Str $s) { return "str:" ~ $s; }
}
sub main() { my $p = Printer.new(); say($p.show(42)); say($p.show("hi")); }
EOF

raku "multi_method_arity" "$(printf '5\n7')" << 'EOF'
class Calc {
    multi method add(Int $a) { return $a; }
    multi method add(Int $a, Int $b) { return $a + $b; }
}
sub main() { my $c = Calc.new(); say($c.add(5)); say($c.add(3, 4)); }
EOF

raku "multi_method_mro_inherited" "$(printf 'base-int:7\nsub-str:yo')" << 'EOF'
class Base { multi method describe(Int $x) { return "base-int:" ~ $x; } }
class Sub is Base { multi method describe(Str $s) { return "sub-str:" ~ $s; } }
sub main() { my $o = Sub.new(); say($o.describe(7)); say($o.describe("yo")); }
EOF

raku "multi_method_subclass_narrower" "a dog" << 'EOF'
class Animal { }
class Dog is Animal { }
class Handler {
    multi method greet(Animal $a) { return "an animal"; }
    multi method greet(Dog $d) { return "a dog"; }
}
sub main() { my $h = Handler.new(); my $d = Dog.new(); say($h.greet($d)); }
EOF

# --- RK-OO-F: .isa / .does type tests. `.isa(T)` is true iff T is in the object's class MRO (nominal
#     inheritance, includes self); `.does(R)` additionally consults composed roles. Returns Bool (1/0,
#     SCRIP's boolean print form). Handled in meth_call before user-method resolution; both native modes. ---
raku "isa_true_self_and_parent" "$(printf '1\n1')" << 'EOF'
class Animal { }
class Dog is Animal { }
sub main() { my $d = Dog.new(); say($d.isa(Animal)); say($d.isa(Dog)); }
EOF
raku "isa_false_sibling" "$(printf '1\n0')" << 'EOF'
class Animal { }
class Dog is Animal { }
class Cat is Animal { }
sub main() { my $d = Dog.new(); say($d.isa(Animal)); say($d.isa(Cat)); }
EOF
raku "does_role" "$(printf '1\n1')" << 'EOF'
role Barker { method bark() { say("woof"); } }
class Dog does Barker { }
sub main() { my $d = Dog.new(); say($d.does(Barker)); say($d.isa(Dog)); }
EOF
raku "meta_parents_chain" "$(printf 'Dog\nMammal Animal')" << 'EOF'
class Animal { }
class Mammal is Animal { }
class Dog is Mammal { }
sub main() { my $d = Dog.new(); say($d.^name); say($d.^parents); }
EOF

# --- RK-OO-F: .^methods / .^attributes introspection. Rakudo MethodContainer.nqp / AttributeContainer.nqp:
#     the non-:local form returns the full set across the MRO (most-derived first), including composed-role
#     methods. In SCRIP's string-based metamodel these are space-joined name lists, walked over the C3 MRO
#     by dat_methods/dat_attributes with dedup. Both native modes (the dat method/role/field tables are
#     replayed into the m4 binary via dat_add_method@PLT and the class spec). ---
raku "meta_methods_mro" "bark speak eat" << 'EOF'
class Animal { has $.name; method speak() { return "..."; } method eat() { return "om"; } }
class Dog is Animal { has $.breed; method bark() { return "woof"; } }
sub main() { my $d = Dog.new(name => "Rex", breed => "Lab"); say($d.^methods); }
EOF
raku "meta_attributes_mro" "name breed" << 'EOF'
class Animal { has $.name; method speak() { return "..."; } }
class Dog is Animal { has $.breed; }
sub main() { my $d = Dog.new(name => "Rex", breed => "Lab"); say($d.^attributes); }
EOF
raku "meta_methods_role" "fetch hello" << 'EOF'
role Greet { method hello() { return "hi"; } }
class Svc does Greet { method fetch() { return "ok"; } }
sub main() { my $s = Svc.new(); say($s.^methods); }
EOF
raku "meta_methods_typeobj" "tick" << 'EOF'
class Clock { method tick() { return "t"; } }
sub main() { say(Clock.^methods); }
EOF

# --- RK-OO-F: .clone (Mu.clone). Shallow-copies the invocant's attribute values into a fresh instance of
#     the same class; optional named twiddles (`$obj.clone(attr => v)`) override specific attributes, the rest
#     carried over verbatim. Rakudo Mu.rakumod multi method clone(Mu:D: *%twiddles). Runtime-only (no lexer/
#     grammar): handled in meth_call before user-method resolution, reusing the dat_construct field-fill (so
#     inherited attributes across the MRO copy for free, and the original is untouched). Both native modes. ---
raku "clone_basic" "$(printf '3\n4')" << 'EOF'
class Point { has $.x; has $.y; }
sub main() { my $p = Point.new(x => 3, y => 4); my $q = $p.clone(); say($q.x); say($q.y); }
EOF
raku "clone_twiddle" "$(printf '3\n99\n4')" << 'EOF'
class Point { has $.x; has $.y; }
sub main() { my $p = Point.new(x => 3, y => 4); my $r = $p.clone(y => 99); say($r.x); say($r.y); say($p.y); }
EOF
raku "clone_inherited" "$(printf 'Rex\n4\nPug')" << 'EOF'
class Animal { has $.name; has $.legs; }
class Dog is Animal { has $.breed; }
sub main() { my $d = Dog.new(name => "Rex", legs => 4, breed => "Lab"); my $c = $d.clone(breed => "Pug"); say($c.name); say($c.legs); say($c.breed); }
EOF

# --- RK-OO-F: :D/:U definiteness constraints + .defined + my $x; (uninit decl).
#     :D params accept only defined (non-SNUL) values; :U params accept only undefined (SNUL).
#     Mangled as TypeName_D / TypeName_U in proc names (colon → underscore for GAS safety).
#     Narrowness: TypeName:D beats TypeName (adds constraint). .defined returns 1/0.
#     my $x; produces SNUL (DT_SNUL=0) via the NUL arm of bb_assign_local. Both native modes. ---
raku "defined_method_true_false" "$(printf '1\n0')" << 'EOF'
sub main() { my $x = 5; my $u; say($x.defined); say($u.defined); }
EOF
raku "multi_colon_d_dispatch" "$(printf 'defined int: 42\nother')" << 'EOF'
multi sub process(Int:D $x) { say("defined int: " ~ $x); }
multi sub process($x) { say("other"); }
sub main() { process(42); my $u; process($u); }
EOF
raku "multi_colon_u_dispatch" "uninitialized" << 'EOF'
multi sub init(Str:U $x) { say("uninitialized"); }
multi sub init(Str $x) { say("has value: " ~ $x); }
sub main() { my $u; init($u); }
EOF

# --- RK-OO-F: :D/:U definiteness enforced on NON-multi sub/method params (runtime type-check at binding).
#     A typed :D/:U param lowers to a __param_check("Type:D", $p, "p") builtin call prepended to the proc
#     body (CALL_ROUTE_FN, both modes); the runtime arm calls rt_mc_accepts and fires the die surface on
#     mismatch. :D rejects undef (SNUL); :U rejects defined; Type:D also rejects a defined wrong-type value.
#     Faithful to Rakudo "Type check failed in binding to parameter '$p'". Multi candidates (name has '$')
#     are skipped (dispatch already filters). Uncaught failure halts (die message on stderr).
raku "param_colon_d_passes" "hi Tom" << 'EOF'
sub greet(Str:D $name) { say("hi " ~ $name); }
sub main() { greet("Tom"); }
EOF
raku_dies "param_colon_d_dies" "hi Tom" << 'EOF'
sub greet(Str:D $name) { say("hi " ~ $name); }
sub main() { greet("Tom"); my $u; greet($u); say("after"); }
EOF
raku_dies "param_colon_u_dies" "ok" << 'EOF'
sub want(Str:U $x) { say("ok"); }
sub main() { my $u; want($u); want("v"); say("after"); }
EOF
raku_dies "param_type_mismatch_dies" "int 5" << 'EOF'
sub takes(Int:D $n) { say("int " ~ $n); }
sub main() { takes(5); takes("x"); say("after"); }
EOF

# --- RK-OO-F: plain-type-name param enforcement (the F-tail, previously deferred). A non-:D/:U typed
#     param (Int $x, Str $s, a user class) now runtime-checks its bound argument at proc entry and DIES
#     faithfully ("Type check failed in binding to parameter ...") on a wrong type — Int rejects Str,
#     subtype (a Dog for an Animal param) is ACCEPTED via the MRO check. Only types the runtime models
#     (numeric/string leaves + registered classes) are enforced; unmodeled barewords are skipped so they
#     cannot false-die. Reuses the __param_check builtin + rt_mc_accepts (no new IR/template). Both modes. ---
raku "param_plain_int_passes" "42" << 'EOF'
sub f(Int $x) { say($x); }
sub main() { f(42); }
EOF
raku_dies "param_plain_int_dies" "7" << 'EOF'
sub f(Int $x) { say($x); }
sub main() { f(7); f("x"); say("after"); }
EOF
raku "param_plain_str_passes" "hi" << 'EOF'
sub f(Str $s) { say($s); }
sub main() { f("hi"); }
EOF
raku "param_plain_class_subtype" "5" << 'EOF'
class Animal { has $.n; }
class Dog is Animal { }
sub describe(Animal $a) { say($a.n); }
sub main() { my $d = Dog.new(n => 5); describe($d); }
EOF

# --- RK-OO-G1: user .gist / .Str / .raku override honored in implicit stringification contexts.
#     Rakudo Mu.rakumod: say -> .gist, print/put -> .Str, string coercion (~, interpolation) -> .Str.
#     SCRIP: say($obj) routes through the user's .gist (rt_write_any_nl / out_write_descr);
#     $obj ~ str and "$obj" route through the user's .Str (str_concat_d). Explicit .gist()/.Str()/.raku()
#     dispatch through the normal user-method path. No override => default (class name), unchanged.
#     Routing is value-shape dispatched (DT_DATA), not language-gated. Both native modes. ---
raku "gist_override_say" "Point(3, 4)" << 'EOF'
class Point { has $.x; has $.y; method gist() { return "Point(" ~ $.x ~ ", " ~ $.y ~ ")"; } }
sub main() { my $p = Point.new(x => 3, y => 4); say($p); }
EOF
raku "str_override_concat" "STR:3!" << 'EOF'
class Point { has $.x; method Str() { return "STR:" ~ $.x; } }
sub main() { my $p = Point.new(x => 3); say($p ~ "!"); }
EOF
raku "str_override_interp" "val=S9" << 'EOF'
class P { has $.x; method Str() { return "S" ~ $.x; } }
sub main() { my $p = P.new(x => 9); say("val=$p"); }
EOF
raku "gist_no_override_default" "Empty" << 'EOF'
class Empty { has $.x; }
sub main() { my $e = Empty.new(x => 1); say($e); }
EOF
raku "raku_str_gist_explicit" "$(printf 'G7\nS7\nR7')" << 'EOF'
class P { has $.x; method gist() { return "G" ~ $.x; } method Str() { return "S" ~ $.x; } method raku() { return "R" ~ $.x; } }
sub main() { my $p = P.new(x => 7); say($p.gist()); say($p.Str()); say($p.raku()); }
EOF
raku "gist_override_inherited" "DOG:Rex" << 'EOF'
class Animal { has $.name; method gist() { return "DOG:" ~ $.name; } }
class Dog is Animal { }
sub main() { my $d = Dog.new(name => "Rex"); say($d); }
EOF

# --- RK-OO-G6: .= method-assignment. `$x .= meth(args)` is sugar for `$x = $x.meth(args)`
#     (Rakudo Mu.rakumod dispatch:<.=>: mutate = mutate."$name"(|c)). Pure grammar desugar:
#     new lexer token OP_DOTEQ (.=); productions build TT_ASSIGN(var, TT_METHCALL(var, meth, args)).
#     No new AST kind, no lowering/runtime change — reuses the existing assign + method-call path.
#     Works for builtin methods (no-paren / empty-paren) and user methods returning a new value.
#     Both native modes; bison regen rc=0, 31 conflicts unchanged. ---
raku "dotassign_builtin_uc" "HELLO" << 'EOF'
sub main() { my $s = "hello"; $s .= uc; say($s); }
EOF
raku "dotassign_empty_paren" "abc" << 'EOF'
sub main() { my $s = "ABC"; $s .= lc(); say($s); }
EOF
raku "dotassign_user_meth_arg" "15" << 'EOF'
class Counter { has $.n; method add($k) { return Counter.new(n => $.n + $k); } }
sub main() { my $c = Counter.new(n => 10); $c .= add(5); say($c.n); }
EOF

# --- RK-OO-G2: operator overloading via `multi sub infix:<op>`. Rakudo: an operator is an ordinary multi
#     sub named infix:<+> (prefix:<...>/postfix:<...> similarly); at a `$a OP $b` call site, a user candidate
#     whose typed params accept the operand types WINS over the builtin numeric op. SCRIP: the lexer mints
#     OP_NAME for infix:<...> (canonicalized to an asm-safe base via rk_op_canon_base, since <>+: are not GAS
#     symbol chars); the multi-sub grammar registers the candidate under the usual $-mangled name; the call-site
#     seam lives in the shared IR_BINOP_ARITH box as a value-shape (DT_DATA) guard — if either operand is a Raku
#     object the box diverts to rt_binop_overload, which dispatches through the existing __multi_call machinery
#     and falls through to numeric arithmetic when no candidate matches. Language-blind (DT_DATA tag, not a
#     language gate); the int fast path is unchanged when neither operand is an object. Both native modes. ---
raku "op_overload_add_two_objs" "$(printf '11\n22')" << 'EOF'
class Vec { has $.x; has $.y; }
multi sub infix:<+>(Vec $a, Vec $b) { return Vec.new(x => $a.x + $b.x, y => $a.y + $b.y); }
sub main() { my $p = Vec.new(x => 1, y => 2); my $q = Vec.new(x => 10, y => 20); my $r = $p + $q; say($r.x); say($r.y); }
EOF
raku "op_overload_mixed_obj_int" "$(printf '500\n450')" << 'EOF'
class Money { has $.cents; }
multi sub infix:<+>(Money $a, Money $b) { return Money.new(cents => $a.cents + $b.cents); }
multi sub infix:<*>(Money $a, $n) { return Money.new(cents => $a.cents * $n); }
sub main() { my $a = Money.new(cents => 150); my $b = Money.new(cents => 350); my $s = $a + $b; say($s.cents); my $t = $a * 3; say($t.cents); }
EOF
raku "op_overload_subtype_operands" "30" << 'EOF'
class Animal { has $.size; }
class Dog is Animal { }
multi sub infix:<+>(Animal $a, Animal $b) { return Animal.new(size => $a.size + $b.size); }
sub main() { my $d1 = Dog.new(size => 10); my $d2 = Dog.new(size => 20); my $r = $d1 + $d2; say($r.size); }
EOF
raku "op_overload_int_unaffected" "$(printf '12\n2\n35')" << 'EOF'
class Vec { has $.x; }
multi sub infix:<+>(Vec $a, Vec $b) { return Vec.new(x => $a.x + $b.x); }
sub main() { my $a = 7; my $b = 5; say($a + $b); say($a - $b); say($a * $b); }
EOF
raku "op_overload_relop_objs" "1" << 'EOF'
class Box { has $.n; }
multi sub infix:<<>(Box $a, Box $b) { return $a.n < $b.n; }
sub main() { my $a = Box.new(n => 3); my $b = Box.new(n => 7); say($a < $b); }
EOF
raku "op_overload_relop_subtype" "30" << 'EOF'
class Animal { has $.size; }
class Dog is Animal { }
multi sub infix:<<>(Animal $a, Animal $b) { return $a.size + $b.size; }
sub main() { my $d1 = Dog.new(size => 10); my $d2 = Dog.new(size => 20); say($d1 < $d2); }
EOF
raku "op_overload_relop_int_unaffected" "$(printf '1\n1')" << 'EOF'
sub main() { say(3 < 9); say(5 < 9); }
EOF
raku "for_range_pointy" "$(printf '1\n2\n3\ndone')" << 'EOF'
sub main() { for 1..3 -> $v { say($v); } say('done'); }
EOF
raku "for_range_pointy_toplevel" "$(printf '1\n2\n3\ndone')" << 'EOF'
for 1..3 -> $v { say($v); }
say('done');
EOF

# --- try/CATCH (Rakudo RakuAST::StatementPrefix::Try + LexicalScope CATCH handler semantics, SCRIP subset).
#     die under try rides the four-port ω spine outward (die returns FAILDESCR when g_script_try_depth>0);
#     per-statement exc_check polls (γ=exception→handler, ω=clean→continue) thread through every block lowered
#     under the try via cx->try_catch, so loop pumps cannot swallow a die. Handler: $_ ← exc_get, exc_clear,
#     then body; a die inside the handler propagates to the enclosing try (Rakudo rethrow) or halts at depth 0. ---
raku "try_die_skips_rest" "after" << 'EOF'
sub main() {
    try { die('boom'); say('inside-after'); }
    say('after');
}
EOF
raku "try_catch_topic" "$(printf 'boom\nafter')" << 'EOF'
sub main() {
    try { die('boom'); } CATCH { say($_); }
    say('after');
}
EOF
raku "try_success_no_catch_fire" "$(printf 'ok\nafter')" << 'EOF'
sub main() {
    try { say('ok'); } CATCH { say('never'); }
    say('after');
}
EOF
raku "try_catch_inside_block" "$(printf 'caught\nafter')" << 'EOF'
sub main() {
    try { die('boom'); CATCH { say('caught'); } }
    say('after');
}
EOF
raku "try_die_in_sub_unwinds" "$(printf 'deep\nafter')" << 'EOF'
sub boomer() { die('deep'); say('sub-after'); }
sub main() {
    try { boomer(); say('body-after'); } CATCH { say($_); }
    say('after');
}
EOF
raku "try_die_in_loop_first_iter" "$(printf '1\nlp\nafter')" << 'EOF'
sub main() {
    try { for 1..3 -> $i { say($i); die('lp'); } } CATCH { say($_); }
    say('after');
}
EOF
raku "try_while_die_halts_loop" "$(printf '1\n2\nw\nafter')" << 'EOF'
sub main() {
    my $i = 0;
    try {
        while ($i < 5) { $i = $i + 1; say($i); if ($i == 2) { die('w'); } }
    } CATCH { say($_); }
    say('after');
}
EOF
raku "try_if_arm_die" "$(printf 'cond\nafter')" << 'EOF'
sub main() {
    my $n = 2;
    try { if ($n > 1) { die('cond'); } say('no'); } CATCH { say($_); }
    say('after');
}
EOF
raku "try_nested_inner_outer" "$(printf 'in:inner\nmid\nout:outer\nafter')" << 'EOF'
sub main() {
    try {
        try { die('inner'); } CATCH { say('in:' ~ $_); }
        say('mid');
        die('outer');
    } CATCH { say('out:' ~ $_); }
    say('after');
}
EOF
raku "try_handler_rethrow_outer" "$(printf 'outer:b\nafter')" << 'EOF'
sub main() {
    try {
        try { die('a'); } CATCH { die('b'); }
        say('unreached');
    } CATCH { say('outer:' ~ $_); }
    say('after');
}
EOF
raku "try_sequential_state_clean" "$(printf 'a\nb\nafter')" << 'EOF'
sub main() {
    try { die('a'); } CATCH { say($_); }
    try { say('b'); } CATCH { say('no'); }
    say('after');
}
EOF
raku_dies "try_die_in_handler_uncaught_halts" "pre" << 'EOF'
sub main() {
    say('pre');
    try { die('a'); } CATCH { die('fatal-in-handler'); }
    say('unreached');
}
EOF

# --- RK-BLK-a: bare block as a first-class value (Code/Block per src/core.c/Block.rakumod). A `{ ... }` in
#     expression position is hoisted to an anonymous 0-ary proc; the value is a DT_BLK descriptor carrying
#     the proc name; `$b()` invokes it via the runtime proc registry (both modes, no new box/template).
raku "blk_store_invoke" "5" << 'EOF'
sub main() { my $b = { say 5; }; $b(); }
EOF
raku "blk_invoke_twice" "$(printf '9\n9')" << 'EOF'
sub main() { my $b = { say 9; }; $b(); $b(); }
EOF
raku "blk_return_value" "6" << 'EOF'
sub main() { my $b = { return 3; }; say $b() + $b(); }
EOF
raku "blk_two_distinct" "$(printf '2\n1')" << 'EOF'
sub main() { my $a = { say 1; }; my $b = { say 2; }; $b(); $a(); }
EOF
raku "blk_calls_sub" "7" << 'EOF'
sub g() { say 7; }
sub main() { my $b = { g(); }; $b(); }
EOF
raku "blk_anon_sub_store" "8" << 'EOF'
sub main() { my $s = sub { say 8; }; $s(); }
EOF
raku "blk_anon_sub_return" "4" << 'EOF'
sub main() { my $s = sub { return 4; }; say $s(); }
EOF

raku "bare_block_stmt_toplevel" "42" << 'EOF'
{ say(42); }
EOF
raku "bare_block_stmt_in_sub" "5" << 'EOF'
sub main() { my $x = 5; { say($x); } }
EOF
raku "bare_block_two_stmts" "$(printf '1\n2')" << 'EOF'
{ say(1); say(2); }
EOF
raku "bare_block_then_stmt" "$(printf '1\n2')" << 'EOF'
{ say(1); } say(2);
EOF
raku "bare_block_value_preserved" "7" << 'EOF'
my $b = { say(7); }; $b();
EOF
raku "my_array_uninit_elems" "0" << 'EOF'
my @a; say @a.elems;
EOF
raku "my_hash_uninit_elems" "0" << 'EOF'
my %h; say %h.elems;
EOF
raku "my_array_uninit_then_stmt" "ok" << 'EOF'
my @a; say "ok";
EOF
raku "my_hash_uninit_store_get" "1" << 'EOF'
my %h; %h<x> = 1; say %h<x>;
EOF
raku "wordlist_elems" "3" << 'EOF'
my @a = <a b c>; say @a.elems;
EOF
raku "wordlist_index" "b" << 'EOF'
my @a = <a b c>; say @a[1];
EOF
raku "wordlist_direct_elems" "3" << 'EOF'
say <x y z>.elems;
EOF
raku "wordlist_hash_subscript_coexist" "5" << 'EOF'
my %h; %h<k> = 5; my @w = <a b c>; say %h<k>;
EOF
raku "hash_pairlist_first" "1" << 'EOF'
my %h = a => 1, b => 2; say %h<a>;
EOF
raku "hash_pairlist_second" "2" << 'EOF'
my %h = a => 1, b => 2; say %h<b>;
EOF
raku "hash_pairlist_paren" "20" << 'EOF'
my %h = (x => 10, y => 20); say %h<y>;
EOF
raku "hash_pairlist_elems" "3" << 'EOF'
my %h = a => 1, b => 2, c => 3; say %h.elems;
EOF
raku "hash_pairlist_strkey" "6" << 'EOF'
my %h = "k1" => 5, "k2" => 6; say %h<k2>;
EOF
raku "say_listop_ints" "123" << 'EOF'
say 1, 2, 3;
EOF
raku "say_listop_strs" "abc" << 'EOF'
say "a", "b", "c";
EOF
raku "say_listop_mixed" "x is 5" << 'EOF'
my $x = 5; say "x is ", $x;
EOF
raku "print_listop" "ab" << 'EOF'
print "a", "b"; say "";
EOF
raku "ternary_true" "5" << 'EOF'
my $x = 1 ?? 5 !! 9; say $x;
EOF
raku "ternary_false" "9" << 'EOF'
my $x = 0 ?? 5 !! 9; say $x;
EOF
raku "ternary_shortcircuit" "5" << 'EOF'
sub boom() { say "BOOM"; return 9; }
sub safe() { return 5; }
my $x = 1 ?? safe() !! boom(); say $x;
EOF
raku "ternary_relop_cond" "big" << 'EOF'
my $n = 10; say $n > 3 ?? "big" !! "small";
EOF
raku "ternary_nested_right" "3" << 'EOF'
my $g = "C"; say $g eq "A" ?? 1 !! $g eq "B" ?? 2 !! 3;
EOF

raku "xrep_basic" "ababab" << 'EOF'
say "ab" x 3;
EOF
raku "xrep_fill" "-----" << 'EOF'
say "-" x 5;
EOF
raku "xrep_vars" "yoyoyoyo" << 'EOF'
my $s = "yo"; my $n = 4; say $s x $n;
EOF
raku "xrep_zero_empty" "[]" << 'EOF'
say "[" ~ ("z" x 0) ~ "]";
EOF
raku "xrep_neg_empty" "[]" << 'EOF'
say "[" ~ ("q" x -2) ~ "]";
EOF
raku "xrep_tighter_than_concat" "abb" << 'EOF'
say "a" ~ "b" x 2;
EOF
raku "xrep_concat_both_sides" "1xxx2" << 'EOF'
say 1 ~ "x" x 3 ~ 2;
EOF
raku "xrep_looser_than_add" "55" << 'EOF'
say 2 + 3 x 2;
EOF
raku "xrep_numeric_left" "77" << 'EOF'
say 7 x 2;
EOF

raku "xxrep_array_str" "x x x" << 'EOF'
my @a = "x" xx 3; say @a;
EOF
raku "xxrep_array_elems" "3" << 'EOF'
my @a = "ab" xx 3; say @a.elems;
EOF
raku "xxrep_zeros" "0 0 0 0 0" << 'EOF'
my @b = 0 xx 5; say @b;
EOF
raku "xxrep_var_count" "4" << 'EOF'
my $n = 4; my @c = "yo" xx $n; say @c.elems;
EOF
raku "xxrep_zero_empty" "0" << 'EOF'
my @d = "z" xx 0; say @d.elems;
EOF

# --- RAKU-100: `my sub` lexical routine declarator (Rakudo scoped -> multi-declarator -> routine-declarator;
#     `my` is a scope prefix, the routine body is identical to a package-scoped sub). SCRIP's flat proc
#     registry makes a `my sub` execute exactly as a top-level sub for the shapes the suite exercises.
raku "mysub_noparam" "42" << 'EOF'
my sub answer() { return 42; }
say answer();
EOF
raku "mysub_param" "25" << 'EOF'
my sub sq($x) { return $x * $x; }
say sq(5);
EOF
raku "mysub_body_stmt" "hi world" << 'EOF'
my sub greet($name) { say "hi " ~ $name; }
greet("world");
EOF
raku "mysub_two" "7
10" << 'EOF'
my sub inc($x) { return $x + 1; }
my sub dbl($x) { return $x * 2; }
say inc(6);
say dbl(5);
EOF
# --- RAKU-100: `constant` / `my constant` (Rakudo type-declarator:sym<constant>). Binds a name once to an
#     evaluated value; the name reads back as that value. Sigilless `NAME` and `$NAME` share one slot
#     (sigil-stripped), matching Raku's `constant FOO`/`constant $FOO` synonymy for read purposes.
raku "const_ident" "3" << 'EOF'
constant PI = 3;
say PI;
EOF
raku "const_sigil" "2" << 'EOF'
constant $E = 2;
say $E;
EOF
raku "const_my" "hello" << 'EOF'
my constant GREETING = "hello";
say GREETING;
EOF
raku "const_in_expr" "15" << 'EOF'
constant N = 10;
say N + 5;
EOF
raku "const_my_expr" "9" << 'EOF'
my constant K = 3;
say K * K;
EOF
raku "const_string_concat" "ab" << 'EOF'
constant A = "a";
constant B = "b";
say A ~ B;
EOF

# --- RAKU-100: parenthesized listop argument `say(1,2,3)` / `say (1,2,3)` / `print(...)`. Rakudo `say(|)`
#     joins gists with "" then one newline (src/core.c/io_operators.rakumod), identical to the bare
#     comma-listop `say 1,2,3` path. Generalizes the old exactly-2-arg paren form to N args; `(1,2,3)`
#     was a parse error in listop-argument position before this rung.
raku "sayparen_three" "123" << 'EOF'
say(1,2,3);
EOF
raku "sayparen_space" "123" << 'EOF'
say (1,2,3);
EOF
raku "sayparen_two" "12" << 'EOF'
say(1,2);
EOF
raku "sayparen_strings" "abc" << 'EOF'
say("a","b","c");
EOF
raku "sayparen_exprs" "127" << 'EOF'
my $x = 2;
say(1, $x, 3+4);
EOF
raku "printparen_three" "123" << 'EOF'
print(1,2,3);
EOF
raku "sub_noparen_body" "42" << 'EOF'
sub t { return 42; }
say t();
EOF
raku "sub_noparen_params" "7" << 'EOF'
sub add($a,$b) { return $a + $b; }
say add(3,4);
EOF
raku "sub_implicit_return" "42" << 'EOF'
sub t() { 42 }
say t();
EOF
raku "sub_implicit_return_noparen" "7" << 'EOF'
sub add($a,$b) { $a + $b }
say add(3,4);
EOF
raku "sub_implicit_return_multistmt" "10" << 'EOF'
sub gen { my $x = 5; $x * 2 }
say gen();
EOF
raku "sub_implicit_return_expr" "20" << 'EOF'
sub sq($n) { $n * $n }
say sq(4) + 4;
EOF
raku "my_sub_noparen" "10" << 'EOF'
my sub inc($n) { $n + 1 }
say inc(9);
EOF
raku "sub_trailing_semi" "5" << 'EOF'
sub five() { return 5; };
say five();
EOF
raku "empty_statement" "$(printf '1\n2')" << 'EOF'
say 1;;
say 2;
EOF
raku "sub_implicit_string_return" "hi" << 'EOF'
sub greet() { "hi" }
say greet();
EOF
raku "bare_multi_typed" "30" << 'EOF'
multi foo(Int $x) { return $x * 10; }
say foo(3);
EOF
raku "bare_multi_untyped" "8" << 'EOF'
multi bar($x) { $x + 1 }
say bar(7);
EOF
raku "bare_multi_two_cands" "ab" << 'EOF'
multi describe(Int $x) { return "a"; }
multi describe(Str $x) { return "b"; }
print describe(1), describe("y");
EOF
raku "type_new_noparen" "3" << 'EOF'
class C { has $.x; method greet() { return 3; } }
my $o = C.new;
say $o.greet();
EOF
raku "type_new_noparen_only_method" "woof" << 'EOF'
class Dog { method speak() { return "woof"; } }
my $d = Dog.new;
say $d.speak();
EOF
raku "method_noparen_scalar" "5" << 'EOF'
my $s = "hello";
say $s.chars;
EOF
# --- Method implicit final-expression return (Rakudo: a block's value is its last statement's value; method
#     body is <blockoid>, same primitive as sub). `method sq($n) { $n * $n }` returns $n*$n with no `return`.
raku "method_implicit_return" "36" << 'EOF'
class C { method sq($n) { $n * $n } }
my $o = C.new;
say $o.sq(6);
EOF
raku "method_implicit_return_multistmt" "10" << 'EOF'
class C { method calc($n) { my $t = $n + 1; $t * 2 } }
my $o = C.new;
say $o.calc(4);
EOF
# --- No-paren method signature `method greet { ... }` (canonical Grammar.nqp: the signature `( <signature> )`
#     is optional). Explicit and implicit return both covered. ---
raku "method_noparen_sig_explicit" "hi" << 'EOF'
class C { method greet { return "hi"; } }
my $o = C.new;
say $o.greet();
EOF
raku "method_noparen_sig_implicit" "42" << 'EOF'
class C { method answer { 42 } }
my $o = C.new;
say $o.answer();
EOF
# --- No-paren method CALL `$obj.meth` in expression position (Rakudo: postfix `.name` is a method call;
#     resolves a user method, else falls back to a public-attribute accessor). ---
raku "methcall_noparen_user" "42" << 'EOF'
class C { method answer() { return 42; } }
my $o = C.new;
say $o.answer;
EOF
raku "methcall_noparen_attr_read" "9" << 'EOF'
class P { has $.x; }
my $p = P.new(x => 9);
say $p.x;
EOF
raku "method_noparen_reads_attr" "7" << 'EOF'
class Point { has $.x; has $.y; method sum { $!x + $!y } }
my $p = Point.new(x => 3, y => 4);
say $p.sum;
EOF

raku "list_value_to_scalar" "1 2 3" << 'EOF'
my $x = (1,2,3);
say $x;
EOF

raku "list_value_empty" "$(printf 'A\nB')" << 'EOF'
my $x = ();
say('A');
say('B');
EOF

raku "list_value_single_trailing" "42" << 'EOF'
my $x = (42,);
say $x;
EOF

raku "for_parenlist_pointy" "$(printf '1\n2\n3')" << 'EOF'
for (1,2,3) -> $x { say($x); }
EOF

raku "for_parenlist_topic" "$(printf '1\n2\n3')" << 'EOF'
for (1,2,3) { say($_); }
EOF

raku "for_parenlist_strings" "$(printf 'a\nb\nc')" << 'EOF'
for ("a","b","c") -> $s { say($s); }
EOF

raku "for_array_pointy" "$(printf '10\n20\n30')" << 'EOF'
my @a = 10,20,30;
for @a -> $v { say($v); }
EOF

raku "for_array_topic" "$(printf '7\n8\n9')" << 'EOF'
my @a = 7,8,9;
for @a { say($_); }
EOF

raku "for_array_sum" "6" << 'EOF'
my @a = 1,2,3;
my $s = 0;
for @a -> $v { $s = $s + $v; }
say($s);
EOF

raku "for_emptylist_noiter" "done" << 'EOF'
for () -> $x { say($x); }
say('done');
EOF

raku "for_commalist_pointy" "$(printf '1\n2\n3')" << 'EOF'
for 1,2,3 -> $x { say($x); }
EOF

raku "for_commalist_topic" "$(printf '1\n2\n3')" << 'EOF'
for 1,2,3 { say($_); }
EOF

raku "for_commalist_strings" "$(printf 'x\ny\nz')" << 'EOF'
for "x","y","z" -> $s { say($s); }
EOF

raku "for_wordlist_pointy" "$(printf 'a\nb\nc')" << 'EOF'
for <a b c> -> $x { say($x); }
EOF

# --- optional trailing ';' before '}' (Rakudo eat-terminator: ';' optional at end of block) ---
raku "block_nosemi_for_say" "$(printf '1\n2\n3')" << 'EOF'
for 1..3 -> $v { say $v }
EOF

raku "block_nosemi_for_saynoparen" "$(printf '1\n2\n3')" << 'EOF'
for 1..3 -> $v { say($v) }
EOF

raku "block_nosemi_for_expr" "6" << 'EOF'
my $x = 0;
for 1..3 -> $v { $x = $x + $v }
say $x;
EOF

raku "block_nosemi_for_print" "$(printf 'abc')" << 'EOF'
for <a b c> -> $c { print $c }
EOF

raku "block_nosemi_nested_for" "$(printf '1\n2\n2\n4')" << 'EOF'
for 1..2 -> $i { for 1..2 -> $j { say $i * $j } }
EOF

raku "block_nosemi_if" "big" << 'EOF'
my $x = 5;
if ($x > 3) { say "big" }
EOF

raku "block_nosemi_while" "$(printf '0\n1\n2')" << 'EOF'
my $i = 0;
while ($i < 3) { say $i; $i = $i + 1 }
EOF

raku "block_nosemi_if_else" "$(printf 'one\ntwo')" << 'EOF'
for 1..2 -> $i { if ($i == 1) { say "one" } else { say "two" } }
EOF

raku "block_nosemi_mid_stmt_then_bare" "$(printf '5\n5')" << 'EOF'
for 1..2 { my $x = 5; say $x }
EOF

raku "sub_nosemi_say" "got 42" << 'EOF'
sub announce($x) { say "got $x" }
announce(42);
EOF

raku "method_nosemi_say" "7" << 'EOF'
class C { has $.v; method show() { say $.v } }
my $o = C.new(v => 7);
$o.show();
EOF

raku "sub_return_still_works" "25" << 'EOF'
sub sq($n) { $n * $n }
say sq(5);
EOF

# --- paren-less if/while/unless/until conditions (Rakudo: parens optional on statement-control) ---
raku "noparen_if" "big" << 'EOF'
my $x = 5;
if $x > 3 { say "big" }
EOF

raku "noparen_if_else" "small" << 'EOF'
my $x = 2;
if $x > 3 { say "big" } else { say "small" }
EOF

raku "noparen_dangling_else" "a-only" << 'EOF'
my $a = 1;
my $b = 0;
if $a == 1 { if $b == 1 { say "both" } else { say "a-only" } }
EOF

raku "noparen_elsif_chain" "two" << 'EOF'
my $x = 2;
if $x == 1 { say "one" } else { if $x == 2 { say "two" } else { say "other" } }
EOF

raku "noparen_while" "$(printf '0\n1\n2')" << 'EOF'
my $i = 0;
while $i < 3 { say $i; $i = $i + 1 }
EOF

raku "noparen_unless" "not big" << 'EOF'
my $x = 2;
unless $x > 5 { say "not big" }
EOF

raku "noparen_until" "$(printf '0\n1\n2')" << 'EOF'
my $i = 0;
until $i >= 3 { say $i; $i = $i + 1 }
EOF

raku "paren_if_still_works" "big" << 'EOF'
my $x = 5;
if ($x > 3) { say "big"; }
EOF

# --- RAKU-100: trailing statement-only forms before } (no semicolon) — rung (a).
# Rakudo Grammar.nqp: statementlist = <statement> <.eat-terminator>*, eat-terminator accepts ; OR
# bumping into )]} OR EOF — the terminator is optional before } for EVERY statement, not just exprs.
# Prior session covered trailing bare-expr + say/print; this covers the stmt-only heads (method call,
# field/twigil assign, array/hash element set, take).
raku "trail_methcall_noargs" "$(printf '7\n7')" << 'EOF'
class Box { has $.v; method show() { say $!v } }
my $b = Box.new(v => 7);
for 1..2 { $b.show() }
EOF

raku "trail_methcall_args" "$(printf '1\n2\n3')" << 'EOF'
class Ctr { has $.n; method emit($x) { say $x } }
my $c = Ctr.new(n => 0);
for 1..3 -> $i { $c.emit($i) }
EOF

raku "trail_arr_set" "$(printf '0\n10\n20')" << 'EOF'
my @a = 0 xx 3;
for 0..2 -> $i { @a[$i] = $i * 10 }
say @a[0]; say @a[1]; say @a[2];
EOF

raku "trail_hash_set" "$(printf '100\n200')" << 'EOF'
my %h;
for 1..2 -> $i { %h{$i} = $i * 100 }
say %h{1}; say %h{2};
EOF

raku "trail_methcall_in_if" "done" << 'EOF'
class Sayer { has $.t; method go() { say $!t } }
my $s = Sayer.new(t => "done");
if True { $s.go() }
EOF

raku "trail_semi_still_works" "$(printf '9\n9')" << 'EOF'
class Box2 { has $.v; method show() { say $!v } }
my $b = Box2.new(v => 9);
for 1..2 { $b.show(); }
EOF

# --- RAKU-100: postfix statement modifiers — rung (b).
# Rakudo Grammar.nqp statement-mod-cond:sym<if|unless> + statement-mod-loop:sym<for|while|until>.
# STMT if/unless COND (run once, conditional); STMT while/until COND (loop); STMT for LIST ($_ topic).
# Pure grammar: reuses TT_IF / TT_UNLESS / TT_WHILE / TT_UNTIL / TT_EVERY+TT_ITERATE AST + lowering.
raku "postfix_if_true" "big" << 'EOF'
my $x = 5;
say "big" if $x > 3;
EOF

raku "postfix_if_false_skips" "" << 'EOF'
my $x = 1;
say "big" if $x > 3;
EOF

raku "postfix_unless" "small" << 'EOF'
my $x = 2;
say "small" unless $x > 3;
EOF

raku "postfix_for_topic" "$(printf '1\n2\n3')" << 'EOF'
say $_ for 1..3;
EOF

raku "postfix_while" "3" << 'EOF'
my $i = 0;
$i = $i + 1 while $i < 3;
say $i;
EOF

raku "postfix_until" "3" << 'EOF'
my $i = 0;
$i = $i + 1 until $i >= 3;
say $i;
EOF

raku "postfix_if_on_assign" "$(printf '99\n99')" << 'EOF'
my $x = 10;
my $y = 0;
$y = 99 if $x > 5;
say $y;
$y = 7 if $x > 50;
say $y;
EOF

raku "postfix_print_if" "hi" << 'EOF'
my $ok = 1;
print "hi" if $ok == 1;
EOF

# --- RAKU-100: elsif keyword — rung (c). KW_ELSIF was already lexed but unused in the grammar;
# wired into if_stmt as an else-if chain (paren + paren-less), building nested TT_IF. Rakudo
# Grammar.nqp block-if uses <else>=[ 'elsif'... | 'else'... ]. Pure grammar, no lexer/token change.
raku "elsif_mid" "b" << 'EOF'
my $x = 2;
if $x == 1 { say "a" } elsif $x == 2 { say "b" } else { say "c" }
EOF

raku "elsif_falls_to_else" "c" << 'EOF'
my $x = 9;
if $x == 1 { say "a" } elsif $x == 2 { say "b" } else { say "c" }
EOF

raku "elsif_no_else" "a" << 'EOF'
my $x = 1;
if $x == 1 { say "a" } elsif $x == 2 { say "b" }
EOF

raku "elsif_chain3" "e" << 'EOF'
my $x = 5;
if $x == 1 { say "a" } elsif $x == 2 { say "b" } elsif $x == 5 { say "e" } else { say "z" }
EOF

raku "elsif_paren" "b" << 'EOF'
my $x = 2;
if ($x == 1) { say "a" } elsif ($x == 2) { say "b" } else { say "c" }
EOF

# --- RAKU-100: compound assignment += -= *= /= ~=, prefix/postfix ++/--, defined-or // — rung (c).
# Rakudo: compound-assign at %assignment prec (desugared $x op= e -> $x = $x op e);
# ++ / -- at %auto-increment prec (desugared $x++ -> $x = $x + 1);
# // at %defined-or prec, __rk_dor(a,b) runtime: returns a if defined (non-NUL non-FAIL), else b.
# Semantics: 0 // "x" -> 0 (zero is defined); undef // "x" -> "x".
raku "compound_add_eq" "8" << 'EOF'
my $x = 5;
$x += 3;
say $x;
EOF

raku "compound_sub_eq" "3" << 'EOF'
my $x = 7;
$x -= 4;
say $x;
EOF

raku "compound_mul_eq" "24" << 'EOF'
my $x = 6;
$x *= 4;
say $x;
EOF

raku "compound_div_eq" "4" << 'EOF'
my $x = 24;
$x /= 6;
say $x;
EOF

raku "compound_cat_eq" "hello world" << 'EOF'
my $s = "hello";
$s ~= " world";
say $s;
EOF

raku "postfix_inc" "$(printf '1\n2')" << 'EOF'
my $i = 0;
$i++;
say $i;
$i++;
say $i;
EOF

raku "postfix_dec" "$(printf '2\n1')" << 'EOF'
my $i = 3;
$i--;
say $i;
$i--;
say $i;
EOF

raku "prefix_inc" "6" << 'EOF'
my $n = 5;
++$n;
say $n;
EOF

raku "prefix_dec" "4" << 'EOF'
my $n = 5;
--$n;
say $n;
EOF

raku "defined_or_undef" "default" << 'EOF'
my $x;
say $x // "default";
EOF

raku "defined_or_zero_is_defined" "0" << 'EOF'
my $x = 0;
say $x // "oops";
EOF

raku "defined_or_chain" "$(printf 'ok\nfallback')" << 'EOF'
my $a = "ok";
my $b;
say $a // "nope";
say $b // "fallback";
EOF

raku "return_sub_trailing" "5" << 'EOF'
sub f() { return 5 }
say f();
EOF

raku "return_method_trailing" "7" << 'EOF'
class C { method g() { return 7 } }
my $o = C.new;
say $o.g();
EOF

raku "return_bare_trailing" "hi" << 'EOF'
sub f() { say "hi"; return }
f();
EOF

raku "return_ifelse_branches" "$(printf '2\n1')" << 'EOF'
sub f($x) { if $x { return 1 } else { return 2 } }
say f(0);
say f(1);
EOF

raku "methchain_new_go" "42" << 'EOF'
class C { method go() { 42 } }
say C.new.go();
EOF

raku "methchain_new_go_rval" "7" << 'EOF'
class C { method go() { 7 } }
my $x = C.new.go();
say $x;
EOF

raku "methchain_self_deep" "3" << 'EOF'
class C { method f() { self } method g() { 3 } }
my $o = C.new;
say $o.f().g();
EOF

raku "methcall_mod_for" "$(printf 'g\ng')" << 'EOF'
class C { method go() { say "g" } }
my $o = C.new;
$o.go() for 1..2;
EOF

raku "methcall_mod_if" "g" << 'EOF'
class C { method go() { say "g" } }
my $o = C.new;
$o.go() if 1;
EOF

raku "methcall_mod_if_arg" "9" << 'EOF'
class C { method n($x) { say $x } }
my $o = C.new;
$o.n(9) if 1;
EOF

raku "mod_with_defined" "yes" << 'EOF'
my $x = 5;
say "yes" with $x;
EOF

raku "mod_with_undef_skips" "end" << 'EOF'
my $y;
say "nope" with $y;
say "end";
EOF

raku "mod_with_topicalizes" "42" << 'EOF'
my $x = 42;
say $_ with $x;
EOF

raku "mod_without_undef_fires" "gone" << 'EOF'
my $y;
say "gone" without $y;
EOF

raku "mod_without_defined_skips" "end" << 'EOF'
my $x = 3;
say "nope" without $x;
say "end";
EOF

raku "mod_with_zero_is_defined" "z" << 'EOF'
my $x = 0;
say "z" with $x;
EOF

raku "mod_with_expr_head" "$(printf '7')" << 'EOF'
my $x = 7;
my $r = 0;
$r = $_ with $x;
say $r;
EOF

raku "mod_without_expr_head" "9" << 'EOF'
my $y;
my $r = 0;
$r = 9 without $y;
say $r;
EOF

raku "chain_cmp_in_range" "in" << 'EOF'
my $x = 5;
if 1 < $x < 10 { say "in" }
EOF

raku "chain_cmp_out_high" "out" << 'EOF'
my $x = 50;
if 1 < $x < 10 { say "bad" } else { say "out" }
EOF

raku "chain_cmp_out_low" "out" << 'EOF'
my $x = 1;
if 1 < $x < 10 { say "bad" } else { say "out" }
EOF

raku "chain_cmp_le_inclusive" "in" << 'EOF'
my $x = 1;
if 1 <= $x < 10 { say "in" }
EOF

raku "chain_cmp_descending" "gt" << 'EOF'
my $x = 5;
if 10 > $x > 1 { say "gt" }
EOF

raku "chain_cmp_unless" "fires" << 'EOF'
my $x = 5;
unless 1 < $x < 3 { say "fires" }
EOF

raku "chain_cmp_triple" "yes" << 'EOF'
if 1 < 2 < 3 < 4 { say "yes" }
EOF

raku "chain_cmp_quad_false" "out" << 'EOF'
if 1 < 2 < 9 < 4 { say "bad" } else { say "out" }
EOF

raku "chain_cmp_mixed_ops" "m" << 'EOF'
my $a = 5;
if 1 < $a == 5 { say "m" }
EOF

raku "mod_given_topicalizes" "42" << 'EOF'
say $_ given 42;
EOF

raku "mod_given_var" "$(printf 'val is 7')" << 'EOF'
my $x = 7;
say "val is " ~ $_ given $x;
EOF

raku "mod_given_expr_head" "9" << 'EOF'
my $r = 0;
$r = $_ given 9;
say $r;
EOF

raku "mod_given_runs_once" "$(printf 'x')" << 'EOF'
my $c = "";
$c = $c ~ "x" given 1;
say $c;
EOF

raku "range_ex_pointy" "$(printf '0\n1\n2\n3\n4')" << 'EOF'
for 0 ..^ 5 -> $i { say $i }
EOF

raku "range_ex_var_end" "$(printf '0\n1\n2')" << 'EOF'
my $n = 3;
for 0 ..^ $n -> $i { say $i }
EOF

raku "upto_prefix_pointy" "$(printf '0\n1\n2\n3\n4')" << 'EOF'
for ^5 -> $i { say $i }
EOF

raku "upto_prefix_topic" "$(printf '0\n1\n2')" << 'EOF'
for ^3 { say $_ }
EOF

raku "range_ex_value" "$(printf '1\n2\n3')" << 'EOF'
for 1 ..^ 4 -> $x { say $x }
EOF

raku "pow_basic" "1024" << 'EOF'
say 2 ** 10;
EOF

raku "pow_right_assoc" "512" << 'EOF'
say 2 ** 3 ** 2;
EOF

raku "pow_tighter_than_uminus" "-4" << 'EOF'
say -2 ** 2;
EOF

raku "pow_tighter_than_add" "11" << 'EOF'
say 3 + 2 ** 3;
EOF

raku "pow_tighter_than_mul" "18" << 'EOF'
say 2 * 3 ** 2;
EOF

raku "pow_var_base" "25" << 'EOF'
my $b = 5;
say $b ** 2;
EOF

raku "trail_mod_say_unless_true" "hi" << 'EOF'
my $x = 0;
{ say "hi" unless $x }
EOF

raku "trail_mod_say_unless_false" "" << 'EOF'
my $x = 1;
{ say "hi" unless $x }
EOF

raku "trail_mod_say_if" "big" << 'EOF'
my $x = 5;
{ say "big" if $x > 3 }
EOF

raku "trail_mod_say_with" "def" << 'EOF'
my $d = 42;
{ say "def" with $d }
EOF

raku "trail_mod_say_for" "$(printf '1\n2\n3')" << 'EOF'
{ say $_ for 1..3 }
EOF

raku "trail_mod_expr_if" "11" << 'EOF'
my $x = 10;
{ $x = $x + 1 if $x > 5 }
say $x;
EOF

raku "trail_mod_sub_body" "big" << 'EOF'
sub f($x) { say "big" if $x > 3 }
f(5);
EOF

raku "trail_mod_method_body" "hi" << 'EOF'
class C { method m($x) { say "hi" unless $x } }
C.new.m(0);
EOF

raku "loop_ctl_while_last_if" "$(printf '0\n1\n2')" << 'EOF'
my $i = 0;
while $i < 10 { last if $i >= 3; say $i; $i = $i + 1 }
EOF

raku "loop_ctl_while_next_if" "$(printf '1\n3\n4')" << 'EOF'
my $i = 0;
while $i < 4 { $i = $i + 1; next if $i == 2; say $i }
EOF

raku "loop_ctl_bare_loop_last" "$(printf '0\n1\n2')" << 'EOF'
my $i = 0;
loop { last if $i >= 3; say $i; $i = $i + 1 }
EOF

raku "loop_ctl_until_last" "$(printf '0\n1')" << 'EOF'
my $i = 0;
until $i >= 10 { last if $i >= 2; say $i; $i = $i + 1 }
EOF

raku "loop_ctl_nested_inner_only" "$(printf '0-0\n0-1\n1-0\n1-1')" << 'EOF'
my $o = 0;
while $o < 2 { my $i = 0; while $i < 5 { last if $i >= 2; say "$o-$i"; $i = $i + 1 }; $o = $o + 1 }
EOF

raku "loop_ctl_last_if_trailing" "$(printf '1\n2\n3')" << 'EOF'
my $i = 0;
loop { $i = $i + 1; say $i; last if $i >= 3 }
EOF

raku "srelop_lt" "y" << 'EOF'
sub main() { say "abc" lt "abd" ?? "y" !! "n"; }
EOF

raku "srelop_gt" "y" << 'EOF'
sub main() { say "b" gt "a" ?? "y" !! "n"; }
EOF

raku "srelop_le_eq" "y" << 'EOF'
sub main() { say "abc" le "abc" ?? "y" !! "n"; }
EOF

raku "srelop_ge" "y" << 'EOF'
sub main() { say "b" ge "a" ?? "y" !! "n"; }
EOF

raku "srelop_lt_lexicographic" "y" << 'EOF'
sub main() { say "10" lt "9" ?? "y" !! "n"; }
EOF

raku "srelop_gt_var" "y" << 'EOF'
my $a = "zebra"; my $b = "apple"; say $a gt $b ?? "y" !! "n";
EOF

raku "loop_cstyle_basic" "$(printf '0\n1\n2')" << 'EOF'
loop (my $i = 0; $i < 3; $i++) { say $i; }
EOF

raku "loop_cstyle_sum" "10" << 'EOF'
my $s = 0; loop (my $i = 1; $i <= 4; $i++) { $s = $s + $i; } say $s;
EOF

raku "loop_cstyle_last" "$(printf '0\n1\n2')" << 'EOF'
loop (my $i = 0; $i < 10; $i++) { last if $i == 3; say $i; }
EOF

raku "loop_cstyle_next_runs_incr" "$(printf '0\n2\n3')" << 'EOF'
loop (my $i = 0; $i < 4; $i++) { next if $i == 1; say $i; }
EOF

raku "loop_cstyle_decr" "$(printf '3\n2\n1')" << 'EOF'
loop (my $i = 3; $i > 0; $i--) { say $i; }
EOF

raku "methname_kw_sort" "99" << 'EOF'
class C { method sort() { return 99 } }
my $o = C.new;
say $o.sort;
EOF

raku "methname_kw_say_chain" "7" << 'EOF'
class C { method say() { return 7 } }
say C.new.say;
EOF

raku "methname_kw_map_arg" "5" << 'EOF'
class C { method map($x) { return $x + 1 } }
my $o = C.new;
say $o.map(4);
EOF

raku "methname_kw_take_noparen" "5" << 'EOF'
class C { method take() { return 5 } }
my $o = C.new;
say $o.take;
EOF

raku "methname_kw_return" "3" << 'EOF'
class C { method return() { return 3 } }
my $o = C.new;
say $o.return();
EOF

raku "methname_kw_grep_empty_paren" "8" << 'EOF'
class C { method grep() { return 8 } }
my $o = C.new;
say $o.grep();
EOF

raku "methname_kw_multi_sort" "40" << 'EOF'
class C { multi method sort(Int $x) { return $x * 10 } }
my $o = C.new;
say $o.sort(4);
EOF

raku "methname_normal_still_works" "42" << 'EOF'
class C { method go() { return 42 } }
say C.new.go;
EOF

raku "destr_two_bare" "$(printf '3\n7')" << 'EOF'
my ($a, $b) = 3, 7;
say $a;
say $b;
EOF

raku "destr_three_last" "3" << 'EOF'
my ($a, $b, $c) = 1, 2, 3;
say $c;
EOF

raku "destr_paren_list_sum" "30" << 'EOF'
my ($a, $b) = (10, 20);
say $a + $b;
EOF

raku "destr_strings" "bar" << 'EOF'
my ($x, $y) = "foo", "bar";
say $y;
EOF

raku "destr_fewer_rhs" "$(printf '1\n2')" << 'EOF'
my ($a, $b, $c) = 1, 2;
say $a;
say $b;
EOF

raku "destr_extra_rhs_dropped" "$(printf '1\n2')" << 'EOF'
my ($a, $b) = 1, 2, 3;
say $a;
say $b;
EOF

raku "repeat_while" "$(printf '1\n2\n3')" << 'EOF'
my $i = 0;
repeat {
    $i = $i + 1;
    say $i;
} while $i < 3;
EOF

raku "repeat_until" "$(printf '11\n12')" << 'EOF'
my $j = 10;
repeat {
    $j = $j + 1;
    say $j;
} until $j >= 12;
EOF

raku "repeat_while_runs_once" "5" << 'EOF'
my $k = 5;
repeat {
    say $k;
} while $k > 100;
EOF

raku "repeat_until_runs_once" "7" << 'EOF'
my $n = 7;
repeat {
    say $n;
} until $n > 0;
EOF

raku "repeat_next" "$(printf '1\n3\n4')" << 'EOF'
my $i = 0;
repeat {
    $i = $i + 1;
    next if $i == 2;
    say $i;
} while $i < 4;
EOF

raku "repeat_last" "$(printf '1\n2')" << 'EOF'
my $i = 0;
repeat {
    $i = $i + 1;
    last if $i == 3;
    say $i;
} while $i < 10;
EOF

raku "arr_reverse" "2 1 3" << 'EOF'
my @a = 3, 1, 2;
say @a.reverse;
EOF

raku "arr_sort_num" "1 2 3" << 'EOF'
my @a = 3, 1, 2;
say @a.sort;
EOF

raku "arr_sort_str" "apple banana cherry" << 'EOF'
my @s = "banana", "apple", "cherry";
say @s.sort;
EOF

raku "arr_unique" "3 1 2" << 'EOF'
my @b = 3, 1, 2, 1;
say @b.unique;
EOF

raku "arr_method_obj_wins" "$(printf 'custom-sort\ncustom-rev')" << 'EOF'
class Widget {
    method sort() { return "custom-sort"; }
    method reverse() { return "custom-rev"; }
}
my $w = Widget.new;
say $w.sort;
say $w.reverse;
EOF

raku "arr_min_num" "1" << 'EOF'
my @a = 3, 1, 2;
say @a.min;
EOF

raku "arr_max_num" "3" << 'EOF'
my @a = 3, 1, 2;
say @a.max;
EOF

raku "arr_first" "3" << 'EOF'
my @a = 3, 1, 2;
say @a.first;
EOF

raku "arr_min_str" "apple" << 'EOF'
my @s = "cherry", "apple", "banana";
say @s.min;
EOF

raku "arr_sort_chain" "1" << 'EOF'
my @a = 3, 1, 2;
say @a.sort.first;
EOF

raku "hash_keys" "a,b,c" << 'EOF'
my %h = a => 1, b => 2, c => 3;
say %h.keys.sort.join(",");
EOF

raku "hash_values" "1,2,3" << 'EOF'
my %h = a => 1, b => 2, c => 3;
say %h.values.sort.join(",");
EOF

raku "hash_pairs" "a:1 b:2 c:3" << 'EOF'
my %h = a => 1, b => 2, c => 3;
say %h.pairs.sort.join(" ");
EOF

raku "hash_kv_elems" "6" << 'EOF'
my %h = a => 1, b => 2, c => 3;
say %h.kv.elems;
EOF

raku "hash_keys_elems" "3" << 'EOF'
my %h = one => 10, two => 20, three => 30;
say %h.keys.elems;
EOF

raku "hash_method_obj_wins" "custom-keys" << 'EOF'
class Cfg { method keys() { return "custom-keys"; } }
my $c = Cfg.new;
say $c.keys;
EOF

raku "arr_keys_idx" "0,1,2" << 'EOF'
my @a = "p", "q", "r";
say @a.keys.join(",");
EOF

raku "arr_values_elems" "p,q,r" << 'EOF'
my @a = "p", "q", "r";
say @a.values.join(",");
EOF

raku "arr_values_sum" "60" << 'EOF'
my @n = 10, 20, 30;
say @n.values.sum;
EOF

raku "arr_keys_chain" "3" << 'EOF'
my @a = 5, 6, 7;
say @a.keys.elems;
EOF

raku "preinc_expr_new" "6" << 'EOF'
my $i = 5; my $j = ++$i; say $j;
EOF

raku "preinc_expr_var" "6" << 'EOF'
my $i = 5; my $j = ++$i; say $i;
EOF

raku "postinc_expr_old" "5" << 'EOF'
my $i = 5; my $j = $i++; say $j;
EOF

raku "postinc_expr_var" "6" << 'EOF'
my $i = 5; my $j = $i++; say $i;
EOF

raku "predec_expr_new" "4" << 'EOF'
my $i = 5; my $j = --$i; say $j;
EOF

raku "postdec_expr_old" "5" << 'EOF'
my $i = 5; say $i--;
EOF

raku "incdec_stmt_still" "5" << 'EOF'
my $i = 3; $i++; $i++; say $i;
EOF

raku "postinc_say_arg" "7" << 'EOF'
my $i = 7; say $i++;
EOF

raku "sprintf_zeropad" "007" << 'EOF'
say sprintf("%03d", 7);
EOF

raku "sprintf_leftjust" "42   |" << 'EOF'
say sprintf("%-5d|", 42);
EOF

raku "sprintf_hex" "ff FF" << 'EOF'
say sprintf("%x %X", 255, 255);
EOF

raku "sprintf_binary" "1010" << 'EOF'
say sprintf("%b", 10);
EOF

raku "sprintf_binary_pad" "00000101" << 'EOF'
say sprintf("%08b", 5);
EOF

raku "sprintf_float_prec" "3.14" << 'EOF'
say sprintf("%.2f", 3.14159);
EOF

raku "sprintf_str_int" "x=5" << 'EOF'
say sprintf("%s=%d", "x", 5);
EOF

raku "sprintf_literal_pct" "100%" << 'EOF'
say sprintf("100%%");
EOF

raku "printf_basic" "1-2" << 'EOF'
printf("%d-%d\n", 1, 2);
EOF

raku "fmt_method_int" "007" << 'EOF'
my $x = 7; say $x.fmt("%03d");
EOF

raku "fmt_method_hex" "ff" << 'EOF'
say 255.fmt("%x");
EOF

raku "fmt_method_str" "[   hi]" << 'EOF'
my $s = "hi"; say $s.fmt("[%5s]");
EOF

raku "starts_with_true" "1" << 'EOF'
say "hello".starts-with("he");
EOF

raku "starts_with_false" "0" << 'EOF'
say "hello".starts-with("xy");
EOF

raku "ends_with_true" "1" << 'EOF'
say "hello".ends-with("lo");
EOF

raku "ends_with_false" "0" << 'EOF'
say "hello".ends-with("xy");
EOF

raku "subst_first" "heLlo" << 'EOF'
say "hello".subst("l","L");
EOF

raku "subst_nomatch" "hello" << 'EOF'
say "hello".subst("xy","Z");
EOF

raku "subst_grow" "bbaa" << 'EOF'
say "aaa".subst("a","bb");
EOF

raku "divis_true" "yes" << 'EOF'
if 6 %% 3 { say "yes" } else { say "no" }
EOF

raku "divis_false" "no" << 'EOF'
if 7 %% 3 { say "yes" } else { say "no" }
EOF

raku "divis_var" "even" << 'EOF'
my $n = 8;
if $n %% 2 { say "even" } else { say "odd" }
EOF

raku "divis_var_odd" "odd" << 'EOF'
my $n = 7;
if $n %% 2 { say "even" } else { say "odd" }
EOF

raku "divis_zero_dividend" "yes" << 'EOF'
if 0 %% 5 { say "yes" } else { say "no" }
EOF

raku "divis_unless" "notdiv" << 'EOF'
unless 10 %% 3 { say "notdiv" }
EOF

raku "divis_prec_over_add" "yes" << 'EOF'
if 1 + 2 %% 3 { say "yes" } else { say "no" }
EOF

raku "relop_val_eq_true" "1" << 'EOF'
say (6 == 6);
EOF

raku "relop_val_gt_false" "0" << 'EOF'
say (6 > 7);
EOF

raku "relop_val_ne_true" "1" << 'EOF'
say (3 != 4);
EOF

raku "relop_val_lt_var" "1" << 'EOF'
my $x = 4; my $y = 9;
say ($x < $y);
EOF

raku "relop_val_le_boundary" "1" << 'EOF'
say (9 <= 9);
EOF

raku "relop_val_cond_still_branches" "$(printf 'ct\nlt')" << 'EOF'
my $x = 4; my $y = 9;
if ($x < $y) { say "ct" } else { say "cf" }
if (4 < 9) { say "lt" } else { say "lf" }
EOF

raku "divis_val_true" "1" << 'EOF'
say 6 %% 3;
EOF

raku "divis_val_false" "0" << 'EOF'
say 7 %% 3;
EOF

raku "divis_val_zero_dividend" "1" << 'EOF'
say 0 %% 5;
EOF

raku "chr_fn_upper" "A" << 'EOF'
say chr(65);
EOF

raku "chr_fn_lower" "a" << 'EOF'
say chr(97);
EOF

raku "ord_fn" "65" << 'EOF'
say ord("A");
EOF

raku "chr_method" "A" << 'EOF'
say 65.chr;
EOF

raku "ord_method" "90" << 'EOF'
say "Z".ord;
EOF

raku "chr_var" "B" << 'EOF'
my $n = 66;
say chr($n);
EOF

raku "ord_var_method" "122" << 'EOF'
my $c = "z";
say $c.ord;
EOF

raku "chr_ord_roundtrip" "Q" << 'EOF'
say chr(ord("Q"));
EOF

raku "chr_concat" "Hi" << 'EOF'
say chr(72) ~ chr(105);
EOF

raku "chr_loop" "ABCDE" << 'EOF'
for 65..69 -> $i { print chr($i) }
say "";
EOF

# --- RAKU-100: reduction metaoperator [OP] (Rakudo metaops.rakumod METAOP_REDUCE_LEFT, \op branch:
# empty->op.() identity, one->that element unchanged, N->strict left fold). Operand is an explicit
# comma-list or array var (value-position range materialization is a separate pre-existing gap — see
# FINDING-2026-07-23-CLAUDE-RK-REDUCE-METAOP-LEFT-FOLD.md; a range smoke is intentionally excluded). ---
raku "reduce_add_list" "10" << 'EOF'
say [+] (1, 2, 3, 4);
EOF

raku "reduce_add_array" "10" << 'EOF'
my @a = 1, 2, 3, 4;
say [+] @a;
EOF

raku "reduce_mul_list" "24" << 'EOF'
say [*] (1, 2, 3, 4);
EOF

raku "reduce_cat_str" "abc" << 'EOF'
my @w = "a", "b", "c";
say [~] @w;
EOF

raku "reduce_cat_nums" "1234" << 'EOF'
say [~] (1, 2, 3, 4);
EOF

raku "reduce_sub_leftfold" "-8" << 'EOF'
say [-] (1, 2, 3, 4);
EOF

raku "reduce_max_list" "4" << 'EOF'
say [max] (3, 1, 4, 2);
EOF

raku "reduce_min_list" "1" << 'EOF'
say [min] (3, 1, 4, 2);
EOF

raku "reduce_add_empty_identity" "0" << 'EOF'
say [+] ();
EOF

raku "reduce_mul_empty_identity" "1" << 'EOF'
say [*] ();
EOF

raku "reduce_add_single" "5" << 'EOF'
say [+] 5;
EOF

raku "reduce_mul_single" "7" << 'EOF'
say [*] 7;
EOF

raku "reduce_add_real_promote" "6.5" << 'EOF'
my @m = 1.5, 2.5, 2.5;
say [+] @m;
EOF

raku "reduce_compose_add" "30" << 'EOF'
my @a = 1, 2, 3, 4;
my @b = 5, 6, 9;
say ([+] @a) + ([+] @b);
EOF

raku "range_arr_elems" "5" << 'EOF'
my @a = 1..5;
say @a.elems;
EOF

raku "range_arr_sum" "15" << 'EOF'
my @a = 1..5;
say @a.sum;
EOF

raku "range_arr_min" "1" << 'EOF'
my @a = 1..5;
say @a.min;
EOF

raku "range_arr_max" "5" << 'EOF'
my @a = 1..5;
say @a.max;
EOF

raku "range_arr_reduce_add" "15" << 'EOF'
my @a = 1..5;
say [+] @a;
EOF

raku "range_arr_reduce_mul" "120" << 'EOF'
my @a = 1..5;
say [*] @a;
EOF

raku "range_arr_join" "1,2,3,4" << 'EOF'
my @a = 1..4;
say @a.join(",");
EOF

raku "range_arr_single" "1" << 'EOF'
my @a = 3..3;
say @a.elems;
EOF

raku "range_arr_empty" "0" << 'EOF'
my @a = 5..1;
say @a.elems;
EOF

raku "range_arr_var_bounds" "20" << 'EOF'
my $lo = 2;
my $hi = 6;
my @a = $lo..$hi;
say @a.sum;
EOF

raku "range_arr_exclusive" "3" << 'EOF'
my @e = 1..^4;
say @e.elems;
EOF

raku "range_arr_typed" "10" << 'EOF'
my Int @a = 1..4;
say @a.sum;
EOF

raku "range_for_generator_unaffected" "6" << 'EOF'
my $t = 0;
for 1..3 -> $x { $t = $t + $x; }
say $t;
EOF

raku "slice_range_mid" "2,3,4" << 'EOF'
my @a = 1,2,3,4,5;
say @a[1..3].join(",");
EOF

raku "slice_range_head" "1,2,3" << 'EOF'
my @a = 1,2,3,4,5;
say @a[0..2].join(",");
EOF

raku "slice_range_sum" "12" << 'EOF'
my @a = 1,2,3,4,5;
say @a[2..4].sum;
EOF

raku "slice_range_single" "3" << 'EOF'
my @a = 1,2,3,4,5;
say @a[2..2].join(",");
EOF

raku "slice_range_exclusive" "2,3,4" << 'EOF'
my @a = 1,2,3,4,5;
say @a[1..^4].join(",");
EOF

raku "slice_range_var_bounds" "2,3,4" << 'EOF'
my @a = 1,2,3,4,5;
my $lo = 1;
my $hi = 3;
say @a[$lo..$hi].join(",");
EOF

raku "slice_of_materialized_range" "12,13,14,15" << 'EOF'
my @r = 10..20;
say @r[2..5].join(",");
EOF

raku "slice_scalar_index_unaffected" "3" << 'EOF'
my @a = 1,2,3,4,5;
say @a[2];
EOF

raku "whatever_last" "40" << 'EOF'
my @a = 10,20,30,40;
say @a[*-1];
EOF

raku "whatever_second_last" "30" << 'EOF'
my @a = 10,20,30,40;
say @a[*-2];
EOF

raku "whatever_first_via_end" "10" << 'EOF'
my @a = 10,20,30,40;
say @a[*-4];
EOF

raku "whatever_string_array" "z" << 'EOF'
my @s = "x","y","z";
say @s[*-1];
EOF

raku "whatever_on_range_array" "5" << 'EOF'
my @r = 1..5;
say @r[*-1];
EOF

raku "whatever_var_offset" "30" << 'EOF'
my @a = 10,20,30,40;
my $n = 2;
say @a[*-$n];
EOF

raku "whatever_in_arith" "50" << 'EOF'
my @a = 10,20,30;
say @a[*-1] + @a[*-2];
EOF

raku "whatever_in_condition" "big" << 'EOF'
my @a = 10,20,30;
if @a[*-1] > 25 { say "big" }
EOF

raku "whatever_expr_offset" "20" << 'EOF'
my @a = 10,20,30,40;
say @a[*-(1+2)];
EOF

raku "whatever_scalar_index_unaffected" "20" << 'EOF'
my @a = 10,20,30,40;
say @a[1];
EOF

raku "pick_three" "10,30,50" << 'EOF'
my @a = 10,20,30,40,50;
say @a[0,2,4].join(",");
EOF

raku "pick_two" "20,40" << 'EOF'
my @a = 10,20,30,40,50;
say @a[1,3].join(",");
EOF

raku "pick_order_preserved" "30,10" << 'EOF'
my @a = 10,20,30,40,50;
say @a[2,0].join(",");
EOF

raku "pick_repeats" "10,10,10" << 'EOF'
my @a = 10,20,30;
say @a[0,0,0].join(",");
EOF

raku "pick_strings" "d-b" << 'EOF'
my @s = "a","b","c","d";
say @s[3,1].join("-");
EOF

raku "pick_elems" "2" << 'EOF'
my @a = 10,20,30,40,50;
say @a[0,2].elems;
EOF

raku "pick_var_index" "20,40" << 'EOF'
my @a = 10,20,30,40,50;
my $i = 1;
say @a[$i,3].join(",");
EOF

raku "pick_range_slice_unaffected" "20,30" << 'EOF'
my @a = 10,20,30,40,50;
say @a[1..2].join(",");
EOF

raku "pick_on_range_array" "1,3,5" << 'EOF'
my @r = 1..5;
say @r[0,2,4].join(",");
EOF

raku "arr_reassign_list" "4,5,6" << 'EOF'
my @a = 1,2,3;
@a = 4,5,6;
say @a.join(",");
EOF

raku "arr_reassign_single" "1" << 'EOF'
my @a = 1,2,3;
@a = 9;
say @a.elems;
EOF

raku "arr_reassign_paren_list" "7,8,9" << 'EOF'
my @a = 1,2,3;
@a = (7,8,9);
say @a.join(",");
EOF

raku "arr_reassign_range" "1,2,3,4" << 'EOF'
my @a = 9;
@a = 1..4;
say @a.join(",");
EOF

raku "arr_reassign_xx" "5,5,5" << 'EOF'
my @a = 1;
@a = 5 xx 3;
say @a.join(",");
EOF

raku "arr_reassign_paren_expr_is_one_elem" "1" << 'EOF'
my @b = 1,2,3;
@b = (1+2);
say @b.elems;
EOF

raku "arr_reassign_scalar_unaffected" "2" << 'EOF'
my $s = 1;
$s = 2;
say $s;
EOF

raku "trailing_comma_decl" "1,2,3" << 'EOF'
my @a = 1,2,3,;
say @a.join(",");
EOF

raku "trailing_comma_no_extra_elem" "3" << 'EOF'
my @a = 1,2,3,;
say @a.elems;
EOF

raku "trailing_comma_paren" "4,5,6" << 'EOF'
my @b = (4,5,6,);
say @b.join(",");
EOF

raku "trailing_comma_call_args" "7" << 'EOF'
sub f($x,$y) { return $x+$y }
say f(3,4,);
EOF

raku "trailing_comma_subscript" "1,3" << 'EOF'
my @c = 1,2,3;
say @c[0,2,].join(",");
EOF

raku "trailing_comma_reassign" "8,9" << 'EOF'
my @a = 1;
@a = 8,9,;
say @a.join(",");
EOF

raku "oob_read_no_unwind" "$(printf 'before\n\nafter')" << 'EOF'
my @a = (10,20,30);
say "before";
say @a[3];
say "after";
EOF

raku "oob_read_assign_continues" "after" << 'EOF'
my @a = (10,20,30);
my $x = @a[7];
say "after";
EOF

raku "oob_read_far_past_end" "$(printf 'A\n\nB')" << 'EOF'
my @a = (1,2,3);
say "A";
say @a[99];
say "B";
EOF

raku "oob_inrange_still_correct" "$(printf '10\n30\n25')" << 'EOF'
my @a = (10,20,30);
say @a[0];
say @a[2];
say @a[1] + 5;
EOF

raku "oob_read_in_loop_continues" "$(printf '1\n2\n3\n\n\ndone')" << 'EOF'
my @a = (1,2,3);
for 0..4 -> $i {
    say @a[$i];
}
say "done";
EOF

raku "oob_star_end_relative_unaffected" "$(printf '30\n20')" << 'EOF'
my @a = (10,20,30);
say @a[*-1];
say @a[*-2];
EOF

raku "oob_destructure_surplus_still_nil" "$(printf '3\n7\nend')" << 'EOF'
my ($a, $b, $c) = 3, 7;
say $a;
say $b;
say "end";
EOF

raku "arr_all_slice_join" "10,20,30" << 'EOF'
my @a = (10,20,30);
say @a[*].join(",");
EOF

raku "arr_all_slice_elems" "3" << 'EOF'
my @a = (10,20,30);
say @a[*].elems;
EOF

raku "arr_all_slice_empty" "0" << 'EOF'
my @e = ();
say @e[*].elems;
EOF

raku "arr_all_slice_single" "7" << 'EOF'
my @a = (7);
say @a[*].join(",");
EOF

raku "arr_all_slice_reduce_metaop_intact" "$(printf '24\n10')" << 'EOF'
my @n = (1,2,3,4);
say [*] @n;
say [+] @n;
EOF

raku "arr_all_slice_star_minus_intact" "$(printf '30\n20')" << 'EOF'
my @a = (10,20,30);
say @a[*-1];
say @a[*-2];
EOF

raku "arr_all_slice_sum" "60" << 'EOF'
my @a = (10,20,30);
say @a[*].sum;
EOF

raku "param_default_omitted" "5" << 'EOF'
sub f($x = 5) { return $x }
say f();
EOF

raku "param_default_supplied" "9" << 'EOF'
sub f($x = 5) { return $x }
say f(9);
EOF

raku "param_default_zero_kept" "0" << 'EOF'
sub f($x = 5) { return $x }
say f(0);
EOF

raku "param_default_second_only" "$(printf '3\n11')" << 'EOF'
sub g($a, $b = 2) { return $a + $b }
say g(1);
say g(1,10);
EOF

raku "param_default_depends_on_earlier" "9" << 'EOF'
sub h($a, $b = $a * 2) { return $a + $b }
say h(3);
EOF

raku "param_default_string" "hi" << 'EOF'
sub s2($a = "hi") { return $a }
say s2();
EOF

raku "param_default_method" "$(printf '42\n8')" << 'EOF'
class C { method m($v = 42) { return $v } }
my $o = C.new;
say $o.m();
say $o.m(8);
EOF

raku "param_default_plain_unaffected" "$(printf '6\n4')" << 'EOF'
sub plain($z) { return $z }
say plain(6);
sub t(Int $n) { return $n }
say t(4);
EOF

raku "div_inexact_rational" "$(printf '3.5\n2.5\n-3.5')" << 'EOF'
say 7/2;
say 10/4;
say -7/2;
EOF

raku "div_exact_stays_integral" "$(printf '4\n2\n3\n5')" << 'EOF'
say 8/2;
say 6/3;
say 9/3;
say 10/2;
EOF

raku "div_roundtrip_multiply" "$(printf '7\n7')" << 'EOF'
my $h = 7/2;
say $h*2 == 7 ?? 7 !! 0;
say $h*2;
EOF

raku_dies "div_by_zero_dies" "before" << 'EOF'
say "before";
say 1/0;
say "after";
EOF

raku "div_var_operands" "$(printf '2.5\n4')" << 'EOF'
my $a = 5; my $b = 2;
say $a/$b;
my $c = 8; my $d = 2;
say $c/$d;
EOF

raku "div_in_expression" "$(printf '6.5\n1.5')" << 'EOF'
say 3 + 7/2;
say 7/2 - 2;
EOF

raku "real_whole_prints_bare" "$(printf '4\n7\n4\n-3')" << 'EOF'
say 4.0;
say (7/2)*2;
say 2.5+1.5;
say -3.0;
EOF

raku "real_fraction_preserved" "$(printf '3.5\n2.5\n-3.5')" << 'EOF'
say 3.5;
say 10/4;
say -7/2;
EOF

raku "real_num_precision_not_truncated" "1.4142135623730951" << 'EOF'
say sqrt(2);
EOF

raku "real_whole_print_no_nl" "4" << 'EOF'
print 4.0;
say "";
EOF

raku "named_arg_basic" "Hello Bob" << 'EOF'
sub greet(:$name) { say "Hello " ~ $name; }
greet(name => "Bob");
EOF

raku "named_arg_mixed_positional" "1
2" << 'EOF'
sub f($a, :$b) { say $a; say $b; }
f(1, b => 2);
EOF

raku "named_arg_two_in_order" "4
9" << 'EOF'
sub f(:$x, :$y) { say $x; say $y; }
f(x => 4, y => 9);
EOF

raku "named_arg_out_of_order" "4
9" << 'EOF'
sub f(:$x, :$y) { say $x; say $y; }
f(y => 9, x => 4);
EOF

raku "named_arg_absent_is_undefined" "0" << 'EOF'
sub f(:$x) { say $x.defined; }
f();
EOF

raku "named_arg_absent_with_default" "5" << 'EOF'
sub f(:$x = 5) { say $x; }
f();
EOF

raku "named_arg_key_x_lexer" "4" << 'EOF'
sub f(:$q, :$x) { say $x; }
f(q => 1, x => 4);
EOF

raku "named_arg_key_x_in_hash" "4" << 'EOF'
my %h = (y => 9, x => 4);
say %h{"x"};
EOF

raku "named_arg_key_x_in_new" "4" << 'EOF'
class C { has $.x; has $.y; }
my $c = C.new(y => 9, x => 4);
say $c.x;
EOF

raku "xrep_still_works_after_key_fix" "1xxx2" << 'EOF'
say 1 ~ "x" x 3 ~ 2;
EOF

raku "xxrep_still_works_after_key_fix" "x x x" << 'EOF'
my @a = "x" xx 3; say @a;
EOF

raku "xrep_scalar_assign_my" "ababab" << 'EOF'
my $s = "ab" x 3; say $s;
EOF

raku "xrep_scalar_assign_bare" "ababab" << 'EOF'
my $s; $s = "ab" x 3; say $s;
EOF

raku "xrep_scalar_assign_var_count" "ababab" << 'EOF'
my $n = 3; my $s = "ab" x $n; say $s;
EOF

raku "xrep_scalar_assign_zero" "[]" << 'EOF'
my $s = "ab" x 0; say "[" ~ $s ~ "]";
EOF

raku "xrep_value_position_still_works" "ababab" << 'EOF'
say "ab" x 3;
EOF

raku "slurpy_basic_rest" "1|2" << 'EOF'
sub f($a, *@r) { say $a ~ "|" ~ @r.elems; }
f(1,2,3);
EOF

raku "slurpy_rest_values" "2 3" << 'EOF'
sub f($a, *@r) { say @r; }
f(1,2,3);
EOF

raku "slurpy_only_param" "3" << 'EOF'
sub f(*@r) { say @r.elems; }
f(10,20,30);
EOF

raku "slurpy_empty_is_empty_array_not_any" "0" << 'EOF'
sub f($a, *@r) { say @r.elems; }
f(1);
EOF

raku "slurpy_no_args_at_all" "0" << 'EOF'
sub f(*@r) { say @r.elems; }
f();
EOF

raku "slurpy_flattens_array_arg" "3" << 'EOF'
sub f(*@r) { say @r.elems; }
my @a = (1,2,3);
f(@a);
EOF

raku "slurpy_flattens_two_arrays" "5" << 'EOF'
sub f($a, *@r) { say @r.elems; }
my @a = (1,2); my @b = (3,4,5);
f(0, @a, @b);
EOF

raku "slurpy_subscript" "3" << 'EOF'
sub f(*@r) { say @r[2]; }
f(1,2,3);
EOF

raku "slurpy_reduction" "10" << 'EOF'
sub f($a, *@r) { say [+] @r; }
f(0, 1,2,3,4);
EOF

raku "slurpy_iterates" "a b" << 'EOF'
sub f(*@r) { my $o = ""; for @r -> $x { $o = $o ~ $x ~ " "; } say $o.trim; }
f("a","b");
EOF

raku "slurpy_strings_flat" "x y z" << 'EOF'
sub f(*@r) { say @r; }
f("x","y","z");
EOF

raku "slurpy_multiply_unregressed" "42" << 'EOF'
my $x = 6; my $y = 7; say $x * $y;
EOF

raku "slurpy_multiply_array_elems_unregressed" "18" << 'EOF'
my $x = 6; my @a = (1,2,3); say $x * @a.elems;
EOF

raku "slurpy_array_literal_unregressed" "1 2 3" << 'EOF'
my @a = (1,2,3); say @a;
EOF

raku "slurpy_lol_basic_rest" "1|2" << 'EOF'
sub f($a, **@r) { say $a ~ "|" ~ @r.elems; }
f(1,2,3);
EOF

raku "slurpy_lol_only_param" "3" << 'EOF'
sub f(**@r) { say @r.elems; }
f(10,20,30);
EOF

raku "slurpy_lol_empty_is_empty_array" "0" << 'EOF'
sub f($a, **@r) { say @r.elems; }
f(1);
EOF

raku "slurpy_lol_rest_values" "2 3" << 'EOF'
sub f($a, **@r) { say @r; }
f(1,2,3);
EOF

raku "slurpy_lol_subscript" "8|9" << 'EOF'
sub f($a, **@r) { say @r[0] ~ "|" ~ @r[1]; }
f(0,8,9);
EOF

raku "slurpy_lol_scalars_match_flat" "3|3" << 'EOF'
sub a($x, *@r)  { return @r.elems; }
sub b($x, **@r) { return @r.elems; }
say a(0,1,2,3) ~ "|" ~ b(0,1,2,3);
EOF

raku "slurpy_pos_flattens_iterable_unregressed" "3" << 'EOF'
my @x = 1, 2;
sub f($a, *@r) { say @r.elems; }
f(0, @x, 9);
EOF

raku "multi_slurpy_variadic_candidate" "V:2" << 'EOF'
multi sub f($a, *@r) { say "V:" ~ @r.elems; }
f(1,2,3);
EOF

raku "multi_slurpy_exact_beats_variadic" "S" << 'EOF'
multi sub f($a, *@r) { say "V:" ~ @r.elems; }
multi sub f($a) { say "S"; }
f(1);
EOF

raku "multi_slurpy_both_candidates" "S|V:2" << 'EOF'
multi sub f($a, *@r) { return "V:" ~ @r.elems; }
multi sub f($a) { return "S"; }
say f(1) ~ "|" ~ f(1,2,3);
EOF

raku "multi_slurpy_lol_candidate" "L:3" << 'EOF'
multi sub f($a, **@r) { say "L:" ~ @r.elems; }
f(1,2,3,4);
EOF

raku "multi_typed_unaffected_by_slurpy" "int" << 'EOF'
multi sub f(Int $a) { say "int"; }
multi sub f(Str $a) { say "str"; }
f(7);
EOF

echo ""

raku "slurpy_named_basic" "1|2" << 'EOF'
sub f($a, *%h) { say $a ~ "|" ~ %h.elems; }
f(1, x => 2, y => 3);
EOF

raku "slurpy_named_empty_is_empty_hash" "5|0" << 'EOF'
sub f($a, *%h) { say $a ~ "|" ~ %h.elems; }
f(5);
EOF

raku "slurpy_named_only_param" "3" << 'EOF'
sub only(*%h) { say %h.elems; }
only(a => 1, b => 2, c => 3);
EOF

raku "slurpy_named_only_param_empty" "0" << 'EOF'
sub only(*%h) { say %h.elems; }
only();
EOF

raku "slurpy_named_value_lookup" "4" << 'EOF'
sub f(*%h) { say %h{'z'}; }
f(z => 4, w => 5);
EOF

raku "slurpy_named_declared_named_consumed" "9|2" << 'EOF'
sub mixed($p, :$n, *%rest) { say $n ~ "|" ~ %rest.elems; }
mixed(7, n => 9, z => 4, w => 5);
EOF

raku "slurpy_named_positional_survives" "7" << 'EOF'
sub mixed($p, :$n, *%rest) { say $p; }
mixed(7, n => 9, z => 4);
EOF

raku "slurpy_named_collector_name_is_not_a_param" "42" << 'EOF'
sub collides(*%h) { say %h{'h'}; }
collides(h => 42);
EOF

raku "slurpy_named_modulo_unregressed" "1" << 'EOF'
my $x = 10; my $y = 3;
say $x % $y;
EOF

raku "slurpy_named_multiply_hash_unregressed" "50" << 'EOF'
my $x = 10; my %h = (a => 5);
say $x * %h{'a'};
EOF

raku "slurpy_named_multiply_scalar_unregressed" "30" << 'EOF'
my $x = 10; my $y = 3;
say $x * $y;
EOF

raku "slurpy_named_pos_slurpy_unregressed" "1|2" << 'EOF'
sub f($a, *@r) { say $a ~ "|" ~ @r.elems; }
f(1,2,3);
EOF

echo "mode-3 (--run):      PASS=$P3 FAIL=$F3 REFUSED=$X3  / $N   (done bar: PASS or REFUSED, never silent FAIL)"
echo "mode-4 (--compile):  PASS=$P4 FAIL=$F4 REFUSED=$X4  / $N   (done bar: PASS or REFUSED, never silent FAIL)"
# COMPLETION BAR (interp deleted 2026-06-15 — two native modes only): ZERO silent m3/m4 FAIL — every native
# mode is either PASS or a LOUD [SMX] REFUSE (no abort, no miscompile, no oracle fallback). A rung is promoted
# only when BOTH m3 and m4 are accounted for together. Floors retained as a backstop ratchet.
[ "$F3" -eq 0 ] && [ "$F4" -eq 0 ] && [ "$P3" -ge "$MODE3_MIN" ] && [ "$P4" -ge "$MODE4_MIN" ]
