#!/usr/bin/env python3
"""util_gc_poll_register_liveness.py -- AT EVERY EMITTED SAFE POINT, IS A REGISTER READ AS A POINTER BEFORE IT IS
NEXT WRITTEN?  (ceo, row gc-a-fast-witness-detector-grades-a-safe-point-conversion-batch-in-seconds-proven-against-
the-known-bad-insertion; design .github/ARCH-GC-COMPILE-TIME-FRAME-MAPS.md; law RULES.md FACT RULE THE COLLECTOR
GUESSES NOTHING.)

⛔ WHY THIS EXISTS, NAMED BY THE GATE IT CONTINUES.  test_gate_gc_the_caller_saved_spill_block_never_holds_a_heap_
reference.sh arm (i) measured the standing population and then wrote down its own next step verbatim: "The step
after this one is to ask, per member, whether the reloaded register is read as a pointer before it is next
written."  That gate proved 806 heap references sit in rtccb across collections and that 161 of them name a block
THE COLLECTOR MOVED AND ARE LEFT UNREPAIRED, because nothing visits that block and nothing can (32 untagged words;
visiting them is a conservative scan and CEO-812 froze that out).  A wrong word is only SPENT when the reload's
register is then used as a pointer -- so the unrepaired count is an upper bound and this reader is what turns it
into a defect list or closes the class.

⛔ AND IT IS THE DISCRIMINATOR THE SITE CONVERSIONS NEED, WHICH IS THE MORE EXPENSIVE HALF.  CEO-1107: choosing
polls by the callee's DECLARED RETURN CLASS put 77 in and cost 57 Icon programs, because the return class says what
the callee RETURNS and nothing about what is ALREADY LIVE in a register across the site.  CEO-1110: a 54-witness
6.8-second sweep cleared 14 sites that still cost 7 programs, so THE SWEEP IS NOT SUFFICIENT EITHER.  Both
instruments ask about the program's OUTPUT.  This one asks about the emitted instruction stream, which is where the
defect is, and it is static: no arena, no stress, no load, no master -- so it never competes for the board channel
two seats share (MODE line 2, DUO).

THE READING, and the conservative direction is the whole design.  Everything on the emitted stack is a DESCR and a
register is not on the stack, so a register live across a poll is relocated by NOTHING unless the poll spills it as
a tagged cell (x86_rt_gc_poll_res for rax:rdx, the _rec_sigma family for r13/r15).  The wrong answer here is A LOST
ROOT, so a read is POINTER unless it is PROVABLY too narrow to hold one -- the callee-saved census's own D32 rule
(a 32-bit read zero-extends: a 64-bit pointer cannot survive it), reused rather than reinvented.  Four verdicts per
(safe point, register), and PROPAGATED is its own bucket rather than folded into either answer, because a count
that hides its undecidables is the defect this file exists against:

  POINTER     the register is read as a memory base or index, or handed to a call in a pointer argument slot,
              before any path writes it.  A collection at this safe point spends a stale pointer here.
  SPILLED     the register is stored to memory before any other classified use.  Reported separately because a
              store into a MAPPED FRAME SLOT is the designed cure (ARCH-GC 3), not a defect -- but this reader
              cannot tell a mapped slot from a scratch cell, so it says where the value went and stops.
  NARROW      every read before the next write is provably narrow -- a 32-bit subregister, or a compare or an
              add/sub against an immediate.  Harmless BY MECHANISM rather than by luck.
  PROPAGATED  the register is copied onward past this reader's hop budget, so it cannot place it.  Named, never
              counted clean.
  DEAD        every path writes the register before reading it.  Nothing to protect.

⛔⛔ THE ARTIFACT THIS READER WAS BORN WITH, WRITTEN DOWN BECAUSE IT READ AS A CLEAN BILL OF HEALTH.  The rtcc
reload (x86_asm.h 395-398) is the FIRST instruction after the poll call and it is itself a WRITE of r8.  A walk that
starts at the call's next instruction therefore stops at once and calls r8 DEAD -- and the first reading over 54
witnesses said exactly that, r8 DEAD at all 2889 safe points, which would have retired a standing 161-unrepaired
finding on an artifact.  A DETECTOR ARM FAILS OPEN: it reads as "there was never a bug here" (CLAUDE.md, ceo
CEO-480/481).  So for a register the reload DEFINES, the walk begins AFTER the reload block, because the gate's
question is about the RELOADED value.  Arm (g) of --selftest is that fail-once and it is the arm that matters.

USAGE
  util_gc_poll_register_liveness.py [PROG ...]        one or more corpus programs (default: the witness set)
  util_gc_poll_register_liveness.py --asm FILE.s      grade an already-emitted .s
  util_gc_poll_register_liveness.py --selftest        fail-once and pass-once on a planted stream
  --regs r8,r12,...       which registers to follow (default: the anchor, the three sigma pins, r12
                          and rbx).  ⛔ r12 AND rbx ARE IN THE DEFAULT BECAUSE LEAVING THEM OUT WAS
                          THE FIRST READING'S MISS: bb_match_end passes r12 as a const char * and
                          hb_blob_span_defer WRITES through it at four existing safe points, all four
                          invisible to a reader that followed only r8 and the sigma pins.
  --by-site               one line per safe point instead of the per-register totals
"""
import argparse
import collections
import os
import re
import subprocess
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DEFAULT_REGS = ["r8", "r12", "r13", "r14", "r15", "rbx"]
POLL_RX = re.compile(r"\bcall\b.*\brt_gc_poll(?:_asm)?\b|\bcall\b.*\brt_gc_point_arr_c\b")
LABEL_RX = re.compile(r"^([A-Za-z_.$][\w.$]*):")
JUMP_RX = re.compile(r"^\s*(jmp|je|jne|jz|jnz|jg|jge|jl|jle|ja|jae|jb|jbe|js|jns|jo|jno|jp|jnp)\s+([\w.$]+)")
CALL_RX = re.compile(r"^\s*call\b")
RET_RX = re.compile(r"^\s*(ret|ud2|hlt)\b")
ARG_REGS = ("rdi", "rsi", "rdx", "rcx", "r8", "r9")
SUB32 = {"r8": "r8d", "r9": "r9d", "r10": "r10d", "r11": "r11d", "r12": "r12d",
         "r13": "r13d", "r14": "r14d", "r15": "r15d", "rax": "eax", "rcx": "ecx",
         "rdx": "edx", "rsi": "esi", "rdi": "edi", "rbx": "ebx"}
SUB16_8 = {"r8": ("r8w", "r8b"), "r13": ("r13w", "r13b"), "r14": ("r14w", "r14b"),
           "r15": ("r15w", "r15b"), "rax": ("ax", "al"), "rcx": ("cx", "cl"),
           "rdx": ("dx", "dl"), "rsi": ("si", "sil"), "rdi": ("di", "dil")}
NARROW_OPS = ("cmp", "test", "add", "sub", "and", "or", "xor", "shl", "shr", "sar", "inc", "dec", "imul")
WRITE_FIRST_OPS = ("mov", "movabs", "lea", "movzx", "movsx", "movsxd", "pop", "xchg", "setz", "setne")


def strip_comment(text):
    out = []
    for piece in text.split("\n"):
        out.append(piece)
    return "\n".join(out)


def parse_asm(text):
    """[(label_or_None, mnemonic, operand_text)] plus {label: index} -- ; separates instructions in our output"""
    prog, labels = [], {}
    for raw in text.split("\n"):
        line = raw.rstrip()
        if not line.strip():
            continue
        pending_label = None
        m = LABEL_RX.match(line.strip())
        if m:
            pending_label = m.group(1)
            line = line.strip()[len(m.group(0)):]
        for piece in line.split(";"):
            piece = piece.strip()
            if not piece:
                continue
            if piece.startswith("."):
                if pending_label is not None and pending_label not in labels:
                    labels[pending_label] = len(prog)
                pending_label = None
                continue
            m2 = LABEL_RX.match(piece)
            if m2 and pending_label is None:
                pending_label = m2.group(1)
                piece = piece[len(m2.group(0)):].strip()
                if not piece:
                    continue
            parts = piece.split(None, 1)
            mnem = parts[0].lower()
            ops = parts[1] if len(parts) > 1 else ""
            if pending_label is not None:
                labels.setdefault(pending_label, len(prog))
                pending_label = None
            prog.append((mnem, ops, piece))
        if pending_label is not None:
            labels.setdefault(pending_label, len(prog))
    return prog, labels


def successors(prog, labels, i):
    mnem, ops, _raw = prog[i]
    line = mnem + " " + ops
    m = JUMP_RX.match(" " + line)
    if m:
        tgt = labels.get(m.group(2))
        if m.group(1) == "jmp":
            return [tgt] if tgt is not None else []
        nxt = [i + 1] if i + 1 < len(prog) else []
        return nxt + ([tgt] if tgt is not None else [])
    if RET_RX.match(" " + line):
        return []
    return [i + 1] if i + 1 < len(prog) else []


def mem_operands(ops):
    return re.findall(r"\[([^\]]*)\]", ops)


def reg_tokens(text):
    return set(re.findall(r"\b(r[0-9]{1,2}[dwb]?|[er]?[abcd]x|[er]?[sd]i|[abcd][lh]|sil|dil|spl|bpl)\b", text))


RELOAD_RX = re.compile(r"^\s*mov\s+(r8|r9|r10|r11)\s*,\s*qword ptr \[rip \+ rtccb\+\d+\]\s*$")


def reload_block(prog, i):
    """(index after the contiguous rtcc reload run at i, {regs that run DEFINES}) -- the reload IS the definition"""
    j, defined = i, set()
    while j < len(prog):
        mnem, ops, _raw = prog[j]
        m = RELOAD_RX.match(" " + mnem + " " + ops)
        if not m:
            break
        defined.add(m.group(1))
        j += 1
    return j, defined


def classify_use(mnem, ops, reg):
    """POINTER / SPILLED / NARROW / COPY:<reg> / None for one instruction's READ of reg -- None means no read"""
    wide = reg
    narrow = {SUB32.get(reg, "")} | set(SUB16_8.get(reg, ()))
    narrow.discard("")
    mems = mem_operands(ops)
    for m in mems:
        if wide in reg_tokens(m):
            return "POINTER"
    outside = ops
    for m in mems:
        outside = outside.replace("[" + m + "]", " ")
    toks = reg_tokens(outside)
    reads_wide = wide in toks
    reads_narrow = bool(narrow & toks)
    if not reads_wide and not reads_narrow:
        return None
    if mnem == "call":
        return "POINTER"
    fields = [f.strip() for f in outside.split(",")]
    if reads_wide and mnem in WRITE_FIRST_OPS and len(fields) > 1:
        dst, src = fields[0], fields[-1]
        if reg_tokens(dst) == {wide} and wide not in reg_tokens(src):
            return None
        if reg_tokens(src) == {wide} and mems:
            return "SPILLED"
        dt = reg_tokens(dst)
        if reg_tokens(src) == {wide} and len(dt) == 1:
            return "COPY:" + list(dt)[0]
        return "PROPAGATED"
    if reads_wide and mnem in NARROW_OPS:
        rhs = fields[-1]
        if re.match(r"^-?(0x)?[0-9a-fA-F]+$", rhs) or rhs.isdigit():
            return "NARROW"
        return "PROPAGATED"
    if reads_wide:
        return "PROPAGATED"
    return "NARROW"


def writes_reg(mnem, ops, reg):
    if mnem not in WRITE_FIRST_OPS and mnem not in NARROW_OPS and mnem != "xor":
        return False
    dst = ops.split(",")[0].strip()
    if dst.startswith("["):
        return False
    toks = reg_tokens(dst)
    if reg in toks:
        return True
    if SUB32.get(reg) in toks:
        return True
    return False


ORDER = {"POINTER": 4, "PROPAGATED": 3, "SPILLED": 2, "NARROW": 1}
BUCKETS = ("POINTER", "PROPAGATED", "SPILLED", "NARROW", "DEAD")


def verdict_at(prog, labels, start, reg, budget=400, hops=2):
    """forward CFG walk from start: the first classified READ on any path, or DEAD when every path writes first.

    A COPY is followed for `hops` more registers rather than answered, because `mov rbx, r13` then a dereference of
    rbx spends the same stale pointer -- stopping at the copy is how 78% of the first reading read PROPAGATED."""
    seen, work, best = set(), [(start, reg, hops)], None
    steps = 0
    while work:
        i, r, h = work.pop()
        if i is None or (i, r) in seen or i >= len(prog):
            continue
        seen.add((i, r))
        steps += 1
        if steps > budget:
            return "PROPAGATED", "walk budget %d exhausted -- not placed, never counted clean" % budget
        mnem, ops, raw = prog[i]
        use = classify_use(mnem, ops, r)
        if use is not None:
            if use.startswith("COPY:"):
                nxt = use.split(":", 1)[1]
                if h > 0:
                    if best is None:
                        best = ("PROPAGATED", raw.strip()[:88] + "   (copy, following %s)" % nxt)
                    work.append((i + 1, nxt, h - 1))
                    continue
                use = "PROPAGATED"
            tag = raw.strip()[:88] + ("" if r == reg else "   (via %s)" % r)
            if best is None or ORDER[use] > ORDER[best[0]]:
                best = (use, tag)
            if use == "POINTER":
                return best
            continue
        if writes_reg(mnem, ops, r):
            continue
        for s in successors(prog, labels, i):
            work.append((s, r, h))
    return best if best is not None else ("DEAD", "every path writes %s before reading it" % reg)


