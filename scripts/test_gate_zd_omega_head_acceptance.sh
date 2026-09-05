#!/bin/bash
# test_gate_zd_omega_head_acceptance.sh — acceptance gate for the row
# zd-omega-head-per-op-filter-one-cause-behind-boolptr-boolidx-and-the-spine-leaks
# (postoffice task: zd-omega-head-per-op-filter-one-cause-behind-boolptr-boolidx-and-the-spine-leaks.task.md)
#
# hq_B root-caused: zd_omega_head (emit.cpp:2498) is a PER-OP FILTER admitting only IR_CMP_TEST while
# Pascal's relop lowers to its sibling IR_BINOP_TEST (RULES.md NO-PER-OP-FILTER). One cause, four tracked
# failures: boolptr, boolidx (same defect via an array lvalue), and the bubble/quick mode-4 spine leaks
# (their own row's one-line description, "zd_plan misses IR_BINOP_TEST merge points", was right all along).
# This row was minted with NO runnable DONE-WHEN; this script IS that DONE-WHEN, per the row's own NEXT.
#
# Checks, in order, REFUSING rc=2 on any missing prerequisite (a missing binary or corpus file is a
# refusal to grade, never a silent pass -- RULES.md "A MISSING PREREQUISITE IS A REFUSAL"):
#   1. STRUCTURAL   -- the exact known-bad single-op admission string named in the FINDING is gone from
#                      zd_omega_head. A regression witness, not a proof the replacement is correct.
#   2. NAMED WITNESSES -- boolptr + boolidx byte-match their .ref in BOTH m3 and m4 (surgical: this row is
#                      about these two names, not the whole Pascal board -- deep5/pb34 are separate,
#                      already-tracked defects and must not gate this row either way).
#   2b. REGRESSION DETECTOR -- a_plainvar + f_const_then_relop (hq_B's FINDING, inline, not corpus fixtures):
#                      the naive 2-line candidate cure passes boolptr/boolidx while silently regressing
#                      a_plainvar, so checking only the two named failures would wave a wrong fix through.
#   3. SPINE LEAK   -- bubble + quick, mode 4, 5/5 runs each under `setarch -R` (fixed address layout):
#                      rc=0 and output byte-matches the checked-in .ref every time. A real per-iteration
#                      $rsp leak crashes before a full-size sort completes, so 5/5 clean full-size runs is
#                      sufficient behavioural evidence without re-deriving seat12/seat02's instruction trace.
#   4. SHARED-NODE CONTROL BATTERY -- zd_plan + x86_asm.h serve all seven languages (RULES.md SHARED-NODE
#                      VERDICT SCOPE): SNOBOL4 must be FAIL=0 over its printed denominator; every other
#                      frontend must not have fallen under its own standing floor.
#
# ⛔ THIS GATE DOES NOT BUILD. Run `make pristine` yourself first for a real verdict (HQ-27) -- running a
# board and a build in the same tree is independently forbidden (RULES.md s268), and this row's own history
# (a live claim on pascal-restore-prezeta touching this exact code) is the direct witness of why that
# matters: do not run this gate concurrently with a build in the same working tree.
#
# Exit: 0 all green · 1 measured and something failed · 2 refused to grade (prerequisite missing).
set -u
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"
cd "$(dirname "$0")/.."
SCRIP="${SCRIP:-$PWD/scrip}"
RT="${RT:-$PWD/out/libscrip_rt.so}"
PASCAL_TESTS="${PASCAL_TESTS:-$S4E/corpus/tests/pascal}"
PASCAL_BENCH="${PASCAL_BENCH:-$S4E/corpus/benchmarks/pascal}"

[ -x "$SCRIP" ] || { echo "⛔ REFUSE(2): no executable $SCRIP -- build first (make pristine)" >&2; exit 2; }
[ -f "$RT" ] || { echo "⛔ REFUSE(2): no $RT -- run make libscrip_rt first" >&2; exit 2; }
for f in "$PASCAL_TESTS/boolptr.pas" "$PASCAL_TESTS/boolptr.ref" "$PASCAL_TESTS/boolidx.pas" "$PASCAL_TESTS/boolidx.ref" \
         "$PASCAL_BENCH/bubble.pas" "$PASCAL_BENCH/bubble.ref" "$PASCAL_BENCH/quick.pas" "$PASCAL_BENCH/quick.ref"; do
    [ -f "$f" ] || { echo "⛔ REFUSE(2): missing witness file $f" >&2; exit 2; }
done
command -v setarch >/dev/null || { echo "⛔ REFUSE(2): setarch not on PATH -- needed for the spine-leak arm" >&2; exit 2; }

