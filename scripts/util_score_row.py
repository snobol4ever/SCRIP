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
import argparse, os, re, subprocess, sys, time

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


def find_table(lines):
    # Locate the standardized-display grid by its header, and PROVE its shape rather than assuming it.
    hdr = None
    for i, l in enumerate(lines):
        if l.startswith("| Language |"):
            hdr = i
            break
    if hdr is None:
        die("no '| Language |' grid header in %s -- the standardized display is gone or renamed" % SCORE_MD)
    cells = [c.strip() for c in lines[hdr].strip().strip("|").split("|")]
    if len(cells) != PROV_COL + 1:
        die("grid header has %d columns, this helper knows %d (%s) -- the display changed shape; fix this table, do not write into the wrong cell"
            % (len(cells), PROV_COL + 1, ", ".join(COLUMNS)))
    for key, (idx, expect) in COLUMNS.items():
        if not cells[idx].startswith(expect):
            die("column %d is %r, expected it to start with %r (--column %s) -- refusing to write into a renamed column"
                % (idx, cells[idx], expect, key))
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
        row = [l for l in body.split("\n") if l.startswith("| rebus |")]
        clauses = len(re.findall(r"(?:^|; )board:", row[0])) if row else 0
        if len(row) != 1 or "1/48" in body or body.count("2/48") != 2 or clauses != 1:
            print("SELFTEST FAIL: the second write did not replace the first -- %d rebus rows, %d board: clauses, "
                  "first-write text %s" % (len(row), clauses, "SURVIVES" if "1/48" in body else "gone")); ok = False
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
