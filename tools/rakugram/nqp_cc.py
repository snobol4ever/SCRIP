#!/usr/bin/env python3
"""nqp_cc.py -- lower Raku character classes, backslash escapes and anchors to total C.

⛔ WHY THIS FILE EXISTS. Rung 3 emitted five primitives as placeholders that SILENTLY RETURN THE
WRONG ANSWER rather than refusing:

    rk_cclass   ignored its spec and consumed any one character   -- <[0-9]> matched 'z'
    rk_esc_s    ignored its kind and consumed any one character   -- \\d matched 'z'
    rk_anchor   returned 1 always                                 -- ^^ and $$ never constrained
    rk_wb       returned 1 always                                 -- « and » never constrained
    rk_look_stub returned 1 always, operand DISCARDED             -- see nqp_emit.py

That is the exact defect nqp_emit.py's own header forbids ("a production we cannot emit REFUSES,
never returns a silent 'no match'") -- and these were worse than that, because they answered "yes,
matched". Measured over the generated parser before the cure: 182 of 518 rules touched at least one
of them, and the 60 rules containing a NEGATIVE lookahead could never match at all, since `if
(always_true) goto fail` fails unconditionally. All five kinds were also counted as MECHANICAL by
nqp_ast.py, so they inflated the port's headline "% mechanical" ladder while being placeholders.

⭐ THE RULE THIS FILE ENFORCES: a class we cannot parse REFUSES AT GENERATION TIME (returns None,
and the emitter turns that into RK_UNIMPL). It is never approximated. Parsing happens here, in
Python, once -- so the emitted C stays total: it cannot encounter a spec it does not understand.

⛔ RAKU CHARACTER-CLASS FACTS THAT ARE EASY TO GET BACKWARDS, each one measured against Grammar.nqp:
  - RANGES ARE `..`, NOT `-`. `<[1..9]>` is a range; `<[-−]>` is two literal characters and
    `<[ ' \\- ]>` is a quote and a hyphen. Reading `-` as a range operator (the POSIX habit) turns
    `<[-−]>` into a range from '-' to U+2212, silently matching thousands of characters.
  - LITERAL WHITESPACE INSIDE `<[...]>` IS INSIGNIFICANT. `<[ i g s m x c e ]>` is a set of seven
    letters, not nine members. A space must be written `\\s` or `\\x20`.
  - THE SIX SPELLINGS DIFFER IN TWO INDEPENDENT AXES, and one of them is width:
        <[a]>  <.[a]>  <+[a]>   consume one char, must be in the set
        <-[a]>                  consume one char, must NOT be in the set
        <?[a]>                  ZERO-WIDTH, must be in the set      -- must not advance
        <![a]>                  ZERO-WIDTH, must NOT be in the set  -- must not advance
    The rung-3 stub advanced for all six, so every `<?[...]>` guard also ate a character.
"""
import re

# item kinds in the emitted table
CC_RANGE, CC_CLASS = 0, 1
# mode codes shared with the C runtime (see C_RUNTIME below)
CC_IN, CC_NOTIN, CC_LOOK_IN, CC_LOOK_NOTIN = 0, 1, 2, 3

# Backslash classes NQP actually uses. Uppercase is the complement of lowercase and is handled by
# the caller, never by a second entry here.
_CLASS = {'d', 'w', 's', 'h', 'v', 'n'}
# Backslash escapes that denote one literal character.
_LITERAL = {'t': 9, 'n': 10, 'r': 13, 'f': 12, 'e': 27, '0': 0, 'a': 7, 'b': 8}


class Refuse(Exception):
    """Raised for any spec this module will not translate. Never caught to produce a guess."""


def _hexesc(s, i):
    """\\x41, \\x[41], \\x[41,42] -- returns (codepoints, next_index)."""
    if s.startswith('[', i):
        j = s.index(']', i)
        return [int(x, 16) for x in re.split(r'[,;]', s[i + 1:j]) if x.strip()], j + 1
    m = re.match(r'[0-9A-Fa-f]{1,6}', s[i:])
    if not m:
        raise Refuse(f'malformed \\x at {s[i:i + 8]!r}')
    return [int(m.group(0), 16)], i + m.end()


