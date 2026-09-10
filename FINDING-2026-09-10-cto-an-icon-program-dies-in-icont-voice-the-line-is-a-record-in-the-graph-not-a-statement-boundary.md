# FINDING 2026-09-10 (cto) — an Icon program dies in icont's voice: the line is a RECORD in the graph, not a statement boundary

Row: `an-icon-program-prints-snobol4s-termination-report-shape-a-language-identity-leak-in-a-shared-runtime-sink` (hq_B mint 2026-09-09; the ceo's order of work after the island, CEO-452/471).

## The leak

Three sinks printed a fatal Icon error. `core_runtime_error` printed SPITBOL's shape (`(0) : ERROR 201 -- ... / in statement 0`) for every program that had no statement number, which is every Icon program; the `runerr` builtin printed `Run-time error N` and the offending value and nothing else; `core_icn_error` printed the line from `g_line`, which no Icon graph ever stored. The runtime knew no line because the Icon lowerer emitted a statement record only under `&trace`, and that record was an `ICN$LINE` CALL per statement, a full by-name call.

## The cure, language-blind

A new IR kind, `IR_LINE_MARK` (`src/ir/IR.h`), is a record: the emitter's blob (`bb_line_mark`, beside `bb_stmt_mark`) stores the line into `g_line` and proceeds. It is NOT `IR_STMT_MARK`. A statement mark is a BOUNDARY: `fl_derive_tier` counts it toward a statements graph, `fct_rsp_range` and the slot granter end lifetimes at it, and the driver resets the frame-carve accumulator (`op_fc_bytes`) there. Marking an Icon statement with the boundary kind broke every loop body: `repeat { if ... ; i +:= 1 }` printed nothing and the R5 gate's `every ... do { f(); n +:= 1 }` segfaulted in both modes, because the loop's spine state was released at each statement. The record kind touches none of the four sites.

The mark enters its successor at α, wired with `lc_γ_to`/`lc_ω_to` like the `ICN$LINE` hook, never through `build()`: `build()` routes a γ edge to a generator-kind successor's β (resume) port, and the asm diff of the smoke `repeat_break` showed `n8_line_mark_α: ... jmp n9_disjunction_β` -- the loop head resumed instead of entered. ASM-DIFF-FIRST named it in one diff.

The proc body hooks `ICN$LINE` (file and line) at its FIRST statement only, so `g_file` is set once per activation; every other statement, at any depth, carries a line mark. Under `&trace` every statement keeps the hook, as before.

The sinks: `core_runtime_error` prints icont's shape when `g_stno == 0 && g_line > 0` (code 2 → 201 division by zero, 22 → 106, the SNOBOL4 codes an Icon program was wearing); `runerr` routes through `core_icn_error`; `core_icn_error` prints `File <basename>; Line <n>`, the message, `offending value: <image(v)>` through the `image` builtin, and `Traceback:` / `main()`, and exits 1 as icont does.

## Measured

Witnesses (m3 and m4): `traceback.icn` → `Run-time error 201 / File traceback.icn; Line 11 / division by zero / Traceback: / main()`; `loadfunc.icn` → 216 at line 22; `f1.icn` (`1 := y`) → 111 with `offending value: 1` at line 4, icont's text to the byte, bar the per-frame traceback lines (below). `repeat_break` and the R5 gate green again in both modes. Five smokes green (icon 15/15, snobol4 7/7, prolog 5/5, pascal 9/9, raku 10/10); comment gate 0; the zeta-switch, semicolon, coexpr and R5 gates PASS. DONE-WHEN: `PASS: 0 of 123 Icon programs print SNOBOL4 termination report shape` (Arizona general, the suite's own census). Icon ladder: 10 of 686 gradings red, all five witnesses red identically on the origin binary (rungs 9, 19, 20, 30, 41: standing reds in hq_S's lane, not this landing's).

Arizona general, per-program abort census on the same corpus: this tree 22 aborts and 1 segfault; the origin control at db870418b read 15 and 1 only because its worktree has no sibling `corpus/` and seven `link`-directive programs (cfunc, levensht, lists_lib, math, numbers, random_lib, rational) fail at link there before the driver runs. All seven are main-less library files; a main-less Icon file aborts `[IBB] FATAL: mode-3 driver: main BB graph not found` on BOTH binaries where icont reports `no main procedure` -- a pre-existing class, named below. Icon master board: both-modes PASS=754/757, the three reds (`procedure_write_256`, `procedure_every_scan_replace_13`, `procedure_record_every_replace_12`) identical on the origin binary and on hq_R's clean board of b9f9bc465 at 14:03Z. SNOBOL4 master arm: both-modes PASS=1893/1917, FAIL=0 both modes, the standing xfail=24.

## Residue, by name

- icont prints one traceback line per frame with the failing expression's text (`{1 := 5} from line 4 in f1.icn`); the runtime has no expression text and prints `main()` alone. That is a second row: the text is a per-node string the lowerer would have to carry.
- After a callee returns, `g_line` holds the callee's last line until the caller's next statement mark; an error later in the SAME caller statement reports the callee's line. The hook at proc entry does not restore on return.
- `core_icn_error` under `&error` (the recoverable branch) is untouched.
- A main-less Icon file (a library linked by others) aborts in the driver, `main BB graph not found`, where icont reports `no main procedure`; the driver's refusal is a report of its own, the third sink of this kind.
