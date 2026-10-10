#!/usr/bin/env python3
"""util_gen_prolog_prelude_libs.py -- SWI-PROLOG'S PURE-PROLOG LIBRARIES AS PRELUDE SOURCES (CEO-1473; cfo 2026-10-10,
row prolog-swi-library-modules-rbtrees-assoc-ordsets-apply-error-pairs-aggregate-option-strings-516-swi-cases).

THE RULING (CEO-1473, approved inside CEO-1352 (5)): swipl's pure-Prolog libraries are vendored under
src/parsers/prolog/prelude/<lib>.pl with their BSD headers, embedded by a generator at build like the parsers, pulled only
by the prelude's existing name-reference closure (a program naming none pays nothing); no run-time file loading.  A
library whose closure names a C-side builtin SCRIP lacks gets that builtin as a row first or the predicate hand-typed,
never a silent stub.

WHAT THIS SCRIPT DOES.  It reads every src/parsers/prolog/prelude/*.pl (VERBATIM copies of swipl 9.0.4's library files,
the version every SWI ref was cut from) and writes src/parsers/prolog/prolog_prelude_libs.inc, a committed table of
const data that prolog_parse.c includes.  Per library it emits the export list (name/arity, a DCG export name//N as
N+2) and its SOURCE as SCRIP will parse it.  THE INJECTOR (prolog_parse.c pl_libs_append) parses a library only when the
program references one of its exports by name/arity (or by the bare name, a closure) that the BASE prelude does not
define -- the base wins per key, so a reference the base answers selects nothing -- and then, from the name/arity
references it finds in the parsed library, selects the libraries that one needs, to a fixpoint.  The embedded text:

  (1) THE MODULE IS FLATTENED, NOT IGNORED.  SCRIP has no module system, so a library's non-exported predicates would
      share one namespace with the user's program, with the base prelude and with every other library (assoc.pl and
      rbtrees.pl both define helpers by the same names).  Every predicate a library defines and does not export is
      renamed '$<lib>_<name>' AT ITS PREDICATE REFERENCES ONLY -- its heads, the body goals that call it, a meta-argument
      that passes it as a closure, a DCG nonterminal -- and never where the same atom is data (library(error) has a
      helper text/1 and the type name text in has_type(text, X)).  Which occurrences are references is read by SWI's
      own reader, util_gen_prolog_prelude_libs_refs.pl, from the subterm positions of each clause under the library's
      operators and meta-predicate declarations; it requires the vendored file to be byte-identical to swipl's own copy.
      A reference written in operator form REFUSES rc=2 (a renamed operator would not parse), and --show lists every
      helper name left standing as data, for review.  The '$' prefix is what the injector already keys on: a '$' name
      is pulled only through the closure of what the program calls.
  (2) DIRECTIVES.  The module system's own directives (module, use_module, autoload, meta_predicate, multifile, public,
      det, set_prolog_flag ...) are dropped, each counted on stdout; conditional compilation (if/elif/else/endif) is
      kept for SCRIP's reader.  ANY OTHER DIRECTIVE REFUSES rc=2 by name: a directive that changes what a library means
      (dynamic, initialization, a term-expansion trigger) is decided when a library brings one, never dropped silently.
  (3) HOOK CLAUSES.  A module-qualified head (sandbox:safe_primitive(...), prolog:message(...)) and a term_expansion or
      goal_expansion clause are dropped and named: they extend another module or the loader, and the expansion clause
      would turn on the program's own expansion machinery.
  (4) COMMENTS AND BLANK LINES ARE STRIPPED from the embedded text (the vendored file keeps them); the text is otherwise
      the library's own, token for token.  Its "..." literals are strings, as SWI reads them: the injector marks every
      library clause dq_forced, so prolog_lower.c lowers it in double_quotes=string whatever the program's flag.
  (6) A DICT-METHOD clause (Dict.method(...) := Value :- Body, read as :=/2) is dropped and named: SCRIP has no dicts.
  (5) OMIT: an export SCRIP already answers natively (a lowering the library's clauses would override, because a file
      definition takes precedence over a special-cased builtin) is listed in OMIT below with its reason and its clauses
      are dropped.  An export the base prelude defines needs no entry: the injector drops it (the base prelude wins).

A head this script cannot key (an operator-form head, a variable head) REFUSES rc=2 by name.  An export with no clause in
the library is SWI's C (term_hash/2 in library(terms)): it leaves the export table and --show names it as C-side, so a
program calling it reaches SCRIP's builtin or an existence error, never a stub.

USAGE: util_gen_prolog_prelude_libs.py [--dir DIR] [--out PATH] [--check | --show LIB]
  --dir     the vendored directory (default src/parsers/prolog/prelude; the sync gate's self-test points it at a plant)
  --check   regenerate in memory and compare with the committed table; rc 1 if it differs (the sync gate's arm)
  --show    print LIB's embedded source as SCRIP will parse it, its exports, renames and drops
rc 0 written / in sync; 1 stale (--check); 2 refused (a construct named above, or a missing prelude directory).
"""
import argparse, os, re, shutil, subprocess, sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
PRELUDE_DIR = os.path.join(ROOT, "src", "parsers", "prolog", "prelude")
DEFAULT_OUT = os.path.join(ROOT, "src", "parsers", "prolog", "prolog_prelude_libs.inc")
REFS_PL = os.path.join(HERE, "util_gen_prolog_prelude_libs_refs.pl")

