# GPROLOG-STUDY-2026-05-28-OPUS.md — GNU Prolog engine study (WAM-CP prerequisite)

**Author:** Opus 4.7, 2026-05-28
**Companion to:** `SWIPL-STUDY-2026-05-28-OPUS.md`
**Purpose:** The WAM-CP goal-file directive — *"analyze GNU Prolog (register-based WAM,
closer to what we emit) before locking the CP record layout; then WAM-CP-1."* This is
that analysis. Source read: `gprolog-master/src/EnginePl/{wam_archi.def,wam_inst.h,
wam_inst.c}` + `src/Pl2Wam/indexing.pl`.

---

## Why GNU Prolog is the right model for us (vs SWIPL)

SWIPL gave us the *concept* (parent-linked CP stack + one BFR register, cut = truncate).
But SWIPL is a **bytecode emulator** — its CP records are heap structs walked by a C
dispatch loop. GNU Prolog **compiles to native code** (Pl2Wam → Wam2Ma → Ma2Asm → .s),
which is exactly SCRIP mode-4. So gprolog answers the questions SWIPL couldn't:

- *What does a CP record look like when there is no interpreter to walk it?* → A fixed
  set of WamWords at **negative offsets from a single register `B`**, written by inline
  code, never "walked."
- *How does compiled native code re-enter an alternative?* → The CP stores a raw **code
  pointer** `ALTB`; backtracking is literally `goto *ALTB(B)`. No dispatch table.
- *How does cut work with zero bookkeeping?* → `B = saved_B`. One register assignment.

This is the model SCRIP should emit toward. Our `g_pl_bfr` ≙ gprolog's `B`.

---

## 1. The register bank (`wam_archi.def`)

GNU Prolog's entire control state is **named global registers**, not a struct:

```
CP    WamCont   continuation pointer (where to go on success-return)
E     WamWordP  last environment pointer (local stack frame)
B     WamWordP  last choice point pointer          ← THIS is our g_pl_bfr
H     WamWordP  top of heap (global stack)
HB1   WamWordP  copy of HB(B) — heap backtrack point, hoisted to a reg for speed
TR    WamWordP  top of trail
S     WamWordP  unification (structure) pointer
```

**Takeaway for us:** we already have `g_pl_trail` (≙ TR) and `g_pl_env` (≙ E). WAM-CP-1
adds `g_pl_bfr` (≙ B). We do NOT need H/HB1 yet — those belong to the LATER tagged-word /
global-stack track. The CP-stack rungs can proceed with just **B + the existing trail**.

---

## 2. The choice-point frame — exact layout (`wam_inst.h:90-104`)

A CP is a run of WamWords; **`B` points at its high end**, fields at negative indices:

```
offset   macro      meaning
 b[-1]    ALTB       code pointer to the NEXT alternative (the "retry" target)
 b[-2]    CPB        saved CP   (continuation on success)
 b[-3]    BCIB       saved byte-code info (debug/FD; we can omit)
 b[-4]    EB         saved E    (environment to restore)
 b[-5]    BB         saved B    (PARENT choice point)  ← the parent link
 b[-6]    HB         saved H    (heap backtrack point)
 b[-7]    TRB        saved TR   (trail backtrack point) ← we restore trail to here
 b[-8]    CSB        saved CS   (constraint stack; FD only, omit)
 b[-9-i]  AB(b,i)    saved argument register A(i), i=0..arity-1
CHOICE_STATIC_SIZE = 8 (the 8 fixed fields above; args follow)
```

**This is the literal answer to "what is the CP record layout."** Mapped to SCRIP, where
we keep `Term*` boxes (no heap/H yet, no FD/CS), the live subset is:

| gprolog field | SCRIP WAM-CP-1 equivalent | notes |
|---|---|---|
| ALTB | `void *resume` | code ptr in mode-4; cursor/clause-index in mode-2 |
| CPB  | (implicit in γ continuation) | mode-2: not needed yet; mode-4: WAM-CP-5 |
| EB   | `Term **env` | restore callee env on retry |
| BB   | `struct pl_choice *parent` | **the parent link SWIPL study flagged missing** |
| HB   | — | deferred to tagged-word track |
| TRB  | `int trail_mark` | restore trail via existing `trail_unwind(&g_pl_trail, mark)` |
| AB(i)| `Term **saved_args` (later) | arg restore; WAM-CP-2 can start without it |

So the WAM-CP-1 struct in the goal file is **correct and minimal** — it is gprolog's
frame with the not-yet-needed fields (H, CS, BCI, CPB) dropped:

