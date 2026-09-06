#!/usr/bin/env bash
# test_snobol4_csnobol4_suite.sh — Phil Budne's CSNOBOL4 test suite, graded against its OWN oracle.
#
# Suite: corpus/packages/snobol4/csnobol4_suite (vendored, unmodified). A NAME.sno with a sibling NAME.ref
# is a graded pair; a NAME.sno with no .ref (e.g. bench.sno, a support/data file) is not a pair and is
# skipped by construction — never a hand-curated exclusion list.
#
# ⛔ THE ONE NAMED EXCEPTION — EXCLUDED_TESTS below (hq_P ruling, msg re-csnobol4-thirty-final-report,
# 2026-09-04, closing row snobol4-csnobol4-thirty-regen-candidate-refs-stale-pin-or-real-defect): ndbm,
# random, sleep, time each `-include` a modules/X/X.sno that dlopen()s ./X.so relative to CWD
# (lib/unix98/load.c:os_load_library) — physically unrunnable from this suite's flattened layout no matter
# what gets vendored. Upstream's OWN test/tests.in (verified directly, not taken on faith) carries these
# `reg` lines commented out: ndbm line 118 ("11/16/2005; off 10/22/2020 (moved to module)"), time line 122
# ("12/16/2010; off 10/25/2020 (moved to module)"), sleep line 130 and random line 132 (also `#`-commented
# in the same file; a specific disablement date is not legible for these two, unlike ndbm/time — do not
# overclaim one). Upstream itself does not run these four; neither do we — in THIS flattened layout.
#
# ⭐ MODULE-REPLACEMENT COVERAGE (landed here, row snobol4-csnobol4-module-replacement-coverage-ndbm-random-time,
# seat08 2026-09-05): Budne's own per-module self-tests — modules/{ndbm,random,time}/test.sno+test.ref and
# modules/time/test2.sno+test2.ref (test2 is literally "formerly test/time.sno", the time/date-family test;
# test.sno in the time module IS the sleep-family equivalent — there is no modules/sleep/ directory, SLEEP()
# ships as part of the "time" module, confirmed by reading modules/time/time.sno's own
# LOAD("SLEEP(REAL)", TIME_DL) line) — run from THEIR OWN directory below as MODULE_TESTS (never the flattened
# $SUITE copy, exactly because they dlopen ./X.so relative to cwd), folded into the SAME TOTAL/RED-M3/RED-M4
# counters as every other pair: no per-op exception list, a defect reached through an admitted family member
# counts like any other (RULES.md). ⛔ MEASURED, NOT ASSUMED GREEN: all four are RED against SCRIP in BOTH
# modes, for two already-out-of-scope, pre-existing reasons, never a fixture problem — the live csnobol4
# oracle passes all four byte-exact from these same directories, confirming the fixtures and this harness's
# plumbing, not the coverage itself, is what's new here. (1) ndbm and time/test2 hit the SAME gap: SCRIP's
# LOAD() (src/runtime/core/core.c, _b_LOAD_stub) recognizes only its own internal "MON_"-prefixed monitor
# hooks and FAILDESCRs everything else — there is no dlopen() anywhere in src/ (grepped clean). CSNOBOL4's
# dynamic external-C-module loading is simply unimplemented. ndbm's own FUNCTION("DBM_OPEN"):F(END) guard
# turns that into a graceful empty-output FAIL; time/test2 calls the LOAD'd GETTIMEOFDAY_ unconditionally and
# gets "Error 5: undefined function or operation" (REJECT). (2) random and time/test (the sleep equivalent)
# separately hit the PRE-EXISTING, ALREADY-DOCUMENTED lowercase-`end` dialect class named in the DIALECT
# paragraph below (case-sensitive SCRIP vs. CSNOBOL4's case-insensitive keywords) — "missing END statement"
# at a parse stage that never reaches LOAD at all. Fixing either gap is out of scope for a coverage-wiring
# row; see FINDING-2026-09-05-seat08-scrip-has-no-load-dlopen-support-confirmed-via-module-coverage.md.
#
# ⛔ PARKED_NO_REGEN below (same ruling) — breakline, k, rewind1, genc: live csnobol4 itself disagrees with
# its OWN historical .ref here (K-format fixed-record I/O looks broken; rewind1 SIGSEGVs on REWIND(5); genc
# now dies at statement 568 with "Error 24, Undefined or erroneous goto", a failure mode never seen before
# in this row), so the REGEN-CANDIDATE tiebreak (below) would misfile a live-oracle regression as "our .ref
# might be stale." ceo ruling (task ledger, snobol4-csnobol4-thirty-regen-candidate-refs-stale-pin-or-real-
# defect #28) covers breakline/k/rewind1; hq_P extended it to genc (msg re-csnobol4-genc-second-gap-and-a-
# third-mystery, 2026-09-04): do NOT re-cut any of the four .refs; grade plainly against csnobol4 and let
# them stay red until the oracle itself is fixed. They still count as ordinary RED (RED-M3/RED-M4,
# FAIL/REJECT/CRASH) — PARKED only removes them from the "maybe-stale-pin" bucket, it does not paper over
# the red. THERE IS NO XFAIL.
#
# ⛔ THE ORACLE IS CSNOBOL4, NOT sbl — `sbl -bf` false-reds on 30 of these ~120 programs (CSNOBOL4-only
# extensions SPITBOL never claims: ORD, &DUMP, popen, ...; RULES.md FACT RULE s261). Primary grading is
# byte-exact against the vendored .ref; the live csnobol4 binary (csnobol4_bin(), lib_oracle_flags.sh) is
# run as a full third arm (informational, same shape as test_snoflake_suite.sh's triangulation arm) AND as
# the TIEBREAK/REGENERATION authority: when SCRIP's mode-3 output disagrees with .ref, "does live csnobol4
# ALSO disagree with .ref?" — if so the .ref pin itself is the suspect, flagged REGEN-CANDIDATE rather than
# folded into the SCRIP fail count. Invoked with `-b` (suppress startup banner), matching Budne's own
# test/run.sh convention (`ARGS="$IARGS -b"`).
#
# ⭐ NORMALIZATION, sourced directly from Budne's own test/tests.in + test.dump.sh + test.trace.sh (the
# suite author's OWN definition of a matching DUMP/TRACE test, not a guess): 4 `dump`-type programs
# (a, dump, diag1, diag2 — &DUMP=1 output) mask `MAXLNGTH = NNN` -> `MAXLNGTH = xxx` and drop `&FILL = '..'`
# lines (both environment/build-dependent, not semantic); 4 `trace`-type programs (ftrace, spit, trace1,
# trace2 — TRACE() output) mask a trailing `time = N.NN` -> `time = xxx` (wall-clock, inherently
# nondeterministic). Applied identically to SCRIP's output, live csnobol4's output, AND the .ref before
# comparison — normalizing noise, not favoring a side. No other test type is touched.
#
# Ladder (per mode, m3 --run / m4 --compile+link+run), decided in this order:
#   HANG    killed by the timeout (rc=124)
#   CRASH   killed by a fatal signal (rc>=128) — or, m4 only, compile/link itself failed ("(CC)" suffix)
#   PASS    output byte-identical to .ref
#   REJECT  output differs, rc!=0 (parse/runtime refusal — didn't reach a normal exit)
#   FAIL    output differs, rc=0 (ran to completion, wrong answer)
#
# ⚠ DIALECT: this suite targets CSNOBOL4, not SPITBOL; SCRIP follows SPITBOL semantics (RULES.md). Known
# dialect-distance classes (lowercase `end`, missing -INCLUDE targets, CSNOBOL4-only builtins) are NOT
# filtered out here — see FINDING-2026-08-27-seat06-csnobol4_suite-triage-eight-classes-three-are-not-scrip-bugs.md
# for the read on which reds are confirmed SCRIP defects vs. corpus/dialect mismatches. This runner reports
# every red by name and leaves classification to that document and follow-on cure rows.
#
# Stdin convention (Budne's own, matching test_csnobol4_budne_suite.sh's STDIN_TESTS list): these programs
# carry their test data after their own END statement; split at the first bare "END" line, feed the tail
# as stdin, and run the head as the program.
#
# ⛔ SETUP_DEP (hq_P ruling above) — openo2 reads back openo.tst, a fixture openo.sno itself writes; upstream
# ran both sequentially in one shared dir (test/run.sh: `reg openo.sno` then `reg openo2.sno`, adjacent and
# ordered — upstream DESIGN, not an accident). This runner's own glob order (`for sno in "$SUITE"/*.sno`,
# locale-collated, NOT byte order) happens to visit openo2.sno BEFORE openo.sno, so without a declared setup
# step openo2 always read the fixture in its pristine vendored-empty state — measured directly, not guessed:
# a byte-for-byte repro outside this harness proves SCRIP's OUTPUT/INPUT association is correct as soon as
# openo runs first. Per-test isolation (comment above, ⛔ ALL arms run with cwd in a scratch dir) stays ruled;
# this is a one-time fixture-priming call, not a re-opening of that isolation.
#
# ⛔ ARGV_FOR (hq_P ruling above) — genc.sno needs `genc.sno v311.sil` (its own .ref header: "generated from
# v311.sil"); HOST(2,N)/HOST(3) (src/runtime/core/core.c) already index the REAL process argv correctly in
# m3, m4, and csnobol4 alike (verified directly against all three arms before wiring this in) — the runner
# just never passed a program argv through. One name, one extra token; extend the map if another test needs it.
#
# ⛔ ALL arms run with cwd in a scratch dir, never in the corpus tree — an OUTPUT unit-I/O association can
# create OR OVERWRITE files named by its argument in the cwd (SPITBOL/CSNOBOL4 dialect). The whole suite is
# COPIED (never symlinked) into the scratch run dir first: a same-directory `-INCLUDE "sibling.sno"` (e.g.
# line.sno -> line2.sno) still resolves cwd-relatively, but a write-target fixture (e.g. openo.tst, a
# pre-committed empty companion file openo.sno's file-I/O test writes into) lands on the scratch copy, never
# the tracked original. ⭐ MEASURED, THE HARD WAY (seat12, same session): the first cut of this script used
# symlinks here, and running it wrote real content into two tracked corpus fixtures (openo.tst, test.bin)
# through the symlink — reverted, cured to a copy before this landed. Never symlink a suite directory a
# program under test might open for writing.
#
# ⛔ A missing suite, compiler, RT, or the csnobol4 oracle REFUSES with rc=2 — never a silent skip-as-success.
# Exit: 0 iff FAIL+REJECT+CRASH+HANG == 0 in BOTH modes over the printed denominator (now includes the 4
# module/* coverage rows added below — measured RED today for reasons unrelated to this denominator, see
# the MODULE-REPLACEMENT COVERAGE paragraph further down).
set -u
HERE="$(cd "$(dirname "$0")" && pwd)"; SD="$HERE/.."; ROOT="$(cd "$SD/.." && pwd)"
. "$HERE/lib_flag_gate.sh" 2>/dev/null || { echo "⛔ REFUSE(rc=2): lib_flag_gate.sh unloadable"; exit 2; }
[ $# -eq 0 ] || flaggate_reject "$1" "(none -- set CSNOBOL4_SUITE / TIMEOUT via environment instead)"
SUITE="${CSNOBOL4_SUITE:-$ROOT/corpus/packages/snobol4/csnobol4_suite}"
SCRIP="$SD/scrip"; RT_DIR="$SD/out"; TIMEOUT="${TIMEOUT:-8}"

[ -d "$SUITE" ] || { echo "⛔ REFUSE(rc=2): suite dir missing: $SUITE"; exit 2; }
[ -x "$SCRIP" ] || { echo "⛔ REFUSE(rc=2): no scrip binary at $SCRIP — build first (make)"; exit 2; }
# ⛔⭐ STALE-BINARY PREFLIGHT (row harness-and-ladder-runner-refuse-on-a-stale-binary-like-the-artifact-regen-
# does, ceo -> hq_T 2026-09-04). The line above proves a binary EXISTS; this one proves it is the binary this
# tree describes. ceo's witness, twice on 2026-09-04: a 10:57 binary graded at 14:03 read RED, then GREEN after
# an incremental make -- a vendor board is exactly where that is least visible, because a plausible all-FAIL
# table is this class's normal output. NO LOGIC HERE: util_require_fresh.sh sources gate_require_fresh from
# lib_gate.sh, the ONE authority (hq_B 4c7253e99) -- never a second copy of the staleness rule.
"$HERE/util_require_fresh.sh" --gate test_snobol4_csnobol4_suite "$SCRIP" "${RT_DIR:-$HERE/../out}/libscrip_rt.so" || exit 2
[ -f "$RT_DIR/libscrip_rt.so" ] || { echo "⛔ REFUSE(rc=2): no $RT_DIR/libscrip_rt.so"; exit 2; }
# ⛔⭐ THE DIALECT SWITCH (ceo RULING R1, GOAL-SNOBOL4-100 § RULINGS 2026-09-04, row snobol4-csnobol4-dialect-
# compat-switch; spelled like Lon's Prolog precedent --compat=swi|gnu). THIS SUITE IS GRADED BY CSNOBOL4, NOT
# SPITBOL, and its refs are CSNOBOL4's own output -- so it runs under --compat=csnobol4 while the SPITBOL-minted
# SNOBOL4 master keeps the default. The switch ADDS CSNOBOL4-only behaviour; the default never widens.
# TODAY'S ONE MEMBER: the SETEXIT trap also fires on NORMAL TERMINATION when &ERRLIMIT is non-zero. SPITBOL fires
# in none of four measured shapes, CSNOBOL4 fires in both &ERRLIMIT-non-zero shapes (hq_P 2026-09-04), and
# setexit2.sno is the entry that turns on it.
# ⛔⭐ BOTH CARRIERS ARE REQUIRED AND THAT IS NOT BELT-AND-BRACES -- IT IS WHAT A *RUNTIME* DIALECT MEANS. The flag
# configures the DRIVER, so it reaches mode-3 (in-process) and any hand run. A mode-4 program is a SEPARATE PROCESS
# started later from a linked binary, and no compile-time flag can reach its runtime; the env var is what the
# standalone binary reads. A runner that passed only the flag would grade m3 under CSNOBOL4 and m4 under SPITBOL
# and report the split as a mode divergence -- the wrong answer in the shape hardest to attribute.
COMPAT="--compat=csnobol4"
export SCRIP_SETEXIT_END=1
. "$HERE/lib_oracle_flags.sh" 2>/dev/null || { echo "⛔ REFUSE(rc=2): lib_oracle_flags.sh unloadable"; exit 2; }
. "$HERE/lib_inventory.sh" 2>/dev/null || { echo "⛔ REFUSE(rc=2): lib_inventory.sh unloadable"; exit 2; }
. "$HERE/lib_progress.sh" 2>/dev/null || { echo "⛔ REFUSE(rc=2): lib_progress.sh unloadable -- a run that leaves the progress table untouched is the same defect as one that leaves its SCORE row untouched (ceo CEO-330/331)"; exit 2; }
CSN="$(csnobol4_bin)" || exit 2
CSN_SRC="$(dirname "$CSN")"   # module-coverage source root: the shared csnobol4 tree's own modules/ dir (see header)

STDIN_TESTS="atn crlf longrec rewind1 sudoku trim0 trim1 uneval2 factor len repl tab words words1"
is_stdin_test() { local n="$1" s; for s in $STDIN_TESTS; do [ "$n" = "$s" ] && return 0; done; return 1; }
DUMP_TESTS="a dump diag1 diag2"; TRACE_TESTS="ftrace keytrace spit trace1 trace2 trfunc t"
EXCLUDED_TESTS="ndbm random sleep time"
is_excluded_test() { local n="$1" s; for s in $EXCLUDED_TESTS; do [ "$n" = "$s" ] && return 0; done; return 1; }
# MODULE-REPLACEMENT COVERAGE for the four EXCLUDED_TESTS above — see header. name:module-subdir:sno:ref.
MODULE_TESTS="ndbm:ndbm:test.sno:test.ref random:random:test.sno:test.ref sleep:time:test.sno:test.ref time:time:test2.sno:test2.ref"
# Like compile_m4 below, but cwd-relative with no SNO_LIB override: these -INCLUDEs are bare same-directory
# names (e.g. 'ndbm.sno'), resolved by running with cwd already in the test's own directory — measured
# directly (seat08 2026-09-05) against a real invocation before wiring this in, not assumed.
compile_m4_mod() { local dir="$1" sno="$2" out="$3" t; t="$(mktemp -d)"
    ( cd "$dir" && "$SCRIP" $COMPAT --compile "$sno" > "$t/p.s" 2>/dev/null ) || { rm -rf "$t"; return 1; }
    gcc -c "$t/p.s" -o "$t/p.o" 2>/dev/null || { rm -rf "$t"; return 1; }
    gcc "$t/p.o" -L"$RT_DIR" -lscrip_rt -lm -Wl,-rpath,"$RT_DIR" -o "$out" 2>/dev/null || { rm -rf "$t"; return 1; }
    rm -rf "$t"; return 0; }
PARKED_NO_REGEN="breakline k rewind1 genc"
is_parked_no_regen() { local n="$1" s; for s in $PARKED_NO_REGEN; do [ "$n" = "$s" ] && return 0; done; return 1; }
setup_dep_for() { case "$1" in openo2) echo openo;; esac; }
argv_for() { case "$1" in genc) echo v311.sil;; esac; }
# ⛔⭐ PRELOAD FLAGS, from upstream tests.in's OWN registration lines ("reg -Laa/aa.sno preload1.sno").
# csnobol4 -h calls -L "load source file before user program"; scrip had no -L at all until SCRIP (this
# landing) -- the argument fell through to the filename path and died as "cannot open '-Laa/aa.sno'", which
# is indistinguishable from a typo. These two entries were the package's whole UNGRADED bucket, and the
# sidecar reason said the pickup was a runner map; measured, it was a driver flag AND this map.
preload_for() { case "$1" in preload1) echo "-Laa/aa.sno";; preload2) echo "-Laa/aa.sno -Lbb/bb.sno";; esac; }
normalize() { # $1=name $2=text -> echoes text, masked per tests.in's dump/trace convention for that name
    local n="$1" t="$2"
    case " $DUMP_TESTS " in *" $n "*) t="$(printf '%s' "$t" | sed -E -e 's/MAXLNGTH = [0-9]+/MAXLNGTH = xxx/' -e "/^&FILL = '/d")";; esac
    case " $TRACE_TESTS " in *" $n "*) t="$(printf '%s' "$t" | sed -E 's/time = [0-9.eE+-]+$/time = xxx/')";; esac
    printf '%s' "$t"
}

