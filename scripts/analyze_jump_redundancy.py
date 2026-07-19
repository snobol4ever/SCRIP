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

def rsp_forwarder(body):
    # BP-9 BC-CENSUS: ζ-pop trampoline — body is one-or-more `add rsp, K` then one `jmp T`.
    # Returns (T, ΣK, n_adds) or None. These fold to ONE `add rsp, ΣK; jmp final` (op_sb static).
    real = [i for i in body if not i.startswith('#')]
    if len(real) < 2:
        return None
    sk = 0
    for ins in real[:-1]:
        m = re.match(r'^add\s+rsp\s*,\s*(\d+)\s*$', ins)
        if not m:
            return None
        sk += int(m.group(1))
    m = re.match(r'^jmp\s+([\w.$]+)\s*$', real[-1])
    if not m:
        return None
    return (m.group(1), sk, len(real) - 1)

def kind(lbl):
    if lbl.endswith('_β'): return 'β-resume'
    if lbl.endswith('_α'): return 'α-start'
    if lbl.endswith('_γ'): return 'γ-succeed'
    if lbl.endswith('_ω'): return 'ω-fail'
    if lbl.startswith('xcat') or lbl.startswith('xalt') or lbl.startswith('xgvarg') or lbl.startswith('xchain'): return 'combinator'
    return 'other'

def classify(path):
    labels, order = parse(path)
    fwd = {}      # bare: lbl -> target
    rfwd = {}     # ζ-pop: lbl -> (target, ΣK, n_adds)
    for lbl, body in labels.items():
        t = forwarder_target(body)
        if t is not None:
            fwd[lbl] = t
            continue
        r = rsp_forwarder(body)
        if r is not None:
            rfwd[lbl] = r
    total_branches = 0
    to_fwd = 0
    to_rfwd = 0
    chain_lens = []
    chain_sums = []
    targets = set()
    rkinds = {'β-resume':0,'α-start':0,'γ-succeed':0,'ω-fail':0,'combinator':0,'other':0}
    for lbl in rfwd:
        rkinds[kind(lbl)] += 1
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
            if tgt in fwd or tgt in rfwd:
                if tgt in rfwd: to_rfwd += 1
                else:           to_fwd += 1
                seen = set(); cur = tgt; n = 0; sk = 0
                while (cur in fwd or cur in rfwd) and cur not in seen and n < 4096:
                    seen.add(cur)
                    if cur in rfwd:
                        cur, k, _ = rfwd[cur]; sk += k
                    else:
                        cur = fwd[cur]
                    n += 1
                chain_lens.append(n)
                chain_sums.append(sk)
    dead_fwd = {'β-resume':0,'α-start':0,'γ-succeed':0,'ω-fail':0,'combinator':0,'other':0}
    dead_total = 0
    for lbl in list(fwd) + list(rfwd):
        if lbl not in targets:
            dead_total += 1
            dead_fwd[kind(lbl)] += 1
    return {
        'file': os.path.basename(path),
        'code_labels': len(labels),
        'fwd_labels': len(fwd),
        'rfwd_labels': len(rfwd),
        'branches': total_branches,
        'branch_to_fwd': to_fwd,
        'branch_to_rfwd': to_rfwd,
        'max_chain': max(chain_lens) if chain_lens else 0,
        'max_sum_k': max(chain_sums) if chain_sums else 0,
        'dead_fwd': dead_total,
        'dead_kinds': dead_fwd,
        'rfwd_kinds': rkinds,
    }

def main():
    pats = sys.argv[1:] or ['/home/claude/corpus/benchmarks/snobol4/*.s']
    files = []
    for p in pats:
        files += sorted(glob.glob(p))
    if not files:
        print('no .s files'); return
    tot = {'code_labels':0,'fwd_labels':0,'rfwd_labels':0,'branches':0,'branch_to_fwd':0,'branch_to_rfwd':0,'max_chain':0,'max_sum_k':0,'dead_fwd':0}
    dk = {'β-resume':0,'α-start':0,'γ-succeed':0,'ω-fail':0,'combinator':0,'other':0}
    rk = {'β-resume':0,'α-start':0,'γ-succeed':0,'ω-fail':0,'combinator':0,'other':0}
    print(f"{'file':28} {'labels':>7} {'fwd':>5} {'ζfwd':>5} {'br':>5} {'br→f':>5} {'br→ζ':>5} {'maxch':>6} {'maxΣK':>6} {'dead':>5}")
    print('-'*88)
    for f in files:
        r = classify(f)
        for k in tot:
            if k in ('max_chain','max_sum_k'): tot[k] = max(tot[k], r[k])
            else: tot[k] += r[k]
        for kk in dk: dk[kk] += r['dead_kinds'][kk]
        for kk in rk: rk[kk] += r['rfwd_kinds'][kk]
        print(f"{r['file']:28} {r['code_labels']:7} {r['fwd_labels']:5} {r['rfwd_labels']:5} {r['branches']:5} {r['branch_to_fwd']:5} {r['branch_to_rfwd']:5} {r['max_chain']:6} {r['max_sum_k']:6} {r['dead_fwd']:5}")
    print('-'*88)
    coll = tot['branch_to_fwd'] + tot['branch_to_rfwd']
    pct = (100.0*coll/tot['branches']) if tot['branches'] else 0.0
    print(f"{'TOTAL':28} {tot['code_labels']:7} {tot['fwd_labels']:5} {tot['rfwd_labels']:5} {tot['branches']:5} {tot['branch_to_fwd']:5} {tot['branch_to_rfwd']:5} {tot['max_chain']:6} {tot['max_sum_k']:6} {tot['dead_fwd']:5}")
    print(f"\ncollapsible branches (bare + ζ-pop forwarder targets): {coll}/{tot['branches']} = {pct:.1f}%  (bare {tot['branch_to_fwd']}, ζ-pop {tot['branch_to_rfwd']})")
    print(f"forwarder labels: bare {tot['fwd_labels']} + ζ-pop {tot['rfwd_labels']} of {tot['code_labels']} code labels")
    print(f"ζ-pop forwarders by wire: β={rk['β-resume']} α={rk['α-start']} γ={rk['γ-succeed']} ω={rk['ω-fail']} comb={rk['combinator']} other={rk['other']}")
    print(f"dead forwarders (never branched-to): {tot['dead_fwd']}")
    print(f"  by kind: β={dk['β-resume']} α={dk['α-start']} γ={dk['γ-succeed']} ω={dk['ω-fail']} comb={dk['combinator']} other={dk['other']}")

if __name__ == '__main__':
    main()