DROP_DIRECTIVES = {"module", "use_module", "autoload", "meta_predicate", "multifile", "public", "det", "set_prolog_flag",
                   "$clausable", "noprofile", "license", "discontiguous", "module_transparent", "predicate_options",
                   "create_prolog_flag", "reexport", "type", "pred", "quasi_quotation_syntax", "volatile", "encoding", "$hide"}
KEEP_DIRECTIVES = {"if", "elif", "else", "endif"}
HOOK_HEADS = {("term_expansion", 2), ("goal_expansion", 2), ("term_expansion", 4), ("goal_expansion", 4)}
OMIT = {"aggregate": {("foreach", 2): "SWI's foreach/2 resets its template between solutions with '$unbind_template'/1, SWI C; the base prelude "
                                      "hand-types it as SWI 6 did, copying the goal per solution with its other variables kept shared"},
        "strings": {("string", 4): "quasi-quotation support ({|string(X)||...|}); SCRIP's reader has no quasi-quotations, and the bare name string "
                                    "would select the library in every program that tests string/1"}}
DICT_METHOD_HEADS = {(":=", 2)}

SYMCH = set("#$&*+-./:<=>?@^~\\")
ALNUM = re.compile(r"[A-Za-z0-9_]")


class Refuse(Exception):
    pass


def tokenize(src, path):
    toks = []
    i, n = 0, len(src)
    while i < n:
        c = src[i]
        if c.isspace():
            j = i
            while j < n and src[j].isspace():
                j += 1
            toks.append(("ws", src[i:j])); i = j; continue
        if c == "%":
            j = src.find("\n", i)
            j = n if j < 0 else j
            toks.append(("comment", src[i:j])); i = j; continue
        if c == "/" and src.startswith("/*", i):
            j = src.find("*/", i + 2)
            if j < 0:
                raise Refuse("%s: an unclosed /* comment" % path)
            toks.append(("comment", src[i:j + 2])); i = j + 2; continue
        if c.isdigit():
            j = i
            if src.startswith("0'", i):
                j = i + 2
                if j < n and src[j] == "\\":
                    j += 2
                    if src[j - 1] in "xX0123456789":
                        while j < n and src[j] != "\\":
                            j += 1
                        j += 1
                elif src.startswith("''", j):
                    j += 2
                else:
                    j += 1
                toks.append(("num", src[i:j])); i = j; continue
            if re.match(r"0[xob]", src[i:i + 2]):
                j = i + 2
                while j < n and (src[j].isalnum() or src[j] == "_"):
                    j += 1
                toks.append(("num", src[i:j])); i = j; continue
            m = re.compile(r"\d[\d_]*(\.\d+)?([eE][+-]?\d+)?(Inf|NaN)?").match(src, i)
            toks.append(("num", m.group(0))); i = m.end(); continue
        if c.isalpha() or c == "_":
            j = i
            while j < n and ALNUM.match(src[j]):
                j += 1
            toks.append(("var" if (c.isupper() or c == "_") else "atom", src[i:j])); i = j; continue
        if c in "'\"`":
            j = i + 1
            while True:
                if j >= n:
                    raise Refuse("%s: an unclosed %s literal" % (path, c))
                if src[j] == "\\":
                    j += 2; continue
                if src[j] == c:
                    if j + 1 < n and src[j + 1] == c:
                        j += 2; continue
                    break
                j += 1
            kind = {"'": "qatom", '"': "str", "`": "bq"}[c]
            toks.append((kind, src[i:j + 1])); i = j + 1; continue
        if c in "()[]{},|":
            toks.append(("punct", c)); i += 1; continue
        if c in "!;":
            toks.append(("atom", c)); i += 1; continue
        if c in SYMCH:
            j = i
            while j < n and src[j] in SYMCH:
                j += 1
            if src[i:j] == "." and (j >= n or src[j].isspace() or src[j] == "%"):
                toks.append(("end", ".")); i = j; continue
            toks.append(("atom", src[i:j])); i = j; continue
        raise Refuse("%s: an unexpected character %r at offset %d" % (path, c, i))
    return toks


