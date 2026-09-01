#!/usr/bin/env python3
# util_ladder_walk.py — THE LADDER WALK (ceo instrument, Lon 2026-09-01: "Make a master plan and being CEO ensure the fleet walks along the ladders").
# Reads the rung tables of .github/MASTER-PLAN.md (### LADDER <ID> — <name> ... | rung | row | lane | gate |) and joins each rung's ROW
# with the postoffice (QUEUE.tsv rank/owner/state · claims/<row>.claim holder+DONE · QUEUE.done.tsv · tasks/<row>.task.md) to print,
# per ladder, where every rung stands, then the VIOLATIONS the ceo must act on:
#   V1 RUNG-WITHOUT-ROW   a rung whose row is TO-MINT or has no QUEUE row (the lane named owes the mint)
#   V2 OFF-LADDER SEAT    a seat holding a live claim on a row that is no rung while a rung is FREE (finish or park, then step onto a rung)
#   V3 STALE PARK         a rung PARKED-AWAITING:x / BLOCKED-ON:x whose blocker x is DONE (un-park owed)
#   V4 RANK INVERSION     an off-ladder FREE row at rank 0/1 (ranks 0/1 are the rungs' — the picker would serve it before a rung)
#   V5 UNOWNED BLOCK      a rung bare-PARKED/BLOCKED with owner unassigned (nobody re-runs its gate)
#   V6 ORPHAN             a FREE row with 'prolog' in its topic that sits on no ladder (an HQ places it on a rung or retires it)
# Exit 0 = every rung placed and no violation · 1 = violations printed · 2 = REFUSE (plan or postoffice unreadable, no rung table found).
# ⛔ It answers "does the QUEUE agree with the PLAN"; it does NOT answer "is the seat working" (that is `s4e_msg.sh fleet`'s COMMITS column).
import os, re, sys, glob, argparse
ap = argparse.ArgumentParser()
ap.add_argument('--plan', default=os.environ.get('S4E_HOME', '/home/claude') + '/.github/MASTER-PLAN.md')
ap.add_argument('--po', default='/home/resources/postoffice')
ap.add_argument('--quiet', action='store_true', help='print only the ladder summary lines and violations')
a = ap.parse_args()
def refuse(msg): print('⛔ REFUSE(2): ' + msg); sys.exit(2)
try: plan = open(a.plan, encoding='utf-8').read().split('\n')
except OSError as e: refuse('plan unreadable: %s' % e)
PO = a.po
for need in ('QUEUE.tsv', 'QUEUE.done.tsv', 'claims', 'tasks'):
    if not os.path.exists(os.path.join(PO, need)): refuse('postoffice missing %s under %s' % (need, PO))
# ---- postoffice state
queue = {}
for line in open(os.path.join(PO, 'QUEUE.tsv'), encoding='utf-8', errors='replace'):
    if line.startswith('#') or not line.strip(): continue
    f = line.rstrip('\n').split('\t')
    if len(f) >= 4 and f[0].isdigit(): queue[f[1]] = {'rank': int(f[0]), 'owner': f[2], 'state': f[3]}
done_rows = set()
for line in open(os.path.join(PO, 'QUEUE.done.tsv'), encoding='utf-8', errors='replace'):
    if line.startswith('#') or not line.strip(): continue
    f = line.rstrip('\n').split('\t')
    if len(f) >= 2 and f[0].isdigit(): done_rows.add(f[1])
claims = {}
for c in glob.glob(os.path.join(PO, 'claims', '*.claim')):
    t = os.path.basename(c)[:-6]
    try: body = open(c, encoding='utf-8', errors='replace').read().split('\n')
    except OSError: continue
    claims[t] = {'holder': body[0].strip(), 'done': any(l.strip() == 'DONE' for l in body)}
tasks = {os.path.basename(p)[:-8] for p in glob.glob(os.path.join(PO, 'tasks', '*.task.md'))}
def is_done(t): return t in done_rows or (t in claims and claims[t]['done'])
# ---- plan parse
ladders = []; cur = None; in_table = False
for raw in plan:
    m = re.match(r'^### LADDER (\S+) — (.+)$', raw)
    if m: cur = {'id': m.group(1), 'name': m.group(2).strip(), 'rungs': []}; ladders.append(cur); in_table = False; continue
    if cur is None or not raw.startswith('|'): in_table = False; continue
    cells = [c.strip() for c in raw.strip().strip('|').split('|')]
    if len(cells) < 3: continue
    if cells[0].lower() == 'rung' and cells[1].lower() == 'row': in_table = True; continue
    if set(cells[0]) <= set('-: '): continue
    if in_table: cur['rungs'].append({'rung': cells[0], 'row': cells[1].strip('`'), 'lane': cells[2] if len(cells) > 2 else '', 'gate': cells[3] if len(cells) > 3 else ''})
