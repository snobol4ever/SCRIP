#!/usr/bin/env python3
"""util_bb_placement_census.py -- CEO-1722: does every BB OPERAND (result) and every BB LOCAL live on the RSP SPINE?

THE WORD (Lon 2026-10-14, in-chat to the ceo): "You have a problem in that the RSP-spine is not being used for 90% of ALL BB
operands." and "And add BB's LOCALS to that list also."  THE GOAL: every BB operand and every BB local lives on the RSP spine at
a fixed compile-time offset unless the BB FRAME-PLACEMENT CRITERION forces it off -- an RBP activation frame only where unbounded
growth can intervene between a cell's write and its read (a gamma-suspend, a frame a choice point or a generator retains); a
static slot never for a per-activation cell.  A cell carved with the whole frame at entry and addressed off rsp is RSP by base
register but NOT on the spine (Lon 2026-09-07: "We do not pre-carve. Each BB carves it's own memory."; the cto's reading
2026-10-14), so it is its own home here and its own column, never folded into the spine.

HOMES come from the COMPILER'S OWN STATE, not from a base-register count of the .s: `scrip --dump-homes --compile` prints one
[HOMES] line per node and per local of every graph the flat chain body emits --
  SPINE       a depth-planner cell (the box carves it when it produces it; zd_on and K > 0)
  RSP_CARVED  a layout cell at a fixed rsp offset in a frame carved whole at entry
  RBP         a layout cell behind rbp (a pinned graph, a generator's rbp frame, an xop frame member)
  NONE        no cell (wiring, a K=0 protocol box, an elided result)
Every rip-relative or absolute memory operand in the .s is a runtime cell, a keyword, a GOT entry or a global variable's GVA
cell, never a BB operand's or local's home; the base-register view is printed beside the compiler view (BB_PLACEMENT_ASM) so
the ceo's measurement of 2026-10-14 reproduces next to the classification that answers it.

FORCING, per cell, over the box graph (`scrip --dump-bb`, the same analysis as util_rbp_placement_census.py):
  a RESULT cell P is FORCED off the spine iff the graph is RETAINED (an Icon generator; a Prolog predicate with more than one
  SUCCEED, whose frame a choice point keeps across its return -- the conservative bound of the rbp census, so determinate-by-
  indexing predicates read as retained and MISPLACED is a floor), or P itself retains, or for some reader O a retaining box
  (SUSPEND, PROC_GEN, CREATE, ACTIVATE, CORET, COFAIL, CALL_VALUE, a call whose callee retains) lies on a gamma path strictly
  between P and O;
  a LOCAL is FORCED iff the graph is a generator or a retaining box can run before a box that touches it (or is it).
STATES per cell: SPINE; RBP_FORCED; RBP_MISPLACED (behind rbp, the criterion admits the spine); CARVED_FORCED (printed, owed to
the rbp audit's UNSAFE question, never counted here); CARVED_MISPLACED (pre-carved, the planner could push it where first
used); NONE.  misplaced = RBP_MISPLACED + CARVED_MISPLACED, each printed apart so neither share hides behind the other.

OUTPUT (stdout; the row's DONE-WHEN greps ^BB_PLACEMENT):
  BB_PLACEMENT <lang> graphs=G operands=N locals=M spine=S rsp_carved=C rbp=R none=X rbp_forced=.. rbp_misplaced=.. carved_forced=.. carved_misplaced=.. misplaced=.. unemitted=E
  BB_PLACEMENT total operands=N locals=M spine=S rbp_misplaced=.. carved_misplaced=.. misplaced=..
  BB_PLACEMENT_ASM <lang> mem_operands=N rsp=.. rbp=.. rip=.. r12=.. r13=.. other=..
  BB_PLACEMENT_CLASS <lang> <state>:<cause> <A|L> <kind> count=..
  --tsv PATH writes one row per misplaced (lang state cause table graph@sig kind count).
rc 0 = measured; rc 2 = the instrument did not measure (a program unmeasured).
"""
import argparse, collections, hashlib, json, os, re, subprocess, sys, tempfile

