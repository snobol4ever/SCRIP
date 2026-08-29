#!/usr/bin/env python3
"""lib_bb_block_label_prefix_check.py -- worker for test_gate_bb_block_label_prefix.sh (row
bb-label-prefix-uniform). See that script's header for the ruling this enforces and the allowlist's
citations; this file is the actual bracket-and-check walk, kept separate so it stays independently
testable (`python3 lib_bb_block_label_prefix_check.py some.s`, prints violations, exit 0/1).

⛔ UTF-8 DISCIPLINE (RULES.md INSTRUMENT LAW #15, this row's own census tool header, seat03 2026-08-29):
match on decoded Greek characters, never on their UTF-8 byte sequence via an ASCII-only regex -- that
class of bug already cost this row one full false census once.

Bracketing: a box's own port-label DEFINITION line (bare `n<uid>_<kind>_<greek>:`, no leading dot -- the
label emit_label_alloc("n%d_%s_<greek>", uid, kind) mints, emit.cpp:2966/2970 and siblings) opens a new
"current block" of (kind, greek). Every OTHER label definition seen before the next port-label line is
checked against that block: conforming shapes are the block's own internal-label convention
(".L<kind>_<greek>_..." -- what x86_internal_resolve() produces, x86_asm.h) or an allowlisted family.
KNOWN LIMITATION: a box whose port label is overridden to a bare exempt name (emit_floater_label /
balias, emit.cpp:2967-2968) does not match the port-label pattern, so it does not open a new tracked
block -- labels inside such a box are checked against whatever block preceded it. Not hit by the
witness set this gate runs today; flagged here rather than silently assumed away.
"""
import re
import sys

GREEK = "αβγω"  # α β γ ω
LABEL_DEF = re.compile(r'^([^\s:]+):(?:\s|$)')
PORT_LABEL = re.compile(r'^([A-Za-z_][A-Za-z0-9_$]*)_([' + GREEK + r'])$')
# A box-level port label is "n<uid>_<kind>_<greek>" -- strip ONLY that leading "n<digits>_" to get kind.
# A function/graph-level bare port label (a "bare chain" nested/inline graph, e.g. "PATTERN_BT_α") has NO
# such numeric-uid prefix and IS its own kind whole -- splitting on the graph name's OWN internal
# underscores (e.g. "PATTERN_BT" -> "BT") was an earlier version of this script's bug, caught by testing
# against pattern_bt.sno: it produced a phantom block "kind='BT'" that does not exist anywhere in emit.cpp.
BOX_UID_PREFIX = re.compile(r'^n(\d+)_(.+)$')

ALLOW = [
    (re.compile(r'^n\d+_[A-Za-z0-9_]+_bx$'),
     "box-span ELF .type@function/.size debug marker (emit.cpp bxs[], ~line 2977) -- spans the box's "
     "entire range, not scoped to one port; verified, ceo-ruled exempt 2026-08-29b"),
    (re.compile(r'^\.S\d+$'),
     "module-level string dedup table (emit.cpp strtab_label) -- shared across boxes, no single owner; "
     "verified, ceo-ruled exempt 2026-08-29b"),
    (re.compile(r'^\.C\d+$'),
     "module-level cset dedup table (emit.cpp csettab_label) -- same as .S<N>; ceo-ruled exempt"),
    (re.compile(r'^(RETURN|FRETURN|NRETURN)$'),
     "per-function shared exit label (emit.cpp emit_floater_label) -- one level above any single box; "
     "verified, ceo-ruled exempt 2026-08-29b"),
    (re.compile(r'^FN__'),
     "function/graph entry point, called from outside the graph -- same class as RETURN-family; "
     "ceo-ruled exempt 2026-08-29b"),
    (re.compile(r'^[A-Za-z_][A-Za-z0-9_$]*_res$'),
     "per-function result-cell label -- same function-level class as RETURN/FN__, not separately "
     "re-litigated (structural extension of the adopted ruling, not a new judgment call)"),
    (re.compile(r'^(main|module_init|__gva_names)$'),
     "program-level entry/init/table symbols -- same function/module-level class as RETURN/FN__/.S/.C"),
    (re.compile(r'^[A-Za-z_][A-Za-z0-9_$]*_α_body$'),
     "function/graph body-entry label (emit.cpp lbl_α_body, emit_label_initf(&lbl_α_body, \"%s_α_body\", "
     "fam) where fam is the GRAPH name, ~line 2665; the bare \"<fam>_α\"/_β/_γ/_ω/_res siblings declared "
     "alongside it are never themselves emitted as a text definition -- verified via direct grep, no live "
     "witness defines them bare) -- same function-level class as FN__/RETURN, one level above any box"),
]


def allowed(name):
    for pat, _why in ALLOW:
        if pat.match(name):
            return True
    return False


def check(path):
    violations = []
    cur_kind = None
    cur_greek = None
    n_checked = 0
    with open(path, encoding='utf-8') as f:
        for lineno, line in enumerate(f, 1):
            m = LABEL_DEF.match(line.rstrip('\n'))
            if not m:
                continue
            name = m.group(1)
            pm = PORT_LABEL.match(name)
            if pm:
                whole = pm.group(1)  # e.g. "n3_match_alternate" (box) or "PATTERN_BT" (bare nested graph)
                bm = BOX_UID_PREFIX.match(whole)
                cur_kind = bm.group(2) if bm else whole
                cur_greek = pm.group(2)
                continue
            n_checked += 1
            if cur_kind is None:
                continue  # preamble before the first block; not this gate's concern
            if allowed(name):
                continue
            # KIND must match the enclosing box exactly; the GREEK segment is accepted as any of the 4
            # valid ports, not only the port textually current at this line. x86_internal_resolve()
            # (x86_asm.h) deliberately LOCKS an internal label to whichever port sees it FIRST -- def or
            # a forward reference, whichever comes first in program order (seat01's a01fe9f6 ledger entry:
            # "mutating a current-port global while the expression is under construction" is unsafe, so a
            # forward jump from alpha to a target physically defined after the beta port label opens is
            # legitimate and correctly resolves to alpha, not beta). Requiring the EXACT textually-current
            # greek here would flag that established, byte-identical-verified behavior as a false defect;
            # requiring the KIND still catches the actual thing this gate exists to catch (a label with no
            # derived-prefix identity at all, or the wrong owning box's identity).
            want_kind = ".L" + cur_kind + "_"
            if any(name.startswith(want_kind + g) for g in GREEK):
                continue
            violations.append((lineno, name, cur_kind, cur_greek))
    return violations, n_checked


def main(argv):
    if len(argv) != 1:
        print("usage: lib_bb_block_label_prefix_check.py file.s", file=sys.stderr)
        return 2
    violations, n_checked = check(argv[0])
    if violations:
        print(f"⛔ {len(violations)} non-conforming label def(s) in {argv[0]}:", file=sys.stderr)
        for lineno, name, kind, greek in violations[:40]:
            print(f"    line {lineno}: {name}  (inside block kind={kind!r} port={greek!r}, "
                  f"wants prefix .L{kind}_{greek})", file=sys.stderr)
        if len(violations) > 40:
            print(f"    ... and {len(violations) - 40} more", file=sys.stderr)
        print(n_checked)
        return 1
    print(n_checked)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
