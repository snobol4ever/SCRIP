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

⛔⛔ CUT-OVER LANGUAGES RE-VERIFY IN A SCRATCH TREE AND DIFF -- NEVER BY DELETING IN PLACE.
(Law 2 AMENDED, ceo 2026-08-30, on hq_P + seat14 reproducing it independently; both reverted, origin intact.)
Law 2 says a GUARD CHANGE REQUIRES A CLEAN REBUILD. That is correct for a language with live sources -- it
cured icon's 14 false failures the same night -- and CATASTROPHIC for a CUT-OVER one, because "clean" means
deleting ALL.* and on a cut-over language THE MASTER IS THE ONLY COPY. Measured on snobol4: rm ALL.* then
rebuild yields TWO entries where 1576 stood, because only 2 loose pairs remain absorbable. The build reports
success. hq_P's phrasing is the one to remember: CLOBBER-BY-GUARD-REBUILD IS CLOBBER WEARING THE LAW AS COVER.

  HOW TO RE-VERIFY A GUARD CHANGE ON A CUT-OVER LANGUAGE:
    1. cp -r corpus/tests/<lang> into a scratch tree; point S4E_HOME at its parent.
    2. Rebuild THERE. The real master is never opened for writing.
    3. DIFF scratch against live -- entry sets by name, then the five-file artifact (Law 2).
    4. Bring differences across SURGICALLY, by the language owner. Never adopt a scratch tree wholesale.
  ⭐ The shrink refusal below is the automated floor under this procedure, not a replacement for it: it stops
  the catastrophic case (0 < pairs << base), but a rebuild that shrinks the master by a NON-dramatic amount
  still passes the ratio and still loses entries. The scratch diff is what sees those.
  ⛔ A language is CUT OVER when its sources have been retired and the master is the source. The tell is this
  builder REFUSING with the collapse message -- if you see it, you are on a cut-over tree and step 1 applies.
  Related shapes that write into the master and therefore need the same care: --split-write (emits a derived
  pair, adopts nothing) and lib_master_extract.sh (materializes entries OUT, never in).
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
# -- icon attributes: Icon's vocabulary is GENERATORS and STRING SCANNING, so the columns are built around
# result sequences (every/suspend/alternation/element-generation) and the scanning environment (?/tab/move/&pos),
# not around pattern primitives. Deliberately NOT the SNOBOL4 table: `ANY`/`TAB`/`POS` exist in both languages
# with different meanings, and borrowing would make a cross-language profile read as agreement where there is none.
ICON_COLS = [
    ("write", LOW("write")), ("writes", LOW("writes")), ("read", LOW("read")), ("reads", LOW("reads")),
    ("find", LOW("find")), ("match", LOW("match")), ("upto", LOW("upto")), ("many", LOW("many")),
    ("any", LOW("any")), ("bal", LOW("bal")), ("tab", LOW("tab")), ("move", LOW("move")),
    ("pos", LOW("pos")), ("stop", LOW("stop")), ("image", LOW("image")), ("type", LOW("type")),
    ("sort", LOW("sort")), ("put", LOW("put")), ("push", LOW("push")), ("pop", LOW("pop")),
    ("get", LOW("get")), ("insert", LOW("insert")), ("delete", LOW("delete")), ("member", LOW("member")),
    ("list", LOW("list")), ("table", LOW("table")), ("set", LOW("set")), ("repl", LOW("repl")),
    ("map", LOW("map")), ("trim", LOW("trim")), ("seq", LOW("seq")), ("integer", LOW("integer")),
    # control and declaration keywords (bare words, Icon is lowercase and case-sensitive)
    ("every", BARE("every")), ("suspend", BARE("suspend")), ("fail", BARE("fail")), ("return", BARE("return")),
    ("while", BARE("while")), ("until", BARE("until")), ("repeat", BARE("repeat")), ("case", BARE("case")),
    ("next", BARE("next")), ("break", BARE("break")), ("create", BARE("create")), ("record", BARE("record")),
    ("procedure", BARE("procedure")), ("local", BARE("local")), ("static", BARE("static")), ("global", BARE("global")),
    # constructs by syntax shape (lexical approximations)
    ("scan", lambda t: 1 if re.search(r"\?\s*[A-Za-z_{(\"]", t) else 0),                  # expr ? expr -- scanning
    ("alternation", lambda t: 1 if re.search(r"[^|]\|[^|]", t) else 0),                    # e1 | e2 -- result sequence
    ("element_gen", lambda t: 1 if re.search(r"(^|[\s(,=])!\s*[A-Za-z_(\"]", t, re.M) else 0),  # !x
    ("limitation", lambda t: 1 if re.search(r"\\\s*[0-9A-Za-z_(]", t) else 0),               # e \ n
    ("to_by", lambda t: 1 if re.search(r"(?<![A-Za-z0-9_])to(?![A-Za-z0-9_])", t) else 0),  # i to j [by k]
    ("assign", lambda t: 1 if ":=" in t else 0),
    ("swap", lambda t: 1 if ":=:" in t else 0),
    ("aug_assign", lambda t: 1 if re.search(r"[-+*/|&]:=", t) else 0),
    ("coexpr_activate", lambda t: 1 if "@" in t else 0),
    ("keyword_ref", lambda t: 1 if re.search(r"&[a-z]", t) else 0),
    # named combinations
    ("every_with_suspend", lambda t: 1 if (re.search(r"(?<![A-Za-z0-9_])every(?![A-Za-z0-9_])", t) and re.search(r"(?<![A-Za-z0-9_])suspend(?![A-Za-z0-9_])", t)) else 0),
    ("scan_with_tab", lambda t: 1 if (re.search(r"\?\s*[A-Za-z_{(\"]", t) and re.search(r"(?<![A-Za-z0-9_])tab\s*\(", t)) else 0),
    ("gen_with_alternation", lambda t: 1 if (re.search(r"(?<![A-Za-z0-9_])(every|suspend)(?![A-Za-z0-9_])", t) and re.search(r"[^|]\|[^|]", t)) else 0),
]
ICON_NAME_FEATURES = [
    ("procedure", "procedure"), ("record", "record"), ("create", "coexpr"), ("every", "every"),
    ("suspend", "suspend"), ("scan", "scan"), ("element_gen", "elemgen"), ("alternation", "alt"),
    ("limitation", "limit"), ("to_by", "to"), ("swap", "swap"), ("coexpr_activate", "activate"),
    ("case", "case"), ("while", "while"), ("until", "until"), ("repeat", "repeat"), ("fail", "fail"),
    ("tab", "tab"), ("move", "move"), ("upto", "upto"), ("many", "many"), ("find", "find"), ("match", "match"),
    ("bal", "bal"), ("sort", "sort"), ("table", "table"), ("list", "list"), ("set", "set"), ("map", "map"),
    ("repl", "repl"), ("seq", "seq"), ("write", "write"), ("writes", "writes"), ("read", "read"),
    ("keyword_ref", "keyword"), ("aug_assign", "augassign"), ("global", "global"), ("static", "static"),
]
# -- pascal attributes: ISO Pascal is CASE-INSENSITIVE, so every matcher here folds case (the corpus happens to
# be lowercase throughout, but a table that only matched lowercase would silently under-report an upper-case
# dialect file and read as "this program uses no constructs" rather than as a miss).
PAS = lambda w: (lambda t: 1 if re.search(r"(?<![A-Za-z0-9_])%s(?![A-Za-z0-9_])" % w, t, re.I) else 0)
PASCAL_COLS = [
    ("program", PAS("program")), ("procedure", PAS("procedure")), ("function", PAS("function")),
    ("var", PAS("var")), ("const", PAS("const")), ("type", PAS("type")), ("record", PAS("record")),
    ("array", PAS("array")), ("set", PAS("set")), ("file", PAS("file")), ("packed", PAS("packed")),
    ("label", PAS("label")), ("goto", PAS("goto")), ("with", PAS("with")), ("case", PAS("case")),
    ("while", PAS("while")), ("repeat", PAS("repeat")), ("for", PAS("for")), ("downto", PAS("downto")),
    ("forward", PAS("forward")), ("nil", PAS("nil")), ("div", PAS("div")), ("mod", PAS("mod")),
    ("in", PAS("in")), ("writeln", PAS("writeln")), ("write", PAS("write")), ("readln", PAS("readln")),
    ("read", PAS("read")), ("new", PAS("new")), ("dispose", PAS("dispose")), ("ord", PAS("ord")),
    ("chr", PAS("chr")), ("succ", PAS("succ")), ("pred", PAS("pred")), ("abs", PAS("abs")),
    ("sqr", PAS("sqr")), ("sqrt", PAS("sqrt")), ("trunc", PAS("trunc")), ("round", PAS("round")),
    ("eof", PAS("eof")), ("eoln", PAS("eoln")), ("odd", PAS("odd")),
    # constructs by syntax shape
    ("assign", lambda t: 1 if ":=" in t else 0),
    ("pointer", lambda t: 1 if re.search(r"\^\s*[A-Za-z_;)\]]", t) else 0),
    ("set_literal", lambda t: 1 if re.search(r"\[[^\]]*\.\.[^\]]*\]", t) else 0),
    ("subrange", lambda t: 1 if re.search(r"[0-9A-Za-z_'\)]\s*\.\.\s*[0-9A-Za-z_']", t) else 0),
    ("nested_proc", lambda t: 1 if len(re.findall(r"(?<![A-Za-z0-9_])(procedure|function)(?![A-Za-z0-9_])", t, re.I)) > 1 else 0),
    # named combinations
    ("for_with_array", lambda t: 1 if (re.search(r"(?<![A-Za-z0-9_])for(?![A-Za-z0-9_])", t, re.I) and re.search(r"(?<![A-Za-z0-9_])array(?![A-Za-z0-9_])", t, re.I)) else 0),
    ("with_record", lambda t: 1 if (re.search(r"(?<![A-Za-z0-9_])with(?![A-Za-z0-9_])", t, re.I) and re.search(r"(?<![A-Za-z0-9_])record(?![A-Za-z0-9_])", t, re.I)) else 0),
    ("pointer_with_new", lambda t: 1 if (re.search(r"\^", t) and re.search(r"(?<![A-Za-z0-9_])new\s*\(", t, re.I)) else 0),
]
PASCAL_NAME_FEATURES = [
    ("program", "program"), ("function", "function"), ("procedure", "procedure"), ("record", "record"),
    ("array", "array"), ("set", "set"), ("file", "file"), ("packed", "packed"), ("pointer", "pointer"),
    ("with", "with"), ("case", "case"), ("while", "while"), ("repeat", "repeat"), ("for", "for"),
    ("downto", "downto"), ("goto", "goto"), ("label", "label"), ("forward", "forward"),
    ("nested_proc", "nested"), ("set_literal", "setlit"), ("subrange", "subrange"), ("div", "div"),
    ("mod", "mod"), ("writeln", "writeln"), ("readln", "readln"), ("new", "new"), ("dispose", "dispose"),
    ("sqrt", "sqrt"), ("trunc", "trunc"), ("round", "round"), ("eof", "eof"), ("odd", "odd"),
]
LANG_TABLES = {
    "snobol4": (COLS, NAME_FEATURES),   # unchanged -- byte-identity control arm
    "snocone": (COLS, NAME_FEATURES),   # SNOBOL-family: same pattern vocabulary, deliberately shared
    "rebus":   (COLS, NAME_FEATURES),   # SNOBOL-family: same pattern vocabulary, deliberately shared
    "prolog":  (PROLOG_COLS, PROLOG_NAME_FEATURES),
    "raku":    (RAKU_COLS, RAKU_NAME_FEATURES),
    "icon":    (ICON_COLS, ICON_NAME_FEATURES),
    "pascal":  (PASCAL_COLS, PASCAL_NAME_FEATURES),
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



# ⛔ dirs whose pairs are COMPANION-DEPENDENT and must NOT be absorbed: an entry runs ALONE in a scratch dir, so a
# test needing sibling files beside it would grade a DIFFERENT program. linker/ is the multi-file IMPORT/EXPORT
# class (its companions are themselves source files, invisible to the extension rule); probe_loose/ is the
# characterized un-convertible residue by definition; rtx_func_11 is the -INCLUDE class whose include resolution
# provably differs between its own dir and master/ (absorbed copy graded m3-red while the source grades green --
# measured v5); all stay loose until their owning rows settle representation.
EXCLUDE_DIRS = {"linker", "probe_loose", "rtx_func_11"}
COMPANION_EXTS = {".inc", ".dat", ".input", ".in", ".json"}


# ⭐⭐ THE ONE ATTRIBUTE EXTRACTOR, CALLABLE OUTSIDE A BUILD (hq_B 2026-08-29, for the construct check).
# The builder computed flags inline, so anything else wanting a construct profile had to RE-IMPLEMENT the
# predicates. ⛔ That is the defect this exposure exists to prevent: the construct check must ask "does any
# master entry still cover what this deleted file covered", and it can only recover the deleted file's text
# from git history. If the historical profile and the master's profile are computed by TWO implementations,
# any disagreement between them is UNATTRIBUTABLE -- you cannot tell a real coverage loss from a drift
# between two copies of the same rule. One function, two callers, or the comparison is not worth making.
# ⚠️ A profile is a LEXICAL approximation. It can show a construct is ABSENT from every master entry; it
# cannot show that the behaviour is adequately tested. Callers must treat it as one-directional.
# ⛔⭐⭐ `modes` IS A DECLARED FIELD, NEVER A DERIVED ONE (hq_C's FORMAT RULING, TRIO 2026-08-29, on hq_B's
# four-for-four evidence). The grading mode is a property of THE RUNNER and is recorded nowhere in the corpus,
# so the builder cannot know it -- it can only carry a declaration someone wrote down.
# ⛔ WHY NOT DERIVE IT FROM THE FAMILY NAME, when `family.startswith("parser") -> ast` is exactly right on all
# four languages measured (prolog 134/134, raku 83/83, snocone 67/67, rebus 15/15): BECAUSE THAT EXACTNESS IS
# THE ARGUMENT AGAINST IT. A heuristic right on every case you have is maximally tempting and gives NO SIGNAL
# when it starts being wrong -- RULES.md § A CORRECT PROCEDURE WITH A FALSE EXPLANATION, where every
# successful use appears to confirm the rule and the rule is never once under test. A name is a proxy that is
# right today by coincidence of naming discipline.
# ⛔ DEFAULT IS `UNKNOWN` AND IT MUST BE LOUD. An unknown-mode entry may NOT be quietly graded in a default
# mode -- that reproduces the original defect with a schema field on top, which is WORSE because the field
# now looks like the question was answered. Unknown is UNPROVEN at grading time, never pass and never fail.
# DECLARATION FILE: tests/<lang>/config/MODES.tsv (or tests/<lang>/MODES.tsv until the flat/config end state
# lands), lines of `family<TAB>modes`, `#` comments ignored. One human-maintained file per language, in the
# folder ceo's flat layout reserves for exactly this kind of companion.
def read_modes_decl(root):
    """{family: modes} from the per-language declaration. Absent file or absent family -> UNKNOWN."""
    decl = {}
    for cand in (os.path.join(root, "config", "MODES.tsv"), os.path.join(root, "MODES.tsv")):
        if not os.path.isfile(cand):
            continue
        with open(cand) as fh:
            for line in fh:
                line = line.strip()
                if not line or line.startswith("#"):
                    continue
                parts = line.split("\t")
                if len(parts) >= 2 and parts[1].strip():
                    decl[parts[0].strip()] = parts[1].strip()
        break
    return decl


def attrs_for_text(text, lang="snobol4"):
    """Construct profile for arbitrary source text, in `lang`'s attribute vocabulary. -> {column: 0|1}"""
    if lang not in LANG_TABLES:
        raise ValueError("no attribute table for language %r -- add it deliberately rather than "
                         "borrowing another dialect's columns" % lang)
    cols, _ = LANG_TABLES[lang]
    return {c: fn(text) for c, fn in cols}


def discover_pairs(ROOT, OUTDIR, EXT):
    pairs, excluded = [], []
    seen_excl_prefix = set()
    for dirpath, dirnames, filenames in os.walk(ROOT):
        rel_dir = os.path.relpath(dirpath, ROOT)
        top = rel_dir.split(os.sep)[0] if rel_dir != "." else ""
        if top in EXCLUDE_DIRS:
            if any(f.endswith(EXT) for f in filenames):
                excluded.append((rel_dir.replace(os.sep, "_"), "companion-dependent dir class (%s) -- stays as files" % top))
            dirnames[:] = []
            continue
        dir_companions = sorted({os.path.splitext(f)[1] for f in filenames if os.path.splitext(f)[1] in COMPANION_EXTS})
        for fn in sorted(filenames):
            if not fn.endswith(EXT):
                continue
            if fn.startswith("ALL"):
                continue  # the master pair itself is never a source
            # ⭐ (Lon 2026-08-29, "take all those *.sno"): the former excluded classes (probe_loose_*, linker_*,
            # rtx_func_11_*) absorb like everything else -- auto-XFAIL by source verdict keeps documented reds
            # from inflating FAIL, and runtime companions resolve from the config/ subfolder both at source
            # grading and from the master (the harness searches <dir>/config since this commit).
            sno = os.path.join(dirpath, fn)
            ref = sno[:-len(EXT)] + ".ref"
            if not os.path.isfile(ref):
                continue  # pairless loose witnesses are not board members and are untouched
            rel = os.path.relpath(sno, ROOT)
            fam = rel[:-len(EXT)].replace(os.sep, "_")
            pairs.append((fam, sno, ref, dir_companions))
    return pairs, excluded


# ============================================================ master-to-master: kind split ===
def split_write(OUTDIR, EXT, lang, _CO, _CC, do_write):
    """⭐ --split-write: emit Lon's END STATE -- a ONE-LINERS file and a MULTI-LINERS file side by side.

    Lon 2026-08-29/30 (routed by ceo): per language, one-liners and multi-liners live in SEPARATE files in
    one flat dir. ceo ruled (2026-08-30) that the write half belongs HERE rather than in the verify-first
    splitter, because this file already owns master writing; reading THE master as input is a
    MASTER-TO-MASTER verb, distinct from the source rebuild whose zero-pairs refusal stays untouched.

    ⛔⭐ THE SPLIT REMOVES A CORRECTNESS INVARIANT THAT IS CURRENTLY HELD BY CONVENTION, and this is the
    strongest argument for the format rather than a side effect of it. write_suite()'s own docstring
    records that format-(B) blocks MUST be emitted LAST: a banner block ends only at the next banner or
    EOF, so any one-line entry written after a block is SILENTLY SWALLOWED into that block's body --
    measured on probe/eval, 21 entries written and 22 read back, one duplicated. In separate files a
    one-liner CANNOT be swallowed, because the one-liners file contains no blocks at all. The ordering
    rule stops being something a writer must remember and becomes a property of the layout.

    ⛔ IT DOES NOT ADOPT THE SPLIT. ALL.* is left exactly as it is; nothing re-points; no source is
    deleted. ceo adopted the ordering law as stated: the split files land in one commit, ADOPTION is its
    own commit gated on the per-entry named-set equality proof, never both together -- otherwise a moved
    floor cannot be attributed to a converter defect or a real regression.

    Verification before writing, and all three must hold (redundant on purpose -- a duplicate that
    replaces a lost entry passes a count and fails a set):
      1. PARTITION  every entry lands in exactly one side; union == input; intersection empty.
      2. COUNT      len(one) + len(multi) == len(input), asserted separately from (1).
      3. ROUNDTRIP  each emitted pair is re-read and must return its own entries, by name and by body.
    """
    src = os.path.join(OUTDIR, "ALL" + EXT)
    ref = os.path.join(OUTDIR, "ALL.ref")
    if not (os.path.isfile(src) and os.path.isfile(ref)):
        sys.stderr.write("REFUSED: --split-write needs an existing master pair at %s / ALL.ref -- this is a\n"
                         "  MASTER-TO-MASTER verb, not a rebuild. Build the master first.\n" % src)
        raise SystemExit(2)
    if lang == "snobol4":
        entries = h.read_suite(src, ref, in_path=h.sidecar_in_path(src), x_path=h.sidecar_xfail_path(src))
    else:
        entries = h.read_block_suite(src, ref, h.banner_re_for(_CO, _CC),
                                     in_path=h.sidecar_in_path(src), x_path=h.sidecar_xfail_path(src))
    if not entries:
        sys.stderr.write("REFUSED: master at %s read back ZERO entries -- an empty split is not a split you\n"
                         "  can verify, and it would silently retire the whole floor.\n" % src)
        raise SystemExit(2)
    one   = [e for e in entries if e.kind == "line"]
    multi = [e for e in entries if e.kind != "line"]
    n_one, n_multi, n_all = len(one), len(multi), len(entries)
    # (2) count, asserted independently of (1)
    if n_one + n_multi != n_all:
        sys.stderr.write("FAILED: count disagreement %d + %d != %d\n" % (n_one, n_multi, n_all))
        raise SystemExit(1)
    # (1) partition by NAME identity, not arithmetic
    s_one, s_multi, s_all = {e.name for e in one}, {e.name for e in multi}, {e.name for e in entries}
    if (s_one | s_multi) != s_all or (s_one & s_multi):
        sys.stderr.write("FAILED: partition is not exact (union/intersection check)\n")
        raise SystemExit(1)
    print("master  : %s (%d entries)" % (src, n_all))
    print("one-liners  (kind=line ) : %d" % n_one)
    print("multi-liners(kind=block) : %d" % n_multi)
    print("\u2705 partition exact, counts agree")
    if not do_write:
        print("\nVERIFY-ONLY. Nothing written. Add --write to emit the two pairs.")
        return 0
    out = []
    for tag, subset in (("ONE", one), ("MULTI", multi)):
        o_src = os.path.join(OUTDIR, tag + EXT)
        o_ref = os.path.join(OUTDIR, tag + ".ref")
        if lang == "snobol4":
            # ⛔⭐ THE SIDECARS ARE NOT OPTIONAL, AND OMITTING THEM COST NINE ENTRIES ON THE FIRST RUN.
            # write_suite() emits only .sno/.ref; stdin and xfail live in ALL.in / ALL.xfail and the
            # master build writes them SEPARATELY (see the snobol4 branch of the main write). Dropping
            # them here silently stripped stdin from every entry that had it -- MEASURED: the master
            # carries exactly 9 stdin entries, ALL of them kind=block, and MULTI graded 666/10 against
            # the combined master's contribution of 675/1. Nine entries in, nine entries lost.
            h.write_suite(subset, o_src, o_ref)
            # ⛔ sidecar_in_path()/sidecar_xfail_path() are DISCOVERY helpers -- they return the path
            # only if the file ALREADY EXISTS, else None. They answer "where is the sidecar", and I
            # used them as "where should the sidecar go": on a fresh output they return None and the
            # write crashed with `stat: path should be string ... not NoneType`. The master build
            # constructs these names directly (out_in = OUTDIR/ALL.in); do the same here.
            _oi = os.path.join(OUTDIR, tag + ".in")
            _ox = os.path.join(OUTDIR, tag + ".xfail")
            if not h.write_stdin_sidecar(subset, _oi, "*", "") and os.path.exists(_oi):
                os.remove(_oi)
            if not h.write_xfail_sidecar(subset, _ox, "*", "") and os.path.exists(_ox):
                os.remove(_ox)
        else:
            h.write_block_suite(subset, o_src, o_ref, _CO, _CC,
                                out_in=h.sidecar_in_path(o_src), out_x=h.sidecar_xfail_path(o_src))
        out.append((tag, o_src, o_ref, subset))
    # (3) ROUNDTRIP -- re-read what was just written and require it back, by name AND by body.
    for tag, o_src, o_ref, subset in out:
        if lang == "snobol4":
            back = h.read_suite(o_src, o_ref, in_path=h.sidecar_in_path(o_src), x_path=h.sidecar_xfail_path(o_src))
        else:
            back = h.read_block_suite(o_src, o_ref, h.banner_re_for(_CO, _CC),
                                      in_path=h.sidecar_in_path(o_src), x_path=h.sidecar_xfail_path(o_src))
        if len(back) != len(subset) or {e.name for e in back} != {e.name for e in subset}:
            sys.stderr.write("FAILED: %s did not round-trip -- wrote %d, read %d\n" % (tag, len(subset), len(back)))
            raise SystemExit(1)
        by = {e.name: e for e in subset}
        # ⛔⭐ COMPARE stdin AND xfail, NOT JUST THE BODY. The first version of this check compared
        # sno_lines alone and PASSED while the writer above was silently dropping stdin -- the check
        # tested a narrower property than the claim it was making, which is the exact defect class this
        # tool exists to guard against, committed inside the guard. A round trip that does not carry
        # every field the grader reads is not a round trip.
        for e in back:
            o = by[e.name]
            if list(e.sno_lines) != list(o.sno_lines):
                sys.stderr.write("FAILED: %s entry %s changed BODY across the round trip\n" % (tag, e.name))
                raise SystemExit(1)
            if (e.stdin or None) != (o.stdin or None):
                sys.stderr.write("FAILED: %s entry %s LOST OR CHANGED STDIN across the round trip -- the\n"
                                 "  .in sidecar was not carried. This silently turns a passing entry red.\n" % (tag, e.name))
                raise SystemExit(1)
            if bool(e.xfail) != bool(o.xfail):
                sys.stderr.write("FAILED: %s entry %s changed XFAIL across the round trip\n" % (tag, e.name))
                raise SystemExit(1)
        print("\u2705 %-5s round-trips: %d entries, bodies byte-identical -> %s" % (tag, len(back), o_src))
    print("\n\u26d4 WRITTEN, NOT ADOPTED. ALL.* is untouched and every consumer still reads it.")
    print("   Adoption is a SEPARATE commit gated on the per-entry named-set equality proof")
    print("   (row snobol4-floor-cutover-to-the-one-flat-suite-board-equality-first).")
    return 0


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
    OUTDIR = ROOT   # ⭐ ZERO SUBFOLDERS (Lon 2026-08-29): the master lives FLAT beside the residue -- ALL.* side by side, no master/ dir
    if not os.path.isdir(ROOT):
        sys.stderr.write("REFUSED: no corpus tree at %s -- nothing to build from.\n" % ROOT)
        raise SystemExit(2)
    global COLS, NAME_FEATURES
    COLS, NAME_FEATURES = LANG_TABLES[lang]
    os.makedirs(OUTDIR, exist_ok=True)
    _modes_decl = read_modes_decl(ROOT)   # declared, never derived -- absent family => UNKNOWN
    delete_absorbed = "--delete-absorbed" in sys.argv[1:]
    if "--split-write" in sys.argv[1:]:
        raise SystemExit(split_write(OUTDIR, EXT, lang, _CO, _CC, "--write" in sys.argv[1:]))
    included, all_entries, per_family = [], [], {}
    absorbed_files = []    # (fam, sno, ref, mode) for post-verification deletion
    companion_copies = {}  # basename -> source path; written into OUTDIR after the merge succeeds
    pairs, excluded = discover_pairs(ROOT, OUTDIR, EXT)
    # ⛔⭐ MERGE, NEVER REPLACE (measured the hard way: an incremental run on a post-retirement tree rebuilt the
    # master from ONLY the new pairs and overwrote 1495 entries with 98 -- caught and restored from the index).
    # If a master already exists, its entries are the BASE: loaded with their names KEPT (names are stable
    # identifiers), origins re-attached from the CSV, and new absorptions appended with counters seeded PAST
    # the existing names so nothing collides. No new pairs + an existing master = "current", clean exit.
    master_sno_path = os.path.join(OUTDIR, "ALL" + EXT)
    master_csv_path = os.path.join(OUTDIR, "ALL.csv")
    base_entries = []
    if os.path.isfile(master_sno_path) and os.path.isfile(os.path.join(OUTDIR, "ALL.ref")):
        if lang == "snobol4":
            base_entries = h.read_suite(master_sno_path, os.path.join(OUTDIR, "ALL.ref"),
                                        in_path=h.sidecar_in_path(master_sno_path), x_path=h.sidecar_xfail_path(master_sno_path))
        else:
            base_entries = h.read_block_suite(master_sno_path, os.path.join(OUTDIR, "ALL.ref"), h.banner_re_for(_CO, _CC),
                                              in_path=h.sidecar_in_path(master_sno_path), x_path=h.sidecar_xfail_path(master_sno_path))
        _csv_origin = {}
        if os.path.isfile(master_csv_path):
            for _row in csv.DictReader(open(master_csv_path)):
                _csv_origin[_row["entry"]] = _row.get("origin", "")
        for e in base_entries:
            e.origin = _csv_origin.get(e.name) or ("master__%s" % e.name)
            e.src_mode = "base"
    # THE DANGEROUS CONDITION IS pairs << base, NOT pairs == 0 (hq_P 2026-08-30, reproduced by seat14;
    # both reverted). Measured on snobol4, which is CUT OVER -- sources retired, the master IS the source:
    # 21 loose pairs remain, 19 correctly refused, leaving TWO absorbable. This guard fired only at ZERO, so
    # 2 walked past and the rebuild produced a master of 2 entries where 1576 had been -- the floor's own
    # input, destroyed by a build that reported success.
    # AND IT WAS hq_B's LAW THAT AIMED PEOPLE AT IT: "a guard change requires a CLEAN rebuild" is right for a
    # language with live sources and catastrophic for a cut-over one, because "clean" means deleting ALL.*
    # and THE SAFETY NET IS THE MASTER ITSELF. hq_P: clobber-by-guard-rebuild is clobber wearing the law as
    # cover.
    # Same shape as the dedupe/clobber pair: ONE AXIS, a correct point in the middle, BOTH ENDS SHIPPING
    # SILENTLY -- too many entries (accumulation) and too few (collapse) each read as an ordinary success.
    # A predicate that tests only one end of an axis is half a guard.
    # ⭐⭐ ORDER MATTERS AND IT IS THE WHOLE FIX (hq_P 2026-08-30, on seat06's measurement): ZERO PAIRS OVER A
    # HEALTHY MASTER IS THE *SUCCESS* STATE OF CONSOLIDATION, not a collapse. Every lane reaches it the moment
    # its last loose pair is absorbed and deleted -- i.e. exactly when the all-hands work succeeds. With the
    # collapse test first, `0 * 4 < 97` refused raku, prolog and eventually every language, and the message
    # told them not to delete ALL.* -- correct advice for a disaster, addressed to a success.
    # ⛔ The two states ARE distinguishable and the discriminator is not a ratio: with ZERO pairs the builder
    # has nothing to write FROM, so it cannot produce a smaller master -- `not pairs` exits without writing a
    # byte. The dangerous case is strictly `0 < pairs << base_entries`: something to write, far too little of
    # it. So the benign branch is checked FIRST and the collapse test now only ever sees pairs > 0.
    if not pairs:
        if base_entries:
            print("MASTER CURRENT: %d entries, zero new absorbable pairs under %s -- nothing to do." % (len(base_entries), ROOT), file=sys.stderr)
            raise SystemExit(0)
        sys.stderr.write("REFUSED: zero absorbable pairs under %s and no master present -- nothing to build from.\n" % ROOT)
        raise SystemExit(2)
    # ⭐⭐ THE COLLAPSE TEST ITSELF MOVED DOWN, past the pair->entries materialization loop -- see the
    # `len(all_entries) * 4 < len(base_entries)` check just before the base merge. It used to stand here
    # and compare PAIRS to ENTRIES, which are different units (hq_B guard, seat06 found the false
    # positive on raku, hq_P swept the rest and named the cause).
    # ⛔⛔ DEDUPE BY ORIGIN AT MERGE TIME (hq_B, measured on rebus: every rebuild DOUBLED the master, 174->348,
    # every origin exactly twice -- the exact opposite failure of the clobber merge-mode was added to fix, and the
    # more dangerous one: a bigger board reads as progress). An absorbed pair whose origins ALREADY exist in the
    # base is the SAME entry rebuilt (its source should have been deleted after absorption): skip it LOUDLY, never
    # append a twin. Origin is the ready-made key -- unique per source entry by construction.
    base_origins = {e.origin for e in base_entries}
    for fam, sno, ref, dir_companions in pairs:
        entries = None
        mode = "suite"
        if lang == "snobol4":
            try:
                entries = h.read_suite(sno, ref, in_path=h.sidecar_in_path(sno), x_path=h.sidecar_xfail_path(sno))
                # ⛔ THE seqN DISCRIMINATOR: read_suite names a TAGLESS line `seqN` -- a PLAIN PROGRAM whose line
                # count happens to match its ref's would silently absorb as N bogus entries. Any seqN name means
                # this pair is NOT a suite; fall through to plain-program absorption instead.
                if any(re.fullmatch(r"seq\d+", e.name) for e in entries):
                    entries = None
            except Exception:
                entries = None
            if entries is None:  # plain program: ONE format-B block entry, body and ref VERBATIM
                mode = "plain"
                if os.path.isfile(sno[:-len(EXT)] + ".input") or os.path.isfile(os.path.join(os.path.dirname(sno), "config", os.path.basename(sno)[:-len(EXT)] + ".input")):
                    excluded.append((fam, "stdin sidecar (.input) -- stays as files until the stdin-sections format extension lands (hq_C row)"))
                    continue
                sno_text = open(sno).read().splitlines()
                ref_text = open(ref).read().splitlines()
                if not sno_text:
                    excluded.append((fam, "empty source"))
                    continue
                if any(k in ln for ln in sno_text for k in ("&FILE", "&LASTFILE", "&LASTLINE", "&LASTNO")):
                    excluded.append((fam, "source-identity-sensitive keyword (&FILE/&LASTFILE/...) -- output depends on the source FILENAME, which absorption renames; stays as files (measured: k09/k11/k30/k32 went red in the master while green loose)"))
                    continue
                if os.path.basename(sno).startswith("probe_loose_fuzz_"):
                    excluded.append((fam, "fuzz nondeterministic-crash class -- a captured ref is one sample of a distribution; util_fuzz_witness_stability.sh owns these, stays as files"))
                    continue
                if any("../" in ln for ln in sno_text):
                    excluded.append((fam, "scratch-escaping relative reference (../) -- semantics cannot survive isolated-entry grading; stays as files"))
                    continue
                if any(h._is_entry_start(ln) for ln in sno_text):
                    excluded.append((fam, "plain body carries suite-format markers (banner or ';* tag' line) -- a block would split on re-read; quarantined class, stays as files"))
                    continue
                # auto-XFAIL by source verdict -- convert_one's own law applied to plain absorption: a documented-red
                # witness never inflates the master's FAIL count, and XPASS polices the marker the day the bug is cured.
                import tempfile as _tf
                _paths = h.resolve_paths()
                _tmp = _tf.mkdtemp(prefix="mstr_")
                try:
                    from pathlib import Path as _P
                    _v = h.run_all_modes(_paths, _P(sno), open(ref).read(), _P(_tmp), ["m3", "m4"])
                    _green = all(x.kind == "PASS" for x in _v.values())
                finally:
                    import shutil as _sh
                    _sh.rmtree(_tmp, ignore_errors=True)
                entries = [h.Entry("block", 1, os.path.basename(sno)[:-len(EXT)], sno_text, ref_text, xfail=not _green)]
        else:
            # ⛔⭐ READ EACH SOURCE PAIR WITH ITS OWN DIALECT READER (hq_B, measured: read_suite on a .pl pair returns
            # the whole file as one entry whose body still contains %--- banners -- content that looks like structure).
            # ⛔⭐ THE seqN DISCRIMINATOR ABOVE EXISTS FOR EXACTLY THIS CASE AND WAS SNOBOL4-ONLY, SO THE DIALECT
            # PATH HIT THE VERY BUG ITS COMMENT DESCRIBES. The old fallback handed a BANNERLESS pair to
            # `read_suite` -- SNOBOL4's LINE reader -- which split a 3-line Icon parser fixture into 3 one-line
            # entries and graded each against one line of its AST ref. Measured before the fix: 203 of 472 icon
            # entries carried n_lines=1 with bodies like `end`, and the master scored ast_pass=0/472.
            # ⭐ Decide by whether the file HAS a banner, never by whether a reader happened to raise: SNOBOL4 is
            # line-oriented so a bannerless .sno may legitimately be many one-line entries, but in every other
            # dialect a program is inherently multi-line and a bannerless pair is ONE program.
            _bre = h.banner_re_for(_CO, _CC)
            try:
                _slines = open(sno, encoding="utf-8", errors="replace").read().splitlines()
                _rlines = open(ref, encoding="utf-8", errors="replace").read().splitlines()
            except Exception as e:
                excluded.append((fam, "dialect read refused: %s" % str(e)[:140]))
                continue
            if not any(_bre.match(_l) for _l in _slines):
                # ⛔⭐ STDIN GUARD, GENERALISED (ceo amendment on seat04's finding, 2026-08-30). This check
                # existed ONLY on the snobol4 path, so every dialect absorbed a stdin-reading program with NO
                # sidecar -- seat04 measured pascal read1-4 absorbed and grading against /dev/null. An entry
                # that silently reads nothing where it should read its input does not fail loudly; it produces
                # a WRONG ANSWER that looks like a verdict. Checks BOTH sidecar spellings and the config/
                # folder the flat layout puts companions in.
                _stem = sno[:-len(EXT)]
                _cfgdir = os.path.join(os.path.dirname(sno), "config")
                _base = os.path.basename(_stem)
                if any(os.path.isfile(x) for x in (_stem + ".input", _stem + ".in",
                                                   os.path.join(_cfgdir, _base + ".input"),
                                                   os.path.join(_cfgdir, _base + ".in"))):
                    excluded.append((fam, "stdin sidecar (.in/.input) -- stays as files until the stdin-sections format extension lands"))
                    continue
                mode = "plain"   # ⚠️ auto-XFAIL-by-source-verdict is NOT applied on this path yet, so a
                                 # pre-existing red absorbs as a visible FAIL rather than a silent XFAIL --
                                 # the loud direction, and a named follow-up rather than a silent difference.
                entries = [h.Entry("block", 1, os.path.basename(sno)[:-len(EXT)], _slines, _rlines)]
            else:
                # ⛔⭐ READ THE SIDECARS, exactly as the snobol4 path does. This call passed no in_path/x_path,
                # so a dialect SUITE pair's stdin and xfail sidecars were silently DROPPED -- the entries
                # absorbed and then graded against /dev/null. My first patch guarded only the PLAIN path, and
                # the positive control caught it: planting a .input beside a rebus source excluded NOTHING,
                # and snocone/rebus already carried three real sidecars the build had absorbed regardless.
                # ⭐ A guard that cannot be made to fire is not a guard -- the excluded-count of 0 was the tell,
                # and it was only visible because the count was checked against a planted case rather than read.
                try:
                    entries = h.read_block_suite(sno, ref, _bre,
                                                 in_path=h.sidecar_in_path(sno), x_path=h.sidecar_xfail_path(sno))
                except Exception as e:
                    excluded.append((fam, "dialect read refused: %s" % str(e)[:140]))
                    continue
        for e in entries:
            e.origin = "%s__%s" % (fam, e.name)
            e.src_mode = mode
        _dup = [e.origin for e in entries if e.origin in base_origins]
        if _dup:
            if len(_dup) == len(entries):
                # ⭐ FULL-DUPLICATE FAMILIES ARE ELIGIBLE FOR DELETE, NOT JUST EXCLUSION (seat05, 2026-08-30,
                # on the prolog verify+delete row). Every one of THIS family's origins already lives in the
                # base -- not appended a second time (all_entries/included/per_family untouched, so the
                # written master and its entry count are unaffected) -- but the source pair is still a
                # deletion CANDIDATE: fed into absorbed_files exactly like a fresh absorption, so the
                # existing BYTE-EQUAL-OR-NO-DELETE loop below re-reads it fresh and compares against the
                # ALREADY-PRESENT base entry (by_origin resolves to that entry either way, since nothing new
                # was added under the same origin). A rebuild that matches deletes; one that doesn't stays,
                # loudly, same failure-closed guarantee as a fresh absorption -- this never trusts the origin
                # match alone as proof of identical content.
                absorbed_files.append((fam, sno, ref, mode))
                excluded.append((fam, "already in the master (%d of %d origins present) -- rebuild of an existing entry, not a new one; NOT re-appended to the output, ELIGIBLE for deletion below once its content re-verifies against the existing entry" % (len(_dup), len(entries))))
            else:
                # Mixed family: some origins new, some already present. Not absorbed (would silently drop
                # the already-present half) and not a deletion candidate (would silently drop the new half)
                # -- stays exactly as excluded families always have, needs per-entry handling this pass
                # doesn't build.
                excluded.append((fam, "PARTIAL origin overlap (%d of %d origins already in the master) -- mixed new/existing family, left untouched; needs per-entry handling, not this pass's shape" % (len(_dup), len(entries))))
            continue
        per_family[fam] = len(entries)
        all_entries.extend(entries)
        absorbed_files.append((fam, sno, ref, mode))
        included.append(fam)
        # ⛔ COMPANIONS RIDE ALONG (the v3 board's fail-delta root cause): run_suite_entry copies the SUITE FILE'S OWN
        # DIR into each entry's scratch, so an absorbed family whose dir carried companion files (crosscheck's .in,
        # beauty's .inc) would grade a DIFFERENT program from master/ unless those companions move with it. Copy them
        # in, refusing on basename collision with different content -- never silently shadowed.
        src_dir = os.path.dirname(sno)
        if os.path.abspath(src_dir) != os.path.abspath(OUTDIR):
            for cf in sorted(os.listdir(src_dir)):
                cext = os.path.splitext(cf)[1]
                if cext in (EXT, ".ref", ".md", ".txt", ".csv", ".s", ".expected") or not os.path.isfile(os.path.join(src_dir, cf)):
                    continue
                if cext in (".in", ".xfail") and os.path.isfile(os.path.join(src_dir, os.path.splitext(cf)[0] + EXT)):
                    continue  # a SIDECAR of some suite in this dir -- its content threads through read/write sidecars, never a raw copy
                dst = os.path.join(OUTDIR, cf)
                srcf = os.path.join(src_dir, cf)
                if os.path.exists(dst) and open(dst, "rb").read() != open(srcf, "rb").read():
                    h.refuse("companion basename collision with different content: %s (from %s)" % (cf, src_dir))
                companion_copies[cf] = srcf
    # feature scan first (names are DERIVED from features), then the descriptive rename with numbered uniqueness
    # ⛔⭐⭐ THE COLLAPSE REFUSAL, AND IT COUNTS ENTRIES AGAINST ENTRIES. It lived ~150 lines up and read
    # `len(pairs) * 4 < len(base_entries)` -- a PAIR count against an ENTRY count, two different units.
    # ⭐ THE CALIBRATION, NOT THE BRANCH, WAS THE SNOBOL4-SPECIFIC PART (hq_P 2026-08-30, on seat06's
    # raku measurement). SNOBOL4's one-liner-heavy tree runs about 1:1 pair-to-entry, so a pair count is a
    # fine proxy THERE and nowhere else: in the banner-block format one pair legitimately carries a whole
    # family. Measured false refusals -- raku 15 pairs / 97 entries, snocone 34 / 264; icon (208/434),
    # pascal (58/149) and rebus (34/48) happened to sit above the 4x line and passed, which is luck, not
    # correctness. A perfect 1:1 correspondence was reading as a collapse and refusing the rebuild.
    # ⛔ NOT cured by exempting the block-format languages: that is a per-op filter over a shared node and
    # would leave the identical latent bug for whatever format lands next. The unit is the fix.
    # ⭐ The 4x ratio is unchanged and the direction of caution is unchanged: this guard is one end of an
    # axis whose other end is the dedupe/accumulation check below, and BOTH ends ship silently otherwise.
    # It is deliberately safer to false-refuse (costs a seat an hour) than to miss (cost: 1576 entries).
    # ⛔ STILL DOES NOT COVER THE DELETE-FIRST PATH: `rm ALL.*` empties base_entries, so this is skipped
    # entirely -- the guard reads the very thing a "clean rebuild" removes (hq_P, 2026-08-30; the builder
    # owning its own --clean is row snobol4-master-guard-sync-and-builder-shrink-refusal, hq_C).
    # ⛔⭐⭐ AND `all_entries` MUST BE NON-EMPTY FOR THIS TO BE A COLLAPSE AT ALL -- fixing only the unit
    # trades one false refusal for another (measured 2026-08-30, hq_B, on a scratch copy of both trees):
    # raku and snocone absorb 15 and 34 pairs into ZERO NEW ENTRIES, because every origin is already in
    # the master. That is the IDEMPOTENT REBUILD -- the same state icon/rebus/pascal reach and report as
    # "NOTHING NEW ABSORBED", exiting 0 -- and `0 * 4 < 97` refuses it just as `15 * 4 < 97` did.
    # ⭐ The reasoning is already written down twenty lines up for `not pairs`, and it transfers exactly:
    # with nothing NEW to append, `all_entries = base_entries + []` rewrites the master AS ITSELF, so the
    # builder cannot produce a smaller master no matter what the ratio says. The dangerous case is still
    # strictly `0 < new << base`: something to write, far too little of it.
    # ⛔ NOTE WHY THE OLD PREDICATE LET icon/pascal/rebus THROUGH -- 208*4>434, 58*4>149, 34*4>48. They
    # passed on their PAIR count and then took the idempotent exit anyway. A guard that admits the right
    # cases for the wrong reason is not confirmed by their green: three languages agreeing meant only that
    # three trees happened to sit above an arbitrary line.
    if base_entries and all_entries and len(all_entries) * 4 < len(base_entries):
        sys.stderr.write(
            "\u26d4 REFUSED: this rebuild would absorb only %d entrie(s) (from %d pair(s)) over an existing\n"
            "   master of %d entries.\n"
            "   That is a COLLAPSE, not a rebuild -- almost certainly a cut-over language whose sources are\n"
            "   retired, where the master IS the source and nothing on disk can rebuild it.\n"
            "   Nothing has been written; the existing master is untouched.\n"
            "   \u26d4 Do NOT fix this by deleting ALL.* and re-running -- on a cut-over language that\n"
            "      deletes the only copy. Rebuild in a scratch copy first if you truly mean it.\n"
            % (len(all_entries), len(pairs), len(base_entries)))
        raise SystemExit(2)
    counters = {}
    all_entries = base_entries + all_entries
    for e in base_entries:  # seed the counters PAST every existing name so new names never collide
        m = re.fullmatch(r"(.*)_(\d+)", e.name)
        if m:
            counters[m.group(1)] = max(counters.get(m.group(1), 0), int(m.group(2)))
    rows = []
    for e in all_entries:
        text = "\n".join(e.sno_lines)
        flags = {c: fn(text) for c, fn in COLS}   # == attrs_for_text(text, lang); COLS is the same table, rebound by main()
        if getattr(e, "src_mode", "") == "base":
            rows.append((e, flags, text))   # a BASE entry keeps its name -- names are stable identifiers (merge-never-replace)
            continue
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
    if lang == "snobol4":
        h.write_suite(all_entries, out_sno, out_ref)
        out_in = os.path.join(OUTDIR, "ALL.in")
        out_x = os.path.join(OUTDIR, "ALL.xfail")
        if not h.write_stdin_sidecar(all_entries, out_in, "*", "") and os.path.exists(out_in):
            os.remove(out_in)
        if not h.write_xfail_sidecar(all_entries, out_x, "*", "") and os.path.exists(out_x):
            os.remove(out_x)
        reread = h.read_suite(out_sno, out_ref, in_path=h.sidecar_in_path(out_sno), x_path=h.sidecar_xfail_path(out_sno))
    else:
        # ⛔⭐ THE SIDECAR WRITERS WERE SNOBOL4-ONLY TOO (ceo amendment). A dialect master got no ALL.in and no
        # ALL.xfail, so any absorbed stdin-carrying entry lost its input and any xfail lost its marker -- both
        # silently. The writers are already parameterised by comment leader; the dialect branch simply never
        # called them. Re-read WITH the sidecars, or the round-trip check verifies a file the grader will not
        # actually use.
        h.write_block_suite(all_entries, out_sno, out_ref, _CO, _CC)
        out_in = os.path.join(OUTDIR, "ALL.in")
        out_x = os.path.join(OUTDIR, "ALL.xfail")
        if not h.write_stdin_sidecar(all_entries, out_in, _CO, _CC) and os.path.exists(out_in):
            os.remove(out_in)
        if not h.write_xfail_sidecar(all_entries, out_x, _CO, _CC) and os.path.exists(out_x):
            os.remove(out_x)
        reread = h.read_block_suite(out_sno, out_ref, h.banner_re_for(_CO, _CC),
                                    in_path=h.sidecar_in_path(out_sno), x_path=h.sidecar_xfail_path(out_sno))
    import shutil
    # ⭐⭐ COMPANIONS GO IN config/, NOT THE FLAT DIR (hq_P 2026-08-30, ceo routed the cure to the finder).
    # Lon's end state is tests/<lang>/ FLAT with ALL.* plus ONE config/ folder holding runtime companions.
    # The harness READ side already implements it -- _copy_companions searches <dir> AND <dir>/config
    # (corpus_suite_harness.py:1105) -- but this WRITER still dropped them in OUTDIR, so the law was
    # half-implemented and moving them by hand was undone by the next rebuild. Sixth instance this week of one
    # half of a contract moving while the other stayed behind; the standing check (grep the other side of the
    # pair in the same commit) is exactly what would have caught it.
    # ⛔ MOVE semantics matter: the reader lets a name in the FLAT dir WIN over config/, so a stale top-level
    # copy would silently shadow the config/ one. The build writes only to config/ and clears any flat twin.
    _cfg = os.path.join(OUTDIR, "config")
    if companion_copies:
        os.makedirs(_cfg, exist_ok=True)
    for cf, srcf in sorted(companion_copies.items()):
        shutil.copy2(srcf, os.path.join(_cfg, cf))
        _stale = os.path.join(OUTDIR, cf)
        if os.path.isfile(_stale) and os.path.abspath(_stale) != os.path.abspath(srcf):
            os.remove(_stale)
    if len(reread) != len(all_entries):
        h.refuse("re-read count %d != written %d -- NOT trusting the merge" % (len(reread), len(all_entries)))
    with open(os.path.join(OUTDIR, "ALL.csv"), "w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["rank", "entry", "origin", "family", "kind", "xfail", "n_lines", "modes"] + [c for c, _ in COLS])
        for rank, (e, flags, text) in enumerate(rows, 1):
            fam = e.origin.split("__", 1)[0]
            w.writerow([rank, e.name, e.origin, fam, e.kind, int(bool(e.xfail)), len(e.sno_lines), _modes_decl.get(fam, "UNKNOWN")] + [flags[c] for c, _ in COLS])
    with open(os.path.join(OUTDIR, "ALL.excluded.txt"), "w") as f:
        for fam, why in excluded:
            f.write("%s\t%s\n" % (fam, why))
    # ⛔⭐ SEPARATE "ALREADY PRESENT" FROM "CANNOT ABSORB" (hq_P, 2026-08-30, on the dedupe landing; the
    # confusing case measured by hq_B on rebus). After dedupe, an idempotent no-op rebuild records every
    # already-absorbed family as an exclusion -- so it printed "48 entries from 0 FAMILIES" with
    # "EXCLUDED: 34", which is self-contradictory on its face and reads as a total absorption failure. The
    # real absorption run had written EXCLUDED: 0.
    # ⭐ THIS IS THE DOUBLING TELL POINTING THE OTHER WAY, and that makes it more expensive rather than less:
    # a summary number moving for a benign reason. The doubling moved the REASSURING way and hid a bug; this
    # moves the ALARMING way and will cost a seat an investigation. Sixteen of them are running this builder
    # right now, so the two exclusion CLASSES must not share one count.
    _already = [(f, w) for f, w in excluded if w.startswith("already in the master")]
    _cannot  = [(f, w) for f, w in excluded if not w.startswith("already in the master")]
    if included:
        print("MASTER SUITE: %d entries from %d families -> %s" % (len(all_entries), len(included), OUTDIR), file=sys.stderr)
    else:
        print("MASTER SUITE: %d entries, NOTHING NEW ABSORBED (idempotent rebuild) -> %s" % (len(all_entries), OUTDIR), file=sys.stderr)
    if _already:
        print("ALREADY PRESENT (not a failure -- the same entries rebuilt): %d family(ies)" % len(_already), file=sys.stderr)
    print("CANNOT ABSORB (loud, see ALL.excluded.txt): %d" % len(_cannot), file=sys.stderr)
    excluded = _cannot + _already   # file keeps both, cannot-absorb first so the real problems read first
    for fam, why in excluded:
        print("  ⛔ %s: %s" % (fam, why), file=sys.stderr)
    print("attribute columns: %d" % (6 + len(COLS)), file=sys.stderr)
    # ⛔⭐ UNKNOWN MODE IS LOUD, per hq_C's FORMAT RULING (3). A recorded-but-unknown mode is only an
    # improvement if the reader is TOLD it is unknown; a silent UNKNOWN is worse than no column at all,
    # because the field makes the question look answered. These entries are UNPROVEN at grading time --
    # never PASS, never FAIL -- and any consumer that grades them in a default mode reproduces the exact
    # defect the column exists to expose.
    _fams = sorted({e.origin.split("__", 1)[0] for e in all_entries})
    _unk = [f for f in _fams if f not in _modes_decl]
    _unk_entries = sum(1 for e in all_entries if e.origin.split("__", 1)[0] not in _modes_decl)
    if _unk:
        print("⛔ MODE UNKNOWN for %d of %d families (%d entries) -- these are UNPROVEN, not passes:"
              % (len(_unk), len(_fams), _unk_entries), file=sys.stderr)
        for f in _unk[:10]:
            print("     %s" % f, file=sys.stderr)
        if len(_unk) > 10:
            print("     ... and %d more (the full set is the `modes` column = UNKNOWN in ALL.csv)" % (len(_unk) - 10), file=sys.stderr)
        print("   Declare them in %s/config/MODES.tsv as `family<TAB>modes`. ⛔ Do NOT guess from the family"
              % ROOT, file=sys.stderr)
        print("   name: a heuristic that is right on every case you have gives no signal when it starts being wrong.", file=sys.stderr)
    else:
        print("mode: declared for all %d families" % len(_fams), file=sys.stderr)
    # -- BYTE-EQUAL-OR-NO-DELETE (the house law, applied to absorption): every absorbed source pair is re-read FRESH
    # from disk and compared entry-by-entry against the written master. A family that does not verify is NEVER deleted.
    # ⭐ GENERALISED PAST SNOBOL4 (seat05, 2026-08-30, on the ceo all-hands consolidation directive): this used to gate
    # on `ok = lang == "snobol4"`, so EVERY non-snobol4 family landed in `unverified` unconditionally, regardless of
    # whether it actually matched -- --delete-absorbed could never delete a single prolog/raku/snocone/rebus file, not
    # because anything failed to verify, but because verification was never attempted for them at all. That silent
    # short-circuit is now gone: every language re-reads its pair with the SAME dialect reader `main()` used to absorb
    # it in the first place (read_block_suite, falling back to read_suite for a genuine one-line-dialect pair -- the
    # identical fallback used at absorption time above) and compares entry-by-entry, same fields, same failure-closed
    # `except Exception: ok = False`. snobol4's own "plain" (whole-file, non-suite) absorption keeps its dedicated
    # whole-file comparison unchanged -- that shape does not exist for any other language (mode is always "suite"
    # outside the snobol4 branch, see main()'s absorption loop above), so it is untouched, not generalised.
    by_origin = {}
    for e, flags, text in rows:
        by_origin[e.origin] = e
    verified, unverified = [], []
    for fam, sno, ref, mode in absorbed_files:
        ok = True
        try:
            if mode == "plain":
                # ⛔⭐ HALF-GENERALIZED CONTRACT, THE FOURTH INSTANCE (seat07 2026-08-30, on the task file's
                # own newly-adopted law naming this exact class): this branch used to read
                # `lang == "snobol4" and mode == "plain"`, but mode=="plain" stopped being SNOBOL4-only the
                # moment hq_P's bannerless-dialect-as-ONE-entry fix landed (absorption above sets mode="plain"
                # for ANY language's bannerless single-file family, not just SNOBOL4's own historic plain-file
                # class) -- the ABSORPTION half of the contract generalized, the VERIFICATION half's language
                # guard did not. MEASURED: a from-scratch rebus rebuild under this exact tree read "VERIFIED: 1
                # families; UNVERIFIED: 33" -- every one of the 33 bannerless AST-drift stems fell through to
                # the `else` branch below, which has no banner to find, so its read_block_suite/read_suite
                # fallback fragmented each file into seqN one-line entries matching no origin in `by_origin`.
                # The comparison body itself was ALREADY language-agnostic (EXT is already the resolved
                # extension for whichever --lang is running, os.path.basename/sidecar_in_path assume nothing
                # SNOBOL4-specific) -- only the gate needed to drop the language check.
                # ⛔⭐⭐ A CHECK THAT DOES NOT CARRY EVERY FIELD THE GRADER READS IS NOT A CHECK (hq_C, 2026-08-30,
                # from their own --split-write round trip which PASSED while dropping stdin for all 9
                # stdin-bearing snobol4 entries -- it compared bodies only). This comparison authorizes
                # DELETING the source, so a field it does not check is a field that can be silently lost
                # forever. It compared sno_lines and ref only: an absorbed entry whose stdin or xfail was
                # dropped verified CLEAN and its source became deletable.
                # ⛔ AND rc CANNOT SUBSTITUTE FOR THE FIELD CHECK. hq_C measured snobol4
                # arb_capture_alt_replace_branch_1: WITH stdin rc=0 and 9 bytes "cat/house"; WITHOUT stdin
                # rc=0 and ZERO bytes. IDENTICAL EXIT CODE. A stdin-starved program exits perfectly cleanly
                # and simply produces nothing, so only comparing the FIELD detects it.
                e = by_origin.get("%s__%s" % (fam, os.path.basename(sno)[:-len(EXT)]))
                _src_in = h.sidecar_in_path(sno)
                _want_stdin = open(_src_in).read() if (_src_in and os.path.isfile(str(_src_in))) else None
                ok = e is not None and e.sno_lines == open(sno).read().splitlines() and \
                    (e.ref if isinstance(e.ref, list) else str(e.ref).split("\n")) == open(ref).read().splitlines() and \
                    (e.stdin or None) == (_want_stdin or None)
            else:
                if lang == "snobol4":
                    reread = h.read_suite(sno, ref, in_path=h.sidecar_in_path(sno), x_path=h.sidecar_xfail_path(sno))
                else:
                    # ⛔⭐ RE-READ WITH THE SAME BANNER-PRESENCE DISCRIMINATOR ABSORPTION USES, NOT A TRY/EXCEPT
                    # FALLBACK (seat05, 2026-08-30 -- this was the try/read_block_suite/except/read_suite shape
                    # absorption ITSELF used when this verification code was first written; hq_B's 13c186ea
                    # rewrote absorption to decide by banner presence instead, and this comparison silently went
                    # stale against it -- the two-independent-copies-of-one-decision class RULES.md's TRANSCRIPTION
                    # rule warns about, just inside one file instead of across two). MEASURED, not assumed: a
                    # genuinely byte-identical banner-less block family (rung05_backtrack_backtrack, content
                    # confirmed identical by hand) read WRONG under the old try/except -- read_block_suite raising
                    # on a bannerless file, the except then handing a multi-line prolog program to read_suite
                    # (SNOBOL4's ONE-LINE reader), producing garbage that never matched. Mirrors main()'s own
                    # discriminator exactly. ⛔ SIDECARS TOO (hq_C, same night, in-conflict with this rewrite --
                    # folded in rather than reverted): the dialect re-read must load what the grader loads, or
                    # `se.stdin` is always None and a real stdin mismatch compares None==None and passes. The
                    # banner-PRESENT arm below passes in_path/x_path exactly as hq_C's fix does; the banner-
                    # ABSENT arm needs no sidecar load at all, because absorption's own STDIN GUARD above
                    # (search "STDIN GUARD, GENERALISED") already refuses any bannerless file that HAS one
                    # before it can ever reach absorbed_files -- there is structurally nothing to load.
                    _bre_v = h.banner_re_for(_CO, _CC)
                    _slines_v = open(sno, encoding="utf-8", errors="replace").read().splitlines()
                    _rlines_v = open(ref, encoding="utf-8", errors="replace").read().splitlines()
                    if not any(_bre_v.match(_l) for _l in _slines_v):
                        reread = [h.Entry("block", 1, os.path.basename(sno)[:-len(EXT)], _slines_v, _rlines_v)]
                    else:
                        reread = h.read_block_suite(sno, ref, _bre_v,
                                                    in_path=h.sidecar_in_path(sno), x_path=h.sidecar_xfail_path(sno))
                for se in reread:
                    e = by_origin.get("%s__%s" % (fam, se.name))
                    if e is None or e.kind != se.kind or (e.stdin or None) != (se.stdin or None):
                        ok = False
                        break
                    if e.kind == "line":
                        strip = lambda ln, nm: ln[: -len(";* %s" % nm)] if ln.endswith(";* %s" % nm) else ln
                        if strip(e.sno_lines[0], e.name) != strip(se.sno_lines[0], se.name) or e.ref != se.ref:
                            ok = False
                            break
                    else:
                        if e.sno_lines != se.sno_lines or e.ref != se.ref or bool(e.xfail) != bool(se.xfail):
                            ok = False
                            break
        except Exception:
            ok = False
        (verified if ok else unverified).append((fam, sno, ref))
    print("VERIFIED for deletion: %d families; UNVERIFIED (kept): %d" % (len(verified), len(unverified)), file=sys.stderr)
    if delete_absorbed:
        n = 0
        touched_dirs = set()
        for fam, sno, ref in verified:
            os.remove(sno)
            os.remove(ref)
            for sc in (sno[:-len(EXT)] + ".in", sno[:-len(EXT)] + ".xfail"):
                if os.path.isfile(sc):
                    os.remove(sc)
                    n += 1
            touched_dirs.add(os.path.dirname(sno))
            n += 2
        for d in sorted(touched_dirs):
            if os.path.abspath(d) == os.path.abspath(OUTDIR):
                continue
            if not any(f.endswith(EXT) for f in os.listdir(d)):
                for cf in sorted(os.listdir(d)):
                    if os.path.splitext(cf)[1] in COMPANION_EXTS or cf.endswith(".ref"):
                        os.remove(os.path.join(d, cf))
                        n += 1
        for dirpath, dirnames, filenames in list(os.walk(ROOT, topdown=False)):
            if not dirnames and not filenames and os.path.abspath(dirpath) != os.path.abspath(ROOT):
                os.rmdir(dirpath)
        print("DELETED %d absorbed source files (verified families only; fully-absorbed dirs' companions included); empty dirs pruned" % n, file=sys.stderr)
    elif verified:
        print("(dry run: pass --delete-absorbed to remove the %d verified families' source pairs)" % len(verified), file=sys.stderr)


if __name__ == "__main__":
    main()
