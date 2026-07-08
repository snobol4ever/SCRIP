  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "X"
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
  push rbp
  mov rbp, rsp
  push rdi
  push rsi
  lea rdi, [rip + __gva_names]
  lea rsi, [rip + __gva]
  mov edx, 1
  call gva_register@PLT
  mov rbx, rax
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
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 160], rax
 pop rsi
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [r12 + 32], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 40], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n2_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "hello world"
# IR_ASSIGN gva
 xchain0_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n2_α
# IR_VAR gva
 xchain0_n2_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
# IR_MATCH_HEAD
 xchain0_n3_α:
 call rt_zls_mark@PLT
 mov qword ptr [r12 + 56], rax
 mov rdi, qword ptr [r12 + 80]
 mov rsi, qword ptr [r12 + 88]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov dword ptr [r12 + 48], 0
.Lx5_0:
 mov r14d, dword ptr [r12 + 48]
 jmp xchain0_n5_α
 xchain0_n3_β:
 add dword ptr [r12 + 48], 1
 mov eax, dword ptr [r12 + 48]
 cmp eax, r15d
 jg .Lx5_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx5_1
 jmp .Lx5_0
.Lx5_1:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 56]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n4_α
# IR_LIT_STRING
 xchain0_n4_α:
 mov qword ptr [r12 + 144], 1
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 152], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp main_γ
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string "not found"
# IR_MATCH_LIT
 xchain0_n5_α:
 mov eax, r14d
 add eax, 7
 cmp eax, r15d
 jg xchain0_n3_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S0]
 mov edx, 7
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain0_n3_β
 add r14d, 7
 jmp xchain0_n7_α
 xchain0_n5_β:
 sub r14d, 7
 jmp xchain0_n3_β
# IR_ASSIGN global
 xchain0_n6_α:
 mov rsi, qword ptr [r12 + 144]
 mov rdx, qword ptr [r12 + 152]
 mov rdi, qword ptr [rip + .Lx9_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 128], rax
 mov qword ptr [r12 + 136], rdx
 jmp main_γ
 xchain0_n6_β:
 jmp main_γ
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "OUTPUT"
# IR_MATCH_RELEASE
 xchain0_n7_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 56]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n8_α
# IR_LIT_STRING
 xchain0_n8_α:
 mov qword ptr [r12 + 112], 1
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain0_n9_α
 xchain0_n8_β:
 jmp main_γ
.Lx12_0:
 .quad .Lx12_0_s
.Lx12_0_s:
 .string "found"
# IR_ASSIGN global
 xchain0_n9_α:
 mov rsi, qword ptr [r12 + 112]
 mov rdx, qword ptr [r12 + 120]
 mov rdi, qword ptr [rip + .Lx13_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 jmp main_γ
 xchain0_n9_β:
 jmp main_γ
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 160]
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
pop r12
ret
.section .rodata
.S0: .string "goodbye"
.text
