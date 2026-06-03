# BB-TEMPLATES-LANG-AUDIT — language-specific code in `src/emitter/BB_templates/`

**Requested by Lon 2026-06-03 (Pascal-BB session 11): "Ensure there is no language specific code in any BB
C++ template code. Make a list of the violators so I can have those code snippets replaced with ABORTS."**

Scan scope: every `.cpp`/`.h` in `src/emitter/BB_templates/` (XA/SM templates not scanned — say the word).
Patterns: raku/rk, icn/icon, prolog/pl, pas/pascal, sno/snobol, WAM/DCG, IR_LANG/LANG_*, is_icon/is_prolog,
plus emitted-string tags (`# BOX SNO`, `RK-`, `ICN-`, `WAM-CP`).

**Headline finding:** zero `IR_LANG_*` / `LANG_*` / `is_<lang>` guards exist inside any template — dispatch
is by IR shape + chain-mode flags (`g_gvar_flat_chain` / `g_descr_flat_chain`, which are representation
names, not language names; borderline only because the docs gloss gvar as "the SNOBOL4 chain"). The leakage
is in (T1) language-NAMED functions/arms/files, (T2) language-tagged emitted strings, (T3) language-naming
C comments (which also violate the comment style rule).

## TIER 1 — language-named code: dispatch arms, functions, forked files (the ABORT candidates)

| # | Site | Snippet | Language |
|---|------|---------|----------|
| 1 | `bb_call.cpp:209` | `if (g_descr_flat_chain && pBB->dval == 2.0) return bb_call_rk_arr_str(pBB);` | Raku-named arm in the shared IR_CALL dispatch |
| 2 | `bb_call_rk.cpp` (whole file; fn at :18) | `std::string bb_call_rk_arr_str(IR_t * pBB)` | Raku-named template |
| 3 | `bb_call.cpp:210` | `... && !strcmp(fn, "DEFINE")) return bb_call_gvar_define_str(pBB);` | SNOBOL4 builtin-NAME knowledge (`DEFINE`) hardcoded in the dispatch |
| 4 | `bb_gvar_assign_icn.cpp` (whole file; :13,:34,:36,:37) | `bb_gvar_assign_icn_str` / `bb_gvar_assign_icn` | Icon-forked variant of `bb_gvar_assign` |
| 5 | `bb_templates.h:44` | `void bb_gvar_assign_icn (IR_t * pBB);` | decl of #4 |
| 6 | `bb_builtin.cpp:3` | `int bb_pl_op_floaty(const char *fn)` | Prolog-named helper |
| 7 | `bb_builtin_common.h:61` | `int bb_pl_op_floaty(const char *fn);` | decl of #6 |
| 8 | `bb_builtin_is_cmp.cpp:77` | `is_floaty = bb_pl_op_floaty(rop)` | call site of #6 |

## TIER 2 — language-tagged emitted strings (asm comments, label prefixes, bomb messages): rename, not ABORT

| Site | String |
|------|--------|
| `bb_call.cpp:80` | label prefix `.Lrkarg%d_%d` (Raku-named, used by ALL languages' marshalled args) |
| `bb_call.cpp:129` | `# BOX SNO IR_CALL DEFINE(spec) ...` |
| `bb_call.cpp:163` | `# BOX SNO IR_CALL %s(...) ...` |
| `bb_binop_gvar_arith.cpp:25` | `# BOX SNO IR_BINOP gvar-arith ...` |
| `bb_gvar_assign.cpp:35,44,55,78` | `# BOX SNO IR_ASSIGN(...)` ×4 |
| `bb_gvar_assign.cpp:64` | bomb text `... (SNOBOL m4 pending LOWER four-port wiring)` |
| `bb_scan_stmt.cpp:38` | `# BOX SNO IR_SCAN literal-pattern ...` |
| `bb_to.cpp:21` | `# BOX IR_TO [ICN-HY-4 ...]` |
| `bb_var.cpp:19` | `# BOX IR_VAR ... [GZ-7 ICN flat-chain ...]` |
| `bb_var.cpp:27` | bomb text `bb_var: unhandled arm (not sno/icn flat-chain or missing slot)` |
| `bb_call_rk.cpp:37` | `# BOX IR_CALL %s(...) [RK-EMIT-2 ...]` |
| `bb_catch.cpp:12` | `# BOX RESOLVE_CATCH (mode-4 STUB — WAM-CP-13 ...)` (WAM = Prolog rung tag) |
| `bb_choice.cpp:62` | `# BOX RESOLVE_CHOICE n=%d (WAM-CP-5 ..., WAM-CP-9 cut)` |
| `bb_goal.cpp:66` | `# BOX RESOLVE_CALL %s/%d (WAM-CP-5, ...)` |

## TIER 3 — C comments naming a language (also violate the ONE-COMMENT style rule)

| Site | Comment |
|------|---------|
| `bb_choice.cpp:1` (file-top block) | "BB template for IR_CHOICE: **Prolog** multi-clause predicate. WAM-CP-5/9 history prose" (+ inline at :59) |
| `bb_goal.cpp:1` (file-top block) | "BB template for IR_GOAL: **Prolog** predicate call (WAM-CP-5). ..." |
| `bb_builtin_term_inspect.cpp:106` | `/* Name/Arity (a1/a2) are by Prolog semantics never compound literals ... */` |

## Borderline (flagged, not counted)

- `g_gvar_flat_chain` / `g_descr_flat_chain` (used in `bb_alt`, `bb_binop_*`, `bb_call*`, `bb_var`):
  chain-REPRESENTATION flags with neutral names; the gvar chain is de-facto SNOBOL4+Pascal and the descr
  chain de-facto Raku, but the templates dispatch on the representation, not the language.
- Per-language builtin/feature template FILES (`bb_builtin_findall`, `bb_builtin_retract_throw`,
  `bb_unify`, `bb_cut`, `bb_every`, `bb_to`, `bb_pat_*`, ...): each implements an IR opcode
  (box-per-opcode is the architecture); only their internal strings/comments above leak language names.

## Consequence applied this session (PB-9a)

`PB-9-DESIGN.md` Step 2 prescribed a `!strncmp(fn, "__pas_", 6)` arm in `bb_call_str` — that would have
landed a brand-new Tier-1 violator. PB-9a was instead implemented language-neutrally: the arm dispatches on
IR shape only (`g_gvar_flat_chain && dval==3.0 && !rt_proc_is_registered(fn)`) and routes to the generic
by-name runtime trampoline `rt_call_arr` with the standard four-port FAIL→ω / else γ contract. Name
knowledge (`__pas_*`) stays in `src/runtime/by_name_dispatch.c` where it already lives. Precision proof:
`v_det_call` (the only dval=3.0 setter, `lower.c:596`) is reached only by `IR_LANG_ICN` and `IR_LANG_PAS`
(`lower.c:955-956`), and Icon never rides the gvar chain — so today the arm fires exactly for Pascal
det-calls to non-proc names, and any future frontend lowering onto the same shape gets the same generic
contract.

## XA_TEMPLATES appendix (scanned 2026-06-03, same session)

`src/emitter/XA_templates/` (17 files): **ZERO language-specific markers.** The only grep hits are
`prologue` substring false-positives on the `prolog` pattern. The LANGUAGE-BLIND FACT RULE's completion
grep therefore starts at 0 for XA; only `BB_templates/` carries the Tier-1/2/3 inventory above.
