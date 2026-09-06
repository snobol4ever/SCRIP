#!/usr/bin/env python3
"""util_gate_wiring.py -- THE ONE POPULATION OF GATES, and the RATCHET over it.

WHAT IT PROTECTS (row requested by hq_S 2026-09-06, routed by the ceo as CEO-326 because it is a
handoff_status check rather than law).  hq_S's measured case, verbatim: test_gate_sno_fence_body_dynamic_
operand.sh landed on disk IN THE SAME PUSH as the cure it grades, and was never added to the recipe.  For a
day the ONE instrument that can grade that cure was run by nothing.  Nothing could have noticed: the gate was
green when run by hand, ls found it, the commit that added it looked complete, and every board was
byte-identical with and without the cure BY CONSTRUCTION, so no suite could miss it either.  AN UNWIRED GATE
IS INDISTINGUISHABLE FROM A WIRED ONE BY EVERY CHECK EXCEPT READING THE RECIPE.  This is that check.

WHY A RATCHET AND NOT THE CENSUS THAT WAS ASKED FOR.  Measured before a line of it was written: 279
scripts/test_gate_*.sh on disk, 64 reachable from `make test` + `make test-postoffice`.  UNWIRED IS THE NORM,
roughly four out of five.  The check as originally specified -- assert reachable, or exempted with a reason --
would have emitted 215 violations on its first run inside handoff_status.sh, which is a check nobody reads
twice, and a muted check is worse than no check because it looks like coverage.  So it fires on the
TRANSITION, never on the backlog: hq_S's own case is the transition case, and it is the only moment anyone can
act cheaply -- the author is still there, the commit is still open, and wiring it is one line.  Once it is in
the backlog it is a stranger's debt about a cure they did not write.  The backlog is a NUMBER THAT CAN ONLY GO
DOWN (TASK-CEILING), not an alarm.

⭐ THE FLOOR IS A SET OF NAMES, NEVER A COUNT (hq_S's constraint, and it is the one that would have been got
wrong).  169 to 168 with a different name added and an old one dropped is a GREEN RATCHET OVER A REAL
REGRESSION -- the same defect as a board whose total holds while its FAIL set changes underneath.  So the
floor is the sorted NAME SET, and every refusal NAMES the gates that moved.

⭐ AND IT PRINTS THE DIFFERENCE, NOT JUST THE REFUSAL (hq_S, 2026-09-06).  "gate X left the reachable set" is
actionable in one line; "the floor moved" sends its reader to diff two files by hand, and the reader who has
to do that is the reader who mutes the check.  Every refusal class below prints one line per NAME with its own
cure command.

⛔ ONE FILE WITH A CLASS COLUMN, NOT TWO FILES.  hq_S asked for the split by analogy to UNGRADABLE vs UNGRADED
(a gate deliberately out of make test is a RULING -- nobody owes work; a gate nobody has wired yet is a TASK --
somebody can pick it up) and offered either two files or one file with a class column.  ONE FILE, and the
reason decides it: THE FAILURE WE BOTH FEAR IS A NAME THAT APPEARS IN NEITHER LIST, and two files make that
state easy to reach and hard to see.  The property worth protecting is not "each list is right", it is "every
name is in exactly one bucket" -- and only a single population can check that.  Arm SUM is that check.

  WIRED   reachable from the recipe today.  MEASURED, so it carries no reason and needs no declarer.
  RULING  deliberately out of make test.  ⛔ REASON REQUIRED AND REFUSED IF EMPTY, taken at the moment of
          exemption from the person exempting it -- otherwise the exemption list quietly becomes the new
          silent backlog with a tidier name and the same shape as the 215.
  TASK    unwired, nobody has ruled.  This is the debt.  Its size is capped by TASK-CEILING, which `adopt`
          lowers and never raises, so a NEWLY LANDED UNWIRED GATE cannot join it silently -- the author's two
          honest options are WIRE IT or RULE IT WITH A REASON.  Raising the ceiling is a deliberate hand edit
          that leaves a fingerprint in the diff.

⛔ REACHABILITY MEANS EXECUTION, AND THE CLOSURE FOLLOWS LITERAL NAMES ONLY.  A gate reached only because some
census GLOBS scripts/test_gate_*.sh and READS the files (test_gate_runners_refuse_on_a_stale_binary.sh does
exactly this, twice) is not run by anything, and counting it would be the instrument lying in the
comfortable direction.  Comment text is stripped from every line before names are taken, in the Makefile
expansion as well as in the scripts: `make -n` echoes a recipe line's trailing `# ...` comment verbatim
because the shell, not make, owns that `#`, and the first draft of this file scored a gate REACHABLE that
exists only inside such a comment.  Lines beginning `echo`/`printf` are skipped for the same reason -- a gate
NAMED in a diagnostic is not a gate RUN.
"""
import argparse, datetime, os, re, subprocess, sys

