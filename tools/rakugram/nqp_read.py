#!/usr/bin/env python3
"""nqp_read.py -- read Rakudo's Perl6/Grammar.nqp into a structured production list.

RUNG 1 of the Grammar.nqp -> recursive-descent port (Lon ruling 2026-08-30: Raku is not LALR,
bison is the wrong instrument -- see .github/GOAL-RAKU-100.md).

This is the READER + CENSUS half only. It deliberately does NOT emit code yet: the point of this
rung is to answer, by measurement rather than prediction, HOW MUCH of the official grammar is
mechanically translatable -- so the port is costed before it is built.

It parses the .nqp regex sublanguage into a small AST. Anything it does not understand is recorded
as UNKNOWN with its source text, never silently skipped -- an unparsed construct that vanishes is
the same defect class as a test that skips-as-success.
"""
import re, sys, json, collections

# ---------------------------------------------------------------------------------------------
# Declaration scanner: pull `[proto] token|rule|regex NAME[:sym<X>] { BODY }` with brace matching.
# ---------------------------------------------------------------------------------------------
# ⛔ A DECLARATION MAY CARRY A PARAMETER LIST, and missing that drops it SILENTLY -- the name simply
# fails to match and the production is never scanned at all. Measured 2026-08-30: 56 of 743
# declarations are parameterized, and they are the grammar's SPINE -- statementlist, statement,
# xblock, babble, quibble. With them missing, a call-graph walk from comp_unit reached 12 rules and
# reported 181 "unreachable" definitions, which reads as a fact about the grammar rather than a bug
# in the reader. ⭐ A DROPPED DECLARATION DOES NOT LOOK LIKE AN ERROR, IT LOOKS LIKE A SMALLER
# GRAMMAR. Parameters may nest one level (`token foo($x, @y?)`).
# Two ordered alternatives, and the ORDER matters: try `name {` first, because an operator name may
# itself contain parens (`token infix:sym<(&)>`), and a parameter-first pattern swallows those.
DECL = re.compile(r'^(?P<ind>\s*)(?P<proto>proto\s+)?(?P<kind>token|rule|regex)\s+'
                  r'(?:(?P<name>[^\s{]+)\s*\{'
                  r'|(?P<pname>[^\s({]+)\s*(?P<params>\((?:[^()]|\([^()]*\))*\))\s*\{)')

def _match_brace(src, start):
    r"""Brace-match from src[start]=='{' to its closer. Returns index of the closing '}' or -1.

    ⛔ Naive matching fails SILENTLY on this file by overrunning into the next production. Measured
    causes, all from real Grammar.nqp bodies:
      token apostrophe { <[ ' \- ]> }        a quote INSIDE a character class opened a phantom
                                             string that ran to the next apostrophe far below
      <.[ \s \# ]>                           a '#' inside a character class opened a phantom comment
    So quotes and '#' are inert while inside a <[...]> character class, and '\' always escapes.
    The failure mode is what makes this worth the care: the production COUNT stays plausible while
    individual bodies quietly absorb their successors, so nothing in the totals looks wrong.
    """
    i, n = start, len(src)
    depth = 0; quote = None; cclass = 0
    while i < n:
        c = src[i]
        if c == '\\': i += 2; continue
        if quote:
            if c == quote: quote = None
            i += 1; continue
        if cclass:
            if c == ']': cclass -= 1
            elif c == '[': cclass += 1
            i += 1; continue
        if re.match(r'<[.+?!\-]?\[', src[i:i+4]):  # <[ <.[ <-[ <+[ <?[ <![ are ALL char classes.
            cclass = 1; i += src.index('[', i) - i + 1; continue
        if c == '«':                       # <<...>> operator name: Rakudo writes infix:sym«<»,
            j = src.find('»', i)           # whose CONTENT is literal < and >. Inert, or the
            i = (j + 1) if j >= 0 else i + 1    # angle brackets corrupt every count downstream.
            continue
        if c == '#':
            while i < n and src[i] != '\n': i += 1
            continue
        if c in '\'"': quote = c; i += 1; continue
        if c == '{': depth += 1
        elif c == '}':
            depth -= 1
            if depth == 0: return i
        i += 1
    return -1

