#!/usr/bin/env python3
"""corpus_suite_harness.py -- the ONE Python 3 (stdlib-only) harness for corpus-suites-consolidation.

Converts a family directory of loose NNN_name.sno + NNN_name.ref pairs into two parallel suite
text files (family.sno / family.ref) and can run/validate either the loose files or the suite
files, keeping a PASS/FAIL/CRASH/HANG/UNPROVEN/SKIP verdict ladder distinct throughout (the
CRASH-collapsed-into-DIVERGE lesson from SCRIP 5ad95ab1 -- see
.github/FINDING-2026-08-24-hq_P-icon-bench-0-of-8-is-one-defect-suspend-procedures-get-no-activation-frame.md).

Format spec: .github postoffice task corpus-suites-consolidation.task.md.
  (A) ONE-LINE entry:  "<stmt1>;<stmt2>;...;* <name>"            family.sno line N
                        "<expected, \\n-escaped if multi-line>"    family.ref line N
  (B) MULTI-LINE (banner) entry, used when a join exceeds 200 chars or changes behaviour:
                        "*----...---- <seq> <name>"  (banner, exactly 80 chars, SNOBOL4 comment syntax)
                        <original statement lines, verbatim, one per physical line>
                        (block runs until the next banner or EOF)
                        Same banner + expected output VERBATIM (no escaping) in family.ref.

⭐ THE JOIN RULE (empirically derived+verified against scrip and the SPITBOL oracle -- SPITBOL manual
"each semicolon ... behaves like a new column one for the statement which follows"): a label must be
IMMEDIATELY adjacent to its preceding ';' (no space); an unlabeled statement needs a SPACE immediately
after ';' (mirrors blank-column-1 = no label); the very FIRST statement on the line needs a leading
space too, if unlabeled, since there is no preceding ';' to carry it. A bare goto-only continuation
line (e.g. a lone ":(END)" on its own physical line, common in this corpus for column alignment) is
MERGED into the immediately preceding statement rather than becoming its own ';'-joined entry --
control flow is identical either way (a goto-only statement does no work of its own), and joining it
standalone is what causes SPITBOL's "ERROR 214 bad label or misplaced continuation line".

Byte-equal-or-no-delete: every entry is validated by actually RUNNING both the original file and
the converted entry (both modes) and comparing full verdicts (not just stdout text) before it is
trusted; the whole point of the empirical validate+fallback loop is that this file does not need to
be a perfect SNOBOL4/SPITBOL grammar -- a wrong guess just falls back to the always-safe multi-line
block shape instead of silently producing a bad conversion.
"""
import argparse
import os
import re
import subprocess
import sys
import tempfile
from pathlib import Path

BANNER_WIDTH = 80
ONE_LINE_CAP = 200
BANNER_RE = re.compile(r"^\*-+ (?P<seq>\d+) (?P<name>\S+)$")


# ============================================================ paths / env ===
def resolve_paths():
    here = Path(__file__).resolve().parent            # SCRIP/scripts
    scrip_root = here.parent                           # SCRIP/
    s4e_home = Path(os.environ.get("S4E_HOME", str(scrip_root.parent)))
    return {
        "scrip_bin": Path(os.environ.get("SCRIP", str(scrip_root / "scrip"))),
        "rt_dir": Path(os.environ.get("RT_DIR", str(scrip_root / "out"))),
        "corpus": s4e_home / "corpus",
        "inc": Path(os.environ.get("INC", str(s4e_home / "corpus" / "include"))),
        "scrip_root": scrip_root,
        "timeout": float(os.environ.get("TIMEOUT", "10")),
        "stdbuf_bin": _which("stdbuf"),
    }


def _which(name):
    for d in os.environ.get("PATH", "").split(os.pathsep):
        p = Path(d) / name
        if p.is_file() and os.access(p, os.X_OK):
            return str(p)
    return None


