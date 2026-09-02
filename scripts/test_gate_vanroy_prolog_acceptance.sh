#!/bin/bash
# test_gate_vanroy_prolog_acceptance.sh -- the PERF-SIDE ACCEPTANCE INSTRUMENT of the PROLOG REDESIGN program (Lon escalation 2026-09-02 10:00, ceo-routed; hq_P owns P6 and P7).
#
# ⭐ WHAT IT GRADES: the 21 van Roy kernels under corpus/benchmarks/prolog/vanroy/, mode 3. Baseline measured by hq_P at SCRIP fa12d7cb, -O0:
#     3 CLEAN · 8 REFUSE (rc=134) · 8 SIGSEGV (rc=139) · 2 rc=1
# The 8 REFUSE are derive deriv divide10 ham meta_qsort queens sendmore tak, and every one is the SAME failure -- "SCRIP FATAL: pl_trail_unwind refuses corrupt trail mark ... its PRODUCER handed
# over garbage" -- i.e. the PZ-4 trail-mark-corruption class. That is why PZ-4 (hq_C) lands first and P7/P6 sequence behind it.
#
# ⛔⛔ WHY IT DOES NOT GRADE "REFUSE 0" ALONE, WHICH IS WHAT THE PROGRAM WAS FIRST SCOPED AS (hq_P raised it, and it is the single most important line in this file): THE REFUSAL IS A GUARD.
# `pl_trail_unwind` REFUSES because it caught a corrupt trail mark before acting on it. So a change that turns a REFUSE into a SIGSEGV PASSES a REFUSE-0 criterion -- and so does DELETING THE GUARD.
# ⭐ THAT IS NOT HYPOTHETICAL HERE: there are ALREADY 8 SIGSEGVs sitting beside the 8 REFUSEs, so "make the refusals stop" has a shorter path than curing them, and it ends at a board that is strictly
# worse than today while reading green. An acceptance test whose cheapest satisfying change is a regression is the vacuous-test class Lon flagged, aimed at the row Lon escalated.
# ✅ SO ACCEPTANCE IS ALL THREE AT ONCE: REFUSE 0 **and** SIGSEGV 0 **and** CLEAN never below the pinned floor. Any one alone is gameable; together they are not.
#
# ⛔ NEVER LOWER A PIN TO REACH GREEN. Raising CLEAN_FLOOR after a real cure is the intended motion and belongs in the same commit as the cure, citing the FINDING.
#
# ⛔⛔⭐ AND IT REPEATS EACH KERNEL, BECAUSE A SINGLE PASS OVER THIS BOARD IS NOT A MEASUREMENT. MEASURED hq_P 2026-09-02 on ONE UNCHANGED BINARY (SCRIP fa12d7cb), four consecutive full passes:
#   CLEAN 3,4,4,4 · REFUSE 8,4,7,5 · CRASH 8,11,8,10
# ELEVEN OF THE 21 KERNELS FLIP between REFUSE, CRASH and CLEAN run to run with no change of any kind -- derive deriv divide10 ham log10 meta_qsort ops8 queens sendmore tak times10. `tak` reads R.R.
# (clean half the time); `deriv` reads .RRR. ⛔ SO "REFUSE 0" AS A SINGLE-PASS CRITERION IS NOT REPRODUCIBLE: a seat can reach it BY CHANCE, and can read a real cure as a regression, in the same hour.
# ⭐ THE CURE FOR THE INSTRUMENT IS WORST-OF-N: each kernel runs REPS times and is classified by its WORST outcome (CRASH > REFUSE > other > CLEAN). That is monotone and stable -- a kernel counts as
# CLEAN only if it is clean EVERY time -- so the board stops flapping and a cure has to hold, not merely occur once.
# ⭐ THE FLAPPING IS ALSO EVIDENCE, AND IT BELONGS IN THE DESIGN PAGE: a corrupt trail mark that sometimes crashes and sometimes gets CAUGHT by pl_trail_unwind is reading memory whose content varies
# between runs -- consistent with the retained frame living BELOW rsp and being clobbered by whatever the next call happens to write there. That is PZ-4's clause (a) mechanism, observed from the outside.
set -u
R="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the SEAT ROOT is derived from THIS script, never a hardcoded seat (hq_C measured 2026-09-02: the old /home/claude_P default graded hq_P's checkout from hq_C's seat)

