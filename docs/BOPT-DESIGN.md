# BOPT — Branch Optimizer (IR→IR, between LOWER and EMITTER)

Design notes for the OPT priority rung in `GOAL-SNOBOL4-BB.md`. Implements the
optimization §5 of Proebsting, *Simple Translation of Goal-Directed Evaluation*
(`docs/8_Simple_Translation_of_Goal_Directed_Evaluation.pdf`): "propagating
copies and eliminating branches to branches (by branch chaining and reordering
the code)."

---

## BOPT-0 — port → field mapping (grounded in source, no code change)

### The exact correspondence

The paper (§4, p.2) splits the four port code-chunks into two attribute classes:

> The start and resume chunks are **synthesized** attributes. The fail and
> succeed chunks are **inherited** attributes.

`src/contracts/IR.h` `struct IR_t` stores **exactly the two inherited ports as
edge fields** and nothing else port-related:

```c
struct IR_t {
    IR_e        op;
    IR_ref_t    γ;          /* succeed edge — INHERITED  */
    IR_ref_t    ω;          /* fail edge    — INHERITED  */
    IR_t      **operands;   /* children                  */
    int         n_operands;
    int         idx;
    IR_graph_t *own;
};
```

The two **synthesized** ports (start, resume) are NOT graph edges — they are
materialized as labels at emit time. This is the single most important fact for
BOPT: the ports BOPT rewrites (γ, ω) live in the IR and can be rewritten by a
pure IR→IR pass; the ports it cannot reach from the IR (α, β) are emit-time
labels and belong to the post-emitter peephole (PEEP).

### Port table

| Greek | Byrd  | Paper   | Attribute   | In IR_t?            | Realized as |
|-------|-------|---------|-------------|---------------------|-------------|
| **α** | call  | start   | synthesized | NO                  | `bb<j>_α` label minted by `bb_fill_alpha` (emit_bb.c); the box's own entry. The box's start chunk is emitted here. |
| **β** | redo  | resume  | synthesized | NO                  | `snoch%d_n%d_β` label allocated by the chain driver (`betas[i]`), passed into `walk_bb_flat` arg 4; the box DEFINES it and emits its resume chunk there. |
| **γ** | exit  | succeed | inherited   | YES (`nd->γ.node`)  | edge resolved by the chain driver to the target node's α label → passed into `walk_bb_flat` arg 2 (`lbl_γ`). The box JUMPS here on success. |
| **ω** | fail  | fail    | inherited   | YES (`nd->ω.node`)  | edge resolved to target α label → `walk_bb_flat` arg 3 (`lbl_ω`). The box JUMPS here on failure. |

`void walk_bb_flat(IR_t *nd, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β);`
— confirms the 4-arg contract: succeed target, fail target, own-resume label; α
synthesized internally.

### How the chunks are wired (emit_bb.c)

- **`bb_fill_alpha(nd)`** (emit_bb.c ~245): mints `bb<j>_α`, stores into
  `g_emit.lbl_α`/`lbl_α_p`. Called by both `FILL` and `EMIT_PAIR_FILL`.
- **`FILL(nd,s,f,b)` / `EMIT_PAIR_FILL(nd,s,f,b)`** (emit_bb.c 272 / 281): set
  `g_emit.lbl_γ=s`, `lbl_ω=f`, `lbl_β=b`, then `walk_bb_node(nd, …)` which emits
  the box body referencing those four labels. The two macros are byte-identical
  except EMIT_PAIR_FILL is used after a deferred EMIT_PAIR list flush.
- **EMIT_PAIR list** (`EMIT_PAIR_DEF`, `EMIT_PAIR_JMP`, `EMIT_PAIR_DEF_JMP`):
  a deferred list of `(define-label, jmp-target)` pairs the combinator drivers
  build, e.g. `EMIT_PAIR_DEF_JMP(lbl_β, last_β)` ⇒ emit `β: jmp last_β`. These
  ARE the "branches to branches" — they are emit-time, not in the IR.

### The statement-chain driver — `codegen_gvar_flat_chain_body` (emit_bb.c ~3990)

This is the per-program-body driver and the natural BOPT hook point.

1. BFS-collect reachable statement nodes from `entry`, following
   `c->γ.node` and `c->ω.node` (each first passed through
   `gvar_chain_resolve_stmt`). Result: `nodes[]`.