split_at_end() { # $1=src -> writes $2=prog (thru END) $3=stdin-tail
    python3 - "$1" "$2" "$3" << 'PY'
import re, sys
lines = open(sys.argv[1], 'r', errors='replace').read().split('\n')
idx = next((i for i, l in enumerate(lines) if re.match(r'^END\s*$', l, re.IGNORECASE)), None)
if idx is None:
    open(sys.argv[2], 'w').write('\n'.join(lines)); open(sys.argv[3], 'w').write('')
else:
    open(sys.argv[2], 'w').write('\n'.join(lines[:idx+1]) + '\n'); open(sys.argv[3], 'w').write('\n'.join(lines[idx+1:]))
PY
}

W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
RUN="$W/run"; mkdir -p "$RUN"
cp -rp "$SUITE"/. "$RUN"/ 2>/dev/null || true
SCRIP_HASH="$(git -C "$SD" rev-parse --short HEAD 2>/dev/null || echo '?')"
CORP_HASH="$(git -C "$ROOT/corpus" rev-parse --short HEAD 2>/dev/null || echo '?')"

compile_m4() { local sno="$1" out="$2" t pre; t="$(mktemp -d)"; pre="$(preload_for "$(basename "$sno" .sno)")"
    SNO_LIB="$SUITE" "$SCRIP" $COMPAT $pre --compile "$sno" > "$t/p.s" 2>/dev/null || { rm -rf "$t"; return 1; }
    gcc -c "$t/p.s" -o "$t/p.o" 2>/dev/null || { rm -rf "$t"; return 1; }
    gcc "$t/p.o" -L"$RT_DIR" -lscrip_rt -lm -Wl,-rpath,"$RT_DIR" -o "$out" 2>/dev/null || { rm -rf "$t"; return 1; }
    rm -rf "$t"; return 0; }
