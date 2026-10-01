#!/usr/bin/env python3
# util_icon_semicolons.py -- insert the semicolons SCRIP Icon requires, exactly where the strict parser asks for them
# (Lon 2026-10-01, CEO-1392: every statement in a procedure body ends in ';'; only the last expression of a { } block is
# bare). The parser is the only authority: each pass runs out/parser_icon, reads "expected ';' after the token at line L
# col C", finds that token's end on line L (a string literal to its closing quote, a name or number to the end of its run,
# any other token one character) and inserts ';' there; it loops until the file parses or the error is not a semicolon.
#   python3 scripts/util_icon_semicolons.py [--check] FILE...     rc 0 every file parses (changed files named), 1 a file
#   still refuses for another reason (named), 2 no parser binary. --check rewrites nothing and reports what it would add.
import os, re, subprocess, sys
HERE = os.path.dirname(os.path.abspath(__file__)); ROOT = os.path.dirname(HERE); PARSER = os.path.join(ROOT, 'out', 'parser_icon')
ERR = re.compile(r"expected ';' after the token at line (\d+) col (\d+)")
def token_end(line, col):
    i = col - 1
    if i >= len(line): return len(line)
    c = line[i]
    if c in '"\'':
        j = i + 1
        while j < len(line):
            if line[j] == '\\': j += 2; continue
            if line[j] == c: return j + 1
            j += 1
        return len(line)
    if c.isalnum() or c == '_':
        j = i
        while j < len(line) and (line[j].isalnum() or line[j] == '_' or line[j] == '.'): j += 1
        return j
    return i + 1
def parse_error(path):
    r = subprocess.run([PARSER, path], stdout=subprocess.DEVNULL, stderr=subprocess.PIPE, text=True, timeout=20)
    return None if r.returncode == 0 else (r.stderr or '').strip().splitlines()[0] if (r.stderr or '').strip() else 'parse error (no message)'
def fix(path, check):
    lines = open(path, encoding='utf-8', newline='').read().split('\n'); added = []
    for _ in range(500):
        err = parse_error(path if not check and not added else write_tmp(path, lines))
        if err is None: break
        m = ERR.search(err)
        if not m: return added, err
        L, C = int(m.group(1)), int(m.group(2))
        if L < 1 or L > len(lines): return added, err
        e = token_end(lines[L - 1], C)
        lines[L - 1] = lines[L - 1][:e] + ';' + lines[L - 1][e:]; added.append((L, e))
        if not check: open(path, 'w', encoding='utf-8', newline='').write('\n'.join(lines))
    return added, None
def write_tmp(path, lines):
    t = path + '.semi.tmp'; open(t, 'w', encoding='utf-8', newline='').write('\n'.join(lines)); return t
def main(argv):
    check = '--check' in argv; files = [a for a in argv if a != '--check']
    if not os.access(PARSER, os.X_OK): print('REFUSE(2): no out/parser_icon -- make first'); return 2
    bad = 0; changed = 0
    for f in files:
        added, err = fix(f, check)
        t = f + '.semi.tmp'
        if os.path.exists(t): os.unlink(t)
        if added: changed += 1; print('%s %s: %d semicolon(s) at %s' % ('WOULD ADD' if check else 'ADDED', f, len(added), ' '.join('%d:%d' % a for a in added[:12])))
        if err: bad += 1; print('STILL REFUSED %s: %s' % (f, err[:160]))
    print('files=%d changed=%d still_refused=%d' % (len(files), changed, bad)); return 1 if bad else 0
if __name__ == '__main__': sys.exit(main(sys.argv[1:]))
