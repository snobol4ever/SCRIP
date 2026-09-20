#!/usr/bin/env python3
"""THE UNMAPPED-STORE CENSUS -- every emitted safe point whose shielded store lands outside its graph's frame map.

ARCH-GC-COMPILE-TIME-FRAME-MAPS.md section 3 has read, since the page was written, that a safe point is
"placed AFTER the call's result has been stored to its mapped slot".  Section 3b (CEO-996) is the finding that
NOTHING HAS EVER ENFORCED THE CLAUSE, and section 3c (CFO-134, CEO-1006) splits the population into four storage
classes.  This is the instrument the row owes first: the clause made checkable, per graph, NAMING EVERY MEMBER.

WHY A STATIC READ OF EMITTED CODE AND NOT AN rc-SHAPED ARM (CEO-997, CEO-556).  The failure this class produces is
a quiet whole-match failure that exits 0 with a plausible wrong answer -- the collector sweeps a live block, the
arena re-issues it in place, and the program prints something.  No rc can tell a cure from a silencing, so the
CURE is graded by ORACLE DIFF on the witness and the POPULATION is graded here, off the emitted text.

THE DECISION PROCEDURE, and every term in it is read from the tree rather than assumed:
  * A graph's map cell is written by its prologue as `lea rX,[rip+.Lgcmap_<g>]` / `mov [base + D], rX`, with the
    DT_MAP tag stored at [base + D-8].  The cell therefore starts at D-8.
  * gc_heap.c's gc_walk_range computes `base = cell - map_off` and walks [base, base+map_off) through the static
    layout table.  Below that it walks [p, base) as WORDS: gc_walk_words tries gc_cell_visit on each one first and
    only counts what that recognizer REFUSES into s_raw_heap, printed as [GC-WALK-SPINE].  So REGION BASE = the
    value of the cell's base register at the cell store.
  * ⛔ A STORE BELOW THE BASE IS THEREFORE NOT THE SAME FACT AS A LOST VALUE, and this census does not claim it is
    (hq_snobol4 2026-09-20, verified A/B/A by the cto; the cfo refuted the sign rule twice before that, CFO-114 and
    CFO-136 -- a negative offset is where spine words live BY CONSTRUCTION and was never the discriminator).  What
    a member means is exactly this: the safe point's correctness rests on gc_cell_visit RECOGNISING the tag that
    lands there, instead of on the frame map that ARCH-GC section 3 says should cover it.  The row's witness is
    lost today only because gc_cell_visit is missing DT_X and DT_SNUL while gc_visit_one has them -- one fact
    spelled twice in one file.  Cure that and the witness answers its oracle at every stress point WITH ITS STORE
    STILL OUTSIDE THE MAP: that surviving 162 is the residual this census exists to name.
  * The rsp displacement from the cell store to each safe point is recovered by a set-valued forward fixpoint over
    the emitted CFG.  A site the fixpoint cannot pin to ONE displacement is UNDECIDABLE and is named as such; it is
    never folded into the green count, because a census that guesses is the defect this row exists to remove.

THE ARITHMETIC IS CONFIRMED AGAINST THE RUNTIME ON THE ROW'S WITNESS.  hq_snobol4's SNO$MKEXPR program emits
`mov [rsp+0],rax / mov [rsp+8],rdx / call rt_gc_poll` at .Lcall_alpha_76_240; this census reads the poll's spine
displacement as -32, so the pointer word at [rsp+8] sits at -24 -- and SCRIP_GC_MAPS=1 prints
`[GC-WALK-SPINE] graph=main off=-24 ... text=EXPR$0$lvl1` at the same site.  Two independent roads, same byte.

VERDICTS a store target k (relative to the region base) can take:
  MAPPED         -- inside a GC_LAY_DESCR or GC_LAY_PTR_GC layout entry; the collector visits it typed.
  RAW-SLOT       -- inside a GC_LAY_RAW entry: the map tells the collector this word is never a pointer.  A heap
                    pointer stored here is a LIE to the collector, not merely an omission.
  BELOW-REGION   -- k < 0.  Class 1 of section 3c: a call result in a spine scratch cell below the region base.
  ABOVE-REGION   -- k >= map_off: the map cell, the wire header and the return address.
  GAP            -- inside the region but covered by no layout entry.
  NO-MAP         -- the graph has no map at all.  Class 2, and NOT this row's residual: it is reported as ONE
                    whole-graph entry attributed to the owning language's row (hq_raku, hq_prolog, hq_snobol4).
  UNDECIDABLE    -- the spine displacement at the site is multi-valued or unknown.  Named, never counted green.

NOT MEASURED HERE AND SAID SO RATHER THAN READ AS ZERO: class 3 of section 3c, a raw heap pointer returned in a
REGISTER PAIR (the cfo's CFO-134).  A frame map describes memory, so no store target exists to classify; this
census cannot see the class and does not claim to.  Blob frames (GC_FRAME_MAP_BLOB, section 6.2b's named
exclusion) are rbp-anchored and are classified against their own layout, not against a spine displacement.
"""
import sys, os, re, subprocess, collections

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
sys.path.insert(0, HERE)
import util_gc_callee_saved_census as CS
import util_gc_census as GCC

