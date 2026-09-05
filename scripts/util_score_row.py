#!/usr/bin/env python3
# util_score_row.py -- THE ONE HELPER EVERY RUNNER CALLS to rewrite its suite's row in the ONE
# LEADERBOARD, .github/SCORE.md.  Row `score-md-rows-are-rewritten-by-the-runner-that-measured-them`
# (rank 0, hq_T; rowed to hq_B at CEO-167 and reassigned when /home/claude_T opened -- GOAL-HQ-TEST
# mandate 2).
#
# THE LAW IT IMPLEMENTS, verbatim from RULES.md FACT RULES (Lon 2026-09-03 ~16:05 CDT to ceo): "Make a
# FACT RULE that any run of a test suite by any session will update the ONE LEADERBOARD. Make it
# official so the file DOES stay up-to-date always." -- and ten minutes earlier, the WHY, which is the
# design spec and not just motivation: "go to a LEADER BOARD that is kept up-to-date. So whenever we
# want to know the state it is there not an hour away of running tests."
#
# ⛔⭐ WHY THIS IS NOT util_build_score_md.py, AND WHY IT MUST NOT BECOME IT. That generator answers a
# different question and answers it well: it REGENERATES THE WHOLE GRID by invoking all seven
# languages' floor gates and all seven master boards. That is ~30-40 minutes on a loaded box -- which
# is the exact cost Lon was ruling against in the sentence that created this file. A helper that
# re-runs anything to record a measurement someone ALREADY MADE has reintroduced the hour-away wait it
# was written to remove. So the hard rule for this file: IT RUNS NO SUITE, NO GATE AND NO COMPILER. It
# reads git for provenance, the Makefile for RT_OPT, and the numbers the caller hands it. If you ever
# find yourself adding a subprocess call to a test script here, you are writing the other tool.
#
# ⛔ AND IT REFUSES RATHER THAN GUESSES (RULES.md: a test that cannot measure REFUSES with rc=2).
# Unknown language, unknown column, a board line carrying no digit, a pipe character that would
# silently corrupt the table, a missing measurer, a grid whose shape does not match -- every one of
# these exits 2 with a named reason.  There is no path here that writes a plausible row it did not get
# told.  A leaderboard that invents a cell is worse than one that is stale: stale says so.
#
# ⛔⭐ THE PROVENANCE COLUMN IS A LIST, NOT A LINE, AND THAT IS DELIBERATE. One row's cells are
# routinely measured by different sessions on different trees -- Icon's STRICT rung suite, its master
# board and its Arizona/JCON vendor numbers were three trees and three measurers on the day this was
# written.  A single "Tree - clock - by" string for the whole row would therefore be false for at
# least two of its cells, and falsely PRECISE, which is worse than vague.  So the last cell holds one
# `<column>: <stamp>` clause per measured column, and rewriting a column rewrites ONLY its own clause.
import argparse
import datetime
import difflib
import importlib.util
import os, re, subprocess, sys, time

S4E = os.environ.get("S4E_HOME") or os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
SCORE_MD = os.path.join(S4E, ".github", "SCORE.md")
REPOS = ["SCRIP", "corpus"]

# The grid's columns, keyed by the short name a runner passes to --column.  Index is the cell position
# in the markdown row.  These are READ BACK from the live header line and cross-checked (see
# find_table) -- the table below is the naming authority, never the shape authority, so a column
# renamed upstream refuses loudly here instead of writing into the wrong cell.
COLUMNS = {
    "entries": (1, "Master suite"),
    "floor":   (2, "Floor / smoke gate"),
    "board":   (3, "Master board"),
    "vendor":  (4, "Vendor / package suites"),
}
PROV_COL = 5
STALE_WARN_COMMITS = 25

# ⭐ THE FALLBACK cmd_write USES WHEN A PLAIN OVERWRITE WOULD LOSE PROSE (see cell_prose_loss below), row
# score-writer-models-the-snobol4-board-cell-so-the-runner-can-write-it. hq_T RULED REFUSE on 2026-09-04
# (task score-md-runners-rewrite-a-cell-in-place-and-silently-discard-the-prose-they-never-modelled) for a
# NAMED reason: silently carrying an old caveat forward attaches it to a new number as though it still
# describes the CURRENT measurement, when its truth is unknown by construction (the Raku
# unknown_defaulted_to_run=10 incident -- a stale claim that would have read as current and correct).
# ⛔ THIS IS NOT THAT. A silent merge and a labelled supersession are different claims. What follows never
# asserts `before` is still true -- it demotes it, explicitly, to "the reading THIS one supersedes", which
# is exactly the rhetorical move every hand-edit of the snobol4 board/entries cells already makes (`⛔
# SUPERSEDED READING BELOW`, `⚠ SUPERSEDES the reading below ... kept verbatim`) -- this only mechanizes it.
# Carrying `before` forward BYTE-FOR-BYTE also makes loss structurally impossible: cell_prose_loss(before,
# result) is empty by construction, because `before` is a literal substring of `result` (see the assert in
# cmd_write). A cell whose entire un-modelled history is already labelled "superseded" by an earlier write
# is not the case hq_T's ruling was written against; a cell carrying a LIVE, un-superseded caveat about the
# measurement being replaced still is, and this fallback does not (cannot) launder that case quiet -- it
# just refuses to leave the runner permanently unable to write a cell that has grown enough history, which
# was becoming the status quo hand-editing was creating anyway. Surfaced back to hq_T as a QA note on this
# row rather than decided silently: this is a seat's reading of an HQ ruling, not a re-ruling of it.
SUPERSEDE_MARKER = "⛔ SUPERSEDES the reading below (util_score_row.py folded it forward verbatim as provenance, not hand-edited, not asserted still true):"


def die(msg, rc=2):
    sys.stderr.write("REFUSED(%d) util_score_row: %s\n" % (rc, msg))
    raise SystemExit(rc)


def git(repo, *args):
    p = os.path.join(S4E, repo)
    if not os.path.exists(os.path.join(p, ".git")):
        return None
    try:
        out = subprocess.run(["git", "-C", p] + list(args), capture_output=True, text=True, timeout=30)
    except Exception:
        return None
    return out.stdout.strip() if out.returncode == 0 else None


def tree_stamp():
    # The SCORE.md-column rendering of lib_gate.sh's gate_stamp tree line -- same three facts (short
    # hash, -DIRTY when the worktree has uncommitted changes, one entry per repo the measurement
    # reads), rendered as markdown instead of as a gate's stdout.  `-e` not `-d` on .git, because in a
    # git WORKTREE .git is a FILE and `-d` silently skips every repo (hq_B measured that one live on
    # 2026-09-02; it printed a stamp with nothing after it).
    parts = []
    for r in REPOS:
        h = git(r, "rev-parse", "--short", "HEAD")
        if h is None:
            parts.append("%s=unknown" % r)
            continue
        dirty = "-DIRTY" if git(r, "status", "--porcelain") else ""
        parts.append("%s `%s%s`" % (r, h, dirty))
    return " · ".join(parts)


def rt_opt():
    # ⛔ READ IT, NEVER TYPE IT.  RT_OPT is -O0 by Lon's s262 FACT RULE and the Makefile carries that at
    # exactly one line -- but this file quoting "-O0" from memory is precisely the transcription defect
    # RULES.md names (a second stale RT_OPT was deleted from the Makefile so a grep could not land on
    # the wrong one; a hardcoded copy here would put it back, one directory over).
    mk = os.path.join(S4E, "SCRIP", "Makefile")
    try:
        for line in open(mk, encoding="utf-8"):
            m = re.match(r"\s*RT_OPT\s*\??=\s*(\S+)", line)
            if m:
                return m.group(1)
    except OSError:
        pass
    return "RT_OPT=unreadable"


def table_shape_error(cells):
    # Return why this header is NOT the standardized display, or None if it IS.  Split out of
    # find_table so the same proof can be applied to EVERY candidate header rather than only the first.
    if len(cells) != PROV_COL + 1:
        return "has %d columns, this helper knows %d (%s)" % (len(cells), PROV_COL + 1, ", ".join(COLUMNS))
    for key, (idx, expect) in COLUMNS.items():
        if not cells[idx].startswith(expect):
            return "column %d is %r, expected it to start with %r (--column %s)" % (idx, cells[idx], expect, key)
    return None


def tree_is_dirty():
    # ⛔⭐ A DIRTY-TREE NUMBER DESCRIBES NO CHECKABLE TREE, SO IT IS NOT A LEADERBOARD ROW (ceo CEO-174,
    # policy for every suite that ever sits inside `make test`). It is a SCOUTING DATUM: real, useful to
    # the person who ran it, and unverifiable by anyone else -- the row would name a hash whose working
    # tree nobody else can reconstruct, which is the unfalsifiable-row defect SCORE.md's own text spends
    # three paragraphs on, merely wearing a real hash as a disguise.
    # ⭐ THE CONDITION IS COMPUTED FROM THE TREE, NEVER CHOSEN. There is deliberately no --force and no env
    # override: an opt-out would make the highest-traffic board the one exception to the FACT RULE, and the
    # exception would be taken by whoever was in the biggest hurry. The clean run IS the landing run, and
    # its SCORE.md change rides the landing's own .github-last push -- so no other seat ever meets an
    # unexplained dirty .github mid-work, which was the whole objection this answers.
    return [r for r in REPOS if git(r, "status", "--porcelain")]


def find_table(lines):
    # Locate the standardized-display grid by its header, and PROVE its shape rather than assuming it.
    # ⛔ EVERY '| Language |' header is a CANDIDATE, not just the first.  SCORE.md carries more than one
    # such grid (ceo CEO-174 added the September-10 M/L/V grid ABOVE this one on 2026-09-03), and binding
    # to the first match made this helper refuse rc=2 for EVERY language and EVERY column -- i.e. the one
    # mechanism the ONE LEADERBOARD fact rule tells every runner to call was dead for the whole fleet,
    # while the grid it was meant to protect sat directly below it.  Shape is the identity here, never
    # position: we take the first header that PROVES it is the standardized display, and if none does we
    # report what each candidate was instead, so the refusal names the real problem.
    candidates = [i for i, l in enumerate(lines) if l.startswith("| Language |")]
    if not candidates:
        die("no '| Language |' grid header in %s -- the standardized display is gone or renamed" % SCORE_MD)
    why = []
    hdr = None
    for i in candidates:
        cells = [c.strip() for c in lines[i].strip().strip("|").split("|")]
        err = table_shape_error(cells)
        if err is None:
            hdr = i
            break
        why.append("line %d %s" % (i + 1, err))
    if hdr is None:
        die("no '| Language |' grid in %s is the standardized display -- refusing to write into the wrong cell. Candidates: %s"
            % (SCORE_MD, "; ".join(why)))
    cells = [c.strip() for c in lines[hdr].strip().strip("|").split("|")]
    rows = {}
    skipped = {}
    for i in range(hdr + 2, len(lines)):
        if not lines[i].startswith("|"):
            break
        c = [x.strip() for x in lines[i].strip().strip("|").split("|")]
        if len(c) == PROV_COL + 1:
            rows[c[0]] = (i, c)
        elif c and c[0]:
            # ⛔⭐⭐ A ROW OF THE WRONG WIDTH IS *SKIPPED*, NEVER *ABSENT*, AND COLLAPSING THAT DISTINCTION IS
            # THE WHOLE DEFECT. This loop used to have no `else` at all: a mis-shaped row was dropped on the
            # floor in silence, and every reader below then described a table it could not see all of. The
            # cost, measured live (seat07 → hq_P, 2026-09-04): a hand edit appended a cross-confirmation note
            # to the snobol4 row as a SEVENTH cell (dc87ee1c), and that one stray `|` deleted snobol4 from
            # every reader of this table at once -- `write` refused for the entire fleet with "no row for
            # language 'snobol4'", `check` quietly stopped reporting its staleness, and `agree` went on
            # printing GATE PASS over 11 mirrored pairs that held not one of snobol4's cells.
            # ⭐ THE SHAPE WORTH REMEMBERING IS THE MESSAGE, NOT THE MISSING `else`. "No row for language
            # 'snobol4'. Rows present: icon, pascal, ..." is well-formed, confident, and points AWAY from the
            # defect sitting one line above it -- so seat07 read it exactly as written and went looking for a
            # missing entry in an internal registry that does not exist. An instrument that answers a
            # NARROWER question than the one it is thought to answer (here: "is it in my dict?" read as "does
            # it have a row?") is this project's recurring bug, the same shape as `command -v` for an oracle.
            # The header already PROVES its shape via table_shape_error; a row is owed the same courtesy.
            skipped[c[0]] = (i, len(c))
            sys.stderr.write("⚠ SCORE.md line %d: row %r has %d columns, this table has %d -- SKIPPED, so every "
                             "reading below OMITS it. It is malformed, not absent: fix that row's '|' count.\n"
                             % (i + 1, c[0], len(c), PROV_COL + 1))
    return hdr, rows, skipped


def merge_clause(existing, key, value):
    # ⛔⭐ ONE CELL, SEVERAL MEASUREMENTS, AND NO RUNNER MAY CLOBBER ITS SIBLINGS. The `Vendor / package
    # suites` cell holds MORE THAN ONE suite per language -- Icon carries Arizona AND JCON, Prolog carries
    # SWI AND GNU -- so a runner that wrote the whole cell would delete the other suite's number every time
    # it ran, and the two would take turns erasing each other with nobody ever seeing a wrong value sit
    # still long enough to be questioned. Keyed clauses make each runner the author of ITS OWN clause and
    # nothing else. Same mechanism the provenance column uses, which is why it is one function.
    clauses = [c.strip() for c in existing.split(";") if c.strip()] if existing and existing != "—" else []
    new = "%s: %s" % (key, value)
    for i, c in enumerate(clauses):
        # ⛔⭐ MATCH `Key: text` OR `Key text` -- the colon is NOT guaranteed, and assuming it costs a TWIN.
        # Measured live while wiring this (hq_T 2026-09-03): the icon vendor cell already read
        # "Arizona 40/89 · 41/89; JCON 43/81 · 41/81" -- keyed by NAME but with no colon, written by hand
        # months before this helper existed. A colon-only regex missed them, appended "Arizona: 40/89 ..."
        # beside the original, and produced exactly the stale-twin shape the gate on this file exists to
        # prevent -- in the one function whose whole job is to prevent it. A legacy format nobody wrote
        # down is still the format; the migration has to meet it where it is, not where the new code wishes
        # it were. The trailing (?=\s|$) keeps `Arizona` from matching a clause about `ArizonaExtended`.
        if re.match(r"^%s\s*(?::|(?=\s|$))" % re.escape(key), c):
            clauses[i] = new
            return "; ".join(clauses)
    clauses.append(new)
    return "; ".join(clauses)


def merge_prov(existing, column, stamp):
    # One `<column>: <stamp>` clause per measured column, ';'-joined.  Rewrite ours IN PLACE (never
    # append beside a stale twin -- that is the defect the FACT RULE names by name), keep everyone
    # else's clause untouched, and preserve their order so a diff of this file reads as one cell
    # changing rather than as the whole row churning.
    clauses = [c.strip() for c in existing.split(";") if c.strip()] if existing and existing != "—" else []
    new = "%s: %s" % (column, stamp)
    for i, c in enumerate(clauses):
        if re.match(r"^%s\s*:" % re.escape(column), c):
            clauses[i] = new
            return "; ".join(clauses)
    clauses.append(new)
    return "; ".join(clauses)


