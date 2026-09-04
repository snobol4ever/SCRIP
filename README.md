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
| SNOBOL4 | Budne CSNOBOL4 suite (118 gradable `.ref` pairs; home-dialect oracle csnobol4, not sbl) | PASS=52 FAIL=25 REJECT=40 CRASH=1 HANG=0 | PASS=52 FAIL=24 REJECT=41 CRASH=1 HANG=0 |
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
against 180.

Phil Budne's **CSNOBOL4 test suite** — 124 vendored programs, 118 with a gradable `.ref`
— is graded against its own oracle, not SPITBOL: `sbl -bf` false-reds on 30 of these
programs on CSNOBOL4-only extensions SPITBOL never claims (ORD, `&DUMP`, `popen`, ...;
RULES.md FACT RULE s261). `scripts/test_snobol4_csnobol4_suite.sh` (2026-09-03): mode 3
PASS=52 FAIL=25 REJECT=40 CRASH/HANG=1 · mode 4 PASS=52 FAIL=24 REJECT=41 CRASH/HANG=1,
denominator 118 (`nqueens.sno` is the one CRASH/HANG entry in both modes — already
documented as flaky SIG11/TIMEOUT across repeated runs, RULES.md ASM-DIFF-FIRST section;
the exact CRASH-vs-HANG split isn't re-measured to one fixed reading here for the same
reason that section gives, but the PASS/FAIL/REJECT counts are stable — re-verified 3x
on a corpus-clean run once the runner's scratch dir was cured from symlinking the suite
(write-through a symlink was silently mutating two tracked fixtures, `openo.tst` and
`test.bin`, across runs) to copying it. A live-csnobol4 triangulation arm (PASS=83 FAIL=35 of 118) also
runs alongside as a tiebreak/regeneration check: of the 35 where live csnobol4 disagrees
with the vendored `.ref` too, most are the already-documented missing-`-INCLUDE`-target
class (a corpus gap, not a SCRIP defect). Read every red by name before citing this
number — see FINDING-2026-08-27-seat06-csnobol4_suite-triage-eight-classes-three-are-not-scrip-bugs.md
for the per-class read (confirmed SCRIP defects: `&DUMP` is a no-op, content after `END`
breaks re-scanning, `-INCLUDE` argument trailing-space handling, a mode-4-only `SETEXIT`
link failure; same dialect-distance caveat as snoflake above for the rest).

And `beauty.sno`, the SNOBOL4 pretty-printer written in
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
nineteen kernels passed the agreement gate on a shared machine — the rest below).
Triangulator: `scripts/bench_triangulate_snobol4.sh` (angle 1 `test_bench_snobol4_timed.sh`,
angle 2 `bench_snobol4_fixed_iter.sh`, angle 3 disk telemetry via `tools/bench_rusage`);
re-run 2026-09-04 under a 16-seat-fleet load and only 2 of 19 kernels agreed (arith_loop,
eval_fixed — both republished below unchanged from the already-AGREEing 2026-08-31 run,
since the two dates' numbers match within noise), confirming this basis is genuinely
load-sensitive rather than re-measuring for its own sake; see hq_P's live-load ruling
this session (RULES.md § THE TWO-NUMBER BENCHMARK BASIS) for why a quiet re-run, not a
loaded one, is what would move this table:

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

**SNOBOL4 demos × vs SPITBOL** (`scripts/bench_triangulate_demos_snobol4.sh`, 2026-08-28,
whole-program wall-clock totals, best of
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

**Major demo — the JCON compiler, written in Icon.** JCON (Proebsting & Townsend,
Arizona) is a production Icon-to-JVM compiler, itself written in Icon: 9,953 lines
across 16 hand-written modules plus 2 that JCON generates with its own Icon programs.
It is the largest real-world Icon program in the corpus, and it is a demo rather than
a test because it exercises the whole front end at once.

The demo entries live in `corpus/demos/icon/jcon/`. Each is a thin file of `link`
directives resolved by SCRIP's own `icn_resolve_links`, naming exactly the modules
that real JCON program is built from — not a concatenated command line, which
silently merges JCON's four separate `procedure main`s into one program. The gate
builds its own `icont` oracle from the same sources every run:

| demo | what it is | m3 | m4 | vs oracle |
|---|---|:---:|:---:|---|
| `interfacegen` | JCON's Java-interface table generator (415 lines out) | ✅ | ✅ | byte-identical |
| `jlink` | `jlink`, JCON's 2-module class linker | ✅ | ✅ | byte-identical |
| `oplexgen` | JCON's operator-lexer generator (611 lines out) | ⚠ | ⚠ | same 611 lines, different order |
| `jtran` | the full 17-module translator | ⛔ | ⛔ | does not build |

`oplexgen` walks `key(t)` to emit its decision tree; SCRIP's table-key order differs
from the oracle's, so the same 611 lines come out in a different order (`sort` of the
two outputs is byte-identical, and m3 ≡ m4 exactly). Icon does not specify `key()`
order, so the generated source stays valid either way.

`jtran` does not currently build in either mode — the compiler itself stops. Both
sites are in `lexer.icn`, on the shape `EXPR ? { while COND do … suspend … }`: the
scan's resume path lands on the loop **condition**'s β port, and a non-resumable
condition (a literal or plain variable) emits only an α entry. `while f()` — a
resumable condition — compiles and links fine. Minimal witness, two lines:

```icon
procedure g(); "" ? { while 1 do suspend 1 }; end
procedure main(); write(g()); end
```

Gate: `scripts/test_demo_icon_jcon.sh` (refuses `rc=2` rather than skipping; a
declared known-difference that starts passing is reported `XPASS` and fails, so a cure
cannot land silently).

**Demo benchmarks.** Three-angle triangulation, `scripts/bench_triangulate_demos_icon.sh`.
⛔ **Basis: one iteration = one whole program run**, so every number is a TOTAL carrying
process startup (and, for m3, the compile). These are not kernel slopes and must never
share a column with the kernel grid above. Angles 1 (fixed time) and 2 (fixed
iterations) agreed within 4% on every row; two independent runs, 2026-09-03, `RT_OPT=-O0`:

| demo | m3 vs `iconx` | m4 vs `iconx` |
|---|:---:|:---:|
| `interfacegen` | 0.19–0.21x | **1.28–1.40x** |
| `jlink` | 0.03x | 0.77x |

Ranges are the spread across the two runs, not error bars. m3 compiles at run time, so
its total includes the compile; m4 is a prebuilt binary. ⛔ No work-basis multiple is
published: these programs run in 4–5 ms against a 2–4 ms process startup, so
total-minus-overhead is dominated by its own error bars — the harness detects that and
refuses the work number rather than printing one. `oplexgen` and `jtran` get timings
but **no multiple**, because their output does not match the oracle.

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
cross-check of these numbers (`scripts/bench_triangulate_prolog.sh`, the same kernel timed two more ways: fixed time
counting iterations, and fixed iterations measuring time, the two rates required to
agree) currently covers the SWI and GNU columns only: SCRIP's arms hit a known crash
in repeated re-entry that is being fixed (still true 2026-09-04 — the triangulator's
own header documents m3 crashing on all 21 van Roy kernels in repeated-call form, a
different program shape than the single-call kernels timed above), and until they
run, these SCRIP numbers
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

**Coverage: the full 21-kernel board, nothing dropped.** The grid above times the ten
kernels that carry a self-timing bracket. The remaining eleven are not absent — they
are *red*, and a benchmark page that shows only what passes is the failure mode this
project measures against. `scripts/bench_prolog_vanroy.sh --two-number` prints all
**21 vanroy kernels**, each in exactly one bucket, and refuses (`rc=2`) if either
rival binary is missing rather than filling a column:

| bucket | n | meaning |
|---|:---:|---|
| MEASURED | 2 | `deriv`, `fib` — an `AGREE` verdict from **every** rival engine in `triangulation-*.tsv`, not just one |
| DECLARED | 8 | runs green, no triangulated rate yet, carries a checked reason |
| REFUSE | 11 | **crashes today** — `SIGSEGV` or `abort` — printed RED with its `rc`, never dropped |

Promotion needs a full house: a kernel that one rival agrees with and another
disagrees with stays out of MEASURED, because a board that cites its friendliest
rival is not measuring, it is choosing. SCRIP's own `m3`/`m4` rows are reported by
the triangulator but never vote on promotion — they are UNPROVEN for every kernel
by the same crash the REFUSE column counts, and letting the subject veto its own
measurement would pin this bucket at zero. The rule is gated by
`scripts/test_gate_vanroy_bucket_rule.sh`.

Eleven of twenty-one kernels crash before they can be timed. That number is the
headline, not a footnote: it is the ceiling on every Prolog performance claim this
page can make, and it is why the buckets are published beside the speeds.

⛔ **The board publishes buckets, not new multiples, and the control arm is why.** Run
twice back-to-back on one binary, all 21 bucket assignments reproduced **identically**
— but the single-run multiples moved by −2.8% on `fib` (≈12 ms of work) and −27.7% on
`deriv` (under 1 ms), the spread widening as the kernel shrinks toward the rivals' 1 ms
quantization. Under fleet load a single-run multiple on a sub-millisecond kernel is a
scouting datum, not a grid, so the speeds above remain the best-of-5 numbers and the
board contributes coverage. Basis is printed per row (`SELF` for the ten bracketed
kernels, `FLOOR` for the rest) and the two are never mixed in one column.


### Raku

**Coverage** (third-party): graded against **Roast**, Raku's official specification
test suite, run whole (`scripts/raku_roast_scoreboard.sh`, 986 in-tier 6.c files,
2026-08-30): **PASS=3 FAIL=9 PARSE-FAIL=927 NO-TAP=4 CRASH=2**, 41 missing. The
frontend today accepts a deliberate working subset of Raku; idiomatic spec-test Raku
is overwhelmingly outside it, and the parse-fail column is the roadmap.

**Benchmarks.** Re-measured 2026-09-02 on the two-number basis (RULES.md § THE
TWO-NUMBER BENCHMARK BASIS), same basis change that corrected the Prolog grid below:
WORK is each kernel's own `wall_us()` bracket (stderr, byte-comparable stdout
unaffected); OVERHEAD is external elapsed − WORK, its own number, never mixed into a
WORK column. `scripts/bench_triangulate_raku.sh`, REPS=3 best-of, every rep
byte-verified against `.ref`; SCRIP `--run` (m3) and `--compile` (m4) vs Rakudo
2026.05. Multiple = Rakudo `work_us` / SCRIP `work_us` (axis named once: above 1.00x
SCRIP is faster):