def refuse(msg):
    print(f"⛔ REFUSING: {msg}", file=sys.stderr)
    sys.exit(3)


def check_scrip(paths):
    if not (paths["scrip_bin"].is_file() and os.access(paths["scrip_bin"], os.X_OK)):
        refuse(f"scrip is not built/executable at {paths['scrip_bin']}")


# ==================================================================== exec ===
class Verdict:
    __slots__ = ("kind", "stdout", "stderr", "returncode", "detail")

    def __init__(self, kind, stdout=b"", stderr=b"", returncode=None, detail=""):
        self.kind = kind  # PASS FAIL CRASH HANG UNPROVEN SKIP
        self.stdout = stdout
        self.stderr = stderr
        self.returncode = returncode
        self.detail = detail

    def text(self):
        return self.stdout.decode("utf-8", "replace")

    def behaviorally_equal(self, other):
        """Same shape of outcome -- used for old-vs-new byte-equal validation. CRASH must match
        the exact signal; HANG/UNPROVEN/SKIP just need the same kind; PASS/FAIL need matching text."""
        if self.kind != other.kind:
            return False
        if self.kind == "CRASH":
            return self.returncode == other.returncode
        if self.kind in ("PASS", "FAIL"):
            return self.text().rstrip("\n") == other.text().rstrip("\n")
        return True

    def __repr__(self):
        return f"Verdict({self.kind}, rc={self.returncode}, detail={self.detail!r})"


def _run_raw(argv, timeout, cwd=None, env=None):
    try:
        r = subprocess.run(argv, stdin=subprocess.DEVNULL, capture_output=True,
                            timeout=timeout, cwd=cwd, env=env)
    except subprocess.TimeoutExpired as e:
        return "HANG", (e.stdout or b""), (e.stderr or b""), None
    except FileNotFoundError as e:
        return "UNPROVEN", b"", str(e).encode(), None
    return "RAN", r.stdout, r.stderr, r.returncode


def classify(argv, timeout, expected_text, cwd=None, env=None):
    kind, out, err, rc = _run_raw(argv, timeout, cwd=cwd, env=env)
    if kind == "HANG":
        return Verdict("HANG", out, err, None, detail=f"exceeded {timeout}s")
    if kind == "UNPROVEN":
        return Verdict("UNPROVEN", out, err, None, detail=err.decode("utf-8", "replace"))
    if rc is not None and rc < 0:
        return Verdict("CRASH", out, err, rc, detail=f"signal {-rc}")
    got = out.decode("utf-8", "replace").rstrip("\n")
    exp = expected_text.rstrip("\n") if expected_text is not None else None
    if exp is not None and got == exp:
        return Verdict("PASS", out, err, rc)
    return Verdict("FAIL", out, err, rc, detail="output mismatch" if exp is not None else "no expected text")


def stdbuf_wrap(paths, argv):
    if paths["stdbuf_bin"]:
        return [paths["stdbuf_bin"], "-o0", "-e0"] + argv
    return argv


def run_m3(paths, sno_path, expected_text, timeout=None):
    timeout = timeout or paths["timeout"]
    argv = stdbuf_wrap(paths, [str(paths["scrip_bin"]), "--run", str(sno_path)])
    env = dict(os.environ, SNO_LIB=str(paths["inc"]))
    return classify(argv, timeout, expected_text, env=env)


