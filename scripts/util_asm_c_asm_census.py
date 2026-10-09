#!/usr/bin/env python3
# util_asm_c_asm_census.py — ⛔ THE NO-ASM->C->ASM CENSUS (Lon 2026-08-20 s194, in-chat: "You must guarantee that no ASM -> C -> ASM.").
#
# WHY THE RULE EXISTS, MEASURED (FINDING-2026-08-20-s194c): the Byrd wire contract puts gamma in r10 and omega in r11
# (Lon s55) while RTCC banks r10/r11 as VM globals in rtccb[32] (x86_asm.h:12 "R10=7, R11=8"; bank/reload at :427/:428/
# :440/:441).  rtccb is a FLAT GLOBAL -- one slot per register, program-wide, WITH NO NESTING.  So when emitted asm calls
# a C function and that C function re-enters asm, the INNER activation banks its wires into the SAME two slots and the
# OUTER activation's continuations are lost.  beauty's M1 SIGSEGV is exactly this: at the fault r11 = rip = &rtccb.
# The shape is the defect; forbidding the shape retires the whole class instead of defending against instances.
#
# WHAT IT COMPUTES: the intersection of
#   (a) C functions CALLED FROM EMITTED ASM   -- the x86("call*", ...) callees and the baked addresses of src/templates/**
#       and src/emitter/** (asm_callees)
#   (b) C functions that TRANSITIVELY REACH an asm activation entry  -- rt_proc_enter & siblings, computed (asm_entries)
# Anything in both is an ASM -> C -> ASM road.
#
# ⛔ LIMITATIONS, STATED SO NOBODY READS THIS AS COMPLETE.  It is a STATIC, NAME-BASED call graph over src/runtime/**.c:
#   1. INDIRECT CALLS ARE INVISIBLE.  A call through a function pointer (p->fn, a dtp slot, a jump table) is not an edge
#      here.  That is a REAL hole and it is the direction the machine actually uses most, so a count of 0 from this tool
#      would NOT prove the property -- it would prove only that no NAMED road remains.
#   2. It does not model the killswitches; a road refused at runtime still counts as a road.
#   3. It parses C by regex.  It is validated on every run against a MEASURED road (GROUND_TRUTH_ROAD) and REFUSES to
#      report if that ground truth stops reproducing; the gate also plants two roads (--plant DIR) and requires both -- an analysis that cannot find the bug we already found by hand
#      has no business grading the tree.
import re, sys, glob, os, collections
BOGUS = {"DESCR_t","fn","code","if","for","while","switch","return","sizeof","do","else","static","extern","inline",
         "void","int","long","char","unsigned","struct","union","typedef","const","goto","case","default","break","continue"}
# ⛔ THE GROUND TRUTH IS A MEASURED ROAD, RE-MEASURED WHEN THE CODE MOVES (CEO-1573).  beauty's s194 M1 path
# (rt_call_arr -> rt_call_arr_impl -> try_call_builtin_by_name -> rt_call_named_proc) no longer exists as written: beauty
# reaches no asm entry from C today (gdb, every entry broken on, SCRIP 7016f6e25).  The road below is the APPLY OF AN
# INDIRECT NAME, measured under gdb on the same tree: Raku `say &::("f")(2)` with `sub f`, mode 3, a breakpoint on every
# asm entry, the frames read from the backtrace -- emitted code (slab pc) -> rt_call_arr_bl_try -> ... -> rt_proc_enter.
# The name the Raku lowerer hands the road there is the source text `(("f"))` (a Raku defect, telegrammed to hq_raku);
# the last edge was measured with the name set to "f" at rt_call_proc_descr, as a correct lowering would hand it.
GROUND_TRUTH_TREE = "7016f6e25"
GROUND_TRUTH_ROAD = ["rt_call_arr_bl_try", "rt_call_arr_bl_s", "rt_call_arr_impl", "try_call_builtin_by_name_bl_s_rq",
                     "script_try_call_builtin_by_name_rq", "rk_call_block_rq", "rk_call_snap_rq", "rk_proc_descr_rq",
                     "rt_call_proc_descr", "rt_call_proc_descr_p", "rt_proc_enter"]
