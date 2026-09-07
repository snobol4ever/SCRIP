#!/usr/bin/env bash
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="$ROOT/scrip"
refuse() { echo "GATE REFUSE(2) [icn_byname_generator_region_is_not_on_the_arena]: $*"; exit 2; }
[ -x "$SCRIP" ] || refuse "no scrip binary at $SCRIP -- cannot measure"
. "$HERE/lib_gate.sh"
gate_require_fresh "$ROOT" src "$ROOT/scrip" "$ROOT/out/libscrip_rt.so"
TD="$(mktemp -d)"; trap 'rm -rf "$TD"' EXIT
cat > "$TD/byname.icn" <<'EOF'
procedure gen()
    suspend 1 | 2 | 3;
end
procedure main()
    local p, x;
    p := gen;
    every x := p() do write(x);
end
EOF
cat > "$TD/direct.icn" <<'EOF'
procedure gen()
    suspend 1 | 2 | 3;
end
procedure main()
    local x;
    every x := gen() do write(x);
end
EOF
GRADED=0; FAIL=0
for w in byname direct; do
  for m in 3 4; do
    if [ "$m" = 3 ]; then
      out="$(SCRIP_ZETA_TELEM=1 timeout 20s "$SCRIP" "$TD/$w.icn" </dev/null 2>&1)"; rc=$?
    else
      "$SCRIP" --compile -o "$TD/$w.s" "$TD/$w.icn" </dev/null >/dev/null 2>&1 || refuse "$w: mode-4 compile failed -- cannot measure"
      gcc -no-pie "$TD/$w.s" -o "$TD/$w.bin" -L "$ROOT/out" -lscrip_rt -lm >/dev/null 2>&1 || refuse "$w: mode-4 link failed -- cannot measure"
      out="$(SCRIP_ZETA_TELEM=1 LD_LIBRARY_PATH="$ROOT/out" timeout 20s "$TD/$w.bin" </dev/null 2>&1)"; rc=$?
    fi
    GRADED=$((GRADED+1))
    body="$(printf '%s\n' "$out" | grep -v '^\[ZLS\]')"
    zls="$(printf '%s\n' "$out" | grep -c '^\[ZLS\]')"
    if [ "$rc" -ne 0 ]; then echo "  FAIL m$m $w: rc=$rc"; FAIL=$((FAIL+1)); continue; fi
    if [ "$(printf '%s' "$body" | tr -d ' \n')" != "123" ]; then echo "  FAIL m$m $w: output [$body] != 1 2 3"; FAIL=$((FAIL+1)); continue; fi
    if [ "$zls" -ne 0 ]; then
      echo "  FAIL m$m $w: $(printf '%s\n' "$out" | grep '^\[ZLS\]' | tail -1)"
      echo "        an Icon generator activation came from the ZLS arena -- RULES.md BB FRAME-PLACEMENT CRITERION, Lon 2026-09-06: No activations on the heap."
      FAIL=$((FAIL+1)); continue
    fi
    echo "  PASS m$m $w: zero ZLS arena allocations, output 1 2 3"
  done
done
[ "$GRADED" -eq 0 ] && refuse "graded zero witnesses"
echo "graded=$GRADED FAIL=$FAIL (witnesses: byname=the cured path, direct=the control that was always on the stack)"
if [ "$FAIL" -ne 0 ]; then echo "GATE FAIL(1) [icn_byname_generator_region_is_not_on_the_arena]: FAIL=$FAIL of $GRADED"; exit 1; fi
echo "GATE PASS(0) [icn_byname_generator_region_is_not_on_the_arena]: $GRADED/$GRADED, no activation storage from rt_zls_alloc"
exit 0
