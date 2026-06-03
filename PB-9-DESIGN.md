# PB-9 DESIGN — Pascal onto compiled BBs (mode-3/4)

**Status: DESIGNED, build held (PB-9a is forkless and turn-key; PB-9e is a representation fork for Lon).**
Written after tracing the full mode-3/4 path end-to-end on `hello.pas`. Mode-2 ladder is green
(Pascal 33/0/1, Icon 130/117/36, Prolog 0/0). SCRIP HEAD at design time = `f79fae0`.

This corrects the goal doc's framing of PB-9 as "rebase onto `x86_asm.h` and start." The wiring already
exists; the gaps are precise and enumerated below.

---

## What already works (verified by running it)

- The driver routes `.pas` into BOTH compiled paths with **no `is_pascal` flag needed**: Pascal rides the
  SNOBOL4 flat-chain (the `:subj` proc envelope), so `--run` (mode-3 JIT) enters via the `!is_icon &&
  !is_prolog` branch's `gvar_flat_chain_build`/`bb_build_flat`, and `--compile` (mode-4) emits a real
  `main:`/`sno_flat_α` prologue.
- `scrip --run hello.pas` reaches `bb_call` and aborts with `unsupported call shape fn='__pas_writeln'`.
  `scrip --compile hello.pas` emits the prologue then hits the same `bb_call` wall.
- The runtime trampoline `rt_call_arr(fn, args, nargs)` (`by_name_dispatch.c:1828`) is **implemented** and
  reaches `try_call_builtin_by_name`, where `__pas_writeln`'s interleaved (value,width) loop already lives.
- `marshal_call_arg` (`bb_call.cpp`) already marshals `IR_LIT_S/I/F/NUL` and variable-slot args into the
  16-byte-stride frame slot region, in both MEDIUM_TEXT (mode-4) and MEDIUM_BINARY (mode-3) forms.

## The trap that makes PB-9a more than a one-liner

`rt_builtin_is_known()` (the `int` gate at `by_name_dispatch.c:705`) does NOT list the `__pas_*` family, so
`bb_call`'s `is_builtin` arm never fires for `__pas_writeln`. BUT routing it to the existing
`bb_call_builtin_str` is a dead end: that arm calls `rt_call_builtin(name,nargs)`, which is a
**`STACKLESS_ABORT` stub** (`rt/rt.c:529`) — left behind when the runtime went stackless. The live path is
`rt_call_arr` with a marshalled args array, exactly as the `dval==2.0` arr path uses it.

## Pascal's IR_CALL shape (from `v_det_call`, `lower.c:591`)

`sval` = builtin name · `ival` = nargs · `dval` = **3.0** · `counter` = `IR_graph_t**` of arg subgraphs.
For `writeln('Hello World!')` the parser emits `__pas_writeln('Hello World!', -1)` (width sentinel `-1`),
so nargs=2, arg subgraphs are `IR_LIT_S` then `IR_LIT_I`.

---

## PB-9a — `hello.pas` mode-3/4 seed. FORKLESS. Turn-key recipe.

### Step 1 — add a frame-address helper to `x86_asm.h` (the one missing primitive)
There is no `lea reg, [r12+disp32]` helper today (only `x86_frame_load64`/`store64`, which `mov` values).
Add, beside them:

```cpp
inline std::string x86_frame_lea(const char * reg, int off) {
    // lea reg, [r12 + disp32]   (REX.W + 8D + modrm + SIB(r12) + disp32)   [BINARY]
    // emit the MEDIUM_TEXT "lea reg, [r12+off]" form symmetrically
}
```
Implement it to mirror `x86_frame_load64`'s structure but with opcode `0x8D` (LEA) instead of the `mov`
load, same modrm/SIB/disp32 for `[r12+disp32]`. Verify the bytes against `x86_frame_load64`'s encoding
(they differ only in the opcode byte: `8B` load vs `8D` lea). This helper is reusable by every later rung
that needs a frame address (var-by-reference, record/array bases, etc.), so it is not throwaway.

