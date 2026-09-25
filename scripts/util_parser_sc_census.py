#!/usr/bin/env python3
"""util_parser_sc_census.py [--lang L[,L]] [--population corpus|ladder|master] [--declared TSV] [--jobs N] [--timeout S]

DOES EACH BOOTSTRAP PARSER WORK? (Lon 2026-09-23 15:2x, in-chat to the ceo: hq_snocone "to get all the SCRIP bootstrap
parsers working".) For every language, the Snocone-hosted parser SCRIP/bootstrap/parser_<lang>.sc -- loaded behind the
14-file bootstrap runtime in the order scripts/run_scrip_parser.sh declares, BY CONCATENATION (the Snocone branch of the
driver never reads a -L chain; util_parser_sc_census.sh's note) -- is run by ./scrip over every program of a declared
population with the program as its stdin. POPULATION ladder (default): every entry of the language's master whose ALL.csv
origin is ladder__* -- the construct ladder, one construct per rung, cut from the public reference; POPULATION master:
every entry of the master; POPULATION corpus, the default (Lon to hq_snocone 15:2x: "Test the parsers using all the corpus
sources"): every file of the language's extension under corpus/, the ALL.* containers and library/ excluded. CLASSIFICATION (hq_snocone's, kept): the first non-blank output line not starting with SEQ<n>
begins with '(' = PARSED (the parser emitted a tree); contains "Parse Error" = REFUSED; a clean exit (rc 0, no error line) that
printed nothing = EMPTY (hq_snocone 2026-09-25: a source holding only comments and control lines -- the gimpel *_driver.sno that are
one -INCLUDE -- has no statement to print, which is neither a tree shown nor a crash); anything else -- a crash, a timeout, heap
exhaustion, a runtime error -- = CRASH, its first line named, because a parser that dies is not a parser that declines.
It prints its denominator per language as each language finishes and names the first REFUSED and first CRASH. A REFUSED
source listed in --declared (default SCRIP/bootstrap/tests/parser_refusals.tsv: lang, corpus-relative path, the measurement
that the source is not a legal program of its language, or for an EMPTY source that it holds no statement) is DECLARED, never red;
an undeclared refusal or EMPTY is red, because a parser that declines a legal program, or prints nothing for one, is not working. rc 0 GREEN (every language: population > 0, CRASH 0, undeclared REFUSED 0),
1 RED, 2 REFUSE (no ./scrip, no master, a population that could not be materialized).
PARSED is a tree, not a CORRECT tree: equivalence with the C frontend's AST is the next bar, written in the row's GOAL.
"""
import concurrent.futures
import csv
import os
import re
import shutil
import subprocess
import sys
import tempfile

HERE = os.path.dirname(os.path.abspath(__file__))
SCRIP = os.path.dirname(HERE)
HOME = os.environ.get("S4E_HOME") or os.path.dirname(SCRIP)
CORPUS = os.path.join(HOME, "corpus")
RT = ["global", "case", "assign", "match", "counter", "stack", "tree", "ShiftReduce", "tdump", "gen", "qize", "semantic", "omega", "trace"]
LANGS = [("snobol4", "sno"), ("icon", "icn"), ("prolog", "pl"), ("raku", "raku"), ("pascal", "pas"), ("snocone", "sc"), ("rebus", "reb")]
SEQ = re.compile(r"^SEQ[0-9]")


def refuse(msg):
    print("REFUSE(2): " + msg)
    sys.exit(2)


def opt(argv, name, default):
    return argv[argv.index(name) + 1] if name in argv else default


def extract(src, ref, key, by_origin, out):
    cmd = [sys.executable, os.path.join(HERE, "corpus_suite_harness.py"), "extract", src, ref]
    cmd += ([out, "--origin", key] if by_origin else [key, out])
    cmd += ["--out-in", out + ".in"]
    r = subprocess.run(cmd, capture_output=True, text=True, timeout=120)
    return r.returncode == 0 and os.path.isfile(out), (r.stdout + r.stderr).strip().splitlines()[-1:] or [""]


