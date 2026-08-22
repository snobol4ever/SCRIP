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
apt-get install -y nasm default-jdk

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

### Benchmark: SCRIP vs SPITBOL — 2026-08-13 s68, SCRIP `b3a57714`

Benchmark corpus (`corpus/benchmarks/snobol4/*.sno`, 23 programs) vs the SPITBOL
x64 oracle (`sbl -b`). Program-reported `TIME()` ms both sides — semantics
verified to match rather than assumed: the manual (p.242) defines `TIME()` under
Unix as time spent computing, excluding I/O wait, and SCRIP's is
`clock()/CLOCKS_PER_SEC`, so both are CPU time. SCRIP = mode-4 `--compile`,
`SCRIP_FN_RBP=1` (the standing RBP-FUNCTION debug mode). **MIN-OF-5**, spread
reported per row. ⚠ NOT run under `setarch -R` (the s22l ±2-program ASLR caveat
applies). **ratio > 1.00x = SCRIP faster.**

⛔ **`RT_OPT=-O0`** — the Makefile default (FACT RULE O0-DEV). The runtime `.so`
is UNOPTIMIZED in these numbers. This is not a footnote: see the reading below.

Suite state this HEAD, m4, graded against the LIVE oracle: **16 PASS / 7 SIG11
of 23.** Timing rows exist only for the 16 that pass — a crashing program's
time is not a measurement.

| benchmark | SPITBOL (ms) | SCRIP (ms) | ratio | spread |
|---|---|---|---|---|
| func_call_overhead | 875 | 229 | **3.82x** | 4.3% |
| func_call | 864 | 230 | **3.75x** | 3.4% |
| op_dispatch | 116 | 32 | **3.62x** | 6.2% |
| fibonacci | 169 | 47 | **3.59x** | 4.2% |
| var_access | 1281 | 379 | **3.37x** | 7.9% |
| arith_mixed | 2111 | 660 | **3.19x** | 6.3% |
| arith_int | 4256 | 1371 | **3.10x** | 4.5% |
| arith_loop | 43 | 14 | **3.07x** | 0% |
| arith_str | 312 | 385 | 0.81x | 5.4% |
| pattern_bt | 222 | 198 | 1.12x | ⚠ 29.7% |
| pattern_bt_deep | 3614 | 3547 | ~1.02x | (n=3) |
| string_concat | 137 | 8 | 17.12x | ⚠ 75% |
| string_manip | 653 | 1876 | 0.34x | ⚠ 50.6% |
| table_churn | 318 | 1081 | 0.29x | ⚠ 129% |
| table_access | 335 | 1163 | 0.28x | ⚠ 142% |

(`cap_imm_nret`/`cap_imm_nret2`, `eval_dynamic`, `eval_fixed`, `mixed_workload`,
`roman`, `string_pattern` = SIG11, no row. `indirect_dispatch` passes but has no
`TIME()` loop.)

⚠ **The five flagged rows exceed `bench_min_of_n.sh`'s own SPREAD_WARN=15% and
are NOT point estimates.** `table_access`/`table_churn` spread >100% means min
and max differ by more than 2× on one binary — allocation/GC behaviour, not a
stable figure. Quoting them as a multiplier would be quoting noise.

**Geomean: 2.92x over the nine clean rows; 3.43x over arithmetic/call/var-access;
1.83x over all fifteen.**

**The split tracks EMITTED CODE vs RUNTIME C, and `-O0` sits on exactly one side
of it.** Arithmetic, calls, variable access and dispatch — all emitted x86 — run
3.1–3.8× faster. Tables and string building — all work done inside
`libscrip_rt.so` — run 3–3.6× slower, and that `.so` is the object compiled at
`-O0`. ⛔ **The losing cluster is therefore CONFOUNDED and must not be read as a
design verdict until re-measured with `RT_OPT="-O2 …"`.** That build is
perf-only, explicit opt-in (O2-DIRECTED-ONLY).

⛔ **Do not quote a single headline multiplier.** Spread runs 0.28×–3.8× on
trustworthy rows. Defensible summary: "≈3× faster on scalar, dispatch and call
work; at parity on pattern backtracking; slower on aggregate/string building at
an `-O0` runtime; EVAL, REPLACE-family and deferred-capture programs do not run."

**Movement vs the 2026-08-09 pass (`26cbd86a`), named not hidden.** The DEFINE
call protocol INVERTED and is now the strongest family: `func_call` 0.49× →
3.75×, `func_call_overhead` 0.52× → 3.82×, `fibonacci` 0.37× → 3.59× — the
TINY-REAL/SIG linkage rungs (s57/s58/s66) landing. Against that, the passing set
moved 20 → 16: `cap_imm_nret`, `cap_imm_nret2`, `eval_fixed` and
`mixed_workload` ran on 08-09 and SIG11 now, joining `roman`, `string_pattern`
and `eval_dynamic`. The seven sort into exactly three shapes — REPLACE-form
match (`string_pattern`, `mixed_workload`, `roman`), EVAL (`eval_dynamic`,
`eval_fixed`), and deferred/immediate capture (`cap_imm_nret{,2}`) — with clean
controls on both sides: DEFINE-with-recursion-but-no-match passes 4/4, and bare
matching with alternation, SPAN and `.` capture passes 2/2. Note this window
spans the RBP eradication (s51/s52) and the RBP-FUNCTION re-introduction (s63),
so the two passes are not the same machine.


### Demo suite: SCRIP vs SPITBOL — 2026-08-09 s34, HEAD `a5c2264` (counter-loop rail)

