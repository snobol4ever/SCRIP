#!/usr/bin/env bash
# test_gate_raku_a_block_passed_to_a_synchronous_builtin_reads_its_enclosing_subs_locals_through_a_reference.sh -- CLOSURE CAPTURE, THE NON-ESCAPING ROAD (M3 of row
# raku-every-suite-to-100-under-nonet-ceo-1266; the cto's binding design q-raku-closure-capture, 2026-10-03, with his three conditions of 2026-10-04).
# THE DEFECT: a block is hoisted to its own proc, so a variable of the enclosing sub it read or wrote was an unresolved name in a different graph: sub f(Int $n) { (1,2).map: { $_ + $n } }
# was refused at the driver guard (about 100 Roast files first-refuse on a captured ordinary name, and the benchmark witnesses rc-perfect-shuffle and divide-and-conquer).
# THE CURE, lower_raku.c rk_cap_find/rk_cap_block (a pass before the hoist): for a block that is a literal argument of a SYNCHRONOUS builtin -- the table rk_cap_synchronous in the source:
# map, grep, first, sort, reduce, min, max, classify, categorize, produce as methods or functions, and the Test functions lives-ok, dies-ok, throws-like, subtest, eval-lives-ok,
# eval-dies-ok -- the free variables (read or written, not declared in the block, declared as a parameter, a my variable or a loop variable of an enclosing sub or block; the parser marks
# declarations, rk_tree.c rk_mark_declared) become HIDDEN LEADING BY-REF PARAMETERS of the block proc, so the existing by-reference machinery (byref_mask, IR_DEREF) reads them and
# the body is unchanged; the block expression becomes a CLOSURE, __blk_close(block, refs...), whose refs are IR_VAR_REF cells into the ENCLOSING STACK SLOTS (a DT_N into a frame slot, which the
# collector ignores as a non-heap pointer: the lifetime rule applied, nothing boxed); a write to a captured scalar goes through __rk_byref_assign (rt_assign_var) and a push to a captured
# array through the same, a copy from one through __rk_deref. The closure value is a typed DT_A with the static marker rk_proto_closure in ARBLK.proto, as List and Pair are: no collector edit,
# no descr tag. ONE function, by_name_dispatch.c rk_call_block, unwraps a closure and prepends its refs to the staged arguments, and every block callback (map, grep, first, sort, reduce, smartmatch
# against a block, subtest, __blk_invoke) goes through it, so the no_c_to_bb count does not rise (the ratchet reads 0 rose) and the callbacks can move to the open road unchanged.
# ARM 2, THE ESCAPING ROAD (the cto's road b, 2026-10-04; row raku-every-suite-to-100-under-nonet-ceo-1266): a block that is assigned to a variable, returned, passed to a routine not in the table, or
# nested in an escaping block is NEVER closed over a stack slot. Each local it captures is classified in the owning sub (rk_cap_proc, rk_cap_boxes): captured by VALUE (a plain hidden leading
# parameter, the value read at the moment the closure is made) when nothing assigns it after its declaration, and BOXED when anything does -- an assignment, an element or hash store, a push or other
# mutating method, an array or hash (a container), or a declaration whose own initializer is the closure (my $B := { ... $B() }): the declaration builds a one-slot heap cell reached by a DT_N
# (__rk_box: array_new(0,0), a name over its element, which gc_visit_one's DT_N slen=1 case keeps alive and relocates), every read and write in the owner goes through the by-reference deref (the
# TT_VAR node carries slen bit 8, RK_VF_BOXED, so lower_rv emits IR_DEREF; stores become __rk_byref_assign), a parameter is boxed by a prologue statement, and each closure holds the same DT_N so
# the sub and every closure share one cell. THE FOUR DOORS the cto named are in the witness: assigned (f), returned (f2), unknown routine (f3), and start (arm 1, st); plus counter, adder, two
# closures sharing one box, an array and a hash box, a boxed parameter copy, a closure made inside an escaping block, and fifty closures alive at once; both modes, and under SCRIP_GC_STRESS 1 3 5
# at a 128 KB window. Before this landing arm 2 expected the doors REFUSED at the guard (they were); counter printed 1 1 1 where Rakudo prints 1 2 3.
# FAILED ONCE, measured on SCRIP 98fbc5031: arm 1 is refused at the guard at its first sub.
# EXIT: 0 arms 1 and 2 match Rakudo in both modes and under the collector; 1 otherwise; 2 REFUSED (stale binary, no gcc).
# Usage: bash scripts/test_gate_raku_a_block_passed_to_a_synchronous_builtin_reads_its_enclosing_subs_locals_through_a_reference.sh   (~3s)
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
cd "$ROOT"
. "$HERE/lib_gate.sh"
GATE_NAME="raku_a_block_passed_to_a_synchronous_builtin_reads_its_enclosing_subs_locals_through_a_reference"
gate_parse_args "$@"
gate_require_fresh "$ROOT" src "$ROOT/scrip" "$ROOT/out/libscrip_rt.so"
command -v gcc >/dev/null 2>&1 || { echo "⛔ REFUSE(rc=2) [$GATE_NAME]: gcc not found -- mode 4 cannot be linked, so half the population is unmeasured"; exit 2; }
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
cat > "$W/capture.raku" <<'EOF'
sub f(Int $n) { return (1, 2).map: { $_ + $n } }
say f(3);
sub g { my $n = 10; return (1, 2, 3).grep: { $_ > $n - 9 } }
say g();
sub h($n) { (1, 2, 3).map({ $_ * $n }).join(",") }
say h(2);
sub k(@deck) { my $mid = 1; flat map { @deck[$_, $_ + $mid] }, 0 .. 0 }
say k([5, 6, 7]);
sub acc { my $a = 0; (1, 2, 3).map({ $a += $_ }); $a }
say acc();
sub nested($m) { (1, 2).map({ my $x = $_; (3, 4).map({ $x * $_ + $m }) }) }
say nested(1);
sub cnt { my $c = 0; (1, 2, 3).grep({ $c++; True }); $c }
say cnt();
sub pusher { my @a; (1, 2, 3).map({ @a.push($_ * 2) }); @a.elems }
say pusher();
sub pusher2 { my @a = 1; (1, 2, 3).map({ @a.push($_) }); @a }
say pusher2();
sub hset { my %s; (1, 2, 3).map({ %s{$_} = 1 }); %s.elems }
say hset();
sub copy($a) { (1, 2).map({ my $y = $a; $y + $_ }) }
say copy(5);
sub strc { my $s = ""; (1, 2, 3).map({ $s ~= $_ }); $s }
say strc();
sub srt($k) { (3, 1, 2).sort({ ($^a - $k).abs <=> ($^b - $k).abs }) }
say srt(2);
sub fst($t) { (1, 2, 3, 4).first({ $_ > $t }) }
say fst(2);
sub red($z) { (1, 2, 3).reduce({ $^a + $^b + $z }) }
say red(10);
sub rec($d) { $d == 0 ?? 0 !! (1,).map({ $_ + rec($d - 1) })[0] }
say rec(3);
sub st($n) { my $p = start { $n + 1 }; await $p }
say st(3);
EOF
cat > "$W/capture.ref" <<'EOF'
(4 5)
(2 3)
2,4,6
(5 6)
6
((4 5) (7 9))
3
3
[1 1 2 3]
3
(6 7)
123
(2 3 1)
3
26
3
4
EOF
fails=0; GATE_EXAMINED=0
ck() {
    local w="$1" m="$2" out rc
    GATE_EXAMINED=$((GATE_EXAMINED + 1))
    if [ "$m" = m3 ]; then out="$(timeout 20 "$ROOT/scrip" --run "$W/$w.raku" 2>/dev/null </dev/null)"; rc=$?
    else
        if ! timeout 60 "$ROOT/scrip" --compile -o "$W/$w.s" "$W/$w.raku" </dev/null >"$W/$w.cerr" 2>&1 \
           || ! gcc -o "$W/$w.bin" "$W/$w.s" -L"$ROOT/out" -lscrip_rt -Wl,-rpath,"$ROOT/out" -lm -lpthread >>"$W/$w.cerr" 2>&1; then
            printf '  FAIL %-7s %s: did not build (%s)\n' "$w" "$m" "$(head -1 "$W/$w.cerr" | cut -c1-110)"; fails=$((fails + 1)); return; fi
        out="$(timeout 20 "$W/$w.bin" 2>/dev/null </dev/null)"; rc=$?
    fi
    if [ "$rc" -ge 124 ]; then printf '  FAIL %-7s %s: died rc=%s\n' "$w" "$m" "$rc"; fails=$((fails + 1)); return; fi
    if [ "$out" = "$(cat "$W/$w.ref")" ]; then printf '  ok   %-7s %s\n' "$w" "$m"
    else printf '  FAIL %-7s %s: got [%s] want [%s]\n' "$w" "$m" "$(printf '%s' "$out" | tr '\n' ' ' | cut -c1-110)" "$(tr '\n' ' ' < "$W/$w.ref" | cut -c1-110)"; fails=$((fails + 1)); fi
}
for w in capture; do for m in m3 m4; do ck "$w" "$m"; done; done
echo "arm 1b: the callback loops hold their source, result and accumulator in the collector's rooted hold stack: the witness under SCRIP_GC_STRESS at the shrunk window, both modes"
unset SCRIP_HEAP_MB; export SCRIP_HEAP_KB="${SCRIP_HEAP_KB:-128}" SCRIP_HEAP_MAX_MB="${SCRIP_HEAP_MAX_MB:-512}"
sf=0
for s in 1 3 5; do for m in m3 m4; do
    GATE_EXAMINED=$((GATE_EXAMINED + 1))
    if [ "$m" = m3 ]; then out="$(SCRIP_GC_STRESS=$s timeout 120 "$ROOT/scrip" --run "$W/capture.raku" 2>/dev/null </dev/null)"; else out="$(SCRIP_GC_STRESS=$s timeout 120 "$W/capture.bin" 2>/dev/null </dev/null)"; fi
    if [ "$out" != "$(cat "$W/capture.ref")" ]; then printf '  FAIL capture %s stress=%s: wrong answer under the collector\n' "$m" "$s"; fails=$((fails + 1)); sf=1; fi