CLEAN_FLOOR=3      # WORST-OF-REPS, measured hq_P 2026-09-02, SCRIP fa12d7cb, -O0, mode 3: fib nrev queens_8 are clean on every pass. Raise it WITH a cure, never lower it.
REPS="${VANROY_REPS:-3}"   # worst-of-N. 1 is NOT a valid setting for a verdict -- see the header; it is offered only for a quick eyeball.
cd "$R/SCRIP" 2>/dev/null || { echo "⛔ REFUSED (rc=2): no $R/SCRIP"; exit 2; }
[ -x ./scrip ] || { echo "⛔ REFUSED (rc=2): ./scrip is not built -- a board built on a missing binary is a plausible all-FAIL table, not a measurement"; exit 2; }
D="$R/corpus/benchmarks/prolog/vanroy"
[ -d "$D" ] || { echo "⛔ REFUSED (rc=2): $D missing -- the kernels moved; re-point this gate rather than shrinking the board"; exit 2; }
n=0; clean=0; refuse=0; segv=0; other=0; refuse_l=""; segv_l=""; other_l=""
for f in "$D"/*.pl; do
    [ -f "$f" ] || continue
    n=$((n+1)); b="$(basename "$f" .pl)"; worst=0; seen=""
    # worst-of-REPS: 3 CRASH > 2 REFUSE > 1 other > 0 CLEAN. A kernel is CLEAN only if it is clean EVERY pass.
    i=0; while [ $i -lt "$REPS" ]; do i=$((i+1))
        out="$(timeout 60s ./scrip "$f" < /dev/null 2>&1)"; rc=$?
        if   printf '%s' "$out" | grep -qi 'refuses\|REFUSE'; then k=2; seen="${seen}R"
        elif [ $rc -eq 139 ] || [ $rc -eq 134 ];              then k=3; seen="${seen}C"
        elif [ $rc -ne 0 ];                                    then k=1; seen="${seen}o"
        else                                                        k=0; seen="${seen}."; fi
        [ $k -gt $worst ] && worst=$k
    done
    flap=""; [ "$(printf '%s' "$seen" | grep -o . | sort -u | tr -d '\n' | wc -c)" -gt 1 ] && flap="!"
    case $worst in
      3) segv=$((segv+1));   segv_l="$segv_l $b[$seen]$flap" ;;
      2) refuse=$((refuse+1)); refuse_l="$refuse_l $b[$seen]$flap" ;;
      1) other=$((other+1));  other_l="$other_l $b[$seen]$flap" ;;
      0) clean=$((clean+1)) ;;
    esac
done
[ "$n" -gt 0 ] || { echo "⛔ REFUSED (rc=2): zero kernels found under $D -- an empty board is not a green board"; exit 2; }
echo "van Roy Prolog acceptance ($n kernels, mode 3, worst-of-$REPS): CLEAN=$clean (floor $CLEAN_FLOOR) · REFUSE=$refuse · CRASH=$segv · other=$other"
echo "  [] shows the per-rep outcome string (. clean, R refuse, C crash, o other); a trailing ! marks a kernel that FLIPPED between reps -- 11 of 21 do so on an unchanged binary."
[ -n "$refuse_l" ] && echo "  REFUSE:$refuse_l"
[ -n "$segv_l" ]   && echo "  CRASH :$segv_l"
[ -n "$other_l" ]  && echo "  other :$other_l"
F=0
[ "$refuse" -eq 0 ] || { echo "  ⛔ REFUSE=$refuse, must be 0"; F=1; }
[ "$segv"   -eq 0 ] || { echo "  ⛔ CRASH=$segv, must be 0 -- a refusal converted into a crash is a REGRESSION, not progress"; F=1; }
[ "$clean" -ge "$CLEAN_FLOOR" ] || { echo "  ⛔ CLEAN=$clean fell below the pinned floor $CLEAN_FLOOR -- kernels that passed no longer do"; F=1; }
[ $F -eq 0 ] && { echo "PASS: REFUSE 0, CRASH 0, CLEAN $clean >= $CLEAN_FLOOR"; exit 0; }
echo "GATE FAIL(1) [vanroy_prolog_acceptance]"
exit 1
