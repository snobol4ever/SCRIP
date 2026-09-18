#!/usr/bin/env python3
"""util_gc_callee_saved_census.py -- WHAT IS IN A CALLEE-SAVED REGISTER AT AN ALLOCATING RETURN, READ OFF THE
EMITTED CODE INSTEAD OF OFF A PARAGRAPH (cto 2026-09-18, CEO-845 row (1); design
.github/ARCH-GC-COMPILE-TIME-FRAME-MAPS.md sections 6.5 and 7 F1; law RULES.md FACT RULE THE COLLECTOR GUESSES
NOTHING, CEO-812).

THE FACT THIS INSTRUMENT EXISTS TO STOP HOLDING BY MEMORY.  Section 6.5 of the frozen design asserts, in prose,
that r13 is the ONE callee-saved register that can hold a collected-heap pointer at an allocating return and only
inside a match; that r14 and r15 are the subject CURSOR and LENGTH and therefore raw; that r12, rbx and rbp carry
a trail top, a saved RSP and a frame base.  Every one of those is a claim about EMITTED CODE, and nothing in the
tree checked any of them.  If a template ever leaves a heap pointer in r12 or r14 across an allocating call, the
collector loses a root and no gate reds.  That is the fourth instrument-shaped hole of 2026-09-18 waiting to
happen, and this census is its cure: the facts are read back out of the .s.

WHAT IT MEASURES, PER MODE-4 .s:
  (1) ALLOCATING CALL SITES.  The allocating set is DERIVED, never a hand list: every function in the runtime .so
      whose call graph reaches rt_gcheap_alloc (util_gc_census.allocating_entries_from_binary, the coo's
      derivation, imported rather than copied).  A call to one of those is a safe point's return.
  (2) LIVE ACROSS.  A real backward liveness over a CFG built from the file's labels and branches, not a forward
      window: a register is live across a call if some path from the call reads it before writing it.  Unknown
      branch targets, indirect jumps and ret are TOP (all six callee-saved registers live), so the analysis
      over-approximates liveness -- the safe direction for a question whose wrong answer is a lost root.
  (2b) OWNED vs PASSED THROUGH.  Live-across alone is nearly vacuous: the SysV contract makes every callee-saved
      register live at every ret, so a register this graph never touches reads as live across every call in it.
      The census therefore splits each live-across reading by whether a definition of that register REACHES the
      call from inside the emitted code.  OWNED means this graph put the value there and must be able to name it.
      PASSED THROUGH means the value is an ancestor's, still sitting in the register because nothing here wrote
      it -- so it is the ANCESTOR's frame that must name it, and no map of THIS graph can.
  (3) WHAT IT HOLDS.  For each OWNED live-across register the census walks back to its reaching definitions and
      classifies each by the FORM of the defining instruction, against a whitelist of forms that provably cannot
      leave a collected-heap pointer in the register:
      PROVABLY NOT A POINTER -- the form itself settles it, no other fact needed:
        RSP      mov <r>, rsp                       a stack address; the heap is not the stack
        IMM      mov <r>, <imm> / movabs / xor r,r  a constant
        D32      mov <r>d, ... / movsxd / movzx     a 32-bit define ZERO-EXTENDS: a 64-bit pointer cannot survive it
        RIP      lea <r>, [rip + <label>]           static data or code beside the program, never the heap
      A COPY OF SOMETHING ELSE -- the form settles NOTHING and the census refuses to call it raw:
        CELL     mov <r>, qword ptr [...]           a copy of a frame or spine cell
        POP      pop <r>                            a copy of whatever the matching push put on the stack
        REG      mov <r>, <other reg>               a copy of another register
        ARITH    add/sub/and/or/inc/dec <r>, ...    a value derived from one of the above
      ⛔ THE REASON THIS SPLIT IS THE POINT OF THE INSTRUMENT: a cell's own DESCR tag is the truth about the CELL,
      and it does not travel to the register.  If the cell holds a collected-heap pointer and the emitted code
      copies it into r12 before an allocating call, the collector can relocate the block, fix up the cell, and
      leave the register pointing at the old address.  "It came from a tagged cell" is therefore NOT an argument
      that the register is raw -- it is the argument that the register needs a tag OF ITS OWN at the site.
      HEAP:
        SUBJECT  mov r13, rax after call rt_match_enter   the one collected-heap pointer section 6.5 names by hand
      Anything else is UNCLASSIFIED: the census names it by site rather than guessing a class, and an
      UNCLASSIFIED reading is the census working, not the census failing.
  (4) SITE-DEPENDENCE.  For each graph, whether a register's live-across status is the SAME at every allocating
      call site of that graph.  This is the measurement that decides where the register facts belong: a fact that
      varies between two sites of one graph CANNOT be carried by a per-graph map without the map lying at one of
      them, and section 6.3's map is one per graph.

USAGE: util_gc_callee_saved_census.py <file.s>...    |    --corpus [--limit N]    |    --selftest
"""
import os, re, sys, subprocess, importlib.util
from collections import defaultdict

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)

