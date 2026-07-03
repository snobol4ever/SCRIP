#!/usr/bin/env python3
# tools/insert_separators.py -- GOAL-STYLE-200COL pass 2 of 2 (v2, statement-level).
#
# v1 HISTORY / WHY v2 EXISTS: v1 detected functions with a single-line regex
# requiring "signature(...) {" on ONE line. That missed every Allman-style
# definition (signature line, '{' on the next line) -- 203 of them across 30
# files, strtab_label included -- and since pass 1 had already stripped their
# old separators, v1 left them with none: a net regression. v1 also planted
# separators BETWEEN a C++ 'template<...>' header and its function. And the
# v1 audit used the same detector, so it was circular. v2 detects at the
# STATEMENT level and is brace-style agnostic; audits must use independent
# eyes (see --audit, and the ctags cross-count in the census).
#
# Rules (GOAL-STYLE-200COL.md):
#   * one 200-char separator immediately before the FIRST LINE of every
#     top-level function definition statement (for 'template<...>' functions
#     that is the template line; for Allman that is the signature line);
#   * consecutive ONE-LINE definitions form a FAMILY sharing one separator;
#   * zero-function files get exactly one separator as line 1;
#   * never two separators in a row; existing separators are dropped and
#     rebuilt, so a rerun is a no-op on a compliant file (that no-op property
#     IS the --audit definition).
#
# Mechanics: line scan with per-line string/char-literal skipping. Preprocessor
# lines are transparent, INCLUDING backslash-continued macro bodies (so code-
# shaped lines inside multi-line #defines are never mistaken for top level).
# 'extern "C" {' blocks are depth-transparent so definitions inside them still
# count as top level. A '{' opening at logical depth 0 is a function iff the
# statement text before it contains '(', has no '=' outside parens, and does
# not start with typedef/namespace (struct/union/enum only reject when there
# are no parens, so 'struct T *f(...)' still counts).
import re, sys

SEP = '/*' + '-' * 196 + '*/'
SEPRE = re.compile(r'^/\*[-=]+\*/$')

def classify(stmt):
    s = stmt.strip()
    if s.startswith('extern') and '(' not in s:
        return 'transparent'
    if s.startswith('namespace') and '(' not in s:
        return 'transparent'
    m = re.match(r'[A-Za-z_][A-Za-z0-9_]*', s)
    fw = m.group(0) if m else ''
    if fw in ('typedef', 'namespace'):
        return 'other'
    if fw in ('struct', 'union', 'enum') and '(' not in s:
        return 'other'
    if fw in ('if', 'for', 'while', 'switch', 'do', 'else', 'return', 'case', 'goto'):
        return 'other'
    pd = 0
    for ch in s:
        if ch == '(':
            pd += 1
        elif ch == ')':
            pd -= 1
        elif ch == '=' and pd == 0:
            return 'other'
    if '(' not in s:
        return 'other'
    return 'func'

def analyze(lines):
    """Return (func_starts, oneliners): line indices (into `lines`) where a
    top-level function definition STATEMENT begins, and which are one-liners."""
    funcs = []
    onel = set()
    frames = []            # (kind 'n'|'t', func_stmt_start or None, open_line)
    stmt_start = None
    in_macro = False
    for idx, ln in enumerate(lines):
        if in_macro:
            in_macro = ln.rstrip().endswith('\\')
            continue
        stripped = ln.strip()
        if stripped.startswith('#'):
            in_macro = ln.rstrip().endswith('\\')
            continue
        ldepth = sum(1 for k, _, _ in frames if k == 'n')
        if ldepth == 0 and stmt_start is None and stripped != '':
            stmt_start = idx
        j, L = 0, len(ln)
        seg = 0
        while j < L:
            c = ln[j]
            if c == '"' or c == "'":
                q = c
                j += 1
                while j < L and ln[j] != q:
                    if ln[j] == '\\':
                        j += 1
                    j += 1
            elif c == '{':
                ldepth = sum(1 for k, _, _ in frames if k == 'n')
                if ldepth == 0:
                    start = stmt_start if stmt_start is not None else idx
                    stmt = (' '.join(x.strip() for x in lines[start:idx]) + ' ' + ln[seg:j].strip()).strip()
                    kind = classify(stmt)
                    if kind == 'func':
                        funcs.append(start)
                        frames.append(('n', start, idx))
                    elif kind == 'transparent':
                        frames.append(('t', None, idx))
                    else:
                        frames.append(('n', None, idx))
                    stmt_start = None
                else:
                    frames.append(('n', None, idx))
            elif c == '}':
                if frames:
                    k, fs, ol = frames.pop()
                    if k == 'n' and fs is not None and ol == idx and fs == idx:
                        onel.add(fs)
                if sum(1 for k, _, _ in frames if k == 'n') == 0:
                    stmt_start = None
                    seg = j + 1
            elif c == ';':
                if sum(1 for k, _, _ in frames if k == 'n') == 0:
                    stmt_start = None
                    seg = j + 1
            j += 1
    return funcs, onel

def rebuild(text):
    lines = [ln for ln in text.split('\n') if not SEPRE.match(ln)]
    if lines and lines[-1] == '':
        lines.pop()
    funcs, onel = analyze(lines)
    fset = set(funcs)
    out = []
    role = 'other'
    nseps = 0
    for idx, ln in enumerate(lines):
        if idx in fset:
            one = idx in onel
            if role != 'sep' and not (one and role == 'oneliner'):
                out.append(SEP)
                nseps += 1
            out.append(ln)
            role = 'oneliner' if one else 'other'
        else:
            out.append(ln)
            if ln.strip():
                role = 'other'
    if not funcs:
        out.insert(0, SEP)
        nseps += 1
    return '\n'.join(out) + '\n', len(funcs), nseps

def main(argv):
    audit = False
    if argv and argv[0] == '--audit':
        audit = True
        argv = argv[1:]
    tf = ts = zero = bad = 0
    badf = []
    for p in argv:
        raw = open(p, encoding='utf-8', errors='surrogateescape').read()
        new, f, s = rebuild(raw)
        tf += f
        ts += s
        if f == 0:
            zero += 1
        if audit:
            if new != raw:
                bad += 1
                badf.append(p)
        elif new != raw:
            open(p, 'w', encoding='utf-8', errors='surrogateescape').write(new)
    if audit:
        print('%d files audited: %d function defs, %d separators expected, %d NONCOMPLIANT' % (len(argv), tf, ts, bad))
        for p in badf[:12]:
            print('  noncompliant:', p)
    else:
        print('%d files: %d function definitions, %d separators placed, %d zero-function files' % (len(argv), tf, ts, zero))

if __name__ == '__main__':
    main(sys.argv[1:])
