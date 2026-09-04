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
outcome_ok = {}   # (test index in `tests`, mode) -> bool, additive: lets the bindings comparator below
                   # reuse this loop's own verdict instead of re-deriving it from the truncated `named`
                   # diagnostics. ⛔ Keyed by INDEX, not (fam, goal) text: 2 goal texts recur verbatim
                   # across distinct entries in this suite (measured: ('and',"'") x5, ('set_prolog_flag',
                   # 'X = "fred"') x3) -- a text key would silently collide and leak one entry's verdict
                   # onto another's lookup.
tmp = tempfile.mkdtemp()
prog = os.path.join(tmp, "t.pl")
for _tidx, (fam, goal, exp) in enumerate(tests):
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
        outcome_ok[(_tidx, mode)] = ok
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

# ⛔⭐ THE BINDINGS COMPARATOR (row prolog-inria-bindings-comparator-turns-the-outcome-class-upper-bound-into-a-true-score,
# seat05, 2026-09-04). THE ABOVE BOARD STAYS EXACTLY AS IT WAS -- not one line of it moved -- because the
# delta between it and this one IS the deliverable: how much of 268/445 was a goal reaching the right
# OUTCOME CLASS while binding the wrong thing. Method: reuse the engine's OWN unification/equality
# (`==/2`) rather than round-tripping through printed text, by splicing the suite's own Prolog syntax for
# each expected binding straight back into a comparison goal -- so quoting/formatting of atoms, numbers,
# lists and compound terms is never our problem, it is `==/2`'s. A goal with MULTIPLE declared solution
# sets (e.g. atom_concat(T1,T2,'hello') listing all 6 splits) is graded PASS if the first real solution
# (the runner already commits via `->`, matching the outcome-class check above) matches ANY one of the
# declared sets -- the suite does not mandate solution order for these, so requiring only the FIRST
# listed alternative would be a stricter and less correct reading than the suite intends.
def _mask_0c_lits(s):
    # ⛔ 0'c (a character-code literal, e.g. 0'a = 97) carries a single unpaired quote that desyncs a
    # naive quote-toggle scanner -- measured live on atom_codes/char_code. But a BLIND regex for "0'." is
    # a false-positive trap of its own: the quoted atom '0' immediately followed by another quoted element,
    # e.g. ...,'0']] (number_chars), contains the literal substring "0'" purely as the tail of '0' and the
    # head of the next token, with no character-literal there at all. So this has to be found IN CONTEXT,
    # walking quote-state properly, and only ever recognised while NOT already inside a quote. Returns
    # (masked_string, restore_dict) -- the mask is a placeholder with no quote/bracket/comma of its own,
    # safe to feed to the depth/comma scanners below; restore before use.
    out = []; ph = {}; i = 0; q = False; n = 0
    while i < len(s):
        ch = s[i]
        if not q and ch == "0" and i + 1 < len(s) and s[i+1] == "'":
            j = i + 4 if (i + 2 < len(s) and s[i+2] == "\\") else i + 3
            j = min(j, len(s))
            tok = s[i:j]
            key = "\x01%d\x01" % n; n += 1
            ph[key] = tok
            out.append(key)
            i = j
            continue
        if ch == "'":
            q = not q
        out.append(ch)
        i += 1
    return "".join(out), ph
def parse_bindings(e):
    # e is the raw expected-result text, e.g. "[[A <-- 'hello world']]" or
    # "[[T1 <-- '',T2 <-- 'hello'], [T1 <-- 'h',T2 <-- 'ello']]". Returns a list of solution-sets, each
    # a list of (varname, value_text) pairs -- or None if e is not a bindings declaration at all.
    e = e.strip()
    if not e.startswith("["): return None
    e, ph = _mask_0c_lits(e)
    body = e[1:-1].strip()  # strip the outer list brackets
    sols = []
    depth = 0; q = False; start = -1
    for i, ch in enumerate(body):
        if ch == "'" : q = not q
        if q: continue
        if ch == "[":
            if depth == 0: start = i
            depth += 1
        elif ch == "]":
            depth -= 1
            if depth == 0 and start >= 0:
                sols.append(body[start+1:i])
                start = -1
    pairs_all = []
    for sol in sols:
        pairs = []; depth = 0; q = False; cut = 0
        for i, ch in enumerate(sol + ","):
            if ch == "'": q = not q
            if q: continue
            if ch in "([{": depth += 1
            elif ch in ")]}": depth -= 1
            elif ch == "," and depth == 0:
                clause = sol[cut:i].strip() if i < len(sol) else sol[cut:].strip()
                cut = i + 1
                if not clause: continue
                m = re.split(r"\s*<--\s*", clause, maxsplit=1)
                if len(m) != 2:
                    return None  # malformed -- refuse to grade this entry as bindings rather than guess
                var, val = m[0].strip(), m[1].strip()
                for key, tok in ph.items():
                    var = var.replace(key, tok); val = val.replace(key, tok)
                pairs.append((var, val))
        pairs_all.append(pairs)
    return pairs_all if pairs_all else None
