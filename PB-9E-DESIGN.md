# PB-9E DESIGN — Nested-proc frames onto compiled BBs: static link on the parent-port thread

**Status: DESIGNED, build held (the representation FORK is Lon's call; two fork points below).**
Written after tracing the live mode-3/4 call mechanism end-to-end and pinning two failing gates.
SCRIP HEAD at design time = `780de9f` (LB-3 landed). Mode-2 ladder green: Pascal 36/0/1 → 37/0/1 with the
new `nestshadow.pas` probe (m2 PASS pinned this session).

---

## The pinned gates (both byte-identical targets vs `pint`)

| Probe | Oracle | m2 today | m3 today | What it proves |
|-------|--------|----------|----------|----------------|
| `nestrec.pas` | `11 21 31` | PASS | `11 11 11` | Recursion clobber: all activations of `outer` share one NV cell for local `x`. |
| `nestshadow.pas` (NEW) | `7 101` | PASS | `107 107` | Lexical-vs-dynamic: sibling `p2` must see OUTER's `x` through the shadowing sibling `p1`. |

`nestshadow` is the structural kill-shot for the shallow-binding shortcut: even if the current
param-style NV save/restore were deepened to cover locals, save/restore IS dynamic-chain semantics —
`p2` would still read `p1`'s seated `x`. "More NV flattening" cannot pass it, exactly as the goal
watermark warned. The static-link model is forced, not preferred.

## The live mechanism (verified by reading it, `rt/rt.c:515`)

`rt_call_named_proc(name, args, nargs)`: looks up the registered proc; `rt_name_save_push` saves the
OLD NV values of the param names and seats args into NV (shallow binding — why flat recursion works);
hands the compiled body a **per-activation frame block** `fb = &g_proc_frame_nest_arena[depth*QW]`;
invokes `p->fn(fb, 0)` (the compiled BB graph entered on its one-register frame); result =
`NV_GET(name)` (funcname-as-return-var); restores NV. **Pascal locals are not in `fb` and not saved —
they are bare NV globals** (the lowerer routes Pascal `IR_VAR`/`IR_ASSIGN` through gvar/NV by name).
That is the whole gap: the per-activation block already exists; nothing per-activation lives in it
except box scratch.

## The m2 semantic mirror (`IR_interp.c:1815-2070`)

`GenFrame { env[], slotref[] (var-param SlotRef), sc, static_link, level }`. At call time:
`callee_dl = proc_table[upi].decl_level; _f->static_link = pas_base(caller, caller_lvl - callee_dl);
_f->level = callee_dl + 1` — the classic display-less static-link convention (handles
sibling-calls-sibling, recursion, child-calls-parent). Uplevel reads walk `static_link` and resolve
by name per frame. Var params are `SlotRef{frame,slot}` indirections resolved by `pas_loc_of_name`.

**The key emit-time fact:** `caller_lvl` and `callee_dl` are LEXICAL constants, so in mode-3/4 every
static-link computation at a call site and every uplevel access at a use site compiles to an
**emit-time-constant number of hops**. No runtime level bookkeeping, no name search, no display.

## The design — frame-as-BB, static link rides the parent-port thread

**Frame layout** (inside the `fb` block the runtime already allocates per activation):
- `[fb+0]` — the static link: the lexical parent activation's `fb`. This slot IS the parent-port
  thread made addressable; uplevel access is port-chasing.
- `[fb+16 + k*16]` — param/local DESCR slots, ordered by the proc's `lower_sc` Scope (params first,
  then locals — the table `proc_table[pi].lower_sc` already carries exactly this from registration).

**Call-site SL computation** (emit-time constant `h = caller_lvl − callee_dl` hops):
`mov rax, <frame reg>` ; `h ×` `mov rax, [rax+0]` ; pass `rax` as the callee's SL. Recursion: `h=0`
when a proc calls itself (SL = own SL... no — self-call has `caller_lvl − callee_dl = 1` hop? No:
self-call: caller_lvl = callee_dl+1, h = 1, SL = caller's OWN static link — correct, the recursive
activation shares the same lexical parent). Child call: h = 0, SL = caller's fb. Sibling: h = 1.

