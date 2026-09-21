#!/usr/bin/env bash
# test_gate_gc_emitted_safe_point_matches_its_contract.sh -- THE EMITTED BYTES READ AGAINST WHAT ARCH-GC SECTION 3
# DECLARES (cto 2026-09-21; row gc-the-emitted-safe-point-sequence-is-checked-against-its-declared-contract, ceo
# CEO-1029; from Hawblitzel and Petrank, AUTOMATED VERIFICATION OF PRACTICAL GARBAGE COLLECTORS, POPL 2009).
#
# WHY THIS GATE EXISTS, IN ONE SENTENCE.  ARCH-GC section 3 has read, since the page was written, that a safe point
# is "the return point of an emitted call into a runtime entry that can allocate, placed AFTER the call's result has
# been stored to its mapped slot", and section 3b (CEO-996) is the finding that NOTHING HAS EVER CHECKED THE EMITTED
# BYTES AGAINST IT.  A design sentence stating a property the emitter is never checked against is indistinguishable,
# to every reader and every instrument, from a property the emitter has.  This gate is that check.
#
# ⛔ WHAT A GREEN HERE DOES NOT SAY, WRITTEN FIRST SO NO READER TAKES IT FOR MORE.  It does not say the collector
# VISITS the mapped slot -- that is gc_heap.c and the cfo's file.  It does not say a mapped value is SAFE: a slot the
# map covers is still lost if the tag the recognizer meets is one it refuses.  It cannot see class 3 of section 3c, a
# raw heap pointer returned in a REGISTER PAIR, because a frame map describes memory and there is no store target to
# classify.  It says exactly one thing: at every safe point this reader can place, the emitted side of the section 3
# contract is CHECKED, and the sites it cannot place are NAMED and ratcheted rather than counted green.
#
# ⛔ THE POPULATION IS A DECLARED NAME SET AND NOT THE DIRECTORY GLOB, and that is the coo's rule of 2026-09-21 in
# their words: A GATE WHOSE POPULATION IS A DIRECTORY GLOB CANNOT CARRY A BLOCKING VERDICT WHEN THE DIRECTORY IS
# SHARED -- either the population is a declared name set or the verdict is REPORTED.  scripts/gc_witnesses/ is
# written by five seats, so a witness ARRIVING is a file addition and is REPORTED here, never blocking; a NAMED
# witness's own numbers MOVING is the whole thing this gate is for.
set -u
cd "$(dirname "$0")/.." || exit 2
ROOT="$PWD"
CHK="$ROOT/scripts/util_gc_safe_point_contract.py"
FLOOR="$ROOT/scripts/gc_safe_point_contract_floor.tsv"
SCRIP="$ROOT/scrip"
POP="${CONTRACT_POP:-$(echo "$ROOT"/scripts/gc_witnesses/*.icn "$ROOT"/scripts/gc_witnesses/*.sno "$ROOT"/scripts/gc_witnesses/*.pl "$ROOT"/scripts/gc_witnesses/*.raku)}"
PLANT_WIT="${CONTRACT_PLANT:-$ROOT/scripts/gc_witnesses/hb_aggt.icn}"
checks=0; fails=0
ck() { checks=$((checks+1)); if [ "$1" = ok ]; then echo "  ok   $2"; else fails=$((fails+1)); echo "  FAIL $2"; fi; }
refuse() { echo "⛔ GATE REFUSED(2) [gc_emitted_safe_point_matches_its_contract]: $1"; exit 2; }

[ -x "$SCRIP" ] || refuse "no scrip binary at $SCRIP -- build before grading"
[ -f "$CHK" ]   || refuse "the checker $CHK is missing -- nothing to grade"
[ -f "$FLOOR" ] || refuse "the declared floor $FLOOR is missing; a gate with no floor either reds the fleet over a known class or passes over everything, and both are worse than refusing"
[ -f "$PLANT_WIT" ] || refuse "the plant's witness $PLANT_WIT is missing -- arm (c) could not run, and an unplanted instrument is not graded"
echo "ARENA SCRIP_HEAP_MB=${SCRIP_HEAP_MB:-1} (the tiny arena is the default of GC testing -- CEO-931/934)"
echo "POPULATION (declared): $(grep -vc '^#' "$FLOOR") witness(es) NAMED in $(basename "$FLOOR"); $(printf '%s\n' $POP | wc -w) file(s) present on disk."
echo "CONTRACT_POP= narrows the run; CONTRACT_PLANT= names the plant's witness."

