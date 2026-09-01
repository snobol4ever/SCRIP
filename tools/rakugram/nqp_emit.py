#!/usr/bin/env python3
"""nqp_emit.py -- emit a C recursive-descent parser from Grammar.nqp's productions.

RUNG 3 of the port. Rung 1 read the grammar, rung 2 proved 87.9% of it is shape-translatable;
this turns that shape into C.

⛔ THE ONE THING THIS FILE MUST GET RIGHT: in Raku regex `|` is LONGEST-TOKEN-MATCH and `||` is
FIRST-MATCH. They compile to DIFFERENT code here (rk_alt_ltm tries every arm and keeps the longest;
ordered choice returns the first that matches). Emitting `|` as ordered choice is the idiomatic
recursive-descent reading and is silently wrong -- correct on most input, wrong exactly where a later
alternative matches longer. `|` outnumbers `||` in Grammar.nqp.

A production we cannot emit becomes a function that REFUSES (returns RK_UNIMPL), never one that
returns 0/"no match" -- an unimplemented rule that reports "did not match" is indistinguishable from
a rule that ran and correctly declined, and would make the parser confidently wrong.
"""
import sys, re, os, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from nqp_read import scan_decls, lex_body, in_grammar
from nqp_ast import P, N, look_operand, lowers, modelled_dynvars, const_value, _MYDECL, _ASSIGN, MODELLED_DYNVARS
import nqp_cc

# panic-class diagnostics inside a {…} block: these ABORT in NQP; here they fail the arm (see Emit.node).
DIAG_FAIL = re.compile(r'\b(panic|typed_panic|obs|malformed|missing|NYI|fail-terminator|die)\b')   # fail-terminator: unterminated quote (quibble/sibble/tribble); nqp::die

_CC_TABLES = {}          # items-tuple -> C table name, so identical classes share one table
def cc_table(items):
    key = tuple(items)
    if key not in _CC_TABLES: _CC_TABLES[key] = f'rk_cc{len(_CC_TABLES)}'
    return _CC_TABLES[key]

_CN = {}
def cname(s):
    base = 'rk_' + re.sub(r'[^A-Za-z0-9_]', '_', s)
    if s in _CN: return _CN[s]
    if base in _CN.values():
        n = 2
        while f'{base}_{n}' in _CN.values(): n += 1
        base = f'{base}_{n}'
    _CN[s] = base
    return base

