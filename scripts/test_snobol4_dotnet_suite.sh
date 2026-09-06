#!/usr/bin/env bash
# test_snobol4_dotnet_suite.sh -- dedicated gate for corpus/packages/snobol4/dotnet (14 programs),
# minted for row snobol4-gimpel-aisnobol-dotnet-measured-with-dedicated-gates (hq_T 2026-09-03): this
# suite was previously counted only in prose and folded into scorecard_snobol4.sh's MISC bucket
# alongside unrelated test dirs -- a regression here was invisible behind that shared denominator.
# Not folded into scorecard_snobol4.sh's own SUITES table (that file is the SINGLE AUTHORITY other
# scripts extract via SCORECARD_PRINT_SUITES=1; this suite is small enough that a standalone script
# carries far less risk than surgery on a 587-line shared mechanism).
#
# Grading: live oracle diff only (no .ref files exist for this suite) -- SCRIP m3 and m4 output compared
# byte-for-byte against a live SPITBOL (`sbl -bf`) run, same stdin, same scratch cwd, same timeout.
# ⛔ STDOUT ONLY, NEVER 2>&1: two engines that agree on an error still print different diagnostic TEXT
# for it, which would manufacture a FAIL out of an agreement -- see test_snobol4_aisnobol_suite.sh's
# header for the measured example and the prior-art false-disagreement class this avoids.
#
# stdin: none of these 14 ship a same-stem .IN/.in/.input file (checked); every program runs with
# /dev/null on both engines -- still a valid oracle-diff comparison. Several (e.g. palin.sno) use the
# TERMINAL pseudo-variable for interactive I/O rather than INPUT/OUTPUT; this script does not need to
# know that dialect's exact redirection semantics, because both engines receive the identical stream --
# the live-oracle diff is robust to a dialect detail neither of us hand-modeled.
#
# UNSCR (not FAIL): two independent oracle-liveness checks, neither alone is sufficient. (1) sbl_died()
# detects a GRACEFUL fatal-report (banner to STDOUT, exits 0 -- rc alone is not a liveness signal;
# copied from scorecard_snobol4.sh's own check, same convention gimpel already uses). (2) the oracle's
# own exit code >= 128 (killed by a signal) catches the UNGRACEFUL case sbl_died cannot: measured live
# on Test.sno/Test2.sno/SourceLines00{1,2,3}.sno, all five carrying a UTF-8 BOM at byte 0 -- the oracle
# misreads the BOM as part of the first label ("ERROR 214 -- bad label or misplaced continuation line")
# and then SEGFAULTS (rc=139) instead of finishing its normal error report, so it never prints the
# "in statement N" footer sbl_died's text check needs. Comparing SCRIP's real output against a crashed
# oracle's truncated one would misattribute an oracle bug (BOM handling) as a SCRIP defect.
#
# REFUSE (not FAIL) for any program whose -INCLUDE target is missing from this directory (none found at
# authoring time -- checked every *.sno for an actual -INCLUDE directive, not just a comment mentioning
# the word -- but the check stays in case a future addition needs it, same convention as the aisnobol gate).
#
# REFUSE (not FAIL) for a file carrying more than one END statement: chap7.sno is a textbook chapter's
# worth of separate example programs concatenated into one file (measured: 2 top-level END lines, the
# second example starting mid-file under its own "Simple word usage program" header) -- not a single
# valid SNOBOL4 program. SCRIP correctly REFUSES it ("duplicate label 'END'"); the oracle instead runs
# only its first chunk silently, which is the more surprising behavior of the two, not the more correct
# one. Grading SCRIP against a partial run of a malformed multi-program file would blame SCRIP for
# being the stricter, arguably more correct, of the two engines.
#
# Exit: 0 iff FAIL3=0 and FAIL4=0 over the printed (scoreable) denominator. REFUSE entries never gate.
set -u
HERE="$(cd "$(dirname "$0")" && pwd)"; SD="$HERE/.."; ROOT="$(cd "$SD/.." && pwd)"
SUITE="${DOTNET_SUITE:-$ROOT/corpus/packages/snobol4/dotnet}"
SCRIP="$SD/scrip"; RT_DIR="$SD/out"; TIMEOUT="${TIMEOUT:-20s}"
[ -d "$SUITE" ] || { echo "⛔ REFUSE(rc=2): suite dir missing: $SUITE"; exit 2; }
[ -x "$SCRIP" ] || { echo "⛔ REFUSE(rc=2): no scrip binary at $SCRIP -- build first (make)"; exit 2; }
# ⛔⭐ STALE-BINARY PREFLIGHT (row harness-and-ladder-runner-refuse-on-a-stale-binary-like-the-artifact-regen-
# does, ceo -> hq_T 2026-09-04). The line above proves a binary EXISTS; this one proves it is the binary this
# tree describes. ceo's witness, twice on 2026-09-04: a 10:57 binary graded at 14:03 read RED, then GREEN after
# an incremental make -- a vendor board is exactly where that is least visible, because a plausible all-FAIL
# table is this class's normal output. NO LOGIC HERE: util_require_fresh.sh sources gate_require_fresh from
# lib_gate.sh, the ONE authority (hq_B 4c7253e99) -- never a second copy of the staleness rule.
"$HERE/util_require_fresh.sh" --gate test_snobol4_dotnet_suite "$SCRIP" "${RT_DIR:-$HERE/../out}/libscrip_rt.so" || exit 2
[ -f "$RT_DIR/libscrip_rt.so" ] || { echo "⛔ REFUSE(rc=2): no $RT_DIR/libscrip_rt.so"; exit 2; }
. "$HERE/lib_oracle_flags.sh" 2>/dev/null || { echo "⛔ REFUSE(rc=2): lib_oracle_flags.sh unloadable"; exit 2; }
SBL="$(sbl_correctness_bin)"; SBL_FLAGS="$(sbl_lang_flags)"
[ -x "$SBL" ] || { echo "⛔ REFUSE(rc=2): oracle absent: $SBL"; exit 2; }
sbl_assert_bf "$SBL" 2>/dev/null || { echo "⛔ REFUSE(rc=2): oracle at $SBL failed the -bf capability check"; exit 2; }
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
SCRIP_HASH="$(git -C "$SD" rev-parse --short HEAD 2>/dev/null || echo '?')"
CORP_HASH="$(git -C "$ROOT/corpus" rev-parse --short HEAD 2>/dev/null || echo '?')"
TOTAL=0; UNSCR=0; P3=0; F3=0; P4=0; F4=0; S4=0
FL3=""; FL4=""; FLU=""
sbl_died() { printf '%s' "$1" | grep -qE ' : ERROR [0-9][0-9][0-9] -- ' && printf '%s' "$1" | grep -qE '^in statement +[0-9]+$'; }
has_bom() { [ "$(head -c3 "$1" | xxd -p 2>/dev/null)" = "efbbbf" ]; }
# ⚠️ A BOM-prefixed source hits the oracle in TWO different shapes, neither of which sbl_died() alone
# catches: (a) segfault mid-report (Test.sno et al, rc>=128, handled above) or (b) a PARSE-time fatal
# that prints only the ERROR line and stops -- rc=1, no crash, but also no "in statement N" footer
# (SourceLines001.sno/Test2.sno measured: prints "ERROR 214 -- bad label or misplaced continuation
# line" then exits, nothing more). Checking the SOURCE BYTES directly is more precise than loosening
# the text match (which would risk matching a real program's own legitimate output by coincidence).
stdin_of() { local sno="$1" b; b="${sno%.sno}"
    for ext in IN in input; do [ -f "$b.$ext" ] && { echo "$b.$ext"; return; }; done
    echo /dev/null; }
