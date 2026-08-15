#!/usr/bin/env bash
# scripts/test_gate_icn_local_no_nv.sh — LVA-1 lock: Icon LOCALS stay ζ-frame slotted ([r12+off]),
# never the NV_* name-table hash. Only true GLOBALS (from 'global' decls) may use NV (mode-3) or the
# [rbx+k*16] GVA array (mode-4). This gate pins the already-correct split so a future change cannot
# silently regress Icon locals onto the hash.
#
# Three checks (mode-4 .s):
#   LOCK 1 — a locals-only program (NO 'global' decl) emits ZERO `call NV_GET`/`call NV_SET`.
#   LOCK 2 — that same program DOES use the ζ-frame `[r12+...]` (locals are really slotted there).
#   LOCK 3 — a program WITH a 'global' decl DOES use the GVA array `[rbx+...]` and `gva_register`
#            (the split is real: globals go to GVA, not that "nothing" uses storage).
#
# Authors: LCherryholmes · Jeffrey Cooper M.D. · Claude

set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP:-$ROOT/scrip}"
TMP="$(mktemp -d)"
trap 'rm -rf "$TMP"' EXIT
fail=0

[ -x "$SCRIP" ] || { echo "LVA-1: scrip not built at $SCRIP"; exit 2; }

# --- LOCK 1 + 2: locals-only program ---
cat > "$TMP/locals.icn" <<'EOF'
procedure main()
  local x, y;
  x := 10;
  y := 20;
  write(x + y);
end
EOF
"$SCRIP" --compile "$TMP/locals.icn" < /dev/null > "$TMP/locals.s" 2> "$TMP/locals.err"
if grep -qiE '\[SMX\]' "$TMP/locals.s" "$TMP/locals.err"; then
    echo "LVA-1 SKIP: locals-only program declined (cannot check) — investigate, not a pass"
    exit 2
fi
nv=$(grep -icE 'call[[:space:]]+NV_GET|call[[:space:]]+NV_SET' "$TMP/locals.s")
cells=$(grep -icE 'rsp[[:space:]]*[+]' "$TMP/locals.s"); leak=$(grep -icE 'r9[[:space:]]*[+]' "$TMP/locals.s")
if [ "$nv" -ne 0 ]; then
    echo "LVA-1 LOCK 1 FAIL: locals-only program emitted $nv NV_GET/NV_SET call(s) — a local leaked onto the hash"
    fail=1
else
    echo "LVA-1 LOCK 1 OK: locals-only program has zero NV_GET/NV_SET calls"
fi
if [ "$cells" -eq 0 ] || [ "$leak" -ne 0 ]; then
    echo "LVA-1 LOCK 2 FAIL: locals not on ζ-SPINE cells or leaked into the r9 GVA arena (rsp refs=$cells, r9 refs=$leak)"
    fail=1
else
    echo "LVA-1 LOCK 2 OK: locals ride ζ-SPINE [rsp+off] cells ($cells refs), zero r9 arena refs"
fi

# --- LOCK 3: a program WITH a global uses GVA ---
cat > "$TMP/global.icn" <<'EOF'
global g;
procedure main()
  g := 5;
  write(g);
end
EOF
"$SCRIP" --compile "$TMP/global.icn" < /dev/null > "$TMP/global.s" 2> "$TMP/global.err"
if grep -qiE '\[SMX\]' "$TMP/global.s" "$TMP/global.err"; then
    echo "LVA-1 SKIP: global program declined (cannot check LOCK 3)"
    exit 2
fi
gva=$(grep -icE 'r9[[:space:]]*[+]' "$TMP/global.s")
greg=$(grep -cE 'gva_register|[.]Lgvan' "$TMP/global.s")
if [ "$gva" -eq 0 ] || [ "$greg" -eq 0 ]; then
    echo "LVA-1 LOCK 3 FAIL: global program did not use the GVA arena (r9 refs=$gva, gva markers=$greg) — globals not on [r9+k*16]"
    fail=1
else
    echo "LVA-1 LOCK 3 OK: global uses the GVA arena ([r9+k*16], $gva refs; gva markers present)"
fi

if [ "$fail" -ne 0 ]; then
    echo "---"
    echo "FAIL: Icon local/global storage split violated."
    exit 1
fi
echo "---"
echo "PASS: Icon locals ride ζ-SPINE [rsp+off] cells (no NV, no r9 leak); globals use the [r9+k*16] GVA arena."
exit 0
