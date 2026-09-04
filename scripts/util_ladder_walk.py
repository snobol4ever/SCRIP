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
ap.add_argument('--v6-rows', action='store_true', help='print V6 orphans one row per line instead of one census line per lane (95 lines on the 2026-09-03 queue; the census is the default so the report stays readable)')
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
# ---- the seven language tables: six map 1:1 to a MASTER-PLAN ladder id; PL has none of its own (Prolog is split T/C/P) so it is the union of those three
LANG_LADDERS = {'SNO': {'SNO'}, 'SC': {'SC'}, 'ICN': {'ICN'}, 'PAS': {'PAS'}, 'RAKU': {'RAKU'}, 'REB': {'REB'}, 'PL': {'T', 'C', 'P'}}
LANG_PREFIX = {'sno': 'SNO', 'snobol4': 'SNO', 'sc': 'SC', 'snocone': 'SC', 'icn': 'ICN', 'icon': 'ICN', 'pas': 'PAS', 'pascal': 'PAS', 'raku': 'RAKU', 'reb': 'REB', 'rebus': 'REB', 'pl': 'PL', 'prolog': 'PL'}
LANG_PREFIX_RE = re.compile(r'^([A-Za-z0-9]+)[-_]')
def row_lang(t):
    m = LANG_PREFIX_RE.match(t)
    return LANG_PREFIX.get(m.group(1).lower()) if m else None
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
ladder_by_id = {l['id']: l for l in ladders}
# ---- LADDER:<tok> LINKS tokens: a row counts as on-ladder if its task file's LINKS line (or anywhere in it) names a ladder id, even when the
# markdown table never lists it by name (this is how Prolog rows attach to T/C/P without a per-row table entry for every one of them)
LADDER_TOK_RE = re.compile(r'\bLADDER:([A-Za-z0-9]+)')
links_ladders = {}
non_ladder = set()
for t in tasks:
    try: body = open(os.path.join(PO, 'tasks', t + '.task.md'), encoding='utf-8', errors='replace').read()
    except OSError: continue
    ids = set(LADDER_TOK_RE.findall(body))
    if ids: links_ladders[t] = ids
    if 'NON-LADDER' in body: non_ladder.add(t)
def on_ladder(t): return t in rung_rows or t in links_ladders
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
# per-language rollup — the 7-way view I22 asks for; PL aggregates ladders T/C/P since Prolog has no single table of its own
for L in ('SNO', 'SC', 'ICN', 'PAS', 'RAKU', 'REB', 'PL'):
    ids = LANG_LADDERS[L]
    members = {r['row'] for lid in ids if lid in ladder_by_id for r in ladder_by_id[lid]['rungs'] if not r['row'].startswith('TO-MINT')}
    members |= {t for t, tok in links_ladders.items() if tok & ids}
    d = c = f = b = m = 0
    for t in members:
        if is_done(t): d += 1
        elif t in claims: c += 1
        elif t in queue:
            if queue[t]['state'] in ('FREE', ''): f += 1
            else: b += 1
        else: m += 1
    print('LADDER-TABLE %s members=%d done=%d claimed=%d free=%d blocked=%d missing=%d' % (L, len(members), d, c, f, b, m))
# V2 off-ladder seats
for t, c in sorted(claims.items()):
    h = c['holder']
    if c['done'] or not re.match(r'^seat\d\d$', h) or t in rung_rows: continue
    if any_free_rung: viol('V2 OFF-LADDER SEAT', '%s holds %s (no rung) while a rung is FREE — finish within the sitting or park, then `next`' % (h, t))
