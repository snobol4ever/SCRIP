# seed/test_sno_cell_3.s — ζ-CELL MACHINE reference embodiment #3   (Lon + Claude, 2026-07-28)
# Assembles: gcc -nostdlib -no-pie test_sno_cell_3.s -o t3 && ./t3
# Subject "BlueGoldBirdFish", pattern:  POS(0) ARBNO('Bird' | 'Blue' | LEN(1)) RPOS(0)
# Oracle:   sbl -b  →  text=BlueGoldBirdFish  len=16       (verified 2026-07-28)
# Expected: CELL3 MATCH start=0 end=16 text=BlueGoldBirdFish
#
# ═══════════════════════════════════════════════════════════════════════════════════
# WHY THIS SEED EXISTS — cell_2's STATIC OPERAND OFFSETS DO NOT SURVIVE ARBNO
# ═══════════════════════════════════════════════════════════════════════════════════
# cell_2 established: one UNIT per box, never freed on a success path ⇒ sibling k of
# an n-ary operator sits at the static offset [rsp + 32k].  Uniformity bought
# contiguity.  That holds for every FIXED-HEIGHT box — LIT, LEN, POS, RPOS, ALT.
#
# ARBNO is not fixed-height.  ARBNO(P) ≡ Q = '' | P Q (manual p.121, shy: matches
# null first, extends only when backtracked into).  Each extension pushes ANOTHER
# arbno unit AND another unit for P, and every one of them must SURVIVE — they carry
# the choice records that let the scanner give instances back.  So the ARBNO's
# contribution between its left sibling and its right sibling is
#
#         48 · (1 + 2·instances)      instances known only at run time
#
# ⛔ Therefore ANY operator whose operand list spans an ARB / ARBNO / BAL cannot
# address its operands by a compile-time displacement.  cell_2's [rsp+32k] is not
# merely inconvenient here — it reads the wrong cells.  THIS FILE PROVES THAT AT
# RUNTIME: the operator reads the static-predicted slot as well as the correct one
# and prints both.  They disagree.
#
# ═══════════════════════════════════════════════════════════════════════════════════
# THE FIX — SIBLING CHAIN.  Delivery stays LIFO; ADDRESSING becomes a link.
# ═══════════════════════════════════════════════════════════════════════════════════
# One new field and one new register.  No graph geometry pass, no prefix sums, no
# op_flat_disp — the link is written by the producer itself with information it
# already holds, so nothing is "known" across boxes (the PEERS discipline holds).
#
#   rbx = SIB — base of the most recently COMPLETED unit at the current level.
#
#   every box α :  mov [rsp+U_PREVSIB], rbx     # my left sibling, whoever it was
#   every box γ :  mov rbx, rsp                 # I am now the completed one
#   fail stub   :  mov rbx, [rbp+U_PREVSIB]     # restored with δ, by the same cut
#   parent α    :  (children clobber rbx freely; the parent's own γ re-establishes it,
#                   and the parent's left sibling was saved in its own unit at α)
#
#   ⇒ an n-ary operator walks n−1 links:
#         op_n = rbx ;  op_{n-1} = [op_n + U_PREVSIB] ;  …
#      Exact under arbitrary dynamic height, and exact after backtracking, because
#      the cut restores SIB from the very cell it restores δ from — control, depth
#      AND operand context all arrive together.  That is the design's own thesis
#      ("control transfer and depth transfer are THE SAME ACT") extended to the
#      one plane it had not yet covered: the operand plane.
#
#   COST: 8 bytes per unit, one store per α, one mov per γ, one mov in the stub,
#   one load per operand beyond the first.  The static form of cell_2 remains a
#   valid FAST PATH and the compiler can pick it per operator by a trivially static
#   test — "does my operand list contain a variable-height producer?"  Fixed-height
#   operators keep [rsp+48k]; only ARB/ARBNO/BAL spans pay for the walk.
#
# ═══════════════════════════════════════════════════════════════════════════════════
# UNIT — 48 bytes (16-aligned, C-call invariant preserved)
#     [U+ 0]  start    u32   result: matched start cursor
#     [U+ 4]  len      u32   result: matched length
#     [U+ 8]  saved_δ  u32   cursor on entry — restored on every retry
#     [U+12]  TAG      u32   0=PLAIN 1=BASE 2=CHOICE-pending
#     [U+16]  resume   u64   next-alternative address
#     [U+24]  prev_CP  u64   choice chain
#     [U+32]  prev_sib u64   sibling chain          ← NEW in this seed
#     [U+40]  aux      u64   ARBNO: the OUTERMOST instance's start cursor
#
# REGISTERS
#   r13 Σ · r14 δ · r15 Δ · rbp CP · rbx SIB · rsp ζ frontier · r10 arbno start
#   rax rcx rdx rsi rdi r8 r9 scratch  (rcx/r11 also clobbered by syscall)
#
# ═══════════════════════════════════════════════════════════════════════════════════
# ARBNO, right-recursive shy form, exactly as the manual states it
#   α        carve unit; aux ← outermost start; push CHOICE{resume=extend}; then
#            SUCCEED ON NULL immediately (shy) with value [aux, δ) and go right.
#   extend   (arrived only by the cut) run ONE instance of P.
#   after_P  EPSILON GUARD — if that instance matched null, stop extending (fail on).
#            SPITBOL refuses to extend on a null instance; one compare against the
#            unit does what the 310-line template's counter used to.
#            else recurse into α, inheriting aux ⇒ the new unit's value spans
#            [outermost start, current δ) and it is the one the right sibling sees.
#   Instance count = live units.  No counter, no .bss depth arena, no quads.
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
    .set U_PREVSIB,32
    .set U_AUX,    40
    .set U_SIZE,   48

    .set TAG_PLAIN,  0
    .set TAG_BASE,   1
    .set TAG_CHOICE, 2

