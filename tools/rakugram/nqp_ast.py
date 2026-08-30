#!/usr/bin/env python3
"""nqp_ast.py -- parse Grammar.nqp production bodies into a structured AST, and report how much
of the official grammar is MECHANICALLY translatable to recursive descent.

RUNG 2 of the port (Lon ruling 2026-08-30, see .github/GOAL-RAKU-100.md). Rung 1 (nqp_read.py)
answered "what constructs appear". This answers "does each body actually PARSE into a tree we could
emit from", which is the question that decides whether the port is mechanical or a rewrite.

⛔ Every node records whether it is EMITTABLE. A construct we cannot translate is kept in the tree as
an explicit UNSUPPORTED node carrying its source text -- never dropped. A translator that silently
skips what it does not understand produces a parser that is confidently wrong, which is strictly
worse than one that refuses.
"""
import sys, re, collections, json
sys.path.insert(0, __import__('os').path.dirname(__file__))
from nqp_read import scan_decls, lex_body

class N:
    __slots__ = ('k', 'v', 'kids', 'ok')
    def __init__(self, k, v=None, kids=None, ok=True):
        self.k, self.v, self.kids, self.ok = k, v, kids or [], ok
    def walk(self):
        yield self
        for c in self.kids: yield from c.walk()
    def __repr__(self): return f"{self.k}({self.v!r}{',' + repr(self.kids) if self.kids else ''})"

# Constructs that a recursive-descent emitter handles directly.
MECHANICAL = {'SEQ','ALT_LTM','ALT_ORD','CALL','LIT','CCLASS','QUANT','LOOK','CAP','ANCHOR','ESC','WB','EMPTY'}
# Constructs that need a hand-written runtime helper, but whose SHAPE is still mechanical.
RUNTIME    = {'GOAL','SEP','CONJ'}
# Constructs that carry NQP semantics we cannot translate by shape alone.
SEMANTIC   = {'CODE','MY','MOD','VAR','UNSUPPORTED'}

class P:
    def __init__(self, toks): self.t, self.i = toks, 0
    def peek(self, o=0): return self.t[self.i+o] if self.i+o < len(self.t) else (None, None)
    def next(self):
        v = self.peek(); self.i += 1; return v
    def at_end(self): return self.i >= len(self.t)

    def parse(self):
        n = self.alt()
        return n

    def alt(self):
        """Alternation is LOWEST precedence. ⛔ '|' is LONGEST-TOKEN-MATCH and '||' is FIRST-MATCH;
        they are DIFFERENT operators and must not be merged into one node kind."""
        # a leading | or || is legal and just introduces the first arm
        kind = None
        if self.peek()[0] in ('ALT_LTM', 'ALT_ORD'):
            kind = self.peek()[0]; self.next()
        arms = [self.seq()]
        while self.peek()[0] in ('ALT_LTM', 'ALT_ORD'):
            k = self.next()[0]
            if kind is None: kind = k
            elif kind != k:
                # mixing | and || at one level is meaningful in Raku; keep it visible
                kind = 'ALT_MIXED'
            arms.append(self.seq())
        if len(arms) == 1 and kind is None: return arms[0]
        return N(kind or 'ALT_LTM', None, arms, ok=(kind != 'ALT_MIXED'))

    def seq(self):
        kids = []
        while not self.at_end():
            k, v = self.peek()
            if k in ('ALT_LTM', 'ALT_ORD') or k == 'CLOSE': break
            a = self.atom()
            if a is not None: kids.append(a)
        if not kids: return N('EMPTY')
        return kids[0] if len(kids) == 1 else N('SEQ', None, kids)

    def atom(self):
        k, v = self.next()
        node = None
        if   k == 'OPEN':
            inner = self.alt()
            if self.peek()[0] == 'CLOSE': self.next()
            node = N('CAP', None, [inner]) if v == '(' else inner
        elif k == 'LIT':      node = N('LIT', v)
        elif k == 'CALL':     node = self.mkcall(v, True)
        elif k == 'CALL_NC':  node = self.mkcall(v, False)
        elif k == 'CCLASS':   node = N('CCLASS', v)
        elif k in ('LOOKP','LOOKN'):
            node = N('LOOK', ('pos' if k == 'LOOKP' else 'neg') + ':' + v)
        elif k == 'NCAP':
            sub = self.atom()
            node = N('CAP', v, [sub] if sub else [])
        elif k == 'ANCHOR':   node = N('ANCHOR', v)
        elif k == 'ESC':      node = N('ESC', v)
        elif k == 'WB':       node = N('WB', v)
        elif k == 'CODE':     node = N('CODE', v[:60], ok=False)
        elif k == 'MY':       node = N('MY', v[:60], ok=False)
        elif k == 'MOD':      node = N('MOD', v, ok=False)
        elif k == 'VAR':      node = N('VAR', v, ok=False)
        elif k == 'GOAL':     node = N('GOAL', v)
        elif k == 'CONJ':     node = N('CONJ', v)
        elif k == 'BARE':     node = N('LIT', v)      # bare word in a regex is a literal match
        elif k == 'PROTOBODY':node = N('PROTOBODY', v)
        elif k is None:       return None
        else:                 node = N('UNSUPPORTED', f"{k}:{v}", ok=False)
        # postfix quantifier / separator
        while self.peek()[0] in ('QUANT', 'SEP'):
            qk, qv = self.next()
            if qk == 'QUANT': node = N('QUANT', qv, [node])
            else:
                sepa = self.atom()
                node = N('SEP', qv, [node] + ([sepa] if sepa else []))
        return node

    def mkcall(self, v, capture):
        m = re.match(r'([\w:<>«»-]+)?\s*(=)?\s*(\.)?([\w:<>«»-]+)?\s*(\((.*)\))?$', v, re.S)
        name, args = v, None
        am = re.match(r'([^(]*)\((.*)\)\s*$', v, re.S)
        if am: name, args = am.group(1), am.group(2)
        if '=' in name:
            alias, name = name.split('=', 1)
        name = name.lstrip('.')
        return N('CALL', name.strip(), [], ok=True) if not args else N('CALL', name.strip() + '(...)', [], ok=True)

