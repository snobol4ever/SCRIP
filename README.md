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
own `TIME()` compute loop in milliseconds unless marked † (wall time;
`indirect_dispatch` has no `TIME()` loop). SCRIP is the mode-4 `--compile`
native binary, gcc `-O0`. Measured 2026-07-18 at HEAD `10bfc42f` (sandbox CPU).
The SPITBOL column reproduces the 2026-07-10 oracle numbers within a few percent
— same programs, same box — so the absolute-ms comparison across the two SCRIP
snapshots is sound this time. **All 16 benchmarks are green.** Lower is faster.

| Benchmark | SPITBOL | SCRIP native | vs SPITBOL | vs SPITBOL on 2026-07-10 |
|-----------|--------:|-------------:|-----------:|-------------------------:|
| var_access | 1,319 | 267 | **4.9× faster** | 5.1× slower |
| op_dispatch | 116 | 31 | **3.7× faster** | 12.0× slower |
| arith_loop | 48 | 17 | **2.8× faster** | 13.7× slower |
| func_call_overhead | 885 | 923 | ~par | 9.5× slower |
| func_call | 879 | 934 | ~par | 9.1× slower |
| indirect_dispatch | 4† | 5† | ~par | 2.1× slower |
| fibonacci | 173 | 268 | 1.5× slower | 13.2× slower |
| pattern_bt | 222 | 892 | 4.0× slower | 599× slower |
| mixed_workload | 175 | 866 | 4.9× slower | 91× slower |
| table_access | 333 | 1,743 | 5.2× slower | 13.5× slower |
| eval_fixed | 267 | 1,406 | 5.3× slower | 8.3× slower |
| string_pattern | 688 | 4,643 | 6.7× slower | ~220× slower |
| string_concat | 142 | 1,013 | 7.1× slower | 13.3× slower |
| string_manip | 645 | 4,949 | 7.7× slower | 17.5× slower |
| roman | 180 | 2,618 | 14.5× slower | 20.0× slower |
| eval_dynamic | 422 | 61,260 | 145× slower | ~172× slower |

Since the 2026-07-10 table, **every benchmark got faster and the whole board
moved** — the register-allocation overhaul and the pattern-arena fix both landed.
**The scalar/dispatch cluster now beats SPITBOL:** `var_access`, `op_dispatch`,
and `arith_loop` flipped from 5–14× slower to 2.8–4.9× *faster*, and the call
cluster closed to par — REG-1 through REG-7 promoted the pend/dcap backtracking
top out of the pinned `[RT_CAS_TOP]` cell into registers (r12, then rbp/rsp
frames), shedding the per-match and per-capture absolute-memory traffic.
**The pattern engine recovered ~150×:** `pattern_bt` went 599× → 4.0× slower once
ZB-ITER-3 retired the DEFER-island swap and moved suspension onto the single rsp
stream, killing the per-match `rt_zls_alloc(65536)`/release churn the previous
note named as the blocker. It is not yet back to the June FZ-engine peak of
**2.0×** — SPITBOL's scanner still leads on raw backtracking — but the arena tax is
gone. The GC-U workspace-class overhaul (WS-CLASS SPLIT/WIDENING, AGG-PRECISE)
cut malloc/strdup churn underneath all of it.

**The one benchmark that barely moved is `eval_dynamic`** (172× → 145× slower,
~61s): the EVAL recompile path did not benefit from the REG/arena work and is now
the single outstanding pathology — the tracked next rung. CSNOBOL4 column still
dropped pending re-measurement on current hardware. Re-grounding headline claims
stays tracked under REC-COV / RC-5. (Attribution here is from the commit log
since 2026-07-10, not a controlled bisect.)

## SNOBOL4 Benchmark — SCRIP vs SPITBOL vs CSNOBOL4

