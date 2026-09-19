#!/usr/bin/env python3
"""util_gc_port_live_census.py -- WHAT IS LIVE IN A CALLEE-SAVED REGISTER AT A BYRD PORT, READ OFF THE EMITTED
CODE (cto 2026-09-18; row gc-the-emitter-emits-the-poll-not-the-template, rung 1 of the ceo's PORT GRAPH
proposal CEO-899; design .github/ARCH-GC-COMPILE-TIME-FRAME-MAPS.md sections 6.5a and 8.3).

THE CLAIM THIS EXISTS TO MEASURE, IN THE ceo's WORDS (CEO-899 (b)): "OMEGA, AND A GRAPH'S SINGLE GAMMA, ARE SAFE
POINTS WITH A STRUCTURALLY EMPTY LIVE SET.  At a concede the box has failed and its zeta contribution is dead; at
a graph's one gamma or one omega the whole activation is finished.  A POLL THERE NEEDS NO 6.5c SPILL RECORD AT
ALL."  If that holds, a poll at a port is far cheaper than a poll at an allocating return, and the 123 unpolled
sites are 123 MINUS those dominated by a port transition.

⛔ WHY IT IS MEASURED AND NOT REASONED.  Section 6.5's original register paragraph asserted, from the meaning of
the registers, what the emitted code does -- and section 6.5a found the REGISTER right and the ROUTE wrong once
someone finally read the .s.  This claim has the same shape: it is derived from what a port MEANS.  And there is
a specific gap the meaning does not cover -- ⭐ THE CLAIM IS ABOUT THE BOX'S CONTRIBUTION, BUT THE POLL'S
OBLIGATION IS ABOUT THE REGISTER FILE.  At a box's concede the box's own zeta cells are dead, but r13 still holds
the ENCLOSING MATCH's subject base, which belongs to an ancestor and is not the conceding box's to abandon.  A
collection there relocates the block and leaves the ancestor's register stale.  So "the box is finished" and "no
register needs carrying" are two different statements and only the second one licenses dropping the record.

WHAT IT MEASURES, per mode-4 .s: at every instruction carrying a PORT label, which of the six callee-saved
registers are LIVE (some path from here reads it before writing it), split the same way section 6.5a splits an
allocating return -- OWNED (a definition inside this emitted code reaches the port) versus PASSED THROUGH (the
value is an ancestor's, still sitting in the register because nothing here wrote it).  The liveness, the CFG, the
reaching-definition walk and the port-label vocabulary are all IMPORTED from util_gc_callee_saved_census.py
rather than copied, so the two instruments cannot disagree about what "live" means.

⛔ AND IT PRINTS ITS POPULATION PER FILE, BECAUSE THE AGGREGATE IS ONE LANGUAGE.  On the eight witnesses of the
callee-saved gate, 160 of the 175 omega ports are PROLOG's; the other seven frontends contribute 15 between them.
Any conclusion read off the total is a conclusion about Prolog's lowering.  That is the same failure as a census
whose population does not contain the class it grades, in the other direction, and the per-file table is the
only thing that stops it.

USAGE: util_gc_port_live_census.py <file.s>...    |    --selftest
"""
import importlib.util, os, re, sys
from collections import Counter, defaultdict

HERE = os.path.dirname(os.path.abspath(__file__))

def load_cs():
    spec = importlib.util.spec_from_file_location("_cs", os.path.join(HERE, "util_gc_callee_saved_census.py"))
    m = importlib.util.module_from_spec(spec); spec.loader.exec_module(m)
    return m

PORT_RX = re.compile(r"_(α|ω|γ|β)(?:_[0-9_]*)?$")
KIND = {"α": "alpha", "β": "beta", "γ": "gamma", "ω": "omega"}
ORDER = ("alpha", "beta", "gamma", "omega")

def port_live(paths, cs):
    """(per-kind totals, per-kind per-register detail, per-file per-kind sites)"""
    tot = Counter(); owned = Counter(); passed = Counter()
    detail = defaultdict(Counter); perfile = defaultdict(Counter); perfile_owned = defaultdict(Counter)
    for path in paths:
        insns = cs.parse(path)
        succ, top, _ = cs.build_cfg(insns)
        cs.build_ctx(insns)
        live, ud = cs.liveness(insns, succ, top)
        pred = cs.build_pred(succ, len(insns))
        base = os.path.basename(path)
        for i, ins in enumerate(insns):
            hit = None
            for L in ins.labels:
                m = PORT_RX.search(L)
                if m: hit = KIND[m.group(1)]; break
            if hit is None: continue
            tot[hit] += 1; perfile[base][hit] += 1
            for r in cs.CS:
                if r not in live[i]: continue
                cls = cs.reaching_defs(insns, pred, ud, i, r)
                if cls == {"ENTRY"}:
                    passed[hit] += 1; detail[hit][r + "/passthru"] += 1
                else:
                    owned[hit] += 1; detail[hit][r + "/owned"] += 1; perfile_owned[base][hit] += 1
    return tot, owned, passed, detail, perfile, perfile_owned

