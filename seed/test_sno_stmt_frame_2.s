# fact_target.s — HAND EMBODIMENT WITNESS 2 (s21x-c): recursion + FRETURN under the design of
# record (RBP/RSP FRAMES + FORTH-STYLE VARIABLE-LENGTH ζ CELLS, statement scope law).
# Same program as fact.sno; oracle: "120" then "7".
#
#   DEFINE('FACT(N)')             :(MAIN1)
#   FACT    LE(N,1)               :F(RECURSE)
#           FACT = 1              :(RETURN)
#   RECURSE FACT = N * FACT(N - 1):(RETURN)
#   MAIN1   DEFINE('NOPE()')      :(MAIN)
#   NOPE                          :(FRETURN)
#   MAIN    OUTPUT = FACT(5)
#           OUTPUT = NOPE()
#           OUTPUT = 7
#   END
#
# WHAT THIS PROVES BEYOND WITNESS 1:
#   - RECURSION: five nested activations, each = fresh 32B save slots + fresh 32B IR_CALL header
#     on the machine stack, LIFO; per-BB operand cells snapshot globals BEFORE the inner call
#     rebinds them, so dynamic scoping is correct by construction. No .bss arenas, no depth
#     counters, no frame_bytes — depth costs stack, exactly SPITBOL's own documented contract.
#   - FRETURN / the ω wire: NOPE() always FRETURNs; the failing call fails its statement; the
#     statement bracket cut reclaims every live cell with zero hand-counted pops; execution
#     continues at the next statement (prints 7).
#   - Per-call return wires: each call site owns its γ/ω labels; the wires ride the frame header,
#     so recursion needs nothing else.
#   - TWO constant-folded DEFINEs emit NOTHING at runtime (no stubs, no registration).
#   - nparams=0 save/restore variant (NOPE: one 16B slot, fname only; the slot is REUSED as the
#     call's RESULT cell on γ — net +16 for a zero-arg call).
# SCOPE NOTE: LE emitted as the runtime's own fast shape — rt_cmp_d on the two operand cells
# (legal for int/real tags; this witness is int-only by construction). The full op75 coercion
# tower is emitter work, out of embodiment scope.
 .intel_syntax noprefix
 .text

#==== FACT body — three statements, no frame of its own ======================================
# rbp on entry anchors the IR_CALL header: [rbp+0]=pad [rbp+8]=caller rbp [rbp+16]=γ [rbp+24]=ω
 .globl FACT_body
FACT_body:
#--- FACT   LE(N,1)   :F(RECURSE) ------------------------------------------------------------
stmt_FACT_1:
 push rbp
 mov rbp, rsp
 sub rsp, 8
 sub rsp, 16                               # BB var N
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rsp + 0], rax
 mov qword ptr [rsp + 8], rdx
 sub rsp, 16                               # BB lit 1
 mov qword ptr [rsp + 0], 6
 mov rax, qword ptr [rip + .Lk1a]
 mov qword ptr [rsp + 8], rax
b_le_N_1:                                  # BB predicate: ( a b — ), value discarded
 lea rdi, [rsp + 16]
 lea rsi, [rsp + 0]
 call rt_cmp_d@PLT
 test eax, eax
 jg stmt_FACT_1_fail                       # N > 1 -> LE fails -> :F(RECURSE)
stmt_FACT_1_end:                           # success: bracket cut discards both cells
 mov rsp, rbp
 pop rbp
 jmp stmt_FACT_2                           # unguarded success -> next statement
stmt_FACT_1_fail:
 mov rsp, rbp
 pop rbp
 jmp stmt_FACT_3                           # :F(RECURSE)
.Lk1a: .quad 1
#--- FACT = 1   :(RETURN) --------------------------------------------------------------------
stmt_FACT_2:
 push rbp
 mov rbp, rsp
 sub rsp, 8
 sub rsp, 16                               # BB lit 1
 mov qword ptr [rsp + 0], 6
 mov rax, qword ptr [rip + .Lk1b]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [rsp + 0]              # BB assign fname global, net -16
 mov rdx, qword ptr [rsp + 8]
 add rsp, 16
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov rsp, rbp
 pop rbp
 jmp RETURN_floater                        # :(RETURN)