# u32 → 5-char right-aligned decimal, written into a static field.  Inline (no call:
# a call would push a return address onto the ζ stack and corrupt the cell layout).
# Clobbers eax ecx edx esi rdi.
    .macro PUTDEC src, field
    mov     eax, \src
    lea     rdi, [rip + \field + 5]
    mov     ecx, 5
1:  dec     rdi
    xor     edx, edx
    mov     esi, 10
    div     esi
    add     dl, 48
    mov     [rdi], dl
    dec     ecx
    test    eax, eax
    jnz     1b
    test    ecx, ecx
    jz      3f
2:  dec     rdi
    mov     byte ptr [rdi], 32
    dec     ecx
    jnz     2b
3:
    .endm

    .macro WRITE_N buf, n
    mov     eax, 1
    mov     edi, 1
    lea     rsi, [rip + \buf]
    mov     edx, \n
    syscall
    .endm

    .macro WRITE buf, len
    mov     eax, 1
    mov     edi, 1
    lea     rsi, [rip + \buf]
    mov     edx, OFFSET \len
    syscall
    .endm

# ═══════════════════════════════════════════════════════════════════════════════════
# SCAN BRACKET — HEAD
# ═══════════════════════════════════════════════════════════════════════════════════
_start:
n0_head_α:
    lea     r13, [rip + subj]
    xor     r14d, r14d                  # δ ← 0
    mov     r15d, 16                    # Δ ← 16  ("BlueGoldBirdFish")
    xor     ebp, ebp                    # CP ← 0
    xor     ebx, ebx                    # SIB ← 0
    sub     rsp, U_SIZE                 # carve U0

n0_head_repush:                         # unanchored retry re-inits in place
    mov     [rsp + U_DELTA], r14d
    mov     dword ptr [rsp + U_TAG], TAG_BASE
    lea     rax, [rip + n0_head_β]
    mov     [rsp + U_RESUME], rax
    mov     [rsp + U_PREV], rbp
    mov     [rsp + U_PREVSIB], rbx
    mov     rbp, rsp                    # CP ← U0
    xor     ebx, ebx                    # fresh sibling level inside the bracket

# ═══════════════════════════════════════════════════════════════════════════════════
# OPERAND 1 — POS(0)                                                     [n1]
# ═══════════════════════════════════════════════════════════════════════════════════
n1_pos0_α:
    sub     rsp, U_SIZE
    mov     [rsp + U_PREVSIB], rbx      # ← sibling link, written by the producer
    mov     [rsp + U_DELTA], r14d
    mov     dword ptr [rsp + U_TAG], TAG_PLAIN
    mov     qword ptr [rsp + U_RESUME], 0
    mov     qword ptr [rsp + U_PREV], 0
    test    r14d, r14d                  # POS(0): cursor must be 0
    jnz     blob_ω
    mov     [rsp + U_START], r14d
    mov     dword ptr [rsp + U_LEN], 0
    mov     rbx, rsp                    # γ: I am the completed sibling

