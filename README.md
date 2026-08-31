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
that produced it. Summary, measured 2026-08-30:

| Language | Third-party suite | mode 3 | mode 4 |
|---|---|---|---|
| SNOBOL4 | snoflake fixtures (180; CSNOBOL4 home dialect — see the controls) | PASS=77 FAIL=96 | PASS=77 FAIL=47 (50 skipped at the link step) |
| Icon | Arizona Icon v9.5 `tests/general` (89 gradable) | PASS=39 FAIL=50 | PASS=40 FAIL=49 |
| Icon | JCON test suite (81 gradable) | PASS=41 FAIL=25 CRASH=13 HANG=2 | PASS=39 FAIL=32 CRASH=8 HANG=2 |
| Prolog | SWI-Prolog plunit suite (114) | PASS=92 FAIL=22 (80%) | — |
| Prolog | GNU Prolog source tree (62 files) | 45 compile as libraries; 15 run: 6 match, 9 differ; 2 parse-fail | — |
| Raku | Roast (official spec tests, 986 in-tier 6.c files) | PASS=3 FAIL=9 PARSE-FAIL=927 NO-TAP=4 CRASH=2 (41 missing) | — |
| Pascal | FPC test suite (181 vendored) | PASS=119 FAIL=62 | PASS=119 FAIL=62 |
| Snocone | none exists (see section) | — | — |
| Rebus | none exists (see section) | — | — |

### SNOBOL4 / SPITBOL