**Runtime protocol** — `rt_call_named_proc` grows an SL parameter (fork point B below); it writes SL
to `[fb+0]`, seats args into `[fb+16..]` (NOT NV), zeroes local slots, invokes `p->fn(fb,0)`.
Funcname-as-return stays NV (`rt_name_save_push(&name,…)` already recursion-protects it) — smallest
blast radius; migrating the return var into the frame is a later cleanup rung, not PB-9e.

**IR shapes** (LANGUAGE-BLIND per the FACT RULE — dispatch on kind+fields only, no names):
- `IR_VAR_FRAME`  — read:  `ival` = slot offset, `dval` = hops. Template: `h×` chained `[base+0]`
  loads, then 16-byte DESCR load at `[base + 16 + slot*16]` → own value slot.
- `IR_ASSIGN_FRAME` — write twin, same fields, value from the standard operand slot.
- LOWER resolves every Pascal name at lowering time against the lexical scope chain it already
  maintains for `decl_level`/`lower_sc`; in-scope names lower to the frame shapes; the remainder
  (globals, funcname-return) keep the existing gvar/NV shapes. Name knowledge stays in LOWER —
  the same boundary LB-3 just enforced for DEFINE.
- Var params: the param slot holds the ADDRESS of the actual cell (`ancestor_fb + off`, computed at
  the call site via the same hop chain); a deref flag or paired deref shapes handle read/write
  through it. This is m2's SlotRef flattened to a pointer. Lands at PB-9e-2, gated by the existing
  varparam/swap/alias/varframe/varmix/vartrans probes (all flat today — their NESTED variants are
  the new gates).

## Fork points — Lon's call

**A. Migration scope.** (1) NESTED-ONLY: procs at decl_level > top migrate params+locals to frame
slots; flat procs keep today's NV mechanism (recursion.pas/flatnoarg gates untouched by
construction). (2) UNIFORM: every registered proc migrates (one mechanism, bigger first diff, the
flat gates must be re-proven byte-identical). Recommendation: NESTED-ONLY for PB-9e-1, UNIFORM as a
follow-up dename/unification rung once green — mirrors the LB-3 pattern of moving the gate last.

**B. Runtime signature.** (1) `rt_call_named_proc_sl(name, args, nargs, void *sl)` beside the
existing entry (old one keeps flat traffic in fork A1). (2) In-band: `args[np]` carries SL.
Recommendation: (1) — explicit, greppable, no array-shape pun.

## Rung ladder

- [x] **PB-9e-0** — discriminator probe `nestshadow.pas` in corpus; oracle `7/101`; m2 PASS pinned;
  m3 `107/107` expected-fail pinned. (THIS SESSION.)
- [ ] **PB-9e-1** — SL plumbing + nested-proc locals/params into frame slots (fork A choice), the
  two IR shapes + templates, call-site hop chains. GATE: `nestrec` + `nestshadow` byte-identical
  m3+m4; flat gates (`recursion` fact-7, `flatnoarg`, `sieve`, `hello`) byte-identical; SNOBOL4
  smoke 19/0 untouched; full cross-language stash-proof.
- [ ] **PB-9e-2** — var params across levels (SlotRef→pointer). GATE: nested variants of
  varparam/swap/varframe/varmix/vartrans (write the probes; pcom-accept first).
- [ ] **PB-9e-3** — nested FUNCTIONS under recursion (`nestfunc`/`nestcount` byte-identical m3+m4;
  funcname NV save/restore already covers the return cell — expected near-free).

## Regression evidence (this session, at `780de9f`)

SNOBOL4 19/0 (m2 7/0 HARD, m3 6/6, m4 6/6); Pascal m2 36/0/1 + nestshadow → 37/0/1; Pascal m3/m4
probes hello/sieve/flatnoarg/recursion-fact7 byte-identical; Icon m2 12/12 HARD (m3/m4 5/12
pre-existing); Prolog m2 5/5 + m4 5/0 (m3 unify pre-existing); Snocone 2/3 pre-existing
(stash-proven); all-langs m4 hello 5/1 (rebus, watermark pin); Tier-1 template grep = 0.
