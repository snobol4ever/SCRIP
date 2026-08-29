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
    # ⛔ -no-pie is NOT applied here, deliberately -- RULED, not blocked: row
    # `m4-pie-vs-no-pie-changes-behaviour-not-just-signal` (seat10 2026-08-28), full analysis in
    # .github/ARCH-ENGINE.md § "Mode-4 Link Mode". gdb on the faulting RSP showed -no-pie is the broken arm, not
    # a measurement artifact: fz_red_m2a_fence_cap_gen / fz_segv_10 are .ref="match" (not crash-expected), PIE
    # runs them clean 20/20, -no-pie SIGSEGVs them 20/20 with RSP==0x0 at fault -- a normal stack address at the
    # identical point under PIE. See corpus_suite_harness.py's compile_m4() for the mirrored banner.
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
done < <(find "$CORPUS/crosscheck" -name "*.sno" 2>/dev/null | sort)
# ⛔⭐ THE LOOSE CROSSCHECK TREE IS GONE, AND ITS ABSENCE HAS TWO CAUSES THAT LOOK IDENTICAL HERE
# (hq_B 2026-08-29, on seat16's report of `find: corpus/crosscheck: No such file or directory`).
#   (a) CONVERTED -- every loose file became a suite family under $CORPUS/tests/snobol4/crosscheck, which the
#       suite block below discovers and grades. Correct end state; the loop above SHOULD find nothing.
#   (b) MOVED/RENAMED -- the layout shifted again and this path is simply stale, in which case the board loses
#       the whole subtree and still prints FAIL=0.
# The $DEMO/$BEAUTY guard above already refuses (b) for its two subtrees, with the right words: "A clean
# numerator over a shrunken denominator is the most dangerous shape a board has, and corpus paths have moved
# three times in two days, so this WILL happen again." It happened again, to the ONE subtree not in that list.
# ⛔ So crosscheck gets the same refusal, conditioned on the distinguisher the other two do not need: absence of
# the loose tree is acceptable ONLY when the converted families are actually present to be graded instead.
if [ ! -d "$CORPUS/crosscheck" ]; then
    _cc_conv="$CORPUS/tests/snobol4/crosscheck"
    if [ -d "$_cc_conv" ] && [ "$(find "$_cc_conv" -name '*.sno' 2>/dev/null | wc -l)" -gt 0 ]; then
        echo "note: loose crosscheck tree is absent and its converted families ARE present ($(find "$_cc_conv" -name '*.sno' | wc -l) family file(s) at ${_cc_conv#$S4E/}) — graded by the suite block below, not skipped."
    else
        echo "⛔ GATE REFUSES: corpus subtree missing: $CORPUS/crosscheck"
        echo "   AND no converted crosscheck families exist at $_cc_conv either, so this board would grade a"
        echo "   NARROWER DENOMINATOR and still print FAIL=0. The corpus layout moved. Repoint this script;"
        echo "   do NOT read a smaller total as a pass."
        exit 2
    fi
