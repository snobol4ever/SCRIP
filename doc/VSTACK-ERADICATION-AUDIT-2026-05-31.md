# VSTACK-ERADICATION AUDIT (Opus 4.8, 2026-05-31)

**Directive (Lon, 2026-05-31):** SCRIP has no value stack. SNOBOL4, Icon, Prolog all use Byrd Boxes for
everything. `g_vstack` and its entire apparatus must not exist — DELETE it. TOP PRIORITY.

This is the non-destructive audit (rung VSX-0): inventory every piece, classify dead vs live, define the
deletion order. No code deleted here.

## The apparatus (all in `src/runtime/rt/rt.c` unless noted)

1. **The array + globals** — `static DESCR_t g_vstack[VSTACK_CAP]` (VSTACK_CAP=65536), `g_vtop`,
   `g_vframe_base`, `g_last_ok`.
2. **The ops indirection** — `rt_vstack_ops_t` (struct, declared in `rt.h:125`), `g_default_ops`,
   `const rt_vstack_ops_t *g_ops`, and the 7 `_default_*` fns (`_default_push/_pop/_peek` already ABORT;
   `_default_depth/_set_depth/_get_last_ok/_set_last_ok`).
3. **Public ops accessors** — `rt_vstack_depth()`, `rt_vstack_pop()`.
4. **Static wrappers** — `vstack_push/vstack_pop/vstack_peek/vstack_pop_str/vstack_pop_int64`, macros
   `LAST_OK_GET/LAST_OK_SET`.
5. **`STACKLESS_ABORT(fn)` macro** (rt.c:15) — its message names `g_vstack`; keep the macro concept but
   it should no longer reference a stack.
6. **~67 `rt_*` functions** whose bodies call a vstack op — the legacy SM / SNOBOL4-pattern / frame
   surface. These are the bulk.

## KEY FINDING — the stack is ALREADY a bomb

`g_ops` is assigned `&g_default_ops` at init and **never reassigned anywhere** (`grep 'g_ops =' src/` →
one hit). The default `push`/`pop`/`peek` ops `abort()` with `[SMX] FATAL: ... There is no value stack.`
So every `vstack_push`/`vstack_pop`/`vstack_peek` site is **already a guaranteed abort** — any `rt_*`
function that reaches one is non-functional in the live BB world. Eradication is therefore removal of
provably-dead-or-aborting code, not a behavioural rewrite of a working stack. The only non-aborting ops
are `depth`/`set_depth`/`get_last_ok`/`set_last_ok` (used by `rt_frame` save/restore + `rt_last_ok`).

## DEAD vs LIVE split (external references — outside rt.c/rt.h)

DEAD (0 external refs — delete in VSX-2, safe): `rt_coerce_num`, `rt_concat`, `rt_decr`,
`rt_exec_stmt_pat`, `rt_exp`, `rt_frame_enter`, `rt_halt_tos`, `rt_incr`, `rt_load_frame`,
`rt_match_blob`, `rt_neg`, `rt_nv_set`, `rt_push_expr`, `rt_push_expression_descr`, `rt_do_return`,
`rt_do_nreturn`, `rt_store_frame`, `rt_set_last_ok`, `rt_set_stno`, `rt_push_null_noflip`,
`rt_push_real_bits`, and essentially all `rt_pat_*` (the SNOBOL4 pattern-primitive runtime:
`rt_pat_lit/ref/span/any/notany/break/breakx/len/pos/rpos/tab/rtab/arb/arbno/bal/fence/fence1/eps/
fail/succeed/abort/cat/alt/deref/rem/refname/capture*/usercall*`).

LIVE (>0 external refs — VSX-3..VSX-6, each needs its callers traced & migrated/deleted first):
- `rt_push_int` — 10 files. The widest. Many callers are themselves dead `rt_*` or other languages'
  not-yet-stackless boxes; trace each.
- `rt_push_str` — 4 files.
- `rt_acomp`, `rt_lcomp`, `rt_arith`, `rt_nv_get`, `rt_match_variant` — 3 files each (SNOBOL4/Icon
  arith + compare + name-table + pattern-match entry; these are SNOBOL4/Icon SM-era boxes).
- `rt_call` — 1. `rt_push_null` — 1. `rt_limit_inc`/`rt_limit_more` — 1 each (Icon limit). `rt_pop_void`
  — 2.

## Cross-language note

The vstack-using `rt_*` are SNOBOL4 + Icon + SM-era. Prolog ALREADY has zero ties (VSX prerequisite done
this session: `rt_pl_atom_push` + `rt_pl_var_push` deleted, `bb_atom`/`bb_logicvar` stackless, guard
`scripts/test_gate_pl_no_value_stack.sh`). So VSX must verify, per LIVE function, that its language's BB
path no longer needs it — grounded in that language's GOAL (`GOAL-SNOBOL4-BB.md` for `rt_pat_*`/`rt_acomp`
/`rt_lcomp`/`rt_concat`; `GOAL-ICON-BB.md` for `rt_limit_*`/Icon arith). The DEAD set is language-agnostic
dead code.

## Deletion order (see GOAL VSX ladder)

VSX-1 tripwire-narrow → VSX-2 dead-set delete → VSX-3 rt_pat_* delete → VSX-4 SNOBOL4 live (arith/compare/
concat/nv) → VSX-5 Icon live (limit/arith) → VSX-6 push_int/push_str last callers → VSX-7 delete the array
+ ops layer + wrappers → VSX-8 zero-check gate. Each rung: build green, all language smokes at/above floor,
FACT 0, and the standing per-language no-vstack gate extended to cover the newly-removed surface.
