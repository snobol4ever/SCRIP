#!/usr/bin/env python3
"""util_mint_package_attribute_csv.py -- write a package suite's ALL.csv attribute file from the enumeration its runner grades.

Lon 2026-09-23 00:1x CDT, in-chat to the ceo, verbatim: "Well, each test should have the needed amount of memory in the
attribute TSV/CSV files." and 00:2x: "get the attribute files created for all test suites." (CEO-1167/1168.)  Every graded
suite carries an ALL.csv; a suite that is a bare directory of programs gets one from THIS script, in the shape the eight
existing package files already use (rank,entry,origin,package,n_lines,stdin,want_rc) plus the heap_kb column that
carries a program's DECLARED arena in KB (empty = the shipped default).  Feature columns are not minted here (row
feature-accounting-stops-at-the-package-boundary).

usage: util_mint_package_attribute_csv.py --package NAME --dir DIR --glob PATTERN [--glob PATTERN ...] [--recursive]
                                           [--origin-root DIR] [--filter-grep REGEX] [--out PATH] [--refresh]
  --glob        shell pattern relative to DIR (repeatable); --recursive walks DIR
  --origin-root the path origins are written relative to (default DIR); for a drop read in place, the drop's root
  --filter-grep keep only files whose text matches REGEX (swi_tests: 'begin_tests\\(')
  --refresh     rewrite an existing ALL.csv, PRESERVING each entry's heap_kb; without it an existing file is REFUSED rc=2
  stdin  = 1 when <entry>.in sits beside the program; want_rc from ALL.wantrc (entry<TAB>rc) when present, else 0
prints: ATTRIBUTE-CSV package=P entries=N stdin=S wantrc=W preserved_heap=H written=PATH
"""
import argparse, csv, glob, io, os, re, sys
def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--package', required=True); ap.add_argument('--dir', required=True)
    ap.add_argument('--glob', action='append', required=True); ap.add_argument('--recursive', action='store_true')
    ap.add_argument('--origin-root', default='')
    ap.add_argument('--filter-grep', default=''); ap.add_argument('--out', default=''); ap.add_argument('--refresh', action='store_true')
    a = ap.parse_args()
    d = os.path.abspath(a.dir)
    if not os.path.isdir(d): print(f"REFUSE(2): no such directory {d}"); return 2
    root = os.path.abspath(a.origin_root) if a.origin_root else d
    out = a.out or os.path.join(d, 'ALL.csv')
    files = []
    for g in a.glob:
        pat = os.path.join(d, '**', g) if a.recursive else os.path.join(d, g)
        files += glob.glob(pat, recursive=a.recursive)
    files = sorted(set(os.path.abspath(f) for f in files if os.path.isfile(f)))
    if a.filter_grep:
        rx = re.compile(a.filter_grep)
        files = [f for f in files if rx.search(open(f, encoding='utf-8', errors='replace').read())]
    if not files: print(f"REFUSE(2): the enumeration matched no file under {d} -- a suite with zero programs is not a suite"); return 2
    preserved = {}
    preserved_stack = {}   # stack_kb, preserved the same way (CEO-1225)
    if os.path.exists(out):
        if not a.refresh: print(f"REFUSE(2): {out} exists; --refresh rewrites it preserving heap_kb"); return 2
        with open(out, newline='', encoding='utf-8') as fh:
            for row in csv.DictReader(fh):
                if row.get('heap_kb'): preserved[row['entry']] = row['heap_kb']
                if row.get('stack_kb'): preserved_stack[row['entry']] = row['stack_kb']
    wantrc = {}
    wr = os.path.join(d, 'ALL.wantrc')
    if os.path.exists(wr):
        for ln in open(wr, encoding='utf-8', errors='replace'):
            ln = ln.rstrip('\n')
            if not ln or ln.startswith('#'): continue
            parts = ln.split('\t') if '\t' in ln else ln.split()
            if len(parts) >= 2: wantrc[parts[0]] = parts[1]
    buf = io.StringIO()
    w = csv.writer(buf, lineterminator='\n')
    w.writerow(['rank', 'entry', 'origin', 'package', 'n_lines', 'stdin', 'want_rc', 'heap_kb', 'stack_kb'])
    n_stdin = n_wantrc = n_pres = 0
    for i, f in enumerate(files, 1):
        rel = os.path.relpath(f, root)
        entry = os.path.splitext(rel)[0]
        with open(f, 'rb') as fh: n_lines = fh.read().count(b'\n')
        has_in = 1 if os.path.exists(os.path.splitext(f)[0] + '.in') else 0
        rc = wantrc.get(entry, wantrc.get(os.path.basename(entry), '0'))
        hk = preserved.get(entry, '')
        n_stdin += has_in; n_wantrc += (rc != '0'); n_pres += bool(hk)
        w.writerow([i, entry, f"{a.package}__{entry}", a.package, n_lines, has_in, rc, hk, preserved_stack.get(entry, '')])
    with open(out, 'w', encoding='utf-8', newline='\n') as fh: fh.write(buf.getvalue())
    print(f"ATTRIBUTE-CSV package={a.package} entries={len(files)} stdin={n_stdin} wantrc={n_wantrc} preserved_heap={n_pres} written={out}")
    return 0
if __name__ == '__main__': sys.exit(main())