HERE = os.path.dirname(os.path.abspath(__file__))
SC = os.path.dirname(HERE)
ROOT = os.environ.get('S4E_HOME') or os.path.dirname(SC)
SCRIP = os.path.join(SC, 'scrip')
CORPUS = os.path.join(ROOT, 'corpus')
EXT = {'.sno': 'snobol4', '.icn': 'icon', '.pl': 'prolog', '.sc': 'snocone', '.reb': 'rebus'}
RET_KINDS = {'SUSPEND', 'PROC_GEN', 'CREATE', 'ACTIVATE', 'CORET', 'COFAIL', 'CALL_VALUE'}
GEN_KINDS = {'SUSPEND', 'PROC_GEN', 'CREATE', 'ACTIVATE', 'CORET', 'COFAIL'}
CALL_KINDS = {'CALL_PROC_STAGED', 'CALL_ICON', 'CALL'}
LOCAL_KINDS = {'VAR', 'VAR_REF', 'ASSIGN_VAR', 'REV_ASSIGN_VAR', 'NULLTEST_VAR', 'FIELD_VAR', 'ASSIGN', 'UNIFY_FIRST', 'UNIFY_VALUE'}
HOMES = re.compile(r'^\[HOMES\] g=(-?\d+) graph=(\S+) (all|local)=(\S+) (?:kind=(\S+) )?home=(\S+) off=(-?\d+)$')
MEM = re.compile(r'\[\s*([a-z0-9]+)')


def run(args, cwd, timeout=300):
    return subprocess.run(args, cwd=cwd, stdin=subprocess.DEVNULL, stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=timeout)


def discover():
    out = []
    for top in ('benchmarks', 'demos'):
        for dp, _, fs in os.walk(os.path.join(CORPUS, top)):
            for f in fs:
                if not f.endswith('.s'):
                    continue
                stem = os.path.join(dp, f[:-2])
                for e, lang in EXT.items():
                    if os.path.isfile(stem + e):
                        out.append((lang, stem + e))
    return sorted(out, key=lambda x: (x[0], x[1]))


def homes_of(lang, src, tmp):
    cwd, base = os.path.dirname(src), os.path.basename(src)
    args = [SCRIP, '--dump-homes', '--compile'] + (['--target=x86'] if lang == 'prolog' else []) + ['-o', tmp, base]
    r = run(args, cwd)
    if r.returncode != 0 or not os.path.isfile(tmp) or os.path.getsize(tmp) == 0:
        return None, None
    nodes, locs = {}, {}
    for ln in r.stderr.decode('utf-8', 'replace').splitlines():
        m = HOMES.match(ln)
        if not m:
            continue
        gi = int(m.group(1))
        if m.group(3) == 'all':
            nodes[(gi, int(m.group(4)))] = (m.group(5), m.group(6), int(m.group(7)))
        else:
            locs[(gi, m.group(4))] = (m.group(6), int(m.group(7)))
    return nodes, locs


def asm_bases(path):
    c = collections.Counter()
    for ln in open(path, encoding='utf-8', errors='replace'):
        s = ln.split('#')[0]
        if not s.strip() or s.lstrip().startswith('.'):
            continue
        for m in MEM.finditer(s):
            b = m.group(1)
            c['rsp' if b == 'rsp' else 'rbp' if b == 'rbp' else 'rip' if b == 'rip' else 'r12' if b == 'r12' else 'r13' if b == 'r13' else 'other' if b[0] == 'r' else 'abs'] += 1
    return c


def zeta_names(src, lang):
    cwd, base = os.path.dirname(src), os.path.basename(src)
    names, vslots, cur = {}, collections.defaultdict(set), None
    r = run([SCRIP, '--dump-zeta'] + (['--target=x86'] if lang == 'prolog' else []) + [base], cwd)
    for ln in r.stdout.decode('utf-8', 'replace').splitlines():
        m = re.match(r"; graph (\d+) '(.*)' ", ln)
        if m:
            cur = int(m.group(1))
            names[cur] = m.group(2)
            continue
        m = re.match(r';\s+vslot \+\d+\s+\d+\s+\S+\s+(\S+)', ln)
        if m and cur is not None:
            vslots[cur].add(m.group(1))
    return names, vslots


