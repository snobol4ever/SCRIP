# seed/test_sno_cell_5.s — ζ-CELL MACHINE reference embodiment #5   (Lon + Claude, 2026-07-28)
# Assembles: gcc -nostdlib -no-pie test_sno_cell_5.s -o t5 && ./t5
# Program:   P = 'a' *P | 'b'   ;   'aaab'  ?  POS(0) P RPOS(0)
# Oracle:    sbl → ORACLE aaab   ('aaax' → NOMATCH)                (verified 2026-07-28)
# Expected:  return trace b / ab / aab / aaab, then CELL5 MATCH start=0 end=4 text=aaab
#
# ═══════════════════════════════════════════════════════════════════════════════════
# WHY: this is test_sno_5/6's DDS-1 scope (ONE stored body, MULTIPLE call sites) made
# RECURSIVE — i.e. the exact class the live tree cannot run: the s198 stored-pattern
# SIGSEGV and the s197 recursive-defer wall.  Two call sites here: the statement site
# and the *P site inside the body itself.  One body, two continuations, unbounded
# re-entry.  The manual's own contract (p.122-123): recursion REQUIRES the unevaluated
# operator, and depth is bounded by the machine stack — which is exactly what per-BB
# self-allocation inherits honestly (fresh carve per entry; no .bss arena, no plan).
#
# MECHANISM — the CALL unit + the CL register:
#   r12 = CL, base of the newest ACTIVE call unit (the call chain, twin of CP).
#   A call site carves a CALL-tagged unit: {resume = MY continuation, U_PREV = prev CL,
#   prevsib = my left operand}, sets CL = unit, jmps body α.  The body is ORDINARY
#   boxes — its interior knows nothing about calls.  Body γ (ONE copy, site-blind):
#   copy result into [CL], rbx = CL (the call unit becomes the site's completed
#   operand), CL = [CL].prev, jmp [CL_old].resume.  ω DOES NOT EXIST for the body:
#   exhausting the body's alternatives cuts to whatever choice preceded the call —
#   the call unit is reclaimed by that same cut, CL restored from the choice unit.
#   CROSS-CALL BACKTRACKING COSTS ZERO INSTRUCTIONS AND ZERO KNOWLEDGE.
#
#   The one law that makes it sound: every choice push saves CL in its unit (U_AUX)
#   and blob_ω restores it — so control, depth, cursor, operand context AND call
#   context all arrive together in the one cut.  The disease (control decoupled from
#   context) cannot re-enter through the call plane either.
#   (U_AUX is CL-save in choice cells and outermost-start in ARBNO cells; the two
#   never coexist in one cell today.  Merge rule when they must: unit grows to 64B.)
#
# UNIT — 48B as in cell_3/4:
#   [+0]start [+4]len [+8]saved_δ [+12]TAG(0 plain·1 base·2 choice·3 CALL)
#   [+16]resume [+24]prev(CP for choice · CL for call) [+32]prev_sib [+40]aux
# REGISTERS  r13 Σ · r14 δ · r15 Δ · rbp CP · rbx SIB · r12 CL · rsp ζ
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
    .set TAG_CALL,   3

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

    .macro WRITE buf, len
    mov     eax, 1
    mov     edi, 1
    lea     rsi, [rip + \buf]
    mov     edx, OFFSET \len
    syscall
    .endm

# ═══════════════════════════════════════════════════════════════════════════════════
# SCAN BRACKET
# ═══════════════════════════════════════════════════════════════════════════════════
_start:
n0_head_α:
    lea     r13, [rip + subj]
    xor     r14d, r14d                  # δ
    mov     r15d, 4                     # Δ ("aaab")
    xor     ebp, ebp                    # CP
    xor     ebx, ebx                    # SIB
    xor     r12d, r12d                  # CL
    sub     rsp, U_SIZE                 # U0

n0_head_repush:
    mov     [rsp + U_DELTA], r14d
    mov     dword ptr [rsp + U_TAG], TAG_BASE
    lea     rax, [rip + n0_head_β]
    mov     [rsp + U_RESUME], rax
    mov     [rsp + U_PREV], rbp
    mov     [rsp + U_PREVSIB], rbx
    mov     [rsp + U_AUX], r12          # CL rides the cell — restored by the cut
    mov     rbp, rsp
    xor     ebx, ebx

