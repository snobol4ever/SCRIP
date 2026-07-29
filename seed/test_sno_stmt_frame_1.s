# add3_target.s — HAND EMBODIMENT (s21x-c) of the design of record: RBP/RSP FRAMES + FORTH-STYLE
# VARIABLE-LENGTH ζ CELLS, statement scope law. Same program as add3.sno; oracle output: 12.
#
#   DEFINE('ADD3(N)')             :(MAIN)
#   ADD3    ADD3 = N + 3          :(RETURN)
#   MAIN    OUTPUT = ADD3(4 * 2) + 1
#   END
#
# WHAT IS DIFFERENT FROM THE LEGACY EMISSION (add3_legacy.s):
#   - NO graph carves (legacy: sub rsp,240 / 240 / 216), NO zero-fill runs, NO granted flat slots.
#   - Every BB allocates its own RESULT (iff used) + LOCALS: ONE `sub rsp,16`; operands are the top
#     16B DESCR cells, indexed from RSP with compile-time sliding offsets.
#   - STATEMENT bracket = the rbp dance: push rbp; mov rbp,rsp; sub rsp,8 (parity) forward;
#     mov rsp,rbp; pop rbp backward. Fail edges jump the bracket cut — no hand-counted pops.
#   - Constant-folded DEFINE emits NOTHING at runtime (no stub proc, no rt_proc_register, no
#     frame_bytes, no wire_adopt, no goto_transfer). The call site is exactly TWO BBs:
#       IR_SAVE_RESTORE — 32B save slots (THE ONLY ζ IN THE CALL): save N + ADD3 globals,
#                         bind N <- arg cell, null the fname global (manual Ch.8).
#       IR_CALL         — sets up the STACK FRAME with RBP/RSP. That is all it does.
#                         header {pad, caller rbp, γ wire, ω wire}, then direct jmp to the body.
#   - RETURN floater = mov rsp,rbp cut (rolls back every statement/BB carve on the way backward),
#     restore caller rbp, jmp γ wire. Return value = the fname global at RETURN.
#   - main_α has no frame at all: one 8B parity pad; γ/ω are two-instruction exits.
#
# Register/ABI facts inherited verbatim from the legacy emission (the runtime contract):
#   DESCR = 16B {a,b}; integer tag 6 in a-low32; fail sentinel eax==99; rt_add/rt_mul args
#   rdi:rsi (left) rdx:rcx (right) -> rax:rdx; NV_SET_fn(rdi=name**, rsi:rdx=value);
#   GVA island absolute cells: ADD3=[1879052288,+8], N=[1879052304,+8].
 .intel_syntax noprefix
 .text

#==== LBL__ADD3 body — statements only; no frame of its own ==================================
# Arrives by direct jmp from IR_CALL; rbp anchors the call frame header:
#   [rbp+0]=pad  [rbp+8]=caller rbp  [rbp+16]=γ wire  [rbp+24]=ω wire
 .globl LBL__ADD3_body
LBL__ADD3_body:
#--- ADD3   ADD3 = N + 3   :(RETURN) ---------------------------------------------------------
stmt_ADD3_1:
 push rbp
 mov rbp, rsp
 sub rsp, 8                                # STATEMENT bracket (dance forward; 16B, parity kept)
b_var_N:                                   # BB var: RESULT used -> carve 16, one instruction
 sub rsp, 16
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rsp + 0], rax
 mov qword ptr [rsp + 8], rdx
b_lit_3:                                   # BB lit: RESULT used -> carve 16
 sub rsp, 16
 mov qword ptr [rsp + 0], 6
 mov rax, qword ptr [rip + .Lc3]
 mov qword ptr [rsp + 8], rax
b_add_N_3:                                 # BB binop: ( op1 op2 — r ), net -16, TOS-relative
 mov eax, dword ptr [rsp + 16]
 cmp eax, 6
 jne .Lb_add_N_3_slow
 mov eax, dword ptr [rsp + 0]
 cmp eax, 6
 jne .Lb_add_N_3_slow
 mov rax, qword ptr [rsp + 24]
 mov rcx, qword ptr [rsp + 8]
 add rax, rcx
 add rsp, 16
 mov qword ptr [rsp + 0], 6
 mov qword ptr [rsp + 8], rax
 jmp b_assign_ADD3
