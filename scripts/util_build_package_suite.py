#!/usr/bin/env python3
"""util_build_package_suite.py -- ONE CONTAINER PER VENDORED PACKAGE (task
every-vendored-package-absorbed-into-the-one-liner-or-multi-liner-python-harness-with-oracle-cut-refs,
hq_T doorbell 2026-09-04, Lon verbatim: "You make the programs run, you measure the output, make a REF
file, and place it into a ONE-LINER or a MULTI-LINER Python test harness.").

For corpus/packages/<lang>/<pkg>/, builds an ADDITIVE container beside the shipped files -- nothing in
the package is moved or rewritten: ALL.<ext> / ALL.ref / ALL.csv, plus ALL.in (only if some entry needs
stdin) and ALL.wantrc (only if some entry's correct exit code is non-zero). A shipped program not
absorbed is named, with a reason, in ALL.excluded.txt -- absorbed + excluded always equals the shipped
count (self-checked below; a mismatch REFUSES rather than silently shrinking the denominator).

REFS ARE CUT FROM THE PACKAGE'S OWN ORACLE, NEVER FROM SCRIP (hq_T's doorbell, verbatim) -- via
lib_oracle_flags.sh's resolve_oracle_bin()/run_oracle(), corpus_suite_harness's OWN authority, same as
every other oracle call in this project. This is deliberately NOT cmd_capture_oracle_refs's three-way
(oracle+m3+m4) agreement gate: that gate is a conservative bootstrap for suites that only want to freeze
currently-GREEN witnesses, and withholding a ref whenever SCRIP disagrees would quietly erase exactly the
defects this container exists to surface. The oracle's own answer is the ref, full stop; SCRIP's
agreement or disagreement with it is what grading measures, not a precondition for measuring at all.

Six ways a shipped program is excluded rather than absorbed, each named by reason in ALL.excluded.txt:
  1. a `-INCLUDE "X"` naming a file not vendored beside it, in the package dir or the source's own dir (a
     corpus completeness gap, not gradable here -- the exclusion reason names the dirs it searched, never "anywhere")
  2. the oracle itself cannot produce ground truth: crashes (signal), hangs past the timeout, or dies
     gracefully (SPITBOL's own "ERROR nnn -- ... / in statement N" fatal-report shape, ported from
     test_snobol4_aisnobol_suite.sh's sbl_died(), already measured correct on this exact package)
  3. the oracle's output is empty -- an empty ref grades a file against nothing forever and reads as
     coverage where there is none (same refusal cmd_capture_oracle_refs makes, same reason)
  4. (SNOBOL4 sources only -- lang in {"", "snobol4"}) the source carries a UTF-8 BOM -- checked
     structurally, BEFORE the oracle ever runs (measured on the dotnet package's 5 BOM-prefixed files: a
     BOM at byte 0 confuses SPITBOL's label lexer, producing either a segfault -- which the crash check
     in reason 2 would also catch -- or a compile-time-only fatal that never executes the program. That
     second shape is NOT text-matchable from here: its only footer, "No END statement found in source
     file(s).", prints to STDERR only, and h.run_oracle()/ora_text is STDOUT ONLY by this project's own
     convention -- see has_bom()'s docstring for the two dead ends tried first)
  5. (SNOBOL4 sources only -- lang in {"", "snobol4"}) more than one top-level END statement -- a
     textbook chapter's worth of separate example programs concatenated into one shipped file, not a
     single valid program (measured on the dotnet package's chap7.sno: 2 top-level END lines). SCRIP
     correctly REFUSES a duplicate 'END' label ("no code generated"); the oracle instead silently keeps
     going past it and produces real-looking output from a partial run -- grading SCRIP against that
     would blame it for being the stricter, arguably more correct, of the two engines, the same
     false-disagreement class as reason 2.
  6. the source is not valid UTF-8 (genuine 8-bit content -- measured on csnobol4_suite's 8bit2.sno, an
     ISO-8859-1 French-language test using accented identifiers, byte 0xC9). read_text_tolerant() can
     decode it (falls back to latin-1, which never raises), but h.write_suite()'s Path.write_text() has no
     encoding param and re-encodes as UTF-8 -- a latin-1 chr(0xC9) round-trips out as bytes 0xC3 0x89, two
     bytes where the vendored source had one. SCRIP is byte-oriented, so the container's copy would stop
     being byte-identical to what the ref was cut from: the same false-disagreement shape as reasons 4/5,
     just discovered one level down in the write path rather than the read path. Fixing write_suite itself
     (shared machinery other suites already depend on) is out of this tool's lane; flagged to hq_T.
Stdin: `<stem>.stdin` / `<stem>.IN` / `<stem>.in` / `<stem>.input` beside the source, tried in that order
(vendored packages ship the exact case they shipped in -- aisnobol's are `.IN` -- so this checks case
variants explicitly rather than relying on with_suffix()'s case-sensitive match).

Grading is unchanged machinery: python3 scripts/corpus_suite_harness.py run ALL.<ext> ALL.ref --modes m3,m4
Stdlib only except corpus_suite_harness/util_build_master_suite, imported for the ONE suite-grammar and
ONE attribute-table authorities respectively -- never a second parser or a second column set.
"""
import os
import re
import sys
import csv
import argparse
from pathlib import Path

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import corpus_suite_harness as h  # noqa: E402
import util_build_master_suite as m  # noqa: E402

