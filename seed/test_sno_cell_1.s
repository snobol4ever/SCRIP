# seed/test_sno_cell_1.s — ζ-CELL MACHINE reference embodiment #1  (Lon + Claude, 2026-07-28)
# Assembles: gcc -nostdlib -no-pie test_sno_cell_1.s -o t && ./t
# Subject "abhello", pattern: ( 'x' | 'he' ) 'llo'  — unanchored, no anchor keyword.
# Expected: CELL MATCH start=2 end=7   (flip subj to "abhexlo" → CELL NOMATCH)
#
# ═══════════════════════════════════════════════════════════════════════════════════
# WHAT A CELL MACHINE IS
# ═══════════════════════════════════════════════════════════════════════════════════
#
# A cell machine executes a Byrd-box graph — every language construct is a four-port
# box (α start · β resume · γ succeed · ω fail) — by keeping ALL match-lifetime state
# as uniform typed CELLS on ONE LIFO stack, where a CONTROL transfer and a DEPTH
# transfer are THE SAME ACT: you only ever arrive at a resume point by popping the
# cell that names it, and the pop leaves rsp exactly where the pusher stood.
#
# Six laws:
#
#   1. PER-BB SELF-ALLOCATION.
#      Every box allocates its own RESULT and LOCALS callee-side at its α
#      (sub rsp, K_box; K_box is a box-local constant — no graph-wide geometry
#      pass exists).  Freeing is caller-side on both exit edges:
#        γ  the consumer overwrites / shrinks-to-result the cell (net FORTH word).
#        ω  the cut (mov rsp,rbp) reclaims the failed box, its consumed operands,
#           and the entire abandoned suffix wholesale — one instruction.
#      Corollary: recursion, EVAL/CODE, and stored-pattern entry are free.  A fresh
#      α carves a fresh frame.  Runtime-created code needs no compile-time plan
#      because none exists.
#
#   2. SELF-RELATIVE ADDRESSING ONLY.
#      A box reads [rsp+0..K) for its own locals and [rsp+K..K+16n) for its
#      operands — the n 16-byte DESCR cells its children left at TOS by LIFO
#      adjacency.  Producers deliver at TOS: a POSITION, not an address.  No box
#      ever holds another box's address.  The reach-over disease (static cross-box
#      displacements kept consistent through dynamic depth — op_flat_disp, fc_geom,
#      parked anchors, rbp floors) is UNREPRESENTABLE, not merely prevented.
#
#   3. CP CHAIN.
#      rbp = CP, pointer to the newest CHOICE cell (WAM register B; callee-saved,
#      survives every C runtime call).  CHOICE cell, 32B, ascending from CP:
#        [CP+ 0]  resume address  (code pointer)
#        [CP+ 8]  low32=saved δ · high32=TAG  (0=CHOICE 1=BASE 2=FENCE 3=ARBNO)
#        [CP+16]  prev_CP
#        [CP+24]  saved mark-head MH=r12  (0 until CELL-5 capture-mark rung)
#      BACKTRACKABLE-BOX LAW: carve LOCALS first, THEN push the 32B header.
#      Header at TOS; locals at [rsp+32..32+K).  The fail-stub's pops land rsp
#      exactly at the box's own locals base — self-relative at every β entry.
#
#   4. UNIVERSAL FAILURE — ONE stub, shared, six instructions.
#        mov rsp, rbp      # cut: discard abandoned suffix down to CP
#        pop rax           # resume address
#        pop r14           # saved δ restored  (TAG rides r14 high32, harmless)
#        pop rbp           # unlink CP
#        add rsp, 8        # discard MH slot (pre-CELL-5)
#        jmp rax           # re-enter at the resume point
#      Correct from ARBITRARY depth.  Replaces: op_flat_disp, fc_geom prefix sums,
#      scanbase rebasing, parked anchors, rbp floors, and the entire ω-edge topology
#      of the blob.  The cut also orphans the abandoned suffix's capture marks —
#      FREE UNDO, zero instructions.
#
#   5. SEQUENCE IS NOTHING.
#      Concatenation is emission order; failure is the stack's business (SPITBOL
#      manual Ch.18 step 6).  A SEQUENCE box emits zero instructions — it is pure
#      wiring.  This file demonstrates it: the SEQ between ALT and LIT 'llo' is the
#      absence of any label or jump between n2_alt_γ and n3_lit_llo_a.
#
#   6. IT IS THE MANUAL'S OWN MACHINE.
#      SPITBOL Ch.18 p.204: "a pushdown stack is used to remember backtracking
#      possibilities."  Step 3 pushes {alternative, cursor}; step 6 pops on failure;
#      empty stack + &ANCHOR=0 advances the start cursor.  The CHOICE cell IS that
#      stack entry.  The BASE cell IS the unanchored retry engine.  WAM-flavored FORTH.
#
# ───────────────────────────────────────────────────────────────────────────────────
# REGISTER CONTRACT
#   r13  Σ  subject base (byte pointer)
#   r14  δ  cursor, 0-based (r14d live; high32 temporarily carries TAG on pop — ok)
#   r15  Δ  subject length
#   rbp  CP newest choice cell (callee-saved)
#   rsp  ζ  frontier
#   r12  MH mark-head chain (pre-CELL-5: pushed 0, discarded on unwind)
#   rax rcx rdx rsi rdi  scratch
#
# ───────────────────────────────────────────────────────────────────────────────────
# PROGRAM BEING COMPILED (SNOBOL4 source notation)
#
#     SUBJ = 'abhello'
#     SUBJ  ?  ( 'x'  |  'he' )  'llo'
#              └── ALT ──────┘  └─SEQ─┘
#
# EXECUTION TRACE (unanchored, δ starts at 0)
#   δ=0: HEAD saves rbp, loads Σ/δ/Δ; pushes BASE{resume=n0_head_β, δ=0, TAG=1}
#        n2_alt α: pushes CHOICE{resume=n2_alt_β, δ=0, TAG=0}
#        n1_lit_x α: 'a' ≠ 'x' → blob_ω
#   blob_ω: rsp=rbp(CHOICE); pop resume=n2_alt_β; pop r14=0; pop rbp=BASE; +8
#        n2_alt β (arm2): n3_lit_he α: 'ab' ≠ 'he' → blob_ω
#   blob_ω: rsp=rbp(BASE); pop resume=n0_head_β; pop r14=0; pop rbp=prev; +8
#        n0_head_β: &ANCHOR=0 → δ=1 → n0_head_α_repush (re-push BASE{δ=1})
#   δ=1: same; 'b'≠'x', 'bh'≠'he' → n0_head_β → δ=2 → n0_head_α_repush
#   δ=2: n1_lit_x: 'a'≠'x' → blob_ω (pops CHOICE, restores δ=2, arm2)
#        n3_lit_he: [r13+2]='h' ✓  [r13+3]='e' ✓ → δ=4
#        n2_alt_γ: ZERO INSTRUCTIONS (CHOICE cell lives below — nobody reads it)
#        SEQ: ZERO INSTRUCTIONS (pure wiring — absent between merge and lit_llo)
#        n4_lit_llo: [r13+4..6]='llo' ✓ → δ=7
#        n0_head_γ: walk CP chain to TAG==BASE → cut → pop start=2 from the cell
#        output: "CELL MATCH start=2 end=7"
# ═══════════════════════════════════════════════════════════════════════════════════

    .intel_syntax noprefix
    .globl _start
    .text

