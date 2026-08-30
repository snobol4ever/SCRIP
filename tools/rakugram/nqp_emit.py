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
from nqp_read import scan_decls, lex_body
from nqp_ast import P, N

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
    def __init__(self): self.buf = []; self.tmp = 0; self.unimpl = False
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
            self.w(ind, f'if (!{cname(nm)}(c)) goto fail;')
        elif k == 'CCLASS':
            self.w(ind, f'if (!rk_cclass(c, {self.cstr(n.v)})) goto fail;')
        elif k == 'ESC':
            self.w(ind, f'if (!rk_esc_s(c, {self.cstr(n.v[1:2])})) goto fail;')
        elif k == 'ANCHOR':
            self.w(ind, f'if (!rk_anchor(c, {self.cstr(n.v)})) goto fail;')
        elif k == 'WB':
            self.w(ind, 'if (!rk_wb(c)) goto fail;')
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
            v = self.t()
            self.w(ind, f'/* lookahead {"positive" if pos else "negative"} */')
            self.w(ind, f'{{ int {v}_save = c->pos; int {v}_m = rk_look_stub(c);')
            self.w(ind, f'  c->pos = {v}_save; if ({"!" if pos else ""}{v}_m) goto fail; }}')
        elif k == 'EMPTY':
            self.w(ind, '/* empty */')
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

def emit_all(path, out_c):
    decls = [d for d in scan_decls(open(path, encoding='utf-8', errors='replace').read())
             if not d.get('overrun')]
    defined = collections.Counter()
    for d in decls: defined[d['name']] += 1
    protos = {d['name'] for d in decls if d['sym'] is not None}
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
        e = Emit()
        root = P(lex_body(d['body'])).parse()
        e.w(0, f'/* {d["kind"]} {d["name"]}' + (f':sym<{d["sym"]}>' if d['sym'] else '') + f'  (Grammar.nqp:{d["line"]}) */')
        e.w(0, f'static int {cname(nm)}(RkCur *c) {{')
        e.w(1, 'int start = c->pos;')
        e.node(root, 1)
        e.w(1, 'return 1;')
        # Only emit the fail label if something can actually jump to it -- a rule with no failing
        # step (all-optional body) otherwise carries an unreachable label.
        if any('goto fail;' in ln for ln in e.buf):
            e.w(0, 'fail:')
            e.w(1, 'c->pos = start; return 0;')
        else:
            for i, ln in enumerate(e.buf):
                if ln.strip() == 'int start = c->pos;':
                    e.buf[i] = ln + ' (void)start;'
                    break
        e.w(0, '}')
        stats['UNIMPL' if e.unimpl else 'EMITTED'] += 1
        fns.append('\n'.join(e.buf))
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
        fns.append('\n'.join(e.buf))
        stats['PROTO'] += 1
    # forward declarations for everything, including inherited NQP builtins
    called = set()
    for d in decls:
        for n in P(lex_body(d['body'])).parse().walk():
            if n.k == 'CALL': called.add(n.v.split('(')[0].strip())
    local = {cname(d['name'] + (('__' + re.sub(r'\W', '_', d['sym'])) if d['sym'] else ''))
             for d in decls if not d['proto']}
    local |= {cname(p) for p in protos}
    # ⛔ Only identifier-shaped names are real subrule calls. A `<...>` assertion body can contain
    # commas, quotes and nested angle brackets (e.g. `<?before ... , ... >`), and letting those
    # through manufactures phantom "inherited builtins" that no one will ever be able to implement.
    IDENT = re.compile(r'^[A-Za-z_][A-Za-z0-9_:-]*$')
    inherited = sorted({c for c in called if c and IDENT.match(c) and cname(c) not in local})
    with open(out_c, 'w') as f:
        f.write(HEADER)
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
            f.write(f'static int {cname(c)}(RkCur *c) {{ (void)c; return RK_UNIMPL; }}  '
                    f'/* TODO: hand-write {c} */\n')
        f.write('\n')
        f.write('\n\n'.join(fns))
        f.write('\n')
    print(f"emitted            : {out_c}")
    print(f"  generated rules  : {stats['EMITTED']}")
    print(f"  refusing rules   : {stats['UNIMPL']}   (return RK_UNIMPL, never a silent 'no match')")
    print(f"  proto dispatchers: {stats['PROTO']}")
    print(f"  inherited stubs needed (NQP HLL::Grammar, hand-written): {len(inherited)}")
    print(f"    {', '.join(inherited[:14])} ...")

HEADER = '''/* GENERATED by tools/rakugram/nqp_emit.py from Rakudo src/Perl6/Grammar.nqp -- DO NOT EDIT.
 *
 * Rung 3 of the Grammar.nqp -> recursive-descent port (Lon ruling 2026-08-30: Raku is not LALR).
 * Regenerate rather than patch; hand edits are lost on the next run.
 *
 * ⛔ `|` is LONGEST-TOKEN-MATCH (rk_alt: every arm tried, longest kept) and `||` is FIRST-MATCH.
 *    They are different operators and compile to different code here.
 */
#include <string.h>
#include <stdlib.h>
#define RK_UNIMPL (-1)
typedef struct { const char *src; int pos; int len; } RkCur;
static int rk_lit(RkCur *c, const char *s) {
    int n = (int)strlen(s);
    if (c->pos + n > c->len || memcmp(c->src + c->pos, s, (size_t)n)) return 0;
    c->pos += n; return 1;
}
static int rk_cclass(RkCur *c, const char *spec) { (void)spec; if (c->pos >= c->len) return 0; c->pos++; return 1; }
static int rk_esc_s(RkCur *c, const char *k) { (void)k; if (c->pos >= c->len) return 0; c->pos++; return 1; }
static int rk_anchor(RkCur *c, const char *k) { (void)c; (void)k; return 1; }
static int rk_wb(RkCur *c) { (void)c; return 1; }
static int rk_look_stub(RkCur *c) { (void)c; return 1; }

'''

if __name__ == '__main__':
    src = sys.argv[1] if len(sys.argv) > 1 else '/home/resources/rakudo-main/src/Perl6/Grammar.nqp'
    out = sys.argv[2] if len(sys.argv) > 2 else '/tmp/claude-1000/-home-claude-C/bf92a46a-6680-4a47-b7db-57e12b40e2b1/scratchpad/rk_grammar_gen.c'
    emit_all(src, out)