.Lb_add_N_3_slow:
 mov rdi, qword ptr [rsp + 16]
 mov rsi, qword ptr [rsp + 24]
 mov rdx, qword ptr [rsp + 0]
 mov rcx, qword ptr [rsp + 8]
 call rt_add@PLT
 cmp eax, 99
 je stmt_ADD3_1_fail                       # fail edge = bracket cut; ZERO hand-counted pops
 add rsp, 16
 mov qword ptr [rsp + 0], rax
 mov qword ptr [rsp + 8], rdx
b_assign_ADD3:                             # BB assign fname global: consumes TOS, net -16
 mov rax, qword ptr [rsp + 0]
 mov rdx, qword ptr [rsp + 8]
 add rsp, 16
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
stmt_ADD3_1_end:                           # bracket rollback (the way backward)
 mov rsp, rbp
 pop rbp
 jmp RETURN_floater                        # :(RETURN)
stmt_ADD3_1_fail:
 mov rsp, rbp
 pop rbp
 jmp RETURN_floater                        # unconditional goto: S and F both :(RETURN)
.Lc3: .quad 3

#==== RETURN floater (program-wide, IR_SAVE_RESTORE role 1) ==================================
RETURN_floater:
 mov rsp, rbp                              # cut: reclaim EVERY statement/BB carve at any depth
 mov rcx, qword ptr [rbp + 16]             # γ wire
 mov rbp, qword ptr [rbp + 8]              # restore caller rbp (dance backward)
 add rsp, 32                               # discard IR_CALL header
 jmp rcx
FRETURN_floater:
 mov rsp, rbp
 mov rcx, qword ptr [rbp + 24]             # ω wire
 mov rbp, qword ptr [rbp + 8]
 add rsp, 32
 jmp rcx

#==== startup ================================================================================
 .section .rodata
.Lgvan0: .string "ADD3"
.Lgvan1: .string "N"
 .align 8
__gva_names:
 .quad .Lgvan0
 .quad .Lgvan1
 .section .text
 .intel_syntax noprefix
 .globl main
main:
 sub rsp, 8
 push rdi
 push rsi
 call core_lib_init@PLT
 mov edi, 2
 call rt_gva_island@PLT
 mov rsi, rax
 lea rdi, [rip + __gva_names]
 mov edx, 2
 call gva_register@PLT
 xor esi, esi
 call main_α
 xor eax, eax
 add rsp, 24
 ret

#==== main graph — NO frame; 8B parity pad only ==============================================
main_α:
 sub rsp, 8
#--- DEFINE('ADD3(N)')  :(MAIN) --- CONSTANT FOLDED: nothing at runtime -----------------------
#--- MAIN   OUTPUT = ADD3(4 * 2) + 1 ---------------------------------------------------------
stmt_MAIN_1:
 push rbp
 mov rbp, rsp
 sub rsp, 8                                # STATEMENT bracket
b_lit_4:
 sub rsp, 16
 mov qword ptr [rsp + 0], 6
 mov rax, qword ptr [rip + .Lc4]
 mov qword ptr [rsp + 8], rax
b_lit_2:
 sub rsp, 16
 mov qword ptr [rsp + 0], 6
 mov rax, qword ptr [rip + .Lc2]
 mov qword ptr [rsp + 8], rax
b_mul:                                     # ( 4 2 — 8 ), net -16
 mov rdi, qword ptr [rsp + 16]
 mov rsi, qword ptr [rsp + 24]
 mov rdx, qword ptr [rsp + 0]
 mov rcx, qword ptr [rsp + 8]
 call rt_mul@PLT
 cmp eax, 99
 je stmt_MAIN_1_fail
 add rsp, 16
 mov qword ptr [rsp + 0], rax
 mov qword ptr [rsp + 8], rdx
