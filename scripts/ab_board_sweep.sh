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
: > "$OUTF"
for sno in $(find "$CORPUS/crosscheck" -name '*.sno' | sort); do
    base="${sno%.sno}"; name=$(basename "$sno" .sno)
    ref=""
    for cand in "$base.ref" "$(dirname "$sno")/$name.ref"; do
        [ -f "$cand" ] && { ref="$cand"; break; }
    done
    out=$(timeout 30s env $ENVSTR "$SCRIP" $FLAG "$sno" < /dev/null 2>/dev/null)
    rc=$?
    if [ $rc -eq 124 ]; then st=TIMEOUT
    elif [ $rc -ne 0 ]; then st=ERROR
    elif [ -z "$ref" ]; then st=NOREF
    elif [ "$out" = "$(cat "$ref")" ]; then st=PASS
    else st=DIVERGE
    fi
    echo "$st $name" >> "$OUTF"
done
awk '{c[$1]++} END {printf "PASS=%d DIVERGE=%d ERROR=%d TIMEOUT=%d NOREF=%d\n", c["PASS"], c["DIVERGE"], c["ERROR"], c["TIMEOUT"], c["NOREF"]}' "$OUTF"