def cell_prose_loss(before, new):
    # ⛔⭐ THE DEFECT THIS GUARDS: a wholesale cell overwrite silently drops whatever hand-written prose it
    # never modelled, WITH THE MEASURED NUMBERS UNCHANGED -- so nothing about the diff looks wrong (task
    # score-md-runners-rewrite-a-cell-in-place-and-silently-discard-the-prose-they-never-modelled, minted
    # off hq_B's 2026-09-03 ICN4 finding; repaired by hand in .github 46ff295c). That repair is the fixture:
    # `before` carried "... rc=0). Re-confirmed on a newer tree and identical across two consecutive runs
    # (before and after a rebase onto origin), as the control arm for the rung-11 LCO landing: the shared
    # alternative-trampoline change in emit.cpp moved nothing here." and the runner's own `new` carried only
    # the measurement up to "rc=0)." -- the whole trailing sentence had no home in the new text.
    # ⛔ MATCHED BY CONTENT, NEVER BY POSITION OR EXACT PREFIX. The SAME repair shows why: the runner's own
    # boilerplate drifted in punctuation between the two captures ("SKIP=0 · MISSING=0" became "SKIP=0
    # MISSING=0"; a comma moved beside the citation backtick) with no human involved -- an exact-prefix or
    # exact-substring check would have flagged that reformatting exactly as hard as the real sentence loss,
    # which is how a helper like this earns a "too noisy, ignore it" reputation and stops getting listened to.
    # So a chunk of `before` only counts as LOST when most of it cannot be found, in one contiguous run,
    # anywhere in `new` -- reformatting leaves a chunk overwhelmingly intact; a dropped sentence leaves none
    # of it.
    if not before or before == "—" or before == new:
        return []
    chunks = [c.strip() for c in re.split(r'(?<=[.!?])\s+(?=[A-Z0-9⚠⛔⭐✅])', before) if c.strip()]
    lost = []
    for chunk in chunks:
        if chunk in new:
            continue
        # ⛔ SUM every matching block, never just the longest one. Two independent formatting nips close
        # together (a dropped '·', a comma that moved) fragment one legitimate reformat into three short
        # contiguous pieces that individually clear nowhere near a single-block threshold, even though
        # together they cover nearly all of `chunk` -- measured against the real 46ff295c drift, which
        # carries exactly two such nips in one sentence.
        sm = difflib.SequenceMatcher(None, new, chunk, autojunk=False)
        matched = sum(blk.size for blk in sm.get_matching_blocks())
        coverage = (matched / len(chunk)) if chunk else 1.0
        if coverage < 0.6:
            lost.append(chunk)
    return lost


# ⛔⭐ "unknown-seat" DEFEATED THE MEASURER GUARD BELOW BY BEING NON-EMPTY. Every runner spelled its default
# `${S4E_SEAT:-}`, so a board run from a plain shell (no hook exporting S4E_SEAT) satisfied the
# "an unattributed row is a claim with nobody behind it" check with a string carrying exactly zero attribution
# -- measured live on this root's own snobol4 board row, 2026-09-03 22:45. ⭐ The identity was never unknown:
# it is a FACT ON DISK, the same root-path map `s4e_msg.sh:91-99` reads, so the cure is to DERIVE it rather
# than to refuse harder. Refusal is kept for the case the map cannot answer -- an unrecognised root.
_PLACEHOLDER_MEASURERS = ("unknown-seat", "unknown", "unknown-hq", "-", "?")
def derive_measurer():
    root = os.path.abspath(S4E).rstrip("/")
    fixed = {"/home/claude": "ceo", "/home/claude_C": "hq_C", "/home/claude_P": "hq_P",
             "/home/claude_B": "hq_B", "/home/claude_T": "hq_T", "/home/claude_U": "hq_U",
             "/home/claude_S": "hq_S", "/home/claude_I": "hq_I", "/home/claude_R": "hq_R"}
    if root in fixed:
        return fixed[root]
    m = re.match(r"^/home/claude([0-9]{1,2})$", root)
    if m:
        return "seat%02d" % int(m.group(1))
    return ""


def write_grid_direct(a):
    gkey = GRID_DIRECT[a.column]
    text = a.text.strip()
    if "|" in text or "|" in (a.measurer or ""):
        die("a '|' in the cell text would silently split the markdown row into the wrong columns -- rephrase it")
    if ";" in text:
        text = text.replace(";", "·")
    if not re.search(r"\d+\s*/\s*\d+", text):
        die("--text for the grid %s column carries no N/M fraction (%r); the ladder cell states PASS p/t over the graded population" % (gkey, text))
    if "\n" in text:
        die("--text spans lines; a markdown table row is one line")
    if os.path.abspath(SCORE_MD) == os.path.abspath(os.path.join(S4E, ".github", "SCORE.md")):
        dirty = tree_is_dirty()
        if dirty:
            print("⚠ SCORE.md ROW SKIPPED — %s %s uncommitted; this run measured a tree nobody else can check out."
                  % (", ".join(dirty), "has" if len(dirty) == 1 else "have"))
            return 0
    lines = open(SCORE_MD, encoding="utf-8").read().split("\n")
    _gh, grows, gskip = find_grid(lines)
    if a.lang in gskip:
        _gl, _gn = gskip[a.lang]
        die("the September-10 grid row for %r is MALFORMED, NOT ABSENT (line %d has %d columns, the grid has %d)" % (a.lang, _gl + 1, _gn, GRID_NCOLS))
    if a.lang not in grows:
        die("no row for language %r in the September-10 grid. Rows present: %s" % (a.lang, ", ".join(sorted(grows))))
    gi = GRID_COLUMNS[gkey][0]
    gbefore = grows[a.lang][gi]
    gbare = re.sub(r"\s*" + GRID_STAMP_RE + r"\s*$", "", gbefore).strip()
    gchunks = [c for c in re.split(r'(?<=[.!?])\s+(?=[A-Z0-9⚠⛔⭐✅])', gbare) if c.strip()]
    if len(gchunks) > 1:
        die("grid %s for %s carries %d sentences no runner models -- a person wrote them; fold what is still true into --text or hand-edit the cell:\n%s"
            % (gkey, a.lang, len(gchunks), "\n".join("  - %s" % c[:160] for c in gchunks)))
    gnew = (text + " " + GRID_STAMP % (time.strftime("%Y-%m-%d"), a.measurer or derive_measurer() or "unknown")).strip()
    if a.dry_run:
        print("WOULD REWRITE grid %s for %s\n  was: %s\n  now: %s" % (gkey, a.lang, gbefore, gnew))
        return 0
    done = False
    for gl, gline in enumerate(lines):
        if gline.startswith("| %s |" % a.lang):
            gc = [x.strip() for x in gline.strip().strip("|").split("|")]
            if len(gc) == GRID_NCOLS and gc[0] == a.lang:
                gc[gi] = gnew
                lines[gl] = "| " + " | ".join(gc) + " |"
                done = True
                print("SCORE.md: grid %s for %s rewritten in place (line %d)\n  was: %s\n  now: %s" % (gkey, a.lang, gl + 1, gbefore, gnew))
                break
    if not done:
        die("internal: find_grid saw a %s row but no line matched it" % a.lang)
    lines = mark_grid_stamp(lines)
    open(SCORE_MD, "w", encoding="utf-8").write("\n".join(lines))
    print("⛔ NOT DONE UNTIL PUSHED: commit .github/SCORE.md with the landing that carried this measurement.")
    return 0


def cmd_write(a):
    if not a.measurer or a.measurer.strip().lower() in _PLACEHOLDER_MEASURERS:
        stale = a.measurer
        a.measurer = os.environ.get("S4E_SEAT", "").strip() or derive_measurer()
        if not a.measurer:
            die("--measurer is required: an unattributed row is a claim with nobody behind it "
                "(got %r, and the root %r is not in the seat map, so it could not be derived)" % (stale, S4E))
        if stale:
            print("⚠ measurer %r is a placeholder, not an identity -- derived %r from the root %s instead "
                  "(the caller should pass ${S4E_SEAT:-} and let this helper resolve it)."
                  % (stale, a.measurer, S4E))
    if a.column in GRID_DIRECT:
        return write_grid_direct(a)
    if a.column not in COLUMNS:
        die("unknown --column %r. Known: %s" % (a.column, ", ".join(sorted(COLUMNS))))
    text = a.text.strip()
    if "|" in text or "|" in a.measurer:
        die("a '|' in the cell text would silently split the markdown row into the wrong columns -- rephrase it")
    # ⛔ ';' IS merge_clause's OWN DELIMITER (and merge_prov's), so text carrying one gets silently
    # torn apart the NEXT time anyone writes into a shared clause cell -- measured live landing the
    # IPL row: a --text with two internal ';'s left two orphaned clause fragments (neither started
    # with the --suite key, so neither matched, so neither got replaced or removed) sitting beside
    # the real clause on the very next write. ⭐ REFUSING outright used to make this PERMANENT for any
    # cell whose only honest phrasing needs one: the snobol4 vendor cell was hand-written with
    # semicolons months before this guard existed, so no runner could ever write that cell again --
    # measured twice in one sitting landing spitbol_testpgms (row
    # score-row-write-refuses-a-semicolon-forever-so-a-hand-edited-cell-can-never-be-runner-written).
    # NORMALISE instead of refusing: a runner's ';' becomes '·' (the project's own convention already
    # used throughout SCORE.md), which keeps the ONE property this guard exists for -- the stored cell
    # never contains a raw ';' that isn't merge_clause's own inserted delimiter, so nothing fragments
    # on the next write -- without ever locking a cell out of being rewritten again.
    if ";" in text:
        normalised = text.replace(";", "·")
        print("⚠ ';' in --text is merge_clause's own clause delimiter -- normalised to '·' so this "
              "write cannot fragment on the next one: %r -> %r" % (text, normalised))
        text = normalised
    if not re.search(r"\d", text):
        die("--text carries no digit (%r). A leaderboard cell states a measurement; a cell with no number "
            "is prose, and the FACT RULE asks for the runner's own board line" % text)
    if "\n" in text:
        die("--text spans lines; a markdown table row is one line")
    # The clean-tree condition guards the REAL leaderboard only. selftest writes to a scratch copy, whose
    # whole purpose is to be written while the working tree is mid-change; applying it there would make the
    # gate fail for every seat who ran it during a landing, i.e. exactly when it matters.
    if os.path.abspath(SCORE_MD) == os.path.abspath(os.path.join(S4E, ".github", "SCORE.md")):
        dirty = tree_is_dirty()
        if dirty:
            print("⚠ SCORE.md ROW SKIPPED — %s %s uncommitted; this run measured a tree nobody else can check out."
                  % (", ".join(dirty), "has" if len(dirty) == 1 else "have"))
            print("  The number above stands as a scouting datum. Commit the tree and re-run to land the row"
                  "  (ceo CEO-174: a dirty-tree number describes no checkable tree).")
            return 0
    lines = open(SCORE_MD, encoding="utf-8").read().split("\n")
    hdr, rows, skipped = find_table(lines)
    if a.lang in skipped:
        _ln, _n = skipped[a.lang]
        die("row %r EXISTS at %s line %d but carries %d columns where this table has %d, so it was SKIPPED -- "
            "it is MALFORMED, NOT ABSENT. Repair that row's '|' count and re-run; nothing about this language "
            "or this helper's registry is at fault. (Every other reader omits it too, and the other two do so "
            "SILENTLY: `check` drops its staleness and `agree` mirrors none of its cells.)"
            % (a.lang, SCORE_MD, _ln + 1, _n, PROV_COL + 1))
    if a.lang not in rows:
        die("no row for language %r in the grid. Rows present: %s" % (a.lang, ", ".join(sorted(rows))))
    i, cells = rows[a.lang]
    idx = COLUMNS[a.column][0]
    stamp = "%s · RT_OPT=%s · %s · %s" % (tree_stamp(), rt_opt(), time.strftime("%Y-%m-%d %H:%M %Z"), a.measurer)
    if a.modes:
        stamp = "%s · modes %s" % (stamp, a.modes)
    before = cells[idx]
    # --suite names ONE measurement inside a cell that holds several. Without it the cell is replaced
    # wholesale, which is right for `board`/`entries` (one suite, one number) and wrong for `vendor`.
    # ⭐ The provenance clause is keyed by the SUITE name when there is one, not "column/suite" -- because
    # the cell already uses bare suite names (`Arizona:`, `JCON:`, `STRICT:`, `smoke:`) and a new key shape
    # would sit beside the old clause instead of replacing it. Match the file's convention, don't impose one.
    key = a.suite or a.column
    # ⛔ A WHOLESALE OVERWRITE MUST NOT SILENTLY DISCARD PROSE IT NEVER MODELLED (see cell_prose_loss).
    # Scoped to the no-suite path on purpose: --suite already merges via merge_clause, touching only its
    # own clause, and no incident has been measured against that narrower path -- guarding it too would be
    # speculative rather than measured, so it is left for a future row if one ever is.
    # ⭐ MODELLED AS A SUPERSESSION, NOT A REFUSAL (see SUPERSEDE_MARKER above for the full reasoning and
    # why this does not reopen hq_T's REFUSE ruling). The MOST RECENT reading inside `before` rides
    # forward BYTE-FOR-BYTE, so nothing live is lost by construction -- the defensive re-check below is
    # what makes that a proof, not an assumption.
    # ⛔⭐ FOLD DEPTH IS CAPPED AT ONE ARCHIVED READING (hq_T ruling on
    # ruling-supersede-fold-approved-with-a-bound). Carrying the FULL `before` forward every time embeds
    # an already-superseded chain again on every later fold, so the cell grows by its whole history on
    # every landing -- measured live: the snobol4 board cell is already ~20 KB on one line. Anything past
    # the first SUPERSEDE_MARKER in `before` was already labelled not-asserted by an earlier fold, so
    # dropping it loses no claim anyone was still entitled to rely on.
    if a.suite:
        cells[idx] = merge_clause(before, a.suite, text)
    else:
        new_text = text
        lost = cell_prose_loss(before, text)
        if lost:
            carried = before.split(SUPERSEDE_MARKER, 1)[0].rstrip()
            new_text = "%s %s %s" % (text, SUPERSEDE_MARKER, carried)
            still_lost = cell_prose_loss(carried, new_text)
            if still_lost:
                die("internal error modelling %s/%s: folding the kept reading forward still lost %d "
                    "sentence(s), which should be impossible when it rides forward verbatim -- "
                    "hand-edit the cell instead of trusting this writer:\n%s"
                    % (a.lang, a.column, len(still_lost), "\n".join("  - %s" % l for l in still_lost)))
        cells[idx] = new_text
    cells[PROV_COL] = merge_prov(cells[PROV_COL], key, stamp)
    newline = "| " + " | ".join(cells) + " |"
    if a.dry_run:
        print("WOULD REWRITE %s line %d" % (SCORE_MD, i + 1))
        print("  was: %s" % before)
        print("  now: %s" % cells[idx])
        print("  prov: %s" % cells[PROV_COL])
        return 0
    lines[i] = newline
    # ⛔⭐⭐ WRITE UPDATES BOTH TABLES, OR SAYS OUT LOUD THAT IT COULD NOT. Until now a runner wrote only the
    # standardized DISPLAY row, so every board run silently staled the September-10 grid cell beside it --
    # the dual-write gap the agree gate reports as one-sided populations (20 of them the day this landed),
    # and the reason that gate re-reds itself no matter who reconciled it last. hq_C measured the same
    # thing independently and named the cure: the writer must move both halves.
    # ⛔ IT NEVER OVERWRITES PROSE TO DO IT. The grid's M and V cells carry hand-written commentary that no
    # runner models, so the grid write is attempted ONLY when it would drop nothing (cell_prose_loss empty
    # -- the same detector hq_B validated on the real .github 46ff295c incident). When prose IS at risk the
    # display still lands, the grid is left alone, and the runner NAMES the cell and the sentence.
    # ⭐ THAT REFUSAL IS THE POINT, NOT A SHORTFALL. hq_B's preserve-or-refuse question is RULED REFUSE here
    # (see the ask on that row): the endangered prose was written ABOUT THE OLD MEASUREMENT, so carrying it
    # forward automatically would attach an old sentence to a new number -- silently converting a true
    # statement into a false one, which is the very failure class the prose guard exists to stop, merely
    # relocated. A human decides whether a sentence survives its number. What changes today is that a
    # staling grid cell stops being SILENT: before, it just drifted; now the write that stales it says so
    # at the moment it happens, with the cell and the blocking sentence named.
    gkey = GRID_MIRROR.get(a.column)
    gnote = ""
    if gkey:
        try:
            _gh, growsg, gskipw = find_grid(lines)
        except SystemExit:
            growsg, gskipw = {}, {}
        if a.lang in gskipw:
            # ⛔ The grid row EXISTS and cannot be read. Saying nothing here would let a write land in the
            # display while its grid twin quietly stops being staleness-checked -- the silence this whole
            # patch is about. It is a note, not a refusal: the display write itself is still honest.
            _gl, _gn = gskipw[a.lang]
            gnote = ("\n  ⚠ the September-10 grid row for %s is MALFORMED, NOT ABSENT (line %d has %d columns, "
                     "the grid has %d) -- its cell was NOT staleness-checked against this write" % (a.lang, _gl + 1, _gn, GRID_NCOLS))
        if a.lang in growsg:
            gi = GRID_COLUMNS[gkey][0]
            grow = growsg[a.lang]
            gbefore = grow[gi]
            # Strip any prior stamp before comparing, or the marker this write appended last time would
            # itself read as prose the next write is about to lose.
            gbare = re.sub(r"\s*" + GRID_STAMP_RE + r"\s*$", "", gbefore).strip()
            gnew = merge_clause(gbare, a.suite, text) if a.suite else text
            # ⛔⭐ cell_prose_loss ALONE IS THE WRONG GATE HERE AND MEASURING IT SAID SO. It answers "does
            # `new` still contain everything `before` said", so the OLD MEASUREMENT -- which this write
            # exists to supersede -- always reads as a lost sentence. Run against the live board it blocked
            # snobol4's grid cell, whose entire content is one measurement line and nothing else: the
            # dual-write would have refused every cell on the file and been useless while looking careful.
            # ⭐ So the auto-update is scoped to the case that is UNAMBIGUOUS rather than merely passing a
            # check: a grid cell that is ONE sentence carrying a measurement is a pure mirror of the display
            # number, with no human commentary to lose. The moment a cell has a second sentence, a person
            # wrote it, and a runner does not get to decide whether it survives its number.
            gchunks = [c for c in re.split(r'(?<=[.!?])\s+(?=[A-Z0-9⚠⛔⭐✅])', gbare) if c.strip()]
            gpure = len(gchunks) <= 1 and re.search(r"\d+\s*/\s*\d+|PASS=|FAIL=|\bpass\b", gbare or "", re.I)
            glost = [] if gpure else cell_prose_loss(gbare, gnew)
            gnew = (gnew + " " + GRID_STAMP % (time.strftime("%Y-%m-%d"), a.measurer or derive_measurer() or "unknown")).strip()
            # ⛔⭐⭐ THE TWO COLUMNS HAVE DIFFERENT CONTRACTS AND A VERBATIM COPY BETWEEN THEM PUBLISHES A
            # WRONG PERCENTAGE. Measured by hq_P as the first customer of the dual-write, on the live board:
            # the DISPLAY board column wants "the runner's OWN printed board line", and `test_corpus_snobol4.sh`
            # prints `m3 PASS=1698 FAIL=0 · m4 PASS=1698 FAIL=0 SKIP=0 MISSING=0` -- which contains NO N/M
            # fraction. The GRID M column is PARSED for one. So the copy left a fractionless grid cell,
            # cell_fractions returned nothing for M, and the progress line silently fell back to the V column
            # alone: snobol4 read **55%** instead of 91% -- confidently wrong by 36 points, on the exact line
            # the ceo reads to answer Lon, and NOT marked `?`.
            # ⭐ PASCAL HID IT, which is the part worth keeping: pascal's board line happens to be phrased
            # `166/166 · 5/5 · 161/161`, so its fraction survived the copy and pascal stayed correct the whole
            # time. One language passing and one failing off the same code path for no reason but PROSE STYLE
            # -- which is exactly why it read as a snobol4 problem rather than as a dual-write problem, and why
            # checking one language would have cleared it.
            # ⛔ DERIVING A FRACTION IS THE OBVIOUS FIX AND IT IS WORSE THAN THE BUG. `PASS=1698 FAIL=0` does
            # not carry the population: the real denominator is 1736 (xfail and friends live outside the
            # pass/fail split), so pass/(pass+fail) would publish 1698/1698 = 100%. A silent 100% is far more
            # dangerous than a silent 55%. So this REFUSES the grid half and says what to do, the same shape
            # as the prose guard: wrong is worse than missing on this line.
            if not cell_fractions(gnew)[0]:
                gnote = ("  ⚠ grid %s NOT updated -- this board line carries NO `N/M` fraction, and the grid %s "
                         "column is PARSED for one. Writing it would leave a cell that reads as ZERO for this "
                         "population and publish a confidently wrong percentage rather than a `?`.\n"
                         "      text: %s\n"
                         "      Give the board line a fraction (e.g. `m3 1698/1736 FAIL=0`) so ONE text "
                         "satisfies both columns, then re-run. Do NOT let this helper derive pass/(pass+fail) "
                         "-- that denominator is not the population and would publish 100%%."
                         % (gkey, gkey, gnew[:200]))
            elif gbare == gnew:
                gnote = "  grid %s: already agrees, nothing to write" % gkey
            elif not gpure:
                glost = glost or [gbefore]
                gnote = ("  ⚠ grid %s NOT updated -- it carries %d sentence(s) no runner models, "
                         "so this measurement now sits in the display only and the grid cell is STALE BY THIS "
                         "WRITE. Fold what is still true into --text, or hand-edit the grid cell:\n%s"
                         % (gkey, len(glost), "\n".join("      - %s" % l[:160] for l in glost)))
            else:
                for gl, gline in enumerate(lines):
                    if gline.startswith("| %s |" % a.lang) and gl != i:
                        gc = [x.strip() for x in gline.strip().strip("|").split("|")]
                        if len(gc) == GRID_NCOLS and gc[0] == a.lang:
                            gc[gi] = gnew
                            lines[gl] = "| " + " | ".join(gc) + " |"
                            gnote = "  grid %s: updated in the same call (line %d)" % (gkey, gl + 1)
                            break
    lines = mark_grid_stamp(lines)
    open(SCORE_MD, "w", encoding="utf-8").write("\n".join(lines))
    print("SCORE.md: %s/%s rewritten in place (line %d)" % (a.lang, a.column, i + 1))
    print("  was: %s" % before)
    print("  now: %s" % cells[idx])
    print("  prov: %s: %s" % (a.column, stamp))
    if gnote:
        print(gnote)
    print("⛔ NOT DONE UNTIL PUSHED: commit .github/SCORE.md with the landing that carried this measurement.")
    return 0


