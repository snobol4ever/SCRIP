# test_pl_zeta_1.s — PZ-1(a) SEED: the THREE-ZETAS Prolog machine, hand-written (GOAL-PROLOG-100, 2026-08-21 seat2).
# Embodies:  f(a). f(b).   g(a) :- !.  g(b).
#            main :- ( f(X), write(X), nl, fail ; true ), ( g(Y), write(Y), nl, fail ; true ).
# Oracle twin: test_pl_zeta_1.pl (gprolog 1.6.0) -> "a\nb\na\n".
#
# THE SHAPE BEING RULED ON (Lon: PZ-1(a) seed ruling):
#   ζ-SPINE (rsp)     : the logic variable X is ONE 16B spine cell carved by the statement; unbound = 0,
#                       bound = Term* (seed: ptr to atom byte).  Non-popping: nothing below a live cell pops it.
#   CHOICE RECORD     : 32B {resume @+0 · TAG @+8 (1=BASE, 0=CHOICE, 2=FENCE/cut) · prev_CP @+16 · saved_MH @+24},
#                       homed IN the activation (PF-1b: a disjunction pushes NO frame of its own) at a
#                       compile-time offset, so E = record+56 is DERIVED and no EB field is needed.
#                       CP register = r15 (production: the record-home discipline; ___-adjacent).
#   ζ-ACTIVATION (rbp): predicate activation pinned at α (push rbp; mov rbp,rsp).  γ RETAINS: no rsp unwind,
#                       caller E restored from [rbp], own base handed in rax.  ω TEARS DOWN: mov rsp,rbp; pop rbp.
#   TRAIL (r12 index) : the undo arena twin (RULING 4).  Binding pushes &cell; backtrack unwinds to the
#                       record's saved_MH; cut does NOT unwind (bindings below the mark stay committed).
#   WIRES             : r10=γ / r11=ω, FUNCTION LINKAGE = BLOB INTERFACE (jmp, never call), WIRE-ORDER
#                       push r11; push r10 => γ @ E-16, ω @ E-8.  Statement failure barrier = the statement's
#                       OWN BASE record (resume = its ω arm), so `fail` is uniformly `jmp [r15]`.
#   CUT               : pop the CP chain to the activation's barrier: r15 = [r15+16].  Memory above stays
#                       until ω / final whack (WAM: cut moves B, reclaims nothing) — LIFO discipline.
# Statement-terminal release: each statement releases its own barrier record (the op_zgpop twin);
# the final whack is main's ω collapsing to its pinned base.
# Build:  as test_pl_zeta_1.s -o t.o && gcc -no-pie t.o -o t && ./t
    .intel_syntax noprefix
    .section .rodata
A:  .byte 'a'
B:  .byte 'b'
NL: .byte 10
    .bss
    .align 8
TRAIL: .zero 256              # seed-only arena; production = runtime trail (base-pinned, RULING 4)
    .text
    .globl main
put:                          # write(1, rsi, 1).  syscall clobbers rcx/r11 — called only from caller-side
    mov eax, 1                # code where the wires are frame-saved, never register-live.
    mov edi, 1
    mov edx, 1
    syscall
    ret
main:
    push rbp
    mov rbp, rsp              # main's pinned base (top-graph ζ-ACTIVATION)
    push r15                  # SysV callee-saved hygiene (seed runs under crt)
    push r12
    sub rsp, 16               # ζ-SPINE: X cell at [rbp-32]
    mov qword ptr [rbp-32], 0 # X unbound
    xor r12d, r12d            # trail top = 0
    # statement 1 BARRIER record (resume = the "; true" arm) — fail is uniformly jmp [r15]
    lea rax, [rip+main_o]
    push 0                    # saved_MH
    push 0                    # prev_CP (chain end)
    push 1                    # TAG = BASE
    push rax                  # resume
    mov r15, rsp
    lea r10, [rip+main_g]     # γ wire
    lea r11, [rip+main_o]     # ω wire
    lea rdi, [rbp-32]         # &X
    jmp f_alpha               # wires-only linkage
main_g:                       # caller γ landing: rbp = main's base (restored by callee), rsp = retained frontier
    mov rsi, [rbp-32]
    call put                  # write(X)
    lea rsi, [rip+NL]
    call put
    jmp qword ptr [r15]       # fail -> newest CP's resume
main_o:                       # statement 1 done (via f's ω wire, or via the barrier record)
    add rsp, 32               # statement-terminal release of the barrier record (op_zgpop twin)
    mov qword ptr [rbp-32], 0 # Y fresh (reuses the cell)
    lea rax, [rip+main_o2]
    push 0
    push 0
    push 1
    push rax
    mov r15, rsp
    lea r10, [rip+main_g2]
    lea r11, [rip+main_o2]
    lea rdi, [rbp-32]
    jmp g_alpha
