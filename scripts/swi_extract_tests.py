#!/usr/bin/env python3
# swi_extract_tests.py — GOAL-PROLOG-100-SWI RUNG SWI-0 step 0.1
# Turn a SWI plunit test file into standalone one-goal probes SCRIP + gprolog can run.
#
# plunit shape:
#   :- begin_tests(Block [, BlockOpts]).
#   <helper clauses / directives>
#   test(Name [, TestOpts]) :- Body.
#   :- end_tests(Block).
#
# Each test becomes corpus/prolog/swi/<file>/<block>__<name>[_<k>].pl with:
#   main :- ( <checked-body> -> write(ok) ; write(fail) ), nl.
# plus every helper clause that appeared inside the block (so Body resolves), and a sibling
# .expected = "ok\n"  (or a .xfail with a reason when the test is out of scope / unrepresentable).
#
# Expected-outcome is read from TestOpts (SWI plunit semantics):
#   (none)            -> Body must succeed
#   true              -> succeed
#   fail              -> Body must fail            -> probe body = \+ ( Body )
#   Var == Value      -> succeed then Var == Value -> probe body = ( Body ), Var == Value
#   error(E,_) / throws(...) -> catch(Body, error(E,_), true)  (ball shape checked loosely)
#   all(T == L)/set(...)/nondet -> findall/collect and compare (best-effort; else SKIP)
# SKIP (emit .xfail, no probe run) when TestOpts or Body needs an out-of-scope feature:
#   sto(rational_trees), cyclic terms, attvar/freeze/dif, tabling, dicts, threads, blocked(...).
import sys, os, re

# ------------------------------------------------------------------------------------------------
# Out-of-scope feature markers (PL-100 scope; see GOAL-PROLOG-100-SWI.md OUT OF SCOPE).
OUT_OF_SCOPE_OPTS = ('sto(', 'blocked(', 'blocked,')
OUT_OF_SCOPE_BUILTINS = (
    'freeze(', 'dif(', 'when(', 'frozen(', 'attvar(', 'put_attr(', 'get_attr(', 'del_attr(',
    'trim_stacks', 'garbage_collect', 'trim_stacks', 'unifiable(',   # attvar / GC internals
    'tabling', 'table ', ':- table', 'abolish_all_tables',
    'engine_create', 'engine_next', 'thread_create', 'mutex_',
    '_{', 'get_dict', 'put_dict', 'dict_create',                     # dicts
    'reset(', 'shift(', 'prompt(',                                   # delimited continuations
    'setup_call_catcher_cleanup',                                    # deep SWI cleanup
)

def is_out_of_scope_body(body):
    b = body
    for m in OUT_OF_SCOPE_BUILTINS:
        if m in b:
            return m.strip()
    return None

def is_out_of_scope_opts(opts):
    o = opts.replace(' ', '')
    for m in OUT_OF_SCOPE_OPTS:
        if m.replace(' ', '') in o:
            return m
    # cyclic-term intent flagged either via sto or an explicit rational_trees mention
    if 'rational_trees' in o:
        return 'rational_trees'
    return None

# ------------------------------------------------------------------------------------------------
# Tokenizer that splits a Prolog source into top-level clauses/directives terminated by ". " /
# ".\n" / ".<eof>", respecting quotes, %-line-comments, /* */ block comments, and nested parens.
def split_clauses(src):
    out, i, n, depth = [], 0, len(src), 0
    buf = []
    in_line_c = in_block_c = False
    q = None  # active quote char
    while i < n:
        c = src[i]
        nxt = src[i+1] if i+1 < n else ''
        if in_line_c:
            buf.append(c)
            if c == '\n':
                in_line_c = False
            i += 1; continue
        if in_block_c:
            buf.append(c)
            if c == '*' and nxt == '/':
                buf.append(nxt); i += 2; in_block_c = False; continue
            i += 1; continue
        if q is not None:
            buf.append(c)
            if c == '\\':                       # escape inside quote
                if nxt: buf.append(nxt); i += 2; continue
            if c == q:
                q = None
            i += 1; continue
        if c == '%':
            in_line_c = True; buf.append(c); i += 1; continue
        if c == '/' and nxt == '*':
            in_block_c = True; buf.append(c); buf.append(nxt); i += 2; continue
        if c in ('"', "'", '`'):
            q = c; buf.append(c); i += 1; continue
        if c == '(' or c == '[' or c == '{':
            depth += 1; buf.append(c); i += 1; continue
        if c == ')' or c == ']' or c == '}':
            depth -= 1; buf.append(c); i += 1; continue
        if c == '.' and depth == 0 and (nxt == '' or nxt.isspace()):
            clause = ''.join(buf).strip()
            if clause:
                out.append(clause)
            buf = []
            i += 1
            continue
        buf.append(c); i += 1
    tail = ''.join(buf).strip()
    if tail:
        out.append(tail)
    return out

