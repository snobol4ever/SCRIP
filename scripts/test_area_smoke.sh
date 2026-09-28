#!/usr/bin/env bash
# test_area_smoke.sh -- THE AREA SMOKE a seat runs per landing (Lon 2026-09-27, verbatim: "So if you change the SPAN function,
# then run every program that has SPAN as a reference. That is one of the columns in the attribute file."; ceo CEO-1342;
# RULES.md section ONE TESTING OFFICER, ONE SCORE BOARD, THE AREA SMOKE clause 4; the coo's row instruments-the-area-smoke-...).
#
#   test_area_smoke.sh                     the landing: reads the tree's diff against origin/main (tracked changes and untracked
#                                          files), maps every touched src/ file and changed function through scripts/area_map.tsv
#                                          to the feature columns of the attribute tables, runs the union in both modes.
#   test_area_smoke.sh FENCE SPAN ...      explicit feature names, as the tables spell them.
#   --list-only                            print the selection and run nothing.
#   AREA_SMOKE_BASE=<ref>                  the base to diff against (default origin/main).
#   AREA_SMOKE_ROOT=<dir>                  the checkout to diff (default this script's SCRIP/); the map is read from THAT root's
#                                          scripts/area_map.tsv when it has one, else from beside this script.
#
# ⛔ THE SEAT MUST NOT HAVE TO KNOW (Lon's sixth question): the map answers, not memory. A touched src/ file with no row in the
# map REFUSES rc=2 naming it -- the lander adds the row with the landing. A tree equal to the base prints NO-DIFF and exits 0.
# A landing that touches only carriers mapped to `-` (no area) prints NO-AREA and exits 0. Files outside src/ (scripts, corpus,
# docs) carry no area and are printed as such: the gates a diff touched are the other half of the per-landing verdict.
# ⛔ NOT A BOARD: corpus_suite_harness.py smoke appends no progress row and writes no score cell (CEO-547); no one-runner guard
# is sourced here because none applies. EXIT: 0 every selected entry PASS in both modes (or nothing to smoke, said loudly);
# 1 a red entry; 2 could not measure (unmapped carrier, unknown feature, zero population, stale binary).
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="${AREA_SMOKE_ROOT:-$(cd "$HERE/.." && pwd)}"
MAP="$ROOT/scripts/area_map.tsv"; [ -f "$MAP" ] || MAP="$HERE/area_map.tsv"
H="$HERE/corpus_suite_harness.py"
BASE="${AREA_SMOKE_BASE:-origin/main}"
[ -f "$MAP" ] || { echo "AREA_SMOKE REFUSE(2): scripts/area_map.tsv is missing -- the carrier map is the instrument"; exit 2; }
[ -f "$H" ] || { echo "AREA_SMOKE REFUSE(2): corpus_suite_harness.py is missing beside this script"; exit 2; }
LIST=""; FEATS=(); for a in "$@"; do case "$a" in --list-only) LIST=--list-only;; -*) echo "AREA_SMOKE REFUSE(2): unknown switch $a"; exit 2;; *) FEATS+=("$a");; esac; done

# expand_features: `*` -> every feature column of every table; `lang:<x>` -> the columns of x's tables; else itself.
expand_features() {
  local out="" f
  for f in "$@"; do
    case "$f" in
      '*') out="$out $(python3 "$H" smoke --vocabulary 2>/dev/null | sed 's/^AREA_SMOKE_VOCAB table=[^ ]* runnable=[^ ]* //' | tr '\n' ' ')" ;;
      lang:*) out="$out $(python3 "$H" smoke --vocabulary --tables "tests/${f#lang:} packages/${f#lang:}/" 2>/dev/null | sed 's/^AREA_SMOKE_VOCAB table=[^ ]* runnable=[^ ]* //' | tr '\n' ' ')" ;;
      -) ;;
      *) out="$out $f" ;;
    esac
  done
  printf '%s\n' "$out" | tr ' ' '\n' | grep -v '^$' | awk '!seen[$0]++' | tr '\n' ' '
}