> ⚠ **NOT REPRODUCIBLE AT HEAD (s154, 2026-08-19).** `claws5-match` and `claws5-match-fence`
> **SIGSEGV in m3 at this HEAD** — the originals in `corpus/programs/snobol4/demo/` do too, on both
> the 66 KB workload and the 1 KB smoke input — so this table's two claws5 rows (and the s128
> match-only grid's `claws5-match` `0.195 ms/match` row below) describe programs that no longer run.
> Re-measured time-based, the four rows that DO run read: treebank-match **0.89×** (was 1.05×),
> treebank-match-fence **1.04×** (was 1.57×), calculator-1-match **1.52×** (was 1.33×),
> calculator-1-match-fence **0.65×** (was 0.98×). Every row that fell by a third is a FENCE row:
> this table's own commentary records FENCE buying SCRIP +49% on treebank (1.57 vs 1.05), and it
> now buys **0%** on all three grammars — the window `a5c2264` → HEAD contains a FENCE regression.
> Kept as the historical record, per this file's convention for superseded grids. See
> `.github/FINDING-2026-08-19-s154-bm4-workload-family-fence-is-inert-on-scrip.md`.

The six pattern demos that are IDENT to the oracle in BOTH modes, re-measured after
MV-BASE + EXPR-CLASSP.  Loop is COUNTER-DRIVEN (`BN.I = BN.I + 1` / `LT(BN.I,N)`),
NOT the historical `reps LEN(1) =` shape — that shape SEGVs SCRIP at N=1 (REPLACE
positive-home family; 4-line witness, same class as `roman`/`string_pattern`), so
the standard rail harness cannot run these programs at this HEAD.  `TIME()` brackets
the match loop only: process startup, dynamic link and pattern-blob compile excluded.
**ratio > 1.00x = SCRIP faster.**  RT `-O0`, single round, no `setarch -R`.

| demo | N | SPITBOL | SCRIP m4 | ratio |
|---|---:|---:|---:|---:|
| claws5-match | 2000 | 503 ms | 312 ms | **1.61x** |
| claws5-match-fence | 2000 | 549 ms | 326 ms | **1.68x** |
| treebank-match | 2000 | 1391 ms | 1330 ms | **1.05x** |
| treebank-match-fence | 2000 | 1652 ms | 1053 ms | **1.57x** |
| calculator-1-match | 200 | 8881 ms | 6655 ms | **1.33x** |
| calculator-1-match-fence | 2000 | 1805 ms | 1838 ms | 0.98x |

**WINDOW ADEQUACY PROVED BY CONVERGENCE, not asserted.**  The same six were run at
N=400 first: 1.56 / 1.67 / 1.02 / 1.51 / 1.36 / 0.99.  A 5x scale-up moved no ratio
by more than 0.06, so these are window-independent.  (Contrast: a wall-clock pass over
the unlooped programs — 3–50 ms windows — gave 1.00 / 0.50 / 0.36 / 0.80 / 1.11 / 0.57,
i.e. the sign INVERTED on four of six.  The 07-24 variance postmortem below predicted
exactly that; sub-second wall windows measure the loader, not the emitted code.)

**MOVEMENT vs the 07-24 rail table below** (that table's ratio is scrip÷sbl, LOWER
faster; inverted here to the common convention):

| demo | 07-24 `a0b9aa41` | 08-09 `a5c2264` | change |
|---|---:|---:|---|
| claws5-match | 1.39x | 1.61x | +16% |
| treebank-match | 0.37x | 1.05x | **2.8x** |
| calculator-1-match | 0.47x | 1.33x | **2.8x** |

⛔ **WHY WE ARE FASTER — HYPOTHESIS, NOT A MEASUREMENT.**  The 07-24 entry attributes
its gap to "emitted-code shape (δ-seam traffic, **ARBNO chain**, branch-chain Σ-pop)"
with the C runtime measured at ~2% of hot path.  The two demos that moved 2.8x
(treebank, calculator-1) are the ARBNO-heavy pair; claws5, which is not ARBNO-dominated,
moved only 16%.  ARB-LON-K16 / ARBNO-LON (`c7a276f6`, 08-06) replaced exactly that chain
with the frameless σ/Δ0 form — no frame, no counter, no links.  The correlation is
suggestive and the mechanism is named, but NOTHING HERE IS BISECTED: the intervening
window also contains SEQ-ERAD, FENCE-SEMANTICS, LADDER PB (PATREF deletion), the OS-2
slices, ZD-8 STFH-CARVE and GAMMA-EXIT.  **To convict ARBNO-LON, A/B `SCRIP_ARBNO_K16=0`
at this HEAD on treebank-match** — the killswitch exists and is byte-identical-inert.
Until someone runs it, "the ARBNO chain died" is a lead, not a finding.

Second signal, unexplained: **both fence variants beat their non-fence siblings on the
SCRIP side** (1.68 vs 1.61; 1.57 vs 1.05) while SPITBOL shows the opposite ordering on
treebank (1652 ms fenced vs 1391 ms unfenced).  FENCE is pruning backtracking SCRIP
would otherwise pay for, and pruning more of it than SPITBOL's does.  `treebank-match`
at 1.05x is the outlier worth attacking: its fence sibling gets 1.57x on nearly identical
work, so the residue is unpruned backtracking, not raw match speed.

⛔ Coverage: these are the SIX of fifteen working-set demos that are IDENT both modes.
Base `claws5` / `treebank-list` / `treebank-array`, `json-match` ×2 and `calculator-2` ×3
do NOT run at this HEAD — no ratio exists for them and none may be quoted.

