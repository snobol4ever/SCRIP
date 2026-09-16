#!/usr/bin/env python3
"""util_suite_rows_vs_progress.py -- THE CROSS-LANGUAGE BATCH AUDIT (MODE line 2 under DECTET, CEO-775; ceo CEO-780a; coo
2026-09-16, row util-suite-rows-vs-progress-the-cross-language-batch-audit-lands-as-an-instrument).

After each batch every HQ's suite rows are on origin and AGREE with the progress DB; a disagreement is a row. This reads
every .github/SUITES.tsv row against /home/resources/progress/results.tsv ON THE ROW'S OWN TREE (column `tree`), over the
modes each program declares (its LAST outcome per mode; PASS only when every recorded mode reads PASS), and prints per row:

    key  row pass/total  DB pass/population  xpass  measurer  last DB reading  verdict

  numerator   row today_pass  vs  DB PASS (AND over declared modes)
  denominator row today_total vs  DB population = every program with a row on that tree, plus every entry of the suite's
              OUTSIDE sidecar not already there (a sidecar-named program is OUTSIDE whatever the DB called it; the same
              reconciliation test_gate_score_row_denominator_includes_xfails.sh uses, sourced from one table below)
  xpass       programs whose PASS rows carry the note `xfail` -- THE DB FOLDS XPASS INTO PASS AND A SUITE BOARD DOES NOT
              (hq_snocone 2026-09-16): a numerator gap EQUAL to xpass means stale xfail markers still on the suite (promote
              them, CEO-753); a gap with xpass 0 means the row is simply behind its tree or its runner counts differently.

VERDICT per row: AGREE · DISAGREE (numerator and/or denominator, each with the difference) · UNPROVEN (no DB rows on the
row's tree, named -- CEO-750: a row without per-program evidence is DARK wearing a number) · DEFERRED (no tree, a DEFERRED
criterion: gnu_fd, CEO-579). rc=1 when any graded row DISAGREES; rc=2 when zero rows are gradable; else 0. An UNPROVEN row
does not by itself make rc=2 (CEO-786 confirmed this reading for the sibling gate: a per-row rc=2 is a standing refusal).

--selftest builds a scratch SUITES.tsv, DB and corpus under mktemp: an agreeing row, a planted NUMERATOR disagreement whose
gap equals its xpass count, a planted DENOMINATOR disagreement, an UNPROVEN row and a DEFERRED row, and proves rc=1 naming
the planted rows with the xpass column, rc=0 once they are removed, rc=2 on an empty DB.

env: S4E_SUITES_TSV, S4E_PROGRESS_DB, S4E_CORPUS_ROOT redirect the three inputs (the selftest and gates use them).
"""
import sys, os, re, collections, tempfile, shutil

HERE = os.path.dirname(os.path.abspath(__file__))
DBNAME = {"sno-master": "snobol4-master", "icn-master": "icon-master", "pl-master": "prolog-master", "pas-master": "pascal-master",
          "raku-master": "raku-master", "snc-master": "snocone-master", "reb-master": "rebus-master", "x64tests": "spitbol_x64"}
SIDECAR = {"sno-master": "tests/snobol4/ALL.outside.tsv",
           "gimpel": "packages/snobol4/gimpel/OUTSIDE_SPITBOL_BASELINE.tsv",
           "csnobol4": "packages/snobol4/csnobol4_suite/OUTSIDE_SPITBOL_BASELINE.tsv",
           "snoflake": "packages/snobol4/snoflake_suite/OUTSIDE_SPITBOL_BASELINE.tsv",
           "aisnobol": "packages/snobol4/aisnobol/OUTSIDE_SPITBOL_BASELINE.tsv",
           "dotnet": "packages/snobol4/dotnet/OUTSIDE_SPITBOL_BASELINE.tsv",
           "testpgms": "packages/snobol4/spitbol_testpgms/OUTSIDE_SPITBOL_BASELINE.tsv",
           "arizona": "packages/icon/arizona_tests/OUTSIDE_ARIZONA_BASELINE.tsv",
           "jcon": "packages/icon/jcon_tests/OUTSIDE_ARIZONA_BASELINE.tsv"}
UNG = {"UNGRADED", "SKIP", "MISSING", "UNPROVEN"}


def stem(n):
    return re.sub(r"\.(sno|spt|sc|icn|pl|reb|raku|pas)$", "", n.split("/")[-1])


