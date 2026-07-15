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
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  mov qword ptr [rsp + 216], rsp
main_α_body:
# IR_LIT_INTEGER
 xchain0_n0_α:
 mov qword ptr [rsp + 144], 6
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rsp + 152], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp main_γ
.Lx1_0:
 .quad 6
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [rsp + 176], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [rsp + 184], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp main_γ
.Lx2_0:
 .quad 7
 xchain0_n2_α:
# IR_BINOP_ARITH
.Lx3_1:
 mov rax, 6
 mov rcx, 7
 imul rax, rcx
 mov qword ptr [rsp + 112], 6
 mov qword ptr [rsp + 120], rax
 jmp xchain0_n3_α
.Lx3_0:
 mov rdi, qword ptr [rsp + 144]
 mov rsi, qword ptr [rsp + 152]
 mov rdx, qword ptr [rsp + 176]
 mov rcx, qword ptr [rsp + 184]
 mov r8d, 2
 lea r9, [rsp + 112]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx3_3
.Lx3_2:
 mov rdi, qword ptr [rsp + 144]
 mov rsi, qword ptr [rsp + 152]
 mov rdx, qword ptr [rsp + 176]
 mov rcx, qword ptr [rsp + 184]
 mov r8d, 2
 call rt_num_arith@PLT
 cmp eax, 99
 je main_γ
 mov qword ptr [rsp + 112], rax
 mov qword ptr [rsp + 120], rdx
.Lx3_3:
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp main_γ
# IR_ASSIGN global
 xchain0_n3_α:
 mov rsi, qword ptr [rsp + 112]
 mov rdx, qword ptr [rsp + 120]
 mov rdi, qword ptr [rip + .Lx4_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 96], rax
 mov qword ptr [rsp + 104], rdx
 jmp main_γ
 xchain0_n3_β:
 jmp main_γ
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rsp + 216]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 216]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret
