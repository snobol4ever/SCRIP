#!/usr/bin/env python3
"""util_arena_pin_census.py -- THE CENSUS OF RECORD for the CEO-1146 arena sweep: which gates PIN the arena,
BY NAME, in what FORM, and whether the pin is reachable from the blocking set.

CEO-1153 (2026-09-22, ruling the coo/ceo overlap option 1): "the arena sweep stays yours under CEO-1146 and its
population is your pinned census BY NAME ... my line-stripped grep reads 27 gates with a literal or brace-default
MB=1, 26 of them in the blocking set; yours is the census of record, the difference is comment stripping."

⛔ WHY THIS IS AN INSTRUMENT AND NOT A grep, stated where the number is.  Two seats greppting the same tree got 27
and 28.  A sweep whose POPULATION is in dispute cannot produce a receipt anyone can check, and "the difference is
comment stripping" is a hypothesis until something prints both halves.  So this census prints, for every gate:
the form, the line, and whether that line SURVIVES comment stripping -- and it prints the DISAGREEING set by name
rather than reconciling the totals silently.  A total that agrees by accident is not agreement.

⛔ THE STRIPPING RULE IS STATED, NOT ASSUMED.  Shell has no tokenizer here, so `#` is treated as starting a comment
only at the start of a line or after whitespace, and NEVER inside single or double quotes on that line.  That rule
is WRONG inside heredocs (a `#` there is data, not a comment) so every heredoc-interior line is reported as
AMBIGUOUS and counted in NEITHER half -- an instrument that guesses on the exact population under dispute is the
defect this row exists to close.

⛔ THE OTHER KNOB IS PART OF THE FORM (the cto, measured 2026-09-22; re-derived at gc_heap.c:191-192 by the coo).
:191 reads SCRIP_HEAP_MB and sets BOTH mb and kb; :192 reads SCRIP_HEAP_KB and OVERWRITES kb.  Last writer wins and
KB is last, so an exported SCRIP_HEAP_KB silently defeats an explicit MB pin -- measured: collector_visits arm 5
read gc2 0 of 5 under an outer KB=64 while passing at its own default.  A pin that does not unset the other knob is
therefore NOT A PIN, and this census grades that per gate (PIN-UNSAFE) instead of treating every pin as equal.
CEO-1153 makes `env -u` a condition of the sweep; two-knobs-set-refuses-rc2 is the ceo's 33rd batch.
"""
import os, re, subprocess, sys

HERE = os.path.dirname(os.path.abspath(__file__)); ROOT = os.path.dirname(HERE)

# Gates whose header seat is the cto: converted by them at SCRIP d58ac175b and EXCLUDED from this sweep by CEO-1153.
CTO_NINE = [
    "test_gate_c2bb_the_generator_arm_is_a_coroutine_start_not_a_box_entry_from_c.sh",
    "test_gate_gc_a_safe_point_stores_into_a_mapped_slot.sh",
    "test_gate_gc_emitted_safe_point_matches_its_contract.sh",
    "test_gate_gc_collector_visits_only_mapped_slots.sh",
    "test_gate_gc_the_caller_saved_spill_block_never_holds_a_heap_reference.sh",
    "test_gate_gc_the_plant_says_whether_it_applied.sh",
    "test_gate_gc_the_spine_cell_grid_is_anchored_at_the_region_base.sh",
    "test_gate_gc_the_spine_tagged_cell_walk_is_load_bearing.sh",
    "test_gate_gc_the_unmapped_spine_store_witness_answers_its_oracle_at_every_band_point.sh",
]

MB1 = re.compile(r'SCRIP_HEAP_MB\s*=\s*"?\'?1"?\'?(?![0-9])|SCRIP_HEAP_MB\s*:-\s*"?\'?1"?\'?(?![0-9])')
ANYMB = re.compile(r'SCRIP_HEAP_MB')
ANYKB = re.compile(r'SCRIP_HEAP_KB')
UNSET = re.compile(r'env\s+(-\w+\s+)*-u\s+SCRIP_HEAP_KB|unset\s+[^\n]*SCRIP_HEAP_KB')

def strip_comment(line):
    """Return (code, ambiguous_reason_or_None).  `#` starts a comment at line start or after whitespace, and never
    inside a quote on that line.  Quote tracking is per line and therefore approximate -- see the docstring."""
    out, sq, dq, prev = [], False, False, ""
    for ch in line:
        if ch == "'" and not dq: sq = not sq
        elif ch == '"' and not sq: dq = not dq
        elif ch == "#" and not sq and not dq and (prev == "" or prev.isspace()):
            break
        out.append(ch); prev = ch
    return "".join(out)

def heredoc_spans(lines):
    """1-based line numbers that sit INSIDE a heredoc body, where `#` is data and stripping would be wrong."""
    inside, tag, spans = False, None, set()
    for i, ln in enumerate(lines, 1):
        if not inside:
            m = re.search(r"<<-?\s*'?\"?([A-Za-z_][A-Za-z0-9_]*)'?\"?", ln)
            if m: inside, tag = True, m.group(1)
        else:
            if ln.strip() == tag: inside, tag = False, None
            else: spans.add(i)
    return spans

