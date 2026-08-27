#!/usr/bin/env python3
"""corpus_suite_harness.py -- the ONE Python 3 (stdlib-only) harness for corpus-suites-consolidation.

Converts a family directory of loose NNN_name.sno + NNN_name.ref pairs into two parallel suite
text files (family.sno / family.ref) and can run/validate either the loose files or the suite
files, keeping a PASS/FAIL/CRASH/HANG/UNPROVEN/SKIP verdict ladder distinct throughout (the
CRASH-collapsed-into-DIVERGE lesson from SCRIP 5ad95ab1 -- see
.github/FINDING-2026-08-24-hq_P-icon-bench-0-of-8-is-one-defect-suspend-procedures-get-no-activation-frame.md).

⛔⭐ A SUITE FILE IS NEVER RUN WHOLE. IT IS NOT A PROGRAM -- IT IS A CONTAINER OF PROGRAMS.
Every entry is extracted and run ALONE, in its own fresh temp dir, by run_suite_entry(). Under format
(B) each block is an independently complete program carrying its OWN `END` statement, and entries
freely reuse label names (`e001`, `e002`, ...) because they never share a namespace. So compiling
`family.sno` directly -- `./scrip family.sno`, or `sbl -bf family.sno` -- is a CATEGORY ERROR, and it
fails in two ways that look exactly like real defects:
    * scrip  -> a pile of "duplicate label" parse errors (entries' labels collide)
    * sbl -bf -> runs the FIRST entry only, then stops at its embedded `END`, so a 34-line .ref
                 appears to "mismatch" a 1-line oracle result
⛔ BOTH ARE ARTIFACTS OF THE WRONG INVOCATION, NOT FINDINGS. Measured 2026-08-27 (hq_C): I read
exactly those two symptoms as a broken consolidation and escalated it as a live coverage regression,
telling the seat converting the family to stop work. It cost them a session interruption and cost me
two false reports. ⭐ THE CHECK THAT WOULD HAVE CAUGHT ME IN ONE COMMAND is the one that seat08 ran
and I did not -- A CONTROL ARM: run the SAME whole-file compile on `patterns.sno`, the pilot family
that has been green for days. It produces the IDENTICAL failure shape. A symptom that reproduces on
a known-good sibling is a property of the format, never a defect in the file under suspicion.
To grade a suite for real, use the mechanism the board uses:
    python3 scripts/corpus_suite_harness.py run <family>.sno <family>.ref --modes m3,m4

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

# ============================================================ language configs ===
# Per-language config for the format-(B)-ONLY (banner-block) conversion path -- convert_blocks
# below. Format (A)'s one-line statement join stays SNOBOL4-specific (parse_statements /
# join_one_line / _has_goto_field): a parser-ladder family never attempts it (format B is the
# task spec's design for these families, not a join-failure fallback), so a new dialect only
# needs its source extension, its comment syntax for the banner (open/close -- close is "" for a
# line-comment language), and its grading mode. Add an entry for a language only once you have
# actually run --dump-ast (or whatever mode) against a real sample and confirmed the banner
# round-trips -- do not add a config for a language nobody has exercised. Snocone remains
# unconfigured as of this comment (suite-harness-lang-configs' remaining scope).
LANG_CONFIGS = {
    "raku": {"ext": ".raku", "comment_open": "#", "comment_close": "", "modes": "ast"},
    "rebus": {"ext": ".reb", "comment_open": "#", "comment_close": "", "modes": "ast"},
    "pascal": {"ext": ".pas", "comment_open": "{", "comment_close": "}", "modes": "m3,m4"},
    "prolog": {"ext": ".pl", "comment_open": "%", "comment_close": "", "modes": "ast"},
}


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


def resolve_oracle_bin(paths):
    """Resolve the correctness oracle's binary+flags via lib_oracle_flags.sh -- the ONE authority
    (RULES.md Oracles section): never hand-assemble an oracle path/flag pair in a second copy.
    Shells out to the real bash accessors (sbl_correctness_bin, sbl_lang_flags) so a future change
    to the shared oracle's location or capability check is picked up automatically, same as every
    bash caller gets. Used only by cmd_capture_oracle_refs -- no other command in this file talks
    to SPITBOL at all, since every other family already ships a committed .ref."""
    lib = paths["scrip_root"] / "scripts" / "lib_oracle_flags.sh"
    if not lib.is_file():
        refuse(f"lib_oracle_flags.sh missing at {lib} -- the ONE oracle-flag authority (RULES.md)")
    r = subprocess.run(["bash", "-c", f". '{lib}' && sbl_correctness_bin && sbl_lang_flags"],
                        capture_output=True, text=True)
    if r.returncode != 0:
        refuse(f"lib_oracle_flags.sh refused (sbl_correctness_bin/sbl_lang_flags): {r.stderr.strip()}")
    lines = r.stdout.strip("\n").splitlines()
    if len(lines) != 2 or not lines[0] or not lines[1]:
        refuse(f"unexpected output from sbl_correctness_bin/sbl_lang_flags: {r.stdout!r}")
    return lines[0], lines[1]  # oracle binary path, language flags (e.g. "-bf")


def run_oracle(oracle_bin, flags, sno_path, timeout):
    """One live oracle invocation, `< /dev/null` like every other scrip/oracle call in this file
    and in test_one_witness.sh (whose exact contract this mirrors: stdout text AND returncode both
    matter -- a witness testing a deliberate error exit is not 'wrong' for exiting non-zero, it is
    wrong only if scrip's rc/text pair disagrees with the oracle's)."""
    argv = [oracle_bin] + flags.split() + [str(sno_path)]
    kind, out, _err, rc = _run_raw(argv, timeout)
    return out.decode("utf-8", "replace").rstrip("\n"), rc, kind


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


def run_ast(paths, src_path, expected_text, timeout=None):
    """Grading mode for parser-ladder families: `scrip --dump-ast`, diffed as text -- same
    classify() used by run_m3/run_m4, just a different argv. No compile/link step, so it is fast
    and language-agnostic (dispatch is by src_path's own extension, same as scrip --run/--compile)."""
    timeout = timeout or paths["timeout"]
    argv = stdbuf_wrap(paths, [str(paths["scrip_bin"]), "--dump-ast", str(src_path)])
    return classify(argv, timeout, expected_text)


# ============================================================= discovery ===
def discover_pairs(family_dir, ext=".sno"):
    """(src, .ref) pairs, sorted by filename. A src file without a matching .ref is skipped
    (mirrors test_corpus_snobol4.sh's own [ ! -f "$ref" ] && continue). Any file whose extension
    is not `ext`/.ref (dead-backend litter: .il/.j/.js/.wat/.s/.asm.ref etc) is never matched by
    this glob pairing and is silently ignored, which is the point -- it is not a test."""
    family_dir = Path(family_dir)
    pairs = []
    for src in sorted(family_dir.glob(f"*{ext}")):
        ref = src.with_suffix(".ref")
        if ref.is_file():
            pairs.append((src, ref))
    return pairs


# ============================================================= statement parse ===
def _has_goto_field(raw):
    """Does this statement's text already carry a :S()/:F()/bare :() goto field? A bare-colon
    continuation line may only merge into a statement that has NONE -- merging onto one that
    already has its own conditional goto (e.g. 'GT(N, 5) :S(DONE)' followed by a fall-through-only
    ':(LOOP)') produces a single statement with two conflicting goto specs, not the original's
    real semantics (test-then-conditional-jump, THEN a separate unconditional-jump statement for
    the fall-through path). Ignores colons inside quoted string literals."""
    in_str = None
    for ch in raw:
        if in_str:
            if ch == in_str:
                in_str = None
        elif ch in "'\"":
            in_str = ch
        elif ch == ":":
            return True
    return False


def parse_statements(sno_text):
    """Original source -> list of {"labeled": bool, "raw": text} statements. Drops column-1 '*'
    comments and blank lines; merges a bare goto-only continuation line (e.g. a lone ':(END)')
    into the immediately preceding statement instead of treating it as its own entry -- but only
    when that statement has no goto field of its own (see _has_goto_field); otherwise the bare
    goto is a genuine separate statement (the fall-through path after a failed conditional test)
    and must stay its own entry."""
    statements = []
    for line in sno_text.splitlines():
        if not line.strip():
            continue
        if line[:1] == "*":
            continue
        stripped = line.strip()
        if stripped.startswith(":") and statements and not _has_goto_field(statements[-1]["raw"]):
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


def make_banner_cfg(seq, name, comment_open, comment_close):
    """Generalized make_banner() for LANG_CONFIGS dialects: comment_open="*"/comment_close=""
    reproduces make_banner() exactly (verified: same dash_count formula, open+dashes+' '+suffix).
    A non-empty comment_close (e.g. Snocone's " */") is pinned at the right margin, same as the
    open side's dash-fill -- open + dashes + ' ' + suffix + close, total BANNER_WIDTH."""
    suffix = f"{seq} {name}"
    fixed = len(comment_open) + 1 + len(suffix) + len(comment_close)
    dash_count = max(1, BANNER_WIDTH - fixed)
    return comment_open + ("-" * dash_count) + " " + suffix + comment_close


def banner_re_for(comment_open, comment_close):
    close = comment_close.strip()
    close_pat = (r"\s*" + re.escape(close)) if close else ""
    return re.compile(r"^" + re.escape(comment_open) + r"-+ (?P<seq>\d+) (?P<name>\S+)" + close_pat + r"$")


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
    if "ast" in modes:
        out["ast"] = run_ast(paths, sno_path, expected_text)
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


def write_block_suite(entries, out_src, out_ref, comment_open, comment_close):
    """write_suite() for a format-(B)-ONLY family: every entry is ALWAYS a banner block (no
    one-line join is ever attempted -- parser-ladder families are format-B by task-spec design,
    not by join-failure fallback), and the banner uses the dialect's own comment syntax."""
    src_lines, ref_lines = [], []
    for e in entries:
        banner = make_banner_cfg(e.seq, e.name, comment_open, comment_close)
        src_lines.append(banner)
        src_lines.extend(e.sno_lines)
        ref_lines.append(banner)
        ref_lines.extend(e.ref)
    Path(out_src).write_text("\n".join(src_lines) + "\n")
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


def read_block_suite(src_path, ref_path, banner_re):
    """read_suite() for a format-(B)-ONLY family: every entry is a banner-delimited block, so
    there is no one-line/block interleaving to detect (unlike read_suite() above, which also
    carries SNOBOL4's format-A one-line entries and BANNER_RE). Written separately rather than
    parameterizing read_suite() itself, to avoid coupling a second dialect's reading to
    ONE_LINE_TAG_RE / _is_entry_start, which are SNOBOL4-format-A-specific and meaningless here."""
    src_lines = Path(src_path).read_text().splitlines()
    ref_lines = Path(ref_path).read_text().splitlines()
    entries = []
    si = ri = seq = 0
    while si < len(src_lines):
        m = banner_re.match(src_lines[si])
        if not m:
            raise ValueError(f"expected a banner at {src_path} line {si + 1}: {src_lines[si]!r}")
        banner_line, name = src_lines[si], m.group("name")
        si += 1
        body = []
        while si < len(src_lines) and not banner_re.match(src_lines[si]):
            body.append(src_lines[si])
            si += 1
        if ri >= len(ref_lines) or ref_lines[ri] != banner_line:
            got = ref_lines[ri] if ri < len(ref_lines) else None
            raise ValueError(f"{ref_path} banner mismatch at seq {seq + 1} ({name}): sno={banner_line!r} ref={got!r}")
        ri += 1
        seg_start = ri
        while ri < len(ref_lines) and not banner_re.match(ref_lines[ri]):
            ri += 1
        seq += 1
        entries.append(Entry("block", seq, name, body, ref_lines[seg_start:ri]))
    return entries


def run_suite_entry(paths, entry, tmp_root, modes, ext=".sno"):
    with tempfile.TemporaryDirectory(dir=tmp_root) as td:
        cand = Path(td) / f"{entry.name}{ext}"
        if entry.kind == "line":
            cand.write_text(entry.sno_lines[0] + "\n")
            expected = entry.ref
        else:
            cand.write_text("\n".join(entry.sno_lines) + "\n")
            expected = "\n".join(entry.ref)
        return run_all_modes(paths, cand, expected, Path(td), modes)


# ================================================================== CLI ===
def cmd_capture_oracle_refs(args):
    """For a family with NO committed .ref files at all (oracle-graded loose files, e.g.
    probe/conformance -- see corpus-suites-consolidation task, probe-consolidate-conformance):
    synthesizes a .ref for every .sno whose CURRENT m3 AND m4 output+returncode both agree with a
    FRESH live run of the correctness oracle (sbl -bf), mirroring test_one_witness.sh's exact
    contract. Writes nothing for a stem that disagrees, that already has a .ref (unless --force),
    or where the oracle itself doesn't run cleanly -- those stay exactly as they are on disk, never
    touched. This is a ONE-TIME bootstrap step: once a .ref exists, the family behaves exactly like
    every other SNOBOL4 suite family (cmd_convert reads a static .ref, same as always) -- grading
    never re-invokes the oracle after this point, matching how every other family's .ref is a frozen
    snapshot, not a live re-check on every run."""
    paths = resolve_paths()
    check_scrip(paths)
    oracle_bin, flags = resolve_oracle_bin(paths)
    print(f"oracle: {oracle_bin} {flags}", file=sys.stderr)
    modes = args.modes.split(",")
    family_dir = Path(args.family_dir)
    srcs = sorted(family_dir.glob("*.sno"))
    if not srcs:
        refuse(f"no .sno files found under {family_dir}")

    green, red = [], []
    for i, src in enumerate(srcs, 1):
        ref_path = src.with_suffix(".ref")
        if ref_path.is_file() and not args.force:
            print(f"[{i}/{len(srcs)}] {src.stem}: SKIP (already has a .ref)", file=sys.stderr)
            continue
        ora_text, ora_rc, ora_kind = run_oracle(oracle_bin, flags, src, paths["timeout"])
        if ora_kind != "RAN":
            red.append((src.stem, f"oracle itself {ora_kind}"))
            print(f"[{i}/{len(srcs)}] {src.stem}: RED (oracle {ora_kind})", file=sys.stderr)
            continue
        agreements = []
        all_agree = True
        for m in modes:
            if m == "m3":
                v = run_m3(paths, src, ora_text)
            elif m == "m4":
                with tempfile.TemporaryDirectory() as td:
                    v = run_m4(paths, src, ora_text, Path(td))
            else:
                refuse(f"unknown mode {m!r} (this command supports m3/m4 only, never ast)")
            agree = (v.kind == "PASS") and (v.returncode == ora_rc)
            agreements.append(f"{m}={'AGREE' if agree else f'{v.kind}(rc={v.returncode} vs oracle {ora_rc})'}")
            all_agree = all_agree and agree
        if all_agree:
            ref_path.write_text(ora_text + "\n")
            green.append(src.stem)
            print(f"[{i}/{len(srcs)}] {src.stem}: GREEN ({' '.join(agreements)}) -- .ref written", file=sys.stderr)
        else:
            red.append((src.stem, " ".join(agreements)))
            print(f"[{i}/{len(srcs)}] {src.stem}: RED ({' '.join(agreements)})", file=sys.stderr)

    print(f"\n{len(green)} green (.ref written, ready for `convert`) -- {len(red)} red (untouched, no .ref):",
          file=sys.stderr)
    for name, reason in red:
        print(f"   RED {name}: {reason}", file=sys.stderr)
    sys.exit(0)


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


def cmd_convert_blocks(args):
    """convert, but for a LANG_CONFIGS dialect and format-(B)-ONLY: never attempts a one-line
    join (parser-ladder families are format-B by task-spec design, not by join-failure fallback).
    Same byte-equal-or-no-delete law as cmd_convert: after --skip filtering, ANY remaining
    original that is not green refuses the WHOLE run, nothing written/deleted -- deliberately NOT
    an auto-skip-if-red behavior, so every excluded fixture is a named, reasoned --skip, same
    discipline as the SNOBOL4 side's stdin-bearing-tests and Snocone-removed-syntax precedents."""
    paths = resolve_paths()
    check_scrip(paths)
    if args.lang not in LANG_CONFIGS:
        refuse(f"unknown --lang {args.lang!r} -- known: {sorted(LANG_CONFIGS)}")
    cfg = LANG_CONFIGS[args.lang]
    ext, comment_open, comment_close = cfg["ext"], cfg["comment_open"], cfg["comment_close"]
    modes = (args.modes or cfg["modes"]).split(",")
    family_dir = Path(args.family_dir)
    pairs = discover_pairs(family_dir, ext=ext)
    if not pairs:
        refuse(f"no {ext}/.ref pairs discovered under {family_dir}")
    print(f"discovered {len(pairs)} pairs in {family_dir}", file=sys.stderr)

    skip_names = set(n for n in (args.skip.split(",") if args.skip else []) if n)
    if skip_names:
        skipped = [p for p in pairs if p[0].stem in skip_names]
        pairs = [p for p in pairs if p[0].stem not in skip_names]
        for src, ref in skipped:
            print(f"⛔ SKIPPING (deliberate): {src.stem} -- {args.skip_reason or 'no reason given'}", file=sys.stderr)
        unmatched = skip_names - {p[0].stem for p in skipped}
        if unmatched:
            refuse(f"--skip named stem(s) not found in {family_dir}: {sorted(unmatched)}")
        if not pairs:
            refuse(f"--skip excluded every discovered pair under {family_dir} -- nothing left to convert")

    entries = []
    failures = []
    tmp_root = Path(tempfile.mkdtemp(prefix="csh_blocks_"))
    try:
        for seq, (src, ref) in enumerate(pairs, start=1):
            name = src.stem
            expected_text = ref.read_text()
            orig = run_all_modes(paths, src, expected_text, tmp_root, modes)
            if any(v.kind != "PASS" for v in orig.values()):
                failures.append((name, f"original file itself is not green: {orig}"))
                print(f"[{seq}/{len(pairs)}] {name}: FAIL (original not green)", file=sys.stderr)
                continue
            body = src.read_text().splitlines()
            ref_body = expected_text.rstrip("\n").splitlines()
            entries.append(Entry("block", len(entries) + 1, name, body, ref_body))
            print(f"[{seq}/{len(pairs)}] {name}: OK", file=sys.stderr)
    finally:
        import shutil
        shutil.rmtree(tmp_root, ignore_errors=True)

    if failures:
        print(f"⛔ {len(failures)} entries FAILED (original not green) -- NOTHING WRITTEN, NOTHING DELETED:", file=sys.stderr)
        for name, reason in failures:
            print(f"   {name}: {reason[:120]}", file=sys.stderr)
        sys.exit(1)

    write_block_suite(entries, args.out_src, args.out_ref, comment_open, comment_close)
    print(f"✅ wrote {args.out_src} / {args.out_ref}: {len(entries)} entries", file=sys.stderr)

    banner_re = banner_re_for(comment_open, comment_close)
    reread = read_block_suite(args.out_src, args.out_ref, banner_re)
    tmp_root = Path(tempfile.mkdtemp(prefix="csh_blocks_verify_"))
    mismatches = []
    try:
        for (src, ref), written in zip(pairs, reread):
            orig_verdicts = run_all_modes(paths, src, ref.read_text(), tmp_root, modes)
            suite_verdicts = run_suite_entry(paths, written, tmp_root, modes, ext=ext)
            if not all(suite_verdicts[m].behaviorally_equal(orig_verdicts[m]) for m in modes):
                mismatches.append((src.stem, orig_verdicts, suite_verdicts))
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
          f"modes={modes}. Safe to delete the {len(pairs) * 2} original files "
          f"({len(failures)} left as loose, not green, not deleted).", file=sys.stderr)
    sys.exit(0)


