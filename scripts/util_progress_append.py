#!/usr/bin/env python3
"""util_progress_append.py -- THE ONE WRITER OF THE PROGRESS DATABASE (Lon 2026-09-06 12:1x, in-chat to ceo, verbatim:
"So are you going to get your act together and enhance your database to allow for time based queries? Get it fixed.";
GOAL-CEO CEO-331; the RULE is /home/resources/progress/README.md, CEO-319: every suite run by any session APPENDS its
per-program rows in the same sitting it rewrites its SCORE.md cell -- a run that leaves the table untouched is a defect
of that run).

THE TABLE: /home/resources/progress/results.tsv, append-only, one row per (program, mode, run):
    ts_utc  scrip  corpus  measurer  class  suite  lang  program  mode  outcome  secs  note
ts_utc is the RUN's own wall clock (UTC, second precision, no zone suffix -- the shape the replay rows already carry);
scrip/corpus are the short hashes of the trees graded; measurer is the seat identity derived from the root path by the
ONE map (util_score_row.derive_measurer -- never a fourth copy of the map); class is master | package | benchmark;
suite is the SUITES.tsv key for packages, <lang>-master for masters, <lang>-bench for benchmarks; outcome is one of
OUTCOMES, never a count; secs is the program's own seconds when the runner knows them, else 0; note is free text
(xfail marker, benchmark rates) and may be empty.

THE CONTROL ARMS: S4E_PROGRESS_DB=<path> redirects the table (gates use a scratch file; the live table is never
touched by a gate); S4E_PROGRESS_OFF=1 records nothing and SAYS SO on stderr. There is no silent path: an unwritable
table is a loud refusal (ProgressUnwritable -> rc=2 from the CLI), never a swallowed exception.

LIBRARY:  from util_progress_append import append_rows; append_rows([{class,suite,lang,program,mode,outcome[,secs][,note]}, ...])
CLI:      util_progress_append.py append --class C --suite S --lang L --program P --mode M --outcome O [--secs N] [--note T]
          util_progress_append.py rows-tsv FILE            # bulk: class suite lang program mode outcome [secs [note]] per line
          util_progress_append.py results-tsv --suite S --lang L FILE   # scorecard_snobol4.sh shape: suite program m3 m4 t3 t4 note
          util_progress_append.py triangulation --lang L FILE           # bench_triangulate_* shape: kernel engine a1 a2 ratio verdict ...
rc 0 = rows written (count printed) or S4E_PROGRESS_OFF · rc 2 = refused (unwritable table, malformed row, unreadable input)."""
import os, sys, csv, io, time, fcntl, subprocess, re, collections
COLUMNS = ["ts_utc", "scrip", "corpus", "measurer", "class", "suite", "lang", "program", "mode", "outcome", "secs", "note"]
CLASSES = ("master", "package", "benchmark")
OUTCOMES = ("PASS", "FAIL", "CRASH", "HANG", "SKIP", "REFUSE", "UNGRADED", "UNPROVEN", "MISSING", "REJECT", "XFAIL", "XPASS")
DB_DEFAULT = "/home/resources/progress/results.tsv"
HERE = os.path.dirname(os.path.abspath(__file__))
S4E = os.environ.get("S4E_HOME") or os.path.abspath(os.path.join(HERE, "..", ".."))


class ProgressUnwritable(Exception):
    pass


def db_path():
    return os.environ.get("S4E_PROGRESS_DB") or DB_DEFAULT


def recording_off():
    return os.environ.get("S4E_PROGRESS_OFF", "") == "1"


def _git_short(repo):
    try:
        out = subprocess.run(["git", "-C", repo, "rev-parse", "--short", "HEAD"], capture_output=True, text=True, timeout=20)
        h = out.stdout.strip()
        if out.returncode == 0 and re.fullmatch(r"[0-9a-f]{7,}", h):
            dirty = subprocess.run(["git", "-C", repo, "status", "--porcelain", "--untracked-files=no"], capture_output=True, text=True, timeout=60)
            return h + ("-dirty" if dirty.stdout.strip() else "")
    except Exception:
        pass
    return "unknown"


_CTX = {}


def context():
    if _CTX:
        return _CTX
    _CTX["scrip"] = _git_short(os.path.join(S4E, "SCRIP"))
    _CTX["corpus"] = _git_short(os.path.join(S4E, "corpus"))
    who = ""
    try:
        sys.path.insert(0, HERE)
        import util_score_row  # noqa: E402  -- the ONE root->identity map; never a fourth copy
        who = util_score_row.derive_measurer() or ""
    except Exception:
        who = ""
    _CTX["measurer"] = who or ("root:" + os.path.basename(os.path.abspath(S4E).rstrip("/")))
    return _CTX