GCMAP_LAYOUT_RX = re.compile(r"\[GC-MAP-LAYOUT\] graph=(\S+) n=(\d+) (.*?) gaps=(\d+) conflicts=(\d+)")
LEA_MAP_RX = re.compile(r"\[rip \+ (\.Lgcmap_[A-Za-z0-9_$]+)\]")
MEM_RX = re.compile(r"\[\s*(rsp|rbp)\s*([+-]\s*-?\d+)?\s*\]")
IMM_RX = re.compile(r"^-?\d+$")
POLL_NAMES = ("rt_gc_poll",)
DELTA_CAP = 8


def disp_of(operand):
    """the (base register, byte displacement) of a simple frame operand, or (None, None)"""
    m = MEM_RX.search(operand)
    if not m:
        return None, None
    return m.group(1), int((m.group(2) or "0").replace(" ", "").replace("+-", "-"))


def read_layouts(text):
    """[GC-MAP-LAYOUT] graph=<g> n=<k> off:kind:size ... -> {graph: [(off, kind, size), ...]}"""
    out = {}
    for m in GCMAP_LAYOUT_RX.finditer(text):
        ents = []
        for tok in m.group(3).split():
            parts = tok.split(":")
            if len(parts) == 3:
                ents.append((int(parts[0]), int(parts[1]), int(parts[2])))
        out[m.group(1)] = ents
    return out


def find_anchors(insns):
    """each graph's map-cell store: {graph: (insn index, base register, cell offset from that register)}"""
    anchors = {}
    for i, ins in enumerate(insns):
        if ins.mnem != "lea" or len(ins.ops) < 2:
            continue
        m = LEA_MAP_RX.search(ins.ops[1])
        if not m or m.group(1).endswith("_s"):
            continue
        graph_label = m.group(1)[len(".Lgcmap_"):]
        for j in range(i + 1, min(i + 8, len(insns))):
            n = insns[j]
            if n.mnem == "mov" and len(n.ops) == 2 and n.ops[1] == ins.ops[0]:
                base, d = disp_of(n.ops[0])
                if base is not None:
                    anchors.setdefault(graph_label, (i, base, d - 8))
                break
    return anchors


def mangle(name):
    """bb_ab_sym_name (src/templates/bb/bb_define.cpp:40) in Python.

    ARCH-GC section 6.2d: mode 4 passes an assembler-legal prefix through this function while the map carries the
    graph's TRUE name, so a Prolog graph is `.Lgcmap_main$2F0` in the text and `main/0` in the report.  The match
    is made by MANGLING the true name, never by inverting the label: the inverse is ambiguous (a name that already
    contains a dollar followed by two hex digits demangles to a different name) and this direction is exact."""
    out = []
    for ch in name:
        if ch.isascii() and (ch.isalnum() or ch in "_$."):
            out.append(ch)
        else:
            for b in ch.encode("utf-8"):
                out.append("$%02X" % b)
    return "".join(out)


