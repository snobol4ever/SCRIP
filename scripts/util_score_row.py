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
    for i in range(hdr + 2, len(lines)):
        if not lines[i].startswith("|"):
            break
        c = [x.strip() for x in lines[i].strip().strip("|").split("|")]
        if len(c) == PROV_COL + 1:
            rows[c[0]] = (i, c)
    return hdr, rows


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
             "/home/claude_B": "hq_B", "/home/claude_T": "hq_T"}
    if root in fixed:
        return fixed[root]
    m = re.match(r"^/home/claude([0-9]{1,2})$", root)
    if m:
        return "seat%02d" % int(m.group(1))
    return ""


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
    if a.column not in COLUMNS:
        die("unknown --column %r. Known: %s" % (a.column, ", ".join(sorted(COLUMNS))))
    text = a.text.strip()
    if "|" in text or "|" in a.measurer:
        die("a '|' in the cell text would silently split the markdown row into the wrong columns -- rephrase it")
    # ⛔ ';' IS merge_clause's OWN DELIMITER (and merge_prov's), so text carrying one gets silently
    # torn apart the NEXT time anyone writes into a shared clause cell -- measured live landing the
    # IPL row: a --text with two internal ';'s left two orphaned clause fragments (neither started
    # with the --suite key, so neither matched, so neither got replaced or removed) sitting beside
    # the real clause on the very next write. Same shape as the '|'/markdown-column check above, one
    # delimiter over; refuse it before it can happen to the next runner. Use '·' (the project's own
    # convention throughout SCORE.md) or ',' instead.
    if ";" in text:
        die("a ';' in the cell text is merge_clause's own clause delimiter and will silently fragment "
            "on the next write to this cell -- use '·' or ',' instead")
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
    hdr, rows = find_table(lines)
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
    if not a.suite:
        lost = cell_prose_loss(before, text)
        if lost:
            die("overwriting %s/%s would silently discard %d sentence(s) this write never modelled, "
                "even though the measurement may be unchanged -- this is about prose, not the number:\n%s\n"
                "Fold whatever is still true into --text and re-run, or hand-edit the cell instead (as "
                ".github 46ff295c did) -- a runner must never drop it silently. (SCORE tooling owner "
                "hq_T: this refuses rather than auto-preserves; see the row's QA for why.)"
                % (a.lang, a.column, len(lost), "\n".join("  - %s" % l for l in lost)))
    cells[idx] = merge_clause(before, a.suite, text) if a.suite else text
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
            _gh, growsg = find_grid(lines)
        except SystemExit:
            growsg = {}
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
            if gbare == gnew:
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
    hdr, rows = find_table(lines)
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
    worst, unknown, out = 0, 0, []
    for lang in sorted(rows):
        i, cells = rows[lang]
        hashes = re.findall(r"SCRIP `([0-9a-f]{7,40})(?:-DIRTY)?`", cells[PROV_COL])
        if not hashes:
            out.append("  %-9s UNPINNED  (no checkable SCRIP hash in its provenance cell)" % lang)
            unknown += 1
            continue
        for h in sorted(set(hashes)):
            n = git("SCRIP", "rev-list", "--count", "%s..origin/main" % h)
            if n is None:
                out.append("  %-9s UNKNOWN   %s is not a commit origin knows" % (lang, h))
                unknown += 1
                continue
            n = int(n)
            worst = max(worst, n)
            flag = "STALE" if n >= a.threshold else "ok   "
            out.append("  %-9s %s %4d commits behind origin/main (measured on %s)" % (lang, flag, n, h))
    print("SCORE.md staleness (threshold %d commits):" % a.threshold)
    for l in out:
        print(l)
    print("worst=%d unpinned/unknown=%d" % (worst, unknown))
    if worst >= a.threshold:
        print("⚠ WARN: %d row-measurement(s) at or past the %d-commit staleness threshold -- re-measure and rewrite the row." % (
            sum(1 for l in out if "STALE" in l), a.threshold))
        return 1
    return 0


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
        _sh, _sr = find_table(_seed_lines)
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
        _hdr, _rows = find_table(body.split("\n"))
        row = [_rows["rebus"][1]] if "rebus" in _rows else []
        cell = row[0][3] if row else ""
        prov = row[0][PROV_COL] if row else ""
        clauses = len(re.findall(r"(?:^|; )board:", prov))
        if len(row) != 1 or "1/48" in cell or cell.count("2/48") != 2 or clauses != 1:
            print("SELFTEST FAIL: the second write did not replace the first -- %d rebus rows in the bound grid, "
                  "%d board: clauses, first-write text %s" % (len(row), clauses, "SURVIVES" if "1/48" in cell else "gone")); ok = False
        else:
            print("SELFTEST: the second write replaced the first cell and its provenance clause, not appended beside it")
        for label, kw in (("unknown language", dict(lang="klingon", column="board", text="1/1", measurer="s")),
                          ("unknown column", dict(lang="rebus", column="nosuch", text="1/1", measurer="s")),
                          ("no digit", dict(lang="rebus", column="board", text="looks fine", measurer="s")),
                          ("pipe injection", dict(lang="rebus", column="board", text="1/1 | evil", measurer="s")),
                          ("semicolon injection", dict(lang="rebus", column="board", text="1/1; evil", measurer="s"))):
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
        # refused), then prove cmd_write refuses the lossy overwrite and allows the lossless one.
        _lines = open(SCORE_MD, encoding="utf-8").read().split("\n")
        _h2, _r2 = find_table(_lines)
        _ri, _rc = _r2["rebus"]
        _fidx = COLUMNS["floor"][0]
        _rc[_fidx] = real_before
        _lines[_ri] = "| " + " | ".join(_rc) + " |"
        open(SCORE_MD, "w", encoding="utf-8").write("\n".join(_lines))
        a7 = A(); a7.lang = "rebus"; a7.column = "floor"; a7.measurer = "selftest"; a7.modes = ""; a7.suite = ""
        a7.dry_run = False; a7.text = sentence_dropped
        try:
            cmd_write(a7)
            print("SELFTEST FAIL: cmd_write silently applied an overwrite that drops a real sentence"); ok = False
        except SystemExit as e:
            if e.code == 2:
                print("SELFTEST: cmd_write correctly REFUSED rc=2 rather than silently drop prose")
            else:
                print("SELFTEST FAIL: cmd_write's prose-loss refusal exited %s, expected 2" % e.code); ok = False
        a8 = A(); a8.lang = "rebus"; a8.column = "floor"; a8.measurer = "selftest"; a8.modes = ""; a8.suite = ""
        a8.dry_run = False; a8.text = reformatted_only
        try:
            cmd_write(a8)
            print("SELFTEST: cmd_write correctly proceeded on a pure reformat (nothing to lose)")
        except SystemExit as e:
            print("SELFTEST FAIL: cmd_write refused a pure reformat that loses nothing (rc=%s)" % e.code); ok = False

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
        _rh, _rr = find_table(_reset_lines)
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
                ("a bare trailing `fail` still drops", "jcon 38/81 fail", {})):
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
            g = list(_grid); g[GRID_COLUMNS["M"][0]] = mcell; g[GRID_COLUMNS["V"][0]] = ""
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
PROGRESS_DROP = ("smoke", "corpus suite", "the earlier", "readings", "was ", "strict rung suite", "supersedes")
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
# Suites with NO runner or NO number: 0 pass over their file count, so unmeasured coverage reads as
# MISSING rather than as ABSENT. Denominators are the brief's own. Applied only when the cell BOTH names
# the suite and carries a no-number marker, so a suite that later gets a real number stops being estimated.
PROGRESS_ESTIMATED = {"snobol4": [("gimpel", 289), ("snoflake", 180), ("aisnobol", 8), ("dotnet", 14)],
                      "icon": [("ipl", 851)]}
