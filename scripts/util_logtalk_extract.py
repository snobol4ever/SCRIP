#!/usr/bin/env python3
"""util_logtalk_extract.py -- the extractor behind test_prolog_logtalk_suite.sh.

Row: prolog-logtalk-iso-3268-conformance-cases-have-no-runner (the row NAME under-counts; see below).
Lon 2026-09-11: "Let's get those Prolog programs graded."
Spiked by the ceo (SCRIP d7766980e), HARDENED AND OWNED by hq_R 2026-09-12.

⛔ THE POPULATION IS 3617, NOT 3268 -- census by head form across the 192 tests.lgt:
    test/2 3268 · succeeds/1 162 · throws/2 134 · fails/1 53  = 3617
The legacy three are the same three expectations spelled differently (succeeds == true, fails == false,
throws(Name,E) == error(E)). A parser that handles only test/2 silently grades 90% and reports 100%.

⛔⭐ WHAT THE HARDENING ACTUALLY FOUND, and it is not any of the four things the spike's own header owed.
The spike reported "3261 of 3268 test/2, 99.8%" and named the remainder as 7 stragglers. The real hole was
31 CASES IN ONE FILE (predicates/write_term_3, cases 115..145) and the cause was the ISO HEX ESCAPE:

    test(lgt_write_term_3_114, true(Assertion)) :- ..., {writeq('\\x2\\')}, ...

`\\x2\\` is ISO 6.4.2.1's numeric escape and it TERMINATES WITH A BACKSLASH. Every quote scanner in this
repo's python (and the spike's) treats a backslash as "escape the next character", so it ate the closing
quote, the string never closed, and the rest of the file -- 31 cases -- vanished into an unterminated
atom. ⭐ THE FAILURE MODE IS THE POINT: it did not raise, it did not warn, and it did not mis-parse a
case into a visibly wrong shape. It SHORTENED THE DENOMINATOR SILENTLY, and it did it in write_term_3 --
the file whose whole subject is exactly the escapes that broke the scanner reading it. A suite is most
likely to break the instrument reading it at precisely the construct that suite exists to test, so the
census a parser prints about a syntax suite must never be trusted without a second, independent count.
The second count here is `grep -c '^\\s*\\(test\\|succeeds\\|fails\\|throws\\)('` per file, and `--census`
prints BOTH and refuses when they disagree.

THE FILE SHAPE, which is what makes this tractable without Logtalk installed:
  * everything BEFORE `:- object(tests,` is the DATABASE: support clauses (a(1). a(2). b(X) :- ...) and
    directives (:- dynamic(unicorn/0).). It must be emitted with every case from that file.
  * `{Goal}` is Logtalk's escape to PLAIN PROLOG -- so no Logtalk runtime is needed.
  * `^^name(...)` are lgtunit FRAMEWORK calls. ⛔ THEY ARE NOT STRIPPABLE: 1141 of the 3617 cases call
    one, and 450 of those BIND the variable the expectation then calls (`^^text_output_assertion(Expected,
    Assertion)` with expectation `true(Assertion)`). Dropping the call leaves Assertion unbound, so the
    case raises instantiation_error and grades RED -- a false defect manufactured by the harness. They are
    mapped onto lib_logtalk_lgtunit.pl, the plain-Prolog shim; a helper the shim does not implement makes
    its case UNGRADED AND NAMED, never dropped and never silently passed.

HEAD FORMS, all four parsed here:
    test(Name, Expectation)                      3268
    test(Name, Expectation, Options)             (a subset of the above; options carry condition/setup/cleanup)
    test(Name)                                   7   -- lgtunit's "declared but not implemented" placeholder
    succeeds(Name) / fails(Name) / throws(Name, Ball)

API: parse_file(path) -> FileCases; parse_suite(root) -> (list[FileCases], list[(path, why)]).
CLI: util_logtalk_extract.py --census <root>   |   --show <tests.lgt> [n]
"""
import os
import re
import sys
import glob

HEADS = ("test", "succeeds", "fails", "throws")
# Directives INSIDE the object body that are plain Prolog and change what the cases mean. Everything else
# there (object/end_object/info/uses/public/... ) is Logtalk's own scaffolding and is dropped.
BODY_KEEP_DIRECTIVES = ("dynamic", "discontiguous", "multifile", "op", "set_prolog_flag")