def mark_grid_stamp(lines):
    # ⛔⭐ THE WHOLE-GRID STAMP STOPS SPEAKING FOR ROWS IT NO LONGER MEASURED.  The `_tree: ... generated
    # ..._` line under the grid was written by util_build_score_md.py to pin the ONE tree that produced
    # ALL the cells above it -- true on the day of a full regeneration, and false the moment any row is
    # rewritten from a different tree.  Measured on this file 2026-09-03: the stamp read `generated
    # 2026-09-02T02:35Z` while six of eight rows carried 2026-09-03 provenance from four different
    # sessions.  That is not a stale label, it is a WRONG one -- it claims coverage it does not have,
    # and unlike a stale row nothing about it looks old.  So a row-write demotes it, once, to what it
    # is actually still evidence of.
    tag = "  (LAST FULL REGENERATION ONLY -- rows rewritten since carry their own tree in the last column)"
    for i, l in enumerate(lines):
        if l.startswith("_tree: ") and l.endswith("_"):
            if tag.strip() not in l:
                lines[i] = l[:-1] + tag + "_"
            return lines
    return lines


def cmd_check(a):
    # Staleness is measured against origin, never against the local checkout: a row is stale when the
    # WORLD has moved past the tree it was measured on, and a seat's own unpushed commits are not the
    # world.  A hash origin has never heard of is its own state (UNKNOWN), never silently zero.
    lines = open(SCORE_MD, encoding="utf-8").read().split("\n")
    hdr, rows, _skipped = find_table(lines)
    # ⛔ --no-fetch EXISTS SO THIS CAN SIT IN A TEST LOOP, AND IT DOWNGRADES THE CLAIM, NOT JUST THE COST.
    # Without a fetch, "behind origin/main" means behind the origin/main THIS CHECKOUT LAST HEARD ABOUT,
    # which understates staleness by however long since the last fetch. That is fine for a gate proving the
    # check RUNS, and not fine for a handoff answering "is the board current" -- so the two callers differ,
    # and the output says which question it answered rather than letting the reader assume the stronger one.
    if a.no_fetch:
        print("(--no-fetch: measured against this checkout's last-known origin/main, which may itself be behind)")
    else:
        for r in REPOS:
            if git(r, "fetch", "-q", "origin") is None and git(r, "rev-parse", "HEAD") is not None:
                print("WARN: could not fetch origin for %s -- staleness below may itself be stale" % r)
    worst, unknown, adrift, out = 0, 0, 0, []
    # ⛔⭐⭐ GRADE THE HASH A READER ACTUALLY SEES, NOT ONLY THE ONE IN THE Tree COLUMN (hq_T 2026-09-05,
    # off seat13's make-test refusal report).  Until now this scanned cells[PROV_COL] and nothing else, on the
    # premise that `write` stamps every measurement there -- which it does (merge_prov, one clause per column).
    # The premise holds for writes THROUGH THIS HELPER and silently fails for the hand-edit, and the board is
    # full of hand-edits.  The witness that found it: snobol4's Master board CELL reads "LANE RE-MEASURE
    # 2026-09-05 (hq_P) ... on SCRIP `f3f4870d7`", while its `board:` provenance clause still reads hq_B
    # 2026-09-04 on `7d7ff2dc5`.  One cell, two trees, two measurers, and `check` had never once graded
    # f3f4870d7 -- the number every human reader of that row is looking at.
    # ⛔ THE DANGEROUS DIRECTION IS THE QUIET ONE.  Here both trees happened to be past the threshold, so the
    # verdict survived and only the attribution was wrong.  Reverse the freshness and it inverts: a cell
    # re-measured by hand onto today's tree reads STALE off its old stamp (a seat re-runs a suite that was
    # already current), and -- worse -- a cell left stale beside a freshly-stamped clause reads **ok** and
    # nobody re-runs anything.  A staleness check that cannot see the cell it is grading is the FACT RULE's
    # own failure mode wearing the FACT RULE's clothes.
    # ⭐ So: scan every measured cell AND the Tree column, name where each tree came from, and give the
    # hand-edit its own state.  UNSTAMPED is not STALE and not UNKNOWN -- the tree may be perfectly current;
    # what is broken is that no provenance clause claims it, so nobody is named for the number and the next
    # `write` to that column will overwrite the stamp without touching the prose that disagrees with it.
    HASH_RX = re.compile(r"SCRIP `([0-9a-f]{7,40})(?:-DIRTY)?`")
    by_col = sorted((idx, key) for key, (idx, _label) in COLUMNS.items())
    _dist = {}

    def distance(h):
        # commits from h to origin/main; None when origin has never heard of h.  Memoized because the same
        # tree is routinely stamped in several columns and a rev-list per mention is pure cost.
        if h not in _dist:
            n = git("SCRIP", "rev-list", "--count", "%s..origin/main" % h)
            _dist[h] = None if n is None else int(n)
        return _dist[h]

    def newest(hashes):
        # The CURRENT claim of a cell is the newest tree it names -- fewest commits behind origin.  A measured
        # cell is a running history (⛔ SUPERSEDED READING BELOW ... and so on down), so the trees buried in
        # its prose are retired by construction and grading them is noise, not coverage.  Ranking by commit
        # DISTANCE and not by position is what makes that safe: it does not care whether the newest reading
        # was appended at the top, spliced into the middle, or introduced by a word this parser never models.
        ranked = [(distance(h), h) for h in set(hashes)]
        live = sorted((d, h) for d, h in ranked if d is not None)
        return (live[0] if live else (None, sorted(h for d, h in ranked)[0] if ranked else None))

    for lang in sorted(rows):
        i, cells = rows[lang]
        # The Tree column is ';'-joined "<key>: <stamp>" clauses (merge_prov), one per measured column plus a
        # free-form one per vendor suite.  Split it so a stamp can be held against the very cell it stamps.
        stamp_of, extra = {}, []
        for clause in cells[PROV_COL].split(";"):
            hs = HASH_RX.findall(clause)
            if not hs:
                continue
            mm = re.match(r"^\s*([A-Za-z0-9_.-]+)\s*:", clause)
            key = mm.group(1) if mm else "(unkeyed)"
            if key in COLUMNS:
                stamp_of.setdefault(key, hs[0])
            else:
                extra.append((key, hs[0]))
        graded_any = False
        for idx, key in by_col:
            if idx >= len(cells):
                continue
            hs = HASH_RX.findall(cells[idx])
            if not hs:
                continue
            graded_any = True
            d, h = newest(hs)
            if d is None:
                out.append("  %-9s %-9s UNKNOWN   %s is not a commit origin knows" % (lang, key, h))
                unknown += 1
                continue
            worst = max(worst, d)
            flag = "STALE" if d >= a.threshold else "ok   "
            note = ""
            # ⛔⭐⭐ THE DEFECT THIS ARM EXISTS FOR (hq_T 2026-09-05, off seat13's make-test refusal report).
            # Until now `check` read cells[PROV_COL] and nothing else, on the premise that every measurement
            # arrives through `write`, which stamps there.  That premise holds for writes through this helper
            # and fails silently for the hand-edit -- and the board is full of hand-edits.  The witness:
            # snobol4's Master board cell reads "LANE RE-MEASURE 2026-09-05 (hq_P) ... on SCRIP `f3f4870d7`",
            # while its `board:` clause still reads hq_B 2026-09-04 on `7d7ff2dc5`.  One cell, two trees, two
            # measurers, and the number every human reader is looking at had never once been graded.
            # ⛔ THE DANGEROUS DIRECTION IS THE QUIET ONE.  There both trees were past the threshold, so only
            # the attribution was wrong.  Reverse the freshness and it inverts: a cell re-measured by hand onto
            # today's tree reads STALE off its old stamp and a seat re-runs a suite that was already current;
            # a cell left stale beside a freshly-stamped clause reads **ok** and nobody re-runs anything.  A
            # staleness check that cannot see the cell it grades is the FACT RULE's own failure mode wearing
            # the FACT RULE's clothes.
            # ⭐ Reported as its own state, never folded into STALE, because the cure is different: STALE says
            # "re-run the suite", ADRIFT says "the number came in by hand, so the stamp names the wrong tree
            # and the wrong measurer" -- and a row can be perfectly CURRENT and still adrift.
            s = stamp_of.get(key)
            if s is None:
                note = "  ⚠ ADRIFT: no `%s:` provenance clause names this cell at all" % key
                adrift += 1
            elif s != h:
                sd = distance(s)
                if sd is None:
                    note = "  ⚠ ADRIFT: `%s:` clause stamps %s, which origin does not know" % (key, s)
                    adrift += 1
                elif sd > d:
                    note = "  ⚠ ADRIFT: `%s:` clause stamps %s (%d behind), %d commit(s) OLDER than the tree this cell claims -- hand-edited, not written through this helper" % (
                        key, s, sd, sd - d)
                    adrift += 1
            out.append("  %-9s %-9s %s %4d commits behind origin/main (cell claims %s)%s" % (
                lang, key, flag, d, h, note))
        # Vendor-suite stamps live only in the Tree column -- they have no cell of their own, so they are
        # graded here or nowhere.
        for key, h in sorted(set(extra)):
            d = distance(h)
            if d is None:
                out.append("  %-9s %-9s UNKNOWN   %s is not a commit origin knows" % (lang, key, h))
                unknown += 1
                continue
            graded_any = True
            worst = max(worst, d)
            out.append("  %-9s %-9s %s %4d commits behind origin/main (Tree-column stamp %s)" % (
                lang, key, "STALE" if d >= a.threshold else "ok   ", d, h))
        if not graded_any:
            out.append("  %-9s UNPINNED  (no checkable SCRIP hash in any measured cell or the Tree column)" % lang)
            unknown += 1
    print("SCORE.md staleness (threshold %d commits):" % a.threshold)
    for l in out:
        print(l)
    print("worst=%d unpinned/unknown=%d adrift=%d" % (worst, unknown, adrift))
    rc = 0
    if worst >= a.threshold:
        print("⚠ WARN: %d row-measurement(s) at or past the %d-commit staleness threshold -- re-measure and rewrite the row." % (
            sum(1 for l in out if "STALE" in l), a.threshold))
        rc = 1
    if adrift:
        # ⭐ Reported SEPARATELY from staleness because the cure is different and the two are independent.
        # Stale says "re-run the suite"; UNSTAMPED says "the number in that cell did not come through this
        # helper, so its stamp names the wrong tree and the wrong measurer" -- the fix is to re-write the cell
        # with `write --column <col>`, which is what makes the stamp and the prose agree again.  A row can be
        # perfectly current and still adrift, which is exactly why it may not be folded into the WARN above.
        print("⚠ WARN: %d cell(s) ADRIFT from their provenance stamp -- the cell was hand-edited, so the Tree "
              "column names a different tree (and a different measurer) than the number a reader sees. Re-write "
              "each with `write --column <col>` so the stamp and the prose agree again." % adrift)
        rc = 1
    return rc