def parse_one(scrip, chain, prog, timeout):
    try:
        with open(prog, "rb") as f:
            r = subprocess.run([scrip, chain], stdin=f, capture_output=True, timeout=timeout)
        text = (r.stdout + r.stderr).decode("utf-8", "replace")
        tag = ""
    except subprocess.TimeoutExpired as e:
        text = ((e.stdout or b"") + (e.stderr or b"")).decode("utf-8", "replace")
        tag = "TIMEOUT %ss" % timeout
    first = ""
    for line in text.splitlines():
        if line.strip() and not SEQ.match(line):
            first = line.strip()
            break
    if tag:
        return "CRASH", tag
    # ⛔ THE EXIT CODE AND EVERY LINE ARE READ, NOT THE FIRST LINE ALONE (hq_snocone 2026-09-23, measured): on SCRIP the
    # rebus parser prints its first tree and then SIGSEGVs, rc=139, which a first-line test called PARSED; and the parser's
    # refusal is a line that IS "Parse Error" -- beauty.sc prints the words, so its correct tree contains them.
    lines = [l.strip() for l in text.splitlines() if l.strip() and not SEQ.match(l)]
    if any(re.fullmatch(r"Parse Error\.?", l) for l in lines):
        return "REFUSED", "Parse Error"
    err = next((l for l in lines if re.search(r"\bERROR \d+ --|^Error \d+|FATAL|ZGC-STALE|CORRUPT CAPTURE", l)), "")
    if r.returncode != 0 or err:
        return "CRASH", "%s (rc=%d)" % (err or first or "no output", r.returncode)
    if not lines:
        return "EMPTY", "no output, rc=0"
    if first.startswith("("):
        return "PARSED", first
    return "CRASH", first or "(no output, rc=%d)" % r.returncode


def task(scrip, chain, lang, ext, src, ref, key, by_origin, d, timeout):
    if by_origin is None:
        prog = key
    else:
        safe = re.sub(r"[^A-Za-z0-9_.-]", "_", key)
        prog = os.path.join(d, safe + "." + ext)
        ok, why = extract(src, ref, key, by_origin, prog)
        if not ok:
            return lang, key, "UNEXTRACTED", why[0]
    cls, first = parse_one(scrip, chain, prog, timeout)
    return lang, key, cls, first