def atom_value(tok):
    kind, text = tok
    if kind == "atom":
        return text
    if kind == "qatom":
        return text[1:-1].replace("''", "'")
    return None


def split_clauses(toks):
    clauses, cur = [], []
    for t in toks:
        cur.append(t)
        if t[0] == "end":
            clauses.append(cur); cur = []
    if any(t[0] not in ("ws", "comment") for t in cur):
        raise Refuse("text after the last clause's end token")
    return clauses


def sig(clause):
    return [t for t in clause if t[0] not in ("ws", "comment")]


def top_level_args(s, k):
    """s[k] is '(' or '[': the token lists of its top-level comma-separated elements and the index after the close."""
    depth, args, cur = 0, [], []
    j = k
    while j < len(s):
        t = s[j]
        if t[0] == "punct" and t[1] in "([{":
            depth += 1
            if depth == 1:
                j += 1; continue
        elif t[0] == "punct" and t[1] in ")]}":
            depth -= 1
            if depth == 0:
                if cur:
                    args.append(cur)
                return args, j + 1
        elif t[0] == "punct" and t[1] in ",|" and depth == 1:
            args.append(cur); cur = []; j += 1; continue
        cur.append(t)
        j += 1
    raise Refuse("an unbalanced bracket")


def parse_exports(s, path):
    """s are the significant tokens of ':- module(Name, [Exports])'.  Returns (module name, [(name, arity)])."""
    args, _ = top_level_args(s, 1)
    if len(args) < 2:
        raise Refuse("%s: a module directive without an export list" % path)
    mod = atom_value(args[0][0])
    lst = args[1]
    if not lst or lst[0] != ("punct", "["):
        raise Refuse("%s: the export list is not a list" % path)
    elems, _ = top_level_args(lst, 0)
    exports = []
    for e in elems:
        e = [t for t in e if t != ("punct", "(") and t != ("punct", ")")]
        if e and atom_value(e[0]) == "op":
            raise Refuse("%s: exports an operator (%s) -- decide where SCRIP's reader learns it before vendoring" % (path, " ".join(t[1] for t in e)))
        if len(e) == 3 and atom_value(e[0]) is not None and atom_value(e[1]) in ("/", "//") and e[2][0] == "num":
            exports.append((atom_value(e[0]), int(e[2][1]) + (2 if atom_value(e[1]) == "//" else 0)))
            continue
        raise Refuse("%s: an export this generator cannot read (%s)" % (path, " ".join(t[1] for t in e)))
    return mod, exports


def quote(name):
    return "'" + name.replace("\\", "\\\\").replace("'", "\\'") + "'"


def strip_comments(text, path):
    out = []
    for t in tokenize(text, path):
        out.append(("\n" if ("\n" in t[1] or t[1].startswith("%")) else " ") if t[0] == "comment" else t[1])
    return "".join(out)


