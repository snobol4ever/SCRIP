#!/usr/bin/env python3
"""util_logtalk_extract.py -- CEO SPIKE, PROVEN, HANDED TO hq_R TO HARDEN AND OWN.

Row: prolog-logtalk-iso-3268-conformance-cases-have-no-runner (the name under-counts; see below).
Lon 2026-09-11: "Let's get those Prolog programs graded."

⛔ THE POPULATION IS 3617, NOT 3268 -- the ceo under-reported it by 349 and the row name carries the
old number. Census by head form across the 192 tests.lgt:
    test/2      3268      succeeds/1   162      throws/2   134      fails/1    53   = 3617
The legacy three are the same three expectations spelled differently: succeeds == true, fails == false,
throws(Name,E) == error(E). A parser that handles only test/2 silently grades 90% and reports 100%.

WHAT THIS SPIKE PROVES (ceo, measured): 3261 of the 3268 test/2 cases extract cleanly from all 192 files
-- 99.8%. The 30 files that yield zero here are exactly the legacy-form files, NOT parse failures.

THE FILE SHAPE, which is what makes this tractable:
  * everything BEFORE `:- object(tests,` is the DATABASE: support clauses (a(1). a(2). b(X) :- ...) and
    directives (:- dynamic(unicorn/0).). It must be emitted with every case from that file.
  * `{Goal}` is Logtalk's escape to PLAIN PROLOG -- so no Logtalk runtime is needed, which is the whole
    reason this suite is gradeable by us.
  * `^^name(...)` are lgtunit framework calls (^^suppress_text_output) and are stripped.
  * expectations, by frequency: true 1764 | error 955 | false 198 | errors 138 | deterministic 50 |
    exists 41 | variant 29 | true(Cond) 21 | fail 18 | subsumes 18 | ball 13 | false(Cond) 5.
    true/error/false/errors/deterministic are 95% of the suite -- ship those first, then the rest.

⛔ HARDENING OWED BY hq_R BEFORE THIS IS A RUNNER: the legacy three forms; the 7 test/2 cases this
misses; nested braces inside a goal; and the DONE-WHEN's identity --
PASS+FAIL+OUTSIDE+UNGRADABLE+UNGRADED+DEFERRED must equal the population it globbed, and a case that
cannot be parsed REFUSES rc=2 and is NAMED, never dropped (ARCH-PROGRAM-LEDGER: UNKNOWN is not ZERO).
"""

import re,sys,os
def parse(path):
    src=open(path,encoding='utf-8',errors='replace').read()
    i=src.find(':- object(')
    db,body=(src[:i],src[i:]) if i>=0 else ('',src)
    db='\n'.join(l for l in db.split('\n') if not l.lstrip().startswith('%'))
    cases=[]
    # a case starts at "test(" at indent and ends at the next "\n\ttest(" or ":- end_object"
    parts=re.split(r'\n(?=\s*(?:test|succeeds|fails|throws)\()',body)
    for p in parts:
        m=re.match(r'\s*test\(\s*([a-zA-Z0-9_]+)\s*,\s*(.*?)\)\s*:-\s*(.*)',p,re.S)
        if not m: continue
        name=m.group(1); rest=m.group(2)+')'+m.group(3)
        # split expectation from body by finding the ") :-" that closes test/2 head
        mm=re.match(r'\s*test\((.*?)\)\s*:-\s*(.*)',p,re.S)
        head=mm.group(1); goal=mm.group(2)
        d=0; cut=None
        for k,ch in enumerate(head):
            if ch=='(':d+=1
            elif ch==')':d-=1
            elif ch==',' and d==0: cut=k; break
        if cut is None: continue
        exp=head[cut+1:].strip()
        goal=goal.rsplit('.',1)[0] if goal.rstrip().endswith('.') else goal
        goal='\n'.join(l for l in goal.split('\n') if not l.lstrip().startswith('%'))
        goal=re.sub(r'\^\^[a-z_]+(\([^)]*\))?\s*,\s*','',goal)   # drop lgtunit helpers
        goal=goal.replace('{','(').replace('}',')')               # plain-Prolog escape
        cases.append((name,exp,' '.join(goal.split())))
    return db,cases
if __name__=='__main__':
    db,cs=parse(sys.argv[1])
    print(f"# db chars={len(db.strip())}  cases={len(cs)}")
    for n,e,g in cs[:int(sys.argv[2]) if len(sys.argv)>2 else 6]:
        print(f"  {n:22s} exp={e[:44]:44.44s} goal={g[:60]}")