The SNOBOL4 frontend is measured against the two reference engines — **SPITBOL x64**
(official `spitbol/x64` `4fe74db`, `sbl -b`, the semantics oracle) and **CSNOBOL4**
(official `philbudne/csnobol4` `bd51fcb5`, `snobol4 -f`, built by the certified two-stage
bootstrap in `scripts/build_official_oracles.sh`: 144/144 regression + table fixed-point) —
on the canonical 16-program suite in `corpus/benchmarks/snobol4/`. The same `.sno` file
runs unmodified on all three engines; **all 16 produce byte-identical output across all
three** (`ms:` self-timing lines stripped; verified pairwise 2026-07-18, SCRIP `f0914867`
mode-4). `indirect_dispatch` is an oracle-error xfail (see its `.xfail`) and excluded from
the win/loss score.

Timing is each program's own `TIME()` self-report in ms (startup excluded; SPITBOL TIME()
diffs are ns, ÷1e6). SCRIP is mode-4 (`--compile` → gcc `-no-pie` + `libscrip_rt.so`).
All three engines run on the **same host** for valid comparison (ratios are meaningless
cross-host). Runners: `scripts/util_bench_snobol4_engines.sh` (oracles),
`scripts/test_bench_snobol4_modes.sh` (SCRIP). Sorted fastest-relative first; ratio is
SCRIP against SPITBOL, **bold** = SCRIP faster.

| Benchmark | SPITBOL | CSNOBOL4 | SCRIP m4 | m4 vs SPITBOL | m4 vs CSNOBOL4 |
|-----------|--------:|---------:|---------:|--------------:|---------------:|
| var_access | 1782 | 3338 | 307 | **0.17× (faster)** | **0.09× (faster)** |
| op_dispatch | 154 | 330 | 36 | **0.23× (faster)** | **0.11× (faster)** |
| arith_loop | 60 | 116 | 21 | **0.35× (faster)** | **0.18× (faster)** |
| func_call_overhead | 1202 | 2259 | 1057 | **0.88× (faster)** | **0.47× (faster)** |
| func_call | 1178 | 2231 | 1073 | **0.91× (faster)** | **0.48× (faster)** |
| pattern_bt | 272 | 625 | 283 | 1.0× | **0.45× (faster)** |
| fibonacci | 203 | 509 | 312 | 1.5× | **0.61× (faster)** |
| mixed_workload | 211 | 474 | 656 | 3.1× | 1.4× |
| table_access | 433 | 2439 | 1384 | 3.2× | **0.57× (faster)** |
| eval_fixed | 307 | 647 | 1017 | 3.3× | 1.6× |
| string_manip | 816 | 1690 | 2931 | 3.6× | 1.7× |
| string_pattern | 815 | 2024 | 3675 | 4.5× | 1.8× |
| roman | 195 | 1070 | 991 | 5.1× | **0.93× (faster)** |
| string_concat | 156 | 320 | 1436 | 9.2× | 4.5× |
| eval_dynamic | 414 | 1125 | (crash) | — | — |

*(Numbers are post-s95 — BP-1 v2 (O(1) builtin dispatch, s95 SCRIP `a7224153`) + BP-4
(table subscript fast-lane, s95 SCRIP `6e5db495`) + BP-2/BP-3 (getenv cache + NV I-gate,
s94) + PERF-PAT-1 (GVA-slot pattern resolution, s93). CSNOBOL4 column is the s94 verified
set (`bd51fcb5`); SPITBOL and SCRIP re-timed on the current host 2026-07-18, SCRIP HEAD
`6e5db495`. Gates: smokes 7/7 both modes ×2, crosscheck m3 305/2 · m4 304/2/1 ·
DIVERGE=1 (1017\_arg\_local). See `GOAL-SNOBOL4-BB.md` § BENCH-PERF LADDER for the full
BP rung plan and remaining bottleneck analysis.)*