CLASSES = ("WIRED", "RULING", "TASK")
CEIL_KEY = "# TASK-CEILING:"
_CMT = re.compile(r"(^|\s)#.*$")
_SH = re.compile(r"[A-Za-z0-9_]+\.sh")


def sh_names(text):
    """Every *.sh basename that a line could actually EXECUTE. Comments and diagnostics excluded."""
    out = set()
    for line in text.splitlines():
        line = _CMT.sub("", line)
        s = line.strip()
        if not s or s.startswith(("echo ", "printf ")):
            continue
        out |= set(_SH.findall(line))
    return out


def disk_gates(root):
    d = os.path.join(root, "scripts")
    return sorted(f for f in os.listdir(d) if f.startswith("test_gate_") and f.endswith(".sh"))


def txt_names(text):
    """Declared-population files a recipe READS. See reachable() for why this indirection is followed."""
    out = set()
    for line in text.splitlines():
        line = _CMT.sub("", line)
        out |= set(re.findall(r"[A-Za-z0-9_]+\.txt", line))
    return out


REQUIRED_TARGETS = ("test",)
OPTIONAL_TARGETS = ("test-postoffice", "preflight")


def reachable(root, skipped=None):
    """The transitive closure of scripts EXECUTED by the make targets that run gates.

    ⛔ REFUSES rather than guessing: `make -n` is the authority on what a recipe expands to (it resolves the
    variables a hand parse of the Makefile would have to re-implement), so a `test` target that cannot expand
    is a CANNOT-MEASURE, never an empty reachable set -- an empty set would read as "every gate just left the
    floor". The optional targets are SKIPPED and NAMED if they will not expand, never silently dropped: a
    target that quietly disappears takes its gates out of the wired set, and the header has to say which
    reading produced that, or the refusal is loud but undiagnosable.

    ⛔⭐ AND IT FOLLOWS A RECIPE'S DECLARED-POPULATION FILE, which is not a refinement -- it is the difference
    between "reachable from the recipe" and "run by something", and the first draft got it wrong by 22 gates.
    `make preflight` landed today reading its arms from scripts/preflight_arms.txt (`grep -E '^scripts/' ...`),
    deliberately, because a static grep over the scripts cannot classify them in either direction. So `make -n
    preflight` names NO gate at all while running 31, and this instrument confidently called 22 of them run by
    nothing. ⭐ THE GENERAL FORM, and it is the same one this file already warns about in the other direction:
    the moment a target reads its population from a file, EXPANDING THE RECIPE STOPS ANSWERING THE QUESTION
    YOU ASKED. Only *.txt files under scripts/ that a seeded recipe actually names are followed, and only
    their `^scripts/` lines -- an over-broad rule here would quietly mark gates wired that nothing runs, which
    is the comfortable direction and the one nobody would notice."""
    seed, texts = set(), []
    for t in REQUIRED_TARGETS + OPTIONAL_TARGETS:
        p = subprocess.run(["make", "-n", "--no-print-directory", t], cwd=root,
                           capture_output=True, text=True, timeout=300)
        if p.returncode != 0:
            if t in REQUIRED_TARGETS:
                raise RuntimeError("`make -n %s` failed rc=%d: %s" % (t, p.returncode, (p.stderr or "").strip()[:400]))
            if skipped is not None:
                skipped.append(t)
            continue
        seed |= sh_names(p.stdout)
        texts.append(p.stdout)
    for blob in texts:
        for t in txt_names(blob):
            pop = os.path.join(root, "scripts", t)
            if not os.path.exists(pop):
                continue
            with open(pop, encoding="utf-8", errors="replace") as fh:
                for line in fh:
                    if line.startswith("scripts/"):
                        seed |= set(_SH.findall(line))
    seen, work = set(), list(seed)
    while work:
        n = work.pop()
        if n in seen:
            continue
        seen.add(n)
        p = os.path.join(root, "scripts", n)
        if os.path.exists(p):
            with open(p, encoding="utf-8", errors="replace") as fh:
                work += [r for r in sh_names(fh.read()) if r not in seen]
    return seen


def tsv_path(root):
    return os.path.join(root, "scripts", "gate_wiring.tsv")


def load(root):
    """-> (rows, ceiling, header_lines). rows is a list of [gate, cls, reason, by, utc] in file order."""
    path = tsv_path(root)
    rows, header, ceiling = [], [], None
    if not os.path.exists(path):
        return rows, ceiling, header
    with open(path, encoding="utf-8") as fh:
        for line in fh:
            line = line.rstrip("\n")
            if line.startswith("#") or not line.strip():
                header.append(line)
                if line.startswith(CEIL_KEY):
                    try:
                        ceiling = int(line.split(":", 1)[1].strip())
                    except ValueError:
                        ceiling = None
                continue
            f = line.split("\t")
            f += ["-"] * (5 - len(f))
            rows.append(f[:5])
    return rows, ceiling, header


