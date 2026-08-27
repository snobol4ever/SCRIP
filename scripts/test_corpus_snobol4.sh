#!/usr/bin/env bash
# scripts/test_corpus_snobol4.sh — SNOBOL4 broad corpus, modes 2+3+4
# Mode-4 gate (hard). Modes 2+3 informational. Reinstated 2026-06-08.
# Compares output against .ref files. Reports PASS/FAIL/SKIP per mode.
#
# Self-contained per RULES.md: paths from $0, timeout on every run.
# AUTHORS: Lon Jones Cherryholmes · Claude Sonnet 4.6   DATE: 2026-05-14
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${SCRIP:-$HERE/../scrip}"
RT_DIR="${RT_DIR:-$HERE/../out}"
CORPUS="$S4E/corpus"
TIMEOUT="${TIMEOUT:-10}"
INC="${INC:-$CORPUS/include}"
BEAUTY="${BEAUTY:-$CORPUS/tests/snobol4/beauty_suite}"
DEMO="${DEMO:-$CORPUS/demo/snobol4}"

# ⛔⛔⛔ s272 hq_C (CEO audit correction 2) — A MISSING PREREQUISITE IS A REFUSAL (rc=2), NEVER A GREEN EXIT.
# These two arms printed SKIP and exited 0, so a box with no compiler built, or no corpus cloned, reported
# SUCCESS to every caller that reads $? — which is every gate, every board, and handoff_status.sh. That is the
# ABSENT-ORACLE FALSE-GREEN class, and it is the exact twin of the `make test` no-recipe trap: a check that
# ran nothing and said nothing was wrong. ⭐ rc=2 means REFUSED-TO-GRADE and is distinct from rc=1 FAILED:
# a caller can tell 'I could not measure' from 'I measured and it is broken'. Silence could say neither.
if [ ! -x "$SCRIP" ]; then echo "⛔ REFUSED TO GRADE: scrip not built at $SCRIP" >&2; exit 2; fi
if [ ! -d "$CORPUS" ]; then echo "⛔ REFUSED TO GRADE: corpus not found at $CORPUS" >&2; exit 2; fi
# ⛔⛔ REFUSE ON A MISSING SUBTREE -- DO NOT SILENTLY DISCOVER FEWER PROGRAMS (hq_C s271). This board read
# "PASS=342 FAIL=0" for a whole session because $DEMO pointed at a path that did not exist: every visible signal
# said green while 22 programs had left the denominator. A clean numerator over a shrunken denominator is the most
# dangerous shape a board has, and corpus paths have moved three times in two days, so this WILL happen again.
# FAIL=0 is not a verdict; FAIL=0 over the expected denominator is.
for _d in "$DEMO" "$BEAUTY"; do
    if [ ! -d "$_d" ]; then echo "⛔ GATE REFUSES: corpus subtree missing: $_d"; echo "   The corpus layout moved. Repoint this script; do NOT read a smaller total as a pass."; exit 2; fi
done

PASS3=0; FAIL3=0; FAILURES3=""
PASS4=0; FAIL4=0; SKIP4=0; FAILURES4=""
MISSING=0; MISSING_LIST=""

WORKDIR="$(mktemp -d)"
trap 'rm -rf "$WORKDIR"' EXIT
T_M3=0; T_M4=0; T0_ALL=$SECONDS

compile_mode4() {
    local sno="$1" out="$2"
    local tmp; tmp="$(mktemp -d)"
    SNO_LIB="$INC" "$SCRIP" --compile "$sno" > "$tmp/p.s" 2>/dev/null || { rm -rf "$tmp"; return 1; }
    (cd "$HERE/.." && gcc -c "$tmp/p.s" -o "$tmp/p.o" 2>/dev/null) || { rm -rf "$tmp"; return 1; }
    gcc "$tmp/p.o" -L"$RT_DIR" -lscrip_rt -lm \
        -Wl,-rpath,"$RT_DIR" -o "$out" 2>/dev/null || { rm -rf "$tmp"; return 1; }
    rm -rf "$tmp"
}