def _clean(v, field):
    s = "" if v is None else str(v)
    if "\t" in s or "\n" in s or "\r" in s:
        raise ValueError(f"{field} carries a tab or newline: {s!r}")
    return s


def normalize_row(r):
    out = {}
    out["class"] = _clean(r.get("class", ""), "class")
    if out["class"] not in CLASSES:
        raise ValueError(f"class must be one of {CLASSES}, not {out['class']!r}")
    for f in ("suite", "lang", "program"):
        out[f] = _clean(r.get(f, ""), f)
        if not out[f]:
            raise ValueError(f"{f} is empty")
    out["mode"] = _clean(r.get("mode", ""), "mode") or "m3"
    out["outcome"] = _clean(r.get("outcome", ""), "outcome").upper()
    if out["outcome"] not in OUTCOMES:
        raise ValueError(f"outcome must be one of {OUTCOMES}, not {out['outcome']!r} (program {out['program']})")
    secs = r.get("secs", 0)
    try:
        secs = float(secs) if secs not in ("", None) else 0.0
    except ValueError:
        raise ValueError(f"secs is not a number: {secs!r} (program {out['program']})")
    out["secs"] = ("%.3f" % secs).rstrip("0").rstrip(".") if secs else "0"
    out["note"] = _clean(r.get("note", ""), "note")
    return out


def append_rows(rows, db=None):
    """Append the rows atomically under a lock. Returns the number written (0 under S4E_PROGRESS_OFF, said aloud)."""
    rows = [normalize_row(r) for r in rows]
    if not rows:
        return 0
    if recording_off():
        print(f"progress: S4E_PROGRESS_OFF=1 -- {len(rows)} row(s) NOT recorded in {db or db_path()} (the control arm; a landing verdict never runs with it set)", file=sys.stderr)
        return 0
    path = db or db_path()
    ctx = context()
    ts = time.strftime("%Y-%m-%dT%H:%M:%S", time.gmtime())
    lines = []
    for r in rows:
        lines.append("\t".join([ts, ctx["scrip"], ctx["corpus"], ctx["measurer"], r["class"], r["suite"], r["lang"], r["program"], r["mode"], r["outcome"], r["secs"], r["note"]]))
    payload = "\n".join(lines) + "\n"
    try:
        d = os.path.dirname(path)
        if d and not os.path.isdir(d):
            raise ProgressUnwritable(f"directory does not exist: {d}")
        lock = path + ".lock"
        with open(lock, "a") as lk:
            fcntl.flock(lk.fileno(), fcntl.LOCK_EX)
            new = not os.path.exists(path) or os.path.getsize(path) == 0
            with open(path, "a", encoding="utf-8", newline="\n") as f:
                if new:
                    f.write("\t".join(COLUMNS) + "\n")
                f.write(payload)
                f.flush()
                os.fsync(f.fileno())
            fcntl.flock(lk.fileno(), fcntl.LOCK_UN)
    except (OSError, ProgressUnwritable) as e:
        msg = f"⛔ PROGRESS DATABASE UNWRITABLE ({path}): {e} -- {len(rows)} row(s) NOT recorded. A run that leaves the table untouched is a defect of that run (progress/README.md, CEO-319); fix the table or the permission, never the caller."
        print(msg, file=sys.stderr)
        raise ProgressUnwritable(msg)
    return len(rows)


SCORECARD_SUITE_KEYS = {"gimpel": "gimpel", "csnobol4": "csnobol4", "csnobol4_suite": "csnobol4", "snoflake": "snoflake", "snoflake_suite": "snoflake",
                        "aisnobol": "aisnobol", "dotnet": "dotnet", "testpgms": "testpgms", "spitbol_testpgms": "testpgms"}