# V4 off-ladder FREE rows at rung ranks
# ⛔⭐ PACKAGE-CLASS ROWS ARE EXEMPT (RULES.md § FACT RULES THE PACKAGE-CLASS RULE, ceo CEO-230 2026-09-04 16:18, on Lon 13:20:
# the percentage IS the V column). A class row minted from a vendored-package census -- topic shape <lang>-<package>-class-<slug>
# (icon-arizona-class-…, pascal-fpc-class-…, prolog-inria-class-…) -- is LADDER-RANK WORK by law, so rank 1 is where it belongs
# and a V4 line naming one is noise: the 16:14 walk printed 18 such lines beside the real inversions, and a report that is
# two-thirds noise is a report nobody reads (see V6's census ruling above). ⛔ EXEMPT BY SHAPE, NOT BY A LIST OF PACKAGE
# NAMES: a list is a census that drifts the day a new suite is vendored, and drifts SILENTLY -- the new package's class rows
# would be flagged again and read as genuine inversions by whoever had stopped seeing the old ones. The shape is what the
# law defines: a recognised language prefix, exactly ONE package token, then `-class-`. `prolog-master-red-class-…` (two
# tokens) is a master-board red class, not a package class, and stays a candidate. ⭐ EXEMPTED ROWS ARE COUNTED ON ONE
# LINE, so the exemption is visible rather than silent -- a filter that removes lines with no trace is indistinguishable
# from a bug that lost them (row ladder-walk-v4-exempts-package-class-rows, hq_T; gate test_gate_ladder_walk_v4_exempts_
# package_class_rows.sh proves both halves: a package class row is silent, a genuine off-ladder rank-1 row is still flagged).
PKG_CLASS_RE = re.compile(r'^(?:%s)-[a-z0-9]+-class-' % '|'.join(sorted(LANG_PREFIX)))
_pkg_class_exempt = []
for t, q in sorted(queue.items(), key=lambda kv: (kv[1]['rank'], kv[0])):
    if t in rung_rows or q['state'] not in ('FREE', ''): continue
    if q['rank'] <= 1 and not is_done(t) and t not in claims:
        if PKG_CLASS_RE.match(t): _pkg_class_exempt.append(t); continue
        viol('V4 RANK INVERSION', 'off-ladder FREE row %s at rank %d — the picker serves it before rungs at that rank' % (t, q['rank']))
if _pkg_class_exempt: print('V4 EXEMPT (package-class rule): %d FREE <lang>-<package>-class-* row(s) at rank 0/1 -- ladder-rank by law, not inversions' % len(_pkg_class_exempt))
# V6 orphans — any row of any recognized language (not just Prolog) sitting on no ladder: no table entry, no LADDER:<tok> LINKS tag, no NON-LADDER exemption
# ⛔ ONE CENSUS LINE PER LANE, NOT ONE PER ROW (ceo 2026-09-03 22:33). Widening V6 from Prolog to all seven
# languages took it from 2 lines to 95, and the ceo reads this every tick on an 8% budget: 95 lines is not a
# violation report, it is a haystack, and the reliable response to a haystack is to stop reading it. ⭐ The
# count is the actionable part; the names are triage. So each lane gets ONE line carrying the TOTAL (nothing is
# hidden) plus its three highest-ranked names, and --v6-rows still prints every row for whoever is working the
# lane. A report nobody reads measures nothing, which is the same failure as a gate nobody runs.
_orphans = {}
for t, q in sorted(queue.items()):
    L = row_lang(t)
    if L and q['state'] in ('FREE', '') and not is_done(t) and t not in claims and t not in non_ladder and not on_ladder(t):
        _orphans.setdefault(L, []).append((q['rank'], t))
for L in sorted(_orphans, key=lambda k: (-len(_orphans[k]), k)):
    rows = sorted(_orphans[L])
    if a.v6_rows:
        for r, t in rows:
            viol('V6 ORPHAN', '%s is a FREE %s row on no ladder (rank %d) — its owning HQ places it on a rung, tags LINKS: LADDER:<id>, or marks NON-LADDER' % (t, L, r))
    else:
        top = ', '.join('%s (rank %d)' % (t, r) for r, t in rows[:3])
        viol('V6 ORPHAN CENSUS', '%-8s %3d FREE off-ladder row(s) — highest-ranked: %s%s' % (L, len(rows), top, '' if len(rows) <= 3 else ' … +%d more (--v6-rows for all)' % (len(rows) - 3)))
print('VIOLATIONS: %d' % len(V))
for v in V: print('  ' + v)
sys.exit(1 if V else 0)