# ═══════════════════════════════════════════════════════════════════════════════════
# OPERAND 2 — ARBNO( 'Bird' | 'Blue' | LEN(1) )                          [n2]
# ═══════════════════════════════════════════════════════════════════════════════════
n2_arbno_α:
    mov     r10d, r14d                  # outermost instance start
    mov     r11, rbx                    # the ARBNO's OUTER left sibling (POS(0))
n2_arbno_carve:                         # recursion re-entry: r10 = start, r11 = outer sib
    sub     rsp, U_SIZE
    mov     [rsp + U_PREVSIB], r11      # ← inherited, NOT rbx: the instances are a
                                        #   NESTED level and must not join the outer
                                        #   operand chain.  This is the one subtlety
                                        #   the sibling chain has: a variable-arity
                                        #   producer republishes its own left link
                                        #   into every recursion so the right sibling
                                        #   sees ONE operand, not `instances` of them.
    mov     [rsp + U_AUX], r10
    mov     [rsp + U_DELTA], r14d
    mov     dword ptr [rsp + U_TAG], TAG_CHOICE
    lea     rax, [rip + n2_arbno_extend]
    mov     [rsp + U_RESUME], rax
    mov     [rsp + U_PREV], rbp
    mov     rbp, rsp                    # CP ← this arbno unit
                                        # SHY: succeed on null, value = [aux, δ)
    mov     [rsp + U_START], r10d
    mov     eax, r14d
    sub     eax, r10d
    mov     [rsp + U_LEN], eax
    mov     rbx, rsp                    # γ
    jmp     n4_rpos0_α

n2_arbno_extend:                        # arrived by the cut: rsp = this unit's base
    jmp     n3_alt_α                    # run ONE instance of P

n2_arbno_after_P:                       # rsp = P's unit base; arbno unit at [rsp+48]
    mov     eax, [rsp + U_LEN]          # EPSILON GUARD
    test    eax, eax
    jz      blob_ω                      # null instance ⇒ stop extending
    mov     r10, [rsp + U_SIZE + U_AUX]      # inherit outermost start
    mov     r11, [rsp + U_SIZE + U_PREVSIB]  # inherit the outer left sibling
    jmp     n2_arbno_carve              # recurse: Q = P Q

# ═══════════════════════════════════════════════════════════════════════════════════
# ARBNO's BODY — ALTERNATE  'Bird' | 'Blue' | LEN(1)                     [n3]
# ═══════════════════════════════════════════════════════════════════════════════════
n3_alt_α:
    sub     rsp, U_SIZE
    mov     [rsp + U_PREVSIB], rbx
    mov     [rsp + U_DELTA], r14d
    mov     dword ptr [rsp + U_TAG], TAG_CHOICE
    lea     rax, [rip + n3_arm2]
    mov     [rsp + U_RESUME], rax
    mov     [rsp + U_PREV], rbp
    mov     rbp, rsp

n3_arm1:                                # 'Bird'
    mov     eax, r14d
    add     eax, 4
    cmp     eax, r15d
    ja      blob_ω
    mov     ecx, r14d
    cmp     byte ptr [r13 + rcx],     66
    jne     blob_ω
    cmp     byte ptr [r13 + rcx + 1], 105
    jne     blob_ω
    cmp     byte ptr [r13 + rcx + 2], 114
    jne     blob_ω
    cmp     byte ptr [r13 + rcx + 3], 100
    jne     blob_ω
    add     r14d, 4
    mov     ecx, 4
    jmp     n3_γ

