# HANDOFF — GOAL-ICON-BB: key() fix + set-membership bug found
Date: 2026-07-04

## Scope
corpus/benchmarks/icon/{concord,deal,geddump,ipxref,micro,micsum,queens,rsg,tgrlink,version}.icn,
diffed against the iconx oracle (byte-for-byte, not just rc=0/non-empty).

## Baseline correction
scripts/test_icon_bench_corpus.sh gates on rc=0 + non-empty stdout and does not
diff against the oracle. True correctness baseline, verified with OUTPUT=1 for
post.icn-linked programs plus banner-stripped diff:

  queens, concord, deal   MATCH oracle byte-for-byte
  version                 trivial (&version banner only, impl-specific)
  micro                   correct structure/labels; too slow to finish inside the
                          harness 30s cap (auto-calibrates to wall clock -- a perf
                          gap, not a logic bug)
  ipxref, rsg, geddump    near-zero real output vs 1208 / 5000 / 12568 oracle lines
  tgrlink                 2 / 3239 lines (header only, then nothing)
  micsum                  1 / 2 lines (header only, no computed summary row)

The earlier-reported "26 vs 30 lines" gap is benign: post.icn's Init__() reassigns
write := writes := 1 to suppress program output for timing; what remains is the
&version/&host/&features banner plus storage/GC/elapsed stats. SCRIP reports 4
fewer &features lines than real Icon (cosmetic), and those stats are inherently
non-deterministic run to run regardless.

## Fix landed this commit: key() table-key generator
lower_key in src/lower/lower_icon.c emitted IR_FAIL unconditionally, so key(t)
always failed and every table-key iteration silently produced nothing. Fixed by
reusing the working IR_ITERATE (unary-bang) Byrd box with a new "key" sval variant,
routing to rt_list_bang_key_at (already implemented in src/runtime/rt/rt.c) instead
of rt_list_bang_at. Dispatch added in src/templates/bb_iterate.cpp (mode-4/x86 path).

Verified: key() generates keys correctly standalone and via the full tgrlink kgen()
idiom (map(k[1],...) || k -> sort -> suspend k[2:0]). No regression: queens, concord,
deal still match the oracle byte-for-byte after rebuild.

Impact: did not green any benchmark on its own. Each of the five broken programs
hits its own earlier, independent bug before key() is ever reached. Still a real,
necessary fix -- tgrlink's kgen depends on it once the next bug (below) is cleared.

## Root cause found, NOT YET FIXED: set membership uses wrong equivalence for structures
Minimal repro (no benchmark needed):

  procedure main()
     local s, a, b
     a := [1,2]; b := [3,4]
     s := set()
     insert(s, a)
     write(member(s, a))   # both: succeeds -- inserted
     write(member(s, b))   # oracle: fails (distinct list, never inserted)
                           #  SCRIP: succeeds  <-- BUG
  end

Icon structures (list, record, ...) are compared by IDENTITY for set/table
membership, not structural equality. SCRIP's set appears to collapse distinct
structure values as equal on lookup (size stays correct at 1, so this is a
comparison bug, not a storage bug).

Confirmed as tgrlink's actual blocker via step-by-step trace, each stage checked
equal to oracle: file read (9417 lines) -> where() (identical offsets) -> crack()
string scan (identical field values) -> chtab population (probed: size=46 in both)
-> kgen/key() (23 dumpcode() calls, matching) -> putchain(). putchain's first line
is `if member(done, o) then return`; once anything is in done, every later object
false-matches, so putchain returns before writing. That is why output stops after
the 2-line header.

ipxref, rsg, and geddump all build sets/tables keyed on records or lists, so this
is a strong candidate for their failures too -- but NOT YET VERIFIED per program;
each still needs its own isolate-reduce-fix pass the way tgrlink was traced here.

## Recommended next steps, in order
1. Find the set member/insert comparison (likely shares hashing/comparison code
   with tables -- same runtime neighborhood as FIELD_GET_fn / table_get /
   by_name_dispatch.c). Fix structure-element comparison to use identity, matching
   Icon semantics, instead of deep/structural equality.
2. Rebuild; re-run the set/member/insert repro above to confirm the fix.
3. Re-check tgrlink end to end against the oracle (tgrlink.dat, full 3239-line diff).
4. Repeat the isolate-reduce cycle for ipxref, rsg, geddump, micsum -- each has its
   own first blocker not yet identified beyond "near-zero output."
5. Decide the target bar for micro: accept "correct structure, timing inherently
   variable, too slow for the current 30s harness cap" as a pass, or treat the
   runtime performance gap itself as a defect to fix.
6. Consider hardening scripts/test_icon_bench_corpus.sh to diff real output (via
   OUTPUT=1 plus banner-stripping for post.icn-linked programs) instead of
   rc=0/non-empty, so future regressions are caught rather than masked.

## Files changed this commit
- src/lower/lower_icon.c        (lower_key: IR_FAIL stub -> IR_ITERATE "key" variant)
- src/templates/bb_iterate.cpp  (dispatch "key" sval -> rt_list_bang_key_at)
- docs/HANDOFF-icon-bb-key-gen-set-member-2026-07-04.md (this file)
