# XA-OPCODE-INVENTORY.md — EAO-1 audit

Inventory of every emission-producing function in `emit_sm.c` and `emit_bb.c`
that fires **outside** the SM/BB template dispatch path.

Each entry: function name / asm block description / call site(s) / verdict.

---

## emit_sm.c

### 1. `emit_sm_stno` (lines 25–37)

**Asm block:** `mov edi, <stno>` + `call rt_set_stno@PLT` (TEXT path); binary
`insn_mov_edi_i32` + `emit_call_sym_plt` (BIN path).

**Call sites:** None outside `emit_sm.c` itself. Declared in `emit_templates.h:28`
but never called from any other translation unit.

**Verdict: DEAD CODE.** The SM_STNO opcode is already fully handled by
`sm_stno()` in `SM_templates/sm_compare.c` for all five backends. The
`emit_sm_stno` function is an orphaned pre-template implementation. **Delete
`emit_sm_stno` from `emit_sm.c` and its declaration from `emit_templates.h`.**

---

### 2. `emit_sm_set_pc_label` / `emit_sm_consume_pc_label` (lines 40–64)

**Asm block:** No asm emission — these manage a `g_pending_pc_label[32]` string
buffer used to pass a pending label from set-site to consume-site.

**Call sites:** Declared in `emit_sm.h:32` (`consume`) but **neither function is
called from any TU other than `emit_sm.c` itself.** No caller in `emit_core.c`,
no caller in any SM/BB template.

**Verdict: DEAD CODE.** The pending-pc-label mechanism was superseded by the
PIVOT/LABELFIX approach in `emit_walk_codegen` (lines 1352–1360): the dispatch
loop emits `.L<pc>:` directly via `fprintf(out, ".L%d:\n", pc)` without using
this buffer. **Delete both functions, `g_pending_pc_label`, and the `emit_sm.h`
declaration.**

---

### 3. `emit_sm_macro_library` (lines 66–121)

**Asm block:** Writes `.intel_syntax noprefix` preamble, then calls
`emit_sm_dispatch()` once per SM opcode group representative to fire every
IS_MACRO_DEF arm. Full GAS macro definition preamble.

**Call sites:**
- `emit_sm_macro_library_to_path` (line 132) — which is called only from
  `emit_walk_codegen` (line 1323) inside the `!g_emit_inline` branch.
- No external callers.

**Proposed XA opcode:** `XA_MACRO_LIBRARY`

Template file: `XA_templates/xa_macro_library.c`

IS_X86 arm: emits `.intel_syntax noprefix` header then loops over
`one_per_group[]` dispatching `emit_sm_dispatch()` for each representative
(absorbing the body of `emit_sm_macro_library`).

---

### 4. `emit_sm_macro_library_to_path` (lines 123–135)

**Description:** Thin wrapper — opens a file, calls `emit_sm_macro_library`,
closes. Called only from `emit_walk_codegen`.

**Verdict:** Wrapper that disappears once `XA_MACRO_LIBRARY` is wired into
`emit_walk_codegen` directly (no file indirection needed if inline). OR keep the
file-write path — `emit_walk_codegen` writes `sm_macros.s` then `.include`s it.
That file-write path itself becomes `xa_dispatch(XA_MACRO_LIBRARY)`. No separate
XA opcode needed for the path wrapper.

---

### 5. `emit_sm_exec_stmt_blob` (lines 1282–1305)

**Asm block:** 4-instruction pattern-match call sequence:
```
lea rdi, [rip + pat_<id>_α]
lea rsi, [rip + <subj_lbl>]   # OR: xor esi, esi
mov edx, <has_repl>
call rt_match_blob@PLT
```

**Call sites:** One — `emit_walk_codegen` line 1373, inside the
`win_exec >= 0 && is_invariant` branch. Not called anywhere else.

**Proposed XA opcode:** `XA_EXEC_STMT_BLOB`

Template file: `XA_templates/xa_exec_stmt_blob.c`

IS_X86 arm: emits the 4-instruction sequence (absorbing `emit_sm_exec_stmt_blob`).
Parameters passed via `g_emit` fields: `g_emit.instr` carries `sname`/`has_repl`;
`g_emit.i` (or a new `g_emit.win_idx`) carries `win_idx`. The `w->pat_id` lookup
stays in the dispatch wrapper.

---

### 6. `emit_file_header` (lines 677–737)

**Asm block:** `.intel_syntax noprefix`, `.globl main`, `main: push rbp`,
`mov rbp, rsp`, optional `lea rdi + call rt_register_expressions@PLT`, optional
`call rt_register_predicates_pl@PLT`, cap-fixup loop (`lea rdi/rsi + call
rt_init_arbno/rt_init_cap_call/rt_init_cap`), `call rt_init@PLT`.