GROUND_TRUTH = list(zip(GROUND_TRUTH_ROAD, GROUND_TRUTH_ROAD[1:]))
DEF  = re.compile(r'^(?:static\s+|inline\s+|extern\s+)*[A-Za-z_][A-Za-z_0-9]*[ \*]+\**([A-Za-z_][A-Za-z_0-9]*)\s*\(')
CALL = re.compile(r'\b([A-Za-z_][A-Za-z_0-9]*)\s*\(')
# NO LEADING WHITESPACE: a forward declaration in this tree sits at COLUMN 0; a body statement is indented.  The first
# cut allowed leading space and therefore ate `    return rt_proc_enter(...);` -- caught by the selftest, which is what
# the selftest is for.
PROTO = re.compile(r'^(?:extern\s+)?[A-Za-z_][A-Za-z_0-9 \*]*\**[A-Za-z_][A-Za-z_0-9]*\s*\([^;{]*\)\s*;\s*$')
STRLIT = re.compile(r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'')
INDIRECT = re.compile(r'\b(?:jmp|call)q?\s+\*')
def asm_entries(root):
    """⛔ THE ASM ACTIVATION ENTRIES ARE COMPUTED, NOT TYPED (CEO-1573).  The typed set had fallen behind the code:
    rt_proc_enter_named, rt_proc_enter_frag, rt_genp_spine_enter_n2 and rt_tiny_glue_enter all enter emitted code and
    none was in it.  An entry is a global symbol DEFINED IN ASM -- a .globl inside an __asm__ block of a C file under
    src/runtime or src/driver, or a .globl / RTX_FUNC / RTX_ENTRY region of a .s/.S file under src/runtime -- whose own
    text jumps or calls THROUGH A REGISTER OR MEMORY (jmp *, call *): that indirect transfer is the step into emitted
    code.  An asm leaf that only calls named C is not an entry."""
    out = set()
    cs = glob.glob(os.path.join(root,'src/runtime/**/*.c'), recursive=True) + glob.glob(os.path.join(root,'src/driver/**/*.c'), recursive=True)
    for path in cs:
        txt = open(path, encoding='utf-8', errors='ignore').read()
        for m in re.finditer(r'__asm__\s*\((.*?)\)\s*;', txt, re.S):
            body = m.group(1)
            if INDIRECT.search(body): out |= set(re.findall(r'\.globl\s+([A-Za-z_][A-Za-z_0-9]*)', body))
    ss = [f for e in ('s','S') for f in glob.glob(os.path.join(root,'src/runtime/**/*.'+e), recursive=True)]
    opener = re.compile(r'^\s*(?:\.globl\s+([A-Za-z_][A-Za-z_0-9]*)|RTX_(?:FUNC|ENTRY)\s*\(\s*([A-Za-z_][A-Za-z_0-9]*))')
    for path in ss:
        cur = None
        for line in open(path, encoding='utf-8', errors='ignore'):
            m = opener.match(line)
            if m: cur = m.group(1) or m.group(2); continue
            if cur and INDIRECT.search(line): out.add(cur)
    return out
def build(root, entries, plant=None):
    """⛔ BODIES ARE BRACE-BOUNDED, NOT 'UNTIL THE NEXT DEFINITION'.  The naive form absorbed everything after a
    function -- including the forward declaration `DESCR_t rt_proc_enter(void *fn);` and the __asm__ string blocks --
    and manufactured an edge rt_proc_call_epilogue_ret -> rt_proc_enter that DOES NOT EXIST (its real body calls only
    rt_proc_call_epilogue_omega/gamma).  That single fake edge sat on 18 of 26 reported roads.  This codebase writes
    `}` at column 0 with zero blank lines, so the closing brace is a reliable terminator."""
    bodies = collections.defaultdict(list)
    for path in sorted(glob.glob(os.path.join(root,'src/runtime/**/*.c'), recursive=True)) + (sorted(glob.glob(os.path.join(plant,'*.c'))) if plant else []):
        cur = None
        for line in open(path, encoding='utf-8', errors='ignore'):
            if cur is not None:
                if line.startswith('}'): cur = None; continue
                if not PROTO.match(line) and not line.lstrip().startswith('"'): bodies[cur].append(STRLIT.sub('""', line))
                continue
            m = DEF.match(line)
            if not m or m.group(1) in BOGUS: continue
            head = line.rstrip()
            if '{' in head and head.endswith('}'):
                bodies[m.group(1)].append(STRLIT.sub('""', head[head.index('{') + 1:]))
            elif ';' not in head and head.endswith(('{',')')):
                cur = m.group(1); bodies[cur]  # touch so a body-less definition still registers
    g = {f: {c for ln in b for c in CALL.findall(ln)} - BOGUS for f,b in bodies.items()}
    known = set(g) | entries
    return {f: (cs & known) for f,cs in g.items()}