fi

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
# ⭐⭐ FAMILIES ARE DISCOVERED FROM THE TREE, NEVER HAND-MAINTAINED (two seats converged on this the same day --
# rows crosscheck-families-filesystem-truth + corpus-suite-family-list-should-autodiscover -- both curing hq_P's
# s277 measurement: a hand-typed for-loop enumeration here let 98 entries -- 19 converted families -- vanish
# from the board silently, GATE OK, while m3/m4 totals fell 365 -> 267. A typed list is a SECOND SOURCE OF TRUTH
# about what exists on disk and it drifts silently in the direction that looks like success. THE CURE: every
# *.sno with a sibling *.ref directly under $SUITES/crosscheck IS a family -- nothing is named, so families are
# now discovered from the tree and nothing can be forgotten when a new one lands.
# ⛔ Scoped to crosscheck/ specifically, not all of $SUITES: beauty_suite/ already has its own driver-pair loop
# a few lines below and would be DOUBLE-COUNTED by a wider glob (its *_driver.sno/.ref pairs match the identical
# by-basename rule). A separate top-level-misc block further below (after the probe block) covers families born
# directly at $SUITES/*.sno (e.g. feat.sno) with its own independent floor -- deliberately NOT folded into this
# loop or its floor, so a crosscheck-only regression and a top-level-misc regression are never conflated into one
# number. parser/smoke/jvm_j3/linker are NOT suite-format families at all (see that block's own comment).
# ⛔ DISCOVERY ALONE CANNOT SEE A FAMILY THAT WENT MISSING (fewer names discovered looks identical to fewer names
# that exist) -- covers the ADDITION direction (a new pair needs no registration, and
# test_gate_crosscheck_family_list_autodiscovers.sh proves that direction hermetically) but not the DELETION
# direction, so a FLOOR stands guard below: a discovered count under it SHRINK-REFUSES through the same
# MISSING/stale-vs-gone diagnosis this script already runs for a stale hardcoded demo path, rather than
# reporting a smaller total as green (TWO-PART PROOF, RULES.md -- the criterion must be able to say NO both ways).
# FLOOR PROVENANCE: 29 pairs, counted directly off $SUITES/crosscheck this commit. This is a FLOOR, not a
# pinned total (RULES.md: a probe never asserts a denominator) -- growth needs no re-pin, ever. Only a
# genuine, intentional retirement may lower this number, in the SAME commit that removes the pair -- exactly
# what the old list required of an ADDITION, now required only of a REMOVAL.
SUITE_FAMILY_FLOOR="${SUITE_FAMILY_FLOOR:-29}"
suite_found=0
while IFS= read -r s_sno; do
    family="crosscheck/$(basename "$s_sno" .sno)"
    s_ref="${s_sno%.sno}.ref"
    if [ ! -f "$HARNESS" ]; then
        echo "⛔ GATE REFUSES: corpus_suite_harness.py missing at $HARNESS"; exit 2
    fi
    if [ ! -f "$s_ref" ]; then
        MISSING=$((MISSING+1)); MISSING_LIST="${MISSING_LIST}  suite:${family}: no oracle ref at ${s_ref}\n"; continue
    fi
    suite_found=$((suite_found+1))
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
done < <(find "$SUITES/crosscheck" -maxdepth 1 -name "*.sno" 2>/dev/null | sort)
if [ "$suite_found" -lt "$SUITE_FAMILY_FLOOR" ]; then
    suite_family_shortfall=$((SUITE_FAMILY_FLOOR-suite_found))
    MISSING=$((MISSING+suite_family_shortfall))
    MISSING_LIST="${MISSING_LIST}  suite-family-count: discovered ${suite_found} pairs under corpus/tests/snobol4/crosscheck, pinned floor is ${SUITE_FAMILY_FLOOR} (short by ${suite_family_shortfall}) -- a family vanished from the tree, or this checkout is behind origin\n"
fi