status_of() { # $1=got $2=rc $3=exp -> echoes PASS|FAIL|REJECT|CRASH|HANG
    local got="$1" rc="$2" exp="$3"
    if [ "$rc" = 124 ]; then echo HANG
    elif [ "$rc" -ge 128 ]; then echo CRASH
    elif [ "$got" = "$exp" ]; then echo PASS
    elif [ "$rc" != 0 ]; then echo REJECT
    else echo FAIL; fi
}

TOTAL=0
M3_PASS=0; M3_FAIL=0; M3_REJECT=0; M3_CRASH=0; M3_HANG=0; RED3=""
M4_PASS=0; M4_FAIL=0; M4_REJECT=0; M4_CRASH=0; M4_HANG=0; RED4=""
CSN_PASS=0; CSN_FAIL=0
REGEN=0; REGEN_LIST=""
EXCLUDED_LIST=""
MOD_SUMMARY=""

for sno in "$SUITE"/*.sno; do
    [ -e "$sno" ] || { echo "⛔ REFUSE(rc=2): zero .sno files in $SUITE"; exit 2; }
    name="$(basename "$sno" .sno)"
    ref="$SUITE/$name.ref"
    [ -f "$ref" ] || continue          # not a graded pair (support file) — never hand-curated
    # ⛔ ALL.sno IS OUR OWN GENERATED CONCATENATION (2006 lines, every entry run together), NOT A SHIPPED
    # VENDOR PROGRAM -- lib_inventory.sh's census already excludes ALL.<ext> fleet-wide (hq_T 2026-09-06,
    # measured across ipl/aisnobol/csnobol4_suite/gimpel), and this loop must agree or `graded` counts one
    # more pair than `shipped` ever offered, and the inventory can never sum. It happens to carry its own
    # .ref (concatenated from every entry's own historical run) and was being silently graded before this.
    [ "$name" = "ALL" ] && continue
    if is_excluded_test "$name"; then EXCLUDED_LIST="$EXCLUDED_LIST $name"; continue; fi
    TOTAL=$((TOTAL+1))
    exp="$(normalize "$name" "$(cat "$ref")")"

    prog="$RUN/$name.sno"; relprog="$name.sno"; inp=/dev/null
    if is_stdin_test "$name"; then
        rm -f "$prog"; split_at_end "$sno" "$prog" "$W/stdin"; inp="$W/stdin"
    fi
    dep="$(setup_dep_for "$name")"
    [ -n "$dep" ] && (cd "$RUN" && SNO_LIB="$SUITE" timeout "$TIMEOUT" "$SCRIP" $COMPAT --run "$dep.sno" > /dev/null 2>&1)
    xargs_extra="$(argv_for "$name")"
    pre_extra="$(preload_for "$name")"

    # ⛔ RELATIVE, NOT $prog: cwd is already $RUN below, and the vendored .ref files were cut against a
    # bare-relative invocation (name.sno). Passing the absolute scratch path here makes every self-path-
    # referencing program (TRACE(), error messages, &FILE) embed a throwaway tmpdir string instead of the
    # bare name the .ref expects — a harness artifact, not a SCRIP or oracle divergence (found triaging
    # row snobol4-csnobol4-thirty-regen-candidate-refs-stale-pin-or-real-defect, seat07 2026-09-04).
    got3="$(cd "$RUN" && SNO_LIB="$SUITE" timeout "$TIMEOUT" "$SCRIP" $COMPAT $pre_extra --run "$relprog" ${xargs_extra:+-- $xargs_extra} < "$inp" 2>&1)"; rc3=$?
    got3="$(normalize "$name" "$got3")"
    st3="$(status_of "$got3" "$rc3" "$exp")"
    progress_append package csnobol4 snobol4 "$name" m3 "$st3" >/dev/null 2>&1 || true
    case "$st3" in
        PASS) M3_PASS=$((M3_PASS+1));;
        FAIL) M3_FAIL=$((M3_FAIL+1)); RED3="$RED3 $name";;
        REJECT) M3_REJECT=$((M3_REJECT+1)); RED3="$RED3 $name";;
        CRASH) M3_CRASH=$((M3_CRASH+1)); RED3="$RED3 $name";;
        HANG) M3_HANG=$((M3_HANG+1)); RED3="$RED3 $name";;
    esac

    if (cd "$RUN" && compile_m4 "$relprog" "$W/prog.bin"); then
        got4="$(cd "$RUN" && SNO_LIB="$SUITE" timeout "$TIMEOUT" "$W/prog.bin" $xargs_extra < "$inp" 2>&1)"; rc4=$?
        got4="$(normalize "$name" "$got4")"
        st4="$(status_of "$got4" "$rc4" "$exp")"
        progress_append package csnobol4 snobol4 "$name" m4 "$st4" >/dev/null 2>&1 || true
        case "$st4" in
            PASS) M4_PASS=$((M4_PASS+1));;
            FAIL) M4_FAIL=$((M4_FAIL+1)); RED4="$RED4 $name";;
            REJECT) M4_REJECT=$((M4_REJECT+1)); RED4="$RED4 $name";;
            CRASH) M4_CRASH=$((M4_CRASH+1)); RED4="$RED4 $name";;
            HANG) M4_HANG=$((M4_HANG+1)); RED4="$RED4 $name";;
        esac
    else
        M4_REJECT=$((M4_REJECT+1)); RED4="$RED4 $name(CC)"
    fi

    gotc="$(cd "$RUN" && timeout "$TIMEOUT" "$CSN" -b $pre_extra "$relprog" $xargs_extra < "$inp" 2>&1)"
    gotc="$(normalize "$name" "$gotc")"
    if [ "$gotc" = "$exp" ]; then CSN_PASS=$((CSN_PASS+1))
    else
        CSN_FAIL=$((CSN_FAIL+1))
        [ "$st3" != PASS ] && ! is_parked_no_regen "$name" && { REGEN=$((REGEN+1)); REGEN_LIST="$REGEN_LIST $name"; }
    fi
done

# ⭐ MODULE-REPLACEMENT COVERAGE LOOP — see header. Same ladder, same shared counters (TOTAL/M3_*/M4_*/
# CSN_*/RED3/RED4) as the loop above; only a distinct "module/" name prefix marks these rows apart in the
# RED-M3/RED-M4 listing. Own scratch copy per test, never the shared /home/resources/csnobol4 tree itself —
# same write-safety rule as $RUN above, against a different shared resource this time (ndbm's test.sno
# creates+deletes foo.db/foo.dir/foo.pag).
MODROOT="$CSN_SRC/modules"
[ -d "$MODROOT" ] || { echo "⛔ REFUSE(rc=2): module coverage source missing: $MODROOT"; exit 2; }
for entry in $MODULE_TESTS; do
    mname="${entry%%:*}"; rest="${entry#*:}"
    mdir="${rest%%:*}"; rest="${rest#*:}"
    msno="${rest%%:*}"; mref="${rest#*:}"
    srcdir="$MODROOT/$mdir"
    [ -f "$srcdir/$msno" ] && [ -f "$srcdir/$mref" ] || { echo "⛔ REFUSE(rc=2): module test pair missing: $srcdir/$msno + $mref"; exit 2; }
    name="module/$mname"
    mrun="$W/mod_$mname"; rm -rf "$mrun"; cp -rp "$srcdir" "$mrun"
    TOTAL=$((TOTAL+1))
    exp="$(normalize "$name" "$(cat "$srcdir/$mref")")"

    got3="$(cd "$mrun" && timeout "$TIMEOUT" "$SCRIP" $COMPAT --run "$msno" < /dev/null 2>&1)"; rc3=$?
    got3="$(normalize "$name" "$got3")"
    st3="$(status_of "$got3" "$rc3" "$exp")"
    progress_append package csnobol4 snobol4 "$name" m3 "$st3" >/dev/null 2>&1 || true
    case "$st3" in
        PASS) M3_PASS=$((M3_PASS+1));;
        FAIL) M3_FAIL=$((M3_FAIL+1)); RED3="$RED3 $name";;
        REJECT) M3_REJECT=$((M3_REJECT+1)); RED3="$RED3 $name";;
        CRASH) M3_CRASH=$((M3_CRASH+1)); RED3="$RED3 $name";;
        HANG) M3_HANG=$((M3_HANG+1)); RED3="$RED3 $name";;
    esac

    st4disp="$st3(different-run)"
    if compile_m4_mod "$mrun" "$msno" "$W/mod_prog.bin"; then
        got4="$(cd "$mrun" && timeout "$TIMEOUT" "$W/mod_prog.bin" < /dev/null 2>&1)"; rc4=$?
        got4="$(normalize "$name" "$got4")"
        st4="$(status_of "$got4" "$rc4" "$exp")"
        progress_append package csnobol4 snobol4 "$name" m4 "$st4" >/dev/null 2>&1 || true
        st4disp="$st4"
        case "$st4" in
            PASS) M4_PASS=$((M4_PASS+1));;
            FAIL) M4_FAIL=$((M4_FAIL+1)); RED4="$RED4 $name";;
            REJECT) M4_REJECT=$((M4_REJECT+1)); RED4="$RED4 $name";;
            CRASH) M4_CRASH=$((M4_CRASH+1)); RED4="$RED4 $name";;
            HANG) M4_HANG=$((M4_HANG+1)); RED4="$RED4 $name";;
        esac
    else
        M4_REJECT=$((M4_REJECT+1)); RED4="$RED4 $name(CC)"; st4disp="REJECT(CC)"
    fi

    gotc="$(cd "$mrun" && timeout "$TIMEOUT" "$CSN" -b "$msno" < /dev/null 2>&1)"
    gotc="$(normalize "$name" "$gotc")"
    ocst=FAIL; [ "$gotc" = "$exp" ] && { CSN_PASS=$((CSN_PASS+1)); ocst=PASS; } || CSN_FAIL=$((CSN_FAIL+1))
    MOD_SUMMARY="$MOD_SUMMARY $mname(m3=$st3,m4=$st4disp,oracle=$ocst)"
