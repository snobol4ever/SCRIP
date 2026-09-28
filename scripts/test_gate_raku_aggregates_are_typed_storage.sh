#!/usr/bin/env bash
# test_gate_raku_aggregates_are_typed_storage.sh -- a Raku Array, List or Hash is typed DESCR-slot storage, never
# a string whose elements are joined by a separator byte (row raku-arrays-lists-and-hashes-are-typed-storage-no-
# delimiter-joined-strings-lon-2026-09-28; ceo CEO-1349; RULES.md FACT RULE NO DELIMITER-JOINED AGGREGATES).
# Lon, in-chat to hq_pascal 2026-09-28, verbatim as relayed: "Get rid of ALL delimited based processing like the
# one I just discovered."
#
# ⛔ THE DEFECT, measured on SCRIP 1a8548771: an array is one string with its elements joined by SOH (0x01), so
#   - an ELEMENT THAT HOLDS THE SEPARATOR splits into two: my @a = "a\x01b", "c" pushed to 4 elements read 5,
#     and a hash key holding 0x01 was not found (Nil);
#   - a NESTED AGGREGATE flattens into its parent: my @n = [1, [2, 3]], 4 read 4 elements, @n[0] read 1 element,
#     and @n[0][1][1] read Nil.
# Both are silent wrong answers: the program exits 0 and prints plausible numbers.
#
# THE WITNESSES are the two the row names, each graded in m3 (--run) and m4 (--compile + link) against a ref cut
# by Rakudo 2026.05 (lib_oracle_flags.sh rakudo_bin; /usr/bin/raku 2022.12 prints the same). The refs are the
# oracle's, never SCRIP's output.
# FAILED ONCE: on SCRIP 1a8548771 both witnesses read wrong in m3 (sep: 3 2 2 5 1 3 3 Nil 11; nest: 4 1 Nil 0 1 7 1 0 0).
#
# EXIT: 0 both witnesses match in both modes; 1 a mismatch or a crash on a witness; 2 REFUSED (stale binary, no gcc).
# Usage: bash scripts/test_gate_raku_aggregates_are_typed_storage.sh   (~2s, no oracle at run time, no network)
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
cd "$ROOT"
. "$HERE/lib_gate.sh"
GATE_NAME="raku_aggregates_are_typed_storage"
gate_parse_args "$@"
gate_require_fresh "$ROOT" src "$ROOT/scrip" "$ROOT/out/libscrip_rt.so"
command -v gcc >/dev/null 2>&1 || { echo "⛔ REFUSE(rc=2) [$GATE_NAME]: gcc not found -- mode 4 cannot be linked, so half the population is unmeasured"; exit 2; }
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
cat > "$W/sep.raku" <<'EOF'
my @a = "a\x01b", "c", "d\x05e";
say @a.elems;
say @a[0].chars;
say @a[2].chars;
@a.push("f\x01g");
say @a.elems;
say @a[3].chars;
my %h = k => "v\x05w", "x\x01y" => 1;
say %h.elems;
say %h<k>.chars;
say %h{"x\x01y"};
say @a.join("|").chars;
EOF
printf '3\n2\n2\n4\n3\n2\n3\n1\n11\n' > "$W/sep.ref"
cat > "$W/nest.raku" <<'EOF'
my @n = [1, [2, 3]], 4;
say @n.elems;
say @n[0].elems;
say @n[0][1][1];
say @n[0][1].elems;
my %h = a => [1, 2, 3], b => { c => 7 };
say %h<a>.elems;
say %h<b><c>;
my @e = [], [];
say @e.elems;
say @e[1].elems;
@n[0][1].push(9);
say @n[0][1].elems;
EOF
printf '2\n2\n3\n2\n3\n7\n2\n0\n3\n' > "$W/nest.ref"
fails=0; GATE_EXAMINED=0
ck() {
    local w="$1" m="$2" out rc
    GATE_EXAMINED=$((GATE_EXAMINED + 1))
    if [ "$m" = m3 ]; then out="$(timeout 20 "$ROOT/scrip" --run "$W/$w.raku" 2>&1 </dev/null)"; rc=$?
    else
        if ! timeout 60 "$ROOT/scrip" --compile -o "$W/$w.s" "$W/$w.raku" </dev/null >"$W/$w.cerr" 2>&1 \
           || ! gcc -o "$W/$w.bin" "$W/$w.s" -L"$ROOT/out" -lscrip_rt -Wl,-rpath,"$ROOT/out" -lm -lpthread >>"$W/$w.cerr" 2>&1; then
            printf '  FAIL %-5s %s: did not build (%s)\n' "$w" "$m" "$(head -1 "$W/$w.cerr" | cut -c1-120)"; fails=$((fails + 1)); return; fi
        out="$(timeout 20 "$W/$w.bin" 2>&1 </dev/null)"; rc=$?
    fi
    if [ "$rc" -ge 124 ]; then printf '  FAIL %-5s %s: died rc=%s\n' "$w" "$m" "$rc"; fails=$((fails + 1)); return; fi
    if [ "$out" = "$(cat "$W/$w.ref")" ]; then printf '  ok   %-5s %s\n' "$w" "$m"
    else printf '  FAIL %-5s %s: got [%s] want [%s]\n' "$w" "$m" "$(printf '%s' "$out" | tr '\n' ' ' | cut -c1-120)" "$(tr '\n' ' ' < "$W/$w.ref")"; fails=$((fails + 1)); fi
}
for w in sep nest; do for m in m3 m4; do ck "$w" "$m"; done; done
gate_verdict "$fails" "witness-mode pair(s) wrong: an element holding a separator byte, or a nested aggregate"
