#!/usr/bin/env python3
"""util_apply_moderef.py -- render the CEO-581 PER-MODE REF of ONE entry for ONE mode, on stdout.

⛔⭐ THIS IS A SHIM OVER corpus_suite_harness.py, NEVER A SECOND IMPLEMENTATION.  It imports that file's own
read_moderef_sidecar(), moderefs_for() and apply_moderef(), so a bash runner and the python harness resolve a
per-mode ref by the SAME rules: the same sidecar derivation (<stem>.moderef beside <stem>.ref), the same
mandatory receipt, the same exactly-one-occurrence requirement and the same majority refusal.  ⭐ THE REASON IT
IS A SHIM IS THE REASON util_apply_ceo409_mask.py IS ONE, and this lane paid for the lesson on 2026-09-11: when
"does this program have stdin?" was written down three times it had three different answers, and the two that
were wrong were wrong silently.  A per-mode ref that means one thing to the master and another to a package
runner is that defect with a new name.

Usage:  util_apply_moderef.py [--sidecar <ALL.moderef>] <ref-path> <entry-name> <mode> [count-out-path]
        > per-mode-expected.  --sidecar names the declaration file for the one caller that cannot derive it:
        an entry EXTRACTED from a master container into a tempdir, whose declarations live beside the
        container.  Copying the sidecar next to the extraction instead would be a second copy that drifts.
Writes the ref with every declared line for <mode> substituted (byte-identical to the file when nothing is
declared, which is the overwhelmingly common case) and, if a count path is given, the number of lines
substituted -- a per-mode ref that nobody can count is a per-mode ref nobody can audit.  REFUSES rc=2 on a
malformed sidecar or a declaration whose ref-line is absent or ambiguous, which is the harness's own refusal
and not a copy of its policy.
"""
import importlib.util, sys, pathlib
def refuse(msg):
    sys.stderr.write("REFUSE(rc=2): util_apply_moderef.py: %s\n" % msg); sys.exit(2)
argv, sidecar = sys.argv[1:], None
if argv and argv[0] == "--sidecar":
    if len(argv) < 2:
        refuse("--sidecar needs a path")
    sidecar, argv = argv[1], argv[2:]
if len(argv) not in (3, 4):
    refuse("needs [--sidecar <path>] <ref-path> <entry-name> <mode> [count-out-path]; got %d positional "
           "argument(s)" % len(argv))
ref_path, entry, mode = argv[0], argv[1], argv[2]
count_out = argv[3] if len(argv) == 4 else None
h = pathlib.Path(__file__).resolve().parent / "corpus_suite_harness.py"
if not h.is_file():
    refuse("corpus_suite_harness.py is not beside this file (%s) -- the shared body moved; re-point rather than "
           "re-implement, because a second per-mode ref reader is a second answer to one question" % h)
spec = importlib.util.spec_from_file_location("_csh", str(h))
mod = importlib.util.module_from_spec(spec)
try:
    spec.loader.exec_module(mod)
except SystemExit:
    raise
except Exception as e:
    refuse("cannot import corpus_suite_harness.py: %s: %s" % (type(e).__name__, e))
for fn in ("read_moderef_sidecar", "moderefs_for", "apply_moderef"):
    if not hasattr(mod, fn):
        refuse("corpus_suite_harness.py has no %s() -- the per-mode ref mechanism was renamed or removed; re-derive "
               "this shim rather than grade a name that no longer exists" % fn)
if not pathlib.Path(ref_path).is_file():
    refuse("%s is not a file -- the ref a per-mode ref is cut FROM must exist before it can be rendered" % ref_path)
decls = mod.read_moderef_sidecar(ref_path, sidecar=sidecar)
rows = mod.moderefs_for(decls, entry, mode)
# ⛔⭐ BYTES IN, THE SAME BYTES OUT -- surrogateescape, NOT errors="replace", the identical argument
# util_apply_ceo409_mask.py records: this sits in the GRADING path, so anything it does to a byte it was not
# asked to substitute is a verdict change nobody asked for, and "replace" would rewrite every non-UTF-8 byte
# to U+FFFD so two streams differing only in bytes it mangled would compare EQUAL.
text = pathlib.Path(ref_path).read_bytes().decode("utf-8", "surrogateescape")
out, n = mod.apply_moderef(text, rows, where="%s [%s/%s]" % (pathlib.Path(ref_path).name, entry, mode))
if count_out:
    pathlib.Path(count_out).write_text(str(n))
sys.stdout.buffer.write(out.encode("utf-8", "surrogateescape"))