#--- BB IR_SAVE_RESTORE (role 0): the ONLY ζ in the whole call process -----------------------
b_save_restore_ADD3:
 sub rsp, 32                               # its own LOCAL STORAGE: 2 save slots, ONE instruction
 mov rax, qword ptr [1879052304]
 mov qword ptr [rsp + 0], rax              # save N
 mov rax, qword ptr [1879052312]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [1879052288]
 mov qword ptr [rsp + 16], rax             # save ADD3 (fname)
 mov rax, qword ptr [1879052296]
 mov qword ptr [rsp + 24], rax
 mov rax, qword ptr [rsp + 32]
 mov qword ptr [1879052304], rax           # bind N <- arg cell (TOS below the slots)
 mov rax, qword ptr [rsp + 40]
 mov qword ptr [1879052312], rax
 mov qword ptr [1879052288], 0             # fname <- null (manual Ch.8 entry contract)
 mov qword ptr [1879052296], 0
#--- BB IR_CALL: sets up the STACK FRAME with RBP/RSP. That is all it does. ------------------
b_call_ADD3:
 lea rcx, [rip + .Lret_γ_1]
 lea rdx, [rip + .Lret_ω_1]
 push rdx                                  # [rbp+24] ω wire
 push rcx                                  # [rbp+16] γ wire
 push rbp                                  # [rbp+8]  caller rbp
 sub rsp, 8                                # [rbp+0]  pad (parity)
 mov rbp, rsp                              # frame pointer forward
 jmp LBL__ADD3_body                        # constant-folded: DIRECT jmp, no registry transfer
.Lret_γ_1:                                 # rsp -> save slots (floater consumed the header)
 mov rax, qword ptr [1879052288]           # return value = fname global at RETURN
 mov rcx, qword ptr [1879052296]
 mov rdx, qword ptr [rsp + 16]
 mov qword ptr [1879052288], rdx           # restore ADD3
 mov rdx, qword ptr [rsp + 24]
 mov qword ptr [1879052296], rdx
 mov rdx, qword ptr [rsp + 0]
 mov qword ptr [1879052304], rdx           # restore N
 mov rdx, qword ptr [rsp + 8]
 mov qword ptr [1879052312], rdx
 add rsp, 32                               # free the save slots
 mov qword ptr [rsp + 0], rax              # ( arg — result ): call box net 0
 mov qword ptr [rsp + 8], rcx
 jmp b_lit_1
.Lret_ω_1:                                 # FRETURN path: restore, then statement fail edge
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
b_lit_1:
 sub rsp, 16
 mov qword ptr [rsp + 0], 6
 mov rax, qword ptr [rip + .Lc1]
 mov qword ptr [rsp + 8], rax
b_add_call_1:                              # ( callres 1 — r ), net -16
 mov eax, dword ptr [rsp + 16]
 cmp eax, 6
 jne .Lb_add_call_1_slow
 mov eax, dword ptr [rsp + 0]
 cmp eax, 6
 jne .Lb_add_call_1_slow
 mov rax, qword ptr [rsp + 24]
 mov rcx, qword ptr [rsp + 8]
 add rax, rcx
 add rsp, 16
 mov qword ptr [rsp + 0], 6
 mov qword ptr [rsp + 8], rax
 jmp b_assign_OUTPUT
.Lb_add_call_1_slow:
 mov rdi, qword ptr [rsp + 16]
 mov rsi, qword ptr [rsp + 24]
 mov rdx, qword ptr [rsp + 0]
 mov rcx, qword ptr [rsp + 8]
 call rt_add@PLT
 cmp eax, 99
 je stmt_MAIN_1_fail
 add rsp, 16
 mov qword ptr [rsp + 0], rax
 mov qword ptr [rsp + 8], rdx
b_assign_OUTPUT:                           # consumes TOS via NV write, net -16
 mov rsi, qword ptr [rsp + 0]
 mov rdx, qword ptr [rsp + 8]
 mov rdi, qword ptr [rip + .Loutname]
 call NV_SET_fn@PLT
 add rsp, 16
stmt_MAIN_1_end:
 mov rsp, rbp
 pop rbp
 jmp main_γ
stmt_MAIN_1_fail:                          # unguarded statement: failure continues
 mov rsp, rbp
 pop rbp
 jmp main_γ
.Lc4: .quad 4
.Lc2: .quad 2
.Lc1: .quad 1
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