def _scan_quoted(s, i):
    """s[i] opens a quote. Return the index just past the closing quote.

    ⛔ THE BACKSLASH RULE IS NOT "skip two". ISO 6.4.2.1 numeric escapes -- \\x2A\\ (hex) and \\101\\
    (octal) -- END WITH A BACKSLASH, so a two-character skip lands ON the terminating backslash, treats
    THAT as the start of a new escape, and swallows the closing quote. That is the 31-case hole this
    file's header describes. Single-character escapes (\\n \\' \\\\ ...) still skip two.
    """
    q = s[i]
    n = len(s)
    i += 1
    while i < n:
        ch = s[i]
        if ch == "\\" and i + 1 < n:
            if s[i + 1] == "x" or s[i + 1].isdigit():
                j = i + 2
                while j < n and s[j] != "\\":
                    j += 1
                i = min(j + 1, n)
                continue
            i += 2
            continue
        if ch == q:
            if i + 1 < n and s[i + 1] == q:   # doubled quote is an escaped quote
                i += 2
                continue
            return i + 1
        i += 1
    return n   # unterminated -- the caller decides; never silently absorb the rest of the file


def _char_literal_end(s, i):
    """s[i:i+2] == "0'". Return the index just past the character literal, or None if it is not one."""
    n = len(s)
    if i + 2 >= n:
        return None
    c = s[i + 2]
    if c == "\\":
        if i + 3 < n and (s[i + 3] == "x" or s[i + 3].isdigit()):
            j = i + 4
            while j < n and s[j] != "\\":
                j += 1
            return min(j + 1, n)
        return min(i + 4, n)
    if c == "'":                       # 0''' is the quote character; 0'' is also written
        return min(i + 4, n) if (i + 3 < n and s[i + 3] == "'") else min(i + 3, n)
    return i + 3


def split_clauses(src):
    """Split Prolog source into clause texts: comments removed, quoted material preserved verbatim."""
    out = []
    buf = []
    i = 0
    n = len(src)
    depth = 0
    while i < n:
        ch = src[i]
        if ch == "%":
            while i < n and src[i] != "\n":
                i += 1
            continue
        if ch == "/" and i + 1 < n and src[i + 1] == "*":
            j = src.find("*/", i + 2)
            i = n if j < 0 else j + 2
            continue
        if ch == "0" and i + 1 < n and src[i + 1] == "'":
            j = _char_literal_end(src, i)
            if j is not None:
                buf.append(src[i:j])
                i = j
                continue
        if ch in "'\"`":
            j = _scan_quoted(src, i)
            buf.append(src[i:j])
            i = j
            continue
        if ch in "([{":
            depth += 1
        elif ch in ")]}":
            depth -= 1
        if ch == "." and depth == 0 and (i + 1 >= n or src[i + 1] in " \t\n\r"):
            t = "".join(buf).strip()
            if t:
                out.append(t)
            buf = []
            i += 1
            continue
        buf.append(ch)
        i += 1
    t = "".join(buf).strip()
    if t:
        out.append(t)
    return out


def _skip_token(s, i):
    """Advance past one quote/char-literal at s[i], else return i unchanged."""
    if s[i] == "0" and i + 1 < len(s) and s[i + 1] == "'":
        j = _char_literal_end(s, i)
        if j is not None:
            return j
    if s[i] in "'\"`":
        return _scan_quoted(s, i)
    return i


def split_args(s):
    """Split an argument list at depth-0 commas, honouring quotes and character literals."""
    args = []
    depth = 0
    cut = 0
    i = 0
    n = len(s)
    while i < n:
        j = _skip_token(s, i)
        if j != i:
            i = j
            continue
        ch = s[i]
        if ch in "([{":
            depth += 1
        elif ch in ")]}":
            depth -= 1
        elif ch == "," and depth == 0:
            args.append(s[cut:i])
            cut = i + 1
        i += 1
    args.append(s[cut:])
    return [a.strip() for a in args]


def _close_paren(s):
    """s is the text after an opening '('. Return the index of its matching ')', or None."""
    depth = 1
    i = 0
    n = len(s)
    while i < n:
        j = _skip_token(s, i)
        if j != i:
            i = j
            continue
        ch = s[i]
        if ch in "([{":
            depth += 1
        elif ch in ")]}":
            depth -= 1
            if depth == 0:
                return i
        i += 1
    return None