def rsp_move(ins):
    """how one instruction moves rsp: an int, or None when the move is not a compile-time constant"""
    mn, ops = ins.mnem, ins.ops
    if mn == "push":
        return -8
    if mn == "pop":
        return 8
    if mn in ("sub", "add") and len(ops) == 2 and ops[0] == "rsp":
        v = ops[1].strip()
        if IMM_RX.match(v):
            return -int(v) if mn == "sub" else int(v)
        return None
    if ops and ops[0] == "rsp" and mn in ("mov", "lea", "xchg", "and", "or", "xor", "imul"):
        return None
    return 0


def seed_rbp(insns, anchor):
    """the rbp displacement from the anchor's rsp, when the prologue established rbp before the cell store.

    Walked backwards from the cell store to the nearest function label, undoing rsp moves, so a `mov rbp, rsp`
    anywhere in the prologue is related to the anchor exactly rather than assumed to be at the same depth."""
    back = 0
    for j in range(anchor - 1, max(-1, anchor - 400), -1):
        ins = insns[j]
        if ins.mnem == "mov" and len(ins.ops) == 2 and ins.ops[0] == "rbp" and ins.ops[1] == "rsp":
            return back
        m = rsp_move(ins)
        if m is None:
            return None
        back -= m
        if ins.labels and any(not L.startswith(".L") for L in ins.labels):
            return None
    return None


def frame_fixpoint(insns, succ, start, rbp0):
    """set-valued forward fixpoint of (rsp, rbp) displacement from `start`; None means 'not a constant here'"""
    st = collections.defaultdict(set)
    st[start].add((0, rbp0))
    work = [start]
    while work:
        i = work.pop()
        ins = insns[i]
        nxt = set()
        for rsp, rbp in st[i]:
            if ins.mnem == "mov" and len(ins.ops) == 2 and ins.ops[0] == "rbp":
                nxt.add((rsp, rsp if ins.ops[1] == "rsp" else None))
                continue
            if ins.ops and ins.ops[0] == "rbp" and ins.mnem not in ("cmp", "test", "push"):
                nxt.add((rsp, None))
                continue
            if rsp is None:
                nxt.add((None, rbp))
                continue
            m = rsp_move(ins)
            nxt.add((None, rbp) if m is None else (rsp + m, rbp))
        if len(nxt) > DELTA_CAP:
            nxt = {(None, None)}
        for t in succ.get(i, ()):
            before = len(st[t])
            st[t] |= nxt
            if len(st[t]) > DELTA_CAP:
                st[t] = {(None, None)}
            if len(st[t]) != before:
                work.append(t)
    return st


def shielded_stores(insns, i):
    """the frame stores in the basic block that ends at the poll -- the stores this safe point was placed after.

    The walk back stops at a control transfer (a store on the other side of a branch does not precede this poll on
    every path) and stops AFTER the instruction that carries a label, because a label is where another path enters."""
    out = []
    j = i - 1
    while j >= 0:
        ins = insns[j]
        if CS.JCC.match(ins.mnem) or ins.mnem in ("call", "ret", "ud2"):
            break
        out.extend(_store_of(ins))
        if ins.labels:
            break
        j -= 1
    return out


def _store_of(ins):
    """the frame store this instruction performs, as [(base register, displacement, source operand)]"""
    if ins.mnem not in ("mov", "movq") or len(ins.ops) != 2:
        return []
    if "[" not in ins.ops[0] or "rip" in ins.ops[0]:
        return []
    base, d = disp_of(ins.ops[0])
    if base is None:
        return []
    return [(base, d, ins.ops[1])]


def classify(k, layout, map_off, blob):
    """the verdict for a store target at byte offset k from the frame's layout anchor"""
    for off, kind, size in layout:
        if off <= k < off + size:
            return "MAPPED" if kind in (0, 2) else "RAW-SLOT"
    if blob:
        return "OUTSIDE-LAYOUT"
    if k < 0:
        return "BELOW-REGION"
    if k >= map_off:
        return "ABOVE-REGION"
    return "GAP"