def compile_m4(paths, sno_path, out_bin, tmp_dir):
    """Mirrors compile_mode4() in test_corpus_snobol4.sh exactly. Returns None on success, or a
    Verdict("SKIP", ...) describing where it failed."""
    s_path = tmp_dir / "p.s"
    env = dict(os.environ, SNO_LIB=str(paths["inc"]))
    with open(s_path, "wb") as f:
        r = subprocess.run([str(paths["scrip_bin"]), "--compile", str(sno_path)],
                            stdout=f, stderr=subprocess.DEVNULL, env=env)
    if r.returncode != 0:
        return Verdict("SKIP", detail="scrip --compile failed")
    o_path = tmp_dir / "p.o"
    r = subprocess.run(["gcc", "-c", str(s_path), "-o", str(o_path)],
                        stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, cwd=str(paths["scrip_root"]))
    if r.returncode != 0:
        return Verdict("SKIP", detail="gcc -c failed")
    rt_dir = str(paths["rt_dir"])
    r = subprocess.run(["gcc", str(o_path), "-L", rt_dir, "-lscrip_rt", "-lm",
                         "-Wl,-rpath," + rt_dir, "-o", str(out_bin)],
                        stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, cwd=str(paths["scrip_root"]))
    if r.returncode != 0:
        return Verdict("SKIP", detail="gcc link failed")
    return None


def run_m4(paths, sno_path, expected_text, tmp_dir, timeout=None):
    timeout = timeout or paths["timeout"]
    if not (paths["rt_dir"] / "libscrip_rt.so").is_file():
        return Verdict("SKIP", detail="libscrip_rt.so not built")
    out_bin = tmp_dir / "t.bin"
    skip = compile_m4(paths, sno_path, out_bin, tmp_dir)
    if skip is not None:
        return skip
    argv = stdbuf_wrap(paths, [str(out_bin)])
    env = dict(os.environ, SNO_LIB=str(paths["inc"]))
    return classify(argv, timeout, expected_text, env=env)


# ============================================================= discovery ===
def discover_pairs(family_dir):
    """(.sno, .ref) pairs, sorted by filename. A .sno without a matching .ref is skipped (mirrors
    test_corpus_snobol4.sh's own [ ! -f "$ref" ] && continue). Any file whose extension is not
    .sno/.ref (dead-backend litter: .il/.j/.js/.wat/.s/.asm.ref etc) is never matched by this glob
    pairing and is silently ignored, which is the point -- it is not a test."""
    family_dir = Path(family_dir)
    pairs = []
    for sno in sorted(family_dir.glob("*.sno")):
        ref = sno.with_suffix(".ref")
        if ref.is_file():
            pairs.append((sno, ref))
    return pairs


# ============================================================= statement parse ===
def parse_statements(sno_text):
    """Original source -> list of {"labeled": bool, "raw": text} statements. Drops column-1 '*'
    comments and blank lines; merges a bare goto-only continuation line (e.g. a lone ':(END)')
    into the immediately preceding statement instead of treating it as its own entry."""
    statements = []
    for line in sno_text.splitlines():
        if not line.strip():
            continue
        if line[:1] == "*":
            continue
        stripped = line.strip()
        if stripped.startswith(":") and statements:
            statements[-1]["raw"] = statements[-1]["raw"] + " " + stripped
            continue
        labeled = not line[:1].isspace()
        statements.append({"labeled": labeled, "raw": stripped})
    return statements


def join_one_line(statements):
    """The empirically-verified join: no space before a label, a space before an unlabeled
    statement (including the very first, which has no preceding ';' to carry it)."""
    pieces = []
    for i, s in enumerate(statements):
        text = re.sub(r"\s+", " ", s["raw"]).strip()
        sep = "" if i == 0 else ";"
        pad = "" if s["labeled"] else " "
        pieces.append(sep + pad + text)
    return "".join(pieces)


def multiline_block(statements):
    """The always-safe fallback: original per-statement text, one per physical line, comments and
    blank lines already stripped, continuations already merged -- verified byte-identical to the
    unmodified original file's behaviour (see corpus-suites-consolidation session notes). Column-1
    still matters per physical line here (same rule as the one-line join): a labeled statement's
    label must start at true column 1, and an unlabeled statement must NOT -- parse_statements()
    stored both as .strip()'d text, so indentation must be restored for unlabeled statements."""
    return [s["raw"] if s["labeled"] else ("        " + s["raw"]) for s in statements]


