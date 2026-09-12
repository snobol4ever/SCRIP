#!/usr/bin/env python3
"""util_logtalk_grade.py -- the grading engine behind test_prolog_logtalk_suite.sh.

Row prolog-logtalk-iso-3268-conformance-cases-have-no-runner (hq_R, 2026-09-12).

⛔⭐ WHAT THIS GRADES AGAINST, AND WHY IT IS NOT AN ORACLE DIFF. Every case in
corpus/packages/prolog/logtalk_iso carries its OWN expectation, taken from ISO/IEC 13211-1 and citing the
standard's section numbers in the comment above it. So this runner does not compare scrip to gprolog's
opinion or swipl's -- it compares scrip to the standard as the standard states it, which is a stronger
instrument than any oracle diff this repo owns, and it is why this row decides whether "100% of the
industry-standard language" can be said about Prolog at all.

THE BUCKETS ARE THE LEDGER'S, AND THE IDENTITY IS THE DELIVERABLE (ARCH-PROGRAM-LEDGER):
    PASS + FAIL + OUTSIDE + UNGRADABLE + UNGRADED + DEFERRED == the population globbed
⛔ A case this runner cannot set up, cannot express, or is told by its own `condition(...)` option to skip
is UNGRADED **and named with its reason** -- never dropped from the population and never counted as
passing. UNKNOWN is not ZERO. The population is printed on the board line and the identity is asserted
here, in code: if it does not hold the runner REFUSES rather than publishing four numbers that do not add
up, because four plausible independent counts with an invisible remainder is exactly the "never graded
business" the package lockdown exists to end.

⛔ THE VERDICT COMPARISON IS DONE IN PROLOG, NOT IN PYTHON TEXT. An expectation like
error(type_error(evaluable, foo/0)) is matched by UNIFYING the thrown ball against a '$lgt_want'/1 clause
inside the generated program. Comparing printed text instead would make quoting, variable naming and
operator spacing our problem, and each of those has its own way of being wrong -- _G0 vs _123 alone would
red every instantiation_error case in the suite.
"""
import os
import re
import shutil
import subprocess
import sys
import tempfile
import glob
from concurrent.futures import ThreadPoolExecutor

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import util_logtalk_extract as ex   # noqa: E402

# ⛔ Helpers the shim implements. A helper NOT in this set makes its case UNGRADED and names the helper, so
# the work owed reads off the board instead of hiding inside a red count. Keep it in sync with
# lib_logtalk_lgtunit.pl by reading that file (below), never by retyping the list -- two lists that can
# disagree about what is implemented is the same defect as two counts that can disagree about a population.
SHIM = os.path.join(os.path.dirname(os.path.abspath(__file__)), "lib_logtalk_lgtunit.pl")

# Directives in a file's database section that are Logtalk/dialect scaffolding, not Prolog the case needs.
DROP_DIRECTIVES = ("if", "elif", "else", "endif", "import", "use_module", "ensure_loaded",
                   "set_logtalk_flag", "encoding", "include")


# ⛔⭐ A FILE'S DATABASE IS NOT ALL PROLOG, AND ONE CLAUSE THAT IS NOT TAKES THE WHOLE FILE DOWN. The
# clauses before `:- object(` and the helper clauses inside it travel with EVERY case of that file, so a
# single helper written in Logtalk's own message-sending syntax (`^^Goal`, `Obj::Goal`) makes every one of
# that file's generated programs a parse error -- and the case then reports `nooutput`, which reads as the
# construct under test failing. Measured 2026-09-12 against the ceo's independent emulation: 40 files and
# 1486 of 3617 cases were graded this way, is_2 and numbers among them, and every one was a verdict on
# `cleanup :- ^^clean_text_input.` rather than on arithmetic. ⭐ THE SHAPE OF THE DEFECT IS THE POINT: the
# harness was wrong in a way that could only ever produce reds, so it never looked like a harness bug --
# a board of failures is what a young frontend is expected to print.
LOGTALK_ONLY_SYNTAX = re.compile(r"\^\^|::")


def _clause_head_name(clause):
    """The functor a database clause defines, or None -- `foo(X) :- ...`, `foo :- ...`, `'q'(X).`"""
    m = re.match(r"\s*('(?:[^']|'')*'|[a-z][A-Za-z0-9_]*)", clause)
    return m.group(1) if m else None


