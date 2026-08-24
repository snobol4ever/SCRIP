#!/usr/bin/env bash
# test_icn_genframe_alloc.sh -- UNIT TEST for the N-2 generator ACTIVATION-RECORD allocator
# (rt_icn_gen_frame_alloc / rt_icn_gen_frame_retire, src/runtime/rt/rt.c).
#
# ⭐ WHY THIS IS A UNIT TEST AND NOT A GATE: the allocator is reachable from no emitted code yet
# (the alpha-prologue wiring is the next slice), so no corpus program exercises it and no board
# would move if it broke. That is exactly the window in which a storage bug gets baked in and then
# blamed on the codegen that lands on top of it -- so the storage half is proven HERE, in isolation,
# before anything depends on it.
#
# ⛔ WHAT IT IS ACTUALLY GUARDING, and why each check earns its place:
#   - the island (rt_ws_alloc) is a BUMP ALLOCATOR WITH NO FREE (g_wsi_ws += total), so without the
#     free list a generator in a loop leaks one record per activation until the island ABORTS. The
#     reuse check is the one that keeps that from coming back.
#   - two LIVE generators must never share a record; the free list must only hand back RETIRED ones.
#   - a reused record must be re-zeroed, or one activation reads the previous one's locals.
set -u
HERE="$(cd "$(dirname "$0")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
SO="$ROOT/out/libscrip_rt.so"
[ -f "$SO" ] || { echo "⛔ REFUSES: $SO missing -- run make first (a missing .so is not a pass)"; exit 2; }
TMP="$(mktemp -d)"; trap 'rm -rf "$TMP"' EXIT
gcc -o "$TMP/t" "$HERE/probes/icn_genframe_alloc.c" "$SO" -Wl,-rpath,"$ROOT/out" || { echo "⛔ REFUSES: probe did not compile/link"; exit 2; }
"$TMP/t"; rc=$?
[ $rc -eq 0 ] && echo "GATE OK: icn genframe allocator" || echo "⛔ FAIL: icn genframe allocator (rc=$rc)"
exit $rc