def make_banner(seq, name):
    suffix = f"{seq} {name}"
    dash_count = max(1, BANNER_WIDTH - 1 - 1 - len(suffix))
    banner = "*" + ("-" * dash_count) + " " + suffix
    return banner


# ============================================================= conversion ===
class Entry:
    def __init__(self, kind, seq, name, sno_lines, ref_text_or_lines):
        self.kind = kind          # "line" or "block"
        self.seq = seq
        self.name = name
        self.sno_lines = sno_lines        # list[str]: 1 line for "line", N for "block"
        self.ref = ref_text_or_lines      # str for "line" (unescaped), list[str] for "block"


def convert_one(paths, sno_path, ref_path, seq, tmp_root, modes):
    """Returns (Entry, report_dict). report_dict always has "ok": bool and "reason": str."""
    name = sno_path.stem
    original_text = sno_path.read_text()
    expected_text = ref_path.read_text()
    statements = parse_statements(original_text)

    orig_verdicts = run_all_modes(paths, sno_path, expected_text, tmp_root, modes)
    if any(v.kind != "PASS" for v in orig_verdicts.values()):
        return None, {"ok": False, "reason": f"original file itself is not green: {orig_verdicts}"}

    # --- attempt (A): one-line join ---
    joined = join_one_line(statements)
    one_line = joined + f";* {name}"
    if len(one_line) <= ONE_LINE_CAP:
        with tempfile.TemporaryDirectory(dir=tmp_root) as td:
            cand = Path(td) / "cand.sno"
            cand.write_text(one_line + "\n")
            cand_verdicts = run_all_modes(paths, cand, expected_text, Path(td), modes)
        if all(cand_verdicts[m].behaviorally_equal(orig_verdicts[m]) for m in modes):
            entry = Entry("line", seq, name, [one_line], expected_text.rstrip("\n"))
            return entry, {"ok": True, "reason": "one-line", "len": len(one_line)}

    # --- fallback (B): multi-line banner block ---
    block_lines = multiline_block(statements)
    with tempfile.TemporaryDirectory(dir=tmp_root) as td:
        cand = Path(td) / "cand.sno"
        cand.write_text("\n".join(block_lines) + "\n")
        cand_verdicts = run_all_modes(paths, cand, expected_text, Path(td), modes)
    if all(cand_verdicts[m].behaviorally_equal(orig_verdicts[m]) for m in modes):
        entry = Entry("block", seq, name, block_lines, expected_text.rstrip("\n").splitlines())
        return entry, {"ok": True, "reason": "multi-line-block"}

    return None, {"ok": False, "reason": f"NEITHER form reproduced the original's behavior: "
                                          f"one-line={cand_verdicts} orig={orig_verdicts}"}


def run_all_modes(paths, sno_path, expected_text, tmp_root, modes):
    out = {}
    if "m3" in modes:
        out["m3"] = run_m3(paths, sno_path, expected_text)
    if "m4" in modes:
        with tempfile.TemporaryDirectory(dir=tmp_root) as td:
            out["m4"] = run_m4(paths, sno_path, expected_text, Path(td))
    return out


# ========================================================== suite writer ===
def write_suite(entries, out_sno, out_ref):
    sno_lines, ref_lines = [], []
    for e in entries:
        if e.kind == "line":
            sno_lines.append(e.sno_lines[0])
            ref_lines.append(e.ref.replace("\n", "\\n"))
        else:
            banner = make_banner(e.seq, e.name)
            sno_lines.append(banner)
            sno_lines.extend(e.sno_lines)
            ref_lines.append(banner)
            ref_lines.extend(e.ref)
    Path(out_sno).write_text("\n".join(sno_lines) + "\n")
    Path(out_ref).write_text("\n".join(ref_lines) + "\n")


# ========================================================== suite reader ===
ONE_LINE_TAG_RE = re.compile(r";\* (\S+)$")


