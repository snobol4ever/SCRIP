#!/usr/bin/env bash
# test_snoflake_suite.sh — run Ori Livneh's snoflake fixture suite against SCRIP m3 + m4, plus a
# SPITBOL comparison arm (ARM_SBL=0 to disable).
#
# Suite: corpus/packages/snobol4/snoflake_suite (vendored from github.com/atdt/snoflake test/programs,
# BSD-2-Clause, commit 54f7b801 — see PROVENANCE.md there). Each .sno is a self-describing fixture:
# a header of '*' comment lines carrying @directives, then a program that runs unmodified.
#   @expect (block '* @expect >>>' .. '* <<<' or single-line)  expected output
#   @input  (block)                                            stdin lines
#   @match  exact (default) | substring | error, optional /i   grading mode
#   @nonstandard                                               implementation-defined output
# Block payload strip per upstream test/program-fixture.js: '* <<<' ends, '*' -> empty line,
# '* xxx' -> 'xxx' (exactly the two chars '* ' removed).
#
# ⭐⭐ GRADING IS AGAINST THE ORACLE, NOT AGAINST @expect (ceo CEO-251, 2026-09-04, ruling (a) on hq_B's ask
# `q-snoflake-denominator-vs-the-dialect-law`). A fixture PASSES when SCRIP's output equals `sbl -bf`'s. The
# fixture's own @expect is INFORMATIONAL and is still graded, but only to compute the dialect tally.
# WHY: snoflake tracks SIL 3.11 / CSNOBOL4 and SCRIP follows SPITBOL (RULES.md Semantics), so two thirds of
# this suite's @expect failures were SCRIP agreeing with SPITBOL, i.e. being right. Grading against the oracle
# scores what the law actually requires; the denominator stays 180 and 100% means every fixture matching SPITBOL.
# ⛔ TWO NORMALIZATIONS, BOTH MEASURED, AND THE RULE IS MEANINGLESS WITHOUT THEM:
#   (1) SPITBOL emits its OWN furniture into the compared stream -- banner, `x86-64 <date>`, page headers and a
#       statement listing -- on 56 of these 180 fixtures, driven by `-LIST` control lines INSIDE the fixtures,
#       not by our flags (`-b` already suppresses the signon). Diverted via lib_oracle_flags.sh's
#       sbl_listing_sink_flag (56 -> 0). ⛔ That flag FAILS OPEN: `-o file open error.` in the oracle's output
#       means the listing came back and every comparison is invalid, so this runner REFUSES rc=2 on it rather
#       than scoring the fixture -- a silently re-furnished stream is exactly the false all-FAIL table class.
#   (2) Error TEXT never matches byte-for-byte and never will: SCRIP says
#       `SCRIP: ERROR 248 -- attempted redefinition of system function: SORT` where sbl says
#       `t.sno(40) : ERROR 248 -- attempted redefinition of system function` -- the SAME refusal, with the
#       file/line on one side and the offending name on the other. So errors compare by ERROR NUMBER.
# ⭐ THE DIALECT TALLY is printed beside the score and never folded into it: fixtures that PASS against the
# oracle while FAILING their own @expect, i.e. where SPITBOL itself departs from what snoflake expects.
# @nonstandard fixtures tally as NSTD-PASS/-FAIL and never count in FAIL (implementation-defined output).
#
# ⚠ DIALECT: snoflake tracks SIL 3.11 / CSNOBOL4, NOT SPITBOL. SCRIP follows SPITBOL semantics.
# A FAIL here may be dialect distance, not a defect — check before curing (RULES.md dialect law).
# The @options directive (3 fixtures: case-folding, stlimit) is NOT honored by this runner; those
# fixtures are listed in the OPTS line and their verdicts carry that caveat.
#
# ⛔ A missing suite, compiler, RT, or oracle REFUSES with rc=2 — never skip-as-success
# (FINDING-2026-08-24-hq_C-the-regrid-turned-a-red-board-green.md).
# Exit: 0 iff FAIL3=0 and FAIL4=0 and SKIP(cc)=0 over the printed denominator (verdict = matches the oracle).
set -u
HERE="$(cd "$(dirname "$0")" && pwd)"; SD="$HERE/.."; ROOT="$(cd "$SD/.." && pwd)"
SUITE="${SNOFLAKE_SUITE:-$ROOT/corpus/packages/snobol4/snoflake_suite}"
SCRIP="$SD/scrip"; RT_DIR="$SD/out"; TIMEOUT="${TIMEOUT:-8s}"; ARM_SBL="${ARM_SBL:-1}"; ARM_CSN="${ARM_CSN:-1}"
[ -d "$SUITE" ] || { echo "⛔ REFUSE(rc=2): suite dir missing: $SUITE — a missing corpus is not a green board"; exit 2; }
[ -x "$SCRIP" ] || { echo "⛔ REFUSE(rc=2): no scrip binary at $SCRIP — build first (make)"; exit 2; }
# ⛔⭐ STALE-BINARY PREFLIGHT (row harness-and-ladder-runner-refuse-on-a-stale-binary-like-the-artifact-regen-
# does, ceo -> hq_T 2026-09-04). The line above proves a binary EXISTS; this one proves it is the binary this
# tree describes. ceo's witness, twice on 2026-09-04: a 10:57 binary graded at 14:03 read RED, then GREEN after
# an incremental make -- a vendor board is exactly where that is least visible, because a plausible all-FAIL
# table is this class's normal output. NO LOGIC HERE: util_require_fresh.sh sources gate_require_fresh from
# lib_gate.sh, the ONE authority (hq_B 4c7253e99) -- never a second copy of the staleness rule.
"$HERE/util_require_fresh.sh" --gate test_snoflake_suite "$SCRIP" "${RT_DIR:-$HERE/../out}/libscrip_rt.so" || exit 2
[ -f "$RT_DIR/libscrip_rt.so" ] || { echo "⛔ REFUSE(rc=2): no $RT_DIR/libscrip_rt.so"; exit 2; }
SBL=""
if [ "$ARM_SBL" = "1" ]; then
    . "$HERE/lib_oracle_flags.sh" 2>/dev/null || { echo "⛔ REFUSE(rc=2): lib_oracle_flags.sh unloadable and ARM_SBL=1"; exit 2; }
    SBL="$(sbl_correctness_bin)"; SBL_FLAGS="$(sbl_lang_flags)"
    [ -x "$SBL" ] || { echo "⛔ REFUSE(rc=2): oracle absent: $SBL — set ARM_SBL=0 to survey without it"; exit 2; }
