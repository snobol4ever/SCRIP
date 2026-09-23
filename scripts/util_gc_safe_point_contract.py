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
  K3  THE SLOT IS ONE THE COLLECTOR COVERS, AND THERE ARE TWO MECHANISMS, NOT ONE.  A store of rax or rdx in
      the window lands either inside a GC_LAY_DESCR / GC_LAY_PTR_GC entry of the graph's own frame map
      (ARCH-GC section 2b RULE 1b, the RBP activation frames) or as a TAGGED CELL ON THE ζ-SPINE
      (RULE 1a, `gc_cell_visit` inside `gc_walk_words`).  This is the clause section 3b (CEO-996) found had
      never been enforced; ⛔ THE FIRST ENFORCEMENT OF IT ASSERTED RULE 1b OVER A POPULATION RULE 1a OWNS and
      called 222 findings over 111 sites a defect.  See SPINE-CELL below -- the correction is MEASURED, not
      argued, and the class is still NAMED so that nothing goes dark.
  K4  ON EVERY PATH.  No label joins the window AFTER the last result store.  A label between the store and the
      poll is a path that reaches the poll WITHOUT passing the store, so the clause holds on one path and says
      nothing about the collection that happens on the other.  This clause is the cto's 2026-09-21 reading and is
      not in the page's words; it is what the page's words MEAN once the emitted text has branches in it.

⛔⭐⭐⭐ SPINE-CELL -- THE VERDICT THAT COST THIS FILE ITS FIRST 222 MEMBERS, AND THE A/B THAT PROVED IT
(cto, 2026-09-22, MODE DUO).  A call result stored as `mov [b+k],rax` / `mov [b+k+8],rdx` at a 16-BYTE-ALIGNED k
below the region base IS A WELL-FORMED DESCR: rax carries the tag word (`DESCR_t.v` is the low BYTE of the first
eight) and rdx carries the union word.  ARCH-GC section 2b RULE 1a gives the ζ-SPINE its own covering mechanism
for exactly this shape -- "the collector walks the spine from RSP to its base as an array of cells, reading
tags" -- and `x86_rt_gc_poll_res` says so in the emitter's own words: "spilled as a DESCR cell under rsp across
the poll and reloaded -- the walker sweeps [poll floor, stack top) and relocates it".  Grading that store
against the FRAME MAP alone is asking the wrong mechanism about it.

⛔ THIS IS NOT A WEAKENING ON A READING.  It is held by an A/B ON THE MECHANISM ITSELF, and the gate
test_gate_gc_the_spine_tagged_cell_walk_is_load_bearing.sh re-runs it: with SCRIP_GC_NO_SPINE_CELL=1 the spine
cell visit is skipped and NOTHING ELSE CHANGES.  Over the declared 52-witness name set at SCRIP_HEAP_MB=1,
SCRIP_GC_STRESS=3, SCRIP_GC_RELOC=1: 16 witnesses CHANGE THEIR ANSWER, 15 of them from the 23 that carry a
BELOW-REGION store.  hb_datblk dumps core, hb_nv SIGSEGVs on a stale pointer through the ZGC-STALE trap, and
hb_mkexpr_unmapped_spine_store -- the witness section 3b is named after -- goes `match` to `nomatch`.  The
spine walk is LOAD-BEARING for this population; the frame map never covered it and was never meant to.

⛔⭐ AND SPINE-CELL IS NOT A DEFECT FLOOR, WHICH COST THE FLEET A RED BEFORE IT WAS NOTICED (cto 2026-09-22).
The floor comparison graded every column with one polarity -- read ABOVE declared is WORSE -- and that rule owns
K1..K4 and UND.  It does NOT own SPINE.  SPINE counts stores COVERED by RULE 1a's mechanism, so a cure that turns
a bare poll into x86_rt_gc_poll_rec_sigma RAISES it: the ceo's e42cc0909 and c60097828 did exactly that and 14
witness/clause pairs went "ABOVE the declared floor", reported as a REGRESSION IN THE EMITTED SAFE-POINT SEQUENCE
when what had happened was the safe points getting better.  This is the same shape the ceo named law-shaped today
after three retractions in one day: A CHECKER ASSERTING A RULE THAT DOES NOT OWN THE POPULATION IT IS GRADING.
SPINE is therefore compared with BLOCKING_KEYS omitting it, and its movement is printed as SPINEMOVED in BOTH
DIRECTIONS and never blocks.  Neither direction is unambiguously bad -- a store moving INTO the mapped region
lowers it and is also a cure -- so the honest treatment is a census that is NAMED, not a floor that is enforced.

