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
# ARM 2, THE PROOF THAT THE STACK ROAD IS NEVER TAKEN FOR A BLOCK THAT MAY ESCAPE: a block that is assigned to a variable, returned or passed to a routine not in the table, and
# that captures a local, is NOT closed over a stack slot. Until the box road (a heap cell reached by DT_N, the cto's road b) lands, each reads REFUSED at the driver guard (rc not 0 and
# "never assigned" on stderr), never a program that would follow a dangling frame slot; when the box road lands these three arms flip to expecting Rakudo's answer. THE FOURTH DOOR
# the cto named, start, is not a block here: start { ... } runs inline at the statement and await returns its value, so it is a plain witness in arm 1 (st), no closure and no frame slot involved.
# FAILED ONCE, measured on SCRIP 98fbc5031: arm 1 is refused at the guard at its first sub.
# EXIT: 0 arm 1 matches Rakudo in both modes and the three doors read refused; 1 otherwise; 2 REFUSED (stale binary, no gcc).
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
echo "arm 2: an escaping block that captures a local is refused, never closed over a stack slot"
cat > "$W/door_assigned.raku" <<'EOF'
sub f($n) { my &g = { $n + 1 }; g() }
say f(3);
EOF
cat > "$W/door_returned.raku" <<'EOF'
sub f($n) { return { $n + 1 } }
say f(3)();
EOF
cat > "$W/door_unknown.raku" <<'EOF'
sub callit(&c) { c() }
sub f($n) { callit({ $n + 1 }) }
say f(3);
EOF
for d in assigned returned unknown; do
    for m in m3 m4; do
        GATE_EXAMINED=$((GATE_EXAMINED + 1))
        if [ "$m" = m3 ]; then out="$(timeout 20 "$ROOT/scrip" --run "$W/door_$d.raku" 2>&1 </dev/null)"; rc=$?
        else out="$(timeout 20 "$ROOT/scrip" --compile -o "$W/door_$d.s" "$W/door_$d.raku" 2>&1 </dev/null)"; rc=$?; fi
        if [ "$rc" -ne 0 ] && printf '%s' "$out" | grep -q 'never assigned'; then printf '  ok   door %-9s %s refused at the guard\n' "$d" "$m"
        else printf '  FAIL door %-9s %s: rc=%s [%s] -- an escaping block must not take the stack road\n' "$d" "$m" "$rc" "$(printf '%s' "$out" | head -c 90 | tr '\n' ' ')"; fails=$((fails + 1)); fi
    done
done
gate_verdict "$fails" "witness-mode pair(s) wrong: a captured local not read or written through its reference, or an escaping block closed over a stack slot"