def split_db(db):
    """(plain-Prolog clauses, names dropped). A helper clause in Logtalk-only syntax cannot be expressed
    here, so it is DROPPED rather than emitted -- and its name is returned, because a case that calls it
    must become UNGRADED-and-named rather than a silent existence_error red."""
    keep, dropped = [], set()
    for cl in db:
        if LOGTALK_ONLY_SYNTAX.search(cl):
            n = _clause_head_name(cl)
            if n:
                dropped.add(n)
            continue
        keep.append(cl)
    return keep, dropped


def calls_dropped(plan, dropped):
    """The dropped helper this plan would call, or None. Checked against the goal AS FINISHED, so a
    setup(...)/cleanup(...) option folded into the goal is covered by the same test."""
    for n in sorted(dropped):
        if re.search(r"(?<![A-Za-z0-9_])%s(?![A-Za-z0-9_])" % re.escape(n.strip("'")), plan.goal or ""):
            return n
        if plan.condition and re.search(r"(?<![A-Za-z0-9_])%s(?![A-Za-z0-9_])" % re.escape(n.strip("'")), plan.condition):
            return n
    return None


def shim_helpers(path=SHIM):
    """The helper names lib_logtalk_lgtunit.pl actually defines, read from the file itself."""
    out = set()
    for m in re.finditer(r"^lgt_([a-z_]+)\(", open(path, encoding="utf-8").read(), re.M):
        out.add(m.group(1))
    for m in re.finditer(r"^lgt_([a-z_]+) :-", open(path, encoding="utf-8").read(), re.M):
        out.add(m.group(1))
    return out


def _braces_to_parens(s):
    """Logtalk's {Goal} escape -> plain (Goal). Quote-aware: a brace inside a quoted atom is data."""
    out = []
    i = 0
    n = len(s)
    while i < n:
        j = ex._skip_token(s, i)
        if j != i:
            out.append(s[i:j])
            i = j
            continue
        ch = s[i]
        out.append("(" if ch == "{" else (")" if ch == "}" else ch))
        i += 1
    return "".join(out)


def _rewrite_helpers(goal, supported):
    """^^name(Args) -> lgt_h(lgt_name(Args)). Returns (goal, unsupported_helper_or_None)."""
    out = []
    i = 0
    n = len(goal)
    while i < n:
        j = ex._skip_token(goal, i)
        if j != i:
            out.append(goal[i:j])
            i = j
            continue
        if goal.startswith("^^", i):
            m = re.match(r"\^\^([a-z_]+)", goal[i:])
            name = m.group(1)
            if name not in supported:
                return None, name
            k = i + m.end()
            if k < n and goal[k] == "(":
                e = ex._close_paren(goal[k + 1:])
                if e is None:
                    return None, name
                args = goal[k + 1:k + 1 + e]
                out.append("lgt_h(lgt_%s(%s))" % (name, args))
                i = k + 1 + e + 1
            else:
                out.append("lgt_h(lgt_%s)" % name)
                i = k
            continue
        out.append(goal[i])
        i += 1
    return "".join(out), None


def _split_option(options):
    """Parse a test/3 option list into {condition,setup,cleanup}; unknown options are reported."""
    got = {}
    unknown = []
    if not options:
        return got, unknown
    t = options.strip()
    if not (t.startswith("[") and t.endswith("]")):
        return got, [t]
    for item in ex.split_args(t[1:-1]):
        item = item.strip()
        if not item:
            continue
        m = re.match(r"([a-z_]+)\s*\(", item)
        if not m:
            unknown.append(item)
            continue
        name = m.group(1)
        e = ex._close_paren(item[m.end():])
        if e is None:
            unknown.append(item)
            continue
        arg = item[m.end():m.end() + e]
        if name in ("condition", "setup", "cleanup"):
            got[name] = arg
        else:
            unknown.append(name)
    return got, unknown


class Plan(object):
    """How one case will be graded, or why it will not be."""
    __slots__ = ("case", "want", "goal", "balls", "weaker", "skip_reason", "condition")

    def __init__(self, case, want=None, goal=None, balls=(), weaker=None, skip_reason=None):
        self.case = case
        self.want = want
        self.goal = goal
        self.balls = list(balls)
        self.weaker = weaker
        self.skip_reason = skip_reason
        self.condition = None


