#!/usr/bin/env python3
"""THE POPULATION MACHINE (CEO-1165 form), kept in the tree at the 36-to-zero landing (cto 2026-09-23): a master by name through
the harness reader and refs, on a control binary and a cure binary, m3 and m4, stress 0 and 1, arms run SEQUENTIALLY with a
fixed environment (a threaded first cut mis-assigned stress levels), every child under RLIMIT_AS 6 GB.
usage: util_gc_population_machine.py <snobol4|icon|prolog|snocone|raku> <control_root> <cure_root> [--names f] [--limit N] [--jobs J] [--exclude a,b]"""
import sys, os, subprocess, json, hashlib, re, collections, resource
resource.setrlimit(resource.RLIMIT_AS, (6 << 30, 6 << 30))
from concurrent.futures import ThreadPoolExecutor
sys.path.insert(0, '/home/claude_cto/SCRIP/scripts')
lang = sys.argv[1]; ctl = sys.argv[2]; cure = sys.argv[3]
names_f = None; limit = None; jobs = 6; exclude = set()
a = sys.argv[4:]
while a:
    k = a.pop(0)
    if k == '--names': names_f = a.pop(0)
    elif k == '--limit': limit = int(a.pop(0))
    elif k == '--jobs': jobs = int(a.pop(0))
    elif k == '--exclude': exclude |= set(a.pop(0).split(','))
ext = {'snobol4': '.sno', 'icon': '.icn', 'snocone': '.sc', 'prolog': '.pl', 'raku': '.raku'}[lang]
base = '/home/claude_cto/corpus/tests/%s/ALL' % lang
os.environ['SCRIP_HEAP_KB'] = '128'; os.environ['SCRIP_GC_RELOC'] = '1'; os.environ['SCRIP_GC_EXERCISE'] = '1'
os.environ.pop('SCRIP_HEAP_MB', None)
import corpus_suite_harness as H
sno, ref = base + ext, base + '.ref'
inp = base + '.in' if os.path.exists(base + '.in') else None
arg = base + '.argv' if os.path.exists(base + '.argv') else None
wrc = base + '.wantrc' if os.path.exists(base + '.wantrc') else None
mod = base + '.moderef' if os.path.exists(base + '.moderef') else None
if lang == 'snobol4':
    entries = H.read_suite(sno, ref, inp, None, wrc, arg, modes=None)
else:
    lc = H.LANG_CONFIGS[lang]
    entries = H.read_block_suite(sno, ref, H.banner_re_for(lc["comment_open"], lc["comment_close"]), in_path=inp, w_path=wrc, a_path=arg, modes=None)
if isinstance(entries, dict): entries = list(entries.values())
if names_f:
    want = set(l.strip() for l in open(names_f) if l.strip())
    entries = [e for e in entries if e.name in want]
if exclude: entries = [e for e in entries if e.name not in exclude]
if limit: entries = entries[:limit]
print('population %s: %d entries' % (lang, len(entries)), flush=True)
EX_RX = re.compile(r'collections=(\d+)\b.*?grew=(\d+)')
def run_one(root, entry, stress, mode):
    paths = H.resolve_paths()
    tmp = os.path.join('/tmp/claude-1000/-home-claude-cto/9e4a1ea5-4ea4-4b56-a505-8a0130c7e600/scratchpad', 'mach_tmp'); os.makedirs(tmp, exist_ok=True)
    v = H.run_suite_entry(paths, entry, tmp, [mode], ext=ext, companion_dir=os.path.dirname(sno))
    vd = v.get(mode) if isinstance(v, dict) else v
    kind = getattr(vd, 'kind', str(vd))
    out = getattr(vd, 'stdout', b'') or b''; err = (getattr(vd, 'stderr', b'') or b'').decode('utf-8', 'replace')
    m = EX_RX.search(err)
    return kind, hashlib.md5(out).hexdigest()[:8], (int(m.group(1)), int(m.group(2))) if m else None
arms = [(s, m) for s in (0, 1) for m in ('m3', 'm4')]
res = {}
def work(item):
    root, e, s, m = item
    try: return (root, e.name, s, m, run_one(root, e, s, m))
    except Exception as ex: return (root, e.name, s, m, ('ERROR', str(ex)[:60], None))
for root in (ctl, cure):
    for (st, md) in arms:
        os.environ['SCRIP'] = os.path.join(root, 'scrip'); os.environ['RT_DIR'] = os.path.join(root, 'out'); os.environ['SCRIP_GC_STRESS'] = str(st)
        def work(e):
            try: return (e.name, run_one(root, e, st, md))
            except Exception as ex: return (e.name, ('ERROR', str(ex)[:60], None))
        with ThreadPoolExecutor(max_workers=jobs) as ex:
            for (n, r) in ex.map(work, entries):
                res[(root, n, st, md)] = r
        print('arm done: %s stress=%d %s' % ('control' if root == ctl else 'cure', st, md), flush=True)
for (s, m) in arms:
    a = {n: res[(ctl, n, s, m)] for n in [e.name for e in entries]}
    b = {n: res[(cure, n, s, m)] for n in [e.name for e in entries]}
    ng_a = {n for n, r in a.items() if r[0] != 'PASS'}; ng_b = {n for n, r in b.items() if r[0] != 'PASS'}
    same_out = sum(1 for n in a if a[n][1] == b[n][1]); same_col = sum(1 for n in a if a[n][2] == b[n][2])
    print('ARM stress=%d %s: control PASS=%d nongreen=%d | cure PASS=%d nongreen=%d | identical-stdout=%d/%d identical-collections=%d/%d'
          % (s, m, len(a) - len(ng_a), len(ng_a), len(b) - len(ng_b), len(ng_b), same_out, len(a), same_col, len(a)), flush=True)
    only_cure = sorted(ng_b - ng_a); only_ctl = sorted(ng_a - ng_b)
    print('  NONGREEN ONLY ON CURE (%d): %s' % (len(only_cure), ' '.join('%s[%s]' % (n, b[n][0]) for n in only_cure[:20])))
    print('  NONGREEN ONLY ON CONTROL (%d): %s' % (len(only_ctl), ' '.join('%s[%s]' % (n, a[n][0]) for n in only_ctl[:20])))
    if s == 0:
        diff_out = sorted(n for n in a if a[n][1] != b[n][1])
        print('  STRESS-0 STDOUT DIFFERS (%d): %s' % (len(diff_out), ' '.join(diff_out[:20])))
        col_b = sum(1 for n in b if b[n][2] and b[n][2][0] > 0); col_a = sum(1 for n in a if a[n][2] and a[n][2][0] > 0)
        print('  STRESS-0 entries that collect: control=%d cure=%d' % (col_a, col_b))
json.dump({'%s|%s|%d|%s' % k: v for k, v in res.items()}, open(os.path.join('/tmp/claude-1000/-home-claude-cto/9e4a1ea5-4ea4-4b56-a505-8a0130c7e600/scratchpad', 'machine_%s.json' % lang), 'w'))
