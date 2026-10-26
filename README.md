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
never typed by hand: the suite's latest reading, written by that suite's own runner in the landing that measured it, with the
day it was measured (the SCRIP tree and the runner of every reading are in that record). Passing / graded counts a program
only when it passes in BOTH modes. The seven rung suites are our own flat
suites with refs cut from each oracle; each Bench row is that language's benchmark programs graded as tests, a program passing when it prints its ref under all three angles (wrapper process, fixed iterations, fixed time limit); the others are vendored third-party suites. Raku is IN DEVELOPMENT: its rows stand as measured.

| Language | Suite | passing / graded | measured |
|:---|:---|---:|:---|
| SNOBOL4 | Gimpel | **144/144** | `2026-10-25` |
| SNOBOL4 | Budne | **70/70** | `2026-10-25` |
| SNOBOL4 | Flake | **126/126** | `2026-10-25` |
| SNOBOL4 | AIS | **8/8** | `2026-10-25` |
| SNOBOL4 | Dotnet | **6/6** | `2026-10-25` |
| SNOBOL4 | TPgm | **8/8** | `2026-10-25` |
| SNOBOL4 | X64T | **33/33** | `2026-10-25` |
| SNOBOL4 | X32T | **16/16** | `2026-10-25` |
| SNOBOL4 | SnoDemo | **24/24** | `2026-10-23` |
| SNOBOL4 | SnoRungs (rungs) | **2142/2142** | `2026-10-25` |
| SNOBOL4 | SnoBench | **23/23** | `2026-10-25` |
| Icon | Zona | **118/118** | `2026-10-25` |
| Icon | Jcon | **85/85** | `2026-10-25` |
| Icon | IPL | **644/644** | `2026-10-25` |
| Icon | IcnDemo | **7/7** | `2026-10-23` |
| Icon | IcnRungs (rungs) | **935/935** | `2026-10-25` |
| Icon | IcnBench | **26/26** | `2026-10-25` |
| Prolog | INRIA | **440/440** | `2026-10-25` |
| Prolog | SWI | **2342/2709** | `2026-10-24` |
| Prolog | GNU source | **43/45** | `2026-10-25` |
| Prolog | Logtalk | **3546/3546** | `2026-10-25` |
| Prolog | ProDemo | **8/8** | `2026-10-23` |
| Prolog | ProRosetta | **447/481** | `2026-10-25` |
| Prolog | ProPuzzles | **235/267** | `2026-10-25` |
| Prolog | TreallaTests | **272/327** | `2026-10-25` |
| Prolog | XsbTests | **170/550** | `2026-10-25` |
| Prolog | ProRungs (rungs) | **577/577** | `2026-10-25` |
| Prolog | ProBench | **42/42** | `2026-10-25` |
| Pascal | FPC | **94/94** | `2026-10-24` |
| Pascal | PAT | **400/400** | `2026-10-23` |
| Pascal | PasRosetta | **199/319** | `2026-10-25` |
| Pascal | PasDemo | **8/8** | `2026-10-24` |
| Pascal | PasRungs (rungs) | **252/252** | `2026-10-24` |
| Pascal | PasBench | **11/11** | `2026-10-24` |
| Raku — IN DEVELOPMENT | Roast | **156/1401** | `2026-10-25` |
| Raku — IN DEVELOPMENT | RakDemo | **0/1** | `2026-10-10` |
| Raku — IN DEVELOPMENT | RakRungs (rungs) | **1062/1062** | `2026-10-24` |
| Raku — IN DEVELOPMENT | RakBench | **80/80** | `2026-10-23` |
| SCRIPtix (hybrid) | TixDemo | **12/12** | `2026-10-23` |
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

