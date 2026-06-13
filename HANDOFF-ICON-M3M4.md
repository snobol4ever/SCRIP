# HANDOFF — Icon modes 3 & 4 to parity with mode 2 (SCRIP / GOAL-ICON-BB)

**Date:** 2026-06-13
**Base commit at session start:** `1361587` (working tree had 3 uncommitted edits at handoff — see below)
**Authors of record:** LCherryholmes · Jeffrey Cooper M.D. · Claude Opus 4.8

---

## SCOREBOARD

Run `bash scripts/test_icon_rung_suite.sh` (all three modes) from repo root after
`bash scripts/build_scrip.sh && make libscrip_rt`.

| Mode | Session start | Session end | Δ | Notes |
|------|--------------:|------------:|---:|-------|
| m2 `--interp`   | 200 | **200** | 0  | HARD GATE — must never decrease |
| m3 `--run`      |  45 | **55**  | +10 | stackless native JIT |
| m4 `--compile`  |  51 | **55**  | +4  | emit .s → as → link libscrip_rt.so → exec |

XFAIL=36, EXCISED=98, TOTAL=283 (unchanged). Smoke: m2 12/12, m3/m4 10/12
(proc_zeroarg, proc_recursion still FAIL — userproc-call path).

---

## WHAT LANDED THIS SESSION (3 files, all gate-safe, still uncommitted)

### 1. THE BIG ONE — `jz`/`jnz` BINARY mis-encoding  ★ +6 m3
**File:** `src/emitter/BB_templates/x86_asm.h`, function `x86_jcc_op()`

`x86_jcc_op()` mapped jump mnemonics → opcodes but had **no case for `jz`/`jnz`**.
`jz` is the same opcode as `je` (0x84), but the table only checked `"je"`, so every
`jz` fell through to the default `return 0x85` — which is `jne`. Net effect: **in
BINARY/JIT mode (m3) every `jz` ran as `jne`, inverting the branch.** TEXT mode (m4)
emitted the string `" jz ω"` which `as` assembles correctly, which is exactly why m3
and m4 diverged on the *same shared template* (e.g. `bb_binop_relop` string path:
`"abc"=="abc"` came out false, `"abc"=="def"` came out true).

Fix added `jz`/`jnz` plus the `jnge`/`jnl`/`jng`/`jnle` synonyms:
```cpp
inline uint8_t x86_jcc_op(const char * mnem) {
    if (!strcmp(mnem, "je")  || !strcmp(mnem, "jz"))   return 0x84;
    if (!strcmp(mnem, "jne") || !strcmp(mnem, "jnz"))  return 0x85;
    if (!strcmp(mnem, "jl")  || !strcmp(mnem, "jnge")) return 0x8C;
    if (!strcmp(mnem, "jge") || !strcmp(mnem, "jnl"))  return 0x8D;
    if (!strcmp(mnem, "jle") || !strcmp(mnem, "jng"))  return 0x8E;
    if (!strcmp(mnem, "jg")  || !strcmp(mnem, "jnle")) return 0x8F;
    return 0x85;
}
```
**IMPORTANT build note:** x86_asm.h is a header compiled into `libscrip_rt.so` via the
templates. After editing it you MUST rebuild the .so or m4 won't pick it up:
`touch src/emitter/BB_templates/bb_binop_relop.cpp && make libscrip_rt`.

### 2. IR_IF flat-chain handler was a stub  ★ +4 m3 / +4 m4
**File:** `src/emitter/emit_bb.c`, `walk_bb_flat` dispatch, `case IR_IF:`

Old code defined β and jumped to γ **twice** then broke — it never walked the
condition/then/else subgraph at all. `lower_if` (in `lower_icon.c`) builds IR_IF with
ONE operand = `centry` (entry of the lowered condition), and the condition's own γ/ω
are already wired to the then-entry / else-entry. So the fix is just to walk that:
```c
case IR_IF: {
    IR_t *centry = bb_child0(nd);
    if (centry && (g_gvar_flat_chain || g_descr_flat_chain)) {
        walk_bb_flat(centry, lbl_γ, lbl_ω, lbl_β);
    } else {
        EMIT_PAIR_RESET();
        EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
        EMIT_PAIR_FILL(nd, lbl_γ, lbl_ω, lbl_β);
    }
    break;
}
```
NOTE: do NOT route IR_IF through `flat_drive_ite` — that helper reads a
`bb_ite_state_t` out of `IR_LIT(nd).ival` which only IR_ITE populates; IR_IF stores
its child via `ir_operand_push` instead. `resolve_ite_entries_em` hard-checks
`nd->op != IR_ITE` and would bail.

### 3. Duplicate β label in write-slot TEXT path
**File:** `src/emitter/BB_templates/bb_call_write_slot.cpp`, `bb_call_write_slot_str()`

TEXT branch emitted the β label twice:
`x86("label", std::string(_.lbl_β)) + x86("label", _.lbl_β)`. Removed the first.
(Assembler silently accepts the dup; symptom was a confusing double `xchain..._β:` in
the .s. Minor on its own but real.)

### REVERTED (do not re-apply blindly)
A `lower_if` change making the no-else case fail to γ instead of ω
(`src/lower/lower_icon.c`). It regressed m2 by 1 (`rung37_str_relop` lost its
`ac=='ca'` line) and was unnecessary once IR_IF was fixed. `lower_icon.c` is clean.
The real Icon-semantics question it was poking at (statement-level expression failure
being discarded vs propagated) is handled correctly enough for now via ω wiring — leave
it unless a specific test demands otherwise, and if so gate it on statement context.

---

## PENDING WORK — PRIORITY ORDER