# ── Suite families (probe/ house-cleaning fan-out) ──────────────────────────────
# ⭐ Same discovered-not-hand-maintained mechanism as the crosscheck block above, generalized to a
# second source directory: $SUITES/probe. The 2026-08-28 house-cleaning fan-out
# (corpus-suites-consolidation's probe-consolidate-* rows) converts probe/<family> loose-file trees
# into suite pairs under tests/snobol4/probe/<family>.{sno,ref}; this loop discovers them the same
# filesystem-is-truth way, so no sibling fan-out row needs its own hand-registration step here -- the
# exact landmine hq_C raised to rank 0 (row corpus-suite-family-list-should-autodiscover) after corpus
# 0e75bfdb's 98-entry silent-invisibility incident, generalized before a SECOND fan-out (~2,300 files,
# 11 concurrent rows) repeats it. First tenant: tests/snobol4/probe/conformance (probe-consolidate-
# conformance, 92 of 147 probe/conformance witnesses; the other 55 stay loose, see that directory's
# own KEEP.md -- 17 still diverge from the live oracle, 38 are cited by name in an existing task file).
while IFS= read -r family; do
    s_sno="$SUITES/probe/${family}.sno"; s_ref="$SUITES/probe/${family}.ref"
    if [ ! -f "$HARNESS" ]; then
        echo "⛔ GATE REFUSES: corpus_suite_harness.py missing at $HARNESS"; exit 2
    fi
    if [ ! -f "$s_sno" ] || [ ! -f "$s_ref" ]; then
        MISSING=$((MISSING+1)); MISSING_LIST="${MISSING_LIST}  suite:probe/${family}: no suite file at ${s_sno}\n"; continue
    fi
    board=$(python3 "$HARNESS" run "$s_sno" "$s_ref" --modes m3,m4 2>/dev/null | grep '^SUITE_BOARD ')
    if [ -z "$board" ]; then
        MISSING=$((MISSING+1)); MISSING_LIST="${MISSING_LIST}  suite:probe/${family}: harness produced no SUITE_BOARD line\n"; continue
    fi
    field() { echo "$board" | grep -oE "$1=[0-9]+" | cut -d= -f2; }
    m3p=$(field m3_pass); m3f=$(field m3_fail); m3c=$(field m3_crash); m3h=$(field m3_hang); m3u=$(field m3_unproven)
    m4p=$(field m4_pass); m4f=$(field m4_fail); m4c=$(field m4_crash); m4h=$(field m4_hang); m4u=$(field m4_unproven); m4s=$(field m4_skip)
    PASS3=$((PASS3+m3p)); FAIL3=$((FAIL3+m3f+m3c+m3h+m3u))
    PASS4=$((PASS4+m4p)); FAIL4=$((FAIL4+m4f+m4c+m4h+m4u)); SKIP4=$((SKIP4+m4s))
    [ "$((m3f+m3c+m3h+m3u))" -gt 0 ] && FAILURES3="${FAILURES3}  FAIL-M3 suite:probe/${family} (rerun: python3 $HARNESS run $s_sno $s_ref --modes m3)\n"
    [ "$((m4f+m4c+m4h+m4u))" -gt 0 ] && FAILURES4="${FAILURES4}  FAIL suite:probe/${family} (rerun: python3 $HARNESS run $s_sno $s_ref --modes m4)\n"
done < <(find "$SUITES/probe" -name '*.sno' 2>/dev/null | sort | while IFS= read -r s; do
    b="${s%.sno}"; [ -f "$b.ref" ] && printf '%s\n' "${b#"$SUITES"/probe/}"
done)

# ── Suite families (top-level misc, e.g. feat) ───────────────────────────────────
# ⭐ Same discovered-not-hand-maintained mechanism as crosscheck/probe above, generalized to a third
# source: $SUITES itself at maxdepth 1 -- a family born directly as tests/snobol4/<name>.sno (not nested
# under crosscheck/ or probe/), reading with an UNPREFIXED name. maxdepth 1 structurally cannot reach
# beauty_suite/'s contents (one directory down, see beauty_suite/KEEP.md for why it never converts at
# all) or crosscheck's/probe's own contents (also one directory down), so no separate exclusion is
# needed. parser/smoke/jvm_j3 have zero .ref siblings today -- they are graded LIVE against the oracle by
# scorecard_snobol4.sh's MISC_DIRS, a different grading model this harness does not implement; linker/
# tests cross-file IMPORT/EXPORT linking, which the suite format's one-file-per-entry model cannot
# represent. None of those four are discovered here (task tests-consolidate-snobol4-loose's ledger has
# the full disposition). First tenant: tests/snobol4/feat.sno (16 of feat/'s 19 pairable programs; 3 left
# loose as documented pre-existing reds, 2 for having no .ref at all).
# ⭐ FLOOR, same shape and same reasoning as crosscheck's above -- own counter, own floor, deliberately not
# merged with crosscheck's suite_found/SUITE_FAMILY_FLOOR so the two regressions are never conflated.
MISC_SUITE_FAMILY_FLOOR="${MISC_SUITE_FAMILY_FLOOR:-1}"
misc_suite_found=0
while IFS= read -r family; do
    s_sno="$SUITES/${family}.sno"; s_ref="$SUITES/${family}.ref"
    if [ ! -f "$HARNESS" ]; then
        echo "⛔ GATE REFUSES: corpus_suite_harness.py missing at $HARNESS"; exit 2
    fi
    if [ ! -f "$s_ref" ]; then
        MISSING=$((MISSING+1)); MISSING_LIST="${MISSING_LIST}  suite:${family}: no oracle ref at ${s_ref}\n"; continue
    fi
    misc_suite_found=$((misc_suite_found+1))
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
done < <(find "$SUITES" -maxdepth 1 -name '*.sno' 2>/dev/null | sort | while IFS= read -r s; do
    b="${s%.sno}"; [ -f "$b.ref" ] && printf '%s\n' "${b#"$SUITES"/}"
done)
if [ "$misc_suite_found" -lt "$MISC_SUITE_FAMILY_FLOOR" ]; then
    misc_shortfall=$((MISC_SUITE_FAMILY_FLOOR-misc_suite_found))
    MISSING=$((MISSING+misc_shortfall))
    MISSING_LIST="${MISSING_LIST}  misc-suite-family-count: discovered ${misc_suite_found} pairs directly under corpus/tests/snobol4, pinned floor is ${MISC_SUITE_FAMILY_FLOOR} (short by ${misc_shortfall}) -- a family vanished from the tree, or this checkout is behind origin\n"