done

echo "── csnobol4_suite: $TOTAL pairs · SCRIP $SCRIP_HASH · corpus $CORP_HASH · RT_OPT -O0 · timeout ${TIMEOUT}s · oracle csnobol4 (Phil Budne, home dialect) · .ref primary, live csnobol4 = triangulation + tiebreak/regen"
echo "CSNOBOL4_SUITE_BOARD total=$TOTAL m3_PASS=$M3_PASS m3_FAIL=$M3_FAIL m3_REJECT=$M3_REJECT m3_CRASH=$M3_CRASH m3_HANG=$M3_HANG m4_PASS=$M4_PASS m4_FAIL=$M4_FAIL m4_REJECT=$M4_REJECT m4_CRASH=$M4_CRASH m4_HANG=$M4_HANG"
echo "csnobol4 (home dialect, triangulation, informational): PASS=$CSN_PASS FAIL=$CSN_FAIL"
[ -n "$RED3" ] && echo "RED-M3:$RED3"
[ -n "$RED4" ] && echo "RED-M4:$RED4"
[ "$REGEN" -gt 0 ] && echo "REGEN-CANDIDATE ($REGEN, SCRIP m3 disagrees with .ref but so does live csnobol4 — .ref pin may be stale):$REGEN_LIST"
[ -n "$EXCLUDED_LIST" ] && echo "EXCLUDED (upstream's own tests.in retired these, see script header):$EXCLUDED_LIST"
[ -n "$MOD_SUMMARY" ] && echo "MODULE-COVERAGE (replacement rows for the EXCLUDED four, folded into the totals above as module/*):$MOD_SUMMARY"

