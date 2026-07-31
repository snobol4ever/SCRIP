# test_sno_dl_1.s — THE DYNAMIC LINK GLUE PAIR, hand embodiment (s22e, 2026-07-31)
# =============================================================================================
# ⛔ STATUS FOR THE NEXT SESSION — READ THIS FIRST, DO NOT SKIP.
#   WRITTEN BY HAND, DESIGN-ONLY SESSION, WRITTEN AT ~85% CONTEXT.  What is TRUE of this file
#   is ONLY what the session log shows it verified: it ASSEMBLES and it EXECUTES standalone and
#   its stdout matches the expected string below.  What is NOT true of it:
#     · it has NEVER been through scrip, any template, any encoder, or either medium;
#     · NO watermark, NO crosscheck, NO oracle (sbl) comparison was run this session;
#     · the encoders it implies (a DL push + a DL floater) DO NOT EXIST YET in x86_asm.h;
#     · it is NOT a golden artifact — nothing may diff against it.
#   It is a HAND EMBODIMENT in the sense of GOAL-SN4-CELL-MACHINE.md's own rule ("a heap-carve
#   embodiment should be executed and oracle-checked BEFORE any encoder learns the shape").  The
#   EXECUTED half is done.  The ORACLE-CHECKED half is NOT — there is no .sno oracle for a raw
#   control-flow shape; the honest equivalent is CELL-7 driving a real *PATTERN program through
#   these two glues and diffing sbl.  DO THAT BEFORE YOU TRUST A LINE OF IT.
#
# WHY IT EXISTS: Lon, s22e — "we need STATIC links implemented as JMP like we have today, BUT we
#   need DYNAMIC links to CALL/SUSPEND through to other BB-graphs."  Contract: DESIGN-SN4-CELL-
#   MACHINE.md § DL.  Rung: GOAL-SN4-CELL-MACHINE.md RUNG CELL-DL.  Customers: *PATTERN and
#   deferred pattern evaluation (DL-PASSTHRU), DEFINE'd-function call via IR_SAVE_RESTORE/IR_CALL
#   and EVAL/CODE (DL-ONESHOT), and the C→graph bootstrap (degenerate DL-ONESHOT).
#
# THE ONE RECORD, BOTH SHAPES.  32B at rbp, built by the CALLER, newest-first via its prev field:
#     [rbp+ 0]  resume     — DL-PASSTHRU: inner graph deposits its β re-entry address here.
#                            DL-ONESHOT: unused pad (x86_call_frame_enter spends `sub rsp,8`).
#                            ⭐ THIS SLOT IS THE UNIFICATION QUESTION: the choice cell's [+0] is
#                            ALSO resume_addr.  If Lon rules the two records are one object, a DL
#                            record is a choice cell with TAG 4/5 and the fail-stub walk already
#                            serves cross-graph unwind.  UNRULED — do not assume either way.
#     [rbp+ 8]  prev rbp   — caller's rbp, restored by both floaters.
#     [rbp+16]  γ wire     — caller's OWN success landing pad (a real label in caller code).
#     [rbp+24]  ω wire     — caller's OWN failure landing pad.
#   Ordering is push ω, push γ, push rbp, push resume — verified against x86_call_frame_enter in
#   src/templates/x86_asm.h and against the shipped caller-side shape in test_sno_stmt_frame_2.s
#   (`lea rcx,[rip+.Lret_γ_Fr]` / `lea rdx,[rip+.Lret_ω_Fr]` / push / push / push rbp / mov rbp,rsp).
#   4 pushes = 32B, so the SysV 16-parity invariant holds by construction.
#
# DL-ONESHOT — the invoked graph runs to EXACTLY ONE of γ/ω and its whole accumulation dies there.
#   Caller: build record, fetch target FROM DATA, `jmp rax`.  Callee, from ANY statement/BB depth:
#   the floater `mov rsp,rbp` / load wire / restore prev rbp / `add rsp,32` / `jmp wire`.
#   NOTHING CONSUMES; SCOPE FREES; THE rbp-CUT *IS* THE FREE.  This is x86_return_floater (wire
#   +16) and x86_freturn_floater (wire +24), both already in x86_asm.h — a DL one-shot needs NO
#   new encoder, only a caller that spells the record.  Manual: Ch.8 p.103 RETURN / FRETURN.
#
# DL-PASSTHRU — the invoker forwards α→inner-α and β→inner-β; inner γ/ω land on the invoker's own
#   pads.  THE DIFFERENCE FROM ONE-SHOT IS ONE THING AND ONLY ONE THING: **at γ the inner graph
#   DOES NOT CUT.**  Its cells stay live on the spine — that is what a suspended alternative IS
#   (manual Ch.18 p.204 step 3: the pushed {alternative, cursor} is exactly this) — and it
#   deposits its β re-entry address into [rbp+0] before jumping the γ wire.  The invoker's β is
#   then `mov rax,[rbp+0]; jmp rax`.  On exhaustion the inner graph takes the SAME floater shape
#   as FRETURN, which cuts the whole retained subtree in one `mov rsp,rbp`.
#   ⛔ LOAD-BEARING INVARIANT, THE ONE WAY THIS BREAKS: between a γ delivery and the matching β,
#   NOTHING may leave rsp below the record.  The suspended graph's locals live there.  Any
#   unbalanced push/carve on the driver's side silently eats them (and per the 914_lgt law it
#   returns WRONG ANSWERS, it does not crash).  LIFO adjacency is the whole ownership discipline.
#
# ⭐ WHAT THIS DOES *NOT* SOLVE, STATED PLAINLY SO NOBODY READS IT AS DONE: the cross-graph
#   pass-thru gap at HEAD is NOT a missing transfer — rt_defer_step already returns a frame base
#   that bb_match_defer enters via `jmp rax`.  The gap is that deferred-evaluation STATE lives in
#   g_dfx[]/g_dfx_top (src/runtime/pattern_match.c:941), a C GLOBAL STACK — forbidden by
#   ARCH-ICON.md, and re-entrancy-hostile for a *FN(T) deferred inside a *FN(T).  THIS FILE SHOWS
#   THE SHAPE THAT STATE MOVES ONTO.  CELL-7 is a STATE-RELOCATION rung, not a codegen rung.
#
# BUILD / RUN (no libc, syscalls only, so it needs nothing from the tree):
#     as --64 -o /tmp/dl1.o seed/test_sno_dl_1.s && ld -o /tmp/dl1 /tmp/dl1.o && /tmp/dl1
# EXPECTED STDOUT, exactly:  AF12x
#     A  = DL-ONESHOT, dynamically-bound target #1, returns via γ wire carrying a value in r8
#     F  = DL-ONESHOT, SAME call site, slot REBOUND at runtime to a target that takes the ω wire
#     12 = DL-PASSTHRU, inner generator suspends and is re-entered through the deposited resume
#     x  = DL-PASSTHRU, inner generator exhausts and cuts to the ω wire
# =============================================================================================
.intel_syntax noprefix
.section .data
fnslot:  .quad fn_ok                      # DYNAMIC binding: rewritten at runtime below.  This
genslot: .quad gen_alpha                  # slot IS the DEFINE / stored-pattern indirection —
                                          # the target is DATA, which is why rel32 cannot serve.