| kernel | SCRIP m3 | SCRIP m4 | Rakudo | × (m3) | × (m4) |
|---|---:|---:|---:|:---:|:---:|
| string-escape | 61 µs | 75 µs | 1.52 ms | **24.9x** | **20.2x** |
| send-more-money-loops | 178 ms | 180 ms | 1.11 s | **6.23x** | **6.16x** |
| point_class_add | 11.4 s | 11.2 s | 1.06 s | 0.093x | 0.095x |
| point_class_add1 | 22.0 s | 16.4 s | 4.04 s | 0.184x | 0.247x |

OVERHEAD (best rep, startup+teardown only): SCRIP m3 ~6–96 ms, m4 ~3–70 ms, Rakudo
~237–300 ms — Rakudo's process-launch constant the totals basis used to charge to
the engine on every kernel regardless of size, most visible on `string-escape` where
it was ~99% of the old 201 ms total.

**The reading:** same split the totals grid already showed, restated on work rather
than process time — loop/integer work (`string-escape`, `send-more-money-loops`)
crushes; object/method-heavy work (`point_class_add`, `point_class_add1`) is behind.
Magnitudes moved once startup stopped being charged to either side (e.g.
`string-escape` 55.9x → 24.9x): both numbers were always real, they measured
different things.

⛔ **Cross-proof status: not yet MEASURED, by design.** `bench_triangulate_raku.sh`
now also runs angle 1 (`test_bench_raku_timed.sh`, live fixed-time search) and angle
2 (`bench_raku_fixed_iter.sh`, a committed fixed-N from `fixed-iter-n.tsv`) and
requires their rates to AGREE (flat 10%, UNBAKED — same unbaked tolerance the Prolog
triangulator uses) before `test_gate_bench_rivals_coverage.sh` will count a kernel
MEASURED. First run: every cell reads DISAGREE, spread 10–36% — neither angle
repeats internally (unlike angle 3's REPS=3 best-of), so a single live-search sample
against a single fixed-N sample is exactly this noisy on a shared, loaded box. The
WORK numbers above are angle 3's own byte-verified, best-of-3 measurements and are
citable on that basis; they are not yet cross-proven the way the Prolog board's
`MEASURED` bucket requires. Baking a real per-kernel noise floor (mirroring the
SNOBOL4 triangulator's `NOISE-FLOOR.tsv`) is the next step, not a retry with a looser
number picked to pass.

The other 13 kernels are not yet timed — each blocker is a named, diagnosed defect (a
map/grep code-path gap covering four of them, array parameters passed by copy instead
of aliased, rational-number semantics, five parser constructs, one crash) and the grid
grows as they land; see `corpus/benchmarks/raku/README.md`.

### Pascal

**Coverage** (third-party): graded against the **Free Pascal compiler's own test
suite**, vendored (181 programs; `scripts/test_pascal_fpc_suite.sh`, 2026-08-30):
**PASS=119 FAIL=62 in both modes**, identical failure sets — the misses are shared
semantic gaps, not per-mode flakiness. The oracle everywhere is `fpc -Miso`: every
reference output is produced by fpc, never by SCRIP's own output (with a per-file
exception list where fpc itself cannot run a program).

**Benchmarks.** First measurement, 2026-09-04, `scripts/bench_triangulate_pascal.sh`
(angle 1 `test_bench_pascal_timed.sh`, angle 2 `bench_pascal_fixed_iter.sh`, angle 3 disk
telemetry) on the 7 reps-capable classic Pascal kernels (`uplevel2`/`uplevel3`/`fbench`/`whet`
are named in `corpus/benchmarks/pascal/EXCLUDED.tsv` with their reasons — a fixed workload,
a compile-time SIGSEGV, and no committed reference, respectively — never silently dropped).
⛔ **Taken under a loaded 16-seat fleet** (RULES.md § THE TWO-NUMBER BENCHMARK BASIS); of 7
kernels, 4 fully passed all three angles' agreement gate and are published, 3 (`bubble`,
`intmm`, `perm`) had at least one angle disagree and are withheld rather than averaged past:

| kernel | × vs fpc (m3) | × vs fpc (m4) |
|---|:---:|:---:|
| queens | 0.99x | **1.07x** |
| quick | 0.96x | **1.04x** |
| sieve | 0.98x | 0.91x |
| towers | 0.96x | 0.96x |

Oracle: `fpc -O2` at its released default (the rival at the speed the public actually runs,
not artificially handicapped) vs SCRIP `RT_OPT=-O0` (no `-O2` builds, ever — RULES.md § NO
-O2 BUILDS). Reading: essentially at parity with fpc's own optimizing compiler on four
classic kernels under real load-noise — a notable result for an `-O0` compiler against an
`-O2` rival, not yet strong enough to call ahead or behind given the withheld three and the
load this run carried. A quiet re-run is the natural next step, not required for this basis
to be citable (the agreement gate is what makes a loaded run trustworthy in the first place).

### Snocone

**Coverage** (third-party): none exists to grade against — Snocone is Koenig's
structured SNOBOL4 (Bell Labs TR 124) and no independent implementation ships a test
suite. Its programs lower into the same engine as SNOBOL4 and are graded through the
SPITBOL oracle; the paper's own examples are the closest thing to vendor tests and
are in the corpus.

**Benchmarks.** **× vs SPITBOL** — the SNOBOL4 twin (Snocone shares SNOBOL4's engine, so a
hand-translated equivalent program, `corpus/benchmarks/snobol4/{fib_recur,arith_loop,
string_concat_twin}.sno`, is the fair rival — README's own framing above, restated: this
does re-measure the same engine, on purpose, to check Snocone's frontend adds no
overhead). First grid, 2026-09-04, `scripts/bench_triangulate_snocone.sh`: callgrind Ir
(contention-immune — taken under a loaded 16-seat fleet, RULES.md § THE TWO-NUMBER
BENCHMARK BASIS), SCRIP mode-4 vs the CLEAN SPITBOL benchmark oracle
(`sbl_clean_bin()`, never `x64/bin/sbl`), both verified output-identical before timing:

| kernel | what it does | × vs SPITBOL (m4) |
|---|---|:---:|
| fib_recur | naive recursive `fib(24)` | **1.75x** |
| arith_loop | 300,000-iteration accumulator | **1.96x** |
| string_concat | 4,000× `&&` concatenation | **1.42x** |

Ahead of SPITBOL on all three — consistent with the SNOBOL4 engine's own kernel grid
above (op_dispatch, string_concat, fibonacci all **>1.4x**), which is exactly what "no
separate grid needed, same engine" predicts. Basis: callgrind Ir only (angles 1/2 are
not yet run for Snocone — a future pass extending `bench_snobol4_fixed_iter.sh`'s
approach to Snocone source would triangulate these the same way the SNOBOL4 grid is).

### Rebus

**Coverage** (third-party): none exists — Rebus has no independent implementation to
borrow a suite from. Its programs are graded through the SNOBOL4 oracle path
(SPITBOL x64) by construction.

**Benchmarks.** First kernel set, 2026-09-04 (`corpus/benchmarks/rebus/`, 3 kernels —
Rebus had none before this row). ⛔ **No external rival implementation exists** (Rebus,
unlike Snocone, has no engine-sharing twin and no independent production
implementation — see Coverage above) — `scripts/bench_triangulate_rebus.sh` reports
SCRIP's own callgrind Ir per kernel rather than fabricate a "× vs" ratio with nothing on
the other side of it (RULES.md § THE INSTRUMENT LAWS: a comparison that cannot show
what it is commensurable with is not one):

| kernel | what it does | m3 Ir (compile+run) | m4 Ir (run only) |
|---|---|---:|---:|
| fib_recur | naive recursive `fib(24)` | 61.0M | 48.4M |
| arith_loop | 300,000-iteration accumulator | 60.7M | 50.9M |
| string_concat | 4,000× `\|\|` concatenation | 16.7M | 6.9M |

m3's count is a whole-process total including SCRIP's own in-process compile-to-slab
step (same caveat as the Icon demo grid above), so it is never read as "m3 is slower
than m4 at running" — only m4 is pure execution. Grows into a real triangulated grid
once Rebus gets a rival worth measuring against, or its own fixed-time/fixed-iteration
angle scripts.

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