# ⛔⛔ A HARDCODED PATH THAT NO LONGER RESOLVES IS A MISSING PROGRAM, NOT A MISSING TEST (hq_C s272, seat04's find).
# The s271 guard above checks the DIRECTORIES exist; it cannot see that $DEMO/wordcount/wordcount.sno stopped
# resolving when the tree re-nested underneath it. These two lines used to `return` silently, so each of the ~40
# hardcoded demo rows below could leave the board with NO signal at all -- neither PASS, nor FAIL, nor SKIP. The
# corpus demo paths have now been repointed FIVE times in one day (6ce46ebc dac73079 843cacfb 1177e66e 50923f55),
# and every single break was noticed only because a human recognised the printed total had shrunk. That is not an
# instrument. ⛔ The DISCOVERED corpora (crosscheck, beauty) filter a ref-less .sno BEFORE calling here, so a
# missing file can only ever mean a stale hardcoded path -- which is always a defect and never a legitimate skip.
run_test() {
    local label="$1" sno="$2" ref="$3" input="${4:-}" filter="${5:-}"
    if [ ! -f "$sno" ]; then MISSING=$((MISSING+1)); MISSING_LIST="${MISSING_LIST}  ${label}: no program at ${sno}\n"; return; fi
    if [ ! -f "$ref" ]; then MISSING=$((MISSING+1)); MISSING_LIST="${MISSING_LIST}  ${label}: no oracle ref at ${ref}\n"; return; fi
    local exp; exp=$(cat "$ref")
    local slug; slug=$(echo "$label" | tr '/: ' '_')
    local inp_arg; [ -n "$input" ] && [ -f "$input" ] && inp_arg="$input" || inp_arg=""


    # ── Mode 3: --run ──────────────────────────────────────────────────────
    local T0m3=$SECONDS
    local got3
    if [ -n "$inp_arg" ]; then
        got3=$(SNO_LIB="$INC" timeout "$TIMEOUT" "$SCRIP" --run "$sno" < "$inp_arg" 2>/dev/null || true)
    else
        got3=$(SNO_LIB="$INC" timeout "$TIMEOUT" "$SCRIP" --run "$sno" < /dev/null 2>/dev/null || true)
    fi
    [ -n "$filter" ] && got3=$(printf '%s\n' "$got3" | grep -v "$filter" || true)
    T_M3=$((T_M3+SECONDS-T0m3))
    if [ "$got3" = "$exp" ]; then PASS3=$((PASS3+1))
    else FAIL3=$((FAIL3+1)); FAILURES3="${FAILURES3}  FAIL-M3 ${label}\n"; fi

    # ── Mode 4: --compile → assemble → link → run ─────────────────────────
    local T0m4=$SECONDS
    if [ ! -f "$RT_DIR/libscrip_rt.so" ]; then SKIP4=$((SKIP4+1)); return; fi
    local bin="$WORKDIR/${slug}.bin"
    if ! compile_mode4 "$sno" "$bin"; then SKIP4=$((SKIP4+1)); FAILURES4="${FAILURES4}  SKIP(compile/link) ${label}\n"; return; fi
    local got4
    if [ -n "$inp_arg" ]; then
        got4=$(SNO_LIB="$INC" timeout "$TIMEOUT" "$bin" < "$inp_arg" 2>/dev/null || true)
    else
        got4=$(SNO_LIB="$INC" timeout "$TIMEOUT" "$bin" < /dev/null 2>/dev/null || true)
    fi
    [ -n "$filter" ] && got4=$(printf '%s\n' "$got4" | grep -v "$filter" || true)
    T_M4=$((T_M4+SECONDS-T0m4))
    if [ "$got4" = "$exp" ]; then PASS4=$((PASS4+1))
    else FAIL4=$((FAIL4+1)); FAILURES4="${FAILURES4}  FAIL ${label}\n"; fi
}

# ── Crosscheck corpus ──────────────────────────────────────────────────────────
while IFS= read -r sno; do
    ref="${sno%.sno}.ref"
    input="${sno%.sno}.input"
    [ ! -f "$ref" ] && continue
    label=$(basename "$sno" .sno)
    run_test "$label" "$sno" "$ref" "$input" ""
