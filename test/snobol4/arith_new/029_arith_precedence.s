  .intel_syntax noprefix
  .text
  .globl main
main:
  push rbp
  mov rbp, rsp
  call rt_frame@PLT
  mov rdi, rax
  xor esi, esi
  call main_α
  xor eax, eax
  pop rbp
  ret
main_α:
#=======================================================================================================================
    .global main_α
    .global main_β
    .global main_γ
    .global main_ω
push r12
  mov r12, rdi
main_α_body:
xchain0_n0_α:
# IR_LIT_INTEGER
bb1_α:
 mov qword ptr [r12 + 48], 6
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 56], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp main_ω
.Lx1_0:
 .quad 2
xchain0_n1_α:
# IR_LIT_INTEGER
bb2_α:
 mov qword ptr [r12 + 80], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 88], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp main_ω
.Lx2_0:
 .quad 3
xchain0_n2_α:
# IR_LIT_INTEGER
bb3_α:
 mov qword ptr [r12 + 96], 6
 mov rax, qword ptr [rip + .Lx3_0]
 mov qword ptr [r12 + 104], rax
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp main_ω
.Lx3_0:
 .quad 4
xchain0_n3_α:
bb4_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 80]
 cmp eax, 100
 je .Lx4_0
 mov eax, dword ptr [r12 + 96]
 cmp eax, 100
 je .Lx4_0
 mov eax, dword ptr [r12 + 80]
 cmp eax, 6
 jne .Lx4_2
 mov eax, dword ptr [r12 + 96]
 cmp eax, 6
 jne .Lx4_2
.Lx4_1:
 mov rax, qword ptr [r12 + 88]
 mov rcx, qword ptr [r12 + 104]
 imul rax, rcx
 mov qword ptr [r12 + 64], 6
 mov qword ptr [r12 + 72], rax
 jmp xchain0_n4_α
.Lx4_0:
 mov rdi, qword ptr [r12 + 80]
 mov rsi, qword ptr [r12 + 88]
 mov rdx, qword ptr [r12 + 96]
 mov rcx, qword ptr [r12 + 104]
 mov r8d, 2
 lea r9, [r12 + 64]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx4_3
.Lx4_2:
 mov rdi, qword ptr [r12 + 80]
 mov rsi, qword ptr [r12 + 88]
 mov rdx, qword ptr [r12 + 96]
 mov rcx, qword ptr [r12 + 104]
 mov r8d, 2
 call rt_num_arith@PLT
 cmp eax, 99
 je main_ω
 mov qword ptr [r12 + 64], rax
 mov qword ptr [r12 + 72], rdx
.Lx4_3:
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp main_ω
xchain0_n4_α:
bb5_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 48]
 cmp eax, 100
 je .Lx5_0
 mov eax, dword ptr [r12 + 64]
 cmp eax, 100
 je .Lx5_0
 mov eax, dword ptr [r12 + 48]
 cmp eax, 6
 jne .Lx5_2
 mov eax, dword ptr [r12 + 64]
 cmp eax, 6
 jne .Lx5_2
.Lx5_1:
 mov rax, qword ptr [r12 + 56]
 mov rcx, qword ptr [r12 + 72]
 add rax, rcx
 mov qword ptr [r12 + 32], 6
 mov qword ptr [r12 + 40], rax
 jmp xchain0_n5_α
.Lx5_0:
 mov rdi, qword ptr [r12 + 48]
 mov rsi, qword ptr [r12 + 56]
 mov rdx, qword ptr [r12 + 64]
 mov rcx, qword ptr [r12 + 72]
 mov r8d, 0
 lea r9, [r12 + 32]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx5_3
.Lx5_2:
 mov rdi, qword ptr [r12 + 48]
 mov rsi, qword ptr [r12 + 56]
 mov rdx, qword ptr [r12 + 64]
 mov rcx, qword ptr [r12 + 72]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je main_ω
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
.Lx5_3:
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp main_ω
xchain0_n5_α:
# IR_ASSIGN global
bb6_α:
 mov rsi, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov rdi, qword ptr [rip + .Lx6_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp main_γ
 xchain0_n5_β:
 jmp main_ω
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
pop r12
ret
main_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
pop r12
ret