### P1. rc=134 real-number relops  (rung17 / rung18 / rung19, ~8+ tests)
Symptom: `libscrip_rt: BOMB — bb_binop_relop: shape mismatch` on e.g.
`if 1.5 > 2.5 then ...`. Root cause: `descr_binop_opnd_slot()` (emit_bb.c:1318)
returns -1 for `IR_LIT_F`, so `op_sa/op_sb` go -1 and BOTH `brr_num_ok()` and
`brr_str_ok()` (bb_binop_relop.cpp) fail → `x86_bomb`. The numeric arm does integer
`cmp rax,rcx`; it has no real path. Need a real-comparison arm (SSE `comisd`, or call a
runtime `rt_jct_relop`-style helper that handles DT_R). Reals are stored as 16-byte
DESCR with `v=DT_R(=7)` and the double in the union; check how `bb_lit_scalar` / arith
templates already materialize a real slot so you reuse the same slot convention.

### P2. rc=124 nested-generator retry loop  (rung03 suspend*, rung14 limit*, rung19 real to-by, ~10 tests)
Infinite loop in the BINARY `to`/`every` resume wiring for nested generators:
`every write((1 to 3) * (1 to 2))` produces 1,2,1,2… forever instead of advancing the
outer generator on inner exhaustion. Distinct from the EVERY-exhaustion fix already in
the tree at the original watermark. Look at `flat_drive_to` and the β/resume chaining
when an inner generator's ω should bump the outer generator's β rather than re-seed.

### P3. userproc calls + locals  (proc_zeroarg / proc_recursion smoke; rung02_proc_*, many rung36/37)
Foundational. `bb_call_userproc.cpp` + frame/locals. Blocks a large fraction of the
remaining corpus. rung02_proc_locals exercises `local total; every total := total + (1 to n); return total`.

### P4. rc=134 misc missing BINARY arms
- rung08_strbuiltins_find / find_gen — `find` generator has no BINARY arm
- rung11_bang_* — list/string `!x` bang generator
- rung16_subscript_* — string/list subscripting
- rung21/25 global — global-var initial/access in BINARY
- rung22_lists_* — list push/put/pull/get/size

### P5. IR_CASE native template  (rung33, 4× rc=139 segfault)
No native BB template for IR_CASE. Build `bb_case.cpp` from JCON `ir_a_Case`
(refs/jcon-master/tran/irgen.icn:232). Port topology was fully read in the prior
session (see transcript).

---

## KEY ARCHITECTURE FACTS (carry-over, verified this session)

- **Ports:** α start, β resume, γ succeed, ω fail. Greek only.
- **DESCR_t** (src/contracts/descr.h): 16 bytes = `DTYPE_t v` (4) + `uint32_t slen` (4)
  + 8-byte union. Tags: DT_SNUL=0, DT_S=1, DT_I=6, DT_R=7, DT_FAIL=99. SysV ABI passes
  a 16-byte struct in a register pair: arg1=rdi/rsi, arg2=rdx/rcx. The relop template
  relies on this exact split.
- **Slot allocation** (`bb_slot_alloc16`/`bb_slot_get`, emit_bb.c:72) is deterministic
  and sequential by walk order, so m3 and m4 get identical offsets — which is why the
  jz divergence had to be an *encoding* bug, not a slot bug. Good triage lever: if m3≠m4
  on a shared template, suspect the BINARY encoder in x86_asm.h, not the template.
- **m3 path:** scrip.c `mode_run` → `bb_build_flat` → `codegen_flat_body` → `walk_bb_flat`
  (MEDIUM_BINARY, in-memory exec).
  **m4 path:** `codegen_flat_build` → same `codegen_flat_body` → `walk_bb_flat`
  (MEDIUM_TEXT → .s). They SHARE the dispatch and the templates; only the medium differs.
- **BINARY encoder** lives in `src/emitter/BB_templates/x86_asm.h` (the big
  `x86(mnem, a, b, ...)` dispatch starting ~line 530, plus the `x86_*` emit helpers).
  TEXT mode mostly returns assembler strings; BINARY mode returns raw bytes via
  `x86_Lrec` / `x86_Jrec`. Many helpers branch on `MEDIUM_BINARY`.

## USEFUL COMMANDS
```
bash scripts/build_scrip.sh && make libscrip_rt      # full build (gate prereq)
bash scripts/test_icon_rung_suite.sh                 # all 3 modes, the source of truth
bash scripts/test_icon_rung_suite.sh --mode run      # m3 only; pipe | grep '^FAIL'
bash scripts/test_smoke_icon.sh                      # 12-case smoke, all 3 modes
./scrip --dump-bb FILE.icn                           # print BB graph per proc (great for tracing)
./scrip --run FILE.icn                               # m3
./scrip --compile FILE.icn > x.s && as x.s -o x.o && \
  gcc x.o out/libscrip_rt.so -o xbin -Wl,-rpath,$(pwd)/out && ./xbin   # m4 by hand
```
Corpus: `/home/claude/corpus/programs/icon/`  (sources `*.icn`, expected `*.expected`).
Canonical refs: `refs/jcon-master/tran/irgen.icn`, `refs/icon-master/src/runtime/*.r`.
Prior transcript: `/mnt/transcripts/2026-06-13-14-26-42-scrip-icon-bb-m3m4-triage.txt`.

## GATE DISCIPLINE
m2 is a HARD gate: never let `--interp` PASS drop below 200. Run the full three-mode
suite before declaring any win. The jz fix is the kind of change that touches every
conditional branch in BINARY mode — re-run the full suite (not just the rung you were
chasing) after anything in x86_asm.h.
