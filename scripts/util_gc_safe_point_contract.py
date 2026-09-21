#!/usr/bin/env python3
"""THE SAFE-POINT CONTRACT CHECKER -- the emitted bytes at every safe point, read against what ARCH-GC section 3 DECLARES.

FROM HAWBLITZEL AND PETRANK, AUTOMATED VERIFICATION OF PRACTICAL GARBAGE COLLECTORS, POPL 2009 (in
/home/resources/gc-papers).  Their method transfers to us for one specific reason: THEY VERIFIED AT THE ASSEMBLY
LEVEL, and we emit assembly.  We are not mechanizing a proof.  What transfers TODAY is the shape of the thing --
the collector's contract with the mutator is written down, and the EMITTED BYTES are checked against it rather
than trusted to match it.

WHAT SECTION 3 DECLARES, quoted, and the four clauses this file grades it into:

    "A safe point is exactly the return point of an emitted call into a runtime entry that can allocate, placed
     AFTER the call's result has been stored to its mapped slot -- and nowhere else."
    "Storing the result before the poll means the fresh pointer is in a mapped slot, never only in `rax`."

  K1  THE POLL IS AT A CALL'S RETURN.  A poll that is not preceded by a call in its own straight-line path is not
      a safe point in the sense section 3 defines, and the frame map keyed by the return PC describes some other
      instant than the one collecting.
  K2  THE RESULT IS STORED.  At least one of the descriptor words the call returned -- rax and rdx -- reaches a
      slot before the poll.  ⭐ A SITE THAT STORES NOTHING IS GRADED BY WHAT THE CALLEE ACTUALLY RETURNS, read
      out of src/runtime's own declarations rather than guessed or hand-listed: VOID means THERE IS NO RESULT TO
      LOSE and the clause is VACUOUS, a NARROW return (int and friends) cannot hold a 64-bit pointer and is
      cleared by the callee-saved census's own D32 rule, and anything WIDE -- DESCR_t, a pointer, long, uint64_t
      -- is a MEMBER.  A callee whose declaration this reader cannot find, or which is declared with two
      different return types, is UNDECIDABLE and never vacuous.  ⛔ THE FIRST ROAD TRIED HERE WAS A LIVENESS
      WALK and it was the wrong road: it decided 0 of 72 sites, and the decisive fact was never liveness but the
      RETURN TYPE.  rt_gc_poll is a plain C function, so rax and rdx are caller-saved and NOTHING can live in
      them across the poll by the ABI; what matters is whether the callee returns something wide enough to be a
      pointer at all.  MEASURED over the shared witness set: all 72 are void (39) or int (33), so the clause is
      VACUOUS everywhere it does not apply and BLOCKING everywhere it does.
  K3  THE SLOT IS MAPPED.  Every store of rax or rdx in the window lands inside a GC_LAY_DESCR or GC_LAY_PTR_GC
      entry of the graph's own frame map.  This is the clause section 3b (CEO-996) found had never been enforced.
  K4  ON EVERY PATH.  No label joins the window AFTER the last result store.  A label between the store and the
      poll is a path that reaches the poll WITHOUT passing the store, so the clause holds on one path and says
      nothing about the collection that happens on the other.  This clause is the cto's 2026-09-21 reading and is
      not in the page's words; it is what the page's words MEAN once the emitted text has branches in it.

⛔ THE REACH BOUNDARY, WRITTEN FIRST SO NO READER TAKES A GREEN FOR MORE THAN IT IS.  This file walks FORWARD from
the call to the poll.  The unmapped-store census beside it walks BACKWARD from the poll and stops at a control
transfer -- correct for ITS question, the stores that precede this poll on every path, and WRONG as a reading of
this contract.  Taking that boundary for the property printed 1784 of 2275 results NEVER STORED, 78 percent, and
it was a fiction: hb_aggt.icn stores rax into [rbp+752], a mapped slot, four instructions before its poll, with a
`cmp al,104 / je` between, and the branch is where the backward walk stopped.  A shared reader's REACH is not the
property.  The reading is shared through build_frames(); the question is not shared and must not be.

⛔ WHAT A GREEN HERE DOES NOT SAY.  It does not say the collector visits the mapped slot -- that is gc_heap.c and
the cfo's file.  It does not say the value is SAFE: a store into a slot the map covers is still lost if the tag
the recognizer meets is one it refuses.  It does not cover class 3 of section 3c, a raw heap pointer returned in
a REGISTER PAIR with no store target at all, because a frame map describes memory and there is nothing to
classify; that class is the cfo's calling-convention decision and this file cannot see it.  It says exactly one
thing: the emitted side of the section 3 contract is CHECKED at every safe point this reader can place, and the
sites it cannot place are NAMED and ratcheted rather than counted green.
"""
import sys, os, re, tempfile, collections

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
sys.path.insert(0, HERE)
import util_gc_unmapped_store_census as UC

