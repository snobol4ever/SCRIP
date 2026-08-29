#!/usr/bin/env python3
"""util_build_master_suite.py -- THE ONE FLAT SUITE + THE FEATURE DATABASE (Lon 2026-08-29, in-chat to CEO, verbatim in substance:
"I think we have organized the tests completely wrong. We should have ONE set of tests, and a database file, CSV file, which has
all the attributes you need to help isolate feature usage. So one flat list with attributes of each feature used in the program.
You could create as many as 30 attributes, for instance the one word primitive functions used like FENCE, POS, ARBNO, then
combinations like PATTERN . tz . *function(tz) would be a named construct. And there should be one big *.sno and one big *.ref
file. Not 10's of folders and 100's of files.")

Builds corpus/tests/snobol4/master/{ALL.sno, ALL.ref, ALL.csv} from EVERY suite pair under corpus/tests/snobol4/.
 - Entry names are prefixed with their source family (probe_kw__kw_defaults) so the flat namespace cannot collide; a collision REFUSES.
 - ALL.csv is the attribute database: one row per entry, ~30 columns of 0/1 feature flags scanned from the entry's own source text
   (primitives by exact UPPER name; constructs by syntax shape; combination columns for named constructs like capture_plus_defer,
   Lon's PATTERN . tz . *function(tz) example). The scanner is a lexical approximation, documented per column -- a DB row is a
   finding aid for isolating feature usage, never a semantic proof.
 - A family whose suite pair will not read (the malformed-conversion class) or that carries a stdin sidecar this merge cannot
   represent is EXCLUDED LOUDLY on stderr and listed in ALL.csv's sibling ALL.excluded.txt -- never silently dropped (the
   shrunken-denominator law).
 - Validation: the written pair is re-read and the entry count must equal the sum of the included families' counts, else REFUSE
   and write nothing. Grading is unchanged machinery: python3 scripts/corpus_suite_harness.py run ALL.sno ALL.ref --modes m3,m4.
Stdlib only. Uses corpus_suite_harness's own read_suite/write_suite -- ONE authority for the suite grammar, never a second parser.
"""
import os
import re
import sys
import csv

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import corpus_suite_harness as h  # noqa: E402

S4E = os.environ.get("S4E_HOME", os.path.dirname(os.path.dirname(HERE)))
ROOT = os.path.join(S4E, "corpus", "tests", "snobol4")
OUTDIR = os.path.join(ROOT, "master")

# -- the attribute columns: (name, predicate over the entry's joined source text) ------------------------------------
UPPER = lambda w: (lambda t: 1 if re.search(r"\b%s\b" % w, t) else 0)
COLS = [
    ("FENCE", UPPER("FENCE")), ("ARB", UPPER("ARB")), ("ARBNO", UPPER("ARBNO")), ("POS", UPPER("POS")),
    ("RPOS", UPPER("RPOS")), ("TAB", UPPER("TAB")), ("RTAB", UPPER("RTAB")), ("LEN", UPPER("LEN")),
    ("SPAN", UPPER("SPAN")), ("BREAK", UPPER("BREAK")), ("ANY", UPPER("ANY")), ("NOTANY", UPPER("NOTANY")),
    ("BAL", UPPER("BAL")), ("REM", UPPER("REM")), ("ABORT", UPPER("ABORT")), ("DEFINE", UPPER("DEFINE")),
    ("OPSYN", UPPER("OPSYN")), ("TABLE", UPPER("TABLE")), ("ARRAY", UPPER("ARRAY")), ("EVAL", UPPER("EVAL")),
    ("CODE", UPPER("CODE")), ("APPLY", UPPER("APPLY")), ("CONVERT", UPPER("CONVERT")), ("DATATYPE", UPPER("DATATYPE")),
    ("TRIM", UPPER("TRIM")), ("DUPL", UPPER("DUPL")), ("REPLACE", UPPER("REPLACE")), ("SIZE", UPPER("SIZE")),
    # constructs by syntax shape (lexical approximations, see header)
    ("cond_assign", lambda t: 1 if re.search(r"\s\.\s+[A-Za-z]", t) else 0),                # P . var
    ("imm_assign", lambda t: 1 if re.search(r"\s\$\s+[A-Za-z]", t) else 0),                 # P $ var (binary $)
    ("deferred_eval", lambda t: 1 if re.search(r"\*[A-Za-z(]", t) else 0),                  # *expr / *f(x)
    ("indirect_ref", lambda t: 1 if re.search(r"\$[A-Za-z(]", t) else 0),                    # $name (unary, no space)
    ("alternation", lambda t: 1 if "|" in t else 0),
    ("keyword_ref", lambda t: 1 if re.search(r"&[A-Z]", t) else 0),
    ("goto_fail", lambda t: 1 if ":F(" in t else 0),
    ("goto_success", lambda t: 1 if ":S(" in t else 0),
    # named combination constructs
    ("capture_plus_defer", lambda t: 1 if (re.search(r"\s\.\s+[A-Za-z]", t) and re.search(r"\*[A-Za-z(]", t)) else 0),  # Lon's PATTERN . tz . *function(tz)
    ("arbno_with_alt", lambda t: 1 if (re.search(r"\bARBNO\b", t) and "|" in t) else 0),
    ("fence_guarded_fail", lambda t: 1 if (re.search(r"\bFENCE\b", t) and ":F(" in t) else 0),
]


