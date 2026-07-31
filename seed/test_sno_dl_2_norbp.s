# test_sno_dl_2_norbp.s — DL-ONESHOT WITH ZERO RBP (s22e, 2026-07-31)
# =============================================================================================
# ⛔ SAME STATUS AS test_sno_dl_1.s: hand-written, assembles/links/executes with deterministic
#    stdout, and NOTHING ELSE.  Never through scrip/template/encoder/either medium; no watermark,
#    no crosscheck, no sbl oracle.  NOT a golden artifact.
#
# THE QUESTION (Lon, s22e): "Can you do it WITHOUT using RBP?"
# THE ANSWER THIS FILE MEASURES: YES — for the class whose EXIT DEPTH IS STATIC.  Not a trick and
#   not a weaker mechanism; it is the same DL-ONESHOT with the anchor deleted, because a graph
#   that knows its own K does not need a depth-immune base to find its way home.
#
# WHAT CHANGES vs test_sno_dl_1.s:
#   · The record SHRINKS 32B → 16B: {[+0] γ wire, [+8] ω wire}.  The `prev rbp` field is gone
#     (nothing to save) and the pad is gone.  2 pushes = 16B, so 16-parity still holds.
#   · The floater `mov rsp,rbp` becomes `add rsp,K` — the callee releases exactly what it carved,
#     which lands rsp back ON the record by LIFO construction.  Wires are then [rsp+0]/[rsp+8].
#   · rbp is NEVER READ AND NEVER WRITTEN in this file.  Verify: `grep -c rbp` == 0.
#
# ⛔ WHAT THIS DOES NOT DO, AND WHY THE GENERAL ANSWER IS STILL "NO":
#   `add rsp,K` requires K to be KNOWN AT THE RETURN SITE.  It is not, for the four constructs
#   law 4 already names — ARBNO extension, FENCE1, deep backtracking, and recursion — because
#   those reach their exit at a depth no compile-time sum can predict.  There the callee must ask
#   something depth-immune where home is, and that something is one register.  rbp is the cheapest
#   one available and FUNCTION is ALREADY one of law 4's four sanctioned RBP constructs, so
#   DL-ONESHOT-with-rbp is by the book, not a violation of THE MODEL.
#
# ⭐ THE USEFUL CONSEQUENCE — THE CLASSIFIER FOR THIS CLASS ALREADY EXISTS AND LANDED THIS WEEK:
#   LP-1 (s22d, SCRIP `23666d36`) computes `flat_all_zd` by pre-prologue entry-reachable BFS —
#   "every value-producing node on this graph passes zd_wl_kind" — and drops the carve to 8.  That
#   predicate IS the static-depth predicate.  LP-2 (moving the RPO walk + zd_plan ahead of
#   XA_FLAT_PROLOGUE so real plan results drive the verdict, not a kind scan) is what would let a
#   DL site READ it and pick this shape over the rbp shape per callee.  So the rung that makes DL
#   rbp-free for the determinate class is already the next item on the s22d NEXT list.
# BUILD:  as --64 -o /tmp/dl2.o seed/test_sno_dl_2_norbp.s && ld -o /tmp/dl2 /tmp/dl2.o && /tmp/dl2
# EXPECTED STDOUT, exactly:  Sf
# =============================================================================================
.intel_syntax noprefix
.section .data
slot: .quad fn_hit                        # dynamic binding, same as dl_1: target is DATA
.section .text
.globl _start
putc:                                     # al = char; balanced (push 8 / pop 8)
    push rax
    mov  rsi, rsp
    mov  rax, 1
    mov  rdi, 1
    mov  rdx, 1
    syscall
    pop  rax
    ret
# ---------------------------------------------------------------------------------------------
# Callees.  Each carves a STATIC K, so each releases exactly K and lands on the record.
# ---------------------------------------------------------------------------------------------
fn_hit:                                   # K = 32
    sub  rsp, 32
    mov  qword ptr [rsp], 83              # 'S' — a result living in its own cell
    mov  r8, [rsp]
    add  rsp, 32                          # ── the floater, rbp-free: release own K ──
    mov  rcx, [rsp+0]                     # γ wire, now at TOS by LIFO construction
    add  rsp, 16                          # pop the record
    jmp  rcx
fn_miss:                                  # K = 48, different depth, same discipline
    sub  rsp, 48
    add  rsp, 48
    mov  rcx, [rsp+8]                     # ω wire
    add  rsp, 16
    jmp  rcx
# ---------------------------------------------------------------------------------------------
_start:
    lea  rcx, [rip + g1]                  # build the 16B record: two wires, nothing else
    lea  rdx, [rip + o1]
    push rdx                              # [+8] ω
    push rcx                              # [+0] γ
    lea  rax, [rip + slot]
    mov  rax, [rax]
    jmp  rax                              # dynamic α entry
g1: mov  rax, r8
    call putc                             # 'S'
    lea  rax, [rip + slot]                # rebind the SAME site at runtime
    lea  rcx, [rip + fn_miss]
    mov  [rax], rcx
    jmp  two
o1: mov  rax, 102
    call putc
two:
    lea  rcx, [rip + g2]
    lea  rdx, [rip + o2]
    push rdx
    push rcx
    lea  rax, [rip + slot]
    mov  rax, [rax]
    jmp  rax
g2: mov  rax, r8
    call putc
    jmp  done
o2: mov  rax, 102                         # 'f' — ω wire, zero rbp involved
    call putc
done:
    mov  rax, 10
    call putc
    mov  rax, 60
    xor  rdi, rdi
    syscall