# ═══════════════════════════════════════════════════════════════════════════════════
# op1 — POS(0)
# ═══════════════════════════════════════════════════════════════════════════════════
n1_pos0_α:
    sub     rsp, U_SIZE
    mov     [rsp + U_PREVSIB], rbx
    mov     [rsp + U_DELTA], r14d
    mov     dword ptr [rsp + U_TAG], TAG_PLAIN
    test    r14d, r14d
    jnz     blob_ω
    mov     [rsp + U_START], r14d
    mov     dword ptr [rsp + U_LEN], 0
    mov     rbx, rsp

# ═══════════════════════════════════════════════════════════════════════════════════
# op2 — CALL SITE 1  (the statement's use of P)
# ═══════════════════════════════════════════════════════════════════════════════════
n2_call1_α:
    sub     rsp, U_SIZE
    mov     [rsp + U_PREVSIB], rbx      # left operand = POS0's unit
    mov     [rsp + U_DELTA], r14d
    mov     dword ptr [rsp + U_TAG], TAG_CALL
    lea     rax, [rip + n2_call1_cont]  # site-1 continuation
    mov     [rsp + U_RESUME], rax
    mov     [rsp + U_PREV], r12         # prev CL
    mov     r12, rsp                    # CL ← this call
    jmp     P_body_α

n2_call1_cont:                          # rbx = call unit, holding P's whole result
                                        # fall through to op3

# ═══════════════════════════════════════════════════════════════════════════════════
# op3 — RPOS(0)
# ═══════════════════════════════════════════════════════════════════════════════════
n3_rpos0_α:
    sub     rsp, U_SIZE
    mov     [rsp + U_PREVSIB], rbx
    mov     [rsp + U_DELTA], r14d
    mov     dword ptr [rsp + U_TAG], TAG_PLAIN
    cmp     r14d, r15d
    jne     blob_ω                      # ← the cut from HERE unwinds INTO the body's
                                        #   pending alternation, through both call
                                        #   frames, with zero call-aware code
    mov     [rsp + U_START], r14d
    mov     dword ptr [rsp + U_LEN], 0
    mov     rbx, rsp

# ═══════════════════════════════════════════════════════════════════════════════════
# operator — 3-ary concat by sibling chain (call unit is just another operand)
# ═══════════════════════════════════════════════════════════════════════════════════
n4_cat3_α:
    mov     r8,  rbx                    # RPOS0
    mov     r9,  [r8 + U_PREVSIB]       # the CALL unit = P's result
    mov     r10, [r9 + U_PREVSIB]       # POS0
    mov     eax, [r10 + U_START]
    mov     ecx, [r10 + U_LEN]
    add     ecx, [r9 + U_LEN]
    add     ecx, [r8 + U_LEN]
    mov     r11, [r10 + U_PREVSIB]      # REPUBLISH (law from test_icon_cell_1.s):
    sub     rsp, U_SIZE                 # consumed operands leave the sibling chain
    mov     [rsp + U_PREVSIB], r11
    mov     [rsp + U_START], eax
    mov     [rsp + U_LEN], ecx
    mov     [rsp + U_DELTA], r14d
    mov     dword ptr [rsp + U_TAG], TAG_PLAIN
    jmp     n0_head_γ

# ═══════════════════════════════════════════════════════════════════════════════════
# THE STORED BODY — P = 'a' *P | 'b'      (ONE copy; interior is ordinary boxes)
# ═══════════════════════════════════════════════════════════════════════════════════
P_body_α:                               # = the ALT's α
    sub     rsp, U_SIZE
    mov     [rsp + U_PREVSIB], rbx
    mov     [rsp + U_DELTA], r14d
    mov     dword ptr [rsp + U_TAG], TAG_CHOICE
    lea     rax, [rip + P_arm2]
    mov     [rsp + U_RESUME], rax
    mov     [rsp + U_PREV], rbp
    mov     [rsp + U_AUX], r12          # ← CL saved; the cut restores it
    mov     rbp, rsp

P_arm1:                                 # 'a'  then  *P
    mov     eax, r14d
    inc     eax
    cmp     eax, r15d
    ja      blob_ω
    mov     ecx, r14d
    cmp     byte ptr [r13 + rcx], 97    # 'a'
    jne     blob_ω
    inc     r14d
    sub     rsp, U_SIZE                 # lit-'a' unit
    mov     [rsp + U_PREVSIB], rbx
    mov     eax, r14d
    dec     eax
    mov     [rsp + U_START], eax
    mov     dword ptr [rsp + U_LEN], 1
    mov     dword ptr [rsp + U_TAG], TAG_PLAIN
    mov     rbx, rsp

