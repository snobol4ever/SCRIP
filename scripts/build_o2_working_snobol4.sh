#!/usr/bin/env bash
# build_o2_working_snobol4.sh — a WORKING optimised SNOBOL4 runtime.
#
# ⛔ THIS IS A WORKAROUND, NOT A FIX, AND MUST NEVER BE CALLED ONE. At -O1 and above the SNOBOL4 runtime
# miscompiles or exercises UB: beauty self-host emits 278 bytes instead of 40,971 and the 17-line witness
# 161_pat_defer_fn_nested_match returns a wrong answer (-O2) or SEGVs (-O1). Two automated file-level
# bisections over all 261 runtime objects localised the whole defect class to exactly TWO files:
#     src/runtime/rt/rt.c          (necessary for the 161 witness)
#     src/runtime/pattern_match.c  (needed as well before beauty recovers)
# The interaction is NON-ADDITIVE -- rt.c alone leaves beauty at 614 bytes, pattern_match.c alone fixes
# neither -- so this is one defect class reachable through two files, not two independent bugs.
#
# Building those two at -O0 and the other 259 at -O2 gives, measured at SCRIP 3f951354:
#     beauty m3 and m4 : 40,971 bytes, md5 6f1671c0757729992ae01a6bdf16f081  FIXED POINT
#     161 witness      : matches its oracle
#     corpus           : m3 357/359, m4 355/359 + 2 SKIP  == the -O0 baseline exactly
#
# ⛔ De-optimising two files HIDES the undefined behaviour rather than removing it, and the same UB can
# resurface through anything that inlines the offending code. Use this to get an optimised arm TODAY;
# the real fix belongs in rt.c and pattern_match.c (row 161-o2-red), and -fsanitize=undefined on exactly
# those two files is the obvious next probe.
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"; cd "$ROOT"
O2="${O2FLAGS:--O2 -g -fno-strict-aliasing -fwrapv -fno-omit-frame-pointer}"
O0="${O0FLAGS:--O0 -g -fno-strict-aliasing -fwrapv -fno-omit-frame-pointer}"
echo "1/3  full runtime at: $O2   (first run pays the full cost; later runs reuse the keyed cache)"
make libscrip_rt RT_OPT="$O2" >/dev/null || { echo "⛔ -O2 build failed"; exit 1; }
PURE="$(make -s buildinfo RT_OPT="$O2" | awk '/RT_TAG/{print $3}')"
[ -d "out/rt_pic-$PURE" ] || { echo "⛔ expected object dir out/rt_pic-$PURE"; exit 1; }
# ⛔ THE HYBRID GETS ITS OWN TAG AND NEVER WRITES INTO THE PURE -O2 CACHE. Overwriting two objects inside
# out/rt_pic-<pure O2 tag> would leave a directory whose name says "everything at -O2" while two files are
# not -- which is precisely the mislabelled-cache class the flag-keyed scheme exists to make impossible.
# Measured the hard way: the first version of this script did exactly that.
TAG="$PURE-hybrid"; D="out/rt_pic-$TAG"
rm -rf "$D"; cp -r "out/rt_pic-$PURE" "$D"
echo "2/3  forked the pure -O2 cache to $D, re-compiling the two known-bad files at -O0 there"
for f in src/runtime/rt/rt.c src/runtime/pattern_match.c; do
  b="$(basename "${f%.*}").o"
  inc="-I$ROOT/src -I$ROOT/src/include -I$ROOT/src/contracts -I$ROOT/src/lower -I$ROOT/src/machine -I$ROOT/src/emitter -I$ROOT/src/runtime/core -I$ROOT/src/runtime/builtins -I$ROOT/src/runtime -I$ROOT/src/runtime/rt -I$ROOT/src/frontend/snobol4 -I$ROOT/src/frontend/raku -I$ROOT/src/optimizer"
  gcc $O0 -g -w -MMD -MP -fPIC $inc -DDYN_ENGINE_LINKED -DIR_DEFINE_NAMES -c "$f" -o "$D/$b" || { echo "⛔ failed on $f"; exit 1; }
  echo "     $f -> $D/$b  (-O0)"
done
echo "3/3  relinking"
gcc -shared "$D"/*.o -lm -lstdc++ -lpthread -o "out/libscrip_rt-$TAG.so" || exit 1
ln -sfn "libscrip_rt-$TAG.so" out/libscrip_rt.so
echo "     out/libscrip_rt.so -> libscrip_rt-$TAG.so"
echo
echo "VERIFY (this script does not claim success -- the gate does):"
echo "   bash scripts/test_gate_m1_self_host_fixed_point.sh both"
echo "   bash scripts/test_corpus_snobol4.sh"
