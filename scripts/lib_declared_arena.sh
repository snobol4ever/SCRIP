#!/usr/bin/env bash
# lib_declared_arena.sh -- THE ONE READER OF A TEST'S DECLARED ARENA, for every runner that is not the
# Python harness. Source it; never copy a line of it (RULES.md, lib_* are SOURCED AUTHORITIES).
#
# ⛔⭐⭐ WHY THIS EXISTS AT ALL. Lon, 2026-09-23, in-chat to the ceo, verbatim: "Well, each test should have
# the needed amount of memory in the attribute TSV/CSV files." (routed CEO-1167). Before that word, a
# program whose live set did not fit the shipped window was a capacity red and an argument for raising the
# default for EVERYBODY. After it, the program DECLARES what it needs, in KB, in its suite's ALL.csv, and
# the runner exports SCRIP_HEAP_KB FOR THAT PROGRAM ONLY. corpus_suite_harness.py reads the same column
# through heap_declarations(); eight package runners execute programs directly and cannot call it, which is
# exactly how one question gets nine written answers. It gets one here.
#
# ⛔ A DECLARATION IS NOT A PIN. A pin is a runner or a gate deciding an arena for a program from the
# outside; util_arena_pin_census.py hunts those and has since CEO-1146. A declaration is the TEST saying
# what it needs, carried in the test's own attribute row, reviewable in a diff, and carried forward across
# every rebuild by both suite builders. The census learns the column so the two are told apart by evidence
# rather than by which file the number happens to sit in.
#
# ⛔⭐⭐ THE FLOOR IS NOT A STYLE RULE AND IT IS THE ONE THING A READER OF THIS FILE MUST NOT "TIDY AWAY".
# src/runtime/rt/gc_heap.c:
#     cap_kb = (long)GC_HEAP_CAP_KB; if (cap_kb < kb) cap_kb = kb;
# with #define GC_HEAP_CAP_KB 4096. So SCRIP_HEAP_KB moves the INITIAL WINDOW always, and the HARD CAP only
# once it climbs past 4096. MEASURED by the coo 2026-09-23, one binary, one 60000-element Icon witness:
#     SCRIP_HEAP_KB=128  -> rc=134, runtime prints "THE CAP IS 4096 KB"
#     SCRIP_HEAP_KB=2048 -> rc=134, runtime prints "THE CAP IS 4096 KB"
#     SCRIP_HEAP_KB=4096 -> rc=134, runtime prints "THE CAP IS 4096 KB"
#     SCRIP_HEAP_KB=4097 -> rc=134, runtime prints "THE CAP IS 4100 KB"   <- the cap moves here, not before
#     SCRIP_HEAP_KB=8192 -> rc=0
# A cell reading 2048 therefore declares NOTHING ABOUT CAPACITY -- the program it names reaches 4096 KB with
# or without it -- while reading to every downstream consumer exactly like a capacity declaration. That is
# the pin-the-runtime-silently-overrides class COO-151 caught one instrument ago, and the baton's own
# guardrail (matched at that arena, NOT matched one step below) is UNPROVABLE for such a cell: one step
# below carries the same cap, so both arms read the same and the evidence is vacuous. Refused, loudly, rc=2.
# ââ­â­ THE RULE, IN THE READER'S OWN HEADER ON THE ceo'S INSTRUCTION:
#     A DECLARATION IS A NUMBER ABOVE THE COMPILED CAP OR IT IS NOT A DECLARATION (ceo CEO-1171, 2026-09-23, ruling on the coo's ask).
# Verbatim from the ruling: "A heap_kb cell at or below 4096 moves the initial window and not the cap,
# so it declares nothing about capacity while reading to every consumer as if it did -- the pin-that-
# the-runtime-overrides class, and your own guardrail (matched at that arena, not matched one step
# below) is vacuous for it by construction. Refusing rc=2 with that sentence is the loud direction;
# warn-and-pass would mint the false label CEO-1101 exists against."
# â SO THE FLOOR BELOW IS NOT A LOCAL CHOICE AND MAY NOT BE SOFTENED TO A WARNING. It was asked
# for as a ruling rather than taken as one, because refusing is criterion-shaped and criterion-writing
# is not the instruments seat's verb; the answer came back as law and is cited here at the constant it
# governs, so a reader who reaches for the number reaches the rule in the same glance.
DECLARED_ARENA_CAP_KB=4096          # gc_heap.c #define GC_HEAP_CAP_KB
DECLARED_ARENA_MAX_KB=4194304       # 4096*1024, the ceiling SCRIP_HEAP_KB itself refuses past

