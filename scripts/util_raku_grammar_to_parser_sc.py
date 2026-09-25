#!/usr/bin/env python3
"""util_raku_grammar_to_parser_sc.py OUT.sc OUT.map.tsv -- mechanical translation of Rakudo's Perl6::Grammar (+ STD, HLL::Grammar) into a Snocone recognizer.
Phase 1 (Lon 2026-09-25): no semantic routines -- no shift/reduce, no tree; the driver prints Parsed. or Parse Error."""
import re, sys, collections
sys.path.insert(0, __import__('os').path.dirname(__file__))
import util_raku_nqp_decl as nqpdecl, util_raku_nqp_regex as nqprx, os
DEBUG = bool(os.environ.get('RK_DEBUG'))
GRAMMAR = '/home/resources/rakudo-2026.05/src/Perl6/Grammar.nqp'
HLL = '/home/resources/rakudo-2026.05/nqp/src/HLL/Grammar.nqp'
PREFIX = {'Perl6::Grammar': 'g', 'STD': 'std', 'HLL::Grammar': 'hll', 'Perl6::QGrammar': 'q', 'Perl6::RegexGrammar': 'rx',
          'Perl6::P5RegexGrammar': 'p5', 'MatchPackageNibbler': 'mpn', 'startstops': 'ss3', 'startstop': 'ss2', 'stop': 'ss1'}
CHAIN = {'Perl6::Grammar': ['Perl6::Grammar', 'STD', 'HLL::Grammar'], 'STD': ['Perl6::Grammar', 'STD', 'HLL::Grammar'],
         'HLL::Grammar': ['Perl6::Grammar', 'STD', 'HLL::Grammar']}
def enc(x):
    out = ''
    for ch in x:
        if ch.isascii() and (ch.isalnum() or ch == '_'): out += ch
        elif ch.isascii(): out += 'x%02x' % ord(ch)
        else: out += 'u%04x' % ord(ch)
    return out
def cprefix(cont):
    parts = cont.split('/') if cont else ['']
    head = PREFIX.get(parts[0], enc(parts[0]) if parts[0] else 'top')
    return '_'.join([head] + [enc(p) for p in parts[1:]])
def base(n): return re.split(r':', n, 1)[0]
def sym_of(n):
    m = re.search(r':sym<([^>]*)>|:sym«([^»]*)»', n)
    return None if not m else (m.group(1) if m.group(1) is not None else m.group(2))
def mangle(cont, n):
    b = base(n); rest = n[len(b):]
    s = cprefix(cont) + '_' + enc(b)
    if rest:
        m = re.match(r':(\w+)[<«](.*)[>»]$', rest)
        s += ('__' if m and m.group(1) == 'sym' else '__' + (m.group(1) if m else '') + '_') + enc(m.group(2) if m else rest)
    return s
def q(sv):
    """a Snocone string literal for sv (UTF-8 text)"""
    if "'" not in sv: return "'" + sv + "'"
    if '"' not in sv: return '"' + sv + '"'
    return ' '.join(q(p) if p else '' for p in re.split(r"(')", sv) if p != '').replace("''", '')
# ---------------------------------------------------------------------------------------------------------------------------------
CODEASSERT = [   # (substring of the assertion's code, the value of the code itself: T or F; the <!{ }> form negates it)
    ('herestub_queue', 'F'), ('$*IN_REGEX_ASSERTION', 'F'), ('$*MULTINESS eq', 'F'), ('bracket_ending', 'F'), ('$*POD_', 'F'),
    ('$*COMPILING_CORE_SETTING ||', 'F'), ('$*RESTRICTED', 'F'), ("?? False !! self.panic", 'F'), ("getlexdyn('$?FILES')", 'F'),
    ('$*WHENEVER_COUNT', 'F'), ('$*MAIN ne $OLD_MAIN', 'F'), ('$*IN_QUASI', 'F'), ('$*IN_PROTO', 'F'), ('language_revision >= 3', 'F'),
    ('$rev >= 3', 'F'), ("&term:<nano>", 'F'), ("$*GOAL eq 'endargs' && !", 'F'), ('$<sibble><infixish>', 'F'), ('$*IN_META ~~', 'F'),
    ("~$<EXPR> ~~ / '!!'", 'F'), ("$<infixish>.Str eq '='", 'F'), ('$*INVOCANT_OK', 'F'), ('pasttype', 'F'), ("$<sigil>.Str eq '&'", 'F'),
    ('!$*IN_DECL && $*VARIABLE', 'F'), ("$*QSIGIL eq '$'", 'F'), ('my $marked := $c.MARKED', 'POSTWS'),
    ('!$*IN_DECL', 'T'), ('$*ARG_FLAT_OK := 0', 'F'), ('is_name($*longname.components())', 'ISNAME'), ('is_name(', 'F'), ('is_type(', 'F'), ('is_lexical', 'F'), ('$is_type', 'ISTYPE'), ('validate_type_smiley', 'T'),
    ('$*IN_DECL', 'F'), ('$*QSIGIL', 'F'), ('$*IN_META', 'F'), ('$*IN_REDUCE', 'F'),
]
MEMO = {'g_terminator': '', 'g__ws': '', 'g_stdstopper': 'endstmt', 'g_lambda': '', 'g_infixstopper': 'ws', 'g_morename': ''}
ERRFAIL = {'panic', 'typed_panic', 'obs', 'obsvar', 'malformed', 'missing', 'NYI', 'FAILGOAL', 'missing_block', 'sorryobs', 'security', 'nomodexpr', 'dupprefix'}
NOOP = {'sorry', 'worry', 'typed_worry', 'typed_sorry', 'explain_mystery', 'cry_sorrows', 'attach_leading_docs', 'AS_MATCH', 'set_braid_from',
        'SIGOK', 'can_meta', 'install_doc_phaser', 'unitstart', 'finishpad', 'newpad', 'newthunk', 'O', 'ccstate', 'heredoc'}