bres = {"m3": [0, 0], "m4": [0, 0]}   # pass, fail -- bindings board covers ALL 445 (non-bindings entries
                                        # inherit their outcome-class verdict: there is nothing finer to check)
bnamed = []
for _tidx, (fam, goal, exp) in enumerate(tests):
    want, wfun = expected_class(exp)
    sols = parse_bindings(exp) if want == "success" else None
    for mode in ("m3", "m4"):
        if sols is None:
            # Nothing finer to check than outcome class -- inherit that verdict directly (never re-derive
            # it from the truncated `named` diagnostics, which can collide on goal[:28]).
            bres[mode][0 if outcome_ok.get((_tidx, mode), False) else 1] += 1
            continue
        disj = " ; ".join(("(" + ",".join("%s == %s" % (v, t) for v, t in sol) + ")") if sol else "true" for sol in sols)
        with open(prog, "w") as f:
            f.write(":- catch( ( %s -> ( (%s) -> write('@BOK') ; write('@BFAIL') ) ; write('@BNO') ), E, ( write('@BER('), write(E), write(')') ) ), nl.\n"
                     % (goal, disj))
        try:
            if mode == "m3":
                r = subprocess.run([scrip, prog], capture_output=True, text=True, timeout=10, stdin=subprocess.DEVNULL, cwd=tmp)
            else:
                s = os.path.join(tmp, "t.s"); b = os.path.join(tmp, "t.bin")
                c = subprocess.run([scrip, "--compile", "-o", s, prog], capture_output=True, text=True, timeout=10, stdin=subprocess.DEVNULL, cwd=tmp)
                if c.returncode != 0: bres[mode][1] += 1; bnamed.append("%s:%s:m4:NOBUILD" % (fam, goal[:28])); continue
                g = subprocess.run(["gcc", "-m64", "-no-pie", s, "-o", b, "-L", os.path.join(os.path.dirname(scrip), "out"),
                                    "-lscrip_rt", "-Wl,-rpath," + os.path.join(os.path.dirname(scrip), "out"), "-lm"],
                                   capture_output=True, text=True, timeout=60, cwd=tmp)
                if g.returncode != 0: bres[mode][1] += 1; bnamed.append("%s:%s:m4:NOLINK" % (fam, goal[:28])); continue
                r = subprocess.run([b], capture_output=True, text=True, timeout=10, stdin=subprocess.DEVNULL, cwd=tmp)
        except subprocess.TimeoutExpired:
            bres[mode][1] += 1; bnamed.append("%s:%s:%s:TIMEOUT" % (fam, goal[:28], mode)); continue
        o = r.stdout
        if "@BOK" in o: bres[mode][0] += 1
        else:
            bres[mode][1] += 1
            reason = "outcome-already-red" if "@BNO" in o or "@BER" in o else "wrong-bindings"
            bnamed.append("%s:%s:%s:%s(%s)" % (fam, goal[:28], mode, reason, o.strip()[:40]))
print("INRIA_SUITE_BINDINGS_BOARD total=%d m3_pass=%d m3_fail=%d m4_pass=%d m4_fail=%d"
      % (len(tests), bres["m3"][0], bres["m3"][1], bres["m4"][0], bres["m4"][1]))
print("  criterion: OUTCOME CLASS AND substitution bindings (== against the suite's own declared [[Var <-- Value]] sets, any ONE declared solution set accepted) -- the suite's own criterion, never weaker")
print("  delta vs OUTCOME-CLASS-ONLY board: m3 %+d  m4 %+d  (goals that reached the right outcome while binding the wrong thing)"
      % (bres["m3"][0] - res["m3"][0], bres["m4"][0] - res["m4"][0]))
if os.environ.get("INRIA_NAME_REDS"):
    for x in bnamed[:80]: print("    B:" + x)
open(os.path.join(tmp, "bindings_board"), "w").write("%d %d %d" % (len(tests), bres["m3"][0], bres["m4"][0]))
print("BINDINGS_BOARD_FOR_SHELL %d %d %d" % (len(tests), bres["m3"][0], bres["m4"][0]))
PY