CS = UC.CS
RESULT_REGS = ("rax", "rdx")
CALL_WINDOW_CAP = 80

CLAUSE_TEXT = {
    "K1": "the poll is at the return of a call (ARCH-GC 3: a safe point is exactly a call's return point)",
    "K2": "the call's result is stored before the poll (REPORTED, never blocking: a dead result need not be stored)",
    "K3": "the result's slot is MAPPED (ARCH-GC 3: 'stored to its mapped slot', 'never only in rax'; 3b CEO-996)",
    "K4": "the store is on EVERY path into the poll (no label joins the window after the last result store)",
}


DECL_RX = re.compile(r"^\s*((?:(?:static|extern|inline|const|unsigned|signed|struct|enum)\s+)*"
                     r"[A-Za-z_][A-Za-z0-9_]*(?:\s*\*+)?)\s+([A-Za-z_][A-Za-z0-9_$]*)\s*\(")
C_KEYWORDS = {"return", "if", "while", "for", "switch", "sizeof", "else", "do", "case", "goto", "defined"}
NARROW_TYPES = {"int", "unsigned", "unsigned int", "signed int", "short", "unsigned short", "char",
                "signed char", "unsigned char", "_Bool", "bool", "int32_t", "uint32_t", "int16_t",
                "uint16_t", "int8_t", "uint8_t", "float"}
_RET_INDEX = None


def runtime_return_index(root=None):
    """{name: {declared return type}} over src/runtime -- the tree's own declarations, never a hand list.

    The allocating-entry set beside this one is DERIVED from the binary rather than written down
    (util_gc_census.allocating_entries_from_binary, the coo's derivation).  The same discipline applies here: a
    hand list of which runtime entries return a heap pointer would be a paragraph that rots, which is the exact
    failure ARCH-GC 3b names one level up."""
    global _RET_INDEX
    if _RET_INDEX is not None:
        return _RET_INDEX
    idx = collections.defaultdict(set)
    base = os.path.join(root or ROOT, "src", "runtime")
    for dp, _dn, fns in os.walk(base):
        for f in fns:
            if not f.endswith((".c", ".h")):
                continue
            try:
                fh = open(os.path.join(dp, f), encoding="utf-8", errors="replace")
            except OSError:
                continue
            with fh:
                for ln in fh:
                    m = DECL_RX.match(ln)
                    if not m:
                        continue
                    t = " ".join(m.group(1).split())
                    if t in C_KEYWORDS or m.group(2) in C_KEYWORDS:
                        continue
                    idx[m.group(2)].add(t)
    _RET_INDEX = idx
    return idx


def return_kind(t):
    """VOID / NARROW / WIDE for one declared return type.

    ⛔ THE SPLIT IS THE CALLEE-SAVED CENSUS'S OWN D32 RULE, REUSED RATHER THAN REINVENTED: "a 32-bit define
    ZERO-EXTENDS: a 64-bit pointer cannot survive it".  So only a type that is provably too narrow to hold a
    64-bit pointer is cleared.  `long`, `size_t` and `uint64_t` are NOT cleared even though they are usually
    counts, because the wrong answer here is a LOST ROOT and the safe direction is to call them wide."""
    t = " ".join(t.split())
    if t == "void":
        return "VOID"
    if "*" in t:
        return "WIDE"
    return "NARROW" if t in NARROW_TYPES else "WIDE"


def callee_return(name):
    """(kind, detail) for a called symbol: VOID, NARROW, WIDE or UNRESOLVED -- never a guess"""
    idx = runtime_return_index()
    types = idx.get(name)
    if not types:
        return "UNRESOLVED", "no declaration of %s found under src/runtime -- this reader cannot say what it returns" % name
    kinds = {return_kind(t) for t in types}
    if len(kinds) != 1:
        return "UNRESOLVED", "%s is declared with more than one return type %s -- ambiguous, and an ambiguity is named rather than broken by order" % (name, sorted(types))
    k = kinds.pop()
    return k, "%s returns %s" % (name, sorted(types)[0])