def read_rows(suites):
    rows = []
    for line in open(suites, encoding="utf-8"):
        if line.startswith("#") or line.startswith("key") or not line.strip():
            continue
        c = line.rstrip("\n").split("\t")
        while len(c) < 12:
            c.append("")
        rows.append(c)
    return rows


def read_db(db, want):
    """last outcome (and note, measurer, ts) per (suite, tree, program, mode) for the wanted (suite, tree) pairs."""
    last = {}
    with open(db, encoding="utf-8") as fh:
        fh.readline()
        for line in fh:
            f = line.rstrip("\n").split("\t")
            if len(f) < 10 or (f[5], f[1]) not in want or f[8] not in ("m3", "m4"):
                continue
            last[(f[5], f[1], f[7], f[8])] = (f[9], f[11] if len(f) > 11 else "", f[3], f[0])
    return last


def sidecar_stems(corpus, key):
    """(outside, ungradable, ungraded) name sets from the suite's sidecars: the OUTSIDE file the map names, plus the
    package's UNGRADABLE.tsv / UNGRADED.tsv beside it (ceo CEO-798: the runner's PACKAGE_INVENTORY line is the population
    authority and those two files are its record on disk; csnobol4: 61 UNGRADABLE of which the 49 OUTSIDE are a subset)."""
    sc = SIDECAR.get(key)
    out, ungradable, ungraded = set(), set(), set()
    if sc and os.path.exists(os.path.join(corpus, sc)):
        for l in open(os.path.join(corpus, sc), encoding="utf-8", errors="replace"):
            if l.startswith("#") or not l.strip():
                continue
            out.add(stem(l.split("\t")[0].strip()))
        for fn, dst in (("UNGRADABLE.tsv", ungradable), ("UNGRADED.tsv", ungraded)):
            fp = os.path.join(corpus, os.path.dirname(sc), fn)
            if os.path.exists(fp):
                for l in open(fp, encoding="utf-8", errors="replace"):
                    if l.startswith("#") or not l.strip():
                        continue
                    dst.add(stem(l.split("\t")[0].strip()))
    return out, ungradable - out, ungraded - out - ungradable


def classify(mm, outside_named):
    v = {o for o, _n in mm.values()}
    if outside_named or "OUTSIDE" in v:
        return "OUTSIDE"
    if "DEFERRED" in v:
        return "DEFERRED"
    if v == {"PASS"}:
        return "PASS"
    if v == {"UNGRADABLE"}:
        return "UNGRADABLE"
    if v <= UNG:
        return "UNGRADED"
    return "FAIL"