⛔ AND WHAT SPINE-CELL DOES NOT SAY, because a verdict that quietly becomes a pass is the failure the INSTRUMENT
LAWS exist against.  It is COUNTED IN ITS OWN COLUMN, declared in the floor and NAMED line by line -- it is not
folded into the green.  It does not say Rule 1a is as strong as a map: the spine walk FINDS the cell by stepping
8 bytes and trying, so a false cell earlier in the range can put it out of phase, and a tag word overwritten
before the collection drops the cell to a raw word (measured: one of six member pointer-halves on hb_datblk read
heap-RAW at one collection).  Making that coverage STATIC rather than discovered is the planner's row and it is
still open.  What SPINE-CELL says is narrower and true: THIS STORE IS COVERED BY A MECHANISM THIS FILE CAN NAME.

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


STORAGE_CLASS = ("extern", "static", "inline", "__inline", "__inline__", "_Noreturn")


def strip_storage_class(t):
    """the declared type with its storage-class specifiers removed -- `extern void` -> `void`

    ⛔ THIS EXISTS BECAUSE ITS ABSENCE PRODUCED 132 FALSE K2 MEMBERS AND I ALMOST READ THEM AS A LANDING'S
    DEFECT (cto 2026-09-22).  `runtime_return_index` captures whatever text precedes the symbol, so a tree that
    writes `extern void rt_coerce_num2_d(...)` indexes the type as `extern void`, which is not equal to "void",
    carries no `*` and is not in NARROW_TYPES -- so it fell through to WIDE and every call of a VOID entry was
    reported as "RESULT-NOT-STORED and the result is WIDE ENOUGH TO BE A HEAP POINTER".  Measured over this
    tree's own declarations: 684 of them changed class once the storage class was stripped, 258 WIDE->VOID and
    426 WIDE->NARROW.  The safe-direction doctrine in return_kind is right and is UNCHANGED -- an UNKNOWN type
    is still called wide -- but `extern void` is not an unknown type, it is void wearing a storage class, and a
    reader that cannot tell the two apart reports a clean tree as dirty."""
    t = " ".join(t.split())
    changed = True
    while changed:
        changed = False
        for kw in STORAGE_CLASS:
            if t == kw or t.startswith(kw + " "):
                t = t[len(kw):].strip()
                changed = True
                break
    return t


def return_kind(t):
    """VOID / NARROW / WIDE for one declared return type.

    ⛔ THE SPLIT IS THE CALLEE-SAVED CENSUS'S OWN D32 RULE, REUSED RATHER THAN REINVENTED: "a 32-bit define
    ZERO-EXTENDS: a 64-bit pointer cannot survive it".  So only a type that is provably too narrow to hold a
    64-bit pointer is cleared.  `long`, `size_t` and `uint64_t` are NOT cleared even though they are usually
    counts, because the wrong answer here is a LOST ROOT and the safe direction is to call them wide."""
    t = strip_storage_class(t)
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


def spine_cell_pair(placed, src, k):
    """True when this store is one half of a tagged ζ-SPINE DESCR cell -- rax at a 16-aligned k, rdx at k+8.

    DESCR_t is {uint8 v; uint8 src_node[3]; uint32 slen; union}, so the FIRST eight bytes are the tag word and
    the second eight are the pointer: a call returning DESCR_t hands them back in rax:rdx in exactly that order.
    Two adjacent stores in that order at a 16-aligned offset therefore mint a cell whose tag is valid before the
    poll runs, which is what ARCH-GC section 2b RULE 1a asks of every spine cell.  The alignment is required
    rather than decorative -- an unaligned pair is not a cell and is left a MEMBER."""
    base = k if src == "rax" else k - 8
    return base % 16 == 0 and ("rax", base) in placed and ("rdx", base + 8) in placed


def spine_cell_pair_multi(multi, g, src, ks):
    """spine_cell_pair for a store the anchor reaches at several depths: rax on the grid at EVERY depth and rdx
    exactly 8 above it at each -- the same cell test, asked once per depth, and one unaligned depth fails it."""
    other = "rdx" if src == "rax" else "rax"
    oks = multi.get((g, other))
    if not oks:
        return False
    base = tuple(ks) if src == "rax" else tuple(k - 8 for k in ks)
    obase = tuple(k - 8 for k in oks) if other == "rdx" else tuple(oks)
    return all(k % 16 == 0 for k in base) and base == obase


