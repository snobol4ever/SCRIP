# seed/test_sno_cell_2.s — ζ-CELL MACHINE reference embodiment #2   (Lon + Claude, 2026-07-28)
# Assembles: gcc -nostdlib -no-pie test_sno_cell_2.s -o t2 && ./t2
# Subject "BlueBird", pattern:  ( 'Bird' | 'Blue' | LEN(1) )  ( 'lueBird' | 'ird' )
# Expected: CELL2 MATCH start=0 end=8 text=BlueBird
#
# ═══════════════════════════════════════════════════════════════════════════════════
# WHAT THIS SEED ADDS OVER test_sno_cell_1.s
# ═══════════════════════════════════════════════════════════════════════════════════
# cell_1 proved CONTROL: universal fail stub, choice cells, unanchored retry.  Every
# box in it was K=0 — no box ever produced a VALUE, so the OPERAND PROTOCOL
# (DESIGN-SN4-CELL-MACHINE.md, last section) was asserted but never executed.
#
# This seed executes it, and in doing so FALSIFIES the doc's stated geometry and
# replaces it.  The claim under test:
#
#     "operand RESULT cells at [rsp+K..K+16n), left directly on top by the children
#      that just ran (LIFO invariant)"
#
# ⛔ THAT IS FALSE AS WRITTEN.  A producer that RETAINS a choice cell (any ALT with
# untried arms left — i.e. the normal case) leaves 32B of choice header sitting
# between its 16B result and the next operand's base.  Whether it retains is
# PATH-DEPENDENT (arm 1 succeeding retains; arm N succeeding does not), so the gap
# is not a compile-time constant.  Static operand offsets are dead on arrival, and
# with them the whole "( op1 op2 — result )" FORTH contract.
#
# ═══════════════════════════════════════════════════════════════════════════════════
# THE FIX — ONE UNIT CELL PER BOX, ALLOCATION DECOUPLED FROM THE CHOICE CHAIN
# ═══════════════════════════════════════════════════════════════════════════════════
# The doc's law "a resume point is only ever reached by POPPING the cell that names
# it" is what forces the gap: it makes the choice cell a separate, poppable object.
# Drop the pop.  Then:
#
#   ONE 32-BYTE UNIT PER BOX.  Every box carves exactly one UNIT at its α and
#   contributes exactly that on its γ path — retained or not, backtracked or not.
#   The UNIT *is* the result slot AND the choice record AND the δ save.  Locals (if
#   any) are carved ABOVE the unit and freed at γ, so at γ the box's residue is
#   always exactly 32B.
#
#     [U+ 0]  start   u32   result: matched start cursor
#     [U+ 4]  len     u32   result: matched length
#     [U+ 8]  saved_δ u32   cursor on entry — restored on every retry
#     [U+12]  TAG     u32   0=PLAIN 1=BASE 2=CHOICE-pending
#     [U+16]  resume  u64   next-alternative address (0 = none left)
#     [U+24]  prev_CP u64   CP chain link (meaningful only while pending)
#
#   FAILURE CUTS TO THE UNIT, IT DOES NOT POP IT:
#         mov rsp, rbp          # cut — abandoned suffix dies, THIS unit survives
#         mov r14d, [rbp+8]     # δ restored
#         mov rax,  [rbp+16]    # resume
#         mov rbp,  [rbp+24]    # unlink CP
#         jmp rax
#   Five instructions, correct from arbitrary depth — same property cell_1 had.  But
#   because the unit SURVIVES the cut, rsp on arrival at any β is the box's own unit
#   base, and every unit to its LEFT is still exactly where it was.
#
#   ⇒ UNIFORMITY BUYS CONTIGUITY.  Sibling k sits at [rsp + 32k].  Statically.
#     Freeing is what breaks it, so nothing is EVER freed on a success path; the cut
#     (ω) and the whole-match commit are the only reclaimers.  Dead units accumulate
#     inside one scan attempt — the same accepted cost the doc already books under
#     FENCE-SQUASH.
#
# ═══════════════════════════════════════════════════════════════════════════════════
# THE ORDERING GUARANTEE (Lon's requirement, restated as an invariant this file proves)
# ═══════════════════════════════════════════════════════════════════════════════════
#   OPERANDS-BEFORE-OPERATOR.  Emission is POSTFIX: the operator box's α is emitted
#   AFTER all n operand boxes, not before them.  (test_sno_1.c is prefix — `seq_α`
#   runs first and jumps into POS0_α — which is why its operands can never be on the
#   stack at the operator's α; the operator ran first.)
#
#   INVARIANT ⓘ  — at the instant ANY box's α is reached, rsp is the base of its own
#   unit-to-be, and its completed left siblings occupy [rsp+0], [rsp+32], [rsp+64]…
#   in right-to-left order.  After its own `sub rsp,32` they are at [rsp+32k], k≥1.
#
#   Corollary — an n-ary operator emitted after its operands finds
#       op_n   at [rsp+ 0]      (nearest — completed last)
#       op_n-1 at [rsp+32]
#       op_1   at [rsp+32(n-1)]
#   and this holds on FIRST arrival and on every BACKTRACKED re-arrival alike,
#   because the cut restores rsp to a unit base and never disturbs anything left of it.
#
#   ⓘ IS CHECKED AT RUNTIME IN THIS FILE — see the two TRACE points.  The second
#   trace fires after a cross-operand backtrack and shows operand 1 at the SAME
#   self-relative address holding a DIFFERENT value.  That is the whole proof.
#
# ═══════════════════════════════════════════════════════════════════════════════════
# EXECUTION TRACE — subject "BlueBird" (Δ=8), unanchored
# ═══════════════════════════════════════════════════════════════════════════════════
#  head α    carve BASE unit U0{δ=0,TAG=1,resume=head_β}; CP=U0
#  n1 α      carve U1{δ=0,TAG=2,resume=arm2}; CP=U1        ← ⓘ: U0 at [rsp+32]
#    arm1    'Bird' @0 : 'B'✓ 'i'✗              → blob_ω
#            cut to U1, δ=0, CP=U0, jmp arm2    (U1 SURVIVES)
#    arm2    relink resume=arm3, CP=U1; 'Blue' @0 ✓ δ=4 → U1={0,4}
#  n5 α      TRACE ①  reads [rsp+0] = U1 = {start=0,len=4}   ← ⓘ HOLDS
#            carve U2{δ=4,TAG=2,resume=arm2}; CP=U2
#    arm1    'lueBird' @4 : δ+7=11 > 8         → blob_ω  (cut to U2, jmp arm2)
#    arm2    LAST arm — no relink; 'ird' @4 : 'B'✗ → blob_ω
#            CP is U1 ⇒ cut lands at U1, DISCARDING U2 wholesale (free undo),
#            δ=0 restored, CP=U0, jmp n1_arm3        ← CROSS-OPERAND BACKTRACK
#    arm3    LAST arm of n1; LEN(1) @0 ✓ δ=1  → U1={0,1}
#  n5 α      TRACE ②  reads [rsp+0] = U1 = {start=0,len=1}
#            SAME ADDRESS, DIFFERENT VALUE  ← ⓘ HOLDS ACROSS BACKTRACK
#            carve U2'{δ=1,TAG=2}; CP=U2'
#    arm1    'lueBird' @1 ✓ δ=8 → U2'={1,7}
#  n8 α      OPERATOR — operands live at [rsp+0]=U2'={1,7}, [rsp+32]=U1={0,1}
#            start←U1.start=0 ; len←1+7=8 ; carve U3={0,8}
#  head γ    walk CP to TAG==BASE, cut, report {0,8} = "BlueBird"
# ═══════════════════════════════════════════════════════════════════════════════════
# REGISTER CONTRACT (unchanged from cell_1)
#   r13 Σ subject base · r14 δ cursor · r15 Δ length · rbp CP · rsp ζ frontier
#   rax rcx rdx rsi rdi r8 r9 scratch   (rcx/r11 additionally clobbered by syscall)
# ═══════════════════════════════════════════════════════════════════════════════════

    .intel_syntax noprefix
    .globl _start
    .text

    .set U_START,   0
    .set U_LEN,     4
    .set U_DELTA,   8
    .set U_TAG,    12
    .set U_RESUME, 16
    .set U_PREV,   24
    .set U_SIZE,   32

    .set TAG_PLAIN,  0
    .set TAG_BASE,   1
    .set TAG_CHOICE, 2

