#!/usr/bin/env bash
# Three-way SNOBOL4 benchmark comparison: SCRIP mode-4 (AOT native) vs SPITBOL vs CSNOBOL4.
# Writes a TSV row per benchmark to $OUT as it goes (partial progress survives a timeout).
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
ROOT=$S4E/SCRIP
B=$S4E/corpus/benchmarks/snobol4
SBL=$S4E/x64/bin/sbl
CSN=$S4E/csnobol4/snobol4
RT=$ROOT/out
OUT=${OUT:-/tmp/cmp3.tsv}
T=${T:-90}
cd "$ROOT"
# light -> heavy ordering (so partial runs accumulate the cheap rows first)
ORDER="indirect_dispatch arith_loop op_dispatch pattern_bt var_access string_concat eval_fixed mixed_workload table_access roman fibonacci string_pattern string_manip func_call_overhead func_call eval_dynamic"
wall() { awk "BEGIN{printf \"%.0f\",($2-$1)*1000}"; }
selfms() { grep -i '^ms:' "$1" 2>/dev/null | grep -o '[0-9]\+' | head -1; }
sans() { grep -vi '^ms:' "$1" 2>/dev/null; }
run() { local t0 t1; t0=$(date +%s.%N); timeout "$T" "$@" </dev/null >"$CO" 2>"$CE"; RC=$?; t1=$(date +%s.%N); WALL=$(wall "$t0" "$t1"); }
printf "bench\tagree\tscrip_ms\tsbl_ms\tcsn_ms\tscrip_self\tsbl_self\tcsn_self\tscrip_rc\tsbl_rc\tcsn_rc\toutput\n" > "$OUT"
for s in $ORDER; do
  CO=/tmp/_c3.$s.o; CE=/tmp/_c3.$s.e
  # --- SCRIP mode-4: compile once, run the native binary ---
  ./scrip --compile "$B/$s.sno" >/tmp/_c3.$s.s 2>/dev/null
  if [ -s /tmp/_c3.$s.s ] && gcc -no-pie -x assembler /tmp/_c3.$s.s -L"$RT" -lscrip_rt -lm -Wl,-rpath,"$RT" -o /tmp/_c3.$s.bin 2>/dev/null; then
    run /tmp/_c3.$s.bin; SCW=$WALL; SCR=$RC; SCS=$(selfms "$CO"); sans "$CO" >/tmp/_c3.$s.scrip
  else
    SCW=- ; SCR=BUILDERR; SCS=- ; : >/tmp/_c3.$s.scrip
  fi
  # --- SPITBOL (sbl -b): exits 1 on benign sandbox segfault-on-exit; output still valid ---
  run "$SBL" -b "$B/$s.sno"; SBW=$WALL; SBR=$RC; SBS=$(selfms "$CO"); sans "$CO" >/tmp/_c3.$s.sbl
  # --- CSNOBOL4 ---
  run "$CSN" "$B/$s.sno"; CNW=$WALL; CNR=$RC; CNS=$(selfms "$CO"); sans "$CO" >/tmp/_c3.$s.csn
  # --- agreement (sans ms line); SCRIP is reference ---
  AG=AGREE
  if ! diff -q /tmp/_c3.$s.scrip /tmp/_c3.$s.sbl >/dev/null 2>&1; then AG=DIFF; fi
  if ! diff -q /tmp/_c3.$s.scrip /tmp/_c3.$s.csn >/dev/null 2>&1; then AG=DIFF; fi
  OUTSUM=$(head -1 /tmp/_c3.$s.scrip | cut -c1-46)
  printf "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n" "$s" "$AG" "$SCW" "$SBW" "$CNW" "${SCS:-?}" "${SBS:-?}" "${CNS:-?}" "$SCR" "$SBR" "$CNR" "$OUTSUM" >> "$OUT"
  echo "done $s: agree=$AG scrip=${SCW}ms sbl=${SBW}ms csn=${CNW}ms"
done