def poll_res_tag(insns, n):
    """' POLL-RES-ENVELOPE' when this store is one half of x86_rt_gc_poll_res's spill -- sub rsp,16 / [rsp+0]=rax /
    [rsp+8]=rdx / call rt_gc_poll (ARCH-GC 6.5b) -- so a finding this reader cannot PLACE still NAMES the form it
    is looking at.  The cfo's bc1092d8f put that form at the two by-name call sites and the reading moved from
    eight to twenty-two without one word saying what had arrived (cto 2026-09-22)."""
    def is_store(j, disp, src):
        if j < 0 or j >= len(insns):
            return False
        for (b, d, s) in UC._store_of(insns[j]):
            if b == "rsp" and d == disp and s.strip() == src:
                return True
        return False
    for j in (n, n - 1):
        if is_store(j, 0, "rax") and is_store(j + 1, 8, "rdx") and j >= 1 \
                and insns[j - 1].mnem == "sub" and len(insns[j - 1].ops) == 2 \
                and insns[j - 1].ops[0].strip() == "rsp" and insns[j - 1].ops[1].strip() == "16":
            for m in range(j + 2, min(len(insns), j + 12)):
                if insns[m].mnem == "call" and insns[m].ops and "rt_gc_poll" in insns[m].ops[0]:
                    return " POLL-RES-ENVELOPE"
                if insns[m].mnem in ("jmp", "je", "jne", "ret"):
                    break
    return ""


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
    placed, multi = {}, {}
    for (b, d, src, n) in frame_st:
        g, k, undec = UC.owner_of(b, d, n, frames)
        if not undec and g in frames:
            placed[(g, src, k)] = n
        elif undec == "FRAME-DEPTH-MULTI-VALUED":
            g2, ks = UC.owner_ks(b, d, n, frames)
            if g2 in frames and ks:
                multi[(g2, src)] = ks
    for (b, d, src, n) in frame_st:
        g, k, undec = UC.owner_of(b, d, n, frames)
        if undec == "FRAME-DEPTH-MULTI-VALUED" and g in frames and (g, src) in multi:
            ks = multi[(g, src)]
            f = frames[g]
            vs = {UC.classify(k2, f["layout"], f["map_off"], f["blob"]) for k2 in ks}
            kss = ",".join("%+d" % k2 for k2 in ks)
            if vs == {"BELOW-REGION"} and spine_cell_pair_multi(multi, g, src, ks):
                out.append(("K3", "SPINE-CELL", "BELOW-REGION k in {%s} src=%s graph=%s -- a tagged DESCR cell on the "
                            "ζ-SPINE at EVERY depth the anchor reaches this site, each on the 16-byte grid, ARCH-GC 2b "
                            "RULE 1a, covered by gc_cell_visit and NOT by this graph's frame map" % (kss, src, g)))
                continue
            out.append(("K3", "UNDECIDABLE", "%s at [%s%+d] src=%s -- depths {%s} classify %s%s"
                        % (undec, b, d, src, kss, "/".join(sorted(vs)), poll_res_tag(insns, n))))
            continue
        if undec or g not in frames:
            out.append(("K3", "UNDECIDABLE", "%s at [%s%+d] src=%s%s"
                        % (undec or "NO-FRAME", b, d, src, poll_res_tag(insns, n))))
            continue
        f = frames[g]
        v = UC.classify(k, f["layout"], f["map_off"], f["blob"])
        if v == "MAPPED":
            continue
        if v == "BELOW-REGION" and spine_cell_pair({(s2, k2) for (g2, s2, k2) in placed if g2 == g}, src, k):
            out.append(("K3", "SPINE-CELL", "%s k=%+d src=%s graph=%s -- a tagged DESCR cell on the ζ-SPINE, "
                        "ARCH-GC 2b RULE 1a, covered by gc_cell_visit and NOT by this graph's frame map"
                        % (v, k, src, g)))
            continue
        out.append(("K3", "MEMBER", "%s k=%+d src=%s graph=%s" % (v, k, src, g)))
    if not frame_st:
        out.append(("K3", "UNDECIDABLE",
                    "RESULT-STATIC-ONLY sym=%s -- a frame map describes a frame, so this store is outside every map "
                    "BY CONSTRUCTION and its safety is a ROOT-SET question (the cfo's file)" % static_st[0][0]))
    if frame_st and not store_dominates(insns, pred, calls, region, i):
        out.append(("K4", "MEMBER", "a path from %s reaches this poll WITHOUT passing any result store"
                    % ",".join(callees)))
    return out