### Demo suite: SCRIP vs SPITBOL — the measurement rail (2026-07-24, HEAD `a0b9aa41`)

The five pattern/eval demos (`corpus/programs/snobol4/demo/*-match.sno`) measured on
the **rail protocol** (`scripts/bench_sno_rail.sh`, s147): the bench loop times
**itself in-program** (`TIME()` delta around the match loop only — process startup,
dynamic link, and pattern-blob compile are excluded *exactly*, no floor estimation),
and the iteration count **auto-ranges ×4 per engine until the compute window ≥ 800 ms**
(cap 16384) — the Van Roy Prolog rail's adequacy rule. 5 interleaved rounds, medians.
Oracle = official SPITBOL x64 (`sbl -b`); byte-identity checked per demo
(timing lines stripped). **Ratio = scrip ÷ sbl per-iteration; lower is faster.**

| Demo | sbl µs/iter | SCRIP m4 µs/iter (RT `-O0`) | ratio | RT `-O2` sinks | ratio |
|------|------------:|----------------------------:|------:|---------------:|------:|
| claws5 | 261 | 188 | **0.72** | 174 | **0.67** |
| json | 2,199 | 1,562 | **0.71** | 1,481 | **0.67** |
| calculator-1 | 46,969 | 99,000 | 2.11 | 95,250 | 2.03 |
| treebank | 702 | 1,915 | 2.73 | 1,891 | 2.69 |
| calculator-2 | 734 | 2,602 | 3.54 | 2,342 | 3.19 |

**Headline:** with valid measurement, **claws5 and json both beat SPITBOL ~1.4×**
(0.67–0.72); treebank is 2.7× slower, calculator-1 2.1×, calculator-2 3.2–3.5×.
Remaining gap is emitted-code shape (δ-seam traffic, ARBNO chain, branch-chain
Σ-pop), per the SLOT-ELIDE attribution result (C runtime ≈ 2% of hot path).

**Variance postmortem — why the earlier fixed-N wall grids were retracted.** A full
day of fixed-N wall-clock measurements (windows 3–80 ms) swung treebank 1.42→3.90
and showed claws5 as 1.1–2.0× *slower*, while calculator-1 — the only demo whose
window exceeded ~2 s — held 1.96–2.11 across every condition. Sub-second wall
windows on a shared host are dominated by startup + load noise; the rail's
self-timing + window-adequacy rule fixes both. The claws5 verdict *inverted*
(slower → 1.4× faster) once startup was excluded — the cautionary example.

**RT `-O2` is retired (2026-07-24 ruling).** Two independent measurements: a
same-binary `.so`-swap A/B and the rail's dual-config columns above both put the
runtime-sink `-O2` effect at a uniform ~3–10% — real but incapable of moving a
2–3× ratio, consistent with the C runtime being ≈ 2% of hot-path cost. `-O2` was
scoped to the **runtime sinks only** (`runtime/{rt,core,builtins}`, 96 objects);
the emit templates (`src/templates/*.cpp`, 118 objects) stay `-O0` always — they
run at *compile* time (and EVAL/CODE JIT), never in the match hot path, and
dominate `.so` build time. Forward plan: the C runtime is to be replaced by a
hand-written **x86 ASM runtime**, optimal for mode 3 and mode 4 alike — no C
compiler, no opt level, in the runtime at all.

## ⭐ Performance — SCRIP vs SPITBOL (2026-08-22 s253, current)

Measured at SCRIP `7dd59a06`, on the Milestone 1 line. Engines: **SPITBOL x64 `sbl -bf`**
(the `-f` arm is the only one matching SCRIP's case-sensitivity, per the s189 ruling) vs SCRIP
**m3** (`--run`, in-process) and **m4** (`--compile` → `as`+`gcc`, links `libscrip_rt.so`).
Instrument: `scripts/test_bench_snobol4_timed.sh` — fixed 500 ms budget, **iterations counted**
(a ratio, not a delta), every row's `check:` line diffed against its `.ref` so **correctness
travels with every number**. `SCRIP_NOHUGE=1`, arena sized past the window (**gc=0 on every row
quoted** — a window containing a collection is a stall figure, not throughput).

⛔ **row bench-external-cpu-and-elapsed-clock: the microbenchmark family below is now clocked
externally, not self-timed.** Every rate is iterations divided by `tools/bench_rusage`'s CPU time
(`user+sys`, read from `wait4()`, one instrument outside both engines, blind to which one it is
timing) — **not** either engine's own `TIME()`. Self-timing was the first thing hostile scrutiny
would attack, and the two engines' `TIME()` were not even the same unit until the s249 NS-TIME fix
(both are `CLOCK_MONOTONIC` now) — see `FINDING-2026-08-22-s253`. `min-det` is `NOISE-FLOOR.tsv`'s
`min_detectable_cpu_pct` (3×cv, best-of-5, load-contaminated reps excluded, not averaged in): a
row-to-row or table-to-table delta smaller than it is WEATHER, not a regression.

⛔ **THESE NUMBERS ARE NOT COMPARABLE WITH ANY PUBLISHED BEFORE 2026-08-21.** The CPU governor was
`powersave` on `amd-pstate-epp` — cores at ~37% of maximum clock — for every earlier measurement on
this host. It is now `performance` (measured 4.624 GHz), and `NOISE-FLOOR.tsv` was re-baked solo
under the new governor the same hour. Ratios survive a governor change better than absolute rates,
but neither is safe to carry across it.

⛔ **The `-O2` column is gone, not omitted.** It was last measured at s198 under the throttled
governor and has not been re-run; quoting it beside a `performance`-governor `-O0` column would be
comparing two different machines. Every number below is `RT_OPT=-O0`.

### ⛔ `-O2` IS NOT CORRECTNESS-EQUIVALENT — read this before quoting any `-O2` number

`RT_OPT` defaults to `-O0`; `-O2` is for benchmark runs only (O0-DEV-O2-BENCH). That rule is
usually explained by build time (~1m40 vs ~9m30). **It is also a correctness rule:** at `-O2`,
same tree, `RT_OPT` the only variable, the Milestone 1 ladder collapses **10/10 → 3/10 in both
modes** and `beauty.sno` self-host emits md5 `1c75f97d…` instead of `6f1671c0…`. Ablation puts
the whole defect in **`libscrip_rt.so`** (rebuilding only the runtime at `-O2`, compiler left at
`-O0`, reproduces it exactly — the emitter is exonerated); the ζ-SM tracer brackets it to **one
port event**: 3,126 events agree, then both arms fire `ORIGIN op=114 IR_STATEMENT_END` on
*different* nodes and `-O2` skips ~68 iterations of beauty's scan loop. Tracked as rank-0
`o2-runtime-red-on-m1`. **The suites below are correctness-gated and pass in both arms; beauty
is quoted at `-O0`, the only arm where it is right.**

### Beauty self-host — the Milestone 1 program (`RT_OPT=-O0`)

Interleaved min-of-7; every single run's md5 checked against the fixed point (**7/7 correct for
all three engines**, so this is a like-for-like race). `sbl` and m3 both *compile and run*;
m4 is run-only and its one-time image build is listed separately.

