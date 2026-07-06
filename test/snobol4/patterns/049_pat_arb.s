  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "X"
  .Lgvan1: .string "V"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .section .bss
  .align 16
__gva: .space 32, 0
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
  mov edx, 2
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
main_α_body:
xchain0_n0_α:
# IR_LIT_STRING
bb1_α:
 mov qword ptr [r12 + 32], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 40], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n2_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "aXb"
xchain0_n1_α:
# IR_ASSIGN gva
bb2_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n2_α
xchain0_n2_α:
# IR_VAR gva
bb3_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n10_α
xchain0_n3_α:
# IR_MATCH_HEAD
bb4_α:
 mov rdi, qword ptr [r12 + 96]
 mov rsi, qword ptr [r12 + 104]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov dword ptr [r12 + 48], 0
.Lx5_0:
 mov r14d, dword ptr [r12 + 48]
 jmp xchain0_n4_α
 xchain0_n3_β:
 add dword ptr [r12 + 48], 1
 mov eax, dword ptr [r12 + 48]
 cmp eax, r15d
 jg xchain0_n10_α
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne xchain0_n10_α
 jmp .Lx5_0
xchain0_n4_α:
# IR_MATCH_CAPTURE_SAVE push
bb5_α:
 lea rdi, [r12 + 64]
 mov esi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_cap_push@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n5_α
 xchain0_n4_β:
 lea rdi, [r12 + 64]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_cap_pop@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n3_β
xchain0_n5_α:
# IR_MATCH_ARB
bb6_α:
 mov dword ptr [r12 + 80], 0
 mov eax, r14d
 mov dword ptr [r12 + 84], eax
 jmp xchain0_n6_α
 xchain0_n5_β:
 add dword ptr [r12 + 80], 1
 mov eax, dword ptr [r12 + 84]
 add eax, dword ptr [r12 + 80]
 cmp eax, r15d
 jg .Lx9_0
 mov r14d, eax
 jmp xchain0_n6_α
.Lx9_0:
 mov r14d, dword ptr [r12 + 84]
 jmp xchain0_n4_β
xchain0_n6_α:
# IR_MATCH_CAPTURE_COND
bb7_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 lea rdi, [r12 + 64]
 call rt_cap_top@PLT
 lea rdi, [rip + .S0]
 mov esi, eax
 mov edx, r14d
 mov ecx, 0
 call rt_cap_assign_cursor@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n7_α
 xchain0_n6_β:
 jmp xchain0_n5_β
xchain0_n7_α:
# IR_MATCH_LEN
bb8_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain0_n6_β
 add r14d, 1
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp xchain0_n6_β
xchain0_n8_α:
# IR_VAR gva
bb9_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 jmp xchain0_n9_α
 xchain0_n8_β:
 jmp main_γ
xchain0_n9_α:
# IR_ASSIGN global
bb10_α:
 mov rsi, qword ptr [r12 + 160]
 mov rdx, qword ptr [r12 + 168]
 mov rdi, qword ptr [rip + .Lx14_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
 jmp main_γ
 xchain0_n9_β:
 jmp main_γ
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "OUTPUT"
xchain0_n10_α:
# IR_LIT_STRING
bb11_α:
 mov qword ptr [r12 + 128], 1
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [r12 + 136], rax
 jmp xchain0_n11_α
 xchain0_n10_β:
 jmp main_γ
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string "fail"
xchain0_n11_α:
# IR_ASSIGN global
bb12_α:
 mov rsi, qword ptr [r12 + 128]
 mov rdx, qword ptr [r12 + 136]
 mov rdi, qword ptr [rip + .Lx16_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 112], rax
 mov qword ptr [r12 + 120], rdx
 jmp main_γ
 xchain0_n11_β:
 jmp main_γ
.Lx16_0:
 .quad .Lx16_0_s
.Lx16_0_s:
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
.section .rodata
.S0: .string "V"
.text
