#!/usr/bin/env bash
# test_gate_harness_transitive_companions.sh -- THE ACCEPTANCE GATE for row
# harness-copies-companions-to-closure-not-just-the-first-level (hq_T -> seat06, 2026-09-05,
# minted on seat06's own FINDING-2026-09-04-seat06-corpus-suite-harness-transitive-include-
# companions-not-copied.md).
#
# THE DEFECT: corpus_suite_harness.py's _copy_companions() only ever resolved the companions an
# entry's OWN text names -- one level. A 2+-level -INCLUDE chain (entry includes A, A itself
# includes B) left B absent from the isolated grading temp dir, so the entry graded through the
# normal board path with the WRONG FAILURE KIND: FAIL/SKIP where the truth (verified directly
# against the real corpus directory, oracle-confirmed) is a CRASH (workspace-heap exhaustion,
# SIGABRT). The total never moved -- the entry buckets XFAIL either way -- which is exactly why
# this is dangerous: it is invisible to every count-based check, and a wrong kind is what someone
# later cites as evidence of a different defect.
#
# THE CURE: _copy_companions is now a worklist that re-scans each newly-copied companion for ITS
# own companions, to a fixed point (closure, not a fixed depth), with a `seen` set as the cycle
# guard so a self- or mutually-including pair terminates instead of spinning.
#
# ARMS: 1 the REAL witness (array_replace_branch_2, a genuine 2-level chain in the SNOBOL4 master)
# graded through run_suite_entry -- self-contained fail-once/pass-once: monkeypatched back to a
# faithful reconstruction of the one-level copier it reproduces the exact pre-cure verdicts (FAIL
# m3 / SKIP m4, matching the FINDING that spawned this row), and with the real cured copier it
# reports CRASH in both modes, matching the oracle-confirmed true state · 2 a synthetic 3-level
# chain resolves to closure, not just depth-2 · 3 a deliberate include cycle (mutual and self)
# terminates promptly instead of spinning · 4 an unresolvable second-level include stays a silent
# no-op -- the real first-level file is still copied, the ghost is not manufactured, no exception
# -- so the transitive resolver cannot manufacture a false green one level down · 5 the mechanism
# is named in the source, so a reviewer greps to it.
# EXIT: 0 all arms · 1 an arm failed · 2 REFUSED (no python3/harness/corpus, or an unbuilt tree).
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
S4E="${S4E_HOME:-$(cd "$ROOT/.." && pwd)}"
H="$HERE/corpus_suite_harness.py"
FAM="$S4E/corpus/tests/snobol4"
[ -f "$H" ] || { echo "⛔ REFUSED-TO-GRADE rc=2: harness not found at $H"; exit 2; }
command -v python3 >/dev/null 2>&1 || { echo "⛔ REFUSED-TO-GRADE rc=2: python3 not on PATH"; exit 2; }
[ -f "$FAM/ALL.sno" ] && [ -f "$FAM/ALL.ref" ] || { echo "⛔ REFUSED-TO-GRADE rc=2: snobol4 master absent at $FAM"; exit 2; }
[ -f "$FAM/gimpel_triage_class8_sig6_perm_module.sno" ] && [ -f "$FAM/gimpel_triage_class8_sig6_perm_swap.sno" ] || {
    echo "⛔ REFUSED-TO-GRADE rc=2: the real 2-level witness companions are gone from $FAM -- ARM 1 needs the genuine corpus witness, not a synthetic stand-in"; exit 2; }
"$HERE/util_require_fresh.sh" --gate test_gate_harness_transitive_companions >/dev/null 2>&1 || {
    echo "⛔ REFUSED-TO-GRADE rc=2: this tree's binary is stale or unbuilt -- ARM 1 runs a real program. Run 'make'."; exit 2; }
fails=0; checks=0
ck() { checks=$((checks+1)); if [ "$1" = ok ]; then printf '  ok    %s\n' "$2"; else printf '  FAIL  %s\n' "$2"; fails=$((fails+1)); fi; }
echo "=== gate: companion files are copied to transitive closure, with a cycle guard ==="

echo "--- ARM 1: the real witness -- fail-once (one-level copier) then pass-once (the cure) ---"
out1="$(timeout 240 python3 - "$H" "$FAM" <<'PY' 2>&1
import sys, pathlib, tempfile
sys.path.insert(0, str(pathlib.Path(sys.argv[1]).parent))
import corpus_suite_harness as H
fam = pathlib.Path(sys.argv[2])
paths = H.resolve_paths()
H.check_scrip(paths)
entries = H.read_suite(str(fam/"ALL.sno"), str(fam/"ALL.ref"),
                        in_path=H.sidecar_in_path(str(fam/"ALL.sno")),
                        x_path=H.sidecar_xfail_path(str(fam/"ALL.sno")),
                        w_path=H.sidecar_wantrc_path(str(fam/"ALL.sno")))
entry = next((e for e in entries if e.name == "array_replace_branch_2"), None)
if entry is None:
    print("MISSING-ENTRY")
    sys.exit(0)

def old_one_level(text, companion_dir, dest_dir):
    """Faithful reconstruction of the PRE-CURE _copy_companions: single pass over the entry's
    own text only, never re-scanning a companion it just copied for that companion's own."""
    import shutil
    if not companion_dir:
        return
    cfg = pathlib.Path(companion_dir) / "config"
    dirs = [pathlib.Path(companion_dir)] + ([cfg] if cfg.is_dir() else [])
    for name in H._companion_files(text):
        if pathlib.Path(name).is_absolute():
            continue
        if ".." in pathlib.Path(name).parts:
            continue
        for d in dirs:
            src, dst = d / name, pathlib.Path(dest_dir) / name
            if src.is_file() and not (dst.exists() and src.samefile(dst)):
                dst.parent.mkdir(parents=True, exist_ok=True)
                shutil.copy(src, dst)
                break

