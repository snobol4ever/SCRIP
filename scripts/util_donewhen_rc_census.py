#!/usr/bin/env python3
# util_donewhen_rc_census.py -- THE ONE AUTHORITY for "can this DONE-WHEN tell a refusal from a red".
# hq_B 2026-09-13, on the ceo audit FINDING-2026-09-11-ceo-a-done-when-that-captures-a-board-turns-could-not-
# measure-into-failed.md. Two classes, both found by that audit, both cured in place by this script's --cure.
# CLASS A -- a DONE-WHEN captures a runner's output and never consults that runner's rc. Under ONE RUNNER, ONE
#   BOARD a runner answers any seat but the coo with rc=2 REFUSE; the criterion captures that TEXT, the marker
#   grep misses, and a refusal degrades into rc=1 FAILED. The row is fine and the audit reads it as red.
# CLASS B -- a DONE-WHEN hardcodes its author's root as its S4E_HOME fallback (${S4E_HOME:-/home/claude_cfo}).
#   `s4e_msg.sh done` exports S4E_HOME so the fallback never fires there; it fires exactly when a seat runs the
#   criterion BY HAND to audit it, and then it silently grades ANOTHER SEAT'S TREE and can print PASS.
# +-- WHAT THIS DOES NOT CALL A DEFECT, WHICH IS THE HALF THAT TOOK THE WORK -------------------------------+
# A capture that never looks at rc but REFUSES rc=2 when the expected marker is absent can already tell "never
# ran" from "ran and red" -- it grades a marker instead of a status, which is a different correct answer, not a
# worse one. 50 criteria are that shape. Calling them defective and "curing" them would be this row's own
# lesson inverted: an instrument answering a narrower question than it thinks it asked. They are counted and
# printed as HONEST and never rewritten.
# +-- AND THE TRAP INSIDE THE CURE --------------------------------------------------------------------------+
# 19 of the defective captures PIPE the runner into grep/tail. For those, appending `rc=$?` reads THE PIPE's
# status, not the runner's -- the $?-after-a-pipeline trap, i.e. the cure would wear the defect's own shape.
# Those are restructured instead: capture raw, check rc, THEN filter.
# The inserted guard is a BRACE GROUP ending in `[ "$_dwrc" = 0 ]` so a capture sitting inside an `&&` chain
# keeps its short-circuit exactly; appending `; _dwrc=$?` bare would silently break the chain.
# ⛔ IT NEVER EXECUTES A CRITERION and never expands one. Static text only.
import re, os, glob, sys, json
TEXTTOOL = {'grep','egrep','fgrep','ls','sed','awk','cat','wc','find','head','tail','nm','git','printf',
            'echo','mktemp','date','stat','sort','uniq','cut','tr','basename','dirname','md5sum','readlink',
            'objdump','readelf','cmp','diff','jq','realpath','pwd','id','comm','xargs','true','false'}
RUNNER = re.compile(r'(?:^|[\s;&|(])(?:bash|sh|python3|\./|\$\{?[A-Za-z_])[^;|&]*?[\w/.$}"\']+\.(sh|py)\b')
CAP = re.compile(r'([A-Za-z_][A-Za-z0-9_]*)=\$\(')
GUARD = ('_dwrc=$?; if [ "$_dwrc" = 2 ]; then '
         'echo "REFUSE(2): the runner could not measure (rc=2) -- this is not a red"; exit 2; fi; ')
HEADS = r'^(GOAL|LINKS|RANK|SCOPE|LEDGER|OWNER|BLOCKED-ON|FINDING|MINTED BY|DONE-WHEN|## )'
def block(text):
    m = re.search(r'^DONE-WHEN:', text, re.M)
    if not m: return None
    s = m.end()
    nxt = re.search(HEADS, text[s:], re.M)
    return (s, s + nxt.start()) if nxt else (s, len(text))
def first_word(inner):
    t = inner.strip()
    for _ in range(3):
        t = re.sub(r'^(?:[A-Za-z_][A-Za-z0-9_]*=(?:"[^"]*"|\'[^\']*\'|\S*)\s+)+', '', t)
        t = re.sub(r'^timeout\s+\S+\s+', '', t)
        t = re.sub(r'^(?:cd\s+(?:"[^"]*"|\S+)\s*(?:\|\|\s*\S+\s*)?&&\s*)+', '', t)
    return t.split()[0] if t.split() else ''
def top_pipe(s):
    d = 0; q = None
    for i, c in enumerate(s):
        if q:
            if c == q: q = None
            continue
        if c in '"\'': q = c
        elif c in '([{': d += 1
        elif c in ')]}': d -= 1
        elif c == '|' and d == 0:
            return -1 if (i + 1 < len(s) and s[i+1] == '|') else i
    return -1
def spans(s):
    out = []
    for m in CAP.finditer(s):
        i = m.end(); depth = 1
        while i < len(s) and depth:
            if s[i] == '(': depth += 1
            elif s[i] == ')': depth -= 1
            i += 1
        out.append((m.group(1), m.start(), m.end(), i))
    return out