fi
CSN=""
if [ "$ARM_CSN" = "1" ]; then
    # ⛔ ASSET ROOT FIRST, WORKSPACE ROOT ONLY AS A FALLBACK (hq_C 2026-08-28).  The original spelling looked ONLY at
    # "$SD/../csnobol4/snobol4" -- INSIDE the seat's workspace root -- which contradicts two standing rules at once:
    # CLAUDE.md's "the oracles are NOT siblings here, they live outside every root" (sbl, icont/iconx and swipl are all
    # reached by asset path), and the handoff law, because handoff_status.sh AUTO-DISCOVERS every top-level directory
    # that is a git repo with an origin remote -- so a csnobol4 clone placed where this line wanted it becomes a
    # PERMANENT handoff blocker the moment it has a local commit, on a remote we do not own.  ⭐ The sibling script
    # build_csnobol4_archive.sh already resolves it as "$S4A/csnobol4" (asset root); this line was the odd one out, and
    # the disagreement was invisible because whoever wrote it had the tree in the one place that worked for them.
    CSN_ASSET="${S4E_ASSETS:-$([ -d "$SD/../x64" ] && echo "$SD/.." || echo /home/resources)}/csnobol4/snobol4"
    if [ -x "$CSN_ASSET" ]; then CSN="$CSN_ASSET"; else CSN="$SD/../csnobol4/snobol4"; fi
    [ -x "$CSN" ] || { echo "⛔ REFUSE(rc=2): csnobol4 absent: $CSN — set ARM_CSN=0 to survey without it (triangulation arm, Lon 2026-08-28)"; exit 2; }