PROGRESS_NO_MARKERS = ("NO NUMBER", "NO RUNNER", "UNGRADED", "NOT VENDORED")
# ⛔⭐ TWO LANGUAGES ARE SCORED BY THEIR LADDER, NOT BY THEIR MASTER (Lon 2026-09-03 20:45 via ceo): Snocone
# and Rebus have NO public conformance suite, so "the manual/report censused into LADDER.tsv IS the standard
# and the ladder green to its top IS the score". Their percent is BUILT RUNGS over DECLARED RUNGS, read from
# the census file itself -- the master board is an instrument for them and must not be the headline.
# ⛔ NO CENSUS => MISSING, never a number: a language whose standard has not been written down has no score,
# and 0% would be a claim we have not earned either.
PROGRESS_LADDER_SCORED = ("snocone", "rebus")


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
        for j in range(i + 2, len(lines)):
            if not lines[j].startswith("|"):
                break
            c = [x.strip() for x in lines[j].strip().strip("|").split("|")]
            if len(c) == GRID_NCOLS:
                rows[c[0]] = c
        return i, rows
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
        hit = [d for d in PROGRESS_DROP if d in ctx]
        if hit:
            dropped.add(t)
            work.append("drop %d/%d (%r)" % (ps, t, hit[0]))
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