def site_label(insns, i):
    """the nearest label at or before instruction i -- what a reader greps for in the .s"""
    for j in range(i, max(-1, i - 40), -1):
        if insns[j].labels:
            return insns[j].labels[0]
    return "-"


def owner_of(base, d, i, frames):
    """(graph, offset from the frame's region base, reason-it-is-undecidable) for one store at a poll.

    The store is attributed to the graph whose map-cell anchor REACHES this instruction in the CFG. Two graphs
    reaching one site is an ambiguity that is NAMED, never a tie broken by order."""
    cand = [(g, f, f["frame"].get(i)) for g, f in frames.items() if f["frame"].get(i)]
    if not cand:
        return None, 0, "NO-ANCHOR-REACHES-SITE"
    if len(cand) > 1:
        return "+".join(c[0] for c in cand), 0, "AMBIGUOUS-OWNER"
    g, f, states = cand[0]
    if f["blob"]:
        return g, d, None if base == "rbp" else "BLOB-FRAME-NOT-RBP-ADDRESSED"
    vals = set()
    for rsp, rbp in states:
        anchor_rel = rsp if base == "rsp" else rbp
        vals.add(None if anchor_rel is None else anchor_rel + d - f["cell"] + f["map_off"])
    if None in vals:
        return g, 0, ("SPINE-DEPTH-UNKNOWN" if base == "rsp" else "RBP-NOT-A-FRAME-BASE-HERE")
    if len(vals) != 1:
        return g, 0, "FRAME-DEPTH-MULTI-VALUED"
    return g, next(iter(vals)), None


def census_asm(asm_path, report_text, tag, out=print):
    """read one emitted program; returns (members, undecidable, examined, refusal)"""
    insns = CS.parse(asm_path)
    if not insns:
        return None, None, 0, f"{tag}: {asm_path} parsed to zero instructions -- not measured"
    succ, top, label_at = CS.build_cfg(insns)
    maps = GCC.read_gcmaps(report_text)
    layouts = read_layouts(report_text)
    if not maps:
        return None, None, 0, f"{tag}: the emitter printed no [GC-MAP] line -- nothing to measure"
    by_label = {mangle(g): g for g in maps}
    frames, unmatched = {}, []
    for lbl, (ai, base, cell) in find_anchors(insns).items():
        g = by_label.get(lbl)
        if g is None:
            unmatched.append(lbl)
            continue
        m = maps[g]
        frames[g] = {"base": base, "cell": cell, "map_off": m["map_off"], "frame_bytes": m["frame_bytes"],
                     "blob": bool(m["flags"] & 4), "layout": layouts.get(g, []),
                     "frame": frame_fixpoint(insns, succ, ai, seed_rbp(insns, ai))}
    if unmatched:
        return None, None, 0, (f"{tag}: {len(unmatched)} map label(s) match no graph in the report "
                               f"({', '.join(sorted(unmatched)[:4])}) -- the mangling fact of ARCH-GC 6.2d "
                               "is not being read correctly and every site under them would read green by accident")
    members, undecidable, examined = [], [], 0
    for i, ins in enumerate(insns):
        if not (ins.mnem == "call" and ins.ops and any(p in ins.ops[0] for p in POLL_NAMES)):
            continue
        lbl = site_label(insns, i)
        for base, d, src in shielded_stores(insns, i):
            g, k, why = owner_of(base, d, i, frames)
            examined += 1
            if why:
                undecidable.append((tag, g or "-", lbl, ins.line, f"[{base}{d:+d}]", why))
                continue
            f = frames[g]
            v = classify(k, f["layout"], f["map_off"], f["blob"])
            if v != "MAPPED":
                members.append((tag, g, lbl, ins.line, k, v, src))
    return members, undecidable, examined, None


