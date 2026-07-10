  .intel_syntax noprefix
  .text
  .globl main
main:
  push rbp
  mov rbp, rsp
  push rdi
  push rsi
  call core_lib_init@PLT
  call rt_frame@PLT
  mov rdi, rax
  xor esi, esi
  call main_α
  xor eax, eax
  mov rsp, rbp
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
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 128], rax
 pop rsi
main_α_body:
# IR_LIT_INTEGER
 xchain0_n0_α:
 mov qword ptr [r12 + 96], 6
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 104], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp main_γ
.Lx1_0:
 .quad 6
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [r12 + 112], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp main_γ
.Lx2_0:
 .quad 7
 xchain0_n2_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 96]
 cmp eax, 100
 je .Lx3_0
 mov eax, dword ptr [r12 + 112]
 cmp eax, 100
 je .Lx3_0
 mov eax, dword ptr [r12 + 96]
 cmp eax, 6
 jne .Lx3_2
 mov eax, dword ptr [r12 + 112]
 cmp eax, 6
 jne .Lx3_2
.Lx3_1:
 mov rax, qword ptr [r12 + 104]
 mov rcx, qword ptr [r12 + 120]
 imul rax, rcx
 mov qword ptr [r12 + 80], 6
 mov qword ptr [r12 + 88], rax
 jmp xchain0_n3_α
.Lx3_0:
 mov rdi, qword ptr [r12 + 96]
 mov rsi, qword ptr [r12 + 104]
 mov rdx, qword ptr [r12 + 112]
 mov rcx, qword ptr [r12 + 120]
 mov r8d, 2
 lea r9, [r12 + 80]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx3_3
.Lx3_2:
 mov rdi, qword ptr [r12 + 96]
 mov rsi, qword ptr [r12 + 104]
 mov rdx, qword ptr [r12 + 112]
 mov rcx, qword ptr [r12 + 120]
 mov r8d, 2
 call rt_num_arith@PLT
 cmp eax, 99
 je main_γ
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
.Lx3_3:
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp main_γ
# IR_ASSIGN global
 xchain0_n3_α:
 mov rsi, qword ptr [r12 + 80]
 mov rdx, qword ptr [r12 + 88]
 mov rdi, qword ptr [rip + .Lx4_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 64], rax
 mov qword ptr [r12 + 72], rdx
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
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 128]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
main_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