def _class_or_literal(ch, out):
    """Append the item denoted by a backslash escape. Returns nothing; raises Refuse if unknown."""
    low = ch.lower()
    if low in _CLASS:
        out.append((CC_CLASS, ord(low), 1 if ch.isupper() else 0))
    elif ch in _LITERAL:
        out.append((CC_RANGE, _LITERAL[ch], _LITERAL[ch]))
    elif not ch.isalnum():
        out.append((CC_RANGE, ord(ch), ord(ch)))          # \\ \- \] \| \; \' -- escaped literal
    elif ch == 'N':
        out.append((CC_CLASS, ord('n'), 1))               # \N == "not a newline"
    else:
        raise Refuse(f'unknown escape \\{ch}')


def _parse_group(body):
    """Parse the inside of one [...] group into a list of (kind, a, b) items."""
    out, i, n = [], 0, len(body)
    while i < n:
        ch = body[i]
        if ch.isspace():                                   # insignificant inside a character class
            i += 1
            continue
        if ch == '\\':
            if i + 1 >= n:
                raise Refuse('trailing backslash')
            nxt = body[i + 1]
            if nxt == 'x':
                cps, i = _hexesc(body, i + 2)
                out.extend((CC_RANGE, cp, cp) for cp in cps)
                continue
            _class_or_literal(nxt, out)
            i += 2
            continue
        # a literal character, possibly the low end of a `..` range
        lo = ord(ch)
        i += 1
        if body.startswith('..', i):
            i += 2
            while i < n and body[i].isspace():
                i += 1
            if i >= n:
                raise Refuse('range with no upper bound')
            if body[i] == '\\':
                if body[i + 1] == 'x':
                    cps, i = _hexesc(body, i + 2)
                    if len(cps) != 1:
                        raise Refuse('multi-codepoint range bound')
                    hi = cps[0]
                else:
                    raise Refuse('escape as range bound')
            else:
                hi = ord(body[i])
                i += 1
            if hi < lo:
                raise Refuse(f'inverted range {lo}..{hi}')
            out.append((CC_RANGE, lo, hi))
        else:
            out.append((CC_RANGE, lo, lo))
    if not out:
        raise Refuse('empty character class')
    return out


def parse_cclass(spec):
    """Lower a `<...>` character-class spec to (mode, items) or None if it must refuse.

    `spec` is the raw text between the angle brackets, e.g. '[a..z]', '?[\\s#]', '-[\\w]', and for
    the combining forms '+[a]-[b]'. Returns None rather than guessing -- the emitter turns that
    into RK_UNIMPL, which is a rule that visibly does not work instead of one that quietly misparses.
    """
    try:
        s = spec.strip()
        mode, neg_first = CC_IN, False
        if s[:1] == '?':
            mode, s = CC_LOOK_IN, s[1:]
        elif s[:1] == '!':
            mode, s = CC_LOOK_NOTIN, s[1:]
        elif s[:1] == '.':
            s = s[1:]
        elif s[:1] == '-':
            mode, neg_first, s = CC_NOTIN, True, s[1:]
        elif s[:1] == '+':
            s = s[1:]
        s = s.strip()
        if not s.startswith('['):
            raise Refuse('not a bracketed class')
        # walk the (possibly several) [...] groups, honouring + / - between them
        pos_items, neg_items = [], []
        sign_is_neg = neg_first
        i = 0
        while i < len(s):
            if s[i].isspace():
                i += 1
                continue
            if s[i] in '+-':
                sign_is_neg = s[i] == '-'
                i += 1
                continue
            if s[i] != '[':
                raise Refuse(f'trailing text {s[i:][:20]!r}')
            depth, j = 0, i
            while j < len(s):
                if s[j] == '\\':
                    j += 2
                    continue
                if s[j] == '[':
                    depth += 1
                elif s[j] == ']':
                    depth -= 1
                    if depth == 0:
                        break
                j += 1
            if depth != 0:
                raise Refuse('unbalanced [ ]')
            items = _parse_group(s[i + 1:j])
            (neg_items if sign_is_neg else pos_items).extend(items)
            sign_is_neg = False
            i = j + 1
        if neg_items and pos_items:
            raise Refuse('mixed +[...]-[...] set arithmetic')
        if neg_items:
            if mode != CC_IN and not neg_first:
                raise Refuse('negated group under a zero-width marker')
            return (CC_NOTIN if mode == CC_IN else mode), neg_items
        return mode, pos_items
    except Refuse:
        return None
    except Exception:
        return None