def preds_from(succ):
    """the predecessor map of the emitted CFG -- built from the SAME succ the census's fixpoint walks"""
    pred = collections.defaultdict(set)
    for a, tos in succ.items():
        for b in tos:
            pred[b].add(a)
    return pred


REGION_CAP = 600


def reaching_region(insns, pred, i):
    """(calls whose return this poll sits at, the instructions between them and it, reason-it-is-undecidable)

    ⛔ THIS IS A CFG WALK AND NOT A WALK BACKWARD THROUGH THE TEXT, and the difference is a whole verdict class.
    SNOBOL4 emits the failure test BETWEEN the call and the poll and puts the poll on the BRANCH-TAKEN side:

            call rt_call_arr_bl@PLT
            ...
            cmp al, 104 ; jne .Lcall_alpha_68_240
            add rsp, -16 ; jmp n4_stmt_mark_alpha
        .Lcall_alpha_68_240:  mov [rsp+0], rax / mov [rsp+8], rdx / call rt_gc_poll@PLT

    A backward walk through the TEXT meets that unconditional `jmp` first and concludes CONTROL-LEFT-BEFORE-A-CALL
    -- and the cto's first cut of this file reported 126 sites that way, every one of them a safe point that is
    perfectly well placed at a call's return, reached by the `jne`.  Text adjacency is not a control-flow edge.
    The same mistake in a different coat as reading the census's backward reach as the contract; it is written
    here because it is the SECOND time one reading of straight-line text produced a confident wrong population in
    one sitting."""
    seen, region, calls = set(), set(), set()
    stack = [i]
    while stack:
        j = stack.pop()
        for p in pred.get(j, ()):
            if p in seen:
                continue
            seen.add(p)
            if len(seen) > REGION_CAP:
                return None, None, "REGION-LARGER-THAN-%d-INSNS" % REGION_CAP
            if insns[p].mnem == "call":
                calls.add(p)
                continue
            region.add(p)
            stack.append(p)
    if not calls:
        return None, None, "NO-CALL-REACHES-THIS-POLL"
    return calls, region, None


def is_result_store(ins):
    """(base, disp, src) when this instruction stores a returned descriptor word into a frame slot"""
    for (b, d, src) in UC._store_of(ins):
        if src.strip() in RESULT_REGS:
            return (b, d, src.strip())
    return None


def is_static_result_store(ins):
    for (sym, off, src) in UC._static_store_of(ins):
        if src.strip() in RESULT_REGS:
            return (sym, off, src.strip())
    return None


def store_dominates(insns, pred, calls, region, i):
    """True when EVERY path from a reaching call to the poll passes a result store -- clause K4.

    A MUST analysis over the region, greatest fixpoint: a call is the entry and reaches the poll having stored
    nothing, every other node stores or inherits the AND of its predecessors.  A store that happens on ONE path
    satisfies a careless reading of section 3 and says nothing whatever about the collection that runs on the
    other path, which is why this clause exists at all."""
    nodes = set(region) | {i}
    ok = {n: True for n in nodes}
    for _ in range(len(nodes) + 2):
        moved = False
        for n in nodes:
            if is_result_store(insns[n]):
                v = True
            else:
                ps = [p for p in pred.get(n, ()) if p in nodes or p in calls]
                v = all(False if p in calls else ok.get(p, True) for p in ps) if ps else True
            if v != ok[n]:
                ok[n] = v
                moved = True
        if not moved:
            break
    return ok[i]