# ═══════════════════════════════════════════════════════════════════════════════════
# SCAN BRACKET — HEAD  (the one real box; saves caller rbp; loads Σ/δ/Δ)
# Ports:  α = _start (statement entry)
#         γ = n0_head_γ (whole match succeeded; commit walk lives here at CELL-5+)
#         ω = n0_head_ω (subject exhausted with no match)
# Cell:   BASE cell pushed at n0_head_α_repush: {resume=n0_head_β, δ=start, TAG=1}
# ═══════════════════════════════════════════════════════════════════════════════════
_start:
n0_head_α:                              # α of the scan bracket (ELF entry alias above)
    lea     r13, [rip + subj]           #  Σ ← &subj
    xor     r14d, r14d                  #  δ ← 0
    mov     r15d, 7                     #  Δ ← 7  (len "abhello")
    # (production HEAD also saves caller rbp here — omitted; _start owns the machine)

# ───────────────────────────────────────────────────────────────────────────────────
n0_head_α_repush:                                # re-entry point for unanchored retry
# BASE cell push: {[+0]=n0_head_β, [+8]=δ|TAG<<32 (TAG=1), [+16]=prev_CP, [+24]=0}
    xor     rax, rax
    push    rax                         #  [+24]  MH = 0  (pre-CELL-5)
    push    rbp                         #  [+16]  prev_CP
    push    r14                         #  [+ 8]  saved δ  (low32); TAG=1 below
    mov     dword ptr [rsp + 4], 1      #  [+12]  TAG = BASE (1)
    lea     rax, [rip + n0_head_β]
    push    rax                         #  [+ 0]  resume address
    mov     rbp, rsp                    #  CP ← this cell
                                        #  fall through to pattern α (no jmp needed)