| engine | min | med | max | spread | vs `sbl` |
|---|---:|---:|---:|---:|---:|
| **SPITBOL `sbl -bf`** | **36 ms** | 36 | 39 | 8.3% | 1.00× |
| SCRIP m4 (run only) | 358 ms | 370 | 391 | 9.2% | **0.10× — 10× slower** |
| SCRIP m3 (compile+run) | 2,568 ms | 2,591 | 2,602 | 1.3% | **0.01× — 71× slower** |

m4 image build, once: `--compile` **1,812 ms** + `gcc` link 274 ms. **SPITBOL compiles *and* runs
beauty in 36 ms — less time than SCRIP spends emitting it.** m3's 2.5 s is dominated by
compilation, not execution.

### Microbenchmark family — 15 programs, **15/15 correct in both arms**, throughput (iters/s, external clock)

`m3:sbl > 1.00×` means **SCRIP is faster**. `RT_OPT=-O0`. `min-det` = `min_detectable_cpu_pct`,
best-of-5, this bake (row bench-external-cpu-and-elapsed-clock) — a row whose ratio moved by less
than its own min-det between sessions has not moved outside its noise floor.

| benchmark | `sbl`/s | m3/s | m4/s | **m3:sbl** | min-det |
|---|---:|---:|---:|---:|---:|
| var_access | 7.5M | 60.0M | 65.8M | **7.98×** | 4.8% |
| func_call | 11.6M | 83.7M | 88.3M | **7.20×** | 3.1% |
| op_dispatch | 9.4M | 66.3M | 68.8M | **7.02×** | 2.3% |
| arith_loop | 21.2M | 145.5M | 153.2M | **6.86×** | 11.0% |
| fibonacci | 5.8K | 32.6K | 33.8K | **5.60×** | 3.6% |
| string_concat | 5.2M | 17.5M | 18.0M | **3.36×** | 33.9%! |
| pattern_bt | 892.3K | 2.5M | 2.5M | **2.83×** | 3.7% |
| string_pattern | 3.5M | 5.0M | 5.1M | **1.42×** | 4.6% |
| eval_fixed | 5.5M | 5.1M | 5.4M | **0.91×** | 4.7% |
| indirect_dispatch | 9.4M | 7.3M | 7.5M | **0.77×** | 1.8% |
| string_manip | 8.4M | 4.9M | 4.9M | **0.58×** | 17.9%! |
| array_sum | 17.5K | 10.0K | 10.1K | **0.57×** | 2.5% |
| mixed_workload | 264.4K | 121.4K | 124.0K | **0.46×** | 3.6% |
| roman | 454.0K | 202.5K | 207.3K | **0.45×** | 4.8% |
| table_access | 15.8K | 4.4K | 4.5K | **0.28×** | 3.8% |

⭐ **Eight of fifteen rows beat the oracle under the external clock** (`m3:sbl > 1.00×`: var_access,
func_call, op_dispatch, arith_loop, fibonacci, string_concat, pattern_bt, string_pattern) — the
s249 self-timed table above (superseded, kept for provenance) read this as "nine," which the s249
numbers do not actually support either on a row-by-row recount (they show the same eight); the count
was never re-verified against its own table before now. `!` marks a row whose own min-det (33.9%,
17.9%) is wide enough that this session's number cannot be compared precisely against any prior one.

