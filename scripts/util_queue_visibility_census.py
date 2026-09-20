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
#   P   task file whose ^DONE-WHEN: line is still the MINT PLACEHOLDER (unclosable the same way E is, but
#       harder to see: the line EXISTS, so E cannot catch it, and `done` executes it whole -- prose is not a
#       command, so a seat can do the whole job correctly and still get a refusal INDISTINGUISHABLE from the
#       one a genuinely wrong cure produces (hq_P census 2026-09-05, ceo ruling CEO-289).
# ⛔⭐ CLASS P KEYS ON THE DONE-WHEN LINE, NEVER ON THE FILE, AND THAT IS THE WHOLE POINT OF THE ARM.  The
# obvious census -- grep -l the placeholder text over tasks/ -- returns 112 where the true figure is 44, a
# 2.5x OVER-REPORT, because ledger lines, handoff notes and QA answers MENTION the placeholder; hq_P found
# two such lines they had written themselves an hour earlier recording that they had REMOVED one.  The
# instrument counted the cure as the disease.  ⭐ It fails in the INFLATING direction, which is the more
# persuasive one: a scary number invites action rather than scrutiny.  Same family as `command -v` for an
# oracle -- a well-formed answer to a slightly different question.
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
# ⛔ hq_B and hq_T were MISSING from this set until 2026-09-05 (hq_T) -- a copy-forward from the TRIO era,
# when only two HQs existed.  Harmless TODAY only because class B filters holders that start with 'seat', so
# no HQ is ever tested against it; the moment that filter widens, every hq_B/hq_T claim reads as stood-down.
# A latent wrong answer that is currently unreachable is still a wrong answer, and it costs one line to fix.
active = set(f"seat{i:02d}" for i in range(1, n + 1)) | {'hq_B', 'hq_C', 'hq_P', 'hq_T', 'hq_U', 'hq_S', 'hq_I', 'hq_R', 'hq_V', 'ceo', 'cto', 'coo'}
A2 = sorted(t for t, r in rows.items() if r['state'] == 'FREE' and t in claims and claims[t]['done'])
B  = sorted(t for t, c in claims.items() if not c['done'] and c['holder'].startswith('seat') and c['holder'] not in active)
# ⭐ A TOMBSTONE IS NOT LOST WORK (hq_C 2026-08-27, from triaging this census's own first C! list).  A baton
# whose header says SUPERSEDED or RENAMED is a deliberate REDIRECT that deliberately has no row -- it exists so
# the next reader is sent to the live topic.  The RETIRED- prefix was already excluded for exactly this reason;
# these are the same thing spelled in the header instead of the filename.  ⛔ Counting them made 4 of the first
# 6 C! findings false, and 1 of 1 E findings false (a rename stub needs no DONE-WHEN) -- and a census that
# re-reports correctly-retired work every run trains its readers to skim it, which costs more than the misses
# it was built to catch.  Measured on the first list: icon-r0-bisect (RENAMED by an hq_C ruling),
# raku-print-say-local-arg-marshal-bomb, rebus-raku-loop-condition-hang-family and
# scrip-crashes-not-cleanly-on-unrunnable-input (all three SUPERSEDED, each naming its live successor).
def _head(t):
    try: return open(os.path.join(tasks_dir, t + '.task.md')).readline()
    except OSError: return ''
def _tombstone(t):
    h = _head(t).upper()
    # RESOLVED-BEFORE-CLAIM joined the tombstone family 2026-08-28 (ceo): a baton kept for PROVENANCE after
    # its defect was independently fixed before anyone claimed it — deliberately de-indexed so it cannot be
    # picked up (witness: conform-opsyn-alias-..., flagged as C! four census runs in a row before this line).
    return t.startswith('RETIRED') or 'SUPERSEDED' in h or 'RENAMED' in h or 'RESOLVED' in h
Cx = sorted(tasks - set(rows) - set(done_rows) - {t for t in tasks if _tombstone(t)})
E  = sorted(t for t in tasks if not _tombstone(t) and 'DONE-WHEN:' not in open(os.path.join(tasks_dir, t + '.task.md')).read())
PLACEHOLDER = 'MUST BE MADE RUNNABLE'
def _donewhen_line(t):
    try:
        for ln in open(os.path.join(tasks_dir, t + '.task.md'), encoding='utf-8', errors='replace'):
            if ln.startswith('DONE-WHEN:'): return ln
    except OSError: pass
    return ''
