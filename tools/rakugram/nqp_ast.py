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
    t = t[1:].strip() if t[:1] in '?!' else t          # drop the ?/! marker the LOOK node already carries
    flips = 0
    while t[:1] == '!':                                 # <!!{ … }>: each extra ! flips the polarity
        flips += 1; t = t[1:].strip()
    if not t:
        return None
    if t[0] == '{':
        dr = dynread_node(t)
        if dr is not None:
            for _ in range(flips): dr = N('NOT', None, [dr])
            return dr
        # ⛔ A code assertion GATES the match on NQP we cannot evaluate -- EXCEPT the one idiom whose
        # value is a literal: a block whose LAST statement is `1` (or `0`). `<!!{ $*LANG := …; 1 }>` is
        # Grammar.nqp's way of running code mid-rule and always succeeding (8 sites, on statementlist,
        # statement and blockoid). Anything whose truth depends on state stays refused (the guard).
        body = t[1:t.rindex('}')] if '}' in t else ''
        last = re.split(r';', body.strip().rstrip(';'))[-1].strip()
        if last == '1':   node = N('EMPTY')                        # constant TRUE
        elif last == '0': node = N('FAILN', 'constant false')      # constant FALSE
        else:             return None
        for _ in range(flips): node = N('NOT', None, [node])
        return node
    if flips:                                           # <!!rule> -- no such spelling in the grammar; refuse
        return None
    m = re.match(r'^(before|after)\b(.*)$', t, re.S)
    if m:
        if m.group(1) == 'after':
            return None            # backwards matching is not implemented
        try:
            return P(lex_body(m.group(2))).parse()
        except Exception:
            return None
    mk = mark_node(t)
    if mk is not None:
        return mk
    m = re.match(r'^([A-Za-z_][A-Za-z0-9_:-]*)\s*$', t)
    return N('CALL', m.group(1)) if m else None


# ---- parse-time dynamic variables (rung 9d) ----------------------------------------------------------
_CONST = re.compile(r"""^\s*(?:'([^']*)'|"([^"]*)"|(-?\d+))\s*;?\s*$""")
_MYDECL = re.compile(r':my\s+([\$@%&]\*[\w-]+)\s*(?::=|=)?\s*(.*?);?\s*$', re.S)
# ⛔ the VALUE stops at the next `;` OR at the next `<var> :=` -- a chained `$*LANG := $*LEAF := expr`
# is TWO writes, and a greedy `[^;]*` swallowed the second inside the first, leaving $*LEAF looking
# constant-written (its only visible write was `:my $*LEAF;`).
_ASSIGN = re.compile(r'([\$@%&]\*[\w-]+)\s*:=(?!=)\s*((?:(?![\$@%&]\*[\w-]+\s*:=)[^;])*)')
MODELLED_DYNVARS = set()      # populated by modelled_dynvars(decls) before emitting or classifying

def const_value(text):
    """The literal a write assigns, as the string NQP would see -- or None if it is not a literal."""
    t = (text or '').strip()
    if t == '': return ''                    # `:my $*X;` -- undefined, false
    m = _CONST.match(t)
    if not m: return None
    return next(g for g in m.groups() if g is not None)

def dynvar_writes(decls):
    """Every write to every $*var across the grammar: {name: [(kind, literal-or-None), …]}."""
    out = collections.defaultdict(list)
    for d in decls:
        for k, v in lex_body(d['body']):
            if k == 'MY':
                m = _MYDECL.match(v.strip())
                if m: out[m.group(1)].append(('my', const_value(m.group(2))))
            elif k in ('CODE', 'LOOKP', 'LOOKN'):
                # ⛔ writes inside code ASSERTIONS count too: `<!!{ $*LANG := $*LEAF := …; 1 }>` is a
                # non-literal write hiding in a constant-true assertion. Scanning only CODE tokens
                # listed $*LANG and $*LEAF as constant-written; they are not.
                for m in _ASSIGN.finditer(v):
                    rhs = m.group(2).strip()
                    # an ASSIGNMENT with an empty right-hand side is the head of a chain (`$*LANG := $*LEAF := …`):
                    # its value is whatever the chain ends in -- never a literal. Only a `:my` may be bare.
                    out[m.group(1)].append(('code', None if rhs == '' else const_value(rhs)))
    return out

