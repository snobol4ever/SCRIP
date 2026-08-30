#!/usr/bin/env python3
"""nqp_reach.py -- which inherited HLL::Grammar rules are REACHABLE from the entry point?

RUNG 5 scoping. There are 77 subrules Grammar.nqp references but does not define; hand-writing all
of them before anything parses is the wrong order. This walks the call graph from `comp_unit` and
reports the inherited rules by how many DISTINCT reachable rules call them -- so the work is ordered
by leverage rather than by the order they happen to appear in the file.

⛔ It reports UNREACHABLE definitions too. A rule nothing reaches is not a gap to fill; writing it
would be work that cannot change any outcome, and it would look like progress.
"""
import sys, os, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from nqp_read import scan_decls, lex_body
from nqp_ast import P

def main(path='/home/resources/rakudo-main/src/Perl6/Grammar.nqp', root='comp_unit'):
    decls = [d for d in scan_decls(open(path, encoding='utf-8', errors='replace').read())
             if not d.get('overrun')]
    defined = collections.defaultdict(list)
    for d in decls: defined[d['name']].append(d)
    edges = collections.defaultdict(set)
    LANGDISP = __import__('re').compile(r"(?:FOREIGN_)?LANG\s*\(([^)]*)\)")
    STRARG   = __import__('re').compile(r"'([A-Za-z_][A-Za-z0-9_-]*)'")
    for d in decls:
        for n in P(lex_body(d['body'])).parse().walk():
            if n.k == 'CALL' and n.v: edges[d['name']].add(n.v.split('(')[0].strip())
        # ⛔ THE GRAMMAR'S CALL GRAPH IS NOT STATICALLY CONNECTED. comp_unit reaches the entire
        # program through `<statementlist=.FOREIGN_LANG($*MAIN, 'statementlist', 1)>` -- it dispatches
        # to a rule BY NAME, AS A STRING, at run time (the slang mechanism, 27 sites). A plain
        # call-graph walk therefore reports 12 reachable rules and 199 "unreachable" definitions,
        # which reads as a fact about the grammar instead of a limit of the instrument. The targets
        # ARE literal here, so follow them -- but note this is the same non-static property as the
        # parse-time-extensible operator table, showing up in the RULE graph rather than the operators.
        for m in LANGDISP.finditer(d['body']):
            for t in STRARG.findall(m.group(1)):
                if t != 'MAIN': edges[d['name']].add(t)
    seen, stack = set(), [root]
    while stack:
        r = stack.pop()
        if r in seen: continue
        seen.add(r)
        for c in edges.get(r, ()): 
            if c not in seen: stack.append(c)
    inherited = collections.Counter()
    for r in seen:
        for c in edges.get(r, ()):
            if c not in defined: inherited[c] += 1
    reach_def = [r for r in seen if r in defined]
    unreach = [n for n in defined if n not in seen]
    print(f"entry point           : {root}")
    print(f"rules REACHABLE       : {len(seen)}  ({len(reach_def)} defined here, {len(seen)-len(reach_def)} inherited)")
    print(f"rules defined but NOT reachable from {root} : {len(unreach)}")
    print()
    print("INHERITED RULES THAT MUST BE HAND-WRITTEN, by number of distinct reachable callers:")
    for name, n in inherited.most_common(24):
        print(f"  {n:>4}  {name}")
    print()
    print(f"total inherited-and-reachable: {len(inherited)}")

if __name__ == '__main__':
    main(*sys.argv[1:])