# ═══════════════════════════════════════════════════════════════════════════════════
# SCAN BRACKET — HEAD
# ═══════════════════════════════════════════════════════════════════════════════════
_start:
n0_head_α:
    lea     r13, [rip + subj]           # Σ
    xor     r14d, r14d                  # δ ← 0
    mov     r15d, 8                     # Δ ← 8   ("BlueBird")
    xor     ebp, ebp                    # CP ← 0
    sub     rsp, U_SIZE                 # carve U0 — the ONLY carve the head ever does

# re-entry for unanchored retry: the cut already left rsp AT U0's base, so the
# retry re-initialises the unit IN PLACE.  No leak, no second carve.
n0_head_repush:
    mov     [rsp + U_DELTA], r14d
    mov     dword ptr [rsp + U_TAG], TAG_BASE
    lea     rax, [rip + n0_head_β]
    mov     [rsp + U_RESUME], rax
    mov     [rsp + U_PREV], rbp
    mov     rbp, rsp                    # CP ← U0
                                        # fall through to op1's α

# ═══════════════════════════════════════════════════════════════════════════════════
# OPERAND 1 — BOX IR_MATCH_ALTERNATE   'Bird' | 'Blue' | LEN(1)      [n1]
# ═══════════════════════════════════════════════════════════════════════════════════
n1_alt_α:
    sub     rsp, U_SIZE                 # carve U1   ⓘ U0 now at [rsp+32]
    mov     [rsp + U_DELTA], r14d
    mov     dword ptr [rsp + U_TAG], TAG_CHOICE
    lea     rax, [rip + n1_arm2]
    mov     [rsp + U_RESUME], rax
    mov     [rsp + U_PREV], rbp
    mov     rbp, rsp                    # CP ← U1
                                        # fall into arm 1