**Benchmarks.** ProBench is the 23 kernels under `corpus/benchmarks/prolog/bench/` -- the classic van Roy / Aquarius set (tak, nreverse, qsort, deriv, ops8, times10, divide10, log10, queens, zebra, crypt, mu, query, sendmore, fib, ham, meta_qsort, ...) -- each a standalone program with its `.ref`, its declared stack and heap beside it (`<k>.heap`, `<k>.stack`: the shipped `-d128m -s4m` as a setting for 22 of them, 64 MB of stack for tak), graded as tests in both modes under three angles by `../harness/scripts/test_prolog_bench_suite.sh` (the suite table's ProBench row). The timing instrument is the three-angle cross-proof `../harness/scripts/bench_triangulate_prolog.sh`: angle 1 (`test_bench_prolog_timed.sh`) runs a live doubling search for the largest iteration count whose between-and-fail loop completes within the budget; angle 2 (`bench_prolog_fixed_iter.sh`) executes the committed count from `fixed-iter-n.tsv`; both run every SCRIP arm under the kernel's declared `-d`/`-s` switches, hold every engine's single-shot output to the `.ref` before timing it, and read the kernel's self-measured WORK per iteration inside the generated bracket; a kernel's reading on an engine is cited only when the two angles agree within 10% on that figure (the two-number basis: the process CPU rate, which the instrument compared until 2026-09-26, carried mode 3's in-process compile as a per-iteration cost that shrank with the count, and mode 3 disagreed with itself on 14 of 23 kernels while mode 4 agreed). The rivals are three arms of two systems, and only one of them is a compiler: **`gplc`**, GNU Prolog 1.4.5's native compiler (Prolog to WAM to its mini-assembly to x86-64, linked; `gplc --no-top-level`), **`gprolog`**, the same system's byte-code WAM interpreter (`gprolog --consult-file`: a consulted file is compiled to WAM byte-code and emulated in C, never native code), and **`swipl`**, SWI-Prolog 9.0.4, a bytecode virtual machine interpreted by C (the class SPITBOL belongs to); each runs the generated program with the per-engine prelude its clock needs, and the committed count for the rival arms is its own column of `fixed-iter-n.tsv` because `gplc` runs the fast kernels 20-80x faster than SCRIP mode 3 on a clock that ticks at 1 ms.

*Measured 2026-09-26 11:43-11:53 CDT on SCRIP `fc979cef9` / corpus `45fb64115`, **RT_OPT=-O0**, fleet quiet (load 0.6-1.1 on 16 cores), **500 ms of iterations per fixed-time point**, SCRIP with every diagnostic off (`SCRIP_DIAG=0`); the record is `corpus/benchmarks/prolog/triangulation-20260926T164302Z.tsv`, with witness_depth_nrev8 re-read at 11:53 after the harness learned that a `gplc` refusal darkens the `gplc` cell and not the kernel (`triangulation-20260926T165317Z.tsv`). The per-iteration figure is angle 1's self-measured work (microseconds per iteration of the kernel's `bench_work`, the between-and-fail loop around it, the write of the answer outside the bracket); the x-factor is rival time / SCRIP time on that same figure (1.5x is one and a half times faster, 0.5x half as fast). A row whose angles disagree on an engine carries its readings and no multiple against that engine.*

