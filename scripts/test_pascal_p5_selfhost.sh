#!/usr/bin/env bash
# test_pascal_p5_selfhost.sh -- Pascal-P5 self-hosting, the SECOND Pascal milestone, with a WORK time per program
#
# Lon to the coo 2026-09-12, verbatim: "If P5 has self hosting, then make that a second milestone." and "For each P4
# and P5 self host test, ensure the speed performance is measured for each program." The P4 twin is
# scripts/test_pascal_p4_selfhost.sh and this script prints the same shape of verdict.
#
# P5 ships TWO halves and its own preprocessing step, which is not optional: source/pcom.pas (the compiler, 6771 lines)
# and source/pint.pas (the interpreter, 3168 lines) both begin with C-preprocessor lines that no Pascal compiler reads,
# fpc -Miso included. P5's own bin/pascpp runs cpp over them; this script runs the same cpp invocation directly so the
# step is visible in the log rather than hidden in a vendor wrapper.
#
# Verdict line: "P5_SELFHOST: pre=<rc> pcom=<rc|BLOCKED> pint=<rc|BLOCKED> gen1_pcode_lines=<n> gen2=<rc|BLOCKED> pcode_identical=<yes|no|n/a>"
# then "SELFHOST OK" (gen2 reproduced gen1's P-code), "SELFHOST PARTIAL" (both halves run, the P-code differs) or
# "SELFHOST BLOCKED" (a half cannot run, with the reason printed).
"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP_BIN:-$ROOT/scrip}"; [ -x "$SCRIP" ] || { echo "⛔ REFUSE(2): no scrip at $SCRIP"; exit 2; }
P5="${P5_DIR:-/home/resources/Pascal-P5}"; [ -d "$P5/source" ] || { echo "⛔ REFUSE(2): no Pascal-P5 source at $P5/source"; exit 2; }
command -v cpp >/dev/null || { echo "⛔ REFUSE(2): no cpp -- P5's own preprocessing step cannot run"; exit 2; }
W=$(mktemp -d) || exit 2; trap 'rm -rf "$W"' EXIT
TREE="SCRIP=$(git -C "$ROOT" rev-parse --short HEAD 2>/dev/null)$(git -C "$ROOT" diff --quiet 2>/dev/null || echo -DIRTY) RT_OPT=-O0 mode=m3 oracle-note=fpc-Miso-refuses-both-halves-unpreprocessed"
echo "P5 self-host on $TREE"
# ---- the preprocessing step, timed, one WORK line per half ---------------------------------------------------------
prerc=0
for h in pcom pint; do
  if [ "$h" = pcom ]; then DEFS="-DWRDSIZ32 -DSELF_COMPILE"; else DEFS="-DWRDSIZ32"; fi
  /usr/bin/time -f "%e %M" -o "$W/tp_$h" cpp -P -nostdinc -traditional-cpp $DEFS "$P5/source/$h.pas" > "$W/$h.pas" 2>"$W/$h.cpp.err"
  rc=$?; [ "$rc" -eq 0 ] || prerc=$rc
  read -r s k < <(tail -1 "$W/tp_$h"); case "$s" in ""|*[!0-9.]*) s=unmeasured; k=unmeasured;; esac
  echo "WORK $h.pas(preprocess, P5's own cpp step) wall=${s}s rss=${k}KB lines=$(wc -l < "$W/$h.pas")  ($TREE)"
