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
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)

set -u
HERE="$(cd "$(dirname "$0")" && pwd)"
REPO="$(cd "$HERE/.." && pwd)"
CORPUS="$(cd "$REPO/.." 2>/dev/null && pwd)/corpus"
[ -d "$CORPUS" ] || CORPUS=$S4E/corpus
SCRIP="$REPO/scrip"

LANG_ARG="${1:-}"
NORM=1
for a in "$@"; do [ "$a" = "--raw" ] && NORM=0; done

[ -x "$SCRIP" ] || { echo "scoreboard: no scrip binary at $SCRIP (run: make scrip)"; exit 2; }
[ -n "$LANG_ARG" ] || { echo "usage: scoreboard.sh {icon|snobol4|snocone|prolog|pascal|raku} [--raw]"; exit 2; }

# ⛔ REFUSE IF THE TOOL'S OWN PREMISE IS GONE, NOT JUST IF A CORPUS PATH IS (2026-08-28,
# scoreboard-all-language-paths-dead-total-zero). This tool's whole job is OLD (--dump-bb) vs NEW
# (--dump-bb2) lowerer comparison for the LOWER REWRITE (GOAL-IR-REDESIGN.md). That rewrite reached
# "PROMOTION COMPLETE" 2026-06-11 (SCRIP 662f249): the new lowerer became the ONLY lowerer, the old
# machinery was deleted, and `--dump-bb2` is no longer a recognized driver flag (confirmed: it is not
# in src/driver/scrip.c's arg table; `scrip --dump-bb2 anything` prints "cannot open '--dump-bb2'" --
# the driver falls through to treating the unrecognized flag string as a filename). Every run since
# then has scored 100% NEWFAIL for every file in every language -- not a regression signal, a dead
# comparison silently misreported as one. Refusing here is the same "measure, don't report zero" duty
# this task already applies to missing corpus directories, one level up: a harness whose SECOND ARM no
# longer exists must refuse just as loudly as one whose corpus went missing.
_bb2_probe="$("$SCRIP" --dump-bb2 /dev/null 2>&1 >/dev/null)"
case "$_bb2_probe" in
  *"cannot open '--dump-bb2'"*)
    echo "scoreboard: REFUSE -- --dump-bb2 is not a recognized scrip flag (LOWER REWRITE promoted to production 2026-06-11, SCRIP 662f249; old lowerer deleted, no 'new' lowerer left to compare against). This tool's OLD-vs-NEW comparison has no second arm to run; every prior invocation since the promotion silently scored 100% NEWFAIL instead of refusing. See .github/GOAL-IR-REDESIGN.md and the scoreboard-all-language-paths-dead-total-zero task ledger for the full finding. Needs a ruling (retire this tool, or repurpose it) before it can produce a number again -- not something a path fix should paper over." >&2
    exit 2 ;;
esac

# ⛔ A HARNESS THAT CANNOT MEASURE MUST REFUSE, NOT REPORT ZERO (2026-08-28, scoreboard-all-language-paths-dead-total-zero).
# `find $missing_dir 2>/dev/null` is silent-nothing: a moved/renamed corpus path produces an empty file
# list, not an error, and TOTAL=0 then reads as "scored and empty" instead of "could not measure". Every
# arm below is existence-checked before its `find` runs; a missing arm refuses by name, rc=2.
need_dir() { [ -d "$1" ] || { echo "scoreboard: REFUSE — $2 corpus directory not found: $1" >&2; exit 2; }; }

files() {
  case "$1" in
    icon)    need_dir "$CORPUS/tests/icon" icon
             find "$CORPUS/tests/icon"    -name '*.icn' 2>/dev/null | sort ;;
    snobol4) need_dir "$CORPUS/tests/snobol4" snobol4
             find "$CORPUS/tests/snobol4" -name '*.sno' 2>/dev/null | sort ;;
    snocone) need_dir "$CORPUS/tests/snocone" "snocone (tests)"
             need_dir "$CORPUS/crosscheck/snocone" "snocone (crosscheck)"
             { find "$CORPUS/tests/snocone" -name '*.sc' 2>/dev/null; find "$CORPUS/crosscheck/snocone" -name '*.sc' 2>/dev/null; } | sort ;;
    prolog)  need_dir "$CORPUS/tests/prolog" prolog
             find "$CORPUS/tests/prolog"  -name '*.pl'  2>/dev/null | sort ;;
    pascal)  need_dir "$CORPUS/tests/pascal" pascal
             find "$CORPUS/tests/pascal" -name '*.pas' 2>/dev/null | sort ;;
    raku)    need_dir "$CORPUS/tests/raku" raku
             find "$CORPUS/tests/raku" \( -name '*.raku' -o -name '*.p6' -o -name '*.pl6' -o -name '*.rk' \) 2>/dev/null | sort ;;
    *) echo "scoreboard: unknown LANG '$1'" >&2; exit 2 ;;
  esac
}

strip_dump() { grep -vE '^; proc' | grep -vE '^[[:space:]]*$'; }
norm_dump()  { if [ "$NORM" = 1 ]; then sed -E 's/ival=[0-9]{7,}/ival=PTR/g'; else cat; fi; }

# ORACLE-CRASH SKIP LIST (Lon ruling 2026-06-09).  The OLD lowering (--dump-bb,
# the oracle) SEGFAULTS partway through these programs, emitting a partial dump
# to stdout before crashing.  A partial dump would otherwise score DIFFER (the
# empty-oracle SKIP test below only catches a fully-empty dump), so a crashing
# oracle masquerades as a real divergence.  These programs are UNSCOREABLE until
# the oracle crash is fixed and are forced to SKIP by relative path.  The B07
# Snocone set crashes on compound-assignment (+=/-=/*=///^=) lowering; cross.sno
# crashes on its @N cursor-capture + conditional-replacement scan.
oracle_crashes() {
  case "$1" in
    test/snobol4/strings/cross.sno) return 0 ;;
    crosscheck/snocone/rungB07/B07_slash_assign.sc)    return 0 ;;
    crosscheck/snocone/rungB07/B07_plus_assign.sc)     return 0 ;;
    crosscheck/snocone/rungB07/B07_compound_chain.sc)  return 0 ;;
    crosscheck/snocone/rungB07/B07_star_assign.sc)     return 0 ;;
    crosscheck/snocone/rungB07/B07_caret_assign.sc)    return 0 ;;
    crosscheck/snocone/rungB07/B07_minus_assign.sc)    return 0 ;;
    *) return 1 ;;
  esac
}

LIST="$(mktemp)"
files "$LANG_ARG" > "$LIST"

M=0; D=0; N=0; S=0; T=0
printf "%-46s  %s\n" "PROGRAM" "RESULT"
printf "%-46s  %s\n" "----------------------------------------------" "------"
while IFS= read -r f; do
  [ -n "$f" ] || continue
  T=$((T + 1))
  rel="${f#"$REPO"/}"; rel="${rel#"$CORPUS"/}"
  if oracle_crashes "$rel"; then
    S=$((S + 1)); printf "%-46s  %s\n" "$rel" "SKIP"; continue
  fi
  o="$(SCRIP_NL=0 "$SCRIP" --dump-bb  "$f" < /dev/null 2>/dev/null | strip_dump | norm_dump)"
  n="$("$SCRIP" --dump-bb2 "$f" < /dev/null 2>/dev/null | strip_dump | norm_dump)"
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