CS = ("rbx", "rbp", "r12", "r13", "r14", "r15")
ALIAS = {}
for _r in CS:
    ALIAS[_r] = _r
for _a, _r in (("ebx", "rbx"), ("bx", "rbx"), ("bl", "rbx"), ("bh", "rbx"),
               ("ebp", "rbp"), ("bp", "rbp"), ("bpl", "rbp")):
    ALIAS[_a] = _r
for _r in ("r12", "r13", "r14", "r15"):
    for _s in ("d", "w", "b"):
        ALIAS[_r + _s] = _r
NARROW = set(a for a, r in ALIAS.items() if a != r)

SUBJECT_SEED = "rt_match_enter"
"""----------------------------------------------------------------------------------------------------------"""
def allocating_entries(so):
    spec = importlib.util.spec_from_file_location("_gc_census", os.path.join(HERE, "util_gc_census.py"))
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod.allocating_entries_from_binary(so, out=lambda *a, **k: None)
"""----------------------------------------------------------------------------------------------------------"""
class Insn:
    __slots__ = ("line", "text", "mnem", "ops", "labels")
    def __init__(self, line, text, labels):
        self.line = line; self.text = text; self.labels = labels
        t = text.split(None, 1)
        self.mnem = t[0].lower()
        self.ops = split_ops(t[1]) if len(t) > 1 else []

def split_ops(s):
    out, depth, cur = [], 0, ""
    for ch in s:
        if ch == "[": depth += 1
        elif ch == "]": depth -= 1
        if ch == "," and depth == 0:
            out.append(cur.strip()); cur = ""
        else:
            cur += ch
    if cur.strip(): out.append(cur.strip())
    return out

def split_stmts(line):
    """GNU as takes ';' as a statement separator, and the emitter uses it to put a jcc beside its cmp. It is NOT a
    separator inside a string literal -- and a Prolog program has graphs named ';/2', whose .string would otherwise
    be sawn in half and the tail parsed as an instruction with a mnemonic of '/2\"'. Measured: that is exactly what
    happened before this splitter learned about quotes."""
    out, cur, q = [], "", False
    for ch in line:
        if ch == '"': q = not q
        if ch == ";" and not q:
            out.append(cur); cur = ""
        else:
            cur += ch
    out.append(cur)
    return out

LABEL_RE = re.compile(r'^([.\w$]+):')
def parse(path):
    """statements in order, each carrying the labels that immediately precede it"""
    insns, pending, in_text = [], [], True
    for ln, raw in enumerate(open(path, encoding="utf-8", errors="replace"), 1):
        s = raw.split("#")[0]
        if not s.strip(): continue
        for part in split_stmts(s):
            p = part.strip()
            while p:
                m = LABEL_RE.match(p)
                if not m: break
                pending.append(m.group(1)); p = p[m.end():].strip()
            if not p: continue
            if p.startswith("."):
                d = p.split(None, 1)[0].lower()
                if d in (".text",): in_text = True
                elif d in (".section", ".data", ".rodata", ".bss"):
                    in_text = ".text" in p
                continue
            if not in_text: continue
            insns.append(Insn(ln, p, tuple(pending))); pending = []
    return insns
"""----------------------------------------------------------------------------------------------------------"""
MEM_RE = re.compile(r'\[([^\]]*)\]')
def regs_in_mem(op):
    out = set()
    for m in MEM_RE.findall(op):
        for tok in re.findall(r'\b([a-z][a-z0-9]{1,4})\b', m):
            if tok in ALIAS: out.add(ALIAS[tok])
    return out