def analyse(lang, src, tmp, seen):
    nodes, locs = homes_of(lang, src, tmp)
    if nodes is None:
        return None
    bases = asm_bases(tmp)
    names, vslots = zeta_names(src, lang)
    raw = run([SCRIP, '--dump-bb', os.path.basename(src)], os.path.dirname(src)).stdout
    dump = json.loads(raw.decode('utf-8', 'replace'))
    boxes = {b['id']: b for b in dump['boxes']}
    by = collections.defaultdict(list)
    for i in boxes:
        by[int(i.split('_')[0][1:])].append(i)
    gedges = collections.defaultdict(list)
    for e in dump['edges']:
        if e['from'] in boxes and e['to'] in boxes:
            gedges[int(e['from'].split('_')[0][1:])].append(e)
    gname = {g: names.get(g, 'g%d' % g) for g in by}
    gen_graph, susp_graph, multi, sigs = {}, {}, {}, {}
    for g, ids in by.items():
        ks = {boxes[i]['kind'] for i in ids}
        gen_graph[gname[g]] = bool(ks & GEN_KINDS)
        susp_graph[gname[g]] = 'SUSPEND' in ks
        multi[gname[g]] = sum(1 for i in ids if boxes[i]['kind'] == 'SUCCEED') > 1
        sigs[g] = hashlib.sha1(json.dumps([sorted((boxes[i]['kind'], boxes[i].get('label', '')) for i in ids),
                                           sorted((e['from'].split('_', 1)[1], e.get('fp', ''), e['to'].split('_', 1)[1], e.get('tp', ''), e.get('rel', ''), e.get('slot', ''))
                                                  for e in gedges[g])]).encode()).hexdigest()[:10]

    def retains(bid):
        b = boxes[bid]
        if b['kind'] in RET_KINDS:
            return True
        if b['kind'] in CALL_KINDS:
            lab = b.get('label', '')
            return susp_graph.get(lab, False) if lang == 'icon' else (multi.get(lab, False) if lang == 'prolog' else False)
        return False

    out = []
    for g in sorted(by):
        key = (lang, gname[g], sigs[g])
        if key in seen:
            continue
        seen.add(key)
        ids = by[g]
        emitted = any((g, int(i.split('_')[1])) in nodes for i in ids)
        rset = {i for i in ids if retains(i)}
        gen = gen_graph[gname[g]] or (lang == 'prolog' and multi[gname[g]])
        gam, rgam, allf, ops = collections.defaultdict(list), collections.defaultdict(list), collections.defaultdict(list), []
        for e in gedges[g]:
            if e.get('rel') == 'op':
                ops.append((e['from'], e['to']))
            elif e.get('fp') == 'gamma':
                allf[e['from']].append(e['to'])
                if e.get('tp') == 'alpha':
                    gam[e['from']].append(e['to'])
                    rgam[e['to']].append(e['from'])
        forced = {i: (gen or i in rset) for i in ids}
        for (o, p) in ops:
            if forced[p]:
                continue
            fwd, st = set(), [p]
            while st:
                x = st.pop()
                for y in gam[x]:
                    if y != o and y not in fwd:
                        fwd.add(y)
                        st.append(y)
            bwd, st = set(), [o]
            while st:
                x = st.pop()
                for y in rgam[x]:
                    if y != p and y not in bwd:
                        bwd.add(y)
                        st.append(y)
            if ((fwd & bwd) - {p, o}) & rset:
                forced[p] = True
        rall = collections.defaultdict(list)
        for x in ids:
            for y in allf[x]:
                rall[y].append(x)
        lforced = collections.defaultdict(lambda: gen)
        for i in ids:
            b = boxes[i]
            lab = b.get('label', '')
            if b['kind'] not in LOCAL_KINDS or lab not in vslots.get(g, ()):
                continue
            if gen or i in rset:
                lforced[lab] = True
                continue
            seen_b, st = set(), [i]
            while st:
                x = st.pop()
                for y in rall[x]:
                    if y not in seen_b:
                        seen_b.add(y)
                        st.append(y)
            if seen_b & rset:
                lforced[lab] = True
        kinds = [boxes[i]['kind'] for i in ids]
        if lang == 'icon':
            cause = 'icn-generator' if gen else ('icn-retaining-call' if rset else 'icn-plain')
        elif lang == 'prolog':
            cause = 'pl-choice-point-retained' if multi[gname[g]] else ('pl-determinate-retaining-callee' if rset else 'pl-determinate-no-retaining-callee')
        else:
            cause = lang + ('-generator' if gen else '-plain')
        cells = []
        for i in ids:
            h = nodes.get((g, int(i.split('_')[1])))
            if h is None:
                continue
            cells.append(('A', boxes[i]['kind'], h[1], forced[i]))
        for vn in sorted(vslots.get(g, ())):
            h = locs.get((g, vn))
            if h is None:
                continue
            cells.append(('L', vn, h[0], lforced[vn]))
        out.append({'name': gname[g], 'sig': sigs[g], 'cause': cause, 'emitted': emitted, 'cells': cells, 'n': len(ids)})
    return {'graphs': out, 'bases': bases}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--files', nargs='*', help='source files to measure instead of the committed-artifact set')
    ap.add_argument('--lang', choices=sorted(set(EXT.values())))
    ap.add_argument('--tsv')
    ap.add_argument('--verbose', action='store_true')
    a = ap.parse_args()
    if not os.access(SCRIP, os.X_OK):
        print('BB_PLACEMENT REFUSE: no ./scrip at ' + SCRIP)
        return 2
    progs = [(next((l for e, l in EXT.items() if f.endswith(e)), None), os.path.abspath(f)) for f in a.files] if a.files else discover()
    progs = [p for p in progs if p[0]]
    if a.lang:
        progs = [p for p in progs if p[0] == a.lang]
    langs = sorted({p[0] for p in progs})
    tot = {l: collections.Counter() for l in langs}
    bases = {l: collections.Counter() for l in langs}
    rows, seen, unmeasured = collections.Counter(), set(), []
    with tempfile.TemporaryDirectory() as td:
        tmp = os.path.join(td, 'p.s')
        for pi, (lang, src) in enumerate(progs):
            print('  [%d/%d] %s %s' % (pi + 1, len(progs), lang, os.path.relpath(src, CORPUS)), file=sys.stderr, flush=True)
            try:
                r = analyse(lang, src, tmp, seen)
            except Exception as ex:
                r = None
                if a.verbose:
                    print('  unmeasured %s: %s' % (src, ex), file=sys.stderr)
            if r is None:
                unmeasured.append(src)
                continue
            bases[lang].update(r['bases'])
            t = tot[lang]
            for gr in r['graphs']:
                t['graphs'] += 1
                if not gr['emitted']:
                    t['unemitted'] += gr['n']
                    continue
                for (tb, kind, home, forced) in gr['cells']:
                    t['operands' if tb == 'A' else 'locals'] += 1
                    if home == 'SPINE':
                        t['spine'] += 1
                    elif home == 'NONE':
                        t['none'] += 1
                    elif home == 'RBP':
                        t['rbp'] += 1
                        st = 'rbp_forced' if forced else 'rbp_misplaced'
                        t[st] += 1
                        if not forced:
                            rows[(lang, st, gr['cause'], tb, gr['name'] + '@' + gr['sig'], kind)] += 1
                    elif home == 'RSP_CARVED':
                        t['rsp_carved'] += 1
                        st = 'carved_forced' if forced else 'carved_misplaced'
                        t[st] += 1
                        if not forced:
                            rows[(lang, st, gr['cause'], tb, gr['name'] + '@' + gr['sig'], kind)] += 1
    for lang in langs:
        if tot[lang]['graphs'] == 0:
            unmeasured.append('<' + lang + ' measured nothing>')
    if unmeasured:
        print('BB_PLACEMENT REFUSE: %d program(s) unmeasured: %s' % (len(unmeasured), ' '.join(os.path.relpath(u, ROOT) if os.path.isabs(u) else u for u in unmeasured[:6])))
        return 2
    gt = collections.Counter()
    for lang in langs:
        t = tot[lang]
        t['misplaced'] = t['rbp_misplaced'] + t['carved_misplaced']
        print('BB_PLACEMENT %s graphs=%d operands=%d locals=%d spine=%d rsp_carved=%d rbp=%d none=%d rbp_forced=%d rbp_misplaced=%d carved_forced=%d carved_misplaced=%d misplaced=%d unemitted=%d' %
              (lang, t['graphs'], t['operands'], t['locals'], t['spine'], t['rsp_carved'], t['rbp'], t['none'], t['rbp_forced'], t['rbp_misplaced'], t['carved_forced'],
               t['carved_misplaced'], t['misplaced'], t['unemitted']))
        for k in ('operands', 'locals', 'spine', 'rbp_misplaced', 'carved_misplaced', 'misplaced'):
            gt[k] += t[k]
    print('BB_PLACEMENT total operands=%d locals=%d spine=%d rbp_misplaced=%d carved_misplaced=%d misplaced=%d' %
          (gt['operands'], gt['locals'], gt['spine'], gt['rbp_misplaced'], gt['carved_misplaced'], gt['misplaced']))
    for lang in langs:
        b = bases[lang]
        n = sum(b.values())
        print('BB_PLACEMENT_ASM %s mem_operands=%d rsp=%d rbp=%d rip=%d r12=%d r13=%d other=%d abs=%d' % (lang, n, b['rsp'], b['rbp'], b['rip'], b['r12'], b['r13'], b['other'], b['abs']))
    cls = collections.Counter()
    for k, n in rows.items():
        cls[(k[0], k[1] + ':' + k[2], k[3], k[5])] += n
    for (l, c, tb, kind), n in sorted(cls.items(), key=lambda x: -x[1])[:40]:
        print('BB_PLACEMENT_CLASS %s %s %s %s count=%d' % (l, c, tb, kind, n))
    if a.tsv:
        with open(a.tsv, 'w') as fh:
            for k in sorted(rows):
                fh.write('\t'.join(list(k) + [str(rows[k])]) + '\n')
    return 0


if __name__ == '__main__':
    sys.exit(main())
