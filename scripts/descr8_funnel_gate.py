#!/usr/bin/env python3
"""descr8_funnel_gate.py — DESCR8 Path A step-3 GATE.

Proves the SOURCE INVARIANT: every raw DESCR_t field touch (.slen/.ptr/.arr/
.tbl/.u/.p and the name/cset discriminator forms) lives inside the sanctioned
macro layer, EXCEPT a small explicit allowlist of confirmed non-DESCR receivers
and foundation-layer constructor definitions.

Sanctioned macro layer (where DESCR access is DEFINED, scanner already skips
descr.h + sil_macros.h; the constructor-def lines in core.h / IR.h / bb_box.h
are allowlisted below):
    src/include/descr.h        GET_/SET_/MK_/IS_ accessors + name discriminators
    src/emitter/sil_macros.h   emitter-side mirror (guarded)
    src/runtime/core/core.h    STRVAL/BSTRVAL/NAMEPTR/NAMEVAL/TABLE_VAL/ARRAY_VAL
    src/include/IR.h           NULVCL/STRVAL ctor twins
    src/include/bb_box.h       descr_match_* builders

This is the C-funnel oracle. It does NOT prove behavior (the snobol4/icon
smokes do that, against the no-regression baseline); it proves that the 8-byte
layout flip will touch ONE header, because nothing reads/writes DESCR fields
raw anywhere else.

Allowlist entries are file:line OR a symbolic substring (for sites whose line
drifts). Each carries a one-line reason. Reviewed by hand 2026-05-31.
Regenerate by RE-REVIEWING scanner output, never by blind renumbering.

Exit 0 = GATE GREEN, 1 = RED (un-funneled residue), 2 = scanner error.
"""
import json, os, subprocess, sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)

# file:line  ->  reason  (confirmed NOT a DESCR_t field access, or a
# foundation-layer constructor/builder definition that must stay raw).
ALLOW = {
    "runtime/interp/gen_runtime.c:1895": "g_kw_cset_names[].ptr — cset-names table struct, not DESCR",
    "runtime/interp/gen_runtime.c:1900": "g_kw_cset_names[].ptr — table struct write",
    "runtime/interp/gen_runtime.c:1910": "g_kw_cset_names[].ptr — table struct read",
    "runtime/interp/gen_runtime.c:1916": "g_kw_cset_names[].ptr — table struct read",
    "runtime/interp/gen_runtime.c:1941": "g_kw_cset_names[].ptr — table struct write",
    "runtime/interp/gen_runtime.c:1960": "g_kw_cset_names[].ptr — table struct write",
    "runtime/core/core.c:2320":  "_var_reg[].ptr — variable-registry struct, not DESCR",
    "runtime/core/core.c:2449":  "_var_reg[].ptr — variable-registry struct",
    "runtime/core/core.c:2460":  "_var_reg[].ptr — variable-registry struct",
    "runtime/core/core.c:2536":  "_var_reg[].ptr — variable-registry struct",
    "runtime/core/core.h:21":    "BSTRVAL ctor definition — foundation layer",
    "runtime/core/core.h:318":   "TABLE_VAL ctor definition — foundation layer",
    "runtime/core/core.h:319":   "ARRAY_VAL ctor definition — foundation layer",
    "include/IR.h:8":            "NULVCL ctor definition — foundation layer",
    "include/IR.h:17":           "STRVAL ctor definition — foundation layer",
    "include/bb_box.h:8":        "descr_match_span signature/body — builder, foundation",
    "include/bb_box.h:10":       "descr_match_cat signature/body — builder, foundation",
    "include/bb_box.h:12":       "descr_match_* DT_SNUL build — builder, foundation",
    "frontend/snocone/snocone_parse.tab.c:2992": "ctx.p — LexCtx struct, not DESCR",
    "frontend/prolog/prolog_builtin.c:25":       "tbl[i].p — builtin table struct, not DESCR",
    "emitter/BB_templates/bb_iterate.cpp:98":  "comment text — scanner artifact",
    "emitter/BB_templates/bb_iterate.cpp:101": "comment text — scanner artifact",
    "emitter/BB_templates/bb_iterate.cpp:142": "comment text — scanner artifact",
    "emitter/BB_templates/bb_iterate.cpp:197": "comment text — scanner artifact",
}

# Symbolic allows: (relpath, substring, reason). Used where the line number
# drifts but the text is stable — matched by content, not position.
SYMBOLIC = [
    ("runtime/core/eval_code.c", "expr_d.slen == 2",
     "DT_E subexpression arity discriminator — not a name/payload field"),
]


def main():
    os.makedirs("/tmp/desc8", exist_ok=True)
    r = subprocess.run([sys.executable, os.path.join(HERE, "descr8_scan.py")],
                       cwd=ROOT, capture_output=True, text=True)
    if r.returncode != 0:
        sys.stderr.write("GATE ERROR: scanner failed\n" + r.stdout + r.stderr)
        return 2
    rows = json.load(open("/tmp/desc8/sites_unique.json"))

    residue = []
    for rel, ln, field, kind, text in rows:
        if f"{rel}:{ln}" in ALLOW:
            continue
        if any(rel == sf and sub in text for sf, sub, _ in SYMBOLIC):
            continue
        residue.append((rel, ln, field, kind, text))

    if not residue:
        print("GATE GREEN — zero raw DESCR field access outside the macro layer")
        print("  (%d scanner hits, all macro-layer or on the reasoned allowlist of %d)"
              % (len(rows), len(ALLOW) + len(SYMBOLIC)))
        return 0

    print("GATE RED — %d un-funneled DESCR field access site(s):" % len(residue))
    for rel, ln, field, kind, text in residue:
        print("  %s:%d [%s .%s] %s" % (rel, ln, kind, field, text[:90]))
    return 1


if __name__ == "__main__":
    sys.exit(main())