BUILTIN = {'alpha': 'ANY(rk_alpha)', 'ident': '*rk_ident', 'upper': 'ANY(rk_upper)', 'space': 'ANY(rk_s)', 'digit': 'ANY(rk_digit)',
           'alnum': 'ANY(rk_alnum)', 'xdigit': 'ANY(rk_xdigit)', 'lower': 'ANY(rk_lower)', 'wb': '*rk_wb', 'ww': '*rk_ww', 'punct': 'ANY(rk_punct)'}
class Gen:
    def __init__(s):
        s.decls = [d for d in nqpdecl.read(GRAMMAR)] + [('HLL::Grammar' if c == 'HLL::Grammar' or not c else c,) + d[1:] for d in nqpdecl.read(HLL) for c in [d[0]]]
        s.rules = collections.OrderedDict(); s.protos = collections.defaultdict(list); s.methods = collections.defaultdict(set)
        for c, k, n, p, b, ln, proto in s.decls:
            top = c.split('/')[0] if c else ''
            if k in ('token', 'rule', 'regex'):
                s.rules[(c, n)] = (k, p, b, ln, proto)
                if sym_of(n) is not None or re.search(r':\w+<', n): s.protos[(c, base(n))].append(n)
            else: s.methods[base(n)].add(c)
        s.helpers = []; s.hn = 0; s.warn = collections.Counter(); s.map = []
    def resolve(s, cont, name):
        top = cont.split('/')[0] if cont else ''
        chain = CHAIN.get(top, [cont, 'STD', 'HLL::Grammar'])
        if '/' in cont: chain = [cont] + [x for x in chain if x != cont]
        for c in chain:
            if (c, name) in s.rules or s.protos.get((c, name)):
                m = mangle(c, name)
                if m in MEMO: return "@rk_cur *rk_mem('%s', '%s')" % (m, MEMO[m])
                if DEBUG: return "*rk_enter('%s')" % m
                return '*' + m
        return None
    def fresh(s, stem): s.hn += 1; return 'h%d_%s' % (s.hn, stem[:24])
    def emit(s, n, ctx):
        t = n[0]
        if t == 'seq': return ' '.join(x for x in (s.emit(i, ctx) for i in n[1]) if x) or 'epsilon'
        if t == 'alt':
            arms = [s.emit(a, ctx) or 'epsilon' for a in n[2]]
            if ctx['ratchet'] and n[1] == '|' and len(arms) > 1:
                pat = arms[-1]
                for a in reversed(arms[:-1]): pat = '@rk_cur *rk_ltm2((%s), (%s))' % (a, pat)
                return pat
            body = ' | '.join('(' + a + ')' for a in arms)
            return ('FENCE(' + body + ')') if ctx['ratchet'] else '(' + body + ')'
        if t == 'conj': s.warn['conj'] += 1; return s.emit(n[1][0], ctx)
        if t == 'group' or t == 'cap': return '(' + (s.emit(n[1], ctx) or 'epsilon') + ')'
        if t == 'named': return s.emit(n[2], ctx) if n[2] else 'epsilon'
        if t == 'lit': return q(n[1]) if n[1] else 'epsilon'
        if t == 'sym': return q(ctx['sym']) if ctx['sym'] is not None else (s.warn.update(['nosym']) or 'epsilon')
        if t == 'any': return 'LEN(1)'
        if t == 'ws': return '*' + ctx['ws']
        if t == 'code' or t == 'mod': return ''
        if t == 'true': return 'epsilon'
        if t == 'false': return 'FAIL'
        if t == 'codeassert': return s.codeassert(n[1], n[2], ctx)
        if t == 'esc': return ESC[n[1]]
        if t == 'anchor': return ANCH[n[1]]
        if t == 'cclass': return cclass(n[1], s)
        if t == 'var': s.warn['var:' + n[1]] += 1; return '*' + VARS.get(n[1], 'rk_var_' + enc(n[1]))
        if t == 'goal': return ' '.join(x for x in (s.emit(n[3], ctx) if n[3] else '', s.emit(n[2], ctx)) if x)
        if t == 'look':
            inner = s.emit(n[3], ctx) or 'epsilon'
            f = {'before': 'rk_nla' if n[2] else 'rk_la', 'after': 'rk_nlb' if n[2] else 'rk_lb'}[n[1]]
            return '@rk_cur *%s(%s)' % (f, inner)
        if t == 'call': return s.call(n, ctx)
        if t == 'quant': return s.quant(n, ctx)
        raise ValueError('node ' + t)
    def codeassert(s, neg, code, ctx):
        """A code assertion reads interpreter state this recognizer does not track; its value is chosen from its MEANING,
        one table, never from the text's shape: the state a recognizer is in is 'no heredoc pending, not in a regex
        assertion, not in a declaration's own name, no pod, no proto, language revision 2 (6.d)'."""
        c = re.sub(r'\s+', ' ', code.strip('{} \n'))
        for key, val in CODEASSERT:
            if key in c:
                s.warn['codeassert:' + val] += 1
                if val == 'POSTWS': return '@rk_cur *rk_postws()'
                if val in ('ISNAME', 'ISTYPE'): return '@rk_cur *%s(%d)' % ('rk_isname' if val == 'ISNAME' else 'rk_istype', 1 if neg else 0)
                truth = (val == 'T')
                return 'epsilon' if truth != neg else 'FAIL'
        s.warn['codeassert:default-holds'] += 1
        return 'epsilon'
    def call(s, n, ctx):
        name, args, zero, neg, alias = n[1], n[2], n[3], n[4], n[5]
        if len(n) > 6:
            return '@rk_cur *%s(%s)' % ('rk_nla' if neg else 'rk_la', s.emit(n[6], ctx)) if name == 'before' else s.emit(n[6], ctx)
        if name in ERRFAIL: return 'FAIL'
        if name in NOOP: return 'epsilon' if not neg else 'FAIL'
        if name == 'ws': pat = '*' + ctx['ws']
        elif name == 'EXPR': pat = '*rk_EXPR'
        elif name in ('MARKER',): return "@rk_cur *rk_marker(%s)" % q(args.strip().strip("'\""))
        elif name in ('MARKED',): return "@rk_cur *rk_marked(%s)" % q(args.strip().strip("'\""))
        elif name in ('LANG', 'FOREIGN_LANG'):
            m = re.findall(r"'([^']*)'", args); lang = args.split(',')[0].strip().strip("'\"")
            tgt = m[-1] if m else 'statementlist'
            if name == 'FOREIGN_LANG': lang = 'MAIN'
            lcont = {'MAIN': 'Perl6::Grammar', '$*MAIN': 'Perl6::Grammar', 'Quote': 'Perl6::QGrammar', 'Regex': 'Perl6::RegexGrammar', 'P5Regex': 'Perl6::P5RegexGrammar'}.get(lang, ctx['cont'])
            pat = s.resolve(lcont, tgt) or s.resolve(ctx['cont'], tgt) or ('*rk_lang_' + enc(tgt))
        elif name == 'nibble':
            return s.nibble_call(args, ctx)
        elif name in ('quibble', 'babble'):
            return s.quibble_call(args, ctx)
        elif name in BUILTIN: pat = BUILTIN[name]
        elif name.startswith('$') or name.startswith('@') or name == '~~': s.warn['dyncall:' + name] += 1; pat = '*rk_dyn_' + enc(name)
        else:
            pat = s.resolve(ctx['cont'], name)
            if pat is None:
                s.warn['method:' + name] += 1; pat = '*rk_m_' + enc(name)
        if zero: return '@rk_cur *%s(%s)' % ('rk_nla' if neg else 'rk_la', pat)
        return pat
    def split_args(s, a):
        out, depth, cur, qch = [], 0, '', None
        for ch in a:
            if qch:
                cur += ch
                if ch == qch: qch = None
                continue
            if ch in '\'"': qch = ch; cur += ch; continue
            if ch in '([{': depth += 1
            elif ch in ')]}': depth -= 1
            if ch == ',' and depth == 0: out.append(cur.strip()); cur = ''; continue
            cur += ch
        if cur.strip(): out.append(cur.strip())
        return out
    def strlit(s, x):
        x = x.strip()
        if x.startswith('['): x = s.split_args(x[1:-1])[0]
        m = re.match(r"^'(.*)'$|^\"(.*)\"$", x, re.S)
        return None if not m else (m.group(1) if m.group(1) is not None else m.group(2))
    def kind_of(s, slang, tweaks):
        if slang in ('Regex', 'P5Regex'): return 'rx'
        t = re.findall(r"'(\w+)'", tweaks or '')
        return 'qq' if 'qq' in t else ('q' if 'q' in t or not t else t[0])
    def nibble_call(s, args, ctx):
        m = re.search(r"quote_lang\((.*)\)\s*$", args, re.S)
        if not m: s.warn['nibble:dynamic'] += 1; return '@rk_cur *rk_nib(rk_dstart, rk_dstop, rk_dkind)'
        parts = s.split_args(m.group(1))
        sl = re.search(r"slang_grammar\('(\w+)'\)", parts[0]); slang = sl.group(1) if sl else 'Quote'
        st = s.strlit(parts[1]) if len(parts) > 1 else None; sp = s.strlit(parts[2]) if len(parts) > 2 else st
        if st is None or sp is None: s.warn['nibble:nodelim'] += 1; return '@rk_cur *rk_nib(rk_dstart, rk_dstop, rk_dkind)'
        return '@rk_cur *rk_nib(%s, %s, %s)' % (q(st), q(sp), q(s.kind_of(slang, parts[3] if len(parts) > 3 else '')))
    def quibble_call(s, args, ctx):
        parts = s.split_args(args)
        sl = re.search(r"slang_grammar\('(\w+)'\)", parts[0]) if parts else None
        slang = sl.group(1) if sl else ('Regex' if parts and 'Regex' in parts[0] else 'Quote')
        tw = parts[1] if len(parts) > 1 else ''
        kind = s.kind_of(slang, tw) if tw.startswith("'") or slang != 'Quote' else 'Q'
        return '*g_ws @rk_cur *rk_star((*g_quotepair *g_ws), FAIL, 0) @rk_cur *rk_qscan(%s)' % q(kind)
    def quant(s, n, ctx):
        a, qq, sep, sk = n[1], n[2], n[3], n[4]
        x = s.emit(a, ctx) or 'epsilon'
        sp = (s.emit(sep, ctx) or 'epsilon') if sep else None
        if not ctx['ratchet']:
            if qq == '*' and not sp: return 'ARBNO(' + x + ')'
        if qq == '?': return 'FENCE((' + x + ') | epsilon)' if ctx['ratchet'] else '((' + x + ') | epsilon)'
        if isinstance(qq, tuple):
            lo, hi = qq[1], qq[2]
            parts = [x] * lo
            if hi == '*': rest = s.quant(('quant', a, '*', sep, sk), ctx); parts.append(rest)
            else:
                for _ in range(int(hi) - lo): parts.append('FENCE((' + x + ') | epsilon)')
            return ' '.join('(' + p + ')' for p in parts) or 'epsilon'
        sepx = sp if sp else 'FAIL'
        f = {'*': 'rk_star', '+': 'rk_plus'}[qq]
        s.hn += 1; lab = '%s_%d' % (ctx['stem'][:30], s.hn)
        if DEBUG: return "@rk_cur *rk_cnt1('%s') *%s(%s, %s, %d)" % (lab, f, x, sepx, 2 if sk == '%%' else (1 if sp else 0))
        return '@rk_cur *%s(%s, %s, %d)' % (f, x, sepx, 2 if sk == '%%' else (1 if sp else 0))
    def rule_pattern(s, key):
        c, n = key; k, p, b, ln, proto = s.rules[key]
        ctx = {'cont': c, 'sym': sym_of(n), 'ratchet': k != 'regex', 'ws': s.ws_of(c), 'stem': enc(base(n))}
        if b.strip() in ('*', '<...>', '...') or (proto and b.strip() in ('*', '<...>')): return None
        try: ast = nqprx.P(b, sigspace=(k == 'rule')).parse()
        except Exception as e: s.warn['parse-error'] += 1; return 'FAIL'
        return s.emit(ast, ctx) or 'epsilon'
    def ws_of(s, c): return mangle('Perl6::Grammar', 'ws') if (c.split('/')[0] if c else '') in ('Perl6::Grammar', 'STD', 'HLL::Grammar', '') else mangle(c.split('/')[0], 'ws')
    def proto_pattern(s, c, bname):
        top = c.split('/')[0]
        chain = CHAIN.get(top, [c, 'STD', 'HLL::Grammar'])
        seen = {}; order = []
        for cc in chain:
            for v in s.protos.get((cc, bname), []):
                sy = sym_of(v)
                if sy not in seen: seen[sy] = (cc, v); order.append(sy)
        arms = sorted(order, key=lambda sy: -len(sy or ''))
        pats = ['*' + mangle(*seen[sy]) for sy in arms]
        if not pats: return 'FAIL'
        chunks = [pats[i:i + 8] for i in range(0, len(pats), 8)]
        if len(chunks) == 1: return 'FENCE(' + ' | '.join(pats) + ')'
        names = []
        for ch in chunks:
            h = s.fresh('proto_' + enc(bname)); s.helpers.append('%s = %s;' % (h, ' | '.join(ch))); names.append('*' + h)
        return 'FENCE(' + ' | '.join(names) + ')'
    def generate(s):
        out = []
        done = set()
        for (c, n), (k, p, b, ln, proto) in s.rules.items():
            nm = mangle(c, n)
            if nm in done: continue
            done.add(nm)
            if (c, base(n)) in s.protos and base(n) == n:
                pat = s.proto_pattern(c, n)
            else:
                pat = s.rule_pattern((c, n))
                if pat is None: pat = s.proto_pattern(c, base(n))
            out.append('%s = %s;' % (nm, pat))
            if not c.startswith('HLL'): s.map.append((c + '/' + n, nm))
        for (c, bn) in list(s.protos):
            nm = mangle(c, bn)
            if nm not in done: done.add(nm); out.append('%s = %s;' % (nm, s.proto_pattern(c, bn)))
        return out
