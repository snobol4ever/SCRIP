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

Where each language stands is measured, not asserted: **Language status** below has a
section per language — master-suite coverage, vendor-suite boards (Icon is graded
against the official Arizona and JCON test suites, including how far the
JCON-compiler self-host gets), and every benchmark grid, with the gaps stated as
gaps.
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
ships with today (`src/parsers/`, below) are hand-written C built with flex/yacc;
`bootstrap/` is a second, self-hosted implementation living alongside them — evidence
the language is expressive enough to write a parser in, and fast enough to run one.
Running them today hits a known parse regression in the shared runtime — tracked, not
forgotten; a run-it-yourself snippet lands here once that clears.

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
src/parsers/   seven parsers (snobol4 icon prolog snocone rebus raku pascal)
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

## Language status — coverage and benchmarks

One section per language: what the test suites measure today, and where speed stands.
Numbers follow the house fact rules — coverage counts name their suite and denominator;
speed is a multiple on the faster axis (`reference time / SCRIP time` — above 1.00x
SCRIP is ahead, below 1.00x behind), rows in one grid share one instrument, and a
number's tree and date are part of its label. A missing grid is stated as missing,
never guessed. The seven-language summary, generated from the corpus masters and each
language's own floor gate (tree SCRIP `4d8a5595` / corpus `7f253b6a4`, 2026-08-30):

| Language | Master suite (`corpus/tests/<lang>/ALL.csv`) | Floor gate, fresh |
|---|---|---|
| SNOBOL4 | 1726 entries, 80 xfail | corpus board m3 **PASS=1672 FAIL=0** · m4 **PASS=1672 FAIL=0** SKIP=0 MISSING=0 |
| Icon | 534 entries, 1 xfail | rung ladder PASS=260 FAIL=8 XFAIL=28 / 297 · smoke 14/14 both modes |
| Prolog | 371 entries, 9 xfail | smoke 5/5 both modes |
| Raku | 129 entries, 14 xfail | master run m3 **PASS=724 FAIL=0** / 724 (m4 identical) |
| Pascal | 149 entries | m3 PASS=161 FAIL=2 · m4 PASS=153 FAIL=1 (suites watermark 96/96) |
| Snocone | 273 entries, 24 xfail | smoke 5/5 |
| Rebus | 48 entries | smoke 4/4 |

### SNOBOL4 / SPITBOL

**Coverage.** The complete frontend. The broad corpus board runs **FAIL=0 in both
modes** (1672 programs and suite entries, 2026-08-30); the 1726-entry master suite
carries 80 xfails, each a named, tracked defect — never a silent skip. Correctness is
triangulated four ways on the vendored **snoflake fixture suite** (180 self-describing
programs): SCRIP in both modes, SPITBOL, and CSNOBOL4 side by side, so a failure is
attributed to a real defect, a dialect difference, or a broken fixture by measurement
(`scripts/test_snoflake_suite.sh`). And `beauty.sno`, the SNOBOL4 pretty-printer
written in SNOBOL4, reproduces itself byte-for-byte in both modes.

**Benchmarks.** Measured 2026-08-27 (wall clock, scaled fixed work, `-O0` runtime,
output agreement verified per kernel before timing — a wrong answer is never a fast
answer):

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

### Icon

**Coverage.** The smoke is clean at 14/14 in both modes, and the 297-rung ladder
reads PASS=260 FAIL=8 XFAIL=28 (2026-08-30) — every FAIL and XFAIL a named, tracked
defect, and the ladder count moves as rungs land. The vendor-suite boards and the JCON self-host
distance follow the benchmark grid below.

**Benchmarks.** **× vs Arizona `iconx`** (10/10 kernels output-identical, 2026-08-27):

| kernel | × | kernel | × |
|---|:---:|---|:---:|
| int_loop | **5.64x** | list_dispatch | **2.52x** |
| mod_isolate | **4.65x** | table_miss_dispatch | **1.49x** |
| concat_dispatch | **4.45x** | concat_int_dispatch | 0.93x |
| concat_table | **2.64x** | concat_intvar | 0.72x |
| concat_strvar | **2.39x** | | |

**Vendor test suites.** Icon is graded against the two official vendor test suites, vendored in the corpus and
mechanically converted to SCRIP's explicit-semicolon dialect (`.std` oracles untouched
— the conversion changes no semantics, so PASS/FAIL measures the engine, and REJECT
names a real front-end parse gap). Measured 2026-08-30, SCRIP `508eeed5` / corpus
`6f60900b2`, `-O0`, both native modes:

