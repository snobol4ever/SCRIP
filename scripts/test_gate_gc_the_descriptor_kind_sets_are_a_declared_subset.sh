#!/usr/bin/env bash
# test_gate_gc_the_descriptor_kind_sets_are_a_declared_subset.sh -- ONE FACT, ONE AUTHORITY, EVERY DIVERGENCE
# WRITTEN DOWN (cto 2026-09-20, built to the cfo's shape on their ask CFO-141; the defect found by hq_snobol4,
# verified A/B/A by the cto at CTO-101; the third spelling named by the cfo at CFO-146).
#
# THE FACT: which descriptor kinds carry a heap payload.  gc_heap.c spells it FIVE times -- gc_visit_one (the heap
# visitor, handed a descriptor that IS one), gc_cell_visit (the emitted-stack recognizer, guessing whether a word
# pair is a descriptor at all), gc_tag_bears_ptr (the preceding-tag test in gc_walk_words), and the two halves of
# the maps reporter's own divergence census, gc_sniff_would_take and gc_type_says_ref.  They disagree, and the
# disagreement is how a whole descriptor kind (DT_X) went unrecognised on the emitted stack: a live value's block
# was swept while gc_visit_one would have marked it.
#
# ⛔ WHY NOT "THE SETS MUST BE EQUAL", which was the cto's first offer and which the cfo REFUSED for a reason that
# is better than the offer: an arm demanding equality would force DT_SNUL into the speculative recognizer, and
# DT_SNUL is 0x00 on a stack where zero is the commonest byte -- the gate would then ENFORCE the defect.  So the
# law is a DECLARED SUBSET: a copy may not exceed its authority, and every kind the authority has and a copy does
# not is either EXCLUDED WITH A REASON or UNDER-CURE WITH AN OWNER AND A ROW.  An unexplained divergence reds.
#
# ⛔ THE ARM THAT MAKES THIS SELF-CLEANING, and it is the half that is usually forgotten: a declaration is STALE
# the day its cure lands.  If DT_X appears in gc_cell_visit while the UNDER-CURE entry is still in the table, this
# gate goes RED -- so the bookkeeping cannot be skipped in either direction, and an exclusion cannot quietly
# become a parking space.
#
# ⛔ RED BY DESIGN ON THE DAY IT LANDS AND DECLARED AS SUCH (CEO-513): nine divergences carry no reason at all --
# gc_sniff_would_take is short of its authority by SIX kinds and gc_type_says_ref by FIVE.  Those two are
# INSTRUMENT halves: a kind missing there does not lose a value, it makes the census that would have FOUND the
# loss read agreement.  That is an instrument reporting success while it is not looking, which is the recurring
# failure the INSTRUMENT LAWS exist to catch, and it is why this gate names them rather than waiting for a cure.
set -u
cd "$(dirname "$0")/.." || exit 2
ROOT="$PWD"
CENSUS="$ROOT/scripts/util_gc_kind_sets.py"
SRC="$ROOT/src/runtime/rt/gc_heap.c"
BASE_UNDECLARED=9
checks=0; fails=0
ck() { checks=$((checks+1)); if [ "$1" = ok ]; then echo "  ok   $2"; else fails=$((fails+1)); echo "  FAIL $2"; fi; }
refuse() { echo "⛔ GATE REFUSED(2) [gc_the_descriptor_kind_sets_are_a_declared_subset]: $1"; exit 2; }

[ -f "$CENSUS" ] || refuse "the census $CENSUS is missing -- nothing to grade"
[ -f "$SRC" ] || refuse "$SRC is missing -- the collector moved and this gate is reading nothing"

# (a) the instrument grades itself first
st="$(timeout 60s python3 "$CENSUS" --selftest 2>&1)"; src=$?
n="$(printf '%s\n' "$st" | sed -n 's/^SELFTEST \([0-9]*\)\/\([0-9]*\) arms green/\1 \2/p')"
set -- ${n:-0 0}
if [ "$src" = 0 ] && [ "${1:-0}" -ge 7 ] && [ "${1:-0}" = "${2:-x}" ]; then
  ck ok "(a) the kind-set census passes its own selftest, ${1} of ${2} arms"
else
  ck no "(a) the kind-set census selftest did not hold (rc=$src): $(printf '%s\n' "$st" | tail -3 | tr '\n' ' ')"
fi

rep="$(timeout 60s python3 "$CENSUS" 2>&1)"; rrc=$?
[ "$rrc" = 2 ] && refuse "$(printf '%s\n' "$rep" | grep -m1 REFUSED)"

# (b) the authority is read from the file, never assumed
auth="$(printf '%s\n' "$rep" | grep -m1 '^KINDSETS AUTHORITY ')"
ak="$(printf '%s\n' "$auth" | sed -n 's/.* kinds=\([0-9]*\) .*/\1/p')"
if [ "${ak:-0}" -ge 11 ] 2>/dev/null; then
  ck ok "(b) the authority is read out of gc_heap.c and carries ${ak} kinds -- $(printf '%s\n' "$auth" | cut -c1-120)"
else
  ck no "(b) the authority read as ${ak:-?} kinds -- a shrunken authority makes every copy look like a subset, which is agreement by erasure: $auth"
fi