if [ ${#FEATS[@]} -eq 0 ]; then
  git -C "$ROOT" rev-parse --verify -q "$BASE^{commit}" >/dev/null || { echo "AREA_SMOKE REFUSE(2): base $BASE is not a commit in $ROOT (set AREA_SMOKE_BASE, or fetch origin)"; exit 2; }
  files="$( { git -C "$ROOT" diff --name-only "$BASE" -- . ; git -C "$ROOT" ls-files --others --exclude-standard -- . ; } 2>/dev/null | sort -u)"
  if [ -z "$files" ]; then echo "AREA_SMOKE NO-DIFF: the tree equals $BASE ($(git -C "$ROOT" rev-parse --short "$BASE")) -- nothing to smoke"; exit 0; fi
  raw=""; unmapped=""; noarea=""; outside=""
  while IFS= read -r f; do
    [ -n "$f" ] || continue
    case "$f" in src/*) ;; *) outside="$outside $f"; continue;; esac
    hit=""
    # rows whose carrier is this file, or a directory prefix of it
    while IFS=$'\t' read -r feat car; do
      case "$feat" in ''|'#'*) continue;; esac
      case "$car" in
        */) case "$f" in "$car"*) hit="$hit $feat";; esac ;;
        */*|*.*) [ "$car" = "$f" ] && hit="$hit $feat" ;;
      esac
    done < "$MAP"
    # symbol rows: the functions the -U0 hunk headers name (a new file has no hunk names; a row for the file is owed then)
    if [ -z "$hit" ] && git -C "$ROOT" cat-file -e "$BASE:$f" 2>/dev/null; then
      funcs="$(git -C "$ROOT" diff -U0 "$BASE" -- "$f" | sed -n 's/^@@[^@]*@@ *//p' | sed -n 's/.*[^A-Za-z0-9_]\([A-Za-z_][A-Za-z0-9_]*\) *(.*/\1/p;s/^\([A-Za-z_][A-Za-z0-9_]*\) *(.*/\1/p' | sort -u)"
      all_named=1; any=0
      for fn in $funcs; do any=1; r="$(awk -F'\t' -v s="$fn" '$1 !~ /^#/ && $2==s {print $1}' "$MAP")"; [ -n "$r" ] && hit="$hit $r" || all_named=0; done
      [ "$any" = 1 ] && [ "$all_named" = 1 ] || hit=""
    fi
    if [ -z "$hit" ]; then unmapped="$unmapped $f"; continue; fi
    case " $hit " in *" - "*) [ "$(printf '%s\n' $hit | grep -vc '^-$')" = 0 ] && { noarea="$noarea $f"; continue; };; esac
    raw="$raw $hit"
    echo "AREA_SMOKE_MAP $f -> $(printf '%s\n' $hit | grep -v '^-$' | awk '!seen[$0]++' | tr '\n' ' ' | sed 's/ $//')"
  done <<< "$files"
  [ -z "$outside" ] || echo "AREA_SMOKE no area (outside src/):$outside -- the gates such a diff touched are the other half of the verdict"
  [ -z "$noarea" ] || echo "AREA_SMOKE no area (mapped to -):$noarea"
  if [ -n "$unmapped" ]; then
    echo "AREA_SMOKE REFUSE(2): touched src/ file(s) with NO ROW in scripts/area_map.tsv:$unmapped -- add feature<TAB>carrier rows with this landing (a shared node maps to *, a frontend to lang:<x>, a helper to -)"
    exit 2
  fi
  FEATS=($(expand_features $raw))
  if [ ${#FEATS[@]} -eq 0 ]; then echo "AREA_SMOKE NO-AREA: the touched files carry no feature area -- nothing to smoke"; exit 0; fi
else
  FEATS=($(expand_features "${FEATS[@]}"))
fi
echo "AREA_SMOKE features (${#FEATS[@]}): ${FEATS[*]}"
python3 "$H" smoke $LIST -- "${FEATS[@]}"
rc=$?
echo "AREA_SMOKE rc=$rc tree=$(git -C "$ROOT" rev-parse --short HEAD 2>/dev/null)$( [ -z "$(git -C "$ROOT" status --porcelain 2>/dev/null)" ] || printf -- '-DIRTY') base=$BASE"
exit $rc