TMP=$(mktemp -d)
trap "rm -rf $TMP" EXIT
FAIL=0
report() { # report <name> <ok:0|1> <detail>
    if [ "$2" -eq 0 ]; then echo "✅ PASS $1 -- $3"; else echo "⛔ FAIL $1 -- $3"; FAIL=$((FAIL+1)); fi
}

# --- 1. STRUCTURAL: the exact known-bad per-op shape must be gone ------------------------------------------
# ⛔⭐ REWRITTEN BY hq_C 2026-09-05. The previous clause grepped for ONE HISTORICAL BAD STRING (the
# IR_CMP_TEST-only spelling). Adding IR_IDENT/IR_DIFFER/IR_BINOP_TEST to the list made that grep MISS, so this
# clause reported ✅ PASS from 2026-08-29 to 2026-09-04 while the per-op filter was still sitting there with
# four ops in it -- a false green that survived six sessions and let seat05 re-derive the whole mechanism from
# SNOBOL4. A structural clause that names the KNOWN-BAD form passes the moment anyone writes a DIFFERENT bad
# form. Assert the SHAPE OF THE CURE instead: no op-identity branch anywhere in the predicate.
predline=$(grep -h 'static int zd_omega_test_kind' src/emitter/emit.cpp)
if [ -z "$predline" ]; then
    report structural-zd-omega-head 1 "zd_omega_test_kind not found at all -- this clause can no longer grade the predicate; repoint the gate rather than reading its silence as a pass"
elif printf '%s' "$predline" | grep -qE '(^|[^_[:alnum:]])op[[:space:]]*==|->op[[:space:]]*=='; then
    report structural-zd-omega-head 1 "zd_omega_test_kind STILL ADMITS BY OP IDENTITY (RULES.md NO PER-OP FILTER) -- an op list of ANY length is the defect, not just the historical one-op spelling: $predline"
elif ! printf '%s' "$predline" | grep -q 'ω\.node'; then
    report structural-zd-omega-head 1 "zd_omega_test_kind no longer branches on op identity but does not consult the ω port either -- a predicate that admits everything is not the family fix: $predline"
else
    report structural-zd-omega-head 0 "zd_omega_test_kind admits by STRUCTURE (ω port, no op== chain), not by op identity"
fi

# --- helpers: compile+link mirrors test_gate_pascal_m4.sh exactly (same RT, same flags) ---------------------
compile_link() { # compile_link <pas> <exe-out> -> rc 0 on success
    local pas=$1 exe=$2 asm="$TMP/$(basename "$pas" .pas).s" obj="$TMP/$(basename "$pas" .pas).o"
    timeout 8s "$SCRIP" --compile "$pas" < /dev/null > "$asm" 2>/dev/null || return 1
    [ -s "$asm" ] || return 1
    gcc -c -o "$obj" "$asm" 2>/dev/null && gcc -o "$exe" "$obj" "$RT" -no-pie -Wl,-rpath,"$(dirname "$RT")" 2>/dev/null
}

# --- 2. Named Pascal witnesses: boolptr + boolidx, BOTH modes -----------------------------------------------
for name in boolptr boolidx; do
    pas="$PASCAL_TESTS/$name.pas"; ref="$PASCAL_TESTS/$name.ref"; inp="$PASCAL_TESTS/$name.in"; [ -f "$inp" ] || inp=/dev/null
    out3=$(timeout 8s "$SCRIP" --run "$pas" < "$inp" 2>/dev/null); rc3=$?
    if [ "$rc3" -eq 0 ] && [ "$out3" = "$(cat "$ref")" ]; then report "pascal-$name-m3" 0 "byte-matches .ref"; else report "pascal-$name-m3" 1 "rc=$rc3, does not match .ref"; fi
    exe="$TMP/$name.exe"
    if compile_link "$pas" "$exe"; then
        out4=$(timeout 8s "$exe" < "$inp" 2>/dev/null); rc4=$?
        if [ "$rc4" -eq 0 ] && [ "$out4" = "$(cat "$ref")" ]; then report "pascal-$name-m4" 0 "byte-matches .ref"; else report "pascal-$name-m4" 1 "rc=$rc4, does not match .ref"; fi
    else
        report "pascal-$name-m4" 1 "compile or link failed"
    fi
done

