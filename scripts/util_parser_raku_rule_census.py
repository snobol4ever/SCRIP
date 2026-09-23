#!/usr/bin/env python3
"""util_parser_raku_rule_census.py [--grammar NQP] [--map TSV] [--parser SC]

IS bootstrap/parser_raku.sc A COMPLETE MECHANICAL TRANSLATION OF THE RAKU GRAMMAR? (Lon 2026-09-23 15:2x, in-chat to the ceo:
hq_snocone to "create a complete mechanical translation of the Raku grammar into Snocone, i.e. parser_raku.sc file".)
THE GRAMMAR is the one the oracle runs: Rakudo 2026.05's DEFAULT grammar is the legacy src/Perl6/Grammar.nqp (RakuAST's
src/Raku/Grammar.nqp is gated off unless RAKUDO_RAKUAST is set, src/main.nqp line 18). Every token, rule and regex it
declares -- keyed <container>/<name>, the container being the full nesting of grammars and roles that declares it (a role
nested inside Perl6::QGrammar is Perl6::QGrammar/b1), read by indentation, :sym<...> variants included -- must appear
in THE MAP bootstrap/parser_raku.map.tsv (nqp_key TAB sc_name), and every sc_name the map names must be DEFINED in the
parser (a line assigning it, `name = ...`, or a `procedure name(`). Methods are code, not grammar: they are counted and
printed, never required. rc 0 GREEN (missing 0 and undefined 0 over a population > 0), 1 RED, 2 REFUSE (no grammar).
"""
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
SCRIP = os.path.dirname(HERE)
DECL = re.compile(r"^\s*(?:proto\s+)?(token|rule|regex)\s+([A-Za-z_][\w\-']*(?::sym<[^>]*>|:sym«[^»]*»|:[A-Za-z_]+<[^>]*>)?)")
METH = re.compile(r"^\s*(?:multi\s+)?method\s+([A-Za-z_][\w\-']*)")
CONT = re.compile(r"^(\s*)(?:my\s+)?(grammar|role)\s+([A-Za-z_][\w:]*)")


def opt(argv, name, default):
    return argv[argv.index(name) + 1] if name in argv else default


def main(argv):
    g = opt(argv, "--grammar", "/home/resources/rakudo-2026.05/src/Perl6/Grammar.nqp")
    m = opt(argv, "--map", os.path.join(SCRIP, "bootstrap", "parser_raku.map.tsv"))
    sc = opt(argv, "--parser", os.path.join(SCRIP, "bootstrap", "parser_raku.sc"))
    if not os.path.isfile(g):
        print("REFUSE(2): no grammar at " + g)
        return 2
    keys, meths, stack = [], 0, []
    for line in open(g, encoding="utf-8", errors="replace"):
        c = CONT.match(line)
        if c:
            ind = len(c.group(1).expandtabs())
            while stack and stack[-1][0] >= ind:
                stack.pop()
            stack.append((ind, c.group(3)))
            continue
        d = DECL.match(line)
        if d:
            ind = len(line) - len(line.lstrip(" \t"))
            while stack and stack[-1][0] >= ind:
                stack.pop()
            keys.append("/".join(n for _, n in stack) + "/" + d.group(2))
            continue
        if METH.match(line):
            meths += 1
    uniq = sorted(set(keys))
    mp = {}
    if os.path.isfile(m):
        for line in open(m, encoding="utf-8"):
            f = line.rstrip("\n").split("\t")
            if len(f) >= 2 and not line.startswith("#") and f[1].strip():
                mp[f[0]] = f[1].strip()
    text = open(sc, encoding="utf-8", errors="replace").read() if os.path.isfile(sc) else ""
    missing = [k for k in uniq if k not in mp]
    undefined = sorted({v for k, v in mp.items() if k in set(uniq) and not re.search(r"(?m)^\s*(?:procedure\s+" + re.escape(v) + r"\s*\(|" + re.escape(v) + r"\s*=)", text)})
    print("RAKU RULE CENSUS: grammar %s -- %d token/rule/regex declarations (%d distinct keys over %d containers), %d methods counted, not required" % (g, len(keys), len(uniq), len({k.rsplit('/', 1)[0] for k in uniq}), meths))
    print("map %s: %s; parser %s: %s" % (m, "%d rows" % len(mp) if os.path.isfile(m) else "ABSENT", sc, "%d bytes" % len(text) if text else "ABSENT"))
    print("RAKU RULE CENSUS: translated=%d of %d; missing from the map=%d; mapped but not defined in the parser=%d" % (len(uniq) - len(missing), len(uniq), len(missing), len(undefined)))
    for k in missing[:12]:
        print("  MISSING " + k)
    for v in undefined[:12]:
        print("  UNDEFINED " + v)
    if not uniq:
        print("REFUSE(2): the grammar declared nothing -- the reader is wrong, not the translation")
        return 2
    if missing or undefined:
        print("RED: parser_raku.sc is not a complete translation of the oracle's grammar")
        return 1
    print("GREEN: every rule of the oracle's grammar is mapped and defined in parser_raku.sc")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