def reg_of(op):
    o = op.strip()
    return ALIAS.get(o) if o in ALIAS else None

JCC = re.compile(r'^j(mp|e|ne|z|nz|g|ge|l|le|a|ae|b|be|s|ns|o|no|p|np|c|nc)$')
TWO_OP_RMW = {"add", "sub", "and", "or", "xor", "imul", "sar", "shl", "shr", "adc", "sbb", "lea", "movsxd", "movsx", "movzx", "mov", "cvtsi2sd", "cvtsi2ss", "movq", "movd", "movabs", "cmovne", "cmove", "cmovg", "cmovl"}
READ_ONLY = {"cmp", "test", "cmp64", "push", "ucomisd", "comisd"}
NO_GPR = {"addsd", "subsd", "mulsd", "divsd", "sqrtsd", "xorps", "xorpd", "pxor", "cvttsd2si", "cvtsd2ss", "cvtss2sd", "ret", "leave", "ud2", "nop", "cqo", "cdq", "cld", "std", "sete", "setne", "setg", "setl", "setge", "setle", "seta", "setb", "int3", "hlt", "syscall", "pause", "endbr64"}

def use_def(ins):
    """(uses, defs) restricted to the six callee-saved registers, plus a flag for 'this form is understood'"""
    mn, ops = ins.mnem, ins.ops
    uses, defs = set(), set()
    if mn == "call":
        return uses, defs, True
    if JCC.match(mn):
        for o in ops:
            r = reg_of(o)
            if r: uses.add(r)
            uses |= regs_in_mem(o)
        return uses, defs, True
    if mn == "pop":
        r = reg_of(ops[0]) if ops else None
        if r: defs.add(r)
        else: uses |= regs_in_mem(ops[0]) if ops else set()
        return uses, defs, True
    if mn in NO_GPR:
        return uses, defs, True
    if mn.startswith("rep"):
        return set(CS), set(), True
    known = mn in TWO_OP_RMW or mn in READ_ONLY or mn in ("inc", "dec", "neg", "not", "idiv", "div", "mul")
    for i, o in enumerate(ops):
        r = reg_of(o)
        uses |= regs_in_mem(o)
        if r is None: continue
        if i == 0 and mn in TWO_OP_RMW:
            defs.add(r)
            if mn not in ("mov", "lea", "movsxd", "movsx", "movzx", "movabs", "movq", "movd", "cvtsi2sd", "cvtsi2ss"):
                uses.add(r)
        elif i == 0 and mn in ("inc", "dec", "neg", "not"):
            defs.add(r); uses.add(r)
        else:
            uses.add(r)
    if mn == "xor" and len(ops) == 2 and reg_of(ops[0]) and reg_of(ops[0]) == reg_of(ops[1]):
        uses.discard(reg_of(ops[0]))
    return uses, defs, known
"""----------------------------------------------------------------------------------------------------------"""
def build_cfg(insns):
    label_at = {}
    for i, ins in enumerate(insns):
        for L in ins.labels:
            label_at.setdefault(L, i)
    succ = defaultdict(list); top = set()
    for i, ins in enumerate(insns):
        mn = ins.mnem
        if mn == "ret" or mn == "ud2":
            top.add(i); continue
        if JCC.match(mn):
            tgt = ins.ops[0] if ins.ops else ""
            uncond = (mn == "jmp")
            if tgt in label_at:
                succ[i].append(label_at[tgt])
            else:
                top.add(i)
            if not uncond and i + 1 < len(insns):
                succ[i].append(i + 1)
            continue
        if i + 1 < len(insns):
            succ[i].append(i + 1)
        else:
            top.add(i)
    return succ, top, label_at

def liveness(insns, succ, top):
    """backward fixpoint; live[i] = registers live BEFORE instruction i"""
    n = len(insns)
    ud = [use_def(ins) for ins in insns]
    live = [set() for _ in range(n)]
    changed = True
    while changed:
        changed = False
        for i in range(n - 1, -1, -1):
            out = set(CS) if i in top else set()
            for s in succ[i]:
                out |= live[s]
            u, d, _ = ud[i]
            new = u | (out - d)
            if new != live[i]:
                live[i] = new; changed = True
    return live, ud