LANG_EXT = {"": ".sno", "snobol4": ".sno", "prolog": ".pl", "raku": ".raku",
            "snocone": ".sc", "rebus": ".reb", "icon": ".icn", "pascal": ".pas"}

# ⭐ PORTED FROM test_snobol4_aisnobol_suite.sh's sbl_died(), MEASURED CORRECT ON THIS EXACT PACKAGE
# (BUILDLIB.sno, 2026-09-03): SPITBOL's graceful-fatal shape is a banner line containing " : ERROR nnn --
# " together with a following "in statement N" footer -- rc alone is not a liveness signal (SPITBOL exits
# 0 after printing this), so the text shape is the only thing that catches it. ⚠ seat15 2026-09-04 tried
# widening this to also catch SPITBOL's OTHER fatal shape, "No END statement found in source file(s).":
# MEASURED WRONG -- that footer prints to STDERR only (`sbl Test2.sno 2>/dev/null` vs `2>/tmp/e`, dotnet
# package's Test2.sno/SourceLines001.sno), and h.run_oracle()/ora_text is STDOUT ONLY by design (the same
# convention test_snobol4_aisnobol_suite.sh's header documents: merging streams manufactures false
# disagreements). A text-pattern check can never see a line that never reaches the stream it reads --
# see has_bom() below for the structural check that actually catches this class.
_SBL_DIED_RE1 = re.compile(r" : ERROR \d\d\d -- ")
_SBL_DIED_RE2 = re.compile(r"^in statement +\d+$", re.MULTILINE)


def sbl_died(text):
    return bool(_SBL_DIED_RE1.search(text)) and bool(_SBL_DIED_RE2.search(text))


# ⭐ ADDED (seat15, 2026-09-04, measured on the dotnet package's 5 BOM-prefixed files -- Test.sno,
# Test2.sno, SourceLines001-003.sno): a U+FEFF byte-order-mark at byte 0 confuses SPITBOL's label lexer.
# Two failure shapes result, NEITHER caught by sbl_died() (both proven by direct oracle invocation):
# (a) segfault mid-report, rc=139 -- IS caught, but only by the separate crash check in build() below,
#     coincidentally, not by anything BOM-aware; (b) a compile-time-only bail: stdout carries just the
# " : ERROR 214 -- bad label or misplaced continuation line" line (matches sbl_died's RE1) with NO
# footer on stdout at all (its footer, "No END statement found in source file(s).", is stderr-only --
# see the comment above) -- so sbl_died() returns False and the lexer-confusion artifact on stdout would
# be absorbed as if it were the program's real ground truth. Checking the SOURCE BYTES directly, before
# ever invoking the oracle, is unambiguous and sidesteps the stdout/stderr question entirely.
_BOM = chr(0xFEFF)  # spelled by codepoint, not a literal byte, so the source stays reviewable in plain ASCII


def has_bom(src_text):
    return src_text.startswith(_BOM)


