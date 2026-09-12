#!/usr/bin/env python3
"""util_swi_match.py -- grade ONE swi_tests file, per CASE, by agreement with the oracle's ref.

Usage: util_swi_match.py SOURCE.pl REF ACTUAL

  REF      the oracle's verdicts beside the source, cut by util_swi_cut_refs.sh: PASS|FAIL|BLOCKED unit:test, EMPTY unit, UNGRADABLE unit reason
  ACTUAL   SCRIP's stdout through corpus/tests/prolog/plunit.pl: '  pass: unit:test', '  FAIL: unit:test  (why)', '  skip: unit:test  [why]'

THE POPULATION IS THE ORACLE'S ENUMERATION: every case the ref names (a forall(Gen) test is one case per generator
instance, recorded by plunit as @(name,Bindings); a macro-generated test has no test/2 head in the source at all), plus,
for a unit the oracle could not run (EMPTY, UNGRADABLE), the test/2 heads the source declares inside that unit (at
least one entry, so a unit is never silently zero).  Names are matched by unit:name and OCCURRENCE ORDER on both sides,
with @(name,Bindings) normalised to name.

Prints one TSV row per case:
  <unit:test#k>  PASS      agrees with the oracle (both pass, or both fail: an agreement control, marked in col 3)
  <unit:test#k>  FAIL      disagrees, col 3 says how (oracle PASS / scrip FAIL (why); oracle FAIL / scrip pass; no verdict from scrip)
  <unit:test#k>  UNGRADED  the oracle gave no PASS/FAIL for it: BLOCKED, EMPTY unit, or UNGRADABLE unit <oracle reason>
then one summary line:
  MATCH declared=N graded=N hit=N hit_pass=N hit_fail_agree=N miss=N ungraded=N
"""
import re, sys
if len(sys.argv) != 4:
    sys.stderr.write(__doc__); sys.exit(2)
src_path, ref_path, act_path = sys.argv[1:4]
begin_re = re.compile(r'^\s*:-\s*begin_tests\(\s*([A-Za-z0-9_]+)')
end_re = re.compile(r'^\s*:-\s*end_tests\(')
test_re = re.compile(r"^\s*test\(\s*('(?:[^'\\]|\\.)*'|[A-Za-z0-9_]+)")
heads = {}
unit = None
for line in open(src_path, encoding='utf-8', errors='replace'):
    m = begin_re.match(line)
    if m: unit = m.group(1); heads.setdefault(unit, []); continue
    if end_re.match(line): unit = None; continue
    if unit is None: continue
    m = test_re.match(line)
    if m:
        name = m.group(1)
        if name.startswith("'"): name = name[1:-1].replace("\\'", "'")
        heads[unit].append(name)
def norm(name):
    m = re.match(r'^@\(([^,]+),', name)
    return m.group(1) if m else name
def split_key(key):
    u, _, t = key.partition(':')
    return u, norm(t)
cases = []
for line in open(ref_path, encoding='utf-8', errors='replace'):
    parts = line.rstrip('\n').split(' ', 2)
    if len(parts) < 2: continue
    verdict, key = parts[0], parts[1]
    if verdict in ('PASS', 'FAIL', 'BLOCKED'):
        u, t = split_key(key); cases.append((u, t, verdict, ''))
    elif verdict in ('EMPTY', 'UNGRADABLE'):
        why = parts[2] if len(parts) > 2 else ''
        names = heads.get(key, []) or ['(unit)']
        for t in names: cases.append((key, t, verdict, why))
act = {}
act_re = re.compile(r'^\s*(pass|FAIL|skip):\s+(\S+?)(?:\s+(.*))?$')
for line in open(act_path, encoding='utf-8', errors='replace'):
    m = act_re.match(line.rstrip('\n'))
    if not m: continue
    u, t = split_key(m.group(2))
    act.setdefault('%s:%s' % (u, t), []).append((m.group(1), (m.group(3) or '').strip()))
seen = {}
hit = hit_pass = hit_fail = miss = ungraded = graded = 0
for u, t, r, why in cases:
    key = '%s:%s' % (u, t)
    k = seen.get(key, 0); seen[key] = k + 1
    label = '%s#%d' % (key, k + 1)
    if r in ('BLOCKED', 'EMPTY', 'UNGRADABLE'):
        print('%s\tUNGRADED\t%s%s' % (label, r, (' ' + why) if why else '')); ungraded += 1; continue
    graded += 1
    av = act.get(key, [])
    a = av[k] if k < len(av) else None
    if a is None:
        print('%s\tFAIL\toracle %s / no verdict from scrip' % (label, r)); miss += 1
    elif r == 'PASS' and a[0] == 'pass':
        print('%s\tPASS\t' % label); hit += 1; hit_pass += 1
    elif r == 'FAIL' and a[0] == 'FAIL':
        print('%s\tPASS\tagreement control: the oracle fails this case too %s' % (label, a[1])); hit += 1; hit_fail += 1
    else:
        print('%s\tFAIL\toracle %s / scrip %s %s' % (label, r, a[0], a[1])); miss += 1
print('MATCH declared=%d graded=%d hit=%d hit_pass=%d hit_fail_agree=%d miss=%d ungraded=%d' % (len(cases), graded, hit, hit_pass, hit_fail, miss, ungraded))