"""----------------------------------------------------------------------------------------------------------"""
def classify_def(insns, i, reg, depth=0):
    """the form of the instruction at i that defines reg, as a class name"""
    ins = insns[i]; mn = ins.mnem; ops = ins.ops
    if mn == "pop": return "POP"
    if mn == "mov" and len(ops) == 2:
        dst, src = ops[0], ops[1]
        if dst.strip() in NARROW and ALIAS.get(dst.strip()) == reg: return "D32"
        if src.strip() == "rsp": return "RSP"
        if re.fullmatch(r'-?\d+|0x[0-9a-fA-F]+', src.strip()): return "IMM"
        if src.strip() == "rax" and reg == "r13":
            for j in range(i - 1, max(-1, i - 3), -1):
                if insns[j].mnem == "call" and SUBJECT_SEED in insns[j].text: return "SUBJECT"
        if "[" in src: return cell_class(insns, i, src, reg, depth)
        if reg_of(src): return "REG"
        return "UNCLASSIFIED"
    if mn == "movabs": return "IMM"
    if mn == "lea" and len(ops) == 2:
        return "RIP" if "rip" in ops[1] else "LEA"
    if mn == "xor" and len(ops) == 2 and reg_of(ops[0]) == reg_of(ops[1]): return "IMM"
    if mn in ("add", "sub", "and", "or", "inc", "dec"): return "ARITH"
    if mn in ("movsxd", "movsx", "movzx"): return "D32"
    return "UNCLASSIFIED"

def build_pred(succ, n):
    pred = defaultdict(list)
    for a, ss in succ.items():
        for b in ss: pred[b].append(a)
    return pred

SLOT_RE = re.compile(r'\[\s*(rbp|rsp|r12|r13|r14|r15|rbx)\s*([+-]\s*-?\d+)?\s*\]')
def slot_key(operand):
    """a frame or spine slot named by base register and constant displacement, or None if it is not one"""
    m = SLOT_RE.search(operand)
    if not m: return None
    disp = (m.group(2) or "+0").replace(" ", "")
    if disp.startswith("+-"): disp = disp[1:]
    try: return (m.group(1), int(disp))
    except ValueError: return None

def cell_class(insns, i, src, reg, depth=0):
    """\u26d4 THE ROUTE MATTERS, NOT JUST THE REGISTER. A reload out of a frame slot is not evidence that the value is
    raw -- it is evidence that the value is whatever was STORED into that slot. Measured on the SNOBOL4 witness and
    this is the whole reason the step exists: bb_match_begin seeds the subject base with `mov r13, rax` after
    rt_match_enter (class SUBJECT), stores it to [rbp-16], and the match templates reload `mov r13, qword ptr
    [rbp-16]` BEFORE an allocating call. A classifier that stopped at the reload would have called the live subject
    pointer a CELL and a whitelist that trusted CELL would have blessed a collected-heap pointer sitting in a
    callee-saved register across a collection. So: find every store into that same slot inside the SAME graph and
    classify what each one stored. All agree -> CELL:<that class>. They disagree, or the slot is not a plain
    base+displacement -> plain CELL, which is NOT in the provably-raw set and says so."""
    key = slot_key(src)
    if key is None or depth > 2: return "CELL"
    g = graph_of(insns, i)
    srcs = set()
    pj = CTX["pushes"].get(key)
    if pj is not None and graph_of(insns, pj) == g:
        r2 = reg_of(insns[pj].ops[0])
        if r2 is not None:
            for k in range(pj - 1, max(-1, pj - 400), -1):
                if r2 in use_def(insns[k])[1]:
                    srcs.add(classify_def(insns, k, r2, depth + 1)); break
            else:
                srcs.add("CALLERS")
    for j in CTX["stores"].get(key, ()):
        ins = insns[j]
        if graph_of(insns, j) != g: continue
        r2 = reg_of(ins.ops[1])
        if r2 is None:
            srcs.add("CELL"); continue
        for k in range(j - 1, max(-1, j - 400), -1):
            if r2 in use_def(insns[k])[1]:
                srcs.add(classify_def(insns, k, r2, depth + 1)); break
        else:
            srcs.add("ENTRY")
    if len(srcs) == 1:
        c = srcs.pop()
        return "CELL" if c in ("CELL", "ENTRY") else "CELL:" + c
    return "CELL"