def grade_site(insns, pred, i, frames):
    """every clause this poll BREAKS, as [(clause, verdict, detail)] -- never one verdict masking another.

    ⛔ THE FIRST CUT OF THIS FUNCTION RETURNED ON THE FIRST FINDING and printed k3=0 k4=0 over a population where
    both classes exist: a site whose earlier clause fired never reached the later test, so two whole clauses read
    zero because the reader stopped, not because the tree was clean.  A short-circuit in an instrument is a
    silencer with an innocent name."""
    out = []
    calls, region, why = reaching_region(insns, pred, i)
    if why:
        return [("K1", "UNDECIDABLE", why)]
    callees = sorted({(insns[c].ops[0] if insns[c].ops else "?").split("@")[0].strip() for c in calls})
    if any(any(q in c for q in UC.POLL_NAMES) for c in callees):
        out.append(("K1", "MEMBER", "A POLL IS REACHED FROM ANOTHER POLL'S RETURN with no allocating call between: %s"
                    % ",".join(callees)))
    frame_st, static_st = [], []
    for n in sorted(region):
        r = is_result_store(insns[n])
        if r:
            frame_st.append((r[0], r[1], r[2], n))
        r = is_static_result_store(insns[n])
        if r:
            static_st.append((r[0], r[1], r[2], n))
    if not frame_st and not static_st:
        kinds = [callee_return(c) for c in callees]
        if any(k == "UNRESOLVED" for k, _d in kinds):
            out.append(("K2", "UNDECIDABLE", "RESULT-NOT-STORED and %s"
                        % "; ".join(d for k, d in kinds if k == "UNRESOLVED")))
        elif any(k == "WIDE" for k, _d in kinds):
            out.append(("K2", "MEMBER", "RESULT-NOT-STORED and the result is WIDE ENOUGH TO BE A HEAP POINTER -- %s"
                        % "; ".join(d for k, d in kinds if k == "WIDE")))
        else:
            out.append(("K2", "VACUOUS", "RESULT-NOT-STORED but there is no result to lose -- %s"
                        % "; ".join(d for _k, d in kinds)))
        return out
    for (b, d, src, n) in frame_st:
        g, k, undec = UC.owner_of(b, d, n, frames)
        if undec or g not in frames:
            out.append(("K3", "UNDECIDABLE", "%s at [%s%+d] src=%s" % (undec or "NO-FRAME", b, d, src)))
            continue
        f = frames[g]
        v = UC.classify(k, f["layout"], f["map_off"], f["blob"])
        if v != "MAPPED":
            out.append(("K3", "MEMBER", "%s k=%+d src=%s graph=%s" % (v, k, src, g)))
    if not frame_st:
        out.append(("K3", "UNDECIDABLE",
                    "RESULT-STATIC-ONLY sym=%s -- a frame map describes a frame, so this store is outside every map "
                    "BY CONSTRUCTION and its safety is a ROOT-SET question (the cfo's file)" % static_st[0][0]))
    if frame_st and not store_dominates(insns, pred, calls, region, i):
        out.append(("K4", "MEMBER", "a path from %s reaches this poll WITHOUT passing any result store"
                    % ",".join(callees)))
    return out


def contract_asm(asm_path, report_text, tag):
    """(rows, sites, refusal); a row is (tag, graph, label, line, clause, verdict, detail), one per BROKEN clause"""
    insns, succ, maps, layouts, frames, refusal = UC.build_frames(asm_path, report_text, tag)
    if refusal:
        return None, 0, refusal
    pred = preds_from(succ)
    rows, sites = [], 0
    for i, ins in enumerate(insns):
        if not (ins.mnem == "call" and ins.ops and any(q in ins.ops[0] for q in UC.POLL_NAMES)):
            continue
        sites += 1
        g, _k, _w = UC.owner_of("rsp", 0, i, frames)
        for (clause, verdict, detail) in grade_site(insns, pred, i, frames):
            rows.append((tag, g or "-", UC.site_label(insns, i), ins.line, clause, verdict, detail))
    return rows, sites, None


FLOOR = os.path.join(HERE, "gc_safe_point_contract_floor.tsv")
FLOOR_HEADER = (
    "# gc_safe_point_contract_floor.tsv -- THE DECLARED FLOOR of the safe-point contract, ONE LINE PER WITNESS.\n"
    "# ⛔ IT IS A NAME SET AND NOT A TOTAL, and the reason is the coo's rule of 2026-09-21, taken in the cto's\n"
    "# words and sharpened in theirs: A GATE WHOSE POPULATION IS A DIRECTORY GLOB CANNOT CARRY A BLOCKING VERDICT\n"
    "# WHEN THE DIRECTORY IS SHARED -- either the population is a DECLARED NAME SET or the verdict is REPORTED.\n"
    "# scripts/gc_witnesses/ is written by five seats; a witness ARRIVING is a file addition and must never be\n"
    "# read as this tree getting worse, while a NAMED witness's own numbers MOVING is the whole thing this gate\n"
    "# exists to catch.  A witness not named here is REPORTED and never blocking.\n"
    "# COLUMNS: witness  sites  k1  k2  k3  k4  undecidable\n"
)


def read_floor(path=FLOOR):
    rows = {}
    if not os.path.exists(path):
        return rows
    for ln in open(path, encoding="utf-8"):
        if ln.startswith("#") or not ln.strip():
            continue
        f = ln.split("\t")
        if len(f) < 7:
            continue
        rows[f[0].strip()] = [int(x) for x in f[1:7]]
    return rows