def save(root, rows, ceiling, header):
    out = []
    for h in header:
        out.append(CEIL_KEY + " %d" % ceiling if h.startswith(CEIL_KEY) else h)
    if not any(h.startswith(CEIL_KEY) for h in header):
        out.append(CEIL_KEY + " %d" % ceiling)
    order = {c: i for i, c in enumerate(CLASSES)}
    for r in sorted(rows, key=lambda r: (order.get(r[1], 9), r[0])):
        out.append("\t".join(r))
    with open(tsv_path(root), "w", encoding="utf-8") as fh:
        fh.write("\n".join(out) + "\n")


def blank(v):
    return v is None or v.strip() in ("", "-")


def analyse(root):
    """One measurement, shared by check/adopt, so the two can never disagree about the world."""
    rows, ceiling, header = load(root)
    disk = set(disk_gates(root))
    skipped = []
    reach = reachable(root, skipped=skipped)
    reach_gates = {g for g in disk if g in reach}
    rec = {}
    dupes = []
    for r in rows:
        if r[0] in rec:
            dupes.append(r[0])
        rec[r[0]] = r
    return dict(rows=rows, ceiling=ceiling, header=header, disk=disk, reach=reach_gates,
                rec=rec, dupes=sorted(set(dupes)), skipped=skipped)


def _cure(cmd):
    return "         cure: %s" % cmd