def report(paths, out=print):
    cs = load_cs()
    tot, owned, passed, detail, perfile, perfile_owned = port_live(paths, cs)
    out("PORT-LIVE CENSUS files=%d" % len(paths))
    out("%-7s %8s %13s %15s %16s" % ("port", "sites", "OWNED live", "PASS-THRU live", "owned per site"))
    for k in ORDER:
        if not tot[k]: continue
        out("%-7s %8d %13d %15d %16.2f" % (k, tot[k], owned[k], passed[k], owned[k] / tot[k]))
    out("")
    out("⛔ PER FILE, because the aggregate is dominated by whichever frontend lowers to the most ports:")
    for f in sorted(perfile):
        out("   %-16s %s" % (f, "  ".join("%s=%d(owned %d)" % (k, perfile[f][k], perfile_owned[f][k]) for k in ORDER if perfile[f][k])))
    out("")
    for k in ("gamma", "omega"):
        if not detail[k]: continue
        out("   %s by register: %s" % (k, "  ".join("%s=%d" % (a, b) for a, b in sorted(detail[k].items()))))
    rbp_o = detail["omega"].get("rbp/owned", 0)
    o_all = owned["omega"]
    r13_live = detail["omega"].get("r13/owned", 0) + detail["omega"].get("r13/passthru", 0)
    out("")
    out("VERDICT-INPUT omega_sites=%d owned=%d owned_minus_rbp=%d r13_live_at_omega=%d"
        % (tot["omega"], o_all, o_all - rbp_o, r13_live))
    out("   rbp is a FRAME BASE -- a stack address, provably not a heap pointer (the RSP/LEA class of 6.5a), so it")
    out("   is separated out rather than counted against the claim. r13 is the ONE register 6.5a measured holding a")
    out("   collected-heap pointer, so r13_live_at_omega is the count of omega ports that still owe a spill record.")
    return 0

def selftest():
    cs = load_cs()
    import tempfile
    fails = []
    def ck(c, m):
        print(("  ok   " if c else "  FAIL ") + m)
        if not c: fails.append(m)
    d = tempfile.mkdtemp(); p = os.path.join(d, "t.s")
    open(p, "w", encoding="utf-8").write(
        " .text\n"
        "g_α:\n"
        " mov r13, rax\n"
        " call rt_thing\n"
        "g_ω:\n"
        " mov rdi, r13\n"
        " ret\n")
    tot, owned, passed, detail, perfile, pfo = port_live([p], cs)
    ck(tot["alpha"] == 1 and tot["omega"] == 1, f"both port kinds are found by their label suffix, got {dict(tot)}")
    ck(detail["omega"].get("r13/owned", 0) == 1,
       f"PLANTED: a register DEFINED in this code and READ AFTER the omega port reads OWNED-live AT that port -- which "
       f"is the reading that would falsify 'an omega has a structurally empty live set', got {dict(detail['omega'])}")
    open(p, "w", encoding="utf-8").write(
        " .text\n"
        "h_α:\n"
        " mov r13, rax\n"
        " mov rdi, r13\n"
        " call rt_thing\n"
        "h_ω:\n"
        " ret\n")
    tot2, owned2, passed2, detail2, _pf, _pfo = port_live([p], cs)
    ck(detail2["omega"].get("r13/owned", 0) == 1,
       f"⛔ THE OVER-APPROXIMATION, ASSERTED SO IT IS NEVER READ AS REAL LIVENESS: a `ret` makes ALL SIX callee-saved "
       f"registers live by the SysV contract, so a port sitting before a ret reads every register live even where the "
       f"value's last READ is behind it. That is the safe direction for a question whose wrong answer loses a root, and "
       f"it means an OWNED count at a port is an UPPER BOUND -- rbp dominating such a count is the ABI talking, not a "
       f"live pointer. Any claim that a port has an empty live set must be read against this. got {dict(detail2['omega'])}")
    open(p, "w", encoding="utf-8").write(
        " .text\n"
        "k_\u03b1:\n"
        " mov r13, rax\n"
        " mov rdi, r13\n"
        " call rt_thing\n"
        "k_\u03c9:\n"
        " jmp .Lspin\n"
        ".Lspin:\n"
        " jmp .Lspin\n")
    tot3b, _o3, _p3, detail3b, _pf3, _pfo3 = port_live([p], cs)
    ck(detail3b["omega"].get("r13/owned", 0) == 0 and tot3b["omega"] == 1,
       f"PLANTED, the other direction, with the ret removed so the ABI contract is out of the way: a register whose "
       f"last read is BEFORE the omega port is NOT owned-live at it. So the instrument does not manufacture the very "
       f"liveness it measures -- the reading above is the ABI, this one is the code. got {dict(detail3b['omega'])}")
    open(p, "w", encoding="utf-8").write(" .text\nnoports:\n mov rax, 1\n ret\n")
    tot3, _o, _p, _d, _pf, _pfo = port_live([p], cs)
    ck(sum(tot3.values()) == 0,
       "a file with no port label reports ZERO ports rather than attributing its instructions to a catch-all")
    print("SELFTEST %d/%d arms green" % (5 - len(fails), 5))
    return 1 if fails else 0

if __name__ == "__main__":
    a = sys.argv[1:]
    if not a or a[0] == "--selftest": sys.exit(selftest())
    sys.exit(report(a))