# (a) the instrument grades ITSELF before it grades the tree
st="$(timeout 120s python3 "$CHK" --selftest 2>&1)"; src=$?
n="$(printf '%s\n' "$st" | sed -n 's/^SELFTEST \([0-9]*\)\/\([0-9]*\) arms green/\1 \2/p')"
set -- ${n:-0 0}
if [ "$src" = 0 ] && [ "${1:-0}" -ge 12 ] && [ "${1:-0}" = "${2:-x}" ]; then
  ck ok "(a) the checker passes its own selftest, ${1} of ${2} arms, every clause PLANTED end to end and K4 carrying a negative arm as well as a positive one"
else
  ck no "(a) the checker's selftest did not hold (rc=$src): $(printf '%s\n' "$st" | tail -3 | tr '\n' ' ')"
fi

# (b) THE READING against the DECLARED floor
READ_OUT="$(mktemp -t gc_contract_reading.XXXXXX)"
trap 'rm -f "$READ_OUT" "$READ_OUT".cmp' EXIT
SCRIP_HEAP_MB="${SCRIP_HEAP_MB:-1}" timeout 900s python3 "$CHK" $POP --name-all > "$READ_OUT" 2>&1; rrc=$?
rd="$(cat "$READ_OUT")"
[ "$rrc" = 2 ] && refuse "the checker refused over the declared population: $(grep -m1 REFUSED "$READ_OUT")"
sl="$(grep -m1 '^CONTRACT SITES ' "$READ_OUT")"
[ -n "$sl" ] || refuse "the checker produced no CONTRACT SITES line -- nothing was measured and a missing reading is never a pass"
# ⛔ THE READING IS PASSED BY PATH AND NOT DOWN A PIPE, AND THE FIRST CUT OF THIS ARM DID THE OTHER THING.
# `python3 - <<'PY'` takes its PROGRAM from stdin, so a pipe into it is silently discarded: the comparison read
# ZERO lines, every declared witness compared against nothing, and the gate reported 38 STALE DECLARATIONS --
# A DEFECT IN THE FLOOR, announced on the strength of a reading that never arrived.  Same class as the two plant
# arms cured at SCRIP c9c7eefc4 the same day: a null input graded as a finding about the subject.  The REFUSAL
# below is the guard, and it is checked rather than trusted.
cmp_out="$(timeout 120s python3 "$ROOT/scripts/util_gc_safe_point_contract.py" --compare "$READ_OUT" "$FLOOR" 2>&1)"
worse="$(printf '%s\n' "$cmp_out" | sed -n 's/^WORSE \([0-9]*\).*/\1/p')"
stale="$(printf '%s\n' "$cmp_out" | sed -n 's/^STALE \([0-9]*\).*/\1/p')"
unlisted="$(printf '%s\n' "$cmp_out" | sed -n 's/^UNLISTED \([0-9]*\).*/\1/p')"
[ -n "$worse" ] || refuse "(b) the floor comparison produced no verdict -- $cmp_out"
if [ "$worse" = 0 ]; then
  ck ok "(b) no NAMED witness breaks a contract clause more than its declared floor -- $(printf '%s\n' "$sl" | sed 's/^CONTRACT SITES //')"
else
  ck no "(b) $worse NAMED witness/clause pair(s) moved ABOVE the declared floor, which is a REGRESSION in the emitted safe-point sequence and not a witness arriving: $(printf '%s\n' "$cmp_out" | sed -n 's/^WORSE [0-9]* //p')"
fi
if [ "${unlisted:-0}" = 0 ]; then
  ck ok "(c) every witness on disk is NAMED in the declared floor"
else
  echo "  note $unlisted witness(es) are on disk but not in the floor -- REPORTED, never blocking (the coo's rule): $(printf '%s\n' "$cmp_out" | sed -n 's/^UNLISTED [0-9]* //p')"
  ck ok "(c) $unlisted unlisted witness(es) REPORTED rather than counted -- a directory five seats write to cannot carry a blocking verdict"
fi

# (d) THE EXPIRY, PROVEN RATHER THAN ASSERTED -- hq_icon's shape (CTO-160, 2026-09-21), copied deliberately
if [ "${stale:-0}" = 0 ]; then
  ck ok "(d) no declaration has outlived its cure"
else
  ck no "(d) $stale declaration(s) in $(basename "$FLOOR") are STALE -- the witness now breaks NOTHING and the floor still says it does. Move the floor in the landing that earned it (python3 scripts/util_gc_safe_point_contract.py <witnesses> --write-floor). A declaration that outlives its cure is how an exemption list becomes the new floor: $(printf '%s\n' "$cmp_out" | sed -n 's/^STALE [0-9]* //p')"
fi