def plan_case(c, supported):
    """Turn one extracted Case into a Plan: either an executable grading, or a NAMED refusal to grade."""
    if c.kind == "test" and not c.expect:
        return Plan(c, skip_reason="lgtunit test/1 placeholder: the suite declares this test and does not implement it")
    goal, bad = _rewrite_helpers(c.goal, supported)
    if bad is not None:
        return Plan(c, skip_reason="lgtunit helper ^^%s is not implemented by lib_logtalk_lgtunit.pl" % bad)
    goal = _braces_to_parens(goal)
    opts, unknown = _split_option(c.options)
    if unknown:
        return Plan(c, skip_reason="test/3 option(s) not understood: %s" % ",".join(unknown))
    # ⛔ THE LEGACY THREE ARE THE SAME THREE EXPECTATIONS SPELLED DIFFERENTLY -- succeeds == true,
    # fails == false, throws(Name,Ball) == the ball. They are normalised into the same expectation
    # vocabulary here rather than given their own grading path, because two paths that mean the same thing
    # are two places for the meaning to drift, and 349 of the 3617 cases (10%) ride on this one.
    if c.kind == "succeeds":
        return _finish(c, "succ", goal, [], None, opts)
    if c.kind == "fails":
        return _finish(c, "fail", goal, [], None, opts)
    if True:
        e = (c.expect or "").strip()
        m = re.match(r"([a-z_]+)\s*(\()?", e)
        head = m.group(1) if m else ""
        arg = None
        if m and m.group(2):
            k = ex._close_paren(e[m.end():])
            if k is None:
                return Plan(c, skip_reason="expectation does not parse: %s" % e[:60])
            arg = e[m.end():m.end() + k]
        want, tail, balls, weaker = None, "", [], None
        if head == "true" and arg is None:
            want = "succ"
        elif head == "true":
            want, tail = "succ", ", (%s)" % arg
        elif head in ("false", "fail") and arg is None:
            want = "fail"
        elif head in ("false", "fail"):
            want, tail = "succ", ", \\+ (%s)" % arg
        elif head == "deterministic":
            want, weaker = "succ", "deterministic"
            tail = ", (%s)" % arg if arg else ""
        elif head == "exists":
            want = "succ"
            tail = ", (%s)" % arg if arg else ""
        elif head == "all":
            want, goal = "succ", "\\+ ( (%s), \\+ (%s) )" % (goal, arg)
        elif head == "variant" and arg is not None:
            a = ex.split_args(arg)
            if len(a) != 2:
                return Plan(c, skip_reason="variant/%d -- expected variant/2" % len(a))
            want, tail = "succ", ", lgt_variant(%s, %s)" % (a[0], a[1])
        elif head == "subsumes" and arg is not None:
            a = ex.split_args(arg)
            if len(a) != 2:
                return Plan(c, skip_reason="subsumes/%d -- expected subsumes/2" % len(a))
            want, tail = "succ", ", lgt_subsumes(%s, %s)" % (a[0], a[1])
        elif head == "error" and arg is not None:
            want, balls = "ball", ["error(%s, _)" % arg]
        elif head == "error_ball" and arg is not None:
            want, balls = "ball", [arg]
        elif head == "errors" and arg is not None:
            t = arg.strip()
            if not (t.startswith("[") and t.endswith("]")):
                return Plan(c, skip_reason="errors(...) argument is not a list: %s" % t[:50])
            want = "ball"
            balls = ["error(%s, _)" % x for x in ex.split_args(t[1:-1]) if x.strip()]
        elif head == "ball" and arg is not None:
            want, balls = "ball", [arg]
        elif head == "balls" and arg is not None:
            t = arg.strip()
            want = "ball"
            balls = [x for x in ex.split_args(t[1:-1]) if x.strip()] if t.startswith("[") else [t]
        else:
            return Plan(c, skip_reason="expectation form not implemented: %s" % e[:60])
        if head != "all":
            goal = goal + tail
        return _finish(c, want, goal, balls, weaker, opts)


def _finish(c, want, goal, balls, weaker, opts):
    if opts.get("setup"):
        goal = "lgt_h((%s)), %s" % (opts["setup"], goal)
    if opts.get("cleanup"):
        goal = "%s, lgt_h((%s))" % (goal, opts["cleanup"])
    p = Plan(c, want=want, goal=goal, balls=balls, weaker=weaker)
    p.condition = opts.get("condition")
    return p