**Coverage** (third-party): the vendored **snoflake fixture suite** — 180
self-describing programs from the [snoflake](https://github.com/atdt/snoflake)
project, written in CSNOBOL4's home dialect — runs four arms side by side
(`scripts/test_snoflake_suite.sh`, 2026-08-30): SCRIP mode 3 PASS=77 FAIL=96 · mode 4
PASS=77 FAIL=47 (50 skipped at the link step), with the two oracle arms as controls — **SPITBOL
itself passes only 107/173 of it** (dialect distance, not defect count) and CSNOBOL4,
its home implementation, 168/5. Read SCRIP's number against the SPITBOL control, not
against 180. Phil Budne's CSNOBOL4 test suite (120 reference pairs) is also vendored;
its board is not yet published. And `beauty.sno`, the SNOBOL4 pretty-printer written in
SNOBOL4, reproduces itself byte-for-byte in both modes — one program, individually
checkable.

**Benchmarks.** Every number below is cross-checked before publication: each kernel
is timed two independent ways — a fixed time window counting iterations, and a fixed
iteration count measuring time — and the two rates must agree within that kernel's
measured noise tolerance; a process-level wrapper separately verifies that no disk
activity leaks into the number, and startup is excluded by construction (the kernel
times its own work). Where the two clocks disagree — as they do under machine load —
the kernel is listed as unverified rather than published. Output is byte-verified
against SPITBOL before any arm is timed.

**SNOBOL4 kernels × vs SPITBOL x64** (work rate; cross-verified 2026-08-31, six of
nineteen kernels passed the agreement gate on a shared machine — the rest below):

| kernel | m3 | m4 |
|---|:---:|:---:|
| op_dispatch | **3.40x** | **3.74x** |
| string_concat | **2.89x** | **3.27x** |
| fibonacci | **1.74x** | **1.56x** |
| pattern_bt | **1.44x** | **1.63x** |
| eval_fixed | 0.88x | 0.98x |
| table_variety | 0.62x | 0.66x |

The other thirteen kernels measured but did not pass the two-clock agreement gate on
that run (concurrent load on the build machine; their unverified readings ranged
0.30x–3.72x) and are withheld until a quiet re-run agrees — a disagreement is
reported, never averaged away.

**SNOBOL4 real-program workloads × vs SPITBOL** (callgrind instruction counts, fixed
work, startup excluded — SCRIP mode 4, 2026-08-23):

| workload | × vs SPITBOL |
|---|:---:|
| claws5 — the 3-level table gauntlet, real 66 KB corpus input | 0.734x |
| json — the deserializer, real 631 KB document | 0.422x |
| claws5 grammar only (pattern engine alone, zero captures) | **1.628x** |

These numbers cut the same machine at different joints. The bare pattern **engine** —
scanning, alternation, backtracking, zero captures — is compiled straight-line code
and beats SPITBOL (claws5 grammar-only at **1.628x**; the verified pattern_bt kernel
at **1.44x**). Real full programs — the same engine plus captures, tables, and the
runtime services they call out to — still trail (claws5 0.734x, json 0.422x). The
compiled part wins; the called-out part loses, which is why optimization targets the
runtime services, not the code generation.

**SNOBOL4 demos × vs SPITBOL** (2026-08-28, whole-program wall-clock totals, best of
3 — a different basis, not comparable to the instruction-count grids above. Every arm's output is
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
points at the GC/allocator as the next lever. This demo grid is still totals-basis and
awaits re-measurement under the cross-checked work-rate discipline above.

**beauty self-host × vs SPITBOL** (2026-08-30, wall clock, best of 5 sets of 10 runs,
outputs byte-verified identical on both arms — the beautifier formatting its own
618-line source): **0.18x** (19.0 ms vs 3.4 ms per run, re-measured on the current
tree the same day). Profiling attributes the gap
mostly to name-lookup and pattern machinery inside the runtime — measured, bounded,
and being worked — not to the compiled code itself.

### Icon

**Coverage** (third-party): the two official vendor suites — Arizona's own test set
and JCON's. Their boards, and how far the JCON self-host gets, follow the benchmark
grid below.

**Benchmarks.** **× vs Arizona `iconx`** (10/10 kernels output-identical, 2026-08-27;
whole-program wall clock — this grid predates the cross-checked work-rate discipline
now used for SNOBOL4 and is queued for re-measurement on that basis):

| kernel | × | kernel | × |
|---|:---:|---|:---:|
| int_loop | **5.64x** | list_dispatch | **2.52x** |
| mod_isolate | **4.65x** | table_miss_dispatch | **1.49x** |
| concat_dispatch | **4.45x** | concat_int_dispatch | 0.93x |
| concat_table | **2.64x** | concat_intvar | 0.72x |
| concat_strvar | **2.39x** | | |

**Vendor test suites.** Icon is graded against the two official vendor test suites,
vendored in the corpus and mechanically converted to SCRIP's explicit-semicolon
dialect (`.std` oracles untouched — the conversion changes no semantics, so PASS/FAIL
measures the engine). Every program in both suites parses. Measured 2026-08-30, both
native modes:

| suite | mode | PASS | FAIL | CRASH | HANG | of |
|---|---|:---:|:---:|:---:|:---:|:---:|
| Arizona Icon v9.5 `general/` | m3 | 39 | 50 | — | — | 89 |
| Arizona Icon v9.5 `general/` | m4 | 40 | 49 | — | — | 89 |
| JCON test suite | m3 | 41 | 25 | 13 | 2 | 81 |
| JCON test suite | m4 | 39 | 32 | 8 | 2 | 81 |

(One JCON file, `tpp`, is excluded by name: its reference output is preprocessor text
over deliberately invalid Icon, ungradable by execution.)

Verdicts: **PASS** — runs, output byte-identical to the vendor reference. **FAIL** —
runs, output differs (a wrong answer). **CRASH** — dies on a signal. **HANG** —
exceeds the 20-second limit. These are unmodified real-world Icon programs exercising
the full language surface. Runners: `scripts/test_icon_arizona_suite.sh`,
`scripts/test_icon_jcon_suite.sh` — each prints its own totals and names every
non-PASS.

**Self-host:** SCRIP compiles the 17-module JCON translator — a production Icon
compiler, `jtran`, written in Icon — into one native x86-64 binary (656K lines of
generated assembly), and that binary starts and drives its full
`preproc → yylex → parse → ast2ir → bc_File` pipeline. Class-file output is blocked
on three remaining gaps in co-expression support — argument capture at `create`,
global state across co-expressions, and per-activation storage for recursive
generators (the last already in progress). When the first lands, classes flow.

### Prolog

**Coverage** (third-party): the **SWI-Prolog plunit test suite**, vendored and run
whole (`scripts/test_prolog_swi_suite.sh`, 2026-08-30): **PASS=92 FAIL=22 of 114 —
80%**. The **GNU Prolog source tree** (62 files — its builtin library and
Prolog-to-WAM compiler): 45 compile as library modules; 15 carry entry points and
run against real `gprolog` — **6 match, 9 differ**; 2 fail to parse
(`scripts/test_prolog_gnu_suite.sh`, 2026-08-30).

**Benchmarks.** Measured 2026-08-30 on the classic van Roy kernels (vendored under
`corpus/benchmarks/prolog/`), on the **two-number basis** (`RULES.md` § THE TWO-NUMBER
BENCHMARK BASIS): each kernel self-times its **work** and reports it to `stderr`, so
`stdout` stays byte-comparable and every arm's output is verified against the
reference before it is timed at all. Startup/finish **overhead** is reported
separately instead of being folded into the number. Best of 5. An independent
cross-check of these numbers (the same kernel timed two more ways: fixed time
counting iterations, and fixed iterations measuring time, the two rates required to
agree) currently covers the SWI and GNU columns only: SCRIP's arms hit a known crash
in repeated re-entry that is being fixed, and until they run, these SCRIP numbers
stand as single-run measurements verified by output, not yet by triangulation.

⛔ **This grid replaces an earlier one that read the other way, and the correction is
the point.** The previous grid timed **whole-program totals** and reported SCRIP ahead
of SWI-Prolog on all ten kernels (up to 3.87x). On work time it is ahead on **one**.
Nothing regressed: the totals were measuring **SWI-Prolog's ~31 ms interpreter
startup**, and eight of these ten kernels do **under 300 µs** of actual work — under
1% of that startup. A basis that lets startup dominate by two orders of magnitude was
reporting the rival's process model, not this compiler's speed.

| kernel | work µs | × vs swipl | kernel | work µs | × vs swipl |
|---|:---:|:---:|---|:---:|:---:|
| tak | 20072 | **1.51x** | fib | 5195 | 0.39x |
| sendmore | 9278 | 0.55x | ops8 | 112 | 0.04x |
| cal | 29 | 0.48x | log10 | 73 | 0.04x |
| deriv | 125 | 0.04x | times10 | 207 | 0.03x |
| divide10 | 141 | 0.03x | derive | 269 | 0.03x |

**Overhead, the second number** (external total − self-timed work, best of 5): SCRIP
mode-4 binary **≈ 4.7–5.6 ms** · SWI-Prolog **≈ 31 ms** · GNU Prolog (`gplc`) native
≈ 1–2 ms. SCRIP's startup advantage over SWI-Prolog is real and large — it is simply
a *different* number from how fast the compiled code runs, which is exactly why the
basis separates them.

⛔ **The GNU Prolog work column is REFUSED, not estimated.** `gprolog`'s finest wall
clock is `real_time/1` at **1 ms**, and eight of these ten kernels do less than 1 ms
of work — their `work_us` reads exactly **0**. A zero denominator yields no multiple,
and neither "infinitely fast" nor "call it one tick" is a number anyone may publish.
Only `tak` (4 ticks) and `sendmore` (1 tick) register at all. The fix is the
fixed-iteration angle, not a fabricated floor; until then this column is a stated gap
rather than a filled-in guess.

**The reading:** ahead of GNU Prolog on startup, far ahead of SWI-Prolog on startup,
and — on work — competitive only on the deep-recursion kernel (`tak`), with the
term-rewriting kernels running 20–35x slower than SWI-Prolog. That inverts the old
grid's conclusion and names the real lever. Twelve further kernels are not timed
because SCRIP's output does not yet match the reference — a wrong answer is never
timed.

### Raku

**Coverage** (third-party): graded against **Roast**, Raku's official specification
test suite, run whole (`scripts/raku_roast_scoreboard.sh`, 986 in-tier 6.c files,
2026-08-30): **PASS=3 FAIL=9 PARSE-FAIL=927 NO-TAP=4 CRASH=2**, 41 missing. The
frontend today accepts a deliberate working subset of Raku; idiomatic spec-test Raku
is overwhelmingly outside it, and the parse-fail column is the roadmap.

**Benchmarks.** First grid, measured 2026-08-30 on the vendored kernel set
(`corpus/benchmarks/raku/`, 18 kernels with Rakudo-produced references; whole-program
wall clock, best of 3, both arms' output byte-verified first; SCRIP mode-4 binary vs
`raku` — Rakudo 2026.05):

| kernel | SCRIP | Rakudo | × |
|---|---:|---:|:---:|
| string-escape | 3.6 ms | 201 ms | **55.9x** |
| send-more-money-loops | 0.18 s | 1.64 s | **9.24x** |
| point_class_add1 | 10.4 s | 4.25 s | 0.41x |
| point_class_add | 12.1 s | 1.63 s | 0.14x |

This grid is whole-program totals and awaits re-measurement on the two-number basis
that just corrected the Prolog grid below — at string-escape's size Rakudo's column
is mostly interpreter startup, so treat that multiple as a startup story, not an
engine one; the three seconds-scale kernels measure the engines. The split reads clean: loop-and-
integer work crushes, object/method-heavy work is behind. The other 14 kernels are
not yet timed — each blocker is a named, diagnosed defect (a map/grep code-path gap
covering four of them, array parameters passed by copy instead of aliased, rational-
number semantics, five parser constructs, one crash) and the grid grows as they land.

### Pascal

**Coverage** (third-party): graded against the **Free Pascal compiler's own test
suite**, vendored (181 programs; `scripts/test_pascal_fpc_suite.sh`, 2026-08-30):
**PASS=119 FAIL=62 in both modes**, identical failure sets — the misses are shared
semantic gaps, not per-mode flakiness. The oracle everywhere is `fpc -Miso`: every
reference output is produced by fpc, never by SCRIP's own output (with a per-file
exception list where fpc itself cannot run a program).

**Benchmarks.** The `fpc` rivals grid is not yet measured.

### Snocone

**Coverage** (third-party): none exists to grade against — Snocone is Koenig's
structured SNOBOL4 (Bell Labs TR 124) and no independent implementation ships a test
suite. Its programs lower into the same engine as SNOBOL4 and are graded through the
SPITBOL oracle; the paper's own examples are the closest thing to vendor tests and
are in the corpus.

**Benchmarks.** No separate grid — Snocone compiles into the SNOBOL4 engine, so the
SPITBOL grids above are its speed story; a Snocone kernel set would re-measure the
same boxes.

### Rebus

**Coverage** (third-party): none exists — Rebus has no independent implementation to
borrow a suite from. Its programs are graded through the SNOBOL4 oracle path
(SPITBOL x64) by construction.

**Benchmarks.** None yet — the smallest frontend, benchmarked as it grows.

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