done; done
[ "$sf" -eq 0 ] && echo "  ok   capture x stress 1 3 5 x m3 m4"
unset SCRIP_HEAP_KB SCRIP_HEAP_MAX_MB
echo "arm 2: a block that may escape (assigned, returned, passed to a routine outside the table, nested in an escaping block) closes over its enclosing sub's locals by VALUE when nothing assigns them after the declaration and over a heap BOX when something does, and outlives its frame"
cat > "$W/escape.raku" <<'EOF'
sub f($n) { my &g = { $n + 1 }; g() }
say f(3);
sub f2($n) { return { $n + 1 } }
say f2(3)();
sub callit(&c) { c() }
sub f3($n) { callit({ $n + 1 }) }
say f3(3);
sub counter() { my $n = 0; return sub { $n += 1; $n } }
my $c = counter(); say $c(); say $c(); say $c(); my $d = counter(); say $d(); say $c();
sub mk($k) { return -> $x { $x + $k } }
my $f = mk(10); my $g = mk(20); say $f(1); say $g(1);
sub two() { my $n = 0; return (sub { $n += 1 }, sub { $n }) }
my ($inc, $get) = two(); $inc(); $inc(); say $get();
sub arrc() { my @a; return { @a.push($_); @a.elems } }
my $p = arrc(); $p(5); say $p(6);
sub hashc() { my %h; return -> $k { %h{$k} = 1; %h.elems } }
my $q = hashc(); $q('a'); say $q('b');
sub mutparam($n) { my $v = $n; return sub { $v += 1; $v } }
my $m = mutparam(10); $m(); say $m();
sub nest($a) { my $t = 0; return -> $b { my $r = $a + $b; my $s = sub { $t += $r; $t }; $s } }
my $nn = nest(1)(2); $nn(); say $nn();
sub many() { my @fs; for 1..50 -> $i { @fs.push(mk($i)) }; @fs.map({ $_(1) }).sum }
say many();
EOF
cat > "$W/escape.ref" <<'EOF'
4
4
4
1
2
3
1
4
11
21
2
2
2
12
6
1325
EOF
for m in m3 m4; do ck escape "$m"; done
sf=0
export SCRIP_HEAP_KB="${SCRIP_HEAP_KB:-128}" SCRIP_HEAP_MAX_MB="${SCRIP_HEAP_MAX_MB:-512}"
for s in 1 3 5; do for m in m3 m4; do
    GATE_EXAMINED=$((GATE_EXAMINED + 1))
    if [ "$m" = m3 ]; then out="$(SCRIP_GC_STRESS=$s timeout 120 "$ROOT/scrip" --run "$W/escape.raku" 2>/dev/null </dev/null)"; else out="$(SCRIP_GC_STRESS=$s timeout 120 "$W/escape.bin" 2>/dev/null </dev/null)"; fi
    if [ "$out" != "$(cat "$W/escape.ref")" ]; then printf '  FAIL escape %s stress=%s: wrong answer under the collector\n' "$m" "$s"; fails=$((fails + 1)); sf=1; fi
done; done
[ "$sf" -eq 0 ] && echo "  ok   escape x stress 1 3 5 x m3 m4"
unset SCRIP_HEAP_KB SCRIP_HEAP_MAX_MB
gate_verdict "$fails" "witness-mode pair(s) wrong: a captured local not read or written through its reference, or an escaping block that lost or shared wrongly a captured variable"
