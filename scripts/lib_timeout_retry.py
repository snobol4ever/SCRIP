"""lib_timeout_retry.py -- subprocess.run FOR A GRADED RUN: A TIMEOUT WHILE THE 1-MINUTE LOAD EXCEEDS THE CORES IS COULD NOT MEASURE,
SO THE COMMAND IS RUN ONCE MORE AND ONLY A SECOND TIMEOUT STANDS (ceo CEO-1335; the coo 2026-10-09). The Python graders' face of
util_timeout_retry.sh (the bash runners' face) -- the two write one stamp format, read the load through lib_fanout.py (the harness's
one definition), and honour S4E_TIMEOUT_RETRY=0 the same way.

    run(argv, timeout=T, key=K, **kw)   is subprocess.run(argv, timeout=T, **kw) with that one difference: on TimeoutExpired, when
    lib_fanout.fanout_load1() exceeds fanout_cores() and S4E_TIMEOUT_RETRY is not 0, the same argv runs once more (a stdin file
    object is rewound; input= is replayed as given), and only a second TimeoutExpired is raised. A retry appends
        <key> TAB <m3|m4|oracle> TAB retried=1 load1=<first>/<second> nproc=<n> [timeout-twice]
    to $S4E_TIMEOUT_STAMP, which util_progress_append.py attaches to the row <key> names (K, else $S4E_TIMEOUT_KEY, else the argv's
    file stems). A timeout at or under the cores is raised at once, exactly as subprocess.run raises it.
"""
import os
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
ORACLES = {"sbl", "spitbol", "fpc", "gprolog", "swipl", "icont", "iconx", "raku", "rakudo", "csnobol4", "snobol4", "jcont", "jconx"}


def _load():
    try:
        import lib_fanout as f
        return f.fanout_load1(), f.fanout_cores()
    except Exception:
        return 0.0, os.cpu_count() or 1


def _role(argv):
    names = [os.path.basename(str(a)) for a in argv]
    if "scrip" in names:
        return "m4" if any("--compile" in str(a) for a in argv) else "m3"
    if any(n in ORACLES for n in names):
        return "oracle"
    return "m4"


def _key(key, argv):
    if key:
        return key
    if os.environ.get("S4E_TIMEOUT_KEY"):
        return os.environ["S4E_TIMEOUT_KEY"]
    stems = []
    for a in argv:
        a = str(a)
        if a.startswith("-") or not ("/" in a or "." in a):
            continue
        b = os.path.basename(a)
        stems.append(b.rsplit(".", 1)[0] if "." in b else b)
    return ",".join(stems)


def _stamp(key, argv, l1, l2, n, twice):
    p = os.environ.get("S4E_TIMEOUT_STAMP", "")
    if not p:
        return
    try:
        with open(p, "a") as f:
            f.write("%s\t%s\tretried=1 load1=%.2f/%.2f nproc=%d%s\n" % (_key(key, argv), _role(argv), l1, l2, n, " timeout-twice" if twice else ""))
    except OSError:
        pass


def run(argv, timeout=None, key=None, **kw):
    try:
        return subprocess.run(argv, timeout=timeout, **kw)
    except subprocess.TimeoutExpired:
        if timeout is None or os.environ.get("S4E_TIMEOUT_RETRY", "1") == "0":
            raise
        l1, n = _load()
        if l1 <= n:
            raise
    stdin = kw.get("stdin")
    if hasattr(stdin, "seek"):
        try:
            stdin.seek(0)
        except (OSError, ValueError):
            pass
    try:
        r = subprocess.run(argv, timeout=timeout, **kw)
    except subprocess.TimeoutExpired:
        _stamp(key, argv, l1, _load()[0], n, True)
        raise
    _stamp(key, argv, l1, _load()[0], n, False)
    return r