2. Allocate `snoch%d_n%d_α` (`lbls[i]`) and `snoch%d_n%d_β` (`betas[i]`) per node.
3. Per node i: define `lbls[i]`, resolve `nodes[i]->γ.node`/`->ω.node` to the
   matching node's α label (`node_γ`/`node_ω`), then
   `walk_bb_flat(nodes[i], node_γ, node_ω, betas[i])`.

Special edge cases already handled in the resolution loop:
- γ-target op `IR_FAIL`            → `lbl_ω`   (program fail label)
- ω-target op `IR_FAIL`            → `lbl_ω`
- ω-target op `IR_SUCCEED`         → `lbl_γ`   (program succeed label)

### Embryonic chaining already present — `gvar_chain_resolve_stmt` (emit_bb.c 3885)

```c
static IR_t *gvar_chain_resolve_stmt(IR_t *n) {
    int guard = 0;
    while (n && guard++ < 4096) {
        if (n->op == IR_SUCCEED && n->γ.node != NULL)                                 { n = n->γ.node; continue; }
        if ((n->op == IR_SEQ || n->op == IR_SEQ_EXPR) && IR_LIT(n).dval == 1.0
             && n->γ.node != NULL)                                                    { n = n->γ.node; continue; }
        break;
    }
    return n;
}
```

This is **already a two-case transitive goto-skip with a cycle guard**: it
chases through (a) `IR_SUCCEED` nodes that carry a continuation, and (b)
"nullary" `IR_SEQ`/`IR_SEQ_EXPR` join markers (`dval==1.0`). BOPT-1 is the
generalization of exactly this resolver into a real, op-classified,
cycle-guarded forwarder-skip applied to the γ/ω edges.

### What the emitted chains actually look like (arith_loop.s, verified)

Two label tiers per statement node:
- `snoch0_n<k>_α` / `snoch0_n<k>_β` — the inter-statement chain (one pair / node).
- `bb<j>_α` — the box's own start, emitted immediately inside `snoch0_n<k>_α`.

Observed redundancy classes:

1. **Adjacent-label pair** — `snoch0_n<k>_α:` immediately followed by `bb<j>_α:`
   for the same point. Two labels, one address. (PEEP merge — emit-time.)
2. **Empty-forwarder box** — `IR_LIT_scalar` whose value is compile-time
   consumed by its sibling assign (`movabs rsi, <imm>`); its entire start chunk
   is `jmp <γ-target>`. (IR-level skip IS NOT op-kind-safe — see hazard below.)
3. **Dead resume stub** — `snoch0_n<k>_β: jmp <target>` where the target equals
   the node's γ-target and nothing ever resumes node k in straight-line flow.
   (PEEP / dead-BB — needs "is node k ever a resume target?" which IS visible in
   the IR via incoming β wiring, so partly BOPT-2.)
4. **Branch-to-branch** — `…_α body … jmp snoch0_n<k>_α` where `snoch0_n<k>_α`'s
   box is itself a forwarder. (BOPT-1 when the forwarder is op-kind-safe.)

---

## Consequence for the two-pass split (confirms Lon's placement)

The paper's single "branch chaining + copy propagation" optimization **splits
across two SCRIP passes by construction**, because SCRIP realizes the four ports
in two tiers (IR γ/ω edges + emit-time α/β/`bb` labels):

- **BOPT (IR→IR, before emitter)** can only rewrite what is in the IR: the
  **inherited γ/ω edges**. It collapses inter-node control chains by
  redirecting a γ/ω edge past a provably-pure-forwarder node to that node's own
  γ. This shrinks the `nodes[]` set the driver emits and the targets it wires.
- **PEEP (post-emitter)** removes the residue BOPT structurally cannot see: the
  adjacent `snoch_α`/`bb_α` label pairs, the dead `_β: jmp` resume stubs, the
  combinator-intermediate `xcat%d_*: jmp parent` hops, and `mov`/`jmp` identity
  round-trips.

Neither pass alone reproduces Fig 1 → Fig 2. They are complementary, exactly as
proposed.

### ⛔ HAZARD for BOPT-1 — "forwarder" is a control fact, not always an op-kind

Class-2 above (`IR_LIT_scalar` looks like a forwarder) is a **dataflow** fact:
the literal forwards control only because its *value* was folded into the
consumer. A literal whose value is live still produces into a temp a sibling
reads. So BOPT-1's forwarder predicate MUST be conservative — skip only nodes
that are **unconditionally pure control forwarders independent of dataflow**:
the existing `IR_SUCCEED`-with-γ and nullary-`IR_SEQ` join, plus provably
value-less control nodes (candidate: bare `IR_GOTO`). Folding away live-LIT hops
is **dead-value elimination**, which belongs with copy-prop (BOPT-3) / CFOLD,
NOT naive branch chaining. Do not overclaim BOPT-1 will collapse the LIT hops;
it collapses the SEQ/SUCCEED/GOTO control hops — the safe, paper-faithful win.