ESC = {'w': 'ANY(rk_w)', 'W': 'NOTANY(rk_w)', 'd': 'ANY(rk_digit)', 'D': 'NOTANY(rk_digit)', 's': 'ANY(rk_s)', 'S': 'NOTANY(rk_s)',
       'h': 'ANY(rk_h)', 'H': 'NOTANY(rk_h)', 'v': 'ANY(rk_v)', 'V': 'NOTANY(rk_v)', 'n': '(cr nl | nl | cr)', 'N': 'NOTANY(nl cr)',
       't': 'tab', 'T': 'NOTANY(tab)', 'r': 'cr', 'R': 'NOTANY(cr)', 'e': "CHAR(27)", 'E': 'NOTANY(CHAR(27))', 'f': 'ff', 'F': 'NOTANY(ff)', '0': 'CHAR(0)'}
ANCH = {'^': 'POS(0)', '$': 'RPOS(0)', '^^': '@rk_cur *rk_bol()', '$$': '@rk_cur *rk_eol()', '<<': '@rk_cur *rk_lwb()', '>>': '@rk_cur *rk_rwb()'}
VARS = {'$*DELIM': 'rk_delim', '$start': 'rk_start', '$stop': 'rk_stop', '$stop1': 'rk_stop', '$stop2': 'rk_stop2'}
UNI = {'Letter': 'rk_alpha', 'L': 'rk_alpha', 'Lu': 'rk_upper', 'Ll': 'rk_lower', 'Alpha': 'rk_alpha', 'Nd': 'rk_digit', 'No': "''", 'Nl': "''",
       'N': 'rk_digit', 'Pd': "'-'", 'Ps': "'([{'", 'Pe': "')]}'", 'Pi': "''", 'Pf': "''", 'Sm': "'+<=>|~'", 'Zs': "' '", 'space': 'rk_s', 'White_Space': 'rk_s'}
