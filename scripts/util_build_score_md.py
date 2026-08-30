#!/usr/bin/env python3
# util_build_score_md.py -- computed generator for the SCORE.md "STANDARDIZED DISPLAY" grid (row
# score-md-master-driven-display, ceo's all-hands-consolidation assignment to seat08, 2026-08-30).
#
# WHY THIS EXISTS: SCORE.md is Lon's "central location for the current score" -- but its own text
# already names its worst defect: 15 of 17 rows carried an unpinned "s283h tree" label that names no
# commit anyone can check out (RULES.md STALE-ORIENTATION / HANDOFF-COMPLETE-NEEDS-A-PUSH, applied to
# a scoreboard). A hand-typed grid re-introduces that defect every time someone edits it by hand.
#
# WHAT THIS DOES, AND DOES NOT, DO. It does NOT reimplement any language's grading logic -- that would
# be a second, drifting copy of logic that already exists and is owned per-language (the exact class
# RULES.md's FACT RULES spend a whole chapter warning about). It (1) reads each language's
# corpus/tests/<lang>/ALL.csv master for entry/xfail counts -- genuinely master-driven, no duplicated
# logic, MASTER PENDING (never a fabricated number) where no master exists yet; (2) invokes each
# language's own existing, authoritative floor/smoke gate script and parses its OWN printed verdict
# line with a NAMED per-language pattern -- a pattern miss is UNPROVEN, never a guess; (3) stamps every
# run with real, checkable per-repo commit hashes (+DIRTY), same discipline as lib_gate.sh's gate_stamp,
# so a reader can `git checkout` exactly what produced a given grid.
import csv, os, re, subprocess, sys, time

S4E = os.environ.get("S4E_HOME") or os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
SCRIP_ROOT = os.path.join(S4E, "SCRIP")
CORPUS_ROOT = os.path.join(S4E, "corpus")
GITHUB_ROOT = os.path.join(S4E, ".github")
GATE_TIMEOUT = int(os.environ.get("SCORE_GATE_TIMEOUT", "150"))

LANGS = ["snobol4", "icon", "prolog", "raku", "pascal", "snocone", "rebus"]

# ONE AUTHORITY per language for how to invoke its floor/smoke gate and how to read its verdict line.
# Each entry's script is unmodified and unowned by this file -- this table only reads what it already
# prints. A script's output format changing breaks its `pattern` match LOUDLY (UNPROVEN), never silently.
GATES = {
    "snobol4": {"script": "test_corpus_snobol4.sh", "args": [],
        "pattern": re.compile(r"GATE (OK|FAIL): m3 PASS=(\d+) FAIL=(\d+).*?m4 PASS=(\d+) FAIL=(\d+) SKIP=(\d+)", re.S),
        "format": lambda m: "m3 %s/%s · m4 %s/%s SKIP=%s" % (m.group(2), m.group(3), m.group(4), m.group(5), m.group(6))},
    "icon": {"script": "test_icon_rung_suite.sh", "args": ["--mode", "interp"],
        "pattern": re.compile(r"--- Icon \(interp\): PASS=(\d+) FAIL=(\d+)(?: BADEXIT=(\d+))? XFAIL=(\d+)(?: REFUSED=(\d+))?(?: MISSING=(\d+))? TOTAL=(\d+)"),
        "format": lambda m: "interp PASS=%s FAIL=%s XFAIL=%s TOTAL=%s" % (m.group(1), m.group(2), m.group(4), m.group(7))},
    "prolog": {"script": "test_smoke_prolog.sh", "args": [],
        "pattern": re.compile(r"mode-2 \(--run\):\s+PASS=(\d+) FAIL=(\d+)\s*/\s*(\d+)"),
        "format": lambda m: "m2 PASS=%s FAIL=%s / %s (HARD GATE)" % (m.group(1), m.group(2), m.group(3))},
    "raku": {"script": "test_smoke_raku.sh", "args": [],
        "pattern": re.compile(r"mode-3 \(--run\):\s+PASS=(\d+) FAIL=(\d+) REFUSED=(\d+)\s*/\s*(\d+)"),
        "format": lambda m: "m3 PASS=%s FAIL=%s REFUSED=%s / %s" % (m.group(1), m.group(2), m.group(3), m.group(4))},
    "pascal": {"script": "test_gate_pascal_m3.sh", "args": [],
        "pattern": re.compile(r"M3: PASS=(\d+) FAIL=(\d+) NOREF=(\d+) XFAIL=(\d+)"),
        "format": lambda m: "m3 PASS=%s FAIL=%s NOREF=%s XFAIL=%s" % (m.group(1), m.group(2), m.group(3), m.group(4))},
    "snocone": {"script": "test_smoke_snocone.sh", "args": [],
        "pattern": re.compile(r"PASS=(\d+) FAIL=(\d+)"),
        "format": lambda m: "PASS=%s FAIL=%s" % (m.group(1), m.group(2))},
    "rebus": {"script": "test_smoke_rebus.sh", "args": [],
        "pattern": re.compile(r"PASS=(\d+) FAIL=(\d+)"),
        "format": lambda m: "PASS=%s FAIL=%s" % (m.group(1), m.group(2))},
}


def repo_stamp(path):
    if not os.path.isdir(os.path.join(path, ".git")):
        return "unknown"
    try:
        h = subprocess.run(["git", "-C", path, "rev-parse", "--short", "HEAD"],
                            capture_output=True, text=True, timeout=10).stdout.strip()
        dirty = subprocess.run(["git", "-C", path, "status", "--porcelain"],
                                capture_output=True, text=True, timeout=10).stdout.strip()
    except Exception:
        return "unknown"
    if not h:
        return "unknown"
    return h + ("-DIRTY" if dirty else "")


