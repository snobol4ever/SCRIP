# FINDING 2026-09-09 (cto) — the empty expression and a literal swap operand had no box, so the compiler aborted where Icon raises 111 and 102

Row: `icon-assignment-to-a-non-lvalue-aborts-the-compiler-where-icon-raises-runtime-error-111` (hq_B minted 2026-09-04; the ceo named its program, `rung36_jcon_errors`, the m4 compiler abort blocker in CEO-445 on Lon's Icon-only order, 2026-09-09 09:0x CDT).

## The claim

`rung36_jcon_errors` aborted the compiler in BOTH modes (`FATAL emit_drive: IR op=16`, then `op=122` once 16 was cured). Neither op lacked a template. Both were the guard sink inside an existing case, fired because the LOWERER handed the box an operand that was wiring, not a value:

1. **`()` — the empty expression — lowered to a bare `IR_SUCCEED`.** `lower_icon.c` TT_SEQ_EXPR with zero statements built a wiring node with no value slot, so `100 - ()` gave `IR_COERCE_NUMERIC` an operand with no slot (op=16, emit.cpp:1682 guard). Icon says `()` and `{}` ARE a value: `&null` (icont prints `&null` for `image(())`). Cure: the zero-statement sequence lowers exactly as `TT_NUL` does — an `IR_VAR "&null"`. Also cures `x := ()` (BOMB `bb_assign_local: needs ... rhs slot`) and `write(image(()))` (BOMB `bb_call marshal`).
2. **A literal side of `:=:`, `<-`, `<->` lowered to `IR_SWAP` with a nameless operand** (op=122, emit.cpp:1932 guard `bb_varslot_peek`), or to a silent `IR_FAIL` for `<-`/`<->`. Icon raises runtime error 111 "variable expected" with the literal as `&errorvalue`. Cure: the same idiom TT_ASSIGN already used for `1 := y` — lower to `CONJ(other side, runerr(111, literal))`. Only the four literal tree kinds (`TT_QLIT TT_ILIT TT_FLIT TT_CSET`) are diverted; a call or subscript side keeps its existing path.
3. **Found on the way, one shared runtime box:** `rt_coerce_num2_d`'s hand-written fast path (`rtx_icnnum.s`) read `&null` and a blank string as 0 without consulting `COERCE_ERR_FAILURE_CONVERTIBLE` (0x1000000), the flag Icon's lowerer sets so the C path raises 102. `1 + &null` and `"" + 1` printed 1 where icont raises 102. Cure, in isolation: under that flag a null self and a blank string self bail to `c_rt_coerce_num2_d`, which already raises. SNOBOL4 never sets the flag; its null-is-zero arithmetic is untouched (the SNOBOL4 master arm is the control).

## Witnesses (all byte-identical to icont/iconx in both modes after the cure)

- `write(image(()))`, `write(image({}))`, `every write(image(() | {}))`, `x := (); write(image(x))` → `&null` ×4.
- `&error := -1; write(image((100-()) | "none")); write(&errornumber, " ", image(&errorvalue))` → `"none"` / `102 &null`.
- `(x :=: "a")`, `(x <-> "b")`, `((1 <- y) & &fail)`, `((1 <-> y) & &fail)`, `((x & 2 & 3 & 4) := 3)` → 111 with `"a"`, `"b"`, `1`, `1`, `4`; `x :=: y` and `x <-> y` on two variables still swap.
- `("" + 1)` → 102 `""`.

## What remains red in `rung36_jcon_errors` (not this row; named for routing)

It now COMPILES in both modes and runs to line 113 of 182 before a SIGSEGV in `scan_match` (`memcmp` on a non-string subject), same site both modes. The cause is upstream of the crash: `[] ? []` (line 92) installs a list as `&subject` and returns `list_8(0)` where icont raises 103 "string expected" before scanning; `(&subject := [])` (line 90) FAILS silently where icont raises 103. With a list left in `&subject`, `=[]` at line 113 memcmp's it. Two builtin-class reds also stand in both modes: `seq("a")` returns 0 where icont raises 101; `"|"(1,2)` reports our error 22 "Undefined function called" where icont raises 106 (string invocation of an operator name — hq_U's named class). The corpus marker is re-cut to say exactly this.