_INCLUDE_RE = re.compile(r"-INCLUDE\s+['\"]([^'\"]+)['\"]")


def find_include_gap(src_text, pkg_dir, src_dir=None):
    """The first -INCLUDE target this source names that resolves in NONE of the dirs searched, and the dirs that
    were searched. -> (name_or_None, [dirs]).
    ⛔⭐ THE RECORDED REASON USED TO OVERCLAIM, AND THE OVERCLAIM IS THE DEFECT (hq_T 2026-09-05, measured on the
    exclusions this function itself wrote). It searched ONE directory and the caller wrote "not vendored anywhere in
    corpus" into ALL.excluded.txt -- a sentence nobody had measured. Of the 8 exclusions on record, 3 are false by
    that sentence's own claim: `-INCLUDE '../modules/ndbm/ndbm.sno'` and `'../modules/time/time.sno'` name files that
    ARE vendored, as packages/snobol4/csnobol4_suite/{ndbm,time}.sno -- the vendored layout flattened the module
    tree and the sources kept naming the original path. A reason is evidence a later reader acts on; one that states
    a wider search than was performed sends them to look for a file they already have.
    ⛔ AND IT READ ONLY THE FIRST -INCLUDE (`.search`, not `.findall`), so a source whose first include resolved and
    whose second did not was absorbed and graded WITHOUT the second -- the same silent missing-dependency class this
    tool exists to keep out of a package suite.
    ⭐ src_dir is the SOURCE FILE'S OWN directory, which is pkg_dir only when the package is flat: build() globs
    `*/*{ext}` too, so a nested source's companion sitting beside it was reported as a gap while being right there."""
    dirs = list(dict.fromkeys([d for d in (src_dir, pkg_dir) if d is not None]))
    for inc in _INCLUDE_RE.findall(src_text):
        # ⛔⭐ THE STRIPPED FORM COUNTS AS VENDORED, AND THIS IS NOT LENIENCY (measured on csnobol4_suite/line.sno and
        # include.sno, which the findall widening above newly caught). Both name `-INCLUDE "line2.sno "` WITH A
        # TRAILING SPACE, beside `-INCLUDE "line2.sno"` without one -- they are csnobol4's own tests OF include-name
        # handling, and line2.sno is vendored right there. This function answers ONE question: is the dependency in
        # the corpus. Whether the compiler trims the name is the very thing those programs test, and their .ref
        # records whichever answer the oracle gives -- so excluding them would drop the two entries that grade the
        # behaviour, on the grounds that they grade it.
        if not any((d / inc).is_file() or (d / inc.strip()).is_file() for d in dirs):
            return inc, dirs
    return None, dirs


_END_LINE_RE = re.compile(r"^END(?:[ \t]|$)")


def count_top_level_ends(src_text):
    """Number of un-indented 'END' statement lines -- SNOBOL4's own program terminator. More than one
    means the shipped file is several example programs concatenated under one name, not a single valid
    program (measured on the dotnet package's chap7.sno: a textbook chapter, 2 top-level END lines).
    Mirrors test_snobol4_dotnet_suite.sh's own check (`grep -cE '^END([[:space:]]|$)'`)."""
    return sum(1 for line in src_text.splitlines() if _END_LINE_RE.match(line))


def read_text_tolerant(path):
    """UTF-8 first (so a real UTF-8 BOM still decodes to U+FEFF for has_bom() to see); falls back to
    latin-1 -- which never raises -- for genuinely non-UTF-8 8-bit source (measured on csnobol4_suite's
    8bit2.sno: ISO-8859-1 French text using accented identifiers like Etude/francais, byte 0xC9 -- a
    deliberate 8-bit-character test, not corruption), so the whole run no longer crashes on the first such
    file. Returns (text, used_latin1) -- the caller EXCLUDES rather than absorbs when used_latin1 is True:
    measured (see enctest, this session) that h.write_suite's Path.write_text() re-encodes as UTF-8 with no
    encoding param exposed, so a latin-1 codepoint like chr(0xC9) round-trips out as the TWO bytes 0xC3 0x89
    -- the container's copy would silently stop being byte-identical to the vendored source the ref was cut
    from, the exact false-disagreement class BOM/multi-END are already excluded for. Fixing write_suite
    itself is out of this tool's lane -- it is shared machinery other suites already depend on."""
    raw = path.read_bytes()
    try:
        return raw.decode("utf-8"), False
    except UnicodeDecodeError:
        return raw.decode("latin-1"), True