# (c) NOBODY EXCEEDS THE AUTHORITY
if ! printf '%s\n' "$rep" | grep -q '^KINDSETS NOT-A-SUBSET '; then
  ck ok "(c) no spelling takes a kind the heap visitor does not -- the subset direction holds, so no copy visits something its authority cannot follow"
else
  ck no "(c) $(printf '%s\n' "$rep" | grep -c '^KINDSETS NOT-A-SUBSET ') spelling(s) exceed the authority: $(printf '%s\n' "$rep" | grep -m2 '^KINDSETS NOT-A-SUBSET ' | tr '\n' ' ')"
fi

# (d) PLANTED, BOTH DIRECTIONS: a new divergence must be NAMED and a landed cure must read STALE
plant="$(timeout 60s python3 - "$CENSUS" "$SRC" <<'PY' 2>&1
import sys, os, tempfile, importlib.util
census_path, src = sys.argv[1], sys.argv[2]
spec = importlib.util.spec_from_file_location("ks", census_path)
ks = importlib.util.module_from_spec(spec); spec.loader.exec_module(ks)
text = open(src, encoding="utf-8").read()
lines = []
drop = text.replace("if (d->v == DT_T) { rt_hblk_t *th = gc_blk_of((const char *)d->tbl); if (th && th->type == HB_AGGT && (char *)d->tbl == (char *)(th + 1)) { rt_gc_visit_descr(d); return 1; } return 0; }",
                    "if (0) { return 0; }", 1)
add = text.replace("    if (d->v == DT_A && gc_block_exact((const char *)d->arr, HB_ARR)) { rt_gc_visit_descr(d); return 1; }",
                   "    if (d->v == DT_X) { rt_gc_visit_descr(d); return 1; }\n    if (d->v == DT_A && gc_block_exact((const char *)d->arr, HB_ARR)) { rt_gc_visit_descr(d); return 1; }", 1)
for name, doctored in (("DROP", drop), ("ADD", add)):
    if doctored == text:
        print(f"PLANT-{name} FAILED-TO-SET-UP the anchor text was not found"); continue
    with tempfile.NamedTemporaryFile("w", suffix=".c", delete=False, encoding="utf-8") as fh:
        fh.write(doctored); p = fh.name
    got = []
    rc = ks.report(p, out=got.append)
    os.unlink(p)
    print(f"PLANT-{name} rc={rc} " + " | ".join(l[:90] for l in got if l.startswith(("KINDSETS UNDECLARED gc_cell_visit lacks DT_T", "KINDSETS STALE"))))
PY
)"
if printf '%s\n' "$plant" | grep -q 'PLANT-DROP rc=1 KINDSETS UNDECLARED gc_cell_visit lacks DT_T' \
   && printf '%s\n' "$plant" | grep -q 'PLANT-ADD rc=1 KINDSETS STALE gc_cell_visit HAS DT_X'; then
  ck ok "(d) PLANTED BOTH WAYS -- removing DT_T from the recognizer is NAMED as undeclared, and adding DT_X while its UNDER-CURE entry stands is NAMED as STALE"
else
  ck no "(d) the plant did not hold, so this gate can neither fail nor self-clean: $plant"
fi

# (e) THE READING ITSELF, and it is the work list
u="$(printf '%s\n' "$rep" | sed -n 's/^KINDSETS .* undeclared_or_stale=\([0-9]*\)$/\1/p')"
if [ "${u:-x}" = 0 ]; then
  ck ok "(e) every divergence from the authority is written down with its reason -- one fact, one authority, no absences"
elif [ "${u:-999}" -le "$BASE_UNDECLARED" ] 2>/dev/null; then
  ck no "(e) ${u} divergence(s) carry no reason (baseline $BASE_UNDECLARED, RED BY DESIGN until they are declared or cured): $(printf '%s\n' "$rep" | grep '^KINDSETS UNDECLARED ' | sed 's/^KINDSETS UNDECLARED //; s/ -- .*//' | tr '\n' ';')"
else
  ck no "(e) THE DIVERGENCE GREW to ${u} against a baseline of $BASE_UNDECLARED -- a new copy of the fact, or a new hole in an old one: $(printf '%s\n' "$rep" | grep '^KINDSETS UNDECLARED ' | sed 's/^KINDSETS UNDECLARED //; s/ -- .*//' | tr '\n' ';')"
fi

# (f) an instrument nobody runs is not an instrument
if grep -q 'test_gate_gc_the_descriptor_kind_sets_are_a_declared_subset.sh' "$ROOT/Makefile" 2>/dev/null; then
  ck ok "(f) this gate is named in the Makefile, as a REPORTED arm -- red by design while arm (e) names nine undeclared divergences (CEO-513 declare-when-you-hold-one-out, CEO-463 a known-red blocking arm makes every seat pay for one open row). It moves to blocking the day arm (e) goes green"
else
  ck no "(f) this gate is not named in the Makefile -- a gate in no runner is not measuring (RULES.md THE INSTRUMENT LAWS)"
fi

echo "population: $checks arm(s) graded, $fails FAIL; census rc=$rrc over 5 spellings in $(basename "$SRC")"
[ "$fails" = 0 ] && { echo "GATE PASS [gc_the_descriptor_kind_sets_are_a_declared_subset]: $checks of $checks arms hold"; exit 0; }
echo "⛔ GATE RED [gc_the_descriptor_kind_sets_are_a_declared_subset]: $fails of $checks arms FAIL"; exit 1