done < <(find "$CORPUS/crosscheck" -name "*.sno" | sort)

# ── Suite families (corpus-suites-consolidation) ────────────────────────────────
# ⭐ A converted family's loose files are gone (the crosscheck loop above simply stops finding
# them once deleted -- no exclusion needed here); this block runs its suite .sno/.ref pair through
# the ONE Python harness (corpus_suite_harness.py) and folds the result into the SAME PASS3/FAIL3/
# PASS4/FAIL4/SKIP4 this board already reports, so "byte-equal before and after" is a statement
# about these same four counters, not a parallel instrument. CRASH/HANG/UNPROVEN at the run stage
# fold into FAIL -- the bucket the old per-file instrument put them in too (it never distinguished
# them either, see RULES.md/FINDING 5ad95ab1); mode-4 compile/link failure folds into SKIP4,
# matching compile_mode4()'s own contract. Missing harness or missing suite files is the SAME
# MISSING/rc=2 loud refusal as a stale hardcoded demo path -- never a silent narrower denominator.
HARNESS="$HERE/corpus_suite_harness.py"
SUITES="$CORPUS/tests/snobol4"
for family in crosscheck/patterns crosscheck/strings crosscheck/gc crosscheck/rung10 crosscheck/keywords crosscheck/functions crosscheck/rung9 crosscheck/rung2 crosscheck/capture crosscheck/output crosscheck/assign crosscheck/arith_new crosscheck/rung8 crosscheck/rung11 crosscheck/control_new crosscheck/data crosscheck/concat crosscheck/rung4 crosscheck/hello crosscheck/rungW04 crosscheck/rungW03 crosscheck/rungW02 crosscheck/rung3 crosscheck/comments crosscheck/rungW07 crosscheck/rungW05 crosscheck/rungW06 crosscheck/rungW01; do
    s_sno="$SUITES/${family}.sno"; s_ref="$SUITES/${family}.ref"
    if [ ! -f "$HARNESS" ]; then
        echo "⛔ GATE REFUSES: corpus_suite_harness.py missing at $HARNESS"; exit 2
    fi
    if [ ! -f "$s_sno" ] || [ ! -f "$s_ref" ]; then
        MISSING=$((MISSING+1)); MISSING_LIST="${MISSING_LIST}  suite:${family}: no suite file at ${s_sno}\n"; continue
    fi
    board=$(python3 "$HARNESS" run "$s_sno" "$s_ref" --modes m3,m4 2>/dev/null | grep '^SUITE_BOARD ')
    if [ -z "$board" ]; then
        MISSING=$((MISSING+1)); MISSING_LIST="${MISSING_LIST}  suite:${family}: harness produced no SUITE_BOARD line\n"; continue
    fi
    field() { echo "$board" | grep -oE "$1=[0-9]+" | cut -d= -f2; }
    m3p=$(field m3_pass); m3f=$(field m3_fail); m3c=$(field m3_crash); m3h=$(field m3_hang); m3u=$(field m3_unproven)
    m4p=$(field m4_pass); m4f=$(field m4_fail); m4c=$(field m4_crash); m4h=$(field m4_hang); m4u=$(field m4_unproven); m4s=$(field m4_skip)
    PASS3=$((PASS3+m3p)); FAIL3=$((FAIL3+m3f+m3c+m3h+m3u))
    PASS4=$((PASS4+m4p)); FAIL4=$((FAIL4+m4f+m4c+m4h+m4u)); SKIP4=$((SKIP4+m4s))
    [ "$((m3f+m3c+m3h+m3u))" -gt 0 ] && FAILURES3="${FAILURES3}  FAIL-M3 suite:${family} (rerun: python3 $HARNESS run $s_sno $s_ref --modes m3)\n"
    [ "$((m4f+m4c+m4h+m4u))" -gt 0 ] && FAILURES4="${FAILURES4}  FAIL suite:${family} (rerun: python3 $HARNESS run $s_sno $s_ref --modes m4)\n"
done