n3_arm2:                                # 'Blue' — relink, one arm still untried
    lea     rax, [rip + n3_arm3]
    mov     [rsp + U_RESUME], rax
    mov     [rsp + U_PREV], rbp
    mov     rbp, rsp
    mov     eax, r14d
    add     eax, 4
    cmp     eax, r15d
    ja      blob_ω
    mov     ecx, r14d
    cmp     byte ptr [r13 + rcx],     66
    jne     blob_ω
    cmp     byte ptr [r13 + rcx + 1], 108
    jne     blob_ω
    cmp     byte ptr [r13 + rcx + 2], 117
    jne     blob_ω
    cmp     byte ptr [r13 + rcx + 3], 101
    jne     blob_ω
    add     r14d, 4
    mov     ecx, 4
    jmp     n3_γ

n3_arm3:                                # LEN(1) — LAST arm, no relink
    mov     eax, r14d
    inc     eax
    cmp     eax, r15d
    ja      blob_ω
    inc     r14d
    mov     ecx, 1

n3_γ:
    mov     eax, [rsp + U_DELTA]
    mov     [rsp + U_START], eax
    mov     [rsp + U_LEN], ecx
    mov     rbx, rsp
    jmp     n2_arbno_after_P

# ═══════════════════════════════════════════════════════════════════════════════════
# OPERAND 3 — RPOS(0)                                                    [n4]
# ═══════════════════════════════════════════════════════════════════════════════════
n4_rpos0_α:
    sub     rsp, U_SIZE
    mov     [rsp + U_PREVSIB], rbx
    mov     [rsp + U_DELTA], r14d
    mov     dword ptr [rsp + U_TAG], TAG_PLAIN
    mov     qword ptr [rsp + U_RESUME], 0
    mov     qword ptr [rsp + U_PREV], 0
    cmp     r14d, r15d                  # RPOS(0): cursor must be at the right end
    jne     blob_ω
    mov     [rsp + U_START], r14d
    mov     dword ptr [rsp + U_LEN], 0
    mov     rbx, rsp

# ═══════════════════════════════════════════════════════════════════════════════════
# OPERATOR — 3-ary CONCAT over POS0 · ARBNO · RPOS0                      [n5]
# Emitted AFTER all three operands (postfix).  All three units are live on the ζ
# stack at this instant — that is the guarantee.  They are NOT at static offsets,
# because operand 2 is variable-height; the sibling chain finds them exactly.
# ═══════════════════════════════════════════════════════════════════════════════════
n5_cat3_α:
    mov     r8,  rbx                    # op3 = RPOS0   (SIB, the last completed)
    mov     r9,  [r8 + U_PREVSIB]       # op2 = ARBNO
    mov     r10, [r9 + U_PREVSIB]       # op1 = POS0

    # ── evidence ① : the three operand units, by chain, with their live depths ──
    mov     rax, r10
    sub     rax, rsp
    PUTDEC  eax, e1_off
    mov     eax, [r10 + U_START]
    PUTDEC  eax, e1_s
    mov     eax, [r10 + U_LEN]
    PUTDEC  eax, e1_l
    WRITE   e1, e1_len

    mov     rax, r9
    sub     rax, rsp
    PUTDEC  eax, e2_off
    mov     eax, [r9 + U_START]
    PUTDEC  eax, e2_s
    mov     eax, [r9 + U_LEN]
    PUTDEC  eax, e2_l
    WRITE   e2, e2_len

    mov     rax, r8
    sub     rax, rsp
    PUTDEC  eax, e3_off
    mov     eax, [r8 + U_START]
    PUTDEC  eax, e3_s
    mov     eax, [r8 + U_LEN]
    PUTDEC  eax, e3_l
    WRITE   e3, e3_len

    # ── evidence ② : what cell_2's STATIC rule would have read for op1 ──────────
    # cell_2 regime says: op3 at [rsp+0], op2 at [rsp+U_SIZE], op1 at [rsp+2·U_SIZE].
    mov     eax, [rsp + 2*U_SIZE + U_START]
    PUTDEC  eax, f1_s
    mov     eax, [rsp + 2*U_SIZE + U_LEN]
    PUTDEC  eax, f1_l
    WRITE   f1, f1_len

    # ── the operator's actual work ─────────────────────────────────────────────
    mov     eax, [r10 + U_START]        # start ← op1.start
    mov     ecx, [r10 + U_LEN]
    add     ecx, [r9 + U_LEN]           # + op2.len
    add     ecx, [r8 + U_LEN]           # + op3.len
    sub     rsp, U_SIZE
    mov     [rsp + U_PREVSIB], rbx
    mov     [rsp + U_START], eax
    mov     [rsp + U_LEN], ecx
    mov     [rsp + U_DELTA], r14d
    mov     dword ptr [rsp + U_TAG], TAG_PLAIN
    mov     qword ptr [rsp + U_RESUME], 0
    mov     qword ptr [rsp + U_PREV], 0
    mov     rbx, rsp
    jmp     n0_head_γ

