# SCRIP

[![License: AGPL v3](https://img.shields.io/badge/License-AGPL_v3-blue.svg)](https://www.gnu.org/licenses/agpl-3.0)

One compiler, seven languages — and an eighth, hybrid one — native code. SCRIP compiles
**SNOBOL4/SPITBOL, Snocone, Icon, Prolog, Rebus, Raku, and Pascal**, and the polyglot
SCRIPtix **`.md`** document that mixes them, to x86-64 through a single engine: every
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
section per language with test-suite results and benchmarks. Correctness is diffed,
byte for byte, against each language's reference implementation.

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

Each mode is graded against the oracles independently (tables below abbreviate them
m3 and m4). Where they diverge, it is an optimization choice — never a semantic one.

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
globals, no per-language code paths past the lowering boundary (enforced by an
automated check in the test suite).

## Eating its own cooking

SCRIP beautifies its own source: `beauty.sno`, the SNOBOL4 pretty-printer written in
SNOBOL4, reproduces itself byte-for-byte when run through the compiler, in both modes
(its timing against SPITBOL is under [§ SNOBOL4 / SPITBOL](#snobol4--spitbol)).

Six of the seven frontends also exist a second time, as real Snocone source that
SCRIP itself compiles and runs — [`bootstrap/parser_<lang>.sc`](bootstrap) (SNOBOL4,
Snocone, Icon, Prolog, Rebus, Raku), sharing one small hand-written runtime under the
same directory. This is not a claim that `scrip` bootstraps itself: the frontends it
ships with today (`src/parsers/`, below) are hand-written C built with flex/yacc;
`bootstrap/` is a second, self-hosted implementation living alongside them — evidence
the language is expressive enough to write a parser in, and fast enough to run one.
Running them today hits a parse regression in the shared runtime; a run-it-yourself
snippet lands here once that is fixed.

## Seven languages, one compiland

> SCRIP is seven languages on five platforms, such that they can call each other and
> even co-exist in the same translation unit — one compiland.
> — Lon Jones Cherryholmes

Today that shows up as the polyglot SCRIPtix format, a plain Markdown `.md` file: one document, one fenced section
per language, compiled and run together from a single `scrip --run` invocation — see
[`corpus/tests/scrip_test/cross_lang.md`](https://github.com/snobol4ever/corpus/blob/main/tests/scrip_test/cross_lang.md) for three languages sharing one
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
../harness/scripts/        500+ test, gate, and benchmark scripts, organized by prefix
```

## Testing

```bash
make test        # the core set: broad SNOBOL4 corpus (both modes) + invariant checks
```

Tests live in `../harness/scripts/`, navigable by prefix: `test_gate_*` (invariants that must
never regress), `test_smoke_*` (per-language smokes), `board_*` / `bench_*`
(scoreboards and benchmarks). The shared test corpus is the sibling
[`corpus`](https://github.com/snobol4ever/corpus) repository — SCRIP's scripts expect
it checked out beside this repo.

## Language status — coverage and benchmarks

One section per language: test-suite results and benchmarks. **Coverage is stated
only in third-party numbers** — each language is graded against its reference
implementation's own published test suite, vendored unmodified. Speed is a multiple
on the faster axis (`reference time / SCRIP time` — above 1.00x SCRIP is ahead,
below 1.00x behind); every number carries its date, and every grid names the script
that produced it. The suite table:

<!-- SUITE-TABLE:BEGIN generated by .github/scripts/util_suite_banner.py --readme from /home/resources/progress/SUITES.tsv at suites@0c01562d8ea5 -- do not edit by hand; ../harness/scripts/test_gate_readme_suite_table_matches_suites_tsv.sh holds it -->
Every row is generated from the leaderboard's machine record, `/home/resources/progress/SUITES.tsv` (rendered into `.github/SCORE.md`),
never typed by hand: the suite's latest reading, written by that suite's own runner in the landing that measured it, in the suite banner's columns --
passing / graded (a program passes only when it passes in BOTH modes), the programs excluded from the denominator (no oracle answer, or outside the
oracle's dialect, each named beside its package), the percentage, the wall time of the suite's last full reading, ✅ at 100%, and the day measured. The seven rung suites are our own flat
suites with refs cut from each oracle; each Bench row is that language's benchmark programs graded as tests, a program passing when it prints its ref under all three angles (wrapper process, fixed iterations, fixed time limit); the others are vendored third-party suites. Raku is IN DEVELOPMENT: its rows stand as measured.

| Language | Suite | passing / graded | excluded | % | wall | state | measured |
|:---|:---|---:|---:|---:|---:|:---:|:---|
| SNOBOL4 | Gimpel | **144/144** | 1 | 100% | 43s | ✅ | `2026-10-25` |
| SNOBOL4 | Budne | **70/70** | 62 | 100% | 10s | ✅ | `2026-10-25` |
| SNOBOL4 | Flake | **126/126** | 54 | 100% | 19s | ✅ | `2026-10-25` |
| SNOBOL4 | AIS | **8/8** | 1 | 100% | 13s | ✅ | `2026-10-25` |
| SNOBOL4 | Dotnet | **6/6** | 8 | 100% | 2s | ✅ | `2026-10-25` |
| SNOBOL4 | TPgm | **8/8** | 0 | 100% | 2s | ✅ | `2026-10-25` |
| SNOBOL4 | X64T | **33/33** | 3 | 100% | 15m31s | ✅ | `2026-10-25` |
| SNOBOL4 | X32T | **16/16** | 5 | 100% | 4s | ✅ | `2026-10-25` |
| SNOBOL4 | SnoDemo | **24/24** | — | 100% | 6s | ✅ | `2026-10-23` |
| SNOBOL4 | SnoRungs (rungs) | **2142/2142** | 1 | 100% | 2m40s | ✅ | `2026-10-25` |
| SNOBOL4 | SnoBench | **23/23** | — | 100% | 16s | ✅ | `2026-10-25` |
| Icon | Zona | **118/118** | 6 | 100% | 33s | ✅ | `2026-10-25` |
| Icon | Jcon | **85/85** | 6 | 100% | 22s | ✅ | `2026-10-25` |
| Icon | IPL | **644/644** | 207 | 100% | 5m20s | ✅ | `2026-10-25` |
| Icon | IcnDemo | **7/7** | 3 | 100% | 9s | ✅ | `2026-10-23` |
| Icon | IcnRungs (rungs) | **935/935** | — | 100% | 45s | ✅ | `2026-10-25` |
| Icon | IcnBench | **26/26** | 0 | 100% | 4s | ✅ | `2026-10-25` |
| Prolog | INRIA | **440/440** | 5 | 100% | 6m39s | ✅ | `2026-10-25` |
| Prolog | SWI | **2342/2709** | 226 | 86% | 35m10s |  | `2026-10-24` |
| Prolog | GNU source | **43/45** | 17 | 96% | 2m28s |  | `2026-10-25` |
| Prolog | Logtalk | **3546/3546** | 71 | 100% | 4h32m | ✅ | `2026-10-25` |
| Prolog | ProDemo | **8/8** | 15 | 100% | 1m46s | ✅ | `2026-10-23` |
| Prolog | ProRosetta | **447/481** | 305 | 93% | 34m52s |  | `2026-10-25` |
| Prolog | ProPuzzles | **235/267** | 148 | 88% | 2h44m |  | `2026-10-25` |
| Prolog | TreallaTests | **272/327** | 129 | 83% | 4m33s |  | `2026-10-25` |
| Prolog | XsbTests | **170/550** | 357 | 31% | 9m53s |  | `2026-10-25` |
| Prolog | ProRungs (rungs) | **577/577** | — | 100% | 8m49s | ✅ | `2026-10-25` |
| Prolog | ProBench | **42/42** | 0 | 100% | 9m37s | ✅ | `2026-10-25` |
| Pascal | FPC | **94/94** | 87 | 100% | 3m31s | ✅ | `2026-10-24` |
| Pascal | PAT | **400/400** | 29 | 100% | 24s | ✅ | `2026-10-23` |
| Pascal | PasRosetta | **199/319** | 345 | 62% | 38m28s |  | `2026-10-25` |
| Pascal | PasDemo | **8/8** | 0 | 100% | 1m02s | ✅ | `2026-10-24` |
| Pascal | PasRungs (rungs) | **252/252** | — | 100% | 22s | ✅ | `2026-10-24` |
| Pascal | PasBench | **11/11** | 0 | 100% | 1m13s | ✅ | `2026-10-24` |
| Raku — IN DEVELOPMENT | Roast | **156/1401** | 63 | 11% | 11m07s |  | `2026-10-25` |
| Raku — IN DEVELOPMENT | RakDemo | **0/1** | — | 0% | — |  | `2026-10-10` |
| Raku — IN DEVELOPMENT | RakRungs (rungs) | **1062/1062** | — | 100% | 2m17s | ✅ | `2026-10-24` |
| Raku — IN DEVELOPMENT | RakBench | **80/80** | 4 | 100% | 7m44s | ✅ | `2026-10-23` |
| Snocone | SncDemo | **23/23** | — | 100% | 23s | ✅ | `2026-10-22` |
| Snocone | SncRungs (rungs) | **358/358** | — | 100% | 21s | ✅ | `2026-10-24` |
| Snocone | SncBench | **23/23** | — | 100% | 4s | ✅ | `2026-10-22` |
| Rebus | RebRungs (rungs) | **43/43** | — | 100% | 3s | ✅ | `2026-10-25` |
| Rebus | RebBench | **3/3** | — | 100% | 2s | ✅ | `2026-10-22` |
| SCRIPtix (hybrid) | TixDemo | **12/12** | — | 100% | 11s | ✅ | `2026-10-23` |

**35 of 44 suites at 100%.**
<!-- SUITE-TABLE:END -->

### SNOBOL4 / SPITBOL

**Coverage** (third-party, every row in the suite table above): **Gimpel** — Gimpel Software's SNOBOL4 program set, each
library module graded through a driver written for it, never excluded; **Budne** — Phil Budne's CSNOBOL4 test suite as
shipped (a program the SPITBOL oracle refuses is named in `OUTSIDE_SPITBOL_BASELINE.tsv` beside the package with the
measurement that put it there and stays in the denominator as debt); **Flake** — the [snoflake](https://github.com/atdt/snoflake)
fixture suite, 180 self-describing programs in CSNOBOL4's home dialect; **AIS** — the Shafto AI programs; **Dotnet** — the
.NET port's programs; **TPgm** — SPITBOL's own testpgms; **X64T** — the self-checking tests shipped with our x64 SPITBOL fork.
Every one is graded against the one oracle, `sbl -bf` (our SPITBOL x64 fork with its enhancements; RULES.md § Oracles), in both
modes; **SnoRungs** is our own flat rungs suite with refs cut from that oracle, and **SnoBench** the benchmark kernels graded as
tests. The numbers are in the table and nowhere else: a count typed here would have no writer.

**Benchmarks.** SnoBench is the 23 kernels under `corpus/benchmarks/snobol4/`, each a standalone program with its `.ref` cut from SPITBOL. The three-angle harness `../harness/scripts/test_snobol4_bench_suite.sh` runs every kernel three ways in each mode -- the pristine program under the external stopwatch `tools/bench_rusage`, a generated fixed-iteration twin and a generated fixed-time twin (`../harness/scripts/bench_wrap_snobol4.py`, the kernel never edited) -- and a kernel is published only when all three print its ref in both modes; with `BENCH_ORACLE_ARM=1` the same pristine program and the same twins run under the clean SPITBOL oracle, so the two engines are timed on the same generated program, on the same box, in the same minute.

*Measured 2026-10-25 19:43 CDT on SCRIP `9bd097fa9` / corpus `a2cab70e9` / harness `9e1deda`, **the whole of SCRIP built `-O2`** -- the compiler and the runtime (`-O2 -g -fno-strict-aliasing -fwrapv -fno-omit-frame-pointer`, in a scratch worktree; Lon 2026-10-25, in-chat to the ceo: *"For your benchmark results you'll need to build the C with -O2 command-line switch."* and *"You'll also want to build SCRIP with -O2 since mode 3 speed results will be affected."*) -- with the fleet idle (load 0.6-1.1 on 16 cores), **1 s of repetitions per point**, three iterations per fixed-iteration point (`BENCH_ORACLE_ARM=1 BENCH_BUD_MS=1000 BENCH_ITER_N=3 bash ../harness/scripts/test_snobol4_bench_suite.sh`), oracle `spitbol-bench-oracle/sbl -bf` (the clean benchmark build, never the monitor-hooked correctness fork), each kernel at its declared heap and stack, SCRIP with every diagnostic off (`SCRIP_DIAG=0`). 23 of 23 kernels printed their ref on every angle in all three engines. The per-repetition figure is the fixed-time twin's (microseconds per repetition of the kernel); the multiple is SPITBOL time / SCRIP time on that same twin (above 1.00x SCRIP is ahead).*

**Runtime and compile, separated.** Every multiple in these grids is RUNTIME ONLY: the generated twin reads the clock at the first and last line of its timed loop, inside the program, so start-up and compile never enter it. The start+compile columns come from a second generated copy of each pristine program with a clock read as its first source line and another at its last, every way out routed through it (`bash ../harness/scripts/bench_stages.sh snobol4`, the generators' `--mode once`), the same copy run under SPITBOL, mode 3 and the mode-4 binary under the external stopwatch `tools/bench_rusage`: RUNTIME = last line - first line, and START+COMPILE = the stopwatch's elapsed - RUNTIME, the median of 7 runs (2026-10-26 09:49 CDT, the same `-O2` build, harness `fabf5cb`, load 0.3-0.4). For SPITBOL and mode 3 that is process start-up plus the compile, since both compile on every run. The start-up column is the same measurement for the mode-4 binary, compiled ahead of the run: process start and the runtime's own initialization. An empty program (`END` alone) reads 0.71 ms on SPITBOL, 2.03 ms on mode 3 and 1.29 ms on a mode-4 binary: that much of each figure is start-up.

| kernel | SPITBOL start+compile ms | SPITBOL µs/rep | m3 start+compile ms | m3 µs/rep | m4 start-up ms | m4 µs/rep | SPITBOL / m3 | SPITBOL / m4 |
|---|---:|---:|---:|---:|---:|---:|:---:|:---:|
| arith_loop | 1.4 | 24.7 | 3.7 | 6.10 | 2.2 | 6.38 | 4.043x | 3.869x |
| arith_loop_twin | 1.6 | 7,064 | 3.6 | 2,006 | 2.2 | 1,946 | 3.521x | 3.630x |
| array_sum | 1.5 | 582.8 | 4.4 | 200.0 | 2.2 | 191.4 | 2.914x | 3.045x |
| eval_fixed | 1.5 | 137.8 | 3.7 | 55.1 | 2.2 | 54.5 | 2.501x | 2.531x |
| fib_recur | 1.5 | 3,750 | 4.1 | 1,789 | 2.1 | 1,854 | 2.096x | 2.023x |
| fibonacci | 1.3 | 213.7 | 4.4 | 105.4 | 2.1 | 110.1 | 2.028x | 1.942x |
| func_call | 1.4 | 34.2 | 4.2 | 13.4 | 2.2 | 14.1 | 2.558x | 2.420x |
| ident_call1 | 1.6 | 29.4 | 3.9 | 10.7 | 2.0 | 9.99 | 2.750x | 2.943x |
| ident_call2 | 1.5 | 32.5 | 3.6 | 9.60 | 2.1 | 8.96 | 3.387x | 3.627x |
| indirect_dispatch | 1.5 | 26.7 | 4.2 | 18.1 | 2.1 | 19.5 | 1.470x | 1.368x |
| mixed_workload | 1.5 | 31.3 | 5.7 | 22.7 | 2.1 | 21.8 | 1.377x | 1.437x |
| name_indirection | 1.6 | 1,143 | 3.9 | 445.6 | 1.9 | 452.2 | 2.566x | 2.529x |
| op_dispatch | 1.4 | 51.7 | 3.9 | 14.7 | 1.9 | 13.5 | 3.520x | 3.826x |
| pattern_bt | 1.4 | 224.5 | 4.3 | 50.8 | 2.2 | 52.7 | 4.419x | 4.264x |
| roman | 1.5 | 161.6 | 5.2 | 148.6 | 2.2 | 147.2 | 1.087x | 1.097x |
| string_concat | 1.5 | 47.4 | 3.7 | 133.0 | 2.1 | 132.8 | 0.357x | 0.357x |
| string_concat_twin | 1.4 | 202.5 | 3.8 | 531.5 | 2.2 | 531.3 | 0.381x | 0.381x |
| string_manip | 1.7 | 56.3 | 3.7 | 36.1 | 2.1 | 35.9 | 1.559x | 1.567x |
| string_pattern | 1.5 | 66.6 | 4.3 | 39.9 | 2.2 | 37.9 | 1.670x | 1.756x |
| table_access | 1.5 | 769.2 | 4.6 | 343.1 | 2.1 | 341.0 | 2.241x | 2.255x |
| table_variety | 1.5 | 590.7 | 7.5 | 246.4 | 2.2 | 241.6 | 2.397x | 2.445x |
| test_icon | 1.6 | 1.06 | 6.6 | 0.229 | 2.1 | 0.215 | 4.638x | 4.936x |
| var_access | 1.7 | 63.0 | 4.2 | 14.6 | 2.1 | 14.1 | 4.319x | 4.478x |

Geometric mean over the 23 kernels, runtime only: **2.129x** in mode 3, **2.151x** in mode 4; SCRIP ahead of SPITBOL on 21 of 23 kernels in mode 3 and 21 in mode 4. The two kernels behind it are string concatenation (`string_concat` 0.357x, `string_concat_twin` 0.381x, both modes).

**The workhorse demos, timed.** The fifteen SNOBOL4 programs of `corpus/demos/snobol4/DEMO-SCALE.tsv` -- claws5 (a CLAWS5 part-of-speech tagger), treebank (a Penn-Treebank bracket parser), json (a JSON parser), calculator, beauty (the pretty-printer beautifying its own 1,631 lines: 671 of `beauty.sno` and its sixteen `-INCLUDE`s) and their `-match` and `-match-fence` variants (the same scan written as one pattern match, with and without `FENCE`) -- each on its workhorse input, the test input repeated to the scale its row declares (one KB is 1,024 bytes), where every engine must print SPITBOL's answer. *Measured 2026-10-26 10:02-10:04 CDT on the same `-O2` build (SCRIP `9bd097fa9` / corpus `a2cab70e9` / harness `259c660`), load 0.4-1.0 on 16 cores, `bash ../harness/scripts/bench_stages.sh snobol4 --reps 7 --prog <demo> --input <scaled input>` for each row: the program verbatim with a clock read at its first source line and one at its last (every goto to END routed through it), the same copy run under each engine at its declared sizes; RUNTIME = last line - first line, START+COMPILE = the external stopwatch's elapsed - RUNTIME, medians of 7 runs; SPITBOL is the clean benchmark build, run where its `-INCLUDE`s resolve.*

| demo (input) | SPITBOL start+compile ms | SPITBOL runtime ms | m3 start+compile ms | m3 runtime ms | m4 start-up ms | m4 runtime ms | SPITBOL / m3 | SPITBOL / m4 |
|---|---:|---:|---:|---:|---:|---:|:---:|:---:|
| claws5 (x16, 1,043 KB) | 3.92 | 73.9 | 8.21 | 48.9 | 3.7 | 43.5 | 1.511x | 1.701x |
| treebank (x1024, 327 KB) ¶ | 4.20 | 199.1 | 19.1 | 208.1 | 4.1 | 195.0 | crashed 3 of 7 | crashed 1 of 7 |
| json (x1, 1,687 KB) | 4.30 | 43.6 | 26.7 | 57.0 | 4.2 | 56.3 | 0.765x | 0.776x |
| calculator (x4, 127 KB) | 3.04 | 33.9 | 12.6 | 108.9 | 3.2 | 92.0 | 0.311x | 0.368x |
| beauty (x1, 43 KB) | 3.17 | 29.0 | 286.7 | 45.2 | 3.8 | 37.0 | 0.642x | 0.784x |
| calculator-1-match (x1, 32 KB) | 1.70 | 32.4 | 8.12 | 66.5 | 2.6 | 45.0 | 0.487x | 0.719x |
| calculator-1-match-fence (x1, 32 KB) | 1.77 | 13.5 | 6.50 | 12.4 | 2.2 | 10.4 | 1.093x | 1.296x |
| calculator-2-match (x4, 127 KB) | 1.86 | 4.54 | 9.03 | 25.9 | 2.9 | 6.21 | 0.175x | 0.730x |
| calculator-2-match-fence (x4, 127 KB) | 2.11 | 6.50 | 7.01 | 9.64 | 2.5 | 3.15 | 0.674x | 2.059x |
| treebank-match (x1024, 327 KB) | 1.95 | 4.10 | 7.88 | 18.3 | 2.7 | 3.98 | 0.224x | 1.030x |
| treebank-match-fence (x1024, 327 KB) ¶ | 2.04 | 4.75 | 5.87 | 2.96 | 2.4 | 1.51 | crashed 4 of 7 | crashed 2 of 7 |
| claws5-match (x32, 2,086 KB) | 1.80 | 6.25 | 6.85 | 16.6 | 2.8 | 6.02 | 0.377x | 1.039x |
| claws5-match-fence (x32, 2,086 KB) | 1.65 | 6.88 | 5.96 | 10.2 | 2.8 | 5.29 | 0.671x | 1.299x |
| json-match (x1, 1,687 KB) | 2.08 | 7.72 | 12.2 | 37.5 | 2.9 | 11.3 | 0.206x | 0.686x |
| json-match-fence (x1, 1,687 KB) | 2.55 | 9.68 | 11.7 | 32.9 | 3.2 | 11.9 | 0.295x | 0.813x |

Geometric mean over the 13 demos every run of which answered: **0.467x** in mode 3 and **0.934x** in mode 4 on runtime alone (SCRIP ahead on 2 and 6). **¶** `treebank` and `treebank-match-fence` gave SPITBOL's answer on their good runs (the figures are those runs') and crashed on the others -- a jump into unmapped memory, SIGSEGV, in both modes; they carry no multiple, and the crash is a rank-1 row (`test_demo_repeat_at_scale.sh treebank 30` reads m3 5 and m4 4 of 30 bad on origin `1cf048007`). Mode 3's start+compile is the compile on every run: 6-27 ms for these programs and 287 ms for beauty's 1,631 lines, against SPITBOL's 1.7-4.3 ms; the mode-4 binary starts in 2.2-4.2 ms.

### Icon

**Coverage** (third-party, every row in the suite table above): **Zona** — Arizona's own test set, shipped with the
reference implementation; **Jcon** — the tests of Proebsting & Townsend's Icon-to-JVM compiler; **IPL** — the Icon Program
Library's programs, each with a ref cut from Arizona `icont`/`iconx` 9.5; **IcnRungs** is our own flat rungs suite and
**IcnBench** the classic benchmark set graded as tests. The oracle everywhere is Arizona `iconx`; a vendored source is
converted to SCRIP's semicolon-required Icon on the way in and checked in converted. Their boards, and how far the JCON
self-host gets, follow the benchmark grid below.

**Benchmarks.** IcnBench is every Icon program under `corpus/benchmarks/icon` with a `procedure main` -- the ten micro-kernels (`bench_icn*`), the six classic programs (`concord`, `deal`, `geddump`, `ipxref`, `queens`, `rsg`), the four `rtx/` kernels and the five `shootout/` programs, plus `tgrlink` -- each a standalone program with its `.ref` cut from Arizona `icont`/`iconx` 9.5.25a and its own `.argv`, input and declared arena beside it. The three-angle harness `../harness/scripts/bench_triangulate_icon.sh` runs each kernel three ways in `iconx`, m3 and m4 -- the pristine program under `tools/bench_rusage`, a generated fixed-iteration twin and a generated fixed-time twin (`../harness/scripts/util_icon_bench_wrap.py`, the kernel never edited; the loop shape is the oracle's: a kernel whose state carries across calls under `iconx` is iterated one process per iteration, `PROC`) -- and a kernel is cited only when all three engines print the ref on every angle.

*Measured 2026-10-25 19:45-19:53 CDT on SCRIP `9bd097fa9` / corpus `a2cab70e9` / harness `9e1deda`, **the whole of SCRIP built `-O2`** (compiler and runtime, as in the SNOBOL4 section; the harness's banner line still prints a literal `RT_OPT=-O0`), fleet idle, load 0.9-1.6 on 16 cores, **1 s of repetitions per fixed-time point, three iterations per fixed-iteration point** (`BUDGET_MS=1000 BENCH_N_DEFAULT=3`), oracle Arizona `iconx` 9.5.25a, SCRIP with every diagnostic off (`SCRIP_DIAG=0`). Every kernel printed its ref on every run of all three angles in every engine. The per-iteration figure is the fixed-time twin's (microseconds per iteration of the kernel's own `main`); the multiple is `iconx` time / SCRIP time on that same twin (above 1.00x SCRIP is ahead). The angles column names any engine whose two timed angles did not agree within 15% (`n/a`: too few iterations to compare); `iconx` reports no in-process work for `ipxref` and `queens`, so those rows carry no multiple.* Every multiple is runtime only: the wrapper reads `&time` at the first and last line of the kernel's own `main`, inside the process, for `PROC` kernels too. The m3 start+compile column is the same runner's per-process external elapsed minus that in-process work (`BUDGET_MS=1 BENCH_N_DEFAULT=1 bash ../harness/scripts/bench_triangulate_icon.sh --out`, three passes, the median, 2026-10-26 09:52-09:55 CDT, whole milliseconds so +/-1 ms): mode 3 compiles the kernel in 5-25 ms of process time; the start-up columns are the same measurement for `iconx` loading the icode `icont` translated ahead (3-13 ms) and for the mode-4 binary (2-3 ms).

| kernel | loop | `iconx` start-up ms | `iconx` µs/it | m3 start+compile ms | m3 µs/it | m4 start-up ms | m4 µs/it | `iconx` / m3 | `iconx` / m4 | angles |
|---|---|---:|---:|---:|---:|---:|---:|:---:|:---:|---|
| int_loop | IN | 6.0 | 71,429 | 5.0 | 8,615 | 2.0 | 8,281 | 8.291x | 8.626x | AGREE |
| int_mod_isolate | IN | 3.5 | 70,000 | 5.0 | 11,222 | 2.0 | 10,381 | 6.238x | 6.743x | AGREE |
| str_concat_dispatch | IN | 9.5 | 73,571 | 5.0 | 10,255 | 2.0 | 10,286 | 7.174x | 7.153x | AGREE |
| str_concat_int_dispatch | IN | 6.0 | 102,000 | 6.0 | 35,379 | 2.0 | 35,483 | 2.883x | 2.875x | AGREE |
| str_concat_intvar | IN | 10.0 | 90,000 | 5.5 | 33,258 | 2.5 | 34,167 | 2.706x | 2.634x | AGREE |
| str_concat_strvar | IN | 9.0 | 75,714 | 5.5 | 30,394 | 2.5 | 30,939 | 2.491x | 2.447x | AGREE |
| str_concat_table | IN | 10.5 | 202,000 | 6.0 | 34,433 | 2.0 | 35,821 | 5.866x | 5.639x | AGREE |
| sub_list_dispatch | IN | 6.5 | 117,778 | 6.0 | 27,027 | 2.5 | 24,512 | 4.358x | 4.805x | AGREE |
| sub_table_miss_dispatch | IN | 10.0 | 90,000 | 5.5 | 28,306 | 2.0 | 28,083 | 3.180x | 3.205x | AGREE |
| sub_table_miss_semantics | IN | 6.0 | 1,818 | 6.5 | 909.1 | 2.0 | 907.4 | 2.000x | 2.004x | iconx n/a, m3 DISAGREE, m4 DISAGREE |
| concord | IN | 6.0 | 22,954 | 11.5 | 16,210 | 2.0 | 13,808 | 1.416x | 1.662x | AGREE |
| deal | IN | 5.0 | 28.8 | 11.0 | 28.3 | 2.0 | 12.3 | 1.018x | 2.341x | iconx n/a, m3 n/a, m4 n/a |
| geddump | PROC | 13.0 | 99,091 | 21.0 | 111,556 | 3.0 | 89,333 | 0.888x | 1.109x | AGREE |
| ipxref | PROC | 5.0 | 0.00 | 20.0 | 1,438 | 2.0 | 1,085 | — | — | iconx n/a, m3 DISAGREE |
| queens | PROC | 3.1 | 0.00 | 10.2 | 100.0 | 2.0 | 80.0 | — | — | iconx n/a, m3 DISAGREE, m4 n/a |
| rsg | PROC | 11.5 | 27,297 | 19.5 | 11,292 | 2.0 | 6,067 | 2.417x | 4.500x | iconx DISAGREE |
| agg_field_isolate | IN | 9.0 | 695,000 | 7.0 | 164,000 | 2.5 | 154,000 | 4.238x | 4.513x | AGREE |
| num_isolate | IN | 7.0 | 2,160,000 | 6.0 | 514,500 | 3.0 | 493,000 | 4.198x | 4.381x | AGREE |
| num_mixed | IN | 8.5 | 565,000 | 5.5 | 183,500 | 2.0 | 185,833 | 3.079x | 3.040x | AGREE |
| rel_isolate | IN | 11.5 | 1,320,000 | 7.0 | 519,000 | 3.0 | 494,000 | 2.543x | 2.672x | AGREE |
| fannkuch | IN | 8.5 | 6,757 | 9.0 | 3,030 | 2.5 | 3,232 | 2.230x | 2.090x | AGREE |
| mandelbrot | IN | 10.5 | 27,297 | 9.0 | 3,918 | 2.0 | 3,735 | 6.967x | 7.308x | AGREE |
| pidigits | IN | 3.0 | 59.7 | 20.0 | 55.2 | 2.0 | 51.2 | 1.082x | 1.166x | iconx n/a, m3 DISAGREE, m4 n/a |
| reverse-complement | IN | 4.5 | 27.7 | 7.5 | 35.6 | 2.0 | 20.2 | 0.778x | 1.371x | iconx n/a, m3 n/a, m4 n/a |
| spectral-norm | IN | 12.5 | 100,000 | 8.5 | 34,067 | 2.0 | 33,667 | 2.935x | 2.970x | AGREE |
| tgrlink | IN | 11.5 | 147,143 | 24.5 | 98,636 | 2.0 | 56,944 | 1.492x | 2.584x | AGREE |

Geometric mean over the 24 kernels with an `iconx` reading: **2.728x** in mode 3 and **3.131x** in mode 4 against Arizona `iconx`; over the 19 whose two timed angles agree in all three engines: **3.297x** and **3.512x**. SCRIP ahead on 22 of 24 in mode 3 and 24 in mode 4; behind only on `geddump` in mode 3 (0.888x) and `reverse-complement` in mode 3 (0.778x, a kernel too short for its angles to be compared), both ahead in mode 4.

**Vendor test suites.** Every Icon suite row — Zona, Jcon, IPL, IcnRungs, IcnBench — is in the suite table above and nowhere else (`../harness/scripts/test_icon_arizona_suite.sh`, `test_icon_jcon_suite.sh`, `test_icon_ipl_suite.sh`).

**Major demo — the JCON compiler, written in Icon.** JCON (Proebsting & Townsend, Arizona) is a production Icon-to-JVM compiler, itself written in Icon: 9,953 lines across 16 hand-written modules plus 2 that JCON generates with its own Icon programs — the largest real-world Icon program in the corpus. The demo entries live in `corpus/demos/icon/jcon/`, each a thin file of `link` directives resolved by SCRIP's own `icn_resolve_links`; the gate `../harness/scripts/test_demo_icon_jcon.sh` builds its own `icont` oracle from the same sources every run and grades stdout (jlink answers on stderr, and the oracle picks the stream once):

| demo | what it is | m3 | m4 | vs oracle |
|---|---|:---:|:---:|---|
| `interfacegen` | JCON's Java-interface table generator (415 lines out) | ✅ | ✅ | byte-identical |
| `jlink` | `jlink`, JCON's 2-module class linker (answers on stderr) | ✅ | ✅ | byte-identical |
| `jtran` | the full 17-module translator on its declared argv | ✅ | ✅ | byte-identical |
| `oplexgen` | JCON's operator-lexer generator (611 lines out) | ✅ | ✅ | byte-identical |

(`../harness/scripts/test_demo_icon_jcon.sh`, 2026-09-26 09:26 CDT on SCRIP `b12bb87f8`: 4 of 4 graded demos match the `icont`/`iconx` oracle in both modes, PASS(0).)

### Prolog

**Coverage** (third-party, every row in the suite table above): **INRIA** — the ISO/IEC 13211-1 conformance suite;
**Logtalk** — the Logtalk ISO conformance suite (3600 cases, each carrying its own expectation from the standard, graded
per family); **SWI** — the SWI-Prolog plunit test tree, graded whole; **GNU** — the GNU Prolog source tree's programs that
carry entry points, run against real `gprolog`; **ProRungs** is our own flat rungs suite and **ProBench** the classic van Roy
kernels graded as tests. The oracle is THE SUPERSET (RULES.md § Oracles): the ISO core, every non-conflicting GNU and SWI
builtin, and a conflict settled through an ISO Prolog flag in the program (double_quotes defaults to codes, as ISO and GNU
read it); `swipl` and `gprolog` are the two reference systems, and both are instrumented for the lock-step monitor.

**Benchmarks.** ProBench is the 42 kernels under `corpus/benchmarks/prolog/bench/` -- the classic van Roy / Aquarius set (tak, nreverse, qsort, deriv, ops8, times10, divide10, log10, queens, zebra, crypt, mu, query, sendmore, fib, ham, meta_qsort, ...) and the larger programs beside it (boyer, browse, chat_parser, prover, serialise, ...) -- each a standalone program with its `.ref` and its declared stack and heap beside it (`<k>.heap`, `<k>.stack`), graded as tests in both modes under three angles by `../harness/scripts/test_prolog_bench_suite.sh` (the suite table's ProBench row: 42/42 on 2026-10-25). The timing instrument is `../harness/scripts/test_bench_prolog_timed.sh`: each kernel's `bench_work` repeated for a fixed budget of CPU time in five engines -- the GNU Prolog compiler (`gplc`, native code), the GNU Prolog interpreter (`gprolog`), SWI-Prolog (`swipl`), SCRIP mode 3 and SCRIP mode 4 -- every engine's single-shot answer checked against the `.ref` before it is timed.

*Measured 2026-10-25 19:55-20:09 CDT on SCRIP `9bd097fa9` / corpus `a2cab70e9` / harness `9e1deda`, **the whole of SCRIP built `-O2`** (compiler and runtime, as in the SNOBOL4 section), the fleet idle (load 0.8-2.5 on 16 cores; the placement watch's hourly census ran on one or two cores during part of the pass), **1 s of iterations per point** (`TIME_BUDGET_MS=1000`), SCRIP with every diagnostic off (`SCRIP_DIAG=0`), GNU Prolog 1.6.0 and SWI-Prolog 9.0.4. The per-iteration figure is RUNTIME ONLY -- the self-measured WORK inside the generated program, `wall_us` read at the first and last line of its `main` around the counted `bench_work` loop, divided by the loop's count (the 2026-10-25 grid that stood here divided each process's whole CPU time by the count, so mode 3's compile of every program was charged to its iterations); the multiple is the rival's time / SCRIP's time on that figure (above 1.00x SCRIP is ahead). The m3 start+compile column is the same run's external elapsed minus that WORK, per process: about 120-145 ms for every kernel (the Prolog library prelude mode 3 compiles with each program), against `gprolog`'s consult 9-18 ms and `swipl`'s 9-13 ms; the start-up columns are the same measurement for the two native binaries, `gplc`'s (3-6 ms) and SCRIP mode 4's (2.7-3.8 ms; 13.7 ms for sieve). 31 of the 42 kernels are timed: the runner times a kernel only when GNU Prolog answers it single-shot, so the 11 kernels whose oracle is SWI-Prolog (chat_parser, det, flatten, moded_path, nand, perfect, pingpong, queens_clpfd, reducer, simple_analyzer, witness_depth_nrev8) carry no row here, and `gplc` refuses `eval` (a dark cell).*

| kernel | `gplc` start-up ms | `gplc` µs/it | `gprolog` start+compile ms | `gprolog` µs/it | `swipl` start+compile ms | `swipl` µs/it | m3 start+compile ms | m3 µs/it | m4 start-up ms | m4 µs/it | `gplc` / m3 | `gplc` / m4 | `gprolog` / m3 | `gprolog` / m4 | `swipl` / m3 | `swipl` / m4 |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|:---:|:---:|:---:|:---:|:---:|:---:|
| boyer | 4.1 | 10,215 | 18.0 | 18,734 | 11.5 | 18,663 | 434.6 | 18,170 | 2.7 | 17,098 | 0.562x | 0.597x | 1.031x | 1.096x | 1.027x | 1.092x |
| browse | 4.4 | 12,246 | 12.8 | 25,516 | 10.4 | 16,204 | 134.3 | 16,396 | 3.4 | 15,090 | 0.747x | 0.812x | 1.556x | 1.691x | 0.988x | 1.074x |
| cal | 4.7 | 0.151 | 11.4 | 0.427 | 9.6 | 1.78 | 125.2 | 1.29 | 3.2 | 0.316 | 0.117x | 0.476x | 0.331x | 1.350x | 1.377x | 5.621x |
| crypt | 4.2 | 30.58 | 12.0 | 112.1 | 10.1 | 216.6 | 130.1 | 102.1 | 3.2 | 85.91 | 0.299x | 0.356x | 1.098x | 1.304x | 2.122x | 2.522x |
| deriv | 4.0 | 1.53 | 10.0 | 1.99 | 8.8 | 4.92 | 128.4 | 5.53 | 3.1 | 5.3 | 0.277x | 0.289x | 0.359x | 0.375x | 0.890x | 0.928x |
| derive | 4.0 | 0.967 | 11.8 | 2.34 | 9.8 | 3.38 | 181.2 | 3.9 | 3.0 | 2.64 | 0.248x | 0.366x | 0.601x | 0.886x | 0.869x | 1.281x |
| divide10 | 4.7 | 0.553 | 10.7 | 1.09 | 9.8 | 2.09 | 132.6 | 2.4 | 2.9 | 1.08 | 0.230x | 0.512x | 0.455x | 1.012x | 0.869x | 1.933x |
| eval | — | — | 9.6 | 66.96 | 9.9 | 130.0 | 122.1 | 240.7 | 2.8 | 255.8 | — | — | 0.278x | 0.262x | 0.540x | 0.508x |
| fast_mu | 5.1 | 15.79 | 12.3 | 32.55 | 9.6 | 34.88 | 131.1 | 28.71 | 3.2 | 24.05 | 0.550x | 0.657x | 1.134x | 1.353x | 1.215x | 1.450x |
| fib | 4.3 | 437.3 | 11.1 | 1,469 | 9.2 | 1,761 | 121.4 | 741.8 | 3.1 | 671.3 | 0.589x | 0.651x | 1.980x | 2.188x | 2.373x | 2.622x |
| ham | 3.9 | 27.73 | 10.9 | 61.28 | 10.0 | 57.39 | 126.9 | 40.65 | 2.8 | 34.68 | 0.682x | 0.799x | 1.507x | 1.767x | 1.412x | 1.655x |
| log10 | 4.8 | 0.253 | 9.6 | 0.508 | 9.4 | 1.53 | 196.6 | 1.59 | 2.8 | 0.537 | 0.159x | 0.471x | 0.320x | 0.947x | 0.967x | 2.860x |
| meta_qsort | 4.3 | 119.9 | 12.2 | 191.0 | 10.2 | 142.5 | 207.6 | 157.4 | 2.8 | 133.3 | 0.762x | 0.900x | 1.213x | 1.433x | 0.905x | 1.069x |
| mu | 4.6 | 11.25 | 9.8 | 25.31 | 9.4 | 22.30 | 125.5 | 20.19 | 3.2 | 16.11 | 0.557x | 0.698x | 1.253x | 1.571x | 1.104x | 1.384x |
| nrev | 4.0 | 6.12 | 10.6 | 15.17 | 10.2 | 20.31 | 128.2 | 15.78 | 3.0 | 15.27 | 0.388x | 0.401x | 0.961x | 0.994x | 1.287x | 1.330x |
| nreverse | 3.7 | 6.08 | 11.3 | 12.86 | 12.1 | 11.65 | 132.2 | 15.97 | 3.1 | 14.94 | 0.380x | 0.407x | 0.805x | 0.861x | 0.729x | 0.780x |
| ops8 | 3.1 | 0.406 | 10.6 | 0.890 | 10.0 | 1.98 | 130.6 | 2.1 | 2.8 | 0.880 | 0.193x | 0.461x | 0.424x | 1.011x | 0.942x | 2.247x |
| poly_10 | 3.9 | 632.6 | 12.0 | 1,268 | 10.2 | 1,914 | 134.3 | 965.1 | 3.0 | 844.7 | 0.655x | 0.749x | 1.313x | 1.501x | 1.983x | 2.266x |
| prover | 4.6 | 16.51 | 11.4 | 25.42 | 10.6 | 27.48 | 142.4 | 26.79 | 3.0 | 22.96 | 0.616x | 0.719x | 0.949x | 1.107x | 1.026x | 1.197x |
| qsort | 4.0 | 11.03 | 11.0 | 21.18 | 9.0 | 26.83 | 124.4 | 17.75 | 3.0 | 16.70 | 0.621x | 0.661x | 1.193x | 1.269x | 1.512x | 1.607x |
| queens | 3.3 | 8,926 | 10.9 | 31,078 | 10.4 | 50,756 | 139.6 | 12,206 | 2.8 | 11,003 | 0.731x | 0.811x | 2.546x | 2.825x | 4.158x | 4.613x |
| queens_8 | 5.7 | 40.48 | 9.2 | 122.5 | 10.2 | 200.9 | 135.3 | 61.43 | 2.9 | 52.77 | 0.659x | 0.767x | 1.994x | 2.321x | 3.271x | 3.808x |
| queensn | 3.7 | 32,406 | 10.6 | 75,688 | 9.8 | 85,093 | 134.4 | 70,707 | 3.1 | 57,995 | 0.458x | 0.559x | 1.070x | 1.305x | 1.203x | 1.467x |
| query | 3.4 | 7.5 | 11.1 | 14.70 | 9.8 | 29.66 | 137.8 | 11.19 | 2.8 | 9.82 | 0.670x | 0.763x | 1.314x | 1.497x | 2.651x | 3.020x |
| sendmore | 4.3 | 853.3 | 10.5 | 2,141 | 10.3 | 4,641 | 133.1 | 910.7 | 3.1 | 828.6 | 0.937x | 1.030x | 2.351x | 2.583x | 5.096x | 5.600x |
| serialise | 4.1 | 5.21 | 10.6 | 11.27 | 10.0 | 12.52 | 131.3 | 9.85 | 3.1 | 8.05 | 0.529x | 0.648x | 1.144x | 1.401x | 1.271x | 1.556x |
| sieve | 3.5 | 13,270 | 11.9 | 16,156 | 9.7 | 14,130 | 143.4 | 10,375,612 | 13.7 | 10,375,369 | 0.001x | 0.001x | 0.002x | 0.002x | 0.001x | 0.001x |
| tak | 5.8 | 1,348 | 13.2 | 4,723 | 11.3 | 5,297 | 133.4 | 3,673 | 3.6 | 3,508 | 0.367x | 0.384x | 1.286x | 1.346x | 1.442x | 1.510x |
| times10 | 3.6 | 0.512 | 10.6 | 1.07 | 13.0 | 2.12 | 143.2 | 2.28 | 3.0 | 0.954 | 0.225x | 0.537x | 0.470x | 1.123x | 0.929x | 2.218x |
| unify | 4.5 | 112.8 | 15.5 | 168.0 | 12.7 | 75.43 | 2895.5 | 217.0 | 3.8 | 84.29 | 0.520x | 1.338x | 0.774x | 1.993x | 0.348x | 0.895x |
| zebra | 3.9 | 478.5 | 11.9 | 910.4 | 9.7 | 969.7 | 146.4 | 898.0 | 3.0 | 814.5 | 0.533x | 0.588x | 1.014x | 1.118x | 1.080x | 1.191x |

Geometric mean, runtime only, SCRIP mode 4 then mode 3, against each of the three: **SWI-Prolog** **1.370x** and 1.026x over 31 kernels (mode 4 ahead on 26, mode 3 on 19); the **GNU Prolog interpreter** **1.011x** and 0.757x over 31 (ahead on 24 and 18); the **GNU Prolog compiler** (`gplc`, the only native compiler in the grid) **0.488x** and 0.361x over 30 (mode 4 ahead on 2, sendmore and unify). One kernel runs pathologically slowly: `sieve`, at about a seven-hundredth of every rival in both modes. `unify` executes in 0.22 ms an iteration in mode 3 but takes 2.9 s to compile (`scrip -n` alone, the compile with no run, reads 2.56 s), the one start+compile figure far outside the rest.

### Raku

**Coverage** (third-party, every row in the suite table above): **Roast** — Raku's official specification test suite, the
shipped `.t` files graded whole against a local Rakudo; **RakRungs** is our own flat rungs suite with refs cut from Rakudo,
**RakBench** the kernel set graded as tests. Raku is IN DEVELOPMENT: the front-end accepts a deliberate working subset of
the language, most of Roast is outside it, and the parse-fail column is the roadmap.

**Benchmarks.** RakBench is the 84 kernels under `corpus/benchmarks/raku/`, from two public sources: Geoffrey Broadwell's `japhb/raku-bench` (Artistic 2.0) -- its 56 microbenchmark bodies (`mb-*`, each entry's own scale baked in) and 21 whole programs (the 8 `rc-` programs from Rosetta Code under the GFDL; the random ones seeded, the two JSON parsers given their data file) -- and the 7 dispatch snippets of Rakudo's own `tools/benchmark.pl` (`rakudo-tools-*`); each with its `.ref` cut from Rakudo 2026.05 and its declared stack and heap beside it, graded as tests in both modes by `../harness/scripts/test_raku_bench_suite.sh` (the suite table's RakBench row: 80 graded; 4 kept out of the denominator in `OUTSIDE.tsv` as too slow to time, 30-50 s a run -- the three `point_class_add` kernels, which pass as RakRungs entries, and `send-more-money-subs`). The timing instrument is the same runner's fixed-time twin (`../harness/scripts/bench_wrap_raku.py`, the kernel never edited): the kernel's output once, held to its `.ref`, then quiet repetitions until the budget is spent, each repetition's captured output held to the first; with `BENCH_ORACLE_ARM=1` the same generated twin runs under Rakudo, its `wall_us()` clock supplied by the benchmark prelude, so both engines are timed on the same program, on the same box, in the same minute. The rival is Rakudo, the reference implementation: Raku has no native compiler.

*Measured 2026-10-26 09:10-09:24 CDT on SCRIP `9bd097fa9` / corpus `a2cab70e9` / harness `317f987`, **the whole of SCRIP built `-O2`** (compiler and runtime, as in the SNOBOL4 section), load 0.7-1.1 on 16 cores (the ceo's single-core demo probes ran beside it from 09:12 to 09:17), **3 s of repetitions per point** -- Rakudo's JIT warms over the first part of a run, so a longer budget times its steady state (`BENCH_ORACLE_ARM=1 BENCH_ANGLES=time BENCH_BUD_MS=3000 bash ../harness/scripts/test_raku_bench_suite.sh`), Rakudo v2026.05, SCRIP with every diagnostic off (`SCRIP_DIAG=0`). 80 of 80 kernels printed their ref in both modes and under Rakudo. The per-repetition figure is the fixed-time twin's (microseconds per repetition of the kernel); the multiple is Rakudo time / SCRIP time on that same twin (above 1.00x SCRIP is ahead). A second pass at 1 s per point (09:00-09:06, the same build) is the cross-check: **†** marks the 7 kernels whose reading in some engine moved more than 25% between the two passes, **\*** the 3 whose slowest engine completed fewer than three repetitions in the budget. Two kernels carry no SCRIP multiple because the measurement showed SCRIP not doing the kernel's work, each a row: **‡** `divide-and-conquer` -- a nested sub declared a second time under one name binds to the first in SCRIP, so the twin's timed copy was never called, and the kernel prints 1 at every depth, which hid it; **§** `mb-deep_scan_for_interpolated_string_var` -- a bare `/regex/` against the topic answers True in SCRIP whatever the topic holds, so the scan it times is not the one Rakudo performs.* Every multiple is runtime only (the twin's clock reads sit at the first and last line of its timed loop). The start+compile columns come from the pristine program with a clock read at its first source line and one at its last (`bash ../harness/scripts/bench_stages.sh raku --reps 3`, harness `fabf5cb`, 2026-10-26 09:50 CDT), external elapsed minus that runtime, the median of 3 runs: Rakudo compiles every run as mode 3 does -- its start+compile reads 167-332 ms a run against mode 3's 3.5-20 ms, and the mode-4 binary starts in 2-5 ms.

| kernel | Rakudo start+compile ms | Rakudo µs/rep | m3 start+compile ms | m3 µs/rep | m4 start-up ms | m4 µs/rep | Rakudo / m3 | Rakudo / m4 |
|---|---:|---:|---:|---:|---:|---:|:---:|:---:|
| divide-and-conquer ‡ | 194.0 | 39,314 | 5.1 | — | 2.2 | — | — | — |
| insertion-sort | 198.7 | 7,207 | 5.4 | 1,693 | 2.2 | 1,698 | 4.256x | 4.245x |
| mb-any_equals | 173.2 | 46.21 | 4.5 | 13.91 | 2.1 | 13.71 | 3.323x | 3.371x |
| mb-array_set_xx | 170.9 | 16,612 | 4.3 | 824.6 | 2.4 | 818.0 | 20.147x | 20.307x |
| mb-charrange | 191.3 | 481.7 | 4.3 | 398.8 | 2.1 | 376.9 | 1.208x | 1.278x |
| mb-charrange_ignorecase | 188.7 | 499.7 | 4.5 | 399.0 | 2.2 | 393.6 | 1.252x | 1.269x |
| mb-create_and_copy_2d_grid_cross | 180.6 | 77,033 | 6.4 | 36,195 | 2.9 | 34,092 | 2.128x | 2.260x |
| mb-create_and_copy_2d_grid_cross_unpack | 187.3 | 152,662 | 6.5 | 35,772 | 3.0 | 33,319 | 4.268x | 4.582x |
| mb-create_and_iterate_hash_kv | 179.6 | 12.19 | 4.7 | 16.45 | 2.1 | 15.65 | 0.741x | 0.779x |
| mb-deep_scan_for_interpolated_string_var § | 183.1 | 1,623 | 4.1 | — | 2.1 | — | — | — |
| mb-for_array_set | 176.4 | 113.4 | 3.8 | 532.8 | 2.0 | 539.8 | 0.213x | 0.210x |
| mb-for_assign | 168.8 | 12.73 | 4.0 | 3 | 2.3 | 3 | 4.242x | 4.236x |
| mb-for_assign_native | 177.3 | 8.35 | 4.1 | 2.98 | 2.3 | 3.03 | 2.801x | 2.758x |
| mb-for_bind | 172.1 | 8.43 | 4.0 | 3.05 | 2.1 | 3.44 | 2.762x | 2.452x |
| mb-for_concat | 176.7 | 62.90 | 4.1 | 310.7 | 2.2 | 337.1 | 0.202x | 0.187x |
| mb-for_concat_2 | 180.5 | 87.28 | 4.3 | 867.6 | 2.1 | 860.6 | 0.101x | 0.101x |
| mb-for_concat_2_native | 171.8 | 77.10 | 4.2 | 913.7 | 2.3 | 907.1 | 0.084x | 0.085x |
| mb-for_concat_native | 176.0 | 40.24 | 4.1 | 308.3 | 1.9 | 340.0 | 0.131x | 0.118x |
| mb-for_empty | 172.9 | 7.22 | 3.7 | 2.25 | 2.2 | 2.06 | 3.201x | 3.498x |
| mb-for_hash_set † | 171.9 | 102.1 | 3.8 | 778.7 | 2.4 | 774.4 | 0.131x | 0.132x |
| mb-for_param_sigil | 178.2 | 112.0 | 4.3 | 638.0 | 2.0 | 559.2 | 0.176x | 0.200x |
| mb-for_postinc | 172.0 | 83.03 | 3.8 | 32.54 | 2.1 | 32.60 | 2.552x | 2.547x |
| mb-for_postinc_native | 179.0 | 8.61 | 4.2 | 4.13 | 2.2 | 3.9 | 2.084x | 2.207x |
| mb-for_push † | 170.7 | 18.75 | 3.9 | 96.54 | 2.1 | 77.06 | 0.194x | 0.243x |
| mb-hello | 167.4 | 3.95 | 3.9 | 0.441 | 2.1 | 0.445 | 8.954x | 8.879x |
| mb-loop_empty | 171.3 | 89.10 | 3.7 | 3.87 | 2.0 | 3.88 | 23.046x | 22.976x |
| mb-loop_empty_native | 180.5 | 6.07 | 3.5 | 3.85 | 1.8 | 3.86 | 1.575x | 1.572x |
| mb-loop_object_accessor | 181.8 | 16.04 | 4.4 | 1,713 | 2.1 | 1,643 | 0.009x | 0.010x |
| mb-postwhile_nil | 179.6 | 88.38 | 3.8 | 10.57 | 2.2 | 11.94 | 8.366x | 7.402x |
| mb-postwhile_nil_native | 173.4 | 5.04 | 3.8 | 10.71 | 2.2 | 11.79 | 0.471x | 0.427x |
| mb-rat_harmonic | 177.5 | 419.3 | 4.3 | 167.6 | 2.1 | 166.0 | 2.501x | 2.526x |
| mb-rat_mul_div_cancel | 172.5 | 403.9 | 4.9 | 207.4 | 2.0 | 205.3 | 1.948x | 1.967x |
| mb-reduce_int_comb_range | 168.9 | 291.8 | 4.9 | 350.9 | 2.1 | 319.7 | 0.832x | 0.913x |
| mb-reduce_range | 170.7 | 818.8 | 3.7 | 21.15 | 2.1 | 21.30 | 38.714x | 38.444x |
| mb-split_string_constant | 178.8 | 1,328 | 4.5 | 737.6 | 2.3 | 721.1 | 1.801x | 1.842x |
| mb-split_string_regex | 182.5 | 18,530 | 4.4 | 2,141 | 2.4 | 2,115 | 8.657x | 8.762x |
| mb-trim_string | 176.6 | 345.3 | 4.6 | 134.7 | 2.2 | 124.1 | 2.564x | 2.783x |
| mb-visit_2d_indices_cross | 172.8 | 28,200 | 5.5 | 15,334 | 2.6 | 12,979 | 1.839x | 2.173x |
| mb-visit_2d_indices_cross_unpack | 175.6 | 63,560 | 4.9 | 14,428 | 3.4 | 13,036 | 4.405x | 4.876x |
| mb-visit_2d_indices_for | 170.2 | 5.68 | 3.8 | 1.04 | 2.1 | 1.02 | 5.477x | 5.588x |
| mb-visit_2d_indices_loop | 172.0 | 5.98 | 3.9 | 1.1 | 2.2 | 1.07 | 5.436x | 5.568x |
| mb-visit_2d_indices_loop_native | 176.7 | 4.38 | 4.2 | 1.11 | 2.4 | 1.09 | 3.931x | 4.028x |
| mb-visit_2d_indices_while | 171.8 | 6.24 | 4.3 | 1.21 | 2.2 | 1.06 | 5.152x | 5.882x |
| mb-visit_2d_indices_while_native | 179.1 | 4.47 | 4.0 | 1.12 | 2.2 | 1.09 | 3.988x | 4.087x |
| mb-while_array_set | 175.6 | 19.21 | 4.1 | 12.94 | 2.4 | 12.69 | 1.485x | 1.515x |
| mb-while_bind † | 170.5 | 30.83 | 4.0 | 4.43 | 2.2 | 4.34 | 6.964x | 7.099x |
| mb-while_concat | 171.0 | 142.8 | 4.2 | 313.5 | 2.1 | 300.2 | 0.456x | 0.476x |
| mb-while_concat_native | 170.9 | 39.53 | 4.4 | 312.1 | 2.2 | 298.3 | 0.127x | 0.133x |
| mb-while_empty | 172.3 | 85.63 | 3.7 | 3.64 | 2.0 | 3.4 | 23.503x | 25.214x |
| mb-while_empty_native | 173.5 | 5.65 | 3.6 | 3.69 | 2.1 | 3.44 | 1.531x | 1.642x |
| mb-while_hash_set | 173.0 | 19.90 | 4.0 | 99.44 | 2.1 | 97.26 | 0.200x | 0.205x |
| mb-while_int2str † | 173.0 | 61.33 | 4.6 | 342.4 | 2.0 | 277.6 | 0.179x | 0.221x |
| mb-while_int2str_concat | 171.8 | 309.8 | 4.1 | 596.5 | 2.3 | 607.3 | 0.519x | 0.510x |
| mb-while_int2str_concat_native | 169.1 | 327.6 | 4.1 | 641.9 | 2.3 | 646.1 | 0.510x | 0.507x |
| mb-while_int2str_native † | 174.0 | 21.32 | 4.0 | 283.3 | 2.3 | 280.1 | 0.075x | 0.076x |
| mb-while_push | 171.9 | 11.57 | 4.3 | 99.40 | 2.3 | 85.54 | 0.116x | 0.135x |
| mb-while_push_join | 176.7 | 17.18 | 4.3 | 100.9 | 2.3 | 80.32 | 0.170x | 0.214x |
| mb-while_pushme | 173.4 | 3.88 | 4.2 | 3.56 | 2.2 | 3.21 | 1.089x | 1.206x |
| merge-sort † | 214.0 | 6,088 | 7.5 | 7,508 | 2.4 | 5,915 | 0.811x | 1.029x |
| parse-json * | 331.8 | 468,710 | 19.5 | 1,533,181 | 4.8 | 1,496,802 | 0.306x | 0.313x |
| parse-json-no-obj-creation | 256.6 | 347,638 | 7.6 | 1,218,221 | 4.4 | 1,212,112 | 0.285x | 0.287x |
| pi-sequential-iteration * | 184.7 | 5,574,176 | 5.4 | 1,239,100 | 2.7 | 1,306,405 | 4.499x | 4.267x |
| rakudo-tools-01-hello-world | 167.6 | 3.83 | 3.7 | 0.459 | 2.1 | 0.442 | 8.349x | 8.667x |
| rakudo-tools-02-10-000-sub-dispatches | 174.0 | 30.11 | 4.5 | 808.2 | 2.2 | 858.6 | 0.037x | 0.035x |
| rakudo-tools-03-10-000-multi-dispatches | 180.2 | 29.50 | 4.4 | 4,966 | 2.2 | 4,855 | 0.006x | 0.006x |
| rakudo-tools-04-10-000-method-dispatches † | 179.6 | 576.7 | 4.6 | 3,184 | 2.2 | 2,844 | 0.181x | 0.203x |
| rakudo-tools-05-10-000-multi-method-dispatches | 182.3 | 593.8 | 4.6 | 17,623 | 2.6 | 15,876 | 0.034x | 0.037x |
| rakudo-tools-06-10-000-operator-dispatches | 173.4 | 143.2 | 4.3 | 21.11 | 2.2 | 19.07 | 6.781x | 7.507x |
| rakudo-tools-07-postfix-10-000-times | 172.4 | 821.5 | 4.0 | 316.5 | 2.3 | 312.0 | 2.596x | 2.633x |
| rc-9-billion-names | 200.5 | 70.74 | 5.6 | 288.0 | 2.1 | 241.3 | 0.246x | 0.293x |
| rc-dragon-curve | 197.3 | 317.5 | 6.3 | 530.1 | 2.2 | 502.3 | 0.599x | 0.632x |
| rc-forest-fire | 280.7 | 35,142 | 15.9 | 14,181 | 2.7 | 13,463 | 2.478x | 2.610x |
| rc-forest-fire-stringify | 222.7 | 6,337 | 8.0 | 2,305 | 2.1 | 2,253 | 2.750x | 2.813x |
| rc-man-or-boy-test | 188.1 | 284.8 | 5.5 | 514.1 | 2.4 | 497.4 | 0.554x | 0.573x |
| rc-mandelbrot | 211.8 | 3,727 | 6.5 | 2,694 | 2.2 | 2,439 | 1.384x | 1.528x |
| rc-perfect-shuffle | 184.8 | 538.1 | 5.0 | 107.8 | 2.1 | 103.7 | 4.991x | 5.189x |
| rc-self-describing-numbers | 190.0 | 30,825 | 6.0 | 33,855 | 2.6 | 31,038 | 0.910x | 0.993x |
| send-more-money-loops | 231.6 | 828,526 | 8.9 | 197,332 | 2.7 | 210,549 | 4.199x | 3.935x |
| spinner * | 180.5 | 191,226 | 6.1 | 7,121,875 | 2.6 | 3,694,241 | 0.027x | 0.052x |
| string-escape | 178.3 | 110.6 | 4.4 | 28.60 | 2.1 | 28.37 | 3.865x | 3.898x |

Geometric mean over the 78 kernels timed in both engines, runtime only: **1.086x** in mode 4, **1.036x** in mode 3 (SCRIP ahead on 46 and 45); over the 71 whose readings held between the two passes, **1.211x** and **1.163x**. SCRIP is far ahead on loops, integer ranges and reductions, and array stores (`mb-reduce_range` 38x, `mb-while_empty` 25x, `mb-loop_empty` 23x, `mb-array_set_xx` 20x in mode 4) and far behind on dispatch -- Rakudo's multi-dispatch snippet 0.006x, an object accessor in a loop 0.010x, sub dispatch 0.035x, multi-method dispatch 0.037x -- and on string concatenation, integer-to-string conversion and `push` (0.08-0.5x in mode 4).

### Pascal

**P4 self-host** (`../harness/scripts/test_pascal_p4_selfhost.sh`, the Zurich Pascal-P4 compiler vendored with its
patch record at `corpus/packages/pascal/p4/PATCHES.md`): the SCRIP-built P4 compiler compiles its own
detabbed source with **zero errors** (generation 1: rc 0, 4018 listing lines, 18911 lines of P-code),
each program's WORK time printed as wall seconds and RSS from `/usr/bin/time` with the tree and mode;
the SCRIP-built P4 interpreter compiles and runs the generation-1 P-code on the same source (generation
2) and today stops on heap exhaustion (its byte-addressed store is an array of records whose every
field store mints a fresh pinned block) -- the verdict line reads `SELFHOST BLOCKED` until the Pascal
heap rides a movable root (row pascal-heap-records-are-reached-through-a-movable-root-not-an-immortal-block,
CEO-648). The oracle cannot grade P4 itself: fpc -Miso refuses its pre-ISO text (ord of a pointer,
mark/release) and default fpc reserves `operator`, so the fpc-built rival for timing carries its own
patch list, not yet written. (coo, 2026-09-12)

**Coverage** (third-party, every row in the suite table above): **FPC** — the Free Pascal compiler's own test suite,
vendored (181 programs); **PAT** — the ISO 7185 conformance suite, 427 tests, Pascal-P5's own transcripts as the record of
what each test requires; **PasRungs** is our own flat rungs suite and **PasBench** the classic Pascal kernels graded as tests.
The oracle everywhere is `fpc -Miso`, with ISO 7185 the authority where FPC is looser than the standard: a rejection test
passes when SCRIP refuses the program with the ISO diagnostic, and a test the standard does not actually reject is named
outside the ISO baseline with its measurement and kept in the denominator.

**Benchmarks.** PasBench is the 11 programs under `corpus/benchmarks/pascal/`, each with its `.ref` cut from `fpc -Miso` and its declared stack and heap beside it (`<k>.heap`, `<k>.stack`), graded as tests in both modes under three angles by `../harness/scripts/test_pascal_bench_suite.sh` (the suite table's PasBench row). The timing instrument is the three-angle cross-proof `../harness/scripts/bench_triangulate_pascal.sh` over the 8 kernels that read their repeat count from stdin -- the six Hennessy/Stanford programs (`bubble`, `intmm`, `perm`, `queens`, `quick`, `towers`), `sieve` and Whetstone (`whet`); `fbench`, `uplevel2` and `uplevel3` bake a fixed workload into their source and are named in `EXCLUDED.tsv` until the harness repeats them at the process level. Angle 1 (`test_bench_pascal_timed.sh`) finds an anchor repeat count by a live doubling search to a 150 ms budget and reports the WORK slope, microseconds per rep fitted over a x1/x4/x16 triple of counts (the fixed cost inside the program, start-up and compile included, cancelled exactly; the two segment slopes must agree within 10% or the cell reads NONLINEAR); angle 2 (`bench_pascal_fixed_iter.sh`) fits the same slope from the committed counts of `SCALE.tsv`, one column for `fpc` and one for SCRIP because the two differ by one to two orders of magnitude; the reps=1 output is held to the `.ref` before any timing; every SCRIP run carries the kernel's declared `-d`/`-s` switches; a kernel is cited on an engine only when the two angles agree within 10%. The rival is Free Pascal 3.2.2 at `fpc -O2`, its released default.

*Measured 2026-10-25 20:46-21:10 CDT on SCRIP `9bd097fa9` / corpus `a2cab70e9`, **the whole of SCRIP built `-O2`** (compiler and runtime, as in the SNOBOL4 section), the fleet idle (load 0.7-1.8 on 16 cores), SCRIP with every diagnostic off (`SCRIP_DIAG=0`); `quick` re-measured 2026-10-26 09:06-09:09 CDT on the same build (load 2.2-2.3), after its SCRIP repeat count was freed from a cap of 12 that an old crash at 13 repetitions had set (it now prints `fpc`'s answer at 12 to 5,000 repetitions in both modes) and that had left both SCRIP cells unmeasured. The records are `corpus/benchmarks/pascal/triangulation-20261026T014608Z.tsv` and `triangulation-20261026T140651Z.tsv`. The figure is angle 1's WORK slope in microseconds per rep of the kernel's own outer loop; the multiple is `fpc` time / SCRIP time on that slope (above 1.00x SCRIP is ahead). Both angles agree within 10% on every kernel in all three engines.* Every multiple is runtime only: the WORK slope is fitted across repetition counts, so the fixed cost of a process -- start-up, and mode 3's compile -- cancels exactly (ISO Pascal has no clock a kernel could read at its first and last line). The m3 start+compile column is that fixed cost read directly, by the formula total - runtime: the external elapsed of one repetition (`tools/bench_rusage`, the median of 7 runs, 2026-10-26 09:56 CDT) minus one repetition's WORK; the start-up columns are the same measurement for the `fpc` binary (0.28-0.74 ms, built with `{$mode objfpc}` as the timing runner builds it) and the mode-4 binary (1.5-1.9 ms).

| kernel | `fpc` start-up ms | `fpc -O2` µs/rep | m3 start+compile ms | m3 µs/rep | m4 start-up ms | m4 µs/rep | `fpc` / m3 | `fpc` / m4 |
|---|---:|---:|---:|---:|---:|---:|:---:|:---:|
| bubble | 0.43 | 82.1 | 15.4 | 3,701 | 1.9 | 3,934 | 0.022x | 0.021x |
| intmm | 0.42 | 83.4 | 14.5 | 874.0 | 1.8 | 790.2 | 0.095x | 0.105x |
| perm | 0.41 | 84.2 | 15.3 | 2,082 | 1.5 | 2,054 | 0.040x | 0.041x |
| queens | 0.36 | 0.906 | 16.6 | 18.9 | 1.7 | 18.4 | 0.048x | 0.049x |
| quick | 0.42 | 6.46 | 15.1 | 57.2 | 1.5 | 56.1 | 0.113x | 0.115x |
| sieve | 0.28 | 14.8 | 13.8 | 274.9 | 1.5 | 273.0 | 0.054x | 0.054x |
| towers | 0.48 | 202.3 | 13.7 | 2,703 | 1.7 | 2,837 | 0.075x | 0.071x |
| whet | 0.74 | 2,014 | 29.4 | 6,644 | 1.6 | 6,653 | 0.303x | 0.303x |

Geometric mean against `fpc -O2` over the 8 kernels, runtime only: **0.070x** in mode 4 and **0.070x** in mode 3 (SCRIP ahead on none) -- SCRIP's Pascal runs at about a fourteenth of the speed of Free Pascal's native code, from a forty-fifth on `bubble` to about a third on the floating-point `whet`. The 2026-09-26 grid that stood here, measured at `-O0`, read 0.0016x. Pascal's front end is due for a redesign (`ARCH-PASCAL-REDESIGN.md`, a scoped symbol table with type descriptors in place of its name-keyed tables), and these numbers are measured before it.

### SCRIPtix — the hybrid `.md` document

The eighth language is the other seven together: a SCRIPtix file is a plain Markdown `.md` document with one fenced section per language, compiled into one program and run from a single `scrip --run` (see [§ Seven languages, one compiland](#seven-languages-one-compiland)). Its programs are the polyglot demos under `corpus/demos/scrip/`, each graded as a test in both modes against its ref on a sample input — the ScrDemo row of the suite table above. The programs, and what works today:

<!-- SCRIP-PROGRAMS:BEGIN generated by .github/scripts/util_readme_scrip_programs.py --write from the progress DB's newest scriptix-demos pass (SCRIP b854bd863, corpus 3fb0f7045, measured 2026-10-02) -- do not edit by hand -->

**11 of 13 programs pass in both modes** against their refs on their sample input (`bash ../harness/scripts/test_demos_suite.sh scriptix`):

| Program | What it does | Languages in one file | mode 3 | mode 4 |
|:---|:---|:---|:---:|:---:|
| `demo01/hello.md` | Hello World | SNOBOL4 · Icon · Prolog | ✅ PASS | ✅ PASS |
| `demo02/wordcount.md` | Word Count | SNOBOL4 · Icon · Prolog | ✅ PASS | ✅ PASS |
| `demo03/roman.md` | Roman Numerals | SNOBOL4 · Icon · Prolog | ✅ PASS | ✅ PASS |
| `demo04/palindrome.md` | Palindrome | SNOBOL4 · Icon · Prolog | ✅ PASS | ✅ PASS |
| `demo05/fib.md` | Fibonacci first 10 | SNOBOL4 · Icon · Prolog | ✅ PASS | ✅ PASS |
| `demo06/sieve.md` | Sieve of Eratosthenes, primes to 50 | SNOBOL4 · Icon · Prolog | ✅ PASS | ✅ PASS |
| `demo07/caesar.md` | ROT13 cipher | SNOBOL4 · Icon · Prolog | ✅ PASS | ✅ PASS |
| `demo08/sort.md` | Sort 8 integers | SNOBOL4 · Icon · Prolog | ✅ PASS | ✅ PASS |
| `demo09/rpn.md` | RPN calculator | SNOBOL4 · Icon · Prolog | ✅ PASS | ✅ PASS |
| `demo10/anagram.md` | Detect anagrams | SNOBOL4 · Icon · Prolog | ✅ PASS | ✅ PASS |
| `infinite_snobol4/inf_eval.sno` | — | SNOBOL4 | 💥 CRASH | 💥 CRASH |
| `infinite_snobol4/inf_snobol4.icn` | — | — | ✅ PASS | ✅ PASS |
| `infinite_snobol4/infinite_snobol4.md` | — | SNOBOL4 · Icon | 💥 CRASH | 💥 CRASH |
<!-- SCRIP-PROGRAMS:END -->

### Snocone

Snocone is Koenig's structured SNOBOL4 (Bell Labs TR 124). No independent implementation ships a test suite, so its programs are graded through the SPITBOL oracle by construction, as SNOBOL4 programs are; the paper's own examples are in the corpus, and the self-hosting front-end's own beautifier is part of its rungs suite. Its suite and benchmark readings are in the suite table above.

### Rebus

Rebus has no independent implementation, so there is no third-party suite to grade against and no rival to time against; its programs are graded through the SNOBOL4 oracle path by construction. Its suite and benchmark readings are in the suite table above.

## Credits

Built by Lon Jones Cherryholmes, working with Claude — Sonnet, Opus, and Fable. Part
of the [snobol4ever](https://github.com/snobol4ever) organization; the org README has
the full story of how a rediscovered 1980 idea (Byrd boxes) became a seven-language
compiler.

## License

AGPL v3. See [LICENSE](LICENSE).

---

*snobol4all. snobol4now. snobol4ever.*