n1_arm1:                                # 'Bird'
    mov     eax, r14d
    add     eax, 4
    cmp     eax, r15d
    ja      blob_ω
    mov     ecx, r14d
    cmp     byte ptr [r13 + rcx],     66     # 'B'
    jne     blob_ω
    cmp     byte ptr [r13 + rcx + 1], 105    # 'i'
    jne     blob_ω
    cmp     byte ptr [r13 + rcx + 2], 114    # 'r'
    jne     blob_ω
    cmp     byte ptr [r13 + rcx + 3], 100    # 'd'
    jne     blob_ω
    add     r14d, 4
    mov     ecx, 4
    jmp     n1_γ

n1_arm2:                                # β-delivered.  rsp = U1 base by construction.
    lea     rax, [rip + n1_arm3]        # RELINK: one arm still untried
    mov     [rsp + U_RESUME], rax
    mov     [rsp + U_PREV], rbp
    mov     rbp, rsp
                                        # 'Blue'
    mov     eax, r14d
    add     eax, 4
    cmp     eax, r15d
    ja      blob_ω
    mov     ecx, r14d
    cmp     byte ptr [r13 + rcx],     66     # 'B'
    jne     blob_ω
    cmp     byte ptr [r13 + rcx + 1], 108    # 'l'
    jne     blob_ω
    cmp     byte ptr [r13 + rcx + 2], 117    # 'u'
    jne     blob_ω
    cmp     byte ptr [r13 + rcx + 3], 101    # 'e'
    jne     blob_ω
    add     r14d, 4
    mov     ecx, 4
    jmp     n1_γ

n1_arm3:                                # LAST arm — NO relink (CP already points past U1)
    mov     eax, r14d                   # LEN(1)
    inc     eax
    cmp     eax, r15d
    ja      blob_ω
    inc     r14d
    mov     ecx, 1
                                        # fall into γ