```c
struct pl_choice {
    int               type;        /* clause-choice | disjunction | builtin-retry  */
    struct pl_choice *parent;      /* = BB : parent CP, the backtrack chain         */
    int               trail_mark;  /* = TRB: trail top to unwind to on retry        */
    Term            **env;         /* = EB : environment to reinstate               */
    void             *resume;      /* = ALTB: next-alternative target (cursor/ptr)  */
    int               cursor;      /* clause index for multi-clause BB_CHOICE       */
};
```

---

## 3. The three operations — and what each teaches us

### Create (`CREATE_CHOICE_COMMON_PART`, wam_inst.c:1396)
```
old_B = B;                       save parent
cur_B = Local_Top + 8 + arity;   bump allocate above the higher of B/E
B = cur_B;                       B now points at the new frame
ALTB=codep_alt; CPB=CP; EB=E; BB=old_B; HB=HB1=H; TRB=TR; CSB=CS;
copy A(0..arity-1) into AB
```
**Lesson:** creation is pure field stores + one register bump. The "parent" is just the
*old* value of B. For SCRIP we `push`: new node's `parent = g_pl_bfr; g_pl_bfr = new`.

### Backtrack / Update (`UPDATE_DELETE_COMMON_PART`, wam_inst.c:1429)
```
cur_B = B;
Pl_Untrail(TRB(cur_B));   ← unwind trail to the saved mark  (we already have this!)
CP = CPB; BCI=BCIB; H = HB; E = EB; CS = CSB;   restore registers
```
then either **Update** (more alternatives left): `ALTB = next; HB1 = H` and reload A(i)
from AB — i.e. *keep the same frame, just point it at the next clause*; or **Delete**
(last alternative): `Assign_B(BB(cur_B))` — *pop the frame by following the parent link*.

**Lesson — this is the single most important structural insight for WAM-CP-2:** retry
does NOT pop-and-repush. It **mutates ALTB in place** (`retry_me_else`) while alternatives
remain, and only pops on the last one (`trust_me_else_fail`). Our current
`bb_active_choice` scan over `nd->state` is a crude emulation of exactly this. WAM-CP-2
should replace it with: on first entry push CP (cursor=0); on redo, `trail_unwind` to
`trail_mark`, `cursor++`, if more clauses → reuse the frame (Update), else pop (Delete).

### Cut (`Pl_Cut`, wam_inst.c:1307)
```c
Pl_Cut(WamWord b_word) { Assign_B(From_WamWord_To_B(b_word)); }
```
**That's the whole thing.** Cut = restore `B` to the value it had when the clause was
entered. Everything allocated since (all CPs created by goals to the left of `!`) is
instantly orphaned. This is the WAM-CP-4 payoff the goal file predicted: cut becomes one
pointer assignment, and `g_pl_cut_flag` disappears.

The prerequisite: **capture B at clause entry**. gprolog exposes this via
`Pl_Get_Current_Choice()` → `From_B_To_WamWord(B)` (wam_inst.c:1293), and the compiler
passes that token to the eventual `!`. For SCRIP: on clause/frame entry, record
`frame_cp_barrier = g_pl_bfr`; `!` does `g_pl_bfr = frame_cp_barrier`.

There is also **`Pl_Soft_Cut`** (wam_inst.c:1320) — unchains *one* CP from the middle of
the chain (for `*->`/soft-cut). Not needed now, but it proves the parent-link design
supports mid-chain removal cheaply. Worth remembering for if/`*->`.

### Untrail (`Pl_Untrail`, wam_inst.c:1730)
Tag-dispatched (TUV/TOV/TMV/TFC) because gprolog trails *bindings of tagged words*. Ours
is simpler — `g_pl_trail` is an array of `Term*` whose `.ref` we reset — so our existing
`trail_unwind(&g_pl_trail, mark)` already IS Pl_Untrail for our representation. **We reuse
it verbatim; no new trail machinery for WAM-CP.**

---

## 4. First-argument indexing (`Pl2Wam/indexing.pl`) — the WAM-CP-8 blueprint

gprolog documents the exact scheme in the file header (lines 50-102). For a predicate
with clauses C1..Cn, the compiler emits a **two-layer** structure:

