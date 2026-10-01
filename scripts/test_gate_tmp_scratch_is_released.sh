#!/usr/bin/env bash
# test_gate_tmp_scratch_is_released.sh -- every scratch a script makes under /tmp is released by that script (ceo 2026-10-01,
# Lon: "find out why /tmp keeps filling and fix the root problem"). Measured that morning on a 63 GB /tmp at 90%: 6957
# top-level entries, 4466 of them older than 48 h, from Python censuses that mkdtemp'd and never rmtree'd (gc_cov 754,
# c_alloc_* 2740, suite_pop_diff 481, ...) and shell bodies whose work dir outlived a signal. Two static arms, NO BUILD:
#   ARM 1  every tempfile.mkdtemp( / mkstemp( in scripts/*.py either registers its own removal ON THE SAME LINE
#          (atexit ... rmtree / TemporaryDirectory) or sits in a file carrying at least as many rmtree/unlink/remove calls
#          as creations. Offenders must be 0 -- an absolute, because the cure is one clause per site.
#   ARM 2  scripts/*.sh calling mktemp with no `trap` anywhere in the file: a RATCHET, at most CEIL (lowered, never raised).
# SELF-PROOF first, on fixtures under mktemp: a planted bare mkdtemp reads 1 offender, a planted registering line reads 0.
set -u
HERE="$(cd "$(dirname "$0")" && pwd)"
CEIL=73   # measured 2026-10-01 at SCRIP c798131e2 after two cures; lower it when you cure one, never raise it
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT; trap 'rm -rf "$W"; exit 143' TERM; trap 'rm -rf "$W"; exit 130' INT
fail=0; ck() { if [ "$1" = ok ]; then echo "  ✅ $2"; else echo "  ⛔ $2"; fail=$((fail+1)); fi; }
py_offenders() { # py_offenders <dir> -> prints "file:line" per offending creation
  local f n_make n_free
  for f in "$1"/*.py; do [ -f "$f" ] || continue
    n_make=$(grep -cE 'tempfile\.mkdtemp\(|tempfile\.mkstemp\(|_tf\.mkdtemp\(|_tf\.mkstemp\(' "$f")
    [ "$n_make" = 0 ] && continue
    n_free=$(grep -cE 'rmtree\(|os\.unlink\(|os\.remove\(|_os\.unlink\(|TemporaryDirectory\(' "$f")
    [ "$n_free" -ge "$n_make" ] && continue
    grep -nE 'tempfile\.mkdtemp\(|tempfile\.mkstemp\(|_tf\.mkdtemp\(|_tf\.mkstemp\(' "$f" | grep -vE 'atexit|rmtree|TemporaryDirectory|unlink' | sed "s#^#$f:#"
  done
}
sh_untrapped() { local f n=0; for f in "$1"/*.sh; do [ -f "$f" ] || continue; grep -q 'mktemp' "$f" || continue; grep -q 'trap ' "$f" && continue; n=$((n+1)); done; echo "$n"; }
echo "--- SELF-PROOF on fixtures ---"
mkdir -p "$W/a" "$W/b"
printf 'import tempfile\nd = tempfile.mkdtemp(prefix="x.")\nprint(d)\n' > "$W/a/leak.py"
printf 'import tempfile\nd = tempfile.mkdtemp(prefix="x."); __import__("atexit").register(__import__("shutil").rmtree, d, True)\nprint(d)\n' > "$W/b/kept.py"
n=$(py_offenders "$W/a" | wc -l); ck "$([ "$n" = 1 ] && echo ok || echo no)" "a planted bare mkdtemp is ONE offender (got $n)"
n=$(py_offenders "$W/b" | wc -l); ck "$([ "$n" = 0 ] && echo ok || echo no)" "a planted mkdtemp that registers its removal is NO offender (got $n)"
printf 'W=$(mktemp -d)\n' > "$W/a/leak.sh"; printf 'W=$(mktemp -d); trap "rm -rf $W" EXIT\n' > "$W/b/kept.sh"
ck "$([ "$(sh_untrapped "$W/a")" = 1 ] && [ "$(sh_untrapped "$W/b")" = 0 ] && echo ok || echo no)" "a planted untrapped mktemp script counts 1, a trapped one 0"
echo "--- ARM 1: Python creations in scripts/ that register no removal ---"
off="$(py_offenders "$HERE")"; n=$(printf '%s' "$off" | grep -c .)
ck "$([ "$n" = 0 ] && echo ok || echo no)" "Python mkdtemp/mkstemp sites with no removal: $n (want 0)"; [ "$n" = 0 ] || printf '%s\n' "$off" | head -20
echo "--- ARM 2: shell scripts calling mktemp with no trap (ratchet) ---"
n=$(sh_untrapped "$HERE"); ck "$([ "$n" -le "$CEIL" ] && echo ok || echo no)" "untrapped mktemp shell scripts: $n (ceiling $CEIL)"
[ "$n" -lt "$CEIL" ] && echo "  ⚠ the count fell to $n: lower CEIL in this gate to ratchet it"
[ "$fail" = 0 ] && { echo "✅ GATE OK: every Python scratch registers its release; untrapped shell mktemp is at or under its ceiling"; exit 0; }
echo "⛔ GATE RED: $fail arm(s) failed"; exit 1
