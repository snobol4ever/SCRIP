#!/usr/bin/env python3
# util_unabsorbed_census.py -- WHICH OF OUR OWN SOURCES ARE NOT YET IN THE ONE-LINER / MULTI-LINER MASTER (ceo, 2026-09-04,
# on Lon's order: "For every source from every language that we've generated or that existed in corpus which is not
# part of any third-party package, if you can get it to run and it has output then add that to the test suite list.
# I.e. All our testing should be ONE-LINER and MULTI-LINER Python test suite. Oh yeah, CEO, have you ensured that all
# test sources have been moved into the ONE-LINER and MULTI-LINER?").
#   python3 scripts/util_unabsorbed_census.py [--lang L] [--list]
# Walks corpus/ minus packages/ (third-party is the package instrument's business) and minus programs/ (Lon 2026-09-04:
# "Go ahead and exclude programs/* folders." -- the 2026-08-27 parser-only ruling on corpus/programs stands). Every source by extension is one of:
# container (ALL.<ext>), module (include/, library/ -- no main, never absorbed alone), accounted (named in
# tests/<lang>/ALL.excluded.txt with a reason), loose pair (has a .ref/.expected/.std beside it), fixture
# (parser/coverage trees or parser_/probe_/coverage_ names), loose source with no ref. Prints the population per tree
# and language; rc=0 when no unaccounted runnable source remains for the languages asked, rc=1 naming what is owed,
# rc=2 when corpus/ cannot be read. A DONE-WHEN reads its rc, never a pinned count.
import argparse, collections, csv, os, re, sys
S4E = os.environ.get('S4E_HOME') or os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))
C = os.path.join(S4E, 'corpus')
EXT = {'sno': 'snobol4', 'sc': 'snocone', 'icn': 'icon', 'pl': 'prolog', 'reb': 'rebus', 'raku': 'raku', 'pas': 'pascal'}
ap = argparse.ArgumentParser(); ap.add_argument('--lang', default=''); ap.add_argument('--list', action='store_true'); A = ap.parse_args()
if not os.path.isdir(os.path.join(C, 'tests')): print('REFUSE(2): no corpus/tests under %s' % C); sys.exit(2)
excluded = collections.defaultdict(set)
additive_excluded = collections.defaultdict(set)   # {lang: {(name, category), ...}} from "name[category]" keys
for lang in set(EXT.values()):
    p = os.path.join(C, 'tests', lang, 'ALL.excluded.txt')
    if os.path.exists(p):
        for line in open(p, errors='replace'):
            line = line.strip()
            if not line or line.startswith('#'): continue
            key = line.split()[0]; excluded[lang].add(key)
            # ⛔⭐ ADDITIVE (demos/benchmarks) exclusions are keyed "name[category]" (util_build_master_suite.py's
            # additive_absorb, disambiguating e.g. a demo and a benchmark that share a basename) -- registered
            # as a (name, category) PAIR, never a bare name: a first attempt at this fix added the bare stripped
            # name to excluded[lang] globally, which MEASURED-WRONG immediately -- demos/snobol4/calculator/
            # calculator-1.sno being excluded[demos] falsely "accounted for" the UNRELATED
            # benchmarks/snobol4/demo/calculator-1.sno (same basename, never itself processed), because the two
            # trees carry genuinely identical basenames by design (the benchmark tree mirrors the demo one at a
            # different scale). The category qualifier is load-bearing, not decoration (hq_P seat08 2026-09-04).
            m = re.match(r'^(.+)\[([^\[\]]+)\]$', key)
            if m: additive_excluded[lang].add((m.group(1), m.group(2)))
# ⛔⭐ ADDITIVE ABSORPTION (demos/benchmarks) is invisible to plain base/path/fam matching: a successfully-absorbed
# source keeps living on disk (--additive never touches/deletes it) and is recorded ONLY as an `origin` in
# ALL.csv, shaped "<singular>_<lang>_<base>__<base>" (additive_absorb's own convention) -- never named in
# ALL.excluded.txt. Reconstruct that exact origin per source rather than matching bare basenames across trees:
# demos/snobol4/roman/roman.sno and benchmarks/snobol4/roman.sno produce DIFFERENT origins
# ("demo_snobol4_roman__roman" vs "benchmark_snobol4_roman__roman"), so a bare-basename match would wrongly
# cross-credit one tree's absorption to the other's still-unabsorbed file of the same name.
absorbed_origins = collections.defaultdict(set)
for lang in set(EXT.values()):
    p = os.path.join(C, 'tests', lang, 'ALL.csv')
    if os.path.exists(p):
        with open(p, newline='', errors='replace') as fh:
            for row in csv.DictReader(fh):
                o = row.get('origin')
                if o: absorbed_origins[lang].add(o)