# declared_arena_kb <all_csv> <entry>
#   Echoes the declared KB for <entry>, or NOTHING when the file, the column or the cell is absent --
#   and "nothing" is the shipped default, which is the honest answer and never an error. Returns 2 and
#   echoes nothing on a cell that is present but unusable, so a caller that tests the exit code gets a
#   refusal and a caller that only reads stdout gets the default. ⛔ NOT a grep: the column's index is read
#   from the header every time, because a positional read is the defect that put heap_kb in seven feature
#   denominators the hour it landed (util_feature_coverage_census.py, cured in the same landing).
declared_arena_kb() {
  local csv="$1" entry="$2"
  [ -n "$csv" ] && [ -f "$csv" ] || return 0
  python3 - "$csv" "$entry" "$DECLARED_ARENA_CAP_KB" "$DECLARED_ARENA_MAX_KB" <<'PY'
import csv, sys
path, want, cap, mx = sys.argv[1], sys.argv[2], int(sys.argv[3]), int(sys.argv[4])
try:
    with open(path, newline="") as f:
        rdr = csv.DictReader(f)
        if not rdr.fieldnames or "heap_kb" not in rdr.fieldnames:
            sys.exit(0)                      # suite not converted yet: shipped default, silently
        for n, row in enumerate(rdr, 2):
            if row.get("entry") != want:
                continue
            v = (row.get("heap_kb") or "").strip()
            if not v:
                sys.exit(0)                  # absent means the shipped default
            if not v.isdigit():
                sys.stderr.write("⛔ REFUSE(2) %s:%d heap_kb=%r is not a plain integer count of KB -- a "
                                 "declaration is a measurement, and a cell no reader can parse is not one\n"
                                 % (path, n, v))
                sys.exit(2)
            k = int(v)
            if k <= cap:
                sys.stderr.write("⛔ REFUSE(2) %s:%d heap_kb=%d is at or below the shipped hard cap of %d KB. "
                                 "gc_heap.c raises the cap only once the window passes it, so this cell moves "
                                 "the initial window and nothing about what the program can reach -- it would "
                                 "read as a capacity declaration while granting no capacity (measured "
                                 "2026-09-23: 128/2048/4096 all abort at a printed 4096 KB cap, 8192 completes). "
                                 "Declare above %d, or leave it empty and use SCRIP_GC_STRESS if the intent was "
                                 "collection pressure (CEO-1158)\n" % (path, n, k, cap, cap))
                sys.exit(2)
            if k > mx:
                sys.stderr.write("⛔ REFUSE(2) %s:%d heap_kb=%d is above the %d KB ceiling SCRIP_HEAP_KB itself "
                                 "refuses -- the runtime would abort on this value rather than run at it\n"
                                 % (path, n, k, mx))
                sys.exit(2)
            print(k)
            sys.exit(0)
except OSError:
    sys.exit(0)
PY
}

# run_at_declared_arena <all_csv> <entry> -- <command...>
#   Runs the command with SCRIP_HEAP_KB exported IFF <entry> declares one, and with the caller's
#   environment otherwise byte-identical to what it was before this lib existed. ⛔ THE SUBSHELL IS THE
#   POINT: an export that leaked would put one program's declared arena under every program after it in the
#   loop, which is a runner pinning an arena -- the exact thing a declaration replaces, reintroduced by the
#   mechanism meant to retire it.
run_at_declared_arena() {
  local csv="$1" entry="$2"; shift 2
  [ "$1" = "--" ] && shift
  local kb; kb=$(declared_arena_kb "$csv" "$entry") || return 2
  if [ -n "$kb" ]; then ( export SCRIP_HEAP_KB="$kb"; "$@" )
  else ( "$@" ); fi
}

# declared_arena_receipt <all_csv> -- one line naming every declaration a board is about to honour, so the
# published row says at which arena it was graded. A board that silently honours a declaration is a board
# whose number cannot be reproduced from its own output (RULES.md THE INSTRUMENT LAWS: a number is not
# labelled until it carries its tree, mode, oracle and RT_OPT -- the arena joins that list here).
declared_arena_receipt() {
  local csv="$1"
  [ -n "$csv" ] && [ -f "$csv" ] || return 0
  python3 - "$csv" "$DECLARED_ARENA_CAP_KB" "$DECLARED_ARENA_MAX_KB" <<'PY'
import csv, sys
cap, mx = int(sys.argv[2]), int(sys.argv[3])
try:
    with open(sys.argv[1], newline="") as f:
        rdr = csv.DictReader(f)
        if not rdr.fieldnames or "heap_kb" not in rdr.fieldnames:
            print("    DECLARED ARENA: this suite's attribute file carries no heap_kb column yet -- every "
                  "entry graded at the shipped default"); sys.exit(0)
        d = [(r.get("entry"), (r.get("heap_kb") or "").strip()) for r in rdr]
    # ⛔⭐ THE RECEIPT APPLIES THE READER'S OWN VALIDATION, AND THE FIRST DRAFT DID NOT. It printed every
    # non-empty cell as an honoured declaration, so a row reading heap_kb=2048 (refused above, because it
    # grants no capacity) and a row reading heap_kb=zzz (unparseable) both came out as "graded at a declared
    # arena" -- a board would have published "3 of 4 graded at a declared arena" over a population where the
    # runner honours exactly ONE. Caught by running the selftest both directions 2026-09-23 rather than by
    # reading the code. A census that counts what the cure does not apply is the worse half of the pair
    # (COO-153, same defect, opposite direction, one tick earlier), so the two share these four lines.
    def usable(v):
        if not v.isdigit():
            return False, "unparseable"
        k = int(v)
        return (cap < k <= mx), ("at or below the %d KB shipped cap, grants no capacity" % cap
                                 if k <= cap else "above the %d KB ceiling" % mx)
    named, refused = [], []
    for n, v in d:
        if not v:
            continue
        ok, why = usable(v)
        (named if ok else refused).append((n, v, why))
    if named:
        print("    DECLARED ARENA (CEO-1167): %d of %d entr(y/ies) graded at a declared arena, the rest at the "
              "shipped default -- %s" % (len(named), len(d), ", ".join("%s=%sKB" % (n, v) for n, v, _ in named)))
    else:
        print("    DECLARED ARENA (CEO-1167): 0 of %d entr(y/ies) declare one -- every entry graded at the "
              "shipped default" % len(d))
    if refused:
        print("    ⛔ DECLARED ARENA REFUSED (%d): these cells are present and are NOT honoured by the runner, "
              "so they are named here rather than counted above -- %s"
              % (len(refused), ", ".join("%s=%s (%s)" % r for r in refused)))
except OSError:
    pass
PY
}
