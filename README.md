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
SNOBOL4, reproduces itself byte-for-byte when run through the compiler, in both modes.

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

| kernel | SPITBOL µs/rep | m3 µs/rep | m4 µs/rep | SPITBOL / m3 | SPITBOL / m4 |
|---|---:|---:|---:|:---:|:---:|
| arith_loop | 24.7 | 6.10 | 6.38 | 4.043x | 3.869x |
| arith_loop_twin | 7,064 | 2,006 | 1,946 | 3.521x | 3.630x |
| array_sum | 582.8 | 200.0 | 191.4 | 2.914x | 3.045x |
| eval_fixed | 137.8 | 55.1 | 54.5 | 2.501x | 2.531x |
| fib_recur | 3,750 | 1,789 | 1,854 | 2.096x | 2.023x |
| fibonacci | 213.7 | 105.4 | 110.1 | 2.028x | 1.942x |
| func_call | 34.2 | 13.4 | 14.1 | 2.558x | 2.420x |
| ident_call1 | 29.4 | 10.7 | 9.99 | 2.750x | 2.943x |
| ident_call2 | 32.5 | 9.60 | 8.96 | 3.387x | 3.627x |
| indirect_dispatch | 26.7 | 18.1 | 19.5 | 1.470x | 1.368x |
| mixed_workload | 31.3 | 22.7 | 21.8 | 1.377x | 1.437x |
| name_indirection | 1,143 | 445.6 | 452.2 | 2.566x | 2.529x |
| op_dispatch | 51.7 | 14.7 | 13.5 | 3.520x | 3.826x |
| pattern_bt | 224.5 | 50.8 | 52.7 | 4.419x | 4.264x |
| roman | 161.6 | 148.6 | 147.2 | 1.087x | 1.097x |
| string_concat | 47.4 | 133.0 | 132.8 | 0.357x | 0.357x |
| string_concat_twin | 202.5 | 531.5 | 531.3 | 0.381x | 0.381x |
| string_manip | 56.3 | 36.1 | 35.9 | 1.559x | 1.567x |
| string_pattern | 66.6 | 39.9 | 37.9 | 1.670x | 1.756x |
| table_access | 769.2 | 343.1 | 341.0 | 2.241x | 2.255x |
| table_variety | 590.7 | 246.4 | 241.6 | 2.397x | 2.445x |
| test_icon | 1.06 | 0.229 | 0.215 | 4.638x | 4.936x |
| var_access | 63.0 | 14.6 | 14.1 | 4.319x | 4.478x |

Geometric mean over the 23 kernels: **2.129x** in mode 3, **2.151x** in mode 4; SCRIP ahead of SPITBOL on 21 of 23 kernels in mode 3 and 21 in mode 4. The two kernels behind it are string concatenation (`string_concat` 0.357x, `string_concat_twin` 0.381x, both modes).

### Icon

**Coverage** (third-party, every row in the suite table above): **Zona** — Arizona's own test set, shipped with the
reference implementation; **Jcon** — the tests of Proebsting & Townsend's Icon-to-JVM compiler; **IPL** — the Icon Program
Library's programs, each with a ref cut from Arizona `icont`/`iconx` 9.5; **IcnRungs** is our own flat rungs suite and
**IcnBench** the classic benchmark set graded as tests. The oracle everywhere is Arizona `iconx`; a vendored source is
converted to SCRIP's semicolon-required Icon on the way in and checked in converted. Their boards, and how far the JCON
self-host gets, follow the benchmark grid below.

