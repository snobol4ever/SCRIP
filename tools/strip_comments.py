#!/usr/bin/env python3
# tools/strip_comments.py -- GOAL-STYLE-200COL pass 1 of 2.
# Removes EVERY comment from C/H/CPP source: all /* */ spans INCLUDING separator
# lines, and all // line comments. Also removes all blank lines and trailing
# whitespace. String literals ("...") and char literals ('...') are respected,
# including escaped quotes, so tokens like the SNOBOL4/Prolog "//" integer-divide
# operator inside string literals are never touched. Files are read/written with
# errors='surrogateescape' so non-UTF-8 bytes round-trip untouched.
#
# Usage:  strip_comments.py [--count] FILE...
#   --count : report what WOULD be removed, change nothing (census mode).
#
# Pair with tools/insert_separators.py, which regenerates the 200-char
# function separators after this pass has removed everything.
import sys

def strip(text):
    out = []
    i, n = 0, len(text)
    removed = 0
    while i < n:
        c = text[i]
        if c == '"' or c == "'":
            q = c
            j = i + 1
            while j < n and text[j] != q:
                if text[j] == '\\':
                    j += 1
                j += 1
            j += 1
            out.append(text[i:j])
            i = j
            continue
        if text.startswith('/*', i):
            e = text.find('*/', i + 2)
            if e == -1:
                raise SystemExit('unterminated block comment at byte %d' % i)
            removed += 1
            i = e + 2
            continue
        if text.startswith('//', i):
            e = text.find('\n', i)
            if e == -1:
                e = n
            removed += 1
            i = e
            continue
        out.append(c)
        i += 1
    s = ''.join(out)
    lines = [ln.rstrip() for ln in s.split('\n')]
    lines = [ln for ln in lines if ln.strip() != '']
    return '\n'.join(lines) + '\n', removed

def main(argv):
    count_only = False
    if argv and argv[0] == '--count':
        count_only = True
        argv = argv[1:]
    total_removed = 0
    total_files_with = 0
    for p in argv:
        raw = open(p, encoding='utf-8', errors='surrogateescape').read()
        new, removed = strip(raw)
        if removed:
            total_files_with += 1
        total_removed += removed
        if not count_only and new != raw:
            open(p, 'w', encoding='utf-8', errors='surrogateescape').write(new)
    mode = 'WOULD remove' if count_only else 'removed'
    print('%d files scanned, %s %d comment spans (in %d files)' % (len(argv), mode, total_removed, total_files_with))

if __name__ == '__main__':
    main(sys.argv[1:])