# ⭐ THE PACKAGE LOCKDOWN inventory line, via the shared body (lib_inventory.sh) -- never a second copy
# of the arithmetic. $TOTAL already equals "pairs in $SUITE measured against the oracle": the excluded-4/
# module-4 swap above is a same-count replacement (four $SUITE pairs physically unrunnable in this
# flattened layout, traded 1:1 for their MODULE_TESTS equivalents from the shared csnobol4 tree), so it
# is the right "graded" figure for a census of $SUITE itself. graded_narrow=0: grading is byte-exact
# against .ref, never by error-number-only.
INV_PACKAGE=csnobol4; INV_DIR="$SUITE"; INV_EXT=".sno"
INV_LINE="$(inventory_line "$TOTAL" 0)"
if [ -n "$INV_LINE" ]; then echo "$INV_LINE"; else echo "⚠ inventory refused (above) -- the board line still stands; the inventory does not" >&2; fi

# ⛔ ONE LEADERBOARD (RULES.md FACT RULE, Lon 2026-09-03 ~16:05). Records what this script just
# measured into .github/SCORE.md; runs nothing itself. Non-fatal: a bookkeeping failure must never
# turn a real measurement into a red board. Matches the other package suites (Arizona/JCON/fpc/GNU/SWI);
# this one and snoflake_suite's own runner were the two missing it (board-packages-into-make-test-
# reported-then-blocking, seat13 2026-09-03).
# ⭐ THE INVENTORY CLAUSE RIDES IN THE CELL TOO (util_score_row.py's inventory_clauses(), CEO-316):
# appended verbatim when non-empty, runner name immediately after so `by=`/backtick attribution
# finds it within the reader's 200-char window; absent when inventory_line refused.
python3 "$HERE/util_score_row.py" write --lang snobol4 --column vendor --suite CSNOBOL4 --modes m3,m4 \
    --measurer "${S4E_SEAT:-}" \
    --text "total=$TOTAL m3 PASS=$M3_PASS FAIL=$M3_FAIL REJECT=$M3_REJECT CRASH=$M3_CRASH HANG=$M3_HANG · m4 PASS=$M4_PASS FAIL=$M4_FAIL REJECT=$M4_REJECT CRASH=$M4_CRASH HANG=$M4_HANG${INV_LINE:+ · $INV_LINE (\`test_snobol4_csnobol4_suite.sh\`)}" \
    || echo "⚠ SCORE.md NOT UPDATED -- record this row by hand (the REFUSED line above says why)"

# ⛔⭐ POPULATION FLOOR (row every-board-wrapper-refuses-on-a-zero-population-instead-of-passing-
# vacuously, hq_T 2026-09-04): every _FAIL/_REJECT/_CRASH/_HANG bucket reads 0 over TOTAL=0 too
# (empty pair discovery) -- refuse before the vacuous-clean verdict below can be reached.
"$HERE/util_require_population.sh" --gate test_snobol4_csnobol4_suite "$TOTAL" 1 "pairs" || exit 2
[ "$M3_FAIL" = 0 ] && [ "$M3_REJECT" = 0 ] && [ "$M3_CRASH" = 0 ] && [ "$M3_HANG" = 0 ] && \
[ "$M4_FAIL" = 0 ] && [ "$M4_REJECT" = 0 ] && [ "$M4_CRASH" = 0 ] && [ "$M4_HANG" = 0 ]