def _additive_origin(top, lang, base):
    singular = top[:-1] if top.endswith('s') else top
    return '%s_%s_%s__%s' % (singular, lang, base, base)
rows = collections.defaultdict(collections.Counter); owed = collections.defaultdict(list)
for root, dirs, files in os.walk(C):
    rel = os.path.relpath(root, C)
    if rel.startswith('packages') or rel.startswith('programs') or '/.git' in root or rel.startswith('.git'): continue   # programs/* excluded on Lon's word 2026-09-04 ("Go ahead and exclude programs/* folders."): the 08-27 parser-only ruling on that tree stands
    for f in files:
        ext = f.rsplit('.', 1)[-1] if '.' in f else ''
        if ext not in EXT: continue
        lang = EXT[ext]
        if A.lang and lang != A.lang: continue
        path = os.path.normpath(os.path.join(rel, f)); top = path.split(os.sep)[0]; base = f[:-len(ext) - 1]
        # ⛔⭐ THE ACCOUNTING KEY THE BUILDER ACTUALLY WRITES IS THE FAMILY NAME, NOT THE BASENAME (hq_B 2026-09-04,
        # row icon-every-non-package-source-...). util_build_master_suite.py names an excluded source by its FAMILY --
        # the path under corpus/tests/<lang>/ with os.sep -> '_' and the extension dropped (discover_pairs: `fam =
        # rel[:-len(EXT)].replace(os.sep, "_")`) -- so a fixture at tests/icon/parser/alt_arith.icn is written as
        # `parser_alt_arith`. This census matched only basename / filename / corpus-relative path, none of which is that
        # string for ANY source in a subdirectory, so every properly-excluded subdirectory source read as OWED.
        # MEASURED at the fix: icon 219 -> 62 owed, and all 157 of the difference already carried a reason line the
        # builder wrote. ⭐ A top-level source was invisible to this bug because there family == basename, which is
        # exactly why it survived: the population that disproved it was the one the instrument never sampled.
        fam = os.path.splitext(path[len(os.path.join('tests', lang)) + 1:])[0].replace(os.sep, '_') if path.startswith(os.path.join('tests', lang) + os.sep) else ''
        if f.startswith('ALL.'): kind = 'container'
        elif top in ('include', 'library'): kind = 'module'
        elif base in excluded[lang] or path in excluded[lang] or f in excluded[lang] or (fam and fam in excluded[lang]): kind = 'accounted'
        elif top in ('demos', 'benchmarks') and ((base, top) in additive_excluded[lang] or _additive_origin(top, lang, base) in absorbed_origins[lang]): kind = 'accounted'
        elif any(os.path.exists(os.path.join(root, base + s)) for s in ('.ref', '.expected', '.std')): kind = 'loose pair (has ref)'; owed[lang].append(path)
        elif re.search(r'(^|/)(parser|coverage)/', path) or re.match(r'parser_|probe_|coverage_', f): kind = 'fixture'; owed[lang].append(path)
        else: kind = 'loose source (no ref)'; owed[lang].append(path)
        rows[(top, lang)][kind] += 1
print('%-12s %-8s %9s %6s %10s %10s %8s %12s' % ('tree', 'lang', 'container', 'module', 'accounted', 'loose-pair', 'fixture', 'loose-noref'))
tot = collections.Counter()
for (top, lang), c in sorted(rows.items()):
    print('%-12s %-8s %9d %6d %10d %10d %8d %12d' % (top, lang, c['container'], c['module'], c['accounted'], c['loose pair (has ref)'], c['fixture'], c['loose source (no ref)']))
    for k, v in c.items(): tot[k] += v
n = sum(len(v) for v in owed.values())
print('UNABSORBED_CENSUS%s: containers=%d modules=%d accounted=%d OWED=%d (loose pairs %d, fixtures %d, loose no-ref %d) -- an owed source is absorbed into its master with an oracle-cut ref, or named in ALL.excluded.txt with the reason it cannot run with output' % (' lang=' + A.lang if A.lang else '', tot['container'], tot['module'], tot['accounted'], n, tot['loose pair (has ref)'], tot['fixture'], tot['loose source (no ref)']))
for lang in sorted(owed): print('  %-8s owed %d' % (lang, len(owed[lang])))
if A.list:
    for lang in sorted(owed):
        for p in sorted(owed[lang]): print('    ' + p)
sys.exit(1 if n else 0)
