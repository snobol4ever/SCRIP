# RK-LOWER-3 — lazy `map`/`grep` as Seq consumers (design, grounded in docs.raku.org)

## Verified Raku semantics (official sources, fetched 2026-05-31)

**map** — docs.raku.org/routine/map + /type/List#routine_map:
> `multi map(&code, +values)` — "invokes `&code` for each element and gathers the return
> values in a sequence and returns it. This happens lazily, i.e. `&code` is only invoked when
> the return values are accessed." Topic is `$_`. Returns a `Seq`. Does NOT flatten embedded
> lists/arrays (each element passed once). Example: `('hello',1,22/7).map: {.^name}` → `(Str Int Rat)`.

**grep** — docs.raku.org/routine/grep + /type/List:
> `sub grep(Mu $matcher, *@elems --> Seq:D)` — "Returns a sequence of elements against which
> `$matcher` smartmatches. The elements are returned in the order in which they appear in the
> original list." For a `{ … }` block matcher (a Callable), smartmatch applies the block to `$_`
> and KEEPS the element iff the block returns a true value.
> Example: `grep { .Str.chars > 3 }, 'hello',1,22/7,42,'world'` → `(hello 3.142857 world)`.

## RK-LOWER-3 scope (mirrors RK-LOWER-2's for-driven gather scope)

The for-driven consumer form: `for map { BODY } SOURCE -> $v { … }` and
`for grep { PRED } SOURCE -> $v { … }`, where SOURCE is any lowerable Seq producer
(an `IR_TO` range or an `IR_GATHER`). The bare-value form `say map {…} 1..3` (map delivering a
whole List as a value to `say`) depends on List-value representation = RK-LOWER-5; deferred,
exactly as RK-LOWER-2 deferred bare-value gather to the for-driven case.

AST (verified via --dump-ast):
  `for map {C} S -> $v`  → TT_EVERY(TT_ITERATE("v", TT_MAP(C, S)), body)
  `for grep {P} S -> $v` → TT_EVERY(TT_ITERATE("v", TT_GREP(P, S)), body)
  TT_MAP/TT_GREP children: c[0] = closure body expr (reads `$_`), c[1] = source expr.

## Mechanism (REUSE the IR_GATHER resumable-producer model + the aggregate_all eager-drain)

Two NEW resumable Seq-producer kinds `IR_MAP` / `IR_GREP` (own resume, β=self — like IR_TO /
IR_GATHER), reached from the Raku arm of the shared TT_EVERY/TT_MAP/TT_GREP cases. Both:
  - lower SOURCE into its OWN value sub-graph (lower_value_subgraph, cursor carries IR_LANG_RKU);
    ptr rides on `.counter` (PRESERVED across bb_reset, like IR_GATHER/IR_SCAN).
  - lower the closure BODY into a SECOND sub-graph; ptr rides on `.ival` (cast). The body reads
    `$_` via IR_VAR("_") → NV_GET_fn("_"); we set `_` before each run with NV_SET_fn.
  - resume cursor on `.state`.

Exec arm (bb_exec.c), on FRESH entry (state==0): eager-drain SOURCE — the aggregate_all idiom
`bb_reset(src); v=bb_exec_once(src); while(!FAIL){collect; v=bb_exec_resume(src);}` — into a
malloc'd DESCR_t[] of source elements (the eager-drain the GOAL names). THEN, per (re)entry,
advance the cursor:
  - MAP : NV_SET_fn("_", src[i]); run closure; yield the closure's value to γ (advance i).
  - GREP: for i forward from cursor, NV_SET_fn("_", src[i]); run closure; if NOT FAIL (truthy
    per binop_apply's relational fail-on-false convention) yield src[i] to γ; else skip. Drain → ω.
Walking past the last element (or empty source) → reset cursor, FAIL to ω (Seq drained) — the for
STATEMENT then completes, identical to IR_GATHER.

Driven by the EXISTING generator PUMP via v_raku_for (body.γ → gen.β re-pump). NO new pump
machinery, NO new emitter (mode-2 only this rung, same as RK-LOWER-0/1/2).

## Additive wiring sites (mirror IR_GATHER's 7 sites)
  IR.h enum (before IR_SNO_PROG); scrip_ir.c name table + bb_reset counter-preservation list;
  lower.c kind_is_resumable + bb_is_gen_kind_raw + v_raku_map/v_raku_grep + TT_EVERY guard +
  TT_MAP/TT_GREP cases (pulled out of the unhandled group, Raku-gated); bb_exec.c exec arms +
  kind_is_gen_kind/ir_is_single_shot lists; prove_lower2.c RAKU proof cases.

FACT RULE: all Raku-gated (cx.lang==IR_LANG_RKU); non-Raku TT_MAP/TT_GREP → lower_unhandled.
Zero emitter files; Icon/SNOBOL4/Prolog arms untouched.