def cmd_selftest(a):
    # Proves the two things this helper must never get wrong -- that it REWRITES rather than appends,
    # and that every refusal path actually refuses -- against a scratch copy, never the real board.
    import tempfile, shutil
    global SCORE_MD
    real = SCORE_MD
    ok = True
    d = tempfile.mkdtemp(prefix="score_row_selftest.")
    try:
        SCORE_MD = os.path.join(d, "SCORE.md")
        shutil.copy(real, SCORE_MD)
        # ⛔ SEED A KNOWN CELL BEFORE PROVING REWRITE-IN-PLACE, rather than inherit whatever prose the LIVE
        # rebus/board cell happens to carry that day. This selftest's own claim ("rewrite-in-place holds")
        # has never depended on the starting content -- only on two known writes landing as one cell -- but
        # the live cell is real board prose (see cell_prose_loss below), and coupling a selftest's pass/fail
        # to today's live board text is exactly the kind of incidental fragility this file exists to avoid
        # elsewhere. Seeding it here keeps that claim provably independent of the day it happens to run.
        _seed_lines = open(SCORE_MD, encoding="utf-8").read().split("\n")
        _sh, _sr, _ = find_table(_seed_lines)
        _sri, _src = _sr["rebus"]
        _src[COLUMNS["board"][0]] = "—"
        _seed_lines[_sri] = "| " + " | ".join(_src) + " |"
        open(SCORE_MD, "w", encoding="utf-8").write("\n".join(_seed_lines))
        n0 = len(open(SCORE_MD, encoding="utf-8").read().split("\n"))
        class A: pass
        for run in (1, 2):
            a2 = A(); a2.lang = "rebus"; a2.column = "board"; a2.measurer = "selftest"
            a2.text = "master: m3 %d/48 · m4 %d/48 (selftest, not a measurement)" % (run, run)
            a2.modes = "m3,m4"; a2.dry_run = False; a2.suite = ""
            cmd_write(a2)
        n1 = len(open(SCORE_MD, encoding="utf-8").read().split("\n"))
        body = open(SCORE_MD, encoding="utf-8").read()
        if n1 != n0:
            print("SELFTEST FAIL: line count moved %d -> %d; a row-write must rewrite IN PLACE, never append" % (n0, n1)); ok = False
        else:
            print("SELFTEST: two writes to one cell left the line count unchanged -- rewrite-in-place holds")
        # ⛔ LOOK ONLY INSIDE THE TABLE find_table BOUND, never across the whole file by row name: SCORE.md
        # carries more than one '| Language |' grid (ceo CEO-174's September-10 M/L/V grid), so every
        # language name appears more than once and a file-wide scan asserts on the wrong row -- the same
        # position-over-shape defect that made find_table itself refuse for the whole fleet.
        _hdr, _rows, _ = find_table(body.split("\n"))
        row = [_rows["rebus"][1]] if "rebus" in _rows else []
        cell = row[0][3] if row else ""
        prov = row[0][PROV_COL] if row else ""
        clauses = len(re.findall(r"(?:^|; )board:", prov))
        if len(row) != 1 or "1/48" in cell or cell.count("2/48") != 2 or clauses != 1:
            print("SELFTEST FAIL: the second write did not replace the first -- %d rebus rows in the bound grid, "
                  "%d board: clauses, first-write text %s" % (len(row), clauses, "SURVIVES" if "1/48" in cell else "gone")); ok = False
        else:
            print("SELFTEST: the second write replaced the first cell and its provenance clause, not appended beside it")
        _lines0 = open(SCORE_MD, encoding="utf-8").read().split("\n")
        _gh0, _gr0, _ = find_grid(_lines0)
        for _gl, _gline in enumerate(_lines0):
            if _gline.startswith("| rebus |"):
                _gc = [x.strip() for x in _gline.strip().strip("|").split("|")]
                if len(_gc) == GRID_NCOLS:
                    _gc[GRID_COLUMNS["L"][0]] = "—"; _lines0[_gl] = "| " + " | ".join(_gc) + " |"
        open(SCORE_MD, "w", encoding="utf-8").write("\n".join(_lines0))
        _lines0 = open(SCORE_MD, encoding="utf-8").read().split("\n")
        for run in (1, 2):
            a4 = A(); a4.lang = "rebus"; a4.column = "ladder"; a4.measurer = "selftest"; a4.modes = ""; a4.dry_run = False; a4.suite = ""
            a4.text = "rungs 0..5 PASS %d/22 (selftest, not a measurement)" % run
            try:
                cmd_write(a4)
            except SystemExit as e:
                print("SELFTEST FAIL: ladder grid write %d refused rc=%s on a seeded one-sentence cell" % (run, e.code)); ok = False
        _lines1 = open(SCORE_MD, encoding="utf-8").read().split("\n")
        _gh1, _gr1, _ = find_grid(_lines1)
        _lc = _gr1["rebus"][GRID_COLUMNS["L"][0]]
        if len(_lines1) != len(_lines0) or "1/22" in _lc or _lc.count("2/22") != 1 or _lc.count("⟨measured") != 1:
            print("SELFTEST FAIL: ladder grid write -- line count %d -> %d, cell %r" % (len(_lines0), len(_lines1), _lc[:120])); ok = False
        else:
            print("SELFTEST: ladder column writes the grid L cell in place; the second write replaced the first and its stamp")
        _lp = open(SCORE_MD, encoding="utf-8").read().split("\n")
        for _gl, _gline in enumerate(_lp):
            if _gline.startswith("| rebus |"):
                _gc = [x.strip() for x in _gline.strip().strip("|").split("|")]
                if len(_gc) == GRID_NCOLS:
                    _gc[GRID_COLUMNS["L"][0]] = "rungs 0..5 PASS 22/22. A person wrote this second sentence."; _lp[_gl] = "| " + " | ".join(_gc) + " |"
        open(SCORE_MD, "w", encoding="utf-8").write("\n".join(_lp))
        a6 = A(); a6.lang = "rebus"; a6.column = "ladder"; a6.measurer = "s"; a6.modes = ""; a6.dry_run = False; a6.suite = ""; a6.text = "rungs 0..5 PASS 22/22"
        try:
            cmd_write(a6); print("SELFTEST FAIL: ladder write over a two-sentence cell did not refuse"); ok = False
        except SystemExit as e:
            if e.code == 2: print("SELFTEST: ladder write over a hand-written two-sentence cell correctly REFUSED rc=2 and named it")
            else: print("SELFTEST FAIL: ladder prose guard exited %s, expected 2" % e.code); ok = False
        a5 = A(); a5.lang = "rebus"; a5.column = "ladder"; a5.measurer = "s"; a5.modes = ""; a5.dry_run = False; a5.suite = ""; a5.text = "rungs 0..5 all green"
        try:
            cmd_write(a5); print("SELFTEST FAIL: ladder write without a fraction did not refuse"); ok = False
        except SystemExit as e:
            if e.code == 2: print("SELFTEST: ladder write without an N/M fraction correctly REFUSED rc=2")
            else: print("SELFTEST FAIL: ladder no-fraction exited %s, expected 2" % e.code); ok = False
        for label, kw in (("unknown language", dict(lang="klingon", column="board", text="1/1", measurer="s")),
                          ("unknown column", dict(lang="rebus", column="nosuch", text="1/1", measurer="s")),
                          ("no digit", dict(lang="rebus", column="board", text="looks fine", measurer="s")),
                          ("pipe injection", dict(lang="rebus", column="board", text="1/1 | evil", measurer="s"))):
            a3 = A(); a3.modes = ""; a3.dry_run = False; a3.suite = ""
            for k, v in kw.items(): setattr(a3, k, v)
            try:
                cmd_write(a3)
                print("SELFTEST FAIL: %s did not refuse" % label); ok = False
            except SystemExit as e:
                if e.code == 2:
                    print("SELFTEST: %s correctly REFUSED rc=2" % label)
                else:
                    print("SELFTEST FAIL: %s exited %s, expected 2" % (label, e.code)); ok = False

        # ⛔⭐ SEMICOLON NORMALISATION, NOT REFUSAL (row score-row-write-refuses-a-semicolon-forever-so-a-
        # hand-edited-cell-can-never-be-runner-written): a ';' in --text must SUCCEED, by becoming '·', and
        # the round trip must stay safe -- proven by a SECOND write that goes through merge_clause (the
        # thing the guard actually protects) on the SAME cell and checking the first write's clause comes
        # back intact, not split at the '·' that used to be a ';'.
        _fh, _fr, _ = find_table(open(SCORE_MD, encoding="utf-8").read().split("\n"))
        _fri, _frc = _fr["rebus"]
        _frc[COLUMNS["floor"][0]] = "—"
        _flines = open(SCORE_MD, encoding="utf-8").read().split("\n")
        _flines[_fri] = "| " + " | ".join(_frc) + " |"
        open(SCORE_MD, "w", encoding="utf-8").write("\n".join(_flines))
        a4 = A(); a4.lang = "rebus"; a4.column = "floor"; a4.measurer = "selftest"
        a4.text = "1/1; semicolon inside"; a4.modes = ""; a4.dry_run = False; a4.suite = ""
        cmd_write(a4)
        _hdr, _rows, _ = find_table(open(SCORE_MD, encoding="utf-8").read().split("\n"))
        cell = _rows["rebus"][1][COLUMNS["floor"][0]] if "rebus" in _rows else ""
        if ";" in cell:
            print("SELFTEST FAIL: semicolon normalisation -- a raw ';' survived into the stored cell: %r" % cell); ok = False
        elif "1/1· semicolon inside" not in cell:
            print("SELFTEST FAIL: semicolon normalisation -- normalised text not found in cell: %r" % cell); ok = False
        else:
            print("SELFTEST: ';' in --text normalised to '·' and written, not refused")
            a5 = A(); a5.lang = "rebus"; a5.column = "floor"; a5.measurer = "selftest"
            a5.text = "2/2"; a5.modes = ""; a5.dry_run = False; a5.suite = "SomeSuite"
            cmd_write(a5)
            _hdr, _rows, _ = find_table(open(SCORE_MD, encoding="utf-8").read().split("\n"))
            cell2 = _rows["rebus"][1][COLUMNS["floor"][0]] if "rebus" in _rows else ""
            clauses2 = [c.strip() for c in cell2.split(";") if c.strip()]
            if len(clauses2) != 2 or clauses2[0] != "1/1· semicolon inside" or not clauses2[1].startswith("SomeSuite:"):
                print("SELFTEST FAIL: round trip -- a later --suite write fragmented or lost the earlier "
                      "normalised clause: %r (split into %r)" % (cell2, clauses2)); ok = False
            else:
                print("SELFTEST: round trip holds -- a later --suite write did not fragment the earlier normalised clause")

        # ⛔⭐ CELL PROSE LOSS -- fixture is the REAL incident, .github 46ff295c, not an invented example.
        # `real_before` is exactly what the snobol4 board cell carried before the runner clobbered it;
        # `reformatted_only` is the SAME sentence with the runner's own boilerplate drift the repair also
        # shows (no human involved, must NOT refuse); `sentence_dropped` is what the runner actually wrote
        # (the trailing sentence gone, must refuse). Both directions matter equally: a check that also
        # catches boilerplate drift is noisy enough that nobody keeps it turned on.
        real_before = ("m3 PASS=1689 FAIL=0 · m4 PASS=1689 FAIL=0 SKIP=0 · MISSING=0 "
                       "(`test_corpus_snobol4.sh` inside `make test`, rc=0). Re-confirmed on a newer tree "
                       "and identical across two consecutive runs (before and after a rebase onto origin), "
                       "as the control arm for the rung-11 LCO landing: the shared alternative-trampoline "
                       "change in emit.cpp moved nothing here.")
        reformatted_only = ("m3 PASS=1689 FAIL=0 · m4 PASS=1689 FAIL=0 SKIP=0 MISSING=0 "
                            "(`test_corpus_snobol4.sh`, inside `make test`, rc=0). Re-confirmed on a newer "
                            "tree and identical across two consecutive runs (before and after a rebase onto "
                            "origin), as the control arm for the rung-11 LCO landing: the shared "
                            "alternative-trampoline change in emit.cpp moved nothing here.")
        sentence_dropped = ("m3 PASS=1689 FAIL=0 · m4 PASS=1689 FAIL=0 SKIP=0 MISSING=0 "
                            "(`test_corpus_snobol4.sh`, inside `make test`, rc=0).")
        if cell_prose_loss(real_before, reformatted_only):
            print("SELFTEST FAIL: cell_prose_loss flagged the real 46ff295c boilerplate drift as lost prose"); ok = False
        else:
            print("SELFTEST: cell_prose_loss did not false-positive on punctuation-only reformatting")
        if not cell_prose_loss(real_before, sentence_dropped):
            print("SELFTEST FAIL: cell_prose_loss missed the real dropped sentence from .github 46ff295c"); ok = False
        else:
            print("SELFTEST: cell_prose_loss correctly caught the real .github 46ff295c dropped sentence")
        # Integration: seed the scratch board directly (bypassing cmd_write, so the seed itself cannot be
        # refused), then prove cmd_write FOLDS a real drop forward instead of losing it (rc=0, both texts
        # present), and still applies a pure reformat as a clean replace (nothing to fold). Each scenario
        # reseeds its own starting cell rather than sharing state, so a7's fold-forward result (which now
        # legitimately contains SUPERSEDE_MARKER) cannot leak into a8's "no marker" assertion.
        def _seed_floor(text):
            _lines = open(SCORE_MD, encoding="utf-8").read().split("\n")
            _h2, _r2, _ = find_table(_lines)
            _ri, _rc = _r2["rebus"]
            _fidx = COLUMNS["floor"][0]
            _rc[_fidx] = text
            _lines[_ri] = "| " + " | ".join(_rc) + " |"
            open(SCORE_MD, "w", encoding="utf-8").write("\n".join(_lines))
            return _fidx

        _fidx = _seed_floor(real_before)
        a7 = A(); a7.lang = "rebus"; a7.column = "floor"; a7.measurer = "selftest"; a7.modes = ""; a7.suite = ""
        a7.dry_run = False; a7.text = sentence_dropped
        try:
            cmd_write(a7)
            _h3, _r3, _ = find_table(open(SCORE_MD, encoding="utf-8").read().split("\n"))
            _after = _r3["rebus"][1][_fidx]
            if sentence_dropped not in _after or real_before not in _after:
                print("SELFTEST FAIL: cmd_write's supersede fallback dropped the new text or the old "
                      "reading instead of carrying both forward verbatim: %r" % _after); ok = False
            else:
                print("SELFTEST: cmd_write correctly folded the old reading forward (supersede fallback) "
                      "instead of silently dropping it or refusing -- the prior sentence survives in the cell")
        except SystemExit as e:
            print("SELFTEST FAIL: cmd_write refused a lossy overwrite instead of folding the old reading "
                  "forward (rc=%s)" % e.code); ok = False

        _seed_floor(real_before)
        a8 = A(); a8.lang = "rebus"; a8.column = "floor"; a8.measurer = "selftest"; a8.modes = ""; a8.suite = ""
        a8.dry_run = False; a8.text = reformatted_only
        try:
            cmd_write(a8)
            _h4, _r4, _ = find_table(open(SCORE_MD, encoding="utf-8").read().split("\n"))
            _after2 = _r4["rebus"][1][_fidx]
            if SUPERSEDE_MARKER in _after2:
                print("SELFTEST FAIL: cmd_write folded a PURE REFORMAT through the supersede fallback "
                      "instead of a clean replace -- cell_prose_loss false-positived on punctuation drift, "
                      "this time through the cmd_write integration path"); ok = False
            else:
                print("SELFTEST: cmd_write correctly proceeded on a pure reformat (nothing to lose, no "
                      "supersede clause added)")
        except SystemExit as e:
            print("SELFTEST FAIL: cmd_write refused a pure reformat that loses nothing (rc=%s)" % e.code); ok = False

        # ⛔⭐ FOLD DEPTH STAYS CAPPED ACROSS REPEATED FOLDS (hq_T ruling on
        # ruling-supersede-fold-approved-with-a-bound). The a7/a8 checks above only ever fold ONCE, which
        # cannot catch a bug that only shows up once `before` already carries a marker of its own -- three
        # folds in a row is the minimum depth that can. Three genuinely distinct readings (no shared prose,
        # so every write is a real loss and a real fold) prove the cell keeps exactly one archived reading
        # no matter how many folds land on it, instead of re-embedding its whole prior self every time.
        _seed_floor("—")
        _fold_a = "board reading Alpha: 17 dogs jumped over lake Baldwin at dawn."
        _fold_b = "measurement Bravo shows: 90 cars parked beside river Clifton at dusk."
        _fold_c = "observation Charlie found: 60 birds nested inside forest Denton at noon."
        for _reading in (_fold_a, _fold_b, _fold_c):
            a9 = A(); a9.lang = "rebus"; a9.column = "floor"; a9.measurer = "selftest"
            a9.modes = ""; a9.suite = ""; a9.dry_run = False; a9.text = _reading
            cmd_write(a9)
        _h5, _r5, _ = find_table(open(SCORE_MD, encoding="utf-8").read().split("\n"))
        _after3 = _r5["rebus"][1][_fidx]
        _markers = _after3.count(SUPERSEDE_MARKER)
        if _markers != 1:
            print("SELFTEST FAIL: three successive folds left %d SUPERSEDE_MARKER occurrence(s), wanted "
                  "exactly 1 -- fold depth is not bounded, the cell grows by its whole history on every "
                  "landing" % _markers); ok = False
        elif "lake Baldwin" in _after3:
            print("SELFTEST FAIL: the oldest, twice-superseded reading survived a bounded fold -- dropping "
                  "it is the whole point of the bound"); ok = False
        elif "river Clifton" not in _after3 or "forest Denton" not in _after3:
            print("SELFTEST FAIL: the live reading or its one kept archive did not survive the bounded "
                  "fold: %r" % _after3); ok = False
        else:
            print("SELFTEST: three successive folds keep exactly one archived reading -- the cell stops "
                  "growing with fold depth instead of accumulating its whole history")

        # ⭐ THE MEASURER CONTRACT HAS TWO ARMS AND BOTH ARE TESTED. An absent or placeholder measurer on a
        # KNOWN root is DERIVED (the identity is a fact on disk, not a guess); on an UNKNOWN root there is
        # nothing to derive from and it still REFUSES. The old selftest asserted only the refusal, so it went
        # red the moment derivation landed -- correctly: it was pinning the behaviour that was being replaced.
        global S4E
        _real_s4e = S4E
        _saved_seat = os.environ.pop("S4E_SEAT", None)
        # ⛔ RESET rebus/board TO EMPTY FIRST. By this point the rewrite-in-place proof above left it
        # holding "master: m3 2/48 ..." -- real prior content that a3.text="1/1" would (correctly, per
        # cell_prose_loss) refuse to clobber, which is not what THIS block is testing. "—" is the file's
        # own empty-cell sentinel, so cell_prose_loss treats it as nothing-to-lose by construction.
        _reset_lines = open(SCORE_MD, encoding="utf-8").read().split("\n")
        _rh, _rr, _ = find_table(_reset_lines)
        _rri, _rrc = _rr["rebus"]
        _rrc[COLUMNS["board"][0]] = "—"
        _reset_lines[_rri] = "| " + " | ".join(_rrc) + " |"
        open(SCORE_MD, "w", encoding="utf-8").write("\n".join(_reset_lines))
        try:
            for label, root, want in (("absent measurer, known root", "/home/claude_T", "hq_T"),
                                      ("placeholder measurer, known root", "/home/claude_T", "hq_T"),
                                      ("absent measurer, numbered seat root", "/home/claude7", "seat07")):
                S4E = root
                a4 = A(); a4.modes = ""; a4.dry_run = False; a4.suite = ""
                a4.lang = "rebus"; a4.column = "board"; a4.text = "1/1"
                a4.measurer = "unknown-seat" if "placeholder" in label else ""
                try:
                    cmd_write(a4)
                    if a4.measurer == want:
                        print("SELFTEST: %s correctly DERIVED %r" % (label, want))
                    else:
                        print("SELFTEST FAIL: %s derived %r, wanted %r" % (label, a4.measurer, want)); ok = False
                except SystemExit as e:
                    print("SELFTEST FAIL: %s refused (rc=%s) instead of deriving %r" % (label, e.code, want)); ok = False
            S4E = "/tmp/not-a-seat-root"
            a5 = A(); a5.modes = ""; a5.dry_run = False; a5.suite = ""
            a5.lang = "rebus"; a5.column = "board"; a5.text = "1/1"; a5.measurer = "unknown-seat"
            try:
                cmd_write(a5)
                print("SELFTEST FAIL: placeholder measurer on an UNKNOWN root did not refuse"); ok = False
            except SystemExit as e:
                if e.code == 2:
                    print("SELFTEST: placeholder measurer on an UNKNOWN root correctly REFUSED rc=2")
                else:
                    print("SELFTEST FAIL: unknown-root refusal exited %s, expected 2" % e.code); ok = False
        finally:
            S4E = _real_s4e
            if _saved_seat is not None: os.environ["S4E_SEAT"] = _saved_seat
        # ⛔⭐ A FRACTION LABELLED BY A TRAILING FAILURE WORD IS NOT A PASS FRACTION -- and the two arms that
        # matter pull in OPPOSITE directions, which is why both are pinned. Under-drop and `924/986
        # PARSE-FAIL` convicts raku of a conflict with the grid's correct `4/986`; over-drop and icon's real
        # `153/153, FAIL=0` vanishes from the board. Nothing separates them but the comma and the `=`.
        for label, txt, want in (
                ("trailing PARSE-FAIL is not a pass fraction", "still 924/986 PARSE-FAIL, byte-identical", {}),
                ("a pass fraction survives beside a fail fraction of the SAME denominator",
                 "roast 4/986 pass · 924/986 PARSE-FAIL", {986: 4}),
                ("`FAIL=0` right after a fraction is a labelled count, NOT that fraction's name",
                 "m3 43/89 FAIL=0", {89: 43}),
                ("a comma before FAIL means it labels the clause, not the number",
                 "run-graded 563/569 both modes + ast 153/153, FAIL=0", {569: 563, 153: 153}),
                ("a bare trailing `fail` still drops", "jcon 38/81 fail", {}),
                # ⛔⭐ A SUPERSEDED READING CITED FOR PROVENANCE MUST NOT BECOME THE CELL'S VALUE, and these two
                # arms exist because the first cure of that bug BROKE THE LIVE NUMBER instead (hq_C s295, hq_T
                # 2026-09-04). Populations group by denominator keeping MIN(pass) -- the both-modes bar -- so a
                # cited older figure silently REPLACED the current one for the agree gate and the percentage
                # alike. The reported symptom was a red gate; the real defect was a WRONG VALUE that looked
                # conservative. ⛔ And the fix must not drop the DENOMINATOR: a superseded citation names THE
                # SAME population at an earlier time, so dropping it wholesale deletes the live measurement too
                # (measured: icon read {153: 153}, losing 596 entirely). One fraction leaves; the population stays.
                ("a superseded reading cited for provenance is not the cell's value",
                 "run-graded **596/596 both modes** + ast 153/153, FAIL=0 \u2014 hq_C read **595/596** on the tree before that cure",
                 {596: 596, 153: 153}),
                ("...and the older `was N/M` idiom no longer deletes the live population with it",
                 "**409/409** run-graded \u00b7 was 377/381", {409: 409})):
            got, _w = cell_fractions(txt)
            if got == want:
                print("SELFTEST: %s" % label)
            else:
                print("SELFTEST FAIL: %s -- read %s, wanted %s (from %r)" % (label, got, want, txt)); ok = False
        # ⛔⭐ THE STALE MARK READS THE PROVENANCE LABEL, NOT PROSE. Arm 1 is the exact live defect Lon
        # named: an M cell measured TODAY that still printed `?` because its own commentary cites an older
        # date. Arm 4 is the opposite guard -- keying on the label must not make the cell's own explicit
        # word unsayable, or a human could never mark a cell stale by hand.
        _t = datetime.date.today()
        _today, _yday = _t.strftime("%Y-%m-%d"), (_t - datetime.timedelta(days=1)).strftime("%Y-%m-%d")
        _grid = [""] * (max(i for i, _n in GRID_COLUMNS.values()) + 1)
        def _prog(mcell, prov):
            # the scratch text goes in the V cell: since Lon 2026-09-04 the percent and its freshness are read
            # from V (the industry-standard packages), and M is printed beside as ours, never counted
            g = list(_grid); g[GRID_COLUMNS["V"][0]] = mcell; g[GRID_COLUMNS["M"][0]] = ""
            return language_progress("snobol4", g, prov)
        _st = lambda day: GRID_STAMP % (day, "hq_T")
        for label, mcell, prov, want in (
                ("an old date in PROSE does not stale a cell stamped TODAY",
                 "m3 10/10 (seat08, 2026-09-03, FINDING-...-09-03-...md) " + _st(_today), "", ""),
                ("a stamp older than today DOES stale it", "m3 10/10 " + _st(_yday), "", "?"),
                ("NO stamp reads STALE -- an undatable number is not a fresh one", "m3 10/10", "", "?"),
                ("the cell's own word STALE still wins over a fresh stamp",
                 "m3 10/10 STALE " + _st(_today), "", "?"),
                ("the stamp itself is never read as a population",
                 "m3 10/10 " + _st(_today), "", "")):
            _pct, mark, _P, _T, _w = _prog(mcell, prov)
            if mark == want:
                print("SELFTEST: %s" % label)
            else:
                print("SELFTEST FAIL: %s -- mark %r, wanted %r (%s)" % (label, mark, want, "; ".join(_w))); ok = False
        # ⛔⭐ THE GRID M COLUMN IS PARSED FOR A FRACTION AND THE DISPLAY COLUMN IS NOT -- a verbatim copy
        # between them published snobol4 at 55%% instead of 91%% (hq_P, as the dual-write's first customer).
        # Both directions pinned: the PASS=N board line -- which is the NATURAL way to print a board -- must
        # never reach the grid, and the fraction form must still land, or the guard would make the whole
        # dual-write inert while looking careful.
        for label, txt, want_frac in (
                ("a PASS=N board line carries no fraction the grid column can parse",
                 "m3 PASS=1698 FAIL=0 · m4 PASS=1698 FAIL=0 SKIP=0 MISSING=0", False),
                ("a board line WITH a fraction is grid-writable", "m3 1698/1736 FAIL=0 · m4 1698/1736", True)):
            got = bool(cell_fractions(txt)[0])
            if got == want_frac:
                print("SELFTEST: %s" % label)
            else:
                print("SELFTEST FAIL: %s -- fraction=%s, wanted %s" % (label, got, want_frac)); ok = False
    finally:
        SCORE_MD = real
        shutil.rmtree(d, ignore_errors=True)
    print("SELFTEST %s" % ("PASS" if ok else "FAIL"))
    return 0 if ok else 1


