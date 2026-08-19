#!/usr/bin/env bash
# board_demos_zeta.sh — DEMO board for CLAWS5 / TREEBANK / JSON / CALCULATOR
#
# Classifies each demo as PASS / DIFF / SIG11 / HANG / ABORT against its .ref.
# Serial by construction (parallel misclassifies SEGV as HANG — s23 measurement).
#
# Usage: bash scripts/board_demos_zeta.sh [m3|m4] [snap <tag> | diff <tagA> <tagB>]
#
# Authors: LCherryholmes | Claude Opus 5

DEMO="/home/claude/corpus/programs/snobol4/demo"
SCRIP="/home/claude/SCRIP/scrip"
TIMEOUT="${TIMEOUT:-30}"
MODE="${1:-m3}"
SNAPDIR="/tmp/demo_board"
mkdir -p "$SNAPDIR"

# demo -> stdin file  (empty = no stdin)
declare -A IN=(
  [claws5]=claws5.input
  [claws5-match]=claws5.input
  [claws5-match-fence]=claws5.input
  [json]=json.input
  [json-match]=json.input
  [json-match-fence]=json.input
  [calculator-1]=calculator.input
  [calculator-1-match]=calculator.input
  [calculator-1-match-fence]=calculator.input
  [calculator-2]=calculator.input
  [calculator-2-match]=calculator.input
  [calculator-2-match-fence]=calculator.input
  [treebank-list]=treebank.input
  [treebank-match]=treebank.input
  [treebank-match-fence]=treebank.input
)

ORDER="claws5 claws5-match claws5-match-fence json json-match json-match-fence calculator-1 calculator-1-match calculator-1-match-fence calculator-2 calculator-2-match calculator-2-match-fence treebank-list treebank-match treebank-match-fence"

classify() {
  local name="$1" sno="$DEMO/$1.sno" ref="$DEMO/$1.ref" stdin="$DEMO/${IN[$1]}"
  [ -f "$sno" ] || { echo "MISSING"; return; }
  [ -f "$ref" ] || { echo "NOREF"; return; }
  local out rc
  if [ "$MODE" = "m3" ]; then
    if [ -n "${IN[$1]}" ] && [ -f "$stdin" ]; then
      out=$(cd "$DEMO" && timeout "$TIMEOUT" "$SCRIP" --run "$sno" < "$stdin" 2>/dev/null); rc=$?
    else
      out=$(cd "$DEMO" && timeout "$TIMEOUT" "$SCRIP" --run "$sno" < /dev/null 2>/dev/null); rc=$?
    fi
  else
    local exe="/tmp/demo_board/$1.exe"
    ( cd "$DEMO" && SCRIP_RTCC=0 timeout "$TIMEOUT" "$SCRIP" --compile "$sno" -o "$exe" ) > /dev/null 2>&1
    [ -x "$exe" ] || { echo "COMPILEFAIL"; return; }
    if [ -n "${IN[$1]}" ] && [ -f "$stdin" ]; then
      out=$(cd "$DEMO" && timeout "$TIMEOUT" "$exe" < "$stdin" 2>/dev/null); rc=$?
    else
      out=$(cd "$DEMO" && timeout "$TIMEOUT" "$exe" < /dev/null 2>/dev/null); rc=$?
    fi
  fi
  case $rc in
    139) echo "SIG11"; return ;;
    124) echo "HANG";  return ;;
    134) echo "ABORT"; return ;;
  esac
  if [ "$out" = "$(cat "$ref")" ]; then echo "PASS"; else echo "DIFF"; fi
}

cmd="${2:-run}"
case "$cmd" in
  snap)
    tag="${3:?tag required}"
    : > "$SNAPDIR/$MODE.$tag"
    for d in $ORDER; do
      v=$(classify "$d"); echo "$d $v" >> "$SNAPDIR/$MODE.$tag"; printf '%-28s %s\n' "$d" "$v"
    done
    echo "--- $MODE/$tag ---"
    awk '{c[$2]++} END{for(k in c) printf "%s=%d ", k, c[k]; print ""}' "$SNAPDIR/$MODE.$tag"
    ;;
  diff)
    a="${3:?}"; b="${4:?}"
    echo "REPAIRED (${a} -> ${b}):"
    join "$SNAPDIR/$MODE.$a" "$SNAPDIR/$MODE.$b" | awk '$2!="PASS" && $3=="PASS"{print "  + "$1" "$2" -> "$3}'
    echo "BROKEN (${a} -> ${b}):"
    join "$SNAPDIR/$MODE.$a" "$SNAPDIR/$MODE.$b" | awk '$2=="PASS" && $3!="PASS"{print "  - "$1" "$2" -> "$3}'
    echo "MOVED (non-PASS class change):"
    join "$SNAPDIR/$MODE.$a" "$SNAPDIR/$MODE.$b" | awk '$2!="PASS" && $3!="PASS" && $2!=$3{print "  ~ "$1" "$2" -> "$3}'
    ;;
  *)
    for d in $ORDER; do printf '%-28s %s\n' "$d" "$(classify "$d")"; done
    ;;
esac