def audit(suites, db, corpus, out=print):
    rows = read_rows(suites)
    if not rows:
        out("⛔ REFUSES (rc=2): SUITES.tsv carries no rows")
        return 2
    want = {(DBNAME.get(c[0], c[0]), c[10]): c[0] for c in rows if c[10]}
    last = read_db(db, want)
    progs = collections.defaultdict(lambda: collections.defaultdict(dict))
    meas = collections.defaultdict(set)
    lastts = collections.defaultdict(str)
    for (s, t, p, m), (o, note, who, ts) in last.items():
        progs[(s, t)][p][m] = (o, note)
        meas[(s, t)].add(who)
        lastts[(s, t)] = max(lastts[(s, t)], ts)
    graded = agree = 0
    disagree, unproven, deferred = [], [], []
    out(f"{'key':12} {'row':>10} {'DB':>10} {'xpass':>5}  {'measurer':10} {'last DB reading':19}  verdict")
    for c in rows:
        key, nick, P, T, tree, crit = c[0], c[1], c[8], c[9], c[10], c[11]
        tag = f"{key:12} {(P or '-') + '/' + (T or '-'):>10}"
        if not tree and "DEFERRED" in crit.upper():
            deferred.append(key)
            out(f"{tag} {'-':>10} {'-':>5}  {'-':10} {'-':19}  DEFERRED by ruling ({crit[:50]})")
            continue
        if not tree:
            unproven.append(key)
            out(f"{tag} {'-':>10} {'-':>5}  {'-':10} {'-':19}  UNPROVEN: no tree on the row (never measured)")
            continue
        d = progs.get((DBNAME.get(key, key), tree), {})
        side, s_ungradable, s_ungraded = sidecar_stems(corpus, key)
        if not d:
            unproven.append(key)
            out(f"{tag} {'-':>10} {'-':>5}  {'-':10} {'-':19}  UNPROVEN: no progress rows for this suite on {tree} (CEO-750)")
            continue
        cnt = collections.Counter()
        seen = set()
        xpass = 0
        for p, mm in d.items():
            seen.add(stem(p))
            k = classify(mm, stem(p) in side)
            cnt[k] += 1
            if k == "PASS" and all(n == "xfail" for _o, n in mm.values()):
                xpass += 1
        cnt["OUTSIDE"] += len(side - seen)
        cnt["UNGRADABLE"] += len(s_ungradable - seen)
        cnt["UNGRADED"] += len(s_ungraded - seen)
        pop = sum(cnt.values())
        graded += 1
        p_row = int(P) if P.isdigit() else -1
        t_row = int(T) if T.isdigit() else -1
        who = ",".join(sorted(x for x in meas[(DBNAME.get(key, key), tree)] if x))[:10]
        line = f"{tag} {str(cnt['PASS']) + '/' + str(pop):>10} {xpass:>5}  {who:10} {lastts[(DBNAME.get(key, key), tree)][:19]:19}"
        why = []
        if p_row != cnt["PASS"]:
            gap = cnt["PASS"] - p_row
            why.append(f"numerator row {P} vs DB {cnt['PASS']} ({gap:+d})" + (" = xpass: STALE MARKERS, promote them (CEO-753)" if gap == xpass and xpass > 0 else ""))
        if t_row != pop:
            why.append(f"denominator row {T} vs DB population {pop} ({t_row - pop:+d}; PASS {cnt['PASS']} FAIL {cnt['FAIL']} OUTSIDE {cnt['OUTSIDE']} UNGRADABLE {cnt['UNGRADABLE']} UNGRADED {cnt['UNGRADED']} DEFERRED {cnt['DEFERRED']})")
        if why:
            disagree.append(f"{key}: " + "; ".join(why))
            out(f"{line}  DISAGREE: " + "; ".join(why))
        else:
            agree += 1
            out(f"{line}  AGREE")
    out(f"population: {len(rows)} row(s): {graded} graded, {agree} agree, {len(disagree)} disagree, {len(unproven)} unproven ({', '.join(unproven) or 'none'}), {len(deferred)} deferred by ruling ({', '.join(deferred) or 'none'})")
    if graded == 0:
        out("⛔ REFUSES (rc=2): zero rows gradable -- no SUITES.tsv row has progress rows on its own tree; this audit measured nothing.")
        return 2
    if disagree:
        out(f"⛔ DISAGREE [suite_rows_vs_progress]: {len(disagree)} of {graded} graded row(s) do not agree with the progress DB on their own tree -- each is a row for its HQ:")
        for x in disagree:
            out("    " + x)
        return 1
    out(f"✅ AGREE [suite_rows_vs_progress]: {agree}/{graded} graded row(s) agree with the progress DB on their own tree.")
    return 0