class Emit:
    def __init__(self, sym=None): self.buf = []; self.tmp = 0; self.unimpl = False; self.sym = sym; self.uses_dyn = False
    def w(self, ind, s): self.buf.append('    ' * ind + s)
    def t(self): self.tmp += 1; return f"t{self.tmp}"

    def node(self, n, ind):
        """Emit code that matches n. Convention: on success advance c->pos and fall through;
        on failure `goto fail` with c->pos restored by the caller's save."""
        k = n.k
        if k == 'SEQ':
            for kid in n.kids: self.node(kid, ind)
        elif k == 'LIT':
            self.w(ind, f'if (!rk_lit(c, {self.cstr(n.v)})) goto fail;')
        elif k == 'CALL':
            nm = n.v.split('(')[0]
            if nm == 'sym':
                # ⛔ `<sym>` is NOT an inherited rule to hand-write -- it is 22 of the 48 "missing"
                # names and every one is a false positive. Inside `token infix:sym<*> { <sym> ... }`
                # it means "match THIS candidate's own literal", so it is per-declaration data the
                # emitter already holds. Emitting a call to a shared rk_sym() would be a single
                # function that has to match 516 different literals and cannot know which.
                if self.sym is not None:
                    self.w(ind, f'if (!rk_lit(c, {self.cstr(self.sym)})) goto fail;')
                    return
                self.unimpl = True
                self.w(ind, '/* <sym> outside a :sym<> candidate */ return RK_UNIMPL;')
                return
            self.w(ind, f'if (!{cname(nm)}(c)) goto fail;')
        elif k in ('CCLASS', 'ESC'):
            # ⛔ Lowered at GENERATION time (nqp_cc.py). A spec that will not parse REFUSES here
            # rather than reaching a runtime routine that would have to guess. Rung 3's stub
            # ignored the spec entirely and consumed one character, so <[0-9]> matched 'z'.
            r = nqp_cc.parse_cclass(n.v) if k == 'CCLASS' else nqp_cc.parse_esc(n.v)
            if r is None:
                self.unimpl = True
                self.w(ind, f'/* UNIMPLEMENTED {k} {str(n.v)[:40]!r} */ return RK_UNIMPL;')
                return
            mode, items = r
            tbl = cc_table(items)
            self.w(ind, f'if (!rk_cc(c, {mode}, {tbl}, {len(items)})) goto fail;')
        elif k == 'ANCHOR':
            self.w(ind, f'if (!rk_anchor(c, {self.cstr(n.v)})) goto fail;')
        elif k == 'WB':
            self.w(ind, f'if (!rk_wb(c, {1 if n.v == chr(0xBB) else 0})) goto fail;')
        elif k == 'CAP':
            for kid in n.kids: self.node(kid, ind)
        elif k == 'ALT_ORD':
            v = self.t()
            self.w(ind, f'int {v}_save = c->pos; int {v}_ok = 0;')
            for i, arm in enumerate(n.kids):
                self.w(ind, f'if (!{v}_ok) {{ c->pos = {v}_save;')
                self.sub(arm, ind + 1, f'{v}_ok = 1;')
                self.w(ind, '}')
            self.w(ind, f'if (!{v}_ok) goto fail;')
        elif k == 'ALT_LTM':
            # ⛔ longest-token-match: EVERY arm is tried and the longest win is kept.
            v = self.t()
            self.w(ind, f'int {v}_save = c->pos; int {v}_best = -1;')
            for arm in n.kids:
                self.w(ind, f'{{ c->pos = {v}_save;')
                self.sub(arm, ind + 1, f'if (c->pos > {v}_best) {v}_best = c->pos;')
                self.w(ind, '}')
            self.w(ind, f'if ({v}_best < 0) {{ c->pos = {v}_save; goto fail; }}')
            self.w(ind, f'c->pos = {v}_best;')
        elif k == 'QUANT':
            q = n.v
            v = self.t()
            if q.startswith('?'):
                self.w(ind, f'{{ int {v}_save = c->pos;')
                self.sub(n.kids[0], ind + 1, '', on_fail=f'c->pos = {v}_save;')
                self.w(ind, '}')
            else:
                minimum = 1 if q.startswith('+') else 0
                self.w(ind, f'{{ int {v}_n = 0; for (;;) {{ int {v}_save = c->pos;')
                self.sub(n.kids[0], ind + 2, f'{v}_n++; if (c->pos == {v}_save) break; continue;',
                         on_fail=f'c->pos = {v}_save; break;')
                self.w(ind, f'}} if ({v}_n < {minimum}) goto fail; }}')
        elif k == 'LOOK':
            pos = n.v.startswith('pos')
            sub = look_operand(n.v.split(':', 1)[1])
            if sub is None:
                self.unimpl = True
                self.w(ind, f'/* UNIMPLEMENTED LOOK {n.v[:44]!r} */ return RK_UNIMPL;')
                return
            while sub.k == 'NOT':                       # <!!{…}>: each extra ! flips the polarity
                pos = not pos; sub = sub.kids[0]
            if sub.k == 'DYNREAD':
                name, neg = sub.v
                want_true = (not neg) == pos          # <?{ $*X }> holds iff X true; <!{ $*X }> iff false; ! inside flips
                v = self.t()
                self.w(ind, f'{{ int {v} = rk_dyn_truthy(c, {self.cstr(name)}); if ({v} < 0) return rk_refuse(c, "read of unbound {name}"); if ({"!" if want_true else ""}{v}) goto fail; }}   /* <{"?" if pos else "!"}{{ {"!" if neg else ""}{name} }}> */')
                return
            if sub.k == 'MARK':
                op, name = sub.v
                call = f'rk_mark_{"set" if op == "set" else "test"}(c, {self.cstr(name)})'
                self.w(ind, f'if ({"!" if pos else ""}{call}) goto fail;   /* <{"?" if pos else "!"}{"MARKER" if op == "set" else "MARKED"}({name!r})> -- zero-width memo */')
                return
            if sub.k in ('EMPTY', 'FAILN'):
                # a CONSTANT code assertion (block ends in literal 1/0): decided at generation time
                holds = (sub.k == 'EMPTY') == pos
                self.w(ind, f'/* constant code assertion {self.short(n.v)}: {"always holds -- no-op" if holds else "never holds -- arm fails"} */' + ('' if holds else ' goto fail;'))
                return
            v = self.t()
            self.w(ind, f'/* lookahead {"positive" if pos else "negative"} -- zero-width */')
            self.w(ind, f'{{ int {v}_save = c->pos; int {v}_m = 0;')
            self.sub(sub, ind + 1, f'{v}_m = 1;')
            self.w(ind, f'  c->pos = {v}_save; if ({"!" if pos else ""}{v}_m) goto fail; }}')
        elif k == 'EMPTY':
            self.w(ind, '/* empty */')
        elif k == 'MARK':
            op, name = n.v
            if op == 'set': self.w(ind, f'if (!rk_mark_set(c, {self.cstr(name)})) goto fail;   /* MARKER: always succeeds unless the table overflows (refuses) */')
            else:           self.w(ind, f'if (!rk_mark_test(c, {self.cstr(name)})) goto fail;  /* MARKED: recorded position == current */')
        elif k == 'FAILN':
            self.w(ind, f'/* {self.short(n.v)} */ goto fail;')
        elif k == 'MY':
            # A parse-time local consumes nothing and cannot fail. If EVERY write to it in the grammar is a
            # literal (nqp_ast.modelled_dynvars), it is PUSHED here with its literal and unwound at every exit
            # of this rule; otherwise it stays a no-op and every read of it refuses (the guard).
            m = _MYDECL.match(str(n.v).strip())
            if m and m.group(1) in MODELLED_DYNVARS:
                self.uses_dyn = True
                self.w(ind, f'if (!rk_dyn_push(c, {self.cstr(m.group(1))}, {self.cstr(const_value(m.group(2)))})) goto fail;   /* :my (modelled) */')
            else:
                self.w(ind, f'/* :my {self.short(n.v)} -- parse-time local: no-op for matching; reads of it refuse */')
        elif k == 'CODE':
            t = str(n.v)
            if DIAG_FAIL.search(t):
                # panic-class diagnostics: rung 6 makes <.panic> FAIL THE ARM (rk_panic returns 0). The
                # code-block spelling gets the same semantics so the two cannot disagree. ⚠ Known
                # divergence, recorded on the baton: NQP's panic ABORTS the parse; failing the arm can let
                # an enclosing alternation accept a program Rakudo rejects. Over-accept only; never
                # over-reject.
                self.w(ind, f'/* {{code}} panic-class {self.short(t)} -- arm fails (rung-6 stub semantics) */ goto fail;')
            else:
                self.w(ind, f'/* {{code}} {self.short(t)} -- action block: no-op for matching */')
                for m in _ASSIGN.finditer(t):          # writes to MODELLED variables are real: set in place
                    if m.group(1) in MODELLED_DYNVARS:
                        self.w(ind, f'if (!rk_dyn_set(c, {self.cstr(m.group(1))}, {self.cstr(const_value(m.group(2)))})) goto fail;   /* {{code}} write to a modelled $* */')
        elif k == 'MOD':
            if str(n.v).lstrip().startswith(':dba'):
                self.w(ind, f'/* {self.short(n.v)} -- diagnostic label, no-op */')
            else:
                self.unimpl = True
                self.w(ind, f'/* UNIMPLEMENTED MOD {self.short(n.v)} -- :i/:s change what matches; not modelled */ return RK_UNIMPL;')
        else:
            self.unimpl = True
            self.w(ind, f'/* UNIMPLEMENTED {k} {str(n.v)[:40]!r} */ return RK_UNIMPL;')

    def sub(self, n, ind, on_ok, on_fail=None):
        """Emit n in an isolated success/failure island (its own fail label)."""
        v = self.t()
        self.w(ind, f'do {{')
        saved, self.buf = self.buf, []
        self.node(n, ind + 1)
        body, self.buf = self.buf, saved
        body = [ln.replace('goto fail;', f'goto {v}_f;') for ln in body]
        self.buf.extend(body)
        if on_ok: self.w(ind + 1, on_ok)
        self.w(ind + 1, f'goto {v}_d;')
        self.w(ind, f'}} while (0);')
        self.w(ind, f'{v}_f: ; {on_fail or ""}')
        self.w(ind, f'{v}_d: ;')

    def short(self, v):
        """One-line, comment-safe excerpt: no newlines, no `*/`, at most 44 chars."""
        t = re.sub(r'\s+', ' ', str(v)).replace('*/', '* /').strip()
        return repr(t[:44] + ('…' if len(t) > 44 else ''))

    def cstr(self, s):
        out = []
        for ch in str(s):
            if   ch == '\\': out.append('\\\\')
            elif ch == '"':  out.append('\\"')
            elif ch == '\n': out.append('\\n')
            elif ch == '\r': out.append('\\r')
            elif ch == '\t': out.append('\\t')
            elif ord(ch) < 32: out.append('\\%03o' % ord(ch))
            elif ord(ch) > 126: out.extend('\\%03o' % b for b in ch.encode('utf-8'))
            else: out.append(ch)
        return '"' + ''.join(out) + '"'

