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

# -- ⭐ LANGUAGE PARAMETERISATION (hq_B 2026-08-29, ceo/Lon: masters for the other six languages).
# ROOT/ext/columns were hardcoded to snobol4. They are now selected by --lang; `snobol4` keeps its EXACT
# previous column set and naming table, so its master rebuilds BYTE-IDENTICAL -- that identity is the control
# arm for this change and is verified, not assumed.
# ⛔ snocone and rebus REUSE the snobol4 columns deliberately rather than getting invented ones: both are
# SNOBOL-family dialects over the same pattern primitives, and a fabricated column set would make their CSVs
# incomparable with snobol4's for no gain. prolog and raku get their own, because their primitives share
# almost nothing with the pattern vocabulary.
LANG_EXT = {"snobol4": ".sno", "prolog": ".pl", "raku": ".raku", "snocone": ".sc", "rebus": ".reb",
            "icon": ".icn", "pascal": ".pas"}

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

# -- prolog attributes: primitives are lowercase atoms, so the matcher differs from snobol4's UPPER ----------
LOW = lambda w: (lambda t: 1 if re.search(r"(?<![A-Za-z0-9_])%s\s*\(" % w, t) else 0)   # name( -- a CALL, not a word
BARE = lambda w: (lambda t: 1 if re.search(r"(?<![A-Za-z0-9_])%s(?![A-Za-z0-9_])" % w, t) else 0)
PROLOG_COLS = [
    ("findall", LOW("findall")), ("bagof", LOW("bagof")), ("setof", LOW("setof")), ("forall", LOW("forall")),
    ("assert", LOW("assert")), ("asserta", LOW("asserta")), ("assertz", LOW("assertz")), ("retract", LOW("retract")),
    ("abolish", LOW("abolish")), ("catch", LOW("catch")), ("throw", LOW("throw")), ("between", LOW("between")),
    ("functor", LOW("functor")), ("arg", LOW("arg")), ("copy_term", LOW("copy_term")), ("call", LOW("call")),
    ("var", LOW("var")), ("nonvar", LOW("nonvar")), ("atom", LOW("atom")), ("number", LOW("number")),
    ("write", LOW("write")), ("writeq", LOW("writeq")), ("write_canonical", LOW("write_canonical")),
    ("format", LOW("format")), ("nl", BARE("nl")), ("clause", LOW("clause")), ("succ", LOW("succ")),
    ("op", LOW("op")),
    # constructs by syntax shape
    ("cut", lambda t: 1 if re.search(r"(^|[\s,(])!\s*[,.)]", t) else 0),
    ("dcg", lambda t: 1 if "-->" in t else 0),
    ("if_then_else", lambda t: 1 if ("->" in t and ";" in t) else 0),
    ("negation", lambda t: 1 if "\\+" in t else 0),
    ("unify", lambda t: 1 if re.search(r"[^:=<>\\]=[^=<>]", t) else 0),
    ("univ", lambda t: 1 if "=.." in t else 0),
    ("list_cons", lambda t: 1 if re.search(r"\[[^\]]*\|[^\]]*\]", t) else 0),
    ("directive", lambda t: 1 if re.search(r"^\s*:-", t, re.M) else 0),
    # named combinations
    ("findall_with_var_goal", lambda t: 1 if (re.search(r"findall\s*\(", t) and re.search(r"findall\s*\([^,]*,\s*[A-Z_]", t)) else 0),
    ("catch_with_throw", lambda t: 1 if (re.search(r"catch\s*\(", t) and re.search(r"throw\s*\(", t)) else 0),
    ("assert_then_retract", lambda t: 1 if (re.search(r"assert[az]?\s*\(", t) and re.search(r"retract\s*\(", t)) else 0),
]
PROLOG_NAME_FEATURES = [
    ("findall", "findall"), ("bagof", "bagof"), ("setof", "setof"), ("forall", "forall"),
    ("catch", "catch"), ("throw", "throw"), ("assert", "assert"), ("asserta", "asserta"), ("assertz", "assertz"),
    ("retract", "retract"), ("abolish", "abolish"), ("between", "between"), ("clause", "clause"),
    ("functor", "functor"), ("copy_term", "copy_term"), ("call", "call"), ("op", "op"),
    ("dcg", "dcg"), ("cut", "cut"), ("if_then_else", "ite"), ("negation", "naf"), ("univ", "univ"),
    ("list_cons", "list"), ("write_canonical", "write_canonical"), ("writeq", "writeq"), ("format", "format"),
    ("directive", "directive"),
]
RAKU_COLS = [
    ("sub", BARE("sub")), ("multi", BARE("multi")), ("my", BARE("my")), ("our", BARE("our")),
    ("say", LOW("say")), ("print", LOW("print")), ("given", BARE("given")), ("when", BARE("when")),
    ("if", BARE("if")), ("elsif", BARE("elsif")), ("unless", BARE("unless")), ("for", BARE("for")),
    ("while", BARE("while")), ("loop", BARE("loop")), ("return", BARE("return")), ("class", BARE("class")),
    ("has", BARE("has")), ("method", BARE("method")), ("regex", BARE("regex")), ("token", BARE("token")),
    ("grep", LOW("grep")), ("map", LOW("map")), ("sort", LOW("sort")), ("join", LOW("join")),
    ("split", LOW("split")), ("sprintf", LOW("sprintf")), ("die", LOW("die")), ("try", BARE("try")),
    ("sigil_scalar", lambda t: 1 if re.search(r"\$[A-Za-z_]", t) else 0),
    ("sigil_array", lambda t: 1 if re.search(r"@[A-Za-z_]", t) else 0),
    ("sigil_hash", lambda t: 1 if re.search(r"%[A-Za-z_]", t) else 0),
    ("junction", lambda t: 1 if re.search(r"\b(any|all|none|one)\s*\(", t) else 0),
    ("range_op", lambda t: 1 if ".." in t else 0),
    ("feed_op", lambda t: 1 if ("==>" in t or "<==" in t) else 0),
    ("smartmatch", lambda t: 1 if "~~" in t else 0),
    ("string_concat", lambda t: 1 if "~" in t else 0),
    ("sub_with_return", lambda t: 1 if (re.search(r"\bsub\b", t) and re.search(r"\breturn\b", t)) else 0),
    ("for_with_say", lambda t: 1 if (re.search(r"\bfor\b", t) and re.search(r"say\s*\(?", t)) else 0),
    ("regex_with_smartmatch", lambda t: 1 if (re.search(r"\b(regex|token|rule)\b", t) and "~~" in t) else 0),
]
RAKU_NAME_FEATURES = [
    ("class", "class"), ("method", "method"), ("multi", "multi"), ("sub", "sub"), ("token", "token"),
    ("regex", "regex"), ("given", "given"), ("when", "when"), ("for", "for"), ("while", "while"),
    ("loop", "loop"), ("grep", "grep"), ("map", "map"), ("sort", "sort"), ("join", "join"), ("split", "split"),
    ("junction", "junction"), ("smartmatch", "smartmatch"), ("feed_op", "feed"), ("range_op", "range"),
    ("sigil_hash", "hash"), ("sigil_array", "array"), ("say", "say"), ("try", "try"), ("die", "die"),
]
LANG_TABLES = {
    "snobol4": (COLS, NAME_FEATURES),   # unchanged -- byte-identity control arm
    "snocone": (COLS, NAME_FEATURES),   # SNOBOL-family: same pattern vocabulary, deliberately shared
    "rebus":   (COLS, NAME_FEATURES),   # SNOBOL-family: same pattern vocabulary, deliberately shared
    "prolog":  (PROLOG_COLS, PROLOG_NAME_FEATURES),
    "raku":    (RAKU_COLS, RAKU_NAME_FEATURES),
}



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