def check(root, out=sys.stdout):
    """rc 0 clean · 1 the ratchet refuses (every refusal NAMES its gates) · 2 cannot measure."""
    try:
        a = analyse(root)
    except Exception as e:                                    # noqa: BLE001 -- any failure to measure is rc=2
        print("⛔ GATE-WIRING UNVERIFIED — cannot measure: %s" % e, file=out)
        print("   This is NOT clean and NOT a refusal: nothing was examined.", file=out)
        return 2
    rows, ceiling, disk, reach, rec = a["rows"], a["ceiling"], a["disk"], a["reach"], a["rec"]
    by_cls = {c: sorted(r[0] for r in rows if r[1] == c) for c in CLASSES}
    task_n = len(by_cls["TASK"])
    print("=== GATE WIRING RATCHET — %d gate(s) on disk · %d reachable from %s ==="
          % (len(disk), len(reach), " + ".join("`make %s`" % t for t in REQUIRED_TARGETS + OPTIONAL_TARGETS
                                               if t not in a["skipped"])), file=out)
    if a["skipped"]:
        print("    ⚠ target(s) that would not expand and were SKIPPED, so their gates read UNREACHABLE here: %s"
              % ", ".join(a["skipped"]), file=out)
    print("    recorded: WIRED %d · RULING %d · TASK %d   (TASK-CEILING %s)"
          % (len(by_cls["WIRED"]), len(by_cls["RULING"]), task_n,
             "unset" if ceiling is None else ceiling), file=out)
    v = 0
    # ⛔ THE RATCHET ARM. A gate that WAS run by make test and is run by nothing now. This is the regression the
    # whole file exists for, and it is why the floor is a name set: a count-only floor stays green when this
    # gate leaves and an unrelated one arrives in the same push.
    left = sorted(g for g in by_cls["WIRED"] if g in disk and g not in reach)
    if left:
        v += len(left)
        print("\n⛔ LEFT THE WIRED SET (%d) — was run by `make test`, is run by NOTHING now:" % len(left), file=out)
        for g in left:
            print("      %s" % g, file=out)
            print(_cure("re-add it to the Makefile `test:` recipe, or name the new state:"), file=out)
            print("               python3 scripts/util_gate_wiring.py declare %s RULING \"<why it is deliberately out>\"" % g, file=out)
    # ⛔ hq_S'S MEASURED CASE, EXACTLY: on disk, run by nothing, and nobody has said so. Fires at the ONE moment
    # it is cheap -- the author is still here and wiring it is one line in the recipe.
    new_unwired = sorted(g for g in disk if g not in rec and g not in reach)
    if new_unwired:
        v += len(new_unwired)
        print("\n⛔ LANDED UNWIRED AND UNDECLARED (%d) — on disk, in no recipe, in no list:" % len(new_unwired), file=out)
        for g in new_unwired:
            print("      %s" % g, file=out)
            print(_cure("wire it into the Makefile `test:` recipe (one line), or rule it out WITH A REASON:"), file=out)
            print("               python3 scripts/util_gate_wiring.py declare %s RULING \"<why it is deliberately out>\"" % g, file=out)
    new_wired = sorted(g for g in disk if g not in rec and g in reach)
    if new_wired:
        v += len(new_wired)
        print("\n⛔ WIRED BUT NOT IN THE FLOOR (%d) — good news the file does not know: record them or the ratchet cannot protect them:" % len(new_wired), file=out)
        for g in new_wired:
            print("      %s" % g, file=out)
        print(_cure("python3 scripts/util_gate_wiring.py adopt"), file=out)
    # ⛔ A RULING/TASK LINE OVER A GATE THAT IS WIRED TODAY IS A HOLE IN THE RATCHET, not a harmless staleness:
    # while it says TASK, arm LEFT-THE-WIRED-SET is switched off for that gate, so it can be un-wired in silence.
    promoted = sorted(g for g in by_cls["RULING"] + by_cls["TASK"] if g in reach)
    if promoted:
        v += len(promoted)
        print("\n⛔ WIRED NOW BUT STILL LISTED AS RULING/TASK (%d) — the ratchet is OFF for these until promoted:" % len(promoted), file=out)
        for g in promoted:
            print("      %-58s recorded %s" % (g, rec[g][1]), file=out)
        print(_cure("python3 scripts/util_gate_wiring.py adopt"), file=out)
    gone = sorted(g for g in rec if g not in disk)
    if gone:
        v += len(gone)
        print("\n⛔ RECORDED BUT NOT ON DISK (%d) — deleted or renamed; the line is now a claim about nothing:" % len(gone), file=out)
        for g in gone:
            print("      %-58s recorded %s" % (g, rec[g][1]), file=out)
        print(_cure("python3 scripts/util_gate_wiring.py adopt"), file=out)
    # ⛔ hq_S'S CONSTRAINT 2, and the one place a ratchet rots: an exemption list that accepts a blank reason IS
    # the new silent backlog with a tidier name. Refused exactly as the package inventory refuses an empty
    # sidecar reason.
    noreason = sorted(r[0] for r in rows if r[1] == "RULING" and (blank(r[2]) or blank(r[3])))
    if noreason:
        v += len(noreason)
        print("\n⛔ RULING WITHOUT A REASON OR A DECLARER (%d) — an exemption nobody signed is not a ruling:" % len(noreason), file=out)
        for g in noreason:
            print("      %s" % g, file=out)
            print(_cure("python3 scripts/util_gate_wiring.py declare %s RULING \"<reason>\"" % g), file=out)
    bad = sorted(r[0] for r in rows if r[1] not in CLASSES)
    if bad:
        v += len(bad)
        print("\n⛔ UNKNOWN CLASS (%d) — must be one of %s:" % (len(bad), "/".join(CLASSES)), file=out)
        for g in bad:
            print("      %-58s recorded %r" % (g, rec[g][1]), file=out)
    if a["dupes"]:
        v += len(a["dupes"])
        print("\n⛔ LISTED TWICE (%d) — every name is in exactly ONE bucket, that is the whole point of one file:" % len(a["dupes"]), file=out)
        for g in a["dupes"]:
            print("      %s" % g, file=out)
    if ceiling is None:
        v += 1
        print("\n⛔ NO TASK-CEILING — the debt has no cap, so it is not a ratchet. Add `%s %d` to the header." % (CEIL_KEY, task_n), file=out)
    elif task_n > ceiling:
        v += 1
        print("\n⛔ TASK-CEILING EXCEEDED — %d > %d. The debt may only go DOWN." % (task_n, ceiling), file=out)
        print(_cure("wire the new gate(s), or rule them out with a reason. Raising the ceiling is a deliberate"), file=out)
        print("               hand edit of the header line, which is what makes it visible in the diff.", file=out)
    print("\nDIFF vs the recorded floor: wired %+d · on-disk %+d · task %d/%d%s"
          % (len(reach) - len(by_cls["WIRED"]), len(disk) - len(rows), task_n,
             -1 if ceiling is None else ceiling, "" if v else "  (no change)"), file=out)
    if v:
        print("GATE-WIRING REFUSES — %d gate(s) named above." % v, file=out)
        return 1
    print("GATE-WIRING CLEAN — every gate on disk is in exactly one bucket; no gate left the wired set.", file=out)
    return 0


