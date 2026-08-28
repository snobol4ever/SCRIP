#!/usr/bin/env bash
# bench_rep_loop_demos_snobol4.sh -- LON'S TWO-ASPECT PRESENTATION LAW for the tier-1 -match demos.
# Row: bench-rep-loop-demo-harness (hq_P, 2026-08-28). Authority: ARCH-BENCH-CAMPAIGN-README-TABLES.md.
#
#   ASPECT 1  command-line elapsed -- the WHOLE PROCESS as a user runs it (compile included and
#             LABELED), external rusage, never engine self-timing.
#   ASPECT 2  in-program bracket -- TIME() read by the program itself either side of the FIXTURE,
#             so the MATCH phase is isolated from the pattern-COMPILE phase.
#
# ⛔ WHY ASPECT 2 EXISTS AT ALL, MEASURED NOT ASSUMED: at their committed inputs these demos are
# startup/compile dominated -- treebank-match spends 99.3% of its instructions COMPILING patterns and
# 0.7% matching, claws5-match 99.7%, json-match ~100%. A match-path cure measured on aspect 1 alone
# reads as noise no matter how large it is; that is how the s168 PT-3 collapse (a MEASURED 1.41x) sat
# shipped-but-disabled for eight days with every board green.
#
# ⛔⛔ THE TRAP THIS HARNESS EXISTS TO NOT FALL INTO, AND IT COST A 13x WRONG ANSWER IN CALIBRATION:
# THE FIRST MATCH PAYS LAZY PATTERN COMPILATION. A rep loop with too few reps therefore measures
# COMPILE again, wearing a bracket. Measured on treebank-match, m3 vs the clean oracle:
#     reps=20   0.085x        <- warm-up dominated, WRONG BY 13x
#     reps=200  0.863x
#     reps=2000 1.017x        <- converged
#     reps=20000 1.128x
# ⭐ SO REPS IS NOT A CONSTANT AND IS NOT COMMITTED ANYWHERE. This harness RAMPS reps and REFUSES to
# report until the per-match figure CONVERGES (two successive ramp steps within TOL). A committed
# rep count would be a number nobody could tell had gone stale -- the same class as a scale column
# nobody can justify from a measurement.
#
# ⛔ THE BRACKET IS WRITTEN TO A FILE, NOT TO STDERR, and that is deliberate: SCRIP silently discards
# every write to an OUTPUT association on fd 2 (rc=0, no diagnostic -- see
# FINDING-2026-08-28-hq_P-scrip-output-association-to-fd2-stderr-silently-discards-writes.md). File
# association works in BOTH engines today, so the harness is not blocked on that defect. It also keeps
# stdout byte-identical, so every committed .ref and the cross-engine agreement gate stay untouched.
set -uo pipefail
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP:-$ROOT/scrip}"; RT="${RT_DIR:-$ROOT/out}"
D="${DEMO_DIR:-$S4E/corpus/demo/snobol4}"
MK="${MKREP:-$HERE/mkrep_demo_fixture.py}"
TARGET_MS="${TARGET_MS:-800}"     # bracket window we ramp toward
TOL="${TOL:-8}"                   # % convergence tolerance between successive ramp steps
MAXREPS="${MAXREPS:-4000000}"
ENGINES="${ENGINES:-sbl m3 m4}"
NOHUGE="${NOHUGE:-1}"; HEAP="${HEAP:-4096}"
. "$HERE/lib_oracle_flags.sh" 2>/dev/null || { echo "⛔ REFUSING: cannot load lib_oracle_flags.sh -- the ONE oracle-flag authority. A private fallback would time a DIFFERENT LANGUAGE (s189: -bf is the only correct arm)." >&2; exit 3; }
SBL="${SBL:-$(sbl_clean_bin)}"; SF="${SBLFLAGS:--d512m -i64m -s256m}"
[ -x "$SCRIP" ] || { echo "⛔ REFUSING: scrip not built at $SCRIP" >&2; exit 3; }
[ -x "$SBL" ]   || { echo "⛔ REFUSING: clean bench oracle missing at $SBL" >&2; exit 3; }
[ -f "$MK" ]    || { echo "⛔ REFUSING: fixture generator missing at $MK" >&2; exit 3; }
WRAP="$ROOT/tools/bench_rusage"
[ -x "$WRAP" ] || gcc -O2 -o "$WRAP" "$ROOT/tools/bench_rusage.c" 2>/dev/null || { echo "⛔ REFUSING: cannot build $WRAP -- aspect 1 needs microsecond external timing, and /usr/bin/time's 10 ms tick reports these demos as 0.0" >&2; exit 3; }
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
RC=0
# ---- one bracketed run; echoes "per_match_ns answer" or "" ----------------------------------------
brk() {  # $1=engine $2=variant.sno $3=input $4=reps $5=stem
  local eng="$2x" v="$2" in="$3" r="$4" stem="$5" out ns
  rm -f "$W/$stem.brk"
  case "$1" in
    sbl) out=$(timeout 600 "$SBL" $(sbl_lang_flags) $SF "$v" < "$in" 2>/dev/null | tail -1) ;;
    m3)  out=$(SCRIP_NOHUGE="$NOHUGE" SCRIP_HEAP_MB="$HEAP" timeout 600 "$SCRIP" "$v" < "$in" 2>/dev/null | tail -1) ;;
    m4)  [ -x "$W/$stem.bin" ] || { "$SCRIP" --compile -o "$W/$stem.s" "$v" </dev/null >/dev/null 2>&1 \
             && as -o "$W/$stem.o" "$W/$stem.s" 2>/dev/null \
             && gcc -no-pie -o "$W/$stem.bin" "$W/$stem.o" -L"$RT" -lscrip_rt -lm -Wl,-rpath,"$RT" 2>/dev/null; }
         [ -x "$W/$stem.bin" ] || { echo ""; return; }
         out=$(SCRIP_NOHUGE="$NOHUGE" SCRIP_HEAP_MB="$HEAP" timeout 600 "$W/$stem.bin" < "$in" 2>/dev/null | tail -1) ;;
  esac
  ns=$(grep -oE 'ns=[0-9]+' "$W/$stem.brk" 2>/dev/null | cut -d= -f2)
  case "$ns" in ''|*[!0-9]*) echo ""; return;; esac
  [ -n "$out" ] || { echo ""; return; }     # ⛔ empty answer is never a pass
  echo "$(awk -v n="$ns" -v r="$r" 'BEGIN{printf "%.6f", n/r}') $out"
}
# ---- ASPECT 2 with convergence ramp ---------------------------------------------------------------
aspect2() {  # $1=engine $2=prog $3=input $4=fam ; echoes "per_match_ns reps answer" or REFUSE reason
  local eng="$1" prog="$2" in="$3" fam="$4" r=1 prev="" cur="" ans="" v tot
  while [ "$r" -le "$MAXREPS" ]; do
    v="$W/$fam.$eng.$r.sno"
    # ⛔ the bracket path MUST use the same reps-keyed stem brk() reads, or the generator writes one file
    # and the reader looks for another -- which is exactly what the reps-keyed stem fix broke first time.
    python3 "$MK" "$prog" "$v" "$W/$fam.$eng.$r.brk" "$r" >/dev/null 2>&1 || { echo "REFUSE fixture-not-identified"; return; }
    # ⛔ THE STEM CARRIES THE REP COUNT. Without it the m4 arm builds its binary once (at reps=1) and
    # reuses it for every ramp step while dividing by the CURRENT r -- per-match times wrong by the
    # whole ramp factor, silently, with every run looking healthy. The variant CHANGES with reps, so
    # anything cached per-variant must be keyed by reps.
    local res; res=$(brk "$eng" "$v" "$in" "$r" "$fam.$eng.$r")
    [ -n "$res" ] || { echo "REFUSE no-bracket-or-empty-answer at reps=$r"; return; }
    cur=$(awk '{print $1}' <<<"$res"); ans=$(cut -d' ' -f2- <<<"$res")
    tot=$(awk -v c="$cur" -v r="$r" 'BEGIN{printf "%.0f", c*r/1e6}')
    if [ -n "$prev" ] && [ "$tot" -ge "$TARGET_MS" ]; then
      awk -v a="$prev" -v b="$cur" -v t="$TOL" 'BEGIN{d=(a>b?a-b:b-a); exit !(a>0 && d*100/a<=t)}' \
        && { echo "$cur $r $ans"; return; }
    fi
    prev="$cur"; r=$((r * 4))
  done
  echo "REFUSE did-not-converge-by-reps=$MAXREPS"
}
printf '%s\n' "=== TWO-ASPECT tier-1 demo board (Lon's presentation law) ===" \
  "  ASPECT 1 = whole process, external elapsed, COMPILE INCLUDED   |  ASPECT 2 = in-program bracket, MATCH ONLY" \
  "  oracle=$(basename "$(dirname "$SBL")")/$(basename "$SBL") -bf $SF | RT_OPT=-O0 | NOHUGE=$NOHUGE HEAP=${HEAP}MB | reps RAMPED to convergence (tol ${TOL}%)" \
  "  ⛔ aspect 1 is a TOTAL and aspect 2 is a SLOPE -- per the FACT RULE they may never share a column."