def program_text(plan, db, shim_src, loaded=()):
    c = plan.case
    parts = [shim_src]
    # ⛔ The tester-loaded plain-Prolog files go in VERBATIM, ahead of the file's own database and without
    # the directive filter below: their :- if/elif/else/endif guards ARE the subject of one group, and
    # dropping a guard while keeping both of its branches defines exactly the predicates the case expects
    # to be undefined -- a silent semantic edit that turns 31 correct failures into 31 wrong successes.
    for cl in loaded:
        parts.append(cl + ".")
    for cl in db:
        if cl.startswith(":-"):
            m = re.match(r":-\s*([a-z_]+)", cl)
            if m and m.group(1) in DROP_DIRECTIVES:
                continue
        parts.append(cl + ".")
    parts.append("'$lgt_say'(V) :- write(user_output, '@V '), write(user_output, V), nl(user_output).")
    parts.append("'$lgt_say2'(V, B) :- write(user_output, '@V '), write(user_output, V), write(user_output, ' | '), "
                 "catch(writeq(user_output, B), _, write(user_output, '<unprintable>')), nl(user_output).")
    # ⛔ ALWAYS at least one '$lgt_want'/1 clause: a call to an undefined predicate raises existence_error,
    # which would be caught by the enclosing catch and reported as a ball the case never threw.
    parts.append("'$lgt_want'('$lgt_no_ball_expected_here').")
    for b in plan.balls:
        parts.append("'$lgt_want'(%s)." % b)
    parts.append("'$lgt_cond' :- %s." % (plan.condition if plan.condition else "true"))
    parts.append("'$lgt_case' :- %s." % plan.goal)
    parts.append(
        ":- catch( ( catch('$lgt_cond', _, fail) -> "
        "catch( ( '$lgt_case' -> '$lgt_say'(succ) ; '$lgt_say'(fail) ), B, "
        "( B = '$lgt_harness'(_) -> '$lgt_say2'(harness, B) "
        "; '$lgt_want'(B) -> '$lgt_say'(ballmatch) "
        "; '$lgt_say2'(ballother, B) ) ) "
        "; '$lgt_say'(skipped) ), B2, '$lgt_say2'(toplevel, B2) ).")
    return "\n".join(parts) + "\n"


def run_one(plan, db, shim_src, scrip, mode, workroot, srcdir, timeout=10, loaded=()):
    """Run one planned case in one mode. Returns (outcome, detail)."""
    d = tempfile.mkdtemp(prefix="lgtcase.", dir=workroot)
    try:
        for f in os.listdir(srcdir):
            s = os.path.join(srcdir, f)
            if os.path.isfile(s) and f not in ("tests.lgt", "tester.lgt"):
                try:
                    shutil.copy2(s, os.path.join(d, f))
                except OSError:
                    pass
        prog = os.path.join(d, "case.pl")
        open(prog, "w", encoding="utf-8").write(program_text(plan, db, shim_src, loaded))
        try:
            if mode == "m3":
                r = subprocess.run([scrip, prog], capture_output=True, text=True, errors="replace", timeout=timeout,
                                   stdin=subprocess.DEVNULL, cwd=d)
            else:
                s_out = os.path.join(d, "case.s")
                b_out = os.path.join(d, "case.bin")
                cp = subprocess.run([scrip, "--compile", "-o", s_out, prog], capture_output=True, text=True, errors="replace",
                                    timeout=timeout, stdin=subprocess.DEVNULL, cwd=d)
                if cp.returncode != 0:
                    return "nobuild", (cp.stderr or cp.stdout).strip()[:160]
                rt = os.path.join(os.path.dirname(os.path.abspath(scrip)), "out")
                g = subprocess.run(["gcc", "-m64", "-no-pie", s_out, "-o", b_out, "-L", rt, "-lscrip_rt",
                                    "-Wl,-rpath," + rt, "-lm"], capture_output=True, text=True, errors="replace", timeout=120, cwd=d)
                if g.returncode != 0:
                    return "nolink", (g.stderr or "").strip()[:160]
                r = subprocess.run([b_out], capture_output=True, text=True, errors="replace", timeout=timeout,
                                   stdin=subprocess.DEVNULL, cwd=d)
        except subprocess.TimeoutExpired:
            return "timeout", ""
        for line in r.stdout.splitlines():
            if line.startswith("@V "):
                rest = line[3:]
                if " | " in rest:
                    a, b = rest.split(" | ", 1)
                    return a.strip(), b.strip()[:160]
                return rest.strip(), ""
        # ⛔ NO VERDICT LINE IS NOT A ZERO AND IT IS NOT A PASS. It is the compiler refusing the program
        # (an ISO builtin this engine does not have yet), or the program dying. Both are real reds for the
        # case -- but they are reported with the reason attached, because "no output" read as FAIL with no
        # explanation is how a whole missing builtin family hides inside a percentage.
        err = (r.stderr or r.stdout).strip().splitlines()
        return "nooutput", (err[0][:160] if err else "rc=%d, no @V line" % r.returncode)
    finally:
        shutil.rmtree(d, ignore_errors=True)


