#!/usr/bin/env bash
# util_rtx_count_syms.sh — DYNAMIC CALL COUNT FOR *ARBITRARY* SYMBOLS, PRE-PORT.
#
# Minted s217 (2026-07-29).  Closes the scope gap named in GOAL-SNOBOL4-RTX.md:
# util_rtx_arm_census.sh derives its symbol list from RTX_FUNC(...) names in
# src/runtime/rtx/*.S, so it can only see symbols that are ALREADY ported — it
# is the POST-port confirmation.  ARCH §7 step 0(d) has to run BEFORE any asm is
# written, on a symbol that is still pure C and therefore invisible to that
# tool.  This script takes the symbol names on the command line instead of
# deriving them, so it works at any stage.
#
# It answers 0(d) ONLY (entries to a symbol, and whether the count SCALES).  It
# CANNOT answer 0(f) pre-port: with no c_* fallback in the tree yet there is no
# bail edge to count, so a symbol with cold arms still reads as pure entries.
# 0(f) pre-port remains a source-reading obligation — read the C body and prove
# the arms, then confirm with util_rtx_arm_census.sh after the port lands.
#
# Machinery is lifted verbatim from util_rtx_arm_census.sh (s216) so its three
# hard-won defect fixes are inherited rather than re-earned:
#   * counters are visibility("hidden") => `inc [rip+cnt]` is legal inside a .so
#     (an exported data symbol would need @GOTPCREL — ARCH §7 step 0(c))
#   * the thunk is `inc` + `jmp *ptr`: clobbers EFLAGS only, never an argument
#     register, so it forwards ANY signature without knowing it
#   * arming is conditional on libscrip_rt.so being loaded in THIS process,
#     because scrip's run spawns a second process with no runtime whose
#     destructor would otherwise overwrite the census with zeros
#
# Usage: bash scripts/util_rtx_count_syms.sh <prog.sno> <sym> [sym...]
#        SCRIP_RTX_<FAM> env vars pass through.
set -u
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
SO="$ROOT/out/libscrip_rt.so"
PROG="${1:?usage: util_rtx_count_syms.sh <prog.sno> <sym> [sym...]}"; shift
[ $# -ge 1 ] || { echo "FATAL: name at least one symbol"; exit 1; }
[ -f "$SO" ] || { echo "FATAL: $SO missing — run make libscrip_rt first"; exit 1; }
[ -x "$ROOT/scrip" ] || { echo "FATAL: $ROOT/scrip missing"; exit 1; }
TMP="$(mktemp -d)"; trap 'rm -rf "$TMP"' EXIT
# Keep only symbols the .so actually exports as text; a typo must FAIL LOUDLY
# rather than silently report zero (the phantom-family failure mode).
nm -D --defined-only "$SO" | awk '$2=="T"{print $3}' | sort -u > "$TMP/dyn.txt"
: > "$TMP/syms.txt"
for s in "$@"; do
  if grep -qx "$s" "$TMP/dyn.txt"; then echo "$s" >> "$TMP/syms.txt"
  else echo "FATAL: '$s' is not an exported text symbol of libscrip_rt.so — check the spelling against the tree (ARCH §7 step 0b)"; exit 1; fi
done
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
  echo '__attribute__((constructor)) static void cs_init(void) { if (!dlopen("libscrip_rt.so", RTLD_NOW | RTLD_NOLOAD)) return; armed = 1;'
  echo '#define X(n) real_##n = dlsym(RTLD_NEXT, #n); if (!real_##n) fprintf(stderr, "COUNTSYMS UNRESOLVED: %s\n", #n);'
  echo 'SYMS'
  echo '#undef X'
  echo '}'
  echo '__attribute__((destructor)) static void cs_fini(void) { if (!armed) return; unsigned long t = 0;'
  echo '#define X(n) t += cnt_##n;'
  echo 'SYMS'
  echo '#undef X'
  echo '  if (!t) return; const char *p = getenv("COUNTSYMS_OUT"); FILE *f = fopen(p ? p : "/tmp/countsyms.txt", "w"); if (!f) return;'
  echo '#define X(n) fprintf(f, "%s %lu\n", #n, cnt_##n);'
  echo 'SYMS'
  echo '#undef X'
  echo '  fclose(f); }'
} > "$TMP/cs.c"
gcc -O0 -shared -fPIC -o "$TMP/cs.so" "$TMP/cs.c" -ldl || { echo "FATAL: interposer build failed"; exit 1; }
COUNTSYMS_OUT="$TMP/out.txt" LD_LIBRARY_PATH="$ROOT/out" LD_PRELOAD="$TMP/cs.so" "$ROOT/scrip" --run "$PROG" > "$TMP/prog.out" 2>"$TMP/prog.err"
RC=$?
[ -s "$TMP/out.txt" ] || { echo "NO DATA — every named symbol counted zero (rc=$RC).  Either the program never reaches them, or it does not run at all."; sed -n '1,6p' "$TMP/prog.err"; exit 1; }
echo "=== DYNAMIC CALL COUNT — $PROG (rc=$RC) ==="
printf "%-40s %14s\n" SYMBOL ENTRIES
while read -r s; do
  n=$(awk -v k="$s" '$1==k{print $2}' "$TMP/out.txt"); n=${n:-0}
  printf "%-40s %14s\n" "$s" "$n"
done < "$TMP/syms.txt"
echo "--- program stdout (first 6 lines, so a broken run cannot masquerade as a measurement) ---"
sed -n '1,6p' "$TMP/prog.out"