def grade_asm(text, regs):
    prog, labels = parse_asm(text)
    sites = [i for i, (m, o, raw) in enumerate(prog) if POLL_RX.search(raw)]
    rows = []
    for i in sites:
        after, defined = reload_block(prog, i + 1)
        per = {}
        for r in regs:
            per[r] = verdict_at(prog, labels, after if r in defined else i + 1, r)
        rows.append((i, per))
    return prog, sites, rows


def emit_asm(prog_path, scrip=None):
    scrip = scrip or os.path.join(ROOT, "scrip")
    fd, out = tempfile.mkstemp(suffix=".s")
    os.close(fd)
    r = subprocess.run([scrip, "--compile", "-o", out, prog_path], stdin=subprocess.DEVNULL,
                       capture_output=True, text=True, timeout=120)
    if r.returncode != 0 or not os.path.exists(out) or os.path.getsize(out) == 0:
        return None, (r.stderr or r.stdout or "").strip().splitlines()[-1:] or ["no asm produced"]
    return out, None


PLANT_CLEAN = """
main_bx:                call             rt_gc_poll_asm@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        cmp              r13, 5
                        mov              r13, 0
                        ret
"""
PLANT_BAD = """
main_bx:                call             rt_gc_poll_asm@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              rax, qword ptr [r13 + 8]
                        mov              r13, 0
                        ret
"""
PLANT_JUMP = """
main_bx:                call             rt_gc_poll_asm@PLT
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   other_α
                        mov              r13, 0
other_α:                mov              rcx, qword ptr [r13 + 16]
                        ret
"""
PLANT_R8_BAD = """
main_bx:                call             rt_gc_poll_asm@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r9,  0
                        mov              rax, qword ptr [r8 + 24]
                        ret
"""
PLANT_COPY_DEREF = """
main_bx:                call             rt_gc_poll_asm@PLT
                        mov              rbx, r13
                        mov              r13, 0
                        mov              rax, qword ptr [rbx + 8]
                        ret
"""
PLANT_SPILL = """
main_bx:                call             rt_gc_poll_asm@PLT
                        mov              qword ptr [rbp - 48], r13
                        mov              r13, 0
                        ret
"""


def selftest():
    fails = []

    def ck(cond, msg):
        print(("  ok   " if cond else "  FAIL ") + msg)
        if not cond:
            fails.append(msg)

    _p, s, rows = grade_asm(PLANT_CLEAN, ["r13"])
    ck(len(s) == 1, "(a) the reader finds the one planted safe point in the clean stream (found %d)" % len(s))
    ck(rows and rows[0][1]["r13"][0] == "NARROW",
       "(b) a compare against an immediate reads NARROW, so the D32 rule is applied and not merely quoted (read %s)"
       % (rows[0][1]["r13"][0] if rows else "nothing"))
    _p, s, rows = grade_asm(PLANT_BAD, ["r13"])
    ck(rows and rows[0][1]["r13"][0] == "POINTER",
       "(c) FAIL-ONCE: r13 used as a memory base after the poll reads POINTER (read %s)"
       % (rows[0][1]["r13"][0] if rows else "nothing"))
    _p, s, rows = grade_asm(PLANT_JUMP, ["r13"])
    ck(rows and rows[0][1]["r13"][0] == "POINTER",
       "(d) the walk FOLLOWS THE JUMP -- our polls sit before an rtcc reload that ends in jmp, so a reader that "
       "stopped at the jump would clear every site by never looking (read %s)"
       % (rows[0][1]["r13"][0] if rows else "nothing"))
    _p, s, rows = grade_asm(PLANT_BAD, ["r14"])
    ck(rows and rows[0][1]["r14"][0] == "DEAD",
       "(e) a register no path reads reads DEAD rather than clean-by-absence (read %s)"
       % (rows[0][1]["r14"][0] if rows else "nothing"))
    _p, s, rows = grade_asm(PLANT_CLEAN.replace("cmp              r13, 5",
                                                "mov              rbx, r13\n                        ret"), ["r13"])
    ck(rows and rows[0][1]["r13"][0] == "PROPAGATED",
       "(f) a copy whose destination is never used runs out of hops and reads PROPAGATED, its own bucket, never "
       "folded into an answer (read %s)" % (rows[0][1]["r13"][0] if rows else "nothing"))
    _p, s, rows = grade_asm(PLANT_R8_BAD, ["r8"])
    ck(rows and rows[0][1]["r8"][0] == "POINTER",
       "(g) FAIL-ONCE ON THE ARTIFACT THAT ALMOST SHIPPED: the rtcc reload is the first instruction after the poll "
       "and is itself a write of r8, so a walk starting there calls r8 DEAD and clears every site by never looking. "
       "The RELOADED r8 dereferenced afterwards must read POINTER (read %s)"
       % (rows[0][1]["r8"][0] if rows else "nothing"))
    _p, s, rows = grade_asm(PLANT_R8_BAD, ["r9"])
    ck(rows and rows[0][1]["r9"][0] == "DEAD",
       "(h) a reloaded register the stream then overwrites is DEAD, so arm (g) is discriminating rather than "
       "painting every reload red (read %s)" % (rows[0][1]["r9"][0] if rows else "nothing"))
    _p, s, rows = grade_asm(PLANT_COPY_DEREF, ["r13"])
    ck(rows and rows[0][1]["r13"][0] == "POINTER",
       "(i) a copy THEN a dereference of the copy reads POINTER -- the same stale pointer is spent either way "
       "(read %s)" % (rows[0][1]["r13"][0] if rows else "nothing"))
    _p, s, rows = grade_asm(PLANT_SPILL, ["r13"])
    ck(rows and rows[0][1]["r13"][0] == "SPILLED",
       "(j) a store of the register into memory reads SPILLED and is not claimed clean, because this reader cannot "
       "tell a mapped frame slot from a scratch cell (read %s)" % (rows[0][1]["r13"][0] if rows else "nothing"))
    print("SELFTEST arms=10 fail=%d" % len(fails))
    return 1 if fails else 0