NAMED = {'alpha': 'rk_alpha', 'digit': 'rk_digit', 'alnum': 'rk_alnum', 'xdigit': 'rk_xdigit', 'space': 'rk_s', 'ident': 'rk_alpha',
         'upper': 'rk_upper', 'lower': 'rk_lower', 'punct': 'rk_punct', 'ws': 'rk_s', 'cntrl': "''", 'print': 'rk_print', 'graph': 'rk_print'}
def cls_set(seg):
    """the chars of one [...] class body as a Snocone string expression"""
    items = []; lit = ''; i = 0; b = seg
    esc = {'n': 'nl', 't': 'tab', 'r': 'cr', 'f': 'ff', 'e': 'CHAR(27)', 's': 'rk_s', 'h': 'rk_h', 'v': 'rk_v', 'w': 'rk_w', 'd': 'rk_digit', '0': 'CHAR(0)'}
    chars = []
    while i < len(b):
        ch = b[i]
        if ch in ' \t\n': i += 1; continue
        if ch == '\\' and i + 1 < len(b):
            e = b[i+1]
            if e in esc: items.append(esc[e]); i += 2; continue
            if e == 'x' and i + 2 < len(b) and b[i+2] == '[':
                j = b.index(']', i); chars.extend(chr(int(h, 16)) for h in b[i+3:j].split(',')); i = j + 1; continue
            chars.append(e); i += 2; continue
        if i + 3 < len(b) and b[i+1:i+3] == '..':
            lo, hi = ch, b[i+3]; i += 4
            if lo.isascii() and hi.isascii(): chars.extend(chr(c) for c in range(ord(lo), ord(hi) + 1))
            else: chars.extend([lo, hi])
            continue
        if ch == "'" or ch == '"':
            j = b.find(ch, i + 1)
            if j > i: chars.extend(b[i+1:j]); i = j + 1; continue
        chars.append(ch); i += 1
    if chars: items.append(q(''.join(dict.fromkeys(chars))))
    return ' '.join(items) if items else "''"