def scan_decls(src):
    """Find each declaration and its body, by brace matching, CROSS-CHECKED against the
    indentation-anchored closing line. Disagreements are reported, never silently resolved."""
    lines = src.split('\n')
    offs, t = [], 0
    for line in lines: offs.append(t); t += len(line) + 1
    starts = []
    for ln, line in enumerate(lines):
        m = DECL.match(line)
        if m: starts.append((ln, m))
    # ⛔ EVERY DECLARATION HAS AN OWNER. Grammar.nqp holds FOUR grammars (Perl6::Grammar 625 decls, QGrammar 75,
    # RegexGrammar 17, P5RegexGrammar 5) and the quote-language roles (startstops/startstop/stop) plus role STD,
    # which Perl6::Grammar `does`. `stopper` is declared FIVE times across them; flattening the file made the
    # quote role's `$stop1 | $stop2` win over STD's `<!>` for the main language and every program refused on
    # it. The innermost enclosing header, by brace extent, is recorded here; the emitter filters on it.
    # A COLUMN-0 header owns every line until the next column-0 header. Class extents are NOT brace-matched:
    # a '{' or '}' literal inside a rule body closes a naive matcher early (measured: Perl6::Grammar's extent
    # ended after 49 rules and the gate caught the mis-filter). A declaration indented deeper than a grammar
    # member (4 spaces) sits inside a nested role/block and is marked nested.
    HDR = re.compile(r'^(grammar|role|class)\s+([\w:]+(?:\[[^\]]*\])?)\s*([^{]*)\{')
    hdrs = []
    for ln, line in enumerate(lines):
        hm = HDR.match(line)
        if hm: hdrs.append((ln, hm.group(2), re.findall(r'does\s+([\w:]+)', hm.group(3))))
    def owner_of(ln):
        best = None
        for hl, nm, does in hdrs:
            if hl <= ln: best = (hl, nm, does)
        return best
    out, disagree = [], 0
    for k, (ln, m) in enumerate(starts):
        obr = offs[ln] + m.end() - 1
        close = _match_brace(src, obr)
        if close < 0: close = len(src) - 1
        body = src[obr+1:close]
        endln = src.count('\n', 0, close)
        limit = starts[k+1][0] if k + 1 < len(starts) else len(lines)
        if endln >= limit:
            # ⛔ the brace match ran into the next declaration (a '{' literal inside the body). The
            # indentation-anchored closing line -- `<indent>}` alone, before the next declaration -- is
            # the body's real end in Rakudo's consistently formatted source. Recovered rather than dropped:
            # `token variable` was one of the two, and dropping it made every program refuse on it.
            ind = m.group('ind')
            cl = next((j for j in range(ln + 1, limit) if lines[j] == ind + '}'), None)
            if cl is not None:
                close = offs[cl]; body = src[obr+1:close]; endln = cl
            else:
                disagree += 1
        nm = m.group('name') or m.group('pname'); sym = None
        params = (m.group('params') or '').strip()      # `token scoped($*SCOPE) {` -> name 'scoped', params '($*SCOPE)'
        # ⛔ the DECL regex's `name` alternative wins for `scoped($*SCOPE)` (no space before the brace), so the
        # parameter list rode the NAME and cname('scoped($*SCOPE)') never matched a call to <scoped('my')>.
        if not params and '(' in nm and nm.endswith(')'):
            params = nm[nm.index('('):]; nm = nm[:nm.index('(')]
        sm = re.match(r'([^:]+):sym[<«](.*)[>»]$', nm)
        if sm: nm, sym = sm.group(1), sm.group(2)
        ow = owner_of(ln)
        indent = len(lines[ln]) - len(lines[ln].lstrip(' '))
        out.append(dict(kind=m.group('kind'), name=nm, sym=sym, proto=bool(m.group('proto')),
                        line=ln+1, endline=endln+1, body=body, overrun=(endln >= limit),
                        owner=(ow[1] if ow else ''), owner_does=(ow[2] if ow else []), owner_nested=(indent > 4),
                        params=params))
    if disagree:
        print(f"⚠️  {disagree} production(s) whose brace-match ran past the next declaration "
              f"-- bodies may be contaminated; investigate before trusting per-production output.",
              file=sys.stderr)
    return out

def in_grammar(decls, grammar='Perl6::Grammar'):
    """The declarations that belong to `grammar`: its own (non-nested) plus those of the top-level roles it
    `does` (STD for the main language). Other grammars and the quote-language roles are other languages --
    reached only through LANG('Quote', …)/LANG('Regex', …), which refuse by name until they are ported."""
    does = set()
    for d in decls:
        if d['owner'] == grammar: does.update(d['owner_does'])
    keep = {grammar} | does
    return [d for d in decls if d['owner'] in keep and not d['owner_nested']]

