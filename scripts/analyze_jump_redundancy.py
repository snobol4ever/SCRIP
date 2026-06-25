#!/usr/bin/env python3
# Classify emitted-jump redundancy in SCRIP .s output.
# A "forwarder label" is a code label whose entire body (ignoring comments)
# is exactly one unconditional `jmp TARGET`. A branch (jmp/jCC) that targets a
# forwarder label is a jump-to-jump a peephole could collapse. Reports the
# collapsible population and the longest forwarder chain, per file and total.
import sys, re, glob, os

JCC = ('jmp','je','jne','jz','jnz','jg','jge','jl','jle','ja','jae','jb','jbe','js','jns')

def parse(path):
    section_code = True
    labels = {}          # label -> list of instruction strings
    order  = []          # label order
    cur = None
    for raw in open(path, encoding='utf-8', errors='replace'):
        line = raw.rstrip('\n')
        s = line.strip()
        if not s or s.startswith('#'):
            continue
        if s.startswith('.section') or s.startswith('.text') or s.startswith('.data') or s.startswith('.bss') or s.startswith('.rodata'):
            section_code = ('.text' in s) or (s == '.text')
            continue
        if s.startswith('.intel_syntax') or s.startswith('.global') or s.startswith('.globl') or s.startswith('.align') or s.startswith('.quad') or s.startswith('.string') or s.startswith('.space') or s.startswith('.byte'):
            continue
        m = re.match(r'^([A-Za-z_.$][\w.$]*)\s*:\s*$', s)
        if m:
            cur = m.group(1)
            if cur not in labels:
                labels[cur] = []
                order.append(cur)
            continue
        if not section_code:
            continue
        if cur is not None:
            labels[cur].append(s)
    return labels, order

def forwarder_target(body):
    # body is list of instruction strings; return TARGET if body is exactly one `jmp T`
    real = [i for i in body if not i.startswith('#')]
    if len(real) == 1:
        m = re.match(r'^jmp\s+([\w.$]+)\s*$', real[0])
        if m:
            return m.group(1)
    return None

def kind(lbl):
    if lbl.endswith('_β'): return 'β-resume'
    if lbl.endswith('_α'): return 'α-start'
    if lbl.startswith('xcat') or lbl.startswith('xalt') or lbl.startswith('xgvarg'): return 'combinator'
    return 'other'

def classify(path):
    labels, order = parse(path)
    fwd = {}
    for lbl, body in labels.items():
        t = forwarder_target(body)
        if t is not None:
            fwd[lbl] = t
    total_branches = 0
    to_fwd = 0
    chain_lens = []
    targets = set()
    for lbl, body in labels.items():
        for ins in body:
            mm = re.match(r'^(\w+)\s+([\w.$]+)\s*$', ins)
            if not mm:
                continue
            op, tgt = mm.group(1), mm.group(2)
            if op not in JCC:
                continue
            total_branches += 1
            targets.add(tgt)
            if tgt in fwd:
                to_fwd += 1
                seen = set(); cur = tgt; n = 0
                while cur in fwd and cur not in seen and n < 4096:
                    seen.add(cur); cur = fwd[cur]; n += 1
                chain_lens.append(n)
    dead_fwd = {'β-resume':0,'α-start':0,'combinator':0,'other':0}
    dead_total = 0
    for lbl in fwd:
        if lbl not in targets:
            dead_total += 1
            dead_fwd[kind(lbl)] += 1
    return {
        'file': os.path.basename(path),
        'code_labels': len(labels),
        'fwd_labels': len(fwd),
        'branches': total_branches,
        'branch_to_fwd': to_fwd,
        'max_chain': max(chain_lens) if chain_lens else 0,
        'dead_fwd': dead_total,
        'dead_kinds': dead_fwd,
    }

def main():
    pats = sys.argv[1:] or ['/home/claude/corpus/benchmarks/snobol4/*.s']
    files = []
    for p in pats:
        files += sorted(glob.glob(p))
    if not files:
        print('no .s files'); return
    tot = {'code_labels':0,'fwd_labels':0,'branches':0,'branch_to_fwd':0,'max_chain':0,'dead_fwd':0}
    dk = {'β-resume':0,'α-start':0,'combinator':0,'other':0}
    print(f"{'file':28} {'labels':>7} {'fwd':>5} {'br':>5} {'br→fwd':>7} {'maxch':>6} {'dead':>5}")
    print('-'*72)
    for f in files:
        r = classify(f)
        for k in tot:
            if k == 'max_chain': tot[k] = max(tot[k], r[k])
            else: tot[k] += r[k]
        for kk in dk: dk[kk] += r['dead_kinds'][kk]
        print(f"{r['file']:28} {r['code_labels']:7} {r['fwd_labels']:5} {r['branches']:5} {r['branch_to_fwd']:7} {r['max_chain']:6} {r['dead_fwd']:5}")
    print('-'*72)
    pct = (100.0*tot['branch_to_fwd']/tot['branches']) if tot['branches'] else 0.0
    print(f"{'TOTAL':28} {tot['code_labels']:7} {tot['fwd_labels']:5} {tot['branches']:5} {tot['branch_to_fwd']:7} {tot['max_chain']:6} {tot['dead_fwd']:5}")
    print(f"\ncollapsible branches (target a forwarder): {tot['branch_to_fwd']}/{tot['branches']} = {pct:.1f}%")
    print(f"forwarder labels: {tot['fwd_labels']}/{tot['code_labels']} code labels ({100.0*tot['fwd_labels']/tot['code_labels']:.0f}%)")
    print(f"dead forwarders (never branched-to): {tot['dead_fwd']}/{tot['fwd_labels']} forwarders")
    print(f"  by kind: β-resume={dk['β-resume']}  α-start={dk['α-start']}  combinator={dk['combinator']}  other={dk['other']}")

if __name__ == '__main__':
    main()