def grade(root, scrip, modes, jobs=8, limit=None, only_group=None):
    # ⛔ ABSOLUTE, AND CHECKED HERE. Every case runs with cwd set to its own scratch directory, so a
    # relative binary path ("./scrip", the obvious thing to type) resolves against the scratch dir and is
    # not there. Measured 2026-09-12: it does not raise anything a reader would notice -- every case comes
    # back as a harness refusal and the board prints a complete, plausible, entirely UNGRADED table.
    scrip = os.path.abspath(scrip)
    if not os.access(scrip, os.X_OK):
        return None, [(scrip, "no executable compiler at this path -- a missing binary prints a full, "
                              "plausible, entirely false board rather than nothing")]
    files, bad = ex.parse_suite(root)
    if bad:
        return None, bad
    supported = shim_helpers()
    shim_src = open(SHIM, encoding="utf-8").read()
    work = []
    # ⛔ THE DATABASE IS SPLIT BEFORE ANY CASE IS PLANNED, per file: the plain-Prolog clauses travel with
    # every case, the Logtalk-only ones are dropped AND NAMED, and a case that would have called one is
    # UNGRADED with that name instead of being graded against a program that does not parse.
    dbs = {}
    for fc in files:
        if only_group and fc.group != only_group:
            continue
        clean, dropped = split_db(fc.db)
        dbs[fc.path] = clean
        for c in fc.cases:
            p = plan_case(c, supported)
            if p.skip_reason is None and dropped:
                n = calls_dropped(p, dropped)
                if n is not None:
                    p.skip_reason = ("logtalk-only-helper-clause: this file defines %s in Logtalk "
                                     "message-sending syntax, which has no plain-Prolog form here" % n)
            work.append((fc, p))
    if limit:
        work = work[:limit]
    workroot = tempfile.mkdtemp(prefix="lgtsuite.")
    results = {}
    try:
        for mode in modes:
            todo = [(i, fc, p) for i, (fc, p) in enumerate(work) if p.skip_reason is None]
            def job(t):
                # ⛔ ONE CASE MUST NEVER TAKE THE RUN DOWN. A harness exception here (the unicode family
                # writes UTF-16/32 bytes to stdout, which killed the first full run on a decode error) is
                # the harness failing, not the case -- it is reported as such and the other 3616 still get
                # measured. A runner that dies at case 900 publishes nothing at all.
                i, fc, p = t
                try:
                    return i, run_one(p, dbs[fc.path], shim_src, scrip, mode, workroot, os.path.dirname(fc.path),
                                      loaded=fc.loaded or ())
                except Exception as e:                      # noqa: BLE001 -- deliberately broad, see above
                    return i, ("harness", "%s: %s" % (type(e).__name__, e))
            with ThreadPoolExecutor(max_workers=jobs) as pool:
                for i, res in pool.map(job, todo):
                    results[(i, mode)] = res
    finally:
        shutil.rmtree(workroot, ignore_errors=True)
    return (work, results), []


def verdict(plan, outcome):
    """PASS / FAIL / UNGRADED for one (plan, outcome), with the reason when it is not graded."""
    if outcome in ("harness", "toplevel"):
        return "UNGRADED", "harness could not set the case up"
    if outcome == "skipped":
        return "UNGRADED", "the case's own condition(...) option is false on this system"
    if plan.want == "succ":
        return ("PASS", "") if outcome == "succ" else ("FAIL", outcome)
    if plan.want == "fail":
        return ("PASS", "") if outcome == "fail" else ("FAIL", outcome)
    return ("PASS", "") if outcome == "ballmatch" else ("FAIL", outcome)