def adopt(root, out=sys.stdout):
    """The MECHANICAL half only: promote what is measurably wired, drop what is gone, lower the ceiling.

    ⛔ IT NEVER INVENTS A CLASS. A gate that is on disk, unwired and unrecorded is the one state a machine must
    not resolve -- classing it TASK would let a newly landed unwired gate join the backlog silently, which is
    the exact defect this file exists to catch, and classing it RULING would be inventing a reason on behalf of
    somebody who never gave one. So adopt writes everything it can PROVE and then exits 1 naming the rest with
    the `declare` line to run. Refusing the whole write because of one such gate would block the good path on
    the bad one; doing the write and returning 0 would be the silent adoption."""
    a = analyse(root)
    rows, ceiling, header, disk, reach, rec = (a["rows"], a["ceiling"], a["header"], a["disk"], a["reach"], a["rec"])
    kept, promoted, dropped = [], [], []
    seen = set()
    for r in rows:
        if r[0] in seen:
            continue
        seen.add(r[0])
        if r[0] not in disk:
            dropped.append(r[0]); continue
        if r[0] in reach:
            if r[1] != "WIRED":
                promoted.append("%s (%s -> WIRED)" % (r[0], r[1]))
            kept.append([r[0], "WIRED", "-", "-", "-"]); continue
        kept.append(r)
    added = []
    for g in sorted(disk - seen):
        if g in reach:
            kept.append([g, "WIRED", "-", "-", "-"]); added.append(g)
    stuck = sorted(g for g in disk if g not in reach and g not in {r[0] for r in kept})
    task_n = len([r for r in kept if r[1] == "TASK"])
    new_ceiling = task_n if ceiling is None else min(ceiling, task_n)
    save(root, kept, new_ceiling, header)
    print("adopt: WIRED %d · RULING %d · TASK %d (ceiling %s -> %d)"
          % (len([r for r in kept if r[1] == "WIRED"]), len([r for r in kept if r[1] == "RULING"]),
             task_n, "unset" if ceiling is None else ceiling, new_ceiling), file=out)
    for label, names in (("promoted to WIRED", promoted), ("added as WIRED", added), ("dropped (not on disk)", dropped)):
        if names:
            print("  %s (%d):" % (label, len(names)), file=out)
            for n in names:
                print("      %s" % n, file=out)
    if not (promoted or added or dropped) and new_ceiling == ceiling:
        print("  nothing to adopt — the file already matches the tree.", file=out)
    if stuck:
        print("\n⛔ NOT ADOPTED (%d) — on disk, run by nothing, and no human has said why. adopt will not guess:" % len(stuck), file=out)
        for g in stuck:
            print("      %s" % g, file=out)
            print("         wire it into the Makefile `test:` recipe, or:", file=out)
            print("         python3 scripts/util_gate_wiring.py declare %s RULING \"<why it is deliberately out>\"" % g, file=out)
        return 1
    return 0


def declare(root, gate, cls, reason, by=None, out=sys.stdout):
    """rc 0 written · 2 refused. The reason is taken HERE, at the moment of exemption, from the person exempting."""
    if cls not in ("RULING", "TASK"):
        print("⛔ REFUSED — class must be RULING or TASK. WIRED is MEASURED, never declared: wire the gate and run `adopt`.", file=out)
        return 2
    if blank(reason) or "\t" in reason or "\n" in reason:
        print("⛔ REFUSED — a %s needs a real reason, on one line, with no tab. An exemption list that accepts a" % cls, file=out)
        print("   blank reason is the new silent backlog with a tidier name.", file=out)
        return 2
    a = analyse(root)
    if gate not in a["disk"]:
        print("⛔ REFUSED — %s is not on disk under scripts/. Nothing to declare." % gate, file=out)
        return 2
    if gate in a["reach"]:
        print("⛔ REFUSED — %s IS reachable from the recipe today; declaring it %s would make the file lie and" % (gate, cls), file=out)
        print("   switch the ratchet OFF for a gate that is actually wired. Run `adopt` instead.", file=out)
        return 2
    rows, ceiling, header = a["rows"], a["ceiling"], a["header"]
    was = a["rec"].get(gate)
    if cls == "TASK" and (was is None or was[1] != "TASK") and ceiling is not None:
        if len([r for r in rows if r[1] == "TASK"]) + 1 > ceiling:
            print("⛔ REFUSED — TASK-CEILING is %d and this would make it %d. THE DEBT MAY ONLY GO DOWN."
                  % (ceiling, len([r for r in rows if r[1] == "TASK"]) + 1), file=out)
            print("   A newly landed gate has two honest homes: the recipe, or a RULING with a reason. If this really", file=out)
            print("   is neither, raise `%s` by hand -- a visible edit somebody can question in the diff." % CEIL_KEY.strip("# :"), file=out)
            return 2
    who = by or os.environ.get("S4E_SEAT") or ""
    if blank(who):
        print("⛔ REFUSED — no declarer. Pass --by <seat> or set S4E_SEAT: an exemption nobody signed is not a ruling.", file=out)
        return 2
    now = datetime.datetime.now(datetime.timezone.utc).strftime("%Y-%m-%dT%H:%MZ")
    rows = [r for r in rows if r[0] != gate]
    rows.append([gate, cls, reason.strip(), who, now])
    task_n = len([r for r in rows if r[1] == "TASK"])
    save(root, rows, task_n if ceiling is None else min(ceiling, task_n), header)
    print("declared %s %s by %s: %s" % (gate, cls, who, reason.strip()), file=out)
    return 0