def emit_all(path, out_c, provided=(), grammar='Perl6::Grammar'):
    decls = [d for d in scan_decls(open(path, encoding='utf-8', errors='replace').read())
             if not d.get('overrun')]
    nall = len(decls)
    decls = in_grammar(decls, grammar)      # ⛔ one language: the grammar plus the roles it does (see nqp_read)
    md = modelled_dynvars(decls)          # ⛔ before any rule is emitted: MY/CODE/LOOK all consult it
    defined = collections.Counter()
    for d in decls: defined[d['name']] += 1
    # ⛔ a proto is a proto because it is DECLARED one, not because candidates exist: `proto token
    # prefix_circumfix_meta_operator { <...> }` has ZERO candidates in this Rakudo, and inferring protos from
    # their :sym<> candidates left it with no dispatcher -- so the name fell into the inherited-stub list and
    # REFUSED, where Rakudo simply fails to match (no candidate). The dispatcher's empty loop yields exactly that.
    protos = {d['name'] for d in decls if d['sym'] is not None} | {d['name'] for d in decls if d['proto']}
    fns, stats = [], collections.Counter()
    seen = set()
    for d in decls:
        # A `proto token foo { <...> }` has no body of its own -- `<...>` means "dispatch to my
        # candidates". Its NAME belongs to the LTM dispatcher emitted below, so emitting it here
        # too produces a duplicate C function.
        if d['proto']: continue
        nm = d['name'] + (('__' + re.sub(r'\W', '_', d['sym'])) if d['sym'] else '')
        if nm in seen: continue
        seen.add(nm)
        e = Emit(d['sym'])
        root = P(lex_body(d['body'])).parse()
        e.w(0, f'/* {d["kind"]} {d["name"]}' + (f':sym<{d["sym"]}>' if d['sym'] else '') + f'  (Grammar.nqp:{d["line"]}) */')
        e.w(0, f'static int {cname(nm)}(RkCur *c) {{')
        e.w(1, 'int start = c->pos;')
        e.node(root, 1)
        if e.uses_dyn:                                   # dynamic scope ends with the rule, on BOTH exits
            e.buf.insert(e.buf.index('    int start = c->pos;') + 1, '    int dyn_mark = c->ndyn;')
            e.w(1, 'c->ndyn = dyn_mark; return 1;')
        else:
            e.w(1, 'return 1;')
        # Only emit the fail label if something can actually jump to it -- a rule with no failing
        # step (all-optional body) otherwise carries an unreachable label.
        if any('goto fail;' in ln for ln in e.buf):
            e.w(0, 'fail:')
            e.w(1, 'c->ndyn = dyn_mark; c->pos = start; return 0;' if e.uses_dyn else 'c->pos = start; return 0;')
        else:
            for i, ln in enumerate(e.buf):
                if ln.strip() == 'int start = c->pos;':
                    e.buf[i] = ln + ' (void)start;'
                    break
        e.w(0, '}')
        stats['UNIMPL' if e.unimpl else 'EMITTED'] += 1
        # ⛔ a refusal names its rule and propagates through rk_refuse(); a bare `return RK_UNIMPL`
        # would be read as a MATCH by every caller (-1 is truthy at `if (!rk_x(c))`). There are TWO
        # append sites in this function; the first patch converted only the other one.
        fns.append('\n'.join(ln.replace('return RK_UNIMPL;', f'return rk_refuse(c, "{nm}");') for ln in e.buf))
    # protoregex dispatchers: longest-token-match over the family's candidates
    for pn in sorted(protos):
        cands = [d for d in decls if d['name'] == pn and d['sym'] is not None]
        e = Emit()
        e.w(0, f'/* proto {pn} -- LTM dispatch over {len(cands)} candidate(s) */')
        e.w(0, f'static int {cname(pn)}(RkCur *c) {{')
        e.w(1, 'int save = c->pos, best = -1;')
        for d in cands:
            fn = cname(d['name'] + '__' + re.sub(r'\W', '_', d['sym']))
            e.w(1, f'c->pos = save; if ({fn}(c) && c->pos > best) best = c->pos;')
        e.w(1, 'if (best < 0) { c->pos = save; return 0; }')
        e.w(1, 'c->pos = best; return 1;')
        e.w(0, '}')
        # ⛔ a refusal names its rule and propagates through rk_refuse(); `return RK_UNIMPL` would be
        # read as a MATCH by every caller (-1 is truthy at `if (!rk_x(c))`).
        fns.append('\n'.join(ln.replace('return RK_UNIMPL;', f'return rk_refuse(c, "{pn}");') for ln in e.buf))
        stats['PROTO'] += 1
    # forward declarations for everything, including inherited NQP builtins
    called = set()
    for d in decls:
        nodes = list(P(lex_body(d['body'])).parse().walk())
        # ⛔ expand lookahead operands with the SAME resolver the emitter uses -- a LOOK node keeps
        # its operand as text, so walk() alone never reaches the subrules inside it.
        for n in list(nodes):
            if n.k == 'LOOK':
                sub = look_operand(n.v.split(':', 1)[1])
                if sub is not None: nodes.extend(sub.walk())
        for n in nodes:
            if n.k != 'CALL': continue
            nm = n.v.split('(')[0].strip()
            # `<sym>` is inlined as this candidate's own literal (see Emit.node), so it is never a
            # call and must not appear in the inherited list. Measured: 385 of 385 declarations using
            # <sym> carry a :sym<> value and ZERO do not, so the guard below never falls through --
            # it is kept as an assertion of that, not as a fallback.
            if nm == 'sym' and d['sym'] is not None: continue
            called.add(nm)
    local = {cname(d['name'] + (('__' + re.sub(r'\W', '_', d['sym'])) if d['sym'] else ''))
             for d in decls if not d['proto']}
    local |= {cname(p) for p in protos}
    # ⛔ Only identifier-shaped names are real subrule calls. A `<...>` assertion body can contain
    # commas, quotes and nested angle brackets (e.g. `<?before ... , ... >`), and letting those
    # through manufactures phantom "inherited builtins" that no one will ever be able to implement.
    IDENT = re.compile(r'^[A-Za-z_][A-Za-z0-9_:-]*$')
    inherited = sorted({c for c in called if c and IDENT.match(c) and cname(c) not in local and c not in set(provided)})
    with open(out_c, 'w') as f:
        f.write(HEADER)
        if _CC_TABLES:
            f.write('/* ---- character-class tables (shared; identical classes collapse) ---- */\n')
            for items, nm in _CC_TABLES.items():
                f.write(nqp_cc.c_table(nm, items) + '\n')
            f.write('\n')
        f.write('/* ---- forward declarations ---- */\n')
        for nm in sorted(local): f.write(f'static int {nm}(RkCur *);\n')
        f.write('\n/* ---- inherited from NQP HLL::Grammar ----\n'
                ' * These are NOT part of Grammar.nqp and cannot be generated from it. Each is emitted\n'
                ' * here as a REFUSING stub so the translation unit compiles and links while the port\n'
                ' * proceeds; every one must be hand-written before the parser can accept real input.\n'
                ' * EXPR and O are the operator-precedence parser and its metadata -- the component that\n'
                ' * carries Raku\'s PARSE-TIME-EXTENSIBLE operator table, and the reason the language is\n'
                ' * not LALR. They are the largest single item of remaining work, not a stub to fill in.\n'
                ' */\n')
        for c in inherited:
            f.write(f'static int {cname(c)}(RkCur *c) {{ return rk_refuse(c, "{c}"); }}  '
                    f'/* TODO: hand-write {c} */\n')
        f.write('\n')
        f.write('\n\n'.join(fns))
        f.write('\n')
    print(f"emitted            : {out_c}")
    print(f"  grammar          : {grammar} + roles it does -- {len(decls)} of {nall} declarations in the file")
    print(f"  generated rules  : {stats['EMITTED']}")
    print(f"  refusing rules   : {stats['UNIMPL']}   (return RK_UNIMPL, never a silent 'no match')")
    print(f"  proto dispatchers: {stats['PROTO']}")
    print(f"  character-class tables: {len(_CC_TABLES)}   (lowered by nqp_cc.py, no runtime guessing)")
    print(f"  modelled parse-time variables: {len(md)}   (every write a literal) -> {', '.join(sorted(md))}")
    print(f"  inherited stubs needed (NQP HLL::Grammar, hand-written): {len(inherited)}")
    print(f"    {', '.join(inherited[:14])} ...")

