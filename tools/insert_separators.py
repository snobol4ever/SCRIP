#!/usr/bin/env python3
# tools/insert_separators.py -- GOAL-STYLE-200COL pass 2 of 2.
# Regenerates the 200-char function separator lines from scratch. Run AFTER
# tools/strip_comments.py (which deletes every old separator regardless of
# width, so this pass rebuilds a uniform set and mixed 120/200 widths become
# structurally impossible).
#
# Rules implemented (per GOAL-STYLE-200COL.md):
#   * exactly one 200-char /*----...----*/ line immediately before every
#     top-level function DEFINITION (a col-0 signature line ending in '{');
#   * consecutive ONE-LINE function definitions form a FAMILY that shares the
#     single separator before the family's first member (the pl_cell.h /
#     x86_asm.h convention);
#   * a file containing no function definitions at all (declaration-only
#     headers, data tables) gets exactly one separator as its first line
#     (the gva_collect.h / arith_fold.h convention);
#   * never two separators in a row.
#
# Function-start heuristic: column-0 line, optional 'extern "C"'/'extern'
# prefix, then IDENT...(args) { with no ';' or '{' inside the arg list.
# Control keywords, typedef/struct/enum/union openers, namespace, and asserts
# are excluded. Declarations (ending ';') never match. This matches the
# uniform one-line-signature style of this codebase; the census after a run
# is the check that coverage is sane.
import re, sys

SEP = '/*' + '-' * 196 + '*/'
SEP_SHAPE = re.compile(r'^/\*[-=]+\*/$')
EXCLUDE = re.compile(r'^(if|for|while|switch|do|else|return|case|goto|typedef|struct|union|enum|namespace|static_assert|_Static_assert|sizeof)\b')
FUNC = re.compile(r'^[A-Za-z_][A-Za-z0-9_ \t\*:<>&,\[\]]*\([^;{]*\)\s*\{')

def norm(ln):
    s = ln
    for p in ('extern "C" ', 'extern '):
        if s.startswith(p):
            s = s[len(p):]
    return s

def is_func_start(ln):
    if not ln or ln[0] in ' \t#}':
        return False
    s = norm(ln)
    if not s or s[0] == '{':
        return False
    if EXCLUDE.match(s):
        return False
    return bool(FUNC.match(s))

def process(path):
    text = open(path, encoding='utf-8', errors='surrogateescape').read()
    lines = text.split('\n')
    if lines and lines[-1] == '':
        lines.pop()
    out = []
    role = 'other'          # 'sep' | 'oneliner' | 'other'
    nfuncs = 0
    nseps = 0
    for ln in lines:
        if SEP_SHAPE.match(ln):
            continue        # defensive: pass 1 should already have removed these
        if is_func_start(ln):
            nfuncs += 1
            one = ln.rstrip().endswith('}')
            if role != 'sep' and not (one and role == 'oneliner'):
                out.append(SEP)
                nseps += 1
            out.append(ln)
            role = 'oneliner' if one else 'other'
        else:
            out.append(ln)
            role = 'other'
    if nfuncs == 0:
        out.insert(0, SEP)
        nseps += 1
    open(path, 'w', encoding='utf-8', errors='surrogateescape').write('\n'.join(out) + '\n')
    return nfuncs, nseps

def main(argv):
    tf = ts = zero = 0
    for p in argv:
        f, s = process(p)
        tf += f
        ts += s
        if f == 0:
            zero += 1
    print('%d files: %d function definitions found, %d separators inserted, %d zero-function files (top separator only)' % (len(argv), tf, ts, zero))

if __name__ == '__main__':
    main(sys.argv[1:])
