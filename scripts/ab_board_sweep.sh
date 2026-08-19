#!/usr/bin/env bash
# ab_board_sweep.sh -- GOAL-SNOBOL4-BB LADDER-AB board instrument (single arm).
#
# Same board mechanics as rtcc_board_sweep.sh (status file "STATUS name" per program,
# BY-SET diffable), but one arm per invocation with an arbitrary env prefix, so the
# same script captures the HEAD baseline and every later arm (SCRIP_AB on/off).
#
# Usage: bash scripts/ab_board_sweep.sh [--mode 3|4] --out FILE [--env 'VAR=V ...']
set -u
MODE=3; OUTF=""; ENVSTR=""; CORPUS=${CORPUS:-/home/claude/corpus}
while [ $# -gt 0 ]; do
    case "$1" in
        --mode) MODE="$2"; shift 2;;
        --out)  OUTF="$2"; shift 2;;
        --env)  ENVSTR="$2"; shift 2;;
        *) echo "unknown arg: $1" >&2; exit 2;;
    esac
done
[ -n "$OUTF" ] || { echo "--out FILE required" >&2; exit 2; }
SCRIP=/home/claude/SCRIP/scrip
[ -x "$SCRIP" ] || { echo "no $SCRIP -- build first" >&2; exit 2; }
if [ "$MODE" = "3" ]; then FLAG=--run; else FLAG=--compile; fi
SCRIP_DIR="$(cd "$(dirname "$SCRIP")" && pwd)"
WORK="$(mktemp -d)"; trap 'rm -rf "$WORK"' EXIT
# ⛔ MODE 4 MUST BUILD AND RUN THE PROGRAM, NOT DIFF ITS ASSEMBLY (s147).  `--compile` writes x86 TEXT
# to stdout; the earlier form compared that assembly against the program's expected RUNTIME OUTPUT, a
# comparison structurally incapable of passing.  It reported PASS=0 DIVERGE=317 on EVERY arm, including
# pristine HEAD -- a full, plausible, entirely meaningless table, which is the s33 "non-empty is not
# alive" false-signal class wearing different clothes.  It stayed usable only as an A/B invariance check
# (both arms identical => no movement), and was used that way at s147 before being repaired here.  Mode 4
# now assembles and links exactly as scripts/test_gate_kw_static.sh does, so its number means what the
# column header says.  Mode 3 is untouched.
run_one() {
    local sno="$1"
    if [ "$MODE" = "3" ]; then timeout 30s env $ENVSTR "$SCRIP" --run "$sno" < /dev/null 2>/dev/null; return $?; fi
    local s="$WORK/m4.s" exe="$WORK/m4.bin"
    timeout 30s env $ENVSTR "$SCRIP" --compile "$sno" < /dev/null > "$s" 2>/dev/null || return $?
    gcc -no-pie -o "$exe" "$s" -L"$SCRIP_DIR/out" -lscrip_rt -lm > /dev/null 2>&1 || return 90
    LD_LIBRARY_PATH="$SCRIP_DIR/out" timeout 30s env $ENVSTR "$exe" < /dev/null 2>/dev/null; return $?
}
: > "$OUTF"
for sno in $(find "$CORPUS/crosscheck" -name '*.sno' | sort); do
    base="${sno%.sno}"; name=$(basename "$sno" .sno)
    ref=""
    for cand in "$base.ref" "$(dirname "$sno")/$name.ref"; do
        [ -f "$cand" ] && { ref="$cand"; break; }
    done
    out=$(run_one "$sno")
    rc=$?
    if [ $rc -eq 124 ]; then st=TIMEOUT
    elif [ $rc -eq 90 ]; then st=LINKFAIL
    elif [ $rc -ne 0 ]; then st=ERROR
    elif [ -z "$ref" ]; then st=NOREF
    elif [ "$out" = "$(cat "$ref")" ]; then st=PASS
    else st=DIVERGE
    fi
    echo "$st $name" >> "$OUTF"
done
awk '{c[$1]++} END {printf "PASS=%d DIVERGE=%d ERROR=%d LINKFAIL=%d TIMEOUT=%d NOREF=%d\n", c["PASS"], c["DIVERGE"], c["ERROR"], c["LINKFAIL"], c["TIMEOUT"], c["NOREF"]}' "$OUTF"
