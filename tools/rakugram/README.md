# tools/rakugram — Grammar.nqp → recursive-descent port

**Why this exists.** Lon ruled 2026-08-30 that the bison/flex Raku grammar is the wrong instrument:
Raku is not LALR, so `raku.y` can never be finished. See `.github/GOAL-RAKU-100.md` §LON RULING for
the measured case. The replacement is a *translation* of Rakudo's official grammar
(`src/Perl6/Grammar.nqp`) into recursive descent — bounded and mechanical, where construct-by-construct
patching of a `.y` was an unbounded search measured at ~3 files of PARSE-FAIL per pass against 924.

**Rung 1 (here): the READER + CENSUS. No code is emitted yet — deliberately.** The point is to cost the
port by measurement before building it.

```bash
python3 tools/rakugram/nqp_read.py [path/to/Grammar.nqp]
```

## What it reports, and what the numbers mean

- **723 productions** (640 `token`, 46 `rule`, 29 `proto token`, 4 `proto rule`, 4 `regex`) — each maps
  to one recursive-descent function. This is the size of the mechanical job.
- **39 protoregex families / 516 candidates** — longest-token-match dispatch, the thing bison cannot do.
- **133 subrules referenced but not defined here** — inherited from NQP's `HLL::Grammar`. ⛔ This is the
  part that is NOT a straight translation: `O`(202 uses) is operator-precedence *metadata* and
  `EXPR`(17) is the operator-precedence expression parser. Raku's operator table is **extensible at
  parse time** (`sub infix:<foo>` installs a new operator mid-parse), so `EXPR`/`O` must be built as a
  real Pratt parser over a mutable precedence table, not baked in.
- **`productions with an UNPARSED char`** and **`CONTAMINATED body`** are the honesty counters. They are
  not decoration: an unread construct that silently vanishes produces a parser that is confidently wrong,
  which is the same defect class as a test that skips-as-success.

## ⛔ Two traps, both measured here, both silent

1. **`|` is LONGEST-TOKEN-MATCH; `||` is FIRST-MATCH.** Counted in this file: 296 vs 183. Rendering `|`
   as ordered choice is the idiomatic recursive-descent reading and it is WRONG — it parses most inputs
   correctly and mis-parses exactly those where a later alternative matches longer.
2. **Naive brace matching overruns, and the totals still look plausible.** A production COUNT stays
   right while individual bodies absorb their successors. Measured causes, each found only by dumping
   context: a quote inside a character class (`token apostrophe { <[ ' \- ]> }`), a `#` inside one
   (`token spacey { <?[\s#]> }`), and French-quoted operator names whose content is literal angle
   brackets (`infix:sym«<»`, 49 occurrences). Char classes come in six spellings — `<[ <.[ <-[ <+[ <?[
   <![` — and missing any one of them reintroduces the bug. Fixing these took overruns 13 → 2.