def _fx(td, gates, recipe, tsv):
    """Build a scratch SCRIP-shaped root: a Makefile whose `test:` recipe is exactly `recipe`, and scripts/.

    ⛔ IT WIPES scripts/ FIRST. The first draft only added files, so every arm ran against the union of every
    earlier arm's fixture -- three arms went red for gates a later arm had left lying about, and the failure
    read as a bug in adopt. A fixture builder that accumulates is testing a world no arm described."""
    import shutil as _sh
    _sh.rmtree(os.path.join(td, "scripts"), ignore_errors=True)
    os.makedirs(os.path.join(td, "scripts"), exist_ok=True)
    with open(os.path.join(td, "Makefile"), "w") as fh:
        fh.write("test:\n")
        for line in recipe:
            fh.write("\t%s\n" % line)
        fh.write("test-postoffice:\n\t@true\n")
    for g in gates:
        with open(os.path.join(td, "scripts", g), "w") as fh:
            fh.write("#!/usr/bin/env bash\nexit 0\n")
    if tsv is not None:
        with open(os.path.join(td, "scripts", "gate_wiring.tsv"), "w") as fh:
            fh.write(tsv)
    return td


def selftest(out=sys.stdout):
    """Hermetic, offline, sub-second. Every arm is a FAIL-ONCE proof: the fixture is built so the arm's own
    refusal is the ONLY thing that can fire, and a green here therefore means the refusal path RAN -- not that
    nothing happened to be wrong. An instrument whose refusals are never exercised is the false-green shape
    this whole file was written against."""
    import io, shutil, tempfile
    fails = []

    def ck(name, got, want):
        ok = got == want
        print("  %-4s %-46s rc=%s want=%s" % ("ok" if ok else "FAIL", name, got, want), file=out)
        if not ok:
            fails.append(name)

    def run(fn, *a, **kw):
        buf = io.StringIO()
        rc = fn(*a, out=buf, **kw)
        return rc, buf.getvalue()

    G = ["test_gate_a.sh", "test_gate_b.sh", "test_gate_c.sh"]
    HDR = "# scratch\n%s 1\n" % CEIL_KEY
    print("--- util_gate_wiring selftest ---", file=out)
    td = tempfile.mkdtemp(prefix="gw_")
    try:
        # ARM 1 CLEAN: two wired, one declared TASK inside the ceiling, nothing on disk unaccounted for.
        _fx(td, G, ["bash scripts/test_gate_a.sh", "bash scripts/test_gate_b.sh"],
            HDR + "test_gate_a.sh\tWIRED\t-\t-\t-\ntest_gate_b.sh\tWIRED\t-\t-\t-\ntest_gate_c.sh\tTASK\t-\t-\t-\n")
        rc, txt = run(check, td); ck("1 clean", rc, 0)
        # ARM 2 THE RATCHET: b leaves the recipe. It must refuse AND NAME b -- printing the difference, not
        # "the floor moved". This is the arm the whole file exists for.
        _fx(td, G, ["bash scripts/test_gate_a.sh"],
            HDR + "test_gate_a.sh\tWIRED\t-\t-\t-\ntest_gate_b.sh\tWIRED\t-\t-\t-\ntest_gate_c.sh\tTASK\t-\t-\t-\n")
        rc, txt = run(check, td); ck("2 left-the-wired-set", rc, 1)
        ck("2 names the gate that left", "LEFT THE WIRED SET" in txt and "test_gate_b.sh" in txt, True)
        # ARM 3 THE COUNT-ONLY TRAP hq_S NAMED: b leaves and d arrives wired in the SAME edit. WIRED count is
        # unchanged (2 -> 2), so a count floor is GREEN over a real regression. The name set must still refuse.
        _fx(td, G + ["test_gate_d.sh"], ["bash scripts/test_gate_a.sh", "bash scripts/test_gate_d.sh"],
            HDR + "test_gate_a.sh\tWIRED\t-\t-\t-\ntest_gate_b.sh\tWIRED\t-\t-\t-\ntest_gate_c.sh\tTASK\t-\t-\t-\n")
        rc, txt = run(check, td); ck("3 count-unchanged regression", rc, 1)
        ck("3 names b, not just a total", "test_gate_b.sh" in txt, True)
        # ARM 4 hq_S'S MEASURED CASE: a gate lands on disk, in no recipe, in no list.
        _fx(td, G + ["test_gate_new.sh"], ["bash scripts/test_gate_a.sh", "bash scripts/test_gate_b.sh"],
            HDR + "test_gate_a.sh\tWIRED\t-\t-\t-\ntest_gate_b.sh\tWIRED\t-\t-\t-\ntest_gate_c.sh\tTASK\t-\t-\t-\n")
        rc, txt = run(check, td); ck("4 landed unwired+undeclared", rc, 1)
        ck("4 names it with its cure", "LANDED UNWIRED" in txt and "test_gate_new.sh" in txt, True)
        # ARM 5 THE HOLE: c is listed TASK but IS wired. While the file says TASK the ratchet is off for c.
        _fx(td, G, ["bash scripts/test_gate_a.sh", "bash scripts/test_gate_b.sh", "bash scripts/test_gate_c.sh"],
            HDR + "test_gate_a.sh\tWIRED\t-\t-\t-\ntest_gate_b.sh\tWIRED\t-\t-\t-\ntest_gate_c.sh\tTASK\t-\t-\t-\n")
        rc, txt = run(check, td); ck("5 wired-but-listed-TASK", rc, 1)
        rc, txt = run(adopt, td); ck("5 adopt promotes it", rc, 0)
        rc, txt = run(check, td); ck("5 clean after adopt", rc, 0)
        rows, ceil, _ = load(td); ck("5 ceiling ratcheted down", ceil, 0)
        # ARM 6 A RECORDED GATE IS DELETED: the line is a claim about nothing.
        os.remove(os.path.join(td, "scripts", "test_gate_c.sh"))
        rc, txt = run(check, td); ck("6 recorded-but-gone", rc, 1)
        ck("6 names the deleted gate", "test_gate_c.sh" in txt, True)
        rc, txt = run(adopt, td); ck("6 adopt drops it", rc, 0)
        rc, txt = run(check, td); ck("6 clean after adopt", rc, 0)
        # ARM 7 hq_S'S CONSTRAINT 2: a RULING with a blank reason is refused, and one with a blank declarer too.
        _fx(td, G, ["bash scripts/test_gate_a.sh"],
            HDR + "test_gate_a.sh\tWIRED\t-\t-\t-\ntest_gate_b.sh\tRULING\t-\t-\t-\ntest_gate_c.sh\tRULING\tlong bench\t-\t-\n")
        rc, txt = run(check, td); ck("7 ruling without reason/declarer", rc, 1)
        ck("7 names both", "test_gate_b.sh" in txt and "test_gate_c.sh" in txt, True)
        # ARM 8 declare REFUSES an empty reason, a wired gate, and the WIRED class itself.
        rc, txt = run(declare, td, "test_gate_b.sh", "RULING", "   ", by="hq_T"); ck("8 declare empty reason", rc, 2)
        rc, txt = run(declare, td, "test_gate_a.sh", "RULING", "real reason", by="hq_T"); ck("8 declare a WIRED gate", rc, 2)
        rc, txt = run(declare, td, "test_gate_b.sh", "WIRED", "real reason", by="hq_T"); ck("8 declare class WIRED", rc, 2)
        rc, txt = run(declare, td, "test_gate_zz.sh", "RULING", "real reason", by="hq_T"); ck("8 declare absent gate", rc, 2)
        os.environ.pop("S4E_SEAT", None)
        rc, txt = run(declare, td, "test_gate_b.sh", "RULING", "real reason"); ck("8 declare with no declarer", rc, 2)
        rc, txt = run(declare, td, "test_gate_b.sh", "RULING", "reads the live fleet", by="hq_T"); ck("8 declare accepted", rc, 0)
        rows, _, _ = load(td)
        ck("8 the reason is stored", [r for r in rows if r[0] == "test_gate_b.sh"][0][2], "reads the live fleet")
        # ARM 9 THE CEILING: a new unwired gate may not join TASK silently.
        _fx(td, G, ["bash scripts/test_gate_a.sh"],
            "# scratch\n%s 1\ntest_gate_a.sh\tWIRED\t-\t-\t-\ntest_gate_b.sh\tTASK\t-\t-\t-\ntest_gate_c.sh\tRULING\tbench\thq_T\t2026-09-06T00:00Z\n" % CEIL_KEY)
        rc, txt = run(check, td); ck("9 at the ceiling is clean", rc, 0)
        rc, txt = run(declare, td, "test_gate_c.sh", "TASK", "just moving it", by="hq_T"); ck("9 declare TASK over ceiling", rc, 2)
        ck("9 says the debt may only go down", "MAY ONLY GO DOWN" in txt, True)
        # ARM 10 SUM/DUPES/UNKNOWN CLASS: every name in exactly one bucket is the property one file buys.
        _fx(td, G, ["bash scripts/test_gate_a.sh"],
            "# scratch\n%s 2\ntest_gate_a.sh\tWIRED\t-\t-\t-\ntest_gate_b.sh\tTASK\t-\t-\t-\ntest_gate_b.sh\tTASK\t-\t-\t-\ntest_gate_c.sh\tNOPE\t-\t-\t-\n" % CEIL_KEY)
        rc, txt = run(check, td); ck("10 duplicate + unknown class", rc, 1)
        ck("10 names both faults", "LISTED TWICE" in txt and "UNKNOWN CLASS" in txt, True)
        # ARM 11 CANNOT MEASURE IS NOT CLEAN AND NOT A REFUSAL: a Makefile with no `test:` target must be rc=2.
        with open(os.path.join(td, "Makefile"), "w") as fh:
            fh.write("other:\n\t@true\n")
        rc, txt = run(check, td); ck("11 unmeasurable is rc=2", rc, 2)
        ck("11 says it examined nothing", "UNVERIFIED" in txt, True)
        # ARM 12 REACHABILITY IS EXECUTION, NOT MENTION: a gate named only in a comment, only in an echo, or
        # only inside a glob-census that READS the files is NOT reachable. This is the arm that keeps the
        # instrument from lying in the comfortable direction.
        _fx(td, G, ["bash scripts/test_gate_a.sh   # see also test_gate_b.sh",
                    "echo 'run test_gate_c.sh by hand'"], None)
        with open(os.path.join(td, "scripts", "test_gate_a.sh"), "w") as fh:
            fh.write("#!/usr/bin/env bash\nfor f in \"$HERE\"/test_gate_*.sh; do grep -q x \"$f\"; done\nexit 0\n")
        got = sorted(g for g in disk_gates(td) if g in reachable(td))
        ck("12 only the executed gate is reachable", got, ["test_gate_a.sh"])
        # ARM 13 A DECLARED POPULATION IS PART OF THE RECIPE. `make preflight` reads its 31 arms from
        # scripts/preflight_arms.txt, so `make -n preflight` names NO gate while running all of them -- and the
        # first draft of this instrument therefore filed 22 genuinely-run gates as debt. Expanding a recipe
        # stops answering the question you asked the moment a target reads its population from a file.
        _fx(td, G, ["bash scripts/test_gate_a.sh"], None)
        with open(os.path.join(td, "Makefile"), "a") as fh:
            fh.write("preflight:\n\t@while read -r arm; do bash $$arm; done < <(grep -E '^scripts/' \"$(ROOT)/scripts/pop_arms.txt\")\n")
        with open(os.path.join(td, "scripts", "pop_arms.txt"), "w") as fh:
            fh.write("# a declared population\nscripts/test_gate_b.sh\n")
        got = sorted(g for g in disk_gates(td) if g in reachable(td))
        ck("13 declared-population arm is reachable", got, ["test_gate_a.sh", "test_gate_b.sh"])
        # ARM 14 AND AN OPTIONAL TARGET THAT WILL NOT EXPAND IS NAMED, NEVER SILENTLY DROPPED: its gates leave
        # the wired set, and a refusal that cannot say WHY they left is loud and undiagnosable.
        with open(os.path.join(td, "Makefile"), "w") as fh:
            fh.write("test:\n\tbash scripts/test_gate_a.sh\n")
        skipped = []
        reachable(td, skipped=skipped)
        ck("14 unexpandable optional targets are named", sorted(skipped), ["preflight", "test-postoffice"])
    finally:
        shutil.rmtree(td, ignore_errors=True)
    print("--- %d arm(s) failed ---" % len(fails), file=out)
    return 1 if fails else 0