def reaching_defs(insns, pred, ud, i, reg, limit=20000):
    """walk backwards over the reverse CFG from i, collecting the first definition of reg on each path"""
    seen, out, stack, steps = set(), set(), list(pred[i]), 0
    while stack and steps < limit:
        j = stack.pop(); steps += 1
        if j in seen: continue
        seen.add(j)
        d = ud[j][1]
        if reg in d:
            out.add(classify_def(insns, j, reg))
        else:
            stack.extend(pred[j])
    if steps >= limit: out.add("TRUNCATED")
    if not out: out.add("ENTRY")
    return out
"""----------------------------------------------------------------------------------------------------------"""
CTX = {"graph_at": [], "stores": {}, "pushes": {}}
def build_ctx(insns):
    """one forward pass for graph attribution and one for the slot-store index, so the cell tracer is
    linear instead of quadratic -- at 28000 lines of Prolog the quadratic shape was the difference between two
    seconds and not finishing"""
    ga = [None] * len(insns); cur = None
    for i, ins in enumerate(insns):
        for L in ins.labels:
            if L.startswith(".L"): continue
            m = GRAPH_LBL.match(L)
            if m: cur = m.group(1)
        ga[i] = cur
    stores = defaultdict(list); pushes = {}
    for i, ins in enumerate(insns):
        if ins.mnem == "mov" and len(ins.ops) == 2:
            k = slot_key(ins.ops[0])
            if k is not None and "[" in ins.ops[0]: stores[k].append(i)
    """\u26d4 A FRAME SLOT IS ALSO WRITTEN BY A PUSH, AND THAT IS THE FORM THE MATCH REGIME USES. bb_match_begin
    emits push rbp; mov rbp, rsp; push r12; push r13; push r14; push r15 -- so [rbp-8] is the caller's r12,
    [rbp-16] the caller's r13 and so on, which the emitter's own comments spell outer_Sigma / outer_delta /
    outer_Delta. Indexing only mov-stores made every one of those reloads read as a CELL with no source, i.e. the
    tracer stopping one instruction short of the fact. The offsets this loop derives were checked against those
    emitted comments and agree slot for slot."""
    k = 0
    for i, ins in enumerate(insns):
        if ins.mnem == "mov" and len(ins.ops) == 2 and ins.ops[0] == "rbp" and ins.ops[1] == "rsp":
            k = 0; continue
        if ins.mnem == "push" and ins.ops and reg_of(ins.ops[0]):
            k += 8; pushes[("rbp", -k)] = i
        elif ins.mnem not in ("push",) and ins.mnem != "nop":
            if ins.mnem in ("call", "jmp") or JCC.match(ins.mnem): k = -10**9
    CTX["graph_at"] = ga; CTX["stores"] = stores; CTX["pushes"] = pushes

GRAPH_LBL = re.compile(r'^([A-Za-z_][\w$]*?)_(?:\u03b1|\u03c9|\u03b3|\u03b2)$')
def graph_of(insns, i):
    """The graph a site belongs to: the nearest preceding PORT label (name followed by alpha, omega, gamma or beta),
    whose name is the same name the map table carries.
    A site with no such label in front of it is UNATTRIBUTED and is EXCLUDED from the site-dependence count rather
    than folded into a catch-all bucket -- lumping two different graphs under one name would INVENT the very
    site-dependence this census is measuring, which is the wrong direction for a number that is about to decide
    where the register facts live."""
    ga = CTX["graph_at"]
    return ga[i] if i < len(ga) else None

PROVABLY_RAW = {"RSP", "IMM", "D32", "RIP"}
COPY = {"CELL", "POP", "REG", "ARITH", "LEA", "CALLERS"}
CELL_PREFIX = "CELL:"
HEAP = {"SUBJECT"}

