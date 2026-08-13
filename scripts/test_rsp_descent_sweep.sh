#!/usr/bin/env bash
# s58 RSP-ONLY DESCENT SWEEP (Lon in-chat: "run the entire test suite in RSP only mode... determine what state it ends").
# The coming-out side is frozen: RETURN/FRETURN/NRETURN are BOMBS.  Classification per program (m4 TEXT, the TINY-REAL scheme):
#   PASS              ran to completion, output == oracle (programs that never call a DEFINE)
#   DESCENT_OK        died at BOMB-RETURN with stdout a PREFIX of the oracle's output — the call made it all the way down
#   BOMB_RETURN_BAD   BOMB-RETURN but pre-bomb stdout is NOT an oracle prefix — descent reached RETURN with corrupted state
#   BOMB_FRETURN/BOMB_NRETURN   died at those floaters (prefix not checked — FRETURN paths diverge from oracle by design here)
#   COMPILE_FAIL / ASM_FAIL / TIMEOUT / SIG<n> / DIFF   everything else — the bugs to fix before only depth-spots remain
cd "$(dirname "$0")/.."
ROOT=$PWD   # include-bearing tests (-include 'lib/*.sno') resolve against the corpus checkout: sbl is CWD-relative, scrip honors SNO_LIB (s59 harness fix; both engines verified on test_case)
OUT=${1:-/tmp/descent_sweep.txt}; : > "$OUT"
S=/tmp/dsw.s; X=/tmp/dsw.x
for f in $(find test demo -name '*.sno' 2>/dev/null | sort); do
  ORA=$(cd /home/claude/corpus && /home/claude/x64/bin/sbl -b "$ROOT/$f" </dev/null 2>/dev/null)
  st=
  if ! SNO_LIB=/home/claude/corpus timeout 30 ./scrip --compile "$f" </dev/null > $S 2>/tmp/dsw.cerr; then st=COMPILE_FAIL
  elif ! gcc -no-pie $S -L out -lscrip_rt -Wl,-rpath,$PWD/out -o $X 2>/tmp/dsw.gerr; then st=ASM_FAIL
  else
    RO=$(timeout 8 $X </dev/null 2>/tmp/dsw.rerr); rc=$?
    if grep -q "BOMB-RETURN" /tmp/dsw.rerr; then
      case "$ORA" in "$RO"*) st=DESCENT_OK;; *) st=BOMB_RETURN_BAD;; esac
    elif grep -q "BOMB-FRETURN" /tmp/dsw.rerr; then st=BOMB_FRETURN
    elif grep -q "BOMB-NRETURN" /tmp/dsw.rerr; then st=BOMB_NRETURN
    elif [ $rc -eq 124 ]; then st=TIMEOUT
    elif [ $rc -ge 128 ]; then st=SIG$((rc-128))
    elif [ "$RO" = "$ORA" ]; then st=PASS
    else st=DIFF; fi
  fi
  echo "$st $f" >> "$OUT"
done
awk '{print $1}' "$OUT" | sort | uniq -c | sort -rn
echo "detail: $OUT"
