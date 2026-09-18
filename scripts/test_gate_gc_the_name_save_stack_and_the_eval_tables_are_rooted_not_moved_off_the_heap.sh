#!/usr/bin/env bash
# test_gate_gc_the_name_save_stack_and_the_eval_tables_are_rooted_not_moved_off_the_heap.sh -- THREE HOLDERS OF THE
# UNROOTED-STATIC-HOLDER CLASS ARE ROOTED, AND THE GATE REFUSES THE OTHER CURE (cfo 2026-09-17, under Lon's FACT RULE
# CEO-832 and the ceo's CEO-836 ordering: REMOVING malloc AND ROOTING THE HOLDERS ARE THE SAME JOB AND THE ROOTING
# COMES FIRST).
#
# THE CLASS: a C file-scope static holds a pointer into the COLLECTED heap, no root walk visits it, a compaction
# relocates or reclaims what it points at, and the static then reads or WRITES through the stale pointer.
#
# THE THREE HOLDERS, each measured on this witness and not inferred:
#   (1) g_name_save (rt.c) -- the SNOBOL4 name-save stack. rt_gc_ws_roots visited every entry's .old DESCR but NEVER
#       the ARRAY BLOCK ITSELF, nor .name, nor .cell. Visiting a descriptor inside a block does not mark the block:
#       the array was unmarked at every collection, reclaimed under a live call, and re-carved as a string. The
#       signature is unmistakable and is the reason this is not a theory -- rt_name_restore read a name pointer whose
#       VALUE was 0x54555054554f5f, which is the ASCII bytes of _OUTPUT: the slot that held a pointer now holds the
#       neighbouring string's characters.
#   (2) g_lbl_tab + its keys and (3) g_eval_cache + its keys (runtime_eval.c) -- the runtime label table and the EVAL
#       chain cache, both on the collected heap with no root; rt_label_set_fn then did strcmp on a moved name.
#
# ⛔ WHY ARM 3 EXISTS AND IS THE POINT OF THE GATE. These three were briefly "cured" by moving them OFF the collected
# heap to malloc/calloc/realloc/strdup (SCRIP f62a33aed). That passes every behavioural arm below and is FORBIDDEN:
# Lon's rule is that we have a collector and the collector is the allocator, and the ceo measured what a whole-tree
# sweep in that shape costs -- 8 of 15 collector gates red, because malloc'd memory never moves, so an unrooted
# holder of it is only ACCIDENTALLY safe. THE CURE FOR AN UNROOTED HOLDER IS A ROOT, NEVER A DIFFERENT ALLOCATOR.
# Arm 3 therefore requires the three allocation sites to still read rt_ws_alloc / rt_ws_realloc / rt_heap_strdup_c,
# so a future seat cannot make this gate green by taking the holders out of the collector's sight.
#
# ⛔ WHY THE STRESS BAND AND NOT TWO VALUES. The row's own DONE-WHEN samples SCRIP_GC_STRESS=30 and =200 only. On
# origin this witness is GREEN at 30 and at 200 and RED at 5 and 10 -- the crashing band is ragged, so two sampled
# points are a lottery (CEO-807). This gate walks a BAND and any red in it is a red.
# FAIL-ONCE MEASURED, 2026-09-17, not asserted: on origin (SCRIP bdc486824) m3 SIGSEGVs rc=139 at stress 5 and 10 and
# passes at 15..500; with the eval tables back on the collected heap and NO roots, m3 and m4 SIGSEGV at 30 and m4 at
# 200 as well; with all three holders rooted, all 16 behavioural arms rc=0 byte-identical.
"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
set -uo pipefail
G="$(basename "${BASH_SOURCE[0]}" .sh)"
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP_BIN:-$ROOT/scrip}"; [ -x "$SCRIP" ] || { echo "⛔ GATE REFUSE(2) [$G]: no scrip at $SCRIP"; exit 2; }
LIBDIR="$ROOT/out"; [ -f "$LIBDIR/libscrip_rt.so" ] || { echo "⛔ GATE REFUSE(2) [$G]: no libscrip_rt.so in $LIBDIR"; exit 2; }
SBL="${SBL_BIN:-/home/resources/x64/bin/sbl}"; [ -x "$SBL" ] || { echo "⛔ GATE REFUSE(2) [$G]: no sbl at $SBL -- the ref is CUT FROM THE ORACLE at run time, never typed"; exit 2; }
W="$ROOT/../corpus/tests/snobol4/code_call_runtime_define_pending_entry.sno"
[ -f "$W" ] || { echo "⛔ GATE REFUSE(2) [$G]: the witness $W is not there, so nothing was measured"; exit 2; }
T=$(mktemp -d) || exit 2; trap 'rm -rf "$T"' EXIT
RC=0
cp "$W" "$T/w.sno"
n=$(awk '/^void rt_gc_ws_roots\(void\)$/{f=1} f&&/&g_name_save\)/{a++} f&&/\.name\)/{b++} f&&/\.cell\)/{c++} f&&/^}/{exit} END{printf "%d %d %d", a+0, b+0, c+0}' "$ROOT/src/runtime/rt/rt.c")
if [ "$n" = "1 1 1" ]; then echo "  structural nsave PASS (rt_gc_ws_roots visits the ARRAY BLOCK &g_name_save and every entry's .name and .cell, not only .old -- visiting a descriptor inside a block does not mark the block)"
else echo "  structural nsave FAIL (rt_gc_ws_roots visits [&g_name_save .name .cell] = [$n], want [1 1 1]: the name-save stack is reclaimed under a live call and re-carved as a string)"; RC=1; fi
if grep -q '^void eval_gc_roots(void)$' "$ROOT/src/runtime/runtime_eval.c" && grep -q 'eval_gc_roots();' "$ROOT/src/runtime/rt/gc_heap.c"; then
    echo "  structural eval PASS (eval_gc_roots is defined in runtime_eval.c AND called from the collector's root phase -- a root walk nothing calls is not a root)"