class Case(object):
    __slots__ = ("file", "group", "kind", "name", "expect", "options", "goal", "helpers", "raw")

    def __init__(self, **kw):
        for k in self.__slots__:
            setattr(self, k, kw.get(k))


class FileCases(object):
    __slots__ = ("path", "group", "db", "loaded", "cases", "unparsed", "counted", "dead", "undecided")

    def __init__(self, **kw):
        for k in self.__slots__:
            setattr(self, k, kw.get(k))


# ⛔⭐ THE TESTER LOADS PLAIN PROLOG FILES AND THE CASES CANNOT PASS WITHOUT THEM. Seven directories ship a
# tester.lgt whose initialization calls '$lgt_load_prolog_file'(Directory + '<name>.pl') BEFORE running the
# tests: the predicates every case of directives/conditional_compilation calls (s1..s8, f01..f31) live in
# file.pl and nowhere else. Grade without it and all 39 raise existence_error -- a complete, plausible,
# entirely harness-made red on the one group whose subject is whether conditional compilation works at all
# (measured 2026-09-12 against the ceo's independent emulation, which inlines the same file and passes 39).
# ⭐ THE FILE IS NAMED BY THE VENDOR'S OWN tester.lgt, never guessed from the directory listing: op_3 and
# discontiguous_1 also ship file.pl, include_1/ensure_loaded_1/initialization_1/multifile_1 ship main.pl or
# file_1.pl, and a directory can ship a .pl the tester deliberately does NOT load.
_TESTER_LOADS = re.compile(r"atom_concat\(\s*Directory\s*,\s*'([^']+\.pl)'")


def _tester_loaded(path):
    """Clauses of the plain-Prolog files this directory's tester.lgt loads, in the order it loads them.
    Kept SEPARATE from the file's own database because they are loaded verbatim -- their :- if/endif guards
    are the thing under test in one group, so the directive filter that cleans a tests.lgt must not run."""
    d = os.path.dirname(path)
    t = os.path.join(d, "tester.lgt")
    if not os.path.exists(t):
        return []
    src = open(t, encoding="utf-8", errors="replace").read()
    if "$lgt_load_prolog_file" not in src:
        return []
    out = []
    for name in _TESTER_LOADS.findall(src):
        f = os.path.join(d, name)
        if os.path.exists(f):
            out.extend(split_clauses(open(f, encoding="utf-8", errors="replace").read()))
    return out


def _group_of(path):
    """The directory that names what is under test: .../predicates/atom_length_2/tests.lgt -> atom_length_2."""
    return os.path.basename(os.path.dirname(path))


def _helpers(goal):
    return sorted(set(re.findall(r"\^\^([a-z_]+)", goal)))


# ⛔⭐ A `:- if(...)` BLOCK INSIDE THE TEST OBJECT IS NOT DECORATION, AND READING BOTH OF ITS BRANCHES MAKES
# FAIL=0 UNREACHABLE BY CONSTRUCTION. format_2 guards ~n on os::operating_system_type(windows): the if-branch
# expects '\r\n' and the else-branch '\n' from the SAME goal under the SAME case name. Grade both and one of
# every pair is red whatever the engine does -- 14 cases in format_2/format_3 alone, and 21 suite-wide.
# ⭐ THE HONEST TREATMENT IS NOT "PICK ONE": it is to decide the guard where this runner can state the fact
# about ITSELF with certainty, and to leave both branches in the population (today's behaviour, and a named
# residual) where it cannot. We are on Linux and we are not any Prolog the suite names, so exactly those two
# conjunct forms are decidable here; everything else -- coinduction support, current_op, catch(1^true) --
# stays UNDECIDED and is reported by --census rather than silently resolved in one direction.
_DIALECTS = ("b", "ciao", "cx", "eclipse", "gnu", "ji", "quintus", "sicstus", "swi", "swipl", "trealla",
             "xsb", "xvm", "yap")


def _conjunct_value(t):
    t = t.strip()
    while t.startswith("(") and _close_paren(t[1:]) == len(t) - 2:
        t = t[1:-1].strip()
    if t.startswith("\\+"):
        v = _conjunct_value(t[2:])
        return None if v is None else (not v)
    m = re.match(r"os::operating_system_type\(\s*([a-z]+)\s*\)$", t)
    if m:
        return m.group(1) == "unix"
    m = re.match(r"current_logtalk_flag\(\s*prolog_dialect\s*,\s*([a-z][a-zA-Z0-9_]*)\s*\)$", t)
    if m:
        return False if m.group(1) in _DIALECTS else None
    return None