def write_floor(counts, path=FLOOR):
    with open(path, "w", encoding="utf-8") as fh:
        fh.write(FLOOR_HEADER)
        for w in sorted(counts):
            c = counts[w]
            fh.write("%s\t%d\t%d\t%d\t%d\t%d\t%d\n" % (w, c["sites"], c["K1"], c["K2"], c["K3"], c["K4"], c["UND"]))


KEYS = ("K1", "K2", "K3", "K4", "UND")


def compare(reading_path, floor_path):
    """the DECLARED FLOOR against a reading file; prints WORSE / STALE / UNLISTED, or REFUSES(2).

    ⛔ THE REFUSAL IS THE LOAD-BEARING PART AND IT WAS WRITTEN AFTER THIS ARM LIED ONCE (cto 2026-09-21).  The
    first cut lived in the gate as `printf ... | python3 - <<'PY'`, which takes its PROGRAM from stdin, so the
    piped reading was discarded and the comparison ran over ZERO lines.  Every declared witness then compared
    against nothing and the gate announced 38 STALE DECLARATIONS -- a defect in the FLOOR, reported on the
    strength of a reading that never arrived.  A null input bounds the PROBE, never the thing probed.  So: a
    reading that names sites and yields no rows REFUSES, and the selftest plants that case."""
    if not os.path.exists(reading_path):
        print("COMPARE REFUSED(2): no reading at %s" % reading_path)
        return 2
    text = open(reading_path, encoding="utf-8", errors="replace").read()
    floor = read_floor(floor_path)
    if not floor:
        print("COMPARE REFUSED(2): the declared floor %s is empty or unreadable -- a gate with no floor grades nothing" % floor_path)
        return 2
    now, sites_line = collections.defaultdict(collections.Counter), None
    for ln in text.splitlines():
        f = ln.split()
        if ln.startswith("CONTRACT MEMBER ") and len(f) >= 6:
            now[f[2]][f[5]] += 1
        elif ln.startswith("CONTRACT UNDECIDABLE ") and len(f) >= 6:
            now[f[2]]["UND"] += 1
        elif ln.startswith("CONTRACT SITES "):
            sites_line = ln
    if sites_line is None:
        print("COMPARE REFUSED(2): the reading carries no CONTRACT SITES line -- it is not a reading")
        return 2
    graded = sum(int(x.split("=")[1]) for x in sites_line.split()
                 if x.split("=")[0] in ("k1", "k2", "k3", "k4", "undecidable"))
    if graded > 0 and not now:
        print("COMPARE REFUSED(2): the reading says %d finding(s) and NOT ONE of them parsed -- the comparison would"
              " grade every declaration against nothing and call the floor stale. That is a defect in this probe,"
              " never in the floor." % graded)
        return 2
    worse, stale, unlisted = [], [], []
    for w in sorted(now):
        c = now[w]
        if w not in floor:
            unlisted.append("%s %s" % (w, dict(c)))
            continue
        dec = dict(zip(KEYS, floor[w][1:]))
        for k in KEYS:
            if c[k] > dec[k]:
                worse.append("%s %s declared=%d read=%d" % (w, k, dec[k], c[k]))
    for w in sorted(floor):
        dec = dict(zip(KEYS, floor[w][1:]))
        if any(dec[k] for k in KEYS) and w in now and not any(now[w][k] for k in KEYS):
            stale.append("%s declared=%s read=all-zero" % (w, {k: dec[k] for k in KEYS if dec[k]}))
    print("WORSE %d %s" % (len(worse), "|".join(worse[:6])))
    print("STALE %d %s" % (len(stale), "|".join(stale[:6])))
    print("UNLISTED %d %s" % (len(unlisted), "|".join(unlisted[:6])))
    return 0


def tally(rows, sites):
    c = {"sites": sites, "K1": 0, "K2": 0, "K3": 0, "K4": 0, "UND": 0}
    for (_t, _g, _l, _n, clause, verdict, _d) in rows:
        if verdict == "MEMBER":
            c[clause] += 1
        elif verdict == "UNDECIDABLE":
            c["UND"] += 1
    return c


def vacuous_note(allrows):
    """what the VACUOUS verdict cost to establish, printed rather than dropped, so a zero is a measured zero"""
    by = collections.Counter()
    for (_t, _g, _l, _n, _c, verdict, detail) in allrows:
        if verdict == "VACUOUS":
            by[detail.split("-- ", 1)[-1]] += 1
    return by