# ============================ THE PROGRESS LINE (Lon 2026-09-03 ~20:15) ============================
# ⛔⭐ THIS READS THE SEPTEMBER-10 GRID, WHICH IS THE **OTHER** TABLE IN SCORE.md. find_table() above binds
# the standardized display (6 columns); this binds the M/L/V/B grid (7 columns) that sits ABOVE it. Both
# headers read `| Language |`, which is exactly the collision that made find_table refuse fleet-wide, so
# this one proves its shape by COLUMN NAME too and refuses naming every candidate. Shape is the identity.
GRID_COLUMNS = {"M": (1, "M ·"), "L": (2, "L ·"), "V": (3, "V ·"), "B": (4, "B ·")}
GRID_NCOLS = 7
# Short names and print order, fixed by the brief.
PROGRESS_LANGS = [("snobol4", "sno"), ("snocone", "sc"), ("icon", "icn"), ("prolog", "pl"),
                  ("raku", "raku"), ("pascal", "pas"), ("rebus", "reb")]
# ⭐ A fraction whose LEFT CONTEXT carries one of these is not this cell's score: a floor/smoke gate, a
# secondary suite quoted for context, or a number the cell itself says it SUPERSEDES. Measured against
# every live cell, not guessed -- `smoke 724/724` (raku M), `smoke 4/4` (rebus M), `corpus suite 10/10`
# (snocone M), `the earlier 144/149` (pascal M). Kept as data so a reader can check the rule against the
# open file, which is the brief's "no hidden weights".
# ⛔ PROGRESS_DROP NAMES A DIFFERENT POPULATION (a smoke set, a corpus suite, the strict rung suite) -- its
# denominator leaves the cell entirely. Supersession words that used to live here ("was ", "the earlier",
# "readings", "supersedes") have MOVED to PROGRESS_SUPERSEDED below, because they name THE SAME population at
# an earlier time and dropping their denominator deleted the live number with them: `**596/596** ... was
# 595/596 before that cure` read as {} for 596, exactly the defect hq_C reported in its other wording.
PROGRESS_DROP = ("smoke", "corpus suite", "strict rung suite")
# ⛔⭐ A FRACTION LABELLED BY WHAT FOLLOWS IT COUNTS FAILURES, NOT PASSES. `924/986 PARSE-FAIL` is the raku
# roast task row's own idiom, and every reader of this parser called it a pass fraction: the agree gate
# convicted raku of a same-denominator conflict against the grid's `4/986` PASS, and both numbers were
# right all along (pass 4 + fail 9 + parse_fail 924 + no_tap 7 + crash 1 = 945, +41 missing = 986). ONE of
# the two tables is wrong is this gate's whole contract, so a false positive there is worse than a miss --
# it sends a reader to reconcile two correct cells. PROGRESS_DROP only ever looked at the 20 chars BEFORE a
# fraction; a label can sit on either side of the number it names.
# ⛔ THE `=` GUARD IS THE WHOLE DIFFICULTY, not an edge case: `ast 153/153, FAIL=0` is a PASS fraction
# followed by the word FAIL, and a naive suffix match drops icon's real number. Two things separate them --
# the marker must follow with NOTHING but whitespace between (a comma means it labels the clause, not the
# fraction), and `FAIL=`/`fail=` is a labelled count that is already read, never this fraction's name.
PROGRESS_FAIL_SUFFIX = ("parse-fail", "fail", "reject", "crash", "no-tap", "xfail", "red", "missing")
# ⛔⭐ A SUPERSEDED READING CITED FOR PROVENANCE IS NOT THIS CELL'S VALUE -- AND IT USED TO WIN (hq_C, s295,
# measured again by hq_T 2026-09-04 on the exact cell). Icon's grid cell read `**596/596 both modes** + ast
# 153/153 ... hq_C read **595/596** on the tree before that cure`, and cell_fractions returned {596: 595}: the
# fraction-grouping keeps MIN(pass) across a denominator, on purpose (the both-modes bar), so the SUPERSEDED
# number silently replaced the current one for every consumer -- the agree gate convicted, and the percentage
# would have published the old figure. ⛔ THE REPORTED SYMPTOM WAS A RED GATE; THE REAL DEFECT WAS A WRONG
# VALUE, which is the more dangerous half and was invisible because it looked like a conservative reading.
# ⭐ hq_C reported that UN-BOLDING the prose fraction fixed it. It did not -- reproduced both ways here, bold
# is not read by this parser at all. What actually changed was the wording: `was 595/596` carries the existing
# "was " marker, `hq_C read ... before that cure` carried none. A cure aimed at the reported mechanism would
# have documented "never bold a superseded fraction" and left the defect exactly where it was.
# ⛔ MARKERS ARE TESTED OVER THE WHOLE CLAUSE, ON BOTH SIDES OF THE NUMBER, not a 20-char lookbehind: this
# board's provenance idiom puts the tell AFTER the fraction ("... on the tree before that cure"), and a clause
# is what a reader sees as one statement. Clause bounds are the cell's own separators -- · — ; and the em dash
# the board already uses to fence provenance off from value -- so a supersession note cannot reach back into
# the measurement it is annotating.
PROGRESS_SUPERSEDED = ("before that", "superseded", "supersedes", "the earlier", "earlier reading", "previously",
                       "prior reading", "stale", "no longer", "used to read", "at the time", "was ", "readings")