def _is_entry_start(line):
    """True at a banner (block start) or a one-line entry (which always ends in the mandatory ;* tag --
    format spec: 'the tag is standard on every line, not optional'). A block's own body lines are raw,
    unjoined original statement text and do not end this way in practice, so this is what lets the block
    reader below know when to STOP without needing a banner on the far side."""
    if line[:1] == "*" and BANNER_RE.match(line):
        return True
    return bool(ONE_LINE_TAG_RE.search(line))


def read_suite(sno_path, ref_path):
    """⛔ FOUND AND FIXED (corpus-suites-consolidation, gc family): a block used to be read as running to
    the NEXT banner -- correct only when the next entry is ALSO a block. A block immediately followed by
    one or more one-line entries (no banner of their own, e.g. gc.sno's 210_gc_deep_nesting -> 211/212/213
    one-liners -> 214's banner) had its .sno-side reader swallow those one-liners' raw text as more of the
    block's body (fixed by stopping at _is_entry_start, not just a banner) -- but that alone still leaves
    the .ref side ambiguous, because a one-liner's ref line carries no marker of its own (only the .sno
    side tags it). So the .ref side is read in a SEPARATE pass once the .sno side has produced the
    ordered, correctly-kinded item list: a block's ref segment runs from its banner to the next ref banner
    (or EOF) exactly as before, but one ref line is peeled off the END of that segment for each consecutive
    one-line item known (from the .sno pass) to follow it before the next block -- order preserved, since
    the .ref file's line order for those trailing one-liners is unchanged from write_suite's -- and
    whatever remains is the block's own output. A pure block-to-block or line-to-line run degenerates to
    the original (correct) behaviour: zero trailing one-liners peeled, block gets the whole segment.
    Verified against both existing suites: patterns.sno (all-block) and strings.sno (all ten one-liners
    first, then all three blocks) never exercise the peel path and re-read byte-identically to before this
    fix; gc.sno (the interleaved case) is what exposed it."""
    sno_lines = Path(sno_path).read_text().splitlines()
    ref_lines = Path(ref_path).read_text().splitlines()

    items = []
    si = 0
    while si < len(sno_lines):
        line = sno_lines[si]
        m = BANNER_RE.match(line) if line[:1] == "*" else None
        if m:
            banner_line, name = line, m.group("name")
            si += 1
            body = []
            while si < len(sno_lines) and not _is_entry_start(sno_lines[si]):
                body.append(sno_lines[si])
                si += 1
            items.append(("block", name, banner_line, body))
        else:
            tag_m = ONE_LINE_TAG_RE.search(line)
            name = tag_m.group(1) if tag_m else f"seq{len(items) + 1}"
            items.append(("line", name, line, None))
            si += 1

    entries = []
    ri = 0
    seq = 0
    i = 0
    while i < len(items):
        kind, name, a, b = items[i]
        seq += 1
        if kind == "line":
            if ri >= len(ref_lines):
                raise ValueError(f"family.ref is shorter than family.sno at seq {seq} ({name})")
            entries.append(Entry("line", seq, name, [a], ref_lines[ri].replace("\\n", "\n")))
            ri += 1
            i += 1
            continue
        banner_line, block_sno = a, b
        if ri >= len(ref_lines) or ref_lines[ri] != banner_line:
            raise ValueError(f"family.ref banner mismatch at seq {seq}: sno={banner_line!r} ref={ref_lines[ri] if ri < len(ref_lines) else None!r}")
        ri += 1
        seg_start = ri
        while ri < len(ref_lines) and not (ref_lines[ri][:1] == "*" and BANNER_RE.match(ref_lines[ri])):
            ri += 1
        seg = ref_lines[seg_start:ri]
        trailing = 0
        j = i + 1
        while j < len(items) and items[j][0] == "line":
            trailing += 1
            j += 1
        if trailing > len(seg):
            raise ValueError(f"family.ref segment after seq {seq} ({name}) has {len(seg)} line(s), too short to hold {trailing} trailing one-line entrie(s)")
        split = len(seg) - trailing
        entries.append(Entry("block", seq, name, block_sno, seg[:split]))
        for k, ref_line in enumerate(seg[split:]):
            _, lname, lraw, _ = items[i + 1 + k]
            seq += 1
            entries.append(Entry("line", seq, lname, [lraw], ref_line.replace("\\n", "\n")))
        i = j
    return entries