n1_γ:                                   # ecx = len ; rsp = U1 base
    mov     eax, [rsp + U_DELTA]        # start = δ saved at α
    mov     [rsp + U_START], eax
    mov     [rsp + U_LEN], ecx
                                        # fall through — SEQ IS NOTHING

# ═══════════════════════════════════════════════════════════════════════════════════
# OPERAND 2 — BOX IR_MATCH_ALTERNATE   'lueBird' | 'ird'             [n5]
# ═══════════════════════════════════════════════════════════════════════════════════
n5_alt_α:
# ── TRACE ⓘ : operand 1's unit must be at [rsp+0] RIGHT NOW ────────────────────────
    mov     eax, [rsp + U_START]
    add     al, 48
    mov     byte ptr [rip + tr_s], al
    mov     eax, [rsp + U_LEN]
    add     al, 48
    mov     byte ptr [rip + tr_l], al
    mov     eax, 1
    mov     edi, 1
    lea     rsi, [rip + tr]
    mov     edx, OFFSET tr_len
    syscall
# ──────────────────────────────────────────────────────────────────────────────────
    sub     rsp, U_SIZE                 # carve U2   ⓘ U1 now at [rsp+32]
    mov     [rsp + U_DELTA], r14d
    mov     dword ptr [rsp + U_TAG], TAG_CHOICE
    lea     rax, [rip + n5_arm2]
    mov     [rsp + U_RESUME], rax
    mov     [rsp + U_PREV], rbp
    mov     rbp, rsp                    # CP ← U2

n5_arm1:                                # 'lueBird'  (7)
    mov     eax, r14d
    add     eax, 7
    cmp     eax, r15d
    ja      blob_ω
    mov     ecx, r14d
    cmp     byte ptr [r13 + rcx],     108    # 'l'
    jne     blob_ω
    cmp     byte ptr [r13 + rcx + 1], 117    # 'u'
    jne     blob_ω
    cmp     byte ptr [r13 + rcx + 2], 101    # 'e'
    jne     blob_ω
    cmp     byte ptr [r13 + rcx + 3], 66     # 'B'
    jne     blob_ω
    cmp     byte ptr [r13 + rcx + 4], 105    # 'i'
    jne     blob_ω
    cmp     byte ptr [r13 + rcx + 5], 114    # 'r'
    jne     blob_ω
    cmp     byte ptr [r13 + rcx + 6], 100    # 'd'
    jne     blob_ω
    add     r14d, 7
    mov     ecx, 7
    jmp     n5_γ

n5_arm2:                                # LAST arm — NO relink.  'ird' (3)
    mov     eax, r14d
    add     eax, 3
    cmp     eax, r15d
    ja      blob_ω
    mov     ecx, r14d
    cmp     byte ptr [r13 + rcx],     105    # 'i'
    jne     blob_ω
    cmp     byte ptr [r13 + rcx + 1], 114    # 'r'
    jne     blob_ω
    cmp     byte ptr [r13 + rcx + 2], 100    # 'd'
    jne     blob_ω
    add     r14d, 3
    mov     ecx, 3

n5_γ:
    mov     eax, [rsp + U_DELTA]
    mov     [rsp + U_START], eax
    mov     [rsp + U_LEN], ecx
                                        # fall through

# ═══════════════════════════════════════════════════════════════════════════════════
# OPERATOR — BOX IR_CONCAT (2-ary)                                   [n8]
# Emitted AFTER both operands.  This is the postfix ordering the whole seed exists
# to demonstrate: at THIS α, both operand units are already on the stack, at static
# self-relative offsets, and have been on every arrival at this label.
#     [rsp + 0]  = operand 2's unit
#     [rsp + 32] = operand 1's unit
# ═══════════════════════════════════════════════════════════════════════════════════
n8_concat_α:
    mov     eax, [rsp + U_SIZE + U_START]   # op1.start   ← the guarantee, used
    mov     ecx, [rsp + U_SIZE + U_LEN]     # op1.len
    add     ecx, [rsp + U_LEN]              # + op2.len
    sub     rsp, U_SIZE                     # carve U3 (uniform: operator is a box too)
    mov     [rsp + U_START], eax
    mov     [rsp + U_LEN], ecx
    mov     [rsp + U_DELTA], r14d
    mov     dword ptr [rsp + U_TAG], TAG_PLAIN
    mov     qword ptr [rsp + U_RESUME], 0
    mov     qword ptr [rsp + U_PREV], 0
    jmp     n0_head_γ