def rows_from_results_tsv(path, suite, lang):
    """scorecard_snobol4.sh results.tsv: suite<TAB>program<TAB>m3<TAB>m4<TAB>t3<TAB>t4<TAB>note -- one row per mode.
    suite="auto" takes column 1 through SCORECARD_SUITE_KEYS and SKIPS (aloud) rows of a suite that is not in the table."""
    out = []
    skipped = collections.Counter()
    for raw in io.open(path, encoding="utf-8", errors="replace"):
        raw = raw.rstrip("\n")
        if not raw or raw.startswith("#"):
            continue
        f = raw.split("\t")
        if len(f) < 4:
            continue
        prog = f[1]
        row_suite = suite
        if suite == "auto":
            row_suite = SCORECARD_SUITE_KEYS.get(f[0].strip().lower(), "")
            if not row_suite:
                skipped[f[0]] += 1
                continue
        for mode, oi, ti in (("m3", 2, 4), ("m4", 3, 5)):
            o = f[oi].strip().upper()
            if o not in OUTCOMES:
                o = {"OK": "PASS", "TIMEOUT": "HANG", "SEGV": "CRASH", "NOREF": "UNGRADED", "-": "SKIP"}.get(o, "")
                if not o:
                    continue
            secs = f[ti] if len(f) > ti else "0"
            try:
                secs = float(secs)
            except ValueError:
                secs = 0
            out.append({"class": "package", "suite": row_suite, "lang": lang, "program": prog, "mode": mode, "outcome": o, "secs": secs, "note": (f[6] if len(f) > 6 else "")})
    if skipped:
        print("progress: results-tsv skipped rows of suite(s) not in the table: " + ", ".join(f"{k}={v}" for k, v in sorted(skipped.items())), file=sys.stderr)
    return out


def rows_from_triangulation(path, lang):
    """bench_triangulate_* TSV: kernel engine angle1_rate angle2_rate ratio verdict ... -- our engines only (m3, m4)."""
    out = []
    for raw in io.open(path, encoding="utf-8", errors="replace"):
        raw = raw.rstrip("\n")
        if not raw or raw.startswith("#") or raw.startswith("kernel\t"):
            continue
        f = raw.split("\t")
        if len(f) < 6:
            continue
        kernel, eng, a1, a2, ratio, verdict = f[0], f[1], f[2], f[3], f[4], f[5]
        if eng not in ("m3", "m4"):
            continue
        o = {"AGREE": "PASS", "DISAGREE": "FAIL", "CHECK-FAIL": "FAIL", "UNPROVEN": "UNPROVEN"}.get(verdict.strip().upper(), "UNPROVEN")
        out.append({"class": "benchmark", "suite": f"{lang}-bench", "lang": lang, "program": kernel, "mode": eng, "outcome": o, "secs": 0, "note": f"rate1={a1};rate2={a2};ratio={ratio};verdict={verdict}"})
    return out


def _main(argv):
    import argparse
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    a = sub.add_parser("append")
    for f in ("class", "suite", "lang", "program", "mode", "outcome"):
        a.add_argument("--" + f, required=True, dest=f)
    a.add_argument("--secs", default="0"); a.add_argument("--note", default="")
    b = sub.add_parser("rows-tsv"); b.add_argument("file")
    c = sub.add_parser("results-tsv"); c.add_argument("--suite", required=True); c.add_argument("--lang", required=True); c.add_argument("file")
    d = sub.add_parser("triangulation"); d.add_argument("--lang", required=True); d.add_argument("file")
    ctxp = sub.add_parser("context")
    args = ap.parse_args(argv)
    try:
        if args.cmd == "context":
            print("\t".join(f"{k}={v}" for k, v in context().items()) + f"\tdb={db_path()}\toff={recording_off()}"); return 0
        if args.cmd == "append":
            rows = [{k: getattr(args, k) for k in ("class", "suite", "lang", "program", "mode", "outcome", "secs", "note")}]
        elif args.cmd == "rows-tsv":
            rows = []
            for raw in io.open(args.file, encoding="utf-8", errors="replace"):
                raw = raw.rstrip("\n")
                if not raw or raw.startswith("#"):
                    continue
                f = raw.split("\t")
                if len(f) < 6:
                    raise ValueError(f"rows-tsv line needs class suite lang program mode outcome [secs [note]]: {raw!r}")
                rows.append({"class": f[0], "suite": f[1], "lang": f[2], "program": f[3], "mode": f[4], "outcome": f[5], "secs": f[6] if len(f) > 6 else 0, "note": f[7] if len(f) > 7 else ""})
        elif args.cmd == "results-tsv":
            rows = rows_from_results_tsv(args.file, args.suite, args.lang)
        else:
            rows = rows_from_triangulation(args.file, args.lang)
        n = append_rows(rows)
        print(f"progress: {n} row(s) recorded in {db_path()}" if n else f"progress: 0 rows recorded ({'S4E_PROGRESS_OFF=1' if recording_off() else 'nothing to record'})")
        return 0
    except ProgressUnwritable:
        return 2
    except (OSError, ValueError) as e:
        print(f"⛔ PROGRESS APPEND REFUSES(2): {e}", file=sys.stderr)
        return 2


if __name__ == "__main__":
    sys.exit(_main(sys.argv[1:]))
