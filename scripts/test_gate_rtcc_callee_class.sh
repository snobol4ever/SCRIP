#!/usr/bin/env bash
# test_gate_rtcc_callee_class.sh — RTCC CALLEE-CLASS GATE (GOAL-RBP-EARN, Lon directive s65).
#
# x86_rtcc_clob() in src/templates/x86_asm.h claims, PER SYMBOL, which of the CLAIMED registers
# {r8,r9,r10,r11} a callee may disturb.  The emitter trusts that claim to SKIP writeback/reload
# slots — so if the claim ever stops matching the assembly, the veneer stops preserving a live VM
# global and the failure mode is a WILD JUMP, not a wrong answer (rtcc_init.c's own words).  The
# table is compile-time truth about hand-written .S bodies that nothing otherwise ties together.
#
# THIS GATE RE-DERIVES THE CENSUS FROM SOURCE AND DIFFS IT AGAINST THE TABLE.  Three invariants,
# any of which failing makes the mask a lie:
#   (1) NO GATE      — a listed symbol must not carry RTX_GATE.  A gate's OFF arm tail-jumps into a
#                      C twin, which makes the register discipline a RUNTIME property (SCRIP_RTX_<FAM>=0
#                      flips it) and no compile-time claim about it can be sound.
#   (2) NO C EXIT    — no call/jmp/jcc leaving the reachable body to a c_* twin, an @PLT symbol, or any
#                      other runtime entry.  The cold delegate is ALWAYS a tail `jmp`, never a `call`:
#                      a census that greps for `call` alone reports pure asm that is nothing of the kind.
#   (3) MASK COVERS  — every destination-position write to r8/r9/r10/r11 anywhere in the reachable body
#                      must appear in that symbol's mask.  Writes are counted through r8d/r8w/r8b too.
#
# Reachability follows local .L labels ACROSS RTX_FUNC boundaries (rtx_alloc.S really does share a tail:
# rt_str_alloc falls into rt_agg_alloc's .Lga_armed).  A jump to a local label this script cannot find is
# reported UNANALYZABLE and FAILS — the gate refuses to certify a body it could not read.
#
# ⛔ UNICODE.  Two RTX entries are spelled with Greek port names (rt_gen_spine_pass_γ / _ω).  An
# [A-Za-z0-9_] census cannot see them and a \b word boundary will not close against a Greek suffix.
# Three separate scans were silently partial for exactly this reason during s65.  Everything here is
# fixed-string or explicitly Unicode-aware; do not "simplify" it back to an ASCII class.
#
# Usage: bash scripts/test_gate_rtcc_callee_class.sh    (exit 0 = green)
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
set -u
ROOT=${ROOT:-$S4E/SCRIP}
python3 - "$ROOT" <<'PY'
import re, sys, glob, os
root = sys.argv[1]
hdr  = os.path.join(root, 'src/templates/x86_asm.h')

# ---- 1. parse the claim table out of x86_rtcc_clob() -------------------------------------------
src = open(hdr, encoding='utf-8').read()
m = re.search(r'static inline unsigned x86_rtcc_clob\(.*?\n\}', src, re.S)
if not m:
    print("FAIL: x86_rtcc_clob() not found in x86_asm.h"); sys.exit(1)
body = m.group(0)
BIT = {'RTCC_C_R8':'r8', 'RTCC_C_R9':'r9', 'RTCC_C_R10':'r10', 'RTCC_C_R11':'r11'}
claim = {}
for name, expr in re.findall(r'\{\s*"((?:[^"\\]|\\.)*)"\s*,\s*([^}]+?)\}', body):
    sym = name.encode().decode('unicode_escape')          # \u03b3 -> γ
    if 'RTCC_C_ALL' in expr:                              # unlisted-by-value: nothing to prove
        continue
    regs = {BIT[b] for b in BIT if b in expr}
    claim[sym] = regs
if not claim:
    print("FAIL: parsed zero classified symbols — table shape changed, gate is blind"); sys.exit(1)

# ---- 2. index every RTX_FUNC block and every local label ---------------------------------------
blocks, labels = {}, {}                                    # sym -> [lines] ; .Llabel -> sym
for path in sorted(glob.glob(os.path.join(root, 'src/runtime/rtx/*.S'))):
    cur = None
    for line in open(path, encoding='utf-8', errors='replace'):
        f = re.search(r'RTX_FUNC\(([^)]*)\)', line)
        if f:
            cur = f.group(1).strip(); blocks[cur] = []; continue
        if re.search(r'RTX_ENDF\(', line):
            cur = None; continue
        if cur is None: continue
        blocks[cur].append(line)
        lab = re.match(r'^\s*(\.L[^\s:]+):', line)
        if lab: labels[lab.group(1)] = cur

WRITE = re.compile(r'^\s*(?:mov|movq|movabs|lea|xor|add|sub|and|or|shl|shr|sar|imul|inc|dec|neg|not|pop'
                   r'|movzx|movsx|movsxd|xchg|set[a-z]{1,3}|cmov[a-z]{1,3})\s+([a-z0-9]+)\s*(?:,|$)', re.I)
XFER  = re.compile(r'^\s*(?:jmp|call|j[a-z]{1,3})\s+(\S+)', re.I)
def canon(r):
    g = re.fullmatch(r'(r(?:8|9|10|11))[dwb]?', r.lower()); return g.group(1) if g else None

fails, checked = [], 0
for sym, mask in sorted(claim.items()):
    if sym not in blocks:
        fails.append((sym, "listed in x86_rtcc_clob() but no RTX_FUNC body found")); continue
    checked += 1
    seen, todo, writes, gate, cexit, unana = set(), [sym], set(), False, [], []
    while todo:
        b = todo.pop()
        if b in seen or b not in blocks: continue
        seen.add(b)
        for line in blocks[b]:
            code = line.split('/*')[0]
            if 'RTX_GATE(' in line and 'RTX_GATE_DEF' not in line: gate = True
            w = WRITE.match(code)
            if w:
                r = canon(w.group(1))
                if r: writes.add(r)
            x = XFER.match(code)
            if x:
                t = x.group(1).rstrip(',')
                if t.startswith('.L'):
                    if t in labels: todo.append(labels[t])
                    else: unana.append(t)
                elif re.fullmatch(r'r(?:ax|cx|dx|si|di|bx|bp|sp|8|9|1[0-5])', t):
                    unana.append("indirect via " + t)
                else:
                    cexit.append(t)
    if gate:  fails.append((sym, "carries RTX_GATE — register discipline is runtime-variable, mask cannot be sound"))
    if cexit: fails.append((sym, "exits to " + ", ".join(sorted(set(cexit))) + " — not self-contained"))
    if unana: fails.append((sym, "UNANALYZABLE transfer to " + ", ".join(sorted(set(unana)))))
    extra = writes - mask
    if extra:
        fails.append((sym, "writes %s but mask says %s — VENEER WOULD DROP A LIVE SLOT"
                      % (",".join(sorted(extra)), ",".join(sorted(mask)) or "<none>")))

print("=== RTCC CALLEE-CLASS GATE — %d classified symbols re-derived from src/runtime/rtx/*.S ===" % checked)
for sym, why in fails: print("  FAIL  %-28s %s" % (sym, why))
if fails:
    print("\n%d violation(s).  The mask table no longer matches the assembly." % len(fails))
    print("Fix the mask in x86_rtcc_clob(), or drop the symbol from the table so it falls back to RTCC_C_ALL.")
    sys.exit(1)
print("  PASS  every mask is covered; no gates, no C exits, no unanalyzable transfers.")
sys.exit(0)
PY