# ═══════════════════════════════════════════════════════════════════════════════════
# BOX: IR_MATCH_ALTERNATE  'x' | 'he'        [n2_alt]
# Ports:  α = n2_alt_α     enter; push CHOICE for arm2; fall into arm1
#         β = n2_alt_β  delivered BY blob_ω (δ restored, CP=BASE)
#         γ = n2_alt_γ any arm success; ZERO INSTRUCTIONS (pending cell harmless)
#         ω → blob_ω     (arm2 fail consumed the CHOICE cell; falls through to BASE)
# Cell:   CHOICE{[+0]=n2_alt_β, [+8]=δ|0<<32, [+16]=prev_CP=BASE, [+24]=0}
# Locals: K=0  (no carve — header IS the entire allocation)
# ═══════════════════════════════════════════════════════════════════════════════════
n2_alt_α:                               # α — push CHOICE, fall into arm 1
    xor     rax, rax
    push    rax                         #  [+24]  MH = 0
    push    rbp                         #  [+16]  prev_CP  (= BASE)
    push    r14                         #  [+ 8]  saved δ; TAG=0 (CHOICE, no store needed)
    lea     rax, [rip + n2_alt_β]
    push    rax                         #  [+ 0]  resume = arm2 entry
    mov     rbp, rsp                    #  CP ← this CHOICE cell
                                        #  fall into arm 1

# ───────────────────────────────────────────────────────────────────────────────────
# BOX: IR_MATCH_LIT  'x'                     [n1_lit_x]   (arm 1 of ALT)
# Ports:  α = n1_litx_α   (fall-through from n2_alt_α)
#         β  — none (transient box; blob_ω pops CHOICE and lands in arm2)
#         γ  — advance δ, fall to n2_alt_γ
#         ω → blob_ω
# Locals: K=0  (transient — no carve, no retain)
# ═══════════════════════════════════════════════════════════════════════════════════
n1_litx_α:                             # α
    mov     eax, r14d
    cmp     eax, r15d                   #  δ < Δ ?
    jae     blob_ω                    #  ω — the ONE stub; always this label
    cmp     byte ptr [r13 + rax], 120   #  [Σ+δ] == 'x' ?
    jne     blob_ω                    #  ω
    inc     r14d                        #  δ += 1  (len 'x')
    jmp     n2_alt_γ                #  γ