# (e) PLANTED ON REAL EMITTED TEXT -- an instrument that cannot fail here cannot pass here either
plant="$(timeout 300s python3 - "$ROOT" "$SCRIP" "$PLANT_WIT" <<'PY' 2>&1
import sys, os, re, tempfile
root, scrip, wit = sys.argv[1], sys.argv[2], sys.argv[3]
sys.path.insert(0, os.path.join(root, "scripts"))
import util_gc_safe_point_contract as SP
with tempfile.TemporaryDirectory() as wd:
    asm, rep, err = SP.UC.emit_and_read(scrip, wit, wd)
    if err:
        print("PLANT-REFUSED-TO-SET-UP", err); sys.exit(0)
    base, _s, refusal = SP.contract_asm(asm, rep, "base")
    if refusal:
        print("PLANT-REFUSED-TO-SET-UP", refusal); sys.exit(0)
    lines = open(asm, encoding="utf-8").read().split("\n")
    def first_poll():
        for n, ln in enumerate(lines):
            if "rt_gc_poll" in ln and "call" in ln:
                return n
        return None
    pn = first_poll()
    if pn is None:
        print("PLANT-REFUSED-TO-SET-UP the plant witness emits no poll"); sys.exit(0)
    cut = [ln for i, ln in enumerate(lines)
           if not (i < pn and i > pn - 12 and re.search(r"mov\s+qword ptr \[(rsp|rbp) [+-] -?\d+\], (rax|rdx)", ln))]
    p1 = asm + ".nostore.s"; open(p1, "w", encoding="utf-8").write("\n".join(cut))
    r1, _s1, ref1 = SP.contract_asm(p1, rep, "nostore")
    moved = [ln for i, ln in enumerate(lines)]
    for i in range(max(0, pn - 12), pn):
        m = re.search(r"mov\s+qword ptr \[(rsp|rbp) [+-] -?\d+\], rax", moved[i])
        if m:
            moved[i] = re.sub(r"\[(rsp|rbp) [+-] -?\d+\]", r"[\1 + -4096]", moved[i]); break
    p2 = asm + ".below.s"; open(p2, "w", encoding="utf-8").write("\n".join(moved))
    r2, _s2, ref2 = SP.contract_asm(p2, rep, "below")
    def k(rows, key):
        return 0 if rows is None else sum(1 for r in rows if r[4] == key and r[5] == "MEMBER")
    print("PLANT-RESULT base_k2=%d base_k3=%d nostore_k2=%d below_k3=%d ref1=%s ref2=%s"
          % (k(base, "K2"), k(base, "K3"), k(r1, "K2"), k(r2, "K3"), ref1, ref2))
PY
)"
b2="$(printf '%s\n' "$plant" | sed -n 's/.*base_k2=\([0-9]*\) .*/\1/p')"
n2="$(printf '%s\n' "$plant" | sed -n 's/.*nostore_k2=\([0-9]*\) .*/\1/p')"
b3="$(printf '%s\n' "$plant" | sed -n 's/.*base_k3=\([0-9]*\) .*/\1/p')"
n3="$(printf '%s\n' "$plant" | sed -n 's/.*below_k3=\([0-9]*\) .*/\1/p')"
if [ -z "$b2" ] || [ -z "$n2" ] || [ -z "$b3" ] || [ -z "$n3" ]; then
  refuse "(e) THE PLANT DID NOT PRODUCE A VERDICT, AND THAT IS A REFUSAL RATHER THAN A FINDING ABOUT THE CHECKER. A null result bounds the PROBE and not the thing probed, and without this branch an empty reading falls through to a default and is graded as the checker ABSORBING the plant. What came back: $plant"
elif [ "$n2" -gt "$b2" ] 2>/dev/null && [ "$n3" -gt "$b3" ] 2>/dev/null; then
  ck ok "(e) PLANTED ON REAL EMITTED TEXT -- deleting the result stores before a poll takes K2 from $b2 to $n2, and moving one store 4096 bytes below its frame base takes K3 from $b3 to $n3: $plant"
else
  ck no "(e) the checker ABSORBED a doctored safe point and still read the same population ($plant) -- an instrument that cannot fail here cannot pass here either"
fi

# (f) an instrument nobody runs is not an instrument
if grep -q 'test_gate_gc_emitted_safe_point_matches_its_contract.sh' "$ROOT/Makefile" 2>/dev/null; then
  ck ok "(f) this gate is named in the Makefile"
else
  ck no "(f) this gate is not named in the Makefile -- a gate in no runner is not measuring (RULES.md THE INSTRUMENT LAWS)"
fi

echo "population: $checks arm(s) graded, $fails FAIL; checker rc=$rrc"
printf '%s\n' "$rd" | grep -m1 '^CONTRACT MEANS'
[ "$fails" = 0 ] && { echo "GATE PASS [gc_emitted_safe_point_matches_its_contract]: $checks of $checks arms hold"; exit 0; }
echo "⛔ GATE RED [gc_emitted_safe_point_matches_its_contract]: $fails of $checks arms FAIL"; exit 1