def run_suite_entry(paths, entry, tmp_root, modes):
    with tempfile.TemporaryDirectory(dir=tmp_root) as td:
        cand = Path(td) / f"{entry.name}.sno"
        if entry.kind == "line":
            cand.write_text(entry.sno_lines[0] + "\n")
            expected = entry.ref
        else:
            cand.write_text("\n".join(entry.sno_lines) + "\n")
            expected = "\n".join(entry.ref)
        return run_all_modes(paths, cand, expected, Path(td), modes)


# ================================================================== CLI ===
def cmd_convert(args):
    paths = resolve_paths()
    check_scrip(paths)
    modes = args.modes.split(",")
    family_dir = Path(args.family_dir)
    pairs = discover_pairs(family_dir)
    if not pairs:
        refuse(f"no .sno/.ref pairs discovered under {family_dir}")
    print(f"discovered {len(pairs)} pairs in {family_dir}", file=sys.stderr)

    # ⛔ DELIBERATE SKIP, NEVER SILENT DROP (Snocone removed-syntax precedent, task ## NEXT / ## SNOCONE):
    # a stem named here is loudly excluded from THIS conversion run and left as a loose file pair --
    # not deleted, not converted. Reason is mandatory and printed so a skip can never be mistaken for
    # a file the harness simply failed to notice.
    skip_names = set(n for n in (args.skip.split(",") if args.skip else []) if n)
    if skip_names:
        skipped = [p for p in pairs if p[0].stem in skip_names]
        pairs = [p for p in pairs if p[0].stem not in skip_names]
        for sno, ref in skipped:
            print(f"⛔ SKIPPING (deliberate): {sno.stem} -- {args.skip_reason or 'no reason given'}", file=sys.stderr)
        unmatched = skip_names - {p[0].stem for p in skipped}
        if unmatched:
            refuse(f"--skip named stem(s) not found in {family_dir}: {sorted(unmatched)}")
        if not pairs:
            refuse(f"--skip excluded every discovered pair under {family_dir} -- nothing left to convert")

    entries = []
    failures = []
    tmp_root = Path(tempfile.mkdtemp(prefix="csh_"))
    try:
        for seq, (sno, ref) in enumerate(pairs, start=1):
            entry, report = convert_one(paths, sno, ref, seq, tmp_root, modes)
            status = "OK" if report["ok"] else "FAIL"
            print(f"[{seq}/{len(pairs)}] {sno.stem}: {status} ({report['reason'][:80]})", file=sys.stderr)
            if entry is not None:
                entries.append(entry)
            else:
                failures.append((sno.stem, report["reason"]))
    finally:
        import shutil
        shutil.rmtree(tmp_root, ignore_errors=True)

    if failures:
        print(f"⛔ {len(failures)} entries FAILED byte-equal validation -- NOTHING WRITTEN, NOTHING DELETED:", file=sys.stderr)
        for name, reason in failures:
            print(f"   {name}: {reason}", file=sys.stderr)
        sys.exit(1)

    one_line_n = sum(1 for e in entries if e.kind == "line")
    block_n = sum(1 for e in entries if e.kind == "block")
    write_suite(entries, args.out_sno, args.out_ref)
    print(f"✅ wrote {args.out_sno} / {args.out_ref}: {len(entries)} entries "
          f"({one_line_n} one-line, {block_n} multi-line-block)", file=sys.stderr)

    # close the loop: re-read the WRITTEN files and re-validate every entry against the originals
    reread = read_suite(args.out_sno, args.out_ref)
    tmp_root = Path(tempfile.mkdtemp(prefix="csh_verify_"))
    mismatches = []
    try:
        for (sno, ref), written in zip(pairs, reread):
            orig_verdicts = run_all_modes(paths, sno, ref.read_text(), tmp_root, modes)
            suite_verdicts = run_suite_entry(paths, written, tmp_root, modes)
            if not all(suite_verdicts[m].behaviorally_equal(orig_verdicts[m]) for m in modes):
                mismatches.append((sno.stem, orig_verdicts, suite_verdicts))
    finally:
        import shutil
        shutil.rmtree(tmp_root, ignore_errors=True)

    if mismatches:
        print(f"⛔ ON-DISK RE-VALIDATION FAILED for {len(mismatches)} entries -- the WRITTEN suite "
              f"files diverge from a fresh re-read/re-run. DO NOT delete originals.", file=sys.stderr)
        for name, o, s in mismatches:
            print(f"   {name}: orig={o} suite={s}", file=sys.stderr)
        sys.exit(1)

    print(f"✅ ON-DISK RE-VALIDATION PASSED: all {len(entries)} entries byte-equal, both directions, "
          f"modes={modes}. Safe to delete the {len(pairs)*2} original files.", file=sys.stderr)
    sys.exit(0)


