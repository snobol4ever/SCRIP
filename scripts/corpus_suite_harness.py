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
  (B) MULTI-LINE (banner) entry, used when a join exceeds 200 chars or changes behaviour, OR when the
      original source carries column-1 '*' comments (SNOBOL4 has no inline-comment syntax, so a
      comment cannot survive a one-line join -- see has_comment_lines()), OR when the original was
      already non-green at conversion time (an XFAIL witness -- see below):
                        "*----...---- <seq> <name>[ XFAIL]"  (banner, 80 chars, SNOBOL4 comment syntax)
                        <original statement lines, verbatim, one per physical line>
                        (block runs until the next banner or EOF)
                        Same banner + expected output VERBATIM (no escaping) in family.ref.
      A comment/blank-preserving block uses the ORIGINAL file's lines verbatim (not the
      statement-reconstructed form multiline_block() produces for the plain length/behaviour
      fallback) -- simpler and lossless, with no dependence on the goto-merge heuristic.

⭐ XFAIL/XPASS (added corpus-crosscheck-probe-total-conversion, probe-consolidate-passthru): a
family may contain witnesses that are DELIBERATELY red at conversion time -- an active correctness
campaign's documented, known-bad state (e.g. probe/passthru's ARCH-PASSTHRU.md witnesses, "a red row
is never denied"). Lon's 2026-08-28 total-conversion ruling forbids leaving these as permanent loose-
file exceptions, but converting one must not silently inflate a caller's FAIL count. convert_one()
never refuses a non-green original; it marks the resulting block Entry `xfail=True` (banner carries
a trailing " XFAIL") and byte-equal-or-no-delete still requires the candidate to reproduce the
ORIGINAL's exact verdict, whatever it is. `run`/cmd_run then buckets an XFAIL entry's non-PASS
verdict as XFAIL (not FAIL) and a surprising PASS as XPASS (not PASS) in the SUITE_BOARD line's
<mode>_xfail/<mode>_xpass fields -- a caller that only reads <mode>_fail/<mode>_pass (like
test_corpus_snobol4.sh's probe/ auto-discovery loop) is unaffected by a witness it was never told
to expect green. XPASS is surfaced exactly as loudly as FAIL: the bug got fixed and nobody promoted
the marker, which is exactly as actionable as a fresh failure, just in the opposite direction.
`list <sno> <ref>` lists every entry name (one per line) for a bash gate that needs to enumerate a
suite's contents before extract()-ing each one -- same read_suite()-is-the-only-authority precedent
as `extract`, this file's established idiom for a gate script that needs standalone per-witness
files with custom env-var arms or output handling (see test_gate_udc.sh, util_board_m4_gva_seed_probe.sh).

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
import hashlib
import os
import re
import subprocess
import sys
import shutil
import time
import tempfile
from pathlib import Path

BANNER_WIDTH = 80
ONE_LINE_CAP = 200
BANNER_RE = re.compile(r"^\*-+ (?P<seq>\d+) (?P<name>\S+)(?P<xfail> XFAIL)?$")
# ⭐ DIALECT-BLIND BANNER SNIFFER -- "does this file LOOK like a suite sidecar", asked without knowing
# which language wrote it. banner_re_for() needs the comment syntax up front; this one is for the single
# question loose_stdin_companion() has to answer about a file it found by NAME, before any language is
# known. Deliberately loose on the left (any run of comment-open punctuation) and strict on the right
# (dashes, seq, name) -- the shape no real stdin content has, which is the whole point.
ANY_BANNER_RE = re.compile(r"^[*#%{/][*#%{/ ]*-{3,} \d+ \S+")

# ============================================================ language configs ===
# Per-language config for the format-(B)-ONLY (banner-block) conversion path -- convert_blocks
# below. Format (A)'s one-line statement join stays SNOBOL4-specific (parse_statements /
# join_one_line / _has_goto_field): a parser-ladder family never attempts it (format B is the
# task spec's design for these families, not a join-failure fallback), so a new dialect only
# needs its source extension, its comment syntax for the banner (open/close -- close is "" for a
# line-comment language), and its grading mode. Add an entry for a language only once you have
# actually run --dump-ast (or whatever mode) against a real sample and confirmed the banner
# round-trips -- do not add a config for a language nobody has exercised.
LANG_CONFIGS = {
    "raku": {"ext": ".raku", "comment_open": "#", "comment_close": "", "modes": "ast"},
    "rebus": {"ext": ".reb", "comment_open": "#", "comment_close": "", "modes": "ast"},
    "pascal": {"ext": ".pas", "comment_open": "{", "comment_close": "}", "modes": "m3,m4"},
    "prolog": {"ext": ".pl", "comment_open": "%", "comment_close": "", "modes": "ast"},
    "icon": {"ext": ".icn", "comment_open": "#", "comment_close": "", "modes": "m3,m4"},
    "snocone": {"ext": ".sc", "comment_open": "/*", "comment_close": " */", "modes": "ast"},
}


# ⛔⭐ `snobol4` IS AN ACCEPTED SPELLING OF THE DEFAULT, AND LEAVING IT OUT COST TWO SESSIONS. The default
# is the empty string, so `--lang snobol4` -- the obvious thing to type, and the spelling this file itself
# uses internally (`lang = args.lang or "snobol4"`) -- was rejected by ARGPARSE, which exits 2 with its own
# usage text. GOAL-TEST-SUITE-CONSISTENCY.md lists that exact confusion as measured shape #1 of HOW A
# CRITERION LIES: a caller reads rc=2 as THE TOOL refusing to measure, when the tool never ran at all.
# Normalised to "" immediately after parsing (see main), because `if args.lang:` is load-bearing in six
# places downstream and a truthy synonym for the default would change which reader parses the suite.
LANG_CHOICES = [""] + sorted(LANG_CONFIGS) + ["snobol4"]


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
    """⛔ ONE REFUSAL CODE, AND IT IS rc=2 (ceo CEO-233, row harness-refusal-exit-code-unified-on-rc-2, 2026-09-04).

    ⛔ THIS EXITED 3 FOR MONTHS, by this file's own local convention, while every bash gate and lib_gate.sh's
    three-code ladder said a refusal is rc=2 -- 0 measured-and-clean · 1 measured-and-red · 2 COULD NOT MEASURE.
    The divergence became untenable when the stale-binary preflight landed here (row harness-and-ladder-runner-
    refuse-on-a-stale-binary-...): it follows the law and exits 2, so ONE tool carried TWO refusal codes and a
    caller could not ask "did it refuse?" without knowing WHICH refusal it hit. ⭐ The old convention was not
    silly -- it wanted to distinguish "could not measure" from a red board -- but rc=1 already carries the red
    board, so the third code bought nothing the law did not already give, and cost the one question every caller
    actually asks. A local convention that disagrees with a fleet law is a trap even when its reasoning is sound,
    because the reasoning lives here and the callers live everywhere."""
    print(f"⛔ REFUSING: {msg}", file=sys.stderr)
    sys.exit(2)


def require_population(paths, count, floor, label):
    """⛔ REFUSE rc=2 when a verdict is about to be computed over fewer than `floor` graded rows.

    ⭐ THIS FUNCTION DELIBERATELY CONTAINS NO FLOOR LOGIC. It shells out to
    scripts/util_require_population.sh, which sources gate_floor() from lib_gate.sh -- the SAME
    function every bash suite runner calls, for the same reason require_fresh() above shells out to
    util_require_fresh.sh rather than reimplementing staleness: a second copy of a rule is how a cured
    defect class comes back the moment nobody is looking at the copy that got missed.

    THE DEFECT: `cmd_run`'s verdict is `sys.exit(0 if not fails else 1)` -- a suite whose `entries` list
    is empty (a malformed or emptied ALL.sno/ALL.ref pair, with no --shard involved to trip the sibling
    check just above) never enters the grading loop, `fails` stays [], and this exits 0. A run that
    graded nothing must never read as a run that graded everything and found it clean (row every-board-
    wrapper-refuses-on-a-zero-population-instead-of-passing-vacuously, hq_T 2026-09-04)."""
    shim = paths["scrip_root"] / "scripts" / "util_require_population.sh"
    if not shim.is_file():
        print(f"⛔ REFUSED-TO-GRADE rc=2: {shim} missing -- cannot establish that this run graded "
              f"anything (the ONE population-floor authority, never reimplemented here)", file=sys.stderr)
        sys.exit(2)
    r = subprocess.run(["bash", str(shim), "--gate", "corpus_suite_harness", str(count), str(floor), label],
                       capture_output=True, text=True)
    if r.returncode != 0:
        sys.stderr.write(r.stdout)
        sys.stderr.write(r.stderr)
        sys.exit(2)


def check_scrip(paths):
    if not (paths["scrip_bin"].is_file() and os.access(paths["scrip_bin"], os.X_OK)):
        refuse(f"scrip is not built/executable at {paths['scrip_bin']}")
    require_fresh(paths)


def require_fresh(paths):
    """⛔ REFUSE rc=2 when the binary about to be graded predates the tree it will be labelled with.

    ⭐ THIS FUNCTION DELIBERATELY CONTAINS NO STALENESS LOGIC. It shells out to
    scripts/util_require_fresh.sh, which sources gate_require_fresh() from lib_gate.sh -- the SAME
    function every bash runner calls (hq_B, SCRIP 4c7253e99). Row harness-and-ladder-runner-refuse-on-
    a-stale-binary-like-the-artifact-regen-does (ceo -> hq_T 2026-09-04).

    ⛔ WHY IT IS NOT A PYTHON REIMPLEMENTATION, WHICH WOULD HAVE BEEN SHORTER AND IS THE WHOLE POINT OF
    THE ROW: this exact idea already existed TWICE in bash (gate_require_fresh and lib_build_currency's
    assert_binary_current). ceo cured a wrong staleness basis in one of them (3d12ca54 -- "IT IS NOT A
    SUPERSET, IT IS WRONG"); the cure never reached the other, so the identical defect regenerated and
    was cured a second time nine days later. A third copy -- in a second LANGUAGE, where no grep for the
    bash symbol would ever find it -- is how that class survives its own cure a third time. The cost of a
    copy is never the duplicated lines; it is that fixing one copy makes everybody believe the class is
    dead.

    THE WITNESS (ceo, 2026-09-04): two false-red audits in one day -- a fetched-not-merged clone at 12:23,
    and a 10:57 binary at 14:03 that read RED and then GREEN after an incremental make. Nothing the board
    printed could distinguish "the tree is broken" from "you are grading last morning's binary".

    ⛔ SCOPE NOTE -- the .so is checked for STALENESS but a MISSING .so is left alone on purpose. This
    harness already has documented behaviour for that case (run_m4 -> Verdict("SKIP", "libscrip_rt.so not
    built")), and turning another instrument's SKIP into a REFUSE is a different row's ruling, not this
    one's. A stale .so, though, is squarely this class and the dangerous half of it: an older runtime
    still exports what source deleted, so m4 grades a program nobody is shipping."""
    shim = paths["scrip_root"] / "scripts" / "util_require_fresh.sh"
    if not shim.is_file():
        print(f"⛔ REFUSED-TO-GRADE rc=2: {shim} missing -- cannot establish that the binary about to be "
              f"graded is current (the ONE stale-binary authority, never reimplemented here)", file=sys.stderr)
        sys.exit(2)
    arts = [str(paths["scrip_bin"])]
    so = paths["rt_dir"] / "libscrip_rt.so"
    if so.is_file():
        arts.append(str(so))
    r = subprocess.run(["bash", str(shim), "--gate", "corpus_suite_harness"] + arts,
                       capture_output=True, text=True)
    if r.returncode != 0:
        sys.stderr.write(r.stdout)
        sys.stderr.write(r.stderr)
        sys.exit(2)


def resolve_oracle_bin(paths, lang=""):
    """Resolve the correctness oracle's binary+flags via lib_oracle_flags.sh -- the ONE authority
    (RULES.md Oracles section): never hand-assemble an oracle path/flag pair in a second copy.
    Shells out to the real bash accessors so a future change to a shared oracle's location or
    capability check is picked up automatically, same as every bash caller gets. Used only by
    cmd_capture_oracle_refs -- no other command in this file talks to a live oracle at all, since
    every other family already ships a committed .ref.

    lang="" or "snobol4" (the original, only path until 2026-08-29): sbl_correctness_bin +
    sbl_lang_flags, i.e. SPITBOL `-bf`. lang="prolog": swipl_bin() from the same lib, invoked
    `-q` alone. No other --lang has an oracle wired here yet; refuses rather than guessing one.

    ⛔ WAS `-q -g halt` until 2026-09-05 (row prolog-every-non-package-source-...-with-oracle-refs,
    seat07) -- `-g halt` runs as a -g GOAL, which fires BEFORE swipl's deferred `initialization(Goal,
    main)` hook (that hook runs at the point the interactive top level would otherwise start, which
    `-g halt` preempts by exiting first). Every file using the `:- initialization(main, main).` idiom
    -- the modern, recommended SWI entry-point form, and the one every corpus/tests/scrip_test/prolog/
    demo file uses -- therefore ran, printed nothing, and got excluded as "oracle produced EMPTY
    output" or (worse, via the loose-pair path, which has no empty-output guard) silently merged a
    vacuous ref. Witness: corpus/tests/scrip_test/prolog/hello.pl prints "Hello, World!" under `swipl
    -q hello.pl` and nothing under `swipl -q -g halt hello.pl`. Bare `-q` is the documented way to run
    a swipl script non-interactively: a registered main-goal fires and auto-halts with Goal's own
    success/fail exit code (the OLD flags always forced exit 0, masking a failing main-goal); a file
    with no entry point at all falls through to the toplevel, hits EOF on `/dev/null` stdin, and exits
    0 with at most a trailing newline -- already stripped by this function's own `.rstrip("\n")`,
    verified empirically against a no-directive witness before this change landed."""
    lib = paths["scrip_root"] / "scripts" / "lib_oracle_flags.sh"
    if not lib.is_file():
        refuse(f"lib_oracle_flags.sh missing at {lib} -- the ONE oracle-flag authority (RULES.md)")
    if lang in ("", "snobol4"):
        r = subprocess.run(["bash", "-c", f". '{lib}' && sbl_correctness_bin && sbl_lang_flags"],
                            capture_output=True, text=True)
        if r.returncode != 0:
            refuse(f"lib_oracle_flags.sh refused (sbl_correctness_bin/sbl_lang_flags): {r.stderr.strip()}")
        lines = r.stdout.strip("\n").splitlines()
        if len(lines) != 2 or not lines[0] or not lines[1]:
            refuse(f"unexpected output from sbl_correctness_bin/sbl_lang_flags: {r.stdout!r}")
        return lines[0], lines[1]  # oracle binary path, language flags (e.g. "-bf")
    if lang == "prolog":
        r = subprocess.run(["bash", "-c", f". '{lib}' && swipl_bin"], capture_output=True, text=True)
        if r.returncode != 0:
            refuse(f"lib_oracle_flags.sh refused (swipl_bin): {r.stderr.strip()}")
        bin_path = r.stdout.strip()
        if not bin_path:
            refuse(f"unexpected empty output from swipl_bin: {r.stdout!r}")
        return bin_path, "-q"
    if lang == "icon":
        # ⭐ THE ONE-STEP DRIVER, NOT icont/iconx (hq_P 2026-08-30). run_oracle()'s contract is a SINGLE
        # invocation -- `[bin] + flags + [name]` in the file's own directory -- and Icon is classically a
        # two-step compile-then-run, which is why this language could not be wired here before. The shared
        # tree ships `icon`, which compiles and runs in one shot, so it fits the existing contract with no
        # change to run_oracle. Resolved through lib_oracle_flags.sh's icon_bin(), never a hand-built path:
        # every oracle in this project lives OFF PATH by design, so `command -v icon` would answer the wrong
        # question and under-report silently.
        r = subprocess.run(["bash", "-c", f". '{lib}' && icon_bin"], capture_output=True, text=True)
        if r.returncode != 0:
            refuse(f"lib_oracle_flags.sh refused (icon_bin): {r.stderr.strip()}")
        bin_path = r.stdout.strip()
        if not bin_path:
            refuse(f"unexpected empty output from icon_bin: {r.stdout!r}")
        return bin_path, ""   # no flags: `icon <file>` is the whole invocation
    if lang == "pascal":
        # ⭐ hq_C's ruling (2026-08-28, KEEP.md § FPC-oracle regen exceptions): `fpc -Miso` is the Pascal
        # correctness oracle. Unlike icon, FPC has no one-step compile-and-run mode, so the "binary" handed
        # to run_oracle() is fpc_oracle_run.sh -- a thin wrapper that compiles to a private tmpdir (never the
        # source directory) and execs the result, matching run_oracle()'s single-process contract exactly
        # the way icon_bin's real one-step binary does. `-Miso` is passed as this resolver's own flags, same
        # shape as snobol4's `-bf`/prolog's `-q` -- never hand-assembled a second time at a call site.
        wrapper = paths["scrip_root"] / "scripts" / "fpc_oracle_run.sh"
        if not wrapper.is_file():
            refuse(f"fpc_oracle_run.sh missing at {wrapper} -- the one-step FPC oracle driver")
        return str(wrapper), "-Miso"
    refuse(f"no oracle wired for --lang {lang!r} in capture-oracle-refs yet (only snobol4/prolog/icon/pascal so far)")


def run_oracle(oracle_bin, flags, sno_path, timeout, stdin_text=None):
    """One live oracle invocation. stdin is `/dev/null` unless the caller passes stdin_text -- the
    one caller that does is cmd_capture_oracle_refs, feeding a loose companion resolved by
    loose_stdin_companion(); every other oracle/scrip call in this file
    and in test_one_witness.sh is unfed (whose exact contract this mirrors: stdout text AND returncode both
    matter -- a witness testing a deliberate error exit is not 'wrong' for exiting non-zero, it is
    wrong only if scrip's rc/text pair disagrees with the oracle's).
    ⛔⭐ ARGV IS THE BARE BASENAME, CWD IS THE FILE'S OWN DIRECTORY (row suite-harness-argv-echoes-a-
    mktemp-path-so-diagnostic-programs-cannot-be-graded). A diagnostic that echoes its own argv (a
    SPITBOL ERROR NNN line naming the file being compiled, e.g.) echoes it VERBATIM -- a full path
    changes every run (mktemp), and even a stable full path differs from what run_m3/run_suite_entry
    will echo later for the SAME witness. Passing just the name, from the right cwd, is what makes
    that echoed text reproducible and comparable to a frozen .ref at all."""
    sno_path = Path(sno_path)
    argv = [oracle_bin] + flags.split() + [sno_path.name]
    kind, out, _err, rc = _run_raw(argv, timeout, cwd=str(sno_path.parent), stdin_text=stdin_text)
    # ⛔⭐ AN ORACLE KILLED BY A SIGNAL IS A CRASH, NOT A RUN -- and until 2026-09-04 this returned "RAN" for one
    # (row every-ref-cutting-path-refuses-when-the-oracle-dies-mid-cut, ceo -> hq_T, on seat07's finding that
    # `sbl -bf` SIGSEGVs on about half its ERROR 212 runs WHILE PRINTING the diagnostic). _run_raw's "RAN" means
    # only "the process was launched and did not time out"; the signal shows up as a NEGATIVE returncode, which
    # cmd_capture_oracle_refs -- whose whole guard is `if ora_kind != "RAN"` -- never looked at. So the one
    # command in this file that mints ground truth from a live oracle would have written a TRUNCATED ref, and a
    # truncated ref is the worst artifact here: a plausible-looking pin that grades every future run against a
    # lie, indistinguishable downstream from a good one.
    # ⛔⛔ THE DEFECT SURVIVED A SOURCE-LEVEL CENSUS THAT DECLARED THIS PATH SAFE, done by hq_T hours earlier and
    # written into the row's own GOAL. Reading the code proved "_run_raw returns CRASH on a signal" -- which is
    # true of classify(), a DIFFERENT function, and not of this one. It was found only when the gate ran the
    # thing with a synthetic oracle that prints output and then segfaults. ⭐ A census of what code SAYS is not a
    # measurement of what it DOES, and the two disagree exactly where it matters most.
    # ⭐ Fixed HERE rather than in cmd_capture_oracle_refs, because util_build_package_suite.py caught this only
    # by independently re-deriving the same test (`ora_rc < 0`) at its own call site -- two copies of one rule,
    # one of which was missing. Callers now inherit the classification from the single reader.
    if kind == "RAN" and rc is not None and rc < 0:
        kind = "CRASH"
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


def _run_raw(argv, timeout, cwd=None, env=None, stdin_text=None):
    try:
        if stdin_text is None:
            r = subprocess.run(argv, stdin=subprocess.DEVNULL, capture_output=True,
                                timeout=timeout, cwd=cwd, env=env)
        else:
            r = subprocess.run(argv, input=stdin_text.encode(), capture_output=True,
                                timeout=timeout, cwd=cwd, env=env)
    except subprocess.TimeoutExpired as e:
        return "HANG", (e.stdout or b""), (e.stderr or b""), None
    except FileNotFoundError as e:
        return "UNPROVEN", b"", str(e).encode(), None
    return "RAN", r.stdout, r.stderr, r.returncode


def classify(argv, timeout, expected_text, cwd=None, env=None, stdin_text=None, want_rc=0):
    kind, out, err, rc = _run_raw(argv, timeout, cwd=cwd, env=env, stdin_text=stdin_text)
    if kind == "HANG":
        return Verdict("HANG", out, err, None, detail=f"exceeded {timeout}s")
    if kind == "UNPROVEN":
        return Verdict("UNPROVEN", out, err, None, detail=err.decode("utf-8", "replace"))
    if rc is not None and rc < 0:
        return Verdict("CRASH", out, err, rc, detail=f"signal {-rc}")
    got = out.decode("utf-8", "replace").rstrip("\n")
    exp = expected_text.rstrip("\n") if expected_text is not None else None
    # ⛔⭐ A POSITIVE NONZERO rc IS NOT A PASS UNLESS IT WAS DECLARED (hq_C ruling 2026-08-30, on seat15's
    # find; corpus-suites-consolidation.task.md § VACUOUS PASS ON POSITIVE rc). Until now only a NEGATIVE rc
    # was inspected (the CRASH arm above) and a positive one fell straight through to the text compare. The
    # witness: snobol4 `simple_output_63` ends in lowercase `end`, SCRIP rejects it rc=1 with the diagnostic
    # on STDERR, stdout is empty, and its .ref is EMPTY -- so got == exp == "" and it scored PASS on every
    # run since it was absorbed, reported as a promotable XPASS. A program that cannot compile was passing.
    # ⛔ A BLANKET rc==0 REQUIREMENT WOULD BE WRONG, and that is why this is declared and not inferred:
    # prolog's assertz_directive_2/3/4 and asserta_assertz_directive_1 PASS with NON-EMPTY matching refs and
    # rc=1 BY DESIGN -- the failure-driven-loop family (ARCH-LANGUAGES.md § FAILURE-DRIVEN LOOP WITH NO
    # FALLBACK CLAUSE). Blanket-gating turns four correct entries red. Blast radius censused before landing:
    # snobol4 1 (the defect), prolog 4 (declared below), and ZERO in icon/raku/rebus/snocone/pascal (seat15).
    # ⭐ Declared-not-derived, same principle as the `modes` column: "rc!=0 plus an empty ref" is a heuristic
    # that is right on today's two languages and silently wrong on the next one.
    if exp is not None and got == exp:
        if rc is not None and rc != want_rc:
            return Verdict("FAIL", out, err, rc,
                           detail=f"output matched but rc={rc}, expected {want_rc} (declare want_rc if this is correct)")
        return Verdict("PASS", out, err, rc)
    return Verdict("FAIL", out, err, rc, detail="output mismatch" if exp is not None else "no expected text")


def stdbuf_wrap(paths, argv):
    if paths["stdbuf_bin"]:
        return [paths["stdbuf_bin"], "-o0", "-e0"] + argv
    return argv


def run_m3(paths, sno_path, expected_text, timeout=None, stdin_text=None, want_rc=0, prog_argv=None):
    timeout = timeout or paths["timeout"]
    # ⛔⭐ ARGV IS THE BARE NAME, NOT THE FULL PATH (row suite-harness-argv-echoes-a-mktemp-path-so-
    # diagnostic-programs-cannot-be-graded) -- a diagnostic that echoes its own argv (e.g. a SPITBOL
    # ERROR NNN line naming the file) would otherwise embed this run's own ever-changing mktemp
    # directory, which no frozen .ref can ever match. cwd (set below) is what makes the bare name
    # still resolve to the right file.
    argv = stdbuf_wrap(paths, [str(paths["scrip_bin"]), "--run", Path(sno_path).name])
    # ⛔⭐ THE `--` SEPARATOR IS MANDATORY AND IS WHAT MAKES THIS SAFE. The driver has NO unknown-flag
    # diagnostic: any unrecognised argument falls through to being treated as a FILENAME, so a declared
    # program argument spelled like a flag (`-n10`, the exact shape the first witness used) would be
    # eaten by the driver as a second source file rather than reaching the program. Everything after
    # `--` is the program's own argv -- verified live: `scrip p.icn -- -n10 foo` -> argc=2, both args in
    # order. Empty/None prog_argv appends NOTHING, not a bare `--`, so every existing caller's argv is
    # byte-identical to what it was before this parameter existed.
    if prog_argv:
        argv = argv + ["--"] + [str(a) for a in prog_argv]
    env = dict(os.environ, SNO_LIB=str(paths["inc"]))
    # ⛔⭐ cwd IS THE SOURCE FILE'S OWN DIRECTORY, NOT THE HARNESS'S. A corpus program's relative
    # opens (Icon `open("X")`, and any read of a data companion) resolve against the RUNNING file's
    # directory -- that is how a loose file is run by hand and by test_corpus_snobol4.sh. Without
    # this the harness ran every original from wherever the caller happened to stand, so a program
    # with a data companion FAILED the convert-blocks "original not green" pre-check and could never
    # be converted. ⛔ AND THE MESSAGE BLAMED THE WRONG THING: "original file itself is not green"
    # points the reader at the corpus file, which is green -- measured on rung36_jcon_fncs1.icn,
    # byte-identical to its .ref when run from its own directory in BOTH modes, FAIL in both from
    # anywhere else. Consistent for suite entries too: run_suite_entry materializes the entry into a
    # temp dir and passes THAT path, so parent is the temp dir -- exactly where it copies companions.
    return classify(argv, timeout, expected_text, cwd=str(Path(sno_path).parent), env=env, stdin_text=stdin_text, want_rc=want_rc)


def compile_m4(paths, sno_path, out_bin, tmp_dir):
    """Mirrors compile_mode4() in test_corpus_snobol4.sh exactly. Returns None on success, or a
    Verdict("SKIP", ...) describing where it failed."""
    s_path = tmp_dir / "p.s"
    env = dict(os.environ, SNO_LIB=str(paths["inc"]))
    sno_path = Path(sno_path)
    # ⛔⭐ SAME FIX AS run_m3, AND THIS CALL HAD NO cwd AT ALL BEFORE (ran from the harness's own cwd,
    # only "working" because argv carried a full path) -- row suite-harness-argv-echoes-a-mktemp-path-
    # so-diagnostic-programs-cannot-be-graded. Bare name in argv, explicit cwd so it still resolves
    # and so a -INCLUDE (compile-time, unlike run_m3's runtime open() concern) resolves relative to
    # the file's own directory too, not wherever the caller happened to stand.
    with open(s_path, "wb") as f:
        r = subprocess.run([str(paths["scrip_bin"]), "--compile", sno_path.name],
                            stdout=f, stderr=subprocess.DEVNULL, env=env, cwd=str(sno_path.parent))
    if r.returncode != 0:
        return Verdict("SKIP", detail="scrip --compile failed")
    o_path = tmp_dir / "p.o"
    r = subprocess.run(["gcc", "-c", str(s_path), "-o", str(o_path)],
                        stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, cwd=str(paths["scrip_root"]))
    if r.returncode != 0:
        return Verdict("SKIP", detail="gcc -c failed")
    rt_dir = str(paths["rt_dir"])
    # ⛔⭐ RULED -- -no-pie STAYS OUT, DELIBERATELY, NOT AN OVERSIGHT (row `m4-pie-vs-no-pie-changes-behaviour-
    # not-just-signal`, seat10 2026-08-28, full analysis + evidence in .github/ARCH-ENGINE.md § "Mode-4 Link
    # Mode"). -no-pie looked like a free determinism fix (it does stabilize a crashing SIGNAL, e.g. fz_segv_09:
    # 4xSIGILL+1xSIGSEGV under PIE -> 5/5 stable SIGSEGV under -no-pie) but gdb on the faulting RSP proved it is
    # not free: fz_red_m2a_fence_cap_gen and fz_segv_10 are NOT crash-expected (.ref = "match") and PIE runs
    # them clean 20/20 -- -no-pie SIGSEGVs them 20/20, RSP == 0x0 at fault under -no-pie, a normal valid stack
    # address at the identical program point under PIE. -no-pie is the arm that is wrong, not a measurement
    # artifact. Do not re-add it here without re-opening that ruling.
    r = subprocess.run(["gcc", str(o_path), "-L", rt_dir, "-lscrip_rt", "-lm",
                         "-Wl,-rpath," + rt_dir, "-o", str(out_bin)],
                        stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, cwd=str(paths["scrip_root"]))
    if r.returncode != 0:
        return Verdict("SKIP", detail="gcc link failed")
    return None


def run_m4(paths, sno_path, expected_text, tmp_dir, timeout=None, stdin_text=None, want_rc=0, prog_argv=None):
    timeout = timeout or paths["timeout"]
    if not (paths["rt_dir"] / "libscrip_rt.so").is_file():
        return Verdict("SKIP", detail="libscrip_rt.so not built")
    out_bin = tmp_dir / "t.bin"
    skip = compile_m4(paths, sno_path, out_bin, tmp_dir)
    if skip is not None:
        return skip
    # ⭐ NO `--` HERE, AND THE ASYMMETRY WITH run_m3 IS THE POINT: a mode-4 binary IS the program, so
    # its argv is the program's argv directly -- there is no driver in front of it to shield. Same
    # declared list, two spellings, one observable result (verified: identical argc/args in m3 and m4).
    argv = stdbuf_wrap(paths, [str(out_bin)]) + [str(a) for a in (prog_argv or [])]
    env = dict(os.environ, SNO_LIB=str(paths["inc"]))
    # Same rule as run_m3 above: the compiled binary's relative opens must resolve against the
    # SOURCE's directory, not the harness's cwd. out_bin is an absolute path, so moving cwd is safe.
    return classify(argv, timeout, expected_text, cwd=str(Path(sno_path).parent), env=env, stdin_text=stdin_text, want_rc=want_rc)


# ⛔⭐ run_ast TAKES NO prog_argv AND MUST NOT -- NOT AN OVERSIGHT. `--dump-ast` never executes the
# program, so declared arguments cannot change its output; accepting them would let an ast-mode family
# declare an argv that silently grades nothing, which is the "criterion that lies" shape. A language
# whose LANG_CONFIGS modes are "ast" (raku, rebus, prolog, snocone today) therefore cannot USE the argv
# sidecar even though it may legally carry one -- read_argv_sidecar refuses that combination wherever the
# modes are known (cmd_run passes them) rather than accepting a declaration it would then ignore.
def run_ast(paths, src_path, expected_text, timeout=None, want_rc=0):
    """Grading mode for parser-ladder families: `scrip --dump-ast`, diffed as text -- same
    classify() used by run_m3/run_m4, just a different argv. No compile/link step, so it is fast
    and language-agnostic (dispatch is by src_path's own extension, same as scrip --run/--compile)."""
    timeout = timeout or paths["timeout"]
    argv = stdbuf_wrap(paths, [str(paths["scrip_bin"]), "--dump-ast", str(src_path)])
    return classify(argv, timeout, expected_text, want_rc=want_rc)


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


def make_banner(seq, name, xfail=False):
    suffix = f"{seq} {name}" + (" XFAIL" if xfail else "")
    dash_count = max(1, BANNER_WIDTH - 1 - 1 - len(suffix))
    banner = "*" + ("-" * dash_count) + " " + suffix
    return banner


def make_banner_cfg(seq, name, comment_open, comment_close, xfail=False):
    """Generalized make_banner() for LANG_CONFIGS dialects: comment_open="*"/comment_close=""
    reproduces make_banner() exactly (verified: same dash_count formula, open+dashes+' '+suffix).
    A non-empty comment_close (e.g. Snocone's " */") is pinned at the right margin, same as the
    open side's dash-fill -- open + dashes + ' ' + suffix + close, total BANNER_WIDTH.
    ⛔ THE `xfail` PARAMETER IS LOAD-BEARING AND WAS MISSING UNTIL 2026-08-28 (row
    suite-harness-xfail-extract-round-trip). make_banner() had it from the start; this generalized
    twin did not, so for EVERY LANG_CONFIGS dialect the flag was silently dropped on write and had
    no group to be read back into -- an XFAIL block-suite entry round-tripped to a plain entry, and
    the next `run` scored a documented, expected red as a genuine FAIL. ⭐ The two writers diverged
    because the flag was added to one and the other was never asked to prove it agreed: a
    generalization that quietly drops a parameter of the thing it generalizes."""
    suffix = f"{seq} {name}" + (" XFAIL" if xfail else "")
    fixed = len(comment_open) + 1 + len(suffix) + len(comment_close)
    dash_count = max(1, BANNER_WIDTH - fixed)
    return comment_open + ("-" * dash_count) + " " + suffix + comment_close


def banner_re_for(comment_open, comment_close):
    close = comment_close.strip()
    close_pat = (r"\s*" + re.escape(close)) if close else ""
    return re.compile(r"^" + re.escape(comment_open) + r"-+ (?P<seq>\d+) (?P<name>\S+)"
                      + r"(?P<xfail> XFAIL)?" + close_pat + r"$")


# ============================================================= conversion ===
class Entry:
    def __init__(self, kind, seq, name, sno_lines, ref_text_or_lines, stdin=None, xfail=False,
                 xfail_reason=None, want_rc=0, argv=None):
        self.kind = kind          # "line" or "block"
        self.seq = seq
        self.name = name
        self.sno_lines = sno_lines        # list[str]: 1 line for "line", N for "block"
        self.ref = ref_text_or_lines      # str for "line" (unescaped), list[str] for "block"
        self.stdin = stdin                # str fed to the entry's stdin, or None = /dev/null (see STDIN below)
        self.xfail = xfail        # block-only: original was already non-green at conversion time
                                   # (byte-equal-or-no-delete still holds -- the candidate reproduces
                                   # the ORIGINAL's verdict exactly, whatever it is, never just PASS).
                                   # cmd_run buckets these as XFAIL/XPASS instead of FAIL, so a
                                   # pre-existing, documented-red witness doesn't poison a caller's
                                   # FAIL=0 gate. See corpus-suites-consolidation.task.md amendment log.
        self.argv = argv          # list[str] passed to the entry as its program arguments, or None = none at
                                   # all (identical to pre-sidecar behaviour). DECLARED in <family>.argv,
                                   # never inferred -- an entry's own text cannot say what it should be RUN
                                   # with. See sidecar_argv_path() for why discovery beats a flag.
        self.want_rc = want_rc    # int: the exit code a CORRECT run of this entry produces. 0 unless the
                                   # family declares otherwise in <family>.wantrc. DECLARED, never inferred.
        self.xfail_reason = xfail_reason  # str: WHY this entry is expected red, or None. Carried in the
                                   # family.xfail sidecar, NEVER in the banner -- boolean and reason live in
                                   # different layers (see the reason-sidecar section). ⛔ DOCUMENTATION ONLY:
                                   # nothing in cmd_run reads it, so a reason can never change a verdict.


def has_comment_lines(text):
    """True if the source carries any column-1 '*' comment line. SNOBOL4 has no inline-comment
    syntax, so a comment cannot survive a one-line join, and parse_statements()/multiline_block()
    silently drop them -- harmless when a comment carries no information, but not for
    probe/passthru's witnesses, which carry dated research rationale cited by ARCH-PASSTHRU.md and
    ~10 FINDINGs. A conversion that launders that away just because byte-equal-or-no-delete only
    checks RUN OUTPUT would be exactly the transcription-kills-provenance class RULES.md warns about."""
    return any(line[:1] == "*" for line in text.splitlines())


def convert_one(paths, sno_path, ref_path, seq, tmp_root, modes, companion_dir=None, stdin_text=None):
    """Returns (Entry, report_dict). report_dict always has "ok": bool and "reason": str.
    ⛔⭐⭐ stdin_text IS NOT OPTIONAL POLISH -- WITHOUT IT A GREEN WITNESS IS LAUNDERED INTO A PERMANENT
    XFAIL AND THE RUN STILL PRINTS ✅ (measured 2026-08-30, hq_B, on a two-entry scratch family).
    A loose stem with a stdin companion whose .ref was minted FED, re-run here UNFED, produces empty
    output, so orig_green goes False -- and convert does not refuse a non-green original by design
    (probe/passthru's deliberately-red witnesses). It converts it verbatim as XFAIL, the candidate is
    ALSO unfed so it reproduces the same empty output, byte-equal-or-no-delete is satisfied, and the
    command exits "✅ ON-DISK RE-VALIDATION PASSED ... Safe to delete the originals" -- including the
    .input file that was the entry's whole meaning. ⭐ Every individual check behaved exactly as
    specified; the defect is that all of them were asked about the same unfed program. Feeding is what
    makes them questions about the real witness."""
    name = sno_path.stem
    original_text = sno_path.read_text()
    # ⛔⭐ CONVERT MUST BE IDEMPOTENT — STRIP OUR OWN `;* <name>` SENTINEL BEFORE RE-PROCESSING.
    # A format-(A) one-liner ends with a MANDATORY `;* <name>` tag. Feed such a line back through convert
    # (which is exactly what merging new witnesses into an EXISTING suite does: extract all, add new,
    # re-convert) and parse_statements() keeps that tag as a trailing comment statement, join_one_line()
    # preserves it, and then `joined + f";* {name}"` appends a SECOND one -- `;* ev_fn_literal;* ev_fn_literal`.
    # Every re-conversion adds another. MEASURED (hq_C 2026-08-28, root-causing seat02's report): re-converting
    # probe/eval's 21 entries doubled the tag on 17 of them in a single pass.
    # Stripping is safe and precise: the tag is only removed when the file is a SINGLE line whose trailing tag
    # names THIS entry, which is our own writer's output and never a hand-authored witness's real last statement.
    _stripped = original_text.strip("\n")
    if "\n" not in _stripped:
        _m = ONE_LINE_TAG_RE.search(_stripped)
        if _m and _m.group(1) == name:
            original_text = _stripped[: _m.start()] + "\n"
    expected_text = ref_path.read_text()
    statements = parse_statements(original_text)

    orig_verdicts = run_all_modes(paths, sno_path, expected_text, tmp_root, modes, stdin_text=stdin_text)
    orig_green = all(v.kind == "PASS" for v in orig_verdicts.values())
    # ⛔ A non-green original is NOT refused. probe/passthru's law-0d witnesses are DELIBERATELY red
    # ("a red row is never denied" -- board_passthru_combo.sh's own header, ARCH-PASSTHRU.md): this
    # active correctness campaign's whole point is documenting a known-bad state until it's cured, so
    # refusing to convert a red witness would either strand it as a permanent loose-file exception
    # (voided by Lon's 2026-08-28 total-conversion ruling) or force it to be silently fixed/altered by
    # a mechanical conversion pass, which is not this row's call to make. byte-equal-or-no-delete still
    # holds in full: the candidate must reproduce the ORIGINAL's verdict exactly, whatever it is, never
    # just "PASS" -- see corpus-crosscheck-probe-total-conversion.task.md's xfail/xpass extension and
    # cmd_run's XFAIL/XPASS bucketing, which keeps a pre-existing red from inflating a caller's FAIL count.
    # ⛔ A STDIN-BEARING ENTRY IS BLOCK-ONLY, and that is the suite format's rule, not a preference here:
    # write_stdin_sidecar() keys every stdin block by banner, and a format-(A) one-liner has no banner to
    # key on -- it raises rather than dropping the stdin silently. Forcing the block form at the point the
    # stdin is known keeps that from ever becoming a late crash after a whole family has converted.
    force_verbatim = has_comment_lines(original_text) or not orig_green or stdin_text is not None

    # --- attempt (A): one-line join --- skipped for a comment-bearing or non-green original: comments
    # have no one-line syntax to survive in, and a red witness is safest preserved byte-for-byte rather
    # than re-derived through the join's statement-merge heuristic.
    if not force_verbatim:
        joined = join_one_line(statements)
        one_line = joined + f";* {name}"
        if len(one_line) <= ONE_LINE_CAP:
            with tempfile.TemporaryDirectory(dir=tmp_root) as td:
                # ⛔⭐ THE WITNESS'S OWN NAME, NOT A "cand.sno" PLACEHOLDER (row suite-harness-argv-
                # echoes-a-mktemp-path-so-diagnostic-programs-cannot-be-graded) -- run_suite_entry
                # (every future regrade of this same entry, once converted) already names its own
                # temp candidate f"{entry.name}{ext}"; using a DIFFERENT name here would make this
                # one-time conversion-verification step echo something ongoing grading never will,
                # even after the argv/cwd fix above makes the echoed text otherwise reproducible.
                cand = Path(td) / f"{name}.sno"
                cand.write_text(one_line + "\n")
                _copy_companions(one_line + "\n", companion_dir, Path(td))
                cand_verdicts = run_all_modes(paths, cand, expected_text, Path(td), modes, stdin_text=stdin_text)
            if all(cand_verdicts[m].behaviorally_equal(orig_verdicts[m]) for m in modes):
                entry = Entry("line", seq, name, [one_line], expected_text.rstrip("\n"))
                return entry, {"ok": True, "reason": "one-line", "len": len(one_line)}

    # --- fallback (B): multi-line banner block --- verbatim original text (comments/blanks/indentation
    # intact) when forced (see has_comment_lines()/force_verbatim above): simpler AND lossless -- no
    # dependence on the goto-merge heuristic, no re-derivation at all. Otherwise unchanged: the existing
    # statement-reconstructed form every already-converted family was proven against.
    block_lines = original_text.splitlines() if force_verbatim else multiline_block(statements)
    with tempfile.TemporaryDirectory(dir=tmp_root) as td:
        # ⛔⭐ THE WITNESS'S OWN NAME, NOT A "cand.sno" PLACEHOLDER (row suite-harness-argv-echoes-a-
        # mktemp-path-so-diagnostic-programs-cannot-be-graded) — matches run_suite_entry's convention.
        cand = Path(td) / f"{name}.sno"
        block_text = "\n".join(block_lines) + "\n"
        cand.write_text(block_text)
        _copy_companions(block_text, companion_dir, Path(td))
        cand_verdicts = run_all_modes(paths, cand, expected_text, Path(td), modes, stdin_text=stdin_text)
    if all(cand_verdicts[m].behaviorally_equal(orig_verdicts[m]) for m in modes):
        entry = Entry("block", seq, name, block_lines, expected_text.rstrip("\n").splitlines(),
                       stdin=stdin_text, xfail=not orig_green)
        if not orig_green:
            reason = "multi-line-block-verbatim(XFAIL: original already non-green)"
        elif force_verbatim:
            reason = "multi-line-block-verbatim(comments preserved)"
        else:
            reason = "multi-line-block"
        return entry, {"ok": True, "reason": reason}

    return None, {"ok": False, "reason": f"NEITHER form reproduced the original's behavior: orig={orig_verdicts}"}


def run_all_modes(paths, sno_path, expected_text, tmp_root, modes, stdin_text=None, want_rc=0, prog_argv=None):
    out = {}
    if "m3" in modes:
        out["m3"] = run_m3(paths, sno_path, expected_text, stdin_text=stdin_text, want_rc=want_rc, prog_argv=prog_argv)
    if "m4" in modes:
        with tempfile.TemporaryDirectory(dir=tmp_root) as td:
            out["m4"] = run_m4(paths, sno_path, expected_text, Path(td), stdin_text=stdin_text, want_rc=want_rc, prog_argv=prog_argv)
    if "ast" in modes:
        # ⛔ stdin is deliberately NOT threaded into run_ast: --dump-ast parses and never executes,
        # so an entry's stdin cannot reach it. Passing it would imply a dependence that does not exist.
        out["ast"] = run_ast(paths, sno_path, expected_text, want_rc=want_rc)
    return out


# ========================================================== suite writer ===
def write_suite(entries, out_sno, out_ref, out_in=None, lang=""):
    """Returns True iff a stdin sidecar was written (out_in given AND at least one entry carries stdin) --
    same contract and same write_stdin_sidecar() call as write_block_suite(), which had this parameter from
    the start while this SNOBOL4 twin did not: a stdin-bearing snobol4 family had nowhere to put its input,
    so the loose companion was simply stranded by conversion (hq_B 2026-08-30).
    ⛔⭐ FORMAT-(B) BLOCKS ARE EMITTED LAST, ALWAYS — THIS ORDERING IS A CORRECTNESS INVARIANT, NOT A STYLE CHOICE.
    A banner block ends only at the NEXT BANNER OR EOF (see read_suite()'s scanner). So any format-(A) one-line entry
    written AFTER a block is silently swallowed into that block's body: the file stops round-tripping, and the swallowed
    entries either vanish or come back duplicated. MEASURED (hq_C 2026-08-28, on seat02's convert-corrupts-eval report):
    re-converting probe/eval's 21 entries wrote 21 and read back 22, with `ev_fn_beauty_shape` appearing twice, because
    alphabetical order put that one block in the MIDDLE and it absorbed the nine one-liners after it.
    ⛔ Suites written before this fix were correct only BY ACCIDENT OF ORDERING — probe/eval survived because its two
    blocks happened to be appended last. Do not "simplify" this back into a single loop over `entries`.
    ⛔⭐ `lang` PICKS THE BANNER'S COMMENT SYNTAX (row every-vendored-package-..., first Icon package via
    util_build_package_suite.py) -- the hardcoded '*' below is SNOBOL4's comment char, not a suite-format
    constant; a '*'-banner in a non-SNOBOL4 block is not a comment in that language at all (Icon's is '#'),
    so the container stops being valid source in its own language. LANG_CONFIGS already carries the right
    comment_open/close (make_banner_cfg/banner_re_for already existed for this, used by
    util_build_master_suite.py -- this writer just never took a lang param to reach them). Blank/"snobol4"
    keeps the exact prior '*' behaviour (every already-built package is lang="" and is unaffected).
    ⛔ The round-trip self-check below MUST use the same convention it just wrote with, or it silently
    validates against its own SNOBOL4-flavored default reader while the real bug (wrong comment char for
    the target language) sails through undetected -- exactly how this shipped unnoticed across 4 SNOBOL4
    packages before the first non-SNOBOL4 one (jcon_tests) hit it."""
    lc = LANG_CONFIGS.get(lang) if lang else None
    lines = [e for e in entries if e.kind == "line"]
    blocks = [e for e in entries if e.kind != "line"]
    if lines and lc:
        raise ValueError(f"write_suite: lang={lang!r} produced {len(lines)} format-A 'line' entries -- "
                          f"make_banner_cfg/read_block_suite is untested for anything but a pure-block suite")
    sno_lines, ref_lines = [], []
    for seq, e in enumerate(lines + blocks, 1):
        # ⛔ RENUMBER THE ENTRY ITSELF, not just the banner being printed. This writer reorders
        # (lines then blocks) AND renumbers, while write_stdin_sidecar() banners its blocks from
        # e.seq -- so leaving e.seq at its pre-reorder value writes a .in banner numbered
        # differently from the .sno banner naming the same entry. read_stdin_sidecar attaches BY
        # NAME and would still work, which is precisely why the inconsistency could sit there
        # unnoticed and confuse the next person to read the two files side by side.
        e.seq = seq
        if e.kind == "line":
            sno_lines.append(e.sno_lines[0])
            ref_lines.append(e.ref.replace("\n", "\\n"))
        else:
            banner = (make_banner_cfg(seq, e.name, lc["comment_open"], lc["comment_close"], xfail=e.xfail)
                      if lc else make_banner(seq, e.name, xfail=e.xfail))
            sno_lines.append(banner)
            sno_lines.extend(e.sno_lines)
            ref_lines.append(banner)
            ref_lines.extend(e.ref)
    Path(out_sno).write_text("\n".join(sno_lines) + "\n")
    Path(out_ref).write_text("\n".join(ref_lines) + "\n")
    # ⛔ AFTER the .sno/.ref write, and keyed by the SAME seq the banners just got (write_suite reorders
    # lines-then-blocks and renumbers, so a sidecar written from the pre-reorder seq would banner its
    # blocks with numbers that no longer name the same entries), and BEFORE the round-trip check below --
    # a check that runs before the sidecar exists cannot verify the sidecar. ⛔ SAME '*'-HARDCODE BUG AS
    # THE MAIN BANNER, ONE FUNCTION OVER: write_stdin_sidecar() already takes comment_open/close as
    # params (it was already lang-agnostic on its own), but this call site never threaded `lc` through --
    # only surfaced once a real Icon package needed BOTH the main banner fix AND a stdin sidecar
    # (jcon_tests, after teaching stdin_for() about .dat: 60->70 absorbed, 17 of them stdin-fed) --
    # `run --lang icon` failed reading ALL.in with the exact same "expected a banner, found '*...'" error.
    wrote_in = False
    if out_in is not None:
        wrote_in = write_stdin_sidecar(lines + blocks, out_in,
                                        lc["comment_open"] if lc else "*", lc["comment_close"] if lc else "")
    # ⛔ ROUND-TRIP OR REFUSE. A writer that cannot be read back is the "lying test" class: it reports success while
    # having destroyed entries. Cheap to check, and it is the only thing standing between an ordering bug and a
    # silently-corrupted permanent suite. ⛔ MUST re-read with the SAME banner convention just written (lc's
    # comment chars via read_block_suite), never the SNOBOL4-flavored read_suite() default -- otherwise this
    # check "passes" by reading back its own wrong assumption instead of the file it actually wrote.
    # ⛔ MUST ALSO PASS in_path WHEN A SIDECAR WAS WRITTEN -- "A CHECK THAT DOES NOT CARRY EVERY FIELD THE
    # GRADER READS IS NOT A CHECK" (hq_C, on the identical principle for the .modes sidecar): a round-trip
    # that never re-reads the .in file it just wrote cannot catch a defect in it, which is exactly how the
    # stdin-sidecar banner bug above shipped past this same self-check undetected.
    _in_for_check = str(out_in) if wrote_in else None
    back = (read_block_suite(out_sno, out_ref, banner_re_for(lc["comment_open"], lc["comment_close"]), in_path=_in_for_check)
            if lc else read_suite(out_sno, out_ref, in_path=_in_for_check))
    if len(back) != len(entries) or [e.name for e in back] != [e.name for e in lines + blocks]:
        raise ValueError(f"⛔ SUITE DID NOT ROUND-TRIP: wrote {len(entries)} entries, read back {len(back)}. "
                         f"wrote={[e.name for e in lines + blocks]!r} read={[e.name for e in back]!r}")
    return wrote_in


def write_block_suite(entries, out_src, out_ref, comment_open, comment_close, out_in=None, out_x=None):
    """write_suite() for a format-(B)-ONLY family: every entry is ALWAYS a banner block (no
    one-line join is ever attempted -- parser-ladder families are format-B by task-spec design,
    not by join-failure fallback), and the banner uses the dialect's own comment syntax.
    Returns True iff a stdin sidecar was written (out_in given AND at least one entry carries
    stdin) -- lets a caller report accurately without re-deriving write_stdin_sidecar's own check."""
    src_lines, ref_lines = [], []
    for e in entries:
        banner = make_banner_cfg(e.seq, e.name, comment_open, comment_close, xfail=e.xfail)
        src_lines.append(banner)
        src_lines.extend(e.sno_lines)
        ref_lines.append(banner)
        ref_lines.extend(e.ref)
    Path(out_src).write_text("\n".join(src_lines) + "\n")
    Path(out_ref).write_text("\n".join(ref_lines) + "\n")
    wrote_in = False
    if out_in is not None:
        wrote_in = write_stdin_sidecar(entries, out_in, comment_open, comment_close)
    if out_x is not None:
        write_xfail_sidecar(entries, out_x, comment_open, comment_close)
    return wrote_in


# =========================================================== stdin sidecar ===
# ⭐ STDIN IS CARRIED OUT-OF-BAND, IN A THIRD PARALLEL FILE (family.in), NEVER IN THE PROGRAM TEXT.
# The suite format already runs on parallel files keyed by an identical banner line (family.sno /
# family.ref); stdin is simply the third rail of that same design, and an entry with no stdin gets
# no banner in the .in file at all (absent = /dev/null, which is exactly the pre-stdin behaviour,
# so every existing suite keeps its current verdicts byte-for-byte).
# ⛔ THE ALTERNATIVE -- an in-band "*<<<STDIN" marker inside the source block -- WAS REJECTED ON
# MEASURED GROUNDS, not taste. In-band signalling over a channel that does not reserve its signal
# byte is fragile by construction: SCRIP a01fe9f6 prepended a port marker to label lines, and
# x86_internal_resolve then ate two bytes of any Pascal record descriptor that legitimately carried
# that byte in its own .string data -- silent, m4-only, and it read as an output mismatch rather
# than a crash (hq_C 2026-08-28, cure 840d05f7). A test program's stdin is ARBITRARY USER BYTES;
# reserving any line shape inside it would recreate that exact class here. Out-of-band cannot collide.
# ⛔ BLOCK ENTRIES ONLY. A format-(A) one-line entry cannot carry stdin -- write it as a block.
# The writer REFUSES such an entry rather than dropping it silently ("a test that cannot measure
# REFUSES; never skip-as-success").
def sidecar_in_path(src_path):
    """The stdin sidecar for a suite is ALWAYS <family>.in beside <family>.sno/.ref -- discovered,
    never passed as a flag. ⭐ That is deliberate: every existing consumer (boards, gates, runners)
    then picks stdin up with ZERO changes to its own argv, so a converted stdin-bearing family
    cannot silently run without its input just because one caller was not updated. Absent file ->
    None -> /dev/null, identical to pre-stdin behaviour."""
    cand = Path(src_path).with_suffix(".in")
    return str(cand) if cand.is_file() else None


# ⛔⭐⭐ ONE RESOLVER FOR THE LOOSE PER-STEM COMPANION -- THERE WERE THREE, AND THEY DISAGREED ABOUT
# WHICH SPELLINGS EXIST (measured 2026-08-30, hq_B, while feeding capture-oracle-refs):
#     sidecar_in_path()        suite level, knew .in and .input, NOT .stdin
#     cmd_convert_blocks()     loose level, knew .stdin ONLY -- a loose .in/.input never reached an entry
#     cmd_capture_oracle_refs() loose level, knew all three, but only in order to REFUSE on them
# All three spellings are LIVE in the corpus (corpus/tests/icon/rung36_jcon_*.stdin, and .input under
# demos/, benchmarks/, packages/), so each resolver was blind to real files the others could see.
# ⭐ AND A PARTIAL RESOLVER NEVER SAYS SO. It resolves to None, None means /dev/null, and the program
# then runs WITH THE WRONG INPUT while its output is graded as a genuine verdict -- a wrong answer
# wearing a verdict, exactly the class sidecar_in_path's own `.input` amendment records. The cure is
# not a fourth list: it is that there is only ever one list, here.
def loose_stdin_companion(src):
    """stdin companion for a LOOSE corpus file: <stem>.stdin / <stem>.in / <stem>.input beside it.
    Returns (text, path, refusal); at most one of text/refusal is ever meaningful --
        (None, None, None)     no companion -> stdin is /dev/null, identical to pre-stdin behaviour
        (str,  Path, None)     feed this text to EVERY arm
        (None, Path|None, str) cannot feed faithfully -- the caller REFUSES, never guesses
    ⛔ AMBIGUITY IS A REFUSAL, NOT A PRECEDENCE RULE. Two companions beside one stem means nobody can
    say which is the program's input, and picking the first is the vacuous-ref failure with one extra
    step: it mints a ref that LOOKS fed and then grades that file against the wrong bytes forever.
    ⛔ NON-UTF-8 IS A REFUSAL TOO. This path feeds text; a byte sequence it cannot carry faithfully
    must stop the mint rather than be lossily re-encoded into a permanent .ref."""
    cands = [c for c in (src.with_suffix(".stdin"), src.with_suffix(".in"), src.with_suffix(".input"))
             if c.is_file()]
    if not cands:
        return None, None, None
    if len(cands) > 1:
        return None, None, ("AMBIGUOUS stdin companion -- %s all exist beside %s; which one is the "
                            "program's input cannot be decided here, keep exactly one"
                            % (", ".join(c.name for c in cands), src.name))
    try:
        text = cands[0].read_bytes().decode("utf-8")
    except UnicodeDecodeError as e:
        return None, cands[0], ("stdin companion %s is not valid UTF-8 (%s) -- this path feeds text and "
                                "will not lossily re-encode a program's own input into a permanent .ref"
                                % (cands[0].name, e))
    # ⛔⭐ A SUITE SIDECAR IS NOT A LOOSE COMPANION, AND THE TWO SPELL THEMSELVES IDENTICALLY. <family>.in
    # beside <family>.sno is the CONVERTED suite's banner-keyed sidecar (sidecar_in_path); <stem>.in beside
    # <stem>.sno is one loose program's raw stdin. Same name shape, same directory, completely different
    # file -- and 6 of them are live in the corpus right now (tests/{snobol4,icon,snocone}/ALL.in,
    # icon/rung36_all.in, icon/rung27_read.in, snocone/crosscheck_rungA14.in).
    # ⭐ Feeding one as raw stdin would not error -- it would quietly push banner lines into the program
    # and grade whatever came out. So the check is on the CONTENT, not the name: real stdin does not open
    # with a banner. Refusing here also says something true and useful, which "wrong output" never would.
    _first = next((ln for ln in text.splitlines() if ln.strip()), "")
    if ANY_BANNER_RE.match(_first):
        return None, cands[0], ("%s is a SUITE .in SIDECAR (its first line is an entry banner), not this "
                                "stem's raw stdin -- feeding it would push banner lines into the program; "
                                "read it with read_stdin_sidecar(), never as loose stdin" % cands[0].name)
    return text, cands[0], None


def read_stdin_sidecar(in_path, banner_re, entries):
    """Attach .in blocks to entries BY NAME. Absent file, or an entry with no block, leaves
    entry.stdin as None (= /dev/null). Refuses on a block naming no entry -- the shape a rename or
    a half-finished conversion produces -- because silence there would run the entry with the WRONG
    stdin (none) and score the result as a genuine FAIL."""
    if in_path is None or not Path(in_path).is_file():
        return
    lines = Path(in_path).read_text().splitlines()
    by_name, i = {}, 0
    while i < len(lines):
        m = banner_re.match(lines[i])
        if not m:
            raise ValueError(f"expected a banner at {in_path} line {i + 1}: {lines[i]!r}")
        name = m.group("name")
        if name in by_name:
            raise ValueError(f"{in_path}: duplicate stdin block for entry {name!r}")
        i += 1
        seg = []
        while i < len(lines) and not banner_re.match(lines[i]):
            seg.append(lines[i]); i += 1
        by_name[name] = ("\n".join(seg) + "\n") if seg else ""
    unknown = sorted(set(by_name) - {e.name for e in entries})
    if unknown:
        raise ValueError(f"{in_path}: stdin blocks with no matching entry: {unknown}")
    for e in entries:
        if e.name in by_name:
            e.stdin = by_name[e.name]


def write_stdin_sidecar(entries, out_in, comment_open, comment_close):
    """Write family.in iff at least one entry carries stdin. Returns True if written."""
    withio = [e for e in entries if e.stdin is not None]
    if not withio:
        return False
    bad = [e.name for e in withio if e.kind == "line"]
    if bad:
        raise ValueError(f"stdin is block-entries-only; convert these to blocks first: {bad}")
    out = []
    for e in withio:
        out.append(make_banner_cfg(e.seq, e.name, comment_open, comment_close))
        if e.stdin:
            out.extend(e.stdin.splitlines())
    Path(out_in).write_text("\n".join(out) + "\n")
    return True



# ========================================================== reason sidecar ===
# ⭐ THE XFAIL *REASON* IS CARRIED OUT-OF-BAND, IN A FOURTH PARALLEL FILE (family.xfail), KEYED BY
# ENTRY NAME EXACTLY LIKE family.in. The BOOLEAN stays in the banner (" XFAIL", seat08, SCRIP
# 3987d9ba) and remains the sole grading signal; this file carries only the human-readable WHY.
# ⛔ THE TWO LIVE IN DIFFERENT LAYERS ON PURPOSE, AND THE SPLIT IS THE WHOLE DESIGN. The banner
# boolean is read by cmd_run to bucket a verdict; the reason is read by a PERSON deciding whether a
# red is still expected. Fusing them -- a banner suffix carrying prose -- would put arbitrary text
# into the one line every reader of the format must parse, and BANNER_RE's own (?P<name>\S+) would
# then be one space away from swallowing it.
# ⛔ WHY KEYED-BY-NAME AND NOT IN-BAND, measured rather than preferred: the loose-file convention
# this replaces put the reason in a sidecar FILE (assign_014_...xfail, 126-164 bytes each), and the
# one-line ";* <name>" tag is INDISTINGUISHABLE FROM PROGRAM TEXT once a one-liner is promoted to a
# block -- the root of two of the three harness defects fixed on 2026-08-28. Entry metadata placed
# inside the entry is the recurring bug here; family.in got this right and this is its third rail.
# ⭐ ABSENT = NO REASON, NEVER AN ERROR. An XFAIL entry with no reason is legal (seat08's 154
# comment-free witnesses converted that way and must keep converting that way), so every existing
# suite keeps its current verdicts byte-for-byte. The refusals below are for the INCOHERENT cases
# only: a reason naming no entry, and a reason on an entry that is not marked XFAIL.
def sidecar_wantrc_path(src_path):
    """The declared-exit-code sidecar for a suite is ALWAYS <family>.wantrc beside <family>.sno/.ref --
    discovered, never passed as a flag, identical to sidecar_in_path()/sidecar_xfail_path(). Absent -> None
    -> every entry expects rc 0. ⭐ Discovery rather than a flag is the whole point: every existing consumer
    -- boards, gates, runners -- picks the declaration up with ZERO changes to its own argv, so an entry that
    legitimately exits nonzero cannot silently be graded by a caller nobody remembered to update."""
    cand = Path(src_path).with_suffix(".wantrc")
    return str(cand) if cand.is_file() else None


def read_wantrc_sidecar(w_path, entries):
    """Attach declared exit codes to entries BY NAME. Format is one `name<TAB>rc` per line; blank lines and
    `#` comments ignored. ⛔ REFUSES on a declaration naming no entry -- the shape a rename or a half-finished
    conversion produces, and the same refusal read_xfail_sidecar makes for the same reason: a declaration that
    matches nothing is either a typo or a stale leftover, and both silently withdraw a guarantee.
    ⛔ REFUSES on rc 0 declared explicitly: 0 is the default, so an explicit 0 is either a misunderstanding of
    the format or a no-op someone will later read as meaningful."""
    if not w_path or not os.path.isfile(w_path):
        return
    by_name = {e.name: e for e in entries}
    unknown = []
    for raw in open(w_path, encoding="utf-8"):
        line = raw.strip()
        if not line or line.startswith("#"):
            continue
        parts = line.split("\t")
        if len(parts) != 2:
            raise ValueError(f"{w_path}: expected 'name<TAB>rc', got: {line!r}")
        name, rc_s = parts[0].strip(), parts[1].strip()
        try:
            rc = int(rc_s)
        except ValueError:
            raise ValueError(f"{w_path}: rc must be an integer for {name!r}, got {rc_s!r}")
        if rc == 0:
            raise ValueError(f"{w_path}: {name!r} declares rc=0, which is already the default -- "
                             f"remove the line rather than restating it")
        if name not in by_name:
            unknown.append(name)
            continue
        by_name[name].want_rc = rc
    if unknown:
        raise ValueError(f"{w_path}: declarations with no matching entry: {unknown}")


def sidecar_argv_path(src_path):
    """The program-arguments sidecar for a suite is ALWAYS <family>.argv beside <family>.sno/.ref --
    discovered, never passed as a flag, identical to sidecar_in_path()/sidecar_wantrc_path(). Absent ->
    None -> every entry runs with NO arguments, byte-identical to the behaviour before this sidecar
    existed. ⭐ Discovery rather than a flag is the same argument sidecar_wantrc_path() makes and it is
    the load-bearing one here: boards, gates and runners pick the declaration up with ZERO changes to
    their own argv, so a witness whose behaviour depends on its arguments cannot silently be graded
    along its no-arguments path by a caller nobody remembered to update. ⛔⭐ THAT SILENT PATH IS THE
    DEFECT THIS CLOSES, and it does not announce itself: an Icon witness written `n := integer(args[1])
    | 6` runs GREEN today on the `| 6` default, so the suite reports a pass for a program whose
    argument handling was never executed at all -- a green that documents nothing (row
    icon-suite-format-has-no-argv-sidecar-so-argv-taking-witnesses-are-ungradable, census 2026-09-06:
    51 such entries over 445 suites / 7317 entries)."""
    cand = Path(src_path).with_suffix(".argv")
    return str(cand) if cand.is_file() else None


def read_argv_sidecar(a_path, entries, modes=None):
    """Attach declared program arguments to entries BY NAME. Format is one `name<TAB>arg<TAB>arg...`
    per line; blank lines and `#` comments ignored. ⭐ TAB-SEPARATED, WITH NO QUOTING LANGUAGE AT ALL:
    an argument may contain spaces, quotes or backslashes and arrives at the program byte-for-byte, so
    there is no shell-word-splitting layer for a declaration to be misread through -- the same reason
    read_wantrc_sidecar parses `name<TAB>rc` rather than a free-form line.
    ⛔ REFUSES on a declaration naming no entry -- the shape a rename or a half-finished conversion
    produces, and the same refusal read_wantrc_sidecar/read_xfail_sidecar make for the same cause: a
    declaration matching nothing is either a typo or a stale leftover, and both silently withdraw the
    very guarantee the file was written to add.
    ⛔ REFUSES on a name declared with NO arguments: no arguments is already the default, so an empty
    declaration is either a misunderstanding of the format or a no-op a later reader will take for
    something meaningful -- exactly the refusal read_wantrc_sidecar makes for an explicit rc=0.
    ⛔ REFUSES when the family is graded in ast mode ONLY: `--dump-ast` never runs the program, so the
    arguments could not possibly reach it, and a declaration that cannot be executed is a criterion
    that lies. modes is None for callers that do not know them (pin-ref, convert), and those attach
    without the check rather than guessing."""
    if not a_path or not os.path.isfile(a_path):
        return
    if modes is not None and not ({"m3", "m4"} & set(modes)):
        raise ValueError(f"{a_path}: this family is graded in {sorted(modes)} only, and --dump-ast never "
                         f"RUNS the program -- declared arguments could not reach it. Either grade the "
                         f"family in m3/m4 or delete the sidecar; do not keep a declaration nothing executes")
    by_name = {e.name: e for e in entries}
    unknown = []
    for raw in open(a_path, encoding="utf-8"):
        line = raw.rstrip("\n")
        if not line.strip() or line.lstrip().startswith("#"):
            continue
        parts = line.split("\t")
        name, args = parts[0].strip(), parts[1:]
        if not args or all(a == "" for a in args):
            raise ValueError(f"{a_path}: {name!r} declares no arguments, which is already the default -- "
                             f"remove the line rather than restating it")
        if name not in by_name:
            unknown.append(name)
            continue
        by_name[name].argv = args
    if unknown:
        raise ValueError(f"{a_path}: declarations with no matching entry: {unknown}")


def sidecar_xfail_path(src_path):
    """The reason sidecar for a suite is ALWAYS <family>.xfail beside <family>.sno/.ref --
    discovered, never passed as a flag, identical to sidecar_in_path(). Absent -> None."""
    cand = Path(src_path).with_suffix(".xfail")
    return str(cand) if cand.is_file() else None


def read_xfail_sidecar(x_path, banner_re, entries):
    """Attach family.xfail blocks to entries BY NAME. Absent file, or an entry with no block, leaves
    entry.xfail_reason as None. ⛔ REFUSES on a block naming no entry (the shape a rename or a
    half-finished conversion produces) and on a reason attached to an entry NOT marked XFAIL. The
    second refusal is the one that earns its keep: when an XPASS is promoted, the ' XFAIL' comes off
    the banner, and a reason left behind would be a stale explanation for a red that no longer
    exists -- documentation that lies is worse than none, and this makes the promotion prove it is
    complete instead of merely looking complete."""
    if x_path is None or not Path(x_path).is_file():
        return
    lines = Path(x_path).read_text().splitlines()
    by_name, i = {}, 0
    while i < len(lines):
        m = banner_re.match(lines[i])
        if not m:
            raise ValueError(f"expected a banner at {x_path} line {i + 1}: {lines[i]!r}")
        name = m.group("name")
        if name in by_name:
            raise ValueError(f"{x_path}: duplicate reason block for entry {name!r}")
        i += 1
        seg = []
        while i < len(lines) and not banner_re.match(lines[i]):
            seg.append(lines[i]); i += 1
        by_name[name] = "\n".join(seg).strip()
    by_entry = {e.name: e for e in entries}
    unknown = sorted(set(by_name) - set(by_entry))
    if unknown:
        raise ValueError(f"{x_path}: reason blocks with no matching entry: {unknown}")
    stale = sorted(n for n in by_name if not by_entry[n].xfail)
    if stale:
        raise ValueError(f"{x_path}: reason given for entries not marked XFAIL: {stale} -- either the "
                         f"' XFAIL' banner suffix was dropped (restore it) or the entry was promoted "
                         f"after an XPASS (delete its reason block in the same commit)")
    for n, reason in by_name.items():
        by_entry[n].xfail_reason = reason


def write_xfail_sidecar(entries, out_x, comment_open, comment_close):
    """Write family.xfail iff at least one entry carries a reason. Returns True if written.
    ⛔ Refuses a reason on a non-XFAIL entry for the same cause read_xfail_sidecar() refuses it --
    a writer that can emit a file its own reader rejects is not a round trip."""
    withreason = [e for e in entries if e.xfail_reason]
    if not withreason:
        return False
    bad = [e.name for e in withreason if not e.xfail]
    if bad:
        raise ValueError(f"reason given for entries not marked XFAIL: {bad}")
    out = []
    for e in withreason:
        out.append(make_banner_cfg(e.seq, e.name, comment_open, comment_close, xfail=e.xfail))
        out.extend(e.xfail_reason.splitlines())
    Path(out_x).write_text("\n".join(out) + "\n")
    return True


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


def read_suite(sno_path, ref_path, in_path=None, x_path=None, w_path=None, a_path=None, modes=None):
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
            banner_line, name, xfail = line, m.group("name"), bool(m.group("xfail"))
            si += 1
            body = []
            # ⛔⭐ THE FIRST LINE AFTER A BANNER IS ALWAYS BODY — NEVER TESTED AGAINST _is_entry_start.
            # _is_entry_start()'s docstring assumes a block's body lines "do not end this way IN PRACTICE".
            # That assumption holds only until an ALREADY-CONVERTED ONE-LINER IS PROMOTED TO A BLOCK, which is
            # exactly what re-converting an existing suite does: the promoted line still carries its own
            # mandatory `;* <name>` tag, _is_entry_start() fires on it, the block gets an EMPTY body, and the
            # very same line is then re-read as a one-line entry -- so the entry comes back TWICE.
            # MEASURED (hq_C 2026-08-28, root-causing seat02's convert-corrupts-eval report): re-converting
            # probe/eval wrote 21 entries and read back 22, `ev_fn_beauty_shape` duplicated, block body empty.
            # A format-(B) block ALWAYS has at least one body line, so consuming one unconditionally is safe
            # for every suite -- interleaved ones included -- and is what makes promotion round-trip.
            if si < len(sno_lines):
                body.append(sno_lines[si])
                si += 1
            while si < len(sno_lines) and not _is_entry_start(sno_lines[si]):
                body.append(sno_lines[si])
                si += 1
            items.append(("block", name, banner_line, body, xfail))
        else:
            tag_m = ONE_LINE_TAG_RE.search(line)
            name = tag_m.group(1) if tag_m else f"seq{len(items) + 1}"
            items.append(("line", name, line, None, False))
            si += 1

    entries = []
    ri = 0
    seq = 0
    i = 0
    while i < len(items):
        kind, name, a, b, xfail = items[i]
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
        entries.append(Entry("block", seq, name, block_sno, seg[:split], xfail=xfail))
        for k, ref_line in enumerate(seg[split:]):
            _, lname, lraw, _, _ = items[i + 1 + k]
            seq += 1
            entries.append(Entry("line", seq, lname, [lraw], ref_line.replace("\\n", "\n")))
        i = j
    read_stdin_sidecar(in_path, BANNER_RE, entries)
    read_xfail_sidecar(x_path, BANNER_RE, entries)
    read_wantrc_sidecar(w_path, entries)
    read_argv_sidecar(a_path, entries, modes)
    return entries


def read_block_suite(src_path, ref_path, banner_re, in_path=None, x_path=None, w_path=None, a_path=None, modes=None):
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
        xfail = bool(m.groupdict().get("xfail"))
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
        entries.append(Entry("block", seq, name, body, ref_lines[seg_start:ri], xfail=xfail))
    read_stdin_sidecar(in_path, banner_re, entries)
    read_xfail_sidecar(x_path, banner_re, entries)
    read_wantrc_sidecar(w_path, entries)
    read_argv_sidecar(a_path, entries, modes)
    return entries


_INCLUDE_PATTERNS = [
    re.compile(r'\$include\s+"([^"]+)"'),   # Icon
    re.compile(r"-INCLUDE\s+'([^']+)'"),    # SNOBOL4 / Snocone
    re.compile(r'-INCLUDE\s+"([^"]+)"'),    # SNOBOL4 / Snocone, double-quote form
    # ⭐ AN INCLUDE DIRECTIVE IS NOT THE ONLY WAY A PROGRAM NAMES A COMPANION FILE, and the other way
    # fails SILENTLY rather than loudly. Icon's open("X") reads a data file relative to the RUNNING
    # file's directory exactly as $include does, but a missing one does not error -- open() simply
    # fails, the program keeps going, and the entry grades as wrong-output with rc=0. Measured on
    # rung36_jcon_fncs1.icn (tests-consolidate-icon): run from its own directory it is byte-identical
    # to its .expected in BOTH modes; run from an isolated temp dir it prints `----> none` and drops
    # 15 lines of file content, rc=0 throughout. That is the same false-green this whole mechanism
    # exists to stop (the `prepro` $include case, cured one file-reference form over), caught here
    # BEFORE a conversion shipped rather than after one silently failed.
    # ⛔ Safe by construction for write-opens: open("foo.baz","w") names a file that does not exist in
    # the family dir, and the caller only copies a name for which `src_companion.is_file()` holds, so
    # a write target is skipped rather than manufactured.
    re.compile(r'\bopen\s*\(\s*"([^"]+)"'),  # Icon open("X") data companion
    # ⛔⭐ SNOBOL4's INPUT/OUTPUT file-association functions name a companion as a quoted ARGUMENT, not
    # via a directive keyword -- INPUT(.holder, 10, , "name.dat") -- so none of the patterns above ever
    # matched this form at all (row suite-harness-argv-echoes-a-mktemp-path-so-diagnostic-programs-
    # cannot-be-graded, probe/csnobol4_triage/input_eof_hang.sno: convert_one's own candidate ran
    # companion-less in a fresh temp dir and got a DIFFERENT bug -- INPUT() on a MISSING file hangs
    # forever, where the original's genuinely-present-but-empty .dat fails cleanly -- measured, not
    # assumed). Matches either quote style; SNOBOL4 accepts both interchangeably.
    # ⛔⭐ BOTH CHARACTER CLASSES MUST EXCLUDE NEWLINE, AND THE PREFIX MUST BE NON-GREEDY (hq_P seat08
    # 2026-09-04, row snobol4-every-non-package-source-...): the original `[^)]*["\']([^"\']+)["\']` let a
    # SHORT quoted non-filename argument earlier in the same call -- e.g. INPUT(.INPUT, 9, '[-f0 -r4194304]')
    # (a scale/mode-flags string, not a companion file, on a benchmark's INPUT association) -- backtrack PAST
    # its own closing quote, since neither class excludes ')' beyond the immediate one nor '\n': with the
    # flags string's own trailing quote reinterpreted as an OPENING quote, `[^"\']+` then happily swallowed
    # every ')' and newline between it and the NEXT quote anywhere later in the entry (a `TERMINAL = '...'`
    # literal, lines away), capturing hundreds of characters of source as a "companion filename" --
    # OSError: File name too long, the whole board dies. MEASURED: benchmark_calculator-1's absorption
    # (this row) is the first entry to combine this common INPUT(...,'flags') idiom with a later string
    # literal in the same body; excluding '\n' bounds the match to one line (a real filename argument never
    # spans lines) and non-greedy `*?` stops at the FIRST quote pair instead of backtracking past it.
    re.compile(r'\b(?:INPUT|OUTPUT)\s*\([^)\n]*?["\']([^"\'\n]+)["\']'),
]


def _companion_files(text):
    """Filenames an entry names as a companion -- an include directive (Icon `$include "X"`,
    SNOBOL4/Snocone `-INCLUDE 'X'`) or a literal data open (Icon `open("X")`). All of these resolve
    relative to the RUNNING file's directory, which a suite entry materialized into an isolated temp
    dir does not share with its original family_dir unless a caller copies the referenced file in
    first (see run_suite_entry's companion_dir). ⛔ The two kinds fail DIFFERENTLY when the companion
    is absent: an include errors out loudly, a data open just fails and lets the program run on to a
    wrong answer with rc=0 -- so the open case is the one that silently manufactures a green."""
    names = []
    for pat in _INCLUDE_PATTERNS:
        names.extend(pat.findall(text))
    return names


def _copy_companions(text, companion_dir, dest_dir):
    # ⭐ COMPANION $include/-INCLUDE/open()/INPUT()/OUTPUT() FILES (tests-consolidate-icon,
    # rung36_jcon_prepro; extended probe-consolidate-m1-and-small, gim_double_include_once_control;
    # extended again, row suite-harness-argv-echoes-a-mktemp-path-so-diagnostic-programs-cannot-be-
    # graded): a candidate runs from a FRESH temp dir, never the family_dir its original loose file
    # lived in, so an include/open/INPUT()/OUTPUT() directive naming a real, present companion file
    # fails to resolve there even though the file exists and the entry is otherwise green. Concretely:
    # probe/csnobol4_triage/input_eof_hang.sno's empty .dat companion -- present, its INPUT() fails
    # cleanly (rc=0, no output); absent, INPUT() never signals failure and the SAME program HANGS
    # (its own original, pre-reclassification defect, back in full, silently, inside convert_one's own
    # candidate-verification). A caller that knows where the family's companion files live (family_dir
    # at conversion time, or the suite file's own directory at grading time -- they are the same
    # directory by convention) passes companion_dir; a missing or unreferenced companion is a silent
    # no-op here and surfaces as an ordinary run FAILURE downstream, not a special error -- the same as
    # any other missing dependency a program tries to open. Shared by every caller that materializes a
    # candidate into an isolated temp dir (run_suite_entry, convert_one) so the copy logic -- including
    # the two guards below, each hardened by a real prior failure -- never drifts between them.
    if not companion_dir:
        return
    # ⭐ config/ SUBFOLDER (Lon 2026-08-29 zero-subfolders + "make a config folder for those files"): the flat
    # test dir keeps ONLY test sources; runtime companions (includes, .dat/.in data, tracepoint .conf) live in
    # <dir>/config. Search BOTH -- a name found in the flat dir wins (older layouts unchanged).
    # ⛔⭐⭐ AND THE FAILURE MODE OF GETTING THIS WRONG IS A HANG, NOT A RED ENTRY (hq_C, measured 2026-08-29).
    # probe/csnobol4_triage/input_eof_hang.sno exits rc=0 cleanly with its .dat present; with the .dat ABSENT,
    # INPUT() never signals failure and THE SAME PROGRAM HANGS. So a companion the search cannot find does not
    # produce four red entries somebody notices -- it produces a board that STOPS, which reads as
    # infrastructure trouble and sends the next person to the runner instead of to the corpus.
    # ⭐ The measurement behind this branch is hq_C's controlled A/B on the snobol4 master, one variable
    # (companion location): companions BESIDE -> m3 1421 pass / 0 fail; the same 16 in config/ WITHOUT this
    # search -> 1417 / 4 fail; WITH it -> 1421 / 0, re-run independently by hq_B with the beside-arm
    # reproducing 1421/0 first as the positive control for the experiment itself.
    _cfg = Path(companion_dir) / "config"
    companion_dirs = [Path(companion_dir)] + ([_cfg] if _cfg.is_dir() else [])
    import shutil
    # ⭐ TRANSITIVE CLOSURE, NOT ONE LEVEL (row harness-copies-companions-to-closure-not-just-the-first-
    # level, hq_T 2026-09-05, on seat06's FINDING-2026-09-04-seat06-corpus-suite-harness-transitive-
    # include-companions-not-copied.md): a companion this function just copied may itself -INCLUDE/open()
    # a companion of its own -- entry includes A, A includes B -- and a single pass over the entry's own
    # text only ever resolved A, leaving B absent from the isolated dest_dir. `queue` is a worklist seeded
    # from the entry's own text and grown by re-scanning each newly-copied file's own text for ITS
    # companions, to a fixed point (closure) -- a 2-level, 3-level or N-level chain all resolve the same
    # way, not just depth-2 (picking a fixed depth would leave depth-3 to be found the same way a month
    # later). `seen` is the cycle guard: a name is popped and dispositioned AT MOST ONCE, so a file that
    # includes itself, or two files that include each other, terminate on the repeat name instead of
    # spinning forever.
    # ⛔ REFUSAL-SAFE AT EVERY LEVEL: a name that resolves to no file in companion_dirs is the same silent
    # no-op it always was (the entry's own run surfaces the miss downstream as an ordinary missing-
    # dependency failure) -- this loop never invents a substitute and never skips scanning a reachable
    # level just because an earlier level's file was missing, so an unresolvable second-level include
    # cannot manufacture a false green one level down the way a naive "stop at the first miss" would.
    seen = set()
    queue = list(_companion_files(text))
    while queue:
        name = queue.pop(0)
        if name in seen:
            continue
        seen.add(name)
        # an ABSOLUTE reference is the program's own scratch path (e.g. /tmp/rung37_fh_test.txt),
        # not a companion in the family dir -- Path(dir)/absolute RETURNS the absolute path for
        # both src and dst, so the copy is file-onto-itself: SameFileError, suite dies boardless
        # (witness: rung37_all after the icon suite conversion, false FAIL on the icon board)
        if Path(name).is_absolute():
            continue
        # ⛔⭐ SEARCH <dir>/config TOO, FOR THE FLAT END STATE (Lon via ceo, 2026-08-29): tests/<lang>/ becomes
        # FLAT -- ALL.{ext,ref,in,csv} side by side, with ZERO subdirectories except ONE `config/` folder
        # holding runtime companions (.inc/.dat/.in/.conf) and docs. Once a family's own directory is gone,
        # its companions live there and NOWHERE ELSE, so a copier that only looks beside the suite file finds
        # nothing and the entry fails as an ordinary missing-dependency error -- indistinguishable from a real
        # defect. Additive and beside-first (the loop breaks on the first hit), so a tree that has NOT
        # flattened yet behaves exactly as before. (merged: hq's config search + ceo's ..-guard/mkdir/loop)
        if ".." in Path(name).parts:
            continue  # a scratch-escaping relative reference (e.g. ../../benchmarks/x.inc) cannot be a companion of an isolated entry; the entry's own run surfaces the miss
        for _cd in companion_dirs:
            src_companion = _cd / name
            dst_companion = Path(dest_dir) / name
            if src_companion.is_file() and not (dst_companion.exists() and src_companion.samefile(dst_companion)):
                dst_companion.parent.mkdir(parents=True, exist_ok=True)
                shutil.copy(src_companion, dst_companion)
                try:
                    queue.extend(_companion_files(src_companion.read_text()))
                except (OSError, UnicodeDecodeError):
                    pass
                break


def run_suite_entry(paths, entry, tmp_root, modes, ext=".sno", companion_dir=None):
    with tempfile.TemporaryDirectory(dir=tmp_root) as td:
        cand = Path(td) / f"{entry.name}{ext}"
        if entry.kind == "line":
            text = entry.sno_lines[0] + "\n"
            expected = entry.ref
        else:
            text = "\n".join(entry.sno_lines) + "\n"
            expected = "\n".join(entry.ref)
        # ⛔⭐ entry.name CAN CARRY A '/' (util_build_package_suite.py qualifies a nested source as
        # "parentdir/stem", e.g. arizona_tests' "general/args") -- the first package whose entries are
        # actually EXECUTED through this exact function to carry one (csnobol4_suite's nested-name
        # collision fix only touched CSV/naming, never grading, since its own nested file was excluded).
        # Without this, cand.write_text() raises FileNotFoundError: the intermediate directory under the
        # fresh tempdir was never created. A no-op for every flat entry name (parents=True on an
        # already-existing Path(td) is a safe no-op), so nothing already grading through this function
        # can regress.
        cand.parent.mkdir(parents=True, exist_ok=True)
        cand.write_text(text)
        _copy_companions(text, companion_dir, Path(td))
        return run_all_modes(paths, cand, expected, Path(td), modes, stdin_text=entry.stdin,
                             want_rc=getattr(entry, 'want_rc', 0), prog_argv=getattr(entry, 'argv', None))


# ================================================================== CLI ===
def cmd_capture_oracle_refs(args):
    """For a family with NO committed .ref files at all (oracle-graded loose files, e.g.
    probe/conformance -- see corpus-suites-consolidation task, probe-consolidate-conformance):
    synthesizes a .ref for every source file whose CURRENT m3 AND m4 output+returncode both agree
    with a FRESH live run of the correctness oracle (SPITBOL `-bf` by default, or --lang prolog's
    swipl), mirroring test_one_witness.sh's exact contract. Writes nothing for a stem that
    disagrees, that already has a .ref (unless --force), or where the oracle itself doesn't run
    cleanly -- those stay exactly as they are on disk, never touched. This is a ONE-TIME bootstrap
    step: once a .ref exists, the family behaves exactly like every other suite family (cmd_convert/
    convert-blocks reads a static .ref, same as always) -- grading never re-invokes the oracle after
    this point, matching how every other family's .ref is a frozen snapshot, not a live re-check on
    every run. --lang defaults to snobol4 (unchanged behavior, glob *.sno) for every existing
    caller; passing --lang prolog globs *.pl and captures against swipl instead (2026-08-29, see
    tests-consolidate-prolog's ~77-file no-.ref backlog -- the gap this parameter closes)."""
    paths = resolve_paths()
    check_scrip(paths)
    lang = args.lang or "snobol4"
    ext = LANG_CONFIGS[lang]["ext"] if lang != "snobol4" else ".sno"
    oracle_bin, flags = resolve_oracle_bin(paths, lang)
    print(f"oracle: {oracle_bin} {flags}", file=sys.stderr)
    modes = args.modes.split(",")
    family_dir = Path(args.family_dir).resolve()   # ⛔ ABSOLUTE, ALWAYS (ceo s283h): run_m3/run_ast set subprocess cwd to the FILE'S OWN PARENT while argv carries this path -- a caller-relative path then resolves against the wrong dir, scrip cannot open it (rc=1), and every 'original' baseline is silently garbage: convert then REFUSES green families ('NEITHER form reproduced') or, worse, validates a candidate against the broken baseline. Measured on probe/kw (15/15 false FAIL) and probe/define (false-validated) 2026-08-29.
    srcs = sorted(family_dir.glob(f"*{ext}"))
    if not srcs:
        refuse(f"no {ext} files found under {family_dir}")

    green, red = [], []
    # ⛔⭐ A SCRIP-RULED PIN IS NOT THE ORACLE'S TO OVERWRITE (row/ask hq_C -> hq_T 2026-09-04). `pin-ref`
    # records, in ALL.refpins.tsv beside the master, every entry whose ref disagrees with the oracle ON A
    # STATED RULING -- the witness being Lon's lambda-deferred-target sugar, which SPITBOL fails and always
    # will. Without this consultation `--force` would faithfully restore the oracle's answer and silently undo
    # a decision nobody remembers making; the refusal is what turns the ledger from documentation into a
    # mechanism. ⭐ It refuses rather than skipping quietly, because "I left this alone" and "I never looked"
    # are the same output otherwise.
    _pins = read_refpins(getattr(args, "family_dir", "") and Path(args.family_dir) / "ALL.sno" or "")
    for i, src in enumerate(srcs, 1):
        ref_path = src.with_suffix(".ref")
        if src.stem in _pins:
            refuse(f"{src.stem} carries a SCRIP-RULED ref pin ({_pins[src.stem]['ruling']!r}, "
                   f"{_pins[src.stem]['measurer']} {_pins[src.stem]['date']}) -- capturing the oracle's answer "
                   f"here would undo that ruling. Re-rule it with `pin-ref` if it is wrong, or exclude this "
                   f"stem from the capture.")
        if ref_path.is_file() and not args.force:
            print(f"[{i}/{len(srcs)}] {src.stem}: SKIP (already has a .ref)", file=sys.stderr)
            continue
        # ⛔⭐⭐ THE STDIN COMPANION IS FED TO ALL THREE ARMS -- IT USED TO BE A FLAT REFUSAL, AND THE REASON
        # THE REFUSAL EXISTED IS THE REASON THE FEED HAS TO BE PROVEN (ceo freeze 2026-08-30 on hq_P's catch;
        # freeze lifted with this one edge, cured here). This gate's whole safety argument is THREE-WAY
        # AGREEMENT -- m3, m4 and the oracle producing the same bytes. That argument COLLAPSES when the
        # disagreement-producing input is missing from all three arms at once: rung36_jcon_recogn ran with no
        # stdin, ALL THREE ARMS AGREED ON EMPTY, and a 1-byte vacuous oracle was minted that would have passed
        # forever (withdrawn, corpus 705cd7ad1).
        # ⭐ THE GENERAL FORM, AND IT IS WHY N ARMS DID NOT HELP: AGREEMENT IS ONLY EVIDENCE WHEN THE ARMS CAN
        # DISAGREE. Three instruments sharing one missing input are one instrument reported three times, and
        # the unanimity reads as MORE confidence rather than less. A fourth arm would not have helped.
        # ⛔ SO FEEDING IS NOT ENOUGH -- AN UNPROVEN FEED IS THE SAME DEFECT WITH BETTER MANNERS. If the text
        # never reaches the program (a plumbing regression here, a dialect whose runner drops stdin, a
        # companion that is really a by-name data file), all three arms are once again sharing one missing
        # input and once again agreeing. The unfed control run below is what makes the feed FALSIFIABLE, and
        # it is the only part of this block that can detect a future regression in the feed itself.
        # ⛔ The old REFUSAL survives, unchanged in spirit, for every path that CANNOT feed -- see
        # loose_stdin_companion(): ambiguity and non-UTF-8 mint nothing and say why. A vacuous ref is worse
        # than a missing one: a missing ref leaves a file ungraded and VISIBLE, while a vacuous ref grades it
        # against nothing forever and reads as coverage.
        stdin_text, stdin_path, stdin_refusal = loose_stdin_companion(src)
        if stdin_refusal:
            red.append((src.stem, "REFUSED: " + stdin_refusal))
            print(f"[{i}/{len(srcs)}] {src.stem}: ⛔ REFUSED ({stdin_refusal.split(' -- ')[0]})", file=sys.stderr)
            continue
        ora_text, ora_rc, ora_kind = run_oracle(oracle_bin, flags, src, paths["timeout"], stdin_text=stdin_text)
        if ora_kind != "RAN":
            red.append((src.stem, f"oracle itself {ora_kind}"))
            print(f"[{i}/{len(srcs)}] {src.stem}: RED (oracle {ora_kind})", file=sys.stderr)
            continue
        # ⭐ THE CONTROL ARM (format Law 4: agreement only counts when the arms CAN disagree). One extra
        # oracle run, unfed, on stdin-bearing stems only -- rare, so the cost is noise. If feeding the
        # companion changes NOTHING about the oracle's kind/text/rc, then this stem's stdin is not reaching
        # the program and the three-way agreement about to be computed is agreement on a shared missing
        # input all over again. Two real causes, one signature: the feed is broken, or the file named
        # <stem>.in/.input is not stdin at all but a by-name data file that merely collides with the stdin
        # spelling. ⛔ Both want a human, and neither wants a minted ref. A HANG or a different rc counts as
        # a difference -- a program that blocks unfed and runs fed is exactly a stem that IS stdin-dependent.
        if stdin_text is not None:
            unfed_text, unfed_rc, unfed_kind = run_oracle(oracle_bin, flags, src, paths["timeout"])
            if (unfed_kind, unfed_text, unfed_rc) == (ora_kind, ora_text, ora_rc):
                red.append((src.stem, "REFUSED: feeding %s changed NOTHING in the oracle (same kind/text/rc "
                                      "fed and unfed) -- either stdin is not reaching this program or that "
                                      "file is a by-name data companion, not stdin; agreement here would "
                                      "again be agreement on a shared missing input" % stdin_path.name))
                print(f"[{i}/{len(srcs)}] {src.stem}: ⛔ REFUSED (feeding {stdin_path.name} changed nothing)", file=sys.stderr)
                continue
        agreements = []
        all_agree = True
        for m in modes:
            if m == "m3":
                v = run_m3(paths, src, ora_text, stdin_text=stdin_text)
            elif m == "m4":
                with tempfile.TemporaryDirectory() as td:
                    v = run_m4(paths, src, ora_text, Path(td), stdin_text=stdin_text)
            else:
                refuse(f"unknown mode {m!r} (this command supports m3/m4 only, never ast)")
            agree = (v.kind == "PASS") and (v.returncode == ora_rc)
            agreements.append(f"{m}={'AGREE' if agree else f'{v.kind}(rc={v.returncode} vs oracle {ora_rc})'}")
            all_agree = all_agree and agree
        # ⛔⭐⭐ REFUSE TO MINT AN EMPTY REF, WHATEVER THE REASON. Two independent routes to the same failure
        # were found on 2026-08-30, hours apart, by two different seats:
        #   hq_P  -- rung36_jcon_recogn ran with NO STDIN; all three arms agreed on empty.
        #   seat05 -- swipl's `-g halt` fires before/instead of a `:- initialization(main,main)` goal, so the
        #             ORACLE produces empty for a whole class of prolog programs that run fine bare.
        # Different causes, IDENTICAL SIGNATURE: every arm agrees, and what they agree on is NOTHING.
        # ⭐ So the guard belongs on the SIGNATURE, not on either cause. An existence check for a stdin
        # companion cannot see seat05's case (there is no companion), and a swipl-flag fix cannot see hq_P's.
        # Empty-agreement is the observable both share, and it will be the observable of the third route
        # nobody has found yet. It is ALSO the backstop for the feed above: if a future change breaks the
        # plumbing on a path the unfed control does not cover, this is what still refuses to write the ref.
        # ⛔ A legitimately-silent program is the acceptable cost: its ref is one line to author deliberately,
        # whereas a vacuous ref grades its file against nothing FOREVER and reads as coverage. Refusing costs
        # a human a minute; minting costs a suite its meaning, silently and permanently.
        # ⛔⭐ RESTORED 2026-08-30 (hq_B) AFTER BEING DELETED WITH NO REPLACEMENT by d67c0f6c, whose subject
        # and body describe three unrelated builder fixes and never mention removing a guard. It came back
        # only because a rebase conflict one hunk ABOVE forced someone to read this function -- the deletion
        # itself merged perfectly clean. ⭐ That is the durable lesson: a removed check has no failing test
        # to announce it. Nothing goes red, the tool gets quieter, and the loss is only visible to a reader
        # who already knew the check was supposed to be here.
        if all_agree and not ora_text.strip():
            red.append((src.stem, "REFUSED: all arms agreed on EMPTY output -- agreement on nothing is not "
                                  "agreement; mint a ref by hand if this program is genuinely silent"))
            print(f"[{i}/{len(srcs)}] {src.stem}: ⛔ REFUSED (all arms agree, but on EMPTY output)", file=sys.stderr)
            continue
        if all_agree:
            ref_path.write_text(ora_text + "\n")
            green.append(src.stem)
            _fed = f" [stdin: {stdin_path.name}]" if stdin_text is not None else ""
            print(f"[{i}/{len(srcs)}] {src.stem}: GREEN ({' '.join(agreements)}){_fed} -- .ref written", file=sys.stderr)
        else:
            red.append((src.stem, " ".join(agreements)))
            print(f"[{i}/{len(srcs)}] {src.stem}: RED ({' '.join(agreements)})", file=sys.stderr)

    print(f"\n{len(green)} green (.ref written, ready for `convert`) -- {len(red)} red (untouched, no .ref):",
          file=sys.stderr)
    for name, reason in red:
        print(f"   RED {name}: {reason}", file=sys.stderr)
    # ⛔⭐⭐ ANY RED EXITS NON-ZERO. This printed a full RED summary and exited 0 until 2026-08-30 (measured by
    # ceo on a scratch family; hq_B's row capture-feed-stdin-and-red-exit) -- which is format Law 3's exact
    # two-audiences shape, "could not measure" and "measured fine" sharing one exit code, sitting on the very
    # tool that carries Law 4. ⭐ A HUMAN reading the terminal sees the ⛔ lines and is fine; a SCRIPT sees
    # rc=0 and marches on, and `capture-oracle-refs D && convert D ...` then converts a family whose refusals
    # it never learned about. The two audiences do not read the same channel, so the channel BOTH read -- the
    # exit code -- is the one that has to carry the verdict.
    # rc=1 -- ran fine, some stems are RED and were left untouched. rc=2 (refuse()) is could-not-measure-AT-ALL,
    # ⛔ UNIFIED FROM rc=3 on 2026-09-04 (ceo CEO-233, row harness-refusal-exit-code-unified-on-rc-2) onto the
    # fleet's one refusal code -- see refuse()'s own docstring. The distinction this comment was defending is
    # intact: rc=1 still means "measured, some stems red", rc=2 now means "could not measure", and they are still
    # different codes. Only the NUMBER moved, onto the one every bash caller and lib_gate.sh already test for.
    sys.exit(1 if red else 0)


def cmd_convert(args):
    paths = resolve_paths()
    check_scrip(paths)
    modes = args.modes.split(",")
    family_dir = Path(args.family_dir).resolve()   # ⛔ ABSOLUTE, ALWAYS (ceo s283h): run_m3/run_ast set subprocess cwd to the FILE'S OWN PARENT while argv carries this path -- a caller-relative path then resolves against the wrong dir, scrip cannot open it (rc=1), and every 'original' baseline is silently garbage: convert then REFUSES green families ('NEITHER form reproduced') or, worse, validates a candidate against the broken baseline. Measured on probe/kw (15/15 false FAIL) and probe/define (false-validated) 2026-08-29.
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
    # ⭐ Resolved ONCE, up front, through the single loose-companion authority -- and kept, because the
    # same text has to reach three separate places that would otherwise each re-derive it: convert_one's
    # original+candidate runs, the .in sidecar, and the on-disk re-validation below.
    stdin_by_stem = {}
    for sno, _ref in pairs:
        _text, _path, _refusal = loose_stdin_companion(sno)
        if _refusal:
            refuse(f"{sno.name}: {_refusal}")
        stdin_by_stem[sno.stem] = _text
    _fed_n = sum(1 for v in stdin_by_stem.values() if v is not None)
    if _fed_n:
        print(f"{_fed_n} of {len(pairs)} pairs carry a stdin companion -- fed to every arm and written to the .in sidecar", file=sys.stderr)
    tmp_root = Path(tempfile.mkdtemp(prefix="csh_"))
    try:
        for seq, (sno, ref) in enumerate(pairs, start=1):
            entry, report = convert_one(paths, sno, ref, seq, tmp_root, modes, companion_dir=sno.parent,
                                        stdin_text=stdin_by_stem[sno.stem])
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
    # out_in is DERIVED, never a flag -- sidecar_in_path()'s own doctrine: every downstream consumer
    # (boards, gates, cmd_run) discovers <family>.in by convention, so a suite cannot silently run
    # without its input just because one caller was not taught a new argv.
    out_in = str(Path(args.out_sno).with_suffix(".in"))
    wrote_in = write_suite(entries, args.out_sno, args.out_ref, out_in=out_in)
    print(f"✅ wrote {args.out_sno} / {args.out_ref}{' / ' + out_in if wrote_in else ''}: {len(entries)} entries "
          f"({one_line_n} one-line, {block_n} multi-line-block)", file=sys.stderr)

    # close the loop: re-read the WRITTEN files and re-validate every entry against the originals
    # ⛔ MATCH BY NAME, NEVER BY POSITION (seat06 2026-08-29): write_suite() deliberately reorders
    # entries (all format-A lines first, then all format-B blocks -- see its own docstring) for a
    # real, previously-fixed correctness reason, so `reread`'s order does not track `pairs`'
    # discovery order (alphabetical) whenever a batch mixes line and block entries. A positional
    # `zip(pairs, reread)` then silently cross-compares UNRELATED entries -- two genuinely-correct
    # conversions can each get reported as a false "ON-DISK RE-VALIDATION FAILED" simply because a
    # third entry's kind shuffled the read-back order. Confirmed on a 3-entry repro (fence_arbno_top
    # [line], ident_differ_inline [block], os1_runtime_k [line] -- discovery order != reread order,
    # and the loop below used to compare ident's own orig output against os1's suite output).
    # ⛔ READ THE .in BACK TOO (sidecar_in_path, not out_in: if no entry carried stdin the file was never
    # created and is_file() correctly degrades to None -> /dev/null). Re-validating without it would run
    # every suite entry unfed against a ref minted fed -- the re-validation would then be measuring a
    # different program than the one just written, which is the whole failure this parameter closes.
    reread = read_suite(args.out_sno, args.out_ref, in_path=sidecar_in_path(args.out_sno))
    by_name = {e.name: e for e in reread}
    tmp_root = Path(tempfile.mkdtemp(prefix="csh_verify_"))
    mismatches = []
    try:
        for sno, ref in pairs:
            written = by_name[sno.stem]
            orig_verdicts = run_all_modes(paths, sno, ref.read_text(), tmp_root, modes,
                                           stdin_text=stdin_by_stem[sno.stem])
            suite_verdicts = run_suite_entry(paths, written, tmp_root, modes, companion_dir=sno.parent)
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

    # ⛔ THE COUNT NAMES THE COMPANIONS TOO. A stdin-bearing stem is THREE loose files, not two, and its
    # companion's content is only safe to delete because it now lives in the .in sidecar just written and
    # re-validated. Saying "N*2" while a third file sits there is how a companion gets left behind as
    # orphaned litter -- or, worse, read as still-load-bearing and the conversion reverted.
    print(f"✅ ON-DISK RE-VALIDATION PASSED: all {len(entries)} entries byte-equal, both directions, "
          f"modes={modes}. Safe to delete the {len(pairs)*2 + _fed_n} original files "
          f"({len(pairs)} .sno + {len(pairs)} .ref"
          f"{f' + {_fed_n} stdin companion(s), now carried by ' + Path(out_in).name if _fed_n else ''}).",
          file=sys.stderr)
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
    family_dir = Path(args.family_dir).resolve()   # ⛔ ABSOLUTE, ALWAYS (ceo s283h): run_m3/run_ast set subprocess cwd to the FILE'S OWN PARENT while argv carries this path -- a caller-relative path then resolves against the wrong dir, scrip cannot open it (rc=1), and every 'original' baseline is silently garbage: convert then REFUSES green families ('NEITHER form reproduced') or, worse, validates a candidate against the broken baseline. Measured on probe/kw (15/15 false FAIL) and probe/define (false-validated) 2026-08-29.
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
    # ⭐ --xfail (hq_P 2026-08-29, row corpus-crosscheck-probe-total-conversion): format (B) had NO way to
    # carry a KNOWN-RED witness into a suite, while format (A)'s convert_one() has marked xfail=not orig_green
    # since the xfail/xpass extension landed. That asymmetry was not a policy against XFAIL -- this function's
    # own docstring only rules out an AUTOMATIC skip-if-red -- but it collided head-on with Lon's 2026-08-28
    # total-conversion ruling: crosscheck/ must GO, convert-blocks refused every red, so 20 real witnesses had
    # no path into a suite and no path out of the tree. An explicit, named, reasoned --xfail is the same
    # discipline --skip already enforces, and it restores parity with the format law, which names an
    # "XFAIL witness" as a first-class format-(B) block.
    xfail_names = set(n for n in (args.xfail.split(",") if getattr(args, "xfail", "") else []) if n)
    if xfail_names:
        unmatched_x = xfail_names - {p[0].stem for p in pairs}
        if unmatched_x:
            refuse(f"--xfail named stem(s) not found among convertible pairs in {family_dir}: {sorted(unmatched_x)}")
    failures = []
    tmp_root = Path(tempfile.mkdtemp(prefix="csh_blocks_"))
    try:
        for seq, (src, ref) in enumerate(pairs, start=1):
            name = src.stem
            expected_text = ref.read_text()
            # ⭐ .stdin SIDECAR (hq_C 2026-08-28 ruling, q-harness-gap-lang-convert-blocks-no-stdin):
            # a loose <stem>.stdin beside <stem>.icn/.expected feeds that one entry's stdin, mirroring
            # the family-level .in sidecar this same content becomes once written (write_block_suite's
            # out_in= below) -- discovered by convention, never a flag, same principle as
            # sidecar_in_path(). Absent file -> None -> /dev/null, identical to pre-stdin behaviour.
            # ⛔ THROUGH THE ONE RESOLVER (hq_B 2026-08-30). This read `.stdin` and only `.stdin`, so a
            # loose `.in`/`.input` -- the spelling the snobol4 corpus actually uses -- resolved to None,
            # ran the entry against /dev/null, and scored the result as a genuine verdict.
            stdin_text, stdin_path, _stdin_refusal = loose_stdin_companion(src)
            if _stdin_refusal:
                refuse(f"{src.name}: {_stdin_refusal}")
            orig = run_all_modes(paths, src, expected_text, tmp_root, modes, stdin_text=stdin_text)
            orig_green = all(v.kind == "PASS" for v in orig.values())
            want_xfail = name in xfail_names
            # ⛔ AN --xfail STEM THAT IS ACTUALLY GREEN IS AN XPASS, AND XPASS IS AS ACTIONABLE AS A FAILURE
            # (RULES; and measured -- 3 of the 4 .xfail markers in crosscheck/snocone were STALE, their bugs
            # long fixed and nobody promoted the marker). Refusing here is what stops a stale claim being
            # laundered into a suite banner where it would then be believed.
            if want_xfail and orig_green:
                failures.append((name, "--xfail names this stem but the ORIGINAL IS GREEN (XPASS) -- the marker is stale; drop it from --xfail"))
                print(f"[{seq}/{len(pairs)}] {name}: FAIL (XPASS -- --xfail names a green original)", file=sys.stderr)
                continue
            if not orig_green and not want_xfail:
                failures.append((name, f"original file itself is not green: {orig}"))
                print(f"[{seq}/{len(pairs)}] {name}: FAIL (original not green)", file=sys.stderr)
                continue
            body = src.read_text().splitlines()
            # ⛔⭐ NOT .rstrip("\n").splitlines() (row icon-construct-ladder-from-rung-0, seat01): rstrip("\n")
            # deletes EVERY trailing newline, not just the file-ending one, so an entry whose real expected
            # output ends in a genuine blank line (e.g. Icon `write(&subject)` after &subject reverts to "")
            # silently loses it here even though orig_green above was computed against the FULL expected_text
            # and validated correctly -- a green witness materializes into the suite with a truncated .ref.
            # Caught on rung05_scan_scan_nested: real oracle output "second\n\n", this line wrote "second\n".
            # splitlines() alone (no rstrip) is the correct one-trailing-newline-is-not-a-line convention:
            # "second\n\n".splitlines() == ["second", ""] (blank line preserved), "x\n".splitlines() == ["x"]
            # (no phantom blank), matching body's own convention two lines above.
            ref_body = expected_text.splitlines()
            entries.append(Entry("block", len(entries) + 1, name, body, ref_body, stdin=stdin_text, xfail=want_xfail))
            print(f"[{seq}/{len(pairs)}] {name}: OK{' XFAIL' if want_xfail else ''}{' (stdin)' if stdin_text is not None else ''}", file=sys.stderr)
    finally:
        import shutil
        shutil.rmtree(tmp_root, ignore_errors=True)

    if failures:
        print(f"⛔ {len(failures)} entries FAILED (original not green) -- NOTHING WRITTEN, NOTHING DELETED:", file=sys.stderr)
        for name, reason in failures:
            print(f"   {name}: {reason[:120]}", file=sys.stderr)
        sys.exit(1)

    out_in = str(Path(args.out_src).with_suffix(".in"))
    wrote_in = write_block_suite(entries, args.out_src, args.out_ref, comment_open, comment_close,
                      out_in=out_in,
                      out_x=str(Path(args.out_src).with_suffix(".xfail")) if any(e.xfail_reason for e in entries) else None)
    print(f"✅ wrote {args.out_src} / {args.out_ref}"
          f"{' / ' + out_in if wrote_in else ''}: {len(entries)} entries", file=sys.stderr)

    banner_re = banner_re_for(comment_open, comment_close)
    # sidecar_in_path (not out_in directly): if wrote_in is False -- no entry carried stdin -- the file
    # was never created, and sidecar_in_path's is_file() check correctly degrades to None (-> /dev/null),
    # same auto-discovery cmd_run's --lang path already uses (ZERO argv changes for any caller).
    reread = read_block_suite(args.out_src, args.out_ref, banner_re,
                              in_path=sidecar_in_path(args.out_src), x_path=sidecar_xfail_path(args.out_src), w_path=sidecar_wantrc_path(args.out_src),
                              a_path=sidecar_argv_path(args.out_src))
    # ⛔ MATCH BY NAME, NEVER BY POSITION -- same fix as cmd_convert, applied defensively here too
    # (format-B-only means this path is not currently exposed to write_suite's line-then-block
    # reorder, but a positional zip between two independently-built lists is the same latent class
    # of bug regardless; see cmd_convert's comment for the confirmed repro).
    by_name = {e.name: e for e in reread}
    tmp_root = Path(tempfile.mkdtemp(prefix="csh_blocks_verify_"))
    mismatches = []
    try:
        for src, ref in pairs:
            written = by_name[src.stem]
            # ⛔ hq_C 2026-08-28: diff STDOUT, never verdict kind alone -- re-derive the ORIGINAL's
            # stdin independently from its own loose .stdin sidecar (not from `written.stdin`), so a
            # round-trip that silently lost or corrupted the sidecar produces a genuine behavioral
            # mismatch here instead of two coincidentally-identical verdicts.
            # ⛔ THE FOURTH COPY OF THE SPELLING LIST LIVED HERE, AND ONLY A CONTROL FOUND IT (hq_B
            # 2026-08-30). This re-derivation is deliberately independent of `written.stdin` -- that is
            # hq_C's design above and it is kept exactly -- but it knew `.stdin` alone, so a `.input`-bearing
            # entry converted correctly, wrote a correct .in sidecar, and was then re-validated against an
            # UNFED original: orig FAIL vs suite PASS, reported as "the written suite diverges" when the
            # written suite was right and the check was wrong. ⭐ A false ⛔ is not the harmless direction of
            # this bug -- it tells a seat to distrust a good conversion and go re-derive a correct .ref.
            # Independence is preserved by reading the loose file FRESH, not by keeping a private list.
            orig_stdin_text, _osp, _osr = loose_stdin_companion(src)
            if _osr:
                refuse(f"{src.name}: {_osr}")
            orig_verdicts = run_all_modes(paths, src, ref.read_text(), tmp_root, modes, stdin_text=orig_stdin_text)
            suite_verdicts = run_suite_entry(paths, written, tmp_root, modes, ext=ext, companion_dir=src.parent)
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

    _fed_n = sum(1 for e in entries if e.stdin is not None)
    print(f"✅ ON-DISK RE-VALIDATION PASSED: all {len(entries)} entries byte-equal, both directions, "
          f"modes={modes}. Safe to delete the {len(pairs) * 2 + _fed_n} original files "
          f"({len(pairs)} {ext} + {len(pairs)} .ref"
          f"{f' + {_fed_n} stdin companion(s), now carried by ' + Path(out_in).name if _fed_n else ''}) "
          f"({len(failures)} left as loose, not green, not deleted).", file=sys.stderr)
    sys.exit(0)


def cmd_run(args):
    paths = resolve_paths()
    _progress_pin(paths)
    check_scrip(paths)
    # ⛔⭐ A SUITE FILE THAT IS NOT THERE IS A REFUSAL, NEVER A CRASH (row harness-refusal-exit-code-unified-on-
    # rc-2, hq_T 2026-09-04, found by that row's OWN gate while it was being written). Until now a missing
    # ALL.<ext> or ALL.ref reached Path.read_text() and died with a FileNotFoundError TRACEBACK, which Python
    # exits **1** for -- and rc=1 in this harness means "ran fine, some entries are RED". So the one case where
    # nothing whatsoever was graded returned the code for a measured red board, to a caller that cannot tell
    # them apart. That is the exact shape this row was minted to remove, one layer below where it was looking.
    # ⛔ CHECKED HERE, BEFORE ANY READER: read_suite/read_block_suite each open BOTH files at different depths,
    # so a guard inside either one would have to be written twice and would still miss the sidecars' own reads.
    for _label, _p in (("suite", args.sno), ("ref", args.ref)):
        if not Path(_p).is_file():
            refuse(f"{_label} file does not exist: {_p} -- nothing was graded, and a run that graded nothing is "
                   f"UNMEASURED, never a red board (check the path, or build the master first)")
    if args.lang:
        cfg = LANG_CONFIGS[args.lang]
        ext = cfg["ext"]
        modes = (args.modes or cfg["modes"]).split(",")
        banner_re = banner_re_for(cfg["comment_open"], cfg["comment_close"])
        entries = read_block_suite(args.sno, args.ref, banner_re, in_path=sidecar_in_path(args.sno),
                                   x_path=sidecar_xfail_path(args.sno), w_path=sidecar_wantrc_path(args.sno),
                                   a_path=sidecar_argv_path(args.sno), modes=modes)
    else:
        ext = ".sno"
        modes = (args.modes or "m3,m4").split(",")
        entries = read_suite(args.sno, args.ref, in_path=sidecar_in_path(args.sno),
                             x_path=sidecar_xfail_path(args.sno), w_path=sidecar_wantrc_path(args.sno),
                             a_path=sidecar_argv_path(args.sno), modes=modes)
    require_population(paths, len(entries), 1, f"entries read from {args.sno} (a suite pair that names zero entries cannot be graded)")
    shard_tag = ""
    if getattr(args, "shard", ""):
        _m = re.fullmatch(r"(\d+)/(\d+)", args.shard.strip())
        if not _m or int(_m.group(2)) < 1 or not (1 <= int(_m.group(1)) <= int(_m.group(2))):
            refuse(f"--shard wants k/N with 1 <= k <= N, got {args.shard!r}")
        _k, _n = int(_m.group(1)), int(_m.group(2))
        _all = len(entries)
        entries = [e for i, e in enumerate(entries) if i % _n == _k - 1]
        if not entries:
            refuse(f"--shard {args.shard} selects zero of the {_all} entries -- N exceeds the suite; a shard that grades nothing is not a board")
        shard_tag = f"shard={_k}/{_n}"
    # ⛔⭐ HONOUR THE modes COLUMN (row board-icon-master-runs-the-ast-graded-parser-fixtures, ceo mint 2026-09-03).
    # MEASURED cause: 153 of the icon master's 534 entries are parser-ladder fixtures whose .ref is a --dump-ast
    # DUMP, and this runner graded them by RUNNING them. Their reds were inevitable and meant nothing, and the
    # printed 398/534 was not a count of anything -- it mixed two populations graded against two different kinds
    # of expected output. Splitting them is not a presentation choice: a denominator that spans two grading
    # regimes cannot be read at all.
    entry_modes = {}
    if getattr(args, "by_modes_column", False):
        entry_modes, csv_path = modes_declarations(args.sno)
        if not entry_modes:
            refuse(f"--by-modes-column needs the suite's modes declaration; neither {Path(args.sno).with_suffix('.modes').name} "
                   f"nor a sibling ALL.csv with a `modes` column is beside {Path(args.sno).name} -- a column that is not there cannot be honoured")
        _uncovered = [e.name for e in entries if e.name not in entry_modes]
        if _uncovered:
            refuse(f"--by-modes-column: {len(_uncovered)} entr(y/ies) are absent from {csv_path} "
                   f"(first: {_uncovered[0]}) -- grading them by a default while calling it 'the modes column' is the defect this flag exists to remove")
        # ⛔⭐ AND THE RUN POPULATION MUST NOT ITSELF BE `ast`, OR THE SPLIT COLLAPSES SILENTLY INTO ONE
        # BUCKET AND PRINTS A FULL, PLAUSIBLE, ENTIRELY FALSE BOARD.  _modes_for() below reads "ast if the
        # column says ast, else `modes`" -- correct on its face, and a trap when `modes` is ITSELF ["ast"],
        # which is exactly what --lang raku/rebus/prolog/snocone give you (LANG_CONFIGS default modes="ast")
        # when the caller omits --modes.  Every entry then satisfies "== ['ast']", ast_graded reads N/N, and
        # every run-graded entry is diffed against `--dump-ast` output it was never meant to match.
        #
        # MEASURED, NOT REASONED (hq_T 2026-09-03, on the Raku master): `run --lang raku --by-modes-column`
        # reported ast_graded=139/139 and ast_fail=42 -- and the suite declares exactly 42 entries as
        # "m3,m4".  All 42 "failures" were the wrong instrument, not a wrong answer.  Adding --modes m3,m4
        # to the same command on the same tree returned the true board: ast 97 entries 83 pass / 14 xfail /
        # FAIL=0, run 42 entries 41/42 both modes.  Nothing about the false board looked false: it had a
        # denominator, a fail list and a stamp, and it disagreed with SCORE.md's stale cell in the direction
        # a reader would have believed (a suite that had "gotten worse").
        #
        # ⛔ IT REFUSES RATHER THAN PICKING m3,m4 FOR YOU.  Guessing the run modes would make this flag
        # succeed by inventing the one input the caller failed to state -- the same class of defect one
        # comment up.  The two existing callers (board_icon_master.sh, test_gate_icon_board_honours_modes_
        # column.sh) both already pass --modes explicitly and are untouched by this.
        if modes == ["ast"] and not args.modes:
            _declared = sorted({v for v in entry_modes.values() if v and v not in ("ast", "UNKNOWN")})
            if _declared:
                refuse(f"--by-modes-column cannot be honoured: {csv_path} declares {len(_declared)} non-ast "
                       f"modes value(s) ({', '.join(_declared)}) but the run population's own modes resolved to "
                       f"'ast' (from --lang {args.lang}'s default), so BOTH populations would be graded by "
                       f"--dump-ast and every run-graded entry would fail against a ref it was never meant to "
                       f"match. Pass the run modes explicitly, e.g. --modes {_declared[0]}")
    else:
        # ⛔⭐⭐ THE MIRROR TRAP, AND THE ONE WITH THREE INDEPENDENT WITNESSES IN A SINGLE DAY. The block
        # above guards `--by-modes-column` WITHOUT `--modes`. This guards `--modes` WITHOUT
        # `--by-modes-column`: the suite declares `modes=ast` entries whose .ref is a --dump-ast DUMP, the
        # caller asks for m3,m4, and every one of those entries is EXECUTED and diffed against an AST
        # dump it was never meant to match. The reds are inevitable and mean nothing.
        # MEASURED THREE TIMES, 2026-09-03/04, by three seats who did not know of each other:
        #   Pascal   (seat11) -- 5 of the "eleven reds" were parser__* modes=ast entries, force-graded
        #   Snocone  (seat12) -- the 175/273 false board; the real one is 7 of 206
        #   Raku     (hq_T)   -- the 42-ast_fail false board, the mirror direction of the same root
        # ⭐ Three suite-level fixes were proposed for what is ONE harness defect. The tell that it was
        # one defect and not three: the same shape appeared in languages whose only shared component is
        # this file. A defect that reproduces across independent lanes is in the thing they share.
        # ⛔ REFUSES, and does not silently pick --by-modes-column for the caller: honouring a column the
        # caller did not ask to honour would change which oracle grades an entry, which is exactly the
        # decision that must never be made implicitly. Scoped to entries ACTUALLY IN THIS RUN, so a shard
        # or family filter that excludes every ast entry is unaffected and never refuses.
        # ⛔⭐⭐ AND THE EVIDENCE MUST TRAVEL WITH THE SUITE, OR THIS GUARD IS BLIND EXACTLY WHERE IT IS NEEDED.
        # It used to read `Path(args.sno).parent / "ALL.csv"` and nothing else, so its activation depended on
        # WHERE THE CALLER HAPPENED TO PUT THE FILE. Every runner that grades an EXTRACTED family in a
        # tempdir -- the documented bridge, test_snocone_corpus_suite.sh's own shape -- has no ALL.csv beside
        # it, and a guard that cannot see its subject says nothing, which is indistinguishable from a pass.
        # MEASURED BOTH WAYS ON ONE PAIR OF COMMANDS (hq_T 2026-09-05, pascal's 5 modes=ast parser entries):
        # graded IN PLACE, rc=2 REFUSING; the SAME entries extracted to a tempdir and graded the same way,
        # rc=1 with a full plausible board -- total=5 m3_fail=5 m4_fail=5, five manufactured reds.
        # ⭐ The other half of the same defect: --by-modes-column REFUSED on an extracted pair for want of
        # that sibling csv, so on an extraction the CORRECT call was impossible and the incorrect one was
        # silent. extract-family now carries a `.modes` sidecar the way it already carries `.in` and
        # `.xfail`, under the law written in its own docstring: A CHECK THAT DOES NOT CARRY EVERY FIELD THE
        # GRADER READS IS NOT A CHECK (hq_C). `modes` was the one field it did not carry.
        _decl, _csvp = modes_declarations(args.sno)
        if _decl and modes and [m for m in modes if m != "ast"]:
            _forced = [e.name for e in entries if _decl.get(e.name) == "ast"]
            if _forced:
                refuse(f"{_csvp} declares {len(_forced)} entr(y/ies) as modes=ast (first: {_forced[0]}), "
                       f"whose .ref is a --dump-ast dump, but this run was asked for --modes {','.join(modes)} "
                       f"WITHOUT --by-modes-column -- so those entries would be EXECUTED and diffed against an "
                       f"AST dump they were never meant to match, manufacturing reds that mean nothing. "
                       f"Pass --by-modes-column so each entry is graded by the modes it declares.")
    def _modes_for(e):
        # ⭐ UNKNOWN is a DEFAULT, never a declaration, and it is COUNTED separately below so it can never be
        # mistaken for one. The alternative -- refusing on UNKNOWN -- would block the honest board on 17 icon
        # entries whose families simply have no MODES.tsv line yet; the alternative to THAT, silently folding
        # them into the run population with no trace, is how a default becomes an unexamined fact.
        declared = entry_modes.get(e.name, "")
        return ["ast"] if declared == "ast" else modes
    ast_entries = [e for e in entries if _modes_for(e) == ["ast"]] if entry_modes else []
    run_entries = [e for e in entries if _modes_for(e) != ["ast"]] if entry_modes else entries
    unknown_defaulted = sum(1 for e in run_entries if entry_modes.get(e.name, "") == "UNKNOWN") if entry_modes else 0
    # ⛔⭐ HONOUR THE DECLARATION PER ENTRY, WHICH IS WHAT THIS FLAG'S OWN --help PROMISES. Before this, every run
    # entry was graded with the CALLER'S modes and the `modes` column only ever chose ast-vs-run, so a family whose
    # runner grades m3 ONLY (19 of prolog's 28 per-rung runners do) was still EXECUTED in m4 by the master and its
    # m4 verdicts were manufactured -- reds for a mode no runner ever claimed. A declared set is intersected with
    # the caller's --modes (never widened past what the caller asked for); an entry whose whole declaration falls
    # outside the request is NOT graded and is reported by name, because silently grading it in the caller's modes
    # is precisely the substitution this flag exists to stop.
    def _run_modes_for(e):
        d = (entry_modes.get(e.name, "") or "").strip() if entry_modes else ""
        if not d or d in ("UNKNOWN", "ast"):
            return list(modes)
        want = {x.strip() for x in d.split(",") if x.strip()}
        return [m for m in modes if m in want]
    mode_n = {m: 0 for m in modes}
    declared_not_requested = []
    counts = {m: {"PASS": 0, "FAIL": 0, "CRASH": 0, "HANG": 0, "UNPROVEN": 0, "SKIP": 0, "XFAIL": 0, "XPASS": 0} for m in modes}
    ast_counts = {"ast": {"PASS": 0, "FAIL": 0, "CRASH": 0, "HANG": 0, "UNPROVEN": 0, "SKIP": 0, "XFAIL": 0, "XPASS": 0}}
    tmp_root = Path(tempfile.mkdtemp(prefix="csh_run_"))
    fails = []
    _progress_rows = []
    try:
        for e in ast_entries:
            verdicts = run_suite_entry(paths, e, tmp_root, ["ast"], ext=ext, companion_dir=Path(args.sno).parent)
            kind = verdicts["ast"].kind
            _progress_rows.append((e.name, "ast", kind, 0, "xfail" if e.xfail else ""))
            if e.xfail:
                if kind == "PASS":
                    ast_counts["ast"]["XPASS"] += 1; fails.append((e.name, "ast", verdicts["ast"]))
                else:
                    ast_counts["ast"]["XFAIL"] += 1
            else:
                ast_counts["ast"][kind] += 1
                if kind != "PASS":
                    fails.append((e.name, "ast", verdicts["ast"]))
        for e in run_entries:
            _em = _run_modes_for(e)
            if not _em:
                declared_not_requested.append(e.name)
                continue
            verdicts = run_suite_entry(paths, e, tmp_root, _em, ext=ext, companion_dir=Path(args.sno).parent)
            for m in _em:
                mode_n[m] += 1
                kind = verdicts[m].kind
                _progress_rows.append((e.name, m, kind, 0, "xfail" if e.xfail else ""))
                # ⛔ An XFAIL entry (probe/passthru's law-0d witnesses: non-green at conversion time,
                # see convert_one()) is EXPECTED to stay red -- bucketing it as XFAIL/XPASS instead of
                # FAIL/PASS keeps a documented, pre-existing defect from inflating a caller's FAIL count
                # (test_corpus_snobol4.sh's probe/ auto-discovery loop reads m3_fail/m4_fail and would
                # otherwise regress a green gate the moment such a witness converts). An XPASS -- the
                # bug got fixed and nobody updated the marker -- is surfaced just as loudly as a FAIL:
                # it is exactly as actionable, only in the opposite direction.
                if e.xfail:
                    if kind == "PASS":
                        counts[m]["XPASS"] += 1
                        fails.append((e.name, m, verdicts[m]))
                    else:
                        counts[m]["XFAIL"] += 1
                else:
                    counts[m][kind] += 1
                    if kind != "PASS":
                        fails.append((e.name, m, verdicts[m]))
    finally:
        import shutil
        shutil.rmtree(tmp_root, ignore_errors=True)

    family = Path(args.sno).stem
    if entry_modes:
        # ⛔ TWO POPULATIONS, TWO DENOMINATORS, PRINTED SEPARATELY AND NEVER SUMMED. The ast board's total is
        # the ast population, not the suite; likewise the run board. A caller that wants "the suite" adds them
        # deliberately and can see what it is adding.
        a = ast_counts["ast"]
        print(f"SUITE_BOARD_AST family={family} " + (f"{shard_tag} " if shard_tag else "") +
              f"total={len(ast_entries)} ast_pass={a['PASS']} ast_fail={a['FAIL']} ast_crash={a['CRASH']} "
              f"ast_hang={a['HANG']} ast_unproven={a['UNPROVEN']} ast_skip={a['SKIP']} "
              f"ast_xfail={a['XFAIL']} ast_xpass={a['XPASS']}")
        print(f"MODES_COLUMN ast_graded={len(ast_entries)}/{len(entries)} run_graded={len(run_entries)}/{len(entries)} "
              f"unknown_defaulted_to_run={unknown_defaulted}")
    fields = [f"family={family}"] + ([shard_tag] if shard_tag else []) + [f"total={len(run_entries) if entry_modes else len(entries)}"]
    for m in modes:
        c = counts[m]
        # ⛔ `<m>_n` is the DENOMINATOR FOR THAT MODE and it is printed because it is no longer `total`: once each
        # entry is graded in its own declared modes, a mode's verdicts count only the entries that declared it, and
        # a reader dividing by `total` would understate every rate. A board that changed its arithmetic silently is
        # the defect this whole flag exists to prevent.
        fields.append(f"{m}_n={mode_n[m] if entry_modes else len(entries)} "
                       f"{m}_pass={c['PASS']} {m}_fail={c['FAIL']} {m}_crash={c['CRASH']} "
                       f"{m}_hang={c['HANG']} {m}_unproven={c['UNPROVEN']} {m}_skip={c['SKIP']} "
                       f"{m}_xfail={c['XFAIL']} {m}_xpass={c['XPASS']}")
    if entry_modes and declared_not_requested:
        fields.append(f"declared_not_requested={len(declared_not_requested)}")
    print("SUITE_BOARD " + " ".join(fields))
    _progress_record(args.sno, paths, _progress_rows)
    # ⛔ the 40-line sample is a SUMMARY, not a listing: the 5 FAIL / 8 XPASS / 10 HANG entries of a 371-entry
    # board never appeared in it, so nothing could be rowed from names. SUITE_LIST_ALL=1 lists every non-PASS
    # entry (opt-in; the default output is unchanged).
    import os as _os
    for name, m, v in (fails if _os.environ.get('SUITE_LIST_ALL') else fails[:40]):
        tag = "XPASS(marker stale, promote it)" if v.kind == "PASS" else v.kind
        print(f"  {tag} {m} {name}: {v.detail}", file=sys.stderr)
    sys.exit(0 if not fails else 1)


PROGRESS_PACKAGE_KEYS = {"arizona_tests": "arizona", "jcon_tests": "jcon", "ipl": "ipl", "csnobol4_suite": "csnobol4", "gimpel": "gimpel",
                         "snoflake_suite": "snoflake", "aisnobol": "aisnobol", "dotnet": "dotnet", "swi_tests": "swi", "gnu_prolog": "gnu",
                         "inriasuite": "inria", "fpc_tests": "fpc", "pat": "pat", "spitbol_testpgms": "testpgms", "roast": "roast"}


def progress_suite_for(sno_path, paths):
    """(class, suite, lang) when sno_path is a CANONICAL suite -- corpus/tests/<lang>/ALL.<ext> or
    corpus/packages/<lang>/<pkg>/ALL.<ext> under this tree's corpus root (or under any root when S4E_PROGRESS_DB
    redirects the table, which is how the gate proves the arm) -- else None. A scratch copy of a master grades
    exactly as before and records nothing: the live table only ever sees the real suites."""
    p = Path(sno_path).resolve()
    parts = p.parts
    redirected = bool(os.environ.get("S4E_PROGRESS_DB"))
    corpus_root = str(Path(paths["corpus"]).resolve())
    under_corpus = str(p).startswith(corpus_root + os.sep)
    if not (under_corpus or redirected):
        return None
    if p.stem != "ALL":
        return None
    if len(parts) >= 3 and parts[-3] == "tests":
        return ("master", f"{parts[-2]}-master", parts[-2])
    if len(parts) >= 4 and parts[-4] == "packages":
        return ("package", PROGRESS_PACKAGE_KEYS.get(parts[-2], parts[-2]), parts[-3])
    return None


def _progress_pin(paths):
    """Read the tree hashes and the scrip+libscrip_rt.so digest BEFORE the first program is graded, so the rows this
    run appends name the ground it ACTUALLY graded. ⛔ util_progress_append.context() used to run rev-parse at APPEND
    time: seat10's board pulled twice and rebuilt once under a 79-minute run and its 3736 rows were stamped with the
    tree that existed when the run ENDED (CEO-338). The runner's own binary-moved refusal fired at 14:23 -- after the
    harness had already written them."""
    try:
        sys.path.insert(0, str(Path(__file__).resolve().parent))
        import util_progress_append as _pa
        _pa.pin_context(str(paths["scrip_bin"]), str(paths["rt_dir"]))
    except Exception as e:
        print(f"⛔ PROGRESS PIN FAILED ({e.__class__.__name__}: {e}) -- this run's rows cannot be proven to name the "
              f"tree they graded, so the append will not be guarded. Fix the pin; never silence it.", file=sys.stderr)


def _progress_record(sno_path, paths, rows):
    """Append one row per (entry, mode) graded to THE PROGRESS DATABASE through the ONE writer (util_progress_append.py).
    Loud on refusal: a board that could not be recorded is printed AND exits 2 -- progress/README.md, CEO-319."""
    where = progress_suite_for(sno_path, paths)
    if where is None or not rows:
        return
    cls, suite, lang = where
    sys.path.insert(0, str(Path(__file__).resolve().parent))
    import util_progress_append as _pa
    try:
        n = _pa.append_rows([{"class": cls, "suite": suite, "lang": lang, "program": name, "mode": m, "outcome": kind, "secs": secs or 0, "note": note} for name, m, kind, secs, note in rows])
    except _pa.ProgressUnwritable:
        sys.exit(2)
    except _pa.ProgressGroundMoved as e:
        print(str(e), file=sys.stderr)
        sys.exit(2)
    if n:
        print(f"PROGRESS_RECORDED suite={suite} class={cls} rows={n} db={_pa.db_path()}")


def refpins_path(src_path):
    """The pin ledger beside a master pair: ALL.refpins.tsv for ALL.sno."""
    return Path(src_path).parent / (Path(src_path).stem + ".refpins.tsv")


def read_refpins(src_path):
    """{entry: {"sha": ..., "ruling": ..., "measurer": ..., "date": ...}} -- {} when there is no ledger."""
    p = refpins_path(src_path)
    out = {}
    if not p.is_file():
        return out
    for raw in p.read_text().splitlines():
        if not raw.strip() or raw.startswith("#"):
            continue
        f = raw.split("\t")
        if len(f) >= 5:
            out[f[0]] = {"sha": f[1], "measurer": f[2], "date": f[3], "ruling": f[4]}
    return out


def cmd_pin_ref(args):
    """⛔⭐ RE-ANCHOR ONE ENTRY'S REF TO SCRIP'S OWN RULED OUTPUT, WITH PROVENANCE AND A LEDGER.

    THE GAP THIS FILLS (hq_C -> hq_T, 2026-09-04, blocking the SNOBOL4 master control arm): an entry whose
    correct answer is a SCRIP RULING rather than the oracle's answer had NO supported way to be re-anchored.
    The witness is `p = LEN(1) . *(n = n + 1)` (entry user_function_len_defer_branch_6): Lon ruled the
    lambda-deferred-target sugar in, SCRIP now prints `before / after n=1 dummy=[]`, SPITBOL fails the
    construct and always will -- so `capture-oracle-refs` would faithfully re-record the OLD answer, which is
    exactly what seat08 warned must never be run on that row. The master's loose source pair no longer exists
    (one-flat-suite), so there was no upstream to fix either: the only remaining option was hand-editing a
    generated 1753-entry file, which hq_C correctly refused to do.

    ⛔ THE PIN IS NOT THE POINT -- THE LEDGER IS. Any tool can overwrite a ref. What makes this safe is that
    every pin is recorded in ALL.refpins.tsv with the entry, a sha256 of the exact pinned text, who ruled it,
    when, and WHY; and capture-oracle-refs consults that ledger and REFUSES to touch a pinned entry rather
    than silently restoring the oracle's answer. A ref that disagrees with the oracle ON PURPOSE and a ref
    that disagrees BY ACCIDENT look identical in the file -- the ledger is the only thing that tells them
    apart, and without it the next oracle capture quietly undoes a ruling nobody remembers.

    ⛔ --ruling IS MANDATORY AND IS NOT DECORATION. A pin with no stated authority is indistinguishable from
    a ref cut while the compiler was broken, which is the precise failure this project spends its gates on.
    ⛔ THE PINNED TEXT COMES FROM A RUN, NEVER FROM TYPING: --from-scrip runs the entry through the mode the
    caller names and pins what actually came out. Text typed by hand is a claim about behaviour; output from
    a run is evidence of it.
    ⭐ Everything except the one entry's ref block is rewritten BYTE-IDENTICALLY -- the writer is the same
    write_block_suite the builder uses, fed the same entries, so the pin cannot smuggle in a reindex."""
    paths = resolve_paths()
    src_path, ref_path = Path(args.sno), Path(args.ref)
    for label, p_ in (("suite", src_path), ("ref", ref_path)):
        if not p_.is_file():
            refuse(f"{label} file does not exist: {p_}")
    if not args.ruling.strip():
        refuse("--ruling is mandatory: a pinned ref with no stated authority cannot be told from a ref cut "
               "while the compiler was broken")
    # ⛔ THE READER IS CHOSEN THE WAY `run` CHOOSES IT, never assumed. The SNOBOL4 master is MIXED -- one-line
    # entries and banner blocks in one file -- and read_block_suite dies on its very first line. Getting this
    # wrong is not a crash you can ignore: a pin is a rewrite of the whole pair, so a reader that mis-parses
    # would rewrite 1753 entries into whatever it thought it read.
    cfg = LANG_CONFIGS[args.lang] if args.lang else None
    if cfg:
        co, cc = cfg["comment_open"], cfg["comment_close"]
        entries = read_block_suite(src_path, ref_path, banner_re_for(co, cc), in_path=sidecar_in_path(src_path),
                                   x_path=sidecar_xfail_path(src_path), w_path=sidecar_wantrc_path(src_path),
                                   a_path=sidecar_argv_path(src_path))
    else:
        co, cc = "*", ""
        entries = read_suite(src_path, ref_path, in_path=sidecar_in_path(src_path),
                             x_path=sidecar_xfail_path(src_path), w_path=sidecar_wantrc_path(src_path),
                             a_path=sidecar_argv_path(src_path))
    hit = [e for e in entries if e.name == args.entry]
    if not hit:
        refuse(f"no entry named {args.entry!r} in {src_path} ({len(entries)} entries) -- `list` prints them")
    e = hit[0]
    # ⭐ Entry.ref is a STRING for a one-line entry and a LIST for a block -- normalise for the diff, and put
    # the entry's own shape back before writing, or a one-line entry would be rewritten as a list of chars.
    old_ref = e.ref.split("\n") if isinstance(e.ref, str) else list(e.ref)
    check_scrip(paths)
    tmp = Path(tempfile.mkdtemp(prefix="pin_ref."))
    try:
        ext = cfg["ext"] if cfg else ".sno"
        one = tmp / (args.entry + ext)
        one.write_text("\n".join(e.sno_lines) + "\n")
        stdin_text = e.stdin
        if args.mode == "m3":
            v = run_m3(paths, one, "\n".join(old_ref), stdin_text=stdin_text, want_rc=e.want_rc, prog_argv=e.argv)
        else:
            v = run_m4(paths, one, "\n".join(old_ref), tmp, stdin_text=stdin_text, want_rc=e.want_rc, prog_argv=e.argv)
        if v.kind in ("CRASH", "HANG", "UNPROVEN", "SKIP"):
            refuse(f"{args.mode} on {args.entry} came back {v.kind} ({v.detail}) -- a pin records what the "
                   f"compiler DOES, and a run that crashed or could not be made says nothing about that")
        # ⭐ Verdict.text is a METHOD here, not a property -- and the whole file calls it as one. Reading the
        # class as if it were a dataclass is how `v.text.split` became "'function' object has no attribute".
        _txt = v.text().rstrip("\n")
        new_ref = _txt.split("\n") if _txt else [""]
    finally:
        shutil.rmtree(tmp, ignore_errors=True)
    # ⛔⛔⭐ THE ORACLE IS ASKED, ALWAYS, AND ITS ANSWER IS RECORDED -- BECAUSE THE FIRST USE OF THIS VERB
    # BLESSED A REGRESSION (hq_T, 2026-09-04, reverted in corpus 33e747c2c). The pin was applied on a REPORTED
    # premise -- "SPITBOL fails this construct too, so it can never be graded against an oracle-captured ref"
    # -- and the premise was false: `sbl -bf` runs that program cleanly, rc=0, and prints exactly what the ref
    # already held. The ref was oracle-valid, SCRIP diverged from it, and the pin turned a CORRECT red gate
    # green by rewriting the expectation to match the compiler. That is the ICN4 false green in mirror image.
    # ⭐ THE CURE IS NOT "BE MORE CAREFUL", IT IS TO MAKE THE PREMISE A MEASUREMENT. "The oracle cannot grade
    # this" is a claim about a program, and this tool can settle it in one run, so it does -- and it records
    # the oracle's own answer in the ledger, so a later reader can check the ruling instead of trusting it.
    # ⛔ A CLEAN ORACLE ANSWER THAT DISAGREES WITH THE PIN REFUSES. Overriding needs --oracle-disagrees-and-i-
    # mean-it, which exists so the override is a sentence someone had to type, not a flag they might miss.
    ora_line, ora_rc, ora_kind, ora_text = "not consulted", None, "", ""
    if not args.lang:
        # resolve_oracle_bin returns (binary, flags) -- the ONE authority, never re-derived here.
        _ob, _of = resolve_oracle_bin(paths, args.lang)
        if _ob:
            _t2 = Path(tempfile.mkdtemp(prefix="pin_oracle."))
            try:
                one2 = _t2 / (args.entry + ".sno")
                one2.write_text("\n".join(e.sno_lines) + "\n")
                ora_text, ora_rc, ora_kind = run_oracle(_ob, _of, one2, paths["timeout"], stdin_text=e.stdin)
            finally:
                shutil.rmtree(_t2, ignore_errors=True)
            ora_line = f"{ora_kind} rc={ora_rc}"
            print(f"--- the ORACLE's own answer for {args.entry}: {ora_line}")
            for l in (ora_text.split("\n") if ora_text else [])[:20]:
                print("    o " + l)
            if ora_kind == "RAN" and ora_rc == 0:
                _ol = ora_text.split("\n") if ora_text else [""]
                if _ol != new_ref and not args.oracle_disagrees_and_i_mean_it:
                    refuse(f"the oracle RAN {args.entry} cleanly (rc=0) and its answer differs from what you are "
                           f"about to pin -- so this entry CAN be oracle-graded, and pinning SCRIP's output over "
                           f"it would turn a correct red into a green. If the oracle is genuinely wrong here, say "
                           f"so with --oracle-disagrees-and-i-mean-it and the ledger will record BOTH answers.")
    if new_ref == old_ref:
        print(f"⭐ NO CHANGE: {args.entry}'s ref already matches {args.mode} output -- nothing pinned, nothing written.")
        return
    print(f"--- {args.entry}: ref BEFORE ({len(old_ref)} line(s))")
    for l in old_ref[:20]:
        print("    - " + l)
    print(f"--- {args.entry}: ref AFTER, from a live {args.mode} run ({len(new_ref)} line(s))")
    for l in new_ref[:20]:
        print("    + " + l)
    if not args.apply:
        print("\nDRY RUN -- nothing written. Re-run with --apply to pin it and record the ledger line.")
        return
    e.ref = "\n".join(new_ref) if isinstance(e.ref, str) else new_ref
    if cfg:
        write_block_suite(entries, src_path, ref_path, co, cc,
                          out_in=sidecar_in_path(src_path), out_x=sidecar_xfail_path(src_path))
    else:
        write_suite(entries, src_path, ref_path, out_in=sidecar_in_path(src_path))
    led = refpins_path(src_path)
    sha = hashlib.sha256(("\n".join(new_ref) + "\n").encode()).hexdigest()[:16]
    rows = read_refpins(src_path)
    rows[args.entry] = {"sha": sha, "measurer": args.measurer or os.environ.get("S4E_SEAT", "unknown"),
                        "date": time.strftime("%Y-%m-%dT%H:%MZ", time.gmtime()),
                        "ruling": args.ruling.strip() + f"  [ORACLE AT PIN TIME: {ora_line}]"}
    with open(led, "w") as f:
        f.write("# SCRIP-RULED REF PINS for this master. ⛔ An entry listed here has a ref that DISAGREES WITH THE\n"
                "# ORACLE ON PURPOSE, on a stated ruling -- capture-oracle-refs REFUSES to touch these, because\n"
                "# restoring the oracle's answer would silently undo a decision nobody remembers making.\n"
                "# ⛔ A pin is evidence only while its sha matches: if the ref has been edited since, the pin no\n"
                "# longer describes the file and the entry must be re-ruled, not re-blessed.\n"
                "# entry\tsha256-16\tmeasurer\tdate\truling\n")
        for k in sorted(rows):
            r = rows[k]
            f.write(f"{k}\t{r['sha']}\t{r['measurer']}\t{r['date']}\t{r['ruling']}\n")
    print(f"\n✅ PINNED {args.entry} from a live {args.mode} run · ledger {led.name} now carries {len(rows)} pin(s)")
    print(f"   sha256/16 {sha} · ruling: {args.ruling.strip()}")
    print("   ⛔ Commit the master pair AND the ledger together -- a pin whose ledger line is unpushed is a ref "
          "that disagrees with the oracle for no recorded reason.")


def cmd_extract(args):
    """Materialize ONE suite entry back into a standalone .sno (+ optional .ref) file. For consumers that
    need per-witness standalone access a shared suite file cannot give them directly -- e.g. a gate script
    compiling one named witness under several env-var arms with custom stdout/stderr handling. Reuses
    read_suite() (ONE AUTHORITY for the suite grammar) rather than re-parsing the format a second time."""
    # ⛔⭐ TWO READERS EXIST AND THIS ONE PICKED THE WRONG ONE FOR BLOCK-ONLY FAMILIES (hq_B 2026-08-29).
    # read_suite() handles the one-line dialect and one-line/block INTERLEAVING; read_block_suite() handles a
    # format-(B)-ONLY family, where every entry is a banner-delimited block. cmd_extract called read_suite()
    # unconditionally, so on a block-only family it compared sno LINES to ref LINES 1:1 -- a correspondence
    # multi-line procedure bodies legitimately break -- and died with
    #     ValueError: family.ref is shorter than family.sno at seq N
    # MEASURED: rung10_augop (5 entries) and rung36_all (36 entries) both FAIL under read_suite and both read
    # CLEANLY under read_block_suite. rung10_augop's ref is 11 lines for 5 entries = 5 banners + 6 output
    # lines, which is exactly right; its .icn is 37 because the bodies are procedures.
    # ⛔⭐ THE ERROR TEXT NAMES ONLY ONE OF ITS TWO CAUSES, which is why this cost a wrong diagnosis before it
    # cost a fix: "family.ref is shorter than family.sno" is emitted BOTH when a ref is genuinely truncated AND
    # when the one-line reader is pointed at a block family. The first reading sends you to regenerate a
    # correct .ref against the oracle -- destructive work on a good file. Prefer the block reader and keep the
    # one-line reader as the fallback, and if BOTH fail report BOTH errors rather than the last one, so the
    # next reader is never handed a single cause for a two-cause signal.
    if getattr(args, "origin", ""):
        _csv = Path(args.sno).with_name("ALL.csv")
        if not _csv.is_file():
            refuse(f"--origin needs the master index beside the suite ({_csv}) and it is not there -- extract by name, or point at the master")
        import csv as _csvmod
        _hits = [r["entry"] for r in _csvmod.DictReader(open(_csv, encoding="utf-8")) if r.get("origin") == args.origin]
        if not _hits:
            _fam = args.origin.split("__", 1)[0]
            _near = sorted({r["origin"] for r in _csvmod.DictReader(open(_csv, encoding="utf-8")) if r.get("origin", "").startswith(_fam + "__")})[:8]
            refuse(f"no entry with origin {args.origin!r} in {_csv}" + (f" (same family has: {', '.join(_near)}{'...' if len(_near) == 8 else ''})" if _near else " (no origin of that family at all)"))
        if args.name and args.name != _hits[0]:
            refuse(f"--origin {args.origin} resolves to entry {_hits[0]!r} but the positional name says {args.name!r}; give one or the other")
        args.name = _hits[0]
    if not args.name:
        refuse("extract needs the entry NAME (positional) or --origin <origin>")
    _ext = Path(args.sno).suffix
    _copen, _cclose = "*", ""
    for _lc in LANG_CONFIGS.values():
        if _lc.get("ext") == _ext:
            _copen, _cclose = _lc.get("comment_open", "*"), _lc.get("comment_close", "")
            break
    # ⛔⭐⭐ A ROUND TRIP THAT DOES NOT CARRY EVERY FIELD THE GRADER READS IS NOT A ROUND TRIP (row
    # suite-harness-xfail-extract-round-trip, hq_P finding 2026-08-30, seat15 landing). Neither reader was
    # ever given in_path/x_path here, so e.stdin and e.xfail_reason came back None for EVERY extracted
    # entry regardless of what the suite actually carries -- exactly the same hole 4cc1ccbb closed in
    # util_build_master_suite.py's deletion verifier, one consumer over. MEASURED live and dangerous:
    # snobol4's ALL.in has 9 stdin-bearing entries; a stdin-starved program routinely exits rc=0 with
    # EMPTY output (identical exit code to a fed run), so nothing about the run distinguishes the two --
    # only carrying the field does. 21 scripts route through lib_master_extract.sh -> this function.
    _in_path = sidecar_in_path(args.sno)
    _x_path = sidecar_xfail_path(args.sno)
    try:
        entries = read_block_suite(args.sno, args.ref, banner_re_for(_copen, _cclose), in_path=_in_path, x_path=_x_path)
    except Exception as _block_err:
        try:
            entries = read_suite(args.sno, args.ref, in_path=_in_path, x_path=_x_path)
        except Exception as _line_err:
            print(f"⛔ REFUSED: {args.sno} reads as neither dialect.\n"
                  f"   as a BLOCK-only family : {type(_block_err).__name__}: {_block_err}\n"
                  f"   as a ONE-LINE family   : {type(_line_err).__name__}: {_line_err}\n"
                  f"   Both causes are shown deliberately: 'ref is shorter than sno' is reachable BOTH from a\n"
                  f"   genuinely truncated .ref AND from reading a block family with the one-line reader.",
                  file=sys.stderr)
            raise SystemExit(2)
    for e in entries:
        if e.name != args.name:
            continue
        text = e.sno_lines[0] if e.kind == "line" else "\n".join(e.sno_lines)
        Path(args.out_sno).write_text(text + "\n")
        if args.out_ref:
            ref_text = e.ref if e.kind == "line" else "\n".join(e.ref)
            Path(args.out_ref).write_text(ref_text + "\n")
        # ⛔ REFUSE, never silently drop: an entry that needs stdin and is materialized without it is not
        # the witness the suite graded -- it is a DIFFERENT program that happens to share source text. A
        # stdin-starved SNOBOL4 program typically still exits rc=0 (INPUT read failure is not fatal by
        # default), so the caller gets a clean-looking run of the wrong thing, not a loud error.
        if e.stdin is not None:
            if not args.out_in:
                refuse(f"{args.name!r} carries stdin ({len(e.stdin)} byte(s)) but --out-in was not given -- "
                       f"materializing it without stdin would silently grade a DIFFERENT witness than the "
                       f"one the suite actually graded (a stdin-starved run commonly still exits rc=0). "
                       f"Pass --out-in <path> and feed it to whatever runs {args.out_sno!r}.")
            Path(args.out_in).write_text(e.stdin)
        if args.out_xfail and e.xfail_reason:
            Path(args.out_xfail).write_text(e.xfail_reason + "\n")
        return
    refuse(f"no entry named {args.name!r} in {args.sno} (have: {', '.join(sorted(e.name for e in entries))})")


def modes_sidecar_path(sno_path):
    """The WRITE target for an extracted suite's modes declaration: <stem>.modes beside the pair, named the
    same way .in and .xfail are. Kept separate from the discovery form below for the reason the extractor
    already documents about its other sidecars -- a discovery function returns None until the file exists,
    which is right for reading and useless for naming one to create."""
    return str(Path(sno_path).with_suffix(".modes"))


def modes_declarations(sno_path):
    """{entry: declared modes} for the suite at `sno_path`, plus the name of the evidence used.

    ⛔⭐ TWO SOURCES, IN THIS ORDER, AND THE ORDER IS THE POINT. A `<stem>.modes` sidecar TRAVELS with an
    extracted family; a sibling `ALL.csv` only exists for a suite still sitting in the corpus. Reading the
    csv alone made every guard below depend on where the caller put the file rather than on what the suite
    declares -- see the measured pascal witness at the mirror-trap guard. Returns ({}, None) when neither is
    present, which is an honest "no declaration was reachable" and never a guess.
    ⛔ THE EMPTY STRING IS NOT A DECLARATION EITHER, and it is the third spelling of "nobody said": UNKNOWN
    (loud, counted separately on every board), `` (silent -- what every corpus/packages/*/ALL.csv carries
    today), and an absent row (which --by-modes-column refuses on). Only one of the three is loud. Kept as
    data rather than normalised away, so a reader can see which spelling a suite used."""
    _p = Path(modes_sidecar_path(sno_path))
    if _p.is_file():
        out = {}
        for line in _p.read_text(encoding="utf-8").splitlines():
            if not line.strip() or line.lstrip().startswith("#"):
                continue
            name, _, m = line.partition("\t")
            out[name.strip()] = m.strip()
        if out:
            return out, _p.name
    _csv_path = Path(sno_path).parent / "ALL.csv"
    if _csv_path.is_file():
        import csv as _csvm
        out = {}
        try:
            with open(_csv_path, newline="") as _f:
                rdr = _csvm.DictReader(_f)
                if rdr.fieldnames and "modes" not in rdr.fieldnames:
                    refuse(f"{_csv_path} has no `modes` column -- nothing to honour")
                for _row in rdr:
                    out[_row.get("entry")] = (_row.get("modes") or "").strip()
        except OSError:
            return {}, None
        if out:
            return out, _csv_path.name
    return {}, None


def cmd_extract_family(args):
    """Materialize every entry of ONE family back out as a standalone SUITE PAIR (still banner-block or
    one-line, matching the master's own format) rather than loose individual files -- the bridge for a
    gate that grades a whole suite via `run` the way an old per-family suite file did, before the
    one-flat-suite cutover retired per-family files (corpus-suites-consolidation.task.md, THE ONE-FLAT-
    SUITE RULING). Family membership comes from the master's OWN CSV (the `family` column), never
    re-derived from a name convention -- entries carry no origin/family field once round-tripped through
    plain suite text, and the family-prefix-in-the-name shape is a naming CONVENTION this row's own
    output uses, not a guarantee every consumer may assume (a descriptive rename could break it silently
    -- see util_build_master_suite.py's own descriptive_name(), which never consults the CSV either).
    ⛔⭐ CARRIES STDIN + XFAIL, BOTH WAYS (seat07 2026-08-30, on hq_C's law: "a check that does not carry
    every field the grader reads is not a check"). The first version of this command read and wrote body
    text only, silently dropping any entry's stdin sidecar -- the identical hole hq_C found and fixed in
    util_build_master_suite.py's deletion verifier the same night (SCRIP 4cc1ccbb), independently hit here
    because extraction is its own round trip with its own read and its own write."""
    import csv as _csv
    with open(args.csv, newline="") as f:
        wanted = {row["entry"] for row in _csv.DictReader(f) if row["family"] == args.family}
    if not wanted:
        refuse(f"no rows with family=={args.family!r} in {args.csv}")
    _ext = Path(args.sno).suffix
    _copen, _cclose = "*", ""
    for _lc in LANG_CONFIGS.values():
        if _lc.get("ext") == _ext:
            _copen, _cclose = _lc.get("comment_open", "*"), _lc.get("comment_close", "")
            break
    # ⛔ sidecar_in_path()/sidecar_xfail_path() are DISCOVERY functions -- they return None unless the
    # candidate file ALREADY EXISTS, which is right for locating an existing sidecar to READ and wrong
    # for naming one to WRITE (at this point args.out_sno has not been created yet, so the candidate
    # can never exist and the discovery form would always hand back None). Compute the WRITE targets
    # the same way those functions do internally (<stem>.in / <stem>.xfail beside the output), not via
    # the discovery wrapper.
    _in, _x = sidecar_in_path(args.sno), sidecar_xfail_path(args.sno)
    try:
        entries = read_block_suite(args.sno, args.ref, banner_re_for(_copen, _cclose), in_path=_in, x_path=_x)
        is_block = True
    except Exception:
        entries = read_suite(args.sno, args.ref, in_path=_in, x_path=_x)
        is_block = False
    sel = [e for e in entries if e.name in wanted]
    if len(sel) != len(wanted):
        got = {e.name for e in sel}
        refuse(f"family {args.family!r}: CSV names {len(wanted)} entries, matched {len(sel)} in {args.sno} "
               f"(missing: {sorted(wanted - got)[:5]})")
    out_in, out_x = str(Path(args.out_sno).with_suffix(".in")), str(Path(args.out_sno).with_suffix(".xfail"))
    if is_block:
        write_block_suite(sel, args.out_sno, args.out_ref, _copen, _cclose)
    else:
        write_suite(sel, args.out_sno, args.out_ref)
    # ⭐ SAME PATTERN AS util_build_master_suite.py's main() write path, deliberately not
    # write_block_suite's own out_in=/out_x= parameters: mirroring the one place this sidecar-write-
    # plus-cleanup dance is already proven correct, rather than trusting write_block_suite's internal
    # handling (which writes when told to but does not clean up a stray existing file if nothing new
    # needs writing) to behave identically.
    if not write_stdin_sidecar(sel, out_in, _copen, _cclose) and os.path.exists(out_in):
        os.remove(out_in)
    if not write_xfail_sidecar(sel, out_x, _copen, _cclose) and os.path.exists(out_x):
        os.remove(out_x)
    # ⛔⭐ AND THE `modes` DECLARATION TRAVELS TOO -- the field this extractor did not carry, under the law
    # its own docstring already states: a check that does not carry every field the grader reads is not a
    # check. Without it, `run --modes m3,m4` over an extracted family could not be guarded (the mirror-trap
    # guard had no evidence to read) and `--by-modes-column` could not be honoured (it refused for want of a
    # sibling ALL.csv), so the correct call was impossible and the incorrect one was silent. Written for the
    # SELECTED entries only, because that is exactly the population this pair will be graded over.
    with open(modes_sidecar_path(args.out_sno), "w", encoding="utf-8") as _mf:
        _mf.write("# modes declaration carried out of %s by extract-family (family=%s). entry<TAB>modes.\n"
                  % (Path(args.csv).name, args.family))
        with open(args.csv, newline="") as _cf:
            _decl_all = {r["entry"]: (r.get("modes") or "").strip() for r in _csv.DictReader(_cf)}
        for _e in sel:
            _mf.write("%s\t%s\n" % (_e.name, _decl_all.get(_e.name, "")))


def cmd_list(args):
    """Print every entry name in a suite, one per line, in file order. For consumers that need to
    enumerate a suite's members -- a board denominator, a tool that materializes every entry into a
    scratch directory -- without re-deriving the suite grammar a second time (same rationale as
    cmd_extract, ONE AUTHORITY: read_suite()).

    ⛔⭐ --lang ADDED 2026-09-02 (hq_P, on hq_B's report from the rung-6 witness landing). THIS SUBCOMMAND
    COULD NOT READ ANY NON-SNOBOL4 MASTER AT ALL. read_suite() hardcodes SNOBOL4's `*` banner marker, so a
    Prolog master (`%` comments) parsed as thousands of one-line entries and died on the UNMODIFIED committed
    ALL.pl/ALL.ref with `family.ref is shorter than family.sno at seq NNNN` -- cmd_run has taken --lang and
    dispatched to read_block_suite() since 2026-08-29, and cmd_list was simply never given the same dispatch.
    ⛔⭐ THE ERROR MESSAGE WAS THE EXPENSIVE HALF: it names the REF FILE as short, so it reads as a corpus
    defect. hq_B hit it while promoting the rung-6 witnesses and had to go and re-check the committed baseline
    to establish that their own edit had not broken it. A WRONG-READER FAULT THAT ACCUSES THE DATA COSTS THE
    NEXT READER A BISECTION THEY DID NOT NEED. Hence the explicit suffix refusal below: it names the reader,
    the suffix and the exact flag to pass, and it fires BEFORE the grammar can produce a misleading message.
    ⭐ AND IT MADE A DOCUMENTED PROCEDURE UNFOLLOWABLE: lib_master_extract.sh's INTERIM PROMOTION PROTOCOL
    tells a promoter to prove the promotion with `list`, which for .pl could not be done at all."""
    if args.lang:
        cfg = LANG_CONFIGS[args.lang]
        entries = read_block_suite(args.sno, args.ref, banner_re_for(cfg["comment_open"], cfg["comment_close"]))
    else:
        suffix = Path(args.sno).suffix
        if suffix and suffix != ".sno":
            owner = [k for k, c in LANG_CONFIGS.items() if c["ext"] == suffix]
            hint = f" -- pass --lang {owner[0]}" if owner else " -- no LANG_CONFIGS dialect claims that suffix"
            refuse(f"list: {args.sno} has suffix {suffix!r}, but with no --lang this reads the SNOBOL4 suite grammar "
                   f"(`*` banners){hint}. Refusing rather than parsing every line as a one-line entry and then blaming "
                   f"the .ref file for being short -- the fault would be the READER, not the data.")
        entries = read_suite(args.sno, args.ref)
    for e in entries:
        print(e.name)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)

    o = sub.add_parser("capture-oracle-refs", help="bootstrap missing .ref files for an oracle-graded, no-.ref family from a live SPITBOL (or --lang dialect) run (m3+m4 must both agree, rc and text)")
    o.add_argument("family_dir")
    o.add_argument("--modes", default="m3,m4")
    o.add_argument("--force", action="store_true", help="re-capture even stems that already have a .ref (default: leave them alone)")
    o.add_argument("--lang", default="", choices=LANG_CHOICES, help="capture against a LANG_CONFIGS dialect's own oracle instead of SPITBOL (only 'prolog' has an oracle wired as of 2026-08-29; default '' means snobol4, unchanged behavior)")
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
    b.add_argument("--xfail", default="", help="comma-separated stems whose original is KNOWN-RED: convert them as XFAIL witnesses instead of refusing. Byte-equal-or-no-delete is UNCHANGED -- the candidate must still reproduce the ORIGINAL's exact verdict. REFUSES if a named stem is actually green (that is an XPASS, as actionable as a failure).")
    b.add_argument("--skip-reason", default="", help="mandatory-in-spirit reason printed for every --skip name")
    b.set_defaults(func=cmd_convert_blocks)

    r = sub.add_parser("run", help="run a suite .sno/.ref pair (or --lang dialect pair) and print PASS/FAIL/CRASH/HANG/UNPROVEN/SKIP counts")
    r.add_argument("sno")
    r.add_argument("ref")
    r.add_argument("--modes", default="", help="default: m3,m4 (or LANG_CONFIGS[lang]['modes'] if --lang given)")
    r.add_argument("--lang", default="", choices=LANG_CHOICES, help="read/grade as a LANG_CONFIGS dialect instead of the default SNOBOL4 suite format")
    r.add_argument("--by-modes-column", action="store_true",
                   help="grade each entry by the `modes` column of the suite's sibling ALL.csv instead of grading every entry the same way: "
                        "modes=ast entries are graded by `scrip --dump-ast` diffed as text, everything else by --modes (default m3,m4). "
                        "Prints the two populations as SEPARATE boards with their OWN denominators. REFUSES rc=2 if the CSV is missing or "
                        "does not cover every entry -- a column that cannot be read is not a column that can be honoured.")
    r.add_argument("--shard", default="", help="k/N: grade only every N-th entry starting at the k-th (1-based, interleaved), so the N shards partition the suite exactly once and their boards SUM to the monolithic board; the SUITE_BOARD line carries shard=k/N and total=<this shard's entries> (row corpus-runner-master-suite-exceeds-single-call-cap, hq_B 2026-09-02)")
    r.set_defaults(func=cmd_run)

    e = sub.add_parser("extract", help="materialize ONE suite entry back into a standalone .sno (+ optional .ref/.in/.xfail) file")
    e.add_argument("sno")
    e.add_argument("ref")
    e.add_argument("name", nargs="?", default="", help="the entry's CURRENT name (the builder renames entries: directive_82); or give --origin instead")
    e.add_argument("out_sno")
    e.add_argument("--origin", default="", help="resolve the entry by its ORIGIN (the durable provenance key, e.g. ladder__rung00_hello) through the ALL.csv beside the master; REFUSES if the CSV or the origin is absent (hq_C ask 2026-09-02, rung 0)")
    e.add_argument("--out-ref", default="", dest="out_ref")
    e.add_argument("--out-in", default="", dest="out_in", help="required if the entry carries stdin -- REFUSES rather than silently materializing a stdin-bearing entry without it")
    e.add_argument("--out-xfail", default="", dest="out_xfail", help="optional: write the entry's xfail reason here if it has one (documentation only, never affects grading)")
    e.set_defaults(func=cmd_extract)

    ef = sub.add_parser("extract-family", help="materialize one family's entries back into a standalone suite pair (banner-block or one-line, matching the source's own format) -- family membership comes from the source's own CSV, never re-derived from names")
    ef.add_argument("sno")
    ef.add_argument("ref")
    ef.add_argument("csv")
    ef.add_argument("family")
    ef.add_argument("out_sno")
    ef.add_argument("out_ref")
    ef.set_defaults(func=cmd_extract_family)

    pr = sub.add_parser("pin-ref", help="re-anchor ONE entry's ref to SCRIP's own ruled output, with a mandatory ruling and a provenance ledger that capture-oracle-refs then refuses to overwrite")
    pr.add_argument("sno"); pr.add_argument("ref"); pr.add_argument("entry")
    pr.add_argument("--ruling", required=True, help="WHY this entry's answer is SCRIP's to decide and not the oracle's (mandatory: an unexplained pin cannot be told from a ref cut while the compiler was broken)")
    pr.add_argument("--mode", default="m3", choices=["m3", "m4"], help="which mode's live output to pin (default m3)")
    pr.add_argument("--measurer", default="", help="who ruled it (defaults to $S4E_SEAT)")
    pr.add_argument("--lang", default="", choices=LANG_CHOICES)
    pr.add_argument("--oracle-disagrees-and-i-mean-it", action="store_true", help="pin even though the oracle RAN this entry cleanly and gave a different answer -- the ledger records both. Spelled out in full because the first use of this verb blessed a regression on an unmeasured claim that the oracle could not grade the entry")
    pr.add_argument("--apply", action="store_true", help="write it (without this the diff is printed and nothing changes)")
    pr.set_defaults(func=cmd_pin_ref)

    l = sub.add_parser("list", help="print every entry name in a suite, one per line, in file order")
    l.add_argument("sno")
    l.add_argument("ref")
    l.add_argument("--lang", default="", choices=LANG_CHOICES, help="read as a LANG_CONFIGS dialect instead of the default SNOBOL4 suite format (a non-.sno suffix without this REFUSES rather than misreading)")
    l.set_defaults(func=cmd_list)

    args = ap.parse_args()
    if getattr(args, "lang", None) == "snobol4":
        args.lang = ""      # the accepted synonym for the default -- see LANG_CHOICES
    args.func(args)


if __name__ == "__main__":
    main()