def stdin_for(stem, pkg_dir):
    # ⭐ `.dat` ADDED (measured on arizona_tests' README.md: "16 with a matching .dat (fed as stdin..."
    # -- a DIFFERENT convention from jcon_tests' same-named extension, which feeds it as BOTH argv[1]
    # AND stdin (JCON's own addtest harness; this builder has no argv mechanism, see the .dat-companion
    # exclusion reason in build() below). Tried last, after every stdin-specific extension, so a package
    # that ships BOTH a real `.stdin` file and an unrelated `.dat` data companion still prefers the
    # explicit one. Safe for jcon_tests' own .dat-as-argv entries too, not just neutral: feeding the same
    # content as stdin can only help (a program that reads it via stdin as well as/instead of argv[1] now
    # gets real input, matching JCON's own "both at once" convention) or leave them exactly as excluded
    # as before (still empty if the program truly needs argv[1] specifically).
    # ⛔⭐ CORRECTION (seat14, same sitting): the ORIGINAL version of this comment claimed "verified
    # empirically... zero change to every absorbed entry... none of jcon's programs read their .dat via
    # stdin at all" -- THAT WAS WRONG, and it shipped in a pushed commit before the mistake was caught.
    # The "verification" piped the rebuild through `grep -E "argv|balanced|..."`, which printed nothing
    # because the build had actually REFUSED early (a stale-binary preflight, unrelated to this change)
    # -- silence from the filter was misread as "nothing changed" rather than "the command never ran the
    # part being checked." Same class of mistake as the tail-3-hides-a-refusal incident minutes earlier
    # in this same sitting (RULES.md's own INSTRUMENT LAWS: an instrument that cannot fail). Re-run with
    # full, unfiltered output once caught: jcon_tests actually goes 60 -> 70 absorbed with this change,
    # not 60 -> 60. Verified for real this time (full output inspected, no grep/tail between the command
    # and the read): 17 of jcon's .dat-stem entries end up stdin-fed and correct (spot-checked `btrees`
    # against its shipped .std under both the argv+stdin invocation JCON's own addtest convention uses
    # AND stdin-only -- identical, byte-for-byte, confirming the program reads via stdin regardless of
    # argv). Only 4 (geddump/htprep/prepro/tgrlink) still come back empty and stay excluded as
    # genuinely argv[1]-dependent. See the corrected corpus commit for the real numbers.
    for ext in ("stdin", "IN", "in", "input", "Input", "dat"):
        cand = pkg_dir / f"{stem}.{ext}"
        if cand.is_file():
            return cand
    return None