.Lk1b: .quad 1
#--- RECURSE  FACT = N * FACT(N - 1)   :(RETURN) ---------------------------------------------
stmt_FACT_3:
 push rbp
 mov rbp, rsp
 sub rsp, 8
 sub rsp, 16                               # BB var N (LEFT operand — snapshot BEFORE rebind)
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rsp + 0], rax
 mov qword ptr [rsp + 8], rdx
 sub rsp, 16                               # BB var N (inner operand)
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rsp + 0], rax
 mov qword ptr [rsp + 8], rdx
 sub rsp, 16                               # BB lit 1
 mov qword ptr [rsp + 0], 6
 mov rax, qword ptr [rip + .Lk1c]
 mov qword ptr [rsp + 8], rax
b_sub_N_1:                                 # BB binop: ( n 1 — n-1 ), net -16
 mov eax, dword ptr [rsp + 16]
 cmp eax, 6
 jne .Lb_sub_slow
 mov eax, dword ptr [rsp + 0]
 cmp eax, 6
 jne .Lb_sub_slow
 mov rax, qword ptr [rsp + 24]
 mov rcx, qword ptr [rsp + 8]
 sub rax, rcx
 add rsp, 16
 mov qword ptr [rsp + 0], 6
 mov qword ptr [rsp + 8], rax
 jmp b_save_restore_FACT_r
.Lb_sub_slow:
 mov rdi, qword ptr [rsp + 16]
 mov rsi, qword ptr [rsp + 24]
 mov rdx, qword ptr [rsp + 0]
 mov rcx, qword ptr [rsp + 8]
 call rt_sub@PLT
 cmp eax, 99
 je stmt_FACT_3_fail                       # fail -> statement fails -> :(RETURN) either way
 add rsp, 16
 mov qword ptr [rsp + 0], rax
 mov qword ptr [rsp + 8], rdx
b_save_restore_FACT_r:                     # BB IR_SAVE_RESTORE — the only ζ in the call
 sub rsp, 32
 mov rax, qword ptr [1879052304]
 mov qword ptr [rsp + 0], rax              # save N
 mov rax, qword ptr [1879052312]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [1879052288]
 mov qword ptr [rsp + 16], rax             # save FACT (fname)
 mov rax, qword ptr [1879052296]
 mov qword ptr [rsp + 24], rax
 mov rax, qword ptr [rsp + 32]
 mov qword ptr [1879052304], rax           # bind N <- arg cell
 mov rax, qword ptr [rsp + 40]
 mov qword ptr [1879052312], rax
 mov qword ptr [1879052288], 0             # fname <- null
 mov qword ptr [1879052296], 0
b_call_FACT_r:                             # BB IR_CALL — frame setup, direct jmp; RECURSION
 lea rcx, [rip + .Lret_γ_Fr]
 lea rdx, [rip + .Lret_ω_Fr]
 push rdx
 push rcx
 push rbp
 sub rsp, 8
 mov rbp, rsp
 jmp FACT_body
.Lret_γ_Fr:
 mov rax, qword ptr [1879052288]           # return value = fname global at RETURN
 mov rcx, qword ptr [1879052296]
 mov rdx, qword ptr [rsp + 16]
 mov qword ptr [1879052288], rdx           # restore FACT
 mov rdx, qword ptr [rsp + 24]
 mov qword ptr [1879052296], rdx
 mov rdx, qword ptr [rsp + 0]
 mov qword ptr [1879052304], rdx           # restore N
 mov rdx, qword ptr [rsp + 8]
 mov qword ptr [1879052312], rdx
 add rsp, 32
 mov qword ptr [rsp + 0], rax              # ( arg — result )
 mov qword ptr [rsp + 8], rcx
 jmp b_mul_N_res
.Lret_ω_Fr:
 mov rdx, qword ptr [rsp + 16]
 mov qword ptr [1879052288], rdx
 mov rdx, qword ptr [rsp + 24]
 mov qword ptr [1879052296], rdx
 mov rdx, qword ptr [rsp + 0]
 mov qword ptr [1879052304], rdx
 mov rdx, qword ptr [rsp + 8]
 mov qword ptr [1879052312], rdx
 add rsp, 32
 jmp stmt_FACT_3_fail
b_mul_N_res:                               # BB binop: ( N res — product ), net -16
 mov eax, dword ptr [rsp + 16]
 cmp eax, 6
 jne .Lb_mul_slow
 mov eax, dword ptr [rsp + 0]
 cmp eax, 6
 jne .Lb_mul_slow
 mov rax, qword ptr [rsp + 24]
 mov rcx, qword ptr [rsp + 8]
 imul rax, rcx
 add rsp, 16
 mov qword ptr [rsp + 0], 6
 mov qword ptr [rsp + 8], rax
 jmp b_assign_FACT_r