printf '%-26s %-4s %12s %14s %8s %10s\n' demo eng "A1_total_ms" "A2_match_ns" "reps" "A2_mult"
while IFS=$'\t' read -r fam prog inp; do
  case "$fam" in ''|\#*) continue;; esac
  P="$D/$prog"; IN="$D/$inp"
  [ -f "$P" ] && [ -f "$IN" ] || { echo "⛔ REFUSE $fam -- missing program or input ($prog / $inp); NAMED, not dropped."; RC=1; continue; }
  declare -A A2 ANS
  for e in $ENGINES; do
    # ASPECT 1: the ORIGINAL program, whole process, best of 3, via tools/bench_rusage.
    # ⛔ NOT /usr/bin/time -f %e -- that clock has 10 ms RESOLUTION and these demos run in single-digit
    # milliseconds, so it reported a flat "0.0" for treebank/claws5/json: a real measurement rendered as
    # a zero, which reads exactly like "too fast to matter" when it means "below my tick". bench_rusage
    # reports elapsed_ns and is already the ONE external-timing authority for the other bench harnesses.
    b=""
    for i in 1 2 3; do
      case "$e" in
        sbl) "$WRAP" timeout 600 "$SBL" $(sbl_lang_flags) $SF "$P" < "$IN" >/dev/null 2>"$W/ru" ;;
        m3)  SCRIP_NOHUGE="$NOHUGE" SCRIP_HEAP_MB="$HEAP" "$WRAP" timeout 600 "$SCRIP" "$P" < "$IN" >/dev/null 2>"$W/ru" ;;
        m4)  [ -x "$W/$fam.a1.bin" ] || { "$SCRIP" --compile -o "$W/$fam.a1.s" "$P" </dev/null >/dev/null 2>&1 && as -o "$W/$fam.a1.o" "$W/$fam.a1.s" 2>/dev/null && gcc -no-pie -o "$W/$fam.a1.bin" "$W/$fam.a1.o" -L"$RT" -lscrip_rt -lm -Wl,-rpath,"$RT" 2>/dev/null; }
             [ -x "$W/$fam.a1.bin" ] || continue
             SCRIP_NOHUGE="$NOHUGE" SCRIP_HEAP_MB="$HEAP" "$WRAP" timeout 600 "$W/$fam.a1.bin" < "$IN" >/dev/null 2>"$W/ru" ;;
      esac
      t=$(grep -oE 'elapsed_ns=[0-9]+' "$W/ru" 2>/dev/null | tail -1 | cut -d= -f2)
      case "$t" in ''|*[!0-9]*) continue;; esac
      [ -z "$b" ] && b="$t"; [ "$t" -lt "$b" ] && b="$t"
    done
    a1=$(awk -v b="${b:-0}" 'BEGIN{printf "%.2f", b/1e6}')
    r2=$(aspect2 "$e" "$P" "$IN" "$fam")
    case "$r2" in
      REFUSE*) printf '%-26s %-4s %12s %14s %8s %10s   ⛔ %s\n' "$fam" "$e" "$a1" "-" "-" "-" "$r2"; RC=1; continue;;
    esac
    A2[$e]=$(awk '{print $1}' <<<"$r2"); reps=$(awk '{print $2}' <<<"$r2"); ANS[$e]=$(cut -d' ' -f3- <<<"$r2")
    mult="-"; [ -n "${A2[sbl]:-}" ] && mult=$(awk -v s="${A2[sbl]}" -v x="${A2[$e]}" 'BEGIN{printf "%.3fx", (x>0)?s/x:0}')
    printf '%-26s %-4s %12s %14.1f %8s %10s\n' "$fam" "$e" "$a1" "${A2[$e]}" "$reps" "$mult"
  done
  # ⛔ correctness anchor: the ANSWER, never the exit code (sbl returns rc=0 while printing ERROR 246)
  ref=""; bad=0
  for e in $ENGINES; do [ -n "${ANS[$e]:-}" ] || continue; [ -z "$ref" ] && ref="${ANS[$e]}"; [ "${ANS[$e]}" = "$ref" ] || bad=1; done
  [ "$bad" = 0 ] && [ -n "$ref" ] || { echo "   ⛔ ANSWER DISAGREEMENT across engines for $fam -- timings above are NOT comparable"; RC=1; }
  unset A2 ANS
done <<'ROWS'
calculator-1-match	calculator/calculator-1-match.sno	calculator/calculator.input
calculator-1-match-fence	calculator/calculator-1-match-fence.sno	calculator/calculator.input
calculator-2-match	calculator/calculator-2-match.sno	calculator/calculator.input
calculator-2-match-fence	calculator/calculator-2-match-fence.sno	calculator/calculator.input
treebank-match	treebank/treebank-match.sno	treebank/treebank.input
treebank-match-fence	treebank/treebank-match-fence.sno	treebank/treebank.input
claws5-match	claws5/claws5-match.sno	claws5/claws5.input
claws5-match-fence	claws5/claws5-match-fence.sno	claws5/claws5.input
json-match	json/json-match.sno	json/json.input
json-match-fence	json/json-match-fence.sno	json/json.input
ROWS
exit "$RC"
