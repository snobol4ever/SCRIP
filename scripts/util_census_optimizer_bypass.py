#!/usr/bin/env python3
"""Census/regression-check: compare mode-3 verdict under default / SCRIP_OPT=0 / SCRIP_ZD=0
for entries in a suite .sno/.ref pair, using corpus_suite_harness's own grading path so
companions/SNO_LIB/byte-comparison semantics match the real gate exactly (a bare
`./scrip extracted.sno` does NOT match -- run_suite_entry's companion_dir wiring is load-bearing).

--only NAME: check a single entry, exit 0 iff it still PASSes under both bypass arms (this is
the acceptance test for optimizer-off-path-segvs-so-the-emergency-bypass-is-not-a-correct-path.task.md).

No --only, no --gate: full census across every non-xfail entry, prints a summary and optionally
writes a per-entry CSV with --out. Always exits 0 in this mode -- informational.

--gate: PINNED-WATERMARK check (hq_P ruling 2026-08-29, topic ruling-watermark-not-blocking-and-
the-doctrine-question-underneath). NOT a FAIL=0 bar on the bypass arms -- a gate nobody can satisfy
gets disabled within a week. Instead: the DEFAULT arm is a hard 0-failures bar (exit 1 if it breaks,
independent of the bypass flags entirely); each bypass arm may regress AT MOST its pinned watermark
(exit 1 if either grows past it); and the graded population must match the pinned denominator or the
ratio silently means something else (exit 2, REFUSE, if the corpus reshuffled since the pins were
set). Called by scripts/test_gate_optbypass_watermark.sh. Exit codes follow lib_gate.sh convention:
0 CLEAN, 1 VIOLATION, 2 UNPROVEN/REFUSE.

--stable-subset (with --gate): the SCRIP_ZD=0 regression count is a SUM OVER NONDETERMINISTIC
ENTRIES -- a handful of witnesses flap between PASS and FAIL/CRASH run to run on one unchanged
binary, so a plain `<=` against an exact count is flaky by construction (task
optbypass-pin-stable-subset). Instead of one raw count: every entry the single census reading
flags as a zd0 regression is re-run --stability-runs (default 10) independent times; an entry is
STABLE and enters STABLE-SUBSET-COUNT (the new pinned, `<=`-able number) iff it counts as a
regression (non-PASS) on EVERY reading or PASSes on every reading -- KEYED ON PASS VS NON-PASS,
NEVER ON THE VERDICT KIND (hq_P correction, i10-thesis-and-the-witness-that-proves-it: an entry
that regresses every time but sometimes as CRASH and sometimes as HANG is stable, not flapping --
bucketing by kind manufactures flakiness that isn't there). Any entry whose PASS/non-PASS status
itself varies across readings is FLAPPING and is named on the FLAPPING-SET line instead, never
summed into the pin. REFUSES(2) if --stability-runs < 10 or if classification cannot complete at
all (infra failure, not an individual flaky reading -- those are
themselves a classification signal, not an error).
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
            # ⛔ POP, NOT INDEX -- same fix and same measured cause as classify_stability() below: an
            # unpopped dict keeps every completed Future (and its cached raw captured stdout/stderr)
            # alive for the whole loop. Pre-existing in this function; found while investigating why a
            # 305-candidate classification pass (which reuses this same pattern) hit 8.5GB RSS.
            for fut in concurrent.futures.as_completed(list(futs)):
                e = futs.pop(fut)
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


def classify_stability(paths, candidates, companion_dir, n_runs, workers):
    """For each candidate entry (already observed regressing under SCRIP_ZD=0 on one census reading),
    take n_runs INDEPENDENT SCRIP_ZD=0 readings and classify: STABLE iff all n_runs readings are the
    identical (kind, rc) pair, else FLAPPING. A single future's own exception is recorded as a
    ("HARNESSERR", msg) reading -- same precedent as run_arm -- rather than aborting the whole
    classification, since an inconsistent per-reading exception is itself flapping evidence, not an
    infra failure. Returns (stable_count, sorted_flapping_names). Raises on genuine infra failure
    (temp dir / thread pool setup) so the caller can REFUSE(2) rather than report a wrong count."""
    if not candidates:
        return 0, []
    for k in BYPASS_ENV:
        os.environ.pop(k, None)
    os.environ["SCRIP_ZD"] = "0"
    tmp_root = Path(tempfile.mkdtemp(prefix="optbypass_stability_"))
    readings = {e.name: [] for e in candidates}
    try:
        with concurrent.futures.ThreadPoolExecutor(max_workers=workers) as ex:
            futs = {ex.submit(h.run_suite_entry, paths, e, tmp_root, ["m3"], companion_dir=companion_dir): e.name
                    for e in candidates for _ in range(n_runs)}
            # ⛔ POP, NOT INDEX (MEASURED, seat08, 2026-09-01): a completed Future keeps its own full
            # result (here: raw captured stdout/stderr bytes) cached internally for as long as ANYTHING
            # holds the Future object itself. A dict that is only ever grown, never shrunk, keeps every
            # one of candidates*n_runs Futures alive as dict KEYS for the whole loop -- so a single
            # candidate that free-runs (unbounded output before its HANG timeout fires, plausible under
            # the correctness-unsound zd0 bypass) has its full captured buffer retained, times every run,
            # times every OTHER candidate's Future still sitting in the dict. Measured cause of an 8.5GB
            # RSS / 36-minute run on ~300 candidates x10: popping each Future the moment its (kind, rc)
            # is extracted lets it (and its buffer) be freed immediately instead of at loop exit.
            for fut in concurrent.futures.as_completed(list(futs)):
                name = futs.pop(fut)
                try:
                    v = fut.result()["m3"]
                    readings[name].append((v.kind, v.returncode))
                except Exception as exc:
                    readings[name].append(("HARNESSERR", str(exc)[:80]))
    finally:
        import shutil
        shutil.rmtree(tmp_root, ignore_errors=True)
        os.environ.pop("SCRIP_ZD", None)

    # ⭐ KEY ON PASS VS NON-PASS, NEVER ON THE VERDICT KIND (hq_P, mail i10-thesis-and-the-witness-
    # that-proves-it, 2026-09-01: "a count that buckets HANG and CRASH separately is unstable BY
    # CONSTRUCTION with no drift required"). An entry that regresses on every one of the n_runs
    # readings but sometimes as CRASH(-11) and sometimes as HANG is STABLE for this gate's purposes
    # -- it counts as a regression every time -- even though its raw (kind, rc) tuple never repeats.
    # The earlier version of this function compared the full tuple and misclassified exactly this
    # shape as flapping; readings[] still keeps the full (kind, rc) detail for diagnostics/--out, but
    # classification itself only asks whether "counts as PASS" ever disagreed across the n_runs.
    stable_ct = 0
    flapping = []
    for e in candidates:
        rs = readings[e.name]
        assert len(rs) == n_runs, f"{e.name}: got {len(rs)}/{n_runs} readings"
        if len({kind == "PASS" for kind, _rc in rs}) == 1:
            stable_ct += 1
        else:
            flapping.append(e.name)
    return stable_ct, sorted(flapping)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--sno", default=DEFAULT_SNO)
    ap.add_argument("--ref", default=DEFAULT_REF)
    ap.add_argument("--only", default="", help="check a single entry name; exit 0 iff it still PASSes under both bypass arms")
    ap.add_argument("--out", default="", help="write the full per-entry CSV here (full-census mode only)")
    ap.add_argument("--workers", type=int, default=4)
    ap.add_argument("--gate", action="store_true", help="pinned-watermark check -- see module docstring")
    ap.add_argument("--pinned-population", type=int, default=1494, dest="pinned_population")
    ap.add_argument("--pinned-opt0-max", type=int, default=176, dest="pinned_opt0_max")
    ap.add_argument("--pinned-zd0-max", type=int, default=291, dest="pinned_zd0_max",
                     help="legacy raw-count pin; ignored by --gate once --stable-subset is set")
    ap.add_argument("--stable-subset", action="store_true", dest="stable_subset",
                     help="split SCRIP_ZD=0 regressions into a reproducible stable subset (pinned) "
                          "and a named flapping set (reported, never summed) -- see module docstring")
    ap.add_argument("--stability-runs", type=int, default=10, dest="stability_runs")
    ap.add_argument("--stability-workers", type=int, default=0, dest="stability_workers",
                     help="concurrency for the N-run classification pass; default 0 means 'use "
                          "--workers' -- MEASURED (seat08, optbypass-pin-stable-subset, 2026-09-01): "
                          "an independent higher value here is a confound, not a convenience. 5 "
                          "identical candidates classified at workers=1/4/8 gave 2/4/5 flapping "
                          "respectively -- the classifier's OWN concurrency was manufacturing "
                          "flakiness the base (--workers) reading never experiences, because SCRIP_ZD=0 "
                          "is the correctness-unsound bypass and its failure mode (stale/uninitialized "
                          "reads) is sensitive to contention. Matching the base reading's concurrency is "
                          "the closest apples-to-apples comparison available, not a guarantee of zero "
                          "confound -- ambient load from OTHER processes still varies at fixed workers.")
    ap.add_argument("--pinned-zd0-stable-max", type=int, default=0, dest="pinned_zd0_stable_max",
                     help="pinned ceiling on STABLE-SUBSET-COUNT; only used with --gate --stable-subset")
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

    if args.gate and len(graded) != args.pinned_population:
        print(f"⛔ REFUSE(2) [optbypass_watermark]: graded population is {len(graded)}, but the watermark was "
              f"pinned against {args.pinned_population}. The corpus reshuffled since the pin was set (this tree "
              f"churns FAST -- CLAUDE.md). A different denominator makes the ratio mean something else, so this "
              f"is refused rather than compared. Re-run with --out to re-measure, then ask hq_P/ceo to re-pin "
              f"--pinned-population/--pinned-opt0-max/--pinned-zd0-max in test_gate_optbypass_watermark.sh.")
        sys.exit(2)

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

    if args.gate:
        violations = []
        if default_fail_ct != 0:
            violations.append(f"DEFAULT arm (no bypass flags -- the shipped compiler) has {default_fail_ct} "
                               f"failure(s) out of {len(graded)}. This is a hard bar independent of the "
                               f"watermark, and it just broke.")
        if len(opt0_reg) > args.pinned_opt0_max:
            violations.append(f"SCRIP_OPT=0 regresses {len(opt0_reg)}/{len(graded)}, above the pinned "
                               f"watermark of {args.pinned_opt0_max}.")

        stable_ct = None
        flapping_names = []
        if args.stable_subset:
            if args.stability_runs < 10:
                print(f"⛔ REFUSE(2) [optbypass_watermark]: --stability-runs={args.stability_runs} < 10 -- "
                      f"the stable/flapping split needs at least 10 independent SCRIP_ZD=0 readings per "
                      f"candidate entry to be trustworthy (GOAL: deterministic across N=10 runs).")
                sys.exit(2)
            candidates = [e for e in entries if e.name in zd0_names]
            stability_workers = args.stability_workers or args.workers
            try:
                stable_ct, flapping_names = classify_stability(
                    paths, candidates, companion_dir, args.stability_runs, stability_workers)
            except Exception as exc:
                print(f"⛔ REFUSE(2) [optbypass_watermark]: stable/flapping classification did not complete "
                      f"({exc!r}) -- a stability run could not finish, cannot classify this reading.")
                sys.exit(2)
            print(f"STABLE-SUBSET-COUNT: {stable_ct}")
            print(f"FLAPPING-SET: {' '.join(flapping_names) if flapping_names else '(none)'}")
            if stable_ct > args.pinned_zd0_stable_max:
                violations.append(f"SCRIP_ZD=0 STABLE-SUBSET-COUNT is {stable_ct}/{len(graded)} "
                                   f"(flapping, excluded: {len(flapping_names)}), above the pinned "
                                   f"stable-subset watermark of {args.pinned_zd0_stable_max}.")
        # ⛔ ADDITIVE, NOT REPLACING (seat08, optbypass-pin-stable-subset, 2026-09-01): the raw-count
        # check stays live even with --stable-subset on. MEASURED, not assumed safe: two full runs of
        # the (bug-fixed) classifier, both --workers 4, disagreed by 9 (251 vs 260 stable) under
        # sharply different FLEET-8 ambient load (25 vs 11-13), and a controlled 5-entry sweep at
        # workers=1/4/8 showed 2/4/5 flapping on the IDENTICAL candidates -- classification outcome is
        # load-sensitive, not yet proven reproducible. hq_P's falsifiable prediction ("stable-subset
        # count should be near-EXACT across readings") is NOT YET CONFIRMED under real fleet load; see
        # the task LEDGER. Until a trustworthy pin exists, --pinned-zd0-stable-max is set generously
        # (informational, not a real ratchet) and this proven raw ceiling remains the actual gate.
        if len(zd0_reg) > args.pinned_zd0_max:
            violations.append(f"SCRIP_ZD=0 regresses {len(zd0_reg)}/{len(graded)}, above the pinned "
                               f"watermark of {args.pinned_zd0_max}.")

        if violations:
            print("⛔ VIOLATION(1) [optbypass_watermark]:")
            for v in violations:
                print(f"    - {v}")
            if len(opt0_reg) > args.pinned_opt0_max:
                print(f"    SCRIP_OPT=0 regressing entries ({len(opt0_reg)}, sorted -- diff against the previous run's list to name the movement):")
                for n, kind, rc in sorted(opt0_reg):
                    print(f"      {n} {kind} rc={rc}")
            if len(zd0_reg) > args.pinned_zd0_max:
                print(f"    SCRIP_ZD=0 regressing entries ({len(zd0_reg)}, sorted -- keyed PASS vs non-PASS; the kind flaps, the membership is what to diff):")
                for n, kind, rc in sorted(zd0_reg):
                    print(f"      {n} {kind} rc={rc}")
            print(f"    wall clock: {time.time()-t0:.1f}s  workers={args.workers}")
            sys.exit(1)
        if args.stable_subset:
            print(f"✅ OK [optbypass_watermark]: DEFAULT 0/{len(graded)} (hard). "
                  f"SCRIP_OPT=0 {len(opt0_reg)}/{len(graded)} (watermark <= {args.pinned_opt0_max}). "
                  f"SCRIP_ZD=0 raw {len(zd0_reg)}/{len(graded)}, STABLE-SUBSET {stable_ct} "
                  f"(watermark <= {args.pinned_zd0_stable_max}), flapping {len(flapping_names)} named above.")
        else:
            print(f"✅ OK [optbypass_watermark]: DEFAULT 0/{len(graded)} (hard). "
                  f"SCRIP_OPT=0 {len(opt0_reg)}/{len(graded)} (watermark <= {args.pinned_opt0_max}). "
                  f"SCRIP_ZD=0 {len(zd0_reg)}/{len(graded)} (watermark <= {args.pinned_zd0_max}).")
        print(f"    wall clock: {time.time()-t0:.1f}s  workers={args.workers}")
        sys.exit(0)

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