# ═══════════════════════════════════════════════════════════════════════════════════
# UNIVERSAL FAILURE STUB — five instructions, cuts but does NOT pop
# ═══════════════════════════════════════════════════════════════════════════════════
blob_ω:
    mov     rsp, rbp                    # cut — abandoned suffix dies; THIS unit lives
    mov     r14d, [rbp + U_DELTA]       # δ restored
    mov     rax,  [rbp + U_RESUME]      # resume address
    mov     rbp,  [rbp + U_PREV]        # unlink CP
    jmp     rax

# ═══════════════════════════════════════════════════════════════════════════════════
# BASE resume — unanchored advance (SPITBOL Ch.18 step 6, &ANCHOR=0 arm)
# ═══════════════════════════════════════════════════════════════════════════════════
n0_head_β:
    cmp     r14d, r15d
    jae     n0_head_ω
    inc     r14d
    jmp     n0_head_repush              # rsp already at U0's base

# ═══════════════════════════════════════════════════════════════════════════════════
# WHOLE-MATCH γ — walk CP chain to the BASE unit, cut, report
# ═══════════════════════════════════════════════════════════════════════════════════
n0_head_γ:
    mov     r8d, [rsp + U_START]        # operator result: start
    mov     r9d, [rsp + U_LEN]          # operator result: len
n0_head_γ_seek:
    cmp     dword ptr [rbp + U_TAG], TAG_BASE
    je      n0_head_γ_cut
    mov     rbp, [rbp + U_PREV]
    jmp     n0_head_γ_seek
n0_head_γ_cut:
    mov     rsp, rbp
    mov     rbp, [rbp + U_PREV]         # bracket exit: caller's CP restored

    mov     eax, r8d
    add     al, 48
    mov     byte ptr [rip + msg_s], al
    mov     eax, r8d
    add     eax, r9d
    add     al, 48
    mov     byte ptr [rip + msg_e], al
    mov     eax, 1                      # write header
    mov     edi, 1
    lea     rsi, [rip + msg]
    mov     edx, OFFSET msg_len
    syscall
    mov     eax, 1                      # write the matched span itself
    mov     edi, 1
    lea     rsi, [r13 + r8]
    mov     edx, r9d
    syscall
    mov     eax, 1
    mov     edi, 1
    lea     rsi, [rip + nl]
    mov     edx, 1
    syscall
    xor     edi, edi
    mov     eax, 60
    syscall

# ═══════════════════════════════════════════════════════════════════════════════════
n0_head_ω:
    mov     eax, 1
    mov     edi, 1
    lea     rsi, [rip + nomsg]
    mov     edx, OFFSET nomsg_len
    syscall
    mov     edi, 1
    mov     eax, 60
    syscall

# ═══════════════════════════════════════════════════════════════════════════════════
    .data
subj:     .ascii  "BlueBird"
tr:       .ascii  "  [inv] op2.alpha sees operand1 unit @[rsp+0] = {start="
tr_s:     .byte   63
          .ascii  ",len="
tr_l:     .byte   63
          .ascii  "}\n"
    .set  tr_len, . - tr
msg:      .ascii  "CELL2 MATCH start="
msg_s:    .byte   63
          .ascii  " end="
msg_e:    .byte   63
          .ascii  " text="
    .set  msg_len, . - msg
nl:       .byte   10
nomsg:    .ascii  "CELL2 NOMATCH\n"
    .set  nomsg_len, . - nomsg
