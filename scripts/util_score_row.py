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


def cmd_write(a):
    if not a.measurer:
        die("--measurer is required: an unattributed row is a claim with nobody behind it")
    if a.column not in COLUMNS:
        die("unknown --column %r. Known: %s" % (a.column, ", ".join(sorted(COLUMNS))))
    text = a.text.strip()
    if "|" in text or "|" in a.measurer:
        die("a '|' in the cell text would silently split the markdown row into the wrong columns -- rephrase it")
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
    cells[idx] = merge_clause(before, a.suite, text) if a.suite else text
    cells[PROV_COL] = merge_prov(cells[PROV_COL], key, stamp)
    newline = "| " + " | ".join(cells) + " |"
    if a.dry_run:
        print("WOULD REWRITE %s line %d" % (SCORE_MD, i + 1))
        print("  was: %s" % before)
        print("  now: %s" % text)
        print("  prov: %s" % cells[PROV_COL])
        return 0
    lines[i] = newline
    lines = mark_grid_stamp(lines)
    open(SCORE_MD, "w", encoding="utf-8").write("\n".join(lines))
    print("SCORE.md: %s/%s rewritten in place (line %d)" % (a.lang, a.column, i + 1))
    print("  was: %s" % before)
    print("  now: %s" % text)
    print("  prov: %s: %s" % (a.column, stamp))
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
                          ("no measurer", dict(lang="rebus", column="board", text="1/1", measurer="")),
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
# Suites with NO runner or NO number: 0 pass over their file count, so unmeasured coverage reads as
# MISSING rather than as ABSENT. Denominators are the brief's own. Applied only when the cell BOTH names
# the suite and carries a no-number marker, so a suite that later gets a real number stops being estimated.
PROGRESS_ESTIMATED = {"snobol4": [("gimpel", 289), ("snoflake", 180), ("aisnobol", 8), ("dotnet", 14)],
                      "icon": [("ipl", 851)]}
PROGRESS_NO_MARKERS = ("NO NUMBER", "NO RUNNER", "UNGRADED", "NOT VENDORED")


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


def language_progress(lang, cells):
    # pass/total over the M and V cells; xfail is never a pass because the grid prints it BESIDE the
    # fraction, never inside it, so an xfail is already excluded by reading the fraction.
    P = T = 0
    work = []
    unreadable = False
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
    # STALE by the cell's own word, or by a month-day label older than today.
    mcell = cells[GRID_COLUMNS["M"][0]]
    stale = "STALE" in mcell
    if not stale:
        today = datetime.date.today()
        for mm, dd in re.findall(r"\b(\d{2})-(\d{2})\b", re.sub(r"`[^`]*`", "", mcell)):
            try:
                d = datetime.date(today.year, int(mm), int(dd))
            except ValueError:
                continue
            if (today - d).days >= 1:
                stale = True
    # ⭐ FLOOR, NEVER ROUND. A completion indicator that rounds shows 100% at 99.6%, which is the one
    # number on this line anybody will act on. Floor reaches 100% only when the work is actually done.
    if unreadable:
        return None, "", P, T, work
    pct = (100 * P) // T if T else 0
    return pct, ("?" if stale else ""), P, T, work


def cmd_progress(a):
    lines = open(SCORE_MD, encoding="utf-8").read().split("\n")
    _hdr, rows = find_grid(lines)
    missing = [l for l, _ in PROGRESS_LANGS if l not in rows]
    if missing:
        die("the September-10 grid has no row for %s -- refusing to publish a progress line over a partial grid" % ", ".join(missing))
    cells_out, bars, tp, tt, missing = [], [], 0, 0, []
    for lang, short in PROGRESS_LANGS:
        pct, mark, P, T, work = language_progress(lang, rows[lang])
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
            sys.stdout.write("  %-8s %3d%%%-1s  %5d/%-5d  %s\n" % (lang, pct, mark, P, T, " · ".join(work)))
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
        print("  ALL %d%% = %d/%d over the M and V printed denominators; ? = the M cell reads STALE or carries a label older than today." % (allpct, tp, tt))
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
