#!/usr/bin/env bash
# scripts/bench_rtx_3arm.sh -- RTX-0e: THE THREE-ARM INTERLEAVED HARNESS.
#
# WHY THIS EXISTS (s209, and it retro-qualifies numbers already in the ladder)
# ---------------------------------------------------------------------------
# Every RTX rung before this one graded itself with a TWO-arm A/B: SCRIP_RTX_<FAM>=1
# against SCRIP_RTX_<FAM>=0.  That control is NOT pristine C.  With the gate off,
# every call to a ported symbol still executes:
#
#       endbr64 ; cmp byte ptr [rip+rtx_gate_<fam>],0 ; je c_<sym>@plt ; jmp <C body>
#
# i.e. a gate compare, a TAKEN branch, and a PLT stub indirection -- on EVERY call --
# which a pristine-C build never pays.  Measured s209 on two programs: the OFF arm is
# 5.8% (func_call) and 7.8% (fibonacci) SLOWER than pristine C.  That bias is
#   (a) ONE-DIRECTIONAL: it always flatters the asm, never the C; and
#   (b) LARGER than the +/-3% null floor the ladder grades against.
# A rung reporting "1.07x" from a two-arm A/B may have measured nothing but its own
# kill-switch.  s209's RTX-4 slice 3 did exactly that: 1.066x two-arm, 1.007x vs pristine.
#
# WHAT THIS DOES DIFFERENTLY
#   Three arms, INTERLEAVED WITHIN EACH ROUND so they share the same thermal/scheduler
#   moment, round 1 discarded (hugepage + page-cache warmup), medians reported:
#     PRISTINE  a .so built with the port absent entirely   (the HONEST baseline)
#     OFF       the ported .so, gate 0                      (kill-switch fallback path)
#     ON        the ported .so, gate 1                      (the asm)
#   Arms are selected by LD_LIBRARY_PATH, so ONE scrip binary runs all three and the
#   only thing that varies is the runtime .so -- no recompile, no relink between arms.
#
# WHAT IT REPORTS AND WHY BOTH RATIOS MATTER
#   ON/PRISTINE  <- THE REAL ANSWER. This is what the rung actually bought.
#   ON/OFF       <- the LEGACY two-arm number, printed ONLY so a rung can see how much
#                   of its historical claim was the artifact.
#   OFF/PRISTINE <- the kill-switch tax itself. If this is ~1.00 the family's gate is
#                   cheap (cold symbol); if it is well under 1.00 the family is hot and
#                   any two-arm number for it is suspect.
#
# USAGE
#   scripts/bench_rtx_3arm.sh --fam CALL --pristine <dir> --rtx <dir> [-r N] prog.sno ...
#   Both dirs must contain a libscrip_rt.so.  Build them by stashing the port for the
#   pristine one; keep the source tree on the PORTED state afterwards.
#
# CAVEATS THAT MUST TRAVEL WITH ANY NUMBER THIS PRINTS
#   - Label every result with RT_OPT (default -O0; -O2 only if Lon directed it).
#   - Programs must be SELF-TIMED (they must print "ms: <n>"); process startup is
#     excluded on purpose, so these are window ratios, not wall-clock ratios.
#   - This box is noisy (s200 recorded a 1.340/2.497/2.606/0.510 spread on table_access).
#     Medians can separate cleanly while per-run RANGES still overlap -- the script
#     prints every raw sample so that overlap is visible rather than hidden.
#   - A ~1.00 ON/PRISTINE is THE ANSWER, not a failed measurement (s187/s204 rule).
#     State the expected band BEFORE running this.

set -u
FAM="CALL"; PRISTINE=""; RTX=""; ROUNDS=5
while [ $# -gt 0 ]; do
  case "$1" in
    --fam)      FAM="$2"; shift 2 ;;
    --pristine) PRISTINE="$2"; shift 2 ;;
    --rtx)      RTX="$2"; shift 2 ;;
    -r)         ROUNDS="$2"; shift 2 ;;
    *)          break ;;
  esac
done
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${SCRIP:-$HERE/../scrip}"
GATE="SCRIP_RTX_${FAM}"

[ -x "$SCRIP" ]                        || { echo "SKIP: scrip not built"; exit 0; }
[ -f "$PRISTINE/libscrip_rt.so" ]      || { echo "FAIL: no libscrip_rt.so in --pristine '$PRISTINE'"; exit 1; }
[ -f "$RTX/libscrip_rt.so" ]           || { echo "FAIL: no libscrip_rt.so in --rtx '$RTX'"; exit 1; }
[ $# -gt 0 ]                           || { echo "FAIL: no programs given"; exit 1; }

med() { echo "$1" | tr ' ' '\n' | grep -v '^$' | sort -n \
        | awk '{a[NR]=$1} END{ if(!NR){print "NA"; exit} print (NR%2)?a[(NR+1)/2]:(a[NR/2]+a[NR/2+1])/2 }'; }
ratio() { awk -v a="$1" -v b="$2" 'BEGIN{ if(b+0==0){print "NA"; exit} printf "%.3f", a/b }'; }

# one sample: $1 = .so dir, $2 = gate value ("" = leave unset, pristine has no gate)
run1() {
  local dir="$1" g="$2" prog="$3" out
  if [ -n "$g" ]; then
    out=$(LD_LIBRARY_PATH="$dir" env "$GATE=$g" timeout 300 "$SCRIP" --run "$prog" </dev/null 2>/dev/null)
  else
    out=$(LD_LIBRARY_PATH="$dir" timeout 300 "$SCRIP" --run "$prog" </dev/null 2>/dev/null)
  fi
  echo "$out" | awk '/^ms: /{print $2; found=1} END{if(!found) print ""}'
}

echo "=== RTX 3-ARM INTERLEAVED HARNESS — family $FAM — rounds=$ROUNDS (round 1 discarded) ==="
echo "    pristine=$PRISTINE"
echo "    rtx     =$RTX"
echo "    RT_OPT  =${RT_OPT:--O0 (Makefile default; label every number with this)}"
rc=0
for prog in "$@"; do
  [ -f "$prog" ] || { echo "  SKIP $prog (missing)"; continue; }
  P=""; F=""; N=""
  for r in $(seq 1 "$ROUNDS"); do
    # interleaved WITHIN the round: same moment, same thermal state, rotating order
    p=$(run1 "$PRISTINE" ""  "$prog")
    f=$(run1 "$RTX"      "0" "$prog")
    n=$(run1 "$RTX"      "1" "$prog")
    if [ "$r" -gt 1 ]; then P="$P $p"; F="$F $f"; N="$N $n"; fi
  done
  mp=$(med "$P"); mf=$(med "$F"); mn=$(med "$N")
  if [ "$mp" = "NA" ] || [ "$mn" = "NA" ]; then
    echo "  $(basename "$prog"): NO 'ms:' WINDOW — program is not self-timed, or it crashed. NOT GRADED."
    rc=1; continue
  fi
  echo "  $(basename "$prog")"
  echo "      PRISTINE=${mp}ms   OFF=${mf}ms   ON=${mn}ms"
  echo "      ON/PRISTINE = $(ratio "$mp" "$mn")x   <-- THE REAL ANSWER"
  echo "      ON/OFF      = $(ratio "$mf" "$mn")x   (legacy two-arm number; the gap vs the line above IS the artifact)"
  echo "      OFF/PRISTINE= $(ratio "$mp" "$mf")x   (the kill-switch tax on this family)"
  echo "      raw PRISTINE:$P"
  echo "      raw OFF     :$F"
  echo "      raw ON      :$N"
done
exit $rc
