#!/usr/bin/env python3
"""util_build_icon_package_container.py -- AN ICON PACKAGE'S CONTAINER IS BUILT FROM ITS BOARD'S OWN UNITS (ceo CEO-1366 (a), 2026-09-30;
row icon-package-containers-regenerated-from-the-shipped-files-and-every-container-key-equals-its-boards-progress-key-ceo-1366, coo).

   python3 scripts/util_build_icon_package_container.py PKGDIR            # read only: what the container would hold, and what it holds now
   python3 scripts/util_build_icon_package_container.py PKGDIR --write    # rewrite ALL.icn ALL.ref ALL.in ALL.wantrc ALL.argv ALL.excluded.txt

PKGDIR is corpus/packages/icon/{arizona_tests,jcon_tests,ipl}. WHY: the three containers were cut on 2026-09-05/06 by
util_build_package_suite.py (its own population, refs re-cut by the oracle over the raw files) and never again, while the boards moved on
(drivers graded under their library's key, CEO-1269; jcon kwds reshaped at b32af4985; the arizona board keying sub/stem). On 2026-09-29 they
held arizona 89 of the board's 116 run units, jcon 70 of 85, IPL 78 of its run tier, and the area smoke graded 15 stale entries red on
bd55a1469 while all three boards read zero red (COO-229). This builder cannot drift that way: its population IS the board's --
util_icon_package_units.py keys(), the one reader of the runners' own key rules -- and each entry's ref IS the .ref the board grades against.

EACH RUN UNIT (a key whose executable has a .ref beside it) becomes ONE ENTRY named by its key, or ONE NAMED EXCLUSION -- never neither:
  - the entry's text is the PRE-STEP's output for the unit's executable (out/scrip-ipp run in the executable's own directory, LPATH the IPL
    include directories -- RULES.md SCRIP DOES NOT PREPROCESS, CEO-1366), so an $include resolves where the board resolves it and the
    container carries no directive; the ref is the executable's .ref, byte for byte;
  - stdin is the executable's .dat (all three runners feed it); jcon also passes the .dat's name as argv[1] (JCON's addtest convention,
    test_icon_jcon_suite.sh run_one) and reads NAME.args; IPL reads NAME.argv (one TAB-separated line, lib_icon_ipl_isolation.sh
    ipl_argv_read) and NAME.rc for a declared exit (ipl_rc_declared); arizona's NAME.wantrc the same way.
  - EXCLUDED, BY NAME, WITH THE REASON: a unit outside the board's graded denominator (the files the smoke and the board both read --
    corpus_suite_harness._smoke_outside_names); a unit the pre-step refuses; a unit that links a library other than an IPL procs module
    (the harness puts a pre-stepped IPL procs directory first on IPATH, and nothing else travels with a container entry); a unit whose
    board run needs a sidecar a container cannot carry (NAME.fixtures/, NAME.env, NAME.pty, NAME.outfiles, NAME.mask).
ALL.csv IS NEVER WRITTEN: in these packages it is the boards' per-unit settings table (every key, compile tier included, heap_kb,
stack_kb, compile_args, run_args -- clause 8 (f)); every entry must already carry its row there, and a missing one refuses the build
(util_icon_package_units.py rows --apply mints it). The accounting is printed and checked: entries + exclusions == run units.
"""
import csv
import os
import re
import subprocess
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import corpus_suite_harness as h  # noqa: E402
import util_icon_package_units as U  # noqa: E402

LINK_RX = re.compile(r"^[ \t]*link[ \t]+([^\n#]*)", re.M)
NO_CARRY = (".env", ".pty", ".outfiles", ".mask")


def links_of(text):
    names = []
    for m in LINK_RX.finditer(text):
        for tok in re.split(r"[,\s]+", m.group(1).replace('"', " ").replace(";", " ")):
            tok = tok.strip()
            if tok:
                names.append(tok[:-4] if tok.endswith(".icn") else tok)
    return names


def pre_step(tool, lpath, exe):
    r = subprocess.run([str(tool), exe.name], cwd=str(exe.parent), stdin=subprocess.DEVNULL, capture_output=True, timeout=60,
                       env=dict(os.environ, LPATH=lpath))
    if r.returncode != 0:
        return None, "pre-step refused (%s rc=%d): %s" % (tool.name, r.returncode, r.stderr.decode("utf-8", "replace").strip().replace("\n", " | ")[:300])
    return r.stdout.decode("utf-8", "replace"), None


def one_line(path):
    lines = [ln for ln in path.read_text(encoding="utf-8", errors="replace").split("\n") if ln.strip() and not ln.lstrip().startswith("#")]
    return lines


