#!/usr/bin/env bash
# test_census_rbp_frames.sh -- GOAL-PASSTHRU-RBP-ERAD PT-0 instrument (Lon directive 2026-08-09: "Count how many RBP frames are being created through the product").
# Segments each regenerated corpus .s by defining label, classes every `mov rbp, rsp` establishment and `push rbp` by the label prefix of the region it sits in:
#   PAT-BLOB  = proc_PAT$* regions (BLOB-GRANT prologue pins + legacy in-blob ARBNO per-iteration rebases + CLASS D gamma resume-record pushes -- the record push is
#               protocol shape, not establishment; it dies with the blob at PT-4/PT-6)      <- deletion targets T1..T3
#   AB-ACT    = *_act_* activation blocks (the manual p.103 pushdown made literal)          <- LICENSED keeper (Lon confirm, GOAL-SNOBOL4-BB LADDER AB)
#   PROC      = other proc_* regions (DEFINE stubs, startup registration)
#   MAIN      = main*/unclassed regions (GLUE-O outer bracket, statement/fence machinery)   <- licensed census {STATEMENT/MATCH_BEGIN/FENCE1} + T5 residual
# Corpus .s files are HONEST CURRENT OUTPUT only at the regen cadence (artifact-truth law): demo + benchmarks regen at handoffs; crosscheck/patterns cadence
# unaudited -- its numbers are indicative, not gate-grade. Ratchet arming (fail on T-class rise) is PT-0's remaining step.
set -u
CORPUS="${1:-/home/claude/corpus}"
AWK='
/^[A-Za-z_.$][A-Za-z0-9_.$]*:/ { lbl=$1; sub(/:.*/,"",lbl);
  if (lbl ~ /^proc_PAT\$/) cls="PAT-BLOB"; else if (lbl ~ /_act_/) cls="AB-ACT"; else if (lbl ~ /^proc_/) cls="PROC"; else if (lbl ~ /^main/) cls="MAIN"; }
/mov +rbp, rsp/ { est[cls]++; te++ }
/push +rbp/     { psh[cls]++; tp++ }
END { for (c in est) printf "  %-9s est=%-5d push=%d\n", c, est[c], psh[c]+0;
      for (c in psh) if (!(c in est)) printf "  %-9s est=0     push=%d\n", c, psh[c];
      printf "  TOTAL     est=%-5d push=%d\n", te+0, tp+0 }'
for d in programs/snobol4/demo benchmarks/snobol4 crosscheck/patterns; do
  D="$CORPUS/$d"; [ -d "$D" ] || continue
  n=$(ls "$D"/*.s 2>/dev/null | wc -l); [ "$n" -gt 0 ] || { echo "$d: no .s"; continue; }
  nb=$(grep -lF 'proc_PAT$' "$D"/*.s 2>/dev/null | wc -l)
  nblob=$(cat "$D"/*.s 2>/dev/null | grep -cE '^proc_PAT\$[0-9]+_α:')
  echo "$d: files=$n blob-bearing=$nb PAT\$-blobs=$nblob"
  cat "$D"/*.s 2>/dev/null | awk "$AWK"
done
exit 0