def main(argv):
    pop = opt(argv, "--population", "corpus")
    decl_path = opt(argv, "--declared", os.path.join(SCRIP, "bootstrap", "tests", "parser_refusals.tsv"))
    declared = set()
    if os.path.isfile(decl_path):
        for line in open(decl_path, encoding="utf-8"):
            f = line.rstrip("\n").split("\t")
            if len(f) >= 3 and not line.startswith("#") and f[2].strip():
                declared.add((f[0], f[1]))
    jobs = int(opt(argv, "--jobs", "6"))
    timeout = int(opt(argv, "--timeout", "10"))
    want = opt(argv, "--lang", "")
    langs = [l for l in LANGS if not want or l[0] in want.split(",")]
    if not langs:
        refuse("no language matches --lang " + want)
    if pop not in ("corpus", "ladder", "master"):
        refuse("--population is corpus, ladder or master")
    scrip = os.path.join(SCRIP, "scrip")
    if not os.access(scrip, os.X_OK):
        refuse("no ./scrip at " + scrip + " -- run make")
    sys.path.insert(0, HERE)
    import corpus_suite_harness as harness
    harness.require_fresh(harness.resolve_paths())
    boot = os.path.join(SCRIP, "bootstrap")
    tmp = tempfile.mkdtemp(prefix="parser_sc_census.")
    try:
        rt = b"".join(open(os.path.join(boot, n + ".sc"), "rb").read() for n in RT)
        work = []
        pops = {}
        for lang, ext in langs:
            parser = os.path.join(boot, "parser_%s.sc" % lang)
            if not os.path.isfile(parser):
                refuse("no bootstrap parser " + parser)
            chain = os.path.join(tmp, "chain_%s.sc" % lang)
            open(chain, "wb").write(rt + open(parser, "rb").read())
            if pop == "corpus":
                files = []
                for dp, dn, fn in os.walk(CORPUS):
                    dn[:] = [x for x in dn if x not in (".git", "library")]
                    files += [os.path.join(dp, x) for x in fn if x.endswith("." + ext) and not x.startswith("ALL.")]
                files.sort()
                pops[lang] = len(files)
                for fpath in files:
                    work.append((scrip, chain, lang, ext, "", "", fpath, None, "", timeout))
                continue
            tdir = os.path.join(CORPUS, "tests", lang)
            src, ref, idx = os.path.join(tdir, "ALL." + ext), os.path.join(tdir, "ALL.ref"), os.path.join(tdir, "ALL.csv")
            if not (os.path.isfile(src) and os.path.isfile(ref) and os.path.isfile(idx)):
                refuse("the %s master (ALL.%s, ALL.ref, ALL.csv) is not under %s" % (lang, ext, tdir))
            rows = list(csv.DictReader(open(idx, encoding="utf-8")))
            keys = [(r["origin"], True) for r in rows if r.get("origin", "").startswith("ladder__")] if pop == "ladder" else [(r["entry"], False) for r in rows if r.get("entry")]
            pops[lang] = len(keys)
            d = os.path.join(tmp, lang)
            os.makedirs(d, exist_ok=True)
            for key, by_origin in keys:
                work.append((scrip, chain, lang, ext, src, ref, key, by_origin, d, timeout))
        print("PARSER-SC CENSUS, population %s, tree %s, timeout %ss per program, jobs %d, declared refusals %d from %s" % (pop, subprocess.run(["git", "-C", SCRIP, "rev-parse", "--short", "HEAD"], capture_output=True, text=True).stdout.strip(), timeout, jobs, len(declared), decl_path if os.path.isfile(decl_path) else "(none)"), flush=True)
        print("%-8s %6s %7s %8s %8s %6s %6s %6s  %s" % ("lang", "pop", "PARSED", "REFUSED", "DECLARED", "CRASH", "EMPTY", "UNEXT", "first undeclared refusal / first crash / first empty"), flush=True)
        red = unext = tp = tn = 0
        for lang, _ in langs:
            r = {"PARSED": 0, "REFUSED": 0, "DECLARED": 0, "CRASH": 0, "EMPTY": 0, "UNEXTRACTED": 0, "first": {}}
            mine = [w for w in work if w[2] == lang]
            with concurrent.futures.ThreadPoolExecutor(max_workers=jobs) as ex:
                for _l, key, cls, first in ex.map(lambda w: task(*w), mine):
                    rel = os.path.relpath(key, CORPUS) if os.path.isabs(key) else key
                    if cls in ("REFUSED", "EMPTY") and (lang, rel) in declared:
                        cls = "DECLARED"
                    r[cls] += 1
                    r["first"].setdefault(cls, "%s [%s]" % (rel, first[:60]))
            n = pops[lang]
            tp += r["PARSED"]
            tn += n
            f = " | ".join(x for x in (r["first"].get("REFUSED", ""), r["first"].get("CRASH", ""), r["first"].get("EMPTY", ""), r["first"].get("UNEXTRACTED", "")) if x)
            print("%-8s %6d %7d %8d %8d %6d %6d %6d  %s" % (lang, n, r["PARSED"], r["REFUSED"], r["DECLARED"], r["CRASH"], r["EMPTY"], r["UNEXTRACTED"], f), flush=True)
            unext += r["UNEXTRACTED"]
            if n == 0 or r["CRASH"] or r["REFUSED"] or r["EMPTY"]:
                red += 1
    finally:
        shutil.rmtree(tmp, ignore_errors=True)
    print("PARSER-SC CENSUS: parsed=%d of %d over %d language(s); languages with a crash or an undeclared refusal: %d; unextracted: %d" % (tp, tn, len(langs), red, unext))
    if unext:
        print("REFUSE(2): %d program(s) of the population could not be materialized -- the census cannot say the parsers work over a population it did not read" % unext)
        return 2
    if red:
        print("RED: %d of %d bootstrap parser(s) crash on, or decline without a declared reason, a program of their %s population" % (red, len(langs), pop))
        return 1
    print("GREEN: every bootstrap parser parses its %s population to a tree, every refusal declared with its reason" % pop)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