fi
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
# ⛔⭐ ARM_SBL=0 IS NO LONGER A SURVEY MODE, IT IS A REFUSAL. Before CEO-251 the oracle was one informational
# arm among four and switching it off cost you a column. It is now THE VERDICT, so with it off every fixture
# would be compared against an empty string and this script would print a full, plausible, entirely FALSE
# all-FAIL table -- the exact class RULES.md and this file's own header warn about. Grading cannot be
# downgraded to a survey by an env var.
[ "$ARM_SBL" = "1" ] || { echo "⛔ REFUSE(rc=2): ARM_SBL=0 but the verdict IS the oracle since CEO-251 -- without sbl there is nothing to grade against, and an ungraded run must never print a board"; exit 2; }
SBL_SINK=""
[ -n "$SBL" ] && { SBL_SINK="$(sbl_listing_sink_flag "$W")" || { echo "⛔ REFUSE(rc=2): no writable listing sink for the oracle -- SPITBOL FAILS OPEN on -o= and would dump its listing back into the compared stream"; exit 2; }; }
GIMPEL="$SUITE/gimpel"
# ⛔ ALL ARMS RUN WITH cwd IN SCRATCH, never in a repo tree: OUTPUT unit-I/O associations create
# files named by their third argument (SPITBOL dialect: a FILE SPEC, e.g. '(121A1)') in the cwd,
# which dirties whatever tree the runner happened to be launched from. Symlinked INCs keep the
# gimpel includes resolvable for the sbl arm, whose include search is cwd-relative.
RUN="$W/run"; mkdir -p "$RUN"; ln -s "$GIMPEL"/*.INC "$GIMPEL"/*.IN "$RUN"/ 2>/dev/null || true
SCRIP_HASH="$(git -C "$SD" rev-parse --short HEAD 2>/dev/null || echo '?')"
CORP_HASH="$(git -C "$ROOT/corpus" rev-parse --short HEAD 2>/dev/null || echo '?')"
DIA=0; DIAL=""; ORACLE_OUT=""; ORACLE_MEETS_EXPECT=0; P3=0; F3=0; P4=0; F4=0; S4=0; PS=0; FS=0; PC=0; FC=0; N3P=0; N3F=0; N4P=0; N4F=0; NSP=0; NSF=0; NCP=0; NCF=0
FL3=""; FL4=""; FLS=""; FLC=""; OPTS_LIST=""; TOTAL=0
parse_fixture() { # $1=sno -> writes $W/exp $W/inp; echoes "match ic nstd hasinp opts"
    local match=exact ic=0 nstd=0 hasinp=0 opts=0 inblock="" line payload rest
    : > "$W/exp"; : > "$W/inp"
    while IFS= read -r line || [ -n "$line" ]; do
        case "$line" in '*'*) ;; *) break;; esac
        if [ -n "$inblock" ]; then
            if [ "$line" = '* <<<' ]; then inblock=""; continue; fi
            if [ "$line" = '*' ]; then payload=""
            elif [ "${line:0:2}" = '* ' ]; then payload="${line:2}"
            else continue; fi
            case "$inblock" in expect) printf '%s\n' "$payload" >> "$W/exp";;
                               input)  printf '%s\n' "$payload" >> "$W/inp";; esac
            continue
        fi
        case "$line" in
            '* @expect >>>'*) inblock=expect;;
            '* @input >>>'*)  inblock=input; hasinp=1;;
            '* @expect '*)    rest="${line#\* @expect }"; rest="${rest#"${rest%%[![:space:]]*}"}"; printf '%s\n' "$rest" >> "$W/exp";;
            '* @match '*)     rest="${line#\* @match }"; rest="${rest#"${rest%%[![:space:]]*}"}"
                              case "$rest" in */i) ic=1; rest="${rest%/i}";; esac; match="$rest";;
            '* @nonstandard'*) nstd=1;;
            '* @options '*)   opts=1;;
        esac
    done < "$1"
    echo "$match $ic $nstd $hasinp $opts"
}
grade() { # $1=got $2=rc $3=match $4=ic ; expects $W/exp; returns 0 pass
    local got="$1" rc="$2" match="$3" ic="$4" exp; exp="$(cat "$W/exp")"
    if [ "$ic" = 1 ]; then got="$(printf '%s' "$got" | tr '[:upper:]' '[:lower:]')"; exp="$(printf '%s' "$exp" | tr '[:upper:]' '[:lower:]')"; fi
    case "$match" in
        exact)     [ "$got" = "$exp" ];;
        substring) case "$got" in *"$exp"*) true;; *) false;; esac;;
        error)     [ "$rc" != 0 ] || printf '%s' "$got" | grep -qi 'error';;
        *)         false;;
    esac
}
oracle_equal() { # $1=scrip output  $2=oracle output -- equal, or the SAME ERROR NUMBER (wording differs by design)
    [ "$1" = "$2" ] && return 0
    local ea eb
    ea="$(printf '%s' "$1" | grep -oiE 'ERROR +[0-9]+' | head -1 | grep -oE '[0-9]+')"
    eb="$(printf '%s' "$2" | grep -oiE 'ERROR +[0-9]+' | head -1 | grep -oE '[0-9]+')"
    [ -n "$ea" ] && [ "$ea" = "$eb" ]; }