def cclass(spec, g):
    sp = spec.strip()
    zero = neg0 = False
    while sp and sp[0] in '?!.':
        if sp[0] == '?': zero = True
        if sp[0] == '!': zero = True; neg0 = not neg0
        sp = sp[1:]
    terms = re.findall(r'([+-]?)\s*(\[(?:\\.|[^\]])*\]|:!?\w+(?:<[^>]*>)?|\w+)', sp)
    pos, negs = [], []
    first = True
    for sign, body in terms:
        if body.startswith('['): e = cls_set(body[1:-1])
        elif body.startswith(':'):
            nm = body.lstrip(':!'); e = UNI.get(nm, 'rk_alpha'); g.warn['uniprop:' + nm] += 0 if nm in UNI else 1
            if body.startswith(':!'): sign = '-'
        else: e = NAMED.get(body, 'rk_alpha')
        (negs if sign == '-' else pos).append(e); first = False
    pos = [x for x in pos if x != "''"]; negs = [x for x in negs if x != "''"]
    if not pos and not negs: m = 'FAIL'
    elif not pos and negs: m = 'NOTANY(' + ' '.join(negs) + ')'
    elif negs: m = '*rk_cdiff(' + ' '.join(pos) + ', ' + ' '.join(negs) + ')'
    else: m = 'ANY(' + ' '.join(pos) + ')'
    if zero: return '@rk_cur *%s(%s)' % ('rk_nla' if neg0 else 'rk_la', m)
    return m