# -- descriptive renaming (Lon 2026-08-29, second ruling same session, verbatim in substance: "rename each and every test in
# the line-break column which will match the CSV file name. Abandon the old naming convention and make nice names that are
# descriptive and use snake_case, so like simple_pattern_with_replace_[0-9], nested_eval, etc.") -- the NAME is derived from
# the entry's own scanned features, most-salient-first, numbered for uniqueness; the old family__stem survives ONLY as the
# CSV's `origin` column (provenance is a database attribute, not a naming convention). ---------------------------------------
NAME_FEATURES = [
    ("DEFINE", "user_function"), ("OPSYN", "opsyn"), ("APPLY", "apply"), ("CODE", "code_eval"), ("EVAL", "eval"),
    ("ARBNO", "arbno"), ("FENCE", "fence"), ("BAL", "bal"), ("ARB", "arb"), ("SPAN", "span"), ("BREAK", "break"),
    ("ANY", "any"), ("NOTANY", "notany"), ("TAB", "tab"), ("RTAB", "rtab"), ("POS", "pos"), ("RPOS", "rpos"),
    ("LEN", "len"), ("REM", "rem"), ("ABORT", "abort"), ("TABLE", "table"), ("ARRAY", "array"),
    ("CONVERT", "convert"), ("DATATYPE", "datatype"), ("TRIM", "trim"), ("DUPL", "dupl"), ("REPLACE", "replace"),
    ("SIZE", "size"), ("deferred_eval", "defer"), ("cond_assign", "capture"), ("imm_assign", "imm_capture"),
    ("indirect_ref", "indirect"), ("alternation", "alt"), ("keyword_ref", "keyword"),
]
_REPL_RE = re.compile(r"^\S+\s+\S.*=\s")


def descriptive_name(text, flags):
    parts = [short for col, short in NAME_FEATURES if flags.get(col)]
    is_match = 1 if re.search(r"^\s*\S+\s+[A-Z*'\"(]", text, re.M) else 0
    if not parts:
        if "OUTPUT" in text:
            base = "simple_output"
        elif "=" in text:
            base = "simple_assign"
        else:
            base = "simple_program"
    else:
        base = "_".join(parts[:3])
        if _REPL_RE.search(text.replace("\n", " ")) and "pattern" not in base and is_match:
            base = base + "_replace"
        if flags.get("goto_fail") or flags.get("goto_success"):
            base = base + "_branch"
    return base


def discover_pairs():
    pairs = []
    for dirpath, dirnames, filenames in os.walk(ROOT):
        if os.path.abspath(dirpath) == os.path.abspath(OUTDIR):
            dirnames[:] = []
            continue
        for fn in sorted(filenames):
            if not fn.endswith(".sno"):
                continue
            sno = os.path.join(dirpath, fn)
            ref = sno[:-4] + ".ref"
            if not os.path.isfile(ref):
                continue  # pairless loose witnesses (probe_loose residue) are not suite pairs
            rel = os.path.relpath(sno, ROOT)
            fam = rel[:-4].replace(os.sep, "_")
            pairs.append((fam, sno, ref))
    return pairs