done
[ "$prerc" -eq 0 ] || { echo "P5_SELFHOST: pre=$prerc pcom=BLOCKED pint=BLOCKED gen1_pcode_lines=0 gen2=BLOCKED pcode_identical=n/a"; echo "SELFHOST BLOCKED (P5's preprocessing step failed: rc=$prerc)"; exit 1; }
# ---- half 1: the compiler must run at all -------------------------------------------------------------------------
( cd "$W" && /usr/bin/time -f "%e %M" -o "$W/t1" timeout 600s "$SCRIP" --run pcom.pas < "$W/pcom.pas" > pcom.out 2> pcom.err ); pcomrc=$?
read -r s1 k1 < <(tail -1 "$W/t1"); case "$s1" in ""|*[!0-9.]*) s1=unmeasured; k1=unmeasured;; esac
echo "WORK pcom.pas(gen1, compiling its own preprocessed source) wall=${s1}s rss=${k1}KB  ($TREE)"
g1lines=$(wc -l < "$W/prr" 2>/dev/null); g1lines=${g1lines:-0}
echo "pcom: rc=$pcomrc out_lines=$(wc -l < "$W/pcom.out") pcode_lines=$g1lines $(grep -v '^Command' "$W/pcom.err" | head -c 160 | tr '\n' ' ')"
# ---- half 2: the interpreter must run at all ----------------------------------------------------------------------
( cd "$W" && /usr/bin/time -f "%e %M" -o "$W/t2" timeout 600s "$SCRIP" --run pint.pas < /dev/null > pint.out 2> pint.err ); pintrc=$?
read -r s2 k2 < <(tail -1 "$W/t2"); case "$s2" in ""|*[!0-9.]*) s2=unmeasured; k2=unmeasured;; esac
echo "WORK pint.pas(load probe, no P-code supplied) wall=${s2}s rss=${k2}KB  ($TREE)"
echo "pint: rc=$pintrc out_lines=$(wc -l < "$W/pint.out") first_line=$(head -1 "$W/pint.out" | cut -c1-60) $(grep -v '^Command' "$W/pint.err" | head -c 120 | tr '\n' ' ')"
if [ "$pcomrc" -ne 0 ] || [ "$g1lines" -eq 0 ]; then
  echo "P5_SELFHOST: pre=0 pcom=$pcomrc pint=$pintrc gen1_pcode_lines=$g1lines gen2=BLOCKED pcode_identical=n/a"
  echo "SELFHOST BLOCKED (the compiler half did not produce P-code: pcom rc=$pcomrc)"
  exit 1
fi
cp "$W/prr" "$W/gen1.pcode"
# ---- generation 2: the interpreter runs gen1's P-code on the same source -------------------------------------------
( cd "$W" && rm -f prr && cp gen1.pcode prd && /usr/bin/time -f "%e %M" -o "$W/t3" timeout 900s "$SCRIP" --run pint.pas < "$W/pcom.pas" > gen2.out 2> gen2.err ); gen2rc=$?
read -r s3 k3 < <(tail -1 "$W/t3"); case "$s3" in ""|*[!0-9.]*) s3=unmeasured; k3=unmeasured;; esac
echo "WORK pint.pas(gen2, running the gen1 P-code on pcom's own source) wall=${s3}s rss=${k3}KB  ($TREE)"
g2lines=$(wc -l < "$W/prr" 2>/dev/null); g2lines=${g2lines:-0}
echo "gen2: rc=$gen2rc out_lines=$(wc -l < "$W/gen2.out") pcode_lines=$g2lines $(grep -v '^Command' "$W/gen2.err" | head -c 160 | tr '\n' ' ')"
if [ "$gen2rc" -ne 0 ] || [ "$g2lines" -eq 0 ]; then
  echo "P5_SELFHOST: pre=0 pcom=0 pint=$pintrc gen1_pcode_lines=$g1lines gen2=BLOCKED pcode_identical=n/a"
  echo "SELFHOST BLOCKED (generation 2 did not finish: rc=$gen2rc)"; exit 1
fi
if cmp -s "$W/prr" "$W/gen1.pcode"; then
  echo "P5_SELFHOST: pre=0 pcom=0 pint=$pintrc gen1_pcode_lines=$g1lines gen2=0 pcode_identical=yes"; echo "SELFHOST OK"; exit 0
fi
echo "first differing line: $(diff "$W/prr" "$W/gen1.pcode" | head -2 | tr '\n' ' ' | cut -c1-160)"
echo "P5_SELFHOST: pre=0 pcom=0 pint=$pintrc gen1_pcode_lines=$g1lines gen2=0 pcode_identical=no"; echo "SELFHOST PARTIAL"; exit 1
