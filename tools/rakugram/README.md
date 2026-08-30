# tools/rakugram — Grammar.nqp → recursive-descent port

**Why this exists.** Lon ruled 2026-08-30 that the bison/flex Raku grammar is the wrong instrument:
Raku is not LALR, so `raku.y` can never be finished. See `.github/GOAL-RAKU-100.md` §LON RULING for
the measured case. The replacement is a *translation* of Rakudo's official grammar
(`src/Perl6/Grammar.nqp`) into recursive descent — bounded and mechanical, where construct-by-construct
patching of a `.y` was an unbounded search measured at ~3 files of PARSE-FAIL per pass against 924.

**Rung 1: READER + CENSUS** (`nqp_read.py`) · **Rung 2: AST + TRANSLATABILITY LADDER** (`nqp_ast.py`)
· **Rung 3: C EMITTER** (`nqp_emit.py`).

```bash
python3 tools/rakugram/nqp_read.py [path/to/Grammar.nqp]   # rung 1: census of constructs
python3 tools/rakugram/nqp_ast.py  [path/to/Grammar.nqp]   # rung 2: parse bodies into an AST
python3 tools/rakugram/nqp_emit.py [Grammar.nqp] [out.c]   # rung 3: emit C recursive descent
gcc -c -O0 -Wall -Wextra out.c                             # rung 3 acceptance: compiles clean
```

## Rung 3 status — the emitter produces C that compiles

    generated rules   236      real recursive-descent bodies
    refusing rules    225      emit `return RK_UNIMPL`, never a silent "no match"
    proto dispatchers  39      longest-token-match over each family's candidates
    inherited stubs    77      NQP HLL::Grammar -- must be hand-written
    gcc -Wall -Wextra: 0 errors, 0 implicit declarations

⚠️ Remaining warnings are unused-function (rules not yet reachable — there is no entry point wired
yet) and unused-label. Both are artifacts of an incomplete port, not defects, and they go away as the
graph gets connected. **Nothing here parses real Raku yet** — the 77 inherited stubs all refuse.

⛔ **A rule we cannot emit REFUSES; it never returns "no match".** An unimplemented rule reporting
failure is indistinguishable from one that ran and correctly declined — the parser would be
confidently wrong instead of loudly incomplete.

## ⭐⭐ THE ANSWER: 87.9% of the official grammar is mechanically translatable

Measured over 721 productions (2 excluded as contaminated, reported not hidden). Each row is
cumulative — the work needed to reach that line:

| reachable by | productions | cumulative |
|---|---|---|
| **mechanical today** | 508 | **70.5%** |
| + finishing the reader's residue | 53 | **77.8%** |
| + complex `:my` (a *local variable* — natural in RD, impossible in bison) | 33 | **82.4%** |
| + regex modifiers (`:i`, `:s`, `:dba(...)`) | 40 | **87.9%** |
| **REAL semantic work (`$*W`, `nqp::`, dynamic parse vars)** | **87** | 100% |

Two refinements that moved this number and are worth keeping, because the naive count understates it
badly. Of 238 embedded `{...}` code blocks, **49% are either empty (a capture flush) or pure
error/panic reporting** — they change no parse decision, so a *parser* can stub them; only a full
compiler needs them. And `:my` reads as a blocker only against a bison baseline: in recursive descent a
parse-time local is just a local.

⛔ **The residue is not evenly hard.** The 87 semantic productions plus `EXPR`/`O` are the real build,
and `EXPR`/`O` is where Raku's operator table is **extended at parse time** — that is a Pratt parser
over a mutable table, and it is the single component that makes the whole language non-LALR.

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