.Lb_mul_slow:
 mov rdi, qword ptr [rsp + 16]
 mov rsi, qword ptr [rsp + 24]
 mov rdx, qword ptr [rsp + 0]
 mov rcx, qword ptr [rsp + 8]
 call rt_mul@PLT
 cmp eax, 99
 je stmt_FACT_3_fail
 add rsp, 16
 mov qword ptr [rsp + 0], rax
 mov qword ptr [rsp + 8], rdx
b_assign_FACT_r:                           # consumes TOS -> fname global, net -16
 mov rax, qword ptr [rsp + 0]
 mov rdx, qword ptr [rsp + 8]
 add rsp, 16
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov rsp, rbp
 pop rbp
 jmp RETURN_floater                        # :(RETURN)
stmt_FACT_3_fail:
 mov rsp, rbp
 pop rbp
 jmp RETURN_floater                        # unconditional goto: S and F both :(RETURN)
.Lk1c: .quad 1

#==== NOPE body — zero statements, always FRETURN ============================================
NOPE_body:
 jmp FRETURN_floater

#==== floaters (program-wide, IR_SAVE_RESTORE roles 1/2) =====================================
RETURN_floater:
 mov rsp, rbp
 mov rcx, qword ptr [rbp + 16]
 mov rbp, qword ptr [rbp + 8]
 add rsp, 32
 jmp rcx
FRETURN_floater:
 mov rsp, rbp
 mov rcx, qword ptr [rbp + 24]
 mov rbp, qword ptr [rbp + 8]
 add rsp, 32
 jmp rcx

#==== startup ================================================================================
 .section .rodata
.Lgvan0: .string "FACT"
.Lgvan1: .string "N"
.Lgvan2: .string "NOPE"
 .align 8
__gva_names:
 .quad .Lgvan0
 .quad .Lgvan1
 .quad .Lgvan2
 .section .text
 .intel_syntax noprefix
 .globl main
main:
 sub rsp, 8
 push rdi
 push rsi
 call core_lib_init@PLT
 mov edi, 3
 call rt_gva_island@PLT
 mov rsi, rax
 lea rdi, [rip + __gva_names]
 mov edx, 3
 call gva_register@PLT
 xor esi, esi
 call main_α
 xor eax, eax
 add rsp, 24
 ret

#==== main graph — parity pad only ===========================================================
main_α:
 sub rsp, 8
#--- DEFINE('FACT(N)') / DEFINE('NOPE()') --- CONSTANT FOLDED: nothing at runtime -------------
#--- MAIN   OUTPUT = FACT(5) -----------------------------------------------------------------
stmt_MAIN_1:
 push rbp
 mov rbp, rsp
 sub rsp, 8
 sub rsp, 16                               # BB lit 5
 mov qword ptr [rsp + 0], 6
 mov rax, qword ptr [rip + .Lk5]
 mov qword ptr [rsp + 8], rax
b_save_restore_FACT_m:                     # IR_SAVE_RESTORE
 sub rsp, 32
 mov rax, qword ptr [1879052304]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052312]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [1879052288]
 mov qword ptr [rsp + 16], rax
 mov rax, qword ptr [1879052296]
 mov qword ptr [rsp + 24], rax
 mov rax, qword ptr [rsp + 32]
 mov qword ptr [1879052304], rax
 mov rax, qword ptr [rsp + 40]
 mov qword ptr [1879052312], rax
 mov qword ptr [1879052288], 0
 mov qword ptr [1879052296], 0
b_call_FACT_m:                             # IR_CALL
 lea rcx, [rip + .Lret_γ_Fm]
 lea rdx, [rip + .Lret_ω_Fm]
 push rdx
 push rcx
 push rbp
 sub rsp, 8
 mov rbp, rsp
 jmp FACT_body
.Lret_γ_Fm:
 mov rax, qword ptr [1879052288]
 mov rcx, qword ptr [1879052296]
 mov rdx, qword ptr [rsp + 16]
 mov qword ptr [1879052288], rdx
 mov rdx, qword ptr [rsp + 24]
 mov qword ptr [1879052296], rdx
 mov rdx, qword ptr [rsp + 0]
 mov qword ptr [1879052304], rdx
 mov rdx, qword ptr [rsp + 8]
 mov qword ptr [1879052312], rdx
 add rsp, 32
 mov qword ptr [rsp + 0], rax
 mov qword ptr [rsp + 8], rcx
 jmp b_assign_OUT_1