| kernel | GNU compiler (`gplc`) µs/it | GNU interpreter (`gprolog`) µs/it | SWI-Prolog (`swipl`) µs/it | SCRIP m3 µs/it | SCRIP m4 µs/it | GNU compiler / m3 | GNU compiler / m4 | GNU interpreter / m3 | GNU interpreter / m4 | SWI-Prolog / m3 | SWI-Prolog / m4 | angles |
|---|---:|---:|---:|---:|---:|:---:|:---:|:---:|:---:|:---:|:---:|---|
| cal | 0.15 | 0.40 | 1.83 | 2.75 | 1.60 | 0.05x | 0.09x | 0.15x | 0.25x | 0.67x | 1.15x | AGREE |
| crypt | 31.7 | 115 | 217 | 923 | 910 | 0.03x | 0.03x | 0.13x | 0.13x | 0.24x | 0.24x | AGREE |
| deriv | 1.49 | 1.91 | 5.06 | 113 | 110 | — | — | 0.02x | 0.02x | 0.04x | 0.05x | GNU compiler DISAGREE |
| derive | 1.49 | 2.31 | 3.45 | 44.7 | 41.5 | — | — | 0.05x | 0.06x | 0.08x | 0.08x | GNU compiler DISAGREE |
| divide10 | 0.50 | 1.10 | 2.19 | 20.0 | 18.5 | — | — | 0.05x | 0.06x | 0.11x | 0.12x | GNU compiler DISAGREE |
| fib | 445 | 1,499 | 2,002 | 3,670 | 3,888 | 0.12x | 0.11x | 0.41x | 0.39x | — | — | SWI-Prolog DISAGREE |
| ham | 22.8 | 60.8 | 58.6 | 862 | 857 | 0.03x | 0.03x | 0.07x | 0.07x | — | — | SWI-Prolog DISAGREE |
| log10 | 0.26 | 0.50 | 1.62 | 13.5 | 12.2 | — | — | 0.04x | 0.04x | 0.12x | 0.13x | GNU compiler DISAGREE |
| meta_qsort | 120 | 199 | 146 | 3,815 | 3,739 | 0.03x | 0.03x | 0.05x | 0.05x | 0.04x | 0.04x | AGREE |
| mu | 11.7 | 25.4 | 22.9 | 280 | 280 | 0.04x | 0.04x | 0.09x | 0.09x | 0.08x | 0.08x | AGREE |
| nrev | 5.78 | 14.5 | 12.5 | 235 | 239 | 0.02x | 0.02x | 0.06x | 0.06x | 0.05x | 0.05x | AGREE |
| nreverse | 5.86 | 12.3 | 11.5 | 191 | 190 | 0.03x | 0.03x | 0.06x | 0.06x | 0.06x | 0.06x | AGREE |
| ops8 | 0.38 | 0.88 | 1.96 | 12.3 | 10.9 | — | — | 0.07x | 0.08x | 0.16x | 0.18x | GNU compiler DISAGREE |
| qsort | 10.4 | 20.6 | 28.4 | 189 | 187 | 0.05x | 0.06x | 0.11x | 0.11x | 0.15x | 0.15x | AGREE |
| queens | 8,734 | 31,875 | 48,405 | 159,916 | 162,198 | 0.05x | 0.05x | 0.20x | 0.20x | 0.30x | 0.30x | AGREE |
| queens_8 | 36.4 | 125 | 182 | 668 | 688 | 0.05x | 0.05x | 0.19x | 0.18x | 0.27x | 0.26x | AGREE |
| queensn | 32,312 | 74,562 | 84,301 | 774,308 | 779,630 | 0.04x | 0.04x | 0.10x | 0.10x | 0.11x | 0.11x | AGREE |
| query | 7.37 | 13.0 | 30.3 | 278 | 274 | 0.03x | 0.03x | 0.05x | 0.05x | — | — | SWI-Prolog DISAGREE |
| sendmore | 816 | 2,133 | 4,662 | 3,702 | 3,845 | 0.22x | 0.21x | 0.58x | 0.55x | 1.26x | 1.21x | AGREE |
| tak | 1,120 | 4,672 | 5,080 | 10,562 | 10,326 | 0.11x | 0.11x | 0.44x | 0.45x | 0.48x | 0.49x | AGREE |
| times10 | 0.50 | 1.00 | 2.11 | 18.0 | 16.6 | — | — | 0.06x | 0.06x | 0.12x | 0.13x | GNU compiler DISAGREE |
| witness_depth_nrev8 | — | 1.13 | 2.77 | 18.2 | 18.2 | — | — | 0.06x | 0.06x | 0.15x | 0.15x | GNU compiler UNPROVEN |
| zebra | 469 | 914 | 937 | 9,579 | 9,524 | 0.05x | 0.05x | 0.10x | 0.10x | 0.10x | 0.10x | AGREE |