def blocking_set():
    mk = os.path.join(ROOT, "Makefile")
    try: txt = open(mk, encoding="utf-8", errors="replace").read()
    except Exception: return None
    return set(re.findall(r"bash\s+scripts/(test_gate_[A-Za-z0-9_]+\.sh)", txt))

def main():
    gates = sorted(f for f in os.listdir(HERE) if f.startswith("test_gate_") and f.endswith(".sh"))
    blocking = blocking_set()
    if blocking is None:
        print("⛔ REFUSE(2): cannot read the Makefile, so blocking-set membership cannot be measured"); return 2

    rows, ambiguous, mentions_only = [], [], []
    for g in gates:
        try: lines = open(os.path.join(HERE, g), encoding="utf-8", errors="replace").read().split("\n")
        except Exception as e:
            print("⛔ REFUSE(2): cannot read %s -- %s" % (g, e)); return 2
        hd = heredoc_spans(lines)
        body = "\n".join(lines)
        hits_code, hits_comment, hits_ambig = [], [], []
        for i, ln in enumerate(lines, 1):
            if not MB1.search(ln): continue
            if i in hd: hits_ambig.append((i, ln.strip()))
            elif MB1.search(strip_comment(ln)): hits_code.append((i, ln.strip()))
            else: hits_comment.append((i, ln.strip()))
        if hits_code or hits_ambig:
            kb = bool(ANYKB.search(body)); unset = bool(UNSET.search(body))
            rows.append((g, hits_code, hits_ambig, hits_comment, kb, unset))
            if hits_ambig and not hits_code: ambiguous.append(g)
        elif hits_comment:
            mentions_only.append((g, hits_comment))

    mine = [r for r in rows if r[0] not in CTO_NINE]
    theirs = [r for r in rows if r[0] in CTO_NINE]

    print("=" * 118)
    print("ARENA PIN CENSUS -- the CEO-1146 sweep population BY NAME (CEO-1153: this is the census of record)")
    print("=" * 118)
    print("  %d gate script(s) on disk . %d reachable from the Makefile . stripping rule stated in the docstring"
          % (len(gates), len(blocking)))
    print()
    print("  PINNED AT MB=1 ON A CODE LINE (%d) -- the population.  '*' = in the blocking set." % len(rows))
    for g, code, ambig, _c, kb, unset in rows:
        who = "cto (CEO-1153 excludes)" if g in CTO_NINE else "coo"
        mark = "*" if g in blocking else " "
        pin = "PIN-UNSAFE: names no SCRIP_HEAP_KB and does not unset it" if not (kb or unset) else \
              ("pin-safe: unsets the other knob" if unset else "PIN-UNSAFE: names SCRIP_HEAP_KB but never unsets it")
        print("   %s %-92s %s" % (mark, g, who))
        for i, ln in (code + ambig):
            tagged = "AMBIGUOUS(heredoc)" if (i, ln) in ambig else "code"
            print("        %-5s %-18s %s" % ("L%d" % i, tagged, ln[:80]))
        print("        %s" % pin)
    print()
    print("  MENTIONED ONLY IN A COMMENT, SO NOT PINNED (%d) -- THIS IS THE SET A LINE-STRIPPED grep AND A RAW grep"
          % len(mentions_only))
    print("  DISAGREE ABOUT, and it is printed by name rather than reconciled into a total:")
    for g, c in mentions_only:
        print("      %-92s L%d" % (g, c[0][0]))
    print()
    print("  IDENTITY: pinned %d + comment-only %d == %d gate(s) whose text matches an MB=1 form"
          % (len(rows), len(mentions_only), len(rows) + len(mentions_only)))
    print("  THE SWEEP'S OWN POPULATION: %d pinned, MINUS the cto's %d by header seat (CEO-1153) == %d MINE"
          % (len(rows), len(theirs), len(mine)))
    print("  of mine, %d are in the blocking set and %d are not."
          % (sum(1 for r in mine if r[0] in blocking), sum(1 for r in mine if r[0] not in blocking)))
    unsafe = [r[0] for r in mine if not r[5]]
    print("  PIN-UNSAFE (does not unset SCRIP_HEAP_KB, so an outer KB defeats the pin -- gc_heap.c:191-192): %d of %d"
          % (len(unsafe), len(mine)))
    if ambiguous:
        print("  ⛔ AMBIGUOUS, counted in NEITHER half and NOT swept until read by hand (%d): %s"
              % (len(ambiguous), " ".join(ambiguous)))
    print("population: %d gate(s) examined, %d pinned, %d mine" % (len(gates), len(rows), len(mine)))
    print("=" * 118)
    return 0

if __name__ == "__main__":
    sys.exit(main())