.Lret_ω_Fm:
 mov rdx, qword ptr [rsp + 16]
 mov qword ptr [1879052288], rdx
 mov rdx, qword ptr [rsp + 24]
 mov qword ptr [1879052296], rdx
 mov rdx, qword ptr [rsp + 0]
 mov qword ptr [1879052304], rdx
 mov rdx, qword ptr [rsp + 8]
 mov qword ptr [1879052312], rdx
 add rsp, 32
 jmp stmt_MAIN_1_fail
b_assign_OUT_1:                            # consumes TOS via NV write, net -16
 mov rsi, qword ptr [rsp + 0]
 mov rdx, qword ptr [rsp + 8]
 mov rdi, qword ptr [rip + .Loutname]
 call NV_SET_fn@PLT
 add rsp, 16
stmt_MAIN_1_end:
 mov rsp, rbp
 pop rbp
 jmp stmt_MAIN_2
stmt_MAIN_1_fail:
 mov rsp, rbp
 pop rbp
 jmp stmt_MAIN_2
.Lk5: .quad 5
#--- OUTPUT = NOPE() -------------------------------------------------------------------------
stmt_MAIN_2:
 push rbp
 mov rbp, rsp
 sub rsp, 8
b_save_restore_NOPE:                       # nparams=0 variant: ONE 16B slot (fname only)
 sub rsp, 16
 mov rax, qword ptr [1879052320]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052328]
 mov qword ptr [rsp + 8], rax
 mov qword ptr [1879052320], 0
 mov qword ptr [1879052328], 0
b_call_NOPE:                               # IR_CALL
 lea rcx, [rip + .Lret_γ_N]
 lea rdx, [rip + .Lret_ω_N]
 push rdx
 push rcx
 push rbp
 sub rsp, 8
 mov rbp, rsp
 jmp NOPE_body
.Lret_γ_N:                                 # (never taken here; emitted correct) — the save
 mov rax, qword ptr [1879052320]           # slot is REUSED as the RESULT cell: net +16
 mov rcx, qword ptr [1879052328]
 mov rdx, qword ptr [rsp + 0]
 mov qword ptr [1879052320], rdx
 mov rdx, qword ptr [rsp + 8]
 mov qword ptr [1879052328], rdx
 mov qword ptr [rsp + 0], rax
 mov qword ptr [rsp + 8], rcx
 mov rsi, qword ptr [rsp + 0]
 mov rdx, qword ptr [rsp + 8]
 mov rdi, qword ptr [rip + .Loutname]
 call NV_SET_fn@PLT
 add rsp, 16
 jmp stmt_MAIN_2_end
.Lret_ω_N:                                 # FRETURN path: restore, statement fails
 mov rdx, qword ptr [rsp + 0]
 mov qword ptr [1879052320], rdx
 mov rdx, qword ptr [rsp + 8]
 mov qword ptr [1879052328], rdx
 add rsp, 16
 jmp stmt_MAIN_2_fail
stmt_MAIN_2_end:
 mov rsp, rbp
 pop rbp
 jmp stmt_MAIN_3
stmt_MAIN_2_fail:                          # unguarded: failure continues
 mov rsp, rbp
 pop rbp
 jmp stmt_MAIN_3
#--- OUTPUT = 7 ------------------------------------------------------------------------------
stmt_MAIN_3:
 push rbp
 mov rbp, rsp
 sub rsp, 8
 sub rsp, 16                               # BB lit 7
 mov qword ptr [rsp + 0], 6
 mov rax, qword ptr [rip + .Lk7]
 mov qword ptr [rsp + 8], rax
 mov rsi, qword ptr [rsp + 0]
 mov rdx, qword ptr [rsp + 8]
 mov rdi, qword ptr [rip + .Loutname]
 call NV_SET_fn@PLT
 add rsp, 16
 mov rsp, rbp
 pop rbp
 jmp main_γ
.Lk7: .quad 7
.Loutname: .quad .Loutname_s
.Loutname_s: .string "OUTPUT"
main_γ:
 mov eax, 1
 xor edx, edx
 add rsp, 8
 ret
main_ω:
 mov eax, 99
 xor edx, edx
 add rsp, 8
 ret
 .section .note.GNU-stack,"",@progbits
