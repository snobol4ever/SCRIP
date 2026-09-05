#!/usr/bin/env bash
# ⛔ RED TODAY, ON PURPOSE, AND DELIBERATELY *NOT* WIRED INTO `make test` YET.
# An integer-required keyword assigned a STRING holding C-style hex ("0x10") silently stores a WRONG
# NUMBER (16) where SPITBOL raises ERROR 208 -- keyword value assigned is not integer. Regression from
# SCRIP 0fa9c4cb4, which replaced a base-10 strtol() acceptance test in kwb_write_ent() with
# kwb_numeric_text() built on strtod(); strtod accepts the C99 0x/0X form, strtol(s,&end,10) did not.
# Row: snobol4-integer-keyword-accepts-hex-string-oracle-raises-error-208 (hq_P, rank 1).
#
# ⭐ WHY IT IS NOT IN `make test`: a red in the blocking set stops EVERY seat's landing, not just the
# owner's -- measured this same day, when two attributed reds blocked three separate seats' DONE-WHENs
# until an HQ ruled exclusion-by-name. Wire this in IN THE COMMIT THAT CURES IT, never before.
#
# ⭐ IT HAS A CONTROL ARM ON PURPOSE: 0fa9c4cb4's INTENT is correct and must survive the cure --
# "3.7" -> 3 is what SPITBOL does. The control arm fails if someone "fixes" this by reverting the
# commit, which would trade a wrong number for a different wrong number.
set -eu
cd "$(dirname "$0")/.." || exit 2
[ -x ./scrip ] || { echo "⛔ REFUSES rc=2: no built ./scrip -- run make first. A missing binary proves NOTHING about this defect."; exit 2; }
SBL=/home/resources/x64/bin/sbl
[ -x "$SBL" ] || { echo "⛔ REFUSES rc=2: correctness oracle absent at $SBL"; exit 2; }
W=$(mktemp -d); trap 'rm -rf "$W"' EXIT
mk(){ printf '\t&ANCHOR = "%s"\n\tOUTPUT = "anchor=" &ANCHOR\nEND\n' "$1" > "$W/k.sno"; }
run3(){ timeout 8 ./scrip --run "$W/k.sno" </dev/null 2>&1 || true; }
run4(){ ./scrip --compile -o "$W/k.s" "$W/k.sno" </dev/null >/dev/null 2>&1 || { echo COMPILEFAIL; return; }
  as -o "$W/k.o" "$W/k.s" 2>/dev/null && gcc -o "$W/k.bin" "$W/k.o" out/libscrip_rt.so -Wl,-rpath,"$PWD/out" 2>/dev/null || { echo LINKFAIL; return; }
  timeout 8 "$W/k.bin" </dev/null 2>&1 || true; }
mk '0x10'; og=$("$SBL" -bf "$W/k.sno" </dev/null 2>&1 || true)
printf '%s' "$og" | grep -q 'ERROR 208' || { echo "⛔ REFUSES rc=2 (premise gone): the oracle no longer raises ERROR 208 for &ANCHOR=\"0x10\". Re-measure before trusting this gate. Oracle said: $og"; exit 2; }
fail=0
for v in '0x10' '0X1f' '  0x10  '; do
  mk "$v"
  for m in 3 4; do g=$("run$m")
    if printf '%s' "$g" | grep -qE 'anchor=[0-9]'; then echo "FAIL: m$m silently accepts hex [$v] -> $(printf '%s' "$g" | head -1)  (oracle: ERROR 208)"; fail=1; fi
  done
done
for pair in '3.7 3' '+3 3' '010 10' '.5 0'; do set -- $pair; mk "$1"
  for m in 3 4; do g=$("run$m" | head -1)
    if [ "$g" != "anchor=$2" ]; then echo "FAIL(control): m$m gives [$g] for [$1], expected anchor=$2 -- 0fa9c4cb4's real-coercion intent was lost. Cure the hex prefix in kwb_numeric_text; do NOT revert the commit."; fail=1; fi
  done
done
[ "$fail" = 0 ] || { echo "⛔ GATE FAIL: integer-keyword hex acceptance (row snobol4-integer-keyword-accepts-hex-string-oracle-raises-error-208)"; exit 1; }
echo "✅ hex-string keyword assignment refused in BOTH modes, and real/decimal coercion preserved"