real = H._copy_companions
try:
    H._copy_companions = old_one_level
    with tempfile.TemporaryDirectory() as t:
        before = H.run_suite_entry(paths, entry, t, ["m3", "m4"], companion_dir=fam)
finally:
    H._copy_companions = real
with tempfile.TemporaryDirectory() as t:
    after = H.run_suite_entry(paths, entry, t, ["m3", "m4"], companion_dir=fam)
print("BEFORE", before["m3"].kind, before["m4"].kind)
print("AFTER", after["m3"].kind, after["m4"].kind)
PY
)"
echo "$out1" | sed 's/^/    /'
grep -q '^MISSING-ENTRY$' <<<"$out1" && { echo "⛔ REFUSED-TO-GRADE rc=2: array_replace_branch_2 is no longer in $FAM/ALL.sno -- the real witness this arm needs is gone"; exit 2; }
before_line="$(grep '^BEFORE' <<<"$out1")"
after_line="$(grep '^AFTER' <<<"$out1")"
[ "$before_line" = "BEFORE FAIL SKIP" ] && ck ok "fail-once: the one-level copier reproduces the exact pre-cure verdicts (FAIL m3 / SKIP m4)" \
    || ck no "fail-once did not reproduce the documented pre-cure shape -- ${before_line:-<no output>}"
[ "$after_line" = "AFTER CRASH CRASH" ] && ck ok "pass-once: the cured copier reports CRASH in both modes, the oracle-confirmed true state" \
    || ck no "the cure did not turn this witness into a CRASH verdict -- ${after_line:-<no output>}"

echo "--- ARM 2/3/4: synthetic closure depth, cycle termination, and refusal-safety ---"
out2="$(timeout 30 python3 - "$H" <<'PY' 2>&1
import sys, pathlib, tempfile, time
sys.path.insert(0, str(pathlib.Path(sys.argv[1]).parent))
import corpus_suite_harness as H

with tempfile.TemporaryDirectory() as cdir, tempfile.TemporaryDirectory() as ddir:
    cd, dd = pathlib.Path(cdir), pathlib.Path(ddir)

    (cd/"syn3_a.sno").write_text("-INCLUDE 'syn3_b.sno'\n")
    (cd/"syn3_b.sno").write_text("-INCLUDE 'syn3_c.sno'\n")
    (cd/"syn3_c.sno").write_text("* terminal, no further include\n")
    H._copy_companions("-INCLUDE 'syn3_a.sno'\n", cd, dd)
    got3 = sorted(p.name for p in dd.glob("syn3_*"))
    print("ARM2", got3 == ["syn3_a.sno", "syn3_b.sno", "syn3_c.sno"], got3)

    (cd/"syn_cyc_a.sno").write_text("-INCLUDE 'syn_cyc_b.sno'\n")
    (cd/"syn_cyc_b.sno").write_text("-INCLUDE 'syn_cyc_a.sno'\n")
    (cd/"syn_self.sno").write_text("-INCLUDE 'syn_self.sno'\n")
    t0 = time.time()
    H._copy_companions("-INCLUDE 'syn_cyc_a.sno'\n", cd, dd)
    H._copy_companions("-INCLUDE 'syn_self.sno'\n", cd, dd)
    dt = time.time() - t0
    gotc = sorted(p.name for p in dd.glob("syn_cyc_*"))
    self_ok = (dd/"syn_self.sno").is_file()
    print("ARM3", (gotc == ["syn_cyc_a.sno", "syn_cyc_b.sno"] and self_ok and dt < 5), gotc, self_ok, f"{dt:.3f}s")

    (cd/"syn_missing_a.sno").write_text("-INCLUDE 'syn_missing_ghost_does_not_exist.sno'\n")
    H._copy_companions("-INCLUDE 'syn_missing_a.sno'\n", cd, dd)
    a_ok = (dd/"syn_missing_a.sno").is_file()
    ghost_absent = not (dd/"syn_missing_ghost_does_not_exist.sno").exists()
    print("ARM4", (a_ok and ghost_absent), a_ok, ghost_absent)
PY
)"
echo "$out2" | sed 's/^/    /'
[[ "$(grep '^ARM2' <<<"$out2")" == "ARM2 True"* ]] && ck ok "3-level chain: all three files present (closure, not depth-2)" \
    || ck no "3-level chain incomplete -- $(grep '^ARM2' <<<"$out2")"
[[ "$(grep '^ARM3' <<<"$out2")" == "ARM3 True"* ]] && ck ok "mutual and self include cycles terminate promptly with both files present" \
    || ck no "cycle handling wrong -- $(grep '^ARM3' <<<"$out2")"
[[ "$(grep '^ARM4' <<<"$out2")" == "ARM4 True"* ]] && ck ok "unresolvable 2nd-level include: real file still copied, ghost not manufactured" \
    || ck no "refusal-safety violated -- $(grep '^ARM4' <<<"$out2")"

echo "--- ARM 5: the source names the mechanism, so a reviewer can find it ---"
grep -qE "transitive|closure" "$H" && ck ok "the harness source names the closure mechanism" || ck no "no transitive/closure mention in $H"

echo "------------------------------------------------------------"
if [ "$fails" -ne 0 ]; then echo "⛔ GATE FAIL: $fails of $checks check(s) failed"; exit 1; fi
echo "✅ GATE PASS: $checks/$checks checks"; exit 0