**Benchmarks.** IcnBench is every Icon program under `corpus/benchmarks/icon` with a `procedure main` -- the ten micro-kernels (`bench_icn*`), the six classic programs (`concord`, `deal`, `geddump`, `ipxref`, `queens`, `rsg`), the four `rtx/` kernels and the five `shootout/` programs, plus `tgrlink` -- each a standalone program with its `.ref` cut from Arizona `icont`/`iconx` 9.5.25a and its own `.argv`, input and declared arena beside it. The three-angle harness `../harness/scripts/bench_triangulate_icon.sh` runs each kernel three ways in `iconx`, m3 and m4 -- the pristine program under `tools/bench_rusage`, a generated fixed-iteration twin and a generated fixed-time twin (`../harness/scripts/util_icon_bench_wrap.py`, the kernel never edited; the loop shape is the oracle's: a kernel whose state carries across calls under `iconx` is iterated one process per iteration, `PROC`) -- and a kernel is cited only when all three engines print the ref on every angle.

*Measured 2026-10-25 19:45-19:53 CDT on SCRIP `9bd097fa9` / corpus `a2cab70e9` / harness `9e1deda`, **the whole of SCRIP built `-O2`** (compiler and runtime, as in the SNOBOL4 section; the harness's banner line still prints a literal `RT_OPT=-O0`), fleet idle, load 0.9-1.6 on 16 cores, **1 s of repetitions per fixed-time point, three iterations per fixed-iteration point** (`BUDGET_MS=1000 BENCH_N_DEFAULT=3`), oracle Arizona `iconx` 9.5.25a, SCRIP with every diagnostic off (`SCRIP_DIAG=0`). Every kernel printed its ref on every run of all three angles in every engine. The per-iteration figure is the fixed-time twin's (microseconds per iteration of the kernel's own `main`); the multiple is `iconx` time / SCRIP time on that same twin (above 1.00x SCRIP is ahead). The angles column names any engine whose two timed angles did not agree within 15% (`n/a`: too few iterations to compare); `iconx` reports no in-process work for `ipxref` and `queens`, so those rows carry no multiple.*

| kernel | loop | `iconx` µs/it | m3 µs/it | m4 µs/it | `iconx` / m3 | `iconx` / m4 | angles |
|---|---|---:|---:|---:|:---:|:---:|---|
| int_loop | IN | 71,429 | 8,615 | 8,281 | 8.291x | 8.626x | AGREE |
| int_mod_isolate | IN | 70,000 | 11,222 | 10,381 | 6.238x | 6.743x | AGREE |
| str_concat_dispatch | IN | 73,571 | 10,255 | 10,286 | 7.174x | 7.153x | AGREE |
| str_concat_int_dispatch | IN | 102,000 | 35,379 | 35,483 | 2.883x | 2.875x | AGREE |
| str_concat_intvar | IN | 90,000 | 33,258 | 34,167 | 2.706x | 2.634x | AGREE |
| str_concat_strvar | IN | 75,714 | 30,394 | 30,939 | 2.491x | 2.447x | AGREE |
| str_concat_table | IN | 202,000 | 34,433 | 35,821 | 5.866x | 5.639x | AGREE |
| sub_list_dispatch | IN | 117,778 | 27,027 | 24,512 | 4.358x | 4.805x | AGREE |
| sub_table_miss_dispatch | IN | 90,000 | 28,306 | 28,083 | 3.180x | 3.205x | AGREE |
| sub_table_miss_semantics | IN | 1,818 | 909.1 | 907.4 | 2.000x | 2.004x | iconx n/a, m3 DISAGREE, m4 DISAGREE |
| concord | IN | 22,954 | 16,210 | 13,808 | 1.416x | 1.662x | AGREE |
| deal | IN | 28.8 | 28.3 | 12.3 | 1.018x | 2.341x | iconx n/a, m3 n/a, m4 n/a |
| geddump | PROC | 99,091 | 111,556 | 89,333 | 0.888x | 1.109x | AGREE |
| ipxref | PROC | 0.00 | 1,438 | 1,085 | — | — | iconx n/a, m3 DISAGREE |
| queens | PROC | 0.00 | 100.0 | 80.0 | — | — | iconx n/a, m3 DISAGREE, m4 n/a |
| rsg | PROC | 27,297 | 11,292 | 6,067 | 2.417x | 4.500x | iconx DISAGREE |
| agg_field_isolate | IN | 695,000 | 164,000 | 154,000 | 4.238x | 4.513x | AGREE |
| num_isolate | IN | 2,160,000 | 514,500 | 493,000 | 4.198x | 4.381x | AGREE |
| num_mixed | IN | 565,000 | 183,500 | 185,833 | 3.079x | 3.040x | AGREE |
| rel_isolate | IN | 1,320,000 | 519,000 | 494,000 | 2.543x | 2.672x | AGREE |
| fannkuch | IN | 6,757 | 3,030 | 3,232 | 2.230x | 2.090x | AGREE |
| mandelbrot | IN | 27,297 | 3,918 | 3,735 | 6.967x | 7.308x | AGREE |
| pidigits | IN | 59.7 | 55.2 | 51.2 | 1.082x | 1.166x | iconx n/a, m3 DISAGREE, m4 n/a |
| reverse-complement | IN | 27.7 | 35.6 | 20.2 | 0.778x | 1.371x | iconx n/a, m3 n/a, m4 n/a |
| spectral-norm | IN | 100,000 | 34,067 | 33,667 | 2.935x | 2.970x | AGREE |
| tgrlink | IN | 147,143 | 98,636 | 56,944 | 1.492x | 2.584x | AGREE |

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

*Measured 2026-10-25 19:55-20:09 CDT on SCRIP `9bd097fa9` / corpus `a2cab70e9` / harness `9e1deda`, **the whole of SCRIP built `-O2`** (compiler and runtime, as in the SNOBOL4 section), the fleet idle (load 0.8-2.5 on 16 cores; the placement watch's hourly census ran on one or two cores during part of the pass), **1 s of iterations per point** (`TIME_BUDGET_MS=1000`), SCRIP with every diagnostic off (`SCRIP_DIAG=0`), GNU Prolog 1.6.0 and SWI-Prolog 9.0.4. The per-iteration figure is microseconds per iteration of the kernel's `bench_work`; the multiple is the rival's time / SCRIP's time (above 1.00x SCRIP is ahead). 31 of the 42 kernels are timed: the runner times a kernel only when GNU Prolog answers it single-shot, so the 11 kernels whose oracle is SWI-Prolog (chat_parser, det, flatten, moded_path, nand, perfect, pingpong, queens_clpfd, reducer, simple_analyzer, witness_depth_nrev8) carry no row here, and `gplc` refuses `eval` (a dark cell).*

| kernel | `gplc` µs/it | `gprolog` µs/it | `swipl` µs/it | m3 µs/it | m4 µs/it | `gplc` / m3 | `gplc` / m4 | `gprolog` / m3 | `gprolog` / m4 | `swipl` / m3 | `swipl` / m4 |
|---|---:|---:|---:|---:|---:|:---:|:---:|:---:|:---:|:---:|:---:|
| boyer | 10,227 | 19,010 | 18,837 | 24,957 | 17,132 | 0.410x | 0.597x | 0.762x | 1.110x | 0.755x | 1.099x |
| browse | 12,260 | 25,696 | 16,361 | 18,488 | 15,097 | 0.663x | 0.812x | 1.390x | 1.702x | 0.885x | 1.084x |
| cal | 0.152 | 0.430 | 1.79 | 1.41 | 0.317 | 0.108x | 0.479x | 0.305x | 1.356x | 1.266x | 5.638x |
| crypt | 30.6 | 112.7 | 217.0 | 109.9 | 86.1 | 0.279x | 0.356x | 1.026x | 1.310x | 1.975x | 2.521x |
| derive | 0.969 | 2.35 | 3.39 | 4.59 | 2.64 | 0.211x | 0.366x | 0.513x | 0.889x | 0.739x | 1.282x |
| deriv | 1.53 | 1.99 | 4.94 | 6.02 | 5.31 | 0.255x | 0.289x | 0.332x | 0.376x | 0.821x | 0.930x |
| divide10 | 0.554 | 1.10 | 2.10 | 2.53 | 1.08 | 0.219x | 0.511x | 0.436x | 1.019x | 0.829x | 1.938x |
| eval | — | 67.5 | 130.3 | 270.3 | 256.5 | — | — | 0.250x | 0.263x | 0.482x | 0.508x |
| fast_mu | 15.9 | 32.7 | 35.0 | 30.7 | 24.1 | 0.517x | 0.659x | 1.067x | 1.359x | 1.141x | 1.454x |
| fib | 438.1 | 1,479 | 1,768 | 771.3 | 671.7 | 0.568x | 0.652x | 1.918x | 2.202x | 2.293x | 2.633x |
| ham | 27.8 | 61.4 | 57.5 | 42.5 | 34.7 | 0.653x | 0.800x | 1.444x | 1.770x | 1.351x | 1.656x |
| log10 | 0.253 | 0.510 | 1.54 | 1.77 | 0.537 | 0.143x | 0.471x | 0.287x | 0.949x | 0.870x | 2.874x |
| meta_qsort | 120.1 | 191.7 | 142.9 | 169.9 | 133.4 | 0.707x | 0.901x | 1.128x | 1.437x | 0.841x | 1.071x |
| mu | 11.3 | 25.4 | 22.4 | 22.1 | 16.2 | 0.510x | 0.697x | 1.151x | 1.575x | 1.013x | 1.386x |
| nreverse | 6.08 | 12.9 | 11.7 | 18.0 | 14.9 | 0.338x | 0.407x | 0.717x | 0.863x | 0.650x | 0.782x |
| nrev | 6.13 | 15.2 | 20.3 | 17.7 | 15.3 | 0.346x | 0.400x | 0.859x | 0.993x | 1.149x | 1.329x |
| ops8 | 0.407 | 0.892 | 1.99 | 2.22 | 0.880 | 0.183x | 0.462x | 0.401x | 1.013x | 0.894x | 2.256x |
| poly_10 | 633.2 | 1,279 | 1,923 | 1,095 | 845.2 | 0.578x | 0.749x | 1.168x | 1.513x | 1.757x | 2.276x |
| prover | 16.6 | 25.6 | 27.6 | 29.0 | 23.0 | 0.572x | 0.721x | 0.884x | 1.113x | 0.954x | 1.202x |
| qsort | 11.0 | 21.3 | 26.9 | 19.6 | 16.7 | 0.562x | 0.660x | 1.087x | 1.275x | 1.371x | 1.609x |
| queens_8 | 40.6 | 123.0 | 201.3 | 63.1 | 52.8 | 0.642x | 0.768x | 1.948x | 2.330x | 3.189x | 3.814x |
| queensn | 32,455 | 76,331 | 85,676 | 79,088 | 58,020 | 0.410x | 0.559x | 0.965x | 1.316x | 1.083x | 1.477x |
| queens | 8,934 | 31,235 | 50,901 | 12,747 | 11,009 | 0.701x | 0.812x | 2.450x | 2.837x | 3.993x | 4.624x |
| query | 7.50 | 14.7 | 29.8 | 11.7 | 9.82 | 0.640x | 0.764x | 1.258x | 1.501x | 2.544x | 3.034x |
| sendmore | 854.1 | 2,150 | 4,680 | 1,041 | 829.2 | 0.821x | 1.030x | 2.066x | 2.593x | 4.497x | 5.643x |
| serialise | 5.23 | 11.3 | 12.5 | 10.3 | 8.06 | 0.506x | 0.649x | 1.094x | 1.404x | 1.213x | 1.557x |
| sieve | 13,282 | 16,338 | 16,403 | 10,515,200 | 10,384,200 | 0.001x | 0.001x | 0.002x | 0.002x | 0.002x | 0.002x |
| tak | 1,353 | 4,773 | 5,340 | 4,193 | 3,510 | 0.323x | 0.385x | 1.138x | 1.360x | 1.273x | 1.521x |
| times10 | 0.512 | 1.08 | 2.13 | 2.41 | 0.954 | 0.212x | 0.537x | 0.447x | 1.131x | 0.882x | 2.230x |
| unify | 113.0 | 168.9 | 76.2 | 2,895,190 | 84.5 | 0.000x | 1.338x | 0.000x | 1.998x | 0.000x | 0.901x |
| zebra | 479.4 | 913.2 | 979.1 | 1,040 | 814.2 | 0.461x | 0.589x | 0.878x | 1.122x | 0.941x | 1.202x |

Geometric mean, SCRIP mode 4 then mode 3, against each of the three: **SWI-Prolog** **1.381x** and 0.697x over 31 kernels (mode 4 ahead on 26); the **GNU Prolog interpreter** **1.016x** and 0.512x over 31 (mode 4 ahead on 24); the **GNU Prolog compiler** (`gplc`, the only native compiler in the grid) **0.488x** and 0.241x over 30 (mode 4 ahead on 2: sendmore and unify). Two kernels answer right and run pathologically slowly in SCRIP: `sieve` at about a six-hundredth of every rival in both modes, and `unify` in mode 3 alone (about 3 s per iteration; mode 4 runs it at 0.901x SWI); both drag the mode-3 means.

### Raku

**Coverage** (third-party, every row in the suite table above): **Roast** — Raku's official specification test suite, the
shipped `.t` files graded whole against a local Rakudo; **RakRungs** is our own flat rungs suite with refs cut from Rakudo,
**RakBench** the kernel set graded as tests. Raku is IN DEVELOPMENT: the front-end accepts a deliberate working subset of
the language, most of Roast is outside it, and the parse-fail column is the roadmap.

**Benchmarks.** RakBench is every Raku benchmark the two public sources hold, 83 kernels under `corpus/benchmarks/raku/` since 2026-09-26: raku-bench's 21 whole programs (Geoffrey Broadwell's `japhb/raku-bench`, Artistic 2.0, the `rc-` programs from Rosetta Code under the GFDL; the two random ones seeded, the two JSON parsers given their data file), its 62 usable microbenchmark bodies with each entry's own scale baked in, and Rakudo's 7 `tools/benchmark.pl` dispatch snippets -- each with its `.ref` cut twice from Rakudo and its declared stack and heap beside it, graded as tests in both modes by `../harness/scripts/test_raku_bench_suite.sh` (the suite table's RakBench row). The census of 2026-09-26 (mode 3, one run each): 36 of 83 print their ref; the 47 that do not are seventeen parser and emitter gaps, each a row on hq_raku (a last statement without its semicolon, statement modifiers, native `int`/`str`, the C-style `loop`, the `X` operator, binding, `push` as a sub, grammars, closures over an outer lexical, Rat arithmetic and float printing among them). The timing instrument is the three-angle cross-proof `../harness/scripts/bench_triangulate_raku.sh`: angle 1 a live doubling search for the largest iteration count whose loop completes within a 3 s CPU budget (Rakudo's JIT is cold for the first hundreds of milliseconds, so a shorter budget times the wrong regime), angle 2 the committed count of `fixed-iter-n.tsv`, angle 3 the kernel's own self-measured WORK bracket under the external stopwatch (OVERHEAD = elapsed - work, Rakudo's 300 ms of start-up never charged to the engine); every rep byte-verified against the `.ref`, every SCRIP run under the kernel's declared `-d`/`-s` switches, a cell cited only when both timed angles agree within 10%. ⚠ Only the four kernels that carry a hand-placed bracket can be looped by this harness; the other 32 that run wait on the kernel-convention row (a pristine source with the loop generated around it, as every other language does). The rival is Rakudo v2022.12, the reference implementation: Raku has no native compiler on this box, so the bar is Rakudo.

*Measured 2026-09-26 13:11-13:16 CDT on SCRIP `0f416ecc0` / corpus `bd5ffd768`, **RT_OPT=-O0**, fleet quiet (load 0.7-1.2 on 16 cores), REPS=3 best-of by work, SCRIP with every diagnostic off (`SCRIP_DIAG=0`); the records are `corpus/benchmarks/raku/worktime-20260926T181134Z.tsv` (angle 3) and `triangulation-20260926T181134Z.tsv` (the cross-proof). WORK is the kernel's own bracket; the x-factor is Rakudo WORK / SCRIP WORK (1.5x is one and a half times faster, 0.5x half as fast); a row whose arms disagree or fail verification carries its readings and no multiple.*

| kernel | Rakudo WORK | SCRIP m3 WORK | SCRIP m4 WORK | Rakudo / m3 | Rakudo / m4 | cross-proof |
|---|---:|---:|---:|:---:|:---:|---|
| point_class_add | 606.5 ms | 6.92 s | 6.20 s | — | — | rakudo DISAGREE |
| point_class_add1 | 3.01 s | 7.17 s | 6.13 s | 0.42x | 0.49x | all three AGREE |
| send-more-money-loops | 748.8 ms | 107.8 ms | 134.7 ms | — | 5.56x | m3 DISAGREE |
| string-escape | 1.1 ms | 49 µs | 56 µs | 23.12x | 20.23x | all three AGREE |

Geometric mean against Rakudo: **3.808x** in mode 4 over the 3 kernels cited and **3.116x** in mode 3 over 2. The reading has not changed shape since the first WORK-basis grid: loop and integer work is well ahead of Rakudo (string-escape), object and method-heavy work well behind (the point_class family), and the untimed kernels are the row above, not a measurement.

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

*Measured 2026-10-25 20:46-21:10 CDT on SCRIP `9bd097fa9` / corpus `a2cab70e9`, **the whole of SCRIP built `-O2`** (compiler and runtime, as in the SNOBOL4 section), the fleet idle (load 0.7-1.8 on 16 cores), SCRIP with every diagnostic off (`SCRIP_DIAG=0`); `quick` re-measured 2026-10-26 09:06-09:09 CDT on the same build (load 2.2-2.3), after its SCRIP repeat count was freed from a cap of 12 that an old crash at 13 repetitions had set (it now prints `fpc`'s answer at 12 to 5,000 repetitions in both modes) and that had left both SCRIP cells unmeasured. The records are `corpus/benchmarks/pascal/triangulation-20261026T014608Z.tsv` and `triangulation-20261026T140651Z.tsv`. The figure is angle 1's WORK slope in microseconds per rep of the kernel's own outer loop; the multiple is `fpc` time / SCRIP time on that slope (above 1.00x SCRIP is ahead). Both angles agree within 10% on every kernel in all three engines.*

| kernel | `fpc -O2` µs/rep | m3 µs/rep | m4 µs/rep | `fpc` / m3 | `fpc` / m4 |
|---|---:|---:|---:|:---:|:---:|
| bubble | 82.1 | 3,701 | 3,934 | 0.022x | 0.021x |
| intmm | 83.4 | 874.0 | 790.2 | 0.095x | 0.105x |
| perm | 84.2 | 2,082 | 2,054 | 0.040x | 0.041x |
| queens | 0.906 | 18.9 | 18.4 | 0.048x | 0.049x |
| quick | 6.46 | 57.2 | 56.1 | 0.113x | 0.115x |
| sieve | 14.8 | 274.9 | 273.0 | 0.054x | 0.054x |
| towers | 202.3 | 2,703 | 2,837 | 0.075x | 0.071x |
| whet | 2,014 | 6,644 | 6,653 | 0.303x | 0.303x |

Geometric mean against `fpc -O2` over the 8 kernels: **0.070x** in mode 4 and **0.070x** in mode 3 (SCRIP ahead on none) -- SCRIP's Pascal runs at about a fourteenth of the speed of Free Pascal's native code, from a forty-fifth on `bubble` to about a third on the floating-point `whet`. The 2026-09-26 grid that stood here, measured at `-O0`, read 0.0016x. Pascal's front end is due for a redesign (`ARCH-PASCAL-REDESIGN.md`, a scoped symbol table with type descriptors in place of its name-keyed tables), and these numbers are measured before it.

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
