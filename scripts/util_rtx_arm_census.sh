#!/usr/bin/env bash
# util_rtx_arm_census.sh — DID THE ASM ACTUALLY RUN, OR DID IT BAIL TO C?
#
# Minted s216 (2026-07-29) after RTX-8 slice 3 was written, built, verified
# output-identical, and then PROVED VACUOUS: rt_cap_push took 57,578 dynamic
# calls on json.sno+twitter.json, passed ARCH §7 step 0(d) cleanly (count
# scaled EXACTLY 2.00x on a structurally-doubled input, hottest unported symbol
# in its family), and yet its ported hot path NEVER EXECUTED ONCE — every call
# bailed to the C fallback on a cold arm.  A `ud2` planted on the commit path
# did not fire with the gate ON.
#
# THE GAP THIS CLOSES: step 0(d) counts entries to a SYMBOL.  A gated RTX port
# is a symbol with two or more arms, and the asm covers only the arm(s) the
# author chose.  So 0(d) can pass in full while the ported arm is dead — s215's
# "a call count cannot name an arm" one level down, where the count is dynamic
# and correct and STILL does not license the port.
#
# THE MEASUREMENT: every gated RTX symbol tail-jumps to a `c_*` fallback when it
# declines an arm (the RTX_GATE / bail-before-mutate shape).  So counting BOTH
# `sym` and `c_sym` splits the traffic mechanically:
#     entries  = calls that reached the asm symbol
#     bailed   = calls that tail-jumped to the C fallback
#     commits  = entries - bailed  = calls the ASM ACTUALLY HANDLED END TO END
# commits == 0 means the port is unfalsifiable on this workload: do not write
# the asm, and do not grade it here.  Run this BEFORE the asm, on the workload
# the rung will be graded on, and again after.
#
# The symbol list is DERIVED FROM THE TREE on every run (RTX_FUNC names in
# src/runtime/rtx/*.S, intersected with the .so's dynamic table), never
# hand-maintained — a checked-in list is the doc-rot class this ladder has paid
# for six times (ARCH §7 step 0, the phantom family).
#
# Usage: bash scripts/util_rtx_arm_census.sh <prog.sno> [< input]
#        SCRIP_RTX_<FAM> env vars pass through, so ON/OFF arms are comparable.
set -u
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
SO="$ROOT/out/libscrip_rt.so"
PROG="${1:?usage: util_rtx_arm_census.sh <prog.sno>}"
[ -f "$SO" ] || { echo "FATAL: $SO missing — run make libscrip_rt first"; exit 1; }
[ -x "$ROOT/scrip" ] || { echo "FATAL: $ROOT/scrip missing"; exit 1; }
TMP="$(mktemp -d)"; trap 'rm -rf "$TMP"' EXIT
# Derive the symbol set from the tree, then keep only what the .so really exports.
nm -D --defined-only "$SO" | awk '$2=="T"{print $3}' | sort -u > "$TMP/dyn.txt"
grep -rhoP 'RTX_FUNC\(\s*\K[A-Za-z0-9_]+' "$ROOT"/src/runtime/rtx/*.S | sort -u > "$TMP/asm.txt"
: > "$TMP/syms.txt"
while read -r s; do
  grep -qx "$s" "$TMP/dyn.txt" || continue
  echo "$s" >> "$TMP/syms.txt"
  grep -qx "c_$s" "$TMP/dyn.txt" && echo "c_$s" >> "$TMP/syms.txt"
done < "$TMP/asm.txt"
[ -s "$TMP/syms.txt" ] || { echo "FATAL: derived zero symbols — did RTX_FUNC or the .so layout change?"; exit 1; }
# Emit the interposer.  Counters are HIDDEN so `inc [rip+cnt]` is legal in a .so
# (an exported data symbol needs @GOTPCREL — ARCH §7 step 0(c), which bit this
# very tool during its own construction).  The thunk is `inc` + `jmp *ptr`: it
# clobbers only EFLAGS, never an argument register, so it forwards ANY signature
# without knowing it.  Arming is conditional on libscrip_rt.so being present in
# THIS process, because scrip's run spawns a second process that has no runtime
# loaded and whose destructor would otherwise overwrite the census with zeros.
{
  echo '#define _GNU_SOURCE'
  echo '#include <dlfcn.h>'
  echo '#include <stdio.h>'
  echo '#include <stdlib.h>'
  printf '#define SYMS'
  while read -r s; do printf ' X(%s)' "$s"; done < "$TMP/syms.txt"
  echo
  echo '#define HID __attribute__((visibility("hidden")))'
  echo '#define X(n) HID unsigned long cnt_##n = 0; HID void *real_##n = 0;'
  echo 'SYMS'
  echo '#undef X'
  echo '#define X(n) __asm__(".text\n.intel_syntax noprefix\n.globl " #n "\n.type " #n ",@function\n" #n ":\n inc qword ptr [rip+cnt_" #n "]\n jmp qword ptr [rip+real_" #n "]\n.att_syntax\n.previous\n");'
  echo 'SYMS'
  echo '#undef X'
  echo 'static int armed = 0;'
  echo '__attribute__((constructor)) static void ac_init(void) { if (!dlopen("libscrip_rt.so", RTLD_NOW | RTLD_NOLOAD)) return; armed = 1;'
  echo '#define X(n) real_##n = dlsym(RTLD_NEXT, #n); if (!real_##n) fprintf(stderr, "ARMCENSUS UNRESOLVED: %s\n", #n);'
  echo 'SYMS'
  echo '#undef X'
  echo '}'
  echo '__attribute__((destructor)) static void ac_fini(void) { if (!armed) return; unsigned long t = 0;'
  echo '#define X(n) t += cnt_##n;'
  echo 'SYMS'
  echo '#undef X'
  echo '  if (!t) return; const char *p = getenv("ARMCENSUS_OUT"); FILE *f = fopen(p ? p : "/tmp/armcensus.txt", "w"); if (!f) return;'
  echo '#define X(n) fprintf(f, "%s %lu\n", #n, cnt_##n);'
  echo 'SYMS'
  echo '#undef X'
  echo '  fclose(f); }'
} > "$TMP/ac.c"
gcc -O0 -shared -fPIC -o "$TMP/ac.so" "$TMP/ac.c" -ldl || { echo "FATAL: interposer build failed"; exit 1; }
ARMCENSUS_OUT="$TMP/out.txt" LD_LIBRARY_PATH="$ROOT/out" LD_PRELOAD="$TMP/ac.so" "$ROOT/scrip" --run "$PROG" > "$TMP/prog.out" 2>"$TMP/prog.err"
RC=$?
[ -s "$TMP/out.txt" ] || { echo "NO DATA — the run counted nothing (rc=$RC).  Check that the program runs at all."; exit 1; }
echo "=== RTX ARM CENSUS — $PROG (rc=$RC) ==="
printf "%-34s %10s %10s %10s  %s\n" SYMBOL ENTRIES BAILED_C COMMITS VERDICT
while read -r s; do
  case "$s" in c_*) continue ;; esac
  e=$(awk -v k="$s" '$1==k{print $2}' "$TMP/out.txt"); e=${e:-0}
  b=$(awk -v k="c_$s" '$1==k{print $2}' "$TMP/out.txt"); b=${b:-0}
  [ "$e" = 0 ] && continue
  c=$((e - b))
  if [ "$c" -le 0 ]; then v="VACUOUS HERE — asm never commits"
  elif [ "$b" -gt "$c" ]; then v="MOSTLY BAILS — cold arm dominates"
  else v="asm handles $c"; fi
  printf "%-34s %10s %10s %10s  %s\n" "$s" "$e" "$b" "$c" "$v"
done < "$TMP/syms.txt"
echo "(symbols with zero entries are omitted: this workload cannot grade them at all)"
