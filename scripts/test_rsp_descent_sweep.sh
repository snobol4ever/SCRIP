#!/usr/bin/env bash
# s58 RSP-ONLY DESCENT SWEEP (Lon in-chat: "run the entire test suite in RSP only mode... determine what state it ends").
# The coming-out side is frozen: RETURN/FRETURN/NRETURN are BOMBS.  Classification per program (m4 TEXT, the TINY-REAL scheme):
#   PASS              ran to completion, output == oracle (programs that never call a DEFINE)
#   DESCENT_OK        died at BOMB-RETURN with stdout a PREFIX of the oracle's output — the call made it all the way down
#   BOMB_RETURN_BAD   BOMB-RETURN but pre-bomb stdout is NOT an oracle prefix — descent reached RETURN with corrupted state
#   BOMB_FRETURN/BOMB_NRETURN   died at those floaters (prefix not checked — FRETURN paths diverge from oracle by design here)
#   COMPILE_FAIL / ASM_FAIL / TIMEOUT / SIG<n> / DIFF   everything else — the bugs to fix before only depth-spots remain
#   ORACLE_FAIL       s60: sbl itself produced nothing and scrip agreed — no oracle to grade against (NOT a scrip defect)
# s60: parameterized (SWEEP_DIRS / SWEEP_LIST) + parallel (SWEEP_JOBS) so the corpus checkout can be swept, not just SCRIP test+demo.
#      Classification logic is ONE AUTHORITY (classify_one) shared by every caller.  Default invocation is byte-identical in
#      behaviour to the s58/s59 instrument: SWEEP_DIRS="test demo", SWEEP_JOBS=1.
cd "$(dirname "$0")/.."
ROOT=$PWD   # include-bearing tests (-include 'lib/*.sno') resolve against the corpus checkout: sbl is CWD-relative, scrip honors SNO_LIB (s59 harness fix; both engines verified on test_case)
OUT=${1:-/tmp/descent_sweep.txt}; : > "$OUT"
: "${SWEEP_DIRS:=test demo}"
: "${SWEEP_JOBS:=1}"
: "${SWEEP_ORACLE:=/home/claude/x64/bin/sbl}"
: "${SWEEP_CORPUS:=/home/claude/corpus}"
export ROOT SWEEP_ORACLE SWEEP_CORPUS
classify_one() {
  f=$1
  case "$f" in /*) src=$f;; *) src=$ROOT/$f;; esac
  tag=$$-$(printf '%s' "$f" | md5sum | cut -c1-10)
  S=/tmp/dsw.$tag.s; X=/tmp/dsw.$tag.x; CE=/tmp/dsw.$tag.cerr; GE=/tmp/dsw.$tag.gerr; RE=/tmp/dsw.$tag.rerr
  ORA=$(cd "$SWEEP_CORPUS" && timeout 30 "$SWEEP_ORACLE" -b "$src" </dev/null 2>/dev/null); ost=$?
  st=
  if ! SNO_LIB="$SWEEP_CORPUS" timeout 30 "$ROOT/scrip" --compile "$src" </dev/null > $S 2>$CE; then st=COMPILE_FAIL
  elif ! gcc -no-pie $S -L "$ROOT/out" -lscrip_rt -Wl,-rpath,"$ROOT/out" -o $X 2>$GE; then st=ASM_FAIL
  else
    RO=$(timeout 8 $X </dev/null 2>$RE); rc=$?
    if grep -q "BOMB-RETURN" $RE; then
      case "$ORA" in "$RO"*) st=DESCENT_OK;; *) st=BOMB_RETURN_BAD;; esac
    elif grep -q "BOMB-FRETURN" $RE; then st=BOMB_FRETURN
    elif grep -q "BOMB-NRETURN" $RE; then st=BOMB_NRETURN
    elif [ $rc -eq 124 ]; then st=TIMEOUT
    elif [ $rc -ge 128 ]; then st=SIG$((rc-128))
    elif [ "$RO" = "$ORA" ]; then st=PASS
    else st=DIFF; fi
  fi
  if [ $ost -ne 0 ] && [ -z "$ORA" ]; then case "$st" in PASS|DIFF) st=ORACLE_FAIL;; esac; fi
  rm -f $S $X $CE $GE $RE
  echo "$st $f"
}
export -f classify_one
if [ -n "${SWEEP_LIST:-}" ]; then LIST=$(cat "$SWEEP_LIST"); else LIST=$(find $SWEEP_DIRS -name '*.sno' 2>/dev/null | sort); fi
printf '%s\n' "$LIST" | xargs -d '\n' -P "$SWEEP_JOBS" -I{} bash -c 'classify_one "$@"' _ {} >> "$OUT"
sort -o "$OUT" "$OUT"
awk '{print $1}' "$OUT" | sort | uniq -c | sort -rn
echo "detail: $OUT"