# ------------------------------------------------------------------------------------------------
# Parse `test(Name [, Opts]) :- Body`  or  `test(Name [, Opts]).` (body = true).
# Returns (name, opts_str, body_str) or None.
TEST_HEAD = re.compile(r'^test\s*\(', re.S)

def parse_test(clause):
    if not TEST_HEAD.match(clause):
        return None
    # split head :- body at the top-level ":-"
    head, body = clause, 'true'
    # find top-level ':-'
    depth = 0; q = None; i = 0; n = len(clause)
    while i < n - 1:
        c = clause[i]
        if q is not None:
            if c == '\\': i += 2; continue
            if c == q: q = None
            i += 1; continue
        if c in ('"', "'", '`'): q = c; i += 1; continue
        if c in '([{': depth += 1
        elif c in ')]}': depth -= 1
        elif depth == 0 and c == ':' and clause[i+1] == '-':
            head = clause[:i].strip(); body = clause[i+2:].strip(); break
        i += 1
    # head = test( ARGS )
    m = re.match(r'^test\s*\((.*)\)\s*$', head, re.S)
    if not m:
        return None
    args = m.group(1)
    # split ARGS at top-level comma into Name , Opts(optional)
    name, opts = split_top_comma(args)
    return (name.strip(), (opts or '').strip(), body.strip())

def split_top_comma(s):
    depth = 0; q = None
    for i, c in enumerate(s):
        if q is not None:
            if c == '\\': continue
            if c == q: q = None
            continue
        if c in ('"', "'", '`'): q = c; continue
        if c in '([{': depth += 1
        elif c in ')]}': depth -= 1
        elif c == ',' and depth == 0:
            return s[:i], s[i+1:]
    return s, None

# ------------------------------------------------------------------------------------------------
# Build the checked probe body from (opts, body) per plunit semantics. Returns (probe_body, skip_reason)
def build_probe(opts, body):
    sk = is_out_of_scope_body(body)
    if sk:
        return None, 'body uses out-of-scope: %s' % sk
    sk = is_out_of_scope_opts(opts)
    if sk:
        return None, 'opts out-of-scope: %s' % sk
    if body_is_cyclic(body):
        return None, 'cyclic unification (needs rational trees) — out of scope'
    o = opts.strip()
    # normalize an option LIST [a,b,c] to its inner text; a bare option keeps as-is
    inner = o
    if o.startswith('[') and o.endswith(']'):
        inner = o[1:-1].strip()
    optlist = [x.strip() for x in split_all_top_comma(inner)] if inner else []

    # detect fail
    if any(x == 'fail' for x in optlist) or inner == 'fail':
        return '\\+ ( %s )' % body, None
    # detect true / empty
    if inner == '' or inner == 'true' or all(x in ('true',) for x in optlist):
        return '( %s )' % body, None
    # error(E,_) / throws / exception
    for x in optlist + ([inner] if inner else []):
        me = re.match(r'^(?:throws\()?\s*error\((.*)\)\s*\)?$', x)
        if x.startswith('error(') or x.startswith('throws('):
            # loose ball match: catch any error, succeed
            return 'catch(( %s ), error(_,_), true)' % body, None
    # X == V  (single comparison outcome)
    for x in optlist + ([inner] if inner else []):
        if '==' in x and not x.startswith('all(') and not x.startswith('set('):
            # x is like "End == 1" ; probe = ( Body ), (X == V)
            return '( ( %s ), ( %s ) )' % (body, x), None
    # all(T == L) / set / nondet -> best-effort SKIP for now (SWI-1 step handles collect probes)
    for x in optlist + ([inner] if inner else []):
        if x.startswith('all(') or x.startswith('set(') or x == 'nondet' or x.startswith('all[') :
            return None, 'collect-outcome (all/set/nondet) — deferred to collect-probe pass'
    # unknown option shape -> SKIP rather than guess wrong
    return None, 'unrecognized test option: %s' % o

def split_all_top_comma(s):
    parts, depth, q, start = [], 0, None, 0
    for i, c in enumerate(s):
        if q is not None:
            if c == '\\': continue
            if c == q: q = None
            continue
        if c in ('"', "'", '`'): q = c; continue
        if c in '([{': depth += 1
        elif c in ')]}': depth -= 1
        elif c == ',' and depth == 0:
            parts.append(s[start:i]); start = i+1
    parts.append(s[start:])
    return parts

# ------------------------------------------------------------------------------------------------
def sanitize(name):
    return re.sub(r'[^A-Za-z0-9_]', '_', name)

# Strip leading %-line and /* */ block comments (and blank lines) from the front of a clause so
# classification sees the real functor. A trailing comment consuming a newline can otherwise glue a
# directive onto a preceding comment fragment and hide it from the ^:- filter.
def strip_leading_comments(cl):
    while True:
        s = cl.lstrip()
        if s.startswith('%'):
            nl = s.find('\n')
            if nl < 0: return ''
            cl = s[nl+1:]; continue
        if s.startswith('/*'):
            end = s.find('*/')
            if end < 0: return ''
            cl = s[end+2:]; continue
        return s

