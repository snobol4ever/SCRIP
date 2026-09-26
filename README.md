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
make test        # the core set: broad SNOBOL4 corpus (both modes) + invariant checks
```

Tests live in `scripts/`, navigable by prefix: `test_gate_*` (invariants that must
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

<!-- SUITE-TABLE:BEGIN generated by .github/scripts/util_suite_banner.py --readme from .github/SUITES.tsv at .github@acfce622 -- do not edit by hand; scripts/test_gate_readme_suite_table_matches_suites_tsv.sh holds it -->
Every row is generated from the leaderboard's machine record, `.github/SUITES.tsv` (the SUITE TABLE of `.github/SCORE.md`),
never typed by hand: the suite's latest reading, written by that suite's own runner in the landing that measured it, with the
day it was measured (the SCRIP tree and the runner of every reading are in that record). Passing / graded counts a program
only when it passes in BOTH modes. The seven masters are our own flat
suites with refs cut from each oracle; each Bench row is that language's benchmark programs graded as tests, a program passing when it prints its ref under all three angles (wrapper process, fixed iterations, fixed time limit); the others are vendored third-party suites. Raku is IN DEVELOPMENT: its rows stand as measured.

| Language | Suite | passing / graded | measured |
|:---|:---|---:|:---|
| SNOBOL4 | Gimpel | **131/148** | `2026-09-25` |
| SNOBOL4 | Budne | **71/93** | `2026-09-25` |
| SNOBOL4 | Flake | **121/180** | `2026-09-26` |
| SNOBOL4 | AIS | **7/7** | `2026-09-25` |
| SNOBOL4 | Dotnet | **5/13** | `2026-09-25` |
| SNOBOL4 | TPgm | **1/8** | `2026-09-16` |
| SNOBOL4 | X64T | **28/36** | `2026-09-25` |
| SNOBOL4 | SnoM (master) | **1971/1988** | `2026-09-25` |
| SNOBOL4 | SnoBench | **23/23** | `2026-09-25` |
| Icon | Zona | **113/119** | `2026-09-25` |
| Icon | Jcon | **86/86** | `2026-09-25` |
| Icon | IPL | **194/843** | `2026-09-25` |
| Icon | IcnM (master) | **830/830** | `2026-09-25` |
| Icon | IcnBench | **26/26** | `2026-09-25` |
| Prolog | INRIA | **442/442** | `2026-09-25` |
| Prolog | SWI | **1051/2935** | `2026-09-23` |
| Prolog | GNU | **11/11** | `2026-09-25` |
| Prolog | Logtalk | **3421/3528** | `2026-09-25` |
| Prolog | ProM (master) | **563/563** | `2026-09-24` |
| Prolog | ProBench | **23/23** | `2026-09-23` |
| Pascal | FPC | **160/181** | `2026-09-25` |
| Pascal | PAT | **386/427** | `2026-09-25` |
| Pascal | PasM (master) | **251/251** | `2026-09-25` |
| Pascal | PasBench | **11/11** | `2026-09-25` |
| Raku — IN DEVELOPMENT | Roast | **21/1464** | `2026-09-25` |
| Raku — IN DEVELOPMENT | RakM (master) | **901/947** | `2026-09-25` |
| Raku — IN DEVELOPMENT | RakBench | **6/17** | `2026-09-25` |
| Snocone | SncM (master) | **338/338** | `2026-09-25` |
| Snocone | SncBench | **16/16** | `2026-09-25` |
| Rebus | RebM (master) | **43/43** | `2026-09-25` |
| Rebus | RebBench | **3/3** | `2026-09-25` |
<!-- SUITE-TABLE:END -->

### SNOBOL4 / SPITBOL

**Coverage** (third-party, every row in the suite table above): **Gimpel** — Gimpel Software's SNOBOL4 program set, each
library module graded through a driver written for it, never excluded; **Budne** — Phil Budne's CSNOBOL4 test suite as
shipped (a program the SPITBOL oracle refuses is named in `OUTSIDE_SPITBOL_BASELINE.tsv` beside the package with the
measurement that put it there and stays in the denominator as debt); **Flake** — the [snoflake](https://github.com/atdt/snoflake)
fixture suite, 180 self-describing programs in CSNOBOL4's home dialect; **AIS** — the Shafto AI programs; **Dotnet** — the
.NET port's programs; **TPgm** — SPITBOL's own testpgms; **X64T** — the self-checking tests shipped with our x64 SPITBOL fork.
Every one is graded against the one oracle, `sbl -bf` (our SPITBOL x64 fork with its enhancements; RULES.md § Oracles), in both
modes; **SnoM** is our own flat master suite with refs cut from that oracle, and **SnoBench** the benchmark kernels graded as
tests. The numbers are in the table and nowhere else: a count typed here would have no writer.

**Benchmarks.** SnoBench is the 23 kernels under `corpus/benchmarks/snobol4/`, each a standalone program with
its `.ref` cut from SPITBOL. The three-angle harness `scripts/test_snobol4_bench_suite.sh` runs every kernel three
ways in each mode — the pristine program under the external stopwatch `tools/bench_rusage`, a generated fixed-iteration
twin and a generated fixed-time twin (`scripts/bench_wrap_snobol4.py`, the kernel never edited) — and a kernel is
published only when all three print its ref in both modes; with `BENCH_ORACLE_ARM=1` the same pristine program and the
same twins run under the clean SPITBOL oracle, so the two engines are timed on the same generated program, on the same
box, in the same minute.

*Measured 2026-09-26 08:46 CDT on SCRIP `81aa23aff` / corpus `05f26cee1`, **RT_OPT=-O0** (Lon 2026-09-26, in-chat to the ceo: *"Do use -O0 for the performance build and comparison as we update SCRIP's README with benchmark timing measurements."*), load 1.1 on 16 cores with the fleet quiet, **500 ms of repetitions per point**, five iterations (`BENCH_BUD_MS=500 BENCH_ITER_N=5`, one warm-up repetition first; `BENCH_ORACLE_ARM=1 bash scripts/test_snobol4_bench_suite.sh` on a fixture copy), oracle `spitbol-bench-oracle/sbl -bf` (the clean benchmark oracle, never the monitor-hooked correctness fork). **Both engines at SPITBOL's own defaults** (`-d128m -i1m -s4m`: SCRIP's shipped defaults since CEO-1261), SCRIP with every diagnostic off (`SCRIP_DIAG=0`, CEO-1262). 23 of 23 kernels printed their ref on every angle in all three engines. Two harness readings were taken this sitting, 08:35 and 08:46, and they agree kernel for kernel within the SPITBOL arm's own spread (its string_concat_twin read 200 then 245 µs; SCRIP's var_access m4 read 21.0 µs on the first run, a disturbed point, and 14.5 on the second); the second is the grid. The per-repetition figure is the fixed-time twin's (microseconds per repetition of the kernel's own function); the x-factor is SPITBOL time / SCRIP time on that same twin (1.5x is one and a half times faster, 0.5x half as fast).*

| kernel | SPITBOL µs/rep | CSNOBOL4 µs/rep | m3 µs/rep | m4 µs/rep | SPITBOL / m3 | SPITBOL / m4 | CSNOBOL4 / m3 | CSNOBOL4 / m4 |
|---|---:|---:|---:|---:|:---:|:---:|:---:|:---:|
| arith_loop | 24.7 | 143.8 | 6.78 | 6.75 | 3.65x | 3.67x | 21.21x | 21.30x |
| arith_loop_twin | 7326.5 | 37162.7 | 2129.2 | 2163.8 | 3.44x | 3.39x | 17.45x | 17.17x |
| array_sum | 600.9 | 3501.3 | 252.1 | 260.6 | 2.38x | 2.31x | 13.89x | 13.44x |
| eval_fixed | 140.2 | 711.2 | 206.4 | 208.4 | 0.68x | 0.67x | 3.45x | 3.41x |
| fib_recur | 3775.1 | 26326.8 | 2228.7 | 2415.1 | 1.69x | 1.56x | 11.81x | 10.90x |
| fibonacci | 213.8 | 1354.4 | 125.7 | 139.3 | 1.70x | 1.53x | 10.77x | 9.72x |
| func_call | 33.3 | 208.5 | 16.6 | 17.8 | 2.00x | 1.87x | 12.56x | 11.72x |
| ident_call1 | 30.8 | 206.6 | 11.7 | 11.0 | 2.63x | 2.81x | 17.66x | 18.78x |
| ident_call2 | 32.4 | 169.9 | 10.2 | 9.42 | 3.18x | 3.44x | 16.66x | 18.04x |
| indirect_dispatch | 26.3 | 148.6 | 48.7 | 49.4 | 0.54x | 0.53x | 3.05x | 3.01x |
| mixed_workload | 30.9 | 160.2 | 37.9 | 39.4 | 0.82x | 0.78x | 4.23x | 4.07x |
| name_indirection | 995.6 | 3486.4 | 1272.8 | 1359.1 | 0.78x | 0.73x | 2.74x | 2.57x |
| op_dispatch | 50.5 | 329.8 | 15.7 | 13.9 | 3.22x | 3.64x | 21.01x | 23.73x |
| pattern_bt | 221.0 | 947.2 | 166.4 | 167.9 | 1.33x | 1.32x | 5.69x | 5.64x |
| roman | 156.4 | 2825.7 | 178.6 | 175.0 | 0.88x | 0.89x | 15.82x | 16.15x |
| string_concat | 34.2 | 218.6 | 38.7 | 38.3 | 0.89x | 0.89x | 5.65x | 5.71x |
| string_concat_twin | 244.6 | 994.3 | 155.1 | 155.6 | 1.58x | 1.57x | 6.41x | 6.39x |
| string_manip | 56.1 | 267.6 | 49.4 | 50.8 | 1.13x | 1.10x | 5.42x | 5.27x |
| string_pattern | 65.2 | 468.6 | 76.5 | 79.7 | 0.85x | 0.82x | 6.13x | 5.88x |
| table_access | 752.1 | 5722.6 | 575.0 | 590.2 | 1.31x | 1.27x | 9.95x | 9.70x |
| table_variety | 580.1 | 2163.4 | 575.6 | 565.2 | 1.01x | 1.03x | 3.76x | 3.83x |
| test_icon | 1.02 | 1.94 | 0.284 | 0.259 | 3.60x | 3.95x | 6.83x | 7.49x |
| var_access | 63.3 | 321.0 | 16.5 | 14.5 | 3.84x | 4.36x | 19.45x | 22.14x |

Geometric mean over the 23 kernels: **1.57x** in mode 3, **1.57x** in mode 4; SCRIP is ahead of SPITBOL on 16 of 23 kernels in mode 3 and 16 in mode 4. **Why SCRIP is slower where it is slower,
measured on this tree with `perf stat` (user instructions and cycles per repetition, the slope between 1000- and 3000-repetition twins) and callgrind
(exclusive share, mode 4):** wherever the compiler resolves the operation — arithmetic, variables, direct calls to DEFINE'd functions, arrays, the
statement loop — SCRIP executes about half SPITBOL's instructions at twice its IPC and reads 2–4x. Every kernel below 1x executes 1.7–2.8x SPITBOL's
instructions (eval_fixed 2.60 M against 1.23 M per repetition, indirect_dispatch 851 K against 308 K, string_concat 733 K against 256 K, roman 3.05 M
against 1.25 M, string_pattern 1.35 M against 787 K), and SCRIP's higher IPC hides half of that gap (roman 0.41x on instructions, 0.91x on cycles;
mixed_workload 0.50x and 0.98x; table_variety 0.50x on instructions and ahead, 1.07x, on cycles). The instructions are in the runtime's C at -O0,
reached per operation: (1) **the deferred-capture pump** `rt_dcap_pump`, a C walk of the pending captures on every match success where SPITBOL stores
two words — 37% of string_pattern, 11% of roman, 7% of the calculator, 6% of mixed_workload; (2) **deferred patterns** (`*pat`, `*f()`): the
`match_defer` boxes re-enter the deferred pattern at every subject position and open a C entry per evaluation (`rt_patv_defer_open_entry`,
`rt_defer_resolve`) — eight `match_defer` boxes are 67% of porter's instructions, and json's profile is flat over four of them; (3) **the by-name
paths** — a DATA field function `n(x)` is a call by name through `rt_call_name_sn4` with a `strcmp`-confirmed inline cache (18% of treebank), an
APPLY'd or indirect call runs the C open/prologue/save-by-name/restore/epilogue protocol (`rt_name_save_push`, `rt_proc_call_prologue`,
`rt_apply_open`, `rt_call_open_by_name_p`, `rt_proc_find`: 45% of indirect_dispatch), and EVAL still enters the generic by-name builtin dispatcher
and reads and writes its result temporary by name (`try_call_builtin_by_name_bl_s` 13%, `NV_GET`/`NV_SET`/`NV_CELL_IF_FASTSET` 22% of eval_fixed);
(4) **the string primitives in C** — the in-place extension bookkeeping of concatenation (`rt_sxt_extend`, `rt_sxt_note`, `rt_sxt_match`: 39% of
string_concat) costs more than the copy it saves, `c_str_concat_d` is another 24%, `IS_NULL` is a function call (5%), and REPLACE (`bn_replace`,
6% of roman) and `descr_to_str` are C at -O0 where SPITBOL's `sbstr`, `pstr1` and `bln10` are hand-written assembly; (5) **the collector at the
1 MB window** — `gc_collect_ex` is 9% of roman and 7% of mixed_workload at about ten times SPITBOL's cost per regeneration. One leak found by the
same profile: on a default build with `--stlimit` off, every DEFINE call still enters the trace layer to find it idle (`trace_idle` and
`rt_trace_event_args`, 6.5% of indirect_dispatch) — rowed. On the demos SCRIP executes 1.7–2.4x SPITBOL's user instructions (treebank 1.80 G against
0.81 G, json 447 M against 184 M, porter 2.19 G against 1.29 G, calculator-1 277 M against 121 M) at 1.5–2.1x the cycles, and takes 3–20x FEWER page
faults since the huge-page stack landing (treebank 2,115 against SPITBOL's 15,661), which is why the wall-clock multiples sit at 0.65–0.97x while the
instruction multiples sit at 0.41–0.59x. SPITBOL's own hot spots, read with `perf record` on `sbl -bf`: porter spends 20% in `pstr1` and 7% in
`sbstr` (its pattern-string and substring primitives), treebank is flat over `bffc3`, `arith`, `o_fns`, `ocnc1` and `sbstr` at 3–5% each.

**Against CSNOBOL4 (Lon 2026-09-26, in-chat to the ceo, verbatim: *"My theory for SNOBOL4 is that we should be 10x faster than an interpreter, CSNOBOL4, and 2-3x faster than a threaded-code interpreter, SPITBOL, since SCRIP is a TRUE COMPILER."*).** The CSNOBOL4 column is Phil Budne's CSNOBOL4B 2.3.3 (`/home/resources/csnobol4/snobol4 -b -f`, the reference engine, never a grader) on the same generated twins and the same demo inputs, measured the same minute. Its `TIME()` is milliseconds, so the fixed-time twin cannot run on it: its per-repetition figure is the process-wrapper CPU-time SLOPE between an N- and a 3N-repetition iteration twin (startup and compile cancel), and SPITBOL measured by the same slope method reads within 0.81–1.22 of its fixed-time column, which is the cross-proof of the method. The multiple is CSNOBOL4 time / SCRIP mode-4 time: geometric mean **8.62x** over the 23 kernels, SCRIP ahead on 23 of 23; the demos claws5 refuses, Error 5 (undefined function or operation) at line 13; treebank refuses, Error 5 (undefined function or operation) at line 92; json refuses, Error 5 (undefined function or operation) at line 223; porter runs, output differs from SPITBOL's; calculator-1 refuses, Error 5 (undefined function or operation) at line 32; calculator-2 refuses, Error 5 (undefined function or operation) at line 62. Against the theory: where the compiler resolves the operation the tight-loop kernels read the order of 10x CSNOBOL4 and 3–4x SPITBOL, and where SCRIP trails SPITBOL (the runtime's C reached per operation, below) it trails the theory by the same mechanism, since CSNOBOL4 and SPITBOL both run those primitives in code written once by hand.

**The workhorse demos on their large inputs, whole program, best of 5, tree `81aa23aff`, 2026-09-26 08:37 CDT (beauty 09:1x), load 0.5–0.7,
`-d512m -i64m -s256m` given to all three engines (`sbl -bf`, `scrip --run`, and the mode-4 binary; RULES.md hard-cap rule clause 8(e)), SCRIP with
`SCRIP_DIAG=0`, outputs byte-identical on all three. The m3 column is the whole program including SCRIP's in-process compile, as SPITBOL's
column includes SPITBOL's; m4 is the prebuilt binary. beauty is the Snocone beautifier self-hosting on its own 41 KB source (Lon 2026-09-26: *"One more SNOBOL4 speed test, the beauty self host"*); its 21 `-INCLUDE` directives resolve from `corpus/include`, so every engine runs it from there, and its three outputs are byte-identical (40,942 bytes):**

| demo | input | SPITBOL ms | m3 ms | m4 ms | m3 | m4 | CSNOBOL4 ms | CSNOBOL4 / m4 |
|---|---:|---:|---:|---:|:---:|:---:|---:|:---:|
| claws5 (CLAWS5inTASA.dat ×16) | 1,043 KB | 68 | 68 | 49 | 0.99x | 1.38x | refuses, Error 5 (undefined function or operation) at line 13 | — |
| treebank (treebank.input ×1024) | 327 KB | 108 | 192 | 168 | 0.56x | 0.65x | refuses, Error 5 (undefined function or operation) at line 92 | — |
| json (citm_catalog.json) | 1,686 KB | 33 | 72 | 42 | 0.46x | 0.79x | refuses, Error 5 (undefined function or operation) at line 223 | — |
| porter (porter.input ×4) | 742 KB | 258 | 435 | 285 | 0.59x | 0.91x | runs, output differs from SPITBOL's | — |
| calculator-1 (calculator.input ×4) | 127 KB | 32 | 84 | 34 | 0.38x | 0.94x | refuses, Error 5 (undefined function or operation) at line 32 | — |
| calculator-2 (calculator.input ×4) | 127 KB | 35 | 82 | 36 | 0.42x | 0.97x | refuses, Error 5 (undefined function or operation) at line 62 | — |
| beauty (self-host: beauty.sno beautifying itself) | 41 KB | 31 | 516 | 140 | 0.06x | 0.22x | refuses, Error 5 (undefined function or operation) at line 51 | — |

### Icon

**Coverage** (third-party, every row in the suite table above): **Zona** — Arizona's own test set, shipped with the
reference implementation; **Jcon** — the tests of Proebsting & Townsend's Icon-to-JVM compiler; **IPL** — the Icon Program
Library's programs, each with a ref cut from Arizona `icont`/`iconx` 9.5; **IcnM** is our own flat master suite and
**IcnBench** the classic benchmark set graded as tests. The oracle everywhere is Arizona `iconx`; a vendored source is
converted to SCRIP's semicolon-required Icon on the way in and checked in converted. Their boards, and how far the JCON
self-host gets, follow the benchmark grid below.

**Benchmarks.** IcnBench is every Icon program under `corpus/benchmarks/icon` with a `procedure main` — the ten micro-kernels (`bench_icn*`), the six classic programs (`concord`, `deal`, `geddump`, `ipxref`, `queens`, `rsg`), the four `rtx/` kernels and the five `shootout/` programs, plus `tgrlink` — each a standalone program with its `.ref` cut from Arizona `icont`/`iconx` 9.5.25a and its own `.argv`, input and declared arena beside it. The three-angle harness `scripts/bench_triangulate_icon.sh` runs each kernel three ways in `iconx`, m3 and m4 — the pristine program under `tools/bench_rusage`, a generated fixed-iteration twin and a generated fixed-time twin (`scripts/util_icon_bench_wrap.py`, the kernel never edited; the loop shape is the oracle's: a kernel whose state carries across calls under `iconx` is iterated one process per iteration, `PROC`) — and a kernel is cited only when all three engines print the ref on every angle and the two timed angles agree within 15%.

*Measured 2026-09-26 09:16–09:21 CDT on SCRIP `b12bb87f8` / corpus `05f26cee1`, **RT_OPT=-O0**, fleet quiet, load 0.5–1.4 on 16 cores, **500 ms of repetitions per fixed-time point, five iterations per fixed-iteration point** (`BUDGET_MS=500 BENCH_N_DEFAULT=5`), oracle Arizona `iconx` 9.5.25a, SCRIP with every diagnostic off (`SCRIP_DIAG=0`). Every kernel printed its ref on every run of all three angles in every engine. The per-iteration figure is the fixed-time twin's (microseconds per iteration of the kernel's own `main`); the x-factor is `iconx` time / SCRIP time on that same twin (1.5x is one and a half times faster, 0.5x half as fast). A row whose two timed angles disagree carries its readings and no multiple: at five iterations a kernel that runs in under a millisecond leaves the fixed-iteration angle at the timer's floor, and those rows are re-read at a larger iteration count below.*

| kernel | loop | `iconx` µs/it | m3 µs/it | m4 µs/it | `iconx` / m3 | `iconx` / m4 | angles |
|---|---|---:|---:|---:|:---:|:---:|---|
| agg_field_isolate | IN | 700000 | 767000 | 785000 | 0.91x | 0.89x | AGREE, PASS |
| int_loop | IN | 72857 | 7738 | 7859 | 9.41x | 9.27x | AGREE, PASS |
| int_mod_isolate | IN | 67500 | 12293 | 11814 | 5.49x | 5.71x | AGREE, PASS |
| num_isolate | IN | 2230000 | 496000 | 484000 | 4.50x | 4.61x | AGREE, PASS |
| num_mixed | IN | 580000 | 769000 | 730000 | 0.75x | 0.79x | AGREE, PASS |
| rel_isolate | IN | 1350000 | 480500 | 489000 | 2.81x | 2.76x | AGREE, PASS |
| str_concat_dispatch | IN | 74286 | 9863 | 9863 | 7.53x | 7.53x | AGREE, PASS |
| str_concat_int_dispatch | IN | 102000 | 91167 | 91500 | 1.12x | 1.11x | AGREE, PASS |
| str_concat_intvar | IN | 88333 | 88167 | 88000 | 1.00x | 1.00x | AGREE, PASS |
| str_concat_strvar | IN | 74286 | 47455 | 47091 | 1.57x | 1.58x | AGREE, PASS |
| str_concat_table | IN | 196667 | 61667 | 61111 | 3.19x | 3.22x | AGREE, PASS |
| sub_list_dispatch | IN | 114000 | 96167 | 96000 | 1.19x | 1.19x | AGREE, PASS |
| sub_table_miss_dispatch | IN | 88333 | 73714 | 72714 | 1.20x | 1.21x | AGREE, PASS |
| sub_table_miss_semantics | IN | 1802 | 3115 | 3096 | 0.58x | 0.58x | AGREE, PASS |
| concord | IN | 22174 | 34600 | 32250 | 0.64x | 0.69x | AGREE, PASS |
| deal | IN | 26.6 | 40.2 | 23.4 | 0.66x | 1.13x | AGREE, PASS |
| fannkuch | IN | 7042 | 8113 | 8073 | 0.87x | 0.87x | AGREE, PASS |
| geddump | PROC | 100000 | 3088000 | 3061000 | 0.03x | 0.03x | AGREE, PASS |
| ipxref | PROC | 0.00 | 3479 | 3674 | — | — | iconx n/a PASS |
| mandelbrot | IN | 27778 | 7754 | 7636 | 3.58x | 3.64x | AGREE, PASS |
| pidigits | IN | 62.8 | 1443 | 1392 | 0.04x | 0.05x | AGREE, PASS |
| queens | PROC | 0.00 | 360 | 451 | — | — | iconx n/a PASS; m4 DISAGREE(37%) PASS |
| reverse-complement | IN | 26.8 | 68.3 | 50.2 | 0.39x | 0.53x | AGREE, PASS |
| rsg | PROC | 29412 | 40308 | 31562 | 0.73x | 0.93x | AGREE, PASS |
| spectral-norm | IN | 100000 | 106800 | 100500 | 0.94x | 1.00x | AGREE, PASS |
| tgrlink | IN | 145000 | 190000 | 195000 | 0.76x | 0.74x | AGREE, PASS |

Geometric mean over the 24 kernels citable on every angle: **1.08x** in mode 3 and **1.14x** in mode 4 against Arizona `iconx`; SCRIP ahead on 12 of 24 in mode 3 and 13 in mode 4. **Why SCRIP is slower where it is slower, measured the same sitting (callgrind, exclusive share, mode 4 at -O0, `SCRIP_DIAG=0`):** where the compiler resolves the operation — integer and real arithmetic, the loop, a string concatenation whose operands it can see, a table store — SCRIP reads 3–9x `iconx` (int_loop 9.3x, str_concat_dispatch 7.5x and 25x on instructions, int_mod_isolate 5.7x, num_isolate 4.6x, mandelbrot 3.6x, str_concat_table 3.2x, rel_isolate 2.8x). Every kernel that trails spends its time in four runtime paths reached per operation: (1) **a builtin called by name** — `try_call_builtin_by_name_bl_s` with `rt_call_arr_impl`, `rt_call_arr_bl_s`, `core_icn_builtin_argcheck` and, in rsg, a `strcasecmp` per call, are 25–40% of num_mixed (0.79x), rsg (0.93x) and tgrlink (0.74x): an Icon builtin reaches its body through a string-keyed lookup on every call where `iconx` calls a function pointer; (2) **a record field resolved by name at run time** — `data_field_ptr` 36%, the `strcmp` it drives 26%, `icn_field_get` 7%, `rt_data_is_record_inst` 8% and `rt_list_view` 8% of agg_field_isolate (0.89x): `r.f` compares field names on every access where `iconx` resolved the field's index when the record type was declared; (3) **the collector at the shipped 1 MB window** — `gc_collect_ex`, `gc_visit_one` and the visitors are 34% of concord (0.69x), 42% of tgrlink (0.74x), 45% of table_miss_semantics and 5% of num_mixed: an Icon program that builds strings and lists collects the window constantly, and each collection costs about ten times a SPITBOL regeneration (the cfo's row); concord's other half is the concatenation itself, already an asm leaf (`str_concat_d` 21.5%), plus `rt_str_alloc` 5%; (4) **number formatting through the C library** — `__printf_buffer`, `_itoa_word` and `vfprintf` are 17% of num_mixed, with `rt_coerce_num2_d` 8% beside them: an integer or real written or concatenated goes through `printf`; (5) **large-integer arithmetic in C at -O0** — pidigits reads 0.04x (`iconx` 62.8 µs per iteration against m4 1,392), every operation of it a `bignum.c` limb loop against `iconx`'s big-number routines at their release optimization. The short kernels re-read at 5,000 iterations, every engine agreeing on both angles: deal 1.14x in mode 4 (0.66x in mode 3), reverse-complement 0.53x (0.39x), pidigits 0.05x (0.04x); at 300 iterations table_miss_semantics 0.58x and fannkuch 0.87x in both modes. **geddump (0.03x)** is the collector and nothing else: `perf` on its mode-4 run puts 75% of its cycles in `gc_visit_one` (23%), `gc_collect_ex` (15%), `gc_walk_words` (8%), `rt_gc_visit_raw` (7%), `gc_block_exact_h` (5%) and the other visitors — a 379 KB GEDCOM file built into one structure under a declared 16 MB arena collects the window over and over at ten times SPITBOL's cost per collection; the output is the ref (322,839 bytes), the time is the collector's. The two one-process-per-iteration programs, whose `iconx` arm reports no in-process work time, read on process elapsed instead: ipxref `iconx` 4.7 ms per run, m4 6.2 ms (0.75x; m3 23.3 ms with its compile, 0.20x); queens `iconx` 3.0 ms, m4 3.0 ms (1.00x; m3 12.7 ms, 0.24x). The JCON demos on the whole-program basis: interfacegen 1.11x and oplexgen 1.08x ahead of `iconx`, jlink 0.67x, on 3–5 ms programs where start-up is over half of every arm's reading (the harness prints the labelled TOTAL multiple and refuses the WORK one, CEO-173); m3 carries its in-process compile of a 10,000-line program (interfacegen 0.18x, jlink 0.03x); jtran's triangulation row is VOID-ANSWER because the triangulation feeds it differently from the graded gate, which matches the oracle on all four demos.

**The ten micro-kernels on the instruction-count instrument, both rivals** (`scripts/bench_icon_kernels.sh`, the same sitting: callgrind Ir for `iconx` and m4, WORK basis (the empty program's Ir subtracted); the `jcont` column is WALL, because a JIT's Ir moves 35% between identical runs — the two columns are never combined into one ranking; `jcont` is JCON's own Icon-to-JVM compiler on this box's JVM, with its ~70 ms start-up named as OVERHEAD and never counted as work). Geometric mean of the Ir multiple over the 10 kernels: **2.43x**, SCRIP ahead on 9.

| kernel | `iconx` Ir | m4 Ir | m4 × vs `iconx` (Ir) | `iconx` ms | `jcont` ms | m4 ms | m4 × vs `jcont` (wall) |
|---|---:|---:|:---:|---:|---:|---:|:---:|
| int_loop | 1,500,325,415 | 151,061,168 | **10.133x** | 70 | 100 | 10 | TOTAL:10.000x |
| int_mod_isolate | 1,432,325,248 | 231,061,805 | **6.280x** | 70 | 80 | 10 | TOTAL:8.000x |
| str_concat_dispatch | 1,472,394,296 | 61,060,860 | **25.370x** | 70 | 110 | 10 | 6.000x |
| str_concat_int_dispatch | 2,179,228,863 | 1,767,063,530 | **1.235x** | 100 | 120 | 90 | 0.778x |
| str_concat_intvar | 1,872,404,720 | 1,744,260,997 | **1.075x** | 90 | 100 | 80 | TOTAL:1.250x |
| str_concat_strvar | 1,546,396,088 | 906,396,938 | **1.711x** | 70 | 100 | 50 | TOTAL:2.000x |
| str_concat_table | 4,381,956,208 | 1,604,170,219 | **2.737x** | 200 | 80 | 60 | TOTAL:1.333x |
| sub_list_dispatch | 2,500,326,255 | 1,817,022,685 | **1.378x** | 110 | 120 | 90 | 0.778x |
| sub_table_miss_dispatch | 1,712,547,751 | 1,374,535,060 | **1.248x** | 80 | 90 | 70 | TOTAL:1.286x |
| sub_table_miss_semantics | 25,963,913 | 65,241,638 | **0.412x** | 0 | 70 | 0 | <1 tick |

**Vendor test suites.** Every Icon suite row — Zona, Jcon, IPL, IcnM, IcnBench — is in the suite table above and nowhere else (`scripts/test_icon_arizona_suite.sh`, `test_icon_jcon_suite.sh`, `test_icon_ipl_suite.sh`).

**Major demo — the JCON compiler, written in Icon.** JCON (Proebsting & Townsend, Arizona) is a production Icon-to-JVM compiler, itself written in Icon: 9,953 lines across 16 hand-written modules plus 2 that JCON generates with its own Icon programs — the largest real-world Icon program in the corpus. The demo entries live in `corpus/demos/icon/jcon/`, each a thin file of `link` directives resolved by SCRIP's own `icn_resolve_links`; the gate `scripts/test_demo_icon_jcon.sh` builds its own `icont` oracle from the same sources every run and grades stdout (jlink answers on stderr, and the oracle picks the stream once):

| demo | what it is | m3 | m4 | vs oracle |
|---|---|:---:|:---:|---|
| `interfacegen` | JCON's Java-interface table generator (415 lines out) | ✅ | ✅ | byte-identical |
| `jlink` | `jlink`, JCON's 2-module class linker (answers on stderr) | ✅ | ✅ | byte-identical |
| `jtran` | the full 17-module translator on its declared argv | ✅ | ✅ | byte-identical |
| `oplexgen` | JCON's operator-lexer generator (611 lines out) | ✅ | ✅ | byte-identical |

(`scripts/test_demo_icon_jcon.sh`, 2026-09-26 09:26 CDT on SCRIP `b12bb87f8`: 4 of 4 graded demos match the `icont`/`iconx` oracle in both modes, PASS(0).)

**Demo benchmarks.** Three-angle triangulation, `scripts/bench_triangulate_demos_icon.sh`, the same sitting, `iconx` vs m3 vs m4, every run answer-checked against the oracle's digest. ⛔ Basis: one iteration is one whole program run, so every number is a TOTAL carrying process start-up (and, for m3, the compile); the harness names OVERHEAD (the empty program per engine) and prints the WORK figure only where start-up is under half the reading (CEO-173). These totals never share a column with the kernel grids above.

| demo | engine | a1 runs/s | a2 runs/s | verdict | total ms | work ms | overhead ms | answer |
|---|---|---:|---:|---|---:|---:|---:|---|
| interfacegen | iconx | 233.7970 | 246.9136 | AGREE | 4.05 | 1.12 | 2.93 | abde733517 |
| interfacegen | m3 | 43.1436 | 43.3727 | AGREE | 23.056 | 19.76 | 3.30 | abde733517 |
| interfacegen | m4 | 275.7292 | 273.2240 | AGREE | 3.66 | 1.76 | 1.90 | abde733517 |
| jlink | iconx | 286.2049 | 307.1253 | AGREE | 3.256 | 0.33 | 2.93 | f7cf281eab |
| jlink | m3 | 10.0261 | 9.8875 | AGREE | 101.138 | 97.84 | 3.30 | f7cf281eab |
| jlink | m4 | 204.4589 | 206.9536 | AGREE | 4.832 | 2.93 | 1.90 | f7cf281eab |
| jtran | iconx | 230.8687 | 239.6932 | AGREE | 4.172 | 1.24 | 2.93 | 22c37c5c23 |
| jtran | m3 | 0.8628 | 0.8780 | VOID-ANSWER | 1138.91 | 1135.61 | 3.30 | 21b7d14552 |
| jtran | m4 | 117.9245 | 120.2790 | VOID-ANSWER | 8.314 | 6.41 | 1.90 | 21b7d14552 |
| oplexgen | iconx | 222.0135 | 228.6237 | AGREE | 4.374 | 1.44 | 2.93 | b35120ad1b |
| oplexgen | m3 | 50.5029 | 50.4541 | AGREE | 19.82 | 16.52 | 3.30 | b35120ad1b |
| oplexgen | m4 | 249.7367 | 245.8210 | AGREE | 4.068 | 2.17 | 1.90 | b35120ad1b |

**The JCON translator on a real input** (Lon 2026-09-26: *"also include the JCON compiler written in Icon; compare SCRIP to iconx."*): `jtran`, the 17-module translator, built by `icont` from the package sources (31 ms) and by SCRIP mode 4 (1,197,139 lines of asm, 4.5 s to compile and link), run on JCON's own `gen_bc.icn` (65 KB), the same minute, load 1.1. The preprocessor stage to stdout, best of 5, outputs byte-identical on all three engines (65,056 bytes): **`iconx` 23.5 ms · m4 36.4 ms (0.65x) · m3 1,180 ms (0.02x — the in-process compile of the 17 modules is inside the wall)**. The full pipeline `preproc gen_bc.icn : yylex : parse : ast2ir : bc_File -class:gen_bc`: **SCRIP m4 completes it in 10.8 s, 87 class files written; Arizona `iconx` dumps core (rc 139) after 1.1 s on the same run**, with the default stack and with 256 MB (CEO-1213), so there is no oracle multiple for the full translation and 10.8 s for 65 KB is the Icon workhorse number the profiles above explain (the builtin-by-name road, record fields by `strcmp`, the collector).

### Prolog

**Coverage** (third-party, every row in the suite table above): **INRIA** — the ISO/IEC 13211-1 conformance suite;
**Logtalk** — the Logtalk ISO conformance suite (3600 cases, each carrying its own expectation from the standard, graded
per family); **SWI** — the SWI-Prolog plunit test tree, graded whole; **GNU** — the GNU Prolog source tree's programs that
carry entry points, run against real `gprolog`; **ProM** is our own flat master suite and **ProBench** the classic van Roy
kernels graded as tests. The oracle is THE SUPERSET (RULES.md § Oracles): the ISO core, every non-conflicting GNU and SWI
builtin, and a conflict settled through an ISO Prolog flag in the program (double_quotes defaults to codes, as ISO and GNU
read it); `swipl` and `gprolog` are the two reference systems, and both are instrumented for the lock-step monitor.

**Benchmarks.** ProBench is the 23 kernels under `corpus/benchmarks/prolog/bench/` — the classic van Roy / Aquarius set (tak, nreverse, qsort, deriv, ops8, times10, divide10, log10, queens, zebra, crypt, mu, query, sendmore, fib, ham, meta_qsort, ...) — each a standalone program with its `.ref`, graded as tests in both modes under three angles by `scripts/test_prolog_bench_suite.sh` (the suite table's ProBench row). The timing instrument is the three-angle cross-proof `scripts/bench_triangulate_prolog.sh`: angle 1 (`test_bench_prolog_timed.sh`) runs a live doubling search for the largest iteration count whose between-and-fail loop completes within the budget and reports iterations per CPU second; angle 2 (`bench_prolog_fixed_iter.sh`) executes the committed count from `fixed-iter-n.tsv` and measures the time; a kernel's rate on an engine is cited only when the two agree within 10%, and every engine's single-shot output is held to the `.ref` before it is timed at all. The rivals are SWI-Prolog 9.0.4 (`swipl`) and GNU Prolog 1.4.5 (`gprolog --consult-file`, its byte-code WAM), both with the per-engine preludes the kernels need.

*Measured 2026-09-26 10:0x CDT on SCRIP `b6d8c9bb8` / corpus `a4d8ab08d`, **RT_OPT=-O0**, fleet quiet (the 1-minute load 1.3–1.6 on 16 cores through the run; the 15-minute average carried an earlier episode the stamp names in CEO-1278), **500 ms fixed-time budget per point**, SCRIP with every diagnostic off (`SCRIP_DIAG=0`). The per-iteration figure is angle 1's (microseconds per iteration of the kernel's whole `main`, the between-and-fail loop around it); the x-factor is rival time / SCRIP time on the same loop (1.5x is one and a half times faster, 0.5x half as fast). A row whose angles disagree, or whose arm skipped or crashed, carries its readings and no multiple.*

| kernel | `gprolog` µs/it | `swipl` µs/it | m3 µs/it | m4 µs/it | `swipl` / m3 | `swipl` / m4 | `gprolog` / m4 | angles |
|---|---:|---:|---:|---:|:---:|:---:|:---:|---|
| cal | 0.56 | 1.94 | 4.45 | 1.66 | 0.44x | 1.17x | 0.34x | AGREE |
| crypt | 118 | 222 | 1029 | 917 | 0.22x | 0.24x | 0.13x | AGREE |
| deriv | 2.06 | 5.09 | 138 | 110 | — | 0.05x | 0.02x | m3 DISAGREE |
| derive | 2.46 | 3.64 | 50.2 | 41.7 | — | 0.09x | 0.06x | m3 DISAGREE |
| divide10 | 1.23 | 2.37 | 21.8 | 18.7 | 0.11x | 0.13x | 0.07x | AGREE |
| fib | 1542 | 1907 | 4194 | 3776 | — | 0.51x | 0.41x | m3 DISAGREE |
| ham | 60.3 | 59.9 | 972 | 858 | — | — | — | gprolog DISAGREE; swipl DISAGREE; m3 DISAGREE; m4 DISAGREE |
| log10 | 0.66 | 1.77 | 15.4 | 12.2 | 0.11x | 0.14x | 0.05x | AGREE |
| meta_qsort | 204 | 147 | 4253 | 3731 | — | 0.04x | 0.05x | m3 DISAGREE |
| mu | 26.9 | 23.0 | 309 | 278 | — | 0.08x | 0.10x | m3 DISAGREE |
| nrev | 14.7 | 12.9 | 267 | 238 | — | 0.05x | 0.06x | m3 DISAGREE |
| nreverse | 13.1 | 11.5 | 218 | 193 | — | 0.06x | 0.07x | m3 DISAGREE |
| ops8 | 1.06 | 2.16 | 14.0 | 11.2 | 0.15x | 0.19x | 0.10x | AGREE |
| qsort | 22.1 | 28.2 | 215 | 189 | — | 0.15x | 0.12x | m3 DISAGREE |
| queens | 31791 | 49509 | 191015 | 164384 | — | — | — | gprolog DISAGREE; swipl DISAGREE; m3 DISAGREE |
| queens_8 | 121 | 209 | 791 | 691 | — | 0.30x | — | gprolog DISAGREE; m3 DISAGREE |
| queensn | 76927 | 90472 | 890789 | 785731 | — | — | — | gprolog UNPROVEN; swipl UNPROVEN; m3 UNPROVEN; m4 UNPROVEN |
| query | 13.8 | 31.4 | 309 | 283 | — | 0.11x | 0.05x | m3 DISAGREE |
| sendmore | 2268 | 4596 | 4202 | 3809 | 1.09x | 1.21x | — | gprolog DISAGREE |
| tak | — | — | — | — | — | — | — | gprolog UNPROVEN; swipl UNPROVEN; m3 UNPROVEN; m4 UNPROVEN |
| times10 | 1.16 | 2.17 | 19.1 | 16.1 | 0.11x | 0.14x | 0.07x | AGREE |
| witness_depth_nrev8 | 1.29 | 2.82 | 19.6 | 17.8 | — | — | — | gprolog UNPROVEN; swipl UNPROVEN; m3 UNPROVEN; m4 UNPROVEN |
| zebra | 863 | 921 | 11006 | 9328 | — | 0.10x | 0.09x | m3 DISAGREE |

Geometric mean against SWI-Prolog: **0.15x** in mode 4 over the 18 kernels whose `swipl` and m4 angles both agree (SCRIP ahead on 2), **0.22x** in mode 3 over the 7 whose `swipl` and m3 angles agree (ahead on 1); against GNU Prolog, **0.08x** in mode 4 over the 16 kernels whose `gprolog` arm also agrees. **Why SCRIP is slower where it is slower, measured the same sitting (callgrind, exclusive share, mode 4 at -O0 on the fixed-iteration twins, `SCRIP_DIAG=0`):** the kernels fall into two classes and both are runtime C reached per operation. **(1) The term-rewriting kernels consult the Prolog parser's operator tables at run time:** `bin_ops_count` 48%, `prolog_op_table_get` 13%, `prefix_ops_count` 9%, `find_user` 2%, `op_type_unclassify` 2% (all `src/parsers/prolog/prolog_parse.c`) and `plc_op_info` 6% with a `strcmp` per lookup 5% — **three quarters of deriv (0.05x)** — so writing or taking apart an operator term (`d(U*V,X,DU*V+U*DV)`, `=..`, `write/1`) re-derives every operator's type and priority by name on every node, where a WAM carries them on the functor; derive, times10, divide10, log10 and ops8 (0.09–0.19x) are the same shape. **(2) Unification, dereference and binding are C calls per cell:** `plw_unify_cells` (with its recursive twin), `plw_cell_deref_slow`, `plw_bind`, `plw_mkc_kids`, `plw_unify_vals`, `rt_pl_deref_val` and `rt_pl_dop_unify_ci_c` are 30–45% of nreverse (0.05x), queens_8 (0.30x), zebra (0.10x) and fib (0.51x): the get, put and unify steps a WAM emits inline as a handful of instructions each are entered as C functions with a descriptor walk per argument; beside them the trail — `pl_tr_needs_log`, `pl_tr_push` and `rt_pl_tr_gc_sync` (5–9%) — and the call prologue in C (`rt_proc_call_prologue_lex` 3–5%, `rt_icn_zframe_args_install` 3%), the collector (`gc_collect_ex` 3–5%) and the allocation of every structure cell (`rt_gcheap_alloc` 3%). **(3) Atoms are interned by name at run time:** `prolog_atom_intern` 5%, `ht_hash` 4% and a `strcmp` 6% of crypt (0.24x) — an atom the compiler already saw is looked up by its characters again when the program runs. Where none of this is on the path SCRIP is ahead: cal 1.17x and sendmore 1.21x against `swipl` in mode 4. Two kernels carry no multiple for reasons that are not speed: tak is skipped by angle 1's own correctness gate because `scrip --run tak.pl` without the kernel's declared stack sidecar overflows the 4 MB default (ERROR 246) where the graded suite, which honours `tak.stack`, passes it in both modes — an instrument gap, rowed; queensn and witness_depth_nrev8 are unproven on every engine (no committed iteration count for angle 2). ham's four arms disagree between their angles at this budget and are not cited.

### Raku

**Coverage** (third-party, every row in the suite table above): **Roast** — Raku's official specification test suite, the
shipped `.t` files graded whole against a local Rakudo; **RakM** is our own flat master suite with refs cut from Rakudo,
**RakBench** the kernel set graded as tests. Raku is IN DEVELOPMENT: the front-end accepts a deliberate working subset of
the language, most of Roast is outside it, and the parse-fail column is the roadmap.

**Benchmarks.** Re-measured 2026-09-04 on the two-number basis (RULES.md § THE
TWO-NUMBER BENCHMARK BASIS) with `scripts/bench_triangulate_raku.sh`.
*Measured 2026-09-04 on SCRIP e560edb92 / corpus b7c674a17, RT_OPT=-O0, modes m3 and m4
vs Rakudo 2026.05, REPS=3 best-of, every rep byte-verified against `.ref` before timing.*

**WORK** is each kernel's own `wall_us()` bracket (written to stderr, so stdout stays
byte-comparable); **OVERHEAD** is external elapsed − WORK, reported as its own number and
never mixed into a WORK column. Multiple = Rakudo WORK / SCRIP WORK (axis named once:
above 1.00x SCRIP is faster). All three angles run; the cross-proof column is angle 1
(live fixed-time search) against angle 2 (committed fixed N):

| kernel | SCRIP m3 WORK | SCRIP m4 WORK | Rakudo WORK | × (m3) | × (m4) | cross-proof |
|---|---:|---:|---:|:---:|:---:|:---|
| string-escape | 65 µs | 71 µs | 1820 µs | **28.0x** | **25.6x** | m3/m4 AGREE, Rakudo DISAGREE |
| point_class_add1 | 10.19 s | 8.93 s | 5.70 s | 0.559x | 0.638x | ✅ all three AGREE |
| point_class_add | 9.57 s | 8.07 s | 1.19 s | 0.125x | 0.148x | m3/m4 AGREE, Rakudo DISAGREE |
| send-more-money-loops | 175 ms | 185 ms | — | — | — | Rakudo arm unverified |

⭐ **Only `point_class_add1` is cross-proven on all three arms this run** and is the one
row citable without qualification. The others' SCRIP arms agree with themselves while
the Rakudo arm's two clocks do not — Rakudo's own run-to-run spread, not SCRIP's. A
disagreement is reported, never averaged away.

OVERHEAD (best rep, startup+teardown only): **Rakudo 286–342 ms · SCRIP m3 6.6–90 ms ·
m4 3.4–92 ms**. Rakudo's process-launch constant is what the old totals basis charged to
the engine on every kernel regardless of size — most visible on `string-escape`, whose
whole WORK is 1820 µs against 286 ms of Rakudo startup, i.e. startup was ~99% of the old
total. That is the entire reason this grid is on the WORK basis.

**The reading:** loop and integer work crushes (`string-escape` 25–28x); object and
method-heavy work is well behind (`point_class_add` 0.125x/0.148x). Both were always
true — the totals basis simply could not show which was which.

The other 13 kernels are not yet timed — each blocker is a named, diagnosed defect (a
map/grep code-path gap covering four of them, array parameters passed by copy instead
of aliased, rational-number semantics, five parser constructs, one crash) and the grid
grows as they land; see `corpus/benchmarks/raku/README.md`.

### Pascal

**P4 self-host** (`scripts/test_pascal_p4_selfhost.sh`, the Zurich Pascal-P4 compiler vendored with its
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
what each test requires; **PasM** is our own flat master suite and **PasBench** the classic Pascal kernels graded as tests.
The oracle everywhere is `fpc -Miso`, with ISO 7185 the authority where FPC is looser than the standard: a rejection test
passes when SCRIP refuses the program with the ISO diagnostic, and a test the standard does not actually reject is named
outside the ISO baseline with its measurement and kept in the denominator.

**Benchmarks.** First measurement, 2026-09-04, `scripts/bench_triangulate_pascal.sh`
(angle 1 `test_bench_pascal_timed.sh`, angle 2 `bench_pascal_fixed_iter.sh`, angle 3 disk
telemetry) on the 7 reps-capable classic Pascal kernels (`uplevel2`/`uplevel3`/`fbench`/`whet`
are named in `corpus/benchmarks/pascal/EXCLUDED.tsv` with their reasons — a fixed workload,
a compile-time SIGSEGV, and no committed reference, respectively — never silently dropped).
⛔ **Taken under a loaded 16-seat fleet** (RULES.md § THE TWO-NUMBER BENCHMARK BASIS); of 7
kernels, 4 fully passed all three angles' agreement gate and are published, 3 (`bubble`,
`intmm`, `perm`) had at least one angle disagree and are withheld rather than averaged past:

⛔ **RETRACTED 2026-09-16 (ceo, CEO-798, on hq_pascal's CEO-782 report):** the four cells that stood here (queens 0.99x/1.07x, quick 0.96x/1.04x, sieve 0.98x/0.91x, towers 0.96x/0.96x) were **the instrument's angle-2-over-angle-1 SELF-AGREEMENT ratio, not a multiple against fpc** — eight of eight cells equal, to two decimals, that column of `triangulation-20260904T021323Z.tsv`, whose true WORK-basis multiples read 0.000675x to 0.032980x, and the same instrument on the same kernels reads the same three-orders-from-parity figure on the 09-04 engine rebuilt and on HEAD (hq_pascal, 2026-09-16, both loads stamped). The grid is a ceo signature and the signature is withdrawn: **no Pascal speed number is published until `bench_triangulate_pascal.sh` exits 0 with every angle agreeing and the ceo signs the run**. Speed is hq_pascal's third axis under DECTET, after completeness.

Oracle: `fpc -O2` at its released default (the rival at the speed the public actually runs,
not artificially handicapped) vs SCRIP `RT_OPT=-O0` (no `-O2` builds, ever — RULES.md § NO
-O2 BUILDS). Reading (superseded by the retraction above): the 2026-09-04 prose claimed parity; the cells it read were not multiples. The kernels are three orders of magnitude from fpc on the WORK basis at `-O0`, and that is the number the next signed grid must carry or beat.

### Snocone

**Coverage** (third-party): none exists to grade against — Snocone is Koenig's
structured SNOBOL4 (Bell Labs TR 124) and no independent implementation ships a test
suite. Its programs lower into the same engine as SNOBOL4 and are graded through the
SPITBOL oracle; the paper's own examples are the closest thing to vendor tests and
are in the corpus. **SncM** in the suite table above is our own flat master suite (the beauty test suite of the
self-hosting front-end is part of it) and **SncBench** the kernel set graded as tests.

**Benchmarks.** **× vs SPITBOL** — the SNOBOL4 twin (Snocone shares SNOBOL4's engine, so a
hand-translated equivalent program, `corpus/benchmarks/snobol4/{fib_recur,arith_loop_twin,
string_concat_twin}.sno`, is the fair rival). Boards: `scripts/bench_triangulate_snocone.sh`
(totals) and `scripts/bench_two_number_ir.sh snocone` (the two-number split below).
*Measured 2026-09-04 on SCRIP 380cc4162 / corpus `201d9e021`, **RT_OPT=-O0**, mode m4 vs
the CLEAN SPITBOL benchmark oracle `sbl_clean_bin() -bf` (never `x64/bin/sbl`), outputs
verified identical before measuring.*

⛔ **Basis: callgrind Ir — instructions retired, not time, and ONE angle.** Snocone has no
fixed-time or fixed-iteration harness yet, so these are not triangulated the way the SNOBOL4
grid above is; they are published as a labelled single-angle board, not as a timed multiple.
**WORK** = total Ir − **OVERHEAD**, where OVERHEAD is this engine's empty-program Ir measured
on the same run (the marked interim of RULES.md § THE TWO-NUMBER BENCHMARK BASIS):
**SCRIP m4 2,794,452 Ir · SPITBOL 208,782 Ir**. Multiple = SPITBOL WORK / SCRIP WORK:

| kernel | what it does | SCRIP m4 WORK | SPITBOL WORK | × vs SPITBOL |
|---|---|---:|---:|:---:|
| fib_recur | naive recursive `fib(24)` | 24,131,699 | 47,007,649 | **1.95x** |
| arith_loop | 300,000-iteration accumulator | 36,628,696 | 77,206,594 | **2.11x** |
| string_concat | 4,000× `&&` concatenation | 3,776,098 | 9,123,253 | **2.42x** |

⭐ **Separating startup moved every row, and `string_concat` most of all — 1.42x on totals to
2.42x on work.** SCRIP's process startup is ~13x SPITBOL's in instructions (2.79M vs 0.21M),
so on the shortest kernel it was 42.5% of the whole reading and was being charged to the
engine. It stays under the CEO-173 50%-of-either-arm bar, so the WORK multiple stands rather
than falling back to the labelled total; that startup is itself a real, separately reported
cost, not a number to hide.

### Rebus

**Coverage** (third-party): none exists — Rebus has no independent implementation to
borrow a suite from. Its programs are graded through the SNOBOL4 oracle path
(SPITBOL x64) by construction; **RebM** in the suite table above is our own flat master suite and **RebBench** its
three kernels graded as tests.

**Benchmarks.** First kernel set, 2026-09-04 (`corpus/benchmarks/rebus/`, 3 kernels —
Rebus had none before this row). ⛔ **No external rival implementation exists** (Rebus,
unlike Snocone, has no engine-sharing twin and no independent production
implementation — see Coverage above) — `scripts/bench_triangulate_rebus.sh` reports
SCRIP's own callgrind Ir per kernel rather than fabricate a "× vs" ratio with nothing on
the other side of it (RULES.md § THE INSTRUMENT LAWS: a comparison that cannot show
what it is commensurable with is not one):

*Measured 2026-09-04 on SCRIP 380cc4162 / corpus `201d9e021`, **RT_OPT=-O0**, boards
`scripts/bench_triangulate_rebus.sh` (totals) and `scripts/bench_two_number_ir.sh rebus`
(the two-number split). **Basis: callgrind Ir — instructions retired, not time, and ONE
angle**; Rebus has no fixed-time or fixed-iteration harness yet, so this is a labelled
single-angle board, not a timed multiple.* **WORK** = total Ir − **OVERHEAD**, the latter
being SCRIP's own empty-program Ir measured on the same run: **m3 5,054,692 · m4
2,825,462 Ir**.

| kernel | what it does | m3 total Ir | m4 total Ir | m4 WORK |
|---|---|---:|---:|---:|
| fib_recur | naive recursive `fib(24)` | 60,984,184 | 48,388,021 | 45,562,559 |
| arith_loop | 300,000-iteration accumulator | 60,662,248 | 50,847,257 | 48,021,795 |
| string_concat | 4,000× `\|\|` concatenation | 16,726,309 | 6,870,587 | 4,045,125 |

⛔ **There is no × column and that is deliberate**, not an omission: Rebus has no
independent implementation anywhere, so there is nothing to be commensurable with, and a
fabricated ratio would be exactly the class RULES.md § THE INSTRUMENT LAWS warns against.
The WORK column is still worth having on its own — it is the number a future rival, or
SCRIP's own next release, gets compared against.

m3's count is a whole-process total including SCRIP's own in-process compile-to-slab
step (same caveat as the Icon demo grid above), so it is never read as "m3 is slower
than m4 at running" — only m4 is pure execution. On `string_concat`, OVERHEAD is 41% of
the m4 reading, which is why the WORK column exists at all: the totals column understates
the engine on every short kernel.

Every number above comes from a named script grading against a third-party suite or
oracle — run it yourself and it prints its own totals.

## Credits

Built by Lon Jones Cherryholmes, working with Claude — Sonnet, Opus, and Fable. Part
of the [snobol4ever](https://github.com/snobol4ever) organization; the org README has
the full story of how a rediscovered 1980 idea (Byrd boxes) became a seven-language
compiler.

## License

AGPL v3. See [LICENSE](LICENSE).

---

*snobol4all. snobol4now. snobol4ever.*