⛔ **Every row moved from the s249 self-timed table, and four moved by more than this session's own
min-det** — this is the credibility case this row exists to make, not noise: **array_sum**
0.90×→0.57× (min-det 2.5%, a 37% shift), **table_access** 0.48×→0.28× (3.8%, 42% shift),
**mixed_workload** 0.54×→0.46× (3.6%, 15% shift), **roman** 0.51×→0.45× (4.8%, 12% shift) — all four
are allocating rows, and `FINDING-2026-08-22-s253` independently flagged SCRIP's `maxrss_kb` on
exactly this trio as 200×+ SPITBOL's on the same kernel; **fibonacci** 6.10×→5.60× and
**indirect_dispatch** 0.80×→0.77× also cleared their min-det (8% and 4% shifts). The rest — including
`string_concat`/`string_manip`'s apparent moves — sit inside min-det this session measured as wide
(≥17.9%) and are not distinguishable from weather either way. **Read this as: the self-timed table
above was measuring something silently different per engine, and now measures the same thing for
both — treat the s249 numbers as provenance, not as the current baseline.** Root-caused fix
attribution for the earlier 0.28×→0.48× table_access move (`getenv` cache sentinel, s249 §7F) is
unaffected by this re-clocking; it is a real, separately-verified codegen fix, not a clock artifact.

⛔ **This table's clock is now clean; the oracle's WORKLOAD is a separate, still-open question.**
HQ (2026-08-22, same day) measured `sbl -bf` under callgrind on `beauty` and found `emit_pm`/
`pm_check_enabled` — pattern-match trace instrumentation gated on unset `SPL_PM_TRACE` — costing
23.47% of SPITBOL's total instructions doing nothing (`FINDING-2026-08-22-hq-scrip-spends-under-one-percent-of-its-instructions-running-the-program.md`).
Every `sbl` number above is measured against that binary as shipped; whether the dead instrumentation
distorts these 15 rows unevenly (more pattern-match-heavy rows would be hit harder) is untested here
— out of this row's scope, which was the clock, not the workload. Read `sbl`-relative ratios above as
clock-clean, not yet workload-clean.

`arith_loop` 5.37× → 7.07× came from three codegen cuts — null-concat copy propagation, an inlined
integer compare, and a four-way literal fold on `IR_BINOP` — taking it from 165 to 112 instructions
per iteration. `table_access` is deliberately unmoved: its cost is that `aggregates.c` stringifies
**every** table key, so an integer key costs *more* than a string key. That is the next rung.

**m4:m3 = 0.98–1.11× on every row** — the two modes are performance-equal, as the m3 ≡ m4 design
invariant requires.

### Demo / real-workload family — 15 programs (`HEAP=4096`, GC-free window)

| benchmark | `sbl`/s | m3/s `-O0` | m3:sbl `-O0` | m3:sbl `-O2` |
|---|---:|---:|---:|---:|
| claws5-match-fence | 9.1K | 15.0K | **1.65×** | **1.58×** |
| claws5-match | 10.3K | 15.8K | **1.53×** | **1.54×** |
| treebank-match-fence | 2.3K | 2.0K | 0.85× | 0.99× |
| treebank-match | 2.5K | 1.9K | 0.74× | 0.81× |
| calculator-1-match | 34 | 22 | 0.65× | 0.74× |
| calculator-1-match-fence | 80 | 39 | 0.49× | 0.69× |
| calculator-1 | 171 | 71 | 0.42× | 0.45× |
| calculator-2-match-fence | 2.2K | 864 | 0.40× | 0.53× |
| calculator-2-match | 3.4K | 851 | 0.25× | 0.35× |
| claws5 (unmatched) | 213 | 41 | 0.19× | 0.22× |

⛔ **5 of 15 rows are red in BOTH arms and are therefore standing defects, not `-O2` fallout**
(attribution measured, not assumed): `json` / `json-match` / `json-match-fence` CRASH, `porter`
CRASH/BUILD-ERR, `calculator-2` DISAGREE. ⛔ `json-match-fence` at `HEAP=4096` printed a nonsense
**74.27G/s** for m4 — a broken row emitting a rate. Never quote it.

### ⭐⭐ What the numbers say — the split follows the *binary* boundary

`RT_OPT` governs **only the C runtime library**, never SCRIP's emitted code. So the `-O2` − `-O0`
delta on each row measures **how much of that row runs in C**, and it separates the table cleanly:

* **Rows SCRIP wins 4.6–7.0× move ≤4% between arms** (var_access, func_call, op_dispatch,
  arith_loop, fibonacci) — statistically nothing against their 2.4–14% min-detectable. They run
  almost entirely in emitted x86. **That engine really is 5–7× faster than SPITBOL.**
* **Rows SCRIP loses are exactly the rows `-O2` helps** — string_concat 1.89×, roman 1.57×,
  string_pattern / eval_fixed 1.29×, mixed_workload 1.25×, pattern_bt 1.24×, table_access 1.17× —
  because they spend their time in the C runtime, which is where SCRIP is behind.

