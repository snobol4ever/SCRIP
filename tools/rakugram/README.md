# tools/rakugram — Grammar.nqp → recursive-descent port

**Why this exists.** Lon ruled 2026-08-30 that the bison/flex Raku grammar is the wrong instrument:
Raku is not LALR, so `raku.y` can never be finished. See `.github/GOAL-RAKU-100.md` §LON RULING for
the measured case. The replacement is a *translation* of Rakudo's official grammar
(`src/Perl6/Grammar.nqp`) into recursive descent — bounded and mechanical, where construct-by-construct
patching of a `.y` was an unbounded search measured at ~3 files of PARSE-FAIL per pass against 924.

**Rung 1: READER + CENSUS** (`nqp_read.py`) · **Rung 2: AST + TRANSLATABILITY LADDER** (`nqp_ast.py`)
· **Rung 3: C EMITTER** (`nqp_emit.py`) · **Rung 4: PRECEDENCE / Pratt parser** (`nqp_prec.py`),
gated by `scripts/test_gate_rakugram_precedence.sh` · **Rung 5: REACHABILITY scoping** (`nqp_reach.py`) · **Rung 6: the hand-written HLL::Grammar layer** (`rk_hll.c`) · **Rung 7: character classes, escapes, anchors and lookahead are REAL** (`nqp_cc.py`, `rk_cc_test.c`).

```bash
python3 tools/rakugram/nqp_read.py [path/to/Grammar.nqp]   # rung 1: census of constructs
python3 tools/rakugram/nqp_ast.py  [path/to/Grammar.nqp]   # rung 2: parse bodies into an AST
python3 tools/rakugram/nqp_emit.py [Grammar.nqp] [out.c]   # rung 3: emit C recursive descent
gcc -c -O0 -Wall -Wextra out.c                             # rung 3 acceptance: compiles clean
python3 tools/rakugram/nqp_prec.py [Grammar.nqp] [out.c] --emit   # rung 4: the operator table
bash scripts/test_gate_rakugram_precedence.sh              # rung 4 acceptance (refuses rc=2 if unmeasurable)
```

## Rung 7 status — the primitives no longer lie (hq_C 2026-09-01)

⛔ **What rung 3 actually shipped for five node kinds was a placeholder that answered "matched" for
any input**, not a refusal: `rk_cclass` ignored its spec and ate one character (`<[0-9]>` matched
`z`), `rk_esc_s` likewise (`\d` matched `z`), `rk_anchor` and `rk_wb` returned 1, and `rk_look_stub`
returned 1 with the lookahead's operand **discarded**. Measured on the generated parser: **181 of 518
rules** rode on at least one of them, and **60 rules could never match at all** — a negative
lookahead over an always-true stub is `if (1) goto fail`. All five kinds were counted as MECHANICAL
by the ladder, so the headline number included them as translated.

⭐ **Why it was invisible:** every one of them passes every *positive* test. A class that matches
everything matches `'5'`. Only asking what a primitive REJECTS separates it from a stub, which is why
`rk_cc_test.c` is written as accept/reject pairs.

**The cure (`nqp_cc.py`):** classes, escapes and anchors are lowered **at generation time**, in
Python, into codepoint-range tables; the emitted C (`rk_cc`, `rk_anchor`, `rk_wb`) is total. A spec
that will not lower **refuses** (`RK_UNIMPL`) — never approximates. Lookahead now emits its operand
as a zero-width island; `<?{ code }>`, `<?after …>` and bare `<?>` refuse. `look_operand()` is ONE
function used by both the emitter and the called-set walk, because a lookahead that resolved to
`<alpha>` only at emit time produced a phantom `rk_alpha` — an implicit-declaration *warning*, so the
file compiled and would have linked against nothing.

**Honest numbers, same tree, one change** (`nqp_ast.py` now counts a shape as mechanical only if
`lowers()` says it does): generated rules **236 → 214**, refusing **243 → 265**, rules on a
silently-wrong primitive **181 → 0**, rules that can never match **60 → 0**, "fully mechanical"
**56.7% → 52.0%** (−35 productions). **The number went down because the instrument stopped lying.**
Three Raku facts the lowering asserts because each misparses plausibly: ranges are `..` not `-`
(`<[-−]>` is two literals, not a range swallowing every ASCII letter); whitespace inside `<[...]>` is
insignificant; `<?[…]>`/`<![…]>` are zero-width and must not advance.

## Rung 6 status — `ws` and the HLL layer exist

`rk_hll.c` hand-writes what cannot be generated from Grammar.nqp: **`ws`** (25 callers — `rule`
inserts it implicitly between every atom, so nothing parses until it exists; handles `#` comments and
`=begin`/`=end` pod), **`ident`/`identifier`**, the four integer literal forms, **`before`/`after`**
lookahead, and the 13 diagnostics as stubs. Gated with the rung-4 checks.

