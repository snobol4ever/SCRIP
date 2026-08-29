# SCRIP

[![License: AGPL v3](https://img.shields.io/badge/License-AGPL_v3-blue.svg)](https://www.gnu.org/licenses/agpl-3.0)

One compiler, seven languages, native code. SCRIP compiles **SNOBOL4/SPITBOL, Snocone,
Icon, Prolog, Rebus, Raku, and Pascal** to x86-64 through a single engine: every
construct in every language lowers to the same IR of four-port **Byrd boxes**, and every
machine instruction is produced by one encoder. x86-64 is the shipping target; **JVM,
.NET, JavaScript, and WebAssembly backends are the near-term roadmap** on the same engine.
Part of the [snobol4ever](https://github.com/snobol4ever) organization.

---

## Quick start

```bash
make                 # → ./scrip  +  out/libscrip_rt.so
make setup           # fresh machine: install packages + reference oracles
make pristine        # full clean rebuild
```

```bash
./scrip prog.sno                 # compile AND run, in-process (mode 3 — the default)
./scrip --compile prog.sno       # emit standalone x86-64 assembly to stdout (mode 4)
./scrip --compile -o prog.s prog.sno   # keep the readable .s
./scrip prog.icn -- arg1 arg2    # program arguments go after --
```

The frontend is chosen by file extension:

| Extension | Language | Reference oracle |
|-----------|----------|------------------|
| `.sno` | SNOBOL4 / SPITBOL | SPITBOL x64, CSNOBOL4 |
| `.sc`  | Snocone (Koenig's structured SNOBOL4, Bell Labs TR 124) | SPITBOL x64 |
| `.icn` | Icon | Arizona `icont`/`iconx` |
| `.pl`  | Prolog | GNU Prolog, SWI-Prolog |
| `.reb` | Rebus | SPITBOL x64 (via SNOBOL4) |
| `.raku`| Raku | Rakudo |
| `.pas` | Pascal (ISO 7185) | Free Pascal (`fpc -Miso`) |

SNOBOL4 is complete; Icon is in active development; Prolog is experimental; Rebus,
Raku, and Pascal are newer frontiers, benchmarked as they land (see Benchmarks below).
Correctness is not asserted, it is diffed: every test runs against its language's
reference implementation, byte for byte.

There is no `--help`; running `./scrip` with no file argument prints usage. Other
flags: `--dump-ast | --dump-ir | --dump-ir-verbose | --dump-bb | --dump-zeta`
(pipeline introspection), `--transpile` (AST → portable SNOBOL4), `--bench`
(wall clock after execution), and SPITBOL-compatible `-sN` / `-mN` sizing.

## Two execution modes, one codegen

**Mode 3 (`--run`, the default)** wires flat x86 basic-block blobs into a sealed
executable slab inside the running process and jumps in — no assembler, no linker,
no temporary files.

**Mode 4 (`--compile`)** emits human-readable x86-64 assembly for a standalone
program. SCRIP itself only emits the `.s`; turning it into a binary is one more
step, against the same runtime mode 3 uses in-process:

```bash
./scrip --compile prog.sno > prog.s
gcc -c prog.s -o prog.o
gcc prog.o -L out -lscrip_rt -lm -Wl,-rpath,out -o prog
```

Each mode is graded against the oracles independently. Where they diverge, it is an
optimization choice — never a semantic one.

## The engine — Byrd boxes

Every pattern node, Icon generator, and Prolog goal compiles to the same four-port box:

| Port | Name | Meaning |
|------|------|---------|
| **α** | proceed | enter fresh |
| **β** | recede | resume after backtrack |
| **γ** | succeed | pass the result forward |
| **ω** | concede | fail, restore, propagate back |

Sequential composition wires γ of one box to α of the next. Alternation saves the
cursor on ω and restores it before the next alternative. The ports are wired at
compile time into straight-line jumps — the wiring *is* the execution; there is no
interpreter loop and no dispatch table on the hot path.

The model was first described by Lawrence Byrd (1980) for Prolog debugging and
generalized by Todd Proebsting (1996) as a code-generation strategy for goal-directed
languages. SNOBOL4 pattern matching, Icon's goal-directed generators, and Prolog's
unification-with-backtracking turn out to be three syntaxes over this one machine —
which is what makes seven frontends in one compiler a design rather than a pile.

Language identity stops at the parser: the driver dispatches once on file extension,
and everything downstream branches on IR kind only — no language enums, no language
globals, no per-language code paths past the lowering boundary (enforced by gate).

## Eating its own cooking

SCRIP beautifies its own source: `beauty.sno`, the SNOBOL4 pretty-printer written in
SNOBOL4, reproduces itself byte-for-byte when run through the compiler, in both modes.

Six of the seven frontends also exist a second time, as real Snocone source that
SCRIP itself compiles and runs — [`bootstrap/parser_<lang>.sc`](bootstrap) (SNOBOL4,
Snocone, Icon, Prolog, Rebus, Raku), sharing one small hand-written runtime under the
same directory. This is not a claim that `scrip` bootstraps itself: the frontends it
ships with today (`src/frontend/`, below) are hand-written C built with flex/yacc;
`bootstrap/` is a second, self-hosted implementation living alongside them — evidence
the language is expressive enough to write a parser in, and fast enough to run one.

## Seven languages, one compiland

> SCRIP is seven languages on five platforms, such that they can call each other and
> even co-exist in the same translation unit — one compiland.
> — Lon Jones Cherryholmes

Today that shows up as the polyglot `.scrip` format: one document, one fenced section
per language, compiled and run together from a single `scrip --run` invocation — see
[`test/cross_lang.scrip`](test/cross_lang.scrip) for three languages sharing one
process and one box-driving runtime. Full cross-language data sharing — one language
reading or writing a value another language set — is the active work, not yet
uniformly proven; x86-64 ships today, and JVM, .NET, JavaScript, and WebAssembly are
the near-term plan on the same engine, so the same program can eventually target any
of the five platforms named above.

## Layout

```
src/frontend/   seven parsers (snobol4 icon prolog snocone rebus raku pascal)
src/lower/      language → IR lowering
src/optimizer/  always-on IR optimizer
src/emitter/    IR → machine code (both modes)
src/templates/  bb/ box templates · x86/ the one instruction encoder · xa/ helpers
src/ir/         IR, box, and ζ-storage contracts
src/runtime/    pattern engine, GC, builtins · rtx/ hand-written asm runtime
src/driver/     the scrip CLI
bootstrap/      the self-hosted Snocone frontends (see above)
scripts/        500+ test, gate, and benchmark scripts, organized by prefix
```

## Testing

```bash
make test        # the blocking set: broad SNOBOL4 corpus (both modes) + invariant gates
```

Tests live in `scripts/`, navigable by prefix: `test_gate_*` (invariants that must
never regress), `test_smoke_*` (per-language smokes), `board_*` / `bench_*`
(scoreboards and benchmarks). The shared test corpus is the sibling
[`corpus`](https://github.com/snobol4ever/corpus) repository — SCRIP's scripts expect
it checked out beside this repo.

## Benchmarks — measured, labeled, gaps included

Speed claims follow the house fact rules: the unit is a multiple on the faster axis
(`reference time / SCRIP time` — above 1.00x SCRIP is ahead, below 1.00x behind), rows
in one grid share one instrument, and a number's tree and date are part of its label.
Measured 2026-08-27 (wall clock, scaled fixed work, `-O0` runtime, output agreement
verified per kernel before timing — a wrong answer is never a fast answer):

**SNOBOL4 × vs SPITBOL x64** (19/19 kernels output-identical; scaled trio):

| kernel | work | × vs SPITBOL |
|---|---|:---:|
| loopsum | 3M iterations | **2.25x** |
| concat | 200k appends | **22.7x** — SPITBOL grows O(n²) here; a class win, labeled as such |
| patmatch | 1M iterations | **0.55x** — the full match *pipeline*: see the resolution below |

**SNOBOL4 priority workloads × vs SPITBOL** (callgrind Ir, slope method — fixed work, no
startup — SCRIP mode-4, tree `2037a02f`, 2026-08-23; the ruled product target for these
is 2.00x–3.00x):

| workload | × vs SPITBOL |
|---|:---:|
| claws5 — the 3-level table gauntlet, real 66 KB corpus input | 0.734x |
| json — the deserializer, real 631 KB document | 0.422x |
| claws5 grammar only (pattern engine alone, zero captures) | **1.628x** |

These two pattern numbers are not a contradiction — they cut the same machine at
different joints, and the split *is* the finding. The bare pattern **engine** —
scanning, alternation, backtracking, zero captures — is compiled Byrd-box code and
beats SPITBOL at **1.628x**. The full match **pipeline** — the same engine plus
captures, deferred actions, and the runtime services they call out to — is behind
(patmatch 0.55x, pattern_bt 0.45x, string_pattern 0.43x). The compiled part wins;
the called-out part loses. That is why the cure work targets the runtime services,
not the codegen.

**SNOBOL4 17-kernel grid × vs SPITBOL** (callgrind Ir/iter, 2026-08-22 tree — restated
onto the faster axis from that scoreboard's own stated scrip/sbl columns):

| kernel | × | kernel | × | kernel | × |
|---|:---:|---|:---:|---|:---:|
| string_concat | **1.72x** | ident_call1 | 0.97x | string_pattern | 0.43x |
| fibonacci | **1.67x** | ident_call2 | 0.93x | table_access | 0.43x |
| var_access | **1.32x** | array_sum | 0.79x | indirect_dispatch | 0.43x |
| func_call | **1.24x** | eval_fixed | 0.67x | string_manip | 0.37x |
| op_dispatch | **1.19x** | pattern_bt | 0.45x | mixed_workload | 0.34x |
| arith_loop | **1.03x** | | | roman | 0.20x |

**SNOBOL4 demos × vs SPITBOL** (2026-08-28, whole-program wall-clock totals, best of
3 — a different basis, never comparable to the slopes above. Every arm's output is
byte-verified against its reference on all three engines before timing. Each demo has
three arms: the full program, its `-match` grammar-only core, and `-match-fence` with
FENCE control added. Mode 3's total *includes its in-process compile on every run*;
mode 4 times the prebuilt binary — at these 2–50 ms totals that difference dominates
the m3 column, so m4 is the like-for-like column against SPITBOL's precompiled runs):

| arm | m3 | m4 | | arm | m3 | m4 |
|---|:---:|:---:|---|---|:---:|:---:|
| claws5 | 0.33x | 0.72x | | calculator-1 | 0.83x | **2.10x** |
| claws5-match | 0.37x | 0.71x | | calculator-1-match | **3.06x** | **7.44x** |
| claws5-match-fence | 0.39x | 0.67x | | calculator-1-match-fence | **1.78x** | **3.67x** |
| json | 0.25x | 0.92x | | calculator-2 | 0.93x | **2.64x** |
| json-match | 0.29x | 0.74x | | calculator-2-match | 0.42x | 0.85x |
| json-match-fence | 0.34x | 0.80x | | calculator-2-match-fence | 0.48x | 0.95x |
| treebank | 0.14x | 0.50x | | | | |
| treebank-match | 0.47x | 0.71x | | | | |
| treebank-match-fence | 0.33x | 0.57x | | | | |

The reading: pure-match arms can crush (calculator-1-match at 7.44x), json is within 8%
of SPITBOL on totals, and the worst full program (treebank, allocation-heavy at 0.50x)
points at the same GC/allocator lever the profile work has already sized.

**Icon × vs Arizona `iconx`** (10/10 kernels output-identical):

| kernel | × | kernel | × |
|---|:---:|---|:---:|
| int_loop | **5.64x** | list_dispatch | **2.52x** |
| mod_isolate | **4.65x** | table_miss_dispatch | **1.49x** |
| concat_dispatch | **4.45x** | concat_int_dispatch | 0.93x |
| concat_table | **2.64x** | concat_intvar | 0.72x |
| concat_strvar | **2.39x** | | |

Prolog is graded for correctness against GNU Prolog and SWI-Prolog (204-program test
tree, all modes); its rivals speed grid, and Raku's and Pascal's, are the frontier —
they get published here the same way, measured with named instruments, when they land.

Correctness is also triangulated against independent implementations: the vendored
**snoflake fixture suite** (180 self-describing programs from the
[snoflake](https://github.com/atdt/snoflake) project) runs four arms side by side —
SCRIP in both modes, SPITBOL, and CSNOBOL4 — so a failure is attributed to a real
defect, a dialect difference, or a broken fixture by measurement, never by guess
(`scripts/test_snoflake_suite.sh`).

Status, reverified 2026-08-29 (tree `7817f370`, pristine build): the SNOBOL4 corpus
suite runs **FAIL=0 in both modes** (1299 programs and suite entries); the
language-identity and medium-invisibility invariant checks are both clean, and
`beauty.sno`'s self-reproduction (above) still holds in both modes. Per-language
checks as of 2026-08-28: Pascal
suites 96/96 both modes (and the loose-program set deterministic at 150/154), Icon
smoke 14/14 both modes, Snocone 5/5, Rebus 4/4, Prolog 5/5 in all three modes
(multiclause backtracking cured that evening; its parser-conformance suite at 12/15,
up from 3/15), Raku parser suite 83/83. Denominators grow as the corpus consolidates;
each script prints its own totals.

## Credits

Built by Lon Jones Cherryholmes, working with Claude — Sonnet, Opus, and Fable. Part
of the [snobol4ever](https://github.com/snobol4ever) organization; the org README has
the full story of how a rediscovered 1980 idea (Byrd boxes) became a seven-language
compiler.

## License

AGPL v3. See [LICENSE](LICENSE).

---

*snobol4all. snobol4now. snobol4ever.*