def cond_value(text):
    """True / False when this system decides the guard, else None (both branches stay in the population)."""
    t = text.strip()
    while t.startswith("(") and _close_paren(t[1:]) == len(t) - 2:
        t = t[1:-1].strip()
    vals = [_conjunct_value(x) for x in split_args(t)]
    if any(v is False for v in vals):
        return False
    if all(v is True for v in vals):
        return True
    return None


def parse_file(path):
    src = open(path, encoding="utf-8", errors="replace").read()
    i = src.find(":- object(")
    db_src, body_src = (src[:i], src[i:]) if i >= 0 else ("", src)
    # ⛔ EVERY db clause is kept, DIRECTIVES INCLUDED. A first cut filtered `:-` clauses down to
    # dynamic/discontiguous/op and that is a silent semantic edit: `:- set_prolog_flag(double_quotes, codes)`
    # and `:- initialization(...)` change what the cases below them MEAN, and a case graded without its
    # file's own flag settings is graded against a different language than the one the suite wrote it for.
    db = split_clauses(db_src)
    body_db = []
    cases = []
    unparsed = []
    dead = 0
    undecided = []
    guards = []            # one entry per open :- if(...): {"live": is this branch in the population, "done": a branch was definitely taken}
    for cl in split_clauses(body_src):
        g = re.match(r":-\s*(if|elif|else|endif)\b", cl)
        if g:
            k = g.group(1)
            if k in ("if", "elif"):
                b = cl.find("(")
                e = _close_paren(cl[b + 1:]) if b >= 0 else None
                arg = cl[b + 1:b + 1 + e] if e is not None else ""
                v = cond_value(arg)
                if v is None:
                    undecided.append(" ".join(arg.split())[:120])
                if k == "if":
                    guards.append({"live": v is not False, "done": v is True})
                elif guards:
                    guards[-1] = {"live": (not guards[-1]["done"]) and v is not False, "done": guards[-1]["done"] or v is True}
            elif k == "else":
                if guards:
                    guards[-1] = {"live": not guards[-1]["done"], "done": guards[-1]["done"]}
            else:
                if guards:
                    guards.pop()
            continue
        live = all(x["live"] for x in guards)
        m = re.match(r"(test|succeeds|fails|throws)\s*\(", cl)
        if m and not live:
            dead += 1
            continue
        if not live:
            continue
        if not m:
            # ⛔⭐ THE OBJECT BODY IS NOT ONLY TEST CLAUSES, and reading it as if it were manufactures
            # false reds by the hundred. 30+ files define their support predicates INSIDE the object,
            # under their own comment "auxiliary predicates used to delay errors to runtime" -- foo/2,
            # variable/1, three/1, small/1. Drop them and the case that calls foo(0, X) raises
            # existence_error(procedure, foo/2), which is a perfectly plausible ISO red and is entirely
            # the harness's doing: 373 of them on the first full run, concentrated in the arithmetic
            # families where an undefined support predicate looks exactly like an evaluable-functor gap.
            # ⭐ THE TELL WAS THE SHAPE OF THE ERROR, NOT ITS COUNT: real conformance reds name the
            # BUILT-IN under test, and these named a lowercase two-letter predicate no standard mentions.
            if cl.startswith(":-"):
                d = re.match(r":-\s*([a-z_]+)", cl)
                if not d or d.group(1) not in BODY_KEEP_DIRECTIVES:
                    continue
            body_db.append(cl)
            continue
        rest = cl[m.end():]
        j = _close_paren(rest)
        if j is None:
            unparsed.append((cl[:70], "head parenthesis never closes"))
            continue
        args = split_args(rest[:j])
        tail = rest[j + 1:].strip()
        kind = m.group(1)
        name = args[0].strip()
        if tail.startswith(":-"):
            goal = tail[2:].strip()
        elif not tail:
            goal = ""     # test(Name). -- lgtunit's not-implemented placeholder; legal, and not a parse failure
        else:
            unparsed.append((cl[:70], "clause neck is not ':-' and the head is not a fact"))
            continue
        expect, options = None, None
        if kind == "test":
            if len(args) >= 2:
                expect = args[1].strip()
            if len(args) >= 3:
                options = args[2].strip()
            if len(args) > 3:
                unparsed.append((cl[:70], "test/%d -- unknown arity" % len(args)))
                continue
        elif kind == "throws":
            if len(args) != 2:
                unparsed.append((cl[:70], "throws/%d -- expected throws/2" % len(args)))
                continue
            expect = "error_ball(%s)" % args[1].strip()
        cases.append(Case(file=path, group=_group_of(path), kind=kind, name=name, expect=expect,
                          options=options, goal=goal, helpers=_helpers(goal), raw=cl))
    db = db + body_db
    counted = 0
    for line in open(path, encoding="utf-8", errors="replace"):
        if re.match(r"^\s*(test|succeeds|fails|throws)\(", line):
            counted += 1
    return FileCases(path=path, group=_group_of(path), db=db, loaded=_tester_loaded(path), cases=cases,
                     unparsed=unparsed, counted=counted, dead=dead, undecided=undecided)