⭐ **`<sym>` was 22 of the 48 "missing" names and every one was a false positive.** Inside
`token infix:sym<*> { <sym> … }` it means *"match this candidate's own literal"* — per-declaration
data the emitter already holds, not a shared rule. A single `rk_sym()` would have to match 516
different literals with no way to know which. Now inlined: `rk_sym` is referenced **zero** times and
the inherited set drops 77 → 61. Measured before relying on it: **385 of 385** declarations using
`<sym>` carry a `:sym<>` value, and zero do not.

⛔ The gate asserts the SUBTLE cases, because each misparses *plausibly*: that `a - b` does not lex as
the identifier `a-b` (the hyphen needs a letter after it), that a leading `_` is not a number, and
that lookahead restores the cursor. A lookahead that consumes is not a lookahead, and the parse simply
continues from the wrong place.

## Rung 5 status — what actually has to be hand-written

A call-graph walk from `comp_unit` reaches **180 rules** (132 defined in Grammar.nqp, 48 inherited);
74 definitions are unreachable from the entry point and are **not** work to do.

Of the 48 inherited-and-reachable: **13 are diagnostics** (`panic`, `sorry`, `obs`, `NYI` … — they
change no parse decision, so a parser stubs them), **2 are already built** (`EXPR`, `O`, rung 4), and
**33 are real rules**, led by `ws` (25 distinct callers), `sym` (22), `ident`, `variable`, `nibble`,
`decint`/`hexint`/`octint`/`binint`, `before`/`after`, `typename`, `routine_def`.

⛔ **THE GRAMMAR'S CALL GRAPH IS NOT STATICALLY CONNECTED.** `comp_unit` reaches the entire program
through `<statementlist=.FOREIGN_LANG($*MAIN, 'statementlist', 1)>` — dispatching to a rule **by name,
as a string, at run time** (the slang mechanism; 27 such sites). A plain walk reports 12 reachable
rules and 199 "unreachable" definitions, which reads as a fact about the grammar rather than a limit
of the instrument. `nqp_reach.py` follows the string targets. ⭐ Note this is the *same* non-static
property as the parse-time-extensible operator table, appearing in the RULE graph instead of the
operators — a second independent reason a fixed table cannot express this language.

## Rung 4 status — `EXPR`/`O` is built and proven

31 precedence levels and 198 operators (164 infix, 24 prefix, 6 postfix) extracted from Grammar.nqp
into a **heap-backed, runtime-mutable** table plus a Pratt loop. The gate proves three things:

| | check |
|---|---|
| precedence | `2 + 3 * 4` → `(2 + (3 * 4))` |
| associativity | `1 - 2 - 3` → `((1 - 2) - 3)` · `2 ** 3 ** 2` → `(2 ** (3 ** 2))` |
| **parse-time extension** | install `fo` at `tA=` *at runtime* → `1 + 2 fo 3` → `(1 + (2 fo 3))` |

⛔ **Precedence is a STRING compared with `strcmp`, not an integer, and that is load-bearing.** A new
level can always be minted BETWEEN two existing ones by extending the string (`"t=" < "tA=" < "u="`) —
which is exactly how `is tighter(&infix:<+>)` is implementable. Integers have no room between 20 and
21; strings always have room. "Cleaning this up" into an int rank is the one refactor that would break
user-defined operators, and it would still pass every test built from a frozen table.

⛔ **Associativity is invisible to a syntax-only test** — a parser folding `1-2-3` as `1-(2-3)` accepts
exactly the same language. The gate asserts tree SHAPE for that reason.

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

## ⭐⭐ THE ANSWER: 86.7% of the official grammar is mechanically translatable

Measured over **739** productions (of 741; 2 excluded as contaminated, reported not hidden). Each row
is cumulative — the work needed to reach that line:

| reachable by | productions | cumulative |
|---|---|---|
| **mechanical today** | 477 ⚠ | **64.5%** ⚠ |
| + finishing the reader's residue | 87 | **76.3%** |
| + complex `:my` (a *local variable* — natural in RD, impossible in bison) | 34 | **80.9%** |
| + regex modifiers (`:i`, `:s`, `:dba(...)`) | 43 | **86.7%** |
| **REAL semantic work (`$*W`, `nqp::`, dynamic parse vars)** | **98** | 100% |

⚠ **The "mechanical today" row does not reproduce (hq_C 2026-09-01).** On today's tree the
shape-only count is **419 (56.7%)**, and once a shape is counted only if it actually lowers
(rung 7, `nqp_ast.lowers()`) it is **384 (52.0%)**. The 477 was computed before the reader change
that recovered 56 declarations and before the placeholder primitives were exposed; it is left in the
table so the correction is visible, not because it is current. Re-run `nqp_ast.py` for the live figure.

⚠️ **These numbers supersede an earlier 87.9% over 721 productions.** That population was wrong: the
declaration scanner did not match a parameterized head (`rule statementlist($*statement_level = 0) {`)
and silently dropped **56 declarations** — including the grammar's spine (`statementlist`, `statement`,
`xblock`, `quibble`). ⛔ **A dropped declaration does not look like an error, it looks like a smaller
grammar.** The conclusion is unchanged; the figure moved 1.2 points.

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