def census(paths, alloc, out=print):
    tot_sites = 0
    live_across = defaultdict(int); owned = defaultdict(int); passed = defaultdict(int)
    classes = defaultdict(lambda: defaultdict(int))
    unclassified = []; copies = []; heapsites = []; unattributed = [0]
    per_graph = defaultdict(lambda: defaultdict(set))
    unknown_mnem = defaultdict(int)
    for path in paths:
        insns = parse(path)
        succ, top, _ = build_cfg(insns)
        build_ctx(insns)
        live, ud = liveness(insns, succ, top)
        pred = build_pred(succ, len(insns))
        for i, ins in enumerate(insns):
            if not ins.mnem.startswith("call"): continue
            m = re.match(r'^[A-Za-z_][\w.$]*', ins.ops[0]) if ins.ops else None
            if not m: continue
            fn = m.group(0).split("@")[0]
            if alloc is not None and fn not in alloc: continue
            tot_sites += 1
            g = graph_of(insns, i)
            if g is None: unattributed[0] += 1
            after = set()
            for s in succ[i]: after |= live[s]
            if i in top: after |= set(CS)
            for r in CS:
                hot = r in after
                if g is not None: per_graph[(os.path.basename(path), g)][r].add(hot)
                if not hot: continue
                live_across[r] += 1
                cls = reaching_defs(insns, pred, ud, i, r)
                if cls == {"ENTRY"}:
                    passed[r] += 1
                    continue
                owned[r] += 1
                for c in cls:
                    classes[r][c] += 1
                    if c.startswith(CELL_PREFIX):
                        tail = c[len(CELL_PREFIX):]
                        if tail in HEAP:
                            heapsites.append((os.path.basename(path), g, ins.line, r, c, insns[i].text[:60]))
                        elif tail not in PROVABLY_RAW:
                            copies.append((os.path.basename(path), g, ins.line, r, c, insns[i].text[:60]))
                    elif c in HEAP:
                        heapsites.append((os.path.basename(path), g, ins.line, r, c, insns[i].text[:60]))
                    elif c in COPY:
                        copies.append((os.path.basename(path), g, ins.line, r, c, insns[i].text[:60]))
                    elif c not in PROVABLY_RAW and c not in HEAP and c != "ENTRY":
                        unclassified.append((os.path.basename(path), g, ins.line, r, c, insns[i].text[:60]))
        for ins in insns:
            _, _, known = use_def(ins)
            if not known: unknown_mnem[ins.mnem] += 1
    return tot_sites, live_across, owned, passed, classes, unclassified, copies, heapsites, per_graph, unknown_mnem, unattributed[0]
