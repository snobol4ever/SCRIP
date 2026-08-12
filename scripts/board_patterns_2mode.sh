#!/usr/bin/env bash
# board_patterns_2mode.sh -- two-mode (m3 --run / m4 --compile) census over crosscheck/patterns.
# Reports per-program AGREE (both modes match .ref) / m3-only-fail / m4-only-fail / both-fail,
# with failure-mode tags (DIFF/HANG/SIGnn/rcN). Self-contained per RULES.md.
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${SCRIP_BIN:-$HERE/../scrip}"
RT_DIR="${RT_DIR:-$HERE/../out}"
DIR="${PAT_CORPUS:-/home/claude/corpus/crosscheck/patterns}"
PER="${PAT_TIMEOUT:-15}"
OUT="${OUT_TSV:-/tmp/patterns_2mode.tsv}"

[ -x "$SCRIP" ] || { echo "no scrip at $SCRIP"; exit 1; }
[ -d "$DIR" ] || { echo "no corpus at $DIR"; exit 1; }

WORKDIR="$(mktemp -d)"
trap 'rm -rf "$WORKDIR"' EXIT

classify_rc() {
  local rc="$1"
  if   [ "$rc" -eq 124 ]; then echo HANG
  elif [ "$rc" -ge 128 ]; then echo "SIG$((rc-128))"
  elif [ "$rc" -ne 0   ]; then echo "rc$rc"
  else echo DIFF
  fi
}

compile_m4() {
  local sno="$1" out="$2" tmp
  tmp="$(mktemp -d)"
  "$SCRIP" --compile "$sno" > "$tmp/p.s" 2>/dev/null || { rm -rf "$tmp"; return 1; }
  [ -s "$tmp/p.s" ] || { rm -rf "$tmp"; return 1; }
  gcc -c "$tmp/p.s" -o "$tmp/p.o" 2>/dev/null || { rm -rf "$tmp"; return 1; }
  gcc -no-pie "$tmp/p.o" -L"$RT_DIR" -lscrip_rt -lm -Wl,-rpath,"$RT_DIR" -o "$out" 2>/dev/null || { rm -rf "$tmp"; return 1; }
  rm -rf "$tmp"; return 0
}

: > "$OUT"
AGREE=0; M3ONLY=0; M4ONLY=0; BOTHFAIL=0; TOTAL=0
cd "$DIR" || exit 1
for s in *.sno; do
  b="${s%.sno}"
  [ -f "$b.ref" ] || continue
  TOTAL=$((TOTAL+1))
  exp="$(cat "$b.ref")"
  IN=/dev/null; [ -f "$b.input" ] && IN="$b.input"

  got3=$(timeout "$PER" "$SCRIP" --run "$s" < "$IN" 2>/dev/null); rc3=$?
  if [ $rc3 -eq 0 ] && [ "$got3" = "$exp" ]; then m3=PASS; else m3=$(classify_rc $rc3); fi

  bin="$WORKDIR/${b}.bin"
  if compile_m4 "$s" "$bin"; then
    got4=$(timeout "$PER" "$bin" < "$IN" 2>/dev/null); rc4=$?
    if [ $rc4 -eq 0 ] && [ "$got4" = "$exp" ]; then m4=PASS; else m4=$(classify_rc $rc4); fi
  else
    m4=BUILDFAIL
  fi

  echo -e "$b\t$m3\t$m4" >> "$OUT"

  if   [ "$m3" = PASS ] && [ "$m4" = PASS ]; then AGREE=$((AGREE+1))
  elif [ "$m3" = PASS ] && [ "$m4" != PASS ]; then M4ONLY=$((M4ONLY+1))
  elif [ "$m3" != PASS ] && [ "$m4" = PASS ]; then M3ONLY=$((M3ONLY+1))
  else BOTHFAIL=$((BOTHFAIL+1))
  fi
done

echo "TOTAL $TOTAL  AGREE(both PASS) $AGREE  m4-only-fail $M4ONLY  m3-only-fail $M3ONLY  both-fail $BOTHFAIL"
echo "tsv: $OUT"
