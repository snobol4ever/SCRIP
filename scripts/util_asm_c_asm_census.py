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
#   (a) C functions CALLED FROM EMITTED ASM   -- the x86("call", "NAME") callee set in src/templates/*.cpp
#   (b) C functions that TRANSITIVELY REACH an asm activation entry  -- rt_proc_enter & siblings
# Anything in both is an ASM -> C -> ASM road.
#
# ⛔ LIMITATIONS, STATED SO NOBODY READS THIS AS COMPLETE.  It is a STATIC, NAME-BASED call graph over src/runtime/**.c:
#   1. INDIRECT CALLS ARE INVISIBLE.  A call through a function pointer (p->fn, a dtp slot, a jump table) is not an edge
#      here.  That is a REAL hole and it is the direction the machine actually uses most, so a count of 0 from this tool
#      would NOT prove the property -- it would prove only that no NAMED road remains.
#   2. It does not model the killswitches; a road refused at runtime still counts as a road.
#   3. It parses C by regex.  It is validated on every run against beauty's MEASURED M1 path (--selftest) and REFUSES to
#      report if that ground truth stops reproducing -- an analysis that cannot find the bug we already found by hand
#      has no business grading the tree.
import re, sys, glob, os, collections
ASM_ENTRIES = {"rt_proc_enter","rt_chain_enter","rt_chain_enter_v","rt_tiny_record_enter",
               "rt_genp_spine_enter","rt_outer_call","rt_outer_call_delta0","rk_gram_enter_box"}
BOGUS = {"DESCR_t","fn","code","if","for","while","switch","return","sizeof","do","else","static","extern","inline",
         "void","int","long","char","unsigned","struct","union","typedef","const","goto","case","default","break","continue"}
GROUND_TRUTH = [("rt_call_arr","rt_call_arr_impl"),("rt_call_arr_impl","try_call_builtin_by_name"),
                ("try_call_builtin_by_name","rt_call_named_proc"),("rt_call_named_proc","rt_proc_enter")]
DEF  = re.compile(r'^(?:static\s+|inline\s+|extern\s+)*[A-Za-z_][A-Za-z_0-9]*[ \*]+\**([A-Za-z_][A-Za-z_0-9]*)\s*\(')
CALL = re.compile(r'\b([A-Za-z_][A-Za-z_0-9]*)\s*\(')
# NO LEADING WHITESPACE: a forward declaration in this tree sits at COLUMN 0; a body statement is indented.  The first
# cut allowed leading space and therefore ate `    return rt_proc_enter(...);` -- caught by the selftest, which is what
# the selftest is for.
PROTO = re.compile(r'^(?:extern\s+)?[A-Za-z_][A-Za-z_0-9 \*]*\**[A-Za-z_][A-Za-z_0-9]*\s*\([^;{]*\)\s*;\s*$')
def build(root):
    """⛔ BODIES ARE BRACE-BOUNDED, NOT 'UNTIL THE NEXT DEFINITION'.  The naive form absorbed everything after a
    function -- including the forward declaration `DESCR_t rt_proc_enter(void *fn);` and the __asm__ string blocks --
    and manufactured an edge rt_proc_call_epilogue_ret -> rt_proc_enter that DOES NOT EXIST (its real body calls only
    rt_proc_call_epilogue_omega/gamma).  That single fake edge sat on 18 of 26 reported roads.  This codebase writes
    `}` at column 0 with zero blank lines, so the closing brace is a reliable terminator."""
    bodies = collections.defaultdict(list)
    for path in sorted(glob.glob(os.path.join(root,'src/runtime/**/*.c'), recursive=True)):
        cur = None
        for line in open(path, encoding='utf-8', errors='ignore'):
            if cur is not None:
                if line.startswith('}'): cur = None; continue
                if not PROTO.match(line) and not line.lstrip().startswith('"'): bodies[cur].append(line)
                continue
            m = DEF.match(line)
            if m and ';' not in line and m.group(1) not in BOGUS and line.rstrip().endswith(('{',')')):
                cur = m.group(1); bodies[cur]  # touch so a body-less definition still registers
    g = {f: {c for ln in b for c in CALL.findall(ln)} - BOGUS for f,b in bodies.items()}
    known = set(g) | ASM_ENTRIES
    return {f: (cs & known) for f,cs in g.items()}
def asm_callees(root):
    pat = re.compile(r'x86\("call[^"]*",\s*"([A-Za-z_][A-Za-z_0-9]*)"')
    out = set()
    for p in glob.glob(os.path.join(root,'src/templates/*.cpp')):
        out |= set(pat.findall(open(p, encoding='utf-8', errors='ignore').read()))
    return out
def main():
    root = os.environ.get('S4E_SCRIP') or os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    g = build(root)
    missing = [a+" -> "+b for a,b in GROUND_TRUTH if b not in g.get(a,())]
    if missing:
        print("⛔ SELFTEST FAILED — the census cannot reproduce beauty's MEASURED M1 path; refusing to report.")
        for m in missing: print("   MISSING EDGE:", m)
        return 3
    reach, ch = {f for f in g if g[f] & ASM_ENTRIES}, True
    while ch:
        ch = False
        for f,cs in g.items():
            if f not in reach and (cs & reach): reach.add(f); ch = True
    bad = sorted(asm_callees(root) & set(g) & reach)
    def path(f, seen=None):
        seen = seen or set()
        if f in seen: return None
        seen.add(f)
        for c in sorted(g.get(f,())):
            if c in ASM_ENTRIES: return [f,c]
            if c in reach:
                p = path(c, set(seen))
                if p: return [f]+p
        return None
    print("=== NO-ASM->C->ASM CENSUS (selftest OK: beauty's M1 path reproduces) ===")
    print("    C functions parsed %d · reaching an asm entry %d · called from emitted asm %d" %
          (len(g), len(reach), len(asm_callees(root))))
    print("    ASM -> C -> ASM roads: %d" % len(bad))
    for f in bad:
        p = path(f)
        print("  ⛔ %-30s %s" % (f, " -> ".join(p) if p else "?"))
    print("COUNT=%d" % len(bad))
    return 0
if __name__ == "__main__": sys.exit(main())