def parse_esc(text):
    """Lower a bare `\\x` escape (outside a character class) to (mode, items), or None to refuse."""
    if len(text) < 2 or text[0] != '\\':
        return None
    ch = text[1]
    out = []
    try:
        if ch == 'x':
            return None            # the body lexer drops the hex digits; refusing beats mismatching
        _class_or_literal(ch, out)
    except Refuse:
        return None
    return CC_IN, out


def c_table(name, items):
    """Render one item list as a C initialiser."""
    body = ', '.join('{%d,%d,%d}' % (k, a, b) for (k, a, b) in items)
    return f'static const RkCCItem {name}[] = {{ {body} }};'


C_RUNTIME = r'''/* ---- character classes, escapes and anchors (lowered by tools/rakugram/nqp_cc.py) ----
 * The tables are produced at GENERATION time, so these routines are TOTAL: there is no spec they
 * can fail to understand and therefore no path on which they guess. Rung 3 shipped placeholders
 * here that consumed one character and answered "matched" for every class; see nqp_cc.py.
 */
typedef struct { int kind; int a, b; } RkCCItem;   /* kind 0: codepoint range a..b · 1: class a, negated b */

static int rk_u8(const char *s, int len, int pos, int *cp) {
    if (pos >= len) return 0;
    unsigned char b0 = (unsigned char)s[pos];
    if (b0 < 0x80) { *cp = b0; return 1; }
    int need = (b0 >= 0xF0) ? 3 : (b0 >= 0xE0) ? 2 : (b0 >= 0xC0) ? 1 : -1;
    if (need < 0 || pos + need >= len) return 0;
    int v = b0 & (0x3F >> need);
    for (int k = 1; k <= need; k++) {
        unsigned char bk = (unsigned char)s[pos + k];
        if ((bk & 0xC0) != 0x80) return 0;
        v = (v << 6) | (bk & 0x3F);
    }
    *cp = v; return need + 1;
}

static int rk_cc_isclass(int code, int cp) {
    switch (code) {
        case 'd': return cp >= '0' && cp <= '9';
        case 'w': return (cp >= 'a' && cp <= 'z') || (cp >= 'A' && cp <= 'Z') ||
                         (cp >= '0' && cp <= '9') || cp == '_';
        case 's': return cp == ' ' || cp == '\t' || cp == '\n' || cp == '\r' || cp == '\f' || cp == 0x0B;
        case 'h': return cp == ' ' || cp == '\t';                       /* horizontal only */
        case 'v': return cp == '\n' || cp == '\r' || cp == '\f' || cp == 0x0B;
        case 'n': return cp == '\n' || cp == '\r';                      /* logical newline */
        default:  return 0;
    }
}

static int rk_cc_member(const RkCCItem *it, int n, int cp) {
    for (int i = 0; i < n; i++) {
        if (it[i].kind == 0) { if (cp >= it[i].a && cp <= it[i].b) return 1; }
        else { int m = rk_cc_isclass(it[i].a, cp); if (it[i].b) m = !m; if (m) return 1; }
    }
    return 0;
}

/* mode 0 in-set consume · 1 not-in-set consume · 2 in-set zero-width · 3 not-in-set zero-width */
static int rk_cc(RkCur *c, int mode, const RkCCItem *it, int n) {
    int cp, w = rk_u8(c->src, c->len, c->pos, &cp);
    if (!w) return mode == 3;      /* at end of input only the negative zero-width guard holds */
    int in = rk_cc_member(it, n, cp);
    if (mode == 2) return in;      /* zero-width: never advance */
    if (mode == 3) return !in;
    if (mode == 0 ? !in : in) return 0;
    c->pos += w; return 1;
}

/* Anchors, zero-width: ^ start-of-STRING · ^^ start-of-LINE · $ end-of-STRING · $$ end-of-LINE.
 * ⛔ ^ and ^^ are different anchors (rung 7 conflated them: ^ also held after a newline). */
static int rk_anchor(RkCur *c, const char *k) {
    int two = k[1] != 0;
    if (k[0] == '^') return c->pos == 0 || (two && c->src[c->pos - 1] == '\n');
    return c->pos >= c->len || (two && c->src[c->pos] == '\n');
}

/* « and » -- left and right word boundary. */
static int rk_wb(RkCur *c, int right) {
    int before = c->pos > 0 && rk_cc_isclass('w', (unsigned char)c->src[c->pos - 1]);
    int after  = c->pos < c->len && rk_cc_isclass('w', (unsigned char)c->src[c->pos]);
    return right ? (before && !after) : (!before && after);
}
'''