.section .text
.globl _start
# ---------------------------------------------------------------------------------------------
# putc — al = char.  Balanced: pushes 8, pops 8.  Clobbers rcx/r11 (syscall) + rax/rsi/rdi/rdx.
# Deliberately uses call/ret on the SAME rsp the ζ spine rides — legal, and the point: a
# suspended pass-thru graph's cells sit BELOW this and survive it because it is balanced.
# ---------------------------------------------------------------------------------------------
putc:
    push rax
    mov  rsi, rsp
    mov  rax, 1
    mov  rdi, 1
    mov  rdx, 1
    syscall
    pop  rax
    ret
# =============================================================================================
# DL-ONESHOT callees.  Each reaches its wire with the floater — from arbitrary depth, hence
# "floater": it does not care how many boxes or statements deep it is when it fires.
# =============================================================================================
fn_ok:                                    # the RETURN role: deliver a value, take the γ wire
    mov  r8, 65                           # 'A' — stands for the function's result descriptor
    sub  rsp, 48                          # PROVE THE CUT: junk depth the floater must discard
    mov  rsp, rbp                         # ── x86_return_floater (x86_asm.h), wire at +16 ──
    mov  rcx, [rbp+16]
    mov  rbp, [rbp+8]
    add  rsp, 32
    jmp  rcx
fn_fail:                                  # the FRETURN role: no value, take the ω wire
    sub  rsp, 96
    mov  rsp, rbp                         # ── x86_freturn_floater, wire at +24 ──
    mov  rcx, [rbp+24]
    mov  rbp, [rbp+8]
    add  rsp, 32
    jmp  rcx