---

## Insertion point (driver)

Lowered graph: `IR_graph_t *sbbg = s2->bbp.table[main_bb_idx]` (driver/scrip.c
~3011 text, ~3323 binary). Emit entry: `gvar_flat_chain_build_text(sbbg, …)`
(~3123) / `gvar_flat_chain_build(sbbg)` (~3326). Precedent pre-emit IR pass at
this seam: `gva_collect_graph(sbbg)` (~3105). **BOPT registers here**, beside
`gva_collect_graph`, in BOTH the text and binary paths. (Edge-rewiring does not
change variable-name counts, so order vs `gva_collect_graph` is free; run BOPT
first so collection/emit see the collapsed graph.)

---

## Step status

- [x] **BOPT-0** — port→field table + realization mechanism + insertion seam.
      This document. No code change.
- [~] **BOPT-1** — ATTEMPTED and FALSIFIED (see below). `src/opt/branchopt.{c,h}`
      written and built but UNWIRED (calls reverted). Do not re-wire as-is.
- [ ] **BOPT-2 / BOPT-3 / gate** — superseded by the finding below.

---

## ⛔ EMPIRICAL FINDING (this session) — BOPT-1-as-in-place-γ/ω-mutation is WRONG

Wrote `bopt_chain(g)`: transitive collapse of the conservative forwarder set
(`SUCCEED`-with-γ, nullary-`SEQ`) by repointing `nd->γ.node` / `nd->ω.node`.
Wired it at the lower→emit seam in both modes, rebuilt clean, then compiled
`arith_loop.sno`. Result:

```
[IBB] FATAL flat_drive_assign: lhs (α) must be IR_VAR with sval (got kind=59)
```

kind 59 = `IR_ASSIGN_CALL`. An `IR_ASSIGN` found an `IR_ASSIGN_CALL` where it
required its LHS `IR_VAR`.

**Root cause — the γ-chain is OVERLOADED.** `gvar_chain_attach_operands`
(emit_bb.c ~3895–3921) walks the γ-spine with a stack to recover each
operator's operands by their *position in the chain* — SNOBOL operands are NOT
all in `operands[]`; many are recovered structurally from the γ-spine (the LIT
feeding an ASSIGN, etc.). Collapsing a `SUCCEED` out of the chain shortens the
spine the operand-recovery walks, so an ASSIGN's LHS var gets displaced → the
FATAL.

**Why the driver's resolver is correct and a mutation pass is not.**
`gvar_chain_resolve_stmt` resolves forwarders **only when computing a jump
target** (`node_γ` / `node_ω` for the label) and **never mutates** the stored
`nd->γ.node`. The edges stay intact for the operand-threading walk; only the
emitted *label* skips the forwarder. So the safe branch-chaining is ALREADY
done (for labels), AND it must be done that way — the graph edges cannot be
rewritten in place because they double as the operand spine.

**Consequence for the whole OPT rung.** For SCRIP's statement IR:
1. The safe forwarder set (`SUCCEED`/nullary-`SEQ`) is already chained for
   labels by the emit-time resolver → a mutation pass over that set is
   redundant *and* unsafe.
2. The residual emitted jump-to-jumps are (a) LIT forwarders — value folded
   into the consumer, dataflow-gated, startup-only (no hot-loop perf), and
   (b) within-box four-port intermediates (`xcat%d_*`, the `β: jmp ω` tails) —
   emit-time, fixed by box type, no IR edge behind them.
3. The hot loop (arith_loop `LOOP`/`LT`) has NO `SUCCEED` forwarders left after
   the resolver; its per-iteration cost is runtime PLT calls per primitive —
   which is the PERF-CALL ladder's target, not branch chaining.

**Net:** the paper's Fig 1→Fig 2 branch-chaining is, for SCRIP's statement
graph, largely ALREADY realized by `gvar_chain_resolve_stmt`. A standalone
branch-optimizer stage that mutates γ/ω does not help and breaks operand
recovery. If a persistent chaining pass is ever wanted (e.g. so the mode-2
interpreter benefits, or as a base for further opts), it must operate on a
SEPARATE control-only view that does not disturb the γ-spine the emitter walks
for operands — not by mutating `IR_t.γ`/`.ω` directly.