**Call sites:** One — `emit_walk_codegen` line ~1360. Static to `emit_sm.c`.

**Proposed XA opcode:** `XA_FILE_HEADER`

Template file: `XA_templates/xa_file_header.c`

IS_X86 arm absorbs the full header sequence. Flags (`has_expression_registry`,
`has_pl_registry`) and cap-fixup array passed via globals already accessible
(`g_cap_fixups`, `g_cap_fixups_n`, `expression_reg_count`, `pl_reg_count`).

---

### 7. `emit_file_footer` (lines 738–753)

**Asm block:** `call rt_finalize@PLT`, `pop rbp`, `ret`, `.size main, .-main`,
`.section .note.GNU-stack`.

**Call sites:** One — `emit_walk_codegen` line ~1405. Static to `emit_sm.c`.

**Proposed XA opcode:** `XA_FILE_FOOTER`

Template file: `XA_templates/xa_file_footer.c` (or fold into `xa_file_header.c`
as `XA_FILE_FOOTER`).

---

### 8. `strtab_emit_rodata` (lines 363–376)

**Asm block:** `.section .rodata` header + one `.string` line per interned string
+ `.text` footer. Emits rodata section for string literals.

**Call sites:** One — `emit_walk_codegen` line ~1356. Static.

**Proposed XA opcode:** `XA_RODATA_STRTAB`

Template file: `XA_templates/xa_rodata.c` IS_X86 arm. (JVM/JS/NET/WASM arms are
stubs for now — non-x86 backends handle string storage differently.)

---

### 9. `emit_expression_registry` (lines 378–418)

**Asm block:** `.section .data`, `.align 8`, `.Lexpression_registry:` label,
`.quad <name>` / `.quad <fn>` pairs, sentinel `.quad 0 / .quad 0`, `.text`.

**Call sites:** One — `emit_walk_codegen` line ~1357. Static.

**Proposed XA opcode:** `XA_EXPRESSION_REGISTRY`

Template file: `XA_templates/xa_expression_registry.c` IS_X86 arm.

---

### 10. `emit_pl_predicate_registry` (lines 578–623)

**Asm block:** Rodata + data sections for Prolog predicate table.

**Call sites:** One — `emit_walk_codegen`. Static.

**Proposed XA opcode:** `XA_PL_PREDICATE_REGISTRY`

Template file: `XA_templates/xa_pl_registry.c` IS_X86 arm.

---

### 11. PC-label emit in `emit_walk_codegen` dispatch loop (line ~1352)

**Asm block:** `fprintf(out, ".L%d:\n", pc)` — emits a `.L<pc>:` label inline in
the dispatch loop, not routed through any function.

**Call sites:** Inline in `emit_walk_codegen`.

**Proposed XA opcode:** `XA_PC_LABEL`

Template file: `XA_templates/xa_pc_label.c`. Replaces the raw `fprintf` with
`xa_dispatch(XA_PC_LABEL)` after setting `g_emit.i = pc`.

---

## emit_bb.c

### 12. `emit_bb_ptr_slot` (lines 106–117)

**Asm block:**
```
.section .data
<zlbl>: .quad 0
.section .text
.intel_syntax noprefix
```
Allocates a named zero-initialised data slot for a BB node pointer and returns
the label.

**Call sites:**
- `BB_templates/bb_arbno.c:31` — one call
- `BB_templates/bb_capture.c:50, 78` — two calls
- `emit_bb.c:415, 441` — two calls (inside `emit_flat_ir` pattern arms)

**Proposed XA opcode:** `XA_BB_PTR_SLOT`

Template file: `XA_templates/xa_bb_ptr_slot.c`. The label output
(`zlbl_out[80]`) is an out-param — this can stay as a helper-function call
rather than a pure opcode, or the label can be returned via a global
`g_xa_last_ptr_slot_lbl`. The 5 call sites are in BB templates and `emit_bb.c`
— not in `emit_walk_codegen` — so this is a different category: a **helper used
by BB template functions**, not a walker-level emission block. A simpler
disposition is: **keep as a BB helper function, but route its body through
`xa_dispatch`** using a small wrapper that writes the label to a global.

---

### 13. `emit_flat_entry_dispatch` (lines 311–327)

**Asm block:**
```
cmp esi, 0
je  <α_body>
jmp <β>
```
The Byrd-box entry trampoline — routes α-port vs β-port.

**Call sites:** Only via `flat_box_entry_dispatch(a,b)` macro in `emit_bb.h:49`,
which expands to `emit_flat_entry_dispatch(a,b)`. The macro is called only from
`emit_flat_body` (line ~486) — itself called only from `bb_build_flat`,
`bb_build_brokered`, and `emit_flat_build` (within `emit_sm.c`).