def discover_pairs(ROOT, OUTDIR, EXT):
    pairs = []
    for dirpath, dirnames, filenames in os.walk(ROOT):
        if os.path.abspath(dirpath) == os.path.abspath(OUTDIR):
            dirnames[:] = []
            continue
        for fn in sorted(filenames):
            if not fn.endswith(EXT):
                continue
            sno = os.path.join(dirpath, fn)
            ref = sno[:-len(EXT)] + ".ref"
            if not os.path.isfile(ref):
                continue  # pairless loose witnesses (probe_loose residue) are not suite pairs
            rel = os.path.relpath(sno, ROOT)
            fam = rel[:-len(EXT)].replace(os.sep, "_")
            pairs.append((fam, sno, ref))
    return pairs


def main():
    # ⭐ --lang selects root, extension and attribute tables. Default snobol4 keeps the previous behaviour
    # exactly, so `util_build_master_suite.py` with no arguments rebuilds a byte-identical master.
    lang = "snobol4"
    for i, a in enumerate(sys.argv[1:]):
        if a == "--lang" and i + 2 <= len(sys.argv[1:]):
            lang = sys.argv[i + 2]
        elif a.startswith("--lang="):
            lang = a.split("=", 1)[1]
    if lang not in LANG_TABLES:
        sys.stderr.write("REFUSED: no attribute table for language %r. Known: %s\n"
                         "  A master built with a fabricated column set is worse than no master -- add the\n"
                         "  language's own primitives deliberately rather than defaulting to another dialect's.\n"
                         % (lang, ", ".join(sorted(LANG_TABLES))))
        raise SystemExit(2)
    EXT = LANG_EXT[lang]
    _CO = h.LANG_CONFIGS[lang]["comment_open"] if lang in h.LANG_CONFIGS else "*"
    _CC = h.LANG_CONFIGS[lang].get("comment_close", "") if lang in h.LANG_CONFIGS else ""
    ROOT = os.path.join(S4E, "corpus", "tests", lang)
    OUTDIR = os.path.join(ROOT, "master")
    if not os.path.isdir(ROOT):
        sys.stderr.write("REFUSED: no corpus tree at %s -- nothing to build from.\n" % ROOT)
        raise SystemExit(2)
    global COLS, NAME_FEATURES
    COLS, NAME_FEATURES = LANG_TABLES[lang]
    os.makedirs(OUTDIR, exist_ok=True)
    excluded, included, all_entries, per_family = [], [], [], {}
    for fam, sno, ref in discover_pairs(ROOT, OUTDIR, EXT):
        inp = sno[:-4] + ".input"
        if os.path.isfile(inp):
            excluded.append((fam, "stdin sidecar (.input) -- the merge cannot represent per-entry stdin; stays a standalone suite"))
            continue
        try:
            # ⛔⭐ READ EACH SOURCE PAIR WITH ITS OWN DIALECT READER. read_suite() parses the SNOBOL4 `*---`
            # banner; pointed at a .pl/.sc/.reb pair it sees NO banners, returns the whole file as ONE entry,
            # and that entry's body then still contains its `%---` banners. Writing that into the master
            # emitted stray in-body banners (measured: a `%--- 1 anon_in_...` line landed between seq 8 and
            # seq 9 of ALL.pl) which the reader then parsed as real entries -- content that looks like
            # structure, the same class as everything else today.
            if lang == "snobol4":
                entries = h.read_suite(sno, ref)
            else:
                try:
                    entries = h.read_block_suite(sno, ref, h.banner_re_for(_CO, _CC))
                except Exception:
                    entries = h.read_suite(sno, ref)   # a genuine one-line-dialect pair
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
    out_sno = os.path.join(OUTDIR, "ALL" + EXT)   # ⭐ per-language: ALL.sno / ALL.pl / ALL.raku / ALL.sc / ALL.reb
    out_ref = os.path.join(OUTDIR, "ALL.ref")
    for e_i, e in enumerate(all_entries, 1):
        e.seq = e_i
    # ⛔⭐ DIALECTS MUST USE THE BLOCK WRITER/READER, NOT THE SNOBOL4 PAIR. write_suite()/read_suite() emit and
    # parse the SNOBOL4 `*---` banner; pointing them at a .pl/.raku/.sc/.reb file writes banners the language's
    # own reader cannot match, and every entry comes back named seqN. MEASURED on the first build attempt: all
    # four languages failed the round-trip check with wrote==read counts but seq-names -- the harness caught it
    # exactly as designed, which is why nothing corrupt was written.
    # snobol4 keeps write_suite() so its master stays byte-identical (verified control arm).
    if lang == "snobol4":
        h.write_suite(all_entries, out_sno, out_ref)
        reread = h.read_suite(out_sno, out_ref)
    else:
        _co, _cc = _CO, _CC
        h.write_block_suite(all_entries, out_sno, out_ref, _co, _cc)
        reread = h.read_block_suite(out_sno, out_ref, h.banner_re_for(_co, _cc))
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