| suite | mode | PASS | FAIL | REJECT | CRASH | HANG | of |
|---|---|:---:|:---:|:---:|:---:|:---:|:---:|
| Arizona Icon v9.5 `general/` | m3 = m4 | 32 | 41 | 16 | — | — | 89 |
| JCON test suite | m3 | 34 | 23 | 14 | 9 | 2 | 82 |
| JCON test suite | m4 | 32 | 28 | 14 | 6 | 2 | 82 |

These are unmodified real-world Icon programs exercising the full language surface;
the in-house Icon smoke and rung ladders (14/14 smoke, both modes; the rung suite's
own board) stay the regression floor. Runners:
`scripts/test_icon_arizona_suite.sh`, `scripts/test_icon_jcon_suite.sh` — each prints
its own totals and names every non-PASS.

**Self-host distance, measured the same day:** SCRIP compiles the 17-module JCON
translator — a production Icon compiler, `jtran`, written in Icon — into one native
x86-64 binary (656K lines of generated asm), and that binary starts and drives its
full `preproc → yylex → parse → ast2ir → bc_File` pipeline. Class-file output is
blocked on three engine gaps, each held by a minimal witness: a created call passes
literals but delivers frame-resident variables as `&null` (Icon law: `create` copies
current local values); `create` re-runs global initialization over the shared global
plane (Icon law: globals are untouched); and per-activation storage for recursive
generators (in flight as its own row). When the first lands, classes flow.

### Prolog

**Coverage.** Graded against GNU Prolog and SWI-Prolog. The 371-entry master suite
carries 9 xfails; the smoke is 5/5 in both modes (2026-08-30), and the
parser-conformance suite reads 12/15 (2026-08-28, up from 3/15 after the multiclause
backtracking cure). Prolog structure rides the same Byrd-box engine as everything
else — no separate term interpreter.

**Benchmarks.** The rivals grid (× vs `gprolog` and `swipl`, both installed; kernel
coverage gate green) is the frontier — it gets published here the same way, measured
with named instruments, when it lands. No number is quoted before then.

### Raku

**Coverage.** The 129-entry master expands to 724 graded runs and reads
**PASS=724 FAIL=0** in both modes (2026-08-30; the full run needs over a minute of
wall clock — a short timeout reads as a hang and is not a red). Parser suite 83/83
(2026-08-28); 14 xfails tracked in the master.

**Benchmarks.** A first kernel set with Rakudo-produced references is vendored under
`corpus/benchmarks/raku/`; the rivals grid is blocked on real-world syntax gaps in
the frontend (tracked as its own row) and lands after those clear.

### Pascal

**Coverage.** Oracle is Free Pascal (`fpc -Miso`) — every `.ref` regenerated from the
oracle, never from SCRIP's own output, with a per-file exception list. The 96-family
suites watermark holds in both modes; on the 149-entry master the fresh gates read
m3 PASS=161 FAIL=2 · m4 PASS=153 FAIL=1 (2026-08-30) — the residue is a known
nondeterministic loose-set class, tracked as its own row — and the benchmark
witnesses read 8/10.

**Benchmarks.** The `fpc` rivals grid is the frontier, same rules as Prolog's.

### Snocone

**Coverage.** Koenig's structured SNOBOL4, lowering into the same engine as SNOBOL4
itself. Master suite 273 entries, 24 xfail; smoke 5/5 (2026-08-30) and crosscheck 8/8
(2026-08-28, re-proven on a pristine build).

**Benchmarks.** No separate grid — Snocone compiles into the SNOBOL4 engine, so the
SPITBOL grids above are its speed story; a Snocone kernel set would re-measure the
same boxes.

### Rebus

**Coverage.** Master suite 48 entries; smoke 4/4 (2026-08-30). Graded through the
SNOBOL4 oracle path (SPITBOL x64) by construction.

**Benchmarks.** None yet — the smallest frontend, benchmarked as it grows.

Across all seven: the language-identity and medium-invisibility invariant gates are
clean at the same tree, and denominators grow as the corpus consolidates — each
script prints its own totals, and a printed total always outranks a number remembered
from this page.

## Credits

Built by Lon Jones Cherryholmes, working with Claude — Sonnet, Opus, and Fable. Part
of the [snobol4ever](https://github.com/snobol4ever) organization; the org README has
the full story of how a rediscovered 1980 idea (Byrd boxes) became a seven-language
compiler.

## License

AGPL v3. See [LICENSE](LICENSE).

---

*snobol4all. snobol4now. snobol4ever.*