# Suites with NO runner or NO number: 0 pass over their file count, so unmeasured coverage reads as
# MISSING rather than as ABSENT. Denominators are the brief's own. Applied only when the cell BOTH names
# the suite and carries a no-number marker, so a suite that later gets a real number stops being estimated.
# ⛔ ipl IS COMPILE-GRADED AND IS DELIBERATELY ABSENT: it compiles 437 of 851 and RUN-grades zero, because
# upstream ships no `.std` oracle output. A suite that never checks an answer cannot sit in a correctness
# denominator -- it is named in the cell and counted nowhere (ceo ruling, Lon's industry-standard basis).
PROGRESS_ESTIMATED = {}   # superseded by counted_fractions: every listed package counts, ungraded ones as zero
# ⛔⭐⭐ ONLY A SHIPPED TEST SUITE COUNTS (Lon 2026-09-04, in-chat to ceo: "Ensure items in that list actually have test
# suites."): the V cell may name every vendored package, but the percent reads ONLY the clauses naming a package that
# ships its own tests or expected results -- csnobol4_suite (Budne's suite, 125 .ref verbatim), snoflake (180 @expect),
# arizona (Icon's tests/general, .std), jcon_tests (.std), swi_tests (SWI's src/Tests, plunit), INRIA (ISO 13211-1),
# fpc_tests (FPC's test programs), PAT (ISO 7185 validation suite, .cmp/.ecp), roast (Raku spec tests, TAP).
# NAMED, NEVER COUNTED: gimpel (a book's modules behind 144 drivers WE wrote), aisnobol and dotnet (program collections),
# ipl (a program library, no tests), gnu_prolog (GNU Prolog's compiler/library source, no tests).
PROGRESS_NO_PUBLIC_SUITE = ("snocone", "rebus")   # no shipped package at all: no percentage, never 100% from our own ladder
# ⛔⭐⭐ EVERY SHIPPED PACKAGE IS A TEST SUITE (Lon 2026-09-04, in-chat to ceo, verbatim: "If GNU Prolog ships with Prolog
# source; that is the test suite I am talking about. You make the programs run, you measure the output, make a REF
# file, and place it into a ONE-LINER or a MULTI-LINER Python test harness."). So every vendored package counts over
# its SHIPPED POPULATION: a package not yet run-graded counts as 0 of its population (it is on the list, it pulls the
# percent down until its programs run against the oracle), never as absent. The percent is the V column only; our own
# master, AST fixtures and ladders are printed as ours and never counted.
PROGRESS_COUNTED = {
    "snobol4": [("csnobol4", r"csnobol4", (118, 124, 125)), ("snoflake", r"[Ss]noflake", (180,)), ("gimpel", r"gimpel", (126, 144, 289)), ("aisnobol", r"aisnobol", (2, 8)), ("dotnet", r"dotnet", (5, 14))],
    "icon": [("arizona", r"[Aa]rizona", (89, 124)), ("jcon", r"[Jj][Cc][Oo][Nn]", (81, 91)), ("ipl", r"\bipl\b", (851,))],
    "prolog": [("swi", r"[Ss][Ww][Ii]", (114, 249)), ("INRIA", r"INRIA|inria|ISO 13211", (445,)), ("gnu", r"[Gg][Nn][Uu]", (62, 91))],
    "pascal": [("fpc", r"fpc", (181,)), ("PAT", r"\bPAT\b|validation suite|ISO 7185", (427, 429))],
    "raku": [("roast", r"roast", (986, 1464))],
}
def counted_fractions(lang, vcell):
    """{denominator: pass} over EVERY listed package: the first RUN-graded fraction over one of the package's own
    known populations within 160 chars after its name (denominator-anchored, so prose like "the grid's 126/124" can
    never be read as a suite); a package with no such fraction counts as 0 of its shipped population. Returns
    (got, work)."""
    # ⛔⭐ THE WINDOW IS THE PACKAGE'S OWN CLAUSE, NOT 160 CHARACTERS (hq_B 18:59, third instance of one shape
    # today; ceo row score-readers-anchor-on-a-labelled-form-and-say-why-they-found-nothing). A V cell is written
    # in `·`-separated clauses, and a writer is ENTITLED to lead with explanation -- the leaderboard doctrine asks
    # for provenance in the cell. A fixed 160-char window turns that entitlement into a wrong number: the snoflake
    # cell said 103/180 and the grid published `0/180 NOT YET RUN-GRADED`. Measured on the live board while
    # writing this: ALL FIVE snobol4 packages read 0 and "not yet run-graded", every one of which HAS been run.
    # ⛔⛔ AND "I FOUND NO FRACTION" IS NOT "IT SCORED ZERO". Those were one output, and that is the same
    # could-not-measure-vs-measured-red collapse this project refuses everywhere else (lib_gate.sh's three exit
    # codes; the harness's rc=2). They are now three distinct outcomes, and only the first contributes a number:
    #   FOUND            a fraction over one of the package's KNOWN populations -> counted
    #   UNREADABLE       digits are there but not in a countable form (a denominator nobody declared, or
    #                    `PASS=82 FAIL=91` rather than a fraction) -> NOT counted, NOT called zero, and the cell's
    #                    own text is quoted back so the writer can see what the reader saw
    #   ABSENT           the package's clause carries no digits at all -> genuinely not run-graded, counts as 0
    # ⭐ A reader that says WHY it found nothing turns a silent wrong number into a fixable message.
    got, work, unread = {}, [], []
    for name, rx, dens in PROGRESS_COUNTED.get(lang, []):
        best, saw = None, ""
        for m in re.finditer(rx, vcell):
            # clause = from this package's name to the next clause separator or the next package name, whichever
            # comes first -- so a long, honest explanation stays INSIDE the clause it belongs to, and a neighbour's
            # number can never be borrowed across a `·`.
            rest = vcell[m.end():]
            ends = [x for x in (rest.find(" · "), rest.find(" — "), rest.find("; ")) if x != -1]
            seg = rest[:min(ends)] if ends else rest
            if re.search(r"compile[- ]graded|COMPILE-graded|compile-only", seg[:60]):
                continue
            for f in re.finditer(r"(\d+)\s*/\s*(\d+)", seg):
                pnum, den = int(f.group(1)), int(f.group(2))
                if den in dens and pnum <= den:
                    best = (pnum, den)
                    break
                if not saw:
                    saw = "%d/%d" % (pnum, den)
            if best:
                break
            if not saw:
                mp = re.search(r"PASS\s*=\s*(\d+)", seg, re.I)
                if mp:
                    saw = "PASS=%s (a count, not a fraction)" % mp.group(1)
        if best is None:
            if saw:
                unread.append(name)
                # ⛔ AN UNREADABLE PACKAGE STAYS IN THE DENOMINATOR AND CONTRIBUTES ZERO PASSES. Dropping it from
                # BOTH sides was this cure's own first draft, and it was backwards: it RAISED every affected
                # language's percent (icon 8% -> 48% in one edit) purely because a cell had become unreadable,
                # while the work line called the result a "FLOOR". A floor is the CONSERVATIVE reading, and
                # excluding the unknown is the optimistic one. ⭐ The rule that keeps the two honest: an
                # unreadable cell must never be able to improve a score. It is a zero with a message, not an
                # abstention -- the message is for the writer, the zero is for the reader.
                got[dens[-1]] = got.get(dens[-1], 0)
                work.append("V %s UNREADABLE, counted 0/%d -- the cell carries %s, which is not a fraction over any "
                            "declared population %s. Counted ZERO (an unreadable cell must never raise a score), and "
                            "named here so it can be fixed: write it as `<pass>/<total>` over a declared population, "
                            "or declare the population." % (name, dens[-1], saw, sorted(dens)))
            else:
                got[dens[-1]] = got.get(dens[-1], 0)
                work.append("V %s 0/%d -- on the list, NOT YET RUN-GRADED (no number in its clause at all; counts "
                            "as zero until its programs run against the oracle)" % (name, dens[-1]))
            continue
        got[best[1]] = best[0]
        work.append("V %s %d/%d" % (name, best[0], best[1]))
    if unread:
        work.append("⚠ %d package(s) UNREADABLE (%s) -- each counted ZERO over its declared population, so this "
                    "language's percent is a genuine FLOOR: fixing the cell can only raise it." % (len(unread), ", ".join(unread)))
    return got, work


def find_grid(lines):
    # Same candidate-and-prove discipline as find_table, against the September-10 grid's own column names.
    candidates = [i for i, l in enumerate(lines) if l.startswith("| Language |")]
    if not candidates:
        die("no '| Language |' header in %s -- the September-10 grid is gone or renamed" % SCORE_MD)
    why = []
    for i in candidates:
        cells = [c.strip() for c in lines[i].strip().strip("|").split("|")]
        if len(cells) != GRID_NCOLS:
            why.append("line %d has %d columns, the grid has %d" % (i + 1, len(cells), GRID_NCOLS))
            continue
        bad = [k for k, (idx, pre) in GRID_COLUMNS.items() if not cells[idx].startswith(pre)]
        if bad:
            why.append("line %d column(s) %s do not start with their %s prefix" % (i + 1, ",".join(sorted(bad)), "M·/L·/V·/B·"))
            continue
        rows = {}
        skipped = {}
        for j in range(i + 2, len(lines)):
            if not lines[j].startswith("|"):
                break
            c = [x.strip() for x in lines[j].strip().strip("|").split("|")]
            if len(c) == GRID_NCOLS:
                rows[c[0]] = c
            elif c and c[0]:
                # ⛔⭐⭐ THE TWIN OF THE SAME DEFECT, AND IT SURVIVED THE CURE OF ITS SIBLING BY ONE DAY.
                # find_table's row loop got its `else` on 2026-09-04 (cc054250f) after one stray `|` deleted
                # snobol4 from every reader of the DISPLAY table. This loop -- same file, same shape, same
                # consequence -- was left with no `else`, so a malformed row in the SEPTEMBER-10 GRID still
                # vanished in silence. Measured (hq_T 2026-09-05, reproducing hq_B's incident): pasting a
                # minimized Icon witness carrying `||` into the icon grid row widened it 7 cells to 9, the row
                # dropped out of `rows`, and `agree` went from GATE RED on a real same-denominator conflict to
                # GATE PASS(0) over 10 pairs holding none of icon's. Nothing was fixed; the gate stopped
                # looking, and it said PASS in the same breath.
                # ⭐ THE RULE THIS PAYS FOR (hq_B, 2026-09-05, into GOAL-TEST-SUITE-CONSISTENCY.md): A GREEN
                # THAT APPEARS WHILE YOU ARE EDITING THE DATA IS A SUSPECT, NOT A REWARD. And the fix-the-twin
                # lesson beside it: a defect cured in one reader of a file is owed to every reader of that
                # file in the same sitting -- the cure that stops at the function it was reported on leaves
                # the identical hole one screen away, wearing a different table's name.
                skipped[c[0]] = (j, len(c))
                sys.stderr.write("⚠ SCORE.md line %d: September-10 grid row %r has %d columns, the grid has %d -- "
                                 "SKIPPED, so every reading below OMITS it. It is malformed, not absent.\n"
                                 % (j + 1, c[0], len(c), GRID_NCOLS))
        return i, rows, skipped
    die("no '| Language |' table in %s is the September-10 grid. Candidates: %s" % (SCORE_MD, "; ".join(why)))


def cell_fractions(raw):
    # Return ({denominator: pass}, workings), or (None, workings) when the cell carries a population this
    # parser cannot read -- see the fail-closed contract in language_progress.
    # ⛔⭐ THE HARNESS SHAPE IS READ FIRST, AND BEFORE PARENTHESES ARE STRIPPED. Measured (ceo, on seat12's
    # snocone cell): `ast 67/67 · run(206) m3 176 pass/7 fail` printed `sc 100%`, because the run population's
    # TOTAL lives inside `run(206)` that the provenance strip deleted and its PASS is prose, not a fraction.
    s = re.sub(r"`[^`]*`", "", raw)
    groups, dropped, work, logged, consumed = {}, set(), [], set(), []
    m = re.search(r"\brun\((\d+)\)", s)
    if m:
        total = int(m.group(1))
        passes = [int(x) for x in re.findall(r"\bm[34]\s+(\d+)\s+pass\b", s)]
        if not passes:
            work.append("run(%d) present but no 'mN <n> pass' to pair with it -- UNREADABLE" % total)
            return None, work
        groups.setdefault(total, []).append(min(passes))
        logged.add(total)
        work.append("run %d/%d (from %s, worse mode)" % (min(passes), total, ",".join(str(x) for x in passes)))
        s = s[:m.start()] + " " + s[m.end():]
        s = re.sub(r"\bm[34]\s+\d+\s+pass(?:\s*/\s*\d+\s+\w+)*", " ", s)
    s = re.sub(r"\([^()]*\)", "", s)
    # ⭐ GROUP BY DENOMINATOR, KEEP MIN(PASS): collapses an m3/m4 twin into ONE population (the grid's own
    # "PER MODE, never summed") and, where the modes disagree, keeps the WORSE -- the both-modes bar.
    # ⛔ A drop marker drops the whole DENOMINATOR, not the one fraction (measured on snocone, whose
    # `corpus suite 10/10 · 10/10` carries the marker before the FIRST twin only).
    for m in re.finditer(r"(?<![\d/])(\d+)\s*/\s*(\d+)(?![\d/])", s):
        t, ps = int(m.group(2)), int(m.group(1))
        ctx = s[max(0, m.start() - 20):m.start()].lower()
        _cl_start = max([s.rfind(ch, 0, m.start()) for ch in ("\u00b7", "\u2014", ";")] + [-1]) + 1
        _cl_end = min([x for x in (s.find(ch, m.end()) for ch in ("\u00b7", "\u2014", ";")) if x != -1] + [len(s)])
        clause = s[_cl_start:_cl_end].lower()
        # ⛔⭐ TWO DROPS THAT MUST NOT BE THE SAME DROP. A PROGRESS_DROP marker ("smoke", "corpus suite") names a
        # DIFFERENT population, so the whole denominator leaves the cell -- correct, and measured on snocone.
        # A PROGRESS_SUPERSEDED marker names THE SAME population at an earlier time, so dropping the
        # denominator would delete the LIVE number with it: the first cure here did exactly that and read
        # icon's `**596/596** ... hq_C read **595/596** before that cure` as {153: 153}, losing 596 entirely.
        # A superseded citation drops ONE FRACTION, like a trailing failure label.
        sup = [d for d in PROGRESS_SUPERSEDED if d in clause]
        hit = [d for d in PROGRESS_DROP if d in ctx]
        if hit:
            dropped.add(t)
            work.append("drop %d/%d (%r)" % (ps, t, hit[0]))
            consumed.append((m.start(), m.end()))
            continue
        if sup:
            work.append("drop %d/%d (superseded reading cited for provenance: %r -- the same population at an earlier time, never this cell's value)" % (ps, t, sup[0]))
            consumed.append((m.start(), m.end()))
            continue
        # ⭐ Drops THIS FRACTION ONLY, never the denominator -- unlike a leading PROGRESS_DROP marker, a
        # trailing label names exactly one number, and the same population may carry a real pass fraction
        # elsewhere in the cell that must survive. If nothing else names the denominator it simply leaves
        # this table, and the gate reports it as the one-sided population it honestly is.
        suf = re.match(r"\s+(%s)\b(?!\s*=)" % "|".join(PROGRESS_FAIL_SUFFIX), s[m.end():], re.I)
        if suf:
            work.append("drop %d/%d (labelled %r -- a FAILURE count, not a pass fraction)" % (ps, t, suf.group(1)))
            consumed.append((m.start(), m.end()))
            continue
        groups.setdefault(t, []).append(ps)
        consumed.append((m.start(), m.end()))
    # ⛔ BLANK EVERY CONSUMED FRACTION OUT OF THE PROBE TEXT. Without this the leftover check re-reads the
    # TAIL of a fraction it already counted -- snobol4's "csnobol4_suite 52/118 PASS" leaves "118 PASS"
    # behind, which looks exactly like an unconsumed population and made the whole language read MISSING
    # while its number had in fact been read correctly. A fail-closed guard that fires on its own successful
    # work is worse than no guard: it is unfalsifiable noise, and the first response to it is to switch it off.
    _b = list(s)
    for _lo, _hi in consumed:
        for _i in range(_lo, _hi):
            _b[_i] = " "
    s = "".join(_b)
    # ⛔⭐ FAIL CLOSED -- but on the text MINUS every dropped clause. Two measured lessons compose here.
    # (1) An unconsumed population marker means the cell is UNREADABLE and must not be scored: not 100%, not
    #     0%, not silently dropped. (2) A cell often quotes a SECONDARY suite for context -- icon's M carries
    #     "STRICT rung suite 266 PASS · 4 FAIL · ... of 298", which is ladder work that must not count toward
    #     M. Checking leftovers on the RAW text made icon (and with it ALL) read MISSING because of a clause
    #     that was never supposed to be counted. So the clause is excised FOR THE CHECK ONLY -- excising it
    #     for the COUNT as well silently re-admitted snocone's floor and prolog's superseded 271/270, because
    #     removing the marker text also removed the evidence that those fractions were droppable.
    probe = s
    for d in PROGRESS_DROP:
        while True:
            mm = re.search(re.escape(d), probe, re.I)
            if not mm:
                break
            lo = max((probe.rfind(c, 0, mm.start()) for c in ("·", ";", "—")), default=-1)
            hi = min([x for x in (probe.find(c, mm.end()) for c in ("·", ";", "—")) if x != -1] or [len(probe)])
            probe = probe[:lo + 1] + " " + probe[hi:]
    # ⛔ key=value PAIRS ARE NOT A LOOSE POPULATION. `in_tier=986 pass=4` reads as "986 pass" to a naive
    # scan, so a cell written in the harness's OWN board idiom -- the most machine-readable form there is --
    # tripped the guard. The lookbehind rejects a digit run that belongs to `key=986`, and `pass=` is
    # excluded outright: an `=` means the number is already labelled and already read.
    leftover = re.findall(r"(?<![=\w])\d+\s+pass\b(?!\s*=)|\bpass\s+\d+|\brun\(|\bdenominator\s+\d+", probe, re.I)
    if leftover:
        work.append("UNREADABLE: %d unconsumed population marker(s): %s" % (len(leftover), ", ".join(sorted(set(leftover)))))
        return None, work
    out = {}
    for t, ps in groups.items():
        if t in dropped:
            work.append("drop %s/%d (denominator dropped)" % ("/".join(str(x) for x in ps), t))
            continue
        out[t] = min(ps)
        if t in logged:
            continue
        if len(ps) > 1:
            work.append("%d/%d (from %s, worse mode)" % (out[t], t, ",".join(str(x) for x in ps)))
        else:
            work.append("%d/%d" % (out[t], t))
    return out, work