---

## QUANTIFIED (this session) — branch-chaining worthless for perf; dead scaffolding is the real finding

`scripts/analyze_jump_redundancy.py` over all 16 SNOBOL benchmark `.s`:

```
TOTAL  code_labels=2444  forwarders=810(33%)  branches=1686
collapsible (branch->forwarder): 34/1686 = 2.0%   max chain = 4
dead forwarders (never branched-to): 793/810
  by kind: beta-resume=645  alpha-start=121  combinator=0  other=27
```

The 34 collapsible branches are ALL in pattern/control-heavy programs (roman 12,
mixed_workload 10, pattern_bt 7, string_pattern 5). Every hot arithmetic / call
/ string / table benchmark scores 0 (arith_loop, fibonacci, func_call,
op_dispatch, string_concat, string_manip, table_access, var_access). A post-emit
branch-chaining peephole — the one place a safe chainer could live (no
operand-spine coupling) — would collapse 2% of branches corpus-wide and nothing
in any hot loop. PEEP/branch-chaining is confirmed worthless for perf.

**Real finding: dead scaffolding.** 793 dead labels (32% of all code labels) are
`label: jmp X` reached by no branch — 645 dead beta-resume stubs. The four-port
template emits a resume port for EVERY box; for non-generator/non-backtracking
boxes it is never targeted. CODE-SIZE / assembly-time cost (bloats `.s` ~1.3x in
labels), NOT runtime — dead code does not execute.

### Identified-but-UNSTARTED rung: DEAD-beta — suppress never-targeted resume ports
Don't emit a box's beta stub when its resume is provably never targeted. Unlike
BOPT this does NOT touch the gamma-spine (beta labels are emit-time, downstream
of operand recovery) — safe in the way BOPT was not. BUT (lesson from the BOPT
crash) "never targeted" is a whole-graph resume-reachability fact; suppressing a
targeted beta breaks backtracking/pattern resume. Needs reachability analysis
first; not a one-line gate. Value is size/build-time, not runtime.

### Runtime hot-loop cost is elsewhere
After the resolver the hot loops have no forwarders left; per-iteration cost is
the runtime PLT call per SNOBOL primitive — the PERF-CALL ladder's target
(table-probe inline, SAB), not the branch/jump family.

---

## Original BOPT-0 analysis (retained below; the HAZARD section predicted part of this)

---

## COPY-PROP measured (this session) — real but tiny, and subsumed by the PLT fix

`scripts/analyze_redundant_reloads.py` over the 16 benchmarks (detector debugged
mid-session — a first version reported a false 0 because the reload itself writes
the reg and tripped the clobber check before the load check; fixed by ordering
the load match first):

```
TOTAL redundant store/reload round-trips: 23   (~1 per benchmark, max 3)
```

These are the paper's copy-prop targets (Fig-2 `mov A,rax; mov rax,A` removal),
and they are genuine — but one stray `mov` per hot box, sitting on the SAME line
as a `call rt_gvar_get_int@PLT`. The call dwarfs the mov by ~an order of
magnitude per iteration.

## CONVERGENCE — the only real lever is the relop operand-read (PERF-CALL)

The arith box reads a GVA global inline (`mov rdx,[rbx+16]; cmp edx,6; jne slow;
mov rax,[rbx+24]; ...; slow: call rt_gvar_get_int`), but the LT relop box (the
`IR_CALL`-inline-relop arm in `bb_call.cpp`, and `bb_binop_gvar_relop.cpp` lines
72/79) reads the SAME global via an unconditional `call rt_gvar_get_int@PLT` —
every iteration, no GVA fast path. Bringing the relop operand-read to parity with
the arith path (the proven pattern in `bb_binop_gvar_arith.cpp`) eliminates the
per-iteration PLT call AND its spill/reload together.

**Net of the whole OPT investigation:** branch-chaining is already done (2%
residual, 0% hot) and unsafe to redo (γ-spine overload — crashes); copy-prop is
~23 instances corpus-wide and subsumed by the PLT fix; the dead β-scaffolding
(645 labels) is size/build-time only. The single performance lever in this
neighborhood is the relop GVA-parity operand-read — a PERF-CALL rung, not an OPT
rung. NOT started here (central template; needs full crosscheck to verify, which
did not fit the session budget). Pattern to copy: `bb_binop_gvar_arith.cpp` GVA
arm. Operand-read sites to fix: `bb_call.cpp` inline-relop arm + `bb_binop_gvar_relop.cpp:72,79`.