def asm_callees(root, plant=None):
    """The callee of every x86("call*", ...) the emitter and the templates write: EVERY identifier literal in the
    call's second argument, so `cond ? "a" : "b"` names both.  The templates live in subdirectories (src/templates/bb,
    x86, xa) and the emitter writes calls too; the old flat glob over src/templates/*.cpp matched no file at all.
    A C function whose ADDRESS the emitter bakes into emitted code is reached from asm as well, whatever helper writes
    the instruction (bb_glue_try_enter("rt_call_arr_bl_try", (uint64_t)(uintptr_t)(void *)rt_call_arr_bl_try, ...)):
    every `(void *)NAME` and `TEMPLATE_FN_ADDR(NAME)` there is a callee too."""
    out = set()
    srcs = [f for d in ('src/templates','src/emitter') for e in ('cpp','h') for f in glob.glob(os.path.join(root,d,'**','*.'+e), recursive=True)]
    srcs += sorted(glob.glob(os.path.join(plant,'*.cpp'))) if plant else []
    for p in srcs:
        txt = open(p, encoding='utf-8', errors='ignore').read()
        for m in re.finditer(r'x86\(\s*"call[^"]*"\s*,', txt):
            i, depth, arg = m.end(), 0, []
            while i < len(txt):
                c = txt[i]
                if c == '"':
                    j = STRLIT.match(txt, i)
                    if not j: break
                    arg.append(j.group(0)); i = j.end(); continue
                if c in '([{': depth += 1
                elif c in ')]}':
                    if depth == 0: break
                    depth -= 1
                elif c in ',;' and depth == 0: break
                i += 1
            out |= {a[1:-1] for a in arg if re.fullmatch(r'"[A-Za-z_][A-Za-z_0-9]*"', a)}
        out |= set(re.findall(r'\(\s*void\s*\*\s*\)\s*([A-Za-z_][A-Za-z_0-9]*)\b(?!\s*[(\[.]|\s*->)', txt))
        out |= set(re.findall(r'TEMPLATE_FN_ADDR\(\s*([A-Za-z_][A-Za-z_0-9]*)\s*\)', txt))
    return out
def main():
    root = os.environ.get('S4E_SCRIP') or os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    plant = sys.argv[2] if len(sys.argv) == 3 and sys.argv[1] == '--plant' else None
    if len(sys.argv) > 1 and not (plant and os.path.isdir(plant)):
        print("usage: util_asm_c_asm_census.py [--plant DIR]  (DIR's *.c read as runtime C, its *.cpp as emitter source)")
        return 2
    entries = asm_entries(root)
    g = build(root, entries, plant)
    callees = asm_callees(root, plant)
    missing = [a+" -> "+b for a,b in GROUND_TRUTH if b not in g.get(a,())]
    if GROUND_TRUTH_ROAD[0] not in callees: missing.append("emitted asm -> " + GROUND_TRUTH_ROAD[0])
    if GROUND_TRUTH_ROAD[-1] not in entries: missing.append(GROUND_TRUTH_ROAD[-1] + " is not an asm entry")
    if missing:
        print("⛔ SELFTEST FAILED — the census cannot reproduce the MEASURED road (%s); refusing to report." % GROUND_TRUTH_TREE)
        for m in missing: print("   MISSING EDGE:", m)
        return 3
    reach, ch = {f for f in g if g[f] & entries}, True
    while ch:
        ch = False
        for f,cs in g.items():
            if f not in reach and (cs & reach): reach.add(f); ch = True
    bad = sorted(callees & set(g) & reach)
    def path(f):
        prev, q = {f: None}, collections.deque([f])
        while q:
            u = q.popleft()
            for c in sorted(g.get(u, ())):
                if c in prev: continue
                prev[c] = u
                if c in entries:
                    p = [c]
                    while prev[p[-1]] is not None: p.append(prev[p[-1]])
                    return p[::-1]
                if c in reach: q.append(c)
        return None
    print("=== NO-ASM->C->ASM CENSUS (selftest OK: the measured road of %s reproduces) ===" % GROUND_TRUTH_TREE)
    print("    C functions parsed %d · asm entries %d · reaching an asm entry %d · called from emitted asm %d" %
          (len(g), len(entries), len(reach), len(callees)))
    print("    ASM -> C -> ASM roads: %d" % len(bad))
    for f in bad:
        p = path(f)
        print("  ⛔ %-30s %s" % (f, " -> ".join(p) if p else "?"))
    print("COUNT=%d" % len(bad))
    return 0
if __name__ == "__main__": sys.exit(main())