# --- 2b. REGRESSION DETECTOR: a_plainvar + f_const_then_relop (hq_B's FINDING, both modes) -------------------
# ⛔ NOT corpus fixtures, deliberately -- hq_B kept these OUT of corpus/tests/pascal on purpose (mid-bisect,
# moving the denominator was explicitly avoided), so they are embedded here rather than committed as .pas/.ref.
# hq_B's own words: "a_plainvar is the cheapest regression detector found so far" -- the naive 2-line candidate
# cure (admit IR_BINOP_TEST + relax the >0 guard) passes boolptr/boolidx while silently REGRESSING this one,
# because the omega arm's temp write and the merge read land at different zd_plan depths. A gate that only
# checks boolptr/boolidx would wave the naive cure through; this block is what stops that.
inline_witness() { # inline_witness <name> <src-var> <expected>
    # Compares with whitespace stripped: Pascal's default writeln(integer) right-pads to a field width
    # (SCRIP included), which is incidental formatting, not the thing this witness is checking -- the
    # witness cares which DIGIT printed, not its column width.
    local name=$1 src="$2" expected="$3" pas="$TMP/$name.pas" want; want=$(printf '%s' "$expected" | tr -d '[:space:]')
    printf '%s\n' "$src" > "$pas"
    local out3 rc3 got3; out3=$(timeout 8s "$SCRIP" --run "$pas" < /dev/null 2>/dev/null); rc3=$?; got3=$(printf '%s' "$out3" | tr -d '[:space:]')
    if [ "$rc3" -eq 0 ] && [ "$got3" = "$want" ]; then report "$name-m3" 0 "matches expected"; else report "$name-m3" 1 "rc=$rc3 got=[$out3] want=[$expected]"; fi
    local exe="$TMP/$name.exe"
    if compile_link "$pas" "$exe"; then
        local out4 rc4 got4; out4=$(timeout 8s "$exe" < /dev/null 2>/dev/null); rc4=$?; got4=$(printf '%s' "$out4" | tr -d '[:space:]')
        if [ "$rc4" -eq 0 ] && [ "$got4" = "$want" ]; then report "$name-m4" 0 "matches expected"; else report "$name-m4" 1 "rc=$rc4 got=[$out4] want=[$expected]"; fi
    else
        report "$name-m4" 1 "compile or link failed"
    fi
}
inline_witness a_plainvar \
'program a(output); var b : boolean; i : integer;
begin i := 7; b := i > 3; if b then writeln(1) else writeln(0);
              b := i < 3; if b then writeln(1) else writeln(0) end.' \
"1
0"
inline_witness f_const_then_relop \
'program f(output); type rp = ^rec; rec = record f : boolean end; var p : rp; i : integer;
begin i := 7; new(p); p^.f := true;  if p^.f then writeln(1) else writeln(0);
                      p^.f := i < 3; if p^.f then writeln(1) else writeln(0) end.' \
"1
0"

# --- 3. bubble + quick, mode 4, 5/5 under setarch -R --------------------------------------------------------
# ⛔ FIXED 2026-09-02 (seat16): both kernels open `readln(reps)` (test_bench_pascal_timed.sh:6, every
# reps-capable Pascal benchmark) -- `< /dev/null` on a run that reads stdin overrides silently and reads
# EOF instead of a real value (CLAUDE.md's own named trap, seat05 2026-09-01: "the redirect overrides the
# pipe, the program reads EOF and prints a plausible answer"), which is exactly what happened here: this
# arm reported 0/5 CLEAN on BOTH kernels even against a tree with Site 1 (bubble's spine leak) already
# fixed. `echo 1` matches every other script that runs these two kernels (calling-convention-depth-tracked's
# own DONE-WHEN, pascal-quick-m4-wrong-checksum-crash-masked's own DONE-WHEN, test_bench_pascal_timed.sh)
# and is the input bubble.ref/quick.ref were generated against.
for name in bubble quick; do
    pas="$PASCAL_BENCH/$name.pas"; ref="$PASCAL_BENCH/$name.ref"; exe="$TMP/$name.bench.exe"
    if compile_link "$pas" "$exe"; then
        hits=0
        for i in 1 2 3 4 5; do
            out=$(echo 1 | setarch "$(uname -m)" -R timeout 30s "$exe" 2>/dev/null); rc=$?
            [ "$rc" -eq 0 ] && [ "$out" = "$(cat "$ref")" ] && hits=$((hits+1))
        done
        if [ "$hits" -eq 5 ]; then report "pascal-$name-m4-5x" 0 "5/5 clean under setarch -R (no crash => no observable spine drift)"; else report "pascal-$name-m4-5x" 1 "$hits/5 clean under setarch -R"; fi
    else
        report "pascal-$name-m4-5x" 1 "compile or link failed"
    fi
