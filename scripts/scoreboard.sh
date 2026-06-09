#!/usr/bin/env bash
# scoreboard.sh LANG [--raw]
#
# Gauntlet harness for the LOWER REWRITE (.github/GOAL-IR-REDESIGN.md).
# Runs `scrip --dump-bb` (OLD = oracle) vs `--dump-bb2` (NEW lowerer) over a
# language's corpus, strips `^; proc` + blank lines, diffs, and tallies
# MATCH / DIFFER / NEWFAIL / SKIP.  Replaces the ephemeral /tmp scoreboard
# (LAD-0a) so the yardstick survives container resets and lives in-repo.
#
#   LANG in: icon snobol4 snocone prolog pascal raku
#
# RESULT classes (per program):
#   MATCH    oracle and new both produced output and the stripped dumps agree
#   DIFFER   both produced output but the stripped dumps differ
#   NEWFAIL  oracle produced output but the new lowerer produced none (regression)
#   SKIP     oracle itself produced no dump (cannot parse/lower) -> not scoreable
#
# NORMALIZATION (LAD-0b -- pending Lon's ruling): by default `ival=<7+ digits>`
# is rewritten to `ival=PTR`, masking the NON-DETERMINISTIC heap pointer the
# oracle stores in GEN_SCAN.ival (legit ivals are <=3 digits: opcodes / arg
# counts / tree-codes; the 7-digit floor only catches pointers).  This changes
# the yardstick, so it is an explicit, named, in-repo flag rather than a hidden
# sed.  Pass --raw to DISABLE normalization and diff bytes exactly.

set -u
HERE="$(cd "$(dirname "$0")" && pwd)"
REPO="$(cd "$HERE/.." && pwd)"
CORPUS="$(cd "$REPO/.." 2>/dev/null && pwd)/corpus"
[ -d "$CORPUS" ] || CORPUS=/home/claude/corpus
SCRIP="$REPO/scrip"

LANG_ARG="${1:-}"
NORM=1
for a in "$@"; do [ "$a" = "--raw" ] && NORM=0; done

[ -x "$SCRIP" ] || { echo "scoreboard: no scrip binary at $SCRIP (run: make scrip)"; exit 2; }
[ -n "$LANG_ARG" ] || { echo "usage: scoreboard.sh {icon|snobol4|snocone|prolog|pascal|raku} [--raw]"; exit 2; }

files() {
  case "$1" in
    icon)    find "$REPO/test/icon"    -name '*.icn' 2>/dev/null | sort ;;
    snobol4) find "$REPO/test/snobol4" -name '*.sno' 2>/dev/null | sort ;;
    snocone) { find "$REPO/test/snocone" -name '*.sc' 2>/dev/null; find "$CORPUS/crosscheck/snocone" -name '*.sc' 2>/dev/null; } | sort ;;
    prolog)  find "$REPO/test/prolog"  -name '*.pl'  2>/dev/null | sort ;;
    pascal)  find "$CORPUS/programs/pascal" -name '*.pas' 2>/dev/null | sort ;;
    raku)    find "$REPO/test/raku" \( -name '*.raku' -o -name '*.p6' -o -name '*.pl6' -o -name '*.rk' \) 2>/dev/null | sort ;;
    *) echo "scoreboard: unknown LANG '$1'" >&2; exit 2 ;;
  esac
}

strip_dump() { grep -vE '^; proc' | grep -vE '^[[:space:]]*$'; }
norm_dump()  { if [ "$NORM" = 1 ]; then sed -E 's/ival=[0-9]{7,}/ival=PTR/g'; else cat; fi; }

LIST="$(mktemp)"
files "$LANG_ARG" > "$LIST"

M=0; D=0; N=0; S=0; T=0
printf "%-46s  %s\n" "PROGRAM" "RESULT"
printf "%-46s  %s\n" "----------------------------------------------" "------"
while IFS= read -r f; do
  [ -n "$f" ] || continue
  T=$((T + 1))
  o="$("$SCRIP" --dump-bb  "$f" < /dev/null 2>/dev/null | strip_dump | norm_dump)"
  n="$("$SCRIP" --dump-bb2 "$f" < /dev/null 2>/dev/null | strip_dump | norm_dump)"
  rel="${f#"$REPO"/}"; rel="${rel#"$CORPUS"/}"
  if [ -z "$o" ]; then
    S=$((S + 1)); printf "%-46s  %s\n" "$rel" "SKIP"
  elif [ -z "$n" ]; then
    N=$((N + 1)); printf "%-46s  %s\n" "$rel" "NEWFAIL"
  elif [ "$o" = "$n" ]; then
    M=$((M + 1)); printf "%-46s  %s\n" "$rel" "MATCH"
  else
    D=$((D + 1)); printf "%-46s  %s\n" "$rel" "DIFFER"
  fi
done < "$LIST"
rm -f "$LIST"

echo "----"
echo "LANG=$LANG_ARG  MATCH=$M  DIFFER=$D  NEWFAIL=$N  SKIP=$S  TOTAL=$T  (normalize=$NORM)"