"""----------------------------------------------------------------------------------------------------------"""
def report(paths, alloc, out=print):
    tot, la, owned, passed, classes, unc, cop, heaps, per_graph, unknown, unattr = census(paths, alloc, out)
    out(f"CALLEE-SAVED CENSUS files={len(paths)} allocating_call_sites={tot}")
    if unknown:
        out("  ⛔ UNDERSTOOD=NO for these mnemonics (the census REFUSES rather than assume they leave the six registers alone): "
            + " ".join(f"{k}={v}" for k, v in sorted(unknown.items())))
    for r in CS:
        cs = classes.get(r, {})
        detail = " ".join(f"{k}={v}" for k, v in sorted(cs.items())) or "-"
        out(f"  {r}: live_across={la.get(r,0)}/{tot} (owned={owned.get(r,0)} passed_through={passed.get(r,0)})  owned_def_classes: {detail}")
    def bucket(k):
        t = k[len(CELL_PREFIX):] if k.startswith(CELL_PREFIX) else k
        if t in HEAP: return "heap"
        if t in PROVABLY_RAW or t == "ENTRY": return "raw"
        if t in COPY: return "copy"
        return "unclassified"
    tally = defaultdict(int)
    for r in CS:
        for k, v in classes.get(r, {}).items(): tally[bucket(k)] += v
    raw_n, copy_n, heap_n = tally["raw"], tally["copy"], tally["heap"]
    out(f"  OWNED READINGS BY WHAT THE ROUTE SETTLES: provably_not_a_pointer={raw_n}  a_copy_of_something_else={copy_n}  "
        f"A_COLLECTED_HEAP_POINTER={heap_n}  unclassified={len(unc)}")
    if heap_n:
        out(f"  \u26d4 {heap_n} OWNED reading(s) ARE A COLLECTED-HEAP POINTER SITTING IN A CALLEE-SAVED REGISTER ACROSS AN "
            f"ALLOCATING RETURN. Section 6.5 names r13 (the subject base) as the one register this can happen to, and the "
            f"census confirms the register while correcting the ROUTE: the pointer does not arrive by the seeding "
            f"mov r13, rax after rt_match_enter -- that one is dead across every allocating call -- it arrives by a RELOAD "
            f"out of the frame cell the match stashed it in. A classifier that stopped at the reload would have called it "
            f"a CELL and a whitelist that trusted CELL would have blessed it. Nothing collects through a map yet, so this "
            f"cannot bite today; it is the exact residual the spill record of section 6.5 exists to carry.")
        by = defaultdict(int)
        for c in heaps: by[(c[3], c[4])] += 1
        out("      " + "  ".join(f"{r}/{k}={v}" for (r, k), v in sorted(by.items())))
        for h in heaps[:4]:
            out(f"      {h[0]} graph={h[1]} line={h[2]} {h[3]}  [{h[5]}]")
    if copy_n:
        out(f"  ⛔ {copy_n} OWNED reading(s) are A COPY OF SOMETHING ELSE, and that is the residual this census exists "
            f"to name: the source cell's DESCR tag is the truth about the CELL and it does not travel to the register, so "
            f"if the source holds a collected-heap pointer the collector can relocate the block, fix up the cell, and leave "
            f"the register pointing at the old address. A per-graph map cannot tag these; only the site can.")
        by = defaultdict(int)
        for c in cop: by[(c[3], c[4])] += 1
        out("      " + "  ".join(f"{r}/{k}={v}" for (r, k), v in sorted(by.items())))
    for u in unc[:12]:
        out(f"      ⛔ UNCLASSIFIED {u[0]} graph={u[1]} line={u[2]} {u[3]} class={u[4]}  [{u[5]}]")
    varies = [(k, r) for k, d in per_graph.items() for r, v in d.items() if len(v) > 1]
    out(f"  SITE-DEPENDENCE: {len(varies)} (graph, register) pair(s) whose live-across status DIFFERS between two "
        f"allocating call sites of the SAME graph, out of {sum(len(d) for d in per_graph.values())} pairs over "
        f"{len(per_graph)} graphs. A pair that differs cannot be carried by a per-graph map (section 6.3 is one map "
        f"per graph) without the map lying at one of its own sites. {unattr} site(s) carried no port label in front of "
        f"them and are EXCLUDED from this count rather than lumped together, because lumping two graphs under one name "
        f"would invent site-dependence.")
    for k, r in varies[:12]:
        out(f"      {k[0]} graph={k[1]} register={r}: live across one allocating return and dead across another")
    out(f"SUMMARY sites={tot} raw={raw_n} copies={copy_n} heap={heap_n} unclassified={len(unc)} "
        f"unknown_mnemonics={sum(unknown.values())} site_dependent_pairs={len(varies)} graphs={len(per_graph)} "
        f"unattributed_sites={unattr} files={len(paths)}")
    return tot, la, owned, passed, classes, unc, cop, varies
"""----------------------------------------------------------------------------------------------------------"""
def main(argv):
    if "--selftest" in argv: return selftest()
    so = os.path.join(ROOT, "out", "libscrip_rt.so")
    alloc = allocating_entries(so) if os.path.exists(so) else None
    if alloc is None:
        print("REFUSE(2): the allocating set could not be derived from the runtime .so -- the census will not fall back to a hand list")
        return 2
    paths = [a for a in argv if a.endswith(".s")]
    if "--corpus" in argv:
        import tempfile, glob
        limit = 0
        if "--limit" in argv: limit = int(argv[argv.index("--limit") + 1])
        d = tempfile.mkdtemp()
        srcs = []
        for ext in ("sno", "sc", "icn", "pl", "reb", "raku", "pas"):
            srcs += sorted(glob.glob(os.path.join(ROOT, "..", "corpus", "benchmarks", "*", "*." + ext)))
        if limit: srcs = srcs[:limit]
        for s in srcs:
            o = os.path.join(d, os.path.basename(s) + ".s")
            try:
                r = subprocess.run([os.path.join(ROOT, "scrip"), "--compile", "-o", o, s],
                                   capture_output=True, timeout=120)
                if r.returncode == 0 and os.path.exists(o) and os.path.getsize(o): paths.append(o)
            except Exception: pass
    if not paths:
        print("REFUSE(2): no .s to read (pass files, or --corpus)")
        return 2
    tot, la, owned, passed, classes, unc, cop, varies = report(paths, alloc)
    if tot == 0:
        print("REFUSE(2): zero allocating call sites found -- the census measured nothing")
        return 2
    return 1 if unc else 0