**The road to "ten times faster" runs through the runtime library, not the code generator** — and
beauty is the extreme case: 1.8 s to emit a program SPITBOL compiles and runs in 36 ms. This
confirms and sharpens the s111 reading below (*"the remaining losses concentrate in the runtime
library"*), which was inferred from build flags; here it is measured with `RT_OPT` as the only
variable.

*(Interface note: the build/flag sections earlier in this file are historical — `scrip-cc` and the
per-backend `-asm`/`-jvm`/`-net` flags are gone. Today there is one `scrip` binary with two modes,
`--run` (m3) and `--compile` (m4), x86-64 only; the other backends are dormant under X86-ONLY.)*

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
| string_concat | 164 | 320 | 11 | **0.07× (faster)** | **0.03× (faster)** |
| var_access | 1804 | 3575 | 339 | **0.19× (faster)** | **0.09× (faster)** |
| op_dispatch | 162 | 391 | 41 | **0.25× (faster)** | **0.10× (faster)** |
| arith_loop | 64 | 125 | 25 | **0.39× (faster)** | **0.20× (faster)** |
| func_call | 1230 | 2590 | 938 | **0.76× (faster)** | **0.36× (faster)** |
| func_call_overhead | 1202 | 2597 | 952 | **0.79× (faster)** | **0.37× (faster)** |
| pattern_bt | 250 | 641 | 200 | **0.80× (faster)** | **0.31× (faster)** |
| fibonacci | 226 | 569 | 229 | 1.01× | **0.40× (faster)** |
| string_manip | 787 | 1751 | 1876 | 2.38× | 1.07× |
| eval_fixed | 311 | 680 | 843 | 2.71× | 1.24× |
| roman | 193 | 1063 | 622 | 3.22× | **0.59× (faster)** |
| string_pattern | 814 | 2041 | 3021 | 3.71× | 1.48× |
| mixed_workload | 208 | 518 | 795 | 3.82× | 1.53× |
| table_access | 427 | 2652 | 1643 | 3.85× | **0.62× (faster)** |
| eval_dynamic | 503 | 1188 | (crash) | — | — |

*(Numbers are post-s110/s111 — BP-1..BP-6 (builtin dispatch O(1), getenv cache, NV I-gate,
table int-key fast-lane, string extend-in-place, zeroing skip) + SPD-1 (SEQ-STATIC default
on) + SPD-2 (RETRY-INTERNAL flat_pat blob positional retry). CSNOBOL4 column is the s111
verified set (`bd51fcb5`, built from `snobol4ever/csnobol4`); all three engines re-timed on
this host 2026-07-20, SCRIP HEAD `02053ed2`. Timing is each program's own `TIME()` self-report
in ms (startup excluded; SPITBOL TIME() diffs are ns, ÷1e6). Gates: smokes 7/7 both modes
×2, crosscheck m3 307/0 · m4 307/0 · DIVERGE=0. See `GOAL-SNOBOL4-BB.md` § BENCH-PERF
LADDER for the full SPD rung plan and remaining bottleneck analysis. ⚠ NOTE s111: claws5
and treebank-list (demo programs, not in this suite) are correctly TRI-IDENTICAL at HEAD
(5622 / 8727 output lines) but remain slower than SPITBOL at the true full-input workload
(claws5 m4 337ms vs sbl 38ms; treebank m4 895ms vs sbl 160ms) — CSNOBOL4 DNFs both with
"Overflow during pattern matching". The s110 LIVE CURSOR perf numbers (24ms/109ms) were an
empty-stdin measurement artifact (s111 finding). SPD-3 profile-first is the next rung.)*

**Scoreboard (14 scored): SCRIP beats CSNOBOL4 on 10/14, beats SPITBOL on 7/14.**
SCRIP's compiled native loops beat SPITBOL outright on register-shaped work (variable
access, arithmetic, operator dispatch, both call-convention benchmarks, pattern backtracking)
and — most dramatically — on `string_concat` (BP-5 extend-in-place: 0.07×, 14× faster than
SPITBOL). The remaining losses concentrate in the **runtime library** (built at `-O0 -g`
vs SPITBOL hand-asm and CSNOBOL4 `-O3`): string manipulation/pattern on string-heavy input,
mixed workloads, EVAL, and the GC-bound string-concat-heavy programmes (roman, table_access).
`eval_dynamic` crashes (known, deprioritised).

### Match-only pair — literal-needle guts flavors (s128, SCRIP runtime `-O0`)

The pure pattern-match programs `corpus/programs/snobol4/demo/{claws5-match,treebank-match}.sno`
(one big anchored match, zero side effects; needles fold 100% frozen-LITERAL) measured under
the four `ZC_*` literal-guts flavors of `bb_match_{span,break,any,notany}`: **A** = default
(compare-chain ≤ `ZC_CSET_CHAIN_MAX`=3, else 256-byte membership table, subject step unrolled
×`ZC_UNROLL_FACTOR`=4) · **R** = `ZC_LIT_GUTS=RANGE` (cset coalesced into contiguous RANGES at
emit; single byte = `cmp/je`, run = unsigned-sub trick `mov eax,esi; sub; cmp; jbe`; claws5's
36-char cset = 2 range tests) · **A∞** = unlimited compare-chain (`ZC_CSET_CHAIN_MAX=256`: every
literal cset a full inline chain, 36-char csets = 36 compares) · **B** = `ZC_LIT_GUTS=INLINE`
(emitted inner needle-loop, RO needle) · **C** = `ZC_LIT_GUTS=CALL` (`rt_sg_scan.S` R13/R15-aware
lean leafs). All flavors ride R13=Σ/R14=δ/R15=Δ. In-program `TIME()` CPU ms (slurp/compile/startup
excluded), K in-program match repetitions (claws5 K=200, treebank K=50), medians of 5 interleaved
reps; `wall` = whole-process shell ms. SPITBOL = official x64 `sbl -b -d512m -i64m [-s256m]`,
`-CASE 0`. 12/12 correctness matrix ref-identical; all four flavors byte-verified distinct in `.s`.

