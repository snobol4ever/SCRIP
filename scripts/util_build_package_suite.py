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
  1. a `-INCLUDE "X"` naming a file not vendored beside it (a corpus completeness gap, not gradable here)
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


def find_include_gap(src_text, pkg_dir):
    """The -INCLUDE target this source names, if it names one AND that target is not vendored beside
    it -- else None. Mirrors test_snobol4_aisnobol_suite.sh's own check (already measured: SNOCORE.sno
    exists nowhere in corpus or /home/resources)."""
    mo = _INCLUDE_RE.search(src_text)
    if not mo:
        return None
    inc = mo.group(1)
    return None if (pkg_dir / inc).is_file() else inc


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
    for ext in ("stdin", "IN", "in", "input", "Input"):
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
        gap = find_include_gap(text, pkg_dir)
        if gap:
            excluded.append((name, f"missing corpus dependency: -INCLUDE {gap!r} not vendored anywhere in corpus"))
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
        h.refuse(f"{pkg_dir}: zero absorbable entries of {tot} shipped -- a container that grades nothing is not a container")

    wrote_in = h.write_suite(entries, str(out_sno), str(out_ref), out_in=str(out_in))
    if not wrote_in and out_in.exists():
        out_in.unlink()

    wr_lines = [f"{e.name}\t{e.want_rc}" for e in entries if e.want_rc]
    if wr_lines:
        out_wantrc.write_text("\n".join(wr_lines) + "\n")
    elif out_wantrc.exists():
        out_wantrc.unlink()

    table_lang = lang or "snobol4"
    cols, _ = m.LANG_TABLES[table_lang]
    with open(out_csv, "w", newline="") as f:
        w = csv.writer(f, lineterminator="\n")
        w.writerow(["rank", "entry", "origin", "package", "n_lines", "stdin", "want_rc"] + [c for c, _fn in cols])
        for e in entries:
            joined = "\n".join(e.sno_lines)
            flags_row = m.attrs_for_text(joined, table_lang)
            w.writerow([e.seq, e.name, f"{pkg_dir.name}__{e.name}", pkg_dir.name, len(e.sno_lines),
                        1 if e.stdin else 0, e.want_rc] + [flags_row[c] for c, _fn in cols])

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