def main(argv):
    ap = argparse.ArgumentParser(add_help=True)
    ap.add_argument("progs", nargs="*", default=[])
    ap.add_argument("--asm", default="")
    ap.add_argument("--selftest", action="store_true")
    ap.add_argument("--regs", default=",".join(DEFAULT_REGS))
    ap.add_argument("--by-site", action="store_true")
    ap.add_argument("--scrip", default="")
    a = ap.parse_args(argv)
    if a.selftest:
        return selftest()
    regs = [r.strip() for r in a.regs.split(",") if r.strip()]
    targets = []
    if a.asm:
        targets = [(a.asm, a.asm)]
    else:
        progs = a.progs
        if not progs:
            wd = os.path.join(ROOT, "scripts", "gc_witnesses")
            progs = sorted(os.path.join(wd, f) for f in os.listdir(wd)
                           if f.endswith((".sno", ".icn", ".pl", ".raku")))
        for p in progs:
            s, err = emit_asm(p, a.scrip or None)
            if s is None:
                print("REFUSE %-44s could not emit asm: %s" % (os.path.basename(p), "; ".join(err)))
                continue
            targets.append((p, s))
    if not targets:
        print("REFUSE(2) no program produced asm -- a denominator of zero is a refusal, not a clean reading")
        return 2
    tot = collections.Counter()
    sites_tot = 0
    worst = []
    for src, asm in targets:
        text = open(asm, encoding="utf-8", errors="replace").read()
        _prog, sites, rows = grade_asm(text, regs)
        sites_tot += len(sites)
        per_prog = collections.Counter()
        for idx, per in rows:
            for r in regs:
                v = per[r][0]
                tot[(r, v)] += 1
                per_prog[v] += 1
                if v == "POINTER":
                    worst.append((os.path.basename(src), idx, r, per[r][1]))
        if a.by_site:
            print("%-40s safe_points=%-4d %s" % (os.path.basename(src), len(sites),
                  " ".join("%s=%d" % (k, per_prog[k]) for k in BUCKETS)))
    print("POLL-REG-LIVENESS programs=%d safe_points=%d regs=%s" % (len(targets), sites_tot, ",".join(regs)))
    short = 0
    for r in regs:
        per = [tot[(r, b)] for b in BUCKETS]
        print("   %-5s %s  total=%d" % (r, " ".join("%s=%-5d" % (b, n) for b, n in zip(BUCKETS, per)), sum(per)))
        if sum(per) != sites_tot:
            short += 1
            print("   ⛔ %s buckets sum to %d over %d safe points -- a bucket is being dropped and every number "
                  "above it is short by the difference" % (r, sum(per), sites_tot))
    if short:
        print("REFUSE(2) the per-register buckets do not cover the printed denominator for %d register(s)" % short)
        return 2
    npointer = sum(tot[(r, "POINTER")] for r in regs)
    print("   POINTER total=%d over %d safe point(s) -- each is a register the collector relocates by nothing"
          % (npointer, sites_tot))
    for w in worst[:25]:
        print("   POINTER %-34s site=%-6d %-4s %s" % w)
    if len(worst) > 25:
        print("   ... %d more POINTER finding(s)" % (len(worst) - 25))
    return 1 if npointer else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