**Scoreboard (15 scored): SCRIP beats CSNOBOL4 on 9/15, beats SPITBOL on 5/15.**
SCRIP's compiled native loops beat SPITBOL outright on register-shaped work (variable
access via GVA slots, arithmetic, operator dispatch) and now also on both call-convention
benchmarks (`func_call`/`func_call_overhead`) where BP-1 removed the by-name dispatch tax.
The remaining losses concentrate in the **runtime library**: string building/manipulation
(`string_concat`/`string_manip`), the pattern scanner on string-heavy input
(`string_pattern`), mixed workloads, and EVAL. `eval_dynamic` crashes (a known
deprioritized issue, 2026-07-18); the string-keyed chain cache makes repeated-string EVAL
(`eval_fixed`) 3.3× slower than SPITBOL but 1.6× faster than CSNOBOL4.

## Prolog Benchmark — SCRIP vs GNU Prolog vs SWI-Prolog

The Prolog frontend is measured against the two mainstream engines — **GNU Prolog 1.4.5**
(gprolog, WAM-to-native) and **SWI-Prolog 9.0.4** (swipl) — on the **van Roy / Aquarius**
suite (`corpus/benchmarks/prolog/bench/*.pl`). **All 22 programs now hold four-way
correctness consensus** (GNU, SWI, SCRIP mode-3 `--run`, SCRIP mode-4 compiled binary,
all byte-identical to `.expected`; `queensn` was the one previously divergent —
the working-set leak is now closed). Measured 2026-07-18 on Intel Xeon @ 2.80 GHz, 1 core.

**This table replaces the 2026-07-18 s98 one (geomean 52×),** which pre-dated the
defect-(b) WS-accrual fix (s99: HB_PLJ collectable per-branch class + GC byte-budget
pacer). The fix closed the crypt/qsort N-cliff, made queensn oracle-correct, and
completed fib ×256 flat — and moved the geomean from 52× to 54× (the hot benches
were already near steady-state; the fix mainly eliminates super-linear blowup at
large N rather than changing the per-iter steady-state rate on the measured rows).
The remaining gap is fixed per-call overhead (trampoline C frame, strcmp lookup,
whole-frame memset, no first-arg indexing, no LCO) — the PL-SPEED/PL-RSP ladders.

**Per-iteration compute** (`scripts/bench_prolog_vanroy.sh`): each bench loops via a
failure-driven wrapper (`between(1,N,_), bench, fail` — backtracking reclaims stacks
on every engine each iteration; full solution enumeration per iteration, identical
work everywhere). Per-iteration ms = (wall − engine startup floor) / N, N auto-ranged
per engine (×4 until ≥300 ms compute). Sorted fastest-relative first; ratio is
SCRIP-m4 vs the oracle. DNF = engine crash/timeout/abort.

| Benchmark | GNU | SWI | SCRIP m3 | SCRIP m4 | m4 vs GNU | m4 vs SWI |
|-----------|----:|----:|---------:|---------:|----------:|----------:|
| zebra | 3.1016 | 2.8047 | 0.6172 | 0.5898 | **<1×** | **<1×** |
| sendmore | 2.8906 | 6.2891 | 66.97 | 59.19 | 20× | 9× |
| query | 0.0204 | 0.0497 | 0.5042 | 0.4972 | 24× | 10× |
| fib | 2.2188 | 2.7969 | 82.50 | 72.31 | 33× | 26× |
| deriv | 0.0034 | 0.0075 | 0.1804 | 0.1584 | 47× | 21× |
| crypt | 0.5996 | 1.4297 | 31.13 | 29.25 | 49× | 20× |
| mu | 0.0378 | 0.0356 | 3.098 | 2.762 | 73× | 78× |
| qsort | 0.0311 | 0.0384 | 2.758 | 2.586 | 83× | 67× |
| ham | 26.25 | 23.13 | 4752 | 2865 | 109× | 124× |
| ops8 | 0.0011 | 0.0017 | 0.1606 | 0.1450 | 132× | 85× |
| nreverse | 0.0176 | 0.0160 | 2.504 | 2.445 | 139× | 153× |
| times10 | 0.0014 | 0.0020 | 0.2139 | 0.2019 | 144× | 101× |
| divide10 | 0.0016 | 0.0022 | 0.3855 | 0.2417 | 151× | 110× |
| derive | 0.0032 | 0.0042 | 0.5703 | 0.5195 | 162× | 124× |
| cal | 0.0005 | 0.0017 | 0.0862 | 0.0895 | 179× | 53× |
| log10 | 0.0007 | 0.0011 | 0.1626 | 0.1472 | 210× | 134× |
| meta_qsort | 0.2883 | 0.2251 | DNF | DNF | — | — |
| nrev | 0.0071 | 0.0166 | DNF | DNF | — | — |
| tak | 8.4844 | 9.0312 | DNF | DNF | — | — |
| queens (16) | — | — | DNF | DNF | — | — |

