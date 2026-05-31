# CHUNKS-step17-rename-validation.md

**Rung:** CH-17-RENAME-a through CH-17-RENAME-g  
**Session:** 2026-05-09  
**Status:** CH-17-RENAME-g LANDED

---

## Part 1 — `EXPR_t` / `EXPR_e` / `E_*` → `AST_t` / `AST_e` / `AST_*` (AR-1 + AR-2)

Executed as `GOAL-AST-RENAME` (carved and executed same session, 2026-05-09).

### Why `AST_t` not `IR_t`

The parse tree built by frontends from source tokens is an **AST**
(Abstract Syntax Tree): kind + sval/ival/dval + children[].  `SM_Program`
is the flat stack-machine instruction array produced by `sm_lower` — that
is the IR.  Calling them by their correct names matches standard compiler
vocabulary (CPython, Clang, GCC all use AST for the source-faithful tree
and IR for the lowered form).

### Mapping table (C side — AR-1)

| Old name | New name | Location |
|----------|----------|----------|
| `EXPR_t` | `AST_t` | `src/ir/ir.h:251` |
| `EXPR_e` | `AST_e` | `src/ir/ir.h:44` |
| `E_QLIT`, `E_VAR`, … (100+ enum values) | `AST_QLIT`, `AST_VAR`, … | `src/ir/ir.h:45–191` |
| `expr_e_name[]` / `ekind_name[]` | `ast_e_name[]` | `src/ir/ir.h:360` |

**Files touched:** 73 C source files across `src/`, `test/`, `scripts/`,
plus `.cs`/`.java`/`.js` driver stubs.

**Regex used:** `\bE_([A-Z][A-Z_]*)\b` → `AST_\1` (word-boundary safe;
verified zero false positives pre-run).

### Mapping table (parser + oracle side — AR-2)

| Old string literal | New string literal | Files |
|--------------------|--------------------|-------|
| `'E_VAR'`, `'E_FNC'`, … | `'AST_VAR'`, `'AST_FNC'`, … | 6 `parser_*.sc` files |
| `"E_VAR"`, `"E_FNC"`, … | `"AST_VAR"`, `"AST_FNC"`, … | 164 `.ref` oracle files |

**Supporting `.sc` files updated:** `ShiftReduce.sc`, `qize.sc`,
`semantic.sc`, `smoke.sc`, `tdump.sc`.

### Gates (AR-1 + AR-2)

All byte-identical to baseline:
- smoke ×6: PASS (7/7, 5/5, 5/5, 5/5, 4/4, 5/5)
- isolation: PASS
- unified_broker: PASS=49
- scrip_all_modes: PASS=2
- Icon corpus: PASS=186 FAIL=47 XFAIL=30 (unchanged)
- SCRIP @ `4c96e9e7`, corpus @ `734bb92`

---

## Part 2 — `chunk` → `expression` vocabulary (CH-17-RENAME-g)

### Why `expression` not `chunk`

The `SmExpression_t` type (formerly `SmChunk_t`) is the **compiled
expression** — a contiguous range of SM ops addressed by `entry_pc`,
callable as a unit, generator-capable when driven through
`bb_broker_drive_sm`.  This is what code that says "evaluate this
expression" actually means: a pure functional region with inputs (arity
args on the SM stack), outputs (TOS + `last_ok`), and no AST walking at
runtime.  Calling it a "chunk" obscured what it does and why GOAL-CHUNKS
exists.  `expression` restores that truth.

### Carving decisions

Two sub-rungs carved from CH-17-RENAME-g spec:

1. **`EXPR_t` rename first** (AR-1/AR-2 above) before the `chunk` →
   `expression` rename.  Rationale: `EXPR_t` (~1391 occurrences) was the
   more pervasive name; landing it first removed the ambiguity where
   `EXPR` could mean either the AST-side `EXPR_t` or the new
   compiled-expression vocabulary.  Once the AST side is `AST_t`, the
   expression rename is unambiguous.

2. **Comment sweep included in same rung** rather than split to
   CH-17-RENAME-h.  Rationale: comments that say "chunk" when they mean
   "expression" are as misleading as type names that say "chunk";
   combining them keeps the diff coherent and the gate budget the same.

### Mapping table

| Old identifier | New identifier | Kind |
|----------------|----------------|------|
| `SM_PUSH_CHUNK` | `SM_PUSH_EXPRESSION` | opcode enum + string table |
| `SM_CALL_CHUNK` | `SM_CALL_EXPRESSION` | opcode enum + string table |
| `SmChunk_t` | `SmExpression_t` | struct typedef (`sm_prog.h`) |
| `pl_chunk_t` | `pl_expression_t` | local typedef (`pl_broker.c`) |
| `sm_call_chunk` | `sm_call_expression` | function (`scrip_rt.c`, `sm_interp.h`) |
| `push_chunk_descr` / `scrip_rt_push_chunk_descr` | `push_expression_descr` / `scrip_rt_push_expression_descr` | function + PLT symbol |
| `scrip_rt_register_chunks` | `scrip_rt_register_expressions` | function |
| `rt_chunk_entry` | `rt_expression_entry` | struct typedef (`scrip_rt.h`) |
| `ChunkRegEntry` | `ExpressionRegEntry` | struct typedef (`scrip_rt.c`) |
| `CHUNK_REG` / `CHUNK_REG_MAX` | `EXPRESSION_REG` / `EXPRESSION_REG_MAX` | `#define` (`scrip_rt.c`) |
| `g_chunk_reg` | `g_expression_reg` | static array (`scrip_rt.c`) |
| `g_chunk_body_lowering` | `g_expression_body_lowering` | flag var (`sm_lower.c`) |
| `sm_emit_push_chunk` | `sm_emit_push_expression` | function (`sm_emit_template.c/h`) |
| `sm_emit_call_chunk` | `sm_emit_call_expression` | function (`sm_emit_template.c/h`) |
| `emit_sm_push_chunk` | `emit_sm_push_expression` | static function (`sm_codegen_x64_emit.c`) |
| `emit_sm_call_chunk` | `emit_sm_call_expression` | static function (`sm_codegen_x64_emit.c`) |
| `PUSH_CHUNK` / `CALL_CHUNK` macros | `PUSH_EXPRESSION` / `CALL_EXPRESSION` macros | `sm_macros.s` (corpus artifact) |

**Files touched (SCRIP):** 32 files across `src/` + `scripts/`.  
**Files touched (corpus):** `programs/snobol4/demo/sm_macros.s`.

### Zero-chunk invariant gate

```
grep -rniE "\bchunk\b" src/ --include="*.c" --include="*.h" \
  | grep -v "CHUNKS-step\|GOAL-CHUNKS\|CHUNKS step"
```
Returns zero hits. PASS.

### Gates (CH-17-RENAME-g)

All byte-identical to baseline:
- build: PASS (clean, no errors)
- smoke ×5: PASS (7/7 SNO, 5/5 ICN, 5/5 RK, 5/5 PL, 5/5 SC)
- unified_broker: PASS=49
- isolation: PASS
- zero-chunk invariant: PASS