def ladder_score(lang):
    # (built, declared) from corpus/tests/<lang>/config/LADDER.tsv, or None when there is no census.
    path = os.path.join(S4E, "corpus", "tests", lang, "config", "LADDER.tsv")
    if not os.path.isfile(path):
        return None
    built = declared = 0
    for line in open(path, encoding="utf-8", errors="replace"):
        if not re.match(r"^rung\d+", line):
            continue
        declared += 1
        if re.search(r"\bBUILT\b", line):
            built += 1
    return (built, declared) if declared else None


def forms_score(lang):
    # (witnessed, declared) straight from util_ladder_forms_check -- ONE definition of L for the whole org, so
    # the ruling that an oracle-observed census cannot be credited applies here without being restated.
    try:
        sp = importlib.util.spec_from_file_location("_lfc", os.path.join(os.path.dirname(os.path.abspath(__file__)),
                                                                        "util_ladder_forms_check.py"))
        m = importlib.util.module_from_spec(sp)
        sp.loader.exec_module(m)
        m.CORPUS = os.path.join(S4E, "corpus")
        st, d, w, miss = m.check_lang(lang, "isolation", verbose=False)
        # ⛔⭐ A PARTIALLY-ENUMERATED STANDARD HAS NO SCORE, exactly as an unwritten one has none. Measured
        # here the moment L entered the percent: rebus declares FORMS for 2 rungs of 12, both fully
        # witnessed, so L read 13/13 and the language printed **100%** on Lon's own line. The ten rungs
        # nobody has enumerated are simply not in the denominator, so the less of the book you have read the
        # better you score -- the vacuity hole, arriving in the headline number rather than in the checker.
        # So an incomplete census returns None and the language reads MISSING with its reason, which is the
        # file's existing doctrine for a census that does not exist at all: 0% would be a claim we have not
        # earned either, and 100% is very much one.
        if st in ("NOT-IN-SCHEMA", "REFUSED"):
            return None
        if any("FORMS cell is empty" in x for x in miss):
            return "incomplete"
        return (w, d) if d else None
    except Exception as e:
        # ⛔ NEVER SWALLOW THIS. A bare `return None` reads downstream as "this language has no census" --
        # a real and expected state -- so an import bug wore the costume of a legitimate answer and printed
        # `L NO census` for all seven at once while looking entirely plausible. Measured: it did exactly that.
        sys.stderr.write("WARN util_score_row.forms_score(%s): %s: %s\n" % (lang, type(e).__name__, e))
        return None


def language_progress(lang, cells, prov=""):
    # ⛔⭐⭐ THE PERCENT IS THE INDUSTRY STANDARD, NEVER OUR OWN TESTS (Lon 2026-09-04, in-chat to ceo, verbatim:
    # "do not consider our stupid tests as part of the percentage; only the packages of industry standard ones
    # count for a percentage. Fix that reporting now." -- after "Is Icon really at 90%? I do not believe it.":
    # icon's 90% was 811/901 with 730 of the denominator our own master and AST fixtures, the two public suites at
    # 48% and 47% barely moving it). Basis from here: the V column ONLY -- the vendored industry-standard packages
    # graded by their own oracle. M and AST are printed beside as "ours" and never counted. A language with no
    # public suite (snocone, rebus) prints no-public-suite and is excluded from ALL by design, never scored 100%
    # from its own ladder.
    if lang in PROGRESS_NO_PUBLIC_SUITE:
        return "NOSUITE", "", 0, 0, ["no public industry-standard suite exists for this language -- not scored, not in ALL (Lon 2026-09-04); its ladder to the reference's top is reported in L, not as a percent"]
    # pass/total over the V cells only; xfail is never a pass because the grid prints it BESIDE the
    # fraction, never inside it, so an xfail is already excluded by reading the fraction.
    P = T = 0
    work = []
    unreadable = False
    # ⛔⭐⭐ VALUE AND LABEL COME FROM THE SAME TABLE OR THE LINE IS A LIE ABOUT ITSELF (ceo audit of
    # 3fbee86c5, and the catch is exact). Keying staleness on the display's `board:` clause while still
    # reading the NUMBER from the September-10 grid paired one table's value with the other table's
    # freshness -- snobol4's grid said 1689/1736 labelled 09-03 while the display said 1698/1736 today, so
    # the line published the OLD number wearing the NEW number's timestamp. ⭐ That is the same
    # value-from-one-run stamp-from-another defect I had named in my own SCORE merge receipt an hour
    # earlier and then committed myself, one table over: knowing a failure shape is not the same as
    # recognising it, and the recognition came from someone else re-reading my landing.
    # So BOTH now come from the standardized display, which is the table runners actually write and the
    # only one carrying provenance. The grid stays the human-facing summary; the agree gate is what keeps
    # the two honest about each other.
    for key in ("V",):
        idx = GRID_COLUMNS[key][0]
        got, w = counted_fractions(lang, cells[idx])
        work += w
        if got is None:
            unreadable = True
            continue
        for t in sorted(got):
            P += got[t]
            T += t
    # the ladder (L) is OURS too: reported beside the percent, never inside it (Lon 2026-09-04: only the packages count)
    fs = forms_score(lang)
    if fs == "incomplete":
        work.append("L census INCOMPLETE (some rungs have no FORMS) -- reported, not in the percent")
    elif fs is not None:
        work.append("L forms %d/%d witnessed of DECLARED-FROM-THE-REFERENCE -- reported, not in the percent" % fs)
    else:
        work.append("L no usable census -- reported, not in the percent")
    vcell = cells[GRID_COLUMNS["V"][0]]
    _mg, _mw = cell_fractions(cells[GRID_COLUMNS["M"][0]])
    work += ["ours(not counted) M %s" % x for x in _mw]
    for name, n in PROGRESS_ESTIMATED.get(lang, []):
        if name in vcell and any(k in vcell for k in PROGRESS_NO_MARKERS):
            T += n
            work.append("V %s 0/%d (estimated: named, no number)" % (name, n))
    # ⛔⭐⭐ STALENESS KEYS ON THE PROVENANCE LABEL, NEVER ON A DATE FOUND IN PROSE (Lon 2026-09-04, in-chat
    # to ceo: "So I see those cells are unverified. Why are they not verified? Get that fixed."). The old
    # rule scanned the M cell's PROSE for any `MM-DD` and marked the language stale if any of them was older
    # than today. Measured on the live board the hour this was written: snobol4's M cell was measured TODAY
    # at 12:07 and still printed `sno 91%?`, because the cell's own commentary says "seat08, 2026-09-03" --
    # a true sentence about history that the freshness check read as the age of the measurement.
    # ⭐ THE GENERAL SHAPE: a cell that EXPLAINS ITSELF WELL accumulates dates, so the better-documented a
    # cell got the staler it looked, and the only way to clear the mark was to delete the history. A
    # freshness rule that punishes provenance prose is pointed exactly backwards.
    # ⛔ THE LABEL IS THE DISPLAY ROW'S OWN `board:` CLAUSE -- machine-written by cmd_write, one canonical
    # ISO stamp, and the mirror of this very M cell (GRID_MIRROR board->M). No label means the age is
    # UNKNOWN, which reads STALE and NAMES ITSELF rather than passing quietly: an undatable number is not a
    # fresh one. The cell's own explicit word STALE still wins outright.
    # staleness keys on the V cell -- the cell the percent is read from -- by its own ⟨measured …⟩ stamp.
    mcell = vcell
    stale = "STALE" in mcell
    if stale:
        work.append("V reads STALE by the cell's own word")
    label = ""
    if not stale:
        dm = re.search(GRID_STAMP_RE, mcell)
        if dm:
            label = dm.group(0)
            d = datetime.date(int(dm.group(1)), int(dm.group(2)), int(dm.group(3)))
            age = (datetime.date.today() - d).days
            stale = age >= 1
            work.append("V stamp %s (%s)" % (label, "today" if age <= 0 else "%d day(s) old -- STALE" % age))
        else:
            stale = True
            work.append("V carries NO canonical ⟨measured …⟩ stamp -- age UNKNOWN, which is not the same as "
                        "fresh, so it reads STALE until a runner's write stamps this very cell")
    # ⭐ FLOOR, NEVER ROUND. A completion indicator that rounds shows 100% at 99.6%, which is the one
    # number on this line anybody will act on. Floor reaches 100% only when the work is actually done.
    if unreadable:
        return None, "", P, T, work
    pct = (100 * P) // T if T else 0
    return pct, ("?" if stale else ""), P, T, work


# ⛔⭐ COLUMN SEMANTICS -- THE CLASS NO READABILITY CHECK REACHES (ceo ruling 2026-09-03 22:39, after hq_T
# published `reb 100%`). That number came from a census written into the M column by a "|"-split off-by-one:
# the cell was PERFECTLY READABLE and completely wrong, because "22/22" is a well-formed population in a cell
# that is allowed to contain one. No parse rule reaches that -- only knowing what the COLUMN MEANS does.
# So: every runner a cell cites must be of that column's KIND.
# ⛔ ORDER MATTERS AND IS MEASURED, NOT GUESSED: `test_prolog_rung_suite.sh` ends in `_suite.sh` and is a
# LADDER runner, so the ladder patterns must be tried BEFORE the vendor ones or the L column's own runner
# would be convicted of being a vendor suite.
COLUMN_KINDS = {
    "L": [r"_ladder\.sh$", r"rung_suite\.sh$", r"LADDER\.tsv$"],
    "V": [r"_suite\.sh$", r"^scorecard_", r"roast_scoreboard", r"_packages?\.sh$"],
    "M": [r"corpus_suite_harness\.py$", r"^board_.*_master\.sh$", r"^test_corpus_.*\.sh$", r"^test_gate_pascal_m[34].*"],
}
KIND_ORDER = ("L", "V", "M")


def citation_kind(cite):
    base = cite.split("/")[-1]
    for k in KIND_ORDER:
        for pat in COLUMN_KINDS[k]:
            if re.search(pat, base) or re.search(pat, cite):
                return k
    return None


def cmd_columns(a):
    lines = open(SCORE_MD, encoding="utf-8").read().split("\n")
    _hdr, rows, gskip = find_grid(lines)
    bad = []
    checked = 0
    for lang in sorted(rows):
        for key in ("M", "L", "V"):
            cell = rows[lang][GRID_COLUMNS[key][0]]
            cites = sorted(set(re.findall(r"[A-Za-z0-9_./-]+\.(?:sh|py|tsv)", cell)))
            for c in cites:
                checked += 1
                k = citation_kind(c)
                if k is None:
                    continue          # an unrecognised tool is not evidence of a wrong column
                if k != key:
                    bad.append("%s %s cell cites %s, which is a %s-column runner" % (lang, key, c, k))
    # ⛔ THE SECOND GATE OVER THE SAME GRID, AND IT HAD THE SAME BLIND SPOT. A malformed row drops out of
    # `rows`, so its citations stop being checked and this gate's PASS silently stops covering that language.
    # Floor first (nothing read at all), then the malformed-row red, then the finding.
    if not rows:
        die("read ZERO rows from the September-10 grid -- a column-semantics gate over no cells is not a PASS")
    if gskip:
        print("⛔ GATE RED [score_column_semantics]: %d grid row(s) are MALFORMED and their citations were not "
              "checked at all -- this gate cannot vouch for a column it could not read" % len(gskip))
        for l in sorted(gskip):
            _ln, _n = gskip[l]
            print("    %s: grid row at line %d has %d columns, the grid has %d" % (l, _ln + 1, _n, GRID_NCOLS))
        return 1
    if bad:
        print("⛔ GATE RED [score_column_semantics]: %d citation(s) in the wrong column" % len(bad))
        for b in bad:
            print("    " + b)
        print("    ⭐ A cell in the wrong column is READABLE AND WRONG -- the one shape no parse check catches.")
        return 1
    print("GATE PASS(0) [score_column_semantics]: %d runner citation(s) across %d grid row(s) all match their column's kind (M master · L ladder · V vendor)" % (checked, len(rows)))
    return 0


# ⛔⭐ THE TWO TABLES MUST AGREE BY VALUE (ceo approval, 2026-09-03). SCORE.md holds the September-10 grid
# AND the standardized display, and `write` updates only the display -- so every grid cell goes stale the
# moment a runner records a measurement, and NOTHING SAYS SO. Measured four times in one sitting: the raku
# M cell read 41/42 after 42/42 was pushed; the raku V cell had no roast number while the display had one;
# the prolog V cell had no INRIA number while the display had one; each needed a hand edit to find.
# ⛔ IT COMPARES VALUES, NEVER DATES. Same-day staleness is invisible to a freshness check by construction:
# the raku cell carried TODAY'S date and a superseded number, because the date was written by an earlier
# measurement that really was today. A timestamp cannot separate "true and current" from "true this morning".
GRID_MIRROR = {"board": "M", "vendor": "V"}
GRID_DIRECT = {"ladder": "L", "bench": "B"}
# ⛔⭐ THE GRID CELL CARRIES ITS OWN MACHINE-WRITTEN DATE, so the progress line reads VALUE and LABEL from the
# SAME table. Reading the value from the grid and the freshness from the display row published the OLD number
# wearing the NEW number's timestamp (ceo audit of 3fbee86c5; snobol4 grid 1689/1736 labelled with the display's
# today). The marker is appended by cmd_write's grid half, contains no `N/M`, so no fraction reader ever sees it.
GRID_STAMP = "⟨measured %s · %s⟩"
GRID_STAMP_RE = r"⟨measured (\d{4})-(\d{2})-(\d{2}) · [^⟩]*⟩"