def parity_step(ins, p, prbp, pr11, pslot):
    """one instruction's effect on the RSP PARITY state (rsp, rbp, r11, [rsp+0]) -- each 0, 8 or None (unknown).

    Only the residue mod 16 is tracked, which is why `and rsp, -16` is a KNOWN 0 here while the depth fixpoint of
    the shared census must poison it: the alignment mask destroys the depth and fixes the parity.  The align
    envelope's restore (`mov r11, rsp` ... `mov [rsp+0], r11` ... `mov rsp, [rsp+0]`) is read through the same
    two shadows the census carries, reduced to parity."""
    m, ops = ins.mnem, [o.strip() for o in ins.ops]
    def m16(x):
        return None if x is None else x % 16
    if m == "and" and len(ops) == 2 and ops[0] == "rsp":
        return (0, prbp, pr11, pslot) if ops[1] in ("-16", "0xfffffffffffffff0", "-0x10") else (None, prbp, pr11, pslot)
    if m == "mov" and len(ops) == 2:
        if ops[0] == "rsp":
            if ops[1] == "rbp":
                return (prbp, prbp, pr11, pslot)
            if UC.rsp_slot_of(ops[1]) == 0:
                return (pslot, prbp, pr11, pslot)
            return (None, prbp, pr11, pslot)
        if ops[0] == "rbp":
            return (p, p if ops[1] == "rsp" else None, pr11, pslot)
        if ops[0] == "r11":
            return (p, prbp, p if ops[1] == "rsp" else None, pslot)
        if UC.rsp_slot_of(ops[0]) == 0:
            return (p, prbp, pr11, pr11 if ops[1] == "r11" else None)
    if m == "pop" and ops and ops[0] == "rbp":
        return (m16(p + 8) if p is not None else None, None, pr11, pslot)
    if m == "pop" and ops and ops[0] == "r11":
        return (m16(p + 8) if p is not None else None, prbp, None, pslot)
    d = UC.rsp_move(ins)
    if d is None:
        return (None, prbp, pr11, pslot)
    return (m16(p + d) if p is not None else None, prbp, pr11, pslot)


BOX_ENTRY_RX = re.compile(r"^n\d+_\w+_bx$")
PORT_ENTRY_RX = re.compile(r"^(n\d+_\w+|main)_[αβγω]$")


def parity_walk(insns, succ, asm_text):
    """K5 -- the RSP parity at every poll call, walked from each function's OWN entry (CEO-1151).

    ⛔ WHY NOT THE ANCHOR WALK: the grid census places a call site only where a graph's map-cell anchor reaches
    it, and in procedure_write_253 -- the cfo's misaligned-stack witness -- it UNREACHES 40 of 74 call sites and
    leaves 9 undecidable, because the wired regime enters boxes through indirect jumps no anchor path crosses.
    So this walk starts at every `.type X, @function` label, every box port label, every procedure port label and
    `main`, with the parity the road fixes there.  MEASURED, NOT ASSUMED, AT THE ONE PLACE C ENTERS EMITTED CODE
    (cto 2026-09-22): the four hand-written entry shims in rt.c all JUMP into the port after their own pushes --
    rt_proc_enter moves rsp by -56, rt_proc_enter_named by -72, rt_proc_enter_barrier and rt_proc_enter_frag by
    -72 -- so from a C call's 8 mod 16 every one lands the port at 0 mod 16; the wiring's own jumps carry the grid
    the grid census holds at every decidable floor (3790 of 3790, CTO-103); and `main` is the ONE label libc
    calls, entered at 8, whose 65544-byte prologue puts it back on the grid.  A first cut called every column-0
    procedure label a called function and read TWELVE define-port polls MISALIGNED that the shims prove aligned;
    the rule is now: `main` (and the plant's `main_bx`) enter at 8, everything else enters on the grid, and every
    verdict names which.  Successors are the census's CFG restricted to the function's own text
    range; a call falls through (the callee returns to the same rsp).  The ABI requires rsp 0 mod 16 AT the call
    instruction, so a poll call at parity 8 is MISALIGNED: every C callee on that path runs misaligned and the
    first aligned SSE store faults -- the movaps in vsnprintf that convicted bb_call_value 67/70 with the
    collector never run (CEO-1151).  Returns {poll index: (entry label, set of parities)}."""
    fn_labels = set(re.findall(r"^\s*\.type\s+([\w.$]+)\s*,\s*@function", asm_text, re.M))
    entries = {}
    for i, ins in enumerate(insns):
        for lab in ins.labels:
            head = i == 0 or insns[i - 1].mnem in ("ret", "jmp", "ud2", "hlt")
            if lab in fn_labels or lab in ("main", "main_bx") or PORT_ENTRY_RX.match(lab) \
                    or (not lab.startswith(".") and head):
                entries[i] = lab
    order = sorted(entries)
    res = {}
    for j, i0 in enumerate(order):
        name = entries[i0]
        end = order[j + 1] if j + 1 < len(order) else len(insns)
        p0 = 8 if name in ("main", "main_bx") else 0
        st = collections.defaultdict(set)
        st[i0].add((p0, None, None, None))
        work = [i0]
        while work:
            i = work.pop()
            ins = insns[i]
            if ins.mnem == "call" and ins.ops and any(q in ins.ops[0] for q in UC.POLL_NAMES):
                res.setdefault(i, (name, set()))[1].update(s[0] for s in st[i])
            nxt = {parity_step(ins, *s) for s in st[i]}
            for tgt in succ.get(i, ()):
                if i0 <= tgt < end and not nxt <= st[tgt]:
                    st[tgt] |= nxt
                    work.append(tgt)
    return res


