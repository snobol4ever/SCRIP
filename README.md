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

## Icon Benchmark — SCRIP vs Arizona Icon (`icont`/`iconx`)

The Icon frontend is measured against canonical **Arizona Icon 9.5.25a** (`icont`/`iconx`,
built from source in-sandbox: `make Configure name=linux && make`, NoGraphics) on
`corpus/benchmarks/icon/` (9 programs; `micro` is not timed — it exists to generate
`micsum`'s input). Invocation is the stock jcon/bmark protocol (`concord <concord.dat` ·
`deal -h 1000` · `geddump <geddump.dat` · `ipxref <ipxref` sources · `micsum <micro-output` ·
`queens -n10` · `rsg <rsg.dat` · `tgrlink tgrlink.dat` · `version`).

**Measured 2026-07-25 at SCRIP `2cbdc2b1`, `RT_OPT=-O0`** (the O0-DEV default; no `-O2`
anywhere — see FACT RULE O2-DIRECTED-ONLY). Single-core container.

### Protocol — and why this grid replaces the previous one

Timing runs with the suite's stock `post.icn` output suppression active (`Init__` reassigns
`write := 1`, the Icon integer-as-selector idiom), so times measure **compute, not I/O**.
That suppression also means **stdout cannot validate a timing run** — a program that skips
its workload entirely still prints the same 30 banner lines.

Correctness is therefore established in a **separate pass with `OUTPUT=1`**, which restores
`write`. The true program output is the window strictly between post.icn's
`*** Benchmarking with output ***` marker and its `elapsed time =` line; that window is
extracted from both engines and compared byte-for-byte. Runner: `scripts/honest_icon_bench.sh`
(mode 4) — **not** `test_icon_bench_corpus.sh`, which grades on `rc==0 && output non-empty`
and by construction cannot distinguish "ran the workload fast" from "skipped the workload".

⚠ **The grid formerly in this section (2026-07-18, `f405c6a7`) reported m4 as 6.8×/6.5×/13.8×
faster than `iconx` on concord/deal/queens. Those numbers are superseded and should not be
cited.** Five of that grid's six times (4–5 ms) sat on the ~4 ms process floor — the exact
signature the output-diffing runner was built to detect — and that measurement was taken when
the suite stood at 6/10 with geddump/tgrlink emitting zero output. The numbers below are
lower because they are taken on runs whose output is verified to match the oracle.

### Correctness — real program output vs `iconx`

| Benchmark | oracle lines | SCRIP lines | Verdict |
|-----------|-------------:|------------:|---------|
| concord | 1,345 | 1,345 | ✅ IDENTICAL |
| deal | 17,000 | 17,000 | ✅ IDENTICAL |
| geddump | 10,145 | 11,222 | ❌ DIVERGE |
| ipxref | 1,208 | 1,208 | ✅ IDENTICAL |
| micsum | 2 | 2 | ✅ IDENTICAL |
| queens | 16,653 | 16,653 | ✅ IDENTICAL |
| rsg | 5,000 | 1,000 | ❌ DIVERGE — see below |
| tgrlink | 3,239 | 3,239 | ✅ IDENTICAL |
| version | 1 | 1 | ⚪ benign — each engine prints its own `&version` |

**6/9 byte-identical · 2 real defects · 1 benign.** Both modes agree on every verdict.

### Timing — wall-clock, best-of-N, `RT_OPT=-O0`

`m3` = `--bench --run` (in-process; **includes** parse+lower+emit in the measured time).
`m4` = `--compile --target=x86` → `as`+`gcc` binary, run only. Process floor ≈ 4 ms.

| Benchmark | iconx | SCRIP m3 | SCRIP m4 | m4 vs iconx | Output |
|-----------|------:|---------:|---------:|------------:|--------|
| concord | 35 | 82 | 63 | 0.56× | ✅ |
| deal | 27 | 93 | 83 | 0.33× | ✅ |
| ipxref | 32 | 84 | 61 | 0.52× | ✅ |
| queens | 57 | 62 | 52 | 1.10× | ✅ |
| tgrlink | 174 | 230 | 202 | 0.86× | ✅ |
| micsum | 3 | 9 | 4 | (floor) | ✅ |
| version | 3 | 5 | 4 | (floor) | ⚪ |
| geddump | 161 | 273 | 245 | *(void — diverges)* | ❌ |
| rsg | 17 | 25 | 5 | *(void — short-circuit)* | ❌ |

**Geomean over the five correctness-verified non-trivial workloads: m4 = 0.62× (SCRIP 1.62×
slower than `iconx`), m3 = 0.50× (2.01× slower).** `micsum`/`version` are excluded as
process-floor noise; `geddump`/`rsg` are excluded because a program that does not produce the
oracle's output has no meaningful time.

### Instruction count (callgrind Ir) — the honest metric

Wall-clock on this corpus is startup-dominated; per the measurement protocol only
`tgrlink`/`geddump` carry trustworthy wall-clock. Ir is the metric that is not.

| Benchmark | iconx Ir | SCRIP Ir | Ratio |
|-----------|---------:|---------:|-------|
| tgrlink | 1,338,636,521 | 1,303,410,904 | **SCRIP 1.027× FEWER instructions** |
| geddump | 1,172,264,182 | 1,367,447,290 | SCRIP 1.17× more *(diverges — not comparable)* |

**This reproduces the s163 diagnosis independently: SCRIP emits native code that executes
fewer instructions than the `iconx` bytecode interpreter, and still loses on wall clock.**
The deficit is therefore not instruction count — it is memory behaviour (cold bump-allocated
arena vs iconx's small warm heap; 25× LL misses and 25× page faults at s163). No
instruction-shaving rung closes that gap.

### Two live defects

- **`rsg` — resolved from "unexplained" to a named short-circuit.** SCRIP emits exactly
  **1,000 blank lines (1 distinct value)** against the oracle's 5,000 lines / 1,604 distinct
  sentences. 1,000 is `rsg.icn`'s default `limit`, so the generator loop runs to its bound
  and produces empty strings — the grammar never populates. **This voids the 2.83–3.40× m4
  "speedup" previously carried for `rsg`: it is not faster, it is not doing the work.**
  Hypothesis (not yet proven, MONITOR-FIRST rung owed): `main` builds
  `plist := [define,generate,grammar,source,comment,prompter,error]` — a list of *procedure
  values* — and dispatches through it; procedure-value handling is the adjacent known-weak
  area (cf. `FINDING-2026-07-20-CLAUDE-ICN-LOCAL-SHADOW-PROC-VALUE-REWRITE-COLLISION.md`).
- **`geddump`** — 11,222 lines vs the oracle's 10,145, both modes. Pre-pinned; see
  `GOAL-ICON-BB.md` (`git revert 7aade169` is the standing lead).

Reproduce: `bash scripts/honest_icon_bench.sh` (m4 timing) and `bash scripts/honest_icon_correctness.sh` (the `OUTPUT=1` real-output diff above). Oracle kit:
`icon-master` → `make Configure name=linux && make` → `bin/icont`, `bin/iconx`. **No Java and
no JCON/JVM path is involved or required** (Lon directive, 2026-07-21): the JCON column
carried by the superseded grid is intentionally not refreshed here.

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