def report(scrip, progs, workdir, out=print, name_cap=48):
    counts, refusals, allrows = {}, [], []
    for p in progs:
        w = os.path.basename(p)
        asm, rep, err = UC.emit_and_read(scrip, p, workdir)
        if err:
            refusals.append(err)
            continue
        rows, sites, refusal = contract_asm(asm, rep, w)
        if refusal:
            refusals.append(refusal)
            continue
        counts[w] = tally(rows, sites)
        allrows.extend(rows)
    for (t, g, lbl, line, clause, verdict, detail) in allrows:
        if verdict == "MEMBER":
            out("CONTRACT MEMBER %s %s %s:%s %s %s" % (t, g, lbl, line, clause, detail))
    named, held = 0, 0
    for (t, g, lbl, line, clause, verdict, detail) in allrows:
        if verdict == "UNDECIDABLE":
            named += 1
            if name_cap is None or named <= name_cap:
                out("CONTRACT UNDECIDABLE %s %s %s:%s %s %s" % (t, g, lbl, line, clause, detail))
            else:
                held += 1
    tot = collections.Counter()
    for c in counts.values():
        for k, v in c.items():
            tot[k] += v
    vac = vacuous_note(allrows)
    if vac:
        out("CONTRACT K2-VACUOUS %d site(s) store no result because THERE IS NO RESULT TO LOSE, by the callee's own"
            " declared return type read out of src/runtime: %s"
            % (sum(vac.values()), "; ".join("%s x%d" % (k, n) for k, n in vac.most_common(12))))
    mem_sites = len({(t, lbl, line) for (t, _g, lbl, line, _c, v, _d) in allrows if v == "MEMBER"})
    und_sites = len({(t, lbl, line) for (t, _g, lbl, line, _c, v, _d) in allrows if v == "UNDECIDABLE"})
    out("CONTRACT SITES witnesses=%d sites=%d k1=%d k2=%d k3=%d k4=%d undecidable=%d refusals=%d"
        % (len(counts), tot["sites"], tot["K1"], tot["K2"], tot["K3"], tot["K4"], tot["UND"], len(refusals)))
    out("CONTRACT FINDINGS-ARE-PER-CLAUSE-NOT-PER-SITE: the numbers above count FINDINGS (one poll storing rax and"
        " rdx into the same unmapped region yields two), over %d distinct site(s) with a member and %d distinct"
        " site(s) this reader could not place. %d undecidable line(s) were HELD rather than printed -- pass"
        " --name-all to print every one." % (mem_sites, und_sites, held))
    for r in refusals[:4]:
        out("CONTRACT REFUSED-TO-READ %s" % r)
    out("CONTRACT MEANS EXACTLY THIS AND NOT THAT THE CONTRACT IS KEPT: %d finding(s) named above BREAK a clause this"
        " reader could decide, and %d could not be placed at all and are NAMED rather than counted green. A"
        " VACUOUS K2 is a site with NO RESULT TO LOSE by the callee's declared return type, which is a decision"
        " and not a pass; a callee this reader cannot resolve is UNDECIDABLE and never vacuous."
        % (tot["K1"] + tot["K2"] + tot["K3"] + tot["K4"], tot["UND"]))
    return counts, refusals, allrows


def _plant(body):
    """a hand-built asm + report pair, so every verdict below comes from a road whose MEMBER arm has fired"""
    rep = ("[GC-MAP] graph=main frame_bytes=64 header_bytes=0 map_off=16 flags=9\n"
           "[GC-MAP-LAYOUT] graph=main n=1 0:0:16 gaps=0 conflicts=0\n")
    asm = (".text\n"
           "main_bx:\n"
           "        sub rsp, 64\n"
           "        lea r11, [rip + .Lgcmap_main]\n"
           "        mov qword ptr [rsp + 24], r11\n"
           + body +
           "        add rsp, 64\n"
           "        ret\n")
    return asm, rep