def reader_refs(path, lib):
    """SWI's own reader over the vendored file (util_gen_prolog_prelude_libs_refs.pl): every clause's start offset and
    head key, and every predicate reference's span -- head, body goal, meta-argument, DCG nonterminal -- never data."""
    if not shutil.which("swipl"):
        raise Refuse("swipl is not on PATH -- the predicate references of %s are read by SWI's own reader" % path)
    r = subprocess.run(["swipl", REFS_PL, "--", path, lib], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    if r.returncode != 0:
        raise Refuse("SWI's reader refused %s (rc=%d): %s" % (path, r.returncode, r.stderr.strip()[:300]))
    heads, refs = {}, []
    for line in r.stdout.split("\n"):
        f = line.split(" ", 5) if line.startswith("R ") else line.split(" ", 4)
        if line.startswith("C "):
            heads[int(f[1])] = (f[2], int(f[3]), f[4])
        elif line.startswith("R "):
            refs.append((int(f[1]), int(f[2]), int(f[3]), f[4], f[5]))
    return heads, refs


def process(path):
    lib = os.path.splitext(os.path.basename(path))[0]
    src = open(path, encoding="utf-8").read()
    toks = tokenize(src, path)
    for no, cl in enumerate(split_clauses(toks), 1):
        s = sig(cl)
        if s and atom_value(s[0]) == ":-" and len(s) > 1:
            d = atom_value(s[1])
            if d not in KEEP_DIRECTIVES and d not in DROP_DIRECTIVES and d != "module":
                raise Refuse("%s clause %d: the directive :- %s is neither dropped nor kept by this generator -- decide what it means for SCRIP first" % (path, no, d))
    heads, refs = reader_refs(path, lib)
    mod, exports, kept, drops, keyed = None, [], [], {}, []
    off = 0
    for no, cl in enumerate(split_clauses(toks), 1):
        start = off
        off += sum(len(t[1]) for t in cl)
        first = start
        for t in cl:
            if t[0] not in ("ws", "comment"):
                break
            first += len(t[1])
        s = sig(cl)
        kind, arity, name = heads.get(first, (None, 0, None))
        if kind is None:
            raise Refuse("%s clause %d: SWI's reader reports no clause at offset %d where this tokenizer starts one" % (path, no, first))
        if kind == "directive":
            d = atom_value(s[1]) if len(s) > 1 else None
            if d == "module" and mod is None:
                mod, exports = parse_exports(s[1:], path)
                drops["directive module"] = drops.get("directive module", 0) + 1
                continue
            if d in KEEP_DIRECTIVES:
                kept.append((start, off)); continue
            if d in DROP_DIRECTIVES:
                drops["directive " + d] = drops.get("directive " + d, 0) + 1
                continue
            raise Refuse("%s clause %d: the directive :- %s is neither dropped nor kept by this generator -- decide what it means for SCRIP first" % (path, no, d))
        if kind == "unkeyable":
            raise Refuse("%s clause %d: a head this generator cannot key (%s)" % (path, no, " ".join(t[1] for t in s[:6])))
        if kind == "qualified":
            drops["module-qualified head"] = drops.get("module-qualified head", 0) + 1
            continue
        if (name, arity) in DICT_METHOD_HEADS:
            drops["dict-method clause (SCRIP has no dicts)"] = drops.get("dict-method clause (SCRIP has no dicts)", 0) + 1
            continue
        if (name, arity) in HOOK_HEADS:
            drops["hook %s/%d" % (name, arity)] = drops.get("hook %s/%d" % (name, arity), 0) + 1
            continue
        if (name, arity) in OMIT.get(lib, {}):
            k = "omitted %s/%d (%s)" % (name, arity, OMIT[lib][(name, arity)])
            drops[k] = drops.get(k, 0) + 1
            continue
        kept.append((start, off)); keyed.append((name, arity))
    if mod is None:
        raise Refuse("%s: no :- module directive -- only a module library is vendored" % path)
    exp = set(exports)
    cside = [e for e in exports if e not in keyed and e not in OMIT.get(lib, {})]
    local = {k for k in keyed if k not in exp}
    renamed, chunks = {}, []
    for cs, ce in kept:
        raw = src[cs:ce]
        for f, t, ar, kd, nm in sorted((r for r in refs if cs <= r[0] < ce and (r[4], r[2]) in local), reverse=True):
            if kd == "c" and src[t:t + 1] != "(":
                raise Refuse("%s: the local predicate %s/%d is referenced in operator form at offset %d -- a renamed operator would not parse" % (path, nm, ar, f))
            new = "$%s_%s" % (lib, nm)
            raw = raw[:f - cs] + quote(new) + raw[t - cs:]
            renamed["%s/%d" % (nm, ar)] = new
        chunks.append(strip_comments(raw, path))
    text = "".join(chunks)
    lines = [l.rstrip() for l in text.split("\n")]
    text = "\n".join(l for l in lines if l.strip()) + "\n"
    out_toks = tokenize(text, path)
    local_names = {n for n, _ in local} - {n for n, _ in exports}
    as_data = sorted({atom_value(t) for t in out_toks if t[0] in ("atom", "qatom") and atom_value(t) in local_names})
    return {"lib": lib, "exports": [e for e in exports if e not in OMIT.get(lib, {}) and e not in cside], "cside": cside, "renames": renamed,
            "drops": drops, "as_data": as_data, "text": text, "atoms": {atom_value(t) for t in out_toks if t[0] in ("atom", "qatom")}}


def c_string_lines(text):
    out = []
    for line in text.split("\n")[:-1]:
        esc = line.replace("\\", "\\\\").replace('"', '\\"').replace("\t", "\\t")
        out.append('    "%s\\n"' % esc)
    return out


def generate(pdir):
    if not os.path.isdir(pdir):
        raise Refuse("no %s" % pdir)
    libs = [process(os.path.join(pdir, f)) for f in sorted(os.listdir(pdir)) if f.endswith(".pl")]
    if not libs:
        raise Refuse("no vendored library under %s" % pdir)
    for L in libs:
        L["names"] = sorted(M["lib"] for M in libs if M is not L and any(n in L["atoms"] for n, _ in M["exports"]))
    o = ["typedef struct { const char *nm; int ar; } pl_prelude_lib_pi_t;",
         "typedef struct { const char *lib; const pl_prelude_lib_pi_t *exports; const char *src; } pl_prelude_lib_t;"]
    for i, L in enumerate(libs):
        o.append("static const pl_prelude_lib_pi_t PL_PRELUDE_LIB_%d_EXPORTS[] = {" % i)
        for n, a in L["exports"]:
            o.append('    { "%s", %d },' % (n.replace("\\", "\\\\").replace('"', '\\"'), a))
        o.append("    { 0, 0 }")
        o.append("};")
        o.append("static const char PL_PRELUDE_LIB_%d_SRC[] =" % i)
        o.extend(c_string_lines(L["text"]))
        o[-1] += ";"
    o.append("static const pl_prelude_lib_t PL_PRELUDE_LIBS[] = {")
    for i, L in enumerate(libs):
        o.append('    { "%s", PL_PRELUDE_LIB_%d_EXPORTS, PL_PRELUDE_LIB_%d_SRC },' % (L["lib"], i, i))
    o.append("    { 0, 0, 0 }")
    o.append("};")
    return libs, "\n".join(o) + "\n"


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--dir", default=PRELUDE_DIR)
    ap.add_argument("--out", default=DEFAULT_OUT)
    ap.add_argument("--check", action="store_true")
    ap.add_argument("--show")
    a = ap.parse_args()
    try:
        libs, txt = generate(a.dir)
    except Refuse as e:
        print("REFUSED(2) util_gen_prolog_prelude_libs: %s" % e, file=sys.stderr)
        return 2
    if a.show:
        L = next((L for L in libs if L["lib"] == a.show), None)
        if not L:
            print("REFUSED(2): no vendored library %s" % a.show, file=sys.stderr)
            return 2
        print("exports: " + " ".join("%s/%d" % e for e in L["exports"]))
        print("renamed: " + " ".join("%s->%s" % kv for kv in sorted(L["renames"].items())))
        print("dropped: " + ", ".join("%s x%d" % kv for kv in sorted(L["drops"].items())))
        print("names exports of (informational -- the injector selects a library by the name/arity references it finds when it parses the libraries it selected): " + " ".join(L["names"]))
        print("C-side exports (no clause in the library; SCRIP answers them as builtins or they are existence errors): " + " ".join("%s/%d" % e for e in L["cside"]))
        print("left as data (a helper's name used as a term, never a goal -- review each): " + " ".join(L["as_data"]))
        sys.stdout.write(L["text"])
        return 0
    if a.check:
        cur = open(a.out).read() if os.path.exists(a.out) else ""
        if cur != txt:
            print("STALE: %s differs from what util_gen_prolog_prelude_libs.py generates from %s -- run it and commit both" % (os.path.relpath(a.out, ROOT), os.path.relpath(a.dir, ROOT)))
            return 1
        print("in sync: %d vendored libraries, %s" % (len(libs), " ".join(L["lib"] for L in libs)))
        return 0
    open(a.out, "w").write(txt)
    for L in libs:
        print("%-20s exports=%-3d renamed=%-3d C-side=%-2d as-data=%-2d dropped: %s" % (L["lib"], len(L["exports"]), len(L["renames"]), len(L["cside"]), len(L["as_data"]), ", ".join("%s x%d" % kv for kv in sorted(L["drops"].items()))))
    print("wrote %s" % os.path.relpath(a.out, ROOT))
    return 0


if __name__ == "__main__":
    sys.exit(main())