# ═══════════════════════════════════════════════════════════════════════════════════
# UNIVERSAL FAILURE STUB — cuts to the unit, restores δ AND the operand context
# ═══════════════════════════════════════════════════════════════════════════════════
blob_ω:
    mov     rsp, rbp                    # cut — abandoned suffix dies, this unit lives
    mov     r14d, [rbp + U_DELTA]       # δ
    mov     rbx,  [rbp + U_PREVSIB]     # SIB — operand context arrives with control
    mov     rax,  [rbp + U_RESUME]
    mov     rbp,  [rbp + U_PREV]
    jmp     rax

# ═══════════════════════════════════════════════════════════════════════════════════
n0_head_β:                              # unanchored advance (Ch.18 step 6)
    cmp     r14d, r15d
    jae     n0_head_ω
    inc     r14d
    jmp     n0_head_repush

# ═══════════════════════════════════════════════════════════════════════════════════
n0_head_γ:
    mov     r8d, [rsp + U_START]
    mov     r9d, [rsp + U_LEN]
n0_head_γ_seek:
    cmp     dword ptr [rbp + U_TAG], TAG_BASE
    je      n0_head_γ_cut
    mov     rbp, [rbp + U_PREV]
    jmp     n0_head_γ_seek
n0_head_γ_cut:
    mov     rsp, rbp
    mov     rbp, [rbp + U_PREV]

    PUTDEC  r8d, msg_s
    mov     eax, r8d
    add     eax, r9d
    PUTDEC  eax, msg_e
    WRITE   msg, msg_len
    mov     eax, 1
    mov     edi, 1
    lea     rsi, [r13 + r8]
    mov     edx, r9d
    syscall
    WRITE_N  nl, 1
    xor     edi, edi
    mov     eax, 60
    syscall

# ═══════════════════════════════════════════════════════════════════════════════════
n0_head_ω:
    WRITE   nomsg, nomsg_len
    mov     edi, 1
    mov     eax, 60
    syscall

# ═══════════════════════════════════════════════════════════════════════════════════
    .data
subj:   .ascii  "BlueGoldBirdFish"

e1:     .ascii  "  [chain] op1 POS(0)  unit @rsp+"
e1_off: .ascii  "?????"
        .ascii  "  {start="
e1_s:   .ascii  "?????"
        .ascii  " len="
e1_l:   .ascii  "?????"
        .ascii  "}\n"
    .set e1_len, . - e1

e2:     .ascii  "  [chain] op2 ARBNO   unit @rsp+"
e2_off: .ascii  "?????"
        .ascii  "  {start="
e2_s:   .ascii  "?????"
        .ascii  " len="
e2_l:   .ascii  "?????"
        .ascii  "}\n"
    .set e2_len, . - e2

e3:     .ascii  "  [chain] op3 RPOS(0) unit @rsp+"
e3_off: .ascii  "?????"
        .ascii  "  {start="
e3_s:   .ascii  "?????"
        .ascii  " len="
e3_l:   .ascii  "?????"
        .ascii  "}\n"
    .set e3_len, . - e3

f1:     .ascii  "  [static] cell_2 rule [rsp+96] for op1 reads {start="
f1_s:   .ascii  "?????"
        .ascii  " len="
f1_l:   .ascii  "?????"
        .ascii  "}  <-- WRONG CELL\n"
    .set f1_len, . - f1

msg:    .ascii  "CELL3 MATCH start="
msg_s:  .ascii  "?????"
        .ascii  " end="
msg_e:  .ascii  "?????"
        .ascii  " text="
    .set msg_len, . - msg
nl:     .byte   10
nomsg:  .ascii  "CELL3 NOMATCH\n"
    .set nomsg_len, . - nomsg