### Step 2 — Pascal-guarded arm in `bb_call_str` (before the abort)
```cpp
if (fn && !strncmp(fn, "__pas_", 6)) {
    IR_graph_t ** subs = (IR_graph_t **)(intptr_t) pBB->counter;
    int base = (narg > 0 && subs && subs[0]) ? bb_slot_alloc16(subs[0]->entry) : bb_slot_alloc16(pBB);
    for (int j = 1; j < narg; j++) if (subs && subs[j]) bb_slot_alloc16(subs[j]->entry);
    int rslot = bb_slot_alloc16(pBB);                       // the call's own value slot
    std::string s;
    for (int j = 0; j < narg; j++)
        s += marshal_call_arg(subs && subs[j] ? subs[j]->entry : NULL, base + j*16, pBB, j);
    if (MEDIUM_TEXT) {
        // mov rdi, <fn rodata label>; lea rsi, [r12+base]; mov edx, narg;
        // call rt_call_arr@PLT; mov [r12+rslot], rax; mov [r12+rslot+8], rdx; jmp γ
    } else if (MEDIUM_BINARY) {
        uint64_t fptr; { DESCR_t (*fp)(const char*,DESCR_t*,int)=rt_call_arr; fptr=(uint64_t)(uintptr_t)(void*)fp; }
        s += x86("mov", "rdi", (uint64_t)(uintptr_t)fn);
        s += x86_frame_lea("rsi", base);                    // <-- the new helper
        s += x86("mov32", "edx", (long)narg);
        s += x86("call", "rt_call_arr", fptr);
        s += x86_frame_store64(rslot, "rax");
        s += x86_frame_store64(rslot + 8, "rdx");
        s += x86("jmp", PORT_GAMMA);                        // writeln cannot fail → unconditional γ
    }
    return s;
}
```
Notes: (a) `marshal_call_arg` handles only literals + var slots — sufficient for the seed and for
`writeln(intvar)`; expression args (`writeln(i*i)`) need general subgraph evaluation, which is PB-9b.
(b) Unconditional `jmp γ` is correct for `__pas_writeln`/`__pas_write` (always succeed). A general
IS_FAIL → γ/ω branch on the returned tag is a PB-9b refinement once a fallible `__pas_*` builtin is in play.

### Step 3 — gate, build, verify
- `cd src/parser/pascal` is NOT touched (no grammar change). Build: `make -j4 scrip`.
- `scrip --run hello.pas` and `scrip --compile hello.pas | as+gcc-link against libscrip_rt` must both print
  `Hello World!`, byte-identical to `pint`.
- The change is `__pas_*`-prefix-guarded and adds one isolated helper, so cross-language regression is
  guaranteed by construction — but still PROVE it: Pascal 33/0/1, Icon 130/117/36, Prolog 0/0, SNOBOL4
  smoke 2/0, ideally via the stash→rebuild→diff method.

---

## The smallest-first ladder above the seed

- **PB-9b — int arithmetic + assignment + `writeln(int/expr)`.** `IR_BINOP`/`IR_VAR`/`IR_ASSIGN` templates
  exist (`bb_binop_arith`, `bb_var`, the `IR_ASSIGN` dispatch). Needs: general arg-subgraph evaluation in the
  PB-9a arm (so `writeln(i*i)` marshals an evaluated expression, not just a literal), and verification of
  Pascal's var-slot model under the flat chain. IS_FAIL γ/ω branch lands here.
- **PB-9c — control flow. THE TEMPLATE-AUTHORING STRETCH.** `IR_IF`, `IR_WHILE`, `IR_FOR`, `IR_REPEAT` have
  **no emitter templates at all** (confirmed: not in `emit_core.c`'s dispatch). Each must be authored per the
  FACT RULES — one `x86(...)` concat per box, reached only via `emit_core` dispatch, no bytes outside a
  template. The four-port wiring is already correct in the IR (lowered by `v_pascal_for`/`v_pascal_repeat`
  and the shared `v_if`/`v_while`); the work is the x86 encoding of the branch/loop topology. `sieve.pas` is
  the gate.
- **PB-9d — flat procedures/functions + value/`var` params.** `bb_call_proc_staged_str` already stages user
  proc calls under the flat chain; verify Pascal frame-slot seating and the `var`-param reference model
  (`SlotRef`) survive the crossing. `recursion.pas` (through `fact(7)`) is the gate.
- **PB-9e — NESTED procedures. THE REPRESENTATION FORK (Lon's call).** Mode-2 expresses nested scope with a
  `GenFrame` C struct carrying a `static_link` pointer (`gen_runtime.h`) — which Invariants 2 & 4 forbid in
  compiled BBs: *"a Pascal frame is a BB, not a C function,"* static link *"riding the parent-port thread."*
  The fork: how does the mode-2 static chain become BB topology in mode-3/4 — a parent-port thread through
  the activation BBs (no C struct, no display array)? This mirrors PB-7 exactly, where the prior session
  designed it and held the build for Lon's one-word go. Recommendation to be made at PB-9e, not before.

Aggregates (record/set/pointer) ride their existing builtin/array/bitmask/NV-heap rails, so they largely
come along once `IR_CALL` (PB-9a) and `IR_IDX` (PB-9b/c) emit; no dedicated aggregate rung is expected
unless a probe forces one.

---

## Why the build is held here

PB-9a is forkless and the recipe above is turn-key, but it touches the JIT byte-encoding path (the new
`x86_frame_lea` helper), where a wrong byte is a silent segfault rather than a compile error, so it needs a
careful build + 2-mode test + full cross-language regression + commit in one clean pass — a fresh context
budget, not the tail of a long session. PB-9e additionally needs Lon's representation call before the upper
ladder (PB-9c onward sits under it for nested cases). Per the no-broken-commits rule, designing to this
fidelity and stopping is the correct checkpoint.
