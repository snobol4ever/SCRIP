  .intel_syntax noprefix
  .text
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  xor esi, esi
  call main_α
  xor eax, eax
  add rsp, 24
  ret
main_α:
#=======================================================================================================================
    .global main_α
    .global main_β
    .global main_γ
    .global main_ω
  sub rsp, 65544
  mov rdi, rsp
  mov ecx, 65544
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 280], rsp
  mov r12, qword ptr [1879048192]
  mov [rsp + 65536], rbp
  mov rbp, rsp
main_α_body:
# IR_LIT_INTEGER
 xchain0_n0_α:
 mov qword ptr [rbp + 144], 6
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rbp + 152], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp main_γ
.Lx1_0:
 .quad 2
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [rbp + 208], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [rbp + 216], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp main_γ
.Lx2_0:
 .quad 3
# IR_LIT_INTEGER
 xchain0_n2_α:
 mov qword ptr [rbp + 240], 6
 mov rax, qword ptr [rip + .Lx3_0]
 mov qword ptr [rbp + 248], rax
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp main_γ
.Lx3_0:
 .quad 4
 xchain0_n3_α:
# IR_BINOP_ARITH
.Lx4_1:
 mov rax, 3
 mov rcx, 4
 imul rax, rcx
 mov qword ptr [rbp + 176], 6
 mov qword ptr [rbp + 184], rax
 jmp xchain0_n4_α
.Lx4_0:
 mov rdi, qword ptr [rbp + 208]
 mov rsi, qword ptr [rbp + 216]
 mov rdx, qword ptr [rbp + 240]
 mov rcx, qword ptr [rbp + 248]
 mov r8d, 2
 lea r9, [rbp + 176]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx4_3
.Lx4_2:
 mov rdi, qword ptr [rbp + 208]
 mov rsi, qword ptr [rbp + 216]
 mov rdx, qword ptr [rbp + 240]
 mov rcx, qword ptr [rbp + 248]
 mov r8d, 2
 call rt_num_arith@PLT
 cmp eax, 99
 je main_γ
 mov qword ptr [rbp + 176], rax
 mov qword ptr [rbp + 184], rdx
.Lx4_3:
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp main_γ
 xchain0_n4_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [rbp + 176]
 cmp eax, 100
 je .Lx5_0
 mov eax, dword ptr [rbp + 176]
 cmp eax, 6
 jne .Lx5_2
.Lx5_1:
 mov rax, 2
 mov rcx, qword ptr [rbp + 184]
 add rax, rcx
 mov qword ptr [rbp + 112], 6
 mov qword ptr [rbp + 120], rax
 jmp xchain0_n5_α
.Lx5_0:
 mov rdi, qword ptr [rbp + 144]
 mov rsi, qword ptr [rbp + 152]
 mov rdx, qword ptr [rbp + 176]
 mov rcx, qword ptr [rbp + 184]
 mov r8d, 0
 lea r9, [rbp + 112]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx5_3
.Lx5_2:
 mov rdi, qword ptr [rbp + 144]
 mov rsi, qword ptr [rbp + 152]
 mov rdx, qword ptr [rbp + 176]
 mov rcx, qword ptr [rbp + 184]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je main_γ
 mov qword ptr [rbp + 112], rax
 mov qword ptr [rbp + 120], rdx
.Lx5_3:
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp main_γ
# IR_ASSIGN global
 xchain0_n5_α:
 mov rsi, qword ptr [rbp + 112]
 mov rdx, qword ptr [rbp + 120]
 mov rdi, qword ptr [rip + .Lx6_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 96], rax
 mov qword ptr [rbp + 104], rdx
 jmp main_γ
 xchain0_n5_β:
 jmp main_γ
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rsp + 280]
mov rbp, [rsp + 65536]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 280]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
mov rbp, [rsp + 65536]
add rsp, 65544
ret