# ───────────────────────────────────────────────────────────────────────────────────
# n2_alt β-delivery point: blob_ω consumed the CHOICE cell, restored δ and rbp=BASE
n2_alt_β:                            # β  (arm 2 entry — rsp = locals base = rsp at α)

# BOX: IR_MATCH_LIT  'he'                    [n3_lit_he]  (arm 2 of ALT)
# Ports:  α = n2_alt_β  (β-delivered; same address by emission order)
#         β  — none (transient)
#         γ  — advance δ, fall to n2_alt_γ
#         ω → blob_ω  (pops BASE cell → n0_head_β → retry or nomatch)
# Locals: K=0
# ═══════════════════════════════════════════════════════════════════════════════════
n3_lithe_α:                            # α  (coincides with n2_alt_β)
    mov     eax, r14d
    add     eax, 2
    cmp     eax, r15d                   #  δ+2 ≤ Δ ?
    ja      blob_ω                    #  ω
    mov     ecx, r14d
    cmp     byte ptr [r13 + rcx],     104  #  'h'
    jne     blob_ω                    #  ω
    cmp     byte ptr [r13 + rcx + 1], 101  #  'e'
    jne     blob_ω                    #  ω
    add     r14d, 2                     #  δ += 2

# ───────────────────────────────────────────────────────────────────────────────────
n2_alt_γ:                           # γ of ALT — ZERO INSTRUCTIONS.
                                        # Depth differs by path (arm1: CHOICE below;
                                        # arm2: consumed) and no downstream code cares.

# ═══════════════════════════════════════════════════════════════════════════════════
# BOX: IR_MATCH_SEQUENCE                     [implicit — SEQ IS NOTHING]
# A sequence has no state, no behavior, and no instructions.
# γ-chain = emission order (fallthrough).
# ω = the stack's business (Ch.18 step 6) — not the sequence's.
# The blank line below IS the sequence box.
# ═══════════════════════════════════════════════════════════════════════════════════

# ═══════════════════════════════════════════════════════════════════════════════════
# BOX: IR_MATCH_LIT  'llo'                   [n4_lit_llo]
# Ports:  α = n4_litllo_α (fall-through from SEQ = fall-through from n2_alt_γ)
#         β  — none (transient)
#         γ  → n0_head_γ
#         ω → blob_ω
# Locals: K=0
# ═══════════════════════════════════════════════════════════════════════════════════
n4_litllo_α:                           # α
    mov     eax, r14d
    add     eax, 3
    cmp     eax, r15d                   #  δ+3 ≤ Δ ?
    ja      blob_ω                    #  ω
    mov     ecx, r14d
    cmp     byte ptr [r13 + rcx],     108  #  'l'
    jne     blob_ω                    #  ω
    cmp     byte ptr [r13 + rcx + 1], 108  #  'l'
    jne     blob_ω                    #  ω
    cmp     byte ptr [r13 + rcx + 2], 111  #  'o'
    jne     blob_ω                    #  ω
    add     r14d, 3                     #  δ += 3
    jmp     n0_head_γ                     #  γ → whole-match success

# ═══════════════════════════════════════════════════════════════════════════════════
# UNIVERSAL FAILURE STUB — blob_ω
# The ONE ω target for every box in the blob.  Six instructions.  Correct from
# arbitrary depth.  Replaces: op_flat_disp, fc_geom, scanbase, parked anchors,
# rbp floors, and the entire ω-edge topology.
#
# On entry: rsp anywhere ≤ rbp (may be arbitrarily deep inside a carve).
# On exit:  control transfers to [CP+0] with:
#             rsp  = caller's locals base (post-pop depth)
#             r14d = the cell's saved δ  (high32 carries TAG — first 32-bit op clears)
#             rbp  = prev_CP  (chain unlinked)
# ═══════════════════════════════════════════════════════════════════════════════════
blob_ω:
    mov     rsp, rbp                    #  cut: rsp ← CP  (abandon suffix, free-undo marks)
    pop     rax                         #  resume address  [CP+0]
    pop     r14                         #  saved δ (+ TAG in high32, harmless)  [CP+8]
    pop     rbp                         #  prev_CP  [CP+16]
    add     rsp, 8                      #  discard MH slot  [CP+24]  (pre-CELL-5)
    jmp     rax                         #  → resume point

