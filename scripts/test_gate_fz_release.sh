#!/usr/bin/env bash
# scripts/test_gate_fz_release.sh
# FZ (FENCE0 SELECTIVE RELEASE) GATE -- two independent locks over corpus/probe/fz.
#   LOCK 1 (verdict): every witness, DISARMED (the shipped default), m3 and m4, byte-equal to its
#           oracle .ref.  Red here means the default arm regressed.
#   LOCK 2 (verdict): NO OVER-RELEASE.  Re-derives each fence's releasable carve FROM THE EMITTED ASM
#           -- follows the alpha-chain from each match_begin box, sums `sub rsp,N` over the contiguous
#           whitelisted leaf boxes at the frontier, and asserts the fence's own `add rsp,R` never
#           exceeds it.  This is the FZ-1 defect's tripwire (it billed 32 against a 16 carve) and it
#           pins NO golden: it checks the compiler's claim against the compiler's own output.
#   REPORT (never a verdict): whether the FZ-3 arm-blocker still reproduces.  Arming by default is
#           refused while it does; if it CLEARS, this line is the signal to re-evaluate, not an error.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${SCRIP:-$HERE/../scrip}"
RT_DIR="${RT_DIR:-$HERE/../out}"
FZ="${FZ:-$S4E/corpus/probe/fz}"
[ -x "$SCRIP" ] || { echo "SKIP scrip not built"; exit 0; }
[ -d "$FZ" ]    || { echo "SKIP no $FZ"; exit 0; }
WORK=$(mktemp -d); trap 'rm -rf "$WORK"' EXIT
rc=0
echo "=== FZ GATE -- LOCK 1: witnesses vs oracle .ref, DISARMED default, both modes ==="
for sno in "$FZ"/*.sno; do
    b=$(basename "$sno" .sno); ref="$FZ/$b.ref"
    [ -f "$ref" ] || { echo "  MISS   $b (no .ref)"; rc=1; continue; }
    m3=$(SCRIP_FENCE0_WHACK=0 timeout 10s "$SCRIP" --run "$sno" < /dev/null 2>&1)
    ok3=FAIL; [ "$m3" = "$(cat "$ref")" ] && ok3=PASS
    m4=FAIL
    if SCRIP_FENCE0_WHACK=0 timeout 20s "$SCRIP" --compile "$sno" < /dev/null > "$WORK/$b.s" 2>/dev/null \
       && gcc -no-pie -o "$WORK/$b.bin" "$WORK/$b.s" -L"$RT_DIR" -lscrip_rt -Wl,-rpath,"$RT_DIR" 2>/dev/null; then
        out=$(timeout 20s "$WORK/$b.bin" < /dev/null 2>&1); [ "$out" = "$(cat "$ref")" ] && m4=PASS
    fi
    printf '  %-32s m3=%s m4=%s\n' "$b" "$ok3" "$m4"
    [ "$ok3" = PASS ] && [ "$m4" = PASS ] || rc=1
done
echo "=== FZ GATE -- LOCK 2: no over-release (re-derived from emitted asm, ARMED) ==="
for sno in "$FZ"/*.sno; do
    b=$(basename "$sno" .sno)
    SCRIP_FENCE0_WHACK=1 timeout 20s "$SCRIP" --compile "$sno" < /dev/null > "$WORK/$b.armed.s" 2>/dev/null || { echo "  SKIP   $b (compile failed)"; continue; }
    python3 - "$WORK/$b.armed.s" "$b" <<'PY' || rc=1
import re, sys
path, name = sys.argv[1], sys.argv[2]
LEAF = ('match_span','match_break','match_breakx','match_tab','match_rtab','match_rem','match_bal')
lines = open(path, encoding='utf-8', errors='replace').read().splitlines()
# Parse BOXES: a box opens at `nK_kind_α:` and its ALPHA REGION runs until that box's own beta/gamma/omega
# label or the next box's alpha.  Within the alpha region we take the carve (`sub rsp,N`), the release
# (`add rsp,N` on the alpha line itself) and the FORWARD exit (last `jmp nJ_..._α`).
boxes, cur = {}, None
for ln in lines:
    m = re.match(r'^(n\d+)_([a-z0-9_]+)_(α|β|γ|ω):(.*)$', ln)
    if m:
        lbl, kind, port, rest = m.group(1), m.group(2), m.group(3), m.group(4)
        if port == 'α':
            cur = lbl; boxes[lbl] = dict(kind=kind, sub=0, rel=0, jmp=None)
            a = re.search(r'\badd\s+rsp,\s*(\d+)', rest)
            if a: boxes[lbl]['rel'] = int(a.group(1))
        else:
            cur = None
        body = rest
    else:
        body = ln
    if cur is None: continue
    sm = re.search(r'\bsub\s+rsp,\s*(\d+)', body)
    if sm: boxes[cur]['sub'] += int(sm.group(1))
    jm = re.findall(r'\bjmp\s+(n\d+)_[a-z0-9_]+_α', body)
    if jm: boxes[cur]['jmp'] = jm[-1]
bad, told = 0, set()
for start in [l for l in boxes if boxes[l]['kind'] == 'match_begin']:
    chain, seen, cur, guard = [], set(), boxes[start]['jmp'], 0
    while cur and cur in boxes and cur not in seen and guard < 10000:
        guard += 1; seen.add(cur)
        if boxes[cur]['kind'] == 'match_fence0':
            budget = 0
            for prev in reversed(chain):
                k = boxes[prev]['kind']
                if k.startswith(LEAF): budget += boxes[prev]['sub']; continue
                if boxes[prev]['sub'] == 0: continue
                break
            rel = boxes[cur]['rel']
            if rel > budget:
                bad = 1
                if cur not in told:
                    told.add(cur); print("  OVER-RELEASE %s: %s releases %d but only %d bytes of whitelisted carve stand at the frontier" % (name, cur, rel, budget))
        chain.append(cur); cur = boxes[cur]['jmp']
print("  %-32s %s" % (name, "OK (no fence releases more than its carve)" if not bad else "OVER-RELEASE DETECTED"))
sys.exit(1 if bad else 0)
PY
done
echo "=== FZ REPORT (not a verdict): FZ-3 arm-blocker status ==="
blk="$FZ/fz3_capture_across_fence.sno"
if [ -f "$blk" ]; then
    got=$(SCRIP_FENCE0_WHACK=1 timeout 10s "$SCRIP" --run "$blk" < /dev/null 2>&1)
    if [ "$got" = "$(cat "${blk%.sno}.ref")" ]; then echo "  FZ-3 CLEARED armed -- the depth-model staging may be fixed; RE-EVALUATE arming by default (see bb_match_fence0.cpp)"
    else echo "  FZ-3 still reproduces armed (expected) -- SCRIP_FENCE0_WHACK stays DEFAULT OFF"; fi
fi
[ $rc -eq 0 ] && echo "FZ GATE: GREEN" || echo "FZ GATE: RED"
exit $rc
