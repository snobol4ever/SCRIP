#!/usr/bin/env python3
"""Census/regression-check: compare mode-3 verdict under default / SCRIP_OPT=0 / SCRIP_ZD=0
for entries in a suite .sno/.ref pair, using corpus_suite_harness's own grading path so
companions/SNO_LIB/byte-comparison semantics match the real gate exactly (a bare
`./scrip extracted.sno` does NOT match -- run_suite_entry's companion_dir wiring is load-bearing).

--only NAME: check a single entry, exit 0 iff it still PASSes under both bypass arms (this is
the acceptance test for optimizer-off-path-segvs-so-the-emergency-bypass-is-not-a-correct-path.task.md).

No --only: full census across every non-xfail entry, prints a summary and optionally writes a
per-entry CSV with --out. Always exits 0 in this mode -- informational, not a blocking gate.
Promoting it to a fleet gate (wiring into make test) is a separate decision for HQ/ceo: as of
the 2026-08-29 census, SCRIP_OPT=0 regresses 176/1494 graded entries and SCRIP_ZD=0 regresses
289/1494, so a strict gate would be permanently red until the underlying defect(s) are cured.
"""
import argparse
import concurrent.futures
import csv
import os
import sys
import tempfile
import time
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import corpus_suite_harness as h  # noqa: E402

BYPASS_ENV = ("SCRIP_OPT", "SCRIP_ZD")
DEFAULT_SNO = str(Path(__file__).resolve().parent.parent.parent / "corpus/tests/snobol4/ALL.sno")
DEFAULT_REF = str(Path(__file__).resolve().parent.parent.parent / "corpus/tests/snobol4/ALL.ref")


def run_arm(paths, entries, env_flag, companion_dir, workers):
    for k in BYPASS_ENV:
        os.environ.pop(k, None)
    if env_flag:
        os.environ[env_flag] = "0"
    tmp_root = Path(tempfile.mkdtemp(prefix="optbypass_"))
    results = {}
    try:
        with concurrent.futures.ThreadPoolExecutor(max_workers=workers) as ex:
            futs = {ex.submit(h.run_suite_entry, paths, e, tmp_root, ["m3"], companion_dir=companion_dir): e
                    for e in entries}
            for fut in concurrent.futures.as_completed(futs):
                e = futs[fut]
                try:
                    v = fut.result()["m3"]
                    results[e.name] = (v.kind, v.returncode)
                except Exception as exc:
                    results[e.name] = ("HARNESSERR", str(exc)[:80])
    finally:
        import shutil
        shutil.rmtree(tmp_root, ignore_errors=True)
    for k in BYPASS_ENV:
        os.environ.pop(k, None)
    return results


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--sno", default=DEFAULT_SNO)
    ap.add_argument("--ref", default=DEFAULT_REF)
    ap.add_argument("--only", default="", help="check a single entry name; exit 0 iff it still PASSes under both bypass arms")
    ap.add_argument("--out", default="", help="write the full per-entry CSV here (full-census mode only)")
    ap.add_argument("--workers", type=int, default=4)
    args = ap.parse_args()

    paths = h.resolve_paths()
    h.check_scrip(paths)
    entries = h.read_suite(args.sno, args.ref, in_path=h.sidecar_in_path(args.sno), x_path=h.sidecar_xfail_path(args.sno))
    companion_dir = Path(args.sno).parent

    if args.only:
        matches = [e for e in entries if e.name == args.only]
        if not matches:
            sys.exit(f"⛛ no entry named {args.only!r} in {args.sno} -- corpus reshuffled again, re-locate the witness")
        e = matches[0]
        ok = True
        for flag in (None,) + BYPASS_ENV:
            r = run_arm(paths, [e], flag, companion_dir, 1)
            kind, rc = r[e.name]
            label = flag or "default"
            print(f"{label}: {kind} rc={rc}")
            if not flag and kind != "PASS":
                sys.exit(f"⛛ default arm itself does not PASS on {args.only!r} (kind={kind} rc={rc}) -- not a clean control, pick another witness")
            if flag and kind != "PASS":
                ok = False
        sys.exit(0 if ok else 1)

    graded = [e for e in entries if not e.xfail]
    print(f"total entries={len(entries)} graded(non-xfail)={len(graded)} xfail={len(entries)-len(graded)}", file=sys.stderr)
    t0 = time.time()
    default_r = run_arm(paths, entries, None, companion_dir, args.workers)
    opt0_r = run_arm(paths, entries, "SCRIP_OPT", companion_dir, args.workers)
    zd0_r = run_arm(paths, entries, "SCRIP_ZD", companion_dir, args.workers)

    if args.out:
        with open(args.out, "w", newline="") as f:
            w = csv.writer(f)
            w.writerow(["name", "xfail", "default_kind", "default_rc", "opt0_kind", "opt0_rc",
                        "zd0_kind", "zd0_rc", "opt0_changed", "zd0_changed"])
            for e in entries:
                dk, drc = default_r[e.name]
                ok, orc = opt0_r[e.name]
                zk, zrc = zd0_r[e.name]
                w.writerow([e.name, e.xfail, dk, drc, ok, orc, zk, zrc, int(ok != dk), int(zk != dk)])

    def summarize(arm_r):
        regs = [(e.name, *arm_r[e.name]) for e in graded if default_r[e.name][0] == "PASS" and arm_r[e.name][0] != "PASS"]
        sig = {}
        for _, kind, rc in regs:
            key = f"CRASH(rc={rc})" if kind == "CRASH" else kind
            sig[key] = sig.get(key, 0) + 1
        return regs, sig

    default_fail_ct = sum(1 for e in graded if default_r[e.name][0] != "PASS")
    opt0_reg, opt0_sig = summarize(opt0_r)
    zd0_reg, zd0_sig = summarize(zd0_r)
    opt0_names = {n for n, *_ in opt0_reg}
    zd0_names = {n for n, *_ in zd0_reg}

    print("\n===== SUMMARY =====")
    print(f"graded population: {len(graded)}  default-arm control failures: {default_fail_ct}")
    print(f"SCRIP_OPT=0 regressions: {len(opt0_reg)}/{len(graded)} ({100*len(opt0_reg)/len(graded):.1f}%)  {opt0_sig}")
    print(f"SCRIP_ZD=0  regressions: {len(zd0_reg)}/{len(graded)} ({100*len(zd0_reg)/len(graded):.1f}%)  {zd0_sig}")
    print(f"overlap: shared={len(opt0_names & zd0_names)} opt0_only={len(opt0_names-zd0_names)} zd0_only={len(zd0_names-opt0_names)}")
    print(f"wall clock: {time.time()-t0:.1f}s")
    if args.out:
        print(f"csv written: {args.out}")


if __name__ == "__main__":
    main()