Geometric mean, SCRIP mode 4 then mode 3, against each of the three: the **GNU Prolog compiler** (`gplc`, the only native compiler in the grid) **0.05x** over the 16 kernels whose brackets both agree and **0.05x** over 16 (ahead on none); the **GNU Prolog interpreter** (`gprolog --consult-file`, its byte-code WAM) **0.10x** over 23 and **0.09x** over 23; **SWI-Prolog** (`swipl`, a bytecode VM) **0.15x** over 20 (ahead on 2: cal and sendmore) and **0.14x** over 20 (ahead on 1). Mode 3 and mode 4 read within a few percent of each other on every kernel: the in-process compile is outside the bracket and the code inside it is the same. **Why SCRIP is slower where it is slower, measured 2026-09-26 10:1x CDT on `b6d8c9bb8` (callgrind, exclusive share, mode 4 at -O0 on the fixed-iteration twins, `SCRIP_DIAG=0`; the compiler and runtime of `3d66e3e09` are that tree's -- the commits between are scripts and this README):** the kernels fall into two classes and both are runtime C reached per operation. **(1) The term-rewriting kernels consult the Prolog parser's operator tables at run time:** `bin_ops_count` 48%, `prolog_op_table_get` 13%, `prefix_ops_count` 9%, `find_user` 2%, `op_type_unclassify` 2% (all `src/parsers/prolog/prolog_parse.c`) and `plc_op_info` 6% with a `strcmp` per lookup 5% — **three quarters of deriv (0.05x)** — so writing or taking apart an operator term (`d(U*V,X,DU*V+U*DV)`, `=..`, `write/1`) re-derives every operator's type and priority by name on every node, where a WAM carries them on the functor; derive, times10, divide10, log10 and ops8 (0.09–0.19x) are the same shape. **(2) Unification, dereference and binding are C calls per cell:** `plw_unify_cells` (with its recursive twin), `plw_cell_deref_slow`, `plw_bind`, `plw_mkc_kids`, `plw_unify_vals`, `rt_pl_deref_val` and `rt_pl_dop_unify_ci_c` are 30–45% of nreverse (0.05x), queens_8 (0.30x), zebra (0.10x) and fib (0.51x): the get, put and unify steps a WAM emits inline as a handful of instructions each are entered as C functions with a descriptor walk per argument; beside them the trail — `pl_tr_needs_log`, `pl_tr_push` and `rt_pl_tr_gc_sync` (5–9%) — and the call prologue in C (`rt_proc_call_prologue_lex` 3–5%, `rt_icn_zframe_args_install` 3%), the collector (`gc_collect_ex` 3–5%) and the allocation of every structure cell (`rt_gcheap_alloc` 3%). **(3) Atoms are interned by name at run time:** `prolog_atom_intern` 5%, `ht_hash` 4% and a `strcmp` 6% of crypt (0.24x) — an atom the compiler already saw is looked up by its characters again when the program runs. Where none of this is on the path SCRIP is ahead: cal 1.17x and sendmore 1.21x against `swipl` in mode 4. tak, queens, queensn, witness_depth_nrev8 and ham carry multiples for the first time in this grid: the three-angle harness now runs every kernel under its own declared stack and heap (`<k>.heap`/`<k>.stack` beside it, carried as `-d`/`-s` switches) and has a committed count for every kernel, where until 2026-09-26 it ran tak at the shipped 4 MB stack and skipped it as a correctness failure. tak's declaration is 64 MB, the size it needs: tak(18,12,6) leaves a choice point on every one of its 63,609 calls that takes the first clause, and SCRIP completes it at 64 MB and overflows at 32 MB where `swipl` completes at 16 MB and overflows at 8 MB -- a retained choice point costs about four times `swipl`'s, the frame's carve in C that the rewrite rows above name. Two kinds of cell carry readings and no multiple against `gplc`: the six kernels `gplc` runs in under 1.5 µs an iteration (deriv, derive, divide10, log10, ops8, times10), whose `gplc` work per iteration moved 11-29% between the two counts the angles ran while repeating to 0.3% at one count -- the native binary's cost is not a constant of the kernel at these counts and the instrument says so; and witness_depth_nrev8, which defines `append/3`, a built-in `gplc` refuses to let a program redefine where `gprolog`'s consult only prints an error line and runs its own: the kernel is outside the `gplc` baseline. The three `swipl` cells without a multiple (fib, ham, query) moved 11-24% between `swipl`'s own two angles at counts that both hold half a second or more.

**Where the boxes hand their logic to C, counted** (`../harness/scripts/bench_prolog_call_census.sh`: the 23 kernels compiled to mode-4 asm on `a6d057a31`, emitted call sites into each runtime entry, summed). Byrd's four ports and Proebsting's compile-time wiring hold for conjunction, the clause step, disjunction, if-then-else, negation, catch/throw, `between/3`, `true`/`fail`/`repeat`, DCG and last call (a ten-million-deep tail call runs under a 512 KB stack); the logic inside the chunk is a call into C, and the census says where: the trail unwind at every β, ω and clause step 4,154 sites (`rt_pl_tr_unwind`); the activation frame's carve and release 9,881 (`rt_jmp_frame_lexprep2`, `rt_icn_zframe_args_install`, `rt_arg_stage`, `rt_proc_call_open_det`, `rt_proc_drop_frame_h`, `rt_nret_fix_tiny`); the runtime meta-call and its resume 2,573; head unification 1,163 (`rt_pl_dop_unify`, `_cs`, `_ci`); a body term built 924 (`rt_pl_dop_mkc`); cut, fence, choice and disjunction open 880; `is/2`, comparison and type tests 648; the dynamic database 585 (`rt_pl_dop_db_*`). The score against the two papers, in `.github/findings/FINDING-2026-09-26-ceo-prolog-byrd-box-score-where-the-boxes-run-the-logic-and-where-c-does.md`: 9 constructs meet, 3 meet in their wiring only, 9 do not meet, 1 (first-argument indexing) is beyond the papers; every one that does not meet is a rewrite row on hq_prolog whose criterion is this census reading zero.

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

**Benchmarks.** PasBench is the 11 programs under `corpus/benchmarks/pascal/`, each with its `.ref` cut from `fpc -Miso` and its declared stack and heap beside it (`<k>.heap`, `<k>.stack`), graded as tests in both modes under three angles by `../harness/scripts/test_pascal_bench_suite.sh` (the suite table's PasBench row). The timing instrument is the three-angle cross-proof `../harness/scripts/bench_triangulate_pascal.sh` over the 8 kernels that read their repeat count from stdin -- the six Hennessy/Stanford programs (`bubble`, `intmm`, `perm`, `queens`, `quick`, `towers`), `sieve` and Whetstone (`whet`); `fbench`, `uplevel2` and `uplevel3` bake a fixed workload into their source and are named in `EXCLUDED.tsv` until the harness repeats them at the process level. Angle 1 (`test_bench_pascal_timed.sh`) finds an anchor repeat count by a live doubling search to a 150 ms budget and reports the WORK slope, microseconds per rep fitted over a x1/x4/x16 triple of counts (the fixed cost inside the program cancelled exactly; the two segment slopes must agree within 10% or the cell reads NONLINEAR); angle 2 (`bench_pascal_fixed_iter.sh`) fits the same slope from the committed counts of `SCALE.tsv`, one column for `fpc` and one for SCRIP because the two differ by two to three orders of magnitude; the reps=1 output is held to the `.ref` before any timing; every SCRIP run carries the kernel's declared `-d`/`-s` switches; a kernel is cited on an engine only when the two angles agree within 10%. The rival is Free Pascal 3.2.2 (`fpc -O2`, its released default: the speed the public runs, and the one legitimate `-O2` on this box) against SCRIP at `RT_OPT=-O0`.

*Measured 2026-09-26 12:00-12:50 CDT on SCRIP `83b784400` / corpus `e65e530ed`, **RT_OPT=-O0**, fleet quiet (load 0.5-1.2 on 16 cores), SCRIP with every diagnostic off (`SCRIP_DIAG=0`); the record is `corpus/benchmarks/pascal/triangulation-20260926T170055Z.tsv`. The figure is angle 1's WORK slope in microseconds per rep of the kernel's own outer loop; the x-factor is `fpc` time / SCRIP time on that slope (1.5x is one and a half times faster, 0.5x half as fast); a row whose angles disagree carries its readings and no multiple.*

| kernel | `fpc -O2` µs/rep | m3 µs/rep | m4 µs/rep | `fpc` / m3 | `fpc` / m4 | angles |
|---|---:|---:|---:|:---:|:---:|---|
| bubble | 83.7 | 233,865 | 220,441 | 0.0004x | 0.0004x | AGREE |
| intmm | 87.1 | 60,753 | 57,956 | 0.0014x | 0.0015x | AGREE |
| perm | 87.4 | 104,283 | 99,553 | 0.0008x | 0.0009x | AGREE |
| queens | 0.94 | 1,702 | 1,633 | 0.0006x | 0.0006x | AGREE |
| quick | 5.86 | 5,580 | 5,222 | 0.0010x | 0.0011x | AGREE |
| sieve | 14.8 | 15,957 | 15,296 | 0.0009x | 0.0010x | AGREE |
| towers | 203 | 8,904 | 9,186 | 0.0228x | 0.0221x | AGREE |
| whet | 2,025 | 297,567 | 283,079 | 0.0068x | 0.0072x | AGREE |

Geometric mean against `fpc -O2`: **0.0016x** in mode 4 over the 8 kernels whose `fpc` and m4 slopes both agree (SCRIP ahead on 0), **0.0016x** in mode 3 over 8 (ahead on 0). **Why SCRIP is three orders of magnitude from `fpc`, measured the same sitting (callgrind, exclusive share of all instructions, mode 4 at -O0, `SCRIP_DIAG=0`, the kernels at 1-200 reps):** two runtime roads carry almost every instruction. **(1) An array element is fetched and stored by a builtin looked up by its name on every access:** the Pascal lowerer emits `a[i]` as a call to the runtime's `arr_get`, `a[i] := v` as `arr_set_pure` and `odd(i)` as `iand`, each through `rt_call_arr_bl` with the builtin's NAME as a string (37 such sites in queens' emitted asm), and the runtime resolves that name by comparing it against its builtin table with `strcmp` on every call: `__strcmp_avx2` is 46% of queens, 51% of bubble, 52% of sieve and 49% of whet, and the two dispatch functions `script_try_call_builtin_by_name` and `try_call_builtin_by_name_bl_s` another 33-37% -- 85-90% of every instruction these kernels execute is the lookup of a name the compiler already knew, before the element is touched at all, where `fpc` emits the indexed load. **(2) The procedure call is a C protocol:** towers (the recursive Hanoi mover, a call per move) spends 18.5% in `rt_icn_zframe_args_install`, 14.7% in `rt_proc_call_prologue_lex`, 13.9% in `rt_proc_call_open_det4`, 10.5% in `memset` clearing the frame, 5.6% in `rt_proc_call_epilogue_γ` and 2.9% in `rt_jmp_frame_lexprep2` -- two thirds of the kernel in the frame's carve, fill and release in C, the road the Prolog call census names (`rt_icn_zframe_args_install` and its five siblings at 9,881 sites there) and the Byrd-box rewrite rows exist for. Where neither road is on the path, the kernel's own emitted boxes are 1-6% each (towers' `n26_return_bx`, `n13_call_bx`, `n2_binop_test_bx`). Both roads are rows on hq_pascal: the element addressed by the compiler, whose criterion is the census of by-name array calls in the eight kernels' emitted asm reading zero, and the call as an emitted prologue. The 2026-09-04 grid that stood here was retracted on 2026-09-16 (CEO-798) because its cells were the instrument's self-agreement ratio; this run is the first the harness signed -- exit 0, every angle agreeing on every kernel -- and it carries the retraction's own prediction, three orders from `fpc` on the WORK basis at `-O0`.

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

Snocone is Koenig's structured SNOBOL4 (Bell Labs TR 124). No independent implementation ships a test suite, so its programs are graded through the SPITBOL oracle by construction, as SNOBOL4 programs are; the paper's own examples are in the corpus, and the self-hosting front-end's own beautifier is part of its rungs suite. Its suite and benchmark readings are kept on the leaderboard (`.github/SCORE.md` § THE SUITE TABLE) and are not shown here.

### Rebus

Rebus has no independent implementation, so there is no third-party suite to grade against and no rival to time against; its programs are graded through the SNOBOL4 oracle path by construction. Its suite and benchmark readings are kept on the leaderboard and are not shown here.

## Credits

Built by Lon Jones Cherryholmes, working with Claude — Sonnet, Opus, and Fable. Part
of the [snobol4ever](https://github.com/snobol4ever) organization; the org README has
the full story of how a rediscovered 1980 idea (Byrd boxes) became a seven-language
compiler.

## License

AGPL v3. See [LICENSE](LICENSE).

---

*snobol4all. snobol4now. snobol4ever.*