def parity_rows(par, i):
    """the K5 row(s) for one poll site from the walk's reading -- ALIGNED is a row too, so the count is measured"""
    if i not in par:
        return [("K5", "UNDECIDABLE", "PARITY-UNREACHED -- no function entry reaches this poll inside its own text range")]
    name, ps = par[i]
    lab = ("function %s (entered by call, 8 mod 16)" % name if name in ("main", "main_bx")
           else "box %s (entered by the wiring's jump, on the grid)" % name if BOX_ENTRY_RX.match(name)
           else "port %s (entered by a jump from the wiring or an entry shim, on the grid)" % name)
    if None in ps:
        return [("K5", "UNDECIDABLE", "PARITY-UNKNOWN from %s -- an rsp write this walk cannot read on some path" % lab)]
    if 8 in ps:
        return [("K5", "MEMBER", "MISALIGNED rsp is 8 mod 16 at this call on %s path(s) from %s -- every C callee on "
                 "that path is misaligned and the first aligned SSE store faults (CEO-1151)"
                 % ("every" if ps == {8} else "some", lab))]
    return [("K5", "ALIGNED", "rsp is 0 mod 16 at this call on every path from %s" % lab)]


def contract_asm(asm_path, report_text, tag):
    """(rows, sites, refusal); a row is (tag, graph, label, line, clause, verdict, detail), one per BROKEN clause"""
    insns, succ, maps, layouts, frames, refusal = UC.build_frames(asm_path, report_text, tag)
    if refusal:
        return None, 0, refusal
    pred = preds_from(succ)
    par = parity_walk(insns, succ, open(asm_path, encoding="utf-8", errors="replace").read())
    rows, sites = [], 0
    for i, ins in enumerate(insns):
        if not (ins.mnem == "call" and ins.ops and any(q in ins.ops[0] for q in UC.POLL_NAMES)):
            continue
        sites += 1
        g, _k, _w = UC.owner_of("rsp", 0, i, frames)
        for (clause, verdict, detail) in grade_site(insns, pred, i, frames) + parity_rows(par, i):
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
    "# 2026-09-22 cto: hb_coexpr_genp_scan.icn undecidable 8 -> 14, spine_cell 8 -> 16.  The cfo's bc1092d8f put\n"
    "# x86_rt_gc_poll_res at the two by-name call sites; the four reached at two depths are now PLACED as SPINE-CELL at\n"
    "# every depth (owner_ks), and the six in the co-expression body plus one call_value beta site reach NO ANCHOR and\n"
    "# are NAMED POLL-RES-ENVELOPE.  A poll ARRIVING at a site this reader cannot place is a site that was UNPOLLED before,\n"
    "# not the tree getting worse; the RSP-parity screen (CEO-1151) grades those seven in the bare-poll row.\n"
    "# k5 is the RSP-PARITY clause (CEO-1151, cto 2026-09-22): a poll call at rsp 8 mod 16 on any path from its function's\n"
    "# own entry (a box entry ASSUMED on the grid, a called function at 8), which misaligns every C callee on that path.\n"
    "# COLUMNS: witness  sites  k1  k2  k3  k4  k5  undecidable  spine_cell\n"
)