# ── Beauty library drivers ─────────────────────────────────────────────────────
for sno in "$BEAUTY"/*_driver.sno; do
    [ ! -f "$sno" ] && continue
    name=$(basename "$sno" .sno)
    ref="$BEAUTY/${name}.ref"
    run_test "$name" "$sno" "$ref" "" ""
done

# ── Demo programs ─────────────────────────────────────────────────────────────
# Coverage audit (demo-corpus-coverage-audit, 2026-08-22): wordcount's ref/input were
# missing (silent no-op, neither PASS nor FAIL) -- regenerated via x64/bin/sbl -bf.
run_test "demo_wordcount" "$DEMO/wordcount/wordcount.sno" "$DEMO/wordcount/wordcount.ref" "$DEMO/wordcount/wordcount.input" ""
run_test "demo_treebank"  "$DEMO/treebank/treebank.sno"  "$DEMO/treebank/treebank.ref"  "$DEMO/treebank/treebank.input"  ""
run_test "demo_claws5"    "$DEMO/claws5/claws5.sno"    "$DEMO/claws5/claws5.ref"    "$DEMO/claws5/claws5.input"    ""
TIMEOUT=30 \
run_test "demo_roman"     "$DEMO/roman/roman.sno"     "$DEMO/roman/roman.ref"     ""                      "^ms:"
# 15 rows below newly gated by the same audit -- each independently oracle-verified
# (x64/bin/sbl -bf) and scrip-verified before wiring in; see FINDING-2026-08-22-*-demo-corpus-coverage-audit.md
run_test "demo_arithmetic"          "$DEMO/arithmetic/arithmetic.sno"          "$DEMO/arithmetic/arithmetic.ref"          "" ""
run_test "demo_counter"             "$DEMO/counter/counter.sno"             "$DEMO/counter/counter.ref"             "" ""
run_test "demo_hello"               "$DEMO/hello/hello.sno"               "$DEMO/hello/hello.ref"               "" ""
run_test "demo_pattern_test"        "$DEMO/pattern_test/pattern_test.sno"        "$DEMO/pattern_test/pattern_test.ref"        "" ""
run_test "demo_claws5_match"        "$DEMO/claws5/claws5-match.sno"        "$DEMO/claws5/claws5-match.ref"        "$DEMO/claws5/claws5.input"     ""
run_test "demo_claws5_match_fence"  "$DEMO/claws5/claws5-match-fence.sno"  "$DEMO/claws5/claws5-match-fence.ref"  "$DEMO/claws5/claws5.input"     ""
run_test "demo_treebank_match"      "$DEMO/treebank/treebank-match.sno"      "$DEMO/treebank/treebank-match.ref"      "$DEMO/treebank/treebank.input"   ""
run_test "demo_treebank_match_fence" "$DEMO/treebank/treebank-match-fence.sno" "$DEMO/treebank/treebank-match-fence.ref" "$DEMO/treebank/treebank.input" ""
run_test "demo_treebank_alloc"      "$DEMO/treebank/treebank-alloc.sno"      "$DEMO/treebank/treebank-alloc.ref"      "$DEMO/treebank/treebank.input"   ""
run_test "demo_porter"              "$DEMO/porter/porter.sno"              "$DEMO/porter/porter.ref"              "$DEMO/porter/porter.input"     ""
# calculator-1/-2 (full evaluators) print a trailing nondeterministic "match_ms=" timing
# line -- same class as demo_roman's "^ms:" filter above, just a different literal marker.
run_test "demo_calculator_1"        "$DEMO/calculator/calculator-1.sno"        "$DEMO/calculator/calculator-1.ref"        "$DEMO/calculator/calculator.input" "^match_ms="
run_test "demo_calculator_1_match"       "$DEMO/calculator/calculator-1-match.sno"       "$DEMO/calculator/calculator-1-match.ref"       "$DEMO/calculator/calculator.input" ""
run_test "demo_calculator_1_match_fence" "$DEMO/calculator/calculator-1-match-fence.sno" "$DEMO/calculator/calculator-1-match-fence.ref" "$DEMO/calculator/calculator.input" ""
run_test "demo_calculator_2_match"       "$DEMO/calculator/calculator-2-match.sno"       "$DEMO/calculator/calculator-2-match.ref"       "$DEMO/calculator/calculator.input" ""
run_test "demo_calculator_2_match_fence" "$DEMO/calculator/calculator-2-match-fence.sno" "$DEMO/calculator/calculator-2-match-fence.ref" "$DEMO/calculator/calculator.input" ""
run_test "demo_json"                     "$DEMO/json/json.sno"                     "$DEMO/json/json.ref"                     "$DEMO/json/json.input"       ""
run_test "demo_json_match"               "$DEMO/json/json-match.sno"               "$DEMO/json/json-match.ref"               "$DEMO/json/json.input"       ""
run_test "demo_json_match_fence"         "$DEMO/json/json-match-fence.sno"         "$DEMO/json/json-match-fence.ref"         "$DEMO/json/json.input"       ""
# ⭐ s266 -- THE THREE json PROGRAMS ARE UN-SKIPPED. They were excluded on a comment reading "HANGS (m3 AND m4)
# ... needs >30s (currently: forever)" and "wrong verdict on valid JSON". Both cures landed 2026-08-23: the hang
# was multi-choice pattern blobs having no drift-immune choice record and no blob re-entry (SCRIP d6eafac3), and
# the citm-scale stack leak was bare-FENCE0 static release (hq_P a42571b7). Measured at that HEAD, stderr kept
# SEPARATE from stdout: all three PASS in m3 AND m4 against their oracle refs. ⛔ THE SEPARATION IS LOad-BEARING
# and is why this looked broken twice: json/calculator write `match_ms=` to TERMINAL precisely so stdout stays
# byte-comparable, so any harness capturing with 2>&1 merges a timing line into the graded stream and reports a
# DIFF that is pure instrument. hq_C re-made that exact mistake while verifying this un-skip and caught it only
# by diffing against the LIVE oracle, which agreed with scrip byte-for-byte while the .ref appeared not to.
# ⭐ The denominator moves 361 -> 364. A skip is a silent subtraction from the denominator: these three were
# green for hours and no board could say so, because the runner had been told once that they hang forever.
# NOT gated -- each has a one-line reason, full repro in the FINDING above:
#   demo/snobol4/calculator/calculator-2.sno                -- diverges from the live oracle almost immediately (not the match_ms line); real bug, not nondeterminism (no RANDOM/RAND in program or generator)
#   demo/expression.sno                  -- -INCLUDEs 15 files (global.sno, ShiftReduce.sno, Gen.sno, ...) absent from this checkout; won't parse

T_ALL=$((SECONDS-T0_ALL))
TOTAL=$((PASS4+FAIL4+SKIP4))
echo "mode-3 (--run):     PASS=$PASS3 FAIL=$FAIL3"
echo "mode-4 (--compile): PASS=$PASS4 FAIL=$FAIL4 SKIP=$SKIP4  ($TOTAL total)"
[ -n "$FAILURES3" ] && printf "$FAILURES3" | head -20
[ -n "$FAILURES4" ] && printf "$FAILURES4" | head -40

printf "TIME M3=%ds M4=%ds TOTAL=%ds\n" "$T_M3" "$T_M4" "$T_ALL"

# ⛔⛔ THE VERDICT IS AN EXIT CODE, NOT A PRINTED NUMBER (hq_C s272). Until now this script's last statement was
# the printf above, so it exited 0 with any number of mode-4 failures -- the SAME false-green shape as `make test`,
# sitting inside the blocking set itself. CLAUDE.md has said "mode-4 is the hard gate" throughout; nothing enforced
# it. m3 stays informational per that documented contract, but it is printed in the verdict so it cannot hide.
if [ "$MISSING" -gt 0 ]; then
    echo "⛔ GATE REFUSES: $MISSING hardcoded corpus path(s) no longer resolve -- the board is SMALLER than it looks:"
    printf "$MISSING_LIST"
    # ⛔⭐⭐ WHY THIS BLOCK NOW DIAGNOSES BEFORE IT ADVISES (hq_P 2026-08-27 s276, after THREE seats in ONE DAY).
    # This refusal used to end with the flat instruction "Repoint them". ⛔ THAT NAMED THE WRONG REMEDY FOR THE
    # DOMINANT CAUSE, AND THE WRONG REMEDY IS THE IRREVERSIBLE ONE. A suite family lands as a CROSS-REPO change --
    # the corpus commit carrying <family>.{sno,ref} and the SCRIP commit adding it to the family list -- and there
    # is no atomic commit across two repos, so for a window any checkout that pulled one and not the other sees
    # paths that "do not resolve". The correct action there is `git pull` in corpus. REPOINTING OR RETIRING THE
    # ENTRY INSTEAD PERMANENTLY SHRINKS THE BOARD, and the gate was the thing telling seats to do it: seat12
    # proposed retiring two live families, seat11 proposed repointing seven, and a third report reached seat08
    # secondhand as a phantom regression -- all in one day, all steered by this message.
    # ⭐ THE FIX IS TO ASK THE QUESTION THE SEAT CANNOT: does the path exist at origin? If yes the checkout is
    # STALE; if no it is genuinely gone. Same file, same symptom, opposite cures -- so the gate must not guess.
    # ⛔ AND IT MUST DEGRADE TO "UNDETERMINED", NEVER TO THE DESTRUCTIVE ADVICE: with no origin, no network, or a
    # failed fetch we print both possibilities and recommend NOTHING. An instrument that cannot measure refuses.
    _stale=0; _gone=0; _undet=0
    if [ -d "$CORPUS/.git" ] && git -C "$CORPUS" rev-parse --verify -q origin/main >/dev/null 2>&1; then
        timeout 20s git -C "$CORPUS" fetch -q origin 2>/dev/null || true
        for _p in $(printf "$MISSING_LIST" | grep -oE "$CORPUS/[^ ]+" | sort -u); do
            _rel="${_p#$CORPUS/}"
            if git -C "$CORPUS" cat-file -e "origin/main:$_rel" 2>/dev/null; then _stale=$((_stale+1)); else _gone=$((_gone+1)); fi
        done
    else _undet=1; fi
    _behind="$(git -C "$CORPUS" rev-list --count HEAD..origin/main 2>/dev/null || echo '?')"
    if [ "$_stale" -gt 0 ]; then
        echo "   ⛔⭐ DIAGNOSIS: YOUR CORPUS CHECKOUT IS STALE -- $_stale of the unresolved path(s) EXIST at origin/main"
        echo "      (corpus is $_behind commit(s) behind origin). ⛔ DO NOT REPOINT AND DO NOT RETIRE THESE ENTRIES:"
        echo "      they are live files and the entries are correct. Repointing would shrink the board permanently."
        echo "      ✅ FIX:  git -C $CORPUS pull --ff-only     then re-run this gate."
        [ "$_gone" -gt 0 ] && echo "      ⚠ $_gone other path(s) are absent at origin too -- treat those separately, see below."
    fi
    if [ "$_gone" -gt 0 ] && [ "$_stale" -eq 0 ]; then
        echo "   DIAGNOSIS: $_gone path(s) are absent at origin/main too, so this is NOT a stale checkout."
        echo "      Repoint or retire those family entries; do NOT read the shrunken total as a pass."
    fi
    if [ "$_undet" -eq 1 ]; then
        echo "   ⚠ UNDETERMINED: no corpus origin/main to compare against, so this gate CANNOT tell a stale checkout"
        echo "      from a genuinely retired suite. ⛔ Recommending nothing -- establish which it is before changing anything."
    fi
    echo "   FAIL=0 over a shrunken denominator is not green."
    exit 2
fi
if [ "$FAIL4" -gt 0 ]; then echo "⛔ GATE FAIL: mode-4 FAIL=$FAIL4 (mode-3 FAIL=$FAIL3, informational)"; exit 1; fi
echo "✅ GATE OK: m3 PASS=$PASS3 FAIL=$FAIL3 · m4 PASS=$PASS4 FAIL=$FAIL4 SKIP=$SKIP4 · MISSING=0"
exit 0