def _fmt_reason(plan, why):
    return plan.skip_reason if plan.skip_reason else why


def main(argv):
    import argparse
    ap = argparse.ArgumentParser(description="grade corpus/packages/prolog/logtalk_iso against ISO/IEC 13211-1")
    ap.add_argument("--suite", required=True)
    ap.add_argument("--scrip", required=True)
    ap.add_argument("--modes", default="m3")
    ap.add_argument("--jobs", type=int, default=12)
    ap.add_argument("--group", default=None, help="grade one directory only (a development aid, never a board)")
    ap.add_argument("--name-reds", action="store_true")
    args = ap.parse_args(argv[1:])
    modes = [m for m in args.modes.split(",") if m]
    out, bad = grade(args.suite, args.scrip, modes, jobs=args.jobs, only_group=args.group)
    if bad:
        # ⛔ THE DONE-WHEN'S REFUSAL: a tests.lgt that cannot be parsed is NAMED and the run publishes
        # nothing. A partial population printed as if it were whole is the one failure this row exists to
        # prevent -- see util_logtalk_extract.py's header for the 31 cases that vanished in silence.
        sys.stderr.write("⛔ REFUSED(2) [logtalk_iso]: %d file(s) could not be parsed; no board is published\n" % len(bad))
        for p, why in bad:
            sys.stderr.write("    %s: %s\n" % (p, why))
        return 2
    work, results = out
    # ⛔⭐ A DEVELOPMENT AID THAT GRADED NOTHING MUST REFUSE, NOT PRINT ZEROS. Measured 2026-09-12: `--group
    # predicates/sub_atom_5` (the path, where the group is the basename) matched no file and this runner
    # printed a complete board reading population=0, identity 0 == 0 ✓, AND per case 0/0 -- the success
    # shape over an empty measurement, which is the one thing every gate in this tree is forbidden to do.
    if args.group and not work:
        groups = sorted({fc.group for fc in ex.parse_suite(args.suite)[0]})
        sys.stderr.write("⛔ REFUSED(2) [logtalk_iso]: --group %r matched no file -- the group is the "
                         "DIRECTORY BASENAME (sub_atom_5), never its path. Known groups: %s\n"
                         % (args.group, ", ".join(groups)))
        return 2
    pop = len(work)
    board = {}
    named = []
    reasons = {}          # keyed (mode, reason) -- see the print below
    weaker = sum(1 for _fc, p in work if p.weaker)
    for mode in modes:
        b = {"PASS": 0, "FAIL": 0, "OUTSIDE": 0, "UNGRADABLE": 0, "UNGRADED": 0, "DEFERRED": 0}
        for i, (fc, p) in enumerate(work):
            if p.skip_reason is not None:
                b["UNGRADED"] += 1
                k = (mode, p.skip_reason.split(":")[0])
                reasons[k] = reasons.get(k, 0) + 1
                continue
            o, d = results[(i, mode)]
            v, why = verdict(p, o)
            b[v] += 1
            if v == "UNGRADED":
                reasons[(mode, why)] = reasons.get((mode, why), 0) + 1
            elif v == "FAIL":
                named.append("%s:%s:%s:%s%s" % (fc.group, p.case.name, mode, o, (" " + d) if d else ""))
        # ⛔ THE IDENTITY IS ASSERTED, NOT ASSUMED. Four plausible counts with an invisible remainder is
        # the exact defect the package lockdown exists to end, so a total that does not reconcile REFUSES.
        tot = sum(b.values())
        if tot != pop:
            sys.stderr.write("⛔ REFUSED(2) [logtalk_iso]: %s buckets sum to %d but the globbed population is %d "
                             "-- a case is in no bucket and would be invisible in every number\n" % (mode, tot, pop))
            return 2
        board[mode] = b
    print("LOGTALK_ISO_BOARD population=%d %s" % (pop, " ".join(
        "%s_pass=%d %s_fail=%d %s_ungraded=%d" % (m, board[m]["PASS"], m, board[m]["FAIL"], m, board[m]["UNGRADED"])
        for m in modes)))
    for m in modes:
        b = board[m]
        print("  identity %s: PASS %d + FAIL %d + OUTSIDE %d + UNGRADABLE %d + UNGRADED %d + DEFERRED %d == %d ✓"
              % (m, b["PASS"], b["FAIL"], b["OUTSIDE"], b["UNGRADABLE"], b["UNGRADED"], b["DEFERRED"], pop))
    print("  criterion: each case's OWN expectation as ISO/IEC 13211-1 states it (the suite cites the section "
          "numbers), matched by UNIFICATION inside the engine -- not an oracle diff, and never a text compare")
    if weaker:
        print("  ⚠ %d case(s) expect `deterministic` and are graded on the WEAKER criterion SUCCEEDS: no "
              "choicepoint check is available in this engine, so tightening it can only move the number DOWN" % weaker)
    # ⛔ PER MODE, NOT SUMMED ACROSS MODES. A first cut pooled them and printed 132 skipped conditions for
    # a suite that has 66 -- a table whose every row is exactly double the truth, on a board whose whole
    # point is that the parts reconcile with the whole. It read as plausible because nothing else on the
    # line moved: the identity still balanced, because the identity is computed per mode and the TABLE was
    # the only thing pooled.
    for m in modes:
        rs = {k[1]: v for k, v in reasons.items() if k[0] == m}
        print("  UNGRADED %s, by reason -- work owed, named, never dropped from the population and never a pass:" % m)
        for k in sorted(rs, key=lambda x: -rs[x]):
            print("    %4d  %s" % (rs[k], k))
    if args.name_reds:
        for x in named[:400]:
            print("    RED " + x)
    # ⛔⭐ THE AND PER CASE, AND IT IS NOT min(m3, m4) (CEO-372; coo 2026-09-12 on this very board). m3 and
    # m4 both read 1218 PASS and the SETS ARE NOT THE SAME -- m4 carries one more UNGRADED and one fewer
    # FAIL, so they differ by at least one case. Two equal counts over different sets are not a number a
    # suite row can state, and the only honest answer is computed PER CASE: a case passes this suite when
    # it passes in EVERY mode graded. Printed beside the per-mode counts, never instead of them.
    both = 0
    for i, (fc, p) in enumerate(work):
        if p.skip_reason is not None:
            continue
        if all(verdict(p, results[(i, m)][0])[0] == "PASS" for m in modes):
            both += 1
    print("  AND per case (passes in EVERY mode graded, the number a suite row states): %d/%d" % (both, pop))
    if len(modes) > 1 and any(board[m]["PASS"] != both for m in modes):
        print("    ⛔ and it is NOT either per-mode count: %s -- equal counts over different sets"
              % ", ".join("%s=%d" % (m, board[m]["PASS"]) for m in modes))
    # ⛔⭐ ONE ROW PER CASE PER MODE INTO THE PROGRESS DATABASE (CEO-331). This runner grades with its own
    # loop, so nothing records it automatically and the coverage reader sees the suite as MISSING -- a flip
    # the progress table cannot see is not paid. ⛔ THE PROGRAM KEY IS group:name, never the goal text:
    # case names repeat across directories (iso_cut_0_01 style names are unique per file, not globally),
    # and a colliding key silently shrinks the denominator. UNGRADED is a real outcome in this table's own
    # vocabulary and is recorded as itself -- never dropped, and never collapsed into FAIL.
    rows = os.path.join(tempfile.gettempdir(), "logtalk_progress_rows.tsv")
    with open(rows, "w") as pf:
        for i, (fc, p) in enumerate(work):
            for m in modes:
                if p.skip_reason is not None:
                    v, note = "UNGRADED", p.skip_reason.split(":")[0][:60]
                else:
                    v, why = verdict(p, results[(i, m)][0])
                    note = "iso-13211-1-case-expectation" if v == "PASS" else (why or v)[:60]
                pf.write("package\tlogtalk\tprolog\t%s:%s\t%s\t%s\t0\t%s\n"
                         % (fc.group, p.case.name, m, v, note))
    print("PROGRESS_ROWS_TSV %s" % rows)
    print("BOARD_FOR_SHELL %d %d %s" % (pop, both, " ".join("%d %d" % (board[m]["PASS"], board[m]["FAIL"]) for m in modes)))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