def cmd_run(args):
    paths = resolve_paths()
    check_scrip(paths)
    if args.lang:
        cfg = LANG_CONFIGS[args.lang]
        ext = cfg["ext"]
        modes = (args.modes or cfg["modes"]).split(",")
        banner_re = banner_re_for(cfg["comment_open"], cfg["comment_close"])
        entries = read_block_suite(args.sno, args.ref, banner_re)
    else:
        ext = ".sno"
        modes = (args.modes or "m3,m4").split(",")
        entries = read_suite(args.sno, args.ref)
    counts = {m: {"PASS": 0, "FAIL": 0, "CRASH": 0, "HANG": 0, "UNPROVEN": 0, "SKIP": 0} for m in modes}
    tmp_root = Path(tempfile.mkdtemp(prefix="csh_run_"))
    fails = []
    try:
        for e in entries:
            verdicts = run_suite_entry(paths, e, tmp_root, modes, ext=ext)
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


def cmd_extract(args):
    """Materialize ONE suite entry back into a standalone .sno (+ optional .ref) file. For consumers that
    need per-witness standalone access a shared suite file cannot give them directly -- e.g. a gate script
    compiling one named witness under several env-var arms with custom stdout/stderr handling. Reuses
    read_suite() (ONE AUTHORITY for the suite grammar) rather than re-parsing the format a second time."""
    entries = read_suite(args.sno, args.ref)
    for e in entries:
        if e.name != args.name:
            continue
        text = e.sno_lines[0] if e.kind == "line" else "\n".join(e.sno_lines)
        Path(args.out_sno).write_text(text + "\n")
        if args.out_ref:
            ref_text = e.ref if e.kind == "line" else "\n".join(e.ref)
            Path(args.out_ref).write_text(ref_text + "\n")
        return
    refuse(f"no entry named {args.name!r} in {args.sno} (have: {', '.join(sorted(e.name for e in entries))})")


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)

    o = sub.add_parser("capture-oracle-refs", help="bootstrap missing .ref files for an oracle-graded, no-.ref family from a live SPITBOL run (m3+m4 must both agree, rc and text)")
    o.add_argument("family_dir")
    o.add_argument("--modes", default="m3,m4")
    o.add_argument("--force", action="store_true", help="re-capture even stems that already have a .ref (default: leave them alone)")
    o.set_defaults(func=cmd_capture_oracle_refs)

    c = sub.add_parser("convert", help="convert a loose-file family into suite .sno/.ref, validating byte-equal before writing")
    c.add_argument("family_dir")
    c.add_argument("out_sno")
    c.add_argument("out_ref")
    c.add_argument("--modes", default="m3,m4")
    c.add_argument("--skip", default="", help="comma-separated .sno stems to deliberately exclude from this run (left as loose files, never deleted)")
    c.add_argument("--skip-reason", default="", help="mandatory-in-spirit reason printed for every --skip name")
    c.set_defaults(func=cmd_convert)

    b = sub.add_parser("convert-blocks", help="convert a format-(B)-only banner-block family for a LANG_CONFIGS dialect (non-SNOBOL4)")
    b.add_argument("lang", choices=sorted(LANG_CONFIGS))
    b.add_argument("family_dir")
    b.add_argument("out_src")
    b.add_argument("out_ref")
    b.add_argument("--modes", default="", help="default: LANG_CONFIGS[lang]['modes']")
    b.add_argument("--skip", default="", help="comma-separated stems to deliberately exclude from this run (left as loose files, never deleted)")
    b.add_argument("--skip-reason", default="", help="mandatory-in-spirit reason printed for every --skip name")
    b.set_defaults(func=cmd_convert_blocks)

    r = sub.add_parser("run", help="run a suite .sno/.ref pair (or --lang dialect pair) and print PASS/FAIL/CRASH/HANG/UNPROVEN/SKIP counts")
    r.add_argument("sno")
    r.add_argument("ref")
    r.add_argument("--modes", default="", help="default: m3,m4 (or LANG_CONFIGS[lang]['modes'] if --lang given)")
    r.add_argument("--lang", default="", choices=[""] + sorted(LANG_CONFIGS), help="read/grade as a LANG_CONFIGS dialect instead of the default SNOBOL4 suite format")
    r.set_defaults(func=cmd_run)

    e = sub.add_parser("extract", help="materialize ONE suite entry back into a standalone .sno (+ optional .ref) file")
    e.add_argument("sno")
    e.add_argument("ref")
    e.add_argument("name")
    e.add_argument("out_sno")
    e.add_argument("--out-ref", default="", dest="out_ref")
    e.set_defaults(func=cmd_extract)

    args = ap.parse_args()
    args.func(args)


if __name__ == "__main__":
    main()