HEADER = '''/* GENERATED by tools/rakugram/nqp_emit.py from Rakudo src/Perl6/Grammar.nqp -- DO NOT EDIT.
 *
 * Rung 3 of the Grammar.nqp -> recursive-descent port (Lon ruling 2026-08-30: Raku is not LALR).
 * Regenerate rather than patch; hand edits are lost on the next run.
 *
 * ⛔ `|` is LONGEST-TOKEN-MATCH (rk_alt: every arm tried, longest kept) and `||` is FIRST-MATCH.
 *    They are different operators and compile to different code here.
 *
 * ⛔ CHARACTER CLASSES, ESCAPES, ANCHORS AND LOOKAHEAD ARE REAL, NOT PLACEHOLDERS. Rung 3 emitted
 *    all five as stubs that answered "matched" for any input; a class we still cannot lower now
 *    REFUSES (RK_UNIMPL) instead. Lowering happens at generation time in tools/rakugram/nqp_cc.py,
 *    which is why the routines below are total.
 */
#include <string.h>
#include <stdlib.h>
#include "rk_cur.h"        /* the ONE RkCur + rk_refuse(): a refusal propagates, it never reads as a match */
static int rk_lit(RkCur *c, const char *s) {
    int n = (int)strlen(s);
    if (c->pos + n > c->len || memcmp(c->src + c->pos, s, (size_t)n)) return 0;
    c->pos += n; return 1;
}
''' + nqp_cc.C_RUNTIME + '''
'''

if __name__ == '__main__':
    args = [x for x in sys.argv[1:] if not x.startswith('--')]
    prov = [x[len('--provided='):] for x in sys.argv[1:] if x.startswith('--provided=')]
    gram = [x[len('--grammar='):] for x in sys.argv[1:] if x.startswith('--grammar=')]
    provided = tuple(n for p in prov for n in p.split(',') if n)     # rules the glue / HLL layer define: no stub
    src = args[0] if len(args) > 0 else '/home/resources/rakudo-main/src/Perl6/Grammar.nqp'
    out = args[1] if len(args) > 1 else '/tmp/rk_grammar_gen.c'
    emit_all(src, out, provided, gram[-1] if gram else 'Perl6::Grammar')