| program | engine | slurp ms | match ms | per-match ms | wall ms |
|---|---|--:|--:|--:|--:|
| claws5-match | **A (chain≤3+table) m4** | 14 | 39 | **0.195** | **65** |
| claws5-match | R (range chain) m4 | 13 | 44 | 0.220 | 69 |
| claws5-match | A∞ (full chain) m4 | 14 | 64 | 0.320 | 89 |
| claws5-match | B (INLINE) m4 | 14 | 141 | 0.705 | 171 |
| claws5-match | C (CALL) m4 | 14 | 144 | 0.720 | 174 |
| claws5-match | SPITBOL | 13 | 45 | 0.225 | 68 |
| treebank-match | A m4 | 78 | 84 | 1.680 | 204 |
| treebank-match | R m4 | 74 | 78 | 1.560 | 191 |
| treebank-match | A∞ m4 | 77 | 96 | 1.920 | 213 |
| treebank-match | B m4 | 76 | 95 | 1.900 | 211 |
| treebank-match | C m4 | 76 | 103 | 2.060 | 218 |
| treebank-match | SPITBOL | 31 | 39 | **0.780** | **85** |

(m3 within noise of m4 on every row — full grid incl. m3 in
`FINDING-2026-07-22-CLAUDE-SN4-SG-ABC-LIT-GUTS-UNROLL-VS-INLINE-VS-CALL.md`.)
**Readings:** on long csets (claws5's 10/26/36-char) the 256B **table wins everything** and
SCRIP beats SPITBOL per-match (0.195 vs 0.225) *and* on whole-process wall (65 vs 68, compile
included); the ladder is **table (0.195) > range-chain (0.220, ≈SPITBOL) > full byte-chain (0.32) >
INLINE/CALL (0.71)** — chains do beat per-char loops (the old chains-beat-strchr report holds),
ranges beat chains by 45%, and the L1-hot table's single load-compare-branch still edges the
range tests' extra branch per range by ~15%; the chain-≤3-then-table ladder stands. treebank's flavor spread is only ~20%
— its 2.2–2.6× gap vs SPITBOL is **not in the cset guts** but in the recursive `*group`/ARBNO
backtracking machinery (SPD-2/BP-9) **and in slurp**: SCRIP's per-line `src = src line CHAR(10)`
costs 78 ms vs SPITBOL's 31 (claws5's CHAR-free slurp is at parity, 14 vs 13 — the delta is
per-line `CHAR(10)` builtin dispatch, a compile-time-folding lead).

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

## Icon Benchmark — SCRIP vs Arizona Icon (`icont`/`iconx`) vs JCON (JVM)

**Measured 2026-08-21 at SCRIP `3b70bf67`, `RT_OPT=-O2`** (the benchmark arm of FACT RULE
O0-DEV-O2-BENCH; `-O0` is the development default and the tree is restored to it afterwards).
Runner: `scripts/bench_icon_rate_3way.sh`. Programs: `corpus/benchmarks/icon/rate/`.

⛔ **TWO REFERENCE ENGINES, TWO DIFFERENT JOBS.** **Arizona Icon 9.5.25a is the CORRECTNESS ORACLE**
(Lon, 2026-07-21: never grade Icon against Java/JVM). **JCON 2.2+git is a SPEED REFERENCE ONLY**,
added at Lon's request 2026-08-21; it grades nothing.

### Method — fixed TIME, count iterations, report a rate

Each program reads its own **`&time`**, warms untimed so a JIT can reach steady state, then runs whole
**UNIT**s until a wall-clock budget expires and reports `units elapsed_ms checksum`. Rate =
`units × UNIT ÷ ms`. Startup is excluded **by construction on every engine** — verified equivalent:
SCRIP 21 ms internal / 25 ms wall, `iconx` 170/175, JCON 90/162 (JCON's gap is exactly its JVM
startup).

⛔ **This replaces fixed-iteration timing, which this project used until 2026-08-21 and which is not
trustworthy on this corpus** (Lon: *"I do not trust those numbers"* — he was right, and inverting the
loop changed the answer):

1. Fixed-iteration runs here last 12–80 ms, so **process startup is a large fraction** and has to be
   hand-subtracted — an error-amplifying step.
2. It **never lets a JIT warm.** It measured JCON cold and reported it ~2.9× *behind* SCRIP. Warm,
   **JCON is ahead of SCRIP on 7 of the 9 scored benchmarks.**
3. At those durations **contention swings a single sample by >50%**: the `iconx` oracle — an unchanged
   binary — read 26,560 then 17,312 ops/ms on two consecutive runs. The table below is **best-of-3**
   with the machine load recorded.

⛔ **A matching checksum does not prove the work happened.** Each program's checksum is per-UNIT, so it
is identical on every engine regardless of how many units each completed — one run yields the rate
*and* a cross-engine correctness check. But **SCRIP constant-folds `"abcdefgh" || "ijklmnop"`** in
`concat_dispatch` (measured: **zero** concat calls in its emitted `.s`, against 2 in every other concat
benchmark) and the folded constant still has the right length, so the checksum matched anyway. That row
is **excluded from scoring**. Audit the emitted asm for the operation under test before trusting a row.

### Rate — ops/ms, higher is better, best-of-3, load 1.9

| benchmark | `iconx` | JCON | SCRIP m3 | SCRIP m4 | m4 vs `iconx` | m4 vs JCON |
|---|---:|---:|---:|---:|---:|---:|
| `int_loop` | 22,987 | 96,947 | 246,320 | **226,760** | **9.86×** | **2.34×** |
| `int_mod` | 24,867 | 121,347 | 164,893 | **155,507** | **6.25×** | **1.28×** |
| `concat_table` | 800 | 22,573 | 1,738 | 1,671 | **2.09×** | 0.07× |
| `concat_strvar` | 22,787 | 74,680 | 43,973 | 42,932 | **1.88×** | 0.57× |
| `concat_int_dispatch` | 18,093 | 67,200 | 19,133 | 18,759 | 1.04× | 0.28× |
| `concat_intvar` | 20,147 | 78,400 | 18,987 | 19,253 | 0.96× | 0.25× |
| `sub_table_miss` | 20,907 | 75,413 | 15,293 | 16,480 | 0.79× | 0.22× |
| `sub_list` | 15,413 | 41,733 | 12,614 | 12,093 | 0.78× | 0.29× |
| `table_semantics` | 8,030 | 39,990 | 2,577 | 2,625 | 0.33× | 0.07× |
| ~~`concat_dispatch`~~ | 24,653 | 71,213 | 206,920 | 201,947 | *(void — SCRIP folds it)* | *(void)* |

