#!/usr/bin/env bash
# test_prolog_inria_suite.sh — THE ISO/IEC 13211-1 PROLOG DENOMINATOR (row prolog-iso-conformance-inria-suite-vendored-and-graded).
# Lon 2026-09-03 20:45: "100% means 100% of the industry standard language." For Prolog the standard is ISO/IEC 13211-1
# and the public suite that grades it is the INRIA suite (Deransart / Ed-Dbali / Cervoni), vendored at
# corpus/packages/prolog/inriasuite (see its README and PROVENANCE.txt). 445 goals across 67 builtin-family files.
#
# ⛔⭐ WHAT THIS GRADES, AND WHAT IT DOES NOT -- stated here and in the board line, because a conformance number that
# overstates itself is worse than no number. It compares the OUTCOME CLASS of each goal: success vs failure vs error,
# and for an error the ERROR TERM'S FUNCTOR (type_error, instantiation_error, ...). It does NOT compare substitution
# bindings, so a goal expected to yield [[X <-- a]] scores as a pass if it merely SUCCEEDS, whatever it bound.
# That is strictly weaker than the suite's own criterion. Tightening it can only ever move the number DOWN.
set -u
GATE_NAME=test_prolog_inria_suite
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="${S4E_HOME:-$(cd "$HERE/../.." && pwd)}"
SUITE="$ROOT/corpus/packages/prolog/inriasuite"
SCRIP="$HERE/../scrip"
refuse() { echo "⛔ REFUSED(2) [$GATE_NAME]: $*" >&2; exit 2; }
[ -d "$SUITE" ] || refuse "no vendored suite at $SUITE -- a suite that is absent is not a suite that is failing"
[ -x "$SCRIP" ] || refuse "no scrip binary at $SCRIP -- run make first; a missing binary prints a full, plausible, entirely false all-FAIL board"
export INRIA_SUITE="$SUITE" INRIA_SCRIP="$SCRIP"
# ⛔ THE HEREDOC DELIMITER IS QUOTED and every value crosses by ENVIRONMENT, never by interpolation. This repo has
# measured the alternative three times in one day: an unquoted heredoc hands the shell the whole program and every
# backtick in it runs as a command. The trap lives in the medium, not in the language you think you are writing.
python3 - <<'PY'
import os, re, subprocess, tempfile, sys
suite = os.environ["INRIA_SUITE"]; scrip = os.environ["INRIA_SCRIP"]
def expected_class(exp):
    e = exp.strip()
    if e == "success": return ("success", None)
    if e == "failure": return ("failure", None)
    if e.startswith("["):  return ("success", None)   # a substitution set implies success (weaker, see header)
    m = re.match(r"([a-z_]+)", e)
    return ("error", m.group(1) if m else None)
def parse(path):
    # ⛔⭐ ENTRIES SPAN LINES, AND ASSUMING THEY DO NOT SILENTLY SHRINKS THE DENOMINATOR. The first cut took only
    # lines that both started "[" and ended "]." and reported total=416 -- a clean, plausible board that had
    # quietly dropped ~50 goals, all of them the LONG ones (sub_atom 25 starts / 13 one-liners, functor-bis 19/12,
    # atom_concat 15/9). Those are exactly the hard cases, so the dropped set was biased toward what we are worst
    # at, which flatters the score twice over. Accumulate until the bracket depth closes.
    # ⛔ STRIP THE TRAILING % COMMENT FIRST -- but only one that is OUTSIDE quotes, since % is legal inside a
    # quoted atom. Measured: 13 of the 67 files carry an explanatory comment after the closing "]." (the `fail`
    # family annotates two of its four goals), and requiring the raw line to END with "]." dropped every one of
    # them. The suite's authors comment the SUBTLE cases, so a comment-blind parser again drops exactly the
    # goals that matter most.
    def decomment(t):
        q = False; out = []
        for ch in t:
            if ch == "'": q = not q
            if ch == "%" and not q: break
            out.append(ch)
        return "".join(out).strip()
    out = []; buf = ""
    for raw in open(path, errors="replace"):
        line = decomment(raw.strip())
        if not buf and not line.startswith("["): continue
        buf = (buf + " " + line).strip() if buf else line
        if buf.count("[") > buf.count("]") or not buf.endswith("]."): continue
        body = buf[1:-2]; buf = ""
        depth = 0; cut = -1
        for i, ch in enumerate(body):
            if ch in "([{": depth += 1
            elif ch in ")]}": depth -= 1
            elif ch == "," and depth == 0: cut = i; break
        if cut < 0: continue
        out.append((body[:cut].strip(), body[cut+1:].strip()))
    return out
