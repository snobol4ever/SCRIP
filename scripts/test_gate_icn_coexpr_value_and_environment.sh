#!/bin/bash
# A co-expression VALUE must be a descriptor, and a co-expression BODY must see the globals its creator
# saw. Row icon-a-co-expression-value-is-not-a-descriptor-and-its-body-cannot-see-globals.
#
# Measured 2026-09-06, before the cure:
#   type(create ...)                     SCRIP "string"  -- bb_create wrote the raw scrip_coctx_t* into
#                                        the DESCRIPTOR'S TAG WORD, so type() read the pointer's low byte
#                                        as a tag and every slot the value passed through handed the next
#                                        reader something it would mis-dispatch on.
#   global g inside a co-expression      SCRIP printed NOTHING where iconx prints 42 -- a global is read
#                                        as [r9 + off] and r9, the GVA base, was NOT among the six
#                                        registers the create packaged for the body's own thread.
#   c1 := create @c2 (c2 a global)       SIGSEGV, rc=139, in sem_post inside scrip_coswitch: the target
#                                        read out of that global was garbage, so the crash was the
#                                        SECOND symptom of the first two defects, not a third defect.
#
# ⛔ WHAT THIS GATE DOES NOT COVER, so a green is not read as co-expressions-complete: a captured LOCAL
# still reads empty inside the body (the frame snapshot is taken but not consulted), and EXHAUSTION does
# not hand control back to the activator -- the ping-pong witness runs all six activations and then loses
# main's continuation. Both are named on the row with their own witnesses.
# ⛔ REFUSES rc=2 unless all 10 runs (5 witnesses x 2 modes) are graded, and rc=2 if the oracle is absent.
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"; cd "$ROOT" || exit 2
ICONT=/home/resources/icon-master/bin/icont; ICONX=/home/resources/icon-master/bin/iconx
[ -x ./scrip ] && [ -f out/libscrip_rt.so ] || { echo "⛔ GATE REFUSES (rc=2): scrip/runtime not built at $ROOT"; exit 2; }
[ -x "$ICONT" ] && [ -x "$ICONX" ] || { echo "⛔ GATE REFUSES (rc=2): Icon oracle missing at $ICONT / $ICONX"; exit 2; }
d=$(mktemp -d) || exit 2; trap 'rm -rf "$d"' EXIT
printf 'global c2\nprocedure main()\n  c2 := create write("in c2");\n  c1 := create @c2;\n  @c1;\n  write("done");\nend\n' > "$d/chained.icn"
printf 'global g\nprocedure main()\n  g := 42;\n  c := create write(g);\n  @c;\nend\n'                                        > "$d/global_in_body.icn"
printf 'procedure main()\n  c := create write("x");\n  write(type(c));\nend\n'                                               > "$d/value_type.icn"
printf 'procedure main()\n  c := create write("in c");\n  @c;\n  write("done");\nend\n'                                      > "$d/one_hop_control.icn"
printf 'procedure main()\n  c := create (write("a") & write("b"));\n  @c;\n  @c;\n  write("done");\nend\n'                    > "$d/two_activations_control.icn"
graded=0; bad=0
for t in chained global_in_body value_type one_hop_control two_activations_control; do
    "$ICONT" -s -o "$d/$t.ora" "$d/$t.icn" >/dev/null 2>&1 || { echo "⛔ GATE REFUSES (rc=2): icont rejected $t -- cannot measure"; exit 2; }
    want=$(timeout 20 "$ICONX" "$d/$t.ora" </dev/null 2>/dev/null); wrc=$?
    [ "$wrc" != 124 ] || { echo "⛔ GATE REFUSES (rc=2): the ORACLE timed out on $t -- cannot measure"; exit 2; }
    o=$(timeout 20 ./scrip "$d/$t.icn" </dev/null 2>/dev/null); r=$?; graded=$((graded+1))
    if [ "$o" != "$want" ] || [ "$r" != "$wrc" ]; then
        echo "  ⛔ RED m3 $t: rc=$r out=[$(printf '%s' "$o" | tr '\n' '|' | cut -c1-50)] -- iconx rc=$wrc out=[$(printf '%s' "$want" | tr '\n' '|' | cut -c1-50)]"; bad=1
    fi
    if timeout 120 ./scrip --compile -o "$d/$t.s" "$d/$t.icn" </dev/null >/dev/null 2>&1 \
       && gcc -m64 -no-pie "$d/$t.s" -o "$d/$t.m4" -L./out -lscrip_rt -Wl,-rpath,"$ROOT/out" -lm -lpthread 2>/dev/null; then
        o=$(timeout 20 "$d/$t.m4" </dev/null 2>/dev/null); r=$?; graded=$((graded+1))
        if [ "$o" != "$want" ] || [ "$r" != "$wrc" ]; then
            echo "  ⛔ RED m4 $t: rc=$r out=[$(printf '%s' "$o" | tr '\n' '|' | cut -c1-50)] -- iconx rc=$wrc out=[$(printf '%s' "$want" | tr '\n' '|' | cut -c1-50)]"; bad=1
        fi
    else
        graded=$((graded+1)); echo "  ⛔ RED m4 $t: does not compile/link"; bad=1
    fi
done
[ "$graded" = 10 ] || { echo "⛔ GATE REFUSES (rc=2): graded $graded of the 10 required runs"; exit 2; }
[ "$bad" = 0 ] || { echo "⛔ GATE RED [icn_coexpr_value_and_environment]: a co-expression value or its body's globals are still lost"; exit 1; }
echo "✅ GATE OK [icn_coexpr_value_and_environment]: a co-expression is a typed value, its body sees globals, and activating one from inside another works -- BOTH modes"
