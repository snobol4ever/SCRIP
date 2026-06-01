# RK-LOWER-4 — Raku junctions (any/all/one/none + infix |/&) onto the unified lower.c

## What landed

Raku junctions cross onto the shared four-port IR for **mode 2**. Constructor forms
(`any(…)` / `all(…)` / `one(…)` / `none(…)`) and the infix forms (`a | b | c` builds an
`any` junction, `a & b & c` builds an `all` junction) share **ONE** lowering — the parser's
`mk_junction` already flattens same-flavor infix chains into the same `TT_FNC(sval=flavor,
c[0]=TT_VAR(flavor), c[1..]=members)` the constructors produce, so a single Raku-gated arm
inside the shared `TT_FNC` case (`lower.c`) handles every form.

## Lowering (lower.c, TT_FNC value-role case, cx.lang==IR_LANG_RKU arm)

A junction lowers to ONE `IR_CALL` to the runtime builtin `__rk_jct_{any,all,one,none}` with
`dval=2.0` (the SNOBOL4 isolated-value-subgraph call idiom). Each member is lowered into its
OWN value sub-graph via `lower_value_subgraph` (the cursor keeps `cx.lang=IR_LANG_RKU` so a
nested junction member re-enters this same arm); the sub-graph pointer array rides on
`IR_CALL.counter`, member count on `.ival`. The node is deterministic (β=ω_in: junctions are
single-shot values).

**Why isolated sub-graphs, not an inline member γ-chain:** the first cut wired members into a
flat α/γ chain (dval=0.0). That FLATTENED mixed-flavor nesting — `10 | (50 & 60)` collapsed
to just `10 | 50`, losing the nested `all` structure (verified via `cat -v` on the tagged
string: `^Ca^A10^A50^D`). Lowering each member as its own sub-graph (exactly what SNOBOL4
call args do) makes a nested-junction member evaluate to ONE opaque ETX-tagged value, so the
outer junction sees it as a single member. The runtime `junction_collapse` already recurses
on `\x03`-prefixed members using `\x04` (EOT) depth spans, so nested collapse works once the
nested value arrives intact.

## Two dispatch-gap fixes (both were blocking mode-2 junctions)

1. **gen_runtime.c — `try_call_builtin_by_name` tail delegation.** The mode-2 `IR_CALL` exec
   arm calls `try_call_builtin_by_name` (gen_runtime.c), but `__rk_jct_*` lives in
   `script_try_call_builtin_by_name` (script_builtins_byname.c) — which post-SMX-4 had **no
   live call site** (orphaned SM-era arm). Added a tail delegation: names gen_runtime doesn't
   handle fall through to `script_try_call_builtin_by_name`. Placed AFTER every gen_runtime
   arm, so the 6 overlapping names (close/open/pop/push/reverse/trim) keep gen_runtime's
   semantics and no live path is disturbed — only previously-REJECTED names are newly served.
   This is the documented APPENDIX-A "SM dispatch-gap fix" pattern (the same shape that lit the
   regex cluster). Side benefit: the whole script-builtin layer (hash/IO/regex/array) is now
   reachable in mode-2, which later Raku rungs (RK-LOWER-5, RK-NFA) will use.

2. **lower_program.c — `binop_apply` junction-collapse hook.** A junction collapses to a
   Boolean only in a relational context (docs.raku.org/type/Junction). The mode-2 relop
   evaluator is `lower_program.c`'s `binop_apply` (NOT rt.c's — that's the compiled mode-3/4
   runtime). Added a prologue to the relational arms: if either operand `junction_is()`
   (ETX-tagged), call `junction_collapse(scalar, jct, tt_op, numeric)` threading the relop
   (any=OR, all=AND, one=XOR1, none=NONE). Covers BOTH the numeric relops (`==`/`!=`/`<`/…)
   and the string relops (`eq`/`ne`/`lt`/… → `numeric=0` → string member compare). The ETX
   prefix is used by no SNOBOL4/Icon/Prolog value, so the guard is FACT-RULE-safe in shared C
   (verified: Icon 6/6, SNOBOL4 7/7 unchanged).

## One Raku-gated semantic fix (lower.c, v_if)

The canonical `rk_junctions.raku` interleaves passing and FAILING `if`s and expects the
sequence to continue past a failed condition. A failed scalar `if ($x == 7)` was aborting the
whole statement sequence — confirmed **pre-existing** (clean tree, `git stash`), independent
of junctions. Root cause: Raku void-statement semantics. Per docs.raku.org/language/control,
an `if` with no `else` whose condition is false yields `Empty` and execution **continues** —
it does NOT fail. Fixed inside the shared `v_if` (FACT RULE: variation branches on `cx.lang`):
for `IR_LANG_RKU` a no-else condition-failure routes to `γ_in` (continue) instead of `ω_in`
(fail). Icon/SNOBOL/Prolog keep `ω_in` (goal-directed expression failure, jcon ir_a_If).

## Tests (all green)

- `test/raku/rk_junctions.raku` — flat any/all/one/none + infix, 6 hits / 3 skips → MATCH
- `test/raku/rk_junction_nest.raku` — mixed-flavor nesting → MATCH
- `test/raku/rk_junction_prec.raku` — precedence + var round-trip + string collapse + nesting → MATCH
- `scripts/test_smoke_raku.sh` — +7 junction cases (jct_any/all/one/none/infix/str/nested) → 17/17
- `scripts/prove_lower2.sh` — +2 topology cases (flat any = 1 node; nested any = 1 node) → 57/57

## Modes 3/4

By-design SMX abort (RK-EMIT not built), identical across both modes — same shape as SNOBOL4.
No emitter touched, so mode-3≡mode-4 by construction. Floors MODE3_MIN/MODE4_MIN=0 hold.

## Files touched (mode-2 only; ZERO emitter)

- `src/lower/lower.c` — junction arm in TT_FNC; Raku void-if in v_if
- `src/lower/lower_program.c` — binop_apply junction-collapse hook
- `src/runtime/interp/gen_runtime.c` — try_call_builtin_by_name tail delegation
- `src/lower/prove_lower2.c` — 2 RAKU topology cases + jct() helper
- `scripts/test_smoke_raku.sh` — 7 junction smoke cases

## Pre-existing baseline items (NOT mine — confirmed by git stash on clean tree)

- `audit_concurrency_invariants.sh` purity COUNT 7 > baseline 6: all 7 are loud-error fprintf
  in Icon/SNOBOL boxes, ZERO Raku. FIX OWED BY ICON/HQ: bump baseline 6→7.
- `test_gate_em_template_byte_identity.sh` = 1/4 (SM/BB emitter migration mid-flight).

## Authors

Lon Jones Cherryholmes · Jeffrey Cooper M.D. · Claude Sonnet · Claude Opus