# ═══════════════════════════════════════════════════════════════════════════════════
# BASE CELL RESUME — n0_head_β
# SPITBOL manual Ch.18 step 6, &ANCHOR=0 arm: advance start cursor, bound-check,
# re-enter.  (&ANCHOR≠0 path: jmp n0_head_ω — one runtime cmp/je, omitted here.)
#
# On entry (from blob_ω): rsp at pre-BASE depth; r14d = old start δ (TAG bits
# in high32 cleared by the inc below); rbp = prev_CP (caller's frame).
# ═══════════════════════════════════════════════════════════════════════════════════
n0_head_β:
    cmp     r14d, r15d                  #  δ ≥ Δ ?  (subject exhausted)
    jae     n0_head_ω                #  ω — whole scan failed
    inc     r14d                        #  start ← start + 1  (also zeros high32 TAG bits)
    jmp     n0_head_α_repush                     #  re-push BASE at new start; rsp already correct

# ═══════════════════════════════════════════════════════════════════════════════════
# SCAN BRACKET — n0_head_γ  (whole-match γ)
# Walk CP chain to TAG==BASE, cut there, consume the cell.
# Winning start δ pops out of the BASE cell itself — no separate head-slot write-back.
# (CELL-5+: commit-walk over mark chain happens here before the cut.)
# ═══════════════════════════════════════════════════════════════════════════════════
n0_head_γ:
n0_head_γ_seek:
    cmp     dword ptr [rbp + 12], 1     #  TAG == BASE ?
    je      n0_head_γ_cut
    mov     rbp, qword ptr [rbp + 16]   #  chain prev  (pending choices die in the cut)
    jmp     n0_head_γ_seek
n0_head_γ_cut:
    mov     rsp, rbp                    #  cut to BASE
    pop     rax                         #  consume: resume  (discard)
    pop     rcx                         #  consume: saved δ = winning start (low32=ecx)
    pop     rbp                         #  consume: restore caller rbp  (bracket exit)
    add     rsp, 8                      #  consume: MH slot
    movzx   ecx, cl                     #  isolate start digit  (0-based; ≤9 for demo)
    add     cl,  48                     #  → ASCII
    mov     byte ptr [rip + msg_s], cl
    mov     eax, r14d
    add     al,  48
    mov     byte ptr [rip + msg_e], al
    mov     eax, 1                      #  sys_write
    mov     edi, 1
    lea     rsi, [rip + msg]
    mov     edx, OFFSET msg_len
    syscall
    xor     edi, edi                    #  exit 0
    mov     eax, 60                     #  sys_exit (γ epilogue, own copy — no shared label)
    syscall

# ═══════════════════════════════════════════════════════════════════════════════════
# SCAN BRACKET — n0_head_ω  (whole-scan ω)
# ═══════════════════════════════════════════════════════════════════════════════════
n0_head_ω:
    mov     eax, 1
    mov     edi, 1
    lea     rsi, [rip + nomsg]
    mov     edx, OFFSET nomsg_len
    syscall
    mov     edi, 1                      #  exit 1
    mov     eax, 60                     #  sys_exit (ω epilogue)
    syscall

# ═══════════════════════════════════════════════════════════════════════════════════
    .data
subj:     .ascii  "abhello"
msg:      .ascii  "CELL MATCH start="
msg_s:    .byte   63
          .ascii  " end="
msg_e:    .byte   63
          .byte   10
    .set  msg_len,   . - msg
nomsg:    .ascii  "CELL NOMATCH\n"
    .set  nomsg_len, . - nomsg
