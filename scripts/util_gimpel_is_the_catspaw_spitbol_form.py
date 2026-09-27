#!/usr/bin/env python3
# util_gimpel_is_the_catspaw_spitbol_form.py -- Lon 2026-09-27 ~12:0x CDT, in-chat to the ceo, verbatim: "Use the *.inc names
# exclusively. Ensure we have the SPITBOL dialect from the Mark Emmer's distribution, and not the SNOBOL4 dialect." (CEO-1319)
# The vendored Gimpel package must be Catspaw's (Mark Emmer's) Algorithms-in-SNOBOL4 distribution v1.06, its \SPITBOL form,
# kept at /home/resources/gimpel/SPITBOL -- never its \SNOBOL4 (SNOBOL4+) form and never a fleet edit. Four checks, each named:
#   FORM     every file of the \SPITBOL form is vendored under its own name LOWER-CASED (NAME.INC -> name.inc, ASM.SPT -> asm.spt,
#            PHRASES.IN -> phrases.in) and equals it with only CR and ^Z dropped and trailing blanks dropped: all 61 of the form's
#            -INCLUDE targets and its INPUT file names are spelled lower-case, so lower-cased names make the sources run verbatim on a
#            case-sensitive file system with no edit to any line (the two DOS 8.3 names, stringout.inc and resolution.inc, are LOCAL.tsv)
#   NOTHING  every other vendored file is a driver's own (NAME_driver.sno/.ref/.input/.in), a sidecar the runner or the
#            inventory reads (ALL.*, *.tsv, README.md, PROVENANCE.md), or declared in LOCAL.tsv (name<TAB>reason)
#   NAMES    no library or program is named *.sno or with an upper-case extension: *.inc exclusively, a program *.spt
#   KEYS     the newest gimpel pass in the progress DB keys no library or program by a .sno name
# rc 0 all four hold, rc 1 any fails (each failure named), rc 2 cannot measure.
import os, re, sys
home = os.environ.get('S4E_HOME') or os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
V = os.path.join(home, 'corpus', 'packages', 'snobol4', 'gimpel')
SP = os.environ.get('GIMPEL_SPITBOL_FORM', '/home/resources/gimpel/SPITBOL')
DB = os.environ.get('S4E_PROGRESS_DB', '/home/resources/progress/results.tsv')
if not os.path.isdir(V) or not os.path.isdir(SP) or not os.path.isfile(DB):
    print('REFUSE(2): cannot see the package %s, the Catspaw SPITBOL form %s or the progress DB %s' % (V, SP, DB)); sys.exit(2)
def norm(p):
    t = open(p, 'rb').read().replace(b'\r', b'').replace(b'\x1a', b'').decode('latin-1')
    return [l.rstrip() for l in t.rstrip('\n ').split('\n')]
bad = {'FORM': [], 'NOTHING': [], 'NAMES': [], 'KEYS': []}
form = sorted(f for f in os.listdir(SP) if not f.upper().startswith('README'))
if not form: print('REFUSE(2): the SPITBOL form directory is empty'); sys.exit(2)
vend = set(os.listdir(V))
for f in form:
    if f.lower() not in vend: bad['FORM'].append(f.lower() + ' absent'); continue
    if norm(os.path.join(SP, f)) != norm(os.path.join(V, f.lower())): bad['FORM'].append(f.lower() + ' differs')
local = set()
lt = os.path.join(V, 'LOCAL.tsv')
if os.path.isfile(lt):
    for l in open(lt, encoding='utf-8'):
        if l.strip() and not l.startswith('#'):
            n, _, why = l.rstrip('\n').partition('\t')
            if why.strip(): local.add(n)
            else: bad['NOTHING'].append(n + ' declared in LOCAL.tsv with no reason')
side = re.compile(r'^(ALL\..*|.*\.tsv|README\.md|PROVENANCE\.md|\.gitkeep)$')
drv = re.compile(r'^[A-Za-z0-9_]+_driver\.(sno|ref|input|in|IN|wantrc)$')
fset = {f.lower() for f in form}
for f in sorted(vend):
    if f in fset or f in local or side.match(f) or drv.match(f): continue
    bad['NOTHING'].append(f)
for f in sorted(vend):
    if side.match(f) or drv.match(f): continue
    if f.endswith('.sno') or re.search(r'\.[A-Z]+$', f): bad['NAMES'].append(f)
rows = []
with open(DB, 'rb') as fh:
    fh.seek(0, 2); fh.seek(max(0, fh.tell() - 60_000_000))
    for raw in fh:
        c = raw.decode('utf-8', 'replace').rstrip('\n').split('\t')
        if len(c) > 7 and c[5] == 'gimpel' and c[4] == 'package': rows.append(c)
if not rows: bad['KEYS'].append('no gimpel package row in the newest 60 MB of the progress DB -- run the gimpel pass')
else:
    last = max(r[0] for r in rows)[:13]
    keys = sorted({r[7] for r in rows if r[0][:13] == last})
    bad['KEYS'] += [k + ' (pass of ' + last + 'h)' for k in keys if k.endswith('.sno') and not k.endswith('_driver.sno')]
ok = True
for k in ('FORM', 'NOTHING', 'NAMES', 'KEYS'):
    if bad[k]:
        ok = False; print('RED %-7s %d: %s' % (k, len(bad[k]), ' '.join(bad[k][:12]) + (' ...' if len(bad[k]) > 12 else '')))
    else: print('ok  %s' % k)
print(('GREEN' if ok else 'RED') + ': gimpel is %sCatspaw\'s (Mark Emmer\'s) SPITBOL form, %d files of %s' % ('' if ok else 'NOT YET ', len(form), SP))
sys.exit(0 if ok else 1)
