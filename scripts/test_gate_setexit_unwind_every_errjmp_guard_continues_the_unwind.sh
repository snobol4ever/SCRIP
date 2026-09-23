#!/usr/bin/env bash
# test_gate_setexit_unwind_every_errjmp_guard_continues_the_unwind.sh -- row snobol4-a-setexit-handler-runs-at-top-level-so-
# freturn-from-it-is-error-242 (hq_snobol4 2026-09-23, CEO-1215/1218). A SETEXIT handler runs NESTED inside core_runtime_error,
# so a RETURN or FRETURN out of it has to get back to the activation that raised the error. core_setexit_handler_return does
# that by unwinding g_core_errjmp_stk TOP-DOWN: it longjmps to the innermost guard, that guard runs ITS OWN cleanup (restoring
# g_core_errjmp_n, g_error, _setexit_resume, the Icon op context, the builtin mark, EVAL_TMP), and then calls
# core_unwind_pending(), which longjmps to the next guard down, until the activation's recorded floor is reached and
# rt_unwind_to_activation jumps through the activation's own wire.
#
# ⛔ THE CHAIN IS ONLY AS STRONG AS ITS WEAKEST GUARD, AND A MISSING LINK IS SILENT. A guard whose catch path does not call
# core_unwind_pending() swallows the unwind: its caller sees an ordinary FAIL, the program runs on in a frame that should have
# returned, and the level record is left marked "unwinding" -- so the NEXT unrelated error caught at that level resumes the
# abandoned unwind and jumps to an activation that may be long gone. Nothing crashes where the link is missing. This gate
# holds every setjmp(g_core_errjmp_stk...) site in src/ to the protocol, so a guard added later cannot silently break it.
#
# THE ONE STRUCTURAL EXCEPTION, held rather than exempted: eval_chain_run_guarded's cleanup of EVAL_TMP and of the chain pool
# lives in its CALLERS (eval_string_transient restores EVAL_TMP and releases or caches the chain after it returns 0), so the
# continuation belongs there -- every call of eval_chain_run_guarded must be followed by core_unwind_pending() on its fail path.
#
# NO BUILD NEEDED (pure source census, <1s). rc=0 clean · rc=1 a guard breaks the chain · rc=2 REFUSAL (blind census).
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; SD="$(cd "$HERE/.." && pwd)"
[ -d "$SD/src" ] || { echo "REFUSE(2): no $SD/src"; exit 2; }
python3 - "$SD/src" <<'PY'
import re, sys, pathlib
src = pathlib.Path(sys.argv[1])
WINDOW = 12
sites, bad, callers = [], [], []
def enclosing_fn(lines, i):
    for j in range(i, -1, -1):
        m = re.match(r'^[A-Za-z_][\w \*\(\)]*?\b(\w+)\s*\([^;]*$', lines[j])
        if m and not lines[j].startswith((' ', '\t', '#', '"')):
            return m.group(1)
    return "?"
for p in sorted(list(src.rglob("*.c")) + list(src.rglob("*.cpp"))):
    lines = p.read_text(encoding="utf-8", errors="replace").split("\n")
    rel = str(p.relative_to(src.parent))
    for i, ln in enumerate(lines):
        if "setjmp(g_core_errjmp_stk" in ln:
            fn = enclosing_fn(lines, i)
            sites.append((rel, i + 1, fn))
            if fn == "eval_chain_run_guarded":
                continue
            if not any("core_unwind_pending" in l for l in lines[i:i + WINDOW]):
                bad.append("%s:%d (%s): the catch path never calls core_unwind_pending() within %d lines" % (rel, i + 1, fn, WINDOW))
        if re.search(r'\beval_chain_run_guarded\s*\(', ln) and "static int eval_chain_run_guarded" not in ln:
            callers.append((rel, i + 1))
            if not any("core_unwind_pending" in l for l in lines[i:i + WINDOW]):
                bad.append("%s:%d: a caller of eval_chain_run_guarded does not continue the unwind after EVAL's own cleanup" % (rel, i + 1))
print("g_core_errjmp_stk guards in src/ -- each catch path must continue a SETEXIT handler-return unwind")
for rel, ln, fn in sites:
    print("  %-40s %-32s %s" % ("%s:%d" % (rel, ln), fn, "(cleanup in its callers: %d call site(s) held)" % len(callers) if fn == "eval_chain_run_guarded" else ""))
if len(sites) < 6:
    print("⛔ REFUSES rc=2: only %d guard site(s) found; the protocol was written against six (EVAL, two arithmetic, by-name dispatch," % len(sites))
    print("   const-fold, two SETEXIT). A shrinking population is a blind census, not a clean one.")
    sys.exit(2)
if not callers:
    print("⛔ REFUSES rc=2: eval_chain_run_guarded has no callers in view -- the one structural exception cannot be held.")
    sys.exit(2)
if bad:
    print("\n⛔ GATE FAILED: %d link(s) break the unwind chain:" % len(bad))
    for b in bad:
        print("   " + b)
    print("   ⭐ THE CURE: after the guard's own cleanup and before it returns, call core_unwind_pending(); it is a no-op unless an")
    print("      unwind is in progress at the current level.")
    sys.exit(1)
print("\n✅ GATE OK: %d guard site(s), every catch path continues the unwind (%d EVAL caller(s) held in place of their guard)." % (len(sites), len(callers)))
PY
exit $?