def modelled_dynvars(decls):
    """The variables whose EVERY write is a literal constant -- the only ones a read may consult.
    ⛔ THE GUARD IN ONE LINE: a variable with any non-literal write ($*IN_DECL := $d, $*IN_META := $in_meta)
    is NOT modelled, its declarations stay no-ops, and every read of it keeps refusing."""
    w = dynvar_writes(decls)
    MODELLED_DYNVARS.clear()
    # SCALARS ONLY: a %*hash or @*array is written by element (`%*RX<i> := 1`, `nqp::push(@*nibbles, …)`)
    # in forms the assignment regex cannot see, so "every write is a literal" cannot be established for them.
    MODELLED_DYNVARS.update(n for n, ws in w.items() if n.startswith('$*') and ws and all(val is not None for _, val in ws))
    return set(MODELLED_DYNVARS)

_DYNREAD = re.compile(r'^\{\s*(!?)\s*([\$@%&]\*[\w-]+)\s*\}$')
def dynread_node(t):
    """`{ $*X }` / `{ !$*X }` inside <?…>/<!…> -> N('DYNREAD', (name, negated)) IF X is modelled, else None."""
    m = _DYNREAD.match(t.strip())
    if not m or m.group(2) not in MODELLED_DYNVARS: return None
    return N('DYNREAD', (m.group(2), m.group(1) == '!'))

_MARK = re.compile(r"^\.?\s*(MARKER|MARKED)\s*\(\s*'([A-Za-z_][A-Za-z0-9_]*)'\s*\)\s*$")
def mark_node(t):
    """`MARKER('n')` / `MARKED('n')` -> N('MARK', ('set'|'test', 'n')). NQP's per-name position memo:
    MARKER records the cursor under n and always succeeds (zero-width); MARKED succeeds iff the recorded
    position equals the current one. The generic assertion resolver drops call ARGUMENTS, which is how
    these became two argument-less inherited stubs that refused; the name is the whole point."""
    m = _MARK.match(t.strip())
    return N('MARK', ('set' if m.group(1) == 'MARKER' else 'test', m.group(2))) if m else None

def lowers(n):
    """Does node `n` actually lower to code, or will the emitter have to REFUSE?

    ⛔ THIS IS WHY THE LADDER MOVED. CCLASS/ESC/LOOK/ANCHOR/WB were counted as MECHANICAL
    unconditionally, while rung 3 emitted them as stubs that answered "matched" for any input --
    so the headline "% mechanical" counted 181 rules as translated that were in fact placeholders,
    60 of which could never match at all. A shape is only mechanical IF IT LOWERS; asking the same
    functions the emitter asks is the only thing that keeps the two numbers from drifting apart.
    """
    if n.k == 'MY':     return True            # parse-time local: consumes nothing, cannot fail (rung 8)
    if n.k == 'CODE':   return True            # action block: cannot fail a match; panic-class fails the arm
    if n.k == 'MOD':    return str(n.v).lstrip().startswith(':dba')   # a diagnostic LABEL; :i/:s change matching
    if n.k == 'CCLASS': return nqp_cc.parse_cclass(n.v) is not None
    if n.k == 'ESC':    return nqp_cc.parse_esc(n.v) is not None
    if n.k == 'LOOK':   return look_operand(n.v.split(':', 1)[1]) is not None
    return True


MECHANICAL = {'SEQ','ALT_LTM','ALT_ORD','CALL','LIT','CCLASS','QUANT','LOOK','CAP','ANCHOR','ESC','WB','EMPTY','FAILN','NOT','MARK','DYNREAD',
              'MY','CODE','MOD'}    # MY/CODE/MOD are mechanical only when lowers() says so (rung 8)
