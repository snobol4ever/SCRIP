#!/usr/bin/env python3
# util_queue_visibility_census.py (ceo, 2026-08-28 — born from Lon's trust audit: "if items got lost the
# instant they were minted, you must go back and scan for missed items").  THE PICKER'S RULE IS THE
# INSTRUMENT: any claim file hides a row, DONE or not (s4e_msg.sh next).  Every prior by-eye health check
# that read QUEUE.tsv's state column over-counted pickable work and missed hidden rows — this script asks
# the authoritative question instead, and it found (first run): the beauty blocker hidden behind a false
# bare-DONE whose DONE-WHEN named a script that never existed; 7 rowless batons incl. an announce-tier
# item; 3 claims held by stood-down seats.  Classes:
#   A2  FREE row hidden behind a DONE-marked claim (the suites-row class — verify the DONE, then release)
#   B   live claim held by a stood-down seat (liveness law: corroborate, then beta)
#   C!  task file with NO row in QUEUE.tsv AND none in QUEUE.done.tsv (unreachable work)
#   E   task file with no ^DONE-WHEN: line (unclosable by construction)
#   F   PARKED/BLOCKED row with no owner (nobody re-runs its gate)
# Exit 0 with all classes empty; exit 1 with findings; exit 2 if the postoffice is unreadable (REFUSE,
# never a silent green — a census that cannot read its subject has measured nothing).
import os, sys
PO = os.environ.get("S4E_PO", "/home/resources/postoffice")
try:
    qlines = open(os.path.join(PO, "QUEUE.tsv")).readlines()
    dlines = open(os.path.join(PO, "QUEUE.done.tsv")).readlines()
    claims_dir = os.path.join(PO, "claims"); tasks_dir = os.path.join(PO, "tasks")
    claim_files = os.listdir(claims_dir); task_files = os.listdir(tasks_dir)
except OSError as e:
    print(f"REFUSED (rc=2): cannot read the postoffice: {e}", file=sys.stderr); sys.exit(2)
def rows_of(lines):
    out = {}
    for ln in lines:
        if ln.startswith('#') or not ln.strip(): continue
        p = ln.rstrip('\n').split('\t')
        if len(p) >= 4: out[p[1]] = {'rank': p[0], 'owner': p[2], 'state': p[3]}
        elif len(p) >= 2: out[p[1]] = {'rank': p[0], 'owner': '?', 'state': '?'}
    return out
rows = rows_of(qlines); done_rows = rows_of(dlines)
claims = {}
for f in claim_files:
    if not f.endswith('.claim'): continue
    body = [l.strip() for l in open(os.path.join(claims_dir, f)) if l.strip()]
    claims[f[:-6]] = {'holder': body[0] if body else '?', 'done': 'DONE' in body}
tasks = set(f[:-8] for f in task_files if f.endswith('.task.md'))
mode = open(os.path.join(PO, "MODE")).readline().strip()
n = int(mode.split('-')[1]) if mode.startswith('FLEET-') else 0
active = set(f"seat{i:02d}" for i in range(1, n + 1)) | {'hq_C', 'hq_P', 'ceo'}
A2 = sorted(t for t, r in rows.items() if r['state'] == 'FREE' and t in claims and claims[t]['done'])
B  = sorted(t for t, c in claims.items() if not c['done'] and c['holder'].startswith('seat') and c['holder'] not in active)
Cx = sorted(tasks - set(rows) - set(done_rows) - {t for t in tasks if t.startswith('RETIRED')})
E  = sorted(t for t in tasks if 'DONE-WHEN:' not in open(os.path.join(tasks_dir, t + '.task.md')).read())
F  = sorted(t for t, r in rows.items() if ('PARKED' in r['state'] or r['state'] == 'BLOCKED') and r['owner'] == 'unassigned')
findings = 0
for name, lst, note in (("A2 FREE-behind-DONE-claim", A2, "verify each DONE, then release"),
                        ("B  claim held by stood-down seat", B, f"MODE={mode}; corroborate then beta"),
                        ("C! rowless task file", Cx, "mint a row or retire with a note"),
                        ("E  no DONE-WHEN line", E, "unclosable by construction"),
                        ("F  parked/blocked, no owner", F, "name an owner/cadence")):
    if lst:
        findings += len(lst)
        print(f"{name} ({len(lst)}) — {note}:")
        for t in lst: print(f"    {t}")
if findings == 0: print("CENSUS CLEAN: every minted row is pickable, closable, and owned."); sys.exit(0)
print(f"TOTAL FINDINGS: {findings}"); sys.exit(1)