def main(argv=None):
    ap = argparse.ArgumentParser(description="the one population of gates, and the ratchet over it")
    ap.add_argument("cmd", choices=["check", "adopt", "declare", "reach", "selftest"])
    ap.add_argument("args", nargs="*")
    ap.add_argument("--root", default=None, help="SCRIP root (default: this script's parent)")
    ap.add_argument("--by", default=None, help="declarer for `declare` (default $S4E_SEAT)")
    ns = ap.parse_args(argv)
    root = ns.root or os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    if ns.cmd == "check":
        return check(root)
    if ns.cmd == "adopt":
        return adopt(root)
    if ns.cmd == "reach":
        # ⛔ HOISTED, AND IT WAS NOT AT FIRST. Written inline as the comprehension's condition, `reachable(root)`
        # -- two `make -n` runs and a closure over scripts/ -- was re-evaluated ONCE PER GATE, 279 times, and a
        # sub-second subcommand took ~4 minutes. It never returned a wrong answer, which is why it read as a
        # hang rather than a bug: an O(n) call hidden in a filter is invisible at every size but the real one.
        _reach = reachable(root)
        for g in sorted(g for g in disk_gates(root) if g in _reach):
            print(g)
        return 0
    if ns.cmd == "declare":
        if len(ns.args) < 3:
            print("usage: util_gate_wiring.py declare <gate.sh> RULING|TASK \"<reason>\"", file=sys.stderr)
            return 2
        return declare(root, ns.args[0], ns.args[1], " ".join(ns.args[2:]), by=ns.by)
    return selftest()


if __name__ == "__main__":
    sys.exit(main())
