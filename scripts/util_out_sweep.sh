#!/usr/bin/env bash
# util_out_sweep.sh — per-program mode-3 RUN-OUTPUT md5 sweep, for behavioural blast-radius gates.
#   bash scripts/util_out_sweep.sh OUT.md5 [LISTFILE]        SCRIP=/path/to/scrip selects the arm
# The OUTPUT counterpart to util_s_md5_sweep.sh (which hashes --compile .s text and cannot see a runtime defect).
# Compare arms with:  diff A.md5 B.md5.  Row format:  <md5>|<rc>\t<path>.
#
# ⛔ THIS IS THE INSTRUMENT s148 USED UNSOUNDLY AND s149 COULD NOT NAME (s149 NEXT-SEAT item 1).  NAMED AND FIXED
# HERE, s150, mechanism measured not guessed.  s148 saw "7 of 510 differ run-to-run under a FIXED .so" and split
# them into "5 already-crashing + 2 TIME() deltas".  Both halves are ONE root cause with TWO faces, and the second
# face is created BY THE SWEEP ITSELF:
#   (a) rc!=0 runs emit nondeterministic text (partial stdout + a crash message racing the flush), so hashing a
#       dead run manufactures movers.  Same s33 "non-empty is not alive" class util_s_md5_sweep.sh fixed at s149.
#   (b) ⛔ ELAPSED-TIME FIELDS TRACK MACHINE LOAD, SO ANY *PARALLEL* SWEEP IS SELF-POISONING.  Measured s150:
#       demo/calculator-1 and calculator-2 run SERIALLY are byte-identical to each other and stable across 3 runs
#       (md5 fd95ab76); run under `xargs -P $(nproc)` they take TWO different md5s, match NEITHER serial value, and
#       SWAP with each other between runs -- which reads exactly like cross-contamination and is not.  Root cause
#       proven by diffing the text, not the hash: `match_ms=0` serial vs `match_ms=1` under load, stdout otherwise
#       byte-identical.  demo/json.sno is the same shape (`match_ms=`, TIME() at :273/:275) and was likewise a
#       FALSE mover in the s150 CN-8 A/B -- serially it is 8d262c4d on BOTH arms.
# CURE, both faces: rc is recorded and a failed run is labelled RUN_RC_<rc> (stable in both arms, never hashed);
# and every elapsed-time field is normalised to a constant BEFORE hashing, which makes the sweep parallel-safe
# instead of merely parallel-fast.  ⛔ NORMALISE, DO NOT EXCLUDE: dropping the programs would blind the sweep to
# real defects in exactly the demos (json, calculator) that exercise the biggest pattern graphs in the corpus.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; SC="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP:-$SC/scrip}"; CORPUS="${CORPUS:-$S4E/corpus}"; TMO="${TMO:-25}"
OUT="${1:?usage: util_out_sweep.sh OUT.md5 [LISTFILE]}"; LIST="${2:-}"
T="$(mktemp)"; trap 'rm -f "$T"' EXIT
if [ -n "$LIST" ]; then cp "$LIST" "$T"; else
  { find "$CORPUS/crosscheck" -name '*.sno'; find "$CORPUS/probe/bb" -name '*.sno'; find "$CORPUS/probe/cn" -name '*.sno'
    find "$CORPUS/probe/kw" -name '*.sno'; find "$CORPUS/probe/arbnostore" -name '*.sno'
    find "$CORPUS/programs/snobol4/demo" -maxdepth 1 -name '*.sno'; } | sort > "$T"; fi
run1() { local p="$1" d o rc; d="$(dirname "$p")"
  o="$(cd "$d" && SNO_LIB="$d" timeout "$TMO" "$SCRIP" --run "$p" </dev/null 2>&1)"; rc=$?
  if [ "$rc" -ne 0 ]; then printf 'RUN_RC_%s\t%s\n' "$rc" "$p"; return; fi
  printf '%s\t%s\n' "$(printf '%s' "$o" | sed -E 's/(match_ms|_ms|msec|elapsed|execution time msec)[= ]+[0-9]+/\1=<T>/g' | md5sum | cut -d' ' -f1)" "$p"; }
export -f run1; export SCRIP TMO
xargs -a "$T" -P "$(nproc)" -I{} bash -c 'run1 "$@"' _ {} | sort -k2 > "$OUT"
echo "$(wc -l < "$OUT") programs -> $OUT"