tests = []
for fn in sorted(os.listdir(suite)):
    p = os.path.join(suite, fn)
    if not os.path.isfile(p) or fn.endswith((".md", ".txt", ".pl")): continue
    for goal, exp in parse(p): tests.append((fn, goal, exp))
if not tests:
    sys.stderr.write("⛔ REFUSED(2) [test_prolog_inria_suite]: parsed ZERO goals from %s -- refusing to print a board with no denominator\n" % suite)
    raise SystemExit(2)
res = {"m3": [0, 0, 0], "m4": [0, 0, 0]}   # pass, fail, crash
named = []
tmp = tempfile.mkdtemp()
prog = os.path.join(tmp, "t.pl")
for fam, goal, exp in tests:
    want, wfun = expected_class(exp)
    with open(prog, "w") as f:
        f.write(":- catch( ( %s -> write('@OK') ; write('@NO') ), E, ( write('@ER('), write(E), write(')') ) ), nl.\n" % goal)
    for mode in ("m3", "m4"):
        try:
            if mode == "m3":
                r = subprocess.run([scrip, prog], capture_output=True, text=True, timeout=10, stdin=subprocess.DEVNULL, cwd=tmp)
            else:
                s = os.path.join(tmp, "t.s"); b = os.path.join(tmp, "t.bin")
                c = subprocess.run([scrip, "--compile", "-o", s, prog], capture_output=True, text=True, timeout=10, stdin=subprocess.DEVNULL, cwd=tmp)
                if c.returncode != 0: res[mode][1] += 1; named.append("%s:%s:m4:NOBUILD" % (fam, goal[:28])); continue
                g = subprocess.run(["gcc", "-m64", "-no-pie", s, "-o", b, "-L", os.path.join(os.path.dirname(scrip), "out"),
                                    "-lscrip_rt", "-Wl,-rpath," + os.path.join(os.path.dirname(scrip), "out"), "-lm"],
                                   capture_output=True, text=True, timeout=60, cwd=tmp)
                if g.returncode != 0: res[mode][1] += 1; named.append("%s:%s:m4:NOLINK" % (fam, goal[:28])); continue
                r = subprocess.run([b], capture_output=True, text=True, timeout=10, stdin=subprocess.DEVNULL, cwd=tmp)
        except subprocess.TimeoutExpired:
            res[mode][2] += 1; named.append("%s:%s:%s:TIMEOUT" % (fam, goal[:28], mode)); continue
        o = r.stdout
        if   "@OK" in o: got, gfun = "success", None
        elif "@NO" in o: got, gfun = "failure", None
        elif "@ER" in o:
            got = "error"
            mm = re.search(r"@ER\(\s*([a-z_]+)", o); gfun = mm.group(1) if mm else None
        else:
            res[mode][1] += 1; named.append("%s:%s:%s:NO-CLASS" % (fam, goal[:28], mode)); continue
        ok = (got == want) and (want != "error" or wfun is None or gfun == wfun)
        if ok: res[mode][0] += 1
        else:
            res[mode][1] += 1
            named.append("%s:%s:%s:want=%s/%s got=%s/%s" % (fam, goal[:28], mode, want, wfun, got, gfun))
print("INRIA_SUITE_BOARD total=%d m3_pass=%d m3_fail=%d m3_crash=%d m4_pass=%d m4_fail=%d m4_crash=%d"
      % (len(tests), res["m3"][0], res["m3"][1], res["m3"][2], res["m4"][0], res["m4"][1], res["m4"][2]))
print("  criterion: OUTCOME CLASS (success/failure/error + error functor); substitution bindings NOT compared -- strictly weaker than the suite's own")
if os.environ.get("INRIA_NAME_REDS"):
    for x in named[:60]: print("    " + x)
open(os.path.join(tmp, "board"), "w").write("%d %d %d" % (len(tests), res["m3"][0], res["m4"][0]))
print("BOARD_FOR_SHELL %d %d %d" % (len(tests), res["m3"][0], res["m4"][0]))
PY