def parse_suite(root):
    """Return (list[FileCases], list[(path, why)]). A file whose two independent counts disagree is a REFUSAL."""
    files = sorted(glob.glob(os.path.join(root, "**", "tests.lgt"), recursive=True))
    out = []
    bad = []
    for f in files:
        fc = parse_file(f)
        for txt, why in fc.unparsed:
            bad.append((f, "%s :: %s" % (why, txt)))
        # ⛔ THE SECOND, INDEPENDENT COUNT. See this file's header: a parser reading a syntax suite fails
        # silently and shortens its own denominator. grep is a different instrument with a different blind
        # spot, so when the two agree the population is believable and when they disagree neither is.
        if fc.counted != len(fc.cases) + (fc.dead or 0):
            bad.append((f, "line-count says %d cases, the term parser says %d graded + %d guarded-out -- two "
                           "readings of one population disagree, so neither is published"
                        % (fc.counted, len(fc.cases), fc.dead or 0)))
        out.append(fc)
    return out, bad


def main(argv):
    if len(argv) >= 3 and argv[1] == "--census":
        files, bad = parse_suite(argv[2])
        tot = sum(len(f.cases) for f in files)
        by_kind, by_exp, by_help = {}, {}, {}
        for f in files:
            for c in f.cases:
                by_kind[c.kind] = by_kind.get(c.kind, 0) + 1
                if c.kind == "test":
                    k = re.match(r"[a-z_]+", c.expect or "")
                    k = k.group(0) if k else "(none)"
                    by_exp[k] = by_exp.get(k, 0) + 1
                for h in c.helpers:
                    by_help[h] = by_help.get(h, 0) + 1
        dead = sum((f.dead or 0) for f in files)
        und = {}
        for f in files:
            for u in (f.undecided or []):
                und.setdefault(u, []).append(f.group)
        print("files=%d cases=%d  (+%d guarded out by a :- if(...) this system decides FALSE)" % (len(files), tot, dead))
        for u, gs in sorted(und.items(), key=lambda x: -len(x[1])):
            print("  UNDECIDED GUARD in %d file(s) -- BOTH branches stay in the population: %s" % (len(gs), u))
        print("  by head form : %s" % sorted(by_kind.items(), key=lambda x: -x[1]))
        print("  by expectation: %s" % sorted(by_exp.items(), key=lambda x: -x[1]))
        print("  by ^^helper  : %s" % sorted(by_help.items(), key=lambda x: -x[1]))
        for p, why in bad:
            print("  REFUSE %s: %s" % (p, why))
        return 2 if bad else 0
    if len(argv) >= 3 and argv[1] == "--show":
        fc = parse_file(argv[2])
        n = int(argv[3]) if len(argv) > 3 else 6
        print("# %s  db-clauses=%d cases=%d (line count %d)" % (fc.path, len(fc.db), len(fc.cases), fc.counted))
        for c in fc.cases[:n]:
            print("  %-26s %-9s exp=%-28.28s helpers=%s" % (c.name, c.kind, c.expect or "-", ",".join(c.helpers) or "-"))
            print("      goal: %s" % " ".join(c.goal.split())[:150])
        return 0
    sys.stderr.write(__doc__.split("API:")[-1])
    return 2


if __name__ == "__main__":
    sys.exit(main(sys.argv))