def cmd_agree(a):
    lines = open(SCORE_MD, encoding="utf-8").read().split("\n")
    _gh, grid, gskip = find_grid(lines)
    _dh, disp, dskip = find_table(lines)
    bad, warn = [], []
    checked = 0
    # ⛔⭐ A LANGUAGE THIS GATE CANNOT READ IS A MEASUREMENT IT DID NOT MAKE, AND SAYING PASS OVER IT IS THE
    # VACUOUS-GATE CLASS. `if lang not in disp: continue` treated an unreadable display row as nothing to do,
    # so a malformed snobol4 row (see find_table) had this gate printing "GATE PASS(0) ... 11 mirrored cell
    # pair(s)" while blind to the project's LARGEST language -- a green that proved the absence of evidence,
    # not evidence of absence. A gate that cannot measure REFUSES; it never skips-as-success.
    unread = sorted(l for l in grid if l in dskip)
    # ⛔⭐ AND THE SAME QUESTION ASKED OF THE OTHER TABLE, which is the half that was missing. A language whose
    # GRID row is malformed never enters `grid` at all, so the loop below cannot skip it -- it never sees it,
    # and `checked` simply comes out smaller. That is invisible by construction: no arm fails, no language is
    # named, and the summary line's own denominator moves without comment.
    gunread = sorted(gskip)
    for lang in sorted(grid):
        if lang not in disp:
            continue
        _i, dcells = disp[lang]
        for col, gkey in GRID_MIRROR.items():
            dfr, _w = cell_fractions(dcells[COLUMNS[col][0]])
            gfr, _w2 = cell_fractions(grid[lang][GRID_COLUMNS[gkey][0]])
            if dfr is None or gfr is None:
                continue          # an unreadable cell is the readability gate's business, not this one
            checked += 1
            # Compare the populations both tables actually name. A denominator present in ONE table only is
            # the staleness this gate exists for; a denominator in both must carry the same pass count.
            for t in sorted(set(dfr) & set(gfr)):
                if dfr[t] != gfr[t]:
                    bad.append("%s: display %s says %d/%d, grid %s says %d/%d" % (lang, col, dfr[t], t, gkey, gfr[t], t))
            only_d = sorted(set(dfr) - set(gfr))
            only_g = sorted(set(gfr) - set(dfr))
            for t in only_d:
                warn.append("%s: display %s carries a %d-population (%d/%d) the grid %s cell does not" % (lang, col, t, dfr[t], t, gkey))
            for t in only_g:
                warn.append("%s: grid %s carries a %d-population (%d/%d) the display %s cell does not" % (lang, gkey, t, gfr[t], t, col))
    # ⛔⭐ TWO SIGNAL STRENGTHS, AND CONFLATING THEM WOULD MAKE THE GATE USELESS. A SAME-DENOMINATOR
    # CONFLICT (both tables name a 81-population, one says 39 and the other 41) is unambiguous: exactly one
    # of them is wrong and a reader cannot tell which. A population present in only ONE table is weaker --
    # it is usually the dual-write gap, but the two tables legitimately summarise at different grains, so
    # convicting on it would red the board for a design choice. Hard conflicts FAIL; one-sided populations
    # are reported as the staleness debt they are.
    for w in warn:
        print("  ⚠ STALE " + w)
    if warn:
        print("  ⭐ Each of those is the dual-write gap: `write` updates the standardized display and not the grid, so a measurement lands in one table and the other keeps yesterday's.")
    # ⛔⭐⭐ THE POPULATION FLOOR, AND IT IS THE ARM THAT WOULD HAVE CAUGHT hq_B's INCIDENT WITHOUT ANY OF THE
    # ABOVE. A comparison gate whose verdict is "0 conflicts" is computing an emptiness, and `0 conflicts over
    # 10 pairs`, `0 conflicts over 1 pair` and `0 conflicts over NOTHING` are the same arithmetic wearing the
    # same words. Both tables can be perfectly well-formed and still share no comparable cell -- so this floor
    # is not a corollary of the malformed-row checks, it is the independent bar: a gate that graded zero
    # pairs REFUSES rc=2 and never prints the success shape (RULES.md § a test that cannot measure refuses).
    if checked == 0:
        why = ""
        if unread or gunread:
            why = (" -- %d malformed row(s) (%s) are why there was nothing left to compare"
                   % (len(set(unread) | set(gunread)), ", ".join(sorted(set(unread) | set(gunread)))))
        die("compared ZERO mirrored cell pairs across %d grid row(s) and %d display row(s)%s. A gate that "
            "measured nothing does not get to say the two tables agree" % (len(grid), len(disp), why))
    if unread or gunread:
        print("⛔ GATE RED [score_tables_agree]: %d language(s) have a MALFORMED row and were not compared at all "
              "-- this gate cannot claim agreement over a table it could only partly read" % len(set(unread) | set(gunread)))
        for l in unread:
            _ln, _n = dskip[l]
            print("    %s: DISPLAY row at line %d has %d columns, the table has %d" % (l, _ln + 1, _n, PROV_COL + 1))
        for l in gunread:
            _ln, _n = gskip[l]
            print("    %s: SEPTEMBER-10 GRID row at line %d has %d columns, the grid has %d" % (l, _ln + 1, _n, GRID_NCOLS))
        print("    ⭐ A GREEN THAT APPEARS WHILE YOU ARE EDITING THE DATA IS A SUSPECT, NOT A REWARD (hq_B, 2026-09-05):")
        print("      breaking a row is how a population goes to zero, and a population of zero scores as a population with no conflicts.")
        return 1
    if bad:
        print("⛔ GATE RED [score_tables_agree]: %d SAME-DENOMINATOR disagreement(s) -- one of the two tables is wrong and a reader cannot tell which" % len(bad))
        for b in bad:
            print("    " + b)
        print("    ⭐ Compared by VALUE, never by date: same-day staleness (a true date beside a superseded number) is invisible to any freshness check.")
        return 1
    print("GATE PASS(0) [score_tables_agree]: %d mirrored cell pair(s), 0 same-denominator conflicts, %d one-sided population(s) reported above" % (checked, len(warn)))
    return 0


def cmd_progress(a):
    lines = open(SCORE_MD, encoding="utf-8").read().split("\n")
    _hdr, rows, gskip = find_grid(lines)
    # ⭐ The freshness label lives on the STANDARDIZED DISPLAY row, not in the grid: the grid has no
    # provenance column, and the display's `board:` clause is the machine-written mirror of the grid's M
    # cell. Bound by shape via find_table, like every other reader of this file.
    _dh, disp, _ = find_table(lines)
    provs = {l: c[PROV_COL] for l, (_i, c) in disp.items()}
    missing = [l for l, _ in PROGRESS_LANGS if l not in rows]
    if missing:
        # ⛔⭐ NAME THE DIFFERENCE BETWEEN A ROW THAT IS GONE AND A ROW THAT CANNOT BE READ. "No row for icon"
        # sent hq_B looking for a deleted row while the row sat in the file two cells too wide (2026-09-05);
        # the identical message sent seat07 looking for a registry that does not exist (2026-09-04). Both
        # refusals were correct, well-formed and confident, and both pointed AWAY from the defect.
        det = []
        for l in missing:
            if l in gskip:
                _gl, _gn = gskip[l]
                det.append("%s MALFORMED, NOT ABSENT (line %d has %d columns, the grid has %d)" % (l, _gl + 1, _gn, GRID_NCOLS))
            else:
                det.append("%s absent" % l)
        die("the September-10 grid has no readable row for %s -- refusing to publish a progress line over a partial grid" % "; ".join(det))
    cells_out, bars, tp, tt, missing = [], [], 0, 0, []
    for lang, short in PROGRESS_LANGS:
        pct, mark, P, T, work = language_progress(lang, rows[lang], provs.get(lang, ""))
        if pct == "NOSUITE":
            cells_out.append("%s no-public-suite" % short)
            bars.append("%s %s" % (short, "-" * 10))
            if a.verbose:
                sys.stdout.write("  %-8s no-public-suite  %s\n" % (lang, " · ".join(work)))
            continue
        if pct is None:
            # ⛔ NOT SCORED, and deliberately not folded into ALL either: a language whose cell we cannot
            # read must not quietly improve or worsen the headline it is missing from.
            missing.append(short)
            cells_out.append("%s MISSING" % short)
            bars.append("%s %s" % (short, "?" * 10))
            if a.verbose:
                # ⛔ NAME THE ACTUAL REASON. "cell unreadable" was the only MISSING reason when the percent
                # came from M+V; now a language can be MISSING because its CENSUS IS INCOMPLETE, which is a
                # different fact with a different owner and a different fix. One label for two causes sends
                # the reader to the wrong file.
                why = "cell unreadable"
                for x in work:
                    if "census is INCOMPLETE" in x:
                        why = "census incomplete -- not all rungs enumerated"
                        break
                    if "NO usable census" in x:
                        why = "no usable census"
                        break
                sys.stdout.write("  %-8s MISSING   (%s)  %s\n" % (lang, why, " · ".join(work)))
            continue
        tp += P
        tt += T
        cells_out.append("%s %d%%%s" % (short, pct, mark))
        bars.append("%s %s" % (short, "█" * (pct // 10) + "░" * (10 - pct // 10)))
        if a.verbose:
            # ⛔ BOTH ARE `grid` BY CONSTRUCTION, and this line is printed rather than assumed because the
            # audit that caught the split read a claim, not the code: value from GRID_COLUMNS M/V, label
            # from that same cell's own ⟨measured …⟩ stamp. If these two words ever differ, the line is
            # pairing one table's number with another's freshness again.
            sys.stdout.write("  %-8s %3d%%%-1s  %5d/%-5d  value-from=grid label-from=grid  %s\n"
                             % (lang, pct, mark, P, T, " · ".join(work)))
    gh = git(".github", "rev-parse", "--short", "HEAD") or "unknown"
    allpct = (100 * tp) // tt if tt else 0
    # ⛔⭐ ALL IS MISSING WHEN ANY LANGUAGE IS MISSING, AND THAT IS NOT PEDANTRY -- IT IS THE SAME
    # ROUNDING-UP DEFECT ONE LEVEL UP. Measured: when icon's cell became unreadable, dropping its
    # 1555-entry denominator moved ALL from 73% to 85%. So an UNREADABLE CELL MADE THE HEADLINE LOOK
    # BETTER -- exactly what fail-closed exists to prevent, arriving through the aggregate instead of
    # through the cell. ALL claims to be the whole program over all seven; it cannot be computed from
    # six, and a "?" is far too quiet for a twelve-point move.
    scored = [sh for l, sh in PROGRESS_LANGS if l not in PROGRESS_NO_PUBLIC_SUITE]
    allcell = "ALL MISSING (%s unreadable)" % ",".join(missing) if missing else "ALL %d%% (%d with a public suite)" % (allpct, len(scored))
    # ⛔ THE LINE PRINTS ITS OWN BASIS (ceo ruling; hq_T's header kept, wording per Lon): the percent changed meaning today.
    print("PROGRESS 09-10 [basis: V = the vendored industry-standard packages, RUN-graded by their own oracle, ONLY; "
          "our own master, AST fixtures and ladders are printed under --verbose as ours and are NOT in the percent; "
          "a compile-graded suite is named and not counted; no public suite = no percent]")
    print("PROGRESS 09-10 | %s | %s | tree %s %s"
          % (" | ".join(cells_out), allcell, gh, time.strftime("%Y-%m-%d %H:%M %Z")))
    print("  " + "  ".join(bars))
    if a.verbose:
        print("  ALL %d%% = %d/%d over the V (industry-standard package) denominators ONLY -- our master and AST fixtures are printed as ours and never counted (Lon 2026-09-04); ? = the V cell reads STALE, or its own ⟨measured …⟩ stamp is older than today, or it carries no stamp at all (age unknown)." % (allpct, tp, tt))
        for lang, short in PROGRESS_LANGS:
            c = rows[lang]
            print("  %-8s L: %s" % (lang, c[GRID_COLUMNS["L"][0]][:110]))
            print("  %-8s B: %s" % (lang, c[GRID_COLUMNS["B"][0]][:110]))
    return 0


# ⭐ THE ONE SEAT-IDENTITY ACCESSOR, EXPOSED (row `vendor-runners-stamp-unknown-seat-into-the-leaderboard-
# when-s4e-seat-is-unset`, hq_T 2026-09-04).  The GOAL asked for `s4e_seat_name()` so every runner stamping
# SCORE.md resolves its identity ONE way.  It is exposed HERE rather than reimplemented in bash for the reason
# the map was wrong in the first place: the root->seat map already existed in three hand-synced copies
# (s4e_msg.sh, s4e_inbox_hook.sh, derive_measurer() below), and a fourth copy spelled in shell would be one
# more thing to keep in step.  `lib_gate.sh`'s s4e_seat_name() delegates here -- a call shape, not a second
# copy, exactly as gate_score_row does.
#
# ⛔ IT REFUSES RATHER THAN INVENTS.  An unrecognised root exits 2 with the root named.  This is the ONE place
# the leaderboard deliberately diverges from the postoffice bus: the bus falls back to basename($S4E) because
# a seat with no name cannot be mailed, while a BOARD ROW signed by a guessed identity is exactly the
# unattributed claim this whole row exists to kill.  The divergence is pinned by
# test_gate_seat_identity_one_map.sh so that "fixing" it has to be deliberate.
def cmd_seat_name(a):
    who = os.environ.get("S4E_SEAT", "").strip() or derive_measurer()
    if not who:
        die("no seat identity: S4E_SEAT is unset and the root %r is not in the seat map, so it could not be "
            "derived. Pass S4E_SEAT=<name> or run from a known root." % S4E)
    print(who)
    return 0


def main():
    p = argparse.ArgumentParser(description="Rewrite one SCORE.md row from numbers a runner already measured. Runs no suite.")
    sub = p.add_subparsers(dest="cmd")
    w = sub.add_parser("write", help="rewrite one language's one column, in place, with provenance")
    w.add_argument("--lang", required=True)
    w.add_argument("--column", required=True, help="one of: " + ", ".join(sorted(COLUMNS)) + " (display, mirrored to the grid) or " + ", ".join(sorted(GRID_DIRECT)) + " (the September-10 grid L/B cells directly)")
    w.add_argument("--text", required=True, help="the runner's OWN printed board line, verbatim where possible")
    w.add_argument("--measurer", required=True, help="seat/HQ identity that ran it")
    w.add_argument("--modes", default="", help="e.g. m3,m4")
    w.add_argument("--suite", default="", help="name ONE measurement inside a shared cell (e.g. Arizona, JCON, SWI, GNU, fpc); "
                                               "without it the whole cell is replaced, which is wrong for the vendor column")
    w.add_argument("--dry-run", action="store_true")
    w.set_defaults(fn=cmd_write)
    c = sub.add_parser("check", help="report every row's staleness against origin/main")
    c.add_argument("--threshold", type=int, default=STALE_WARN_COMMITS)
    c.add_argument("--no-fetch", action="store_true", help="do not contact origin; grade against the last-known origin/main (for test loops)")
    c.set_defaults(fn=cmd_check)
    g = sub.add_parser("progress", help="print the ONE progress line from the September-10 grid; runs no suite")
    g.add_argument("--verbose", action="store_true", help="show the per-language workings, plus the L and B cells the one-liner omits")
    g.set_defaults(fn=cmd_progress)
    g2 = sub.add_parser("agree", help="assert the September-10 grid and the standardized display agree BY VALUE")
    g2.set_defaults(fn=cmd_agree)
    k = sub.add_parser("columns", help="assert every runner a grid cell cites is of that column's kind")
    k.set_defaults(fn=cmd_columns)
    s = sub.add_parser("selftest", help="prove rewrite-in-place and every refusal path, on a scratch copy")
    s.set_defaults(fn=cmd_selftest)
    n = sub.add_parser("seat-name", help="print THE ONE seat identity derived from the root path; runs no suite")
    n.set_defaults(fn=cmd_seat_name)
    a = p.parse_args()
    if not getattr(a, "fn", None):
        p.print_help()
        return 2
    if a.cmd != "seat-name" and not os.path.exists(SCORE_MD):
        die("no leaderboard at %s (S4E_HOME=%s)" % (SCORE_MD, S4E))
    return a.fn(a)


if __name__ == "__main__":
    raise SystemExit(main())