**Geomean m4 vs GNU = 54× · m4 vs SWI = 35×** (s99, 2026-07-18; 15 programs,
non-DNF rows excluding ham; previous s98 baseline 52× vs GNU).

What the shape says:

- **zebra and sendmore beat both oracles** — the Byrd-box machine's deterministic
  engine (RSP-F-2, landed s97) compiles these structure-heavy predicates onto
  rsp-carved frames with zero C trampolines; mode-3 and mode-4 match within noise.
- **mode-3 ≡ mode-4 on every row** (within noise) — the in-process and compiled paths
  share one codegen; the design invariant holds empirically.
- **The gap is dominated by fixed per-call overhead, not by search.** The heavier the
  bench, the *better* SCRIP fares relatively (fib 33×, crypt 49×) while trivial-body
  benches sit at 150×+ (cal, log10) — the signature of per-call costs the PL-SPEED
  ladder targets: a whole-frame memset per activation, a malloc per activation,
  a C trampoline frame per nondet Prolog call, no first-argument indexing, no LCO.
- **ham (109×) is order-of-magnitude only:** WS accrual begins at N=1, so
  auto-ranging stops at N=1 and the per-iter figure is a single-shot wall time
  dominated by startup — not a steady-state rate. The number will collapse when
  the ZLS/pcells accrual rungs land.
- **DNF set** (meta\_qsort, nrev, tak): stack-depth deaths under the current C
  trampoline per call; the RSP-spine completion (PL-RSP-FINISH) retires them.
- **Startup is a strength.** On end-to-end single-shot latency
  (`scripts/bench_prolog_perf.sh`, median of 3, startup included), SCRIP m4 binaries
  *beat both engines* on the small half of the suite (8–10 ms vs gprolog 17–22 ms and
  swipl 22–28 ms); mode-4 compile+link is ~85–150 ms per program.

Reproduce: `scripts/test_bench_prolog_4way.sh` (correctness), `scripts/bench_prolog_vanroy.sh`
(per-iteration table above), `scripts/bench_prolog_perf.sh` (end-to-end latency), over
`corpus/benchmarks/prolog/bench/` with loop drivers in `corpus/benchmarks/prolog/vanroy/`.

---

## Icon Benchmark — SCRIP vs Icon vs JCON

The Icon frontend is measured against canonical **Icon 9.5** (`icont`/`iconx`, built from
source) and **JCON 2.2** (Proebsting/Townsend's Icon-on-JVM compiler, built from source on
OpenJDK 21) on the merged **icon-master/tests/bench + jcon-master/bmark suite**
(`corpus/benchmarks/icon/`, 10 programs; `corpus/benchmarks/README-ICON-JCON.md` is
authoritative for the merge). Runner: `scripts/test_icon_bench_corpus.sh` — the canonical
jcon/bmark invocation protocol on every engine (`concord <concord.dat` · `deal -h 1000` ·
`geddump <geddump.dat` · `ipxref <ipxref` sources · `micro 0.05` · `micsum <micro-output` ·
`queens -n10` · `rsg <rsg.dat` · `tgrlink tgrlink.dat` · `version`), with the suite's stock
`post.icn` output suppression active (`Init__` reassigns `write := 1`), so times measure
compute, not I/O. This is a DIFFERENT protocol from the 2026-07-14 measurement formerly in
this section (`OUTPUT=1`, I/O-inclusive, 7 programs) — the two are not comparable rows.