# ---------------------------------------------------------------------------------------------
# Body tokenizer for the nqp regex sublanguage.
# ---------------------------------------------------------------------------------------------
def lex_body(b):
    toks, i, n = [], 0, len(b)
    def emit(k, v): toks.append((k, v))
    while i < n:
        c = b[i]
        if c in ' \t\r\n': i += 1; continue
        if c == '#':
            while i < n and b[i] != '\n': i += 1
            continue
        # embedded code block { ... }
        if c == '{':
            d, j = 0, i
            while j < n:
                if b[j] == '{': d += 1
                elif b[j] == '}':
                    d -= 1
                    if d == 0: break
                j += 1
            emit('CODE', b[i:j+1]); i = j + 1; continue
        # :my / :dba / other declarators
        if c == ':' and b.startswith(':my', i):
            j = b.find(';', i)
            j = n if j < 0 else j
            emit('MY', b[i:j+1]); i = j + 1; continue
        if c == ':' and re.match(r':\w+', b[i:]):
            m = re.match(r':\w+(\([^)]*\))?', b[i:]); emit('MOD', m.group(0)); i += m.end(); continue
        # assertion / subrule  < ... >  (brace-aware for nested <>)
        if c == '<':
            # ⛔ depth-count < > but SKIP quoted strings and [ ] character classes: a `>` inside `'=>'` or
            # `'>>>>>>>'` is not a closer. Measured before this fix: 101 stray `>` tokens (33 rules refusing
            # first on UNKNOWN:>) and every `?[` class truncated -- vcs-conflict, the `'=>'` lookaheads, …
            d, j, cb = 0, i, 0
            while j < n:
                ch = b[j]
                if ch == '\\': j += 2; continue
                # inside a { code } block within the assertion, < and > are OPERATORS (`$rev < 3`), not brackets:
                # counting them cut `<?{ $rev < 3 }>` short and dragged the next assertion into it.
                if ch == '{': cb += 1; j += 1; continue
                if ch == '}' and cb: cb -= 1; j += 1; continue
                if cb: j += 1; continue
                if ch in "'\"":
                    k = b.find(ch, j + 1)
                    while k > 0 and b[k-1] == '\\': k = b.find(ch, k + 1)
                    j = (k if k > 0 else n) + 1; continue
                if ch == '[':
                    bd = 1; j += 1
                    while j < n and bd:
                        if b[j] == '\\': j += 2; continue
                        if b[j] == '[': bd += 1
                        elif b[j] == ']': bd -= 1
                        j += 1
                    continue
                if ch == '<': d += 1
                elif ch == '>':
                    d -= 1
                    if d == 0: break
                j += 1
            inner = b[i+1:j]
            if   re.match(r'[.+?!\-]?\[', inner): emit('CCLASS', inner)
            elif inner.startswith('?'): emit('LOOKP', inner)
            elif inner.startswith('!'): emit('LOOKN', inner)
            elif inner.startswith('['): emit('CCLASS', inner)
            elif inner.startswith('.'): emit('CALL_NC', inner[1:])
            elif inner == '...':        emit('PROTOBODY', inner)
            else:                       emit('CALL', inner)
            i = j + 1; continue
        # named capture  $<name>=
        if b.startswith('$<', i):
            j = b.find('>', i)
            m = re.match(r'\$<([^>]*)>\s*=', b[i:])
            if m: emit('NCAP', m.group(1)); i += m.end(); continue
            emit('VAR', b[i:j+1]); i = j + 1; continue
        # ⛔ a lone `$` is the END-OF-STRING ANCHOR (13 sites, comp_unit among them). It must be decided
        # BEFORE the variable regex below, whose character class includes `]` and `>` -- which is how `$]`
        # was being lexed as a VARIABLE named `$]` (two such phantoms in the census) and `$ ` as UNKNOWN.
        if c == '$' and (i + 1 >= len(b) or b[i + 1] in ' \t\n\r)]|&>'):
            emit('ANCHOR', '$'); i += 1; continue
        if c == '$' or c == '@' or c == '%':
            m = re.match(r'[$@%][\w*.:<>\[\]-]+', b[i:])
            if m: emit('VAR', m.group(0)); i += m.end(); continue
        # literals
        if c in "'\"":
            j, q = i + 1, c
            while j < n and b[j] != q:
                if b[j] == '\\': j += 1
                j += 1
            emit('LIT', b[i+1:j]); i = j + 1; continue
        # alternation: || is ordered, | is LONGEST-TOKEN-MATCH. The distinction is load-bearing.
        if b.startswith('||', i): emit('ALT_ORD', '||'); i += 2; continue
        if b.startswith('&&', i): emit('CONJ', '&&'); i += 2; continue
        if c == '|':              emit('ALT_LTM', '|'); i += 1; continue
        if c == '&':              emit('CONJ', '&'); i += 1; continue
        if c in '[](': emit('OPEN' if c in '[(' else 'CLOSE', c); i += 1; continue
        if c == ')': emit('CLOSE', c); i += 1; continue
        if b.startswith('**', i):
            m = re.match(r'\*\*\s*[\d.^]*(\{[^}]*\})?', b[i:]); emit('QUANT', m.group(0)); i += m.end(); continue
        if c in '*+?':
            emit('QUANT', c); i += 1
            if i < n and b[i] in '?:': i += 1      # non-greedy / ratchet marker
            continue
        if c == '%':  emit('SEP', '%'); i += 1; continue
        if c == '~':  emit('GOAL', '~'); i += 1; continue
        if c == '\\':
            # \xHHHH keeps its hex digits (and \x[…] its bracket form): bom is `\xFEFF` -- the FIRST rule comp_unit
            # calls -- and dropping the digits made it refuse on every program, including the empty one.
            m = re.match(r'\\x(?:\[[0-9A-Fa-f, ]+\]|[0-9A-Fa-f]{1,6})', b[i:]) or re.match(r'\\.', b[i:])
            emit('ESC', m.group(0)); i += m.end(); continue
        if c == '«':
            j = b.find('»', i)
            if j >= 0: emit('LIT', b[i+1:j]); i = j + 1; continue
            emit('WB', c); i += 1; continue
        if c == '»': emit('WB', c); i += 1; continue
        if b.startswith('^^', i) or b.startswith('$$', i): emit('ANCHOR', b[i:i+2]); i += 2; continue
        if c == '^': emit('ANCHOR', '^'); i += 1; continue
        m = re.match(r'[\w-]+', b[i:])
        if m: emit('BARE', m.group(0)); i += m.end(); continue
        emit('UNKNOWN', c); i += 1
    return toks