# ---- drift guard --------------------------------------------------------------------------------
# rk_cc_test.c hard-codes the tables it exercises. This asserts those tables are what THIS module
# lowers the same specs to, so the C test cannot quietly pass against a table the generator no
# longer produces. Run by test_gate_rakugram_precedence.sh; exits non-zero on the first mismatch.
SELFTEST = [
    ('[1..9]',          (CC_IN,          [(CC_RANGE, 49, 57)])),
    ('[-−]',       (CC_IN,          [(CC_RANGE, 45, 45), (CC_RANGE, 8722, 8722)])),
    ("[ ' \\- ]",       (CC_IN,          [(CC_RANGE, 39, 39), (CC_RANGE, 45, 45)])),
    ('?[\\s#]',         (CC_LOOK_IN,     [(CC_CLASS, ord('s'), 0), (CC_RANGE, 35, 35)])),
    ('![(]',            (CC_LOOK_NOTIN,  [(CC_RANGE, 40, 40)])),
    ('-[\\w]',          (CC_NOTIN,       [(CC_CLASS, ord('w'), 0)])),
    ('[ i g s m x c e ]', (CC_IN,        [(CC_RANGE, ord(ch), ord(ch)) for ch in 'igsmxce'])),
    ('[\\x0028 \\x003C]', (CC_IN,        [(CC_RANGE, 40, 40), (CC_RANGE, 60, 60)])),
    ('?[',              None),            # truncated -- 3 of these in Grammar.nqp
    ("![<]>\n || '",    None),            # brace-overrun contamination
]
SELFTEST_ESC = [
    ('\\d', (CC_IN, [(CC_CLASS, ord('d'), 0)])),
    ('\\h', (CC_IN, [(CC_CLASS, ord('h'), 0)])),
    ('\\N', (CC_IN, [(CC_CLASS, ord('n'), 1)])),
    ('\\x', None),                         # the body lexer drops the digits: refuse, never guess
]

if __name__ == '__main__':
    import sys
    if sys.argv[1:] == ['--selftest']:
        bad = 0
        for spec, want in SELFTEST:
            got = parse_cclass(spec)
            ok = got == want
            bad += not ok
            print(f"  {'ok  ' if ok else 'FAIL'} cclass {spec!r:28} -> {got}")
        for spec, want in SELFTEST_ESC:
            got = parse_esc(spec)
            ok = got == want
            bad += not ok
            print(f"  {'ok  ' if ok else 'FAIL'} esc    {spec!r:28} -> {got}")
        print('all lowerings agree with rk_cc_test.c' if not bad else f'{bad} MISMATCH(ES)')
        sys.exit(1 if bad else 0)
    if sys.argv[1:] == ['--runtime']:
        sys.stdout.write(C_RUNTIME)
        sys.exit(0)
    print("usage: nqp_cc.py --selftest | --runtime", file=sys.stderr)
    sys.exit(2)
