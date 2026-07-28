# seed/test_sno_cell_6.s — ζ-CELL MACHINE #6: SEQ/ALT PREFIX ALLOCATION  (Lon 2026-07-28)
# gcc -nostdlib -no-pie test_sno_cell_6.s -o t6 && ./t6
# Program: POS(0) ARBNO('Bird'|'Blue'|LEN(1)) RPOS(0)  on "BlueGoldBirdFish"  (== cell_3)
# Oracle (sbl): text=BlueGoldBirdFish len=16.   Expected: CELL6 MATCH start=0 end=16
#
# LON'S RULING: IR_SEQUENCE and IR_ALTERNATE were about to be deleted as "pure wiring".
# They are the ONLY nodes carrying an ARITY, and arity IS the allocation size.  Keep
# them, make them PREFIX, let them allocate; everything else stays POSTFIX and writes
# into its own slot at a COMPILE-TIME offset.
#   SEQ  arity n → header + n result slots (all live, all consumed)
#   ALT  arity n → 1 result slot + n-1 retry edges (one arm survives)
# RETIRED BY THIS SEED: the sibling chain (cell_3), the REPUBLISH law (cell_4/5), the
# ARBNO nested-link special case, and the fused 48B unit — result slots and choice
# cells were only ever the same object because nobody owned the slots.  Now the parent
# does:  RESULT SLOT = 16B {start,len} (parent-allocated) · CHOICE CELL = 32B (pushed).
#
# LEGITIMACY: a static slot offset is reach-over UNLESS its base arrives with control.
# It does — rbx = E rides every choice cell beside δ and is restored by the same cut.
# EVAL/CODE still safe: this is a per-node arity, not a graph-global geometry pass.
#
#   FRAME        [E+ 0] prev_E   [E+ 8] n     [E+16] slot1 …  slot k at [E+16(k)]
#   CHOICE 32B   [+ 0] resume  [+8] δ|TAG  [+16] prev_CP  [+24] saved_E
#   REGISTERS    r13 Σ · r14 δ · r15 Δ · rbp CP · rbx E · r10 arbno pre-δ

    .intel_syntax noprefix
    .globl _start
    .text
    .set TAG_BASE, 1
    .set TAG_CHOICE, 2
    .set S1, 16                         # slot offsets from E — STATIC, the whole point
    .set S2, 32
    .set S3, 48

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

    .macro PUSHCH resume_lbl                # 32B choice cell; CP ← it
    sub     rsp, 32
    lea     rax, [rip + \resume_lbl]
    mov     [rsp + 0], rax
    mov     [rsp + 8], r14d
    mov     dword ptr [rsp + 12], TAG_CHOICE
    mov     [rsp + 16], rbp
    mov     [rsp + 24], rbx                 # saved E — base arrives with control
    mov     rbp, rsp
    .endm

_start:
n0_head_α:
    lea     r13, [rip + subj]
    xor     r14d, r14d
    mov     r15d, 16
    xor     ebp, ebp
    xor     ebx, ebx
n0_head_repush:
    sub     rsp, 32                         # BASE cell (unanchored retry)
    lea     rax, [rip + n0_head_β]
    mov     [rsp + 0], rax
    mov     [rsp + 8], r14d
    mov     dword ptr [rsp + 12], TAG_BASE
    mov     [rsp + 16], rbp
    mov     [rsp + 24], rbx
    mov     rbp, rsp

# ═══ IR_SEQUENCE, arity 3 — PREFIX.  ALL THREE SLOTS EXIST BEFORE ANY CHILD RUNS ═══
n1_seq_α:
    sub     rsp, 16 + 3*16                  # header + 3 result slots — n is STATIC
    mov     [rsp + 0], rbx                  # prev_E
    mov     qword ptr [rsp + 8], 3          # arity, for the γ fold
    mov     rbx, rsp                        # E ← my frame

# ═══ child 1 — POS(0) — postfix leaf, writes its OWN slot ═══
n2_pos0_α:
    test    r14d, r14d
    jnz     blob_ω
    mov     [rbx + S1 + 0], r14d
    mov     dword ptr [rbx + S1 + 4], 0