compile_m4() { local sno="$1" out="$2" t rc; t="$(mktemp -d)"
    SNO_LIB="$GIMPEL" "$SCRIP" --compile "$sno" > "$t/p.s" 2>"$t/compile.err"; rc=$?
    if [ "$rc" -ne 0 ]; then
        # ⭐ snoflake-sixteen-fixtures-pass-mode-3-and-fail-mode-4 (seat01 2026-09-04): a `SCRIP: ERROR N -- ...`
        # here is a GRADED PROGRAM ANSWER, same class as run_one's m3 crash-and-print (header comment (2)
        # above) -- not a toolchain failure. It happens at compile time only because literal top-level DEFINE
        # is prescanned ahead of codegen (lower_snobol4.c), unlike every other SNOBOL4 runtime error, which
        # compiles fine and only raises at execution. Recognize this ONE shape and let oracle_equal grade it
        # by ERROR NUMBER like any other error; anything else (segfault, an unrecognized message, no
        # diagnostic at all) is a real toolchain failure and stays SKIP(cc), rc=1.
        CTERR="$(cat "$t/compile.err")"; rm -rf "$t"
        printf '%s' "$CTERR" | grep -qE '^SCRIP: ERROR [0-9]+ -- ' && return 2
        return 1
    fi
    CTERR=""
    gcc -c "$t/p.s" -o "$t/p.o" 2>/dev/null || { rm -rf "$t"; return 1; }
    gcc "$t/p.o" -L"$RT_DIR" -lscrip_rt -lm -Wl,-rpath,"$RT_DIR" -o "$out" 2>/dev/null || { rm -rf "$t"; return 1; }
    rm -rf "$t"; }
run_one() { # $1=cmdkind $2=sno -> sets GOT RC ; input from $W/inp if HASINP
    local inp=/dev/null; [ "$HASINP" = 1 ] && inp="$W/inp"
    case "$1" in
        m3)  GOT="$(cd "$RUN" && SNO_LIB="$GIMPEL" timeout "$TIMEOUT" "$SCRIP" --run "$2" < "$inp" 2>&1)"; RC=$?;;
        m4)  GOT="$(cd "$RUN" && SNO_LIB="$GIMPEL" timeout "$TIMEOUT" "$W/prog.bin" < "$inp" 2>&1)"; RC=$?;;
        sbl) GOT="$(cd "$RUN" && timeout "$TIMEOUT" "$SBL" $SBL_FLAGS $SBL_SINK "$2" < "$inp" 2>&1)"; RC=$?;;
        csn) GOT="$(cd "$RUN" && timeout "$TIMEOUT" "$CSN" "$2" < "$inp" 2>&1)"; RC=$?;;
    esac; }