done

# --- 4. SHARED-NODE CONTROL BATTERY: every other frontend must not have regressed ---------------------------
echo "--- shared-node control battery (SNOBOL4 must be FAIL=0; others must not fall under their standing floor) ---"

# ⛔ NOT rc ALONE: test_corpus_snobol4.sh's own exit code hard-fails on mode-4 FAIL only (mode-3 FAIL is
# printed as "informational" by that script's own design, confirmed 2026-08-29 -- a run here reported
# rc=0 "GATE OK" with m3 FAIL=1, m4 FAIL=0). This row's DONE-WHEN wants BOTH modes FAIL=0, so parse both
# counts explicitly rather than trust a exit code that is scoped to one mode only.
out=$(bash scripts/test_corpus_snobol4.sh 2>&1)
f3=$(printf '%s\n' "$out" | grep -oP 'm3 PASS=[0-9]+ FAIL=\K[0-9]+' | head -1)
f4=$(printf '%s\n' "$out" | grep -oP 'm4 PASS=[0-9]+ FAIL=\K[0-9]+' | head -1)
if [ -n "${f3:-}" ] && [ -n "${f4:-}" ] && [ "$f3" -eq 0 ] && [ "$f4" -eq 0 ]; then
    report snobol4-blocking 0 "$(printf '%s\n' "$out" | tail -1)"
else
    report snobol4-blocking 1 "m3 FAIL=${f3:-UNPARSEABLE} m4 FAIL=${f4:-UNPARSEABLE} -- both modes must read 0 (informational-only m3 FAIL is not a pass for this row)"
fi

out=$(bash scripts/test_icon_rung_suite.sh --mode interp 2>&1)
icon_pass=$(printf '%s\n' "$out" | grep -oP '(?<=^--- Icon \(interp\): PASS=)[0-9]+' | head -1)
if [ -n "${icon_pass:-}" ] && [ "$icon_pass" -ge 232 ]; then report icon-floor 0 "PASS=$icon_pass (>=232 standing floor)"; else report icon-floor 1 "PASS=${icon_pass:-UNPARSEABLE} (<232 floor, or the anchored summary line did not print)"; fi

out=$(bash scripts/test_smoke_raku.sh 2>&1)
if printf '%s\n' "$out" | grep -qE '^mode-3 .*FAIL=0 ' && printf '%s\n' "$out" | grep -qE '^mode-4 .*FAIL=0 '; then
    report raku-smoke 0 "both modes FAIL=0"
else
    report raku-smoke 1 "$(printf '%s\n' "$out" | grep '^mode-' | tr '\n' ' ')"
fi

# ⭐⭐ RE-PINNED seat15 2026-09-01 UNDER hq_P'S EXPLICIT RULING (reply to ask q-prolog-instruments-were-blind:
# "test_gate_zd_omega_head_acceptance.sh:166 is MY gate ... re-pin its crosscheck consumption to the CURRENT
# measured state as a FLOOR WITH THE FAIL-SET BY NAME ... so the gate grades omega-head regressions and not
# Prolog's pre-existing correctness debt (which is the umbrella's)").
# ⛔ WHY NOT A BARE rc: this line consumed test_crosscheck_prolog.sh's exit code. That script was BLIND until
# 2026-09-01 -- it captured --run into both sides and branched on X != X (mode 4 never invoked), read .ref
# where the corpus stores .expected (oracle never consulted), and pre-skipped crashers -- so it reported
# PASS=13 FAIL=0 exit 0 and this gate read a manufactured green. Cured, it honestly reports m3 3/15 m4 3/15,
# and a bare rc would now pin THIS gate permanently red: a criterion that can never say YES, which is as
# useless as one that can never say NO. See FINDING-2026-09-01-seat15-prolog-both-mode-instruments-were-blind.md.
# ⭐ THE FLOOR: the twelve stems below are the measured fail-set at SCRIP 3ce7a526 (pristine -O0, corpus
# ad1fdaa71) -- IDENTICAL for m3 and m4. A stem LEAVING the set is a red (omega-head regression, this gate's
# business); a stem ENTERING it is a red. The set shrinking is good news that must still be acknowledged by
# hand, so it reports red until someone re-pins it deliberately -- that is the point of a floor.
# ⭐ RE-PINNED hq_P 2026-09-02 at SCRIP 48b09e04 (-O0), measured on TWO binaries -- the pushed tree and a scratch worktree of the same tree
# plus one Pascal-only statement -- both reading the identical 12-stem set, m3 == m4: rung15_abolish_abolish_one_of_two LEFT the set (cured
# upstream between 3ce7a526 and 48b09e04; acknowledged here, as the floor's own text asks) and rung22_write_canonical_write_canonical_list
# ENTERED it (a Prolog correctness regression in the same window; routed to hq_C, the umbrella's, not this gate's). DIVERGE reads 1 or 2
# run to run on the same binary (the flapping class) and is deliberately not part of the pin.
PL_XC_KNOWN="rung11_findall_findall_arith rung11_findall_findall_filter rung14_retract_retract_basic rung14_retract_retract_mixed rung15_abolish_abolish_then_reassert rung22_write_canonical_write_canonical_list rung44_setof_group rung45_reflect_clause_facts rung45_reflect_clause_findall rung50_between_enum rung50_for_alias rung66_current_stream"
out=$(bash scripts/test_crosscheck_prolog.sh 2>&1)
xc_got=$(printf '%s\n' "$out" | grep -oE 'm3 FAIL [a-z0-9_]+' | sed 's/m3 FAIL //' | sort | tr '\n' ' ')
xc_want=$(printf '%s\n' $PL_XC_KNOWN | sort | tr '\n' ' ')
xc_sum=$(printf '%s\n' "$out" | grep '^PL-CROSSCHECK')
if [ "$xc_got" = "$xc_want" ]; then
    report prolog-crosscheck 0 "fail-set matches the pinned floor (12 stems, m3==m4) — $xc_sum"
