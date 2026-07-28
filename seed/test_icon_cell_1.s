# seed/test_icon_cell_1.s — ζ-CELL MACHINE, Icon flavor #1   (Lon + Claude, 2026-07-28)
# Assembles: gcc -nostdlib -no-pie test_icon_cell_1.s -o ti && ./ti
# Program:   every write(5 > ((1 to 2) * (3 to 4)))        == seed/test_icon.c
# Reference: compiled test_icon.c emits values 3, 4 then overall failure (its
#            Success!/Failure. lines are harness decoration; the VALUE SEQUENCE is
#            the semantics).  Expected here:  3 / 4 / ICON1 EVERY EXHAUSTED
#
# ═══════════════════════════════════════════════════════════════════════════════════
# WHAT THIS PROVES
#  1. AN ICON GENERATOR IS A RETAIN-FLAVOR BOX, nothing more.  TO's α carves a unit
#     that is simultaneously its value slot, its choice record, and its loop state
#     (limit in U_AUX).  γ retains.  β (=step) is reached ONLY by the cut, re-arms
#     the SAME unit in place, delivers the next value at the SAME TOS.  suspend =
#     retain + γ; resume = the cut.  test_icon.c's 4-port goto lattice (x_start/
#     succeed/resume/fail per node) collapses into the one universal stub.
#  2. GENERATOR RE-INITIALIZATION IS FREE.  When (1 to 2) resumes, the cut has
#     ALREADY reclaimed the dead (3 to 4) unit and the dead product above it; the
#     outer's step jmps the inner's α, which carves FRESH at exactly that spot.  The
#     class JCON-selfhost fights as SEQ-COEXPR-REINIT is a non-problem: reclamation
#     and re-entry are the same cut.
#  3. EVERY IS A BASE CELL + ONE JMP.  Driving = jmp blob_ω after the body.
#     Exhaustion of everything lands at the EVERY cell's resume = done.
#  4. ⭐ THE REPUBLISH RULE (new law, discovered here; retrofits cell_4/5's operators).
#     An operator that CONSUMES its operands must not leave them on the sibling
#     chain: its result unit links to op1's OWN prev-sib, i.e.
#           result.prev_sib = leftmost_operand.prev_sib
#     Without it, `5 > (A*B)` finds B as its left operand instead of 5 (A,B are
#     consumed by *, but chain-visible).  With it, consumed subtrees vanish from the
#     chain exactly as they vanish from the FORTH picture ( op1 op2 — result ).
#     Variable-arity boxes (ARBNO, cell_3) already republish for the same reason —
#     this generalizes their special case into the operator law.
#
# UNIT — 48B, fields as cell_3/4/5.  U_START = the VALUE (ints here, not spans).
#        U_AUX = TO's inclusive limit.  No subject: r13/r14/r15 unused this seed.
# REGISTERS  rbp CP · rbx SIB · rsp ζ · rax rcx rdx rsi rdi r8..r11 scratch
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

# ═══════════════════════════════════════════════════════════════════════════════════
# EVERY bracket — a BASE cell whose resume is the loop-done continuation
# ═══════════════════════════════════════════════════════════════════════════════════
_start:
n0_every_α:
    xor     ebp, ebp                    # CP
    xor     ebx, ebx                    # SIB
    sub     rsp, U_SIZE
    mov     dword ptr [rsp + U_TAG], TAG_BASE
    lea     rax, [rip + n0_every_done]
    mov     [rsp + U_RESUME], rax
    mov     [rsp + U_PREV], rbp
    mov     [rsp + U_PREVSIB], rbx
    mov     rbp, rsp
    xor     ebx, ebx

# ═══════════════════════════════════════════════════════════════════════════════════
# op1 of `>` — the literal 5.  A singleton: plain unit, no choice.  Its "resume"
# is nonexistence — failure simply cuts past it.
# ═══════════════════════════════════════════════════════════════════════════════════
n1_lit5_α:
    sub     rsp, U_SIZE
    mov     [rsp + U_PREVSIB], rbx
    mov     dword ptr [rsp + U_TAG], TAG_PLAIN
    mov     dword ptr [rsp + U_START], 5
    mov     rbx, rsp

# ═══════════════════════════════════════════════════════════════════════════════════
# GENERATOR — (1 to 2)          RETAIN box; β = step, reached only by the cut
# continuation after each delivery = (3 to 4)'s α
# ═══════════════════════════════════════════════════════════════════════════════════
nA_to12_α:
    sub     rsp, U_SIZE
    mov     [rsp + U_PREVSIB], rbx      # left sib = lit 5
    mov     dword ptr [rsp + U_TAG], TAG_CHOICE
    mov     dword ptr [rsp + U_START], 1        # value ← lo
    mov     dword ptr [rsp + U_AUX],   2        # limit
    lea     rax, [rip + nA_to12_step]
    mov     [rsp + U_RESUME], rax
    mov     [rsp + U_PREV], rbp
    mov     rbp, rsp                    # CP ← me (I am resumable)
    mov     rbx, rsp                    # γ: deliver at TOS, RETAIN
    jmp     nB_to34_α

