  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "N"
  .align 8
__gva_names:
  .quad .Lgvan0
  .section .bss
  .align 16
__gva: .space 16, 0
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  lea rdi, [rip + __gva_names]
  lea rsi, [rip + __gva]
  mov edx, 1
  call gva_register@PLT
  mov rbx, rax
  sub rsp, 65536
  mov rdi, rsp
  mov ecx, 8192
  xor eax, eax
  rep stosq
  mov rdi, rsp
  xor esi, esi
  call main_α
  xor eax, eax
  add rsp, 65536
  add rsp, 24
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
  mov qword ptr [r12 + 200], rsp
 push rsi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 192], rax
 pop rsi
main_α_body:
# IR_LIT_INTEGER
 xchain0_n0_α:
 mov qword ptr [r12 + 112], 6
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n2_α
.Lx1_0:
 .quad 42
# IR_ASSIGN gva
 xchain0_n1_α:
 mov rax, qword ptr [r12 + 112]
 mov rdx, qword ptr [r12 + 120]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n2_α
# IR_VAR
 xchain0_n2_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp main_γ
# IR_ASSIGN global
 xchain0_n3_α:
 mov rsi, qword ptr [r12 + 160]
 mov rdx, qword ptr [r12 + 168]
 mov rdi, qword ptr [rip + .Lx4_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
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
mov rsp, qword ptr [r12 + 200]
pop r12
ret
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 192]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
main_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, qword ptr [r12 + 200]
pop r12
ret