def main(path):
    src = open(path, encoding='utf-8', errors='replace').read()
    decls = scan_decls(src)
    kinds = collections.Counter()
    tokhist = collections.Counter()
    unknown = collections.Counter()
    calls = collections.Counter()
    per_prod_unknown = 0
    protos = collections.Counter()
    for d in decls:
        kinds[('proto ' if d['proto'] else '') + d['kind']] += 1
        if d['sym'] is not None: protos[d['name']] += 1
        toks = lex_body(d['body'])
        d['ntok'] = len(toks)
        bad = 0
        for k, v in toks:
            tokhist[k] += 1
            if k == 'UNKNOWN': unknown[v] += 1; bad += 1
            if k in ('CALL', 'CALL_NC'):
                calls[re.split(r'[(=:\s]', v)[0]] += 1
        if bad: per_prod_unknown += 1
    print(f"FILE                : {path}")
    print(f"productions         : {len(decls)}")
    for k, v in sorted(kinds.items(), key=lambda x: -x[1]): print(f"  {k:<14} {v}")
    print(f"protoregex families : {len(protos)}  (candidates: {sum(protos.values())})")
    print(f"productions with an UNPARSED char : {per_prod_unknown}")
    over = [d for d in decls if d.get('overrun')]
    print(f"productions with a CONTAMINATED body (brace-match overran) : {len(over)}"
          + (("  -> " + ', '.join(f"{d['name']}({d['line']})" for d in over)) if over else ""))
    print()
    print("BODY CONSTRUCT HISTOGRAM (what a translator must implement):")
    for k, v in tokhist.most_common(): print(f"  {k:<12} {v}")
    if unknown:
        print()
        print("UNPARSED CHARS (must be understood before emitting code):")
        for k, v in unknown.most_common(12): print(f"  {k!r:<8} {v}")
    print()
    print(f"distinct subrules referenced : {len(calls)}")
    defined = {d['name'] for d in decls}
    undef = [c for c in calls if c and c not in defined]
    print(f"referenced but NOT defined in this file : {len(undef)}  (inherited//NQP builtins)")
    print("  top:", ', '.join(f"{c}({calls[c]})" for c in sorted(undef, key=lambda x: -calls[x])[:12]))
    json.dump([{k: v for k, v in d.items() if k != 'body'} for d in decls],
              open('/tmp/claude-1000/-home-claude-C/bf92a46a-6680-4a47-b7db-57e12b40e2b1/scratchpad/decls.json', 'w'), indent=1)

if __name__ == '__main__':
    main(sys.argv[1] if len(sys.argv) > 1 else '/home/resources/rakudo-main/src/Perl6/Grammar.nqp')