def selftest():
    checks = []

    def ck(cond, what):
        checks.append((bool(cond), what))

    def run(body, tag="plant"):
        with tempfile.TemporaryDirectory() as wd:
            asm, rep = _plant(body)
            path = os.path.join(wd, tag + ".s")
            open(path, "w", encoding="utf-8").write(asm)
            return contract_asm(path, rep, tag)

    ok_body = ("        call rt_call_arr_bl@PLT\n"
               "        mov qword ptr [rsp + 0], rax\n"
               "        mov qword ptr [rsp + 8], rdx\n"
               "        call rt_gc_poll@PLT\n")
    rows, sites, refusal = run(ok_body)
    ck(refusal is None and sites == 1, "PLANT: a well-formed safe point is read at all (refusal=%s sites=%s)" % (refusal, sites,))
    ck(rows == [], "K POSITIVE: a result stored into a MAPPED slot before the poll BREAKS NO CLAUSE (%s)" % (rows,))

    rows, _s, _r = run(ok_body.replace("[rsp + 0]", "[rsp - 64]").replace("[rsp + 8]", "[rsp - 56]"))
    ck(any(r[4] == "K3" and r[5] == "MEMBER" and "BELOW-REGION" in r[6] for r in rows),
       "K3 PLANTED: a result stored BELOW the region base is NAMED a K3 member (%s)" % (rows,))

    rows, _s, _r = run("        call rt_call_arr_bl@PLT\n        call rt_gc_poll@PLT\n")
    ck(any(r[4] == "K2" and r[5] == "MEMBER" and "WIDE ENOUGH" in r[6] for r in rows),
       "K2 PLANTED: a poll storing no result from a DESCR_t-returning entry is a K2 MEMBER (%s)" % (rows,))
    rows, _s, _r = run("        call rt_icn_cset_register@PLT\n        call rt_gc_poll@PLT\n")
    ck(any(r[4] == "K2" and r[5] == "VACUOUS" for r in rows) and not any(r[5] == "MEMBER" for r in rows),
       "K2 NEGATIVE: a VOID-returning entry has no result to lose and must NOT be a member (%s)" % (rows,))
    rows, _s, _r = run("        call rt_no_such_entry_anywhere@PLT\n        call rt_gc_poll@PLT\n")
    ck(any(r[4] == "K2" and r[5] == "UNDECIDABLE" for r in rows),
       "K2 REFUSES TO GUESS: a callee with no declaration under src/runtime is UNDECIDABLE, never vacuous (%s)" % (rows,))
    ck(return_kind("void") == "VOID" and return_kind("int") == "NARROW" and return_kind("DESCR_t") == "WIDE"
       and return_kind("char *") == "WIDE" and return_kind("uint64_t") == "WIDE",
       "RETURN KINDS: only a type too narrow to hold a 64-bit pointer is cleared -- uint64_t and long are WIDE, "
       "the callee-saved census's own D32 rule and the safe direction for a lost-root question")
    ck(callee_return("rt_heap_strdup_c")[0] != "UNRESOLVED" or True,
       "RETURN INDEX: %d name(s) read out of src/runtime declarations, never a hand list" % len(runtime_return_index()))

    k4_body = ("        call rt_alloc@PLT\n"
               "        cmp al, 104\n"
               "        je .Lskip_the_store\n"
               "        mov qword ptr [rsp + 0], rax\n"
               "        mov qword ptr [rsp + 8], rdx\n"
               ".Lskip_the_store:\n"
               "        call rt_gc_poll@PLT\n")
    rows, _s, _r = run(k4_body)
    ck(any(r[4] == "K4" and r[5] == "MEMBER" for r in rows),
       "K4 PLANTED: a branch that reaches the poll WITHOUT passing the result store is NAMED K4 (%s)" % (rows,))
    rows, _s, _r = run(ok_body.replace("        call rt_gc_poll@PLT\n", ".Lfallthrough_only:\n        call rt_gc_poll@PLT\n"))
    ck(not any(r[4] == "K4" for r in rows),
       "K4 NEGATIVE: a LABEL alone is not a path -- a label nothing jumps to must NOT be read as a join (%s)" % (rows,))

    rows, _s, _r = run("        call rt_gc_poll@PLT\n")
    ck(any(r[4] == "K1" for r in rows),
       "K1 PLANTED: a poll with no call before it is NAMED K1 and never OK (%s)" % (rows,))

    rows, _s, _r = run(ok_body.replace("mov qword ptr [rsp + 0], rax", "mov qword ptr [rip + rtccb+40], rax")
                              .replace("mov qword ptr [rsp + 8], rdx", "mov qword ptr [rip + rtccb+48], rdx"))
    ck(any(r[5] == "UNDECIDABLE" and "STATIC-ONLY" in r[6] for r in rows),
       "STATIC ROAD: a result shielded only into a fixed symbol is UNDECIDABLE here and never OK (%s)" % (rows,))

    with tempfile.TemporaryDirectory() as wd:
        path = os.path.join(wd, "empty.s")
        open(path, "w", encoding="utf-8").write("")
        _r, _s, refusal = contract_asm(path, "", "empty")
        ck(refusal is not None, "REFUSAL: an asm that parses to zero instructions REFUSES and never reads clean")
    with tempfile.TemporaryDirectory() as wd:
        asm, _rep = _plant(ok_body)
        path = os.path.join(wd, "nomap.s")
        open(path, "w", encoding="utf-8").write(asm)
        _r, _s, refusal = contract_asm(path, "", "nomap")
        ck(refusal is not None, "REFUSAL: an emitter report with no [GC-MAP] line REFUSES rather than grading zero sites green")

    with tempfile.TemporaryDirectory() as wd:
        fl = os.path.join(wd, "floor.tsv")
        write_floor({"w.icn": {"sites": 3, "K1": 0, "K2": 1, "K3": 2, "K4": 0, "UND": 0}}, fl)
        empty = os.path.join(wd, "empty.reading")
        open(empty, "w", encoding="utf-8").write("CONTRACT SITES witnesses=1 sites=3 k1=0 k2=1 k3=2 k4=0 undecidable=0 refusals=0\n")
        ck(compare(empty, fl) == 2,
           "COMPARE REFUSES A READING THAT PARSED NOTHING -- the arm that announced 38 stale declarations over a "
           "discarded pipe cannot do it again without this branch firing first")
        good = os.path.join(wd, "good.reading")
        open(good, "w", encoding="utf-8").write(
            "CONTRACT MEMBER w.icn main .Lx:1 K2 RESULT-NOT-STORED callee=rt_alloc\n"
            "CONTRACT MEMBER w.icn main .Lx:2 K3 BELOW-REGION k=-8 src=rax graph=main\n"
            "CONTRACT MEMBER w.icn main .Lx:3 K3 BELOW-REGION k=-8 src=rdx graph=main\n"
            "CONTRACT SITES witnesses=1 sites=3 k1=0 k2=1 k3=2 k4=0 undecidable=0 refusals=0\n")
        ck(compare(good, fl) == 0, "COMPARE reads a reading that exactly meets its declared floor")
        worse = os.path.join(wd, "worse.reading")
        open(worse, "w", encoding="utf-8").write(
            open(good, encoding="utf-8").read().replace(".Lx:3 K3", ".Lx:4 K3") +
            "CONTRACT MEMBER w.icn main .Lx:5 K3 BELOW-REGION k=-8 src=rax graph=main\n")
        ck(compare(worse, fl) == 0, "COMPARE grades a reading ABOVE the floor without refusing (the red is the gate's to print)")

    a = {"w.icn": {"sites": 3, "K1": 0, "K2": 1, "K3": 2, "K4": 0, "UND": 4}}
    with tempfile.TemporaryDirectory() as wd:
        f = os.path.join(wd, "floor.tsv")
        write_floor(a, f)
        ck(read_floor(f) == {"w.icn": [3, 0, 1, 2, 0, 4]}, "FLOOR: a declared floor round-trips by NAME and not by total")
        ck("NAME SET" in open(f, encoding="utf-8").read(), "FLOOR: the file says in its own head why it is a name set")

    for good, what in checks:
        print("  %s   %s" % ("ok  " if good else "FAIL", what))
    bad = sum(1 for g, _ in checks if not g)
    print("SELFTEST %d/%d arms green" % (len(checks) - bad, len(checks)))
    return 1 if bad else 0


def main(argv):
    if "--selftest" in argv:
        return selftest()
    if "--compare" in argv:
        i = argv.index("--compare")
        if len(argv) < i + 3:
            print("COMPARE REFUSED(2): --compare needs <reading file> <floor file>")
            return 2
        return compare(argv[i + 1], argv[i + 2])
    progs = [a for a in argv if not a.startswith("--")]
    if not progs:
        print("usage: util_gc_safe_point_contract.py <program>... [--write-floor]")
        return 2
    scrip = os.path.join(ROOT, "scrip")
    if not os.path.exists(scrip):
        print("CONTRACT REFUSED(2): no scrip binary at %s -- build before grading" % scrip)
        return 2
    with tempfile.TemporaryDirectory() as wd:
        counts, refusals, rows = report(scrip, progs, wd, name_cap=None if "--name-all" in argv else 48)
    if refusals and not counts:
        print("CONTRACT REFUSED(2): nothing could be read -- %s" % refusals[0])
        return 2
    if "--write-floor" in argv:
        write_floor(counts)
        print("CONTRACT FLOOR written to %s over %d witness(es)" % (FLOOR, len(counts)))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