def classify(root):
    kinds = collections.Counter()
    for n in root.walk(): kinds[n.k] += 1
    hard = sum(v for k, v in kinds.items() if k in SEMANTIC)
    rt   = sum(v for k, v in kinds.items() if k in RUNTIME)
    return kinds, hard, rt

def main(path='/home/resources/rakudo-main/src/Perl6/Grammar.nqp'):
    decls = scan_decls(open(path, encoding='utf-8', errors='replace').read())
    tot = collections.Counter(); nfull = nrt = nsem = 0
    per_sem = collections.Counter()
    rows = []
    for d in decls:
        if d.get('overrun'): continue
        root = P(lex_body(d['body'])).parse()
        kinds, hard, rt = classify(root)
        tot.update(kinds)
        if hard == 0 and rt == 0: nfull += 1
        elif hard == 0:           nrt += 1
        else:
            nsem += 1
            for k in SEMANTIC:
                if kinds.get(k): per_sem[k] += 1
        rows.append((d['name'], d['sym'], hard, rt))
    n = len(rows)
    print(f"productions parsed into an AST : {n}  (of {len(decls)}; {len(decls)-n} skipped as contaminated)")
    print()
    print("TRANSLATABILITY -- the number that decides whether this port is mechanical:")
    print(f"  FULLY MECHANICAL (emit directly)          : {nfull}  ({100.0*nfull/n:.1f}%)")
    print(f"  + needs a runtime helper (goal/sep/conj)  : {nrt}  ({100.0*nrt/n:.1f}%)")
    print(f"  = shape-translatable subtotal             : {nfull+nrt}  ({100.0*(nfull+nrt)/n:.1f}%)")
    print(f"  carries NQP SEMANTICS (code/:my/vars)     : {nsem}  ({100.0*nsem/n:.1f}%)")
    print()
    print("  of those, which semantic construct blocks them:")
    for k, v in per_sem.most_common(): print(f"    {k:<12} {v}")
    print()
    print("AST NODE HISTOGRAM:")
    for k, v in tot.most_common(): print(f"  {k:<12} {v}")
    print()
    print(f"⛔ LTM vs ordered alternation: ALT_LTM={tot.get('ALT_LTM',0)}  ALT_ORD={tot.get('ALT_ORD',0)}"
          f"  ALT_MIXED={tot.get('ALT_MIXED',0)}")
    print("   '|' is longest-token-match, '||' is first-match. Emitting '|' as ordered choice is the")
    print("   idiomatic RD reading and is WRONG -- right on most input, silently wrong where a later")
    print("   alternative matches longer.")

if __name__ == '__main__':
    main(*sys.argv[1:])