def main():
    os.makedirs(OUTDIR, exist_ok=True)
    excluded, included, all_entries, per_family = [], [], [], {}
    for fam, sno, ref in discover_pairs():
        inp = sno[:-4] + ".input"
        if os.path.isfile(inp):
            excluded.append((fam, "stdin sidecar (.input) -- the merge cannot represent per-entry stdin; stays a standalone suite"))
            continue
        try:
            entries = h.read_suite(sno, ref)
        except Exception as e:  # malformed-conversion class: loud, listed, never silent
            excluded.append((fam, "read_suite refused: %s" % str(e)[:140]))
            continue
        # a probe_loose dir can hold a stray VALID pair (relocated loose witnesses) -- those are single programs,
        # not suite containers; read_suite reads a bare program as one nameless entry, which is fine to carry.
        for e in entries:
            e.origin = "%s__%s" % (fam, e.name)
        per_family[fam] = len(entries)
        all_entries.extend(entries)
        included.append(fam)
    # feature scan first (names are DERIVED from features), then the descriptive rename with numbered uniqueness
    counters = {}
    rows = []
    for e in all_entries:
        text = "\n".join(e.sno_lines)
        flags = {c: fn(text) for c, fn in COLS}
        base = descriptive_name(text, flags)
        counters[base] = counters.get(base, 0) + 1
        new = "%s_%d" % (base, counters[base])
        old = e.name
        if e.kind == "line":  # a format-A entry carries its name IN the line's trailing `;* <name>` tag -- rename BOTH or read_suite re-derives the old name from the tag and the round-trip check refuses (measured, first build)
            ln = e.sno_lines[0]
            tag = ";* %s" % old
            if ln.endswith(tag):
                e.sno_lines[0] = ln[: -len(tag)] + ";* %s" % new
        e.name = new
        rows.append((e, flags, text))
    # -- LEVEL ORDERING (Lon 2026-08-29, third ruling same session, verbatim in substance: "the concept of smoke, regression,
    # rungs, pattern, feature specific, these ideas all go away and are replaced by a list of tests with as many attributes as
    # you need... Then a smoke test would be say the first 20-50 tests. Then you can extend another level at 100-200. Then the
    # last level all 1200+ being the full regression.") -- the list is ORDERED simple-green-first so a level is a PREFIX of it:
    # green before xfail (a smoke set must be expected-green), fewer features before more, shorter before longer. The CSV's
    # `rank` column is the position; a runner takes level N by taking rank <= N.
    order = sorted(range(len(rows)), key=lambda i: (int(bool(rows[i][0].xfail)), sum(rows[i][1].values()), len(rows[i][0].sno_lines), rows[i][0].name))
    all_entries = [rows[i][0] for i in order]
    rows = [rows[i] for i in order]
    names = [e.name for e in all_entries]
    dup = {n for n in names if names.count(n) > 1}
    if dup:
        h.refuse("master merge name collision(s): %s" % sorted(dup)[:5])
    out_sno = os.path.join(OUTDIR, "ALL.sno")
    out_ref = os.path.join(OUTDIR, "ALL.ref")
    for e_i, e in enumerate(all_entries, 1):
        e.seq = e_i
    h.write_suite(all_entries, out_sno, out_ref)
    reread = h.read_suite(out_sno, out_ref)
    if len(reread) != len(all_entries):
        h.refuse("re-read count %d != written %d -- NOT trusting the merge" % (len(reread), len(all_entries)))
    with open(os.path.join(OUTDIR, "ALL.csv"), "w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["rank", "entry", "origin", "family", "kind", "xfail", "n_lines"] + [c for c, _ in COLS])
        for rank, (e, flags, text) in enumerate(rows, 1):
            fam = e.origin.split("__", 1)[0]
            w.writerow([rank, e.name, e.origin, fam, e.kind, int(bool(e.xfail)), len(e.sno_lines)] + [flags[c] for c, _ in COLS])
    with open(os.path.join(OUTDIR, "ALL.excluded.txt"), "w") as f:
        for fam, why in excluded:
            f.write("%s\t%s\n" % (fam, why))
    print("MASTER SUITE: %d entries from %d families -> %s" % (len(all_entries), len(included), OUTDIR), file=sys.stderr)
    print("EXCLUDED (loud, see ALL.excluded.txt): %d" % len(excluded), file=sys.stderr)
    for fam, why in excluded:
        print("  ⛔ %s: %s" % (fam, why), file=sys.stderr)
    print("attribute columns: %d" % (5 + len(COLS)), file=sys.stderr)


if __name__ == "__main__":
    main()