def emit_and_read(scrip, prog, workdir, env_extra=None):
    """compile one program to asm and capture the emitter's map report; returns (asm path, report text, error)"""
    base = os.path.basename(prog)
    asm = os.path.join(workdir, base + ".s")
    env = dict(os.environ)
    env["SCRIP_GC_MAPS_REPORT"] = "1"
    env.setdefault("SCRIP_HEAP_MB", "1")
    if env_extra:
        env.update(env_extra)
    try:
        r = subprocess.run([scrip, "--compile", prog], capture_output=True, text=True, env=env,
                           timeout=180, stdin=subprocess.DEVNULL)
    except (OSError, subprocess.TimeoutExpired) as e:
        return None, None, f"{base}: --compile failed: {e}"
    if r.returncode != 0 or not r.stdout.strip():
        return None, None, f"{base}: --compile rc={r.returncode} with {len(r.stdout)} bytes of asm -- not measured"
    with open(asm, "w", encoding="utf-8") as fh:
        fh.write(r.stdout)
    return asm, r.stderr, None


VERDICTS = ("BELOW-REGION", "RAW-SLOT", "GAP", "ABOVE-REGION", "OUTSIDE-LAYOUT", "NO-MAP")


def report(scrip, progs, workdir, out=print):
    """THE CENSUS.  rc 0 green, 1 red (members named), 2 refused (could not measure)."""
    if not progs:
        out("CENSUS unmapped-store REFUSED(2): no witness named -- a census over an empty population reads zero by never looking")
        return 2
    all_members, all_undec, examined, graphs_seen = [], [], 0, set()
    for prog in progs:
        tag = os.path.basename(prog)
        if not os.path.exists(prog):
            out(f"CENSUS unmapped-store REFUSED(2): witness {prog} missing"); return 2
        asm, rep, err = emit_and_read(scrip, prog, workdir)
        if err:
            out(f"CENSUS unmapped-store REFUSED(2): {err}"); return 2
        members, undec, ex, refusal = census_asm(asm, rep, tag, out=out)
        if refusal:
            out(f"CENSUS unmapped-store REFUSED(2): {refusal}"); return 2
        graphs_seen |= set(GCC.read_gcmaps(rep).keys())
        all_members += members; all_undec += undec; examined += ex
    if examined == 0:
        out("CENSUS unmapped-store REFUSED(2): zero shielded stores examined over "
            f"{len(progs)} witness(es) -- a zero has to be a zero somebody could have failed"); return 2
    counts = collections.Counter(m[5] for m in all_members)
    for tag, g, lbl, line, k, v, src in sorted(all_members, key=lambda m: (m[0], m[1], m[3])):
        out(f"CENSUS unmapped-store MEMBER {v} witness={tag} graph={g} site={lbl} line={line} off={k:+d} src={src}")
    for tag, g, lbl, line, at, why in sorted(all_undec, key=lambda m: (m[0], m[3])):
        out(f"CENSUS unmapped-store UNDECIDABLE {why} witness={tag} graph={g} site={lbl} line={line} at={at}")
    named = len(all_members)
    total = sum(counts.values())
    if named != total:
        out(f"CENSUS unmapped-store REFUSED(2): counted {total} member(s) and printed {named} name(s) -- a count "
            "without its names is a work list nobody can pick up (CTO-96, INSTRUMENT LAWS batch 30 clause 2)")
        return 2
    per = collections.defaultdict(lambda: [0, 0, 0])
    for m in all_members:
        per[m[0].rsplit(".", 1)[-1]][0] += 1
    for u in all_undec:
        per[u[0].rsplit(".", 1)[-1]][1] += 1
    for prog in progs:
        per[os.path.basename(prog).rsplit(".", 1)[-1]][2] += 1
    for lang in sorted(per):
        mem, und, wit = per[lang]
        note = ("" if und == 0 else
                "  -- ZERO MEMBERS HERE IS NOT A CLEAN READING: this language has undecidable sites and is UNMEASURED at them"
                if mem == 0 else "")
        out(f"CENSUS unmapped-store LANG {lang} witnesses={wit} members={mem} undecidable={und}{note}")
    why = collections.Counter(u[5] for u in all_undec)
    for w, n in sorted(why.items(), key=lambda kv: -kv[1]):
        out(f"CENSUS unmapped-store UNDECIDABLE-REASON {w} n={n}")
    out("CENSUS unmapped-store WHY NO-ANCHOR-REACHES-SITE IS A LIMIT AND NOT A GUESS TO BE PAPERED OVER: the wired "
        "regime enters those boxes through an INDIRECT jump, so no path from the frame's own prologue reaches them "
        "in the emitted CFG. Attributing them by textual containment inside the FN__ region was tried and REFUSED "
        "on measurement -- all 159 Prolog function regions of hb_wsb_pl_atom_dup.pl re-point rbp away from their "
        "own frame at least once (mov rbp, [rbp+N] and mov rbp, rax), so containment would grade a store against a "
        "frame that is not the one it lands in. Making these sites decidable is part of the cure, not of the census.")
    out(f"CENSUS unmapped-store witnesses={len(progs)} graphs={len(graphs_seen)} shielded_stores={examined} "
        f"members={named} undecidable={len(all_undec)} "
        + " ".join(f"{v}={counts.get(v, 0)}" for v in VERDICTS))
    out("CENSUS unmapped-store NOT MEASURED HERE AND NAMED: class 3 of ARCH-GC section 3c -- a raw heap pointer "
        "returned in a REGISTER PAIR (cfo CFO-134). A frame map describes memory, so the class presents no store "
        "target to classify; this census cannot see it and does not report zero for it.")
    if named or all_undec:
        return 1
    return 0