main_g2:
    mov rsi, [rbp-32]
    call put
    lea rsi, [rip+NL]
    call put
    jmp qword ptr [r15]       # after g's cut this is the STATEMENT barrier -> main_o2 (prints only "a")
main_o2:                      # final whack: collapse to main's pinned base
    lea rsp, [rbp-16]
    pop r12
    pop r15
    pop rbp
    xor eax, eax
    ret
#=====================================================================================================
f_alpha:                      # f(a). f(b).    in: rdi=&X, r10=γ, r11=ω.   α CARVES:
    push rbp
    mov rbp, rsp              # E := own base; [rbp] = caller E
    push r11                  # ω @ E-8    (WIRE-ORDER pair)
    push r10                  # γ @ E-16
    push rdi                  # &X @ E-24
    lea rax, [rip+f_c2]       # BASE record homed in the activation: E = record+56 (derived, no EB)
    push r12                  # saved_MH @ +24
    push r15                  # prev_CP  @ +16
    push 1                    # TAG BASE @ +8
    push rax                  # resume   @ +0  (= clause 2)
    mov r15, rsp              # CP := this record
f_c1:                         # clause 1: X = a
    mov rax, [rbp-24]
    lea rcx, [rip+A]
    mov [rax], rcx            # bind
    lea rcx, [rip+TRAIL]
    mov rdx, [rbp-24]
    mov [rcx+r12], rdx        # trail &X
    add r12, 8
    mov rdx, [rbp-16]         # γ wire (read before E moves)
    mov rax, rbp              # hand own base (retain protocol)
    mov rbp, [rbp]            # E := caller   —  γ RETAINS: rsp untouched
    jmp rdx
f_c2:                         # β-resume: clause 2.  Entered by jmp [r15]; rsp unknown-deep — record-relative only.
    lea rbp, [r15+56]         # re-anchor E from the record (the PF-1b derivation)
    mov rdx, [r15+24]         # trail unwind to saved_MH
1:  cmp r12, rdx
    jbe 2f
    sub r12, 8
    lea rcx, [rip+TRAIL]
    mov rax, [rcx+r12]
    mov qword ptr [rax], 0    # unbind
    jmp 1b
2:  lea rax, [rip+f_x]
    mov [r15], rax            # next resume = exhausted
    mov rax, [rbp-24]
    lea rcx, [rip+B]
    mov [rax], rcx            # bind X = b
    lea rcx, [rip+TRAIL]
    mov rdx, [rbp-24]
    mov [rcx+r12], rdx
    add r12, 8
    mov rdx, [rbp-16]
    mov rax, rbp
    mov rbp, [rbp]
    jmp rdx                   # γ retains again
f_x:                          # exhausted = ω: unwind trail, pop CP, TEAR DOWN, jump ω wire
    lea rbp, [r15+56]
    mov rdx, [r15+24]
1:  cmp r12, rdx
    jbe 2f
    sub r12, 8
    lea rcx, [rip+TRAIL]
    mov rax, [rcx+r12]
    mov qword ptr [rax], 0
    jmp 1b
2:  mov r15, [r15+16]         # pop CP to prev (= the statement barrier)
    mov rdx, [rbp-8]          # ω wire (read before the whack)
    mov rsp, rbp              # ω tears down the whole activation
    pop rbp                   # E := caller; rsp = entry frontier
    jmp rdx
#=====================================================================================================
g_alpha:                      # g(a) :- !.  g(b).   — the cut shape
    push rbp
    mov rbp, rsp
    push r11
    push r10
    push rdi
    lea rax, [rip+g_c2]
    push r12
    push r15
    push 1
    push rax
    mov r15, rsp
g_c1:                         # clause 1: X = a, then !
    mov rax, [rbp-24]
    lea rcx, [rip+A]
    mov [rax], rcx
    lea rcx, [rip+TRAIL]
    mov rdx, [rbp-24]
    mov [rcx+r12], rdx
    add r12, 8
    mov r15, [r15+16]         # ! CUT: pop CP chain to the activation's barrier.  No trail unwind
                              # (bindings below the mark stay committed); no memory reclaimed (WAM law).
    mov rdx, [rbp-16]
    mov rax, rbp
    mov rbp, [rbp]
    jmp rdx                   # γ retains
g_c2:                         # would be clause 2 — unreachable after the cut (record left in place, chain popped)
    lea rbp, [r15+56]
    mov rdx, [rbp-8]
    mov rsp, rbp
    pop rbp
    jmp rdx
    .section .note.GNU-stack,"",@progbits