fi

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
run_test "demo_calculator_2"        "$DEMO/calculator/calculator-2.sno"        "$DEMO/calculator/calculator-2.ref"        "$DEMO/calculator/calculator.input" "^match_ms="
# OUTPUT() association to a '[-fn]' descriptor spec, routed by hq_P 2026-08-28: every write to the
# associated variable was silently discarded in BOTH modes (rc=0, ordinary output intact), because
# _OUTPUT_ never called the -fn parser that _INPUT_ had been calling all along. Graded on -f1 rather
# than -f2 because this harness compares stdout; the defect was never fd2-specific.
run_test "feat_io_fd_assoc"        "$CORPUS/tests/snobol4/feat/f21_io_fd_assoc.sno"        "$CORPUS/tests/snobol4/feat/f21_io_fd_assoc.ref"        "" ""
# k41: the IMMEDIATE pattern lambda, the arm that is LANDED (row lang-lambda-pattern-primitives).
# ⛔ NOT k40. k40 is the row's INSTRUMENT, authored before the cure and RED ON PURPOSE until the
# whole feature lands (conditional lambda, backtrack-unqueue, stored-pattern round trip are all
# still unlanded) -- wiring THAT into the board would turn a deliberate measurement into a broken
# gate. A red instrument cannot also be a regression guard, so k41 guards what works and k40 keeps
# measuring the distance left. ⛔ SELF-PINNED, never sbl-graded: SPITBOL cannot even LEX the file --
# sbl -bf dies at the Greek line with ERROR 230 "illegal character" -- so no oracle arm exists.
run_test "k41_lambda_immediate"    "$CORPUS/probe/conformance/k41_lambda_immediate_landed.sno"    "$CORPUS/probe/conformance/k41_lambda_immediate_landed.ref"    "" ""
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
# ⛔⭐ STAMP THE TREE ON THE BOARD ITSELF (seat09's ask, 2026-08-29, from a live disagreement).
# seat09 and hq_B both ran THIS SCRIPT and got different denominators -- 1339 and 1377 -- each reproducible
# on demand in its own root. Neither number is stale and neither of us could settle it, because the output
# said what was measured but not WHAT TREE IT WAS MEASURED ON. A denominator is a fact about a corpus, and
# this board folds DEMO + BEAUTY + crosscheck families + misc from a SEPARATE REPO whose commit the SCRIP
# hash does not pin: two roots can sit at the same SCRIP commit and different corpus commits and disagree
# by 38 programs with nothing in either transcript to show it.
# ⭐ lib_gate.sh already stamps per-repo HEAD + dirty/clean + machine on every gate verdict, but THE BOARD
# THAT MATTERS MOST DID NOT SOURCE IT -- the instrument most often quoted in commit messages was the one
# with no provenance. Sourced tolerantly: if the lib is unreachable the board still runs, because a missing
# stamp must never cost a measurement.
if . "$HERE/lib_gate.sh" 2>/dev/null && command -v gate_stamp >/dev/null 2>&1; then gate_stamp
else echo "    (tree stamp unavailable — lib_gate.sh not sourced; record SCRIP and corpus HEAD by hand when quoting these numbers)"; fi

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
