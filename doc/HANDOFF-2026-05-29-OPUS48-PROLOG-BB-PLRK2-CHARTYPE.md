# HANDOFF — PLR-K-2: char_type/2 mode-3 native + mode-4 emit

**Date:** 2026-05-29
**Model:** Opus 4.8
**Goal:** GOAL-PROLOG-BB.md
**Files touched:** `src/lower/bb_exec.c` (+ effect helper), `src/lower/bb_exec.h` (decl),
`src/emitter/BB_templates/bb_builtin.cpp` (BINARY + TEXT arms + decl).
**FACT:** clean — arm1 = 0, arm2 = 12 (baseline). All new byte production inside `BB_templates/`.

## What landed

`char_type/2` had a mode-2 oracle arm in `bb_exec.c` but **no emitter arm at all** in
`bb_builtin.cpp` — so it fell through to the double-jump stub in BOTH mode-3 (MEDIUM_BINARY) and
mode-4 (MEDIUM_TEXT). Boolean tests wrongly succeeded (`char_type('3',alpha)` → `yes`) and
extractor forms printed `_` (output var never bound).

- **New effect helper `rt_pl_char_type`** in `bb_exec.c` (after `rt_pl_downcase_atom`), a faithful
  transliteration of the mode-2 oracle: `(k0,i0,s0)` = char arg, `ty` = type name, `is_compound`
  flag, `(ki,ii,si)` = inner var for extractor forms. Boolean tests (alpha/alnum/digit/space/
  white/upper/lower/punct/graph/csym/csymf/end_of_line/newline) return 1/0; extractor forms
  (digit(V)/to_lower(L)/to_upper(U)/upper(L)/lower(U)/code(C)) derive the value, `unify` it into
  the inner var under a trail mark. Decl added to `bb_exec.h`.
- **MEDIUM_BINARY arm** (mode-3): raw bytes, `rt_pl_char_type` called by absolute `movabs`. 8 args
  → 6 in regs (rdi=k0 rsi=i0 rdx=s0 rcx=ty r8=is_compound r9=ki) + `[rsp+0]=ii`, `[rsp+8]=si`
  (`sub rsp,16`). String ptrs (char sval, type name, inner sval) loaded absolute (in-process).
  Standard `test eax/je ω/jmp γ/β→ω` bin-patch tail.
- **MEDIUM_TEXT arm** (mode-4): byte-twin via `s_2asm` + `@PLT` + `lea [rip+strtab_label]` for the
  string ptrs (type name + char sval + inner sval — all already pre-interned in the strtab).

## Results

- **GATE-2 crosscheck: 43 → 47 PASS (+4)** — rung21 alpha/digit_val/to_upper_lower/upper_lower now
  3-mode AGREE (space_alnum was already passing).
- **Mode-3 native rung suite: 39 → 43 (+4).**
- **Mode-4 rung21: → 5/5** (was ~1/5; closes the CAT-D char_type/2 mode-4 emit gap).
- All 5 rung21 char_type 3-mode AGREE and byte-identical to `.expected`:
  alpha → `yes/no`; digit_val → `7`; space_alnum → `yes/yes/yes`; to_upper_lower → `A/z`;
  upper_lower → `a/B`.

## Gates (all green, zero regressions)

| Gate | Result |
|---|---|
| GATE-1 smoke | 5/5 |
| GATE-2 crosscheck | **47 PASS** (was 43), 1 ORACLE_MISS |
| GATE-3 mode-2 | 104/107 byte-identical (mode-2 oracle logic unchanged — refactored into shared helper, output identical) |
| GATE-3 mode-3 native | **43/107** (was 39) |
| GATE-4 mode-4 minimal | 4/4 |
| mode-4 rung21 corpus | 5/5 |
| FACT arm1 / arm2 | 0 / 12 |
| siblings icon/raku/snobol4 | 5/5 / 5/5 / 13/13 |

## NEXT

Continue the small TEXT-only / unported builtin arms:
- **numbervars/3** (rung20, 3 mode-3 fails) — compound-arg construction.
- **type-test BB_PL_STRUCT compound arg** (`rt_pl_type_test_term`) — the one PLR-J-1/J-3 left
  honest-abort-guarded (e.g. `is_list([1,2,3])` in mode-3).
- **writeq/write_canonical** (rung22) — quoting / operator-notation writer.
- **format/1,2 compound** (rung19), **retract/retractall** (rung14), **string_io** (rung24).
- **findall/3** (rung11) last — needs its own protocol (`nd->ival` is `bb_pl_findall_state_t*`,
  not an arity int).

Orthogonal pre-existing (not introduced, not fixed): mode-2 cut-in-disjunction loop; mode-3
native nested-`is` (`R is 3*10+4` → `6` not `34`).