else echo "  structural eval FAIL (eval_gc_roots is missing, or gc_heap.c never calls it: the label table and the eval cache have no root)"; RC=1; fi
bad=0
for fn in eval_cache_put rt_label_set_fn; do
    body=$(awk -v f="$fn" 'index($0, f"(")&&/^static void |^void /{d=1} d{print} d&&/^}/{exit}' "$ROOT/src/runtime/runtime_eval.c")
    printf '%s' "$body" | grep -qE '\b(malloc|calloc|realloc|strdup)[[:space:]]*\(' && { echo "  fact-rule FAIL ($fn allocates with a C allocator -- moving a holder off the collected heap is the SAME EVASION as pinning it, one indirection further out; the cure for an unrooted holder is a ROOT)"; bad=1; }
    printf '%s' "$body" | grep -qE 'rt_ws_alloc|rt_ws_realloc|rt_heap_strdup_c' || { echo "  fact-rule FAIL ($fn no longer allocates from the collected heap at all, so the roots below are guarding nothing)"; bad=1; }
done
body=$(awk '/^static void rt_name_save_grow\(void\)/{d=1} d{print} d&&/^}/{exit}' "$ROOT/src/runtime/rt/rt.c")
printf '%s' "$body" | grep -qE '\b(malloc|calloc|realloc)[[:space:]]*\(' && { echo "  fact-rule FAIL (rt_name_save_grow allocates with a C allocator)"; bad=1; }
printf '%s' "$body" | grep -q 'rt_ws_realloc' || { echo "  fact-rule FAIL (rt_name_save_grow no longer grows on the collected heap)"; bad=1; }
if [ "$bad" -eq 0 ]; then echo "  fact-rule PASS (all three holders still allocate from the COLLECTED heap -- this gate cannot be made green by taking them out of the collector's sight)"; else RC=1; fi
( cd "$T" && timeout 20s "$SBL" -bf w.sno </dev/null ) > "$T/w.ref" 2>&1 || { echo "⛔ GATE REFUSE(2) [$G]: the oracle refused its own witness -- no ref to grade against"; exit 2; }
[ -s "$T/w.ref" ] || { echo "⛔ GATE REFUSE(2) [$G]: the oracle produced an EMPTY ref"; exit 2; }
if "$SCRIP" --compile "$T/w.sno" -o "$T/w.s" </dev/null >/dev/null 2>&1 && gcc -m64 -no-pie -rdynamic "$T/w.s" -Wl,-rpath,"$LIBDIR" -L"$LIBDIR" -lscrip_rt -lm -lpthread -o "$T/w4" 2>"$T/ld.log"; then :; else echo "⛔ GATE REFUSE(2) [$G]: mode-4 compile or link failed, so every m4 arm measured nothing (see $T/ld.log)"; exit 2; fi
red=0; ran=0
for st in 0 5 10 15 20 30 40 200; do
    ( cd "$T" && SCRIP_GC_STRESS=$st timeout 20s "$SCRIP" w.sno </dev/null > m3.out 2>/dev/null ); r=$?; ran=$((ran+1))
    if [ "$r" -eq 0 ] && cmp -s "$T/m3.out" "$T/w.ref"; then :; else echo "  m3 stress=$st FAIL (rc=$r; 139 is the stale-holder SIGSEGV, 0-with-diff the silently wrong answer)"; red=$((red+1)); fi
    ( cd "$T" && SCRIP_GC_STRESS=$st timeout 20s ./w4 </dev/null > m4.out 2>/dev/null ); r=$?; ran=$((ran+1))
    if [ "$r" -eq 0 ] && cmp -s "$T/m4.out" "$T/w.ref"; then :; else echo "  m4 stress=$st FAIL (rc=$r)"; red=$((red+1)); fi
done
if [ "$red" -eq 0 ]; then echo "  band PASS (the DEXP witness is rc=0 and byte-identical to the oracle at every one of $ran runs across SCRIP_GC_STRESS 0 5 10 15 20 30 40 200 in BOTH modes -- a band, because origin is green at 30 and 200 and red at 5 and 10, so two sampled points are a lottery)"
else echo "  band FAIL ($red of $ran runs red across the stress band -- a holder of collected-heap memory is still unrooted)"; RC=1; fi
if [ "$RC" = 0 ]; then echo "✅ GATE PASS(0) [$G]: the name-save stack, the runtime label table and the eval chain cache are ROOTED while still living on the collected heap, and the DEXP witness survives the whole stress band in both modes (examined 4 arms, $ran runs)"
else echo "⛔ GATE FAIL(1) [$G]: an unrooted static holder of collected-heap memory is back, or a holder was moved off the heap instead of rooted (examined 4 arms)"; fi
echo "    tree: SCRIP=$(git -C "$ROOT" rev-parse --short HEAD 2>/dev/null)$(git -C "$ROOT" diff --quiet 2>/dev/null || echo -DIRTY)  measured $(date -u +%Y-%m-%dT%H:%MZ)"
exit $RC