# ── CALL SITE 2 — the *P inside the body: SAME body, DIFFERENT continuation ────────
n5_call2_α:
    sub     rsp, U_SIZE
    mov     [rsp + U_PREVSIB], rbx      # left operand = lit-'a'
    mov     [rsp + U_DELTA], r14d
    mov     dword ptr [rsp + U_TAG], TAG_CALL
    lea     rax, [rip + n5_call2_cont]
    mov     [rsp + U_RESUME], rax
    mov     [rsp + U_PREV], r12
    mov     r12, rsp
    jmp     P_body_α                    # RECURSION = jmp + fresh carve.  Nothing else.

n5_call2_cont:                          # rbx = inner call unit (P's result)
    mov     r9,  rbx                    # concat 'a' + P
    mov     r10, [r9 + U_PREVSIB]
    mov     eax, [r10 + U_START]
    mov     ecx, [r10 + U_LEN]
    add     ecx, [r9 + U_LEN]
    mov     r11, [r10 + U_PREVSIB]      # REPUBLISH (law from test_icon_cell_1.s):
    sub     rsp, U_SIZE                 # consumed operands leave the sibling chain
    mov     [rsp + U_PREVSIB], r11
    mov     [rsp + U_START], eax
    mov     [rsp + U_LEN], ecx
    mov     dword ptr [rsp + U_TAG], TAG_PLAIN
    mov     rbx, rsp
    jmp     P_body_γ

P_arm2:                                 # 'b'   (LAST arm — no relink)
    mov     eax, r14d
    inc     eax
    cmp     eax, r15d
    ja      blob_ω
    mov     ecx, r14d
    cmp     byte ptr [r13 + rcx], 98    # 'b'
    jne     blob_ω
    inc     r14d
    sub     rsp, U_SIZE
    mov     [rsp + U_PREVSIB], rbx
    mov     eax, r14d
    dec     eax
    mov     [rsp + U_START], eax
    mov     dword ptr [rsp + U_LEN], 1
    mov     dword ptr [rsp + U_TAG], TAG_PLAIN
    mov     rbx, rsp
                                        # fall into γ

P_body_γ:                               # ONE copy, site-blind.  rbx = body result.
    mov     eax, [rbx + U_START]        # deliver result INTO the call unit
    mov     [r12 + U_START], eax
    mov     eax, [rbx + U_LEN]
    mov     [r12 + U_LEN], eax
    # trace: print the span this activation returns (evidence ladder b/ab/aab/aaab)
    mov     eax, [r12 + U_START]
    lea     rsi, [r13 + rax]
    mov     edx, [r12 + U_LEN]
    mov     eax, 1
    mov     edi, 1
    syscall
    mov     eax, 1
    mov     edi, 1
    lea     rsi, [rip + nl]
    mov     edx, 1
    syscall
    mov     rbx, r12                    # the call unit IS the site's operand now
    mov     rax, [r12 + U_RESUME]       # site continuation
    mov     r12, [r12 + U_PREV]         # unlink CL
    jmp     rax

# ═══════════════════════════════════════════════════════════════════════════════════
# UNIVERSAL FAILURE — control + depth + cursor + operands + CALL CONTEXT, one cut
# ═══════════════════════════════════════════════════════════════════════════════════
blob_ω:
    mov     rsp, rbp
    mov     r14d, [rbp + U_DELTA]
    mov     rbx,  [rbp + U_PREVSIB]
    mov     r12,  [rbp + U_AUX]         # CL restored — the call plane heals too
    mov     rax,  [rbp + U_RESUME]
    mov     rbp,  [rbp + U_PREV]
    jmp     rax

n0_head_β:
    cmp     r14d, r15d
    jae     n0_head_ω
    inc     r14d
    jmp     n0_head_repush

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
    mov     eax, 1
    mov     edi, 1
    lea     rsi, [rip + nl]
    mov     edx, 1
    syscall
    xor     edi, edi
    mov     eax, 60
    syscall

n0_head_ω:
    WRITE   nomsg, nomsg_len
    mov     edi, 1
    mov     eax, 60
    syscall

# ═══════════════════════════════════════════════════════════════════════════════════
    .data
subj:   .ascii  "aaab"
msg:    .ascii  "CELL5 MATCH start="
msg_s:  .ascii  "?????"
        .ascii  " end="
msg_e:  .ascii  "?????"
        .ascii  " text="
    .set msg_len, . - msg
nl:     .byte   10
nomsg:  .ascii  "CELL5 NOMATCH\n"
    .set nomsg_len, . - nomsg
