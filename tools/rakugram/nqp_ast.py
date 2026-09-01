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
import nqp_cc

def look_operand(inner):
    """Resolve the text inside <?...> / <!...> to a matchable node, or None to REFUSE.

    ⛔ A lookahead we cannot translate must refuse. It must NEVER fall back to "always true":
    under a negative marker that inverts to "always fail", which is how 60 rules came to be unable
    to match anything at all while still looking translated.

    ⛔ THIS IS THE ONE RESOLVER. The emitter and the called-set walk in emit_all() both go through
    it, because a LOOK node stores its operand as TEXT rather than as child nodes -- so a walk that
    does not expand it here cannot see the subrules a lookahead calls. When only the emitter
    resolved them, `<?before <alpha>>` emitted a call to rk_alpha that no forward declaration
    covered: an IMPLICIT-DECLARATION WARNING, not an error, so the file still compiled and would
    have linked against nothing. That is the phantom-rule class the mkcall docstring already
    records; keeping one resolver is what stops it coming back.
    """
    t = inner.strip()
    t = t[1:].strip() if t[:1] in '?!' else t          # drop the ?/! marker
    if not t or t[0] == '{':
        return None                # <?{ code }> gates the match on NQP we cannot evaluate
    m = re.match(r'^(before|after)\b(.*)$', t, re.S)
    if m:
        if m.group(1) == 'after':
            return None            # backwards matching is not implemented
        try:
            return P(lex_body(m.group(2))).parse()
        except Exception:
            return None
    m = re.match(r'^([A-Za-z_][A-Za-z0-9_:-]*)\s*$', t)
    return N('CALL', m.group(1)) if m else None

def lowers(n):
    """Does node `n` actually lower to code, or will the emitter have to REFUSE?

    ⛔ THIS IS WHY THE LADDER MOVED. CCLASS/ESC/LOOK/ANCHOR/WB were counted as MECHANICAL
    unconditionally, while rung 3 emitted them as stubs that answered "matched" for any input --
    so the headline "% mechanical" counted 181 rules as translated that were in fact placeholders,
    60 of which could never match at all. A shape is only mechanical IF IT LOWERS; asking the same
    functions the emitter asks is the only thing that keeps the two numbers from drifting apart.
    """
    if n.k == 'CCLASS': return nqp_cc.parse_cclass(n.v) is not None
    if n.k == 'ESC':    return nqp_cc.parse_esc(n.v) is not None
    if n.k == 'LOOK':   return look_operand(n.v.split(':', 1)[1]) is not None
    return True


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
        """Resolve `<...>` assertion text to the SUBRULE NAME it calls.

        ⛔ Three argument spellings, and missing any one manufactures a phantom rule nobody can
        implement: `<foo(args)>` parenthesised, `<foo: args>` colon-form (used by every diagnostic --
        `<.typed_panic: 'X::Syntax::...'>`), and `<alias=.foo>` aliased. Measured: the colon form alone
        produced 40 phantom C functions such as
        rk_typed_panic___X__Syntax__Variable__IndirectDeclaration_ -- and each was an
        IMPLICIT-DECLARATION WARNING, not an error, so the file compiled and would have linked
        against nothing. A phantom that warns is worse than one that fails.
        """
        t = v.strip()
        am = re.match(r'([^(]*)\((.*)\)\s*$', t, re.S)
        if am: t = am.group(1)
        cm = re.match(r'([^:\s]+(?:::[^:\s]+)*)\s*:\s.*$', t, re.S)
        if cm: t = cm.group(1)
        if '=' in t: t = t.split('=', 1)[1]
        t = t.strip().lstrip('.')
        m = re.match(r'^([A-Za-z_][A-Za-z0-9_:-]*)', t)
        if not m:
            return N('UNSUPPORTED', f'assertion:{v[:40]}', ok=False)
        return N('CALL', m.group(1))

def classify(root):
    kinds = collections.Counter()
    stub = 0
    for n in root.walk():
        kinds[n.k] += 1
        # ⛔ A MECHANICAL-SHAPED NODE THAT DOES NOT LOWER IS NOT MECHANICAL. Counting the shape
        # alone is what let 181 rules read as translated while riding placeholder primitives.
        if not lowers(n):
            stub += 1
            kinds['~unlowered:' + n.k] += 1
    hard = sum(v for k, v in kinds.items() if k in SEMANTIC) + stub
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
            for k in kinds:
                if k.startswith('~unlowered:'): per_sem[k] += 1
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