# Constructs that need a hand-written runtime helper, but whose SHAPE is still mechanical.
RUNTIME    = {'GOAL','SEP','CONJ'}
# Constructs that carry NQP semantics we cannot translate by shape alone.
# ⛔ RUNG 8 MOVED MY/CODE OUT OF HERE, AND THE MOVE IS ONLY SOUND BECAUSE OF THE CONSULTATION GUARD:
#    a `:my $*X := …` declaration and a `{ $*X := … }` assignment consume no input and cannot fail, so for
#    MATCHING they are no-ops -- but their VALUE is un-modelled. Every READ of a parse-time variable
#    (`<?{ $*X }>` via look_operand, `$*X` interpolation via VAR, `<.stopper>`/goal via GOAL-with-dynvar)
#    MUST keep refusing until the variable is modelled, and modelling one means implementing every WRITE
#    as well as every read -- a no-op write to a modelled variable is a silent wrong answer.
SEMANTIC   = {'VAR','UNSUPPORTED'}

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
        they are DIFFERENT operators and must not be merged into one node kind -- and `||` binds
        LOOSER than `|`: `A | B || C` is `(A | B) || C` (S05). Rung 8 flattened a level that mixed them
        into one ALT_MIXED node that refused; measured, that was 20 rules including the whole spine
        (statement, pblock, blockoid, termish, _ws). Rung 9 parses the precedence instead."""
        # a leading | or || is legal and just introduces the first arm
        if self.peek()[0] in ('ALT_LTM', 'ALT_ORD'): self.next()
        groups = [[self.seq()]]            # groups are joined by ||; the arms inside a group by |
        while self.peek()[0] in ('ALT_LTM', 'ALT_ORD'):
            k = self.next()[0]
            arm = self.seq()
            if k == 'ALT_ORD': groups.append([arm])
            else: groups[-1].append(arm)
        def ltm(arms): return arms[0] if len(arms) == 1 else N('ALT_LTM', None, arms)
        nodes = [ltm(g) for g in groups]
        return nodes[0] if len(nodes) == 1 else N('ALT_ORD', None, nodes)

    def seq(self):
        kids = []
        while not self.at_end():
            k, v = self.peek()
            if k in ('ALT_LTM', 'ALT_ORD') or k == 'CLOSE': break
            a = self.atom()
            if a is not None: kids.append(a)
        kids = self.goal_rewrite(kids)
        if not kids: return N('EMPTY')
        return kids[0] if len(kids) == 1 else N('SEQ', None, kids)

    def goal_rewrite(self, kids):
        """`A ~ B C…`  ==>  `A C… B`   (the S05 goal operator; rung 8).

        The opener A is the atom before `~`, the closer B the atom after it, and C is THE REST OF THE
        SEQUENCE -- measured over Grammar.nqp's 45 sites: C is 0..3 atoms and never a separate group
        (`'(' ~ ')' <EXPR>` is the common shape). Matching-wise the rewrite is exact: what `~` adds is
        the diagnostic on a missing closer, which is an error MESSAGE, not a parse decision. A `~` with
        no opener or no closer is malformed and becomes UNSUPPORTED so the rule refuses.
        """
        out, i = [], 0
        while i < len(kids):
            k = kids[i]
            if k.k != 'GOAL':
                out.append(k); i += 1; continue
            if not out or i + 1 >= len(kids):
                out.append(N('UNSUPPORTED', 'goal~ without opener/closer', ok=False)); i += 1; continue
            A, B, C = out.pop(), kids[i + 1], list(kids[i + 2:])
            out.append(A); out.extend(self.goal_rewrite(C)); out.append(B)
            return out
        return out

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
        elif k == 'CODE':     node = N('CODE', v, ok=False)
        elif k == 'MY':       node = N('MY', v, ok=False)
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
        mk = mark_node(t)
        if mk is not None: return mk
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
    md = modelled_dynvars(decls)
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
    print(f"modelled parse-time variables (every write a literal): {len(md)} -> {sorted(md)}")
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

SELFTEST = [   # body text -> expected (kind, value) sequence after the goal rewrite
    ("'(' ~ ')' <expr> <foo>", [('LIT','('), ('CALL','expr'), ('CALL','foo'), ('LIT',')')]),
    ("'[' ~ ']' <arglist>",    [('LIT','['), ('CALL','arglist'), ('LIT',']')]),
    ("'{' ~ '}'",              [('LIT','{'), ('LIT','}')]),
    ("~ ')' <expr>",           [('UNSUPPORTED',None), ('LIT',')'), ('CALL','expr')]),   # no opener: refuses
]
SELFTEST_LOOK = [   # inner text of <…> -> (kind of resolved node, polarity flips) or None
    ("!!{ $*LANG := $*LEAF := $/.clone_braid_from(self); 1 }", ('NOT', 'EMPTY')),   # const true, one flip
    ("?{ $/.set_actions($actions); 1 }",                      ('EMPTY', None)),
    ("!{ $*QSIGIL }",                                         None),                # STATE: stays refused (guard)
    ("?{ $*IN_DECL }",                                        None),
    ("!{ 0 }",                                                ('FAILN', None)),
    ("?before 'x'",                                           ('LIT', None)),
]
SELFTEST_MARK = [
    ("?MARKED('endstmt')", ('MARK', ('test', 'endstmt'))),
    ("?MARKER('endstmt')", ('MARK', ('set', 'endstmt'))),
    (".MARKER('ws')",      ('MARK', ('set', 'ws'))),
    ("MARKED",             None),          # no name: the name is the whole point -> refuse
]
SELFTEST_ALT = [   # body -> shape, as (kind, [arm kinds]) ; `||` binds looser than `|`
    ("<a> | <b> || <c>",   ('ALT_ORD', ['ALT_LTM', 'CALL'])),
    ("<a> || <b> | <c>",   ('ALT_ORD', ['CALL', 'ALT_LTM'])),
    ("<a> | <b>",          ('ALT_LTM', ['CALL', 'CALL'])),
    ("<a> || <b>",         ('ALT_ORD', ['CALL', 'CALL'])),
    ("<a> | <b> | <c> || <d> || <e> | <f>", ('ALT_ORD', ['ALT_LTM', 'CALL', 'ALT_LTM'])),
]

if __name__ == '__main__':
    if sys.argv[1:] == ['--selftest']:
        bad = 0
        for body, want in SELFTEST:
            root = P(lex_body(body)).parse()
            kids = root.kids if root.k == 'SEQ' else [root]
            got = [(k.k, (k.v if k.k != 'UNSUPPORTED' else None)) for k in kids]
            ok = got == want; bad += not ok
            print(f"  {'ok  ' if ok else 'FAIL'} {body!r:28} -> {got}")
        for inner, want in SELFTEST_LOOK:
            n = look_operand(inner)
            got = None if n is None else (n.k, n.kids[0].k if n.kids else None)
            ok = got == want; bad += not ok
            print(f"  {'ok  ' if ok else 'FAIL'} <{inner[:38]!s:38}> -> {got}")
        MODELLED_DYNVARS.update({'$*QSIGIL'})          # selftest fixture: QSIGIL modelled, IN_DECL not
        chain = [(m.group(1), m.group(2).strip()) for m in _ASSIGN.finditer("$*LANG := $*LEAF := $/.clone_braid_from(self); 1")]
        ok = chain == [('$*LANG', ''), ('$*LEAF', '$/.clone_braid_from(self)')]
        bad += not ok; print(f"  {'ok  ' if ok else 'FAIL'} chained assignment is TWO writes -> {chain}")
        w = dynvar_writes([{'body': "<!!{ $*LANG := $*LEAF := $/.clone_braid_from(self); 1 }>", 'name': 'x'}])
        ok = w['$*LANG'] == [('code', None)] and w['$*LEAF'] == [('code', None)]
        bad += not ok; print(f"  {'ok  ' if ok else 'FAIL'} …and NEITHER counts as a literal write (chain head is not bare) -> {dict(w)}")
        for inner, want in [("!{ $*QSIGIL }", ('DYNREAD', ('$*QSIGIL', False))), ("?{ !$*QSIGIL }", ('DYNREAD', ('$*QSIGIL', True))), ("?{ $*IN_DECL }", None)]:
            n = look_operand(inner); got = None if n is None else (n.k, n.v); ok = got == want; bad += not ok
            print(f"  {'ok  ' if ok else 'FAIL'} <{inner:20}> -> {got}   {'(guard: not modelled -> refuses)' if want is None else ''}")
        for inner, want in SELFTEST_MARK:
            n = look_operand(inner) if inner[:1] in '?!' else mark_node(inner)
            got = None if n is None else (n.k, n.v)
            ok = got == want; bad += not ok
            print(f"  {'ok  ' if ok else 'FAIL'} <{inner:20}> -> {got}")
        for body, (kind, arms) in SELFTEST_ALT:
            root = P(lex_body(body)).parse()
            got = (root.k, [k.k for k in root.kids])
            ok = got == (kind, arms); bad += not ok
            print(f"  {'ok  ' if ok else 'FAIL'} {body!r:40} -> {got}")
        print('goal rewrite + alternation precedence agree' if not bad else f'{bad} MISMATCH(ES)'); sys.exit(1 if bad else 0)
    main(*sys.argv[1:])
