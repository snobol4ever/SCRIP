# SCRIP

[![License: AGPL v3](https://img.shields.io/badge/License-AGPL_v3-blue.svg)](https://www.gnu.org/licenses/agpl-3.0)

A multi-language compiler collection — SNOBOL4, Icon, Prolog, Snocone, Rebus — targeting x86-64 native ASM,
JVM bytecode, .NET MSIL, WebAssembly, and portable C — all from a single IR.
Part of the [snobol4ever](https://github.com/snobol4ever) organization.

---

## What This Is

`SCRIP` (the **scrip-cc** compiler) is a from-scratch SNOBOL4 compiler: one frontend
pipeline (`scrip-cc`) feeding five independent backend emitters. Write SNOBOL4 once.
Run it anywhere.

| Flag | Output | Status |
|------|--------|--------|
| *(default)* | Portable C with labeled gotos | ✅ 106/106 corpus |
| `-asm` | x86-64 NASM assembly | ✅ 97/106 corpus · 9 known failures |
| `-jvm` | JVM Jasmin bytecode (`.j`) | ✅ 106/106 corpus · `beauty.sno` ✅ |
| `-net` | .NET CIL assembly (`.il`) | ✅ 110/110 corpus · roman + wordcount ✅ |
| `-wasm` | WebAssembly text format (`.wat`) | 🚧 active — SW-2, hello/literals/arith |
| `-js`   | Node.js module (`.js`)           | 🚧 SJ-6 — 14/17 feat · 1286/0 emit |

The 9 ASM failures (tests 022, 055, 064, cross, word1–4, wordcount) are under active
investigation via the five-way differential monitor.

Sister repos: [`snobol4jvm`](https://github.com/snobol4ever/snobol4jvm) (full Clojure→JVM
pipeline, 2,033 tests) and [`snobol4dotnet`](https://github.com/snobol4ever/snobol4dotnet)
(full C#→MSIL pipeline, 1,874 tests).

---

## The Architecture — Byrd Boxes All the Way Down

Every SNOBOL4 statement has the same shape:

```
label:   subject   pattern   = replacement   :S(goto)  :F(goto)
```

Each pattern node compiles to a **Byrd box** — four labeled entry points wired at
compile time, zero runtime dispatch:

| Port | Greek | Meaning |
|------|-------|---------|
| **proceed** | **α** | Enter fresh — cursor at current position |
| **recede**  | **β** | Resume after backtrack from a child |
| **succeed** | **γ** | Match succeeded — advance cursor, pass forward |
| **concede** | **ω** | Match failed — restore cursor, propagate back |

Sequential composition wires γ of one node to α of the next. Alternation saves the
cursor on the left-ω path and restores it before trying right. ARBNO wires child-γ
back into its own α until child-ω exits. The wiring *is* the execution — no interpreter
table, no virtual dispatch on the hot path.

This model, first described by Lawrence Byrd in 1980 for Prolog debugging and
generalized by Todd Proebsting in 1996 as a syntax-directed code generation strategy
for goal-directed languages, turns out to describe SNOBOL4 pattern matching exactly.
All five backends implement the same four-port wiring. The semantics are identical
whether the output is C labeled gotos, x86-64 JMP instructions, JVM `goto` bytecodes,
CIL `br` instructions, or WebAssembly `return_call` tail calls.

**Hot path:** pure labeled gotos. Zero overhead. No `setjmp` on the hot path.
**Cold path:** `longjmp` for `ABORT`, bare `FENCE`, and genuine runtime errors only.

---

## Five Frontends

Five frontends share the same IR (`AST_t` / `STMT_t`):

| Frontend | Source language | Status |
|----------|----------------|--------|
| **SNOBOL4 / SPITBOL** | Full SNOBOL4 + SPITBOL extensions | ✅ active — all backends |
| **Snocone** | Andrew Koenig's structured C-like frontend (Bell Labs TR 124, 1986) | ✅ active — ASM backend (`-sc -asm`) |
| **Rebus** | Structured transpiler — Rebus source → SNOBOL4 | ✅ complete — M-REBUS ✅ |
| **Icon** | Icon — generators, suspend/resume, string scanning | ✅ active — ASM + JVM backends (`-icn`) |
| **Prolog** | Prolog — unification, backtrack, Byrd Box wiring | ✅ active — ASM + JVM backends (`-pl`) |

The Byrd Box IR is the bridge between languages. Icon generators map to the same
four ports. Prolog unification is goal-directed evaluation — the same model.
SNOBOL4, Icon, and Prolog are three syntaxes for one execution machine.

---

## Build

```bash
# Dependencies
apt-get install -y libgc-dev nasm default-jdk

# Build scrip-cc
make -C src

# C backend (default)
./scrip-cc program.sno > prog.c && gcc prog.c -lgc -o prog && ./prog

# ASM backend
./scrip-cc -asm program.sno > prog.s
nasm -f elf64 prog.s -o prog.o && gcc prog.o -lgc -o prog && ./prog

# JVM backend
./scrip-cc -jvm program.sno > prog.j
java -jar src/backend/jasmin.jar prog.j -d .
java -cp . Prog

# NET backend
./scrip-cc -net program.sno > prog.il
ilasm prog.il && mono prog.exe

# WASM backend
./scrip-cc -wasm -o prog.wat program.sno
wat2wasm --enable-tail-call prog.wat -o prog.wasm
node test/wasm/run_wasm.js prog.wasm
```

---

## Corpus Ladder

All backends climb the same 12-rung ladder against
[`corpus`](https://github.com/snobol4ever/corpus)/`crosscheck/`:

```
Rung  1: hello / output    Rung  5: control       Rung  9: keywords
Rung  2: assign            Rung  6: patterns       Rung 10: functions
Rung  3: concat            Rung  7: capture        Rung 11: data
Rung  4: arith             Rung  8: strings        Rung 12: beauty.sno
```

| Backend | Corpus | Rung 12 | Notes |
|---------|:------:|:-------:|-------|
| C (portable) | ✅ 106/106 | — | Full corpus |
| x86-64 ASM | ⚠ 97/106 | — | 9 known failures; monitor investigation active |
| JVM bytecode | ✅ 106/106 | ✅ | `beauty.sno` self-beautifies — M-JVM-BEAUTY ✅ |
| .NET MSIL | ✅ 110/110 | — | roman + wordcount pass — M-NET-SAMPLES ✅ |

**Oracle:** CSNOBOL4 2.3.3 — `snobol4 -f -P256k -I$INC file.sno`

---

## Validate

```bash
# C backend
bash test/crosscheck/run_crosscheck.sh

# ASM backend (STOP_ON_FAIL=0 shows all results)
STOP_ON_FAIL=0 bash test/crosscheck/run_crosscheck_asm_corpus.sh

# JVM backend — full corpus
JASMIN=src/backend/jasmin.jar
bash test/crosscheck/run_crosscheck_jvm.sh

# JVM backend — manual per-rung (e.g. patterns rung)
JASMIN=src/backend/jasmin.jar
PDIR=../corpus/crosscheck/patterns
for sno in $PDIR/*.sno; do
  base=$(basename $sno .sno); TMPD=$(mktemp -d)
  ./scrip-cc -jvm "$sno" > $TMPD/p.j 2>/dev/null
  java -jar $JASMIN $TMPD/p.j -d $TMPD/ 2>/dev/null
  cls=$(ls $TMPD/*.class 2>/dev/null | head -1 | xargs basename 2>/dev/null | sed 's/.class//')
  got=$(java -cp $TMPD $cls 2>/dev/null); exp=$(cat "${sno%.sno}.ref" 2>/dev/null)
  rm -rf $TMPD
  [ "$got" = "$exp" ] && echo "PASS $base" || echo "FAIL $base"
done

# NET backend
bash test/crosscheck/run_crosscheck_net.sh
```

---

## Correctness — Chomsky Hierarchy Oracles

SNOBOL4 patterns are not a regex engine. They are a universal grammar machine.
The corpus includes mathematical oracles at every tier of the Chomsky hierarchy:

| Tier | Oracle language | All backends |
|------|----------------|:------------:|
| Type 3 — Regular | `(a\|b)*abb`, `a*b*`, `{x^2n}` | ✅ |
| Type 2 — Context-free | `{a^n b^n}`, palindromes, Dyck language | ✅ |
| Type 1 — Context-sensitive | `{a^n b^n c^n}` | ✅ |
| Type 0 — Turing | `{w#w}` copy language | ✅ |

These are proven results, not empirical approximations. A backend either computes the
correct answer or it does not.

---

## Repository Layout

```
src/
  frontend/
    snobol4/          SNOBOL4/SPITBOL lexer + parser → AST + IR
    snocone/          Snocone frontend (SC language, ~10 source files)
    rebus/            Rebus transpiler
    icon/             Icon frontend — ASM + JVM
    prolog/           Prolog frontend — ASM + JVM
  backend/
    c/                Portable C emitter (emit_byrd.c 2,709 lines · emit.c 2,220 lines)
    x64/              x86-64 NASM emitter (emit_byrd_asm.c 4,159 lines)
    jvm/              JVM Jasmin emitter (emit_byrd_jvm.c 4,051 lines · jasmin.jar)
    net/              .NET CIL emitter (emit_byrd_net.c 1,934 lines)
  driver/
    main.c            scrip-cc entry point — flag dispatch
  runtime/
    asm/              NASM macro library + runtime helpers
test/
  crosscheck/         106-program corpus + .ref oracle outputs
  sprint_asm/         ASM regression suite
  jvm_j3/             JVM sprint J3 smoke tests
  rebus/              Rebus round-trip tests
  smoke/              Quick sanity tests
artifacts/
  asm/
    beauty_prog.s     beauty.sno → x86-64 ASM (tracked canonical output)
    samples/
      roman.s         roman.sno → x86-64 ASM
      wordcount.s     wordcount.sno → x86-64 ASM
  jvm/                hello_prog.j · roman.j · wordcount.j
  net/                hello_prog.il
  c/                  Canonical C outputs
```

---

## The Five-Way Monitor

Active on the `asm-backend` branch: a parallel differential monitor that runs the
same SNOBOL4 program through all five participants simultaneously and compares trace
streams event-by-event via named FIFOs.

| # | Participant | Role |
|---|-------------|------|
| 1 | CSNOBOL4 2.3.3 | Primary oracle |
| 2 | SPITBOL x64 4.0f | Secondary oracle |
| 3 | SCRIP ASM backend | Compiled target |
| 4 | SCRIP JVM backend | Compiled target |
| 5 | SCRIP NET backend | Compiled target |

`monitor_ipc.so` — a LOAD'd C shared library — writes trace events to a per-participant
named FIFO, bypassing stdio entirely. The collector reads all five FIFOs in parallel.
The first line where any participant diverges from the oracle is the exact statement,
variable, and value where the bug fires. No bisecting. No guessing.

**Status (2026-03-21):** CSNOBOL4 ✅ · SPITBOL ✅ · ASM ✅ working in isolation.
JVM OUTPUT fast-path hook and NET emitter hook in progress — M-MONITOR-IPC-5WAY next.

---

---

## JavaScript Backend (In Progress — SJ-6)

`-js` produces a Node.js module runnable with `node prog.js`.

```bash
# JS backend
./scrip-cc -js program.sno -o prog.js
SNO_RUNTIME=src/runtime/js/sno_runtime.js node prog.js
```

**Status:** SJ-6 · feat suite **14/17 PASS** · emit-diff **1286/0**

| Feature | Status |
|---------|--------|
| Arithmetic, strings, control flow | ✅ |
| Pattern matching (LIT/ANY/SPAN/BREAK/ARB/ARBNO/BAL/…) | ✅ |
| Immediate capture (`$`) / conditional capture (`.`) | ✅ |
| Hello suite (hello, literals, INTEGER, UCASE, REMDR) | ✅ 4/4 |
| User-defined functions / DEFINE | 🔧 SJ-7 |
| INPUT line buffering | 🔧 SJ-7 |
| `run_invariants.sh` wiring | 🔧 SJ-7 |

### Pattern Engine — `sno_engine.js`

The JS pattern runtime (`src/runtime/js/sno_engine.js`, 532 lines) is an
iterative frame-based engine modelled after the Clojure implementation in
[`snobol4jvm`](https://github.com/snobol4ever/snobol4jvm). Frame state uses
Greek variable names matching the Clojure source:

```
Frame ζ = [Σ, Δ, σ, δ, Π, φ, Ψ]
  Σ/Δ — subject string + cursor on entry
  σ/δ — subject string + current cursor
  Π   — current pattern node
  φ   — child index (ALT/SEQ) or retry state
  Ψ   — parent frame stack
Ω     — backtrack stack
α     — current action signal (:proceed/:succeed/:fail/:recede)
λ     — current node type tag
```

Frames are **immutable plain JS arrays** — transitions create new arrays,
old ones are GC'd. No `memcpy`, no snapshot/restore, no arena. The GC *is*
the stack allocator.

### Benchmark: SCRIP vs SPITBOL

Corpus benchmarks (`corpus/benchmarks/snobol4/*.sno`) run head-to-head against
the SPITBOL x64 fork oracle (`snobol4ever/x64`). Each figure is the program's
own `TIME()` compute loop in milliseconds unless marked † (wall time; the
harness's TIME() capture truncates past its timeout). SCRIP is the mode-4
`--compile` native binary, gcc `-O0`. Measured 2026-07-10 (sandbox CPU —
absolute ms are not comparable to the 2026-06-24 table; the vs-SPITBOL ratio
is, since both engines ran on the same box each time). **All 16 benchmarks are
now green** — `roman`, `eval_fixed`/`eval_dynamic`, and `indirect_dispatch`,
excluded in June, all run ref-correct today. Lower is faster.

| Benchmark | SPITBOL | SCRIP native | vs SPITBOL | vs SPITBOL on 2026-06-24 |
|-----------|--------:|-------------:|-----------:|-------------------------:|
| indirect_dispatch | 5† | 10† | 2.1× slower | — (was excluded) |
| var_access | 1,318 | 6,680 | 5.1× slower | 64× |
| eval_fixed | 270 | 2,240 | 8.3× slower | — (was excluded) |
| func_call | 918 | 8,398 | 9.1× slower | 76× |
| func_call_overhead | 884 | 8,440 | 9.5× slower | 77× |
| op_dispatch | 117 | 1,405 | 12.0× slower | 53× |
| fibonacci | 171 | 2,259 | 13.2× slower | 66× |
| string_concat | 145 | 1,925 | 13.3× slower | 12× |
| table_access | 340 | 4,575 | 13.5× slower | 56× |
| arith_loop | 47 | 645 | 13.7× slower | 68× |
| string_manip | 651 | 11,386 | 17.5× slower | 54× |
| roman | 172 | 3,432 | 20.0× slower | — (was excluded) |
| mixed_workload | 170 | 15,539 | 91× slower | 41× |
| eval_dynamic | 480 | 82,443† | ~172× slower | — (was excluded) |
| string_pattern | 689 | 151,600† | ~220× slower | 21× |
| pattern_bt | 230 | 137,700† | ~599× slower | **2.0×** |

These are honest current numbers, and the story since the 2026-06-24 table
splits cleanly in two. **The scalar engine got dramatically better:** the
call/dispatch/variable/table cluster closed its gap 3–13× (direct-call
protocol DCR-1/2/3, the GVA register slab, RPF, table double-walk
elimination) — June's 40–77× band is now a 5–18× band. **The pattern engine
traded its crown jewel for correctness:** June's `pattern_bt` at 2.0× was
produced by the original FZ-optimized pattern engine at its peak, the day
before GZ#5 amputated it; SN4-PAT rebuilt patterns from scratch on the
defer-callout architecture — oracle-verified and far more complete (EVAL/CODE,
replacement splice, FENCE/ARBNO ladders all landed since) — but paying a
per-match `rt_zls_alloc(65536)`/release pair that the old engine never did.
The FZ-5 frozen-head inline was re-ported 2026-07-10 (eliminating the
per-match pattern fetch, ~4% on pattern_bt), which proved the fetch was never
the cost: **DEFER-ZLS** (killing the per-match arena churn) is the tracked
rung for the pattern tail, alongside the `-O0` build and the CSTACK-default
A/B. CSNOBOL4 column dropped pending re-measurement on current hardware.
Re-grounding headline claims stays tracked under REC-COV / RC-5.

## Prolog Benchmark — SCRIP vs GNU Prolog vs SWI-Prolog

The Prolog frontend is measured against the two mainstream native engines —
**GNU Prolog 1.4.5** (gprolog, a mature WAM-to-native compiler) and **SWI-Prolog 9.0.4**
(swipl) — on the community-standard **van Roy / Aquarius** performance suite
(`corpus/benchmarks/prolog/bench/*.pl`, UCB/CSD 89/50). All 22 programs reach **four-way
correctness consensus**: GNU, SWI, SCRIP mode-3 (`--run`, in-process x86 in an RX slab),
and SCRIP mode-4 (`--compile --target=x86` → `as`+`gcc` binary) all produce output
byte-identical to the gprolog-derived `.expected` signature.

Timing follows the van Roy methodology: each program's compute core is looped *N* times
in one process (so per-program compile amortizes to ~0 and we measure steady-state
per-iteration compute), best-of-*N* wall time, SCRIP binaries run with a pre-sized GC
heap to suppress collection-cycle noise. Figures are **per-iteration compute in
milliseconds** (`m4` is the mode-4 native binary, gcc `-O0`). Lower is faster; the ratio
is SCRIP-m4 against gprolog. Sorted fastest-relative first.

| Benchmark | GNU | SWI | SCRIP m4 | m4 vs GNU | m4 vs SWI |
|-----------|----:|----:|---------:|----------:|----------:|
| cal | 0.072 | 0.071 | 0.034 | **0.48× (faster)** | **0.48× (faster)** |
| sendmore | 4.288 | 10.297 | 4.286 | **1.00×** | **0.42× (faster)** |
| deriv | 0.036 | 0.036 | 0.073 | 2.02× | 2.03× |
| ops8 | 0.038 | 0.038 | 0.078 | 2.02× | 2.05× |
| times10 | 0.046 | 0.046 | 0.092 | 2.02× | 2.00× |
| divide10 | 0.038 | 0.038 | 0.078 | 2.03× | 2.05× |
| log10 | 0.037 | 0.037 | 0.076 | 2.03× | 2.05× |
| crypt | 0.541 | 0.778 | 1.256 | 2.32× | 1.61× |
| queens | 64.39 | 108.2 | 209.0 | 3.25× | 1.93× |
| queens_8 | 0.342 | 0.610 | 1.146 | 3.35× | 1.88× |
| tak | 12.03 | 21.16 | 41.73 | 3.47× | 1.97× |
| fib | 3.596 | 4.408 | 12.79 | 3.56× | 2.90× |
| query | 0.092 | 0.103 | 0.330 | 3.59× | 3.20× |
| ham | 0.281 | 0.158 | 1.031 | 3.67× | 6.53× |
| derive | 0.038 | 0.038 | 0.158 | 4.20× | 4.16× |
| queensn | 158.3 | 183.5 | 792.0 | 5.00× | 4.32× |
| nreverse | 0.079 | 0.078 | 0.406 | 5.14× | 5.21× |
| zebra | 2.305 | 2.304 | 13.65 | 5.92× | 5.92× |
| mu | 0.083 | 0.084 | 0.524 | 6.34× | 6.24× |
| qsort | 0.076 | 0.153 | 0.515 | 6.74× | 3.37× |
| nrev | 0.039 | 0.079 | 0.407 | 10.48× | 5.15× |
| meta_qsort | 0.750 | 0.521 | 9.017 | 12.03× | 17.3× |

**Geomean m4 vs GNU = 3.28×** (median 3.56×). Two structural facts:

- **mode-3 ≡ mode-4 on every program** — the in-process and compiled-binary paths share
  the GZ codegen and execute identically (the design invariant, confirmed empirically).
- **The gap tracks heap traffic, exactly as the inline-cell campaign (PL-DESCR) predicts.**
  Search- and atom-bound programs are at parity or faster (cal 0.48×, sendmore 1.00×);
  arithmetic-bound ones cluster near 2× (the symbolic-derivative set, crypt); recursion-bound
  near 3.5× (fib, tak, queens); the worst are list/structure-heavy (nrev 10.5×, qsort 6.7×,
  zebra 5.9×) and the meta-interpreter (meta_qsort 12×). That is the boxed-`Term*`
  compound-allocation tax — PL-DESCR-2 inlined *scalars* (ints/atoms/vars) into 16-byte cells,
  but compounds still hit the heap, which is precisely where these programs live. Against SWI,
  SCRIP is at parity or faster on a fair fraction (SWI is itself often slower than gprolog here).

These are honest current numbers, not the target. Being within ~3.3× geomean of a mature
native WAM, with the four-port boxed model and no first-argument indexing yet, is the
present standing; closing it is the PL-DESCR (inline compound cells, last-call optimization)
and first-argument-indexing work. Reproduce with `scripts/test_bench_prolog_4way.sh`
(correctness) over `corpus/benchmarks/prolog/bench/`.

---

## The Bootstrap Goal

The correctness target is self-hosting. Two gates:

**M-BEAUTIFY-BOOTSTRAP** — `beauty.sno` (the SNOBOL4 beautifier written in SNOBOL4)
reads itself and produces output identical to its input on all backends. A fixed point.

**M-COMPILER-BOOTSTRAP** — `compiler.sno` (the full compiler written in SNOBOL4)
compiles itself.

The JVM backend has already passed Rung 12: `beauty.sno` via the JVM backend produces
output byte-for-byte identical to the CSNOBOL4 oracle (M-JVM-BEAUTY ✅, commit
`b67d0b1` J-212). The other backends follow.

---

## The Development Story

SCRIP is co-authored by **Lon Jones Cherryholmes** and **Claude Sonnet 4.6**.

The sessions run like a buddy comedy: Lon arrives with an architectural insight or an
inconvenient bug, Claude writes the code, they argue about the right abstraction, one
of them is wrong, they figure out which one, the milestone fires, and Claude writes the
commit. Then they do it again, starting fresh with no memory of the previous session
except whatever made it into the docs.

The architecture has a name for that: the session log. Every session's mental state
at handoff is recorded in
[SESSIONS_ARCHIVE.md](https://github.com/snobol4ever/.github/blob/main/SESSIONS_ARCHIVE.md)
so the next Claude can pick up exactly where the last one left off. It is, in a way,
the compiler writing itself — one session at a time.

---

## Active Development

Sprint state lives in [snobol4ever/.github](https://github.com/snobol4ever/.github):

- **PLAN.md** — milestone dashboard, sprint state, session handoffs
- **ARCH-monitor.md** — five-way monitor design and sprint detail
- **SESSIONS_ARCHIVE.md** — full session history, append-only

**Current sprint:** G-10 · SJ-6 (SNOBOL4×JS) — engine complete, bench done, DEFINE/RETURN next.

---

## Collaborators

- **Lon Jones Cherryholmes** — compiler architecture, all backends, SCRIP lead
- **Jeffrey Cooper, M.D.** — snobol4dotnet, .NET MSIL target
- **Claude Sonnet 4.6** — scrip-cc co-author; every sprint, every Byrd box,
  every labeled goto — written in session, committed, pushed


---

## Source Volume (G-VOLUME · M-VOL-X ✅ · 2026-03-22)

> `wc -l` scan of `src/`. Generated artifacts (`.s` files, 36,890 lines across 28 files) excluded.
> Categories are logical function — comparable across SCRIP, snobol4jvm, snobol4dotnet.
> % of total = % of `src/` lines only.

| Category | Files | Lines | Blank-stripped | % total |
|----------|------:|------:|:--------------:|--------:|
| Parser / lexer | 20 | 6,368 | 5,728 | 20.5% |
| Code emitter | 11 | 17,291 | 15,936 | 55.6% |
| Pattern engine | 10 | 1,588 | 1,421 | 5.1% |
| Runtime / builtins | 7 | 4,614 | 4,120 | 14.8% |
| Driver / CLI | 1 | 140 | 128 | 0.5% |
| Extensions / plugins | 3 | 1,085 | 969 | 3.5% |
| Tests | 47 | 6,265 | 5,495 | — |
| Benchmarks | 12 | 1,603 | 1,541 | — |
| Docs / Markdown | 2 | 1,080 | 814 | — |
| **Total (src)** | **54** | **31,090** | **28,306** | **100%** |


---

## IR EKind — SNOBOL4 Operator Name Reference

Four-column reference: SIL/CSNOBOL4 proc name · MINIMAL/SPITBOL `o$` entry · functional name · current IR node.
Source authority: `snobol4-2.3.3/v311.sil` (CSNOBOL4) and `spitbol-docs/v37.min` (SPITBOL v3.7).

### Unary operators

| Syntax | SIL / CSNOBOL4 | MINIMAL / SPITBOL | Functional name | IR node |
|--------|---------------|-------------------|-----------------|---------|
| `+X` | `PLS` | `o$aff` — affirmation | numeric coerce / affirmation | `AST_PLS` → **`AST_PLS`** (unary plus; see note) |
| `-X` | `MNS` | `o$com` — complementation | arithmetic negation | `AST_MNS` |
| `\X` | `NEG` | `o$nta/b/c` — negation | logical negation (not) | `AST_NOT` |
| `?X` | `QUES` | `o$int` — interrogation | interrogation | `AST_INTERROGATE` |
| `@X` | `ATOP` | `o$cas` — cursor assignment | cursor position capture | `AST_CAPT_CURSOR` |
| `$X` | *(c$ind, inline)* | `o$inv` — indirection | indirection | `AST_INDIRECT` |
| `&X` | *(c$key, inline)* | `o$kwv` — keyword reference | keyword reference | `AST_KEYWORD` |
| `*X` | *(c$def, inline)* | *(c$def, no o$ entry)* | deferred expression | `AST_DEFER` |
| `.X` | *(unary, via NAM)* | `o$nam` — name reference | name reference (unary) | `AST_NAME` |

**Note on `AST_PLS` vs `AST_PLS`:** SIL `PLS` and MINIMAL `o$aff` are the same operation.
The IR currently has both `AST_PLS` and `AST_PLS` with identical semantics — one must be removed.
Decision: `AST_PLS` is the canonical name (matches SIL); `AST_PLS` is deprecated.

### Binary operators

| Syntax | SIL / CSNOBOL4 | MINIMAL / SPITBOL | Functional name | IR node |
|--------|---------------|-------------------|-----------------|---------|
| `X + Y` | `ADD` | `o$add` — addition | addition | `AST_ADD` |
| `X - Y` | `SUB` | `o$sub` — subtraction | subtraction | `AST_SUB` |
| `X * Y` | `MPY` | `o$mlt` — multiplication | multiplication | `AST_MUL` |
| `X / Y` | `DIV` | `o$dvd` — division | division | `AST_DIV` |
| `X ! Y` | `EXPOP` | `o$exp` — exponentiation | exponentiation | `AST_POW` |
| `X Y` (blank, value ctx) | `CONCAT` | `o$cnc` — concatenation | string concatenation | `AST_CAT` |
| `X Y` (blank, pattern ctx) | *(BINCON path, CONCL)* | *(c$cnc type, Byrd wiring)* | goal-directed pattern sequence | `AST_SEQ` |
| `X \| Y` | `OR` / `ORPP` | `o$alt` — alternation | pattern alternation | `AST_ALT` |
| `X ? Y` | `SCAN` | `o$pmv/pmn/pms` — pattern match | pattern match / scan | `AST_SCAN` |
| `X = Y` | `ASGN` | `o$ass` — assignment | assignment | `AST_ASSIGN` |
| `X . Y` | `NAM` | `o$pas` — pattern assignment | conditional capture (on match) | `AST_CAPT_COND_ASGN` |
| `X $ Y` | `DOL` | `o$ima` — immediate assignment | immediate capture | `AST_CAPT_IMMED_ASGN` |