compile_m4() { local sno="$1" out="$2" t; t="$(mktemp -d)"
    SNO_LIB="$SUITE" "$SCRIP" --compile "$sno" > "$t/p.s" 2>/dev/null || { rm -rf "$t"; return 1; }
    gcc -c "$t/p.s" -o "$t/p.o" 2>/dev/null || { rm -rf "$t"; return 1; }
    gcc "$t/p.o" -L"$RT_DIR" -lscrip_rt -lm -Wl,-rpath,"$RT_DIR" -o "$out" 2>/dev/null || { rm -rf "$t"; return 1; }
    rm -rf "$t"; }
RUN="$W/run"; mkdir -p "$RUN"
for sno in "$SUITE"/*.sno; do
    [ -e "$sno" ] || { echo "⛔ REFUSE(rc=2): zero fixtures in $SUITE"; exit 2; }
    name="$(basename "$sno" .sno)"; TOTAL=$((TOTAL+1))
    inc="$(grep -ohE "^[[:space:]]*-INCLUDE ['\"][^'\"]+['\"]" "$sno" | sed -E "s/.*-INCLUDE ['\"]([^'\"]+)['\"]/\1/" | head -1)"
    if [ -n "$inc" ] && [ ! -f "$SUITE/$inc" ]; then
        UNSCR=$((UNSCR+1)); FLU="$FLU $name(missing-include:$inc)"; continue
    fi
    if has_bom "$sno"; then
        UNSCR=$((UNSCR+1)); FLU="$FLU $name(oracle-cannot-parse-utf8-bom)"; continue
    fi
    ends="$(grep -cE '^END([[:space:]]|$)' "$sno")"
    if [ "$ends" -gt 1 ]; then
        UNSCR=$((UNSCR+1)); FLU="$FLU $name(multi-program-file:$ends-END-statements)"; continue
    fi
    inp="$(stdin_of "$sno")"
    gotS="$(cd "$RUN" && timeout "$TIMEOUT" "$SBL" $SBL_FLAGS "$sno" < "$inp" 2>/dev/null)"; rcS=$?
    if [ "$rcS" -ge 128 ] || sbl_died "$gotS"; then
        UNSCR=$((UNSCR+1))
        if [ "$rcS" -ge 128 ]; then FLU="$FLU $name(oracle-crashed:sig$((rcS-128)))"; else FLU="$FLU $name(oracle-died)"; fi
        continue
    fi
    got3="$(cd "$RUN" && SNO_LIB="$SUITE" timeout "$TIMEOUT" "$SCRIP" --run "$sno" < "$inp" 2>/dev/null)"
    if [ "$got3" = "$gotS" ]; then P3=$((P3+1)); else F3=$((F3+1)); FL3="$FL3 $name"; fi
    if compile_m4 "$sno" "$W/prog.bin"; then
        got4="$(cd "$RUN" && timeout "$TIMEOUT" "$W/prog.bin" < "$inp" 2>/dev/null)"
        if [ "$got4" = "$gotS" ]; then P4=$((P4+1)); else F4=$((F4+1)); FL4="$FL4 $name"; fi
    else S4=$((S4+1)); FL4="$FL4 $name(CC)"
    fi
done
SCORED=$((TOTAL-UNSCR))
echo "DOTNET_BOARD total=$TOTAL scored=$SCORED unscr=$UNSCR m3_pass=$P3 m3_fail=$F3 m4_pass=$P4 m4_fail=$F4 m4_skip=$S4 -- SCRIP $SCRIP_HASH corpus $CORP_HASH RT_OPT=-O0 oracle=sbl-bf timeout=$TIMEOUT"
[ -n "$FLU" ] && echo "UNSCR (missing corpus dependency, not a SCRIP defect):$FLU"
[ -n "$FL3" ] && echo "FAIL-M3 (vs live sbl -bf):$FL3"
[ -n "$FL4" ] && echo "FAIL-M4 (vs live sbl -bf):$FL4"
# ⭐ THE PACKAGE LOCKDOWN (Lon 2026-09-06): TOTAL is a fresh per-run filesystem census (the for loop
# above), never a cached count, so shipped can never silently lag the vendored dir. Every one of the 14
# lands in SCORED or UNSCR (named above) -- nothing here is ever left unclassified.
echo "PACKAGE_INVENTORY shipped=$TOTAL graded=$SCORED ungradable=$UNSCR ungraded=$((TOTAL - SCORED - UNSCR))"
# ⛔⭐ POPULATION FLOOR (row every-board-wrapper-refuses-on-a-zero-population-instead-of-passing-
# vacuously, hq_T 2026-09-04): F3/F4/S4 all read 0 over zero SCORED entries too (empty corpus dir,
# every witness oracle-crashed/died) -- refuse before the vacuous-clean verdict below can be reached.
"$HERE/util_require_population.sh" --gate test_snobol4_dotnet_suite "$SCORED" 1 "scored rows (total=$TOTAL unscr=$UNSCR)" || exit 2
[ "$F3" = 0 ] && [ "$F4" = 0 ] && [ "$S4" = 0 ]