def build(pkg_dir, lang, out_prefix="ALL"):
    ext = LANG_EXT[lang]
    paths = h.resolve_paths()
    h.check_scrip(paths)
    oracle_bin, flags = h.resolve_oracle_bin(paths, lang)
    print(f"oracle: {oracle_bin} {flags}", file=sys.stderr)
    # ⭐ ONE LEVEL OF SUBDIRS, MATCHING THE TASK'S OWN DONE-WHEN EXACTLY (`find "$d" -maxdepth 2 ...`,
    # seat15 2026-09-04, measured on corpus/packages/prolog/gnu_prolog: all 62 shipped .pl files sit one
    # level down, in BipsPl/ and Pl2Wam/, zero directly in gnu_prolog/ itself -- a bare `pkg_dir.glob`
    # found nothing and would have REFUSED a real, non-empty package). Matching DONE-WHEN's own depth is
    # what keeps "shipped" meaning the same count in both places; going deeper (rglob) would not (checked:
    # gnu_prolog has no depth-3 files, so this and unrestricted recursion agree here, but won't in general).
    srcs = sorted(p for p in list(pkg_dir.glob(f"*{ext}")) + list(pkg_dir.glob(f"*/*{ext}"))
                  if not p.stem.startswith(out_prefix))
    if not srcs:
        h.refuse(f"no {ext} files under {pkg_dir} (direct or one level down)")
    entries, excluded = [], []
    for i, src in enumerate(srcs, 1):
        # ⭐ QUALIFY WITH PARENT DIR WHEN NESTED (measured on csnobol4_suite: aa.sno at pkg_dir root AND
        # aa/aa.sno one level down are BOTH real, byte-identical vendored fixtures -- bare name would
        # give both entries the identical name/origin ("aa"/"csnobol4_suite__aa"), which is exactly the
        # kind of collision THE MASTER SUITE's CSV manifest exists to make queryable by name; a name that
        # means two different things defeats that).
        name = f"{src.parent.name}/{src.stem}" if src.parent != pkg_dir else src.stem
        text, non_utf8 = read_text_tolerant(src)
        if non_utf8:
            excluded.append((name, "source is not valid UTF-8 (genuine 8-bit content, e.g. accented bytes) -- "
                                        "h.write_suite's writer has no encoding param and would re-encode it as UTF-8, "
                                        "silently changing the byte-oriented source's own byte count/content versus what "
                                        "the ref was cut from; excluded rather than absorbed-but-wrong until write_suite "
                                        "itself grows byte-faithful output (shared machinery, out of this tool's lane)"))
            print(f"[{i}/{len(srcs)}] {name}: EXCLUDED (non-UTF-8 8-bit source)", file=sys.stderr)
            continue
        gap, gap_dirs = find_include_gap(text, pkg_dir, src.parent)
        if gap:
            _where = ", ".join(str(d) for d in gap_dirs)
            excluded.append((name, f"missing corpus dependency: -INCLUDE {gap!r} resolves in none of {_where} "
                                        f"(searched exactly those dirs -- this reason names its own search, so a file "
                                        f"that IS vendored elsewhere under another path reads as what it is)"))
            print(f"[{i}/{len(srcs)}] {name}: EXCLUDED (missing -INCLUDE {gap!r})", file=sys.stderr)
            continue
        if lang in ("", "snobol4") and has_bom(text):
            excluded.append((name, "source carries a UTF-8 BOM -- confuses the oracle's label lexer (segfault, or a "
                                        "compile-time-only fatal that never executes the program), so its output would be "
                                        "a lexer-confusion artifact, not the program's ground truth"))
            print(f"[{i}/{len(srcs)}] {name}: EXCLUDED (UTF-8 BOM)", file=sys.stderr)
            continue
        if lang in ("", "snobol4"):
            n_end = count_top_level_ends(text)
            if n_end > 1:
                excluded.append((name, f"multi-program file: {n_end} top-level END statements -- not a single valid "
                                            "program (SCRIP correctly refuses a duplicate 'END' label; the oracle instead "
                                            "silently continues past it, so neither engine's output is a meaningful "
                                            "single-program comparison)"))
                print(f"[{i}/{len(srcs)}] {name}: EXCLUDED (multi-program file, {n_end} END statements)", file=sys.stderr)
                continue
        stdin_path = stdin_for(src.stem, src.parent)  # the file's OWN dir -- == pkg_dir when flat, matters once nested
        stdin_text = stdin_path.read_text() if stdin_path else None
        ora_text, ora_rc, ora_kind = h.run_oracle(oracle_bin, flags, src, paths["timeout"], stdin_text=stdin_text)
        if ora_kind == "HANG":
            excluded.append((name, "oracle timed out -- non-terminating or too slow for the grading timeout"))
            print(f"[{i}/{len(srcs)}] {name}: EXCLUDED (oracle HANG)", file=sys.stderr)
            continue
        if ora_kind == "UNPROVEN":
            h.refuse(f"oracle binary not runnable at all: {oracle_bin}")
        if ora_rc is not None and ora_rc < 0:
            excluded.append((name, f"oracle crashed (signal {-ora_rc})"))
            print(f"[{i}/{len(srcs)}] {name}: EXCLUDED (oracle crashed, signal {-ora_rc})", file=sys.stderr)
            continue
        if sbl_died(ora_text):
            excluded.append((name, "oracle died: graceful fatal report (e.g. undefined function) -- no ground truth to grade against"))
            print(f"[{i}/{len(srcs)}] {name}: EXCLUDED (oracle died)", file=sys.stderr)
            continue
        if not ora_text.strip():
            # ⛔⭐ AN EMPTY RUN IS NOT ALWAYS AN EMPTY PROGRAM (measured on jcon_tests, the first package
            # with argv-consuming entries: 14 of 91 files ship a same-stem `.dat` that JCON's own
            # `addtest` convention feeds as BOTH argv[1] AND stdin -- test_icon_jcon_suite.sh already
            # does this correctly. This builder/h.run_oracle() has no argv-passing mechanism at all
            # (ALL.in covers stdin only), so it invoked these with NEITHER, and several genuinely need
            # the file to produce anything -- a harness gap, not a fact about the program. ⚠ NOT every
            # .dat-stem file needs argv: 7 others (endetab/fncs1/io/mffsol/mindfa/others/recent)
            # produced correct non-empty output through this exact same bare invocation and are already
            # ABSORBED above, untouched by this branch -- so this reason is scoped to the empty-output
            # case specifically, never a blanket "has a .dat -> exclude" rule that would demote them.
            _dat = src.parent / f"{src.stem}.dat"
            if _dat.is_file():
                excluded.append((name, "oracle produced EMPTY output when run bare, but a same-stem .dat companion "
                                            "exists -- likely needs it as argv[1] (JCON addtest convention), and this "
                                            "builder has no argv-passing mechanism (ALL.in is stdin-only); needs an "
                                            "ALL.args extension or grading via the package's own dedicated runner, "
                                            "not a fact about the program's real output"))
                print(f"[{i}/{len(srcs)}] {name}: EXCLUDED (empty output, has .dat -- likely needs argv, harness gap)", file=sys.stderr)
                continue
            excluded.append((name, "oracle produced EMPTY output -- refusing to mint a vacuous ref"))
            print(f"[{i}/{len(srcs)}] {name}: EXCLUDED (empty oracle output)", file=sys.stderr)
            continue
        want_rc = ora_rc if ora_rc else 0
        e = h.Entry("block", len(entries) + 1, name, text.splitlines(), ora_text.split("\n"),
                     stdin=stdin_text, want_rc=want_rc)
        entries.append(e)
        _fed = f" [stdin: {stdin_path.name}]" if stdin_text is not None else ""
        _rc = f" [want_rc={want_rc}]" if want_rc else ""
        print(f"[{i}/{len(srcs)}] {name}: ABSORBED{_fed}{_rc}", file=sys.stderr)

    tot = len(srcs)
    if len(entries) + len(excluded) != tot:
        h.refuse(f"population arithmetic broken: {len(entries)} absorbed + {len(excluded)} excluded != {tot} shipped")

    out_sno = pkg_dir / f"{out_prefix}{ext}"
    out_ref = pkg_dir / f"{out_prefix}.ref"
    out_in = pkg_dir / f"{out_prefix}.in"
    out_wantrc = pkg_dir / f"{out_prefix}.wantrc"
    out_csv = pkg_dir / f"{out_prefix}.csv"
    out_excl = pkg_dir / f"{out_prefix}.excluded.txt"

    if not entries:
        # ⭐ hq_T ruling 2026-09-04 (q-package-builder-zero-absorbable-policy, to seat15): refusing the
        # container is correct -- an ALL.csv with 0 rows would read as HARNESSED with a score of zero to
        # every downstream census -- but the honest state is OWED WITH A NAMED ACCOUNTING, not silence.
        # Land the excluded-reasons file even though no container follows; a zero-absorbable package is
        # NOT-HARNESS-SHAPED, and the reasons are exactly as real as they'd be beside a non-empty ALL.csv.
        if excluded:
            out_excl.write_text("\n".join(f"{name}: {reason}" for name, reason in sorted(excluded)) + "\n")
        h.refuse(f"{pkg_dir}: zero absorbable entries of {tot} shipped -- a container that grades nothing is not a "
                    f"container; NOT-HARNESS-SHAPED accounting written to {out_excl.name} ({tot} named, 0 absorbable)")

    wrote_in = h.write_suite(entries, str(out_sno), str(out_ref), out_in=str(out_in), lang=lang)
    if not wrote_in and out_in.exists():
        out_in.unlink()

    wr_lines = [f"{e.name}\t{e.want_rc}" for e in entries if e.want_rc]
    if wr_lines:
        out_wantrc.write_text("\n".join(wr_lines) + "\n")
    elif out_wantrc.exists():
        out_wantrc.unlink()

    table_lang = lang or "snobol4"
    cols, _ = m.LANG_TABLES[table_lang]
    # ⛔⭐ THE `modes` COLUMN (row every-vendored-package-..., hq_T 2026-09-05) -- cmd_run's MIRROR TRAP
    # guard (this same file, row board-icon-master-runs-the-ast-graded-parser-fixtures /
    # test_gate_modes_declaration_travels.sh, landed the same day) now REFUSES to grade ANY suite whose
    # ALL.csv lacks a `modes` column at all, run() called with or without --by-modes-column -- measured
    # AFTER this fix's need surfaced: `run gimpel/ALL.sno gimpel/ALL.ref --modes m3,m4` (no --lang, an
    # already-DONE, already-committed SNOBOL4 package) refuses identically. Every package this builder
    # ever wrote (aisnobol/dotnet/csnobol4_suite/gimpel, all pre-dating that guard) is silently
    # ungradeable until its ALL.csv is rebuilt with this column -- not an Icon-specific gap. Every entry
    # this builder ever produces is a normal runnable program (never an ast-only fixture mixed in with
    # run-graded ones the way the Icon MASTER suite is), so ONE constant value for the whole package is
    # honest, not a per-entry guess: the language's own LANG_CONFIGS declaration, or "m3,m4" for
    # blank/snobol4 (every package graded so far was always run `--modes m3,m4`, never `--lang`).
    pkg_modes = h.LANG_CONFIGS[lang]["modes"] if lang in h.LANG_CONFIGS else "m3,m4"
    # ⛔ MERGE NOTE (two seats found the identical bug independently the same day -- this row and task
    # snobol4-aisnobol-csv-missing-modes-column-blocks-measurement): the other fix left `modes` PRESENT
    # but always "" ("this builder has no ast-graded entries, so empty is the honest declaration").
    # Traced `_modes_for()`/the MIRROR TRAP guard in corpus_suite_harness.py to settle it rather than
    # guess: both arms test the string EXACTLY EQUALS "ast", nothing tests presence/absence of a value --
    # "" and pkg_modes are provably equivalent to every current guard. Kept pkg_modes anyway, not to
    # relitigate a settled call but because it is strictly more informative for zero behaviour difference
    # (a human or future tool reading the CSV sees the real grading modes instead of a blank that looks
    # identical to "nobody declared anything"), and it is already measured working (jcon_tests, gimpel).
    with open(out_csv, "w", newline="") as f:
        w = csv.writer(f, lineterminator="\n")
        w.writerow(["rank", "entry", "origin", "package", "n_lines", "stdin", "want_rc", "modes"] + [c for c, _fn in cols])
        for e in entries:
            joined = "\n".join(e.sno_lines)
            flags_row = m.attrs_for_text(joined, table_lang)
            w.writerow([e.seq, e.name, f"{pkg_dir.name}__{e.name}", pkg_dir.name, len(e.sno_lines),
                        1 if e.stdin else 0, e.want_rc, pkg_modes] + [flags_row[c] for c, _fn in cols])

    if excluded:
        out_excl.write_text("\n".join(f"{name}: {reason}" for name, reason in sorted(excluded)) + "\n")
    elif out_excl.exists():
        out_excl.unlink()

    print(f"\n{pkg_dir}: {len(entries)} absorbed, {len(excluded)} excluded, {tot} shipped (balanced)", file=sys.stderr)
    return entries, excluded, tot


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("package_dir")
    ap.add_argument("--lang", default="", choices=sorted(set(LANG_EXT) - {""}),
                     help="default snobol4 (blank); selects extension + attribute table + oracle")
    args = ap.parse_args()
    pkg_dir = Path(args.package_dir).resolve()
    if not pkg_dir.is_dir():
        h.refuse(f"not a directory: {pkg_dir}")
    build(pkg_dir, args.lang)


if __name__ == "__main__":
    main()