# ═══ child 2 — ARBNO — variable arity: lives in ONE slot, instances pile above ═══
n3_arbno_α:
    mov     [rbx + S2 + 0], r14d            # slot2.start ← δ   (accumulator IS the slot;
    mov     dword ptr [rbx + S2 + 4], 0     # slot2.len ← 0      cell_3's U_AUX dies here)
n3_arbno_push:
    PUSHCH  n3_arbno_extend
    jmp     n4_rpos0_α                      # shy: succeed on null, go right

n3_arbno_extend:                            # only reachable by the cut
    mov     r10d, r14d                      # δ before the instance (epsilon guard)
    jmp     n5_alt_α

n3_arbno_fold:                              # instance matched
    cmp     r14d, r10d
    je      blob_ω                          # null instance ⇒ stop extending
    mov     eax, r14d
    sub     eax, [rbx + S2 + 0]
    mov     [rbx + S2 + 4], eax             # slot2.len ← δ − slot2.start
    jmp     n3_arbno_push                   # recurse

# ═══ IR_ALTERNATE, arity 3 — PREFIX, but allocates 1 slot + 2 retry edges ═══
n5_alt_α:
    PUSHCH  n5_arm2
n5_arm1:                                    # 'Bird'
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
    jmp     n3_arbno_fold
n5_arm2:                                    # 'Blue' — one edge left, re-arm
    PUSHCH  n5_arm3
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
    jmp     n3_arbno_fold
n5_arm3:                                    # LEN(1) — last edge, no re-arm
    mov     eax, r14d
    inc     eax
    cmp     eax, r15d
    ja      blob_ω
    inc     r14d
    jmp     n3_arbno_fold

# ═══ child 3 — RPOS(0).  rsp is at a WILDLY dynamic depth here; [E+S3] is static ═══
n4_rpos0_α:
    cmp     r14d, r15d
    jne     blob_ω
    mov     [rbx + S3 + 0], r14d
    mov     dword ptr [rbx + S3 + 4], 0

# ═══ IR_SEQUENCE γ — fold the n slots it allocated ═══
n1_seq_γ:
    mov     r8d, [rbx + S1 + 0]             # start ← slot1.start
    mov     r9d, [rbx + S1 + 4]
    add     r9d, [rbx + S2 + 4]
    add     r9d, [rbx + S3 + 4]             # len ← Σ slot lens
    mov     rax, rbx
    sub     rax, rsp                        # how deep the instances actually went
    PUTDEC  eax, ev_d

    mov     eax, 1
    mov     edi, 1
    lea     rsi, [rip + ev]
    mov     edx, OFFSET ev_len
    syscall
    jmp     n0_head_γ

blob_ω:
    mov     rsp, rbp
    mov     rax,  [rbp + 0]
    mov     r14d, [rbp + 8]
    mov     rbx,  [rbp + 24]                # E restored — with δ, by the same cut
    mov     rbp,  [rbp + 16]
    add     rsp, 32
    jmp     rax

n0_head_β:
    cmp     r14d, r15d
    jae     n0_head_ω
    inc     r14d
    jmp     n0_head_repush

n0_head_γ:
    PUTDEC  r8d, msg_s
    mov     eax, r8d
    add     eax, r9d
    PUTDEC  eax, msg_e
    mov     eax, 1
    mov     edi, 1
    lea     rsi, [rip + msg]
    mov     edx, OFFSET msg_len
    syscall
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
    mov     eax, 1
    mov     edi, 1
    lea     rsi, [rip + nomsg]
    mov     edx, OFFSET nomsg_len
    syscall
    mov     edi, 1
    mov     eax, 60
    syscall

    .data
subj:   .ascii  "BlueGoldBirdFish"
ev:     .ascii  "  [prefix] slots STATIC at E+16/+32/+48 while rsp sank "
ev_d:   .ascii  "?????"
        .ascii  " bytes below E\n"
    .set ev_len, . - ev
msg:    .ascii  "CELL6 MATCH start="
msg_s:  .ascii  "?????"
        .ascii  " end="
msg_e:  .ascii  "?????"
        .ascii  " text="
    .set msg_len, . - msg
nl:     .byte   10
nomsg:  .ascii  "CELL6 NOMATCH\n"
    .set nomsg_len, . - nomsg