def selftest():
    """the arithmetic and the refusals, held against hand-built inputs rather than against a program's output"""
    ok = [0, 0]

    def arm(name, cond):
        ok[1] += 1
        ok[0] += 1 if cond else 0
        print(f"  {'ok  ' if cond else 'FAIL'} {name}")

    arm("disp_of reads the emitter's [rsp + -48] spelling", disp_of("qword ptr [rsp + -48]") == ("rsp", -48))
    arm("disp_of reads a bare [rsp]", disp_of("qword ptr [rsp]") == ("rsp", 0))
    arm("disp_of refuses a rip-relative operand", disp_of("qword ptr [rip + rtccb+40]") == (None, None))
    lay = [(0, 0, 144), (144, 1, 8), (152, 2, 16), (168, 1, 72), (240, 0, 176)]
    arm("classify: a DESCR entry is MAPPED", classify(0, lay, 416, False) == "MAPPED")
    arm("classify: a PTR_GC entry is MAPPED", classify(152, lay, 416, False) == "MAPPED")
    arm("classify: a RAW entry is a LIE, not a miss", classify(144, lay, 416, False) == "RAW-SLOT")
    arm("classify: below the region base is the witness class", classify(-24, lay, 416, False) == "BELOW-REGION")
    arm("classify: at or above map_off is the header", classify(416, lay, 416, False) == "ABOVE-REGION")
    arm("classify: a hole inside the region is a GAP", classify(200, [(0, 0, 16)], 416, False) == "GAP")
    arm("read_layouts parses the emitter's line",
        read_layouts("[GC-MAP-LAYOUT] graph=main n=2 0:0:16 16:1:8 gaps=0 conflicts=0")["main"] == [(0, 0, 16), (16, 1, 8)])
    arm("read_gcmaps is the harness's reader, not a copy",
        GCC.read_gcmaps("[GC-MAP] graph=main frame_bytes=432 header_bytes=0 map_off=416 flags=9")["main"]["map_off"] == 416)
    arm("report REFUSES an empty population", report("/nonexistent", [], "/tmp") == 2)
    print(f"SELFTEST {ok[0]}/{ok[1]} arms green")
    return 0 if ok[0] == ok[1] else 1


def main(argv):
    import argparse, tempfile
    ap = argparse.ArgumentParser(description="census of emitted safe points whose shielded store lands outside the frame map")
    ap.add_argument("progs", nargs="*")
    ap.add_argument("--scrip", default=os.path.join(ROOT, "scrip"))
    ap.add_argument("--selftest", action="store_true")
    a = ap.parse_args(argv)
    if a.selftest:
        return selftest()
    with tempfile.TemporaryDirectory(prefix="gc_unmapped_") as wd:
        return report(a.scrip, a.progs, wd)


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
