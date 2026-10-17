#!/usr/bin/env bash
# test_gate_sno_atn_after_a_failed_code_the_deferred_pattern_call_freturns_as_spitbol_does.sh
#
# THE WITNESS (ceo 2026-10-17, CEO-1811; found through the coo's AIS ATN red of 2026-10-15, bisected by hq_runtime to 1a36cb772): aisnobol's ATN compiles each FUNCTION of its network source with
# CODE() from inside a pattern match -- S_ is the deferred target of NULL $ *S_('F') in a pattern built by EVAL, S_ reaches S_F by the computed goto :($('S_' NA)), and when CODE fails S_F
# calls ERROR and leaves by FRETURN. This gate feeds ATN its own ATN.IN with one FUNCTION body made a syntax error that sbl -bf also rejects (LEXICAL_FEATURES[X) for LEXICAL_FEATURES[X]):
# sbl -bf prints Compilation error, Compilation failed and FATAL ERROR and exits 0; SCRIP stops after Compilation error with SIGSEGV, a jump to an address inside the live GC heap (ZGC-STALE).
# The answer is cut from sbl -bf at run time; the two wall-clock lines ATN prints are masked as ALL.mask masks them. rc=0 clean · rc=1 a divergence · rc=2 REFUSAL.
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
B="$HERE/.."
C="${CORPUS:-$B/../corpus}"
P="$C/packages/snobol4/aisnobol"
SBL="${SBL:-/home/resources/x64/bin/sbl}"
refuse() { echo "⛔ REFUSES rc=2: $*"; exit 2; }
[ -x "$B/scrip" ] || refuse "$B/scrip is not built -- cannot measure"
[ -f "$B/out/libscrip_rt.so" ] || refuse "out/libscrip_rt.so is not built -- mode 4 cannot link"
[ -x "$SBL" ] || refuse "the SNOBOL4 oracle $SBL is absent -- the answer cannot be cut"
[ -f "$P/ATN.sno" ] && [ -f "$P/ATN.IN" ] || refuse "$P/ATN.sno or ATN.IN is absent"
"$HERE/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
D="$(mktemp -d)" || refuse "no scratch dir"
trap 'rm -rf "$D"' EXIT
cp "$P/ATN.sno" "$D/ATN.sno"
sed 's/LEXICAL_FEATURES[<[]X[]>]/LEXICAL_FEATURES[X)/' "$P/ATN.IN" > "$D/in"
grep -q 'LEXICAL_FEATURES\[X)' "$D/in" || refuse "ATN.IN no longer carries GETF's LEXICAL_FEATURES subscript -- the witness was not formed"
mask() { sed -E 's/^[0-9]+ milliseconds (compile time|used)$/N milliseconds \1/' "$1"; }
( cd "$D" && timeout -k 5 60 "$SBL" -bf ATN.sno < in > want.raw 2>&1; echo $? > want.rc )
mask "$D/want.raw" > "$D/want"
grep -q 'FATAL ERROR' "$D/want" || refuse "sbl -bf did not reach the lines after the failed CODE -- the oracle's answer does not measure the continuation"
red=0
arm() { if [ "$2" = ok ]; then echo "  ok   $1"; else echo "  FAIL $1: $2"; red=$((red + 1)); fi; }
( cd "$D" && timeout -k 5 60 "$B/scrip" -d131072k -s4096k --stlimit ATN.sno < in > m3.raw 2> m3.err; echo $? > m3.rc )
( cd "$D" && timeout -k 5 60 "$B/scrip" --compile --stlimit -o m4.s ATN.sno < /dev/null 2> m4.cerr && gcc m4.s -L"$B/out" -lscrip_rt -lm -lpthread -Wl,-rpath,"$B/out" -o m4.bin 2>> m4.cerr \
  && timeout -k 5 60 ./m4.bin -d131072k -s4096k < in > m4.raw 2> m4.err; echo $? > m4.rc )
for m in m3 m4; do
    mask "$D/$m.raw" > "$D/$m"
    if cmp -s "$D/want" "$D/$m" && [ "$(cat "$D/$m.rc")" = "$(cat "$D/want.rc")" ]; then arm "$m equals sbl -bf ($(wc -l < "$D/want") lines, rc $(cat "$D/want.rc"))" ok
    else arm "$m" "rc $(cat "$D/$m.rc") (sbl $(cat "$D/want.rc")), $(wc -l < "$D/$m") of $(wc -l < "$D/want") lines, first divergence: $(diff "$D/want" "$D/$m" | head -3 | tr '\n' '|' | cut -c1-160) $(head -1 "$D/$m.err" | cut -c1-120)"; fi
done
if [ "$red" -eq 0 ]; then echo "GATE PASS(0) [sno_atn_failed_code_deferred_freturn]: 2 of 2 modes equal sbl -bf"; exit 0; fi
echo "GATE FAIL(1) [sno_atn_failed_code_deferred_freturn]: $red of 2 modes diverge from sbl -bf"; exit 1
