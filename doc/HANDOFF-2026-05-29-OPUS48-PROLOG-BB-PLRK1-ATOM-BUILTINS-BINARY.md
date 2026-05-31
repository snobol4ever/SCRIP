# HANDOFF — PLR-K-1: atom-builtin MEDIUM_BINARY arms (mode-3 native)

**Date:** 2026-05-29
**Model:** Opus 4.8
**Goal:** GOAL-PROLOG-BB.md
**File touched:** `src/emitter/BB_templates/bb_builtin.cpp` only (one template file).
**FACT:** clean — arm1 = 0, arm2 = 12 (baseline). All new byte production is inside
`BB_templates/`.

## What landed

The unported-builtin-BINARY-arm cluster the GOAL-PROLOG-BB NEXT pointer named. Ported the
atom/string builtin family from MEDIUM_TEXT-only to MEDIUM_BINARY, so they run natively under
`--run` (mode-3) instead of emitting their assembly strings AS raw bytes (the double-jump-stub
bug class that made every result variable print `_`).

Arms ported (all byte-twins of their existing MEDIUM_TEXT CAT-D arms, but raw bytes with absolute
`movabs` for in-process pointers — atom `sval` loaded directly like the `write` arm, not via
`lea [rip+strtab_label]`):

- **CAT-D-1/3** — `atom_length` / `upcase_atom` / `downcase_atom` (+ `string_length` /
  `string_upper` / `string_lower` aliases). 6-scalar SysV call (`edi/rsi/rdx, ecx/r8/r9`,
  no stack) → `rt_pl_atom_length` / `rt_pl_upcase_atom` / `rt_pl_downcase_atom`.
- **CAT-D-4/5** — `atom_string` / `string_to_atom` → `rt_pl_atom_string_pair`; `copy_term` →
  `rt_pl_copy_term`. Same 6-scalar shape, folded into the same arm.
- **CAT-D-2/3** — `atom_concat` / `string_concat` → `rt_pl_atom_concat`. 9 scalars: 6 in regs,
  the `(k2,i2,s2)` triplet on the stack (`sub rsp,32` / data at `[rsp+0/+8/+16]` / `add rsp,32`).
- **CAT-D-6** — `atom_chars` / `atom_codes` / `string_chars` / `string_codes`. Two paths mirror
  the TEXT arm:
  - Path A (scalar `a1`): 7 scalars, `s1` on the stack (`sub rsp,16`) →
    `rt_pl_atom_chars_codes`.
  - Path B (literal cons-cell `a1`, `BB_PL_STRUCT`): build the list `Term*` via
    `emit_build_compound_term_bin` into `r8`, then `rt_pl_atom_chars_codes_term`
    (5 args, all in regs). 8-byte scratch frame to keep `rsp` 16-aligned across the build's
    internal `call`.

All arms use the standard bin-patch tail: `test eax,eax` → `je ω` / `jmp γ` / `β: jmp ω`,
with the `bin` patch triple `{j+2, j+6+1, j+6+5+1}` → `{ω, γ, ω}`.

Added forward decls for the 8 referenced rt helpers to the `extern "C"` block at the top of
`bb_builtin.cpp` (matching the existing `rt_pl_is` / `rt_pl_type_test` pattern — the BINARY arm
references helpers by address, so they must be declared).

## Results

- **GATE-2 crosscheck: 33 → 43 PASS (+10).**
- **Mode-3 native rung suite: 29 → 39 PASS (+10).**
  Newly green: rung12 (5 — atom builtins, all 3-mode AGREE incl. path-B list literals), plus
  ripple gains in rung13 / rung16 / rung24 / rung26 (these reuse atom_concat / copy_term /
  string_to_atom / atom_chars).
- rung12 verified 3-mode AGREE and byte-identical to `.expected`:
  - `atom_length` → `5/0`; `atom_case` → `HELLO/world/5`; `atom_concat` → `foobar/hello world`;
  - `atom_chars` → `[h,i]` / `world` (path B `atom_chars(A,[w,o,r,l,d])`);
  - `atom_codes` → `[104,105]` / `hello` (path B `atom_codes(A,[104,...])`).

## Gates (all green, zero regressions)

| Gate | Result |
|---|---|
| GATE-1 smoke | 5/5 |
| GATE-2 crosscheck | **43 PASS** (was 33), 1 ORACLE_MISS |
| GATE-3 mode-2 (`--interp`) | 104/107 byte-identical (mode-2 untouched) |
| GATE-3 mode-3 native (`--run`) | **39/107** (was 29) |
| GATE-4 mode-4 minimal | 4/4 |
| GATE-SWI | 57/57 (100%) |
| FACT arm1 / arm2 | 0 / 12 |
| siblings icon/raku/snobol4 | 5/5 / 5/5 / 13/13 |
| mode-4 corpus (TEXT arm untouched) | atom_length mode-4 still `5/0` |

## NEXT

More unported builtin BINARY arms — the remaining mode-3 native rung-suite fails are still
mostly TEXT-only arms:
- **rung11 findall/3** (5) — `nd->ival` holds `bb_pl_findall_state_t*`, not an arity int; needs
  a dedicated BINARY path (emit goal sub-graph inline or its own protocol). Hardest of the set.
- **rung19 format/1,2** (5) — `format` compound-args path.
- **rung14 retract/retractall** (5).
- **rung20 numbervars/3** (3), **rung21 char_type/2** (4 — 1 left), **rung22 writeq/
  write_canonical** (4), **rung24 string_io** (3 left), **rung29 number_ops float** (4),
  **rung27 aggregate** (bagof/setof/aggregate_all).
Recommend the type-test `BB_PL_STRUCT` compound arg next (small — `rt_pl_type_test_term`, the one
PLR-J-1/J-3 left honest-abort-guarded), then char_type/numbervars (scalar, mechanical), saving
findall for its own session.

Orthogonal pre-existing bugs (not introduced, not fixed): mode-2 cut-in-disjunction loop;
mode-3 native nested-`is` (`R is 3*10+4` → `6` not `34`).
