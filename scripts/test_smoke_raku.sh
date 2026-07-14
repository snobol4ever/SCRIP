#!/usr/bin/env bash
# test_smoke_raku.sh — per-frontend smoke for Raku, run in BOTH native execution modes.
#   mode 3 = --run     (in-process stackless native x86 BB blobs) — the primary mode.
#   mode 4 = --compile (standalone x86-64 asm -> as -> link libscrip_rt -> run).
# NOTE (2026-06-15): the IR-graph interpreter (mode 2 / --run) was DELETED — nothing walks an IR graph to
#    interpret it in any mode. The harness no longer invokes --run; m3 and m4 are the only modes. A program
#    either runs natively (PASS) or is cleanly declined with a loud [SMX] banner (DECLINED) — there is no oracle
#    fallback. DONE BAR: m3 AND m4 each PASS-or-DECLINED with ZERO silent FAIL (no abort, no miscompile).
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
    # yet) => counted DECLINED, NOT FAIL. The done bar (interp now deleted): m3 AND m4 each PASS-or-DECLINED —
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

raku "field_write_ro_dies" "" << 'EOF'
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
raku "priv_attr_external_dies" "" << 'EOF'
class Secret { has $!code; method reveal() { return $!code; } }
sub main() { my $s = Secret.new(code => 42); say($s.code()); }
EOF

raku "priv_attr_internal_ok" "42" << 'EOF'
class Secret { has $!code; method reveal() { return $!code; } }
sub main() { my $s = Secret.new(code => 42); say($s.reveal()); }
EOF

raku "priv_attr_inherited_dies" "" << 'EOF'
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
raku "priv_attr_external_noparen_dies" "" << 'EOF'
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
raku "priv_array_attr_external_dies" "" << 'EOF'
class Stack { has @!items; method size() { return 1; } }
sub main() { my $s = Stack.new(); say($s.items); }
EOF

raku "priv_hash_attr_external_dies" "" << 'EOF'
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
raku "role_conflict_unresolved" "" << 'EOF'
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
raku "required_unimplemented" "" << 'EOF'
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
#     (uncaught die aborts; caught needs try/CATCH which DECLINES) — so only the present case is asserted here. ---
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
raku "attr_required_absent" "" << 'EOF'
class Point { has $.x is required; has $.y; }
sub main() { my $p = Point.new(y => 9); say("unreached"); }
EOF
raku "attr_required_absent_inherited" "" << 'EOF'
class Base { has $.id is required; }
class Sub is Base { method who() { return $!id; } }
sub main() { my $s = Sub.new(); say($s.who()); }
EOF
# --- native die route: an uncaught die surfaces to stderr + exit 1; statements after it do NOT run, so only
#     the pre-die output reaches stdout (the death is on stderr, not asserted here). Both modes. ---
raku "die_uncaught_halts" "before" << 'EOF'
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

# --- ~~ smartmatch verdict: regex rides the C NFA matcher (re.c); m3/m4 cleanly DECLINE (regex is run-only here) ---
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
raku "param_colon_d_dies" "hi Tom" << 'EOF'
sub greet(Str:D $name) { say("hi " ~ $name); }
sub main() { greet("Tom"); my $u; greet($u); say("after"); }
EOF
raku "param_colon_u_dies" "ok" << 'EOF'
sub want(Str:U $x) { say("ok"); }
sub main() { my $u; want($u); want("v"); say("after"); }
EOF
raku "param_type_mismatch_dies" "int 5" << 'EOF'
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
raku "param_plain_int_dies" "7" << 'EOF'
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
raku "op_overload_relop_objs" "7" << 'EOF'
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
raku "op_overload_relop_int_unaffected" "$(printf '9\n9')" << 'EOF'
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
raku "try_die_in_handler_uncaught_halts" "pre" << 'EOF'
sub main() {
    say('pre');
    try { die('a'); } CATCH { die('fatal-in-handler'); }
    say('unreached');
}
EOF

echo ""
echo "mode-3 (--run):      PASS=$P3 FAIL=$F3 DECLINED=$X3  / $N   (done bar: PASS or DECLINED, never silent FAIL)"
echo "mode-4 (--compile):  PASS=$P4 FAIL=$F4 DECLINED=$X4  / $N   (done bar: PASS or DECLINED, never silent FAIL)"
# COMPLETION BAR (interp deleted 2026-06-15 — two native modes only): ZERO silent m3/m4 FAIL — every native
# mode is either PASS or a LOUD [SMX] DECLINE (no abort, no miscompile, no oracle fallback). A rung is promoted
# only when BOTH m3 and m4 are accounted for together. Floors retained as a backstop ratchet.
[ "$F3" -eq 0 ] && [ "$F4" -eq 0 ] && [ "$P3" -ge "$MODE3_MIN" ] && [ "$P4" -ge "$MODE4_MIN" ]
