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
r12=$(grep -icE 'r12[[:space:]]*\+' "$TMP/locals.s")
if [ "$nv" -ne 0 ]; then
    echo "LVA-1 LOCK 1 FAIL: locals-only program emitted $nv NV_GET/NV_SET call(s) — a local leaked onto the hash"
    fail=1
else
    echo "LVA-1 LOCK 1 OK: locals-only program has zero NV_GET/NV_SET calls"
fi
if [ "$r12" -eq 0 ]; then
    echo "LVA-1 LOCK 2 FAIL: locals-only program uses no [r12+off] — locals are not ζ-frame slotted"
    fail=1
else
    echo "LVA-1 LOCK 2 OK: locals use the ζ-frame ([r12+off], $r12 refs)"
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
rbx=$(grep -icE 'rbx[[:space:]]*\+' "$TMP/global.s")
greg=$(grep -c 'gva_register' "$TMP/global.s")
if [ "$rbx" -eq 0 ] || [ "$greg" -eq 0 ]; then
    echo "LVA-1 LOCK 3 FAIL: global program did not use the GVA array (rbx refs=$rbx, gva_register=$greg) — globals not on [rbx+k*16]"
    fail=1
else
    echo "LVA-1 LOCK 3 OK: global uses the GVA array ([rbx+k*16], $rbx refs; gva_register present)"
fi

if [ "$fail" -ne 0 ]; then
    echo "---"
    echo "FAIL: Icon local/global storage split violated."
    exit 1
fi
echo "---"
echo "PASS: Icon locals stay ζ-frame ([r12+off], no NV); globals use the [rbx+k*16] GVA array."
exit 0
