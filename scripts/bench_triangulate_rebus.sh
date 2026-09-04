#!/usr/bin/env bash
# bench_triangulate_rebus.sh -- Rebus benchmark board (row readme-perf-grids-three-angles-all-seven,
# I26). ⛔ WHY THIS DIFFERS FROM THE OTHER TRIANGULATORS: Rebus has NO independent rival implementation
# to compare against (README § Rebus: "no independent implementation to borrow a suite from") -- unlike
# Snocone, which shares SNOBOL4's engine closely enough that "the SNOBOL4 twin" is a fair, apples-to-apples
# rival, Rebus is Icon-lineage (Griswold TR 84-9) with no production implementation at all. Fabricating a
# "× vs X" ratio with no X would be exactly the unit-mismatch class RULES.md § THE INSTRUMENT LAWS warns
# against (a correction/comparison that does not show what it is commensurable with is not one) -- so this
# board reports SCRIP's own m3/m4 instruction counts, never a synthesized external multiple.
#
# METHOD: callgrind Ir per kernel (valgrind --tool=callgrind), NOT wall-clock -- per hq_P's live-load
# ruling this session (box at 20-30 load, 16-seat fleet running; wall-clock numbers taken now are noise,
# callgrind is contention-immune). m4 Ir is pure execution (prebuilt binary); m3 Ir is a WHOLE-PROCESS
# total that includes SCRIP's own in-process compile-to-slab step, same caveat the Icon demo grid already
# states (README § Icon, "m3 compiles at run time, so its total includes the compile") -- so m3 and m4
# are never presented as if they measure the same thing, and no m3-vs-m4 multiple is published either
# (that WOULD be commensurable, but "faster because it skipped compiling" is not the question this board
# asks; a future clock-hook inside the JIT path could split it, not done here).
#
# EXIT: 0 = every kernel measured. 2 = REFUSED -- missing scrip/valgrind/corpus, loud, never a fabricated row.
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
S4E="${S4E_HOME:-$(cd "$HERE/../.." && pwd)}"
SCRIP_BIN="${SCRIP:-$HERE/../scrip}"
RT_DIR="$HERE/../out"
KDIR="$S4E/corpus/benchmarks/rebus"
KERNELS="fib_recur arith_loop string_concat"

if [ ! -x "$SCRIP_BIN" ]; then echo "⛔ REFUSED (rc=2): scrip not built at $SCRIP_BIN"; exit 2; fi
if ! command -v valgrind >/dev/null 2>&1; then echo "⛔ REFUSED (rc=2): valgrind not on PATH -- callgrind Ir is this board's whole method"; exit 2; fi
if [ ! -d "$KDIR" ]; then echo "⛔ REFUSED (rc=2): no kernel dir at $KDIR"; exit 2; fi

ir_of() {  # $1 = argv... -> echoes PROGRAM TOTALS Ir for running that command under callgrind
  local out; out="$(mktemp -d)"
  valgrind --tool=callgrind --callgrind-out-file="$out/cg.out" "$@" >"$out/prog.out" 2>"$out/vg.log"
  callgrind_annotate "$out/cg.out" 2>/dev/null | awk '/PROGRAM TOTALS/{gsub(/,/,"",$1); print $1}'
  rm -rf "$out"
}

echo "REBUS_TRIANGULATE -- callgrind Ir, RT_OPT=-O0, tree $(git -C "$HERE/.." rev-parse --short HEAD 2>/dev/null || echo '?')"
echo "kernel               m3_Ir(compile+run)   m4_Ir(run only)"
SCRIP_HASH="$(git -C "$HERE/.." rev-parse --short HEAD 2>/dev/null || echo unknown)"
N=0
for k in $KERNELS; do
  f="$KDIR/$k.reb"
  if [ ! -f "$f" ]; then echo "⛔ REFUSED (rc=2): kernel missing: $f"; exit 2; fi
  m3ir="$(ir_of "$SCRIP_BIN" --run "$f" < /dev/null)"
  s="$(mktemp -d)"
  "$SCRIP_BIN" --compile "$f" -o "$s/p.s" < /dev/null 2>/dev/null || { echo "⛔ REFUSED (rc=2): $k failed to compile mode-4"; exit 2; }
  gcc "$s/p.s" -o "$s/p.bin" -L"$RT_DIR" -lscrip_rt -Wl,-rpath,"$RT_DIR" 2>/dev/null || { echo "⛔ REFUSED (rc=2): $k failed to link mode-4"; exit 2; }
  m4ir="$(ir_of "$s/p.bin" < /dev/null)"
  rm -rf "$s"
  printf '%-20s %18s %18s\n' "$k" "${m3ir:-REFUSED}" "${m4ir:-REFUSED}"
  N=$((N+1))
done
echo "REBUS_BOARD kernels=$N (no external rival exists; see this script's own header)"
[ "$N" -gt 0 ]