def language_progress(lang, cells, prov=""):
    if lang in PROGRESS_LADDER_SCORED:
        ls = ladder_score(lang)
        if ls is None:
            return None, "", 0, 0, ["LADDER-SCORED language with NO config/LADDER.tsv census -- no standard written down, so no score (never 0%)"]
        b, d = ls
        return (100 * b) // d, "", b, d, ["ladder %d/%d rungs BUILT of DECLARED (this language is scored by its ladder, not its master)" % (b, d)]
    # pass/total over the M and V cells; xfail is never a pass because the grid prints it BESIDE the
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
    for key in ("M", "V"):
        idx = GRID_COLUMNS[key][0]
        got, w = cell_fractions(cells[idx])
        work += ["%s %s" % (key, x) for x in w]
        if got is None:
            unreadable = True
            continue
        for t in sorted(got):
            P += got[t]
            T += t
    vcell = cells[GRID_COLUMNS["V"][0]]
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
    mcell = cells[GRID_COLUMNS["M"][0]]
    stale = "STALE" in mcell
    if stale:
        work.append("M reads STALE by the cell's own word")
    label = ""
    if not stale:
        dm = re.search(GRID_STAMP_RE, mcell)
        if dm:
            label = dm.group(0)
            d = datetime.date(int(dm.group(1)), int(dm.group(2)), int(dm.group(3)))
            age = (datetime.date.today() - d).days
            stale = age >= 1
            work.append("M stamp %s (%s)" % (label, "today" if age <= 0 else "%d day(s) old -- STALE" % age))
        else:
            stale = True
            work.append("M carries NO canonical ⟨measured …⟩ stamp -- age UNKNOWN, which is not the same as "
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
    _hdr, rows = find_grid(lines)
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
    if bad:
        print("⛔ GATE RED [score_column_semantics]: %d citation(s) in the wrong column" % len(bad))
        for b in bad:
            print("    " + b)
        print("    ⭐ A cell in the wrong column is READABLE AND WRONG -- the one shape no parse check catches.")
        return 1
    print("GATE PASS(0) [score_column_semantics]: %d runner citation(s) all match their column's kind (M master · L ladder · V vendor)" % checked)
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
# ⛔⭐ THE GRID CELL CARRIES ITS OWN MACHINE-WRITTEN DATE, so the progress line reads VALUE and LABEL from the
# SAME table. Reading the value from the grid and the freshness from the display row published the OLD number
# wearing the NEW number's timestamp (ceo audit of 3fbee86c5; snobol4 grid 1689/1736 labelled with the display's
# today). The marker is appended by cmd_write's grid half, contains no `N/M`, so no fraction reader ever sees it.
GRID_STAMP = "⟨measured %s · %s⟩"
GRID_STAMP_RE = r"⟨measured (\d{4})-(\d{2})-(\d{2}) · [^⟩]*⟩"


def cmd_agree(a):
    lines = open(SCORE_MD, encoding="utf-8").read().split("\n")
    _gh, grid = find_grid(lines)
    _dh, disp = find_table(lines)
    bad, warn = [], []
    checked = 0
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
    _hdr, rows = find_grid(lines)
    # ⭐ The freshness label lives on the STANDARDIZED DISPLAY row, not in the grid: the grid has no
    # provenance column, and the display's `board:` clause is the machine-written mirror of the grid's M
    # cell. Bound by shape via find_table, like every other reader of this file.
    _dh, disp = find_table(lines)
    provs = {l: c[PROV_COL] for l, (_i, c) in disp.items()}
    missing = [l for l, _ in PROGRESS_LANGS if l not in rows]
    if missing:
        die("the September-10 grid has no row for %s -- refusing to publish a progress line over a partial grid" % ", ".join(missing))
    cells_out, bars, tp, tt, missing = [], [], 0, 0, []
    for lang, short in PROGRESS_LANGS:
        pct, mark, P, T, work = language_progress(lang, rows[lang], provs.get(lang, ""))
        if pct is None:
            # ⛔ NOT SCORED, and deliberately not folded into ALL either: a language whose cell we cannot
            # read must not quietly improve or worsen the headline it is missing from.
            missing.append(short)
            cells_out.append("%s MISSING" % short)
            bars.append("%s %s" % (short, "?" * 10))
            if a.verbose:
                sys.stdout.write("  %-8s MISSING   (cell unreadable)  %s\n" % (lang, " · ".join(work)))
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
    allcell = "ALL MISSING (%s unreadable)" % ",".join(missing) if missing else "ALL %d%%" % allpct
    print("PROGRESS 09-10 | %s | %s | tree %s %s"
          % (" | ".join(cells_out), allcell, gh, time.strftime("%Y-%m-%d %H:%M %Z")))
    print("  " + "  ".join(bars))
    if a.verbose:
        print("  ALL %d%% = %d/%d over the M and V printed denominators; ? = the M cell reads STALE, or its own ⟨measured …⟩ stamp is older than today, or it carries no stamp at all (age unknown)." % (allpct, tp, tt))
        for lang, short in PROGRESS_LANGS:
            c = rows[lang]
            print("  %-8s L: %s" % (lang, c[GRID_COLUMNS["L"][0]][:110]))
            print("  %-8s B: %s" % (lang, c[GRID_COLUMNS["B"][0]][:110]))
    return 0


def main():
    p = argparse.ArgumentParser(description="Rewrite one SCORE.md row from numbers a runner already measured. Runs no suite.")
    sub = p.add_subparsers(dest="cmd")
    w = sub.add_parser("write", help="rewrite one language's one column, in place, with provenance")
    w.add_argument("--lang", required=True)
    w.add_argument("--column", required=True, help="one of: " + ", ".join(sorted(COLUMNS)))
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
    a = p.parse_args()
    if not getattr(a, "fn", None):
        p.print_help()
        return 2
    if not os.path.exists(SCORE_MD):
        die("no leaderboard at %s (S4E_HOME=%s)" % (SCORE_MD, S4E))
    return a.fn(a)


if __name__ == "__main__":
    raise SystemExit(main())