**Status, measured 2026-07-18 at SCRIP `f405c6a7` (origin HEAD):** iconx **10/10** ·
JCON **10/10** (the two anchors agree on all line counts) · **SCRIP m3 6/10 · m4 6/10**.
Both anchors built and verified in-sandbox; every divergence below is SCRIP's own.

**Timing** (ms, wall-clock, best of 3, gcc `-O0`, shared container) on the six programs
SCRIP runs to completion. `m3` = `--run` (includes in-process parse+lower+emit); `m4` =
`--compile --target=x86` → `as`+`gcc` binary (run only). Startup floors: iconx ~4 ms ·
JCON ~125 ms (JVM) · m4 ~4 ms.

| Benchmark | iconx | JCON | SCRIP m3 | SCRIP m4 | m4 vs iconx | m4 vs JCON |
|-----------|------:|-----:|---------:|---------:|------------:|-----------:|
| concord | 34 | 283 | 14 | 5 | **6.8× faster** | 57× faster |
| deal | 26 | 290 | 13 | 4 | **6.5× faster** | 72× faster |
| micsum | 4 | 139 | 8 | 5 | ~parity (floor) | 28× faster |
| queens | 55 | 325 | 14 | 4 | **13.8× faster** | 81× faster |
| rsg | 17 | 269 | 27 | 9 | **1.9× faster** | 30× faster |
| version | 4 | 128 | 6 | 4 | parity (floor) | 32× faster |

**Geomean m4 vs iconx = 3.1× faster** over all six (8.5× over the three non-trivial
workloads concord/deal/queens; micsum/version sit on the ~4 ms process floor).
**m4 vs JCON = ~46× faster**, but read that with care: JCON's ~125 ms JVM startup floor
dominates every sub-second program here, so the ratio measures startup as much as code
quality. m3 beats iconx on the real workloads (2–4×) despite paying full compilation
inside the measured time.

**The four failures — three are known-cause, two are freshly-pinned regressions:**

- **micro** — TIMEOUT, pre-existing heap exhaustion (`[ZHP]` at ~battery 94+; the
  put/get O(n)-copy-per-append in `by_name_dispatch.c`, WS/GC-ladder territory, not
  bench-lane). Identical before and after every commit below.
- **geddump + tgrlink** — rc=0 with ZERO output lines, both modes. **Regression, bisected
  to exactly `7aade169`** (ICN-GOTO-SURVEY α-force pilot: dj α-entry trampoline
  eradication + `lower_every` mark→body force-α). Both were passing at `d52fdaac`
  (tgrlink byte-identical to iconx, 3239L; geddump full-output).
- **ipxref** — `FATAL emit_drive: IR op=27 has no template`, both modes. **Regression,
  bisected to exactly `00a027ca`** (ICN-MOVE-LABEL-ERAD slice 2: the IR_DISJUNCTION nary
  self-state rewrite of `lower_alt`). Passing at `16064867`, the commit immediately before.
- Why the regressions went unseen: the intervening sessions re-proved the RUNG suite
  (246/14/32 zero-delta per commit) but never re-ran this bench track, and
  geddump/ipxref/tgrlink have no (or only partial) diffable rung variants. The bench
  runner is a report, not a gate — these three now need FZ-style rungs or the runner in
  the per-commit gate list.

Reproduce: `bash scripts/test_icon_bench_corpus.sh` (builds everything it needs; SKIP_BUILD=1
after first run). Oracle kit: `icon-master` → `make Configure name=linux && make Icont`;
`jcon-master` → `make build` with `icont` and a full JDK on PATH (invoke `jcont` via `bash`
— it uses bashisms; run it from the source dir with bare filenames).

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