def build(pkgdir):
    pkgdir = Path(pkgdir).resolve()
    paths = h.resolve_paths()
    tool = Path(os.environ.get("IPP_BIN", str(Path(paths["scrip_root"]) / "out" / "scrip-ipp")))
    if not (tool.is_file() and os.access(tool, os.X_OK)):
        h.refuse(f"the Icon pre-step {tool} is not built -- make builds it beside scrip")
    ipl = Path(paths["corpus"]) / "packages" / "icon" / "ipl"
    lpath = os.environ.get("LPATH") or f"{ipl / 'incl'}:{ipl / 'gincl'}"
    procs = {p.stem for p in (ipl / "procs").glob("*.icn")}
    pkg, want = U.keys(str(pkgdir))
    outside = h._smoke_outside_names(pkgdir / "ALL.icn")
    with open(pkgdir / "ALL.csv", newline="") as f:
        have = {r["entry"] for r in csv.DictReader(f)}
    entries, excluded, units = [], [], 0
    for key in sorted(want):
        src, exe = want[key]
        if not exe:
            continue
        exe = Path(exe)
        ref = exe.with_suffix(".ref")
        if not ref.is_file():
            continue
        units += 1
        if key in outside or Path(key).name in outside or exe.name in outside or src and Path(src).name in outside:
            excluded.append((key, "outside the board's graded denominator (declared beside the package; the board and the smoke skip it alike)"))
            continue
        side = [s for s in NO_CARRY if exe.with_suffix(s).exists()] + (["fixtures/"] if Path(str(exe)[:-4] + ".fixtures").is_dir() else [])
        if side:
            excluded.append((key, "its board run needs %s beside it, which a container entry cannot carry -- graded by its board" % ", ".join(exe.stem + s for s in side)))
            continue
        text, why = pre_step(tool, lpath, exe)
        if why:
            excluded.append((key, why))
            continue
        foreign = [n for n in links_of(text) if n not in procs]
        if foreign:
            excluded.append((key, "links %s -- a library that does not travel with a container entry (only the IPL procs directory is on the "
                                  "harness's link path) -- graded by its board" % ", ".join(foreign)))
            continue
        if key not in have:
            h.refuse(f"{pkg}: {key} has no settings row in ALL.csv -- run util_icon_package_units.py rows {pkgdir} --apply first")
        dat = exe.with_suffix(".dat")
        stdin = dat.read_text(encoding="utf-8", errors="replace") if dat.is_file() else None
        argv, want_rc = None, 0
        if pkg == "jcon_tests":
            if dat.is_file():
                argv = [dat.name]
            if exe.with_suffix(".args").is_file():
                argv = open(exe.with_suffix(".args")).read().split()
        elif pkg == "ipl" and exe.with_suffix(".argv").is_file():
            ln = one_line(exe.with_suffix(".argv"))
            if len(ln) != 1:
                h.refuse(f"{exe.with_suffix('.argv')} holds {len(ln)} declaration lines, expected exactly 1")
            argv = ln[0].split("\t")
        for rc_side in (".rc", ".wantrc"):
            if exe.with_suffix(rc_side).is_file():
                ln = one_line(exe.with_suffix(rc_side))
                want_rc = int(ln[0].split()[0]) if ln else 0
        e = h.Entry("block", len(entries) + 1, key, text.rstrip("\n").split("\n"),
                    ref.read_text(encoding="utf-8", errors="replace").rstrip("\n").split("\n"), stdin=stdin, want_rc=want_rc)
        if argv:
            e.argv = argv
        entries.append(e)
    if len(entries) + len(excluded) != units:
        h.refuse(f"{pkg}: accounting broken -- {len(entries)} entries + {len(excluded)} excluded != {units} run units")
    return pkg, pkgdir, entries, excluded, units


def write(pkgdir, entries, excluded):
    out_in = pkgdir / "ALL.in"
    wrote_in = h.write_suite(entries, str(pkgdir / "ALL.icn"), str(pkgdir / "ALL.ref"), out_in=str(out_in), lang="icon")
    if not wrote_in and out_in.exists():
        out_in.unlink()
    for name, lines in (("ALL.wantrc", [f"{e.name}\t{e.want_rc}" for e in entries if e.want_rc]),
                        ("ALL.argv", [e.name + "\t" + "\t".join(e.argv) for e in entries if getattr(e, "argv", None)])):
        p = pkgdir / name
        if lines:
            p.write_text("\n".join(lines) + "\n")
        elif p.exists():
            p.unlink()
    (pkgdir / "ALL.excluded.txt").write_text("\n".join(f"{n}: {r}" for n, r in sorted(excluded)) + ("\n" if excluded else ""))


def main(argv):
    if len(argv) < 2 or argv[1].startswith("-"):
        print(__doc__.split("\n\n")[0]); return 2
    pkg, pkgdir, entries, excluded, units = build(argv[1])
    print(f"{pkg}: {units} run unit(s) -> {len(entries)} container entr(y/ies), {len(excluded)} excluded by name")
    kinds = {}
    for _n, r in excluded:
        k = r.split(" ")[0]
        kinds[k] = kinds.get(k, 0) + 1
    print("  exclusions by reason: " + ", ".join(f"{k} {v}" for k, v in sorted(kinds.items())))
    if "--write" in argv:
        write(pkgdir, entries, excluded)
        print(f"  written: ALL.icn ALL.ref ALL.in ALL.wantrc ALL.argv ALL.excluded.txt under {pkgdir} (ALL.csv untouched)")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