# =============================================================================================
# DL-PASSTHRU callee — a two-value generator.  Allocates its OWN cell at α, keeps it across γ,
# releases it only at ω.  This is THE MODEL's law verbatim: allocates at α, reads/writes ONLY
# its own cell, releases at ω, jumps.
# =============================================================================================
gen_alpha:
    sub  rsp, 16                          # ITS OWN cell.  16B granular ⇒ 16-parity preserved.
    mov  qword ptr [rsp], 0               # local: how many instances delivered so far
gen_yield:
    mov  rax, [rsp]                       # ⭐ at β re-entry rsp is EXACTLY here — LIFO invariant
    add  rax, 1
    mov  [rsp], rax
    cmp  rax, 2
    jg   gen_omega                        # shy, bounded: two instances then exhaust
    lea  rcx, [rip + gen_resume]          # ── THE DEPOSIT: publish β re-entry into the record ──
    mov  [rbp+0], rcx
    add  rax, 48
    mov  r8, rax                          # '1' then '2'
    jmp  qword ptr [rbp+16]               # ── γ WITHOUT CUTTING: the cell above stays live ──
gen_resume:
    jmp  gen_yield                        # invoker's β lands here through [rbp+0]
gen_omega:
    mov  rsp, rbp                         # ── same floater shape as FRETURN: one cut takes the
    mov  rcx, [rbp+24]                    #    generator's cell AND the record together ──
    mov  rbp, [rbp+8]
    add  rsp, 32
    jmp  rcx
# =============================================================================================
_start:
    xor  rbp, rbp                         # the C→graph bootstrap's degenerate record: prev = 0
# ---- DL-ONESHOT call #1, target resolved from data → fn_ok → γ ------------------------------
    lea  rcx, [rip + os1_γ]               # ── x86_call_frame_enter: BOTH LANDING PADS ARE DATA ─
    lea  rdx, [rip + os1_ω]
    push rdx                              # [+24] ω
    push rcx                              # [+16] γ
    push rbp                              # [+8]  prev rbp
    sub  rsp, 8                           # [+0]  pad (one-shot never reads it)
    mov  rbp, rsp
    lea  rax, [rip + fnslot]              # ── DYNAMIC α ENTRY: target is DATA, not a rel32 ──
    mov  rax, [rax]
    jmp  rax
os1_γ:
    mov  rax, r8
    call putc                             # 'A'
    lea  rax, [rip + fnslot]              # REBIND THE SAME CALL SITE — this is DEFINE's whole
    lea  rcx, [rip + fn_fail]             # point (Ch.8 p.102: the binding happens at RUNTIME)
    mov  [rax], rcx
    jmp  os2
os1_ω:
    mov  rax, 70
    call putc
os2:
# ---- DL-ONESHOT call #2, SAME site, slot now bound to fn_fail → ω ---------------------------
    lea  rcx, [rip + os2_γ]
    lea  rdx, [rip + os2_ω]
    push rdx
    push rcx
    push rbp
    sub  rsp, 8
    mov  rbp, rsp
    lea  rax, [rip + fnslot]
    mov  rax, [rax]
    jmp  rax
os2_γ:
    mov  rax, r8
    call putc
    jmp  pt
os2_ω:
    mov  rax, 70                          # 'F' — the ω wire fired, no value produced
    call putc
pt:
# ---- DL-PASSTHRU: enter, consume every γ by backtracking into β, land on ω ------------------
    lea  rcx, [rip + pt_γ]
    lea  rdx, [rip + pt_ω]
    push rdx                              # [+24] ω
    push rcx                              # [+16] γ
    push rbp                              # [+8]  prev rbp
    xor  rax, rax
    push rax                              # [+0]  resume = 0 SENTINEL — the one shape difference
    mov  rbp, rsp                         #        from one-shot's `sub rsp,8`: it MUST be zeroed,
    lea  rax, [rip + genslot]             #        because β tests it to tell "never suspended"
    mov  rax, [rax]                       #        from "has a resume."
    jmp  rax                              # α → inner α
pt_γ:
    mov  rax, r8
    call putc                             # '1', then '2'.  BALANCED — see the load-bearing
    mov  rax, [rbp+0]                     # invariant above; the generator's cell is under us.
    test rax, rax
    jz   pt_ω                             # sentinel still 0 ⇒ nothing suspended ⇒ treat as ω
    jmp  rax                              # ── β: re-enter the SUSPENDED graph through its own
pt_ω:                                     #      published resume.  No C broker in this path. ──
    mov  rax, 120
    call putc                             # 'x'
# ---- done -----------------------------------------------------------------------------------
    mov  rax, 10
    call putc
    mov  rax, 60
    xor  rdi, rdi
    syscall