else
    report prolog-crosscheck 1 "FAIL-SET MOVED vs pinned floor — left:[$(comm -23 <(printf '%s\n' $xc_want) <(printf '%s\n' $xc_got) | tr '\n' ' ')] entered:[$(comm -13 <(printf '%s\n' $xc_want) <(printf '%s\n' $xc_got) | tr '\n' ' ')] — $xc_sum"
fi

out=$(bash scripts/test_smoke_snocone.sh 2>&1); rc=$?
if [ "$rc" -eq 0 ]; then report snocone-smoke 0 "$(printf '%s\n' "$out" | tail -1)"; else report snocone-smoke 1 "$(printf '%s\n' "$out" | tail -1)"; fi

out=$(bash scripts/test_smoke_rebus.sh 2>&1); rc=$?
if [ "$rc" -eq 0 ]; then report rebus-smoke 0 "$(printf '%s\n' "$out" | tail -1)"; else report rebus-smoke 1 "$(printf '%s\n' "$out" | tail -1)"; fi

# ⛔ NOT A FAIL=0 BAR, DELIBERATELY: polyglot demos carry pre-existing, already-owned failures with NO
# relation to zd_omega_head/IR_BINOP_TEST -- confirmed 2026-08-29 by running this exact check against an
# UNCHANGED tree (structural check above still FAILs, i.e. zero code has moved) and seeing FAIL>0 anyway.
# Attributed, not just excused: polyglot-main-selector-ignores-main-mod-registry (driver main-selection,
# names this exact script as ITS OWN before/after instrument) + polyglot-define-entry-address-wrong-in-
# merged-program (SNOBOL4 DEFINE entry-address, demo03/demo08) + polyglot-demo-empty-output-rc0 (live claim)
# together account for the standing red. Floor pattern matches icon-floor above: assert no NEW regression
# below today's watermark, never a bar this row cannot itself clear. Re-measure before trusting the floor
# after any polyglot-lane row lands -- do not carry it forward uncritically (FACT RULE: re-measured, not copied).
out=$(bash scripts/test_gate_polyglot_demos.sh 2>&1)
p3=$(printf '%s\n' "$out" | grep -oP '(?<=^m3 PASS=)[0-9]+'); p4=$(printf '%s\n' "$out" | grep -oP '(?<=^m4 PASS=)[0-9]+')
if [ -n "${p3:-}" ] && [ -n "${p4:-}" ] && [ "$p3" -ge 7 ] && [ "$p4" -ge 3 ]; then
    report polyglot-demos-floor 0 "m3 PASS=$p3 (>=7) m4 PASS=$p4 (>=3) -- 2026-08-29 pre-existing floor, owned by other rows"
else
    report polyglot-demos-floor 1 "m3 PASS=${p3:-UNPARSEABLE} m4 PASS=${p4:-UNPARSEABLE} -- fell BELOW the pre-existing floor: a NEW regression, unlike the standing polyglot reds"
fi

echo "=== $([ "$FAIL" -eq 0 ] && echo ALL GREEN || echo "$FAIL CHECK(S) FAILED") ==="
[ "$FAIL" -eq 0 ]