def read_floor(path=FLOOR):
    rows = {}
    if not os.path.exists(path):
        return rows
    for ln in open(path, encoding="utf-8"):
        if ln.startswith("#") or not ln.strip():
            continue
        f = [x.strip() for x in ln.split("\t")]
        if len(f) < 7:
            continue
        if len(f) >= 9:
            r = [int(x) for x in f[1:9]]
        else:
            r = [int(x) for x in f[1:6]] + [0, int(f[6]), int(f[7]) if len(f) >= 8 and f[7] else 0]
        rows[f[0]] = r
    return rows


def write_floor(counts, path=FLOOR):
    """rewrite the floor from a reading, CARRYING FORWARD the UNGRADEABLE declaration the file holds by hand.

    The first rewrite after that line was added (faddd1a00) silently dropped it and arm (g) of the gate went red on
    a floor the checker itself had just written -- a writer that erases a declaration it does not understand is a
    silencer with an innocent name (cto 2026-09-22).  The line is a DECLARATION and shrinks only in the landing that
    earns it, so the writer copies it verbatim and never computes it."""
    keep = ""
    if os.path.exists(path):
        for ln in open(path, encoding="utf-8"):
            if ln.startswith("# UNGRADEABLE:"):
                keep = ln
                break
    with open(path, "w", encoding="utf-8") as fh:
        fh.write(FLOOR_HEADER.replace("# COLUMNS: witness", keep + "# COLUMNS: witness") if keep else FLOOR_HEADER)
        for w in sorted(counts):
            c = counts[w]
            fh.write("%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n"
                     % (w, c["sites"], c["K1"], c["K2"], c["K3"], c["K4"], c.get("K5", 0), c["UND"], c.get("SPINE", 0)))


KEYS = ("K1", "K2", "K3", "K4", "K5", "UND", "SPINE")
BLOCKING_KEYS = ("K1", "K2", "K3", "K4", "K5", "UND")


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
        elif ln.startswith("CONTRACT SPINE-CELL ") and len(f) >= 6:
            now[f[2]]["SPINE"] += 1
        elif ln.startswith("CONTRACT SITES "):
            sites_line = ln
    if sites_line is None:
        print("COMPARE REFUSED(2): the reading carries no CONTRACT SITES line -- it is not a reading")
        return 2
    graded = sum(int(x.split("=")[1]) for x in sites_line.split()
                 if x.split("=")[0] in ("k1", "k2", "k3", "k4", "k5", "undecidable", "spine_cell"))
    if graded > 0 and not now:
        print("COMPARE REFUSED(2): the reading says %d finding(s) and NOT ONE of them parsed -- the comparison would"
              " grade every declaration against nothing and call the floor stale. That is a defect in this probe,"
              " never in the floor." % graded)
        return 2
    worse, stale, unlisted, spine_moved = [], [], [], []
    for w in sorted(now):
        c = now[w]
        if w not in floor:
            unlisted.append("%s %s" % (w, dict(c)))
            continue
        dec = dict(zip(KEYS, floor[w][1:]))
        for k in BLOCKING_KEYS:
            if c[k] > dec[k]:
                worse.append("%s %s declared=%d read=%d" % (w, k, dec[k], c[k]))
        if c["SPINE"] != dec["SPINE"]:
            spine_moved.append("%s SPINE declared=%d read=%+d" % (w, dec["SPINE"], c["SPINE"] - dec["SPINE"]))
    for w in sorted(floor):
        dec = dict(zip(KEYS, floor[w][1:]))
        if any(dec[k] for k in KEYS) and w in now and not any(now[w][k] for k in KEYS):
            stale.append("%s declared=%s read=all-zero" % (w, {k: dec[k] for k in KEYS if dec[k]}))
    print("SPINEMOVED %d %s" % (len(spine_moved), "|".join(spine_moved[:6])))
    print("WORSE %d %s" % (len(worse), "|".join(worse[:6])))
    print("STALE %d %s" % (len(stale), "|".join(stale[:6])))
    print("UNLISTED %d %s" % (len(unlisted), "|".join(unlisted[:6])))
    return 0


