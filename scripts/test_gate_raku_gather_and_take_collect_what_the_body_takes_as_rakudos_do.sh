#!/usr/bin/env bash
# test_gate_raku_gather_and_take_collect_what_the_body_takes_as_rakudos_do.sh -- `gather` / `take` (31 Roast files' first refusal: "an excised node (rk_excise) has no native template")
# (row raku-every-suite-to-100-under-nonet-ceo-1266; Roast compile census 796/1381 on SCRIP 137f4d712; ceo CEO-1479).
# THE DESIGN. The parser already builds TT_GATHER with TT_SUSPEND for `take` (Icon's every/suspend shape) and the lowerer excised the gather. lower_raku.c rk_desugar_gather
# turns each gather into a SEQ_EXPR that declares a collector `@__gatherN`, runs the body with every `take X` of that gather (not of a nested sub) rewritten to
# `@__gatherN.push(X)`, and answers the collector as a List (__rk_arr_values); a take inside a block handed to map/grep/for-like synchronous builtins reaches the collector
# through the capture road (rk_cap_block), nested gathers each own theirs. EAGER: the body runs to completion when the gather is evaluated -- an infinite gather is not
# supported by this road (it is the lazy Seq row), a take reached through a call to another sub is not supported either (dynamic scope), both named here so the claim is exact.
# THE WITNESS, ref cut by Rakudo (/usr/bin/raku) at generation: statement gather, gather as an expression printed as a List, the statement-prefix `gather for`, a sub
# returning a gather, a take inside a map block that also writes a captured counter, nested gathers, a gather as the source of `for`, .sum and .join on a gather; both
# modes, and under SCRIP_GC_STRESS 1 3 5 at the shrunk window.
# EXIT: 0 every witness-mode pair equals Rakudo's output; 1 one differs; 2 REFUSED (stale binary, no gcc).
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
cd "$ROOT"
. "$HERE/lib_gate.sh"
GATE_NAME="raku_gather_and_take_collect_what_the_body_takes_as_rakudos_do"
gate_parse_args "$@"
gate_require_fresh "$ROOT" src "$ROOT/scrip" "$ROOT/out/libscrip_rt.so"
command -v gcc >/dev/null 2>&1 || { echo "⛔ REFUSE(rc=2) [$GATE_NAME]: gcc not found -- mode 4 cannot be linked, so half the population is unmeasured"; exit 2; }
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
cat > "$W/gather.raku" <<'EOF'
my @a = gather { take 1; take 2; for 3..4 { take $_ * 2 } };
say @a;
say gather { take 5; take 6 };
say (gather { take $_ for 1..3 }).elems;
my @b = gather for 1..3 { take $_ + 10 }
say @b;
sub evens(Int $n) { return gather { for 1..$n { take $_ if $_ %% 2 } } }
say evens(9);
sub squares(Int $n) { gather { my $k = 0; (1..$n).map: { take $_ * $_; $k++ }; take $k } }
say squares(4);
my @nested = gather { for 1..2 -> $i { take gather { take $i; take $i * 10 } } };
say @nested.elems;
say @nested[1];
for gather { take "x"; take "y" } -> $c { print "[$c]" }
print "\n";
say gather { take $_ for 1..5 }.sum;
my $n = 3;
say gather { for 1..$n { take $_ ** 2 } }.join(",");
EOF
cat > "$W/gather.ref" <<'EOF'
[1 2 6 8]
(5 6)
3
[11 12 13]
(2 4 6 8)
(1 4 9 16 4)
2
(2 20)
[x][y]
15
1,4,9
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
for w in gather; do for m in m3 m4; do ck "$w" "$m"; done; done
echo "arm 2: the witness under SCRIP_GC_STRESS at the shrunk window, both modes"
unset SCRIP_HEAP_MB; export SCRIP_HEAP_KB="${SCRIP_HEAP_KB:-128}" SCRIP_HEAP_MAX_MB="${SCRIP_HEAP_MAX_MB:-512}"
sf=0
for s in 1 3 5; do for m in m3 m4; do
    GATE_EXAMINED=$((GATE_EXAMINED + 1))
    if [ "$m" = m3 ]; then out="$(SCRIP_GC_STRESS=$s timeout 120 "$ROOT/scrip" --run "$W/gather.raku" 2>/dev/null </dev/null)"; else out="$(SCRIP_GC_STRESS=$s timeout 120 "$W/gather.bin" 2>/dev/null </dev/null)"; fi
    if [ "$out" != "$(cat "$W/gather.ref")" ]; then printf '  FAIL gather %s stress=%s: wrong answer under the collector\n' "$m" "$s"; fails=$((fails + 1)); sf=1; fi
done; done
[ "$sf" -eq 0 ] && echo "  ok   gather x stress 1 3 5 x m3 m4"
gate_verdict "$fails" "witness-mode pair(s) wrong: a gather does not collect what its body takes as Rakudo's does"
