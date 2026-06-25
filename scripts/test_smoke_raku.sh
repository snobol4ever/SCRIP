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
#     (uncaught die aborts; caught needs try/CATCH which EXCISES) — so only the present case is asserted here. ---
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