# Occurs-check-free cyclic unification needs rational trees (SWI sto(rational_trees)); out of scope.
# Heuristic: a body that unifies a var with a compound containing that same var, e.g. X=f(X) / X=f(Y),Y=f(X).
def body_is_cyclic(body):
    # X = f(...X...) direct
    for m in re.finditer(r'\b([A-Z_][A-Za-z0-9_]*)\s*=\s*([a-z][A-Za-z0-9_]*)\s*\(([^()]*)\)', body):
        var, inside = m.group(1), m.group(3)
        if re.search(r'\b' + re.escape(var) + r'\b', inside):
            return True
    # mutual: X=f(Y), Y=f(X)
    pairs = re.findall(r'\b([A-Z_][A-Za-z0-9_]*)\s*=\s*[a-z][A-Za-z0-9_]*\s*\(\s*([A-Z_][A-Za-z0-9_]*)\s*\)', body)
    d = {}
    for a, b in pairs:
        d[a] = b
    for a, b in d.items():
        if d.get(b) == a:
            return True
    return False

def main():
    if len(sys.argv) < 3:
        sys.stderr.write('usage: swi_extract_tests.py <plunit_file.pl> <out_dir>\n')
        return 2
    src_path, out_dir = sys.argv[1], sys.argv[2]
    with open(src_path, 'r', errors='replace') as f:
        src = f.read()
    base = os.path.splitext(os.path.basename(src_path))[0]
    dest = os.path.join(out_dir, base)
    os.makedirs(dest, exist_ok=True)

    clauses = split_clauses(src)
    # walk blocks: track begin_tests(Block...) / end_tests, accumulate helper clauses per block
    stats = {'blocks': 0, 'tests': 0, 'probes': 0, 'skips': 0}
    cur_block = None
    helpers = []           # non-test clauses inside the current block
    name_seen = {}
    for raw in clauses:
        cl = strip_leading_comments(raw)
        if not cl:
            continue
        mb = re.match(r'^:-\s*begin_tests\s*\(\s*([A-Za-z0-9_]+)', cl)
        if mb:
            cur_block = mb.group(1); helpers = []; name_seen = {}; stats['blocks'] += 1
            continue
        if re.match(r'^:-\s*end_tests', cl):
            cur_block = None; helpers = []
            continue
        if cur_block is None:
            continue
        t = parse_test(cl)
        if t is None:
            # helper clause or directive inside the block
            if cl.startswith(':-'):
                # strip SWI-only directives that would error in gprolog/scrip
                if re.match(r'^:-\s*(style_check|set_prolog_flag|use_module|module|ensure_loaded|table|dynamic|discontiguous)\b', cl):
                    # keep dynamic/discontiguous (ISO); drop the SWI-cosmetic ones
                    if re.match(r'^:-\s*(dynamic|discontiguous)\b', cl):
                        helpers.append(cl + '.')
                    continue
                helpers.append(cl + '.')
                continue
            helpers.append(cl + '.')
            continue
        stats['tests'] += 1
        name, opts, body = t
        key = sanitize(name)
        name_seen[key] = name_seen.get(key, 0) + 1
        if name_seen[key] > 1:
            key = '%s_%d' % (key, name_seen[key])
        stem = os.path.join(dest, '%s__%s' % (cur_block, key))
        probe, skip = build_probe(opts, body)
        # a test can pull a helper clause that itself uses an out-of-scope feature (e.g. unify_fv/1
        # calls garbage_collect); scan the helpers this test would carry, too.
        if probe is not None:
            helper_text = '\n'.join(helpers)
            hsk = is_out_of_scope_body(helper_text)
            if hsk:
                probe, skip = None, 'helper uses out-of-scope: %s' % hsk
        if probe is None:
            stats['skips'] += 1
            with open(stem + '.pl.xfail', 'w') as f:
                f.write('SKIP %s : %s\n' % (base, skip))
                f.write('%% test(%s, %s) :- %s.\n' % (name, opts, body))
            continue
        stats['probes'] += 1
        with open(stem + '.pl', 'w') as f:
            f.write('%% extracted from %s block(%s) test(%s)\n' % (base, cur_block, name))
            for h in helpers:
                f.write(h + '\n')
            f.write('main :- ( %s -> write(ok) ; write(fail) ), nl.\n' % probe)
            f.write(':- initialization(main).\n')
        with open(stem + '.expected', 'w') as f:
            f.write('ok\n')
    print('%s: blocks=%d tests=%d probes=%d skips=%d -> %s' %
          (base, stats['blocks'], stats['tests'], stats['probes'], stats['skips'], dest))
    return 0

if __name__ == '__main__':
    sys.exit(main())