```
try_me_else/retry_me_else/trust_me_else_fail      ← the linear clause chain (fallback)
switch_on_term(LabVar,LabAtm,LabInt,LabLst,LabStc) ← dispatch on 1st-arg TAG
  LabAtm: switch_on_atom([...])    hash atom        → clause(s) | try/retry/trust group
  LabInt: switch_on_integer([...]) hash int
  LabLst: try/retry/trust          list cells
  LabStc: switch_on_structure([(f/n, Lab)...])
  LabVar: full try/retry/trust chain (a var matches every clause)
```

**Key ideas to steal for WAM-CP-8:**
1. **Tag-first dispatch.** Look at the principal tag of A(0): var → must try all; bound →
   jump to the bucket. A *bound* first arg with a unique clause → **no choice point at
   all** (the determinism win — assert `g_pl_bfr` unchanged across the call).
2. **`switch_on_X` hash tables** map a concrete first-arg value to a single label or a
   small try/retry group, skipping non-matching clauses entirely.
3. **The linear `try_me_else` chain is still emitted** as the var-case fallback. Indexing
   is an *accelerator layered on top*, not a replacement. This means WAM-CP-2 (build the
   linear chain via CP stack) is a strict prerequisite for WAM-CP-8, and they compose
   cleanly — exactly the goal-file dependency order.

For SCRIP this lands as: a first-arg classifier in `lower_pl.c` that partitions a
predicate's clauses by principal functor/atom/int of arg 0, emitting a jump table; the CP
is only pushed when the selected bucket has >1 clause.

---

## 5. Concrete recommendations — locking the WAM-CP-1 record

1. **Adopt the goal-file struct as-is** — it is gprolog's frame minus the deferred fields.
   Confirmed correct. Add one field the goal file omitted but gprolog proves we want:
   `Term **saved_args` (≙ AB) — needed by WAM-CP-2 to restore arg registers on retry.
   For WAM-CP-1 (substrate only) it can be `NULL`; wire it in WAM-CP-2.
2. **`resume` is polymorphic by mode.** Mode-2: it holds the clause `cursor`/a `tree_t*`
   body pointer (interpreter re-enters by index). Mode-4: it holds a raw code label
   (`.Lplpred_<name>_<arity>_<clause>` retry target) — gprolog's ALTB. WAM-CP-5 promotes
   the stashed CAT-A-3 r12 buffer to write exactly this. Keep `resume` as `void*` so both
   modes share the record; do not split the struct.
3. **`g_pl_bfr` is a single global `struct pl_choice *`**, initialized NULL. Push =
   `cp->parent = g_pl_bfr; g_pl_bfr = cp`. Truncate-to-barrier (cut) =
   `g_pl_bfr = barrier`. Pop = `g_pl_bfr = g_pl_bfr->parent`. Three one-liners, matching
   gprolog's Create/Cut/Delete.
4. **Reuse `trail_mark`/`trail_unwind`.** Do not invent a parallel trail. gprolog's
   Untrail and ours are the same operation at different representation granularity.
5. **Defer H/HB/CS/BCI/CPB.** They belong to the tagged-word + global-stack LATER track.
   Including them now would couple WAM-CP to a migration the goal file explicitly
   separates. The record is designed to *grow* these fields later (gprolog proves the
   superset is coherent), so adding them is non-breaking.

**Net:** the SWIPL study set the direction; gprolog confirms the exact field layout, the
in-place-update retry discipline (the real fix for our `nd->state` scan), the
one-assignment cut, and the indexing scheme. WAM-CP-1's struct is validated. Proceed.

---

## 6. One divergence to keep in mind

gprolog allocates CPs on the **local stack by bumping a pointer** (`Local_Top + 8 +
arity`), interleaved with environments — both control structures share one stack, ordered
by address, which is what makes `Local_Top = max(B,E)` and the cut/LCO age comparisons
(`B older than E?`) into pointer comparisons. SCRIP's WAM-CP-1 starts with **heap-malloc'd
`pl_choice` records linked by `parent`** (simpler, no stack discipline yet). That is fine
for correctness and for rungs 1-5, but **WAM-CP-6 (LCO) wants the address-ordered
property** ("is `g_pl_bfr` older than the current frame?"). When we reach LCO, either (a)
move CP records onto a contiguous arena so `<`/`>` comparisons are meaningful, or (b)
carry a monotonic `stamp`/`age` counter on each record and compare those. Recommend (b)
first (a 4-byte field, no allocator change), matching gprolog's `STAMP` register
(`wam_archi.def`), which it already bumps in Create/Delete for exactly this kind of age
test. Add `int stamp;` to the record at WAM-CP-1 time so LCO needs no later struct change.
