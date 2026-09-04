#!/usr/bin/env bash
# bench_triangulate_snocone.sh -- Snocone benchmark board vs its SNOBOL4 twin (row
# readme-perf-grids-three-angles-all-seven, I26). Snocone compiles into the same engine as SNOBOL4
# (README § Snocone), so "the SNOBOL4 twin" -- the identical kernel hand-translated into SNOBOL4 syntax,
# both graded against the same output -- is the fair rival here, per this task's own GOAL text.
#
# METHOD: callgrind Ir per kernel (valgrind --tool=callgrind against SCRIP m4 and against the CLEAN
# SPITBOL benchmark oracle), NOT wall-clock -- per hq_P's live-load ruling this session (box at 20-30
# load, 16-seat fleet running; wall-clock is noise right now, callgrind is contention-immune). Multiple =
# SPITBOL_Ir / SCRIP_Ir (reference/ours, the FASTER axis -- RULES.md § THE TWO-NUMBER BENCHMARK BASIS's
# unit rule; above 1.00x SCRIP is faster). m3 Ir is reported separately, never folded into the m4-vs-SPITBOL
# multiple, because m3's total includes SCRIP's own in-process compile (same caveat as the Icon demo grid).
# ⛔ ORACLE: the CLEAN benchmark oracle only (lib_oracle_flags.sh:sbl_clean_bin(), -bf) -- never
# x64/bin/sbl, whose monitor-IPC bridge costs ~2.2-3.5x Ir and would misreport every number here.
#
# EXIT: 0 = every kernel measured on both engines. 2 = REFUSED -- missing scrip/valgrind/oracle/corpus.
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
S4E="${S4E_HOME:-$(cd "$HERE/../.." && pwd)}"
SCRIP_BIN="${SCRIP:-$HERE/../scrip}"
RT_DIR="$HERE/../out"
SC_DIR="$S4E/corpus/benchmarks/snocone"
SNO_DIR="$S4E/corpus/benchmarks/snobol4"
# kernel:twin-sno-basename pairs -- string_concat's SNOBOL4 twin is named _twin to avoid colliding
# with the pre-existing string_concat.sno kernel (a different, unrelated benchmark).
KERNELS="fib_recur:fib_recur arith_loop:arith_loop_twin string_concat:string_concat_twin"

if [ ! -x "$SCRIP_BIN" ]; then echo "⛔ REFUSED (rc=2): scrip not built at $SCRIP_BIN"; exit 2; fi
if ! command -v valgrind >/dev/null 2>&1; then echo "⛔ REFUSED (rc=2): valgrind not on PATH"; exit 2; fi
. "$HERE/lib_oracle_flags.sh" 2>/dev/null || { echo "⛔ REFUSED (rc=2): lib_oracle_flags.sh unloadable"; exit 2; }
SBL="$(sbl_clean_bin)"
[ -x "$SBL" ] || { echo "⛔ REFUSED (rc=2): clean SPITBOL oracle absent: $SBL"; exit 2; }
SBL_FLAGS="$(sbl_lang_flags)"

ir_of() {  # $1.. = argv -> echoes PROGRAM TOTALS Ir
  local out; out="$(mktemp -d)"
  valgrind --tool=callgrind --callgrind-out-file="$out/cg.out" "$@" >"$out/prog.out" 2>"$out/vg.log"
  callgrind_annotate "$out/cg.out" 2>/dev/null | awk '/PROGRAM TOTALS/{gsub(/,/,"",$1); print $1}'
  rm -rf "$out"
}

echo "SNOCONE_TRIANGULATE -- callgrind Ir vs SPITBOL (clean oracle $SBL_FLAGS), RT_OPT=-O0, tree $(git -C "$HERE/.." rev-parse --short HEAD 2>/dev/null || echo '?')"
echo "kernel               m3_Ir(compile+run)   m4_Ir(run only)      SPITBOL_Ir      × (m4 vs SPITBOL)"
N=0
for pair in $KERNELS; do
  k="${pair%%:*}"; twin="${pair##*:}"
  scf="$SC_DIR/$k.sc"; snf="$SNO_DIR/$twin.sno"
  [ -f "$scf" ] || { echo "⛔ REFUSED (rc=2): kernel missing: $scf"; exit 2; }
  [ -f "$snf" ] || { echo "⛔ REFUSED (rc=2): twin missing: $snf"; exit 2; }
  m3ir="$(ir_of "$SCRIP_BIN" --run "$scf" < /dev/null)"
  s="$(mktemp -d)"
  "$SCRIP_BIN" --compile "$scf" -o "$s/p.s" < /dev/null 2>/dev/null || { echo "⛔ REFUSED (rc=2): $k failed to compile mode-4"; exit 2; }
  gcc "$s/p.s" -o "$s/p.bin" -L"$RT_DIR" -lscrip_rt -Wl,-rpath,"$RT_DIR" 2>/dev/null || { echo "⛔ REFUSED (rc=2): $k failed to link mode-4"; exit 2; }
  m4ir="$(ir_of "$s/p.bin" < /dev/null)"
  rm -rf "$s"
  sblir="$(ir_of "$SBL" $SBL_FLAGS "$snf" < /dev/null)"
  mult="$(awk -v r="$sblir" -v o="$m4ir" 'BEGIN{ if (o+0==0) print "n/a"; else printf "%.3fx", r/o }')"
  printf '%-20s %18s %18s %18s   %s\n' "$k" "${m3ir:-REFUSED}" "${m4ir:-REFUSED}" "${sblir:-REFUSED}" "$mult"
  N=$((N+1))
done
echo "SNOCONE_BOARD kernels=$N"
[ "$N" -gt 0 ]