def master_info(lang):
    csv_path = os.path.join(CORPUS_ROOT, "tests", lang, "ALL.csv")
    if not os.path.isfile(csv_path):
        return None
    with open(csv_path, newline="") as f:
        rows = list(csv.DictReader(f))
    xfail = 0
    for r in rows:
        v = (r.get("xfail") or "").strip().lower()
        if v and v not in ("0", "false", "no"):
            xfail += 1
    return {"entries": len(rows), "xfail": xfail}


def run_gate(lang, fake_missing=False):
    g = GATES.get(lang)
    if not g:
        return "UNPROVEN(2): no gate wired in this generator for %r" % lang
    script_path = os.path.join(SCRIP_ROOT, "scripts", g["script"])
    if fake_missing or not os.path.isfile(script_path):
        return "UNPROVEN(2): gate script missing at scripts/%s" % g["script"]
    cmd = ["bash", os.path.join("scripts", g["script"])] + g["args"]
    try:
        p = subprocess.run(cmd, cwd=SCRIP_ROOT, stdin=subprocess.DEVNULL,
                            capture_output=True, text=True, timeout=GATE_TIMEOUT)
    except subprocess.TimeoutExpired:
        return "UNPROVEN(2): gate timed out after %ss -- an rc is not a measurement of time, this is NOT a hang/pass verdict, just unmeasured" % GATE_TIMEOUT
    out = (p.stdout or "") + "\n" + (p.stderr or "")
    m = g["pattern"].search(out)
    if not m:
        # Surface the gate's OWN stated reason when it gave one (a REFUSES/UNPROVEN line is more useful
        # than a generic "didn't match" -- this is exactly what caught the live corpus/demos vs corpus/demo
        # path mismatch during this row's own build) -- never fabricate a number either way.
        first_line = next((ln.strip() for ln in out.splitlines() if ln.strip()), "")
        reason = (": %s" % first_line) if first_line else ""
        return "UNPROVEN(2): gate ran (rc=%d), output did not match the expected pattern%s" % (p.returncode, reason)
    return g["format"](m)


def build_grid(langs):
    lines = ["| Language | Master suite (`ALL.csv`) | Floor/smoke gate |", "|---|---|---|"]
    for lang in langs:
        mi = master_info(lang)
        master_col = "MASTER PENDING" if mi is None else ("%d entries" % mi["entries"]) + (", %d xfail" % mi["xfail"] if mi["xfail"] else "")
        gate_col = run_gate(lang)
        lines.append("| %s | %s | %s |" % (lang, master_col, gate_col))
    stamp = "tree: SCRIP=%s corpus=%s .github=%s  generated %s" % (
        repo_stamp(SCRIP_ROOT), repo_stamp(CORPUS_ROOT), repo_stamp(GITHUB_ROOT),
        time.strftime("%Y-%m-%dT%H:%MZ", time.gmtime()))
    return "\n".join(lines) + "\n\n_%s_\n" % stamp


def selftest():
    # RULES.md TWO-PART PROOF, applied to this generator itself: exercise BOTH directions before
    # trusting a single row of its own output -- prove it can say "not measured" (UNPROVEN, on a
    # deliberately-broken gate) AND that it can say "measured" (a real PASS/FAIL, on a real gate).
    ok = True
    # ⛔ The positive-control arm below deliberately does NOT use snobol4: it is picked to be the smallest,
    # fastest gate this table knows (a smoke test, seconds not minutes) precisely so this generator's OWN
    # mechanism is proven independent of any ONE language's gate happening to be healthy this week. Coupling
    # a tool's self-test to a specific language's external gate health is an accidental dependency, not a
    # deliberate one -- caught live during this row's own build when test_corpus_snobol4.sh started
    # refusing (a real, separately-flagged corpus/demos path defect, hq_C `snobol4-floor-gate-refuses-demos-
    # vs-demo-path-mismatch`) and took this selftest down with it for a reason that had nothing to do with
    # this generator.
    missing = run_gate("rebus", fake_missing=True)
    if not missing.startswith("UNPROVEN(2)"):
        print("SELFTEST FAIL: a missing gate script did not refuse loudly -- got: %r" % missing)
        ok = False
    else:
        print("SELFTEST: missing-gate arm correctly UNPROVEN -- %s" % missing)
    real = run_gate("rebus")
    if real.startswith("UNPROVEN"):
        print("SELFTEST FAIL: the real rebus gate could not be measured at all -- got: %r" % real)
        ok = False
    else:
        print("SELFTEST: real rebus gate produced a measured result -- %s" % real)
    unwired = run_gate("no-such-language")
    if "no gate wired" not in unwired:
        print("SELFTEST FAIL: an unwired language did not refuse cleanly -- got: %r" % unwired)
        ok = False
    else:
        print("SELFTEST: unwired-language arm correctly refused -- %s" % unwired)
    print("SELFTEST %s" % ("PASS" if ok else "FAIL"))
    return 0 if ok else 1


def main():
    argv = sys.argv[1:]
    if "--selftest" in argv:
        return selftest()
    only = None
    for a in argv:
        if a.startswith("--lang="):
            only = a.split("=", 1)[1]
    langs = [only] if only else LANGS
    if only and only not in GATES:
        sys.stderr.write("REFUSED: unknown language %r. Known: %s\n" % (only, ", ".join(LANGS)))
        return 2
    print(build_grid(langs))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
