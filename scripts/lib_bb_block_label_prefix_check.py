#!/usr/bin/env python3
"""lib_bb_block_label_prefix_check.py -- worker for test_gate_bb_block_label_prefix.sh (row
bb-label-prefix-uniform). See that script's header for the ruling this enforces; this file is the
bracket-and-check walk, kept separate so it stays independently testable
(`python3 lib_bb_block_label_prefix_check.py some.s`, prints violations, exit 0/1).

MERGED DESIGN (seat02 2026-08-29, per hq_P's task-LEDGER ruling "SEAT11 COLLISION ADJUDICATED ... THE
RULING: MERGE, DO NOT PICK"): bracketing/ownership-tracking below is seat11's classify() (recovered at
`git show 57ecf03e:scripts/test_gate_bb_block_label_prefix.sh`) -- it treats a bareword Greek-suffixed
landing pad with no "n<uid>_" prefix (PATTERN_BT_α, a DEFINE'd proc's own by-name entry point,
bb_define.cpp) as PRESERVING the enclosing box's identity rather than opening a new block. seat05's
original bracketing here treated any "<word>_<greek>" as a new owner and false-flagged 7 labels on the
pattern_bt.sno witness (measured, hq_P: "NO n<digits>_<kind>_<greek> box port opens between n47_define_*
and n48_statement_end_α -- the whole span is one box's output"). seat11's classifier, unmodified, does
NOT enforce a Greek letter on the _as/_af/_ry/_rt/_s<N> port-target families at all (ceo-endorsed
2026-08-29c: these ARE real box-owned gamma/omega jump targets -- emit.cpp node_γ/node_ω assignment at
~3074/3090/1398/3095 -- not siblings of the exempt _bx range-marker), so that requirement is added below
as its OWN independent check, per hq_P's explicit "add seat05's greek requirement ... as a clearly
separated second check" -- not folded into the bracketing walk, so neither arm's logic has to reason
about the other.

⛔ UTF-8 DISCIPLINE (RULES.md INSTRUMENT LAW #15, this row's own census tool header, seat03 2026-08-29):
match on decoded Greek characters, never their UTF-8 byte sequence via an ASCII-only regex -- that class
of bug already cost this row one full false census once.
"""
import re
import sys

GREEK = "αβγω"  # α β γ ω
GREEK_SET = set(GREEK)
LABEL_DEF = re.compile(r'^([^\s:]+):(?:\s|$)')
# A real box port: "n<uid>_<kind>_<greek>" (emit_label_alloc("n%d_%s_<greek>", uid, kind)). kind may
# itself contain underscores ("match_alternate"), so it is everything between the uid and the final
# Greek segment -- NOT split on the kind's own underscores (that produced a phantom "kind='BT'" block on
# an earlier version of this script, caught testing against pattern_bt.sno; PATTERN_BT has no uid prefix
# at all, see BAREWORD_GREEK below).
PORT_LABEL = re.compile(r'^n(\d+)_(.+)_([' + GREEK + r'])$')
BOXFAM = re.compile(r'^n\d+_')                                   # this box's OWN family (_bx/_as/_af/...)
BAREWORD_GREEK = re.compile(r'^[A-Za-z_][A-Za-z0-9_$]*_([' + GREEK + r'])$')
FAMILY_SUFFIX = re.compile(r'_(as|af|ry|rt|s\d+)$')               # the port-target families needing Greek

