#!/usr/bin/env bash
# bench_icon_rate_3way.sh — FIXED-TIME RATE comparison: SCRIP m3/m4 vs Arizona iconx vs JCON.
#
# ⭐ WHY FIXED-TIME AND NOT FIXED-ITERATION (Lon, 2026-08-21, in-chat: "flip the tests to loop on a
# fixed time and report iterations, versus looping on a fixed iteration and reporting time").  The
# fixed-iteration form is what this project used before and it is NOT trustworthy on this corpus:
#   (1) its runs are 12-80ms, so PROCESS STARTUP is a large fraction and has to be hand-subtracted;
#   (2) it never lets a JIT warm — it measured JCON cold and reported it ~2.9x behind SCRIP, when a
#       warm JCON is actually AHEAD of SCRIP on 7 of 9 scored benchmarks;
#   (3) at those durations, machine contention swings a single sample by 50%+ (measured: the iconx
#       oracle, an unchanged binary, read 26,560 then 17,312 ops/ms on two consecutive runs).
# Fixed-time fixes all three: each program reads its own &time (so startup is excluded BY
# CONSTRUCTION on every engine), warms untimed, then counts whole UNITs until the budget expires.
# Rate = units * UNIT / ms.  Verified: &time has the same meaning on all three engines (SCRIP 21ms
# internal / 25ms wall; iconx 170/175; JCON 90/162 — the JCON gap is exactly its JVM startup).
#
# ⛔ EVERY RUN ALSO CHECKS CORRECTNESS.  Each program prints a PER-UNIT checksum, which is identical
# on every engine no matter how many units each completed, so rate and cross-engine agreement come
# out of the same run.  ⛔ BUT A MATCHING CHECKSUM DOES NOT PROVE THE WORK HAPPENED: SCRIP CONSTANT-
# FOLDS `"abcdefgh" || "ijklmnop"` in rate_concat_dispatch (measured: ZERO concat calls in its
# emitted .s, against 2 in every other concat benchmark), and the folded constant still has the
# right length.  Audit the emitted asm for the operation under test before trusting any row.
#
# ⛔ RT_OPT: build SCRIP with -O2 before running (O0-DEV-O2-BENCH).  Measuring the -O0 development
# build against optimized oracles understates SCRIP by ~2x on every runtime-heavy benchmark; that
# error was made and caught during this harness's own bring-up.
#
# Usage: [REPS=3] [WARM=500] [BUD=1500] bash scripts/bench_icon_rate_3way.sh
# Oracles: icont_bin()/iconx_bin() (shared /home/resources/icon-master/bin) and refs/jcon-master/bin/{jcont,jcon}.
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
. "$HERE/lib_oracle_flags.sh" 2>/dev/null || { echo "REFUSING: cannot load lib_oracle_flags.sh -- the ONE oracle-flag authority (s200/s255), Icon-aware since row icon-oracle-accessors-shared." >&2; exit 3; }
R=/home/claude1/SCRIP; SCRIP=$R/scrip; RT=$R/out
J=$R/refs/jcon-master/bin
ICONT_BIN="$(icont_bin)" || exit 2
iconx_bin >/dev/null || exit 2
I="$(dirname "$ICONT_BIN")"
export PATH="$J:$I:$PATH"
S=${S4E_RATE:-/home/claude1/corpus/benchmarks/icon/rate}
WARM=${WARM:-500}; BUD=${BUD:-1500}; REPS=${REPS:-3}
LOAD0=$(cut -d" " -f1-3 /proc/loadavg)
W=$(mktemp -d /tmp/rate3_XXXXXX); cd "$W"; cp $S/*.icn .
declare -A UNIT=( [int_loop]=20000 [int_mod]=20000 [concat_dispatch]=20000 [concat_int_dispatch]=20000 \
  [concat_intvar]=20000 [concat_strvar]=20000 [concat_table]=10000 [sub_list]=20000 \
  [sub_table_miss]=20000 [table_semantics]=15000 )
printf "%-22s %12s %12s %12s %12s   %8s %8s   %s\n" BENCHMARK "iconx" "jcon" "SCRIP-m3" "SCRIP-m4" "vs-iconx" "vs-jcon" "CHECKSUMS"
printf -- "---------------------- ------------ ------------ ------------ ------------   -------- --------   ---------\n"
for f in rate_*.icn; do
  n=${f%.icn}; k=${n#rate_}; u=${UNIT[$k]}
  "$I/icont" -s -o "$n.icx" "$f" >/dev/null 2>&1
  jcont -s -o "$n.jx" "$f" >/dev/null 2>&1
  "$SCRIP" --compile "$f" > "$n.s" 2>/dev/null </dev/null
  gcc -no-pie "$n.s" -L"$RT" -lscrip_rt -Wl,-rpath,"$RT" -lm -o "$n.bin" 2>/dev/null
  bestrate() { local u=$1; shift; local b=0 r x; for r in $(seq $REPS); do x=$("$@" $WARM $BUD 2>/dev/null </dev/null); x=$(awk -v u=$u '{ if ($2>0) printf "%.0f", ($1*u)/$2; else print 0 }' <<< "$x"); [ "${x:-0}" -gt "$b" ] && b=$x; done; echo $b; }
  onecks()  { "$@" $WARM $BUD 2>/dev/null </dev/null | awk '{print $3}'; }
  ai=$(bestrate $u "$I/iconx" "$n.icx");     ci=$(onecks "$I/iconx" "$n.icx")
  aj=$(bestrate $u ./"$n.jx");               cj=$(onecks ./"$n.jx")
  a3=$(bestrate $u "$SCRIP" --run "$f" --);  c3=$(onecks "$SCRIP" --run "$f" --)
  a4=$(bestrate $u ./"$n.bin");              c4=$(onecks ./"$n.bin")
  if [ "$ci" = "$cj" ] && [ "$ci" = "$c3" ] && [ "$ci" = "$c4" ] && [ -n "$ci" ]; then cs="OK($ci)"; else cs="MISMATCH i=$ci j=$cj m3=$c3 m4=$c4"; fi
  vi=$(awk -v a=$a4 -v b=$ai 'BEGIN{ if(b>0) printf "%.2fx", a/b; else printf "-" }')
  vj=$(awk -v a=$a4 -v b=$aj 'BEGIN{ if(b>0) printf "%.2fx", a/b; else printf "-" }')
  printf "%-22s %12s %12s %12s %12s   %8s %8s   %s\n" "$k" "$ai" "$aj" "$a3" "$a4" "$vi" "$vj" "$cs"
done
echo "(ops/ms, higher is better; BEST of ${REPS}; warm=${WARM}ms budget=${BUD}ms; vs-* compare SCRIP m4)"
echo "load before/after: $LOAD0 / $(cut -d' ' -f1-3 /proc/loadavg)"
echo "workdir: $W"