**Proposed XA opcode:** `XA_ENTRY_DISPATCH`

Template file: `XA_templates/xa_flat.c` IS_X86 arm. The two label args
(`lbl_alpha_body`, `lbl_beta`) are passed via globals or kept as function params
on the XA call. Given this fires only inside `emit_flat_body`, the simplest
route is `xa_dispatch(XA_ENTRY_DISPATCH)` with labels stashed in `g_emit`
temporarily.

---

### 14. `emit_flat_body` prologue (lines 483–490)

**Asm block:** `.globl` declarations + `lea r10, [rip + Δ]` setup, emitted
before `emit_flat_ir` walk:
```
.globl <α>
.globl <β>
.globl <γ>
.globl <ω>
<α>:
lea r10, [rip + Δ]          # emit_sym_lea_r10
```

**Call sites:** Internal to `emit_flat_body` (static). Always runs when
`text_externalise == 1`.

**Proposed XA opcode:** `XA_FLAT_PROLOGUE`

Template file: `XA_templates/xa_flat.c` (same file as `XA_ENTRY_DISPATCH`).

---

### 15. `emit_bb_macro_library_to_path` (lines 625–674+)

**Asm block:** Full BB macro library — `.macro` / `.endm` blocks for DELTA_LOAD,
SIGLEN_LOAD, EPS_α, EPS_β, FAIL_α, and all port-call macros.

**Call sites:** `emit_walk_codegen` line ~1330 (via `emit_bb_macro_library_to_path`).
Exported in `emit_bb.h:16`. No other callers found.

**Proposed XA opcode:** `XA_BB_MACRO_LIBRARY`

Template file: `XA_templates/xa_bb_macro_library.c` IS_X86 arm (absorbs the
`bm_*` call sequence). OR fold into `XA_MACRO_LIBRARY` as a second phase.

---

## Summary table

| # | Function | File | XA opcode | Verdict |
|---|----------|------|-----------|---------|
| 1 | `emit_sm_stno` | emit_sm.c | — | **DEAD — delete** |
| 2 | `emit_sm_set_pc_label` / `emit_sm_consume_pc_label` | emit_sm.c | — | **DEAD — delete** |
| 3 | `emit_sm_macro_library` | emit_sm.c | `XA_MACRO_LIBRARY` | route |
| 4 | `emit_sm_macro_library_to_path` | emit_sm.c | (wrapper, folds into #3) | simplify |
| 5 | `emit_sm_exec_stmt_blob` | emit_sm.c | `XA_EXEC_STMT_BLOB` | route |
| 6 | `emit_file_header` | emit_sm.c | `XA_FILE_HEADER` | route |
| 7 | `emit_file_footer` | emit_sm.c | `XA_FILE_FOOTER` | route |
| 8 | `strtab_emit_rodata` | emit_sm.c | `XA_RODATA_STRTAB` | route |
| 9 | `emit_expression_registry` | emit_sm.c | `XA_EXPRESSION_REGISTRY` | route |
| 10 | `emit_pl_predicate_registry` | emit_sm.c | `XA_PL_PREDICATE_REGISTRY` | route |
| 11 | PC-label fprintf in dispatch loop | emit_sm.c | `XA_PC_LABEL` | route |
| 12 | `emit_bb_ptr_slot` | emit_bb.c | `XA_BB_PTR_SLOT` | route (BB helper) |
| 13 | `emit_flat_entry_dispatch` | emit_bb.c | `XA_ENTRY_DISPATCH` | route |
| 14 | `emit_flat_body` prologue | emit_bb.c | `XA_FLAT_PROLOGUE` | route |
| 15 | `emit_bb_macro_library_to_path` | emit_bb.c | `XA_BB_MACRO_LIBRARY` | route |

**Dead code to delete immediately (EAO-1 finding):**
- `emit_sm_stno` + declaration in `emit_templates.h`
- `emit_sm_set_pc_label` + `emit_sm_consume_pc_label` + `g_pending_pc_label` + declaration in `emit_sm.h`

**XA opcodes to create (EAO-2 onward):**
`XA_MACRO_LIBRARY`, `XA_BB_MACRO_LIBRARY`, `XA_EXEC_STMT_BLOB`,
`XA_FILE_HEADER`, `XA_FILE_FOOTER`, `XA_RODATA_STRTAB`,
`XA_EXPRESSION_REGISTRY`, `XA_PL_PREDICATE_REGISTRY`, `XA_PC_LABEL`,
`XA_BB_PTR_SLOT`, `XA_ENTRY_DISPATCH`, `XA_FLAT_PROLOGUE`.