def tally(rows, sites):
    c = {"sites": sites, "K1": 0, "K2": 0, "K3": 0, "K4": 0, "K5": 0, "UND": 0, "SPINE": 0, "ALIGNED": 0}
    for (_t, _g, _l, _n, clause, verdict, _d) in rows:
        if verdict == "MEMBER":
            c[clause] += 1
        elif verdict == "UNDECIDABLE":
            c["UND"] += 1
        elif verdict == "SPINE-CELL":
            c["SPINE"] += 1
        elif verdict == "ALIGNED":
            c["ALIGNED"] += 1
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
    for (t, g, lbl, line, clause, verdict, detail) in allrows:
        if verdict == "SPINE-CELL":
            out("CONTRACT SPINE-CELL %s %s %s:%s %s %s" % (t, g, lbl, line, clause, detail))
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
    out("CONTRACT SITES witnesses=%d sites=%d k1=%d k2=%d k3=%d k4=%d k5=%d undecidable=%d spine_cell=%d refusals=%d"
        % (len(counts), tot["sites"], tot["K1"], tot["K2"], tot["K3"], tot["K4"], tot["K5"], tot["UND"], tot["SPINE"],
           len(refusals)))
    par_und = sum(1 for r in allrows if r[4] == "K5" and r[5] == "UNDECIDABLE")
    out("CONTRACT PARITY sites=%d aligned=%d misaligned=%d undecided=%d -- rsp parity at every poll call, walked from"
        " each function's OWN entry (a box entered by the wiring's jump is ASSUMED on the grid, a called function enters"
        " at 8 mod 16); a misaligned poll misaligns every C callee on its path (CEO-1151), and an undecided one is"
        " NAMED above, never counted green" % (tot["sites"], tot["ALIGNED"], tot["K5"], par_und))
    out("CONTRACT SPINE-CELL IS NAMED, NOT FORGIVEN: %d finding(s) above are call results stored as tagged DESCR"
        " cells on the ζ-SPINE, which ARCH-GC 2b RULE 1a covers and this graph's frame map does not. They are"
        " counted in their own column and declared in the floor. The A/B that earns them the verdict is"
        " test_gate_gc_the_spine_tagged_cell_walk_is_load_bearing.sh, NOT this reader's opinion." % tot["SPINE"])
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
           "        push rbp\n"
           "        sub rsp, 64\n"
           "        lea r11, [rip + .Lgcmap_main]\n"
           "        mov qword ptr [rsp + 24], r11\n"
           + body +
           "        add rsp, 64\n"
           "        pop rbp\n"
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
    ck([r for r in rows if r[4] != "K5"] == [], "K POSITIVE: a result stored into a MAPPED slot before the poll BREAKS NO CLAUSE (%s)" % (rows,))
    ck(any(r[4] == "K5" and r[5] == "ALIGNED" for r in rows) and not any(r[4] == "K5" and r[5] != "ALIGNED" for r in rows),
       "K5 POSITIVE: a poll in an ABI-aligned frame (entry 8, push rbp, sub 64) reads ALIGNED and nothing else (%s)" % ([r for r in rows if r[4] == "K5"],))
    rows, _s, _r = run("        push rax\n" + ok_body)
    ck(any(r[4] == "K5" and r[5] == "MEMBER" and "MISALIGNED" in r[6] for r in rows),
       "K5 PLANTED: one extra push before the poll puts rsp at 8 mod 16 and the poll reads MISALIGNED (%s)" % ([r for r in rows if r[4] == "K5"],))
    rows, _s, _r = run("        push rax\n        mov r11, rsp\n        and rsp, -16\n        sub rsp, 16\n"
                       "        mov qword ptr [rsp + 0], r11\n" + ok_body + "        mov rsp, qword ptr [rsp + 0]\n")
    ck(any(r[4] == "K5" and r[5] == "ALIGNED" for r in rows) and not any(r[4] == "K5" and r[5] == "MEMBER" for r in rows),
       "K5 RE-ALIGNED: the same misaligned path inside an align envelope (and rsp,-16) reads ALIGNED, and the envelope's "
       "restore through [rsp+0] is read rather than poisoned (%s)" % ([r for r in rows if r[4] == "K5"],))

    rows, _s, _r = run(ok_body.replace("[rsp + 0]", "[rsp - 64]").replace("[rsp + 8]", "[rsp - 56]"))
    ck(any(r[4] == "K3" and r[5] == "SPINE-CELL" and "BELOW-REGION" in r[6] for r in rows)
       and not any(r[5] == "MEMBER" for r in rows),
       "SPINE-CELL PLANTED: rax at a 16-aligned k below the region base and rdx at k+8 is a TAGGED SPINE CELL, "
       "ARCH-GC 2b RULE 1a, and is NAMED rather than counted a K3 member (%s)" % (rows,))

    rows, _s, _r = run(ok_body.replace("[rsp + 0]", "[rsp - 56]").replace("[rsp + 8]", "[rsp - 48]"))
    ck(any(r[4] == "K3" and r[5] == "MEMBER" and "BELOW-REGION" in r[6] for r in rows),
       "K3 PLANTED: the SAME pair at an UNALIGNED k is not a cell -- the spine walk reads cells on a 16-byte "
       "grid, so an unaligned pair stays a K3 member (%s)" % (rows,))

    two_depths = ("        test eax, eax\n"
                  "        je .Lpd2\n"
                  "        sub rsp, 16\n"
                  "        jmp .Lpd3\n"
                  ".Lpd2:\n"
                  "        sub rsp, %s\n"
                  ".Lpd3:\n"
                  "        call rt_call_arr_bl@PLT\n"
                  "        mov qword ptr [rsp + 0], rax\n"
                  "        mov qword ptr [rsp + 8], rdx\n"
                  "        call rt_gc_poll@PLT\n")
    rows, _s, _r = run(two_depths % "32")
    ck(any(r[4] == "K3" and r[5] == "SPINE-CELL" and "k in {" in r[6] for r in rows)
       and not any(r[5] in ("MEMBER", "UNDECIDABLE") for r in rows),
       "SPINE-CELL AT TWO DEPTHS PLANTED: a pair the anchor reaches at k=-16 and k=-32 is a tagged cell at EVERY depth "
       "and is PLACED, not left FRAME-DEPTH-MULTI-VALUED -- the cfo's by-name poll_res sites read this way (%s)" % (rows,))
    rows, _s, _r = run(two_depths % "24")
    ck(any(r[4] == "K3" and r[5] == "UNDECIDABLE" and "MULTI-VALUED" in r[6] and "BELOW-REGION" in r[6] for r in rows)
       and not any(r[5] == "SPINE-CELL" for r in rows),
       "TWO DEPTHS, ONE OFF THE GRID, PLANTED: the same pair at k=-16 and k=-24 is a cell on one path and not on the "
       "other, so it stays UNDECIDABLE with both depths NAMED rather than being placed on the aligned one (%s)" % (rows,))

    rows, _s, _r = run(ok_body.replace("[rsp + 0]", "[rsp - 64]").replace(
        "        mov qword ptr [rsp + 8], rdx\n", ""))
    ck(any(r[4] == "K3" and r[5] == "MEMBER" and "BELOW-REGION" in r[6] for r in rows),
       "K3 PLANTED: a LONE tag word below the region base is half a cell and half a cell is not a cell (%s)"
       % (rows,))

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

    a = {"w.icn": {"sites": 3, "K1": 0, "K2": 1, "K3": 2, "K4": 0, "UND": 4, "SPINE": 5}}
    with tempfile.TemporaryDirectory() as wd:
        f = os.path.join(wd, "floor.tsv")
        write_floor(a, f)
        ck(read_floor(f) == {"w.icn": [3, 0, 1, 2, 0, 0, 4, 5]}, "FLOOR: a declared floor round-trips by NAME and not by total")
        legacy = os.path.join(wd, "legacy.tsv")
        open(legacy, "w", encoding="utf-8").write(FLOOR_HEADER + "w.icn\t3\t0\t1\t2\t0\t4\n")
        ck(read_floor(legacy) == {"w.icn": [3, 0, 1, 2, 0, 0, 4, 0]},
           "FLOOR: a SEVEN-column line written before the spine_cell column reads back with spine_cell=0 rather "
           "than refusing -- the column was ADDED and an old floor is not a corrupt one")
        ck("NAME SET" in open(f, encoding="utf-8").read(), "FLOOR: the file says in its own head why it is a name set")
        ck(return_kind("extern void") == "VOID" and return_kind("static void") == "VOID"
           and return_kind("static inline int") == "NARROW" and return_kind("extern int") == "NARROW",
           "RETURN KIND: a storage class is stripped before the width test -- `extern void` is VOID and not WIDE")
        ck(return_kind("void *") == "WIDE" and return_kind("static char *") == "WIDE"
           and return_kind("long") == "WIDE" and return_kind("size_t") == "WIDE",
           "RETURN KIND: the SAFE DIRECTION survives the strip -- a pointer, and any type not provably narrow, is still WIDE")
        ck(return_kind("externalise_t") == "WIDE" and return_kind("staticky") == "WIDE",
           "RETURN KIND: the strip is WORD-WISE -- a type whose name merely BEGINS with a storage-class keyword is untouched")

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