for sno in "$SUITE"/*.sno; do
    [ -e "$sno" ] || { echo "⛔ REFUSE(rc=2): zero fixtures in $SUITE"; exit 2; }
    name="$(basename "$sno" .sno)"; TOTAL=$((TOTAL+1))
    read -r MATCH IC NSTD HASINP OPTS <<< "$(parse_fixture "$sno")"
    [ "$OPTS" = 1 ] && OPTS_LIST="$OPTS_LIST $name"
    run_one sbl "$sno"; ORACLE_OUT="$GOT"
    case "$ORACLE_OUT" in *"-o file open error"*)
        echo "⛔ REFUSE(rc=2): the oracle's listing sink would not open on $name, so SPITBOL dumped its listing"
        echo "   back into the compared stream. Every verdict from here would be against a re-furnished stream."; exit 2;; esac
    if grade "$ORACLE_OUT" "$RC" "$MATCH" "$IC"; then [ "$NSTD" = 1 ] && NSP=$((NSP+1)) || PS=$((PS+1)); ORACLE_MEETS_EXPECT=1
    else [ "$NSTD" = 1 ] && NSF=$((NSF+1)) || { FS=$((FS+1)); FLS="$FLS $name"; }; ORACLE_MEETS_EXPECT=0; fi
    run_one m3 "$sno"
    if oracle_equal "$GOT" "$ORACLE_OUT"; then [ "$NSTD" = 1 ] && N3P=$((N3P+1)) || P3=$((P3+1))
        [ "$ORACLE_MEETS_EXPECT" = 0 ] && { DIA=$((DIA+1)); DIAL="$DIAL $name"; }
    else [ "$NSTD" = 1 ] && N3F=$((N3F+1)) || { F3=$((F3+1)); FL3="$FL3 $name"; }; fi
    compile_m4 "$sno" "$W/prog.bin"; m4rc=$?
    if [ "$m4rc" -eq 0 ]; then
        run_one m4 "$sno"
        if oracle_equal "$GOT" "$ORACLE_OUT"; then [ "$NSTD" = 1 ] && N4P=$((N4P+1)) || P4=$((P4+1))
        else [ "$NSTD" = 1 ] && N4F=$((N4F+1)) || { F4=$((F4+1)); FL4="$FL4 $name"; }; fi
    elif [ "$m4rc" -eq 2 ]; then
        if oracle_equal "$CTERR" "$ORACLE_OUT"; then [ "$NSTD" = 1 ] && N4P=$((N4P+1)) || P4=$((P4+1))
        else [ "$NSTD" = 1 ] && N4F=$((N4F+1)) || { F4=$((F4+1)); FL4="$FL4 $name(CTERR)"; }; fi
    else S4=$((S4+1)); FL4="$FL4 $name(CC)"; fi
    if [ -n "$CSN" ]; then
        run_one csn "$sno"
        if grade "$GOT" "$RC" "$MATCH" "$IC"; then [ "$NSTD" = 1 ] && NCP=$((NCP+1)) || PC=$((PC+1))
        else [ "$NSTD" = 1 ] && NCF=$((NCF+1)) || { FC=$((FC+1)); FLC="$FLC $name"; }; fi
    fi
done
echo "── snoflake_suite: $TOTAL fixtures · SCRIP $SCRIP_HASH · corpus $CORP_HASH · RT_OPT -O0 · timeout $TIMEOUT · graded vs the ORACLE sbl -bf (ceo CEO-251); @expect is informational and drives only the dialect tally"
echo "mode-3 (--run):     PASS=$P3 FAIL=$F3  NSTD $N3P/$((N3P+N3F))"
echo "mode-4 (--compile): PASS=$P4 FAIL=$F4 SKIP(cc)=$S4  NSTD $N4P/$((N4P+N4F))"
echo "dialect tally (NOT in the score): $DIA fixture(s) pass against SPITBOL while failing their own @expect -- SPITBOL itself departs from what snoflake expects there"
[ -n "$SBL" ] && echo "sbl -bf vs @expect (informational, the dialect measurement): PASS=$PS FAIL=$FS  NSTD $NSP/$((NSP+NSF))"
[ -n "$CSN" ] && echo "csnobol4 (home dialect, triangulation): PASS=$PC FAIL=$FC  NSTD $NCP/$((NCP+NCF))"
[ -n "$OPTS_LIST" ] && echo "OPTS not honored:$OPTS_LIST"
[ -n "$FL3" ] && echo "FAIL-M3:$FL3"
[ -n "$FL4" ] && echo "FAIL-M4:$FL4"
[ -n "$FLS" ] && echo "FAIL-SBL:$FLS"
[ -n "$FLC" ] && echo "FAIL-CSN:$FLC"
[ -n "$DIAL" ] && echo "DIALECT:$DIAL"
# ⛔⭐ POPULATION FLOOR (row every-board-wrapper-refuses-on-a-zero-population-instead-of-passing-
# vacuously, hq_T 2026-09-04): P3/F3/N3P/N3F all read 0 when "$SUITE"/*.sno matches nothing (an unset
# or wrong $SUITE) -- refuse before the vacuous-clean verdict below can be reached.
"$HERE/util_require_population.sh" --gate test_snoflake_suite "$((P3+F3+N3P+N3F))" 1 "mode-3 gradings under \$SUITE" || exit 2

# ⛔ ONE LEADERBOARD (RULES.md FACT RULE, Lon 2026-09-03 ~16:05). Records what this script just
# measured into .github/SCORE.md; runs nothing itself. Non-fatal: a bookkeeping failure must never
# turn a real measurement into a red board. Matches the other package suites (Arizona/JCON/fpc/GNU/SWI);
# this one and csnobol4_suite's own runner were the two missing it (board-packages-into-make-test-
# reported-then-blocking, seat13 2026-09-03).
python3 "$HERE/util_score_row.py" write --lang snobol4 --column vendor --suite Snoflake --modes m3,m4 \
    --measurer "${S4E_SEAT:-}" \
    --text "mode-3 PASS=$P3 FAIL=$F3 NSTD $N3P/$((N3P+N3F)) · mode-4 PASS=$P4 FAIL=$F4 SKIP(cc)=$S4 NSTD $N4P/$((N4P+N4F)) (\`test_snoflake_suite.sh\`)" \
    || echo "⚠ SCORE.md NOT UPDATED -- record this row by hand (the REFUSED line above says why)"

[ "$F3" = 0 ] && [ "$F4" = 0 ] && [ "$S4" = 0 ]