# Ruled exempt (ceo 2026-08-29b/c, hq_P 2026-08-29 -- mechanism test: "a greek infix names a PORT OF A
# BOX; a label with no owning box has no port, so the infix would not be redundant, it would be FALSE").
# Carried over unchanged from the pre-merge gate's allowlist, same citations -- an allowlist entry is a
# claim, not a convenience; don't add one here without a citation and a task-LEDGER entry.
EXEMPT = [
    (re.compile(r'^n\d+_[A-Za-z0-9_]+_bx$'),
     "box-span ELF .type@function/.size debug marker (emit.cpp bxs[], ~line 2977) -- spans the box's "
     "entire range, never a jump target; ceo-ruled exempt 2026-08-29b"),
    (re.compile(r'^\.S\d+$'),
     "module-level string dedup table (emit.cpp strtab_label) -- shared across boxes, no single owner; "
     "ceo-ruled exempt 2026-08-29b"),
    (re.compile(r'^\.C\d+$'),
     "module-level cset dedup table (emit.cpp csettab_label) -- same as .S<N>; ceo-ruled exempt"),
    (re.compile(r'^(RETURN|FRETURN|NRETURN)$'),
     "per-function shared exit label (emit.cpp emit_floater_label) -- one level above any single box; "
     "ceo-ruled exempt 2026-08-29b"),
    (re.compile(r'^FN__'),
     "function/graph entry point, called from outside the graph -- same class as RETURN-family; "
     "ceo-ruled exempt 2026-08-29b"),
    (re.compile(r'^[A-Za-z_][A-Za-z0-9_$]*_res$'),
     "per-function result-cell label -- same function-level class as RETURN/FN__"),
    (re.compile(r'^(main|module_init|__gva_names)$'),
     "program-level entry/init/table symbols -- same function/module-level class as RETURN/FN__/.S/.C"),
    (re.compile(r'^[A-Za-z_][A-Za-z0-9_$]*_α_body$'),
     "function/graph body-entry label (emit.cpp lbl_α_body, fam is the GRAPH name, ~line 2665) -- one "
     "level above any box"),
    (re.compile(r'^\.Lgvan\d+$'),
     "driver-level GVA name table (src/driver/scrip.c, not any bb_*/xa_*/emit.cpp box loop) -- no owning "
     "box, no port to name; hq_P 2026-08-29, ceo-endorsed"),
    (re.compile(r'^\.Lstartup_(pname|prec|pnames)\d+$'),
     "driver-level per-procedure startup/reflection table (src/driver/scrip.c) -- same ruling as .Lgvan"),
    (re.compile(r'^\.Lseala\d+$'),
     "driver-level rt_proc_seal_alpha startup table (src/driver/scrip.c) -- same ruling as .Lgvan"),
]


def exempt(name):
    for pat, _why in EXEMPT:
        if pat.match(name):
            return True
    return False


def classify(name):
    """('anchor', kind) opens a new block; 'preserve' leaves the current owner unchanged; 'reset' clears
    it (genuinely no box owner here); 'data' is content, checked against the current owner."""
    pm = PORT_LABEL.match(name)
    if pm:
        return 'anchor', pm.group(2)
    if BOXFAM.match(name):
        return 'preserve', None            # n<uid>_<kind>_bx/_as/_af/_ry/_rt/_s<N>: already self-owned
    if not name.startswith('.'):
        if BAREWORD_GREEK.match(name):
            return 'preserve', None        # PATTERN_BT_α / Push_γ: a by-name landing pad embedded inside
                                            # the currently-dispatching box's own output (bb_define.cpp
                                            # composes these from the proc name, not the n<uid>_<kind>
                                            # port loop) -- not a new box scope. SEAT11 COLLISION (task
                                            # LEDGER): measured on pattern_bt.s, requiring these to open
                                            # their own block false-flags the enclosing box's real content.
        return 'reset', None               # module_init, a bare proc/label name, ...: no box owner here
    return 'data', None


def check(path):
    violations = []
    greek_missing = []
    owner = None
    n_checked = 0
    with open(path, encoding='utf-8') as f:
        for lineno, line in enumerate(f, 1):
            m = LABEL_DEF.match(line.rstrip('\n'))
            if not m:
                continue
            name = m.group(1)
            n_checked += 1
            if FAMILY_SUFFIX.search(name) and not (GREEK_SET & set(name)):
                greek_missing.append((lineno, name))
            if exempt(name):
                continue
            kind, new_owner = classify(name)
            if kind == 'anchor':
                owner = new_owner
            elif kind == 'reset':
                owner = None
            elif kind == 'data':
                if owner is not None and owner not in name:
                    violations.append((lineno, name, owner))
            # 'preserve': owner unchanged, not checked
    return violations, greek_missing, n_checked


def main(argv):
    if len(argv) != 1:
        print("usage: lib_bb_block_label_prefix_check.py file.s", file=sys.stderr)
        return 2
    violations, greek_missing, n_checked = check(argv[0])
    ok = True
    if violations:
        ok = False
        print(f"⛔ {len(violations)} label(s) inside a block not carrying that block's own owner, in {argv[0]}:", file=sys.stderr)
        for lineno, name, owner in violations[:40]:
            print(f"    line {lineno}: {name}  (inside block owner={owner!r})", file=sys.stderr)
        if len(violations) > 40:
            print(f"    ... and {len(violations) - 40} more", file=sys.stderr)
    if greek_missing:
        ok = False
        print(f"⛔ {len(greek_missing)} port-target label(s) missing a Greek letter (_as/_af/_ry/_rt/_s<N> family, ceo-endorsed 2026-08-29c), in {argv[0]}:", file=sys.stderr)
        for lineno, name in greek_missing[:40]:
            print(f"    line {lineno}: {name}", file=sys.stderr)
        if len(greek_missing) > 40:
            print(f"    ... and {len(greek_missing) - 40} more", file=sys.stderr)
    print(n_checked)
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