P  = sorted(t for t in tasks if not _tombstone(t) and PLACEHOLDER in _donewhen_line(t))
F  = sorted(t for t, r in rows.items() if ('PARKED' in r['state'] or r['state'] == 'BLOCKED') and r['owner'] == 'unassigned')
# ⛔⭐ R: a DONE row with NO COMPUTED RECEIPT (coo 2026-09-16, ceo CEO-790/793) -- the state column says DONE but no claims/<topic>.claim
# carries DONE, so it reached DONE by a path outside `done` (the verb that COMPUTES completion and, since today, writes a
# RECEIPT line: rc, tree, elapsed, via, time, seat). 17 such rows on 2026-09-16; one, reopened by audit, was RED on origin.
# --grade-receiptless runs each such row's DONE-WHEN (timeout 120 s) and classes it: prose (no runnable criterion / the mint
# placeholder), refuses (rc 2, 120, 126, 127), red (any other non-zero), green (rc 0 -- close it through `done` for its receipt).
R  = sorted(t for t, r in rows.items() if r['state'].split(':')[0] == 'DONE' and not (t in claims and claims[t]['done']))
Rh = sorted(t for t, r in done_rows.items() if r['state'].split(':')[0] == 'DONE' and not (t in claims and claims[t]['done']))
GRADE = '--grade-receiptless' in sys.argv
def _grade(t):
    import subprocess
    dw = _donewhen_line(t)
    if not dw or PLACEHOLDER in dw: return 'prose'
    body = dw[len('DONE-WHEN:'):].strip()
    if not body or body.startswith('⛔') or body.startswith('TBD') or body.lower().startswith('tbd'): return 'prose'
    root = os.environ.get('S4E_HOME') or os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))
    # S4E_DONE_WHEN_RUN=1 admits THE RUN (lib_one_runner.sh) and, since ceo CEO-997, publishes NOTHING: util_score_row.py
    # no longer reads it as a leaderboard-write door. It had to be closed there rather than dropped here -- this census
    # grades EVERY receiptless row's criterion, so while that door stood, a hygiene pass over a row whose DONE-WHEN is a
    # board was a fleet-wide publish button, and the row's own seat never ran it.
    try:
        r = subprocess.run(['bash', '-c', body], cwd=root, env=dict(os.environ, S4E_HOME=root, S4E_DONE_WHEN_RUN='1'), capture_output=True, text=True, timeout=120)
        rc = r.returncode
    except subprocess.TimeoutExpired:
        rc = 124
    if rc == 0: return 'green (rc 0 -- close it through `done` so the claim carries its receipt)'
    if rc in (2, 120, 124, 126, 127): return f'refuses (rc {rc})'
    return f'red (rc {rc})'
findings = 0
for name, lst, note in (("A2 FREE-behind-DONE-claim", A2, "verify each DONE, then release"),
                        ("B  claim held by stood-down seat", B, f"MODE={mode}; corroborate then beta"),
                        ("C! rowless task file", Cx, "mint a row or retire with a note"),
                        ("E  no DONE-WHEN line", E, "unclosable by construction"),
                        ("F  parked/blocked, no owner", F, "name an owner/cadence"),
                        ("P  DONE-WHEN is still the mint placeholder", P, "write a real criterion and prove it RED once; `done` executes this line whole"),
                        ("R  DONE row with NO COMPUTED RECEIPT (no claim carrying DONE)", R, "reached DONE outside `done`; sweep refuses it; re-run `done` for a receipt, or reopen")):
    if lst:
        findings += len(lst)
        print(f"{name} ({len(lst)}) — {note}:")
        for t in lst:
            r = rows.get(t, {})
            print(f"    {t}" + (f"  [rank {r.get('rank','?')}, owner {r.get('owner','?')}]" + (f"  criterion: {_grade(t)}" if GRADE else "") if name.startswith('R ') else ''))
if Rh:
    print(f"R  (history, informational) {len(Rh)} row(s) in QUEUE.done.tsv carry DONE with no claim carrying DONE -- swept before the receipt existed; not counted")
if findings == 0: print("CENSUS CLEAN: every minted row is pickable, closable, and owned."); sys.exit(0)
print(f"TOTAL FINDINGS: {findings}"); sys.exit(1)