nA_to12_step:                           # β — the cut landed rsp HERE, unlinked CP
    mov     eax, [rsp + U_START]
    inc     eax
    cmp     eax, [rsp + U_AUX]
    jg      blob_ω                      # exhausted → fail onward (cut to EVERY)
    mov     [rsp + U_START], eax        # re-arm the SAME unit in place
    mov     [rsp + U_PREV], rbp
    mov     rbp, rsp
    mov     rbx, rsp
    jmp     nB_to34_α                   # re-run the right operand FROM α:
                                        # fresh carve on the just-reclaimed ground —
                                        # generator re-init by construction

# ═══════════════════════════════════════════════════════════════════════════════════
# GENERATOR — (3 to 4)          same shape; continuation = the multiply
# ═══════════════════════════════════════════════════════════════════════════════════
nB_to34_α:
    sub     rsp, U_SIZE
    mov     [rsp + U_PREVSIB], rbx      # left sib = (1 to 2)'s unit
    mov     dword ptr [rsp + U_TAG], TAG_CHOICE
    mov     dword ptr [rsp + U_START], 3
    mov     dword ptr [rsp + U_AUX],   4
    lea     rax, [rip + nB_to34_step]
    mov     [rsp + U_RESUME], rax
    mov     [rsp + U_PREV], rbp
    mov     rbp, rsp
    mov     rbx, rsp
    jmp     nC_mul_α

nB_to34_step:
    mov     eax, [rsp + U_START]
    inc     eax
    cmp     eax, [rsp + U_AUX]
    jg      blob_ω                      # exhausted → cut lands at (1 to 2)'s step
    mov     [rsp + U_START], eax
    mov     [rsp + U_PREV], rbp
    mov     rbp, rsp
    mov     rbx, rsp
    jmp     nC_mul_α

# ═══════════════════════════════════════════════════════════════════════════════════
# OPERATOR — A * B     transient; REPUBLISHES per the new law
# ═══════════════════════════════════════════════════════════════════════════════════
nC_mul_α:
    mov     r9,  rbx                    # op2 = B
    mov     r10, [r9 + U_PREVSIB]       # op1 = A
    mov     eax, [r10 + U_START]
    imul    eax, [r9 + U_START]
    mov     r11, [r10 + U_PREVSIB]      # ⭐ republish: op1's own prev-sib (= lit 5)
    sub     rsp, U_SIZE
    mov     [rsp + U_PREVSIB], r11      #    consumed A,B leave the chain
    mov     [rsp + U_START], eax
    mov     dword ptr [rsp + U_TAG], TAG_PLAIN
    mov     rbx, rsp

# ═══════════════════════════════════════════════════════════════════════════════════
# OPERATOR — 5 > x     Icon comparison: succeeds producing the RIGHT operand
# ═══════════════════════════════════════════════════════════════════════════════════
nD_gt_α:
    mov     r9,  rbx                    # op2 = product
    mov     r10, [r9 + U_PREVSIB]       # op1 = lit 5   (correct BECAUSE mul republished)
    mov     eax, [r10 + U_START]
    cmp     eax, [r9 + U_START]
    jle     blob_ω                      # comparison fails → resume newest generator
    mov     ecx, [r9 + U_START]
    mov     r11, [r10 + U_PREVSIB]      # republish past 5
    sub     rsp, U_SIZE
    mov     [rsp + U_PREVSIB], r11
    mov     [rsp + U_START], ecx
    mov     dword ptr [rsp + U_TAG], TAG_PLAIN
    mov     rbx, rsp

# ═══════════════════════════════════════════════════════════════════════════════════
# write(x) — side effect at γ, then EVERY DRIVES: jmp blob_ω
# ═══════════════════════════════════════════════════════════════════════════════════
nE_write_α:
    mov     eax, [rbx + U_START]
    PUTDEC  eax, wv
    mov     eax, 1
    mov     edi, 1
    lea     rsi, [rip + wmsg]
    mov     edx, OFFSET wmsg_len
    syscall
    jmp     blob_ω                      # ← `every`.  That is the entire loop.

# ═══════════════════════════════════════════════════════════════════════════════════
# UNIVERSAL FAILURE STUB
# ═══════════════════════════════════════════════════════════════════════════════════
blob_ω:
    mov     rsp, rbp
    mov     rbx,  [rbp + U_PREVSIB]
    mov     rax,  [rbp + U_RESUME]
    mov     rbp,  [rbp + U_PREV]
    jmp     rax

# ═══════════════════════════════════════════════════════════════════════════════════
n0_every_done:                          # EVERY cell's resume: all generators spent
    mov     eax, 1
    mov     edi, 1
    lea     rsi, [rip + dmsg]
    mov     edx, OFFSET dmsg_len
    syscall
    xor     edi, edi
    mov     eax, 60
    syscall

# ═══════════════════════════════════════════════════════════════════════════════════
    .data
wmsg:   .ascii  "?????"
    .set wv, wmsg                       # PUTDEC target = the 5-char field itself
        .byte   10
    .set wmsg_len, . - wmsg
dmsg:   .ascii  "ICON1 EVERY EXHAUSTED\n"
    .set dmsg_len, . - dmsg