def scan(seg):
    """-> (defective_edits, honest_count).  edit = (start, end, replacement, piped)"""
    edits, honest = [], 0
    for var, a_start, i_start, end in spans(seg):
        inner = seg[i_start:end-1]
        fw = first_word(inner)
        if fw.split('/')[-1] in TEXTTOOL or fw in TEXTTOOL: continue
        if not RUNNER.search(inner): continue
        if not re.search(r'[\$"\{]' + re.escape(var) + r'\b', seg[end:]): continue
        tail = seg[end:end+120]
        rc_checked = bool(re.match(r'\s*;?\s*(rc|RC|r|_rc|st|status|_dwrc)=\$\?', tail)) or bool(re.match(r'\s*\|\|', tail))
        pi = top_pipe(inner); piped = pi >= 0
        refuses = False
        for g in re.finditer(r'[\$"\{]' + re.escape(var) + r'\b', seg[end:]):
            sub = seg[end+g.start(): end+g.start()+260]
            if re.search(r'\|\|\s*\{[^}]*exit\s+2', sub) or re.search(r'&&\s*\{[^}]*exit\s+2', sub):
                refuses = True; break
        if (rc_checked and not piped) or refuses:
            if refuses and not rc_checked: honest += 1
            continue
        if piped:
            runner, filt = inner[:pi].strip(), inner[pi+1:].strip()
            repl = '{ _dwraw=$(' + runner + '); ' + GUARD + var + '=$(printf "%s\\n" "$_dwraw" | ' + filt + '); }'
        else:
            repl = '{ ' + var + '=$(' + inner + '); ' + GUARD + '[ "$_dwrc" = 0 ]; }'
        edits.append((a_start, end, repl, piped))
    # ⛔ NESTED/OVERLAPPING CAPTURES CORRUPT A REVERSE-ORDER REWRITE. `VAR=$(` matches inside an outer
    # capture too, and applying both edits shifts the other's offsets -- measured: it spliced a guard into
    # the middle of `g=$(cd "$R/S` and produced two unparseable criteria. Keep the OUTERMOST of any
    # overlapping pair and drop the rest; a criterion that needs two guards gets the one that encloses.
    kept = []
    for ed in sorted(edits, key=lambda x: (x[0], -x[1])):
        if all(ed[1] <= k[0] or ed[0] >= k[1] for k in kept):
            kept.append(ed)
    return kept, honest
def classB(seg):
    return [m.group(1).strip() for m in re.finditer(r'\$\{S4E_HOME:-([^}]*)\}', seg)
            if re.match(r'^/home/claude', m.group(1).strip())]
def main():
    ap_cure = '--cure' in sys.argv
    tasks = os.environ.get('S4E_TASKS', '/home/resources/postoffice/tasks')
    if not os.path.isdir(tasks):
        print(f"REFUSE(2): tasks directory unreachable at {tasks} -- a census that cannot see its population must not print 0")
        return 2
    files = sorted(glob.glob(os.path.join(tasks, "*.task.md")))
    if not files:
        print(f"REFUSE(2): no *.task.md under {tasks} -- cannot measure")
        return 2
    A, B, honest, touched = [], [], 0, 0
    for p in files:
        raw = open(p, encoding='utf-8', errors='replace').read()
        br = block(raw)
        if not br: continue
        s, e = br; seg = raw[s:e]; orig = seg
        base = os.path.basename(p)[:-len('.task.md')]
        fbs = classB(seg)
        if fbs: B.append({'baton': base, 'fallbacks': sorted(set(fbs))})
        edits, h = scan(seg); honest += h
        for var_e in edits:
            A.append({'baton': base, 'piped': var_e[3], 'at': seg[var_e[0]:var_e[0]+70].strip()})
        if ap_cure:
            # ⛔ CLASS B FIRST, THEN RE-SCAN. The B substitution SHORTENS the criterion (${S4E_HOME:-/home/
            # claude_ceo} -> ${S4E_HOME:-$PWD} loses 12 bytes), so class-A offsets computed against the
            # pre-substitution text land 12 bytes late and splice a guard into the middle of a token --
            # measured: it produced `g=$(cd "$R/S{ _dwraw=$(...`, two criteria that no longer parse. An
            # offset is only valid against the exact string it was computed from.
            seg = re.sub(r'(\$\{S4E_HOME:-)/home/claude[A-Za-z0-9_]*(\})', r'\1$PWD\2', seg)
            edits, _h2 = scan(seg)
            for a, en, repl, _ in reversed(edits):
                seg = seg[:a] + repl + seg[en:]
            if seg != orig:
                open(p, 'w', encoding='utf-8').write(raw[:s] + seg + raw[e:]); touched += 1
    if '--json' in sys.argv:
        print(json.dumps({'population': len(files), 'A': A, 'B': B, 'honest': honest}, indent=1)); return 0
    print(f"DONE-WHEN rc/root census over {len(files)} batons at {tasks}")
    print(f"  CLASS A defective (captures a runner, cannot tell a refusal from a red): {len(A)}"
          f"  [piped, where rc=$? would read the pipe: {sum(1 for x in A if x['piped'])}]")
    print(f"  CLASS A honest   (no rc check, but refuses 2 when the marker is absent): {honest}  -- not a defect, never rewritten")
    print(f"  CLASS B          (hardcodes a seat root as its S4E_HOME fallback):       {len(B)}")
    if ap_cure:
        print(f"  CURED IN PLACE: {touched} baton file(s) rewritten"); return 0
    if '--list' in sys.argv:
        for x in A: print(f"    A{'|pipe' if x['piped'] else '     '} {x['baton']}  [{x['at']}]")
        for x in B: print(f"    B       {x['baton']}  {x['fallbacks']}")
    return 1 if (A or B) else 0
sys.exit(main())
