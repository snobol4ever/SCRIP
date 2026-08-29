# SCRIP

[![License: AGPL v3](https://img.shields.io/badge/License-AGPL_v3-blue.svg)](https://www.gnu.org/licenses/agpl-3.0)

One compiler, seven languages, native code. SCRIP compiles **SNOBOL4/SPITBOL, Snocone,
Icon, Prolog, Rebus, Raku, and Pascal** to x86-64 through a single engine: every
construct in every language lowers to the same IR of four-port **Byrd boxes**, and every
machine instruction is produced by one encoder. x86-64 is the shipping target; **JVM,
.NET, JavaScript, and WebAssembly backends are the planned roadmap** on the same engine.
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
./scrip --compile prog.sno       # emit standalone x86-64 assembly, assemble, link (mode 4)
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

**Mode 4 (`--compile`)** emits human-readable x86-64 assembly, assembles and links it
into a standalone binary against `out/libscrip_rt.so` (pattern engine, garbage
collector, builtins).

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
scripts/        500+ test, gate, board, and bench scripts, organized by prefix
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
| patmatch | 1M iterations | **0.55x** — the named gap: the core pattern lane, under active cure |

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

Status snapshot (2026-08-28, end of day): the SNOBOL4 corpus board runs **FAIL=0 in
both modes** (1299 programs and suite entries at that day's tree); `beauty.sno` — the
SNOBOL4 beautifier written in SNOBOL4 — reproduces itself byte-identically through the
compiler in both modes; per-language checks that day: Pascal suites 96/96 both modes
(and the loose-program set deterministic at 150/154), Icon smoke 14/14 both modes,
Snocone 5/5, Rebus 4/4, Prolog 4/5, Raku parser suite 83/83. Denominators grow as the
corpus consolidates; the boards print their own totals.

## License

AGPL v3. See [LICENSE](LICENSE).

---

*snobol4all. snobol4now. snobol4ever.*
