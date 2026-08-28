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
# Grading: exact = string equality; substring = output contains expect; /i = case-insensitive;
# error = nonzero rc OR /error/i in combined output. @nonstandard fixtures tally as NSTD-PASS/-FAIL
# and never count in FAIL — upstream's own reference cross-checks skip them (implementation-defined).
#
# ⚠ DIALECT: snoflake tracks SIL 3.11 / CSNOBOL4, NOT SPITBOL. SCRIP follows SPITBOL semantics.
# A FAIL here may be dialect distance, not a defect — check before curing (RULES.md dialect law).
# The @options directive (3 fixtures: case-folding, stlimit) is NOT honored by this runner; those
# fixtures are listed in the OPTS line and their verdicts carry that caveat.
#
# ⛔ A missing suite, compiler, RT, or oracle REFUSES with rc=2 — never skip-as-success
# (FINDING-2026-08-24-hq_C-the-regrid-turned-a-red-board-green.md).
# Exit: 0 iff FAIL3=0 and FAIL4=0 over the printed denominator. NSTD and sbl never gate.
set -u
HERE="$(cd "$(dirname "$0")" && pwd)"; SD="$HERE/.."; ROOT="$(cd "$SD/.." && pwd)"
SUITE="${SNOFLAKE_SUITE:-$ROOT/corpus/packages/snobol4/snoflake_suite}"
SCRIP="$SD/scrip"; RT_DIR="$SD/out"; TIMEOUT="${TIMEOUT:-8s}"; ARM_SBL="${ARM_SBL:-1}"; ARM_CSN="${ARM_CSN:-1}"
[ -d "$SUITE" ] || { echo "⛔ REFUSE(rc=2): suite dir missing: $SUITE — a missing corpus is not a green board"; exit 2; }
[ -x "$SCRIP" ] || { echo "⛔ REFUSE(rc=2): no scrip binary at $SCRIP — build first (make)"; exit 2; }
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
GIMPEL="$SUITE/gimpel"
# ⛔ ALL ARMS RUN WITH cwd IN SCRATCH, never in a repo tree: OUTPUT unit-I/O associations create
# files named by their third argument (SPITBOL dialect: a FILE SPEC, e.g. '(121A1)') in the cwd,
# which dirties whatever tree the runner happened to be launched from. Symlinked INCs keep the
# gimpel includes resolvable for the sbl arm, whose include search is cwd-relative.
RUN="$W/run"; mkdir -p "$RUN"; ln -s "$GIMPEL"/*.INC "$GIMPEL"/*.IN "$RUN"/ 2>/dev/null || true
SCRIP_HASH="$(git -C "$SD" rev-parse --short HEAD 2>/dev/null || echo '?')"
CORP_HASH="$(git -C "$ROOT/corpus" rev-parse --short HEAD 2>/dev/null || echo '?')"
P3=0; F3=0; P4=0; F4=0; S4=0; PS=0; FS=0; PC=0; FC=0; N3P=0; N3F=0; N4P=0; N4F=0; NSP=0; NSF=0; NCP=0; NCF=0
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
compile_m4() { local sno="$1" out="$2" t; t="$(mktemp -d)"
    SNO_LIB="$GIMPEL" "$SCRIP" --compile "$sno" > "$t/p.s" 2>/dev/null || { rm -rf "$t"; return 1; }
    gcc -c "$t/p.s" -o "$t/p.o" 2>/dev/null || { rm -rf "$t"; return 1; }
    gcc "$t/p.o" -L"$RT_DIR" -lscrip_rt -lm -Wl,-rpath,"$RT_DIR" -o "$out" 2>/dev/null || { rm -rf "$t"; return 1; }
    rm -rf "$t"; }
run_one() { # $1=cmdkind $2=sno -> sets GOT RC ; input from $W/inp if HASINP
    local inp=/dev/null; [ "$HASINP" = 1 ] && inp="$W/inp"
    case "$1" in
        m3)  GOT="$(cd "$RUN" && SNO_LIB="$GIMPEL" timeout "$TIMEOUT" "$SCRIP" --run "$2" < "$inp" 2>&1)"; RC=$?;;
        m4)  GOT="$(cd "$RUN" && SNO_LIB="$GIMPEL" timeout "$TIMEOUT" "$W/prog.bin" < "$inp" 2>&1)"; RC=$?;;
        sbl) GOT="$(cd "$RUN" && timeout "$TIMEOUT" "$SBL" $SBL_FLAGS "$2" < "$inp" 2>&1)"; RC=$?;;
        csn) GOT="$(cd "$RUN" && timeout "$TIMEOUT" "$CSN" "$2" < "$inp" 2>&1)"; RC=$?;;
    esac; }
for sno in "$SUITE"/*.sno; do
    [ -e "$sno" ] || { echo "⛔ REFUSE(rc=2): zero fixtures in $SUITE"; exit 2; }
    name="$(basename "$sno" .sno)"; TOTAL=$((TOTAL+1))
    read -r MATCH IC NSTD HASINP OPTS <<< "$(parse_fixture "$sno")"
    [ "$OPTS" = 1 ] && OPTS_LIST="$OPTS_LIST $name"
    run_one m3 "$sno"
    if grade "$GOT" "$RC" "$MATCH" "$IC"; then [ "$NSTD" = 1 ] && N3P=$((N3P+1)) || P3=$((P3+1))
    else [ "$NSTD" = 1 ] && N3F=$((N3F+1)) || { F3=$((F3+1)); FL3="$FL3 $name"; }; fi
    if compile_m4 "$sno" "$W/prog.bin"; then
        run_one m4 "$sno"
        if grade "$GOT" "$RC" "$MATCH" "$IC"; then [ "$NSTD" = 1 ] && N4P=$((N4P+1)) || P4=$((P4+1))
        else [ "$NSTD" = 1 ] && N4F=$((N4F+1)) || { F4=$((F4+1)); FL4="$FL4 $name"; }; fi
    else S4=$((S4+1)); FL4="$FL4 $name(CC)"; fi
    if [ -n "$SBL" ]; then
        run_one sbl "$sno"
        if grade "$GOT" "$RC" "$MATCH" "$IC"; then [ "$NSTD" = 1 ] && NSP=$((NSP+1)) || PS=$((PS+1))
        else [ "$NSTD" = 1 ] && NSF=$((NSF+1)) || { FS=$((FS+1)); FLS="$FLS $name"; }; fi
    fi
    if [ -n "$CSN" ]; then
        run_one csn "$sno"
        if grade "$GOT" "$RC" "$MATCH" "$IC"; then [ "$NSTD" = 1 ] && NCP=$((NCP+1)) || PC=$((PC+1))
        else [ "$NSTD" = 1 ] && NCF=$((NCF+1)) || { FC=$((FC+1)); FLC="$FLC $name"; }; fi
    fi
done
echo "── snoflake_suite: $TOTAL fixtures · SCRIP $SCRIP_HASH · corpus $CORP_HASH · RT_OPT -O0 · timeout $TIMEOUT · graded vs EMBEDDED @expect (snoflake/CSNOBOL4 dialect, NOT SPITBOL refs)"
echo "mode-3 (--run):     PASS=$P3 FAIL=$F3  NSTD $N3P/$((N3P+N3F))"
echo "mode-4 (--compile): PASS=$P4 FAIL=$F4 SKIP(cc)=$S4  NSTD $N4P/$((N4P+N4F))"
[ -n "$SBL" ] && echo "sbl -bf (dialect distance, informational): PASS=$PS FAIL=$FS  NSTD $NSP/$((NSP+NSF))"
[ -n "$CSN" ] && echo "csnobol4 (home dialect, triangulation): PASS=$PC FAIL=$FC  NSTD $NCP/$((NCP+NCF))"
[ -n "$OPTS_LIST" ] && echo "OPTS not honored:$OPTS_LIST"
[ -n "$FL3" ] && echo "FAIL-M3:$FL3"
[ -n "$FL4" ] && echo "FAIL-M4:$FL4"
[ -n "$FLS" ] && echo "FAIL-SBL:$FLS"
[ -n "$FLC" ] && echo "FAIL-CSN:$FLC"
[ "$F3" = 0 ] && [ "$F4" = 0 ] && [ "$S4" = 0 ]