PRELUDE = r'''/* parser_raku.sc -- a MECHANICAL translation of Rakudo 2026.05's grammar (src/Perl6/Grammar.nqp, with the STD role and
   NQP's HLL::Grammar it inherits) into Snocone patterns, generated by scripts/util_raku_grammar_to_parser_sc.py; the rule map is
   bootstrap/parser_raku.map.tsv.  Lon 2026-09-25, in-chat to hq_snocone: "For Raku, do a mechanical conversion from the official
   grammar specification into the SNOBOL4 grammar patterns, just like all the others and re-write parser_raku.sc completely." and
   "For Raku, test first without any semantic rountines, i.e. no tree building, then add those after the syntax check only works."
   PHASE 1: a recognizer -- no shift, no reduce, no tree; the driver prints Parsed. or Parse Error. */
&ANCHOR   = 1;
rk_lower  = 'abcdefghijklmnopqrstuvwxyz';
rk_upper  = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ';
rk_digit  = '0123456789';
rk_alpha  = rk_lower rk_upper '_';
rk_alnum  = rk_alpha rk_digit;
rk_w      = rk_alnum;
rk_xdigit = rk_digit 'abcdefABCDEF';
rk_h      = ' ' tab;
rk_v      = nl cr ff CHAR(11);
rk_s      = rk_h rk_v;
rk_punct  = '!"#%&' "'" '()*,-./:;?@[\]_{}';
rk_print  = rk_alnum rk_punct ' $+<=>^`|~';
rk_cur    = 0;
rk_marks  = TABLE();
function rk_la(p, c) { c = rk_cur; if (Src ? TAB(c) p) { rk_la = epsilon; return; } freturn; }
function rk_nla(p, c) { c = rk_cur; if (Src ? TAB(c) p) freturn; rk_nla = epsilon; return; }
function rk_lb(p, c) { c = rk_cur; if (SUBSTR(Src, 1, c) ? ARB p RPOS(0)) { rk_lb = epsilon; return; } freturn; }
function rk_nlb(p, c) { c = rk_cur; if (SUBSTR(Src, 1, c) ? ARB p RPOS(0)) freturn; rk_nlb = epsilon; return; }
function rk_ch(k) { rk_ch = GT(k, 0) LE(k, SIZE(Src)) SUBSTR(Src, k, 1); return; }
function rk_isw(k, ch) { ch = rk_ch(k); if (IDENT(ch)) freturn; if (ch ? ANY(rk_w)) return; freturn; }
function rk_bol() { if (EQ(rk_cur, 0)) { rk_bol = epsilon; return; } if (rk_ch(rk_cur) ? ANY(rk_v)) { rk_bol = epsilon; return; } freturn; }
function rk_eol() { if (EQ(rk_cur, SIZE(Src))) { rk_eol = epsilon; return; } if (rk_ch(rk_cur + 1) ? ANY(rk_v)) { rk_eol = epsilon; return; } freturn; }
function rk_lwb() { if (rk_isw(rk_cur)) freturn; if (rk_isw(rk_cur + 1)) { rk_lwb = epsilon; return; } freturn; }
function rk_rwb() { if (rk_isw(rk_cur + 1)) freturn; if (rk_isw(rk_cur)) { rk_rwb = epsilon; return; } freturn; }
function rk_ww_f() { if (rk_isw(rk_cur)) { if (rk_isw(rk_cur + 1)) { rk_ww_f = epsilon; return; } } freturn; }
function rk_wb_f() { if (rk_isw(rk_cur)) { if (rk_isw(rk_cur + 1)) freturn; rk_wb_f = epsilon; return; } if (rk_isw(rk_cur + 1)) { rk_wb_f = epsilon; return; } freturn; }
rk_ww     = @rk_cur *rk_ww_f();
rk_wb     = @rk_cur *rk_wb_f();
function rk_iter(p, sp, mode, c, n, e, t) {
    n = 0;
    while (1) {
        t = c;
        if (GT(n, 0) EQ(mode, 1) ~(Src ? TAB(t) sp @e)) break;
        if (GT(n, 0) GE(mode, 1)) { if (Src ? TAB(t) sp @e) t = e; else break; }
        if (~(Src ? TAB(t) p @e)) { if (GT(n, 0) EQ(mode, 2)) c = t; break; }
        if (LE(e, c)) break;
        c = e; n = n + 1;
    }
    rk_n = n; rk_iter = c; return;
}
function rk_star(p, sp, mode, c) { c = rk_iter(p, sp, mode, rk_cur); rk_star = TAB(c); return; }
function rk_plus(p, sp, mode, c) { c = rk_iter(p, sp, mode, rk_cur); if (EQ(rk_n, 0)) freturn; rk_plus = TAB(c); return; }
rk_cnt = TABLE(); rk_tot = 0;
function rk_enter(nm, a, i) {
    rk_cnt[nm] = rk_cnt[nm] + 1; rk_tot = rk_tot + 1;
    if (EQ(rk_tot, 200000)) {
        a = SORT(rk_cnt, 2); i = PROTOTYPE(a); i = i ? BREAK(',') . i; i = +i;
        while (GT(i, 0)) { if (GT(i, PROTOTYPE(a) ? BREAK(',') . rk_x) 0) 1; OUTPUT = a[i, 2] ' ' a[i, 1]; i = i - 1; if (LT(i, +rk_x - 30)) break; }
    }
    rk_enter = $nm; return;
}
rk_qc = TABLE(); rk_qt = 0;
function rk_cnt1(l, a, i) {
    rk_qc[l] = rk_qc[l] + 1; rk_qt = rk_qt + 1;
    if (EQ(rk_qt, 400000)) { a = SORT(rk_qc, 2); i = 1; while (a[i, 1]) i = i + 1; i = i - 1; while (GT(i, 0)) { OUTPUT = a[i, 2] ' ' a[i, 1]; i = i - 1; if (LT(i, 0)) break; } }
    rk_cnt1 = epsilon; return;
}
rk_names = TABLE(); rk_namech = rk_alnum "'-:";
function rk_lastname(j) { j = rk_cur; while (GT(j, 0)) { if (~(SUBSTR(Src, j, 1) ? ANY(rk_namech))) break; j = j - 1; } rk_lastname = SUBSTR(Src, j + 1, rk_cur - j); return; }
function rk_known(nm) { if (nm ? POS(0) '::') return; if (nm ? POS(0) ANY(rk_upper)) return; if (DIFFER(rk_names[nm])) return; freturn; }
function rk_isname(neg) { if (rk_known(rk_lastname())) { if (EQ(neg, 1)) freturn; rk_isname = epsilon; return; } if (EQ(neg, 1)) { rk_isname = epsilon; return; } freturn; }
function rk_istype(neg, nm) { nm = rk_lastname(); if (nm ? POS(0) ANY(rk_upper)) { if (EQ(neg, 1)) freturn; rk_istype = epsilon; return; } if (EQ(neg, 1)) { rk_istype = epsilon; return; } freturn; }
function rk_prescan(i, nm, t) {
    i = 0;
    while (Src ? TAB(i) BREAK(rk_bs 'c') @i) {
        t = SUBSTR(Src, i + 1, 9);
        if (t ? POS(0) rk_bs ANY(rk_alpha)) { if (Src ? TAB(i + 1) SPAN(rk_alnum '-') . nm) rk_names[nm] = 1; }
        if (t ? POS(0) 'constant ') { if (Src ? TAB(i + 9) FENCE(SPAN(' ') | epsilon) SPAN(rk_alnum '-') . nm) rk_names[nm] = 1; }
        i = i + 1;
    }
    return;
}
function rk_ltm2(p, q, c, e, e1, e2) {
    c = rk_cur; e1 = -1; e2 = -1;
    if (Src ? TAB(c) p @e) e1 = e;
    if (Src ? TAB(c) q @e) e2 = e;
    if (LT(e1, 0)) { if (LT(e2, 0)) freturn; rk_ltm2 = TAB(e2); return; }
    if (GE(e1, e2)) { rk_ltm2 = TAB(e1); return; }
    rk_ltm2 = TAB(e2); return;
}
function rk_postws() { if (IDENT(rk_marks['ws'], rk_cur)) { if (IDENT(rk_marks['ws_from'], rk_cur)) { rk_postws = epsilon; return; } freturn; } rk_postws = epsilon; return; }
function rk_marker(m) { rk_marks[m] = rk_cur; rk_marker = epsilon; return; }
function rk_marked(m) { if (IDENT(rk_marks[m], rk_cur)) { rk_marked = epsilon; return; } freturn; }
function rk_cdiff(a, b, ch, r) { r = ''; while (a ? LEN(1) . ch = '') { if (~(ch ? ANY(b))) r = r ch; } rk_cdiff = ANY(r); return; }
rk_ident  = ANY(rk_alpha) FENCE(SPAN(rk_alnum) | epsilon);
rk_bs     = CHAR(92);
function rk_skipbal(i, d, n, ch) {
    d = 0; n = SIZE(Src);
    while (LT(i, n)) {
        ch = SUBSTR(Src, i + 1, 1);
        if (IDENT(ch, rk_bs)) i = i + 2;
        else if (IDENT(ch, '{')) { d = d + 1; i = i + 1; }
        else if (IDENT(ch, '}')) { d = d - 1; i = i + 1; if (EQ(d, 0)) { rk_skipbal = i; return; } }
        else i = i + 1;
    }
    freturn;
}
function rk_scan(i, st, sp, kind, n, d, ch, ls, lp) {
    n = SIZE(Src); d = 0; ls = SIZE(st); lp = SIZE(sp);
    while (LT(i, n)) {
        if (EQ(d, 0)) { if (IDENT(SUBSTR(Src, i + 1, lp), sp)) { rk_scan = i; return; } }
        ch = SUBSTR(Src, i + 1, 1);
        if (IDENT(ch, rk_bs)) i = i + 2;
        else if (DIFFER(st, sp) IDENT(SUBSTR(Src, i + 1, ls), st)) { d = d + 1; i = i + ls; }
        else if (DIFFER(st, sp) GT(d, 0) IDENT(SUBSTR(Src, i + 1, lp), sp)) { d = d - 1; i = i + lp; }
        else if (IDENT(kind, 'qq') IDENT(ch, '{')) { i = rk_skipbal(i); }
        else i = i + 1;
    }
    freturn;
}
function rk_nib(st, sp, kind, i) { i = rk_scan(rk_cur, st, sp, kind); rk_nib = TAB(i); return; }
rk_pairs = TABLE(); rk_pairs['('] = ')'; rk_pairs['['] = ']'; rk_pairs['{'] = '}'; rk_pairs['<'] = '>';
rk_pairs[CHAR(194) CHAR(171)] = CHAR(194) CHAR(187); rk_pairs[CHAR(239) CHAR(189) CHAR(162)] = CHAR(239) CHAR(189) CHAR(163);
function rk_qscan(kind, o, c, i) {
    o = SUBSTR(Src, rk_cur + 1, 1);
    if (IDENT(o, CHAR(194))) o = SUBSTR(Src, rk_cur + 1, 2);
    if (IDENT(o, CHAR(239))) o = SUBSTR(Src, rk_cur + 1, 3);
    if (IDENT(o) | (o ? ANY(rk_w rk_s))) freturn;
    c = rk_pairs[o]; if (IDENT(c)) c = o;
    i = rk_scan(rk_cur + SIZE(o), o, c, kind);
    rk_qscan = TAB(i + SIZE(c)); return;
}
rk_memo = TABLE();
function rk_mem(nm, mk, k, r, e) {
    k = nm ':' rk_cur; if (DIFFER(mk)) { if (IDENT(rk_marks[mk], rk_cur)) k = k ':m'; }
    r = rk_memo[k];
    if (IDENT(r)) { if (Src ? TAB(rk_cur) $nm @e) r = e; else r = -1; rk_memo[k] = r; }
    if (LT(r, 0)) freturn;
    if (IDENT(nm, 'g__ws')) { rk_marks['ws'] = r; rk_marks['ws_from'] = rk_cur; }
    rk_mem = TAB(r); return;
}
g_ws      = @rk_cur *rk_mem('g__ws', '');
rk_termish = *g_termish;
rk_EXPR_step = *g_ws *g_infixish *g_ws *rk_termish;
rk_EXPR   = *rk_termish @rk_cur *rk_star(*rk_EXPR_step, FAIL, 0);
'''
DRIVER = r'''
/* ==================================================================================================================== */
Src = '';
while ((Line = INPUT)) Src = Src Line nl;
rk_prescan();
if (Src ? POS(0) *g_comp_unit RPOS(0)) OUTPUT = 'Parsed.';
else OUTPUT = 'Parse Error.';
'''
if __name__ == '__main__':
    g = Gen(); body = g.generate()
    undefined = set(re.findall(r'\*(rk_m_\w+|rk_dyn_\w+|rk_var_\w+|rk_lang_\w+)', '\n'.join(body + g.helpers)))
    stubs = ['%s = FAIL;' % u for u in sorted(undefined)]
    text = PRELUDE + '\n'.join(stubs) + '\n' + '\n'.join(body) + '\n' + '\n'.join(g.helpers) + '\n' + DRIVER
    open(sys.argv[1], 'w').write(text)
    open(sys.argv[2], 'w').write('# nqp_key\tsc_name -- the census key of each Perl6/Grammar.nqp declaration and the Snocone pattern it becomes\n' + '\n'.join('%s\t%s' % kv for kv in g.map) + '\n')
    print('rules', len(body), 'helpers', len(g.helpers), 'stubs', len(stubs), 'map', len(g.map), 'bytes', len(text))
    for k, v in g.warn.most_common(40): print('  warn %-40s %d' % (k, v))