**Scoreboard (9 scored): against Arizona Icon SCRIP wins 4, ties 2, loses 3. Against JCON SCRIP wins
2 and loses 7.**

The shape of that result is consistent and unsurprising once seen: **SCRIP's native compilation wins
where the work is register-shaped** — integer loops (**9.9×** over `iconx`) and integer modulo
(**6.3×**) — and **loses where the work lives in the runtime library**: list subscripting, table
lookup, and above all the table-building `table_semantics` benchmark, where SCRIP is **3× slower than
`iconx` and 15× slower than JCON**. Concatenation is the boundary: ~1.9× ahead on a string variable,
at parity on an integer variable. **This mirrors the SNOBOL4 result in this same README**, whose
losses also concentrate in the runtime library rather than in emitted code.

⛔ **The table/list runtime is therefore the single highest-value Icon optimization target**, and JCON
is the proof that the headroom is real: a JVM implementation is **14×** faster on `concat_table` and
**15×** faster on `table_semantics`. JCON also beats Arizona ~3–5× across the board once warm, while
paying ~72 ms of JVM startup — it is the stronger reference engine on sustained work and by far the
weaker on short runs.

### ⛔ The larger corpus programs do not run — no speed claim may be made for them

| program | state at `3b70bf67` |
|---|---|
| `queens` · `deal` · `concord` · `ipxref` · `rsg` · `geddump` | **SEGV** (rc=139, core dumped) |
| `tgrlink` | **early-exit** — 2 lines against both oracles' 3,239 |
| `micsum` | ✅ correct |
| `micro` · `version` | diverge **by design** (a timing harness, and `&version`) |

The five link-dependent programs crash **even when `options.icn` / `post.icn` / `shuffle.icn` are
passed to `--compile`** (SCRIP accepts multiple source files without complaint), so this is not simply
a missing-library problem. Closing it is `GOAL-ICON-100.md` Definition-of-Done item 3. `tgrlink` is the
standing reminder of why every timing here is paired with an output check: it "finished" in 31 ms
against `iconx`'s 127 ms — a 4× "win" — while doing almost none of the work.

Reproduce: build both oracles (below), then
`RT_OPT="-O2 -g -fno-strict-aliasing -fwrapv -fno-omit-frame-pointer" make pristine` and
`bash scripts/bench_icon_rate_3way.sh`. Correctness on the full corpus:
`bash scripts/honest_icon_correctness.sh`.

**Oracle build traps, both hit this session.** *Arizona* (`refs/icon-master`): the `linux` config
enables graphics, so on a headless box you must comment `#define Graphics 1` in `src/h/define.h`
**and** blank `XLIBS =` in both `Makedefs` and `config/linux/Makedefs`, or the build stops at
`X11/xpm.h` and then at `-lXpm`. *JCON* (`refs/jcon-master`, OpenJDK 21): **its translator is itself
written in Icon**, so `icont` must be built and on `PATH` first.

### Superseded grids — kept for the lessons, not the numbers

⚠ **The fixed-iteration grid published earlier on 2026-08-21 is superseded by the table above.** It
reported SCRIP m4 beating `iconx` on 9 of 10 microbenchmarks and being 2.9× faster than JCON. Both
claims were artifacts of the method: the runs were startup-dominated with hand-subtracted floors, and
**JCON was measured cold**. Under fixed-time measurement SCRIP wins 4 of 9 against `iconx` and loses 7
of 9 to JCON.

⚠ **The 2026-07-18 grid (`f405c6a7`) reported 6.8×/6.5×/13.8× on concord/deal/queens. Void:** five of
its six times sat on the ~4 ms process floor, and the suite then stood at 6/10 with `geddump`/`tgrlink`
emitting zero output.

⚠ **The 2026-07-25 grid (`2cbdc2b1`, `RT_OPT=-O0`) reported `m4` at 0.62× geomean — SCRIP 1.62×
*slower* than `iconx`** — on 6/9 byte-identical correctness. It was taken on the **`-O0` development
arm**, and the larger programs it timed have since regressed to SEGV. ⛔ Its build-arm lesson is live:
measuring the `-O0` build against optimized oracles understates SCRIP by roughly 2× on every
runtime-heavy benchmark, an error made and caught during this harness's own bring-up. Its
architectural finding still stands: measured with callgrind, SCRIP executed **fewer instructions than
the `iconx` interpreter** on `tgrlink` (1.303 G vs 1.339 G Ir) and still lost on wall clock, locating
the deficit in **memory behaviour** — a cold bump-allocated arena against `iconx`'s small warm heap,
25× the LL misses and 25× the page faults at s163 — not in instruction count. ⛔ A related hypothesis
was tested and **falsified** this session: SCRIP's throughput does **not** decay over a sustained
window (concat_intvar held 9,461→10,507 ops/ms from a 500 ms to a 6,000 ms budget), so the runtime
deficit is per-operation cost, not progressive GC degradation.

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