"""----------------------------------------------------------------------------------------------------------"""
def selftest():
    import tempfile
    ok = [0, 0]
    def ck(c, what):
        ok[1] += 1
        if c: ok[0] += 1
        else: print(f"  SELFTEST RED: {what}")
    d = tempfile.mkdtemp()
    p = os.path.join(d, "t.s")
    open(p, "w").write(
        " .text\n"
        "f_α:\n"
        " push rbx\n"
        " mov rbx, rsp\n"
        " call rt_alloc_thing\n"
        " mov rsp, rbx\n"
        " pop rbx\n"
        " ret\n"
        "g_α:\n"
        " push r14\n"
        " mov r14, qword ptr [rsp + 8]\n"
        " call rt_alloc_thing\n"
        " pop r14\n"
        " ret\n")
    alloc = {"rt_alloc_thing"}
    tot, la, owned, passed, classes, unc, cop, heaps, pg, unk, unattr = census([p], alloc)
    ck(tot == 2, f"two allocating call sites seen, got {tot}")
    ck(owned.get("rbx", 0) == 1, f"rbx is OWNED-live across the call that reads it afterwards, got {owned.get('rbx',0)}")
    ck(passed.get("r13", 0) == 2, f"a register this witness never writes reads as PASSED THROUGH at both calls, got {passed.get('r13',0)}")
    ck(classes.get("rbx", {}).get("RSP", 0) == 1, "rbx's reaching definition classifies as RSP")
    ck(owned.get("r14", 0) == 0, f"r14 is not owned-live across its call (the pop redefines it), got {owned.get('r14',0)}")
    ck(not unk, f"every mnemonic in the witness is understood, got {dict(unk)}")
    open(p, "w").write(
        " .text\n"
        "h_α:\n"
        " push r12\n"
        " call rt_weird_producer\n"
        " mov r12, rax\n"
        " call rt_alloc_thing\n"
        " mov rdi, r12\n"
        " pop r12\n"
        " ret\n")
    tot, la, owned, passed, classes, unc, cop, heaps, pg, unk, unattr = census([p], alloc)
    ck(owned.get("r12", 0) == 1, f"a register owned-live across the call is seen, got {owned.get('r12',0)}")
    ck(any(u[3] == "r12" and u[4] == "UNCLASSIFIED" for u in unc),
       f"a register defined by an un-whitelisted form reads UNCLASSIFIED rather than being assumed raw, got {unc}")
    open(p, "w").write(
        " .text\n"
        "k_α:\n"
        " call rt_match_enter\n"
        " mov r13, rax\n"
        " call rt_alloc_thing\n"
        " mov rdi, r13\n"
        " ret\n")
    tot, la, owned, passed, classes, unc, cop, heaps, pg, unk, unattr = census([p], alloc)
    ck(classes.get("r13", {}).get("SUBJECT", 0) == 1,
       f"the one heap class is recognised by its seeding call, got {dict(classes.get('r13',{}))}")
    open(p, "w").write(
        " .text\n"
        "v_α:\n"
        " push rbx\n"
        " mov rbx, rsp\n"
        " call rt_alloc_thing\n"
        " mov rsp, rbx\n"
        " call rt_alloc_thing\n"
        " pop rbx\n"
        " ret\n")
    tot, la, owned, passed, classes, unc, cop, heaps, pg, unk, unattr = census([p], alloc)
    varies = [(k, r) for k, dd in pg.items() for r, v in dd.items() if len(v) > 1]
    ck(any(r == "rbx" for _, r in varies),
       f"a register live across one allocating call of a graph and dead across another is reported as site-dependent, got {varies}")
    print(f"SELFTEST {ok[0]}/{ok[1]} arms green")
    return 0 if ok[0] == ok[1] else 1

if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
