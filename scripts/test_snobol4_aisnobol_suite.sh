#!/usr/bin/env bash
# test_snobol4_aisnobol_suite.sh -- dedicated gate for corpus/packages/snobol4/aisnobol (8 programs),
# minted for row snobol4-gimpel-aisnobol-dotnet-measured-with-dedicated-gates (hq_T 2026-09-03): this
# suite was previously counted only in prose and folded into scorecard_snobol4.sh's MISC bucket
# alongside unrelated test dirs -- a regression here was invisible behind that shared denominator.
# Not folded into scorecard_snobol4.sh's own SUITES table (that file is the SINGLE AUTHORITY other
# scripts extract via SCORECARD_PRINT_SUITES=1; this suite is small enough that a standalone script
# carries far less risk than surgery on a 587-line shared mechanism with its own history of half-applied
# edits tearing things for the whole box).
#
# Grading: live oracle diff only (no .ref files exist for this suite) -- SCRIP m3 and m4 output compared
# byte-for-byte against a live SPITBOL (`sbl -bf`) run, same stdin, same scratch cwd, same timeout.
# Correctness is defined by oracle diff (RULES.md); a hand-authored expected-output file would just be a
# guess wearing an oracle's clothes.
# ⛔ STDOUT ONLY, NEVER 2>&1: two engines that agree an undefined function was called still print
# different diagnostic TEXT for it (SCRIP: "Error 5 ... Undefined function"; sbl: "ERROR 022 --
# undefined function called") -- merging stderr into the comparison manufactures a FAIL out of an
# agreement, the exact false-disagreement class already documented in this project's Prolog census
# history (seat15, corpus 6a9f01fe4: "an earlier, wrong first pass that merged 2>&1 produced 31 false
# disagreements purely from differing warning text on the same empty stdout").
#
# stdin: ATN/ENDING/HSORT/SIR/WANG each ship a same-stem `.IN` file (case as shipped, not `.in`); fed
# verbatim to BOTH engines. BUILDLIB and KALAH have none -- run with /dev/null, which is still a valid
# oracle-diff comparison (both engines see the identical empty stream).
#
# UNSCR (not FAIL): two independent oracle-liveness checks, neither alone is sufficient. (1) sbl_died()
# detects a GRACEFUL fatal-report (prints a banner to STDOUT and exits 0 -- rc alone is not a liveness
# signal, copied from scorecard_snobol4.sh's own check, same convention gimpel already uses; measured
# live on BUILDLIB.sno, "ERROR 022 -- undefined function called" + "in statement N" both present).
# (2) the oracle's own exit code >= 128 (killed by a signal) catches the UNGRACEFUL case sbl_died
# cannot -- measured on this suite's dotnet sibling, where a UTF-8 BOM makes the oracle SEGFAULT
# mid-report instead of finishing it. Comparing SCRIP against a dead-or-crashed oracle's output would
# manufacture a FAIL where there is no ground truth to fail against.
#
# REFUSE (not FAIL, not silently skipped) for a structural gap neither engine can ever clear:
# TEST.sno and SIR.sno both `-INCLUDE "SNOCORE.sno"`, which exists NOWHERE in corpus or in the asset
# root (checked: `find corpus /home/resources -iname SNOCORE*` -- zero hits). This is a missing corpus
# dependency, not a SCRIP defect (same class as the lon_cherryholmes Trace.sno/host.sno gap already
# documented in scorecard_snobol4.sh) -- UNSCR is named on its own line, never merely absent from FAIL.
#
# Exit: 0 iff FAIL3=0 and FAIL4=0 over the printed (scoreable) denominator. REFUSE entries never gate.
set -u
HERE="$(cd "$(dirname "$0")" && pwd)"; SD="$HERE/.."; ROOT="$(cd "$SD/.." && pwd)"
SUITE="${AISNOBOL_SUITE:-$ROOT/corpus/packages/snobol4/aisnobol}"
SCRIP="$SD/scrip"; RT_DIR="$SD/out"; TIMEOUT="${TIMEOUT:-20s}"
[ -d "$SUITE" ] || { echo "⛔ REFUSE(rc=2): suite dir missing: $SUITE"; exit 2; }
[ -x "$SCRIP" ] || { echo "⛔ REFUSE(rc=2): no scrip binary at $SCRIP -- build first (make)"; exit 2; }
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
# A UTF-8 BOM at byte 0 confuses the oracle's label parser two different ways (segfault mid-report, or
# a parse-time fatal with no footer for sbl_died() to key on) -- see the dotnet sibling script's header
# for the measured detail. None of this suite's 8 files carry a BOM today; the check stays for when one
# does, so this suite doesn't silently misgrade the day someone adds one.
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
    inc="$(grep -ohE "\-INCLUDE ['\"][^'\"]+['\"]" "$sno" | sed -E "s/-INCLUDE ['\"]([^'\"]+)['\"]/\1/" | head -1)"
    if [ -n "$inc" ] && [ ! -f "$SUITE/$inc" ]; then
        UNSCR=$((UNSCR+1)); FLU="$FLU $name(missing-include:$inc)"; continue
    fi
    if has_bom "$sno"; then
        UNSCR=$((UNSCR+1)); FLU="$FLU $name(oracle-cannot-parse-utf8-bom)"; continue
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
echo "AISNOBOL_BOARD total=$TOTAL scored=$SCORED unscr=$UNSCR m3_pass=$P3 m3_fail=$F3 m4_pass=$P4 m4_fail=$F4 m4_skip=$S4 -- SCRIP $SCRIP_HASH corpus $CORP_HASH RT_OPT=-O0 oracle=sbl-bf timeout=$TIMEOUT"
[ -n "$FLU" ] && echo "UNSCR (missing corpus dependency, not a SCRIP defect):$FLU"
[ -n "$FL3" ] && echo "FAIL-M3 (vs live sbl -bf):$FL3"
[ -n "$FL4" ] && echo "FAIL-M4 (vs live sbl -bf):$FL4"
[ "$F3" = 0 ] && [ "$F4" = 0 ] && [ "$S4" = 0 ]
