#!/usr/bin/env bash
# util_fc_spine_census.sh — RUNG ZB-FC (ARCH-ZETA §13): count FORTH-cell pushes per kind across the corpus
# under SCRIP_ZETA_PORT=6.  A LEDGER COUNTER, NOT A GATE (the .s-regen philosophy: never enforce sameness —
# the number is supposed to GROW as ZB-FC-3/ZB-ACT-3 land).  Method: compile every crosscheck .sno with
# --compile under PORT=6, attribute each "sub rsp, 16" to the IR_MATCH_* comment block it appears under.
set -u
SCRIP="${SCRIP:-/home/claude/SCRIP/scrip}"
CORPUS="${CORPUS:-/home/claude/corpus/crosscheck}"
[ -d "$CORPUS" ] || CORPUS="/home/claude/corpus/programs/snobol4/crosscheck"
[ -d "$CORPUS" ] || { echo "no corpus dir found"; exit 1; }
declare -A CELLS; TOTAL=0; PROGS=0
for f in $(find "$CORPUS" -name "*.sno" | sort); do
    [ -f "$f" ] || continue
    s=$(SCRIP_ZETA_PORT=6 timeout 8 "$SCRIP" --compile --target=x86 "$f" < /dev/null 2>/dev/null) || continue
    PROGS=$((PROGS+1))
    kind=""
    while IFS= read -r line; do
        case "$line" in
            *"# IR_MATCH_"*) kind="${line##*# }" ;;
            *"sub rsp, 16"*) [ -n "$kind" ] && { CELLS[$kind]=$(( ${CELLS[$kind]:-0} + 1 )); TOTAL=$((TOTAL+1)); } ;;
        esac
    done <<< "$s"
done
FC_SET=" IR_MATCH_SPAN IR_MATCH_TAB IR_MATCH_RTAB IR_MATCH_BREAK IR_MATCH_BREAKX IR_MATCH_BAL IR_MATCH_REM "
echo "=== FORTH SPINE CENSUS (PORT=6, $PROGS programs compiled) ==="
FC=0; ZL=0
for k in $(echo "${!CELLS[@]}" | tr ' ' '\n' | sort); do
    case "$FC_SET" in *" $k "*) tag="fc cell"; FC=$((FC+${CELLS[$k]})) ;; *) tag="zls2/other rsp push (s11 class, NOT an fc cell)"; ZL=$((ZL+${CELLS[$k]})) ;; esac
    printf "  %-24s %5d  %s\n" "$k" "${CELLS[$k]}" "$tag"
done
echo "  FC CELLS: $FC   ·   other rsp pushes: $ZL   ·   total rsp pushes: $TOTAL"