ladders = [l for l in ladders if l['rungs']]
if not ladders: refuse('no "### LADDER <ID> — <name>" section with a | rung | row | table found in %s' % a.plan)
rung_rows = {r['row'] for l in ladders for r in l['rungs'] if not r['row'].startswith('TO-MINT')}
V = []
def viol(code, text): V.append('%s %s' % (code, text))
any_free_rung = False
for l in ladders:
    counts = {'DONE': 0, 'CLAIMED': 0, 'FREE': 0, 'BLOCKED': 0, 'TO-MINT': 0, 'MISSING': 0}
    lines = []
    for r in l['rungs']:
        t = r['row']
        if t.startswith('TO-MINT'):
            st = t; counts['TO-MINT'] += 1; viol('V1 RUNG-WITHOUT-ROW', '%s/%s: %s — lane %s owes the mint' % (l['id'], r['rung'], t, r['lane']))
        elif is_done(t): st = 'DONE'; counts['DONE'] += 1
        elif t in claims: st = 'CLAIMED:' + claims[t]['holder']; counts['CLAIMED'] += 1
        elif t in queue:
            q = queue[t]; s = q['state']
            if s == 'FREE' or s == '':
                st = 'FREE rank %d' % q['rank']; counts['FREE'] += 1; any_free_rung = True
                if q['rank'] > 1: viol('V4 RANK INVERSION', '%s/%s: rung %s is FREE at rank %d (rungs live at rank 0/1)' % (l['id'], r['rung'], t, q['rank']))
            else:
                st = s + ' rank %d' % q['rank']; counts['BLOCKED'] += 1
                mb = re.match(r'^(PARKED-AWAITING|BLOCKED-ON):(.+)$', s)
                if mb and is_done(mb.group(2)): viol('V3 STALE PARK', '%s/%s: %s is %s but %s is DONE — un-park owed' % (l['id'], r['rung'], t, s, mb.group(2)))
                if s in ('PARKED', 'BLOCKED') and q['owner'] == 'unassigned': viol('V5 UNOWNED BLOCK', '%s/%s: %s is %s with owner unassigned' % (l['id'], r['rung'], t, s))
        elif t in tasks: st = 'ROWLESS (task file only)'; counts['MISSING'] += 1; viol('V1 RUNG-WITHOUT-ROW', '%s/%s: %s has a task file but no QUEUE row — lane %s' % (l['id'], r['rung'], t, r['lane']))
        else: st = 'MISSING'; counts['MISSING'] += 1; viol('V1 RUNG-WITHOUT-ROW', '%s/%s: %s exists nowhere — lane %s owes the mint' % (l['id'], r['rung'], t, r['lane']))
        lines.append('  %-5s %-64s %s' % (r['rung'], t[:64], st))
    print('LADDER %s — %s: rungs=%d done=%d claimed=%d free=%d blocked=%d to-mint=%d missing=%d' % (l['id'], l['name'], len(l['rungs']), counts['DONE'], counts['CLAIMED'], counts['FREE'], counts['BLOCKED'], counts['TO-MINT'], counts['MISSING']))
    if not a.quiet: print('\n'.join(lines))
# V2 off-ladder seats
for t, c in sorted(claims.items()):
    h = c['holder']
    if c['done'] or not re.match(r'^seat\d\d$', h) or t in rung_rows: continue
    if any_free_rung: viol('V2 OFF-LADDER SEAT', '%s holds %s (no rung) while a rung is FREE — finish within the sitting or park, then `next`' % (h, t))
# V4 off-ladder FREE rows at rung ranks
for t, q in sorted(queue.items(), key=lambda kv: (kv[1]['rank'], kv[0])):
    if t in rung_rows or q['state'] not in ('FREE', ''): continue
    if q['rank'] <= 1 and not is_done(t) and t not in claims: viol('V4 RANK INVERSION', 'off-ladder FREE row %s at rank %d — the picker serves it before rungs at that rank' % (t, q['rank']))
# V6 orphans
for t, q in sorted(queue.items()):
    if 'prolog' in t and t not in rung_rows and q['state'] in ('FREE', '') and not is_done(t) and t not in claims: viol('V6 ORPHAN', '%s is a FREE Prolog row on no ladder (rank %d) — hq_C/hq_P place it on a rung or retire it' % (t, q['rank']))
print('VIOLATIONS: %d' % len(V))
for v in V: print('  ' + v)
sys.exit(1 if V else 0)