def selftest():
    w = tempfile.mkdtemp(prefix="suite_rows_vs_progress.")
    try:
        os.makedirs(os.path.join(w, "corpus", "tests", "snobol4"))
        suites = os.path.join(w, "SUITES.tsv")
        db = os.path.join(w, "results.tsv")
        hdr = "key\tnick\temoji\tlang\tfirst_date\tfirst_pass\tfirst_total\ttoday_date\ttoday_pass\ttoday_total\ttree\tcriterion_changed\n"
        rows = ["sno-master\tSnoM\tx\tsnobol4\t2026-09-06\t1\t3\t2026-09-16\t2\t4\tfeedbeef1\tfixture\n",       # 2 PASS 1 FAIL + 1 sidecar OUTSIDE = 2/4 AGREE
                "snc-master\tSncM\tx\tsnocone\t2026-09-03\t1\t3\t2026-09-16\t1\t3\tfeedbeef1\tfixture\n",       # DB 2/3, the extra PASS is an xfail-noted one: gap == xpass
                "gimpel\tGimpel\tx\tsnobol4\t2026-09-04\t1\t2\t2026-09-16\t1\t2\tfeedbeef1\tfixture\n",         # DB 1/3 (1 PASS 1 FAIL 1 UNGRADED): denominator planted
                "roast\tRoast\tx\traku\t2026-09-03\t4\t986\t2026-09-13\t6\t986\tdeadbeef2\tfixture\n",           # no rows on its tree: UNPROVEN
                "gnu_fd\tGnuFD\tx\tprolog\t\t\t\t\t\t30\t\tDEFERRED by ruling CEO-579\n"]
        open(suites, "w").write("# fixture\n" + hdr + "".join(rows))
        open(os.path.join(w, "corpus", "tests", "snobol4", "ALL.outside.tsv"), "w").write("p_out\tORACLE_REFUSES\tfixture\n")
        def row(tree, suite, prog, mode, out, note=""):
            return f"2026-09-16T00:00:00\t{tree}\tc\tfixture\tmaster\t{suite}\tsnobol4\t{prog}\t{mode}\t{out}\t0\t{note}\n"
        lines = ["ts_utc\tscrip\tcorpus\tmeasurer\tclass\tsuite\tlang\tprogram\tmode\toutcome\tsecs\tnote\n"]
        for m in ("m3", "m4"):
            lines += [row("feedbeef1", "snobol4-master", "p1", m, "PASS"), row("feedbeef1", "snobol4-master", "p2", m, "PASS"), row("feedbeef1", "snobol4-master", "p3", m, "FAIL")]
            lines += [row("feedbeef1", "snocone-master", "s1", m, "PASS"), row("feedbeef1", "snocone-master", "s2", m, "PASS", "xfail"), row("feedbeef1", "snocone-master", "s3", m, "FAIL")]
            lines += [row("feedbeef1", "gimpel", "g1", m, "PASS"), row("feedbeef1", "gimpel", "g2", m, "CRASH"), row("feedbeef1", "gimpel", "g3", m, "UNGRADED")]
        lines += [row("feedbeef1", "snobol4-master", "p2", "m3", "FAIL"), row("feedbeef1", "snobol4-master", "p2", "m3", "PASS")]   # last row wins
        open(db, "w").write("".join(lines))
        fails = 0
        def ck(ok, label):
            nonlocal fails
            print(("  ok    " if ok else "  FAIL  ") + label)
            if not ok:
                fails += 1
        buf = []
        rc = audit(suites, db, os.path.join(w, "corpus"), out=buf.append)
        txt = "\n".join(buf)
        ck(rc == 1 and "sno-master" in txt and re.search(r"sno-master .*AGREE", txt) and re.search(r"snc-master .*DISAGREE: numerator row 1 vs DB 2 \(\+1\) = xpass: STALE MARKERS", txt) and
           re.search(r"gimpel .*DISAGREE: denominator row 2 vs DB population 3", txt) and re.search(r"roast .*UNPROVEN", txt) and re.search(r"gnu_fd .*DEFERRED", txt),
           "(a) rc=1: the agreeing row AGREEs, the planted numerator gap is named and equals its xpass (stale markers), the planted denominator gap is named by class, UNPROVEN and DEFERRED named")
        open(suites, "w").write("# fixture\n" + hdr + rows[0] + rows[3] + rows[4])
        buf = []
        rc = audit(suites, db, os.path.join(w, "corpus"), out=buf.append)
        ck(rc == 0 and "1 agree" in "\n".join(buf), "(b) rc=0 once the planted rows are removed (the agreeing, UNPROVEN and DEFERRED rows remain)")
        open(db, "w").write(lines[0])
        buf = []
        rc = audit(suites, db, os.path.join(w, "corpus"), out=buf.append)
        ck(rc == 2, "(c) rc=2 REFUSES on an empty DB (zero rows gradable)")
        print(f"population: 3 selftest arm(s), {fails} FAIL")
        print("SELFTEST " + ("PASS" if fails == 0 else "FAIL"))
        return 0 if fails == 0 else 1
    finally:
        shutil.rmtree(w, ignore_errors=True)


def main(argv):
    if "--selftest" in argv:
        return selftest()
    if any(a.startswith("-") for a in argv):
        sys.stderr.write("usage: util_suite_rows_vs_progress.py [--selftest]   (env: S4E_SUITES_TSV S4E_PROGRESS_DB S4E_CORPUS_ROOT)\n")
        return 2
    suites = os.environ.get("S4E_SUITES_TSV") or os.path.join(HERE, "..", "..", ".github", "SUITES.tsv")
    db = os.environ.get("S4E_PROGRESS_DB") or "/home/resources/progress/results.tsv"
    corpus = os.environ.get("S4E_CORPUS_ROOT") or os.path.join(HERE, "..", "..", "corpus")
    for f, what in ((suites, "SUITES.tsv"), (db, "the progress DB")):
        if not os.path.exists(f):
            print(f"⛔ REFUSES (rc=2): cannot read {what} at {f}")
            return 2
    return audit(suites, db, corpus)


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