def cmd_run(args):
    paths = resolve_paths()
    check_scrip(paths)
    modes = args.modes.split(",")
    entries = read_suite(args.sno, args.ref)
    counts = {m: {"PASS": 0, "FAIL": 0, "CRASH": 0, "HANG": 0, "UNPROVEN": 0, "SKIP": 0} for m in modes}
    tmp_root = Path(tempfile.mkdtemp(prefix="csh_run_"))
    fails = []
    try:
        for e in entries:
            verdicts = run_suite_entry(paths, e, tmp_root, modes)
            for m in modes:
                counts[m][verdicts[m].kind] += 1
                if verdicts[m].kind != "PASS":
                    fails.append((e.name, m, verdicts[m]))
    finally:
        import shutil
        shutil.rmtree(tmp_root, ignore_errors=True)

    family = Path(args.sno).stem
    fields = [f"family={family}", f"total={len(entries)}"]
    for m in modes:
        c = counts[m]
        fields.append(f"{m}_pass={c['PASS']} {m}_fail={c['FAIL']} {m}_crash={c['CRASH']} "
                       f"{m}_hang={c['HANG']} {m}_unproven={c['UNPROVEN']} {m}_skip={c['SKIP']}")
    print("SUITE_BOARD " + " ".join(fields))
    for name, m, v in fails[:40]:
        print(f"  {v.kind} {m} {name}: {v.detail}", file=sys.stderr)
    sys.exit(0 if not fails else 1)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)

    c = sub.add_parser("convert", help="convert a loose-file family into suite .sno/.ref, validating byte-equal before writing")
    c.add_argument("family_dir")
    c.add_argument("out_sno")
    c.add_argument("out_ref")
    c.add_argument("--modes", default="m3,m4")
    c.add_argument("--skip", default="", help="comma-separated .sno stems to deliberately exclude from this run (left as loose files, never deleted)")
    c.add_argument("--skip-reason", default="", help="mandatory-in-spirit reason printed for every --skip name")
    c.set_defaults(func=cmd_convert)

    r = sub.add_parser("run", help="run a suite .sno/.ref pair and print PASS/FAIL/CRASH/HANG/UNPROVEN/SKIP counts")
    r.add_argument("sno")
    r.add_argument("ref")
    r.add_argument("--modes", default="m3,m4")
    r.set_defaults(func=cmd_run)

    args = ap.parse_args()
    args.func(args)


if __name__ == "__main__":
    main()
