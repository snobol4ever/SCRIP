#!/usr/bin/env python3
"""util_logtalk_crossindex.py -- THE PREDICATE-TO-GROUP CROSS-INDEX, and the denominator a family verdict
should actually be read against (cto ruling to hq_C, 2026-09-13).

⛔⭐ WHY THIS EXISTS, MEASURED, NOT ARGUED. On 2026-09-13 hq_C cured two real defects in atom_concat/3 and
put_code/2. predicates/atom_concat_3 read 28 of 28 BEFORE and 28 of 28 AFTER. predicates/put_code_2 read
10 of 16 before and 10 of 16 after. Neither eponymous group could have gone red no matter how wrong its
predicate was, because every case in both is ASCII and byte indexing and character indexing AGREE ON EVERY
ASCII ATOM. The only cases in 3,268 that could see either defect live in unicode/builtins -- a directory
named for the INGREDIENT and not for the predicate -- so the evidence for a defect in atom_concat/3 was
filed under neither atom_concat nor concatenation, and a seat walking predicates/ group by group can close
every one of them and ship both bugs.

⭐ AN INSTRUMENT THAT CANNOT PRODUCE THE RED IS INDISTINGUISHABLE FROM ONE THAT FOUND NONE, AND ONLY THE
SECOND IS EVIDENCE. A green eponymous group is a claim about a POPULATION, and the population that matters
is every case that CALLS the predicate, wherever it is filed.

WHAT IT READS. The suite's own ALL.csv feature-attribute table (scripts/util_prolog_iso_attributes.py),
whose ~170 0/1 columns already record, per case, every ISO builtin the case's goal text names. So the
cross-index is DERIVED MECHANICALLY from the case sources and never hand-maintained -- a hand list would
go stale in the flattering direction the first time a case moved directories.

    --group G [G...]   print each group's eponymous denominator beside its cross-indexed one
    --strict           REFUSE(2) when a group's predicate has NO case outside its own group -- its green is
                       unfalsifiable from anywhere else in the suite. Measured blast radius on the suite as
                       vendored 2026-09-13: 144 eponymous groups, 104 with outside evidence, 40 without.
    --census           print the whole table: every eponymous group, inside and outside populations
rc 0 clean · 1 a group has no outside evidence (with --strict: 2, a refusal) · 2 could not measure
"""
import csv
import os
import sys


def load(suite):
    p = os.path.join(suite, "ALL.csv")
    if not os.path.isfile(p):
        sys.stderr.write("⛔ REFUSE(2): no ALL.csv at %s -- the feature-attribute table this index is derived from\n" % p)
        sys.exit(2)
    r = list(csv.reader(open(p, encoding="utf-8")))
    if len(r) < 2:
        sys.stderr.write("⛔ REFUSE(2): %s has no rows\n" % p)
        sys.exit(2)
    hdr = r[0]
    return hdr, {h: i for i, h in enumerate(hdr)}, r[1:]


def group_of(row):
    return row[1].split(":")[0] if len(row) > 1 else ""


def predicate_of(g, idx):
    """The predicate column a group is NAMED for: strip a trailing _<arity>, else the bare name."""
    parts = g.rsplit("_", 1)
    if len(parts) == 2 and parts[1].isdigit() and parts[0] in idx:
        return parts[0]
    return g if g in idx else None


def report(suite, groups, strict, census):
    hdr, idx, rows = load(suite)
    allg = sorted(set(group_of(x) for x in rows))
    bypred = {}
    for g in allg:
        p = predicate_of(g, idx)
        if p:
            bypred.setdefault(p, []).append(g)
    names = allg if census else groups
    rc = 0
    for g in names:
        p = predicate_of(g, idx)
        pop = sum(1 for x in rows if group_of(x) == g)
        if not p:
            print("  %-34s eponymous=%-4d cross-indexed=--    NOT NAMED FOR ONE PREDICATE -- this group is"
                  " OUTSIDE evidence for others, never a denominator of its own" % (g, pop))
            continue
        c = idx[p]
        inside = sum(1 for x in rows if group_of(x) == g and len(x) > c and x[c] == "1")
        outside = [x[1] for x in rows if group_of(x) != g and len(x) > c and x[c] == "1"]
        where = sorted(set(n.split(":")[0] for n in outside))
        print("  %-34s eponymous=%-4d calls-it=%-4d  cross-indexed=%-4d  outside=%-4d %s"
              % (g, pop, inside, inside + len(outside), len(outside), ("in " + ", ".join(where)) if where else ""))
        if inside == 0:
            print("      ⛔ NOT ONE CASE IN THIS GROUP NAMES %s: the group is named for a predicate its own"
                  " cases never call in callable position (a directive, or a flag the extractor does not set)."
                  " Its eponymous denominator is a count of something else." % p)
            rc = max(rc, 1)
        if not outside:
            print("      ⛔ NO CASE OUTSIDE THIS GROUP CALLS %s. A green here is UNFALSIFIABLE from anywhere"
                  " else in the suite: the group is its own only witness, so it cannot distinguish a correct"
                  " predicate from one whose defect its population cannot vary." % p)
            rc = max(rc, 1)
        sibs = [x for x in bypred.get(p, []) if x != g]
        if sibs:
            print("      ⚠ NAME-LEVEL, NOT NAME/ARITY: ALL.csv's feature columns are keyed by NAME, so this"
                  " count also contains every case calling %s at another arity (%s has its own group%s: %s)."
                  " Read the outside figure as an UPPER bound on independent evidence for this arity."
                  % (p, p, "s" if len(sibs) > 1 else "", ", ".join(sibs)))
    if strict and rc:
        sys.stderr.write("⛔ REFUSE(2) --strict: at least one group above has no independent witness; its verdict is a"
                         " claim about a population that cannot contain the counter-example.\n")
        return 2
    return rc


def main(argv):
    suite = os.environ.get("LOGTALK_SUITE")
    if not suite:
        here = os.path.dirname(os.path.abspath(__file__))
        suite = os.path.join(here, "..", "..", "corpus", "packages", "prolog", "logtalk_iso")
    strict = "--strict" in argv
    census = "--census" in argv
    groups = []
    if "--group" in argv:
        i = argv.index("--group") + 1
        while i < len(argv) and not argv[i].startswith("--"):
            groups.append(argv[i]); i += 1
    if not groups and not census:
        sys.stderr.write(__doc__)
        return 2
    return report(suite, groups, strict, census)


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
